/*
  xdsp_14_SSD1331.ino - Display SSD1331 support for Tasmota

  Copyright (C) 2020  Jeroen Vermeulen, Gerhard Mutz and Theo Arends

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
#ifdef USE_DISPLAY_SSD1331

#define XDSP_14                14

#define COLORED                1
#define UNCOLORED              0
#define USE_TINY_FONT

#define SSD1331_BLACK       0x0000      //   0,   0,   0
#define SSD1331_RED         0xF800      // 255,   0,   0
#define SSD1331_WHITE       0xFFFF      // 255, 255, 255

#include <Adafruit_SSD1331.h>

extern uint8_t *buffer;
extern uint8_t color_type;
Adafruit_SSD1331 *ssd1331;

/*********************************************************************************************/

void SSD1331_InitDriver() {
  if (!Settings.display_model) {
    Settings.display_model = XDSP_14;
  }

  if (XDSP_14 == Settings.display_model) {

    if (Settings.display_width != Adafruit_SSD1331::TFTWIDTH) {
      Settings.display_width = Adafruit_SSD1331::TFTWIDTH;
    }
    if (Settings.display_height != Adafruit_SSD1331::TFTHEIGHT) {
      Settings.display_height = Adafruit_SSD1331::TFTHEIGHT;
    }

    buffer=0;

    // default colors
    fg_color = SSD1331_WHITE;
    bg_color = SSD1331_BLACK;

    // init renderer
    if  (PinUsed(GPIO_SSPI_CS) && PinUsed(GPIO_SSPI_MOSI) && PinUsed(GPIO_SSPI_SCLK)){
      ssd1331  = new Adafruit_SSD1331(Pin(GPIO_SSPI_CS),Pin(GPIO_SSPI_DC),Pin(GPIO_SSPI_MOSI),Pin(GPIO_SSPI_SCLK),OLED_RESET);
    } else {
      if (PinUsed(GPIO_SPI_CS) && PinUsed(GPIO_SPI_MOSI) && PinUsed(GPIO_SPI_CLK)) {
        ssd1331  = new Adafruit_SSD1331(Pin(GPIO_SPI_CS),Pin(GPIO_SPI_DC),Pin(GPIO_SPI_MOSI),Pin(GPIO_SPI_CLK),OLED_RESET);
      } else {
        return;
      }
    }

    delay(100);
    ssd1331->begin();
    renderer = ssd1331;
    // Rotation is currently broken, https://github.com/adafruit/Adafruit-SSD1331-OLED-Driver-Library-for-Arduino/issues/26
    renderer->DisplayInit(DISPLAY_INIT_MODE,Settings.display_size,Settings.display_rotate,Settings.display_font);
    renderer->dim(Settings.display_dimmer);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->clearDisplay();
    renderer->setTextFont(1);
    renderer->DrawStringAt(24, 27, "SSD1331", SSD1331_RED, 0);
    delay(1000);
#endif

    color_type = COLOR_COLOR;
  }
}

#ifdef USE_DISPLAY_MODES1TO5

void SSD1331PrintLog(void)
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

      AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "[%s]"), disp_screen_buffer[last_row]);

      renderer->println(disp_screen_buffer[last_row]);
      renderer->Updateframe();
    }
  }
}

void SSD1331Time(void)
{
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

void SSD1331Refresh(void)  // Every second
{
  if (Settings.display_mode) {  // Mode 0 is User text
    switch (Settings.display_mode) {
      case 1:  // Time
        SSD1331Time();
        break;
      case 2:  // Local
      case 3:  // Local
      case 4:  // Mqtt
      case 5:  // Mqtt
        SSD1331PrintLog();
        break;
    }
  }
}

#endif  // USE_DISPLAY_MODES1TO5
/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp14(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
      SSD1331_InitDriver();
  }
  else if (XDSP_14 == Settings.display_model) {
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
