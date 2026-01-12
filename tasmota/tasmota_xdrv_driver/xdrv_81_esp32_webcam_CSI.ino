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
  SESSION_RTP,         // H.264 over RTP/UDP
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
  // === Camera Core (shared by all session types) ===
  esp_cam_ctlr_handle_t cam_handle;
  isp_proc_handle_t isp_handle;
  esp_ldo_channel_handle_t ldo_mipi_phy;
  
  uint8_t *frame_buffer[2];        // Double-buffered ping-pong
  size_t frame_buffer_size;
  
  CSI_Config config;               // Sensor configuration from Berry
  esp_cam_ctlr_trans_t cam_trans;  // CSI transaction struct
  
  volatile int write_idx;          // Buffer CSI is writing to (0 or 1)
  volatile int read_idx;           // Buffer ready for reading
  
  volatile camera_state_t state;   // Pipeline lifecycle state
  camera_session_t session_type;   // Active session type
  
  TaskHandle_t cam_task_handle;    // Frame processing task
  SemaphoreHandle_t frame_mutex;   // Protects frame buffer access
  SemaphoreHandle_t resume_sem;    // Task pause/resume sync
  
  // === MJPEG Session (HTTP streaming, snapshots) ===
  jpeg_encoder_handle_t jpeg_handle;
  void *jpeg_buffer;
  size_t jpeg_buffer_size;
  jpeg_encode_cfg_t jpeg_cfg;
  uint8_t jpeg_quality;            // 1-100, default 50
  
  SemaphoreHandle_t jpeg_mutex;    // Protects JPEG encoder
  
  ESP8266WebServer *CamServer;     // HTTP stream server (port 81)
  WiFiClient *client_ptr;          // Active streaming client
  uint8_t stream_active;           // 0=none, 2=streaming
  
  // === RTP Session (H.264 over UDP) ===
  esp_h264_enc_handle_t h264_handle;  // H.264 hardware encoder
  uint8_t *h264_buffer;            // H.264 input buffer (raw YUV from ISP)
  size_t h264_buffer_size;
  uint8_t *h264_out_buffer;        // H.264 output buffer (encoded NAL units)
  size_t h264_out_buffer_size;
  
  WiFiUDP rtp_udp;                 // UDP socket for RTP
  IPAddress rtp_dest_ip;           // Destination IP
  uint16_t rtp_dest_port;          // Destination port (default 5004)
  uint16_t rtp_sequence;           // RTP sequence number (increments per packet)
  uint32_t rtp_timestamp;          // RTP timestamp (90kHz clock)
  uint32_t rtp_ssrc;               // Synchronization source ID
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
  int next_idx = (Wc.write_idx + 1) % 2;
  Wc.write_idx = next_idx;
  
  // Give hardware the address of the new write buffer
  trans->buffer = Wc.frame_buffer[next_idx];
  trans->buflen = Wc.frame_buffer_size;
  
  return false; 
}

// Callback: Frame transfer finished - Wake processing task
static bool IRAM_ATTR csi_on_trans_finished(esp_cam_ctlr_handle_t handle, esp_cam_ctlr_trans_t *trans, void *user_data) {
  // Reject if not streaming (shutdown in progress)
  if (Wc.state != CAM_STREAMING) return false;
  
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
    if (Wc.state == CAM_STOPPING) {
      break;
    }
    
    // Handle pause request - signal we're paused, then wait for resume
    if (Wc.state == CAM_PAUSING) {
      Wc.state = CAM_PAUSED;  // Signal that we're safely paused
      // Wait for resume signal (blocks until semaphore given)
      xSemaphoreTake(Wc.resume_sem, portMAX_DELAY);
      // After resume, check state again
      if (Wc.state == CAM_STOPPING) {
        break;
      }
      continue;  // Go back to top of loop
    }
    
    // Only process frames when streaming and we got a notification
    if (Wc.state != CAM_STREAMING || ulNotificationValue == 0) {
      continue;
    }
    
    // We have a frame!
    uint32_t frame_start = millis();
    
    // Lock frame access to prevent race with WcLoop
    uint32_t mutex_start = micros();
    if (xSemaphoreTake(Wc.frame_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
      WcStats.frames_unsent++;
      continue;
    }
    
    WcStats.last_mutex_wait_us = micros() - mutex_start;
    if (WcStats.last_mutex_wait_us > WcStats.max_mutex_wait_us) {
      WcStats.max_mutex_wait_us = WcStats.last_mutex_wait_us;
    }
    
    // Check state after acquiring mutex - exit quickly if stopping
    if (Wc.state == CAM_STOPPING) {
      xSemaphoreGive(Wc.frame_mutex);
      break;
    }
    
    // Only process if we have a client connected to stream
    if (Wc.stream_active != 2 || !Wc.client_ptr || !Wc.client_ptr->connected()) {
      WcStats.frames_unsent++;
      xSemaphoreGive(Wc.frame_mutex);
      continue;
    }
    
    uint8_t *source_buf = Wc.frame_buffer[Wc.read_idx];
    
    // Cache Sync (Hardware M2C)
    uint32_t cache_start = micros();
    esp_cache_msync(source_buf, Wc.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
    WcStats.last_cache_sync_us = micros() - cache_start;
    
    // Lock JPEG encoder to prevent race with HandleImage
    uint32_t jpeg_mutex_start = micros();
    if (xSemaphoreTake(Wc.jpeg_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
      WcStats.frames_unsent++;
      xSemaphoreGive(Wc.frame_mutex);
      continue;
    }
    
    // Check state after acquiring mutex - exit quickly if stopping
    if (Wc.state == CAM_STOPPING) {
      xSemaphoreGive(Wc.jpeg_mutex);
      xSemaphoreGive(Wc.frame_mutex);
      break;
    }
    
    WcStats.last_jpeg_mutex_wait_us = micros() - jpeg_mutex_start;
    
    uint32_t jpeg_size = 0;
    uint32_t jpeg_start = micros();
    esp_err_t ret = jpeg_encoder_process(Wc.jpeg_handle, 
                                          &Wc.jpeg_cfg,
                                          source_buf, 
                                          Wc.frame_buffer_size,
                                          (uint8_t*)Wc.jpeg_buffer, 
                                          Wc.jpeg_buffer_size,
                                          &jpeg_size);
    WcStats.last_jpeg_encode_us = micros() - jpeg_start;
    if (WcStats.last_jpeg_encode_us > WcStats.max_jpeg_encode_us) {
      WcStats.max_jpeg_encode_us = WcStats.last_jpeg_encode_us;
    }
    
    // Auto-Reset on 0x103 (Engine Stuck)
    if (ret == ESP_ERR_INVALID_STATE) {
      jpeg_del_encoder_engine(Wc.jpeg_handle);
      jpeg_encode_engine_cfg_t jpeg_eng_cfg = { .intr_priority = 0, .timeout_ms = 100 };
      jpeg_new_encoder_engine(&jpeg_eng_cfg, &Wc.jpeg_handle);
      WcStats.jpeg_resets++;
      xSemaphoreGive(Wc.jpeg_mutex);
      xSemaphoreGive(Wc.frame_mutex);
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
      uint32_t raw_size = Wc.frame_buffer_size;
      WcStats.compression_ratio_x100 = (raw_size * 100) / jpeg_size;
      
      // Check state before starting network write - don't start new writes if stopping
      if (Wc.state == CAM_STOPPING) {
        xSemaphoreGive(Wc.jpeg_mutex);
        xSemaphoreGive(Wc.frame_mutex);
        break;
      }
      
      uint32_t network_start = micros();
      Wc.client_ptr->print("--" BOUNDARY "\r\n");
      Wc.client_ptr->printf("Content-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n", jpeg_size);
      Wc.client_ptr->write((const uint8_t*)Wc.jpeg_buffer, jpeg_size);
      Wc.client_ptr->print("\r\n");
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
    
    xSemaphoreGive(Wc.jpeg_mutex);
    xSemaphoreGive(Wc.frame_mutex);
    
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
  Wc.cam_task_handle = NULL;
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
    rtppacket[2] = (Wc.rtp_sequence >> 8) & 0xFF;
    rtppacket[3] = Wc.rtp_sequence & 0xFF;

    // Timestamp (Big Endian)
    rtppacket[4] = (Wc.rtp_timestamp >> 24) & 0xFF;
    rtppacket[5] = (Wc.rtp_timestamp >> 16) & 0xFF;
    rtppacket[6] = (Wc.rtp_timestamp >> 8) & 0xFF;
    rtppacket[7] = Wc.rtp_timestamp & 0xFF;

    // SSRC (Big Endian)
    rtppacket[8] = (Wc.rtp_ssrc >> 24) & 0xFF;
    rtppacket[9] = (Wc.rtp_ssrc >> 16) & 0xFF;
    rtppacket[10] = (Wc.rtp_ssrc >> 8) & 0xFF;
    rtppacket[11] = Wc.rtp_ssrc & 0xFF;

    // Copy NAL unit directly after RTP header
    memcpy(rtppacket + RTP_HEADER_SIZE, naldata, nallen);

    // Send UDP Packet
    Wc.rtp_udp.beginPacket(Wc.rtp_dest_ip, Wc.rtp_dest_port);
    Wc.rtp_udp.write(rtppacket, RTP_HEADER_SIZE + nallen);
    Wc.rtp_udp.endPacket();

    Wc.rtp_sequence++;
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

      rtppacket[2] = (Wc.rtp_sequence >> 8) & 0xFF;
      rtppacket[3] = Wc.rtp_sequence & 0xFF;
      rtppacket[4] = (Wc.rtp_timestamp >> 24) & 0xFF;
      rtppacket[5] = (Wc.rtp_timestamp >> 16) & 0xFF;
      rtppacket[6] = (Wc.rtp_timestamp >> 8) & 0xFF;
      rtppacket[7] = Wc.rtp_timestamp & 0xFF;
      rtppacket[8] = (Wc.rtp_ssrc >> 24) & 0xFF;
      rtppacket[9] = (Wc.rtp_ssrc >> 16) & 0xFF;
      rtppacket[10] = (Wc.rtp_ssrc >> 8) & 0xFF;
      rtppacket[11] = Wc.rtp_ssrc & 0xFF;

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
      Wc.rtp_udp.beginPacket(Wc.rtp_dest_ip, Wc.rtp_dest_port);
      Wc.rtp_udp.write(rtppacket, 14 + chunksize);
      Wc.rtp_udp.endPacket();

      // Traffic Shaping for ESP32 UDP stability (Reduces possible green artifacts)
      delayMicroseconds(50);

      Wc.rtp_sequence++;
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
  
  // Loop forever, exit only on CAM_STOPPING
  while (true) {
    // Wait here until ISR signals "Frame Ready"
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    
    // Exit on stop signal
    if (Wc.state == CAM_STOPPING) {
      break;
    }
    
    // Handle pause request - signal we're paused, then wait for resume
    if (Wc.state == CAM_PAUSING) {
      Wc.state = CAM_PAUSED;
      xSemaphoreTake(Wc.resume_sem, portMAX_DELAY);
      if (Wc.state == CAM_STOPPING) {
        break;
      }
      continue;
    }
    
    // Only process frames when streaming and we got a notification
    if (Wc.state != CAM_STREAMING || ulNotificationValue == 0) {
      continue;
    }
    
    // We have a frame!
    uint32_t frame_start = millis();
    
    // Lock frame access
    if (xSemaphoreTake(Wc.frame_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
      WcStats.frames_unsent++;
      continue;
    }
    
    // Check state after acquiring mutex
    if (Wc.state == CAM_STOPPING) {
      xSemaphoreGive(Wc.frame_mutex);
      break;
    }
    
    uint8_t *source_buf = Wc.frame_buffer[Wc.read_idx];
    
    // Cache Sync (Hardware M2C)
    esp_cache_msync(source_buf, Wc.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
    
    // Copy frame data to H.264 input buffer
    memcpy(Wc.h264_buffer, source_buf, Wc.frame_buffer_size);
    
    xSemaphoreGive(Wc.frame_mutex);
    
    // Setup input frame
    in_frame.raw_data.buffer = Wc.h264_buffer;
    in_frame.raw_data.len = Wc.h264_buffer_size;
    in_frame.pts = WcStats.frames_captured;
    
    // Setup output frame buffer
    out_frame.raw_data.buffer = Wc.h264_out_buffer;
    out_frame.raw_data.len = Wc.h264_out_buffer_size;
    
    // Encode H.264
    uint32_t encode_start = micros();
    esp_h264_err_t ret = esp_h264_enc_process(Wc.h264_handle, &in_frame, &out_frame);
    uint32_t encode_time = micros() - encode_start;
    
    if (ret == ESP_H264_ERR_OK && out_frame.length > 0) {
      // Parse and send NAL units via RTP
      if (Wc.session_type == SESSION_RTP) {
        uint8_t* nal_data = Wc.h264_out_buffer;
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
          
          // Find next start code to determine NAL length
          size_t nal_length = 0;
          for (size_t i = 0; i < remaining - 2; i++) {
            if (i >= 2 && nal_data[i] == 0 && nal_data[i+1] == 0 && (nal_data[i+2] == 0 || nal_data[i+2] == 1)) {
              nal_length = (nal_data[i+2] == 1) ? i : i + 1;
              break;
            }
          }
          if (nal_length == 0) nal_length = remaining; // Last NAL
          
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
        Wc.rtp_timestamp += (90000 / Wc.config.fps);
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
  
  // Task exiting - delete ourselves
  Wc.cam_task_handle = NULL;
  vTaskDelete(NULL);
}

/*********************************************************************************************/
// Encoder Setup Helper Functions

static uint32_t WcSetupJpegEncoder(void) {
  jpeg_encode_engine_cfg_t jpeg_eng_cfg = {
    .intr_priority = 0,
    .timeout_ms = 100,
  };
  
  esp_err_t ret = jpeg_new_encoder_engine(&jpeg_eng_cfg, &Wc.jpeg_handle);
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
    .src_type = JPEG_ENCODE_IN_FORMAT_YUV422,
    .sub_sample = JPEG_DOWN_SAMPLING_YUV422,
    .image_quality = Wc.jpeg_quality,
  };
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: JPEG encoder initialized, buffer=%d bytes"), actual_size);
  return 1;
}

static uint32_t WcSetupH264Encoder(void) {
  // Width/height must be 16-byte aligned (macroblock size)
  uint16_t width = ((Wc.config.width + 15) >> 4) << 4;
  uint16_t height = ((Wc.config.height + 15) >> 4) << 4;
  
  esp_h264_enc_cfg_hw_t cfg = {
    .pic_type = ESP_H264_RAW_FMT_O_UYY_E_VYY,  // HW encoder requires this YUV420 format
    .gop = 30,
    .fps = (uint8_t)(Wc.config.fps ? Wc.config.fps : 30),
    .res = {.width = width, .height = height},
    .rc = {
      .bitrate = (uint32_t)(width * height / 10),  // ~10% of raw size
      .qp_min = 26,
      .qp_max = 30
    }
  };
  
  // Allocate input frame buffer (raw YUV from ISP)
  size_t in_size = (size_t)((float)width * height * ESP_H264_GET_BPP_BY_PIC_TYPE(cfg.pic_type));
  Wc.h264_buffer = (uint8_t*)esp_h264_aligned_calloc(16, 1, in_size, &in_size, ESP_H264_MEM_SPIRAM);
  if (!Wc.h264_buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 input buffer alloc failed"));
    return 0;
  }
  Wc.h264_buffer_size = in_size;
  
  // Allocate output frame buffer (encoded NAL units) - same size as input
  size_t out_size = in_size;
  Wc.h264_out_buffer = (uint8_t*)esp_h264_aligned_calloc(16, 1, out_size, &out_size, ESP_H264_MEM_SPIRAM);
  if (!Wc.h264_out_buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 output buffer alloc failed"));
    esp_h264_free(Wc.h264_buffer);
    Wc.h264_buffer = NULL;
    return 0;
  }
  Wc.h264_out_buffer_size = out_size;
  
  // Create encoder
  esp_h264_err_t ret = esp_h264_enc_hw_new(&cfg, &Wc.h264_handle);
  if (ret != ESP_H264_ERR_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 encoder create failed (0x%x)"), ret);
    esp_h264_free(Wc.h264_buffer);
    esp_h264_free(Wc.h264_out_buffer);
    Wc.h264_buffer = NULL;
    Wc.h264_out_buffer = NULL;
    return 0;
  }
  
  // Open encoder
  ret = esp_h264_enc_open(Wc.h264_handle);
  if (ret != ESP_H264_ERR_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 encoder open failed (0x%x)"), ret);
    esp_h264_enc_del(Wc.h264_handle);
    esp_h264_free(Wc.h264_buffer);
    esp_h264_free(Wc.h264_out_buffer);
    Wc.h264_handle = NULL;
    Wc.h264_buffer = NULL;
    Wc.h264_out_buffer = NULL;
    return 0;
  }
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: H.264 encoder initialized (%dx%d, buffer=%d bytes)"), width, height, in_size);
  return 1;
}

/*********************************************************************************************/

uint32_t WcSetup(bool reset_config) {
  if (Wc.state != CAM_IDLE) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI already initialized (state=%d)"), Wc.state);
    return 1;
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
  // RTP session needs YUV420 for H.264 encoder, MJPEG uses YUV422
  cam_ctlr_color_t csi_output_format = (Wc.session_type == SESSION_RTP) ? CAM_CTLR_COLOR_YUV420 : CAM_CTLR_COLOR_YUV422;
  
  esp_cam_ctlr_csi_config_t csi_config = {
    .ctlr_id = 0,
    .h_res = Wc.config.width,
    .v_res = Wc.config.height,
    .data_lane_num = Wc.config.lane_num,
    .lane_bit_rate_mbps = (int)Wc.config.mipi_clock,
    .input_data_color_type = (cam_ctlr_color_t)COLOR_TYPE_ID(COLOR_SPACE_RAW, (color_pixel_raw_format_t)Wc.config.format),
    .output_data_color_type = csi_output_format,
    .queue_items = 1,
    .byte_swap_en = false,
  };
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI output format: %s"), 
    (csi_output_format == CAM_CTLR_COLOR_YUV420) ? "YUV420" : "YUV422");

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
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI controller enabled (not started yet)"));

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
    // RTP session needs YUV420 for H.264 encoder, MJPEG uses YUV422
    isp_color_t isp_output_format = (Wc.session_type == SESSION_RTP) ? ISP_COLOR_YUV420 : ISP_COLOR_YUV422;
    
    esp_isp_processor_cfg_t isp_config = {
      .clk_hz = 120 * 1000 * 1000, //TODO: eventually calculate this based on configured AV session
      .input_data_source = ISP_INPUT_DATA_SOURCE_CSI,
      .input_data_color_type = (isp_color_t)COLOR_TYPE_ID(COLOR_SPACE_RAW, (color_pixel_raw_format_t)Wc.config.format),
      .output_data_color_type = isp_output_format,
      .h_res = Wc.config.width,
      .v_res = Wc.config.height,
    };
    
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: ISP output format: %s"), 
      (isp_output_format == ISP_COLOR_YUV420) ? "YUV420" : "YUV422");

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

  // 8. Initialize encoder based on session type
  if (Wc.session_type == SESSION_MJPEG_HTTP) {
    if (!WcSetupJpegEncoder()) {
      esp_isp_disable(Wc.isp_handle);
      esp_isp_del_processor(Wc.isp_handle);
      esp_cam_ctlr_disable(Wc.cam_handle);
      esp_cam_ctlr_del(Wc.cam_handle);
      Wc.cam_handle = NULL;
      Wc.isp_handle = NULL;
      return 0;
    }
  } else if (Wc.session_type == SESSION_RTP) {
    if (!WcSetupH264Encoder()) {
      esp_isp_disable(Wc.isp_handle);
      esp_isp_del_processor(Wc.isp_handle);
      esp_cam_ctlr_disable(Wc.cam_handle);
      esp_cam_ctlr_del(Wc.cam_handle);
      Wc.cam_handle = NULL;
      Wc.isp_handle = NULL;
      return 0;
    }
  }

  // 9. Create mutexes for thread safety
  Wc.frame_mutex = xSemaphoreCreateMutex();
  Wc.jpeg_mutex = xSemaphoreCreateMutex();
  Wc.resume_sem = xSemaphoreCreateBinary();  // For pause/resume
  
  if (!Wc.frame_mutex || !Wc.jpeg_mutex || !Wc.resume_sem) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to create mutexes"));
    // Cleanup
    if (Wc.frame_mutex) vSemaphoreDelete(Wc.frame_mutex);
    if (Wc.jpeg_mutex) vSemaphoreDelete(Wc.jpeg_mutex);
    if (Wc.resume_sem) vSemaphoreDelete(Wc.resume_sem);
    jpeg_del_encoder_engine(Wc.jpeg_handle);
    esp_isp_disable(Wc.isp_handle);
    esp_isp_del_processor(Wc.isp_handle);
    esp_cam_ctlr_stop(Wc.cam_handle);
    esp_cam_ctlr_disable(Wc.cam_handle);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Mutexes created"));

  // 10. Create processing task based on session type
  TaskFunction_t task_func;
  const char *task_name;
  
  if (Wc.session_type == SESSION_MJPEG_HTTP) {
    task_func = MjpegProcessingTask;
    task_name = "MjpegTask";
  } else {
    task_func = H264ProcessingTask;
    task_name = "H264Task";
  }
  
  BaseType_t task_created = xTaskCreatePinnedToCore(
    task_func,
    task_name,
    4096,
    NULL,
    5,
    &Wc.cam_task_handle,
    1  // Pin to Core 1
  );
  
  if (task_created != pdPASS) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to create processing task"));
    vSemaphoreDelete(Wc.frame_mutex);
    vSemaphoreDelete(Wc.jpeg_mutex);
    vSemaphoreDelete(Wc.resume_sem);
    jpeg_del_encoder_engine(Wc.jpeg_handle);
    esp_isp_disable(Wc.isp_handle);
    esp_isp_del_processor(Wc.isp_handle);
    esp_cam_ctlr_disable(Wc.cam_handle);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Processing task created"));

  Wc.state = CAM_INIT;
  // session_type is already set by caller (WcSession command or default)

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Setup complete (session=%d)"), Wc.session_type);
  return 1;
}

uint32_t WcStart(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: WcStart called - state=%d"), Wc.state);
  
  if (Wc.state != CAM_INIT) {
    if (Wc.state == CAM_STREAMING) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Already streaming"));
      return 1;
    }
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Cannot start from state %d"), Wc.state);
    return 0;
  }

  // Reset statistics
  memset(&WcStats, 0, sizeof(WcStats));
  WcStats.start_time = millis();

  // Start CSI controller (ISR callbacks will begin firing)
  esp_err_t ret = esp_cam_ctlr_start(Wc.cam_handle);
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
    esp_cam_ctlr_stop(Wc.cam_handle);
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Sensor streaming started"));
  
  // Give sensor time to start streaming
  delay(100);

  Wc.state = CAM_STREAMING;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Streaming active"));
  return 1;
}

// Pause the task for reconfiguration - task stays alive but stops processing
uint32_t WcPause(void) {
  if (Wc.state != CAM_STREAMING) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Cannot pause from state %d"), Wc.state);
    return 0;
  }
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Pausing task"));
  
  // Signal task to pause
  Wc.state = CAM_PAUSING;
  
  // Wake task so it sees the state change
  xTaskNotifyGive(Wc.cam_task_handle);
  
  // Wait for task to acknowledge pause (max 500ms)
  for (int i = 0; i < 50 && Wc.state != CAM_PAUSED; i++) {
    delay(10);
  }
  
  if (Wc.state != CAM_PAUSED) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Task didn't pause"));
    return 0;
  }
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Task paused"));
  return 1;
}

// Resume the task after reconfiguration
uint32_t WcResume(void) {
  if (Wc.state != CAM_PAUSED) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Cannot resume from state %d"), Wc.state);
    return 0;
  }
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Resuming task"));
  
  // Set state back to streaming
  Wc.state = CAM_STREAMING;
  
  // Signal task to resume
  xSemaphoreGive(Wc.resume_sem);
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Task resumed"));
  return 1;
}

uint32_t WcStop(void) {
  if (Wc.state == CAM_IDLE || Wc.state == CAM_STOPPING) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Nothing to stop (state=%d)"), Wc.state);
    return 0;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopping (state=%d)"), Wc.state);
  
  // 1. Set state to STOPPING - task will exit on next iteration
  Wc.state = CAM_STOPPING;
  
  // 2. If task was paused, release it so it can see STOPPING and exit
  xSemaphoreGive(Wc.resume_sem);
  
  // 3. Wake task and wait for it to exit
  if (Wc.cam_task_handle) {
    xTaskNotifyGive(Wc.cam_task_handle);
    
    // Wait for task to exit (task sets handle to NULL before deleting itself)
    for (int i = 0; i < 50 && Wc.cam_task_handle != NULL; i++) {
      delay(10);
    }
    
    // Force delete if task didn't exit
    if (Wc.cam_task_handle != NULL) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Task didn't exit, force deleting"));
      vTaskDelete(Wc.cam_task_handle);
      Wc.cam_task_handle = NULL;
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Task stopped"));
  }
  
  // 4. Stop CSI controller - no more ISR callbacks
  esp_err_t ret = esp_cam_ctlr_stop(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to stop CSI (0x%x)"), ret);
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: CSI stopped"));
  
  // 4. Stop sensor streaming via Berry
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Calling Berry stream stop"));
  callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 0, nullptr, 0);
  
  // 5. Unregister callbacks
  esp_cam_ctlr_evt_cbs_t null_cbs = {0};
  esp_cam_ctlr_register_event_callbacks(Wc.cam_handle, &null_cbs, NULL);
  
  // 6. Disable CSI controller
  ret = esp_cam_ctlr_disable(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to disable CSI (0x%x)"), ret);
  }
  
  // 7. Delete CSI controller
  ret = esp_cam_ctlr_del(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to delete CSI (0x%x)"), ret);
  }
  Wc.cam_handle = NULL;
  
  // 8. Disable and delete ISP
  if (Wc.isp_handle) {
    esp_isp_disable(Wc.isp_handle);
    esp_isp_del_processor(Wc.isp_handle);
    Wc.isp_handle = NULL;
  }
  
  // 9. Delete JPEG encoder
  if (Wc.jpeg_handle) {
    jpeg_del_encoder_engine(Wc.jpeg_handle);
    Wc.jpeg_handle = NULL;
  }
  
  // 10. Delete H.264 encoder
  if (Wc.h264_handle) {
    esp_h264_enc_close(Wc.h264_handle);
    esp_h264_enc_del(Wc.h264_handle);
    Wc.h264_handle = NULL;
  }
  
  // 11. Free buffers
  if (Wc.jpeg_buffer) {
    free(Wc.jpeg_buffer);
    Wc.jpeg_buffer = NULL;
    Wc.jpeg_buffer_size = 0;
  }
  
  if (Wc.h264_buffer) {
    esp_h264_free(Wc.h264_buffer);
    Wc.h264_buffer = NULL;
    Wc.h264_buffer_size = 0;
  }
  
  if (Wc.h264_out_buffer) {
    esp_h264_free(Wc.h264_out_buffer);
    Wc.h264_out_buffer = NULL;
    Wc.h264_out_buffer_size = 0;
  }
  
  for (int i = 0; i < 2; i++) {
    if (Wc.frame_buffer[i]) {
      free(Wc.frame_buffer[i]);
      Wc.frame_buffer[i] = NULL;
    }
  }
  Wc.frame_buffer_size = 0;
  
  // 12. Clean up client pointer before deleting mutex
  if (Wc.client_ptr) {
    delete Wc.client_ptr;
    Wc.client_ptr = nullptr;
  }
  
  // 13. Delete mutexes
  if (Wc.frame_mutex) {
    vSemaphoreDelete(Wc.frame_mutex);
    Wc.frame_mutex = NULL;
  }
  if (Wc.jpeg_mutex) {
    vSemaphoreDelete(Wc.jpeg_mutex);
    Wc.jpeg_mutex = NULL;
  }
  if (Wc.resume_sem) {
    vSemaphoreDelete(Wc.resume_sem);
    Wc.resume_sem = NULL;
  }

  // 14. Final state
  Wc.state = CAM_IDLE;
  Wc.stream_active = 0;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopped"));
  return 1;
}

// Get Frame logic is now internal to HandleWebcamMjpegTask (Zero Copy)
uint8_t* WcGetFrameCSI(uint32_t timeout_ms) {
    // Legacy function support - just returns current read buffer
    if (Wc.state != CAM_STREAMING) return NULL;
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
    ResponseCmndNumber(Wc.config.res_index);
    return;
  }
  
  // Reject if busy
  if (Wc.state == CAM_STOPPING || Wc.state == CAM_PAUSING || Wc.state == CAM_PAUSED) {
    ResponseCmndChar_P(PSTR("Busy"));
    return;
  }
  
  // Pause task first if streaming (ensures clean state)
  bool was_streaming = (Wc.state == CAM_STREAMING);
  if (was_streaming) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Pausing for resolution change"));
    if (!WcPause()) {
      ResponseCmndChar_P(PSTR("Pause failed"));
      return;
    }
  }
  
  // Now safe to stop - task is paused
  if (Wc.state == CAM_PAUSED || Wc.state == CAM_INIT) {
    WcStop();
  }
  
  // Store resolution index
  Wc.config.res_index = (uint8_t)XdrvMailbox.payload;
  
  // Reinitialize with new resolution and start streaming
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Reinitializing with resolution mode %d"), XdrvMailbox.payload);
  uint32_t result = WcSetup(false);  // Don't reset config - keep resolution index
  
  if (result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Setup failed for resolution %d"), XdrvMailbox.payload);
    ResponseCmndFailed();
  } else {
    WcStart();
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Resolution changed to mode %d (%dx%d)"), 
      XdrvMailbox.payload, Wc.config.width, Wc.config.height);
    ResponseCmndNumber(XdrvMailbox.payload);
  }
}

void CmndWcStream(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcStream called, payload=%d"), XdrvMailbox.payload);
  ResponseCmndStateText(Wc.state == CAM_STREAMING);
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
  const char* state_name = (Wc.state < 6) ? state_names[Wc.state] : "UNKNOWN";
  
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
    Wc.config.width, Wc.config.height,
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
  
  int x = 0, y = 0, w = 0, h = 0, bin = 0, fps = 0, format = 0;
  int parsed = sscanf(XdrvMailbox.data, "%d,%d,%d,%d,%d,%d,%d", &x, &y, &w, &h, &bin, &fps, &format);
  
  if (parsed != 7) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to parse (got %d, expected 7)"), parsed);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid. Use: x,y,w,h,bin,fps,format\"}}"));
    return;
  }

  // Validate geometry
  if (w < 16 || h < 16 || w > Wc.config.max_width || h > Wc.config.max_height) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid geometry %dx%d"), w, h);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid geometry. W/H must be 16-2592/1944\"}}"));
    return;
  }
  
  // Validate binning (1=1x1, 2=2x2)
  if (bin < 1 || bin > 2 || (bin != 1 && bin != 2)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid binning %d"), bin);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid binning. Use 1 or 2\"}}"));
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
  
  // Validate format (0=RAW8, 1=RAW10, 2=RAW12)
  if (format < 0 || format > 2) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid format %d"), format);
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Invalid format. Use 0=RAW8, 1=RAW10, 2=RAW12\"}}"));
    return;
  }

  // Reject if busy
  if (Wc.state == CAM_STOPPING || Wc.state == CAM_PAUSING || Wc.state == CAM_PAUSED) {
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Busy\"}}"));
    return;
  }

  // Pause task first if streaming (ensures clean state)
  bool was_streaming = (Wc.state == CAM_STREAMING);
  if (was_streaming) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Pausing for window change"));
    if (!WcPause()) {
      Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Pause failed\"}}"));
      return;
    }
  }

  // Now safe to stop - task is paused
  if (Wc.state == CAM_PAUSED || Wc.state == CAM_INIT) {
    WcStop();
  }

  // Pre-fill Config for Berry (safe assignment with validation)
  Wc.config.offset_x = (uint16_t)x;
  Wc.config.offset_y = (uint16_t)y;
  Wc.config.width = (uint16_t)w;
  Wc.config.height = (uint16_t)h;
  Wc.config.binning = (uint8_t)bin;
  Wc.config.fps = (uint8_t)fps;
  Wc.config.format = (uint8_t)format;
  Wc.config.res_index = 255; // Signal "Custom Mode"

  // Re-Init (Calls Berry 'init') and start streaming
  if (WcSetup(false)) {
    WcStart();
    Response_P(PSTR("{\"WcWindow\":{\"Status\":\"Applied\",\"Width\":%d,\"Height\":%d,\"Binning\":%d,\"FPS\":%d,\"Format\":%d}}"), 
      Wc.config.width, Wc.config.height, Wc.config.binning, Wc.config.fps, Wc.config.format);
  } else {
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Setup Failed\"}}"));
  }
}


/*********************************************************************************************/

void CmndWcQuality(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcQuality called, payload=%d"), XdrvMailbox.payload);
  
  // Query current quality
  if (XdrvMailbox.payload < 0) {
    ResponseCmndNumber(Wc.jpeg_quality);
    return;
  }
  
  // Validate quality (1-100)
  if (XdrvMailbox.payload < 1 || XdrvMailbox.payload > 100) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid quality %d (must be 1-100)"), XdrvMailbox.payload);
    ResponseCmndFailed();
    return;
  }
  
  // Set new quality (will take effect on next stream start)
  Wc.jpeg_quality = (uint8_t)XdrvMailbox.payload;
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: JPEG quality set to %d"), Wc.jpeg_quality);
  ResponseCmndNumber(Wc.jpeg_quality);
}

void CmndWcSession(void) {
  // Session types: 0=None, 1=MJPEG, 2=RTP, 3=WebRTC, 4=DSI
  const char* session_names[] = {"None", "MJPEG", "RTP", "WebRTC", "DSI"};
  
  // Query current session type
  if (XdrvMailbox.payload < 0) {
    Response_P(PSTR("{\"WcSession\":{\"Type\":%d,\"Name\":\"%s\",\"State\":%d}}"), 
      Wc.session_type, 
      (Wc.session_type <= 4) ? session_names[Wc.session_type] : "Unknown",
      Wc.state);
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
  if (new_type == Wc.session_type) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Session type unchanged"));
    Response_P(PSTR("{\"WcSession\":{\"Type\":%d,\"Name\":\"%s\"}}"), 
      Wc.session_type, session_names[Wc.session_type]);
    return;
  }
  
  // Check if implemented
  if (new_type == SESSION_WEBRTC || new_type == SESSION_DSI_DISPLAY) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Session type %d not yet implemented"), new_type);
    Response_P(PSTR("{\"WcSession\":{\"Error\":\"Not implemented\",\"Requested\":%d}}"), new_type);
    return;
  }
  
  // Stop current session if running
  if (Wc.state != CAM_IDLE) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Stopping current session for switch"));
    WcStop();
  }
  
  // Set new session type
  Wc.session_type = new_type;
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Session type changed to %d (%s)"), 
    Wc.session_type, session_names[Wc.session_type]);
  
  // Initialize RTP session
  if (new_type == SESSION_RTP) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP init start"));
    
    Wc.rtp_sequence = random(0, 65535);
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP sequence=%u"), Wc.rtp_sequence);
    
    Wc.rtp_timestamp = random(0, UINT32_MAX);
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP timestamp=%u"), Wc.rtp_timestamp);
    
    Wc.rtp_ssrc = random(0, UINT32_MAX);
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP ssrc=%u"), Wc.rtp_ssrc);
    
    // Set default destination only if not configured yet
    if (Wc.rtp_dest_ip == IPAddress(0, 0, 0, 0)) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP setting default dest IP"));
      Wc.rtp_dest_ip.fromString("192.168.1.100");
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP default dest IP set"));
    }
    
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP dest port=%u"), Wc.rtp_dest_port);
    
    // Start UDP socket
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP starting UDP socket"));
    Wc.rtp_udp.begin(5004);
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP UDP socket started"));
    
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP initialized, dest %s:%d"), 
      Wc.rtp_dest_ip.toString().c_str(), Wc.rtp_dest_port);
  }
  
  // Force H.264-compatible resolution for RTP session
  if (new_type == SESSION_RTP) {
    Wc.config.width = 1280;
    Wc.config.height = 720;
    Wc.config.binning = 2;
    Wc.config.fps = 30;
    Wc.config.format = 0;  // RAW8
    Wc.config.res_index = 255;  // Custom mode
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP session forcing 1280x720@30fps"));
  }
  
  // Setup and start new session (unless SESSION_NONE)
  if (new_type != SESSION_NONE) {
    if (WcSetup(false)) {
      WcStart();
    }
  }
  
  Response_P(PSTR("{\"WcSession\":{\"Type\":%d,\"Name\":\"%s\",\"State\":%d}}"), 
    Wc.session_type, session_names[Wc.session_type], Wc.state);
}

void CmndWcRtpDest(void) {
  // Query current RTP destination
  if (XdrvMailbox.data_len == 0) {
    Response_P(PSTR("{\"WcRtpDest\":{\"IP\":\"%s\",\"Port\":%d}}"), 
      Wc.rtp_dest_ip.toString().c_str(), Wc.rtp_dest_port);
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
  Wc.rtp_dest_port = port;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTP destination set to %s:%d"), 
    Wc.rtp_dest_ip.toString().c_str(), Wc.rtp_dest_port);
  
  Response_P(PSTR("{\"WcRtpDest\":{\"IP\":\"%s\",\"Port\":%d}}"), 
    Wc.rtp_dest_ip.toString().c_str(), Wc.rtp_dest_port);
}


/*********************************************************************************************/

void WcInit(void) {
  // Cannot use memset on struct with C++ objects (WiFiUDP has vtable)
  // Manually initialize critical members
  Wc.cam_handle = NULL;
  Wc.isp_handle = NULL;
  Wc.ldo_mipi_phy = NULL;
  Wc.frame_buffer[0] = NULL;
  Wc.frame_buffer[1] = NULL;
  Wc.frame_buffer_size = 0;
  Wc.write_idx = 0;
  Wc.read_idx = 0;
  Wc.state = CAM_IDLE;
  Wc.session_type = SESSION_MJPEG_HTTP;
  Wc.cam_task_handle = NULL;
  Wc.frame_mutex = NULL;
  Wc.resume_sem = NULL;
  Wc.jpeg_handle = NULL;
  Wc.jpeg_buffer = NULL;
  Wc.jpeg_buffer_size = 0;
  Wc.jpeg_quality = 50;
  Wc.jpeg_mutex = NULL;
  Wc.CamServer = NULL;
  Wc.client_ptr = NULL;
  Wc.stream_active = 0;
  Wc.h264_handle = NULL;
  Wc.h264_buffer = NULL;
  Wc.h264_buffer_size = 0;
  Wc.h264_out_buffer = NULL;
  Wc.h264_out_buffer_size = 0;
  Wc.rtp_dest_port = 5004;
  Wc.rtp_sequence = 0;
  Wc.rtp_timestamp = 0;
  Wc.rtp_ssrc = 0;
  
  // WiFiUDP and IPAddress have constructors, don't touch them
  
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
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Handle camserver - state=%d"), Wc.state);
  
  if (!Wc.CamServer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CamServer is NULL!"));
    return;
  }
  
  if (Wc.state != CAM_STREAMING) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Not streaming - rejecting request"));
    Wc.CamServer->send(503, "text/plain", "Camera not ready");
    return;
  }
  
  // Lock frame access for client setup
  if (xSemaphoreTake(Wc.frame_mutex, pdMS_TO_TICKS(200)) == pdTRUE) {
    // Allocate client on heap to avoid stack issues
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Allocating client..."));
    if (Wc.client_ptr) {
      delete Wc.client_ptr;
      Wc.client_ptr = nullptr;
    }
    
    Wc.client_ptr = new WiFiClient();
    if (!Wc.client_ptr) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate client!"));
      xSemaphoreGive(Wc.frame_mutex);
      return;
    }
    
    *Wc.client_ptr = Wc.CamServer->client();
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client allocated, connected=%d"), Wc.client_ptr->connected());
    
    // Send HTTP header
    Wc.client_ptr->print("HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n\r\n");
    Wc.stream_active = 2;
    
    xSemaphoreGive(Wc.frame_mutex);
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

/*********************************************************************************************/
// RTP Session Functions (H.264 over UDP)
//
// DEPRECATED: These stub functions are probably subject to be removed.
// H.264 encoder setup is now handled by WcSetupH264Encoder() called from WcSetup().
// RTP packet sending will be integrated into CamProcessingTask.

uint32_t WcRtpSetup(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("RTP: Setup - NOT IMPLEMENTED"));
  
  // TODO: Initialize H.264 encoder (from Espressif example)
  // Width/height must be 16-byte aligned (macroblock size)
  // uint16_t width = ((Wc.config.width + 15) >> 4 << 4);
  // uint16_t height = ((Wc.config.height + 15) >> 4 << 4);
  //
  // esp_h264_enc_cfg_hw_t cfg = {
  //   .gop = 30,
  //   .fps = 30,
  //   .res = {.width = width, .height = height},
  //   .rc = {.bitrate = width * height / 10, .qp_min = 26, .qp_max = 30},
  //   .pic_type = ESP_H264_RAW_FMT_UYVY  // ISP outputs YUV422
  // };
  //
  // Allocate input buffer (from camera)
  // in_frame.raw_data.len = width * height * ESP_H264_GET_BPP_BY_PIC_TYPE(cfg.pic_type);
  // in_frame.raw_data.buffer = esp_h264_aligned_calloc(16, 1, in_frame.raw_data.len, ...);
  //
  // Allocate output buffer (H.264 NAL units)
  // out_frame.raw_data.len = in_frame.raw_data.len / 10;  // Compressed ~10x
  // out_frame.raw_data.buffer = esp_h264_aligned_calloc(16, 1, out_frame.raw_data.len, ...);
  //
  // esp_h264_enc_hw_new(&cfg, &Wc.h264_handle);
  // esp_h264_enc_open(Wc.h264_handle);
  
  // TODO: Initialize RTP state
  // Wc.rtp_sequence = 0;
  // Wc.rtp_timestamp = 0;
  // Wc.rtp_ssrc = esp_random();  // Random SSRC
  // Wc.rtp_dest_port = 5004;     // Default RTP port
  
  return 0;  // Not implemented
}

void WcRtpStop(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("RTP: Stop - NOT IMPLEMENTED"));
  
  // TODO: Close H.264 encoder
  // if (Wc.h264_handle) {
  //   esp_h264_enc_close(Wc.h264_handle);
  //   esp_h264_enc_del(Wc.h264_handle);
  //   Wc.h264_handle = NULL;
  // }
  
  // TODO: Free H.264 buffer
  // TODO: Close UDP socket
}

// Send one H.264 frame as RTP packets
void WcRtpSendFrame(uint8_t *nal_data, size_t nal_size) {
  // TODO: Implement RTP packetization (RFC 6184)
  // - Single NAL unit mode for small NALs (< MTU)
  // - FU-A fragmentation for large NALs (> MTU)
  
  // RTP header (12 bytes):
  // Byte 0: V=2, P=0, X=0, CC=0 → 0x80
  // Byte 1: M=marker, PT=96 (dynamic)
  // Bytes 2-3: Sequence number
  // Bytes 4-7: Timestamp (90kHz)
  // Bytes 8-11: SSRC
}

/*********************************************************************************************/

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
  if (Wc.CamServer && Wc.state == CAM_STREAMING) {
    WSContentSend_P(PSTR("<p></p><center><img onerror='setTimeout(()=>{this.src=this.src;},1000)' src='http://%_I:81/stream' alt='Webcam stream''></center><p></p>"),(uint32_t)WiFi.localIP());
  }
}


void WcLoop(void) {
  // Skip during state transitions
  if (Wc.state == CAM_STOPPING || Wc.state == CAM_IDLE) {
    return;
  }
  
  if (Wc.state == CAM_STREAMING && !Wc.CamServer && !TasmotaGlobal.global_state.network_down) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Starting stream server..."));
    WcSetStreamserver(1);
  }
  
  if (Wc.CamServer) {
    Wc.CamServer->handleClient();
    
    // Monitor client connection - cleanup if disconnected (with mutex protection)
    if (Wc.stream_active && xSemaphoreTake(Wc.frame_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      if (Wc.client_ptr && !Wc.client_ptr->connected()) {
        delete Wc.client_ptr;
        Wc.client_ptr = nullptr;
        Wc.stream_active = 0;
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client disconnected"));
      }
      xSemaphoreGive(Wc.frame_mutex);
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
      if (Wc.state == CAM_IDLE) {
        WcSetup(true);  // First boot - reset config to defaults
      }
      break;
    case FUNC_EVERY_SECOND:
      // Auto-start streaming once WiFi is available
      if (Wc.state == CAM_INIT && !TasmotaGlobal.global_state.network_down) {
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
