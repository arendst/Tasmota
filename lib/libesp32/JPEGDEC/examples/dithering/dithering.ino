//
// Example to demonstrate the built-in dithering features of JPEGDEC
// Dithering is available for grayscale output (1/2/4-bits per pixel)
// This example makes use of my FastEPD library for parallel e-ink displays
// The target device is the M5Stack PaperS3, but any 1-bit or 4-bit grayscale display
// can be substituted
//
#include <JPEGDEC.h>
#include <FastEPD.h>
#include "../../test_images/tulips.h" // 640x480 color images
FASTEPD epaper;
JPEGDEC jpg;
//
// JPEGDraw callback function
// called periodically with blocks of multiple MCUs of pixels
// We're requesting 1 or 4-bit pixels, so with decodeDither()
// that's the pixel format that JPEGDEC supplies
//
// 4-bpp = (bits) aaaabbbb. a = top nibble, left pixel, b = bottom nibble, right pixel
// 0 = black, 0xf (15) = white
//
// 1-bpp = (bits) abcdefgh. a = MSB (0x80), leftmost pixel, h = LSB (0x01), rightmost pixel
// 0 = black, 1 = white
//
// This example function does not take into account rotation of the display buffer
// If you want to view the image rotated from the native orientation of the display
// you'll need to write the pixels in the rotated direction.
//
int JPEGDraw(JPEGDRAW *pDraw)
{
  int y, iPitch, shift;
  uint8_t *s, *d, *pBuffer = epaper.currentBuffer();
  if (jpg.getPixelType() == ONE_BIT_DITHERED) {
    shift = 3;
  } else {
    shift = 1;
  }
  iPitch = epaper.width() >> shift; // destination pitch of the display framebuffer
  for (y=0; y<pDraw->iHeight; y++) { // this is 8 or 16 depending on the color subsampling
   // This is the destination pointer into the correct spot (with centering)
   // in the FastEPD framebuffer
    d = &pBuffer[((pDraw->y + y)*iPitch) + (pDraw->x >> shift)];
    // This is the pointer to the source pixels provided by this JPEGDraw callback
    // each line advances pDraw->iWidth pixels into the memory
    s = (uint8_t *)pDraw->pPixels;
    s += (y * (pDraw->iWidth >> shift));
    // The pixel format of the display is the same as JPEGDEC, so just copy it
    memcpy(d, s, pDraw->iWidth >> shift);
  } // for y
  return 1; // continue decoding
} /* JPEGDraw() */

void setup()
{
  int rc, w, h, xoff, yoff;
  uint8_t *pDither;
  Serial.begin(115200);
  delay(3000); // wait for CDC-Serial to start
  // Initialize the ESP32-S3 I/O and allocate (PSRAM) for the framebuffer
  rc = epaper.initPanel(BB_PANEL_M5PAPERS3); // 960x540 parallel eink
  if (rc != BBEP_SUCCESS) { // something went wrong
    Serial.println("Error initializing eink display!");
    while (1) {}; // wait forever
  }
  epaper.fillScreen(BBEP_WHITE); // start in 1-bit mode
  epaper.setFont(FONT_12x16);
  epaper.setTextColor(BBEP_BLACK);
  epaper.setCursor(312, 6);
  epaper.print("1-bit Floyd Steinberg Dither");
  if (jpg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    // 1-bit dithered can fit in the existing JPEGIMAGE structure
      jpg.setPixelType(ONE_BIT_DITHERED); // request 1-bit dithered output
      w = jpg.getWidth();
      h = jpg.getHeight();
      // We need to allocate extra memory to do dithering because it needs the full
      // width of the image x 16 rows to do Floyd Steinberg's algorithm properly
      pDither = (uint8_t *)malloc(w * 16);
      // center the image on the display
      xoff = (epaper.width() - w)/2;
      yoff = (epaper.height() - h)/2;
      jpg.decodeDither(xoff, yoff, pDither, 0); // decode it and pass centering offsets
      jpg.close();
      free(pDither);
      epaper.fullUpdate(true); // display the 1-bpp dithered image 
  }
  delay(3000); // allow time to observe it
  // Now switch to 4-bpp grayscale mode and use 4-bpp dithering
  epaper.setMode(BB_MODE_4BPP);
  epaper.fillScreen(0xf); // 0-15 in grayscale mode, 0=black, 15 = white
  epaper.setCursor(312, 6);
  epaper.print("4-bit Floyd Steinberg Dither");
  // Decode the same image again
  if (jpg.openFLASH((uint8_t *)tulips, sizeof(tulips), JPEGDraw)) {
    // 1-bit dithered can fit in the existing JPEGIMAGE structure
      jpg.setPixelType(FOUR_BIT_DITHERED); // request 4-bit dithered output
      w = jpg.getWidth();
      h = jpg.getHeight();
      // We need to allocate extra memory to do dithering because it needs the full
      // width of the image x 16 rows to do Floyd Steinberg's algorithm properly
      // Neighboring pixels are used to spread the color errors and if we limit it
      // to the JPEG MCU blocks, visible lines will form on block boundaries
      pDither = (uint8_t *)malloc(w * 16);
      // center the image on the display
      xoff = (epaper.width() - w)/2;
      yoff = (epaper.height() - h)/2;
      jpg.decodeDither(xoff, yoff, pDither, 0); // decode it and pass centering offsets
      jpg.close();
      free(pDither);
      epaper.fullUpdate(true); // display the 1-bpp dithered image 
  }
  // done, program will end
} /* setup() */

void loop()
{
  // nothing needed here
} /* loop() */

