// Copyright 2015-2017 Espressif Systems (Shanghai) PTE LTD
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

#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/xtensa_api.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"
#include "esp_log.h"

#include "epaper-29-ws.h"

static const char* TAG = "ePaper Driver";

#define EPAPER_QUE_SIZE_DEFAULT 10

const unsigned char lut_full_update[] =
{
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00
};

// This LUT is not yet used in the code below
const unsigned char lut_partial_update[] =
{
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static portMUX_TYPE epaper_spinlock = portMUX_INITIALIZER_UNLOCKED;
#define EPAPER_ENTER_CRITICAL(mux)    portENTER_CRITICAL(mux)
#define EPAPER_EXIT_CRITICAL(mux)     portEXIT_CRITICAL(mux)

// LCD data/command
typedef struct {
    uint8_t dc_io;
    uint8_t dc_level;
} epaper_dc_t;

typedef struct {
    spi_device_handle_t bus;
    epaper_conf_t pin;      /* EPD properties */
    epaper_paint_t paint;   /* Paint properties */
    epaper_dc_t dc;
    xSemaphoreHandle spi_mux;
} epaper_dev_t;

/* This function is called (in irq context!) just before a transmission starts.
 * It will set the D/C line to the value indicated in the user field
 */
static void iot_epaper_pre_transfer_callback(spi_transaction_t *t)
{
    epaper_dc_t *dc = (epaper_dc_t *) t->user;
    gpio_set_level((int)dc->dc_io, (int)dc->dc_level);
}

static esp_err_t _iot_epaper_spi_send(spi_device_handle_t spi, spi_transaction_t* t)
{
    return spi_device_transmit(spi, t);
}

void iot_epaper_send(spi_device_handle_t spi, const uint8_t *data, int len, epaper_dc_t *dc)
{
    esp_err_t ret;
    if (len == 0) {
        return;    // no need to send anything
    }
    spi_transaction_t t = {
        .length = len * 8,  // Len is in bytes, transaction length is in bits.
        .tx_buffer = data,
        .user = (void *) dc,
    };
    ret = _iot_epaper_spi_send(spi, &t);
    assert(ret == ESP_OK);
}

static void iot_epaper_send_command(epaper_handle_t dev, unsigned char command)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    device->dc.dc_io = device->pin.dc_pin;
    device->dc.dc_level = device->pin.dc_lev_cmd;
    iot_epaper_send(device->bus, &command, 1, &device->dc);
}

static void iot_epaper_send_byte(epaper_handle_t dev, const uint8_t data)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    device->dc.dc_io = device->pin.dc_pin;
    device->dc.dc_level = device->pin.dc_lev_data;
    iot_epaper_send(device->bus, &data, 1, &device->dc);
}

static void iot_epaper_send_data(epaper_handle_t dev, const uint8_t *data, int length)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    device->dc.dc_io = device->pin.dc_pin;
    device->dc.dc_level = device->pin.dc_lev_data;
    // To Do: original driver was sending byte by byte
    // Pay attention to possible performance issues
    iot_epaper_send(device->bus, data, length, &device->dc);
    ESP_LOGI(TAG, "SPI data sent %d", length);
}

static void iot_epaper_paint_init(epaper_handle_t dev, unsigned char* image, int width, int height)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    device->paint.rotate = E_PAPER_ROTATE_0;
    device->paint.image = image;
    /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
    device->paint.width = width % 8 ? width + 8 - (width % 8) : width;
    device->paint.height = height;
}

static void iot_epaper_gpio_init(epaper_conf_t * pin)
{
    gpio_pad_select_gpio(pin->reset_pin);
    gpio_set_direction(pin->reset_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(pin->reset_pin, pin->rst_active_level);
    gpio_pad_select_gpio(pin->dc_pin);
    gpio_set_direction(pin->dc_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(pin->dc_pin, 1);
    ets_delay_us(10000);
    gpio_set_level(pin->dc_pin, 0);
    gpio_pad_select_gpio(pin->busy_pin);
    gpio_set_direction(pin->busy_pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(pin->busy_pin, GPIO_PULLUP_ONLY);
}

static esp_err_t iot_epaper_spi_init(epaper_handle_t dev, spi_device_handle_t *e_spi, epaper_conf_t *pin)
{
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .miso_io_num = -1,  // MISO not used, we are transferring to the slave only
        .mosi_io_num = pin->mosi_pin,
        .sclk_io_num = pin->sck_pin,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        // The maximum size sent below covers the case
        // when the whole frame buffer is transferred to the slave
        .max_transfer_sz = EPD_WIDTH * EPD_HEIGHT / 8,
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = pin->clk_freq_hz,
        .mode = 0,  // SPI mode 0
        .spics_io_num = pin->cs_pin,
        // To Do: clarify what does it mean
        .queue_size = EPAPER_QUE_SIZE_DEFAULT,
        // We are sending only in one direction (to the ePaper slave)
        .flags = (SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_3WIRE),
        //Specify pre-transfer callback to handle D/C line
        .pre_cb = iot_epaper_pre_transfer_callback,
    };
    ret = spi_bus_initialize(pin->spi_host, &buscfg, 1);
    assert(ret == ESP_OK);
    ret = spi_bus_add_device(pin->spi_host, &devcfg, e_spi);
    assert(ret == ESP_OK);
    return ret;
}

static void iot_epaper_set_lut(epaper_handle_t dev)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    iot_epaper_send_command(dev, E_PAPER_WRITE_LUT_REGISTER);
    iot_epaper_send_data(dev, lut_full_update, sizeof(lut_full_update));
    xSemaphoreGiveRecursive(device->spi_mux);
}

static void iot_epaper_epd_init(epaper_handle_t dev)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);

    iot_epaper_reset(dev);

    /* This part of code is ePaper module specific
     * It has been copied from the instructions as it is
     */
    iot_epaper_send_command(dev, E_PAPER_DRIVER_OUTPUT_CONTROL);
    iot_epaper_send_byte(dev, (EPD_HEIGHT - 1) & 0xff);
    iot_epaper_send_byte(dev, ((EPD_HEIGHT - 1) >> 8) & 0xff);
    iot_epaper_send_byte(dev, 0x00);    // GD = 0; SM = 0; TB = 0

    iot_epaper_send_command(dev, E_PAPER_BOOSTER_SOFT_START_CONTROL);
    iot_epaper_send_byte(dev, 0xD7);
    iot_epaper_send_byte(dev, 0xD6);
    iot_epaper_send_byte(dev, 0x9D);

    iot_epaper_send_command(dev, E_PAPER_WRITE_VCOM_REGISTER);
    iot_epaper_send_byte(dev, 0xA8);    // VCOM 7C

    iot_epaper_send_command(dev, E_PAPER_SET_DUMMY_LINE_PERIOD);
    iot_epaper_send_byte(dev, 0x1A);    // 4 dummy lines per gate

    iot_epaper_send_command(dev, E_PAPER_SET_GATE_TIME);
    iot_epaper_send_byte(dev, 0x08);    // 2us per line

    iot_epaper_send_command(dev, E_PAPER_DATA_ENTRY_MODE_SETTING);
    iot_epaper_send_byte(dev, 0x03);    // X increment; Y increment

    iot_epaper_set_lut(dev);
    xSemaphoreGiveRecursive(device->spi_mux);
}

epaper_handle_t iot_epaper_create(spi_device_handle_t bus, epaper_conf_t *epconf)
{
    epaper_dev_t* dev = (epaper_dev_t*) calloc(1, sizeof(epaper_dev_t));
    dev->spi_mux = xSemaphoreCreateRecursiveMutex();
    uint8_t* frame_buf = (unsigned char*) heap_caps_malloc(
            (epconf->width * epconf->height / 8), MALLOC_CAP_8BIT);
    if (frame_buf == NULL) {
        ESP_LOGE(TAG, "frame_buffer malloc fail");
        return NULL;
    }
    iot_epaper_gpio_init(epconf);
    ESP_LOGD(TAG, "gpio init ok");
    if (bus) {
        dev->bus = bus;
    } else {
        iot_epaper_spi_init(dev, &dev->bus, epconf);
        ESP_LOGD(TAG, "spi init ok");
    }
    dev->pin = *epconf;
    iot_epaper_epd_init(dev);
    iot_epaper_paint_init(dev, frame_buf, epconf->width, epconf->height);
    return (epaper_handle_t) dev;
}

esp_err_t iot_epaper_delete(epaper_handle_t dev, bool del_bus)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;

    iot_epaper_sleep(dev);

    spi_bus_remove_device(device->bus);
    if (del_bus) {
        spi_bus_free(device->pin.spi_host);
    }
    vSemaphoreDelete(device->spi_mux);
    if (device->paint.image) {
        free(device->paint.image);
        device->paint.image = NULL;
    }
    free(device);
    return ESP_OK;
}

int iot_epaper_get_width(epaper_handle_t dev)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    return device->paint.width;
}

void iot_epaper_set_width(epaper_handle_t dev, int width)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    device->paint.width = width % 8 ? width + 8 - (width % 8) : width;
    xSemaphoreGiveRecursive(device->spi_mux);

}

int iot_epaper_get_height(epaper_handle_t dev)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    return device->paint.height;
}

void iot_epaper_set_height(epaper_handle_t dev, int height)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    device->paint.height = height;
    xSemaphoreGiveRecursive(device->spi_mux);
}

int iot_epaper_get_rotate(epaper_handle_t dev)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    return device->paint.rotate;
}

void iot_epaper_set_rotate(epaper_handle_t dev, int rotate)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    device->paint.rotate = rotate;
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: Getters and Setters
 */
unsigned char* iot_epaper_get_image(epaper_handle_t dev)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    return device->paint.image;
}

/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 */
static void iot_epaper_draw_absolute_pixel(epaper_handle_t dev, int x, int y, int colored)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    if (x < 0 || x >= device->paint.width || y < 0 || y >= device->paint.height) {
        return;
    }
    EPAPER_ENTER_CRITICAL(&epaper_spinlock);
    if (device->pin.color_inv) {
        if (colored) {
            device->paint.image[(x + y * device->paint.width) / 8] |= 0x80 >> (x % 8);
        } else {
            device->paint.image[(x + y * device->paint.width) / 8] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (colored) {
            device->paint.image[(x + y * device->paint.width) / 8] &= ~(0x80 >> (x % 8));
        } else {
            device->paint.image[(x + y * device->paint.width) / 8] |= 0x80 >> (x % 8);
        }
    }
    EPAPER_EXIT_CRITICAL(&epaper_spinlock);
}

void iot_epaper_clean_paint(epaper_handle_t dev, int colored)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    for (int x = 0; x < device->paint.width; x++) {
        for (int y = 0; y < device->paint.height; y++) {
            iot_epaper_draw_absolute_pixel(dev, x, y, colored);
        }
    }
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: this displays a string on the frame buffer but not refresh
 */
void iot_epaper_draw_string(epaper_handle_t dev, int x, int y, const char* text, epaper_font_t* font, int colored)
{
    const char* p_text = text;
    unsigned int counter = 0;
    int refcolumn = x;
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        /* Display one character on EPD */
        iot_epaper_draw_char(dev, refcolumn, y, *p_text, font, colored);
        /* Decrement the column position by 16 */
        refcolumn += font->width;
        /* Point on the next character */
        p_text++;
        counter++;
    }
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void iot_epaper_draw_pixel(epaper_handle_t dev, int x, int y, int colored)
{
    int point_temp;
    epaper_dev_t* device = (epaper_dev_t*) dev;
    if (device->paint.rotate == E_PAPER_ROTATE_0) {
        if (x < 0 || x >= device->paint.width || y < 0 || y >= device->paint.height) {
            return;
        }
        iot_epaper_draw_absolute_pixel(dev, x, y, colored);
    } else if (device->paint.rotate == E_PAPER_ROTATE_90) {
        if (x < 0 || x >= device->paint.height || y < 0 || y >= device->paint.width) {
            return;
        }
        point_temp = x;
        x = device->paint.width - y;
        y = point_temp;
        iot_epaper_draw_absolute_pixel(dev, x, y, colored);
    } else if (device->paint.rotate == E_PAPER_ROTATE_180) {
        if (x < 0 || x >= device->paint.width || y < 0 || y >= device->paint.height) {
            return;
        }
        x = device->paint.width - x;
        y = device->paint.height - y;
        iot_epaper_draw_absolute_pixel(dev, x, y, colored);
    } else if (device->paint.rotate == E_PAPER_ROTATE_270) {
        if (x < 0 || x >= device->paint.height || y < 0 || y >= device->paint.width) {
            return;
        }
        point_temp = x;
        x = y;
        y = device->paint.height - point_temp;
        iot_epaper_draw_absolute_pixel(dev, x, y, colored);
    }
}

/**
 *  @brief: this draws a character on the frame buffer but not refresh
 */
void iot_epaper_draw_char(epaper_handle_t dev, int x, int y, char ascii_char, epaper_font_t* font, int colored)
{
    int i, j;
    unsigned int char_offset = (ascii_char - ' ') * font->height * (font->width / 8 + (font->width % 8 ? 1 : 0));
    const unsigned char* ptr = &font->font_table[char_offset];
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    for (j = 0; j < font->height; j++) {
        for (i = 0; i < font->width; i++) {
            if (*ptr & (0x80 >> (i % 8))) {
                iot_epaper_draw_pixel(dev, x + i, y + j, colored);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (font->width % 8 != 0) {
            ptr++;
        }
    }
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: this draws a line on the frame buffer
 */
void iot_epaper_draw_line(epaper_handle_t dev, int x0, int y0, int x1, int y1,
        int colored)
{
    /* Bresenham algorithm */
    int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    while ((x0 != x1) && (y0 != y1)) {
        iot_epaper_draw_pixel(dev, x0, y0, colored);
        if (2 * err >= dy) {
            err += dy;
            x0 += sx;
        }
        if (2 * err <= dx) {
            err += dx;
            y0 += sy;
        }
    }
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: this draws a horizontal line on the frame buffer
 */
void iot_epaper_draw_horizontal_line(epaper_handle_t dev, int x, int y, int width, int colored)
{
    int i;
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    for (i = x; i < x + width; i++) {
        iot_epaper_draw_pixel(dev, i, y, colored);
    }
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: this draws a vertical line on the frame buffer
 */
void iot_epaper_draw_vertical_line(epaper_handle_t dev, int x, int y, int height, int colored)
{
    int i;
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    for (i = y; i < y + height; i++) {
        iot_epaper_draw_pixel(dev, x, i, colored);
    }
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: this draws a rectangle
 */
void iot_epaper_draw_rectangle(epaper_handle_t dev, int x0, int y0, int x1, int y1, int colored)
{
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    iot_epaper_draw_horizontal_line(dev, min_x, min_y, max_x - min_x + 1, colored);
    iot_epaper_draw_horizontal_line(dev, min_x, max_y, max_x - min_x + 1, colored);
    iot_epaper_draw_vertical_line(dev, min_x, min_y, max_y - min_y + 1, colored);
    iot_epaper_draw_vertical_line(dev, max_x, min_y, max_y - min_y + 1, colored);
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: this draws a filled rectangle
 */
void ior_epaper_draw_filled_rectangle(epaper_handle_t dev, int x0, int y0, int x1, int y1, int colored)
{
    int min_x, min_y, max_x, max_y;
    int i;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    for (i = min_x; i <= max_x; i++) {
        iot_epaper_draw_vertical_line(dev, i, min_y, max_y - min_y + 1, colored);
    }
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: this draws a circle
 */
void iot_epaper_draw_circle(epaper_handle_t dev, int x, int y, int radius,
        int colored)
{
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    do {
        iot_epaper_draw_pixel(dev, x - x_pos, y + y_pos, colored);
        iot_epaper_draw_pixel(dev, x + x_pos, y + y_pos, colored);
        iot_epaper_draw_pixel(dev, x + x_pos, y - y_pos, colored);
        iot_epaper_draw_pixel(dev, x - x_pos, y - y_pos, colored);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
    xSemaphoreGiveRecursive(device->spi_mux);
}

/**
 *  @brief: this draws a filled circle
 */
void iot_epaper_draw_filled_circle(epaper_handle_t dev, int x, int y, int radius, int colored)
{
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    do {
        iot_epaper_draw_pixel(dev, x - x_pos, y + y_pos, colored);
        iot_epaper_draw_pixel(dev, x + x_pos, y + y_pos, colored);
        iot_epaper_draw_pixel(dev, x + x_pos, y - y_pos, colored);
        iot_epaper_draw_pixel(dev, x - x_pos, y - y_pos, colored);
        iot_epaper_draw_horizontal_line(dev, x + x_pos, y + y_pos, 2 * (-x_pos) + 1, colored);
        iot_epaper_draw_horizontal_line(dev, x + x_pos, y - y_pos, 2 * (-x_pos) + 1, colored);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
    xSemaphoreGiveRecursive(device->spi_mux);
}

void iot_epaper_wait_idle(epaper_handle_t dev)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    while (gpio_get_level((gpio_num_t) device->pin.busy_pin) == device->pin.busy_active_level) {
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void iot_epaper_reset(epaper_handle_t dev)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    gpio_set_level((gpio_num_t) device->pin.reset_pin, (~(device->pin.rst_active_level)) & 0x1);
    ets_delay_us(200);
    gpio_set_level((gpio_num_t) device->pin.reset_pin, (device->pin.rst_active_level) & 0x1);             //module reset
    ets_delay_us(200);
    gpio_set_level((gpio_num_t) device->pin.reset_pin, (~(device->pin.rst_active_level)) & 0x1);
    iot_epaper_wait_idle(dev);
    xSemaphoreGiveRecursive(device->spi_mux);
}

/* This function has been exposed to implement partial updates of the image
 * To Do: implement partial updates of the image
 */
void iot_set_ram_area(epaper_handle_t dev, int x_start, int y_start, int x_end, int y_end)
{
    iot_epaper_send_command(dev, E_PAPER_SET_RAM_X_ADDRESS_START_END_POSITION);
    iot_epaper_send_byte(dev, x_start >> 3);  // 8 pixels per byte
    iot_epaper_send_byte(dev, x_end   >> 3);  // 8 pixels per byte
    iot_epaper_send_command(dev, E_PAPER_SET_RAM_Y_ADDRESS_START_END_POSITION);
    iot_epaper_send_byte(dev, y_start & 0xff);
    iot_epaper_send_byte(dev, y_start >> 8);
    iot_epaper_send_byte(dev, y_end & 0xff);
    iot_epaper_send_byte(dev, y_end   >> 8);
}

/* This function has been exposed to implement partial updates of the image
 * To Do: implement partial updates of the image
 */
void iot_set_ram_address_counter(epaper_handle_t dev, int x, int y)
{
    iot_epaper_send_command(dev, E_PAPER_SET_RAM_X_ADDRESS_COUNTER);
    iot_epaper_send_byte(dev, x >> 3);  // 8 pixels per byte
    iot_epaper_send_command(dev, E_PAPER_SET_RAM_Y_ADDRESS_COUNTER);
    iot_epaper_send_byte(dev, y & 0xff);
    iot_epaper_send_byte(dev, y >> 8);
}

/* This transfer to the display the whole image frame
 * To Do: implement partial updates of the image
 */
void iot_epaper_display_frame(epaper_handle_t dev, const unsigned char* frame_buffer)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    if (frame_buffer == NULL) {
        frame_buffer = device->paint.image;
    }
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    if (frame_buffer != NULL) {

        // configure ePaper's memory to send data
        iot_set_ram_area(dev, 0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
        iot_set_ram_address_counter(dev, 0, 0);

        // send image data
        iot_epaper_send_command(dev, E_PAPER_WRITE_RAM);
        iot_epaper_send_data(dev, frame_buffer, device->paint.width / 8 * device->paint.height);

        // update display
        iot_epaper_send_command(dev, E_PAPER_DISPLAY_UPDATE_CONTROL_2);
        iot_epaper_send_byte(dev, 0xC4);
        iot_epaper_send_command(dev, E_PAPER_MASTER_ACTIVATION);
        iot_epaper_send_command(dev, E_PAPER_TERMINATE_FRAME_READ_WRITE);
        iot_epaper_wait_idle(dev);
    }
    xSemaphoreGiveRecursive(device->spi_mux);
}

void iot_epaper_sleep(epaper_handle_t dev)
{
    epaper_dev_t* device = (epaper_dev_t*) dev;
    xSemaphoreTakeRecursive(device->spi_mux, portMAX_DELAY);
    iot_epaper_send_command(dev, E_PAPER_DEEP_SLEEP_MODE);
    iot_epaper_wait_idle(dev);
    xSemaphoreGiveRecursive(device->spi_mux);
}

