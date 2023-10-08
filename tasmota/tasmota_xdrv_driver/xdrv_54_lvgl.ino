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
#include "tasmota_lvgl_assets.h"    // force compilation of assets

#define XDRV_54             54

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "Adafruit_LvGL_Glue.h"

Adafruit_LvGL_Glue * glue;

// **************************************************
// Logging
// **************************************************
#if LV_USE_LOG
#ifdef USE_BERRY
static void lvbe_debug(const char *msg);
static void lvbe_debug(const char *msg) {
  be_writebuffer("LVG: ", sizeof("LVG: "));
  be_writebuffer(msg, strlen(msg));
  be_writebuffer("\n", sizeof("\n"));
}
#endif
#endif

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
#if (LV_COLOR_DEPTH == 16) && (LV_COLOR_16_SWAP == 1)
      uint16_t * pix = (uint16_t*) color_p;
      for (uint32_t i = 0; i < btw / 2; i++) (pix[i] = pix[i] << 8 | pix[i] >> 8);
#endif
      if (btw > 0) {    // if we had a previous error (ex disk full) don't try to write anymore
        int32_t ret = glue->getScreenshotFile()->write((const uint8_t*) color_p, btw);
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
  int lvbe_fclose(lvbe_FILE * stream) {
    File * f_ptr = (File*) stream;
    f_ptr->close();
    delete f_ptr;
    // AddLog(LOG_LEVEL_INFO, "LVG: lvbe_fclose(%p)", f_ptr);
    return 0;
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

/************************************************************
 * Initialize the display / touchscreen drivers then launch lvgl
 *
 * We use Adafruit_LvGL_Glue to leverage the Adafruit
 * display ecosystem.
 ************************************************************/
extern Renderer *Init_uDisplay(const char *desc);


void start_lvgl(const char * uconfig);
void start_lvgl(const char * uconfig) {

  if (glue != nullptr) {
    AddLog(LOG_LEVEL_DEBUG, D_LOG_LVGL "LVGL was already initialized");
    return;
  }

  if (!renderer || uconfig) {
#ifdef USE_UNIVERSAL_DISPLAY    // TODO - we will probably support only UNIV_DISPLAY
    renderer  = Init_uDisplay((char*)uconfig);
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
  // By default set the display color to black and opacity to 100%
  lv_disp_set_bg_color(NULL, lv_color_from_uint32(USE_LVGL_BG_DEFAULT));
  lv_disp_set_bg_opa(NULL, LV_OPA_COVER);
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_from_uint32(USE_LVGL_BG_DEFAULT), static_cast<uint32_t>(LV_PART_MAIN) | static_cast<uint32_t>(LV_STATE_DEFAULT));
  lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, static_cast<uint32_t>(LV_PART_MAIN) | static_cast<uint32_t>(LV_STATE_DEFAULT));

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
  lv_freetype_init(USE_LVGL_FREETYPE_MAX_FACES,
                   USE_LVGL_FREETYPE_MAX_SIZES,
                   UsePSRAM() ? USE_LVGL_FREETYPE_MAX_BYTES_PSRAM : USE_LVGL_FREETYPE_MAX_BYTES);
#endif
#ifdef USE_LVGL_PNG_DECODER
  lv_png_init();
#endif // USE_LVGL_PNG_DECODER

  if (UsePSRAM()) {
    lv_img_cache_set_size(LV_IMG_CACHE_DEF_SIZE_PSRAM);
  }

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_LVGL "LVGL initialized"));
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv54(uint32_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_INIT:
      break;
    case FUNC_LOOP:
      if (glue) {
        if (TasmotaGlobal.sleep > USE_LVGL_MAX_SLEEP) {
          TasmotaGlobal.sleep = USE_LVGL_MAX_SLEEP;   // sleep is max 10ms
        }
        lv_task_handler();
      }
      break;
    case FUNC_EVERY_50_MSECOND:
      break;
    case FUNC_EVERY_100_MSECOND:
      break;
    case FUNC_EVERY_SECOND:
      break;
    case FUNC_COMMAND:
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
