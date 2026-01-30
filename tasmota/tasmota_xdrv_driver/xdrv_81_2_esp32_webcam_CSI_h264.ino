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

#include <t_bearssl.h> // Required for WebSocket Handshake (SHA1)

bool WsPerformWsHandshake(WiFiClient* client) {
    if (!client || !client->connected()) return false;
    String key = "";
    bool upgrade = false;
    
    // 1. Read Headers
    unsigned long start = millis();
    while (client->connected() && millis() - start < 500) {
        if (client->available()) {
            String line = client->readStringUntil('\n');
            line.trim();
            if (line.length() == 0) break;
            
            if (line.indexOf("Upgrade: websocket") >= 0 || line.indexOf("Upgrade: WebSocket") >= 0) upgrade = true;
            if (line.startsWith("Sec-WebSocket-Key: ")) key = line.substring(19);
        }
    }
    
    if (!upgrade || key.length() == 0) {
        AddLog(LOG_LEVEL_ERROR, PSTR("WS: Handshake failed (No Upgrade or Key)"));
        return false;
    }
    
    AddLog(LOG_LEVEL_INFO, PSTR("WS: Client Key: %s"), key.c_str());

    // 2. Concatenate & Hash (Using BearSSL)
    String concat = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    uint8_t hash[20];
    
    // BearSSL Implementation
    // Force alignment to prevent ESP32 load/store errors on the context state
    br_sha1_context ctx __attribute__((aligned(4)));
    br_sha1_init(&ctx);
    br_sha1_update(&ctx, (const void*)concat.c_str(), concat.length());
    br_sha1_out(&ctx, hash);

    AddLog(LOG_LEVEL_INFO, PSTR("WS: Hash[0]: %02X (Expect 52 for key ujrx...)"), hash[0]);

    // 3. Base64 Encode (Reference Implementation)
    String acceptKey = "";
    const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    for(int i=0; i<18; i+=3) {
        uint32_t n = (hash[i] << 16) | (hash[i+1] << 8) | hash[i+2];
        acceptKey += b64[(n >> 18) & 63];
        acceptKey += b64[(n >> 12) & 63];
        acceptKey += b64[(n >> 6) & 63];
        acceptKey += b64[n & 63];
    }
    // Final block (18, 19)
    uint32_t n = (hash[18] << 16) | (hash[19] << 8);
    acceptKey += b64[(n >> 18) & 63];
    acceptKey += b64[(n >> 12) & 63];
    acceptKey += b64[(n >> 6) & 63];
    acceptKey += '=';

    AddLog(LOG_LEVEL_INFO, PSTR("WS: Accept Key: %s"), acceptKey.c_str());

    client->print("HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: " + acceptKey + "\r\n\r\n");
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
  
  // Start Servers for H264 Session (Multicast)
  if (!Wc.rtsp.server) { 
      Wc.rtsp.server = new WiFiServer(554); 
      Wc.rtsp.server->begin(); 
  }
  if (!Wc.ws.server) { 
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: Starting WS Server on 82"));
      Wc.ws.server = new WiFiServer(82);  
      Wc.ws.server->begin(); 
  }

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: H.264 encoder initialized (%dx%d, buffer=%d bytes)"), width, height, in_size);
  return 1;
}
/*********************************************************************************************/

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
    WiFiClient c = Wc.ws.server->available();
    if (WsPerformWsHandshake(&c)) {
        if (Wc.ws.client_ptr) delete Wc.ws.client_ptr;
        Wc.ws.client_ptr = new WiFiClient(c);
        Wc.ws.active = true;
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: WS Client Connected"));
    } else {
        c.stop();
    }
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

// NOTE (Safari/WebCodecs + WS H.264):
// Safari may not reliably start the ws:// H.264 pipeline on plain HTTP until triggered by a user gesture,
// so the WebCodecs demo page uses a Start button (and a small WS “kick” + reconnect) to force the socket
// into OPEN before decoding; once running, Safari decodes fine with correct key/delta signaling and SPS/PPS+IDR.
const char WC_H264_HTML[] PROGMEM = R"rawliteral(<!DOCTYPE html><html><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>ESP32 H.264</title><style>body{margin:0;background:#000;display:flex;justify-content:center;align-items:center;height:100vh}canvas{max-width:100%;max-height:100%}#ui{position:fixed;top:10px;left:10px;z-index:10;font:14px sans-serif;color:#fff}button{font:14px sans-serif;padding:6px 10px;margin-right:6px}</style></head><body><div id="ui"><button id="start">Start</button><button id="stop" disabled>Stop</button><span id="st">idle</span></div><canvas id="c"></canvas><script>(()=>{const canvas=document.getElementById('c'),ctx=canvas.getContext('2d');const bStart=document.getElementById('start'),bStop=document.getElementById('stop'),st=document.getElementById('st');console.log('[WC] location=',location.href,'secure=',window.isSecureContext,'proto=',location.protocol);console.log('[WC] VideoDecoder=',('VideoDecoder'in window),typeof VideoDecoder);const wsProto=(location.protocol==='https:')?'wss://':'ws://';const WS_URL=wsProto+location.hostname+':82/';let ws=null,decoder=null,haveKey=false,ts=0,run=false;let frames=0,keyFrames=0,deltaFrames=0,bytes=0,lastLog=performance.now();let lastFps=0,lastKbps=0,vidW=0,vidH=0;const fps=25;const frameDurUs=Math.round(1000000/fps);function setStatus(s){st.textContent=s;}function fmtKbps(kbps){if(!isFinite(kbps))return '0 kbps';if(kbps<1000)return Math.round(kbps)+' kbps';return (kbps/1000).toFixed(2)+' Mbps';}function wsStateName(s){return s===0?'CONNECTING':s===1?'OPEN':s===2?'CLOSING':s===3?'CLOSED':'?';}function drawHud(){try{const w=canvas.width|0,h=canvas.height|0;if(w<10||h<10)return;ctx.save();ctx.font='14px monospace';ctx.textBaseline='top';const lines=[`FPS: ${lastFps}`,`Rate: ${fmtKbps(lastKbps)}`,`Res: ${vidW}x${vidH}`,`Frames: key=${keyFrames} delta=${deltaFrames}`,`WS: ${ws?wsStateName(ws.readyState):'none'} (${ws?ws.readyState:'-'})`,`Dec: ${decoder?decoder.state:'none'} q=${decoder?decoder.decodeQueueSize:'-'}`,`Secure: ${window.isSecureContext}`];let maxW=0;for(const s of lines){maxW=Math.max(maxW,ctx.measureText(s).width);}const pad=8;const boxW=Math.min(w-10,Math.ceil(maxW+pad*2));const boxH=lines.length*18+pad*2;ctx.globalAlpha=0.65;ctx.fillStyle='#000';ctx.fillRect(8,8,boxW,boxH);ctx.globalAlpha=1;ctx.fillStyle='#0f0';let y=8+pad;for(const s of lines){ctx.fillText(s,8+pad,y);y+=18;}ctx.restore();}catch(e){}}function hardStop(){run=false;try{if(ws){ws.onopen=ws.onmessage=ws.onerror=ws.onclose=null;ws.close();}}catch(e){}ws=null;try{if(decoder){decoder.close();}}catch(e){}decoder=null;haveKey=false;ts=0;frames=keyFrames=deltaFrames=bytes=0;lastFps=0;lastKbps=0;setStatus('stopped');bStart.disabled=false;bStop.disabled=true;console.log('[WC] stopped');}function kickWebSocket(url){try{const k=new WebSocket(url);k.onopen=()=>{console.log('[WC] kick WS open/close');try{k.close();}catch(e){}};k.onerror=(e)=>console.log('[WC] kick WS error',e);}catch(e){console.log('[WC] kick WS ctor threw',e);}}function openWebSocket(url,timeoutMs){return new Promise((resolve,reject)=>{let w;try{w=new WebSocket(url);}catch(e){reject(e);return;}const t=setTimeout(()=>{try{console.warn('[WC] WS connect timeout, closing (readyState=',w.readyState,')');w.close();}catch(e){}reject(new Error('WS connect timeout'));},timeoutMs);w.onopen=()=>{clearTimeout(t);resolve(w);};w.onerror=(e)=>{clearTimeout(t);reject(e);};});}function scanAnnexBTypes(u8){const t=new Set();for(let i=0;i+4<u8.length;i++){let sc=0;if(u8[i]===0&&u8[i+1]===0&&u8[i+2]===1)sc=3;else if(u8[i]===0&&u8[i+1]===0&&u8[i+2]===0&&u8[i+3]===1)sc=4;if(!sc)continue;const b=u8[i+sc];if(b!==undefined)t.add(b&31);}return Array.from(t).sort((a,b)=>a-b);}async function startStream(){hardStop();run=true;bStart.disabled=true;bStop.disabled=false;setStatus('starting...');haveKey=false;ts=0;frames=keyFrames=deltaFrames=bytes=0;lastLog=performance.now();decoder=new VideoDecoder({output:f=>{vidW=f.displayWidth;vidH=f.displayHeight;if(canvas.width!==vidW)canvas.width=vidW;if(canvas.height!==vidH)canvas.height=vidH;ctx.drawImage(f,0,0);drawHud();f.close()},error:e=>{console.error('[WC] VideoDecoder error:',e,'state=',decoder&&decoder.state,'q=',decoder&&decoder.decodeQueueSize);setStatus('decoder error');}});const cfg={codec:'avc1.42001E',optimizeForLatency:true};try{if(VideoDecoder.isConfigSupported){const s=await VideoDecoder.isConfigSupported(cfg);console.log('[WC] isConfigSupported=',s);}}catch(e){console.log('[WC] isConfigSupported error',e);}try{decoder.configure(cfg);console.log('[WC] configured',cfg,'state=',decoder.state);setStatus('decoder configured');}catch(e){console.error('[WC] configure failed',e);setStatus('configure failed');return;}console.log('[WC] connecting WS',WS_URL);kickWebSocket(WS_URL);await new Promise(r=>setTimeout(r,80));try{ws=await openWebSocket(WS_URL,1500);console.log('[WC] WS open',ws.url);setStatus('WS open');}catch(e){console.error('[WC] WS open failed',e);setStatus('WS open failed');if(run){console.log('[WC] retry in 500ms');setTimeout(()=>{if(run)startStream();},500);}return;}ws.binaryType='arraybuffer';ws.onclose=(e)=>{console.log('[WC] WS close code=',e.code,'reason=',e.reason,'wasClean=',e.wasClean);setStatus('WS closed');if(run){console.log('[WC] reconnect in 500ms');setTimeout(()=>{if(run)startStream();},500);}};ws.onerror=(e)=>{console.error('[WC] WS error',e);setStatus('WS error');};ws.onmessage=(e)=>{try{const u8=new Uint8Array(e.data);bytes+=u8.byteLength;if(u8.length<2){console.warn('[WC] short packet',u8.length);return;}const isKey=(u8[0]===1);if(!haveKey&&!isKey){console.warn('[WC] drop delta before first key');return;}if(isKey&&!haveKey){haveKey=true;const data0=u8.subarray(1);console.log('[WC] first KEY received bytes=',u8.length);console.log('[WC] key NAL types=',scanAnnexBTypes(data0));console.log('[WC] key head=',Array.from(data0.subarray(0,32)).map(b=>b.toString(16).padStart(2,'0')).join(' '));setStatus('decoding');}if(isKey)keyFrames++;else deltaFrames++;const data=u8.subarray(1);const chunk=new EncodedVideoChunk({type:isKey?'key':'delta',timestamp:ts,data:data});ts+=frameDurUs;decoder.decode(chunk);frames++;const now=performance.now();if(now-lastLog>1000){lastFps=frames;lastKbps=(bytes*8)/1000;console.log('[WC] fps=',frames,'key=',keyFrames,'delta=',deltaFrames,'KB=',Math.round(bytes/1024),'state=',decoder.state,'q=',decoder.decodeQueueSize,'haveKey=',haveKey,'wsState=',ws.readyState);frames=0;bytes=0;lastLog=now;}}catch(x){console.error('[WC] decode exception',x,'state=',decoder&&decoder.state,'q=',decoder&&decoder.decodeQueueSize,'haveKey=',haveKey);setStatus('decode exception');}};}bStart.onclick=()=>{console.log('[WC] Start clicked');startStream();};bStop.onclick=()=>{console.log('[WC] Stop clicked');hardStop();};})();</script></body></html>)rawliteral";

void HandleWebcamH264Html() { if (Webserver) Webserver->send(200, "text/html", WC_H264_HTML); }

#endif  // USE_CSI_WEBCAM
#endif  // ESP32P4