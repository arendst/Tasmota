/*
  xdsp_09_SSD1351.ino - Display SSD1351 support for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_SPI
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_RA8876

#define XDSP_10                10
#define XI2C_39                39  // See I2CDEVICES.md

#define COLORED                1
#define UNCOLORED              0

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT

#include <RA8876.h>

uint8_t ra8876_ctouch_counter = 0;
extern uint8_t *buffer;
extern uint8_t color_type;
RA8876 *ra8876;

/*********************************************************************************************/
void RA8876_InitDriver()
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_10;
  }

  if (XDSP_10 == Settings.display_model) {

    if (Settings.display_width != RA8876_TFTWIDTH) {
      Settings.display_width = RA8876_TFTWIDTH;
    }
    if (Settings.display_height != RA8876_TFTHEIGHT) {
      Settings.display_height = RA8876_TFTHEIGHT;
    }
    buffer=0;

    // default colors
    fg_color = RA8876_WHITE;
    bg_color = RA8876_BLACK;

#ifdef ESP32
#undef HW_SPI_MOSI
#define HW_SPI_MOSI 23
#undef HW_SPI_MISO
#define HW_SPI_MISO 19
#undef HW_SPI_CLK
#define HW_SPI_CLK 18
#else
#undef HW_SPI_MOSI
#define HW_SPI_MOSI 13
#undef HW_SPI_MISO
#define HW_SPI_MISO 12
#undef HW_SPI_CLK
#define HW_SPI_CLK 14
#endif

    // init renderer, must use hardware spi
    if (PinUsed(GPIO_SSPI_CS) && (Pin(GPIO_SSPI_MOSI)==HW_SPI_MOSI) && (Pin(GPIO_SSPI_MISO)==HW_SPI_MISO) && (Pin(GPIO_SSPI_SCLK)==HW_SPI_CLK)) {
      ra8876  = new RA8876(Pin(GPIO_SSPI_CS),Pin(GPIO_SSPI_MOSI),Pin(GPIO_SSPI_MISO),Pin(GPIO_SSPI_SCLK),Pin(GPIO_BACKLIGHT));
    } else {
      if (PinUsed(GPIO_SPI_CS) && (Pin(GPIO_SPI_MOSI)==HW_SPI_MOSI) && (Pin(GPIO_SPI_MISO)==HW_SPI_MISO) && (Pin(GPIO_SPI_CLK)==HW_SPI_CLK)) {
        ra8876  = new RA8876(Pin(GPIO_SPI_CS),Pin(GPIO_SPI_MOSI),Pin(GPIO_SPI_MISO),Pin(GPIO_SPI_CLK),Pin(GPIO_BACKLIGHT));
      } else {
        return;
      }
    }

    ra8876->begin();
    renderer = ra8876;
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);
    renderer->dim(Settings.display_dimmer);

    //testall();
#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->setTextColor(RA8876_WHITE,RA8876_BLACK);
    renderer->DrawStringAt(600, 300, "RA8876", RA8876_RED,0);
    delay(1000);

#endif
    color_type = COLOR_COLOR;

#ifdef USE_FT5206
    Touch_Init(Wire);
#endif

  }
}


#ifdef USE_FT5206
#ifdef USE_TOUCH_BUTTONS

// no rotation support
void RA8876_RotConvert(int16_t *x, int16_t *y) {
int16_t temp;
  if (renderer) {
    *x=*x*renderer->width()/800;
    *y=*y*renderer->height()/480;

    *x = renderer->width() - *x;
    *y = renderer->height() - *y;
  }
}

// check digitizer hit
void RA8876_CheckTouch(void) {
  ra8876_ctouch_counter++;
  if (2 == ra8876_ctouch_counter) {
    // every 100 ms should be enough
    ra8876_ctouch_counter = 0;
    Touch_Check(RA8876_RotConvert);
  }
}
#endif // USE_TOUCH_BUTTONS
#endif // USE_FT5206

/*
void testall() {
ra8876->clearScreen(0);

ra8876->colorBarTest(true);
delay(1000);
ra8876->colorBarTest(false);

pixelTest();


delay(1000);

triangleTest();

delay(1000);

circleTest();

delay(1000);

gradientTest();

delay(1000);

textTest();

delay(1000);
}

void gradientTest()
{
Serial.println("Gradient test.");

ra8876->clearScreen(0);

int width = ra8876->getWidth();
int barHeight = ra8876->getHeight() / 4;

uint32_t starttime = millis();

for (int i = 0; i <= 255; i++)
{
  ra8876->fillRect((width / 256.0) * i, 0, (width / 256.0) * (i + 1) - 1, barHeight, RGB565(i, 0, 0));
  ra8876->fillRect((width / 256.0) * i, barHeight, (width / 256.0) * (i + 1) - 1, barHeight * 2, RGB565(0, i, 0));
  ra8876->fillRect((width / 256.0) * i, barHeight * 2, (width / 256.0) * (i + 1) - 1, barHeight * 3, RGB565(0, 0, i));
  ra8876->fillRect((width / 256.0) * i, barHeight * 3, (width / 256.0) * (i + 1) - 1, barHeight * 4, RGB565(i, i, i));
}

uint32_t elapsedtime = millis() - starttime;
Serial.print("Gradient test took "); Serial.print(elapsedtime); Serial.println(" ms");
}

void pixelTest()
{
Serial.println("Pixel test.");

int width = ra8876->getWidth();
int height = ra8876->getHeight();

uint16_t colors[] = {RGB565(255, 0, 0), RGB565(0, 255, 0), RGB565(0, 0, 255)};

uint32_t starttime = millis();

for (int c = 0; c < 3; c++)
{
  for (int i = 0; i < 3000; i++)
  {
    int x = random(0, width);
    int y = random(0, height);

    ra8876->drawPixel(x, y, colors[c]);
  }
  delay(0);
}

uint32_t elapsedtime = millis() - starttime;
Serial.print("Pixel test took "); Serial.print(elapsedtime); Serial.println(" ms");
}

void triangleTest()
{
Serial.println("Triangle test.");

int width = ra8876->getWidth();
int height = ra8876->getHeight();

uint32_t starttime = millis();

for (int i = 0; i < 2000; i++)
{
  int x1 = random(0, width);
  int y1 = random(0, height);
  int x2 = random(0, width);
  int y2 = random(0, height);
  int x3 = random(0, width);
  int y3 = random(0, height);

  uint16_t color = RGB565(random(0, 255), random(0, 255), random(0, 255));

  ra8876->fillTriangle(x1, y1, x2, y2, x3, y3, color);
  delay(0);
}

uint32_t elapsedtime = millis() - starttime;
Serial.print("Triangle test took "); Serial.print(elapsedtime); Serial.println(" ms");
}

void circleTest()
{
Serial.println("Circle test.");

int width = ra8876->getWidth();
int height = ra8876->getHeight();

uint32_t starttime = millis();

for (int i = 0; i < 2000; i++)
{
  int x = random(0, width);
  int y = random(0, height);
  int r = random(0, 384);

  uint16_t color = RGB565(random(0, 255), random(0, 255), random(0, 255));

  ra8876->fillCircle(x, y, r, color);
  delay(0);
}

uint32_t elapsedtime = millis() - starttime;
Serial.print("Circle test took "); Serial.print(elapsedtime); Serial.println(" ms");
}

void textTest()
{
Serial.println("Text test.");

uint32_t starttime = millis();

for (int s = 1; s <= 4; s++)
{
  ra8876->setTextScale(s);

  for (int i = 0; i < 3; i++)
  {
    ra8876->setCursor((ra8876->getWidth() / 3) * i, ra8876->getCursorY());
    ra8876->selectInternalFont((enum FontSize) i);
    ra8876->print("Hello");
  }

  ra8876->println();
  delay(0);
}

ra8876->setCursor(0, 32 * 10);
ra8876->setTextScale(1);

ra8876->selectInternalFont(RA8876_FONT_SIZE_32, RA8876_FONT_ENCODING_8859_1);
ra8876->println("Latin 1: na\xEFve");  // naïve

ra8876->selectInternalFont(RA8876_FONT_SIZE_32, RA8876_FONT_ENCODING_8859_2);
ra8876->println("Latin 2: \xE8" "a\xE8kalica");

ra8876->selectInternalFont(RA8876_FONT_SIZE_32, RA8876_FONT_ENCODING_8859_4);
ra8876->println("Latin 4: gie\xF0" "at");  // gieđat

ra8876->selectInternalFont(RA8876_FONT_SIZE_32, RA8876_FONT_ENCODING_8859_5);
ra8876->println("Latin 5: \xD2\xD5\xD4\xD8");  // веди

ra8876->selectInternalFont(RA8876_FONT_SIZE_32);
ra8876->print("Symbols: ");
ra8876->putChars("\x00\x01\x02\x03\x04\x05\x06\x07", 8);
ra8876->putChars("\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F", 8);
ra8876->putChars("\x10\x11\x12\x13\x14\x15\x16\x17", 8);
ra8876->putChars("\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F", 8);

uint32_t elapsedtime = millis() - starttime;
Serial.print("Text test took "); Serial.print(elapsedtime); Serial.println(" ms");
}
*/
/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp10(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
      RA8876_InitDriver();
  }
  else if (XDSP_10 == Settings.display_model) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
      case FUNC_DISPLAY_EVERY_50_MSECOND:
#ifdef USE_FT5206
        if (FT5206_found) RA8876_CheckTouch();
#endif
        break;
    }
  }
  return result;
}
#endif  // USE_DISPLAY_RA8876
#endif  // USE_DISPLAY
#endif  // USE_SPI
