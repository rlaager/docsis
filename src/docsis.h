/*
 *  DOCSIS configuration file encoder.
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002,2003,2004,2005 Evvolve Media SRL, office@evvolve.com
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

#ifndef __DOCSIS_H
#define __DOCSIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
/*
#ifdef WIN32
#include <winsock.h>
#else */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/* #endif */

#include "docsis_common.h"

int yyerror (char *s);

/*
 * create a TLV from scratch, return pointer to TLV struct
 */
struct tlv *create_tlv (struct symbol_entry *sym_ptr, union t_val *value);
struct tlv *create_snmpset_tlv (struct symbol_entry *sym_ptr,
				char *oid_name,
				char oid_type, union t_val *value);
struct tlv *create_generic_tlv (struct symbol_entry *sym_ptr,
				int tlv_code,
				int tlv_length, union t_val *value);
struct tlv *create_generic_str_tlv (struct symbol_entry *sym_ptr,
				int tlv_code,
				union t_val *value);
struct tlv *create_generic_strzero_tlv (struct symbol_entry *sym_ptr,
				int tlv_code,
				union t_val *value);
struct tlv *create_generic_typed_tlv (struct symbol_entry *sym_ptr,
				int tlv_code,
				encode_func_t encode_func,
				union t_val *value);
struct tlv *create_snmpw_tlv (struct symbol_entry *sym_ptr,
			      char *oid_string, union t_val *value);

struct tlv *create_cpemac_tlv (struct symbol_entry *sym_ptr,
			       union t_val *value);
struct tlv *create_external_file_tlv (struct symbol_entry *sym_ptr,
			       union t_val *value);


/* add a TLV to a list; create the list if NULL, otherwise re-alloc new list,
   copy new TLV and old list to new one  and free old list  */
struct tlv *add_tlv_sibling (struct tlv *tlv, struct tlv *newtlv);

struct tlv *merge_tlvlist (struct tlv *tlv1,
				struct tlv *tlv2 );

/* create a tlvlist with 1 tlv (parent) from a tlvcode and a tlvlist */

struct tlv *assemble_tlv_in_parent (int tlvcode,
					  struct tlv *tlv);
/* flatten a tlvlist to its final binary form */
unsigned int flatten_tlvlist (unsigned char *buf, struct tlv_list *list);
unsigned int flatten_tlvsubtree (unsigned char *buf, unsigned int used_size, struct tlv *tlvhead);

/* Find the symbol_entry given the symbol identifier -> docsis_lex.l  */
struct symbol_entry *find_symbol_by_name (char *sym_str);

void hmac_md5 (unsigned char *text, int text_len, unsigned char *key,
	       size_t key_len, unsigned char *digest);
void md5_print_digest (unsigned char *digest);
int init_global_symtable (void);
unsigned int tlvtreelen  (struct tlv *tlv);
int parse_config_file (char *file, struct tlv **parse_tree_result);
int yylex (void);
void decode_file (char *file);
int encode_one_file (char *input_file, char *output_file,
		       unsigned char *key, unsigned int keylen, int encode_docsis, unsigned int hash);
char *get_output_name (char *input_path, char *new_extension);

#endif /* __DOCSIS_H */
