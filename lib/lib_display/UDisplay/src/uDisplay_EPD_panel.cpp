// ======================================================
// uDisplay_epd_panel.cpp - E-Paper Display Panel Implementation
// ======================================================

#include "uDisplay_EPD_panel.h"
#include <Arduino.h>

// EPD Command Definitions
static constexpr uint8_t DRIVER_OUTPUT_CONTROL                = 0x01;
static constexpr uint8_t BOOSTER_SOFT_START_CONTROL           = 0x0C;
static constexpr uint8_t GATE_SCAN_START_POSITION             = 0x0F;
static constexpr uint8_t DEEP_SLEEP_MODE                      = 0x10;
static constexpr uint8_t DATA_ENTRY_MODE_SETTING              = 0x11;
static constexpr uint8_t SW_RESET                             = 0x12;
static constexpr uint8_t TEMPERATURE_SENSOR_CONTROL           = 0x1A;
static constexpr uint8_t MASTER_ACTIVATION                    = 0x20;
static constexpr uint8_t DISPLAY_UPDATE_CONTROL_1             = 0x21;
static constexpr uint8_t DISPLAY_UPDATE_CONTROL_2             = 0x22;
static constexpr uint8_t WRITE_RAM                            = 0x24;
static constexpr uint8_t WRITE_VCOM_REGISTER                  = 0x2C;
static constexpr uint8_t WRITE_LUT_REGISTER                   = 0x32;
static constexpr uint8_t SET_DUMMY_LINE_PERIOD                = 0x3A;
static constexpr uint8_t SET_GATE_TIME                        = 0x3B;
static constexpr uint8_t BORDER_WAVEFORM_CONTROL              = 0x3C;
static constexpr uint8_t SET_RAM_X_ADDRESS_START_END_POSITION = 0x44;
static constexpr uint8_t SET_RAM_Y_ADDRESS_START_END_POSITION = 0x45;
static constexpr uint8_t SET_RAM_X_ADDRESS_COUNTER            = 0x4E;
static constexpr uint8_t SET_RAM_Y_ADDRESS_COUNTER            = 0x4F;
static constexpr uint8_t TERMINATE_FRAME_READ_WRITE           = 0xFF;

EPDPanel::EPDPanel(const EPDPanelConfig& config,
                   SPIController* spi_ctrl,
                   uint8_t* framebuffer)
    : spi(spi_ctrl), cfg(config), fb_buffer(framebuffer), update_mode(0), rotation(0)
{
    // Don't do automatic initialization here - let the descriptor init commands handle it
    // The uDisplay framework will call send_spi_cmds() after panel creation
    // which will handle reset, LUT setup, and initial display state
}

EPDPanel::~EPDPanel() {
    // Panel doesn't own framebuffer or SPI controller
    
    // Free owned LUT data
    if (cfg.lut_full_data) {
        free(cfg.lut_full_data);
    }
    
    if (cfg.lut_partial_data) {
        free(cfg.lut_partial_data);
    }
    
    for (uint8_t i = 0; i < 5; i++) {
        if (cfg.lut_array_data[i]) {
            free(cfg.lut_array_data[i]);
        }
    }
}

void EPDPanel::delay_sync(int32_t ms) {
    uint8_t busy_level = cfg.busy_invert ? LOW : HIGH;
    uint32_t time = millis();
    if (cfg.busy_pin >= 0) {
        while (digitalRead(cfg.busy_pin) == busy_level) {
            delay(1);
            if ((millis() - time) > cfg.busy_timeout) {
                break;
            }
        }
    } else {
        delay(ms);
    }
}

void EPDPanel::resetDisplay() {
    if (cfg.reset_pin < 0) return;
    
    pinMode(cfg.reset_pin, OUTPUT);
    digitalWrite(cfg.reset_pin, HIGH);
    delay(10);
    digitalWrite(cfg.reset_pin, LOW);
    delay(10);
    digitalWrite(cfg.reset_pin, HIGH);
    delay(10);
    delay_sync(100); // Use delay_sync instead of waitBusy
}

void EPDPanel::waitBusy() {
    // Deprecated - use delay_sync instead
    delay_sync(cfg.update_time);
}

void EPDPanel::setLut(const uint8_t* lut, uint16_t len) {
    if (!lut || len == 0) return;
    
    spi->beginTransaction();
    spi->csLow();
    spi->writeCommand(WRITE_LUT_REGISTER);
    for (uint16_t i = 0; i < len; i++) {
        spi->writeData8(lut[i]);
    }
    spi->csHigh();
    spi->endTransaction();
}

void EPDPanel::setMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    int x_start1 = (x_start >> 3) & 0xFF;
    int x_end1 = (x_end >> 3) & 0xFF;
    int y_start1 = y_start & 0xFF;
    int y_start2 = (y_start >> 8) & 0xFF;
    int y_end1 = y_end & 0xFF;
    int y_end2 = (y_end >> 8) & 0xFF;

    spi->beginTransaction();
    spi->csLow();
    spi->writeCommand(SET_RAM_X_ADDRESS_START_END_POSITION);
    spi->writeData8(x_start1);
    spi->writeData8(x_end1);
    
    spi->writeCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    if (cfg.ep_mode == 3) {
        // ep_mode 3: reversed Y order
        spi->writeData8(y_end1);
        spi->writeData8(y_end2);
        spi->writeData8(y_start1);
        spi->writeData8(y_start2);
    } else {
        spi->writeData8(y_start1);
        spi->writeData8(y_start2);
        spi->writeData8(y_end1);
        spi->writeData8(y_end2);
    }
    spi->csHigh();
    spi->endTransaction();
}

void EPDPanel::setMemoryPointer(int x, int y) {
    int x1, y1, y2;
    
    if (cfg.ep_mode == 3) {
        x1 = (x >> 3) & 0xFF;
        y--;
        y1 = y & 0xFF;
        y2 = (y >> 8) & 0xFF;
    } else {
        x1 = (x >> 3) & 0xFF;
        y1 = y & 0xFF;
        y2 = (y >> 8) & 0xFF;
    }

    spi->beginTransaction();
    spi->csLow();
    spi->writeCommand(SET_RAM_X_ADDRESS_COUNTER);
    spi->writeData8(x1);
    spi->writeCommand(SET_RAM_Y_ADDRESS_COUNTER);
    spi->writeData8(y1);
    spi->writeData8(y2);
    spi->csHigh();
    spi->endTransaction();
}

void EPDPanel::clearFrameMemory(uint8_t color) {
    setMemoryArea(0, 0, cfg.width - 1, cfg.height - 1);
    setMemoryPointer(0, 0);
    
    spi->beginTransaction();
    spi->csLow();
    spi->writeCommand(WRITE_RAM);
    
    uint32_t pixel_count = (cfg.width * cfg.height) / 8;
    for (uint32_t i = 0; i < pixel_count; i++) {
        spi->writeData8(color);
    }
    
    spi->csHigh();
    spi->endTransaction();
}

void EPDPanel::displayFrame() {
    spi->beginTransaction();
    spi->csLow();
    spi->writeCommand(DISPLAY_UPDATE_CONTROL_2);
    spi->writeData8(0xC4);
    spi->writeCommand(MASTER_ACTIVATION);
    spi->writeData8(TERMINATE_FRAME_READ_WRITE);
    spi->csHigh();
    spi->endTransaction();
    
    delay_sync(cfg.update_time); // Use delay_sync with proper timing
}

void EPDPanel::drawAbsolutePixel(int x, int y, uint16_t color) {
    // Bounds check using physical dimensions
    if (x < 0 || x >= cfg.width || y < 0 || y >= cfg.height) {
        return;
    }
    
    // CRITICAL: Must match Renderer::drawPixel() layout!
    // 
    // Two rendering systems write to the SAME framebuffer:
    // 1. Renderer::drawPixel() - used by DrawStringAt() for text (Splash Screen)
    // 2. EPDPanel::drawPixel() - used by Adafruit_GFX for graphics (circles, lines)
    //
    // Both MUST use the same framebuffer layout: Y-column-wise
    // Layout: fb[x + (y/8)*width] with bit position (y&7)
    // This means 8 vertical pixels are stored in one byte.
    //
    // setFrameMemory() will convert Y-column to X-row format when sending to hardware.
    
    if (color) {
        fb_buffer[x + (y / 8) * cfg.width] |= (1 << (y & 7));
    } else {
        fb_buffer[x + (y / 8) * cfg.width] &= ~(1 << (y & 7));
    }
}

// ===== UniversalPanel Interface Implementation =====

bool EPDPanel::drawPixel(int16_t x, int16_t y, uint16_t color) {   
    if (!fb_buffer) return false;
    
    // Get rotated dimensions for bounds check
    int16_t w = cfg.width, h = cfg.height;
    if (rotation == 1 || rotation == 3) {
        std::swap(w, h);
    }
    
    if ((x < 0) || (x >= w) || (y < 0) || (y >= h)) {
        return false;  // Out of bounds
    }
    
    // Apply rotation transformation using PHYSICAL dimensions (gxs/gys)
    switch (rotation) {
        case 1:
            std::swap(x, y);
            x = cfg.width - x - 1;   // gxs
            break;
        case 2:
            x = cfg.width - x - 1;   // gxs
            y = cfg.height - y - 1;  // gys
            break;
        case 3:
            std::swap(x, y);
            y = cfg.height - y - 1;  // gys
            break;
    }
    
    // Convert color to monochrome and draw
    drawAbsolutePixel(x, y, (color != 0) ? 1 : 0);
    return true;
}

bool EPDPanel::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {   
    // Use drawPixel to handle rotation properly
    for (int16_t yp = y; yp < y + h; yp++) {
        for (int16_t xp = x; xp < x + w; xp++) {
            drawPixel(xp, yp, color);
        }
    }
    return true;
}

bool EPDPanel::pushColors(uint16_t *data, uint32_t len, bool first) {
    // Convert RGB565 to monochrome and write to framebuffer
    // Pixel is white if at least one of the 3 RGB components is above 50%
    static constexpr uint16_t RGB16_TO_MONO = 0x8410;
    
    if (!fb_buffer) return false;
    
    // Write pixels to framebuffer based on window coordinates
    // IMPORTANT: window coordinates are in LOGICAL (rotated) space,
    // so we must use drawPixel (not drawAbsolutePixel) to apply rotation!
    for (int16_t y = window_y1; y < window_y2 && len > 0; y++) {
        for (int16_t x = window_x1; x < window_x2 && len > 0; x++, len--) {
            uint16_t color = *data++;
            // Convert to mono: white if any component > 50%
            bool pixel = (color & RGB16_TO_MONO) ? true : false;
            if (cfg.invert_colors) pixel = !pixel;
            drawPixel(x, y, pixel ? 1 : 0);
        }
    }
    
    return true; // Handled by EPD panel
}

bool EPDPanel::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    // Save window coordinates for pushColors
    window_x1 = x0;
    window_y1 = y0;
    window_x2 = x1;
    window_y2 = y1;
    return true;
}

bool EPDPanel::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    while (w--) {
        drawPixel(x, y, color);
        x++;
    }
    return true;
}

bool EPDPanel::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    while (h--) {
        drawPixel(x, y, color);
        y++;
    }
    return true;
}

bool EPDPanel::displayOnff(int8_t on) {
    // EPD doesn't have on/off in traditional sense
    return true;
}

bool EPDPanel::invertDisplay(bool invert) {
    // Toggle color inversion logic
    cfg.invert_colors = invert;
    
    // For EPD, we need to redraw the entire display when inversion changes
    if (fb_buffer) {
        // Invert the entire framebuffer
        uint32_t byte_count = (cfg.width * cfg.height) / 8;
        for (uint32_t i = 0; i < byte_count; i++) {
            fb_buffer[i] = ~fb_buffer[i];
        }
        updateFrame();
    }
    return true;
}

bool EPDPanel::setRotation(uint8_t rot) {
    rotation = rot & 3;  // Store rotation (0-3)
    return true;
}

bool EPDPanel::updateFrame() {
    if (!fb_buffer) return false;
    
    // Handle different EPD modes
    if (cfg.ep_mode == 1 || cfg.ep_mode == 3) {
        // Mode 1 (2-LUT) or Mode 3 (command-based): Use descriptor command sequences
        switch (update_mode) {
            case 1: // DISPLAY_INIT_PARTIAL
                if (cfg.epc_part_cnt && cfg.send_cmds_callback) {
                    cfg.send_cmds_callback(cfg.epcoffs_part, cfg.epc_part_cnt);
                }
                break;
            case 2: // DISPLAY_INIT_FULL
                if (cfg.epc_full_cnt && cfg.send_cmds_callback) {
                    cfg.send_cmds_callback(cfg.epcoffs_full, cfg.epc_full_cnt);
                }
                break;
            default: // DISPLAY_INIT_MODE (0)
                // Default: write framebuffer and display
                setFrameMemory(fb_buffer, 0, 0, cfg.width, cfg.height);
                displayFrame();
        }
    } else if (cfg.ep_mode == 2) {
        // Mode 2 (5-LUT / 4.2" displays): Use internal displayFrame_42
        displayFrame_42();
    }
    
    return true;
}

// ===== ep_mode 2 Support (5-LUT mode) =====

void EPDPanel::setLuts() {
    if (!cfg.lut_array || !cfg.lut_cnt) return;
    
    for (uint8_t index = 0; index < 5; index++) {
        if (cfg.lut_cmd[index] == 0 || !cfg.lut_array[index]) continue;
        
        spi->beginTransaction();
        spi->csLow();
        spi->writeCommand(cfg.lut_cmd[index]);
        for (uint8_t count = 0; count < cfg.lut_cnt[index]; count++) {
            spi->writeData8(cfg.lut_array[index][count]);
        }
        spi->csHigh();
        spi->endTransaction();
    }
}

void EPDPanel::clearFrame_42() {
    spi->beginTransaction();
    spi->csLow();
    
    spi->writeCommand(cfg.saw_1);
    for (uint16_t j = 0; j < cfg.height; j++) {
        for (uint16_t i = 0; i < cfg.width; i++) {
            spi->writeData8(0xFF);
        }
    }

    spi->writeCommand(cfg.saw_2);
    for (uint16_t j = 0; j < cfg.height; j++) {
        for (uint16_t i = 0; i < cfg.width; i++) {
            spi->writeData8(0xFF);
        }
    }

    spi->writeCommand(cfg.saw_3);
    spi->csHigh();
    spi->endTransaction();
    
    delay_sync(100);
}

void EPDPanel::displayFrame_42() {
    spi->beginTransaction();
    spi->csLow();
    
    spi->writeCommand(cfg.saw_1);
    for(int i = 0; i < cfg.width / 8 * cfg.height; i++) {
        spi->writeData8(0xFF);
    }
    
    spi->csHigh();
    spi->endTransaction();
    delay(2);

    spi->beginTransaction();
    spi->csLow();
    spi->writeCommand(cfg.saw_2);
    for(int i = 0; i < cfg.width / 8 * cfg.height; i++) {
        spi->writeData8(fb_buffer[i] ^ 0xff);
    }
    spi->csHigh();
    spi->endTransaction();
    delay(2);

    setLuts();

    spi->beginTransaction();
    spi->csLow();
    spi->writeCommand(cfg.saw_3);
    spi->csHigh();
    spi->endTransaction();
    
    delay_sync(100);
}

// ===== Frame Memory Management =====

// Helper: Convert Y-column framebuffer to X-row format and send via SPI
// Y-column: fb[x + (y/8)*width] with bit (y&7) - 8 vertical pixels per byte
// X-row: 8 horizontal pixels per byte, MSB = leftmost pixel
void EPDPanel::sendYColumnAsXRow(const uint8_t* y_column_buffer, uint16_t buffer_width,
                                  uint16_t rows, uint16_t cols_bytes) {
    for (uint16_t row = 0; row < rows; row++) {
        for (uint16_t x_byte = 0; x_byte < cols_bytes; x_byte++) {
            uint8_t byte_out = 0;
            for (uint8_t bit = 0; bit < 8; bit++) {
                uint16_t x = x_byte * 8 + bit;
                uint8_t pixel = (y_column_buffer[x + (row / 8) * buffer_width] >> (row & 7)) & 1;
                if (pixel) byte_out |= (0x80 >> bit);
            }
            spi->writeData8(byte_out ^ 0xff);
        }
    }
}

void EPDPanel::setFrameMemory(const uint8_t* image_buffer) {
    setMemoryArea(0, 0, cfg.width - 1, cfg.height - 1);
    setMemoryPointer(0, 0);
    
    spi->beginTransaction();
    spi->csLow();
    spi->writeCommand(WRITE_RAM);
    sendYColumnAsXRow(image_buffer, cfg.width, cfg.height, cfg.width / 8);
    spi->csHigh();
    spi->endTransaction();
}

void EPDPanel::setFrameMemory(const uint8_t* image_buffer, uint16_t x, uint16_t y, uint16_t image_width, uint16_t image_height) {
    if (!image_buffer) return;
    
    // Align to 8-pixel boundary
    x &= 0xFFF8;
    image_width &= 0xFFF8;
    
    uint16_t x_end = (x + image_width >= cfg.width) ? cfg.width - 1 : x + image_width - 1;
    uint16_t y_end = (y + image_height >= cfg.height) ? cfg.height - 1 : y + image_height - 1;

    // Full screen optimization
    if (!x && !y && image_width == cfg.width && image_height == cfg.height) {
        setFrameMemory(image_buffer);
        return;
    }

    setMemoryArea(x, y, x_end, y_end);
    setMemoryPointer(x, y);
    
    spi->beginTransaction();
    spi->csLow();
    spi->writeCommand(WRITE_RAM);
    sendYColumnAsXRow(image_buffer, image_width, y_end - y + 1, (x_end - x + 1) / 8);
    spi->csHigh();
    spi->endTransaction();
}

void EPDPanel::sendEPData() {
    // EP_SEND_DATA (0x66) - used by some display.ini files (e.g. v2)
    // Must also convert Y-column to X-row format like setFrameMemory()
    sendYColumnAsXRow(fb_buffer, cfg.width, cfg.height, cfg.width / 8);
}

// ===== Update Mode Control =====

void EPDPanel::setUpdateMode(uint8_t mode) {
    update_mode = mode;
}
