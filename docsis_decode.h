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

#ifndef _DOCSIS_DECODE_H
#define _DOCSIS_DECODE_H

#include "docsis_common.h"

int decode_tlvbuf (unsigned char *buf, unsigned int buflen, unsigned char docs_parent); 
int pretty_decode_buffer (unsigned char *buf, unsigned int buflen, unsigned char docs_parent); 
struct symbol_entry *find_symbol_by_code_and_pid (unsigned char code, unsigned int pid);

void decode_special (unsigned char *tlvbuf, symbol_type *sym);
void decode_uint (unsigned char *tlvbuf, struct symbol_entry *sym);
void decode_ushort (unsigned char *tlvbuf, symbol_type *sym);
void decode_uchar (unsigned char *tlvbuf, symbol_type *sym);
void decode_ip (unsigned char *tlvbuf, symbol_type *sym);
void decode_ether (unsigned char *tlvbuf, symbol_type *sym);
void decode_ethermask (unsigned char *tlvbuf, symbol_type *sym);
void decode_md5 (unsigned char *tlvbuf, symbol_type *sym);
void decode_snmp_wd (unsigned char *tlvbuf, symbol_type *sym);
void decode_snmp_object (unsigned char *tlvbuf, symbol_type *sym);
void decode_string (unsigned char *tlvbuf, symbol_type *sym);
void decode_strzero (unsigned char *tlvbuf, symbol_type *sym);
void decode_hexstr (unsigned char *tlvbuf, symbol_type *sym);
void decode_unknown (unsigned char *tlvbuf, symbol_type *sym);
void decode_aggregate (unsigned char *tlvbuf, symbol_type *sym);
void decode_main_aggregate (unsigned char *tlvbuf, unsigned int buflen);
void decode_vspecific(unsigned char *tlvbuf, symbol_type *sym);
int hexadecimal_to_binary(const char *, u_char *);
int str_isalpha (const char *str, size_t str_len);
int str_isprint (const char *str, size_t str_len);
void snprint_hexadecimal ( unsigned char *outbuf, size_t outsize, const char *str, size_t str_len );

void __docsis_indent ( int opCode, int doPrint );

#endif /* _DOCSIS_DECODE_H */
