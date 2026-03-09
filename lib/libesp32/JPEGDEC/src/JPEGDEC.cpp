//
// JPEG Decoder
//
// written by Larry Bank
// bitbank@pobox.com
// Arduino port started 8/2/2020
// Original JPEG code written 26+ years ago :)
// The goal of this code is to decode baseline JPEG images
// using no more than 18K of RAM (if sent directly to an LCD display)
//
// Copyright 2020 BitBank Software, Inc. All Rights Reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//    http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//===========================================================================
//
#include "JPEGDEC.h"

// forward references
JPEG_STATIC int JPEGInit(JPEGIMAGE *pJPEG);
JPEG_STATIC int JPEGParseInfo(JPEGIMAGE *pPage, int bExtractThumb);
JPEG_STATIC void JPEGGetMoreData(JPEGIMAGE *pPage);
JPEG_STATIC int DecodeJPEG(JPEGIMAGE *pImage);
JPEG_STATIC void JPEG_setFramebuffer(JPEGIMAGE *pPage, void *pFramebuffer);
void JPEG_setCropArea(JPEGIMAGE *pJPEG, int x, int y, int w, int h);
void JPEG_getCropArea(JPEGIMAGE *pJPEG, int *x, int *y, int *w, int *h);
void JPEG_setFramebuffer(JPEGIMAGE *pJPEG, void *pFramebuffer);

// Include the C code which does the actual work
#include "jpeg.inl"

void JPEGDEC::setFramebuffer(void *pFramebuffer)
{
    JPEG_setFramebuffer(&_jpeg, pFramebuffer);
} /* setFramebuffer() */

int JPEGDEC::getPixelType()
{
    return (int)_jpeg.ucPixelType;
}
void JPEGDEC::setPixelType(int iType)
{
    if (iType >= 0 && iType < INVALID_PIXEL_TYPE)
        _jpeg.ucPixelType = (uint8_t)iType;
    else
        _jpeg.iError = JPEG_INVALID_PARAMETER;
} /* setPixelType() */

void JPEGDEC::setMaxOutputSize(int iMaxMCUs)
{
    if (iMaxMCUs < 1)
        iMaxMCUs = 1; // don't allow invalid value
    _jpeg.iMaxMCUs = iMaxMCUs;
} /* setMaxOutputSize() */
//
// Memory initialization
//
int JPEGDEC::openRAM(uint8_t *pData, int iDataSize, JPEG_DRAW_CALLBACK *pfnDraw)
{
    memset(&_jpeg, 0, sizeof(JPEGIMAGE));
    _jpeg.ucMemType = JPEG_MEM_RAM;
    _jpeg.pfnRead = readRAM;
    _jpeg.pfnSeek = seekMem;
    _jpeg.pfnDraw = pfnDraw;
    _jpeg.pfnOpen = NULL;
    _jpeg.pfnClose = NULL;
    _jpeg.JPEGFile.iSize = iDataSize;
    _jpeg.JPEGFile.pData = pData;
    _jpeg.iMaxMCUs = 1000; // set to an unnaturally high value to start
    return JPEGInit(&_jpeg);
} /* openRAM() */

int JPEGDEC::openFLASH(const uint8_t *pData, int iDataSize, JPEG_DRAW_CALLBACK *pfnDraw)
{
    memset(&_jpeg, 0, sizeof(JPEGIMAGE));
    _jpeg.ucMemType = JPEG_MEM_FLASH;
    _jpeg.pfnRead = readFLASH;
    _jpeg.pfnSeek = seekMem;
    _jpeg.pfnDraw = pfnDraw;
    _jpeg.pfnOpen = NULL;
    _jpeg.pfnClose = NULL;
    _jpeg.JPEGFile.iSize = iDataSize;
    _jpeg.JPEGFile.pData = (uint8_t *)pData;
    _jpeg.iMaxMCUs = 1000; // set to an unnaturally high value to start
    return JPEGInit(&_jpeg);
} /* openFLASH() */

int JPEGDEC::getOrientation()
{
    return (int)_jpeg.ucOrientation;
} /* getOrientation() */

int JPEGDEC::getLastError()
{
    return _jpeg.iError;
} /* getLastError() */

int JPEGDEC::getWidth()
{
    return _jpeg.iWidth;
} /* getWidth() */

int JPEGDEC::getHeight()
{
    return _jpeg.iHeight;
} /* getHeight() */

int JPEGDEC::hasThumb()
{
    return (int)_jpeg.ucHasThumb;
} /* hasThumb() */

int JPEGDEC::getThumbWidth()
{
    return _jpeg.iThumbWidth;
} /* getThumbWidth() */

int JPEGDEC::getThumbHeight()
{
    return _jpeg.iThumbHeight;
} /* getThumbHeight() */

int JPEGDEC::getBpp()
{
    return (int)_jpeg.ucBpp;
} /* getBpp() */

int JPEGDEC::getSubSample()
{
    return (int)_jpeg.ucSubSample;
} /* getSubSample() */

void JPEGDEC::setCropArea(int x, int y, int w, int h)
{
    JPEG_setCropArea(&_jpeg, x, y, w, h);
} /* setCropArea() */

void JPEGDEC::getCropArea(int *x, int *y, int *w, int *h)
{
    JPEG_getCropArea(&_jpeg, x, y, w, h);
} /* getCropArea() */

int JPEGDEC::getJPEGType()
{
    return (_jpeg.ucMode == 0xc2) ? JPEG_MODE_PROGRESSIVE : JPEG_MODE_BASELINE;
} /* getJPEGType() */

#ifdef __LINUX__
int JPEGDEC::open(const char *szFilename, JPEG_DRAW_CALLBACK *pfnDraw)
{
    memset(&_jpeg, 0, sizeof(JPEGIMAGE));
    _jpeg.pfnRead = readFile;
    _jpeg.pfnClose = closeFile;
    _jpeg.pfnSeek = seekFile;
    _jpeg.pfnDraw = pfnDraw;
    _jpeg.iMaxMCUs = 1000;
    _jpeg.JPEGFile.fHandle = openFile(szFilename, &_jpeg.JPEGFile.iSize);
    if (_jpeg.JPEGFile.fHandle == NULL) return 0;
    return JPEGInit(&_jpeg);
} /* open() */
#endif // __LINUX__
//
// File (SD/MMC) based initialization
//
int JPEGDEC::open(const char *szFilename, JPEG_OPEN_CALLBACK *pfnOpen, JPEG_CLOSE_CALLBACK *pfnClose, JPEG_READ_CALLBACK *pfnRead, JPEG_SEEK_CALLBACK *pfnSeek, JPEG_DRAW_CALLBACK *pfnDraw)
{
    memset(&_jpeg, 0, sizeof(JPEGIMAGE));
    _jpeg.pfnRead = pfnRead;
    _jpeg.pfnSeek = pfnSeek;
    _jpeg.pfnDraw = pfnDraw;
    _jpeg.pfnOpen = pfnOpen;
    _jpeg.pfnClose = pfnClose;
    _jpeg.iMaxMCUs = 1000; // set to an unnaturally high value to start
    _jpeg.JPEGFile.fHandle = (*pfnOpen)(szFilename, &_jpeg.JPEGFile.iSize);
    if (_jpeg.JPEGFile.fHandle == NULL)
       return 0;
    return JPEGInit(&_jpeg);

} /* open() */

//
// data stream initialization
//
int JPEGDEC::open(void *fHandle, int iDataSize, JPEG_CLOSE_CALLBACK *pfnClose, JPEG_READ_CALLBACK *pfnRead, JPEG_SEEK_CALLBACK *pfnSeek, JPEG_DRAW_CALLBACK *pfnDraw)
{
    memset(&_jpeg, 0, sizeof(JPEGIMAGE));
    _jpeg.pfnRead = pfnRead;
    _jpeg.pfnSeek = pfnSeek;
    _jpeg.pfnDraw = pfnDraw;
    _jpeg.pfnClose = pfnClose;
    _jpeg.iMaxMCUs = 1000; // set to an unnaturally high value to start
    _jpeg.JPEGFile.iSize = iDataSize;
    _jpeg.JPEGFile.fHandle = fHandle;
    return JPEGInit(&_jpeg);
} /* open() */

#ifdef FS_H
static int32_t FileRead(JPEGFILE *handle, uint8_t *buffer, int32_t length)
{
    return ((File *)(handle->fHandle))->read(buffer, length);
}
static int32_t FileSeek(JPEGFILE *handle, int32_t position)
{
    return ((File *)(handle->fHandle))->seek(position);
}
static void FileClose(void *handle)
{
    ((File *)handle)->close();
}

int JPEGDEC::open(File &file, JPEG_DRAW_CALLBACK *pfnDraw)
{
    if (!file) return 0;
    memset(&_jpeg, 0, sizeof(JPEGIMAGE));
    _jpeg.pfnRead = FileRead;
    _jpeg.pfnSeek = FileSeek;
    _jpeg.pfnClose = FileClose;
    _jpeg.pfnDraw = pfnDraw;
    _jpeg.iMaxMCUs = 1000;
    _jpeg.JPEGFile.fHandle = &file;
    _jpeg.JPEGFile.iSize = file.size();
    return JPEGInit(&_jpeg);
}
#endif // FS_H

void JPEGDEC::close()
{
    if (_jpeg.pfnClose)
        (*_jpeg.pfnClose)(_jpeg.JPEGFile.fHandle);
} /* close() */

//
// Decode the image
// returns:
// 1 = good result
// 0 = error
//
int JPEGDEC::decode(int x, int y, int iOptions)
{
    _jpeg.iXOffset = x;
    _jpeg.iYOffset = y;
    _jpeg.iOptions = iOptions;
    return DecodeJPEG(&_jpeg);
} /* decode() */
//
// set draw callback user pointer variable
//
void JPEGDEC::setUserPointer(void *p)
{
    _jpeg.pUser = p;
}

int JPEGDEC::decodeDither(int x, int y, uint8_t *pDither, int iOptions)
{
    _jpeg.iXOffset = x;
    _jpeg.iYOffset = y;
    _jpeg.iOptions = iOptions;
    _jpeg.pDitherBuffer = pDither;
    return DecodeJPEG(&_jpeg);
}

int JPEGDEC::decodeDither(uint8_t *pDither, int iOptions)
{
    _jpeg.iOptions = iOptions;
    _jpeg.pDitherBuffer = pDither;
    return DecodeJPEG(&_jpeg);
}
