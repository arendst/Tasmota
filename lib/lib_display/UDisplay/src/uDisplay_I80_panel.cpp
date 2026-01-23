#include "uDisplay_I80_panel.h"

#if (SOC_LCD_I80_SUPPORTED && SOC_LCDCAM_I80_NUM_BUSES && !SOC_PARLIO_GROUPS )

#ifdef UDSP_DEBUG
extern void AddLog(uint32_t loglevel, const char *formatP, ...);
#define LOG_LEVEL_DEBUG 3
#endif

// Pin control helpers
static inline volatile uint32_t* get_gpio_hi_reg(int_fast8_t pin) { return (pin & 32) ? &GPIO.out1_w1ts.val : &GPIO.out_w1ts; }
static inline volatile uint32_t* get_gpio_lo_reg(int_fast8_t pin) { return (pin & 32) ? &GPIO.out1_w1tc.val : &GPIO.out_w1tc; }
static inline void gpio_hi(int_fast8_t pin) { if (pin >= 0) *get_gpio_hi_reg(pin) = 1 << (pin & 31); }
static inline void gpio_lo(int_fast8_t pin) { if (pin >= 0) *get_gpio_lo_reg(pin) = 1 << (pin & 31); }

I80Panel::I80Panel(const I80PanelConfig& config)
    : cfg(config),
      _width(config.width), _height(config.height), _rotation(0),
      _i80_bus(nullptr), _dev(nullptr), _dmadesc(nullptr),
      _DMA_Enabled(false), _dma_chan(nullptr), _dmadesc_size(0),
      _addr_x0(0), _addr_y0(0), _addr_x1(0), _addr_y1(0) {
    
    framebuffer = nullptr;

    // Initialize pins manually FIRST (matching old code order)
    if (cfg.cs_pin >= 0) {
        pinMode(cfg.cs_pin, OUTPUT);
        digitalWrite(cfg.cs_pin, HIGH);
    }
    pinMode(cfg.dc_pin, OUTPUT);
    digitalWrite(cfg.dc_pin, HIGH);
    pinMode(cfg.wr_pin, OUTPUT);
    digitalWrite(cfg.wr_pin, HIGH);
    if (cfg.rd_pin >= 0) {
        pinMode(cfg.rd_pin, OUTPUT);
        digitalWrite(cfg.rd_pin, HIGH);
    }

    for (int i = 0; i < 8; i++) {
        pinMode(cfg.data_pins_low[i], OUTPUT);
    }
    if (cfg.bus_width == 16) {
        for (int i = 0; i < 8; i++) {
            pinMode(cfg.data_pins_high[i], OUTPUT);
        }
    }

    // Now create I80 bus config
    esp_lcd_i80_bus_config_t bus_config = {
        .dc_gpio_num = cfg.dc_pin,
        .wr_gpio_num = cfg.wr_pin,
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .bus_width = cfg.bus_width,
        .max_transfer_bytes = (size_t)cfg.width * cfg.height * 2
    };
    
    // Set data pins
    if (cfg.bus_width == 8) {
        for (int i = 0; i < 8; i++) {
            bus_config.data_gpio_nums[i] = cfg.data_pins_low[i];
        }
    } else {
        for (int i = 0; i < 8; i++) {
            bus_config.data_gpio_nums[i] = cfg.data_pins_low[i];
            bus_config.data_gpio_nums[i + 8] = cfg.data_pins_high[i];
        }
    }
    
    // Create I80 bus (this will take over GPIO matrix for DC, WR, and data pins)
    esp_lcd_new_i80_bus(&bus_config, &_i80_bus);

    // Calculate clock using original algorithm
    uint32_t div_a, div_b, div_n, clkcnt;
    calcClockDiv(&div_a, &div_b, &div_n, &clkcnt, 240*1000*1000, cfg.clock_speed_hz);
    
    lcd_cam_lcd_clock_reg_t lcd_clock;
    lcd_clock.lcd_clkcnt_n = std::max((uint32_t)1u, clkcnt - 1);
    lcd_clock.lcd_clk_equ_sysclk = (clkcnt == 1);
    lcd_clock.lcd_ck_idle_edge = true;
    lcd_clock.lcd_ck_out_edge = false;
    lcd_clock.lcd_clkm_div_num = div_n;
    lcd_clock.lcd_clkm_div_b = div_b;
    lcd_clock.lcd_clkm_div_a = div_a;
    lcd_clock.lcd_clk_sel = 2; // 240MHz
    lcd_clock.clk_en = true;
    _clock_reg_value = lcd_clock.val;

    _alloc_dmadesc(1);
    _dev = &LCD_CAM;

    // EXECUTE INITIALIZATION COMMANDS (from original uDisplay code)
    if (cfg.init_commands && cfg.init_commands_count > 0) {
        uint16_t index = 0;
        pb_beginTransaction();
        
        while (index < cfg.init_commands_count) {
            cs_control(false);
            
            uint8_t cmd = cfg.init_commands[index++];
            pb_writeCommand(cmd, 8);
            
            if (index < cfg.init_commands_count) {
                uint8_t args = cfg.init_commands[index++];
                uint8_t arg_count = args & 0x1f;
                
#ifdef UDSP_DEBUG
                AddLog(LOG_LEVEL_DEBUG, "UDisplay: cmd, args %02x, %d", cmd, arg_count);
#endif
                
                for (uint32_t cnt = 0; cnt < arg_count && index < cfg.init_commands_count; cnt++) {
                    uint8_t arg_data = cfg.init_commands[index++];
#ifdef UDSP_DEBUG
                    AddLog(LOG_LEVEL_DEBUG, "%02x ", arg_data);
#endif
                    pb_writeData(arg_data, 8);
                }
                
                cs_control(true);
                
                // Handle delay after command
                if (args & 0x80) {
                    uint32_t delay_ms = 0;
                    switch (args & 0xE0) {
                        case 0x80:  delay_ms = 150; break;
                        case 0xA0:  delay_ms =  10; break;
                        case 0xE0:  delay_ms = 500; break;
                    }
                    if (delay_ms > 0) {
#ifdef UDSP_DEBUG
                        AddLog(LOG_LEVEL_DEBUG, "UDisplay: delay %d ms", delay_ms);
#endif
                        delay(delay_ms);
                    }
                }
            } else {
                cs_control(true);
            }
        }
        
        pb_endTransaction();
    }
}

I80Panel::~I80Panel() {
    deInitDMA();
    if (_dmadesc) {
        heap_caps_free(_dmadesc);
        _dmadesc = nullptr;
    }
    if (_i80_bus) {
        esp_lcd_del_i80_bus(_i80_bus);
    }
}

// DMA Implementation
bool I80Panel::initDMA() {
    if (_DMA_Enabled) return true;

    gdma_channel_alloc_config_t dma_chan_config = {
        .direction = GDMA_CHANNEL_DIRECTION_TX
    };
    
    if (gdma_new_channel(&dma_chan_config, &_dma_chan) == ESP_OK) {
        gdma_connect(_dma_chan, GDMA_MAKE_TRIGGER(GDMA_TRIG_PERIPH_LCD, 0));
        _alloc_dmadesc(16);
        _DMA_Enabled = true;
        return true;
    }
    
    return false;
}

void I80Panel::deInitDMA() {
    if (_dma_chan) {
        gdma_disconnect(_dma_chan);
        gdma_del_channel(_dma_chan);
        _dma_chan = nullptr;
    }
    _DMA_Enabled = false;
}

bool I80Panel::dmaBusy() {
    if (!_DMA_Enabled) return false;
    return (_dev->lcd_user.val & LCD_CAM_LCD_START);
}

void I80Panel::dmaWait() {
    if (!_DMA_Enabled) return;
    while (dmaBusy()) {
        delay(1);
    }
}

// Graphics implementation
bool I80Panel::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return true;
    
    pb_beginTransaction();
    cs_control(false);
    setAddrWindow_int(x, y, 1, 1);
    writeColor(color);
    cs_control(true);
    pb_endTransaction();
    return true;
}

bool I80Panel::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if((x >= _width) || (y >= _height)) return true;
    if((x + w - 1) >= _width)  w = _width - x;
    if((y + h - 1) >= _height) h = _height - y;

    pb_beginTransaction();
    cs_control(false);
    setAddrWindow_int(x, y, w, h);
    
    for (int16_t yp = h; yp > 0; yp--) {
        for (int16_t xp = w; xp > 0; xp--) {
            writeColor(color);
        }
    }
    
    cs_control(true);
    pb_endTransaction();
    return true;
}

bool I80Panel::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if((x >= _width) || (y >= _height)) return true;
    if((x + w - 1) >= _width)  w = _width - x;

    pb_beginTransaction();
    cs_control(false);
    setAddrWindow_int(x, y, w, 1);
    
    while (w--) {
        writeColor(color);
    }
    
    cs_control(true);
    pb_endTransaction();
    return true;
}

bool I80Panel::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if ((x >= _width) || (y >= _height)) return true;
    if ((y + h - 1) >= _height) h = _height - y;

    pb_beginTransaction();
    cs_control(false);
    setAddrWindow_int(x, y, 1, h);
    
    while (h--) {
        writeColor(color);
    }
    
    cs_control(true);
    pb_endTransaction();
    return true;
}

bool I80Panel::pushColors(uint16_t *data, uint32_t len, bool swapped) {
    // Match old code: just push pixels, no transaction management
    // Transaction is managed by setAddrWindow()
    pb_pushPixels(data, len, !swapped, false);  // swap_bytes=true to match old driver
    return true;
}

bool I80Panel::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    // Match old code behavior exactly
    if (!x0 && !y0 && !x1 && !y1) {
        // End transaction signal
        cs_control(true);
        pb_endTransaction();
    } else {
        // Begin transaction and send address window commands
        pb_beginTransaction();
        cs_control(false);
        setAddrWindow_int(x0, y0, x1 - x0, y1 - y0);
        // Leave transaction open for pushColors
    }
    return true;
}

bool I80Panel::displayOnff(int8_t on) {
    return false; // Let uDisplay handle display commands
}

bool I80Panel::invertDisplay(bool invert) {
    return false; // Let uDisplay handle inversion commands
}

bool I80Panel::setRotation(uint8_t rotation) {
    _rotation = rotation & 3;
    
    // Calculate new dimensions based on rotation FIRST
    uint16_t new_width, new_height;
    switch (_rotation) {
        case 0:
        case 2:
            new_width = cfg.width;
            new_height = cfg.height;
            break;
        case 1:
        case 3:
            new_width = cfg.height;
            new_height = cfg.width;
            break;
    }
    
    // Send MADCTL rotation command to display
    pb_beginTransaction();
    cs_control(false);
    
    // Send rotation command (matches old code behavior)
    pb_writeCommand(cfg.cmd_madctl, 8);
    if (!cfg.allcmd_mode) {
        pb_writeData(cfg.rot_cmd[_rotation], 8);
    } else {
        pb_writeCommand(cfg.rot_cmd[_rotation], 8);
    }
    
    // For sa_mode == 8, also send startline command
    if (cfg.sa_mode == 8 && !cfg.allcmd_mode) {
        pb_writeCommand(cfg.cmd_startline, 8);
        pb_writeData((_rotation < 2) ? new_height : 0, 8);
    }
    
    cs_control(true);
    pb_endTransaction();
    
    // Update dimensions
    _width = new_width;
    _height = new_height;
    
    return true;
}

bool I80Panel::updateFrame() {
    return true; // I80 updates are immediate
}

uint32_t I80Panel::getSimpleResistiveTouch(uint32_t threshold) {
    uint32_t aval = 0;
    uint16_t xp, yp;
    if (pb_busy()) return 0;

    // Disable GPIO matrix routing to use pins as GPIOs
    _pb_init_pin(true);
    
    // Temporarily reconfigure I80 pins as GPIOs for analog touch
    gpio_matrix_out(cfg.dc_pin, 0x100, 0, 0);

    pinMode(cfg.data_pins_low[0], INPUT_PULLUP);
    pinMode(cfg.dc_pin, INPUT_PULLUP);

    pinMode(cfg.cs_pin, OUTPUT);
    pinMode(cfg.data_pins_low[1], OUTPUT);
    digitalWrite(cfg.cs_pin, HIGH);
    digitalWrite(cfg.data_pins_low[1], LOW);

    xp = 4096 - analogRead(cfg.data_pins_low[0]);

    pinMode(cfg.cs_pin, INPUT_PULLUP);
    pinMode(cfg.data_pins_low[1], INPUT_PULLUP);

    pinMode(cfg.data_pins_low[0], OUTPUT);
    pinMode(cfg.dc_pin, OUTPUT);
    digitalWrite(cfg.data_pins_low[0], HIGH);
    digitalWrite(cfg.dc_pin, LOW);

    yp = 4096 - analogRead(cfg.data_pins_low[1]);

    aval = (xp << 16) | yp;

    // Restore pins to I80 function
    pinMode(cfg.dc_pin, OUTPUT);
    pinMode(cfg.cs_pin, OUTPUT);
    pinMode(cfg.data_pins_low[0], OUTPUT);
    pinMode(cfg.data_pins_low[1], OUTPUT);
    digitalWrite(cfg.dc_pin, HIGH);
    digitalWrite(cfg.cs_pin, HIGH);

    // Re-enable GPIO matrix routing for I80
    _pb_init_pin(false);
    gpio_matrix_out(cfg.dc_pin, LCD_DC_IDX, 0, 0);

    return aval;
}

// Color mode helper
void I80Panel::writeColor(uint16_t color) {
    if (cfg.color_mode == 18) {
        uint8_t r = (color & 0xF800) >> 11;
        uint8_t g = (color & 0x07E0) >> 5;
        uint8_t b = color & 0x001F;
        r = (r * 255) / 31;
        g = (g * 255) / 63;
        b = (b * 255) / 31;
        
        pb_writeData(r, 8);
        pb_writeData(g, 8);
        pb_writeData(b, 8);
    } else {
        pb_writeData(color, 16);
    }
}

void I80Panel::setAddrWindow_int(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // Apply rotation-specific offsets (matches old code logic)
    x += cfg.x_addr_offset[_rotation];
    y += cfg.y_addr_offset[_rotation];
    
    uint16_t x2 = x + w - 1;
    uint16_t y2 = y + h - 1;
       
    if (cfg.sa_mode != 8) {
        // Normal mode: send 32-bit packed coordinates
        uint32_t xa = ((uint32_t)x << 16) | x2;
        uint32_t ya = ((uint32_t)y << 16) | y2;
        
        pb_writeCommand(cfg.cmd_set_addr_x, 8);
        pb_writeData(xa, 32);
        pb_writeCommand(cfg.cmd_set_addr_y, 8);
        pb_writeData(ya, 32);
        
        if (cfg.cmd_write_ram != 0xff) {
            pb_writeCommand(cfg.cmd_write_ram, 8);
        }
    } else {
        // Special mode 8: swap coordinates if rotation is odd
        if (_rotation & 1) {
            uint16_t tmp;
            tmp = x; x = y; y = tmp;
            tmp = x2; x2 = y2; y2 = tmp;
        }
        
        pb_writeCommand(cfg.cmd_set_addr_x, 8);
        if (cfg.allcmd_mode) {
            pb_writeCommand(x, 8);
            pb_writeCommand(x2, 8);
        } else {
            pb_writeData(x, 8);
            pb_writeData(x2, 8);
        }
        
        pb_writeCommand(cfg.cmd_set_addr_y, 8);
        if (cfg.allcmd_mode) {
            pb_writeCommand(y, 8);
            pb_writeCommand(y2, 8);
        } else {
            pb_writeData(y, 8);
            pb_writeData(y2, 8);
        }
        
        if (cfg.cmd_write_ram != 0xff) {
            pb_writeCommand(cfg.cmd_write_ram, 8);
        }
    }
    
    // Store for push operations
    _addr_x0 = x;
    _addr_y0 = y;
    _addr_x1 = x2;
    _addr_y1 = y2;
}

// Low-level I80 implementation
void I80Panel::calcClockDiv(uint32_t* div_a, uint32_t* div_b, uint32_t* div_n, uint32_t* clkcnt, uint32_t baseClock, uint32_t targetFreq) {
    uint32_t diff = INT32_MAX;
    *div_n = 256;
    *div_a = 63;
    *div_b = 62;
    *clkcnt = 64;
    
    uint32_t start_cnt = std::min<uint32_t>(64u, (baseClock / (targetFreq * 2) + 1));
    uint32_t end_cnt = std::max<uint32_t>(2u, baseClock / 256u / targetFreq);
    if (start_cnt <= 2) { end_cnt = 1; }
    
    for (uint32_t cnt = start_cnt; diff && cnt >= end_cnt; --cnt) {
        float fdiv = (float)baseClock / cnt / targetFreq;
        uint32_t n = std::max<uint32_t>(2u, (uint32_t)fdiv);
        fdiv -= n;

        for (uint32_t a = 63; diff && a > 0; --a) {
            uint32_t b = roundf(fdiv * a);
            if (a == b && n == 256) break;
            
            uint32_t freq = baseClock / ((n * cnt) + (float)(b * cnt) / (float)a);
            uint32_t d = abs((int)targetFreq - (int)freq);
            if (diff <= d) continue;
            
            diff = d;
            *clkcnt = cnt;
            *div_n = n;
            *div_b = b;
            *div_a = a;
            if (b == 0 || a == b) break;
        }
    }
    
    if (*div_a == *div_b) {
        *div_b = 0;
        *div_n += 1;
    }
}

void I80Panel::_alloc_dmadesc(size_t len) {
    if (_dmadesc) heap_caps_free(_dmadesc);
    _dmadesc_size = len;
    _dmadesc = (lldesc_t*)heap_caps_malloc(sizeof(lldesc_t) * len, MALLOC_CAP_DMA);
}

void I80Panel::_setup_dma_desc_links(const uint8_t *data, int32_t len) {
    // ORIGINAL CODE: This function was empty in the original implementation
    // DMA descriptor setup is incomplete - transfers larger than pre-allocated
    // descriptor count will be silently truncated, causing corrupted data
    // This matches the original uDisplay behavior but should be fixed eventually
    
    static constexpr size_t MAX_DMA_LEN = (4096-4);
    // TODO: Implement proper DMA descriptor chain setup
    // Currently, if len > MAX_DMA_LEN * _dmadesc_size, data will be truncated
    // without any error detection or recovery
}

void I80Panel::pb_beginTransaction(void) {
    auto dev = _dev;
    dev->lcd_clock.val = _clock_reg_value;
    dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
    dev->lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M;
}

void I80Panel::pb_endTransaction(void) {
    auto dev = _dev;
    while (dev->lcd_user.val & LCD_CAM_LCD_START) {}
}

void I80Panel::pb_wait(void) {
    auto dev = _dev;
    while (dev->lcd_user.val & LCD_CAM_LCD_START) {}
}

bool I80Panel::pb_busy(void) {
    auto dev = _dev;
    return (dev->lcd_user.val & LCD_CAM_LCD_START);
}

void I80Panel::_pb_init_pin(bool read) {
    if (read) {
        if (cfg.bus_width == 8) {
            for (size_t i = 0; i < 8; ++i) {
                gpio_ll_output_disable(&GPIO, (gpio_num_t)cfg.data_pins_low[i]);
            }
        } else {
            for (size_t i = 0; i < 8; ++i) {
                gpio_ll_output_disable(&GPIO, (gpio_num_t)cfg.data_pins_low[i]);
            }
            for (size_t i = 0; i < 8; ++i) {
                gpio_ll_output_disable(&GPIO, (gpio_num_t)cfg.data_pins_high[i]);
            }
        }
    } else {
        auto idx_base = LCD_DATA_OUT0_IDX;
        if (cfg.bus_width == 8) {
            for (size_t i = 0; i < 8; ++i) {
                gpio_matrix_out(cfg.data_pins_low[i], idx_base + i, 0, 0);
            }
        } else {
            for (size_t i = 0; i < 8; ++i) {
                gpio_matrix_out(cfg.data_pins_low[i], idx_base + i, 0, 0);
            }
            for (size_t i = 0; i < 8; ++i) {
                gpio_matrix_out(cfg.data_pins_high[i], idx_base + 8 + i, 0, 0);
            }
        }
    }
}

bool I80Panel::pb_writeCommand(uint32_t data, uint_fast8_t bit_length) {
    auto dev = _dev;
    auto reg_lcd_user = &(dev->lcd_user.val);
    dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

    if (cfg.bus_width == 8) {
        auto bytes = bit_length >> 3;
        do {
            dev->lcd_cmd_val.lcd_cmd_value = data;
            data >>= 8;
            while (*reg_lcd_user & LCD_CAM_LCD_START) {}
            *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
        } while (--bytes);
        return true;
    } else {
        dev->lcd_cmd_val.val = data;
        while (*reg_lcd_user & LCD_CAM_LCD_START) {}
        *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
        return true;
    }
}

void I80Panel::pb_writeData(uint32_t data, uint_fast8_t bit_length) {
    auto dev = _dev;
    auto reg_lcd_user = &(dev->lcd_user.val);
    dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
    auto bytes = bit_length >> 3;

    if (cfg.bus_width == 8) {
        uint8_t shift = (bytes - 1) * 8;
        for (uint32_t cnt = 0; cnt < bytes; cnt++) {
            dev->lcd_cmd_val.lcd_cmd_value = (data >> shift) & 0xff;
            shift -= 8;
            while (*reg_lcd_user & LCD_CAM_LCD_START) {}
            *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
        }
        return;
    } else {
        if (bytes == 1 || bytes == 4) {
            uint8_t shift = (bytes - 1) * 8;
            for (uint32_t cnt = 0; cnt < bytes; cnt++) {
                dev->lcd_cmd_val.lcd_cmd_value = (data >> shift) & 0xff;
                shift -= 8;
                while (*reg_lcd_user & LCD_CAM_LCD_START) {}
                *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
            }
            return;
        }

        dev->lcd_cmd_val.val = data;
        while (*reg_lcd_user & LCD_CAM_LCD_START) {}
        *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
        return;
    }
}

void I80Panel::pb_writeBytes(const uint8_t* data, uint32_t length, bool use_dma) {
    // original code commented out
    /*
    uint32_t freq = spi_speed * 1000000;
    uint32_t slow = (freq< 4000000) ? 2 : (freq < 8000000) ? 1 : 0;

    auto dev = _dev;
    do {
      auto reg_lcd_user = &(dev->lcd_user.val);
      dev->lcd_misc.lcd_cd_cmd_set  = 0;
      dev->lcd_cmd_val.lcd_cmd_value = data[0] | data[1] << 16;
      uint32_t cmd_val = data[2] | data[3] << 16;
      while (*reg_lcd_user & LCD_CAM_LCD_START) {}
      *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;

      if (use_dma) {
        if (slow) { ets_delay_us(slow); }
        _setup_dma_desc_links(&data[4], length - 4);
        gdma_start(_dma_chan, (intptr_t)(_dmadesc));
        length = 0;
      } else {
        size_t len = length;
        if (len > CACHE_SIZE) {
          len = (((len - 1) % CACHE_SIZE) + 4) & ~3u;
        }
        memcpy(_cache_flip, &data[4], (len-4+3)&~3);
        _setup_dma_desc_links((const uint8_t*)_cache_flip, len-4);
        gdma_start(_dma_chan, (intptr_t)(_dmadesc));
        length -= len;
        data += len;
        _cache_flip = _cache[(_cache_flip == _cache[0])];
      }
      dev->lcd_cmd_val.lcd_cmd_value = cmd_val;
      dev->lcd_misc.lcd_cd_data_set = 0;
      *reg_lcd_user = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_UPDATE_REG;
      while (*reg_lcd_user & LCD_CAM_LCD_START) {}
      *reg_lcd_user = LCD_CAM_LCD_ALWAYS_OUT_EN | LCD_CAM_LCD_DOUT | LCD_CAM_LCD_CMD | LCD_CAM_LCD_CMD_2_CYCLE_EN | LCD_CAM_LCD_START;
    } while (length);
*/
}

// FIXED: Byte swap logic was backwards in 8-bit mode
void I80Panel::pb_pushPixels(uint16_t* data, uint32_t length, bool swap_bytes, bool use_dma) {
    auto dev = _dev;
    auto reg_lcd_user = &(dev->lcd_user.val);
    dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;

    if (cfg.bus_width == 8) {
        if (swap_bytes) {
            for (uint32_t cnt = 0; cnt < length; cnt++) {
                dev->lcd_cmd_val.lcd_cmd_value = *data >> 8;  // High byte first
                while (*reg_lcd_user & LCD_CAM_LCD_START) {}
                *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
                dev->lcd_cmd_val.lcd_cmd_value = *data;       // Low byte second
                while (*reg_lcd_user & LCD_CAM_LCD_START) {}
                *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
                data++;
            }
        } else {
            for (uint32_t cnt = 0; cnt < length; cnt++) {
                dev->lcd_cmd_val.lcd_cmd_value = *data;       // Low byte first
                while (*reg_lcd_user & LCD_CAM_LCD_START) {}
                *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
                dev->lcd_cmd_val.lcd_cmd_value = *data >> 8;  // High byte second
                while (*reg_lcd_user & LCD_CAM_LCD_START) {}
                *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
                data++;
            }
        }
    } else {
        if (swap_bytes) {
            uint16_t iob;
            for (uint32_t cnt = 0; cnt < length; cnt++) {
                iob = *data++;
                iob = (iob << 8) | (iob >> 8);
                dev->lcd_cmd_val.lcd_cmd_value = iob;
                while (*reg_lcd_user & LCD_CAM_LCD_START) {}
                *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
            }
        } else {
            for (uint32_t cnt = 0; cnt < length; cnt++) {
                dev->lcd_cmd_val.lcd_cmd_value = *data++;
                while (*reg_lcd_user & LCD_CAM_LCD_START) {}
                *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_M | LCD_CAM_LCD_START;
            }
        }
    }
}

void I80Panel::cs_control(bool level) {
    auto pin = cfg.cs_pin;
    if (pin < 0) return;
    if (level) {
        gpio_hi(pin);
    } else {
        gpio_lo(pin);
    }
}

#endif // SOC_LCD_I80_SUPPORTED && SOC_LCDCAM_I80_NUM_BUSES