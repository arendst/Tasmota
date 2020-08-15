/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: huffman.c,v 1.2 2005/05/24 16:01:55 albertofloyd Exp $ 
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
 * Jon Recker (jrecker@real.com)
 * February 2005
 *
 * huffman.c - Huffman decoding
 **************************************************************************************/

#include "coder.h"

/**************************************************************************************
 * Function:    DecodeHuffmanScalar
 *
 * Description: decode one Huffman symbol from bitstream
 *
 * Inputs:      pointers to Huffman table and info struct
 *              left-aligned bit buffer with >= huffTabInfo->maxBits bits
 *
 * Outputs:     decoded symbol in *val
 *
 * Return:      number of bits in symbol
 *
 * Notes:       assumes canonical Huffman codes:
 *                first CW always 0, we have "count" CW's of length "nBits" bits
 *                starting CW for codes of length nBits+1 = 
 *                  (startCW[nBits] + count[nBits]) << 1
 *                if there are no codes at nBits, then we just keep << 1 each time 
 *                  (since count[nBits] = 0)
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ int DecodeHuffmanScalar(const signed short *huffTab, const HuffInfo *huffTabInfo, unsigned int bitBuf, signed int *val)
{
    unsigned int count, start, shift, t;
	const unsigned /*char*/ int *countPtr;
	const signed short *map;

	map = huffTab + huffTabInfo->offset;
	countPtr = huffTabInfo->count;

	start = 0;
	count = 0;
	shift = 32;
	do {
		start += count;
		start <<= 1;
		map += count;
		count = *countPtr++;
		shift--;
		t = (bitBuf >> shift) - start;
	} while (t >= count);
	
	*val = (signed int)pgm_read_word(&map[t]);
	return (countPtr - huffTabInfo->count);
}

#define APPLY_SIGN(v, s)		{(v) ^= ((signed int)(s) >> 31); (v) -= ((signed int)(s) >> 31);}

#define GET_QUAD_SIGNBITS(v)	(((unsigned int)(v) << 17) >> 29)	/* bits 14-12, unsigned */
#define GET_QUAD_W(v)			(((signed int)(v) << 20) >>   29)	/* bits 11-9, sign-extend */
#define GET_QUAD_X(v)			(((signed int)(v) << 23) >>   29)	/* bits  8-6, sign-extend */
#define GET_QUAD_Y(v)			(((signed int)(v) << 26) >>   29)	/* bits  5-3, sign-extend */
#define GET_QUAD_Z(v)			(((signed int)(v) << 29) >>   29)	/* bits  2-0, sign-extend */

#define GET_PAIR_SIGNBITS(v)	(((unsigned int)(v) << 20) >> 30)	/* bits 11-10, unsigned */
#define GET_PAIR_Y(v)			(((signed int)(v) << 22) >>   27)	/* bits  9-5, sign-extend */
#define GET_PAIR_Z(v)			(((signed int)(v) << 27) >>   27)	/* bits  4-0, sign-extend */

#define GET_ESC_SIGNBITS(v)		(((unsigned int)(v) << 18) >> 30)	/* bits 13-12, unsigned */
#define GET_ESC_Y(v)			(((signed int)(v) << 20) >>   26)	/* bits 11-6, sign-extend */
#define GET_ESC_Z(v)			(((signed int)(v) << 26) >>   26)	/* bits  5-0, sign-extend */

/**************************************************************************************
 * Function:    UnpackZeros
 *
 * Description: fill a section of coefficients with zeros
 *
 * Inputs:      number of coefficients
 *
 * Outputs:     nVals zeros, starting at coef
 *
 * Return:      none
 *
 * Notes:       assumes nVals is always a multiple of 4 because all scalefactor bands
 *                are a multiple of 4 coefficients long
 **************************************************************************************/
static void UnpackZeros(int nVals, int *coef)
{
	while (nVals > 0) {
		*coef++ = 0;
		*coef++ = 0;
		*coef++ = 0;
		*coef++ = 0;
		nVals -= 4;
	}
}

/**************************************************************************************
 * Function:    UnpackQuads
 *
 * Description: decode a section of 4-way vector Huffman coded coefficients
 *
 * Inputs       BitStreamInfo struct pointing to start of codewords for this section
 *              index of Huffman codebook
 *              number of coefficients
 *
 * Outputs:     nVals coefficients, starting at coef
 *
 * Return:      none
 *
 * Notes:       assumes nVals is always a multiple of 4 because all scalefactor bands
 *                are a multiple of 4 coefficients long
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ static void UnpackQuads(BitStreamInfo *bsi, int cb, int nVals, int *coef)
{
	int w, x, y, z, maxBits, nCodeBits, nSignBits, val;
	unsigned int bitBuf;

	maxBits = huffTabSpecInfo[cb - HUFFTAB_SPEC_OFFSET].maxBits + 4;
	while (nVals > 0) {
		/* decode quad */
		bitBuf = GetBitsNoAdvance(bsi, maxBits) << (32 - maxBits);
		nCodeBits = DecodeHuffmanScalar(huffTabSpec, &huffTabSpecInfo[cb - HUFFTAB_SPEC_OFFSET], bitBuf, &val);

		w = GET_QUAD_W(val);
		x = GET_QUAD_X(val);
		y = GET_QUAD_Y(val);
		z = GET_QUAD_Z(val);

		bitBuf <<= nCodeBits;
		nSignBits = (int)GET_QUAD_SIGNBITS(val);
		AdvanceBitstream(bsi, nCodeBits + nSignBits);
		if (nSignBits) {
			if (w)	{APPLY_SIGN(w, bitBuf); bitBuf <<= 1;}
			if (x)	{APPLY_SIGN(x, bitBuf); bitBuf <<= 1;}
			if (y)	{APPLY_SIGN(y, bitBuf); bitBuf <<= 1;}
			if (z)	{APPLY_SIGN(z, bitBuf); bitBuf <<= 1;}
		}
		*coef++ = w; *coef++ = x; *coef++ = y; *coef++ = z;
		nVals -= 4;
	}
}

/**************************************************************************************
 * Function:    UnpackPairsNoEsc
 *
 * Description: decode a section of 2-way vector Huffman coded coefficients,
 *                using non-esc tables (5 through 10)
 *
 * Inputs       BitStreamInfo struct pointing to start of codewords for this section
 *              index of Huffman codebook (must not be the escape codebook)
 *              number of coefficients
 *
 * Outputs:     nVals coefficients, starting at coef
 *
 * Return:      none
 *
 * Notes:       assumes nVals is always a multiple of 2 because all scalefactor bands
 *                are a multiple of 4 coefficients long
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ static void UnpackPairsNoEsc(BitStreamInfo *bsi, int cb, int nVals, int *coef)
{
	int y, z, maxBits, nCodeBits, nSignBits, val;
	unsigned int bitBuf;

	maxBits = huffTabSpecInfo[cb - HUFFTAB_SPEC_OFFSET].maxBits + 2;
	while (nVals > 0) {
		/* decode pair */
		bitBuf = GetBitsNoAdvance(bsi, maxBits) << (32 - maxBits);
		nCodeBits = DecodeHuffmanScalar(huffTabSpec, &huffTabSpecInfo[cb-HUFFTAB_SPEC_OFFSET], bitBuf, &val);

		y = GET_PAIR_Y(val);
		z = GET_PAIR_Z(val);

		bitBuf <<= nCodeBits;
		nSignBits = GET_PAIR_SIGNBITS(val);
		AdvanceBitstream(bsi, nCodeBits + nSignBits);
		if (nSignBits) {
			if (y)	{APPLY_SIGN(y, bitBuf); bitBuf <<= 1;}
			if (z)	{APPLY_SIGN(z, bitBuf); bitBuf <<= 1;}
		}
		*coef++ = y; *coef++ = z;
		nVals -= 2;
	}
}

/**************************************************************************************
 * Function:    UnpackPairsEsc
 *
 * Description: decode a section of 2-way vector Huffman coded coefficients,
 *                using esc table (11)
 *
 * Inputs       BitStreamInfo struct pointing to start of codewords for this section
 *              index of Huffman codebook (must be the escape codebook)
 *              number of coefficients
 *
 * Outputs:     nVals coefficients, starting at coef
 *
 * Return:      none
 *
 * Notes:       assumes nVals is always a multiple of 2 because all scalefactor bands
 *                are a multiple of 4 coefficients long
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ static void UnpackPairsEsc(BitStreamInfo *bsi, int cb, int nVals, int *coef)
{
	int y, z, maxBits, nCodeBits, nSignBits, n, val;
	unsigned int bitBuf;

	maxBits = huffTabSpecInfo[cb - HUFFTAB_SPEC_OFFSET].maxBits + 2;
	while (nVals > 0) {
		/* decode pair with escape value */
		bitBuf = GetBitsNoAdvance(bsi, maxBits) << (32 - maxBits);
		nCodeBits = DecodeHuffmanScalar(huffTabSpec, &huffTabSpecInfo[cb-HUFFTAB_SPEC_OFFSET], bitBuf, &val);

		y = GET_ESC_Y(val);
		z = GET_ESC_Z(val);

		bitBuf <<= nCodeBits;
		nSignBits = GET_ESC_SIGNBITS(val);
		AdvanceBitstream(bsi, nCodeBits + nSignBits);
	
		if (y == 16) {
			n = 4;
			while (GetBits(bsi, 1) == 1)
				n++;
			y = (1 << n) + GetBits(bsi, n);
		}
		if (z == 16) {
			n = 4;
			while (GetBits(bsi, 1) == 1)
				n++;
			z = (1 << n) + GetBits(bsi, n);
		}

		if (nSignBits) {
			if (y)	{APPLY_SIGN(y, bitBuf); bitBuf <<= 1;}
			if (z)	{APPLY_SIGN(z, bitBuf); bitBuf <<= 1;}
		}

		*coef++ = y; *coef++ = z;
		nVals -= 2;
	}
}

/**************************************************************************************
 * Function:    DecodeSpectrumLong
 *
 * Description: decode transform coefficients for frame with one long block
 *
 * Inputs:      platform specific info struct
 *              BitStreamInfo struct pointing to start of spectral data
 *                (14496-3, table 4.4.29) 
 *              index of current channel
 *
 * Outputs:     decoded, quantized coefficients for this channel
 *
 * Return:      none
 *
 * Notes:       adds in pulse data if present
 *              fills coefficient buffer with zeros in any region not coded with
 *                codebook in range [1, 11] (including sfb's above sfbMax)
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ void DecodeSpectrumLong(PSInfoBase *psi, BitStreamInfo *bsi, int ch)
{
	int i, sfb, cb, nVals, offset;
	const /*short*/ int *sfbTab;
	unsigned char *sfbCodeBook;
	int *coef;
	ICSInfo *icsInfo;
	PulseInfo *pi;

	coef = psi->coef[ch];
	icsInfo = (ch == 1 && psi->commonWin == 1) ? &(psi->icsInfo[0]) : &(psi->icsInfo[ch]);

	/* decode long block */
	sfbTab = sfBandTabLong + sfBandTabLongOffset[psi->sampRateIdx];
	sfbCodeBook = psi->sfbCodeBook[ch];
	for (sfb = 0; sfb < icsInfo->maxSFB; sfb++) {
		cb = *sfbCodeBook++;
		nVals = sfbTab[sfb+1] - sfbTab[sfb];
		
		if (cb == 0)
			UnpackZeros(nVals, coef);
		else if (cb <= 4)
			UnpackQuads(bsi, cb, nVals, coef);
		else if (cb <= 10)
			UnpackPairsNoEsc(bsi, cb, nVals, coef);
		else if (cb == 11)
			UnpackPairsEsc(bsi, cb, nVals, coef);
		else
			UnpackZeros(nVals, coef);

		coef += nVals;
	}

	/* fill with zeros above maxSFB */
	nVals = NSAMPS_LONG - sfbTab[sfb];
	UnpackZeros(nVals, coef);

	/* add pulse data, if present */
	pi = &psi->pulseInfo[ch];
	if (pi->pulseDataPresent) {
		coef = psi->coef[ch];
		offset = sfbTab[pi->startSFB];
		for (i = 0; i < pi->numPulse; i++) {
			offset += pi->offset[i];
			if (coef[offset] > 0)
				coef[offset] += pi->amp[i];
			else
				coef[offset] -= pi->amp[i];
		}
		ASSERT(offset < NSAMPS_LONG);
	}
}

/**************************************************************************************
 * Function:    DecodeSpectrumShort
 *
 * Description: decode transform coefficients for frame with eight short blocks
 *
 * Inputs:      platform specific info struct
 *              BitStreamInfo struct pointing to start of spectral data
 *                (14496-3, table 4.4.29) 
 *              index of current channel
 *
 * Outputs:     decoded, quantized coefficients for this channel
 *
 * Return:      none
 *
 * Notes:       fills coefficient buffer with zeros in any region not coded with
 *                codebook in range [1, 11] (including sfb's above sfbMax)
 *              deinterleaves window groups into 8 windows
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ void DecodeSpectrumShort(PSInfoBase *psi, BitStreamInfo *bsi, int ch)
{
	int gp, cb, nVals=0, win, offset, sfb;
	const /*short*/ int *sfbTab;
	unsigned char *sfbCodeBook;
	int *coef;
	ICSInfo *icsInfo;

	coef = psi->coef[ch];
	icsInfo = (ch == 1 && psi->commonWin == 1) ? &(psi->icsInfo[0]) : &(psi->icsInfo[ch]);

	/* decode short blocks, deinterleaving in-place */
	sfbTab = sfBandTabShort + sfBandTabShortOffset[psi->sampRateIdx];
	sfbCodeBook = psi->sfbCodeBook[ch];
	for (gp = 0; gp < icsInfo->numWinGroup; gp++) {
		for (sfb = 0; sfb < icsInfo->maxSFB; sfb++) {
			nVals = sfbTab[sfb+1] - sfbTab[sfb];
			cb = *sfbCodeBook++;

			for (win = 0; win < icsInfo->winGroupLen[gp]; win++) {
				offset = win*NSAMPS_SHORT;
				if (cb == 0)
					UnpackZeros(nVals, coef + offset);
				else if (cb <= 4)
					UnpackQuads(bsi, cb, nVals, coef + offset);
				else if (cb <= 10)
					UnpackPairsNoEsc(bsi, cb, nVals, coef + offset);
				else if (cb == 11)
					UnpackPairsEsc(bsi, cb, nVals, coef + offset);
				else 
					UnpackZeros(nVals, coef + offset);
			}
			coef += nVals;
		}

		/* fill with zeros above maxSFB */
		for (win = 0; win < icsInfo->winGroupLen[gp]; win++) {
			offset = win*NSAMPS_SHORT;
			nVals = NSAMPS_SHORT - sfbTab[sfb];
			UnpackZeros(nVals, coef + offset);
		}
		coef += nVals;
		coef += (icsInfo->winGroupLen[gp] - 1)*NSAMPS_SHORT;
	}

	ASSERT(coef == psi->coef[ch] + NSAMPS_LONG);
}
