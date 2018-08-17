/*
  xdrv_98_sd1306.ino - EXPERIMENTAL display support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends, Gerhard Mutz and Adafruit

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


  The SSD1306 has an expanded command set over the SH1106.
  The SH1106 only supports page addressing mode
  The SSD1306 added special horizontal and vertical addressing modes.
  The SSD1306 also has capability for automatic scrolling.
  If you (or your library) uses page addressing mode, it will work with either of the contollers (with the difference of line length, 128 vs. 132).
  If you (or your library) uses horizontal or vertical addressing mode it will only work on the SSD1306.

  about 9.1k
*/

#if defined(USE_I2C) || defined(USE_SPI)
#ifdef USE_DISPLAY
#ifdef USE_SSD1306

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

uint8_t font_x=6,font_y=8,txtsize=1;

void DisplayInit(void) {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)

  display.setTextColor(WHITE,BLACK);
  display.clearDisplay();
  display.setTextSize(2);
  DrawStringAt(20,20,(char*)"Adafruit",0);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.display();
  display.clearDisplay();

  display_ready=1;
}

void Clr_screen() {
  display.clearDisplay();
  display.display();
  display.setCursor(0,0);
}

void Init_Partial() {
}

void Init_Full() {
}

void Draw_HLine(uint16_t x,uint16_t y,int16_t len) {
  display.writeFastHLine(x,y,len,WHITE);
}

void Draw_VLine(uint16_t x,uint16_t y,int16_t len) {
  display.writeFastVLine(x,y,len,WHITE);
}

void Draw_Circle(uint16_t x,uint16_t y,uint16_t rad) {
  display.drawCircle(x,y,rad,WHITE);
}

void Draw_FilledCircle(uint16_t x,uint16_t y,uint16_t rad) {
  display.fillCircle(x,y,rad,WHITE);
}

void Draw_Rectangle(uint16_t x,uint16_t y,uint16_t x2,uint16_t y2) {
  display.drawRect(x,y,x2,y2,WHITE);
}

void Draw_FilledRectangle(uint16_t x,uint16_t y,uint16_t x2,uint16_t y2, uint8_t invert) {
  display.fillRect(x,y,x2,y2,invert?WHITE:BLACK);
}

void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
  display.writeLine(x0,y0,x1,y1,WHITE);
}

void Draw_Frame() {
  display.display();
}

void SetFontorSize(uint8_t font) {
  txtsize=font;
  display.setTextSize(txtsize);
}

void SetRotation(uint8_t rot) {
  display.setRotation(rot);
}

void DrawStringAt(uint16_t x,uint16_t y,char *str,uint8_t flag) {
  if (!flag) {
    display.setCursor(x,y);
  } else {
    display.setCursor((x-1)*font_x*txtsize,(y-1)*font_y*txtsize);
  }
  display.println(str);
}

void DisplayOnOff(uint8_t on) {
  if (on) {
    display.ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel
  } else {
    display.ssd1306_command(SSD1306_DISPLAYOFF);//--turn off oled panel
  }
}


#endif  // USE_SSD1306
#endif  // USE_DISPLAY
#endif  // USE_I2C or USE_SPI
