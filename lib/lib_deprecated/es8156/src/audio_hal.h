/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
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

#ifndef _AUDIO_HAL_H_
#define _AUDIO_HAL_H_

#define AUDIO_HAL_VOL_DEFAULT 60

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Select media hal codec mode
 */
typedef enum {
    AUDIO_HAL_CODEC_MODE_ENCODE = 1,  /*!< select adc */
    AUDIO_HAL_CODEC_MODE_DECODE,      /*!< select dac */
    AUDIO_HAL_CODEC_MODE_BOTH,        /*!< select both adc and dac */
    AUDIO_HAL_CODEC_MODE_LINE_IN,     /*!< set adc channel */
} audio_hal_codec_mode_t;

/**
 * @brief Select adc channel for input mic signal
 */
typedef enum {
    AUDIO_HAL_ADC_INPUT_LINE1 = 0x00,  /*!< mic input to adc channel 1 */
    AUDIO_HAL_ADC_INPUT_LINE2,         /*!< mic input to adc channel 2 */
    AUDIO_HAL_ADC_INPUT_ALL,           /*!< mic input to both channels of adc */
    AUDIO_HAL_ADC_INPUT_DIFFERENCE,    /*!< mic input to adc difference channel */
} audio_hal_adc_input_t;

/**
 * @brief Select channel for dac output
 */
typedef enum {
    AUDIO_HAL_DAC_OUTPUT_LINE1 = 0x00,  /*!< dac output signal to channel 1 */
    AUDIO_HAL_DAC_OUTPUT_LINE2,         /*!< dac output signal to channel 2 */
    AUDIO_HAL_DAC_OUTPUT_ALL,           /*!< dac output signal to both channels */
} audio_hal_dac_output_t;

/**
 * @brief Select operating mode i.e. start or stop for audio codec chip
 */
typedef enum {
    AUDIO_HAL_CTRL_STOP  = 0x00,  /*!< set stop mode */
    AUDIO_HAL_CTRL_START = 0x01,  /*!< set start mode */
} audio_hal_ctrl_t;

/**
 * @brief Select I2S interface operating mode i.e. master or slave for audio codec chip
 */
typedef enum {
    AUDIO_HAL_MODE_SLAVE = 0x00,   /*!< set slave mode */
    AUDIO_HAL_MODE_MASTER = 0x01,  /*!< set master mode */
} audio_hal_iface_mode_t;

/**
 * @brief Select I2S interface samples per second
 */
typedef enum {
    AUDIO_HAL_08K_SAMPLES,   /*!< set to  8k samples per second */
    AUDIO_HAL_11K_SAMPLES,   /*!< set to 11.025k samples per second */
    AUDIO_HAL_16K_SAMPLES,   /*!< set to 16k samples in per second */
    AUDIO_HAL_22K_SAMPLES,   /*!< set to 22.050k samples per second */
    AUDIO_HAL_24K_SAMPLES,   /*!< set to 24k samples in per second */
    AUDIO_HAL_32K_SAMPLES,   /*!< set to 32k samples in per second */
    AUDIO_HAL_44K_SAMPLES,   /*!< set to 44.1k samples per second */
    AUDIO_HAL_48K_SAMPLES,   /*!< set to 48k samples per second */
} audio_hal_iface_samples_t;

/**
 * @brief Select I2S interface number of bits per sample
 */
typedef enum {
    AUDIO_HAL_BIT_LENGTH_16BITS = 1,   /*!< set 16 bits per sample */
    AUDIO_HAL_BIT_LENGTH_24BITS,       /*!< set 24 bits per sample */
    AUDIO_HAL_BIT_LENGTH_32BITS,       /*!< set 32 bits per sample */
} audio_hal_iface_bits_t;

/**
 * @brief Select I2S interface format for audio codec chip
 */
typedef enum {
    AUDIO_HAL_I2S_NORMAL = 0,  /*!< set normal I2S format */
    AUDIO_HAL_I2S_LEFT,        /*!< set all left format */
    AUDIO_HAL_I2S_RIGHT,       /*!< set all right format */
    AUDIO_HAL_I2S_DSP,         /*!< set dsp/pcm format */
} audio_hal_iface_format_t;

/**
 * @brief I2s interface configuration for audio codec chip
 */
typedef struct {
    audio_hal_iface_mode_t mode;        /*!< audio codec chip mode */
    audio_hal_iface_format_t fmt;       /*!< I2S interface format */
    audio_hal_iface_samples_t samples;  /*!< I2S interface samples per second */
    audio_hal_iface_bits_t bits;        /*!< i2s interface number of bits per sample */
} audio_hal_codec_i2s_iface_t;

/**
 * @brief Configure media hal for initialization of audio codec chip
 */
typedef struct {
    audio_hal_adc_input_t adc_input;       /*!< set adc channel */
    audio_hal_dac_output_t dac_output;     /*!< set dac channel */
    audio_hal_codec_mode_t codec_mode;     /*!< select codec mode: adc, dac or both */
    audio_hal_codec_i2s_iface_t i2s_iface; /*!< set I2S interface configuration */
} audio_hal_codec_config_t;

#ifdef __cplusplus
}
#endif

#endif //__AUDIO_HAL_H__
