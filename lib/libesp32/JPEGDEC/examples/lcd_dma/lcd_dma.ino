//
// LCD w/DMA example showing best practices
//
// DMA = Direct Memory Access
// DMA is extra hardware within a MCU which can move data independently of the CPU.
// This feature can be used to reduce execution time by allowing the CPU to get back
// to doing the desired work while data is read or written to devices such as displays.
// For JPEG image decoding, DMA can allow pixels to be sent to the display while the
// decoder is busy decoding the next set of pixels. The speed advantage gained from
// DMA varies depending on how fast the data can be sent to the external device.
// In the best case scenario, the transmission of the data takes less time than
// preparation of the data - DMA will effectively 'erase' the transmission time
// because the CPU will be kept busy doing the actual work 100% of the time.
// DMA operations like those of image decoding benefit from having two buffers - 
// one is where new data is prepared and the other is for transmitting that data.
// If you use a single buffer, you run the risk of new data overwriting old data
// before it has been transmitted.
//
// JPEGDEC DMA Feature
// The JPEGDEC library includes internal support for managing a ping-pong buffer
// scheme to allow your code to simply use the pixel data without worrying about it
// getting overwritten. The JPEG_USES_DMA flag, when passed to the decode() method,
// tells JPEGDEC to split the internal pixel buffer in half and alternate using the
// two halves with each call to JPEGDraw.
//
#include <bb_spi_lcd.h>
#include <JPEGDEC.h>
#include "../../test_images/zebra.h"

JPEGDEC jpg;
BB_SPI_LCD lcd;
bool bDMA; // a flag for our JPEGDraw function to know if DMA should be enabled
//
// Draw callback from JPEG decoder
//
// Called multiple times with groups of MCUs (minimum coded units)
// these are 8x8, 8x16, 16x8 or 16x16 blocks of pixels depending on the
// color subsampling option of the JPEG image
// Each call can have a large group (e.g. 128x16) of pixels
//
// For this example, we set a global boolean value (bDMA) to indicate whether or not
// we should ask the display library to transmit the pixels using DMA
// JPEGDEC manages the ping-pong buffers, so we don't have to.
//
int JPEGDraw(JPEGDRAW *pDraw)
{
  lcd.setAddrWindow(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  lcd.pushPixels((uint16_t *)pDraw->pPixels, pDraw->iWidth * pDraw->iHeight, (bDMA) ? DRAW_TO_LCD | DRAW_WITH_DMA : DRAW_TO_LCD);
  return 1;
} /* JPEGDraw() */

void setup()
{
  int xoff, yoff;
  long lTime;

  lcd.begin(DISPLAY_WS_AMOLED_18); // set this to the correct display type for your board
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  lcd.setFont(FONT_12x16);
  lcd.setCursor((lcd.width() - 192)/2, 0);
  lcd.println("JPEG DMA Example");
  lcd.setCursor((lcd.width() - 264)/2, 16);
  lcd.println("Decode + center on LCD");
  bDMA = false;
  if (jpg.openFLASH((uint8_t *)zebra, sizeof(zebra), JPEGDraw)) { // pass the data and its size
      jpg.setPixelType(RGB565_BIG_ENDIAN); // bb_spi_lcd uses big-endian RGB565 pixels
      // if the image is smaller than the LCD dimensions, center it
      xoff = (lcd.width() - jpg.getWidth())/2;
      yoff = (lcd.height() - jpg.getHeight())/2;
      lTime = millis();
      jpg.decode(xoff,yoff,0); // center the image and no options bits (0)
      lTime = millis() - lTime; // total time to decode + display
      lcd.setCursor(20, lcd.height()-16);
      lcd.printf("W/O DMA, decoded in %d ms", (int)lTime);
      delay(5000);
  }
  bDMA = true;
  if (jpg.openFLASH((uint8_t *)zebra, sizeof(zebra), JPEGDraw)) { // pass the data and its size
      jpg.setPixelType(RGB565_BIG_ENDIAN); // bb_spi_lcd uses big-endian RGB565 pixels
      // if the image is smaller than the LCD dimensions, center it
      xoff = (lcd.width() - jpg.getWidth())/2;
      yoff = (lcd.height() - jpg.getHeight())/2;
      lTime = millis();
      jpg.decode(xoff,yoff, JPEG_USES_DMA); // center the image and prepare for DMA
      lTime = millis() - lTime; // total time to decode + display
      lcd.setCursor(20, lcd.height()-16);
      lcd.printf("With DMA, decoded in %d ms", (int)lTime);
  }
} /* setup() */

void loop()
{

} /* loop() */

