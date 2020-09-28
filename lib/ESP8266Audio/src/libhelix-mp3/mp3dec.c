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
 * mp3dec.c - platform-independent top level MP3 decoder API
 **************************************************************************************/

#include "string.h"
//#include "hlxclib/string.h"		/* for memmove, memcpy (can replace with different implementations if desired) */
#include "mp3common.h"	/* includes mp3dec.h (public API) and internal, platform-independent API */


//#define PROFILE
#ifdef PROFILE
#include "systime.h"
#endif

/**************************************************************************************
 * Function:    MP3InitDecoder
 *
 * Description: allocate memory for platform-specific data
 *              clear all the user-accessible fields
 *
 * Inputs:      none
 *
 * Outputs:     none
 *
 * Return:      handle to mp3 decoder instance, 0 if malloc fails
 **************************************************************************************/
HMP3Decoder MP3InitDecoder(void)
{
	MP3DecInfo *mp3DecInfo;

	mp3DecInfo = AllocateBuffers();

	return (HMP3Decoder)mp3DecInfo;
}

/**************************************************************************************
 * Function:    MP3FreeDecoder
 *
 * Description: free platform-specific data allocated by InitMP3Decoder
 *              zero out the contents of MP3DecInfo struct
 *
 * Inputs:      valid MP3 decoder instance pointer (HMP3Decoder)
 *
 * Outputs:     none
 *
 * Return:      none
 **************************************************************************************/
void MP3FreeDecoder(HMP3Decoder hMP3Decoder)
{
	MP3DecInfo *mp3DecInfo = (MP3DecInfo *)hMP3Decoder;

	if (!mp3DecInfo)
		return;

	FreeBuffers(mp3DecInfo);
}

/**************************************************************************************
 * Function:    MP3FindSyncWord
 *
 * Description: locate the next byte-alinged sync word in the raw mp3 stream
 *
 * Inputs:      buffer to search for sync word
 *              max number of bytes to search in buffer
 *
 * Outputs:     none
 *
 * Return:      offset to first sync word (bytes from start of buf)
 *              -1 if sync not found after searching nBytes
 **************************************************************************************/
int MP3FindSyncWord(unsigned char *buf, int nBytes)
{
	int i;

	/* find byte-aligned syncword - need 12 (MPEG 1,2) or 11 (MPEG 2.5) matching bits */
	for (i = 0; i < nBytes - 1; i++) {
		if ( (buf[i+0] & SYNCWORDH) == SYNCWORDH && (buf[i+1] & SYNCWORDL) == SYNCWORDL )
			return i;
	}
	
	return -1;
}

/**************************************************************************************
 * Function:    MP3FindFreeSync
 *
 * Description: figure out number of bytes between adjacent sync words in "free" mode
 *
 * Inputs:      buffer to search for next sync word
 *              the 4-byte frame header starting at the current sync word
 *              max number of bytes to search in buffer
 *
 * Outputs:     none
 *
 * Return:      offset to next sync word, minus any pad byte (i.e. nSlots)
 *              -1 if sync not found after searching nBytes
 *
 * Notes:       this checks that the first 22 bits of the next frame header are the
 *                same as the current frame header, but it's still not foolproof
 *                (could accidentally find a sequence in the bitstream which 
 *                 appears to match but is not actually the next frame header)
 *              this could be made more error-resilient by checking several frames
 *                in a row and verifying that nSlots is the same in each case
 *              since free mode requires CBR (see spec) we generally only call
 *                this function once (first frame) then store the result (nSlots)
 *                and just use it from then on
 **************************************************************************************/
static int MP3FindFreeSync(unsigned char *buf, unsigned char firstFH[4], int nBytes)
{
	int offset = 0;
	unsigned char *bufPtr = buf;

	/* loop until we either: 
	 *  - run out of nBytes (FindMP3SyncWord() returns -1)
	 *  - find the next valid frame header (sync word, version, layer, CRC flag, bitrate, and sample rate
	 *      in next header must match current header)
	 */
	while (1) {
		offset = MP3FindSyncWord(bufPtr, nBytes);
		bufPtr += offset;
		if (offset < 0) {
			return -1;
		} else if ( (bufPtr[0] == firstFH[0]) && (bufPtr[1] == firstFH[1]) && ((bufPtr[2] & 0xfc) == (firstFH[2] & 0xfc)) ) {
			/* want to return number of bytes per frame, NOT counting the padding byte, so subtract one if padFlag == 1 */
			if ((firstFH[2] >> 1) & 0x01)
				bufPtr--;
			return bufPtr - buf;
		}
		bufPtr += 3;
		nBytes -= (offset + 3);
	};

	return -1;
}

/**************************************************************************************
 * Function:    MP3GetLastFrameInfo
 *
 * Description: get info about last MP3 frame decoded (number of sampled decoded, 
 *                sample rate, bitrate, etc.)
 *
 * Inputs:      valid MP3 decoder instance pointer (HMP3Decoder)
 *              pointer to MP3FrameInfo struct
 *
 * Outputs:     filled-in MP3FrameInfo struct
 *
 * Return:      none
 *
 * Notes:       call this right after calling MP3Decode
 **************************************************************************************/
void MP3GetLastFrameInfo(HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo)
{
	MP3DecInfo *mp3DecInfo = (MP3DecInfo *)hMP3Decoder;

	if (!mp3DecInfo || mp3DecInfo->layer != 3) {
		mp3FrameInfo->bitrate = 0;
		mp3FrameInfo->nChans = 0;
		mp3FrameInfo->samprate = 0;
		mp3FrameInfo->bitsPerSample = 0;
		mp3FrameInfo->outputSamps = 0;
		mp3FrameInfo->layer = 0;
		mp3FrameInfo->version = 0;
	} else {
		mp3FrameInfo->bitrate = mp3DecInfo->bitrate;
		mp3FrameInfo->nChans = mp3DecInfo->nChans;
		mp3FrameInfo->samprate = mp3DecInfo->samprate;
		mp3FrameInfo->bitsPerSample = 16;
		mp3FrameInfo->outputSamps = mp3DecInfo->nChans * (int)samplesPerFrameTab[mp3DecInfo->version][mp3DecInfo->layer - 1];
		mp3FrameInfo->layer = mp3DecInfo->layer;
		mp3FrameInfo->version = mp3DecInfo->version;
	}
}

/**************************************************************************************
 * Function:    MP3GetNextFrameInfo
 *
 * Description: parse MP3 frame header
 *
 * Inputs:      valid MP3 decoder instance pointer (HMP3Decoder)
 *              pointer to MP3FrameInfo struct
 *              pointer to buffer containing valid MP3 frame header (located using 
 *                MP3FindSyncWord(), above)
 *
 * Outputs:     filled-in MP3FrameInfo struct
 *
 * Return:      error code, defined in mp3dec.h (0 means no error, < 0 means error)
 **************************************************************************************/
int MP3GetNextFrameInfo(HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo, unsigned char *buf)
{
	MP3DecInfo *mp3DecInfo = (MP3DecInfo *)hMP3Decoder;

	if (!mp3DecInfo)
		return ERR_MP3_NULL_POINTER;

	if (UnpackFrameHeader(mp3DecInfo, buf) == -1 || mp3DecInfo->layer != 3)
		return ERR_MP3_INVALID_FRAMEHEADER;

	MP3GetLastFrameInfo(mp3DecInfo, mp3FrameInfo);

	return ERR_MP3_NONE;
}

/**************************************************************************************
 * Function:    MP3ClearBadFrame
 *
 * Description: zero out pcm buffer if error decoding MP3 frame
 *
 * Inputs:      mp3DecInfo struct with correct frame size parameters filled in
 *              pointer pcm output buffer
 *
 * Outputs:     zeroed out pcm buffer
 *
 * Return:      none
 **************************************************************************************/
static void MP3ClearBadFrame(MP3DecInfo *mp3DecInfo, short *outbuf)
{
	int i;

	if (!mp3DecInfo)
		return;

	for (i = 0; i < mp3DecInfo->nGrans * mp3DecInfo->nGranSamps * mp3DecInfo->nChans; i++)
		outbuf[i] = 0;
}

/**************************************************************************************
 * Function:    MP3Decode
 *
 * Description: decode one frame of MP3 data
 *
 * Inputs:      valid MP3 decoder instance pointer (HMP3Decoder)
 *              double pointer to buffer of MP3 data (containing headers + mainData)
 *              number of valid bytes remaining in inbuf
 *              pointer to outbuf, big enough to hold one frame of decoded PCM samples
 *              flag indicating whether MP3 data is normal MPEG format (useSize = 0)
 *                or reformatted as "self-contained" frames (useSize = 1)
 *
 * Outputs:     PCM data in outbuf, interleaved LRLRLR... if stereo
 *                number of output samples = nGrans * nGranSamps * nChans
 *              updated inbuf pointer, updated bytesLeft
 *
 * Return:      error code, defined in mp3dec.h (0 means no error, < 0 means error)
 *
 * Notes:       switching useSize on and off between frames in the same stream 
 *                is not supported (bit reservoir is not maintained if useSize on)
 **************************************************************************************/
int MP3Decode(HMP3Decoder hMP3Decoder, unsigned char **inbuf, int *bytesLeft, short *outbuf, int useSize)
{
	int offset, bitOffset, mainBits, gr, ch, fhBytes, siBytes, freeFrameBytes;
	int prevBitOffset, sfBlockBits, huffBlockBits;
	unsigned char *mainPtr;
	MP3DecInfo *mp3DecInfo = (MP3DecInfo *)hMP3Decoder;
	
	#ifdef PROFILE
	long time;
	#endif

	if (!mp3DecInfo)
		return ERR_MP3_NULL_POINTER;

	/* unpack frame header */
	fhBytes = UnpackFrameHeader(mp3DecInfo, *inbuf);
	if (fhBytes < 0)	
		return ERR_MP3_INVALID_FRAMEHEADER;		/* don't clear outbuf since we don't know size (failed to parse header) */
	*inbuf += fhBytes;
	
#ifdef PROFILE
	time = systime_get();
#endif
	/* unpack side info */
	siBytes = UnpackSideInfo(mp3DecInfo, *inbuf);
	if (siBytes < 0) {
		MP3ClearBadFrame(mp3DecInfo, outbuf);
		return ERR_MP3_INVALID_SIDEINFO;
	}
	*inbuf += siBytes;
	*bytesLeft -= (fhBytes + siBytes);
#ifdef PROFILE
	time = systime_get() - time;
	printf("UnpackSideInfo: %i ms\n", time);
#endif
	
	
	/* if free mode, need to calculate bitrate and nSlots manually, based on frame size */
	if (mp3DecInfo->bitrate == 0 || mp3DecInfo->freeBitrateFlag) {
		if (!mp3DecInfo->freeBitrateFlag) {
			/* first time through, need to scan for next sync word and figure out frame size */
			mp3DecInfo->freeBitrateFlag = 1;
			mp3DecInfo->freeBitrateSlots = MP3FindFreeSync(*inbuf, *inbuf - fhBytes - siBytes, *bytesLeft);
			if (mp3DecInfo->freeBitrateSlots < 0) {
				MP3ClearBadFrame(mp3DecInfo, outbuf);
				return ERR_MP3_FREE_BITRATE_SYNC;
			}
			freeFrameBytes = mp3DecInfo->freeBitrateSlots + fhBytes + siBytes;
			mp3DecInfo->bitrate = (freeFrameBytes * mp3DecInfo->samprate * 8) / (mp3DecInfo->nGrans * mp3DecInfo->nGranSamps);
		}
		mp3DecInfo->nSlots = mp3DecInfo->freeBitrateSlots + CheckPadBit(mp3DecInfo);	/* add pad byte, if required */
	}

	/* useSize != 0 means we're getting reformatted (RTP) packets (see RFC 3119)
	 *  - calling function assembles "self-contained" MP3 frames by shifting any main_data 
	 *      from the bit reservoir (in previous frames) to AFTER the sync word and side info
	 *  - calling function should set mainDataBegin to 0, and tell us exactly how large this
	 *      frame is (in bytesLeft)
	 */
	if (useSize) {
		mp3DecInfo->nSlots = *bytesLeft;
		if (mp3DecInfo->mainDataBegin != 0 || mp3DecInfo->nSlots <= 0) {
			/* error - non self-contained frame, or missing frame (size <= 0), could do loss concealment here */
			MP3ClearBadFrame(mp3DecInfo, outbuf);
			return ERR_MP3_INVALID_FRAMEHEADER;
		}

		/* can operate in-place on reformatted frames */
		mp3DecInfo->mainDataBytes = mp3DecInfo->nSlots;
		mainPtr = *inbuf;
		*inbuf += mp3DecInfo->nSlots;
		*bytesLeft -= (mp3DecInfo->nSlots);
	} else {
		/* out of data - assume last or truncated frame */
		if (mp3DecInfo->nSlots > *bytesLeft) {
			MP3ClearBadFrame(mp3DecInfo, outbuf);
			return ERR_MP3_INDATA_UNDERFLOW;	
		}

#ifdef PROFILE
	time = systime_get();
#endif
		/* fill main data buffer with enough new data for this frame */
		if (mp3DecInfo->mainDataBytes >= mp3DecInfo->mainDataBegin) {
			/* adequate "old" main data available (i.e. bit reservoir) */
			memmove(mp3DecInfo->mainBuf, mp3DecInfo->mainBuf + mp3DecInfo->mainDataBytes - mp3DecInfo->mainDataBegin, mp3DecInfo->mainDataBegin);
			memcpy(mp3DecInfo->mainBuf + mp3DecInfo->mainDataBegin, *inbuf, mp3DecInfo->nSlots);

			mp3DecInfo->mainDataBytes = mp3DecInfo->mainDataBegin + mp3DecInfo->nSlots;
			*inbuf += mp3DecInfo->nSlots;
			*bytesLeft -= (mp3DecInfo->nSlots);
			mainPtr = mp3DecInfo->mainBuf;
		} else {
			/* not enough data in bit reservoir from previous frames (perhaps starting in middle of file) */
			memcpy(mp3DecInfo->mainBuf + mp3DecInfo->mainDataBytes, *inbuf, mp3DecInfo->nSlots);
			mp3DecInfo->mainDataBytes += mp3DecInfo->nSlots;
			*inbuf += mp3DecInfo->nSlots;
			*bytesLeft -= (mp3DecInfo->nSlots);
			MP3ClearBadFrame(mp3DecInfo, outbuf);
			return ERR_MP3_MAINDATA_UNDERFLOW;
		}
#ifdef PROFILE
	time = systime_get() - time;
	printf("data buffer filling: %i ms\n", time);
#endif

	}
	bitOffset = 0;
	mainBits = mp3DecInfo->mainDataBytes * 8;

	/* decode one complete frame */
	for (gr = 0; gr < mp3DecInfo->nGrans; gr++) {
		for (ch = 0; ch < mp3DecInfo->nChans; ch++) {
			
			#ifdef PROFILE
				time = systime_get();
			#endif
			/* unpack scale factors and compute size of scale factor block */
			prevBitOffset = bitOffset;
			offset = UnpackScaleFactors(mp3DecInfo, mainPtr, &bitOffset, mainBits, gr, ch);
			#ifdef PROFILE
				time = systime_get() - time;
				printf("UnpackScaleFactors: %i ms\n", time);
			#endif

			sfBlockBits = 8*offset - prevBitOffset + bitOffset;
			huffBlockBits = mp3DecInfo->part23Length[gr][ch] - sfBlockBits;
			mainPtr += offset;
			mainBits -= sfBlockBits;

			if (offset < 0 || mainBits < huffBlockBits) {
				MP3ClearBadFrame(mp3DecInfo, outbuf);
				return ERR_MP3_INVALID_SCALEFACT;
			}

			#ifdef PROFILE
				time = systime_get();
			#endif
			/* decode Huffman code words */
			prevBitOffset = bitOffset;
			offset = DecodeHuffman(mp3DecInfo, mainPtr, &bitOffset, huffBlockBits, gr, ch);
			if (offset < 0) {
				MP3ClearBadFrame(mp3DecInfo, outbuf);
				return ERR_MP3_INVALID_HUFFCODES;
			}
			#ifdef PROFILE
				time = systime_get() - time;
				printf("Huffman: %i ms\n", time);
			#endif

			mainPtr += offset;
			mainBits -= (8*offset - prevBitOffset + bitOffset);
		}
		
		#ifdef PROFILE
			time = systime_get();
		#endif
		/* dequantize coefficients, decode stereo, reorder short blocks */
		if (Dequantize(mp3DecInfo, gr) < 0) {
			MP3ClearBadFrame(mp3DecInfo, outbuf);
			return ERR_MP3_INVALID_DEQUANTIZE;			
		}
		#ifdef PROFILE
			time = systime_get() - time;
			printf("Dequantize: %i ms\n", time);
		#endif

		/* alias reduction, inverse MDCT, overlap-add, frequency inversion */
		for (ch = 0; ch < mp3DecInfo->nChans; ch++)
		{
		#ifdef PROFILE
			time = systime_get();
		#endif
			if (IMDCT(mp3DecInfo, gr, ch) < 0) {
				MP3ClearBadFrame(mp3DecInfo, outbuf);
				return ERR_MP3_INVALID_IMDCT;			
			}
		#ifdef PROFILE
			time = systime_get() - time;
			printf("IMDCT: %i ms\n", time);
		#endif
		}
		
		#ifdef PROFILE
			time = systime_get();
		#endif
		/* subband transform - if stereo, interleaves pcm LRLRLR */
		if (Subband(mp3DecInfo, outbuf + gr*mp3DecInfo->nGranSamps*mp3DecInfo->nChans) < 0) {
			MP3ClearBadFrame(mp3DecInfo, outbuf);
			return ERR_MP3_INVALID_SUBBAND;			
		}
		#ifdef PROFILE
			time = systime_get() - time;
			printf("Subband: %i ms\n", time);
		#endif
		
	}
	return ERR_MP3_NONE;
}
