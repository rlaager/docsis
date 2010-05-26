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
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  DOCSIS is a registered trademark of Cablelabs, http://www.cablelabs.com
 */


#ifndef _DOCSIS_SNMP_H
#define _DOCSIS_SNMP_H

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/utilities.h>
#include <net-snmp/config_api.h>
#include <net-snmp/output_api.h>
#include <net-snmp/mib_api.h>

#include "docsis.h"

unsigned int encode_vbind ( char *oid_string, char oid_asntype, union t_val *value,
                        unsigned char *out_buffer, size_t out_size );
int decode_vbind ( unsigned char *, unsigned int);
unsigned int encode_snmp_oid ( char *oid_string, unsigned char *out_buffer, size_t out_size );
unsigned int decode_snmp_oid ( unsigned char *data, size_t data_len);

unsigned char *_docsis_snmp_build_var_op(unsigned char * data,
                  oid * var_name,
                  size_t * var_name_len,
                  unsigned char var_val_type,
                  size_t var_val_len,
                  unsigned char * var_val, size_t * listlength);

unsigned char *_docsis_asn_build_sequence(unsigned char * data, size_t * datalength, unsigned char type, size_t length);

#endif /* _DOCSIS_SNMP_H */

