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
 * coder.h - private, implementation-specific header file
 **************************************************************************************/

#ifndef _CODER_H
#define _CODER_H

#pragma GCC optimize ("O3")

#include "mp3common.h"

#if defined(ASSERT)
#undef ASSERT
#endif
#if defined(_WIN32) && defined(_M_IX86) && (defined (_DEBUG) || defined (REL_ENABLE_ASSERTS))
#define ASSERT(x) if (!(x)) __asm int 3;
#else
#define ASSERT(x) /* do nothing */
#endif

#ifndef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))
#endif

/* clip to range [-2^n, 2^n - 1] */
#define CLIP_2N(y, n) { \
	int sign = (y) >> 31;  \
	if (sign != (y) >> (n))  { \
		(y) = sign ^ ((1 << (n)) - 1); \
	} \
}

#define SIBYTES_MPEG1_MONO		17
#define SIBYTES_MPEG1_STEREO	32
#define SIBYTES_MPEG2_MONO		 9
#define SIBYTES_MPEG2_STEREO	17

/* number of fraction bits for pow43Tab (see comments there) */
#define POW43_FRACBITS_LOW		22
#define POW43_FRACBITS_HIGH		12

#define DQ_FRACBITS_OUT			25	/* number of fraction bits in output of dequant */
#define	IMDCT_SCALE				2	/* additional scaling (by sqrt(2)) for fast IMDCT36 */

#define	HUFF_PAIRTABS			32
#define BLOCK_SIZE				18
#define	NBANDS					32
#define MAX_REORDER_SAMPS		((192-126)*3)		/* largest critical band for short blocks (see sfBandTable) */
#define VBUF_LENGTH				(17 * 2 * NBANDS)	/* for double-sized vbuf FIFO */

/* additional external symbols to name-mangle for static linking */
#define	SetBitstreamPointer	STATNAME(SetBitstreamPointer)
#define	GetBits				STATNAME(GetBits)
#define	CalcBitsUsed		STATNAME(CalcBitsUsed)
#define	DequantChannel		STATNAME(DequantChannel)
#define	MidSideProc			STATNAME(MidSideProc)
#define	IntensityProcMPEG1	STATNAME(IntensityProcMPEG1)
#define	IntensityProcMPEG2	STATNAME(IntensityProcMPEG2)
#define PolyphaseMono		STATNAME(PolyphaseMono)
#define PolyphaseStereo		STATNAME(PolyphaseStereo)
#define FDCT32				STATNAME(FDCT32)

#define	ISFMpeg1			STATNAME(ISFMpeg1)
#define	ISFMpeg2			STATNAME(ISFMpeg2)
#define	ISFIIP				STATNAME(ISFIIP)
#define uniqueIDTab			STATNAME(uniqueIDTab)
#define	coef32				STATNAME(coef32)
#define	polyCoef			STATNAME(polyCoef)
#define	csa					STATNAME(csa)
#define	imdctWin			STATNAME(imdctWin)

#define	huffTable			STATNAME(huffTable)
#define	huffTabOffset		STATNAME(huffTabOffset)
#define	huffTabLookup		STATNAME(huffTabLookup)
#define	quadTable			STATNAME(quadTable)
#define	quadTabOffset		STATNAME(quadTabOffset)
#define	quadTabMaxBits		STATNAME(quadTabMaxBits)

/* map these to the corresponding 2-bit values in the frame header */
typedef enum {
	Stereo = 0x00,	/* two independent channels, but L and R frames might have different # of bits */
	Joint = 0x01,	/* coupled channels - layer III: mix of M-S and intensity, Layers I/II: intensity and direct coding only */
	Dual = 0x02,	/* two independent channels, L and R always have exactly 1/2 the total bitrate */
	Mono = 0x03		/* one channel */
} StereoMode;

typedef struct _BitStreamInfo {
	unsigned char *bytePtr;
	unsigned int iCache;
	int cachedBits;
	int nBytes;
} BitStreamInfo;

typedef struct _FrameHeader {
    MPEGVersion ver;	/* version ID */
    int layer;			/* layer index (1, 2, or 3) */
    int crc;			/* CRC flag: 0 = disabled, 1 = enabled */
    int brIdx;			/* bitrate index (0 - 15) */
    int srIdx;			/* sample rate index (0 - 2) */
    int paddingBit;		/* padding flag: 0 = no padding, 1 = single pad byte */
    int privateBit;		/* unused */
    StereoMode sMode;	/* mono/stereo mode */
    int modeExt;		/* used to decipher joint stereo mode */
    int copyFlag;		/* copyright flag: 0 = no, 1 = yes */
    int origFlag;		/* original flag: 0 = copy, 1 = original */
    int emphasis;		/* deemphasis mode */
    int CRCWord;		/* CRC word (16 bits, 0 if crc not enabled) */

	const SFBandTable *sfBand;
} FrameHeader;

typedef struct _SideInfoSub {
    int part23Length;		/* number of bits in main data */ 
    int nBigvals;			/* 2x this = first set of Huffman cw's (maximum amplitude can be > 1) */
    int globalGain;			/* overall gain for dequantizer */
    int sfCompress;			/* unpacked to figure out number of bits in scale factors */
    int winSwitchFlag;		/* window switching flag */
    int blockType;			/* block type */
    int mixedBlock;			/* 0 = regular block (all short or long), 1 = mixed block */
    int tableSelect[3];		/* index of Huffman tables for the big values regions */
    int subBlockGain[3];	/* subblock gain offset, relative to global gain */
    int region0Count;		/* 1+region0Count = num scale factor bands in first region of bigvals */
    int region1Count;		/* 1+region1Count = num scale factor bands in second region of bigvals */
    int preFlag;			/* for optional high frequency boost */
    int sfactScale;			/* scaling of the scalefactors */
    int count1TableSelect;	/* index of Huffman table for quad codewords */
} SideInfoSub;

typedef struct _SideInfo {
	int mainDataBegin;
	int privateBits;
	int scfsi[MAX_NCHAN][MAX_SCFBD];				/* 4 scalefactor bands per channel */
	
	SideInfoSub	sis[MAX_NGRAN][MAX_NCHAN];
} SideInfo;

typedef struct {
    int cbType;		/* pure long = 0, pure short = 1, mixed = 2 */
    int cbEndS[3];	/* number nonzero short cb's, per subbblock */
	int cbEndSMax;	/* max of cbEndS[] */
    int cbEndL;		/* number nonzero long cb's  */
} CriticalBandInfo;

typedef struct _DequantInfo {
	int workBuf[MAX_REORDER_SAMPS];		/* workbuf for reordering short blocks */
	CriticalBandInfo cbi[MAX_NCHAN];	/* filled in dequantizer, used in joint stereo reconstruction */
} DequantInfo;

typedef struct _HuffmanInfo {
	int huffDecBuf[MAX_NCHAN][MAX_NSAMP];		/* used both for decoded Huffman values and dequantized coefficients */
	int nonZeroBound[MAX_NCHAN];				/* number of coeffs in huffDecBuf[ch] which can be > 0 */
	int gb[MAX_NCHAN];							/* minimum number of guard bits in huffDecBuf[ch] */
} HuffmanInfo;

typedef enum _HuffTabType {
	noBits,
	oneShot,
	loopNoLinbits,
	loopLinbits,
	quadA,
	quadB,
	invalidTab
} HuffTabType;

typedef struct _HuffTabLookup {
	int	linBits;
	int /*HuffTabType*/ tabType;
} HuffTabLookup;

typedef struct _IMDCTInfo {
	int outBuf[MAX_NCHAN][BLOCK_SIZE][NBANDS];	/* output of IMDCT */	
	int overBuf[MAX_NCHAN][MAX_NSAMP / 2];		/* overlap-add buffer (by symmetry, only need 1/2 size) */
	int numPrevIMDCT[MAX_NCHAN];				/* how many IMDCT's calculated in this channel on prev. granule */
	int prevType[MAX_NCHAN];
	int prevWinSwitch[MAX_NCHAN];
	int gb[MAX_NCHAN];
} IMDCTInfo;

typedef struct _BlockCount {
	int nBlocksLong;
	int nBlocksTotal;
	int nBlocksPrev; 
	int prevType;
	int prevWinSwitch;
	int currWinSwitch;
	int gbIn;
	int gbOut;
} BlockCount;

/* max bits in scalefactors = 5, so use char's to save space */
typedef struct _ScaleFactorInfoSub {
	char l[23];            /* [band] */
	char s[13][3];         /* [band][window] */
} ScaleFactorInfoSub;  

/* used in MPEG 2, 2.5 intensity (joint) stereo only */
typedef struct _ScaleFactorJS {
	int intensityScale;		
	int	slen[4];
	int	nr[4];
} ScaleFactorJS;

typedef struct _ScaleFactorInfo {
	ScaleFactorInfoSub sfis[MAX_NGRAN][MAX_NCHAN];
	ScaleFactorJS sfjs;
} ScaleFactorInfo;

/* NOTE - could get by with smaller vbuf if memory is more important than speed
 *  (in Subband, instead of replicating each block in FDCT32 you would do a memmove on the
 *   last 15 blocks to shift them down one, a hardware style FIFO)
 */ 
typedef struct _SubbandInfo {
	int vbuf[MAX_NCHAN * VBUF_LENGTH];		/* vbuf for fast DCT-based synthesis PQMF - double size for speed (no modulo indexing) */
	int vindex;								/* internal index for tracking position in vbuf */
} SubbandInfo;

/* bitstream.c */
void SetBitstreamPointer(BitStreamInfo *bsi, int nBytes, unsigned char *buf);
unsigned int GetBits(BitStreamInfo *bsi, int nBits);
int CalcBitsUsed(BitStreamInfo *bsi, unsigned char *startBuf, int startOffset);

/* dequant.c, dqchan.c, stproc.c */
int DequantChannel(int *sampleBuf, int *workBuf, int *nonZeroBound, FrameHeader *fh, SideInfoSub *sis, 
					ScaleFactorInfoSub *sfis, CriticalBandInfo *cbi);
void MidSideProc(int x[MAX_NCHAN][MAX_NSAMP], int nSamps, int mOut[2]);
void IntensityProcMPEG1(int x[MAX_NCHAN][MAX_NSAMP], int nSamps, FrameHeader *fh, ScaleFactorInfoSub *sfis, 
						CriticalBandInfo *cbi, int midSideFlag, int mixFlag, int mOut[2]);
void IntensityProcMPEG2(int x[MAX_NCHAN][MAX_NSAMP], int nSamps, FrameHeader *fh, ScaleFactorInfoSub *sfis, 
						CriticalBandInfo *cbi, ScaleFactorJS *sfjs, int midSideFlag, int mixFlag, int mOut[2]);

/* dct32.c */
// about 1 ms faster in RAM, but very large
void FDCT32(int *x, int *d, int offset, int oddBlock, int gb);// __attribute__ ((section (".data")));

/* hufftabs.c */
extern const HuffTabLookup huffTabLookup[HUFF_PAIRTABS];
extern const int huffTabOffset[HUFF_PAIRTABS];
extern const unsigned short huffTable[];
extern const unsigned char quadTable[64+16];
extern const int quadTabOffset[2];
extern const int quadTabMaxBits[2];

/* polyphase.c (or asmpoly.s)
 * some platforms require a C++ compile of all source files,
 * so if we're compiling C as C++ and using native assembly
 * for these functions we need to prevent C++ name mangling.
 */
#ifdef __cplusplus
extern "C" {
#endif
void PolyphaseMono(short *pcm, int *vbuf, const int *coefBase);
void PolyphaseStereo(short *pcm, int *vbuf, const int *coefBase);
#ifdef __cplusplus
}
#endif

/* trigtabs.c */
extern const int imdctWin[4][36];
extern const int ISFMpeg1[2][7];
extern const int ISFMpeg2[2][2][16];
extern const int ISFIIP[2][2];
extern const int csa[8][2];
extern const int coef32[31];
extern const int polyCoef[264];

#endif	/* _CODER_H */
