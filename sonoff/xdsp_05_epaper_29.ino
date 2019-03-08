/*
  xdsp_05_epaper_29.ino - 2.9 Inch display e-paper support for Sonoff-Tasmota

  Copyright (C) 2019  Theo Arends, Gerhard Mutz and Waveshare

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
#ifdef USE_DISPLAY_EPAPER_29

#define XDSP_05                5

#define EPD_TOP                12
#define EPD_FONT_HEIGTH        12

#define COLORED                0
#define UNCOLORED              1

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
#define USE_TINY_FONT

#include <epd2in9.h>
#include <epdpaint.h>

unsigned char image[(EPD_HEIGHT * EPD_WIDTH) / 8];

Paint paint(image, EPD_WIDTH, EPD_HEIGHT);    // width should be the multiple of 8
Epd epd;
sFONT *selected_font;

uint16_t epd_scroll;

/*********************************************************************************************/

void EpdInitMode(void)
{
  // whiten display with full update
  epd.Init(lut_full_update);

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  delay(3000);

  // switch to partial update
  epd.Init(lut_partial_update);

  // Clear image memory
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  delay(500);

  selected_font = &Font12;

  paint.SetRotate(Settings.display_rotate);
/*
  // Welcome text
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(50, 50, "Waveshare E-Paper Display!", selected_font, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  delay(1000);
*/
  paint.Clear(UNCOLORED);

  epd_scroll = EPD_TOP;
}

void EpdInitPartial(void)
{
  epd.Init(lut_partial_update);
  //paint.Clear(UNCOLORED);
  epd.DisplayFrame();
  delay(500);
}

void EpdInitFull(void)
{
  epd.Init(lut_full_update);
  //paint.Clear(UNCOLORED);
  //epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();
  delay(3000);
}

void EpdInit(uint8_t mode)
{
  switch(mode) {
    case DISPLAY_INIT_MODE:
      EpdInitMode();
      break;
    case DISPLAY_INIT_PARTIAL:
      EpdInitPartial();
      break;
    case DISPLAY_INIT_FULL:
      EpdInitFull();
      break;
  }
}

void EpdInitDriver(void)
{
  if (!Settings.display_model) {
    Settings.display_model = XDSP_05;
  }

  if (XDSP_05 == Settings.display_model) {
    if ((pin[GPIO_SPI_CS] < 99) && (pin[GPIO_SPI_CLK] < 99) && (pin[GPIO_SPI_MOSI] < 99)) {
      epd.cs_pin = pin[GPIO_SPI_CS];
      epd.sclk_pin = pin[GPIO_SPI_CLK];   // 14
      epd.mosi_pin = pin[GPIO_SPI_MOSI];  // 13
      EpdInitMode();
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("EPD: HardSPI CS %d, CLK %d, MOSI %d"), epd.cs_pin, epd.sclk_pin, epd.mosi_pin);
    }
    else if ((pin[GPIO_SSPI_CS] < 99) && (pin[GPIO_SSPI_SCLK] < 99) && (pin[GPIO_SSPI_MOSI] < 99)) {
      epd.cs_pin = pin[GPIO_SSPI_CS];
      epd.sclk_pin = pin[GPIO_SSPI_SCLK];
      epd.mosi_pin = pin[GPIO_SSPI_MOSI];
      EpdInitMode();
      AddLog_P2(LOG_LEVEL_DEBUG, PSTR("EPD: SoftSPI CS %d, CLK %d, MOSI %d"), epd.cs_pin, epd.sclk_pin, epd.mosi_pin);
    }
  }
}

/*********************************************************************************************/

void EpdClear(void)
{
  paint.Clear(UNCOLORED);
}

void EpdSetFont(uint8_t font)
{
  if (1 == font) {
    selected_font = &Font12;
  } else {
#ifdef USE_TINY_FONT
    if (2 == font) {
      selected_font = &Font24;
    } else {
      selected_font = &Font8;
    }
#else
    selected_font = &Font24;
#endif
  }
}

void EpdDisplayFrame(void)
{
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  epd.Sleep();
}

void EpdDrawStringAt(uint16_t x, uint16_t y, char *str, uint8_t color, uint8_t flag)
{
  if (!flag) {
    paint.DrawStringAt(x, y, str, selected_font, color);
  } else {
    paint.DrawStringAt((x-1) * selected_font->Width, (y-1) * selected_font->Height, str, selected_font, color);
  }
}

// not needed
void EpdDisplayOnOff(uint8_t on)
{

}

void EpdOnOff(void)
{
  EpdDisplayOnOff(disp_power);
}

/*********************************************************************************************/

#ifdef USE_DISPLAY_MODES1TO5

void EpdPrintLog(void)
{
  disp_refresh--;
  if (!disp_refresh) {
    disp_refresh = Settings.display_refresh;
    if (Settings.display_rotate) {
      if (!disp_screen_buffer_cols) { DisplayAllocScreenBuffer(); }
    }

    char* txt = DisplayLogBuffer('\040');
    if (txt != NULL) {
      uint8_t size = Settings.display_size;
      uint16_t theight = size * EPD_FONT_HEIGTH;

      EpdSetFont(size);
      uint8_t last_row = Settings.display_rows -1;

//      epd_scroll = theight;  // Start below header
      epd_scroll = 0;  // Start at top with no header
      for (uint8_t i = 0; i < last_row; i++) {
        strlcpy(disp_screen_buffer[i], disp_screen_buffer[i +1], disp_screen_buffer_cols);
        EpdDrawStringAt(0, epd_scroll, disp_screen_buffer[i], COLORED, 0);
        epd_scroll += theight;
      }
      strlcpy(disp_screen_buffer[last_row], txt, disp_screen_buffer_cols);
      DisplayFillScreen(last_row);
      EpdDrawStringAt(0, epd_scroll, disp_screen_buffer[last_row], COLORED, 0);
//      EpdDisplayFrame();

      AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "[%s]"), txt);
    }
  }
}

void EpdRefresh(void)  // Every second
{
  if (Settings.display_mode) {  // Mode 0 is User text
/*
    char tftdt[Settings.display_cols[0] +1];
    char date4[11];  // 24-04-2017
    char space[Settings.display_cols[0] - 17];
    char time[9];    // 13:45:43

    EpdSetFont(1);

    snprintf_P(date4, sizeof(date4), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"), RtcTime.day_of_month, RtcTime.month, RtcTime.year);
    memset(space, 0x20, sizeof(space));
    space[sizeof(space) -1] = '\0';
    snprintf_P(time, sizeof(time), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
    snprintf_P(tftdt, sizeof(tftdt), PSTR("%s%s%s"), date4, space, time);

    EpdDrawStringAt(0, 0, tftdt, COLORED, 0);
*/
    switch (Settings.display_mode) {
      case 1:  // Text
      case 2:  // Local
      case 3:  // Local
      case 4:  // Mqtt
      case 5:  // Mqtt
        EpdPrintLog();
        EpdDisplayFrame();
        break;
    }

//    EpdDisplayFrame();
  }
}

#endif  // USE_DISPLAY_MODES1TO5

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdsp05(uint8_t function)
{
  bool result = false;

  if (spi_flg || soft_spi_flg) {
    if (FUNC_DISPLAY_INIT_DRIVER == function) {
      EpdInitDriver();
    }
    else if (XDSP_05 == Settings.display_model) {

      if (!dsp_color) { dsp_color = COLORED; }

      switch (function) {
        case FUNC_DISPLAY_MODEL:
          result = true;
          break;
        case FUNC_DISPLAY_INIT:
          EpdInit(dsp_init);
          break;
        case FUNC_DISPLAY_POWER:
          EpdOnOff();
          break;
        case FUNC_DISPLAY_CLEAR:
          EpdClear();
          break;
        case FUNC_DISPLAY_DRAW_HLINE:
          paint.DrawHorizontalLine(dsp_x, dsp_y, dsp_len, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_VLINE:
          paint.DrawVerticalLine(dsp_x, dsp_y, dsp_len, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_LINE:
          paint.DrawLine(dsp_x, dsp_y, dsp_x2, dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_CIRCLE:
          paint.DrawCircle(dsp_x, dsp_y, dsp_rad, dsp_color);
          break;
        case FUNC_DISPLAY_FILL_CIRCLE:
          paint.DrawFilledCircle(dsp_x, dsp_y, dsp_rad, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_RECTANGLE:
          paint.DrawRectangle(dsp_x, dsp_y, dsp_x + dsp_x2, dsp_y + dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_FILL_RECTANGLE:
          paint.DrawFilledRectangle(dsp_x, dsp_y, dsp_x + dsp_x2, dsp_y + dsp_y2, dsp_color);
          break;
        case FUNC_DISPLAY_DRAW_FRAME:
          EpdDisplayFrame();
          break;
        case FUNC_DISPLAY_TEXT_SIZE:
//          EpdSetFontorSize(Settings.display_size);
          break;
        case FUNC_DISPLAY_FONT_SIZE:
          EpdSetFont(Settings.display_font);
          break;
        case FUNC_DISPLAY_DRAW_STRING:
          EpdDrawStringAt(dsp_x, dsp_y, dsp_str, dsp_color, dsp_flag);
          break;
        case FUNC_DISPLAY_ONOFF:
          EpdDisplayOnOff(dsp_on);
          break;
        case FUNC_DISPLAY_ROTATION:
          paint.SetRotate(Settings.display_rotate);
          break;
#ifdef USE_DISPLAY_MODES1TO5
        case FUNC_DISPLAY_EVERY_SECOND:
          EpdRefresh();
          break;
#endif  // USE_DISPLAY_MODES1TO5
      }
    }
  }
  return result;
}

#endif  // USE_DISPLAY_EPAPER_29
#endif  // USE_DISPLAY
#endif  // USE_SPI
