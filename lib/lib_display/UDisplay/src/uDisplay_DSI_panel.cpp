// ======================================================
// uDisplay_DSI_panel.cpp - Hardcoded JD9165 MIPI-DSI Implementation
// Based on esp_lcd_jd9165.c from Espressif
// ======================================================

#include "uDisplay_DSI_panel.h"
#if SOC_MIPI_DSI_SUPPORTED
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_mipi_dsi.h"
#include "esp_ldo_regulator.h"
#include "driver/gpio.h"
#include <rom/cache.h>

extern void AddLog(uint32_t loglevel, const char* formatP, ...);

DSIPanel::DSIPanel(const DSIPanelConfig& config)
    : cfg(config), rotation(0)
{

    framebuffer_size = cfg.width * cfg.height * 2;

    esp_err_t ret;

    // Step 1: Initialize LDO for display power (from config)
    if (cfg.ldo_channel >= 0 && cfg.ldo_voltage_mv > 0) {
        esp_ldo_channel_config_t ldo_config = {
            .chan_id = cfg.ldo_channel,
            .voltage_mv = cfg.ldo_voltage_mv,
        };
        ret = esp_ldo_acquire_channel(&ldo_config, &ldo_handle);
        if (ret != ESP_OK) {
            AddLog(3, "DSI: Failed to acquire LDO: %d", ret);
            return;
        }
        AddLog(3, "DSI: LDO enabled (ch %d @ %dmV)", cfg.ldo_channel, cfg.ldo_voltage_mv);
        delay(10);
    } else {
        AddLog(3, "DSI: No LDO configuration");
    }

    // Step 2: Create DSI bus (from config)
    esp_lcd_dsi_bus_handle_t dsi_bus = nullptr;
    esp_lcd_dsi_bus_config_t bus_config = {
        .bus_id = 0,
        .num_data_lanes = cfg.dsi_lanes,
        .lane_bit_rate_mbps = cfg.lane_speed_mbps
    };
    ret = esp_lcd_new_dsi_bus(&bus_config, &dsi_bus);
    if (ret != ESP_OK) {
        AddLog(3, "DSI: Failed to create DSI bus: %d", ret);
        return;
    }
    AddLog(3, "DSI: DSI bus created");

    // Step 3: Create DBI IO for commands
    esp_lcd_dbi_io_config_t io_config = {
        .virtual_channel = 0,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    ret = esp_lcd_new_panel_io_dbi(dsi_bus, &io_config, &io_handle);
    if (ret != ESP_OK) {
        AddLog(3, "DSI: Failed to create DBI IO: %d", ret);
        return;
    }
    AddLog(3, "DSI: DBI IO created");

    // Step 4: Configure DPI panel (from config)
    esp_lcd_dpi_panel_config_t dpi_config = {};
    dpi_config.dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT;
    dpi_config.dpi_clock_freq_mhz = cfg.pixel_clock_hz / 1000000;
    dpi_config.virtual_channel = 0;
    dpi_config.pixel_format = LCD_COLOR_PIXEL_FORMAT_RGB565;
    dpi_config.num_fbs = 1;
    dpi_config.video_timing.h_size = cfg.width;
    dpi_config.video_timing.v_size = cfg.height;
    dpi_config.video_timing.hsync_back_porch = cfg.timing.h_back_porch;
    dpi_config.video_timing.hsync_pulse_width = cfg.timing.h_sync_pulse;
    dpi_config.video_timing.hsync_front_porch = cfg.timing.h_front_porch;
    dpi_config.video_timing.vsync_back_porch = cfg.timing.v_back_porch;
    dpi_config.video_timing.vsync_pulse_width = cfg.timing.v_sync_pulse;
    dpi_config.video_timing.vsync_front_porch = cfg.timing.v_front_porch;
    dpi_config.flags.use_dma2d = 1;
    
    AddLog(3, "DSI: DPI config: clk=%dMHz res=%dx%d", dpi_config.dpi_clock_freq_mhz, cfg.width, cfg.height);
    AddLog(3, "DSI: H timing: BP=%d PW=%d FP=%d", cfg.timing.h_back_porch, cfg.timing.h_sync_pulse, cfg.timing.h_front_porch);
    AddLog(3, "DSI: V timing: BP=%d PW=%d FP=%d", cfg.timing.v_back_porch, cfg.timing.v_sync_pulse, cfg.timing.v_front_porch);
    AddLog(3, "DSI: Expected: clk=54MHz res=1024x600 H:160/40/160 V:23/10/12");

    // Step 5: Create DPI panel
    ret = esp_lcd_new_panel_dpi(dsi_bus, &dpi_config, &panel_handle);
    if (ret != ESP_OK) {
        AddLog(3, "DSI: Failed to create DPI panel: %d", ret);
        return;
    }
    AddLog(3, "DSI: DPI panel created");

    // Step 6: Reset via GPIO (from config)
    if (cfg.reset_pin >= 0) {
        gpio_config_t gpio_conf = {
            .pin_bit_mask = 1ULL << cfg.reset_pin,
            .mode = GPIO_MODE_OUTPUT,
        };
        gpio_config(&gpio_conf);
        gpio_set_level((gpio_num_t)cfg.reset_pin, 1);
        delay(5);
        gpio_set_level((gpio_num_t)cfg.reset_pin, 0);
        delay(10);
        gpio_set_level((gpio_num_t)cfg.reset_pin, 1);
        delay(120);
        AddLog(3, "DSI: GPIO reset completed (pin %d)", cfg.reset_pin);
    } else {
        AddLog(3, "DSI: No reset pin configured");
    }

    // Step 7: Initialize DPI panel
    ret = esp_lcd_panel_init(panel_handle);
    if (ret != ESP_OK) {
        AddLog(3, "DSI: Panel init failed: %d", ret);
        return;
    }
    AddLog(3, "DSI: DPI panel initialized");

    // Step 8: Get framebuffer
    void* fb_ptr = nullptr;
    ret = esp_lcd_dpi_panel_get_frame_buffer(panel_handle, 1, &fb_ptr);
    if (ret == ESP_OK && fb_ptr != nullptr) {
        framebuffer = (uint16_t*)fb_ptr;
        AddLog(3, "DSI: Framebuffer at %p", framebuffer);
    } else {
        framebuffer = nullptr;
        AddLog(3, "DSI: No framebuffer, using draw_bitmap");
    }

    // Step 9: Send init commands from INI file
    if (cfg.init_commands && cfg.init_commands_count > 0) {
        AddLog(3, "DSI: Sending init commands from INI file");
        uint16_t index = 0;
        uint16_t cmd_num = 0;
        
        while (index < cfg.init_commands_count) {
            uint8_t cmd = cfg.init_commands[index++];
            uint8_t data_size = cfg.init_commands[index++];
            
            if (data_size > 0) {
                ret = esp_lcd_panel_io_tx_param(io_handle, cmd, &cfg.init_commands[index], data_size);
                index += data_size;
            } else {
                ret = esp_lcd_panel_io_tx_param(io_handle, cmd, NULL, 0);
            }
            
            if (ret != ESP_OK) {
                AddLog(3, "DSI: Cmd 0x%02x failed: %d", cmd, ret);
            }
            
            uint8_t delay_ms = cfg.init_commands[index++];
            if (delay_ms > 0) {
                vTaskDelay(pdMS_TO_TICKS(delay_ms));
            }
            cmd_num++;
        }
        AddLog(3, "DSI: Sent %d commands from INI", cmd_num);
    } else {
        AddLog(3, "DSI: No init commands in config");
    }
}

DSIPanel::~DSIPanel() {
    if (panel_handle) {
        esp_lcd_panel_del(panel_handle);
    }
    if (io_handle) {
        esp_lcd_panel_io_del(io_handle);
    }
    if (ldo_handle) {
        esp_ldo_release_channel(ldo_handle);
    }
}

bool DSIPanel::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (!framebuffer || x < 0 || y < 0 || x >= cfg.width || y >= cfg.height) return true;

    int16_t w = cfg.width, h = cfg.height;
    switch (rotation) {
        case 1: std::swap(w, h); std::swap(x, y); x = w - x - 1; break;
        case 2: x = w - x - 1; y = h - y - 1; break;
        case 3: std::swap(w, h); std::swap(x, y); y = h - y - 1; break;
    }

    uint16_t* p = &framebuffer[y * cfg.width + x];
    *p = color;
    framebuffer_dirty = true;

    return true;
}

bool DSIPanel::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    for (int16_t yp = y; yp < y + h; yp++) {
        uint16_t* line_start = &framebuffer[yp * cfg.width + x];
        for (int16_t i = 0; i < w; i++) {
            line_start[i] = color;
        }
        // CACHE_WRITEBACK_ADDR((uint32_t)line_start, w * 2);
    }
    framebuffer_dirty = true;
    return true;
}

bool DSIPanel::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    return fillRect(x, y, w, 1, color);
}

bool DSIPanel::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    return fillRect(x, y, 1, h, color);
}

bool DSIPanel::pushColors(uint16_t *data, uint32_t len, bool not_swapped) {
    esp_err_t ret = esp_lcd_panel_draw_bitmap(panel_handle, window_x0, window_y0, window_x1, window_y1, data);
    return (ret == ESP_OK);
}

bool DSIPanel::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    window_x0 = x0;
    window_y0 = y0;
    window_x1 = x1;
    window_y1 = y1;
    return true;
}

bool DSIPanel::displayOnff(int8_t on) {
    if (!io_handle) return false;
    
    // Use commands from descriptor
    uint8_t cmd = on ? cfg.cmd_display_on : cfg.cmd_display_off;
    
    esp_err_t ret = esp_lcd_panel_io_tx_param(io_handle, cmd, NULL, 0);
    return (ret == ESP_OK);
}

bool DSIPanel::invertDisplay(bool invert) {
    if (!io_handle) return false;
    
    // Standard MIPI DCS commands for invert
    uint8_t cmd = invert ? 0x21 : 0x20;  // 0x21 = INVON, 0x20 = INVOFF
    esp_err_t ret = esp_lcd_panel_io_tx_param(io_handle, cmd, NULL, 0);
    return (ret == ESP_OK);
}

bool DSIPanel::setRotation(uint8_t rot) {
    if (!io_handle) return false;
    
    rotation = rot & 3;
    
    // Standard MIPI DCS MADCTL (0x36) values for rotation
    // These are common values but may need adjustment for specific displays
    uint8_t madctl_val = 0;
    
    switch (rotation) {
    case 0:  // Portrait
        madctl_val = 0x00;  // Normal
        break;
    case 1:  // Landscape (90° clockwise)
        madctl_val = 0x60;  // MX + MV
        break;
    case 2:  // Portrait inverted (180°)
        madctl_val = 0xC0;  // MX + MY
        break;
    case 3:  // Landscape inverted (270° clockwise)
        madctl_val = 0xA0;  // MY + MV
        break;
    }
    
    // Send MADCTL command (0x36) with rotation value
    esp_err_t ret = esp_lcd_panel_io_tx_param(io_handle, 0x36, &madctl_val, 1);
    return false; // pass job to Renderer
}

bool DSIPanel::updateFrame() {
    if (!framebuffer_dirty) {
        return true;
    }
    CACHE_WRITEBACK_ADDR((uint32_t)framebuffer, framebuffer_size); //KISS and fast enough!
    framebuffer_dirty = false;  // ← RESET

    return true;
}

#endif // SOC_MIPI_DSI_SUPPORTED
