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
  SESSION_RTSP_AND_WS, // H.264 over RTP/UDP with RTSP control or WebSocket
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
  CAM_STOPPING,        // Full shutdown in progress, reject all work
  CAM_FAILED           // Initialization failed, manual intervention required
} camera_state_t;

// Failure reason tracking
typedef enum {
  CAM_FAIL_NONE = 0,
  CAM_FAIL_MEMORY,           // Memory allocation failure
  CAM_FAIL_BERRY_INIT,       // Berry sensor init failed
  CAM_FAIL_BERRY_STREAM,     // Berry stream control failed
  CAM_FAIL_CSI_INIT,         // CSI controller init failed
  CAM_FAIL_ISP_INIT,         // ISP init failed
  CAM_FAIL_ENCODER_INIT,     // JPEG/H264 encoder init failed
  CAM_FAIL_MUTEX,            // Mutex creation failed
  CAM_FAIL_TASK,             // Task creation failed
  CAM_FAIL_LDO               // MIPI LDO init failed
} camera_fail_reason_t;

/*********************************************************************************************/

// Configuration - what Berry tells us about the sensor (28 bytes)
struct CSI_Config {
  uint16_t width;           // 0-1: Active pixels per line
  uint16_t height;          // 2-3: Active lines per frame
  uint16_t max_width;       // 4-5: Maximum sensor resolution width
  uint16_t max_height;      // 6-7: Maximum sensor resolution height
  uint8_t format;           // 8: COLOR_PIXEL_RAW8/RAW10/RAW12 (pixel format part only)
  uint8_t lane_num;         // 9: Number of CSI lanes (typically 2)
  uint16_t mipi_clock;      // 10-11: Mbps per lane (e.g. 200) - Total MIPI bandwidth = mipi_clock * lane_num Mbps
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
    camera_fail_reason_t fail_reason;
    esp_err_t fail_esp_err;
    uint32_t isp_gamma_y[16];    
  } core;
  
  // --- 2. JPEG Session (POD) ---
  struct {
    jpeg_encoder_handle_t handle;
    void *buffer;
    size_t buffer_size;
    jpeg_encode_cfg_t cfg;
    uint8_t quality; // TODO: move to core, as we use it for H264 too
    SemaphoreHandle_t mutex;
    ESP8266WebServer *server;
    WiFiClient *client_ptr;
    uint8_t stream_active;
  } jpeg;
  
  // --- 3. H.264 Session (POD) ---
  struct {
    esp_h264_enc_handle_t handle;
    uint8_t *out_buffer;
    size_t out_buffer_size;
    uint32_t motion_val;      // Current rolling average (0-100+)
    uint32_t motion_raw;      // Raw P-frame size (debug)
    // SPS/PPS storage for WebRTC
    uint8_t sps_buffer[512];
    size_t sps_len;
    uint8_t pps_buffer[256];
    size_t pps_len;
    bool sps_pps_captured;
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

  // --- 6. WebSocket Server (POD) ---
  struct {
      WiFiServer *server;
      WiFiClient *client_ptr; // Use pointer to control lifecycle
      bool active;
  } ws;
  
  // --- 7. C++ Objects (Complex types - NO MEMSET) ---
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

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 5, 0)
bool WcIspApplyConfig(isp_proc_handle_t handle, const char* sensor_name, int width, int height);
#endif

#define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"

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

const char kWCCommands[] PROGMEM = D_PREFX_WEBCAM "|"
  D_CMND_WC_RES "|" D_CMND_WC_STREAM "|" D_CMND_WC_STOP "|" D_CMND_WC_STATUS "|" D_CMND_WC_CONFIG "|" D_CMND_WC_WINDOW "|" D_CMND_WC_QUALITY "|" D_CMND_WC_SESSION;

void (* const WCCommand[])(void) PROGMEM = {
  &CmndWcRes, &CmndWcStream, &CmndWcStop, &CmndWcStatus, &CmndWcConfig, &CmndWcWindow, &CmndWcQuality, &CmndWcSession
};

/*********************************************************************************************/

// Debug counters for callbacks (volatile for ISR access)
static volatile uint32_t cb_get_new_count = 0;
static volatile uint32_t cb_finished_count = 0;

// Helper function to handle failures - performs full cleanup and sets failed state
void WcSetFailed(camera_fail_reason_t reason, esp_err_t esp_err = ESP_OK) {
  AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Entering FAILED state, reason=%d, esp_err=0x%x"), reason, esp_err);
  
  // 1. Stop sensor streaming (harmless if not yet started)
  callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 0, nullptr, 0);
  
  // 2. Gate ISR and wake/join processing task
  if (Wc.core.cam_task_handle) {
    Wc.core.state = CAM_STOPPING;
    if (Wc.core.resume_sem) xSemaphoreGive(Wc.core.resume_sem);
    // NOTE: no xSemaphoreGive(frame_mutex) — mutex, not a signal
    xTaskNotifyGive(Wc.core.cam_task_handle);
    
    for (int i = 0; i < 50 && Wc.core.cam_task_handle != NULL; i++) {
      delay(10);
    }
    
    if (Wc.core.cam_task_handle != NULL) {
      vTaskDelete(Wc.core.cam_task_handle);
      Wc.core.cam_task_handle = NULL;
    }
  } else {
    Wc.core.state = CAM_STOPPING;
  }
  
  // 3. Deinitialize hardware pipeline
  WcDeinitPipeline();
  
  // 4. Clean up network clients
  if (Wc.jpeg.client_ptr) {
    delete Wc.jpeg.client_ptr;
    Wc.jpeg.client_ptr = nullptr;
  }
  WcRtspStop();
  WcWebRTCStop();
  
  // 5. Delete mutexes
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
  
  // 6. Set failed state with reason
  Wc.core.state = CAM_FAILED;
  Wc.core.fail_reason = reason;
  Wc.core.fail_esp_err = esp_err;
  Wc.jpeg.stream_active = 0;
  
  AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Cleanup complete, state=FAILED"));
}

// Callback: Provide new buffer for next frame (Ping-Pong Logic)
static bool IRAM_ATTR csi_on_get_new_vb(esp_cam_ctlr_handle_t handle, esp_cam_ctlr_trans_t *trans, void *user_data) {
  if (Wc.core.state != CAM_STREAMING) return false;  // ← ADDED
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
  TaskHandle_t task = Wc.core.cam_task_handle;  // safe capture
  if (!task) return false;
  vTaskNotifyGiveFromISR(task, &xHigherPriorityTaskWoken);
  
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
    if (!Wc.core.frame_buffer[i]) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Frame buffer %d allocation failed"), i);
      WcSetFailed(CAM_FAIL_MEMORY);
      return 0;
    }
    esp_cache_msync(Wc.core.frame_buffer[i], Wc.core.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_C2M);
  }
  Wc.core.write_idx = 0;
  Wc.core.read_idx = 1;

  // 2. Configure CSI
  cam_ctlr_color_t csi_output_format = (Wc.core.session_type == SESSION_RTSP_AND_WS || Wc.core.session_type == SESSION_WEBRTC) ? CAM_CTLR_COLOR_YUV420 : CAM_CTLR_COLOR_YUV422; // H.264 requires YUV420, JPEG needs YUV422 on early P4 chips
  
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
  ret = esp_cam_new_csi_ctlr(&csi_config, &Wc.core.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CSI init failed (0x%x)"), ret);
    WcSetFailed(CAM_FAIL_CSI_INIT, ret);
    return 0;
  }

  // 3. Callbacks
  esp_cam_ctlr_evt_cbs_t cbs = { .on_get_new_trans = csi_on_get_new_vb, .on_trans_finished = csi_on_trans_finished };
  Wc.core.cam_trans.buffer = Wc.core.frame_buffer[0];
  Wc.core.cam_trans.buflen = Wc.core.frame_buffer_size;
  ret = esp_cam_ctlr_register_event_callbacks(Wc.core.cam_handle, &cbs, &Wc.core.cam_trans);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Callback registration failed (0x%x)"), ret);
    WcSetFailed(CAM_FAIL_CSI_INIT, ret);
    return 0;
  }
  ret = esp_cam_ctlr_enable(Wc.core.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CSI enable failed (0x%x)"), ret);
    WcSetFailed(CAM_FAIL_CSI_INIT, ret);
    return 0;
  }

  // 4. ISP — always created here with correct output format for session type
  {
    isp_color_t isp_output_format = (Wc.core.session_type == SESSION_RTSP_AND_WS || Wc.core.session_type == SESSION_WEBRTC) ? ISP_COLOR_YUV420 : ISP_COLOR_YUV422;
    esp_isp_processor_cfg_t isp_config = {
      .clk_hz = 120 * 1000 * 1000,
      .input_data_source = ISP_INPUT_DATA_SOURCE_CSI,
      .input_data_color_type = (isp_color_t)COLOR_TYPE_ID(COLOR_SPACE_RAW, (color_pixel_raw_format_t)Wc.core.config.format),
      .output_data_color_type = isp_output_format,
      .h_res = Wc.core.config.width,
      .v_res = Wc.core.config.height,
    };
    ret = esp_isp_new_processor(&isp_config, &Wc.core.isp_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP init failed (0x%x)"), ret);
      WcSetFailed(CAM_FAIL_ISP_INIT, ret);
      return 0;
    }

    esp_isp_enable(Wc.core.isp_handle);

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 5, 0)
    WcIspApplyConfig(Wc.core.isp_handle, Wc.core.config.name, Wc.core.config.width, Wc.core.config.height);
#endif
  }

  // 5. Encoders
  if (Wc.core.session_type == SESSION_MJPEG_HTTP) {
    if (!WcSetupJpegEncoder()) {
      WcSetFailed(CAM_FAIL_ENCODER_INIT);
      return 0;
    }
  } else if (Wc.core.session_type == SESSION_RTSP_AND_WS || Wc.core.session_type == SESSION_WEBRTC) {
    if (!WcSetupH264Encoder()) {
      WcSetFailed(CAM_FAIL_ENCODER_INIT);
      return 0;
    }
  }

  return 1;
}


// De-initialize only the resolution-dependent hardware
void WcDeinitPipeline() {
  // 0. AWB must go before ISP processor
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 5, 0)
  WcIspDeinitAWB();
#endif
  
  // 1. Delete Encoder
  if (Wc.h264.handle) {
    esp_h264_enc_del(Wc.h264.handle); // Only if using helper that doesn't double-free
    Wc.h264.handle = NULL;
  }
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

  // 3. Delete ISP
  if (Wc.core.isp_handle) {
    esp_isp_disable(Wc.core.isp_handle);
    esp_isp_del_processor(Wc.core.isp_handle);
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
    WcSetFailed(CAM_FAIL_LDO, ret);
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
    WcSetFailed(CAM_FAIL_BERRY_INIT);
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
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Total MIPI bandwidth: %d Mbps"), Wc.core.config.mipi_clock * Wc.core.config.lane_num);
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: ===== END CONFIG ====="));

  // 3. Initialize Resolution-Dependent Hardware (Buffers, CSI, ISP, Encoders)
  if (!WcInitPipeline()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Pipeline Init Failed"));
    return 0;
  }

  // 4. Create mutexes for thread safety
  Wc.core.frame_mutex = xSemaphoreCreateMutex();
  Wc.jpeg.mutex = xSemaphoreCreateMutex();
  Wc.core.resume_sem = xSemaphoreCreateBinary();  // For pause/resume
  
  if (!Wc.core.frame_mutex || !Wc.jpeg.mutex || !Wc.core.resume_sem) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to create mutexes"));
    WcDeinitPipeline();
    if (Wc.core.frame_mutex) vSemaphoreDelete(Wc.core.frame_mutex);
    if (Wc.jpeg.mutex) vSemaphoreDelete(Wc.jpeg.mutex);
    if (Wc.core.resume_sem) vSemaphoreDelete(Wc.core.resume_sem);
    WcSetFailed(CAM_FAIL_MUTEX);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Mutexes created"));

  // 5. Create processing task based on session type
  TaskFunction_t task_func = nullptr;   // ← was uninitialized
  const char *task_name = nullptr;      // ← was uninitialized
  void WebRTCProcessingTask(void *pvParameters);
  
  if (Wc.core.session_type == SESSION_MJPEG_HTTP) {
    task_func = MjpegProcessingTask;
    task_name = "MjpegTask";
  } else if (Wc.core.session_type == SESSION_RTSP_AND_WS) {
    task_func = H264ProcessingTask;
    task_name = "H264Task";
  } else if (Wc.core.session_type == SESSION_WEBRTC) {
    // Initialize WebRTC specific structs/DTLS (network layer only)
    if (!WcSetupWebRTC()) return 0;

    // Set Task
    task_func = WebRTCProcessingTask;
    task_name = "WebRTCTask";
  }
  
  // ← ADDED: guard against unknown/unhandled session type
  if (!task_func) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Unknown session type %d, no task function"), Wc.core.session_type);
    WcDeinitPipeline();
    vSemaphoreDelete(Wc.core.frame_mutex);
    vSemaphoreDelete(Wc.jpeg.mutex);
    vSemaphoreDelete(Wc.core.resume_sem);
    WcSetFailed(CAM_FAIL_TASK);
    return 0;
  }
  
  BaseType_t task_created = xTaskCreatePinnedToCore(
    task_func,
    task_name,
    16000, // 8KB Stack
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
    WcSetFailed(CAM_FAIL_TASK);
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
    WcSetFailed(CAM_FAIL_BERRY_STREAM);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Sensor streaming started"));
  
  // Give sensor time to start streaming
  delay(100);

  Wc.core.state = CAM_STREAMING;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Streaming active"));
  return 1;
}


uint32_t WcStop(void) {
  if (Wc.core.state == CAM_IDLE || Wc.core.state == CAM_STOPPING) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Nothing to stop (state=%d)"), Wc.core.state);
    return 0;
  }
  
  // If already failed, just clear failure info and return
  if (Wc.core.state == CAM_FAILED) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Clearing failed state"));
    Wc.core.fail_reason = CAM_FAIL_NONE;
    Wc.core.fail_esp_err = ESP_OK;
    Wc.core.state = CAM_IDLE;
    return 1;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopping (state=%d)"), Wc.core.state);
  
  // 1. Stop sensor streaming (MIPI stops; harmless if already idle)
  callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 0, nullptr, 0);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Called Berry stream stop"));
  
  // 2. Gate ISR immediately
  Wc.core.state = CAM_STOPPING;
  
  // 3. Wake paused task so it can see STOPPING and exit
  if (Wc.core.resume_sem) xSemaphoreGive(Wc.core.resume_sem);
  // NOTE: no xSemaphoreGive(frame_mutex) — mutex not a signal; task unblocked by notify below
  
  // 4. Wake task and wait for it to exit
  if (Wc.core.cam_task_handle) {
    xTaskNotifyGive(Wc.core.cam_task_handle);
    
    // Wait for task to exit (task sets handle to NULL before deleting itself)
    for (int i = 0; i < 50 && Wc.core.cam_task_handle != NULL; i++) {
      delay(25);
    }
    
    // Force delete if task didn't exit
    if (Wc.core.cam_task_handle != NULL) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Task didn't exit cleanly, force deleting"));
      vTaskDelete(Wc.core.cam_task_handle);
      Wc.core.cam_task_handle = NULL;
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Task stopped"));
  }
  
  // 5. Deinitialize hardware pipeline (Buffers, CSI, ISP, Encoders)
  WcDeinitPipeline();
  
  // 6. Clean up network clients
  if (Wc.jpeg.client_ptr) {
    delete Wc.jpeg.client_ptr;
    Wc.jpeg.client_ptr = nullptr;
  }
  
  // 7. Stop RTSP server and close client
  WcRtspStop();
  
  // 8. Stop WebRTC (UDP, DTLS task, state)
  WcWebRTCStop();
  
  // 9. Delete mutexes
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

  // 10. Final state
  Wc.core.state = CAM_IDLE;
  Wc.jpeg.stream_active = 0;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopped"));
  return 1;
}

/*********************************************************************************************/

void WcInit(void) {
  // POD sections can be safely memset
  memset(&Wc.core, 0, sizeof(Wc.core));
  memset(&Wc.jpeg, 0, sizeof(Wc.jpeg));
  memset(&Wc.h264, 0, sizeof(Wc.h264));
  memset(&Wc.rtp, 0, sizeof(Wc.rtp));
  memset(&Wc.rtsp, 0, sizeof(Wc.rtsp));
  memset(&Wc.ws, 0, sizeof(Wc.ws));
  
  // Set non-zero defaults
  Wc.core.state = CAM_IDLE;
  Wc.core.session_type = SESSION_MJPEG_HTTP;
  Wc.core.fail_reason = CAM_FAIL_NONE;
  Wc.core.fail_esp_err = ESP_OK;
  Wc.jpeg.quality = 50;
  Wc.rtp.dest_port = 5004;
  
  // C++ objects (rtp_dest_ip, rtp_udp, rtsp_client) have constructors - don't touch
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI driver loaded"));
}


void WcPicSetup(void) {
  WebServer_on(PSTR("/wc.jpg"), HandleImage);
  WebServer_on(PSTR("/wc.mjpeg"), HandleImage);
  WebServer_on(PSTR("/snapshot.jpg"), HandleImage);
  // WebServer_on(PSTR("/wc_h264.html"), HandleWebcamH264Html);
}

void WcLoop(void) {
  // Skip during state transitions or failure
  if (Wc.core.state == CAM_STOPPING || Wc.core.state == CAM_IDLE || Wc.core.state == CAM_FAILED) {
    return;
  }

  // Handle RTSP control connections (Session 2)
  if (Wc.core.session_type == SESSION_RTSP_AND_WS) {
    HandleRtsp();
  }

  // WebRTC DTLS is handled by dedicated HandshakeTask (file 4) - no call here

  // MJPEG HTTP server (port 81) must ONLY run in MJPEG session
  if (Wc.core.state == CAM_STREAMING) {

    if (Wc.core.session_type == SESSION_MJPEG_HTTP && !TasmotaGlobal.global_state.network_down) {
      if (!Wc.jpeg.server) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Starting stream server..."));
        WcSetMjpegServer(1);
      }
    } else {
      // Not MJPEG session OR network down -> ensure port 81 server is down
      if (Wc.jpeg.server) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stopping MJPEG stream server (not MJPEG session / net down)"));
        WcSetMjpegServer(0);
      }
    }
  }

  // Only handle MJPEG clients if the server exists (i.e., MJPEG session)
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
      WcShowInfo(true);
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      WcShowInfo(false);
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
    case FUNC_EVERY_250_MSECOND:
      if (Wc.core.state == CAM_STREAMING) {
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 5, 0)
        WcIspAwbProcess();
#endif
      }
      break;
    case FUNC_EVERY_SECOND:
      // Auto-start streaming once WiFi is available (only from INIT, not after FAILED)
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
