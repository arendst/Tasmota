// ======================================================
// uDisplay_rgb_panel.h - RGB Panel Implementation  
// ======================================================

#pragma once
#ifdef ESP32
#if __has_include("soc/soc_caps.h")
# include "soc/soc_caps.h"
#else
# error "No ESP capability header found"
#endif
#endif

#if SOC_LCD_RGB_SUPPORTED

#include "uDisplay_panel.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

class RGBPanel : public UniversalPanel {
public:
    // Takes only the ESP-IDF config
    RGBPanel(const esp_lcd_rgb_panel_config_t *config);
    ~RGBPanel();
    
    bool drawPixel(int16_t x, int16_t y, uint16_t color) override;
    bool fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    bool pushColors(uint16_t *data, uint32_t len, bool first = false) override;
    bool setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) override;
    bool drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    bool drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;

    bool displayOnff(int8_t on) override;
    bool invertDisplay(bool invert) override;
    bool setRotation(uint8_t rotation) override;
    bool updateFrame() override;
    uint16_t* framebuffer = nullptr;

private:
    esp_lcd_panel_handle_t panel_handle = nullptr;
    uint8_t rotation = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    int16_t window_x1 = 0;
    int16_t window_y1 = 0; 
    int16_t window_x2 = 1;
    int16_t window_y2 = 1;
    size_t framebuffer_size = 0;
    uint32_t framebuffer_dirty = false;

};

#endif //SOC_LCD_RGB_SUPPORTED