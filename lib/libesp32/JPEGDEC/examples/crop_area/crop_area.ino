//
// Crop Area Example
// Written by Larry Bank (bitbank@pobox.com)
//
// This sketch demonstrates how to use the optimized crop feature of JPEGDEC
// The library supports cropping JPEG images during the decode process. The
// unused parts of the image are not decoded, so this reduces the decode time.
// The only limitation is that images must be cropped on MCU (minimum coded unit)
// boundaries. The setCropArea() function takes care of this for you if you
// provide coordinates that don't line up, they will be adjusted. The reason
// this is needed is because the nature of JPEG images is that they're composed
// of 8x8 pixel blocks. When color subsampling is enabled, these MCUs can also
// be 16x8, 8x16 or 16x16 pixels. To reduce the confusion when cropping images,
// the library will return the adjust area to know exactly what you're getting.
// The JPEGDraw() callback function will only be passed the cropped image, not
// the full image size.
//
#include <bb_spi_lcd.h>
#include "JPEGDEC.h"
#include "croptest.h"
BB_SPI_LCD lcd;
JPEGDEC jpeg;
//
// Pixel drawing callback
// called once for each set of MCUs (minimum coded units).
// JPEGDEC will try to send as many pixels as it can per call.
// In this case, it's as many as can fit in
// the internal 4K pixel buffer. This allows it to run more
// efficiently than calling this for every MCU. The blocks of pixels
// will typically be 128x8 or 128x16.
//
int drawMCUs(JPEGDRAW *pDraw)
{
  int iPixelCount;
  iPixelCount = pDraw->iWidth * pDraw->iHeight; // number of pixels to draw in this call
  lcd.setAddrWindow(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  lcd.pushPixels(pDraw->pPixels, iPixelCount, DRAW_TO_LCD);
  return 1; // returning true (1) tells JPEGDEC to continue decoding. Returning false (0) would quit decoding immediately.
} /* drawMCUs() */

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}; // CDC-Serial takes a few seconds to start
  lcd.begin(DISPLAY_WS_AMOLED_18); // 1.8" Waveshare AMOLED 368x448
  Serial.printf("LCD size = %dx%d\n", lcd.width(), lcd.height());
  lcd.setFont(FONT_12x16);
  lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  Serial.println("Starting...");
} /* loop() */

void loop()
{
long lTime;
char szTemp[128];
int x, y, cx, cy;
int iXOff, iYOff; // used for centering the image

  lcd.fillScreen(TFT_BLACK);
  lcd.println("JPEG Crop Example");
  lcd.println("Full image decode:");
  if (jpeg.openFLASH((uint8_t *)croptest, sizeof(croptest), drawMCUs)) {
    Serial.println("Successfully opened JPEG image");
    Serial.printf("Image size: %d x %d, orientation: %d, bpp %d, sub %02x\n", jpeg.getWidth(),
      jpeg.getHeight(), jpeg.getOrientation(), jpeg.getBpp(), jpeg.getSubSample());
    jpeg.setPixelType(RGB565_BIG_ENDIAN); // The LCD wants the 16-bit pixels in big-endian order
    // center the image on the display
    iXOff = (lcd.width() - jpeg.getWidth())/2;
    if (iXOff < 0) iXOff = 0;
    iYOff = (lcd.height() - jpeg.getHeight())/2;
    if (iYOff < 0) iYOff = 0;
    lTime = millis();
    // Decode and draw the image on the LCD
    if (jpeg.decode(iXOff,iYOff, 0)) {
      lTime = millis() - lTime;
      sprintf(szTemp, "Successfully decoded image in %d ms", (int)lTime);
      Serial.println(szTemp);
      lcd.setCursor(0, lcd.height() - 16);
      lcd.printf("Decode+display: %d ms", (int)lTime);
    }
    jpeg.close();
  } else {
    Serial.println("Failed to open image");
  }
  delay(4000); // allow user to see the results
  // Now decode a cropped image
  lcd.fillScreen(TFT_BLACK);
  lcd.println("JPEG Crop Example");
  lcd.println("Cropped image decode:");
  if (jpeg.openFLASH((uint8_t *)croptest, sizeof(croptest), drawMCUs)) {
    jpeg.setPixelType(RGB565_BIG_ENDIAN); // The LCD wants the 16-bit pixels in big-endian order
    jpeg.setCropArea(120, 65, 119, 110); // requested area
    lcd.printf("Requested: (120,65,119,110)\n");
    jpeg.getCropArea(&x, &y, &cx, &cy);
    lcd.printf("Actual: (%d,%d,%d,%d)\n", x, y, cx, cy);
    // Center the image on the display.
    // getWidth() will always return the full size
    // so we need to use the returned crop area as the new drawing size
    iXOff = (lcd.width() - cx)/2;
    if (iXOff < 0) iXOff = 0;
    iYOff = (lcd.height() - cy)/2;
    if (iYOff < 0) iYOff = 0;
    lTime = millis();
    // Decode and draw the image on the LCD
    if (jpeg.decode(iXOff,iYOff, 0)) {
      lTime = millis() - lTime;
      sprintf(szTemp, "Successfully decoded image in %d ms", (int)lTime);
      Serial.println(szTemp);
      lcd.setCursor(0, lcd.height() - 16);
      lcd.printf("Decode+display: %d ms", (int)lTime);
    }
    jpeg.close();
  } else {
    Serial.println("Failed to open image");
  }
  delay(10000);
} /* loop() */
