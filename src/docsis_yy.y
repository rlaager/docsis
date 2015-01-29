/*
 *  DOCSIS configuration file encoder.
 *  Copyright (c) 2001,2005 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002,2003,2004 Evvolve Media SRL,office@evvolve.com
 *  Copyright (c) 2014 - 2015 Adrian Simionov, daniel.simionov@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  DOCSIS is a registered trademark of Cablelabs, http://www.cablelabs.com
 */

%{
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "docsis.h"
#include "docsis_encode.h"
#include "docsis_snmp.h"
#if YYTEXT_POINTER
extern char *yytext;
#else
extern char yytext[];
#endif

extern unsigned int line; 	/* current line number, defined in a.l */
extern struct tlv *global_tlvtree_head; /* Global list of all config TLVs */
extern symbol_type *global_symtable;
extern FILE *yyin;

struct tlv *_my_tlvtree_head;

%}

%union { 	/* Token types */
	int intval;			/* For integers */
	unsigned int uintval;		/* For unsigned integers */
	symbol_type *symptr; 		/* For token identifiers */
	char *strval;		 	/* For strings */
	unsigned char *ustrval; 	/* For any other types */
	unsigned int ip;		/* For IP Addresses */
	struct tlv_list *tlvlist; 	/* For for struct tlvlist pointers */
	struct tlv *tlvptr;		/* For struct tlv pointers; */
}

%token <symptr>  T_IDENTIFIER
%token <uintval> T_INTEGER
%token <symptr>  T_IDENT_COS
%token <symptr>  T_IDENT_BPI
%token <symptr>  T_IDENT_SNMPW
%token <symptr>  T_IDENT_SNMPSET
%token <symptr>  T_IDENT_GENERIC
%token <symptr>  T_IDENT_CVC
%token <strval>  T_ETHERMASK
%token <strval>  T_LABEL_OID
%token <strval>  T_SUBMGT_FILTERS
%token <strval>  T_IP
%token <strval>  T_IP6
%token <strval>  T_MAC
%token <strval>  T_MAIN
%token <strval>  T_STRING
%token <strval>  T_HEX_STRING
%token <strval>  T_TIMETICKS
%token <strval>  T_IP_LIST
%token <strval>  T_IP6_LIST
%token <strval>  T_IP6_PREFIX_LIST

%token <uintval>  T_ASNTYPE_INT
%token <uintval>  T_ASNTYPE_UINT
%token <uintval>  T_ASNTYPE_SHORT
%token <uintval>  T_ASNTYPE_CHAR
%token <uintval>  T_ASNTYPE_GAUGE
%token <uintval>  T_ASNTYPE_COUNTER
%token <uintval>  T_ASNTYPE_TIMETICKS
%token <uintval>  T_ASNTYPE_IP
%token <uintval>  T_ASNTYPE_OBJID
%token <uintval>  T_ASNTYPE_STRING
%token <uintval>  T_ASNTYPE_HEXSTR
%token <uintval>  T_ASNTYPE_DECSTR
%token <uintval>  T_ASNTYPE_BITSTR
%token <uintval>  T_ASNTYPE_BIGINT
%token <uintval>  T_ASNTYPE_UBIGINT
%token <uintval>  T_ASNTYPE_FLOAT
%token <uintval>  T_ASNTYPE_DOUBLE
%token <uintval>  T_TLV_CODE
%token <uintval>  T_TLV_LENGTH
%token <uintval>  T_TLV_VALUE
%token <uintval>  T_TLV_STR_VALUE
%token <uintval>  T_TLV_STRZERO_VALUE
%token <uintval>  T_TLV_TYPE
%token <uintval>  T_IP_IP6_PORT

%type <tlvptr>  assignment_stmt
%type <tlvptr>  generic_stmt
%type <tlvptr>  assignment_list
%type <tlvptr>  generic_assignment_list
%type <tlvptr>  subsettings_stmt

%%

/*

How does this work ?

When we recognize an assignment statement (for example "MaxCPE 13;") we create
a "struct tlv" which contains the type associated with this configuration
setting, the length of this configuration setting and the value. The type and
lenth are in docsis_symtable.h. Consecutive assignment_stmts are reduced
to an assignment_list, creating a "tlvlist" which is essentially a list of
pointers to stuct tlv's.

"Special" cases like ClassOfService (configuration settings which have
sub-types) are treated a bit different.

For example, within the ClassOfService statement we have a list of
assignment_stmts which are reduced to an assignment_list, and then to a
subsettings_stmt. When we flatten and reduce the subsettings_stmt to an assignment_list we
merge the subsettings_stmt tlvlist with the tlvlist corresponding to the
assignment_list (may be empty if a subsettings_stmt appears first in
the config file) so we have a single tlvlist.

The idea is that, when we finish parsing, we end up with a "global_tlvlist"
which contains pointers to all the top-level struct tlv's that we assembled while
parsing the configuration file. At this point, we "flatten" this list into a
tlvbuf, add CM MIC and CMTS MIC and pad and that's it.

*/

/* Definitions of bison/yacc grammar */

main_stmt: 	T_MAIN '{' assignment_list '}' {
			_my_tlvtree_head = $3;
			_my_tlvtree_head->parent = NULL; }
		;

assignment_list: assignment_list assignment_stmt { $$ = add_tlv_sibling ($1,$2); }
		| assignment_stmt {  $$=add_tlv_sibling(NULL,$1); }
		| assignment_list subsettings_stmt {
			$$ = merge_tlvlist($1,$2);  }
		| subsettings_stmt {
			$$ = merge_tlvlist (NULL, $1); }
		;

generic_assignment_list: generic_assignment_list generic_stmt { $$ = add_tlv_sibling ($1,$2); }
			 | generic_stmt { $$=add_tlv_sibling(NULL,$1); }
			 ;


subsettings_stmt:  	T_IDENTIFIER '{' assignment_list  '}'	{
			$$ = assemble_tlv_in_parent ( $1->docsis_code, $3 ); }
		| T_IDENT_GENERIC T_TLV_CODE T_INTEGER '{' generic_assignment_list '}' {
			$$ = assemble_tlv_in_parent ( $3, $5 ); }
		;

assignment_stmt:  T_IDENTIFIER T_INTEGER ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_STRING ';'  {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_HEX_STRING ';'  {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_SUBMGT_FILTERS ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_IP ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_IP_LIST ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_IP6 ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_IP6_LIST ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_IP6_PREFIX_LIST ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_IP_IP6_PORT ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_MAC ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_ETHERMASK ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENTIFIER T_LABEL_OID ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}
		| T_IDENT_CVC T_STRING ';'  {
			$$ = create_external_file_tlv ($1, (union t_val *)&$2);}
		| T_IDENT_SNMPW T_LABEL_OID T_INTEGER ';' {
			$$ = create_snmpw_tlv ( $1, $2, (union t_val *) &$3 ); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_INT T_INTEGER ';' {
			$$ = create_snmpset_tlv($1,$2,'i',(union t_val *)&$4); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_GAUGE T_INTEGER ';' {
			$$ = create_snmpset_tlv($1,$2,'g',(union t_val *)&$4); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_UINT T_INTEGER ';' {
			$$ = create_snmpset_tlv($1,$2,'u',(union t_val *)&$4); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_COUNTER T_INTEGER ';' {
			$$ = create_snmpset_tlv($1,$2,'c',(union t_val *)&$4); }
	        | T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_IP T_IP ';' {
			$$ = create_snmpset_tlv($1,$2,'a',(union t_val *)&$4); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_STRING T_STRING ';' {
			$$ = create_snmpset_tlv($1,$2,'s',(union t_val *)&$4); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_HEXSTR T_HEX_STRING ';' {
			$$ = create_snmpset_tlv($1,$2,'x',(union t_val *)&$4); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_OBJID T_LABEL_OID ';' {
			$$ = create_snmpset_tlv($1,$2,'o',(union t_val *)&$4); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_TIMETICKS T_INTEGER ';' {
			$$ = create_snmpset_tlv($1,$2,'t',(union t_val *)&$4); }
		| T_IDENT_GENERIC T_TLV_CODE T_INTEGER T_TLV_STR_VALUE T_STRING ';' {
			$$ = create_generic_str_tlv($1,$3, (union t_val *)&$5); }
		| T_IDENT_GENERIC T_TLV_CODE T_INTEGER T_TLV_STRZERO_VALUE T_STRING ';' {
			$$ = create_generic_strzero_tlv($1,$3, (union t_val *)&$5); }
		| generic_stmt {
			$$ = $1; }
		;

generic_stmt:	T_IDENT_GENERIC T_TLV_CODE T_INTEGER T_TLV_LENGTH T_INTEGER T_TLV_VALUE T_HEX_STRING ';' {
			$$ = create_generic_tlv($1,$3,$5, (union t_val *)&$7); }
		| T_IDENT_GENERIC T_TLV_CODE T_INTEGER T_TLV_TYPE T_ASNTYPE_INT T_TLV_VALUE T_INTEGER ';' {
			$$ = create_generic_typed_tlv($1,$3,encode_uint, (union t_val *)&$7); }
		| T_IDENT_GENERIC T_TLV_CODE T_INTEGER T_TLV_TYPE T_ASNTYPE_SHORT T_TLV_VALUE T_INTEGER ';' {
			$$ = create_generic_typed_tlv($1,$3,encode_ushort, (union t_val *)&$7); }
		| T_IDENT_GENERIC T_TLV_CODE T_INTEGER T_TLV_TYPE T_ASNTYPE_CHAR T_TLV_VALUE T_INTEGER ';' {
			$$ = create_generic_typed_tlv($1,$3,encode_uchar, (union t_val *)&$7); }
		| T_IDENT_GENERIC T_TLV_CODE T_INTEGER T_TLV_TYPE T_ASNTYPE_IP T_TLV_VALUE T_IP ';' {
			$$ = create_generic_typed_tlv($1,$3,encode_ip, (union t_val *)&$7); }
                ;
%%

int yyerror(char *s) {
	fprintf(stderr, "%d:%s token %s\n",line,s,yytext );
	return 0;
}

#define TLVINIT(p) if(p) p->first_child=NULL; p->next_sibling=NULL; p->parent=NULL

/*
 * Given a symbol identifier, AND a buffer containing the raw "value",
 * returns a pointer to a tlv structure containing TLV buffer and the
 * length of the buffer, including docsis code and value length fields.
 * The length is relevant when we "assemble" two tlv_lists from an
 * an assignment_list + assignment_stmt
 */

struct tlv *
create_tlv(struct symbol_entry *sym_ptr, union t_val *value)
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv) ) ;
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = sym_ptr->encode_func(tlvbuf->tlv_value,value,sym_ptr);
/*		if (tlvbuf->tlv_len <= 0 ) {
			fprintf(stderr, "Got 0-length value while scanning for %s at line %d\n",sym_ptr->sym_ident,line );
			exit (-1);
   		} */
  return tlvbuf;
}

/* Given a symbol identifier ( always SnmpMibObject ), a string
** containing an OID name, a string which sets the value of the OID and a t_val
** which contains the raw value (of type oid_asntype), creates a TLV with the
** corresponding variable binding.
*/

struct tlv *
create_snmpset_tlv ( struct symbol_entry *sym_ptr,
				char *oid_string,
				char oid_asntype,
				union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = encode_vbind (oid_string, oid_asntype, value,
			            tlvbuf->tlv_value,TLV_VSIZE );

		if (tlvbuf->tlv_len <= 0 ) {
			fprintf(stderr, "got len 0 value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,line );
			exit (-1);
   		}
  free(oid_string);
/* We only free non-string values since we parse strings into a static buffer in docsis_lex.l */
  if (	oid_asntype == 'x' || oid_asntype == 'a' ||
	oid_asntype == 'd' || oid_asntype == 'o' )  {
	 	free(value->strval);
  }
  return tlvbuf;
}


/* Given a symbol identifier ( always SnmpWrite ), a string
** containing an OID, and an integer, creates a TLV containing the
** OID ASN encoding + 1 control byte to permit/deny SNMP Write operations on a
** MIB variable which has the OID as prefix.
*/

struct tlv *
create_snmpw_tlv ( struct symbol_entry *sym_ptr,
                               char *oid_string,
                               union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = encode_snmp_oid ( oid_string, tlvbuf->tlv_value, TLV_VSIZE );

                if (tlvbuf->tlv_len <= 0 ) {
                        fprintf(stderr, "got len 0 value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,line );
                        exit (-1);
                }
  tlvbuf->tlv_value[tlvbuf->tlv_len] = (unsigned char) value->intval ;
  tlvbuf->tlv_len++;
  free(oid_string);
  return tlvbuf;
}

/* Given a code, length and raw value (encoded as a hex string),
** creates a TLV encoding. This can be used for e.g. VendorSpecific
** Information or unsupported (future?) configuration settings.
*/
struct tlv *
create_generic_tlv ( struct symbol_entry *sym_ptr,
				 int tlv_code,
                                 int tlv_length,
                                 union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = tlv_code;
  tlvbuf->tlv_len = encode_hexstr ( tlvbuf->tlv_value, value, sym_ptr );

                if (tlvbuf->tlv_len <= 0 ) {
                        fprintf(stderr, "got len 0 value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,
line );
                        exit (-1);
                }
                if (tlvbuf->tlv_len != tlv_length ) {
                        fprintf(stderr, "Length mismatch while encoding GenericTLV: given length %d, value length %d at line %d\n", tlvbuf->tlv_len, tlv_length, line );

                        exit (-1);
		}
  return tlvbuf;
}

/*
** Same as above, except the value is encoded as a string.
*/

struct tlv *
create_generic_str_tlv ( struct symbol_entry *sym_ptr,
                                 int tlv_code,
                                 union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = tlv_code;
  tlvbuf->tlv_len = encode_string ( tlvbuf->tlv_value, value, sym_ptr );

                if (tlvbuf->tlv_len <= 0 ) {
                        fprintf(stderr, "got len 0 value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,
line );
                        exit (-1);
                }
                if (tlvbuf->tlv_len > 255 ) {
                        fprintf(stderr, "Warning: string length %d longer than 255, line %d\n",tlvbuf->tlv_len, line );

                }
  /* don't free strings - we use a static buffer to parse them */
  return tlvbuf;
}

/*
** Same as above, except the value is encoded as a zero-terminated string.
*/

struct tlv *
create_generic_strzero_tlv ( struct symbol_entry *sym_ptr,
                                 int tlv_code,
                                 union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  TLVINIT(tlvbuf);
  tlvbuf->docs_code = tlv_code;
  tlvbuf->tlv_len = encode_string ( tlvbuf->tlv_value, value, sym_ptr );

                if (tlvbuf->tlv_len <= 0 ) {
                        fprintf(stderr, "got 0-length value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,
line );
                        exit (-1);
                }
                if (tlvbuf->tlv_len > 254 ) {
                        fprintf(stderr, "Warning: total string length %d longer than 255, line %d\n",tlvbuf->tlv_len, line );

                } else {
			tlvbuf->tlv_len = tlvbuf->tlv_len + 1; /* add terminating 0 */
		}
  tlvbuf->tlv_value[tlvbuf->tlv_len]=0;
  return tlvbuf;
}

/* Given symbol name and a strings that represents a filename
** creates a TLV encoding from the raw data in the file.
** If the file is longer than 255, it is split into "parts" of
** 255 octets max.
*/
struct tlv *
create_external_file_tlv ( struct symbol_entry *sym_ptr,
                                 union t_val *value )
{
  struct tlv *new_tlvbuf=NULL, *first_tlvbuf = NULL, *old_tlvbuf = NULL;
  size_t read_len;
  unsigned char read_buffer[255];
  FILE *ext_file;

  if ((ext_file = fopen (value->strval, "rb")) == NULL) {
	fprintf(stderr, "Error: can't open external file %s at line %d\n", value->strval, line);
	exit (-5);
  }

  while ( !feof(ext_file) ) {
 	if (! (read_len = fread (read_buffer, 1, 254, ext_file)) ) {
	  fprintf (stderr, "Error reading data from %s\n", value->strval) ;
  	  fclose (ext_file);
	  exit(-5);
  	}

	new_tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  	TLVINIT(new_tlvbuf);
	if (first_tlvbuf == NULL) first_tlvbuf = new_tlvbuf;

  	new_tlvbuf->docs_code = sym_ptr->docsis_code;
  	new_tlvbuf->tlv_len = read_len;

	memcpy (new_tlvbuf->tlv_value, read_buffer, read_len);
	if(old_tlvbuf) old_tlvbuf->next_sibling = new_tlvbuf;
	old_tlvbuf = new_tlvbuf;
  }
  fclose (ext_file);
  return first_tlvbuf;
}

/* Given a code, type, and value, creates a TLV encoding.
** Expected to be used for VendorSpecific settings
*/
struct tlv *
create_generic_typed_tlv ( struct symbol_entry *sym_ptr,
				int tlv_code,
			int (*encode_func) (unsigned char *, void *, struct symbol_entry *),
				union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  tlvbuf->docs_code = tlv_code;
  tlvbuf->tlv_len = encode_func ( tlvbuf->tlv_value, value, sym_ptr );

  if (tlvbuf->tlv_len <= 0 ) {
    fprintf (stderr, "got len 0 value while scanning for %s\n at line %d\n",sym_ptr->sym_ident,line );
    exit (-1);
  }
  return tlvbuf;
}

/*
 * Adds a TLV to the current tlv "list". If the tlv pointer we are called with is NULL,
 * we assume this is the "first" tlv and just return the initialized newtlv.
 */

struct tlv *add_tlv_sibling (struct tlv *tlv, struct tlv *newtlv)
{
  struct tlv *tlvptr, *last_sibling;

  if (newtlv == NULL ) {
	fprintf(stderr, "Error: add_tlv_sibling called with NULL tlv sibling ! \n " ) ;
	exit(-23);
  }

  if (tlv != NULL ) {
/*  	tlv->parent=NULL;  */
	/* find "last" sibling in this assignment list */

	for (tlvptr=tlv; tlvptr; tlvptr=tlvptr->next_sibling)
		last_sibling=tlvptr;
	/* Add the new TLV at the end of the list */
	last_sibling->next_sibling = newtlv;
	return tlv;
  } else  {
	return newtlv;
  }
}

/* Merge two parse "subtrees" into one  */

struct tlv *
merge_tlvlist(struct tlv *tlv1, struct tlv *tlv2)
{
  struct tlv *tlvptr, *last_sibling;
  if ( tlv2 == NULL ) {
	fprintf(stderr, "merge_tlvlist called with NULL tlv2\n");
	exit(-2);
  }

  if ( tlv1 == NULL ) {
	/* subsettings_stmt can be empty; in that case its value is NULL */
	return tlv2;
  }
  /* find "last" sibling in this assignment list */

  for (tlvptr=tlv1; tlvptr; tlvptr=tlvptr->next_sibling)
	last_sibling=tlvptr;
  last_sibling->next_sibling = tlv2;
  return tlv1;

}

/*
 * Creates the "parent" tlv that holds the "subtree" (e.g. the parent TLV's first_child will point to the
 * first element of this subtree.
 */

struct tlv *
assemble_tlv_in_parent (int tlvcode, struct tlv *child_tlv)
{
  struct tlv *parent_tlv;

  parent_tlv = (struct tlv *) malloc ( sizeof ( struct tlv) );
  memset(parent_tlv, 0, sizeof(struct tlv));

  parent_tlv->docs_code = tlvcode;
  parent_tlv->tlv_len = 0;
  parent_tlv->first_child = child_tlv;
  child_tlv->parent = parent_tlv;
  /* TODO: make al children's "parent" point back to parent_tlv */
  return parent_tlv;
}

/*
 * Creates a buffer filled with the TLV bytes sequentially, as they should be
 * found in the final configuration file.
 * The function walkts the tree recursively in "depth-last" mode and aggregates
 * the parsed values into an output "binary".
 */

unsigned int flatten_tlvsubtree ( unsigned char *buf, unsigned int used_size, struct tlv *tlv)
{
  struct tlv *tlvptr;
  unsigned int rsize;
  unsigned short netshort;
  register unsigned char *cp;

  if ( buf == NULL ) {
	fprintf(stderr, "Error: can't flatten tlvlist in a NULL destination buffer\n" );
	exit (-2);
  }

  cp = buf + used_size;

  for (tlvptr=tlv; tlvptr; tlvptr=tlvptr->next_sibling ) {
	if (tlvptr->first_child )  { /* Sub-Settings */
		/* we don't know the size yet, so we delay writing type & length */
		rsize = flatten_tlvsubtree(buf, (cp-buf)+2, tlvptr->first_child);
		if (rsize > 255) {
			fprintf(stderr, "Warning: at line %d: aggregate size of settings block larger than 255, skipping\n", line);
			continue;
		}
		*cp = (unsigned char) tlvptr->docs_code; cp++;
		*cp = (unsigned char) rsize; cp++;
		cp = cp + rsize;
  	} else if (tlvptr->tlv_len <= 255 ) {

			*cp = (unsigned char) tlvptr->docs_code; cp++;
			*cp = (unsigned char) tlvptr->tlv_len; cp++;
			if (tlvptr->tlv_len > 0)
				memcpy ( cp, tlvptr->tlv_value, tlvptr->tlv_len );
  	     		cp = cp + tlvptr->tlv_len;
		} else {
			/* convert TLV11 to TLV64 */
			if (tlvptr->docs_code == 11) {
				*cp = 64; cp++;
			        netshort = htons(tlvptr->tlv_len);
				memcpy ( cp, &netshort, sizeof(unsigned short));
				cp=cp+sizeof(unsigned short);
       				memcpy ( cp, tlvptr->tlv_value, tlvptr->tlv_len );
             			cp = cp + tlvptr->tlv_len;
			} else {
				fprintf(stderr, "Warning at line %d: Non-SnmpMibObject TLV larger than 255... skipping.\n", line);
				continue;
			}
		}
	}
  return (unsigned int) (cp - buf - used_size); /* return the number of bytes encoded this time  */
}

/*
 *  Recursively walk the tree and calculate the total size of the "concatenated" TLVs
 */
unsigned int tlvtreelen (struct tlv *tlv)
{
   struct tlv *tlvptr;
   unsigned int current_size=0;

   if (tlv == NULL) return 0;

   for (tlvptr=tlv; tlvptr; tlvptr=tlvptr->next_sibling)  {
        if (tlvptr->first_child)
            current_size += 2+tlvtreelen(tlvptr->first_child);
        else
            current_size += 2+tlvptr->tlv_len;
   }
   return current_size;
}

int parse_config_file ( char *file, struct tlv **parse_tree_result )
{
  FILE *cf;
  int rval;

  if ( !strcmp(file, "-") )
  {
	cf = stdin;
  }
  else if ( (cf = fopen ( file, "r" )) == NULL )
  {
	fprintf (stderr, "docsis: Can't open input file %s\n", file );
	return -1;
  }

  yyin = cf ;
#ifdef DEBUG
  yydebug = 1;
#endif
  rval = yyparse();
  if (!rval) *parse_tree_result = _my_tlvtree_head;
  fclose(cf);
  return rval;
}
