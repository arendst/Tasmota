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
 * mp3tabs.c - platform-independent tables for MP3 decoder (global, read-only)
 **************************************************************************************/

#include "mp3common.h"
#include <pgmspace.h>

/* indexing = [version][samplerate index]
 * sample rate of frame (Hz)
 */
const int samplerateTab[3][3] PROGMEM = {
  {44100, 48000, 32000},		/* MPEG-1 */
  {22050, 24000, 16000},		/* MPEG-2 */
  {11025, 12000,  8000},		/* MPEG-2.5 */
};

/* indexing = [version][layer][bitrate index]
 * bitrate (kbps) of frame
 *   - bitrate index == 0 is "free" mode (bitrate determined on the fly by
 *       counting bits between successive sync words)
 */
const int/*short*/ bitrateTab[3][3][15] PROGMEM = {
	{
		/* MPEG-1 */
		{  0, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448}, /* Layer 1 */
		{  0, 32, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,384}, /* Layer 2 */
		{  0, 32, 40, 48, 56, 64, 80, 96,112,128,160,192,224,256,320}, /* Layer 3 */
	},
	{
		/* MPEG-2 */
		{  0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256}, /* Layer 1 */
		{  0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160}, /* Layer 2 */
		{  0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160}, /* Layer 3 */
	},
	{
		/* MPEG-2.5 */
		{  0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256}, /* Layer 1 */
		{  0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160}, /* Layer 2 */
		{  0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160}, /* Layer 3 */
	},
};

/* indexing = [version][layer]
 * number of samples in one frame (per channel)
 */
const int/*short*/ samplesPerFrameTab[3][3] PROGMEM = {
	{384, 1152, 1152 }, /* MPEG1 */
	{384, 1152,  576 }, /* MPEG2 */
	{384, 1152,  576 }, /* MPEG2.5 */
};

/* layers 1, 2, 3 */
const short bitsPerSlotTab[3] = {32, 8, 8};

/* indexing = [version][mono/stereo]
 * number of bytes in side info section of bitstream 
 */
const int/*short*/ sideBytesTab[3][2] PROGMEM = {
	{17, 32},	/* MPEG-1:   mono, stereo */
	{ 9, 17},	/* MPEG-2:   mono, stereo */
	{ 9, 17},	/* MPEG-2.5: mono, stereo */
};

/* indexing = [version][sampleRate][bitRate] 
 * for layer3, nSlots = floor(samps/frame * bitRate / sampleRate / 8)
 *   - add one pad slot if necessary
 */
const int/*short*/ slotTab[3][3][15] PROGMEM = {
	{
		/* MPEG-1 */
		{ 0, 104, 130, 156, 182, 208, 261, 313, 365, 417, 522, 626, 731, 835,1044 },	/* 44 kHz */
		{ 0,  96, 120, 144, 168, 192, 240, 288, 336, 384, 480, 576, 672, 768, 960 },	/* 48 kHz */
		{ 0, 144, 180, 216, 252, 288, 360, 432, 504, 576, 720, 864,1008,1152,1440 },	/* 32 kHz */
	},
	{	
		/* MPEG-2 */
		{ 0,  26,  52,  78, 104, 130, 156, 182, 208, 261, 313, 365, 417, 470, 522 },	/* 22 kHz */
		{ 0,  24,  48,  72,  96, 120, 144, 168, 192, 240, 288, 336, 384, 432, 480 },	/* 24 kHz */
		{ 0,  36,  72, 108, 144, 180, 216, 252, 288, 360, 432, 504, 576, 648, 720 },	/* 16 kHz */
	},
	{
		/* MPEG-2.5 */
		{ 0,  52, 104, 156, 208, 261, 313, 365, 417, 522, 626, 731, 835, 940,1044 },	/* 11 kHz */
		{ 0,  48,  96, 144, 192, 240, 288, 336, 384, 480, 576, 672, 768, 864, 960 },	/* 12 kHz */
		{ 0,  72, 144, 216, 288, 360, 432, 504, 576, 720, 864,1008,1152,1296,1440 },	/*  8 kHz */
	},
};

/* indexing = [version][sampleRate][long (.l) or short (.s) block] 
 *   sfBandTable[v][s].l[cb] = index of first bin in critical band cb (long blocks)
 *   sfBandTable[v][s].s[cb] = index of first bin in critical band cb (short blocks)
 */
const SFBandTable sfBandTable[3][3] PROGMEM = {
	{
		/* MPEG-1 (44, 48, 32 kHz) */
		{
			{ 0,  4,  8, 12, 16, 20, 24, 30, 36, 44, 52, 62, 74, 90,110,134,162,196,238,288,342,418,576 },
			{ 0,  4,  8, 12, 16, 22, 30, 40, 52, 66, 84,106,136,192 }
		},
		{
			{ 0,  4,  8, 12, 16, 20, 24, 30, 36, 42, 50, 60, 72, 88,106,128,156,190,230,276,330,384,576 },
			{ 0,  4,  8, 12, 16, 22, 28, 38, 50, 64, 80,100,126,192 }
		},
		{
			{ 0,  4,  8, 12, 16, 20, 24, 30, 36, 44, 54, 66, 82,102,126,156,194,240,296,364,448,550,576 },
			{ 0,  4,  8, 12, 16, 22, 30, 42, 58, 78,104,138,180,192 }
		}
	},

	{
		/* MPEG-2 (22, 24, 16 kHz) */
		{
			{ 0,  6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96,116,140,168,200,238,284,336,396,464,522,576 },
			{ 0,  4,  8, 12, 18, 24, 32, 42, 56, 74,100,132,174,192 }
		},
		{
			{ 0,  6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96,114,136,162,194,232,278,332,394,464,540,576 },
			{ 0,  4,  8, 12, 18, 26, 36, 48, 62, 80,104,136,180,192 }
		},
		{
			{ 0,  6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96,116,140,168,200,238,284,336,396,464,522,576 }, 
			{ 0,  4,  8, 12, 18, 26, 36, 48, 62, 80,104,134,174,192 }
		},
	},

	{
		/* MPEG-2.5 (11, 12, 8 kHz) */
		{
			{ 0,  6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96,116,140,168,200,238,284,336,396,464,522,576 },
			{ 0,  4,  8, 12, 18, 26, 36, 48, 62, 80,104,134,174,192 }
		},
		{
			{ 0,  6, 12, 18, 24, 30, 36, 44, 54, 66, 80, 96,116,140,168,200,238,284,336,396,464,522,576 },
			{ 0,  4,  8, 12, 18, 26, 36, 48, 62, 80,104,134,174,192 }
		},
		{
			{ 0, 12, 24, 36, 48, 60, 72, 88,108,132,160,192,232,280,336,400,476,566,568,570,572,574,576 },
			{ 0,  8, 16, 24, 36, 52, 72, 96,124,160,162,164,166,192 }
		},
	},
};


