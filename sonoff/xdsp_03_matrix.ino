/*
  xdsp_03_matrix.ino - Display 8x8 matrix support for Sonoff-Tasmota

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
#ifdef USE_DISPLAY_MATRIX

#define XDSP_03                    3

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>  // 8x8 Matrix

Adafruit_8x8matrix *matrix[8];
uint8_t mtx_matrices = 0;
uint8_t mtx_state = 0;
uint8_t mtx_counter = 0;
int16_t mtx_x = 0;
int16_t mtx_y = 0;

char mtx_buffer[80];
uint8_t mtx_mode = 0;
uint8_t mtx_loop = 0;

/*********************************************************************************************/

void MatrixWrite()
{
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->writeDisplay();
  }
}

void MatrixClear()
{
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->clear();
  }
  MatrixWrite();
}

void MatrixFixed(char* txt)
{
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->clear();
    matrix[i]->setCursor(-i *8, 0);
    matrix[i]->print(txt);
    matrix[i]->setBrightness(Settings.display_dimmer);
  }
  MatrixWrite();
}

void MatrixCenter(char* txt)
{
  int offset;

  int len = strlen(txt);
  offset = (len < 8) ? offset = ((mtx_matrices *8) - (len *6)) / 2 : 0;
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->clear();
    matrix[i]->setCursor(-(i *8)+offset, 0);
    matrix[i]->print(txt);
    matrix[i]->setBrightness(Settings.display_dimmer);
  }
  MatrixWrite();
}

void MatrixScrollLeft(char* txt, int loop)
{
  switch (mtx_state) {
  case 1:
    mtx_state = 2;
    // Horiz. position of text -- starts off right edge
    mtx_x = 8 * mtx_matrices;

    snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "[%s]"), txt);
    AddLog(LOG_LEVEL_DEBUG);

    disp_refresh = Settings.display_refresh;
  case 2:
  disp_refresh--;
    if (!disp_refresh) {
      disp_refresh = Settings.display_refresh;
      for (byte i = 0; i < mtx_matrices; i++) {
        matrix[i]->clear();
        matrix[i]->setCursor(mtx_x - i *8, 0);
        matrix[i]->print(txt);
        matrix[i]->setBrightness(Settings.display_dimmer);
      }
      MatrixWrite();
      // Move text position left by 1 pixel.
      mtx_x--;
      int16_t len = strlen(txt);
      if (mtx_x < -(len *6)) {
        mtx_state = loop;
      }
    }
    break;
  }
}

void MatrixScrollUp(char* txt, int loop)
{
  int wordcounter = 0;
  char tmpbuf[200];
  char *words[100];
//  char separators[] = " ,.;:!?";
//  char separators[] = " ";
//  char separators[] = " /|";
  char separators[] = " /";

  switch (mtx_state) {
  case 1:
    mtx_state = 2;
    // Vertical position of text -- starts off left bottom edge
    mtx_y = 8;
    mtx_counter = 0;
    disp_refresh = Settings.display_refresh;
  case 2:
    disp_refresh--;
    if (!disp_refresh) {
      disp_refresh = Settings.display_refresh;
      strlcpy(tmpbuf, txt, sizeof(tmpbuf));
      char *p = strtok(tmpbuf, separators);
      while (p != NULL && wordcounter < 40) {
        words[wordcounter++] = p;
        p = strtok(NULL, separators);
      }
      for (byte i = 0; i < mtx_matrices; i++) {
        matrix[i]->clear();
        for (byte j = 0; j < wordcounter; j++) {
          matrix[i]->setCursor(-i *8, mtx_y + (j *8));
          matrix[i]->println(words[j]);
        }
        matrix[i]->setBrightness(Settings.display_dimmer);
      }
      MatrixWrite();
      if (((mtx_y %8) == 0) && mtx_counter) {
        mtx_counter--;
      } else {
        mtx_y--;                   // Move text position up by 1 pixel.
        mtx_counter = STATES * 1;  // Hold text for 1 seconds
      }
      if (mtx_y < -(wordcounter *8)) {
        mtx_state = loop;
      }
    }
    break;
  }
}

/*********************************************************************************************/

void MatrixInitMode()
{
  for (byte i = 0; i < mtx_matrices; i++) {
    matrix[i]->setRotation(Settings.display_rotate);  // 1
    matrix[i]->setBrightness(Settings.display_dimmer);
    matrix[i]->blinkRate(0);               // 0 - 3
    matrix[i]->setTextWrap(false);         // Allow text to run off edges
//    matrix[i]->setTextSize(Settings.display_size);
//    matrix[i]->setTextColor(LED_RED);
    matrix[i]->cp437(true);
  }
  MatrixClear();
}

void MatrixInit(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
      MatrixInitMode();
      break;
    case DISPLAY_INIT_PARTIAL:
    case DISPLAY_INIT_FULL:
      break;
  }
}

void MatrixInitDriver()
{
  if (!Settings.display_model) {
    if (I2cDevice(Settings.display_address[1])) {
      Settings.display_model = XDSP_03;
    }
  }

  if (XDSP_03 == Settings.display_model) {
    mtx_state = 1;
    for (mtx_matrices = 0; mtx_matrices < 8; mtx_matrices++) {
      if (Settings.display_address[mtx_matrices]) {
        matrix[mtx_matrices] = new Adafruit_8x8matrix();
        matrix[mtx_matrices]->begin(Settings.display_address[mtx_matrices]);
      } else {
        break;
      }
    }

    MatrixInitMode();
  }
}

void MatrixOnOff()
{
  if (!disp_power) {
    MatrixClear();
  }
}

void MatrixDrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag)
{
  snprintf(mtx_buffer, sizeof(mtx_buffer), str);
  mtx_mode = x;
  mtx_loop = y;
  if (!mtx_state) { mtx_state = 1; }
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void MatrixBufferScroll(uint8_t direction)
{
  if (disp_log_buffer_idx != disp_log_buffer_ptr) {
    if (!mtx_state) {
      mtx_state = 1;
    }

    char *pch = strchr(disp_log_buffer[disp_log_buffer_ptr],'~');  // = 0x7E (~) Replace degrees character (276 octal)
    if (pch != NULL) {
      disp_log_buffer[disp_log_buffer_ptr][pch - disp_log_buffer[disp_log_buffer_ptr]] = '\370';  // = 0xF8
    }

    if (direction) {
      MatrixScrollUp(disp_log_buffer[disp_log_buffer_ptr], 0);
    } else {
      MatrixScrollLeft(disp_log_buffer[disp_log_buffer_ptr], 0);
    }
    if (!mtx_state) {
      DisplayLogBufferPtrInc();
    }
  } else {
    char disp_time[9];    // 13:45:43

    snprintf_P(disp_time, sizeof(disp_time), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
    MatrixFixed(disp_time);
  }
}

#endif  // USE_DISPLAY_MODES1TO5

void MatrixRefresh()  // Every second
{
  if (disp_power) {
    switch (Settings.display_mode) {
      case 0: {
        switch (mtx_mode) {
          case 0:
            MatrixScrollLeft(mtx_buffer, mtx_loop);
            break;
          case 1:
            MatrixScrollUp(mtx_buffer, mtx_loop);
            break;
        }
        break;
      }
#ifdef USE_DISPLAY_MODES1TO5
      case 2: {
        char disp_date[9];    // 24-04-17
        snprintf_P(disp_date, sizeof(disp_date), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%02d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year -2000);
        MatrixFixed(disp_date);
        break;
      }
      case 3: {
        char disp_day[10];    // Mon
        snprintf_P(disp_day, sizeof(disp_day), PSTR("%d %s"), RtcTime.day_of_month, RtcTime.name_of_month);
        MatrixCenter(disp_day);
        break;
      }
      case 4:
        MatrixBufferScroll(0);
        break;
      case 1:  // Time and user text
      case 5:  // Time, user text and MQTT
        MatrixBufferScroll(1);
        break;
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

boolean Xdsp03(byte function)
{
  boolean result = false;

  if (i2c_flg) {
    if (FUNC_DISPLAY_INIT_DRIVER == function) {
      MatrixInitDriver();
    }
    else if (XDSP_03 == Settings.display_model) {
      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_INIT:
          MatrixInit(dsp_init);
          break;
        case FUNC_DISPLAY_EVERY_50_MSECOND:
          MatrixRefresh();
          break;
        case FUNC_DISPLAY_POWER:
          MatrixOnOff();
          break;
        case FUNC_DISPLAY_DRAW_STRING:
          MatrixDrawStringAt(dsp_x, dsp_y, dsp_str, dsp_color, dsp_flag);
          break;
      }
    }
  }
  return result;
}

#endif  // USE_DISPLAY_MATRIX
#endif  // USE_DISPLAY
#endif  // USE_I2C
