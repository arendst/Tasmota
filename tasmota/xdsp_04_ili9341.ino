/*
  xdsp_04_ILI9341.ino - Display ILI9341/2 support for Tasmota

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
#ifdef USE_DISPLAY_ILI9341

#define XDSP_04                4

enum IliModes { ILIMODE_9341 = 1, ILIMODE_9342, ILIMODE_MAX };

#include <ILI9341_2.h>

extern uint8_t *buffer;
extern uint8_t color_type;
ILI9341_2 *ili9341_2;

#if defined(USE_FT5206)
#include <FT5206.h>
uint8_t ili9342_ctouch_counter = 0;
#elif defined(USE_XPT2046)
#include <XPT2046_Touchscreen.h>
uint8_t ili9342_ctouch_counter = 0;
#endif // USE_FT5206

bool tft_init_done = false;


//Settings.display_options.type = ILIMODE_9341;

/*********************************************************************************************/

void ILI9341_InitDriver()
{

  // There are displays without CS
  if (PinUsed(GPIO_ILI9341_CS) || PinUsed(GPIO_ILI9341_DC) &&
      (TasmotaGlobal.spi_enabled || TasmotaGlobal.soft_spi_enabled)) {

    Settings.display_model = XDSP_04;

    if (Settings.display_width != ILI9341_TFTWIDTH) {
      Settings.display_width = ILI9341_TFTWIDTH;
    }
    if (Settings.display_height != ILI9341_TFTHEIGHT) {
      Settings.display_height = ILI9341_TFTHEIGHT;
    }

    // disable screen buffer
    buffer = NULL;

    if (!Settings.display_options.type || (Settings.display_options.type >= ILIMODE_MAX)) {
      Settings.display_options.type = ILIMODE_9341;
    }

    // default colors
    fg_color = ILI9341_WHITE;
    bg_color = ILI9341_BLACK;

    // check for special case with 2 SPI busses (ESP32 bitcoin)
    if (TasmotaGlobal.soft_spi_enabled) {
      // Init renderer, may use hardware spi, however we use SSPI defintion because SD card uses SPI definition  (2 spi busses)
      if (PinUsed(GPIO_SSPI_MOSI) && PinUsed(GPIO_SSPI_MISO) && PinUsed(GPIO_SSPI_SCLK)) {
        ili9341_2 = new ILI9341_2(Pin(GPIO_ILI9341_CS), Pin(GPIO_SSPI_MOSI), Pin(GPIO_SSPI_MISO), Pin(GPIO_SSPI_SCLK), Pin(GPIO_OLED_RESET), Pin(GPIO_ILI9341_DC), Pin(GPIO_BACKLIGHT), 2, Settings.display_options.type & 3);
      }
    } else if (TasmotaGlobal.spi_enabled) {
      if (PinUsed(GPIO_ILI9341_DC)) {
        ili9341_2 = new ILI9341_2(Pin(GPIO_ILI9341_CS), Pin(GPIO_SPI_MOSI), Pin(GPIO_SPI_MISO), Pin(GPIO_SPI_CLK), Pin(GPIO_OLED_RESET), Pin(GPIO_ILI9341_DC), Pin(GPIO_BACKLIGHT), 1, Settings.display_options.type & 3);
      }
    }

    if (ili9341_2 == nullptr) {
      AddLog(LOG_LEVEL_INFO, PSTR("DSP: ILI934x invalid GPIOs"));
      return;
    }

    ili9341_2->init(Settings.display_width, Settings.display_height);
    renderer = ili9341_2;

    renderer->DisplayInit(DISPLAY_INIT_MODE, Settings.display_size, Settings.display_rotate, Settings.display_font);
    renderer->dim(Settings.display_dimmer);

#ifdef SHOW_SPLASH
    // Welcome text
    renderer->setTextFont(2);
    renderer->setTextSize(1);
    renderer->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    renderer->DrawStringAt(50, (Settings.display_height/2)-12, (Settings.display_options.type & 3)==ILIMODE_9341?"ILI9341 TFT!":"ILI9342 TFT!", ILI9341_WHITE, 0);
    delay(1000);
#endif // SHOW_SPLASH

    color_type = COLOR_COLOR;

#ifdef USE_DISPLAY_MODES1TO5
    if (Settings.display_rotate) {
      DisplayAllocScreenBuffer();
    }
    Ili9341InitMode();
#endif  // USE_DISPLAY_MODES1TO5

#ifdef ESP32
#ifdef USE_FT5206
    // start digitizer with fixed adress and pins for esp32
    #undef SDA_2
    #define SDA_2 21
    #undef SCL_2
    #define SCL_2 22
    Wire1.begin(SDA_2, SCL_2, 400000);
    Touch_Init(Wire1);
#endif // USE_FT5206
#endif // ESP32

#ifdef USE_XPT2046
	  Touch_Init(Pin(GPIO_XPT2046_CS));
#endif

    tft_init_done = true;
    AddLog(LOG_LEVEL_INFO, PSTR("DSP: ILI9341"));
  }
}


void core2_disp_pwr(uint8_t on);
void core2_disp_dim(uint8_t dim);

void ili9342_bpwr(uint8_t on) {
#ifdef USE_M5STACK_CORE2
  core2_disp_pwr(on);
#endif
}

void ili9342_dimm(uint8_t dim) {
#ifdef USE_M5STACK_CORE2
  core2_disp_dim(dim);
#endif
}

#if defined(USE_FT5206) || defined(USE_XPT2046)
#ifdef USE_TOUCH_BUTTONS

#if defined(USE_FT5206)
void TS_RotConvert(int16_t *x, int16_t *y) {

int16_t temp;
  if (renderer) {
    uint8_t rot = renderer->getRotation();
    switch (rot) {
      case 0:
        break;
      case 1:
        temp = *y;
        *y = renderer->height() - *x;
        *x = temp;
        break;
      case 2:
        *x = renderer->width() - *x;
        *y = renderer->height() - *y;
        break;
      case 3:
        temp = *y;
        *y = *x;
        *x = renderer->width() - temp;
        break;
    }
  }
}
#elif defined(USE_XPT2046)
void TS_RotConvert(int16_t *x, int16_t *y) {

int16_t temp;
  if (renderer) {
    uint8_t rot = renderer->getRotation();
//    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(" TS: before convert x:%d / y:%d  screen r:%d / w:%d / h:%d"), *x, *y,rot,renderer->width(),renderer->height());
	temp = map(*x,XPT2046_MINX,XPT2046_MAXX, renderer->height(), 0);
	*x = map(*y,XPT2046_MINY,XPT2046_MAXY, renderer->width(), 0);
	*y = temp;
    switch (rot) {
      case 0:
        break;
      case 1:
        temp = *y;
        *y = renderer->width() - *x;
        *x = temp;
        break;
      case 2:
        *x = renderer->width() - *x;
        *y = renderer->height() - *y;
        break;
      case 3:
        temp = *y;
        *y = *x;
        *x = renderer->height() - temp;
        break;
    }
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(" TS: after convert x:%d / y:%d  screen r:%d / w:%d / h:%d"), *x, *y,rot,renderer->width(),renderer->height());
  }
}
#endif

// check digitizer hit
void ili9342_CheckTouch() {
ili9342_ctouch_counter++;
  if (2 == ili9342_ctouch_counter) {
    // every 100 ms should be enough
    ili9342_ctouch_counter = 0;
    Touch_Check(TS_RotConvert);
  }
}
#endif // USE_TOUCH_BUTTONS
#endif // USE_FT5206



#ifdef USE_DISPLAY_MODES1TO5

#define TFT_TOP                16
#define TFT_BOTTOM             16
#define TFT_FONT_WIDTH         6
#define TFT_FONT_HEIGTH        8     // Adafruit minimal font heigth pixels

uint16_t tft_top = TFT_TOP;
uint16_t tft_bottom = TFT_BOTTOM;
uint16_t tft_scroll = TFT_TOP;
uint16_t tft_cols = 0;

bool Ili9341Header(void) {
  if (Settings.display_cols[0] != tft_cols) {
    tft_cols = Settings.display_cols[0];
    if (tft_cols > 17) {
      tft_top = TFT_TOP;
      tft_bottom = TFT_BOTTOM;
    } else {
      tft_top = 0;
      tft_bottom = 0;
    }
    tft_scroll = tft_top;
    renderer->setScrollMargins(tft_top, tft_bottom);
  }
  return (tft_cols > 17);
}

void Ili9341InitMode(void) {
//  renderer->setRotation(Settings.display_rotate);  // 0
#ifdef USE_DISPLAY_ILI9341
//  renderer->invertDisplay(0);
#endif
  renderer->fillScreen(ILI9341_BLACK);
  renderer->setTextWrap(false);         // Allow text to run off edges
  renderer->cp437(true);
  if (!Settings.display_mode) {
    renderer->setCursor(0, 0);
    renderer->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    renderer->setTextSize(1);
  } else {
    Ili9341Header();
    renderer->setCursor(0, 0);
    renderer->setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
    renderer->setTextSize(2);
//    tft->println("HEADER");
  }
}

void Ili9341PrintLog(void) {
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    if (Settings.display_rotate) {
      if (!disp_screen_buffer_cols) { DisplayAllocScreenBuffer(); }
    }

    char* txt = DisplayLogBuffer('\370');
    if (txt != nullptr) {
      uint8_t size = Settings.display_size;
      uint16_t theight = size * TFT_FONT_HEIGTH;

      renderer->setTextSize(size);
      renderer->setTextColor(ILI9341_CYAN, ILI9341_BLACK);  // Add background color to solve flicker
      if (!Settings.display_rotate) {  // Use hardware scroll
        renderer->setCursor(0, tft_scroll);
        renderer->fillRect(0, tft_scroll, renderer->width(), theight, ILI9341_BLACK);  // Erase line
        renderer->print(txt);
        tft_scroll += theight;
        if (tft_scroll >= (renderer->height() - tft_bottom)) {
          tft_scroll = tft_top;
        }
        renderer->scrollTo(tft_scroll);
      } else {
        uint8_t last_row = Settings.display_rows -1;

        tft_scroll = (tft_top) ? theight : 0;  // Start below header
        renderer->setCursor(0, tft_scroll);
        for (uint32_t i = 0; i < last_row; i++) {
          strlcpy(disp_screen_buffer[i], disp_screen_buffer[i +1], disp_screen_buffer_cols);
//          tft->fillRect(0, tft_scroll, tft->width(), theight, ILI9341_BLACK);  // Erase line
          renderer->print(disp_screen_buffer[i]);
          tft_scroll += theight;
          renderer->setCursor(0, tft_scroll);
          delay(1);  // Fix background runs heap usage due to long runtime of this loop (up to 1 second)
        }
        strlcpy(disp_screen_buffer[last_row], txt, disp_screen_buffer_cols);
        DisplayFillScreen(last_row);
        renderer->print(disp_screen_buffer[last_row]);
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "[%s]"), txt);
    }
  }
}

void ILI9341_Refresh(void) {  // Every second
  if (Settings.display_mode) {  // Mode 0 is User text
    // 24-04-2017 13:45:43 = 19 + 1 ('\0') = 20
    // 24-04-2017 13:45 = 16 + 1 ('\0') = 17

    if (Ili9341Header()) {
      char tftdt[Settings.display_cols[0] +1];
      char date4[11];  // 24-04-2017
      uint8_t time_size = (Settings.display_cols[0] >= 20) ? 9 : 6;  // 13:45:43 or 13:45
      char spaces[Settings.display_cols[0] - (8 + time_size)];
      char time[time_size];    // 13:45:43

      renderer->setTextSize(Settings.display_size);
      renderer->setTextColor(ILI9341_YELLOW, ILI9341_RED);   // Add background color to solve flicker
      renderer->setCursor(0, 0);

      snprintf_P(date4, sizeof(date4), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);
      memset(spaces, 0x20, sizeof(spaces));
      spaces[sizeof(spaces) -1] = '\0';
      snprintf_P(time, sizeof(time), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
      snprintf_P(tftdt, sizeof(tftdt), PSTR("%s%s%s"), date4, spaces, time);

      renderer->print(tftdt);
    } else {
      renderer->setCursor(0, 0);
    }

    switch (Settings.display_mode) {
      case 1:  // Text
      case 2:  // Local
      case 3:  // Local
      case 4:  // Mqtt
      case 5:  // Mqtt
        Ili9341PrintLog();
        break;
    }
  }
}
#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************/
/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdsp04(uint8_t function)
{
  bool result = false;

  if (FUNC_DISPLAY_INIT_DRIVER == function) {
    ILI9341_InitDriver();
  }
  else if (tft_init_done && (XDSP_04 == Settings.display_model)) {
      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_TEXT_SIZE:
        case FUNC_DISPLAY_FONT_SIZE:
        case DISPLAY_INIT_MODE:
          renderer->clearDisplay();
          break;
#if defined(USE_FT5206) || defined(USE_XPT2046)
#ifdef USE_TOUCH_BUTTONS
        case FUNC_DISPLAY_EVERY_50_MSECOND:
#if defined(USE_FT5206)
          if (FT5206_found) {
#elif defined(USE_XPT2046)
          if (XPT2046_found) {
#endif

            ili9342_CheckTouch();
          }
          break;
#endif // USE_TOUCH_BUTTONS
#endif // USE_FT5206
#ifdef USE_DISPLAY_MODES1TO5
        case FUNC_DISPLAY_EVERY_SECOND:
          ILI9341_Refresh();
          break;
#endif  // USE_DISPLAY_MODES1TO5

    }
  }
  return result;
}

#endif  // USE_DISPLAY_ILI9341
#endif  // USE_DISPLAY
#endif  // USE_SPI
