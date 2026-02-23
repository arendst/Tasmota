// WIP
// ======================================================
// uDisplay_spi_panel.h - SPI LCD Panel Implementation
// ======================================================

#pragma once

#include "uDisplay_panel.h"
#include "uDisplay_SPI_controller.h"

typedef struct LVGL_PARAMS_t {
  uint16_t flushlines;
  union {
    uint8_t data;
    struct {
      uint8_t use_dma : 1;
      uint8_t swap_color : 1;
      uint8_t async_dma : 1;   // force DMA completion before returning, avoid conflict with other devices on same bus. If set you should make sure the display is the only device on the bus
      uint8_t busy_invert : 1;
      uint8_t invert_bw : 1;
      uint8_t resvd_3 : 1;
      uint8_t resvd_4 : 1;
      uint8_t resvd_5 : 1;
    };
  };
}LVGL_PARAMS_t;


/**
 * Configuration for SPI-based displays
 */
struct SPIPanelConfig {
    // ===== Display Dimensions =====
    uint16_t width;
    uint16_t height;
    uint8_t bpp;              // bits per pixel (1, 8, 16, etc.)
    uint8_t col_mode;         // color mode (16, 18, etc.)

    // ===== Address Window Protocol =====
    uint8_t cmd_set_addr_x;   // Command to set X address range
    uint8_t cmd_set_addr_y;   // Command to set Y address range  
    uint8_t cmd_write_ram;    // Command to write pixel data

    // ===== Display Control Commands =====
    uint8_t cmd_display_on;
    uint8_t cmd_display_off;
    uint8_t cmd_invert_on;
    uint8_t cmd_invert_off;
    uint8_t cmd_memory_access; // For rotation settings
    uint8_t cmd_startline;     // For vertical scroll offset

    // ===== Per-Rotation Configuration =====
    uint8_t rot_cmd[4];        // Memory access command variant for each rotation
    uint16_t x_addr_offset[4]; // Address offset per rotation
    uint16_t y_addr_offset[4];
    uint8_t address_mode;      // Addressing scheme (8, 16, 32-bit)

    // ===== Flags =====
    bool all_commands_mode;    // If true: send data bytes as commands

    // ===== Reset & Power Control =====
    int8_t reset_pin;          // GPIO for display reset (-1 if none)
    // int8_t busy_pin;        // REMOVED - busy_pin is EPD-only, moved to EPDPanelConfig
    int8_t bpanel;             // Backlight GPIO (-1 if none)
};

class SPIPanel : public UniversalPanel {
public:
    /**
     * Constructor - receives framebuffer from uDisplay if needed
     */
    SPIPanel(const SPIPanelConfig& config,
             SPIController* spi_ctrl,
             uint8_t* framebuffer);
    
    ~SPIPanel();
    
    // ===== UniversalPanel Interface =====
    bool drawPixel(int16_t x, int16_t y, uint16_t color) override;
    bool fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    bool pushColors(uint16_t *data, uint32_t len, bool not_swapped = false) override;
    bool setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) override;
    bool drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    bool drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
    
    bool displayOnff(int8_t on) override;
    bool invertDisplay(bool invert) override;
    bool setRotation(uint8_t rotation) override;
    bool updateFrame() override;

private:
    // ===== Hardware & Configuration =====
    SPIController* spi;            // Not owned by panel
    SPIPanelConfig cfg;            // Copy of config

    // ===== Framebuffer =====
    uint8_t* fb_buffer;            // Framebuffer (if provided by uDisplay)

    // ===== Display State =====
    uint8_t rotation;              // Current rotation (0-3)
    uint16_t width;                // Current width
    uint16_t height;               // Current height
    int16_t window_x0, window_y0, window_x1, window_y1;
    bool display_on;
    bool inverted;

    bool use_hw_spi = false;

    // ===== Internal Helpers =====
    void setAddrWindow_internal(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void sendAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void sendCommand(uint8_t cmd);
    void sendData8(uint8_t data);
    void sendData16(uint16_t data);
    void writeColor(uint16_t color);
    void resetDisplay();
    void waitBusy();
};