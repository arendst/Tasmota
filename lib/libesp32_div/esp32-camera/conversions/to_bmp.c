// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
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
#include <stddef.h>
#include <string.h>
#include "img_converters.h"
#include "soc/efuse_reg.h"
#include "esp_heap_caps.h"
#include "yuv.h"
#include "sdkconfig.h"
#include "esp_jpg_decode.h"

#include "esp_system.h"
#if ESP_IDF_VERSION_MAJOR >= 4 // IDF 4+
#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
#include "esp32/spiram.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/spiram.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/spiram.h"
#else 
#error Target CONFIG_IDF_TARGET is not supported
#endif
#else // ESP32 Before IDF 4.0
#include "esp_spiram.h"
#endif

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#define TAG ""
#else
#include "esp_log.h"
static const char* TAG = "to_bmp";
#endif

static const int BMP_HEADER_LEN = 54;

typedef struct {
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
    uint32_t dibheadersize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bmp_header_t;

typedef struct {
        uint16_t width;
        uint16_t height;
        uint16_t data_offset;
        const uint8_t *input;
        uint8_t *output;
} rgb_jpg_decoder;

static void *_malloc(size_t size)
{
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
}

//output buffer and image width
static bool _rgb_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
    rgb_jpg_decoder * jpeg = (rgb_jpg_decoder *)arg;
    if(!data){
        if(x == 0 && y == 0){
            //write start
            jpeg->width = w;
            jpeg->height = h;
            //if output is null, this is BMP
            if(!jpeg->output){
                jpeg->output = (uint8_t *)_malloc((w*h*3)+jpeg->data_offset);
                if(!jpeg->output){
                    return false;
                }
            }
        } else {
            //write end
        }
        return true;
    }

    size_t jw = jpeg->width*3;
    size_t t = y * jw;
    size_t b = t + (h * jw);
    size_t l = x * 3;
    uint8_t *out = jpeg->output+jpeg->data_offset;
    uint8_t *o = out;
    size_t iy, ix;

    w = w * 3;

    for(iy=t; iy<b; iy+=jw) {
        o = out+iy+l;
        for(ix=0; ix<w; ix+= 3) {
            o[ix] = data[ix+2];
            o[ix+1] = data[ix+1];
            o[ix+2] = data[ix];
        }
        data+=w;
    }
    return true;
}

static bool _rgb565_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
    rgb_jpg_decoder * jpeg = (rgb_jpg_decoder *)arg;
    if(!data){
        if(x == 0 && y == 0){
            //write start
            jpeg->width = w;
            jpeg->height = h;
            //if output is null, this is BMP
            if(!jpeg->output){
                jpeg->output = (uint8_t *)_malloc((w*h*3)+jpeg->data_offset);
                if(!jpeg->output){
                    return false;
                }
            }
        } else {
            //write end
        }
        return true;
    }

    size_t jw = jpeg->width*3;
    size_t jw2 = jpeg->width*2;
    size_t t = y * jw;
    size_t t2 = y * jw2;
    size_t b = t + (h * jw);
    size_t l = x * 2;
    uint8_t *out = jpeg->output+jpeg->data_offset;
    uint8_t *o = out;
    size_t iy, iy2, ix, ix2;

    w = w * 3;

    for(iy=t, iy2=t2; iy<b; iy+=jw, iy2+=jw2) {
        o = out+iy2+l;
        for(ix2=ix=0; ix<w; ix+= 3, ix2 +=2) {
            uint16_t r = data[ix];
            uint16_t g = data[ix+1];
            uint16_t b = data[ix+2];
            uint16_t c = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
            o[ix2+1] = c>>8;
            o[ix2] = c&0xff;
        }
        data+=w;
    }
    return true;
}

//input buffer
static uint32_t _jpg_read(void * arg, size_t index, uint8_t *buf, size_t len)
{
    rgb_jpg_decoder * jpeg = (rgb_jpg_decoder *)arg;
    if(buf) {
        memcpy(buf, jpeg->input + index, len);
    }
    return len;
}

static bool jpg2rgb888(const uint8_t *src, size_t src_len, uint8_t * out, jpg_scale_t scale)
{
    rgb_jpg_decoder jpeg;
    jpeg.width = 0;
    jpeg.height = 0;
    jpeg.input = src;
    jpeg.output = out;
    jpeg.data_offset = 0;

    if(esp_jpg_decode(src_len, scale, _jpg_read, _rgb_write, (void*)&jpeg) != ESP_OK){
        return false;
    }
    return true;
}

bool jpg2rgb565(const uint8_t *src, size_t src_len, uint8_t * out, jpg_scale_t scale)
{
    rgb_jpg_decoder jpeg;
    jpeg.width = 0;
    jpeg.height = 0;
    jpeg.input = src;
    jpeg.output = out;
    jpeg.data_offset = 0;

    if(esp_jpg_decode(src_len, scale, _jpg_read, _rgb565_write, (void*)&jpeg) != ESP_OK){
        return false;
    }
    return true;
}

bool jpg2bmp(const uint8_t *src, size_t src_len, uint8_t ** out, size_t * out_len)
{

    rgb_jpg_decoder jpeg;
    jpeg.width = 0;
    jpeg.height = 0;
    jpeg.input = src;
    jpeg.output = NULL;
    jpeg.data_offset = BMP_HEADER_LEN;

    if(esp_jpg_decode(src_len, JPG_SCALE_NONE, _jpg_read, _rgb_write, (void*)&jpeg) != ESP_OK){
        return false;
    }

    size_t output_size = jpeg.width*jpeg.height*3;

    jpeg.output[0] = 'B';
    jpeg.output[1] = 'M';
    bmp_header_t * bitmap  = (bmp_header_t*)&jpeg.output[2];
    bitmap->reserved = 0;
    bitmap->filesize = output_size+BMP_HEADER_LEN;
    bitmap->fileoffset_to_pixelarray = BMP_HEADER_LEN;
    bitmap->dibheadersize = 40;
    bitmap->width = jpeg.width;
    bitmap->height = -jpeg.height;//set negative for top to bottom
    bitmap->planes = 1;
    bitmap->bitsperpixel = 24;
    bitmap->compression = 0;
    bitmap->imagesize = output_size;
    bitmap->ypixelpermeter = 0x0B13 ; //2835 , 72 DPI
    bitmap->xpixelpermeter = 0x0B13 ; //2835 , 72 DPI
    bitmap->numcolorspallette = 0;
    bitmap->mostimpcolor = 0;

    *out = jpeg.output;
    *out_len = output_size+BMP_HEADER_LEN;

    return true;
}

bool fmt2rgb888(const uint8_t *src_buf, size_t src_len, pixformat_t format, uint8_t * rgb_buf)
{
    int pix_count = 0;
    if(format == PIXFORMAT_JPEG) {
        return jpg2rgb888(src_buf, src_len, rgb_buf, JPG_SCALE_NONE);
    } else if(format == PIXFORMAT_RGB888) {
        memcpy(rgb_buf, src_buf, src_len);
    } else if(format == PIXFORMAT_RGB565) {
        int i;
        uint8_t hb, lb;
        pix_count = src_len / 2;
        for(i=0; i<pix_count; i++) {
            hb = *src_buf++;
            lb = *src_buf++;
            *rgb_buf++ = (lb & 0x1F) << 3;
            *rgb_buf++ = (hb & 0x07) << 5 | (lb & 0xE0) >> 3;
            *rgb_buf++ = hb & 0xF8;
        }
    } else if(format == PIXFORMAT_GRAYSCALE) {
        int i;
        uint8_t b;
        pix_count = src_len;
        for(i=0; i<pix_count; i++) {
            b = *src_buf++;
            *rgb_buf++ = b;
            *rgb_buf++ = b;
            *rgb_buf++ = b;
        }
    } else if(format == PIXFORMAT_YUV422) {
        pix_count = src_len / 2;
        int i, maxi = pix_count / 2;
        uint8_t y0, y1, u, v;
        uint8_t r, g, b;
        for(i=0; i<maxi; i++) {
            y0 = *src_buf++;
            u = *src_buf++;
            y1 = *src_buf++;
            v = *src_buf++;

            yuv2rgb(y0, u, v, &r, &g, &b);
            *rgb_buf++ = b;
            *rgb_buf++ = g;
            *rgb_buf++ = r;

            yuv2rgb(y1, u, v, &r, &g, &b);
            *rgb_buf++ = b;
            *rgb_buf++ = g;
            *rgb_buf++ = r;
        }
    }
    return true;
}

bool fmt2bmp(uint8_t *src, size_t src_len, uint16_t width, uint16_t height, pixformat_t format, uint8_t ** out, size_t * out_len)
{
    if(format == PIXFORMAT_JPEG) {
        return jpg2bmp(src, src_len, out, out_len);
    }

    *out = NULL;
    *out_len = 0;

    int pix_count = width*height;
    size_t out_size = (pix_count * 3) + BMP_HEADER_LEN;
    uint8_t * out_buf = (uint8_t *)_malloc(out_size);
    if(!out_buf) {
        ESP_LOGE(TAG, "_malloc failed! %u", out_size);
        return false;
    }

    out_buf[0] = 'B';
    out_buf[1] = 'M';
    bmp_header_t * bitmap  = (bmp_header_t*)&out_buf[2];
    bitmap->reserved = 0;
    bitmap->filesize = out_size;
    bitmap->fileoffset_to_pixelarray = BMP_HEADER_LEN;
    bitmap->dibheadersize = 40;
    bitmap->width = width;
    bitmap->height = -height;//set negative for top to bottom
    bitmap->planes = 1;
    bitmap->bitsperpixel = 24;
    bitmap->compression = 0;
    bitmap->imagesize = pix_count * 3;
    bitmap->ypixelpermeter = 0x0B13 ; //2835 , 72 DPI
    bitmap->xpixelpermeter = 0x0B13 ; //2835 , 72 DPI
    bitmap->numcolorspallette = 0;
    bitmap->mostimpcolor = 0;

    uint8_t * rgb_buf = out_buf + BMP_HEADER_LEN;
    uint8_t * src_buf = src;


    //convert data to RGB888
    if(format == PIXFORMAT_RGB888) {
        memcpy(rgb_buf, src_buf, pix_count*3);
    } else if(format == PIXFORMAT_RGB565) {
        int i;
        uint8_t hb, lb;
        for(i=0; i<pix_count; i++) {
            hb = *src_buf++;
            lb = *src_buf++;
            *rgb_buf++ = (lb & 0x1F) << 3;
            *rgb_buf++ = (hb & 0x07) << 5 | (lb & 0xE0) >> 3;
            *rgb_buf++ = hb & 0xF8;
        }
    } else if(format == PIXFORMAT_GRAYSCALE) {
        int i;
        uint8_t b;
        for(i=0; i<pix_count; i++) {
            b = *src_buf++;
            *rgb_buf++ = b;
            *rgb_buf++ = b;
            *rgb_buf++ = b;
        }
    } else if(format == PIXFORMAT_YUV422) {
        int i, maxi = pix_count / 2;
        uint8_t y0, y1, u, v;
        uint8_t r, g, b;
        for(i=0; i<maxi; i++) {
            y0 = *src_buf++;
            u = *src_buf++;
            y1 = *src_buf++;
            v = *src_buf++;

            yuv2rgb(y0, u, v, &r, &g, &b);
            *rgb_buf++ = b;
            *rgb_buf++ = g;
            *rgb_buf++ = r;

            yuv2rgb(y1, u, v, &r, &g, &b);
            *rgb_buf++ = b;
            *rgb_buf++ = g;
            *rgb_buf++ = r;
        }
    }
    *out = out_buf;
    *out_len = out_size;
    return true;
}

bool frame2bmp(camera_fb_t * fb, uint8_t ** out, size_t * out_len)
{
    return fmt2bmp(fb->buf, fb->len, fb->width, fb->height, fb->format, out, out_len);
}
