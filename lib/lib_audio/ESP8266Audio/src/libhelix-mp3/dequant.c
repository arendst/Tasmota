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
 * dequant.c - dequantization, stereo processing (intensity, mid-side), short-block
 *               coefficient reordering
 **************************************************************************************/

#include "coder.h"
#include "assembly.h"

/**************************************************************************************
 * Function:    Dequantize
 *
 * Description: dequantize coefficients, decode stereo, reorder short blocks
 *                (one granule-worth)
 *
 * Inputs:      MP3DecInfo structure filled by UnpackFrameHeader(), UnpackSideInfo(),
 *                UnpackScaleFactors(), and DecodeHuffman() (for this granule)
 *              index of current granule
 *
 * Outputs:     dequantized and reordered coefficients in hi->huffDecBuf 
 *                (one granule-worth, all channels), format = Q26
 *              operates in-place on huffDecBuf but also needs di->workBuf
 *              updated hi->nonZeroBound index for both channels
 *
 * Return:      0 on success, -1 if null input pointers
 *
 * Notes:       In calling output Q(DQ_FRACBITS_OUT), we assume an implicit bias 
 *                of 2^15. Some (floating-point) reference implementations factor this 
 *                into the 2^(0.25 * gain) scaling explicitly. But to avoid precision 
 *                loss, we don't do that. Instead take it into account in the final 
 *                round to PCM (>> by 15 less than we otherwise would have).
 *              Equivalently, we can think of the dequantized coefficients as 
 *                Q(DQ_FRACBITS_OUT - 15) with no implicit bias. 
 **************************************************************************************/
int Dequantize(MP3DecInfo *mp3DecInfo, int gr)
{
	int i, ch, nSamps, mOut[2];
	FrameHeader *fh;
	SideInfo *si;
	ScaleFactorInfo *sfi;
	HuffmanInfo *hi;
	DequantInfo *di;
	CriticalBandInfo *cbi;

	/* validate pointers */
	if (!mp3DecInfo || !mp3DecInfo->FrameHeaderPS || !mp3DecInfo->SideInfoPS || !mp3DecInfo->ScaleFactorInfoPS || 
		!mp3DecInfo->HuffmanInfoPS || !mp3DecInfo->DequantInfoPS)
		return -1;

	fh = (FrameHeader *)(mp3DecInfo->FrameHeaderPS);

	/* si is an array of up to 4 structs, stored as gr0ch0, gr0ch1, gr1ch0, gr1ch1 */
	si = (SideInfo *)(mp3DecInfo->SideInfoPS);
	sfi = (ScaleFactorInfo *)(mp3DecInfo->ScaleFactorInfoPS);
	hi = (HuffmanInfo *)mp3DecInfo->HuffmanInfoPS;
	di = (DequantInfo *)mp3DecInfo->DequantInfoPS;
	cbi = di->cbi;
	mOut[0] = mOut[1] = 0;

	/* dequantize all the samples in each channel */
	for (ch = 0; ch < mp3DecInfo->nChans; ch++) {
		hi->gb[ch] = DequantChannel(hi->huffDecBuf[ch], di->workBuf, &hi->nonZeroBound[ch], fh, 
			&si->sis[gr][ch], &sfi->sfis[gr][ch], &cbi[ch]);
	}

	/* joint stereo processing assumes one guard bit in input samples
	 * it's extremely rare not to have at least one gb, so if this is the case
	 *   just make a pass over the data and clip to [-2^30+1, 2^30-1]
	 * in practice this may never happen
	 */
	if (fh->modeExt && (hi->gb[0] < 1 || hi->gb[1] < 1)) {
		for (i = 0; i < hi->nonZeroBound[0]; i++) {
			if (hi->huffDecBuf[0][i] < -0x3fffffff)	 hi->huffDecBuf[0][i] = -0x3fffffff;
			if (hi->huffDecBuf[0][i] >  0x3fffffff)	 hi->huffDecBuf[0][i] =  0x3fffffff;
		}
		for (i = 0; i < hi->nonZeroBound[1]; i++) {
			if (hi->huffDecBuf[1][i] < -0x3fffffff)	 hi->huffDecBuf[1][i] = -0x3fffffff;
			if (hi->huffDecBuf[1][i] >  0x3fffffff)	 hi->huffDecBuf[1][i] =  0x3fffffff;
		}
	}

	/* do mid-side stereo processing, if enabled */
	if (fh->modeExt >> 1) {
		if (fh->modeExt & 0x01) {
			/* intensity stereo enabled - run mid-side up to start of right zero region */
			if (cbi[1].cbType == 0)
				nSamps = fh->sfBand->l[cbi[1].cbEndL + 1];
			else 
				nSamps = 3 * fh->sfBand->s[cbi[1].cbEndSMax + 1];
		} else {
			/* intensity stereo disabled - run mid-side on whole spectrum */
			nSamps = MAX(hi->nonZeroBound[0], hi->nonZeroBound[1]);
		}
		MidSideProc(hi->huffDecBuf, nSamps, mOut);
	}

	/* do intensity stereo processing, if enabled */
	if (fh->modeExt & 0x01) {
		nSamps = hi->nonZeroBound[0];
		if (fh->ver == MPEG1) {
			IntensityProcMPEG1(hi->huffDecBuf, nSamps, fh, &sfi->sfis[gr][1], di->cbi, 
				fh->modeExt >> 1, si->sis[gr][1].mixedBlock, mOut);
		} else {
			IntensityProcMPEG2(hi->huffDecBuf, nSamps, fh, &sfi->sfis[gr][1], di->cbi, &sfi->sfjs,
				fh->modeExt >> 1, si->sis[gr][1].mixedBlock, mOut);
		}
	}

	/* adjust guard bit count and nonZeroBound if we did any stereo processing */
	if (fh->modeExt) {
		hi->gb[0] = CLZ(mOut[0]) - 1;
		hi->gb[1] = CLZ(mOut[1]) - 1;
		nSamps = MAX(hi->nonZeroBound[0], hi->nonZeroBound[1]);
		hi->nonZeroBound[0] = nSamps;
		hi->nonZeroBound[1] = nSamps;
	}

	/* output format Q(DQ_FRACBITS_OUT) */
	return 0;
}
