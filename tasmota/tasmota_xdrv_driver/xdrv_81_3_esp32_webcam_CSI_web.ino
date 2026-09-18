/*
  xdrv_81_3_esp32_webcam_CSI_web.ino - Web UI and Console Interface for Tasmota

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
  
  Responsibilities:
  - Tasmota Web UI integration (WcShowStream)
  - Console Command handling (CmndWc*)
  - Routing user requests to the appropriate subsystem (MJPEG vs H.264)
*/

#ifdef ESP32
#ifdef USE_CSI_WEBCAM

/*********************************************************************************************\
 * Tasmota Web UI Integration
 * 
 * Routes the stream display based on the active session type.
\*********************************************************************************************/

void WcShowStream(void) {
  if (Wc.core.state != CAM_STREAMING) {
    return;
  }

  // ---- Session 1: MJPEG (Legacy <img> tag) ----
  if (Wc.core.session_type == SESSION_MJPEG_HTTP) {
    if (!Wc.jpeg.server) return; // Server not ready

    uint32_t ip = (uint32_t)NetworkAddress();

    WSContentSend_P(PSTR("<div><div id='wc_s'>Loading...</div>"));

    // Render standard MJPEG Image tag
    WSContentSend_P(PSTR("<img id='wc_img' style='max-width:100%%;' "
                         "src='http://%_I:81/stream' "
                         "onerror='setTimeout(()=>{this.src=this.src;},1000)'>"), ip);

    // JS for resolution display
    WSContentSend_P(PSTR(
      "<script>"
      "setInterval(function(){"
      "  var i=eb('wc_img');"
      "  if(i && i.naturalWidth){"
      "    eb('wc_s').innerHTML='MJPEG: '+i.naturalWidth+'x'+i.naturalHeight;"
      "  }"
      "},1000);"
      "</script></div>"
    ));
    return;
  }

  // ---- Session 2: H.264 (WebCodecs Canvas) ----
  else if (Wc.core.session_type == SESSION_RTSP_AND_WS) {
    // Dispatch to H.264 module to inject the player
    WcH264SendUi(); // Using global Wc.core.config inside
    return;
  }

  // ---- Session 3: WebRTC ----
  else if (Wc.core.session_type == SESSION_WEBRTC) {
    WSContentSend_P(PSTR(
      "<div><video id='wc_vid' autoplay muted controls playsinline style='width:100%%;background:#000'></video></div>"
      "<div id='wc_dbg' style='font-family:monospace;font-size:10px;margin-top:5px;max-height:200px;overflow-y:auto'></div>"
      "<script>"
      "const dbg=s=>{var d=document.getElementById('wc_dbg');d.innerHTML+=s+'<br>';d.scrollTop=d.scrollHeight;};"

      "const rpc = new RTCPeerConnection({"
      "  iceServers: []," // LAN only, no STUN needed
      "  sdpSemantics: 'unified-plan'"
      "});"
      
      "rpc.ontrack = function(evt) {"
      "  dbg('ontrack: kind='+evt.track.kind+' id='+evt.track.id+' state='+evt.track.readyState+' streams='+evt.streams.length);"
      "  var v=document.getElementById('wc_vid');"
      "  if(evt.streams&&evt.streams[0]){"
      "    v.srcObject=evt.streams[0];"
      "  }else if(!v.srcObject){"
      "    var ms=new MediaStream();"
      "    ms.addTrack(evt.track);"
      "    v.srcObject=ms;"
      "  }else{"
      "    v.srcObject.addTrack(evt.track);"
      "  }"
      "  if(evt.track.kind==='audio'){"
      "    dbg('Audio track attached, muted='+v.muted);"
      "  }"
      "};"
      
      "rpc.onicecandidate = function(evt) {"
      "  if(evt.candidate){"
      "    dbg('ICE candidate: '+evt.candidate.candidate);"
      "  }else{"
      "    dbg('ICE gathering complete');"
      "  }"
      "};"
      
      "rpc.oniceconnectionstatechange=()=>{"
      "  dbg('ICE state: '+rpc.iceConnectionState);"
      "  document.title='ICE:'+rpc.iceConnectionState;"
      "};"
      
      "rpc.onicegatheringstatechange=()=>{"
      "  dbg('ICE gathering: '+rpc.iceGatheringState);"
      "};"
      
      "rpc.onsignalingstatechange=()=>{"
      "  dbg('Signaling: '+rpc.signalingState);"
      "};"
      
      "rpc.addTransceiver('video', {direction: 'recvonly'});"
      "rpc.addTransceiver('audio', {direction: 'recvonly'});"

      "dbg('Creating offer...');"
      "rpc.createOffer().then(offer=>{"
      "  dbg('Offer created, setting local desc');"
      "  return rpc.setLocalDescription(offer);"
      "}).then(()=>{"
      "  dbg('Sending offer to server');"
      "  return fetch('/webrtc/offer',{"
      "    method:'POST',"
      "    headers:{'Content-Type':'text/plain'},"
      "    body:rpc.localDescription.sdp"
      "  });"
      "}).then(r=>{"
      "  if(!r.ok)return r.text().then(t=>{throw new Error('Server '+r.status+': '+t)});"
      "  return r.text();"
      "})"
      ".then(sdp=>{"
      "  if(!sdp.startsWith('v=')){throw new Error('Invalid SDP: '+sdp.substring(0,60));}"
      "  dbg('Got answer, setting remote desc');"
      "  console.log('Answer SDP:',sdp);"
      "  return rpc.setRemoteDescription({type:'answer',sdp:sdp});"
      "}).then(()=>{"
      "  dbg('Remote desc set OK');"
      "  console.log('Remote desc set OK');"
      "})"
      ".catch(e=>{"
      "  dbg('ERROR: '+e.message);"
      "  document.title='ERR:'+e.message;"
      "  console.error(e);"
      "});"
      "</script>"
    ));
    return;
  }
}

/*********************************************************************************************\
 * Console Commands
 * 
 * Moved from Core to keep hardware logic clean.
\*********************************************************************************************/

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
  bool was_failed = (Wc.core.state == CAM_FAILED);
  
  // If failed, clear failure state first
  if (was_failed) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Clearing failed state for resolution change"));
    Wc.core.fail_reason = CAM_FAIL_NONE;
    Wc.core.fail_esp_err = ESP_OK;
    Wc.core.state = CAM_IDLE;
  }
  
  // 1. Pause task first if streaming (skip if was failed or idle)
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
    ResponseCmndFailed();
    return;
  }
  
  // 6. Re-Initialize Hardware Pipeline
  if (!WcInitPipeline()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Pipeline Init Failed"));
    ResponseCmndFailed();
    return;
  }
  
  // 7. Restart CSI and sensor streaming (if we were streaming before)
  if (was_streaming) {
    esp_err_t ret = esp_cam_ctlr_start(Wc.core.cam_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to restart CSI (0x%x)"), ret);
      ResponseCmndFailed();
      return;
    }
    
    int32_t berry_result = callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 1, nullptr, 0);
    if (berry_result == 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry stream_on failed"));
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
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Sensor Init Failed\"}}"));
    return;
  }

  // 6. Re-Initialize Hardware
  if (!WcInitPipeline()) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Pipeline Init Failed"));
    Response_P(PSTR("{\"WcWindow\":{\"Error\":\"Pipeline Init Failed\"}}"));
    return;
  }

  // 7. Restart CSI and sensor streaming (if we were streaming before)
  if (was_streaming) {
    esp_err_t ret = esp_cam_ctlr_start(Wc.core.cam_handle);
    if (ret != ESP_OK) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Failed to restart CSI (0x%x)"), ret);
      Response_P(PSTR("{\"WcWindow\":{\"Error\":\"CSI Start Failed\"}}"));
      return;
    }
    
    int32_t berry_result = callBerryEventDispatcher(PSTR("camera"), PSTR("stream"), 1, nullptr, 0);
    if (berry_result == 0) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Berry stream_on failed"));
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
  
  const char* state_names[] = {"IDLE", "INIT", "STREAMING", "PAUSING", "PAUSED", "STOPPING", "FAILED"};
  const char* state_name = (Wc.core.state < 7) ? state_names[Wc.core.state] : "UNKNOWN";
  
  const char* fail_names[] = {"NONE", "MEMORY", "BERRY_INIT", "BERRY_STREAM", "CSI_INIT", "ISP_INIT", "ENCODER_INIT", "MUTEX", "TASK", "LDO"};
  const char* fail_name = (Wc.core.fail_reason < 10) ? fail_names[Wc.core.fail_reason] : "UNKNOWN";

  // Detailed Status JSON
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
                  "},"
                  "\"Failure\":{"
                  "\"Reason\":\"%s\",\"EspError\":\"0x%x\""
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
             WcStats.compression_ratio_x100 / 100.0f,
             fail_name,
             Wc.core.fail_esp_err);
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

void CmndWcQuality(void) {
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: WcQuality called, payload=%d"), XdrvMailbox.payload);
  
  if (XdrvMailbox.payload < 0) {
    ResponseCmndNumber(Wc.jpeg.quality);
    return;
  }
  
  if (XdrvMailbox.payload < 1 || XdrvMailbox.payload > 100) {
    ResponseCmndFailed();
    return;
  }
  
  Wc.jpeg.quality = (uint8_t)XdrvMailbox.payload;
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: encoder quality set to %d"), Wc.jpeg.quality);
  ResponseCmndNumber(Wc.jpeg.quality);
}

void CmndWcSession(void) {
  // Session types: 0=None, 1=MJPEG, 2=RTSP, 3=WebRTC
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
  if (XdrvMailbox.payload > 3) {
    ResponseCmndFailed();
    return;
  }

  camera_session_t new_type = (camera_session_t)XdrvMailbox.payload;

  if (new_type == Wc.core.session_type) {
    Response_P(PSTR("{\"WcSession\":{\"Type\":%d,\"Name\":\"%s\"}}"), 
               Wc.core.session_type, session_names[Wc.core.session_type]);
    return;
  }
  
  // Implemented Checks
  if (new_type > 3) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Session type %d not yet implemented"), new_type);
    Response_P(PSTR("{\"WcSession\":{\"Error\":\"Not implemented\",\"Requested\":%d}}"), new_type);
    return;
  }

  // --- Perform Switch ---
  
  if (Wc.core.state != CAM_IDLE && Wc.core.state != CAM_FAILED) {
    WcStop();
  } else if (Wc.core.state == CAM_FAILED) {
    // Clear failure state
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Clearing failed state for session change"));
    Wc.core.fail_reason = CAM_FAIL_NONE;
    Wc.core.fail_esp_err = ESP_OK;
    Wc.core.state = CAM_IDLE;
  }

  Wc.core.session_type = new_type;
  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Session type changed to %d (%s)"), 
         Wc.core.session_type, session_names[Wc.core.session_type]);

  // Special Setup for RTSP (Session 2)
  if (new_type == SESSION_RTSP_AND_WS) {
    Wc.rtp.sequence = random(0, 65535);
    Wc.rtp.timestamp = random(0, UINT32_MAX);
    Wc.rtp.ssrc = random(0, UINT32_MAX);
    Wc.rtsp.streaming = false;
    
    // Start RTSP/WS Servers
    if (!Wc.rtsp.server) {
        Wc.rtsp.server = new WiFiServer(554);
        Wc.rtsp.server->begin();
    }
    if (!Wc.ws.server) {
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: Starting WS Server on 82"));
        Wc.ws.server = new WiFiServer(82);
        Wc.ws.server->begin();
    }
    Wc.rtp_udp.begin(5004);
  }
  
  // Special Setup for WebRTC (Session 3)
  if (new_type == SESSION_WEBRTC) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: WebRTC session selected - audio task will launch after DTLS handshake"));
    // Start WS Server for WebRTC signaling
    if (!Wc.ws.server) {
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: Starting WS Server on 82"));
        Wc.ws.server = new WiFiServer(82);
        Wc.ws.server->begin();
    }
  }

  // Auto-Start (unless NONE)
  if (new_type != SESSION_NONE) {
    if (WcSetup(false)) {
      WcStart();
    }
  }

  Response_P(PSTR("{\"WcSession\":{\"Type\":%d,\"Name\":\"%s\"}}"), 
             Wc.core.session_type, session_names[Wc.core.session_type]);
}


// Web UI Strings
const char HTTP_WC_MODE[] PROGMEM = "{s}Camera Mode{m}%s{e}";
const char HTTP_WC_RES[]  PROGMEM = "{s}Resolution{m}%dx%d{e}";
const char HTTP_WC_FPS[]  PROGMEM = "{s}Frame Rate{m}%d fps{e}";

void WcShowInfo(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"Cam\":{\"Sensor\":\"%.8s\",\"State\":%d,\"Res\":\"%dx%d\",\"FPS\":%d,\"Motion\":%d}"), Wc.core.config.name, Wc.core.state, Wc.core.config.width, Wc.core.config.height, WcStats.last_fps, Wc.h264.motion_val);
    return;
  }

  const char* mode_str = "Standby";
  if (Wc.core.state == CAM_STREAMING) {
    switch (Wc.core.session_type) {
      case SESSION_MJPEG_HTTP: mode_str = "MJPEG Server"; break;
      case SESSION_RTSP_AND_WS: mode_str = "H264 Stream"; break;
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


#endif  // USE_CSI_WEBCAM
#endif  // ESP32P4
