
/* 
 *  DOCSIS configuration file encoder. 
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <ucd-snmp/ucd-snmp-config.h>
#include <ucd-snmp/ucd-snmp-includes.h>
#include <ucd-snmp/system.h>
#include <ucd-snmp/parse.h>
#include <ucd-snmp/mib.h>

#include "md5.h"
#include "docsis_decode.h"
#include "version.h"

#ifdef SOLARIS
#include "inet_aton.h"
#endif

#ifndef NUM_IDENTIFIERS
#define NUM_IDENTIFIERS 29
#endif /*  NUM_IDENTIFIERS, also defined in docsis_symtable.h  */

#define MAXINT 2000000000 
#define TLV_VSIZE 255 


struct symbol_entry { 
	char sym_ident[30];
	unsigned char docsis_code;
	unsigned char docsis_parent;
	int (*get_value_func) (unsigned char *, void *, struct symbol_entry *);
	unsigned int low_limit;
	unsigned int high_limit;
}; 

typedef struct symbol_entry symbol_type;

struct tlv { 
	unsigned char docs_code;
	unsigned char tlv_len;
	unsigned char tlv_value[TLV_VSIZE];
};

struct tlv_list {
 	int tlv_count;	/* How many TLVs we have in this list */ 
	struct tlv **tlvlist;
};

union t_val {           /* union for returning token values */
        int intval;             /* For integers */
        unsigned int uintval;             /* For integers */
        symbol_type *symptr;    /* For token identifiers */
        char *strval;           /* For strings */
        unsigned char *ustrval; /* For strings */
        unsigned int ip;        /* For IP Addresses */
        struct tlv_list *tlvlist; /* For for struct tlvlist pointers */
        struct tlv *tlvptr;     /* For struct tlv pointers; */
};

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

/* Find the symbol_entry given the symbol identifier */
struct symbol_entry *find_sym (char *sym_str);
/* each of these returns the length of the value parsed ! */

int get_uint   (unsigned char *buf, void *tval, struct symbol_entry *sym_ptr ); 
int get_short  (unsigned char *buf, void *tval, struct symbol_entry *sym_ptr ); 
int get_uchar  (unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_ip     (unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_mac    (unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_string (unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_nothing(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );

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

