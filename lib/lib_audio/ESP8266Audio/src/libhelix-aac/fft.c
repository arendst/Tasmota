/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: fft.c,v 1.1 2005/02/26 01:47:34 jrecker Exp $ 
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
 * Jon Recker (jrecker@real.com), Ken Cooke (kenc@real.com)
 * February 2005
 *
 * fft.c - Ken's optimized radix-4 DIT FFT, optional radix-8 first pass for odd log2(N)
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"

#define NUM_FFT_SIZES	2
static const int nfftTab[NUM_FFT_SIZES] PROGMEM ={64, 512};
static const int nfftlog2Tab[NUM_FFT_SIZES] PROGMEM = {6, 9};

#define SQRT1_2 0x5a82799a	/* sqrt(1/2) in Q31 */

#define swapcplx(p0,p1) \
	t = p0; t1 = *(&(p0)+1); p0 = p1; *(&(p0)+1) = *(&(p1)+1); p1 = t; *(&(p1)+1) = t1

/**************************************************************************************
 * Function:    BitReverse
 *
 * Description: Ken's fast in-place bit reverse, using super-small table
 *
 * Inputs:      buffer of samples
 *              table index (for transform size)
 *
 * Outputs:     bit-reversed samples in same buffer
 *
 * Return:      none
 **************************************************************************************/
 /*__attribute__ ((section (".data"))) */ static void BitReverse(int *inout, int tabidx)
{
    int *part0, *part1;
	int a,b, t,t1;
	const unsigned char* tab = bitrevtab + bitrevtabOffset[tabidx];
	int nbits = nfftlog2Tab[tabidx];

	part0 = inout;
    part1 = inout + (1 << nbits);
	
	while ((a = pgm_read_byte(tab++)) != 0) {
        b = pgm_read_byte(tab++);

        swapcplx(part0[4*a+0], part0[4*b+0]);	/* 0xxx0 <-> 0yyy0 */
        swapcplx(part0[4*a+2], part1[4*b+0]);	/* 0xxx1 <-> 1yyy0 */
        swapcplx(part1[4*a+0], part0[4*b+2]);	/* 1xxx0 <-> 0yyy1 */
        swapcplx(part1[4*a+2], part1[4*b+2]);	/* 1xxx1 <-> 1yyy1 */
    }

    do {
        swapcplx(part0[4*a+2], part1[4*a+0]);	/* 0xxx1 <-> 1xxx0 */
    } while ((a = pgm_read_byte(tab++)) != 0);
	
	
}

/**************************************************************************************
 * Function:    R4FirstPass
 *
 * Description: radix-4 trivial pass for decimation-in-time FFT
 *
 * Inputs:      buffer of (bit-reversed) samples
 *              number of R4 butterflies per group (i.e. nfft / 4)
 *
 * Outputs:     processed samples in same buffer
 *
 * Return:      none
 *
 * Notes:       assumes 2 guard bits, gains no integer bits, 
 *                guard bits out = guard bits in - 2
 **************************************************************************************/
 /* __attribute__ ((section (".data"))) */ static void R4FirstPass(int *x, int bg)
{
    int ar, ai, br, bi, cr, ci, dr, di;
	
	for (; bg != 0; bg--) {

		ar = x[0] + x[2];
		br = x[0] - x[2];
		ai = x[1] + x[3];
		bi = x[1] - x[3];
		cr = x[4] + x[6];
		dr = x[4] - x[6];
		ci = x[5] + x[7];
		di = x[5] - x[7];

		/* max per-sample gain = 4.0 (adding 4 inputs together) */
		x[0] = ar + cr;
		x[4] = ar - cr;
		x[1] = ai + ci;
		x[5] = ai - ci;
		x[2] = br + di;
		x[6] = br - di;
		x[3] = bi - dr;
		x[7] = bi + dr;

		x += 8;
	}
}

/**************************************************************************************
 * Function:    R8FirstPass
 *
 * Description: radix-8 trivial pass for decimation-in-time FFT
 *
 * Inputs:      buffer of (bit-reversed) samples
 *              number of R8 butterflies per group (i.e. nfft / 8)
 *
 * Outputs:     processed samples in same buffer
 *
 * Return:      none
 *
 * Notes:       assumes 3 guard bits, gains 1 integer bit
 *              guard bits out = guard bits in - 3 (if inputs are full scale)
 *                or guard bits in - 2 (if inputs bounded to +/- sqrt(2)/2)
 *              see scaling comments in code
 **************************************************************************************/
 /* __attribute__ ((section (".data"))) */ static void R8FirstPass(int *x, int bg)
{
    int ar, ai, br, bi, cr, ci, dr, di;
	int sr, si, tr, ti, ur, ui, vr, vi;
	int wr, wi, xr, xi, yr, yi, zr, zi;

	for (; bg != 0; bg--) {

		ar = x[0] + x[2];
		br = x[0] - x[2];
		ai = x[1] + x[3];
		bi = x[1] - x[3];
		cr = x[4] + x[6];
		dr = x[4] - x[6];
		ci = x[5] + x[7];
		di = x[5] - x[7];

		sr = ar + cr;
		ur = ar - cr;
		si = ai + ci;
		ui = ai - ci;
		tr = br - di;
		vr = br + di;
		ti = bi + dr;
		vi = bi - dr;

		ar = x[ 8] + x[10];
		br = x[ 8] - x[10];
		ai = x[ 9] + x[11];
		bi = x[ 9] - x[11];
		cr = x[12] + x[14];
		dr = x[12] - x[14];
		ci = x[13] + x[15];
		di = x[13] - x[15];

		/* max gain of wr/wi/yr/yi vs input = 2
		 *  (sum of 4 samples >> 1) 
		 */
		wr = (ar + cr) >> 1;
		yr = (ar - cr) >> 1;
		wi = (ai + ci) >> 1;
		yi = (ai - ci) >> 1;

		/* max gain of output vs input = 4
		 *  (sum of 4 samples >> 1 + sum of 4 samples >> 1) 
		 */
		x[ 0] = (sr >> 1) + wr;
		x[ 8] = (sr >> 1) - wr;
		x[ 1] = (si >> 1) + wi;
		x[ 9] = (si >> 1) - wi;
		x[ 4] = (ur >> 1) + yi;
		x[12] = (ur >> 1) - yi;
		x[ 5] = (ui >> 1) - yr;
		x[13] = (ui >> 1) + yr;

		ar = br - di;
		cr = br + di;
		ai = bi + dr;
		ci = bi - dr;

		/* max gain of xr/xi/zr/zi vs input = 4*sqrt(2)/2 = 2*sqrt(2)
		 *  (sum of 8 samples, multiply by sqrt(2)/2, implicit >> 1 from Q31) 
		 */
		xr = MULSHIFT32(SQRT1_2, ar - ai);
		xi = MULSHIFT32(SQRT1_2, ar + ai);
		zr = MULSHIFT32(SQRT1_2, cr - ci);
		zi = MULSHIFT32(SQRT1_2, cr + ci);

		/* max gain of output vs input = (2 + 2*sqrt(2) ~= 4.83)
		 *  (sum of 4 samples >> 1, plus xr/xi/zr/zi with gain of 2*sqrt(2))
		 * in absolute terms, we have max gain of appx 9.656 (4 + 0.707*8)
		 *  but we also gain 1 int bit (from MULSHIFT32 or from explicit >> 1)
		 */
		x[ 6] = (tr >> 1) - xr;
		x[14] = (tr >> 1) + xr;
		x[ 7] = (ti >> 1) - xi;
		x[15] = (ti >> 1) + xi;
		x[ 2] = (vr >> 1) + zi;
		x[10] = (vr >> 1) - zi;
		x[ 3] = (vi >> 1) - zr;
		x[11] = (vi >> 1) + zr;

		x += 16;
	}
}

/**************************************************************************************
 * Function:    R4Core
 *
 * Description: radix-4 pass for decimation-in-time FFT
 *
 * Inputs:      buffer of samples
 *              number of R4 butterflies per group
 *              number of R4 groups per pass
 *              pointer to twiddle factors tables
 *
 * Outputs:     processed samples in same buffer
 *
 * Return:      none
 *
 * Notes:       gain 2 integer bits per pass (see scaling comments in code)
 *              min 1 GB in
 *              gbOut = gbIn - 1 (short block) or gbIn - 2 (long block)
 *              uses 3-mul, 3-add butterflies instead of 4-mul, 2-add
 **************************************************************************************/
 /* __attribute__ ((section (".data"))) */ static void R4Core(int *x, int bg, int gp, int *wtab)
{
	int ar, ai, br, bi, cr, ci, dr, di, tr, ti;
	int wd, ws, wi;
	int i, j, step;
	int *xptr, *wptr;

	for (; bg != 0; gp <<= 2, bg >>= 2) {

		step = 2*gp;
		xptr = x;

		/* max per-sample gain, per group < 1 + 3*sqrt(2) ~= 5.25 if inputs x are full-scale
		 * do 3 groups for long block, 2 groups for short block (gain 2 int bits per group)
		 *
		 * very conservative scaling:
		 *   group 1: max gain = 5.25,           int bits gained = 2, gb used = 1 (2^3 = 8)
		 *   group 2: max gain = 5.25^2 = 27.6,  int bits gained = 4, gb used = 1 (2^5 = 32)
		 *   group 3: max gain = 5.25^3 = 144.7, int bits gained = 6, gb used = 2 (2^8 = 256)
		 */
		for (i = bg; i != 0; i--) {

			wptr = wtab;

			for (j = gp; j != 0; j--) {

				ar = xptr[0];
				ai = xptr[1];
				xptr += step;
				
				/* gain 2 int bits for br/bi, cr/ci, dr/di (MULSHIFT32 by Q30)
				 * gain 1 net GB
				 */
				ws = wptr[0];
				wi = wptr[1];
				br = xptr[0];
				bi = xptr[1];
				wd = ws + 2*wi;
				tr = MULSHIFT32(wi, br + bi);
				br = MULSHIFT32(wd, br) - tr;	/* cos*br + sin*bi */
				bi = MULSHIFT32(ws, bi) + tr;	/* cos*bi - sin*br */
				xptr += step;
				
				ws = wptr[2];
				wi = wptr[3];
				cr = xptr[0];
				ci = xptr[1];
				wd = ws + 2*wi;
				tr = MULSHIFT32(wi, cr + ci);
				cr = MULSHIFT32(wd, cr) - tr;
				ci = MULSHIFT32(ws, ci) + tr;
				xptr += step;
				
				ws = wptr[4];
				wi = wptr[5];
				dr = xptr[0];
				di = xptr[1];
				wd = ws + 2*wi;
				tr = MULSHIFT32(wi, dr + di);
				dr = MULSHIFT32(wd, dr) - tr;
				di = MULSHIFT32(ws, di) + tr;
				wptr += 6;

				tr = ar;
				ti = ai;
				ar = (tr >> 2) - br;
				ai = (ti >> 2) - bi;
				br = (tr >> 2) + br;
				bi = (ti >> 2) + bi;

				tr = cr;
				ti = ci;
				cr = tr + dr;
				ci = di - ti;
				dr = tr - dr;
				di = di + ti;

				xptr[0] = ar + ci;
				xptr[1] = ai + dr;
				xptr -= step;
				xptr[0] = br - cr;
				xptr[1] = bi - di;
				xptr -= step;
				xptr[0] = ar - ci;
				xptr[1] = ai - dr;
				xptr -= step;
				xptr[0] = br + cr;
				xptr[1] = bi + di;
				xptr += 2;
			}
			xptr += 3*step;
		}
		wtab += 3*step;
	}
}


/**************************************************************************************
 * Function:    R4FFT
 *
 * Description: Ken's very fast in-place radix-4 decimation-in-time FFT
 *
 * Inputs:      table index (for transform size)
 *              buffer of samples (non bit-reversed)
 *
 * Outputs:     processed samples in same buffer
 *
 * Return:      none
 *
 * Notes:       assumes 5 guard bits in for nfft <= 512
 *              gbOut = gbIn - 4 (assuming input is from PreMultiply)
 *              gains log2(nfft) - 2 int bits total
 *                so gain 7 int bits (LONG), 4 int bits (SHORT)
 **************************************************************************************/
void R4FFT(int tabidx, int *x)
{
	int order = nfftlog2Tab[tabidx];
	int nfft = nfftTab[tabidx];

	/* decimation in time */
	BitReverse(x, tabidx);

	if (order & 0x1) {
		/* long block: order = 9, nfft = 512 */
		R8FirstPass(x, nfft >> 3);						/* gain 1 int bit,  lose 2 GB */
		R4Core(x, nfft >> 5, 8, (int *)twidTabOdd);		/* gain 6 int bits, lose 2 GB */
	} else {
		/* short block: order = 6, nfft = 64 */
		R4FirstPass(x, nfft >> 2);						/* gain 0 int bits, lose 2 GB */
		R4Core(x, nfft >> 4, 4, (int *)twidTabEven);	/* gain 4 int bits, lose 1 GB */
	}
}
