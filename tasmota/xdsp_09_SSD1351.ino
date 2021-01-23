/*
  xdsp_09_SSD1351.ino - Display SSD1351 support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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
#ifdef USE_DISPLAY_SSD1351

#define XDSP_09                9

#define COLORED                1
#define UNCOLORED              0

// uses about 1.9k flash + renderer class
// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT

#include <SSD1351.h>

bool ssd1351_init_done = false;
extern uint8_t *buffer;
extern uint8_t color_type;
SSD1351 *ssd1351;

/*********************************************************************************************/

void SSD1351_InitDriver() {
  if (PinUsed(GPIO_SSD1351_CS) &&
     ((TasmotaGlobal.soft_spi_enabled & SPI_MOSI) || (TasmotaGlobal.spi_enabled & SPI_MOSI))) {

    Settings.display_model = XDSP_09;

    if (Settings.display_width != SSD1351_WIDTH) {
      Settings.display_width = SSD1351_WIDTH;
    }
    if (Settings.display_height != SSD1351_HEIGHT) {
      Settings.display_height = SSD1351_HEIGHT;
    }

    buffer = 0;

    // default colors
    fg_color = SSD1351_WHITE;
    bg_color = SSD1351_BLACK;

    // init renderer
    if (TasmotaGlobal.soft_spi_enabled){
      ssd1351 = new SSD1351(Pin(GPIO_SSD1351_CS), Pin(GPIO_SSPI_MOSI), Pin(GPIO_SSPI_SCLK));
    }
    else if (TasmotaGlobal.spi_enabled) {
      ssd1351 = new SSD1351(Pin(GPIO_SSD1351_CS), Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_CLK));
    }

    delay(100);
    ssd1351->begin();
    renderer = ssd1351;
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);
    renderer->dim(Settings.display_dimmer);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->setTextColor(SSD1351_WHITE,SSD1351_BLACK);
    renderer->DrawStringAt(10, 60, "SSD1351", SSD1351_RED,0);
    delay(1000);

#endif
    color_type = COLOR_COLOR;

    ssd1351_init_done = true;
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: SSD1351"));
  }
}

#ifdef USE_DISPLAY_MODES1TO5

void SSD1351PrintLog(void) {
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

void SSD1351Time(void) {
  char line[12];

  renderer->clearDisplay();
  renderer->setTextSize(2);
  renderer->setCursor(0, 0);
  snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);  // [ 12:34:56 ]
  renderer->println(line);
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);   // [01-02-2018]
  renderer->println(line);
  renderer->Updateframe();
}

void SSD1351Refresh(void) {     // Every second
  if (Settings.display_mode) {  // Mode 0 is User text
    switch (Settings.display_mode) {
      case 1:  // Time
        SSD1351Time();
        break;
      case 2:  // Local
      case 3:  // Local
      case 4:  // Mqtt
      case 5:  // Mqtt
        SSD1351PrintLog();
        break;
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp09(uint8_t function) {
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
      SSD1351_InitDriver();
  }
  else if (ssd1351_init_done && (XDSP_09 == Settings.display_model)) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        SSD1351Refresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
  return result;
}
#endif  // USE_DISPLAY_SSD1351
#endif  // USE_DISPLAY
#endif  // USE_SPI
