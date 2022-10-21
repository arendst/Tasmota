/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: coder.h,v 1.2 2005/06/27 21:06:00 gwright Exp $ 
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
 * coder.h - definitions of platform-specific data structures, functions, and tables
 **************************************************************************************/

#ifndef _CODER_H
#define _CODER_H

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
#define MAX(a,b)        ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)        ((a) < (b) ? (a) : (b))
#endif

#define NWINDOWS_LONG           1
#define NWINDOWS_SHORT          8

#define DATA_BUF_SIZE           510             /* max count = 255 + 255 */
#define FILL_BUF_SIZE           269             /* max count = 15 + 255 - 1*/
#define ADIF_COPYID_SIZE        9
#define MAX_COMMENT_BYTES       255

#define MAX_NUM_FCE                     15
#define MAX_NUM_SCE                     15
#define MAX_NUM_BCE                     15
#define MAX_NUM_LCE                      3
#define MAX_NUM_ADE                      7
#define MAX_NUM_CCE                     15

#define CHAN_ELEM_IS_CPE(x)             (((x) & 0x10) >> 4)  /* bit 4 = SCE/CPE flag */
#define CHAN_ELEM_GET_TAG(x)    (((x) & 0x0f) >> 0)  /* bits 3-0 = instance tag */

#define CHAN_ELEM_SET_CPE(x)    (((x) & 0x01) << 4)  /* bit 4 = SCE/CPE flag */
#define CHAN_ELEM_SET_TAG(x)    (((x) & 0x0f) << 0)  /* bits 3-0 = instance tag */

#define MAX_HUFF_BITS                   20
#define HUFFTAB_SPEC_OFFSET             1

/* do y <<= n, clipping to range [-2^30, 2^30 - 1] (i.e. output has one guard bit) */
/*
#define CLIP_2N_SHIFT(y, n) {                   \
        int sign = (y) >> 31;                   \
        if (sign != (y) >> (30 - (n)))  {       \
            (y) = sign ^ (0x3fffffff);          \
        } else {                                \
            (y) = (y) << (n);                   \
        }                                       \
    }
*/
/* clip to [-2^n, 2^n-1], valid range of n = [1, 30] */
/*
#define CLIP_2N(val, n) {                               \
        if ((val) >> 31 != (val) >> (n))                \
            (val) = ((val) >> 31) ^ ((1 << (n)) - 1);   \
    }
*/
	

#define SF_DQ_OFFSET            15
#define FBITS_OUT_DQ            20
#define FBITS_OUT_DQ_OFF        (FBITS_OUT_DQ - SF_DQ_OFFSET)   /* number of fraction bits out of dequant, including 2^15 bias */

#define FBITS_IN_IMDCT          FBITS_OUT_DQ_OFF        /* number of fraction bits into IMDCT */
#define GBITS_IN_DCT4           4                                       /* min guard bits in for DCT4 */

#define FBITS_LOST_DCT4         1               /* number of fraction bits lost (>> out) in DCT-IV */
#define FBITS_LOST_WND          1               /* number of fraction bits lost (>> out) in synthesis window (neg = gain frac bits) */
#define FBITS_LOST_IMDCT        (FBITS_LOST_DCT4 + FBITS_LOST_WND)
#define FBITS_OUT_IMDCT         (FBITS_IN_IMDCT - FBITS_LOST_IMDCT)

#define NUM_IMDCT_SIZES         2

/* additional external symbols to name-mangle for static linking */
#define DecodeProgramConfigElement              STATNAME(DecodeProgramConfigElement)
#define DecodeHuffmanScalar                             STATNAME(DecodeHuffmanScalar)
#define DecodeSpectrumLong                              STATNAME(DecodeSpectrumLong)
#define DecodeSpectrumShort                             STATNAME(DecodeSpectrumShort)
#define DecodeICSInfo                                   STATNAME(DecodeICSInfo)
#define DCT4                                                    STATNAME(DCT4)
#define R4FFT                                                   STATNAME(R4FFT)

#define DecWindowOverlapNoClip                  STATNAME(DecWindowOverlapNoClip)
#define DecWindowOverlapLongStartNoClip STATNAME(DecWindowOverlapLongStartNoClip)
#define DecWindowOverlapLongStopNoClip  STATNAME(DecWindowOverlapLongStopNoClip)
#define DecWindowOverlapShortNoClip             STATNAME(DecWindowOverlapShortNoClip)

#define huffTabSpecInfo                         STATNAME(huffTabSpecInfo)
#define huffTabSpec                                     STATNAME(huffTabSpec)
#define huffTabScaleFactInfo            STATNAME(huffTabScaleFactInfo)
#define huffTabScaleFact                        STATNAME(huffTabScaleFact)
#define cos4sin4tab                                     STATNAME(cos4sin4tab)
#define cos4sin4tabOffset                       STATNAME(cos4sin4tabOffset)
#define cos1sin1tab                                     STATNAME(cos1sin1tab)
#define sinWindow                                       STATNAME(sinWindow)
#define sinWindowOffset                         STATNAME(sinWindowOffset)
#define kbdWindow                                       STATNAME(kbdWindow)
#define kbdWindowOffset                         STATNAME(kbdWindowOffset)
#define bitrevtab                                       STATNAME(bitrevtab)
#define bitrevtabOffset                         STATNAME(bitrevtabOffset)
#define uniqueIDTab                                     STATNAME(uniqueIDTab)
#define twidTabEven                                     STATNAME(twidTabEven)
#define twidTabOdd                                      STATNAME(twidTabOdd)

typedef struct _HuffInfo {
    int maxBits;                                                    /* number of bits in longest codeword */
    unsigned /*char*/ int count[MAX_HUFF_BITS];         /* count[i] = number of codes with length i+1 bits */
    int offset;                                                             /* offset into symbol table */
} HuffInfo;

typedef struct _PulseInfo {
    unsigned char pulseDataPresent;
    unsigned char numPulse;
    unsigned char startSFB;
    unsigned char offset[MAX_PULSES];
    unsigned char amp[MAX_PULSES];
} PulseInfo;

typedef struct _TNSInfo {
    unsigned char tnsDataPresent;
    unsigned char numFilt[MAX_TNS_FILTERS];     /* max 1 filter each for 8 short windows, or 3 filters for 1 long window */
    unsigned char coefRes[MAX_TNS_FILTERS];
    unsigned char length[MAX_TNS_FILTERS];
    unsigned char order[MAX_TNS_FILTERS];
    unsigned char dir[MAX_TNS_FILTERS];
    signed char   coef[MAX_TNS_COEFS];              /* max 3 filters * 20 coefs for 1 long window, or 1 filter * 7 coefs for each of 8 short windows */
} TNSInfo;

typedef struct _GainControlInfo {
    unsigned char gainControlDataPresent;
    unsigned char maxBand;
    unsigned char adjNum[MAX_GAIN_BANDS][MAX_GAIN_WIN];
    unsigned char alevCode[MAX_GAIN_BANDS][MAX_GAIN_WIN][MAX_GAIN_ADJUST];
    unsigned char alocCode[MAX_GAIN_BANDS][MAX_GAIN_WIN][MAX_GAIN_ADJUST];
} GainControlInfo;

typedef struct _ICSInfo {
    unsigned char icsResBit;
    unsigned char winSequence;
    unsigned char winShape;
    unsigned char maxSFB;
    unsigned char sfGroup;
    unsigned char predictorDataPresent;
    unsigned char predictorReset;
    unsigned char predictorResetGroupNum;
    unsigned char predictionUsed[MAX_PRED_SFB];
    unsigned char numWinGroup;
    unsigned char winGroupLen[MAX_WIN_GROUPS];
} ICSInfo;

typedef struct _ADTSHeader {
    /* fixed */
    unsigned char id;                             /* MPEG bit - should be 1 */
    unsigned char layer;                          /* MPEG layer - should be 0 */
    unsigned char protectBit;                     /* 0 = CRC word follows, 1 = no CRC word */
    unsigned char profile;                        /* 0 = main, 1 = LC, 2 = SSR, 3 = reserved */
    unsigned char sampRateIdx;                    /* sample rate index range = [0, 11] */
    unsigned char privateBit;                     /* ignore */
    unsigned char channelConfig;                  /* 0 = implicit, >0 = use default table */
    unsigned char origCopy;                       /* 0 = copy, 1 = original */
    unsigned char home;                           /* ignore */

    /* variable */
    unsigned char copyBit;                        /* 1 bit of the 72-bit copyright ID (transmitted as 1 bit per frame) */
    unsigned char copyStart;                      /* 1 = this bit starts the 72-bit ID, 0 = it does not */
    int           frameLength;                    /* length of frame */
    int           bufferFull;                     /* number of 32-bit words left in enc buffer, 0x7FF = VBR */
    unsigned char numRawDataBlocks;               /* number of raw data blocks in frame */

    /* CRC */
    int           crcCheckWord;                   /* 16-bit CRC check word (present if protectBit == 0) */
} ADTSHeader;

typedef struct _ADIFHeader {
    unsigned char copyBit;                        /* 0 = no copyright ID, 1 = 72-bit copyright ID follows immediately */
    unsigned char origCopy;                       /* 0 = copy, 1 = original */
    unsigned char home;                           /* ignore */
    unsigned char bsType;                         /* bitstream type: 0 = CBR, 1 = VBR */
    int           bitRate;                        /* bitRate: CBR = bits/sec, VBR = peak bits/frame, 0 = unknown */
    unsigned char numPCE;                         /* number of program config elements (max = 16) */
    int           bufferFull;                     /* bits left in bit reservoir */
    unsigned char copyID[ADIF_COPYID_SIZE];       /* optional 72-bit copyright ID */
} ADIFHeader;

/* sizeof(ProgConfigElement) = 82 bytes (if KEEP_PCE_COMMENTS not defined) */
typedef struct _ProgConfigElement {
    unsigned char elemInstTag;                    /* element instance tag */
    unsigned char profile;                        /* 0 = main, 1 = LC, 2 = SSR, 3 = reserved */
    unsigned char sampRateIdx;                    /* sample rate index range = [0, 11] */
    unsigned char numFCE;                         /* number of front channel elements (max = 15) */
    unsigned char numSCE;                         /* number of side channel elements (max = 15) */
    unsigned char numBCE;                         /* number of back channel elements (max = 15) */
    unsigned char numLCE;                         /* number of LFE channel elements (max = 3) */
    unsigned char numADE;                         /* number of associated data elements (max = 7) */
    unsigned char numCCE;                         /* number of valid channel coupling elements (max = 15) */
    unsigned char monoMixdown;                    /* mono mixdown: bit 4 = present flag, bits 3-0 = element number */
    unsigned char stereoMixdown;                  /* stereo mixdown: bit 4 = present flag, bits 3-0 = element number */
    unsigned char matrixMixdown;                  /* matrix mixdown: bit 4 = present flag, bit 3 = unused, 
                                                     bits 2-1 = index, bit 0 = pseudo-surround enable */
    unsigned char fce[MAX_NUM_FCE];               /* front element channel pair: bit 4 = SCE/CPE flag, bits 3-0 = inst tag */
    unsigned char sce[MAX_NUM_SCE];               /* side element channel pair: bit 4 = SCE/CPE flag, bits 3-0 = inst tag */
    unsigned char bce[MAX_NUM_BCE];               /* back element channel pair: bit 4 = SCE/CPE flag, bits 3-0 = inst tag */
    unsigned char lce[MAX_NUM_LCE];               /* instance tag for LFE elements */
    unsigned char ade[MAX_NUM_ADE];               /* instance tag for ADE elements */
    unsigned char cce[MAX_NUM_BCE];               /* channel coupling elements: bit 4 = switching flag, bits 3-0 = inst tag */

#ifdef KEEP_PCE_COMMENTS
    /* make this optional - if not enabled, decoder will just skip comments */
    unsigned char commentBytes;
    unsigned char commentField[MAX_COMMENT_BYTES];
#endif

} ProgConfigElement;

/* state info struct for baseline (MPEG-4 LC) decoding */
typedef struct _PSInfoBase {
    /* header information */
    ADTSHeader            fhADTS;
    ADIFHeader            fhADIF;
    ProgConfigElement     pce[MAX_NUM_PCE_ADIF];
    int                   dataCount;
    unsigned char         dataBuf[DATA_BUF_SIZE];
    int                   fillCount;
    unsigned char         fillBuf[FILL_BUF_SIZE];

    /* state information which is the same throughout whole frame */
    int                   nChans;
    int                   useImpChanMap;
    int                   sampRateIdx;

    /* state information which can be overwritten by subsequent elements within frame */
    ICSInfo               icsInfo[MAX_NCHANS_ELEM];

    int                   commonWin;
    short                 scaleFactors[MAX_NCHANS_ELEM][MAX_SF_BANDS];
    unsigned char         sfbCodeBook[MAX_NCHANS_ELEM][MAX_SF_BANDS];

    int                   msMaskPresent;
    unsigned char         msMaskBits[MAX_MS_MASK_BYTES];

    int                   pnsUsed[MAX_NCHANS_ELEM];
    int                   pnsLastVal;
    int                   intensityUsed[MAX_NCHANS_ELEM];

    PulseInfo             pulseInfo[MAX_NCHANS_ELEM];

    TNSInfo               tnsInfo[MAX_NCHANS_ELEM];
    int                   tnsLPCBuf[MAX_TNS_ORDER];
    int                   tnsWorkBuf[MAX_TNS_ORDER];

    GainControlInfo       gainControlInfo[MAX_NCHANS_ELEM];

    int                   gbCurrent[MAX_NCHANS_ELEM];
    int                   coef[MAX_NCHANS_ELEM][AAC_MAX_NSAMPS];
#ifdef AAC_ENABLE_SBR
    int                   sbrWorkBuf[MAX_NCHANS_ELEM][AAC_MAX_NSAMPS];
#endif
    /* state information which must be saved for each element and used in next frame */
    int                   overlap[AAC_MAX_NCHANS][AAC_MAX_NSAMPS];
    int                   prevWinShape[AAC_MAX_NCHANS];

} PSInfoBase;

/* private implementation-specific functions */

/* decelmnt.c */
int DecodeProgramConfigElement(ProgConfigElement *pce, BitStreamInfo *bsi);

/* huffman.c */
int DecodeHuffmanScalar(const signed short *huffTab, const HuffInfo *huffTabInfo, unsigned int bitBuf, signed int *val);
void DecodeSpectrumLong(PSInfoBase *psi, BitStreamInfo *bsi, int ch);
void DecodeSpectrumShort(PSInfoBase *psi, BitStreamInfo *bsi, int ch);

/* noiseless.c */
void DecodeICSInfo(BitStreamInfo *bsi, ICSInfo *icsInfo, int sampRateIdx);

/* dct4.c */
void DCT4(int tabidx, int *coef, int gb);

/* fft.c */
void R4FFT(int tabidx, int *x);

/* sbrimdct.c */
void DecWindowOverlapNoClip(int *buf0, int *over0, int *out0, int winTypeCurr, int winTypePrev);
void DecWindowOverlapLongStartNoClip(int *buf0, int *over0, int *out0, int winTypeCurr, int winTypePrev);
void DecWindowOverlapLongStopNoClip(int *buf0, int *over0, int *out0, int winTypeCurr, int winTypePrev);
void DecWindowOverlapShortNoClip(int *buf0, int *over0, int *out0, int winTypeCurr, int winTypePrev);

/* hufftabs.c */
extern const HuffInfo huffTabSpecInfo[11];
extern const signed short huffTabSpec[1241];
extern const HuffInfo huffTabScaleFactInfo; 
extern const signed short huffTabScaleFact[121];

/* trigtabs.c */
extern const int cos4sin4tabOffset[NUM_IMDCT_SIZES];
extern const int sinWindowOffset[NUM_IMDCT_SIZES];
extern const int kbdWindowOffset[NUM_IMDCT_SIZES];
extern const unsigned char bitrevtab[17 + 129];
extern const int bitrevtabOffset[NUM_IMDCT_SIZES];

#ifdef HELIX_CONFIG_AAC_GENERATE_TRIGTABS_FLOAT
/* trigtabs_fltgen.c */
extern int cos4sin4tab[128 + 1024];
extern int cos1sin1tab[514];
extern int sinWindow[128 + 1024];
extern int kbdWindow[128 + 1024];
extern int twidTabEven[4*6 + 16*6 + 64*6];
extern int twidTabOdd[8*6 + 32*6 + 128*6];
#else
/* trigtabs.c */
extern const int cos4sin4tab[128 + 1024];
extern const int cos1sin1tab[514];
extern const int sinWindow[128 + 1024];
extern const int kbdWindow[128 + 1024];
extern const int twidTabEven[4*6 + 16*6 + 64*6];
extern const int twidTabOdd[8*6 + 32*6 + 128*6];
#endif

#endif  /* _CODER_H */

