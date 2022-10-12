/*
 *  DOCSIS configuration file encoder.
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002,2003,2004,2005 Evvolve Media SRL,office@evvolve.com
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

#ifndef _DOCSIS_COMMON_H
#define _DOCSIS_COMMON_H

/* needed in docsis_symtable.h */
#define NUM_IDENTIFIERS 1431

#define MAXINT 2000000000
#define TLV_VSIZE 1024
#define TRUE 1
#define FALSE 0

#define INDENT_NOOP 		100
#define INDENT_CLEAR 		101
#define INDENT_INCREMENT 	102
#define INDENT_DECREMENT 	103

struct symbol_entry;

typedef int (*encode_func_t) (unsigned char *, void *, struct symbol_entry *);
typedef void (*decode_func_t) (unsigned char *, struct symbol_entry *, size_t length);

struct symbol_entry {
	unsigned int id;
	char sym_ident[50];
	unsigned char docsis_code;
	unsigned int parent_id;
	encode_func_t encode_func;
	decode_func_t decode_func;
	unsigned int low_limit;
	unsigned int high_limit;
};

typedef struct symbol_entry symbol_type;

struct tlv {
	unsigned short docs_code;
	unsigned short tlv_len;
	unsigned char tlv_value[TLV_VSIZE];
	struct tlv *parent;
	struct tlv *next_sibling;
	struct tlv *first_child;
};

union t_val {           /* union for returning token values */
        int intval;             /* For integers */
        unsigned int uintval;   /* For unsigned integers */
        symbol_type *symptr;    /* For token identifiers */
        char *strval;           /* For strings */
        unsigned char *ustrval; /* For (unsigned char *) strings */
        unsigned int ip;        /* For IP Addresses */
        struct tlv_list *tlvlist; /* For for struct tlvlist pointers */
        struct tlv *tlvptr;     /* For struct tlv pointers; */
};

#endif /* _DOCSIS_COMMON_H */

