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

/*********************************************************************************************\
 * helper functions
\*********************************************************************************************/

struct be_img_util {
  static void clear(image_t *img){
    if(img->buf){
      free(img->buf);
      img->buf = nullptr;
    }
    img->len = 0;
    img->format = PIXFORMAT_JPEG;
    img->width = 0;
    img->height = 0;
  }

  static int getBytesPerPixel(pixformat_t f){
    int bpp;
    switch(f) {
      case PIXFORMAT_GRAYSCALE:
        bpp = 1;
        break;
      case PIXFORMAT_RGB565:
        bpp = 2;
        break;
      default:
        bpp = 3;
    }
    return bpp;
  }

  static bool from_jpg(image_t *img, uint8_t* buffer, size_t len) {
    uint16_t width, height;
    if(jpeg_size(buffer, len, &width, &height) != true){
      return false;
    }
    pixformat_t format = PIXFORMAT_JPEG;
    return from_buffer(img, buffer, len, width, height, format);
  }

  static bool from_buffer(image_t *img, uint8_t* buffer, size_t len, uint16_t w, uint16_t h, pixformat_t f) {
    if(img->buf != nullptr) {
      free(img->buf);
    }
    img->buf = (uint8_t *)heap_caps_malloc((len)+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if(img->buf) {
      memcpy(img->buf,buffer,len);
      img->len = len;
      img->format = f;
      img->width = w;
      img->height = h;
      return true;
    }
    return false;
  }

  static void rgb888_to_grayscale_inplace(uint8_t * buffer, size_t buffer_len){
    uint8_t r, g, b;
    for (uint32_t cnt=0; cnt<buffer_len; cnt+=3) {
      r = buffer[cnt];
      g = buffer[cnt+1];
      b = buffer[cnt+2];
      buffer[cnt/3] = (r + g + b) / 3;
    }
  }

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

  static size_t _jpg_out_cb(void * arg, size_t index, const void* data, size_t len){
    be_jpg_encoder_t *jpeg = (be_jpg_encoder_t *)arg; 

    memcpy(jpeg->output + index, data, len);
    jpeg->output_len = index+len;
    return len;
  }

  static int _rgb_write_dummy(JPEGDRAW* d)
  {
      // not used for now
      return 1;
  }

  static bool jpeg_decode_one_image(uint8_t *input_buf, int len, uint8_t *output_buf, int type, image_t * img) {
    img->jpeg = new JPEGDEC;
    img->jpeg->openRAM(input_buf, len, _rgb_write_dummy);
    img->jpeg->setPixelType(type);
    img->jpeg->setFramebuffer(output_buf);
    bool success = img->jpeg->decode(0,0,0);
    img->jpeg->close();
    delete img->jpeg;
    return success;
  }


  // https://web.archive.org/web/20131016210645/http://www.64lines.com/jpeg-width-height , but shorter now by 48 bytes flash
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

  int be_img_init(struct bvm *vm);
  int be_img_init(struct bvm *vm) {
    image_t *img = new image_t;
    be_pushcomptr(vm, (void*)img);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

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
      const int bpp = be_img_util::getBytesPerPixel(pixformat_t(format));
      uint16_t w,h;
      if (be_img_util::jpeg_size(src_buf, src_buf_len, &w, &h) == false){
        be_raise(vm, "img_error", "no compatible jpg buffer");
        be_return_nil(vm); //do not destroy the old image
      }
      const size_t newSize = w * h * bpp;
      if(newSize != img->len){
        img->buf = (uint8_t*)heap_caps_realloc((void*)img->buf, newSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if(!img->buf){
          be_img_util::clear(img);
          be_raise(vm, "img_error", "reallocation failed");
          be_return_nil(vm);
        }
      }
      switch(pixformat_t(format)) {
        case PIXFORMAT_GRAYSCALE:
          success = be_img_util::jpeg_decode_one_image(src_buf, src_buf_len, img->buf, EIGHT_BIT_GRAYSCALE, img);
          break;
        case PIXFORMAT_RGB565:
          success = be_img_util::jpeg_decode_one_image(src_buf, src_buf_len, img->buf, RGB565_LITTLE_ENDIAN, img);
          success = true;
          break;
        case PIXFORMAT_RGB888:
          success = be_img_util::jpeg_decode_one_image(src_buf, src_buf_len, img->buf, RGB8888, img);
          success = true;
          break;
      }
      if(success){
        img->len = newSize;
        img->format = pixformat_t(format);
        img->width = w;
        img->height = h;
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

  int be_img_from_buffer(struct bvm *vm); // (bytes(),width,height,format)
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

  int be_img_get_buffer(struct bvm *vm); //(roi)
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

    const int bpp = be_img_util::getBytesPerPixel(img->format);
    const size_t roi_size = dsc->width * dsc->height * bpp;
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
        for(int byte_comp = 0; byte_comp < bpp; byte_comp += 1){
          roi_buf[in_idx++] = img->buf[(transformed_X * bpp) + ((transformed_Y * bpp) * img->width) + byte_comp];
        }
      }
    }
    
    be_pushbytes(vm, roi_buf, roi_size);
    free(roi_buf);
    be_return(vm);
  }

  int be_img_convert_to(struct bvm *vm); // (pixformat)
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
        temp_buf_len /= 4; // a very rough guess
      }
      temp_buf = (uint8_t *)heap_caps_malloc((temp_buf_len)+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      if(temp_buf == nullptr) {
        be_raise(vm, "img_error", "not enough heap");
        be_return_nil(vm);
      }
      if(format != PIXFORMAT_JPEG) {
        if(!fmt2rgb888((const uint8_t *)img->buf, img->len, img->format, temp_buf)) {
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
        case PIXFORMAT_GRAYSCALE: // always from temporary RGB88
          be_img_util::rgb888_to_grayscale_inplace(temp_buf,temp_buf_len);
          temp_buf_len = pixel_count;
          break;
        case PIXFORMAT_RGB565: // always from temporary RGB88
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
            fmt2jpg_cb(img->buf, img->len, img->width, img->height, img->format, quality, be_img_util::_jpg_out_cb, (void *)&jpeg);
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
      img->format = pixformat_t(format);
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
    be_map_insert_int(vm, "format", img->format);
    be_pop(vm, 1);
    be_return(vm);
  }

} //extern "C"

#endif // USE_BERRY_JPEG

#endif  // USE_BERRY
