/*
  xdsp_01_lcd.ino - Display LCD support for Sonoff-Tasmota

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
#ifdef USE_DISPLAY_LCD

#define XDSP_01                1

#define LCD_ADDRESS1           0x27         // LCD I2C address option 1
#define LCD_ADDRESS2           0x3F         // LCD I2C address option 2

#define LCD_BUFFER_COLS        40           // Max number of columns in display shadow buffer
#define LCD_BUFFER_ROWS        8            // Max number of lines in display shadow buffer

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C *lcd;

/*********************************************************************************************/

void LcdInitMode()
{
  lcd->init();
  lcd->clear();
//  memset(lcd_screen_buffer[Settings.display_rows -1], 0x20, Settings.display_cols[0]);
//  lcd_screen_buffer[Settings.display_rows -1][Settings.display_cols[0]] = 0;
}

void LcdInit(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
      LcdInitMode();
      break;
    case DISPLAY_INIT_PARTIAL:
    case DISPLAY_INIT_FULL:
      break;
  }
}

void LcdInitDriver()
{
  if (!Settings.display_model) {
    if (I2cDevice(LCD_ADDRESS1)) {
      Settings.display_address[0] = LCD_ADDRESS1;
      Settings.display_model = XDSP_01;
    }
    else if (I2cDevice(LCD_ADDRESS2)) {
      Settings.display_address[0] = LCD_ADDRESS2;
      Settings.display_model = XDSP_01;
    }
  }

  if (XDSP_01 == Settings.display_model) {
    lcd = new LiquidCrystal_I2C(Settings.display_address[0], Settings.display_cols[0], Settings.display_rows);

    LcdInitMode();
  }
}

void LcdDrawStringAt()
{
  lcd->setCursor(dsp_x, dsp_y);
  lcd->print(dsp_str);
}

void LcdDisplayOnOff(uint8_t on)
{
  if (on) {
    lcd->backlight();
  } else {
    lcd->noBacklight();
  }
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

char lcd_screen_buffer[LCD_BUFFER_ROWS][LCD_BUFFER_COLS +1];

void LcdCenter(byte row, char* txt)
{
  int offset;
  int len;
  char line[Settings.display_cols[0] +2];

  memset(line, 0x20, Settings.display_cols[0]);
  line[Settings.display_cols[0]] = 0;
  len = strlen(txt);
  offset = (len < Settings.display_cols[0]) ? offset = (Settings.display_cols[0] - len) / 2 : 0;
  strncpy(line +offset, txt, len);
  lcd->setCursor(0, row);
  lcd->print(line);
}

void LcdPrintLogLine()
{
  uint8_t last_row = Settings.display_rows -1;

  for (byte i = 0; i < last_row; i++) {
    strlcpy(lcd_screen_buffer[i], lcd_screen_buffer[i +1], sizeof(lcd_screen_buffer[i]));
    lcd->setCursor(0, i);            // Col 0, Row i
    lcd->print(lcd_screen_buffer[i +1]);
  }

  char *pch = strchr(disp_log_buffer[disp_log_buffer_ptr],'~');  // = 0x7E (~) Replace degrees character (276 octal)
  if (pch != NULL) {
    disp_log_buffer[disp_log_buffer_ptr][pch - disp_log_buffer[disp_log_buffer_ptr]] = '\337';  // = 0xDF
  }
  strlcpy(lcd_screen_buffer[last_row], disp_log_buffer[disp_log_buffer_ptr], sizeof(lcd_screen_buffer[last_row]));

  // Fill with spaces
  byte len = sizeof(lcd_screen_buffer[last_row]) - strlen(lcd_screen_buffer[last_row]);
  if (len) {
    memset(lcd_screen_buffer[last_row] + strlen(lcd_screen_buffer[last_row]), 0x20, len);
    lcd_screen_buffer[last_row][sizeof(lcd_screen_buffer[last_row])-1] = 0;
  }

  snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "[%s]"), lcd_screen_buffer[last_row]);
  AddLog(LOG_LEVEL_DEBUG);

  lcd->setCursor(0, last_row);
  lcd->print(lcd_screen_buffer[last_row]);
}

void LcdPrintLog()
{
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    disp_log_buffer_active = (disp_log_buffer_idx != disp_log_buffer_ptr);
    if (disp_log_buffer_active) {
      LcdPrintLogLine();
      DisplayLogBufferPtrInc();
    }
  }
}

void LcdTime()
{
  char line[Settings.display_cols[0] +1];

  snprintf_P(line, sizeof(line), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
  LcdCenter(0, line);
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);
  LcdCenter(1, line);
}

void LcdRefresh()  // Every second
{
  if (Settings.display_mode) {  // Mode 0 is User text
    switch (Settings.display_mode) {
      case 1:  // Time
        LcdTime();
        break;
      case 2:  // Local
      case 4:  // Mqtt
        LcdPrintLog();
        break;
      case 3:  // Local
      case 5: {  // Mqtt
        LcdPrintLog();
        if (!disp_log_buffer_active) {
          LcdTime();
        }
        break;
      }
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdsp01(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    if (FUNC_DISPLAY_INIT_DRIVER == function) {
      LcdInitDriver();
    }
    else if (XDSP_01 == Settings.display_model) {
      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_INIT:
          LcdInit(dsp_init);
          break;
        case FUNC_DISPLAY_POWER:
          LcdDisplayOnOff(disp_power);
          break;
        case FUNC_DISPLAY_CLEAR:
          lcd->clear();
          break;
//        case FUNC_DISPLAY_DRAW_HLINE:
//          break;
//        case FUNC_DISPLAY_DRAW_VLINE:
//          break;
//        case FUNC_DISPLAY_DRAW_CIRCLE:
//          break;
//        case FUNC_DISPLAY_FILL_CIRCLE:
//          break;
//        case FUNC_DISPLAY_DRAW_RECTANGLE:
//          break;
//        case FUNC_DISPLAY_FILL_RECTANGLE:
//          break;
//        case FUNC_DISPLAY_DRAW_FRAME:
//          break;
//        case FUNC_DISPLAY_FONT_SIZE:
//          break;
        case FUNC_DISPLAY_DRAW_STRING:
          LcdDrawStringAt();
          break;
        case FUNC_DISPLAY_ONOFF:
          LcdDisplayOnOff(dsp_on);
          break;
//        case FUNC_DISPLAY_ROTATION:
//          break;
#ifdef USE_DISPLAY_MODES1TO5
        case FUNC_DISPLAY_EVERY_SECOND:
          LcdRefresh();
          break;
#endif  // USE_DISPLAY_MODES1TO5
      }
    }
  }
  return result;
}

#endif  // USE_DISPLAY_LCD
#endif  // USE_DISPLAY
#endif  // USE_I2C
