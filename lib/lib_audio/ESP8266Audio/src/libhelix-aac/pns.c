/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: pns.c,v 1.2 2005/03/10 17:01:56 jrecker Exp $ 
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
 * pns.c - perceptual noise substitution
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"

/**************************************************************************************
 * Function:    Get32BitVal
 *
 * Description: generate 32-bit unsigned random number
 *
 * Inputs:      last number calculated (seed, first time through)
 *
 * Outputs:     new number, saved in *last
 *
 * Return:      32-bit number, uniformly distributed between [0, 2^32)
 *
 * Notes:       uses simple linear congruential generator
 **************************************************************************************/
static unsigned int Get32BitVal(unsigned int *last)
{
	unsigned int r = *last;

	/* use same coefs as MPEG reference code (classic LCG)
	 * use unsigned multiply to force reliable wraparound behavior in C (mod 2^32)
	 */
	r = (1664525U * r) + 1013904223U;
	*last = r;

	return r;
}


#define NUM_ITER_INVSQRT	4

#define X0_COEF_2	0xc0000000	/* Q29: -2.0 */
#define X0_OFF_2	0x60000000	/* Q29:  3.0 */
#define Q26_3		0x0c000000	/* Q26:  3.0 */

/**************************************************************************************
 * Function:    InvRootR
 *
 * Description: use Newton's method to solve for x = 1/sqrt(r)
 *
 * Inputs:      r in Q30 format, range = [0.25, 1] (normalize inputs to this range)
 *
 * Outputs:     none
 *
 * Return:      x = Q29, range = (1, 2)
 *
 * Notes:       guaranteed to converge and not overflow for any r in this range
 *              
 *              xn+1  = xn - f(xn)/f'(xn)
 *              f(x)  = 1/sqrt(r) - x = 0 (find root)
 *                    = 1/x^2 - r
 *              f'(x) = -2/x^3
 *
 *              so xn+1 = xn/2 * (3 - r*xn^2)
 *
 *              NUM_ITER_INVSQRT = 3, maxDiff = 1.3747e-02
 *              NUM_ITER_INVSQRT = 4, maxDiff = 3.9832e-04
 **************************************************************************************/
static int InvRootR(int r)
{
	int i, xn, t;

	/* use linear equation for initial guess
	 * x0 = -2*r + 3 (so x0 always >= correct answer in range [0.25, 1))
	 * xn = Q29 (at every step)
	 */
	xn = (MULSHIFT32(r, X0_COEF_2) << 2) + X0_OFF_2;

	for (i = 0; i < NUM_ITER_INVSQRT; i++) {
		t = MULSHIFT32(xn, xn);					/* Q26 = Q29*Q29 */
		t = Q26_3 - (MULSHIFT32(r, t) << 2);	/* Q26 = Q26 - (Q31*Q26 << 1) */
		xn = MULSHIFT32(xn, t) << (6 - 1);		/* Q29 = (Q29*Q26 << 6), and -1 for division by 2 */
	}

	/* clip to range (1.0, 2.0) 
	 * (because of rounding, this can converge to xn slightly > 2.0 when r is near 0.25)
	 */
	if (xn >> 30)
		xn = (1 << 30) - 1;

	return xn;
}

/**************************************************************************************
 * Function:    ScaleNoiseVector
 *
 * Description: apply scaling to vector of noise coefficients for one scalefactor band
 *
 * Inputs:      unscaled coefficients
 *              number of coefficients in vector (one scalefactor band of coefs)
 *              scalefactor for this band (i.e. noise energy)
 *
 * Outputs:     nVals coefficients in Q(FBITS_OUT_DQ_OFF)
 *
 * Return:      guard bit mask (OR of abs value of all noise coefs)
 **************************************************************************************/
static int ScaleNoiseVector(int *coef, int nVals, int sf)
{

/* pow(2, i/4.0) for i = [0,1,2,3], format = Q30 */
static const int pow14[4] PROGMEM = { 
	0x40000000, 0x4c1bf829, 0x5a82799a, 0x6ba27e65
};

	int i, c, spec, energy, sq, scalef, scalei, invSqrtEnergy, z, gbMask;
	
	energy = 0;
	for (i = 0; i < nVals; i++) {
		spec = coef[i];

		/* max nVals = max SFB width = 96, so energy can gain < 2^7 bits in accumulation */
		sq = (spec * spec) >> 8;		/* spec*spec range = (-2^30, 2^30) */
		energy += sq;
	}

	/* unless nVals == 1 (or the number generator is broken...), this should not happen */
	if (energy == 0)
		return 0;	/* coef[i] must = 0 for i = [0, nVals-1], so gbMask = 0 */

	/* pow(2, sf/4) * pow(2, FBITS_OUT_DQ_OFF) */
	scalef = pow14[sf & 0x3];
	scalei = (sf >> 2) + FBITS_OUT_DQ_OFF;

	/* energy has implied factor of 2^-8 since we shifted the accumulator 
	 * normalize energy to range [0.25, 1.0), calculate 1/sqrt(1), and denormalize
	 *   i.e. divide input by 2^(30-z) and convert to Q30
	 *        output of 1/sqrt(i) now has extra factor of 2^((30-z)/2)
	 *        for energy > 0, z is an even number between 0 and 28
	 * final scaling of invSqrtEnergy:
	 *  2^(15 - z/2) to compensate for implicit 2^(30-z) factor in input
	 *  +4 to compensate for implicit 2^-8 factor in input
	 */
	z = CLZ(energy) - 2;					/* energy has at least 2 leading zeros (see acc loop) */
	z &= 0xfffffffe;						/* force even */
	invSqrtEnergy = InvRootR(energy << z);	/* energy << z must be in range [0x10000000, 0x40000000] */
	scalei -= (15 - z/2 + 4);				/* nInt = 1/sqrt(energy) in Q29 */

	/* normalize for final scaling */
	z = CLZ(invSqrtEnergy) - 1;
	invSqrtEnergy <<= z;
	scalei -= (z - 3 - 2);	/* -2 for scalef, z-3 for invSqrtEnergy */
	scalef = MULSHIFT32(scalef, invSqrtEnergy);	/* scalef (input) = Q30, invSqrtEnergy = Q29 * 2^z */
	gbMask = 0;

	if (scalei < 0) {
		scalei = -scalei;
		if (scalei > 31)
			scalei = 31;
		for (i = 0; i < nVals; i++) {
			c = MULSHIFT32(coef[i], scalef) >> scalei;
			gbMask |= FASTABS(c);
			coef[i] = c;
		}
	} else {
		/* for scalei <= 16, no clipping possible (coef[i] is < 2^15 before scaling) 
		 * for scalei > 16, just saturate exponent (rare)
		 *   scalef is close to full-scale (since we normalized invSqrtEnergy)
		 * remember, we are just producing noise here
		 */
		if (scalei > 16)
			scalei = 16;
		for (i = 0; i < nVals; i++) {
			c = MULSHIFT32(coef[i] << scalei, scalef);
			coef[i] = c;
			gbMask |= FASTABS(c);
		}
	}

	return gbMask;
}

/**************************************************************************************
 * Function:    GenerateNoiseVector
 *
 * Description: create vector of noise coefficients for one scalefactor band
 *
 * Inputs:      seed for number generator
 *              number of coefficients to generate
 *
 * Outputs:     buffer of nVals coefficients, range = [-2^15, 2^15)
 *              updated seed for number generator
 *
 * Return:      none
 **************************************************************************************/
static void GenerateNoiseVector(int *coef, int *last, int nVals)
{
	int i;
	
	for (i = 0; i < nVals; i++)
		coef[i] = ((signed int)Get32BitVal((unsigned int *)last)) >> 16;
}

/**************************************************************************************
 * Function:    CopyNoiseVector
 *
 * Description: copy vector of noise coefficients for one scalefactor band from L to R
 *
 * Inputs:      buffer of left coefficients
 *              number of coefficients to copy
 *
 * Outputs:     buffer of right coefficients
 *
 * Return:      none
 **************************************************************************************/
static void CopyNoiseVector(int *coefL, int *coefR, int nVals)
{
	int i;

	for (i = 0; i < nVals; i++)
		coefR[i] = coefL[i];
}

/**************************************************************************************
 * Function:    PNS
 *
 * Description: apply perceptual noise substitution, if enabled (MPEG-4 only)
 *
 * Inputs:      valid AACDecInfo struct
 *              index of current channel
 *
 * Outputs:     shaped noise in scalefactor bands where PNS is active
 *              updated minimum guard bit count for this channel
 *
 * Return:      0 if successful, -1 if error
 **************************************************************************************/
int PNS(AACDecInfo *aacDecInfo, int ch)
{
	int gp, sfb, win, width, nSamps, gb, gbMask;
	int *coef;
	const /*short*/ int *sfbTab;
	unsigned char *sfbCodeBook;
	short *scaleFactors;
	int msMaskOffset, checkCorr, genNew;
	unsigned char msMask;
	unsigned char *msMaskPtr;
	PSInfoBase *psi;
	ICSInfo *icsInfo;
	
	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return -1;
	psi = (PSInfoBase *)(aacDecInfo->psInfoBase);
	icsInfo = (ch == 1 && psi->commonWin == 1) ? &(psi->icsInfo[0]) : &(psi->icsInfo[ch]);
	
	if (!psi->pnsUsed[ch])
		return 0;
	
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
	checkCorr = (aacDecInfo->currBlockID == AAC_ID_CPE && psi->commonWin == 1 ? 1 : 0);
	
	gbMask = 0;
	for (gp = 0; gp < icsInfo->numWinGroup; gp++) {
		for (win = 0; win < icsInfo->winGroupLen[gp]; win++) {
			msMaskPtr = psi->msMaskBits + ((gp*icsInfo->maxSFB) >> 3);
			msMaskOffset = ((gp*icsInfo->maxSFB) & 0x07);
			msMask = (*msMaskPtr++) >> msMaskOffset;
			
			for (sfb = 0; sfb < icsInfo->maxSFB; sfb++) {
				width = sfbTab[sfb+1] - sfbTab[sfb];
				if (sfbCodeBook[sfb] == 13) {
					if (ch == 0) {
						/* generate new vector, copy into ch 1 if it's possible that the channels will be correlated 
						 * if ch 1 has PNS enabled for this SFB but it's uncorrelated (i.e. ms_used == 0),
						 *    the copied values will be overwritten when we process ch 1
						 */
						GenerateNoiseVector(coef, &psi->pnsLastVal, width);
						if (checkCorr && psi->sfbCodeBook[1][gp*icsInfo->maxSFB + sfb] == 13)
							CopyNoiseVector(coef, psi->coef[1] + (coef - psi->coef[0]), width);
					} else {
						/* generate new vector if no correlation between channels */
						genNew = 1;
						if (checkCorr && psi->sfbCodeBook[0][gp*icsInfo->maxSFB + sfb] == 13) {
							if ( (psi->msMaskPresent == 1 && (msMask & 0x01)) || psi->msMaskPresent == 2 )
								genNew = 0;
						}
						if (genNew)
							GenerateNoiseVector(coef, &psi->pnsLastVal, width);
					}
					gbMask |= ScaleNoiseVector(coef, width, psi->scaleFactors[ch][gp*icsInfo->maxSFB + sfb]);
				}
				coef += width;
				
				/* get next mask bit (should be branchless on ARM) */
				msMask >>= 1;
				if (++msMaskOffset == 8) {
					msMask = *msMaskPtr++;
					msMaskOffset = 0;
				}
			}
			coef += (nSamps - sfbTab[icsInfo->maxSFB]);
		}
		sfbCodeBook += icsInfo->maxSFB;
		scaleFactors += icsInfo->maxSFB;
	}
	
	/* update guard bit count if necessary */
	gb = CLZ(gbMask) - 1;
	if (psi->gbCurrent[ch] > gb)
		psi->gbCurrent[ch] = gb;
	
	return 0;
}
