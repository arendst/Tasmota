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
 * stproc.c - mid-side and intensity (MPEG1 and MPEG2) stereo processing
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"

/**************************************************************************************
 * Function:    MidSideProc
 *
 * Description: sum-difference stereo reconstruction
 *
 * Inputs:      vector x with dequantized samples from left and right channels
 *              number of non-zero samples (MAX of left and right)
 *              assume 1 guard bit in input
 *              guard bit mask (left and right channels)
 *
 * Outputs:     updated sample vector x
 *              updated guard bit mask
 *
 * Return:      none
 *
 * Notes:       assume at least 1 GB in input
 **************************************************************************************/
void MidSideProc(int x[MAX_NCHAN][MAX_NSAMP], int nSamps, int mOut[2])  
{
	int i, xr, xl, mOutL, mOutR;
	
	/* L = (M+S)/sqrt(2), R = (M-S)/sqrt(2) 
	 * NOTE: 1/sqrt(2) done in DequantChannel() - see comments there
	 */
	mOutL = mOutR = 0;
	for(i = 0; i < nSamps; i++) {
		xl = x[0][i];
		xr = x[1][i];
		x[0][i] = xl + xr;
		x[1][i] = xl - xr;
		mOutL |= FASTABS(x[0][i]);
		mOutR |= FASTABS(x[1][i]);
	}
	mOut[0] |= mOutL;
	mOut[1] |= mOutR;
}

/**************************************************************************************
 * Function:    IntensityProcMPEG1
 *
 * Description: intensity stereo processing for MPEG1
 *
 * Inputs:      vector x with dequantized samples from left and right channels
 *              number of non-zero samples in left channel
 *              valid FrameHeader struct
 *              two each of ScaleFactorInfoSub, CriticalBandInfo structs (both channels)
 *              flags indicating midSide on/off, mixedBlock on/off
 *              guard bit mask (left and right channels)
 *
 * Outputs:     updated sample vector x
 *              updated guard bit mask
 *
 * Return:      none
 *
 * Notes:       assume at least 1 GB in input
 *
 * TODO:        combine MPEG1/2 into one function (maybe)
 *              make sure all the mixed-block and IIP logic is right
 **************************************************************************************/
void IntensityProcMPEG1(int x[MAX_NCHAN][MAX_NSAMP], int nSamps, FrameHeader *fh, ScaleFactorInfoSub *sfis, 
						CriticalBandInfo *cbi, int midSideFlag, int mixFlag, int mOut[2])
{
	int i=0, j=0, n=0, cb=0, w=0;
	int sampsLeft, isf, mOutL, mOutR, xl, xr;
	int fl, fr, fls[3], frs[3];
	int cbStartL=0, cbStartS=0, cbEndL=0, cbEndS=0;
	int *isfTab;
	(void)mixFlag;

	/* NOTE - this works fine for mixed blocks, as long as the switch point starts in the
	 *  short block section (i.e. on or after sample 36 = sfBand->l[8] = 3*sfBand->s[3]
	 * is this a safe assumption?
	 * TODO - intensity + mixed not quite right (diff = 11 on he_mode)
	 *  figure out correct implementation (spec ambiguous about when to do short block reorder)
	 */
	if (cbi[1].cbType == 0) {
		/* long block */
		cbStartL = cbi[1].cbEndL + 1;
		cbEndL =   cbi[0].cbEndL + 1;
		cbStartS = cbEndS = 0;
		i = fh->sfBand->l[cbStartL];
	} else if (cbi[1].cbType == 1 || cbi[1].cbType == 2) {
		/* short or mixed block */
		cbStartS = cbi[1].cbEndSMax + 1;
		cbEndS =   cbi[0].cbEndSMax + 1;
		cbStartL = cbEndL = 0;
		i = 3 * fh->sfBand->s[cbStartS];
	}

	sampsLeft = nSamps - i;		/* process to length of left */
	isfTab = (int *)ISFMpeg1[midSideFlag];
	mOutL = mOutR = 0;

	/* long blocks */
	for (cb = cbStartL; cb < cbEndL && sampsLeft > 0; cb++) {
		isf = sfis->l[cb];
		if (isf == 7) {
			fl = ISFIIP[midSideFlag][0];
			fr = ISFIIP[midSideFlag][1];
		} else {
			fl = isfTab[isf];	
			fr = isfTab[6] - isfTab[isf];
		}

		n = fh->sfBand->l[cb + 1] - fh->sfBand->l[cb];
		for (j = 0; j < n && sampsLeft > 0; j++, i++) {
			xr = MULSHIFT32(fr, x[0][i]) << 2;	x[1][i] = xr; mOutR |= FASTABS(xr);
			xl = MULSHIFT32(fl, x[0][i]) << 2;	x[0][i] = xl; mOutL |= FASTABS(xl);
			sampsLeft--;
		}
	}

	/* short blocks */
	for (cb = cbStartS; cb < cbEndS && sampsLeft >= 3; cb++) {
		for (w = 0; w < 3; w++) {
			isf = sfis->s[cb][w];
			if (isf == 7) {
				fls[w] = ISFIIP[midSideFlag][0];
				frs[w] = ISFIIP[midSideFlag][1];
			} else {
				fls[w] = isfTab[isf];
				frs[w] = isfTab[6] - isfTab[isf];
			}
		}

		n = fh->sfBand->s[cb + 1] - fh->sfBand->s[cb];
		for (j = 0; j < n && sampsLeft >= 3; j++, i+=3) {
			xr = MULSHIFT32(frs[0], x[0][i+0]) << 2;	x[1][i+0] = xr;	mOutR |= FASTABS(xr);
			xl = MULSHIFT32(fls[0], x[0][i+0]) << 2;	x[0][i+0] = xl;	mOutL |= FASTABS(xl);
			xr = MULSHIFT32(frs[1], x[0][i+1]) << 2;	x[1][i+1] = xr;	mOutR |= FASTABS(xr);
			xl = MULSHIFT32(fls[1], x[0][i+1]) << 2;	x[0][i+1] = xl;	mOutL |= FASTABS(xl);
			xr = MULSHIFT32(frs[2], x[0][i+2]) << 2;	x[1][i+2] = xr;	mOutR |= FASTABS(xr);
			xl = MULSHIFT32(fls[2], x[0][i+2]) << 2;	x[0][i+2] = xl;	mOutL |= FASTABS(xl);
			sampsLeft -= 3;
		}
	}
	mOut[0] = mOutL;
	mOut[1] = mOutR;
	
	return;
}

/**************************************************************************************
 * Function:    IntensityProcMPEG2
 *
 * Description: intensity stereo processing for MPEG2
 *
 * Inputs:      vector x with dequantized samples from left and right channels
 *              number of non-zero samples in left channel
 *              valid FrameHeader struct
 *              two each of ScaleFactorInfoSub, CriticalBandInfo structs (both channels)
 *              ScaleFactorJS struct with joint stereo info from UnpackSFMPEG2()
 *              flags indicating midSide on/off, mixedBlock on/off
 *              guard bit mask (left and right channels)
 *
 * Outputs:     updated sample vector x
 *              updated guard bit mask
 *
 * Return:      none
 *
 * Notes:       assume at least 1 GB in input
 *
 * TODO:        combine MPEG1/2 into one function (maybe)
 *              make sure all the mixed-block and IIP logic is right
 *                probably redo IIP logic to be simpler
 **************************************************************************************/
void IntensityProcMPEG2(int x[MAX_NCHAN][MAX_NSAMP], int nSamps, FrameHeader *fh, ScaleFactorInfoSub *sfis, 
						CriticalBandInfo *cbi, ScaleFactorJS *sfjs, int midSideFlag, int mixFlag, int mOut[2])
{
	int i, j, k, n, r, cb, w;
	int fl, fr, mOutL, mOutR, xl, xr;
	int sampsLeft;
	int isf, sfIdx, tmp, il[23];
	int *isfTab;
	int cbStartL, cbStartS, cbEndL, cbEndS;

	(void)mixFlag;
	
	isfTab = (int *)ISFMpeg2[sfjs->intensityScale][midSideFlag];
	mOutL = mOutR = 0;

	/* fill buffer with illegal intensity positions (depending on slen) */
	for (k = r = 0; r < 4; r++) {
		tmp = (1 << sfjs->slen[r]) - 1;
		for (j = 0; j < sfjs->nr[r]; j++, k++) 
			il[k] = tmp;
	}

	if (cbi[1].cbType == 0) {
		/* long blocks */
		il[21] = il[22] = 1;
		cbStartL = cbi[1].cbEndL + 1;	/* start at end of right */
		cbEndL =   cbi[0].cbEndL + 1;	/* process to end of left */
		i = fh->sfBand->l[cbStartL];
		sampsLeft = nSamps - i;

		for(cb = cbStartL; cb < cbEndL; cb++) {
			sfIdx = sfis->l[cb];
			if (sfIdx == il[cb]) {
				fl = ISFIIP[midSideFlag][0];
				fr = ISFIIP[midSideFlag][1];
			} else {
				isf = (sfis->l[cb] + 1) >> 1;
				fl = isfTab[(sfIdx & 0x01 ? isf : 0)];
				fr = isfTab[(sfIdx & 0x01 ? 0 : isf)];
			}
			n = MIN(fh->sfBand->l[cb + 1] - fh->sfBand->l[cb], sampsLeft);

			for(j = 0; j < n; j++, i++) {
				xr = MULSHIFT32(fr, x[0][i]) << 2;	x[1][i] = xr;	mOutR |= FASTABS(xr);
				xl = MULSHIFT32(fl, x[0][i]) << 2;	x[0][i] = xl;	mOutL |= FASTABS(xl);
			}

			/* early exit once we've used all the non-zero samples */
			sampsLeft -= n;
			if (sampsLeft == 0)		
				break;
		}
	} else {
		/* short or mixed blocks */
		il[12] = 1;

		for(w = 0; w < 3; w++) {
			cbStartS = cbi[1].cbEndS[w] + 1;		/* start at end of right */
			cbEndS =   cbi[0].cbEndS[w] + 1;		/* process to end of left */
			i = 3 * fh->sfBand->s[cbStartS] + w;

			/* skip through sample array by 3, so early-exit logic would be more tricky */
			for(cb = cbStartS; cb < cbEndS; cb++) {
				sfIdx = sfis->s[cb][w];
				if (sfIdx == il[cb]) {
					fl = ISFIIP[midSideFlag][0];
					fr = ISFIIP[midSideFlag][1];
				} else {
					isf = (sfis->s[cb][w] + 1) >> 1;
					fl = isfTab[(sfIdx & 0x01 ? isf : 0)];
					fr = isfTab[(sfIdx & 0x01 ? 0 : isf)];
				}
				n = fh->sfBand->s[cb + 1] - fh->sfBand->s[cb];

				for(j = 0; j < n; j++, i+=3) {
					xr = MULSHIFT32(fr, x[0][i]) << 2;	x[1][i] = xr;	mOutR |= FASTABS(xr);
					xl = MULSHIFT32(fl, x[0][i]) << 2;	x[0][i] = xl;	mOutL |= FASTABS(xl);
				}
			}
		}
	}
	mOut[0] = mOutL;
	mOut[1] = mOutR;

	return;
}

