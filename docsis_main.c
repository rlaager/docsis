/* 
 *  DOCSIS configuration file encoder. 
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002,2003 Evvolve Media SRL,office@evvolve.com
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
#include "docsis_symtable.h"
#include "docsis_globals.h"
#include "ethermac.h"
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WIN32
#include <io.h>
#include "inet_aton.h"
#else
#include <unistd.h>
#endif
#include <fcntl.h>


extern unsigned int line; 	/* defined in docsis_lex.l */


unsigned int add_cm_mic ( unsigned char *tlvbuf, unsigned int tlvbuflen ) 
{
  MD5_CTX mdContext; 
  unsigned char digest[16];

  MD5Init(&mdContext );
  MD5Update(&mdContext, tlvbuf, tlvbuflen );
  MD5Final(digest,&mdContext);
  tlvbuf[tlvbuflen]= 6;
  tlvbuf[tlvbuflen+1]=16;
  memcpy ( tlvbuf+tlvbuflen+2, digest, 16);
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
#define NR_TLVS 17  
  unsigned char digest_order[NR_TLVS] = { 1,2,3,4,17,43,6,18,19,20,22,23,24,25,28,29,26 }; 

  unsigned char *cmts_tlvs;
  unsigned char digest[17];

  cmts_tlvs = (unsigned char *) malloc ( tlvbuflen+1 );
  dp = cmts_tlvs;
  for (i=0; i<NR_TLVS; i++ ) { 
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
  printf ("##### Calculating CMTS MIC using TLVs:\n");
  decode_main_aggregate ( cmts_tlvs, dp-cmts_tlvs);
  printf ("##### End of CMTS MIC TLVs\n");
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
	printf ("Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net\n");
	printf ("Copyright (c) 2002,2003 Evvolve Media SRL, docsis@evvolve.com \n\n");
	printf ("To encode a cable modem configuration file: \n\t %s -e <modem_cfg_file> <key_file> <output_file>\n",prog_name);
	printf ("To encode a MTA configuration file: \n\t %s -p <mta_cfg_file> <output_file>\n",prog_name);
	printf ("To decode a CM or MTA config file: \n\t %s -d <binary_file>\n",prog_name);
	printf ("\nWhere:\n<modem_cfg_file>\t= name of the text (human readable) configuration file\n<key_file>\t\t= text file containing the authentication key to be used\n\t\t\t  for the CMTS MIC (Message Integrity Check).\n<output_file> \t\t= name of the output file where you want the binary data\n\t\t\t  to be written to (if it doesn't exist it is created).\n\t\t\t  This file can be TFTP-downloaded by DOCSIS-compliant\n\t\t\t  cable modems\n<binary_file>\t\t= name of the binary file you want to decode\n"); 
	printf ("\nSee examples/*.cfg for configuration file format.\n");
	printf ("\nPlease send bugs or questions to docsis-users@lists.sourceforge.net\n\n");
	exit ( -10 );
}

int main(int argc,char *argv[] ) 
{

	unsigned char key[65];
	FILE *cf,*kf,*of;
	unsigned char *buffer;
	unsigned int buflen=0,keylen=0;
	unsigned int is_mta;
	int i;

#ifdef WIN32
	int	nReturnCode;
	WSADATA	wsaData;
	WORD wVersionRequired = MAKEWORD(1, 1);
	// WinSock DLL initialization
	nReturnCode = WSAStartup(wVersionRequired, &wsaData);
	if (nReturnCode != 0) {
		printf("error: WinSock initialization failure\n");
		exit(1);
	}
	// WinSock version check
	if (wsaData.wVersion != wVersionRequired) {
		printf("error: WinSock version unavailable\n");
		WSACleanup();
		exit(1);
	}
	init_mib();
#endif

	setenv("MIBS", "ALL", 1);
	init_snmp("snmpapp");
        snmp_set_mib_warnings(2);

	init_global_symtable();

   	if (! netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID,  NETSNMP_DS_LIB_PRINT_NUMERIC_OIDS)) {
        	netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_NUMERIC_OIDS);
   	} /* we want OIDs to appear in numeric form */

   	if (! netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID,  NETSNMP_DS_LIB_PRINT_NUMERIC_ENUM)) {
        	netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_NUMERIC_ENUM);
   	} /* we want enums to appear in numeric form as integers */

   	if (! netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID,  NETSNMP_DS_LIB_PRINT_FULL_OID)) {
       		netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_FULL_OID);
   	} /* we want to full numeric OID to be printed, including prefix */

/*   	if (! netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID,  NETSNMP_DS_LIB_QUICK_PRINT)) {
        	netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_QUICK_PRINT);
   	} */ /* quick print - doesnt give you the type however */

/*   	if (! netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID,  NETSNMP_DS_LIB_DONT_BREAKDOWN_OIDS)) {
        	netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_DONT_BREAKDOWN_OIDS);
   	}  */
/*
   	if (! netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID,  NETSNMP_DS_LIB_PRINT_UCD_STYLE_OID)) {
        	netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_UCD_STYLE_OID);
   	}   */

   	if (! netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID,  NETSNMP_DS_LIB_DONT_PRINT_UNITS)) {
        	netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_DONT_PRINT_UNITS);
   	} 

        netsnmp_ds_set_int(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_OID_OUTPUT_FORMAT,
                                                      NETSNMP_OID_OUTPUT_SUFFIX); 

   	if (! netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_RANDOM_ACCESS)) {
       		netsnmp_ds_toggle_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_RANDOM_ACCESS);
#ifdef DEBUG
		printf ("/* Random OID access: %d */\n", netsnmp_ds_get_boolean(NETSNMP_DS_LIBRARY_ID,NETSNMP_DS_LIB_RANDOM_ACCESS));
#endif /* DEBUG */
   	} /* so we can use sysContact.0 instead of system.sysContact.0  */

	memset (prog_name,0,255);
	strncpy (prog_name, argv[0], 254);

	if ( ! (argc == 3 || argc == 4 || argc == 5 ) ) 
	{
		usage (prog_name); 
		exit (10);
	}
	
	if ( !strcmp (argv[1],"-e")) 
		{ 
			if ( argc != 5 ) usage(prog_name);
			is_mta = FALSE;
			if ( !strcmp (argv[2],argv[4]) ) 
			{ 
				printf ("Error: source file is same as destination file\n");
				exit (-100); /* we don't overwrite the source file */
			}
		} 
	else if ( !strcmp (argv[1], "-p")) 
		{
			if ( argc != 4 ) usage(prog_name);
			is_mta = TRUE;
			if ( !strcmp (argv[2],argv[3]) ) 
			{ 
				printf ("Error: source file is same as destination file\n");
				exit (-100); 
			}
		}
		
	else if ( !strcmp (argv[1],"-d")) 
		{
			if (argc != 3 ) 
			{ 
				usage(prog_name);
			} else {
		 		decode_file (argv[2]);
			}
		} else { 
			
			usage(prog_name); 
	     	}


#ifdef WIN32
	if ( (cf = fopen ( argv[2],"rb" ))== NULL ) { 
#else
	if ( (cf = fopen ( argv[2],"r" ))== NULL ) { 
#endif
		printf ("%s: Can't open config file %s\n",argv[0],argv[1]);
		exit(-5);
	}

	if (!is_mta) 
	{
#ifdef WIN32
		if ( (kf = fopen ( argv[3],"rb" ))== NULL ) { 
#else
		if ( (kf = fopen ( argv[3],"r" ))== NULL ) { 
#endif
			printf ("%s: Can't open keyfile %s\n",argv[0],argv[2]);
			exit(-5);
		}
        
        	keylen = fread (key,sizeof(unsigned char), 64, kf);

		if (keylen < 1) {
			printf ("%s: error: key must be at least 1 char long\n",argv[0]);
			exit (-101);
		}

		while (key[keylen-1] == 10 || key[keylen-1]==13) { 
			keylen--; /* eliminate trailing \n or \r */
		}
	} /* is_mta = FALSE */
        
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
	decode_main_aggregate ( buffer, buflen);
	if (!is_mta)  
	{
		/* Don't add CM MIC, CMTS MIC, EoD and pad unless it's a CM config file */
		buflen = add_cm_mic ( buffer, buflen );
		buflen = add_cmts_mic ( buffer, buflen,key,keylen );
		buflen = add_eod_and_pad ( buffer, buflen );
	} 
	printf ("Final buffer content:\n");
	decode_main_aggregate ( buffer, buflen );

	if (!is_mta) 
		{ 
	/* Output file is argv[4] */
#ifdef WIN32
			if ( (of = fopen ( argv[4],"wb" )) == NULL ) { 
#else
			if ( (of = fopen ( argv[4],"w" )) == NULL ) { 
#endif
				printf ("%s: Can't open output file %s\n",argv[0],argv[3]);
				exit(-5);
			}
		} 
	else 
		{
	
#ifdef WIN32
			if ( (of = fopen ( argv[3],"wb" )) == NULL ) { 
#else
			if ( (of = fopen ( argv[3],"w" )) == NULL ) { 
#endif
				printf ("%s: Can't open output file %s\n",argv[0],argv[3]);
				exit(-5);
			}
		}	

	fwrite ( buffer, sizeof(unsigned char), buflen, of);
	return 0;
}

int init_global_symtable(void)
{
  global_symtable = (symbol_type *) malloc(sizeof(symbol_type)*NUM_IDENTIFIERS);  if (global_symtable == NULL) {
        printf ( "Error allocating memory!\n");
        exit (255);
  }
  memcpy(global_symtable, symtable, sizeof(symbol_type)*NUM_IDENTIFIERS);
  return 1;
}

void decode_file(char *file) 
{
	int ifd;
	unsigned char *buffer;
	unsigned int buflen=0;
	int rv=0;
	struct stat st;

#ifdef WIN32
	if ( (ifd = open (file, O_RDONLY|O_BINARY))==-1 )  {
#else
	if ( (ifd = open (file, O_RDONLY))==-1 )  {
#endif
		printf("Error opening file %s: %s",file,strerror(errno));
		exit (-1);
	}	
	if ((rv=fstat(ifd, &st))) { 
		printf("stat on file %s: %s",file,strerror(errno));
		exit(-1);
	}
	
	buffer = (unsigned char *) malloc (st.st_size*sizeof(unsigned char)+1);
	
	buflen = read(ifd, buffer, st.st_size);

	decode_main_aggregate ( buffer, buflen );
	exit(0);
}
