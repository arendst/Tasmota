//
// Display a JPEG image on a SPI LCD
// written by Larry Bank 5/12/2025
//
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <JPEGDEC.h>
#include <bb_spi_lcd.h> // SPI LCD library
BB_SPI_LCD lcd;
JPEGDEC jpg;

// Pin definitions for Adafruit PiTFT HAT
// GPIO 25 = Pin 22
#define DC_PIN 25
// OrangePi RV2 #define DC_PIN 49
// GPIO 27 = Pin 13
#define RESET_PIN -1
// GPIO 8 = Pin 24
#define CS_PIN -1
//#define CS_PIN 76
// GPIO 24 = Pin 18
#define LED_PIN 24
//#define LED_PIN 92
// SPI device is sent as MOSI pin
#define MOSI_PIN 0
// SPI device CS line is sent as MISO pin (spidev3.0 in this case)
#define MISO_PIN 0
#define SCK_PIN -1
#define LCD_TYPE LCD_ILI9341

int JPEGDraw(JPEGDRAW *pDraw)
{
     if (pDraw->y + pDraw->iHeight > lcd.height()) return 0; // beyond bottom
     lcd.setAddrWindow(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
     lcd.pushPixels(pDraw->pPixels, pDraw->iWidth * pDraw->iHeight);
     return 1;
} /* JPEGDraw() */

int main(int argc, const char *argv[])
{

    if (argc != 2) { // show help
       printf("showimg - display a JPEG image on a SPI LCD\n");
       printf("usage: showimg <input_file>\n");
       return 0;
    }
	// Initialize the display
	// int spilcdInit(int iLCDType, int bFlipRGB, int bInvert, int bFlipped, int32_t iSPIFreq, int iCSPin, int iDCPin, int iResetPin, int iLEDPin, int iMISOPin, int iMOSIPin, int iCLKPin);
        lcd.begin(LCD_TYPE, FLAGS_NONE, 62500000, CS_PIN, DC_PIN, RESET_PIN, LED_PIN, MISO_PIN, MOSI_PIN, SCK_PIN);
	lcd.setRotation(90);
        lcd.fillScreen(TFT_BLACK);
	lcd.setTextColor(TFT_GREEN, TFT_BLACK);
	lcd.drawStringFast("Linux JPEG Decoder Example", 0,0,FONT_12x16);
	if (jpg.open(argv[1], JPEGDraw)) {
		printf("Image opened: %dx%d\n", jpg.getWidth(), jpg.getHeight());
                jpg.setPixelType(RGB565_BIG_ENDIAN);
    		jpg.decode(0, 16, 0); // leave the first line of text showing
	}
	return 0;
} /* main() */

