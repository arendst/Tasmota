/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: decelmnt.c,v 1.1 2005/02/26 01:47:34 jrecker Exp $ 
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
 * decelmnt.c - syntactic element decoding
 **************************************************************************************/

#include "coder.h"

/**************************************************************************************
 * Function:    DecodeSingleChannelElement
 *
 * Description: decode one SCE
 *
 * Inputs:      BitStreamInfo struct pointing to start of SCE (14496-3, table 4.4.4) 
 *
 * Outputs:     updated element instance tag
 *
 * Return:      0 if successful, -1 if error
 *
 * Notes:       doesn't decode individual channel stream (part of DecodeNoiselessData)
 **************************************************************************************/
static int DecodeSingleChannelElement(AACDecInfo *aacDecInfo, BitStreamInfo *bsi)
{
	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return -1;

	/* read instance tag */
	aacDecInfo->currInstTag = GetBits(bsi, NUM_INST_TAG_BITS);

	return 0;
}

/**************************************************************************************
 * Function:    DecodeChannelPairElement
 *
 * Description: decode one CPE
 *
 * Inputs:      BitStreamInfo struct pointing to start of CPE (14496-3, table 4.4.5) 
 *
 * Outputs:     updated element instance tag
 *              updated commonWin
 *              updated ICS info, if commonWin == 1
 *              updated mid-side stereo info, if commonWin == 1
 *
 * Return:      0 if successful, -1 if error
 *
 * Notes:       doesn't decode individual channel stream (part of DecodeNoiselessData)
 **************************************************************************************/
static int DecodeChannelPairElement(AACDecInfo *aacDecInfo, BitStreamInfo *bsi)
{
	int sfb, gp, maskOffset;
	unsigned char currBit, *maskPtr;
	PSInfoBase *psi;
	ICSInfo *icsInfo;

	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return -1;
	psi = (PSInfoBase *)(aacDecInfo->psInfoBase);
	icsInfo = psi->icsInfo;

	/* read instance tag */
	aacDecInfo->currInstTag = GetBits(bsi, NUM_INST_TAG_BITS);

	/* read common window flag and mid-side info (if present) 
	 * store msMask bits in psi->msMaskBits[] as follows:
	 *  long blocks -  pack bits for each SFB in range [0, maxSFB) starting with lsb of msMaskBits[0]
	 *  short blocks - pack bits for each SFB in range [0, maxSFB), for each group [0, 7]
	 * msMaskPresent = 0 means no M/S coding
	 *               = 1 means psi->msMaskBits contains 1 bit per SFB to toggle M/S coding
	 *               = 2 means all SFB's are M/S coded (so psi->msMaskBits is not needed)
	 */
	psi->commonWin = GetBits(bsi, 1);
	if (psi->commonWin) {
		DecodeICSInfo(bsi, icsInfo, psi->sampRateIdx);
		psi->msMaskPresent = GetBits(bsi, 2);
		if (psi->msMaskPresent == 1) {
			maskPtr = psi->msMaskBits;
			*maskPtr = 0;
			maskOffset = 0;
			for (gp = 0; gp < icsInfo->numWinGroup; gp++) {
				for (sfb = 0; sfb < icsInfo->maxSFB; sfb++) {
					currBit = (unsigned char)GetBits(bsi, 1);
					*maskPtr |= currBit << maskOffset;
					if (++maskOffset == 8) {
						maskPtr++;
						*maskPtr = 0;
						maskOffset = 0;
					}
				}		
			}
		}
	}

	return 0;
}

/**************************************************************************************
 * Function:    DecodeLFEChannelElement
 *
 * Description: decode one LFE
 *
 * Inputs:      BitStreamInfo struct pointing to start of LFE (14496-3, table 4.4.9) 
 *
 * Outputs:     updated element instance tag
 *
 * Return:      0 if successful, -1 if error
 *
 * Notes:       doesn't decode individual channel stream (part of DecodeNoiselessData)
 **************************************************************************************/
static int DecodeLFEChannelElement(AACDecInfo *aacDecInfo, BitStreamInfo *bsi)
{
	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return -1;

	/* read instance tag */
	aacDecInfo->currInstTag = GetBits(bsi, NUM_INST_TAG_BITS);

	return 0;
}

/**************************************************************************************
 * Function:    DecodeDataStreamElement
 *
 * Description: decode one DSE
 *
 * Inputs:      BitStreamInfo struct pointing to start of DSE (14496-3, table 4.4.10) 
 *
 * Outputs:     updated element instance tag
 *              filled in data stream buffer
 *
 * Return:      0 if successful, -1 if error
 **************************************************************************************/
static int DecodeDataStreamElement(AACDecInfo *aacDecInfo, BitStreamInfo *bsi)
{
	unsigned int byteAlign, dataCount;
	unsigned char *dataBuf;
	PSInfoBase *psi;

	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return -1;
	psi = (PSInfoBase *)(aacDecInfo->psInfoBase);

	aacDecInfo->currInstTag = GetBits(bsi, NUM_INST_TAG_BITS);
	byteAlign = GetBits(bsi, 1);
	dataCount = GetBits(bsi, 8);
	if (dataCount == 255)
		dataCount += GetBits(bsi, 8);

	if (byteAlign)
		ByteAlignBitstream(bsi);

	psi->dataCount = dataCount;
	dataBuf = psi->dataBuf;
	while (dataCount--)
		*dataBuf++ = GetBits(bsi, 8);

	return 0;
}

/**************************************************************************************
 * Function:    DecodeProgramConfigElement
 *
 * Description: decode one PCE
 *
 * Inputs:      BitStreamInfo struct pointing to start of PCE (14496-3, table 4.4.2) 
 *
 * Outputs:     filled-in ProgConfigElement struct
 *              updated BitStreamInfo struct
 *
 * Return:      0 if successful, error code (< 0) if error
 *
 * Notes:       #define KEEP_PCE_COMMENTS to save the comment field of the PCE
 *                (otherwise we just skip it in the bitstream, to save memory)
 **************************************************************************************/
int DecodeProgramConfigElement(ProgConfigElement *pce, BitStreamInfo *bsi)
{
	int i;

	pce->elemInstTag =   GetBits(bsi, 4);
	pce->profile =       GetBits(bsi, 2);
	pce->sampRateIdx =   GetBits(bsi, 4);
	pce->numFCE =        GetBits(bsi, 4);
	pce->numSCE =        GetBits(bsi, 4);
	pce->numBCE =        GetBits(bsi, 4);
	pce->numLCE =        GetBits(bsi, 2);
	pce->numADE =        GetBits(bsi, 3);
	pce->numCCE =        GetBits(bsi, 4);

	pce->monoMixdown = GetBits(bsi, 1) << 4;	/* present flag */
	if (pce->monoMixdown)
		pce->monoMixdown |= GetBits(bsi, 4);	/* element number */

	pce->stereoMixdown = GetBits(bsi, 1) << 4;	/* present flag */
	if (pce->stereoMixdown)
		pce->stereoMixdown  |= GetBits(bsi, 4);	/* element number */

	pce->matrixMixdown = GetBits(bsi, 1) << 4;	/* present flag */
	if (pce->matrixMixdown) {
		pce->matrixMixdown  |= GetBits(bsi, 2) << 1;	/* index */
		pce->matrixMixdown  |= GetBits(bsi, 1);			/* pseudo-surround enable */
	}

	for (i = 0; i < pce->numFCE; i++) {
		pce->fce[i]  = GetBits(bsi, 1) << 4;	/* is_cpe flag */
		pce->fce[i] |= GetBits(bsi, 4);			/* tag select */
	}

	for (i = 0; i < pce->numSCE; i++) {
		pce->sce[i]  = GetBits(bsi, 1) << 4;	/* is_cpe flag */
		pce->sce[i] |= GetBits(bsi, 4);			/* tag select */
	}

	for (i = 0; i < pce->numBCE; i++) {
		pce->bce[i]  = GetBits(bsi, 1) << 4;	/* is_cpe flag */
		pce->bce[i] |= GetBits(bsi, 4);			/* tag select */
	}

	for (i = 0; i < pce->numLCE; i++)
		pce->lce[i] = GetBits(bsi, 4);			/* tag select */

	for (i = 0; i < pce->numADE; i++)
		pce->ade[i] = GetBits(bsi, 4);			/* tag select */

	for (i = 0; i < pce->numCCE; i++) {
		pce->cce[i]  = GetBits(bsi, 1) << 4;	/* independent/dependent flag */
		pce->cce[i] |= GetBits(bsi, 4);			/* tag select */
	}


	ByteAlignBitstream(bsi);

#ifdef KEEP_PCE_COMMENTS
	pce->commentBytes = GetBits(bsi, 8);
	for (i = 0; i < pce->commentBytes; i++)
		pce->commentField[i] = GetBits(bsi, 8);
#else
	/* eat comment bytes and throw away */
	i = GetBits(bsi, 8);
	while (i--)
		GetBits(bsi, 8);
#endif

	return 0;
}

/**************************************************************************************
 * Function:    DecodeFillElement
 *
 * Description: decode one fill element
 *
 * Inputs:      BitStreamInfo struct pointing to start of fill element
 *                (14496-3, table 4.4.11) 
 *
 * Outputs:     updated element instance tag
 *              unpacked extension payload
 *
 * Return:      0 if successful, -1 if error
 **************************************************************************************/
static int DecodeFillElement(AACDecInfo *aacDecInfo, BitStreamInfo *bsi)
{
	unsigned int fillCount;
	unsigned char *fillBuf;
	PSInfoBase *psi;

	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return -1;
	psi = (PSInfoBase *)(aacDecInfo->psInfoBase);

	fillCount = GetBits(bsi, 4);
	if (fillCount == 15)
		fillCount += (GetBits(bsi, 8) - 1);

	psi->fillCount = fillCount;
	fillBuf = psi->fillBuf;
	while (fillCount--)
		*fillBuf++ = GetBits(bsi, 8);

	aacDecInfo->currInstTag = -1;	/* fill elements don't have instance tag */
	aacDecInfo->fillExtType = 0;

#ifdef AAC_ENABLE_SBR
	/* check for SBR 
	 * aacDecInfo->sbrEnabled is sticky (reset each raw_data_block), so for multichannel 
	 *    need to verify that all SCE/CPE/ICCE have valid SBR fill element following, and 
	 *    must upsample by 2 for LFE
	 */
	if (psi->fillCount > 0) {
		aacDecInfo->fillExtType = (int)((psi->fillBuf[0] >> 4) & 0x0f);
		if (aacDecInfo->fillExtType == EXT_SBR_DATA || aacDecInfo->fillExtType == EXT_SBR_DATA_CRC)
			aacDecInfo->sbrEnabled = 1;
	}
#endif

	aacDecInfo->fillBuf = psi->fillBuf;
	aacDecInfo->fillCount = psi->fillCount;

	return 0;
}

/**************************************************************************************
 * Function:    DecodeNextElement
 *
 * Description: decode next syntactic element in AAC frame
 *
 * Inputs:      valid AACDecInfo struct
 *              double pointer to buffer containing next element
 *              pointer to bit offset
 *              pointer to number of valid bits remaining in buf
 *
 * Outputs:     type of element decoded (aacDecInfo->currBlockID)
 *              type of element decoded last time (aacDecInfo->prevBlockID)
 *              updated aacDecInfo state, depending on which element was decoded
 *              updated buffer pointer
 *              updated bit offset
 *              updated number of available bits
 *
 * Return:      0 if successful, error code (< 0) if error
 **************************************************************************************/
int DecodeNextElement(AACDecInfo *aacDecInfo, unsigned char **buf, int *bitOffset, int *bitsAvail)
{
	int err, bitsUsed;
	PSInfoBase *psi;
	BitStreamInfo bsi;

	/* validate pointers */
	if (!aacDecInfo || !aacDecInfo->psInfoBase)
		return ERR_AAC_NULL_POINTER;
	psi = (PSInfoBase *)(aacDecInfo->psInfoBase);

	/* init bitstream reader */
	SetBitstreamPointer(&bsi, (*bitsAvail + 7) >> 3, *buf);
	GetBits(&bsi, *bitOffset);

	/* read element ID (save last ID for SBR purposes) */
	aacDecInfo->prevBlockID = aacDecInfo->currBlockID;
	aacDecInfo->currBlockID = GetBits(&bsi, NUM_SYN_ID_BITS);

	/* set defaults (could be overwritten by DecodeXXXElement(), depending on currBlockID) */
	psi->commonWin = 0;
 
	err = 0;
	switch (aacDecInfo->currBlockID) {
	case AAC_ID_SCE:
		err = DecodeSingleChannelElement(aacDecInfo, &bsi);
		break;
	case AAC_ID_CPE:
		err = DecodeChannelPairElement(aacDecInfo, &bsi);
		break;
	case AAC_ID_CCE:
		/* TODO - implement CCE decoding */
		break;
	case AAC_ID_LFE:
		err = DecodeLFEChannelElement(aacDecInfo, &bsi);
		break;
	case AAC_ID_DSE:
		err = DecodeDataStreamElement(aacDecInfo, &bsi);
		break;
	case AAC_ID_PCE:
		err = DecodeProgramConfigElement(psi->pce + 0, &bsi);
		break;
	case AAC_ID_FIL:
		err = DecodeFillElement(aacDecInfo, &bsi);
		break;
	case AAC_ID_END:
		break;
	}
	if (err)
		return ERR_AAC_SYNTAX_ELEMENT;

	/* update bitstream reader */
	bitsUsed = CalcBitsUsed(&bsi, *buf, *bitOffset);
	*buf += (bitsUsed + *bitOffset) >> 3;
	*bitOffset = (bitsUsed + *bitOffset) & 0x07;
	*bitsAvail -= bitsUsed;

	if (*bitsAvail < 0)
		return ERR_AAC_INDATA_UNDERFLOW;

	return ERR_AAC_NONE;
}

