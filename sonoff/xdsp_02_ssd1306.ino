/*
  xdsp_02_ssd1306.ino - Display Oled ssd1306 support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends and Adafruit

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

#ifdef USE_I2C
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_SSD1306

#define XDSP_02                2

#define OLED_ADDRESS1          0x3C         // Oled 128x32 I2C address
#define OLED_ADDRESS2          0x3D         // Oled 128x64 I2C address

#define OLED_FONT_WIDTH        6
#define OLED_FONT_HEIGTH       8

//#define OLED_BUFFER_COLS       21 or 11   // Max number of columns in display shadow buffer
//#define OLED_BUFFER_ROWS        8 or 16   // Max number of lines in display shadow buffer

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 *oled;

uint8_t ssd1306_font_x = OLED_FONT_WIDTH;
uint8_t ssd1306_font_y = OLED_FONT_HEIGTH;

/*********************************************************************************************/

void Ssd1306InitMode(void)
{
  oled->setRotation(Settings.display_rotate);  // 0
  oled->invertDisplay(false);
  oled->clearDisplay();
  oled->setTextWrap(false);         // Allow text to run off edges
  oled->cp437(true);

  oled->setTextSize(Settings.display_size);
  oled->setTextColor(WHITE);
  oled->setCursor(0,0);
  oled->display();
}

void Ssd1306Init(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
      Ssd1306InitMode();
#ifdef USE_DISPLAY_MODES1TO5
      DisplayClearScreenBuffer();
#endif  // USE_DISPLAY_MODES1TO5
      break;
    case DISPLAY_INIT_PARTIAL:
    case DISPLAY_INIT_FULL:
      break;
  }
}

void Ssd1306InitDriver(void)
{
  if (!Settings.display_model) {
    if (I2cDevice(OLED_ADDRESS1)) {
      Settings.display_address[0] = OLED_ADDRESS1;
      Settings.display_model = XDSP_02;
    }
    else if (I2cDevice(OLED_ADDRESS2)) {
      Settings.display_address[0] = OLED_ADDRESS2;
      Settings.display_model = XDSP_02;
    }
  }

  if (XDSP_02 == Settings.display_model) {
    oled = new Adafruit_SSD1306();
    oled->begin(SSD1306_SWITCHCAPVCC, Settings.display_address[0]);

#ifdef USE_DISPLAY_MODES1TO5
    DisplayAllocScreenBuffer();
#endif  // USE_DISPLAY_MODES1TO5

    Ssd1306InitMode();
  }
}

void Ssd1306Clear(void)
{
  oled->clearDisplay();
  oled->setCursor(0, 0);
  oled->display();
}

void Ssd1306DrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag)
{
  if (!flag) {
    oled->setCursor(x, y);
  } else {
    oled->setCursor((x-1) * ssd1306_font_x * Settings.display_size, (y-1) * ssd1306_font_y * Settings.display_size);
  }
  oled->println(str);
}

void Ssd1306DisplayOnOff(uint8_t on)
{
  if (on) {
    oled->ssd1306_command(SSD1306_DISPLAYON);
  } else {
    oled->ssd1306_command(SSD1306_DISPLAYOFF);
  }
}

void Ssd1306OnOff(void)
{
  Ssd1306DisplayOnOff(disp_power);
  oled->display();
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void Ssd1306PrintLog(void)
{
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    if (!disp_screen_buffer_cols) { DisplayAllocScreenBuffer(); }

    char* txt = DisplayLogBuffer('\370');
    if (txt != NULL) {
      uint8_t last_row = Settings.display_rows -1;

      oled->clearDisplay();
      oled->setTextSize(Settings.display_size);
      oled->setCursor(0,0);
      for (uint8_t i = 0; i < last_row; i++) {
        strlcpy(disp_screen_buffer[i], disp_screen_buffer[i +1], disp_screen_buffer_cols);
        oled->println(disp_screen_buffer[i]);
      }
      strlcpy(disp_screen_buffer[last_row], txt, disp_screen_buffer_cols);
      DisplayFillScreen(last_row);

      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "[%s]"), disp_screen_buffer[last_row]);

      oled->println(disp_screen_buffer[last_row]);
      oled->display();
    }
  }
}

void Ssd1306Time(void)
{
  char line[12];

  oled->clearDisplay();
  oled->setTextSize(2);
  oled->setCursor(0, 0);
  snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);  // [ 12:34:56 ]
  oled->println(line);
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);   // [01-02-2018]
  oled->println(line);
  oled->display();
}

void Ssd1306Refresh(void)  // Every second
{
  if (Settings.display_mode) {  // Mode 0 is User text
    switch (Settings.display_mode) {
      case 1:  // Time
        Ssd1306Time();
        break;
      case 2:  // Local
      case 3:  // Local
      case 4:  // Mqtt
      case 5:  // Mqtt
        Ssd1306PrintLog();
        break;
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp02(uint8_t function)
{
  bool result = false;

  if (i2c_flg) {
    if (FUNC_DISPLAY_INIT_DRIVER == function) {
      Ssd1306InitDriver();
    }
    else if (XDSP_02 == Settings.display_model) {

      if (!dsp_color) { dsp_color = WHITE; }

      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_INIT:
          Ssd1306Init(dsp_init);
          break;
        case FUNC_DISPLAY_POWER:
          Ssd1306OnOff();
          break;
        case FUNC_DISPLAY_CLEAR:
          Ssd1306Clear();
          break;
        case FUNC_DISPLAY_DRAW_HLINE:
          oled->writeFastHLine(dsp_x, dsp_y, dsp_len, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_VLINE:
          oled->writeFastVLine(dsp_x, dsp_y, dsp_len, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_LINE:
          oled->writeLine(dsp_x, dsp_y, dsp_x2, dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_CIRCLE:
          oled->drawCircle(dsp_x, dsp_y, dsp_rad, dsp_color);
          break;
        case FUNC_DISPLAY_FILL_CIRCLE:
          oled->fillCircle(dsp_x, dsp_y, dsp_rad, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_RECTANGLE:
          oled->drawRect(dsp_x, dsp_y, dsp_x2, dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_FILL_RECTANGLE:
          oled->fillRect(dsp_x, dsp_y, dsp_x2, dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_FRAME:
          oled->display();
          break;
        case FUNC_DISPLAY_TEXT_SIZE:
          oled->setTextSize(Settings.display_size);
          break;
        case FUNC_DISPLAY_FONT_SIZE:
//          oled->setTextSize(Settings.display_font);
          break;
        case FUNC_DISPLAY_DRAW_STRING:
          Ssd1306DrawStringAt(dsp_x, dsp_y, dsp_str, dsp_color, dsp_flag);
          break;
        case FUNC_DISPLAY_ONOFF:
          Ssd1306DisplayOnOff(dsp_on);
          break;
        case FUNC_DISPLAY_ROTATION:
          oled->setRotation(Settings.display_rotate);
          break;
#ifdef USE_DISPLAY_MODES1TO5
        case FUNC_DISPLAY_EVERY_SECOND:
          Ssd1306Refresh();
          break;
#endif  // USE_DISPLAY_MODES1TO5
      }
    }
  }
  return result;
}

#endif  // USE_DISPLAY_SSD1306
#endif  // USE_DISPLAY
#endif  // USE_I2C
