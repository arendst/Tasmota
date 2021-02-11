/*
  xdsp_07_SH1106.ino - Display Oled SH1106 support for Tasmota

  Copyright (C) 2021  Theo Arends and Adafruit

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
#ifdef USE_DISPLAY_SH1106

#define OLED_RESET 4

#define SPRINT(A) char str[32];sprintf(str,"val: %d ",A);Serial.println((char*)str);

extern uint8_t *buffer;

#define XDSP_07                7
#define XI2C_06                6            // See I2CDEVICES.md

#define OLED_ADDRESS1          0x3C         // Oled 128x32 I2C address
#define OLED_ADDRESS2          0x3D         // Oled 128x64 I2C address

#define OLED_BUFFER_COLS       40           // Max number of columns in display shadow buffer
#define OLED_BUFFER_ROWS       16           // Max number of lines in display shadow buffer

#define OLED_FONT_WIDTH        6
#define OLED_FONT_HEIGTH       8

#include <Wire.h>
#include <renderer.h>
#include <Adafruit_SH1106.h>

Adafruit_SH1106 *oled1106;

/*********************************************************************************************/


void SH1106InitDriver()
{
  if (!Settings.display_model) {
    if (I2cSetDevice(OLED_ADDRESS1)) {
      Settings.display_address[0] = OLED_ADDRESS1;
      Settings.display_model = XDSP_07;
    }
    else if (I2cSetDevice(OLED_ADDRESS2)) {
      Settings.display_address[0] = OLED_ADDRESS2;
      Settings.display_model = XDSP_07;
    }
  }

  if (XDSP_07 == Settings.display_model) {
    I2cSetActiveFound(Settings.display_address[0], "SH1106");

    if (Settings.display_width != SH1106_LCDWIDTH) {
      Settings.display_width = SH1106_LCDWIDTH;
    }
    if (Settings.display_height != SH1106_LCDHEIGHT) {
      Settings.display_height = SH1106_LCDHEIGHT;
    }

    // allocate screen buffer
    if (buffer) free(buffer);
    buffer=(unsigned char*)calloc((SH1106_LCDWIDTH * SH1106_LCDHEIGHT) / 8,1);
    if (!buffer) return;

    // init renderer
    oled1106 = new Adafruit_SH1106(SH1106_LCDWIDTH,SH1106_LCDHEIGHT);
    renderer=oled1106;
    renderer->Begin(SH1106_SWITCHCAPVCC, Settings.display_address[0],0);
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);
    renderer->setTextColor(1,0);

#ifdef SHOW_SPLASH
    renderer->setTextFont(0);
    renderer->setTextSize(2);
    renderer->setCursor(20,20);
    renderer->println(F("SH1106"));
    renderer->Updateframe();
    renderer->DisplayOnff(1);
#endif
  }
}


/*********************************************************************************************/
#ifdef USE_DISPLAY_MODES1TO5

void SH1106PrintLog(void)
{
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    if (!disp_screen_buffer_cols) { DisplayAllocScreenBuffer(); }

    char* txt = DisplayLogBuffer('\370');
    if (txt != NULL) {
      uint8_t last_row = Settings.display_rows -1;

      renderer->clearDisplay();
      renderer->setTextSize(Settings.display_size);
      renderer->setCursor(0,0);
      for (byte i = 0; i < last_row; i++) {
        strlcpy(disp_screen_buffer[i], disp_screen_buffer[i +1], disp_screen_buffer_cols);
        renderer->println(disp_screen_buffer[i]);
      }
      strlcpy(disp_screen_buffer[last_row], txt, disp_screen_buffer_cols);
      DisplayFillScreen(last_row);

      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "[%s]"), disp_screen_buffer[last_row]);

      renderer->println(disp_screen_buffer[last_row]);
      renderer->Updateframe();
    }
  }
}

void SH1106Time(void)
{
  char line[12];

  renderer->clearDisplay();
  renderer->setTextSize(Settings.display_size);
  renderer->setTextFont(Settings.display_font);
  renderer->setCursor(0, 0);
  snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);  // [ 12:34:56 ]
  renderer->println(line);
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);   // [01-02-2018]
  renderer->println(line);
  renderer->Updateframe();
}

void SH1106Refresh(void)  // Every second
{
  if (!renderer) return;
  if (Settings.display_mode) {  // Mode 0 is User text
    switch (Settings.display_mode) {
      case 1:  // Time
        SH1106Time();
        break;
      case 2:  // Local
      case 3:  // Local
      case 4:  // Mqtt
      case 5:  // Mqtt
        SH1106PrintLog();
        break;
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp07(uint8_t function)
{
  if (!I2cEnabled(XI2C_06)) { return false; }

  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    SH1106InitDriver();
  }
  else if (XDSP_07 == Settings.display_model) {

    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        SH1106Refresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
  return result;
}

#endif  // USE_DISPLAY_SH1106
#endif  // USE_DISPLAY
#endif  // USE_I2C
