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

#ifdef ESP32P4
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
#include "esp_cache.h"

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
  void *frame_buffer;
  size_t frame_buffer_size;
  
  CSI_Config config;        // Sensor configuration
  
  uint8_t up;
  bool streaming;
  uint8_t stream_active;
  WiFiClient client;
  ESP8266WebServer *CamServer;
} Wc;


/*********************************************************************************************/

// Callback: Provide new buffer for next frame
static bool IRAM_ATTR csi_on_get_new_vb(esp_cam_ctlr_handle_t handle, esp_cam_ctlr_trans_t *trans, void *user_data) {
  esp_cam_ctlr_trans_t *new_trans = (esp_cam_ctlr_trans_t *)user_data;
  trans->buffer = new_trans->buffer;
  trans->buflen = new_trans->buflen;
  return false;
}

// Callback: Frame transfer finished
static bool IRAM_ATTR csi_on_trans_finished(esp_cam_ctlr_handle_t handle, esp_cam_ctlr_trans_t *trans, void *user_data) {
  // Frame is ready in buffer
  // Could signal semaphore here if needed
  return false;
}

/*********************************************************************************************/

uint32_t WcSetup() {
  if (Wc.up) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI already initialized"));
    return Wc.up;
  }

  // Fill config with defaults for OV5647
  Wc.config.width = 800;
  Wc.config.height = 640;
  Wc.config.format = 0;        // RAW8
  Wc.config.has_isp = 0;       // No onboard ISP
  Wc.config.lane_bitrate = 400;
  Wc.config.lane_num = 2;

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI init %dx%d @ %d Mbps"), Wc.config.width, Wc.config.height, Wc.config.lane_bitrate);


  // Call Berry to initialize sensor
  // Berry will detect sensor, configure it, and update config struct
  int32_t result = callBerryEventDispatcher(PSTR("camera"), PSTR("init"), 0, (const char*)&Wc.config, sizeof(CSI_Config));
  
  if (result == 0) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: No Berry camera driver"));
    return 0;
  }
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Berry configured %dx%d format=%d"), 
    Wc.config.width, Wc.config.height, Wc.config.format);
  
  // Allocate frame buffer in PSRAM
  Wc.frame_buffer_size = Wc.config.width * Wc.config.height * 2; // RGB565 = 2 bytes per pixel

  Wc.frame_buffer = heap_caps_calloc(1, Wc.frame_buffer_size,
                                    MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (!Wc.frame_buffer) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to allocate frame buffer (%d bytes)"), Wc.frame_buffer_size);
    return 0;
  }

  esp_cache_msync(Wc.frame_buffer, Wc.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_C2M);

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Frame buffer allocated at %p (%d bytes)"), Wc.frame_buffer, Wc.frame_buffer_size);

  // Determine color types based on sensor capabilities
  cam_ctlr_color_t input_color, output_color;
  bool use_esp32_isp = false;
  
  if (Wc.config.has_isp) {
    // Sensor has onboard ISP, outputs processed format (RGB565/YUV422)
    if (Wc.config.format == 2) {
      input_color = CAM_CTLR_COLOR_RGB565;
      output_color = CAM_CTLR_COLOR_RGB565;
    } else if (Wc.config.format == 3) {
      input_color = CAM_CTLR_COLOR_YUV422;
      output_color = CAM_CTLR_COLOR_YUV422;
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Unknown sensor format %d"), Wc.config.format);
      return 0;
    }
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Sensor has ISP, no ESP32 ISP needed"));
  } else {
    // Sensor outputs RAW, need ESP32 ISP to convert
    if (Wc.config.format == 0) {
      input_color = CAM_CTLR_COLOR_RAW8;
    } else if (Wc.config.format == 1) {
      input_color = CAM_CTLR_COLOR_RAW10;
    } else {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Unknown RAW format %d"), Wc.config.format);
      return 0;
    }
    output_color = CAM_CTLR_COLOR_RGB565;
    use_esp32_isp = true;
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Sensor outputs RAW, will use ESP32 ISP"));
  }

  // Configure CSI controller
  esp_cam_ctlr_csi_config_t csi_config = {
    .ctlr_id = 0,
    .h_res = Wc.config.width,
    .v_res = Wc.config.height,
    .data_lane_num = Wc.config.lane_num,
    .lane_bit_rate_mbps = (int)Wc.config.lane_bitrate,
    .input_data_color_type = input_color,
    .output_data_color_type = output_color,
    .queue_items = 1,
    .byte_swap_en = false,
  };

  esp_err_t ret = esp_cam_new_csi_ctlr(&csi_config, &Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CSI controller init failed (0x%x)"), ret);
    free(Wc.frame_buffer);
    Wc.frame_buffer = NULL;
    return 0;
  }

  // Setup transaction for frame buffer
  esp_cam_ctlr_trans_t new_trans = {
    .buffer = Wc.frame_buffer,
    .buflen = Wc.frame_buffer_size,
  };

  // Register callbacks
  esp_cam_ctlr_evt_cbs_t cbs = {
    .on_get_new_trans = csi_on_get_new_vb,
    .on_trans_finished = csi_on_trans_finished,
  };

  ret = esp_cam_ctlr_register_event_callbacks(Wc.cam_handle, &cbs, &new_trans);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to register callbacks (0x%x)"), ret);
    esp_cam_ctlr_del(Wc.cam_handle);
    free(Wc.frame_buffer);
    Wc.frame_buffer = NULL;
    return 0;
  }

  // Enable CSI controller
  ret = esp_cam_ctlr_enable(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to enable CSI controller (0x%x)"), ret);
    esp_cam_ctlr_del(Wc.cam_handle);
    free(Wc.frame_buffer);
    Wc.frame_buffer = NULL;
    return 0;
  }

  // Configure ISP (Image Signal Processor) only if needed
  if (use_esp32_isp) {
    isp_color_t isp_input_color;
    if (Wc.config.format == 0) {
      isp_input_color = ISP_COLOR_RAW8;
    } else {
      isp_input_color = ISP_COLOR_RAW10;
    }
    
    esp_isp_processor_cfg_t isp_config = {
      .clk_hz = 80 * 1000 * 1000,
      .input_data_source = ISP_INPUT_DATA_SOURCE_CSI,
      .input_data_color_type = isp_input_color,
      .output_data_color_type = ISP_COLOR_RGB565,
      .has_line_start_packet = false,
      .has_line_end_packet = false,
      .h_res = Wc.config.width,
      .v_res = Wc.config.height,
    };

    ret = esp_isp_new_processor(&isp_config, &Wc.isp_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ISP init failed (0x%x)"), ret);
      esp_cam_ctlr_disable(Wc.cam_handle);
      esp_cam_ctlr_del(Wc.cam_handle);
      free(Wc.frame_buffer);
      Wc.frame_buffer = NULL;
      return 0;
    }

    ret = esp_isp_enable(Wc.isp_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to enable ISP (0x%x)"), ret);
      esp_isp_del_processor(Wc.isp_handle);
      esp_cam_ctlr_disable(Wc.cam_handle);
      esp_cam_ctlr_del(Wc.cam_handle);
      free(Wc.frame_buffer);
      Wc.frame_buffer = NULL;
      return 0;
    }
    
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: ESP32 ISP enabled"));
  } else {
    Wc.isp_handle = NULL;
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: ESP32 ISP not needed"));
  }

  Wc.up = 1;
  Wc.streaming = false;

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: CSI initialized successfully"));
  return Wc.up;
}

uint32_t WcStart(void) {
  if (!Wc.up) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: CSI not initialized"));
    return 0;
  }

  if (Wc.streaming) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Already streaming"));
    return 1;
  }

  // Call Berry to start sensor streaming (register 0x0100)
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Calling Berry stream_on"));
  callBerryEventDispatcher(PSTR("camera"), PSTR("stream_on"), 0, nullptr, 0);
  
  delay(10);

  esp_err_t ret = esp_cam_ctlr_start(Wc.cam_handle);
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to start CSI (0x%x)"), ret);
    return 0;
  }

  Wc.streaming = true;
  
  // Start the stream server
  WcSetStreamserver(1);
  
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Streaming started"));
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

uint8_t* WcGetFrameCSI(uint32_t timeout_ms) {
  if (!Wc.up || !Wc.streaming) {
    return NULL;
  }

  esp_cam_ctlr_trans_t trans = {
    .buffer = Wc.frame_buffer,
    .buflen = Wc.frame_buffer_size,
  };

  esp_err_t ret = esp_cam_ctlr_receive(Wc.cam_handle, &trans, pdMS_TO_TICKS(timeout_ms));
  if (ret != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Frame receive timeout or error (0x%x)"), ret);
    return NULL;
  }

  // Sync cache
  esp_cache_msync(Wc.frame_buffer, Wc.frame_buffer_size, ESP_CACHE_MSYNC_FLAG_DIR_M2C);

  return (uint8_t*)Wc.frame_buffer;
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
// - Currently streams RGB565 raw data (TODO: add JPEG conversion)
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
  Wc.CamServer->sendHeader("Location", "/cam.mjpeg");
  Wc.CamServer->send(302, "", "");
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Root called"));
}

void HandleWebcamMjpeg(void) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Handle camserver"));
  Wc.stream_active = 1;
  Wc.client = Wc.CamServer->client();
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Create client"));
}

void HandleWebcamMjpegTask(void) {
  if (!Wc.client.connected()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client fail"));
    Wc.stream_active = 0;
    return;
  }
  
  if (1 == Wc.stream_active) {
    Wc.client.flush();
    Wc.client.setTimeout(3);
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Start stream"));
    Wc.client.print("HTTP/1.1 200 OK\r\n"
      "Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n"
      "\r\n");
    Wc.stream_active = 2;
  }

  if (2 == Wc.stream_active) {
    // Get frame from CSI camera
    uint8_t *frame = WcGetFrameCSI(1000);
    if (!frame) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Frame fail"));
      Wc.stream_active = 0;
      return;
    }

    // For now, send RGB565 data directly
    // TODO: Convert to JPEG for better streaming
    size_t frame_len = Wc.frame_buffer_size;

    Wc.client.print("--" BOUNDARY "\r\n");
    Wc.client.printf("Content-Type: image/rgb565\r\n"
      "Content-Length: %d\r\n"
      "\r\n", static_cast<int>(frame_len));
    Wc.client.write((char *)frame, frame_len);
    Wc.client.print("\r\n");
  }
  
  if (0 == Wc.stream_active) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stream exit"));
    Wc.client.flush();
    Wc.client.stop();
  }
}

void HandleImage(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  WiFiClient client = Webserver->client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.jpg\r\n";
  response += "Content-type: image/rgb565\r\n\r\n";
  Webserver->sendContent(response);

  // Get a single frame
  uint8_t *frame = WcGetFrameCSI(1000);
  if (frame) {
    client.write((char *)frame, Wc.frame_buffer_size);
  }
  client.stop();

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CAM: Sending image"));
}

uint32_t WcSetStreamserver(uint32_t flag) {
  if (TasmotaGlobal.global_state.network_down) { 
    Wc.stream_active = 0;
    return 0; 
  }

  if (flag) {
    if (!Wc.CamServer) {
      Wc.stream_active = 0;
      Wc.CamServer = new ESP8266WebServer(81);
      Wc.CamServer->on("/", HandleWebcamRoot);
      Wc.CamServer->on("/cam.mjpeg", HandleWebcamMjpeg);
      Wc.CamServer->on("/cam.jpg", HandleWebcamMjpeg);
      Wc.CamServer->on("/stream", HandleWebcamMjpeg);
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stream init"));
      Wc.CamServer->begin();
    }
  } else {
    if (Wc.CamServer) {
      Wc.stream_active = 0;
      Wc.CamServer->stop();
      delete Wc.CamServer;
      Wc.CamServer = NULL;
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stream exit"));
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

void WcLoop(void) {
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
      if(Wc.up == 0) WcSetup();
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  return result;
}

#endif  // USE_WEBCAM_CSI
#endif  // ESP32P4
