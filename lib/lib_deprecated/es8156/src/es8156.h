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

#ifndef _ES8156_H
#define _ES8156_H

#include "audio_hal.h"
#include "esp_types.h"
#include "esxxx_common.h"

#ifdef __cplusplus
extern "C" {
#endif


#define ES8156_ADDR (0x08)

/* ES8156 register space */
/*
* RESET Control
*/
#define ES8156_RESET_REG00             0x00
/*
* Clock Managerment
*/
#define ES8156_MAINCLOCK_CTL_REG01     0x01
#define ES8156_SCLK_MODE_REG02         0x02
#define ES8156_LRCLK_DIV_H_REG03       0x03
#define ES8156_LRCLK_DIV_L_REG04       0x04
#define ES8156_SCLK_DIV_REG05          0x05
#define ES8156_NFS_CONFIG_REG06        0x06
#define ES8156_MISC_CONTROL1_REG07     0x07
#define ES8156_CLOCK_ON_OFF_REG08      0x08
#define ES8156_MISC_CONTROL2_REG09     0x09
#define ES8156_TIME_CONTROL1_REG0A     0x0a
#define ES8156_TIME_CONTROL2_REG0B     0x0b
/*
* System Control
*/
#define ES8156_CHIP_STATUS_REG0C       0x0c
#define ES8156_P2S_CONTROL_REG0D       0x0d
#define ES8156_DAC_OSR_COUNTER_REG10   0x10
/*
* SDP Control
*/
#define ES8156_DAC_SDP_REG11           0x11
#define ES8156_AUTOMUTE_SET_REG12      0x12
#define ES8156_DAC_MUTE_REG13          0x13
#define ES8156_VOLUME_CONTROL_REG14    0x14

/*
* ALC Control
*/
#define ES8156_ALC_CONFIG1_REG15       0x15
#define ES8156_ALC_CONFIG2_REG16       0x16
#define ES8156_ALC_CONFIG3_REG17       0x17
#define ES8156_MISC_CONTROL3_REG18     0x18
#define ES8156_EQ_CONTROL1_REG19       0x19
#define ES8156_EQ_CONTROL2_REG1A       0x1a
/*
* Analog System Control
*/
#define ES8156_ANALOG_SYS1_REG20       0x20
#define ES8156_ANALOG_SYS2_REG21       0x21
#define ES8156_ANALOG_SYS3_REG22       0x22
#define ES8156_ANALOG_SYS4_REG23       0x23
#define ES8156_ANALOG_LP_REG24         0x24
#define ES8156_ANALOG_SYS5_REG25       0x25
/*
* Chip Information
*/
#define ES8156_I2C_PAGESEL_REGFC       0xFC
#define ES8156_CHIPID1_REGFD           0xFD
#define ES8156_CHIPID0_REGFE           0xFE
#define ES8156_CHIP_VERSION_REGFF      0xFF

typedef enum {
    ES8156_VOL_MIN = 0x00,      /*!< -95.5dB */
    ES8156_VOL_MIN_10dB = 0xAB, /*!< -10dB */
    ES8156_VOL_MIN_9dB = 0xAD,  /*!< -9dB */
    ES8156_VOL_MIN_6dB = 0xB3,  /*!< -6dB */
    ES8156_VOL_MIN_3dB = 0xB9,  /*!< -3dB */
    ES8156_VOL_0dB = 0xBF,      /*!< 0dB */
    ES8156_VOL_3dB = 0xC5,      /*!< +3dB */
    ES8156_VOL_10dB = 0xD3,     /*!< +10dB */
    ES8156_VOL_MAX = 0xFF,      /*!< +32dB */
} es8156_volume_t;

/*
 * @brief Initialize ES8156 codec chip
 *
 * @param codec_cfg  configuration of ES8156
 *
 * @return
 *      - ESP_OK
 *      - ESP_FAIL
 */
esp_err_t es8156_codec_init(TwoWire *tw, audio_hal_codec_config_t *codec_cfg);

/**
 * @brief Deinitialize ES8156 codec chip
 *
 * @return
 *     - ESP_OK
 *     - ESP_FAIL
 */
esp_err_t es8156_codec_deinit(void);


esp_err_t es8156_config_fmt(es_i2s_fmt_t fmt);

/**
 * @brief Configure ES8156 DAC mute or not. Basically you can use this function to mute the output or unmute
 *
 * @param enable enable(1) or disable(0)
 *
 * @return
 *     - ESP_FAIL Parameter error
 *     - ESP_OK   Success
 */
esp_err_t es8156_codec_set_voice_mute(bool enable);

/**
 * @brief  Set voice volume
 *
 * @param volume:  voice volume (0~100)
 *
 * @return
 *     - ESP_OK
 *     - ESP_FAIL
 */
esp_err_t es8156_codec_set_voice_volume(uint8_t volume);

/**
 * @brief Get voice volume
 *
 * @param[out] *volume:  voice volume (0~255)
 *
 * @return
 *     - ESP_OK
 *     - ESP_FAIL
 */
esp_err_t es8156_codec_get_voice_volume(uint8_t *volume);

/**
 * @brief
 *
 * @param is_right
 * @return esp_err_t
 */
esp_err_t es8156_set_channel(uint8_t is_right);




#ifdef __cplusplus
}
#endif
#endif
