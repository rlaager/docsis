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

#include "docsis.h"
#include <ctype.h>

extern unsigned int line; /* from a.l */

#define PACKET_LENGTH (8 * 1024)

unsigned int encode_vbind ( char *oid_string, char oid_asntype, union t_val *value, unsigned char *out_buffer, unsigned int out_size )
{
  oid var_name[MAX_OID_LEN];
  size_t name_len=MAX_OID_LEN;
  unsigned char *data_ptr;
  unsigned char buf[SPRINT_MAX_LEN];
  unsigned int ltmp;
  struct tree *tp; 
  struct range_list *rp; 
  long len;
  int rv;
  
  memset (buf, 0, SPRINT_MAX_LEN);
  if ( !read_objid(oid_string, var_name, &name_len) ) { 
	if (!get_node(oid_string, var_name, &name_len) ) { 
		printf("Can't find oid %s at line %d\n", oid_string, line);
		exit ( -37); 
	}
  }

  switch ( oid_asntype ) {
      case 'i':
	data_ptr = snmp_build_var_op ( 	out_buffer,
					var_name,
					&name_len,
					ASN_INTEGER, 
					sizeof(int),
					(unsigned char *) &value->intval, 
					&out_size); 
	return data_ptr-out_buffer;	
	break;
	;;
      case 's':
      case 'x':
      case 'd': 
        if (oid_asntype == 'd'){
         /* ltmp = ascii_to_binary(value->strval, buf);
          strncpy((char*)buf, value, SPRINT_MAX_BUF);
          ltmp = strlen((char*)buf); */
        } else if (oid_asntype == 's'){
          strncpy((char*)buf, value->strval, SPRINT_MAX_LEN);
        } else if (oid_asntype == 'x'){
           if ( (rv = hex_to_binary(value->strval, buf)) == -1 ) {
		printf("Invalid hexadecimal string at line %d\n", line);
		exit(-200);
		}
	   ltmp = (unsigned int ) rv;
        }

          len = strlen((char*)buf);
        if (len < 0 || len > SPRINT_MAX_LEN-1) {
          printf ("String too long at line %d, max allowed %d\n", line,SPRINT_MAX_LEN);
	  exit (67);
          break;
        }
        tp = get_tree(var_name, name_len, get_tree_head());
        if (tp && tp->ranges && !ds_get_boolean(DS_LIBRARY_ID, DS_LIB_DONT_CHECK_RANGE)) {
            rp = tp->ranges;
            while (rp) {
                if (rp->low <= len && len <= rp->high) break;
                rp = rp->next;
            }
            if (!rp) {
                printf ("Value too long at line %d\n", line);
		exit(68);
                break;
            }
        }
        data_ptr = snmp_build_var_op (	out_buffer,
					var_name,
					&name_len,
					ASN_OCTET_STR, 
					len, 
					(unsigned char *) buf,
					&out_size);
	return data_ptr-out_buffer;	
        break;                                                    
      case 'a':
	if (!inet_aton(value->strval, (struct in_addr *) &ltmp ) ) { 
	    printf ( "Invalid IP address %s at line %d\n",value->strval,line);
	    return 0;
	}
	data_ptr = snmp_build_var_op ( 	out_buffer,
					var_name,
					&name_len,
					ASN_IPADDRESS, 
					sizeof(int),
					(unsigned char *) &ltmp, 
					&out_size); 
	return data_ptr-out_buffer;	
	break;
	;;
      default: 
	printf("Variable type %s not supported yet", &oid_asntype );	
	return 0;
	;;
  }
/* NOTREACHED */
}
       

int decode_vbind ( unsigned char *data, unsigned int vb_len ) 
{ 
   unsigned char *var_val; 
   int badtype;
   unsigned int len;
   struct variable_list *vp;
   oid objid[MAX_OID_LEN]; 
	

   if (! ds_get_boolean (DS_LIBRARY_ID,DS_LIB_PRINT_NUMERIC_OIDS)) { 
	ds_toggle_boolean(DS_LIBRARY_ID, DS_LIB_PRINT_NUMERIC_OIDS);  
   }
   if (! ds_get_boolean (DS_LIBRARY_ID,DS_LIB_PRINT_FULL_OID)) { 
	ds_toggle_boolean(DS_LIBRARY_ID, DS_LIB_PRINT_FULL_OID);  
   }

   vp = ( struct variable_list *)malloc ( sizeof(struct variable_list));
   if ( vp == NULL ) { 
	exit (-12);
   }

    vp->next_variable = NULL;
    vp->val.string = NULL;
    vp->name_length = MAX_OID_LEN;
    vp->name = 0;
    data = snmp_parse_var_op(data, objid, &vp->name_length, &vp->type,&vp->val_len, &var_val, ( size_t * ) &vb_len );
                             
    if (data == NULL)
      return -1;
    if (snmp_set_var_objid(vp, objid, vp->name_length))
        return -1;

    len = PACKET_LENGTH;
    switch((short)vp->type){
    case ASN_INTEGER:
      vp->val.integer = (long *)vp->buf;
      vp->val_len = sizeof(long);
      asn_parse_int(var_val, &len, &vp->type,
                    (long *)vp->val.integer,
                    sizeof(vp->val.integer));
      break;
    case ASN_COUNTER:
    case ASN_GAUGE:
    case ASN_TIMETICKS:
    case ASN_UINTEGER:
      vp->val.integer = (long *)vp->buf;
      vp->val_len = sizeof(u_long);
      asn_parse_unsigned_int(var_val, &len, &vp->type,
                             (u_long *)vp->val.integer,
                             sizeof(vp->val.integer));
      break;
#ifdef OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_COUNTER64:
    case ASN_OPAQUE_U64:
#endif /* OPAQUE_SPECIAL_TYPES */
    case ASN_COUNTER64:
      vp->val.counter64 = (struct counter64 *)vp->buf;
      vp->val_len = sizeof(struct counter64);
      asn_parse_unsigned_int64(var_val, &len, &vp->type,
                               (struct counter64 *)vp->val.counter64,
                               sizeof(*vp->val.counter64));
      break;
#ifdef OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_FLOAT:
      vp->val.floatVal = (float *)vp->buf;
      vp->val_len = sizeof(float);
      asn_parse_float(var_val, &len, &vp->type,
                      vp->val.floatVal,
                      vp->val_len);
      break;
    case ASN_OPAQUE_DOUBLE:
      vp->val.doubleVal = (double *)vp->buf;
      vp->val_len = sizeof(double);
      asn_parse_double(var_val, &len, &vp->type,
                       vp->val.doubleVal,
                       vp->val_len);
      break;
    case ASN_OPAQUE_I64:
      vp->val.counter64 = (struct counter64 *)vp->buf;
      vp->val_len = sizeof(struct counter64);
      asn_parse_signed_int64(var_val, &len, &vp->type,
                             (struct counter64 *)vp->val.counter64,
                             sizeof(*vp->val.counter64));

      break;
#endif /* OPAQUE_SPECIAL_TYPES */
      case ASN_OCTET_STR:
      case ASN_IPADDRESS:
      case ASN_OPAQUE:
      case ASN_NSAP:
        if (vp->val_len < sizeof(vp->buf)){
          vp->val.string = (u_char *)vp->buf;
        } else {
          vp->val.string = (u_char *)malloc((unsigned)vp->val_len);
        }
        asn_parse_string(var_val, &len, &vp->type, vp->val.string,
                         &vp->val_len);
        break;
      case ASN_OBJECT_ID:
        vp->val_len = MAX_OID_LEN;
        asn_parse_objid(var_val, &len, &vp->type, objid, &vp->val_len);
        vp->val_len *= sizeof(oid);
        vp->val.objid = (oid *)malloc((unsigned)vp->val_len);
        memmove(vp->val.objid, objid, vp->val_len);
        break;
      case SNMP_NOSUCHOBJECT:
      case SNMP_NOSUCHINSTANCE:
      case SNMP_ENDOFMIBVIEW:
      case ASN_NULL:
        break;
      case ASN_BIT_STR:
        vp->val.bitstring = (u_char *)malloc(vp->val_len);
        asn_parse_bitstring(var_val, &len, &vp->type,
                            vp->val.bitstring, &vp->val_len);
        break;
      default:
        printf ("Error: bad type returned (%x)\n", vp->type);
        badtype = 1;
        break;
    }                                          
	print_variable ( vp->name, vp->name_length, vp) ;

	return badtype;
}

unsigned int encode_oid ( char *oid_string, unsigned char *out_buffer, unsigned int out_size  ) 
{
  oid var_name[MAX_OID_LEN];
  size_t name_len=MAX_OID_LEN;   
  unsigned char *data_ptr;

  if ( !read_objid(oid_string, var_name, &name_len) ) {
        if (!get_node(oid_string, var_name, &name_len) ) {
                printf("Can't find oid %s at line %d\n", oid_string, line);
                exit ( -37);
        }

  }     

  data_ptr = asn_build_objid(out_buffer, &out_size,
            (u_char)(ASN_UNIVERSAL | ASN_PRIMITIVE | ASN_OBJECT_ID),
            var_name, name_len);             

  return data_ptr-out_buffer;
}

unsigned int decode_wd ( unsigned char *data, unsigned int data_len) 
{
  oid this_oid[MAX_OID_LEN];
  size_t oid_len = MAX_OID_LEN;
  unsigned char type;
  void *retval; /* generic pointer, will only use to check return value */
  int len;

  len=data_len-1; /* The last char is the enable/disable switch */

  if((retval=asn_parse_objid (data, &len, &type, this_oid, &oid_len) )==NULL) {
	printf("OID.parse.error");
	} else { 
    		print_objid(this_oid, oid_len);

	}
	printf ("%d\n", (int) data[data_len-1]); 
return 1;
}
 
int
hex_to_binary(const char *str,
              u_char *bufp)
{
  int len, itmp;
  printf ("Hex string rx'd: %s\n", str);
  if (!bufp) return -1;
  if (*str && *str == '0' && (*(str+1) == 'x' || *(str+1) == 'X')) str += 2;
  for (len = 0; *str; str++) {
    if (isspace(*str)) continue;
    if (!isxdigit(*str)) return -1;
    len++;
    if (sscanf(str++, "%2x", &itmp) == 0) return -1;
    *bufp++ = itmp;
    if (!*str) return -1; /* odd number of chars is an error */
  }
  return len;
}

