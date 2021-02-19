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
/*********************************************************************************************\
 * ESP32 webcam based on example in Arduino-ESP32 library
 *
 * Template as used on ESP32-CAM WiFi + bluetooth Camera Module Development Board ESP32 With Camera Module OV2640 Geekcreit for Arduino
 * {"NAME":"AITHINKER CAM","GPIO":[4992,1,1,1,1,5088,1,1,1,1,1,1,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,1,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":1}
 *
 * Supported commands:
 * WcStream     = Control streaming, 0 = stop, 1 = start
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
 * WcInit       = Init Camera Interface
 * WcRtsp       = Control RTSP Server, 0=disable, 1=enable (forces restart) (if defined ENABLE_RTSPSERVER)
 *
 * Only boards with PSRAM should be used. To enable PSRAM board should be se set to esp32cam in common32 of platform_override.ini
 * board                   = esp32cam
 * To speed up cam processing cpu frequency should be better set to 240Mhz in common32 of platform_override.ini
 * board_build.f_cpu       = 240000000L
 * remarks for AI-THINKER
 * GPIO0 zero must be disconnected from any wire after programming because this pin drives the cam clock and does
 * not tolerate any capictive load
 * flash led = gpio 4
 * red led = gpio 33
 * optional rtsp url: rtsp://xxx.xxx.xxx.xxx:8554/mjpeg/1
 */

/*********************************************************************************************/

#define XDRV_81           81

#include "esp_camera.h"
#include "sensor.h"
#include "fb_gfx.h"
#include "fd_forward.h"
#include "fr_forward.h"

bool HttpCheckPriviledgedAccess(bool);
extern ESP8266WebServer *Webserver;

#define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"



// CAMERA_MODEL_AI_THINKER default template pins
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

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

struct {
  uint8_t  up;
  uint16_t width;
  uint16_t height;
  uint8_t  stream_active;
  WiFiClient client;
  ESP8266WebServer *CamServer;
  struct PICSTORE picstore[MAX_PICSTORE];
#ifdef USE_FACE_DETECT
  uint8_t  faces;
  uint16_t face_detect_time;
  uint32_t face_ltime;
  mtmn_config_t mtmn_config = {0};
#endif // USE_FACE_DETECT
#ifdef ENABLE_RTSPSERVER
  WiFiServer *rtspp;
  CStreamer *rtsp_streamer;
  CRtspSession *rtsp_session;
  WiFiClient rtsp_client;
  uint8_t rtsp_start;
  OV2640 cam;
  uint32_t rtsp_lastframe_time;
#endif // ENABLE_RTSPSERVER
} Wc;



/*********************************************************************************************/

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
  if (!PinUsed(GPIO_WEBCAM_XCLK) || !PinUsed(GPIO_WEBCAM_PCLK) ||
      !PinUsed(GPIO_WEBCAM_VSYNC) || !PinUsed(GPIO_WEBCAM_HREF) ||
      !PinUsed(GPIO_WEBCAM_SIOD) || !PinUsed(GPIO_WEBCAM_SIOC)) {
    pin_used = false;
  }
  return pin_used;
}

uint32_t WcSetup(int32_t fsiz) {
  if (fsiz >= FRAMESIZE_FHD) { fsiz = FRAMESIZE_FHD - 1; }

  Wc.stream_active = 0;

  if (fsiz < 0) {
    esp_camera_deinit();
    Wc.up = 0;
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
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
//  config.pixel_format = PIXFORMAT_GRAYSCALE;
//  config.pixel_format = PIXFORMAT_RGB565;

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
    config.pin_sscb_sda = Pin(GPIO_WEBCAM_SIOD);  // SIOD_GPIO_NUM;
    config.pin_sscb_scl = Pin(GPIO_WEBCAM_SIOC);  // SIOC_GPIO_NUM;
    config.pin_pwdn = Pin(GPIO_WEBCAM_PWDN);       // PWDN_GPIO_NUM;
    config.pin_reset = Pin(GPIO_WEBCAM_RESET);    // RESET_GPIO_NUM;

    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: User template"));
  } else {
    // defaults to AI THINKER
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
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Default template"));
  }

  //ESP.getPsramSize()

  //esp_log_level_set("*", ESP_LOG_INFO);

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.

  bool psram = psramFound();
  if (psram) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: PSRAM found"));
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: PSRAM not found"));
  }

//  AddLog(LOG_LEVEL_INFO, PSTR("CAM: heap check 1: %d"),ESP_getFreeHeap());

  // stupid workaround camera diver eats up static ram should prefer PSRAM
  // so we steal static ram to force driver to alloc PSRAM
//  uint32_t maxfram = ESP.getMaxAllocHeap();
//  void *x=malloc(maxfram-4096);
  void *x = 0;
  esp_err_t err = esp_camera_init(&config);
  if (x) { free(x); }

  if (err != ESP_OK) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Init failed with error 0x%x"), err);
    return 0;
  }

//  AddLog(LOG_LEVEL_INFO, PSTR("CAM: heap check 2: %d"),ESP_getFreeHeap());

  sensor_t * wc_s = esp_camera_sensor_get();

  wc_s->set_vflip(wc_s, Settings.webcam_config.flip);
  wc_s->set_hmirror(wc_s, Settings.webcam_config.mirror);
  wc_s->set_brightness(wc_s, Settings.webcam_config.brightness -2);  // up the brightness just a bit
  wc_s->set_saturation(wc_s, Settings.webcam_config.saturation -2);  // lower the saturation
  wc_s->set_contrast(wc_s, Settings.webcam_config.contrast -2);      // keep contrast

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

#ifdef USE_FACE_DETECT
  fd_init();
#endif

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: Initialized"));

  Wc.up = 1;
  if (psram) { Wc.up = 2; }

  return Wc.up;
}

/*********************************************************************************************/

int32_t WcSetOptions(uint32_t sel, int32_t value) {
  int32_t res = 0;
  sensor_t *s = esp_camera_sensor_get();
  if (!s) { return -99; }

  switch (sel) {
    case 0:
      if (value >= 0) { s->set_framesize(s, (framesize_t)value); }
      res = s->status.framesize;
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
  }

  return res;
}

uint32_t WcGetWidth(void) {
  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) { return 0; }
  Wc.width = wc_fb->width;
  esp_camera_fb_return(wc_fb);
  return Wc.width;
}

uint32_t WcGetHeight(void) {
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

/*********************************************************************************************/

#ifdef USE_FACE_DETECT

void fd_init(void) {
  Wc.mtmn_config.type = FAST;
  Wc.mtmn_config.min_face = 80;
  Wc.mtmn_config.pyramid = 0.707;
  Wc.mtmn_config.pyramid_times = 4;
  Wc.mtmn_config.p_threshold.score = 0.6;
  Wc.mtmn_config.p_threshold.nms = 0.7;
  Wc.mtmn_config.p_threshold.candidate_number = 20;
  Wc.mtmn_config.r_threshold.score = 0.7;
  Wc.mtmn_config.r_threshold.nms = 0.7;
  Wc.mtmn_config.r_threshold.candidate_number = 10;
  Wc.mtmn_config.o_threshold.score = 0.7;
  Wc.mtmn_config.o_threshold.nms = 0.7;
  Wc.mtmn_config.o_threshold.candidate_number = 1;
}

#define FACE_COLOR_WHITE  0x00FFFFFF
#define FACE_COLOR_BLACK  0x00000000
#define FACE_COLOR_RED    0x000000FF
#define FACE_COLOR_GREEN  0x0000FF00
#define FACE_COLOR_BLUE   0x00FF0000
#define FACE_COLOR_YELLOW (FACE_COLOR_RED | FACE_COLOR_GREEN)
#define FACE_COLOR_CYAN   (FACE_COLOR_BLUE | FACE_COLOR_GREEN)
#define FACE_COLOR_PURPLE (FACE_COLOR_BLUE | FACE_COLOR_RED)
void draw_face_boxes(dl_matrix3du_t *image_matrix, box_array_t *boxes, int face_id);

/*
void draw_face_boxes(dl_matrix3du_t *image_matrix, box_array_t *boxes, int face_id) {
    int x, y, w, h, i;
    uint32_t color = FACE_COLOR_YELLOW;
    if(face_id < 0){
        color = FACE_COLOR_RED;
    } else if(face_id > 0){
        color = FACE_COLOR_GREEN;
    }
    fb_data_t fb;
    fb.width = image_matrix->w;
    fb.height = image_matrix->h;
    fb.data = image_matrix->item;
    fb.bytes_per_pixel = 3;
    fb.format = FB_BGR888;
    for (i = 0; i < boxes->len; i++){
        // rectangle box
        x = (int)boxes->box[i].box_p[0];
        y = (int)boxes->box[i].box_p[1];
        w = (int)boxes->box[i].box_p[2] - x + 1;
        h = (int)boxes->box[i].box_p[3] - y + 1;
        fb_gfx_drawFastHLine(&fb, x, y, w, color);
        fb_gfx_drawFastHLine(&fb, x, y+h-1, w, color);
        fb_gfx_drawFastVLine(&fb, x, y, h, color);
        fb_gfx_drawFastVLine(&fb, x+w-1, y, h, color);
#if 0
        // landmark
        int x0, y0, j;
        for (j = 0; j < 10; j+=2) {
            x0 = (int)boxes->landmark[i].landmark_p[j];
            y0 = (int)boxes->landmark[i].landmark_p[j+1];
            fb_gfx_fillRect(&fb, x0, y0, 3, 3, color);
        }
#endif
    }
}
*/

//#define DL_SPIRAM_SUPPORT

uint32_t WcSetFaceDetect(int32_t value) {
  if (value >= 0) { Wc.face_detect_time = value; }
  return Wc.faces;
}

uint32_t WcDetectFace(void);

uint32_t WcDetectFace(void) {
  dl_matrix3du_t *image_matrix;
  size_t out_len, out_width, out_height;
  uint8_t * out_buf;
  bool s;
  bool detected = false;
  int face_id = 0;
  camera_fb_t *fb;

  if ((millis() - Wc.face_ltime) > Wc.face_detect_time) {
    Wc.face_ltime = millis();
    fb = esp_camera_fb_get();
    if (!fb) { return ESP_FAIL; }

    image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
    if (!image_matrix) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: dl_matrix3du_alloc failed"));
      esp_camera_fb_return(fb);
      return ESP_FAIL;
    }

    out_buf = image_matrix->item;
    //out_len = fb->width * fb->height * 3;
    //out_width = fb->width;
    //out_height = fb->height;

    s = fmt2rgb888(fb->buf, fb->len, fb->format, out_buf);
    esp_camera_fb_return(fb);
    if (!s){
      dl_matrix3du_free(image_matrix);
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: to rgb888 failed"));
      return ESP_FAIL;
    }

    box_array_t *net_boxes = face_detect(image_matrix, &Wc.mtmn_config);
    if (net_boxes){
      detected = true;
      Wc.faces = net_boxes->len;
      //if(recognition_enabled){
      //    face_id = run_face_recognition(image_matrix, net_boxes);
      //}
      //draw_face_boxes(image_matrix, net_boxes, face_id);
      free(net_boxes->score);
      free(net_boxes->box);
      free(net_boxes->landmark);
      free(net_boxes);
    } else {
      Wc.faces = 0;
    }
    dl_matrix3du_free(image_matrix);
    //Serial.printf("face detected: %d",Wc.faces);

  }
  return 0;
}
#endif

/*********************************************************************************************/


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

void HandleImage(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  uint32_t bnum = Webserver->arg(F("p")).toInt();
  if ((bnum < 0) || (bnum > MAX_PICSTORE)) { bnum= 1; }
  WiFiClient client = Webserver->client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.jpg\r\n";
  response += "Content-type: image/jpeg\r\n\r\n";
  Webserver->sendContent(response);

  if (!bnum) {
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    camera_fb_t *wc_fb = 0;
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) { return; }
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

  if (Settings.webcam_config.stream) {
    if (!Wc.CamServer) {
      WcStreamControl();
    }
  }

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
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Frame fail"));
      Wc.stream_active = 0;
    }
  }
  if (2 == Wc.stream_active) {
    if (wc_fb->format != PIXFORMAT_JPEG) {
      jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
      if (!jpeg_converted){
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: JPEG compression failed"));
        _jpg_buf_len = wc_fb->len;
        _jpg_buf = wc_fb->buf;
      }
    } else {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }

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
    Wc.client.print("\r\n--" BOUNDARY "\r\n");

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
  //CamServer->redirect("http://" + String(ip) + ":81/cam.mjpeg");
  Wc.CamServer->sendHeader("Location", WiFi.localIP().toString() + ":81/cam.mjpeg");
  Wc.CamServer->send(302, "", "");
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Root called"));
}

/*********************************************************************************************/

uint32_t WcSetStreamserver(uint32_t flag) {
  if (TasmotaGlobal.global_state.network_down) { return 0; }

  Wc.stream_active = 0;

  if (flag) {
    if (!Wc.CamServer) {
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
      Wc.CamServer->stop();
      delete Wc.CamServer;
      Wc.CamServer = NULL;
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stream exit"));
    }
  }
  return 0;
}

void WcStreamControl() {
  WcSetStreamserver(Settings.webcam_config.stream);
  WcSetup(Settings.webcam_config.resolution);
}

/*********************************************************************************************/


void WcLoop(void) {
  if (Wc.CamServer) {
    Wc.CamServer->handleClient();
    if (Wc.stream_active) { HandleWebcamMjpegTask(); }
  }
  if (wc_motion.motion_detect) { WcDetectMotion(); }
#ifdef USE_FACE_DETECT
  if (Wc.face_detect_time) { WcDetectFace(); }
#endif

#ifdef ENABLE_RTSPSERVER
    if (Settings.webcam_config.rtsp && !TasmotaGlobal.global_state.wifi_down && Wc.up) {
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
  if (Settings.webcam_config.stream) {
//    if (!Wc.CamServer || !Wc.up) {
    if (!Wc.CamServer) {
      WcStreamControl();
      delay(50);   // Give the webcam webserver some time to prepare the stream
    }
    if (Wc.CamServer && Wc.up) {
      WSContentSend_P(PSTR("<p></p><center><img src='http://%_I:81/stream' alt='Webcam stream' style='width:99%%;'></center><p></p>"),
        (uint32_t)WiFi.localIP());
    }
  }
}

void WcInit(void) {
  if (!Settings.webcam_config.data) {
    Settings.webcam_config.stream = 1;
    Settings.webcam_config.resolution = FRAMESIZE_QVGA;
    Settings.webcam_config.flip = 0;
    Settings.webcam_config.mirror = 0;
    Settings.webcam_config.saturation = 0;  // -2
    Settings.webcam_config.brightness = 3;  // 1
    Settings.webcam_config.contrast = 2;    // 0
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_PRFX_WEBCAM "WC"
#define D_CMND_WC_STREAM "Stream"
#define D_CMND_WC_RESOLUTION "Resolution"
#define D_CMND_WC_MIRROR "Mirror"
#define D_CMND_WC_FLIP "Flip"
#define D_CMND_WC_SATURATION "Saturation"
#define D_CMND_WC_BRIGHTNESS "Brightness"
#define D_CMND_WC_CONTRAST "Contrast"
#define D_CMND_WC_INIT "Init"
#define D_CMND_RTSP "Rtsp"

const char kWCCommands[] PROGMEM =  D_PRFX_WEBCAM "|"  // Prefix
  "|" D_CMND_WC_STREAM "|" D_CMND_WC_RESOLUTION "|" D_CMND_WC_MIRROR "|" D_CMND_WC_FLIP "|"
  D_CMND_WC_SATURATION "|" D_CMND_WC_BRIGHTNESS "|" D_CMND_WC_CONTRAST "|" D_CMND_WC_INIT
#ifdef ENABLE_RTSPSERVER
  "|" D_CMND_RTSP
#endif // ENABLE_RTSPSERVER
  ;

void (* const WCCommand[])(void) PROGMEM = {
  &CmndWebcam, &CmndWebcamStream, &CmndWebcamResolution, &CmndWebcamMirror, &CmndWebcamFlip,
  &CmndWebcamSaturation, &CmndWebcamBrightness, &CmndWebcamContrast, &CmndWebcamInit
#ifdef ENABLE_RTSPSERVER
  , &CmndWebRtsp
#endif // ENABLE_RTSPSERVER
  };

void CmndWebcam(void) {
  Response_P(PSTR("{\"" D_PRFX_WEBCAM "\":{\"" D_CMND_WC_STREAM "\":%d,\"" D_CMND_WC_RESOLUTION "\":%d,\"" D_CMND_WC_MIRROR "\":%d,\""
    D_CMND_WC_FLIP "\":%d,\""
    D_CMND_WC_SATURATION "\":%d,\"" D_CMND_WC_BRIGHTNESS "\":%d,\"" D_CMND_WC_CONTRAST "\":%d"
#ifdef ENABLE_RTSPSERVER
  ",\"" D_CMND_RTSP "\":%d"
#endif // ENABLE_RTSPSERVER
  "}}"),
    Settings.webcam_config.stream, Settings.webcam_config.resolution, Settings.webcam_config.mirror,
    Settings.webcam_config.flip,
    Settings.webcam_config.saturation -2, Settings.webcam_config.brightness -2, Settings.webcam_config.contrast -2
#ifdef ENABLE_RTSPSERVER
  , Settings.webcam_config.rtsp
#endif // ENABLE_RTSPSERVER
  );
}

void CmndWebcamStream(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings.webcam_config.stream = XdrvMailbox.payload;
    if (!Settings.webcam_config.stream) { WcStreamControl(); }  // Stop stream
  }
  ResponseCmndStateText(Settings.webcam_config.stream);
}

void CmndWebcamResolution(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < FRAMESIZE_FHD)) {
    Settings.webcam_config.resolution = XdrvMailbox.payload;
    WcSetOptions(0, Settings.webcam_config.resolution);
  }
  ResponseCmndNumber(Settings.webcam_config.resolution);
}

void CmndWebcamMirror(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings.webcam_config.mirror = XdrvMailbox.payload;
    WcSetOptions(3, Settings.webcam_config.mirror);
  }
  ResponseCmndStateText(Settings.webcam_config.mirror);
}

void CmndWebcamFlip(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings.webcam_config.flip = XdrvMailbox.payload;
    WcSetOptions(2, Settings.webcam_config.flip);
  }
  ResponseCmndStateText(Settings.webcam_config.flip);
}

void CmndWebcamSaturation(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings.webcam_config.saturation = XdrvMailbox.payload +2;
    WcSetOptions(6, Settings.webcam_config.saturation -2);
  }
  ResponseCmndNumber(Settings.webcam_config.saturation -2);
}

void CmndWebcamBrightness(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings.webcam_config.brightness = XdrvMailbox.payload +2;
    WcSetOptions(5, Settings.webcam_config.brightness -2);
  }
  ResponseCmndNumber(Settings.webcam_config.brightness -2);
}

void CmndWebcamContrast(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings.webcam_config.contrast = XdrvMailbox.payload +2;
    WcSetOptions(4, Settings.webcam_config.contrast -2);
  }
  ResponseCmndNumber(Settings.webcam_config.contrast -2);
}

void CmndWebcamInit(void) {
  WcStreamControl();
  ResponseCmndDone();
}

#ifdef ENABLE_RTSPSERVER
void CmndWebRtsp(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings.webcam_config.rtsp = XdrvMailbox.payload;
    TasmotaGlobal.restart_flag = 2;
  }
  ResponseCmndStateText(Settings.webcam_config.rtsp);
}
#endif // ENABLE_RTSPSERVER


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv81(uint8_t function) {
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
    case FUNC_COMMAND:
      result = DecodeCommand(kWCCommands, WCCommand);
      break;
    case FUNC_PRE_INIT:
      WcInit();
      break;

  }
  return result;
}

#endif  // USE_WEBCAM
#endif  // ESP32
