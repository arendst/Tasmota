#include "Adafruit_LvGL_Glue.h"
#include <lvgl.h>
#include "lv_berry.h"

// ARCHITECTURE-SPECIFIC TIMER STUFF ---------------------------------------

extern void lv_flush_callback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

// Tick interval for LittlevGL internal timekeeping; 1 to 10 ms recommended
static const int lv_tick_interval_ms = 5;

static void lv_tick_handler(void) { lv_tick_inc(lv_tick_interval_ms); }

// TOUCHSCREEN STUFF -------------------------------------------------------


uint32_t Touch_Status(int32_t sel);

static void touchscreen_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  data->point.x = Touch_Status(1); // Last-pressed coordinates
  data->point.y = Touch_Status(2);
  data->state = Touch_Status(0) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
  data->continue_reading = false; /*No buffering now so no more data read*/
  // keep data for TS calibration
  lv_ts_calibration.state = data->state;
  if (data->state == LV_INDEV_STATE_PRESSED) {    // if not pressed, the data may be invalid
    lv_ts_calibration.x = data->point.x;
    lv_ts_calibration.y = data->point.y;
    lv_ts_calibration.raw_x = Touch_Status(-1);
    lv_ts_calibration.raw_y = Touch_Status(-2);
  }
}

// OTHER LITTLEVGL VITALS --------------------------------------------------

#if LV_COLOR_DEPTH != 16
#pragma error("LV_COLOR_DEPTH must be 16")
#endif
// This isn't necessarily true, don't mention it for now. See notes later.
//#if LV_COLOR_16_SWAP != 0
//  #pragma message("Set LV_COLOR_16_SWAP to 0 for best display performance")
//#endif

// Actual RAM usage will be 2X these figures, since using 2 DMA buffers...
#define LV_BUFFER_ROWS 60 // Most others have a bit more space


// // This is the flush function required for LittlevGL screen updates.
// // It receives a bounding rect and an array of pixel data (conveniently
// // already in 565 format, so the Earth was lucky there).
// static void lv_flush_callback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
//   // Get pointer to glue object from indev user data
//   Adafruit_LvGL_Glue *glue = (Adafruit_LvGL_Glue *)disp->user_data;

//   uint16_t width = (area->x2 - area->x1 + 1);
//   uint16_t height = (area->y2 - area->y1 + 1);

//   // check if we are currently doing a screenshot
//   if (glue->getScreenshotFile() != nullptr) {
//     // save pixels to file
//     int32_t btw = (width * height * LV_COLOR_DEPTH + 7) / 8;
//     while (btw > 0) {
//       int32_t ret = glue->getScreenshotFile()->write((const uint8_t*) color_p, btw);
//       if (ret >= 0) {
//         btw -= ret;
//       } else {
//         btw = 0;  // abort
//       }
//     }
//     lv_disp_flush_ready(disp);
//     return; // ok
// //   }

//   Renderer *display = glue->display;

//   if (!glue->first_frame) {
//       //display->dmaWait();  // Wait for prior DMA transfer to complete
//       //display->endWrite(); // End transaction from any prior call
//   } else {
//       glue->first_frame = false;
//   }

//   display->setAddrWindow(area->x1, area->y1, area->x1+width, area->y1+height);
//   display->pushColors((uint16_t *)color_p, width * height, true);
//   display->setAddrWindow(0,0,0,0);

//   lv_disp_flush_ready(disp);

// }

// #if (LV_USE_LOG)
// // Optional LittlevGL debug print function, writes to Serial if debug is
// // enabled when calling glue begin() function.
// static void lv_debug(lv_log_level_t level, const char *file, uint32_t line, const char *fname,
//                      const char *dsc) {
//   Serial.print(file);
//   Serial.write('@');
//   Serial.print(line);
//   Serial.print(":");
//   Serial.print(fname);
//   Serial.write("->");
//   Serial.println(dsc);
// }
// #endif


// GLUE LIB FUNCTIONS ------------------------------------------------------

// Constructor
/**
 * @brief Construct a new Adafruit_LvGL_Glue::Adafruit_LvGL_Glue object,
 * initializing minimal variables
 *
 */
Adafruit_LvGL_Glue::Adafruit_LvGL_Glue(void)
    : first_frame(true), lv_pixel_buf(NULL) {
}

// Destructor
/**
 * @brief Destroy the Adafruit_LvGL_Glue::Adafruit_LvGL_Glue object, freeing any
 * memory previously allocated within this library.
 *
 */
Adafruit_LvGL_Glue::~Adafruit_LvGL_Glue(void) {
  delete[] lv_pixel_buf;
  // Probably other stuff that could be deallocated here
}

// begin() function is overloaded for STMPE610 touch, ADC touch, or none.

// Pass in POINTERS to ALREADY INITIALIZED display & touch objects (user code
// should have previously called corresponding begin() functions and checked
// return states before invoking this),
// they are NOT initialized here. Debug arg is
// touch arg can be NULL (or left off) if using LittlevGL as a passive widget
// display.

/**
 * @brief Configure the glue layer and the underlying LvGL code to use the given
 * TFT display driver instance and touchscreen controller
 *
 * @param tft Pointer to an **already initialized** display object instance
 * @param touch Pointer to an **already initialized** `Adafruit_STMPE610`
 * touchscreen controller object instance
 * @param debug Debug flag to enable debug messages. Only used if LV_USE_LOG is
 * configured in LittleLVGL's lv_conf.h
 * @return LvGLStatus The status of the initialization:
 * * LVGL_OK : Success
 * * LVGL_ERR_TIMER : Failure to set up timers
 * * LVGL_ERR_ALLOC : Failure to allocate memory
 */
// LvGLStatus Adafruit_LvGL_Glue::begin(uDisplay_lvgl *tft,
//                                      Adafruit_STMPE610 *touch, bool debug) {
//   is_adc_touch = false;
//   return begin(tft, (void *)touch, debug);
// }
/**
 * @brief Configure the glue layer and the underlying LvGL code to use the given
 * TFT display driver and touchscreen controller instances
 *
 * @param tft Pointer to an **already initialized** display object instance
 * @param touch Pointer to an **already initialized** `TouchScreen` touchscreen
 * controller object instance
 * @param debug Debug flag to enable debug messages. Only used if LV_USE_LOG is
 * configured in LittleLVGL's lv_conf.h
 * @return LvGLStatus The status of the initialization:
 * * LVGL_OK : Success
 * * LVGL_ERR_TIMER : Failure to set up timers
 * * LVGL_ERR_ALLOC : Failure to allocate memory
 */
// LvGLStatus Adafruit_LvGL_Glue::begin(uDisplay_lvgl *tft, TouchScreen *touch,
//                                      bool debug) {
//   is_adc_touch = true;
//   return begin(tft, (void *)touch, debug);
// }
/**
 * @brief Configure the glue layer and the underlying LvGL code to use the given
 * TFT display driver and touchscreen controller instances
 *
 * @param tft Pointer to an **already initialized** display object instance
 * @param debug Debug flag to enable debug messages. Only used if LV_USE_LOG is
 * configured in LittleLVGL's lv_conf.h
 * @return LvGLStatus The status of the initialization:
 * * LVGL_OK : Success
 * * LVGL_ERR_TIMER : Failure to set up timers
 * * LVGL_ERR_ALLOC : Failure to allocate memory
 */
LvGLStatus Adafruit_LvGL_Glue::begin(Renderer *tft, bool debug) {
  return begin(tft, (void *)NULL, debug);
}

LvGLStatus Adafruit_LvGL_Glue::begin(Renderer *tft, void *touch, bool debug) {

  lv_init();

  // Allocate LvGL display buffer (x2 because DMA double buffering)
  LvGLStatus status = LVGL_ERR_ALLOC;
  // if ((lv_pixel_buf = new lv_color_t[LV_HOR_RES_MAX * LV_BUFFER_ROWS * 2])) {

  uint32_t lvgl_buffer_size;

  //lvgl_buffer_size = LV_HOR_RES_MAX * LV_BUFFER_ROWS;
  uint8_t flushlines = tft->lvgl_pars()->fluslines;
  lvgl_buffer_size = tft->width() * (flushlines ? flushlines:LV_BUFFER_ROWS);
  if (tft->lvgl_pars()->use_dma) {
    lvgl_buffer_size /= 2;
    if (lvgl_buffer_size < 1000000) {
      lv_pixel_buf2 = new lv_color_t[lvgl_buffer_size];
    }
    if (!lv_pixel_buf2) {
      return status;
    }
  } else {
    lv_pixel_buf2 = nullptr;
  }

  if ((lv_pixel_buf = new lv_color_t[lvgl_buffer_size])) {

    display = tft;
    touchscreen = (void *)touch;

    // // Initialize LvGL display buffers
    // lv_disp_buf_init(
    //     &lv_disp_buf, lv_pixel_buf,                     // 1st half buf
    //     &lv_pixel_buf[LV_HOR_RES_MAX * LV_BUFFER_ROWS], // 2nd half buf
    //     LV_HOR_RES_MAX * LV_BUFFER_ROWS);

    // Initialize LvGL display buffers
    lv_disp_draw_buf_init(
        &lv_disp_buf, lv_pixel_buf,                     // 1st half buf
        lv_pixel_buf2, // 2nd half buf
        lvgl_buffer_size);

    // Initialize LvGL display driver
    lv_disp_drv_init(&lv_disp_drv);
    lv_disp_drv.hor_res = tft->width();
    lv_disp_drv.ver_res = tft->height();
    lv_disp_drv.flush_cb = lv_flush_callback;
    lv_disp_drv.draw_buf = &lv_disp_buf;
    lv_disp_drv.user_data = (void*)this;
    lv_disp_drv_register(&lv_disp_drv);

    // Initialize LvGL input device (touchscreen already started)
    if (touch) { // Can also pass NULL if passive widget display
      lv_indev_drv_init(&lv_indev_drv);          // Basic init
      lv_indev_drv.type = LV_INDEV_TYPE_POINTER; // Is pointer dev
      lv_indev_drv.read_cb = touchscreen_read;   // Read callback
      lv_indev_drv.user_data = (void*)this;
      lv_input_dev_ptr = lv_indev_drv_register(&lv_indev_drv);
    }

    // TIMER SETUP is architecture-specific ----------------------------

    // ESP 32------------------------------------------------
    tick.attach_ms(lv_tick_interval_ms, lv_tick_handler);
    status = LVGL_OK;
    // -----------------------------------------
  }

  if (status != LVGL_OK) {
    delete[] lv_pixel_buf;
    lv_pixel_buf = NULL;
  }

  return status;
}
