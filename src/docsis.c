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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H  */

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/config_api.h>
#include <net-snmp/output_api.h>
#include <net-snmp/mib_api.h>

#include "docsis.h"
#include "docsis_globals.h"
#include "docsis_symtable.h"
#include "ethermac.h"
#include "md5.h"

struct tlv *global_tlvtree_head;
symbol_type *global_symtable;

static void setup_mib_flags(int resolve_oids, char *custom_mibs);

static unsigned int
add_cm_mic (unsigned char *tlvbuf, unsigned int tlvbuflen)
{
  unsigned char digest[16];
  MD5_CTX mdContext;

  if (tlvbuf == NULL || tlvbuflen == 0)
	return 0;

  MD5Init (&mdContext);
  MD5Update (&mdContext, tlvbuf, tlvbuflen);
  MD5Final (digest, &mdContext);
  tlvbuf[tlvbuflen] = 6;
  tlvbuf[tlvbuflen + 1] = 16;
  memcpy (tlvbuf + tlvbuflen + 2, digest, 16);
  return (tlvbuflen + 18);	/* we added the CM Message Integrity Check  */
}

static unsigned int
add_eod_and_pad (unsigned char *tlvbuf, unsigned int tlvbuflen)
{
  int nr_pads;

  if (tlvbuf == NULL || tlvbuflen == 0)
	return 0;

  tlvbuf[tlvbuflen] = 255;
  tlvbuflen = tlvbuflen + 1;
  nr_pads = (4 - (tlvbuflen % 4)) % 4;
  memset (&tlvbuf[tlvbuflen], 0, nr_pads);
  return (tlvbuflen + nr_pads);
}

static unsigned int
add_cmts_mic (unsigned char *tlvbuf, unsigned int tlvbuflen,
	      unsigned char *key, int keylen)
{

  int i;
  register unsigned char *cp, *dp;
  unsigned char *cmts_tlvs;
  unsigned char digest[17];

/* Only these configuration TLVs must be used to calculate the CMTS MIC */
#define NR_CMTS_MIC_TLVS 21
  unsigned char digest_order[NR_CMTS_MIC_TLVS] =
    { 1, 2, 3, 4, 17, 43, 6, 18, 19, 20, 22, 23, 24, 25, 28, 29, 26, 35, 36, 37, 40 };

  if (tlvbuf == NULL || tlvbuflen == 0 )
	return 0;

  cmts_tlvs = (unsigned char *) malloc (tlvbuflen + 1); /* Plenty of space */
  dp = cmts_tlvs;
  for (i = 0; i < NR_CMTS_MIC_TLVS; i++)
    {
      cp = tlvbuf;
      while ((unsigned int) (cp - tlvbuf) < tlvbuflen)
	{
	  if (cp[0] == digest_order[i])
	    {
	      memcpy (dp, cp, cp[1] + 2);
	      dp = dp + cp[1] + 2;
	      cp = cp + cp[1] + 2;
	    }
	  else
	    {
	      if ( cp[0] == 64 ) {
		fprintf(stderr, "docsis: warning: TLV64 (length > 255) not allowed in DOCSIS config files\n");
		cp = cp + (size_t) ntohs(*((unsigned short *)(cp+1))) + 3;
	      } else {
	      	cp = cp + cp[1] + 2;
	      }
	    }
	}
    }
  fprintf (stderr, "##### Calculating CMTS MIC using TLVs:\n");
  decode_main_aggregate (cmts_tlvs, dp - cmts_tlvs);
  fprintf (stderr, "##### End of CMTS MIC TLVs\n");
  hmac_md5 (cmts_tlvs, dp - cmts_tlvs, key, keylen, digest);
  md5_print_digest (digest);
  tlvbuf[tlvbuflen] = 7;	/* CMTS MIC */
  tlvbuf[tlvbuflen + 1] = 16;	/* length of MD5 digest */
  memcpy (&tlvbuf[tlvbuflen + 2], digest, 16);
  free (cmts_tlvs);
  return (tlvbuflen + 18);
}

#ifdef __GNUC__
static void usage () __attribute__((__noreturn__));
#endif

static void
usage ()
{
  fprintf(stderr, "DOCSIS Configuration File creator, version %s\n", VERSION);
  fprintf(stderr, "Copyright (c) 1999,2000,2001 Cornel Ciocirlan, ctrl@users.sourceforge.net\n");
  fprintf(stderr, "Copyright (c) 2002,2003,2004,2005 Evvolve Media SRL, docsis@evvolve.com \n\n");

  fprintf(stderr, "To encode a cable modem configuration file: \n\t docsis -e <modem_cfg_file> <key_file> <output_file>\n");
  fprintf(stderr, "To encode multiple cable modem configuration files: \n\t docsis -m <modem_cfg_file1> ...  <key_file> <new_extension>\n");
  fprintf(stderr, "To encode a MTA configuration file: \n\t docsis -p <mta_cfg_file> <output_file>\n");
  fprintf(stderr, "To encode multiple MTA configuration files: \n\t docsis -m -p <mta_file1> ...  <new_extension>\n");
  fprintf(stderr, "To decode a CM or MTA config file: \n\t docsis -d <binary_file>\n");
  fprintf(stderr, "To decode a CM or MTA config file with OIDs: \n\t docsis -o -d <binary_file>\n");
  fprintf(stderr, "\nTo specify the MIBPATH encode or decode use: \n"
		  "\tdocsis -M \"PATH1:PATH2\" -d <binary_file>\n"
		  "\tdocsis -M \"PATH1:PATH2\" -e <modem_cfg_file> <key_file> <output_file>\n"
		  "\tdocsis -M \"PATH1:PATH2\" -m <modem_cfg_file1> ...  <key_file> <new_extension>\n"
		  "\tdocsis -M \"PATH1:PATH2\" -p <mta_cfg_file> <output_file>\n"
		  "\tdocsis -M \"PATH1:PATH2\" -m -p <mta_file1> ...  <new_extension>\n");
  fprintf(stderr, "\nWhere:\n<cfg_file>\t\t= name of text (human readable) cable modem or MTA \n\t\t\t"
		  "configuration file\n<key_file>\t\t= text file containing the authentication key \n\t\t\t"
		  "(shared secret) to be used for the CMTS MIC\n<output_file> \t\t= name of output file where"
		  "the binary data will\n\t\t\t  be written to (if it does not exist it is created).\n<binary_file>"
                  "\t\t= name of binary file to be decoded\n<new_extension>\t\t= new extension to be used when encoding multiple files\n");
  fprintf(stderr, "\nSee examples/*.cfg for configuration file format.\n");
  fprintf(stderr, "\nPlease send bugs or questions to docsis-users@lists.sourceforge.net\n\n");
  exit (-10);
}


int
main (int argc, char *argv[])
{
  unsigned char key[65];
  FILE *kf;
  char *config_file=NULL, *key_file=NULL, *output_file=NULL, *extension_string=NULL, *custom_mibs=NULL;
  unsigned int keylen = 0;
  unsigned int encode_docsis = FALSE, decode_bin = FALSE;
  int i;
  int resolve_oids = 1;

  if (argc < 2 ) {
	usage();
  }

  if (!strcmp (argv[1], "-o") ){
	resolve_oids = 0;
	if (!strcmp (argv[2], "-d")) {
		decode_bin = TRUE;
		config_file = argv[3];
	} else {
		usage();
	}
  }else if (!strcmp (argv[1], "-m") ){ /* variable number of args, encoding multiple files */
	if (argc < 5 ) {
		usage();
	}
    	extension_string = argv[argc-1];
        if (!strcmp ( argv[2], "-p")) {
		key_file = NULL;
	} else {
		key_file = argv[argc-2];
		encode_docsis = TRUE;
	}
  } else if (!strcmp (argv[1], "-M") ){ /* define custom MIBDIRS */
        if (argc < 4 ) {
                usage();
        }
	custom_mibs=argv[2];
        if (!strcmp (argv[3], "-d")) {
                decode_bin = TRUE;
                config_file = argv[4];
        } else if (!strcmp (argv[3], "-m")) {
        	if (argc < 5 ) {
                	usage();
        	}
        	extension_string = argv[argc-1];
        	if (!strcmp ( argv[4], "-p")) {
                	key_file = NULL;
        	} else {
                	key_file = argv[argc-2];
                	encode_docsis = TRUE;
        	}
        } else if (!strcmp (argv[3], "-p")) {
                config_file = argv[4];
                output_file = argv[5];
        } else if (!strcmp (argv[3], "-e")) {
                encode_docsis = TRUE;
                config_file = argv[4];
                key_file = argv[5];
                output_file = argv[6];
	} else {
                usage();
        }	
  } else {
  	switch (argc)
    	{
    	case 3:
      		if (strcmp (argv[1], "-d"))
			usage ();
      		decode_bin = TRUE;
      		config_file = argv[2];
      		break;
      		;;
    	case 4:
      		if (strcmp (argv[1], "-p"))
			usage ();
      		config_file = argv[2];
      		output_file = argv[3];
      		break;
      		;;
    	case 5:
      		if (strcmp (argv[1], "-e"))
			usage ();
      		encode_docsis = TRUE;
      		config_file = argv[2];
      		key_file = argv[3];
      		output_file = argv[4];
      		break;
      		;;
    	default:
		usage ();
    }
  }

  if (encode_docsis)
    {
      if ((kf = fopen (key_file, "r")) == NULL)
	{
	  fprintf (stderr, "docsis: error: can't open keyfile %s\n", key_file);
	  exit (-5);
	}
      keylen = fread (key, sizeof (unsigned char), 64, kf);
      while (keylen > 0 && (key[keylen - 1] == 10 || key[keylen - 1] == 13))
	{
	  keylen--;		/* eliminate trailing \n or \r */
	}
    }

  init_global_symtable ();
  setup_mib_flags(resolve_oids,custom_mibs);

  if (decode_bin)
  {
      decode_file (config_file);
      exit(0); // TODO: clean shutdown
  }

  if (extension_string) { /* encoding multiple files */
	if (encode_docsis) {
		/* encode argv[argc-3] to argv[2] */
		for (i=2; i<argc-2; i++)  {
			if ( (output_file = get_output_name (argv[i], extension_string)) == NULL ) {
				fprintf(stderr, "Cannot process input file %s, extension too short ?\n",argv[i] );
				continue;
			}

			fprintf(stderr, "Processing input file %s: output to  %s\n",argv[i], output_file);
			if (encode_one_file (argv[i], output_file, key, keylen, encode_docsis)) {
				exit(2);
			}
			free (output_file);
			output_file = NULL;
		}
	} else {
		/* encode argv[argc-2] to argv[3] */
		for (i=3; i<argc-1; i++)  {
			if ( (output_file = get_output_name (argv[i], extension_string)) == NULL ) {
				fprintf(stderr, "Cannot process input file %s, extension too short ?\n",argv[i] );
				continue;
			}
			fprintf (stderr, "Processing input file %s: output to  %s\n",argv[i], output_file);
			if (encode_one_file (argv[i], output_file, key, keylen, encode_docsis)) {
				exit(2);
			}
			free (output_file);
			output_file = NULL;
		}
	}
  } else {
	if (encode_one_file (config_file, output_file, key, keylen, encode_docsis)) {
		exit(2);
	}
	/* encode argv[1] */
  }
  free(global_symtable);
  shutdown_mib();
  return 0;
}

int encode_one_file ( char *input_file, char *output_file,
	 		unsigned char *key, unsigned int keylen, int encode_docsis )
{
  int parse_result=0;
  unsigned int buflen;
  unsigned char *buffer;
  FILE *of;

  /* It's not an error to specify the input and output as "-". */
  if (!strcmp (input_file, output_file) && strcmp (input_file, "-"))
  {
	fprintf(stderr, "docsis: Error: source file is the same as destination file\n");
	return -1;
  }

  parse_result = parse_config_file (input_file, &global_tlvtree_head );

  if (parse_result || global_tlvtree_head == NULL)
    {
      fprintf(stderr, "Error parsing config file %s\n", input_file);
      return -1;
    }
/* Check whether we're encoding PacketCable */

  if (global_tlvtree_head->docs_code == 254) {
	fprintf(stderr, "First TLV is MtaConfigDelimiter, forcing PacketCable MTA file.\n");
	encode_docsis=0;
  }

/* walk the tree to find out how much memory we need */
	/* leave some room for CM MIC, CMTS MIC, pad */
  buflen = tlvtreelen (global_tlvtree_head);
  buffer = (unsigned char *) malloc ( buflen + 255 );
  buflen = flatten_tlvsubtree(buffer, 0, global_tlvtree_head);


#ifdef DEBUG
  fprintf(stderr, "TLVs found in parsed config file:\n");
  decode_main_aggregate (buffer, buflen);
#endif

  if (encode_docsis)
    {
      /* CM config file => add CM MIC, CMTS MIC, End-of-Data and pad */
      buflen = add_cm_mic (buffer, buflen);
      buflen = add_cmts_mic (buffer, buflen, key, keylen);
      buflen = add_eod_and_pad (buffer, buflen);
    }

  fprintf (stderr, "Final content of config file:\n");

  decode_main_aggregate (buffer, buflen);
  if (!strcmp (output_file, "-"))
    {
      of = stdout;
    }
  else if ((of = fopen (output_file, "wb")) == NULL)
    {
      fprintf (stderr, "docsis: error: can't open output file %s\n", output_file);
      return -2;
    }
  fwrite (buffer, sizeof (unsigned char), buflen, of);
  fclose (of);
  free(buffer);
  return 0;

  /*free(global_tlvlist->tlvlist); free(global_tlvlist); */ /* TODO free tree */
}

int
init_global_symtable (void)
{
  global_symtable =
    (symbol_type *) malloc (sizeof (symbol_type) * NUM_IDENTIFIERS);
  if (global_symtable == NULL)
    {
      fprintf(stderr, "Error allocating memory\n");
      exit (255);
    }
  memcpy (global_symtable, symtable, sizeof (symbol_type) * NUM_IDENTIFIERS);
  return 1;
}

void
decode_file (char *file)
{
  int ifd;
  unsigned char *buffer;
  unsigned int buflen = 0;
  int rv = 0;
  struct stat st;
  if ((ifd = open (file, O_RDONLY)) == -1)
    {
      fprintf(stderr, "Error opening binary file %s: %s\n", file, strerror (errno));
      exit (-1);
    }
  if ((rv = fstat (ifd, &st)))
    {
      fprintf(stderr, "Can't stat file %s: %s\n", file, strerror (errno));
      exit (-1);
    }
  buffer = (unsigned char *) malloc (st.st_size * sizeof (unsigned char) + 1);
  buflen = read (ifd, buffer, st.st_size);
  decode_main_aggregate (buffer, buflen);
  free(buffer);
}


static void
setup_mib_flags(int resolve_oids, char *custom_mibs) {

#ifdef DEBUG
/*  snmp_set_mib_warnings (2); */
#endif /* DEBUG  */
snmp_set_mib_warnings (1);

  if (custom_mibs)
    {
     setenv ("MIBDIRS", custom_mibs, 1);
    }

  if (resolve_oids)
    {
     setenv ("MIBS", "ALL", 1);
    }

#ifdef HAVE_NETSNMP_INIT_MIB
  netsnmp_init_mib ();
#else
  init_mib ();
#endif

  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_NUMERIC_OIDS))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_PRINT_NUMERIC_OIDS);
    }				/* we want OIDs to appear in numeric form */
  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_NUMERIC_ENUM))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_PRINT_NUMERIC_ENUM);
    }				/* we want enums to appear in numeric form as integers */
  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_PRINT_FULL_OID))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_PRINT_FULL_OID);
    }				/* we want to full numeric OID to be printed, including prefix */
  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_DONT_PRINT_UNITS))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_DONT_PRINT_UNITS);
    }
  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_RANDOM_ACCESS))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_RANDOM_ACCESS);
    }				/* so we can use sysContact.0 instead of system.sysContact.0  */
  if (!netsnmp_ds_get_boolean
      (NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_NUMERIC_TIMETICKS))
    {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID,
				 NETSNMP_DS_LIB_NUMERIC_TIMETICKS);
    }				/* so we can use sysContact.0 instead of system.sysContact.0  */
}


/*
 * Given a string representing a filename path and a new extension_string,
 * returns the path with the extension part replaced by the new extension.
 * The old filename must have an extension and the new extension cannot be
 * longer than the old one.
 */

char *get_output_name ( char *input_path, char *extension_string )
{
  size_t pathlen=0, i=0, old_ext_len=0;
  char *new_path;

  if (input_path == NULL || extension_string == NULL)
	return NULL;
  if ( (new_path = strdup(input_path) ) == NULL )
	return NULL;  /* out of memory */

  pathlen = strlen(input_path);

  /* Identify the length of the old extension */
  for (i=pathlen; i > 0; i--) {
  	if ( input_path[i] == '/' || input_path[i] == '\\' )
		break;
  	if ( input_path[i] == '.' )  {
		old_ext_len = pathlen - i;
		break;
	}
  }

  if (old_ext_len < strlen (extension_string) )
	return NULL;

  memset (&new_path[pathlen - old_ext_len], 0, old_ext_len);
  strncpy (&new_path[pathlen - old_ext_len], extension_string, strlen(extension_string) );

  return new_path;
  /* !!! caller has to free the new string after using it !!!  */
}
