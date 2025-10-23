#include "uDisplay.h"
#include "uDisplay_config.h"

#ifdef USE_ESP32_S3

#ifdef ESP32
#include "esp8266toEsp32.h"
#endif

#define WAIT_LCD_NOT_BUSY while (*reg_lcd_user & LCD_CAM_LCD_START) {}

// ===== Parallel Bus Control Functions =====

void uDisplay::pb_beginTransaction(void) {
    auto dev = _dev;
    dev->lcd_clock.val = _clock_reg_value;
    dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
    dev->lcd_user.val = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG;
    _cache_flip = _cache[0];
}

void uDisplay::pb_endTransaction(void) {
    auto dev = _dev;
    while (dev->lcd_user.val & LCD_CAM_LCD_START) {}
}

void uDisplay::pb_wait(void) {
    auto dev = _dev;
    while (dev->lcd_user.val & LCD_CAM_LCD_START) {}
}

bool uDisplay::pb_busy(void) {
    auto dev = _dev;
    return (dev->lcd_user.val & LCD_CAM_LCD_START);
}

// ===== Parallel Bus Write Functions =====

bool uDisplay::pb_writeCommand(uint32_t data, uint_fast8_t bit_length) {
    auto dev = _dev;
    auto reg_lcd_user = &(dev->lcd_user.val);
    dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE | LCD_CAM_LCD_CD_CMD_SET;

    if (interface == _UDSP_PAR8) {
        auto bytes = bit_length >> 3;
        do {
            dev->lcd_cmd_val.lcd_cmd_value = data;
            data >>= 8;
            WAIT_LCD_NOT_BUSY
            *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
        } while (--bytes);
        return true;
    } else {
        dev->lcd_cmd_val.val = data;
        WAIT_LCD_NOT_BUSY
        *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
        return true;
    }
}

void uDisplay::pb_writeData(uint32_t data, uint_fast8_t bit_length) {
    auto dev = _dev;
    auto reg_lcd_user = &(dev->lcd_user.val);
    dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;
    auto bytes = bit_length >> 3;

    if (interface == _UDSP_PAR8) {
        uint8_t shift = (bytes - 1) * 8;
        for (uint32_t cnt = 0; cnt < bytes; cnt++) {
            dev->lcd_cmd_val.lcd_cmd_value = (data >> shift) & 0xff;
            shift -= 8;
            WAIT_LCD_NOT_BUSY
            *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
        }
        return;
    } else {
        if (bytes == 1 || bytes == 4) {
            uint8_t shift = (bytes - 1) * 8;
            for (uint32_t cnt = 0; cnt < bytes; cnt++) {
                dev->lcd_cmd_val.lcd_cmd_value = (data >> shift) & 0xff;
                shift -= 8;
                WAIT_LCD_NOT_BUSY
                *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
            }
            return;
        }

        dev->lcd_cmd_val.val = data;
        WAIT_LCD_NOT_BUSY
        *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
        return;
    }
}

void uDisplay::pb_pushPixels(uint16_t* data, uint32_t length, bool swap_bytes, bool use_dma) {
    auto dev = _dev;
    auto reg_lcd_user = &(dev->lcd_user.val);
    dev->lcd_misc.val = LCD_CAM_LCD_CD_IDLE_EDGE;

    if (interface == _UDSP_PAR8) {
        if (swap_bytes) {
            for (uint32_t cnt = 0; cnt < length; cnt++) {
                dev->lcd_cmd_val.lcd_cmd_value = *data;
                while (*reg_lcd_user & LCD_CAM_LCD_START) {}
                *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
                dev->lcd_cmd_val.lcd_cmd_value = *data >> 8;
                WAIT_LCD_NOT_BUSY
                *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
                data++;
            }
        } else {
            for (uint32_t cnt = 0; cnt < length; cnt++) {
                dev->lcd_cmd_val.lcd_cmd_value = *data >> 8;
                while (*reg_lcd_user & LCD_CAM_LCD_START) {}
                *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
                dev->lcd_cmd_val.lcd_cmd_value = *data;
                WAIT_LCD_NOT_BUSY
                *reg_lcd_user = LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
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
                WAIT_LCD_NOT_BUSY
                *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
            }
        } else {
            for (uint32_t cnt = 0; cnt < length; cnt++) {
                dev->lcd_cmd_val.lcd_cmd_value = *data++;
                WAIT_LCD_NOT_BUSY
                *reg_lcd_user = LCD_CAM_LCD_2BYTE_EN | LCD_CAM_LCD_CMD | LCD_CAM_LCD_UPDATE_REG | LCD_CAM_LCD_START;
            }
        }
    }
}

#endif