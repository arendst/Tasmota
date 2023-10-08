/*
  jpeg_utils.c - Version header file for Tasmota

  Copyright (C) 2021  Theo Arends

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
#ifdef JPEG_PICTS

#include "img_converters.h"
#include "esp_jpg_decode.h"

void rgb888_to_565(uint8_t *in, uint16_t *out, uint32_t len) {
uint8_t red, grn, blu;
uint16_t b , g, r;

  for (uint32_t cnt=0; cnt<len; cnt++) {
    blu = *in++;
    grn = *in++;
    red = *in++;

    b = (blu >> 3) & 0x1f;
    g = ((grn >> 2) & 0x3f) << 5;
    r = ((red >> 3) & 0x1f) << 11;
    *out++ = (r | g | b);
  }

}

void rgb888_to_565i(uint8_t *in, uint16_t *out, uint32_t len) {
uint8_t red, grn, blu;
uint16_t b , g, r;

  for (uint32_t cnt=0; cnt<len; cnt++) {
    blu = 255-*in++;
    grn = 255-*in++;
    red = 255-*in++;

    b = (blu >> 3) & 0x1f;
    g = ((grn >> 2) & 0x3f) << 5;
    r = ((red >> 3) & 0x1f) << 11;
    *out++ = (r | g | b);
  }
}

typedef struct {
        uint16_t width;
        uint16_t height;
        uint16_t data_offset;
        const uint8_t *input;
        uint8_t *output;
} rgb_jpg_decoder;

//input buffer
static uint32_t _jpg_read(void * arg, size_t index, uint8_t *buf, size_t len)
{
    rgb_jpg_decoder * jpeg = (rgb_jpg_decoder *)arg;
    if(buf) {
        memcpy(buf, jpeg->input + index, len);
    }
    return len;
}

//output buffer and image width
static bool _rgb_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
    rgb_jpg_decoder * jpeg = (rgb_jpg_decoder *)arg;
    if(!data){
        if(x == 0 && y == 0){
            //write start
            jpeg->width = w;
            jpeg->height = h;
            //if output is null, this is BMP
            if(!jpeg->output){
                jpeg->output = (uint8_t *)malloc((w*h*3)+jpeg->data_offset);
                if(!jpeg->output){
                    return false;
                }
            }
        } else {
            //write end
        }
        return true;
    }

    size_t jw = jpeg->width*3;
    size_t t = y * jw;
    size_t b = t + (h * jw);
    size_t l = x * 3;
    uint8_t *out = jpeg->output+jpeg->data_offset;
    uint8_t *o = out;
    size_t iy, ix;

    w = w * 3;

    for(iy=t; iy<b; iy+=jw) {
        o = out+iy+l;
        for(ix=0; ix<w; ix+= 3) {
            o[ix] = data[ix+2];
            o[ix+1] = data[ix+1];
            o[ix+2] = data[ix];
        }
        data+=w;
    }
    return true;
}


esp_err_t esp_jpg_decode(size_t len, jpg_scale_t scale, jpg_reader_cb reader, jpg_writer_cb writer, void * arg);


bool jpg2rgb888(const uint8_t *src, size_t src_len, uint8_t * out, jpg_scale_t scale)
{
    rgb_jpg_decoder jpeg;
    jpeg.width = 0;
    jpeg.height = 0;
    jpeg.input = src;
    jpeg.output = out;
    jpeg.data_offset = 0;

    if(esp_jpg_decode(src_len, scale, _jpg_read, _rgb_write, (void*)&jpeg) != ESP_OK){
        return false;
    }
    return true;
}

// https://web.archive.org/web/20131016210645/http://www.64lines.com/jpeg-width-height
//Gets the JPEG size from the array of data passed to the function, file reference: http://www.obrador.com/essentialjpeg/headerinfo.htm
char get_jpeg_size(unsigned char* data, unsigned int data_size, unsigned short *width, unsigned short *height) {
   //Check for valid JPEG image
   int i=0;   // Keeps track of the position within the file
   if(data[i] == 0xFF && data[i+1] == 0xD8 && data[i+2] == 0xFF && data[i+3] == 0xE0) {
      i += 4;
      // Check for valid JPEG header (null terminated JFIF)
      if(data[i+2] == 'J' && data[i+3] == 'F' && data[i+4] == 'I' && data[i+5] == 'F' && data[i+6] == 0x00) {
         //Retrieve the block length of the first block since the first block will not contain the size of file
         unsigned short block_length = data[i] * 256 + data[i+1];
         while(i<data_size) {
            i+=block_length;               //Increase the file index to get to the next block
            if(i >= data_size) return false;   //Check to protect against segmentation faults
            if(data[i] != 0xFF) return false;   //Check that we are truly at the start of another block
            if(data[i+1] == 0xC0) {            //0xFFC0 is the "Start of frame" marker which contains the file size
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
         return false;                     //If this point is reached then no size was found
      }else{ return false; }                  //Not a valid JFIF string

   }else{ return false; }                     //Not a valid SOI header
}


#endif // JPEG_PICTS
#endif //ESP32

#ifdef USE_DISPLAY_DUMP
#define bytesPerPixel 3
#define fileHeaderSize 14
#define infoHeaderSize 40

void createBitmapFileHeader(uint32_t height, uint32_t width, uint8_t *fileHeader) {
  int paddingSize = (4 - (width*bytesPerPixel) % 4) % 4;

    int fileSize = fileHeaderSize + infoHeaderSize + (bytesPerPixel*width+paddingSize) * height;
    memset(fileHeader,0,fileHeaderSize);
    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

}

void createBitmapInfoHeader(uint32_t height, uint32_t width, uint8_t *infoHeader ) {
    memset(infoHeader,0,infoHeaderSize);

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);
    infoHeader[24] = (unsigned char)0x13;  // 72 dpi
    infoHeader[25] = (unsigned char)0x0b;
    infoHeader[28] = (unsigned char)0x13;
    infoHeader[29] = (unsigned char)0x0b;

}
#endif // USE_DISPLAY_DUMP
