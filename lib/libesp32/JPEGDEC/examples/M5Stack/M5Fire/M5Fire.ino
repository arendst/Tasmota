// JPEGDEC example for Adafruit GFX displays

#include "JPEGDEC.h"
// Sample image (truncated) containing a 320x240 Exif thumbnail
// if an image doesn't display, it may have been created using progressive, open gimp, export as a jpg, remove progressive.

#include "thumb_test.h"
//#include "ncc1701.h" //http://clipart-library.com/clip-art/uss-enterprise-silhouette-11.htm
//#include "batman.h" //http://clipart-library.com/clipart/batman-clip-art_19.htm

#include <M5Stack.h>

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240


JPEGDEC jpeg;

int JPEGDraw(JPEGDRAW *pDraw)
{

  M5.Lcd.drawBitmap((int16_t)pDraw->x, (int16_t)pDraw->y, (int16_t)pDraw->iWidth, (int16_t)pDraw->iHeight, pDraw->pPixels);
  return 1;
} /* JPEGDraw() */

void setup() {
  Serial.begin(115200);
  
  Serial.println("Starting...");
  
M5.Lcd.begin();
M5.Lcd.fillScreen(TFT_BLACK);
} /* setup() */

void loop() {
int i;
long lTime;
int iOption[4] = {0, JPEG_SCALE_HALF, JPEG_SCALE_QUARTER, JPEG_SCALE_EIGHTH};
int iCenterX[4] = {0,80,120,140};
int iCenterY[4] = {0,60,90,105};

  for (i=0; i<4; i++)
  {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.startWrite(); // Not sharing TFT bus on PyPortal, just CS once and leave it
    if (jpeg.openFLASH((uint8_t *)thumb_test, sizeof(thumb_test), JPEGDraw))
    //if (jpeg.openFLASH((uint8_t *)ncc1701, sizeof(ncc1701), JPEGDraw))
    //if (jpeg.openFLASH((uint8_t *)batman, sizeof(batman), JPEGDraw))
    
    {
      lTime = micros();
      if (jpeg.hasThumb()) {jpeg.decode(iCenterX[i],iCenterY[i],JPEG_EXIF_THUMBNAIL | iOption[i]); } else { jpeg.decode(iCenterX[i],iCenterY[i], iOption[i]); }
            {
        lTime = micros() - lTime;
        Serial.printf("%d x %d image, decode time = %d us\n", jpeg.getWidth() >> i, jpeg.getHeight() >> i, (int)lTime);
      }
      jpeg.close();
    }
    delay(2000); // pause between images
  } // for i
} /* loop() */
