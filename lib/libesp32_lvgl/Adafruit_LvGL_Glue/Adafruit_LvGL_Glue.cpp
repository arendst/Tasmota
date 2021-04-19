#include "Adafruit_LvGL_Glue.h"
#include <lvgl.h>

// ARCHITECTURE-SPECIFIC TIMER STUFF ---------------------------------------

// Tick interval for LittlevGL internal timekeeping; 1 to 10 ms recommended
static const int lv_tick_interval_ms = 10;

#if defined(ARDUINO_ARCH_SAMD) // --------------------------------------

// Because of the way timer/counters are paired, and because parallel TFT
// uses timer 2 for write strobe, this needs to use timer 4 or above...
#define TIMER_NUM 4
#define TIMER_ISR TC4_Handler

// Interrupt service routine for zerotimer object
void TIMER_ISR(void) { Adafruit_ZeroTimer::timerHandler(TIMER_NUM); }

// Timer compare match 0 callback -- invokes LittlevGL timekeeper.
static void timerCallback0(void) { lv_tick_inc(lv_tick_interval_ms); }

#elif defined(ESP32) // ------------------------------------------------

static void lv_tick_handler(void) { lv_tick_inc(lv_tick_interval_ms); }

#elif defined(NRF52_SERIES) // -----------------------------------------

#define TIMER_ID NRF_TIMER4
#define TIMER_IRQN TIMER4_IRQn
#define TIMER_ISR TIMER4_IRQHandler
#define TIMER_FREQ 16000000

extern "C" {
// Timer interrupt service routine
void TIMER_ISR(void) {
  if (TIMER_ID->EVENTS_COMPARE[0]) {
    TIMER_ID->EVENTS_COMPARE[0] = 0;
  }
  lv_tick_inc(lv_tick_interval_ms);
}
}

#endif

// TOUCHSCREEN STUFF -------------------------------------------------------

// STMPE610 calibration for raw touch data
#define TS_MINX 100
#define TS_MAXX 3800
#define TS_MINY 100
#define TS_MAXY 3750

// Same, for ADC touchscreen
#define ADC_XMIN 325
#define ADC_XMAX 750
#define ADC_YMIN 240
#define ADC_YMAX 840

static bool touchscreen_read(struct _lv_indev_drv_t *indev_drv,
                             lv_indev_data_t *data) {
  static lv_coord_t last_x = 0, last_y = 0;
  static uint8_t release_count = 0;

  // Get pointer to glue object from indev user data
  Adafruit_LvGL_Glue *glue = (Adafruit_LvGL_Glue *)indev_drv->user_data;
  uDisplay_lvgl *disp = glue->display;

  if (glue->is_adc_touch) {
    TouchScreen *touch = (TouchScreen *)glue->touchscreen;
    TSPoint p = touch->getPoint();
    // Serial.printf("%d %d %d\r\n", p.x, p.y, p.z);
    // Having an issue with spurious z=0 results from TouchScreen lib.
    // Since touch is polled periodically, workaround is to watch for
    // several successive z=0 results, and only then regard it as
    // a release event (otherwise still touched).
    if (p.z < touch->pressureThreshhold) { // A zero-ish value
      release_count += (release_count < 255);
      if (release_count >= 4) {
        data->state = LV_INDEV_STATE_REL; // Is REALLY RELEASED
      } else {
        data->state = LV_INDEV_STATE_PR; // Is STILL PRESSED
      }
    } else {
      release_count = 0;               // Reset release counter
      data->state = LV_INDEV_STATE_PR; // Is PRESSED
      switch (glue->display->getRotation()) {
      case 0:
        last_x = map(p.x, ADC_XMIN, ADC_XMAX, 0, disp->width() - 1);
        last_y = map(p.y, ADC_YMAX, ADC_YMIN, 0, disp->height() - 1);
        break;
      case 1:
        last_x = map(p.y, ADC_YMAX, ADC_YMIN, 0, disp->width() - 1);
        last_y = map(p.x, ADC_XMAX, ADC_XMIN, 0, disp->height() - 1);
        break;
      case 2:
        last_x = map(p.x, ADC_XMAX, ADC_XMIN, 0, disp->width() - 1);
        last_y = map(p.y, ADC_YMIN, ADC_YMAX, 0, disp->height() - 1);
        break;
      case 3:
        last_x = map(p.y, ADC_YMIN, ADC_YMAX, 0, disp->width() - 1);
        last_y = map(p.x, ADC_XMIN, ADC_XMAX, 0, disp->height() - 1);
        break;
      }
    }
    data->point.x = last_x; // Last-pressed coordinates
    data->point.y = last_y;
    return false; // No buffering of ADC touch data
  } else {
    uint8_t fifo; // Number of points in touchscreen FIFO
    bool moar = false;
    Adafruit_STMPE610 *touch = (Adafruit_STMPE610 *)glue->touchscreen;
    // Before accessing SPI touchscreen, wait on any in-progress
    // DMA screen transfer to finish (shared bus).
    //disp->dmaWait();
    disp->endWrite();
    if ((fifo = touch->bufferSize())) { // 1 or more points await
      data->state = LV_INDEV_STATE_PR;  // Is PRESSED
      TS_Point p = touch->getPoint();
      // Serial.printf("%d %d %d\r\n", p.x, p.y, p.z);
      // On big TFT FeatherWing, raw X axis is flipped??
      if ((glue->display->width() == 480) || (glue->display->height() == 480)) {
        p.x = (TS_MINX + TS_MAXX) - p.x;
      }
      switch (glue->display->getRotation()) {
      case 0:
        last_x = map(p.x, TS_MAXX, TS_MINX, 0, disp->width() - 1);
        last_y = map(p.y, TS_MINY, TS_MAXY, 0, disp->height() - 1);
        break;
      case 1:
        last_x = map(p.y, TS_MINY, TS_MAXY, 0, disp->width() - 1);
        last_y = map(p.x, TS_MINX, TS_MAXX, 0, disp->height() - 1);
        break;
      case 2:
        last_x = map(p.x, TS_MINX, TS_MAXX, 0, disp->width() - 1);
        last_y = map(p.y, TS_MAXY, TS_MINY, 0, disp->height() - 1);
        break;
      case 3:
        last_x = map(p.y, TS_MAXY, TS_MINY, 0, disp->width() - 1);
        last_y = map(p.x, TS_MAXX, TS_MINX, 0, disp->height() - 1);
        break;
      }
      moar = (fifo > 1); // true if more in FIFO, false if last point
#if defined(NRF52_SERIES)
      // Not sure what's up here, but nRF doesn't seem to always poll
      // the FIFO size correctly, causing false release events. If it
      // looks like we've read the last point from the FIFO, pause
      // briefly to allow any more FIFO events to pile up. This
      // doesn't seem to be necessary on SAMD or ESP32. ???
      if (!moar) {
        delay(50);
      }
#endif
    } else {                            // FIFO empty
      data->state = LV_INDEV_STATE_REL; // Is RELEASED
    }

    data->point.x = last_x; // Last-pressed coordinates
    data->point.y = last_y;
    return moar;
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

// This is the flush function required for LittlevGL screen updates.
// It receives a bounding rect and an array of pixel data (conveniently
// already in 565 format, so the Earth was lucky there).
static void lv_flush_callback(lv_disp_drv_t *disp, const lv_area_t *area,
                              lv_color_t *color_p) {
  // Get pointer to glue object from indev user data
  Adafruit_LvGL_Glue *glue = (Adafruit_LvGL_Glue *)disp->user_data;
  uDisplay_lvgl *display = glue->display;

  if (!glue->first_frame) {
    //display->dmaWait();  // Wait for prior DMA transfer to complete
    display->endWrite(); // End transaction from any prior call
  } else {
    glue->first_frame = false;
  }

  uint16_t width = (area->x2 - area->x1 + 1);
  uint16_t height = (area->y2 - area->y1 + 1);
  // display->startWrite();
  // display->setAddrWindow(area->x1, area->y1, width, height);
  display->writePixels(area->x1, area->y1, width, height,
                       (uint16_t *)color_p, width * height);
  // display->pushColors((uint16_t *)color_p, width * height, false);

  lv_disp_flush_ready(disp);
}

#if (LV_USE_LOG)
// Optional LittlevGL debug print function, writes to Serial if debug is
// enabled when calling glue begin() function.
static void lv_debug(lv_log_level_t level, const char *file, uint32_t line, const char *fname,
                     const char *dsc) {
  Serial.print(file);
  Serial.write('@');
  Serial.print(line);
  Serial.print(":");
  Serial.print(fname);
  Serial.write("->");
  Serial.println(dsc);
}
#endif

// GLUE LIB FUNCTIONS ------------------------------------------------------

// Constructor
/**
 * @brief Construct a new Adafruit_LvGL_Glue::Adafruit_LvGL_Glue object,
 * initializing minimal variables
 *
 */
Adafruit_LvGL_Glue::Adafruit_LvGL_Glue(void)
    : first_frame(true), lv_pixel_buf(NULL) {
#if defined(ARDUINO_ARCH_SAMD)
  zerotimer = NULL;
#endif
}

// Destructor
/**
 * @brief Destroy the Adafruit_LvGL_Glue::Adafruit_LvGL_Glue object, freeing any
 * memory previously allocated within this library.
 *
 */
Adafruit_LvGL_Glue::~Adafruit_LvGL_Glue(void) {
  delete[] lv_pixel_buf;
#if defined(ARDUINO_ARCH_SAMD)
  delete zerotimer;
#endif
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
LvGLStatus Adafruit_LvGL_Glue::begin(uDisplay_lvgl *tft,
                                     Adafruit_STMPE610 *touch, bool debug) {
  is_adc_touch = false;
  return begin(tft, (void *)touch, debug);
}
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
LvGLStatus Adafruit_LvGL_Glue::begin(uDisplay_lvgl *tft, TouchScreen *touch,
                                     bool debug) {
  is_adc_touch = true;
  return begin(tft, (void *)touch, debug);
}
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
LvGLStatus Adafruit_LvGL_Glue::begin(uDisplay_lvgl *tft, bool debug) {
  return begin(tft, (void *)NULL, debug);
}

LvGLStatus Adafruit_LvGL_Glue::begin(uDisplay_lvgl *tft, void *touch,
                                     bool debug) {

  lv_init();
// #if (LV_USE_LOG)
//   if (debug) {
//     lv_log_register_print_cb(lv_debug); // Register debug print function
//   }
// #endif

  // Allocate LvGL display buffer (x2 because DMA double buffering)
  LvGLStatus status = LVGL_ERR_ALLOC;
  // if ((lv_pixel_buf = new lv_color_t[LV_HOR_RES_MAX * LV_BUFFER_ROWS * 2])) {
  if ((lv_pixel_buf = new lv_color_t[LV_HOR_RES_MAX * LV_BUFFER_ROWS])) {

    display = tft;
    touchscreen = (void *)touch;

    // // Initialize LvGL display buffers
    // lv_disp_buf_init(
    //     &lv_disp_buf, lv_pixel_buf,                     // 1st half buf
    //     &lv_pixel_buf[LV_HOR_RES_MAX * LV_BUFFER_ROWS], // 2nd half buf
    //     LV_HOR_RES_MAX * LV_BUFFER_ROWS);

    // Initialize LvGL display buffers
    lv_disp_buf_init(
        &lv_disp_buf, lv_pixel_buf,                     // 1st half buf
        nullptr, // 2nd half buf
        LV_HOR_RES_MAX * LV_BUFFER_ROWS);

    // Initialize LvGL display driver
    lv_disp_drv_init(&lv_disp_drv);
    lv_disp_drv.hor_res = tft->width();
    lv_disp_drv.ver_res = tft->height();
    lv_disp_drv.flush_cb = lv_flush_callback;
    lv_disp_drv.buffer = &lv_disp_buf;
    lv_disp_drv.user_data = (lv_disp_drv_user_data_t)this;
    lv_disp_drv_register(&lv_disp_drv);

    // Initialize LvGL input device (touchscreen already started)
    if ((touch)) { // Can also pass NULL if passive widget display
      lv_indev_drv_init(&lv_indev_drv);          // Basic init
      lv_indev_drv.type = LV_INDEV_TYPE_POINTER; // Is pointer dev
      lv_indev_drv.read_cb = touchscreen_read;   // Read callback
      lv_indev_drv.user_data = (lv_indev_drv_user_data_t)this;
      lv_input_dev_ptr = lv_indev_drv_register(&lv_indev_drv);
    }

    // TIMER SETUP is architecture-specific ----------------------------

#if defined(ARDUINO_ARCH_SAMD) // --------------------------------------

    // status is still ERR_ALLOC until proven otherwise...
    if ((zerotimer = new Adafruit_ZeroTimer(TIMER_NUM))) {
      uint8_t divider = 1;
      uint16_t compare = 0;
      tc_clock_prescaler prescaler = TC_CLOCK_PRESCALER_DIV1;

      status = LVGL_OK; // We're prob good now, but one more test...

      int freq = 1000 / lv_tick_interval_ms;

      if ((freq < (48000000 / 2)) && (freq > (48000000 / 65536))) {
        divider = 1;
        prescaler = TC_CLOCK_PRESCALER_DIV1;
      } else if (freq > (48000000 / 65536 / 2)) {
        divider = 2;
        prescaler = TC_CLOCK_PRESCALER_DIV2;
      } else if (freq > (48000000 / 65536 / 4)) {
        divider = 4;
        prescaler = TC_CLOCK_PRESCALER_DIV4;
      } else if (freq > (48000000 / 65536 / 8)) {
        divider = 8;
        prescaler = TC_CLOCK_PRESCALER_DIV8;
      } else if (freq > (48000000 / 65536 / 16)) {
        divider = 16;
        prescaler = TC_CLOCK_PRESCALER_DIV16;
      } else if (freq > (48000000 / 65536 / 64)) {
        divider = 64;
        prescaler = TC_CLOCK_PRESCALER_DIV64;
      } else if (freq > (48000000 / 65536 / 256)) {
        divider = 256;
        prescaler = TC_CLOCK_PRESCALER_DIV256;
      } else {
        status = LVGL_ERR_TIMER; // Invalid frequency
      }

      if (status == LVGL_OK) {
        compare = (48000000 / divider) / freq;
        // Initialize timer
        zerotimer->configure(prescaler, TC_COUNTER_SIZE_16BIT,
                             TC_WAVE_GENERATION_MATCH_PWM);
        zerotimer->setCompare(0, compare);
        zerotimer->setCallback(true, TC_CALLBACK_CC_CHANNEL0, timerCallback0);
        zerotimer->enable(true);
      }
    }

#elif defined(ESP32) // ------------------------------------------------

    tick.attach_ms(lv_tick_interval_ms, lv_tick_handler);
    status = LVGL_OK;

#elif defined(NRF52_SERIES) // -----------------------------------------

    TIMER_ID->TASKS_STOP = 1;               // Stop timer
    TIMER_ID->MODE = TIMER_MODE_MODE_Timer; // Not counter mode
    TIMER_ID->TASKS_CLEAR = 1;
    TIMER_ID->BITMODE = TIMER_BITMODE_BITMODE_16Bit
                        << TIMER_BITMODE_BITMODE_Pos;
    TIMER_ID->PRESCALER = 0; // 1:1 prescale (16 MHz)
    TIMER_ID->INTENSET = TIMER_INTENSET_COMPARE0_Enabled
                         << TIMER_INTENSET_COMPARE0_Pos; // Event 0 int
    TIMER_ID->CC[0] = TIMER_FREQ / (lv_tick_interval_ms * 1000);

    NVIC_DisableIRQ(TIMER_IRQN);
    NVIC_ClearPendingIRQ(TIMER_IRQN);
    NVIC_SetPriority(TIMER_IRQN, 2); // Lower priority than soft device
    NVIC_EnableIRQ(TIMER_IRQN);

    TIMER_ID->TASKS_START = 1; // Start timer

    status = LVGL_OK;

#endif // end timer setup --------------------------------------------------
  }

  if (status != LVGL_OK) {
    delete[] lv_pixel_buf;
    lv_pixel_buf = NULL;
#if defined(ARDUINO_ARCH_SAMD)
    delete zerotimer;
    zerotimer = NULL;
#endif
  }

  return status;
}
