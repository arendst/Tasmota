/*
  xdsp_05_epaper.ino - Display e-paper support for Tasmota

  Copyright (C) 2021  Theo Arends, Gerhard Mutz and Waveshare

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
#ifdef USE_DISPLAY_EPAPER_42

#define XDSP_06                6

#define COLORED42              1
#define UNCOLORED42            0

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT

#include <epd4in2.h>
#include <epdpaint.h>

extern uint8_t *buffer;
bool epd42_init_done = false;

Epd42 *epd42;


/*********************************************************************************************/

void EpdInitDriver42() {
  if (PinUsed(GPIO_EPAPER42_CS) &&
     ((TasmotaGlobal.soft_spi_enabled & SPI_MOSI) || (TasmotaGlobal.spi_enabled & SPI_MOSI))) {

    Settings.display_model = XDSP_06;

    if (Settings.display_width != EPD_WIDTH42) {
      Settings.display_width = EPD_WIDTH42;
    }
    if (Settings.display_height != EPD_HEIGHT42) {
      Settings.display_height = EPD_HEIGHT42;
    }

    // allocate screen buffer
    if (buffer) free(buffer);
    buffer=(unsigned char*)calloc((EPD_WIDTH42 * EPD_HEIGHT42) / 8,1);
    if (!buffer) return;

    // init renderer
    epd42  = new Epd42(EPD_WIDTH42, EPD_HEIGHT42);

    if (TasmotaGlobal.soft_spi_enabled) {
      epd42->Begin(Pin(GPIO_EPAPER42_CS), Pin(GPIO_SSPI_MOSI), Pin(GPIO_SSPI_SCLK));
    }
    else if (TasmotaGlobal.spi_enabled) {
      epd42->Begin(Pin(GPIO_EPAPER42_CS), Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_CLK));
    }

    renderer = epd42;

    epd42->Init();

    renderer->fillScreen(0);

    // whiten display with full update, takes 4 seconds
    epd42->Init(DISPLAY_INIT_FULL);

    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);

    epd42->ClearFrame();
    renderer->Updateframe();
    delay(3000);
    renderer->setTextColor(1,0);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->DrawStringAt(50, 140, "Waveshare E-Paper!", COLORED42,0);
    renderer->Updateframe();
    delay(350);
    renderer->fillScreen(0);
#endif

    epd42_init_done = true;
    AddLog_P(LOG_LEVEL_INFO, PSTR("DSP: E-Paper 4.2"));
  }
}

/*********************************************************************************************/



/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void EpdRefresh42()  // Every second
{
  if (Settings.display_mode) {  // Mode 0 is User text

  }
}

#endif  // USE_DISPLAY_MODES1TO5


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp06(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    EpdInitDriver42();
  }
  else if (epd42_init_done && (XDSP_06 == Settings.display_model)) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        EpdRefresh42();
        break;
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
  return result;
}

#endif  // USE_DISPLAY_EPAPER42
#endif  // USE_DISPLAY
#endif  // USE_SPI
