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
#include "docsis_globals.h"
#include "ethermac.h"

extern unsigned int line; 	/* defined in a.l */
extern symbol_type *symtable; 	/* defined in a_symtable.h,included by a_yy.y */


int get_uint ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr ) 
{ 
  unsigned int int_value;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */
  if ( buf == NULL ) { 
	printf ("get_uint called w/NULL buffer!\n");
	exit (-1);
  }

  if ( tval == NULL  ) { 
	printf ("get_uint called w/NULL value struct !\n");
	exit (-1);
  }
  helper = (union t_val *) tval;
  if ( sym_ptr->low_limit || sym_ptr->high_limit ) { 
	if ( helper->uintval < sym_ptr->low_limit || helper->uintval > sym_ptr->high_limit ) {
		printf ("%s: at line %d, %s value %d out of range %hd-%hd\n ", prog_name,line,sym_ptr->sym_ident,helper->uintval,sym_ptr->low_limit, sym_ptr->high_limit); 
		exit(-15);
	}
  }
  int_value = htonl( helper->uintval );
#ifdef DEBUG
  printf ("get_uint: found %s value %hd\n",sym_ptr->sym_ident, helper->uintval);
#endif /* DEBUG */
  memcpy ( buf,&int_value, sizeof(unsigned int));
  return ( sizeof(unsigned int));
}


int get_short ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  unsigned short sint;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */
  if ( buf == NULL ) {
        printf ("get_short called w/NULL buffer!\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        printf ("get_short called w/NULL value struct !\n");
        exit (-1);
  }
  helper = (union t_val *) tval;
  if ( sym_ptr->low_limit || sym_ptr->high_limit ) { 
	if ( helper->uintval < sym_ptr->low_limit || helper->uintval > sym_ptr->high_limit ) {
		printf ("%s: at line %d, %s value %d out of range %hd-%hd\n ", prog_name,line,sym_ptr->sym_ident,helper->uintval,sym_ptr->low_limit, sym_ptr->high_limit); 
		exit(-15);
	}
  }
  sint = htons( (unsigned short) helper->uintval );
#ifdef DEBUG
  printf ("get_short: found %s value %hd\n",sym_ptr->sym_ident, helper->uintval);
#endif /* DEBUG */
  memcpy ( buf,&sint,sizeof(unsigned short));
  return ( sizeof(unsigned short));
}                      

int get_uchar ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr )
{
  unsigned int int_value;
  char *cp;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */
  if ( buf == NULL ) {
        printf ("get_uchar called w/NULL buffer!\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        printf ("get_uchar called w/NULL value struct !\n");
        exit (-1);
  }

  helper = (union t_val *) tval;
  int_value = htonl( helper->uintval );

  if ( sym_ptr->low_limit || sym_ptr->high_limit ) { 
	if ( helper->uintval < sym_ptr->low_limit || helper->uintval > sym_ptr->high_limit ) {
		printf ("%s: at line %d, %s value %d out of range %hd-%hd\n ", prog_name,line,sym_ptr->sym_ident,helper->uintval,sym_ptr->low_limit, sym_ptr->high_limit); 
		exit(-15);
	}
  }
  cp = (char *)&int_value;
  buf[0]=(unsigned char)cp[3];
#ifdef DEBUG
  printf ("get_uchar: found %s value %hd\n",sym_ptr->sym_ident, helper->uintval);
#endif 
		
  return ( sizeof(unsigned char));
}             


int get_ip( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr ) 
{
struct in_addr in;
int retval; 	     /* return value of inet_aton */
union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  if ( buf == NULL ) {
        printf ("get_ip called w/NULL buffer!\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        printf ("get_ip called w/NULL value struct !\n");
        exit (-1);
  }

  helper = (union t_val *) tval;

  if (!(retval = inet_aton ( helper->strval, &in)) ) { 
	printf ( "Invalid IP address %s at line %d", helper->strval, line );
	exit (-1);
  }

  printf ("get_ip: found %s at line %d\n",inet_ntoa(in), line);
  memcpy ( buf, &in, sizeof(struct in_addr));
  return ( sizeof(struct in_addr));      
}

int get_mac ( unsigned char *buf, void *tval, struct symbol_entry *sym_ptr ) 
{
int retval; 	     /* return value of inet_aton */
union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  if ( buf == NULL ) {
        printf ("get_mac called w/NULL buffer!\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        printf ("get_mac called w/NULL value struct !\n");
        exit (-1);
  }

  helper = (union t_val *) tval;

  if (!(retval = ether_aton ( helper->strval, buf)) ) { 
	printf ( "Invalid MAC address %s at line %d", helper->strval, line );
	exit (-1);
  }
#ifdef DEBUG
  printf ("get_mac: found %s at line %d\n", ether_ntoa(buf), line);
#endif  /* DEBUG */
  return retval;  /* hopefully this equals 6 :) */     
}

int get_string(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr ) 
{ 

  unsigned int string_size;
  union t_val *helper; /* We only use this to cast the void* we receive to what we think it should be */

  if ( buf == NULL ) {
        printf ("get_string called w/NULL buffer!\n");
        exit (-1);
  }

  if ( tval == NULL  ) {
        printf ("get_string called w/NULL value struct !\n");
        exit (-1);
  }
  helper = (union t_val *) tval;
  string_size = strlen ( helper->strval );

  printf ("get_string: found '%s' on line %d\n", helper->strval, line );
  memset(buf,0,string_size+1);
  memcpy ( buf, helper->strval, string_size);
  return ( string_size );  

}
 
int get_nothing(unsigned char *buf, void *tval, struct symbol_entry *sym_ptr ) {
return 0;
}

unsigned int add_cm_mic ( unsigned char *tlvbuf, unsigned int tlvbuflen ) 
{
  MD5_CTX mdContext; 
  
  MD5Init(&mdContext );
  MD5Update(&mdContext, tlvbuf, tlvbuflen );
  MD5Final(&mdContext);
  tlvbuf[tlvbuflen]= 6;
  tlvbuf[tlvbuflen+1]=16;
  memcpy ( tlvbuf+tlvbuflen+2, &(mdContext.digest), 16);
  return (tlvbuflen+18); /* we added the CM Message Integrity Check  */
}

unsigned int add_eod_and_pad ( unsigned char *tlvbuf, unsigned int tlvbuflen ) 
{
  int nr_pads;
  tlvbuf[tlvbuflen]=255;
  tlvbuflen = tlvbuflen +1;
  nr_pads = 4 - (tlvbuflen-4 * ((unsigned int) floor ( (double) (tlvbuflen/4)))); 
  if ( nr_pads < 0 ) { 
	printf ( "Illegal number of pads\n");
	exit (-1);
  }
  memset(&tlvbuf[tlvbuflen], 0, nr_pads );

  return (tlvbuflen+nr_pads); 
}


unsigned int add_cmts_mic (unsigned char *tlvbuf, unsigned int tlvbuflen, unsigned char *key, int keylen ) 
{
  int i; 
  register unsigned char *cp,*dp;

/* Only these configuration tlv's must be used to calculate the CMTS MIC */
  unsigned char digest_order[10] = { 1,2,3,4,17,43,6,18,19,20 }; 

  unsigned char *cmts_tlvs;
  unsigned char digest[17];

  cmts_tlvs = (unsigned char *) malloc ( tlvbuflen+1 );
  dp = cmts_tlvs;
  for (i=0; i<10; i++ ) { 
	cp = tlvbuf;
	while ( (unsigned int) (cp - tlvbuf) < tlvbuflen ) {
		if ( cp[0]==digest_order[i] ) {
			memcpy ( dp, cp, cp[1]+2 );
			dp = dp + cp[1] +2;
			cp = cp + cp[1] +2;
		} else {
			cp = cp + cp[1] +2;
		}
	}
  }
  printf (" TLVs for CMTS MIC:\n");
  decode_tlvbuf ( cmts_tlvs, dp-cmts_tlvs, 0);
  printf ("End TLVs for CMTS MIC.\n");
  hmac_md5 ( cmts_tlvs, dp-cmts_tlvs, key, keylen, digest) ; 
  md5_print_digest(digest);
  tlvbuf[tlvbuflen]=7; 		/* CMTS MIC */
  tlvbuf[tlvbuflen+1]=16; 	/* length of MD5 digest */
  memcpy ( &tlvbuf[tlvbuflen+2], digest, 16);
  return (tlvbuflen+18);
}

void usage(char *prog_name ) 
{
	printf( "DOCSIS Configuration File creator, version %s.%s\n", VERSION,PATCHLEVEL); 
	printf ("Copyright (c) 2000 Cornel Ciocirlan, ctrl@users.sourceforge.net\n");
	printf( "Usage: \n\t %s <modem_cfg_file> <key_file> <output_file>\n",prog_name);
	printf ( "\nWhere:\n<modem_cfg_file>\t= name of the text (human readable) configuration file\n<key_file>\t\t= text file containing the authentication key to be used\n\t\t\t  for the CMTS MIC (Message Integrity Check).\n<output_file> \t\t= name of the output file where you want the binary data\n\t\t\t  to be written to (if it doesn't exist it is created).\n\t\t\t  This file can be TFTP-downloaded by DOCSIS-compliant\n\t\t\t  cable modems\n"); 
	exit ( -10 );
}


int decode_stdin(unsigned char *key, int keylen) {
unsigned char buf[1024];
int len;
FILE *in;
unsigned char *cp;

in = fopen ( "gi1.md5", "r");
memset ( buf, 0, 1024);
len=fread ( buf, sizeof(unsigned char), 1023, in);
cp = buf;
decode_tlvbuf (buf, len, 0);
/*
while ( cp -buf <len ) { 
	if (cp[0]==7)  {
		add_cmts_mic ( buf,cp-buf,key,keylen); 
	} else { 
		cp = cp + 2 + cp[1];
	}
} */

decode_tlvbuf (buf, len, 0);
exit(0);
}



int main(int argc,char *argv[] ) 
{

	unsigned char key[65];
	FILE *cf,*kf,*of;
	unsigned char *buffer;
	unsigned int buflen=0,keylen=0;
	int i;

	init_snmp("snmpapp");

   	if (! ds_get_boolean (DS_LIBRARY_ID,DS_LIB_PRINT_NUMERIC_OIDS)) {
        	ds_toggle_boolean(DS_LIBRARY_ID, DS_LIB_PRINT_NUMERIC_OIDS);
   	}
   	if (! ds_get_boolean (DS_LIBRARY_ID,DS_LIB_PRINT_FULL_OID)) {
       		ds_toggle_boolean(DS_LIBRARY_ID, DS_LIB_PRINT_FULL_OID);
   	}

	memset (prog_name,0,255);
	strncpy ( prog_name, argv[0], 254);
	if ( argc != 4 ) { 
		usage(prog_name);
	} 

	if ( (cf = fopen ( argv[1],"r" ))== NULL ) { 
		printf ("%s: Can't open config file %s\n",argv[0],argv[1]);
		exit(-5);
	}
	if ( (kf = fopen ( argv[2],"r" ))== NULL ) { 
		printf ("%s: Can't open keyfile %s\n",argv[0],argv[2]);
		exit(-5);
	}
        
        keylen = fread (key,sizeof(unsigned char), 64, kf);
	if (keylen <= 0) {
		printf ("%s: key must be at least 1 char long\n",argv[0]);
	}
	if (key[keylen-1] == 10 ) { 
		keylen--; /* eliminate trailing \n */
	}
        
	/* decode_stdin(key,keylen); */
	
	parse_input_file(cf);
  	/* decode_tlvlist(global_tlvlist, 0); */
	if ( global_tlvlist == NULL ) { 
		printf ( "Error parsing config file %s\n", argv[1]);
		exit ( 70 );
	}
	for ( i=0; i<global_tlvlist->tlv_count; i++ ) {
		buflen+=2; /* docs_code and len */
	 	buflen+=global_tlvlist->tlvlist[i]->tlv_len;
	}
	buflen+=255;

	buffer = (unsigned char *) malloc ( buflen );

	buflen = flatten_tlvlist(buffer, global_tlvlist);
	decode_tlvbuf ( buffer, buflen, 0);
	buflen = add_cm_mic ( buffer, buflen );
	decode_tlvbuf ( buffer, buflen, 0);
	buflen = add_cmts_mic ( buffer, buflen,key,keylen );
	printf ("Final buffer content:\n");
	buflen = add_eod_and_pad ( buffer, buflen );
	decode_tlvbuf ( buffer, buflen, 0);
	if ( (of = fopen ( argv[3],"w" )) == NULL ) { 
		printf ("%s: Can't open output file %s\n",argv[0],argv[3]);
		exit(-5);
	}
	fwrite ( buffer, sizeof(unsigned char), buflen, of);
	return 0;
}


