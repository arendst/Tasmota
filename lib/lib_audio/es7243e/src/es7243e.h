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

#ifndef _ES7243E_H_
#define _ES7243E_H_

#include "audio_hal.h"
#include "esp_types.h"
#include "esxxx_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ES7243_ADDR 0x10

/**
 * @brief Initialize ES7243E adc chip
 *
 * @param codec_cfg configuration of ES7243E
 *
 * @return
 *     - ESP_OK
 *     - ESP_FAIL
 */
esp_err_t es7243e_adc_init(TwoWire *tw, audio_hal_codec_config_t *codec_cfg);

/**
 * @brief  Set gain of given mask
 *
 * @param[in] mic_mask Mask of MIC channel
 *
 * @param[in] gain:  gain
 *
 *       gain        :   value
 *       GAIN_0DB    :   1
 *       GAIN_3DB    :   2
 *       GAIN_6DB    :   3
 *           ·
 *           ·
 *           ·
 *       GAIN_30DB   :   10
 *       GAIN_33DB   :   11
 *       GAIN_34_5DB :   12
 *       GAIN_36DB   :   13
 *       GAIN_37_5DB :   14
 *
 * @return
 *     - ESP_OK
 *     - ESP_FAIL
 */
// esp_err_t es7243_adc_set_gain(es7243_input_mics_t mic_mask, es7243_gain_value_t gain);

void es7243e_setgain(uint8_t gain);

#ifdef __cplusplus
}
#endif

#endif
