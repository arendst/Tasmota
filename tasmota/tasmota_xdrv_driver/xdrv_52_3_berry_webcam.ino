/*
  xdrv_52_3_berry_cam.ino - Berry scripting language, native functions

  Copyright (C) 2024 Christian Baars & Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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


#ifdef USE_BERRY

#include <berry.h>

#if defined(USE_WEBCAM)
#ifdef USE_BERRY_CAM // driver name is webcam in order to have this code behind the berry_img driver
#define USE_WEBCAM_SETUP_ONLY

#include "esp_camera.h"

/*********************************************************************************************\
 * Native functions mapped to Berry functions
\*********************************************************************************************/
extern "C" {

  /**
   * @brief Global webcam state structure
   */
  struct {
    uint16_t width = 0;    ///< Current frame width
    uint16_t height = 0;   ///< Current frame height
    uint8_t mode = 0;      ///< Camera mode/resolution setting
  } WcBerry;

  /**
   * @brief Initializes the webcam hardware
   * @param vm Berry VM pointer
   * @return Berry nil
   */
  int be_cam_init(struct bvm *vm);
  int be_cam_init(struct bvm *vm) {
    WcInit();
    be_return(vm);
  }

  /**
   * @brief Configures webcam with specified mode/resolution
   * @param vm Berry VM pointer
   * @return Berry integer (setup result code)
   * Berry arguments: (mode)
   */
  int be_cam_setup(struct bvm *vm);
  int be_cam_setup(struct bvm *vm) {
    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 1) {
      WcBerry.mode = be_toint(vm, 1);
    }
    else{
      be_raise(vm, "cam_error", "need mode");
      be_return_nil(vm);
    }
    
    int result = WcSetup(WcBerry.mode);
    if (result > 0) {
      camera_fb_t *wc_fb = esp_camera_fb_get();
      if(wc_fb){
        WcBerry.width = wc_fb->width;
        WcBerry.height = wc_fb->height;
        esp_camera_fb_return(wc_fb);
      }
    }
    be_pushint(vm, result);
    be_pop(vm, 1);
    be_return(vm);
  }

  /**
   * @brief Captures image from webcam, optionally decoding to specified format
   * @param vm Berry VM pointer
   * @return Berry bytes buffer (JPEG) or nil if storing to img instance
   * Berry arguments: ([img_instance], [format])
   * 
   * If no arguments: returns JPEG as bytes buffer
   * If img instance provided: stores image in instance (JPEG by default)
   * If format provided: decodes JPEG to specified format before storing
   */
  int be_cam_get_image(struct bvm *vm);
  int be_cam_get_image(struct bvm *vm) {

    camera_fb_t *wc_fb = esp_camera_fb_get();
    if (!wc_fb) {
      be_raise(vm, "cam_error", "could not get frame");
      be_return_nil(vm);
    }

    WcBerry.width = wc_fb->width;
    WcBerry.height = wc_fb->height;

    int32_t argc = be_top(vm);
    if (argc >= 1 && be_isinstance(vm, 1)) {
      const char * c = be_classname(vm, 1);
      if(strcmp(c,"img") != 0) {
        be_raise(vm, "cam_error", "instance not of img class");
        esp_camera_fb_return(wc_fb);
        be_return_nil(vm);
      }
      image_t * img = be_get_image_instance(vm);
      int format = -1;
      if (argc == 2 && be_isint(vm, 2)) {
        format =  be_toint(vm, 2);
      }
      if(img){
        if(format < 0 || format == PIXFORMAT_JPEG){
          be_img_util::from_buffer(img,wc_fb->buf, wc_fb->len, wc_fb->width, wc_fb->height, PIXFORMAT_JPEG);
        } 
        else {
          bool success = false;
          int bpp = be_img_util::getBytesPerPixel(pixformat_t(format));
          if(wc_fb->width * wc_fb->height * bpp != img->len){ // we do not really want tu use it like that, but repeatedly write the same format
            img->buf = (uint8_t*)heap_caps_realloc((void*)img->buf, wc_fb->width * wc_fb->height * bpp, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
            if(!img->buf){
              be_img_util::clear(img);
              be_raise(vm, "cam_error", "reallocation failed");
              be_return_nil(vm);
            }
          }
          img->width =  wc_fb->width;
          img->height = wc_fb->height;
          img->bpp = bpp;
          switch(pixformat_t(format)) {
            case PIXFORMAT_GRAYSCALE:
              success = be_img_util::jpeg_decode_one_image(wc_fb->buf, wc_fb->len, img->buf, EIGHT_BIT_GRAYSCALE, img);
              break;
            case PIXFORMAT_RGB565:
              success = be_img_util::jpeg_decode_one_image(wc_fb->buf, wc_fb->len, img->buf, RGB565_BIG_ENDIAN, img);
              break;
            case PIXFORMAT_YUV422: // Format 1: RGB565LE
              success = be_img_util::jpeg_decode_one_image(wc_fb->buf, wc_fb->len, img->buf, RGB565_LITTLE_ENDIAN, img);
              break;
            case PIXFORMAT_RGB888:
              success = be_img_util::jpeg_decode_one_image(wc_fb->buf, wc_fb->len, img->buf, RGB8888, img);
              break;
          }
          if(success){
            img->len = wc_fb->width * wc_fb->height * bpp;
            img->format = pixformat_t(format);
          } else {
            be_img_util::clear(img);
          }
        }
      }
      else{
        esp_camera_fb_return(wc_fb);
        be_raise(vm, "cam_error", "no image store");
      }
    }
    else{
      be_pushbytes(vm, wc_fb->buf, wc_fb->len); // JPG
    }

    esp_camera_fb_return(wc_fb);
    be_return(vm);
  }

  /**
   * @brief Returns webcam information as Berry map
   * @param vm Berry VM pointer
   * @return Berry map with mode, width, height
   */
  int be_cam_info(struct bvm *vm);
  int be_cam_info(struct bvm *vm) {
    be_newobject(vm, "map");
    be_map_insert_int(vm, "mode", WcBerry.mode);
    be_map_insert_int(vm, "width", WcBerry.width);
    be_map_insert_int(vm, "height", WcBerry.height);

    be_pop(vm, 1);
    be_return(vm);
  }
} //extern "C"

#endif // USE_BERRY_CAM
#endif // USE_WEBCAM

#endif  // USE_BERRY
