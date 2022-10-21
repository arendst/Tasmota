/* ***** BEGIN LICENSE BLOCK *****  
 * Source last modified: $Id: buffers.c,v 1.1 2005/02/26 01:47:34 jrecker Exp $ 
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
 * buffers.c - allocation and deallocation of internal AAC decoder buffers
 **************************************************************************************/

#if defined(USE_DEFAULT_STDLIB) || defined(ARDUINO)
#include <stdlib.h>
#else
#include "hlxclib/stdlib.h"
#endif

#include "coder.h"

/**************************************************************************************
 * Function:    ClearBuffer
 *
 * Description: fill buffer with 0's
 *
 * Inputs:      pointer to buffer
 *              number of bytes to fill with 0
 *
 * Outputs:     cleared buffer
 *
 * Return:      none
 *
 * Notes:       slow, platform-independent equivalent to memset(buf, 0, nBytes)
 **************************************************************************************/
#include <string.h>
 void ClearBuffer(void *buf, int nBytes)
{
/*	int i;
	unsigned char *cbuf = (unsigned char *)buf;

	for (i = 0; i < nBytes; i++)
		cbuf[i] = 0;

	return;
	*/
	memset(buf, 0, nBytes);
}

/**************************************************************************************
 * Function:    AllocateBuffers
 *
 * Description: allocate all the memory needed for the AAC decoder
 *
 * Inputs:      none
 *
 * Outputs:     none
 *
 * Return:      pointer to AACDecInfo structure, cleared to all 0's (except for
 *                pointer to platform-specific data structure)
 *
 * Notes:       if one or more mallocs fail, function frees any buffers already
 *                allocated before returning
 **************************************************************************************/
AACDecInfo *AllocateBuffers(void)
{
	AACDecInfo *aacDecInfo;

	aacDecInfo = (AACDecInfo *)malloc(sizeof(AACDecInfo));
	if (!aacDecInfo)
		return 0;
	ClearBuffer(aacDecInfo, sizeof(AACDecInfo));

	aacDecInfo->psInfoBase = malloc(sizeof(PSInfoBase));
	if (!aacDecInfo->psInfoBase) {
		FreeBuffers(aacDecInfo);
		return 0;
	}
	ClearBuffer(aacDecInfo->psInfoBase, sizeof(PSInfoBase));

	return aacDecInfo;
}

AACDecInfo *AllocateBuffersPre(void **ptr, int *sz)
{
        AACDecInfo *aacDecInfo;

        char *p = (char*)*ptr;
        aacDecInfo = (AACDecInfo *)p;
        p += (sizeof(AACDecInfo) +7 ) & ~7;
        *sz -= (sizeof(AACDecInfo) +7 ) & ~7;
        if (*sz < 0)
                return 0;
        ClearBuffer(aacDecInfo, sizeof(AACDecInfo));

        aacDecInfo->psInfoBase = (PSInfoBase*)p;
        p += (sizeof(PSInfoBase) + 7) & ~7;
        *sz -= (sizeof(PSInfoBase) + 7) & ~7;
        if (*sz <0) {
                return 0;
        }
        ClearBuffer(aacDecInfo->psInfoBase, sizeof(PSInfoBase));

	*ptr = p;

        return aacDecInfo;
}

#ifndef SAFE_FREE
#define SAFE_FREE(x)	{if (x)	free(x);	(x) = 0;}	/* helper macro */
#endif

/**************************************************************************************
 * Function:    FreeBuffers
 *
 * Description: frees all the memory used by the AAC decoder
 *
 * Inputs:      pointer to initialized AACDecInfo structure
 *
 * Outputs:     none
 *
 * Return:      none
 *
 * Notes:       safe to call even if some buffers were not allocated (uses SAFE_FREE)
 **************************************************************************************/
void FreeBuffers(AACDecInfo *aacDecInfo)
{
	if (!aacDecInfo)
		return;

	SAFE_FREE(aacDecInfo->psInfoBase);
	SAFE_FREE(aacDecInfo);
}
