#include "uDisplay_I2C_panel.h"

i2c_panel::i2c_panel(const I2CPanelConfig& config, uint8_t* framebuffer)
    : cfg(config), framebuffer(framebuffer) {
            
    // Execute initialization commands
    if (cfg.init_commands && cfg.init_commands_count > 0) {
        for (uint16_t i = 0; i < cfg.init_commands_count; i++) {
            i2c_command(cfg.init_commands[i]);
        }
    }
}

bool i2c_panel::updateFrame() {
    if (!framebuffer) return false;
    
    i2c_command(cfg.cmd_set_addr_x | 0x0);
    i2c_command(cfg.page_start | 0x0);  
    i2c_command(cfg.page_end | 0x0);

    uint8_t ys = cfg.height >> 3;
    uint8_t xs = cfg.width >> 3;
    uint8_t m_row = cfg.cmd_set_addr_y;
    uint8_t m_col = cfg.col_start;

    uint16_t p = 0;
    uint8_t i, j, k = 0;

    for (i = 0; i < ys; i++) {
        i2c_command(0xB0 + i + m_row);
        i2c_command(m_col & 0xf);
        i2c_command(0x10 | (m_col >> 4));

        for (j = 0; j < 8; j++) {
            cfg.wire->beginTransmission(cfg.i2c_address);
            cfg.wire->write(0x40);
            for (k = 0; k < xs; k++, p++) {
                cfg.wire->write(framebuffer[p]);
            }
            cfg.wire->endTransmission();
        }
    }
    return true;
}

bool i2c_panel::displayOnff(int8_t on) {
    i2c_command(on ? cfg.cmd_display_on : cfg.cmd_display_off);
    return true;
}

bool i2c_panel::invertDisplay(bool invert) {
    i2c_command(invert ? cfg.cmd_invert_on : cfg.cmd_invert_off);
    return true;
}

void i2c_panel::i2c_command(uint8_t val) {
    cfg.wire->beginTransmission(cfg.i2c_address);
    cfg.wire->write(0);
    cfg.wire->write(val);
    cfg.wire->endTransmission();
}