#ifndef _ADAFRUIT_LVGL_GLUE_H_
#define _ADAFRUIT_LVGL_GLUE_H_

#include <lvgl.h>              // LittlevGL core lib
#include <renderer.h>
#include <Ticker.h> // ESP32-specific timer lib
#include <FS.h>

typedef enum {
  LVGL_OK,
  LVGL_ERR_ALLOC,
  LVGL_ERR_TIMER,
} LvGLStatus;

/**
 * @brief Class to act as a "glue" layer between the LvGL graphics library and
 * most of Adafruit's TFT displays
 *
 */

class Adafruit_LvGL_Glue {
public:
  Adafruit_LvGL_Glue(void);
  ~Adafruit_LvGL_Glue(void);
  // Different begin() funcs for STMPE610, ADC or no touch
  // LvGLStatus begin(uDisplay_lvgl *tft, Adafruit_STMPE610 *touch,
  //                  bool debug = false);
  // LvGLStatus begin(uDisplay_lvgl *tft, TouchScreen *touch,
  //                  bool debug = false);
  LvGLStatus begin(Renderer *tft, bool debug = false);
  LvGLStatus begin(Renderer *tft, void *touch, bool debug);
  // These items need to be public for some internal callbacks,
  // but should be avoided by user code please!
  Renderer *display; ///< Pointer to the SPITFT display instance
  void *touchscreen;        ///< Pointer to the touchscreen object to use
  bool is_adc_touch; ///< determines if the touchscreen controlelr is ADC based
  bool first_frame;  ///< Tracks if a call to `lv_flush_callback` needs to wait
                     ///< for DMA transfer to complete
  void setScreenshotFile(File *f) { screenshot = f; }
  File * getScreenshotFile(void) const { return screenshot; }
  void stopScreenshot(void) { screenshot = nullptr; }

private:
  lv_disp_drv_t lv_disp_drv;
  lv_disp_buf_t lv_disp_buf;
  lv_color_t *lv_pixel_buf;
  lv_color_t *lv_pixel_buf2;
  lv_indev_drv_t lv_indev_drv;
  lv_indev_t *lv_input_dev_ptr;
  Ticker tick;
  File * screenshot = nullptr;
};

#endif // _ADAFRUIT_LVGL_GLUE_H_
