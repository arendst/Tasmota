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
 * bitstream.c - bitstream unpacking, frame header parsing, side info parsing
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"

/**************************************************************************************
 * Function:    SetBitstreamPointer
 *
 * Description: initialize bitstream reader
 *
 * Inputs:      pointer to BitStreamInfo struct
 *              number of bytes in bitstream
 *              pointer to byte-aligned buffer of data to read from
 *
 * Outputs:     filled bitstream info struct
 *
 * Return:      none
 **************************************************************************************/
void SetBitstreamPointer(BitStreamInfo *bsi, int nBytes, unsigned char *buf)
{
	/* init bitstream */
	bsi->bytePtr = buf;
	bsi->iCache = 0;		/* 4-byte unsigned int */
	bsi->cachedBits = 0;	/* i.e. zero bits in cache */
	bsi->nBytes = nBytes;
}

/**************************************************************************************
 * Function:    RefillBitstreamCache
 *
 * Description: read new data from bitstream buffer into bsi cache
 *
 * Inputs:      pointer to initialized BitStreamInfo struct
 *
 * Outputs:     updated bitstream info struct
 *
 * Return:      none
 *
 * Notes:       only call when iCache is completely drained (resets bitOffset to 0)
 *              always loads 4 new bytes except when bsi->nBytes < 4 (end of buffer)
 *              stores data as big-endian in cache, regardless of machine endian-ness
 *
 * TODO:        optimize for ARM
 *              possibly add little/big-endian modes for doing 32-bit loads
 **************************************************************************************/
static __inline void RefillBitstreamCache(BitStreamInfo *bsi)
{
	int nBytes = bsi->nBytes;

	/* optimize for common case, independent of machine endian-ness */
	if (nBytes >= 4) {
		bsi->iCache  = (*bsi->bytePtr++) << 24;
		bsi->iCache |= (*bsi->bytePtr++) << 16;
		bsi->iCache |= (*bsi->bytePtr++) <<  8;
		bsi->iCache |= (*bsi->bytePtr++);
		bsi->cachedBits = 32;
		bsi->nBytes -= 4;
	} else {
		bsi->iCache = 0;
		while (nBytes--) {
			bsi->iCache |= (*bsi->bytePtr++);
			bsi->iCache <<= 8;
		}
		bsi->iCache <<= ((3 - bsi->nBytes)*8);
		bsi->cachedBits = 8*bsi->nBytes;
		bsi->nBytes = 0;
	}
}

/**************************************************************************************
 * Function:    GetBits
 *
 * Description: get bits from bitstream, advance bitstream pointer
 *
 * Inputs:      pointer to initialized BitStreamInfo struct
 *              number of bits to get from bitstream
 *
 * Outputs:     updated bitstream info struct
 *
 * Return:      the next nBits bits of data from bitstream buffer
 *
 * Notes:       nBits must be in range [0, 31], nBits outside this range masked by 0x1f
 *              for speed, does not indicate error if you overrun bit buffer 
 *              if nBits = 0, returns 0 (useful for scalefactor unpacking)
 *
 * TODO:        optimize for ARM
 **************************************************************************************/
unsigned int GetBits(BitStreamInfo *bsi, int nBits)
{
	unsigned int data, lowBits;

	nBits &= 0x1f;							/* nBits mod 32 to avoid unpredictable results like >> by negative amount */
	data = bsi->iCache >> (31 - nBits);		/* unsigned >> so zero-extend */
	data >>= 1;								/* do as >> 31, >> 1 so that nBits = 0 works okay (returns 0) */
	bsi->iCache <<= nBits;					/* left-justify cache */
	bsi->cachedBits -= nBits;				/* how many bits have we drawn from the cache so far */

	/* if we cross an int boundary, refill the cache */
	if (bsi->cachedBits < 0) {
		lowBits = -bsi->cachedBits;
		RefillBitstreamCache(bsi);
		data |= bsi->iCache >> (32 - lowBits);		/* get the low-order bits */
	
		bsi->cachedBits -= lowBits;			/* how many bits have we drawn from the cache so far */
		bsi->iCache <<= lowBits;			/* left-justify cache */
	}

	return data;
}

/**************************************************************************************
 * Function:    CalcBitsUsed
 *
 * Description: calculate how many bits have been read from bitstream
 *
 * Inputs:      pointer to initialized BitStreamInfo struct
 *              pointer to start of bitstream buffer
 *              bit offset into first byte of startBuf (0-7) 
 *
 * Outputs:     none
 *
 * Return:      number of bits read from bitstream, as offset from startBuf:startOffset
 **************************************************************************************/
int CalcBitsUsed(BitStreamInfo *bsi, unsigned char *startBuf, int startOffset)
{
	int bitsUsed;

	bitsUsed  = (bsi->bytePtr - startBuf) * 8;
	bitsUsed -= bsi->cachedBits;
	bitsUsed -= startOffset;

	return bitsUsed;
}

/**************************************************************************************
 * Function:    CheckPadBit
 *
 * Description: check whether padding byte is present in an MP3 frame
 *
 * Inputs:      MP3DecInfo struct with valid FrameHeader struct 
 *                (filled by UnpackFrameHeader())
 *
 * Outputs:     none
 *
 * Return:      1 if pad bit is set, 0 if not, -1 if null input pointer
 **************************************************************************************/
int CheckPadBit(MP3DecInfo *mp3DecInfo)
{
	FrameHeader *fh;

	/* validate pointers */
	if (!mp3DecInfo || !mp3DecInfo->FrameHeaderPS)
		return -1;

	fh = ((FrameHeader *)(mp3DecInfo->FrameHeaderPS));

	return (fh->paddingBit ? 1 : 0);
}

/**************************************************************************************
 * Function:    UnpackFrameHeader
 *
 * Description: parse the fields of the MP3 frame header
 *
 * Inputs:      buffer pointing to a complete MP3 frame header (4 bytes, plus 2 if CRC)
 *
 * Outputs:     filled frame header info in the MP3DecInfo structure
 *              updated platform-specific FrameHeader struct
 *
 * Return:      length (in bytes) of frame header (for caller to calculate offset to
 *                first byte following frame header)
 *              -1 if null frameHeader or invalid header
 *
 * TODO:        check for valid modes, depending on capabilities of decoder
 *              test CRC on actual stream (verify no endian problems)
 **************************************************************************************/
int UnpackFrameHeader(MP3DecInfo *mp3DecInfo, unsigned char *buf)
{

	int verIdx;
	FrameHeader *fh;

	/* validate pointers and sync word */
	if (!mp3DecInfo || !mp3DecInfo->FrameHeaderPS || (buf[0] & SYNCWORDH) != SYNCWORDH || (buf[1] & SYNCWORDL) != SYNCWORDL)
		return -1;

	fh = ((FrameHeader *)(mp3DecInfo->FrameHeaderPS));

	/* read header fields - use bitmasks instead of GetBits() for speed, since format never varies */
	verIdx =         (buf[1] >> 3) & 0x03;
	fh->ver =        (MPEGVersion)( verIdx == 0 ? MPEG25 : ((verIdx & 0x01) ? MPEG1 : MPEG2) );
	fh->layer = 4 - ((buf[1] >> 1) & 0x03);     /* easy mapping of index to layer number, 4 = error */
	fh->crc =   1 - ((buf[1] >> 0) & 0x01);
	fh->brIdx =      (buf[2] >> 4) & 0x0f;
	fh->srIdx =      (buf[2] >> 2) & 0x03;
	fh->paddingBit = (buf[2] >> 1) & 0x01;
	fh->privateBit = (buf[2] >> 0) & 0x01;
	fh->sMode =      (StereoMode)((buf[3] >> 6) & 0x03);      /* maps to correct enum (see definition) */    
	fh->modeExt =    (buf[3] >> 4) & 0x03;
	fh->copyFlag =   (buf[3] >> 3) & 0x01;
	fh->origFlag =   (buf[3] >> 2) & 0x01;
	fh->emphasis =   (buf[3] >> 0) & 0x03;

	/* check parameters to avoid indexing tables with bad values */
	if (fh->srIdx == 3 || fh->layer == 4 || fh->brIdx == 15)
		return -1;

	fh->sfBand = &sfBandTable[fh->ver][fh->srIdx];	/* for readability (we reference sfBandTable many times in decoder) */
	if (fh->sMode != Joint)		/* just to be safe (dequant, stproc check fh->modeExt) */
		fh->modeExt = 0;

	/* init user-accessible data */
	mp3DecInfo->nChans = (fh->sMode == Mono ? 1 : 2);
	mp3DecInfo->samprate = samplerateTab[fh->ver][fh->srIdx];
	mp3DecInfo->nGrans = (fh->ver == MPEG1 ? NGRANS_MPEG1 : NGRANS_MPEG2);
	mp3DecInfo->nGranSamps = ((int)samplesPerFrameTab[fh->ver][fh->layer - 1]) / mp3DecInfo->nGrans;
	mp3DecInfo->layer = fh->layer;
	mp3DecInfo->version = fh->ver;
	
	/* get bitrate and nSlots from table, unless brIdx == 0 (free mode) in which case caller must figure it out himself
	 * question - do we want to overwrite mp3DecInfo->bitrate with 0 each time if it's free mode, and
	 *  copy the pre-calculated actual free bitrate into it in mp3dec.c (according to the spec, 
	 *  this shouldn't be necessary, since it should be either all frames free or none free)
	 */
	if (fh->brIdx) {
		mp3DecInfo->bitrate = ((int)bitrateTab[fh->ver][fh->layer - 1][fh->brIdx]) * 1000;
	
		/* nSlots = total frame bytes (from table) - sideInfo bytes - header - CRC (if present) + pad (if present) */
		mp3DecInfo->nSlots = (int)slotTab[fh->ver][fh->srIdx][fh->brIdx] - 
			(int)sideBytesTab[fh->ver][(fh->sMode == Mono ? 0 : 1)] - 
			4 - (fh->crc ? 2 : 0) + (fh->paddingBit ? 1 : 0);
	}

	/* load crc word, if enabled, and return length of frame header (in bytes) */
	if (fh->crc) {
		fh->CRCWord = ((int)buf[4] << 8 | (int)buf[5] << 0);
		return 6;
	} else {
		fh->CRCWord = 0;
		return 4;
	}
}

/**************************************************************************************
 * Function:    UnpackSideInfo
 *
 * Description: parse the fields of the MP3 side info header
 *
 * Inputs:      MP3DecInfo structure filled by UnpackFrameHeader()
 *              buffer pointing to the MP3 side info data
 *
 * Outputs:     updated mainDataBegin in MP3DecInfo struct
 *              updated private (platform-specific) SideInfo struct
 *
 * Return:      length (in bytes) of side info data
 *              -1 if null input pointers
 **************************************************************************************/
int UnpackSideInfo(MP3DecInfo *mp3DecInfo, unsigned char *buf)
{
	int gr, ch, bd, nBytes;
	BitStreamInfo bitStreamInfo, *bsi;
	FrameHeader *fh;
	SideInfo *si;
	SideInfoSub *sis;

	/* validate pointers and sync word */
	if (!mp3DecInfo || !mp3DecInfo->FrameHeaderPS || !mp3DecInfo->SideInfoPS)
		return -1;

	fh = ((FrameHeader *)(mp3DecInfo->FrameHeaderPS));
	si = ((SideInfo *)(mp3DecInfo->SideInfoPS));

	bsi = &bitStreamInfo;
	if (fh->ver == MPEG1) {
		/* MPEG 1 */
		nBytes = (fh->sMode == Mono ? SIBYTES_MPEG1_MONO : SIBYTES_MPEG1_STEREO);
		SetBitstreamPointer(bsi, nBytes, buf);
		si->mainDataBegin = GetBits(bsi, 9);
		si->privateBits =   GetBits(bsi, (fh->sMode == Mono ? 5 : 3));

		for (ch = 0; ch < mp3DecInfo->nChans; ch++)
			for (bd = 0; bd < MAX_SCFBD; bd++)
				si->scfsi[ch][bd] = GetBits(bsi, 1);
	} else {
		/* MPEG 2, MPEG 2.5 */
		nBytes = (fh->sMode == Mono ? SIBYTES_MPEG2_MONO : SIBYTES_MPEG2_STEREO);
		SetBitstreamPointer(bsi, nBytes, buf);
		si->mainDataBegin = GetBits(bsi, 8);
		si->privateBits =   GetBits(bsi, (fh->sMode == Mono ? 1 : 2));
	}

	for(gr =0; gr < mp3DecInfo->nGrans; gr++) {
		for (ch = 0; ch < mp3DecInfo->nChans; ch++) {
			sis = &si->sis[gr][ch];						/* side info subblock for this granule, channel */

			sis->part23Length =    GetBits(bsi, 12);
			sis->nBigvals =        GetBits(bsi, 9);
			sis->globalGain =      GetBits(bsi, 8);
			sis->sfCompress =      GetBits(bsi, (fh->ver == MPEG1 ? 4 : 9));
			sis->winSwitchFlag =   GetBits(bsi, 1);

			if(sis->winSwitchFlag) {
				/* this is a start, stop, short, or mixed block */
				sis->blockType =       GetBits(bsi, 2);		/* 0 = normal, 1 = start, 2 = short, 3 = stop */
				sis->mixedBlock =      GetBits(bsi, 1);		/* 0 = not mixed, 1 = mixed */
				sis->tableSelect[0] =  GetBits(bsi, 5);
				sis->tableSelect[1] =  GetBits(bsi, 5);
				sis->tableSelect[2] =  0;					/* unused */
				sis->subBlockGain[0] = GetBits(bsi, 3);
				sis->subBlockGain[1] = GetBits(bsi, 3);
				sis->subBlockGain[2] = GetBits(bsi, 3);

				/* TODO - check logic */
				if (sis->blockType == 0) {
					/* this should not be allowed, according to spec */
					sis->nBigvals = 0;
					sis->part23Length = 0;
					sis->sfCompress = 0;
				} else if (sis->blockType == 2 && sis->mixedBlock == 0) {
					/* short block, not mixed */
					sis->region0Count = 8;
				} else {
					/* start, stop, or short-mixed */
					sis->region0Count = 7;
				}
				sis->region1Count = 20 - sis->region0Count;
			} else {
				/* this is a normal block */
				sis->blockType = 0;
				sis->mixedBlock = 0;
				sis->tableSelect[0] =  GetBits(bsi, 5);
				sis->tableSelect[1] =  GetBits(bsi, 5);
				sis->tableSelect[2] =  GetBits(bsi, 5);
				sis->region0Count =    GetBits(bsi, 4);
				sis->region1Count =    GetBits(bsi, 3);
			}
			sis->preFlag =           (fh->ver == MPEG1 ? GetBits(bsi, 1) : 0);
			sis->sfactScale =        GetBits(bsi, 1);
			sis->count1TableSelect = GetBits(bsi, 1);
		}
	}
	mp3DecInfo->mainDataBegin = si->mainDataBegin;	/* needed by main decode loop */

	ASSERT(nBytes == CalcBitsUsed(bsi, buf, 0) >> 3);

	return nBytes;	
}

