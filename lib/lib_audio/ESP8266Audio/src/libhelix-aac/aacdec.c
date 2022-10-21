/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: aacdec.c,v 1.1 2005/02/26 01:47:31 jrecker Exp $ 
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
 * aacdec.c - platform-independent top level decoder API
 **************************************************************************************/

#include "aaccommon.h"

//#include "profile.h"

#define PROFILE_START(x)
#define PROFILE_END()

/**************************************************************************************
 * Function:    AACInitDecoder
 *
 * Description: allocate memory for platform-specific data
 *              clear all the user-accessible fields
 *              initialize SBR decoder if enabled
 *
 * Inputs:      none
 *
 * Outputs:     none
 *
 * Return:      handle to AAC decoder instance, 0 if malloc fails
 **************************************************************************************/
HAACDecoder AACInitDecoder(void)
{
	AACDecInfo *aacDecInfo;

	aacDecInfo = AllocateBuffers();
	if (!aacDecInfo)
		return 0;

#ifdef AAC_ENABLE_SBR
	if (InitSBR(aacDecInfo)) {
		AACFreeDecoder(aacDecInfo);
		return 0;
	}
#endif

	return (HAACDecoder)aacDecInfo;
}

HAACDecoder AACInitDecoderPre(void *ptr, int sz)
{
        AACDecInfo *aacDecInfo;

        aacDecInfo = AllocateBuffersPre(&ptr, &sz);
        if (!aacDecInfo)
                return 0;

#ifdef AAC_ENABLE_SBR
        if (InitSBRPre(aacDecInfo, &ptr, &sz)) {
                return 0;
        }
#endif

        return (HAACDecoder)aacDecInfo;
}

/**************************************************************************************
 * Function:    AACFreeDecoder
 *
 * Description: free platform-specific data allocated by AACInitDecoder
 *              free SBR decoder if enabled
 *
 * Inputs:      valid AAC decoder instance pointer (HAACDecoder)
 *
 * Outputs:     none
 *
 * Return:      none
 **************************************************************************************/
void AACFreeDecoder(HAACDecoder hAACDecoder)
{
	AACDecInfo *aacDecInfo = (AACDecInfo *)hAACDecoder;

	if (!aacDecInfo)
		return;

#ifdef AAC_ENABLE_SBR
	FreeSBR(aacDecInfo);
#endif
	FreeBuffers(aacDecInfo);
}

/**************************************************************************************
 * Function:    AACFindSyncWord
 *
 * Description: locate the next byte-alinged sync word in the raw AAC stream
 *
 * Inputs:      buffer to search for sync word
 *              max number of bytes to search in buffer
 *
 * Outputs:     none
 *
 * Return:      offset to first sync word (bytes from start of buf)
 *              -1 if sync not found after searching nBytes
 **************************************************************************************/
int AACFindSyncWord(unsigned char *buf, int nBytes)
{
	int i;

	/* find byte-aligned syncword (12 bits = 0xFFF) */
	for (i = 0; i < nBytes - 1; i++) {
		if ( (buf[i+0] & SYNCWORDH) == SYNCWORDH && (buf[i+1] & SYNCWORDL) == SYNCWORDL )
			return i;
	}
	
	return -1;
}

/**************************************************************************************
 * Function:    AACGetLastFrameInfo
 *
 * Description: get info about last AAC frame decoded (number of samples decoded, 
 *                sample rate, bit rate, etc.)
 *
 * Inputs:      valid AAC decoder instance pointer (HAACDecoder)
 *              pointer to AACFrameInfo struct
 *
 * Outputs:     filled-in AACFrameInfo struct
 *
 * Return:      none
 *
 * Notes:       call this right after calling AACDecode()
 **************************************************************************************/
void AACGetLastFrameInfo(HAACDecoder hAACDecoder, AACFrameInfo *aacFrameInfo)
{
	AACDecInfo *aacDecInfo = (AACDecInfo *)hAACDecoder;

	if (!aacDecInfo) {
		aacFrameInfo->bitRate =       0;
		aacFrameInfo->nChans =        0;
		aacFrameInfo->sampRateCore =  0;
		aacFrameInfo->sampRateOut =   0;
		aacFrameInfo->bitsPerSample = 0;
		aacFrameInfo->outputSamps =   0;
		aacFrameInfo->profile =       0;
		aacFrameInfo->tnsUsed =       0;
		aacFrameInfo->pnsUsed =       0;
	} else {
		aacFrameInfo->bitRate =       aacDecInfo->bitRate;
		aacFrameInfo->nChans =        aacDecInfo->nChans;
		aacFrameInfo->sampRateCore =  aacDecInfo->sampRate;
		aacFrameInfo->sampRateOut =   aacDecInfo->sampRate * (aacDecInfo->sbrEnabled ? 2 : 1);
		aacFrameInfo->bitsPerSample = 16;
		aacFrameInfo->outputSamps =   aacDecInfo->nChans * AAC_MAX_NSAMPS * (aacDecInfo->sbrEnabled ? 2 : 1);
		aacFrameInfo->profile =       aacDecInfo->profile;
		aacFrameInfo->tnsUsed =       aacDecInfo->tnsUsed;
		aacFrameInfo->pnsUsed =       aacDecInfo->pnsUsed;
	}
}

/**************************************************************************************
 * Function:    AACSetRawBlockParams
 *
 * Description: set internal state variables for decoding a stream of raw data blocks
 *
 * Inputs:      valid AAC decoder instance pointer (HAACDecoder)
 *              flag indicating source of parameters
 *              AACFrameInfo struct, with the members nChans, sampRate, and profile
 *                optionally filled-in
 *
 * Outputs:     updated codec state 
 *
 * Return:      0 if successful, error code (< 0) if error
 *
 * Notes:       if copyLast == 1, then the codec sets up its internal state (for 
 *                decoding raw blocks) based on previously-decoded ADTS header info
 *              if copyLast == 0, then the codec uses the values passed in
 *                aacFrameInfo to configure its internal state (useful when the
 *                source is MP4 format, for example)
 **************************************************************************************/
int AACSetRawBlockParams(HAACDecoder hAACDecoder, int copyLast, AACFrameInfo *aacFrameInfo)
{
	AACDecInfo *aacDecInfo = (AACDecInfo *)hAACDecoder;

	if (!aacDecInfo)
		return ERR_AAC_NULL_POINTER;

	aacDecInfo->format = AAC_FF_RAW;
	if (copyLast)
		return SetRawBlockParams(aacDecInfo, 1, 0, 0, 0);
	else
		return SetRawBlockParams(aacDecInfo, 0, aacFrameInfo->nChans, aacFrameInfo->sampRateCore, aacFrameInfo->profile);
}

/**************************************************************************************
 * Function:    AACFlushCodec
 *
 * Description: flush internal codec state (after seeking, for example)
 *
 * Inputs:      valid AAC decoder instance pointer (HAACDecoder)
 *
 * Outputs:     updated state variables in aacDecInfo
 *
 * Return:      0 if successful, error code (< 0) if error
 **************************************************************************************/
int AACFlushCodec(HAACDecoder hAACDecoder)
{
	int ch;
	AACDecInfo *aacDecInfo = (AACDecInfo *)hAACDecoder;

	if (!aacDecInfo)
		return ERR_AAC_NULL_POINTER;

	/* reset common state variables which change per-frame
	 * don't touch state variables which are (usually) constant for entire clip 
	 *   (nChans, sampRate, profile, format, sbrEnabled)
	 */
	aacDecInfo->prevBlockID = AAC_ID_INVALID;
	aacDecInfo->currBlockID = AAC_ID_INVALID;
	aacDecInfo->currInstTag = -1;
	for (ch = 0; ch < MAX_NCHANS_ELEM; ch++)
		aacDecInfo->sbDeinterleaveReqd[ch] = 0;
	aacDecInfo->adtsBlocksLeft = 0;
	aacDecInfo->tnsUsed = 0;
	aacDecInfo->pnsUsed = 0;

	/* reset internal codec state (flush overlap buffers, etc.) */
	FlushCodec(aacDecInfo);
#ifdef AAC_ENABLE_SBR
	FlushCodecSBR(aacDecInfo);
#endif

	return ERR_AAC_NONE;
}

/**************************************************************************************
 * Function:    AACDecode
 *
 * Description: decode AAC frame
 *
 * Inputs:      valid AAC decoder instance pointer (HAACDecoder)
 *              double pointer to buffer of AAC data
 *              pointer to number of valid bytes remaining in inbuf
 *              pointer to outbuf, big enough to hold one frame of decoded PCM samples
 *                (outbuf must be double-sized if SBR enabled)
 *
 * Outputs:     PCM data in outbuf, interleaved LRLRLR... if stereo
 *                number of output samples = 1024 per channel (2048 if SBR enabled)
 *              updated inbuf pointer
 *              updated bytesLeft
 *
 * Return:      0 if successful, error code (< 0) if error
 *
 * Notes:       inbuf pointer and bytesLeft are not updated until whole frame is
 *                successfully decoded, so if ERR_AAC_INDATA_UNDERFLOW is returned
 *                just call AACDecode again with more data in inbuf
 **************************************************************************************/
int AACDecode(HAACDecoder hAACDecoder, unsigned char **inbuf, int *bytesLeft, short *outbuf)
{
	int err, offset, bitOffset, bitsAvail;
	int ch, baseChan, elementChans;
	unsigned char *inptr;
	AACDecInfo *aacDecInfo = (AACDecInfo *)hAACDecoder;
#ifdef AAC_ENABLE_SBR
	int baseChanSBR, elementChansSBR;
#endif

	if (!aacDecInfo)
		return ERR_AAC_NULL_POINTER;

	/* make local copies (see "Notes" above) */
	inptr = *inbuf;
	bitOffset = 0;
	bitsAvail = (*bytesLeft) << 3;

	/* first time through figure out what the file format is */
	if (aacDecInfo->format == AAC_FF_Unknown) {
		if (bitsAvail < 32)
			return ERR_AAC_INDATA_UNDERFLOW;
		
		if (IS_ADIF(inptr)) {
			/* unpack ADIF header */
			aacDecInfo->format = AAC_FF_ADIF;
			err = UnpackADIFHeader(aacDecInfo, &inptr, &bitOffset, &bitsAvail);
			if (err)
				return err;
		} else {
			/* assume ADTS by default */
			aacDecInfo->format = AAC_FF_ADTS;
		}
	} 
	

	
	/* if ADTS, search for start of next frame */
	if (aacDecInfo->format == AAC_FF_ADTS) {
		/* can have 1-4 raw data blocks per ADTS frame (header only present for first one) */
		if (aacDecInfo->adtsBlocksLeft == 0) {
			offset = AACFindSyncWord(inptr, bitsAvail >> 3);
			if (offset < 0)
				return ERR_AAC_INDATA_UNDERFLOW;
			inptr += offset;
			bitsAvail -= (offset << 3);

			err = UnpackADTSHeader(aacDecInfo, &inptr, &bitOffset, &bitsAvail);
			if (err)
				return err;

			if (aacDecInfo->nChans == -1) {
				/* figure out implicit channel mapping if necessary */
				err = GetADTSChannelMapping(aacDecInfo, inptr, bitOffset, bitsAvail);
				if (err)
					return err;
			}
		}
		aacDecInfo->adtsBlocksLeft--;
	} else if (aacDecInfo->format == AAC_FF_RAW) {
		err = PrepareRawBlock(aacDecInfo);
		if (err)
			return err;
	}



	/* check for valid number of channels */
	if (aacDecInfo->nChans > AAC_MAX_NCHANS || aacDecInfo->nChans <= 0)
		return ERR_AAC_NCHANS_TOO_HIGH;

	/* will be set later if active in this frame */
	aacDecInfo->tnsUsed = 0;
	aacDecInfo->pnsUsed = 0;

	bitOffset = 0;
	baseChan = 0;
#ifdef AAC_ENABLE_SBR	
	baseChanSBR = 0;
#endif	
	do {
	

	
		/* parse next syntactic element */
		err = DecodeNextElement(aacDecInfo, &inptr, &bitOffset, &bitsAvail);
		if (err)
			return err;
		
		elementChans = elementNumChans[aacDecInfo->currBlockID];
		if (baseChan + elementChans > AAC_MAX_NCHANS)
			return ERR_AAC_NCHANS_TOO_HIGH;

		/* noiseless decoder and dequantizer */
		for (ch = 0; ch < elementChans; ch++) {
      PROFILE_START("noiseless decoder");
			err = DecodeNoiselessData(aacDecInfo, &inptr, &bitOffset, &bitsAvail, ch);
      PROFILE_END();
      			
			if (err)
				return err;

			PROFILE_START("dequant");
			if (Dequantize(aacDecInfo, ch))
				return ERR_AAC_DEQUANT;
      PROFILE_END();
		}

    PROFILE_START("mid-side and intensity stereo");
		/* mid-side and intensity stereo */
		if (aacDecInfo->currBlockID == AAC_ID_CPE) {
			if (StereoProcess(aacDecInfo))
				return ERR_AAC_STEREO_PROCESS;
		}
    PROFILE_END();


		/* PNS, TNS, inverse transform */
		for (ch = 0; ch < elementChans; ch++) {
      PROFILE_START("PNS");
			if (PNS(aacDecInfo, ch))
				return ERR_AAC_PNS;
      PROFILE_END();

			if (aacDecInfo->sbDeinterleaveReqd[ch]) {
				/* deinterleave short blocks, if required */
				if (DeinterleaveShortBlocks(aacDecInfo, ch))
					return ERR_AAC_SHORT_BLOCK_DEINT;
				aacDecInfo->sbDeinterleaveReqd[ch] = 0;
			}

      PROFILE_START("TNS");
			if (TNSFilter(aacDecInfo, ch))
				return ERR_AAC_TNS;
      PROFILE_END();
	
      PROFILE_START("IMDCT");
			if (IMDCT(aacDecInfo, ch, baseChan + ch, outbuf))
				return ERR_AAC_IMDCT;
      PROFILE_END();
		}

#ifdef AAC_ENABLE_SBR
		if (aacDecInfo->sbrEnabled && (aacDecInfo->currBlockID == AAC_ID_FIL || aacDecInfo->currBlockID == AAC_ID_LFE)) {
			if (aacDecInfo->currBlockID == AAC_ID_LFE)
				elementChansSBR = elementNumChans[AAC_ID_LFE];
			else if (aacDecInfo->currBlockID == AAC_ID_FIL && (aacDecInfo->prevBlockID == AAC_ID_SCE || aacDecInfo->prevBlockID == AAC_ID_CPE))
				elementChansSBR = elementNumChans[aacDecInfo->prevBlockID];
			else 
				elementChansSBR = 0;
			
			if (baseChanSBR + elementChansSBR > AAC_MAX_NCHANS)
				return ERR_AAC_SBR_NCHANS_TOO_HIGH;

			/* parse SBR extension data if present (contained in a fill element) */
			if (DecodeSBRBitstream(aacDecInfo, baseChanSBR))
				return ERR_AAC_SBR_BITSTREAM;

			/* apply SBR */
			if (DecodeSBRData(aacDecInfo, baseChanSBR, outbuf))
				return ERR_AAC_SBR_DATA;

			baseChanSBR += elementChansSBR;
		}
#endif
		
		baseChan += elementChans;
	} while (aacDecInfo->currBlockID != AAC_ID_END);

	/* byte align after each raw_data_block */
	if (bitOffset) {
		inptr++;
		bitsAvail -= (8-bitOffset);
		bitOffset = 0;
		if (bitsAvail < 0)
			return ERR_AAC_INDATA_UNDERFLOW;
	}

	/* update pointers */
	aacDecInfo->frameCount++;
	*bytesLeft -= (inptr - *inbuf);
	*inbuf = inptr;

	return ERR_AAC_NONE;
}

