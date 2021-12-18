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
#include "soc/i2s_struct.h"
#include "esp_idf_version.h"
#if (ESP_IDF_VERSION_MAJOR >= 4) && (ESP_IDF_VERSION_MINOR > 1)
#include "hal/gpio_ll.h"
#else
#include "soc/gpio_periph.h"
#define esp_rom_delay_us ets_delay_us
static inline int gpio_ll_get_level(gpio_dev_t *hw, int gpio_num)
{
    if (gpio_num < 32) {
        return (hw->in >> gpio_num) & 0x1;
    } else {
        return (hw->in1.data >> (gpio_num - 32)) & 0x1;
    }
}
#endif
#include "ll_cam.h"
#include "xclk.h"
#include "cam_hal.h"

static const char *TAG = "esp32 ll_cam";

#define I2S_ISR_ENABLE(i) {I2S0.int_clr.i = 1;I2S0.int_ena.i = 1;}
#define I2S_ISR_DISABLE(i) {I2S0.int_ena.i = 0;I2S0.int_clr.i = 1;}

typedef union {
    struct {
        uint32_t sample2:8;
        uint32_t unused2:8;
        uint32_t sample1:8;
        uint32_t unused1:8;
    };
    uint32_t val;
} dma_elem_t;

typedef enum {
    /* camera sends byte sequence: s1, s2, s3, s4, ...
     * fifo receives: 00 s1 00 s2, 00 s2 00 s3, 00 s3 00 s4, ...
     */
    SM_0A0B_0B0C = 0,
    /* camera sends byte sequence: s1, s2, s3, s4, ...
     * fifo receives: 00 s1 00 s2, 00 s3 00 s4, ...
     */
    SM_0A0B_0C0D = 1,
    /* camera sends byte sequence: s1, s2, s3, s4, ...
     * fifo receives: 00 s1 00 00, 00 s2 00 00, 00 s3 00 00, ...
     */
    SM_0A00_0B00 = 3,
} i2s_sampling_mode_t;

typedef size_t (*dma_filter_t)(uint8_t* dst, const uint8_t* src, size_t len);

static i2s_sampling_mode_t sampling_mode = SM_0A00_0B00;

static size_t ll_cam_bytes_per_sample(i2s_sampling_mode_t mode)
{
    switch(mode) {
    case SM_0A00_0B00:
        return 4;
    case SM_0A0B_0B0C:
        return 4;
    case SM_0A0B_0C0D:
        return 2;
    default:
        assert(0 && "invalid sampling mode");
        return 0;
    }
}

static size_t IRAM_ATTR ll_cam_dma_filter_jpeg(uint8_t* dst, const uint8_t* src, size_t len)
{
    const dma_elem_t* dma_el = (const dma_elem_t*)src;
    size_t elements = len / sizeof(dma_elem_t);
    size_t end = elements / 4;
    // manually unrolling 4 iterations of the loop here
    for (size_t i = 0; i < end; ++i) {
        dst[0] = dma_el[0].sample1;
        dst[1] = dma_el[1].sample1;
        dst[2] = dma_el[2].sample1;
        dst[3] = dma_el[3].sample1;
        dma_el += 4;
        dst += 4;
    }
    return elements;
}

static size_t IRAM_ATTR ll_cam_dma_filter_grayscale(uint8_t* dst, const uint8_t* src, size_t len)
{
    const dma_elem_t* dma_el = (const dma_elem_t*)src;
    size_t elements = len / sizeof(dma_elem_t);
    size_t end = elements / 4;
    for (size_t i = 0; i < end; ++i) {
        // manually unrolling 4 iterations of the loop here
        dst[0] = dma_el[0].sample1;
        dst[1] = dma_el[1].sample1;
        dst[2] = dma_el[2].sample1;
        dst[3] = dma_el[3].sample1;
        dma_el += 4;
        dst += 4;
    }
    return elements;
}

static size_t IRAM_ATTR ll_cam_dma_filter_grayscale_highspeed(uint8_t* dst, const uint8_t* src, size_t len)
{
    const dma_elem_t* dma_el = (const dma_elem_t*)src;
    size_t elements = len / sizeof(dma_elem_t);
    size_t end = elements / 8;
    for (size_t i = 0; i < end; ++i) {
        // manually unrolling 4 iterations of the loop here
        dst[0] = dma_el[0].sample1;
        dst[1] = dma_el[2].sample1;
        dst[2] = dma_el[4].sample1;
        dst[3] = dma_el[6].sample1;
        dma_el += 8;
        dst += 4;
    }
    // the final sample of a line in SM_0A0B_0B0C sampling mode needs special handling
    if ((elements & 0x7) != 0) {
        dst[0] = dma_el[0].sample1;
        dst[1] = dma_el[2].sample1;
        elements += 1;
    }
    return elements / 2;
}

static size_t IRAM_ATTR ll_cam_dma_filter_yuyv(uint8_t* dst, const uint8_t* src, size_t len)
{
    const dma_elem_t* dma_el = (const dma_elem_t*)src;
    size_t elements = len / sizeof(dma_elem_t);
    size_t end = elements / 4;
    for (size_t i = 0; i < end; ++i) {
        dst[0] = dma_el[0].sample1;//y0
        dst[1] = dma_el[0].sample2;//u
        dst[2] = dma_el[1].sample1;//y1
        dst[3] = dma_el[1].sample2;//v

        dst[4] = dma_el[2].sample1;//y0
        dst[5] = dma_el[2].sample2;//u
        dst[6] = dma_el[3].sample1;//y1
        dst[7] = dma_el[3].sample2;//v
        dma_el += 4;
        dst += 8;
    }
    return elements * 2;
}

static size_t IRAM_ATTR ll_cam_dma_filter_yuyv_highspeed(uint8_t* dst, const uint8_t* src, size_t len)
{
    const dma_elem_t* dma_el = (const dma_elem_t*)src;
    size_t elements = len / sizeof(dma_elem_t);
    size_t end = elements / 8;
    for (size_t i = 0; i < end; ++i) {
        dst[0] = dma_el[0].sample1;//y0
        dst[1] = dma_el[1].sample1;//u
        dst[2] = dma_el[2].sample1;//y1
        dst[3] = dma_el[3].sample1;//v

        dst[4] = dma_el[4].sample1;//y0
        dst[5] = dma_el[5].sample1;//u
        dst[6] = dma_el[6].sample1;//y1
        dst[7] = dma_el[7].sample1;//v
        dma_el += 8;
        dst += 8;
    }
    if ((elements & 0x7) != 0) {
        dst[0] = dma_el[0].sample1;//y0
        dst[1] = dma_el[1].sample1;//u
        dst[2] = dma_el[2].sample1;//y1
        dst[3] = dma_el[2].sample2;//v
        elements += 4;
    }
    return elements;
}

static void IRAM_ATTR ll_cam_vsync_isr(void *arg)
{
    //DBG_PIN_SET(1);
    cam_obj_t *cam = (cam_obj_t *)arg;
    BaseType_t HPTaskAwoken = pdFALSE;
    // filter
    ets_delay_us(1);
    if (gpio_ll_get_level(&GPIO, cam->vsync_pin) == !cam->vsync_invert) {
        ll_cam_send_event(cam, CAM_VSYNC_EVENT, &HPTaskAwoken);
        if (HPTaskAwoken == pdTRUE) {
            portYIELD_FROM_ISR();
        }
    }
    //DBG_PIN_SET(0);
}

static void IRAM_ATTR ll_cam_dma_isr(void *arg)
{
    //DBG_PIN_SET(1);
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
    //DBG_PIN_SET(0);
}

bool ll_cam_stop(cam_obj_t *cam)
{
    I2S0.conf.rx_start = 0;
    I2S_ISR_DISABLE(in_suc_eof);
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

    I2S_ISR_ENABLE(in_suc_eof);

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

    I2S0.rx_eof_num = cam->dma_half_buffer_size / sizeof(dma_elem_t);
    I2S0.in_link.addr = ((uint32_t)&cam->dma[0]) & 0xfffff;

    I2S0.in_link.start = 1;
    I2S0.conf.rx_start = 1;
    return true;
}

esp_err_t ll_cam_config(cam_obj_t *cam, const camera_config_t *config)
{
    // Enable and configure I2S peripheral
    periph_module_enable(PERIPH_I2S0_MODULE);

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

    I2S0.conf.rx_slave_mod = 1;
    I2S0.conf.rx_right_first = 0;
    I2S0.conf.rx_msb_right = 0;
    I2S0.conf.rx_msb_shift = 0;
    I2S0.conf.rx_mono = 0;
    I2S0.conf.rx_short_sync = 0;

    I2S0.conf2.lcd_en = 1;
    I2S0.conf2.camera_en = 1;

    // Configure clock divider
    I2S0.clkm_conf.clkm_div_a = 0;
    I2S0.clkm_conf.clkm_div_b = 0;
    I2S0.clkm_conf.clkm_div_num = 2;
    
    I2S0.fifo_conf.dscr_en = 1;
    I2S0.fifo_conf.rx_fifo_mod = sampling_mode;
    I2S0.fifo_conf.rx_fifo_mod_force_en = 1;

    I2S0.conf_chan.rx_chan_mod = 1;
    I2S0.sample_rate_conf.rx_bits_mod = 0;
    I2S0.timing.val = 0;
    I2S0.timing.rx_dsync_sw = 1;

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
    gpio_matrix_in(config->pin_vsync, I2S0I_V_SYNC_IDX, false);

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
        gpio_matrix_in(data_pins[i], I2S0I_DATA_IN0_IDX + i, false);
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
}

uint8_t ll_cam_get_dma_align(cam_obj_t *cam)
{
    return 0;
}

static bool ll_cam_calc_rgb_dma(cam_obj_t *cam){
    size_t dma_half_buffer_max = CONFIG_CAMERA_DMA_BUFFER_SIZE_MAX / 2 / cam->dma_bytes_per_item;
    size_t dma_buffer_max = 2 * dma_half_buffer_max;
    size_t node_max = LCD_CAM_DMA_NODE_BUFFER_MAX_SIZE / cam->dma_bytes_per_item;

    size_t line_width = cam->width * cam->in_bytes_per_pixel;
    size_t image_size = cam->height * line_width;
    if (image_size > (4 * 1024 * 1024) || (line_width > dma_half_buffer_max)) {
        ESP_LOGE(TAG, "Resolution too high");
        return 0;
    }

    size_t node_size = node_max;
    size_t nodes_per_line = 1;
    size_t lines_per_node = 1;
    size_t lines_per_half_buffer = 1;
    size_t dma_half_buffer_min = node_max;
    size_t dma_half_buffer = dma_half_buffer_max;
    size_t dma_buffer_size = dma_buffer_max;

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
    // Calculate minimum EOF size = max(mode_size, line_size)
    dma_half_buffer_min = node_size * nodes_per_line;
    // Calculate max EOF size divisable by node size
    dma_half_buffer = (dma_half_buffer_max / dma_half_buffer_min) * dma_half_buffer_min;
    // Adjust EOF size so that height will be divisable by the number of lines in each EOF
    lines_per_half_buffer = dma_half_buffer / line_width;
    while((cam->height % lines_per_half_buffer) != 0){
        dma_half_buffer = dma_half_buffer - dma_half_buffer_min;
        lines_per_half_buffer = dma_half_buffer / line_width;
    }
    // Calculate DMA size
    dma_buffer_size =(dma_buffer_max / dma_half_buffer) * dma_half_buffer;
    
    ESP_LOGI(TAG, "node_size: %4u, nodes_per_line: %u, lines_per_node: %u, dma_half_buffer_min: %5u, dma_half_buffer: %5u, lines_per_half_buffer: %2u, dma_buffer_size: %5u, image_size: %u", 
            node_size * cam->dma_bytes_per_item, nodes_per_line, lines_per_node, dma_half_buffer_min * cam->dma_bytes_per_item, dma_half_buffer * cam->dma_bytes_per_item, lines_per_half_buffer, dma_buffer_size * cam->dma_bytes_per_item, image_size);

    cam->dma_buffer_size = dma_buffer_size * cam->dma_bytes_per_item;
    cam->dma_half_buffer_size = dma_half_buffer * cam->dma_bytes_per_item;
    cam->dma_node_buffer_size = node_size * cam->dma_bytes_per_item;
    cam->dma_half_buffer_cnt = cam->dma_buffer_size / cam->dma_half_buffer_size;
    return 1;
}

bool ll_cam_dma_sizes(cam_obj_t *cam)
{
    cam->dma_bytes_per_item = ll_cam_bytes_per_sample(sampling_mode);
    if (cam->jpeg_mode) {
        cam->dma_half_buffer_cnt = 8;
        cam->dma_node_buffer_size = 2048;
        cam->dma_half_buffer_size = cam->dma_node_buffer_size * 2;
        cam->dma_buffer_size = cam->dma_half_buffer_cnt * cam->dma_half_buffer_size;
    } else {
        return ll_cam_calc_rgb_dma(cam);
    }
    return 1;
}

static dma_filter_t dma_filter = ll_cam_dma_filter_jpeg;

size_t IRAM_ATTR ll_cam_memcpy(cam_obj_t *cam, uint8_t *out, const uint8_t *in, size_t len)
{
    //DBG_PIN_SET(1);
    size_t r = dma_filter(out, in, len);
    //DBG_PIN_SET(0);
    return r;
}

esp_err_t ll_cam_set_sample_mode(cam_obj_t *cam, pixformat_t pix_format, uint32_t xclk_freq_hz, uint16_t sensor_pid)
{
    if (pix_format == PIXFORMAT_GRAYSCALE) {
        if (sensor_pid == OV3660_PID || sensor_pid == OV5640_PID || sensor_pid == NT99141_PID) {
            if (xclk_freq_hz > 10000000) {
                sampling_mode = SM_0A00_0B00;
                dma_filter = ll_cam_dma_filter_yuyv_highspeed;
            } else {
                sampling_mode = SM_0A0B_0C0D;
                dma_filter = ll_cam_dma_filter_yuyv;
            }
            cam->in_bytes_per_pixel = 1;       // camera sends Y8
        } else {
            if (xclk_freq_hz > 10000000 && sensor_pid != OV7725_PID) {
                sampling_mode = SM_0A00_0B00;
                dma_filter = ll_cam_dma_filter_grayscale_highspeed;
            } else {
                sampling_mode = SM_0A0B_0C0D;
                dma_filter = ll_cam_dma_filter_grayscale;
            }
            cam->in_bytes_per_pixel = 2;       // camera sends YU/YV
        }
        cam->fb_bytes_per_pixel = 1;       // frame buffer stores Y8
    } else if (pix_format == PIXFORMAT_YUV422 || pix_format == PIXFORMAT_RGB565) {
            if (xclk_freq_hz > 10000000 && sensor_pid != OV7725_PID) {
                if (sensor_pid == OV7670_PID) {
                    sampling_mode = SM_0A0B_0B0C;
                } else {
                    sampling_mode = SM_0A00_0B00;
                }
                dma_filter = ll_cam_dma_filter_yuyv_highspeed;
            } else {
                sampling_mode = SM_0A0B_0C0D;
                dma_filter = ll_cam_dma_filter_yuyv;
            }
            cam->in_bytes_per_pixel = 2;       // camera sends YU/YV
            cam->fb_bytes_per_pixel = 2;       // frame buffer stores YU/YV/RGB565
    } else if (pix_format == PIXFORMAT_JPEG) {
        cam->in_bytes_per_pixel = 1;
        cam->fb_bytes_per_pixel = 1;
        dma_filter = ll_cam_dma_filter_jpeg;
        sampling_mode = SM_0A00_0B00;
    } else {
        ESP_LOGE(TAG, "Requested format is not supported");
        return ESP_ERR_NOT_SUPPORTED;
    }
    I2S0.fifo_conf.rx_fifo_mod = sampling_mode;
    return ESP_OK;
}
