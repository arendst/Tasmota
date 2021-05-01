/*
  xdrv_54_lvgl.ino - LVLG integration

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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


#ifdef USE_LVGL

#include <renderer.h>
#include "lvgl.h"

#define XDRV_54             54

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "Adafruit_LvGL_Glue.h"


/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */

SemaphoreHandle_t xGuiSemaphore;
//uDisplay * udisp = nullptr;

// necessary for compilation
uint8_t color_type_lvgl = 0;
uint8_t * buffer_lvgl = nullptr;
void udisp_dimm_lvgl(uint8_t dim) {}
void udisp_bpwr_lvgl(uint8_t on) {}

extern "C" {

  const char task_name[] = "periodic_gui";
    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
      &lv_tick_task,
      nullptr,
      ESP_TIMER_TASK,
      task_name
    };
}

Adafruit_LvGL_Glue * glue;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void guiTask(void *pvParameter);

/************************************************************
 * Provide a regular tick to lvgl, every ms
 ************************************************************/

#ifndef LV_TICK_PERIOD_MS
#define LV_TICK_PERIOD_MS   1   // default to tick every 1 ms
#endif

static void lv_tick_task(void *arg) {
  (void) arg;
  lv_tick_inc(LV_TICK_PERIOD_MS);
}

// **************************************************
// Logging
// **************************************************
#if LV_USE_LOG
static void lvbe_debug(lv_log_level_t level, const char *file, uint32_t line, const char *fname, const char *msg);
static void lvbe_debug(lv_log_level_t level, const char *file, uint32_t line, const char *fname, const char *msg) {
  be_writebuffer("LVG: ", sizeof("LVG: "));
  be_writebuffer(fname, strlen(fname));
  be_writebuffer(" -> ", sizeof(" -> "));
  be_writebuffer(msg, strlen(msg));
  be_writebuffer("\n", sizeof("\n"));
}
#endif

/************************************************************
 * Maint FreeRTOS task used in a separate thread
 ************************************************************/
static void guiTask(void *pvParameter) {
  (void) pvParameter;
  xGuiSemaphore = xSemaphoreCreateMutex();

  /* Create and start a periodic timer interrupt to call lv_tick_inc */
  esp_timer_handle_t periodic_timer;
  ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

  while (1) {
      /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
      vTaskDelay(pdMS_TO_TICKS(10));

      /* Try to take the semaphore, call lvgl related function on success */
      if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
          lv_task_handler();
          xSemaphoreGive(xGuiSemaphore);
      }
  }

  /* A task should NEVER return */
  vTaskDelete(NULL);
}


/************************************************************
 * Main screen refresh function
 ************************************************************/
// This is the flush function required for LittlevGL screen updates.
// It receives a bounding rect and an array of pixel data (conveniently
// already in 565 format, so the Earth was lucky there).
void lv_flush_callback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void lv_flush_callback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  // Get pointer to glue object from indev user data
  Adafruit_LvGL_Glue *glue = (Adafruit_LvGL_Glue *)disp->user_data;

  uint16_t width = (area->x2 - area->x1 + 1);
  uint16_t height = (area->y2 - area->y1 + 1);

  // check if we are currently doing a screenshot
  if (glue->getScreenshotFile() != nullptr) {
    // save pixels to file
    int32_t btw = (width * height * LV_COLOR_DEPTH + 7) / 8;
    while (btw > 0) {
      int32_t ret = glue->getScreenshotFile()->write((const uint8_t*) color_p, btw);
      if (ret >= 0) {
        btw -= ret;
      } else {
        btw = 0;  // abort
      }
    }
    lv_disp_flush_ready(disp);
    return; // ok
  }

  Renderer *display = glue->display;

  if (!glue->first_frame) {
      //display->dmaWait();  // Wait for prior DMA transfer to complete
      //display->endWrite(); // End transaction from any prior call
  } else {
      glue->first_frame = false;
  }

  uint32_t pixels_len = width * height;
  uint32_t chrono_start = millis();
  display->setAddrWindow(area->x1, area->y1, area->x1+width, area->y1+height);
  display->pushColors((uint16_t *)color_p, pixels_len, false);
  display->setAddrWindow(0,0,0,0);
  uint32_t chrono_time = millis() - chrono_start;

  lv_disp_flush_ready(disp);

  if (pixels_len >= 10000 && (!display->lvgl_param.use_dma)) {
    AddLog(LOG_LEVEL_DEBUG, D_LOG_LVGL "Refreshed %d pixels in %d ms (%i pix/ms)", pixels_len, chrono_time,
            chrono_time > 0 ? pixels_len / chrono_time : -1);
  }
}

/************************************************************
 * Callbacks for file system access from LVGL
 *
 * Useful to load fonts or images from file system
 ************************************************************/

#ifdef USE_UFILESYS
static lv_fs_res_t lvbe_fs_open(lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t lvbe_fs_open(lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_open(%p, %p, %s, %i) %i", drv, file_p, path, mode, sizeof(File));
  const char * modes = nullptr;
  switch (mode) {
    case LV_FS_MODE_WR:
      modes = "w";
      break;
    case LV_FS_MODE_RD:
      modes = "r";
      break;
    case LV_FS_MODE_WR | LV_FS_MODE_RD:
      modes = "rw";
      break;
  }

  if (modes == nullptr) {
    AddLog(LOG_LEVEL_INFO, "LVG: fs_open, unsupported mode %d", mode);
    return LV_FS_RES_INV_PARAM;
  }

  // Add "/" prefix
  String file_path = "/";
  file_path += path;

  File f = dfsp->open(file_path.c_str(), modes);
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_open(%s) -> %i", file_path.c_str(), (int32_t)f);
  // AddLog(LOG_LEVEL_INFO, "LVG: F=%*_H", sizeof(f), &f);
  if (f) {
    File * f_ptr = new File(f);                 // copy to dynamic object
    *f_ptr = f;                                 // TODO is this necessary?
    *((File**)file_p) = f_ptr;
    return LV_FS_RES_OK;
  } else {
    return LV_FS_RES_NOT_EX;
  }
}

static lv_fs_res_t lvbe_fs_close(lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t lvbe_fs_close(lv_fs_drv_t * drv, void * file_p) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_close(%p, %p)", drv, file_p);
  File * f_ptr = (File*) file_p;
  f_ptr->close();
  delete f_ptr;
  return LV_FS_RES_OK;
}

static lv_fs_res_t lvbe_fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t lvbe_fs_read(lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_read(%p, %p, %p, %i, %p)", drv, file_p, buf, btr, br);
  File * f_ptr = (File*) file_p;
  // AddLog(LOG_LEVEL_INFO, "LVG: F=%*_H", sizeof(File), f_ptr);
  int32_t ret = f_ptr->read((uint8_t*) buf, btr);
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_read -> %i", ret);
  if (ret >= 0) {
    *br = ret;
    return LV_FS_RES_OK;
  } else {
    return LV_FS_RES_UNKNOWN;
  }
}

static lv_fs_res_t lvbe_fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t lvbe_fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_write(%p, %p, %p, %i, %p)", drv, file_p, buf, btw, bw);
  File * f_ptr = (File*) file_p;
  int32_t ret = f_ptr->write((const uint8_t*) buf, btw);
  if (ret >= 0) {
    *bw = ret;
    return LV_FS_RES_OK;
  } else {
    return LV_FS_RES_UNKNOWN;
  }
}

static lv_fs_res_t lvbe_fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);
static lv_fs_res_t lvbe_fs_tell(lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_tell(%p, %p, %p)", drv, file_p, pos_p);
  File * f_ptr = (File*) file_p;
  *pos_p = f_ptr->position();
  return LV_FS_RES_OK;
}

static lv_fs_res_t lvbe_fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos);
static lv_fs_res_t lvbe_fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_seek(%p, %p, %i)", drv, file_p, pos);
  File * f_ptr = (File*) file_p;
  if (f_ptr->seek(pos)) {
    return LV_FS_RES_OK;
  } else {
    return LV_FS_RES_UNKNOWN;
  }
}

static lv_fs_res_t lvbe_fs_size(lv_fs_drv_t * drv, void * file_p, uint32_t * size_p);
static lv_fs_res_t lvbe_fs_size(lv_fs_drv_t * drv, void * file_p, uint32_t * size_p) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_size(%p, %p, %p)", drv, file_p, size_p);
  File * f_ptr = (File*) file_p;
  *size_p = f_ptr->size();
  return LV_FS_RES_OK;
}

static lv_fs_res_t lvbe_fs_remove(lv_fs_drv_t * drv, const char *path);
static lv_fs_res_t lvbe_fs_remove(lv_fs_drv_t * drv, const char *path) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_remove(%p, %s)", drv, path);
  dfsp->remove(path);
  return LV_FS_RES_OK;
}
#endif // USE_UFILESYS

/************************************************************
 * Initialize the display / touchscreen drivers then launch lvgl
 *
 * We use Adafruit_LvGL_Glue to leverage the Adafruit
 * display ecosystem.
 ************************************************************/

Renderer *Init_uDisplay(const char *desc, int8_t cs);


void start_lvgl(const char * uconfig);
void start_lvgl(const char * uconfig) {

  if (glue != nullptr) {
    AddLog(LOG_LEVEL_INFO, D_LOG_LVGL " LVGL was already initialized");
    return;
  }

  if (!renderer || uconfig) {
#ifdef USE_UNIVERSAL_DISPLAY    // TODO - we will probably support only UNIV_DISPLAY
    renderer  = Init_uDisplay((char*)uconfig, -1);
    if (!renderer) return;
#else
    return;
#endif
  }

  renderer->DisplayOnff(true);

  // **************************************************
  // Initialize the glue between Adafruit and LVGL
  // **************************************************
  glue = new Adafruit_LvGL_Glue();

  // Initialize glue, passing in address of display & touchscreen
  LvGLStatus status = glue->begin(renderer, (void*)1, false);
  if (status != LVGL_OK) {
    AddLog(LOG_LEVEL_ERROR, PSTR("Glue error %d"), status);
    return;
  }

  // Set the default background color of the display
  // This is normally overriden by an opaque screen on top
#ifdef USE_BERRY
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_from_uint32(USE_LVGL_BG_DEFAULT));
  lv_obj_set_style_local_bg_opa(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
#endif

#if LV_USE_LOG
  lv_log_register_print_cb(lvbe_debug);
#endif // LV_USE_LOG

#ifdef USE_UFILESYS
  // Add file system mapping
  lv_fs_drv_t drv;      // can be on heap, LVGL creates a copy
  lv_fs_drv_init(&drv);                     /*Basic initialization*/

  drv.letter = 'A';                         /*An uppercase letter to identify the drive */
  drv.file_size = 0;           /*Size required to store a file object*/
  drv.rddir_size = 0;           /*Size required to store a directory object (used by dir_open/close/read)*/
  drv.ready_cb = nullptr;               /*Callback to tell if the drive is ready to use */
  drv.open_cb = &lvbe_fs_open;                 /*Callback to open a file */
  drv.close_cb = &lvbe_fs_close;               /*Callback to close a file */
  drv.read_cb = &lvbe_fs_read;                 /*Callback to read a file */
  drv.write_cb = &lvbe_fs_write;               /*Callback to write a file */
  drv.seek_cb = &lvbe_fs_seek;                 /*Callback to seek in a file (Move cursor) */
  drv.tell_cb = &lvbe_fs_tell;                 /*Callback to tell the cursor position  */
  drv.trunc_cb = nullptr;               /*Callback to delete a file */
  drv.size_cb = &lvbe_fs_size;                 /*Callback to tell a file's size */
  drv.rename_cb = nullptr;             /*Callback to rename a file */
  drv.remove_cb = &lvbe_fs_remove;             /*Callback to remove a file */

  drv.dir_open_cb = nullptr;         /*Callback to open directory to read its content */
  drv.dir_read_cb = nullptr;         /*Callback to read a directory's content */
  drv.dir_close_cb = nullptr;       /*Callback to close a directory */

  drv.free_space_cb = nullptr;     /*Callback to tell free space on the drive */
  // drv.user_data = nullptr;             /*Any custom data if required*/

  lv_fs_drv_register(&drv);                 /*Finally register the drive*/

#endif // USE_UFILESYS

  /* If you want to use a task to create the graphic, you NEED to create a Pinned task
    * Otherwise there can be problem such as memory corruption and so on.
    * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
  xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 0, NULL, 1);

  AddLog(LOG_LEVEL_INFO, PSTR("LVGL initialized"));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv54(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      break;
    case FUNC_LOOP:
      break;
    case FUNC_EVERY_50_MSECOND:
      break;
    case FUNC_EVERY_100_MSECOND:
      break;
    case FUNC_EVERY_SECOND:
      break;
    case FUNC_COMMAND:
    //   result = DecodeCommand(kBrCommands, BerryCommand);
      break;
    case FUNC_RULES_PROCESS:
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      break;
    case FUNC_MQTT_DATA:
      break;
    case FUNC_WEB_SENSOR:
      break;

    case FUNC_JSON_APPEND:
      break;

    case FUNC_BUTTON_PRESSED:
      break;

  }
  return result;
}

#endif  // USE_LVGL
