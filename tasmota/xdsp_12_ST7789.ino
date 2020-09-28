/*
  xdsp_12_ST7789.ino - Display ST7789 support for Tasmota

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
#ifdef USE_DISPLAY_ST7789

#define XDSP_12                12
#define XI2C_38                38  // See I2CDEVICES.md

#undef COLORED
#define COLORED                1
#undef UNCOLORED
#define UNCOLORED              0

// touch panel controller
#undef FT5206_address
#define FT5206_address 0x38

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#undef USE_TINY_FONT
#define USE_TINY_FONT


#include <Arduino_ST7789.h>
#include <FT5206.h>

// currently fixed
#define BACKPLANE_PIN 2

extern uint8_t *buffer;
extern uint8_t color_type;
Arduino_ST7789 *st7789;

#ifdef USE_FT5206
uint8_t st7789_ctouch_counter = 0;
#endif // USE_FT5206

/*********************************************************************************************/

void ST7789_InitDriver()
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_12;
  }

  if (XDSP_12 == Settings.display_model) {

    if (Settings.display_width != ST7789_TFTWIDTH) {
      Settings.display_width = ST7789_TFTWIDTH;
    }
    if (Settings.display_height != ST7789_TFTHEIGHT) {
      Settings.display_height = ST7789_TFTHEIGHT;
    }

    // disable screen buffer
    buffer=NULL;

    // default colors
    fg_color = ST7789_WHITE;
    bg_color = ST7789_BLACK;

    int8_t bppin=BACKPLANE_PIN;
    if  (PinUsed(GPIO_BACKLIGHT)) {
      bppin=Pin(GPIO_BACKLIGHT);
    }

    int8_t reset = -1;
    if  (PinUsed(GPIO_OLED_RESET)) {
      reset=Pin(GPIO_OLED_RESET);
    }

    int8_t cs = -1;
    if  (PinUsed(GPIO_SSPI_CS)) {
      cs=Pin(GPIO_SSPI_CS);
    } else if (PinUsed(GPIO_SPI_CS)) {
      cs=Pin(GPIO_SPI_CS);
    }

#ifdef ESP32
#undef HW_SPI_MOSI
#define HW_SPI_MOSI 23
#undef HW_SPI_CLK
#define HW_SPI_CLK 18
#else
#undef HW_SPI_MOSI
#define HW_SPI_MOSI 13
#undef HW_SPI_CLK
#define HW_SPI_CLK 14
#endif

    // init renderer, may use hardware spi
    if (PinUsed(GPIO_SPI_CS) && (Pin(GPIO_SPI_MOSI)==HW_SPI_MOSI) && (Pin(GPIO_SPI_CLK)==HW_SPI_CLK) && PinUsed(GPIO_SPI_DC)) {
        st7789  = new Arduino_ST7789(Pin(GPIO_SPI_DC), reset, cs, bppin);
    } else {
      if ((PinUsed(GPIO_SSPI_CS) || PinUsed(GPIO_OLED_RESET)) && PinUsed(GPIO_SSPI_MOSI) && PinUsed(GPIO_SSPI_SCLK) && PinUsed(GPIO_SSPI_DC)) {
        st7789  = new Arduino_ST7789(Pin(GPIO_SSPI_DC), reset, Pin(GPIO_SSPI_MOSI), Pin(GPIO_SSPI_SCLK), cs, bppin);
      } else {
        return;
      }
    }
    st7789->init(Settings.display_width,Settings.display_height);
    renderer = st7789;
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->setTextColor(ST7789_WHITE,ST7789_BLACK);
    renderer->DrawStringAt(30, 100, "ST7789 TFT!", ST7789_WHITE,0);
    delay(1000);
#endif

    color_type = COLOR_COLOR;

#ifdef ESP32
#ifdef USE_FT5206
    // start digitizer with fixed adress and pins for esp32
    #define SDA_2 23
    #define SCL_2 32
    Wire1.begin(SDA_2, SCL_2, 400000);
    Touch_Init(Wire1);
#endif // USE_FT5206
#endif // ESP32

  }
}

#ifdef ESP32
#ifdef USE_FT5206
#ifdef USE_TOUCH_BUTTONS

void ST7789_RotConvert(int16_t *x, int16_t *y) {
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
void ST7789_CheckTouch() {
st7789_ctouch_counter++;
  if (2 == st7789_ctouch_counter) {
    // every 100 ms should be enough
    st7789_ctouch_counter = 0;
    Touch_Check(ST7789_RotConvert);
  }
}
#endif // USE_TOUCH_BUTTONS
#endif // USE_FT5206
#endif // ESP32

/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp12(uint8_t function)
{
  bool result = false;

//AddLog_P2(LOG_LEVEL_INFO, PSTR("touch %d - %d"), FT5206_found, function);

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    ST7789_InitDriver();
  }
  else if (XDSP_12 == Settings.display_model) {
      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_EVERY_50_MSECOND:
#ifdef USE_FT5206
#ifdef USE_TOUCH_BUTTONS
          if (FT5206_found) {
            ST7789_CheckTouch();
          }
#endif
#endif // USE_FT5206
          break;
    }
  }
  return result;
}

#endif  // USE_DISPLAY_ST7789
#endif  // USE_DISPLAY
#endif  // USE_SPI
