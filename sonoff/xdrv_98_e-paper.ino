/*
  xdrv_98_sd1306.ino - EXPERIMENTAL display support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends, Gerhard Mutz and Waveshare

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

  about 12,5k
*/

#if defined(USE_I2C) || defined(USE_SPI)
#ifdef USE_DISPLAY
#ifdef USE_EPAPER

#include <epd2in9.h>
#include <epdpaint.h>

// using font 8 is opional (num=3)
// very badly readable, but may be useful for graphs
//#define USE_TINY_FONT


unsigned char image[(EPD_HEIGHT*EPD_WIDTH)/8];

Paint paint(image,EPD_WIDTH,EPD_HEIGHT);    // width should be the multiple of 8
Epd epd;
sFONT *selected_font;

void DisplayInit(void) {
  // init paper display
  #define COLORED     0
  #define UNCOLORED   1

  if (pin[GPIO_SSPI_CS]<99) {
    epd.cs_pin=pin[GPIO_SSPI_CS];
  } else {
    return;
  }
  if (pin[GPIO_SSPI_MOSI]<99) {
    epd.mosi_pin=pin[GPIO_SSPI_MOSI];
  } else {
    return;
  }
  if (pin[GPIO_SSPI_SCLK]<99) {
    epd.sclk_pin=pin[GPIO_SSPI_SCLK];
  } else {
    return;
  }

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

  selected_font=&Font12;

  // Welcome text
  paint.SetRotate(ROTATE_90);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(50, 50, "Waveshare E-Paper Display!", selected_font, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
  delay(1000);

  paint.Clear(UNCOLORED);

  display_ready=1;
}

void Clr_screen() {
  paint.Clear(UNCOLORED);
}

void Init_Partial() {
  epd.Init(lut_partial_update);
  paint.Clear(UNCOLORED);
  epd.DisplayFrame();
  delay(500);
}

void Init_Full() {
  epd.Init(lut_full_update);
  paint.Clear(UNCOLORED);
  epd.ClearFrameMemory(0xFF);
  epd.DisplayFrame();
  delay(3000);
}

void Draw_HLine(uint16_t x,uint16_t y,int16_t len) {
  paint.DrawHorizontalLine(x,y,len,COLORED);
}

void Draw_VLine(uint16_t x,uint16_t y,int16_t len) {
  paint.DrawVerticalLine(x,y,len,COLORED);
}

void Draw_Circle(uint16_t x,uint16_t y,uint16_t rad) {
  paint.DrawCircle(x,y,rad,COLORED);
}

void Draw_FilledCircle(uint16_t x,uint16_t y,uint16_t rad) {
  paint.DrawFilledCircle(x,y,rad,COLORED);
}

void Draw_Rectangle(uint16_t x,uint16_t y,uint16_t x2,uint16_t y2) {
  paint.DrawRectangle(x,y,x2,y2,COLORED);
}


void Draw_FilledRectangle(uint16_t x,uint16_t y,uint16_t x2,uint16_t y2, uint8_t invert) {
  paint.DrawFilledRectangle(x,y,x2,y2,invert?COLORED:UNCOLORED);
}

void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
  paint.DrawLine(x0,y0,x1,y1,COLORED);
}


void Draw_Frame() {
  epd.SetFrameMemory(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
}

void SetFontorSize(uint8_t font) {
  if (font==1) {
    selected_font=&Font12;
  } else {
#ifdef USE_TINY_FONT
    if (font==2) {
      selected_font=&Font24;
    } else {
      selected_font=&Font8;
    }
#else
    selected_font=&Font24;
#endif
  }
}

void SetRotation(uint8_t rot) {
  paint.SetRotate(rot);
}

void DrawStringAt(uint16_t x,uint16_t y,char *str,uint8_t flag) {
  if (!flag) {
    paint.DrawStringAt(x,y,str,selected_font, COLORED);
  } else {
    paint.DrawStringAt((x-1)*selected_font->Width,(y-1)*selected_font->Height,str,selected_font, COLORED);
  }
}

// not needed
void DisplayOnOff(uint8_t on) {
}


#endif  // USE_EPAPER
#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
