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
#include "esp_jpg_decode.h"

#include "esp_system.h"
#if ESP_IDF_VERSION_MAJOR >= 4 // IDF 4+
#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
#include "esp32/rom/tjpgd.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "tjpgd.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/rom/tjpgd.h"
#else
#error Target CONFIG_IDF_TARGET is not supported
#endif
#else // ESP32 Before IDF 4.0
#include "rom/tjpgd.h"
#endif

#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
#include "esp32-hal-log.h"
#define TAG ""
#else
#include "esp_log.h"
static const char* TAG = "esp_jpg_decode";
#endif

typedef struct {
        jpg_scale_t scale;
        jpg_reader_cb reader;
        jpg_writer_cb writer;
        void * arg;
        size_t len;
        size_t index;
} esp_jpg_decoder_t;

static const char * jd_errors[] = {
    "Succeeded",
    "Interrupted by output function",
    "Device error or wrong termination of input stream",
    "Insufficient memory pool for the image",
    "Insufficient stream input buffer",
    "Parameter error",
    "Data format error",
    "Right format but not supported",
    "Not supported JPEG standard"
};

static uint32_t _jpg_write(JDEC *decoder, void *bitmap, JRECT *rect)
{
    uint16_t x = rect->left;
    uint16_t y = rect->top;
    uint16_t w = rect->right + 1 - x;
    uint16_t h = rect->bottom + 1 - y;
    uint8_t *data = (uint8_t *)bitmap;

    esp_jpg_decoder_t * jpeg = (esp_jpg_decoder_t *)decoder->device;

    if (jpeg->writer) {
        return jpeg->writer(jpeg->arg, x, y, w, h, data);
    }
    return 0;
}

static uint32_t _jpg_read(JDEC *decoder, uint8_t *buf, uint32_t len)
{
    esp_jpg_decoder_t * jpeg = (esp_jpg_decoder_t *)decoder->device;
    if (jpeg->len && len > (jpeg->len - jpeg->index)) {
        len = jpeg->len - jpeg->index;
    }
    if (len) {
        len = jpeg->reader(jpeg->arg, jpeg->index, buf, len);
        if (!len) {
            ESP_LOGE(TAG, "Read Fail at %u/%u", jpeg->index, jpeg->len);
        }
        jpeg->index += len;
    }
    return len;
}

esp_err_t esp_jpg_decode(size_t len, jpg_scale_t scale, jpg_reader_cb reader, jpg_writer_cb writer, void * arg)
{
    static uint8_t work[3100];
    JDEC decoder;
    esp_jpg_decoder_t jpeg;

    jpeg.len = len;
    jpeg.reader = reader;
    jpeg.writer = writer;
    jpeg.arg = arg;
    jpeg.scale = scale;
    jpeg.index = 0;

    JRESULT jres = jd_prepare(&decoder, _jpg_read, work, 3100, &jpeg);
    if(jres != JDR_OK){
        ESP_LOGE(TAG, "JPG Header Parse Failed! %s", jd_errors[jres]);
        return ESP_FAIL;
    }

    uint16_t output_width = decoder.width / (1 << (uint8_t)(jpeg.scale));
    uint16_t output_height = decoder.height / (1 << (uint8_t)(jpeg.scale));

    //output start
    writer(arg, 0, 0, output_width, output_height, NULL);
    //output write
    jres = jd_decomp(&decoder, _jpg_write, (uint8_t)jpeg.scale);
    //output end
    writer(arg, output_width, output_height, output_width, output_height, NULL);

    if (jres != JDR_OK) {
        ESP_LOGE(TAG, "JPG Decompression Failed! %s", jd_errors[jres]);
        return ESP_FAIL;
    }
    //check if all data has been consumed.
    if (len && jpeg.index < len) {
        _jpg_read(&decoder, NULL, len - jpeg.index);
    }

    return ESP_OK;
}

