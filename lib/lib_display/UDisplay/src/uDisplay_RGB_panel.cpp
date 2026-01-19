// ======================================================
// panel/uDisplay_rgb_panel.cpp - RGB Panel Implementation
// ======================================================
#include "uDisplay_RGB_panel.h"

#if SOC_LCD_RGB_SUPPORTED

#include <cstdint>
#include <algorithm>
#include <rom/cache.h>

RGBPanel::RGBPanel(const esp_lcd_rgb_panel_config_t *config) {
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    width = config->timings.h_res;
    height = config->timings.v_res;
    framebuffer_size = width * height * 2; // 16 bpp
    void* buf = NULL;
    esp_lcd_rgb_panel_get_frame_buffer(panel_handle, 1, &buf);
    framebuffer = (uint16_t*)buf;
    uint16_t color = random(0xffff);
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 1, 1, &color));
}

RGBPanel::~RGBPanel() {
    // TODO: Cleanup panel_handle if needed
}

bool RGBPanel::drawPixel(int16_t x, int16_t y, uint16_t color) {
    int16_t w = width, h = height;
    
    // Apply rotation
    switch (rotation) {
    case 1: std::swap(w, h); std::swap(x, y); x = w - x - 1; break;
    case 2: x = w - x - 1; y = h - y - 1; break;
    case 3: std::swap(w, h); std::swap(x, y); y = h - y - 1; break;
    }
    
    if ((x < 0) || (x >= w) || (y < 0) || (y >= h)) return true; // Handled (out of bounds)
    
    framebuffer[y * w + x] = color;
    framebuffer_dirty = true;
    return true; // Handled by RGB panel
}

bool RGBPanel::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    for (int16_t yp = y; yp < y + h; yp++) {
        uint16_t* line_start = &framebuffer[yp * width + x];
        for (int16_t i = 0; i < w; i++) {
            line_start[i] = color;
        }
    }
    framebuffer_dirty = true;
    return true; // Handled by RGB panel
}

bool RGBPanel::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    window_x1 = x0;
    window_y1 = y0; 
    window_x2 = x1;
    window_y2 = y1;
    return true; // Handled by RGB panel
}

bool RGBPanel::pushColors(uint16_t *data, uint32_t len, bool first) {
    esp_lcd_panel_draw_bitmap(panel_handle, window_x1, window_y1, window_x2, window_y2, data);
    return true; // Handled by RGB panel
}

bool RGBPanel::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    uint16_t* line_start = &framebuffer[y * width + x];
    for (int16_t i = 0; i < w; i++) {
        line_start[i] = color;
    }
    CACHE_WRITEBACK_ADDR((uint32_t)line_start, w * 2);
    return true; // Handled by RGB panel
}

bool RGBPanel::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    for (int16_t j = 0; j < h; j++) {
        framebuffer[(y + j) * width + x] = color;
    }
    CACHE_WRITEBACK_ADDR((uint32_t)&framebuffer[y * width + x], h * 2);
    return true; // Handled by RGB panel
}

bool RGBPanel::displayOnff(int8_t on) {
    esp_lcd_panel_disp_on_off(panel_handle, on != 0);
    return false; // bpanel is controlled from display class
}

bool RGBPanel::invertDisplay(bool invert) {
    // TODO: Not supported by RGB panels in ESP-IDF API
    return false; // Not handled - let uDisplay handle if possible
}

bool RGBPanel::setRotation(uint8_t rotation) {
    this->rotation = rotation & 3;
    esp_lcd_panel_mirror(panel_handle, rotation == 1 || rotation == 2, rotation & 2);
    esp_lcd_panel_swap_xy(panel_handle, rotation & 1);
    return true; // Handled by RGB panel
}

bool RGBPanel::updateFrame() {
    if (!framebuffer_dirty) {
        return true;
    }
    CACHE_WRITEBACK_ADDR((uint32_t)framebuffer, framebuffer_size); //KISS and fast enough!
    framebuffer_dirty = false;

    return true; // Handled (no-op is still handled)
}


#endif // #if SOC_LCD_RGB_SUPPORTED
