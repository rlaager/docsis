
/* 
 *  DOCSIS configuration file encoder. 
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002 Evvolve Media SRL, office@evvolve.com
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

#ifndef _DOCSIS_H
#define _DOCSIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/utilities.h>
#include <net-snmp/config_api.h>
#include <net-snmp/output_api.h>
#include <net-snmp/mib_api.h>


#include "md5.h"
#include "docsis_common.h"

#ifdef SOLARIS
#include "inet_aton.h"
#endif

int yyerror(char *s);                  

/* 
 * create a TLV from scratch, return pointer to TLV struct 
 */
struct tlv *create_tlv ( struct symbol_entry *sym_ptr, union t_val *value);
struct tlv *create_snmpset_tlv( struct symbol_entry *sym_ptr,
                                char *oid_name,
                                char oid_type,
                                union t_val *value ); 
struct tlv *create_snmpw_tlv ( struct symbol_entry *sym_ptr,
                               char *oid_string,
                               union t_val *value );         
struct tlv *create_cpemac_tlv(struct symbol_entry *sym_ptr, union t_val *value);


/* add a TLV to a list; create the list if NULL, otherwise re-alloc new list, 
   copy new TLV and old list to new one  and free old list  */
struct tlv_list *add_tlv_to_list(struct tlv_list *list, struct tlv *newtlv);

struct tlv_list *merge_tlvlist( struct tlv_list *list1,struct tlv_list *list2);  

/* create a tlvlist with 1 tlv (parent) from a sym_ptr and a tlvlist */

struct tlv_list *assemble_list_in_parent ( struct symbol_entry *sym_ptr, struct tlv_list *list);
/* flatten a tlvlist to its final binary form */
unsigned int flatten_tlvlist ( unsigned char *buf, struct tlv_list *list );

/* Find the symbol_entry given the symbol identifier -> docsis_lex.l  */
struct symbol_entry *find_symbol_by_name (char *sym_str);

/* each of these returns the length of the value parsed ! */

unsigned int encode_vbind ( char *oid_string, char oid_asntype, union t_val *value,
                        unsigned char *out_buffer, unsigned int out_size );
int decode_vbind ( unsigned char *, unsigned int);
unsigned int encode_oid ( char *oid_string, unsigned char *out_buffer, unsigned int out_size );
unsigned int decode_wd ( unsigned char *data, unsigned int data_len); 
void hmac_md5(unsigned char *text, int text_len, unsigned char *key, int key_len, unsigned char *digest);
void md5_print_digest ( unsigned char *digest );
int init_global_symtable ( void) ;
int parse_input_file( FILE *afile );
int hexadecimal_to_binary(const char *, u_char *);
int yylex(void);
void decode_file ( char *file );

#endif /* _DOCSIS_H */

