/* 
 *  DOCSIS configuration file encoder. 
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002,2003,2004 Evvolve Media SRL,office@evvolve.com
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
#include "docsis_symtable.h"
#include "docsis_globals.h"
#include "ethermac.h"
#include "md5.h"

extern unsigned int line;	/* defined in docsis_lex.l */

unsigned int
add_cm_mic (unsigned char *tlvbuf, unsigned int tlvbuflen)
{
  MD5_CTX mdContext;
  unsigned char digest[16];
  MD5Init (&mdContext);
  MD5Update (&mdContext, tlvbuf, tlvbuflen);
  MD5Final (digest, &mdContext);
  tlvbuf[tlvbuflen] = 6;
  tlvbuf[tlvbuflen + 1] = 16;
  memcpy (tlvbuf + tlvbuflen + 2, digest, 16);
  return (tlvbuflen + 18);	/* we added the CM Message Integrity Check  */
}

unsigned int
add_eod_and_pad (unsigned char *tlvbuf, unsigned int tlvbuflen)
{
  int nr_pads;
  tlvbuf[tlvbuflen] = 255;
  tlvbuflen = tlvbuflen + 1;
  nr_pads =
    4 - (tlvbuflen - 4 * ((unsigned int) floor ((double) (tlvbuflen / 4))));
  if (nr_pads < 0)
    {
      printf ("Illegal number of pads\n");
      exit (-1);
    }
  memset (&tlvbuf[tlvbuflen], 0, nr_pads);
  return (tlvbuflen + nr_pads);
}

unsigned int
add_cmts_mic (unsigned char *tlvbuf, unsigned int tlvbuflen,
	      unsigned char *key, int keylen)
{
  int i;
  register unsigned char *cp, *dp;

/* Only these configuration TLVs must be used to calculate the CMTS MIC */
#define NR_TLVS 17
  unsigned char digest_order[NR_TLVS] =
    { 1, 2, 3, 4, 17, 43, 6, 18, 19, 20, 22, 23, 24, 25, 28, 29, 26 };
  unsigned char *cmts_tlvs;
  unsigned char digest[17];
  cmts_tlvs = (unsigned char *) malloc (tlvbuflen + 1);
  dp = cmts_tlvs;
  for (i = 0; i < NR_TLVS; i++)
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
	      cp = cp + cp[1] + 2;
	    }
	}
    }
  printf ("##### Calculating CMTS MIC using TLVs:\n");
  decode_main_aggregate (cmts_tlvs, dp - cmts_tlvs);
  printf ("##### End of CMTS MIC TLVs\n");
  hmac_md5 (cmts_tlvs, dp - cmts_tlvs, key, keylen, digest);
  md5_print_digest (digest);
  tlvbuf[tlvbuflen] = 7;	/* CMTS MIC */
  tlvbuf[tlvbuflen + 1] = 16;	/* length of MD5 digest */
  memcpy (&tlvbuf[tlvbuflen + 2], digest, 16);
  free (cmts_tlvs);
  return (tlvbuflen + 18);
}

void
usage (char *prog_name)
{
  printf ("DOCSIS Configuration File creator, version %s\n", VERSION);
  printf
    ("Copyright (c) 1999,2000,2001 Cornel Ciocirlan, ctrl@users.sourceforge.net\n");
  printf
    ("Copyright (c) 2002,2003,2004 Evvolve Media SRL, docsis@evvolve.com \n\n");

  printf
    ("To encode a cable modem configuration file: \n\t %s -e <modem_cfg_file> <key_file> <output_file>\n",
     prog_name);
  printf
    ("To encode a MTA configuration file: \n\t %s -p <mta_cfg_file> <output_file>\n",
     prog_name);
  printf ("To decode a CM or MTA config file: \n\t %s -d <binary_file>\n",
	  prog_name);
  printf
    ("\nWhere:\n<cfg_file>\t\t= name of text (human readable) cable modem or MTA \n\t\t\t  configuration file\n<key_file>\t\t= text file containing the authentication key \n\t\t\t  (shared secret) to be used for the CMTS MIC\n<output_file> \t\t= name of output file where you want the binary data\n\t\t\t  to be written to (if it doesn't exist it is created).\n\t\t\t  This file can be TFTP-downloaded by DOCSIS-compliant\n\t\t\t  cable modems\n<binary_file>\t\t= name of binary file you want to be decoded\n");
  printf ("\nSee examples/*.cfg for configuration file format.\n");
  printf
    ("\nPlease send bugs or questions to docsis-users@lists.sourceforge.net\n\n");
  exit (-10);
} 


int
main (int argc, char *argv[])
{
  unsigned char key[65];
  FILE *kf, *of;
  char *config_file, *key_file, *output_file;
  unsigned char *buffer;
  unsigned int buflen = 0, keylen = 0;
  unsigned int encode_mta = FALSE, encode_docsis = FALSE, decode_bin = FALSE;
  int i=0, parse_result=0;
  memset (prog_name, 0, 255);
  strncpy (prog_name, argv[0], 254);
  switch (argc)

    {
    case 3:
      if (strcmp (argv[1], "-d"))
	usage (prog_name);
      decode_bin = TRUE;
      config_file = argv[2];
      key_file = NULL;
      output_file = NULL;
      break;
      ;;
    case 4:
      if (strcmp (argv[1], "-p"))
	usage (prog_name);
      encode_mta = TRUE;
      config_file = argv[2];
      key_file = NULL;
      output_file = argv[3];
      break;
      ;;
    case 5:
      encode_docsis = TRUE;
      if (strcmp (argv[1], "-e"))
	usage (prog_name);
      config_file = argv[2];
      key_file = argv[3];
      output_file = argv[4];
      break;
      ;;
    default:
      usage (prog_name);
      exit (10);
    }
  if (encode_docsis || encode_mta)
    {
      if (!strcmp (config_file, output_file))

	{
	  printf ("%s: Error: source file is the same as destination file\n", prog_name);
	  exit (-100);
	}
    }
  if (encode_docsis)

    {
      if ((kf = fopen (key_file, "r")) == NULL)
	{
	  printf ("%s: error: can't open keyfile %s\n", prog_name, key_file);
	  exit (-5);
	}
      keylen = fread (key, sizeof (unsigned char), 64, kf);
      if (keylen < 1)
	{
	  printf ("%s: error: key must be at least 1 char long\n", prog_name );
	  exit (-101);
	}
      while (key[keylen - 1] == 10 || key[keylen - 1] == 13)
	{
	  keylen--;		/* eliminate trailing \n or \r */
	}
    }

  init_global_symtable ();

#ifdef DEBUG
/*  snmp_set_mib_warnings (2); */
#endif /* DEBUG  */

  setenv ("MIBS", "ALL", 1);
  init_mib ();
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

#ifdef DEBUG
      printf ("/* Random OID access: %d */\n",
	      netsnmp_ds_get_boolean (NETSNMP_DS_LIBRARY_ID,
				      NETSNMP_DS_LIB_RANDOM_ACCESS));

#endif /* DEBUG */
    }				/* so we can use sysContact.0 instead of system.sysContact.0  */
  if (decode_bin)
    {
      decode_file (config_file);
    }
  else
    {
      parse_result = parse_config_file (config_file);
    }
  if (parse_result || global_tlvlist == NULL)
    {
      printf ("Error parsing config file %s\n", config_file);
      exit (70);
    }

/* Find out how much memory we need to malloc to hold all TLVs */

  for (i = 0; i < global_tlvlist->tlv_count; i++)
    {
      buflen += 2;		/* docs_code and len */
      buflen += global_tlvlist->tlvlist[i]->tlv_len;
    }
  buflen += 255;		/* leave some room for CM MIC, CMTS MIC, pad */
  buffer = (unsigned char *) malloc (buflen);
  buflen = flatten_tlvlist (buffer, global_tlvlist);
#ifdef DEBUG
  printf ("TLVs found in parsed config file:\n");
  decode_main_aggregate (buffer, buflen);
#endif 

  if (encode_docsis)
    {
      /* CM config file => add CM MIC, CMTS MIC, End-of-Data and pad */
      buflen = add_cm_mic (buffer, buflen);
      buflen = add_cmts_mic (buffer, buflen, key, keylen);
      buflen = add_eod_and_pad (buffer, buflen);
    }

  printf ("Final content of config file:\n");

  decode_main_aggregate (buffer, buflen);

  if ((of = fopen (output_file, "w")) == NULL)
    {
      printf ("%s: error: can't open output file %s\n", prog_name, output_file);
      exit (-5);
    }
  fwrite (buffer, sizeof (unsigned char), buflen, of);
  return 0;
}

int
init_global_symtable (void)
{
  global_symtable =
    (symbol_type *) malloc (sizeof (symbol_type) * NUM_IDENTIFIERS);
  if (global_symtable == NULL)

    {
      printf ("Error allocating memory!\n");
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
      printf ("Error opening binary file %s: %s", file, strerror (errno));
      exit (-1);
    }
  if ((rv = fstat (ifd, &st)))
    {
      printf ("Can't stat file %s: %s", file, strerror (errno));
      exit (-1);
    }
  buffer = (unsigned char *) malloc (st.st_size * sizeof (unsigned char) + 1);
  buflen = read (ifd, buffer, st.st_size);
  decode_main_aggregate (buffer, buflen);
  exit (0);
}
