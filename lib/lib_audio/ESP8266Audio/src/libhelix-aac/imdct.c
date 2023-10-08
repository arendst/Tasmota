/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: imdct.c,v 1.1 2005/02/26 01:47:35 jrecker Exp $ 
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
 * imdct.c - inverse MDCT
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"

#define RND_VAL		(1 << (FBITS_OUT_IMDCT-1))

#ifndef AAC_ENABLE_SBR

/**************************************************************************************
 * Function:    DecWindowOverlap
 *
 * Description: apply synthesis window, do overlap-add, clip to 16-bit PCM,
 *                for winSequence LONG-LONG
 *
 * Inputs:      input buffer (output of type-IV DCT)
 *              overlap buffer (saved from last time)
 *              number of channels
 *              window type (sin or KBD) for input buffer
 *              window type (sin or KBD) for overlap buffer
 *
 * Outputs:     one channel, one frame of 16-bit PCM, interleaved by nChans
 *
 * Return:      none
 *
 * Notes:       this processes one channel at a time, but skips every other sample in
 *                the output buffer (pcm) for stereo interleaving
 *              this should fit in registers on ARM
 *
 * TODO:        ARM5E version with saturating overlap/add (QADD)
 *              asm code with free pointer updates, better load scheduling
 **************************************************************************************/
/*__attribute__ ((section (".data")))*/ static void DecWindowOverlap(int *buf0, int *over0, short *pcm0, int nChans, int winTypeCurr, int winTypePrev)
{
	int in, w0, w1, f0, f1;
	int *buf1, *over1;
	short *pcm1;
	const int *wndPrev, *wndCurr;

	buf0 += (1024 >> 1);
	buf1  = buf0  - 1;
	pcm1  = pcm0 + (1024 - 1) * nChans;
	over1 = over0 + 1024 - 1;

	wndPrev = (winTypePrev == 1 ? kbdWindow + kbdWindowOffset[1] : sinWindow + sinWindowOffset[1]);
	if (winTypeCurr == winTypePrev) {
		/* cut window loads in half since current and overlap sections use same symmetric window */
		do {
			w0 = *wndPrev++;
			w1 = *wndPrev++;
			in = *buf0++;

			f0 = MULSHIFT32(w0, in);
			f1 = MULSHIFT32(w1, in);

			in = *over0;	
			*pcm0 = CLIPTOSHORT( (in - f0 + RND_VAL) >> FBITS_OUT_IMDCT );
			pcm0 += nChans;

			in = *over1;	
			*pcm1 = CLIPTOSHORT( (in + f1 + RND_VAL) >> FBITS_OUT_IMDCT );
			pcm1 -= nChans;

			in = *buf1--;
			*over1-- = MULSHIFT32(w0, in);
			*over0++ = MULSHIFT32(w1, in);
		} while (over0 < over1);
	} else {
		/* different windows for current and overlap parts - should still fit in registers on ARM w/o stack spill */
		wndCurr = (winTypeCurr == 1 ? kbdWindow + kbdWindowOffset[1] : sinWindow + sinWindowOffset[1]);
		do {
			w0 = *wndPrev++;
			w1 = *wndPrev++;
			in = *buf0++;

			f0 = MULSHIFT32(w0, in);
			f1 = MULSHIFT32(w1, in);

			in = *over0;	
			*pcm0 = CLIPTOSHORT( (in - f0 + RND_VAL) >> FBITS_OUT_IMDCT );
			pcm0 += nChans;

			in = *over1;	
			*pcm1 = CLIPTOSHORT( (in + f1 + RND_VAL) >> FBITS_OUT_IMDCT );
			pcm1 -= nChans;

			w0 = *wndCurr++;
			w1 = *wndCurr++;
			in = *buf1--;

			*over1-- = MULSHIFT32(w0, in);
			*over0++ = MULSHIFT32(w1, in);
		} while (over0 < over1);
	}
}

/**************************************************************************************
 * Function:    DecWindowOverlapLongStart
 *
 * Description: apply synthesis window, do overlap-add, clip to 16-bit PCM,
 *                for winSequence LONG-START
 *
 * Inputs:      input buffer (output of type-IV DCT)
 *              overlap buffer (saved from last time)
 *              number of channels
 *              window type (sin or KBD) for input buffer
 *              window type (sin or KBD) for overlap buffer
 *
 * Outputs:     one channel, one frame of 16-bit PCM, interleaved by nChans
 *
 * Return:      none
 *
 * Notes:       this processes one channel at a time, but skips every other sample in
 *                the output buffer (pcm) for stereo interleaving
 *              this should fit in registers on ARM
 *
 * TODO:        ARM5E version with saturating overlap/add (QADD)
 *              asm code with free pointer updates, better load scheduling
 **************************************************************************************/
 /*__attribute__ ((section (".data")))*/ static void DecWindowOverlapLongStart(int *buf0, int *over0, short *pcm0, int nChans, int winTypeCurr, int winTypePrev)
{
	int i,  in, w0, w1, f0, f1;
	int *buf1, *over1;
	short *pcm1;
	const int *wndPrev, *wndCurr;

	buf0 += (1024 >> 1);
	buf1  = buf0  - 1;
	pcm1  = pcm0 + (1024 - 1) * nChans;
	over1 = over0 + 1024 - 1;

	wndPrev = (winTypePrev == 1 ? kbdWindow + kbdWindowOffset[1] : sinWindow + sinWindowOffset[1]);
	i = 448;	/* 2 outputs, 2 overlaps per loop */
	do {
		w0 = *wndPrev++;
		w1 = *wndPrev++;
		in = *buf0++;

		f0 = MULSHIFT32(w0, in);
		f1 = MULSHIFT32(w1, in);

		in = *over0;	
		*pcm0 = CLIPTOSHORT( (in - f0 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm0 += nChans;

		in = *over1;	
		*pcm1 = CLIPTOSHORT( (in + f1 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm1 -= nChans;

		in = *buf1--;

		*over1-- = 0;		/* Wn = 0 for n = (2047, 2046, ... 1600) */
		*over0++ = in >> 1;	/* Wn = 1 for n = (1024, 1025, ... 1471) */
	} while (--i);

	wndCurr = (winTypeCurr == 1 ? kbdWindow + kbdWindowOffset[0] : sinWindow + sinWindowOffset[0]);

	/* do 64 more loops - 2 outputs, 2 overlaps per loop */
	do {
		w0 = *wndPrev++;
		w1 = *wndPrev++;
		in = *buf0++;

		f0 = MULSHIFT32(w0, in);
		f1 = MULSHIFT32(w1, in);

		in = *over0;	
		*pcm0 = CLIPTOSHORT( (in - f0 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm0 += nChans;

		in = *over1;	
		*pcm1 = CLIPTOSHORT( (in + f1 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm1 -= nChans;

		w0 = *wndCurr++;	/* W[0], W[1], ... --> W[255], W[254], ... */
		w1 = *wndCurr++;	/* W[127], W[126], ... --> W[128], W[129], ... */
		in = *buf1--;

		*over1-- = MULSHIFT32(w0, in);	/* Wn = short window for n = (1599, 1598, ... , 1536) */
		*over0++ = MULSHIFT32(w1, in);	/* Wn = short window for n = (1472, 1473, ... , 1535) */
	} while (over0 < over1);
}

/**************************************************************************************
 * Function:    DecWindowOverlapLongStop
 *
 * Description: apply synthesis window, do overlap-add, clip to 16-bit PCM,
 *                for winSequence LONG-STOP
 *
 * Inputs:      input buffer (output of type-IV DCT)
 *              overlap buffer (saved from last time)
 *              number of channels
 *              window type (sin or KBD) for input buffer
 *              window type (sin or KBD) for overlap buffer
 *
 * Outputs:     one channel, one frame of 16-bit PCM, interleaved by nChans
 *
 * Return:      none
 *
 * Notes:       this processes one channel at a time, but skips every other sample in
 *                the output buffer (pcm) for stereo interleaving
 *              this should fit in registers on ARM
 *
 * TODO:        ARM5E version with saturating overlap/add (QADD)
 *              asm code with free pointer updates, better load scheduling
 **************************************************************************************/
 /*__attribute__ ((section (".data")))*/ static void DecWindowOverlapLongStop(int *buf0, int *over0, short *pcm0, int nChans, int winTypeCurr, int winTypePrev)
{
	int i, in, w0, w1, f0, f1;
	int *buf1, *over1;
	short *pcm1;
	const int *wndPrev, *wndCurr;

	buf0 += (1024 >> 1);
	buf1  = buf0  - 1;
	pcm1  = pcm0 + (1024 - 1) * nChans;
	over1 = over0 + 1024 - 1;

	wndPrev = (winTypePrev == 1 ? kbdWindow + kbdWindowOffset[0] : sinWindow + sinWindowOffset[0]);
	wndCurr = (winTypeCurr == 1 ? kbdWindow + kbdWindowOffset[1] : sinWindow + sinWindowOffset[1]);

	i = 448;	/* 2 outputs, 2 overlaps per loop */
	do {
		/* Wn = 0 for n = (0, 1, ... 447) */
		/* Wn = 1 for n = (576, 577, ... 1023) */
		in = *buf0++;
		f1 = in >> 1;	/* scale since skipping multiply by Q31 */

		in = *over0;	
		*pcm0 = CLIPTOSHORT( (in + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm0 += nChans;

		in = *over1;	
		*pcm1 = CLIPTOSHORT( (in + f1 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm1 -= nChans;

		w0 = *wndCurr++;
		w1 = *wndCurr++;
		in = *buf1--;

		*over1-- = MULSHIFT32(w0, in);
		*over0++ = MULSHIFT32(w1, in);
	} while (--i);

	/* do 64 more loops - 2 outputs, 2 overlaps per loop */
	do {
		w0 = *wndPrev++;	/* W[0], W[1], ...W[63] */
		w1 = *wndPrev++;	/* W[127], W[126], ... W[64] */
		in = *buf0++;

		f0 = MULSHIFT32(w0, in);
		f1 = MULSHIFT32(w1, in);

		in = *over0;	
		*pcm0 = CLIPTOSHORT( (in - f0 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm0 += nChans;

		in = *over1;	
		*pcm1 = CLIPTOSHORT( (in + f1 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm1 -= nChans;

		w0 = *wndCurr++;
		w1 = *wndCurr++;	
		in = *buf1--;

		*over1-- = MULSHIFT32(w0, in);
		*over0++ = MULSHIFT32(w1, in);
	} while (over0 < over1);
}

/**************************************************************************************
 * Function:    DecWindowOverlapShort
 *
 * Description: apply synthesis window, do overlap-add, clip to 16-bit PCM,
 *                for winSequence EIGHT-SHORT (does all 8 short blocks)
 *
 * Inputs:      input buffer (output of type-IV DCT)
 *              overlap buffer (saved from last time)
 *              number of channels
 *              window type (sin or KBD) for input buffer
 *              window type (sin or KBD) for overlap buffer
 *
 * Outputs:     one channel, one frame of 16-bit PCM, interleaved by nChans
 *
 * Return:      none
 *
 * Notes:       this processes one channel at a time, but skips every other sample in
 *                the output buffer (pcm) for stereo interleaving
 *              this should fit in registers on ARM
 *
 * TODO:        ARM5E version with saturating overlap/add (QADD)
 *              asm code with free pointer updates, better load scheduling
 **************************************************************************************/
 /*__attribute__ ((section (".data"))) */ static void DecWindowOverlapShort(int *buf0, int *over0, short *pcm0, int nChans, int winTypeCurr, int winTypePrev)
{
	int i, in, w0, w1, f0, f1;
	int *buf1, *over1;
	short *pcm1;
	const int *wndPrev, *wndCurr;

	wndPrev = (winTypePrev == 1 ? kbdWindow + kbdWindowOffset[0] : sinWindow + sinWindowOffset[0]);
	wndCurr = (winTypeCurr == 1 ? kbdWindow + kbdWindowOffset[0] : sinWindow + sinWindowOffset[0]);

	/* pcm[0-447] = 0 + overlap[0-447] */
	i = 448;
	do {
		f0 = *over0++;
		f1 = *over0++;
		*pcm0 = CLIPTOSHORT( (f0 + RND_VAL) >> FBITS_OUT_IMDCT );	pcm0 += nChans;
		*pcm0 = CLIPTOSHORT( (f1 + RND_VAL) >> FBITS_OUT_IMDCT );	pcm0 += nChans;
		i -= 2;
	} while (i);

	/* pcm[448-575] = Wp[0-127] * block0[0-127] + overlap[448-575] */
	pcm1  = pcm0 + (128 - 1) * nChans;
	over1 = over0 + 128 - 1;
	buf0 += 64;
	buf1  = buf0  - 1;
	do {
		w0 = *wndPrev++;	/* W[0], W[1], ...W[63] */
		w1 = *wndPrev++;	/* W[127], W[126], ... W[64] */
		in = *buf0++;

		f0 = MULSHIFT32(w0, in);
		f1 = MULSHIFT32(w1, in);

		in = *over0;	
		*pcm0 = CLIPTOSHORT( (in - f0 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm0 += nChans;

		in = *over1;	
		*pcm1 = CLIPTOSHORT( (in + f1 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm1 -= nChans;

		w0 = *wndCurr++;
		w1 = *wndCurr++;
		in = *buf1--;

		/* save over0/over1 for next short block, in the slots just vacated */
		*over1-- = MULSHIFT32(w0, in);
		*over0++ = MULSHIFT32(w1, in);
	} while (over0 < over1);

	/* pcm[576-703] = Wc[128-255] * block0[128-255] + Wc[0-127] * block1[0-127] + overlap[576-703] 
	 * pcm[704-831] = Wc[128-255] * block1[128-255] + Wc[0-127] * block2[0-127] + overlap[704-831] 
	 * pcm[832-959] = Wc[128-255] * block2[128-255] + Wc[0-127] * block3[0-127] + overlap[832-959] 
	 */
	for (i = 0; i < 3; i++) {
		pcm0 += 64 * nChans;
		pcm1 = pcm0 + (128 - 1) * nChans;
		over0 += 64;
		over1 = over0 + 128 - 1;
		buf0 += 64;
		buf1 = buf0 - 1;
		wndCurr -= 128;

		do {
			w0 = *wndCurr++;	/* W[0], W[1], ...W[63] */
			w1 = *wndCurr++;	/* W[127], W[126], ... W[64] */
			in = *buf0++;

			f0 = MULSHIFT32(w0, in);
			f1 = MULSHIFT32(w1, in);

			in  = *(over0 - 128);	/* from last short block */
			in += *(over0 + 0);		/* from last full frame */
			*pcm0 = CLIPTOSHORT( (in - f0 + RND_VAL) >> FBITS_OUT_IMDCT );
			pcm0 += nChans;

			in  = *(over1 - 128);	/* from last short block */
			in += *(over1 + 0);		/* from last full frame */
			*pcm1 = CLIPTOSHORT( (in + f1 + RND_VAL) >> FBITS_OUT_IMDCT );
			pcm1 -= nChans;

			/* save over0/over1 for next short block, in the slots just vacated */
			in = *buf1--;
			*over1-- = MULSHIFT32(w0, in);
			*over0++ = MULSHIFT32(w1, in);
		} while (over0 < over1);
	}

	/* pcm[960-1023] = Wc[128-191] * block3[128-191] + Wc[0-63]   * block4[0-63] + overlap[960-1023]  
	 * over[0-63]    = Wc[192-255] * block3[192-255] + Wc[64-127] * block4[64-127]
	 */
	pcm0 += 64 * nChans;
	over0 -= 832;				/* points at overlap[64] */
	over1 = over0 + 128 - 1;	/* points at overlap[191] */
	buf0 += 64;
	buf1 = buf0 - 1;
	wndCurr -= 128;
	do {
		w0 = *wndCurr++;	/* W[0], W[1], ...W[63] */
		w1 = *wndCurr++;	/* W[127], W[126], ... W[64] */
		in = *buf0++;

		f0 = MULSHIFT32(w0, in);
		f1 = MULSHIFT32(w1, in);

		in  = *(over0 + 768);	/* from last short block */
		in += *(over0 + 896);	/* from last full frame */
		*pcm0 = CLIPTOSHORT( (in - f0 + RND_VAL) >> FBITS_OUT_IMDCT );
		pcm0 += nChans;

		in  = *(over1 + 768);	/* from last short block */
		*(over1 - 128) = in + f1;

		in = *buf1--;
		*over1-- = MULSHIFT32(w0, in);	/* save in overlap[128-191] */
		*over0++ = MULSHIFT32(w1, in);	/* save in overlap[64-127] */
	} while (over0 < over1);
	
	/* over0 now points at overlap[128] */
	
	/* over[64-191]   = Wc[128-255] * block4[128-255] + Wc[0-127] * block5[0-127] 
	 * over[192-319]  = Wc[128-255] * block5[128-255] + Wc[0-127] * block6[0-127]
	 * over[320-447]  = Wc[128-255] * block6[128-255] + Wc[0-127] * block7[0-127]  
	 * over[448-576]  = Wc[128-255] * block7[128-255]
	 */
	for (i = 0; i < 3; i++) {
		over0 += 64;
		over1 = over0 + 128 - 1;
		buf0 += 64;
		buf1 = buf0 - 1;
		wndCurr -= 128;
		do {
			w0 = *wndCurr++;	/* W[0], W[1], ...W[63] */
			w1 = *wndCurr++;	/* W[127], W[126], ... W[64] */
			in = *buf0++;

			f0 = MULSHIFT32(w0, in);
			f1 = MULSHIFT32(w1, in);

			/* from last short block */
			*(over0 - 128) -= f0;
			*(over1 - 128)+= f1;

			in = *buf1--;
			*over1-- = MULSHIFT32(w0, in);
			*over0++ = MULSHIFT32(w1, in);
		} while (over0 < over1);
	}

	/* over[576-1024] = 0 */ 
	i = 448;
	over0 += 64;
	do {
		*over0++ = 0;
		*over0++ = 0;
		*over0++ = 0;
		*over0++ = 0;
		i -= 4;
	} while (i);
}

#endif	/* !AAC_ENABLE_SBR */

/**************************************************************************************
 * Function:    IMDCT
 *
 * Description: inverse transform and convert to 16-bit PCM
 *
 * Inputs:      valid AACDecInfo struct
 *              index of current channel (0 for SCE/LFE, 0 or 1 for CPE)
 *              output channel (range = [0, nChans-1])
 *
 * Outputs:     complete frame of decoded PCM, after inverse transform
 *
 * Return:      0 if successful, -1 if error
 *
 * Notes:       If AAC_ENABLE_SBR is defined at compile time then window + overlap 
 *                does NOT clip to 16-bit PCM and does NOT interleave channels
 *              If AAC_ENABLE_SBR is NOT defined at compile time, then window + overlap 
 *                does clip to 16-bit PCM and interleaves channels
 *              If SBR is enabled at compile time, but we don't know whether it is
 *                actually used for this frame (e.g. the first frame of a stream),
 *                we need to produce both clipped 16-bit PCM in outbuf AND
 *                unclipped 32-bit PCM in the SBR input buffer. In this case we make
 *                a separate pass over the 32-bit PCM to produce 16-bit PCM output.
 *                This inflicts a slight performance hit when decoding non-SBR files.
 **************************************************************************************/
int IMDCT(AACDecInfo *aacDecInfo, int ch, int chOut, short *outbuf)
{
	int i;
	PSInfoBase *psi;
	ICSInfo *icsInfo;

	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return -1;
	psi = (PSInfoBase *)(aacDecInfo->psInfoBase);
	icsInfo = (ch == 1 && psi->commonWin == 1) ? &(psi->icsInfo[0]) : &(psi->icsInfo[ch]);
	outbuf += chOut;

	/* optimized type-IV DCT (operates inplace) */
	if (icsInfo->winSequence == 2) {
		/* 8 short blocks */
		for (i = 0; i < 8; i++)
			DCT4(0, psi->coef[ch] + i*128, psi->gbCurrent[ch]);
	} else {
		/* 1 long block */
		DCT4(1, psi->coef[ch], psi->gbCurrent[ch]);
	}

#ifdef AAC_ENABLE_SBR
	/* window, overlap-add, don't clip to short (send to SBR decoder) 
	 * store the decoded 32-bit samples in top half (second AAC_MAX_NSAMPS samples) of coef buffer
	 */
	if (icsInfo->winSequence == 0)
		DecWindowOverlapNoClip(psi->coef[ch], psi->overlap[chOut], psi->sbrWorkBuf[ch], icsInfo->winShape, psi->prevWinShape[chOut]);
	else if (icsInfo->winSequence == 1)
		DecWindowOverlapLongStartNoClip(psi->coef[ch], psi->overlap[chOut], psi->sbrWorkBuf[ch], icsInfo->winShape, psi->prevWinShape[chOut]);
	else if (icsInfo->winSequence == 2)
		DecWindowOverlapShortNoClip(psi->coef[ch], psi->overlap[chOut], psi->sbrWorkBuf[ch], icsInfo->winShape, psi->prevWinShape[chOut]);
	else if (icsInfo->winSequence == 3)
		DecWindowOverlapLongStopNoClip(psi->coef[ch], psi->overlap[chOut], psi->sbrWorkBuf[ch], icsInfo->winShape, psi->prevWinShape[chOut]);

	if (!aacDecInfo->sbrEnabled) {
		for (i = 0; i < AAC_MAX_NSAMPS; i++) {
			*outbuf = CLIPTOSHORT((psi->sbrWorkBuf[ch][i] + RND_VAL) >> FBITS_OUT_IMDCT);
			outbuf += aacDecInfo->nChans;
		}
	}

	aacDecInfo->rawSampleBuf[ch] = psi->sbrWorkBuf[ch];
	aacDecInfo->rawSampleBytes = sizeof(int);
	aacDecInfo->rawSampleFBits = FBITS_OUT_IMDCT;
#else
	/* window, overlap-add, round to PCM - optimized for each window sequence */
	if (icsInfo->winSequence == 0)
		DecWindowOverlap(psi->coef[ch], psi->overlap[chOut], outbuf, aacDecInfo->nChans, icsInfo->winShape, psi->prevWinShape[chOut]);
	else if (icsInfo->winSequence == 1)
		DecWindowOverlapLongStart(psi->coef[ch], psi->overlap[chOut], outbuf, aacDecInfo->nChans, icsInfo->winShape, psi->prevWinShape[chOut]);
	else if (icsInfo->winSequence == 2)
		DecWindowOverlapShort(psi->coef[ch], psi->overlap[chOut], outbuf, aacDecInfo->nChans, icsInfo->winShape, psi->prevWinShape[chOut]);
	else if (icsInfo->winSequence == 3)
		DecWindowOverlapLongStop(psi->coef[ch], psi->overlap[chOut], outbuf, aacDecInfo->nChans, icsInfo->winShape, psi->prevWinShape[chOut]);

	aacDecInfo->rawSampleBuf[ch] = 0;
	aacDecInfo->rawSampleBytes = 0;
	aacDecInfo->rawSampleFBits = 0;
#endif

	psi->prevWinShape[chOut] = icsInfo->winShape;

	return 0;
}
