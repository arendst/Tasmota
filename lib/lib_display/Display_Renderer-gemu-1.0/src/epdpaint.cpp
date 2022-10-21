/**
 *  @filename   :   epdpaint.cpp
 *  @brief      :   Paint tools
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
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

#include <pgmspace.h>
#include "epdpaint.h"

Paint::Paint(int16_t width, int16_t height) :
Renderer(width,height) {
}


void Paint::DisplayOnff(int8_t on) {
}

void Paint::Begin(int16_t p1,int16_t p2,int16_t p3) {
}

void Paint::Updateframe() {
}

void Paint::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {

}


#define renderer_swap(a, b) { int16_t t = a; a = b; b = t; }
/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 * we must use this for epaper because these displays have a strange and different bit pattern
 */
void Paint::DrawAbsolutePixel(int x, int y, int16_t color) {

    int16_t w=width(),h=height(),rot=getRotation();
    if (rot==1 || rot==3) {
      renderer_swap(w,h);
    }

    if (x < 0 || x >= w || y < 0 || y >= h) {
        return;
    }
    if (IF_INVERT_COLOR) {
        if (color) {
            framebuffer[(x + y * w) / 8] |= 0x80 >> (x % 8);
        } else {
            framebuffer[(x + y * w) / 8] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (color) {
            framebuffer[(x + y * w) / 8] &= ~(0x80 >> (x % 8));
        } else {
            framebuffer[(x + y * w) / 8] |= 0x80 >> (x % 8);
        }
    }
}

#if 0
/**
 *  @brief: this draws a pixel by the coordinates
 */
void Paint::drawPixel(int16_t x, int16_t y, uint16_t color) {
    int16_t point_temp;
    int8_t rot=getRotation();
    if (rot == ROTATE_0) {
        if(x < 0 || x >= width() || y < 0 || y >= height()) {
            return;
        }
        DrawAbsolutePixel(x, y, color);
    } else if (rot== ROTATE_90) {
        if(x < 0 || x >= width() || y < 0 || y >=height() ) {
          return;
        }
        point_temp = x;
        x = height() - y;
        y = point_temp;
        DrawAbsolutePixel(x, y, color);
    } else if (rot == ROTATE_180) {
        if(x < 0 || x >= width() || y < 0 || y >= height()) {
          return;
        }
        x = width() - x;
        y = height() - y;
        DrawAbsolutePixel(x, y, color);
    } else if (rot == ROTATE_270) {
        if(x < 0 || x >= width() || y < 0 || y >= height()) {
          return;
        }
        point_temp = x;
        x = y;
        y = width() - point_temp;
        DrawAbsolutePixel(x, y, color);
    }
}
#else


void Paint::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (!framebuffer) return;
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
    return;

  // check rotation, move pixel around if necessary
  switch (getRotation()) {
  case 1:
    renderer_swap(x, y);
    x = WIDTH - x - 1;
    break;
  case 2:
    x = WIDTH - x - 1;
    y = HEIGHT - y - 1;
    break;
  case 3:
    renderer_swap(x, y);
    y = HEIGHT - y - 1;
    break;
  }

  // x is which column
  DrawAbsolutePixel(x,y,color);
  /*
    switch (color)
    {
      case WHITE:   buffer[x+ (y/8)*WIDTH] |=  (1 << (y&7)); break;
      case BLACK:   buffer[x+ (y/8)*WIDTH] &= ~(1 << (y&7)); break;
      case INVERSE: buffer[x+ (y/8)*WIDTH] ^=  (1 << (y&7)); break;
    }*/

}
#endif
/**
*  @brief: this draws a horizontal line on the frame buffer
*/
void Paint::drawFastHLine(int16_t x, int16_t y, int16_t line_width, uint16_t colored) {
    int i;
    for (i = x; i < x + line_width; i++) {
        drawPixel(i, y, colored);
    }
}

/**
*  @brief: this draws a vertical line on the frame buffer
*/
void Paint::drawFastVLine(int16_t x, int16_t y, int16_t line_height, uint16_t colored) {
    int i;
    for (i = y; i < y + line_height; i++) {
        drawPixel(x, i, colored);
    }
}


/* END OF FILE */
