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

// Configuration - what Berry tells us about the sensor
struct CSI_Config {
  uint16_t width;
  uint16_t height;
  uint8_t format;           // 0=RAW8, 1=RAW10, 2=RGB565, 3=YUV422
  uint8_t has_isp;          // Sensor has onboard ISP?
  uint32_t lane_bitrate;    // MIPI lane bitrate
  uint8_t lane_num;         // Number of CSI lanes
};

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
  volatile bool frame_ready;   // Flag set by ISR
  
  uint8_t up;
  bool streaming;
  uint8_t stream_active;
  WiFiClient *client_ptr;   // Pointer to avoid client() issues
  ESP8266WebServer *CamServer;
} Wc;



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

// Callback: Frame transfer finished
static bool IRAM_ATTR csi_on_trans_finished(esp_cam_ctlr_handle_t handle, esp_cam_ctlr_trans_t *trans, void *user_data) {
  cb_finished_count++;
  
  // The buffer we just finished writing is now the readable one
  Wc.read_idx = Wc.write_idx; 
  Wc.frame_ready = true;
  
  return false; 
}


/*********************************************************************************************/

uint32_t WcSetup() {
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

  // PRE-FILL CONFIG WITH DEFAULTS
  Wc.config.width = 800;
  Wc.config.height = 640;
  Wc.config.format = 0;        // RAW8
  Wc.config.has_isp = 0;
  Wc.config.lane_bitrate = 200;
  Wc.config.lane_num = 2;

  // 2. Call Berry to initialize sensor
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Calling Berry init for sensor..."));
  int32_t result = callBerryEventDispatcher(PSTR("camera"), PSTR("init"), 0, (char*)&Wc.config, sizeof(CSI_Config));
  
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Berry init result=%d"), result);
  
  if (result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry init failed or no driver loaded"));
    return 0;
  }
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Berry configured %dx%d format=%d bitrate=%d lanes=%d"), 
    Wc.config.width, Wc.config.height, Wc.config.format, Wc.config.lane_bitrate, Wc.config.lane_num);

  // 3. Allocate Two Frame Buffers (Aligned 64-byte for JPEG DMA)
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
    .lane_bit_rate_mbps = (int)Wc.config.lane_bitrate,
    .input_data_color_type = CAM_CTLR_COLOR_RAW8,
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
  esp_isp_processor_cfg_t isp_config = {
    .clk_hz = 80 * 1000 * 1000,
    .input_data_source = ISP_INPUT_DATA_SOURCE_CSI,
    .input_data_color_type = ISP_COLOR_RAW8,
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

  Wc.up = 1;
  Wc.streaming = false;

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Setup complete"));
  return Wc.up;
}

uint32_t WcStart(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: WcStart called - up=%d streaming=%d"), Wc.up, Wc.streaming);
  
  if (!Wc.up) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CSI not initialized"));
    return 0;
  }

  if (Wc.streaming) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Already streaming"));
    return 1;
  }

  // CSI controller is already started during setup
  // Just need to start sensor streaming via Berry
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Calling Berry stream_on"));
  int32_t berry_result = callBerryEventDispatcher(PSTR("camera"), PSTR("stream_on"), 0, nullptr, 0);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Berry stream_on result: %d"), berry_result);
  
  if (berry_result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry stream_on failed"));
    return 0;
  }
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Sensor streaming started"));
  
  // Give sensor time to start streaming
  delay(100);

  Wc.streaming = true;
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Ready - backup buffer should update automatically"));
  return 1;
}

uint32_t WcStop(void) {
  if (!Wc.up || !Wc.streaming) {
    return 0;
  }

  esp_err_t ret = esp_cam_ctlr_stop(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to stop CSI (0x%x)"), ret);
    return 0;
  }

  // Call Berry to stop sensor streaming
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Calling Berry stream_off"));
  callBerryEventDispatcher(PSTR("camera"), PSTR("stream_off"), 0, nullptr, 0);

  Wc.streaming = false;
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

#define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"

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
  
  // Allocate client on heap to avoid stack issues
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Allocating client..."));
  if (Wc.client_ptr) {
    delete Wc.client_ptr;
    Wc.client_ptr = nullptr;
  }
  
  Wc.client_ptr = new WiFiClient();
  if (!Wc.client_ptr) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate client!"));
    return;
  }
  
  *Wc.client_ptr = Wc.CamServer->client();
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client allocated, connected=%d"), Wc.client_ptr->connected());
  
  Wc.stream_active = 1;
}

void HandleWebcamMjpegTask(void) {
  if (!Wc.stream_active || !Wc.client_ptr || !Wc.client_ptr->connected()) {
    if (Wc.client_ptr) { delete Wc.client_ptr; Wc.client_ptr = nullptr; }
    Wc.stream_active = 0;
    return;
  }
  
  if (Wc.stream_active == 1) {
    Wc.client_ptr->print("HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n\r\n");
    Wc.stream_active = 2;
  }

  if (Wc.stream_active == 2 && Wc.frame_ready) {
    
    // 1. Grab 'readable' buffer (Ping-Pong)
    // Hardware is writing to write_idx, we read from read_idx
    uint8_t *source_buf = Wc.frame_buffer[Wc.read_idx];
    Wc.frame_ready = false; 

    // 2. Sync Cache (M2C) - No memcpy needed!
    esp_cache_msync(source_buf, Wc.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);
    
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
      return; 
    }

    if (ret == ESP_OK && jpeg_size > 0) {
       Wc.client_ptr->print("--" BOUNDARY "\r\n");
       Wc.client_ptr->printf("Content-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n", jpeg_size);
       Wc.client_ptr->write((const uint8_t*)Wc.jpeg_buffer, jpeg_size);
       Wc.client_ptr->print("\r\n");
    }
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

  // Poll for next frame
  Wc.frame_ready = false;
  uint32_t start = millis();
  while(!Wc.frame_ready && millis() - start < 1000) { delay(1); }
  
  if (Wc.frame_ready) {
    uint8_t *source_buf = Wc.frame_buffer[Wc.read_idx];
    esp_cache_msync(source_buf, Wc.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);

    uint32_t jpeg_size = 0;
    esp_err_t ret = jpeg_encoder_process(Wc.jpeg_handle, &Wc.jpeg_cfg, source_buf, Wc.frame_buffer_size, (uint8_t*)Wc.jpeg_buffer, Wc.jpeg_buffer_size, &jpeg_size);
    if (ret == ESP_OK && jpeg_size > 0) {
      client.write((char *)Wc.jpeg_buffer, jpeg_size);
    }
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
    WSContentSend_P(PSTR("<p></p><center><img onerror='setTimeout(()=>{this.src=this.src;},1000)' src='http://%_I:81/stream' alt='Webcam stream' style='width:99%%;'></center><p></p>"),(uint32_t)WiFi.localIP());
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
  // ;WcTestReceive();
  
  if (Wc.streaming && !Wc.CamServer && !TasmotaGlobal.global_state.network_down) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Starting stream server..."));
    WcSetStreamserver(1);
  }
  
  if (Wc.CamServer) {
    Wc.CamServer->handleClient();
    if (Wc.stream_active) { 
      HandleWebcamMjpegTask(); 
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
        WcSetup();
      }
      break;
    case FUNC_EVERY_SECOND:
      // Auto-start streaming once WiFi is available
      if (Wc.up && !Wc.streaming && !TasmotaGlobal.global_state.network_down) {
        WcStart();
      }
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif  // USE_WEBCAM_CSI
#endif  // ESP32P4
