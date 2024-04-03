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



uint8_t wr_redir=0;

#define register

#define SPRINT(A) char str[32];sprintf(str,"val: %d ",A);Serial.println((char*)str);

#define OLED_FONT_WIDTH        6
#define OLED_FONT_HEIGTH       8
#define BLACK 0

#ifdef USE_GFX
Renderer::Renderer(int16_t x, int16_t y) : Adafruit_GFX(x, y) {
#else
Renderer::Renderer(int16_t x, int16_t y) {
#endif

  font=0;
#ifdef USE_EPD_FONTS
  selected_font = &Font12;
#endif
  disp_bpp = 16;
}

uint16_t Renderer::GetColorFromIndex(uint8_t index) {
  if (index>0) return 1;
  return 0;
}

void Renderer::dim(uint8_t contrast) {
  uint8_t contrast8 = ((uint32_t)contrast * 255) / 15;
  dim10(contrast8, contrast8 * 4);
}

void Renderer::dim10(uint8_t contrast, uint16_t contrast_gamma) {

}

void Renderer::pushColors(uint16_t *data, uint16_t len, boolean first) {

}

void Renderer::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

}

void Renderer::DisplayOnff(int8_t on) {

}

void Renderer::DisplayInit(int8_t p,int8_t size,int8_t rot,int8_t font) {

}

void Renderer::Begin(int16_t p1,int16_t p2,int16_t p3) {

}

void Renderer::Sleep(void) {

}

void Renderer::Updateframe() {

}

void Renderer::TS_RotConvert(int16_t *x, int16_t *y) {

}

uint8_t *Renderer::allocate_framebuffer(uint32_t size) {
  if (framebuffer) free(framebuffer);
  framebuffer = (unsigned char*)calloc(size, 1);
  if (!framebuffer) return 0;
  return framebuffer;
}


void Renderer::setTextSize(uint8_t sf) {
  if (sf < 1) sf = 1;
  if (sf > 16) sf = 16;
  tsize = sf;
  Adafruit_GFX::setTextSize(sf);
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
    uint8_t sf = tsize;

    for (j = 0; j < xfont->Height; j++) {
        for (i = 0; i < xfont->Width; i++) {
            if (pgm_read_byte(ptr) & (0x80 >> (i % 8))) {
                if (sf == 1) {
                  writePixel(x + i, y + j, colored);
                } else {
                  writeFillRect(x + i * sf, y + j * sf, sf, sf, colored);
                }
            } else {
              // fill background
                if (!drawmode) {
                  if (sf == 1) {
                    writePixel(x + i, y + j, textbgcolor);
                  } else {
                    writeFillRect(x + i * sf, y + j * sf, sf, sf, textbgcolor);
                  }
                }
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

/*
    if (font == 5 && !drawmode) {
      // clear bckground
      int16_t x1,y1;
      uint16_t w,h;
      Adafruit_GFX::getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    }*/
#ifndef USE_EPD_FONTS
    font=0;
#endif

#ifndef USE_GFX_FONTS
    if (!font || font == 5) {
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
      x=(x-1)*xfont->Width*tsize;
      y=(y-1)*xfont->Height*tsize;
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
        refcolumn += xfont->Width*tsize;
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

extern uint8_t *loaded_font;

void Renderer::setTextFont(uint8_t f) {

  font = f;

  setFont(0);

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
      break;
    default:
      selected_font = &Font12;
      break;
  }
#else
#ifdef USE_EPD_FONTS
  switch (font) {
  case 1:
    selected_font = &Font12;
    break;
  case 2:
    selected_font = &Font24;
    break;
  case 3:
#ifdef USE_TINY_FONT
    selected_font = &Font8;
#else
    selected_font = &Font24;
#endif
    break;
  case 4:
#ifdef USE_7SEG_FONT
    selected_font = &Font24_7seg;
    break;
#endif
  case 5:
    setFont(ramfont);
    break;

  default:
    selected_font = &Font12;
    break;
  }
#endif
#endif
}


void Renderer::SetRamfont(uint8_t *font) {

  ramfont = (GFXfont*)font;
  if (font) {
    uint32_t bitmap_offset = (uint32_t)ramfont->bitmap;
    uint32_t glyph_offset = (uint32_t)ramfont->glyph;

    ramfont->bitmap = (uint8_t*)((uint32_t)font + bitmap_offset);
    ramfont->glyph = (GFXglyph*)((uint32_t)font + glyph_offset);
  }
  setFont(ramfont);
}

void Renderer::clearDisplay(void) {
  fillScreen(BLACK);
}

#define renderer_swap(a, b) { int16_t t = a; a = b; b = t; }

void Renderer::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  boolean bSwap = false;
  if (!framebuffer) return;
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
  register uint8_t *pBuf = framebuffer;
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
  if (!framebuffer) return;
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
  if (!framebuffer) return;
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
  register uint8_t *pBuf = framebuffer;
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
    switch (color)
    {
      case WHITE:   framebuffer[x+ (y/8)*WIDTH] |=  (1 << (y&7)); break;
      case BLACK:   framebuffer[x+ (y/8)*WIDTH] &= ~(1 << (y&7)); break;
      case INVERSE: framebuffer[x+ (y/8)*WIDTH] ^=  (1 << (y&7)); break;
    }

}


// this is called for every driver
void Renderer::setDrawMode(uint8_t mode) {
  drawmode=mode;
}

void Renderer::invertDisplay(boolean i) {
}

void Renderer::reverseDisplay(boolean i) {
}

void Renderer::setScrollMargins(uint16_t top, uint16_t bottom) {

}
void Renderer::scrollTo(uint16_t y) {

}

void Renderer::SetPwrCB(pwr_cb cb) {

}
void Renderer::SetDimCB(dim_cb cb) {

}

uint16_t Renderer::fgcol(void) {
  return 0;
}
uint16_t Renderer::bgcol(void) {
  return 0;
}
int8_t Renderer::color_type(void) {
 return 0;
}

bool Renderer::utouch_Init(char **name) {
  return false;
}

uint16_t Renderer::touched(void) {
  return false;
}

int16_t Renderer::getPoint_x(void) {
  return 0;
}

int16_t Renderer::getPoint_y(void) {
  return 0;
}

void Renderer::Splash(void) {

}

const char dname[1] = {0};

char *Renderer::devname(void) {
  return (char*)dname;
}

LVGL_PARAMS *Renderer::lvgl_pars(void) {
  return &lvgl_param;
}

void Renderer::ep_update_mode(uint8_t mode) {
}

void Renderer::ep_update_area(uint16_t xp, uint16_t yp, uint16_t width, uint16_t height, uint8_t mode) {
}

uint32_t Renderer::get_sr_touch(uint32_t xp, uint32_t xm, uint32_t yp, uint32_t ym) {
  return 0;
}

// #ifndef USE_DISPLAY_LVGL_ONLY

void VButton::xdrawButton(bool inverted) {
  wr_redir=1;
  drawButton(inverted);
  wr_redir=0;
}

void VButton::xinitButtonUL(Renderer *renderer, int16_t gxp, int16_t gyp, uint16_t gxs, uint16_t gys, uint16_t outline,\
  uint16_t fill, uint16_t textcolor , char *label, uint8_t textsize) {

  initButtonUL(renderer, gxp, gyp, gxs, gys, outline, fill, textcolor, label, textsize);

  spars.xp = gxp;
  spars.yp = gyp;
  spars.xs = gxs;
  spars.ys = gys;

}

boolean VButton::didhit(int16_t x, int16_t y) {
  return ((x >= spars.xp) && (x < (int16_t) (spars.xp + spars.xs)) &&
          (y >= spars.yp) && (y < (int16_t) (spars.yp + spars.ys)));
}

void VButton::SliderInit(Renderer *renderer, uint16_t xp, uint16_t yp, uint16_t xs, uint16_t ys, uint16_t nelem, uint16_t bgcol, uint16_t frcol, uint16_t barcol) {
  spars.xp = xp;
  spars.yp = yp;
  spars.xs = xs;
  spars.ys = ys;
  spars.nelem = nelem;
  spars.bgcol = bgcol;
  spars.frcol = frcol;
  spars.barcol = barcol;
  rend = renderer;

  rend->fillRect(spars.xp, spars.yp, spars.xs, spars.ys, spars.bgcol);

  if (xs < ys) {
    float bxs = spars.xs - 6;
    float bys = (float)(spars.ys - 6) / (float)nelem;
    float bxp = xp + 3;
    float byp = yp + 3;
    for (uint32_t count = 0; count < spars.nelem; count++) {
      rend->fillRect(bxp, byp, bxs, bys - 3, spars.barcol);
      rend->drawRect(bxp, byp, bxs, bys - 3, spars.frcol);
      byp += bys;
    }
  } else {
    float bys = spars.ys - 6;
    float bxs = (float)(spars.xs - 6) / (float)nelem;
    float byp = yp + 3;
    float bxp = xp + 3;
    for (uint32_t count = 0; count < spars.nelem; count++) {
      rend->fillRect(bxp, byp, bxs - 3 , bys, spars.barcol);
      rend->drawRect(bxp, byp, bxs - 3, bys, spars.frcol);
      bxp += bxs;
    }
  }
}

uint16_t VButton::UpdateSlider(int16_t x, int16_t y) {
  uint16_t elems = spars.nelem + 1;

  if (x < 0) {
    x = spars.xp + (-x * spars.xs) / 100;
    y = spars.yp + (spars.ys - (-y * spars.ys) / 100);
  }

  if (spars.xs < spars.ys) {
    uint16_t dy = spars.ys - (y - spars.yp);
    uint16_t limit =  elems - ((float)dy /(float)spars.ys * elems);
    float bxs = spars.xs - 6;
    float bys = (float)(spars.ys - 6) / (float)spars.nelem;
    float bxp = spars.xp + 3;
    float byp = spars.yp + 3;
    uint16_t col;
    for (uint32_t count = 0; count < spars.nelem; count++) {
      if (count >= limit) {
        col = spars.barcol;
      } else {
        col = spars.bgcol;
      }
      rend->fillRect(bxp, byp, bxs, bys - 3, col);
      rend->drawRect(bxp, byp, bxs, bys - 3, spars.frcol);
      byp += bys;
    }
    return 100 - (float(y - spars.yp) / (float)spars.ys) * 100.0;
  } else {
    uint16_t limit = (x - spars.xp) * elems / spars.xs;
    float bys = spars.ys - 6;
    float bxs = (float)(spars.xs - 6) / (float)spars.nelem;
    float byp = spars.yp + 3;
    float bxp = spars.xp + 3;
    uint16_t col;
    for (uint32_t count = 0; count < spars.nelem; count++) {
      if (count < limit) {
        col = spars.barcol;
      } else {
        col = spars.bgcol;
      }
      rend->fillRect(bxp, byp, bxs - 3, bys, col);
      rend->drawRect(bxp, byp, bxs - 3 , bys, spars.frcol);
      bxp += bxs;
    }
    return (float(x - spars.xp) / (float)spars.xs) * 100.0;
  }

}
// #endif // USE_DISPLAY_LVGL_ONLY




/* END OF FILE */
