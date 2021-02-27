/*
  xdsp_08_ILI9488.ino - Display ILI9488 support for Tasmota

  Copyright (C) 2021  Theo Arends, Gerhard Mutz

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
#ifdef USE_DISPLAY_ILI9488

#define XDSP_08                8
#define XI2C_38                38  // See I2CDEVICES.md

#define COLORED                1
#define UNCOLORED              0

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT


#include <ILI9488.h>
uint8_t ili9488_ctouch_counter = 0;
bool ili9488_init_done = false;

// currently fixed
#define BACKPLANE_PIN 2

extern uint8_t *buffer;
extern uint8_t color_type;
ILI9488 *ili9488;
extern const uint16_t picture[];

/*********************************************************************************************/

void ILI9488_InitDriver(void) {
  if (PinUsed(GPIO_ILI9488_CS) && (TasmotaGlobal.spi_enabled & SPI_MOSI)) {

    Settings.display_model = XDSP_08;

    if (Settings.display_width != ILI9488_TFTWIDTH) {
      Settings.display_width = ILI9488_TFTWIDTH;
    }
    if (Settings.display_height != ILI9488_TFTHEIGHT) {
      Settings.display_height = ILI9488_TFTHEIGHT;
    }

    // disable screen buffer
    buffer = NULL;

    // default colors
    fg_color = ILI9488_WHITE;
    bg_color = ILI9488_BLACK;

    int8_t bppin = BACKPLANE_PIN;
    if (PinUsed(GPIO_BACKLIGHT)) {
      bppin = Pin(GPIO_BACKLIGHT);
    }

    // init renderer, must use hardware spi
    ili9488  = new ILI9488(Pin(GPIO_ILI9488_CS), Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_CLK), bppin);

    ili9488->begin();
    renderer = ili9488;
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);
    renderer->dim(Settings.display_dimmer);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->setTextColor(ILI9488_WHITE,ILI9488_BLACK);
    renderer->DrawStringAt(50, 50, "ILI9488 TFT Display!", ILI9488_WHITE,0);
    delay(1000);

    //renderer->drawRGBBitmap(100,100, picture,51,34);
#endif

    color_type = COLOR_COLOR;
    // start digitizer
#ifdef USE_FT5206
    Touch_Init(Wire);
#endif

    ili9488_init_done = true;
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: ILI9488"));
  }
}

#ifdef USE_FT5206
#ifdef USE_TOUCH_BUTTONS

void ILI9488_RotConvert(int16_t *x, int16_t *y) {
int16_t temp;
  if (renderer) {
    uint8_t rot=renderer->getRotation();
    switch (rot) {
      case 0:
        temp=*y;
        *y=renderer->height()-*x;
        *x=temp;
        break;
      case 1:
        break;
      case 2:
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
void ILI9488_CheckTouch(void) {
  ili9488_ctouch_counter++;
  if (2 == ili9488_ctouch_counter) {
    // every 100 ms should be enough
    ili9488_ctouch_counter = 0;
    Touch_Check(ILI9488_RotConvert);
  }
}
#endif // USE_TOUCH_BUTTONS
#endif // USE_FT5206


/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp08(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    ILI9488_InitDriver();
  }
  else if (ili9488_init_done && (XDSP_08 == Settings.display_model)) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
      case FUNC_DISPLAY_EVERY_50_MSECOND:
#ifdef USE_TOUCH_BUTTONS
        if (FT5206_found) {
          ILI9488_CheckTouch();
        }
#endif
        break;
    }
  }
  return result;
}

#endif  // USE_DISPLAY_ILI9488
#endif  // USE_DISPLAY
#endif  // USE_SPI
