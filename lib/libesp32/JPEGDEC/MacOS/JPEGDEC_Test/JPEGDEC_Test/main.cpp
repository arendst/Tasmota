//
//  main.cpp
//  JPEGDEC_Test
//
//  Created by Laurence Bank on 2/6/25.
//

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "../../../src/JPEGDEC.cpp"
#include "../../../test_images/tulips.h" // 640x480 56k byte test image
#include "../../../test_images/thumb_test.h" // thumbnail extraction
#include "corrupt1.h" // invalid header offsets
#include "corrupt2.h" // global buffer overflow 1
#include "corrupt3.h" // global buffer overflow 2
#include "corrupt4.h" // FPE 1
#include "corrupt5.h" // FPE 2
#define LOOP_COUNT 100

JPEGDEC jpg;
int x1, y1, x2, y2;
int iWidth, iHeight;
uint16_t *pOldPixels;
int bDMAFailed;
int iTotal = 0;
int iTotalFail = 0, iTotalPass = 0;
//
// Return the current time in microseconds
//
int Micros(void)
{
int iTime;
struct timespec res;
                        
    clock_gettime(CLOCK_MONOTONIC, &res);
    iTime = (int)(1000000*res.tv_sec + res.tv_nsec/1000);
                
    return iTime;
} /* Micros() */

//
// Simple logging print
//
void JPEGLOG(int line, char *string, const char *result)
{
    printf("Line: %d: msg: %s%s\n", line, string, result);
} /* JPEGLOG() */

// Draw callback
int JPEGDraw(JPEGDRAW *pDraw)
{
    if (pDraw->pPixels == pOldPixels) {
        bDMAFailed = 1; // DMA option should toggle the buffer pointer with each callback
    }
    pOldPixels = pDraw->pPixels;
    
    // record the max extents of the pixel positions
    if (pDraw->x < x1) x1 = pDraw->x;
    if (pDraw->y < y1) y1 = pDraw->y;
    if (pDraw->x + pDraw->iWidthUsed -1 > x2) x2 = pDraw->x + pDraw->iWidthUsed -1;
    if (pDraw->y + pDraw->iHeight-1 > y2) y2 = pDraw->y + pDraw->iHeight-1;
    return 1; // continue to decode
} /* JPEGDraw() */

int main(int argc, const char * argv[]) {
    int i, rc, iTime1, iTime2;
    int w, h;
    uint8_t *pFuzzData;
    char *szTestName;
    const char *szStart = " - START";
    // Test 1 - Decode to the correct full image dimensions
    x1 = y1 = 1000;
    x2 = y2 = 0;
    szTestName = (char *)"JPEG full image decode";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    if (jpg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
        if (jpg.decode(0,0,0)) { // full sized decode
            iWidth = jpg.getWidth();
            iHeight = jpg.getHeight();
            w = 1 + x2 - x1; h = 1 + y2 - y1;
            if (iHeight == h && iWidth == w) {
                JPEGLOG(__LINE__, szTestName, " - PASSED");
                iTotalPass++;
            } else {
                iTotalFail++;
                JPEGLOG(__LINE__, szTestName, " - FAILED");
                if (iHeight != h) {
                    printf("Image Height = %d, decoded Height = %d\n", iHeight, h);
                } else {
                    printf("Image Width = %d, decoded Width = %d\n", iWidth, w);
                }
            }
        } else { // decode failed
            iTotalFail++;
            JPEGLOG(__LINE__, szTestName, " - decode failed");
        }
        jpg.close();
    } else {
        iTotalFail++;
        JPEGLOG(__LINE__, szTestName, " - open failed");
    }
    // Test 2 - Decode to the correct cropped dimensions
    szTestName = (char *)"JPEG full image decode";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    if (jpg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
        jpg.setCropArea(50, 50, 125, 170); // purposely using coordinates which will get adjusted
        jpg.getCropArea(&x1, &y1, &iWidth, &iHeight);
        x1 = y1 = 1000;
        x2 = y2 = 0;
        if (jpg.decode(0,0,0)) { // cropped decode
            w = 1 + x2 - x1; h = 1 + y2 - y1;
            if (iHeight == h && iWidth == w) {
                JPEGLOG(__LINE__, szTestName, " - PASSED");
                iTotalPass++;
            } else {
                iTotalFail++;
                JPEGLOG(__LINE__, szTestName, " - FAILED");
                if (iHeight != h) {
                    printf("Image Height = %d, decoded Height = %d\n", iHeight, h);
                } else {
                    printf("Image Width = %d, decoded Width = %d\n", iWidth, w);
                }
            }
        } else { // decode failed
            iTotalFail++;
            JPEGLOG(__LINE__, szTestName, " - decode failed");
        }
        jpg.close();
    } else {
        iTotalFail++;
        JPEGLOG(__LINE__, szTestName, " - open failed");
    }
    // Test 3 - Decode a color image as grayscale (faster)
    szTestName = (char *)"JPEG color->gray image decode";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    iTime1 = Micros();
    for (i=0; i<LOOP_COUNT; i++) { // need to do it many times to get an accurate time
        jpg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw);
        jpg.decode(0,0,JPEG_LUMA_ONLY); // grayscale decode
        jpg.close();
    }
    iTime1 = Micros() - iTime1; // total decode time in microseconds
    iTime2 = Micros();
    for (i=0; i<LOOP_COUNT; i++) {
        jpg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw);
        jpg.decode(0,0,0);
        jpg.close();
    }
    iTime2 = Micros() - iTime2;
    printf("%d iterations: color decode - %d us, grayscale decode - %d us\n", LOOP_COUNT, iTime2, iTime1);
    if (iTime1 <= (iTime2 * 5)/8) { // it should be at least 40% faster
        JPEGLOG(__LINE__, szTestName, " - PASSED");
        iTotalPass++;
    } else {
        iTotalFail++;
        JPEGLOG(__LINE__, szTestName, " - FAILED");
    }
    // Test 4 - open a corrupt image without crashing
    szTestName = (char *)"JPEG purposely corrupt image 1";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    if (jpg.openFLASH((uint8_t *)corrupt1, sizeof(corrupt1), JPEGDraw)) {
        jpg.decode(0,0,0);
        jpg.close();
    }
    JPEGLOG(__LINE__, szTestName, " - PASSED");
    iTotalPass++;
    // Test 5 - open a corrupt image without crashing
    szTestName = (char *)"JPEG purposely corrupt image 2";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    if (jpg.openFLASH((uint8_t *)corrupt2, sizeof(corrupt2), JPEGDraw)) {
        jpg.decode(0,0,0);
        jpg.close();
    }
    JPEGLOG(__LINE__, szTestName, " - PASSED");
    iTotalPass++;

    // Test 6 - open a corrupt image without crashing
    szTestName = (char *)"JPEG purposely corrupt image 3";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    if (jpg.openFLASH((uint8_t *)corrupt3, sizeof(corrupt3), JPEGDraw)) {
        jpg.decode(0,0,0);
        jpg.close();
    }
    JPEGLOG(__LINE__, szTestName, " - PASSED");
    iTotalPass++;
    
    // Test 7 - open a corrupt image without crashing
    szTestName = (char *)"JPEG purposely corrupt image 4";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    if (jpg.openFLASH((uint8_t *)FPE1, sizeof(FPE1), JPEGDraw)) {
        jpg.decode(0,0,0);
        jpg.close();
    }
    JPEGLOG(__LINE__, szTestName, " - PASSED");
    iTotalPass++;
    
    // Test 8 - open a corrupt image without crashing
    szTestName = (char *)"JPEG purposely corrupt image 5";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    if (jpg.openFLASH((uint8_t *)FPE2, sizeof(FPE2), JPEGDraw)) {
        jpg.decode(0,0,0);
        jpg.close();
    }
    JPEGLOG(__LINE__, szTestName, " - PASSED");
    iTotalPass++;
    
    // Test 9 - confirm DMA option is properly providing a ping-pong buffer
    bDMAFailed = 0;
    pOldPixels = NULL;
    szTestName = (char *)"JPEG DMA ping-pong buffer";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    if (jpg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
        jpg.decode(0,0,JPEG_USES_DMA);
        jpg.close();
    }
    if (!bDMAFailed) {
        JPEGLOG(__LINE__, szTestName, " - PASSED");
        iTotalPass++;
    } else {
        iTotalFail++;
        JPEGLOG(__LINE__, szTestName, " - FAILED");
    }
    
    // Test 10 - confirm correct extraction of EXIF thumbnail
    szTestName = (char *)"JPEG EXIF Thumbnail";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    if (jpg.openFLASH((uint8_t *)thumb_test, sizeof(thumb_test), JPEGDraw)) {
        if (jpg.hasThumb()) {
            jpg.decode(0,0,JPEG_EXIF_THUMBNAIL);
            jpg.close();
            if (jpg.getWidth() != 320 || jpg.getHeight() != 240) {
                iTotalFail++;
                JPEGLOG(__LINE__, szTestName, " - FAILED");
                printf("Thumbnail not decoded successfully\n");
            } else {
                iTotalPass++;
                JPEGLOG(__LINE__, szTestName, " - PASSED");
            }
        } else {
            iTotalFail++;
            JPEGLOG(__LINE__, szTestName, " - FAILED");
            printf("Thumbnail not detected\n");
        }
    } else {
        iTotalFail++;
        JPEGLOG(__LINE__, szTestName, " - open failed");
    }

    // FUZZ testing
    // Randomize the input data (file header and compressed data) and confirm that the library returns an error code
    // and doesn't have an invalid pointer exception
    printf("Begin fuzz testing...\n");
    szTestName = (char *)"Single Byte Sequential Corruption Test";
    iTotal++;
    pFuzzData = (uint8_t *)malloc(sizeof(tulips));
    JPEGLOG(__LINE__, szTestName, szStart);
    // We don't need to corrupt the file all the way to the end because it will take a loooong time
    // The header is the main area where corruption can cause erratic behavior
    for (i=0; i<2000; i++) { // corrupt each byte one at a time by inverting it
        memcpy(pFuzzData, tulips, sizeof(tulips)); // start with the valid data
        pFuzzData[i] = ~pFuzzData[i]; // invert the bits of this byte
        if (jpg.openFLASH(pFuzzData, sizeof(tulips), JPEGDraw)) { // the JPEG header may be rejected
            rc = jpg.decode(0,0,0);
            jpg.close();
        }
    } // for each test
    JPEGLOG(__LINE__, szTestName, " - PASSED");
    iTotalPass++;
    
    // Fuzz test part 2 - multi-byte random corruption
    szTestName = (char *)"Multi-Byte Random Corruption Test";
    iTotal++;
    JPEGLOG(__LINE__, szTestName, szStart);
    for (i=0; i<1000; i++) { // 1000 iterations of random spots in the file to corrupt with random values
        int iOffset;
        memcpy(pFuzzData, tulips, sizeof(tulips)); // start with the valid data
        iOffset = rand() % sizeof(tulips);
        pFuzzData[iOffset] = (uint8_t)rand();
        iOffset = rand() % sizeof(tulips); // corrupt 2 spots just for good measure
        pFuzzData[iOffset] = (uint8_t)rand();
        if (jpg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) { // the JPEG header may be rejected
            rc = jpg.decode(0,0,0);
            jpg.close();
        }
    } // for each test
    JPEGLOG(__LINE__, szTestName, " - PASSED");
    iTotalPass++;
    
    free(pFuzzData);
    printf("Total tests: %d, %d passed, %d failed\n", iTotal, iTotalPass, iTotalFail);
    return 0;
}
