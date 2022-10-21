/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: sbr.h,v 1.2 2005/05/20 18:05:41 jrecker Exp $ 
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
 * sbr.h - definitions of platform-specific SBR data structures, functions, and tables
 **************************************************************************************/

#ifndef _SBR_H
#define _SBR_H

#include "aaccommon.h"
#include "bitstream.h"

#ifndef ASSERT
#if defined(_WIN32) && defined(_M_IX86) && (defined (_DEBUG) || defined (REL_ENABLE_ASSERTS))
#define ASSERT(x) if (!(x)) __asm int 3;
#else
#define ASSERT(x) /* do nothing */
#endif
#endif

#ifndef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))
#endif

#define NUM_TIME_SLOTS			16
#define SAMPLES_PER_SLOT		2	/* RATE in spec */
#define NUM_SAMPLE_RATES_SBR	9	/* downsampled (single-rate) mode unsupported, so only use Fs_sbr >= 16 kHz */

#define MAX_NUM_ENV					5
#define MAX_NUM_NOISE_FLOORS		2
#define MAX_NUM_NOISE_FLOOR_BANDS	5	/* max Nq, see 4.6.18.3.6 */
#define MAX_NUM_PATCHES				5
#define MAX_NUM_SMOOTH_COEFS		5

#define HF_GEN			8
#define HF_ADJ			2

#define MAX_QMF_BANDS	48		/* max QMF subbands covered by SBR (4.6.18.3.6) */

#define FBITS_IN_QMFA	14
#define FBITS_LOST_QMFA	(1 + 2 + 3 + 2 + 1)	/* 1 from cTab, 2 in premul, 3 in FFT, 2 in postmul, 1 for implicit scaling by 2.0 */
#define FBITS_OUT_QMFA	(FBITS_IN_QMFA - FBITS_LOST_QMFA)

#define MIN_GBITS_IN_QMFS		2
#define FBITS_IN_QMFS			FBITS_OUT_QMFA
#define FBITS_LOST_DCT4_64		(2 + 3 + 2)		/* 2 in premul, 3 in FFT, 2 in postmul */

#define FBITS_OUT_DQ_ENV	29	/* dequantized env scalefactors are Q(29 - envDataDequantScale) */
#define FBITS_OUT_DQ_NOISE	24	/* range of Q_orig = [2^-24, 2^6] */
#define NOISE_FLOOR_OFFSET	6

/* see comments in ApplyBoost() */
#define FBITS_GLIM_BOOST	24
#define FBITS_QLIM_BOOST	14

#define MAX_HUFF_BITS		20
#define NUM_QMF_DELAY_BUFS	10
#define DELAY_SAMPS_QMFA	(NUM_QMF_DELAY_BUFS * 32)
#define DELAY_SAMPS_QMFS	(NUM_QMF_DELAY_BUFS * 128)

/* additional external symbols to name-mangle for static linking */
#define FFT32C							STATNAME(FFT32C)
#define CalcFreqTables					STATNAME(CalcFreqTables)
#define AdjustHighFreq					STATNAME(AdjustHighFreq)
#define GenerateHighFreq				STATNAME(GenerateHighFreq)
#define DecodeSBREnvelope				STATNAME(DecodeSBREnvelope)
#define DecodeSBRNoise					STATNAME(DecodeSBRNoise)
#define UncoupleSBREnvelope				STATNAME(UncoupleSBREnvelope)
#define UncoupleSBRNoise				STATNAME(UncoupleSBRNoise)
#define InvRNormalized					STATNAME(InvRNormalized)
#define RatioPowInv						STATNAME(RatioPowInv)
#define SqrtFix							STATNAME(SqrtFix)
#define QMFAnalysis						STATNAME(QMFAnalysis)
#define QMFSynthesis					STATNAME(QMFSynthesis)
#define GetSampRateIdx					STATNAME(GetSampRateIdx)
#define UnpackSBRHeader					STATNAME(UnpackSBRHeader)
#define UnpackSBRSingleChannel			STATNAME(UnpackSBRSingleChannel)
#define UnpackSBRChannelPair			STATNAME(UnpackSBRChannelPair)

/* asm functions */
#define CVKernel1						STATNAME(CVKernel1)
#define CVKernel2						STATNAME(CVKernel2)
#define QMFAnalysisConv					STATNAME(QMFAnalysisConv)
#define QMFSynthesisConv				STATNAME(QMFSynthesisConv)

#define k0Tab							STATNAME(k0Tab)
#define k2Tab							STATNAME(k2Tab)
#define goalSBTab						STATNAME(goalSBTab)
#define huffTabSBR						STATNAME(huffTabSBR)
#define huffTabSBRInfo					STATNAME(huffTabSBRInfo)
#define log2Tab							STATNAME(log2Tab)
#define noiseTab						STATNAME(noiseTab)
#define cTabA							STATNAME(cTabA)
#define cTabS							STATNAME(cTabS)

/* do y <<= n, clipping to range [-2^30, 2^30 - 1] (i.e. output has one guard bit) */
#define CLIP_2N_SHIFT30(y, n) { \
	int sign = (y) >> 31;  \
	if (sign != (y) >> (30 - (n)))  { \
		(y) = sign ^ (0x3fffffff); \
	} else { \
		(y) = (y) << (n); \
	} \
}

/*
#define CLIP_2N(y, n) { \
	int sign = (y) >> 31;  \
	if (sign != ((y) >> (n)))  { \
		(y) = sign ^ ((1 << (n)) - 1); \
	} \
}
*/

enum {
	SBR_GRID_FIXFIX = 0,
	SBR_GRID_FIXVAR = 1,
	SBR_GRID_VARFIX = 2,
	SBR_GRID_VARVAR = 3
};

enum {
	HuffTabSBR_tEnv15 =    0,
	HuffTabSBR_fEnv15 =    1,
	HuffTabSBR_tEnv15b =   2,
	HuffTabSBR_fEnv15b =   3,
	HuffTabSBR_tEnv30 =    4,
	HuffTabSBR_fEnv30 =    5,
	HuffTabSBR_tEnv30b =   6,
	HuffTabSBR_fEnv30b =   7,
	HuffTabSBR_tNoise30 =  8,
	HuffTabSBR_fNoise30 =  5,
	HuffTabSBR_tNoise30b = 9,
	HuffTabSBR_fNoise30b = 7
};

typedef struct _HuffInfo {
	int maxBits;							/* number of bits in longest codeword */
    unsigned /*char*/ int count[MAX_HUFF_BITS];		/* count[i] = number of codes with length i+1 bits */
	int offset;								/* offset into symbol table */
} HuffInfo;

/* need one SBRHeader per element (SCE/CPE), updated only on new header */
typedef struct _SBRHeader {
	int                   count;

	unsigned char         ampRes;
	unsigned char         startFreq;
	unsigned char         stopFreq;
	unsigned char         crossOverBand;
	unsigned char         resBitsHdr;
	unsigned char         hdrExtra1;
	unsigned char         hdrExtra2;

	unsigned char         freqScale;
	unsigned char         alterScale;
	unsigned char         noiseBands;
	
	unsigned char         limiterBands;
	unsigned char         limiterGains;
	unsigned char         interpFreq;
	unsigned char         smoothMode;
} SBRHeader;

/* need one SBRGrid per channel, updated every frame */
typedef struct _SBRGrid {
	unsigned char         frameClass;
	unsigned char         ampResFrame;
	unsigned char         pointer;

	unsigned char         numEnv;						/* L_E */
	unsigned char         envTimeBorder[MAX_NUM_ENV+1];	/* t_E */
	unsigned char         freqRes[MAX_NUM_ENV];			/* r */

	unsigned char         numNoiseFloors;							/* L_Q */
	unsigned char         noiseTimeBorder[MAX_NUM_NOISE_FLOORS+1];	/* t_Q */

	unsigned char         numEnvPrev;
	unsigned char         numNoiseFloorsPrev;
	unsigned char         freqResPrev;
} SBRGrid;

/* need one SBRFreq per element (SCE/CPE/LFE), updated only on header reset */
typedef struct _SBRFreq {
	int                   kStart;				/* k_x */
	int                   nMaster;
	int                   nHigh;
	int                   nLow;
	int                   nLimiter;				/* N_l */
	int                   numQMFBands;			/* M */
	int                   numNoiseFloorBands;	/* Nq */

	int                   kStartPrev;
	int                   numQMFBandsPrev;

	unsigned char         freqMaster[MAX_QMF_BANDS + 1];	/* not necessary to save this  after derived tables are generated */
	unsigned char         freqHigh[MAX_QMF_BANDS + 1];
	unsigned char         freqLow[MAX_QMF_BANDS / 2 + 1];	/* nLow = nHigh - (nHigh >> 1) */
	unsigned char         freqNoise[MAX_NUM_NOISE_FLOOR_BANDS+1];
	unsigned char         freqLimiter[MAX_QMF_BANDS / 2 + MAX_NUM_PATCHES];		/* max (intermediate) size = nLow + numPatches - 1 */

	unsigned char         numPatches;
	unsigned char         patchNumSubbands[MAX_NUM_PATCHES + 1];
	unsigned char         patchStartSubband[MAX_NUM_PATCHES + 1];
} SBRFreq;

typedef struct _SBRChan {
	int                   reset;
	unsigned char         deltaFlagEnv[MAX_NUM_ENV];
	unsigned char         deltaFlagNoise[MAX_NUM_NOISE_FLOORS];

	signed char           envDataQuant[MAX_NUM_ENV][MAX_QMF_BANDS];		/* range = [0, 127] */
	signed char           noiseDataQuant[MAX_NUM_NOISE_FLOORS][MAX_NUM_NOISE_FLOOR_BANDS];

	unsigned char         invfMode[2][MAX_NUM_NOISE_FLOOR_BANDS];	/* invfMode[0/1][band] = prev/curr */
	int                   chirpFact[MAX_NUM_NOISE_FLOOR_BANDS];		/* bwArray */
	unsigned char         addHarmonicFlag[2];						/* addHarmonicFlag[0/1] = prev/curr */
	unsigned char         addHarmonic[2][64];						/* addHarmonic[0/1][band] = prev/curr */
	
	int                   gbMask[2];	/* gbMask[0/1] = XBuf[0-31]/XBuf[32-39] */
	signed char           laPrev;

	int                   noiseTabIndex;
	int                   sinIndex;
	int                   gainNoiseIndex;
	int                   gTemp[MAX_NUM_SMOOTH_COEFS][MAX_QMF_BANDS];
	int                   qTemp[MAX_NUM_SMOOTH_COEFS][MAX_QMF_BANDS];

} SBRChan;

typedef struct _PSInfoSBR {
	/* save for entire file */
	int                   frameCount;
	int                   sampRateIdx;

	/* state info that must be saved for each channel */
	SBRHeader             sbrHdr[AAC_MAX_NCHANS];
	SBRGrid               sbrGrid[AAC_MAX_NCHANS];
	SBRFreq               sbrFreq[AAC_MAX_NCHANS];
	SBRChan               sbrChan[AAC_MAX_NCHANS];

	/* temp variables, no need to save between blocks */
	unsigned char         dataExtra;
	unsigned char         resBitsData;
	unsigned char         extendedDataPresent;
	int                   extendedDataSize;

	signed char           envDataDequantScale[MAX_NCHANS_ELEM][MAX_NUM_ENV];
	int                   envDataDequant[MAX_NCHANS_ELEM][MAX_NUM_ENV][MAX_QMF_BANDS];
	int                   noiseDataDequant[MAX_NCHANS_ELEM][MAX_NUM_NOISE_FLOORS][MAX_NUM_NOISE_FLOOR_BANDS];

	int                   eCurr[MAX_QMF_BANDS];
	unsigned char         eCurrExp[MAX_QMF_BANDS];
	unsigned char         eCurrExpMax;
	signed char           la;

	int                   crcCheckWord;
	int                   couplingFlag;
	int                   envBand;
	int                   eOMGainMax;
	int                   gainMax;
	int                   gainMaxFBits;
	int                   noiseFloorBand;
	int                   qp1Inv;
	int                   qqp1Inv;
	int                   sMapped;
	int                   sBand;
	int                   highBand;

	int                   sumEOrigMapped;
	int                   sumECurrGLim;
	int                   sumSM;
	int                   sumQM;
	int                   gLimBoost[MAX_QMF_BANDS];
	int                   qmLimBoost[MAX_QMF_BANDS];
	int                   smBoost[MAX_QMF_BANDS];

	int                   smBuf[MAX_QMF_BANDS];
	int                   qmLimBuf[MAX_QMF_BANDS];
	int                   gLimBuf[MAX_QMF_BANDS];
	int                   gLimFbits[MAX_QMF_BANDS];

	int                   gFiltLast[MAX_QMF_BANDS];
	int                   qFiltLast[MAX_QMF_BANDS];

	/* large buffers */
	int                   delayIdxQMFA[AAC_MAX_NCHANS];
	int                   delayQMFA[AAC_MAX_NCHANS][DELAY_SAMPS_QMFA];
	int                   delayIdxQMFS[AAC_MAX_NCHANS];
	int                   delayQMFS[AAC_MAX_NCHANS][DELAY_SAMPS_QMFS];
	int                   XBufDelay[AAC_MAX_NCHANS][HF_GEN][64][2];
	int                   XBuf[32+8][64][2];

} PSInfoSBR;

/* sbrfft.c */
void FFT32C(int *x);

/* sbrfreq.c */
int CalcFreqTables(SBRHeader *sbrHdr, SBRFreq *sbrFreq, int sampRateIdx);

/* sbrhfadj.c */
void AdjustHighFreq(PSInfoSBR *psi, SBRHeader *sbrHdr, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int ch);

/* sbrhfgen.c */
void GenerateHighFreq(PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int ch);

/* sbrhuff.c */
void DecodeSBREnvelope(BitStreamInfo *bsi, PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int ch);
void DecodeSBRNoise(BitStreamInfo *bsi, PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChan, int ch);
void UncoupleSBREnvelope(PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChanR);
void UncoupleSBRNoise(PSInfoSBR *psi, SBRGrid *sbrGrid, SBRFreq *sbrFreq, SBRChan *sbrChanR);

/* sbrmath.c */
int InvRNormalized(int r);
int RatioPowInv(int a, int b, int c);
int SqrtFix(int x, int fBitsIn, int *fBitsOut);

/* sbrqmf.c */
int QMFAnalysis(int *inbuf, int *delay, int *XBuf, int fBitsIn, int *delayIdx, int qmfaBands);
void QMFSynthesis(int *inbuf, int *delay, int *delayIdx, int qmfsBands, short *outbuf, int nChans);

/* sbrside.c */
int GetSampRateIdx(int sampRate);
int UnpackSBRHeader(BitStreamInfo *bsi, SBRHeader *sbrHdr);
void UnpackSBRSingleChannel(BitStreamInfo *bsi, PSInfoSBR *psi, int chOut);
void UnpackSBRChannelPair(BitStreamInfo *bsi, PSInfoSBR *psi, int chOut);

/* sbrtabs.c */
extern const unsigned char k0Tab[NUM_SAMPLE_RATES_SBR][16];
extern const unsigned char k2Tab[NUM_SAMPLE_RATES_SBR][14];
extern const unsigned char goalSBTab[NUM_SAMPLE_RATES_SBR];
extern const HuffInfo huffTabSBRInfo[10];
extern const signed int /*short*/ huffTabSBR[604];
extern const int log2Tab[65];
extern const int noiseTab[512*2];
extern const int cTabA[165];
extern const int cTabS[640];

#endif	/* _SBR_H */
