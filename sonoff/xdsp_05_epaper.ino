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

#ifdef USE_SPI
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_EPAPER

#define XDSP_05                5

#define COLORED                0
#define UNCOLORED              1

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT

#include <epd2in9.h>
#include <epdpaint.h>

unsigned char image[(EPD_HEIGHT * EPD_WIDTH) / 8];

Paint paint(image, EPD_WIDTH, EPD_HEIGHT);    // width should be the multiple of 8
Epd epd;
sFONT *selected_font;

/*********************************************************************************************/

void EpdInitMode()
{
  // whiten display with full update
  epd.Init(lut_full_update);

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  delay(3000);

  // switch to partial update
  epd.Init(lut_partial_update);

  // Clear image memory
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  delay(500);

  selected_font = &Font12;

  paint.SetRotate(Settings.display_rotate);
/*
  // Welcome text
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(50, 50, "Waveshare E-Paper Display!", selected_font, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  delay(1000);
*/
  paint.Clear(UNCOLORED);
}

void EpdInitPartial()
{
  epd.Init(lut_partial_update);
  //paint.Clear(UNCOLORED);
  epd.DisplayFrame();
  delay(500);
}

void EpdInitFull()
{
  epd.Init(lut_full_update);
  //paint.Clear(UNCOLORED);
  //epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();
  delay(3000);
}

void EpdInit(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
      EpdInitMode();
      break;
    case DISPLAY_INIT_PARTIAL:
      EpdInitPartial();
      break;
    case DISPLAY_INIT_FULL:
      EpdInitFull();
      break;
  }
}

void EpdInitDriver()
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_05;
  }

  if (XDSP_05 == Settings.display_model) {
    epd.cs_pin = pin[GPIO_SPI_CS];
    epd.mosi_pin = pin[GPIO_SPI_MOSI];  // 13
    epd.sclk_pin = pin[GPIO_SPI_CLK];   // 14

    EpdInitMode();
  }
}

/*********************************************************************************************/

void EpdClear()
{
  paint.Clear(UNCOLORED);
}

void EpdSetFontorSize(uint8_t font)
{
  if (1 == font) {
    selected_font = &Font12;
  } else {
#ifdef USE_TINY_FONT
    if (2 == font) {
      selected_font = &Font24;
    } else {
      selected_font = &Font8;
    }
#else
    selected_font = &Font24;
#endif
  }
}

void EpdDrawStringAt(uint16_t x, uint16_t y, char *str, uint8_t color, uint8_t flag)
{
  if (!flag) {
    paint.DrawStringAt(x, y, str, selected_font, color);
  } else {
    paint.DrawStringAt((x-1) * selected_font->Width, (y-1) * selected_font->Height, str, selected_font, color);
  }
}

// not needed
void EpdDisplayOnOff(uint8_t on)
{

}

void EpdOnOff()
{
  EpdDisplayOnOff(disp_power);
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void EpdRefresh()  // Every second
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

  if (spi_flg) {
    if (FUNC_DISPLAY_INIT_DRIVER == function) {
      EpdInitDriver();
    }
    else if (XDSP_04 == Settings.display_model) {

      if (!dsp_color) { dsp_color = COLORED; }

      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_INIT:
          EpdInit(dsp_init);
          break;
        case FUNC_DISPLAY_POWER:
          EpdOnOff();
          break;
        case FUNC_DISPLAY_CLEAR:
          EpdClear();
          break;
        case FUNC_DISPLAY_DRAW_HLINE:
          paint.DrawHorizontalLine(dsp_x, dsp_y, dsp_len, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_VLINE:
          paint.DrawVerticalLine(dsp_x, dsp_y, dsp_len, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_LINE:
          paint.DrawLine(dsp_x, dsp_y, dsp_x2, dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_CIRCLE:
          paint.DrawCircle(dsp_x, dsp_y, dsp_rad, dsp_color);
          break;
        case FUNC_DISPLAY_FILL_CIRCLE:
          paint.DrawFilledCircle(dsp_x, dsp_y, dsp_rad, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_RECTANGLE:
          paint.DrawRectangle(dsp_x, dsp_y, dsp_x + dsp_x2, dsp_y + dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_FILL_RECTANGLE:
          paint.DrawFilledRectangle(dsp_x, dsp_y, dsp_x + dsp_x2, dsp_y + dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_FRAME:
          epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
          epd.DisplayFrame();
          break;
        case FUNC_DISPLAY_FONT_SIZE:
          EpdSetFontorSize(Settings.display_size);
          break;
        case FUNC_DISPLAY_DRAW_STRING:
          EpdDrawStringAt(dsp_x, dsp_y, dsp_str, dsp_color, dsp_flag);
          break;
        case FUNC_DISPLAY_ONOFF:
          EpdDisplayOnOff(dsp_on);
          break;
        case FUNC_DISPLAY_ROTATION:
          paint.SetRotate(Settings.display_rotate);
          break;
#ifdef USE_DISPLAY_MODES1TO5
        case FUNC_DISPLAY_EVERY_SECOND:
          EpdRefresh();
          break;
#endif  // USE_DISPLAY_MODES1TO5
      }
    }
  }
  return result;
}

#endif  // USE_DISPLAY_EPAPER
#endif  // USE_DISPLAY
#endif  // USE_SPI
