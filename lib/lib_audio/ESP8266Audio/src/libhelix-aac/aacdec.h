/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: aacdec.h,v 1.8 2005/11/10 00:15:08 margotm Exp $ 
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
 * aacdec.h - public C API for AAC decoder
 **************************************************************************************/

#ifndef _AACDEC_H
#define _AACDEC_H

#if defined(_WIN32) && !defined(_WIN32_WCE)
#
#elif defined(_WIN32) && defined(_WIN32_WCE) && defined(ARM)
#
#elif defined(_WIN32) && defined(WINCE_EMULATOR)
#
#elif defined (__arm) && defined (__ARMCC_VERSION)
#
#elif defined(_SYMBIAN) && defined(__WINS__)
#
#elif defined(__GNUC__) && defined(__arm__)
#
#elif defined(__GNUC__) && defined(__i386__)
#
#elif defined(__GNUC__) && defined(__amd64__)
#
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__POWERPC__))
#
#elif defined(_OPENWAVE_SIMULATOR) || defined(_OPENWAVE_ARMULATOR)
#
#elif defined(_SOLARIS) && !defined(__GNUC__)
#
#elif defined(ARDUINO)
#
#else
#error No platform defined. See valid options in aacdec.h
#endif

#ifndef USE_DEFAULT_STDLIB
#define USE_DEFAULT_STDLIB
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* according to spec (13818-7 section 8.2.2, 14496-3 section 4.5.3)
 * max size of input buffer = 
 *    6144 bits =  768 bytes per SCE or CCE-I
 *   12288 bits = 1536 bytes per CPE
 *       0 bits =    0 bytes per CCE-D (uses bits from the SCE/CPE/CCE-I it is coupled to)
 */
#ifndef AAC_MAX_NCHANS				/* if max channels isn't set in makefile, */
#define AAC_MAX_NCHANS		2		/* set to default max number of channels  */
#endif
#define AAC_MAX_NSAMPS		1024
#define AAC_MAINBUF_SIZE	(768 * AAC_MAX_NCHANS)

#define AAC_NUM_PROFILES	3
#define AAC_PROFILE_MP		0
#define AAC_PROFILE_LC		1
#define AAC_PROFILE_SSR		2

/* define these to enable decoder features */
#if defined(HELIX_FEATURE_AUDIO_CODEC_AAC_SBR)
#define AAC_ENABLE_SBR
#endif //  HELIX_FEATURE_AUDIO_CODEC_AAC_SBR.
#define AAC_ENABLE_MPEG4

enum {
	ERR_AAC_NONE                          =   0,
	ERR_AAC_INDATA_UNDERFLOW              =  -1,
	ERR_AAC_NULL_POINTER                  =  -2,
	ERR_AAC_INVALID_ADTS_HEADER           =  -3,
	ERR_AAC_INVALID_ADIF_HEADER           =  -4,
	ERR_AAC_INVALID_FRAME                 =  -5,
	ERR_AAC_MPEG4_UNSUPPORTED             =  -6,
	ERR_AAC_CHANNEL_MAP                   =  -7,
	ERR_AAC_SYNTAX_ELEMENT                =  -8,

	ERR_AAC_DEQUANT                       =  -9,
	ERR_AAC_STEREO_PROCESS                = -10,
	ERR_AAC_PNS                           = -11,
	ERR_AAC_SHORT_BLOCK_DEINT             = -12,
	ERR_AAC_TNS                           = -13,
	ERR_AAC_IMDCT                         = -14,
	ERR_AAC_NCHANS_TOO_HIGH               = -15,

	ERR_AAC_SBR_INIT                      = -16,
	ERR_AAC_SBR_BITSTREAM                 = -17,
	ERR_AAC_SBR_DATA                      = -18,
	ERR_AAC_SBR_PCM_FORMAT                = -19,
	ERR_AAC_SBR_NCHANS_TOO_HIGH           = -20,
	ERR_AAC_SBR_SINGLERATE_UNSUPPORTED    = -21,

	ERR_AAC_RAWBLOCK_PARAMS               = -22,

	ERR_AAC_UNKNOWN						= -9999
};

typedef struct _AACFrameInfo {
	int bitRate;
	int nChans;
	int sampRateCore;
	int sampRateOut;
	int bitsPerSample;
	int outputSamps;
	int profile;
	int tnsUsed;
	int pnsUsed;
} AACFrameInfo;

typedef void *HAACDecoder;

/* public C API */
HAACDecoder AACInitDecoder(void);
HAACDecoder AACInitDecoderPre(void *ptr, int sz);
void AACFreeDecoder(HAACDecoder hAACDecoder);
int AACDecode(HAACDecoder hAACDecoder, unsigned char **inbuf, int *bytesLeft, short *outbuf);

int AACFindSyncWord(unsigned char *buf, int nBytes);
void AACGetLastFrameInfo(HAACDecoder hAACDecoder, AACFrameInfo *aacFrameInfo);
int AACSetRawBlockParams(HAACDecoder hAACDecoder, int copyLast, AACFrameInfo *aacFrameInfo);
int AACFlushCodec(HAACDecoder hAACDecoder);

#ifdef HELIX_CONFIG_AAC_GENERATE_TRIGTABS_FLOAT
int AACInitTrigtabsFloat(void);
void AACFreeTrigtabsFloat(void);
#endif

#ifdef __cplusplus
}
#endif

#endif	/* _AACDEC_H */
