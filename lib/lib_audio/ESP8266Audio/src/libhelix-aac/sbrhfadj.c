/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: sbrhfadj.c,v 1.3 2005/05/24 16:01:55 albertofloyd Exp $ 
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
 * sbrhfadj.c - high frequency adjustment for SBR
 **************************************************************************************/

#include "sbr.h"
#include "assembly.h"

/* invBandTab[i] = 1.0 / (i + 1), Q31 */
static const int invBandTab[64] PROGMEM = {
	0x7fffffff, 0x40000000, 0x2aaaaaab, 0x20000000, 0x1999999a, 0x15555555, 0x12492492, 0x10000000, 
	0x0e38e38e, 0x0ccccccd, 0x0ba2e8ba, 0x0aaaaaab, 0x09d89d8a, 0x09249249, 0x08888889, 0x08000000, 
	0x07878788, 0x071c71c7, 0x06bca1af, 0x06666666, 0x06186186, 0x05d1745d, 0x0590b216, 0x05555555, 
	0x051eb852, 0x04ec4ec5, 0x04bda12f, 0x04924925, 0x0469ee58, 0x04444444, 0x04210842, 0x04000000, 
	0x03e0f83e, 0x03c3c3c4, 0x03a83a84, 0x038e38e4, 0x03759f23, 0x035e50d8, 0x03483483, 0x03333333, 
	0x031f3832, 0x030c30c3, 0x02fa0be8, 0x02e8ba2f, 0x02d82d83, 0x02c8590b, 0x02b93105, 0x02aaaaab, 
	0x029cbc15, 0x028f5c29, 0x02828283, 0x02762762, 0x026a439f, 0x025ed098, 0x0253c825, 0x02492492, 
	0x023ee090, 0x0234f72c, 0x022b63cc, 0x02222222, 0x02192e2a, 0x02108421, 0x02082082, 0x02000000, 
};

/**************************************************************************************
 * Function:    EstimateEnvelope
 *
 * Description: estimate power of generated HF QMF bands in one time-domain envelope
 *                (4.6.18.7.3)
 *
 * Inputs:      initialized PSInfoSBR struct
 *              initialized SBRHeader struct for this SCE/CPE block
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              index of current envelope
 *
 * Outputs:     power of each QMF subband, stored as integer (Q0) * 2^N, N >= 0
 *
 * Return:      none
 **************************************************************************************/
static void EstimateEnvelope(PSInfoSBR *psi, SBRHeader *sbrHdr, SBRGrid *sbrGrid, SBRFreq *sbrFreq, int env)
{
	int i, m, iStart, iEnd, xre, xim, nScale, expMax;
	int p, n, mStart, mEnd, invFact, t;
	int *XBuf;
	U64 eCurr;
	unsigned char *freqBandTab;

	/* estimate current envelope */
	iStart = sbrGrid->envTimeBorder[env] + HF_ADJ;
	iEnd =   sbrGrid->envTimeBorder[env+1] + HF_ADJ;
	if (sbrGrid->freqRes[env]) {
		n = sbrFreq->nHigh;
		freqBandTab = sbrFreq->freqHigh;
	} else {
		n = sbrFreq->nLow;
		freqBandTab = sbrFreq->freqLow;
	}

	/* ADS should inline MADD64 (smlal) properly, but check to make sure */
	expMax = 0;
	if (sbrHdr->interpFreq) {
		for (m = 0; m < sbrFreq->numQMFBands; m++) {
			eCurr.w64 = 0;
			XBuf = psi->XBuf[iStart][sbrFreq->kStart + m];
			for (i = iStart; i < iEnd; i++) {
				/* scale to int before calculating power (precision not critical, and avoids overflow) */
				xre = (*XBuf) >> FBITS_OUT_QMFA;	XBuf += 1;
				xim = (*XBuf) >> FBITS_OUT_QMFA;	XBuf += (2*64 - 1);
				eCurr.w64 = MADD64(eCurr.w64, xre, xre);
				eCurr.w64 = MADD64(eCurr.w64, xim, xim);
			}

			/* eCurr.w64 is now Q(64 - 2*FBITS_OUT_QMFA) (64-bit word)
			 * if energy is too big to fit in 32-bit word (> 2^31) scale down by power of 2
			 */
			nScale = 0;
			if (eCurr.r.hi32) {
				nScale = (32 - CLZ(eCurr.r.hi32)) + 1;
				t  = (int)(eCurr.r.lo32 >> nScale);		/* logical (unsigned) >> */
				t |= eCurr.r.hi32 << (32 - nScale);
			} else if (eCurr.r.lo32 >> 31) {
				nScale = 1;
				t  = (int)(eCurr.r.lo32 >> nScale);		/* logical (unsigned) >> */
			} else {
				t  = (int)eCurr.r.lo32;
			}

			invFact = invBandTab[(iEnd - iStart)-1];
			psi->eCurr[m] = MULSHIFT32(t, invFact);
			psi->eCurrExp[m] = nScale + 1;	/* +1 for invFact = Q31 */
			if (psi->eCurrExp[m] > expMax)
				expMax = psi->eCurrExp[m];
		}
	} else {
		for (p = 0; p < n; p++) {
			mStart = freqBandTab[p];
			mEnd =   freqBandTab[p+1];
			eCurr.w64 = 0;
			for (i = iStart; i < iEnd; i++) {
				XBuf = psi->XBuf[i][mStart];
				for (m = mStart; m < mEnd; m++) {
					xre = (*XBuf++) >> FBITS_OUT_QMFA;
					xim = (*XBuf++) >> FBITS_OUT_QMFA;
					eCurr.w64 = MADD64(eCurr.w64, xre, xre);
					eCurr.w64 = MADD64(eCurr.w64, xim, xim);
				}
			}

			nScale = 0;
			if (eCurr.r.hi32) {
				nScale = (32 - CLZ(eCurr.r.hi32)) + 1;
				t  = (int)(eCurr.r.lo32 >> nScale);		/* logical (unsigned) >> */
				t |= eCurr.r.hi32 << (32 - nScale);
			} else if (eCurr.r.lo32 >> 31) {
				nScale = 1;
				t  = (int)(eCurr.r.lo32 >> nScale);		/* logical (unsigned) >> */
			} else {
				t  = (int)eCurr.r.lo32;
			}

			invFact = invBandTab[(iEnd - iStart)-1];
			invFact = MULSHIFT32(invBandTab[(mEnd - mStart)-1], invFact) << 1;
			t = MULSHIFT32(t, invFact);

			for (m = mStart; m < mEnd; m++) {
				psi->eCurr[m - sbrFreq->kStart] = t;
				psi->eCurrExp[m - sbrFreq->kStart] = nScale + 1;	/* +1 for invFact = Q31 */
			}
			if (psi->eCurrExp[mStart - sbrFreq->kStart] > expMax)
				expMax = psi->eCurrExp[mStart - sbrFreq->kStart];
		}
	}
	psi->eCurrExpMax = expMax;
}

/**************************************************************************************
 * Function:    GetSMapped
 *
 * Description: calculate SMapped (4.6.18.7.2)
 *
 * Inputs:      initialized PSInfoSBR struct
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              initialized SBRChan struct for this channel
 *              index of current envelope
 *              index of current QMF band
 *              la flag for this envelope
 *
 * Outputs:     none
 *
 * Return:      1 if a sinusoid is present in this band, 0 if not
 **************************************************************************************/
static int GetSMapped(SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int env, int band, int la)
{
	int bandStart, bandEnd, oddFlag, r;

	if (sbrGrid->freqRes[env]) {
		/* high resolution */
		bandStart = band;
		bandEnd = band+1;
	} else {
		/* low resolution (see CalcFreqLow() for mapping) */
		oddFlag = sbrFreq->nHigh & 0x01;
		bandStart = (band > 0 ? 2*band - oddFlag : 0);		/* starting index for freqLow[band] */
		bandEnd = 2*(band+1) - oddFlag;						/* ending index for freqLow[band+1] */
	}

	/* sMapped = 1 if sIndexMapped == 1 for any frequency in this band */
	for (band = bandStart; band < bandEnd; band++) {
		if (sbrChan->addHarmonic[1][band]) {
			r = ((sbrFreq->freqHigh[band+1] + sbrFreq->freqHigh[band]) >> 1);
			if (env >= la || sbrChan->addHarmonic[0][r] == 1)
				return 1;
		}
	}
	return 0;
}

#define GBOOST_MAX	0x2830afd3	/* Q28, 1.584893192 squared */
#define	ACC_SCALE	6

/* squared version of table in 4.6.18.7.5 */
static const int limGainTab[4] PROGMEM = {0x20138ca7, 0x40000000, 0x7fb27dce, 0x80000000};	/* Q30 (0x80000000 = sentinel for GMAX) */

/**************************************************************************************
 * Function:    CalcMaxGain
 *
 * Description: calculate max gain in one limiter band (4.6.18.7.5)
 *
 * Inputs:      initialized PSInfoSBR struct
 *              initialized SBRHeader struct for this SCE/CPE block
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              index of current channel (0 for SCE, 0 or 1 for CPE)
 *              index of current envelope
 *              index of current limiter band
 *              number of fraction bits in dequantized envelope 
 *                (max = Q(FBITS_OUT_DQ_ENV - 6) = Q23, can go negative)
 *
 * Outputs:     updated gainMax, gainMaxFBits, and sumEOrigMapped in PSInfoSBR struct
 *
 * Return:      none
 **************************************************************************************/
static void CalcMaxGain(PSInfoSBR *psi, SBRHeader *sbrHdr, SBRGrid *sbrGrid, SBRFreq *sbrFreq, int ch, int env, int lim, int fbitsDQ)
{
	int m, mStart, mEnd, q, z, r;
	int sumEOrigMapped, sumECurr, gainMax, eOMGainMax, envBand;
	unsigned char eCurrExpMax;
	unsigned char *freqBandTab;

	mStart = sbrFreq->freqLimiter[lim];   /* these are offsets from kStart */
	mEnd =   sbrFreq->freqLimiter[lim + 1];
	freqBandTab = (sbrGrid->freqRes[env] ? sbrFreq->freqHigh : sbrFreq->freqLow);

	/* calculate max gain to apply to signal in this limiter band */
	sumECurr = 0;
	sumEOrigMapped = 0;
	eCurrExpMax = psi->eCurrExpMax;
	eOMGainMax = psi->eOMGainMax;
	envBand = psi->envBand;
	for (m = mStart; m < mEnd; m++) {
		/* map current QMF band to appropriate envelope band */
		if (m == freqBandTab[envBand + 1] - sbrFreq->kStart) {
			envBand++;
			eOMGainMax = psi->envDataDequant[ch][env][envBand] >> ACC_SCALE;	/* summing max 48 bands */
		}
		sumEOrigMapped += eOMGainMax;

		/* easy test for overflow on ARM */
		sumECurr += (psi->eCurr[m] >> (eCurrExpMax - psi->eCurrExp[m]));
		if (sumECurr >> 30) {
			sumECurr >>= 1;
			eCurrExpMax++;
		}
	}
	psi->eOMGainMax = eOMGainMax;
	psi->envBand = envBand;

	psi->gainMaxFBits = 30;	/* Q30 tables */
	if (sumECurr == 0) {
		/* any non-zero numerator * 1/EPS_0 is > G_MAX */
		gainMax = (sumEOrigMapped == 0 ? (int)limGainTab[sbrHdr->limiterGains] : (int)0x80000000);
	} else if (sumEOrigMapped == 0) {
		/* 1/(any non-zero denominator) * EPS_0 * limGainTab[x] is appx. 0 */
		gainMax = 0;
	} else {
		/* sumEOrigMapped = Q(fbitsDQ - ACC_SCALE), sumECurr = Q(-eCurrExpMax) */
		gainMax = limGainTab[sbrHdr->limiterGains];
		if (sbrHdr->limiterGains != 3) {
			q = MULSHIFT32(sumEOrigMapped, gainMax);	/* Q(fbitsDQ - ACC_SCALE - 2), gainMax = Q30  */
			z = CLZ(sumECurr) - 1;
			r = InvRNormalized(sumECurr << z);	/* in =  Q(z - eCurrExpMax), out = Q(29 + 31 - z + eCurrExpMax) */
			gainMax = MULSHIFT32(q, r);			/* Q(29 + 31 - z + eCurrExpMax + fbitsDQ - ACC_SCALE - 2 - 32) */
			psi->gainMaxFBits = 26 - z + eCurrExpMax + fbitsDQ - ACC_SCALE;
		}
	}
	psi->sumEOrigMapped = sumEOrigMapped;
	psi->gainMax = gainMax;
}

/**************************************************************************************
 * Function:    CalcNoiseDivFactors
 *
 * Description: calculate 1/(1+Q) and Q/(1+Q) (4.6.18.7.4; 4.6.18.7.5)
 *
 * Inputs:      dequantized noise floor scalefactor
 *
 * Outputs:     1/(1+Q) and Q/(1+Q), format = Q31
 *
 * Return:      none
 **************************************************************************************/
static void CalcNoiseDivFactors(int q, int *qp1Inv, int *qqp1Inv)
{
	int z, qp1, t, s;

	/* 1 + Q_orig */
	qp1  = (q >> 1);
	qp1 += (1 << (FBITS_OUT_DQ_NOISE - 1));		/* >> 1 to avoid overflow when adding 1.0 */
	z = CLZ(qp1) - 1;							/* z <= 31 - FBITS_OUT_DQ_NOISE */
	qp1 <<= z;									/* Q(FBITS_OUT_DQ_NOISE + z) = Q31 * 2^-(31 - (FBITS_OUT_DQ_NOISE + z)) */
	t = InvRNormalized(qp1) << 1;				/* Q30 * 2^(31 - (FBITS_OUT_DQ_NOISE + z)), guaranteed not to overflow */

	/* normalize to Q31 */
	s = (31 - (FBITS_OUT_DQ_NOISE - 1) - z - 1);	/* clearly z >= 0, z <= (30 - (FBITS_OUT_DQ_NOISE - 1)) */
	*qp1Inv = (t >> s);								/* s = [0, 31 - FBITS_OUT_DQ_NOISE] */
	*qqp1Inv = MULSHIFT32(t, q) << (32 - FBITS_OUT_DQ_NOISE - s);
}

/**************************************************************************************
 * Function:    CalcComponentGains
 *
 * Description: calculate gain of envelope, sinusoids, and noise in one limiter band
 *                (4.6.18.7.5)
 *
 * Inputs:      initialized PSInfoSBR struct
 *              initialized SBRHeader struct for this SCE/CPE block
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              initialized SBRChan struct for this channel
 *              index of current channel (0 for SCE, 0 or 1 for CPE)
 *              index of current envelope
 *              index of current limiter band
 *              number of fraction bits in dequantized envelope
 *
 * Outputs:     gains for envelope, sinusoids and noise
 *              number of fraction bits for envelope gain
 *              sum of the total gain for each component in this band
 *              other updated state variables
 *
 * Return:      none
 **************************************************************************************/
static void CalcComponentGains(PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int ch, int env, int lim, int fbitsDQ)
{
	int d, m, mStart, mEnd, q, qm, noiseFloor, sIndexMapped;
	int shift, eCurr, maxFlag, gainMax, gainMaxFBits;
	int gain, sm, z, r, fbitsGain, gainScale;
	unsigned char *freqBandTab;

	mStart = sbrFreq->freqLimiter[lim];   /* these are offsets from kStart */
	mEnd =   sbrFreq->freqLimiter[lim + 1];

	gainMax = psi->gainMax;
	gainMaxFBits = psi->gainMaxFBits;

	d = (env == psi->la || env == sbrChan->laPrev ? 0 : 1);
	freqBandTab = (sbrGrid->freqRes[env] ? sbrFreq->freqHigh : sbrFreq->freqLow);

	/* figure out which noise floor this envelope is in (only 1 or 2 noise floors allowed) */
	noiseFloor = 0;
	if (sbrGrid->numNoiseFloors == 2 && sbrGrid->noiseTimeBorder[1] <= sbrGrid->envTimeBorder[env])
		noiseFloor++;

	psi->sumECurrGLim = 0;
	psi->sumSM = 0;
	psi->sumQM = 0;
	/* calculate energy of noise to add in this limiter band */
	for (m = mStart; m < mEnd; m++) {
		if (m == sbrFreq->freqNoise[psi->noiseFloorBand + 1] - sbrFreq->kStart) {
			/* map current QMF band to appropriate noise floor band (NOTE: freqLimiter[0] == freqLow[0] = freqHigh[0]) */
			psi->noiseFloorBand++;
			CalcNoiseDivFactors(psi->noiseDataDequant[ch][noiseFloor][psi->noiseFloorBand], &(psi->qp1Inv), &(psi->qqp1Inv));
		}
		if (m == sbrFreq->freqHigh[psi->highBand + 1] - sbrFreq->kStart)
			psi->highBand++;
		if (m == freqBandTab[psi->sBand + 1] - sbrFreq->kStart) {
			psi->sBand++;
			psi->sMapped = GetSMapped(sbrGrid, sbrFreq, sbrChan, env, psi->sBand, psi->la);
		}

		/* get sIndexMapped for this QMF subband */
		sIndexMapped = 0;
		r = ((sbrFreq->freqHigh[psi->highBand+1] + sbrFreq->freqHigh[psi->highBand]) >> 1);
		if (m + sbrFreq->kStart == r) {
			/* r = center frequency, deltaStep = (env >= la || sIndexMapped'(r, numEnv'-1) == 1) */
			if (env >= psi->la || sbrChan->addHarmonic[0][r] == 1)
				sIndexMapped = sbrChan->addHarmonic[1][psi->highBand];
		}

		/* save sine flags from last envelope in this frame:
		 *   addHarmonic[0][0...63] = saved sine present flag from previous frame, for each QMF subband
		 *   addHarmonic[1][0...nHigh-1] = addHarmonic bit from current frame, for each high-res frequency band
		 * from MPEG reference code - slightly different from spec
		 *   (sIndexMapped'(m,LE'-1) can still be 0 when numEnv == psi->la)
		 */
		if (env == sbrGrid->numEnv - 1) {
			if (m + sbrFreq->kStart == r)
				sbrChan->addHarmonic[0][m + sbrFreq->kStart] = sbrChan->addHarmonic[1][psi->highBand];
			else
				sbrChan->addHarmonic[0][m + sbrFreq->kStart] = 0;
		}

		gain = psi->envDataDequant[ch][env][psi->sBand];
		qm = MULSHIFT32(gain, psi->qqp1Inv) << 1;
		sm = (sIndexMapped ? MULSHIFT32(gain, psi->qp1Inv) << 1 : 0);

		/* three cases: (sMapped == 0 && delta == 1), (sMapped == 0 && delta == 0), (sMapped == 1) */
		if (d == 1 && psi->sMapped == 0)
			gain = MULSHIFT32(psi->qp1Inv, gain) << 1;
		else if (psi->sMapped != 0)
			gain = MULSHIFT32(psi->qqp1Inv, gain) << 1;

		/* gain, qm, sm = Q(fbitsDQ), gainMax = Q(fbitsGainMax) */
		eCurr = psi->eCurr[m];
		if (eCurr) {
			z = CLZ(eCurr) - 1;
			r = InvRNormalized(eCurr << z);		/* in = Q(z - eCurrExp), out = Q(29 + 31 - z + eCurrExp) */
			gainScale = MULSHIFT32(gain, r);	/* out = Q(29 + 31 - z + eCurrExp + fbitsDQ - 32) */
			fbitsGain = 29 + 31 - z + psi->eCurrExp[m] + fbitsDQ - 32;
		} else {
			/* if eCurr == 0, then gain is unchanged (divide by EPS = 1) */
			gainScale = gain;
			fbitsGain = fbitsDQ;
		}

		/* see if gain for this band exceeds max gain */
		maxFlag = 0;
		if (gainMax != (int)0x80000000) {
			if (fbitsGain >= gainMaxFBits) {
				shift = MIN(fbitsGain - gainMaxFBits, 31);
				maxFlag = ((gainScale >> shift) > gainMax ? 1 : 0);
			} else {
				shift = MIN(gainMaxFBits - fbitsGain, 31);
				maxFlag = (gainScale > (gainMax >> shift) ? 1 : 0);
			}
		}

		if (maxFlag) {
			/* gainScale > gainMax, calculate ratio with 32/16 division */
			q = 0;
			r = gainScale;	/* guaranteed > 0, else maxFlag could not have been set */
			z = CLZ(r);
			if (z < 16) {
				q = 16 - z;
				r >>= q;	/* out = Q(fbitsGain - q) */
			}

			z = CLZ(gainMax) - 1;
			r = (gainMax << z) / r;		/* out = Q((fbitsGainMax + z) - (fbitsGain - q)) */
			q = (gainMaxFBits + z) - (fbitsGain - q);	/* r = Q(q) */
			if (q > 30) {
				r >>= MIN(q - 30, 31);
			} else {
				z = MIN(30 - q, 30);
				CLIP_2N_SHIFT30(r, z);	/* let r = Q30 since range = [0.0, 1.0) (clip to 0x3fffffff = 0.99999) */
			}

			qm = MULSHIFT32(qm, r) << 2;
			gain = MULSHIFT32(gain, r) << 2;
			psi->gLimBuf[m] = gainMax;
			psi->gLimFbits[m] = gainMaxFBits;
		} else {
			psi->gLimBuf[m] = gainScale;
			psi->gLimFbits[m] = fbitsGain;
		}

		/* sumSM, sumQM, sumECurrGLim = Q(fbitsDQ - ACC_SCALE) */
		psi->smBuf[m] = sm;
		psi->sumSM += (sm >> ACC_SCALE);

		psi->qmLimBuf[m] = qm;
		if (env != psi->la && env != sbrChan->laPrev && sm == 0)
			psi->sumQM += (qm >> ACC_SCALE);

		/* eCurr * gain^2 same as gain^2, before division by eCurr 
		 * (but note that gain != 0 even if eCurr == 0, since it's divided by eps)
		 */
		if (eCurr)
			psi->sumECurrGLim += (gain >> ACC_SCALE);
	}
}

/**************************************************************************************
 * Function:    ApplyBoost
 *
 * Description: calculate and apply boost factor for envelope, sinusoids, and noise 
 *                in this limiter band (4.6.18.7.5)
 *
 * Inputs:      initialized PSInfoSBR struct
 *              initialized SBRFreq struct for this SCE/CPE block
 *              index of current limiter band
 *              number of fraction bits in dequantized envelope
 *
 * Outputs:     envelope gain, sinusoids and noise after scaling by gBoost
 *              format = Q(FBITS_GLIM_BOOST) for envelope gain,
 *                     = Q(FBITS_QLIM_BOOST) for noise
 *                     = Q(FBITS_OUT_QMFA) for sinusoids
 *
 * Return:      none
 *
 * Notes:       after scaling, each component has at least 1 GB
 **************************************************************************************/
static void ApplyBoost(PSInfoSBR *psi, SBRFreq *sbrFreq, int lim, int fbitsDQ)
{
	int m, mStart, mEnd, q, z, r;
	int sumEOrigMapped, gBoost;

	mStart = sbrFreq->freqLimiter[lim];   /* these are offsets from kStart */
	mEnd =   sbrFreq->freqLimiter[lim + 1];

	sumEOrigMapped = psi->sumEOrigMapped >> 1;
	r = (psi->sumECurrGLim >> 1) + (psi->sumSM >> 1) + (psi->sumQM >> 1);	/* 1 GB fine (sm and qm are mutually exclusive in acc) */
	if (r < (1 << (31-28))) {
		/* any non-zero numerator * 1/EPS_0 is > GBOOST_MAX 
		 * round very small r to zero to avoid scaling problems
		 */
		gBoost = (sumEOrigMapped == 0 ? (1 << 28) : GBOOST_MAX);
		z = 0;
	} else if (sumEOrigMapped == 0) {
		/* 1/(any non-zero denominator) * EPS_0 is appx. 0 */
		gBoost = 0;
		z = 0;
	} else {
		/* numerator (sumEOrigMapped) and denominator (r) have same Q format (before << z) */
		z = CLZ(r) - 1;	/* z = [0, 27] */
		r = InvRNormalized(r << z);
		gBoost = MULSHIFT32(sumEOrigMapped, r);
	}

	/* gBoost = Q(28 - z) */
	if (gBoost > (GBOOST_MAX >> z)) {
		gBoost = GBOOST_MAX;
		z = 0;
	}
	gBoost <<= z;	/* gBoost = Q28, minimum 1 GB */

	/* convert gain, noise, sinusoids to fixed Q format, clipping if necessary
	 *   (rare, usually only happens at very low bitrates, introduces slight
	 *    distortion into final HF mapping, but should be inaudible)
	 */
	for (m = mStart; m < mEnd; m++) {
		/* let gLimBoost = Q24, since in practice the max values are usually 16 to 20
		 *   unless limiterGains == 3 (limiter off) and eCurr ~= 0 (i.e. huge gain, but only
		 *   because the envelope has 0 power anyway)
		 */
		q = MULSHIFT32(psi->gLimBuf[m], gBoost) << 2;	/* Q(gLimFbits) * Q(28) --> Q(gLimFbits[m]-2) */
		r = SqrtFix(q, psi->gLimFbits[m] - 2, &z);
		z -= FBITS_GLIM_BOOST;
		if (z >= 0) {
			psi->gLimBoost[m] = r >> MIN(z, 31);
		} else {
			z = MIN(30, -z);
			CLIP_2N_SHIFT30(r, z);
			psi->gLimBoost[m] = r;
		}

		q = MULSHIFT32(psi->qmLimBuf[m], gBoost) << 2;	/* Q(fbitsDQ) * Q(28) --> Q(fbitsDQ-2) */
		r = SqrtFix(q, fbitsDQ - 2, &z);
		z -= FBITS_QLIM_BOOST;		/* << by 14, since integer sqrt of x < 2^16, and we want to leave 1 GB */
		if (z >= 0) {
			psi->qmLimBoost[m] = r >> MIN(31, z);
		} else {
			z = MIN(30, -z);
			CLIP_2N_SHIFT30(r, z);
			psi->qmLimBoost[m] = r;
		}

		q = MULSHIFT32(psi->smBuf[m], gBoost) << 2;		/* Q(fbitsDQ) * Q(28) --> Q(fbitsDQ-2) */
		r = SqrtFix(q, fbitsDQ - 2, &z);
		z -= FBITS_OUT_QMFA;		/* justify for adding to signal (xBuf) later */
		if (z >= 0) {
			psi->smBoost[m] = r >> MIN(31, z);
		} else {
			z = MIN(30, -z);
			CLIP_2N_SHIFT30(r, z);
			psi->smBoost[m] = r;
		}
	}
}

/**************************************************************************************
 * Function:    CalcGain
 *
 * Description: calculate and apply proper gain to HF components in one envelope 
 *                (4.6.18.7.5)
 *
 * Inputs:      initialized PSInfoSBR struct
 *              initialized SBRHeader struct for this SCE/CPE block
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              initialized SBRChan struct for this channel
 *              index of current channel (0 for SCE, 0 or 1 for CPE)
 *              index of current envelope
 *
 * Outputs:     envelope gain, sinusoids and noise after scaling
 *
 * Return:      none
 **************************************************************************************/
static void CalcGain(PSInfoSBR *psi, SBRHeader *sbrHdr, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int ch, int env)
{
	int lim, fbitsDQ;

	/* initialize to -1 so that mapping limiter bands to env/noise bands works right on first pass */
	psi->envBand        = -1;
	psi->noiseFloorBand = -1;
	psi->sBand          = -1;
	psi->highBand       = -1;

	fbitsDQ = (FBITS_OUT_DQ_ENV - psi->envDataDequantScale[ch][env]);	/* Q(29 - optional scalefactor) */
	for (lim = 0; lim < sbrFreq->nLimiter; lim++) {
		/* the QMF bands are divided into lim regions (consecutive, non-overlapping) */
		CalcMaxGain(psi, sbrHdr, sbrGrid, sbrFreq, ch, env, lim, fbitsDQ);
		CalcComponentGains(psi, sbrGrid, sbrFreq, sbrChan, ch, env, lim, fbitsDQ);
		ApplyBoost(psi, sbrFreq, lim, fbitsDQ);
	}
}

/* hSmooth table from 4.7.18.7.6, format = Q31 */
static const int hSmoothCoef[MAX_NUM_SMOOTH_COEFS] PROGMEM = {
	0x2aaaaaab, 0x2697a512, 0x1becfa68, 0x0ebdb043, 0x04130598, 
};

/**************************************************************************************
 * Function:    MapHF
 *
 * Description: map HF components to proper QMF bands, with optional gain smoothing
 *                filter (4.6.18.7.6)
 *
 * Inputs:      initialized PSInfoSBR struct
 *              initialized SBRHeader struct for this SCE/CPE block
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              initialized SBRChan struct for this channel
 *              index of current envelope
 *              reset flag (can be non-zero for first envelope only)
 *
 * Outputs:     complete reconstructed subband QMF samples for this envelope
 *
 * Return:      none
 * 
 * Notes:       ensures that output has >= MIN_GBITS_IN_QMFS guard bits,
 *                so it's not necessary to check anything in the synth QMF
 **************************************************************************************/
static void MapHF(PSInfoSBR *psi, SBRHeader *sbrHdr, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int env, int hfReset)
{
	int noiseTabIndex, sinIndex, gainNoiseIndex, hSL;
	int i, iStart, iEnd, m, idx, j, s, n, smre, smim;
	int gFilt, qFilt, xre, xim, gbMask, gbIdx;
	int *XBuf;

	noiseTabIndex =   sbrChan->noiseTabIndex;
	sinIndex =        sbrChan->sinIndex;
	gainNoiseIndex =  sbrChan->gainNoiseIndex;	/* oldest entries in filter delay buffer */

	if (hfReset)
		noiseTabIndex = 2;	/* starts at 1, double since complex */
	hSL = (sbrHdr->smoothMode ? 0 : 4); 

	if (hfReset) {
		for (i = 0; i < hSL; i++) {
			for (m = 0; m < sbrFreq->numQMFBands; m++) {
				sbrChan->gTemp[gainNoiseIndex][m] = psi->gLimBoost[m];
				sbrChan->qTemp[gainNoiseIndex][m] = psi->qmLimBoost[m];
			}
			gainNoiseIndex++;
			if (gainNoiseIndex == MAX_NUM_SMOOTH_COEFS)
				gainNoiseIndex = 0;
		}
		ASSERT(env == 0);	/* should only be reset when env == 0 */
	}

	iStart = sbrGrid->envTimeBorder[env];
	iEnd =   sbrGrid->envTimeBorder[env+1];
	for (i = iStart; i < iEnd; i++) {
		/* save new values in temp buffers (delay)
		 * we only store MAX_NUM_SMOOTH_COEFS most recent values, 
		 *   so don't keep storing the same value over and over
		 */
		if (i - iStart < MAX_NUM_SMOOTH_COEFS) {
			for (m = 0; m < sbrFreq->numQMFBands; m++) {
				sbrChan->gTemp[gainNoiseIndex][m] = psi->gLimBoost[m];
				sbrChan->qTemp[gainNoiseIndex][m] = psi->qmLimBoost[m];
			}
		}

		/* see 4.6.18.7.6 */
		XBuf = psi->XBuf[i + HF_ADJ][sbrFreq->kStart];
		gbMask = 0;
		for (m = 0; m < sbrFreq->numQMFBands; m++) {
			if (env == psi->la || env == sbrChan->laPrev) {
				/* no smoothing filter for gain, and qFilt = 0 (only need to do once) */
				if (i == iStart) {
					psi->gFiltLast[m] = sbrChan->gTemp[gainNoiseIndex][m];
					psi->qFiltLast[m] = 0;
				}
			} else if (hSL == 0) {
				/* no smoothing filter for gain, (only need to do once) */
				if (i == iStart) {
					psi->gFiltLast[m] = sbrChan->gTemp[gainNoiseIndex][m];
					psi->qFiltLast[m] = sbrChan->qTemp[gainNoiseIndex][m];
				}
			} else {
				/* apply smoothing filter to gain and noise (after MAX_NUM_SMOOTH_COEFS, it's always the same) */
				if (i - iStart < MAX_NUM_SMOOTH_COEFS) {
					gFilt = 0;
					qFilt = 0;
					idx = gainNoiseIndex;
					for (j = 0; j < MAX_NUM_SMOOTH_COEFS; j++) {
						/* sum(abs(hSmoothCoef[j])) for all j < 1.0 */
						gFilt += MULSHIFT32(sbrChan->gTemp[idx][m], hSmoothCoef[j]);
						qFilt += MULSHIFT32(sbrChan->qTemp[idx][m], hSmoothCoef[j]);
						idx--;
						if (idx < 0)
							idx += MAX_NUM_SMOOTH_COEFS;
					}
					psi->gFiltLast[m] = gFilt << 1;	/* restore to Q(FBITS_GLIM_BOOST) (gain of filter < 1.0, so no overflow) */
					psi->qFiltLast[m] = qFilt << 1;	/* restore to Q(FBITS_QLIM_BOOST) */
				}
			}

			if (psi->smBoost[m] != 0) {
				/* add scaled signal and sinusoid, don't add noise (qFilt = 0) */
				smre = psi->smBoost[m];
				smim = smre;

				/* sinIndex:  [0] xre += sm   [1] xim += sm*s   [2] xre -= sm   [3] xim -= sm*s  */
				s = (sinIndex >> 1);	/* if 2 or 3, flip sign to subtract sm */
				s <<= 31;
				smre ^= (s >> 31);
				smre -= (s >> 31);
				s ^= ((m + sbrFreq->kStart) << 31);
				smim ^= (s >> 31);
				smim -= (s >> 31);

				/* if sinIndex == 0 or 2, smim = 0; if sinIndex == 1 or 3, smre = 0 */
				s = sinIndex << 31;
				smim &= (s >> 31);
				s ^= 0x80000000;
				smre &= (s >> 31);

				noiseTabIndex += 2;		/* noise filtered by 0, but still need to bump index */
			} else {
				/* add scaled signal and scaled noise */
				qFilt = psi->qFiltLast[m];	
				n = noiseTab[noiseTabIndex++];
				smre = MULSHIFT32(n, qFilt) >> (FBITS_QLIM_BOOST - 1 - FBITS_OUT_QMFA);

				n = noiseTab[noiseTabIndex++];
				smim = MULSHIFT32(n, qFilt) >> (FBITS_QLIM_BOOST - 1 - FBITS_OUT_QMFA);
			}
			noiseTabIndex &= 1023;	/* 512 complex numbers */

			gFilt = psi->gFiltLast[m];
			xre = MULSHIFT32(gFilt, XBuf[0]);
			xim = MULSHIFT32(gFilt, XBuf[1]);
			CLIP_2N_SHIFT30(xre, 32 - FBITS_GLIM_BOOST);
			CLIP_2N_SHIFT30(xim, 32 - FBITS_GLIM_BOOST);

			xre += smre;	*XBuf++ = xre;
			xim += smim;	*XBuf++ = xim;

			gbMask |= FASTABS(xre);
			gbMask |= FASTABS(xim);
		}
		/* update circular buffer index */
		gainNoiseIndex++;
		if (gainNoiseIndex == MAX_NUM_SMOOTH_COEFS)
			gainNoiseIndex = 0;

		sinIndex++;
		sinIndex &= 3;

		/* ensure MIN_GBITS_IN_QMFS guard bits in output
		 * almost never occurs in practice, but checking here makes synth QMF logic very simple
		 */
		if (gbMask >> (31 - MIN_GBITS_IN_QMFS)) {
			XBuf = psi->XBuf[i + HF_ADJ][sbrFreq->kStart];
			for (m = 0; m < sbrFreq->numQMFBands; m++) {
				xre = XBuf[0];	xim = XBuf[1];	
				CLIP_2N(xre, (31 - MIN_GBITS_IN_QMFS));	
				CLIP_2N(xim, (31 - MIN_GBITS_IN_QMFS));	
				*XBuf++ = xre;	*XBuf++ = xim;
			}	
			CLIP_2N(gbMask, (31 - MIN_GBITS_IN_QMFS));	
		}
		gbIdx = ((i + HF_ADJ) >> 5) & 0x01;
		sbrChan->gbMask[gbIdx] |= gbMask;
	}
	sbrChan->noiseTabIndex =  noiseTabIndex;
	sbrChan->sinIndex =       sinIndex;
	sbrChan->gainNoiseIndex = gainNoiseIndex;
}

/**************************************************************************************
 * Function:    AdjustHighFreq
 *
 * Description: adjust high frequencies and add noise and sinusoids (4.6.18.7)
 *
 * Inputs:      initialized PSInfoSBR struct
 *              initialized SBRHeader struct for this SCE/CPE block
 *              initialized SBRGrid struct for this channel
 *              initialized SBRFreq struct for this SCE/CPE block
 *              initialized SBRChan struct for this channel
 *              index of current channel (0 for SCE, 0 or 1 for CPE)
 *
 * Outputs:     complete reconstructed subband QMF samples for this channel
 *
 * Return:      none
 **************************************************************************************/
void AdjustHighFreq(PSInfoSBR *psi, SBRHeader *sbrHdr, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int ch)
{
	int i, env, hfReset;
	unsigned char frameClass, pointer;

	frameClass = sbrGrid->frameClass;
	pointer  = sbrGrid->pointer;

	/* derive la from table 4.159 */
	if ((frameClass == SBR_GRID_FIXVAR || frameClass == SBR_GRID_VARVAR) && pointer > 0)
		psi->la = sbrGrid->numEnv + 1 - pointer;
	else if (frameClass == SBR_GRID_VARFIX && pointer > 1)
		psi->la = pointer - 1;
	else
		psi->la = -1;

	/* for each envelope, estimate gain and adjust SBR QMF bands */
	hfReset = sbrChan->reset;
	for (env = 0; env < sbrGrid->numEnv; env++) {
		EstimateEnvelope(psi, sbrHdr, sbrGrid, sbrFreq, env);
		CalcGain(psi, sbrHdr, sbrGrid, sbrFreq, sbrChan, ch, env);
		MapHF(psi, sbrHdr, sbrGrid, sbrFreq, sbrChan, env, hfReset);
		hfReset = 0;	/* only set for first envelope after header reset */
	}

	/* set saved sine flags to 0 for QMF bands outside of current frequency range */
	for (i = 0; i < sbrFreq->freqLimiter[0] + sbrFreq->kStart; i++)
		sbrChan->addHarmonic[0][i] = 0;
	for (i = sbrFreq->freqLimiter[sbrFreq->nLimiter] + sbrFreq->kStart; i < 64; i++)
		sbrChan->addHarmonic[0][i] = 0;
	sbrChan->addHarmonicFlag[0] = sbrChan->addHarmonicFlag[1];

	/* save la for next frame */
	if (psi->la == sbrGrid->numEnv)
		sbrChan->laPrev = 0;
	else
		sbrChan->laPrev = -1;
}
