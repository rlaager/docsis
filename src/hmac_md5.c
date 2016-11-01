/*
 *  DOCSIS configuration file encoder.
 *  Copyright (c) 2001 Cornel Ciocirlan, ctrl@users.sourceforge.net.
 *  Copyright (c) 2002, 2003 Evvolve Media SRL, office@evvolve.com
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

#include <string.h>
#include <stdio.h>
#include "docsis.h"
#include "md5.h"

/*
 * debugging function
 * print the MD5 digest found in the arg buffer
 */

void md5_print_digest ( unsigned char *digest )
{
  int j;
  /* TODO check that the buffer actually contains 16 chars ... */
  fprintf(stdout, " --- MD5 DIGEST: 0x");
  for (j=0;j<16;j++)
	fprintf(stdout, "%02x", digest[j] );
  fprintf(stdout, "\n");
}

/*
** Function: hmac_md5
** Mostly cut & paste from RFC 2104
*/

/* unsigned char*  text;                pointer to data stream 		*/
/* int             text_len;            length of data stream 		*/
/* unsigned char*  key;                 pointer to authentication key 	*/
/* int             key_len;             length of authentication key 	*/
/* unsigned char*  digest;              caller digest to be filled in 	*/

void
hmac_md5(unsigned char *text, int text_len, unsigned char *key, size_t key_len, unsigned char *digest)
{
        MD5_CTX context;
        unsigned char k_ipad[65];    /* inner padding -
                                      * key XORd with ipad
                                      */
        unsigned char k_opad[65];    /* outer padding -
                                      * key XORd with opad
                                      */
        unsigned char tk[16];
        int i;
 	unsigned char tmpdigest[16];

        /* if key is longer than 64 bytes reset it to key=MD5(key) */
        if (key_len > 64) {

                MD5_CTX      tctx;

                MD5_Init(&tctx);
                MD5_Update(&tctx, key, key_len);
                MD5_Final(tmpdigest,&tctx);

                memcpy (tk, tmpdigest, 16);
		key = tk;
                key_len = 16;
        }

        /*
         * the HMAC_MD5 transform looks like:
         *
         * MD5(K XOR opad, MD5(K XOR ipad, text))
         *
         * where K is an n byte key
         * ipad is the byte 0x36 repeated 64 times
         * opad is the byte 0x5c repeated 64 times
         * and text is the data being protected
         */

        /* start out by storing key in pads */
        memset ( k_ipad, 0, sizeof k_ipad);
        memset ( k_opad, 0, sizeof k_opad);
        memcpy ( k_ipad, key, key_len);
        memcpy ( k_opad, key, key_len);

        /* XOR key with ipad and opad values */
        for (i=0; i<64; i++) {
                k_ipad[i] ^= 0x36;
                k_opad[i] ^= 0x5c;
        }
        /*
         * perform inner MD5
         */
        MD5_Init(&context);                   /* init context for 1st
                                              * pass */
        MD5_Update(&context, k_ipad, 64);      /* start with inner pad */
        MD5_Update(&context, text, text_len); /* then text of datagram */
        MD5_Final(digest, &context);          /* finish up 1st pass */
        /*
         * perform outer MD5
         */
        MD5_Init(&context);                   /* init context for 2nd
                                              * pass */
        MD5_Update(&context, k_opad, 64);     /* start with outer pad */
        MD5_Update(&context, digest, 16);     /* then results of 1st
                                              * hash */
        MD5_Final(digest,&context);          /* finish up 2nd pass */
}
