//
// JPEG Display helper class
//
// written by Larry Bank
// bitbank@pobox.com
//
// Copyright 2025 BitBank Software, Inc. All Rights Reserved.
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
#ifndef __JPEGDISPLAY_IMPL__
#define __JPEGDISPLAY_IMPL__
#include "JPEGDisplay.h"

static int JPEGDraw(JPEGDRAW *pDraw)
{
BB_SPI_LCD *pLCD = (BB_SPI_LCD *)pDraw->pUser;

    pLCD->setAddrWindow(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    pLCD->pushPixels((uint16_t *)pDraw->pPixels, pDraw->iWidth * pDraw->iHeight);
    return 1;
} /* JPEGDraw() */

static void * jpegOpen(const char *filename, int32_t *size) {
  static File myfile;
  myfile = SD.open(filename);
  *size = myfile.size();
  return &myfile;
}
static void * jpegOpenLFS(const char *filename, int32_t *size) {
  static File myfile;
  myfile = LittleFS.open(filename, FILE_READ);
  if (myfile) {
      *size = myfile.size();
      return &myfile;
  } else {
     return NULL;
  }
}           
static void jpegClose(void *handle) {
  File *pFile = (File *)handle;
  if (pFile) pFile->close();
}
static int32_t jpegRead(JPEGFILE *handle, uint8_t *buffer, int32_t length) {
  File *pFile = (File *)handle->fHandle;
  if (!pFile) return 0;
  return pFile->read(buffer, length);
}
static int32_t jpegSeek(JPEGFILE *handle, int32_t position) {
  File *pFile = (File *)handle->fHandle;
  if (!pFile) return 0;
  return pFile->seek(position);
}

int JPEGDisplay::loadJPEG(BB_SPI_LCD *pLCD, int x, int y, const void *pData, int iDataSize, int iOptions)
{
JPEGDEC *jpeg;
int w, h, rc;

    jpeg = (JPEGDEC *)malloc(sizeof(JPEGDEC));
    if (!jpeg) {
        _iLastError = JPEG_ERROR_MEMORY;
        return 0;
    }
    if (jpeg->openRAM((uint8_t *)pData, iDataSize, JPEGDraw)) {
        jpeg->setPixelType(RGB565_BIG_ENDIAN);
        w = jpeg->getWidth();
        h = jpeg->getHeight();
        if (x == JPEGDISPLAY_CENTER) {
            x = (pLCD->width() - w)/2;
            if (x < 0) x = 0;
        } else if (x < 0 || w + x > pLCD->width()) {
            _iLastError = JPEG_INVALID_PARAMETER;
            free(jpeg);
            return 0; // clipping not supported
        }
        if (y == JPEGDISPLAY_CENTER) {
            y = (pLCD->height() - h)/2;
            if (y < 0) y = 0;
        } else if (y < 0 || y + h > pLCD->height()) {
        // clipping is not supported
            _iLastError = JPEG_INVALID_PARAMETER;
            free(jpeg);
            return 0;
        }
        jpeg->setUserPointer((void *)pLCD);
        jpeg->decode(x, y, iOptions); // perform decoding
        jpeg->close();
        free(jpeg);
        _iLastError = JPEG_SUCCESS;
        return 1;
   }
   _iLastError = jpeg->getLastError();
   free(jpeg);
   return 0;
} /* loadJPEG() */

int JPEGDisplay::loadJPEG(BB_SPI_LCD *pLCD, int x, int y, const char *fname, int iOptions)
{
    JPEGDEC *jpeg;
    int w, h, rc;

    jpeg = (JPEGDEC *)malloc(sizeof(JPEGDEC));
    if (!jpeg) {
        _iLastError = JPEG_ERROR_MEMORY;
        return 0;
    }
    if (jpeg->open(fname, jpegOpen, jpegClose, jpegRead, jpegSeek, JPEGDraw)) {
        jpeg->setPixelType(RGB565_BIG_ENDIAN);
        w = jpeg->getWidth();
        h = jpeg->getHeight();
        if (x == JPEGDISPLAY_CENTER) {
            x = (pLCD->width() - w)/2;
            if (x < 0) x = 0;
        } else if (x < 0 || w + x > pLCD->width()) {
            _iLastError = JPEG_INVALID_PARAMETER;
            free(jpeg);
            return 0; // clipping not supported
        }
        if (y == JPEGDISPLAY_CENTER) {
            y = (pLCD->height() - h)/2;
            if (y < 0) y = 0;
        } else if (y < 0 || y + h > pLCD->height()) {
        // clipping is not supported
            _iLastError = JPEG_INVALID_PARAMETER;
            free(jpeg);
            return 0;
        }
        jpeg->setUserPointer((void *)pLCD);
        jpeg->decode(x, y, iOptions); // decode the image
        jpeg->close();
        free(jpeg);
        _iLastError = JPEG_SUCCESS;
        return 1;
    }
    _iLastError = jpeg->getLastError();
    free(jpeg);
    return 0;
} /* loadJPEG() */

int JPEGDisplay::loadJPEG_LFS(BB_SPI_LCD *pLCD, int x, int y, const char *fname, int iOptions)
{
    JPEGDEC *jpeg;
    int w, h, rc;

    if (!LittleFS.begin(false)) {
        return 0;
    }
    jpeg = (JPEGDEC *)malloc(sizeof(JPEGDEC));
    if (!jpeg) {
        _iLastError = JPEG_ERROR_MEMORY;
        return 0;
    }
    if (jpeg->open(fname, jpegOpenLFS, jpegClose, jpegRead, jpegSeek, JPEGDraw)) {
        jpeg->setPixelType(RGB565_BIG_ENDIAN);
        w = jpeg->getWidth();
        h = jpeg->getHeight();
        if (x == JPEGDISPLAY_CENTER) {
            x = (pLCD->width() - w)/2;
            if (x < 0) x = 0;
        } else if (x < 0 || w + x > pLCD->width()) {
            jpeg->close();
            free(jpeg);
            _iLastError = JPEG_INVALID_PARAMETER;
            return 0; // clipping not supported
        }
        if (y == JPEGDISPLAY_CENTER) {
            y = (pLCD->height() - h)/2;
            if (y < 0) y = 0;
        } else if (y < 0 || y + h > pLCD->height()) {
        // clipping is not supported
            jpeg->close();
            free(jpeg);
            _iLastError = JPEG_INVALID_PARAMETER;
            return 0;
        }
        jpeg->setUserPointer((void *)pLCD);
        jpeg->decode(x, y, iOptions); // decode the image
        jpeg->close();
        _iLastError = JPEG_SUCCESS;
        free(jpeg);
        return 1;
    } else {
//        Serial.printf("jpeg->open failed with code: %d\n", jpeg->getLastError());
    }
    _iLastError = jpeg->getLastError();
    free(jpeg);
    return 0;
} /* loadJPEG_LFS() */

int JPEGDisplay::getJPEGInfo(int *width, int *height, int *bpp, const void *pData, int iDataSize)
{
    JPEGDEC *jpeg;
    int rc;

    if (!width || !height || !bpp || !pData || iDataSize < 32) return 0;
    
    jpeg = (JPEGDEC *)malloc(sizeof(JPEGDEC));
    if (!jpeg) {
        _iLastError = JPEG_ERROR_MEMORY;
        return 0;
    }
    if (jpeg->openRAM((uint8_t *)pData, iDataSize, JPEGDraw)) {
        *width = jpeg->getWidth();
        *height = jpeg->getHeight();
        *bpp = jpeg->getBpp();
        free(jpeg);
        _iLastError = JPEG_SUCCESS;
        return 1;
    }
    _iLastError = jpeg->getLastError();
    free(jpeg);
    return 0;
} /* getJPEGInfo() */

int JPEGDisplay::getJPEGInfo(int *width, int *height, int *bpp, const char *fname)
{
    JPEGDEC *jpeg;
    int rc;

    if (!width || !height || !bpp || !fname) return 0;
    jpeg = (JPEGDEC *)malloc(sizeof(JPEGDEC));
    if (!jpeg) {
        _iLastError = JPEG_ERROR_MEMORY;
        return 0;
    }
    if (jpeg->open(fname, jpegOpen, jpegClose, jpegRead, jpegSeek, JPEGDraw)) {
        *width = jpeg->getWidth();
        *height = jpeg->getHeight();
        *bpp = jpeg->getBpp();
        jpeg->close();
        free(jpeg);
        _iLastError = JPEG_SUCCESS;
        return 1;
    }
    _iLastError = jpeg->getLastError();
    free(jpeg);
    return 0;
} /* getJPEGInfo() */

int JPEGDisplay::getJPEGInfo_LFS(int *width, int *height, int *bpp, const char *fname)
{
    JPEGDEC *jpeg;
    int rc;

    if (!LittleFS.begin(false)) {
        return 0;
    }
    if (!width || !height || !bpp || !fname) return 0;
    jpeg = (JPEGDEC *)malloc(sizeof(JPEGDEC));
    if (!jpeg) {
        _iLastError = JPEG_ERROR_MEMORY;
        return 0;
    }
    if (jpeg->open(fname, jpegOpenLFS, jpegClose, jpegRead, jpegSeek, JPEGDraw)) {
        *width = jpeg->getWidth();
        *height = jpeg->getHeight();
        *bpp = jpeg->getBpp();
        jpeg->close();
        free(jpeg);
        _iLastError = JPEG_SUCCESS;
        return 1;
    }
    _iLastError = jpeg->getLastError();
    free(jpeg);
    return 0;
} /* getJPEGInfo_LFS() */

#endif // __JPEGDISPLAY_IMPL__
