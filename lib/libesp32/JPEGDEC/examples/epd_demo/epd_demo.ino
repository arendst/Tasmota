// JPEGDEC example for 4.2" monochrome E-Paper displays
// Does not work with tri-color 4.2" displays!

#include <JPEGDEC.h>
#include "SPI.h"
#include "Adafruit_GFX.h"

#include "Grayscale_IL0398.h"
#include "lange.h" // Demo photographs, grayscale, sized 300x400

#define DISPLAY_WIDTH 300
#define DISPLAY_HEIGHT 400

// set these values to match the pins and the SPI bus for your display
#define EPD_CS 44
#define EPD_DC 45
#define EPD_RST 46
#define EPD_BSY 47
#define EPD_DISPLAY_BUS &SPI1

// adds a basic Floyd-Steinberg dither to each block of pixels rendered.
bool dither = true;

JPEGDEC jpeg;
Grayscale_IL0398 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, EPD_DC, EPD_RST, EPD_CS, -1, EPD_BSY, EPD_DISPLAY_BUS);

int JPEGDraw(JPEGDRAW *pDraw)
{
  int x = pDraw->x;
  int y = pDraw->y;
  int w = pDraw->iWidth;
  int h = pDraw->iHeight;

  for(int i = 0; i < w * h; i++)
  {
    pDraw->pPixels[i] = (pDraw->pPixels[i] & 0x7e0) >> 5; // extract just the six green channel bits.
  }

  if (dither)
  {
    for(int16_t j = 0; j < h; j++)
    {
      for(int16_t i = 0; i < w; i++)
      {
        int8_t oldPixel = constrain(pDraw->pPixels[i + j * w], 0, 0x3F);
        int8_t newPixel = oldPixel & 0x38; // or 0x30 to dither to 2-bit directly. much improved tonal range, but more horizontal banding between blocks.
        pDraw->pPixels[i + j * w] = newPixel;
        int quantError = oldPixel - newPixel;      
        if(i + 1 < w) pDraw->pPixels[i + 1 + j * w] += quantError * 7 / 16;
        if((i - 1 >= 0) && (j + 1 < h)) pDraw->pPixels[i - 1 + (j + 1) * w] += quantError * 3 / 16;
        if(j + 1 < h) pDraw->pPixels[i + (j + 1) * w] += quantError * 5 / 16;
        if((i + 1 < w) && (j + 1 < h)) pDraw->pPixels[i + 1 + (j + 1) * w] += quantError * 1 / 16;
      } // for i
    } // for j
  } // if dither
  
  for(int16_t i = 0; i < w; i++)
  {
    for(int16_t j = 0; j < h; j++)
    {
      switch (constrain(pDraw->pPixels[i + j * w] >> 4, 0, 3))
      {
        case 0:
          display.writePixel(x+i, y+j, EPD_BLACK);
          break;
        case 1:
          display.writePixel(x+i, y+j, EPD_DARK);
          break;
        case 2:
          display.writePixel(x+i, y+j, EPD_LIGHT);
          break;
        case 3:
          display.writePixel(x+i, y+j, EPD_WHITE);
          break;
      } // switch
    } // for j
  } // for i
  return 1;
} /* JPEGDraw() */

void setup() {
  Serial.begin(115200);
  //while (!Serial) delay(1000);
  Serial.println("Starting...");
  display.begin();
  display.fillRect(0, 0, 300, 400, EPD_BLACK);
  display.display();
} /* setup() */

const uint8_t * images[] = {demo_001_jpg, demo_002_jpg, demo_003_jpg, demo_004_jpg, demo_005_jpg, demo_006_jpg, demo_007_jpg, demo_008_jpg};
size_t imageLengths[] = {demo_001_jpg_len, demo_002_jpg_len, demo_003_jpg_len, demo_004_jpg_len, demo_005_jpg_len, demo_006_jpg_len, demo_007_jpg_len, demo_008_jpg_len};

void loop() {
  int i;
  long lTime;

  for (i=0; i<8; i++)
  {
    if (jpeg.openFLASH((uint8_t *)images[i], imageLengths[i], JPEGDraw))
    {
      lTime = micros();
      if (jpeg.decode((300 - jpeg.getWidth()) / 2, (400 - jpeg.getHeight()) / 2, 0))
      {
        lTime = micros() - lTime;
        Serial.printf("%d x %d image, decode time = %d us\n", jpeg.getWidth(), jpeg.getHeight(), (int)lTime);
      }
      jpeg.close();
    }
    display.display();
    delay(4000); // pause between images
  } // for i
} /* loop() */
