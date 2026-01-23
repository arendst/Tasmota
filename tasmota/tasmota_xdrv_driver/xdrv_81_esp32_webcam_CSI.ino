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


/*********************************************************************************************/
// MJPEG Processing Task - Dedicated FreeRTOS task for JPEG encoding and HTTP streaming

void MjpegProcessingTask(void *pvParameters) {
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); // 100ms timeout
  uint32_t last_fps_calc = millis();
  uint32_t frames_in_second = 0;
  static uint32_t last_profile_log = 0;
  
  // Loop forever, exit only on CAM_STOPPING
  while (true) {
    // Wait here until ISR signals "Frame Ready"
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    
    // Exit on stop signal
    if (Wc.core.state == CAM_STOPPING) {
      break;
    }
    
    // Handle pause request - signal we're paused, then wait for resume
    if (Wc.core.state == CAM_PAUSING) {
      Wc.core.state = CAM_PAUSED;  // Signal that we're safely paused
      // Wait for resume signal (blocks until semaphore given)
      xSemaphoreTake(Wc.core.resume_sem, portMAX_DELAY);
      // After resume, check state again
      if (Wc.core.state == CAM_STOPPING) {
        break;
      }
      continue;  // Go back to top of loop
    }
    
    // Only process frames when streaming and we got a notification
    if (Wc.core.state != CAM_STREAMING || ulNotificationValue == 0) {
      continue;
    }
    
    // We have a frame!
    uint32_t frame_start = millis();
    
    // Lock frame access to prevent race with WcLoop
    uint32_t mutex_start = micros();
    if (xSemaphoreTake(Wc.core.frame_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
      WcStats.frames_unsent++;
      continue;
    }
    
    WcStats.last_mutex_wait_us = micros() - mutex_start;
    if (WcStats.last_mutex_wait_us > WcStats.max_mutex_wait_us) {
      WcStats.max_mutex_wait_us = WcStats.last_mutex_wait_us;
    }
    
    // Check state after acquiring mutex - exit quickly if stopping
    if (Wc.core.state == CAM_STOPPING) {
      xSemaphoreGive(Wc.core.frame_mutex);
      break;
    }
    
    // Only process if we have a client connected to stream
    if (Wc.jpeg.stream_active != 2 || !Wc.jpeg.client_ptr || !Wc.jpeg.client_ptr->connected()) {
      WcStats.frames_unsent++;
      xSemaphoreGive(Wc.core.frame_mutex);
      continue;
    }
    
    uint8_t *source_buf = Wc.core.frame_buffer[Wc.core.read_idx];
    
    // Cache Sync (Hardware M2C)
    uint32_t cache_start = micros();
    esp_cache_msync(source_buf, Wc.core.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
    WcStats.last_cache_sync_us = micros() - cache_start;
    
    // Lock JPEG encoder to prevent race with HandleImage
    uint32_t jpeg_mutex_start = micros();
    if (xSemaphoreTake(Wc.jpeg.mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
      WcStats.frames_unsent++;
      xSemaphoreGive(Wc.core.frame_mutex);
      continue;
    }
    
    // Check state after acquiring mutex - exit quickly if stopping
    if (Wc.core.state == CAM_STOPPING) {
      xSemaphoreGive(Wc.jpeg.mutex);
      xSemaphoreGive(Wc.core.frame_mutex);
      break;
    }
    
    WcStats.last_jpeg_mutex_wait_us = micros() - jpeg_mutex_start;
    
    uint32_t jpeg_size = 0;
    uint32_t jpeg_start = micros();
    esp_err_t ret = jpeg_encoder_process(Wc.jpeg.handle, 
                                          &Wc.jpeg.cfg,
                                          source_buf, 
                                          Wc.core.frame_buffer_size,
                                          (uint8_t*)Wc.jpeg.buffer, 
                                          Wc.jpeg.buffer_size,
                                          &jpeg_size);
    WcStats.last_jpeg_encode_us = micros() - jpeg_start;
    if (WcStats.last_jpeg_encode_us > WcStats.max_jpeg_encode_us) {
      WcStats.max_jpeg_encode_us = WcStats.last_jpeg_encode_us;
    }
    
    // Auto-Reset on 0x103 (Engine Stuck)
    if (ret == ESP_ERR_INVALID_STATE) {
      jpeg_del_encoder_engine(Wc.jpeg.handle);
      jpeg_encode_engine_cfg_t jpeg_eng_cfg = { .intr_priority = 0, .timeout_ms = 100 };
      jpeg_new_encoder_engine(&jpeg_eng_cfg, &Wc.jpeg.handle);
      WcStats.jpeg_resets++;
      xSemaphoreGive(Wc.jpeg.mutex);
      xSemaphoreGive(Wc.core.frame_mutex);
      continue;
    }

    if (ret == ESP_OK && jpeg_size > 0) {
      // Track JPEG size statistics
      WcStats.last_jpeg_size = jpeg_size;
      if (WcStats.min_jpeg_size == 0 || jpeg_size < WcStats.min_jpeg_size) {
        WcStats.min_jpeg_size = jpeg_size;
      }
      if (jpeg_size > WcStats.max_jpeg_size) {
        WcStats.max_jpeg_size = jpeg_size;
      }
      // Calculate compression ratio (raw YUV422 size / JPEG size)
      uint32_t raw_size = Wc.core.frame_buffer_size;
      WcStats.compression_ratio_x100 = (raw_size * 100) / jpeg_size;
      
      // Check state before starting network write - don't start new writes if stopping
      if (Wc.core.state == CAM_STOPPING) {
        xSemaphoreGive(Wc.jpeg.mutex);
        xSemaphoreGive(Wc.core.frame_mutex);
        break;
      }
      
      uint32_t network_start = micros();
      Wc.jpeg.client_ptr->print("--" BOUNDARY "\r\n");
      Wc.jpeg.client_ptr->printf("Content-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n", jpeg_size);
      Wc.jpeg.client_ptr->write((const uint8_t*)Wc.jpeg.buffer, jpeg_size);
      Wc.jpeg.client_ptr->print("\r\n");
      WcStats.last_network_write_us = micros() - network_start;
      if (WcStats.last_network_write_us > WcStats.max_network_write_us) {
        WcStats.max_network_write_us = WcStats.last_network_write_us;
      }
      
      // Statistics
      WcStats.frames_processed++;
      WcStats.bytes_sent += jpeg_size;
      frames_in_second++;
      
      // Yield to let network stack process
      taskYIELD();
    } else {
      WcStats.jpeg_errors++;
    }
    
    xSemaphoreGive(Wc.jpeg.mutex);
    xSemaphoreGive(Wc.core.frame_mutex);
    
    // Track frame processing time
    WcStats.last_frame_time_ms = millis() - frame_start;
    
    // Calculate FPS every second
    if (millis() - last_fps_calc >= 1000) {
      WcStats.last_fps = frames_in_second;
      
      // Calculate averages (if we processed frames)
      if (frames_in_second > 0) {
        WcStats.avg_mutex_wait_us = WcStats.last_mutex_wait_us;
        WcStats.avg_cache_sync_us = WcStats.last_cache_sync_us;
        WcStats.avg_jpeg_encode_us = WcStats.last_jpeg_encode_us;
        WcStats.avg_network_write_us = WcStats.last_network_write_us;
        WcStats.avg_jpeg_size = WcStats.last_jpeg_size;
      }
      
      frames_in_second = 0;
      last_fps_calc = millis();
      
      // Update uptime
      if (WcStats.start_time > 0) {
        WcStats.uptime_seconds = (millis() - WcStats.start_time) / 1000;
      }
    }
    
    // Periodic profiling log (every 5 seconds)
    if (millis() - last_profile_log >= 5000) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: Profile - JPEG:%uus(%uKB,%.2fx) Net:%uus Cache:%uus Mutex:%uus FPS:%u"),
        WcStats.last_jpeg_encode_us,
        WcStats.last_jpeg_size / 1024,
        WcStats.compression_ratio_x100 / 100.0f,
        WcStats.last_network_write_us,
        WcStats.last_cache_sync_us,
        WcStats.last_mutex_wait_us,
        WcStats.last_fps);
      last_profile_log = millis();
    }
  }
  
  // Task exiting - delete ourselves
  Wc.core.cam_task_handle = NULL;
  vTaskDelete(NULL);
}

/*********************************************************************************************/
// RTP Helper Function

#define RTP_MAX_PAYLOAD 1400  // MTU safety margin
#define RTP_HEADER_SIZE 12

void SendNalUnitRTP(uint8_t* naldata, size_t nallen, uint8_t naltype, uint8_t nalnri) {
  // Buffer for RTP packet construction
  // 12 (RTP Header) + 2 (FU-A Headers) + Payload
  uint8_t rtppacket[RTP_MAX_PAYLOAD + RTP_HEADER_SIZE + 2];

  // ---------------------------------------------------------
  // Case A: Small NAL - Send as Single NAL Unit Packet
  // ---------------------------------------------------------
  if (nallen <= RTP_MAX_PAYLOAD) {
    // Build RTP header
    rtppacket[0] = 0x80;  // V=2, P=0, X=0, CC=0
    rtppacket[1] = 96;    // PT=96 (H.264)
    
    // Set Marker Bit (M) if this is the last NAL of a frame (Video Coding Layer NALs)
    if (naltype == 1 || naltype == 5) {
      rtppacket[1] |= 0x80;
    }

    // Sequence Number (Big Endian)
    rtppacket[2] = (Wc.rtp.sequence >> 8) & 0xFF;
    rtppacket[3] = Wc.rtp.sequence & 0xFF;

    // Timestamp (Big Endian)
    rtppacket[4] = (Wc.rtp.timestamp >> 24) & 0xFF;
    rtppacket[5] = (Wc.rtp.timestamp >> 16) & 0xFF;
    rtppacket[6] = (Wc.rtp.timestamp >> 8) & 0xFF;
    rtppacket[7] = Wc.rtp.timestamp & 0xFF;

    // SSRC (Big Endian)
    rtppacket[8] = (Wc.rtp.ssrc >> 24) & 0xFF;
    rtppacket[9] = (Wc.rtp.ssrc >> 16) & 0xFF;
    rtppacket[10] = (Wc.rtp.ssrc >> 8) & 0xFF;
    rtppacket[11] = Wc.rtp.ssrc & 0xFF;

    // Copy NAL unit directly after RTP header
    memcpy(rtppacket + RTP_HEADER_SIZE, naldata, nallen);

    // Send UDP Packet
    Wc.rtp_udp.beginPacket(Wc.rtp_dest_ip, Wc.rtp.dest_port);
    Wc.rtp_udp.write(rtppacket, RTP_HEADER_SIZE + nallen);
    Wc.rtp_udp.endPacket();

    Wc.rtp.sequence++;
  } 
  // ---------------------------------------------------------
  // Case B: Large NAL - Fragment using FU-A (RFC 3984)
  // ---------------------------------------------------------
  else {
    size_t payloadsize = RTP_MAX_PAYLOAD - 2;  // Reserve 2 bytes for FU indicator & header
    size_t offset = 1;  // Skip the original NAL header (byte 0)
    bool firstfragment = true;

    while (offset < nallen) {
      // Calculate size of this chunk
      size_t chunksize = (nallen - offset > payloadsize) ? payloadsize : (nallen - offset);
      bool lastfragment = (offset + chunksize >= nallen);

      // Build RTP Header
      rtppacket[0] = 0x80;
      rtppacket[1] = 96; // PT=96

      // Set Marker Bit ONLY on the very last fragment of a VCL NAL
      if (lastfragment && (naltype == 1 || naltype == 5)) {
        rtppacket[1] |= 0x80; 
      }

      rtppacket[2] = (Wc.rtp.sequence >> 8) & 0xFF;
      rtppacket[3] = Wc.rtp.sequence & 0xFF;
      rtppacket[4] = (Wc.rtp.timestamp >> 24) & 0xFF;
      rtppacket[5] = (Wc.rtp.timestamp >> 16) & 0xFF;
      rtppacket[6] = (Wc.rtp.timestamp >> 8) & 0xFF;
      rtppacket[7] = Wc.rtp.timestamp & 0xFF;
      rtppacket[8] = (Wc.rtp.ssrc >> 24) & 0xFF;
      rtppacket[9] = (Wc.rtp.ssrc >> 16) & 0xFF;
      rtppacket[10] = (Wc.rtp.ssrc >> 8) & 0xFF;
      rtppacket[11] = Wc.rtp.ssrc & 0xFF;

      // --- FU-A Specific Headers ---
      // Byte 12: FU Indicator (F + NRI + Type 28)
      rtppacket[12] = (nalnri << 5) | 28;

      // Byte 13: FU Header (S + E + R + Original Type)
      rtppacket[13] = naltype & 0x1F; 
      
      if (firstfragment) {
        rtppacket[13] |= 0x80;  // Set S bit
      }
      if (lastfragment) {
        rtppacket[13] |= 0x40;  // Set E bit
      }

      // Copy payload fragment
      memcpy(rtppacket + 14, naldata + offset, chunksize);

      // Send UDP Packet
      Wc.rtp_udp.beginPacket(Wc.rtp_dest_ip, Wc.rtp.dest_port);
      Wc.rtp_udp.write(rtppacket, 14 + chunksize);
      Wc.rtp_udp.endPacket();

      // Traffic Shaping for ESP32 UDP stability (Reduces possible green artifacts)
      // delayMicroseconds(50);

      Wc.rtp.sequence++;
      offset += chunksize;
      firstfragment = false;
    }
  }
}

/*********************************************************************************************/
// H.264 Processing Task - Dedicated FreeRTOS task for H.264 encoding (RTP/WebRTC/Matter)

void H264ProcessingTask(void *pvParameters) {
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); // 100ms timeout
  uint32_t last_fps_calc = millis();
  uint32_t frames_in_second = 0;
  static uint32_t last_profile_log = 0;
  
  // H.264 encoder frame structures
  esp_h264_enc_in_frame_t in_frame = {};
  esp_h264_enc_out_frame_t out_frame = {};

  // Copy frame data to H.264 input buffer
  size_t h264_expected_size = Wc.core.config.width * Wc.core.config.height * 3 / 2; // 1.5 because YUV420
  // Protection: don't copy more than the destination can hold
  if (h264_expected_size > Wc.h264.buffer_size) h264_expected_size = Wc.h264.buffer_size;
  
  // Loop forever, exit only on CAM_STOPPING
  while (true) {
    // Wait here until ISR signals "Frame Ready"
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    
    // Exit on stop signal
    if (Wc.core.state == CAM_STOPPING) {
      break;
    }
    
    // Handle pause request - signal we're paused, then wait for resume
    if (Wc.core.state == CAM_PAUSING) {
      Wc.core.state = CAM_PAUSED;
      xSemaphoreTake(Wc.core.resume_sem, portMAX_DELAY);
      if (Wc.core.state == CAM_STOPPING) {
        break;
      }
      continue;
    }
    
    // Only process frames when streaming and we got a notification
    if (Wc.core.state != CAM_STREAMING || ulNotificationValue == 0) {
      continue;
    }
    
    // We have a frame!
    uint32_t frame_start = millis();
    
    // Lock frame access
    if (xSemaphoreTake(Wc.core.frame_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
      WcStats.frames_unsent++;
      continue;
    }
    
    // Check state after acquiring mutex
    if (Wc.core.state == CAM_STOPPING) {
      xSemaphoreGive(Wc.core.frame_mutex);
      break;
    }
    
    uint8_t *source_buf = Wc.core.frame_buffer[Wc.core.read_idx];
    
    // Cache Sync (Hardware M2C)
    esp_cache_msync(source_buf, Wc.core.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
    
    memcpy(Wc.h264.buffer, source_buf, h264_expected_size);
    
    xSemaphoreGive(Wc.core.frame_mutex);
    
    // Setup input frame
    in_frame.raw_data.buffer = Wc.h264.buffer;
    in_frame.raw_data.len = Wc.h264.buffer_size;
    in_frame.pts = WcStats.frames_captured;
    
    // Setup output frame buffer
    out_frame.raw_data.buffer = Wc.h264.out_buffer;
    out_frame.raw_data.len = Wc.h264.out_buffer_size;
    
    // Encode H.264
    uint32_t encode_start = micros();
    esp_h264_err_t ret = esp_h264_enc_process(Wc.h264.handle, &in_frame, &out_frame);
    uint32_t encode_time = micros() - encode_start;
    
    if (ret == ESP_H264_ERR_OK && out_frame.length > 0) {
      // Parse and send NAL units via RTP (only if RTSP session is in PLAY state)
      if (Wc.core.session_type == SESSION_RTSP && Wc.rtsp.streaming) {
        uint8_t* nal_data = Wc.h264.out_buffer;
        size_t remaining = out_frame.length;
        
        while (remaining > 0) {
          // Find start code (0x00000001 or 0x000001)
          size_t start_code_len = 0;
          if (remaining >= 4 && nal_data[0] == 0 && nal_data[1] == 0 && nal_data[2] == 0 && nal_data[3] == 1) {
            start_code_len = 4;
          } else if (remaining >= 3 && nal_data[0] == 0 && nal_data[1] == 0 && nal_data[2] == 1) {
            start_code_len = 3;
          } else {
            break; // No more NAL units
          }
          
          // Skip start code
          nal_data += start_code_len;
          remaining -= start_code_len;
          
          // Find next start code to determine NAL length (SAFE VERSION)
          size_t nal_length = remaining; // Default: take everything left as the last NAL
          
          for (size_t i = 0; i + 2 < remaining; i++) {
            // Check for 00 00 01 (3-byte start code)
            if (nal_data[i] == 0 && nal_data[i+1] == 0 && nal_data[i+2] == 1) {
              nal_length = i;
              break;
            }
            // Check for 00 00 00 01 (4-byte start code)
            // We need 4 bytes available to check this
            if (i + 3 < remaining && nal_data[i] == 0 && nal_data[i+1] == 0 && nal_data[i+2] == 0 && nal_data[i+3] == 1) {
              nal_length = i;
              break;
            }
          }
          
          // CRITICAL SAFETY: If length is 0 (start code at very beginning?), force skip to avoid infinite loop
          if (nal_length == 0 && remaining > 0) {
             // This implies malformed data or start code immediately found. 
             // Skip 1 byte to progress.
             nal_length = 1; 
          }
          
          // Extract NAL type
          uint8_t nal_header = nal_data[0];
          uint8_t nal_type = nal_header & 0x1F;
          uint8_t nal_nri = (nal_header >> 5) & 0x03;
          
          // Send NAL unit via RTP
          SendNalUnitRTP(nal_data, nal_length, nal_type, nal_nri);
          
          // Move to next NAL
          nal_data += nal_length;
          remaining -= nal_length;
        }
        
        // Increment RTP timestamp (90kHz clock)
        Wc.rtp.timestamp += (90000 / Wc.core.config.fps);
      }
      
      WcStats.frames_processed++;
      frames_in_second++;
      WcStats.bytes_sent += out_frame.length;
    }
    
    // Track frame processing time
    WcStats.last_frame_time_ms = millis() - frame_start;
    
    // Calculate FPS every second
    if (millis() - last_fps_calc >= 1000) {
      WcStats.last_fps = frames_in_second;
      frames_in_second = 0;
      last_fps_calc = millis();
      
      if (WcStats.start_time > 0) {
        WcStats.uptime_seconds = (millis() - WcStats.start_time) / 1000;
      }
    }
    
    // Periodic profiling log (every 5 seconds)
    if (millis() - last_profile_log >= 5000) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: H264 Profile - Encode:%uus Size:%u FPS:%u"),
        encode_time,
        out_frame.length,
        WcStats.last_fps);
      last_profile_log = millis();
    }
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: H264 task exited correctly"));
  // Task exiting - delete ourselves
  Wc.core.cam_task_handle = NULL;
  vTaskDelete(NULL);
}

/*********************************************************************************************/
// Encoder Setup Helper Functions

static uint32_t WcSetupJpegEncoder(void) {
  jpeg_encode_engine_cfg_t jpeg_eng_cfg = {
    .intr_priority = 0,
    .timeout_ms = 100,
  };
  
  esp_err_t ret = jpeg_new_encoder_engine(&jpeg_eng_cfg, &Wc.jpeg.handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: JPEG encoder init failed (0x%x)"), ret);
    return 0;
  }
  
  // Allocate JPEG output buffer
  jpeg_encode_memory_alloc_cfg_t jpeg_mem_cfg = {
    .buffer_direction = JPEG_ENC_ALLOC_OUTPUT_BUFFER,
  };
  
  size_t actual_size = 0;
  Wc.jpeg.buffer = jpeg_alloc_encoder_mem(Wc.core.config.width * Wc.core.config.height / 2, &jpeg_mem_cfg, &actual_size);
  if (!Wc.jpeg.buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate JPEG buffer"));
    jpeg_del_encoder_engine(Wc.jpeg.handle);
    return 0;
  }
  Wc.jpeg.buffer_size = actual_size;

  Wc.jpeg.cfg = {
    .height = Wc.core.config.height,
    .width = Wc.core.config.width,
    .src_type = JPEG_ENCODE_IN_FORMAT_YUV422,
    .sub_sample = JPEG_DOWN_SAMPLING_YUV422,
    .image_quality = Wc.jpeg.quality,
  };
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: JPEG encoder initialized, buffer=%d bytes"), actual_size);
  return 1;
}

static uint32_t WcSetupH264Encoder(void) {
  // Width/height must be 16-byte aligned (macroblock size)
  uint16_t width = ((Wc.core.config.width + 15) >> 4) << 4;
  uint16_t height = ((Wc.core.config.height + 15) >> 4) << 4;
  
  esp_h264_enc_cfg_hw_t cfg = {
    .pic_type = ESP_H264_RAW_FMT_O_UYY_E_VYY,  // HW encoder requires this YUV420 format
    .gop = 30,
    .fps = (uint8_t)(Wc.core.config.fps ? Wc.core.config.fps : 30),
    .res = {.width = width, .height = height},
    .rc = {
      .bitrate = (uint32_t)(width * height * 2),  // ~10% of raw size
      .qp_min = 15,
      .qp_max = 26
    }
  };
  
  // Allocate input frame buffer (raw YUV from ISP)
  size_t in_size = (size_t)((float)width * height * ESP_H264_GET_BPP_BY_PIC_TYPE(cfg.pic_type));
  Wc.h264.buffer = (uint8_t*)esp_h264_aligned_calloc(16, 1, in_size, &in_size, ESP_H264_MEM_SPIRAM);
  if (!Wc.h264.buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 input buffer alloc failed"));
    return 0;
  }
  Wc.h264.buffer_size = in_size;
  
  // Allocate output frame buffer (encoded NAL units) - same size as input
  size_t out_size = in_size;
  Wc.h264.out_buffer = (uint8_t*)esp_h264_aligned_calloc(16, 1, out_size, &out_size, ESP_H264_MEM_SPIRAM);
  if (!Wc.h264.out_buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 output buffer alloc failed"));
    esp_h264_free(Wc.h264.buffer);
    Wc.h264.buffer = NULL;
    return 0;
  }
  Wc.h264.out_buffer_size = out_size;
  
  // Create encoder
  esp_h264_err_t ret = esp_h264_enc_hw_new(&cfg, &Wc.h264.handle);
  if (ret != ESP_H264_ERR_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 encoder create failed (0x%x)"), ret);
    esp_h264_free(Wc.h264.buffer);
    esp_h264_free(Wc.h264.out_buffer);
    Wc.h264.buffer = NULL;
    Wc.h264.out_buffer = NULL;
    return 0;
  }
  
  // Open encoder
  ret = esp_h264_enc_open(Wc.h264.handle);
  if (ret != ESP_H264_ERR_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 encoder open failed (0x%x)"), ret);
    esp_h264_enc_del(Wc.h264.handle);
    esp_h264_free(Wc.h264.buffer);
    esp_h264_free(Wc.h264.out_buffer);
    Wc.h264.handle = NULL;
    Wc.h264.buffer = NULL;
    Wc.h264.out_buffer = NULL;
    return 0;
  }
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: H.264 encoder initialized (%dx%d, buffer=%d bytes)"), width, height, in_size);
  return 1;
}
/*********************************************************************************************/

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


/*********************************************************************************************/

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

void WcRtspStop(void) {
  if (Wc.rtsp.server) {
    Wc.rtsp.server->stop();
    delete Wc.rtsp.server;
    Wc.rtsp.server = NULL;
  }
  if (Wc.rtsp_client) {
    Wc.rtsp_client.stop();
  }
  Wc.rtsp.streaming = false;
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
// RTSP Server Handler

// Helper to send RTSP responses
void RtspResp(uint32_t cseq, const char* extra = nullptr, const char* body = nullptr) {
  // Use _P for PSTR format string
  Wc.rtsp_client.printf_P(PSTR("RTSP/1.0 200 OK\r\nCSeq: %u\r\n"), cseq);
  
  if (extra) Wc.rtsp_client.print(extra); // extra is already a char* (likely from PSTR)
  
  if (body) {
    Wc.rtsp_client.printf_P(PSTR("Content-Length: %u\r\n\r\n%s"), strlen(body), body);
  } else {
    Wc.rtsp_client.print(PSTR("\r\n"));
  }
}

void HandleRtsp() {
  if (!Wc.rtsp.server) return;

  // 1. Connection Management
  if (Wc.rtsp.server->hasClient()) {
    if (Wc.rtsp_client && Wc.rtsp_client.connected()) {
      Wc.rtsp.server->available().stop(); // Reject busy
    } else {
      Wc.rtsp_client = Wc.rtsp.server->available();
      AddLog(LOG_LEVEL_INFO, PSTR("RTSP: Client connected"));
    }
  }

  if (!Wc.rtsp_client || !Wc.rtsp_client.connected()) {
    if (Wc.rtsp.streaming) Wc.rtsp.streaming = false;
    return;
  }

  // 2. Request Handling
  if (!Wc.rtsp_client.available()) return;

  String req = Wc.rtsp_client.readStringUntil('\n');
  req.trim();
  if (req.length() == 0) return;

  AddLog(LOG_LEVEL_DEBUG, PSTR("RTSP: %s"), req.c_str());

  // Header Parsing
  uint32_t cseq = 0;
  int client_port = 0;
  
  while (Wc.rtsp_client.available()) {
    String h = Wc.rtsp_client.readStringUntil('\n');
    h.trim();
    if (h.length() == 0) break;
    
    if (h.startsWith(PSTR("CSeq:"))) {
      cseq = h.substring(5).toInt();
    }
    if (h.startsWith(PSTR("Transport:"))) {
      int idx = h.indexOf(PSTR("client_port="));
      if (idx > 0) client_port = h.substring(idx + 12).toInt();
    }
  }

  // 3. Command Dispatch
  if (req.indexOf(PSTR("OPTIONS")) >= 0) {
    RtspResp(cseq, PSTR("Public: OPTIONS, DESCRIBE, SETUP, PLAY, TEARDOWN\r\n"));
  } 
  else if (req.indexOf(PSTR("DESCRIBE")) >= 0) {
    char sdp[256];
    IPAddress ip = WiFi.localIP();
    snprintf_P(sdp, sizeof(sdp), 
      PSTR("v=0\r\no=- 0 0 IN IP4 %s\r\ns=Tasmota Stream\r\nc=IN IP4 0.0.0.0\r\nt=0 0\r\nm=video 0 RTP/AVP 96\r\na=rtpmap:96 H264/90000\r\na=fmtp:96 packetization-mode=1;profile-level-id=42001E\r\na=control:track0\r\n"),
      ip.toString().c_str());
      
    RtspResp(cseq, PSTR("Content-Type: application/sdp\r\n"), sdp);
  } 
  else if (req.indexOf(PSTR("SETUP")) >= 0) {
    if (client_port > 0) {
      Wc.rtsp.client_rtp_port = client_port;
      Wc.rtsp.client_rtcp_port = client_port + 1;
      Wc.rtp_dest_ip = Wc.rtsp_client.remoteIP();
      Wc.rtp.dest_port = client_port;
      Wc.rtsp.session_id = random(100000, 999999);
      
      char trans[128];
      snprintf_P(trans, sizeof(trans), 
        PSTR("Session: %u\r\nTransport: RTP/AVP;unicast;client_port=%u-%u;server_port=%u-%u\r\n"),
        Wc.rtsp.session_id, Wc.rtsp.client_rtp_port, Wc.rtsp.client_rtcp_port, Wc.rtp.dest_port, Wc.rtp.dest_port+1);
      
      RtspResp(cseq, trans);
    }
  } 
  else if (req.indexOf(PSTR("PLAY")) >= 0) {
    Wc.rtsp.streaming = true;
    char sess[64];
    snprintf_P(sess, sizeof(sess), PSTR("Session: %u\r\nRange: npt=0.000-\r\n"), Wc.rtsp.session_id);
    RtspResp(cseq, sess);
  } 
  else if (req.indexOf(PSTR("TEARDOWN")) >= 0) {
    Wc.rtsp.streaming = false;
    char sess[64];
    snprintf_P(sess, sizeof(sess), PSTR("Session: %u\r\n"), Wc.rtsp.session_id);
    RtspResp(cseq, sess);
    Wc.rtsp_client.stop();
  }
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
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Root called - CamServer=%p"), Wc.jpeg.server);
  if (!Wc.jpeg.server) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CamServer is NULL in Root!"));
    return;
  }
  Wc.jpeg.server->sendHeader("Location", "/cam.mjpeg");
  Wc.jpeg.server->send(302, "", "");
}

void HandleWebcamMjpeg(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Handle camserver - state=%d"), Wc.core.state);
  
  if (!Wc.jpeg.server) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CamServer is NULL!"));
    return;
  }
  
  if (Wc.core.state != CAM_STREAMING) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Not streaming - rejecting request"));
    Wc.jpeg.server->send(503, "text/plain", "Camera not ready");
    return;
  }
  
  // Lock frame access for client setup
  if (xSemaphoreTake(Wc.core.frame_mutex, pdMS_TO_TICKS(200)) == pdTRUE) {
    // Allocate client on heap to avoid stack issues
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Allocating client..."));
    if (Wc.jpeg.client_ptr) {
      delete Wc.jpeg.client_ptr;
      Wc.jpeg.client_ptr = nullptr;
    }
    
    Wc.jpeg.client_ptr = new WiFiClient();
    if (!Wc.jpeg.client_ptr) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate client!"));
      xSemaphoreGive(Wc.core.frame_mutex);
      return;
    }
    
    *Wc.jpeg.client_ptr = Wc.jpeg.server->client();
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client allocated, connected=%d"), Wc.jpeg.client_ptr->connected());
    
    // Send HTTP header
    Wc.jpeg.client_ptr->print("HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n\r\n");
    Wc.jpeg.stream_active = 2;
    
    xSemaphoreGive(Wc.core.frame_mutex);
  }
}

void HandleImage(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  WiFiClient client = Webserver->client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.jpg\r\n";
  response += "Content-type: image/jpeg\r\n\r\n";
  Webserver->sendContent(response);

  if (!Wc.jpeg.handle || !Wc.jpeg.buffer) {
    client.stop();
    return;
  }

  // Wait for next frame (simple delay to let task process)
  delay(100);
  
  // Lock JPEG encoder to prevent race with CamProcessingTask
  if (xSemaphoreTake(Wc.jpeg.mutex, pdMS_TO_TICKS(200)) == pdTRUE) {
    uint8_t *source_buf = Wc.core.frame_buffer[Wc.core.read_idx];
    esp_cache_msync(source_buf, Wc.core.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);

    uint32_t jpeg_size = 0;
    esp_err_t ret = jpeg_encoder_process(Wc.jpeg.handle, &Wc.jpeg.cfg, source_buf, Wc.core.frame_buffer_size, (uint8_t*)Wc.jpeg.buffer, Wc.jpeg.buffer_size, &jpeg_size);
    if (ret == ESP_OK && jpeg_size > 0) {
      client.write((char *)Wc.jpeg.buffer, jpeg_size);
    }
    
    xSemaphoreGive(Wc.jpeg.mutex);
  }
  
  client.stop();
}


uint32_t WcSetStreamserver(uint32_t flag) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: WcSetStreamserver flag=%d CamServer=%p"), flag, Wc.jpeg.server);
  
  if (TasmotaGlobal.global_state.network_down) { 
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Network down, aborting"));
    Wc.jpeg.stream_active = 0;
    return 0; 
  }

  if (flag) {
    if (!Wc.jpeg.server) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: Creating stream server on port 81..."));
      Wc.jpeg.stream_active = 0;
      Wc.jpeg.server = new ESP8266WebServer(81);
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: CamServer created at %p"), Wc.jpeg.server);
      
      if (!Wc.jpeg.server) {
        AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate CamServer!"));
        return 0;
      }
      
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Registering handlers..."));
      Wc.jpeg.server->on("/", HandleWebcamRoot);
      Wc.jpeg.server->on("/cam.mjpeg", HandleWebcamMjpeg);
      Wc.jpeg.server->on("/cam.jpg", HandleWebcamMjpeg);
      Wc.jpeg.server->on("/stream", HandleWebcamMjpeg);
      
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Starting server..."));
      Wc.jpeg.server->begin();
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stream server started on port 81"));
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stream server already running"));
    }
  } else {
    if (Wc.jpeg.server) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopping stream server..."));
      Wc.jpeg.stream_active = 0;
      Wc.jpeg.server->stop();
      delete Wc.jpeg.server;
      Wc.jpeg.server = NULL;
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
  if (Wc.jpeg.server && Wc.core.state == CAM_STREAMING) {
    uint32_t ip = (uint32_t)WiFi.localIP();
    
    // Container and Status Label
    WSContentSend_P(PSTR("<div><div id='wc_s'>Loading...</div>"));
    
    // Image with ID 'wc_img'
    WSContentSend_P(PSTR("<img id='wc_img' style='max-width:100%%;' "
                         "src='http://%_I:81/stream' "
                         "onerror='setTimeout(()=>{this.src=this.src;},1000)'>"), ip);

    // JS using Tasmota's eb() helper
    WSContentSend_P(PSTR(
      "<script>"
      "setInterval(function(){"
      "  var i=eb('wc_img');"
      "  if(i && i.naturalWidth){"
      "    eb('wc_s').innerHTML='MJPEG: '+i.naturalWidth+'x'+i.naturalHeight;"
      "  }"
      "},1000);"
      "</script></div>"));
  }
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
    // Optional: If Tasmota asks for JSON (e.g. /cm?cmnd=status 10), appends to response
    // But usually FUNC_WEB_SENSOR passes json=false for the Main Page HTML
    return;
  }

  // 1. Determine Session Name
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

  // 2. Send Table Rows to Web Page
  // Mode
  WSContentSend_PD(HTTP_WC_MODE, mode_str);

  // Resolution
  WSContentSend_PD(HTTP_WC_RES, Wc.core.config.width, Wc.core.config.height);

  // FPS (Only show if streaming)
  if (Wc.core.state == CAM_STREAMING) {
    WSContentSend_PD(HTTP_WC_FPS, WcStats.last_fps);
  }
  AddLog(LOG_LEVEL_INFO, PSTR("_"));
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

#endif  // USE_WEBCAM_CSI
#endif  // ESP32P4
