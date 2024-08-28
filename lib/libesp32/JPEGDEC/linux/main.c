// JPEG perf test
// Written by Larry Bank
// 
// Will open an arbitrary JPEG file if passed on the command line
// or will use the sample image (tulips)
//
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "JPEGDEC.h"
#include "jpeg.inl"
#include "../test_images/tulips.h"

// Human readable error messages
const char *szErrors[] = {"Success", "Invalid parameter", "Decode error", "Unsupported feature", "Invalid file"};

/* Windows BMP header for RGB565 images */
uint8_t winbmphdr_rgb565[138] =
        {0x42,0x4d,0,0,0,0,0,0,0,0,0x8a,0,0,0,0x7c,0,
         0,0,0,0,0,0,0,0,0,0,1,0,8,0,3,0,
         0,0,0,0,0,0,0x13,0x0b,0,0,0x13,0x0b,0,0,0,0,
         0,0,0,0,0,0,0,0xf8,0,0,0xe0,0x07,0,0,0x1f,0,
         0,0,0,0,0,0,0x42,0x47,0x52,0x73,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,
         0,0,0,0,0,0,0,0,0,0};

/* Windows BMP header for 8/24/32-bit images (54 bytes) */
uint8_t winbmphdr[54] =
        {0x42,0x4d,
         0,0,0,0,         /* File size */
         0,0,0,0,0x36,4,0,0,0x28,0,0,0,
         0,0,0,0, /* Xsize */
         0,0,0,0, /* Ysize */
         1,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,       /* number of planes, bits per pel */
         0,0,0,0};

JPEGIMAGE jpg;
uint8_t *pFrame;
int iDestPitch;
//uint8_t ucDitherBuffer[1024 * 16];
int ucPixelType = RGB565_BIG_ENDIAN; // default output used for SPI LCDs

//
// Minimal code to save frames as Windows BMP files
//
void WriteBMP(char *fname, uint8_t *pBitmap, uint8_t *pPalette, int cx, int cy, int bpp, int iDestPitch)
{
FILE * oHandle;
int i, bsize, lsize;
uint32_t *l;
uint8_t *s;
uint8_t *ucTemp;
uint8_t *pHdr;
int iHeaderSize;

    ucTemp = (uint8_t *)malloc(cx * 4);

    if (bpp == 16) {
        pHdr = winbmphdr_rgb565;
        iHeaderSize = sizeof(winbmphdr_rgb565);
    } else {
        pHdr = winbmphdr;
        iHeaderSize = sizeof(winbmphdr);
    }
    
    oHandle = fopen(fname, "w+b");
    bsize = (cx * bpp) >> 3;
    lsize = (bsize + 3) & 0xfffc; /* Width of each line */
    pHdr[26] = 1; // number of planes
    pHdr[28] = (uint8_t)bpp;

   /* Write the BMP header */
   l = (uint32_t *)&pHdr[2];
    i =(cy * lsize) + iHeaderSize;
    if (bpp <= 8)
        i += 1024;
   *l = (uint32_t)i; /* Store the file size */
   l = (uint32_t *)&pHdr[34]; // data size
   i = (cy * lsize);
   *l = (uint32_t)i; // store data size
   l = (uint32_t *)&pHdr[18];
   *l = (uint32_t)cx;      /* width */
   *(l+1) = (uint32_t)cy;  /* height */
    l = (uint32_t *)&pHdr[10]; // OFFBITS
    if (bpp <= 8) {
        *l = iHeaderSize + 1024;
    } else { // no palette
        *l = iHeaderSize;
    }
   fwrite(pHdr, 1, iHeaderSize, oHandle);
    if (bpp <= 8) {
    if (pPalette == NULL) {// create a grayscale palette
        int iDelta, iCount = 1<<bpp;
        int iGray = 0;
        iDelta = 255/(iCount-1);
        for (i=0; i<iCount; i++) {
            ucTemp[i*4+0] = (uint8_t)iGray;
            ucTemp[i*4+1] = (uint8_t)iGray;
            ucTemp[i*4+2] = (uint8_t)iGray;
            ucTemp[i*4+3] = 0;
            iGray += iDelta;
        }
    } else {
        for (i=0; i<256; i++) // change palette to WinBMP format
        {
            ucTemp[i*4 + 0] = pPalette[(i*3)+2];
            ucTemp[i*4 + 1] = pPalette[(i*3)+1];
            ucTemp[i*4 + 2] = pPalette[(i*3)+0];
            ucTemp[i*4 + 3] = 0;
        }
    }
    fwrite(ucTemp, 1, 1024, oHandle);
    } // palette write
   /* Write the image data */
   for (i=cy-1; i>=0; i--)
    {
        s = &pBitmap[i*iDestPitch];
        if (bpp == 24) { // swap R/B for Windows BMP byte order
            int j, iBpp = bpp/8;
            uint8_t *d = ucTemp;
            for (j=0; j<cx; j++) {
                d[0] = s[2]; d[1] = s[1]; d[2] = s[0];
                d += iBpp; s += iBpp;
            }
            fwrite(ucTemp, 1, (size_t)lsize, oHandle);
        } else {
            fwrite(s, 1, (size_t)lsize, oHandle);
        }
    }
    free(ucTemp);
    fclose(oHandle);
} /* WriteBMP() */

long micros(void)
{
long iTime;
struct timespec res;

    clock_gettime(CLOCK_MONOTONIC, &res);
    iTime = 1000000*res.tv_sec + res.tv_nsec/1000;

    return iTime;
} /* micros() */

int ConvertFileTest(char *argv[], int iFraction)
{
    int i, rc;
    int cx, cy, bpp;
    int iOption;
    long lTime;
    
    if (iFraction == 2) {
       iOption = JPEG_SCALE_HALF;
    } else if (iFraction == 4) {
       iOption = JPEG_SCALE_QUARTER;
    } else if (iFraction == 8) {
       iOption = JPEG_SCALE_EIGHTH;
    } else {
       iOption = 0; // full size
    }

    ucPixelType = RGB565_LITTLE_ENDIAN; //RGB8888; // generate 32-bit pixels for a Windows BMP file
    rc = JPEG_openFile(&jpg, argv[1], NULL);
    if (!rc) {
        printf("Error opening file %s\n", argv[1]);
        return -1;
    }
    cx = jpg.iWidth / iFraction;
    cy = jpg.iHeight / iFraction;
    cx = (cx + 7) & 0xfff8; // align on at least 16-byte boundary
    if (ucPixelType == RGB8888) {
        iDestPitch = (cx * 4); // 32-bits per pixel
        bpp = 32;
    } else {
        iDestPitch = cx * 2; // 16-bpp
        bpp = 16;
    }
    i = iDestPitch * (cy+15);
    pFrame = (uint8_t *)malloc(i);
    if (pFrame == NULL) {
        printf("Error allocating %d bytes\n", i);
        return -1;
    }
    JPEG_setFramebuffer(&jpg, pFrame);
    jpg.ucPixelType = ucPixelType;
    lTime = micros();
    printf("Converting %d x %d image, fraction = %d\n", cx, cy, iFraction);
    if (JPEG_decode(&jpg, 0, 0, iOption)) {
        lTime = micros() - lTime;
        printf("JPEG decoded in %d us\n", (int)lTime);
        WriteBMP(argv[2], pFrame, NULL, cx, cy, bpp, iDestPitch);
    } else {
        printf("Decode failed, last error = %s\n", szErrors[JPEG_getLastError(&jpg)]);
        return -1;
    }
    JPEG_close(&jpg);
    return 0;
} /* ConvertFileTest() */

int JPEGDraw(JPEGDRAW *pDraw)
{
    (void)pDraw;
    return 1;
}

int PerfTest(int argc, char *argv[])
{
    int rc;
    long lTime;

    // first test full size decoding
    if (argc == 1) {
        rc = JPEG_openRAM(&jpg, (uint8_t *)tulips, sizeof(tulips), JPEGDraw);
    } else {
        rc = JPEG_openFile(&jpg, argv[1], JPEGDraw);
    }
    if (rc)
    {
        lTime = micros();
//        jpg.pDitherBuffer = ucDitherBuffer; // not used in this test
        jpg.ucPixelType = ucPixelType;
        if (JPEG_decode(&jpg, 0, 0, 0)) { // full size
            lTime = micros() - lTime;
            printf("full sized decode in %d us\n", (int)lTime);
        } else {
            printf("Decode failed, last error = %d\n", JPEG_getLastError(&jpg));
        return 0;
        }
        JPEG_close(&jpg);
    } else {
        printf("open() failed, last error = %d\n", JPEG_getLastError(&jpg));
        return -1;
    }
// Half size scaled test
    if (argc == 2)
        rc = JPEG_openFile(&jpg, argv[1], JPEGDraw);
    else
        rc = JPEG_openRAM(&jpg, (uint8_t *)tulips, sizeof(tulips), JPEGDraw);
    if (rc)
    {
        lTime = micros();
        if (JPEG_decode(&jpg, 0, 0, JPEG_SCALE_HALF)) { // 1/2 size
            lTime = micros() - lTime;
            printf("half sized decode in %d us\n", (int)lTime);
        }
        JPEG_close(&jpg);
    }
    // Quarter size scaled test
    if (argc == 2)
        rc = JPEG_openFile(&jpg, argv[1], JPEGDraw);
    else
        rc = JPEG_openRAM(&jpg, (uint8_t *)tulips, sizeof(tulips), JPEGDraw);
    if (rc)
    {
        lTime = micros();
        if (JPEG_decode(&jpg, 0, 0, JPEG_SCALE_QUARTER)) { // 1/4 size
            lTime = micros() - lTime;
            printf("quarter sized decode in %d us\n", (int)lTime);
        }
        JPEG_close(&jpg);
    }
    // Eighth size scaled test
    if (argc == 2)
        rc = JPEG_openFile(&jpg, argv[1], JPEGDraw);
    else
        rc = JPEG_openRAM(&jpg, (uint8_t *)tulips, sizeof(tulips), JPEGDraw);
    if (rc)
    {
        lTime = micros();
        if (JPEG_decode(&jpg, 0, 0, JPEG_SCALE_EIGHTH)) { // 1/8 size
            lTime = micros() - lTime;
            printf("eighth sized decode in %d us\n", (int)lTime);
        }
        JPEG_close(&jpg);
    }
    return 0; // done
} /* PerfTest() */

int main(int argc, char *argv[])
{
int rc = -1;

    printf("JPEG decoder demo\n");
    printf("options:\n");
    printf("Run without parameters to test in-memory decoding\n");
    printf("Pass a single filename to test decoding performance\n");
    printf("Pass 2 filenames to convert a JPEG file to a BMP file\n");
    printf("e.g. ./jpegdec <infile.jpg> <outfile.bmp>\n\n");

    if (argc == 1 || argc == 2) {
        rc = PerfTest(argc, argv);
    } else if (argc == 3) {
        rc = ConvertFileTest(argv, 1); // change the 1 to 2/4/8 for fractional decode
    } else {
        printf("Invalid number of parameters (re-read the above)\n");
        return rc;
    }
    return rc;
} /* main() */
