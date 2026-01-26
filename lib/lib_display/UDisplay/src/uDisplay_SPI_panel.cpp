// WIP
// ======================================================
// uDisplay_spi_panel.cpp - SPI LCD Panel Implementation
// ======================================================

#include "uDisplay_SPI_panel.h"
#include <Arduino.h>
extern void AddLog(uint32_t loglevel, const char* formatP, ...);


SPIPanel::SPIPanel(const SPIPanelConfig& config,
                   SPIController* spi_ctrl,
                   uint8_t* framebuffer)
    : spi(spi_ctrl), cfg(config), fb_buffer(framebuffer), 
      rotation(0), display_on(true), inverted(false)
{
    width = cfg.width;
    height = cfg.height;
    // Initialize address window state
    window_x0 = 0;
    window_y0 = 0;
    window_x1 = 0;
    window_y1 = 0;
    use_hw_spi = (spi->spi_config.dc >= 0) && (spi->spi_config.bus_nr <= 2);
}

SPIPanel::~SPIPanel() {
    // Panel doesn't own framebuffer or SPI controller
}


// ===== UniversalPanel Interface Implementation =====

bool SPIPanel::drawPixel(int16_t x, int16_t y, uint16_t color) {
    // From original uDisplay::drawPixel - only handle direct SPI drawing for color TFTs
    if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) return true;

    // Only handle direct SPI drawing for color displays without framebuffer
    if (!fb_buffer && cfg.bpp >= 16) {
        spi->beginTransaction();
        spi->csLow();
        setAddrWindow_internal(x, y, 1, 1);
        spi->writeCommand(cfg.cmd_write_ram);
        
        if (cfg.col_mode == 18) {
            // From original WriteColor function
            uint8_t r = (color & 0xF800) >> 11;
            uint8_t g = (color & 0x07E0) >> 5;
            uint8_t b = color & 0x001F;
            r = (r * 255) / 31;
            g = (g * 255) / 63;
            b = (b * 255) / 31;
            spi->writeData8(r);
            spi->writeData8(g);
            spi->writeData8(b);
        } else {
            spi->writeData16(color);
        }
        spi->csHigh();
        spi->endTransaction();
        return true;
    }
    
    return false; // Let uDisplay handle framebuffer cases (monochrome OLEDs)
}

bool SPIPanel::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    // From original uDisplay::fillRect
    if((x >= width) || (y >= height)) return true;
    if((x + w - 1) >= width)  w = width - x;
    if((y + h - 1) >= height) h = height - y;

    // Only handle direct SPI drawing for color displays without framebuffer
    if (!fb_buffer && cfg.bpp >= 16) {
        spi->beginTransaction();
        spi->csLow();
        setAddrWindow_internal(x, y, w, h);
        spi->writeCommand(cfg.cmd_write_ram);
        
        if (cfg.col_mode == 18) {
            uint8_t r = (color & 0xF800) >> 11;
            uint8_t g = (color & 0x07E0) >> 5;
            uint8_t b = color & 0x001F;
            r = (r * 255) / 31;
            g = (g * 255) / 63;
            b = (b * 255) / 31;
            
            for (int16_t yp = h; yp > 0; yp--) {
                for (int16_t xp = w; xp > 0; xp--) {
                    spi->writeData8(r);
                    spi->writeData8(g);
                    spi->writeData8(b);
                }
            }
        } else {
            for (int16_t yp = h; yp > 0; yp--) {
                for (int16_t xp = w; xp > 0; xp--) {
                    spi->writeData16(color);
                }
            }
        }
        spi->csHigh();
        spi->endTransaction();
        return true;
    }
    
    return false; // Let uDisplay handle framebuffer cases (monochrome OLEDs)
}

bool SPIPanel::pushColors(uint16_t *data, uint32_t len, bool not_swapped) {
    // Only handle direct rendering for color displays
    if (cfg.bpp < 16) {
        return false;
    }
    
    // Handle byte swapping for LVGL (when not_swapped == false)
    if (!not_swapped && cfg.col_mode != 18) {
        // LVGL data - bytes are already swapped
        if (use_hw_spi) {
#ifdef ESP32
            spi->pushPixelsDMA(data, len);
#else
            spi->getSPI()->writeBytes((uint8_t*)data, len * 2);
#endif
        } else {
            // Software SPI - write pixel by pixel
            for (uint16_t i = 0; i < len; i++) {
                spi->writeData16(data[i]);
            }
        }
        return true;
    }
    
    // Handle 18-bit color mode
    if (cfg.col_mode == 18) {
#ifdef ESP32
        if (use_hw_spi) {
            uint8_t *line = (uint8_t*)malloc(len * 3);
            if (line) {
                uint8_t *lp = line;
                for (uint32_t cnt = 0; cnt < len; cnt++) {
                    uint16_t color = data[cnt];
                    if (!not_swapped) {
                        color = (color << 8) | (color >> 8);
                    }
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
                spi->pushPixels3DMA(line, len);
                free(line);
            }
        } else
#endif
        {
            // Software SPI or ESP8266
            for (uint16_t i = 0; i < len; i++) {
                uint16_t color = data[i];
                if (!not_swapped) {
                    color = (color << 8) | (color >> 8);
                }
                uint8_t r = (color & 0xF800) >> 11;
                uint8_t g = (color & 0x07E0) >> 5;
                uint8_t b = color & 0x001F;
                r = (r * 255) / 31;
                g = (g * 255) / 63;
                b = (b * 255) / 31;
                spi->writeData8(r);
                spi->writeData8(g);
                spi->writeData8(b);
            }
        }
        return true;
    }
    
    // Handle 16-bit color mode with no byte swapping (not_swapped == true)
    if (not_swapped) {
        if (use_hw_spi) {
#ifdef ESP32
            spi->getSPI()->writePixels(data, len * 2);
#else
            // ESP8266: writePixels() doesn't exist, use per-pixel write
            for (uint16_t i = 0; i < len; i++) {
                spi->writeData16(data[i]);
            }
#endif
            return true;
        }
        // Software SPI - write per-pixel
        for (uint16_t i = 0; i < len; i++) {
            spi->writeData16(data[i]);
        }
        return true;
    }
    
    return false;
}

bool SPIPanel::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    // From original uDisplay::setAddrWindow
    window_x0 = x0;
    window_y0 = y0;
    window_x1 = x1;
    window_y1 = y1;
    if (!x0 && !y0 && !x1 && !y1) {
        spi->csHigh();
        spi->endTransaction();
    } else {
        spi->beginTransaction();
        spi->csLow();
        setAddrWindow_internal(x0, y0, x1 - x0, y1 - y0);
    }
    return true;
}

void SPIPanel::setAddrWindow_internal(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // From original uDisplay::setAddrWindow_int
    x += cfg.x_addr_offset[rotation];
    y += cfg.y_addr_offset[rotation];
    uint16_t x2 = x + w - 1;
    uint16_t y2 = y + h - 1;

    if (cfg.address_mode != 8) {
        // 16/32-bit addressing (most TFT displays)
        uint32_t xa = ((uint32_t)x << 16) | x2;
        uint32_t ya = ((uint32_t)y << 16) | y2;

        spi->writeCommand(cfg.cmd_set_addr_x);
        spi->writeData32(xa);

        spi->writeCommand(cfg.cmd_set_addr_y);
        spi->writeData32(ya);

        if (cfg.cmd_write_ram != 0xFF) {
            spi->writeCommand(cfg.cmd_write_ram);
        }
    } else {
        // 8-bit addressing mode (OLED displays)
        if (rotation & 1) {
            // Vertical address increment mode
            uint16_t temp = x; x = y; y = temp;
            temp = x2; x2 = y2; y2 = temp;
        }
        
        spi->writeCommand(cfg.cmd_set_addr_x);
        if (cfg.all_commands_mode) {
            spi->writeData8(x);
            spi->writeData8(x2);
        } else {
            spi->writeCommand(x);
            spi->writeCommand(x2);
        }
        
        spi->writeCommand(cfg.cmd_set_addr_y);
        if (cfg.all_commands_mode) {
            spi->writeData8(y);
            spi->writeData8(y2);
        } else {
            spi->writeCommand(y);
            spi->writeCommand(y2);
        }
        
        if (cfg.cmd_write_ram != 0xFF) {
            spi->writeCommand(cfg.cmd_write_ram);
        }
    }
}

bool SPIPanel::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    // From original uDisplay::drawFastHLine
    return fillRect(x, y, w, 1, color);
}

bool SPIPanel::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    // From original uDisplay::drawFastVLine
    return fillRect(x, y, 1, h, color);
}

bool SPIPanel::displayOnff(int8_t on) {
    display_on = (on != 0);
    
    spi->beginTransaction();
    spi->csLow();
    if (display_on && cfg.cmd_display_on != 0xFF) {
        spi->writeCommand(cfg.cmd_display_on);
    } else if (!display_on && cfg.cmd_display_off != 0xFF) {
        spi->writeCommand(cfg.cmd_display_off);
    }
    spi->csHigh();
    spi->endTransaction();
    
    return false; //true;
}

bool SPIPanel::invertDisplay(bool invert) {
    inverted = invert;
    
    spi->beginTransaction();
    spi->csLow();
    if (invert && cfg.cmd_invert_on != 0xFF) {
        spi->writeCommand(cfg.cmd_invert_on);
    } else if (!invert && cfg.cmd_invert_off != 0xFF) {
        spi->writeCommand(cfg.cmd_invert_off);
    }
    spi->csHigh();
    spi->endTransaction();
    
    return true;
}

bool SPIPanel::setRotation(uint8_t rot) {
    // From original uDisplay::setRotation
    rotation = rot & 3;
    spi->beginTransaction();
    spi->csLow();

    if (cfg.cmd_memory_access != 0xFF && cfg.rot_cmd[rotation] != 0xFF) {
        spi->writeCommand(cfg.cmd_memory_access);
        if (!cfg.all_commands_mode) {
            spi->writeData8(cfg.rot_cmd[rotation]);
        } else {
            spi->writeCommand(cfg.rot_cmd[rotation]);
        }
        spi->csHigh();
        spi->endTransaction();
        switch (rotation) {
            case 0:
            case 2:
                width = cfg.width;
                height = cfg.height;
                break;
            case 1:
            case 3:
                width = cfg.height;
                height = cfg.width;
                break;
        }
        return true;
    }
    spi->csHigh();
    spi->endTransaction();
    return false;
}

bool SPIPanel::updateFrame() {
    // From original uDisplay::Updateframe - only for monochrome SPI OLEDs
    // Only handle framebuffer updates for monochrome displays
    if (!fb_buffer || cfg.bpp != 1) return false;
    
    // OLED page-based framebuffer update (from original code)
    uint8_t ys = height >> 3;
    uint8_t xs = width >> 3;
    uint8_t m_row = cfg.cmd_set_addr_y; // saw_2 in original
    uint8_t m_col = 0; // i2c_col_start in original

    uint16_t p = 0;
    uint8_t i, j, k = 0;

    spi->beginTransaction();
    spi->csLow();
    for (i = 0; i < ys; i++) {
        spi->writeCommand(0xB0 + i + m_row); // set page address
        spi->writeCommand(m_col & 0xf); // set lower column address
        spi->writeCommand(0x10 | (m_col >> 4)); // set higher column address

        for (j = 0; j < 8; j++) {
            for (k = 0; k < xs; k++, p++) {
                spi->writeData8(fb_buffer[p]);
            }
        }
    }
    spi->csHigh();
    spi->endTransaction();
    return true;
}