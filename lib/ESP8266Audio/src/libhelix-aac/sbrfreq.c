/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: sbrfreq.c,v 1.2 2005/05/20 18:05:41 jrecker Exp $ 
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
 * sbrfreq.c - frequency band table calculation for SBR
 **************************************************************************************/

#include "sbr.h"
#include "assembly.h"

/**************************************************************************************
 * Function:    BubbleSort
 *
 * Description: in-place sort of unsigned chars
 *
 * Inputs:      buffer of elements to sort
 *              number of elements to sort
 *
 * Outputs:     sorted buffer
 *
 * Return:      none
 **************************************************************************************/
static void BubbleSort(unsigned char *v, int nItems)
{
	int i;
	unsigned char t;

	while (nItems >= 2) {
		for (i = 0; i < nItems-1; i++) {
			if (v[i+1] < v[i]) {
				t = v[i+1];	
				v[i+1] = v[i];	
				v[i] = t;
			}
		}
		nItems--;
	}
}

/**************************************************************************************
 * Function:    VMin
 *
 * Description: find smallest element in a buffer of unsigned chars
 *
 * Inputs:      buffer of elements to search
 *              number of elements to search
 *
 * Outputs:     none
 *
 * Return:      smallest element in buffer
 **************************************************************************************/
static unsigned char VMin(unsigned char *v, int nItems)
{
	int i;
	unsigned char vMin;

	vMin = v[0];
	for (i = 1; i < nItems; i++) {
		if (v[i] < vMin)
			vMin = v[i];
	}
	return vMin;
}

/**************************************************************************************
 * Function:    VMax
 *
 * Description: find largest element in a buffer of unsigned chars
 *
 * Inputs:      buffer of elements to search
 *              number of elements to search
 *
 * Outputs:     none
 *
 * Return:      largest element in buffer
 **************************************************************************************/
static unsigned char VMax(unsigned char *v, int nItems)
{
	int i;
	unsigned char vMax;

	vMax = v[0];
	for (i = 1; i < nItems; i++) {
		if (v[i] > vMax)
			vMax = v[i];
	}
	return vMax;
}

/**************************************************************************************
 * Function:    CalcFreqMasterScaleZero
 *
 * Description: calculate master frequency table when freqScale == 0 
 *                (4.6.18.3.2.1, figure 4.39)
 *
 * Inputs:      alterScale flag
 *              index of first QMF subband in master freq table (k0)
 *              index of last QMF subband (k2)
 *
 * Outputs:     master frequency table
 *
 * Return:      number of bands in master frequency table
 *
 * Notes:       assumes k2 - k0 <= 48 and k2 >= k0 (4.6.18.3.6)
 **************************************************************************************/
static int CalcFreqMasterScaleZero(unsigned char *freqMaster, int alterScale, int k0, int k2)
{
	int nMaster, k, nBands, k2Achieved, dk, vDk[64], k2Diff;

	if (alterScale) {
		dk = 2;
		nBands = 2 * ((k2 - k0 + 2) >> 2);
	} else {
		dk = 1;
		nBands = 2 * ((k2 - k0) >> 1);
	}

	if (nBands <= 0)
		return 0;

	k2Achieved = k0 + nBands * dk;
	k2Diff = k2 - k2Achieved;
	for (k = 0; k < nBands; k++)
		vDk[k] = dk;

	if (k2Diff > 0) {
		k = nBands - 1;
		while (k2Diff) {
			vDk[k]++;
			k--;
			k2Diff--;
		}
	} else if (k2Diff < 0) {
		k = 0;
		while (k2Diff) {
			vDk[k]--;
			k++;
			k2Diff++;
		}
	}

	nMaster = nBands;
	freqMaster[0] = k0;
	for (k = 1; k <= nBands; k++)
		freqMaster[k] = freqMaster[k-1] + vDk[k-1];

	return nMaster;
}

/* mBandTab[i] = temp1[i] / 2 */
static const int mBandTab[3] PROGMEM = {6, 5, 4};

/* invWarpTab[i] = 1.0 / temp2[i], Q30 (see 4.6.18.3.2.1) */
static const int invWarpTab[2] PROGMEM = {0x40000000, 0x313b13b1};

/**************************************************************************************
 * Function:    CalcFreqMasterScale
 *
 * Description: calculate master frequency table when freqScale > 0 
 *                (4.6.18.3.2.1, figure 4.39)
 *
 * Inputs:      alterScale flag
 *              freqScale flag
 *              index of first QMF subband in master freq table (k0)
 *              index of last QMF subband (k2)
 *
 * Outputs:     master frequency table
 *
 * Return:      number of bands in master frequency table
 *
 * Notes:       assumes k2 - k0 <= 48 and k2 >= k0 (4.6.18.3.6)
 **************************************************************************************/
static int CalcFreqMaster(unsigned char *freqMaster, int freqScale, int alterScale, int k0, int k2)
{
	int bands, twoRegions, k, k1, t, vLast, vCurr, pCurr;
	int invWarp, nBands0, nBands1, change;
	unsigned char vDk1Min, vDk0Max;
	unsigned char *vDelta;

	if (freqScale < 1 || freqScale > 3)
		return -1;

	bands = mBandTab[freqScale - 1];
	invWarp = invWarpTab[alterScale];

	/* tested for all k0 = [5, 64], k2 = [k0, 64] */
	if (k2*10000 > 22449*k0) {
		twoRegions = 1;
		k1 = 2*k0;
	} else {
		twoRegions = 0;
		k1 = k2;
	}
	
	/* tested for all k0 = [5, 64], k1 = [k0, 64], freqScale = [1,3] */
	t = (log2Tab[k1] - log2Tab[k0]) >> 3;				/* log2(k1/k0), Q28 to Q25 */
	nBands0 = 2 * (((bands * t) + (1 << 24)) >> 25);	/* multiply by bands/2, round to nearest int (mBandTab has factor of 1/2 rolled in) */

	/* tested for all valid combinations of k0, k1, nBands (from sampRate, freqScale, alterScale) 
	 * roundoff error can be a problem with fixpt (e.g. pCurr = 12.499999 instead of 12.50003)
	 *   because successive multiplication always undershoots a little bit, but this
	 *   doesn't occur in any of the ratios we encounter from the valid k0/k1 bands in the spec
	 */
	t = RatioPowInv(k1, k0, nBands0);
	pCurr = k0 << 24;
	vLast = k0;
	vDelta = freqMaster + 1;	/* operate in-place */
	for (k = 0; k < nBands0; k++) {
		pCurr = MULSHIFT32(pCurr, t) << 8;	/* keep in Q24 */
		vCurr = (pCurr + (1 << 23)) >> 24;
		vDelta[k] = (vCurr - vLast);
		vLast = vCurr;
	}

	/* sort the deltas and find max delta for first region */
	BubbleSort(vDelta, nBands0);
	vDk0Max = VMax(vDelta, nBands0);

	/* fill master frequency table with bands from first region */
	freqMaster[0] = k0;
	for (k = 1; k <= nBands0; k++)
		freqMaster[k] += freqMaster[k-1];

	/* if only one region, then the table is complete */
	if (!twoRegions)
		return nBands0;

	/* tested for all k1 = [10, 64], k2 = [k0, 64], freqScale = [1,3] */
	t = (log2Tab[k2] - log2Tab[k1]) >> 3;		/* log2(k1/k0), Q28 to Q25 */
	t = MULSHIFT32(bands * t, invWarp) << 2;	/* multiply by bands/2, divide by warp factor, keep Q25 */
	nBands1 = 2 * ((t + (1 << 24)) >> 25);		/* round to nearest int */
				
	/* see comments above for calculations in first region */
	t = RatioPowInv(k2, k1, nBands1);
	pCurr = k1 << 24;
	vLast = k1;
	vDelta = freqMaster + nBands0 + 1;	/* operate in-place */
	for (k = 0; k < nBands1; k++) {
		pCurr = MULSHIFT32(pCurr, t) << 8;	/* keep in Q24 */
		vCurr = (pCurr + (1 << 23)) >> 24;
		vDelta[k] = (vCurr - vLast);
		vLast = vCurr;
	}

	/* sort the deltas, adjusting first and last if the second region has smaller deltas than the first */
	vDk1Min = VMin(vDelta, nBands1);
	if (vDk1Min < vDk0Max) {
		BubbleSort(vDelta, nBands1);
		change = vDk0Max - vDelta[0];
		if (change > ((vDelta[nBands1 - 1] - vDelta[0]) >> 1))
			 change = ((vDelta[nBands1 - 1] - vDelta[0]) >> 1);
		vDelta[0] += change;
		vDelta[nBands1-1] -= change;
	}
	BubbleSort(vDelta, nBands1);

	/* fill master frequency table with bands from second region 
	 * Note: freqMaster[nBands0] = k1
	 */
	for (k = 1; k <= nBands1; k++)
		freqMaster[k + nBands0] += freqMaster[k + nBands0 - 1];

	return (nBands0 + nBands1);
}

/**************************************************************************************
 * Function:    CalcFreqHigh
 *
 * Description: calculate high resolution frequency table (4.6.18.3.2.2)
 *
 * Inputs:      master frequency table
 *              number of bands in master frequency table 
 *              crossover band from header
 *
 * Outputs:     high resolution frequency table
 *
 * Return:      number of bands in high resolution frequency table
 **************************************************************************************/
static int CalcFreqHigh(unsigned char *freqHigh, unsigned char *freqMaster, int nMaster, int crossOverBand)
{
	int k, nHigh;

	nHigh = nMaster - crossOverBand;

	for (k = 0; k <= nHigh; k++)
		freqHigh[k] = freqMaster[k + crossOverBand];
	
	return nHigh;
}

/**************************************************************************************
 * Function:    CalcFreqLow
 *
 * Description: calculate low resolution frequency table (4.6.18.3.2.2)
 *
 * Inputs:      high resolution frequency table
 *              number of bands in high resolution frequency table 
 *
 * Outputs:     low resolution frequency table
 *
 * Return:      number of bands in low resolution frequency table
 **************************************************************************************/
static int CalcFreqLow(unsigned char *freqLow, unsigned char *freqHigh, int nHigh)
{
	int k, nLow, oddFlag;

	nLow = nHigh - (nHigh >> 1);
	freqLow[0] = freqHigh[0];
	oddFlag = nHigh & 0x01;

	for (k = 1; k <= nLow; k++)
		freqLow[k] = freqHigh[2*k - oddFlag];

	return nLow;
}

/**************************************************************************************
 * Function:    CalcFreqNoise
 *
 * Description: calculate noise floor frequency table (4.6.18.3.2.2)
 *
 * Inputs:      low resolution frequency table
 *              number of bands in low resolution frequency table
 *              index of starting QMF subband for SBR (kStart)
 *              index of last QMF subband (k2)
 *              number of noise bands
 *
 * Outputs:     noise floor frequency table
 *
 * Return:      number of bands in noise floor frequency table
 **************************************************************************************/
static int CalcFreqNoise(unsigned char *freqNoise, unsigned char *freqLow, int nLow, int kStart, int k2, int noiseBands)
{
	int i, iLast, k, nQ, lTop, lBottom;

	lTop = log2Tab[k2];
	lBottom = log2Tab[kStart];
	nQ = noiseBands*((lTop - lBottom) >> 2);	/* Q28 to Q26, noiseBands = [0,3] */
	nQ = (nQ + (1 << 25)) >> 26;
	if (nQ < 1)
		nQ = 1;

	ASSERT(nQ <= MAX_NUM_NOISE_FLOOR_BANDS);	/* required from 4.6.18.3.6 */

	iLast = 0;
	freqNoise[0] = freqLow[0];
	for (k = 1; k <= nQ; k++) {
		i = iLast + (nLow - iLast) / (nQ + 1 - k);	/* truncating division */
		freqNoise[k] = freqLow[i];
		iLast = i;
	}

	return nQ;
}

/**************************************************************************************
 * Function:    BuildPatches
 *
 * Description: build high frequency patches (4.6.18.6.3)
 *
 * Inputs:      master frequency table
 *              number of bands in low resolution frequency table
 *              index of first QMF subband in master freq table (k0)
 *              index of starting QMF subband for SBR (kStart)
 *              number of QMF bands in high resolution frequency table
 *              sample rate index
 *
 * Outputs:     starting subband for each patch
 *              number of subbands in each patch
 *
 * Return:      number of patches
 **************************************************************************************/
static int BuildPatches(unsigned char *patchNumSubbands, unsigned char *patchStartSubband, unsigned char *freqMaster, 
						int nMaster, int k0, int kStart, int numQMFBands, int sampRateIdx)
{
	int i, j, k;
	int msb, sb, usb, numPatches, goalSB, oddFlag;

	msb = k0;
	usb = kStart;
	numPatches = 0;
	goalSB = goalSBTab[sampRateIdx];

	if (nMaster == 0) {
		patchNumSubbands[0] = 0;
		patchStartSubband[0] = 0;
		return 0;
	}

	if (goalSB < kStart + numQMFBands) {
		k = 0;
		for (i = 0; freqMaster[i] < goalSB; i++)
			k = i+1;
	} else {
		k = nMaster;
	}

	do {
		j = k+1;
		do {
			j--;
			sb = freqMaster[j];
			oddFlag = (sb - 2 + k0) & 0x01;
		} while (sb > k0 - 1 + msb - oddFlag);

		patchNumSubbands[numPatches] = MAX(sb - usb, 0);
		patchStartSubband[numPatches] = k0 - oddFlag - patchNumSubbands[numPatches];

		/* from MPEG reference code - slightly different from spec */
		if ((patchNumSubbands[numPatches] < 3) && (numPatches > 0))
			break;

		if (patchNumSubbands[numPatches] > 0) {
			usb = sb;
			msb = sb;
			numPatches++;
		} else {
			msb = kStart;
		}

		if (freqMaster[k] - sb < 3)
			k = nMaster;

	} while (sb != (kStart + numQMFBands) && numPatches <= MAX_NUM_PATCHES);

	return numPatches;
}

/**************************************************************************************
 * Function:    FindFreq
 *
 * Description: search buffer of unsigned chars for a specific value
 *
 * Inputs:      buffer of elements to search
 *              number of elements to search
 *              value to search for
 *
 * Outputs:     none
 *
 * Return:      non-zero if the value is found anywhere in the buffer, zero otherwise
 **************************************************************************************/
static int FindFreq(unsigned char *freq, int nFreq, unsigned char val)
{
	int k;

	for (k = 0; k < nFreq; k++) {
		if (freq[k] == val)
			return 1;
	}

	return 0;
}

/**************************************************************************************
 * Function:    RemoveFreq
 *
 * Description: remove one element from a buffer of unsigned chars
 *
 * Inputs:      buffer of elements
 *              number of elements
 *              index of element to remove
 *
 * Outputs:     new buffer of length nFreq-1
 *
 * Return:      none
 **************************************************************************************/
static void RemoveFreq(unsigned char *freq, int nFreq, int removeIdx)
{
	int k;

	if (removeIdx >= nFreq)
		return;

	for (k = removeIdx; k < nFreq - 1; k++)
		freq[k] = freq[k+1];
}

/**************************************************************************************
 * Function:    CalcFreqLimiter
 *
 * Description: calculate limiter frequency table (4.6.18.3.2.3)
 *
 * Inputs:      number of subbands in each patch
 *              low resolution frequency table
 *              number of bands in low resolution frequency table
 *              index of starting QMF subband for SBR (kStart)
 *              number of limiter bands
 *              number of patches
 *
 * Outputs:     limiter frequency table
 *
 * Return:      number of bands in limiter frequency table
 **************************************************************************************/
static int CalcFreqLimiter(unsigned char *freqLimiter, unsigned char *patchNumSubbands, unsigned char *freqLow, 
						   int nLow, int kStart, int limiterBands, int numPatches)
{
	int k, bands, nLimiter, nOctaves;
	int limBandsPerOctave[3] = {120, 200, 300};		/* [1.2, 2.0, 3.0] * 100 */
	unsigned char patchBorders[MAX_NUM_PATCHES + 1];

	/* simple case */
	if (limiterBands == 0) {
		freqLimiter[0] = freqLow[0] - kStart;
		freqLimiter[1] = freqLow[nLow] - kStart;
		return 1;
	}

	bands = limBandsPerOctave[limiterBands - 1];
	patchBorders[0] = kStart;

	/* from MPEG reference code - slightly different from spec (top border) */
	for (k = 1; k < numPatches; k++)
		patchBorders[k] = patchBorders[k-1] + patchNumSubbands[k-1];
	patchBorders[k] = freqLow[nLow];

	for (k = 0; k <= nLow; k++)
		freqLimiter[k] = freqLow[k];

	for (k = 1; k < numPatches; k++)
		freqLimiter[k+nLow] = patchBorders[k];

	k = 1;
	nLimiter = nLow + numPatches - 1;
	BubbleSort(freqLimiter, nLimiter + 1);

	while (k <= nLimiter) {
		nOctaves = log2Tab[freqLimiter[k]] - log2Tab[freqLimiter[k-1]];	/* Q28 */
		nOctaves = (nOctaves >> 9) * bands;	/* Q19, max bands = 300 < 2^9 */
		if (nOctaves < (49 << 19)) {		/* compare with 0.49*100, in Q19 */
			if (freqLimiter[k] == freqLimiter[k-1] || FindFreq(patchBorders, numPatches + 1, freqLimiter[k]) == 0) {
				RemoveFreq(freqLimiter, nLimiter + 1, k);
				nLimiter--;
			} else if (FindFreq(patchBorders, numPatches + 1, freqLimiter[k-1]) == 0) {
				RemoveFreq(freqLimiter, nLimiter + 1, k-1);
				nLimiter--;
			} else {
				k++;
			}
		} else {
			k++;
		}
	}

	/* store limiter boundaries as offsets from kStart */
	for (k = 0; k <= nLimiter; k++)
		freqLimiter[k] -= kStart;

	return nLimiter;
}

/**************************************************************************************
 * Function:    CalcFreqTables
 *
 * Description: calulate master and derived frequency tables, and patches
 *
 * Inputs:      initialized SBRHeader struct for this SCE/CPE block
 *              initialized SBRFreq struct for this SCE/CPE block
 *              sample rate index of output sample rate (after SBR)
 *
 * Outputs:     master and derived frequency tables, and patches
 *
 * Return:      non-zero if error, zero otherwise
 **************************************************************************************/
int CalcFreqTables(SBRHeader *sbrHdr, SBRFreq *sbrFreq, int sampRateIdx)
{
	int k0, k2;

	k0 = k0Tab[sampRateIdx][sbrHdr->startFreq];

	if (sbrHdr->stopFreq == 14)
		k2 = 2*k0;
	else if (sbrHdr->stopFreq == 15)
		k2 = 3*k0;
	else
		k2 = k2Tab[sampRateIdx][sbrHdr->stopFreq];
	if (k2 > 64)
		k2 = 64;

	/* calculate master frequency table */
	if (sbrHdr->freqScale == 0)
		sbrFreq->nMaster = CalcFreqMasterScaleZero(sbrFreq->freqMaster, sbrHdr->alterScale, k0, k2);
	else
		sbrFreq->nMaster = CalcFreqMaster(sbrFreq->freqMaster, sbrHdr->freqScale, sbrHdr->alterScale, k0, k2);

	/* calculate high frequency table and related parameters */
	sbrFreq->nHigh = CalcFreqHigh(sbrFreq->freqHigh, sbrFreq->freqMaster, sbrFreq->nMaster, sbrHdr->crossOverBand);
	sbrFreq->numQMFBands = sbrFreq->freqHigh[sbrFreq->nHigh] - sbrFreq->freqHigh[0];
	sbrFreq->kStart = sbrFreq->freqHigh[0];

	/* calculate low frequency table */
	sbrFreq->nLow = CalcFreqLow(sbrFreq->freqLow, sbrFreq->freqHigh, sbrFreq->nHigh);

	/* calculate noise floor frequency table */
	sbrFreq->numNoiseFloorBands = CalcFreqNoise(sbrFreq->freqNoise, sbrFreq->freqLow, sbrFreq->nLow, sbrFreq->kStart, k2, sbrHdr->noiseBands);

	/* calculate limiter table */
	sbrFreq->numPatches = BuildPatches(sbrFreq->patchNumSubbands, sbrFreq->patchStartSubband, sbrFreq->freqMaster, 
		sbrFreq->nMaster, k0, sbrFreq->kStart, sbrFreq->numQMFBands, sampRateIdx);
	sbrFreq->nLimiter = CalcFreqLimiter(sbrFreq->freqLimiter, sbrFreq->patchNumSubbands, sbrFreq->freqLow, sbrFreq->nLow, sbrFreq->kStart,
		sbrHdr->limiterBands, sbrFreq->numPatches);

	return 0;
}
