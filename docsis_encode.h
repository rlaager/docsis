
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

#ifndef _DOCSIS_ENCODE_H
#define _DOCSIS_ENCODE_H

#include "docsis_common.h"

/* each of these returns the length of the value parsed ! */

int get_uint   	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr ); 
int get_ushort 	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr ); 
int get_uchar  	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_ip     	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_ether	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_ethermask(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_string 	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_hexstr 	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );
int get_nothing	(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr );

#endif /* _DOCSIS_ENCODE_H */

