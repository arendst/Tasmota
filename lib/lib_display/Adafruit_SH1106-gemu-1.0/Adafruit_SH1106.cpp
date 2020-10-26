/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

/*********************************************************************
I change the adafruit SSD1306 to SH1106

SH1106 driver similar to SSD1306 so, just change the display() method.

However, SH1106 driver don't provide several functions such as scroll commands.


*********************************************************************/

//#include <avr/pgmspace.h>
#ifndef __SAM3X8E__
// #include <util/delay.h>
#endif
#include <stdlib.h>
#include <Wire.h>
//#include <../../sonoff/settings.h>

#include "Adafruit_SH1106.h"
#define DISPLAY_INIT_MODE 0

// the memory buffer for the LCD
extern uint8_t *buffer;

Adafruit_SH1106::Adafruit_SH1106(int16_t width, int16_t height) :
Renderer(width,height) {
}

void Adafruit_SH1106::DisplayOnff(int8_t on) {
  if (on) {
    SH1106_command(SH1106_DISPLAYON);
  } else {
    SH1106_command(SH1106_DISPLAYOFF);
  }
}

void Adafruit_SH1106::Updateframe() {
  display();
}

void Adafruit_SH1106::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {
// ignore update mode
  //if (p==DISPLAY_INIT_MODE) {
    setRotation(rot);
    invertDisplay(false);
    setTextWrap(false);         // Allow text to run off edges
    cp437(true);
    setTextFont(font);
    setTextSize(size);
    setTextColor(WHITE,BLACK);
    setCursor(0,0);
    fillScreen(BLACK);
    Updateframe();
  //}
}

void Adafruit_SH1106::Begin(int16_t p1,int16_t p2,int16_t p3) {
  begin(p1,p2,p3);
}


void Adafruit_SH1106::begin(uint8_t vccstate, uint8_t i2caddr, bool reset) {
  _vccstate = vccstate;
  _i2caddr = i2caddr;
    // I2C Init
    Wire.begin();

   #if defined SH1106_128_32
    // Init sequence for 128x32 OLED module
    SH1106_command(SH1106_DISPLAYOFF);                    // 0xAE
    SH1106_command(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
    SH1106_command(0x80);                                  // the suggested ratio 0x80
    SH1106_command(SH1106_SETMULTIPLEX);                  // 0xA8
    SH1106_command(0x1F);
    SH1106_command(SH1106_SETDISPLAYOFFSET);              // 0xD3
    SH1106_command(0x0);                                   // no offset
    SH1106_command(SH1106_SETSTARTLINE | 0x0);            // line #0
    SH1106_command(SH1106_CHARGEPUMP);                    // 0x8D
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x10); }
    else
      { SH1106_command(0x14); }
    SH1106_command(SH1106_MEMORYMODE);                    // 0x20
    SH1106_command(0x00);                                  // 0x0 act like ks0108
    SH1106_command(SH1106_SEGREMAP | 0x1);
    SH1106_command(SH1106_COMSCANDEC);
    SH1106_command(SH1106_SETCOMPINS);                    // 0xDA
    SH1106_command(0x02);
    SH1106_command(SH1106_SETCONTRAST);                   // 0x81
    SH1106_command(0x8F);
    SH1106_command(SH1106_SETPRECHARGE);                  // 0xd9
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x22); }
    else
      { SH1106_command(0xF1); }
    SH1106_command(SH1106_SETVCOMDETECT);                 // 0xDB
    SH1106_command(0x40);
    SH1106_command(SH1106_DISPLAYALLON_RESUME);           // 0xA4
    SH1106_command(SH1106_NORMALDISPLAY);                 // 0xA6
  #endif

  #if defined SH1106_128_64
    // Init sequence for 128x64 OLED module
    SH1106_command(SH1106_DISPLAYOFF);                    // 0xAE
    SH1106_command(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
    SH1106_command(0x80);                                  // the suggested ratio 0x80
    SH1106_command(SH1106_SETMULTIPLEX);                  // 0xA8
    SH1106_command(0x3F);
    SH1106_command(SH1106_SETDISPLAYOFFSET);              // 0xD3
    SH1106_command(0x00);                                   // no offset

    SH1106_command(SH1106_SETSTARTLINE | 0x0);            // line #0 0x40
    SH1106_command(SH1106_CHARGEPUMP);                    // 0x8D
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x10); }
    else
      { SH1106_command(0x14); }
    SH1106_command(SH1106_MEMORYMODE);                    // 0x20
    SH1106_command(0x00);                                  // 0x0 act like ks0108
    SH1106_command(SH1106_SEGREMAP | 0x1);
    SH1106_command(SH1106_COMSCANDEC);
    SH1106_command(SH1106_SETCOMPINS);                    // 0xDA
    SH1106_command(0x12);
    SH1106_command(SH1106_SETCONTRAST);                   // 0x81
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x9F); }
    else
      { SH1106_command(0xCF); }
    SH1106_command(SH1106_SETPRECHARGE);                  // 0xd9
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x22); }
    else
      { SH1106_command(0xF1); }
    SH1106_command(SH1106_SETVCOMDETECT);                 // 0xDB
    SH1106_command(0x40);
    SH1106_command(SH1106_DISPLAYALLON_RESUME);           // 0xA4
    SH1106_command(SH1106_NORMALDISPLAY);                 // 0xA6
  #endif

  #if defined SH1106_96_16
    // Init sequence for 96x16 OLED module
    SH1106_command(SH1106_DISPLAYOFF);                    // 0xAE
    SH1106_command(SH1106_SETDISPLAYCLOCKDIV);            // 0xD5
    SH1106_command(0x80);                                  // the suggested ratio 0x80
    SH1106_command(SH1106_SETMULTIPLEX);                  // 0xA8
    SH1106_command(0x0F);
    SH1106_command(SH1106_SETDISPLAYOFFSET);              // 0xD3
    SH1106_command(0x00);                                   // no offset
    SH1106_command(SH1106_SETSTARTLINE | 0x0);            // line #0
    SH1106_command(SH1106_CHARGEPUMP);                    // 0x8D
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x10); }
    else
      { SH1106_command(0x14); }
    SH1106_command(SH1106_MEMORYMODE);                    // 0x20
    SH1106_command(0x00);                                  // 0x0 act like ks0108
    SH1106_command(SH1106_SEGREMAP | 0x1);
    SH1106_command(SH1106_COMSCANDEC);
    SH1106_command(SH1106_SETCOMPINS);                    // 0xDA
    SH1106_command(0x2);	//ada x12
    SH1106_command(SH1106_SETCONTRAST);                   // 0x81
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x10); }
    else
      { SH1106_command(0xAF); }
    SH1106_command(SH1106_SETPRECHARGE);                  // 0xd9
    if (vccstate == SH1106_EXTERNALVCC)
      { SH1106_command(0x22); }
    else
      { SH1106_command(0xF1); }
    SH1106_command(SH1106_SETVCOMDETECT);                 // 0xDB
    SH1106_command(0x40);
    SH1106_command(SH1106_DISPLAYALLON_RESUME);           // 0xA4
    SH1106_command(SH1106_NORMALDISPLAY);                 // 0xA6
  #endif

  SH1106_command(SH1106_DISPLAYON);//--turn on oled panel
}


void Adafruit_SH1106::invertDisplay(uint8_t i) {
  if (i) {
    SH1106_command(SH1106_INVERTDISPLAY);
  } else {
    SH1106_command(SH1106_NORMALDISPLAY);
  }
}

void Adafruit_SH1106::SH1106_command(uint8_t c) {

    // I2C
    uint8_t control = 0x00;   // Co = 0, D/C = 0
    Wire.beginTransmission(_i2caddr);
    WIRE_WRITE(control);
    WIRE_WRITE(c);
    Wire.endTransmission();

}

// Dim the display
// dim = true: display is dimmed
// dim = false: display is normal
void Adafruit_SH1106::dim(uint8_t dim) {
  uint8_t contrast;

  if (dim) {
    contrast = 0; // Dimmed display
  } else {
    if (_vccstate == SH1106_EXTERNALVCC) {
      contrast = 0x9F;
    } else {
      contrast = 0xCF;
    }
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  SH1106_command(SH1106_SETCONTRAST);
  SH1106_command(contrast);
}

void Adafruit_SH1106::SH1106_data(uint8_t c) {
    // I2C
    uint8_t control = 0x40;   // Co = 0, D/C = 1
    Wire.beginTransmission(_i2caddr);
    WIRE_WRITE(control);
    WIRE_WRITE(c);
    Wire.endTransmission();
}

void Adafruit_SH1106::display(void) {
    SH1106_command(SH1106_SETLOWCOLUMN | 0x0);  // low col = 0
    SH1106_command(SH1106_SETHIGHCOLUMN | 0x0);  // hi col = 0
    SH1106_command(SH1106_SETSTARTLINE | 0x0); // line #0
    // I2C
    //height >>= 3;
    //width >>= 3;
	byte height=64;
	byte width=132;
	byte m_row = 0;
	byte m_col = 2;


	height >>= 3;
	width >>= 3;
	//Serial.println(width);

	int p = 0;

	byte i, j, k =0;

	for ( i = 0; i < height; i++) {

		// send a bunch of data in one xmission
        SH1106_command(0xB0 + i + m_row);//set page address
        SH1106_command(m_col & 0xf);//set lower column address
        SH1106_command(0x10 | (m_col >> 4));//set higher column address

        for( j = 0; j < 8; j++){
			Wire.beginTransmission(_i2caddr);
            Wire.write(0x40);
            for ( k = 0; k < width; k++, p++) {
		Wire.write(buffer[p]);
            }
            Wire.endTransmission();
        	}
	}

}

// clear everything
void Adafruit_SH1106::clearDisplay(void) {
  memset(buffer, 0, (SH1106_LCDWIDTH*SH1106_LCDHEIGHT/8));
}
