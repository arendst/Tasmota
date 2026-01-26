/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2021 <ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifdef ESP32

#include <Wire.h>
#include <freertos/FreeRTOS.h>
#include "string.h"
#include "esp_log.h"
#include "rom/ets_sys.h"
#include "es8156.h"

/*
typedef struct {
    int mck_io_num;     //!< MCK in out pin
    int bck_io_num;     //!< BCK in out pin
    int ws_io_num;      //!< WS in out pin
    int data_out_num;   //!< DATA out pin
    int data_in_num;    //!< DATA in pin
} i2s_pin_config_t;
*/

static const char *TAG = "es8156";

static TwoWire *es8156_wire;


static esp_err_t es8156_write_reg(uint8_t reg_addr, uint8_t data)
{
    es8156_wire->beginTransmission(ES8156_ADDR);
    es8156_wire->write(reg_addr);
    es8156_wire->write(data);
    es8156_wire->endTransmission();
    return 0;
}

static uint8_t es8156_read_reg(uint8_t reg_addr)
{
    uint8_t data;
    es8156_wire->beginTransmission(ES8156_ADDR);
    es8156_wire->write(reg_addr);
    es8156_wire->endTransmission(false);
    es8156_wire->requestFrom(ES8156_ADDR, (size_t)1);
    data = es8156_wire->read();
    //i2c_bus_read_byte(i2c_handle, reg_addr, &data);
    return data;
}

esp_err_t es8156_codec_init(TwoWire *tw, audio_hal_codec_config_t *cfg)
{

    es8156_wire = tw;
    esp_err_t ret_val = ESP_OK;
    //ret_val |= bsp_i2c_add_device(&i2c_handle, ES8156_ADDR);

    uint8_t misc_ctrl_reg_val = 0x00;
    uint8_t dac_iface_reg_val = 0x00;

    if (AUDIO_HAL_MODE_MASTER == cfg->i2s_iface.mode) {
        misc_ctrl_reg_val |= 0b00100000;
    } else {
        misc_ctrl_reg_val |= 0b00000000;
    }

    switch (cfg->i2s_iface.bits) {
    case AUDIO_HAL_BIT_LENGTH_16BITS:
        dac_iface_reg_val |= 0b00110000;
        break;
    case AUDIO_HAL_BIT_LENGTH_24BITS:
        dac_iface_reg_val |= 0b00000000;
        break;
    case AUDIO_HAL_BIT_LENGTH_32BITS:
        dac_iface_reg_val |= 0b01000000;
        break;
    default:    /* Use 32 bit as default */
        dac_iface_reg_val |= 0b01000000;
        break;
    }

    ret_val |= es8156_write_reg(0x09, misc_ctrl_reg_val);  // MCLK from pad, Slave mode, power down DLL, enable pin pull up
    ret_val |= es8156_write_reg(0x11, dac_iface_reg_val);  // DAC Interface Config
    ret_val |= es8156_write_reg(0x14, ES8156_VOL_MIN_3dB); // Set default volume to 0dB

    return ret_val;
}

esp_err_t es8156_codec_deinit(void)
{
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t es8156_codec_set_voice_mute(bool enable)
{
    int regv = es8156_read_reg(ES8156_DAC_MUTE_REG13);
    if (enable) {
        regv = regv | BIT(1) | BIT(2);
    } else {
        regv = regv & (~(BIT(1) | BIT(2))) ;
    }
    es8156_write_reg(ES8156_DAC_MUTE_REG13, regv);
    return ESP_OK;
}

esp_err_t es8156_codec_set_voice_volume(uint8_t volume)
{
    if (volume > 100) {
        volume = 100;
    }
    uint8_t d = 0xBF - 60 + 6 * volume / 10;
    if (0 == volume) {
        d = 0;
    }
    return es8156_write_reg(ES8156_VOLUME_CONTROL_REG14, d);
}

esp_err_t es8156_codec_get_voice_volume(uint8_t *volume)
{
    *volume = es8156_read_reg(ES8156_VOLUME_CONTROL_REG14);

    return ESP_OK;
}

esp_err_t es8156_config_fmt(es_i2s_fmt_t fmt)
{
    ESP_LOGW(TAG, "Not support config format for es8156 now");
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t es8156_set_channel(uint8_t is_right)
{
    uint8_t reg = es8156_read_reg(ES8156_DAC_SDP_REG11);
    if (is_right) {
        reg |= 0b00000100;
    } else {
        reg &= 0b11111011;
    }
    return es8156_write_reg(ES8156_DAC_SDP_REG11, reg);
}

#endif
