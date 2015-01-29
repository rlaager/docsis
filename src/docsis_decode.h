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

#ifndef _DOCSIS_DECODE_H
#define _DOCSIS_DECODE_H

#include <stdlib.h>
#include "docsis_common.h"

struct symbol_entry *find_symbol_by_code_and_pid (unsigned char code, unsigned int pid);

void decode_special (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_uint (unsigned char *tlvbuf, struct symbol_entry *sym, size_t length );
void decode_uint24 (unsigned char *tlvbuf, struct symbol_entry *sym, size_t length );
void decode_ushort (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_uchar (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_ip (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_ip_list (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_ip6 (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_ip6_list (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_ip6_prefix_list (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_ip_ip6 (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_char_ip_ip6 (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_ip_ip6_port (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_lenzero (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_ether (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_dual_qtag (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_char_list (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_ethermask (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_md5 (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_snmp_wd (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_snmp_object (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_oid (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_string (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_strzero (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_hexstr (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_ushort_list (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_unknown (unsigned char *tlvbuf, symbol_type *sym, size_t length );
void decode_aggregate (unsigned char *tlvbuf, symbol_type *sym, size_t length);
void decode_main_aggregate (unsigned char *tlvbuf, size_t buflen);
void decode_vspecific(unsigned char *tlvbuf, symbol_type *sym, size_t length);
int hexadecimal_to_binary(const char *string, unsigned char *out_buf);
int str_isalpha (const char *str, size_t str_len);
int str_isprint (const char *str, size_t str_len);
void snprint_hexadecimal ( char *outbuf, size_t outsize, const char *str, size_t str_len );

void __docsis_indent ( int opCode, int doPrint );

#endif /* _DOCSIS_DECODE_H */
