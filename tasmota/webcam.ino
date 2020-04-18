
#if defined(ESP32) && defined(USE_WEBCAM)

#define CAMERA_MODEL_AI_THINKER

//#define USE_TEMPLATE

#ifndef USE_TEMPLATE

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

#endif


#include "esp_camera.h"


//#include "SimStreamer.h"
//#include "OV2640Streamer.h"
//#include "OV2640.h"
//#include "CRtspSession.h"


camera_fb_t *wc_fb;
uint8_t wc_up;
bool psram;
uint16_t wc_width;
uint16_t wc_height;

uint32_t webcam_setup(void) {

  if (wc_up) {
    if (psram) return 2;
    return 1;
  }

camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

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
/*
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
*/

  config.pin_d0 = pin[GPIO_WEBCAM_Y2_GPIO_NUM];  //Y2_GPIO_NUM;
  config.pin_d1 = pin[GPIO_WEBCAM_Y3_GPIO_NUM];  //Y3_GPIO_NUM;
  config.pin_d2 = pin[GPIO_WEBCAM_Y4_GPIO_NUM];  //Y4_GPIO_NUM;
  config.pin_d3 = pin[GPIO_WEBCAM_Y5_GPIO_NUM];  //Y5_GPIO_NUM;
  config.pin_d4 = pin[GPIO_WEBCAM_Y6_GPIO_NUM];  //Y6_GPIO_NUM;
  config.pin_d5 = pin[GPIO_WEBCAM_Y7_GPIO_NUM];  //Y7_GPIO_NUM;
  config.pin_d6 = pin[GPIO_WEBCAM_Y8_GPIO_NUM];  //Y8_GPIO_NUM;
  config.pin_d7 = pin[GPIO_WEBCAM_Y9_GPIO_NUM];  //Y9_GPIO_NUM;
  config.pin_xclk = pin[GPIO_WEBCAM_XCLK_GPIO_NUM];  //XCLK_GPIO_NUM;
  config.pin_pclk = pin[GPIO_WEBCAM_PCLK_GPIO_NUM];  //PCLK_GPIO_NUM;
  config.pin_vsync = pin[GPIO_WEBCAM_VSYNC_GPIO_NUM];  //VSYNC_GPIO_NUM;
  config.pin_href = pin[GPIO_WEBCAM_HREF_GPIO_NUM];  //HREF_GPIO_NUM;
  config.pin_sscb_sda = pin[GPIO_WEBCAM_SIOD_GPIO_NUM];  //SIOD_GPIO_NUM;
  config.pin_sscb_scl = pin[GPIO_WEBCAM_SIOC_GPIO_NUM];  //SIOC_GPIO_NUM;

  int16_t xpin;
  xpin=pin[GPIO_WEBCAM_PWDN_GPIO_NUM];
  if (xpin==99) xpin=-1;
  config.pin_pwdn = xpin; //PWDN_GPIO_NUM;
  xpin=pin[GPIO_WEBCAM_RESET_GPIO_NUM];
  if (xpin==99) xpin=-1;
  config.pin_reset = xpin; //PWDN_GPIO_NUM;
#endif

  //ESP.getPsramSize()

  //esp_log_level_set("*", ESP_LOG_INFO);


  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  psram=psramFound();
  if (psram) {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    AddLog_P(LOG_LEVEL_INFO,"PSRAM found!");
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    AddLog_P(LOG_LEVEL_INFO,"PSRAM not found!");
  }

  //cam.init(esp32cam_config);

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    AddLog_P2(LOG_LEVEL_INFO,"Camera init failed with error 0x%x", err);
    return 0;
  }

  sensor_t * wc_s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (wc_s->id.PID == OV3660_PID) {
    wc_s->set_vflip(wc_s, 1); // flip it back
    wc_s->set_brightness(wc_s, 1); // up the brightness just a bit
    wc_s->set_saturation(wc_s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  wc_s->set_framesize(wc_s, FRAMESIZE_CIF);

  AddLog_P(LOG_LEVEL_INFO,"Camera successfully initialized!");

  wc_up=1;

  if (psram) return 2;
  return 1;
}

uint32_t wc_set_framesize(uint32_t size) {
  sensor_t *s = esp_camera_sensor_get();
  return s->set_framesize(s,(framesize_t)size);
}

uint32_t wc_get_width(void) {
  return wc_width;
}

uint32_t wc_get_height(void) {
  return wc_height;
}

#define MAX_PICSTORE 4
struct PICSTORE {
  uint8_t *buff;
  uint32_t len;
} picstore[MAX_PICSTORE];



uint32_t wc_get_frame(int32_t bnum) {
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;

  //return 0;
  if (bnum<0) {
    if (bnum<-MAX_PICSTORE) bnum=-1;
    bnum--;
    if (picstore[bnum].buff) free(picstore[bnum].buff);
    return 0;
  }

  wc_fb = esp_camera_fb_get();
  if (!wc_fb) return 0;
  uint32_t len=wc_fb->len;

  if (!bnum) {
    wc_width = wc_fb->width;
    wc_height = wc_fb->height;
    esp_camera_fb_return(wc_fb);
    return 0;
  }

  if (bnum<1 || bnum>MAX_PICSTORE) bnum=1;
  bnum--;
  if (picstore[bnum].buff) free(picstore[bnum].buff);
  picstore[bnum].buff = (uint8_t *)heap_caps_malloc(len+4,MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (picstore[bnum].buff) {
    if (wc_fb->format!=PIXFORMAT_JPEG) {
      bool jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
      if (!jpeg_converted){
          //Serial.println("JPEG compression failed");
          _jpg_buf_len = wc_fb->len;
          _jpg_buf = wc_fb->buf;
      }
    } else {
      _jpg_buf_len = wc_fb->len;
      _jpg_buf = wc_fb->buf;
    }
    memcpy(picstore[bnum].buff,_jpg_buf,_jpg_buf_len);
    picstore[bnum].len=_jpg_buf_len;
  } else {
    picstore[bnum].len=0;
  }
  esp_camera_fb_return(wc_fb);
  if (!picstore[bnum].buff) return 0;
  return  len;
}





/*
typedef enum {
    FRAMESIZE_96x96,    // 96x96
    FRAMESIZE_QQVGA,    // 160x120
    FRAMESIZE_QQVGA2,   // 128x160
    FRAMESIZE_QCIF,     // 176x144
    FRAMESIZE_HQVGA,    // 240x176
    FRAMESIZE_240x240,  // 240x240
    FRAMESIZE_QVGA,     // 320x240 6
    FRAMESIZE_CIF,      // 400x296 7
    FRAMESIZE_VGA,      // 640x480 8
    FRAMESIZE_SVGA,     // 800x600
    FRAMESIZE_XGA,      // 1024x768
    FRAMESIZE_SXGA,     // 1280x1024
    FRAMESIZE_UXGA,     // 1600x1200
    FRAMESIZE_QXGA,     // 2048*1536
    FRAMESIZE_INVALID
} framesize_t;

flash led = gpio4
red led = gpio 33
*/


#endif
