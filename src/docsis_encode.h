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

#ifndef _DOCSIS_ENCODE_H
#define _DOCSIS_ENCODE_H

/* each of these returns the length of the value parsed ! */

int encode_uint   	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_uint24   	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ushort 	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_uchar  	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ip     	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ip_list     	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ip6     	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ip6_list     	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ip6_prefix_list  	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ip_ip6  	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_char_ip_ip6  	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ip_ip6_port  	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_lenzero  	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ether	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_dual_qtag	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_char_list	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ethermask	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_string 	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_strzero	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_hexstr 	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_oid 		(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_ushort_list	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int encode_nothing	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );

#endif /* _DOCSIS_ENCODE_H */

