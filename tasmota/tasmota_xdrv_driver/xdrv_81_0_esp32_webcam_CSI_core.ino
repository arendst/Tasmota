/*
  xdrv_81_0_esp32_webcam_CSI_core.ino - ESP32-P4 CSI webcam support for Tasmota

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

// H.264 encoder for RTP session (ESP32-P4 hardware)
extern "C" {
#include "esp_h264_enc_single_hw.h"
#include "esp_h264_alloc.h"
}

// UDP for RTP transport
#include <WiFiUdp.h>

/*********************************************************************************************/

// Session type - what kind of output we're producing
typedef enum {
  SESSION_NONE = 0,
  SESSION_MJPEG_HTTP,
  SESSION_RTSP,        // H.264 over RTP/UDP with RTSP control
  SESSION_WEBRTC,      // H.264 over SRTP (encrypted RTP + signaling)
  SESSION_DSI_DISPLAY
} camera_session_t;

// Pipeline lifecycle state
typedef enum {
  CAM_IDLE = 0,        // Nothing initialized
  CAM_INIT,            // CSI created & enabled, not started
  CAM_STREAMING,       // CSI started, ISR active, task running
  CAM_PAUSING,         // Requesting task to pause
  CAM_PAUSED,          // Task paused, safe to reconfigure
  CAM_STOPPING         // Full shutdown in progress, reject all work
} camera_state_t;

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
  // --- 1. Core Camera State (POD) ---
  struct {
    esp_cam_ctlr_handle_t cam_handle;
    isp_proc_handle_t isp_handle;
    esp_ldo_channel_handle_t ldo_mipi_phy;
    uint8_t *frame_buffer[2];
    size_t frame_buffer_size;
    CSI_Config config;
    esp_cam_ctlr_trans_t cam_trans;
    volatile int write_idx;
    volatile int read_idx;
    volatile camera_state_t state;
    camera_session_t session_type;
    TaskHandle_t cam_task_handle;
    SemaphoreHandle_t frame_mutex;
    SemaphoreHandle_t resume_sem;
  } core;
  
  // --- 2. JPEG Session (POD) ---
  struct {
    jpeg_encoder_handle_t handle;
    void *buffer;
    size_t buffer_size;
    jpeg_encode_cfg_t cfg;
    uint8_t quality;
    SemaphoreHandle_t mutex;
    ESP8266WebServer *server;
    WiFiClient *client_ptr;
    uint8_t stream_active;
  } jpeg;
  
  // --- 3. H.264 Session (POD) ---
  struct {
    esp_h264_enc_handle_t handle;
    uint8_t *buffer;
    size_t buffer_size;
    uint8_t *out_buffer;
    size_t out_buffer_size;
    uint32_t motion_val;      // Current rolling average (0-100+)
    uint32_t motion_raw;      // Raw P-frame size (debug)
  } h264;
  
  // --- 4. RTP Protocol (POD) ---
  struct {
    uint16_t dest_port;
    uint16_t sequence;
    uint32_t timestamp;
    uint32_t ssrc;
  } rtp;
  
  // --- 5. RTSP Protocol (POD) ---
  struct {
    WiFiServer *server;
    uint32_t session_id;
    uint16_t client_rtp_port;
    uint16_t client_rtcp_port;
    bool streaming;
  } rtsp;
  
  // --- 6. C++ Objects (Complex types - NO MEMSET) ---
  IPAddress rtp_dest_ip;
  WiFiUDP rtp_udp;
  WiFiClient rtsp_client;
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
  
  // Detailed timing breakdown (in microseconds for precision)
  uint32_t last_mutex_wait_us;      // Time waiting for frame_mutex
  uint32_t last_cache_sync_us;      // Cache sync duration
  uint32_t last_jpeg_encode_us;     // JPEG encoding duration
  uint32_t last_network_write_us;   // Network transmission duration
  uint32_t last_jpeg_mutex_wait_us; // Time waiting for jpeg_mutex
  
  // Averages over last second
  uint32_t avg_mutex_wait_us;
  uint32_t avg_cache_sync_us;
  uint32_t avg_jpeg_encode_us;
  uint32_t avg_network_write_us;
  
  // Max values (to catch spikes)
  uint32_t max_mutex_wait_us;
  uint32_t max_jpeg_encode_us;
  uint32_t max_network_write_us;
  
  // JPEG size tracking
  uint32_t last_jpeg_size;        // Last JPEG size in bytes
  uint32_t avg_jpeg_size;         // Average JPEG size over last second
  uint32_t min_jpeg_size;         // Minimum JPEG size seen
  uint32_t max_jpeg_size;         // Maximum JPEG size seen
  uint32_t compression_ratio_x100; // Compression ratio * 100 (e.g., 1500 = 15.00x)
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
  int next_idx = (Wc.core.write_idx + 1) % 2;
  Wc.core.write_idx = next_idx;
  
  // Give hardware the address of the new write buffer
  trans->buffer = Wc.core.frame_buffer[next_idx];
  trans->buflen = Wc.core.frame_buffer_size;
  
  return false; 
}

// Callback: Frame transfer finished - Wake processing task
static bool IRAM_ATTR csi_on_trans_finished(esp_cam_ctlr_handle_t handle, esp_cam_ctlr_trans_t *trans, void *user_data) {
  // Reject if not streaming (shutdown in progress)
  if (Wc.core.state != CAM_STREAMING) return false;
  
  cb_finished_count++;
  
  // The buffer we just finished writing is now the readable one
  Wc.core.read_idx = Wc.core.write_idx;
  
  // Statistics: increment frames captured
  WcStats.frames_captured++;
  
  // Wake up the processing task immediately
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(Wc.core.cam_task_handle, &xHigherPriorityTaskWoken);
  
  return xHigherPriorityTaskWoken == pdTRUE;
}


// Initialize the resolution-dependent hardware
// Returns 1 on success, 0 on failure
uint32_t WcInitPipeline() {
  esp_err_t ret;

  // 1. Allocate Frame Buffers
  Wc.core.frame_buffer_size = Wc.core.config.width * Wc.core.config.height * 2;
  for (int i = 0; i < 2; i++) {
    Wc.core.frame_buffer[i] = (uint8_t*)heap_caps_aligned_calloc(64, 1, Wc.core.frame_buffer_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!Wc.core.frame_buffer[i]) return 0;
    esp_cache_msync(Wc.core.frame_buffer[i], Wc.core.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_C2M);
  }
  Wc.core.write_idx = 0;
  Wc.core.read_idx = 1;

  // 2. Configure CSI
  cam_ctlr_color_t csi_output_format = (Wc.core.session_type == SESSION_RTSP) ? CAM_CTLR_COLOR_YUV420 : CAM_CTLR_COLOR_YUV422;
  
  esp_cam_ctlr_csi_config_t csi_config = {
    .ctlr_id = 0,
    .h_res = Wc.core.config.width,
    .v_res = Wc.core.config.height,
    .data_lane_num = Wc.core.config.lane_num,
    .lane_bit_rate_mbps = (int)Wc.core.config.mipi_clock,
    .input_data_color_type = (cam_ctlr_color_t)COLOR_TYPE_ID(COLOR_SPACE_RAW, (color_pixel_raw_format_t)Wc.core.config.format),
    .output_data_color_type = csi_output_format,
    .queue_items = 1,
    .byte_swap_en = false,
  };
  if (esp_cam_new_csi_ctlr(&csi_config, &Wc.core.cam_handle) != ESP_OK) return 0;

  // 3. Callbacks
  esp_cam_ctlr_evt_cbs_t cbs = { .on_get_new_trans = csi_on_get_new_vb, .on_trans_finished = csi_on_trans_finished };
  Wc.core.cam_trans.buffer = Wc.core.frame_buffer[0];
  Wc.core.cam_trans.buflen = Wc.core.frame_buffer_size;
  if (esp_cam_ctlr_register_event_callbacks(Wc.core.cam_handle, &cbs, &Wc.core.cam_trans) != ESP_OK) return 0;
  if (esp_cam_ctlr_enable(Wc.core.cam_handle) != ESP_OK) return 0;

  // 4. ISP
  if (tasmota_wc_isp_handle) {
    Wc.core.isp_handle = tasmota_wc_isp_handle;
    esp_isp_enable(Wc.core.isp_handle);
  } else {
    isp_color_t isp_output_format = (Wc.core.session_type == SESSION_RTSP) ? ISP_COLOR_YUV420 : ISP_COLOR_YUV422;
    esp_isp_processor_cfg_t isp_config = {
      .clk_hz = 120 * 1000 * 1000,
      .input_data_source = ISP_INPUT_DATA_SOURCE_CSI,
      .input_data_color_type = (isp_color_t)COLOR_TYPE_ID(COLOR_SPACE_RAW, (color_pixel_raw_format_t)Wc.core.config.format),
      .output_data_color_type = isp_output_format,
      .h_res = Wc.core.config.width,
      .v_res = Wc.core.config.height,
    };
    if (esp_isp_new_processor(&isp_config, &Wc.core.isp_handle) != ESP_OK) return 0;
    esp_isp_enable(Wc.core.isp_handle);
  }

  // 5. Encoders
  if (Wc.core.session_type == SESSION_MJPEG_HTTP) {
    if (!WcSetupJpegEncoder()) return 0;
  } else if (Wc.core.session_type == SESSION_RTSP) {
    if (!WcSetupH264Encoder()) return 0;
  }

  return 1;
}


// De-initialize only the resolution-dependent hardware
void WcDeinitPipeline() {
  // 1. Delete Encoder
  if (Wc.h264.handle) {
    esp_h264_enc_del(Wc.h264.handle); // Only if using helper that doesn't double-free
    Wc.h264.handle = NULL;
  }
  if (Wc.h264.buffer) { esp_h264_free(Wc.h264.buffer); Wc.h264.buffer = NULL; }
  if (Wc.h264.out_buffer) { esp_h264_free(Wc.h264.out_buffer); Wc.h264.out_buffer = NULL; }

  if (Wc.jpeg.handle) {
    jpeg_del_encoder_engine(Wc.jpeg.handle);
    Wc.jpeg.handle = NULL;
  }
  if (Wc.jpeg.buffer) { free(Wc.jpeg.buffer); Wc.jpeg.buffer = NULL; }

  // 2. Stop & Delete CSI
  if (Wc.core.cam_handle) {
    esp_cam_ctlr_stop(Wc.core.cam_handle);
    esp_cam_ctlr_disable(Wc.core.cam_handle);
    esp_cam_ctlr_del(Wc.core.cam_handle);
    Wc.core.cam_handle = NULL;
  }

  // 3. Delete ISP (Only if we own it)
  if (Wc.core.isp_handle) {
    esp_isp_disable(Wc.core.isp_handle);
    if (!tasmota_wc_isp_handle) {
      esp_isp_del_processor(Wc.core.isp_handle);
    }
    Wc.core.isp_handle = NULL;
  }

  // 4. Free Frame Buffers
  for (int i = 0; i < 2; i++) {
    if (Wc.core.frame_buffer[i]) {
      free(Wc.core.frame_buffer[i]);
      Wc.core.frame_buffer[i] = NULL;
    }
  }
}

uint32_t WcSetup(bool reset_config) {
  if (Wc.core.state != CAM_IDLE) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI already initialized (state=%d)"), Wc.core.state);
    return 1;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== SETUP START (Double-Buffered) ====="));

  // 1. Initialize MIPI PHY LDO
  esp_ldo_channel_config_t ldo_mipi_phy_config = {
    .chan_id = 3,        // LDO_VO3 for MIPI PHY
    .voltage_mv = 2500,  // 2.5V for MIPI PHY
  };
  
  esp_err_t ret = esp_ldo_acquire_channel(&ldo_mipi_phy_config, &Wc.core.ldo_mipi_phy);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to acquire MIPI LDO (0x%x)"), ret);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: MIPI PHY LDO enabled"));

  // PRE-FILL CONFIG WITH DEFAULTS (only on first boot, not on resolution change)
  if (reset_config) {
    memset(&Wc.core.config, 0, sizeof(Wc.core.config));
    Wc.core.config.width = 640;
    Wc.core.config.height = 480;
    Wc.core.config.lane_num = 2;
    Wc.core.config.mipi_clock = 200;
    Wc.core.config.fps = 1; // Default to 1 to avoid div-by-zero later
  }

  // 2. Call Berry to initialize sensor (zero-copy: pass struct address as idx)
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== CALLING BERRY INIT ====="));
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Config buffer addr=0x%08X size=%d bytes"), (uint32_t)&Wc.core.config, sizeof(CSI_Config));
  
  uint32_t config_addr = (uint32_t)&Wc.core.config;
  int32_t result = callBerryEventDispatcher(PSTR("camera"), PSTR("init"), config_addr, nullptr, 0);
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== BERRY INIT RESULT=%d ====="), result);
  
  if (result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry init failed or no driver loaded"));
    return 0;
  }
  
  // Log raw bytes for debugging
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Raw bytes [0-7]: %02X %02X %02X %02X %02X %02X %02X %02X"),
    ((uint8_t*)&Wc.core.config)[0], ((uint8_t*)&Wc.core.config)[1], ((uint8_t*)&Wc.core.config)[2], ((uint8_t*)&Wc.core.config)[3],
    ((uint8_t*)&Wc.core.config)[4], ((uint8_t*)&Wc.core.config)[5], ((uint8_t*)&Wc.core.config)[6], ((uint8_t*)&Wc.core.config)[7]);
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== CONFIG FROM BERRY ====="));
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Sensor Name: %.8s"), Wc.core.config.name);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Resolution: %dx%d"), Wc.core.config.width, Wc.core.config.height);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Format: %d"), Wc.core.config.format);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: MIPI Clock: %d Mbps/lane"), Wc.core.config.mipi_clock);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Lanes: %d"), Wc.core.config.lane_num);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== END CONFIG ====="));

  // 3. Initialize Resolution-Dependent Hardware (Buffers, CSI, ISP, Encoders)
  if (!WcInitPipeline()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Pipeline Init Failed"));
    // WcInitPipeline calls Deinit internally on failure, so we are safe
    return 0;
  }

  // 4. Create mutexes for thread safety
  Wc.core.frame_mutex = xSemaphoreCreateMutex();
  Wc.jpeg.mutex = xSemaphoreCreateMutex();
  Wc.core.resume_sem = xSemaphoreCreateBinary();  // For pause/resume
  
  if (!Wc.core.frame_mutex || !Wc.jpeg.mutex || !Wc.core.resume_sem) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to create mutexes"));
    WcDeinitPipeline(); // Cleanup hardware
    // Cleanup mutexes
    if (Wc.core.frame_mutex) vSemaphoreDelete(Wc.core.frame_mutex);
    if (Wc.jpeg.mutex) vSemaphoreDelete(Wc.jpeg.mutex);
    if (Wc.core.resume_sem) vSemaphoreDelete(Wc.core.resume_sem);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Mutexes created"));

  // 5. Create processing task based on session type
  TaskFunction_t task_func;
  const char *task_name;
  
  if (Wc.core.session_type == SESSION_MJPEG_HTTP) {
    task_func = MjpegProcessingTask;
    task_name = "MjpegTask";
  } else {
    task_func = H264ProcessingTask;
    task_name = "H264Task";
  }
  
  BaseType_t task_created = xTaskCreatePinnedToCore(
    task_func,
    task_name,
    8192, // 8KB Stack
    NULL,
    5,
    &Wc.core.cam_task_handle,
    1  // Pin to Core 1
  );
  
  if (task_created != pdPASS) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to create processing task"));
    WcDeinitPipeline();
    vSemaphoreDelete(Wc.core.frame_mutex);
    vSemaphoreDelete(Wc.jpeg.mutex);
    vSemaphoreDelete(Wc.core.resume_sem);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Processing task created"));

  Wc.core.state = CAM_INIT;
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Setup complete (session=%d)"), Wc.core.session_type);
  return 1;
}

uint32_t WcStart(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: WcStart called - state=%d"), Wc.core.state);
  
  if (Wc.core.state != CAM_INIT) {
    if (Wc.core.state == CAM_STREAMING) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Already streaming"));
      return 1;
    }
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Cannot start from state %d"), Wc.core.state);
    return 0;
  }

  // Reset statistics
  memset(&WcStats, 0, sizeof(WcStats));
  WcStats.start_time = millis();

  // Start CSI controller (ISR callbacks will begin firing)
  esp_err_t ret = esp_cam_ctlr_start(Wc.core.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to start CSI (0x%x)"), ret);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI controller started"));

  // Start sensor streaming via Berry
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Calling Berry stream"));
  int32_t berry_result = callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 1, nullptr, 0); // idx=1 (start)
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Berry stream_on result: %d"), berry_result);
  
  if (berry_result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry stream_on failed"));
    esp_cam_ctlr_stop(Wc.core.cam_handle);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Sensor streaming started"));
  
  // Give sensor time to start streaming
  delay(100);

  Wc.core.state = CAM_STREAMING;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Streaming active"));
  return 1;
}

// Pause the task for reconfiguration - task stays alive but stops processing
uint32_t WcPause(void) {
  if (Wc.core.state != CAM_STREAMING) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Cannot pause from state %d"), Wc.core.state);
    return 0;
  }
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Pausing task"));
  
  // Signal task to pause
  Wc.core.state = CAM_PAUSING;
  
  // Wake task so it sees the state change
  xTaskNotifyGive(Wc.core.cam_task_handle);
  
  // Wait for task to acknowledge pause (max 500ms)
  for (int i = 0; i < 50 && Wc.core.state != CAM_PAUSED; i++) {
    delay(10);
  }
  
  if (Wc.core.state != CAM_PAUSED) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Task didn't pause"));
    return 0;
  }
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Task paused"));
  return 1;
}


uint32_t WcStop(void) {
  if (Wc.core.state == CAM_IDLE || Wc.core.state == CAM_STOPPING) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Nothing to stop (state=%d)"), Wc.core.state);
    return 0;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopping (state=%d)"), Wc.core.state);
  
  // 1. Set state to STOPPING - task will exit on next iteration
  Wc.core.state = CAM_STOPPING;
  
  // 2. If task was paused, release it so it can see STOPPING and exit
  if (Wc.core.resume_sem) xSemaphoreGive(Wc.core.resume_sem);
  if (Wc.core.frame_mutex) xSemaphoreGive(Wc.core.frame_mutex);
  
  // 3. Wake task and wait for it to exit
  if (Wc.core.cam_task_handle) {
    xTaskNotifyGive(Wc.core.cam_task_handle);
    
    // Wait for task to exit (task sets handle to NULL before deleting itself)
    for (int i = 0; i < 50 && Wc.core.cam_task_handle != NULL; i++) {
      delay(10);
    }
    
    // Force delete if task didn't exit
    if (Wc.core.cam_task_handle != NULL) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Task didn't exit cleanly, force deleting"));
      vTaskDelete(Wc.core.cam_task_handle);
      Wc.core.cam_task_handle = NULL;
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Task stopped"));
  }

  // 4. Stop sensor streaming via Berry
  callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 0, nullptr, 0);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Called Berry stream stop"));
  
  // 5. Deinitialize hardware pipeline (Buffers, CSI, ISP, Encoders)
  WcDeinitPipeline();
  
  // 6. Clean up network clients
  if (Wc.jpeg.client_ptr) {
    delete Wc.jpeg.client_ptr;
    Wc.jpeg.client_ptr = nullptr;
  }
  
  // 7. Stop RTSP server and close client
  WcRtspStop();
  
  // 8. Delete mutexes
  if (Wc.core.frame_mutex) {
    vSemaphoreDelete(Wc.core.frame_mutex);
    Wc.core.frame_mutex = NULL;
  }
  if (Wc.jpeg.mutex) {
    vSemaphoreDelete(Wc.jpeg.mutex);
    Wc.jpeg.mutex = NULL;
  }
  if (Wc.core.resume_sem) {
    vSemaphoreDelete(Wc.core.resume_sem);
    Wc.core.resume_sem = NULL;
  }

  // 9. Final state
  Wc.core.state = CAM_IDLE;
  Wc.jpeg.stream_active = 0;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopped"));
  return 1;
}

// Change resolution dynamically without dropping the connection (Hot Swap)
// Returns true if successful
bool WcChangeResolution(uint16_t width, uint16_t height) {
  // 1. Validate input
  if (width == 0 || height == 0) return false;
  if (width == Wc.core.config.width && height == Wc.core.config.height) return true; // No change needed

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Changing resolution to %dx%d"), width, height);

  // 2. If Idle, just update config and return
  if (Wc.core.state == CAM_IDLE) {
    Wc.core.config.width = width;
    Wc.core.config.height = height;
    return true;
  }

  // 3. Pause the Pipeline (if running)
  camera_state_t prev_state = Wc.core.state;
  if (prev_state == CAM_STREAMING) {
    Wc.core.state = CAM_PAUSING;
    
    // Ensure task isn't blocked on resume_sem
    if (Wc.core.resume_sem) xSemaphoreGive(Wc.core.resume_sem);

    // Wait for task to enter PAUSED state (timeout 500ms)
    for (int i = 0; i < 50; i++) {
      if (Wc.core.state == CAM_PAUSED) break;
      delay(10);
    }
    
    if (Wc.core.state != CAM_PAUSED) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to pause for resolution change"));
      Wc.core.state = prev_state; // Try to resume
      return false;
    }
  }

  // 4. Tear down hardware (Keep Task & Client Alive)
  WcDeinitPipeline();

  // 5. Update Configuration
  Wc.core.config.width = width;
  Wc.core.config.height = height;
  
  // 6. Notify Sensor Driver (Berry) to update physical sensor
  // We reuse the existing 'init' command which passes the updated config struct
  uint32_t config_addr = (uint32_t)&Wc.core.config;
  int32_t result = callBerryEventDispatcher(PSTR("camera"), PSTR("init"), config_addr, nullptr, 0);
  if (result == 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry sensor resize failed"));
      // We are in a bad state now (Hardware gone, Sensor failed). 
      // Force full stop.
      WcStop(); 
      return false;
  }

  // 7. Re-Initialize Hardware Pipeline
  if (!WcInitPipeline()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Pipeline re-init failed"));
    WcStop();
    return false;
  }

  // 8. Resume Pipeline
  if (prev_state == CAM_STREAMING) {
    Wc.core.state = CAM_STREAMING;
    // Release the task to start processing again
    if (Wc.core.resume_sem) xSemaphoreGive(Wc.core.resume_sem);
  } else {
    Wc.core.state = CAM_INIT;
  }
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Resolution changed successfully"));
  return true;
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
#define D_CMND_WC_QUALITY "Quality"
#define D_CMND_WC_SESSION "Session"
#define D_CMND_WC_RTP_DEST "RtpDest"

const char kWCCommands[] PROGMEM = D_PREFX_WEBCAM "|"  // Prefix
  D_CMND_WC_RES "|" D_CMND_WC_STREAM "|" D_CMND_WC_STOP "|" D_CMND_WC_STATUS "|" D_CMND_WC_CONFIG "|" D_CMND_WC_WINDOW "|" D_CMND_WC_QUALITY "|" D_CMND_WC_SESSION "|" D_CMND_WC_RTP_DEST;

void (* const WCCommand[])(void) PROGMEM = {
  &CmndWcRes, &CmndWcStream, &CmndWcStop, &CmndWcStatus, &CmndWcConfig, &CmndWcWindow, &CmndWcQuality, &CmndWcSession, &CmndWcRtpDest
};

// Command handlers (mockup/stub implementations)

void CmndWcRes(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcRes called, payload=%d"), XdrvMailbox.payload);
  
  if (XdrvMailbox.payload < 0) {
    ResponseCmndNumber(Wc.core.config.res_index);
    return;
  }
  
  // Reject if busy
  if (Wc.core.state == CAM_STOPPING || Wc.core.state == CAM_PAUSING) {
    ResponseCmndChar_P(PSTR("Busy"));
    return;
  }
  
  // Determine current state
  bool was_streaming = (Wc.core.state == CAM_STREAMING);
  
  // 1. Pause task first if streaming
  if (was_streaming) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Pausing for resolution change"));
    Wc.core.state = CAM_PAUSING;
    
    if (Wc.core.resume_sem) xSemaphoreGive(Wc.core.resume_sem);

    for (int i = 0; i < 50; i++) {
      if (Wc.core.state == CAM_PAUSED) break;
      delay(10);
    }
    
    if (Wc.core.state != CAM_PAUSED) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to pause"));
      ResponseCmndChar_P(PSTR("Pause failed"));
      return;
    }
  }
  
  // 2. Stop CSI and sensor streaming
  if (Wc.core.cam_handle) {
    esp_cam_ctlr_stop(Wc.core.cam_handle);
  }
  callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 0, nullptr, 0);
  
  // 3. Teardown hardware
  WcDeinitPipeline();
  
  // 4. Update Config
  Wc.core.config.res_index = (uint8_t)XdrvMailbox.payload;
  
  // 5. Notify Sensor (Berry)
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Reinitializing sensor with mode %d"), XdrvMailbox.payload);
  uint32_t config_addr = (uint32_t)&Wc.core.config;
  int32_t result = callBerryEventDispatcher(PSTR("camera"), PSTR("init"), config_addr, nullptr, 0);
  
  if (result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry init failed"));
    WcStop();
    ResponseCmndFailed();
    return;
  }
  
  // 6. Re-Initialize Hardware Pipeline
  if (!WcInitPipeline()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Pipeline Init Failed"));
    WcStop();
    ResponseCmndFailed();
    return;
  }
  
  // 7. Restart CSI and sensor streaming (if we were streaming before)
  if (was_streaming) {
    esp_err_t ret = esp_cam_ctlr_start(Wc.core.cam_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to restart CSI (0x%x)"), ret);
      WcStop();
      ResponseCmndFailed();
      return;
    }
    
    int32_t berry_result = callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 1, nullptr, 0);
    if (berry_result == 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry stream_on failed"));
      WcStop();
      ResponseCmndFailed();
      return;
    }
    
    delay(100); // Give sensor time to start
    
    // Resume task
    Wc.core.state = CAM_STREAMING;
    if (Wc.core.resume_sem) xSemaphoreGive(Wc.core.resume_sem);
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Resumed streaming"));
  } else {
    Wc.core.state = CAM_INIT;
  }
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Resolution changed to mode %d (%dx%d)"), 
      XdrvMailbox.payload, Wc.core.config.width, Wc.core.config.height);
  ResponseCmndNumber(XdrvMailbox.payload);
}

void CmndWcWindow(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcWindow called"));
  
  int x = 0, y = 0, w = 0, h = 0, bin = 0, fps = 0, format = 0;
  int parsed = sscanf(XdrvMailbox.data, "%d,%d,%d,%d,%d,%d,%d", &x, &y, &w, &h, &bin, &fps, &format);
  
  if (parsed != 7) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to parse (got %d, expected 7)"), parsed);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid. Use: x,y,w,h,bin,fps,format\"}}"));
    return;
  }

  // Validate geometry
  if (w < 16 || h < 16 || w > Wc.core.config.max_width || h > Wc.core.config.max_height) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid geometry %dx%d"), w, h);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid geometry. W/H must be 16-2592/1944\"}}"));
    return;
  }
  
  // Validate binning
  if (bin < 1 || bin > 2) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid binning %d"), bin);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid binning. Use 1 or 2\"}}"));
    return;
  }
  
  // Validate FPS
  if (fps < 0 || fps > 120) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid FPS %d"), fps);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid FPS. Use 1-120\"}}"));
    return;
  }
  if (fps == 0) fps = 30;
  
  // Validate format
  if (format < 0 || format > 2) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid format %d"), format);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid format. Use 0=RAW8, 1=RAW10, 2=RAW12\"}}"));
    return;
  }

  // Reject if busy
  if (Wc.core.state == CAM_STOPPING || Wc.core.state == CAM_PAUSING) {
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Busy\"}}"));
    return;
  }

  bool was_streaming = (Wc.core.state == CAM_STREAMING);

  // 1. Pause task
  if (was_streaming) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Pausing for window change"));
    Wc.core.state = CAM_PAUSING;
    
    if (Wc.core.resume_sem) xSemaphoreGive(Wc.core.resume_sem);

    for (int i = 0; i < 50; i++) {
      if (Wc.core.state == CAM_PAUSED) break;
      delay(10);
    }
    
    if (Wc.core.state != CAM_PAUSED) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to pause"));
      Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Pause Failed\"}}"));
      return;
    }
  }

  // 2. Stop CSI and sensor streaming
  if (Wc.core.cam_handle) {
    esp_cam_ctlr_stop(Wc.core.cam_handle);
  }
  callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 0, nullptr, 0);

  // 3. Teardown hardware
  WcDeinitPipeline();

  // 4. Update Config
  Wc.core.config.offset_x = (uint16_t)x;
  Wc.core.config.offset_y = (uint16_t)y;
  Wc.core.config.width = (uint16_t)w;
  Wc.core.config.height = (uint16_t)h;
  Wc.core.config.binning = (uint8_t)bin;
  Wc.core.config.fps = (uint8_t)fps;
  Wc.core.config.format = (uint8_t)format;
  Wc.core.config.res_index = 255;

  // 5. Notify Sensor
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Reinitializing sensor with custom window"));
  uint32_t config_addr = (uint32_t)&Wc.core.config;
  int32_t result = callBerryEventDispatcher(PSTR("camera"), PSTR("init"), config_addr, nullptr, 0);

  if (result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry init failed"));
    WcStop();
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Sensor Init Failed\"}}"));
    return;
  }

  // 6. Re-Initialize Hardware
  if (!WcInitPipeline()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Pipeline Init Failed"));
    WcStop();
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Pipeline Init Failed\"}}"));
    return;
  }

  // 7. Restart CSI and sensor streaming (if we were streaming before)
  if (was_streaming) {
    esp_err_t ret = esp_cam_ctlr_start(Wc.core.cam_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to restart CSI (0x%x)"), ret);
      WcStop();
      Response_P(PSTR("{\"WcWindow\":{\"Error\":\"CSI Start Failed\"}}"));
      return;
    }
    
    int32_t berry_result = callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 1, nullptr, 0);
    if (berry_result == 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry stream_on failed"));
      WcStop();
      Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Stream Start Failed\"}}"));
      return;
    }
    
    delay(100); // Give sensor time to start
    
    // Resume task
    Wc.core.state = CAM_STREAMING;
    if (Wc.core.resume_sem) xSemaphoreGive(Wc.core.resume_sem);
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Resumed streaming"));
  } else {
    Wc.core.state = CAM_INIT;
  }

  Response_P(PSTR("{\"WcWindow\":{\"Status\":\"Applied\",\"Width\":%d,\"Height\":%d,\"Binning\":%d,\"FPS\":%d,\"Format\":%d}}"), 
    Wc.core.config.width, Wc.core.config.height, Wc.core.config.binning, Wc.core.config.fps, Wc.core.config.format);
}

void CmndWcStream(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcStream called, payload=%d"), XdrvMailbox.payload);
  ResponseCmndStateText(Wc.core.state == CAM_STREAMING);
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
  
  const char* state_names[] = {"IDLE", "INIT", "STREAMING", "PAUSING", "PAUSED", "STOPPING"};
  const char* state_name = (Wc.core.state < 6) ? state_names[Wc.core.state] : "UNKNOWN";
  
  Response_P(PSTR("{\"WcStatus\":{\"State\":\"%s\",\"Resolution\":\"%dx%d\","
                  "\"FramesCaptured\":%u,\"FramesProcessed\":%u,\"FramesUnsent\":%u,"
                  "\"JpegErrors\":%u,\"JpegResets\":%u,\"BytesSent\":%u,"
                  "\"UptimeSeconds\":%u,\"FPS\":%u,\"LastFrameTimeMs\":%u,"
                  "\"Timing\":{"
                    "\"MutexWaitUs\":%u,\"CacheSyncUs\":%u,\"JpegEncodeUs\":%u,\"NetworkWriteUs\":%u,"
                    "\"MaxMutexUs\":%u,\"MaxJpegUs\":%u,\"MaxNetworkUs\":%u"
                  "},"
                  "\"JPEG\":{"
                    "\"LastSize\":%u,\"AvgSize\":%u,\"MinSize\":%u,\"MaxSize\":%u,\"CompressionRatio\":\"%.2f\""
                  "}}}"),
    state_name,
    Wc.core.config.width, Wc.core.config.height,
    WcStats.frames_captured,
    WcStats.frames_processed,
    WcStats.frames_unsent,
    WcStats.jpeg_errors,
    WcStats.jpeg_resets,
    WcStats.bytes_sent,
    WcStats.uptime_seconds,
    WcStats.last_fps,
    WcStats.last_frame_time_ms,
    WcStats.last_mutex_wait_us,
    WcStats.last_cache_sync_us,
    WcStats.last_jpeg_encode_us,
    WcStats.last_network_write_us,
    WcStats.max_mutex_wait_us,
    WcStats.max_jpeg_encode_us,
    WcStats.max_network_write_us,
    WcStats.last_jpeg_size,
    WcStats.avg_jpeg_size,
    WcStats.min_jpeg_size,
    WcStats.max_jpeg_size,
    WcStats.compression_ratio_x100 / 100.0f);
}

void CmndWcConfig(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcConfig called"));
  Response_P(PSTR("{\"WcConfig\":{\"Sensor\":\"%.8s\",\"Width\":%d,\"Height\":%d,\"MaxWidth\":%d,\"MaxHeight\":%d,\"Format\":%d,\"MipiClock\":%d,\"Lanes\":%d,\"OffsetX\":%d,\"OffsetY\":%d,\"Binning\":%d,\"FPS\":%d,\"ResIndex\":%d,\"Flags\":\"0x%02X\"}}"),
    Wc.core.config.name,
    Wc.core.config.width, 
    Wc.core.config.height,
    Wc.core.config.max_width,
    Wc.core.config.max_height,
    Wc.core.config.format, 
    Wc.core.config.mipi_clock, 
    Wc.core.config.lane_num,
    Wc.core.config.offset_x,
    Wc.core.config.offset_y,
    Wc.core.config.binning,
    Wc.core.config.fps,
    Wc.core.config.res_index,
    Wc.core.config.flags);
}


/*********************************************************************************************/

void CmndWcQuality(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcQuality called, payload=%d"), XdrvMailbox.payload);
  
  // Query current quality
  if (XdrvMailbox.payload < 0) {
    ResponseCmndNumber(Wc.jpeg.quality);
    return;
  }
  
  // Validate quality (1-100)
  if (XdrvMailbox.payload < 1 || XdrvMailbox.payload > 100) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid quality %d (must be 1-100)"), XdrvMailbox.payload);
    ResponseCmndFailed();
    return;
  }
  
  // Set new quality (will take effect on next stream start)
  Wc.jpeg.quality = (uint8_t)XdrvMailbox.payload;
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: JPEG quality set to %d"), Wc.jpeg.quality);
  ResponseCmndNumber(Wc.jpeg.quality);
}

void CmndWcSession(void) {
  // Session types: 0=None, 1=MJPEG, 2=RTSP, 3=WebRTC, 4=DSI
  const char* session_names[] = {"None", "MJPEG", "RTSP", "WebRTC", "DSI"};
  
  // Query current session type
  if (XdrvMailbox.payload < 0) {
    Response_P(PSTR("{\"WcSession\":{\"Type\":%d,\"Name\":\"%s\",\"State\":%d}}"), 
      Wc.core.session_type, 
      (Wc.core.session_type <= 4) ? session_names[Wc.core.session_type] : "Unknown",
      Wc.core.state);
    return;
  }
  
  // Validate session type
  if (XdrvMailbox.payload > 4) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid session type %d"), XdrvMailbox.payload);
    ResponseCmndFailed();
    return;
  }
  
  camera_session_t new_type = (camera_session_t)XdrvMailbox.payload;
  
  // Same session type - do nothing
  if (new_type == Wc.core.session_type) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Session type unchanged"));
    Response_P(PSTR("{\"WcSession\":{\"Type\":%d,\"Name\":\"%s\"}}"), 
      Wc.core.session_type, session_names[Wc.core.session_type]);
    return;
  }
  
  // Check if implemented
  if (new_type == SESSION_WEBRTC || new_type == SESSION_DSI_DISPLAY) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Session type %d not yet implemented"), new_type);
    Response_P(PSTR("{\"WcSession\":{\"Error\":\"Not implemented\",\"Requested\":%d}}"), new_type);
    return;
  }
  
  // Stop current session if running
  if (Wc.core.state != CAM_IDLE) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopping current session for switch"));
    WcStop();
  }
  
  // Set new session type
  Wc.core.session_type = new_type;
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Session type changed to %d (%s)"), 
    Wc.core.session_type, session_names[Wc.core.session_type]);
  
  // Initialize RTSP session
  if (new_type == SESSION_RTSP) {
    Wc.rtp.sequence = random(0, 65535);
    Wc.rtp.timestamp = random(0, UINT32_MAX);
    Wc.rtp.ssrc = random(0, UINT32_MAX);
    Wc.rtsp.streaming = false;
    
    // Start RTSP server on port 554
    Wc.rtsp.server = new WiFiServer(554);
    Wc.rtsp.server->begin();
    
    // Start UDP socket for RTP
    Wc.rtp_udp.begin(5004);
    
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTSP server started on port 554"));
  }
  
  // Setup and start new session (unless SESSION_NONE)
  if (new_type != SESSION_NONE) {
    if (WcSetup(false)) {
      WcStart();
    }
  }
  
  Response_P(PSTR("{\"WcSession\":{\"Type\":%d,\"Name\":\"%s\",\"State\":%d}}"), 
    Wc.core.session_type, session_names[Wc.core.session_type], Wc.core.state);
}

void CmndWcRtpDest(void) {
  // Query current RTP destination
  if (XdrvMailbox.data_len == 0) {
    Response_P(PSTR("{\"WcRtpDest\":{\"IP\":\"%s\",\"Port\":%d}}"), 
      Wc.rtp_dest_ip.toString().c_str(), Wc.rtp.dest_port);
    return;
  }
  
  // Parse IP and port from data
  // Format: "192.168.1.100 5004"
  char ip_str[16] = {0};
  uint16_t port = 5004;  // Default port
  
  // Find space separator
  char* space_pos = strchr(XdrvMailbox.data, ' ');
  if (space_pos) {
    // Copy IP part
    size_t ip_len = space_pos - XdrvMailbox.data;
    if (ip_len < sizeof(ip_str)) {
      strncpy(ip_str, XdrvMailbox.data, ip_len);
      ip_str[ip_len] = '\0';
      
      // Parse port
      port = atoi(space_pos + 1);
    }
  } else {
    // Only IP provided, use default port
    strncpy(ip_str, XdrvMailbox.data, sizeof(ip_str) - 1);
  }
  
  // Validate IP
  IPAddress new_ip;
  if (!new_ip.fromString(ip_str)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid IP address: %s"), ip_str);
    ResponseCmndFailed();
    return;
  }
  
  // Validate port
  if (port == 0 || port > 65535) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid port: %d"), port);
    ResponseCmndFailed();
    return;
  }
  
  // Update destination
  Wc.rtp_dest_ip = new_ip;
  Wc.rtp.dest_port = port;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP destination set to %s:%d"), 
    Wc.rtp_dest_ip.toString().c_str(), Wc.rtp.dest_port);
  
  Response_P(PSTR("{\"WcRtpDest\":{\"IP\":\"%s\",\"Port\":%d}}"), 
    Wc.rtp_dest_ip.toString().c_str(), Wc.rtp.dest_port);
}


/*********************************************************************************************/

void WcInit(void) {
  // POD sections can be safely memset
  memset(&Wc.core, 0, sizeof(Wc.core));
  memset(&Wc.jpeg, 0, sizeof(Wc.jpeg));
  memset(&Wc.h264, 0, sizeof(Wc.h264));
  memset(&Wc.rtp, 0, sizeof(Wc.rtp));
  memset(&Wc.rtsp, 0, sizeof(Wc.rtsp));
  
  // Set non-zero defaults
  Wc.core.state = CAM_IDLE;
  Wc.core.session_type = SESSION_MJPEG_HTTP;
  Wc.jpeg.quality = 50;
  Wc.rtp.dest_port = 5004;
  
  // C++ objects (rtp_dest_ip, rtp_udp, rtsp_client) have constructors - don't touch
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI driver loaded"));
}


void WcLoop(void) {
  // Skip during state transitions
  if (Wc.core.state == CAM_STOPPING || Wc.core.state == CAM_IDLE) {
    return;
  }
  
  // Handle RTSP control connections
  if (Wc.core.session_type == SESSION_RTSP) {
    HandleRtsp();
  }
  
  if (Wc.core.state == CAM_STREAMING && !Wc.jpeg.server && !TasmotaGlobal.global_state.network_down) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Starting stream server..."));
    WcSetStreamserver(1);
  }
  
  if (Wc.jpeg.server) {
    Wc.jpeg.server->handleClient();
    
    // Monitor client connection - cleanup if disconnected (with mutex protection)
    if (Wc.jpeg.stream_active && xSemaphoreTake(Wc.core.frame_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      if (Wc.jpeg.client_ptr && !Wc.jpeg.client_ptr->connected()) {
        delete Wc.jpeg.client_ptr;
        Wc.jpeg.client_ptr = nullptr;
        Wc.jpeg.stream_active = 0;
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client disconnected"));
      }
      xSemaphoreGive(Wc.core.frame_mutex);
    }
  }
}

// Web UI Strings
const char HTTP_WC_MODE[] PROGMEM = "{s}Camera Mode{m}%s{e}";
const char HTTP_WC_RES[]  PROGMEM = "{s}Resolution{m}%dx%d{e}";
const char HTTP_WC_FPS[]  PROGMEM = "{s}Frame Rate{m}%d fps{e}";

void WcWebInfo(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"Cam\":{\"Sensor\":\"%.8s\",\"State\":%d,\"Res\":\"%dx%d\",\"FPS\":%d,\"Motion\":%d}"), Wc.core.config.name, Wc.core.state, Wc.core.config.width, Wc.core.config.height, WcStats.last_fps, Wc.h264.motion_val);
    return;
  }

  const char* mode_str = "Standby";
  if (Wc.core.state == CAM_STREAMING) {
    switch (Wc.core.session_type) {
      case SESSION_MJPEG_HTTP: mode_str = "MJPEG Server"; break;
      case SESSION_RTSP:       mode_str = "RTSP Stream"; break;
      case SESSION_WEBRTC:     mode_str = "WebRTC"; break;
      case SESSION_DSI_DISPLAY:mode_str = "Local Display"; break;
      default:                 mode_str = "Active"; break;
    }
  } else if (Wc.core.state == CAM_INIT || Wc.core.state == CAM_PAUSED) {
    mode_str = "Ready";
  }

  WSContentSend_PD(HTTP_WC_MODE, mode_str);
  WSContentSend_PD(HTTP_WC_RES, Wc.core.config.width, Wc.core.config.height);
  if (Wc.core.state == CAM_STREAMING) {
    WSContentSend_PD(HTTP_WC_FPS, WcStats.last_fps);
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
    case FUNC_JSON_APPEND:
      WcWebInfo(true);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      WcWebInfo(false);
      break;
#endif
    case FUNC_PRE_INIT:
      WcInit();
      break;
    case FUNC_INIT:
      if (Wc.core.state == CAM_IDLE) {
        WcSetup(true);  // First boot - reset config to defaults
      }
      break;
    case FUNC_EVERY_SECOND:
      // Auto-start streaming once WiFi is available
      if (Wc.core.state == CAM_INIT && !TasmotaGlobal.global_state.network_down) {
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

#endif  // USE_CSI_WEBCAM
#endif  // ESP32P4
