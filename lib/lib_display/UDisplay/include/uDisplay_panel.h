// ======================================================
// uDisplay_panel.h - Base Panel Interface
// ======================================================

#pragma once

#if CONFIG_IDF_TARGET_ESP32P4
#include "esp_cache.h"
    #define CACHE_WRITEBACK_ADDR(addr, size) esp_cache_msync((void*)addr, size, ESP_CACHE_MSYNC_FLAG_DIR_C2M)
#else
    #define CACHE_WRITEBACK_ADDR(addr, size) Cache_WriteBack_Addr(addr, size)
#endif

#include <Arduino.h>

class UniversalPanel {
public:
    virtual ~UniversalPanel() {}
    
    // Core graphics API - return true if handled, false for uDisplay fallback
    virtual bool drawPixel(int16_t x, int16_t y, uint16_t color) = 0;
    virtual bool fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
    virtual bool pushColors(uint16_t *data, uint32_t len, bool first = false) = 0;
    virtual bool setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) = 0;
    virtual bool drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) = 0;
    virtual bool drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) = 0;

    // Control API - return true if handled, false for uDisplay fallback  
    virtual bool displayOnff(int8_t on) = 0;
    virtual bool invertDisplay(bool invert) = 0;
    virtual bool setRotation(uint8_t rotation) = 0;
    
    // Frame update method for displays that need explicit updates
    virtual bool updateFrame() = 0;
    // Framebuffer - own or external
    uint16_t* framebuffer = nullptr;
};