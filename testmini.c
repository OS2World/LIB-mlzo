/* testmini.c -- very simple test program for the miniLZO library

   This file is part of the LZO real-time data compression library.

   Copyright (C) 1997 Markus Franz Xaver Johannes Oberhumer
   Copyright (C) 1996 Markus Franz Xaver Johannes Oberhumer

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   Markus F.X.J. Oberhumer
   markus.oberhumer@jk.uni-linz.ac.at
 */


#include <stdio.h>
#include <stdlib.h>


/*************************************************************************
// This program shows the basic usage of the LZO library.
// We will compress a block of data and decompress again.
//
// Please see also LZO.FAQ
**************************************************************************/

/* First let's include "minizo.h". */

#include "minilzo.h"


/* We want to compress the data block at `in' with length `IN_LEN' to
 * the block at `out'. Because the input block may be incompressible,
 * we must provide a little more output space in case that compression
 * is not possible.
 */

#define IN_LEN		(128*1024L)
#define OUT_LEN		(IN_LEN + IN_LEN / 64 + 16 + 3)

static lzo_byte in  [ IN_LEN ];
static lzo_byte out [ OUT_LEN ];


/* Work-memory allocation.
 * Because we will align our work memory on a multiple of 16 (for reasons
 * of speed) we allocate 16 extra bytes.
 */

static lzo_byte _wrkmem [ LZO1X_MEM_COMPRESS + 16 ];


/*************************************************************************
//
**************************************************************************/

int main(int argc, char *argv[])
{
	int r;
	lzo_uint in_len;
	lzo_uint out_len;
	lzo_byte *wrkmem;

	if (argc < 0 || argv == NULL)	/* avoid warning about unused args */
		return 0;

/*
 * Step 1: prepare the input block that will get compressed.
 *         We just fill it with zeros in this example program,
 *         but you would use your real-world data here.
 */
	in_len = IN_LEN;
	lzo_memset(in,0,in_len);

/*
 * Step 2: align the work-memory
 */
	wrkmem = LZO_ALIGN(_wrkmem,16);

/*
 * Step 3: initialize the LZO library
 */
	if (lzo_init() != LZO_E_OK)
	{
		printf("lzo_init() failed !!!\n");
		return 3;
	}

/*
 * Step 4: compress from `in' to `out' with LZO1X-1
 */
	r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
	if (r == LZO_E_OK)
		printf("compressed %lu bytes into %lu bytes\n",
			(long) in_len, (long) out_len);
	else
	{
		printf("compression failed: %d\n", r);
		return 2;
	}
	/* check for an incompressible block */
	if (out_len >= in_len)
	{
		printf("This block contains incompressible data.\n");
		return 0;
	}

/*
 * Step 5: decompress again, now going from `out' to `in'
 */
	r = lzo1x_decompress(out,out_len,in,&in_len,NULL);
	if (r == LZO_E_OK)
		printf("decompressed %lu bytes back into %lu bytes\n",
			(long) out_len, (long) in_len);
	else
	{
		printf("decompression failed: %d\n", r);
		return 1;
	}

	printf("miniLZO simple compression test passed.\n");
	return 0;
}

/*
vi:ts=4
*/
