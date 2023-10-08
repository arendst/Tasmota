/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: noiseless.c,v 1.1 2005/02/26 01:47:35 jrecker Exp $ 
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
 * noiseless.c - decode channel info, scalefactors, quantized coefficients, 
 *                 scalefactor band codebook, and TNS coefficients from bitstream
 **************************************************************************************/

#include "coder.h"

//#include "profile.h"

//#define PROFILE_START(x)
//#define PROFILE_END()

/**************************************************************************************
 * Function:    DecodeICSInfo
 *
 * Description: decode individual channel stream info
 *
 * Inputs:      BitStreamInfo struct pointing to start of ICS info
 *                (14496-3, table 4.4.6) 
 *              sample rate index
 *
 * Outputs:     updated icsInfo struct
 *
 * Return:      none
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ void DecodeICSInfo(BitStreamInfo *bsi, ICSInfo *icsInfo, int sampRateIdx)
{
	int sfb, g, mask;

	icsInfo->icsResBit =      GetBits(bsi, 1);
	icsInfo->winSequence =    GetBits(bsi, 2);
	icsInfo->winShape =       GetBits(bsi, 1);
	if (icsInfo->winSequence == 2) {
		/* short block */
		icsInfo->maxSFB =     GetBits(bsi, 4);
		icsInfo->sfGroup =    GetBits(bsi, 7);
		icsInfo->numWinGroup =    1;
		icsInfo->winGroupLen[0] = 1;
		mask = 0x40;	/* start with bit 6 */
		for (g = 0; g < 7; g++) {
			if (icsInfo->sfGroup & mask)	{ 
				icsInfo->winGroupLen[icsInfo->numWinGroup - 1]++;
			} else { 
				icsInfo->numWinGroup++; 
				icsInfo->winGroupLen[icsInfo->numWinGroup - 1] = 1; 
			}
			mask >>= 1;
		}
	} else {
		/* long block */
		icsInfo->maxSFB =               GetBits(bsi, 6);
		icsInfo->predictorDataPresent = GetBits(bsi, 1);
		if (icsInfo->predictorDataPresent) {
			icsInfo->predictorReset =   GetBits(bsi, 1);
			if (icsInfo->predictorReset)
				icsInfo->predictorResetGroupNum = GetBits(bsi, 5);
			for (sfb = 0; sfb < MIN(icsInfo->maxSFB, predSFBMax[sampRateIdx]); sfb++)
				icsInfo->predictionUsed[sfb] = GetBits(bsi, 1);
		}
		icsInfo->numWinGroup = 1;
		icsInfo->winGroupLen[0] = 1;
	}
}

/**************************************************************************************
 * Function:    DecodeSectionData
 *
 * Description: decode section data (scale factor band groupings and 
 *                associated Huffman codebooks)
 *
 * Inputs:      BitStreamInfo struct pointing to start of ICS info
 *                (14496-3, table 4.4.25)
 *              window sequence (short or long blocks)
 *              number of window groups (1 for long blocks, 1-8 for short blocks)
 *              max coded scalefactor band
 *
 * Outputs:     index of Huffman codebook for each scalefactor band in each section
 *
 * Return:      none
 *
 * Notes:       sectCB, sectEnd, sfbCodeBook, ordered by window groups for short blocks
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ static void DecodeSectionData(BitStreamInfo *bsi, int winSequence, int numWinGrp, int maxSFB, unsigned char *sfbCodeBook)
{
	int g, cb, sfb;
	int sectLen, sectLenBits, sectLenIncr, sectEscapeVal;

	sectLenBits = (winSequence == 2 ? 3 : 5);
	sectEscapeVal = (1 << sectLenBits) - 1;

	for (g = 0; g < numWinGrp; g++) {
		sfb = 0;
		while (sfb < maxSFB) {
			cb = GetBits(bsi, 4);	/* next section codebook */
			sectLen = 0;
			do {
				sectLenIncr = GetBits(bsi, sectLenBits);
				sectLen += sectLenIncr;
			} while (sectLenIncr == sectEscapeVal);

			sfb += sectLen;
			while (sectLen--)
				*sfbCodeBook++ = (unsigned char)cb;
		}
		ASSERT(sfb == maxSFB);
	}
}

/**************************************************************************************
 * Function:    DecodeOneScaleFactor
 *
 * Description: decode one scalefactor using scalefactor Huffman codebook
 *
 * Inputs:      BitStreamInfo struct pointing to start of next coded scalefactor
 *
 * Outputs:     updated BitstreamInfo struct
 *
 * Return:      one decoded scalefactor, including index_offset of -60
 **************************************************************************************/
static int DecodeOneScaleFactor(BitStreamInfo *bsi)
{
	int nBits, val;
	unsigned int bitBuf;

	/* decode next scalefactor from bitstream */
	bitBuf = GetBitsNoAdvance(bsi, huffTabScaleFactInfo.maxBits) << (32 - huffTabScaleFactInfo.maxBits);
  //PROFILE_START("DecodeHuffmanScalar");
	nBits = DecodeHuffmanScalar(huffTabScaleFact, &huffTabScaleFactInfo, bitBuf, &val);
	AdvanceBitstream(bsi, nBits);
  //PROFILE_END();
	return val;
}

/**************************************************************************************
 * Function:    DecodeScaleFactors
 *
 * Description: decode scalefactors, PNS energy, and intensity stereo weights
 *
 * Inputs:      BitStreamInfo struct pointing to start of ICS info
 *                (14496-3, table 4.4.26)
 *              number of window groups (1 for long blocks, 1-8 for short blocks)
 *              max coded scalefactor band
 *              global gain (starting value for differential scalefactor coding)
 *              index of Huffman codebook for each scalefactor band in each section
 *
 * Outputs:     decoded scalefactor for each section
 *
 * Return:      none
 *
 * Notes:       sfbCodeBook, scaleFactors ordered by window groups for short blocks
 *              for section with codebook 13, scaleFactors buffer has decoded PNS
 *                energy instead of regular scalefactor
 *              for section with codebook 14 or 15, scaleFactors buffer has intensity
 *                stereo weight instead of regular scalefactor
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ static void DecodeScaleFactors(BitStreamInfo *bsi, int numWinGrp, int maxSFB, int globalGain,
								  unsigned char *sfbCodeBook, short *scaleFactors)
{
	int g, sfbCB, nrg, npf, val, sf, is;

	/* starting values for differential coding */
	sf = globalGain;
	is = 0;
	nrg = globalGain - 90 - 256;
	npf = 1;

	for (g = 0; g < numWinGrp * maxSFB; g++) {
		sfbCB = *sfbCodeBook++;

		if (sfbCB  == 14 || sfbCB == 15) {
			/* intensity stereo - differential coding */
			val = DecodeOneScaleFactor(bsi);
			is += val;
			*scaleFactors++ = (short)is;
		} else if (sfbCB == 13) {
			/* PNS - first energy is directly coded, rest are Huffman coded (npf = noise_pcm_flag) */ 
			if (npf) {
				val = GetBits(bsi, 9);
				npf = 0;
			} else {
				val = DecodeOneScaleFactor(bsi);
			}
			nrg += val;
			*scaleFactors++ = (short)nrg;
		} else if (sfbCB >= 1 && sfbCB <= 11) {
			/* regular (non-zero) region - differential coding */
			val = DecodeOneScaleFactor(bsi);
			sf += val;
			*scaleFactors++ = (short)sf;
		} else {
			/* inactive scalefactor band if codebook 0 */
			*scaleFactors++ = 0;
		}
	}
}

/**************************************************************************************
 * Function:    DecodePulseInfo
 *
 * Description: decode pulse information
 *
 * Inputs:      BitStreamInfo struct pointing to start of pulse info
 *                (14496-3, table 4.4.7)
 *
 * Outputs:     updated PulseInfo struct
 *
 * Return:      none
 **************************************************************************************/
/* __attribute__ ((section (".data"))) */ static void DecodePulseInfo(BitStreamInfo *bsi, PulseInfo *pi)
{
	int i;

	pi->numPulse = GetBits(bsi, 2) + 1;		/* add 1 here */
	pi->startSFB = GetBits(bsi, 6);
	for (i = 0; i < pi->numPulse; i++) {
		pi->offset[i] = GetBits(bsi, 5);
		pi->amp[i] = GetBits(bsi, 4);
	}
}

/**************************************************************************************
 * Function:    DecodeTNSInfo
 *
 * Description: decode TNS filter information
 *
 * Inputs:      BitStreamInfo struct pointing to start of TNS info
 *                (14496-3, table 4.4.27)
 *              window sequence (short or long blocks)
 *
 * Outputs:     updated TNSInfo struct
 *              buffer of decoded (signed) TNS filter coefficients
 *
 * Return:      none
 **************************************************************************************/
static const signed char sgnMask[3] = {0x02,  0x04,  0x08};
static const signed char negMask[3] = {~0x03, ~0x07, ~0x0f};

static void DecodeTNSInfo(BitStreamInfo *bsi, int winSequence, TNSInfo *ti, signed char *tnsCoef)
{
	int i, w, f, coefBits, compress;
	signed char c, s, n;

	unsigned char *filtLength, *filtOrder, *filtDir;

	filtLength = ti->length;
	filtOrder =  ti->order;
	filtDir =    ti->dir;

	if (winSequence == 2) {
		/* short blocks */
		for (w = 0; w < NWINDOWS_SHORT; w++) {
			ti->numFilt[w] = GetBits(bsi, 1);
			if (ti->numFilt[w]) {
				ti->coefRes[w] = GetBits(bsi, 1) + 3;
				*filtLength =    GetBits(bsi, 4);
				*filtOrder =     GetBits(bsi, 3);
				if (*filtOrder) {
					*filtDir++ =      GetBits(bsi, 1);
					compress =        GetBits(bsi, 1);
					coefBits = (int)ti->coefRes[w] - compress;	/* 2, 3, or 4 */
					s = sgnMask[coefBits - 2];
					n = negMask[coefBits - 2];
					for (i = 0; i < *filtOrder; i++) {
						c = GetBits(bsi, coefBits);
						if (c & s)	c |= n;
						*tnsCoef++ = c;
					}
				}
				filtLength++;
				filtOrder++;
			}
		}
	} else {
		/* long blocks */
		ti->numFilt[0] = GetBits(bsi, 2);
		if (ti->numFilt[0])
			ti->coefRes[0] = GetBits(bsi, 1) + 3;
		for (f = 0; f < ti->numFilt[0]; f++) {
			*filtLength =      GetBits(bsi, 6);
			*filtOrder =       GetBits(bsi, 5);
			if (*filtOrder) {
				*filtDir++ =     GetBits(bsi, 1);
				compress =       GetBits(bsi, 1);
				coefBits = (int)ti->coefRes[0] - compress;	/* 2, 3, or 4 */
				s = sgnMask[coefBits - 2];
				n = negMask[coefBits - 2];
				for (i = 0; i < *filtOrder; i++) {
					c = GetBits(bsi, coefBits);
					if (c & s)	c |= n;
					*tnsCoef++ = c;
				}
			}
			filtLength++;
			filtOrder++;
		}
	}
}

/* bitstream field lengths for gain control data:
 *   gainBits[winSequence][0] = maxWindow (how many gain windows there are)
 *   gainBits[winSequence][1] = locBitsZero (bits for alocCode if window == 0)
 *   gainBits[winSequence][2] = locBits (bits for alocCode if window != 0)
 */
static const unsigned char gainBits[4][3] = {
	{1, 5, 5},  /* long */
	{2, 4, 2},  /* start */
	{8, 2, 2},  /* short */
	{2, 4, 5},  /* stop */
};

/**************************************************************************************
 * Function:    DecodeGainControlInfo
 *
 * Description: decode gain control information (SSR profile only)
 *
 * Inputs:      BitStreamInfo struct pointing to start of gain control info
 *                (14496-3, table 4.4.12)
 *              window sequence (short or long blocks)
 *
 * Outputs:     updated GainControlInfo struct
 *
 * Return:      none
 **************************************************************************************/
static void DecodeGainControlInfo(BitStreamInfo *bsi, int winSequence, GainControlInfo *gi)
{
	int bd, wd, ad;
	int locBits, locBitsZero, maxWin;
	
	gi->maxBand = GetBits(bsi, 2);
	maxWin =      (int)gainBits[winSequence][0];
	locBitsZero = (int)gainBits[winSequence][1];
	locBits =     (int)gainBits[winSequence][2];

	for (bd = 1; bd <= gi->maxBand; bd++) {
		for (wd = 0; wd < maxWin; wd++) {
			gi->adjNum[bd][wd] = GetBits(bsi, 3);
			for (ad = 0; ad < gi->adjNum[bd][wd]; ad++) {
				gi->alevCode[bd][wd][ad] = GetBits(bsi, 4);
				gi->alocCode[bd][wd][ad] = GetBits(bsi, (wd == 0 ? locBitsZero : locBits));
			}
		}
	}
}

/**************************************************************************************
 * Function:    DecodeICS
 *
 * Description: decode individual channel stream
 *
 * Inputs:      platform specific info struct
 *              BitStreamInfo struct pointing to start of individual channel stream
 *                (14496-3, table 4.4.24) 
 *              index of current channel
 *
 * Outputs:     updated section data, scale factor data, pulse data, TNS data, 
 *                and gain control data
 *
 * Return:      none
 **************************************************************************************/
static void DecodeICS(PSInfoBase *psi, BitStreamInfo *bsi, int ch)
{
	int globalGain;
	ICSInfo *icsInfo;
	PulseInfo *pi;
	TNSInfo *ti;
	GainControlInfo *gi;

	icsInfo = (ch == 1 && psi->commonWin == 1) ? &(psi->icsInfo[0]) : &(psi->icsInfo[ch]);

	globalGain = GetBits(bsi, 8);
	if (!psi->commonWin)
		DecodeICSInfo(bsi, icsInfo, psi->sampRateIdx);

	DecodeSectionData(bsi, icsInfo->winSequence, icsInfo->numWinGroup, icsInfo->maxSFB, psi->sfbCodeBook[ch]);

	DecodeScaleFactors(bsi, icsInfo->numWinGroup, icsInfo->maxSFB, globalGain, psi->sfbCodeBook[ch], psi->scaleFactors[ch]);
	
	pi = &psi->pulseInfo[ch];
	pi->pulseDataPresent = GetBits(bsi, 1);
	if (pi->pulseDataPresent)
		DecodePulseInfo(bsi, pi);

	ti = &psi->tnsInfo[ch];
	ti->tnsDataPresent = GetBits(bsi, 1);
	if (ti->tnsDataPresent)
		DecodeTNSInfo(bsi, icsInfo->winSequence, ti, ti->coef);

	gi = &psi->gainControlInfo[ch];
	gi->gainControlDataPresent = GetBits(bsi, 1);
	if (gi->gainControlDataPresent)
		DecodeGainControlInfo(bsi, icsInfo->winSequence, gi);
}

/**************************************************************************************
 * Function:    DecodeNoiselessData
 *
 * Description: decode noiseless data (side info and transform coefficients)
 *
 * Inputs:      valid AACDecInfo struct
 *              double pointer to buffer pointing to start of individual channel stream
 *                (14496-3, table 4.4.24)
 *              pointer to bit offset
 *              pointer to number of valid bits remaining in buf
 *              index of current channel
 *
 * Outputs:     updated global gain, section data, scale factor data, pulse data,
 *                TNS data, gain control data, and spectral data
 *
 * Return:      0 if successful, error code (< 0) if error
 **************************************************************************************/
int DecodeNoiselessData(AACDecInfo *aacDecInfo, unsigned char **buf, int *bitOffset, int *bitsAvail, int ch)
{
	int bitsUsed;
	BitStreamInfo bsi;
	PSInfoBase *psi;
	ICSInfo *icsInfo;

	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return ERR_AAC_NULL_POINTER;
	psi = (PSInfoBase *)(aacDecInfo->psInfoBase);
	icsInfo = (ch == 1 && psi->commonWin == 1) ? &(psi->icsInfo[0]) : &(psi->icsInfo[ch]);
	
	SetBitstreamPointer(&bsi, (*bitsAvail+7) >> 3, *buf);
	GetBits(&bsi, *bitOffset);

	DecodeICS(psi, &bsi, ch);

	if (icsInfo->winSequence == 2)
		DecodeSpectrumShort(psi, &bsi, ch);
	else
		DecodeSpectrumLong(psi, &bsi, ch);

	bitsUsed = CalcBitsUsed(&bsi, *buf, *bitOffset);
	*buf += ((bitsUsed + *bitOffset) >> 3);
	*bitOffset = ((bitsUsed + *bitOffset) & 0x07);
	*bitsAvail -= bitsUsed;

	aacDecInfo->sbDeinterleaveReqd[ch] = 0;
	aacDecInfo->tnsUsed |= psi->tnsInfo[ch].tnsDataPresent;	/* set flag if TNS used for any channel */

	return ERR_AAC_NONE;
}
