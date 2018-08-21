/*
  xdsp_02_ssd1306.ino - Display Oled ssd1306 support for Sonoff-Tasmota

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

#ifdef USE_I2C
#ifdef USE_DISPLAY
#ifdef USE_DISPLAY_SSD1306

#define XDSP_02                2

#define OLED_ADDRESS1          0x3C         // Oled 128x32 I2C address
#define OLED_ADDRESS2          0x3D         // Oled 128x64 I2C address

#define OLED_BUFFER_COLS       40           // Max number of columns in display shadow buffer
#define OLED_BUFFER_ROWS       16           // Max number of lines in display shadow buffer

#define OLED_FONT_WIDTH        6
#define OLED_FONT_HEIGTH       8

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 *oled;

uint8_t ssd1306_font_x = OLED_FONT_WIDTH;
uint8_t ssd1306_font_y = OLED_FONT_HEIGTH;

/*********************************************************************************************/

void Ssd1306InitMode()
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
      break;
    case DISPLAY_INIT_PARTIAL:
    case DISPLAY_INIT_FULL:
      break;
  }
}

void Ssd1306InitDriver()
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

    Ssd1306InitMode();
  }
}

void Ssd1306Clear()
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

void Ssd1306OnOff()
{
  Ssd1306DisplayOnOff(disp_power);
  oled->display();
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

char oled_screen_buffer[OLED_BUFFER_ROWS][OLED_BUFFER_COLS +1];

void Ssd1306PrintLogLine()
{
  uint8_t last_row = Settings.display_rows -1;

  oled->clearDisplay();
  oled->setTextSize(Settings.display_size);
  oled->setCursor(0,0);
  for (byte i = 0; i < last_row; i++) {
    strlcpy(oled_screen_buffer[i], oled_screen_buffer[i +1], sizeof(oled_screen_buffer[i]));
    oled->println(oled_screen_buffer[i]);
  }

  char *pch = strchr(disp_log_buffer[disp_log_buffer_ptr],'~');  // = 0x7E (~) Replace degrees character (276 octal)
  if (pch != NULL) {
    disp_log_buffer[disp_log_buffer_ptr][pch - disp_log_buffer[disp_log_buffer_ptr]] = '\370';  // = 0xF8
  }
  strlcpy(oled_screen_buffer[last_row], disp_log_buffer[disp_log_buffer_ptr], sizeof(oled_screen_buffer[last_row]));

  // Fill with spaces
  byte len = sizeof(oled_screen_buffer[last_row]) - strlen(oled_screen_buffer[last_row]);
  if (len) {
    memset(oled_screen_buffer[last_row] + strlen(oled_screen_buffer[last_row]), 0x20, len);
    oled_screen_buffer[last_row][sizeof(oled_screen_buffer[last_row])-1] = 0;
  }

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "[%s]"), oled_screen_buffer[last_row]);
  AddLog(LOG_LEVEL_DEBUG);

  oled->println(oled_screen_buffer[last_row]);
  oled->display();
}

void Ssd1306PrintLog()
{
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    disp_log_buffer_active = (disp_log_buffer_idx != disp_log_buffer_ptr);
    if (disp_log_buffer_active) {
      Ssd1306PrintLogLine();
      DisplayLogBufferPtrInc();
    }
  }
}

void Ssd1306Time()
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

void Ssd1306Refresh()  // Every second
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

boolean Xdsp02(byte function)
{
  boolean result = false;

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
        case FUNC_DISPLAY_FONT_SIZE:
          oled->setTextSize(Settings.display_size);
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
