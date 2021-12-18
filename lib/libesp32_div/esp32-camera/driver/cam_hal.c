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
#include "esp_heap_caps.h"
#include "ll_cam.h"
#include "cam_hal.h"

static const char *TAG = "cam_hal";

static cam_obj_t *cam_obj = NULL;

static const uint32_t JPEG_SOI_MARKER = 0xFFD8FF;  // written in little-endian for esp32
static const uint16_t JPEG_EOI_MARKER = 0xD9FF;  // written in little-endian for esp32

static int cam_verify_jpeg_soi(const uint8_t *inbuf, uint32_t length)
{
    uint32_t sig = *((uint32_t *)inbuf) & 0xFFFFFF;
    if(sig != JPEG_SOI_MARKER) {
        for (uint32_t i = 0; i < length; i++) {
            sig = *((uint32_t *)(&inbuf[i])) & 0xFFFFFF;
            if (sig == JPEG_SOI_MARKER) {
                ESP_LOGW(TAG, "SOI: %d", i);
                return i;
            }
        }
        ESP_LOGW(TAG, "NO-SOI");
        return -1;
    }
    return 0;
}

static int cam_verify_jpeg_eoi(const uint8_t *inbuf, uint32_t length)
{
    int offset = -1;
    uint8_t *dptr = (uint8_t *)inbuf + length - 2;
    while (dptr > inbuf) {
        uint16_t sig = *((uint16_t *)dptr);
        if (JPEG_EOI_MARKER == sig) {
            offset = dptr - inbuf;
            //ESP_LOGW(TAG, "EOI: %d", length - (offset + 2));
            return offset;
        }
        dptr--;
    }
    return -1;
}

static bool cam_get_next_frame(int * frame_pos)
{
    if(!cam_obj->frames[*frame_pos].en){
        for (int x = 0; x < cam_obj->frame_cnt; x++) {
            if (cam_obj->frames[x].en) {
                *frame_pos = x;
                return true;
            }
        }
    } else {
        return true;
    }
    return false;
}

static bool cam_start_frame(int * frame_pos)
{
    if (cam_get_next_frame(frame_pos)) {
        if(ll_cam_start(cam_obj, *frame_pos)){
            // Vsync the frame manually
            ll_cam_do_vsync(cam_obj);
            uint64_t us = (uint64_t)esp_timer_get_time();
            cam_obj->frames[*frame_pos].fb.timestamp.tv_sec = us / 1000000UL;
            cam_obj->frames[*frame_pos].fb.timestamp.tv_usec = us % 1000000UL;
            return true;
        }
    }
    return false;
}

void IRAM_ATTR ll_cam_send_event(cam_obj_t *cam, cam_event_t cam_event, BaseType_t * HPTaskAwoken)
{
    if (xQueueSendFromISR(cam->event_queue, (void *)&cam_event, HPTaskAwoken) != pdTRUE) {
        ll_cam_stop(cam);
        cam->state = CAM_STATE_IDLE;
        ESP_EARLY_LOGE(TAG, "EV-%s-OVF", cam_event==CAM_IN_SUC_EOF_EVENT ? "EOF" : "VSYNC");
    }
}

//Copy fram from DMA dma_buffer to fram dma_buffer
static void cam_task(void *arg)
{
    int cnt = 0;
    int frame_pos = 0;
    cam_obj->state = CAM_STATE_IDLE;
    cam_event_t cam_event = 0;
    
    xQueueReset(cam_obj->event_queue);

    while (1) {
        xQueueReceive(cam_obj->event_queue, (void *)&cam_event, portMAX_DELAY);
        DBG_PIN_SET(1);
        switch (cam_obj->state) {

            case CAM_STATE_IDLE: {
                if (cam_event == CAM_VSYNC_EVENT) {
                    //DBG_PIN_SET(1);
                    if(cam_start_frame(&frame_pos)){
                        cam_obj->frames[frame_pos].fb.len = 0;
                        cam_obj->state = CAM_STATE_READ_BUF;
                    }
                    cnt = 0;
                }
            }
            break;

            case CAM_STATE_READ_BUF: {
                camera_fb_t * frame_buffer_event = &cam_obj->frames[frame_pos].fb;
                size_t pixels_per_dma = (cam_obj->dma_half_buffer_size * cam_obj->fb_bytes_per_pixel) / (cam_obj->dma_bytes_per_item * cam_obj->in_bytes_per_pixel);
                
                if (cam_event == CAM_IN_SUC_EOF_EVENT) {
                    if(!cam_obj->psram_mode){
                        if (cam_obj->fb_size < (frame_buffer_event->len + pixels_per_dma)) {
                            ESP_LOGW(TAG, "FB-OVF");
                            ll_cam_stop(cam_obj);
                            DBG_PIN_SET(0);
                            continue;
                        }
                        frame_buffer_event->len += ll_cam_memcpy(cam_obj,
                            &frame_buffer_event->buf[frame_buffer_event->len], 
                            &cam_obj->dma_buffer[(cnt % cam_obj->dma_half_buffer_cnt) * cam_obj->dma_half_buffer_size], 
                            cam_obj->dma_half_buffer_size);
                    }
                    //Check for JPEG SOI in the first buffer. stop if not found
                    if (cam_obj->jpeg_mode && cnt == 0 && cam_verify_jpeg_soi(frame_buffer_event->buf, frame_buffer_event->len) != 0) {
                        ll_cam_stop(cam_obj);
                        cam_obj->state = CAM_STATE_IDLE;
                    }
                    cnt++;

                } else if (cam_event == CAM_VSYNC_EVENT) {
                    //DBG_PIN_SET(1);
                    ll_cam_stop(cam_obj);

                    if (cnt || !cam_obj->jpeg_mode || cam_obj->psram_mode) {
                        if (cam_obj->jpeg_mode) {
                            if (!cam_obj->psram_mode) {
                                if (cam_obj->fb_size < (frame_buffer_event->len + pixels_per_dma)) {
                                    ESP_LOGW(TAG, "FB-OVF");
                                    cnt--;
                                } else {
                                    frame_buffer_event->len += ll_cam_memcpy(cam_obj,
                                        &frame_buffer_event->buf[frame_buffer_event->len], 
                                        &cam_obj->dma_buffer[(cnt % cam_obj->dma_half_buffer_cnt) * cam_obj->dma_half_buffer_size], 
                                        cam_obj->dma_half_buffer_size);
                                }
                            }
                            cnt++;
                        }

                        cam_obj->frames[frame_pos].en = 0;

                        if (cam_obj->psram_mode) {
                            if (cam_obj->jpeg_mode) {
                                frame_buffer_event->len = cnt * cam_obj->dma_half_buffer_size;
                            } else {
                                frame_buffer_event->len = cam_obj->recv_size;
                            }
                        } else if (!cam_obj->jpeg_mode) {
                            if (frame_buffer_event->len != cam_obj->fb_size) {
                                cam_obj->frames[frame_pos].en = 1;
                                ESP_LOGE(TAG, "FB-SIZE: %u != %u", frame_buffer_event->len, cam_obj->fb_size);
                            }
                        }
                        //send frame
                        if(!cam_obj->frames[frame_pos].en && xQueueSend(cam_obj->frame_buffer_queue, (void *)&frame_buffer_event, 0) != pdTRUE) {
                            //pop frame buffer from the queue
                            camera_fb_t * fb2 = NULL;
                            if(xQueueReceive(cam_obj->frame_buffer_queue, &fb2, 0) == pdTRUE) {
                                //push the new frame to the end of the queue
                                if (xQueueSend(cam_obj->frame_buffer_queue, (void *)&frame_buffer_event, 0) != pdTRUE) {
                                    cam_obj->frames[frame_pos].en = 1;
                                    ESP_LOGE(TAG, "FBQ-SND");
                                }
                                //free the popped buffer
                                cam_give(fb2);
                            } else {
                                //queue is full and we could not pop a frame from it
                                cam_obj->frames[frame_pos].en = 1;
                                ESP_LOGE(TAG, "FBQ-RCV");
                            }
                        }
                    }

                    if(!cam_start_frame(&frame_pos)){
                        cam_obj->state = CAM_STATE_IDLE;
                    } else {
                        cam_obj->frames[frame_pos].fb.len = 0;
                    }
                    cnt = 0;
                }
            }
            break;
        }
        DBG_PIN_SET(0);
    }
}

static lldesc_t * allocate_dma_descriptors(uint32_t count, uint16_t size, uint8_t * buffer)
{
    lldesc_t *dma = (lldesc_t *)heap_caps_malloc(count * sizeof(lldesc_t), MALLOC_CAP_DMA);
    if (dma == NULL) {
        return dma;
    }

    for (int x = 0; x < count; x++) {
        dma[x].size = size;
        dma[x].length = 0;
        dma[x].sosf = 0;
        dma[x].eof = 0;
        dma[x].owner = 1;
        dma[x].buf = (buffer + size * x);
        dma[x].empty = (uint32_t)&dma[(x + 1) % count];
    }
    return dma;
}

static esp_err_t cam_dma_config(const camera_config_t *config)
{
    bool ret = ll_cam_dma_sizes(cam_obj);
    if (0 == ret) {
        return ESP_FAIL;
    }

    cam_obj->dma_node_cnt = (cam_obj->dma_buffer_size) / cam_obj->dma_node_buffer_size; // Number of DMA nodes
    cam_obj->frame_copy_cnt = cam_obj->recv_size / cam_obj->dma_half_buffer_size; // Number of interrupted copies, ping-pong copy

    ESP_LOGI(TAG, "buffer_size: %d, half_buffer_size: %d, node_buffer_size: %d, node_cnt: %d, total_cnt: %d", 
             cam_obj->dma_buffer_size, cam_obj->dma_half_buffer_size, cam_obj->dma_node_buffer_size, cam_obj->dma_node_cnt, cam_obj->frame_copy_cnt);

    cam_obj->dma_buffer = NULL;
    cam_obj->dma = NULL;

    cam_obj->frames = (cam_frame_t *)heap_caps_calloc(1, cam_obj->frame_cnt * sizeof(cam_frame_t), MALLOC_CAP_DEFAULT);
    CAM_CHECK(cam_obj->frames != NULL, "frames malloc failed", ESP_FAIL);

    uint8_t dma_align = 0;
    size_t fb_size = cam_obj->fb_size;
    if (cam_obj->psram_mode) {
        dma_align = ll_cam_get_dma_align(cam_obj);
        if (cam_obj->fb_size < cam_obj->recv_size) {
            fb_size = cam_obj->recv_size;
        }
    }

    /* Allocate memeory for frame buffer */
    size_t alloc_size = fb_size * sizeof(uint8_t) + dma_align;
    uint32_t _caps = MALLOC_CAP_8BIT;
    if (CAMERA_FB_IN_DRAM == config->fb_location) {
        _caps |= MALLOC_CAP_INTERNAL;
    } else {
        _caps |= MALLOC_CAP_SPIRAM;
    }
    for (int x = 0; x < cam_obj->frame_cnt; x++) {
        cam_obj->frames[x].dma = NULL;
        cam_obj->frames[x].fb_offset = 0;
        cam_obj->frames[x].en = 0;
        ESP_LOGI(TAG, "Allocating %d Byte frame buffer in %s", alloc_size, _caps & MALLOC_CAP_SPIRAM ? "PSRAM" : "OnBoard RAM");
        cam_obj->frames[x].fb.buf = (uint8_t *)heap_caps_malloc(alloc_size, _caps);
        CAM_CHECK(cam_obj->frames[x].fb.buf != NULL, "frame buffer malloc failed", ESP_FAIL);
        if (cam_obj->psram_mode) {
            //align PSRAM buffer. TODO: save the offset so proper address can be freed later
            cam_obj->frames[x].fb_offset = dma_align - ((uint32_t)cam_obj->frames[x].fb.buf & (dma_align - 1));
            cam_obj->frames[x].fb.buf += cam_obj->frames[x].fb_offset;
            ESP_LOGI(TAG, "Frame[%d]: Offset: %u, Addr: 0x%08X", x, cam_obj->frames[x].fb_offset, (uint32_t)cam_obj->frames[x].fb.buf);
            cam_obj->frames[x].dma = allocate_dma_descriptors(cam_obj->dma_node_cnt, cam_obj->dma_node_buffer_size, cam_obj->frames[x].fb.buf);
            CAM_CHECK(cam_obj->frames[x].dma != NULL, "frame dma malloc failed", ESP_FAIL);
        }
        cam_obj->frames[x].en = 1;
    }

    if (!cam_obj->psram_mode) {
        cam_obj->dma_buffer = (uint8_t *)heap_caps_malloc(cam_obj->dma_buffer_size * sizeof(uint8_t), MALLOC_CAP_DMA);
        if(NULL == cam_obj->dma_buffer) {
            ESP_LOGE(TAG,"%s(%d): DMA buffer %d Byte malloc failed, the current largest free block:%d Byte", __FUNCTION__, __LINE__, 
                     cam_obj->dma_buffer_size, heap_caps_get_largest_free_block(MALLOC_CAP_DMA));
            return ESP_FAIL;
        }

        cam_obj->dma = allocate_dma_descriptors(cam_obj->dma_node_cnt, cam_obj->dma_node_buffer_size, cam_obj->dma_buffer);
        CAM_CHECK(cam_obj->dma != NULL, "dma malloc failed", ESP_FAIL);
    }

    return ESP_OK;
}

esp_err_t cam_init(const camera_config_t *config)
{
    CAM_CHECK(NULL != config, "config pointer is invalid", ESP_ERR_INVALID_ARG);

    esp_err_t ret = ESP_OK;
    cam_obj = (cam_obj_t *)heap_caps_calloc(1, sizeof(cam_obj_t), MALLOC_CAP_DMA);
    CAM_CHECK(NULL != cam_obj, "lcd_cam object malloc error", ESP_ERR_NO_MEM);

    cam_obj->swap_data = 0;
    cam_obj->vsync_pin = config->pin_vsync;
    cam_obj->vsync_invert = true;

    ll_cam_set_pin(cam_obj, config);
    ret = ll_cam_config(cam_obj, config);
    CAM_CHECK_GOTO(ret == ESP_OK, "ll_cam initialize failed", err);

#if CAMERA_DBG_PIN_ENABLE
    PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[DBG_PIN_NUM], PIN_FUNC_GPIO);
    gpio_set_direction(DBG_PIN_NUM, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(DBG_PIN_NUM, GPIO_FLOATING);
#endif

    ESP_LOGI(TAG, "cam init ok");
    return ESP_OK;

err:
    free(cam_obj);
    cam_obj = NULL;
    return ESP_FAIL;
}

esp_err_t cam_config(const camera_config_t *config, framesize_t frame_size, uint16_t sensor_pid)
{
    CAM_CHECK(NULL != config, "config pointer is invalid", ESP_ERR_INVALID_ARG);
    esp_err_t ret = ESP_OK;

    ret = ll_cam_set_sample_mode(cam_obj, (pixformat_t)config->pixel_format, config->xclk_freq_hz, sensor_pid);

    cam_obj->jpeg_mode = config->pixel_format == PIXFORMAT_JPEG;
#if CONFIG_IDF_TARGET_ESP32
    cam_obj->psram_mode = false;
#else
    cam_obj->psram_mode = (config->xclk_freq_hz == 16000000);
#endif
    cam_obj->frame_cnt = config->fb_count;
    cam_obj->width = resolution[frame_size].width;
    cam_obj->height = resolution[frame_size].height;

    if(cam_obj->jpeg_mode){
        cam_obj->recv_size = cam_obj->width * cam_obj->height / 5;
        cam_obj->fb_size = cam_obj->recv_size;
    } else {
        cam_obj->recv_size = cam_obj->width * cam_obj->height * cam_obj->in_bytes_per_pixel;
        cam_obj->fb_size = cam_obj->width * cam_obj->height * cam_obj->fb_bytes_per_pixel;
    }
    
    ret = cam_dma_config(config);
    CAM_CHECK_GOTO(ret == ESP_OK, "cam_dma_config failed", err);

    cam_obj->event_queue = xQueueCreate(cam_obj->dma_half_buffer_cnt - 1, sizeof(cam_event_t));
    CAM_CHECK_GOTO(cam_obj->event_queue != NULL, "event_queue create failed", err);

    size_t frame_buffer_queue_len = cam_obj->frame_cnt;
    if (config->grab_mode == CAMERA_GRAB_LATEST && cam_obj->frame_cnt > 1) {
        frame_buffer_queue_len = cam_obj->frame_cnt - 1;
    }
    cam_obj->frame_buffer_queue = xQueueCreate(frame_buffer_queue_len, sizeof(camera_fb_t*));
    CAM_CHECK_GOTO(cam_obj->frame_buffer_queue != NULL, "frame_buffer_queue create failed", err);

    ret = ll_cam_init_isr(cam_obj);
    CAM_CHECK_GOTO(ret == ESP_OK, "cam intr alloc failed", err);

    
#if CONFIG_CAMERA_CORE0
    xTaskCreatePinnedToCore(cam_task, "cam_task", 2048, NULL, configMAX_PRIORITIES - 2, &cam_obj->task_handle, 0);
#elif CONFIG_CAMERA_CORE1
    xTaskCreatePinnedToCore(cam_task, "cam_task", 2048, NULL, configMAX_PRIORITIES - 2, &cam_obj->task_handle, 1);
#else
    xTaskCreate(cam_task, "cam_task", 2048, NULL, configMAX_PRIORITIES - 2, &cam_obj->task_handle);
#endif

    ESP_LOGI(TAG, "cam config ok");
    return ESP_OK;

err:
    cam_deinit();
    return ESP_FAIL;
}

esp_err_t cam_deinit(void)
{
    if (!cam_obj) {
        return ESP_FAIL;
    }

    cam_stop();
    if (cam_obj->task_handle) {
        vTaskDelete(cam_obj->task_handle);
    }
    if (cam_obj->event_queue) {
        vQueueDelete(cam_obj->event_queue);
    }
    if (cam_obj->frame_buffer_queue) {
        vQueueDelete(cam_obj->frame_buffer_queue);
    }
    if (cam_obj->dma) {
        free(cam_obj->dma);
    }
    if (cam_obj->dma_buffer) {
        free(cam_obj->dma_buffer);
    }
    if (cam_obj->frames) {
        for (int x = 0; x < cam_obj->frame_cnt; x++) {
            free(cam_obj->frames[x].fb.buf - cam_obj->frames[x].fb_offset);
            if (cam_obj->frames[x].dma) {
                free(cam_obj->frames[x].dma);
            }
        }
        free(cam_obj->frames);
    }

    ll_cam_deinit(cam_obj);

    free(cam_obj);
    cam_obj = NULL;
    return ESP_OK;
}

void cam_stop(void)
{
    ll_cam_vsync_intr_enable(cam_obj, false);
    ll_cam_stop(cam_obj);
}

void cam_start(void)
{
    ll_cam_vsync_intr_enable(cam_obj, true);
}

camera_fb_t *cam_take(TickType_t timeout)
{
    camera_fb_t *dma_buffer = NULL;
    TickType_t start = xTaskGetTickCount();
    xQueueReceive(cam_obj->frame_buffer_queue, (void *)&dma_buffer, timeout);
    if (dma_buffer) {
        if(cam_obj->jpeg_mode){
            // find the end marker for JPEG. Data after that can be discarded
            int offset_e = cam_verify_jpeg_eoi(dma_buffer->buf, dma_buffer->len);
            if (offset_e >= 0) {
                // adjust buffer length
                dma_buffer->len = offset_e + sizeof(JPEG_EOI_MARKER);
                return dma_buffer;
            } else {
                ESP_LOGW(TAG, "NO-EOI");
                cam_give(dma_buffer);
                return cam_take(timeout - (xTaskGetTickCount() - start));//recurse!!!!
            }
        } else if(cam_obj->psram_mode && cam_obj->in_bytes_per_pixel != cam_obj->fb_bytes_per_pixel){
            //currently this is used only for YUV to GRAYSCALE
            dma_buffer->len = ll_cam_memcpy(cam_obj, dma_buffer->buf, dma_buffer->buf, dma_buffer->len);
        }
        return dma_buffer;
    } else {
        ESP_LOGW(TAG, "Failed to get the frame on time!");
    }
    return NULL;
}

void cam_give(camera_fb_t *dma_buffer)
{
    for (int x = 0; x < cam_obj->frame_cnt; x++) {
        if (&cam_obj->frames[x].fb == dma_buffer) {
            cam_obj->frames[x].en = 1;
            break;
        }
    }
}
