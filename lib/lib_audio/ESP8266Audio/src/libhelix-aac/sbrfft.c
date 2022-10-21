/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: sbrfft.c,v 1.1 2005/02/26 01:47:35 jrecker Exp $ 
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
 * sbrfft.c - optimized FFT for SBR QMF filters
 **************************************************************************************/

#include "sbr.h"
#include "assembly.h"

#define SQRT1_2	0x5a82799a

/* swap RE{p0} with RE{p1} and IM{P0} with IM{P1} */
#define swapcplx(p0,p1) \
	t = p0; t1 = *(&(p0)+1); p0 = p1; *(&(p0)+1) = *(&(p1)+1); p1 = t; *(&(p1)+1) = t1

/* nfft = 32, hard coded since small, fixed size FFT
static const unsigned char bitrevtab32[9] = {
	0x01, 0x04, 0x03, 0x06, 0x00, 0x02, 0x05, 0x07, 0x00,
};
*/

/* twiddle table for radix 4 pass, format = Q31 */
static const int twidTabOdd32[8*6] = {
	0x40000000, 0x00000000, 0x40000000, 0x00000000, 0x40000000, 0x00000000, 0x539eba45, 0xe7821d59, 
	0x4b418bbe, 0xf383a3e2, 0x58c542c5, 0xdc71898d, 0x5a82799a, 0xd2bec333, 0x539eba45, 0xe7821d59, 
	0x539eba45, 0xc4df2862, 0x539eba45, 0xc4df2862, 0x58c542c5, 0xdc71898d, 0x3248d382, 0xc13ad060, 
	0x40000000, 0xc0000000, 0x5a82799a, 0xd2bec333, 0x00000000, 0xd2bec333, 0x22a2f4f8, 0xc4df2862, 
	0x58c542c5, 0xcac933ae, 0xcdb72c7e, 0xf383a3e2, 0x00000000, 0xd2bec333, 0x539eba45, 0xc4df2862, 
	0xac6145bb, 0x187de2a7, 0xdd5d0b08, 0xe7821d59, 0x4b418bbe, 0xc13ad060, 0xa73abd3b, 0x3536cc52, 
};

/**************************************************************************************
 * Function:    BitReverse32
 *
 * Description: Ken's fast in-place bit reverse
 *
 * Inputs:      buffer of 32 complex samples
 *
 * Outputs:     bit-reversed samples in same buffer
 *
 * Return:      none
**************************************************************************************/
static void BitReverse32(int *inout)
{
	int t, t1;

	swapcplx(inout[2],  inout[32]);
	swapcplx(inout[4],  inout[16]);
	swapcplx(inout[6],  inout[48]);
	swapcplx(inout[10], inout[40]);
	swapcplx(inout[12], inout[24]);
	swapcplx(inout[14], inout[56]);
	swapcplx(inout[18], inout[36]);
	swapcplx(inout[22], inout[52]);
	swapcplx(inout[26], inout[44]);
	swapcplx(inout[30], inout[60]);
	swapcplx(inout[38], inout[50]);
	swapcplx(inout[46], inout[58]);
}

/**************************************************************************************
 * Function:    R8FirstPass32
 *
 * Description: radix-8 trivial pass for decimation-in-time FFT (log2(N) = 5)
 *
 * Inputs:      buffer of (bit-reversed) samples
 *
 * Outputs:     processed samples in same buffer
 *
 * Return:      none
 *
 * Notes:       assumes 3 guard bits, gains 1 integer bit
 *              guard bits out = guard bits in - 3 (if inputs are full scale)
 *                or guard bits in - 2 (if inputs bounded to +/- sqrt(2)/2)
 *              see scaling comments in fft.c for base AAC
 *              should compile with no stack spills on ARM (verify compiled output)
 *              current instruction count (per pass): 16 LDR, 16 STR, 4 SMULL, 61 ALU
 **************************************************************************************/
static void R8FirstPass32(int *r0)
{
	int r1, r2, r3, r4, r5, r6, r7;
	int r8, r9, r10, r11, r12, r14;

	/* number of passes = fft size / 8 = 32 / 8 = 4 */
	r1 = (32 >> 3);
	do {

		r2 = r0[8];
		r3 = r0[9];
		r4 = r0[10];
		r5 = r0[11];
		r6 = r0[12];
		r7 = r0[13];
		r8 = r0[14];
		r9 = r0[15];

		r10 = r2 + r4;
		r11 = r3 + r5;
		r12 = r6 + r8;
		r14 = r7 + r9;

		r2 -= r4;
		r3 -= r5;
		r6 -= r8;
		r7 -= r9;

		r4 = r2 - r7;
		r5 = r2 + r7;
		r8 = r3 - r6;
		r9 = r3 + r6;

		r2 = r4 - r9;
		r3 = r4 + r9;
		r6 = r5 - r8;
		r7 = r5 + r8;

		r2 = MULSHIFT32(SQRT1_2, r2);	/* can use r4, r5, r8, or r9 for constant and lo32 scratch reg */
		r3 = MULSHIFT32(SQRT1_2, r3);
		r6 = MULSHIFT32(SQRT1_2, r6);
		r7 = MULSHIFT32(SQRT1_2, r7);

		r4 = r10 + r12;
		r5 = r10 - r12;
		r8 = r11 + r14;
		r9 = r11 - r14;
	
		r10 = r0[0];
		r11 = r0[2];
		r12 = r0[4];
		r14 = r0[6];

		r10 += r11;
		r12 += r14;

		r4 >>= 1;
		r10 += r12;
		r4 += (r10 >> 1);
		r0[ 0] = r4;
		r4 -= (r10 >> 1);
		r4 = (r10 >> 1) - r4;
		r0[ 8] = r4;

		r9 >>= 1;
		r10 -= 2*r12;
		r4 = (r10 >> 1) + r9;
		r0[ 4] = r4;
		r4 = (r10 >> 1) - r9;
		r0[12] = r4;
		r10 += r12;

		r10 -= 2*r11;
		r12 -= 2*r14;

		r4 =  r0[1];
		r9 =  r0[3];
		r11 = r0[5];
		r14 = r0[7];

		r4 += r9;
		r11 += r14;

		r8 >>= 1;
		r4 += r11;
		r8 += (r4 >> 1);
		r0[ 1] = r8;
		r8 -= (r4 >> 1);
		r8 = (r4 >> 1) - r8;
		r0[ 9] = r8;

		r5 >>= 1;
		r4 -= 2*r11;
		r8 = (r4 >> 1) - r5;
		r0[ 5] = r8;
		r8 = (r4 >> 1) + r5;
		r0[13] = r8;
		r4 += r11;

		r4 -= 2*r9;
		r11 -= 2*r14;

		r9 = r10 - r11;
		r10 += r11;
		r14 = r4 + r12;
		r4 -= r12;

		r5 = (r10 >> 1) + r7;
		r8 = (r4 >> 1) - r6;
		r0[ 2] = r5;
		r0[ 3] = r8;

		r5 = (r9 >> 1) - r2;
		r8 = (r14 >> 1) - r3;
		r0[ 6] = r5;
		r0[ 7] = r8;

		r5 = (r10 >> 1) - r7;
		r8 = (r4 >> 1) + r6;
		r0[10] = r5;
		r0[11] = r8;
		
		r5 = (r9 >> 1) + r2;
		r8 = (r14 >> 1) + r3;
		r0[14] = r5;
		r0[15] = r8;

		r0 += 16;
		r1--;
	} while (r1 != 0);
}

/**************************************************************************************
 * Function:    R4Core32
 *
 * Description: radix-4 pass for 32-point decimation-in-time FFT
 *
 * Inputs:      buffer of samples
 *
 * Outputs:     processed samples in same buffer
 *
 * Return:      none
 *
 * Notes:       gain 2 integer bits
 *              guard bits out = guard bits in - 1 (if inputs are full scale)
 *              see scaling comments in fft.c for base AAC
 *              uses 3-mul, 3-add butterflies instead of 4-mul, 2-add
 *              should compile with no stack spills on ARM (verify compiled output)
 *              current instruction count (per pass): 16 LDR, 16 STR, 4 SMULL, 61 ALU
 **************************************************************************************/
static void R4Core32(int *r0)
{
	int r2, r3, r4, r5, r6, r7;
	int r8, r9, r10, r12, r14;
	int *r1;

	r1 = (int *)twidTabOdd32;
	r10 = 8;
	do {
		/* can use r14 for lo32 scratch register in all MULSHIFT32 */
		r2 = r1[0];
		r3 = r1[1];
		r4 = r0[16];
		r5 = r0[17];
		r12 = r4 + r5;
		r12 = MULSHIFT32(r3, r12);
		r5  = MULSHIFT32(r2, r5) + r12;
		r2 += 2*r3;
		r4  = MULSHIFT32(r2, r4) - r12;	

		r2 = r1[2];
		r3 = r1[3];
		r6 = r0[32];
		r7 = r0[33];
		r12 = r6 + r7;
		r12 = MULSHIFT32(r3, r12);
		r7  = MULSHIFT32(r2, r7) + r12;
		r2 += 2*r3;
		r6  = MULSHIFT32(r2, r6) - r12;
		
		r2 = r1[4];
		r3 = r1[5];
		r8 = r0[48];
		r9 = r0[49];
		r12 = r8 + r9;
		r12 = MULSHIFT32(r3, r12);
		r9  = MULSHIFT32(r2, r9) + r12;
		r2 += 2*r3;
		r8  = MULSHIFT32(r2, r8) - r12;

		r2 = r0[0];
		r3 = r0[1];

		r12 = r6 + r8;
		r8  = r6 - r8;
		r14 = r9 - r7;
		r9  = r9 + r7;

		r6 = (r2 >> 2) - r4;
		r7 = (r3 >> 2) - r5;
		r4 += (r2 >> 2);
		r5 += (r3 >> 2);

		r2 = r4 + r12;
		r3 = r5 + r9;
		r0[0] = r2;
		r0[1] = r3;
		r2 = r6 - r14;
		r3 = r7 - r8;
		r0[16] = r2;
		r0[17] = r3;
		r2 = r4 - r12;
		r3 = r5 - r9;
		r0[32] = r2;
		r0[33] = r3;
		r2 = r6 + r14;
		r3 = r7 + r8;
		r0[48] = r2;
		r0[49] = r3;

		r0 += 2;
		r1 += 6;
		r10--;
	} while (r10 != 0);
}

/**************************************************************************************
 * Function:    FFT32C
 *
 * Description: Ken's very fast in-place radix-4 decimation-in-time FFT
 *
 * Inputs:      buffer of 32 complex samples (before bit-reversal)
 *
 * Outputs:     processed samples in same buffer
 *
 * Return:      none
 *
 * Notes:       assumes 3 guard bits in, gains 3 integer bits
 *              guard bits out = guard bits in - 2
 *              (guard bit analysis includes assumptions about steps immediately
 *               before and after, i.e. PreMul and PostMul for DCT)
 **************************************************************************************/
void FFT32C(int *x)
{
	/* decimation in time */
	BitReverse32(x);

	/* 32-point complex FFT */
	R8FirstPass32(x);	/* gain 1 int bit,  lose 2 GB (making assumptions about input) */
	R4Core32(x);		/* gain 2 int bits, lose 0 GB (making assumptions about input) */
}
