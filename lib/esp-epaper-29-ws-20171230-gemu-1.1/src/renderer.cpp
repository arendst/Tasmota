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
#include "renderer.h"

#define USE_EPD_FONTS
//#define USE_ALL_EPD_FONTS
//#define USE_GFX_FONTS
#define USE_TINY_FONT

uint8_t wr_redir=0;

uint8_t *buffer;

#define register

#define SPRINT(A) char str[32];sprintf(str,"val: %d ",A);Serial.println((char*)str);

#define OLED_FONT_WIDTH        6
#define OLED_FONT_HEIGTH       8
#define BLACK 0

Renderer::Renderer(int16_t x, int16_t y) :
Adafruit_GFX(x, y) {
  font=0;
#ifdef USE_EPD_FONTS
  selected_font = &Font12;
#endif

}

uint16_t Renderer::GetColorFromIndex(uint8_t index) {
  if (index>0) return 1;
  return 0;
}

void Renderer::dim(uint8_t contrast) {

}

void Renderer::pushColors(uint16_t *data, uint8_t len, boolean first) {

}

void Renderer::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

}

void Renderer::DisplayOnff(int8_t on) {

}

void Renderer::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {

}

void Renderer::Begin(int16_t p1,int16_t p2,int16_t p3) {

}

void Renderer::Updateframe() {

}

/**
 *  @brief: this draws a charactor on the frame buffer but not refresh
 */
void Renderer::DrawCharAt(int16_t x, int16_t y, char ascii_char,int16_t colored) {
#ifdef USE_EPD_FONTS
    sFONT *xfont = selected_font;
    int i, j;
    unsigned int char_offset = (ascii_char - ' ') * xfont->Height * (xfont->Width / 8 + (xfont->Width % 8 ? 1 : 0));
    const unsigned char* ptr = &xfont->table[char_offset];

    for (j = 0; j < xfont->Height; j++) {
        for (i = 0; i < xfont->Width; i++) {
            if (pgm_read_byte(ptr) & (0x80 >> (i % 8))) {
                writePixel(x + i, y + j, colored);
            } else {
              // fill background
                if (!drawmode) writePixel(x + i, y + j, textbgcolor);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (xfont->Width % 8 != 0) {
            ptr++;
        }
    }
#endif
}

/**
*  @brief: this displays a string on the frame buffer but not refresh
*/
void Renderer::DrawStringAt(int16_t x, int16_t y, const char* text, uint16_t colored, uint8_t flag) {
    const char* p_text = text;
    unsigned int counter = 0;
    int refcolumn = x;
    sFONT *xfont = selected_font;

#ifndef USE_EPD_FONTS
    font=0;
#endif

#ifndef USE_GFX_FONTS
    if (!font) {
#endif
      if (flag) {
        x=(x-1)*OLED_FONT_WIDTH*textsize_x;
        y=(y-1)*OLED_FONT_HEIGTH*textsize_y;
      }
      setCursor(x,y);
      setTextColor(colored,textbgcolor);
      print(text);
      return;
#ifndef USE_GFX_FONTS
    }
#endif


    if (flag) {
      x=(x-1)*xfont->Width;
      y=(y-1)*xfont->Height;
      refcolumn = x;
    }

    /* Send the string character by character on EPD */
    if (font==7) {
      return FastString(x,y,colored,p_text);
    }
    while (*p_text != 0) {
        /* Display one character on EPD */
        DrawCharAt(refcolumn, y, *p_text, colored);
        /* increment the column position */
        refcolumn += xfont->Width;
        /* Point on the next character */
        p_text++;
        counter++;
    }

}

void Renderer::FastString(uint16_t x,uint16_t y,uint16_t tcolor, const char* str) {

}

#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include <Fonts/FreeMono24pt7b.h>

sFONT RAFont = {
  0,
  7, /* Width */
  12, /* Height */
};

void Renderer::setTextFont(uint8_t f) {
  font=f;

#ifdef USE_GFX_FONTS
  switch (f) {
      case 0:
        setFont(0);
        break;
      case 1:
        setFont(&FreeMono12pt7b);
        break;
      case 2:
        setFont(&FreeMono18pt7b);
        break;
      case 3:
        setFont(&FreeMono24pt7b);
        break;
      default:
        setFont(0);
        break;
  }

#endif

#ifdef USE_ALL_EPD_FONTS
  switch (font) {
    case 1:
      selected_font = &Font12;
      break;
    case 2:
      selected_font = &Font24;
      break;
    case 3:
      selected_font = &Font8;
      break;
    case 4:
      selected_font = &Font16;
      break;
    case 5:
      selected_font = &Font20;
      break;
    case 7:
      selected_font = &RAFont;
    default:
      font=0;
  }
#else
#ifdef USE_EPD_FONTS
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
#endif
#endif

}


void Renderer::clearDisplay(void) {
  fillScreen(BLACK);
}

#define renderer_swap(a, b) { int16_t t = a; a = b; b = t; }

void Renderer::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  boolean bSwap = false;
  if (!buffer) return;
  switch(getRotation()) {
    case 0:
      // 0 degree rotation, do nothing
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x
      bSwap = true;
      renderer_swap(x, y);
      x = WIDTH - x - 1;
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      x -= (w-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y  and adjust y for w (not to become h)
      bSwap = true;
      renderer_swap(x, y);
      y = HEIGHT - y - 1;
      y -= (w-1);
      break;
  }

  if(bSwap) {
    drawFastVLineInternal(x, y, w, color);
  } else {
    drawFastHLineInternal(x, y, w, color);
  }
}

void Renderer::drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) {
  // Do bounds/limit checks
  if(y < 0 || y >= HEIGHT) { return; }

  // make sure we don't try to draw below 0
  if(x < 0) {
    w += x;
    x = 0;
  }

  // make sure we don't go off the edge of the display
  if( (x + w) > WIDTH) {
    w = (WIDTH - x);
  }

  // if our width is now negative, punt
  if(w <= 0) { return; }

  // set up the pointer for  movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * WIDTH);
  // and offset x columns in
  pBuf += x;

  register uint8_t mask = 1 << (y&7);

  switch (color)
  {
  case WHITE:         while(w--) { *pBuf++ |= mask; }; break;
    case BLACK: mask = ~mask;   while(w--) { *pBuf++ &= mask; }; break;
  case INVERSE:         while(w--) { *pBuf++ ^= mask; }; break;
  }
}

void Renderer::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  if (!buffer) return;
  bool bSwap = false;
  switch(getRotation()) {
    case 0:
      break;
    case 1:
      // 90 degree rotation, swap x & y for rotation, then invert x and adjust x for h (now to become w)
      bSwap = true;
      renderer_swap(x, y);
      x = WIDTH - x - 1;
      x -= (h-1);
      break;
    case 2:
      // 180 degree rotation, invert x and y - then shift y around for height.
      x = WIDTH - x - 1;
      y = HEIGHT - y - 1;
      y -= (h-1);
      break;
    case 3:
      // 270 degree rotation, swap x & y for rotation, then invert y
      bSwap = true;
      renderer_swap(x, y);
      y = HEIGHT - y - 1;
      break;
  }

  if(bSwap) {
    drawFastHLineInternal(x, y, h, color);
  } else {
    drawFastVLineInternal(x, y, h, color);
  }
}


void Renderer::drawFastVLineInternal(int16_t x, int16_t __y, int16_t __h, uint16_t color) {

  // do nothing if we're off the left or right side of the screen
  if(x < 0 || x >= WIDTH) { return; }

  // make sure we don't try to draw below 0
  if(__y < 0) {
    // __y is negative, this will subtract enough from __h to account for __y being 0
    __h += __y;
    __y = 0;

  }

  // make sure we don't go past the height of the display
  if( (__y + __h) > HEIGHT) {
    __h = (HEIGHT - __y);
  }

  // if our height is now negative, punt
  if(__h <= 0) {
    return;
  }

  // this display doesn't need ints for coordinates, use local byte registers for faster juggling
  register uint8_t y = __y;
  register uint8_t h = __h;


  // set up the pointer for fast movement through the buffer
  register uint8_t *pBuf = buffer;
  // adjust the buffer pointer for the current row
  pBuf += ((y/8) * WIDTH);
  // and offset x columns in
  pBuf += x;

  // do the first partial byte, if necessary - this requires some masking
  register uint8_t mod = (y&7);
  if(mod) {
    // mask off the high n bits we want to set
    mod = 8-mod;

    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    // register uint8_t mask = ~(0xFF >> (mod));
    static uint8_t premask[8] = {0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE };
    register uint8_t mask = premask[mod];

    // adjust the mask if we're not going to reach the end of this byte
    if( h < mod) {
      mask &= (0XFF >> (mod-h));
    }

  switch (color)
    {
    case WHITE:   *pBuf |=  mask;  break;
    case BLACK:   *pBuf &= ~mask;  break;
    case INVERSE: *pBuf ^=  mask;  break;
    }

    // fast exit if we're done here!
    if(h<mod) { return; }

    h -= mod;

    pBuf += WIDTH;
  }


  // write solid bytes while we can - effectively doing 8 rows at a time
  if(h >= 8) {
    if (color == INVERSE)  {          // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
      do  {
      *pBuf=~(*pBuf);

        // adjust the buffer forward 8 rows worth of data
        pBuf += WIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    else {
      // store a local value to work with
      register uint8_t val = (color == WHITE) ? 255 : 0;

      do  {
        // write our value in
      *pBuf = val;

        // adjust the buffer forward 8 rows worth of data
        pBuf += WIDTH;

        // adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
        h -= 8;
      } while(h >= 8);
      }
    }

  // now do the final partial byte, if necessary
  if(h) {
    mod = h & 7;
    // this time we want to mask the low bits of the byte, vs the high bits we did above
    // register uint8_t mask = (1 << mod) - 1;
    // note - lookup table results in a nearly 10% performance improvement in fill* functions
    static uint8_t postmask[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F };
    register uint8_t mask = postmask[mod];
    switch (color)
    {
      case WHITE:   *pBuf |=  mask;  break;
      case BLACK:   *pBuf &= ~mask;  break;
      case INVERSE: *pBuf ^=  mask;  break;
    }
  }
}
/*
void Renderer::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (jdrawPixel)(*jdrawPixel)(x,y,color);
}
*/

// the most basic function, set a single pixel
void Renderer::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (!buffer) return;
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
    switch (color)
    {
      case WHITE:   buffer[x+ (y/8)*WIDTH] |=  (1 << (y&7)); break;
      case BLACK:   buffer[x+ (y/8)*WIDTH] &= ~(1 << (y&7)); break;
      case INVERSE: buffer[x+ (y/8)*WIDTH] ^=  (1 << (y&7)); break;
    }

}

void Renderer::setDrawMode(uint8_t mode) {
  drawmode=mode;
}

void VButton::xdrawButton(bool inverted) {
  wr_redir=1;
  drawButton(inverted);
  wr_redir=0;
}


/* END OF FILE */
