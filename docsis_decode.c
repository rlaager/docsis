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

#include "docsis.h"
#include "docsis_decode.h"
#include "ethermac.h"

#include <math.h>
#include <ctype.h>

extern symbol_type *global_symtable;


struct symbol_entry *find_symbol_by_code_and_pid (unsigned char code, unsigned int pid)
{
 int i;

 for ( i=0; i<NUM_IDENTIFIERS; i++) {
        if (global_symtable[i].docsis_code == code && global_symtable[i].parent_id == pid) {
                return &global_symtable[i];
        }
 }
 return NULL;
}

void decode_uint (unsigned char *tlvbuf, struct symbol_entry *sym)
{
  static unsigned int helper;

  memset( &helper, 0,sizeof(unsigned int));
  memcpy( &helper, tlvbuf+2, sizeof(unsigned int));

  printf ( "%s %u;\n", sym->sym_ident, ntohl(helper));
}

void decode_ushort (unsigned char *tlvbuf, symbol_type *sym)
{
  static unsigned short helper;

  memset( &helper, 0,sizeof(unsigned short));
  memcpy( &helper, tlvbuf+2, sizeof(unsigned short));

  printf ( "%s %hu;\n", sym->sym_ident, ntohs(helper));
}

void decode_uchar (unsigned char *tlvbuf, symbol_type *sym)
{
  printf ( "%s %hhu;\n", sym->sym_ident, (unsigned char) *(tlvbuf+2));
}

void decode_ip (unsigned char *tlvbuf, symbol_type *sym)
{
  static struct in_addr helper;
  memcpy (&helper, tlvbuf+2, sizeof(struct in_addr));
  printf ( "%s %s;\n", 
	sym->sym_ident, inet_ntoa(helper) );
}

void decode_ether (unsigned char *tlvbuf, symbol_type *sym)
{
  printf ( "%s %s;\n", 
	sym->sym_ident, ether_ntoa(tlvbuf+2));
}

void decode_ethermask (unsigned char *tlvbuf, symbol_type *sym)
{
/* the return value of ether_ntoa is a pointer to a static string 
 * in the ether_ntoa function, so we have to print the values in two
 * "passees" to avoid the 2nd call overwriting the 1st. 
 */
  printf ( "%s %s/", sym->sym_ident, ether_ntoa(tlvbuf+2));

  printf( "%s;\n", ether_ntoa(tlvbuf+8));
}

void decode_md5 (unsigned char *tlvbuf, symbol_type *sym)
{
  int j=0;
  printf ("/* %s ", sym->sym_ident);
  for (j=0;j<16;j++)	printf ("%02x", tlvbuf[j+2]);

  printf("; */\n");
}

void decode_snmp_wd (unsigned char *tlvbuf, symbol_type *sym)
{
  printf ( "%s ", sym->sym_ident);
  decode_wd (tlvbuf+2,(unsigned int) tlvbuf[1]);
  printf(";\n");
}

void decode_snmp_object (unsigned char *tlvbuf, symbol_type *sym)
{
  printf ( "%s ", sym->sym_ident);
  decode_vbind (tlvbuf+2, (unsigned int) tlvbuf[1]);
/*  22-06-03  decode_vbind prints the trailing ';' as well  */
  printf("\n"); 
}

void decode_string (unsigned char *tlvbuf, symbol_type *sym)
{
 char *helper; 

 helper = (char *) malloc ( ((unsigned int) tlvbuf[1])+1 ); 
 memset ( helper, 0, tlvbuf[1]+1);
 strncpy ( helper, (char *) tlvbuf+2, (unsigned int) tlvbuf[1] );
 printf ( "%s \"%s\";\n", sym->sym_ident, helper );
}

void decode_strzero (unsigned char *tlvbuf, symbol_type *sym)
{
 char *helper;

 helper = (char *) malloc ( ((unsigned int) tlvbuf[1])+1 );
 memset ( helper, 0, tlvbuf[1]+1);
 strncpy ( helper, (char *) tlvbuf+2, (unsigned int) tlvbuf[1] );
 printf ( "%s \"%s\";\n", sym->sym_ident, helper );
}


void decode_hexstr (unsigned char *tlvbuf, symbol_type *sym)
{
 char *helper; 
 unsigned int i;
 unsigned int len;

/* TODO */
 len = (unsigned int) tlvbuf[1]; 
 helper = (char *) malloc ( len+1 ); 
 memset ( helper, 0, len+1);
 memcpy ( helper, (char *) tlvbuf+2, len );
 printf ( "%s 0x", sym->sym_ident);
 for(i=0; i<len; i++) { 
	printf("%02x", (unsigned char) helper[i]);
 }
 printf(";\n");
 free(helper);
}

void decode_unknown (unsigned char *tlvbuf, symbol_type *sym)
{
  int len=0;
  char *cp,*value;
  char hexvalue[514];

  len = (unsigned int) tlvbuf[1];  

  memset (hexvalue, 0, 514);

  value = (char *) malloc ( len+1 ); 
  memset ( value, 0, len+1);
  memcpy ( value, (char *) tlvbuf+2, len );


  printf ("GenericTLV TlvCode %d TlvLength %d TlvValue ", 
			(unsigned int) tlvbuf[0], len) ;

  snprint_hexadecimal ( hexvalue, 514, value, len);

  printf("%s;", hexvalue);

  cp = value;

  if ( str_isprint(cp, len) ) { 
	printf ("  /* %s */", cp);
  }

  printf("\n");

  free(value);
}

void decode_special (unsigned char *tlvbuf, symbol_type *sym)
{
  printf ( "%s\n", sym->sym_ident);
}

void decode_aggregate (unsigned char *tlvbuf, symbol_type *sym)
{
  register unsigned char *cp;
  symbol_type *current_symbol;
  

  cp = tlvbuf+2; /* skip type,len of parent TLV */

  printf( "%s\n", sym->sym_ident);
  __docsis_indent(INDENT_NOOP, TRUE);
  printf( "{\n");

  __docsis_indent(INDENT_INCREMENT, FALSE);

/*  while ( (unsigned int) (cp - tlvbuf +(sizeof(unsigned char))) < (unsigned int) tlvbuf[1] ) { */
  while ( (unsigned int) (cp - tlvbuf) < (unsigned int) tlvbuf[1] ) {

  __docsis_indent(INDENT_NOOP, TRUE);

  current_symbol = find_symbol_by_code_and_pid (cp[0], sym->id);
  if (current_symbol == NULL) { 
		decode_unknown(cp, NULL);	
  	} else { 
      		current_symbol->decode_func (cp, current_symbol);
  	}
 	cp = (unsigned char*) cp + (((unsigned char)*(cp+1))+2)*sizeof(unsigned char);
  }

  __docsis_indent(INDENT_DECREMENT, FALSE);

  __docsis_indent(INDENT_NOOP, TRUE);
  printf("}\n");
}

/* 
** This is just like decode_aggregate, but it only does symbol lookup 
** for the VendorIdentifier which is the only 'known' TLV.
**/

void decode_vspecific (unsigned char *tlvbuf, symbol_type *sym)
{
  register unsigned char *cp;
  symbol_type *first_symbol;

  cp = tlvbuf+2; /* skip type,len of parent TLV */

  printf( "%s\n", sym->sym_ident);
  __docsis_indent(INDENT_NOOP, TRUE);
  printf( "{\n");

  __docsis_indent(INDENT_INCREMENT, FALSE);

  /* First TLV inside VendorSpecific has to be VendorIdentifier... the rest we don't care */ 
  
  first_symbol = find_symbol_by_code_and_pid (cp[0], sym->id); 
  
  if (first_symbol == NULL) {
  		__docsis_indent(INDENT_NOOP, TRUE);
		printf ("/* WARNING: Invalid VendorSpecific option - 1st element is NOT VendorIdentifier */\n");
  		__docsis_indent(INDENT_NOOP, TRUE);
                decode_unknown(cp, NULL);
        } else {
	/* Symbol found ... check if it's the right one */ 
		if (!strncmp (first_symbol->sym_ident, "VendorIdentifier", 16)) 
		{ 
  			__docsis_indent(INDENT_NOOP, TRUE);
	                first_symbol->decode_func(cp, first_symbol);
		} 
		else 
		{ 
  			__docsis_indent(INDENT_NOOP, TRUE);
			printf ("/* Invalid VendorSpecific option - 1st element is NOT VendorIdentifier */");
  			__docsis_indent(INDENT_NOOP, TRUE);
			decode_unknown(cp, NULL);
		}
        }

  cp = (unsigned char*) cp + (((unsigned char)*(cp+1))+2)*sizeof(unsigned char);

  while ( (unsigned int) (cp - tlvbuf) < (unsigned int) tlvbuf[1] ) {

  __docsis_indent(INDENT_NOOP, TRUE);

        decode_unknown(cp, NULL);
        cp = (unsigned char*) cp + (((unsigned char)*(cp+1))+2)*sizeof(unsigned char);
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
 * It's also a bif different from docsis_aggregate in that docsis_aggregate 
 * takes an aggregate tlvbuf as argument that INCLUDES the "parent" code and 
 * length. On the main aggregate we don't have a code / length. 
 */

void decode_main_aggregate (unsigned char *tlvbuf, unsigned int buflen)
{
  register unsigned char *cp;
  symbol_type *current_symbol;
  
  cp = tlvbuf; 

  __docsis_indent(INDENT_CLEAR, FALSE);
	  
  printf( "Main \n{\n");
  __docsis_indent(INDENT_INCREMENT, FALSE);

  while ( (unsigned int) (cp - tlvbuf) < buflen ) {

  __docsis_indent(INDENT_NOOP, TRUE);

  current_symbol = find_symbol_by_code_and_pid (cp[0],0);
  if (current_symbol == NULL) { 
		decode_unknown(cp, NULL);	
  	} else { 
      		current_symbol->decode_func (cp, current_symbol);
  	}
 	cp = (unsigned char*) cp + (((unsigned char)*(cp+1))+2)*sizeof(unsigned char);
  }
  __docsis_indent(INDENT_DECREMENT, FALSE);
  printf("}\n");
}

int
hexadecimal_to_binary (const char *str, u_char * bufp)
{
  int len, itmp;
  printf ("Hex string rx'd: %s\n", str);
  if (!bufp)
    return -1;
  if (*str && *str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X'))
    str += 2;
  for (len = 0; *str; str++)
    {
      if (isspace (*str))
	continue;
      if (!isxdigit (*str))
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
		if (!(isalnum(str[i]) && isprint(str[i]) && isascii(str[i])) ) return FALSE; 

  return TRUE;
}

int 
str_isprint (const char *str, size_t str_len) 
{
  unsigned int i;

  for (i=0; i<str_len; i++) 
		if (!(isprint(str[i]))) return FALSE; 

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
snprint_hexadecimal ( unsigned char *outbuf, size_t outsize, const char *str, size_t str_len ) 
{
  unsigned int i;
  unsigned char *cp;

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
		for (i=0; i<numtabs; i++) printf ("\t"); 
	}
}

