/*
  xdrv_81_esp32_webcam_CSI.ino - ESP32-P4 CSI webcam support for Tasmota

  Copyright (C) 2025  Christian Baars and Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef ESP32
#ifdef USE_CSI_WEBCAM

/*********************************************************************************************\
 * ESP32-P4 MIPI CSI Camera Driver
 * 
 * Architecture:
 * - C-Core: Generic CSI controller (this file)
 * - Berry: Sensor-specific logic (I2C, registers, power control)
 * 
 * Responsibilities:
 * - CSI controller initialization and configuration
 * - DMA buffer management
 * - Frame acquisition callbacks
 * - Generic frame get/release API
 * 
 * Does NOT handle:
 * - Sensor I2C/SCCB communication (Berry handles this)
 * - Sensor register configuration (Berry handles this)
 * - Reset/power-down GPIO (Berry handles this)
\*********************************************************************************************/

#define XDRV_81           81

#include "esp_cam_ctlr_csi.h"
#include "esp_cam_ctlr.h"
#include "driver/isp.h"
#include "driver/isp_bf.h"
#include "esp_cache.h"
#include "driver/jpeg_encode.h"
#include "esp_ldo_regulator.h"

/*********************************************************************************************/

// Configuration - what Berry tells us about the sensor (28 bytes)
struct CSI_Config {
  uint16_t width;           // 0-1: Active pixels per line
  uint16_t height;          // 2-3: Active lines per frame
  uint16_t max_width;       // 4-5: Maximum sensor resolution width
  uint16_t max_height;      // 6-7: Maximum sensor resolution height
  uint8_t format;           // 8: COLOR_PIXEL_RAW8/RAW10/RAW12 (pixel format part only)
  uint8_t lane_num;         // 9: Number of CSI lanes (typically 2)
  uint16_t mipi_clock;      // 10-11: Mbps per lane (e.g. 200)
  uint16_t offset_x;        // 12-13: Sensor X-offset (Ignored by C++, used by Berry for ROI)
  uint16_t offset_y;        // 14-15: Sensor Y-offset (Ignored by C++, used by Berry for ROI)
  uint8_t binning;          // 16: 1=None/1x1, 2=2x2, ...
  uint8_t fps;              // 17: Target FPS (e.g., 30)
  char name[8];             // 18-25: Sensor name (null-terminated)
  uint8_t res_index;        // 26: Resolution Index (0-255)
  uint8_t flags;            // 27: Bitmask (Bit 0=V-Flip, Bit 1=H-Mirror)
} __attribute__((packed));

#define CSI_FLAG_VFLIP    (1 << 0)
#define CSI_FLAG_HMIRROR  (1 << 1)

// Runtime state - handles and buffers
struct {
  esp_cam_ctlr_handle_t cam_handle;
  isp_proc_handle_t isp_handle;
  jpeg_encoder_handle_t jpeg_handle;
  esp_ldo_channel_handle_t ldo_mipi_phy;
  
  // Buffers (Double-Buffered / Ping-Pong)
  uint8_t *frame_buffer[2];    // Two buffers for ping-pong
  size_t frame_buffer_size;
  
  void *jpeg_buffer;
  size_t jpeg_buffer_size;
  jpeg_encode_cfg_t jpeg_cfg;
  
  CSI_Config config;        // Sensor configuration
  esp_cam_ctlr_trans_t cam_trans;  // Transaction struct
  
  volatile int write_idx;      // Buffer CSI is currently writing to (0 or 1)
  volatile int read_idx;       // Buffer available for reading (finished)
  
  TaskHandle_t cam_task_handle;  // FreeRTOS task for frame processing
  
  SemaphoreHandle_t client_mutex;  // Protects client_ptr access
  SemaphoreHandle_t jpeg_mutex;    // Protects JPEG encoder access
  
  uint8_t up;
  bool streaming;
  bool changing_state;         // Lock to prevent re-entrance in WcStart/WcStop
  uint8_t stream_active;
  WiFiClient *client_ptr;   // Pointer to avoid client() issues
  ESP8266WebServer *CamServer;
} Wc;

// Statistics
struct {
  uint32_t frames_captured;    // Total frames from ISR
  uint32_t frames_processed;   // Frames successfully encoded
  uint32_t frames_unsent;      // Frames captured but not sent to client
  uint32_t jpeg_errors;        // JPEG encoding errors
  uint32_t jpeg_resets;        // JPEG encoder resets (0x103 errors)
  uint32_t bytes_sent;         // Total bytes sent to clients
  uint32_t uptime_seconds;     // Streaming uptime
  uint32_t last_fps;           // Calculated FPS (updated every second)
  uint32_t last_frame_time_ms; // Last frame processing time
  uint32_t start_time;         // millis() when streaming started
} WcStats;

#define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"

/*********************************************************************************************/

// Debug counters for callbacks (volatile for ISR access)
static volatile uint32_t cb_get_new_count = 0;
static volatile uint32_t cb_finished_count = 0;

// Callback: Provide new buffer for next frame (Ping-Pong Logic)
static bool IRAM_ATTR csi_on_get_new_vb(esp_cam_ctlr_handle_t handle, esp_cam_ctlr_trans_t *trans, void *user_data) {
  cb_get_new_count++;
  
  // Switch to the OTHER buffer for the next write
  int next_idx = (Wc.write_idx + 1) % 2;
  Wc.write_idx = next_idx;
  
  // Give hardware the address of the new write buffer
  trans->buffer = Wc.frame_buffer[next_idx];
  trans->buflen = Wc.frame_buffer_size;
  
  return false; 
}

// Callback: Frame transfer finished - Wake processing task
static bool IRAM_ATTR csi_on_trans_finished(esp_cam_ctlr_handle_t handle, esp_cam_ctlr_trans_t *trans, void *user_data) {
  cb_finished_count++;
  
  // The buffer we just finished writing is now the readable one
  Wc.read_idx = Wc.write_idx;
  
  // Statistics: increment frames captured
  WcStats.frames_captured++;
  
  // Wake up the processing task immediately
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(Wc.cam_task_handle, &xHigherPriorityTaskWoken);
  
  return xHigherPriorityTaskWoken == pdTRUE;
}


/*********************************************************************************************/
// Camera Processing Task - Dedicated FreeRTOS task for frame processing

void CamProcessingTask(void *pvParameters) {
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(500); // 500ms timeout
  uint32_t last_fps_calc = millis();
  uint32_t frames_in_second = 0;
  
  while (true) {
    // Wait here until ISR signals "Frame Ready"
    // This consumes 0% CPU while waiting
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    
    if (ulNotificationValue > 0) {
      // We have a frame!
      uint32_t frame_start = millis();
      
      // Lock client access to prevent race with WcLoop
      if (xSemaphoreTake(Wc.client_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        // Only process if we have a client connected to stream
        if (Wc.stream_active == 2 && Wc.client_ptr && Wc.client_ptr->connected()) {
          uint8_t *source_buf = Wc.frame_buffer[Wc.read_idx];
          
          // Cache Sync (Hardware M2C)
          esp_cache_msync(source_buf, Wc.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
          
          // Lock JPEG encoder to prevent race with HandleImage
          if (xSemaphoreTake(Wc.jpeg_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            uint32_t jpeg_size = 0;
            esp_err_t ret = jpeg_encoder_process(Wc.jpeg_handle, 
                                                  &Wc.jpeg_cfg,
                                                  source_buf, 
                                                  Wc.frame_buffer_size,
                                                  (uint8_t*)Wc.jpeg_buffer, 
                                                  Wc.jpeg_buffer_size,
                                                  &jpeg_size);
            
            // Auto-Reset on 0x103 (Engine Stuck)
            if (ret == ESP_ERR_INVALID_STATE) {
              jpeg_del_encoder_engine(Wc.jpeg_handle);
              jpeg_encode_engine_cfg_t jpeg_eng_cfg = { .intr_priority = 0, .timeout_ms = 100 };
              jpeg_new_encoder_engine(&jpeg_eng_cfg, &Wc.jpeg_handle);
              WcStats.jpeg_resets++;
              xSemaphoreGive(Wc.jpeg_mutex);
              xSemaphoreGive(Wc.client_mutex);
              continue;
            }

            if (ret == ESP_OK && jpeg_size > 0) {
              Wc.client_ptr->print("--" BOUNDARY "\r\n");
              Wc.client_ptr->printf("Content-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n", jpeg_size);
              Wc.client_ptr->write((const uint8_t*)Wc.jpeg_buffer, jpeg_size);
              Wc.client_ptr->print("\r\n");
              
              // Statistics
              WcStats.frames_processed++;
              WcStats.bytes_sent += jpeg_size;
              frames_in_second++;
              
              // Yield to let network stack process
              taskYIELD();
            } else {
              WcStats.jpeg_errors++;
            }
            
            xSemaphoreGive(Wc.jpeg_mutex);
            
            // Track frame processing time
            WcStats.last_frame_time_ms = millis() - frame_start;
          } else {
            // Couldn't get JPEG mutex
            WcStats.frames_unsent++;
          }
        } else {
          // No client or not streaming - frame unsent
          WcStats.frames_unsent++;
        }
        
        xSemaphoreGive(Wc.client_mutex);
      } else {
        // Couldn't get client mutex
        WcStats.frames_unsent++;
      }
      
      // Calculate FPS every second
      if (millis() - last_fps_calc >= 1000) {
        WcStats.last_fps = frames_in_second;
        frames_in_second = 0;
        last_fps_calc = millis();
        
        // Update uptime
        if (WcStats.start_time > 0) {
          WcStats.uptime_seconds = (millis() - WcStats.start_time) / 1000;
        }
      }
    }
  }
}

/*********************************************************************************************/

uint32_t WcSetup(bool reset_config) {
  if (Wc.up) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI already initialized"));
    return Wc.up;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== SETUP START (Double-Buffered) ====="));

  // 1. Initialize MIPI PHY LDO
  esp_ldo_channel_config_t ldo_mipi_phy_config = {
    .chan_id = 3,        // LDO_VO3 for MIPI PHY
    .voltage_mv = 2500,  // 2.5V for MIPI PHY
  };
  
  esp_err_t ret = esp_ldo_acquire_channel(&ldo_mipi_phy_config, &Wc.ldo_mipi_phy);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to acquire MIPI LDO (0x%x)"), ret);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: MIPI PHY LDO enabled"));

  // PRE-FILL CONFIG WITH DEFAULTS (only on first boot, not on resolution change)
  if (reset_config) {
    memset(&Wc.config, 0, sizeof(Wc.config));
    Wc.config.width = 800;
    Wc.config.height = 640;
    Wc.config.max_width = 0;
    Wc.config.max_height = 0;
    Wc.config.format = 0;        // RAW8
    Wc.config.lane_num = 2;
    Wc.config.mipi_clock = 200;
    Wc.config.offset_x = 0;
    Wc.config.offset_y = 0;
    Wc.config.binning = 0;
    Wc.config.fps = 0;
    Wc.config.res_index = 0;
    Wc.config.flags = 0;
  }

  // 2. Call Berry to initialize sensor (zero-copy: pass struct address as idx)
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== CALLING BERRY INIT ====="));
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Config buffer addr=0x%08X size=%d bytes"), (uint32_t)&Wc.config, sizeof(CSI_Config));
  
  uint32_t config_addr = (uint32_t)&Wc.config;
  int32_t result = callBerryEventDispatcher(PSTR("camera"), PSTR("init"), config_addr, nullptr, 0);
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== BERRY INIT RESULT=%d ====="), result);
  
  if (result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry init failed or no driver loaded"));
    return 0;
  }
  
  // Log raw bytes for debugging
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Raw bytes [0-7]: %02X %02X %02X %02X %02X %02X %02X %02X"),
    ((uint8_t*)&Wc.config)[0], ((uint8_t*)&Wc.config)[1], ((uint8_t*)&Wc.config)[2], ((uint8_t*)&Wc.config)[3],
    ((uint8_t*)&Wc.config)[4], ((uint8_t*)&Wc.config)[5], ((uint8_t*)&Wc.config)[6], ((uint8_t*)&Wc.config)[7]);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Raw bytes [18-25]: %02X %02X %02X %02X %02X %02X %02X %02X"),
    ((uint8_t*)&Wc.config)[18], ((uint8_t*)&Wc.config)[19], ((uint8_t*)&Wc.config)[20], ((uint8_t*)&Wc.config)[21],
    ((uint8_t*)&Wc.config)[22], ((uint8_t*)&Wc.config)[23], ((uint8_t*)&Wc.config)[24], ((uint8_t*)&Wc.config)[25]);
  
  // Log what Berry sent us (DRY RUN - verify data exchange)
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== CONFIG FROM BERRY ====="));
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Sensor Name: %.8s"), Wc.config.name);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Resolution: %dx%d"), Wc.config.width, Wc.config.height);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Max Resolution: %dx%d"), Wc.config.max_width, Wc.config.max_height);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Format: %d (COLOR_PIXEL_RAW8/10/12)"), Wc.config.format);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: MIPI Clock: %d Mbps/lane"), Wc.config.mipi_clock);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Lanes: %d"), Wc.config.lane_num);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Offset: X=%d Y=%d"), Wc.config.offset_x, Wc.config.offset_y);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Binning: %d (0=1x1, 1=2x2, 2=4x4)"), Wc.config.binning);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: FPS: %d"), Wc.config.fps);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ResIndex: %d"), Wc.config.res_index);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Flags: 0x%02X (VFlip=%d HMirror=%d)"), 
    Wc.config.flags,
    (Wc.config.flags & CSI_FLAG_VFLIP) ? 1 : 0,
    (Wc.config.flags & CSI_FLAG_HMIRROR) ? 1 : 0);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== END CONFIG ====="));

  // 3. Allocate Two Frame Buffers (Aligned 64-byte for JPEG DMA)
  // Free old buffers if they exist (resolution change scenario)
  for (int i = 0; i < 2; i++) {
    if (Wc.frame_buffer[i]) {
      free(Wc.frame_buffer[i]);
      Wc.frame_buffer[i] = NULL;
    }
  }
  
  Wc.frame_buffer_size = Wc.config.width * Wc.config.height * 2; // RGB565 = 2 bytes per pixel
  
  for (int i = 0; i < 2; i++) {
      Wc.frame_buffer[i] = (uint8_t*)heap_caps_aligned_calloc(64, 1, Wc.frame_buffer_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      if (!Wc.frame_buffer[i]) {
        AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Alloc failed for buffer %d"), i);
        return 0;
      }
      // Pre-flush cache
      esp_cache_msync(Wc.frame_buffer[i], Wc.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_C2M);
  }
  
  Wc.write_idx = 0;
  Wc.read_idx = 1;

  // 4. Configure CSI controller
  esp_cam_ctlr_csi_config_t csi_config = {
    .ctlr_id = 0,
    .h_res = Wc.config.width,
    .v_res = Wc.config.height,
    .data_lane_num = Wc.config.lane_num,
    .lane_bit_rate_mbps = (int)Wc.config.mipi_clock,
    .input_data_color_type = (cam_ctlr_color_t)COLOR_TYPE_ID(COLOR_SPACE_RAW, (color_pixel_raw_format_t)Wc.config.format),
    .output_data_color_type = CAM_CTLR_COLOR_YUV422,
    .queue_items = 1,
    .byte_swap_en = false,
  };

  ret = esp_cam_new_csi_ctlr(&csi_config, &Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CSI controller init failed (0x%x)"), ret);
    return 0;
  }

  // 5. Register Callbacks
  esp_cam_ctlr_evt_cbs_t cbs = {
    .on_get_new_trans = csi_on_get_new_vb,
    .on_trans_finished = csi_on_trans_finished,
  };
  
  // Start with buffer 0
  Wc.cam_trans.buffer = Wc.frame_buffer[0];
  Wc.cam_trans.buflen = Wc.frame_buffer_size;

  ret = esp_cam_ctlr_register_event_callbacks(Wc.cam_handle, &cbs, &Wc.cam_trans);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to register callbacks (0x%x)"), ret);
    esp_cam_ctlr_del(Wc.cam_handle);
    return 0;
  }

  // 6. Enable and Start CSI
  ret = esp_cam_ctlr_enable(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to enable CSI (0x%x)"), ret);
    esp_cam_ctlr_del(Wc.cam_handle);
    return 0;
  }

  ret = esp_cam_ctlr_start(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to start CSI (0x%x)"), ret);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI controller started"));

  // 7. Configure ISP
  extern isp_proc_handle_t tasmota_wc_isp_handle;
  if (tasmota_wc_isp_handle != nullptr) {
    Wc.isp_handle = tasmota_wc_isp_handle;
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Using Berry ISP handle"));
    
    ret = esp_isp_enable(Wc.isp_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to enable Berry ISP (0x%x)"), ret);
      return 0;
    }
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: ISP enabled"));
  } else {
    // Fallback: Create ISP until Berry ISP driver is ready
    esp_isp_processor_cfg_t isp_config = {
      .clk_hz = 120 * 1000 * 1000, //TODO: eventually calculate this based on configured AV session
      .input_data_source = ISP_INPUT_DATA_SOURCE_CSI,
      .input_data_color_type = (isp_color_t)COLOR_TYPE_ID(COLOR_SPACE_RAW, (color_pixel_raw_format_t)Wc.config.format),
      .output_data_color_type = ISP_COLOR_YUV422,
      .h_res = Wc.config.width,
      .v_res = Wc.config.height,
    };

    ret = esp_isp_new_processor(&isp_config, &Wc.isp_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP init failed (0x%x)"), ret);
      esp_cam_ctlr_stop(Wc.cam_handle);
      esp_cam_ctlr_disable(Wc.cam_handle);
      return 0;
    }

    // esp_isp_bf_config_t bf_config = {
    //   .input_data_color_type = ISP_COLOR_RAW8,
    //   .pattern = ISP_BAYER_PATTERN_BGGR, 
    // };
    // esp_isp_bf_configure(Wc.isp_handle, &bf_config);
    
    ret = esp_isp_enable(Wc.isp_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to enable ISP (0x%x)"), ret);
      esp_isp_del_processor(Wc.isp_handle);
      esp_cam_ctlr_stop(Wc.cam_handle);
      return 0;
    }
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: ISP enabled"));
  }

  // 8. Initialize JPEG encoder engine (100ms Timeout)
  jpeg_encode_engine_cfg_t jpeg_eng_cfg = {
    .intr_priority = 0,
    .timeout_ms = 100,
  };
  
  ret = jpeg_new_encoder_engine(&jpeg_eng_cfg, &Wc.jpeg_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: JPEG encoder init failed (0x%x)"), ret);
    return 0;
  }
  
  // Allocate JPEG output buffer
  jpeg_encode_memory_alloc_cfg_t jpeg_mem_cfg = {
    .buffer_direction = JPEG_ENC_ALLOC_OUTPUT_BUFFER,
  };
  
  size_t actual_size = 0;
  Wc.jpeg_buffer = jpeg_alloc_encoder_mem(Wc.config.width * Wc.config.height / 2, &jpeg_mem_cfg, &actual_size);
  if (!Wc.jpeg_buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate JPEG buffer"));
    jpeg_del_encoder_engine(Wc.jpeg_handle);
    return 0;
  }
  Wc.jpeg_buffer_size = actual_size;

  Wc.jpeg_cfg = {
    .height = Wc.config.height,
    .width = Wc.config.width,
    .src_type = JPEG_ENCODE_IN_FORMAT_YUV422, // from ISP
    .sub_sample = JPEG_DOWN_SAMPLING_YUV422,
    .image_quality = 70,
  };
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: JPEG encoder initialized, buffer=%d bytes"), actual_size);

  // 9. Create mutexes for thread safety
  Wc.client_mutex = xSemaphoreCreateMutex();
  Wc.jpeg_mutex = xSemaphoreCreateMutex();
  
  if (!Wc.client_mutex || !Wc.jpeg_mutex) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to create mutexes"));
    // Cleanup
    if (Wc.client_mutex) vSemaphoreDelete(Wc.client_mutex);
    if (Wc.jpeg_mutex) vSemaphoreDelete(Wc.jpeg_mutex);
    jpeg_del_encoder_engine(Wc.jpeg_handle);
    esp_isp_disable(Wc.isp_handle);
    esp_isp_del_processor(Wc.isp_handle);
    esp_cam_ctlr_stop(Wc.cam_handle);
    esp_cam_ctlr_disable(Wc.cam_handle);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Mutexes created"));

  // 10. Create camera processing task
  BaseType_t task_created = xTaskCreatePinnedToCore(
    CamProcessingTask,
    "CamTask",
    4096,
    NULL,
    5,
    &Wc.cam_task_handle,
    0  // Pin to Core 0
  );
  
  if (task_created != pdPASS) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to create processing task"));
    // Cleanup
    vSemaphoreDelete(Wc.client_mutex);
    vSemaphoreDelete(Wc.jpeg_mutex);
    jpeg_del_encoder_engine(Wc.jpeg_handle);
    esp_isp_disable(Wc.isp_handle);
    esp_isp_del_processor(Wc.isp_handle);
    esp_cam_ctlr_stop(Wc.cam_handle);
    esp_cam_ctlr_disable(Wc.cam_handle);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Processing task created"));

  Wc.up = 1;
  Wc.streaming = false;

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Setup complete"));
  return Wc.up;
}

uint32_t WcStart(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: WcStart called - up=%d streaming=%d changing=%d"), Wc.up, Wc.streaming, Wc.changing_state);
  
  if (!Wc.up) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CSI not initialized"));
    return 0;
  }

  if (Wc.streaming) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Already streaming"));
    return 1;
  }
  
  // Prevent re-entrance
  if (Wc.changing_state) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: State change in progress, ignoring WcStart"));
    return 0;
  }
  Wc.changing_state = true;

  // Reset statistics
  memset(&WcStats, 0, sizeof(WcStats));
  WcStats.start_time = millis();

  // CSI controller is already started during setup
  // Just need to start sensor streaming via Berry
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Calling Berry stream"));
  int32_t berry_result = callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 1, nullptr, 0); // idx=1 (start)
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Berry stream_on result: %d"), berry_result);
  
  if (berry_result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry stream_on failed"));
    Wc.changing_state = false;
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Sensor streaming started"));
  
  // Give sensor time to start streaming
  delay(100);

  Wc.streaming = true;
  Wc.changing_state = false;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Ready - backup buffer should update automatically"));
  return 1;
}

uint32_t WcStop(void) {
  if (!Wc.up || !Wc.streaming) {
    return 0;
  }
  
  // Prevent re-entrance
  if (Wc.changing_state) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: State change in progress, ignoring WcStop"));
    return 0;
  }
  Wc.changing_state = true;

  // Call Berry to stop sensor streaming first
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Calling Berry stream stop"));
  callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 0, nullptr, 0); // idx=0 (stop)
  
  // Give sensor time to stop streaming
  delay(50);

  // Stop CSI controller (stops ISR callbacks)
  esp_err_t ret = esp_cam_ctlr_stop(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to stop CSI (0x%x)"), ret);
  }
  
  // Disable CSI controller BEFORE deleting task (prevents ISR race)
  ret = esp_cam_ctlr_disable(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to disable CSI (0x%x)"), ret);
  }
  
  // Let hardware settle after disable
  delay(100);
  
  // Give task time to exit current iteration (client_ptr stays alive for seamless resolution change)
  delay(50);
  
  // Delete processing task (now safe - ISR disabled, client cleared)
  if (Wc.cam_task_handle) {
    vTaskDelete(Wc.cam_task_handle);
    Wc.cam_task_handle = NULL;
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Processing task deleted"));
  }
  
  // Delete CSI controller
  ret = esp_cam_ctlr_del(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to delete CSI (0x%x)"), ret);
  }
  Wc.cam_handle = NULL;
  
  // Disable ISP
  if (Wc.isp_handle) {
    ret = esp_isp_disable(Wc.isp_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to disable ISP (0x%x)"), ret);
    }
    
    // Delete ISP
    ret = esp_isp_del_processor(Wc.isp_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to delete ISP (0x%x)"), ret);
    }
    Wc.isp_handle = NULL;
  }
  
  // Delete JPEG encoder
  if (Wc.jpeg_handle) {
    ret = jpeg_del_encoder_engine(Wc.jpeg_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to delete JPEG encoder (0x%x)"), ret);
    }
    Wc.jpeg_handle = NULL;
  }
  
  // Free JPEG buffer
  if (Wc.jpeg_buffer) {
    free(Wc.jpeg_buffer);
    Wc.jpeg_buffer = NULL;
    Wc.jpeg_buffer_size = 0;
  }
  
  // Free frame buffers
  for (int i = 0; i < 2; i++) {
    if (Wc.frame_buffer[i]) {
      free(Wc.frame_buffer[i]);
      Wc.frame_buffer[i] = NULL;
    }
  }
  Wc.frame_buffer_size = 0;
  
  // Delete mutexes
  if (Wc.client_mutex) {
    vSemaphoreDelete(Wc.client_mutex);
    Wc.client_mutex = NULL;
  }
  if (Wc.jpeg_mutex) {
    vSemaphoreDelete(Wc.jpeg_mutex);
    Wc.jpeg_mutex = NULL;
  }

  Wc.streaming = false;
  Wc.up = 0;
  Wc.stream_active = 0;  // Signal WcLoop to clean up server
  Wc.changing_state = false;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Streaming stopped"));
  return 1;
}

// Get Frame logic is now internal to HandleWebcamMjpegTask (Zero Copy)
uint8_t* WcGetFrameCSI(uint32_t timeout_ms) {
    // Legacy function support - just returns current read buffer
    if (!Wc.up || !Wc.streaming) return NULL;
    return Wc.frame_buffer[Wc.read_idx];
}


/*********************************************************************************************/
// Command definitions

#define D_PREFX_WEBCAM "Wc"
#define D_CMND_WC_RES "Res"
#define D_CMND_WC_STREAM "Stream"
#define D_CMND_WC_STOP "Stop"
#define D_CMND_WC_STATUS "Status"
#define D_CMND_WC_CONFIG "Config"
#define D_CMND_WC_WINDOW "Window"

const char kWCCommands[] PROGMEM = D_PREFX_WEBCAM "|"  // Prefix
  D_CMND_WC_RES "|" D_CMND_WC_STREAM "|" D_CMND_WC_STOP "|" D_CMND_WC_STATUS "|" D_CMND_WC_CONFIG "|" D_CMND_WC_WINDOW;

void (* const WCCommand[])(void) PROGMEM = {
  &CmndWcRes, &CmndWcStream, &CmndWcStop, &CmndWcStatus, &CmndWcConfig, &CmndWcWindow
};

// Command handlers (mockup/stub implementations)

void CmndWcRes(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcRes called, payload=%d"), XdrvMailbox.payload);
  
  if (XdrvMailbox.payload < 0) {
    ResponseCmndNumber(Wc.config.res_index);
    return;
  }
  
  // Stop streaming if active
  if (Wc.streaming) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopping stream before resolution change"));
    WcStop();
  }
  
  // Mark as uninitialized
  Wc.up = 0;
  
  // Store resolution index
  Wc.config.res_index = (uint8_t)XdrvMailbox.payload;
  
  // Reinitialize with new resolution
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Reinitializing with resolution mode %d"), XdrvMailbox.payload);
  uint32_t result = WcSetup(false);  // Don't reset config - keep resolution index
  
  if (result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Setup failed for resolution %d"), XdrvMailbox.payload);
    ResponseCmndFailed();
  } else {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Resolution changed to mode %d (%dx%d)"), 
      XdrvMailbox.payload, Wc.config.width, Wc.config.height);
    ResponseCmndNumber(XdrvMailbox.payload);
  }
}

void CmndWcStream(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcStream called, payload=%d"), XdrvMailbox.payload);
  // TODO: Implement stream control
  ResponseCmndStateText(Wc.streaming);  // Return current streaming state
}

void CmndWcStop(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcStop called"));
  
  uint32_t result = WcStop();
  
  if (result) {
    ResponseCmndChar_P(PSTR("Stopped"));
  } else {
    ResponseCmndChar_P(PSTR("Already stopped or not initialized"));
  }
}

void CmndWcStatus(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcStatus called"));
  
  Response_P(PSTR("{\"WcStatus\":{\"Streaming\":\"%s\",\"Resolution\":\"%dx%d\","
                  "\"FramesCaptured\":%u,\"FramesProcessed\":%u,\"FramesUnsent\":%u,"
                  "\"JpegErrors\":%u,\"JpegResets\":%u,\"BytesSent\":%u,"
                  "\"UptimeSeconds\":%u,\"FPS\":%u,\"LastFrameTimeMs\":%u}}"),
    Wc.streaming ? "ON" : "OFF",
    Wc.config.width, Wc.config.height,
    WcStats.frames_captured,
    WcStats.frames_processed,
    WcStats.frames_unsent,
    WcStats.jpeg_errors,
    WcStats.jpeg_resets,
    WcStats.bytes_sent,
    WcStats.uptime_seconds,
    WcStats.last_fps,
    WcStats.last_frame_time_ms);
}

void CmndWcConfig(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcConfig called"));
  Response_P(PSTR("{\"WcConfig\":{\"Sensor\":\"%.8s\",\"Width\":%d,\"Height\":%d,\"MaxWidth\":%d,\"MaxHeight\":%d,\"Format\":%d,\"MipiClock\":%d,\"Lanes\":%d,\"OffsetX\":%d,\"OffsetY\":%d,\"Binning\":%d,\"FPS\":%d,\"ResIndex\":%d,\"Flags\":\"0x%02X\"}}"),
    Wc.config.name,
    Wc.config.width, 
    Wc.config.height,
    Wc.config.max_width,
    Wc.config.max_height,
    Wc.config.format, 
    Wc.config.mipi_clock, 
    Wc.config.lane_num,
    Wc.config.offset_x,
    Wc.config.offset_y,
    Wc.config.binning,
    Wc.config.fps,
    Wc.config.res_index,
    Wc.config.flags);
}

void CmndWcWindow(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcWindow called"));
  
  int x = 0, y = 0, w = 0, h = 0, bin = 0, fps = 0;
  int parsed = sscanf(XdrvMailbox.data, "%d,%d,%d,%d,%d,%d", &x, &y, &w, &h, &bin, &fps);
  
  if (parsed != 6) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to parse (got %d, expected 6)"), parsed);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid. Use: x,y,w,h,bin,fps\"}}"));
    return;
  }

  // Validate geometry
  if (w < 16 || h < 16 || w > 2592 || h > 1944) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid geometry %dx%d"), w, h);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid geometry. W/H must be 16-2592/1944\"}}"));
    return;
  }
  
  // Validate binning (1=1x1, 2=2x2, 4=4x4)
  if (bin < 1 || bin > 4 || (bin != 1 && bin != 2 && bin != 4)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid binning %d"), bin);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid binning. Use 1, 2, or 4\"}}"));
    return;
  }
  
  // Validate FPS (1-120 range, default to 30 if 0)
  if (fps < 0 || fps > 120) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid FPS %d"), fps);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid FPS. Use 1-120\"}}"));
    return;
  }
  if (fps == 0) {
    fps = 30;  // Default
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: FPS not specified, using default 30"));
  }

  // Stop current stream
  if (Wc.streaming) {
    WcStop();
  }
  Wc.up = 0;

  // Pre-fill Config for Berry (safe assignment with validation)
  Wc.config.offset_x = (uint16_t)x;
  Wc.config.offset_y = (uint16_t)y;
  Wc.config.width = (uint16_t)w;
  Wc.config.height = (uint16_t)h;
  Wc.config.binning = (uint8_t)bin;
  Wc.config.fps = (uint8_t)fps;
  Wc.config.res_index = 255; // Signal "Custom Mode"

  // Re-Init (Calls Berry 'init')
  if (WcSetup(false)) {
    Response_P(PSTR("{\"WcWindow\":{\"Status\":\"Applied\",\"Width\":%d,\"Height\":%d,\"Binning\":%d,\"FPS\":%d}}"), 
      Wc.config.width, Wc.config.height, Wc.config.binning, Wc.config.fps);
  } else {
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Setup Failed\"}}"));
  }
}


/*********************************************************************************************/

void WcInit(void) {
  memset(&Wc, 0, sizeof(Wc));
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI driver loaded"));

}

/*********************************************************************************************/
// Webcam streaming support
//
// Architecture:
// - Stream server runs on port 81
// - Supports MJPEG streaming via /cam.mjpeg, /stream
// - Supports single frame capture via /wc.jpg, /snapshot.jpg
//
// Endpoints:
// - http://IP:81/           -> redirects to /cam.mjpeg
// - http://IP:81/cam.mjpeg  -> MJPEG stream
// - http://IP:81/stream     -> MJPEG stream
// - http://IP/wc.jpg        -> single frame capture


bool HttpCheckPriviledgedAccess(bool);
extern ESP8266WebServer *Webserver;

void HandleWebcamRoot(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Root called - CamServer=%p"), Wc.CamServer);
  if (!Wc.CamServer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CamServer is NULL in Root!"));
    return;
  }
  Wc.CamServer->sendHeader("Location", "/cam.mjpeg");
  Wc.CamServer->send(302, "", "");
}

void HandleWebcamMjpeg(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Handle camserver - up=%d streaming=%d"), Wc.up, Wc.streaming);
  
  if (!Wc.CamServer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CamServer is NULL!"));
    return;
  }
  
  if (!Wc.up || !Wc.streaming) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Not ready - rejecting stream request"));
    Wc.CamServer->send(503, "text/plain", "Camera not ready");
    return;
  }
  
  // Lock client access
  if (xSemaphoreTake(Wc.client_mutex, pdMS_TO_TICKS(200)) == pdTRUE) {
    // Allocate client on heap to avoid stack issues
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Allocating client..."));
    if (Wc.client_ptr) {
      delete Wc.client_ptr;
      Wc.client_ptr = nullptr;
    }
    
    Wc.client_ptr = new WiFiClient();
    if (!Wc.client_ptr) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate client!"));
      xSemaphoreGive(Wc.client_mutex);
      return;
    }
    
    *Wc.client_ptr = Wc.CamServer->client();
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client allocated, connected=%d"), Wc.client_ptr->connected());
    
    // Send HTTP header
    Wc.client_ptr->print("HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n\r\n");
    Wc.stream_active = 2;
    
    xSemaphoreGive(Wc.client_mutex);
  }
}

void HandleImage(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  WiFiClient client = Webserver->client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.jpg\r\n";
  response += "Content-type: image/jpeg\r\n\r\n";
  Webserver->sendContent(response);

  if (!Wc.jpeg_handle || !Wc.jpeg_buffer) {
    client.stop();
    return;
  }

  // Wait for next frame (simple delay to let task process)
  delay(100);
  
  // Lock JPEG encoder to prevent race with CamProcessingTask
  if (xSemaphoreTake(Wc.jpeg_mutex, pdMS_TO_TICKS(200)) == pdTRUE) {
    uint8_t *source_buf = Wc.frame_buffer[Wc.read_idx];
    esp_cache_msync(source_buf, Wc.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);

    uint32_t jpeg_size = 0;
    esp_err_t ret = jpeg_encoder_process(Wc.jpeg_handle, &Wc.jpeg_cfg, source_buf, Wc.frame_buffer_size, (uint8_t*)Wc.jpeg_buffer, Wc.jpeg_buffer_size, &jpeg_size);
    if (ret == ESP_OK && jpeg_size > 0) {
      client.write((char *)Wc.jpeg_buffer, jpeg_size);
    }
    
    xSemaphoreGive(Wc.jpeg_mutex);
  }
  
  client.stop();
}

uint32_t WcSetStreamserver(uint32_t flag) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: WcSetStreamserver flag=%d CamServer=%p"), flag, Wc.CamServer);
  
  if (TasmotaGlobal.global_state.network_down) { 
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Network down, aborting"));
    Wc.stream_active = 0;
    return 0; 
  }

  if (flag) {
    if (!Wc.CamServer) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: Creating stream server on port 81..."));
      Wc.stream_active = 0;
      Wc.CamServer = new ESP8266WebServer(81);
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: CamServer created at %p"), Wc.CamServer);
      
      if (!Wc.CamServer) {
        AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate CamServer!"));
        return 0;
      }
      
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Registering handlers..."));
      Wc.CamServer->on("/", HandleWebcamRoot);
      Wc.CamServer->on("/cam.mjpeg", HandleWebcamMjpeg);
      Wc.CamServer->on("/cam.jpg", HandleWebcamMjpeg);
      Wc.CamServer->on("/stream", HandleWebcamMjpeg);
      
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Starting server..."));
      Wc.CamServer->begin();
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stream server started on port 81"));
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stream server already running"));
    }
  } else {
    if (Wc.CamServer) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopping stream server..."));
      Wc.stream_active = 0;
      Wc.CamServer->stop();
      delete Wc.CamServer;
      Wc.CamServer = NULL;
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stream server stopped"));
    }
  }
  return 0;
}

void WcPicSetup(void) {
  WebServer_on(PSTR("/wc.jpg"), HandleImage);
  WebServer_on(PSTR("/wc.mjpeg"), HandleImage);
  WebServer_on(PSTR("/snapshot.jpg"), HandleImage);
}

void WcShowStream(void) {
  if (Wc.CamServer && Wc.up) {
    WSContentSend_P(PSTR("<p></p><center><img onerror='setTimeout(()=>{this.src=this.src;},1000)' src='http://%_I:81/stream' alt='Webcam stream''></center><p></p>"),(uint32_t)WiFi.localIP());
  }
}

// TEST: Read buffer 0 directly
void WcTestReceive(void) {
  static uint32_t test_count = 0;
  static uint8_t last_frame[8] = {0};
  
  if (!Wc.up || !Wc.streaming) {
    return;
  }
  
  // Test every 50 loop iterations (~1 second at 50fps)
  test_count++;
  if (test_count < 50) {
    return;
  }
  test_count = 0;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: === TEST: Checking buffers ==="));
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Callbacks: get_new=%d finished=%d"), cb_get_new_count, cb_finished_count);
  
  // Check our first allocated frame buffer
  if (Wc.frame_buffer[0]) {
    esp_cache_msync(Wc.frame_buffer[0], 64, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
    uint8_t *buf = (uint8_t*)Wc.frame_buffer[0];
    
    bool changed = false;
    for (int i = 0; i < 8; i++) {
      if (buf[i] != last_frame[i]) {
        changed = true;
        break;
      }
    }
    
    if (changed) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: FRAME buffer[0] CHANGED: %02X %02X %02X %02X %02X %02X %02X %02X"), 
        buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
      memcpy(last_frame, buf, 8);
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: FRAME buffer[0] static: %02X %02X %02X %02X %02X %02X %02X %02X"), 
        buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
    }
  }
}

void WcLoop(void) {
  if (Wc.streaming && !Wc.CamServer && !TasmotaGlobal.global_state.network_down) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Starting stream server..."));
    WcSetStreamserver(1);
  }
  
  if (Wc.CamServer) {
    Wc.CamServer->handleClient();
    
    // Monitor client connection - cleanup if disconnected (with mutex protection)
    if (Wc.stream_active && xSemaphoreTake(Wc.client_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      if (Wc.client_ptr && !Wc.client_ptr->connected()) {
        delete Wc.client_ptr;
        Wc.client_ptr = nullptr;
        Wc.stream_active = 0;
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client disconnected"));
      }
      xSemaphoreGive(Wc.client_mutex);
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv81(uint32_t function) {
  bool result = false;
  switch (function) {
    case FUNC_LOOP:
      WcLoop();
      break;
    case FUNC_WEB_ADD_HANDLER:
      WcPicSetup();
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
      WcShowStream();
      break;
    case FUNC_PRE_INIT:
      WcInit();
      break;
    case FUNC_INIT:
      if(Wc.up == 0) {
        WcSetup(true);  // First boot - reset config to defaults
      }
      break;
    case FUNC_EVERY_SECOND:
      // Auto-start streaming once WiFi is available
      if (Wc.up && !Wc.streaming && !TasmotaGlobal.global_state.network_down) {
        WcStart();
      }
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kWCCommands, WCCommand);
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif  // USE_WEBCAM_CSI
#endif  // ESP32P4
