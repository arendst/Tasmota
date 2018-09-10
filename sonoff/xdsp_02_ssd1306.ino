/*
  xdsp_02_SSD1306.ino - Display Oled SSD1306 support for Sonoff-Tasmota

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

#define OLED_RESET 4

#define SPRINT(A) char str[32];sprintf(str,"val: %d ",A);Serial.println((char*)str);

extern uint8_t *buffer;

#define XDSP_02                2

#define OLED_ADDRESS1          0x3C         // Oled 128x32 I2C address
#define OLED_ADDRESS2          0x3D         // Oled 128x64 I2C address

#define OLED_BUFFER_COLS       40           // Max number of columns in display shadow buffer
#define OLED_BUFFER_ROWS       16           // Max number of lines in display shadow buffer

#define OLED_FONT_WIDTH        6
#define OLED_FONT_HEIGTH       8

#include <Wire.h>
#include <renderer.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 *oled1306;

/*********************************************************************************************/


void SSD1306InitDriver()
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

    // allocate screen buffer
    if (buffer) free(buffer);
    buffer=(unsigned char*)calloc((SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT) / 8,1);
    if (!buffer) return;

    // init renderer
    oled1306 = new Adafruit_SSD1306(SSD1306_LCDWIDTH,SSD1306_LCDHEIGHT);
    renderer=oled1306;
    renderer->Begin(SSD1306_SWITCHCAPVCC, Settings.display_address[0],0);
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);

#ifdef SHOW_SPLASH
    renderer->setTextSize(2);
    renderer->setCursor(20,20);
    renderer->println((char*)(const char*)"Adafruit");
    renderer->Updateframe();
    renderer->DisplayOnff(1);
#endif
  }
}


/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

char oled1306_screen_buffer[OLED_BUFFER_ROWS][OLED_BUFFER_COLS +1];

void SSD1306PrintLogLine()
{
  uint8_t last_row = Settings.display_rows -1;

  //oled->clearDisplay();
  renderer->fillScreen(BLACK);
  renderer->setTextSize(Settings.display_size);
  renderer->setCursor(0,0);
  for (byte i = 0; i < last_row; i++) {
    strlcpy(oled_screen_buffer[i], oled_screen_buffer[i +1], sizeof(oled_screen_buffer[i]));
    renderer->println(oled_screen_buffer[i]);
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

  renderer->println(oled_screen_buffer[last_row]);
  renderer->Updateframe();
}

void SSD1306PrintLog()
{
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    disp_log_buffer_active = (disp_log_buffer_idx != disp_log_buffer_ptr);
    if (disp_log_buffer_active) {
      SSD1306PrintLogLine();
      DisplayLogBufferPtrInc();
    }
  }
}

void SSD1306Time()
{
  char line[12];

  //oled->clearDisplay();
  renderer->fillScreen(BLACK);
  renderer->setTextSize(2);
  renderer->setCursor(0, 0);
  snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);  // [ 12:34:56 ]
  renderer->println(line);
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);   // [01-02-2018]
  renderer->println(line);
  renderer->Updateframe();
}

void SSD1306Refresh()  // Every second
{
  if (Settings.display_mode) {  // Mode 0 is User text
    switch (Settings.display_mode) {
      case 1:  // Time
        SSD1306Time();
        break;
      case 2:  // Local
      case 3:  // Local
      case 4:  // Mqtt
      case 5:  // Mqtt
        SSD1306PrintLog();
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
      SSD1306InitDriver();
    }
    else if (XDSP_02 == Settings.display_model) {

      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
#ifdef USE_DISPLAY_MODES1TO5
        case FUNC_DISPLAY_EVERY_SECOND:
          SSD1306Refresh();
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
