/*
  xdrv_39_webcam.ino - ESP32 webcam support for Tasmota

  Copyright (C) 2020  Gerhard Mutz and Theo Arends

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
 * {"NAME":"AITHINKER CAM No SPI","GPIO":[4992,65504,65504,65504,5472,5312,65504,65504,5504,5536,65504,65504,5568,5440,5280,5248,0,5216,5408,5376,0,5344,5024,5056,0,0,0,0,4928,65504,5120,5088,5184,0,0,5152],"FLAG":0,"BASE":1}
 * Template with SPI configured. This needs define USE_SPI
 * {"NAME":"AITHINKER CAM","GPIO":[4992,65504,672,65504,5472,5312,65504,65504,5504,5536,736,704,5568,5440,5280,5248,0,5216,5408,5376,0,5344,5024,5056,0,0,0,0,4928,65504,5120,5088,5184,0,0,5152],"FLAG":0,"BASE":1}
 *
 * Command: Webcam <number>
 *  0 = Stop streaming
 *  1 = FRAMESIZE_QQVGA2 (128x160)
 *  2 = FRAMESIZE_QCIF (176x144)
 *  3 = FRAMESIZE_HQVGA (240x176)
 *  4 = FRAMESIZE_QVGA (320x240)
 *  5 = FRAMESIZE_CIF (400x296)
 *  6 = FRAMESIZE_VGA (640x480)
 *  7 = FRAMESIZE_SVGA (800x600)
 *  8 = FRAMESIZE_XGA (1024x768)
 *  9 = FRAMESIZE_SXGA (1280x1024)
 * 10 = FRAMESIZE_UXGA (1600x1200)
 *
 * Only boards with PSRAM should be used. To enable PSRAM board should be se set to esp32cam in common32 of platform_override.ini
 * board                   = esp32cam
 * To speed up cam processing cpu frequency should be better set to 240Mhz in common32 of platform_override.ini
 * board_build.f_cpu       = 240000000L
\*********************************************************************************************/

#define XDRV_39                    39

#define CAMERA_MODEL_AI_THINKER

//#define USE_TEMPLATE

#define WC_LOGLEVEL LOG_LEVEL_INFO
#include "fb_gfx.h"
#include "fd_forward.h"
#include "fr_forward.h"

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


#include "esp_camera.h"
#include "sensor.h"

uint8_t wc_up;
uint16_t wc_width;
uint16_t wc_height;
uint8_t wc_stream_active;
#ifdef USE_FACE_DETECT
uint8_t faces;
uint16_t face_detect_time;
#endif

uint32_t wc_setup(int32_t fsiz) {
  if (fsiz > 10) { fsiz = 10; }

  wc_stream_active = 0;

  if (fsiz < 0) {
    esp_camera_deinit();
    wc_up = 0;
    return 0;
  }

  if (wc_up) {
    esp_camera_deinit();
    AddLog_P2(WC_LOGLEVEL, PSTR("CAM: deinit"));
    //return wc_up;
  }

//esp_log_level_set("*", ESP_LOG_VERBOSE);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
//  config.pixel_format = PIXFORMAT_GRAYSCALE;
//  config.pixel_format = PIXFORMAT_RGB565;

#ifndef USE_TEMPLATE
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
#else
  if (PinUsed(GPIO_WEBCAM_Y2) && PinUsed(GPIO_WEBCAM_Y3) && PinUsed(GPIO_WEBCAM_Y4) && PinUsed(GPIO_WEBCAM_Y5)\
   && PinUsed(GPIO_WEBCAM_Y6) && PinUsed(GPIO_WEBCAM_Y7) && PinUsed(GPIO_WEBCAM_Y8) && PinUsed(GPIO_WEBCAM_Y9)\
   && PinUsed(GPIO_WEBCAM_XCLK) && PinUsed(GPIO_WEBCAM_PCLK) && PinUsed(GPIO_WEBCAM_VSYNC) && PinUsed(GPIO_WEBCAM_HREF)\
   && PinUsed(GPIO_WEBCAM_SIOD) && PinUsed(GPIO_WEBCAM_SIOC)) {
    config.pin_d0 = Pin(GPIO_WEBCAM_Y2);  //Y2_GPIO_NUM;
    config.pin_d1 = Pin(GPIO_WEBCAM_Y3);  //Y3_GPIO_NUM;
    config.pin_d2 = Pin(GPIO_WEBCAM_Y4);  //Y4_GPIO_NUM;
    config.pin_d3 = Pin(GPIO_WEBCAM_Y5);  //Y5_GPIO_NUM;
    config.pin_d4 = Pin(GPIO_WEBCAM_Y6);  //Y6_GPIO_NUM;
    config.pin_d5 = Pin(GPIO_WEBCAM_Y7);  //Y7_GPIO_NUM;
    config.pin_d6 = Pin(GPIO_WEBCAM_Y8);  //Y8_GPIO_NUM;
    config.pin_d7 = Pin(GPIO_WEBCAM_Y9);  //Y9_GPIO_NUM;
    config.pin_xclk = Pin(GPIO_WEBCAM_XCLK);  //XCLK_GPIO_NUM;
    config.pin_pclk = Pin(GPIO_WEBCAM_PCLK);  //PCLK_GPIO_NUM;
    config.pin_vsync = Pin(GPIO_WEBCAM_VSYNC);  //VSYNC_GPIO_NUM;
    config.pin_href = Pin(GPIO_WEBCAM_HREF);  //HREF_GPIO_NUM;
    config.pin_sscb_sda = Pin(GPIO_WEBCAM_SIOD);  //SIOD_GPIO_NUM;
    config.pin_sscb_scl = Pin(GPIO_WEBCAM_SIOC);  //SIOC_GPIO_NUM;
    int16_t xpin;
    xpin = Pin(GPIO_WEBCAM_PWDN);
    if (99 == xpin) { xpin = -1; }
    config.pin_pwdn = xpin; //PWDN_GPIO_NUM;
    xpin = Pin(GPIO_WEBCAM_RESET);
    if (99 == xpin) { xpin=-1; }
    config.pin_reset = xpin; //RESET_GPIO_NUM;
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
  }
#endif

  //ESP.getPsramSize()

  //esp_log_level_set("*", ESP_LOG_INFO);


  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.

  bool psram = psramFound();
  if (psram) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    AddLog_P2(WC_LOGLEVEL, PSTR("CAM: PSRAM found"));
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    AddLog_P2(WC_LOGLEVEL, PSTR("CAM: PSRAM not found"));
  }

  // stupid workaround camera diver eats up static ram should prefer PSRAM
  // so we steal static ram to force driver to alloc PSRAM
  //ESP.getMaxAllocHeap()

//  void *x=malloc(70000);
  void *x = 0;
  esp_err_t err = esp_camera_init(&config);
  if (x) { free(x); }

  if (err != ESP_OK) {
    AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Init failed with error 0x%x"), err);
    return 0;
  }

  sensor_t * wc_s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (OV3660_PID == wc_s->id.PID) {
    wc_s->set_vflip(wc_s, 1);        // flip it back
    wc_s->set_brightness(wc_s, 1);   // up the brightness just a bit
    wc_s->set_saturation(wc_s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  wc_s->set_framesize(wc_s, (framesize_t)fsiz);

  camera_fb_t *wc_fb = esp_camera_fb_get();
  wc_width = wc_fb->width;
  wc_height = wc_fb->height;
  esp_camera_fb_return(wc_fb);


#ifdef USE_FACE_DETECT
  fd_init();
#endif

  AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Initialized"));

  wc_up = 1;
  if (psram) { wc_up=2; }

  return wc_up;
}

int32_t wc_set_options(uint32_t sel, int32_t value) {
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

uint32_t wc_get_width(void) {
  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) { return 0; }
  wc_width = wc_fb->width;
  esp_camera_fb_return(wc_fb);
  return wc_width;
}

uint32_t wc_get_height(void) {
  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) { return 0; }
  wc_height = wc_fb->height;
  esp_camera_fb_return(wc_fb);
  return wc_height;
}

#ifndef MAX_PICSTORE
#define MAX_PICSTORE 4
#endif
struct PICSTORE {
  uint8_t *buff;
  uint32_t len;
};

struct PICSTORE picstore[MAX_PICSTORE];

#ifdef COPYFRAME
struct PICSTORE tmp_picstore;
#endif

uint32_t get_picstore(int32_t num, uint8_t **buff) {
  if (num<0) { return MAX_PICSTORE; }
  *buff = picstore[num].buff;
  return picstore[num].len;
}

uint32_t wc_get_jpeg(uint8_t **buff) {
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  camera_fb_t *wc_fb;
  wc_fb = esp_camera_fb_get();
  if (!wc_fb) { return 0; }
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
  esp_camera_fb_return(wc_fb);
  *buff = _jpg_buf;
  return _jpg_buf_len;
}

uint32_t wc_get_frame(int32_t bnum) {
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  camera_fb_t *wc_fb = 0;
  bool jpeg_converted = false;

  if (bnum < 0) {
    if (bnum < -MAX_PICSTORE) { bnum=-1; }
    bnum = -bnum;
    bnum--;
    if (picstore[bnum].buff) { free(picstore[bnum].buff); }
    picstore[bnum].len = 0;
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
    AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Can't get frame"));
    return 0;
  }
  if (!bnum) {
    wc_width = wc_fb->width;
    wc_height = wc_fb->height;
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
  if (picstore[bnum].buff) { free(picstore[bnum].buff); }
  picstore[bnum].buff = (uint8_t *)heap_caps_malloc(_jpg_buf_len+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (picstore[bnum].buff) {
    memcpy(picstore[bnum].buff, _jpg_buf, _jpg_buf_len);
    picstore[bnum].len = _jpg_buf_len;
  } else {
    AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Can't allocate picstore"));
    picstore[bnum].len = 0;
  }
  if (wc_fb) { esp_camera_fb_return(wc_fb); }
  if (jpeg_converted) { free(_jpg_buf); }
  if (!picstore[bnum].buff) { return 0; }

  return  _jpg_buf_len;
}

bool HttpCheckPriviledgedAccess(bool);
extern ESP8266WebServer *Webserver;

void HandleImage(void) {
  if (!HttpCheckPriviledgedAccess(true)) { return; }

  uint32_t bnum = Webserver->arg(F("p")).toInt();
  if ((bnum < 0) || (bnum > MAX_PICSTORE)) { bnum= 1; }
  WiFiClient client = Webserver->client();
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.jpg\r\n";
  response += "Content-type: image/jpeg\r\n\r\n";
  Webserver->sendContent(response);

  if (!bnum) {
    uint8_t *buff;
    uint32_t len;
    len = wc_get_jpeg(&buff);
    if (len) {
      client.write(buff,len);
      free(buff);
    }
  } else {
    bnum--;
    if (!picstore[bnum].len) {
      AddLog_P2(WC_LOGLEVEL, PSTR("CAM: No image #: %d"), bnum);
      return;
    }
    client.write((char *)picstore[bnum].buff, picstore[bnum].len);
  }

  AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Sending image #: %d"), bnum+1);
}

ESP8266WebServer *CamServer;
#define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"

WiFiClient client;

void handleMjpeg(void) {
  AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Handle camserver"));
  //if (!wc_stream_active) {
    wc_stream_active = 1;
    client = CamServer->client();
    AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Create client"));
  //}
}

static mtmn_config_t mtmn_config = {0};

#ifdef USE_FACE_DETECT
void fd_init(void) {
  mtmn_config.type = FAST;
  mtmn_config.min_face = 80;
  mtmn_config.pyramid = 0.707;
  mtmn_config.pyramid_times = 4;
  mtmn_config.p_threshold.score = 0.6;
  mtmn_config.p_threshold.nms = 0.7;
  mtmn_config.p_threshold.candidate_number = 20;
  mtmn_config.r_threshold.score = 0.7;
  mtmn_config.r_threshold.nms = 0.7;
  mtmn_config.r_threshold.candidate_number = 10;
  mtmn_config.o_threshold.score = 0.7;
  mtmn_config.o_threshold.nms = 0.7;
  mtmn_config.o_threshold.candidate_number = 1;
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

#define DL_SPIRAM_SUPPORT

uint32_t wc_set_face_detect(int32_t value) {
  if (value >= 0) { face_detect_time=value; }
  return faces;
}

uint32_t face_ltime;

uint32_t detect_face(void);

uint32_t detect_face(void) {
dl_matrix3du_t *image_matrix;
size_t out_len, out_width, out_height;
uint8_t * out_buf;
bool s;
bool detected = false;
int face_id = 0;
camera_fb_t *fb;

  if ((millis()-face_ltime) > face_detect_time) {
    face_ltime = millis();
    fb = esp_camera_fb_get();
    if (!fb) { return ESP_FAIL; }

    image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
    if (!image_matrix) {
      AddLog_P2(WC_LOGLEVEL, PSTR("CAM: dl_matrix3du_alloc failed"));
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
      AddLog_P2(WC_LOGLEVEL, PSTR("CAM: to rgb888 failed"));
      return ESP_FAIL;
    }

    box_array_t *net_boxes = face_detect(image_matrix, &mtmn_config);
    if (net_boxes){
      detected = true;
      faces=net_boxes->len;
      //if(recognition_enabled){
      //    face_id = run_face_recognition(image_matrix, net_boxes);
      //}
      //draw_face_boxes(image_matrix, net_boxes, face_id);
      free(net_boxes->score);
      free(net_boxes->box);
      free(net_boxes->landmark);
      free(net_boxes);
    } else {
      faces=0;
    }
    dl_matrix3du_free(image_matrix);
    //Serial.printf("face detected: %d",faces);

  }
}
#endif

void handleMjpeg_task(void) {
  camera_fb_t *wc_fb;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;

  //WiFiClient client = CamServer->client();
  uint32_t tlen;
  bool jpeg_converted = false;

  if (!client.connected()) {
    wc_stream_active = 0;
    AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Client fail"));
    goto exit;
  }

  if (1 == wc_stream_active) {
    client.flush();
    client.setTimeout(3);
    AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Start stream"));
    client.print("HTTP/1.1 200 OK\r\n"
      "Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n"
      "\r\n");
    wc_stream_active = 2;
  } else {
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) {
      wc_stream_active = 0;
      AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Frame fail"));
      goto exit;
    }


    if (wc_fb->format != PIXFORMAT_JPEG) {
      jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
      if (!jpeg_converted){
        AddLog_P2(WC_LOGLEVEL, PSTR("CAM: JPEG compression failed"));
        _jpg_buf_len = wc_fb->len;
        _jpg_buf = wc_fb->buf;
      }
    } else {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }

    client.printf("Content-Type: image/jpeg\r\n"
      "Content-Length: %d\r\n"
      "\r\n", static_cast<int>(_jpg_buf_len));
    tlen = client.write(_jpg_buf, _jpg_buf_len);
    /*
    if (tlen!=_jpg_buf_len) {
      esp_camera_fb_return(wc_fb);
      wc_stream_active=0;
      AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Send fail"));
    }*/
    client.print("\r\n--" BOUNDARY "\r\n");

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
    //AddLog_P2(WC_LOGLEVEL, PSTR("CAM: send frame"));

exit:
    if (!wc_stream_active) {
      AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Stream exit"));
      client.flush();
      client.stop();
    }
  }
}

void CamHandleRoot(void) {
  //CamServer->redirect("http://" + String(ip) + ":81/cam.mjpeg");
  CamServer->sendHeader("Location", WiFi.localIP().toString() + ":81/cam.mjpeg");
  CamServer->send(302, "", "");
  AddLog_P2(WC_LOGLEVEL, PSTR("CAM: root called"));
}

uint16_t motion_detect;
uint32_t motion_ltime;
uint32_t motion_trigger;
uint32_t motion_brightness;
uint8_t *last_motion_buffer;

uint32_t wc_set_motion_detect(int32_t value) {
  if (value >= 0) { motion_detect=value; }
  if (-1 == value) {
    return motion_trigger;
  } else  {
    return motion_brightness;
  }
}

// optional motion detector
void detect_motion(void) {
  camera_fb_t *wc_fb;
  uint8_t *out_buf=0;

  if ((millis()-motion_ltime) > motion_detect) {
    motion_ltime = millis();
    wc_fb = esp_camera_fb_get();
    if (!wc_fb) { return; }

    if (!last_motion_buffer) {
      last_motion_buffer=(uint8_t *)heap_caps_malloc((wc_fb->width*wc_fb->height)+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    }
    if (last_motion_buffer) {
      if (PIXFORMAT_JPEG == wc_fb->format) {
        out_buf = (uint8_t *)heap_caps_malloc((wc_fb->width*wc_fb->height*3)+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (out_buf) {
          fmt2rgb888(wc_fb->buf, wc_fb->len, wc_fb->format, out_buf);
          uint32_t x, y;
          uint8_t *pxi = out_buf;
          uint8_t *pxr = last_motion_buffer;
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
          motion_trigger = accu / ((wc_fb->height * wc_fb->width) / 100);
          motion_brightness = bright / ((wc_fb->height * wc_fb->width) / 100);
          free(out_buf);
        }
      }
    }
    esp_camera_fb_return(wc_fb);
  }
}

void wc_show_stream(void) {
  if (CamServer) {
    WSContentSend_P(PSTR("<p></p><center><img src='http://%s:81/stream' alt='Webcam stream' style='width:99%%;'></center><p></p>"),
         WiFi.localIP().toString().c_str());
  }
}

uint32_t wc_set_streamserver(uint32_t flag) {
  if (global_state.wifi_down) { return 0; }

  wc_stream_active = 0;

  if (flag) {
    if (!CamServer) {
      CamServer = new ESP8266WebServer(81);
      CamServer->on("/", CamHandleRoot);
      CamServer->on("/cam.mjpeg", handleMjpeg);
      CamServer->on("/cam.jpg", handleMjpeg);
      CamServer->on("/stream", handleMjpeg);
      AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Stream init"));
      CamServer->begin();
    }
  } else {
    if (CamServer) {
      CamServer->stop();
      delete CamServer;
      CamServer = NULL;
      AddLog_P2(WC_LOGLEVEL, PSTR("CAM: Stream exit"));
    }
  }
  return 0;
}

void WcStreamControl(uint32_t resolution) {
  wc_set_streamserver(resolution);
  /*if (0 == resolution) {
    resolution=-1;
  }*/
  wc_setup(resolution);
}

void wc_loop(void) {
  if (CamServer) { CamServer->handleClient(); }
  if (wc_stream_active) { handleMjpeg_task(); }
  if (motion_detect) { detect_motion(); }
#ifdef USE_FACE_DETECT
  if (face_detect_time) { detect_face(); }
#endif
}

void wc_pic_setup(void) {
  Webserver->on("/wc.jpg", HandleImage);
  Webserver->on("/wc.mjpeg", HandleImage);
}

/*
typedef enum {
    // FRAMESIZE_96x96,    // 96x96
    FRAMESIZE_QQVGA,    // 160x120 0
    FRAMESIZE_QQVGA2,   // 128x160 1
    FRAMESIZE_QCIF,     // 176x144 2
    FRAMESIZE_HQVGA,    // 240x176 3

  //  FRAMESIZE_240x240,  // 240x240 3

    FRAMESIZE_QVGA,     // 320x240 4
    FRAMESIZE_CIF,      // 400x296 5
    FRAMESIZE_VGA,      // 640x480 6
    FRAMESIZE_SVGA,     // 800x600 7
    FRAMESIZE_XGA,      // 1024x768 8
    FRAMESIZE_SXGA,     // 1280x1024 9
    FRAMESIZE_UXGA,     // 1600x1200 10


    FRAMESIZE_QXGA,     // 2048*1536
    FRAMESIZE_INVALID
} framesize_t;

flash led = gpio4
red led = gpio 33
*/

void WcInit(void) {
  if (Settings.esp32_webcam_resolution > 10) {
    Settings.esp32_webcam_resolution = 0;
  }
}


/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_CMND_WEBCAM "Webcam"

const char kWCCommands[] PROGMEM =  "|"    // no prefix
  D_CMND_WEBCAM
  ;

void (* const WCCommand[])(void) PROGMEM = {
  &CmndWebcam,
  };

void CmndWebcam(void) {
  uint32_t flag = 0;
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 10)) {
    Settings.esp32_webcam_resolution=XdrvMailbox.payload;
    WcStreamControl(Settings.esp32_webcam_resolution);
  }
  if (CamServer) { flag = 1; }
  Response_P(PSTR("{\"" D_CMND_WEBCAM "\":{\"Streaming\":\"%s\"}"),GetStateText(flag));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv39(uint8_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      wc_loop();
      break;
    case FUNC_WEB_ADD_HANDLER:
      wc_pic_setup();
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
     //if (Settings.esp32_webcam_resolution) {
#ifndef USE_SCRIPT
       WcStreamControl(Settings.esp32_webcam_resolution);
       wc_show_stream();
#endif
     //}
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
