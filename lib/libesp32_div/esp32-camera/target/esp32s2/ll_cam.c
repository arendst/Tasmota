// Copyright 2010-2020 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string.h>
#include "soc/system_reg.h"
#include "soc/i2s_struct.h"
#include "hal/gpio_ll.h"
#include "ll_cam.h"
#include "xclk.h"
#include "cam_hal.h"

static const char *TAG = "s2 ll_cam";

#define I2S_ISR_ENABLE(i) {I2S0.int_clr.i = 1;I2S0.int_ena.i = 1;}
#define I2S_ISR_DISABLE(i) {I2S0.int_ena.i = 0;I2S0.int_clr.i = 1;}

static void IRAM_ATTR ll_cam_vsync_isr(void *arg)
{
    //DBG_PIN_SET(1);
    cam_obj_t *cam = (cam_obj_t *)arg;
    BaseType_t HPTaskAwoken = pdFALSE;
    // filter
    ets_delay_us(1);
    if (gpio_ll_get_level(&GPIO, cam->vsync_pin) == !cam->vsync_invert) {
        ll_cam_send_event(cam, CAM_VSYNC_EVENT, &HPTaskAwoken);
    }

    if (HPTaskAwoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
    //DBG_PIN_SET(0);
}

static void IRAM_ATTR ll_cam_dma_isr(void *arg)
{
    cam_obj_t *cam = (cam_obj_t *)arg;
    BaseType_t HPTaskAwoken = pdFALSE;

    typeof(I2S0.int_st) status = I2S0.int_st;
    if (status.val == 0) {
        return;
    }

    I2S0.int_clr.val = status.val;

    if (status.in_suc_eof) {
        ll_cam_send_event(cam, CAM_IN_SUC_EOF_EVENT, &HPTaskAwoken);
    }

    if (HPTaskAwoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

bool ll_cam_stop(cam_obj_t *cam)
{
    I2S0.conf.rx_start = 0;

    if (cam->jpeg_mode || !cam->psram_mode) {
        I2S_ISR_DISABLE(in_suc_eof);
    }

    I2S0.in_link.stop = 1;
    return true;
}

esp_err_t ll_cam_deinit(cam_obj_t *cam)
{
    gpio_isr_handler_remove(cam->vsync_pin);

    if (cam->cam_intr_handle) {
        esp_intr_free(cam->cam_intr_handle);
        cam->cam_intr_handle = NULL;
    }

    return ESP_OK;
}

bool ll_cam_start(cam_obj_t *cam, int frame_pos)
{
    I2S0.conf.rx_start = 0;

    if (cam->jpeg_mode || !cam->psram_mode) {
        I2S_ISR_ENABLE(in_suc_eof);
    }

    I2S0.conf.rx_reset = 1;
    I2S0.conf.rx_reset = 0;
    I2S0.conf.rx_fifo_reset = 1;
    I2S0.conf.rx_fifo_reset = 0;
    I2S0.lc_conf.in_rst = 1;
    I2S0.lc_conf.in_rst = 0;
    I2S0.lc_conf.ahbm_fifo_rst = 1;
    I2S0.lc_conf.ahbm_fifo_rst = 0;
    I2S0.lc_conf.ahbm_rst = 1;
    I2S0.lc_conf.ahbm_rst = 0;

    I2S0.rx_eof_num = cam->dma_half_buffer_size; // Ping pong operation
    if (!cam->psram_mode) {
        I2S0.in_link.addr = ((uint32_t)&cam->dma[0]) & 0xfffff;
    } else {
        I2S0.in_link.addr = ((uint32_t)&cam->frames[frame_pos].dma[0]) & 0xfffff;
    }
    
    I2S0.in_link.start = 1;
    I2S0.conf.rx_start = 1;
    return true;
}

esp_err_t ll_cam_config(cam_obj_t *cam, const camera_config_t *config)
{
    esp_err_t err = camera_enable_out_clock(config);
    if(err != ESP_OK) {
        return err;
    }
    periph_module_enable(PERIPH_I2S0_MODULE);
    // Configure the clock
    I2S0.clkm_conf.clkm_div_num = 2; // 160MHz / 2 = 80MHz
    I2S0.clkm_conf.clkm_div_b = 0;
    I2S0.clkm_conf.clkm_div_a = 0;
    I2S0.clkm_conf.clk_sel = 2;
    I2S0.clkm_conf.clk_en = 1;


    I2S0.conf.val = 0;
    I2S0.fifo_conf.val = 0;
    I2S0.fifo_conf.dscr_en = 1;

    I2S0.lc_conf.ahbm_fifo_rst = 1;
    I2S0.lc_conf.ahbm_fifo_rst = 0;
    I2S0.lc_conf.ahbm_rst = 1;
    I2S0.lc_conf.ahbm_rst = 0;
    I2S0.lc_conf.check_owner = 0;
    //I2S0.lc_conf.indscr_burst_en = 1;
    //I2S0.lc_conf.ext_mem_bk_size = 0; // DMA access external memory block size. 0: 16 bytes, 1: 32 bytes, 2:64 bytes, 3:reserved

    I2S0.timing.val = 0;

    I2S0.int_ena.val = 0;
    I2S0.int_clr.val = ~0;

    I2S0.conf2.lcd_en = 1;
    I2S0.conf2.camera_en = 1;

    // Configuration data format
    I2S0.conf.rx_slave_mod = 1;
    I2S0.conf.rx_right_first = 0;
    I2S0.conf.rx_msb_right = cam->swap_data;
    I2S0.conf.rx_short_sync = 0;
    I2S0.conf.rx_mono = 0;
    I2S0.conf.rx_msb_shift = 0;
    I2S0.conf.rx_dma_equal = 1;

    // Configure sampling rate
    I2S0.sample_rate_conf.rx_bck_div_num = 1;
    I2S0.sample_rate_conf.rx_bits_mod = 8;

    I2S0.conf1.rx_pcm_bypass = 1;

    I2S0.conf2.i_v_sync_filter_en = 1;
    I2S0.conf2.i_v_sync_filter_thres = 4;
    I2S0.conf2.cam_sync_fifo_reset = 1;
    I2S0.conf2.cam_sync_fifo_reset = 0;

    I2S0.conf_chan.rx_chan_mod = 1;

    I2S0.fifo_conf.rx_fifo_mod_force_en = 1;
    I2S0.fifo_conf.rx_data_num = 32;
    I2S0.fifo_conf.rx_fifo_mod = 2;

    I2S0.lc_conf.in_rst  = 1;
    I2S0.lc_conf.in_rst  = 0;

    I2S0.conf.rx_start = 1;

    return ESP_OK;
}

void ll_cam_vsync_intr_enable(cam_obj_t *cam, bool en)
{
    if (en) {
        gpio_intr_enable(cam->vsync_pin);
    } else {
        gpio_intr_disable(cam->vsync_pin);
    }
}

esp_err_t ll_cam_set_pin(cam_obj_t *cam, const camera_config_t *config)
{
    gpio_config_t io_conf = {0};
    io_conf.intr_type = cam->vsync_invert ? GPIO_PIN_INTR_NEGEDGE : GPIO_PIN_INTR_POSEDGE;
    io_conf.pin_bit_mask = 1ULL << config->pin_vsync;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);
    gpio_install_isr_service(ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM);
    gpio_isr_handler_add(config->pin_vsync, ll_cam_vsync_isr, cam);
    gpio_intr_disable(config->pin_vsync);

    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[config->pin_pclk], PIN_FUNC_GPIO);
    gpio_set_direction(config->pin_pclk, GPIO_MODE_INPUT);
    gpio_set_pull_mode(config->pin_pclk, GPIO_FLOATING);
    gpio_matrix_in(config->pin_pclk, I2S0I_WS_IN_IDX, false);

    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[config->pin_vsync], PIN_FUNC_GPIO);
    gpio_set_direction(config->pin_vsync, GPIO_MODE_INPUT);
    gpio_set_pull_mode(config->pin_vsync, GPIO_FLOATING);
    gpio_matrix_in(config->pin_vsync, I2S0I_V_SYNC_IDX, cam->vsync_invert);

    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[config->pin_href], PIN_FUNC_GPIO);
    gpio_set_direction(config->pin_href, GPIO_MODE_INPUT);
    gpio_set_pull_mode(config->pin_href, GPIO_FLOATING);
    gpio_matrix_in(config->pin_href, I2S0I_H_SYNC_IDX, false);

    int data_pins[8] = {
        config->pin_d0, config->pin_d1, config->pin_d2, config->pin_d3, config->pin_d4, config->pin_d5, config->pin_d6, config->pin_d7,
    };
    for (int i = 0; i < 8; i++) {
        PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[data_pins[i]], PIN_FUNC_GPIO);
        gpio_set_direction(data_pins[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(data_pins[i], GPIO_FLOATING);
        // High bit alignment, IN16 is always the highest bit
        // fifo accesses data by bit, when rx_bits_mod is 8, the data needs to be aligned by 8 bits
        gpio_matrix_in(data_pins[i], I2S0I_DATA_IN0_IDX + 8 + i, false);
    }

    gpio_matrix_in(0x38, I2S0I_H_ENABLE_IDX, false);

    return ESP_OK;
}

esp_err_t ll_cam_init_isr(cam_obj_t *cam)
{
    return esp_intr_alloc(ETS_I2S0_INTR_SOURCE, ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM, ll_cam_dma_isr, cam, &cam->cam_intr_handle);
}

void ll_cam_do_vsync(cam_obj_t *cam)
{
    ll_cam_vsync_intr_enable(cam, false);
    gpio_matrix_in(cam->vsync_pin, I2S0I_V_SYNC_IDX, !cam->vsync_invert);
    ets_delay_us(10);
    gpio_matrix_in(cam->vsync_pin, I2S0I_V_SYNC_IDX, cam->vsync_invert);
    ll_cam_vsync_intr_enable(cam, true);
}

uint8_t ll_cam_get_dma_align(cam_obj_t *cam)
{
    return 64;//16 << I2S0.lc_conf.ext_mem_bk_size;
}

static bool ll_cam_calc_rgb_dma(cam_obj_t *cam){
    size_t node_max = LCD_CAM_DMA_NODE_BUFFER_MAX_SIZE / cam->dma_bytes_per_item;
    size_t line_width = cam->width * cam->in_bytes_per_pixel;
    size_t node_size = node_max;
    size_t nodes_per_line = 1;
    size_t lines_per_node = 1;

    // Calculate DMA Node Size so that it's divisable by or divisor of the line width
    if(line_width >= node_max){
        // One or more nodes will be requied for one line
        for(size_t i = node_max; i > 0; i=i-1){
            if ((line_width % i) == 0) {
                node_size = i;
                nodes_per_line = line_width / node_size;
                break;
            }
        }
    } else {
        // One or more lines can fit into one node
        for(size_t i = node_max; i > 0; i=i-1){
            if ((i % line_width) == 0) {
                node_size = i;
                lines_per_node = node_size / line_width;
                while((cam->height % lines_per_node) != 0){
                    lines_per_node = lines_per_node - 1;
                    node_size = lines_per_node * line_width;
                }
                break;
            }
        }
    }

    ESP_LOGI(TAG, "node_size: %4u, nodes_per_line: %u, lines_per_node: %u", 
            node_size * cam->dma_bytes_per_item, nodes_per_line, lines_per_node);

    cam->dma_node_buffer_size = node_size * cam->dma_bytes_per_item;

    if (cam->psram_mode) {
        cam->dma_buffer_size = cam->recv_size * cam->dma_bytes_per_item;
        cam->dma_half_buffer_cnt = 2;
        cam->dma_half_buffer_size = cam->dma_buffer_size / cam->dma_half_buffer_cnt;
    } else {
        size_t dma_half_buffer_max = CONFIG_CAMERA_DMA_BUFFER_SIZE_MAX / 2 / cam->dma_bytes_per_item;
        if (line_width > dma_half_buffer_max) {
            ESP_LOGE(TAG, "Resolution too high");
            return 0;
        }

        // Calculate minimum EOF size = max(mode_size, line_size)
        size_t dma_half_buffer_min = node_size * nodes_per_line;

        // Calculate max EOF size divisable by node size
        size_t dma_half_buffer = (dma_half_buffer_max / dma_half_buffer_min) * dma_half_buffer_min;

        // Adjust EOF size so that height will be divisable by the number of lines in each EOF
        size_t lines_per_half_buffer = dma_half_buffer / line_width;
        while((cam->height % lines_per_half_buffer) != 0){
            dma_half_buffer = dma_half_buffer - dma_half_buffer_min;
            lines_per_half_buffer = dma_half_buffer / line_width;
        }

        // Calculate DMA size
        size_t dma_buffer_max = 2 * dma_half_buffer_max;
        size_t dma_buffer_size = dma_buffer_max;
        dma_buffer_size =(dma_buffer_max / dma_half_buffer) * dma_half_buffer;
        
        ESP_LOGI(TAG, "dma_half_buffer_min: %5u, dma_half_buffer: %5u, lines_per_half_buffer: %2u, dma_buffer_size: %5u", 
                dma_half_buffer_min * cam->dma_bytes_per_item, dma_half_buffer * cam->dma_bytes_per_item, lines_per_half_buffer, dma_buffer_size * cam->dma_bytes_per_item);

        cam->dma_buffer_size = dma_buffer_size * cam->dma_bytes_per_item;
        cam->dma_half_buffer_size = dma_half_buffer * cam->dma_bytes_per_item;
        cam->dma_half_buffer_cnt = cam->dma_buffer_size / cam->dma_half_buffer_size;
    }
    return 1;
}

bool ll_cam_dma_sizes(cam_obj_t *cam)
{
    cam->dma_bytes_per_item = 1;
    if (cam->jpeg_mode) {
        if (cam->psram_mode) {
            cam->dma_buffer_size = cam->recv_size;
            cam->dma_half_buffer_size = 1024;
            cam->dma_half_buffer_cnt = cam->dma_buffer_size / cam->dma_half_buffer_size;
            cam->dma_node_buffer_size = cam->dma_half_buffer_size;
        } else {
            cam->dma_half_buffer_cnt = 16;
            cam->dma_buffer_size = cam->dma_half_buffer_cnt * 1024;
            cam->dma_half_buffer_size = cam->dma_buffer_size / cam->dma_half_buffer_cnt;
            cam->dma_node_buffer_size = cam->dma_half_buffer_size;
        }
    } else {
        return ll_cam_calc_rgb_dma(cam);
    }
    return 1;
}

size_t IRAM_ATTR ll_cam_memcpy(cam_obj_t *cam, uint8_t *out, const uint8_t *in, size_t len)
{
    // YUV to Grayscale
    if (cam->in_bytes_per_pixel == 2 && cam->fb_bytes_per_pixel == 1) {
        size_t end = len / 8;
        for (size_t i = 0; i < end; ++i) {
            out[0] = in[0];
            out[1] = in[2];
            out[2] = in[4];
            out[3] = in[6];
            out += 4;
            in += 8;
        }
        return len / 2;
    }

    // just memcpy
    memcpy(out, in, len);
    return len;
}

esp_err_t ll_cam_set_sample_mode(cam_obj_t *cam, pixformat_t pix_format, uint32_t xclk_freq_hz, uint16_t sensor_pid)
{
    if (pix_format == PIXFORMAT_GRAYSCALE) {
        if (sensor_pid == OV3660_PID || sensor_pid == OV5640_PID || sensor_pid == NT99141_PID) {
            cam->in_bytes_per_pixel = 1;       // camera sends Y8
        } else {
            cam->in_bytes_per_pixel = 2;       // camera sends YU/YV
        }
        cam->fb_bytes_per_pixel = 1;       // frame buffer stores Y8
    } else if (pix_format == PIXFORMAT_YUV422 || pix_format == PIXFORMAT_RGB565) {
            cam->in_bytes_per_pixel = 2;       // camera sends YU/YV
            cam->fb_bytes_per_pixel = 2;       // frame buffer stores YU/YV/RGB565
    } else if (pix_format == PIXFORMAT_JPEG) {
        cam->in_bytes_per_pixel = 1;
        cam->fb_bytes_per_pixel = 1;
    } else {
        ESP_LOGE(TAG, "Requested format is not supported");
        return ESP_ERR_NOT_SUPPORTED;
    }
    return ESP_OK;
}
