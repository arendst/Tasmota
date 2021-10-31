/**
 *  @filename   :   epd4in2.cpp
 *  @brief      :   Implements for Dual-color e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <esp_attr.h>
#include <esp_assert.h>
#include <esp_heap_caps.h>
#include <esp_log.h>
#include <esp_types.h>
#include <xtensa/core-macros.h>
#include <driver/gpio.h>
#include "m5epd4in7.h"
#include "M5EPD_Driver.h"
#include "M5EPD.h"


uint16_t M5Epd47::GetColorFromIndex(uint8_t index) {
  return index & 0xf;
}

M5Epd47::M5Epd47(int16_t dwidth, int16_t dheight) :  Renderer(dwidth, dheight) {
  width = dwidth;
  height = dheight;
  disp_bpp = 4;
}

int32_t M5Epd47::Init(void) {
//  epd_init(EPD_LUT_1K);
//  hl = epd_hl_init(WAVEFORM);
  uint32_t size = width * height / 2;
  framebuffer = (uint8_t*)heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  lvgl_param.fluslines = 10;


  pinMode(M5EPD_CS_PIN, OUTPUT);
  digitalWrite(M5EPD_CS_PIN, 1);

  EPD.begin(M5EPD_SCK_PIN, M5EPD_MOSI_PIN, M5EPD_MISO_PIN, M5EPD_CS_PIN, M5EPD_BUSY_PIN);
  EPD.SetRotation(0);
  EPD.Clear(true);

  return 0;
}

void M5Epd47::DisplayInit(int8_t p, int8_t size, int8_t rot, int8_t font) {

  if (p ==  DISPLAY_INIT_MODE) {
    EPD.WritePartGram4bpp(0, 0, width, height, framebuffer);
    EPD.UpdateArea(0, 0, width, height, UPDATE_MODE_GC16);
  }
  if (p ==  DISPLAY_INIT_FULL) {
    EPD.WritePartGram4bpp(0, 0, width, height, framebuffer);
    EPD.UpdateArea(0, 0, width, height, UPDATE_MODE_GC16);
    return;
  }
  if (p ==  DISPLAY_INIT_PARTIAL) {
    EPD.WritePartGram4bpp(0, 0, width, height, framebuffer);
    EPD.UpdateArea(0, 0, width, height, UPDATE_MODE_GL16);
    return;
  }
  setRotation(rot);
  setTextWrap(false);
  cp437(true);
  setTextFont(font);
  setTextSize(size);
  setCursor(0,0);
  fillScreen(15);
}

void M5Epd47::Updateframe() {
    EPD.WritePartGram4bpp(0, 0, width, height, framebuffer);
    EPD.UpdateArea(0, 0, width, height, UPDATE_MODE_GC16);
}

void M5Epd47::fillScreen(uint16_t color) {
  color &= 0xf;
   uint8_t icol = (color << 4) | color;
   memset(framebuffer, icol, width * height / 2);
}

#define _swap(a, b) { uint16_t t = a; a = b; b = t; }

void M5Epd47::drawPixel(int16_t x, int16_t y, uint16_t color) {
uint16_t xp = x;
uint16_t yp = y;
uint8_t *buf_ptr;

  switch (getRotation()) {
    case 1:
      _swap(xp, yp);
      xp = width - xp - 1;
      break;
    case 2:
      xp = width - xp - 1;
      yp = height - yp - 1;
      break;
    case 3:
      _swap(xp, yp);
      yp = height - yp - 1;

      break;
  }

  if (xp >= width) return;
  if (yp >= height) return;
  buf_ptr = &framebuffer[yp * width / 2 + xp / 2];

    if (!(xp % 2)) {
        *buf_ptr = (*buf_ptr & 0x0F) | (color << 4);
    } else {
        *buf_ptr = (*buf_ptr & 0xF0) | (color & 0xf);
    }

}

void M5Epd47::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  while (h--) {
    drawPixel(x , y , color);
    y++;
  }
}
void M5Epd47::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  while (w--) {
    drawPixel(x , y , color);
    x++;
  }
}

void M5Epd47::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

  // just save params or update frame
  if (!x0 && !y0 && !x1 && !y1) {
    //Updateframe();
  } else {
    seta_xp1 = x0;
    seta_xp2 = x1;
    seta_yp1 = y0;
    seta_yp2 = y1;
  }

}

static inline void lvgl_color_swap2(uint16_t *data, uint16_t len) { for (uint32_t i = 0; i < len; i++) (data[i] = data[i] << 8 | data[i] >> 8); }

void M5Epd47::pushColors(uint16_t *data, uint16_t len, boolean not_swapped) {

  if (not_swapped == false) {
    lvgl_color_swap2(data, len);
  }

    // stupid bw version
uint16_t x1 = seta_xp1;
uint16_t x2 = seta_xp2;
uint16_t y1 = seta_yp1;
uint16_t y2 = seta_yp2;

    for (uint32_t y = y1; y < y2; y++) {
      for (uint32_t x = x1; x < x2; x++) {
        uint16_t color = *data++;
        uint8_t red = ((color >> 11) & 0x1f) << 3;
        uint8_t green = ((color >> 5) & 0x3f) << 2;
        uint8_t blue = (color & 0x1f) << 3;
        color = (red + green + blue) / 3;
        color >>= 4;
        drawPixel(x, y, color);
        len--;
        if (!len) {
          seta_yp1 = y + 1;
          return;
        }
      }
    }
}

/* END OF FILE */
