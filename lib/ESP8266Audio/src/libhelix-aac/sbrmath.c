/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: sbrmath.c,v 1.1 2005/02/26 01:47:35 jrecker Exp $ 
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
 * sbrmath.c - fixed-point math functions for SBR
 **************************************************************************************/

#include "sbr.h"
#include "assembly.h"

#define Q28_2	0x20000000	/* Q28: 2.0 */
#define Q28_15	0x30000000	/* Q28: 1.5 */

#define NUM_ITER_IRN		5

/**************************************************************************************
 * Function:    InvRNormalized
 *
 * Description: use Newton's method to solve for x = 1/r
 *
 * Inputs:      r = Q31, range = [0.5, 1) (normalize your inputs to this range)
 *
 * Outputs:     none
 *
 * Return:      x = Q29, range ~= [1.0, 2.0]
 *
 * Notes:       guaranteed to converge and not overflow for any r in [0.5, 1)
 * 
 *              xn+1  = xn - f(xn)/f'(xn)
 *              f(x)  = 1/r - x = 0 (find root)
 *                    = 1/x - r
 *              f'(x) = -1/x^2
 *
 *              so xn+1 = xn - (1/xn - r) / (-1/xn^2)
 *                      = xn * (2 - r*xn)
 *
 *              NUM_ITER_IRN = 2, maxDiff = 6.2500e-02 (precision of about 4 bits)
 *              NUM_ITER_IRN = 3, maxDiff = 3.9063e-03 (precision of about 8 bits)
 *              NUM_ITER_IRN = 4, maxDiff = 1.5288e-05 (precision of about 16 bits)
 *              NUM_ITER_IRN = 5, maxDiff = 3.0034e-08 (precision of about 24 bits)
 **************************************************************************************/
int InvRNormalized(int r)
{
	int i, xn, t;

	/* r =   [0.5, 1.0) 
	 * 1/r = (1.0, 2.0] 
	 *   so use 1.5 as initial guess 
	 */
	xn = Q28_15;

	/* xn = xn*(2.0 - r*xn) */
	for (i = NUM_ITER_IRN; i != 0; i--) {
		t = MULSHIFT32(r, xn);			/* Q31*Q29 = Q28 */
		t = Q28_2 - t;					/* Q28 */
		xn = MULSHIFT32(xn, t) << 4;	/* Q29*Q28 << 4 = Q29 */
	}

	return xn;
}

#define NUM_TERMS_RPI	5
#define LOG2_EXP_INV	0x58b90bfc	/* 1/log2(e), Q31 */

/* invTab[x] = 1/(x+1), format = Q30 */
static const int invTab[NUM_TERMS_RPI] PROGMEM = {0x40000000, 0x20000000, 0x15555555, 0x10000000, 0x0ccccccd};

/**************************************************************************************
 * Function:    RatioPowInv
 *
 * Description: use Taylor (MacLaurin) series expansion to calculate (a/b) ^ (1/c)
 *
 * Inputs:      a = [1, 64], b = [1, 64], c = [1, 64], a >= b
 *
 * Outputs:     none
 *
 * Return:      y = Q24, range ~= [0.015625, 64]
 **************************************************************************************/
int RatioPowInv(int a, int b, int c)
{
	int lna, lnb, i, p, t, y;

	if (a < 1 || b < 1 || c < 1 || a > 64 || b > 64 || c > 64 || a < b)
		return 0;

	lna = MULSHIFT32(log2Tab[a], LOG2_EXP_INV) << 1;	/* ln(a), Q28 */
	lnb = MULSHIFT32(log2Tab[b], LOG2_EXP_INV) << 1;	/* ln(b), Q28 */
	p = (lna - lnb) / c;	/* Q28 */

	/* sum in Q24 */
	y = (1 << 24);
	t = p >> 4;		/* t = p^1 * 1/1! (Q24)*/
	y += t;

	for (i = 2; i <= NUM_TERMS_RPI; i++) {
		t = MULSHIFT32(invTab[i-1], t) << 2;
		t = MULSHIFT32(p, t) << 4;	/* t = p^i * 1/i! (Q24) */
		y += t;
	}

	return y;
}

/**************************************************************************************
 * Function:    SqrtFix
 *
 * Description: use binary search to calculate sqrt(q)
 *
 * Inputs:      q = Q30
 *              number of fraction bits in input
 *
 * Outputs:     number of fraction bits in output
 *
 * Return:      lo = Q(fBitsOut)
 *
 * Notes:       absolute precision varies depending on fBitsIn
 *              normalizes input to range [0x200000000, 0x7fffffff] and takes 
 *                floor(sqrt(input)), and sets fBitsOut appropriately
 **************************************************************************************/
int SqrtFix(int q, int fBitsIn, int *fBitsOut)
{
	int z, lo, hi, mid;

	if (q <= 0) {
		*fBitsOut = fBitsIn;
		return 0;
	}

	/* force even fBitsIn */
	z = fBitsIn & 0x01;
	q >>= z;
	fBitsIn -= z;

	/* for max precision, normalize to [0x20000000, 0x7fffffff] */
	z = (CLZ(q) - 1);
	z >>= 1;
	q <<= (2*z);

	/* choose initial bounds */
	lo = 1;
	if (q >= 0x10000000)
		lo = 16384;	/* (int)sqrt(0x10000000) */
	hi = 46340;		/* (int)sqrt(0x7fffffff) */

	/* do binary search with 32x32->32 multiply test */
	do {
		mid = (lo + hi) >> 1;
		if (mid*mid > q)
			hi = mid - 1;
		else
			lo = mid + 1;
	} while (hi >= lo);
	lo--;

	*fBitsOut = ((fBitsIn + 2*z) >> 1);
	return lo;
}
