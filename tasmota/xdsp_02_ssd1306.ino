/*
  xdsp_02_ssd1306.ino - Display Oled SSD1306 support for Tasmota

  Copyright (C) 2020  Theo Arends and Adafruit

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
#define XI2C_04                4            // See I2CDEVICES.md

#define OLED_RESET 4

#define SPRINT(A) char str[32];sprintf(str,"val: %d ",A);Serial.println((char*)str);

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

extern uint8_t *buffer;

/*********************************************************************************************/

void SSD1306InitDriver(void)
{
  if (!Settings.display_model) {
    if (I2cSetDevice(OLED_ADDRESS1)) {
      Settings.display_address[0] = OLED_ADDRESS1;
      Settings.display_model = XDSP_02;
    }
    else if (I2cSetDevice(OLED_ADDRESS2)) {
      Settings.display_address[0] = OLED_ADDRESS2;
      Settings.display_model = XDSP_02;
    }
  }

  if (XDSP_02 == Settings.display_model) {
    I2cSetActiveFound(Settings.display_address[0], "SSD1306");

    if ((Settings.display_width != 64) && (Settings.display_width != 96) && (Settings.display_width != 128)) {
      Settings.display_width = 128;
    }
    if ((Settings.display_height != 16) && (Settings.display_height != 32) && (Settings.display_height != 48) && (Settings.display_height != 64)) {
      Settings.display_height = 64;
    }

    uint8_t reset_pin = -1;
    if (PinUsed(GPIO_OLED_RESET)) {
      reset_pin = Pin(GPIO_OLED_RESET);
    }

    // allocate screen buffer
    if (buffer) { free(buffer); }
    buffer = (unsigned char*)calloc((Settings.display_width * Settings.display_height) / 8,1);
    if (!buffer) { return; }

    // init renderer
    // oled1306 = new Adafruit_SSD1306(SSD1306_LCDWIDTH,SSD1306_LCDHEIGHT);
    oled1306 = new Adafruit_SSD1306(Settings.display_width, Settings.display_height, &Wire, reset_pin);
    oled1306->begin(SSD1306_SWITCHCAPVCC, Settings.display_address[0], reset_pin >= 0);
    renderer = oled1306;
    renderer->DisplayInit(DISPLAY_INIT_MODE, Settings.display_size, Settings.display_rotate, Settings.display_font);
    renderer->setTextColor(1,0);

#ifdef SHOW_SPLASH
    renderer->setTextFont(0);
    renderer->setTextSize(2);
    renderer->setCursor(20,20);
    renderer->println(F("SSD1306"));
    renderer->Updateframe();
    renderer->DisplayOnff(1);
#endif

  }
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

      renderer->clearDisplay();
      renderer->setTextSize(Settings.display_size);
      renderer->setCursor(0,0);
      for (byte i = 0; i < last_row; i++) {
        strlcpy(disp_screen_buffer[i], disp_screen_buffer[i +1], disp_screen_buffer_cols);
        renderer->println(disp_screen_buffer[i]);
      }
      strlcpy(disp_screen_buffer[last_row], txt, disp_screen_buffer_cols);
      DisplayFillScreen(last_row);

      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "[%s]"), disp_screen_buffer[last_row]);

      renderer->println(disp_screen_buffer[last_row]);
      renderer->Updateframe();
    }
  }
}

void Ssd1306Time(void)
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

void Ssd1306Refresh(void)  // Every second
{
  if (!renderer) return;

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

bool Xdsp02(byte function)
{
  if (!I2cEnabled(XI2C_04)) { return false; }

  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    SSD1306InitDriver();
  }
  else if (XDSP_02 == Settings.display_model) {
    switch (function) {
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        Ssd1306Refresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
    }
  }
  return result;
}

#endif  // USE_DISPLAY_SSD1306
#endif  // USE_DISPLAY
#endif  // USE_I2C
