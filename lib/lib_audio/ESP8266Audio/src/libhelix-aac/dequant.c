/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: dequant.c,v 1.2 2005/05/20 18:05:41 jrecker Exp $ 
 *   
 * Portions Copyright (c) 1995-2005 RealNetworks, Inc. All Rights Reserved.  
 *       
 * The contents of this file, and the files included with this file, 
 * are subject to the current version of the RealNetworks Public 
 * Source License (the "RPSL") available at 
 * http://www.helixcommunity.org/content/rpsl unless you have licensed 
 * the file under the current version of the RealNetworks Community 
 * Source License (the "RCSL") available at 
 * http://www.helixcommunity.org/content/rcsl, in which case the RCSL 
 * will apply. You may also obtain the license terms directly from 
 * RealNetworks.  You may not use this file except in compliance with 
 * the RPSL or, if you have a valid RCSL with RealNetworks applicable 
 * to this file, the RCSL.  Please see the applicable RPSL or RCSL for 
 * the rights, obligations and limitations governing use of the 
 * contents of the file. 
 *   
 * This file is part of the Helix DNA Technology. RealNetworks is the 
 * developer of the Original Code and owns the copyrights in the 
 * portions it created. 
 *   
 * This file, and the files included with this file, is distributed 
 * and made available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY 
 * KIND, EITHER EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS 
 * ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET 
 * ENJOYMENT OR NON-INFRINGEMENT. 
 *  
 * Technology Compatibility Kit Test Suite(s) Location:  
 *    http://www.helixcommunity.org/content/tck  
 *  
 * Contributor(s):  
 *   
 * ***** END LICENSE BLOCK ***** */  

/**************************************************************************************
 * Fixed-point HE-AAC decoder
 * Jon Recker (jrecker@real.com), Ken Cooke (kenc@real.com)
 * February 2005
 *
 * dequant.c - transform coefficient dequantization and short-block deinterleaving
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"

#define	SF_OFFSET			100

/* pow(2, i/4.0) for i = [0,1,2,3], format = Q30 */
static const int pow14[4] PROGMEM = { 
	0x40000000, 0x4c1bf829, 0x5a82799a, 0x6ba27e65
};

/* pow(2, i/4.0) * pow(j, 4.0/3.0) for i = [0,1,2,3],  j = [0,1,2,...,15]
 * format = Q28 for j = [0-3], Q25 for j = [4-15]
 */
static const int pow43_14[4][16] PROGMEM = {
	{
	0x00000000, 0x10000000, 0x285145f3, 0x453a5cdb, /* Q28 */
	0x0cb2ff53, 0x111989d6, 0x15ce31c8, 0x1ac7f203, /* Q25 */
	0x20000000, 0x257106b9, 0x2b16b4a3, 0x30ed74b4, /* Q25 */
	0x36f23fa5, 0x3d227bd3, 0x437be656, 0x49fc823c, /* Q25 */
	},
	{
	0x00000000, 0x1306fe0a, 0x2ff221af, 0x52538f52, 
	0x0f1a1bf4, 0x1455ccc2, 0x19ee62a8, 0x1fd92396, 
	0x260dfc14, 0x2c8694d8, 0x333dcb29, 0x3a2f5c7a, 
	0x4157aed5, 0x48b3aaa3, 0x50409f76, 0x57fc3010, 
	},
	{
	0x00000000, 0x16a09e66, 0x39047c0f, 0x61e734aa, 
	0x11f59ac4, 0x182ec633, 0x1ed66a45, 0x25dfc55a, 
	0x2d413ccd, 0x34f3462d, 0x3cefc603, 0x4531ab69, 
	0x4db4adf8, 0x56752054, 0x5f6fcfcd, 0x68a1eca1, 
	},
	{
	0x00000000, 0x1ae89f99, 0x43ce3e4b, 0x746d57b2, 
	0x155b8109, 0x1cc21cdc, 0x24ac1839, 0x2d0a479e, 
	0x35d13f33, 0x3ef80748, 0x48775c93, 0x524938cd, 
	0x5c68841d, 0x66d0df0a, 0x717e7bfe, 0x7c6e0305, 
	},
};

/* pow(j, 4.0 / 3.0) for j = [16,17,18,...,63], format = Q23 */
static const int pow43[48] PROGMEM = {
	0x1428a2fa, 0x15db1bd6, 0x1796302c, 0x19598d85, 
	0x1b24e8bb, 0x1cf7fcfa, 0x1ed28af2, 0x20b4582a, 
	0x229d2e6e, 0x248cdb55, 0x26832fda, 0x28800000, 
	0x2a832287, 0x2c8c70a8, 0x2e9bc5d8, 0x30b0ff99, 
	0x32cbfd4a, 0x34eca001, 0x3712ca62, 0x393e6088, 
	0x3b6f47e0, 0x3da56717, 0x3fe0a5fc, 0x4220ed72, 
	0x44662758, 0x46b03e7c, 0x48ff1e87, 0x4b52b3f3, 
	0x4daaebfd, 0x5007b497, 0x5268fc62, 0x54ceb29c, 
	0x5738c721, 0x59a72a59, 0x5c19cd35, 0x5e90a129, 
	0x610b9821, 0x638aa47f, 0x660db90f, 0x6894c90b, 
	0x6b1fc80c, 0x6daeaa0d, 0x70416360, 0x72d7e8b0, 
	0x75722ef9, 0x78102b85, 0x7ab1d3ec, 0x7d571e09, 
};

/* sqrt(0.5), format = Q31 */
#define SQRTHALF 0x5a82799a

/* Minimax polynomial approximation to pow(x, 4/3), over the range
 *  poly43lo: x = [0.5, 0.7071]
 *  poly43hi: x = [0.7071, 1.0]
 *
 * Relative error < 1E-7
 * Coefs are scaled by 4, 2, 1, 0.5, 0.25
 */
 
//fb
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
static const int poly43lo[5] PROGMEM = { 0x29a0bda9, 0xb02e4828, 0x5957aa1b, 0x236c498d, 0xff581859 };
static const int poly43hi[5] PROGMEM = { 0x10852163, 0xd333f6a4, 0x46e9408b, 0x27c2cef0, 0xfef577b4 };
#pragma GCC diagnostic pop

/* pow2exp[i] = pow(2, i*4/3) exponent */
static const int pow2exp[8] PROGMEM = { 14, 13, 11, 10, 9, 7, 6, 5 };

/* pow2exp[i] = pow(2, i*4/3) fraction */
static const int pow2frac[8] PROGMEM = {
	0x6597fa94, 0x50a28be6, 0x7fffffff, 0x6597fa94, 
	0x50a28be6, 0x7fffffff, 0x6597fa94, 0x50a28be6
};

/**************************************************************************************
 * Function:    DequantBlock
 *
 * Description: dequantize one block of transform coefficients (in-place)
 *
 * Inputs:      quantized transform coefficients, range = [0, 8191]
 *              number of samples to dequantize
 *              scalefactor for this block of data, range = [0, 256]
 *
 * Outputs:     dequantized transform coefficients in Q(FBITS_OUT_DQ_OFF)
 *
 * Return:      guard bit mask (OR of abs value of all dequantized coefs)
 *
 * Notes:       applies dequant formula y = pow(x, 4.0/3.0) * pow(2, (scale - 100)/4.0)
 *                * pow(2, FBITS_OUT_DQ_OFF)
 *              clips outputs to Q(FBITS_OUT_DQ_OFF)
 *              output has no minimum number of guard bits
 **************************************************************************************/
static int DequantBlock(int *inbuf, int nSamps, int scale)
{
	int iSamp, scalef, scalei, x, y, gbMask, shift, tab4[4];
	const int *tab16, *coef;

	if (nSamps <= 0)
		return 0;

	scale -= SF_OFFSET;	/* new range = [-100, 156] */

	/* with two's complement numbers, scalei/scalef factorization works for pos and neg values of scale:
	 *  [+4...+7] >> 2 = +1, [ 0...+3] >> 2 = 0, [-4...-1] >> 2 = -1, [-8...-5] >> 2 = -2 ...
	 *  (-1 & 0x3) = 3, (-2 & 0x3) = 2, (-3 & 0x3) = 1, (0 & 0x3) = 0
	 *
	 * Example: 2^(-5/4) = 2^(-1) * 2^(-1/4) = 2^-2 * 2^(3/4)
	 */
	tab16 = pow43_14[scale & 0x3];
	scalef = pow14[scale & 0x3];
	scalei = (scale >> 2) + FBITS_OUT_DQ_OFF;

	/* cache first 4 values:
	 * tab16[j] = Q28 for j = [0,3]
	 * tab4[x] = x^(4.0/3.0) * 2^(0.25*scale), Q(FBITS_OUT_DQ_OFF)
	 */
	shift = 28 - scalei;	
	if (shift > 31) {
		tab4[0] = tab4[1] = tab4[2] = tab4[3] = 0;
	} else if (shift <= 0) {
		shift = -shift;
		if (shift > 31)
			shift = 31;
		for (x = 0; x < 4; x++) {
			y = tab16[x];
			if (y > (0x7fffffff >> shift))
				y = 0x7fffffff;		/* clip (rare) */
			else
				y <<= shift;
			tab4[x] = y;
		}
	} else {
		tab4[0] = 0;
		tab4[1] = tab16[1] >> shift;
		tab4[2] = tab16[2] >> shift;
		tab4[3] = tab16[3] >> shift;
	}

	gbMask = 0;
	do {
		iSamp = *inbuf;
		x = FASTABS(iSamp);

		if (x < 4) {
			y = tab4[x];
		} else  {

			if (x < 16) {
				/* result: y = Q25 (tab16 = Q25) */
				y = tab16[x];
				shift = 25 - scalei;
			} else if (x < 64) {
				/* result: y = Q21 (pow43tab[j] = Q23, scalef = Q30) */
				y = pow43[x-16];	
				shift = 21 - scalei;
				y = MULSHIFT32(y, scalef);
			} else {
				/* normalize to [0x40000000, 0x7fffffff]
				 * input x = [64, 8191] = [64, 2^13-1]
				 * ranges:
				 *  shift = 7:   64 -  127
				 *  shift = 6:  128 -  255
				 *  shift = 5:  256 -  511
				 *  shift = 4:  512 - 1023
				 *  shift = 3: 1024 - 2047
				 *  shift = 2: 2048 - 4095
				 *  shift = 1: 4096 - 8191
				 */
				x <<= 17;
				shift = 0;
				if (x < 0x08000000)
					x <<= 4, shift += 4;
				if (x < 0x20000000)
					x <<= 2, shift += 2;
				if (x < 0x40000000)
					x <<= 1, shift += 1;

				coef = (x < SQRTHALF) ? poly43lo : poly43hi;

				/* polynomial */
				y = coef[0];
				y = MULSHIFT32(y, x) + coef[1];
				y = MULSHIFT32(y, x) + coef[2];
				y = MULSHIFT32(y, x) + coef[3];
				y = MULSHIFT32(y, x) + coef[4];
				y = MULSHIFT32(y, pow2frac[shift]) << 3;

				/* fractional scale 
				 * result: y = Q21 (pow43tab[j] = Q23, scalef = Q30)
				 */
				y = MULSHIFT32(y, scalef);	/* now y is Q24 */
				shift = 24 - scalei - pow2exp[shift];
			}

			/* integer scale */
			if (shift <= 0) {
				shift = -shift;
				if (shift > 31)
					shift = 31;

				if (y > (0x7fffffff >> shift))
					y = 0x7fffffff;		/* clip (rare) */
				else
					y <<= shift;
			} else {
				if (shift > 31)
					shift = 31;
				y >>= shift;
			}
		}

		/* sign and store (gbMask used to count GB's) */
		gbMask |= y;

		/* apply sign */
		iSamp >>= 31;
		y ^= iSamp;
		y -= iSamp;

		*inbuf++ = y;
	} while (--nSamps);

	return gbMask;
}

/**************************************************************************************
 * Function:    Dequantize
 *
 * Description: dequantize all transform coefficients for one channel
 *
 * Inputs:      valid AACDecInfo struct (including unpacked, quantized coefficients)
 *              index of current channel
 *
 * Outputs:     dequantized coefficients, including short-block deinterleaving
 *              flags indicating if intensity and/or PNS is active
 *              minimum guard bit count for dequantized coefficients
 *
 * Return:      0 if successful, error code (< 0) if error
 **************************************************************************************/
int Dequantize(AACDecInfo *aacDecInfo, int ch)
{
	int gp, cb, sfb, win, width, nSamps, gbMask;
	int *coef;
	const int /*short*/ *sfbTab;
	unsigned char *sfbCodeBook;
	short *scaleFactors;
	PSInfoBase *psi;
	ICSInfo *icsInfo;

	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return ERR_AAC_NULL_POINTER;
	psi = (PSInfoBase *)(aacDecInfo->psInfoBase);
	icsInfo = (ch == 1 && psi->commonWin == 1) ? &(psi->icsInfo[0]) : &(psi->icsInfo[ch]);
		
	if (icsInfo->winSequence == 2) {
		sfbTab = sfBandTabShort + sfBandTabShortOffset[psi->sampRateIdx];
		nSamps = NSAMPS_SHORT;
	} else {
		sfbTab = sfBandTabLong + sfBandTabLongOffset[psi->sampRateIdx];
		nSamps = NSAMPS_LONG;
	}
	coef = psi->coef[ch];
	sfbCodeBook = psi->sfbCodeBook[ch];
	scaleFactors = psi->scaleFactors[ch];

	psi->intensityUsed[ch] = 0;
	psi->pnsUsed[ch] = 0;
	gbMask = 0;
	for (gp = 0; gp < icsInfo->numWinGroup; gp++) {
		for (win = 0; win < icsInfo->winGroupLen[gp]; win++) {
			for (sfb = 0; sfb < icsInfo->maxSFB; sfb++) {
				/* dequantize one scalefactor band (not necessary if codebook is intensity or PNS) 
				 * for zero codebook, still run dequantizer in case non-zero pulse data was added
				 */
				cb = (int)(sfbCodeBook[sfb]);
				width = sfbTab[sfb+1] - sfbTab[sfb];
				if (cb >= 0 && cb <= 11)
					gbMask |= DequantBlock(coef, width, scaleFactors[sfb]);
				else if (cb == 13)
					psi->pnsUsed[ch] = 1;
				else if (cb == 14 || cb == 15)
					psi->intensityUsed[ch] = 1;	/* should only happen if ch == 1 */
				coef += width;
			}
			coef += (nSamps - sfbTab[icsInfo->maxSFB]);
		}
		sfbCodeBook += icsInfo->maxSFB;
		scaleFactors += icsInfo->maxSFB;
	}
	aacDecInfo->pnsUsed |= psi->pnsUsed[ch];	/* set flag if PNS used for any channel */

	/* calculate number of guard bits in dequantized data */
	psi->gbCurrent[ch] = CLZ(gbMask) - 1;

	return ERR_AAC_NONE;
}

/**************************************************************************************
 * Function:    DeinterleaveShortBlocks
 *
 * Description: deinterleave transform coefficients in short blocks for one channel
 *
 * Inputs:      valid AACDecInfo struct (including unpacked, quantized coefficients)
 *              index of current channel
 *
 * Outputs:     deinterleaved coefficients (window groups into 8 separate windows)
 *
 * Return:      0 if successful, error code (< 0) if error
 *
 * Notes:       only necessary if deinterleaving not part of Huffman decoding
 **************************************************************************************/
int DeinterleaveShortBlocks(AACDecInfo *aacDecInfo, int ch)
{
	(void)aacDecInfo;
	(void)ch;
	/* not used for this implementation - short block deinterleaving performed during Huffman decoding */
	return ERR_AAC_NONE;
}
