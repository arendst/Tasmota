/*
  xdsp_14_SSD1331.ino - Display SSD1331 support for Tasmota

  Copyright (C) 2021  Jeroen Vermeulen, Gerhard Mutz and Theo Arends

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
#ifdef USE_DISPLAY_SSD1331 // This driver eats 5.3 K flash

#define XDSP_14                14

#define COLORED                1
#define UNCOLORED              0
#define USE_TINY_FONT

#define SSD1331_BLACK       0x0000      //   0,   0,   0
#define SSD1331_WHITE       0xFFFF      // 255, 255, 255
#define SSD1331_RED         0xF800      // 255,   0,   0
#define SSD1331_BLUE        0x001F      //   0,   0, 255

#include <Adafruit_SSD1331.h>
#include <SPI.h>

bool ssd1331_init_done = false;

extern uint8_t color_type;
Adafruit_SSD1331 *ssd1331;

/*********************************************************************************************/

void SSD1331_InitDriver() {
  if (PinUsed(GPIO_SSD1331_CS) && PinUsed(GPIO_SSD1331_DC) &&
     ((TasmotaGlobal.soft_spi_enabled & SPI_MOSI) || (TasmotaGlobal.spi_enabled & SPI_MOSI))) {

    Settings->display_model = XDSP_14;

    if (Settings->display_width != Adafruit_SSD1331::TFTWIDTH) {
      Settings->display_width = Adafruit_SSD1331::TFTWIDTH;
    }
    if (Settings->display_height != Adafruit_SSD1331::TFTHEIGHT) {
      Settings->display_height = Adafruit_SSD1331::TFTHEIGHT;
    }


    // default colors
    fg_color = SSD1331_WHITE;
    bg_color = SSD1331_BLACK;

    // init renderer
    if (TasmotaGlobal.soft_spi_enabled) {
      ssd1331 = new Adafruit_SSD1331(Pin(GPIO_SSD1331_CS), Pin(GPIO_SSD1331_DC), Pin(GPIO_SSPI_MOSI), Pin(GPIO_SSPI_SCLK), Pin(GPIO_OLED_RESET));
    }
    else if (TasmotaGlobal.spi_enabled) {
      ssd1331 = new Adafruit_SSD1331(&SPI, Pin(GPIO_SSD1331_CS), Pin(GPIO_SSD1331_DC), Pin(GPIO_OLED_RESET));
    }

    delay(100);
    ssd1331->begin();
    renderer = ssd1331;
    // Rotation is currently broken, https://github.com/adafruit/Adafruit-SSD1331-OLED-Driver-Library-for-Arduino/issues/26
    renderer->DisplayInit(DISPLAY_INIT_MODE, Settings->display_size, Settings->display_rotate, Settings->display_font);
    renderer->dim(Settings->display_dimmer);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->clearDisplay();
    renderer->setTextFont(1);
    renderer->DrawStringAt(24, 27, "SSD1331", SSD1331_RED, 0);
    delay(1000);
    renderer->clearDisplay();
#endif

    color_type = COLOR_COLOR;

    ssd1331_init_done = true;
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: SSD1331"));
  }
}

#ifdef USE_DISPLAY_MODES1TO5

void SSD1331PrintLog(bool withDateTime) {
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings->display_refresh;
    if (!disp_screen_buffer_cols) { DisplayAllocScreenBuffer(); }

    char* txt = DisplayLogBuffer('\370');
    if (txt != NULL) {
      uint8_t last_row = Settings->display_rows -1;

      renderer->clearDisplay();
      renderer->setCursor(0,0);

      if (withDateTime) {
        char line[17];
        snprintf_P(line, sizeof(line), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d %02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.hour, RtcTime.minute, RtcTime.day_of_month, RtcTime.month, RtcTime.year);  // [12:34 01-02-2018]
        renderer->setTextColor(SSD1331_BLUE);
        renderer->println(line);
        renderer->setTextColor(fg_color);
        last_row--;
      }

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

void SSD1331Time(void) {
  char line[12];

  renderer->clearDisplay();
  snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);  // [ 12:34:56 ]
  renderer->setCursor(17, 20);
  renderer->println(line);
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);   // [01-02-2018]
  renderer->setCursor(17, 35);
  renderer->println(line);
  renderer->Updateframe();
}

void SSD1331Refresh(void) {     // Every second
  if (Settings->display_mode) {  // Mode 0 is User text
    switch (Settings->display_mode) {
      case 1:  // Time
        SSD1331Time();
        break;
      case 2:  // Local
      case 4:  // Mqtt
        SSD1331PrintLog(false);
        break;
      case 3:  // Local + Time
      case 5:  // Mqtt + Time
        SSD1331PrintLog(true);
        break;
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp14(uint8_t function) {
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
      SSD1331_InitDriver();
  }
  else if (ssd1331_init_done && (XDSP_14 == Settings->display_model)) {
    switch (function) {
      case FUNC_DISPLAY_MODEL:
        result = true;
        break;
#ifdef USE_DISPLAY_MODES1TO5
      case FUNC_DISPLAY_EVERY_SECOND:
        SSD1331Refresh();
        break;
#endif  // USE_DISPLAY_MODES1TO5
    }
  }
  return result;
}
#endif  // USE_DISPLAY_SSD1331
#endif  // USE_DISPLAY
#endif  // USE_SPI
