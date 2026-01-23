#ifndef _UDISPLAY_I2C_PANEL_H_
#define _UDISPLAY_I2C_PANEL_H_

#include <Arduino.h>
#include <Wire.h>
#include "uDisplay_panel.h"

/**
 * Configuration for I2C displays (typically OLED like SSD1306)
 */
struct I2CPanelConfig {
    // ===== Display Dimensions =====
    uint16_t width;
    uint16_t height;
    uint8_t bpp;              // bits per pixel (typically 1 for OLED)

    // ===== I2C Configuration =====
    uint8_t i2c_address;      // I2C device address
    TwoWire* wire;            // I2C bus instance

    // ===== Display Commands =====
    uint8_t cmd_set_addr_x;   // Set column address command
    uint8_t cmd_set_addr_y;   // Set page address command
    uint8_t cmd_write_ram;    // Write data command
    
    // ===== Display Control Commands =====
    uint8_t cmd_display_on;
    uint8_t cmd_display_off;
    uint8_t cmd_invert_on;
    uint8_t cmd_invert_off;

    // ===== Address Range =====
    uint8_t page_start;       // Starting page
    uint8_t page_end;         // Ending page
    uint8_t col_start;        // Starting column
    uint8_t col_end;          // Ending column

    // ===== Initialization =====
    uint8_t* init_commands;
    uint16_t init_commands_count;
};

class i2c_panel : public UniversalPanel {

public:
    /**
     * Constructor - receives configuration struct and framebuffer
     */
    i2c_panel(const I2CPanelConfig& config, uint8_t* framebuffer);
    
    bool updateFrame() override;
    bool displayOnff(int8_t on) override;
    bool invertDisplay(bool invert) override;
    bool setRotation(uint8_t rotation) override { return false; }
    
    bool drawPixel(int16_t x, int16_t y, uint16_t color) override { return false; }
    bool fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override { return false; }
    bool pushColors(uint16_t *data, uint32_t len, bool first = false) override { return false; }
    bool setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) override { return false; }
    bool drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override { return false; }
    bool drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override { return false; }

    uint8_t* framebuffer = nullptr;

private:
    // ===== Hardware & Configuration =====
    I2CPanelConfig cfg;        // Copy of config
    
    // ===== Internal Helpers =====
    void i2c_command(uint8_t val);
};

#endif // _UDISPLAY_I2C_PANEL_H_