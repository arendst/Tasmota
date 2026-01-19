#include "uDisplay.h"
#include "uDisplay_config.h"

// ===== Basic Drawing Primitives =====

static constexpr uint16_t RGB16_TO_MONO      = 0x8410;
static constexpr uint16_t RGB16_SWAP_TO_MONO = 0x1084;

void uDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (universal_panel->drawPixel(x, y, color)) {
        return; // Handled by universal panel
    }

    if (framebuffer) {
        Renderer::drawPixel(x, y, color);
        return;
    }
}

void uDisplay::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) return;
    if((x + w - 1) >= _width)  w = _width - x;

    if (universal_panel->drawFastHLine(x, y, w, color)) {
        return;
    }

    if (framebuffer) {
        Renderer::drawFastHLine(x, y, w, color);
        return;
    }

}

void uDisplay::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if (framebuffer) {
        Renderer::drawFastVLine(x, y, h, color);
        return;
    }

    // Rudimentary clipping
    if ((x >= _width) || (y >= _height)) return;
    if ((y + h - 1) >= _height) h = _height - y;

    if (universal_panel->drawFastVLine(x, y, h, color)) {
        return;
    }


}

void uDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (universal_panel->fillRect(x, y, w, h, color)) {
        return;
    }

    if (framebuffer) {
        Renderer::fillRect(x, y, w, h, color);
        return;
    }
}

void uDisplay::fillScreen(uint16_t color) {
    fillRect(0, 0, width(), height(), color);
}

static inline void lvgl_color_swap(uint16_t *data, uint16_t len) { for (uint32_t i = 0; i < len; i++) (data[i] = data[i] << 8 | data[i] >> 8); }

void uDisplay::pushColors(uint16_t *data, uint32_t len, boolean not_swapped) {  //not_swapped is always true in call form LVGL driver!!!!

    if (lvgl_param.swap_color) {
        not_swapped = !not_swapped;
    }
    universal_panel->pushColors(data, len, not_swapped);
}

// convert to mono, these are framebuffer based
void uDisplay::pushColorsMono(uint16_t *data, uint32_t len, bool rgb16_swap) {
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
    universal_panel->setAddrWindow(x0, y0, x1, y1);
}

void uDisplay::setRotation(uint8_t rotation) {
    cur_rot = rotation;
    if (universal_panel->setRotation(rotation)) {
        // Update Renderer dimensions based on rotation
        switch (rotation) {
            case 0:
            case 2:
                _width = gxs;
                _height = gys;
                break;
            case 1:
            case 3:
                _width = gys;
                _height = gxs;
                break;
        }
        return;
    }

    if (framebuffer) {
        Renderer::setRotation(cur_rot);
        return;
    }
}

void uDisplay::Updateframe(void) {
    universal_panel->updateFrame();
}
