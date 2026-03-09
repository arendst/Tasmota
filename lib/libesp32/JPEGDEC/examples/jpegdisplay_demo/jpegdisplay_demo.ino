//
// JPEGDisplay class demo
//
// This sketch shows how to use the new helper class, JPEGDisplay to more easily
// display JPEG images on displays supported by my bb_spi_lcd library
// There are only two overloaded methods exposed by the library: loadJPEG(), getJPEGInfo()
// It allows you to pass JPEG image data as a pointer or a filename on a uSD card
// loadJPEG requires a x,y position for where to draw the image. This code doesn't
// currently support clipping, so attempts to draw off the edge of the display
// will return with an error.
//
#include <JPEGDisplay.h>  // the definitions
#include <JPEGDisplay.inl> // the code
#include <bb_spi_lcd.h>
#include <SPI.h>
#include <SD.h>
#include "octocat_small.h"

BB_SPI_LCD lcd, sprite; // one instance for the display and another for a 'sprite'
JPEGDisplay jd; // only one instance of this class is needed
#define USE_SDCARD
SPIClass SD_SPI;
bool bSD = false;
// These GPIOs are for the uSD card slot on the JC4827W543 "Cheap Yellow Display"
#define SD_CS 10
#define SD_MOSI 11
#define SD_SCK 12
#define SD_MISO 13
// These GPIOs are for the uSD card slot on the Waveshare ESP32-S3 AMOLED 2.41"
//#define SD_CS 2
//#define SD_MOSI 5
//#define SD_SCK 4
//#define SD_MISO 6

void setup() {
  int x, y, w, h, bpp;
  lcd.begin(DISPLAY_CYD_543);
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_GREEN);
  lcd.setFont(FONT_12x16);

#ifdef USE_SDCARD  
  SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, SD_SPI, 10000000)) { // Faster than 10MHz seems to fail on the CYDs
    lcd.println("Card Mount Failed");
  } else {
    lcd.println("Card Mount Succeeded");
    bSD = true;
  }
// Load a JPEG from the uSD card
  if (bSD) {
    // Instead of passing absolute x/y positioning, let the library center it
      jd.loadJPEG(&lcd, JPEGDISPLAY_CENTER, JPEGDISPLAY_CENTER, "/tulips_320x213.jpg"); // load this image from the root dir of the SD card
      delay(5000);
  }
  delay(3000);
#endif // USE_SDCARD
  //
  // Load and display the PNG image all over the display by using a 'sprite'
  // First, create a sprite instance of BB_SPI_LCD with the createVirtual() method
  // Next, decode a PNG image directly into the sprite memory
  // And finally, draw it in multiple places on the LCD
  //
  lcd.fillScreen(TFT_BLACK);
  // You can request the dimensions and bit depth of the image BEFORE decoding it
  if (jd.getJPEGInfo(&w, &h, &bpp, octocat_small, sizeof(octocat_small))) {
    sprite.createVirtual(w, h); // create a sprite of the JPEG image size
    // The JPEG image can be decoded directly into the sprite instance
    jd.loadJPEG(&sprite, 0, 0, octocat_small, sizeof(octocat_small));
    for (int y = 0; y < lcd.height(); y += h) { // now draw it all over the LCD
        for (int x = 0; x < lcd.width(); x += w) {
          lcd.drawSprite(x, y, &sprite, 0xffffffff); // 0xffffffff = no transparent color
        } // for x
    } // for y
    sprite.freeVirtual(); // free the sprite memory
  }
}

void loop() {
}
