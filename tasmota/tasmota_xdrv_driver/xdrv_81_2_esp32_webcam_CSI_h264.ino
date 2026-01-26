/*
  xdrv_81_2_esp32_webcam_CSI_h264.ino - ESP32-P4 CSI webcam support for Tasmota

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


// --- H.264 / RTSP Streaming Module ---

void H264ProcessingTask(void *pvParameters) {
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); // 100ms timeout
  uint32_t last_fps_calc = millis();
  uint32_t frames_in_second = 0;
  static uint32_t last_profile_log = 0;
  
  // H.264 encoder frame structures
  esp_h264_enc_in_frame_t in_frame = {};
  esp_h264_enc_out_frame_t out_frame = {};
  memset(Wc.h264.buffer, 0, Wc.h264.buffer_size); 

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
          WcSendNalUnit(nal_data, nal_length, nal_type, nal_nri);
          
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

uint32_t WcSetupH264Encoder(void) {
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

/*********************************************************************************************/

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
// RTP Helper Function

#define RTP_MAX_PAYLOAD 1400  // MTU safety margin
#define RTP_HEADER_SIZE 12

// Helper: Builds RTP header and sends packet
void WcRtpSend(bool marker, const uint8_t* payload, size_t len, const uint8_t* fu_header = nullptr) {
  uint8_t header[12];
  
  // RTP Header: V=2, P=0, X=0, CC=0, PT=96
  header[0] = 0x80;
  header[1] = 96 | (marker ? 0x80 : 0x00);
  
  // Big Endian Headers
  header[2] = (Wc.rtp.sequence >> 8) & 0xFF;
  header[3] = Wc.rtp.sequence & 0xFF;
  header[4] = (Wc.rtp.timestamp >> 24) & 0xFF;
  header[5] = (Wc.rtp.timestamp >> 16) & 0xFF;
  header[6] = (Wc.rtp.timestamp >> 8) & 0xFF;
  header[7] = Wc.rtp.timestamp & 0xFF;
  header[8] = (Wc.rtp.ssrc >> 24) & 0xFF;
  header[9] = (Wc.rtp.ssrc >> 16) & 0xFF;
  header[10] = (Wc.rtp.ssrc >> 8) & 0xFF;
  header[11] = Wc.rtp.ssrc & 0xFF;

  Wc.rtp_udp.beginPacket(Wc.rtp_dest_ip, Wc.rtp.dest_port);
  Wc.rtp_udp.write(header, 12);
  if (fu_header) Wc.rtp_udp.write(fu_header, 2);
  Wc.rtp_udp.write(payload, len);
  Wc.rtp_udp.endPacket();

  Wc.rtp.sequence++;
}

void WcSendNalUnit(uint8_t* naldata, size_t nallen, uint8_t naltype, uint8_t nalnri) {
  // Case A: Small NAL - Send as Single Packet
  if (nallen <= RTP_MAX_PAYLOAD) {
    bool marker = (naltype == 1 || naltype == 5);
    WcRtpSend(marker, naldata, nallen);
  } 
  // Case B: Large NAL - Fragment (FU-A)
  else {
    size_t offset = 1; // Skip NAL header
    size_t payload_cap = RTP_MAX_PAYLOAD - 2; 
    uint8_t fu_indicator = (nalnri << 5) | 28; 

    while (offset < nallen) {
      size_t chunk = (nallen - offset > payload_cap) ? payload_cap : (nallen - offset);
      bool is_last  = (offset + chunk >= nallen);
      bool is_first = (offset == 1);
      
      uint8_t fu_header_byte = naltype;
      if (is_first) fu_header_byte |= 0x80; 
      if (is_last)  fu_header_byte |= 0x40; 

      uint8_t fu_data[2] = { fu_indicator, fu_header_byte };
      bool marker = is_last && (naltype == 1 || naltype == 5);
      
      WcRtpSend(marker, naldata + offset, chunk, fu_data);
      
      offset += chunk;
    }
  }
}

#endif  // USE_CSI_WEBCAM
#endif  // ESP32P4