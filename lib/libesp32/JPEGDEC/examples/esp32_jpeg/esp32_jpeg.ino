//
// JPEG thumbnail decode test
//
// This example sketch decodes an EXIF thumbnail image
// embedded in a 12 megapixel photo taken on a mobile phone
//
// It's written to run on an ESP32 connected to a ILI9341 LCD
// although it can certainly be compiled on other target MCUs and displays
//
#include <bb_spi_lcd.h>
#include "JPEGDEC.h"
#include "../test_images/thumb_test.h"

// These pin definitions are for a custom ESP32
// board. Please change them to match the display
// and board you're using

#define CS_PIN 4
#define DC_PIN 12
#define LED_PIN 16
#define RESET_PIN -1
#define MISO_PIN 19
#define MOSI_PIN 23
#define SCK_PIN 18

// Static instance of the JPEGDEC structure. It requires about
// 17.5K of RAM. You can allocate it dynamically too. Internally it
// does not allocate or free any memory; all memory management decisions
// are left to you
JPEGDEC jpeg;

// The LCD display library instance
SPILCD lcd;
//
// Pixel drawing callback
// called once for each set of MCUs (minimum coded units).
// JPEGDEC will try to send as many pixels as it can per call.
// In this case, it's as many as can fit in
// the internal 4K pixel buffer. This allows it to run more
// efficiently than calling this for every MCU. For this demo, the
// MCUs are only 4x4 pixels each since we ask to decode the image
// at 1/4 size
//
int drawMCUs(JPEGDRAW *pDraw)
{
  int iCount;
  iCount = pDraw->iWidth * pDraw->iHeight; // number of pixels to draw in this call
//  Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  spilcdSetPosition(&lcd, pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, DRAW_TO_LCD);
  spilcdWriteDataBlock(&lcd, (uint8_t *)pDraw->pPixels, iCount*2, DRAW_TO_LCD | DRAW_WITH_DMA);
  return 1; // returning true (1) tells JPEGDEC to continue decoding. Returning false (0) would quit decoding immediately.
} /* drawMCUs() */

void setup()
{
  Serial.begin(115200);
  while (!Serial) {};
  spilcdInit(&lcd, LCD_ILI9341, FLAGS_NONE, 40000000, CS_PIN, DC_PIN, RESET_PIN, LED_PIN, MISO_PIN, MOSI_PIN, SCK_PIN);
  spilcdSetOrientation(&lcd, LCD_ORIENTATION_90);
  spilcdFill(&lcd, 0, DRAW_TO_LCD); // erase display to black
  spilcdWriteString(&lcd, 46,0,(char *)"JPEG Thumbnail test", 0x7e0,0,FONT_12x16, DRAW_TO_LCD);
  delay(4000);
} /* setup() */

void loop() {
long lTime;
char szTemp[64];

  // Open a large JPEG image stored in FLASH memory (included as thumb_test.h)
  // This image is 12 megapixels, but has a 320x240 embedded thumbnail in it
  if (jpeg.openFLASH((uint8_t *)thumb_test, sizeof(thumb_test), drawMCUs))
  {
    Serial.println("Successfully opened JPEG image");
    Serial.printf("Image size: %d x %d, orientation: %d, bpp: %d\n", jpeg.getWidth(),
      jpeg.getHeight(), jpeg.getOrientation(), jpeg.getBpp());
    if (jpeg.hasThumb())
       Serial.printf("Thumbnail present: %d x %d\n", jpeg.getThumbWidth(), jpeg.getThumbHeight());
    jpeg.setPixelType(RGB565_BIG_ENDIAN); // The SPI LCD wants the 16-bit pixels in big-endian order
    lTime = micros();
    // Draw the thumbnail image in the middle of the display (upper left corner = 120,100) at 1/4 scale
    if (jpeg.decode(120,100,JPEG_SCALE_QUARTER | JPEG_EXIF_THUMBNAIL))
    {
      lTime = micros() - lTime;
      sprintf(szTemp, "Successfully decoded image in %d us", (int)lTime);
      Serial.println(szTemp);
      spilcdWriteString(&lcd, 0, 200, szTemp, 0xffe0, 0, FONT_8x8, DRAW_TO_LCD);
    }
    jpeg.close();
  }
  
  delay(10000); // repeat every 10 seconds
}
