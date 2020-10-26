/* ***** BEGIN LICENSE BLOCK ***** 
 * Version: RCSL 1.0/RPSL 1.0 
 *  
 * Portions Copyright (c) 1995-2002 RealNetworks, Inc. All Rights Reserved. 
 *      
 * The contents of this file, and the files included with this file, are 
 * subject to the current version of the RealNetworks Public Source License 
 * Version 1.0 (the "RPSL") available at 
 * http://www.helixcommunity.org/content/rpsl unless you have licensed 
 * the file under the RealNetworks Community Source License Version 1.0 
 * (the "RCSL") available at http://www.helixcommunity.org/content/rcsl, 
 * in which case the RCSL will apply. You may also obtain the license terms 
 * directly from RealNetworks.  You may not use this file except in 
 * compliance with the RPSL or, if you have a valid RCSL with RealNetworks 
 * applicable to this file, the RCSL.  Please see the applicable RPSL or 
 * RCSL for the rights, obligations and limitations governing use of the 
 * contents of the file.  
 *  
 * This file is part of the Helix DNA Technology. RealNetworks is the 
 * developer of the Original Code and owns the copyrights in the portions 
 * it created. 
 *  
 * This file, and the files included with this file, is distributed and made 
 * available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS 
 * FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * 
 * Technology Compatibility Kit Test Suite(s) Location: 
 *    http://www.helixcommunity.org/content/tck 
 * 
 * Contributor(s): 
 *  
 * ***** END LICENSE BLOCK ***** */ 

/**************************************************************************************
 * Fixed-point MP3 decoder
 * Jon Recker (jrecker@real.com), Ken Cooke (kenc@real.com)
 * June 2003
 *
 * polyphase.c - final stage of subband transform (polyphase synthesis filter)
 *
 * This is the C reference version using __int64
 * Look in the appropriate subdirectories for optimized asm implementations 
 *   (e.g. arm/asmpoly.s)
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"

/* input to Polyphase = Q(DQ_FRACBITS_OUT-2), gain 2 bits in convolution
 *  we also have the implicit bias of 2^15 to add back, so net fraction bits = 
 *    DQ_FRACBITS_OUT - 2 - 2 - 15
 *  (see comment on Dequantize() for more info)
 */
#define DEF_NFRACBITS	(DQ_FRACBITS_OUT - 2 - 2 - 15)	
#define CSHIFT	12	/* coefficients have 12 leading sign bits for early-terminating mulitplies */

static __inline short ClipToShort(int x, int fracBits)
{
	int sign;
	
	/* assumes you've already rounded (x += (1 << (fracBits-1))) */
	x >>= fracBits;
	
	/* Ken's trick: clips to [-32768, 32767] */
	sign = x >> 31;
	if (sign != (x >> 15))
		x = sign ^ ((1 << 15) - 1);

	return (short)x;
}

#define MC0M(x)	{ \
	c1 = *coef;		coef++;		c2 = *coef;		coef++; \
	vLo = *(vb1+(x));			vHi = *(vb1+(23-(x))); \
	sum1L = MADD64(sum1L, vLo,  c1);	sum1L = MADD64(sum1L, vHi, -c2); \
}

#define MC1M(x)	{ \
	c1 = *coef;		coef++; \
	vLo = *(vb1+(x)); \
	sum1L = MADD64(sum1L, vLo,  c1); \
}

#define MC2M(x)	{ \
		c1 = *coef;		coef++;		c2 = *coef;		coef++; \
		vLo = *(vb1+(x));	vHi = *(vb1+(23-(x))); \
		sum1L = MADD64(sum1L, vLo,  c1);	sum2L = MADD64(sum2L, vLo,  c2); \
		sum1L = MADD64(sum1L, vHi, -c2);	sum2L = MADD64(sum2L, vHi,  c1); \
}

/**************************************************************************************
 * Function:    PolyphaseMono
 *
 * Description: filter one subband and produce 32 output PCM samples for one channel
 *
 * Inputs:      pointer to PCM output buffer
 *              number of "extra shifts" (vbuf format = Q(DQ_FRACBITS_OUT-2))
 *              pointer to start of vbuf (preserved from last call)
 *              start of filter coefficient table (in proper, shuffled order)
 *              no minimum number of guard bits is required for input vbuf 
 *                (see additional scaling comments below)
 *
 * Outputs:     32 samples of one channel of decoded PCM data, (i.e. Q16.0)
 *
 * Return:      none
 *
 * TODO:        add 32-bit version for platforms where 64-bit mul-acc is not supported
 *                (note max filter gain - see polyCoef[] comments)
 **************************************************************************************/
void PolyphaseMono(short *pcm, int *vbuf, const int *coefBase)
{	
	int i;
	const int *coef;
	int *vb1;
	int vLo, vHi, c1, c2;
	Word64 sum1L, sum2L, rndVal;

	rndVal = (Word64)( 1 << (DEF_NFRACBITS - 1 + (32 - CSHIFT)) );

	/* special case, output sample 0 */
	coef = coefBase;
	vb1 = vbuf;
	sum1L = rndVal;

	MC0M(0)
	MC0M(1)
	MC0M(2)
	MC0M(3)
	MC0M(4)
	MC0M(5)
	MC0M(6)
	MC0M(7)

	*(pcm + 0) = ClipToShort((int)SAR64(sum1L, (32-CSHIFT)), DEF_NFRACBITS);

	/* special case, output sample 16 */
	coef = coefBase + 256;
	vb1 = vbuf + 64*16;
	sum1L = rndVal;

	MC1M(0)
	MC1M(1)
	MC1M(2)
	MC1M(3)
	MC1M(4)
	MC1M(5)
	MC1M(6)
	MC1M(7)

	*(pcm + 16) = ClipToShort((int)SAR64(sum1L, (32-CSHIFT)), DEF_NFRACBITS);

	/* main convolution loop: sum1L = samples 1, 2, 3, ... 15   sum2L = samples 31, 30, ... 17 */
	coef = coefBase + 16;
	vb1 = vbuf + 64;
	pcm++;

	/* right now, the compiler creates bad asm from this... */
	for (i = 15; i > 0; i--) {
		sum1L = sum2L = rndVal;

		MC2M(0)
		MC2M(1)
		MC2M(2)
		MC2M(3)
		MC2M(4)
		MC2M(5)
		MC2M(6)
		MC2M(7)

		vb1 += 64;
		*(pcm)       = ClipToShort((int)SAR64(sum1L, (32-CSHIFT)), DEF_NFRACBITS);
		*(pcm + 2*i) = ClipToShort((int)SAR64(sum2L, (32-CSHIFT)), DEF_NFRACBITS);
		pcm++;
	}
}

#define MC0S(x)	{ \
	c1 = *coef;		coef++;		c2 = *coef;		coef++; \
	vLo = *(vb1+(x));		vHi = *(vb1+(23-(x))); \
	sum1L = MADD64(sum1L, vLo,  c1);	sum1L = MADD64(sum1L, vHi, -c2); \
	vLo = *(vb1+32+(x));	vHi = *(vb1+32+(23-(x))); \
	sum1R = MADD64(sum1R, vLo,  c1);	sum1R = MADD64(sum1R, vHi, -c2); \
}

#define MC1S(x)	{ \
	c1 = *coef;		coef++; \
	vLo = *(vb1+(x)); \
	sum1L = MADD64(sum1L, vLo,  c1); \
	vLo = *(vb1+32+(x)); \
	sum1R = MADD64(sum1R, vLo,  c1); \
}

#define MC2S(x)	{ \
		c1 = *coef;		coef++;		c2 = *coef;		coef++; \
		vLo = *(vb1+(x));	vHi = *(vb1+(23-(x))); \
		sum1L = MADD64(sum1L, vLo,  c1);	sum2L = MADD64(sum2L, vLo,  c2); \
		sum1L = MADD64(sum1L, vHi, -c2);	sum2L = MADD64(sum2L, vHi,  c1); \
		vLo = *(vb1+32+(x));	vHi = *(vb1+32+(23-(x))); \
		sum1R = MADD64(sum1R, vLo,  c1);	sum2R = MADD64(sum2R, vLo,  c2); \
		sum1R = MADD64(sum1R, vHi, -c2);	sum2R = MADD64(sum2R, vHi,  c1); \
}

/**************************************************************************************
 * Function:    PolyphaseStereo
 *
 * Description: filter one subband and produce 32 output PCM samples for each channel
 *
 * Inputs:      pointer to PCM output buffer
 *              number of "extra shifts" (vbuf format = Q(DQ_FRACBITS_OUT-2))
 *              pointer to start of vbuf (preserved from last call)
 *              start of filter coefficient table (in proper, shuffled order)
 *              no minimum number of guard bits is required for input vbuf 
 *                (see additional scaling comments below)
 *
 * Outputs:     32 samples of two channels of decoded PCM data, (i.e. Q16.0)
 *
 * Return:      none
 *
 * Notes:       interleaves PCM samples LRLRLR...
 *
 * TODO:        add 32-bit version for platforms where 64-bit mul-acc is not supported
 **************************************************************************************/
void PolyphaseStereo(short *pcm, int *vbuf, const int *coefBase)
{
	int i;
	const int *coef;
	int *vb1;
	int vLo, vHi, c1, c2;
	Word64 sum1L, sum2L, sum1R, sum2R, rndVal;

	rndVal = (Word64)( 1 << (DEF_NFRACBITS - 1 + (32 - CSHIFT)) );

	/* special case, output sample 0 */
	coef = coefBase;
	vb1 = vbuf;
	sum1L = sum1R = rndVal;

	MC0S(0)
	MC0S(1)
	MC0S(2)
	MC0S(3)
	MC0S(4)
	MC0S(5)
	MC0S(6)
	MC0S(7)

	*(pcm + 0) = ClipToShort((int)SAR64(sum1L, (32-CSHIFT)), DEF_NFRACBITS);
	*(pcm + 1) = ClipToShort((int)SAR64(sum1R, (32-CSHIFT)), DEF_NFRACBITS);

	/* special case, output sample 16 */
	coef = coefBase + 256;
	vb1 = vbuf + 64*16;
	sum1L = sum1R = rndVal;

	MC1S(0)
	MC1S(1)
	MC1S(2)
	MC1S(3)
	MC1S(4)
	MC1S(5)
	MC1S(6)
	MC1S(7)

	*(pcm + 2*16 + 0) = ClipToShort((int)SAR64(sum1L, (32-CSHIFT)), DEF_NFRACBITS);
	*(pcm + 2*16 + 1) = ClipToShort((int)SAR64(sum1R, (32-CSHIFT)), DEF_NFRACBITS);

	/* main convolution loop: sum1L = samples 1, 2, 3, ... 15   sum2L = samples 31, 30, ... 17 */
	coef = coefBase + 16;
	vb1 = vbuf + 64;
	pcm += 2;

	/* right now, the compiler creates bad asm from this... */
	for (i = 15; i > 0; i--) {
		sum1L = sum2L = rndVal;
		sum1R = sum2R = rndVal;

		MC2S(0)
		MC2S(1)
		MC2S(2)
		MC2S(3)
		MC2S(4)
		MC2S(5)
		MC2S(6)
		MC2S(7)

		vb1 += 64;
		*(pcm + 0)         = ClipToShort((int)SAR64(sum1L, (32-CSHIFT)), DEF_NFRACBITS);
		*(pcm + 1)         = ClipToShort((int)SAR64(sum1R, (32-CSHIFT)), DEF_NFRACBITS);
		*(pcm + 2*2*i + 0) = ClipToShort((int)SAR64(sum2L, (32-CSHIFT)), DEF_NFRACBITS);
		*(pcm + 2*2*i + 1) = ClipToShort((int)SAR64(sum2R, (32-CSHIFT)), DEF_NFRACBITS);
		pcm += 2;
	}
}
