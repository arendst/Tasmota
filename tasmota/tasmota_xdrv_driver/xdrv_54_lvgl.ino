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

#ifdef ESP32
#if defined(USE_LVGL) && defined(USE_UNIVERSAL_DISPLAY)

#include <renderer.h>
#include "lvgl.h"
#include "core/lv_global.h"         // needed for LV_GLOBAL_DEFAULT
#include "tasmota_lvgl_assets.h"    // force compilation of assets

#define XDRV_54             54

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

struct LVGL_Glue {
  lv_display_t *lv_display = nullptr;
  lv_indev_t *lv_indev = nullptr;
  void *lv_pixel_buf = nullptr;
  void *lv_pixel_buf2 = nullptr;
  Ticker tick;
  File * screenshot = nullptr;
};
LVGL_Glue * lvgl_glue;

// **************************************************
// Logging
// **************************************************
#if LV_USE_LOG
#ifdef USE_BERRY
static void lvbe_debug(lv_log_level_t, const char *msg);
static void lvbe_debug(lv_log_level_t, const char *msg) {
  be_writebuffer("LVG: ", sizeof("LVG: "));
  be_writebuffer(msg, strlen(msg));
}
#endif
#endif

/************************************************************
 * Main screen refresh function
 ************************************************************/
// This is the flush function required for LittlevGL screen updates.
// It receives a bounding rect and an array of pixel data (conveniently
// already in 565 format, so the Earth was lucky there).
void lv_flush_callback(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p);
void lv_flush_callback(lv_display_t *disp, const lv_area_t *area, uint8_t *color_p) {
  uint16_t width = (area->x2 - area->x1 + 1);
  uint16_t height = (area->y2 - area->y1 + 1);

  // check if we are currently doing a screenshot
  if (lvgl_glue->screenshot != nullptr) {
    // save pixels to file
    int32_t btw = (width * height * LV_COLOR_DEPTH + 7) / 8;
    yield();            // ensure WDT does not fire
    while (btw > 0) {
      if (btw > 0) {    // if we had a previous error (ex disk full) don't try to write anymore
        int32_t ret = lvgl_glue->screenshot->write((const uint8_t*) color_p, btw);
        if (ret >= 0) {
          btw -= ret;
        } else {
          btw = 0;  // abort
        }
      }
    }
    lv_disp_flush_ready(disp);
    return; // ok
  }

  uint32_t pixels_len = width * height;
  uint32_t chrono_start = millis();
  renderer->setAddrWindow(area->x1, area->y1, area->x1+width, area->y1+height);
  renderer->pushColors((uint16_t *)color_p, pixels_len, true);
  renderer->setAddrWindow(0,0,0,0);
  renderer->Updateframe();
  uint32_t chrono_time = millis() - chrono_start;

  lv_disp_flush_ready(disp);

  if (pixels_len >= 10000 && (!renderer->lvgl_param.use_dma)) {
    if (HighestLogLevel() >= LOG_LEVEL_DEBUG_MORE) {
      AddLog(LOG_LEVEL_DEBUG_MORE, D_LOG_LVGL "Refreshed %d pixels in %d ms (%i pix/ms)", pixels_len, chrono_time,
              chrono_time > 0 ? pixels_len / chrono_time : -1);
    }
  }
}


/************************************************************
 * Emulation of stdio for FreeType
 *
 ************************************************************/

#ifdef USE_UFILESYS

#include <FS.h>
#include "ZipReadFS.h"
extern FS *ffsp;
extern FS *ufsp;
FS lv_zip_ufsp(ZipReadFSImplPtr(new ZipReadFSImpl(&ffsp, "/sd/", &ufsp)));

extern "C" {

  typedef void lvbe_FILE;

  // FILE * fopen ( const char * filename, const char * mode );
  lvbe_FILE * lvbe_fopen(const char * filename, const char * mode ) {

    // Add "/" prefix
    String file_path = "/";
    file_path += filename;

    File f = lv_zip_ufsp.open(file_path, mode);
    // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fopen(%s) -> %i", file_path.c_str(), (int32_t)f);
    // AddLog(LOG_LEVEL_INFO, "LVG: F=%*_H", sizeof(f), &f);
    if (f) {
      File * f_ptr = new File(f);                 // copy to dynamic object
      *f_ptr = f;                                 // TODO is this necessary?
      return f_ptr;
    }
    return nullptr;
  }

  // int fclose ( FILE * stream );
  lv_fs_res_t lvbe_fclose(lvbe_FILE * stream) {
    File * f_ptr = (File*) stream;
    f_ptr->close();
    delete f_ptr;
    // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fclose(%p)", f_ptr);
    return LV_FS_RES_OK;
  }

  // size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
  size_t lvbe_fread(void * ptr, size_t size, size_t count, lvbe_FILE * stream) {
    File * f_ptr = (File*) stream;
    // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fread (%p, %i, %i, %p)", ptr, size, count, f_ptr);

    int32_t ret = f_ptr->read((uint8_t*) ptr, size * count);
    // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fread -> %i", ret);
    if (ret < 0) {    // error
      ret = 0;
    }
    return ret;
  }

  // int fseek ( FILE * stream, long int offset, int origin );
  int lvbe_fseek(lvbe_FILE * stream, long int offset, int origin ) {
    File * f_ptr = (File*) stream;
    // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fseek(%p, %i, %i)", f_ptr, offset, origin);
    
    fs::SeekMode mode = fs::SeekMode::SeekSet;
    if (SEEK_CUR == origin) {
      mode = fs::SeekMode::SeekCur;
    } else if (SEEK_END == origin) {
      mode = fs::SeekMode::SeekEnd;
    }
    bool ok = f_ptr->seek(offset, mode);
    return ok ? 0 : -1;
  }

  // long int ftell ( FILE * stream );
  int lvbe_ftell(lvbe_FILE * stream) {
    File * f_ptr = (File*) stream;
    // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_ftell(%p) -> %i", f_ptr, f_ptr->position());
    return f_ptr->position();
  }

}
#endif // USE_UFILESYS

/************************************************************
 * Callbacks for file system access from LVGL
 *
 * Useful to load fonts or images from file system
 ************************************************************/

#ifdef USE_UFILESYS
static void * lvbe_fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode);
static void * lvbe_fs_open(lv_fs_drv_t * drv, const char * path, lv_fs_mode_t mode) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_open(%p, %p, %s, %i) %i", drv, file_p, path, mode, sizeof(File));
  const char * modes = nullptr;
  switch (mode) {
    case LV_FS_MODE_WR:                   modes = "w";    break;
    case LV_FS_MODE_RD:                   modes = "r";    break;
    case LV_FS_MODE_WR | LV_FS_MODE_RD:   modes = "rw";   break;
  }

  if (modes == nullptr) {
    AddLog(LOG_LEVEL_INFO, "LVG: fs_open, unsupported mode %d", mode);
    return nullptr;
  }

  return (void*) lvbe_fopen(path, modes);
}

static lv_fs_res_t lvbe_fs_close(lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t lvbe_fs_close(lv_fs_drv_t * drv, void * file_p) {
  return lvbe_fclose((void*)file_p);
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

static lv_fs_res_t lvbe_fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence);
static lv_fs_res_t lvbe_fs_seek(lv_fs_drv_t * drv, void * file_p, uint32_t pos, lv_fs_whence_t whence) {
  // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fs_seek(%p, %p, %i)", drv, file_p, pos);
  File * f_ptr = (File*) file_p;
  SeekMode seek;
  switch (whence) {
    case LV_FS_SEEK_SET: seek = SeekSet; break;
    case LV_FS_SEEK_CUR: seek = SeekCur; break;
    case LV_FS_SEEK_END: seek = SeekEnd; break;
    default: return LV_FS_RES_UNKNOWN;
  }

  if (f_ptr->seek(pos, seek)) {
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
#endif // USE_UFILESYS

/*********************************************************************************************\
 * Memory handler
 * Use PSRAM if available
\*********************************************************************************************/
extern "C" {
  /*
  Use the following

  extern void *lvbe_malloc(size_t size);
  extern void  lvbe_free(void *ptr);
  extern void *lvbe_realloc(void *ptr, size_t size);
  extern void *lvbe_calloc(size_t num, size_t size);
  */
  void *lvbe_malloc(uint32_t size);
  void *lvbe_realloc(void *ptr, size_t size);
  void *lvbe_calloc(size_t num, size_t size);
#ifdef USE_BERRY_PSRAM
  void *lvbe_malloc(uint32_t size) {
    return special_malloc(size);
  }
  void *lvbe_realloc(void *ptr, size_t size) {
    return special_realloc(ptr, size);
  }
  void *lvbe_calloc(size_t num, size_t size) {
    return special_calloc(num, size);
  }
#else // USE_BERRY_PSRAM
  void *lvbe_malloc(uint32_t size) {
    return malloc(size);
  }
  void *lvbe_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
  }
  void *lvbe_calloc(size_t num, size_t size) {
    return calloc(num, size);
  }
#endif // USE_BERRY_PSRAM

  void lvbe_free(void *ptr) {
    free(ptr);
  }

#ifdef USE_LVGL_PNG_DECODER
  // for PNG decoder, use same allocators as LVGL
  void* lodepng_malloc(size_t size) { return lvbe_malloc(size); }
  void* lodepng_realloc(void* ptr, size_t new_size) { return lvbe_realloc(ptr, new_size); }
  void lodepng_free(void* ptr) { lvbe_free(ptr); }
#endif // USE_LVGL_PNG_DECODER

}

// ARCHITECTURE-SPECIFIC TIMER STUFF ---------------------------------------

extern void lv_flush_callback(lv_display_t *disp, const lv_area_t *area, uint8_t * px_map);

// Tick interval for LittlevGL internal timekeeping; 1 to 10 ms recommended
static const int lv_tick_interval_ms = 5;

static void lv_tick_handler(void) { lv_tick_inc(lv_tick_interval_ms); }

// TOUCHSCREEN STUFF -------------------------------------------------------

uint32_t Touch_Status(int32_t sel);

//typedef void (*lv_indev_read_cb_t)(lv_indev_t * indev, lv_indev_data_t * data);
void lvgl_touchscreen_read(lv_indev_t *indev_drv, lv_indev_data_t *data);
void lvgl_touchscreen_read(lv_indev_t *indev_drv, lv_indev_data_t *data) {
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

// Actual RAM usage will be 2X these figures, since using 2 DMA buffers...
#define LV_BUFFER_ROWS 60 // Most others have a bit more space

/************************************************************
 * Initialize the display / touchscreen drivers then launch lvgl
 *
 * We use our own simplified mapping on top of Universal display
 ************************************************************/
extern Renderer *Init_uDisplay(const char *desc);

void start_lvgl(const char * uconfig);
void start_lvgl(const char * uconfig) {

  if (lvgl_glue != nullptr) {
    AddLog(LOG_LEVEL_DEBUG_MORE, D_LOG_LVGL "LVGL was already initialized");
    return;
  }

  if (!renderer || uconfig) {
    renderer  = Init_uDisplay((char*)uconfig);
    AddLog(LOG_LEVEL_ERROR, "LVG: Could not start Universal Display");
    if (!renderer) return;
  }

  renderer->DisplayOnff(true);

  // **************************************************
  // Initialize LVGL
  // **************************************************
  lvgl_glue = new LVGL_Glue;

  // Initialize lvgl_glue, passing in address of display & touchscreen
  lv_init();

  // Allocate LvGL display buffer (x2 because DMA double buffering)
  bool status_ok = true;
  size_t lvgl_buffer_size;
  do {
    uint32_t flushlines = renderer->lvgl_pars()->flushlines;
    if (0 == flushlines) flushlines = LV_BUFFER_ROWS;

    lvgl_buffer_size = renderer->width() * flushlines;
    if (renderer->lvgl_pars()->use_dma) {
      lvgl_buffer_size /= 2;
      if (lvgl_buffer_size < 1000000) {
        // allocate preferably in internal memory which is faster than PSRAM
        AddLog(LOG_LEVEL_DEBUG, "LVG: Allocating buffer2 %i bytes in main memory (flushlines %i)", (lvgl_buffer_size * (LV_COLOR_DEPTH / 8)) / 1024, flushlines);
        lvgl_glue->lv_pixel_buf2 = heap_caps_malloc_prefer(lvgl_buffer_size * (LV_COLOR_DEPTH / 8), 2, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL, MALLOC_CAP_8BIT);
      }
      if (!lvgl_glue->lv_pixel_buf2) {
        status_ok = false;
        break;
      }
    }

    // allocate preferably in internal memory which is faster than PSRAM
    AddLog(LOG_LEVEL_DEBUG, "LVG: Allocating buffer1 %i KB in main memory (flushlines %i)", (lvgl_buffer_size * (LV_COLOR_DEPTH / 8)) / 1024, flushlines);
    lvgl_glue->lv_pixel_buf = heap_caps_malloc_prefer(lvgl_buffer_size * (LV_COLOR_DEPTH / 8), 2, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL, MALLOC_CAP_8BIT);
    if (!lvgl_glue->lv_pixel_buf) {
      status_ok = false;
      break;
    }
  } while (0);

  if (!status_ok) {
    if (lvgl_glue->lv_pixel_buf) {
      free(lvgl_glue->lv_pixel_buf);
      lvgl_glue->lv_pixel_buf = NULL;
    }
    if (lvgl_glue->lv_pixel_buf2) {
      free(lvgl_glue->lv_pixel_buf2);
      lvgl_glue->lv_pixel_buf2 = NULL;
    }
    delete lvgl_glue;
    lvgl_glue = nullptr;
    AddLog(LOG_LEVEL_ERROR, "LVG: Could not allocate buffers");
    return;
  }

  // Initialize LvGL display driver
  lvgl_glue->lv_display = lv_display_create(renderer->width(), renderer->height());
  lv_display_set_flush_cb(lvgl_glue->lv_display, lv_flush_callback);
  lv_display_set_buffers(lvgl_glue->lv_display, lvgl_glue->lv_pixel_buf, lvgl_glue->lv_pixel_buf2, lvgl_buffer_size * (LV_COLOR_DEPTH / 8), LV_DISPLAY_RENDER_MODE_PARTIAL);

  // Initialize LvGL input device (touchscreen already started)
  lvgl_glue->lv_indev = lv_indev_create();
  lv_indev_set_type(lvgl_glue->lv_indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(lvgl_glue->lv_indev, lvgl_touchscreen_read);

  // ESP 32------------------------------------------------
  lvgl_glue->tick.attach_ms(lv_tick_interval_ms, lv_tick_handler);
  // -----------------------------------------

  // Set the default background color of the display
  // This is normally overriden by an opaque screen on top
#ifdef USE_BERRY
  // By default set the display color to black and opacity to 100%
  lv_obj_t * background = lv_layer_bottom();
  lv_obj_set_style_bg_color(background, lv_color_hex(USE_LVGL_BG_DEFAULT), static_cast<uint32_t>(LV_PART_MAIN) | static_cast<uint32_t>(LV_STATE_DEFAULT));
  lv_obj_set_style_bg_opa(background, LV_OPA_COVER, static_cast<uint32_t>(LV_PART_MAIN) | static_cast<uint32_t>(LV_STATE_DEFAULT));
  // lv_disp_set_bg_color(NULL, lv_color_from_uint32(USE_LVGL_BG_DEFAULT));
  // lv_disp_set_bg_opa(NULL, LV_OPA_COVER);
  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(USE_LVGL_BG_DEFAULT), static_cast<uint32_t>(LV_PART_MAIN) | static_cast<uint32_t>(LV_STATE_DEFAULT));
  lv_obj_set_style_bg_opa(lv_screen_active(), LV_OPA_COVER, static_cast<uint32_t>(LV_PART_MAIN) | static_cast<uint32_t>(LV_STATE_DEFAULT));

#if LV_USE_LOG
  lv_log_register_print_cb(lvbe_debug);
#endif // LV_USE_LOG
#endif

#ifdef USE_UFILESYS
  // Add file system mapping
  static lv_fs_drv_t drv;      // LVGL8, needs to be static and not on stack
  lv_fs_drv_init(&drv);                     /*Basic initialization*/

  drv.letter = 'A';                         /*An uppercase letter to identify the drive */
  drv.ready_cb = nullptr;               /*Callback to tell if the drive is ready to use */
  drv.open_cb = &lvbe_fs_open;                 /*Callback to open a file */
  drv.close_cb = &lvbe_fs_close;               /*Callback to close a file */
  drv.read_cb = &lvbe_fs_read;                 /*Callback to read a file */
  drv.write_cb = &lvbe_fs_write;               /*Callback to write a file */
  drv.seek_cb = &lvbe_fs_seek;                 /*Callback to seek in a file (Move cursor) */
  drv.tell_cb = &lvbe_fs_tell;                 /*Callback to tell the cursor position  */

  drv.dir_open_cb = nullptr;         /*Callback to open directory to read its content */
  drv.dir_read_cb = nullptr;         /*Callback to read a directory's content */
  drv.dir_close_cb = nullptr;       /*Callback to close a directory */
  // drv.user_data = nullptr;             /*Any custom data if required*/

  lv_fs_drv_register(&drv);                 /*Finally register the drive*/

#endif // USE_UFILESYS

#ifdef USE_LVGL_FREETYPE
  // initialize the FreeType renderer
  lv_freetype_init(USE_LVGL_FREETYPE_MAX_FACES);
  // lv_freetype_init(USE_LVGL_FREETYPE_MAX_FACES,
  //                  USE_LVGL_FREETYPE_MAX_SIZES,
  //                  UsePSRAM() ? USE_LVGL_FREETYPE_MAX_BYTES_PSRAM : USE_LVGL_FREETYPE_MAX_BYTES);
#endif
#ifdef USE_LVGL_PNG_DECODER
  lv_lodepng_init();
#endif // USE_LVGL_PNG_DECODER

  // TODO check later about cache size
  if (UsePSRAM()) {
    lv_cache_set_max_size(LV_GLOBAL_DEFAULT()->img_cache, LV_IMG_CACHE_DEF_SIZE_PSRAM, nullptr);
  } else {
    lv_cache_set_max_size(LV_GLOBAL_DEFAULT()->img_cache, LV_IMG_CACHE_DEF_SIZE_NOPSRAM, nullptr);
  }

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LVGL "LVGL initialized"));
}

/*********************************************************************************************\
 * Callable from Berry
\*********************************************************************************************/
bool lvgl_started(void);
bool lvgl_started(void) {
  return (lvgl_glue != nullptr);
}

void lvgl_set_screenshot_file(File * file);
void lvgl_set_screenshot_file(File * file) {
  lvgl_glue->screenshot = file;
}

void lvgl_reset_screenshot_file(void);
void lvgl_reset_screenshot_file(void) {
  lvgl_glue->screenshot = nullptr;
}

File * lvgl_get_screenshot_file(void);
File * lvgl_get_screenshot_file(void) {
  return lvgl_glue->screenshot;
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv54(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      if (lvgl_glue) {
        if (TasmotaGlobal.sleep > USE_LVGL_MAX_SLEEP) {
          TasmotaGlobal.sleep = USE_LVGL_MAX_SLEEP;   // sleep is max 10ms
        }
        lv_task_handler();
      }
      break;
    case FUNC_ACTIVE:
      result = true;
      break;

  }
  return result;
}

#endif  // defined(USE_LVGL) && defined(USE_UNIVERSAL_DISPLAY)
#endif  // ESP32
