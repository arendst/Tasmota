/*
  xdrv_52_3_berry_img.ino - Berry scripting language, native functions

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

#ifdef USE_BERRY_IMAGE

#include "esp_camera.h"
#include "JPEGDEC.h"

/*********************************************************************************************\
 * typedefs
\*********************************************************************************************/

typedef struct {
    uint8_t * buf = nullptr;
    size_t len = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint8_t bpp = 0;
    pixformat_t format = PIXFORMAT_JPEG;
    JPEGDEC * jpeg = nullptr;
} image_t;

typedef struct {
  float scale_X;
  float shear_X;
  float shear_Y;
  float scale_Y;
  uint16_t translation_X;
  uint16_t translation_Y;
  uint16_t width;
  uint16_t height;
} be_img_roi_descriptor_t;

typedef struct {
        uint16_t width;
        uint16_t height;
        const uint8_t *input;
        uint8_t *output;
        size_t output_len;
} be_jpg_encoder_t;

typedef struct { 
    image_t *img; 
    int stride;
    size_t buf_size;
    bool crop_enabled;
    uint16_t target_width;
    uint16_t target_height;
} be_jpg_decode_ctx_t;

/*********************************************************************************************\
 * helper functions
\*********************************************************************************************/

struct be_img_util {
  /**
   * @brief Clears all image data and resets structure to defaults
   * @param img Pointer to image structure
   */
  static void clear(image_t *img){
    if(img->buf){
      free(img->buf);
      img->buf = nullptr;
    }
    img->len = 0;
    img->bpp = 0;
    img->format = PIXFORMAT_JPEG;
    img->width = 0;
    img->height = 0;
  }

  /**
   * @brief Returns bytes per pixel for a given pixel format
   * @param f Pixel format (note: value 1 is treated as RGB565LE, not YUV422)
   * @return Number of bytes per pixel (1, 2, or 3)
   */
  static int getBytesPerPixel(pixformat_t f){
    int bpp;
    switch(f) {
      case PIXFORMAT_GRAYSCALE:
        bpp = 1;
        break;
      case PIXFORMAT_RGB565:
        bpp = 2;
        break;
      case PIXFORMAT_YUV422: // Special case: we use value 1 for RGB565LE
        bpp = 2;
        break;
      default:
        bpp = 3;
    }
    return bpp;
  }

  /**
   * @brief Sets image format and updates bpp accordingly
   * @param img Pointer to image structure
   * @param format Pixel format to set
   */
  static void setFormat(image_t *img, pixformat_t format) {
    img->format = format;
    img->bpp = getBytesPerPixel(format);
  }

  /**
   * @brief Creates image from JPEG buffer without decoding
   * @param img Pointer to image structure
   * @param buffer JPEG data buffer
   * @param len Buffer length
   * @return true on success, false on failure
   */
  static bool from_jpg(image_t *img, uint8_t* buffer, size_t len) {
    uint16_t width, height;
    if(jpeg_size(buffer, len, &width, &height) != true){
      return false;
    }
    pixformat_t format = PIXFORMAT_JPEG;
    return from_buffer(img, buffer, len, width, height, format);
  }

  /**
   * @brief Creates image from raw buffer
   * @param img Pointer to image structure
   * @param buffer Source buffer
   * @param len Buffer length
   * @param w Image width
   * @param h Image height
   * @param f Pixel format
   * @return true on success, false on failure
   */
  static bool from_buffer(image_t *img, uint8_t* buffer, size_t len, uint16_t w, uint16_t h, pixformat_t f) {
    if(img->buf != nullptr) {
      free(img->buf);
    }
    img->buf = (uint8_t *)heap_caps_malloc((len)+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if(img->buf) {
      memcpy(img->buf,buffer,len);
      img->len = len;
      setFormat(img, f);
      img->width = w;
      img->height = h;
      return true;
    }
    return false;
  }

  /**
   * @brief Converts RGB888 to grayscale in-place
   * @param buffer RGB888 buffer
   * @param buffer_len Buffer length in bytes
   */
  static void rgb888_to_grayscale_inplace(uint8_t * buffer, size_t buffer_len){
    uint8_t r, g, b;
    for (uint32_t cnt=0; cnt<buffer_len; cnt+=3) {
      r = buffer[cnt];
      g = buffer[cnt+1];
      b = buffer[cnt+2];
      buffer[cnt/3] = (r + g + b) / 3;
    }
  }

  /**
   * @brief Converts RGB888 to RGB565 in-place (produces little endian)
   * @param buffer RGB888 buffer
   * @param buffer_len Buffer length in bytes
   */
  static void rgb888_to_565_inplace(uint8_t * buffer, size_t buffer_len){
    union{
      uint8_t* temp_buf ;
      uint16_t* temp_buf_16;
    };
    temp_buf = buffer;
    uint8_t red, grn, blu;
    uint16_t r, g, b;
    int j = 0;
    for (uint32_t i=0; i < buffer_len; i+=3) {
      blu = temp_buf[i];
      grn = temp_buf[i+1];
      red = temp_buf[i+2];
      b = (blu >> 3) & 0x1f;
      g = ((grn >> 2) & 0x3f) << 5;
      r = ((red >> 3) & 0x1f) << 11;
      temp_buf_16[j++] = (r | g | b);
    }
  }

  /**
   * @brief Swaps byte order of RGB565 buffer (LE <-> BE)
   * @param buffer RGB565 buffer
   * @param buffer_len Buffer length in bytes
   */
  static void rgb565_swap_bytes(uint8_t * buffer, size_t buffer_len){
    uint16_t* buf16 = (uint16_t*)buffer;
    size_t count = buffer_len / 2;
    for (size_t i = 0; i < count; i++) {
      buf16[i] = (buf16[i] >> 8) | (buf16[i] << 8);
    }
  }

  /**
   * @brief Callback for JPEG encoding output
   * @param arg Pointer to be_jpg_encoder_t structure
   * @param index Current write position
   * @param data Data to write
   * @param len Data length
   * @return Number of bytes written
   */
  static size_t _jpg_out_cb(void * arg, size_t index, const void* data, size_t len){
    be_jpg_encoder_t *jpeg = (be_jpg_encoder_t *)arg; 
    memcpy(jpeg->output + index, data, len);
    jpeg->output_len = index+len;
    return len;
  }

  /**
   * @brief Callback for JPEG decoder to draw MCU blocks
   * @param pDraw Pointer to JPEGDRAW structure
   * @return 1 on success, 0 on failure
   */
  static int jpeg_draw_mcu(JPEGDRAW *pDraw) {
      be_jpg_decode_ctx_t *ctx = (be_jpg_decode_ctx_t*)pDraw->pUser;
      if (!ctx || !ctx->img || !ctx->img->buf) return 0;
      image_t *img = ctx->img;
      
      // Crop check - skip MCUs outside target area (top-left crop)
      if (ctx->crop_enabled) {
          if (pDraw->x >= ctx->target_width || pDraw->y >= ctx->target_height) {
              return 1;  // Skip this MCU entirely
          }
      }
      
      // Calculate actual copy dimensions (handle partial MCUs at edges)
      int copy_width = pDraw->iWidth;
      int copy_height = pDraw->iHeight;
      if (ctx->crop_enabled) {
          if (pDraw->x + copy_width > ctx->target_width) {
              copy_width = ctx->target_width - pDraw->x;
          }
          if (pDraw->y + copy_height > ctx->target_height) {
              copy_height = ctx->target_height - pDraw->y;
          }
      }
      
      // Check if we're converting RGB8888 -> RGB888
      if (img->bpp == 3 && pDraw->iBpp == 32) {  // 32 bits = 4 bytes
          // Convert 4-byte pixels to 3-byte pixels on the fly
          size_t dst_offset = (pDraw->y * ctx->stride) + (pDraw->x * img->bpp);
          uint8_t *dst = img->buf + dst_offset;
          const uint8_t *src = (const uint8_t*)pDraw->pPixels;
          
          for (int y = 0; y < copy_height; ++y) {
              for (int x = 0; x < copy_width; ++x) {
                  dst[(y * ctx->stride) + (x * 3) + 0] = src[(y * pDraw->iWidth * 4) + (x * 4) + 0];
                  dst[(y * ctx->stride) + (x * 3) + 1] = src[(y * pDraw->iWidth * 4) + (x * 4) + 1];
                  dst[(y * ctx->stride) + (x * 3) + 2] = src[(y * pDraw->iWidth * 4) + (x * 4) + 2];
                  // Skip 4th byte
              }
          }
      } else {
          // Normal case - direct copy
          size_t dst_offset = (pDraw->y * ctx->stride) + (pDraw->x * img->bpp);
          // Check bounds for actual copy area, not full stride
          size_t last_row_offset = ((pDraw->y + copy_height - 1) * ctx->stride) + (pDraw->x * img->bpp) + (copy_width * img->bpp);
          if (last_row_offset > ctx->buf_size) {
              return 0;
          }
          
          uint8_t *dst = img->buf + dst_offset;
          const uint8_t *src = (const uint8_t*)pDraw->pPixels;
          
          for (int y = 0; y < copy_height; ++y) {
              memcpy(dst + y * ctx->stride, 
                    src + y * pDraw->iWidth * img->bpp,
                    copy_width * img->bpp);
          }
      }
      return 1;
  }

  /**
   * @brief Decodes JPEG image to specified format
   * @param input_buf JPEG input buffer
   * @param len Input buffer length
   * @param output_buf Output buffer for decoded pixels
   * @param type Pixel type (EIGHT_BIT_GRAYSCALE, RGB565_LITTLE_ENDIAN, RGB8888)
   * @param img Image structure (must have width, height, bpp pre-set)
   * @return true on success, false on failure
   */
  static bool jpeg_decode_one_image(uint8_t *input_buf, int len, uint8_t *output_buf, 
                                      int type, image_t *img) {
      if (!input_buf || len <= 0 || !output_buf || !img) return false;
      if (img->bpp == 0) return false;
      
      // Parse JPEG dimensions to check for mismatch
      uint16_t jpeg_width, jpeg_height;
      bool crop_enabled = false;
      if (jpeg_size(input_buf, len, &jpeg_width, &jpeg_height)) {
          if (jpeg_width != img->width || jpeg_height != img->height) {
              // Dimension mismatch - enable cropping
              crop_enabled = true;
              AddLog(LOG_LEVEL_DEBUG, PSTR("IMG: JPEG crop %dx%d -> %dx%d"), 
                     jpeg_width, jpeg_height, img->width, img->height);
          }
      }
      
      img->buf = output_buf;
      int stride = img->width * img->bpp;
      size_t buf_size = img->width * img->height * img->bpp;
      be_jpg_decode_ctx_t ctx = {img, stride, buf_size, crop_enabled, img->width, img->height};

      JPEGDEC *jpeg = new JPEGDEC();
      if (!jpeg) return false;
      
      int result = jpeg->openRAM(input_buf, len, jpeg_draw_mcu);
      if (result <= 0) {
          delete jpeg;
          return false;
      }
      jpeg->setUserPointer(&ctx);
      jpeg->setPixelType(type);
      bool ok = jpeg->decode(0, 0, 0);
      
      jpeg->close();
      delete jpeg;
      return ok;
  }

  /**
   * @brief Extracts width and height from JPEG buffer
   * @param buffer JPEG data buffer
   * @param data_size Buffer size
   * @param width Output: image width
   * @param height Output: image height
   * @return true if valid JPEG and dimensions extracted, false otherwise
   */
  static bool jpeg_size(uint8_t* buffer, size_t data_size, uint16_t *width, uint16_t *height) {
    union{
      uint8_t * data;
      uint16_t * data16;
      uint32_t * data32;
    };
    data = buffer;
    if(data[data_size-2] != 0xff ||  data[data_size-1] != 0xd9){
      return false; // quick check for correct termination
    }
    if(data32[0] == 0xE0FFD8FF) {
      // Check for valid JPEG header (null terminated JFIF)
      // if(data[i+2] == 'J' && data[i+3] == 'F' && data[i+4] == 'I' && data[i+5] == 'F' && data[i+6] == 0x00) {
      if(data16[3] == 0x464a && data16[4] == 0x4649) {
          //Retrieve the block length of the first block since the first block will not contain the size of file
        int i = 20;
        uint16_t block_length = 0;
        while(i<data_size) {
          i+=block_length;               //Increase the file index to get to the next block
          if(i >= data_size) {
            return false;}   //Check to protect against segmentation faults
          if (data[i] != 0xFF) {
            return false;}
          if(data[i+1] == 0xC0 || data[i+1] == 0xC2) {   //0xFFC0 or 0xFFC2 is the "Start of frame" marker which contains the file size
            //The structure of the 0xFFC0 block is quite simple [0xFFC0][ushort length][uchar precision][ushort x][ushort y]
            *height = data[i+5]*256 + data[i+6];
            *width = data[i+7]*256 + data[i+8];
            return true;
          }
          else
          {
            i+=2;                              //Skip the block marker
            block_length = data[i] * 256 + data[i+1];   //Go to the next block
          }
        }
      }
    }
    return false;               //Not a valid SOI header
  }
};

/*********************************************************************************************\
 * Native functions mapped to Berry functions
\*********************************************************************************************/

extern "C" {

  /**
   * @brief Gets image instance from Berry VM
   * @param vm Berry VM pointer
   * @return Pointer to image_t structure
   */
  image_t* be_get_image_instance(struct bvm *vm);
  image_t* be_get_image_instance(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    image_t * img = (image_t *) be_tocomptr(vm, -1);
    be_pop(vm, 1);
    if(!img){
      be_raise(vm, "img_error", "no store instance");
    }
    return img;
  }

  /**
   * @brief Initializes a new image instance
   * @param vm Berry VM pointer
   * @return Berry nil
   */
  int be_img_init(struct bvm *vm);
  int be_img_init(struct bvm *vm) {
    image_t *img = new image_t;
    be_pushcomptr(vm, (void*)img);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  /**
   * @brief Deinitializes and frees image instance
   * @param vm Berry VM pointer
   * @return Berry nil
   */
  int be_img_deinit(struct bvm *vm);
  int be_img_deinit(struct bvm *vm) {
    be_getmember(vm, 1, ".p");
    image_t * img = (image_t *) be_tocomptr(vm, -1);
    if(img){
      if(img->buf != nullptr){
        free(img->buf);
      }
      delete img;
      be_pushcomptr(vm, (void*) NULL);
      be_setmember(vm, 1, ".p");
    }
    be_return_nil(vm);
  }

  /**
   * @brief Loads image from JPEG buffer, optionally decoding to specified format
   * @param vm Berry VM pointer
   * @return Berry nil
   * Berry arguments: (bytes_buffer, [format])
   */
  int be_img_from_jpg(struct bvm *vm);
  int be_img_from_jpg(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc > 1 && be_isbytes(vm, 2)){
      size_t src_buf_len;
      uint8_t* src_buf = (uint8_t*) be_tobytes(vm, 2, &src_buf_len);
      int format = -1;
      if(argc == 3 && be_isint(vm, 3)){
        format =  be_toint(vm, 3);
      }
      image_t * img = be_get_image_instance(vm);
      if(format == -1){
        if(be_img_util::from_jpg(img,src_buf,src_buf_len) == false){
          be_raise(vm, "img_error", "could not store from jpg buffer");
        } else {
          be_return_nil(vm); // done
        }
      }
      
      bool success = false;
      uint16_t w,h;
      if (be_img_util::jpeg_size(src_buf, src_buf_len, &w, &h) == false){
        be_raise(vm, "img_error", "no compatible jpg buffer");
        be_return_nil(vm); //do not destroy the old image
      }
      
      be_img_util::setFormat(img, pixformat_t(format));
      const size_t newSize = w * h * img->bpp;
      
      if(newSize != img->len){
        img->buf = (uint8_t*)heap_caps_realloc((void*)img->buf, newSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if(!img->buf){
          be_img_util::clear(img);
          be_raise(vm, "img_error", "reallocation failed");
          be_return_nil(vm);
        }
      }
      
      img->width = w;
      img->height = h;
      
      if(format == PIXFORMAT_GRAYSCALE) {
        success = be_img_util::jpeg_decode_one_image(src_buf, src_buf_len, img->buf, EIGHT_BIT_GRAYSCALE, img);
      }
      else if(format == PIXFORMAT_RGB565) { // Format 0: RGB565 big endian
        success = be_img_util::jpeg_decode_one_image(src_buf, src_buf_len, img->buf, RGB565_BIG_ENDIAN, img);
      }
      else if(format == 1) { // Format 1: RGB565 little endian (RGB565LE)
        success = be_img_util::jpeg_decode_one_image(src_buf, src_buf_len, img->buf, RGB565_LITTLE_ENDIAN, img);
      }
      else if(format == PIXFORMAT_RGB888) {
        success = be_img_util::jpeg_decode_one_image(src_buf, src_buf_len, img->buf, RGB8888, img);
      }
      else {
        be_img_util::clear(img);
        be_raise(vm, "img_error", "unsupported format");
        be_return_nil(vm);
      }
      
      if(success){
        img->len = newSize;
      } else {
        be_img_util::clear(img);
        be_raise(vm, "img_error", "jpg decoding failed");
      }
    }
    else{
      be_raise(vm, "img_error", "wrong args for jpg buffer");
    }
    be_return(vm);
  }

  /**
   * @brief Loads image from raw buffer
   * @param vm Berry VM pointer
   * @return Berry nil
   * Berry arguments: (bytes_buffer, width, height, format)
   */
  int be_img_from_buffer(struct bvm *vm);
  int be_img_from_buffer(struct bvm *vm) {
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 5 && be_isbytes(vm, 2) && be_isint(vm, 3) && be_isint(vm, 4) && be_isint(vm, 5)){
      size_t src_buf_len;
      uint8_t* src_buf = (uint8_t*) be_tobytes(vm, 2, &src_buf_len);
      // be_pop(vm, 2);
      image_t * img = be_get_image_instance(vm);
      if(img){
        if(img->buf != nullptr){
          free(img->buf);
        }
        uint16_t width = be_toint(vm, 3);
        uint16_t height = be_toint(vm, 4);
        int format = be_toint(vm, 5);
        if(be_img_util::from_buffer(img, src_buf, src_buf_len, width, height, (pixformat_t)format) == false){
          be_raise(vm, "img_error", "could not store from byte buffer");
        }
      }
    }
    be_return(vm);
  }

  /**
   * @brief Gets image buffer or ROI (region of interest) buffer
   * @param vm Berry VM pointer
   * @return Berry bytes buffer
   * Berry arguments: ([roi_descriptor])
   */
  int be_img_get_buffer(struct bvm *vm);
  int be_img_get_buffer(struct bvm *vm) {
    image_t * img = be_get_image_instance(vm);
    be_img_roi_descriptor_t * dsc;

    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isbytes(vm, 2)){
      if(img->format == PIXFORMAT_JPEG){
        be_raise(vm, "img_error", "JPEG not supported for ROI");
      }
      size_t dsc_len;
      dsc = (be_img_roi_descriptor_t*) be_tobytes(vm, 2, &dsc_len);
      if(dsc_len != sizeof(be_img_roi_descriptor_t)){
        be_raise(vm, "img_error", "invalid ROI descriptor");
      }
    }
    else{
      be_pushbytes(vm, img->buf, img->len);
      be_return(vm);
    }

    const size_t roi_size = dsc->width * dsc->height * img->bpp;
    uint8_t* roi_buf = (uint8_t*)malloc(roi_size);
    if(roi_buf == nullptr) {
      be_raise(vm, "img_error", "ROI buffer allocation failed");
    }

  /**
   *   Fills ROI using an affine matrix (https://en.wikipedia.org/wiki/Affine_transformation#Image_transformation)
   *   | scale_x shear_x translation_x |
   *   | shear_y scale_y translation_y |
   *   | 0	      0       1            | - these are constants in this scope
   */
    uint32_t in_idx = 0;
    for(uint16_t y = 0; y < dsc->height; y += 1) {
      for(uint16_t x = 0; x < dsc->width; x += 1) {
        int transformed_X = (x * dsc->scale_X) + (y * dsc->shear_Y) + dsc->translation_X;
        int transformed_Y = (x * dsc->shear_X) + (y * dsc->scale_Y) + dsc->translation_Y;
        for(int byte_comp = 0; byte_comp < img->bpp; byte_comp += 1){
          roi_buf[in_idx++] = img->buf[(transformed_X * img->bpp) + ((transformed_Y * img->bpp) * img->width) + byte_comp];
        }
      }
    }
    
    be_pushbytes(vm, roi_buf, roi_size);
    free(roi_buf);
    be_return(vm);
  }

  /**
   * @brief Converts image to different pixel format
   * @param vm Berry VM pointer
   * @return Berry nil
   * Berry arguments: (format, [quality_for_jpeg])
   */
  int be_img_convert_to(struct bvm *vm);
  int be_img_convert_to(struct bvm *vm) {
    image_t * img = be_get_image_instance(vm);
    if(img->len == 0) {
      be_raise(vm, "img_error", "no image data");
      be_return(vm);
    }

    int32_t argc = be_top(vm);
    if (argc >= 2 && be_isint(vm, 2)) {
      uint32_t format = be_toint(vm, 2);
      int32_t option = -1;
      if(be_isint(vm, 3)){
        option = be_toint(vm, 3);
      }
      uint8_t* temp_buf = nullptr;

      if(img->format == format){
        be_raise(vm, "img_error", "no format change");
        be_return(vm);
      }
      uint16_t bpp = 3; // most likely byte-per-pixel value
      const size_t pixel_count = img->width * img->height ;
      size_t  temp_buf_len = pixel_count * bpp;
      if(format == PIXFORMAT_JPEG) {
        temp_buf_len /= 8; // a very rough guess
      }
      temp_buf = (uint8_t *)heap_caps_malloc((temp_buf_len)+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      if(temp_buf == nullptr) {
        be_raise(vm, "img_error", "not enough heap");
        be_return_nil(vm);
      }
      if(format != PIXFORMAT_JPEG) {
        // Handle RGB565LE (format 1) - need to swap to BE before conversion
        bool swapped = false;
        if(img->format == PIXFORMAT_YUV422) { // Format 1 is RGB565LE in our implementation
          be_img_util::rgb565_swap_bytes(img->buf, img->len);
          swapped = true;
        }
        
        pixformat_t src_format = (img->format == PIXFORMAT_YUV422) ? PIXFORMAT_RGB565 : img->format;
        if(!fmt2rgb888((const uint8_t *)img->buf, img->len, src_format, temp_buf)) {
          if(swapped) {
            be_img_util::rgb565_swap_bytes(img->buf, img->len); // Swap back on failure
          }
          free(temp_buf);
          be_raise(vm, "img_error", "not enough heap");
          be_return_nil(vm);
        }
      }
      be_jpg_encoder_t jpeg;
      jpeg.width = 0;
      jpeg.height = 0;
      jpeg.input = img->buf;
      jpeg.output = temp_buf;
      jpeg.output_len = 0;
      switch(format){
        case PIXFORMAT_GRAYSCALE: // always from temporary RGB888
          be_img_util::rgb888_to_grayscale_inplace(temp_buf,temp_buf_len);
          temp_buf_len = pixel_count;
          break;
        case PIXFORMAT_RGB565: // Format 0: RGB565 big endian - from temporary RGB888
          be_img_util::rgb888_to_565_inplace(temp_buf,temp_buf_len);
          be_img_util::rgb565_swap_bytes(temp_buf, pixel_count * 2); // Convert LE to BE
          temp_buf_len = pixel_count * 2;
          break;
        case PIXFORMAT_YUV422: // Format 1: RGB565LE - from temporary RGB888
          be_img_util::rgb888_to_565_inplace(temp_buf,temp_buf_len);
          temp_buf_len = pixel_count * 2;
          break;
        case PIXFORMAT_RGB888:
         // should already be there
          break;
        case PIXFORMAT_JPEG:
          {
            int quality = 12;
            if(option != -1){
              quality = option;
            }
            // Handle RGB565LE (format 1) - need to swap to BE before JPEG encoding
            pixformat_t encode_format = img->format;
            if(img->format == PIXFORMAT_YUV422) { // Format 1 is RGB565LE
              be_img_util::rgb565_swap_bytes(temp_buf, img->len); // Swap LE to BE in temp buffer
              encode_format = PIXFORMAT_RGB565;
            }
            fmt2jpg_cb(img->buf, img->len, img->width, img->height, encode_format, quality, be_img_util::_jpg_out_cb, (void *)&jpeg);
            temp_buf_len = jpeg.output_len;
          }
          break;
        default:
          free(temp_buf);
          be_raise(vm, "img_error", "format not supported");
          // be_return_nil(vm);
         break;
      }
      free(img->buf);
      be_img_util::setFormat(img, pixformat_t(format));
      img->len = temp_buf_len;
      img->buf = (uint8_t*)heap_caps_realloc((void*)temp_buf, img->len, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT); // shrinking should never fail ...
      if(img->buf == nullptr) {
        be_img_util::clear(img);
        free(temp_buf);
        be_raise(vm, "img_error", "reallocation failed");
      }
    }
    be_return(vm);
  }

  /**
   * @brief Returns image information as Berry map
   * @param vm Berry VM pointer
   * @return Berry map with buf_addr, size, width, height, bpp, format
   */
  int be_img_info(struct bvm *vm);
  int be_img_info(struct bvm *vm) {
    image_t * img = be_get_image_instance(vm);
    if(!img){
      be_raise(vm, "img_error", "no image instance");
      be_return(vm);
    }
    be_newobject(vm, "map");
    be_map_insert_int(vm, "buf_addr", (uint32_t)img->buf);
    be_map_insert_int(vm, "size", img->len);
    be_map_insert_int(vm, "width", img->width);
    be_map_insert_int(vm, "height", img->height);
    be_map_insert_int(vm, "bpp", img->bpp);
    be_map_insert_int(vm, "format", img->format);
    be_pop(vm, 1);
    be_return(vm);
  }

} //extern "C"

#endif // USE_BERRY_JPEG

#endif  // USE_BERRY