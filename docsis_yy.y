/* 
 *  DOCSIS configuration file encoder. 
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002 Evvolve Media SRL,office@evvolve.com
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  DOCSIS is a registered trademark of Cablelabs, http://www.cablelabs.com
 */

%{
#include "docsis.h"

extern unsigned int line; 	/* current line number, defined in a.l */
extern struct tlv_list *global_tlvlist; /* Global list of all config TLVs */
extern symbol_type *global_symtable;
 
%}                  

%union { 	/* Token types */
	int intval;		/* For integers */
	unsigned int uintval;		/* For integers */
	symbol_type *symptr; 		/* For token identifiers */
	char *strval;		 	/* For strings */
	unsigned char *ustrval; 	/* For other any other types */
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
%token <strval>  T_MAC
%token <strval>  T_ETHERMASK
%token <strval>  T_LABEL_OID
%token <strval>  T_IP
%token <strval>  T_MAC
%token <strval>  T_MAIN
%token <strval>  T_STRING
%token <strval>  T_HEX_STRING

%token <uintval>  T_ASNTYPE_INT
%token <uintval>  T_ASNTYPE_UINT
%token <uintval>  T_ASNTYPE_TICKS 
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


%type <tlvptr>  assignment_stmt
%type <tlvlist> assignment_list
%type <tlvlist> config_stmt
/* %type <tlvlist> config_stmt_list */
/* %type <tlvlist> config_stmt */

%%
/* 

How this works ? 

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
cos_stmt. When we flatten/reduce the cos_stmt to a config_stmt_list we merge the 
cos_stmt tlvlist with the tlvlist corresponding to config_stmt_list (may be
empty if cos_stmt is the first statement that appears in the config file) so we
have a single tlvlist. 

The idea is that, when we finish parsing, we end up with a "global_tlvlist" 
which contains pointers to all the struct tlv's that we assembled while parsing
the configuration file. At this point, we "flatten" this list into a tlvbuf, 
add CM MIC and CMTS MIC and pad and that's it. */


/* Definitions of bison/yacc grammar */

main_stmt: 	T_MAIN '{' assignment_list '}' {
			global_tlvlist = $3; }
		;

assignment_list: assignment_list assignment_stmt { $$ = add_tlv_to_list ($1,$2); } 
		| assignment_stmt {  $$=add_tlv_to_list(NULL,$1); } 
		| assignment_list config_stmt { 
			$$ = merge_tlvlist($1,$2);  }
		| config_stmt { 
			$$ = merge_tlvlist (NULL, $1); }
		; 

config_stmt:  	T_IDENTIFIER '{' assignment_list  '}'	{ 
			$$ = assemble_list_in_parent ( $1, $3 ); }
		;

assignment_stmt:  T_IDENTIFIER T_INTEGER ';' { 
			$$ = create_tlv ($1, (union t_val *)&$2);} 
		| T_IDENTIFIER T_STRING ';'  {
			$$ = create_tlv ($1, (union t_val *)&$2);}	
		| T_IDENTIFIER T_HEX_STRING ';'  {
			$$ = create_tlv ($1, (union t_val *)&$2);}	
		| T_IDENTIFIER T_IP ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}	
		| T_IDENTIFIER T_MAC ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}	
		| T_IDENTIFIER T_ETHERMASK ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}	
		| T_IDENTIFIER T_LABEL_OID ';' {
			$$ = create_tlv ($1, (union t_val *)&$2);}	
		| T_IDENT_SNMPW T_LABEL_OID T_INTEGER ';' {    
			$$ = create_snmpw_tlv ( $1, $2, (union t_val *) &$3 ); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_INT T_INTEGER ';' {
			$$ = create_snmpset_tlv($1,$2,'i',(union t_val *)&$4); }
	        | T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_IP T_IP ';' {
			$$ = create_snmpset_tlv($1,$2,'a',(union t_val *)&$4); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_STRING T_STRING ';' {
			$$ = create_snmpset_tlv($1,$2,'s',(union t_val *)&$4); }
		| T_IDENT_SNMPSET T_LABEL_OID T_ASNTYPE_HEXSTR T_HEX_STRING ';' {
			$$ = create_snmpset_tlv($1,$2,'x',(union t_val *)&$4); }
		| T_IDENT_GENERIC T_TLV_CODE T_INTEGER T_TLV_LENGTH T_INTEGER T_TLV_VALUE T_HEX_STRING ';' {
			$$ = create_generic_tlv($1,$3,$5, (union t_val *)&$7); }
                ;                                        
%%

int yyerror(char *s) {
	fprintf(stderr, "%d:%s token %s\n",line,s, yytext );
	return 0;
}

/* 
 * Given a symbol identifier, AND a buffer containing the raw "value", 
 * returns a pointer to a tlv structure containing TLV buffer and the 
 * length of the buffer, including docsis code and value length fields. 
 * The length is relevant when we "assemble" two tlv_lists from an  
 * an assignment_list + assignment_stmt
 */ 

struct tlv *create_tlv(struct symbol_entry *sym_ptr, union t_val *value) 
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv) ) ;	
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = sym_ptr->get_value_func(tlvbuf->tlv_value,value,sym_ptr);
		if (tlvbuf->tlv_len <= 0 ) { 
			printf ("Got 0-length value while scanning for %s at line %d\n",sym_ptr->sym_ident,line );
			exit (-1);
   		}
  return tlvbuf;
}

/* Given a symbol identifier ( well, always SnmpMibObject ), a string 
** containing an OID name, a string which sets the value of the OID and a t_val 
** which contains the raw value (of type oid_asntype), creates a TLV with the 
** corresponding VariableBinding  
*/

struct tlv *create_snmpset_tlv ( struct symbol_entry *sym_ptr,
				char *oid_string,
				char oid_asntype, 
				union t_val *value ) 
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));	
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = encode_vbind (oid_string, oid_asntype, value,
			            tlvbuf->tlv_value,TLV_VSIZE );
				    
		if (tlvbuf->tlv_len <= 0 ) { 
			printf ("got len 0 value while scanning for %s\n at line %d",sym_ptr->sym_ident,line );
			exit (-1);
   		}
  return tlvbuf;
}


/* Given a symbol identifier ( always SnmpWrite ), a string
** containing an OID, and an integer, creates a TLV containing the  
** OID ASN encoding + 1 control byte to permit/deny SNMP Write operations on a
** MIB variable which has the OID as prefix.
*/

struct tlv *create_snmpw_tlv ( struct symbol_entry *sym_ptr,
                               char *oid_string,
                               union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  tlvbuf->docs_code = sym_ptr->docsis_code;
  tlvbuf->tlv_len = encode_oid ( oid_string, tlvbuf->tlv_value, TLV_VSIZE );
                                    
                if (tlvbuf->tlv_len <= 0 ) {
                        printf ("got len 0 value while scanning for %s\n at line %d",sym_ptr->sym_ident,line );
                        exit (-1);
                }
  tlvbuf->tlv_value[tlvbuf->tlv_len] = (unsigned char) value->intval ;
  tlvbuf->tlv_len++;
  return tlvbuf;
}         

/* Given a code, length and raw value (encoded as a hex string), 
** creates a TLV encoding. This can be used for e.g. VendorSpecific 
** Information or unsupported (future?) configuration settings. 
*/
struct tlv *create_generic_tlv ( struct symbol_entry *sym_ptr,
				 int tlv_code,
                                 int tlv_length,
                                 union t_val *value )
{
  struct tlv *tlvbuf=NULL;

  tlvbuf = (struct tlv *) malloc (sizeof(struct tlv));
  tlvbuf->docs_code = tlv_code;
  tlvbuf->tlv_len = get_hexstr ( tlvbuf->tlv_value, value, sym_ptr );

                if (tlvbuf->tlv_len <= 0 ) {
                        printf ("got len 0 value while scanning for %s\n at line %d",sym_ptr->sym_ident,
line );
                        exit (-1);
                }
                if (tlvbuf->tlv_len != tlv_length ) {
                        printf ("Length mismatch while encoding GenericTLV: given length %d, value value length %d at line %d",tlvbuf->tlv_len, tlv_length, line );

                        exit (-1);
		}
  return tlvbuf;
}

/* Adds a TLV to a tlvlist. If the tlvlist pointer we are called with is NULL, 
 * we create the new tlvlist. 
 */ 

struct tlv_list *add_tlv_to_list(struct tlv_list *list, struct tlv *newtlv) 
{
  struct tlv_list *newtlvlist;
  newtlvlist = (struct tlv_list *) malloc ( sizeof(struct tlv_list) );
  if ( list == NULL ) {  /* assignment_list :== assignment_stmt  */
/* create list of tlv pointers with 1 tlv pointer */
	newtlvlist->tlv_count = 1;	
	newtlvlist->tlvlist   = (struct tlv **) malloc ( sizeof (struct tlv *));
  	newtlvlist->tlvlist[0] = newtlv;
	return newtlvlist;
  } else { /* assignment_list :== assignment_list assignment_stmt */
/* add a new tlv pointer to the existing list of tlv pointers */
	newtlvlist->tlv_count = list->tlv_count + 1;
	newtlvlist->tlvlist = (struct tlv **) malloc((newtlvlist->tlv_count)*sizeof(struct tlv *));
	memcpy(&(newtlvlist->tlvlist[0]),&(list->tlvlist[0]),list->tlv_count*sizeof(struct tlv *));
	newtlvlist->tlvlist[newtlvlist->tlv_count-1]=newtlv;
	free (list->tlvlist); free(list);
  	return newtlvlist;
  }
}

/* Merges two tlvlists into one tlvlist. */

struct tlv_list *merge_tlvlist( struct tlv_list *list1, struct tlv_list *list2 )
{
  struct tlv_list *newtlvlist;
  newtlvlist = (struct tlv_list *) malloc ( sizeof(struct tlv_list) );
  if ( list2 == NULL ) {
	printf ("merge_tlvlist called with NULL list2 !\n"); 
	exit (-2);
  }
  
  if ( list1 == NULL ) {
	/* config_stmt_list can be empty; in that case its value is NULL */
	return list2;
  } 
  newtlvlist->tlv_count = list1->tlv_count+list2->tlv_count;
  newtlvlist->tlvlist = (struct tlv **) malloc ((newtlvlist->tlv_count)*sizeof ( struct tlv *));
  memcpy ( &(newtlvlist->tlvlist[0]),&(list1->tlvlist[0]),list1->tlv_count*sizeof(struct tlv *));  
  memcpy ( &(newtlvlist->tlvlist[list1->tlv_count]),&(list2->tlvlist[0]),list2->tlv_count*sizeof(struct tlv *));
  free(list1->tlvlist);free(list1);
  free(list2->tlvlist);free(list2);
/* We don't free the TLVs since they are still referenced by newtlvlist->tlvlist */
  return newtlvlist;
}

/* 
 * Used for creating a top-level list corresponding to ClassOfService,
 * BaselinePrivacy, Telephony Settings etc.
 * Creates a "flat" TLV buffer out of a tlvlist, and assembles the 
 * TLV buffer into a new TLV which has the parent's docsis_code. 
 * Returns a TLV list with the new TLV 
 */

struct tlv_list *assemble_list_in_parent (struct symbol_entry *sym_ptr, struct tlv_list *list) 
{
  struct tlv_list *newtlvlist;
  struct tlv *parent_tlv;

  newtlvlist = (struct tlv_list *) malloc ( sizeof ( struct tlv_list ));
  newtlvlist->tlvlist = (struct tlv **) malloc ( sizeof ( struct tlv *));
  parent_tlv = (struct tlv *) malloc ( sizeof ( struct tlv) );
  
  parent_tlv->docs_code = sym_ptr->docsis_code;
  parent_tlv->tlv_len = flatten_tlvlist(parent_tlv->tlv_value,list);
  newtlvlist->tlv_count=1;
  newtlvlist->tlvlist[0]=parent_tlv;
  return newtlvlist;
}

/* Creates a buffer filled with the TLV bytes sequentially, as they will be 
 * found in the final configuration file. 
 * We use this function either to create a "parent" tlv corresponding to 
 * ClassOfService-like TLVs, or to create the final buffer to which we add the 
 * MIC digests, end-of-data marker and pad and write to the output file.
 */

unsigned int flatten_tlvlist (unsigned char *buf, struct tlv_list *list )
{
  register unsigned char *cp;
  int i;

  if ( buf == NULL ) { 
	printf( "Error: can't flatten tlvlist in a NULL destination  buffer!\n" );
	exit (-2);
  }
  cp = buf;

  for ( i=0; i< list->tlv_count; i++ ) { 
	*cp = (unsigned char) list->tlvlist[i]->docs_code; cp++;
	*cp = (unsigned char) list->tlvlist[i]->tlv_len; cp++;
	memcpy ( cp, list->tlvlist[i]->tlv_value, list->tlvlist[i]->tlv_len ); 
  	     cp = cp + list->tlvlist[i]->tlv_len;
	free ( list->tlvlist[i] ); /* The TLV won't be referenced anymore */
  }
  /* We leave it to the caller to free the list */
  return (unsigned int) (cp-buf); 
}

int parse_input_file ( FILE *thefile ) 
{ 
  yyin = thefile ; 
#ifdef DEBUG
  yydebug = 1;
#endif
  return yyparse();
}
