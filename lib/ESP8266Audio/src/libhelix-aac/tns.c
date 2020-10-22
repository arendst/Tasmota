/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: tns.c,v 1.2 2005/05/24 16:01:55 albertofloyd Exp $ 
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
 * tns.c - apply TNS to spectrum
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"

#define FBITS_LPC_COEFS	20

//fb
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"

/* inverse quantization tables for TNS filter coefficients, format = Q31
 * see bottom of file for table generation
 * negative (vs. spec) since we use MADD for filter kernel
 */
static const int invQuant3[16] PROGMEM = {
	0x00000000, 0xc8767f65, 0x9becf22c, 0x83358feb, 0x83358feb, 0x9becf22c, 0xc8767f65, 0x00000000,
	0x2bc750e9, 0x5246dd49, 0x6ed9eba1, 0x7e0e2e32, 0x7e0e2e32, 0x6ed9eba1, 0x5246dd49, 0x2bc750e9,
};

static const int invQuant4[16] PROGMEM = {
	0x00000000, 0xe5632654, 0xcbf00dbe, 0xb4c373ee, 0xa0e0a15f, 0x9126145f, 0x8643c7b3, 0x80b381ac,
	0x7f7437ad, 0x7b1d1a49, 0x7294b5f2, 0x66256db2, 0x563ba8aa, 0x4362210e, 0x2e3d2abb, 0x17851aad,
};

#pragma GCC diagnostic pop

/**************************************************************************************
 * Function:    DecodeLPCCoefs
 *
 * Description: decode LPC coefficients for TNS
 *
 * Inputs:      order of TNS filter
 *              resolution of coefficients (3 or 4 bits)
 *              coefficients unpacked from bitstream
 *              scratch buffer (b) of size >= order
 *
 * Outputs:     LPC coefficients in Q(FBITS_LPC_COEFS), in 'a'
 *
 * Return:      none
 * 
 * Notes:       assumes no guard bits in input transform coefficients
 *              a[i] = Q(FBITS_LPC_COEFS), don't store a0 = 1.0 
 *                (so a[0] = first delay tap, etc.) 
 *              max abs(a[i]) < log2(order), so for max order = 20 a[i] < 4.4 
 *                (up to 3 bits of gain) so a[i] has at least 31 - FBITS_LPC_COEFS - 3 
 *                guard bits
 *              to ensure no intermediate overflow in all-pole filter, set 
 *                FBITS_LPC_COEFS such that number of guard bits >= log2(max order)
 **************************************************************************************/
static void DecodeLPCCoefs(int order, int res, signed char *filtCoef, int *a, int *b)
{
	int i, m, t;
	const int *invQuantTab;

	if (res == 3)			invQuantTab = invQuant3;
	else if (res == 4)		invQuantTab = invQuant4;
	else					return;

	for (m = 0; m < order; m++) {
		t = invQuantTab[filtCoef[m] & 0x0f];	/* t = Q31 */
		for (i = 0; i < m; i++)
			b[i] = a[i] - (MULSHIFT32(t, a[m-i-1]) << 1);
		for (i = 0; i < m; i++) 
			a[i] = b[i];
		a[m] = t >> (31 - FBITS_LPC_COEFS);
	}
}

/**************************************************************************************
 * Function:    FilterRegion
 *
 * Description: apply LPC filter to one region of coefficients
 *
 * Inputs:      number of transform coefficients in this region
 *              direction flag (forward = 1, backward = -1)
 *              order of filter
 *              'size' transform coefficients
 *              'order' LPC coefficients in Q(FBITS_LPC_COEFS)
 *              scratch buffer for history (must be >= order samples long)
 *
 * Outputs:     filtered transform coefficients
 *
 * Return:      guard bit mask (OR of abs value of all filtered transform coefs)
 * 
 * Notes:       assumes no guard bits in input transform coefficients
 *              gains 0 int bits
 *              history buffer does not need to be preserved between regions
 **************************************************************************************/
static int FilterRegion(int size, int dir, int order, int *audioCoef, int *a, int *hist)
{
	int i, j, y, hi32, inc, gbMask;
	U64 sum64;

	/* init history to 0 every time */
	for (i = 0; i < order; i++)
		hist[i] = 0;

	sum64.w64 = 0;     /* avoid warning */
	gbMask = 0;
	inc = (dir ? -1 : 1);
	do {
		/* sum64 = a0*y[n] = 1.0*y[n] */
		y = *audioCoef;
		sum64.r.hi32 = y >> (32 - FBITS_LPC_COEFS);
		sum64.r.lo32 = y << FBITS_LPC_COEFS;
		 
		/* sum64 += (a1*y[n-1] + a2*y[n-2] + ... + a[order-1]*y[n-(order-1)]) */
        for (j = order - 1; j > 0; j--) {
			sum64.w64 = MADD64(sum64.w64, hist[j], a[j]);
            hist[j] = hist[j-1];
		}
		sum64.w64 = MADD64(sum64.w64, hist[0], a[0]);
		y = (sum64.r.hi32 << (32 - FBITS_LPC_COEFS)) | (sum64.r.lo32 >> FBITS_LPC_COEFS);

		/* clip output (rare) */
		hi32 = sum64.r.hi32;
		if ((hi32 >> 31) != (hi32 >> (FBITS_LPC_COEFS-1)))
			y = (hi32 >> 31) ^ 0x7fffffff;

		hist[0] = y;
		*audioCoef = y;
		audioCoef += inc;
		gbMask |= FASTABS(y);
	} while (--size);

	return gbMask;
}

/**************************************************************************************
 * Function:    TNSFilter
 *
 * Description: apply temporal noise shaping, if enabled
 *
 * Inputs:      valid AACDecInfo struct
 *              index of current channel
 *
 * Outputs:     updated transform coefficients
 *              updated minimum guard bit count for this channel
 *
 * Return:      0 if successful, -1 if error
 **************************************************************************************/
int TNSFilter(AACDecInfo *aacDecInfo, int ch)
{
	int win, winLen, nWindows, nSFB, filt, bottom, top, order, maxOrder, dir;
	int start, end, size, tnsMaxBand, numFilt, gbMask;
	int *audioCoef;
	unsigned char *filtLength, *filtOrder, *filtRes, *filtDir; 
	signed char *filtCoef;
	const unsigned /*char*/ int *tnsMaxBandTab;
	const /*short*/ int *sfbTab;
	ICSInfo *icsInfo;
	TNSInfo *ti;
	PSInfoBase *psi;

	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return -1;
	psi = (PSInfoBase *)(aacDecInfo->psInfoBase);
	icsInfo = (ch == 1 && psi->commonWin == 1) ? &(psi->icsInfo[0]) : &(psi->icsInfo[ch]);
	ti = &psi->tnsInfo[ch];

	if (!ti->tnsDataPresent)
		return 0;

	if (icsInfo->winSequence == 2) {
		nWindows = NWINDOWS_SHORT;
		winLen = NSAMPS_SHORT;
		nSFB = sfBandTotalShort[psi->sampRateIdx];
		maxOrder = tnsMaxOrderShort[aacDecInfo->profile];
		sfbTab = sfBandTabShort + sfBandTabShortOffset[psi->sampRateIdx];
		tnsMaxBandTab = tnsMaxBandsShort + tnsMaxBandsShortOffset[aacDecInfo->profile];
		tnsMaxBand = tnsMaxBandTab[psi->sampRateIdx];
	} else {
		nWindows = NWINDOWS_LONG;
		winLen = NSAMPS_LONG;
		nSFB = sfBandTotalLong[psi->sampRateIdx];
		maxOrder = tnsMaxOrderLong[aacDecInfo->profile];
		sfbTab = sfBandTabLong + sfBandTabLongOffset[psi->sampRateIdx];
		tnsMaxBandTab = tnsMaxBandsLong + tnsMaxBandsLongOffset[aacDecInfo->profile];
		tnsMaxBand = tnsMaxBandTab[psi->sampRateIdx];
	}

	if (tnsMaxBand > icsInfo->maxSFB)
		tnsMaxBand = icsInfo->maxSFB;

	filtRes =    ti->coefRes;
	filtLength = ti->length;
	filtOrder =  ti->order;
	filtDir =    ti->dir;
	filtCoef =   ti->coef;

	gbMask = 0;
	audioCoef =  psi->coef[ch];
	for (win = 0; win < nWindows; win++) {
		bottom = nSFB;
		numFilt = ti->numFilt[win];
		for (filt = 0; filt < numFilt; filt++) {
			top = bottom;
			bottom = top - *filtLength++;
			bottom = MAX(bottom, 0);
			order = *filtOrder++;
			order = MIN(order, maxOrder);

			if (order) {
				start = sfbTab[MIN(bottom, tnsMaxBand)];
				end   = sfbTab[MIN(top, tnsMaxBand)];
				size = end - start;
				if (size > 0) {
					dir = *filtDir++;
					if (dir)
						start = end - 1;

					DecodeLPCCoefs(order, filtRes[win], filtCoef, psi->tnsLPCBuf, psi->tnsWorkBuf);
					gbMask |= FilterRegion(size, dir, order, audioCoef + start, psi->tnsLPCBuf, psi->tnsWorkBuf);
				}
				filtCoef += order;
			}
		}
		audioCoef += winLen;
	}

	/* update guard bit count if necessary */
	size = CLZ(gbMask) - 1;
	if (psi->gbCurrent[ch] > size)
		psi->gbCurrent[ch] = size;

	return 0;
}

/*	Code to generate invQuantXXX[] tables
 *  {
 *    int res, i, t;
 *    double powScale, iqfac, iqfac_m, d;
 *
 *    powScale = pow(2.0, 31) * -1.0;	/ ** make coefficients negative for using MADD in kernel ** /
 *    for (res = 3; res <= 4; res++) {
 *      iqfac =   ( ((1 << (res-1)) - 0.5) * (2.0 / M_PI) );
 *      iqfac_m = ( ((1 << (res-1)) + 0.5) * (2.0 / M_PI) );
 *      printf("static const int invQuant%d[16] = {\n", res);
 *      for (i = 0; i < 16; i++) {
 *        / ** extend bottom 4 bits into signed, 2's complement number ** /
 *        t = (i << 28) >> 28;
 *
 *        if (t >= 0)   d = sin(t / iqfac);
 *        else          d = sin(t / iqfac_m);
 *
 *        d *= powScale;
 *        printf("0x%08x, ", (int)(d > 0 ? d + 0.5 : d - 0.5));
 *        if ((i & 0x07) == 0x07)
 *           printf("\n");
 *      }
 *      printf("};\n\n");
 *	  }
 *  }
 */

