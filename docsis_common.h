
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

/* 
    change history
	2003-01-10 changed NUM_IDENTIFIERS to match what we have in docsis_symtable.h

*/

#ifndef _DOCSIS_COMMON_H
#define _DOCSIS_COMMON_H

#include "version.h"

#ifndef NUM_IDENTIFIERS
#define NUM_IDENTIFIERS 129
#endif /*  NUM_IDENTIFIERS, needed in docsis_symtable.h  */

#define MAXINT 2000000000 
#define TLV_VSIZE 255 
#define TRUE 1
#define FALSE 0

#define INDENT_NOOP 		100
#define INDENT_CLEAR 		101
#define INDENT_INCREMENT 	102
#define INDENT_DECREMENT 	103

struct symbol_entry { 
	unsigned int id;
	char sym_ident[30];
	unsigned char docsis_code;
	unsigned int parent_id;
	int (*get_value_func) (unsigned char *, void *, struct symbol_entry *);
	void (*decode_func) (unsigned char *, struct symbol_entry *);
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

#endif /* _DOCSIS_COMMON_H */

