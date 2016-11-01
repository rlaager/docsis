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
#include "sha1.h"

struct tlv *global_tlvtree_head;
symbol_type *global_symtable;
unsigned int nohash = 0;
unsigned int dialplan = 0;

static void setup_mib_flags(int resolve_oids, char *custom_mibs);

static unsigned int
add_cm_mic (unsigned char *tlvbuf, unsigned int tlvbuflen)
{
  unsigned char digest[16];
  MD5_CTX mdContext;

  if (tlvbuf == NULL || tlvbuflen == 0)
	return 0;

  MD5_Init (&mdContext);
  MD5_Update (&mdContext, tlvbuf, tlvbuflen);
  MD5_Final (digest, &mdContext);
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
		cp = cp + (size_t) ntohs(*((unsigned short *)(cp+1))) + 3;
	      } else {
	      	cp = cp + cp[1] + 2;
	      }
	    }
	}
    }
  fprintf (stdout, "##### Calculating CMTS MIC using TLVs:\n");
  decode_main_aggregate (cmts_tlvs, dp - cmts_tlvs);
  fprintf (stdout, "##### End of CMTS MIC TLVs\n");
  hmac_md5 (cmts_tlvs, dp - cmts_tlvs, key, keylen, digest);
  md5_print_digest (digest);
  tlvbuf[tlvbuflen] = 7;	/* CMTS MIC */
  tlvbuf[tlvbuflen + 1] = 16;	/* length of MD5 digest */
  memcpy (&tlvbuf[tlvbuflen + 2], digest, 16);
  free (cmts_tlvs);
  return (tlvbuflen + 18);
}

static unsigned int
add_mta_hash (unsigned char *tlvbuf, unsigned int tlvbuflen, unsigned int hash) {
  SHA_CTX shactx;
  unsigned char hash_value[SHA_DIGEST_LENGTH];

  SHA1_Init(&shactx);
  SHA1_Update(&shactx, tlvbuf, tlvbuflen);
  SHA1_Final(hash_value, &shactx);

  if (hash == 1) {
    memcpy (tlvbuf + tlvbuflen - 3, "\x0b\x28\x30\x26\x06\x0e\x2b\x06\x01\x04\x01\xa3\x0b\x02\x02\x01\x01\x02\x07\x00\x04\x14", 22);
    tlvbuflen += 19;
  }
  if (hash == 2) {
    memcpy (tlvbuf + tlvbuflen - 3, "\x0b\x26\x30\x24\x06\x0c\x2b\x06\x01\x04\x01\xba\x08\x01\x01\x02\x09\x00\x04\x14", 20);
    tlvbuflen += 17;
  }

  memcpy (tlvbuf + tlvbuflen, hash_value, SHA_DIGEST_LENGTH);
  tlvbuflen += SHA_DIGEST_LENGTH;
  memcpy (tlvbuf + tlvbuflen, "\xfe\x01\xff", 3);
  tlvbuflen += 3;

  return (tlvbuflen);
}

static unsigned int
add_dialplan (unsigned char *tlvbuf, unsigned int tlvbuflen) {
  FILE *dialplan_file;
  char *dialplan_buffer;
  unsigned int fileSize;
  unsigned int readSize;
  unsigned short local_v_len;
  unsigned short *p_local_v_len = &local_v_len;
  unsigned short local_char;
  unsigned short *p_local_char = &local_char;

  dialplan_file = fopen("dialplan.txt", "rb");
  if (!dialplan_file) {
    fprintf(stderr, "Cannot open dialplan.txt file, fatal error, closing.\n");
    exit(-1);
  }
  fseek(dialplan_file, 0, SEEK_END);
  fileSize = ftell (dialplan_file);
  fseek(dialplan_file, 0, SEEK_SET);
  dialplan_buffer = malloc(fileSize);
  if (!dialplan_buffer) {
    fprintf(stderr, "Fatal error allocating memory for dialplan buffer, closing.\n");
    exit(-1);
  }
  readSize = fread(dialplan_buffer, fileSize, 1, dialplan_file);
  if (!readSize) {
    fprintf(stderr, "Something went wrong reading the dialplan file, closing.\n");
    exit(-1);
  }
  fclose(dialplan_file);

  tlvbuflen -= 3;
  memcpy(tlvbuf + tlvbuflen, "\x40", 1);
  tlvbuflen += 1;
  if (fileSize > 0x7f) {
    local_v_len = htons(2 + 2 + 20 + 2 + 2 + fileSize);
  } else if (fileSize > 0x69) {
    local_v_len = htons(2 + 2 + 20 + 1 + 1 + fileSize);
  } else {
    local_v_len = htons(1 + 1 + 20 + 1 + 1 + fileSize);
  }
  memcpy(tlvbuf + tlvbuflen, p_local_v_len, sizeof(local_v_len));
  tlvbuflen += sizeof(local_v_len);
  memcpy(tlvbuf + tlvbuflen, "\x30", 1);
  tlvbuflen += 1;
  local_char = 0x16 + fileSize;
  if (local_char < 0x80) {
    memcpy(tlvbuf + tlvbuflen, p_local_char, sizeof(short));
    tlvbuflen += sizeof(char);
  } else {
    memcpy(tlvbuf + tlvbuflen, "\x82", 1);
    tlvbuflen += 1;
    if (fileSize > 0x7f) {
      local_v_len = htons(20 + 2 + 2 + fileSize);
    } else {
      local_v_len = htons(20 + 1 + 1 + fileSize);
    }
    memcpy(tlvbuf + tlvbuflen, p_local_v_len, sizeof(local_v_len));
    tlvbuflen += sizeof(local_v_len);
  }
  memcpy(tlvbuf + tlvbuflen, "\x06\x12\x2b\x06\x01\x04\x01\xa3\x0b\x02\x02\x08\x02\x01\x01\x03\x01\x01\x02\x00", 20);
  tlvbuflen += 20;
  memcpy(tlvbuf + tlvbuflen, "\x04", 1);
  tlvbuflen += 1;

  if (fileSize > 0x7f) {
    memcpy(tlvbuf + tlvbuflen, "\x82", 1);
    tlvbuflen += 1;
    local_v_len = (unsigned short) htons(fileSize);
    memcpy(tlvbuf + tlvbuflen, p_local_v_len, sizeof(local_v_len));
    tlvbuflen += sizeof(local_v_len);
  } else {
    local_char = (unsigned short) fileSize;
    memcpy(tlvbuf + tlvbuflen, p_local_char, sizeof(short));
    tlvbuflen += sizeof(char);
  }

  memcpy(tlvbuf + tlvbuflen, dialplan_buffer, fileSize);
  tlvbuflen = tlvbuflen + fileSize;
  free(dialplan_buffer);

  memcpy (tlvbuf + tlvbuflen, "\xfe\x01\xff", 3);
  tlvbuflen += 3;
  return (tlvbuflen);
}

#ifdef __GNUC__
static void usage () __attribute__((__noreturn__));
#endif

static void
usage ()
{
  fprintf(stderr, "DOCSIS Configuration File creator, version %s\n", VERSION);
  fprintf(stderr, "Copyright (c) 1999,2000,2001 Cornel Ciocirlan, ctrl@users.sourceforge.net\n");
  fprintf(stderr, "Copyright (c) 2002,2003,2004,2005 Evvolve Media SRL, docsis@evvolve.com\n");
  fprintf(stderr, "Copyright (c) 2014 - 2015 Adrian Simionov, daniel.simionov@gmail.com\n\n");

  fprintf(stderr, "To encode a cable modem configuration file: \n\tdocsis [modifiers] -e <modem_cfg_file> <key_file> <output_file>\n");
  fprintf(stderr, "To encode multiple cable modem configuration files: \n\tdocsis [modifiers] -m <modem_cfg_file1> ... <key_file> <new_extension>\n");
  fprintf(stderr, "To encode a MTA configuration file: \n\tdocsis [modifiers] -p <mta_cfg_file> <output_file>\n");
  fprintf(stderr, "To encode multiple MTA configuration files: \n\tdocsis [modifiers] -m -p <mta_file1> ... <new_extension>\n");
  fprintf(stderr, "To decode a CM or MTA config file: \n\tdocsis [modifiers] -d <binary_file>\n\n");

  fprintf(stderr, "Where:\n<cfg_file>\t\t= name of text (human readable) cable modem or MTA \n"
		  "\t\t\t  configuration file;\n"
		  "<key_file>\t\t= text file containing the authentication key\n"
		  "\t\t\t  (shared secret) to be used for the CMTS MIC;\n"
		  "<output_file> \t\t= name of output file where"
		  " the binary data will\n\t\t\t  be written to (if it does not exist it is created);\n"
		  "<binary_file>\t\t= name of binary file to be decoded;\n"
		  "<new_extension>\t\t= new extension to be used when encoding multiple files.\n\n");

  fprintf(stderr, "The following command-line modifiers are available:\n"
	"	-o\n"
	"		Decode OIDs numerically.\n\n"
	"	-M \"PATH1:PATH2\"\n"
	"		Specify the SNMP MIB directory when encoding or decoding configuration\n"
	"		files.\n\n"
	"	-na | -eu\n"
	"		Adds CableLabs PacketCable or Excentis EuroPacketCable SHA1 hash\n"
	"		when encoding an MTA config file.\n\n"
	"	-dialplan\n"
	"		Adds a PC20 dialplan from an external file called \"dialplan.txt\" in\n"
	"		the current directory.\n\n"
	"	-nohash\n"
	"		Removes the PacketCable SHA1 hash from the MTA config file when\n"
	"		decoding.\n"
	);
  fprintf(stderr, "\nSee examples/*.cfg for sample configuration files.\n");
  fprintf(stderr, "\nPlease report bugs or feature requests on GitHub.");
  fprintf(stderr, "\nProject repository is https://github.com/rlaager/docsis\n\n");
  exit (-10);
}

int
main (int argc, char *argv[])
{
  unsigned char key[65];
  FILE *kf;
  char *config_file=NULL, *key_file=NULL, *output_file=NULL, *extension_string=NULL, *custom_mibs=NULL;
  unsigned int keylen = 0;
  unsigned int encode_docsis = FALSE, decode_bin = FALSE, hash = 0;
  int i;
  int resolve_oids = 1;

  while (argc > 0) {
    argc--; argv++;

    if (!argc) {
      usage();
    }

    /* the initial command-line parameters are flags / modifiers */
    if (!strcmp (argv[0], "-nohash")) {
      nohash = 1;

      continue;
    }

    if (!strcmp (argv[0], "-o")) {
      resolve_oids = 0;
      continue;
    }

    if (!strcmp (argv[0], "-M")) {
      if (argc < 2 ) {
        usage();
      }

      custom_mibs=argv[1];

      argc--; argv++;
      continue;
    }

    if (!strcmp (argv[0], "-na")) {
      if (hash) {
        usage();
      }
      hash = 1;
      continue;
    }

    if (!strcmp (argv[0], "-eu")) {
      if (hash) {
        usage();
      }
      hash = 2;
      continue;
    }

    if (!strcmp (argv[0], "-dialplan")) {
      dialplan = 1;
      continue;
    }

    /* the following command-line parameters are actions */

    if (!strcmp (argv[0], "-d")) {
      if (argc < 2 ) {
        usage();
      }

      decode_bin = TRUE;
      config_file = argv[1];

      break;
    }

    if (!strcmp (argv[0], "-e")) {
      if (argc < 4 ) {
        usage();
      }

      encode_docsis = TRUE;
      config_file = argv[1];
      key_file = argv[2];
      output_file = argv[3];

      break;
    }

    if (!strcmp (argv[0], "-m")) {
      extension_string = argv[argc-1];
      key_file = argv[argc-2];
      encode_docsis = TRUE;

      continue;
    }

    if (!strcmp (argv[0], "-p")) {
      /* encode_docsis may already have been set via the "-m" option */
      encode_docsis = 0;

      argc--; argv++;

      if (argc < 2 ) {
        usage();
      }

      /* -p might be followed by -dialplan.  This is allowed for backwards
       * compatibility */
      if (!strcmp (argv[0], "-dialplan")) {
        dialplan = 1;
        argc--; argv++;
      }

      if (argc < 2 ) {
        usage();
      }

      /* if -m has not already been specified, then we expect "<mta_cfg_file> <output_file>" */
      if (extension_string == NULL) {
        config_file = argv[0];
        output_file = argv[1];
      }

      break;
    }

    /* no more recognisable options means that we've either finished parsing
     * all arguments or else that the remaining arguments refer to a list of
     * config files */
    if ((argc && encode_docsis) || (argc && decode_bin)) {
      break;
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
      fclose(kf);
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
		/* encode argv[argc-3] to argv[0] */
		for (i=0; i<argc-2; i++) {
			if ( (output_file = get_output_name (argv[i], extension_string)) == NULL ) {
				fprintf(stderr, "Cannot process input file %s, extension too short ?\n",argv[i] );
				continue;
			}

			fprintf(stderr, "Processing input file %s: output to  %s\n",argv[i], output_file);
			if (encode_one_file (argv[i], output_file, key, keylen, encode_docsis, hash)) {
				exit(2);
			}
			free (output_file);
			output_file = NULL;
		}
	} else {
		/* encode argv[argc-2] to argv[0] */
		for (i=0; i<argc-1; i++) {
			if ( (output_file = get_output_name (argv[i], extension_string)) == NULL ) {
				fprintf(stderr, "Cannot process input file %s, extension too short ?\n",argv[i] );
				continue;
			}
			fprintf (stderr, "Processing input file %s: output to  %s\n",argv[i], output_file);
			if (encode_one_file (argv[i], output_file, key, keylen, encode_docsis, hash)) {
				exit(2);
			}
			free (output_file);
			output_file = NULL;
		}
	}
  } else {
	if (encode_one_file (config_file, output_file, key, keylen, encode_docsis, hash)) {
		exit(2);
	}
	/* encode argv[1] */
  }
  free(global_symtable);
  shutdown_mib();
  return 0;
}

int encode_one_file ( char *input_file, char *output_file,
	 		unsigned char *key, unsigned int keylen, int encode_docsis, unsigned int hash)
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
	/* leave some room for CM MIC, CMTS MIC, pad, and a HUGE PC20 dialplan */
  buflen = tlvtreelen (global_tlvtree_head);
  buffer = (unsigned char *) malloc ( buflen + 255 + 8192 );
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

  if (dialplan == 1) {
    printf("Adding PC20 dialplan from external file.\n");
    buflen = add_dialplan (buffer, buflen);
  }

  if (hash == 1) {
    printf("Adding NA ConfigHash to MTA file.\n");
    buflen = add_mta_hash (buffer, buflen, hash);
  }
  if (hash == 2) {
    printf("Adding EU ConfigHash to MTA file.\n");
    buflen = add_mta_hash (buffer, buflen, hash);
  }

  fprintf (stdout, "Final content of config file:\n");

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
  if (fwrite (buffer, 1, buflen, of) != buflen)
    {
      fprintf (stderr, "docsis: error: can't write to output file %s\n", output_file);
      return -2;
    }
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
/* We do not want warning for normal users. Should be set with an argument on the CLI maybe?
 * snmp_set_mib_warnings (1); */

  if (custom_mibs)
    {
     setenv ("MIBDIRS", custom_mibs, 1);
    }

  if (resolve_oids)
    {
     setenv ("MIBS", "ALL", 1);
    }

  if (!resolve_oids) {
    if (!netsnmp_ds_get_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_OID_OUTPUT_NUMERIC)) {
      netsnmp_ds_toggle_boolean (NETSNMP_DS_LIBRARY_ID, NETSNMP_OID_OUTPUT_NUMERIC);
    }
  }

  /* DOCSIS vendors are notorious for supplying MIBs with invalid syntax */
  netsnmp_ds_set_boolean(NETSNMP_DS_LIBRARY_ID, NETSNMP_DS_LIB_MIB_PARSE_LABEL, 1);

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
