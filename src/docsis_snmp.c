/*
 *  DOCSIS configuration file encoder.
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002,2003,2004,2005 Evvolve Media SRL,office@evvolve.com
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

#include "docsis_snmp.h"
#include "docsis_decode.h"

extern unsigned int line;	/* from a.l */

#define PACKET_LENGTH (8 * 1024)

unsigned int
encode_vbind (char *oid_string, char oid_asntype, union t_val *value,
	      unsigned char *out_buffer, size_t out_size)
{
  oid var_name[MAX_OID_LEN];
  size_t name_len = MAX_OID_LEN;
  oid oid_value[MAX_OID_LEN];
  size_t oid_value_len = MAX_OID_LEN;
  unsigned char *data_ptr;
  unsigned char buf[SPRINT_MAX_LEN];
  unsigned int ltmp;
  struct tree *tp;
  struct range_list *rp;
  long len=0;
  int rv;
  long longval;
  unsigned long ulongval;

  memset (buf, 0, SPRINT_MAX_LEN);
  if (!get_node (oid_string, var_name, &name_len))
    {
      if (!read_objid (oid_string, var_name, &name_len))
	{
	  /* enable random access */
	  if (!netsnmp_ds_get_boolean
	      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_RANDOM_ACCESS))
	    {
	      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
					 NETSNMP_DS_LIB_RANDOM_ACCESS);
	    }

	  if (!get_node (oid_string, var_name, &name_len))
	    {
	      /* Not found, try UCD style OID access */
	      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
					 NETSNMP_DS_LIB_READ_UCD_STYLE_OID);
	      if (!netsnmp_ds_get_boolean
		  (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_READ_UCD_STYLE_OID))
		{
		  netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
					     NETSNMP_DS_LIB_READ_UCD_STYLE_OID);
		}

	      if (!read_objid (oid_string, var_name, &name_len))
		{

		  /* Hmm, still not found, go back to net-snmp-style OID and try regex access */
		  netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
					     NETSNMP_DS_LIB_READ_UCD_STYLE_OID);

		  if (!netsnmp_ds_get_boolean
		      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_REGEX_ACCESS))
		    {
		      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
						 NETSNMP_DS_LIB_REGEX_ACCESS);
		    }

		  if (!get_wild_node (oid_string, var_name, &name_len))
		    {
		      /* Ran out of ideas. */
		      fprintf(stderr, "/* Error: Can't find oid %s at line %d */\n",
			      oid_string, line);
		      snmp_perror ("encode_vbind");
		      return 0;
		    }
		}
	    }
	}
    }

  switch (oid_asntype)
    {
    case 'i':
      longval = value->intval;
      data_ptr = _docsis_snmp_build_var_op (out_buffer,
				    var_name,
				    &name_len,
				    ASN_INTEGER,
				    sizeof (long),
				    (unsigned char *) &longval,
				    &out_size);
      return data_ptr - out_buffer;
      break;
      ;;
    case 'g':
      ulongval = value->intval;
      data_ptr = _docsis_snmp_build_var_op (out_buffer,
				    var_name,
				    &name_len,
				    ASN_GAUGE,
				    sizeof (unsigned long),
				    (unsigned char *) &ulongval,
				    &out_size);
      return data_ptr - out_buffer;
      break;
      ;;
    case 'c':
      ulongval = value->intval;
      data_ptr = _docsis_snmp_build_var_op (out_buffer,
				    var_name,
				    &name_len,
				    ASN_COUNTER,
				    sizeof (unsigned long),
				    (unsigned char *) &ulongval,
				    &out_size);
      return data_ptr - out_buffer;
      break;
      ;;
    case 'u':
      ulongval = value->intval;
      data_ptr = _docsis_snmp_build_var_op (out_buffer,
				    var_name,
				    &name_len,
				    ASN_GAUGE,
				    sizeof (unsigned long),
				    (unsigned char *) &ulongval,
				    &out_size);
      return data_ptr - out_buffer;
      break;
      ;;
    case 't':
      ulongval = value->intval;
      data_ptr = _docsis_snmp_build_var_op (out_buffer,
                                    var_name,
                                    &name_len,
                                    ASN_TIMETICKS,
                                    sizeof (unsigned long),
                                    (unsigned char *) &ulongval,
                                    &out_size);
      return data_ptr - out_buffer;
      break;
      ;;
    case 's':
    case 'x':
    case 'd':
      if (oid_asntype == 'd')
	{
	  /* ltmp = ascii_to_binary(value->strval, buf);
	     strncpy((char*)buf, value, SPRINT_MAX_BUF);
	     ltmp = strlen((char*)buf); */
	}
      else if (oid_asntype == 's')
	{
	  strncpy ((char *) buf, value->strval, SPRINT_MAX_LEN);
	  len = strlen ((char *) buf);
	}
      else if (oid_asntype == 'x')
	{
	  if ((rv = hexadecimal_to_binary (value->strval, buf)) == -1)
	    {
	      fprintf(stderr, "Invalid hexadecimal string at line %d\n", line);
	      return 0;
	    }
	  ltmp = (unsigned int) rv;
	  len = ltmp;
	}

      if (len < 0 || len > SPRINT_MAX_LEN - 1)
	{
	  fprintf(stderr, "String too long at line %d, max allowed %d\n", line,
		  SPRINT_MAX_LEN);
	  return 0;
	  break;
	}
      tp = get_tree (var_name, name_len, get_tree_head ());
      if (tp && tp->ranges
	  && !netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID,
				      NETSNMP_DS_LIB_DONT_CHECK_RANGE))
	{
	  rp = tp->ranges;
	  while (rp)
	    {
	      if (rp->low <= len && len <= rp->high)
		break;
	      rp = rp->next;
	    }
	  if (!rp)
	    {
	      fprintf(stderr, "Value too long at line %d\n", line);
	      return 0;
	      break;
	    }
	}
      /* If length is more than 127, won't fit into a 1-byte quantity */
      if (len+name_len+8 < 0x7f ) {
     		data_ptr = _docsis_snmp_build_var_op (out_buffer,
                                    var_name,
                                    &name_len,
                                    ASN_OCTET_STR,
                                    len, (unsigned char *) buf,
                                    &out_size);
      } else {
      		data_ptr = snmp_build_var_op (out_buffer,
				    var_name,
				    &name_len,
				    ASN_OCTET_STR,
				    len, (unsigned char *) buf,
				    &out_size);
      }
#ifdef DEBUG
      fprintf (stderr, "encoded len %ld var_len %d leftover %ud difference %d\n", len, name_len, out_size, (data_ptr - out_buffer) );
#endif
      return data_ptr - out_buffer;
      break;
      ;;
    case 'a':
      if (!inet_aton (value->strval, (struct in_addr *) &ltmp))
	{
	  fprintf(stderr, "Invalid IP address %s at line %d\n", value->strval, line);
	  return 0;
	}
      data_ptr = _docsis_snmp_build_var_op (out_buffer,
				    var_name,
				    &name_len,
				    ASN_IPADDRESS,
				    sizeof (struct in_addr),
				    (unsigned char *) &ltmp, &out_size);
      return data_ptr - out_buffer;
      break;
      ;;
    case 'o':

	strncpy ((char *) buf, value->strval, SPRINT_MAX_LEN);
	len = strlen ((char *) buf);

	if (!read_objid ((char *) buf, oid_value, &oid_value_len))
	  {
		fprintf(stderr, "Can't find oid %s at line %d\n", buf, line);
		return 0;
    	  }

	data_ptr = _docsis_snmp_build_var_op (out_buffer,
                                    var_name,
                                    &name_len,
                                    ASN_OBJECT_ID,
                                    oid_value_len*sizeof(oid),
                                    (unsigned char *) &oid_value, &out_size);
	return data_ptr - out_buffer;
	break;
	;;

    default:
      fprintf(stderr, "Variable type %s not supported yet\n", &oid_asntype);
      return 0;
      ;;
    }
/* NOTREACHED */
}


int
decode_vbind (unsigned char *data, unsigned int vb_len)
{
  unsigned char *var_val;
  oid var_name[MAX_OID_LEN];	/* To test the objid */
  size_t name_len = MAX_OID_LEN;	/* To test the objid */
  int badtype=0;
  size_t len;
  struct variable_list *vp;
  oid objid[MAX_OID_LEN];
  char _docsis_snmp_label[50];	/* To hold the 'name' of the type, i.e. Integer etc */
  char *enum_string = NULL;
  static char outbuf[1024];
  struct tree *subtree;
  struct enum_list *enums;

  memset (outbuf, 0, 1024);

  vp = (struct variable_list *) malloc (sizeof (struct variable_list));
  if (vp == NULL)
    {
      fprintf (stderr, "Out of memory\n");
      return 0;
    }
  memset (vp, 0, sizeof (struct variable_list));

  vp->next_variable = NULL;
  vp->val.string = NULL;
  vp->name_length = MAX_OID_LEN;
  vp->name = 0;
  data = snmp_parse_var_op (data, objid, &vp->name_length, &vp->type, &vp->val_len,
		       &var_val, (size_t *) & vb_len);

  if (data == NULL)
    return -1;
  if (snmp_set_var_objid (vp, objid, vp->name_length))
    return -1;

  len = PACKET_LENGTH;

  if (netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_EXTENDED_INDEX))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
                                 NETSNMP_DS_LIB_EXTENDED_INDEX);
    } /* Disable extended index format ... makes it harder to parse tokens in lex */
  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_NUMERIC_ENUM))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
                                 NETSNMP_DS_LIB_PRINT_NUMERIC_ENUM);
    } /* Enable printing numeric enums */
  if (netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_ESCAPE_QUOTES))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
                                 NETSNMP_DS_LIB_ESCAPE_QUOTES);
    } /* Disable escape quotes in string index output  */

  netsnmp_ds_set_int (NETSNMP_DS_LIBRARY_ID,
			      NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			      NETSNMP_OID_OUTPUT_SUFFIX);

  snprint_objid (outbuf, 1023, vp->name, vp->name_length);

  if (!get_node (outbuf, var_name, &name_len))
    {
      if (!read_objid (outbuf, var_name, &name_len))
	{
	  fprintf (stderr,
		   "/* Hmm ... can't find oid %s at line %d ... perhaps the MIBs are not installed ? */\n",
		   outbuf, line);
	  /* temporarily set full output format */
	  netsnmp_ds_set_int (NETSNMP_DS_LIBRARY_ID,
			      NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			      NETSNMP_OID_OUTPUT_FULL);
	  memset (outbuf, 0, 1024);
	  snprint_objid (outbuf, 1023, vp->name, vp->name_length);
	  /* Go back to suffix-mode for better readability */
	  netsnmp_ds_set_int (NETSNMP_DS_LIBRARY_ID,
			      NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			      NETSNMP_OID_OUTPUT_SUFFIX);
	}
    }

  printf("%s", outbuf);

/* save the subtree - we need it later to show enums */
  subtree = get_tree (var_name, name_len, get_tree_head() );

/* This first switch is just for saving the type in the format we actually want
   to print. */

  switch ((short) vp->type)
    {
    case ASN_INTEGER:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "Integer");
      break;
    case ASN_COUNTER:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "Counter32");
      break;
    case ASN_GAUGE:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "Gauge32");
      break;
    case ASN_TIMETICKS:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "TimeTicks");
      break;
    case ASN_UINTEGER:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "Unsigned32");
      break;
#ifdef OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_COUNTER64:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "OpaqueCounter64");
      break;
    case ASN_OPAQUE_U64:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "OpaqueU64");
      break;
#endif /* OPAQUE_SPECIAL_TYPES */
    case ASN_COUNTER64:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "Counter64");
      break;
#ifdef OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_FLOAT:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "OpaqueFloat");
      break;
    case ASN_OPAQUE_DOUBLE:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "OpaqueDouble");
      break;
    case ASN_OPAQUE_I64:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "OpaqueI64");
      break;
#endif /* OPAQUE_SPECIAL_TYPES */
    case ASN_OCTET_STR:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "String");
      break;
    case ASN_IPADDRESS:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "IPAddress");
      break;
    case ASN_OPAQUE:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "Opaque");
      break;
    case ASN_NSAP:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "NSAP");
      break;
    case ASN_OBJECT_ID:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "ObjectID");
      break;
    case ASN_BIT_STR:
      memset (_docsis_snmp_label, 0, 50);
      sprintf (_docsis_snmp_label, "BitString");
      break;
    }

  switch ((short) vp->type)
    {
    case ASN_INTEGER:

      vp->val.integer = (long *) vp->buf;
      vp->val_len = sizeof (long);
      asn_parse_int (var_val, &len, &vp->type,
		     (long *) vp->val.integer, sizeof (vp->val.integer));

      break;
    case ASN_COUNTER:
    case ASN_GAUGE:
    case ASN_TIMETICKS:
    case ASN_UINTEGER:
      vp->val.integer = (long *) vp->buf;
      vp->val_len = sizeof (u_long);
      asn_parse_unsigned_int (var_val, &len, &vp->type,
			      (u_long *) vp->val.integer,
			      sizeof (vp->val.integer));

      break;

#ifdef OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_COUNTER64:
    case ASN_OPAQUE_U64:
#endif /* OPAQUE_SPECIAL_TYPES */
    case ASN_COUNTER64:
      vp->val.counter64 = (struct counter64 *) vp->buf;
      vp->val_len = sizeof (struct counter64);
      asn_parse_unsigned_int64 (var_val, &len, &vp->type,
				(struct counter64 *) vp->val.counter64,
				sizeof (*vp->val.counter64));
      break;
#ifdef OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_FLOAT:
      vp->val.floatVal = (float *) vp->buf;
      vp->val_len = sizeof (float);
      asn_parse_float (var_val, &len, &vp->type,
		       vp->val.floatVal, vp->val_len);
      break;
    case ASN_OPAQUE_DOUBLE:
      vp->val.doubleVal = (double *) vp->buf;
      vp->val_len = sizeof (double);
      asn_parse_double (var_val, &len, &vp->type,
			vp->val.doubleVal, vp->val_len);
      break;
    case ASN_OPAQUE_I64:
      vp->val.counter64 = (struct counter64 *) vp->buf;
      vp->val_len = sizeof (struct counter64);
      asn_parse_signed_int64 (var_val, &len, &vp->type,
			      (struct counter64 *) vp->val.counter64,
			      sizeof (*vp->val.counter64));

      break;
#endif /* OPAQUE_SPECIAL_TYPES */
    case ASN_OCTET_STR:
    case ASN_IPADDRESS:
    case ASN_OPAQUE:
    case ASN_NSAP:
      if (vp->val_len < sizeof (vp->buf))
	{
	  vp->val.string = (u_char *) vp->buf;
	}
      else
	{
	  vp->val.string = (u_char *) malloc ((unsigned) vp->val_len+1);
	  memset(vp->val.string, 0, vp->val_len+1);
	}
      asn_parse_string (var_val, &len, &vp->type, vp->val.string,
			&vp->val_len);
      break;
    case ASN_OBJECT_ID:
      vp->val_len = MAX_OID_LEN;
      asn_parse_objid (var_val, &len, &vp->type, objid, &vp->val_len);
      vp->val_len *= sizeof (oid);
      vp->val.objid = (oid *) malloc ((unsigned) vp->val_len);
      memmove (vp->val.objid, objid, vp->val_len);
      break;
    case SNMP_NOSUCHOBJECT:
    case SNMP_NOSUCHINSTANCE:
    case SNMP_ENDOFMIBVIEW:
    case ASN_NULL:
      break;
    case ASN_BIT_STR:
      vp->val.bitstring = (u_char *) malloc (vp->val_len);
      asn_parse_bitstring (var_val, &len, &vp->type,
			   vp->val.bitstring, &vp->val_len);
      break;
    default:
      fprintf(stderr, "Error: bad type returned (%x)\n", vp->type);
      badtype = 1;
      break;
    }

  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_FULL_OID))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_PRINT_FULL_OID);
    }
  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_NUMERIC_OIDS))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_PRINT_NUMERIC_OIDS);
    }
  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_QUICK_PRINT))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_QUICK_PRINT);
    }

  if (!strcmp (_docsis_snmp_label, "String"))	/* Strings need special treatment - see below  */
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_QUICK_PRINT);
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_ESCAPE_QUOTES);
    }

  switch ((short) vp->type)
    {
    case ASN_OCTET_STR:
	if (str_isprint((char *) vp->val.string, vp->val_len))
		{
		 	snprintf(outbuf, vp->val_len+5, "\"%s\"", vp->val.string);
		} else {
			snprint_hexadecimal (outbuf, 1023, (char *) vp->val.string, vp->val_len);
      			memset (_docsis_snmp_label, 0, 50);
      			sprintf (_docsis_snmp_label, "HexString");
		}
	break;

    case ASN_BIT_STR:
		snprint_hexadecimal (outbuf, 1023, (char *) vp->val.bitstring, vp->val_len);
		break;
    case ASN_OBJECT_ID:
      		netsnmp_ds_set_int (NETSNMP_DS_LIBRARY_ID,
			  	NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			  	NETSNMP_OID_OUTPUT_NUMERIC);
		snprint_value (outbuf, 1023, vp->name, vp->name_length, vp);
      		netsnmp_ds_set_int (NETSNMP_DS_LIBRARY_ID,
				NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
				NETSNMP_OID_OUTPUT_SUFFIX);
		break;

    default:
	snprint_value (outbuf, 1023, vp->name, vp->name_length, vp);

    }

    if ( subtree )
    {
   	enums = subtree->enums;
	   	for (; enums; enums = enums->next) {
			if (enums->value == *vp->val.integer) {
			enum_string = enums->label;
       			break;
			}
		}
    }
  if (enum_string)
	printf(" %s %s; /* %s */", _docsis_snmp_label, outbuf, enum_string);
  else
	printf(" %s %s ;", _docsis_snmp_label, outbuf);


  snmp_free_var (vp);

  return badtype;
}

unsigned int
encode_snmp_oid (char *oid_string, unsigned char *out_buffer,
	    size_t out_size)
{
  oid var_name[MAX_OID_LEN];
  size_t name_len = MAX_OID_LEN;
  unsigned char *data_ptr;

  if (!read_objid (oid_string, var_name, &name_len))
    {
      if (!get_node (oid_string, var_name, &name_len))
	{
	  fprintf(stderr, "Can't find oid %s at line %d\n", oid_string, line);
	  return 0;
	}

    }

  data_ptr = asn_build_objid (out_buffer, &out_size,
			      (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
					ASN_OBJECT_ID), var_name, name_len);

  return data_ptr - out_buffer;
}

unsigned int
decode_snmp_oid (unsigned char *data, size_t data_len)
{
  oid this_oid[MAX_OID_LEN];
  size_t oid_len = MAX_OID_LEN;
  unsigned char type;
  void *retval;			/* generic pointer, will only use to check return value */
  size_t len;
  static char outbuf[1024];

  memset (outbuf, 0, 1024);

  len = data_len;		/* The last char is the enable/disable switch */

  if ((retval =
       asn_parse_objid (data, &len, &type, this_oid, &oid_len)) == NULL)
    {
      fprintf(stderr, "OID.parse.error");
      return 0;
    }
  else
    {
      netsnmp_ds_set_int (NETSNMP_DS_LIBRARY_ID,
			  NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
			  NETSNMP_OID_OUTPUT_NUMERIC);
      snprint_objid (outbuf, 1023, this_oid, oid_len);
    }
/*  fprintf(stderr, "%s %d", outbuf, (int) data[data_len - 1]); */
  printf("%s", outbuf);
  return 1;
}

/*
 * The following two functions differ from stock net-snmp functions by not supporting a long
 * type for the length of the SNMP VarBinds
 *
 * u_char * docsis_snmp_build_var_op(
 * u_char *data         IN - pointer to the beginning of the output buffer
 * oid *var_name        IN - object id of variable
 * size_t *var_name_len IN - length of object id
 * u_char var_val_type  IN - type of variable
 * size_t var_val_len   IN - length of variable
 * u_char *var_val      IN - value of variable
 * int *listlength      IN/OUT - number of valid bytes left in the output buffer
 */

u_char         *
_docsis_snmp_build_var_op(u_char * data,
                          oid * var_name,
                          size_t * var_name_len,
                          u_char var_val_type,
                          size_t var_val_len,
                          u_char * var_val,
                          size_t * listlength)
{
    size_t          dummyLen, headerLen;
    u_char	 *tmpDataPtr;
    u_char         *dataPtr;

    dummyLen = *listlength;
    dataPtr = data;
#if 0
    data = asn_build_sequence(data, &dummyLen,
                              (u_char) (ASN_SEQUENCE | ASN_CONSTRUCTOR),
                              0);
    if (data == NULL) {
        return NULL;
    }
#endif
    if (dummyLen < 2)
        return NULL;
    data += 2;
    dummyLen -= 2;

    headerLen = data - dataPtr;
    *listlength -= headerLen;
    DEBUGDUMPHEADER("send", "Name");
    tmpDataPtr = data;
    data = asn_build_objid(data, listlength,
                           (u_char) (ASN_UNIVERSAL | ASN_PRIMITIVE |
                                     ASN_OBJECT_ID), var_name,
                           *var_name_len);
    DEBUGINDENTLESS();
    if (data == NULL) {
        ERROR_MSG("Can't build OID for variable");
        return NULL;
    }
    DEBUGDUMPHEADER("send", "Value");
    switch (var_val_type) {
    case ASN_INTEGER:
        data = asn_build_int(data, listlength, var_val_type,
                             (long *) var_val, var_val_len);
        break;
    case ASN_GAUGE:
    case ASN_COUNTER:
    case ASN_TIMETICKS:
    case ASN_UINTEGER:
        data = asn_build_unsigned_int(data, listlength, var_val_type,
                                      (u_long *) var_val, var_val_len);
        break;
#ifdef OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_COUNTER64:
    case ASN_OPAQUE_U64:
#endif
    case ASN_COUNTER64:
        data = asn_build_unsigned_int64(data, listlength, var_val_type,
                                        (struct counter64 *) var_val,
                                        var_val_len);
        break;
    case ASN_OCTET_STR:
    case ASN_IPADDRESS:
    case ASN_OPAQUE:
    case ASN_NSAP:
    	tmpDataPtr = data;
        data = asn_build_string(data, listlength, var_val_type,
                                var_val, var_val_len);
        break;
    case ASN_OBJECT_ID:
        data = asn_build_objid(data, listlength, var_val_type,
                               (oid *) var_val, var_val_len / sizeof(oid));
        break;
    case ASN_NULL:
        data = asn_build_null(data, listlength, var_val_type);
        break;
    case ASN_BIT_STR:
        data = asn_build_bitstring(data, listlength, var_val_type,
                                   var_val, var_val_len);
        break;
    case SNMP_NOSUCHOBJECT:
    case SNMP_NOSUCHINSTANCE:
    case SNMP_ENDOFMIBVIEW:
        data = asn_build_null(data, listlength, var_val_type);
        break;
#ifdef OPAQUE_SPECIAL_TYPES
    case ASN_OPAQUE_FLOAT:
        data = asn_build_float(data, listlength, var_val_type,
                               (float *) var_val, var_val_len);
        break;
    case ASN_OPAQUE_DOUBLE:
        data = asn_build_double(data, listlength, var_val_type,
                                (double *) var_val, var_val_len);
        break;
    case ASN_OPAQUE_I64:
        data = asn_build_signed_int64(data, listlength, var_val_type,
                                      (struct counter64 *) var_val,
                                      var_val_len);
        break;
#endif                          /* OPAQUE_SPECIAL_TYPES */
    default:
        ERROR_MSG("wrong type");
        return NULL;
    }
    DEBUGINDENTLESS();
    if (data == NULL) {
        ERROR_MSG("Can't build value");
        return NULL;
    }
    dummyLen = (data - dataPtr) - headerLen;

    _docsis_asn_build_sequence(dataPtr, &dummyLen,
                       (u_char) (ASN_SEQUENCE | ASN_CONSTRUCTOR),
                       dummyLen);
    return data;
}



/*
 * _docsis_asn_build_sequence - builds an ASN header for a sequence with the ID and
 * length specified.
 *  On entry, datalength is input as the number of valid bytes following
 *   "data".  On exit, it is returned as the number of valid bytes
 *   in this object following the id and length.
 *
 *  This only works on data types < 30, i.e. no extension octets.
 *  The maximum length is 0xFF;
 *
 *  Returns a pointer to the first byte of the contents of this object.
 *  Returns NULL on any error.

 u_char * asn_build_sequence(
 u_char     *data         IN - pointer to start of object
 int        *datalength   IN/OUT - number of valid bytes left in buffer
 u_char      type         IN - asn type of object
 int         length       IN - length of object
 */
u_char         *
_docsis_asn_build_sequence(u_char * data,
                   size_t * datalength, u_char type, size_t length)
{
    if (length > 0x7f)  {
	fprintf(stderr, "Warning: string too long, may result in illegal encoding\n");
    }

    if (*datalength <2) {
        fprintf(stderr,  "SNMP encoding error (build sequence): length %d < 2: PUNT",
                (int) *datalength);
        return NULL;
    }
    *datalength -= 2;
    *data++ = type;
     if (*datalength >255) {
        fprintf(stderr,  "SNMP encoding error (_docsis_build sequence): length %d >255 : PUNT",
                (int) *datalength);
        return NULL;
    }
    *data++ = (u_char) (length & 0xFF);
    return data;
}
