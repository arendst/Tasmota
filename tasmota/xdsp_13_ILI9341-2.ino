/*
  xdsp_13_ILI9341-TTGO-TS.ino - Display ILI9341 support for Tasmota

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

//#ifdef USE_SPI
#ifdef USE_SPI
#ifdef USE_DISPLAY
#if (defined(USE_DISPLAY_ILI9341_2) || defined(USE_DISPLAY_ILI9342))

#define XDSP_13                13

#undef COLORED
#define COLORED                1
#undef UNCOLORED
#define UNCOLORED              0


// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#undef USE_TINY_FONT
#define USE_TINY_FONT


#include <ILI9341_2.h>

extern uint8_t *buffer;
extern uint8_t color_type;
ILI9341_2 *ili9341_2;

#ifdef USE_FT5206
#include <FT5206.h>
#undef FT6336_address
#define FT6336_address 0x38
uint8_t ili9342_ctouch_counter = 0;
#endif // USE_FT5206


/*********************************************************************************************/

void ILI9341_2_InitDriver()
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_13;
  }


  if (XDSP_13 == Settings.display_model) {

    if (Settings.display_width != ILI9341_2_TFTWIDTH) {
      Settings.display_width = ILI9341_2_TFTWIDTH;
    }
    if (Settings.display_height != ILI9341_2_TFTHEIGHT) {
      Settings.display_height = ILI9341_2_TFTHEIGHT;
    }

    // disable screen buffer
    buffer=NULL;

    // default colors
    fg_color = ILI9341_2_WHITE;
    bg_color = ILI9341_2_BLACK;

#ifdef USE_M5STACK_CORE2
    ili9341_2  = new ILI9341_2(5, -2, 15, -2);
#else
    // init renderer, may use hardware spi, however we use SSPI defintion because SD card uses SPI definition  (2 spi busses)
    if (PinUsed(GPIO_SSPI_CS) && PinUsed(GPIO_OLED_RESET) && PinUsed(GPIO_BACKLIGHT) && PinUsed(GPIO_SSPI_MOSI) && PinUsed(GPIO_SSPI_MISO) && PinUsed(GPIO_SSPI_SCLK) && PinUsed(GPIO_SSPI_DC)) {
        ili9341_2  = new ILI9341_2(Pin(GPIO_SSPI_CS), Pin(GPIO_SSPI_MOSI), Pin(GPIO_SSPI_MISO), Pin(GPIO_SSPI_SCLK), Pin(GPIO_OLED_RESET), Pin(GPIO_SSPI_DC), Pin(GPIO_BACKLIGHT));
    } else {
      return;
    }
#endif

    ili9341_2->init(Settings.display_width,Settings.display_height);
    renderer = ili9341_2;
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);
    renderer->dim(Settings.display_dimmer);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->setTextColor(ILI9341_2_WHITE,ILI9341_2_BLACK);
    renderer->DrawStringAt(30, 100, "ILI9341 TFT!", ILI9341_2_WHITE,0);
    delay(1000);
#endif

    color_type = COLOR_COLOR;

#ifdef ESP32
#ifdef USE_FT5206
        // start digitizer with fixed adress and pins for esp32
        #define SDA_2 21
        #define SCL_2 22
        Wire1.begin(SDA_2, SCL_2, 400000);
        Touch_Init(Wire1);
#endif // USE_FT5206
#endif // ESP32


  }
}

void core2_disp_pwr(uint8_t on);
void core2_disp_dim(uint8_t dim);

void ili9342_bpwr(uint8_t on) {
#ifdef USE_M5STACK_CORE2
  core2_disp_pwr(on);
#endif
}

void ili9342_dimm(uint8_t dim) {
#ifdef USE_M5STACK_CORE2
  core2_disp_dim(dim);
#endif
}

#ifdef ESP32
#ifdef USE_FT5206
#ifdef USE_TOUCH_BUTTONS

void ili9342_RotConvert(int16_t *x, int16_t *y) {

int16_t temp;
  if (renderer) {
    uint8_t rot=renderer->getRotation();
    switch (rot) {
      case 0:
        break;
      case 1:
        temp=*y;
        *y=renderer->height()-*x;
        *x=temp;
        break;
      case 2:
        *x=renderer->width()-*x;
        *y=renderer->height()-*y;
        break;
      case 3:
        temp=*y;
        *y=*x;
        *x=renderer->width()-temp;
        break;
    }
  }
}

// check digitizer hit
void ili9342_CheckTouch() {
ili9342_ctouch_counter++;
  if (2 == ili9342_ctouch_counter) {
    // every 100 ms should be enough
    ili9342_ctouch_counter = 0;
    Touch_Check(ili9342_RotConvert);
  }
}
#endif // USE_TOUCH_BUTTONS
#endif // USE_FT5206
#endif // ESP32


/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp13(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    ILI9341_2_InitDriver();
  }
  else if (XDSP_13 == Settings.display_model) {
      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
#ifdef USE_FT5206
#ifdef USE_TOUCH_BUTTONS
        case FUNC_DISPLAY_EVERY_50_MSECOND:
          if (FT5206_found) {
            ili9342_CheckTouch();
          }
          break;
#endif // USE_TOUCH_BUTTONS
#endif // USE_FT5206
    }
  }
  return result;
}

#endif  // USE_DISPLAY_ILI9341_2
#endif  // USE_DISPLAY
#endif  // USE_SPI
