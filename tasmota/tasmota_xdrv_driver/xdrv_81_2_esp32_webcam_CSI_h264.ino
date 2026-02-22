/*
  xdrv_81_2_esp32_webcam_CSI_h264.ino - H.264/RTP/RTSP/WebSocket Streaming for Tasmota ESP32-P4

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

#include <t_bearssl.h> // Required for WebSocket Handshake (SHA1)

bool WsPerformWsHandshake(WiFiClient* client) {
  if (!client || !client->connected()) return false;

  char buf[512]; // Stack buffer for reading headers (Internal RAM, but temporary)
  char key[64] = {0}; // To store Sec-WebSocket-Key
  bool upgrade_found = false;
  unsigned long start = millis();

  // 1. Read Headers (Line by Line using raw buffer)
  while (client->connected() && millis() - start < 1000) {
    if (client->available()) {
      // Read until newline or buffer full
      size_t len = client->readBytesUntil('\n', buf, sizeof(buf) - 1);
      buf[len] = 0; // Null terminate
      
      // Trim CR if present
      if (len > 0 && buf[len - 1] == '\r') buf[len - 1] = 0;
      
      // Empty line = End of Headers
      if (strlen(buf) == 0) break;

      // Check for Upgrade: websocket
      if (strcasestr(buf, "Upgrade: websocket")) {
        upgrade_found = true;
      }

      // Extract Key
      // Header: "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ=="
      char* key_ptr = strcasestr(buf, "Sec-WebSocket-Key:");
      if (key_ptr) {
        key_ptr += 18; // Skip "Sec-WebSocket-Key:"
        while (*key_ptr == ' ') key_ptr++; // Skip spaces
        strncpy(key, key_ptr, sizeof(key) - 1);
      }
    }
  }

  if (!upgrade_found || strlen(key) == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("WS: Handshake failed (No Upgrade or Key)"));
    return false;
  }

  // 2. Concatenate & Hash (No String Objects)
  // GUID: 258EAFA5-E914-47DA-95CA-C5AB0DC85B11
  // Max Key (24) + GUID (36) + Null = 61 chars. 128 is plenty.
  char concat[128];
  snprintf(concat, sizeof(concat), "%s258EAFA5-E914-47DA-95CA-C5AB0DC85B11", key);

  uint8_t hash[20];
  br_sha1_context ctx;
  br_sha1_init(&ctx);
  br_sha1_update(&ctx, (const void*)concat, strlen(concat));
  br_sha1_out(&ctx, hash);

  // 3. Base64 Encode (Fixed Buffer)
  // 20 bytes -> 28 Base64 chars
  char acceptKey[32];
  const char* b64 PROGMEM = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  
  for (int i = 0, j = 0; i < 18; i += 3, j += 4) {
    uint32_t n = (hash[i] << 16) | (hash[i+1] << 8) | hash[i+2];
    acceptKey[j]   = b64[(n >> 18) & 63];
    acceptKey[j+1] = b64[(n >> 12) & 63];
    acceptKey[j+2] = b64[(n >> 6) & 63];
    acceptKey[j+3] = b64[n & 63];
  }
  // Final block (padding)
  uint32_t n = (hash[18] << 16) | (hash[19] << 8);
  acceptKey[24] = b64[(n >> 18) & 63];
  acceptKey[25] = b64[(n >> 12) & 63];
  acceptKey[26] = b64[(n >> 6) & 63];
  acceptKey[27] = '=';
  acceptKey[28] = 0;

  // 4. Send Response (Zero String Allocation)
  // Use client->printf or multiple prints to avoid building one giant string
  client->print(PSTR("HTTP/1.1 101 Switching Protocols\r\n"
                "Upgrade: websocket\r\n"
                "Connection: Upgrade\r\n"
                "Sec-WebSocket-Accept: "));
  client->print(acceptKey);
  client->print("\r\n\r\n");

  return true;
}

// --- H.264 / RTSP Streaming Module ---

void H264ProcessingTask(void *pvParameters) {
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); // 100ms timeout
  uint32_t last_fps_calc = millis();
  uint32_t frames_in_second = 0;
  static uint32_t last_profile_log = 0;
  
  // H.264 encoder frame structures
  esp_h264_enc_in_frame_t in_frame = {};
  esp_h264_enc_out_frame_t out_frame = {};

  size_t h264_expected_size;
  
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

    // 1. Lock frame access
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

    // 2. Cache Sync is still required (Hardware Memory-to-CPU coherency)
    // Even for HW-to-HW, we ensure the previous write is visible
    esp_cache_msync(source_buf, Wc.core.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);

    // --- ZERO-COPY OPTIMIZATION START ---
    // We do NOT release the mutex yet. We hold it until encoding is done.
    in_frame.raw_data.buffer = source_buf;
    h264_expected_size = Wc.core.config.width * Wc.core.config.height * 3 / 2;
    in_frame.raw_data.len = h264_expected_size; 
    
    // Note: We use h264_expected_size to ensure we don't feed the encoder garbage 
    // if the buffer is larger than the YUV420 footprint.

    // Setup output frame buffer (unchanged)
    out_frame.raw_data.buffer = Wc.h264.out_buffer;
    out_frame.raw_data.len = Wc.h264.out_buffer_size;

    // 3. Encode H.264 (Hardware)
    uint32_t encode_start = micros();
    esp_h264_err_t ret = esp_h264_enc_process(Wc.h264.handle, &in_frame, &out_frame);
    uint32_t encode_time = micros() - encode_start;

    // 4. NOW we release the mutex
    // The encoder is done reading source_buf, so the ISP is free to overwrite it.
    xSemaphoreGive(Wc.core.frame_mutex);
    
    if (ret == ESP_H264_ERR_OK && out_frame.length > 0) {
      // Motion Detection Calculation
      bool is_keyframe = (out_frame.frame_type == ESP_H264_FRAME_TYPE_IDR || 
                          out_frame.frame_type == ESP_H264_FRAME_TYPE_I);
      
      if (!is_keyframe) {
          Wc.h264.motion_raw = out_frame.length;
          // Calculate score (approx 1 unit per 100 bytes of change)
          uint32_t raw_score = out_frame.length / 128; 
          
          // Exponential Moving Average (EMA)
          // Value = (Old * 4 + New) / 5
          Wc.h264.motion_val = (Wc.h264.motion_val * 4 + raw_score) / 5;
          
          // Optional: Trigger event if high
          if (Wc.h264.motion_val > 50) { // Threshold 50 (~5KB change)
             // Debounce logic here if needed
          }
      }


      // --- Path A: WebSocket Multicast (Binary) ---
      if (Wc.ws.active) {
        if (Wc.ws.client_ptr && Wc.ws.client_ptr->connected()) {

          // Payload = [1 byte flags][H264 bytes...]
          uint8_t flag = is_keyframe ? 1 : 0;
          size_t payload_len = out_frame.length + 1;

          // Manual WebSocket Frame Construction (FIN, Binary)
          uint8_t hdr[10];
          hdr[0] = 0x82; // FIN + Binary
          size_t hlen = 2;

          if (payload_len < 126) {
            hdr[1] = payload_len;
          } else if (payload_len < 65536) {
            hdr[1] = 126;
            hdr[2] = (payload_len >> 8) & 0xFF;
            hdr[3] = payload_len & 0xFF;
            hlen = 4;
          } else {
            hdr[1] = 127;
            hdr[2]=0; hdr[3]=0; hdr[4]=0; hdr[5]=0;
            hdr[6]=(payload_len>>24)&0xFF;
            hdr[7]=(payload_len>>16)&0xFF;
            hdr[8]=(payload_len>>8)&0xFF;
            hdr[9]=(payload_len)&0xFF;
            hlen = 10;
          }

          Wc.ws.client_ptr->write(hdr, hlen);
          Wc.ws.client_ptr->write(&flag, 1);
          Wc.ws.client_ptr->write(out_frame.raw_data.buffer, out_frame.length);

        } else {
          Wc.ws.active = false;
        }
      }


      // --- Path B: Parse and send NAL units via RTP (only if RTSP session is in PLAY state) ---
      if (Wc.rtsp.streaming) {
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
  uint16_t width = ((Wc.core.config.width) >> 4) << 4; // may cut off up to 15 pixels
  uint16_t height = ((Wc.core.config.height) >> 4) << 4;

  // --- DYNAMIC QUALITY MAPPING --- subject to change based on testing/feedback ---
  uint8_t q_user = Wc.jpeg.quality;
  if (q_user < 5) q_user = 5;
  if (q_user > 100) q_user = 100;

  // 1. Bitrate: 100 = 6Mbps, 50 = 3Mbps, 10 = 600kbps
  uint32_t target_bitrate = q_user * 60000; 

  // 2. QP_Min: 100 -> QP15, 50 -> QP37, 10 -> QP47
  // We aggressively raise QP (lower quality) as the user slider goes down.
  uint8_t target_qp_min = 50 - (q_user / 4); 
  if (target_qp_min < 15) target_qp_min = 15; 
  if (target_qp_min > 25) target_qp_min = 25;

  AddLog(LOG_LEVEL_INFO, PSTR("H264: Quality %d -> Bitrate %d bps, QP_Min %d"), q_user, target_bitrate, target_qp_min);
  
  esp_h264_enc_cfg_hw_t cfg = {
    .pic_type = ESP_H264_RAW_FMT_O_UYY_E_VYY,  // HW encoder requires this YUV420 format
    .gop = 30,
    .fps = (uint8_t)(Wc.core.config.fps ? Wc.core.config.fps : 30),
    .res = {.width = width, .height = height},
    .rc = {
      .bitrate = target_bitrate,
      .qp_min = target_qp_min,
      .qp_max = 35
    }
  };
  
  // Allocate output frame buffer (encoded NAL units) - same size as input
  size_t out_size = (size_t)((float)width * height * ESP_H264_GET_BPP_BY_PIC_TYPE(cfg.pic_type));
  Wc.h264.out_buffer = (uint8_t*)esp_h264_aligned_calloc(16, 1, out_size, &out_size, ESP_H264_MEM_SPIRAM);
  if (!Wc.h264.out_buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 output buffer alloc failed"));
    return 0;
  }
  Wc.h264.out_buffer_size = out_size;
  
  // Create encoder
  esp_h264_err_t ret = esp_h264_enc_hw_new(&cfg, &Wc.h264.handle);
  if (ret != ESP_H264_ERR_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 encoder create failed (0x%x)"), ret);
    esp_h264_free(Wc.h264.out_buffer);
    Wc.h264.out_buffer = NULL;
    return 0;
  }
  
  // Open encoder
  ret = esp_h264_enc_open(Wc.h264.handle);
  if (ret != ESP_H264_ERR_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: H.264 encoder open failed (0x%x)"), ret);
    esp_h264_enc_del(Wc.h264.handle);
    esp_h264_free(Wc.h264.out_buffer);
    Wc.h264.handle = NULL;
    Wc.h264.out_buffer = NULL;
    return 0;
  }
  
  // --- SPS/PPS Probe: Force-encode one black frame to extract headers ---
  // ESP32-P4 hardware encoder may not generate SPS/PPS in every frame
  // We need to capture them once for WebRTC SDP negotiation
  AddLog(LOG_LEVEL_INFO, PSTR("H264: Probing for SPS/PPS headers"));
  
  // Initialize SPS/PPS storage
  Wc.h264.sps_len = 0;
  Wc.h264.pps_len = 0;
  Wc.h264.sps_pps_captured = false;
  
  // 1. Allocate a temporary black frame (O_UYY_E_VYY format size = 1.5 * w * h)
  size_t frame_size = width * height * 3 / 2;
  uint8_t *dummy_buf = (uint8_t*)heap_caps_malloc(frame_size, MALLOC_CAP_SPIRAM);
  if (dummy_buf) {
    memset(dummy_buf, 128, frame_size); // Grayish/Black (Y=128, U=128, V=128)
    
    esp_h264_enc_in_frame_t in_f = { .raw_data = { .buffer = dummy_buf, .len = frame_size } };
    esp_h264_enc_out_frame_t out_f = { .raw_data = { .buffer = Wc.h264.out_buffer, .len = Wc.h264.out_buffer_size } };
    
    // 2. Encode one frame to extract headers
    esp_h264_err_t ret = esp_h264_enc_process(Wc.h264.handle, &in_f, &out_f);
    
    if (ret == ESP_H264_ERR_OK && out_f.length > 0) {
      // 3. Scan for SPS (type 7) and PPS (type 8)
      uint8_t *p = out_f.raw_data.buffer;
      uint32_t len = out_f.length;
      
      // Simple NAL scanner
      for (uint32_t i = 0; i < len - 4; i++) {
        if (p[i] == 0 && p[i+1] == 0 && p[i+2] == 0 && p[i+3] == 1) {
          uint8_t type = p[i+4] & 0x1F;
          if (type == 7) { // SPS
            // Find next start code to determine SPS length
            uint32_t sps_start = i + 4;
            uint32_t sps_end = len;
            for (uint32_t j = i + 4; j < len - 3; j++) {
              if (p[j] == 0 && p[j+1] == 0 && p[j+2] == 0 && p[j+3] == 1) {
                sps_end = j;
                break;
              }
            }
            uint32_t sps_length = sps_end - sps_start;
            if (sps_length > 0 && sps_length <= sizeof(Wc.h264.sps_buffer)) {
              memcpy(Wc.h264.sps_buffer, p + sps_start, sps_length);
              Wc.h264.sps_len = sps_length;
              AddLog(LOG_LEVEL_INFO, PSTR("🎯 H264: Captured SPS (%d bytes) from HW Encoder!"), sps_length);
            }
          } else if (type == 8) { // PPS
            // Find next start code to determine PPS length
            uint32_t pps_start = i + 4;
            uint32_t pps_end = len;
            for (uint32_t j = i + 4; j < len - 3; j++) {
              if (p[j] == 0 && p[j+1] == 0 && p[j+2] == 0 && p[j+3] == 1) {
                pps_end = j;
                break;
              }
            }
            uint32_t pps_length = pps_end - pps_start;
            if (pps_length > 0 && pps_length <= sizeof(Wc.h264.pps_buffer)) {
              memcpy(Wc.h264.pps_buffer, p + pps_start, pps_length);
              Wc.h264.pps_len = pps_length;
              AddLog(LOG_LEVEL_INFO, PSTR("🎯 H264: Captured PPS (%d bytes) from HW Encoder!"), pps_length);
            }
          }
        }
      }
      
      // Check if we captured both SPS and PPS
      if (Wc.h264.sps_len > 0 && Wc.h264.pps_len > 0) {
        Wc.h264.sps_pps_captured = true;
        AddLog(LOG_LEVEL_INFO, PSTR("H264: Successfully captured SPS/PPS headers for WebRTC"));
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("H264: Could not capture SPS/PPS headers from probe frame"));
        AddLog(LOG_LEVEL_ERROR, PSTR("H264: WebRTC may fail without SPS/PPS in SDP"));
      }
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("H264: Probe frame encode failed (ret=0x%x, len=%d)"), ret, out_f.length);
    }
    free(dummy_buf);
  } else {
    AddLog(LOG_LEVEL_ERROR, PSTR("H264: Could not allocate probe buffer (%d bytes)"), frame_size);
  }
  // -------------------------------------------------------------------
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: H.264 encoder initialized (%dx%d, buffer=%d bytes)"), width, height, out_size);
  return 1;
}

/*********************************************************************************************/

void WcRtspStop(void) {
  // Stop RTSP
  if (Wc.rtsp.server) {
    Wc.rtsp.server->stop();
    delete Wc.rtsp.server;
    Wc.rtsp.server = NULL;
  }
  if (Wc.rtsp_client) {
    Wc.rtsp_client.stop();
  }
  Wc.rtsp.streaming = false;

  // Stop WebSocket
  Wc.ws.active = false;
  if (Wc.ws.server) { 
    Wc.ws.server->stop(); 
    delete Wc.ws.server; 
    Wc.ws.server = NULL; 
  }
  if (Wc.ws.client_ptr) { 
    delete Wc.ws.client_ptr; 
    Wc.ws.client_ptr = NULL; 
  }
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
  
  // --- 1. Port 554 (RTSP) Handling ---
  if (Wc.rtsp.server) {
    if (Wc.rtsp.server->hasClient()) {
        if (Wc.rtsp_client && Wc.rtsp_client.connected()) {
        Wc.rtsp.server->available().stop(); // Reject busy
        } else {
        Wc.rtsp_client = Wc.rtsp.server->available();
        AddLog(LOG_LEVEL_INFO, PSTR("RTSP: Client connected"));
        }
    }


    if (Wc.rtsp_client && Wc.rtsp_client.connected() && Wc.rtsp_client.available()) {
        String req = Wc.rtsp_client.readStringUntil('\n');
        req.trim();
        if (req.length() > 0) {
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


            // Command Dispatch
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
    } else {
         if (Wc.rtsp.streaming && !Wc.rtsp_client.connected()) Wc.rtsp.streaming = false;
    }
  }

  // --- 2. Port 82 (WebSocket) Handling ---
  if (Wc.ws.server && Wc.ws.server->hasClient()) {
    WiFiClient* new_client = new WiFiClient(Wc.ws.server->available());
    if (new_client && new_client->connected() && WsPerformWsHandshake(new_client)) {
        if (Wc.ws.client_ptr) delete Wc.ws.client_ptr;
        Wc.ws.client_ptr = new_client;
        Wc.ws.active = true;
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: WS Client Connected"));
    } else {
        if (new_client) {
          new_client->stop();
          delete new_client;
        }
    }
  }
}


/*********************************************************************************************/
// RTP Helper Function

#define WC_RTP_MAX_PAYLOAD 1400  // MTU safety margin
#define WC_RTP_HEADER_SIZE 12

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
  if (nallen <= WC_RTP_MAX_PAYLOAD) {
    bool marker = (naltype == 1 || naltype == 5);
    WcRtpSend(marker, naldata, nallen);
  } 
  // Case B: Large NAL - Fragment (FU-A)
  else {
    size_t offset = 1; // Skip NAL header
    size_t payload_cap = WC_RTP_MAX_PAYLOAD - 2; 
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

// NOTE (Safari/WebCodecs + WS H.264):
// Safari may not reliably start the ws:// H.264 pipeline on plain HTTP until triggered by a user gesture,
// so the WebCodecs demo page uses a Start button (and a small WS “kick” + reconnect) to force the socket
// into OPEN before decoding; once running, Safari decodes fine with correct key/delta signaling and SPS/PPS+IDR.

void WcH264SendUi() {
  
  // 1. CSS
  char style_buf[512];
  snprintf_P(style_buf, sizeof(style_buf), PSTR(
    "<style>"
    "#wc_c{position:relative;width:100%%;background:#000;display:flex;justify-content:center;align-items:center;aspect-ratio:%d/%d}"
    "#wc_cvs{width:100%%;height:100%%;display:block}"
    "</style>"
  ), Wc.core.config.width, Wc.core.config.height);
  WSContentSend(style_buf, strlen(style_buf)); 

  // 2. HTML
  WSContentSend_P(PSTR(
    "<div id='wc_c'>"
      "<canvas id='wc_cvs'></canvas>"
    "</div>"
  ));

  // 3. JS (Using eb())
  WSContentSend_P(PSTR(
    "<script>"
    "(()=>{"
      "const cvs=eb('wc_cvs');"  // <--- Shorthand used here
      "const ctx=cvs.getContext('2d');"
      "const WS_URL=(location.protocol==='https:'?'wss://':'ws://')+location.hostname+':82/';"
      "let ws=null,dec=null,key=false,run=false;"
      "let fCount=0,bytes=0,lTime=performance.now();"
      "let s_fps=0,s_kbps=0,s_key=0,s_delta=0,vidW=0,vidH=0;"
      
      "function drawHud(){"
         "if(vidW<10)return;"
         "ctx.save();"
         "ctx.font='14px monospace';ctx.textBaseline='top';"
         "const lines=[`FPS: ${s_fps}`,`KB/s: ${s_kbps}`,`Res: ${vidW}x${vidH}`,`Key: ${s_key} Delta: ${s_delta}`,`WS: ${ws?ws.readyState:'-'}`];"
         "ctx.globalAlpha=0.6;ctx.fillStyle='#000';"
         "ctx.fillRect(8,8,160,lines.length*16+8);"
         "ctx.globalAlpha=1;ctx.fillStyle='#0f0';"
         "let y=12;"
         "lines.forEach(l=>{ctx.fillText(l,12,y);y+=16;});"
         "ctx.restore();"
      "}"
      
      "function kickWebSocket(url){try{const k=new WebSocket(url);k.onopen=()=>{try{k.close();}catch(e){}};k.onerror=(e)=>{console.log(e)};}catch(e){}}"
      "function openWebSocket(url){return new Promise((resolve,reject)=>{let w;try{w=new WebSocket(url);}catch(e){reject(e);return;}w.onopen=()=>{resolve(w);};w.onerror=(e)=>{reject(e);};});}"
      
      "function stop(){"
        "run=false;"
        "if(ws){ws.close();ws=null;}"
        "if(dec){dec.close();dec=null;}"
      "}"
      
      "async function start(){"
        "stop();"
        "run=true;"
        "s_key=0;s_delta=0;"
        "if(!('VideoDecoder' in window)){console.error('No WebCodecs');return;}"
        
        "dec=new VideoDecoder({"
          "output:f=>{"
             "vidW=f.displayWidth;vidH=f.displayHeight;"
             "if(cvs.width!==vidW)cvs.width=vidW;"
             "if(cvs.height!==vidH)cvs.height=vidH;"
             "ctx.drawImage(f,0,0);"
             "drawHud();"
             "f.close();"
             "fCount++;"
          "},"
          "error:e=>{console.error(e);stop();}"
        "});"
        
        "try{"
          "dec.configure({codec:'avc1.42001E',optimizeForLatency:true});"
        "}catch(e){console.error(e);return;}"
        
        "kickWebSocket(WS_URL);"
        "await new Promise(r=>setTimeout(r,80));"
        
        "try{"
          "ws=await openWebSocket(WS_URL);"
          "ws.binaryType='arraybuffer';"
          "ws.onclose=()=>{if(run)setTimeout(start,1000);};"
          "ws.onmessage=e=>{"
            "bytes+=e.data.byteLength;"
            "let u8=new Uint8Array(e.data);"
            "if(u8.length<1)return;"
            "let isKey=(u8[0]===1);"
            "if(isKey){key=true;s_key++;}"
            "else{s_delta++;}"
            "if(!key)return;"
            "try{"
              "dec.decode(new EncodedVideoChunk({type:isKey?'key':'delta',timestamp:performance.now()*1000,data:u8.subarray(1)}));"
            "}catch(x){console.error(x);}"
            
            "if(performance.now()-lTime>1000){"
              "s_fps=fCount;"
              "s_kbps=Math.round(bytes/1024);"
              "fCount=0;bytes=0;lTime=performance.now();"
            "}"
          "};"
        "}catch(e){console.error(e);stop();}"
      "}"
      
      "start();"
    "})();"
    "</script>"
  ));
}

#endif  // USE_CSI_WEBCAM
#endif  // ESP32