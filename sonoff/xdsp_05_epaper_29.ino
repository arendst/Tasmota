/*
  xdsp_05_epaper.ino - Display e-paper support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends, Gerhard Mutz and Waveshare

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

#ifdef USE_SOFTSPI
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_EPAPER29

#define XDSP_05                5

#define COLORED                1
#define UNCOLORED              0

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT

#include <epd2in9.h>
#include <epdpaint.h>

//unsigned char image[(EPD_HEIGHT * EPD_WIDTH) / 8];
extern uint8_t *buffer;

Epd *epd;

/*********************************************************************************************/

void EpdInitDriver29()
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_05;
  }

  if (XDSP_05 == Settings.display_model) {

    // allocate screen buffer
    if (buffer) free(buffer);
    buffer=(unsigned char*)calloc((EPD_WIDTH * EPD_HEIGHT) / 8,1);
    if (!buffer) return;

    // init renderer
    epd  = new Epd(EPD_WIDTH,EPD_HEIGHT);

    // whiten display with full update, takes 3 seconds
#ifdef USE_SOFTSPI
    if  ((pin[GPIO_SSPI_CS]<99) && (pin[GPIO_SSPI_MOSI]<99) && (pin[GPIO_SSPI_SCLK]<99)){
      epd->Begin(pin[GPIO_SSPI_CS],pin[GPIO_SSPI_MOSI],pin[GPIO_SSPI_SCLK]);
    } else {
      free(buffer);
      return;
    }
#else
    if ((pin[GPIO_SPI_CS]<99) && (pin[GPIO_SPI_MOSI]<99) && (pin[GPIO_SPI_CLK]<99)) {
      epd->Begin(pin[GPIO_SPI_CS],pin[GPIO_SPI_MOSI],pin[GPIO_SPI_CLK]);
    } else {
      free(buffer);
      return;
    }
#endif

    renderer = epd;
    epd->Init(DISPLAY_INIT_FULL);
    epd->Init(DISPLAY_INIT_PARTIAL);
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(1);
    renderer->DrawStringAt(50, 50, (char*)(const char*)"Waveshare E-Paper Display!", COLORED,0);
    renderer->Updateframe();
    delay(1000);
    renderer->fillScreen(0);
#endif

  }
}

/*********************************************************************************************/



/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void EpdRefresh29()  // Every second
{
  if (Settings.display_mode) {  // Mode 0 is User text

  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdsp05(byte function)
{
  boolean result = false;

  //if (spi_flg) {
    if (FUNC_DISPLAY_INIT_DRIVER == function) {
      EpdInitDriver29();
    }
    else if (XDSP_05 == Settings.display_model) {

      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;

#ifdef USE_DISPLAY_MODES1TO5
        case FUNC_DISPLAY_EVERY_SECOND:
          EpdRefresh29();
          break;
#endif  // USE_DISPLAY_MODES1TO5
      }
    }
  //}
  return result;
}

#endif  // USE_DISPLAY_EPAPER
#endif  // USE_DISPLAY
#endif  // USE_SPI
