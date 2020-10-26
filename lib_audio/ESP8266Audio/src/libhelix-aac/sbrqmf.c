/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: sbrqmf.c,v 1.2 2005/05/19 20:45:20 jrecker Exp $ 
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
 * sbrqmf.c - analysis and synthesis QMF filters for SBR
 **************************************************************************************/

#include "sbr.h"
#include "assembly.h"

/* PreMultiply64() table
 * format = Q30
 * reordered for sequential access
 *
 * for (i = 0; i < 64/4; i++) {
 *   angle = (i + 0.25) * M_PI / nmdct;
 *   x = (cos(angle) + sin(angle));
 *   x =  sin(angle);
 * 
 *   angle = (nmdct/2 - 1 - i + 0.25) * M_PI / nmdct;
 *   x = (cos(angle) + sin(angle));
 *   x =  sin(angle);
 * }
 */
static const int cos4sin4tab64[64] PROGMEM = {
	0x40c7d2bd, 0x00c90e90, 0x424ff28f, 0x3ff4e5e0, 0x43cdd89a, 0x03ecadcf, 0x454149fc, 0x3fc395f9,
	0x46aa0d6d, 0x070de172, 0x4807eb4b, 0x3f6af2e3, 0x495aada2, 0x0a2abb59, 0x4aa22036, 0x3eeb3347,
	0x4bde1089, 0x0d415013, 0x4d0e4de2, 0x3e44a5ef, 0x4e32a956, 0x104fb80e, 0x4f4af5d1, 0x3d77b192,
	0x50570819, 0x135410c3, 0x5156b6d9, 0x3c84d496, 0x5249daa2, 0x164c7ddd, 0x53304df6, 0x3b6ca4c4,
	0x5409ed4b, 0x19372a64, 0x54d69714, 0x3a2fcee8, 0x55962bc0, 0x1c1249d8, 0x56488dc5, 0x38cf1669,
	0x56eda1a0, 0x1edc1953, 0x57854ddd, 0x374b54ce, 0x580f7b19, 0x2192e09b, 0x588c1404, 0x35a5793c,
	0x58fb0568, 0x2434f332, 0x595c3e2a, 0x33de87de, 0x59afaf4c, 0x26c0b162, 0x59f54bee, 0x31f79948,
	0x5a2d0957, 0x29348937, 0x5a56deec, 0x2ff1d9c7, 0x5a72c63b, 0x2b8ef77d, 0x5a80baf6, 0x2dce88aa,
};

/* PostMultiply64() table
 * format = Q30
 * reordered for sequential access
 *
 * for (i = 0; i <= (32/2); i++) {
 *   angle = i * M_PI / 64;
 *   x = (cos(angle) + sin(angle));
 *   x = sin(angle);
 * }
 */
static const int cos1sin1tab64[34] PROGMEM = {
	0x40000000, 0x00000000, 0x43103085, 0x0323ecbe, 0x45f704f7, 0x0645e9af, 0x48b2b335, 0x09640837, 
	0x4b418bbe, 0x0c7c5c1e, 0x4da1fab5, 0x0f8cfcbe, 0x4fd288dc, 0x1294062f, 0x51d1dc80, 0x158f9a76, 
	0x539eba45, 0x187de2a7, 0x553805f2, 0x1b5d100a, 0x569cc31b, 0x1e2b5d38, 0x57cc15bc, 0x20e70f32, 
	0x58c542c5, 0x238e7673, 0x5987b08a, 0x261feffa, 0x5a12e720, 0x2899e64a, 0x5a6690ae, 0x2afad269, 
	0x5a82799a, 0x2d413ccd,
};

/**************************************************************************************
 * Function:    PreMultiply64
 *
 * Description: pre-twiddle stage of 64-point DCT-IV
 *
 * Inputs:      buffer of 64 samples
 *
 * Outputs:     processed samples in same buffer
 *
 * Return:      none
 *
 * Notes:       minimum 1 GB in, 2 GB out, gains 2 int bits
 *              gbOut = gbIn + 1
 *              output is limited to sqrt(2)/2 plus GB in full GB
 *              uses 3-mul, 3-add butterflies instead of 4-mul, 2-add
 **************************************************************************************/
static void PreMultiply64(int *zbuf1)
{
	int i, ar1, ai1, ar2, ai2, z1, z2;
	int t, cms2, cps2a, sin2a, cps2b, sin2b;
	int *zbuf2;
	const int *csptr;

	zbuf2 = zbuf1 + 64 - 1;
	csptr = cos4sin4tab64;

	/* whole thing should fit in registers - verify that compiler does this */
	for (i = 64 >> 2; i != 0; i--) {
		/* cps2 = (cos+sin), sin2 = sin, cms2 = (cos-sin) */
		cps2a = *csptr++;	
		sin2a = *csptr++;
		cps2b = *csptr++;	
		sin2b = *csptr++;

		ar1 = *(zbuf1 + 0);
		ai2 = *(zbuf1 + 1);
		ai1 = *(zbuf2 + 0);
		ar2 = *(zbuf2 - 1);

		/* gain 2 ints bit from MULSHIFT32 by Q30
		 * max per-sample gain (ignoring implicit scaling) = MAX(sin(angle)+cos(angle)) = 1.414
		 * i.e. gain 1 GB since worst case is sin(angle) = cos(angle) = 0.707 (Q30), gain 2 from
		 *   extra sign bits, and eat one in adding
		 */
		t  = MULSHIFT32(sin2a, ar1 + ai1);
		z2 = MULSHIFT32(cps2a, ai1) - t;
		cms2 = cps2a - 2*sin2a;
		z1 = MULSHIFT32(cms2, ar1) + t;
		*zbuf1++ = z1;	/* cos*ar1 + sin*ai1 */
		*zbuf1++ = z2;	/* cos*ai1 - sin*ar1 */

		t  = MULSHIFT32(sin2b, ar2 + ai2);
		z2 = MULSHIFT32(cps2b, ai2) - t;
		cms2 = cps2b - 2*sin2b;
		z1 = MULSHIFT32(cms2, ar2) + t;
		*zbuf2-- = z2;	/* cos*ai2 - sin*ar2 */
		*zbuf2-- = z1;	/* cos*ar2 + sin*ai2 */
	}
}

/**************************************************************************************
 * Function:    PostMultiply64
 *
 * Description: post-twiddle stage of 64-point type-IV DCT
 *
 * Inputs:      buffer of 64 samples
 *              number of output samples to calculate
 *
 * Outputs:     processed samples in same buffer
 *
 * Return:      none
 *
 * Notes:       minimum 1 GB in, 2 GB out, gains 2 int bits
 *              gbOut = gbIn + 1
 *              output is limited to sqrt(2)/2 plus GB in full GB
 *              nSampsOut is rounded up to next multiple of 4, since we calculate
 *                4 samples per loop
 **************************************************************************************/
static void PostMultiply64(int *fft1, int nSampsOut)
{
	int i, ar1, ai1, ar2, ai2;
	int t, cms2, cps2, sin2;
	int *fft2;
	const int *csptr;

	csptr = cos1sin1tab64;
	fft2 = fft1 + 64 - 1;

	/* load coeffs for first pass
	 * cps2 = (cos+sin)/2, sin2 = sin/2, cms2 = (cos-sin)/2
	 */
	cps2 = *csptr++;
	sin2 = *csptr++;
	cms2 = cps2 - 2*sin2;

	for (i = (nSampsOut + 3) >> 2; i != 0; i--) {
		ar1 = *(fft1 + 0);
		ai1 = *(fft1 + 1);
		ar2 = *(fft2 - 1);
		ai2 = *(fft2 + 0);

		/* gain 2 int bits (multiplying by Q30), max gain = sqrt(2) */
		t = MULSHIFT32(sin2, ar1 + ai1);
		*fft2-- = t - MULSHIFT32(cps2, ai1);
		*fft1++ = t + MULSHIFT32(cms2, ar1);

		cps2 = *csptr++;
		sin2 = *csptr++;

		ai2 = -ai2;
		t = MULSHIFT32(sin2, ar2 + ai2);
		*fft2-- = t - MULSHIFT32(cps2, ai2);
		cms2 = cps2 - 2*sin2;
		*fft1++ = t + MULSHIFT32(cms2, ar2);
	}
}

/**************************************************************************************
 * Function:    QMFAnalysisConv
 *
 * Description: convolution kernel for analysis QMF 
 *
 * Inputs:      pointer to coefficient table, reordered for sequential access
 *              delay buffer of size 32*10 = 320 real-valued PCM samples
 *              index for delay ring buffer (range = [0, 9])
 *
 * Outputs:     64 consecutive 32-bit samples
 *
 * Return:      none
 *
 * Notes:       this is carefully written to be efficient on ARM
 *              use the assembly code version in sbrqmfak.s when building for ARM!
 **************************************************************************************/
#if (defined (__arm) && defined (__ARMCC_VERSION)) || (defined (_WIN32) && defined (_WIN32_WCE) && defined (ARM)) || (defined(__GNUC__) && defined(__arm__))
#ifdef __cplusplus
extern "C"
#endif
void QMFAnalysisConv(int *cTab, int *delay, int dIdx, int *uBuf);
#else
void QMFAnalysisConv(int *cTab, int *delay, int dIdx, int *uBuf)
{
	int k, dOff;
	int *cPtr0, *cPtr1;
	U64 u64lo, u64hi;

	dOff = dIdx*32 + 31;
	cPtr0 = cTab;
	cPtr1 = cTab + 33*5 - 1;

	/* special first pass since we need to flip sign to create cTab[384], cTab[512] */
	u64lo.w64 = 0;
	u64hi.w64 = 0;
	u64lo.w64 = MADD64(u64lo.w64,  *cPtr0++,   delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
	u64hi.w64 = MADD64(u64hi.w64,  *cPtr0++,   delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
	u64lo.w64 = MADD64(u64lo.w64,  *cPtr0++,   delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
	u64hi.w64 = MADD64(u64hi.w64,  *cPtr0++,   delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
	u64lo.w64 = MADD64(u64lo.w64,  *cPtr0++,   delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
	u64hi.w64 = MADD64(u64hi.w64,  *cPtr1--,   delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
	u64lo.w64 = MADD64(u64lo.w64, -(*cPtr1--), delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
	u64hi.w64 = MADD64(u64hi.w64,  *cPtr1--,   delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
	u64lo.w64 = MADD64(u64lo.w64, -(*cPtr1--), delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
	u64hi.w64 = MADD64(u64hi.w64,  *cPtr1--,   delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}

	uBuf[0]  = u64lo.r.hi32;
	uBuf[32] = u64hi.r.hi32;
	uBuf++;
	dOff--;

	/* max gain for any sample in uBuf, after scaling by cTab, ~= 0.99 
	 * so we can just sum the uBuf values with no overflow problems
	 */
	for (k = 1; k <= 31; k++) {
		u64lo.w64 = 0;
		u64hi.w64 = 0;
		u64lo.w64 = MADD64(u64lo.w64, *cPtr0++, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
		u64hi.w64 = MADD64(u64hi.w64, *cPtr0++, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
		u64lo.w64 = MADD64(u64lo.w64, *cPtr0++, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
		u64hi.w64 = MADD64(u64hi.w64, *cPtr0++, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
		u64lo.w64 = MADD64(u64lo.w64, *cPtr0++, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
		u64hi.w64 = MADD64(u64hi.w64, *cPtr1--, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
		u64lo.w64 = MADD64(u64lo.w64, *cPtr1--, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
		u64hi.w64 = MADD64(u64hi.w64, *cPtr1--, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
		u64lo.w64 = MADD64(u64lo.w64, *cPtr1--, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}
		u64hi.w64 = MADD64(u64hi.w64, *cPtr1--, delay[dOff]);	dOff -= 32; if (dOff < 0) {dOff += 320;}

		uBuf[0]  = u64lo.r.hi32;
		uBuf[32] = u64hi.r.hi32;
		uBuf++;
		dOff--;
	}
}
#endif

/**************************************************************************************
 * Function:    QMFAnalysis
 *
 * Description: 32-subband analysis QMF (4.6.18.4.1)
 *
 * Inputs:      32 consecutive samples of decoded 32-bit PCM, format = Q(fBitsIn)
 *              delay buffer of size 32*10 = 320 PCM samples
 *              number of fraction bits in input PCM
 *              index for delay ring buffer (range = [0, 9])
 *              number of subbands to calculate (range = [0, 32])
 *
 * Outputs:     qmfaBands complex subband samples, format = Q(FBITS_OUT_QMFA)
 *              updated delay buffer
 *              updated delay index
 *
 * Return:      guard bit mask
 *
 * Notes:       output stored as RE{X0}, IM{X0}, RE{X1}, IM{X1}, ... RE{X31}, IM{X31}
 *              output stored in int buffer of size 64*2 = 128 
 *                (zero-filled from XBuf[2*qmfaBands] to XBuf[127])
 **************************************************************************************/
int QMFAnalysis(int *inbuf, int *delay, int *XBuf, int fBitsIn, int *delayIdx, int qmfaBands)
{
	int n, y, shift, gbMask;
	int *delayPtr, *uBuf, *tBuf;

	/* use XBuf[128] as temp buffer for reordering */
	uBuf = XBuf;		/* first 64 samples */
	tBuf = XBuf + 64;	/* second 64 samples */

	/* overwrite oldest PCM with new PCM
	 * delay[n] has 1 GB after shifting (either << or >>)
	 */
	delayPtr = delay + (*delayIdx * 32);
	if (fBitsIn > FBITS_IN_QMFA) {
		shift = MIN(fBitsIn - FBITS_IN_QMFA, 31);
		for (n = 32; n != 0; n--) {
			y = (*inbuf) >> shift;
			inbuf++;
			*delayPtr++ = y;
		}
	} else {
		shift = MIN(FBITS_IN_QMFA - fBitsIn, 30);
		for (n = 32; n != 0; n--) {
			y = *inbuf++;
			CLIP_2N_SHIFT30(y, shift);
			*delayPtr++ = y;
		}
	}
	
	QMFAnalysisConv((int *)cTabA, delay, *delayIdx, uBuf);
	
	/* uBuf has at least 2 GB right now (1 from clipping to Q(FBITS_IN_QMFA), one from
	 *   the scaling by cTab (MULSHIFT32(*delayPtr--, *cPtr++), with net gain of < 1.0)
	 * TODO - fuse with QMFAnalysisConv to avoid separate reordering
	 */
    tBuf[2*0 + 0] = uBuf[0];
    tBuf[2*0 + 1] = uBuf[1];
    for (n = 1; n < 31; n++) {
        tBuf[2*n + 0] = -uBuf[64-n];
        tBuf[2*n + 1] =  uBuf[n+1];
    }
    tBuf[2*31 + 1] =  uBuf[32];
    tBuf[2*31 + 0] = -uBuf[33];
	
	/* fast in-place DCT-IV - only need 2*qmfaBands output samples */
	PreMultiply64(tBuf);	/* 2 GB in, 3 GB out */
	FFT32C(tBuf);			/* 3 GB in, 1 GB out */
	PostMultiply64(tBuf, qmfaBands*2);	/* 1 GB in, 2 GB out */

	/* TODO - roll into PostMultiply (if enough registers) */
	gbMask = 0;
	for (n = 0; n < qmfaBands; n++) {
		XBuf[2*n+0] =  tBuf[ n + 0];	/* implicit scaling of 2 in our output Q format */
		gbMask |= FASTABS(XBuf[2*n+0]);
		XBuf[2*n+1] = -tBuf[63 - n];
		gbMask |= FASTABS(XBuf[2*n+1]);
	}

	/* fill top section with zeros for HF generation */
	for (    ; n < 64; n++) {
		XBuf[2*n+0] = 0;
		XBuf[2*n+1] = 0;
	}

	*delayIdx = (*delayIdx == NUM_QMF_DELAY_BUFS - 1 ? 0 : *delayIdx + 1);

	/* minimum of 2 GB in output */
	return gbMask;
}

/* lose FBITS_LOST_DCT4_64 in DCT4, gain 6 for implicit scaling by 1/64, lose 1 for cTab multiply (Q31) */
#define FBITS_OUT_QMFS	(FBITS_IN_QMFS - FBITS_LOST_DCT4_64 + 6 - 1)
#define RND_VAL			(1 << (FBITS_OUT_QMFS-1))

/**************************************************************************************
 * Function:    QMFSynthesisConv
 *
 * Description: final convolution kernel for synthesis QMF 
 *
 * Inputs:      pointer to coefficient table, reordered for sequential access
 *              delay buffer of size 64*10 = 640 complex samples (1280 ints)
 *              index for delay ring buffer (range = [0, 9])
 *              number of QMF subbands to process (range = [0, 64])
 *              number of channels
 *
 * Outputs:     64 consecutive 16-bit PCM samples, interleaved by factor of nChans
 *
 * Return:      none
 *
 * Notes:       this is carefully written to be efficient on ARM
 *              use the assembly code version in sbrqmfsk.s when building for ARM!
 **************************************************************************************/
#if (defined (__arm) && defined (__ARMCC_VERSION)) || (defined (_WIN32) && defined (_WIN32_WCE) && defined (ARM)) || (defined(__GNUC__) && defined(__arm__))
#ifdef __cplusplus
extern "C"
#endif
void QMFSynthesisConv(int *cPtr, int *delay, int dIdx, short *outbuf, int nChans);
#else
void QMFSynthesisConv(int *cPtr, int *delay, int dIdx, short *outbuf, int nChans)
{
	int k, dOff0, dOff1;
	U64 sum64;

	dOff0 = (dIdx)*128;
	dOff1 = dOff0 - 1;
	if (dOff1 < 0)
		dOff1 += 1280;

	/* scaling note: total gain of coefs (cPtr[0]-cPtr[9] for any k) is < 2.0, so 1 GB in delay values is adequate */
	for (k = 0; k <= 63; k++) {
		sum64.w64 = 0;
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff0]);	dOff0 -= 256; if (dOff0 < 0) {dOff0 += 1280;}
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff1]);	dOff1 -= 256; if (dOff1 < 0) {dOff1 += 1280;}
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff0]);	dOff0 -= 256; if (dOff0 < 0) {dOff0 += 1280;}
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff1]);	dOff1 -= 256; if (dOff1 < 0) {dOff1 += 1280;}
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff0]);	dOff0 -= 256; if (dOff0 < 0) {dOff0 += 1280;}
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff1]);	dOff1 -= 256; if (dOff1 < 0) {dOff1 += 1280;}
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff0]);	dOff0 -= 256; if (dOff0 < 0) {dOff0 += 1280;}
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff1]);	dOff1 -= 256; if (dOff1 < 0) {dOff1 += 1280;}
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff0]);	dOff0 -= 256; if (dOff0 < 0) {dOff0 += 1280;}
		sum64.w64 = MADD64(sum64.w64, *cPtr++, delay[dOff1]);	dOff1 -= 256; if (dOff1 < 0) {dOff1 += 1280;}

		dOff0++;
		dOff1--;
		*outbuf = CLIPTOSHORT((sum64.r.hi32 + RND_VAL) >> FBITS_OUT_QMFS);
		outbuf += nChans;
	}
}
#endif

/**************************************************************************************
 * Function:    QMFSynthesis
 *
 * Description: 64-subband synthesis QMF (4.6.18.4.2)
 *
 * Inputs:      64 consecutive complex subband QMF samples, format = Q(FBITS_IN_QMFS)
 *              delay buffer of size 64*10 = 640 complex samples (1280 ints)
 *              index for delay ring buffer (range = [0, 9])
 *              number of QMF subbands to process (range = [0, 64])
 *              number of channels
 *
 * Outputs:     64 consecutive 16-bit PCM samples, interleaved by factor of nChans
 *              updated delay buffer
 *              updated delay index
 *
 * Return:      none
 *
 * Notes:       assumes MIN_GBITS_IN_QMFS guard bits in input, either from
 *                QMFAnalysis (if upsampling only) or from MapHF (if SBR on)
 **************************************************************************************/
void QMFSynthesis(int *inbuf, int *delay, int *delayIdx, int qmfsBands, short *outbuf, int nChans)
{
	int n, a0, a1, b0, b1, dOff0, dOff1, dIdx;
	int *tBufLo, *tBufHi;

	dIdx = *delayIdx;
	tBufLo = delay + dIdx*128 + 0;
	tBufHi = delay + dIdx*128 + 127;

	/* reorder inputs to DCT-IV, only use first qmfsBands (complex) samples 
	 * TODO - fuse with PreMultiply64 to avoid separate reordering steps
	 */
    for (n = 0; n < qmfsBands >> 1; n++) {
		a0 = *inbuf++;
		b0 = *inbuf++;
		a1 = *inbuf++;
		b1 = *inbuf++;
		*tBufLo++ = a0;
        *tBufLo++ = a1;
        *tBufHi-- = b0;
        *tBufHi-- = b1;
    }
	if (qmfsBands & 0x01) {
		a0 = *inbuf++;
		b0 = *inbuf++;
		*tBufLo++ = a0;
        *tBufHi-- = b0;
        *tBufLo++ = 0;
		*tBufHi-- = 0;
		n++;
	}
    for (     ; n < 32; n++) {
		*tBufLo++ = 0;
        *tBufHi-- = 0;
        *tBufLo++ = 0;
        *tBufHi-- = 0;
	}

	tBufLo = delay + dIdx*128 + 0;
	tBufHi = delay + dIdx*128 + 64;

	/* 2 GB in, 3 GB out */
	PreMultiply64(tBufLo);
	PreMultiply64(tBufHi);

	/* 3 GB in, 1 GB out */
	FFT32C(tBufLo);
	FFT32C(tBufHi);

	/* 1 GB in, 2 GB out */
	PostMultiply64(tBufLo, 64);
	PostMultiply64(tBufHi, 64);

	/* could fuse with PostMultiply64 to avoid separate pass */
	dOff0 = dIdx*128;
	dOff1 = dIdx*128 + 64;
	for (n = 32; n != 0; n--) {
		a0 =  (*tBufLo++);
		a1 =  (*tBufLo++);
		b0 =  (*tBufHi++);
		b1 = -(*tBufHi++);

		delay[dOff0++] = (b0 - a0);
		delay[dOff0++] = (b1 - a1);
		delay[dOff1++] = (b0 + a0);
		delay[dOff1++] = (b1 + a1);
	}

	QMFSynthesisConv((int *)cTabS, delay, dIdx, outbuf, nChans);

	*delayIdx = (*delayIdx == NUM_QMF_DELAY_BUFS - 1 ? 0 : *delayIdx + 1);
}
