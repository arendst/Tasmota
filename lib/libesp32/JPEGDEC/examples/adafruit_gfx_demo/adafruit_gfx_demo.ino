// JPEGDEC example for Adafruit GFX displays

#include <JPEGDEC.h>
// Sample image (truncated) containing a 320x240 Exif thumbnail
#include "thumb_test.h"

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

// PyPortal-specific pins
#define SD_CS         32 // SD card select
#define TFT_D0        34 // Data bit 0 pin (MUST be on PORT byte boundary)
#define TFT_WR        26 // Write-strobe pin (CCL-inverted timer output)
#define TFT_DC        10 // Data/command pin
#define TFT_CS        11 // Chip-select pin
#define TFT_RST       24 // Reset pin
#define TFT_RD         9 // Read-strobe pin
#define TFT_BACKLIGHT 25 // Backlight enable (active high)

Adafruit_ILI9341 tft(tft8bitbus, TFT_D0, TFT_WR, TFT_DC, TFT_CS, TFT_RST, TFT_RD);
JPEGDEC jpeg;

int JPEGDraw(JPEGDRAW *pDraw)
{
//  Serial.printf("jpeg draw: x,y=%d,%d, cx,cy = %d,%d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
//  Serial.printf("Pixel 0 = 0x%04x\n", pDraw->pPixels[0]);
  tft.dmaWait(); // Wait for prior writePixels() to finish
  tft.setAddrWindow(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  tft.writePixels(pDraw->pPixels, pDraw->iWidth * pDraw->iHeight, true, false); // Use DMA, big-endian
  return 1;
} /* JPEGDraw() */

void setup() {
  Serial.begin(115200);
  //while (!Serial);
  Serial.println("Starting...");
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on

  // put your setup code here, to run once:
  tft.begin();
  tft.setRotation(3); // PyPortal native orientation
} /* setup() */

void loop() {
int i;
long lTime;
int iOption[4] = {0, JPEG_SCALE_HALF, JPEG_SCALE_QUARTER, JPEG_SCALE_EIGHTH};
int iCenterX[4] = {0,80,120,140};
int iCenterY[4] = {0,60,90,105};

  for (i=0; i<4; i++)
  {
    tft.fillScreen(ILI9341_BLACK);
    tft.startWrite(); // Not sharing TFT bus on PyPortal, just CS once and leave it
    if (jpeg.openFLASH((uint8_t *)thumb_test, sizeof(thumb_test), JPEGDraw))
    {
      lTime = micros();
      if (jpeg.decode(iCenterX[i],iCenterY[i],JPEG_EXIF_THUMBNAIL | iOption[i]))
      {
        lTime = micros() - lTime;
        Serial.printf("%d x %d image, decode time = %d us\n", jpeg.getWidth() >> i, jpeg.getHeight() >> i, (int)lTime);
      }
      jpeg.close();
    }
    delay(2000); // pause between images
  } // for i
} /* loop() */
