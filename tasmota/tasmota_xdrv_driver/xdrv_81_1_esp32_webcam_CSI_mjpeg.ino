/*
  xdrv_81_1_esp32_webcam_CSI_mjpeg.ino - MJPEG Streaming for Tasmota ESP32-P4

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
 * ESP32-P4 MIPI CSI Camera Driver - MJPEG Module
 * 
 * Architecture:
 * - C-Core: Generic CSI controller (xdrv_81_0_esp32_webcam_CSI_core.ino)
 * - Berry: Sensor-specific logic (I2C, registers, power control)
 * - MJPEG Module: MJPEG streaming support (this file)
 * - H.264 Module: H.264 streaming support (xdrv_81_2_esp32_webcam_CSI_h264.ino)
 * 
 * Responsibilities:
 * - MJPEG encoder initialization and configuration
 * - MJPEG frame processing task
\*********************************************************************************************/


uint32_t WcSetupJpegEncoder(void) {
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


// --- MJPEG / HTTP Streaming Module ---
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

uint32_t WcSetMjpegServer(uint32_t enable) {
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: WcSetMjpegServer enable=%d CamServer=%p"), enable, Wc.jpeg.server);
  
  if (enable && TasmotaGlobal.global_state.network_down) { 
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Network down, aborting"));
    Wc.jpeg.stream_active = 0;
    return 0; 
  }

  if (enable) {
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

#endif  // USE_CSI_WEBCAM
#endif  // ESP32P4