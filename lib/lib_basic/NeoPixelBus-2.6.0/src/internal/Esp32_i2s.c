// WARNING:  This file contains code that is more than likely already 
// exposed from the Esp32 Arduino API.  It will be removed once integration is complete.
//
// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#if defined(ARDUINO_ARCH_ESP32)

#include <string.h>
#include <stdio.h>
#include "stdlib.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"


#if ESP_IDF_VERSION_MAJOR>=4
#include "esp_intr_alloc.h"
#else
#include "esp_intr.h"
#endif

#include "soc/gpio_reg.h"
#include "soc/gpio_sig_map.h"
#include "soc/io_mux_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/i2s_struct.h"
#include "soc/dport_reg.h"
#include "soc/sens_reg.h"
#include "driver/gpio.h"
#include "driver/i2s.h"
#include "driver/dac.h"
#include "Esp32_i2s.h"
#include "esp32-hal.h"

#if ESP_IDF_VERSION_MAJOR<4
#define I2S_BASE_CLK (160000000L)
#endif

#define ESP32_REG(addr) (*((volatile uint32_t*)(0x3FF00000+(addr))))

#define I2S_DMA_QUEUE_SIZE      16

#define I2S_DMA_SILENCE_LEN     256 // bytes

typedef struct i2s_dma_item_s {
    uint32_t  blocksize: 12;    // datalen
    uint32_t  datalen  : 12;    // len*(bits_per_sample/8)*2 => max 2047*8bit/1023*16bit samples
    uint32_t  unused   :  5;    // 0
    uint32_t  sub_sof  :  1;    // 0
    uint32_t  eof      :  1;    // 1 => last?
    uint32_t  owner    :  1;    // 1

    void* data;                // malloc(datalen)
    struct i2s_dma_item_s* next;

    // if this pointer is not null, it will be freed
    void* free_ptr;

    // if DMA buffers are preallocated
    uint8_t* buf;
} i2s_dma_item_t;

typedef struct {
        i2s_dev_t* bus;
        int8_t  ws;
        int8_t  bck;
        int8_t  out;
        int8_t  in;
        uint32_t rate;
        intr_handle_t isr_handle;
        xQueueHandle tx_queue;

        uint8_t* silence_buf;
        size_t silence_len;

        i2s_dma_item_t* dma_items;
        size_t dma_count;
        uint32_t dma_buf_len :12;
        uint32_t unused      :20;
} i2s_bus_t;

static uint8_t i2s_silence_buf[I2S_DMA_SILENCE_LEN];

#if !defined(CONFIG_IDF_TARGET_ESP32S2)
// (I2S_NUM_MAX == 2)
static i2s_bus_t I2S[I2S_NUM_MAX] = {
    {&I2S0, -1, -1, -1, -1, 0, NULL, NULL, i2s_silence_buf, I2S_DMA_SILENCE_LEN, NULL, I2S_DMA_QUEUE_SIZE, 0, 0},
    {&I2S1, -1, -1, -1, -1, 0, NULL, NULL, i2s_silence_buf, I2S_DMA_SILENCE_LEN, NULL, I2S_DMA_QUEUE_SIZE, 0, 0}
};
#else
static i2s_bus_t I2S[I2S_NUM_MAX] = {
    {&I2S0, -1, -1, -1, -1, 0, NULL, NULL, i2s_silence_buf, I2S_DMA_SILENCE_LEN, NULL, I2S_DMA_QUEUE_SIZE, 0, 0}
};
#endif

void IRAM_ATTR i2sDmaISR(void* arg);
bool i2sInitDmaItems(uint8_t bus_num);

bool i2sInitDmaItems(uint8_t bus_num) {
    if (bus_num >= I2S_NUM_MAX) {
        return false;
    }
    if (I2S[bus_num].tx_queue) {// already set
        return true;
    }

    if (I2S[bus_num].dma_items == NULL) {
        I2S[bus_num].dma_items = (i2s_dma_item_t*)(malloc(I2S[bus_num].dma_count* sizeof(i2s_dma_item_t)));
        if (I2S[bus_num].dma_items == NULL) {
            log_e("MEM ERROR!");
            return false;
        }
    }

    int i, i2, a;
    i2s_dma_item_t* item;

    for(i=0; i<I2S[bus_num].dma_count; i++) {
        i2 = (i+1) % I2S[bus_num].dma_count;
        item = &I2S[bus_num].dma_items[i];
        item->eof = 1;
        item->owner = 1;
        item->sub_sof = 0;
        item->unused = 0;
        item->data = I2S[bus_num].silence_buf;
        item->blocksize = I2S[bus_num].silence_len;
        item->datalen = I2S[bus_num].silence_len;
        item->next = &I2S[bus_num].dma_items[i2];
        item->free_ptr = NULL;
        if (I2S[bus_num].dma_buf_len) {
            item->buf = (uint8_t*)(malloc(I2S[bus_num].dma_buf_len));
            if (item->buf == NULL) {
                log_e("MEM ERROR!");
                for(a=0; a<i; a++) {
                    free(I2S[bus_num].dma_items[i].buf);
                }
                free(I2S[bus_num].dma_items);
                I2S[bus_num].dma_items = NULL;
                return false;
            }
        } else {
            item->buf = NULL;
        }
    }

    I2S[bus_num].tx_queue = xQueueCreate(I2S[bus_num].dma_count, sizeof(i2s_dma_item_t*));
    if (I2S[bus_num].tx_queue == NULL) {// memory error
        log_e("MEM ERROR!");
        free(I2S[bus_num].dma_items);
        I2S[bus_num].dma_items = NULL;
        return false;
    }
    return true;
}

void i2sSetSilenceBuf(uint8_t bus_num, uint8_t* data, size_t len) {
    if (bus_num >= I2S_NUM_MAX || !data || !len) {
        return;
    }
    I2S[bus_num].silence_buf = data;
    I2S[bus_num].silence_len = len;
}

esp_err_t i2sSetClock(uint8_t bus_num, uint8_t div_num, uint8_t div_b, uint8_t div_a, uint8_t bck, uint8_t bits) {
    if (bus_num >= I2S_NUM_MAX || div_a > 63 || div_b > 63 || bck > 63) {
        return ESP_FAIL;
    }
    i2s_dev_t* i2s = I2S[bus_num].bus;

    typeof(i2s->clkm_conf) clkm_conf;

    clkm_conf.val = 0;
#if !defined(CONFIG_IDF_TARGET_ESP32S2)
    clkm_conf.clka_en = 0;
#else
    clkm_conf.clk_sel = 2;
#endif

    clkm_conf.clkm_div_a = div_a;
    clkm_conf.clkm_div_b = div_b;
    clkm_conf.clkm_div_num = div_num;
    i2s->clkm_conf.val = clkm_conf.val;

    typeof(i2s->sample_rate_conf) sample_rate_conf;
    sample_rate_conf.val = 0;
    sample_rate_conf.tx_bck_div_num = bck;
    sample_rate_conf.rx_bck_div_num = bck;
    sample_rate_conf.tx_bits_mod = bits;
    sample_rate_conf.rx_bits_mod = bits;
    i2s->sample_rate_conf.val = sample_rate_conf.val;
    return ESP_OK;
}

void i2sSetDac(uint8_t bus_num, bool right, bool left) {
    if (bus_num >= I2S_NUM_MAX) {
        return;
    }

    if (!right && !left) {
        dac_output_disable(DAC_CHANNEL_1);
        dac_output_disable(DAC_CHANNEL_2);
        dac_i2s_disable();
        I2S[bus_num].bus->conf2.lcd_en = 0;
        I2S[bus_num].bus->conf.tx_right_first = 0;
        I2S[bus_num].bus->conf2.camera_en = 0;
        I2S[bus_num].bus->conf.tx_msb_shift = 1;// I2S signaling
        return;
    }

    i2sSetPins(bus_num, -1, false);
    I2S[bus_num].bus->conf2.lcd_en = 1;
    I2S[bus_num].bus->conf.tx_right_first = 0;
    I2S[bus_num].bus->conf2.camera_en = 0;
    I2S[bus_num].bus->conf.tx_msb_shift = 0;
    dac_i2s_enable();
    
    if (right) {// DAC1, right channel, GPIO25
        dac_output_enable(DAC_CHANNEL_1);
    }
    if (left) { // DAC2, left  channel, GPIO26
        dac_output_enable(DAC_CHANNEL_2);
    }
}

void i2sSetPins(uint8_t bus_num, int8_t out, bool invert) {
    if (bus_num >= I2S_NUM_MAX) {
        return;
    }

    if (out >= 0) {
        if (I2S[bus_num].out != out) {
            if (I2S[bus_num].out >= 0) {
                gpio_matrix_out(I2S[bus_num].out, 0x100, invert, false);
            }
            I2S[bus_num].out = out;
            pinMode(out, OUTPUT);

            int i2sSignal;
#if !defined(CONFIG_IDF_TARGET_ESP32S2)
//            (I2S_NUM_MAX == 2)
            if (bus_num == 1) {
                i2sSignal = I2S1O_DATA_OUT23_IDX;
            }
            else
#endif
            {
                i2sSignal = I2S0O_DATA_OUT23_IDX;
            }

            gpio_matrix_out(out, i2sSignal, invert, false);
        }
    } else if (I2S[bus_num].out >= 0) {
        gpio_matrix_out(I2S[bus_num].out, 0x100, invert, false);
        I2S[bus_num].out = -1;
    }

}

bool i2sWriteDone(uint8_t bus_num) {
    if (bus_num >= I2S_NUM_MAX) {
        return false;
    }
    return (I2S[bus_num].dma_items[I2S[bus_num].dma_count - 1].data == I2S[bus_num].silence_buf);
}

void i2sInit(uint8_t bus_num, uint32_t bits_per_sample, uint32_t sample_rate, i2s_tx_chan_mod_t chan_mod, i2s_tx_fifo_mod_t fifo_mod, size_t dma_count, size_t dma_len) {
    if (bus_num >= I2S_NUM_MAX) {
        return;
    }

    I2S[bus_num].dma_count = dma_count;
    I2S[bus_num].dma_buf_len = dma_len & 0xFFF;

    if (!i2sInitDmaItems(bus_num)) {
        return;
    }

#if !defined(CONFIG_IDF_TARGET_ESP32S2)
// (I2S_NUM_MAX == 2)
    if (bus_num) {
        periph_module_enable(PERIPH_I2S1_MODULE);
    } else 
#endif
    {
        periph_module_enable(PERIPH_I2S0_MODULE);
    }

    esp_intr_disable(I2S[bus_num].isr_handle);
    i2s_dev_t* i2s = I2S[bus_num].bus;
    i2s->out_link.stop = 1;
    i2s->conf.tx_start = 0;
    i2s->int_ena.val = 0;
    i2s->int_clr.val = 0xFFFFFFFF;
    i2s->fifo_conf.dscr_en = 0;

    // reset fifo
    i2s->conf.rx_fifo_reset = 1;
    i2s->conf.rx_fifo_reset = 0;
    i2s->conf.tx_fifo_reset = 1;
    i2s->conf.tx_fifo_reset = 0;

    // reset i2s
    i2s->conf.tx_reset = 1;
    i2s->conf.tx_reset = 0;
    i2s->conf.rx_reset = 1;
    i2s->conf.rx_reset = 0;

    // reset dma
    i2s->lc_conf.in_rst = 1;
    i2s->lc_conf.in_rst = 0;
    i2s->lc_conf.out_rst = 1;
    i2s->lc_conf.out_rst = 0;

    // Enable and configure DMA
    typeof(i2s->lc_conf) lc_conf;
    lc_conf.val = 0;
    lc_conf.out_eof_mode = 1;
    i2s->lc_conf.val = lc_conf.val;

    i2s->pdm_conf.pcm2pdm_conv_en = 0;
    i2s->pdm_conf.pdm2pcm_conv_en = 0;
    // SET_PERI_REG_BITS(RTC_CNTL_CLK_CONF_REG, RTC_CNTL_SOC_CLK_SEL, 0x1, RTC_CNTL_SOC_CLK_SEL_S);

    typeof(i2s->conf_chan) conf_chan;
    conf_chan.val = 0;
    conf_chan.tx_chan_mod = chan_mod; //  0-two channel;1-right;2-left;3-righ;4-left
    conf_chan.rx_chan_mod = chan_mod; //  0-two channel;1-right;2-left;3-righ;4-left
    i2s->conf_chan.val = conf_chan.val;

    typeof(i2s->fifo_conf) fifo_conf;
    fifo_conf.val = 0;
    fifo_conf.tx_fifo_mod = fifo_mod; //  0-right&left channel;1-one channel
    fifo_conf.rx_fifo_mod = fifo_mod; //  0-right&left channel;1-one channel
    i2s->fifo_conf.val = fifo_conf.val;

    typeof(i2s->conf) conf;
    conf.val = 0;
    conf.tx_msb_shift = (bits_per_sample != 8);// 0:DAC/PCM, 1:I2S
    conf.tx_right_first = (bits_per_sample == 8);
    i2s->conf.val = conf.val;

    typeof(i2s->conf2) conf2;
    conf2.val = 0;
    conf2.lcd_en = (bits_per_sample == 8);
    i2s->conf2.val = conf2.val;

    i2s->fifo_conf.tx_fifo_mod_force_en = 1;

    i2s->pdm_conf.rx_pdm_en = 0;
    i2s->pdm_conf.tx_pdm_en = 0;

    i2sSetSampleRate(bus_num, sample_rate, bits_per_sample);

    //  enable intr in cpu // 
    int i2sIntSource;

#if !defined(CONFIG_IDF_TARGET_ESP32S2)
//    (I2S_NUM_MAX == 2)
    if (bus_num == 1) {
        i2sIntSource = ETS_I2S1_INTR_SOURCE;
    }
    else
#endif
    {
        i2sIntSource = ETS_I2S0_INTR_SOURCE;
    }


    esp_intr_alloc(i2sIntSource, ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LEVEL1, &i2sDmaISR, &I2S[bus_num], &I2S[bus_num].isr_handle);
    //  enable send intr
    i2s->int_ena.out_eof = 1;
    i2s->int_ena.out_dscr_err = 1;

    i2s->fifo_conf.dscr_en = 1;// enable dma
    i2s->out_link.start = 0;
    i2s->out_link.addr = (uint32_t)(&I2S[bus_num].dma_items[0]); // loads dma_struct to dma
    i2s->out_link.start = 1; // starts dma
    i2s->conf.tx_start = 1;// Start I2s module

    esp_intr_enable(I2S[bus_num].isr_handle);
}

esp_err_t i2sSetSampleRate(uint8_t bus_num, uint32_t rate, uint8_t bits) {
    if (bus_num >= I2S_NUM_MAX) {
        return ESP_FAIL;
    }

    if (I2S[bus_num].rate == rate) {
        return ESP_OK;
    }

    int clkmInteger, clkmDecimals, bck = 0;
    double denom = (double)1 / 63;
    int channel = 2;

//    double mclk;
    double clkmdiv;

    int factor;

    if (bits == 8) {
        factor = 120;
    } else {
        factor = (256 % bits) ? 384 : 256;
    }

    clkmdiv = (double)I2S_BASE_CLK / (rate* factor);
    if (clkmdiv > 256) {
        log_e("rate is too low");
        return ESP_FAIL;
    }
    I2S[bus_num].rate = rate;

    clkmInteger = clkmdiv;
    clkmDecimals = ((clkmdiv - clkmInteger) / denom);

    if (bits == 8) {
//        mclk = rate* factor;
        bck = 60;
        bits = 16;
    } else {
//        mclk = (double)clkmInteger + (denom* clkmDecimals);
        bck = factor/(bits* channel);
    }

    i2sSetClock(bus_num, clkmInteger, clkmDecimals, 63, bck, bits);

    return ESP_OK;
}

void IRAM_ATTR i2sDmaISR(void* arg)
{
    i2s_dma_item_t* dummy = NULL;
    i2s_bus_t* dev = (i2s_bus_t*)(arg);
    portBASE_TYPE hpTaskAwoken = 0;

    if (dev->bus->int_st.out_eof) {
        i2s_dma_item_t* item = (i2s_dma_item_t*)(dev->bus->out_eof_des_addr);
        item->data = dev->silence_buf;
        item->blocksize = dev->silence_len;
        item->datalen = dev->silence_len;
        if (xQueueIsQueueFullFromISR(dev->tx_queue) == pdTRUE) {
            xQueueReceiveFromISR(dev->tx_queue, &dummy, &hpTaskAwoken);
        }
        xQueueSendFromISR(dev->tx_queue, (void*)&item, &hpTaskAwoken);
    }
    dev->bus->int_clr.val = dev->bus->int_st.val;
    if (hpTaskAwoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

size_t i2sWrite(uint8_t bus_num, uint8_t* data, size_t len, bool copy, bool free_when_sent) {
    if (bus_num >= I2S_NUM_MAX || !I2S[bus_num].tx_queue) {
        return 0;
    }
    size_t index = 0;
    size_t toSend = len;
    size_t limit = I2S_DMA_MAX_DATA_LEN;
    i2s_dma_item_t* item = NULL;

    while (len) {
        toSend = len;
        if (toSend > limit) {
            toSend = limit;
        }

        if (xQueueReceive(I2S[bus_num].tx_queue, &item, portMAX_DELAY) == pdFALSE) {
            log_e("xQueueReceive failed\n");
            break;
        }
        // data is constant. no need to copy
        item->data = data + index;
        item->blocksize = toSend;
        item->datalen = toSend;

        len -= toSend;
        index += toSend;
    }
    return index;
}


#endif
