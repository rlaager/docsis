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
#include "ethermac.h"

#include <math.h>

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
  printf ( "%s %s;\n", 
	sym->sym_ident, inet_ntoa(*((struct in_addr *)(tlvbuf+2))) );
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
  printf ("%s ", sym->sym_ident);
  for (j=0;j<16;j++)	printf ("%02x", tlvbuf[j+2]);

  printf(";\n");
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
  printf(";\n");
}

void decode_string (unsigned char *tlvbuf, symbol_type *sym)
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
 int i;

/* TODO */

 helper = (char *) malloc ( ((unsigned int) tlvbuf[1])+1 ); 
 memset ( helper, 0, tlvbuf[1]+1);
 memcpy ( helper, (char *) tlvbuf+2, (unsigned int) tlvbuf[1] );
 printf ( "%s 0x", sym->sym_ident);
 for(i=0; i<(int) tlvbuf[1]; i++) { 
	printf("%02x", (unsigned char) helper[i]);
 }
 printf(";\n");
}

void decode_unknown (unsigned char *tlvbuf, symbol_type *sym)
{
  int j=0,len=0;

  len = (int) tlvbuf[1];  

  printf ("/* GenericUnknownTLV code %d len %d value 0x ", 
			(unsigned int) tlvbuf[0],(unsigned int) tlvbuf[1]);

  for (j=0;j<len;j++)	printf ("%02x", tlvbuf[j+2]);

  printf("; */\n");
}

void decode_special (unsigned char *tlvbuf, symbol_type *sym)
{
  printf ( "%s\n", sym->sym_ident);
}

void decode_aggregate (unsigned char *tlvbuf, symbol_type *sym)
{
  register unsigned char *cp;
  symbol_type *current_symbol;
  
  cp = tlvbuf+2*sizeof(unsigned char); /* skip type,len of parent TLV */
  printf( "%s {\n", sym->sym_ident);

  while ( (unsigned int) (cp - tlvbuf)  < (unsigned int) tlvbuf[1] ) {
  current_symbol = find_symbol_by_code_and_pid (cp[0], sym->id);
  if (current_symbol == NULL) { 
		decode_unknown(cp, NULL);	
  	} else { 
      		current_symbol->decode_func (cp, current_symbol);
  	}
 	cp = (unsigned char*) cp + (((unsigned char)*(cp+1))+2)*sizeof(unsigned char);
  }
  printf("}\n");
}

/* This function is needed because we don't have a symbol to call it. 
 * We can't put a "Main" symbol in the symtable because docsis_code is 
 * unsigned char (in struct symbol_entry) and we reserve the values for 
 * DOCSIS use. 
 * It's also a bif different from docsis_aggregate in that docsis_aggregate 
 * takes an aggregate tlvbuf as argument that INCLUDES the "parent" code and 
 * length. On the main aggregate we don't have a "parent" code / length, the 
 * first code/length is the first configuration setting.
 */

void decode_main_aggregate (unsigned char *tlvbuf, unsigned int buflen)
{
  register unsigned char *cp;
  symbol_type *current_symbol;
  
  cp = tlvbuf; 
  printf( "Main {\n");

  while ( (unsigned int) (cp - tlvbuf) < buflen ) {
  current_symbol = find_symbol_by_code_and_pid (cp[0],0);
  if (current_symbol == NULL) { 
		decode_unknown(cp, NULL);	
  	} else { 
      		current_symbol->decode_func (cp, current_symbol);
  	}
 	cp = (unsigned char*) cp + (((unsigned char)*(cp+1))+2)*sizeof(unsigned char);
  }
  printf("}\n");
}
