/*
  xdrv_81_esp32_webcam.ino - ESP32 webcam support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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
#ifdef USE_WEBCAM
// defining USE_WEBCAM_V2 will use xdrv_81_esp32_webcam_task.ino instead.
#ifndef USE_WEBCAM_V2

/*********************************************************************************************\
 * ESP32 webcam based on example in Arduino-ESP32 library
 *
 * Template as used on ESP32-CAM WiFi + bluetooth Camera Module Development Board ESP32 With Camera Module OV2640 Geekcreit for Arduino
 * {"NAME":"AITHINKER CAM","GPIO":[4992,1,672,1,416,5088,1,1,1,6720,736,704,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,1,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":2}
 *
 * Supported commands:
 * WcInterrupt     = Control streaming, 0 = stop, 1 = start
 * WcResolution = Set resolution
 0 = FRAMESIZE_96X96,    // 96x96
 1 = FRAMESIZE_QQVGA,    // 160x120
 2 = FRAMESIZE_QCIF,     // 176x144
 3 = FRAMESIZE_HQVGA,    // 240x176
 4 = FRAMESIZE_240X240,  // 240x240
 5 = FRAMESIZE_QVGA,     // 320x240
 6 = FRAMESIZE_CIF,      // 400x296
 7 = FRAMESIZE_HVGA,     // 480x320
 8 = FRAMESIZE_VGA,      // 640x480
 9 = FRAMESIZE_SVGA,     // 800x600
 10 = FRAMESIZE_XGA,      // 1024x768
 11 = FRAMESIZE_HD,       // 1280x720
 12 = FRAMESIZE_SXGA,     // 1280x1024
 13 = FRAMESIZE_UXGA,     // 1600x1200
 // 3MP Sensors above this no yet supported with this driver
 14 = FRAMESIZE_FHD,      // 1920x1080
 15 = FRAMESIZE_P_HD,     //  720x1280
 16 = FRAMESIZE_P_3MP,    //  864x1536
 17 = FRAMESIZE_QXGA,     // 2048x1536
 // 5MP Sensors
 18 = FRAMESIZE_QHD,      // 2560x1440
 19 = FRAMESIZE_WQXGA,    // 2560x1600
 20 = FRAMESIZE_P_FHD,    // 1080x1920
 21 = FRAMESIZE_QSXGA,    // 2560x1920
 22 = FRAMESIZE_INVALID

 * WcMirror     = Mirror picture, 0 = no, 1 = yes
 * WcFlip       = Flip picture, 0 = no, 1 = yes
 * WcSaturation = Set picture Saturation -2 ... +2
 * WcBrightness = Set picture Brightness -2 ... +2
 * WcContrast   = Set picture Contrast -2 ... +2
 * WcSpecialEffekt = Set Special Picture Effect: 0 = off, 1 = , 2 = , 3 = , 4 = , 5 = , 6 =
 * WcAWB          = Auto White Balance, 0 = no, 1 = yes
 * WcWBMode       = White Balance Mode, 0 = auto, 1 =
 * WcAWBGain      = Auto White Balance Gain, 0 = no, 1 = yes
 * WcAEC          = Auto exposure control (Sensor), 0 = no, 1 = yes
 * WcAECDSP       = Auto exposure control (DSP), 0 = no, 1 = yes
 * WcAECValue     = Auto exposure control value, 0 ... 1024
 * WcAECLevel     = Auto exposure control level, -2 ... +2
 * WcAGC          = Auto gain control, 0 = no, 1 = yes
 * WcAGCGain      = Auto gain control gain, 0 .. 30
 * WcGainCeiling  = Gain ceiling, 0 .. 6 (0 = x2, 1 = x4, 2 = x8, 3 = x16, 4 = x32, 5 = x64, 6 = x128)
 * WcGammaCorrect = Auto Gamma Correct, 0 = no, 1 = yes
 * WcLensCorrect  = Auto Lens Correct, 0 = no, 1 = yes
 * WcWPC          = White Pixel Correct, 0 = no, 1 = yes
 * WcDCW          = Downscale, 0 = no, 1 = yes
 * WcBPC          = Black Pixel Correct, 0 = no, 1 = yes
 * WcColorbar     = Show Colorbar, 0 = no, 1 = yes
 * WcFeature      = Set extended Feature, 0 = off, 1 = reduce FPS, 2 = Nightmode
 * WcStats        = Show Statistics
 * WcInit         = Init Camera Interface
 * WcRtsp         = Control RTSP Server, 0=disable, 1=enable (forces restart) (if defined ENABLE_RTSPSERVER)
 *
 * WcFeature Explanation
 * See https://hobbylad.files.wordpress.com/2020/02/ov2640-camera-module-software-application-notes.pdf
 * for more Information
 * 0: Normal Operation
 * 1: Reduced FPS: Set XCLK Divisor to 2. Reduces Framerate and also increases exposure time. This causes
 *    better low light performance. See above document, page 6
 * 2: Night Mode: Further increase exposure time and lower the Framerate depending on available light.
 *    See above Document, Page 8
 *
 * Only boards with PSRAM should be used.
 * To speed up cam processing cpu frequency should be better set to 240Mhz
 *
 * remarks for AI-THINKER
 * GPIO0 zero must be disconnected from any wire after programming because this pin drives the cam clock and does
 * not tolerate any capictive load
 * the AITHINKER module does not have CAM_RESET - so if you get the camera into a bad state, power off restart is the only way out.
 * flash led = gpio 4
 * red led = gpio 33
 * optional rtsp url: rtsp://xxx.xxx.xxx.xxx:8554/mjpeg/1
 * 
 * SH 2023-05-14 - added mutex for many webcam functions - this is to prevent multi-threaded access to the camera functions, which 
 * can case error 0x105 upon re-init.
 * Errors 0x103 and 0xffffffff could indicate CAM_PWDN incorrect.
 * 
 * I2C use: if USE_I2C is enabled, you can set GPIO26 to I2c_SDA/2 and GPIO27 to I2C_SCL/2, and then use the shared I2C bus 2.
 * Then you can use cmd i2cscan2 to check for camera presence.
 */

/*********************************************************************************************/

#define XDRV_81           81

#include "cam_hal.h"
#include "esp_camera.h"
#include "sensor.h"
#include "fb_gfx.h"
#include "camera_pins.h"

SemaphoreHandle_t WebcamMutex = nullptr;

#ifndef USE_WEBCAM_SETUP_ONLY
bool HttpCheckPriviledgedAccess(bool);
extern ESP8266WebServer *Webserver;

// use mutex like:
// TasAutoMutex localmutex(&WebcamMutex, "somename");
// in any function.  Will wait for mutex to be clear, and auto-release when the function exits.

#define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"

#ifndef MAX_PICSTORE
  #define MAX_PICSTORE 4
#endif
struct PICSTORE {
  uint8_t *buff;
  uint32_t len;
};

#ifdef ENABLE_RTSPSERVER
#include <OV2640.h>
#include <SimStreamer.h>
#include <OV2640Streamer.h>
#include <CRtspSession.h>
#ifndef RTSP_FRAME_TIME
#define RTSP_FRAME_TIME 100
#endif // RTSP_FRAME_TIME
#endif // ENABLE_RTSPSERVER

#endif //USE_WEBCAM_SETUP_ONLY

struct {
  uint8_t  up = 0;
  uint16_t width;
  uint16_t height;
  uint8_t  stream_active;
 #ifndef USE_WEBCAM_SETUP_ONLY
  WiFiClient client;
  ESP8266WebServer *CamServer;
  struct PICSTORE picstore[MAX_PICSTORE];
#ifdef ENABLE_RTSPSERVER
  WiFiServer *rtspp;
  CStreamer *rtsp_streamer;
  CRtspSession *rtsp_session;
  WiFiClient rtsp_client;
  uint8_t rtsp_start;
#endif // ENABLE_RTSPSERVER
  OV2640 cam;
  uint32_t rtsp_lastframe_time;
#endif // USE_WEBCAM_SETUP_ONLY
} Wc;

struct {
  uint32_t camcnt = 0;
  uint32_t camfps = 0;
  uint32_t camfail = 0;
  uint32_t jpegfail = 0;
  uint32_t clientfail = 0;
  char name[7] = "Webcam";
} WcStats;

/*********************************************************************************************/

void WcInterrupt(uint32_t state) {
  TasAutoMutex localmutex(&WebcamMutex, "WcInterrupt");
  // Stop camera ISR if active to fix TG1WDT_SYS_RESET
  if (!Wc.up) { return; }

  if (state) {
    // Re-enable interrupts
    cam_start();
  } else {
    // Stop interrupts
    cam_stop();
  }
}

bool WcPinUsed(void) {
  bool pin_used = true;
  for (uint32_t i = 0; i < MAX_WEBCAM_DATA; i++) {
    if (!PinUsed(GPIO_WEBCAM_DATA, i)) {
      pin_used = false;
    }
//    if (i < MAX_WEBCAM_HSD) {
//      if (!PinUsed(GPIO_WEBCAM_HSD, i)) {
//        pin_used = false;
//      }
//    }
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: i2c_enabled_2: %d"), TasmotaGlobal.i2c_enabled_2);

  if (!PinUsed(GPIO_WEBCAM_XCLK) || !PinUsed(GPIO_WEBCAM_PCLK) ||
      !PinUsed(GPIO_WEBCAM_VSYNC) || !PinUsed(GPIO_WEBCAM_HREF) ||
      ((!PinUsed(GPIO_WEBCAM_SIOD) || !PinUsed(GPIO_WEBCAM_SIOC)) && !TasmotaGlobal.i2c_enabled_2)    // preferred option is to reuse and share I2Cbus 2
      ) {
        pin_used = false;
  }
  return pin_used;
}


void WcFeature(int32_t value) {
  TasAutoMutex localmutex(&WebcamMutex, "WcFeature");
  sensor_t * wc_s = esp_camera_sensor_get();
  if (!wc_s) { return; }

  if (value != 1) {
      // CLKRC: Set Clock Divider to 0 = fullspeed
      wc_s->set_reg(wc_s, 0x111, 0x3f, 0x00);
      vTaskDelay(200 / portTICK_PERIOD_MS);
  }
  if (value != 2) {
      // Stop Nightmode
      wc_s->set_reg(wc_s, 0x103, 0xff, 0x0a);   // COM1: Reset dummy frames
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x43);   // Reserved Reg
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x4b);   // Reserved Reg
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x43);   // Reserved Reg
  }

  switch(value) {
    case 1:
      // Reduce FPS
      // CLKRC: Set Clock Divider to 2
      wc_s->set_reg(wc_s, 0x111, 0x3f, 0x02);
      break;
    case 2:
      // Start Nightmode
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x4b);   // Reserved Reg
      wc_s->set_reg(wc_s, 0x103, 0xff, 0xcf);   // COM1: Allow 7 dummy frames
      break;
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Feature: %d"), value);
}


void WcApplySettings() {
  TasAutoMutex localmutex(&WebcamMutex, "WcApplySettings");
  sensor_t * wc_s = esp_camera_sensor_get();
  if (!wc_s) { return; }

  wc_s->set_vflip(wc_s, Settings->webcam_config.flip);
  wc_s->set_hmirror(wc_s, Settings->webcam_config.mirror);

  wc_s->set_brightness(wc_s, Settings->webcam_config.brightness - 2);
  wc_s->set_saturation(wc_s, Settings->webcam_config.saturation - 2);
  wc_s->set_contrast(wc_s, Settings->webcam_config.contrast - 2);

  wc_s->set_special_effect(wc_s, Settings->webcam_config2.special_effect);

  wc_s->set_whitebal(wc_s, Settings->webcam_config.awb);
  wc_s->set_wb_mode(wc_s, Settings->webcam_config2.wb_mode);
  wc_s->set_awb_gain(wc_s, Settings->webcam_config.awb_gain);

  wc_s->set_exposure_ctrl(wc_s, Settings->webcam_config.aec);
  wc_s->set_aec_value(wc_s, Settings->webcam_config2.aec_value - 2);
  wc_s->set_ae_level(wc_s, Settings->webcam_config2.ae_level);
  wc_s->set_aec2(wc_s, Settings->webcam_config.aec2);

  wc_s->set_gain_ctrl(wc_s, Settings->webcam_config.agc);
  wc_s->set_agc_gain(wc_s, Settings->webcam_config2.agc_gain);
  wc_s->set_gainceiling(wc_s, (gainceiling_t)Settings->webcam_config2.gainceiling);

  wc_s->set_raw_gma(wc_s, Settings->webcam_config.raw_gma);
  wc_s->set_lenc(wc_s, Settings->webcam_config.lenc);
  wc_s->set_wpc(wc_s, Settings->webcam_config.wpc);
  wc_s->set_dcw(wc_s, Settings->webcam_config.dcw);
  wc_s->set_bpc(wc_s, Settings->webcam_config.bpc);

  WcFeature(Settings->webcam_config.feature);

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Settings updated"));
}

void WcSetDefaults(uint32_t upgrade) {
  if (!upgrade) {
    Settings->webcam_config.flip = 0;
    Settings->webcam_config.mirror = 0;

    Settings->webcam_config.saturation = 2; // = 0
    Settings->webcam_config.brightness = 2; // = 0
    Settings->webcam_config.contrast = 2;   // = 0
  }

  Settings->webcam_config2.special_effect = 0;
  Settings->webcam_config.colorbar = 0;

  Settings->webcam_config.awb = 1;        // white balance
  Settings->webcam_config2.wb_mode = 0;   // white balance mode
  Settings->webcam_config.awb_gain = 1;   // white blance gain

  Settings->webcam_config.aec = 1;          // autoexposure (sensor)
  Settings->webcam_config.aec2 = 1;         // autoexposure (dsp)
  Settings->webcam_config2.ae_level = 2;    // autoexposure level (-2 - +2, default 0)
  Settings->webcam_config2.aec_value = 204; // manual exposure value

  Settings->webcam_config.agc = 1;          // auto gain control
  Settings->webcam_config2.agc_gain = 5;    // manual gain control
  Settings->webcam_config2.gainceiling = 0; // auto gain ceiling

  Settings->webcam_config.raw_gma = 1;      // gamma correct
  Settings->webcam_config.lenc = 1;         // lens correction
  Settings->webcam_config.wpc = 1;          // white pixel correct
  Settings->webcam_config.dcw = 1;          // downsize en
  Settings->webcam_config.bpc = 0;          // black pixel correct?

  Settings->webcam_config.feature = 0;

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Defaults set"));

  if (Wc.up) { WcApplySettings(); }
}

uint32_t WcSetup(int32_t fsiz) {
  TasAutoMutex localmutex(&WebcamMutex, "WcSetup");

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: WcSetup"));
  if (fsiz >= FRAMESIZE_FHD) { fsiz = FRAMESIZE_FHD - 1; }

  int stream_active = Wc.stream_active;
  Wc.stream_active = 0;

  if (fsiz < 0) {
    if (Wc.up){    
      esp_camera_deinit();
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Deinit fsiz %d"), fsiz);
      Wc.up = 0;
    }
    return 0;
  }

  if (Wc.up) {
    esp_camera_deinit();
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Deinit"));
    //return Wc.up;
  }
  Wc.up = 0;

//esp_log_level_set("*", ESP_LOG_VERBOSE);

  camera_config_t config;

  memset(&config, 0, sizeof(config));

  if (WcPinUsed()) {
    config.pin_d0 = Pin(GPIO_WEBCAM_DATA);        // Y2_GPIO_NUM;
    config.pin_d1 = Pin(GPIO_WEBCAM_DATA, 1);     // Y3_GPIO_NUM;
    config.pin_d2 = Pin(GPIO_WEBCAM_DATA, 2);     // Y4_GPIO_NUM;
    config.pin_d3 = Pin(GPIO_WEBCAM_DATA, 3);     // Y5_GPIO_NUM;
    config.pin_d4 = Pin(GPIO_WEBCAM_DATA, 4);     // Y6_GPIO_NUM;
    config.pin_d5 = Pin(GPIO_WEBCAM_DATA, 5);     // Y7_GPIO_NUM;
    config.pin_d6 = Pin(GPIO_WEBCAM_DATA, 6);     // Y8_GPIO_NUM;
    config.pin_d7 = Pin(GPIO_WEBCAM_DATA, 7);     // Y9_GPIO_NUM;
    config.pin_xclk = Pin(GPIO_WEBCAM_XCLK);      // XCLK_GPIO_NUM;
    config.pin_pclk = Pin(GPIO_WEBCAM_PCLK);      // PCLK_GPIO_NUM;
    config.pin_vsync = Pin(GPIO_WEBCAM_VSYNC);    // VSYNC_GPIO_NUM;
    config.pin_href = Pin(GPIO_WEBCAM_HREF);      // HREF_GPIO_NUM;
    config.pin_sccb_sda = Pin(GPIO_WEBCAM_SIOD);  // SIOD_GPIO_NUM; - unset to use shared I2C bus 2
    config.pin_sccb_scl = Pin(GPIO_WEBCAM_SIOC);  // SIOC_GPIO_NUM;
    if(TasmotaGlobal.i2c_enabled_2){              // configure SIOD and SIOC as SDA,2 and SCL,2
      config.sccb_i2c_port = 1;                   // reuse initialized bus 2, can be shared now
      if(config.pin_sccb_sda < 0){                // GPIO_WEBCAM_SIOD must not be set to really make it happen
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: Use I2C bus2"));
      }
    }
    config.pin_pwdn = Pin(GPIO_WEBCAM_PWDN);       // PWDN_GPIO_NUM;
    config.pin_reset = Pin(GPIO_WEBCAM_RESET);    // RESET_GPIO_NUM;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Template pin config"));
  } else if (Y2_GPIO_NUM != -1) {
    // Modell is set in camera_pins.h
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Compile flag pin config"));
  } else {
    // no valid config found -> abort
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: No pin config"));
    return 0;
}

  int32_t ledc_channel = analogAttach(config.pin_xclk);
  if (ledc_channel < 0) {
    AddLog(LOG_LEVEL_ERROR, "CAM: cannot allocated ledc channel, remove a PWM GPIO");
  }
  config.ledc_channel = (ledc_channel_t) ledc_channel;
  AddLog(LOG_LEVEL_DEBUG_MORE, "CAM: XCLK on GPIO %i using ledc channel %i", config.pin_xclk, config.ledc_channel);
  config.ledc_timer = LEDC_TIMER_0;
//  config.xclk_freq_hz = 20000000;
  config.xclk_freq_hz = Settings->webcam_clk * 1000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //esp_log_level_set("*", ESP_LOG_INFO);

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.

  bool psram = UsePSRAM();
  if (psram) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: PSRAM found"));
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_DRAM;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: PSRAM not found"));
  }

  esp_err_t err;
  // cannot hurt to retry...
  for (int i = 0; i < 3; i++){
    err = esp_camera_init(&config);

    if (err != ESP_OK) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: InitErr 0x%x try %d"), err, (i+1));
      esp_camera_deinit();
    } else {
      if (i){
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: InitOK try %d"), (i+1));
      }
      break;
    }
  }

  if (err != ESP_OK) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: InitErr 0x%x"), err);
    return 0;
  }

//  AddLog(LOG_LEVEL_INFO, PSTR("CAM: heap check 2: %d"),ESP_getFreeHeap());

  sensor_t * wc_s = esp_camera_sensor_get();
  // drop down frame size for higher initial frame rate
  wc_s->set_framesize(wc_s, (framesize_t)fsiz);

  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Init failed to get the frame on time"));
    return 0;
  }
  Wc.width = wc_fb->width;
  Wc.height = wc_fb->height;
  esp_camera_fb_return(wc_fb);

  WcApplySettings();

  camera_sensor_info_t *info = esp_camera_sensor_get_info(&wc_s->id);

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: %s Initialized"), info->name);


  Wc.up = 1;
  if (psram) { Wc.up = 2; }

  // restore stream_active if we setup ok.
  Wc.stream_active = stream_active;

  return Wc.up;
}

/*********************************************************************************************/
#ifndef USE_WEBCAM_SETUP_ONLY
int32_t WcSetOptions(uint32_t sel, int32_t value) {
  int32_t res = 0;
  TasAutoMutex localmutex(&WebcamMutex, "WcSetOptions");

  sensor_t *s = esp_camera_sensor_get();
  if (!s) { return -99; }

  switch (sel) {
    case 0:
      if (value >= 0) { s->set_framesize(s, (framesize_t)value); }
      res = s->status.framesize;

      // WcFeature is lost on resolution change
      WcApplySettings();
      break;
    case 1:
      if (value >= 0) { s->set_special_effect(s, value); }
      res = s->status.special_effect;
      break;
    case 2:
      if (value >= 0) { s->set_vflip(s, value); }
      res = s->status.vflip;
      break;
    case 3:
      if (value >= 0) { s->set_hmirror(s, value); }
      res = s->status.hmirror;
      break;
    case 4:
      if (value >= -4) { s->set_contrast(s, value); }
      res = s->status.contrast;
      break;
    case 5:
      if (value >= -4) { s->set_brightness(s, value); }
      res = s->status.brightness;
      break;
    case 6:
      if (value >= -4) { s->set_saturation(s,value); }
      res = s->status.saturation;
      break;
    case 7:
      if (value >= 0) { s->set_whitebal(s, value); }
      res = s->status.awb;
      break;
    case 8:
      if (value >= 0) { s->set_wb_mode(s, value); }
      res = s->status.wb_mode;
      break;
    case 9:
      if (value >= 0) { s->set_awb_gain(s, value); }
      res = s->status.awb_gain;
      break;
    case 10:
      if (value >= 0) { s->set_exposure_ctrl(s, value); }
      res = s->status.aec;
      break;
    case 11:
      if (value >= 0) { s->set_aec_value(s, value); }
      res = s->status.aec_value;
      break;
    case 12:
      if (value >= 0) { s->set_ae_level(s, value); }
      res = s->status.ae_level;
      break;
    case 13:
      if (value >= 0) { s->set_aec2(s, value); }
      res = s->status.aec2;
      break;
    case 14:
      if (value >= 0) { s->set_gain_ctrl(s, value); }
      res = s->status.agc;
      break;
    case 15:
      if (value >= 0) { s->set_agc_gain(s, value); }
      res = s->status.agc_gain;
      break;
    case 16:
      if (value >= 0) { s->set_gainceiling(s, (gainceiling_t)value); }
      res = s->status.gainceiling;
      break;
    case 17:
      if (value >= 0) { s->set_raw_gma(s, value); }
      res = s->status.raw_gma;
      break;
    case 18:
      if (value >= 0) { s->set_lenc(s, value); }
      res = s->status.lenc;
      break;
    case 19:
      if (value >= 0) { s->set_wpc(s, value); }
      res = s->status.wpc;
      break;
    case 20:
      if (value >= 0) { s->set_dcw(s, value); }
      res = s->status.dcw;
      break;
    case 21:
      // blackpixelcontrol
      if (value >= 0) { s->set_bpc(s, value); }
      res = s->status.bpc;
      break;
    case 22:
      if (value >= 0) { s->set_colorbar(s, value); }
      res = s->status.colorbar;
      break;
    case 23:
      if (value >= 0) { WcFeature(value); }
      break;
  }

  return res;
}

uint32_t WcGetWidth(void) {
  TasAutoMutex localmutex(&WebcamMutex, "WcGetWidth");
  
  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) { return 0; }
  Wc.width = wc_fb->width;
  esp_camera_fb_return(wc_fb);
  return Wc.width;
}

uint32_t WcGetHeight(void) {
  TasAutoMutex localmutex(&WebcamMutex, "WcGetWidth");
  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) { return 0; }
  Wc.height = wc_fb->height;
  esp_camera_fb_return(wc_fb);
  return Wc.height;
}

/*********************************************************************************************/

struct WC_Motion {
uint16_t motion_detect;
uint32_t motion_ltime;
uint32_t motion_trigger;
uint32_t motion_brightness;
uint8_t *last_motion_buffer;
} wc_motion;


uint32_t WcSetMotionDetect(int32_t value) {
  if (value >= 0) { wc_motion.motion_detect = value; }
  if (-1 == value) {
    return wc_motion.motion_trigger;
  } else  {
    return wc_motion.motion_brightness;
  }
}

// optional motion detector
void WcDetectMotion(void) {
  camera_fb_t *wc_fb;
  uint8_t *out_buf = 0;
  TasAutoMutex localmutex(&WebcamMutex, "WcDetectMotion");

  if ((millis()-wc_motion.motion_ltime) > wc_motion.motion_detect) {
    wc_motion.motion_ltime = millis();
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) { return; }

    if (!wc_motion.last_motion_buffer) {
      wc_motion.last_motion_buffer = (uint8_t *)heap_caps_malloc((wc_fb->width*wc_fb->height) + 4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    }
    if (wc_motion.last_motion_buffer) {
      if (PIXFORMAT_JPEG == wc_fb->format) {
        out_buf = (uint8_t *)heap_caps_malloc((wc_fb->width*wc_fb->height*3)+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (out_buf) {
          fmt2rgb888(wc_fb->buf, wc_fb->len, wc_fb->format, out_buf);
          uint32_t x, y;
          uint8_t *pxi = out_buf;
          uint8_t *pxr = wc_motion.last_motion_buffer;
          // convert to bw
          uint64_t accu = 0;
          uint64_t bright = 0;
          for (y = 0; y < wc_fb->height; y++) {
            for (x = 0; x < wc_fb->width; x++) {
              int32_t gray = (pxi[0] + pxi[1] + pxi[2]) / 3;
              int32_t lgray = pxr[0];
              pxr[0] = gray;
              pxi += 3;
              pxr++;
              accu += abs(gray - lgray);
              bright += gray;
            }
          }
          wc_motion.motion_trigger = accu / ((wc_fb->height * wc_fb->width) / 100);
          wc_motion.motion_brightness = bright / ((wc_fb->height * wc_fb->width) / 100);
          free(out_buf);
        }
      }
    }
    esp_camera_fb_return(wc_fb);
  }
}

#ifdef COPYFRAME
struct PICSTORE tmp_picstore;
#endif

uint32_t WcGetPicstore(int32_t num, uint8_t **buff) {
  if (num<0) { return MAX_PICSTORE; }
  *buff = Wc.picstore[num].buff;
  return Wc.picstore[num].len;
}

uint32_t WcGetFrame(int32_t bnum) {
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  camera_fb_t *wc_fb = 0;
  bool jpeg_converted = false;
  TasAutoMutex localmutex(&WebcamMutex, "WcGetFrame");

  if (bnum < 0) {
    if (bnum < -MAX_PICSTORE) { bnum=-1; }
    bnum = -bnum;
    bnum--;
    if (Wc.picstore[bnum].buff) { free(Wc.picstore[bnum].buff); }
    Wc.picstore[bnum].len = 0;
    return 0;
  }

#ifdef COPYFRAME
  if (bnum & 0x10) {
    bnum &= 0xf;
    _jpg_buf = tmp_picstore.buff;
    _jpg_buf_len = tmp_picstore.len;
    if (!_jpg_buf_len) { return 0; }
    goto pcopy;
  }
#endif

  wc_fb = esp_camera_fb_get();
  if (!wc_fb) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Can't get frame"));
    return 0;
  }
  if (!bnum) {
    Wc.width = wc_fb->width;
    Wc.height = wc_fb->height;
    esp_camera_fb_return(wc_fb);
    return 0;
  }

  if (wc_fb->format != PIXFORMAT_JPEG) {
    jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
    if (!jpeg_converted){
      //Serial.println("JPEG compression failed");
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }
  } else {
    _jpg_buf_len = wc_fb->len;
    _jpg_buf = wc_fb->buf;
  }

pcopy:
  if ((bnum < 1) || (bnum > MAX_PICSTORE)) { bnum = 1; }
  bnum--;
  if (Wc.picstore[bnum].buff) { free(Wc.picstore[bnum].buff); }
  Wc.picstore[bnum].buff = (uint8_t *)heap_caps_malloc(_jpg_buf_len+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (Wc.picstore[bnum].buff) {
    memcpy(Wc.picstore[bnum].buff, _jpg_buf, _jpg_buf_len);
    Wc.picstore[bnum].len = _jpg_buf_len;
  } else {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Can't allocate picstore"));
    Wc.picstore[bnum].len = 0;
  }
  if (wc_fb) { esp_camera_fb_return(wc_fb); }
  if (jpeg_converted) { free(_jpg_buf); }
  if (!Wc.picstore[bnum].buff) { return 0; }

  return  _jpg_buf_len;
}

//////////////// Handle authentication /////////////////

bool WebcamAuthenticate(void)
{
  if (strlen(SettingsText(SET_WEBPWD)) && (HTTP_MANAGER_RESET_ONLY != Web.state)) {
    return Wc.CamServer->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD));
  } else {
    return true;
  }
}

bool WebcamCheckPriviledgedAccess(bool autorequestauth = true)
{

  if(Settings->webcam_config2.auth == 0){
    return true;
  }

  if (autorequestauth && !WebcamAuthenticate()) {
    Wc.CamServer->requestAuthentication();
    return false;
  }
  return true;
}

///////////////////////////////////////////////////

void HandleImage(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  uint32_t bnum = Webserver->arg(F("p")).toInt();
  if ((bnum < 0) || (bnum > MAX_PICSTORE)) { bnum= 1; }
  WiFiClient client = Webserver->client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.jpg\r\n";
  response += "Content-type: image/jpeg\r\n\r\n";
  Webserver->sendContent(response);

  TasAutoMutex localmutex(&WebcamMutex, "HandleImage");

  if (!bnum) {
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    camera_fb_t *wc_fb = 0;
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) { return; }
    if (Wc.stream_active < 2) {
      // fetch some more frames
      esp_camera_fb_return(wc_fb);
      wc_fb = esp_camera_fb_get();
      esp_camera_fb_return(wc_fb);
      wc_fb = esp_camera_fb_get();
    }
    if (wc_fb->format != PIXFORMAT_JPEG) {
      bool jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
      if (!jpeg_converted) {
        _jpg_buf_len = wc_fb->len;
        _jpg_buf = wc_fb->buf;
      }
    } else {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }
    if (_jpg_buf_len) {
      client.write((char *)_jpg_buf, _jpg_buf_len);
    }
    if (wc_fb) { esp_camera_fb_return(wc_fb); }
  } else {
    bnum--;
    if (!Wc.picstore[bnum].len) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: No image #: %d"), bnum);
      return;
    }
    client.write((char *)Wc.picstore[bnum].buff, Wc.picstore[bnum].len);
  }
  client.stop();

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CAM: Sending image #: %d"), bnum+1);
}

void HandleImageBasic(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_HTTP "Capture image"));

  if (Settings->webcam_config.stream) {
    if (!Wc.CamServer) {
      WcInterruptControl();
    }
  }

  TasAutoMutex localmutex(&WebcamMutex, "HandleImage");
  camera_fb_t *wc_fb;
  wc_fb = esp_camera_fb_get();  // Acquire frame
  if (!wc_fb) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Frame buffer could not be acquired"));
    return;
  }

  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  if (wc_fb->format != PIXFORMAT_JPEG) {
    bool jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
    if (!jpeg_converted) {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }
  } else {
    _jpg_buf_len = wc_fb->len;
    _jpg_buf = wc_fb->buf;
  }

  if (_jpg_buf_len) {
    Webserver->client().flush();
    WSHeaderSend();
    Webserver->sendHeader(F("Content-disposition"), F("inline; filename=snapshot.jpg"));
    Webserver->send_P(200, "image/jpeg", (char *)_jpg_buf, _jpg_buf_len);
    Webserver->client().stop();
  }

  esp_camera_fb_return(wc_fb);  // Free frame buffer

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CAM: Image sent"));
}

void HandleWebcamMjpeg(void) {
  if(!WebcamCheckPriviledgedAccess()){
    Wc.CamServer->send(403,"","");
    return;
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Handle camserver"));
//  if (!Wc.stream_active) {
// always restart stream
    Wc.stream_active = 1;
    Wc.client = Wc.CamServer->client();
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Create client"));
//  }
}

void HandleWebcamMjpegTask(void) {
  camera_fb_t *wc_fb;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;

  //WiFiClient client = CamServer->client();
  uint32_t tlen;
  bool jpeg_converted = false;

  if (!Wc.client.connected()) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client fail"));
    Wc.stream_active = 0;
    WcStats.clientfail++;
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

  TasAutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpegTask");

  if (2 == Wc.stream_active) {
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Frame fail"));
      Wc.stream_active = 0;
      WcStats.camfail++;
    }
    WcStats.camcnt++;
  }
  if (2 == Wc.stream_active) {
    if (wc_fb->format != PIXFORMAT_JPEG) {
      jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
      if (!jpeg_converted){
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: JPEG compression failed"));
        _jpg_buf_len = wc_fb->len;
        _jpg_buf = wc_fb->buf;
        WcStats.jpegfail++;
      }
    } else {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }

    Wc.client.print("--" BOUNDARY "\r\n");
    Wc.client.printf("Content-Type: image/jpeg\r\n"
      "Content-Length: %d\r\n"
      "\r\n", static_cast<int>(_jpg_buf_len));
    tlen = Wc.client.write(_jpg_buf, _jpg_buf_len);
    /*
    if (tlen!=_jpg_buf_len) {
      esp_camera_fb_return(wc_fb);
      Wc.stream_active=0;
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Send fail"));
    }*/
//    Wc.client.print("\r\n--" BOUNDARY "\r\n");
    Wc.client.print("\r\n");

#ifdef COPYFRAME
    if (tmp_picstore.buff) { free(tmp_picstore.buff); }
    tmp_picstore.buff = (uint8_t *)heap_caps_malloc(_jpg_buf_len+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (tmp_picstore.buff) {
      memcpy(tmp_picstore.buff, _jpg_buf, _jpg_buf_len);
      tmp_picstore.len = _jpg_buf_len;
    } else {
      tmp_picstore.len = 0;
    }
#endif

    if (jpeg_converted) { free(_jpg_buf); }
    esp_camera_fb_return(wc_fb);
    //AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: send frame"));
  }
  if (0 == Wc.stream_active) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stream exit"));
    Wc.client.flush();
    Wc.client.stop();
  }
}

void HandleWebcamRoot(void) {
  if(!WebcamCheckPriviledgedAccess()){
    Wc.CamServer->send(403,"","");
    return;
  }
  //CamServer->redirect("http://" + String(ip) + ":81/cam.mjpeg");
  Wc.CamServer->sendHeader("Location", "/cam.mjpeg");
  Wc.CamServer->send(302, "", "");
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Root called"));
}

/*********************************************************************************************/

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

void WcInterruptControl() {
  TasAutoMutex localmutex(&WebcamMutex, "WcInterruptControl");

  WcSetStreamserver(Settings->webcam_config.stream);
  if(Wc.up == 0) {
    WcSetup(Settings->webcam_config.resolution);
  }

}

/*********************************************************************************************/


void WcLoop(void) {
  // if (4 == Wc.stream_active) { return; }

  if (Wc.CamServer) {
    Wc.CamServer->handleClient();
    if (Wc.stream_active) { HandleWebcamMjpegTask(); }
  }
  if (wc_motion.motion_detect) { WcDetectMotion(); }

#ifdef ENABLE_RTSPSERVER
    if (Settings->webcam_config.rtsp && !TasmotaGlobal.global_state.wifi_down && Wc.up) {
      if (!Wc.rtsp_start) {
        Wc.rtspp = new WiFiServer(8554);
        Wc.rtspp->begin();
        Wc.rtsp_start = 1;
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTSP init"));
        Wc.rtsp_lastframe_time = millis();
      }

      // If we have an active client connection, just service that until gone
      if (Wc.rtsp_session) {
        Wc.rtsp_session->handleRequests(0); // we don't use a timeout here,
        // instead we send only if we have new enough frames

        uint32_t now = millis();
        if ((now-Wc.rtsp_lastframe_time) > RTSP_FRAME_TIME) {
            Wc.rtsp_session->broadcastCurrentFrame(now);
            Wc.rtsp_lastframe_time = now;
          //  AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTSP session frame"));
        }

        if (Wc.rtsp_session->m_stopped) {
            delete Wc.rtsp_session;
            delete Wc.rtsp_streamer;
            Wc.rtsp_session = NULL;
            Wc.rtsp_streamer = NULL;
            AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTSP stopped"));
        }
      }
      else {
        Wc.rtsp_client = Wc.rtspp->accept();
        if (Wc.rtsp_client) {
            Wc.rtsp_streamer = new OV2640Streamer(&Wc.rtsp_client, Wc.cam);        // our streamer for UDP/TCP based RTP transport
            Wc.rtsp_session = new CRtspSession(&Wc.rtsp_client, Wc.rtsp_streamer); // our threads RTSP session and state
            AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTSP stream created"));
        }
      }
    }
#endif // ENABLE_RTSPSERVER
}

void WcPicSetup(void) {
  WebServer_on(PSTR("/wc.jpg"), HandleImage);
  WebServer_on(PSTR("/wc.mjpeg"), HandleImage);
  WebServer_on(PSTR("/snapshot.jpg"), HandleImage);
}

void WcShowStream(void) {
  if (Settings->webcam_config.stream) {
//    if (!Wc.CamServer || !Wc.up) {
    if (!Wc.CamServer) {
      WcInterruptControl();
    }
    if (Wc.CamServer && Wc.up!=0) {
      // Give the webcam webserver some time to prepare the stream - catch error in JS
      WSContentSend_P(PSTR("<p></p><center><img onerror='setTimeout(()=>{this.src=this.src;},1000)' src='http://%_I:81/stream' alt='Webcam stream' style='width:99%%;'></center><p></p>"),(uint32_t)WiFi.localIP());
    }
  }
}

#endif // USE_WEBCAM_SETUP_ONLY

void WcInit(void) {
  if (!Settings->webcam_config.data) {
    Settings->webcam_config.stream = 1;
    Settings->webcam_config.resolution = FRAMESIZE_QVGA;
    WcSetDefaults(0);
  }
  // previous webcam driver had only a small subset of possible config vars
  // in this case we have to only set the new variables to default values
  if(!Settings->webcam_config2.upgraded) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Upgrade settings"));
    WcSetDefaults(1);
    Settings->webcam_config2.upgraded = 1;
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/
#ifndef USE_WEBCAM_SETUP_ONLY

#define D_PRFX_WEBCAM "WC"
#define D_CMND_WC_STREAM "Stream"
#define D_CMND_WC_RESOLUTION "Resolution"
#define D_CMND_WC_MIRROR "Mirror"
#define D_CMND_WC_FLIP "Flip"
#define D_CMND_WC_SATURATION "Saturation"
#define D_CMND_WC_BRIGHTNESS "Brightness"
#define D_CMND_WC_CONTRAST "Contrast"
#define D_CMND_WC_SPECIALEFFECT "SpecialEffect"

#define D_CMND_WC_AWB "AWB"
#define D_CMND_WC_WB_MODE "WBMode"
#define D_CMND_WC_AWB_GAIN "AWBGain"

#define D_CMND_WC_AEC "AEC"
#define D_CMND_WC_AEC_VALUE "AECValue"
#define D_CMND_WC_AE_LEVEL "AELevel"
#define D_CMND_WC_AEC2 "AECDSP"

#define D_CMND_WC_AGC "AGC"
#define D_CMND_WC_AGC_GAIN "AGCGain"
#define D_CMND_WC_GAINCEILING "GainCeiling"

#define D_CMND_WC_RAW_GMA "GammaCorrect"
#define D_CMND_WC_LENC "LensCorrect"

#define D_CMND_WC_WPC "WPC"
#define D_CMND_WC_DCW "DCW"
#define D_CMND_WC_BPC "BPC"

#define D_CMND_WC_COLORBAR "Colorbar"

#define D_CMND_WC_FEATURE "Feature"
#define D_CMND_WC_SETDEFAULTS "SetDefaults"
#define D_CMND_WC_STATS "Stats"

#define D_CMND_WC_INIT "Init"
#define D_CMND_RTSP "Rtsp"

#define D_CMND_WC_AUTH "Auth"
#define D_CMND_WC_CLK "Clock"

const char kWCCommands[] PROGMEM =  D_PRFX_WEBCAM "|"  // Prefix
  "|" D_CMND_WC_STREAM "|" D_CMND_WC_RESOLUTION "|" D_CMND_WC_MIRROR "|" D_CMND_WC_FLIP "|"
  D_CMND_WC_SATURATION "|" D_CMND_WC_BRIGHTNESS "|" D_CMND_WC_CONTRAST "|" D_CMND_WC_SPECIALEFFECT "|"
  D_CMND_WC_AWB "|" D_CMND_WC_WB_MODE "|" D_CMND_WC_AWB_GAIN "|" D_CMND_WC_AEC "|"
  D_CMND_WC_AEC_VALUE "|" D_CMND_WC_AE_LEVEL "|" D_CMND_WC_AEC2 "|" D_CMND_WC_AGC "|"
  D_CMND_WC_AGC_GAIN "|" D_CMND_WC_GAINCEILING "|" D_CMND_WC_RAW_GMA "|" D_CMND_WC_LENC "|"
  D_CMND_WC_WPC "|" D_CMND_WC_DCW "|" D_CMND_WC_BPC "|" D_CMND_WC_COLORBAR "|" D_CMND_WC_FEATURE "|"
  D_CMND_WC_SETDEFAULTS "|" D_CMND_WC_STATS "|" D_CMND_WC_INIT "|" D_CMND_WC_AUTH "|" D_CMND_WC_CLK
#ifdef ENABLE_RTSPSERVER
  "|" D_CMND_RTSP
#endif // ENABLE_RTSPSERVER
  ;

void (* const WCCommand[])(void) PROGMEM = {
  &CmndWebcam, &CmndWebcamStream, &CmndWebcamResolution, &CmndWebcamMirror, &CmndWebcamFlip,
  &CmndWebcamSaturation, &CmndWebcamBrightness, &CmndWebcamContrast, &CmndWebcamSpecialEffect,
  &CmndWebcamAWB, &CmndWebcamWBMode, &CmndWebcamAWBGain, &CmndWebcamAEC, &CmndWebcamAECValue,
  &CmndWebcamAELevel, &CmndWebcamAEC2, &CmndWebcamAGC, &CmndWebcamAGCGain, &CmndWebcamGainCeiling,
  &CmndWebcamGammaCorrect, &CmndWebcamLensCorrect, &CmndWebcamWPC, &CmndWebcamDCW, &CmndWebcamBPC,
  &CmndWebcamColorbar, &CmndWebcamFeature, &CmndWebcamSetDefaults,
  &CmndWebcamStats, &CmndWebcamInit, &CmndWebcamAuth, &CmndWebcamClock
#ifdef ENABLE_RTSPSERVER
  , &CmndWebRtsp
#endif // ENABLE_RTSPSERVER
  };

void CmndWebcam(void) {
  Response_P(PSTR("{\"" D_PRFX_WEBCAM "\":{\"" D_CMND_WC_STREAM "\":%d,\"" D_CMND_WC_RESOLUTION "\":%d,\"" D_CMND_WC_MIRROR "\":%d,\""
    D_CMND_WC_FLIP "\":%d,\""
    D_CMND_WC_SATURATION "\":%d,\"" D_CMND_WC_BRIGHTNESS "\":%d,\"" D_CMND_WC_CONTRAST "\":%d,\""
    D_CMND_WC_SPECIALEFFECT "\":%d,\"" D_CMND_WC_AWB  "\":%d,\"" D_CMND_WC_WB_MODE "\":%d,\""
    D_CMND_WC_AWB_GAIN "\":%d,\"" D_CMND_WC_AEC "\":%d,\"" D_CMND_WC_AEC_VALUE "\":%d,\""
    D_CMND_WC_AE_LEVEL "\":%d,\"" D_CMND_WC_AEC2 "\":%d,\"" D_CMND_WC_AGC "\":%d,\""
    D_CMND_WC_AGC_GAIN "\":%d,\"" D_CMND_WC_GAINCEILING "\":%d,\"" D_CMND_WC_RAW_GMA "\":%d,\""
    D_CMND_WC_LENC "\":%d,\"" D_CMND_WC_WPC "\":%d,\"" D_CMND_WC_DCW "\":%d,\"" D_CMND_WC_BPC "\":%d,\""
    D_CMND_WC_COLORBAR "\":%d,\"" D_CMND_WC_FEATURE "\":%d,\"" D_CMND_WC_AUTH "\":%d"
#ifdef ENABLE_RTSPSERVER
  ",\"" D_CMND_RTSP "\":%d"
#endif // ENABLE_RTSPSERVER
  "}}"),
    Settings->webcam_config.stream, Settings->webcam_config.resolution, Settings->webcam_config.mirror,
    Settings->webcam_config.flip,
    Settings->webcam_config.saturation -2, Settings->webcam_config.brightness -2, Settings->webcam_config.contrast -2,
    Settings->webcam_config2.special_effect, Settings->webcam_config.awb, Settings->webcam_config2.wb_mode,
    Settings->webcam_config.awb_gain, Settings->webcam_config.aec, Settings->webcam_config2.aec_value,
    Settings->webcam_config2.ae_level -2, Settings->webcam_config.aec2, Settings->webcam_config.agc,
    Settings->webcam_config2.agc_gain, Settings->webcam_config2.gainceiling, Settings->webcam_config.raw_gma,
    Settings->webcam_config.lenc, Settings->webcam_config.wpc, Settings->webcam_config.dcw, Settings->webcam_config.bpc,
    Settings->webcam_config.colorbar, Settings->webcam_config.feature, Settings->webcam_config2.auth
#ifdef ENABLE_RTSPSERVER
  , Settings->webcam_config.rtsp
#endif // ENABLE_RTSPSERVER
  );
}

void CmndWebcamStream(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.stream = XdrvMailbox.payload;
    if (!Settings->webcam_config.stream) { 
      WcInterruptControl();  // Stop stream
    } else {
      WcSetStreamserver(Settings->webcam_config.stream);  // Ensure server is running
    }
  }
  ResponseCmndStateText(Settings->webcam_config.stream);
}

void CmndWebcamResolution(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < FRAMESIZE_FHD)) {
    Settings->webcam_config.resolution = XdrvMailbox.payload;
    WcSetOptions(0, Settings->webcam_config.resolution);
  }
  ResponseCmndNumber(Settings->webcam_config.resolution);
}

void CmndWebcamMirror(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.mirror = XdrvMailbox.payload;
    WcSetOptions(3, Settings->webcam_config.mirror);
  }
  ResponseCmndStateText(Settings->webcam_config.mirror);
}

void CmndWebcamFlip(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.flip = XdrvMailbox.payload;
    WcSetOptions(2, Settings->webcam_config.flip);
  }
  ResponseCmndStateText(Settings->webcam_config.flip);
}

void CmndWebcamSaturation(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config.saturation = XdrvMailbox.payload +2;
    WcSetOptions(6, Settings->webcam_config.saturation -2);
  }
  ResponseCmndNumber(Settings->webcam_config.saturation -2);
}

void CmndWebcamBrightness(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config.brightness = XdrvMailbox.payload +2;
    WcSetOptions(5, Settings->webcam_config.brightness -2);
  }
  ResponseCmndNumber(Settings->webcam_config.brightness -2);
}

void CmndWebcamContrast(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config.contrast = XdrvMailbox.payload +2;
    WcSetOptions(4, Settings->webcam_config.contrast -2);
  }
  ResponseCmndNumber(Settings->webcam_config.contrast -2);
}

void CmndWebcamSpecialEffect(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
    Settings->webcam_config2.special_effect = XdrvMailbox.payload;
    WcSetOptions(1, Settings->webcam_config2.special_effect);
  }
  ResponseCmndNumber(Settings->webcam_config2.special_effect);
}

void CmndWebcamAWB(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.awb = XdrvMailbox.payload;
    WcSetOptions(7, Settings->webcam_config.awb);
  }
  ResponseCmndStateText(Settings->webcam_config.awb);
}

void CmndWebcamWBMode(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 4)) {
    Settings->webcam_config2.wb_mode = XdrvMailbox.payload;
    WcSetOptions(8, Settings->webcam_config2.wb_mode);
  }
  ResponseCmndNumber(Settings->webcam_config2.wb_mode);
}

void CmndWebcamAWBGain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.awb_gain = XdrvMailbox.payload;
    WcSetOptions(9, Settings->webcam_config.awb_gain);
  }
  ResponseCmndStateText(Settings->webcam_config.awb_gain);
}

void CmndWebcamAEC(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.aec = XdrvMailbox.payload;
    WcSetOptions(10, Settings->webcam_config.aec);
  }
  ResponseCmndStateText(Settings->webcam_config.aec);
}

void CmndWebcamAECValue(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1200)) {
    Settings->webcam_config2.aec_value = XdrvMailbox.payload;
    WcSetOptions(11, Settings->webcam_config2.aec_value);
  }
  ResponseCmndNumber(Settings->webcam_config2.aec_value);
}

void CmndWebcamAELevel(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config2.ae_level = XdrvMailbox.payload + 2;
    WcSetOptions(12, Settings->webcam_config2.ae_level - 2);
  }
  ResponseCmndNumber(Settings->webcam_config2.ae_level - 2);
}

void CmndWebcamAEC2(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.aec2 = XdrvMailbox.payload;
    WcSetOptions(13, Settings->webcam_config.aec2);
  }
  ResponseCmndStateText(Settings->webcam_config.aec2);
}

void CmndWebcamAGC(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.agc = XdrvMailbox.payload;
    WcSetOptions(14, Settings->webcam_config.agc);
  }
  ResponseCmndStateText(Settings->webcam_config.agc);
}

void CmndWebcamAGCGain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 30)) {
    Settings->webcam_config2.agc_gain = XdrvMailbox.payload;
    WcSetOptions(15, Settings->webcam_config2.agc_gain);
  }
  ResponseCmndNumber(Settings->webcam_config2.agc_gain);
}

void CmndWebcamGainCeiling(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
    Settings->webcam_config2.gainceiling = XdrvMailbox.payload;
    WcSetOptions(16, Settings->webcam_config2.gainceiling);
  }
  ResponseCmndNumber(Settings->webcam_config2.gainceiling);
}

void CmndWebcamGammaCorrect(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.raw_gma = XdrvMailbox.payload;
    WcSetOptions(17, Settings->webcam_config.raw_gma);
  }
  ResponseCmndStateText(Settings->webcam_config.raw_gma);
}

void CmndWebcamLensCorrect(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.lenc = XdrvMailbox.payload;
    WcSetOptions(18, Settings->webcam_config.lenc);
  }
  ResponseCmndStateText(Settings->webcam_config.lenc);
}

void CmndWebcamWPC(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.wpc = XdrvMailbox.payload;
    WcSetOptions(19, Settings->webcam_config.wpc);
  }
  ResponseCmndStateText(Settings->webcam_config.wpc);
}

void CmndWebcamDCW(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.dcw = XdrvMailbox.payload;
    WcSetOptions(20, Settings->webcam_config.dcw);
  }
  ResponseCmndStateText(Settings->webcam_config.dcw);
}

void CmndWebcamBPC(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.bpc = XdrvMailbox.payload;
    WcSetOptions(21, Settings->webcam_config.bpc);
  }
  ResponseCmndStateText(Settings->webcam_config.bpc);
}

void CmndWebcamColorbar(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.colorbar = XdrvMailbox.payload;
    WcSetOptions(22, Settings->webcam_config.colorbar);
  }
  ResponseCmndStateText(Settings->webcam_config.colorbar);
}

void CmndWebcamFeature(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config.feature = XdrvMailbox.payload;
    WcSetOptions(23, Settings->webcam_config.feature);
  }
  ResponseCmndNumber(Settings->webcam_config.feature);
}

void CmndWebcamAuth(void){
  if((XdrvMailbox.payload >=0) && (XdrvMailbox.payload <= 1)){
    Settings->webcam_config2.auth = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->webcam_config2.auth);
}

void CmndWebcamClock(void){
  if((XdrvMailbox.payload >= 10) && (XdrvMailbox.payload <= 200)){
    Settings->webcam_clk = XdrvMailbox.payload;
    WcInterruptControl();
  }
  ResponseCmndNumber(Settings->webcam_clk);
}

void CmndWebcamInit(void) {
  WcSetup(Settings->webcam_config.resolution);
  WcInterruptControl();
  ResponseCmndDone();
}

void CmndWebcamSetDefaults(void) {
  WcSetDefaults(0);
  ResponseCmndDone();
}

#define D_WEBCAM_STATS_FPS "FPS"
#define D_WEBCAM_STATS_CAMFAIL "CamFail"
#define D_WEBCAM_STATS_JPEGFAIL "JpegFail"
#define D_WEBCAM_STATS_CLIENTFAIL "ClientFail"

void CmndWebcamStats(void) {
  Response_P(PSTR("{\"" D_PRFX_WEBCAM D_CMND_WC_STATS "\":{\"" D_WEBCAM_STATS_FPS "\":%d,\""
    D_WEBCAM_STATS_CAMFAIL "\":%d,\"" D_WEBCAM_STATS_JPEGFAIL "\":%d,\"" D_WEBCAM_STATS_CLIENTFAIL "\":%d"
  "}}"),
  WcStats.camfps, WcStats.camfail, WcStats.jpegfail, WcStats.clientfail
  );
}

#ifdef ENABLE_RTSPSERVER
void CmndWebRtsp(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.rtsp = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndStateText(Settings->webcam_config.rtsp);
}
#endif // ENABLE_RTSPSERVER

void WcUpdateStats(void) {
  WcStats.camfps = WcStats.camcnt;
  WcStats.camcnt = 0;
}

const char HTTP_WEBCAM_FPS[] PROGMEM = "{s}%s " D_FRAME_RATE "{m}%d " D_UNIT_FPS  "{e}";

void WcStatsShow(void) {
#ifdef USE_WEBSERVER
  WSContentSend_PD(HTTP_WEBCAM_FPS, WcStats.name, WcStats.camfps);
#endif  // USE_WEBSERVER
}

#endif //USE_WEBCAM_SETUP_ONLY

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv81(uint32_t function) {
  bool result = false;
  #ifndef USE_WEBCAM_SETUP_ONLY
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
    case FUNC_EVERY_SECOND:
      WcUpdateStats();
    case FUNC_WEB_SENSOR:
      WcStatsShow();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kWCCommands, WCCommand);
      break;
    case FUNC_PRE_INIT:
      WcInit();
      break;
    case FUNC_INIT:
      if(Wc.up == 0) WcSetup(Settings->webcam_config.resolution);
      break;
    case FUNC_ACTIVE:
      result = true;
      break;
  }
  #endif // USE_WEBCAM_SETUP_ONLY
  return result;
}

#endif  // USE_WEBCAM_LEGACY
#endif  // USE_WEBCAM
#endif  // ESP32
