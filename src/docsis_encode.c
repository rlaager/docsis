/*
 *  DOCSIS configuration file encoder.
 *  Copyright (c) 2001,2005 Cornel Ciocirlan, ctrl@users.sourceforge.net.
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

#include <netdb.h>

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <math.h>

#include "docsis_common.h"
#include "docsis_encode.h"
#include "docsis_snmp.h"
#include "ethermac.h"

extern unsigned int line; 	/* defined in docsis_lex.l */


int encode_uint ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  unsigned int int_value;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */
  if ( buf == NULL ) {
	fprintf(stderr, "encode_uint called w/NULL buffer\n");
	exit (-1);
  }

  if ( tval == NULL  ) {
	fprintf(stderr, "encode_uint called w/NULL value struct\n");
	exit (-1);
  }
  helper = (union t_val *) tval;
  if ( sym_ptr->low_limit || sym_ptr->high_limit ) {
	if ( helper->uintval < sym_ptr->low_limit || helper->uintval > sym_ptr->high_limit ) {
		fprintf(stderr, "docsis: at line %d, %s value %d out of range %hd-%hd\n ", line,sym_ptr->sym_ident,helper->uintval,sym_ptr->low_limit, sym_ptr->high_limit);
		exit(-15);
	}
  }
  int_value  = htonl( helper->uintval );
#ifdef DEBUG
  fprintf(stderr, "encode_uint: found %s value %d\n",sym_ptr->sym_ident, helper->uintval);
#endif /* DEBUG */
  memcpy ( buf,&int_value, sizeof(unsigned int));
  return ( sizeof(unsigned int));
}

int encode_uint24 ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  unsigned char byte1, byte2, byte3;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  helper = (union t_val *) tval;

  byte1 = (helper->uintval >> (0)) & 0xFF;
  byte2 = (helper->uintval >> (8)) & 0xFF;
  byte3 = (helper->uintval >> (16)) & 0xFF;

  memcpy ( buf,&byte3, sizeof(char));
  memcpy ( buf+sizeof(char),&byte2, sizeof(char));
  memcpy ( buf+2*sizeof(char),&byte1, sizeof(char));
  return ( 3 * sizeof(char));
}

int encode_ushort ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  unsigned short sint;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */
  if ( buf == NULL ) {
        fprintf(stderr, "encode_ushort called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_ushort called w/NULL value struct\n");
        exit (-1);
  }
  helper = (union t_val *) tval;
  if ( sym_ptr->low_limit || sym_ptr->high_limit ) {
	if ( helper->uintval < sym_ptr->low_limit || helper->uintval > sym_ptr->high_limit ) {
		fprintf(stderr, "docsis: at line %d, %s value %d out of range %hd-%hd\n ", line,sym_ptr->sym_ident,helper->uintval,sym_ptr->low_limit, sym_ptr->high_limit);
		exit(-15);
	}
  }
  sint = htons( (unsigned short) helper->uintval );
#ifdef DEBUG
  fprintf(stderr, "encode_ushort: found %s value %hd\n",sym_ptr->sym_ident, helper->uintval);
#endif /* DEBUG */
  memcpy ( buf,&sint,sizeof(unsigned short));
  return ( sizeof(unsigned short));
}

int encode_uchar ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  unsigned int int_value;
  char *cp;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */
  if ( buf == NULL ) {
        fprintf(stderr, "encode_uchar called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_uchar called w/NULL value struct\n");
        exit (-1);
  }

  helper = (union t_val *) tval;
  int_value = htonl( helper->uintval );

  if ( sym_ptr->low_limit || sym_ptr->high_limit ) {
	if ( helper->uintval < sym_ptr->low_limit || helper->uintval > sym_ptr->high_limit ) {
		fprintf(stderr, "docsis: at line %d, %s value %d out of range %hd-%hd\n ", line,sym_ptr->sym_ident,helper->uintval,sym_ptr->low_limit, sym_ptr->high_limit);
		exit(-15);
	}
  }
  cp = (char *)&int_value;
  buf[0]=(unsigned char)cp[3];
#ifdef DEBUG
  fprintf(stderr, "encode_uchar: found %s value %hd\n",sym_ptr->sym_ident, helper->uintval);
#endif

  return ( sizeof(unsigned char));
}


int encode_ip( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  struct in_addr in;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  if ( buf == NULL ) {
        fprintf(stderr, "encode_ip called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_ip called w/NULL value struct\n");
        exit (-1);
  }

  helper = (union t_val *) tval;

  if ( !inet_aton ( helper->strval, &in) ) {
	fprintf(stderr,  "Invalid IP address %s at line %d", helper->strval, line );
	exit (-1);
  }
#ifdef DEBUG
  fprintf(stderr, "encode_ip: found %s at line %d\n",inet_ntoa(in), line);
#endif /* DEBUG */
  memcpy ( buf, &in, sizeof(struct in_addr));
  free(helper->strval);
  return ( sizeof(struct in_addr));
}

int encode_ip_list( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  int i;
  char *token;
  char *array[16];
  const char s[2] = ",";
  struct in_addr in;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  helper = (union t_val *) tval;
  i = 0;
  token = strtok(helper->strval, s);
  while (token != NULL)
  {
    array[i] = token;
    token = strtok (NULL, s);
    if ( inet_aton ( array[i], &in) ) {
      memcpy ( buf + 4 * i, &in, sizeof(struct in_addr));
    } else {
      fprintf(stderr, "Invalid IP address %s at line %d\n", helper->strval, line );
      exit (-1);
    }
    i++;
  }
  free(helper->strval);
  return ( i * sizeof(struct in_addr));
}

int encode_ip6( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
#ifdef DEBUG
  char ip[INET6_ADDRSTRLEN];
#endif /*DEBUG */
  struct in6_addr in;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  if ( buf == NULL ) {
        fprintf(stderr, "encode_ip called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_ip called w/NULL value struct\n");
        exit (-1);
  }

  helper = (union t_val *) tval;

  if ( !inet_pton(AF_INET6, helper->strval, &in) ) {
	fprintf(stderr,  "Invalid IP address %s at line %d\n", helper->strval, line );
	exit (-1);
  }
#ifdef DEBUG
  fprintf(stderr, "encode_ip: found %s at line %d\n", inet_ntop(AF_INET6, &in, ip, sizeof(ip)), line);
#endif /* DEBUG */
  memcpy ( buf, &in, sizeof(struct in6_addr));
  free(helper->strval);
  return ( sizeof(struct in6_addr));
}

int encode_ip6_list( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  int i;
  char *token;
  char *array[16];
  const char s[2] = ",";
  struct in6_addr in6;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  helper = (union t_val *) tval;
  i = 0;
  token = strtok(helper->strval, s);
  while (token != NULL)
  {
    array[i] = token;
    token = strtok (NULL, s);
    if ( inet_pton ( AF_INET6, array[i], &in6) ) {
      memcpy ( buf + 16 * i, &in6, sizeof(struct in6_addr));
    } else {
      fprintf(stderr, "Invalid IP address %s at line %d\n", helper->strval, line );
      exit (-1);
    }
    i++;
  }
  free(helper->strval);
  return ( i * sizeof(struct in6_addr));
}

int encode_ip6_prefix_list( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  char final;
  char *str1, *str2, *token, *subtoken;
  char *saveptr1, *saveptr2;
  int j;
  struct in6_addr in6;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  helper = (union t_val *) tval;

  for (j = 0, str1 = helper->strval; ; j++, str1 = NULL) {
    token = strtok_r(str1, ",", &saveptr1);
    if (token == NULL)
      break;
    for (str2 = token; ; str2 = NULL) {
      subtoken = strtok_r(str2, "/", &saveptr2);
      if (subtoken == NULL)
        break;
      if ( inet_pton ( AF_INET6, subtoken, &in6) ) {
        memcpy ( buf + 17 * j, &in6, sizeof(struct in6_addr));
      } else {
        final = (char)atoi(subtoken);
        memcpy ( buf + 17 * j + sizeof(struct in6_addr),&final,sizeof(unsigned char));
      }
    }
  }

  free(helper->strval);
  return ( j * ( sizeof(struct in6_addr) + sizeof(unsigned char) ) );
}

int encode_ip_ip6( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  struct in6_addr in6;
  struct in_addr in;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  helper = (union t_val *) tval;

  if ( inet_pton(AF_INET6, helper->strval, &in6) ) {
    memcpy ( buf, &in6, sizeof(struct in6_addr));
    free(helper->strval);
    return ( sizeof(struct in6_addr));
  } else if ( inet_aton ( helper->strval, &in) ) {
    memcpy ( buf, &in, sizeof(struct in_addr));
    free(helper->strval);
    return ( sizeof(struct in_addr));
  } else {
    fprintf(stderr, "Invalid IP address %s at line %d\n", helper->strval, line );
    exit (-1);
  }
}

int encode_char_ip_ip6( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  struct in6_addr in6;
  struct in_addr in;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  char ipv4 = 1;
  char ipv6 = 2;

  helper = (union t_val *) tval;

  if ( inet_pton(AF_INET6, helper->strval, &in6) ) {
    memcpy ( buf, &ipv6, sizeof(char) );
    memcpy ( buf + 1, &in6, sizeof(struct in6_addr));
    free(helper->strval);
    return ( sizeof(char) + sizeof(struct in6_addr));
  } else if ( inet_aton ( helper->strval, &in) ) {
    memcpy ( buf, &ipv4, sizeof(char) );
    memcpy ( buf + 1, &in, sizeof(struct in_addr));
    free(helper->strval);
    return ( sizeof(char) + sizeof(struct in_addr));
  } else {
    fprintf(stderr, "Invalid IP address %s at line %d\n", helper->strval, line );
    exit (-1);
  }
}

int encode_ip_ip6_port( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  struct in6_addr in6;
  struct in_addr in;
  int i;
  short int port;
  char *token;
  char *array[2];
  const char s[2] = "/";

  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  helper = (union t_val *) tval;

  i = 0;
  token = strtok(helper->strval, s);
  while (token != NULL)
  {
    array[i++] = token;
    token = strtok (NULL, s);
  }
  port = htons(atoi(array[1]));

  if ( inet_pton(AF_INET6, array[0], &in6) ) {
    memcpy ( buf, &in6, sizeof(struct in6_addr));
    memcpy ( buf + sizeof(struct in6_addr),&port,sizeof(unsigned short));
    free(helper->strval);
    return ( sizeof(struct in6_addr) + sizeof(unsigned short));
  } else if ( inet_aton ( array[0], &in) ) {
    memcpy ( buf, &in, sizeof(struct in_addr));
    memcpy ( buf + sizeof(struct in_addr),&port,sizeof(unsigned short));
    free(helper->strval);
    return ( sizeof(struct in_addr) + sizeof(unsigned short));
  } else {
    fprintf(stderr, "Invalid IP address / port combination %s at line %d\n", helper->strval, line );
    exit (-1);
  }
}

int encode_lenzero( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  return (0);
}

int encode_ether ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
int retval; 	     /* return value of inet_aton */
union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  if ( buf == NULL ) {
        fprintf(stderr, "encode_ether called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_ether called w/NULL value struct\n");
        exit (-1);
  }

  helper = (union t_val *) tval;

  if (!(retval = ether_aton ( helper->strval, buf)) ) {
	fprintf(stderr, "Invalid MAC address %s at line %d", helper->strval, line );
	exit (-1);
  }
#ifdef DEBUG
  fprintf(stderr, "encode_ether: found %s at line %d\n", ether_ntoa(buf), line);
#endif  /* DEBUG */
  free(helper->strval);
  return retval;  /* hopefully this equals 6 :) */
}

int encode_dual_qtag ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  int i, final;
  char *token;
  char *array[2];
  const char s[2] = ",";
  union t_val *helper;

  helper = (union t_val *) tval;
  i = 0;
  token = strtok(helper->strval, s);
  while (token != NULL)
  {
     array[i++] = token;
     token = strtok (NULL, s);
  }
  final = htonl(atoi(array[0]) << 16 | atoi(array[1]));
  memcpy (buf, &final, sizeof(final));
  free(helper->strval);
  return (sizeof(final));
}

int encode_char_list ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
    short int i;
    char *token, final;
    const char s[2] = ",";
    union t_val *helper;

    helper = (union t_val *) tval;
    i = 0;
    token = strtok(helper->strval, s);
    while (token != NULL)
    {
        final = (char)atoi(token);
        memcpy (buf + i, &final, sizeof(char));
        token = strtok (NULL, s);
        i++;
    }
    free(helper->strval);
    return(i * sizeof(char));
}

int encode_ethermask ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
int reta, retb; 	     /* return value of ether_aton */
char *ether,*mask;
union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  if ( buf == NULL ) {
        fprintf(stderr, "encode_ethermask called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_ethermask called w/NULL value struct\n");
        exit (-1);
  }

  helper = (union t_val *) tval;
  ether = helper -> strval;

  mask = strchr ( ether, (int) '/');
  if (mask == NULL) {
	fprintf(stderr, "encode_ethermask: at line %d, format should be <mac_address>/<mac_mask>\n", line);
 	exit (-1);
  }
  mask[0]=0x0; mask++; /* cut the string in two */

  if (!(reta = ether_aton ( ether, buf)) ) {
	fprintf(stderr, "Invalid MAC address %s at line %d\n", ether, line );
	exit (-1);
  }
  if (!(retb = ether_aton ( mask, buf+reta*(sizeof(char))) ) ) {
	fprintf(stderr, "Invalid MAC address %s at line %d\n", mask, line );
	exit (-1);
  }

#ifdef DEBUG
  fprintf(stderr, "encode_ethermask: found %s/%s at line %d\n", ether_ntoa(buf), ether_ntoa(buf+reta*sizeof(char)), line);
#endif  /* DEBUG */
  free(helper->strval);
  return (reta+retb);  /* hopefully this equals 12 :) */
}

int encode_string(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  unsigned int string_size;
  /* We only use this to cast the void* we receive to what we think it should be */
  union t_val *helper;

  if ( buf == NULL ) {
        fprintf(stderr, "encode_string called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_string called w/NULL value struct\n");
        exit (-1);
  }
  helper = (union t_val *) tval;
  string_size = strlen ( helper->strval );
  if (sym_ptr->low_limit || sym_ptr->high_limit) {
	if ( string_size < sym_ptr->low_limit ) {
		fprintf(stderr, "encode_string: String too short, must be min %d chars\n",
							sym_ptr->low_limit);
		exit(-1);
	}
	if ( sym_ptr->high_limit < string_size ) {
		fprintf(stderr, "encode_string: String too long (%d chars), must be max %d chars\n",
							string_size, sym_ptr->high_limit);
		exit(-1);
	}
  }

#ifdef DEBUG
  fprintf(stderr, "encode_string: found '%s' on line %d\n", helper->strval, line );
#endif /* DEBUG */
  memset(buf,0,string_size+1);
  memcpy ( buf, helper->strval, string_size);
 /* No need to free strings because we use a static buffer to parse them */
  return ( string_size );
}

/* This is for strings which need the terminating 0 at the end, e.g. Service Flow Class Name */

int encode_strzero(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  unsigned int string_size;
  /* We only use this to cast the void* we receive to what we think it should be */
  union t_val *helper;

  if ( buf == NULL ) {
        fprintf(stderr, "encode_string called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_string called w/NULL value struct\n");
        exit (-1);
  }
  helper = (union t_val *) tval;
  string_size = strlen ( helper->strval );
  if (sym_ptr->low_limit || sym_ptr->high_limit) {
        if ( string_size < sym_ptr->low_limit ) {
                fprintf(stderr, "encode_string: String too short, must be min %d chars\n",
                                                        sym_ptr->low_limit);
                exit(-1);
        }
        if ( sym_ptr->high_limit < string_size ) {
                fprintf(stderr, "encode_string: String too long, must be max %d chars\n",
                                                        sym_ptr->high_limit);
                exit(-1);
        }
  }

#ifdef DEBUG
  fprintf(stderr, "encode_string: found '%s' on line %d\n", helper->strval, line );
#endif /* DEBUG */
  memset(buf,0,string_size+1);
  memcpy ( buf, helper->strval, string_size);
 /* No need to free strings because we use a static buffer to parse them */
  return ( string_size+1 );
}

int encode_hexstr (unsigned char *buf, void *tval, struct symbol_entry *sym_ptr)
{
  unsigned int fragval;
  unsigned int i;
  int rval;
  char *p;
  unsigned int string_size;
  /* We only use this to cast the void * we receive  and extract the data from the union */
  union t_val *helper;

  if ( buf == NULL ) {
        fprintf(stderr, "encode_hexstr called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_hexstr called w/NULL value struct\n");
        exit (-1);
  }
  helper = (union t_val *) tval;
  string_size = strlen ( helper->strval );

  if ( string_size % 2 != 0 ) {
        fprintf(stderr, "encode_hexstr: invalid hex string\n");
        exit (-1);
  }

  p = helper->strval;

  if (p[0] != '0' || ( p[1] != 'x' && p[1] != 'X' )) {
	fprintf(stderr, "encode_hexstr: invalid hex string %s\n", p);
	exit (-1);
  }

  p += 2*sizeof(char);

  i=0;

  while (*p) {
	if ( (rval=sscanf(p, "%02x", &fragval)) == 0) {
		fprintf(stderr, "Invalid Hex Value %s\n", helper->strval);
		exit (-1);
	}
 	buf[i] = (char) fragval; i++;
	p += 2*sizeof(char);
  }

  if (sym_ptr->low_limit || sym_ptr->high_limit) {
	if (  i < sym_ptr->low_limit ) {
		fprintf(stderr, "encode_hexstr: Hex value too short, must be min %d octets\n",
							sym_ptr->low_limit);
		exit(-1);
	}
	if ( sym_ptr->high_limit < i ) {
		fprintf(stderr, "encode_hexstr: Hex value too long, must be max %d octets\n",
							sym_ptr->high_limit);
		exit(-1);
	}
  }
#ifdef DEBUG
  fprintf(stderr, "encode_hexstr: found '%s' on line %d\n", helper->strval, line );
#endif /* DEBUG */
  free(helper->strval);
  /* TODO Fix bug added by free(helper->strval) when double quote is used in text config file. */
  return ( i );
}

/* This is for strings which need the terminating 0 at the end, e.g. Service Flow Class Name */

int encode_oid(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  unsigned int output_size;
  /* We only use this to cast the void* we receive to what we think it should be */
  union t_val *helper;

  if ( buf == NULL ) {
        fprintf(stderr, "encode_oid called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_oid called w/NULL value struct\n");
        exit (-1);
  }

  helper = (union t_val *) tval;

  output_size = encode_snmp_oid(helper->strval, buf, TLV_VSIZE);
  free(helper->strval);
  return ( output_size );
}


int encode_ushort_list( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
unsigned short numbers[128];
unsigned long value_found;
unsigned int nr_found=0;
char *cp;
char *endptr[1];
#ifdef DEBUG
unsigned int i;
#endif

union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  if ( buf == NULL ) {
        fprintf(stderr, "encode_ushort_list called w/NULL buffer\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        fprintf(stderr, "encode_ushort_list called w/NULL value struct\n");
        exit (-1);
  }

  helper = (union t_val *) tval;

  cp = helper->strval;

  do {
	if(*cp ==',' || *cp == ' ') cp++;

	value_found = strtoul( cp, endptr, 10);

	if (endptr == NULL)

	if (cp == *endptr) {
		fprintf(stderr, "Parse error at line %d: expecting digits\n",line);
		exit (-11);
	}
	if (value_found > 65535) {
		fprintf(stderr, "Parse error at line %d: value cannot exceed 65535\n",line);
		exit (-11);
	}
	nr_found++;
	numbers[nr_found-1]=htons((unsigned short) value_found);

	cp=*endptr;

  } while (*cp);

  if (sym_ptr->low_limit || sym_ptr->high_limit) {
        if (  nr_found < sym_ptr->low_limit ) {
                fprintf(stderr, "Line %d: Not enough numbers, minimum %d\n", line,
                                                        sym_ptr->low_limit);
                exit(-1);
        }
        if ( sym_ptr->high_limit < nr_found ) {
                fprintf(stderr, "Line %d: too many numbers, max %d\n", line,
                                                        sym_ptr->high_limit);
                exit(-1);
        }
  }

#ifdef DEBUG
  fprintf(stderr, "encode_ushort_list: found ");
  for(i=0; i<nr_found; i++)
	fprintf(stderr, "%d ", ntohs(numbers[i]) );
  fprintf(stderr, "\n");

#endif /* DEBUG */
  memcpy ( buf, numbers, nr_found*sizeof(unsigned short));
  free(helper->strval);
  return ( nr_found*sizeof(unsigned short));
}

int encode_nothing(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
return 0;
}
