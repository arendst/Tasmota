// WIP - NOT REALLY IMPLEMENTED!!!
// ======================================================
// uDisplay_DSI_panel.h - MIPI-DSI Display Panel Implementation  
// ======================================================

#pragma once
#ifdef ESP32
#if __has_include("soc/soc_caps.h")
# include "soc/soc_caps.h"
#else
# error "No ESP capability header found"
#endif

#if SOC_MIPI_DSI_SUPPORTED

#include "uDisplay_panel.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_ldo_regulator.h"

struct DSIPanelConfig {
    // Basic display info
    uint16_t width;
    uint16_t height;
    uint8_t bpp;                    // 24
    
    // DSI parameters (from :H line)
    uint8_t dsi_lanes;              // 2
    int8_t te_pin;                  // -1 (no TE)
    int8_t reset_pin;               // -1 (no reset control)
    int ldo_channel;                // 3  
    int ldo_voltage_mv;             // 2500
    uint32_t pixel_clock_hz;        // 54000000
    uint32_t lane_speed_mbps;       // 750
    uint8_t rgb_order;              // 0=RGB, 1=BGR
    uint8_t data_endian;            // 0=Big, 1=Little
    
    // Video timing (from :V line)
    struct {
        uint16_t h_front_porch;     // 160
        uint16_t v_front_porch;     // 40  
        uint16_t h_back_porch;      // 160
        uint16_t h_sync_pulse;      // 12
        uint16_t v_sync_pulse;      // 10
        uint16_t v_back_porch;      // 23
    } timing;
    
    // Init commands (from :I section)
    uint8_t* init_commands;
    uint16_t init_commands_count;
    
    // Display on/off commands (from :O and :o lines)
    uint8_t cmd_display_on;   // 0x29
    uint8_t cmd_display_off;  // 0x28
};

class DSIPanel : public UniversalPanel {
public:
    // Constructor - takes ESP-IDF panel handle (already initialized)
    DSIPanel(const DSIPanelConfig& config);
    ~DSIPanel();
    
    // Core graphics API (must return bool)
    bool drawPixel(int16_t x, int16_t y, uint16_t color) override;
    bool fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    bool pushColors(uint16_t *data, uint32_t len, bool not_swapped) override;
    bool setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) override;
    bool drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    bool drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;

    // Control API
    bool displayOnff(int8_t on) override;
    bool invertDisplay(bool invert) override;
    bool setRotation(uint8_t rotation) override;
    bool updateFrame() override;
    
    // Get direct framebuffer access (for DPI mode)
    uint16_t* framebuffer = nullptr;

private:
    // ESP-IDF panel handle
    esp_lcd_panel_handle_t panel_handle = nullptr;
    esp_lcd_panel_io_handle_t io_handle = nullptr;
    esp_ldo_channel_handle_t ldo_handle = nullptr;
    DSIPanelConfig cfg;
    void sendInitCommandsDBI();
    
    // Display parameters
    uint8_t rotation = 0;

    // Address window tracking
    int16_t window_x0 = 0;
    int16_t window_y0 = 0;
    int16_t window_x1 = 0;
    int16_t window_y1 = 0;
    size_t framebuffer_size = 0;
    uint32_t framebuffer_dirty = false;
};

#endif // SOC_MIPI_DSI_SUPPORTED
#endif // ESP32