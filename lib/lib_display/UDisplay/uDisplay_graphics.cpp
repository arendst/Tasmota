#include "uDisplay.h"
#include "uDisplay_config.h"
#include "uDisplay_spi.h"

// ===== Basic Drawing Primitives =====

static constexpr uint16_t RGB16_TO_MONO      = 0x8410;
static constexpr uint16_t RGB16_SWAP_TO_MONO = 0x1084;
#define renderer_swap(a, b) { int16_t t = a; a = b; b = t; }

void uDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
#ifdef USE_ESP32_S3
    if (interface == _UDSP_RGB) {
        drawPixel_RGB(x, y, color);
        return;
    }
#endif

    if (ep_mode) {
        drawPixel_EPD(x, y, color);
        return;
    }

    if (framebuffer) {
        Renderer::drawPixel(x, y, color);
        return;
    }

    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    setAddrWindow_int(x, y, 1, 1);
    WriteColor(color);
    SPI_CS_HIGH
    SPI_END_TRANSACTION
}

void uDisplay::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if (ep_mode) {
        drawFastHLine_EPD(x, y, w, color);
        return;
    }

    if (framebuffer) {
        Renderer::drawFastHLine(x, y, w, color);
        return;
    }

    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) return;
    if((x + w - 1) >= _width)  w = _width - x;

#ifdef USE_ESP32_S3
    if (interface == _UDSP_RGB) {
        if (cur_rot > 0) {
            while (w--) {
                drawPixel_RGB(x , y , color);
                x++;
            }
        } else {
            uint16_t *fb = rgb_fb;
            fb += (int32_t)y * _width;
            fb += x;
            while (w--) {
                *fb = color;
                Cache_WriteBack_Addr((uint32_t)fb, 2);
                fb++;
                x++;
            }
        }
        return;
    }
#endif

    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    setAddrWindow_int(x, y, w, 1);

    if (col_mode == 18) {
        uint8_t r = (color & 0xF800) >> 11;
        uint8_t g = (color & 0x07E0) >> 5;
        uint8_t b = color & 0x001F;
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;

        while (w--) {
            ulcd_data8(r);
            ulcd_data8(g);
            ulcd_data8(b);
        }
    } else {
        while (w--) {
            WriteColor(color);
        }
    }

    SPI_CS_HIGH
    SPI_END_TRANSACTION
}

void uDisplay::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if (ep_mode) {
        drawFastVLine_EPD(x, y, h, color);
        return;
    }

    if (framebuffer) {
        Renderer::drawFastVLine(x, y, h, color);
        return;
    }

    // Rudimentary clipping
    if ((x >= _width) || (y >= _height)) return;
    if ((y + h - 1) >= _height) h = _height - y;

#ifdef USE_ESP32_S3
    if (interface == _UDSP_RGB) {
        if (cur_rot > 0) {
            while (h--) {
                drawPixel_RGB(x , y , color);
                y++;
            }
        } else {
            uint16_t *fb = rgb_fb;
            fb += (int32_t)y * _width;
            fb += x;
            while (h--) {
                *fb = color;
                Cache_WriteBack_Addr((uint32_t)fb, 2);
                fb+=_width;
                y++;
            }
        }
        return;
    }
#endif

    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    setAddrWindow_int(x, y, 1, h);

    if (col_mode == 18) {
        uint8_t r = (color & 0xF800) >> 11;
        uint8_t g = (color & 0x07E0) >> 5;
        uint8_t b = color & 0x001F;
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;

        while (h--) {
            ulcd_data8(r);
            ulcd_data8(g);
            ulcd_data8(b);
        }
    } else {
        while (h--) {
            WriteColor(color);
        }
    }

    SPI_CS_HIGH
    SPI_END_TRANSACTION
}

void uDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
#ifdef USE_ESP32_S3
    if (interface == _UDSP_RGB) {
        for (uint32_t yp = y; yp < y + h; yp++) {
            drawFastHLine(x, yp, w, color);
        }
        return;
    }
#endif

    if (ep_mode) {
        fillRect_EPD(x, y, w, h, color);
        return;
    }

    if (framebuffer) {
        Renderer::fillRect(x, y, w, h, color);
        return;
    }

    if((x >= _width) || (y >= _height)) return;
    if((x + w - 1) >= _width)  w = _width - x;
    if((y + h - 1) >= _height) h = _height - y;

    SPI_BEGIN_TRANSACTION
    SPI_CS_LOW
    setAddrWindow_int(x, y, w, h);

    if (col_mode == 18) {
        uint8_t r = (color & 0xF800) >> 11;
        uint8_t g = (color & 0x07E0) >> 5;
        uint8_t b = color & 0x001F;
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;

        for (y = h; y > 0; y--) {
            for (x = w; x > 0; x--) {
                ulcd_data8(r);
                ulcd_data8(g);
                ulcd_data8(b);
            }
        }
    } else {
        for (y = h; y > 0; y--) {
            for (x = w; x > 0; x--) {
                WriteColor(color);
            }
        }
    }
    
    SPI_CS_HIGH
    SPI_END_TRANSACTION
}

void uDisplay::fillScreen(uint16_t color) {
    fillRect(0, 0, width(), height(), color);
}

static inline void lvgl_color_swap(uint16_t *data, uint16_t len) { for (uint32_t i = 0; i < len; i++) (data[i] = data[i] << 8 | data[i] >> 8); }

void uDisplay::pushColors(uint16_t *data, uint16_t len, boolean not_swapped) {

  if (lvgl_param.swap_color) {
    not_swapped = !not_swapped;
  }

  //Serial.printf("push %x - %d - %d - %d\n", (uint32_t)data, len, not_swapped, lvgl_param.data);

  // Isolating _UDSP_RGB to increase code sharing
  //
  // Use ESP-IDF LCD driver to push colors and rely on the following assumptions:
  // * bytes swapping is already handled in the driver configuration (see uDisplay::Init()),
  // * pushColors() is only called with not_swapped equals true,
  // * cache flushing is done by the LCD driver.
  if (interface == _UDSP_RGB) {
#ifdef USE_ESP32_S3
    if (!not_swapped) {
      // internal error -> write error message but continue (with possibly wrong colors)
      AddLog(LOG_LEVEL_ERROR, PSTR("DSP: Unexpected byte-swapping requested in pushColors()"));
    }

    // check that bytes count matches the size of area, and remove from inner loop
    if ((seta_yp2 - seta_yp1) * (seta_xp2 - seta_xp2) > len) { return; }

    esp_lcd_panel_draw_bitmap(_panel_handle, seta_xp1, seta_yp1, seta_xp2, seta_yp2, (void *)data);
#endif
    return;
  }


  if (not_swapped == false) {
    // called from LVGL bytes are swapped
    if (bpp != 16) {
      // lvgl_color_swap(data, len); -- no need to swap anymore, we have inverted the mask
      pushColorsMono(data, len, true);
      return;
    }

    if ( (col_mode != 18) && (spi_dc >= 0) && (spi_nr <= 2) ) {
      // special version 8 bit spi I or II
#ifdef ESP8266
      lvgl_color_swap(data, len);
      while (len--) {
        uspi->write(*data++);
      }
#else
      if (lvgl_param.use_dma) {
        pushPixelsDMA(data, len );
      } else {
        uspi->writeBytes((uint8_t*)data, len * 2);
      }
#endif
    } else {

#ifdef ESP32
      if ( (col_mode == 18) && (spi_dc >= 0) && (spi_nr <= 2) ) {
        uint8_t *line = (uint8_t*)malloc(len * 3);
        uint8_t *lp = line;
        if (line) {
          uint16_t color;
          for (uint32_t cnt = 0; cnt < len; cnt++) {
            color = *data++;
            color = (color << 8) | (color >> 8);
            uint8_t r = (color & 0xF800) >> 11;
            uint8_t g = (color & 0x07E0) >> 5;
            uint8_t b = color & 0x001F;
            r = (r * 255) / 31;
            g = (g * 255) / 63;
            b = (b * 255) / 31;
            *lp++ = r;
            *lp++ = g;
            *lp++ = b;
          }

          if (lvgl_param.use_dma) {
            pushPixels3DMA(line, len );
          } else {
            uspi->writeBytes(line, len * 3);
          }
          free(line);
        }

      } else {
        // 9 bit and others
        if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
  #ifdef USE_ESP32_S3
          pb_pushPixels(data, len, true, false);
  #endif // USE_ESP32_S3
        } else {
          lvgl_color_swap(data, len);
          while (len--) {
            WriteColor(*data++);
          }
        }
      }
#endif // ESP32

#ifdef ESP8266
      lvgl_color_swap(data, len);
      while (len--) {
        WriteColor(*data++);
      }
#endif
    }
  } else {
    // called from displaytext, no byte swap, currently no dma here

    if (bpp != 16) {
      pushColorsMono(data, len);
      return;
    }
    if ( (col_mode != 18) && (spi_dc >= 0) && (spi_nr <= 2) ) {
      // special version 8 bit spi I or II
  #ifdef ESP8266
      while (len--) {
        //uspi->write(*data++);
        WriteColor(*data++);
      }
  #else
      uspi->writePixels(data, len * 2);
  #endif
    } else {
      // 9 bit and others
      if (interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
#ifdef USE_ESP32_S3
        pb_pushPixels(data, len, false, false);
#endif // USE_ESP32_S3
      } else {
        while (len--) {
          WriteColor(*data++);
        }
      }
    }
  }
}

// convert to mono, these are framebuffer based
void uDisplay::pushColorsMono(uint16_t *data, uint16_t len, bool rgb16_swap) {
  // pixel is white if at least one of the 3 components is above 50%
  // this is tested with a simple mask, swapped if needed
  uint16_t rgb16_to_mono_mask = rgb16_swap ? RGB16_SWAP_TO_MONO : RGB16_TO_MONO;

  for (uint32_t y = seta_yp1; y < seta_yp2; y++) {
    seta_yp1++;
    if (lvgl_param.invert_bw) {
      for (uint32_t x = seta_xp1; x < seta_xp2; x++) {
        uint16_t color = *data++;
        if (bpp == 1) color = (color & rgb16_to_mono_mask) ? 0 : 1;
        drawPixel(x, y, color);   // todo - inline the method to save speed
        len--;
        if (!len) return;         // failsafe - exist if len (pixel number) is exhausted
      }
    } else {
      for (uint32_t x = seta_xp1; x < seta_xp2; x++) {
        uint16_t color = *data++;
        if (bpp == 1) color = (color & rgb16_to_mono_mask) ? 1 : 0;
        drawPixel(x, y, color);   // todo - inline the method to save speed
        len--;
        if (!len) return;         // failsafe - exist if len (pixel number) is exhausted
      }
    }
  }
}

void uDisplay::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    if (bpp != 16 || interface == _UDSP_RGB) {
        // Just save params or update frame
        if (!x0 && !y0 && !x1 && !y1) {
            if (!ep_mode) {
                Updateframe();
            }
        } else {
            seta_xp1 = x0;
            seta_xp2 = x1;
            seta_yp1 = y0;
            seta_yp2 = y1;
        }
        return;
    }

    if (interface == _UDSP_RGB) {
        return;
    }

    if (!x0 && !y0 && !x1 && !y1) {
        SPI_CS_HIGH
        SPI_END_TRANSACTION
    } else {
        SPI_BEGIN_TRANSACTION
        SPI_CS_LOW
        setAddrWindow_int(x0, y0, x1 - x0, y1 - y0);
    }
}

void uDisplay::setAddrWindow_int(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    if (interface == _UDSP_RGB) {
        return;
    }

    x += x_addr_offs[cur_rot];
    y += y_addr_offs[cur_rot];

    if (sa_mode != 8) {
        uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
        uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);

        ulcd_command(saw_1);
        ulcd_data32(xa);

        ulcd_command(saw_2);
        ulcd_data32(ya);

        if (saw_3 != 0xff) {
            ulcd_command(saw_3); // write to RAM
        }
    } else {
        uint16_t x2 = x + w - 1,
                 y2 = y + h - 1;

        if (cur_rot & 1) { // Vertical address increment mode
            renderer_swap(x,y);
            renderer_swap(x2,y2);
        }
        ulcd_command(saw_1);
        if (allcmd_mode) {
            ulcd_data8(x);
            ulcd_data8(x2);
        } else {
            ulcd_command(x);
            ulcd_command(x2);
        }
        ulcd_command(saw_2);
        if (allcmd_mode) {
            ulcd_data8(y);
            ulcd_data8(y2);
        } else {
            ulcd_command(y);
            ulcd_command(y2);
        }
        if (saw_3 != 0xff) {
            ulcd_command(saw_3); // write to RAM
        }
    }
}

void uDisplay::setRotation(uint8_t rotation) {
    cur_rot = rotation;

    if (framebuffer) {
        Renderer::setRotation(cur_rot);
        return;
    }

    if (interface == _UDSP_SPI || interface == _UDSP_PAR8 || interface == _UDSP_PAR16) {
        if (ep_mode) {
            Renderer::setRotation(cur_rot);
            return;
        }
        SPI_BEGIN_TRANSACTION
        SPI_CS_LOW
        ulcd_command(madctrl);

        if (!allcmd_mode) {
            ulcd_data8(rot[cur_rot]);
        } else {
            ulcd_command(rot[cur_rot]);
        }

        if ((sa_mode == 8) && !allcmd_mode) {
            ulcd_command(startline);
            ulcd_data8((cur_rot < 2) ? height() : 0);
        }

        SPI_CS_HIGH
        SPI_END_TRANSACTION
    }
    
    switch (rotation) {
        case 0:
            _width  = gxs;
            _height = gys;
            break;
        case 1:
            _width  = gys;
            _height = gxs;
            break;
        case 2:
            _width  = gxs;
            _height = gys;
            break;
        case 3:
            _width  = gys;
            _height = gxs;
            break;
    }

#ifdef USE_ESP32_S3
    if (interface == _UDSP_RGB) {
        // Utilize the ESP-IDF LCD driver's support for display rotation
        esp_lcd_panel_mirror(_panel_handle, rotation == 1 || rotation == 2, rotation & 2);
        esp_lcd_panel_swap_xy(_panel_handle, rotation & 1);
    }
#endif
}

#ifdef USE_ESP32_S3
void uDisplay::drawPixel_RGB(int16_t x, int16_t y, uint16_t color) {
    int16_t w = _width, h = _height;

    if ((x < 0) || (x >= w) || (y < 0) || (y >= h)) {
        return;
    }

    // check rotation, move pixel around if necessary
    switch (cur_rot) {
    case 1:
        renderer_swap(w, h);
        renderer_swap(x, y);
        x = w - x - 1;
        break;
    case 2:
        x = w - x - 1;
        y = h - y - 1;
        break;
    case 3:
        renderer_swap(w, h);
        renderer_swap(x, y);
        y = h - y - 1;
        break;
    }

    uint16_t *fb = rgb_fb;
    fb += (int32_t)y * w;
    fb += x;
    *fb = color;
    Cache_WriteBack_Addr((uint32_t)fb, 2);
}
#endif