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
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  DOCSIS is a registered trademark of Cablelabs, http://www.cablelabs.com
 */

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <math.h>
#include <ctype.h>

/* #include "docsis.h" */
#include "docsis_decode.h"
#include "docsis_globals.h"
#include "docsis_snmp.h"
#include "ethermac.h"


struct symbol_entry *
find_symbol_by_code_and_pid (unsigned char code, unsigned int pid)
{
 int i;

 for ( i=0; i<NUM_IDENTIFIERS; i++) {
        if (global_symtable[i].docsis_code == code && global_symtable[i].parent_id == pid) {
                return &global_symtable[i];
        }
 }
 return NULL;
}

void decode_uint (unsigned char *tlvbuf, struct symbol_entry *sym, size_t length)
{
  static unsigned int helper;
  if (length != sizeof(unsigned int) ) {
	fprintf(stderr, "u_int length mismatch\n");
	exit(-45);
  }
  memset( &helper, 0, sizeof(unsigned int));
  memcpy( &helper, tlvbuf, length);

  printf("%s %u;\n", sym->sym_ident, ntohl(helper));
}

void decode_ushort (unsigned char *tlvbuf, symbol_type *sym, size_t length)
{
  static unsigned short helper;
  if (length != sizeof(unsigned short) ) {
        fprintf(stderr, "u_short length mismatch\n");
        exit(-45);
  }

  memset( &helper, 0, length);
  memcpy( &helper, tlvbuf, length );

  printf("%s %hu;\n", sym->sym_ident, ntohs(helper));
}

void decode_uchar (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  printf("%s %hhu;\n", sym->sym_ident, (unsigned char) *tlvbuf );
}

void decode_ip (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  static struct in_addr helper;
  if (length != sizeof(struct in_addr) ) {
        fprintf(stderr, "ip address length mismatch\n");
        exit(-45);
  }

  memcpy (&helper, tlvbuf, length );
  printf("%s %s;\n",
	sym->sym_ident, inet_ntoa(helper) );
}

void decode_ip6 (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  static struct in6_addr helper;
  char ipstr[INET6_ADDRSTRLEN];
  if (length != sizeof(struct in6_addr) ) {
        fprintf(stderr, "ip address length mismatch\n");
        exit(-45);
  }

  memcpy (&helper, tlvbuf, length );
  fprintf (stdout, "%s %s;\n",
	sym->sym_ident, inet_ntop(AF_INET6,tlvbuf,ipstr,sizeof ipstr) );
}

void decode_ether (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{

 if (length != 6 ) {
        fprintf(stderr, "ethermac length mismatch\n");
        exit(-45);
 }
 printf("%s %s;\n",
	sym->sym_ident, ether_ntoa(tlvbuf) );
}

void decode_ethermask (unsigned char *tlvbuf, symbol_type *sym, size_t length)
{
/* the return value of ether_ntoa is a pointer to a static string
 * in the ether_ntoa function, so we have to print the values in two
 * "passees" to avoid the 2nd call overwriting the 1st.
 */
  if (length != 12 ) {
        fprintf(stderr, "ethermac_and_mask length mismatch\n");
        exit(-45);
  }
  printf("%s %s/", sym->sym_ident, ether_ntoa(tlvbuf));

  printf("%s;\n", ether_ntoa(tlvbuf+6));
}

void decode_md5 (unsigned char *tlvbuf, symbol_type *sym, size_t length)
{
  size_t j=0;
  if (length != 16 ) {
        fprintf(stderr, "md5digest length mismatch\n");
        exit(-45);
  }

  printf("/* %s ", sym->sym_ident);
  for (j=0;j<length;j++)	printf("%02x", tlvbuf[j]);

  printf("; */\n");
}

void decode_snmp_wd (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  fprintf(stderr, "%s ", sym->sym_ident);

  /* last char in this TLV is not part of OID */
  decode_snmp_oid (tlvbuf, (unsigned int) length-1 );

  printf(" %d ;\n", (unsigned int) tlvbuf[length-1] );
}

void decode_oid (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  printf("%s ", sym->sym_ident);
  decode_snmp_oid (tlvbuf,(unsigned int) length );
  printf(";\n");
}

void decode_snmp_object (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  printf("%s ", sym->sym_ident);
  decode_vbind (tlvbuf, length );
/*  22-06-03  decode_vbind prints the trailing ';' as well  */
  printf("\n");
}

void decode_string (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
 static char helper[TLV_VSIZE];

/* helper = (char *) malloc ( ((unsigned int) tlvbuf[1])+1 );  */
 memset ( helper, 0, length+1);
 strncpy ( helper, (char *) tlvbuf, length );
 printf("%s \"%s\";\n", sym->sym_ident, helper );
}

void decode_strzero (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
 char *helper;

 helper = (char *) malloc ( length +1 );
 memset ( helper, 0, length+1);
 strncpy ( helper, (char *) tlvbuf, length );
 printf("%s \"%s\";\n", sym->sym_ident, helper );
 free(helper);
}


void decode_hexstr (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
 char *helper;
 unsigned int i;
 unsigned int len;

/* TODO */
 len =  length;
 helper = (char *) malloc ( len+1 );
 memset ( helper, 0, len+1);
 memcpy ( helper, (char *) tlvbuf, len );
 printf("%s 0x", sym->sym_ident);
 for(i=0; i<len; i++) {
	printf("%02x", (unsigned char) helper[i]);
 }
 printf(";\n");
 free(helper);
}

void decode_ushort_list (unsigned char *tlvbuf, symbol_type *sym, size_t length)
{
 char *helper;
 unsigned int i;
 unsigned int len;

 len = length ;
 helper = (char *) malloc ( len+1 );
 memset ( helper, 0, len+1);
 memcpy ( helper, (char *) tlvbuf, len );
 printf("%s ", sym->sym_ident);
 if ( len < 2*sym->low_limit || len > 2*sym->high_limit )
	printf("/* -- warning: illegal length of buffer --*/");

 for(i=0; i<len; i=i+2) {
        printf("%hu", ntohs( (* (unsigned short *) &helper[i])) );
	if (i< len-2) printf(",");
 }
 printf(";\n");
 free(helper);
}

void decode_unknown (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  size_t len=0;
  char *cp,*value;
  char hexvalue[514];

  len = length ;
  if (len > 256 ) {
	fprintf(stderr, "/* ** next TLV is truncated** */"); len = 256;
  }
  memset (hexvalue, 0, 514);

  value = (char *) malloc ( len+1 );
  memset ( value, 0, len+1);
  memcpy ( value, (char *) tlvbuf+2, len );

  cp = value;
  if ( str_isprint(cp, len) && len > 1 ) {

	printf("GenericTLV TlvCode %d TlvString ",
			(unsigned int) tlvbuf[0]) ;
	printf("\"%s\"; /* tlv length = %zd */", cp, len);
  } else if ( len > 1 && cp[len-1] == 0 && str_isprint(cp, len-1 ) ) {
	printf("GenericTLV TlvCode %d TlvStringZero ",
                        (unsigned int) tlvbuf[0] ) ;
	printf("\"%s\"; /* tlv length = %zd */", cp, len);
  } else {
	printf("GenericTLV TlvCode %d TlvLength %zd TlvValue ",
			(unsigned int) tlvbuf[0], len) ;
  	snprint_hexadecimal ( hexvalue, 514, value, len);

	printf("%s;", hexvalue);
  }
  printf("\n");
  free(value);
}

void decode_special (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  printf("%s\n", sym->sym_ident);
}

void decode_aggregate (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  register unsigned char *cp;
  symbol_type *current_symbol;
  unsigned int tlv_llen = 1; /* length of "Length" encoding of current TLV */
  size_t tlv_vlen; 	/* length of "Value" encoding of current TLV */

  /* cp = tlvbuf+1+tlv_llen; */ /* skip type,len of parent TLV */
  cp = tlvbuf;

  printf("%s\n", sym->sym_ident);
  __docsis_indent(INDENT_NOOP, TRUE);
  printf("{\n");

  __docsis_indent(INDENT_INCREMENT, FALSE);

/*  while ( (unsigned int) (cp - tlvbuf +(sizeof(unsigned char))) < (unsigned int) tlvbuf[1] ) { */
  while ( (unsigned int) (cp - tlvbuf) < (unsigned int) length ) {

  __docsis_indent(INDENT_NOOP, TRUE);
  current_symbol = find_symbol_by_code_and_pid (cp[0], sym->id);
  tlv_vlen = (size_t) cp[1];
  if (current_symbol == NULL) {
		decode_unknown(cp, NULL, tlv_vlen );
  	} else {
      		current_symbol->decode_func (cp+1+tlv_llen, current_symbol, tlv_vlen);
  	}
 	cp = (unsigned char*) cp + 1 + tlv_llen + tlv_vlen; /* skip type, length value */
  }

  __docsis_indent(INDENT_DECREMENT, FALSE);

  __docsis_indent(INDENT_NOOP, TRUE);
  printf("}\n");
}

/*
** This is just like decode_aggregate, but it only does symbol lookup
** for the VendorIdentifier which is the only 'known' TLV.
**/

void decode_vspecific (unsigned char *tlvbuf, symbol_type *sym, size_t length )
{
  register unsigned char *cp;
  symbol_type *first_symbol;
  unsigned int tlv_llen = 1; /* length of "Length" encoding of current TLV */
  size_t tlv_vlen; 	/* length of "Value" encoding of current TLV */

  /*  cp = tlvbuf+1+tlv_llen; */ /* skip type,len of parent TLV */
  cp = tlvbuf;

  printf("%s\n", sym->sym_ident);
  __docsis_indent(INDENT_NOOP, TRUE);
  printf("{\n");

  __docsis_indent(INDENT_INCREMENT, FALSE);

  /* First TLV inside VendorSpecific has to be VendorIdentifier... the rest we don't care */

  first_symbol = find_symbol_by_code_and_pid (cp[0], sym->id);
  tlv_vlen = (size_t) cp[1];
  if (first_symbol == NULL) {
  		__docsis_indent(INDENT_NOOP, TRUE);
		fprintf(stderr, "/* WARNING: Invalid VendorSpecific option - 1st element is NOT VendorIdentifier */\n");
  		__docsis_indent(INDENT_NOOP, TRUE);
                decode_unknown(cp, NULL, tlv_vlen);
        } else {
	/* Symbol found ... check if it's the right one */
		if (!strncmp (first_symbol->sym_ident, "VendorIdentifier", 16))
		{
  			__docsis_indent(INDENT_NOOP, TRUE);
	                first_symbol->decode_func(cp+1+tlv_llen, first_symbol, tlv_vlen);
		}
		else
		{
  			__docsis_indent(INDENT_NOOP, TRUE);
			fprintf(stderr, "/* Invalid VendorSpecific option - 1st element is NOT VendorIdentifier */");
  			__docsis_indent(INDENT_NOOP, TRUE);
			decode_unknown(cp, NULL, tlv_vlen );
		}
        }

  cp = (unsigned char*) cp + 1 + tlv_llen + tlv_vlen; /* skip type, length, value */

  while ( (unsigned int) (cp - tlvbuf) < (unsigned int) length ) {

  __docsis_indent(INDENT_NOOP, TRUE);

 	tlv_vlen = (size_t) cp[1];
        decode_unknown(cp, NULL, tlv_vlen );
        cp = (unsigned char*) cp + 1 + tlv_llen + tlv_vlen; /* skip type, length, value */
  }

  __docsis_indent(INDENT_DECREMENT, FALSE);

  __docsis_indent(INDENT_NOOP, TRUE);
  printf("}\n");
}

/*
 * This function is needed because we don't have a symbol to call it.
 * We can't put a "Main" symbol in the symtable because docsis_code is
 * unsigned char (in struct symbol_entry) and we reserve the values for
 * DOCSIS use.
 * It's also a bit different from docsis_aggregate in that docsis_aggregate
 * takes an aggregate tlvbuf as argument that INCLUDES the "parent" code and
 * length. On the main aggregate we don't have a code / length.
 */

void decode_main_aggregate (unsigned char *tlvbuf, size_t buflen)
{
#define TLV_LEN_SIZE 1
  register unsigned char *cp = NULL;
  symbol_type *current_symbol;
  unsigned int tlv_llen = 1; /* length of "Length" encoding of current TLV */
  size_t tlv_vlen; 	/* length of "Value" encoding of current TLV */

  cp = tlvbuf;

  __docsis_indent(INDENT_CLEAR, FALSE);

  printf("Main \n{\n");
  __docsis_indent(INDENT_INCREMENT, FALSE);

  while ( (unsigned int) (cp - tlvbuf) < buflen ) {

  __docsis_indent(INDENT_NOOP, TRUE);

  current_symbol = find_symbol_by_code_and_pid (cp[0],0);
  if (cp[0] == 64) {
	tlv_llen = 2;
	tlv_vlen = (size_t) ntohs(*((unsigned short *)(cp+1)));
  } else  {
	tlv_llen = 1;
	tlv_vlen = (size_t) cp[1];
  }
  if (current_symbol == NULL) {
		decode_unknown(cp, NULL, (size_t) cp[1] );
  	} else {
      		current_symbol->decode_func (cp+1+tlv_llen, current_symbol, tlv_vlen );
  	}
#ifdef DEBUG
	if (cp[0] == 64 )   /* TLV 64 has length encoded as a short */
	   printf("/* TLV 64, size %hu */ \n", ntohs(*((unsigned short *)(cp+1))) );
#endif
    cp = (unsigned char*) cp + 1 + tlv_llen + tlv_vlen ; /* type, length, value   */
  }
  __docsis_indent(INDENT_DECREMENT, FALSE);
  printf("}\n");
}

int
hexadecimal_to_binary (const char *str, unsigned char * bufp)
{
  int len, itmp;
#ifdef DEBUG
  printf("Hex string rx'd: %s\n", str);
#endif
  if (!bufp)
    return -1;
  if (*str && *str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X'))
    str += 2;
  for (len = 0; *str; str++)
    {
      if (isspace ((int) *str))
	continue;
      if (!isxdigit ((int) *str))
	return -1;
      len++;
      if (sscanf (str++, "%2x", &itmp) == 0)
	return -1;
      *bufp++ = itmp;
      if (!*str)
	return -1;		/* odd number of chars is an error */
    }
  return len;
}

int
str_isalpha (const char *str, size_t str_len)
{
  unsigned int i;

  for (i=0; i<str_len; i++)
		if (!(isalnum((int) str[i]) && isprint((int) str[i]) && isascii((int) str[i])) ) return FALSE;

  return TRUE;
}

int
str_isprint (const char *str, size_t str_len)
{
  unsigned int i;

  for (i=0; i<str_len; i++)
		if (!(isprint((int) str[i]))) return FALSE;

  return TRUE;
}

/*
 * Print a string in hex format output ... for strings that are not alphanumeric, i.e.
 * HexString, BitString etc
 * str is the original string
 * str_len is the length of the original string
 * outbuf is a pointer to the output buffer
 */


void
snprint_hexadecimal ( char *outbuf, size_t outsize, const char *str, size_t str_len )
{
  unsigned int i;
  char *cp;

  cp=outbuf;

  memset (outbuf, 0, outsize);
  sprintf(cp, "0x");
  cp = cp +2*sizeof(char);

  for (i=0; i<str_len && (unsigned int) (cp-outbuf) < outsize; i++) {
	sprintf(cp, "%02x", (unsigned char) str[i]);
	cp = cp +2*sizeof(char);
  }

}

/* Simple indent handler ... */

void __docsis_indent(int opCode, int doPrint )
{
	static int numtabs;
	int i;

	switch (opCode)
	{
		case INDENT_INCREMENT:
			numtabs++;
			break;
			;;
		case INDENT_DECREMENT:
			numtabs--;
			break;
			;;
		case INDENT_CLEAR:
			numtabs=0;
			break;
			;;
	}

	if ( doPrint )
	{
		for (i=0; i<numtabs; i++) printf("\t");
	}
}

