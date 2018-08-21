/*
  xdsp_04_ili9341.ino - Display Tft Ili9341 support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends and Adafruit

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
#ifdef USE_DISPLAY_ILI9341

#define XDSP_04                4

#define TFT_TOP                16
#define TFT_BOTTOM             16
#define TFT_FONT_WIDTH         6
#define TFT_FONT_HEIGTH        8     // Adafruit minimal font heigth pixels

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>        // TFT 320x240 and 480x320

Adafruit_ILI9341 *tft;

uint16_t tft_scroll;

/*********************************************************************************************/

void Ili9341InitMode()
{
  tft->setRotation(Settings.display_rotate);  // 0
  tft->invertDisplay(0);
  tft->fillScreen(ILI9341_BLACK);
  tft->setTextWrap(false);         // Allow text to run off edges
  tft->cp437(true);
  if (!Settings.display_mode) {
    tft->setCursor(0, 0);
    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft->setTextSize(1);
  } else {
    tft->setScrollMargins(TFT_TOP, TFT_BOTTOM);
    tft->setCursor(0, 0);
    tft->setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    tft->setTextSize(2);
    tft->println("HEADER");

    tft_scroll = TFT_TOP;
  }
}

void Ili9341Init(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
      Ili9341InitMode();
      break;
    case DISPLAY_INIT_PARTIAL:
    case DISPLAY_INIT_FULL:
      break;
  }
}

void Ili9341InitDriver()
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_04;
  }

  if (XDSP_04 == Settings.display_model) {
    tft = new Adafruit_ILI9341(pin[GPIO_SPI_CS], pin[GPIO_SPI_DC]);
    tft->begin();

    Ili9341InitMode();
  }
}

void Ili9341Clear()
{
  tft->fillScreen(ILI9341_BLACK);
  tft->setCursor(0, 0);
}

void Ili9341DrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag)
{
  uint16_t active_color = ILI9341_WHITE;

  tft->setTextSize(Settings.display_size);
  if (!flag) {
    tft->setCursor(x, y);
  } else {
    tft->setCursor((x-1) * TFT_FONT_WIDTH * Settings.display_size, (y-1) * TFT_FONT_HEIGTH * Settings.display_size);
  }
  if (color) { active_color = color; }
  tft->setTextColor(active_color, ILI9341_BLACK);
  tft->println(str);
}

void Ili9341DisplayOnOff(uint8_t on)
{
//  tft->showDisplay(on);
//  tft->invertDisplay(on);
  if (pin[GPIO_BACKLIGHT] < 99) {
    pinMode(pin[GPIO_BACKLIGHT], OUTPUT);
    digitalWrite(pin[GPIO_BACKLIGHT], on);
  }
}

void Ili9341OnOff()
{
  Ili9341DisplayOnOff(disp_power);
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void Ili9341PrintLogLine()
{
  tft->setTextColor(ILI9341_CYAN, ILI9341_BLACK);   // Add background color to solve flicker
  tft->setCursor(0, tft_scroll);
  byte size = Settings.display_size;
  tft->setTextSize(size);
  uint16_t theight = size * TFT_FONT_HEIGTH;

  tft->fillRect(0, tft_scroll, tft->width(), theight, ILI9341_BLACK);  // Erase line

  char *pch = strchr(disp_log_buffer[disp_log_buffer_ptr],'~');  // = 0x7E (~) Replace degrees character (276 octal)
  if (pch != NULL) {
    disp_log_buffer[disp_log_buffer_ptr][pch - disp_log_buffer[disp_log_buffer_ptr]] = '\370';  // = 0xF8
  }

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_APPLICATION "[%s]"), disp_log_buffer[disp_log_buffer_ptr]);
  AddLog(LOG_LEVEL_DEBUG);

  tft->print(disp_log_buffer[disp_log_buffer_ptr]);
  tft_scroll += theight;
  if (tft_scroll >= (tft->height() - TFT_BOTTOM)) {
    tft_scroll = TFT_TOP;
  }
  tft->scrollTo(tft_scroll);
}

void Ili9341PrintLog()
{
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    disp_log_buffer_active = (disp_log_buffer_idx != disp_log_buffer_ptr);
    if (disp_log_buffer_active) {
      Ili9341PrintLogLine();
      DisplayLogBufferPtrInc();
    }
  }
}

void Ili9341Refresh()  // Every second
{
  if (Settings.display_mode) {  // Mode 0 is User text
    char tftdt[21];
    char disp_time[9];    // 13:45:43
    char disp_date4[11];  // 24-04-2017

    snprintf_P(disp_time, sizeof(disp_time), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
    snprintf_P(disp_date4, sizeof(disp_date4), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);

    tft->setTextSize(2);
    tft->setTextColor(ILI9341_YELLOW, ILI9341_BLACK);   // Add background color to solve flicker
    tft->setCursor(0, 0);
    snprintf_P(tftdt, sizeof(tftdt), PSTR("%s  %s"), disp_date4, disp_time);
    tft->print(tftdt);

    switch (Settings.display_mode) {
      case 1:  // Text
      case 2:  // Local
      case 3:  // Local
      case 4:  // Mqtt
      case 5:  // Mqtt
        Ili9341PrintLog();
        break;
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdsp04(byte function)
{
  boolean result = false;

  if (spi_flg) {
    if (FUNC_DISPLAY_INIT_DRIVER == function) {
      Ili9341InitDriver();
    }
    else if (XDSP_04 == Settings.display_model) {

      if (!dsp_color) { dsp_color = ILI9341_WHITE; }

      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_INIT:
          Ili9341Init(dsp_init);
          break;
        case FUNC_DISPLAY_POWER:
          Ili9341OnOff();
          break;
        case FUNC_DISPLAY_CLEAR:
          Ili9341Clear();
          break;
        case FUNC_DISPLAY_DRAW_HLINE:
          tft->writeFastHLine(dsp_x, dsp_y, dsp_len, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_VLINE:
          tft->writeFastVLine(dsp_x, dsp_y, dsp_len, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_LINE:
          tft->writeLine(dsp_x, dsp_y, dsp_x2, dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_CIRCLE:
          tft->drawCircle(dsp_x, dsp_y, dsp_rad, dsp_color);
          break;
        case FUNC_DISPLAY_FILL_CIRCLE:
          tft->fillCircle(dsp_x, dsp_y, dsp_rad, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_RECTANGLE:
          tft->drawRect(dsp_x, dsp_y, dsp_x2, dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_FILL_RECTANGLE:
          tft->fillRect(dsp_x, dsp_y, dsp_x2, dsp_y2, dsp_color);
          break;
//        case FUNC_DISPLAY_DRAW_FRAME:
//          oled->display();
//          break;
        case FUNC_DISPLAY_FONT_SIZE:
          tft->setTextSize(Settings.display_size);
          break;
        case FUNC_DISPLAY_DRAW_STRING:
          Ili9341DrawStringAt(dsp_x, dsp_y, dsp_str, dsp_color, dsp_flag);
          break;
        case FUNC_DISPLAY_ONOFF:
          Ili9341DisplayOnOff(dsp_on);
          break;
        case FUNC_DISPLAY_ROTATION:
          tft->setRotation(Settings.display_rotate);
          break;
#ifdef USE_DISPLAY_MODES1TO5
        case FUNC_DISPLAY_EVERY_SECOND:
          Ili9341Refresh();
          break;
#endif  // USE_DISPLAY_MODES1TO5
      }
    }
  }
  return result;
}

#endif  // USE_DISPLAY_ILI9341
#endif  // USE_DISPLAY
#endif  // USE_SPI
