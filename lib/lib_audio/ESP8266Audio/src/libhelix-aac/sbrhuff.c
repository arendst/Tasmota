/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: sbrhuff.c,v 1.1 2005/02/26 01:47:35 jrecker Exp $ 
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
 * sbrhuff.c - functions for unpacking Huffman-coded envelope and noise data
 **************************************************************************************/

#include "sbr.h"
#include "assembly.h"

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
static int DecodeHuffmanScalar(const signed /*short*/ int *huffTab, const HuffInfo *huffTabInfo, unsigned int bitBuf, signed int *val)
{
    unsigned int count, start, shift, t;
	const unsigned int /*char*/ *countPtr;
	const signed int /*short*/ *map;

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
	
	*val = (signed int)map[t];
	return (countPtr - huffTabInfo->count);
}

/**************************************************************************************
 * Function:    DecodeOneSymbol
 *
 * Description: dequantize one Huffman symbol from bitstream, 
 *                using table huffTabSBR[huffTabIndex]
 *
 * Inputs:      BitStreamInfo struct pointing to start of next Huffman codeword
 *              index of Huffman table
 *
 * Outputs:     bitstream advanced by number of bits in codeword
 *
 * Return:      one decoded symbol
 **************************************************************************************/
static int DecodeOneSymbol(BitStreamInfo *bsi, int huffTabIndex)
{
	int nBits, val;
	unsigned int bitBuf;
	const HuffInfo *hi;

	hi = &(huffTabSBRInfo[huffTabIndex]);

	bitBuf = GetBitsNoAdvance(bsi, hi->maxBits) << (32 - hi->maxBits);
	nBits = DecodeHuffmanScalar(huffTabSBR, hi, bitBuf, &val);
	AdvanceBitstream(bsi, nBits);
	
	return val;
}

/* [1.0, sqrt(2)], format = Q29 (one guard bit for decoupling) */
static const int envDQTab[2] PROGMEM = {0x20000000, 0x2d413ccc};

/**************************************************************************************
 * Function:    DequantizeEnvelope
 *
 * Description: dequantize envelope scalefactors
 *
 * Inputs:      number of scalefactors to process
 *              amplitude resolution flag for this frame (0 or 1)
 *              quantized envelope scalefactors
 * 
 * Outputs:     dequantized envelope scalefactors
 *
 * Return:      extra int bits in output (6 + expMax)
 *              in other words, output format = Q(FBITS_OUT_DQ_ENV - (6 + expMax))
 *
 * Notes:       dequantized scalefactors have at least 2 GB
 **************************************************************************************/
static int DequantizeEnvelope(int nBands, int ampRes, signed char *envQuant, int *envDequant)
{
	int exp, expMax, i, scalei;

	if (nBands <= 0)
		return 0;
	
	/* scan for largest dequant value (do separately from envelope decoding to keep code cleaner) */
	expMax = 0;
	for (i = 0; i < nBands; i++) {
		if (envQuant[i] > expMax)
			expMax = envQuant[i];
	}

	/* dequantized envelope gains
	 *   envDequant = 64*2^(envQuant / alpha) = 2^(6 + envQuant / alpha)
	 *     if ampRes == 0, alpha = 2 and range of envQuant = [0, 127]
	 *     if ampRes == 1, alpha = 1 and range of envQuant = [0, 63]
	 * also if coupling is on, envDequant is scaled by something in range [0, 2]
	 * so range of envDequant = [2^6, 2^69] (no coupling), [2^6, 2^70] (with coupling)
	 * 
	 * typical range (from observation) of envQuant/alpha = [0, 27] --> largest envQuant ~= 2^33
	 * output: Q(29 - (6 + expMax))
	 *
	 * reference: 14496-3:2001(E)/4.6.18.3.5 and 14496-4:200X/FPDAM8/5.6.5.1.2.1.5
	 */
	if (ampRes) {
		do {
			exp = *envQuant++;
			scalei = MIN(expMax - exp, 31);
			*envDequant++ = envDQTab[0] >> scalei;
		} while (--nBands);

		return (6 + expMax);
	} else {
		expMax >>= 1;
		do {
			exp = *envQuant++;
			scalei = MIN(expMax - (exp >> 1), 31);
			*envDequant++ = envDQTab[exp & 0x01] >> scalei;
		} while (--nBands);

		return (6 + expMax);
	}

}

/**************************************************************************************
 * Function:    DequantizeNoise
 *
 * Description: dequantize noise scalefactors
 *
 * Inputs:      number of scalefactors to process
 *              quantized noise scalefactors
 * 
 * Outputs:     dequantized noise scalefactors, format = Q(FBITS_OUT_DQ_NOISE)
 *
 * Return:      none
 *
 * Notes:       dequantized scalefactors have at least 2 GB
 **************************************************************************************/
static void DequantizeNoise(int nBands, signed char *noiseQuant, int *noiseDequant)
{
	int exp, scalei;
	
	if (nBands <= 0)
		return;

	/* dequantize noise floor gains (4.6.18.3.5):
	 *   noiseDequant = 2^(NOISE_FLOOR_OFFSET - noiseQuant)
	 *
	 * range of noiseQuant = [0, 30] (see 4.6.18.3.6), NOISE_FLOOR_OFFSET = 6
	 *   so range of noiseDequant = [2^-24, 2^6]
	 */
	do {
		exp = *noiseQuant++;
		scalei = NOISE_FLOOR_OFFSET - exp + FBITS_OUT_DQ_NOISE;	/* 6 + 24 - exp, exp = [0,30] */

		if (scalei < 0)
			*noiseDequant++ = 0;
		else if (scalei < 30)
			*noiseDequant++ = 1 << scalei;
		else
			*noiseDequant++ = 0x3fffffff;	/* leave 2 GB */

	} while (--nBands);
}

/**************************************************************************************
 * Function:    DecodeSBREnvelope
 *
 * Description: decode delta Huffman coded envelope scalefactors from bitstream
 *
 * Inputs:      BitStreamInfo struct pointing to start of env data
 *              initialized PSInfoSBR struct
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              initialized SBRChan struct for this channel
 *              index of current channel (0 for SCE, 0 or 1 for CPE)
 * 
 * Outputs:     dequantized env scalefactors for left channel (before decoupling)
 *              dequantized env scalefactors for right channel (if coupling off)
 *                or raw decoded env scalefactors for right channel (if coupling on)
 *
 * Return:      none
 **************************************************************************************/
void DecodeSBREnvelope(BitStreamInfo *bsi, PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int ch)
{
	int huffIndexTime, huffIndexFreq, env, envStartBits, band, nBands, sf, lastEnv;
	int freqRes, freqResPrev, dShift, i;

	if (psi->couplingFlag && ch) {
		dShift = 1;
		if (sbrGrid->ampResFrame) {
			huffIndexTime = HuffTabSBR_tEnv30b;
			huffIndexFreq = HuffTabSBR_fEnv30b;
			envStartBits = 5;
		} else {
			huffIndexTime = HuffTabSBR_tEnv15b;
			huffIndexFreq = HuffTabSBR_fEnv15b;
			envStartBits = 6;
		}
	} else {
		dShift = 0;
		if (sbrGrid->ampResFrame) {
			huffIndexTime = HuffTabSBR_tEnv30;
			huffIndexFreq = HuffTabSBR_fEnv30;
			envStartBits = 6;
		} else {
			huffIndexTime = HuffTabSBR_tEnv15;
			huffIndexFreq = HuffTabSBR_fEnv15;
			envStartBits = 7;
		}
	}

	/* range of envDataQuant[] = [0, 127] (see comments in DequantizeEnvelope() for reference) */
	for (env = 0; env < sbrGrid->numEnv; env++) {
		nBands =      (sbrGrid->freqRes[env] ? sbrFreq->nHigh : sbrFreq->nLow);
		freqRes =     (sbrGrid->freqRes[env]);
		freqResPrev = (env == 0 ? sbrGrid->freqResPrev : sbrGrid->freqRes[env-1]);
		lastEnv =     (env == 0 ? sbrGrid->numEnvPrev-1 : env-1);
		if (lastEnv < 0)
			lastEnv = 0;	/* first frame */

		ASSERT(nBands <= MAX_QMF_BANDS);

		if (sbrChan->deltaFlagEnv[env] == 0) {
			/* delta coding in freq */
			sf = GetBits(bsi, envStartBits) << dShift;
			sbrChan->envDataQuant[env][0] = sf;
			for (band = 1; band < nBands; band++) {
				sf = DecodeOneSymbol(bsi, huffIndexFreq) << dShift;
				sbrChan->envDataQuant[env][band] = sf + sbrChan->envDataQuant[env][band-1];
			}
		} else if (freqRes == freqResPrev) {
			/* delta coding in time - same freq resolution for both frames */
			for (band = 0; band < nBands; band++) {
				sf = DecodeOneSymbol(bsi, huffIndexTime) << dShift;
				sbrChan->envDataQuant[env][band] = sf + sbrChan->envDataQuant[lastEnv][band];
			}
		} else if (freqRes == 0 && freqResPrev == 1) {
			/* delta coding in time - low freq resolution for new frame, high freq resolution for old frame */
			for (band = 0; band < nBands; band++) {
				sf = DecodeOneSymbol(bsi, huffIndexTime) << dShift;
				sbrChan->envDataQuant[env][band] = sf;
				for (i = 0; i < sbrFreq->nHigh; i++) {
					if (sbrFreq->freqHigh[i] == sbrFreq->freqLow[band]) {
						sbrChan->envDataQuant[env][band] += sbrChan->envDataQuant[lastEnv][i];
						break;
					}
				}
			}
		} else if (freqRes == 1 && freqResPrev == 0) {
			/* delta coding in time - high freq resolution for new frame, low freq resolution for old frame */
			for (band = 0; band < nBands; band++) {
				sf = DecodeOneSymbol(bsi, huffIndexTime) << dShift;
				sbrChan->envDataQuant[env][band] = sf;
				for (i = 0; i < sbrFreq->nLow; i++) {
					if (sbrFreq->freqLow[i] <= sbrFreq->freqHigh[band] && sbrFreq->freqHigh[band] < sbrFreq->freqLow[i+1] ) {
						sbrChan->envDataQuant[env][band] += sbrChan->envDataQuant[lastEnv][i];
						break;
					}
				}
			}
		}

		/* skip coupling channel */
		if (ch != 1 || psi->couplingFlag != 1)
			psi->envDataDequantScale[ch][env] = DequantizeEnvelope(nBands, sbrGrid->ampResFrame, sbrChan->envDataQuant[env], psi->envDataDequant[ch][env]);
	}
	sbrGrid->numEnvPrev = sbrGrid->numEnv;
	sbrGrid->freqResPrev = sbrGrid->freqRes[sbrGrid->numEnv-1];
}

/**************************************************************************************
 * Function:    DecodeSBRNoise
 *
 * Description: decode delta Huffman coded noise scalefactors from bitstream
 *
 * Inputs:      BitStreamInfo struct pointing to start of noise data
 *              initialized PSInfoSBR struct
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              initialized SBRChan struct for this channel
 *              index of current channel (0 for SCE, 0 or 1 for CPE)
 * 
 * Outputs:     dequantized noise scalefactors for left channel (before decoupling)
 *              dequantized noise scalefactors for right channel (if coupling off)
 *                or raw decoded noise scalefactors for right channel (if coupling on)
 *
 * Return:      none
 **************************************************************************************/
void DecodeSBRNoise(BitStreamInfo *bsi, PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int ch)
{
	int huffIndexTime, huffIndexFreq, noiseFloor, band, dShift, sf, lastNoiseFloor;

	if (psi->couplingFlag && ch) {
		dShift = 1;
		huffIndexTime = HuffTabSBR_tNoise30b;
		huffIndexFreq = HuffTabSBR_fNoise30b;
	} else {
		dShift = 0;
		huffIndexTime = HuffTabSBR_tNoise30;
		huffIndexFreq = HuffTabSBR_fNoise30;
	}

	for (noiseFloor = 0; noiseFloor < sbrGrid->numNoiseFloors; noiseFloor++) {
		lastNoiseFloor = (noiseFloor == 0 ? sbrGrid->numNoiseFloorsPrev-1 : noiseFloor-1);
		if (lastNoiseFloor < 0)
			lastNoiseFloor = 0;	/* first frame */

		ASSERT(sbrFreq->numNoiseFloorBands <= MAX_QMF_BANDS);

		if (sbrChan->deltaFlagNoise[noiseFloor] == 0) {
			/* delta coding in freq */
			sbrChan->noiseDataQuant[noiseFloor][0] = GetBits(bsi, 5) << dShift;
			for (band = 1; band < sbrFreq->numNoiseFloorBands; band++) {
				sf = DecodeOneSymbol(bsi, huffIndexFreq) << dShift;
				sbrChan->noiseDataQuant[noiseFloor][band] = sf + sbrChan->noiseDataQuant[noiseFloor][band-1];
			}
		} else {
			/* delta coding in time */
			for (band = 0; band < sbrFreq->numNoiseFloorBands; band++) {
				sf = DecodeOneSymbol(bsi, huffIndexTime) << dShift;
				sbrChan->noiseDataQuant[noiseFloor][band] = sf + sbrChan->noiseDataQuant[lastNoiseFloor][band];
			}
		}

		/* skip coupling channel */
		if (ch != 1 || psi->couplingFlag != 1)
			DequantizeNoise(sbrFreq->numNoiseFloorBands, sbrChan->noiseDataQuant[noiseFloor], psi->noiseDataDequant[ch][noiseFloor]);
	}
	sbrGrid->numNoiseFloorsPrev = sbrGrid->numNoiseFloors;
}

/* dqTabCouple[i] = 2 / (1 + 2^(12 - i)), format = Q30 */
static const int dqTabCouple[25] PROGMEM = {
	0x0007ff80, 0x000ffe00, 0x001ff802, 0x003fe010, 0x007f8080, 0x00fe03f8, 0x01f81f82, 0x03e0f83e, 
	0x07878788, 0x0e38e38e, 0x1999999a, 0x2aaaaaab, 0x40000000, 0x55555555, 0x66666666, 0x71c71c72, 
	0x78787878, 0x7c1f07c2, 0x7e07e07e, 0x7f01fc08, 0x7f807f80, 0x7fc01ff0, 0x7fe007fe, 0x7ff00200, 
	0x7ff80080, 
};

/**************************************************************************************
 * Function:    UncoupleSBREnvelope
 *
 * Description: scale dequantized envelope scalefactors according to channel 
 *                coupling rules
 *
 * Inputs:      initialized PSInfoSBR struct including
 *                dequantized envelope data for left channel
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              initialized SBRChan struct for right channel including
 *                quantized envelope scalefactors
 * 
 * Outputs:     dequantized envelope data for left channel (after decoupling)
 *              dequantized envelope data for right channel (after decoupling)
 *
 * Return:      none
 **************************************************************************************/
void UncoupleSBREnvelope(PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChanR)
{
	int env, band, nBands, scalei, E_1;

	scalei = (sbrGrid->ampResFrame ? 0 : 1);
	for (env = 0; env < sbrGrid->numEnv; env++) {
		nBands = (sbrGrid->freqRes[env] ? sbrFreq->nHigh : sbrFreq->nLow);
		psi->envDataDequantScale[1][env] = psi->envDataDequantScale[0][env]; /* same scalefactor for L and R */
		for (band = 0; band < nBands; band++) {
			/* clip E_1 to [0, 24] (scalefactors approach 0 or 2) */
			E_1 = sbrChanR->envDataQuant[env][band] >> scalei;
			if (E_1 < 0)	E_1 = 0;
			if (E_1 > 24)	E_1 = 24;

			/* envDataDequant[0] has 1 GB, so << by 2 is okay */
			psi->envDataDequant[1][env][band] = MULSHIFT32(psi->envDataDequant[0][env][band], dqTabCouple[24 - E_1]) << 2;
			psi->envDataDequant[0][env][band] = MULSHIFT32(psi->envDataDequant[0][env][band], dqTabCouple[E_1]) << 2;
		}
	}
}

/**************************************************************************************
 * Function:    UncoupleSBRNoise
 *
 * Description: scale dequantized noise floor scalefactors according to channel 
 *                coupling rules
 *
 * Inputs:      initialized PSInfoSBR struct including
 *                dequantized noise data for left channel
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              initialized SBRChan struct for this channel including
 *                quantized noise scalefactors
 * 
 * Outputs:     dequantized noise data for left channel (after decoupling)
 *              dequantized noise data for right channel (after decoupling)
 *
 * Return:      none
 **************************************************************************************/
void UncoupleSBRNoise(PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChanR)
{
	int noiseFloor, band, Q_1;
	
	for (noiseFloor = 0; noiseFloor < sbrGrid->numNoiseFloors; noiseFloor++) {
		for (band = 0; band < sbrFreq->numNoiseFloorBands; band++) {
			/* Q_1 should be in range [0, 24] according to 4.6.18.3.6, but check to make sure */
			Q_1 = sbrChanR->noiseDataQuant[noiseFloor][band];
			if (Q_1 < 0)	Q_1 = 0;
			if (Q_1 > 24)	Q_1 = 24;

			/* noiseDataDequant[0] has 1 GB, so << by 2 is okay */
			psi->noiseDataDequant[1][noiseFloor][band] = MULSHIFT32(psi->noiseDataDequant[0][noiseFloor][band], dqTabCouple[24 - Q_1]) << 2;
			psi->noiseDataDequant[0][noiseFloor][band] = MULSHIFT32(psi->noiseDataDequant[0][noiseFloor][band], dqTabCouple[Q_1]) << 2;
		}
	}
}
