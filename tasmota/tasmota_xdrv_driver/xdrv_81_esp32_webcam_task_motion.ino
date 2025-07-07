/*
  xdrv_99_esp32_webcamberry.ino - ESP32 webcam support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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
#ifdef USE_WEBCAM
// defining USE_WEBCAM_V2 will use xdrv_81_esp32_webcam_task.ino rather than xdrv_81_esp32_webcam.ino
#ifdef USE_WEBCAM_V2
// only applies to USE_WEBCAM_V2
#ifdef USE_WEBCAM_MOTION


/*********************************************************************************************\
 * ESP32 webcam motion routines
 *
 * WcGetmotionpixelsN = (N=1..4) read addr, len, w, h as JSON {"addr":123456,"len":12345,"w":160,"h":120, "format":4} 
 *    motion(1)
 *    difference(2) buffer - e.g for berry
 *    mask(3)
 *    background(4)
 *     e.g. could be used to read pixels, or change pixels from berry.

 * WcConvertFrameN <format> <scale> - convert a wcgetframe in picstore from jpeg to <format> (0=2BPP/RGB565, 3=1BPP/GRAYSCALE, 5=3BPP/RGB888), <scale> (0-3)
 *     converts in place, replacing the stored frame with the new format.  Data can be retrieved using wcgetpicstoreN (e.g. for use in berry)
 *     will fail if it can't convert or allocate.
 * 
 * WcSetPicture - SetPictureN (N=1-MAX_PICTORE) expects 'addr len format [width height]
 *   use to populate a frame in Wc.picstore from Berry.  e.g. to put a JPEG mask there
 *   so you can then decode it, get it's address, get the address of the current mask, and 
 *   copy data across.
 *   if sending JPEG (format=0|5), width and height are calculated on decode.
 *   if sending pixels (format=4(GRAY)|6(RGB)|1(RGB565)), width and height are required, and used to allocate.
 *   binary data is copied from addr.  i.e. you can send the addr/len from Berry introspect bytes.
 *    ideas: could be used to set background image based on time of day.

### Enable motion detection interval
WCsetMotiondetect <timems>
WCsetMotiondetect 2000

### disable motion detection
WCsetMotiondetect 0

### return overall normalised pixel difference.
WCsetMotiondetect -1

### return overall normalised brightness.
WCsetMotiondetect -2

### motion detect via jpeg frame size (% change)
Does not run motion detect frame scan!
(i.e. you can turn off WCsetMotiondetect 0 - and detect at 50fps)
WCsetMotiondetect2 <fsizediff percent limit>
WCsetMotiondetect2 20

### Pixel diff value threshold
if set > 0, pixels with a difference > threshold will be counted
if a difference buffer is enabled, then pixels in it are raised to 255.
WCsetMotiondetect3 <pixel diff threshold 1-255>
WCsetMotiondetect3 10

### Pixels over threshold trigger
if the number of pixels changed (per 10000) is > limit, motion is triggered.
set 0 to disable.
WCsetMotiondetect4 <pixel count trigger thresh, in 1/10000>
WCsetMotiondetect4 10

### set scale for motion detect image ref camera res
the number 0-7 = 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128
values 0-3 use scaling on jpeg decode (fast).
values 4-7 add software scaling (not much performance gain, but some) 
WCsetMotiondetect5 0-7

### enable difference buffer
enable/disable the difference buffer
WCsetMotiondetect6 0-1

### Set motion trigger threshold (accumulated pixels difference)
normalised value, try 1000...
WCsetMotiondetect7 0-nnnnn


Endpoints:
http://IP/motionlbuff.jpg - last motion buffer as jpeg (fully scaled) - if motion enabled, else 404
http://IP/motionbuff.jpg - motion buffer as jpeg (only jpeg scaled) - if motion enabled, else 404
http://IP/motiondiff.jpg - last difference buffer as jpeg (fully scaled) - if enabled, else 404

http://IP:81/diff.mjpeg - motion or difference buffer stream as mjpeg (fully scaled, buf is motion if diff not enabled) - if enabled, else no data

 */

/*********************************************************************************************/

/*********************************************************************************************/
/*
Berry usage:
if you turn on motion detect with "wcsetmotiondetect 1000" (number is ms between detect)
or it you turn on frame sending using "wcberryframes 1"
then berry will be called expecting to find a driver which has a method "webcam", e.g.:

var piccount = 0
class MyWebcamDriver
  #- create a method for adding a button to the main menu -#
  def webcam(cmd, idx, payload)
    print("webcam "+cmd+" payload:"+payload)
    if (cmd == "motion")
      #split payload at space
      var val = int(payload)
      if (val > 1000)
        piccount = piccount+1
        var tcmd = "wcsavepic0 /myfile" .. piccount .. ".jpg"
        tasmota.cmd(tcmd)
        print("webcam motion -> "+tcmd)
      end
    end
  end
end

This  will be called with 
"motion <framediff> <<framebrightness>"
and/or
"frame"

*/
/*********************************************************************************************/

#include "cam_hal.h"
#include "esp_camera.h"
#include "sensor.h"
#include "fb_gfx.h"
#include "camera_pins.h"
#include "jpeg_decoder.h"
//#include "img_converters.h"

extern SemaphoreHandle_t WebcamMutex;

// use mutex like:
// TasAutoMutex localmutex(&WebcamMutex, "somename", 200);
// in any function.  Will wait for mutex to be clear, and auto-release when the function exits.

/*********************************************************************************************/
// functions to encode into a jpeg buffer.
// we keep ONE allocated buffer for this, and expand it if the jpeg does not fit.
// single expanding jpeg buffer for encode into
// call encode_reset() to maybe recover from failure to allocate.

#define DEFAULT_INITIAL_JPEG_LEN 16384


// this is a callback called from the fmt2jpg_cb in WcencodeToJpeg.
// it writes jpeg data to our buffer, and dynamically re-allocates the buffer
// if it's not large enough.
// the intent is to KEEP one buffer, which will hit some maximal size, and so
// avoid memory fragmentation, and be a little faster.
// ocb(oarg, index, data, len);
size_t WcJpegEncoderStore_jpg_out_cb(void * arg, size_t index, const void* data, size_t len){
  struct PICSTORE *p = (struct PICSTORE *)arg; 
  if (p->allocatedLen < 0){
    return 0;
  }
  if (!p->buff){
    pic_alloc(p, 0, 0, DEFAULT_INITIAL_JPEG_LEN, PIXFORMAT_JPEG, 0);
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: jpeg alloced %d->%d at %X"), 0, p->allocatedLen, p->buff);
    if (!p->buff){
      return 0;
    }
  }

  if (index + len > p->allocatedLen){
    int oldlen = p->allocatedLen;
    // re-allocate buffer with memcoy of data
    pic_alloc(p, 0, 0, len + 2048, PIXFORMAT_JPEG, 2);
    if (!p->buff){
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: jpeg alloc failed to get %d"), p->allocatedLen);
      return 0;
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: jpeg re-alloced %d->%d at %X"), oldlen, p->allocatedLen, p->buff);
    }
  }

  if (p->buff + index + len > p->buff + p->allocatedLen ){
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: jpeg overlen??? %d+%d > %d at %X"), index, len, p->allocatedLen);
    return 0;
  }
  if (p->buff + index + len < p->buff ){
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: jpeg -ve index??? %d+%d < %d at %X"), index, len, 0);
    return 0;
  }

  memcpy(p->buff + index, data, len);
  // record len used
  p->len = index+len;
  return len;
}

// a jpeg encode which uses the above callback.
// used in creating jpegs from motion buffers,
// and possibly later for video if we want to get raw frame data for speed
// when we primarily want to process pixels (e.g. tensorflow).
// dest is filled if it returns true.
// DO NOT FREE BUFFER.
bool WcencodeToJpeg(uint8_t *src, size_t srclen, int width, int height, int format, uint8_t quality, struct PICSTORE *dest){
  dest->format = (int)PIXFORMAT_JPEG;
  bool converted = fmt2jpg_cb(src, srclen, width, height, (pixformat_t )format, quality, WcJpegEncoderStore_jpg_out_cb, (void *) dest);
  return converted && dest->buff;
}

void Wcencode_reset(struct PICSTORE *dest){
  pic_free(dest);
}
/*********************************************************************************************/




/*********************************************************************************************/


/*********************************************************************************************/


/*********************************************************************************************/

WC_Motion wc_motion;

void WcSetMotionDefaults(){
  wc_motion.motion_trigger_limit = 1000; // last amount of difference measured (~100 for none, > ~1000 for motion?)
  wc_motion.scale = 3;
  wc_motion.swscale = 0;
};

void HandleImagemotionmask(){
  HandleImageAny(wc_motion.mask);
}
void HandleImagemotiondiff(){
  HandleImageAny(wc_motion.diff);
}
void HandleImagemotionbuff(){
  HandleImageAny(wc_motion.frame);
}
void HandleImagemotionlbuff(){
  HandleImageAny(wc_motion.last_motion);
}
void HandleImagemotionbackgroundbuff(){
  HandleImageAny(wc_motion.background);
}

uint32_t WcSetMotionDetect(int32_t value) {
  if (value >= 0) { wc_motion.motion_detect = value; }
  if (!wc_motion.motion_detect){ // if turning it off...
    // don't free whilst buffer in use
    TasAutoMutex localmutex(&WebcamMutex, "HandleImage", 200);
    wc_motion.motion_state = 0;  // prevent set of output to stop bad detect at start
    pic_free_p(&wc_motion.frame);
    pic_free_p(&wc_motion.last_motion);
    pic_free_p(&wc_motion.diff);
    pic_free_p(&wc_motion.mask);
    pic_free_p(&wc_motion.background);

    wc_motion.motion_trigger = 0;
    wc_motion.motion_brightness = 0;
  }

  switch(value){
    case -1:
      return wc_motion.motion_trigger;
    case -2:
      return wc_motion.motion_brightness;
    case -20:
      WcMotionLog();
    break;
  }
  return value;
}

bool WcConvertFrame(int32_t bnum_i, int format, int scale) {
  if ((bnum_i < 0) || bnum_i >= MAX_PICSTORE) return false;
  if ((scale < 0) || scale > 3) return false;
  struct PICSTORE *ps = &Wc.picstore[bnum_i];
  if (!ps->buff) return false;
  // if jpeg decode
  bool res = false;
  if (ps->format == PIXFORMAT_JPEG && format != PIXFORMAT_JPEG) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ConvertFrame from JPEG to %d"), format);

    struct PICSTORE psout = {0};
    res = convertJpegToPixels(ps->buff, ps->len, ps->width, ps->height, scale, format, &psout);
    if (res) {
      free(ps->buff);
      memcpy(ps, &psout, sizeof(*ps));
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ConvertFrame failed %d,%d to %d at 1/%d"), ps->width, ps->height, format, (1<<scale));
    }
  } else {
    // must be jpeg encode
      // we don't support conversion excet to and from jpeg.
    if (format == PIXFORMAT_JPEG) {
      struct PICSTORE psout = {0};
      // will allocate just enough if > 16k required
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ConvertFrame from %d to jpeg"), (int)ps->format);
      res = WcencodeToJpeg(ps->buff, ps->len, ps->width, ps->height, (int)ps->format, 80, &psout);
      if (res) {
        free(ps->buff);
        memcpy(ps, &psout, sizeof(*ps));
      } else {
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ConvertFrame jpeg encode failed %d,%d from %d"), ps->width, ps->height, (int)ps->format);
      }
    } else {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: ConvertFrame cannot convert to %d"), (int)format);
    }
  }
  return res;
}



void WcMotionLog(){
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CAM: motion: w:%d h:%d scale:1/%d:1/%d ms:%u val:%d br: %d triggerpoint:%d, px10000:%d"), 
    (Wc.width/(1<<wc_motion.scale))/(1<<wc_motion.swscale), 
    (Wc.height/(1<<wc_motion.scale))/(1<<wc_motion.swscale), 
    (1<<wc_motion.scale), 
    (1<<wc_motion.swscale), 
    wc_motion.last_duration,
    wc_motion.motion_trigger, 
    wc_motion.motion_brightness, 
    wc_motion.motion_trigger_limit,
    wc_motion.changedPixelPertenthousand
  );
}


typedef struct {
        uint16_t width;
        uint16_t height;
        uint16_t data_offset;
        const uint8_t *input;
        struct PICSTORE *poutput;
} wc_rgb_jpg_decoder;

/*********************************************************************************************/
/* Lets go a little faster by using bgr, since we don't care about byte order                */
// from to_bmp.c - unfortunately thier version is static
unsigned int wc_jpg_read(void * arg, size_t index, uint8_t *buf, size_t len)
{
    wc_rgb_jpg_decoder * jpeg = (wc_rgb_jpg_decoder *)arg;
    if(buf) {
        memcpy(buf, jpeg->input + index, len);
    }
    return len;
}


// output buffer and image width
// this is to write macroblocks to the output.
// x,y,w,h are the jpeg numbers
// we ASSUME that the data presented to us is RGB888 - even for decode of a mono jpeg?
static bool _mono_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
    wc_rgb_jpg_decoder * jpeg = (wc_rgb_jpg_decoder *)arg;
    // called with null to start and end write.
    if(!data){
        if(x == 0 && y == 0){
            //write start
            jpeg->width = w;
            jpeg->height = h;
            if (jpeg->poutput){
              pic_alloc(jpeg->poutput, jpeg->width, jpeg->height, 0, PIXFORMAT_GRAYSCALE, 1);
            }
        } else {
            //write end
        }
        if (!jpeg->poutput || !jpeg->poutput->buff)
          return false;
        return true;
    }
    if (!jpeg->poutput || !jpeg->poutput->buff)
      return false;
    uint8_t *out = jpeg->poutput->buff + jpeg->data_offset;
    uint8_t *o = out;
    size_t djw = jpeg->width; // ouptut stride
    size_t dl = x; // offset into output image data for x

    // first pixel in destination
    o = out+(y*djw)+dl;
    // data already points to first pixel in source
    // and the start of the next line follows the end of the previous,
    // so no need to take into account stride

    // loop over each pixel, get a grey value, and put it in the output
    for(int iy = 0; iy < h; iy++) {
      uint8_t *op = o;
      for(int ix = 0; ix < w; ix++) {
        int32_t gray = (*(data++) + *(data++) + *(data++)) / 3;
        *(op++) = gray;
      }
      o += djw; // output stride
    }
    return true;
}

#ifdef WC_USE_RGB_DECODE      
//output buffer and image width
// from to_bmp.c - unfortunately thier version is static
static bool wc_rgb_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
    wc_rgb_jpg_decoder * jpeg = (wc_rgb_jpg_decoder *)arg;
    if(!data){
        if(x == 0 && y == 0){
            //write start
            jpeg->width = w;
            jpeg->height = h;
            if (jpeg->poutput){
              pic_alloc(jpeg->poutput, jpeg->width, jpeg->height, 0, PIXFORMAT_RGB888, 1);
            }
        } else {
            //write end
        }
        if (!jpeg->poutput || !jpeg->poutput->buff)
          return false;
        return true;
    }
    if (!jpeg->poutput || !jpeg->poutput->buff)
      return false;

    size_t jw = jpeg->width*3;
    size_t t = y * jw;
    size_t b = t + (h * jw);
    size_t l = x * 3;
    uint8_t *out = jpeg->poutput->buff + jpeg->data_offset;
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

static bool wc_rgb565_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data)
{
    wc_rgb_jpg_decoder * jpeg = (wc_rgb_jpg_decoder *)arg;
    if(!data){
        if(x == 0 && y == 0){
            //write start
            jpeg->width = w;
            jpeg->height = h;
            if (jpeg->poutput){
              pic_alloc(jpeg->poutput, jpeg->width, jpeg->height, 0, PIXFORMAT_RGB565, 1);
            }
        } else {
            //write end
        }
        if (!jpeg->poutput || !jpeg->poutput->buff)
          return false;
        return true;
    }
    if (!jpeg->poutput || !jpeg->poutput->buff)
      return false;

    // ###### TODO #####
    // I find this code highly suspect - copied from esp camdriver
    // seems they copied the RGB888 and adapted?
    size_t jw = jpeg->width*3;
    size_t jw2 = jpeg->width*2;
    size_t t = y * jw;
    size_t t2 = y * jw2;
    size_t b = t + (h * jw);
    size_t l = x * 2;
    uint8_t *out = jpeg->poutput->buff + jpeg->data_offset;
    uint8_t *o = out;
    size_t iy, iy2, ix, ix2;

    w = w * 3;

    for(iy=t, iy2=t2; iy<b; iy+=jw, iy2+=jw2) {
        o = out+iy2+l;
        for(ix2=ix=0; ix<w; ix+= 3, ix2 +=2) {
            uint16_t r = data[ix];
            uint16_t g = data[ix+1];
            uint16_t b = data[ix+2];
            uint16_t c = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
            o[ix2+1] = c>>8;
            o[ix2] = c&0xff;
        }
        data+=w;
    }
    return true;
}
#endif

// converts to a monochrome pixel array - quite fast
bool wc_jpg2mono(const uint8_t *src, size_t src_len, struct PICSTORE * out, int scale)
{
    wc_rgb_jpg_decoder jpeg;
    jpeg.width = 0;
    jpeg.height = 0;
    jpeg.input = src;
    jpeg.poutput = out;
    jpeg.data_offset = 0;

    if(esp_jpg_decode(src_len, (jpg_scale_t)scale, wc_jpg_read, _mono_write, (void*)&jpeg) != ESP_OK){
        return false;
    }
    return true;
}


#ifdef WC_USE_RGB_DECODE      
// converts to a 3x8 bit pixel array
// from to_bmp.c - unfortunately thier version is static
bool wc_jpg2rgb888(const uint8_t *src, size_t src_len, struct PICSTORE * out, int scale)
{
    wc_rgb_jpg_decoder jpeg;
    jpeg.width = 0;
    jpeg.height = 0;
    jpeg.input = src;
    jpeg.poutput = out;
    jpeg.data_offset = 0;

    if(esp_jpg_decode(src_len, (jpg_scale_t) scale, wc_jpg_read, wc_rgb_write, (void*)&jpeg) != ESP_OK){
        return false;
    }
    return true;
}


bool wc_jpg2rgb565(const uint8_t *src, size_t src_len, struct PICSTORE * out, int scale)
{
    wc_rgb_jpg_decoder jpeg;
    jpeg.width = 0;
    jpeg.height = 0;
    jpeg.input = src;
    jpeg.poutput = out;
    jpeg.data_offset = 0;

    if(esp_jpg_decode(src_len, (jpg_scale_t) scale, wc_jpg_read, wc_rgb565_write, (void*)&jpeg) != ESP_OK){
        return false;
    }
    return true;
}
#endif


// general jpeg to pixel conversion
// may be used for gettign pixels for other processing, e.g. tensorflow.
// supports scaling (0-3 -> 1:1. 1:2, 1:4, 1:8)
// supports pixelformats GRAYSCALE, RGB565, RGB888 (see define)
// pass in a camera_fb_t * and a buffer will be allocated/re-allocated if ->len != size required
bool convertJpegToPixels(const uint8_t *src_buf, size_t src_len, int width, int height, int scale, int format, struct PICSTORE *out){
  int size = 0;
  width = width / (1<<scale);
  height = height / (1<<scale);
  bool allocated = pic_alloc(out, width, height, 0, format, 1);
  if (!allocated){
    return false;
  }

  switch(format){
    case PIXFORMAT_GRAYSCALE:{ 
      return wc_jpg2mono(src_buf, src_len, out, scale);
    } break;
#ifdef WC_USE_RGB_DECODE      
    case PIXFORMAT_RGB565:{
      return wc_jpg2rgb565(src_buf, src_len, out, scale);
    } break;
    case PIXFORMAT_RGB888:{
      return wc_jpg2rgb888(src_buf, src_len, out, scale);
    } break;
#endif
    default: return false;
  }
}


/*********************************************************************************************/
// auto populate mask from diff image
void WcAutoMask(){
  //uint32_t auto_mask; // number of mootion detects to run automask over
  //uint8_t auto_mask_pixel_threshold; // pixel change threshold to add pixel to mask
  //uint8_t auto_mask_pixel_expansion; // number of pixels atound the detected pixel to set in mask (square)
  int width = Wc.width/(1<<wc_motion.scale);
  int height = Wc.height/(1<<wc_motion.scale);
  int swscalex = (1<<wc_motion.swscale);
  int scaledwidth = width/swscalex;
  int swscaley = (1<<wc_motion.swscale);
  int scaledheight = height/swscaley;

  if (!wc_motion.diff || !wc_motion.mask) return;

  uint8_t *pxdy = wc_motion.diff->buff;
  uint8_t *pxmy = wc_motion.mask->buff;
  uint8_t thresh = wc_motion.auto_mask_pixel_threshold;
  int expansion = wc_motion.auto_mask_pixel_expansion;
  int stride = scaledwidth;
  for (int y = 0; y < scaledheight; y++){
    uint8_t *pxd = pxdy + y*stride;
    for (int x = 0; x < scaledwidth; x++){
      uint8_t diff = *(pxd++);
      if (diff > thresh){
        for (int ym = y-expansion; ym < y + expansion; ym++){
          if (ym < 0) continue;
          if (ym >= scaledheight) break;
          for (int xm = x-expansion; xm < x + expansion; xm++){
            if (xm < 0) continue;
            if (xm >= scaledwidth) break;
            uint8_t *pxm = pxmy + ym*stride + xm;
            *pxm = 255;
          }
        }
      }
    }
  }

}


/*********************************************************************************************/
// motion detect routine.
// Wc.width and Wc.height must be set
// buffer is passed in
// if it fails to allocate, it will TURN OFF detection
void WcDetectMotionFn(uint8_t *_jpg_buf, int _jpg_buf_len){
  int width = Wc.width/(1<<wc_motion.scale);
  int height = Wc.height/(1<<wc_motion.scale);
  int pixelcount = width*height;
  int swscalex = (1<<wc_motion.swscale);
  int scaledwidth = width/swscalex;

  int swscaley = (1<<wc_motion.swscale);
  int scaledheight = height/swscaley;

  // ajdust to be on 8 pixel boundaries.
  //scaledwidth = ((scaledwidth+7)/8)*8;
  //scaledheight = ((scaledheight+7)/8)*8;
  int scaledpixelcount = scaledwidth*scaledheight;

  // if the frame changed size, reallocate
  uint32_t last_motion_buffer_len = (scaledpixelcount) + 4;

  bool newbuffers = false;

  if (!wc_motion.last_motion){
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: first motion buf?"));
    newbuffers = true;
  } else {
    if (last_motion_buffer_len != wc_motion.last_motion->len) {
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: motion buf size change now %d"), last_motion_buffer_len);
      newbuffers = true;
    }
  }

  // if diff enable changed
  if ((wc_motion.enable_diffbuff && !wc_motion.diff) ||
      (!wc_motion.enable_diffbuff && wc_motion.diff)){
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: diff enable now %d"), wc_motion.enable_diffbuff);
    newbuffers = true;
  }
  
  if ((wc_motion.enable_backgroundbuff && !wc_motion.background) ||
      (!wc_motion.enable_backgroundbuff && wc_motion.background)){
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: background enable now %d"), wc_motion.enable_backgroundbuff);
    newbuffers = true;
  }

  if ((wc_motion.enable_mask && !wc_motion.mask) ||
      (!wc_motion.enable_mask && wc_motion.mask)){
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: mask enable now %d"), wc_motion.enable_mask);
    newbuffers = true;
  }

  // detect change in scale and swscale as well as frame size in
  if (newbuffers){
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: motion - realloc"));
    // create and keep a frame buffers.
    wc_motion.required_motion_buffer_len = last_motion_buffer_len;
    wc_motion.motion_state = 0;  // prevent set of output to stop bad detect at start
    wc_motion.scaledwidth = scaledwidth;
    wc_motion.scaledheight = scaledheight;
    
    // allocate or keep if len still correct
    pic_alloc_p(&wc_motion.frame, width, height, 0, PIXFORMAT_GRAYSCALE, WC_ALLOC_ALWAYS);
    if (!wc_motion.frame || !wc_motion.frame->allocatedLen) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: motion: no allocate frame"));
      pic_free_p(&wc_motion.frame);
    }
    pic_alloc_p(&wc_motion.last_motion, scaledwidth, scaledheight, 0, PIXFORMAT_GRAYSCALE, WC_ALLOC_ALWAYS);
    if (!wc_motion.last_motion || !wc_motion.last_motion->allocatedLen) {
      AddLog(LOG_LEVEL_ERROR, PSTR("CAM: motion: no allocate last_motion"));
      pic_free_p(&wc_motion.last_motion);
    }
    if (wc_motion.enable_diffbuff){
      pic_alloc_p(&wc_motion.diff, scaledwidth, scaledheight, 0, PIXFORMAT_GRAYSCALE, WC_ALLOC_ALWAYS);
      if (!wc_motion.diff || !wc_motion.diff->allocatedLen) {
        AddLog(LOG_LEVEL_ERROR, PSTR("CAM: motion: no allocate diff"));
        pic_free_p(&wc_motion.diff);
      }
    } else {
      pic_free_p(&wc_motion.diff);
    }
    if (wc_motion.enable_backgroundbuff){
      pic_alloc_p(&wc_motion.background, scaledwidth, scaledheight, 0, PIXFORMAT_GRAYSCALE, WC_ALLOC_ALWAYS);
      if (!wc_motion.background || !wc_motion.background->allocatedLen) {
        AddLog(LOG_LEVEL_ERROR, PSTR("CAM: motion: no allocate background"));
        pic_free_p(&wc_motion.background);
      }
    } else {
      pic_free_p(&wc_motion.background);
    }
    if (wc_motion.enable_mask){
      pic_alloc_p(&wc_motion.mask, scaledwidth, scaledheight, 0, PIXFORMAT_GRAYSCALE, WC_ALLOC_ALWAYS);
      if (!wc_motion.mask || !wc_motion.mask->allocatedLen) {
        AddLog(LOG_LEVEL_ERROR, PSTR("CAM: motion: no allocate mask"));
        pic_free_p(&wc_motion.mask);
      }
    } else {
      pic_free_p(&wc_motion.mask);
    }
  }

  // every time, not just on allocation failure
  if (!wc_motion.frame || !wc_motion.last_motion) {
    // indicate failure
    wc_motion.motion_trigger = 0;
    wc_motion.motion_brightness = 0;
    // and maybe signal via berry
    wc_motion.motion_processed = 1;
    return;
  }

  // enable us to call with null just to allocate buffers
  if (!_jpg_buf){
    return;
  }

  uint32_t start = millis();

  // both buffers are valid if we get here
  bool jpegres;

  /*JPG_SCALE_NONE,    JPG_SCALE_2X,    JPG_SCALE_4X,    JPG_SCALE_8X,*/
  int scale = wc_motion.scale;
  // convert the input jpeg (full size)
  // to a mono using jpeg decoder scaling to save memory
  jpegres = wc_jpg2mono(_jpg_buf, _jpg_buf_len, wc_motion.frame, scale);
  if (!jpegres){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: motion: jpeg decode failure"));
    wc_motion.motion_trigger = 0;
    wc_motion.motion_brightness = 0;
    // and maybe signal via berry
    wc_motion.motion_processed = 1;
    return;
  }

  // setup pixel pointers
  uint8_t *pxiy = wc_motion.frame->buff;
  uint8_t *pxry = wc_motion.last_motion->buff;

  // optional difference buffer - may be nullptr unless enabled
  uint8_t *pxdy = nullptr;
  if (wc_motion.diff && wc_motion.diff->buff) pxdy = wc_motion.diff->buff;
  // optional backkground buffer - may be nullptr unless enabled
  uint8_t *pxby = nullptr;
  if (wc_motion.background && wc_motion.background->buff) pxdy = wc_motion.background->buff;
  // optional mask buffer - may be nullptr unless enabled
  uint8_t *pxmy = nullptr;
  if (wc_motion.mask && wc_motion.mask->buff) pxmy = wc_motion.mask->buff;

  // uint32 will handle up to 4096x4096x8bit
  uint32_t accu = 0;
  uint32_t bright = 0;
  uint8_t thresh = wc_motion.pixelThreshold;
  uint32_t changedPixelCount = 0;

  // for unscaled, a simple loop over total length, maybe marginally faster
  if (wc_motion.frame->len == wc_motion.last_motion->len){
    uint8_t *pxi = pxiy;
    uint8_t *pxr = pxry;
    uint8_t *pxd = pxdy; // may be nullptr;
    uint8_t *pxb = pxby; // may be nullptr;
    uint8_t *pxm = pxmy; // may be nullptr;
    for (int i = 0; i < wc_motion.frame->len; i++){
      // if we have a mask, and the mask pixel value > 20, then ignore this pixel
      uint8_t gray = *pxi;
      if (pxm && (*pxm > 20)) {
        if (pxb) {
          if (wc_motion.capture_background){
            *pxb = *pxi;
          }
          pxb++;
        }
        pxi++;
        *(pxr++) = gray; // set background regardless
        pxm++;
        if (pxd) {
          *(pxd++) = 0; // clear diff
        }
      } else {
        uint8_t diff;
        if (pxb){
          diff = abs((int)(*pxi) - (int)(*pxb));
          if (wc_motion.capture_background){
            *pxb = *pxi;
          }
          pxb++;
        } else {
          diff = abs((int)(*pxi) - (int)(*pxr));
        }
        *(pxr++) = gray;
        pxi++;
        accu += diff;
        // store difference image
        // look at pixel threshold if configured
        if (thresh && diff > thresh){
          changedPixelCount++;
          if (pxd) diff = 255;
        }
        if (pxd) *(pxd++) = diff;
        if (pxm) pxm++;
      }
      bright += gray;
    }
  } else {
    uint32_t x, y;
    // for softare scaled, a silightly more complex loop.
    int xincrement = swscalex;
    int yincrement = swscaley;
    int stride = yincrement*width;
    // sample half way down each scaled line, not at the top.
    if (yincrement > 2){
      pxiy += stride*(yincrement/2);
    }
    for (y = 0; y < scaledheight; y++) {
      uint8_t *pxi = pxiy + y*stride;
      uint8_t *pxr = pxry + y*scaledwidth;
      uint8_t *pxd = nullptr;
      uint8_t *pxb = nullptr;
      uint8_t *pxm = nullptr;
      if (pxdy) pxd = pxdy + y*scaledwidth;
      if (pxby) pxb = pxby + y*scaledwidth;
      if (pxmy) pxm = pxmy + y*scaledwidth;
      for (x = 0; x < scaledwidth;x ++) {
        int32_t gray = *pxi;
        if (pxm && (*pxm > 20)) {
          if (pxb) {
            if (wc_motion.capture_background){
              *pxb = gray;
            }
            pxb++;
          }
          pxi += xincrement;
          *(pxr++) = gray;
          pxm++;
          if (pxd) *(pxd++) = 0; // clear diff
        } else {
          uint8_t diff;
          if (pxb){
            diff = abs((int)(gray) - (int)(*pxb));
            if (wc_motion.capture_background){
              *pxb = gray;
            }
            pxb++;
          } else {
            diff = abs((int)(gray) - (int)(*pxr));
          }
          *(pxr++) = gray;
          pxi += xincrement;
          accu += diff;

          // look at pixel threshold if configured
          if (thresh && diff > thresh){
            changedPixelCount++;
            if (pxd) diff = 255;
          }
          // store difference image
          if (pxd) *(pxd++) = diff;
          if (pxm) pxm++;
        }
        bright += gray;
      }
    }
  }

  // we only capture background once when asked to by this flag
  wc_motion.capture_background = 0;

  // when scaledpixelcount is < 100, float becomes necessary
  float divider = (((float)scaledpixelcount) / 100.0);

  wc_motion.motion_brightness = (int)((float)bright / divider);
  wc_motion.changedPixelPertenthousand = (int)((float)changedPixelCount / divider);

  if (wc_motion.motion_state){
    wc_motion.motion_trigger = (int)((float)accu / divider);
    if (wc_motion.motion_trigger > wc_motion.motion_trigger_limit){
      wc_motion.motion_triggered = 1;
    }

    if (wc_motion.pixel_trigger_limit && wc_motion.changedPixelPertenthousand > wc_motion.pixel_trigger_limit){
      wc_motion.motion_triggered = 1;
    }

  } else {
    // first run, 
    wc_motion.motion_state = 1;
    wc_motion.motion_trigger = 0;
  }

  // trigger Berry calling webcam.motion if it exists
  wc_motion.motion_processed = 1;

  if (wc_motion.enable_mask && wc_motion.auto_mask > 0){
    WcAutoMask();
    wc_motion.auto_mask--;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: motion: auto_mask %d"), wc_motion.auto_mask);
  }
  uint32_t end = millis();

  wc_motion.last_duration = end - start;
  WcMotionLog();
}

/*********************************************************************************************/


// NOTE input format is esp format + 1, and 0 -> jpeg
// wcConvertFrame1-4 0 [0] - option arg scale 0-3 -> valid on jpeg decode only
void CmndWebcamConvertFrame(void){
  int bnum = XdrvMailbox.index;
  // bnum is 1-4
  if ((bnum < 1) || (bnum > MAX_PICSTORE)){
    return;  // Command Error
  }
  int format = 0;
  int scale = 0;

  if(XdrvMailbox.data_len){
    char tmp[20];
    strncpy(tmp, XdrvMailbox.data, 10);
    char *arg = strtok(tmp, " ");
    format = atoi(arg);
    arg = strtok(nullptr, " ");
    if (arg){
      scale = atoi(arg);
    }
  }

  // NOTE input format is esp format + 1, and 0 -> jpeg
  if (!format){
    format = PIXFORMAT_JPEG;
  } else {
    format--;
  }
  if (!wc_check_format(format)){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid format %d"), format+1);
    return;  // Command Error
  }
  struct PICSTORE *ps = &Wc.picstore[bnum-1];
  if (!ps->buff){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: No pic at %d"), bnum);
    return;  // Command Error
  }
  if (ps->format != PIXFORMAT_JPEG && format != PIXFORMAT_JPEG){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ConvertFrame only go to or from JPEG"));
    return;  // Command Error
  }

  // takes INDEX into store
  bool res = WcConvertFrame(bnum-1, format, scale);
  if (res) {
    ResponseCmndDone();
  }
}

// Allows Berry to send native address, len, format, optional width, height
// "addr len format [width height]"
// give it a bad address, and it WILL die.
void CmndWebcamSetPicture(void){
  int bnum = XdrvMailbox.index;
  if (!XdrvMailbox.data_len || bnum < 1 || bnum > MAX_PICSTORE) {
    return;  // Command Error
  }
  struct PICSTORE *p = &Wc.picstore[bnum-1];

  char tmp[100];
  strncpy(tmp, XdrvMailbox.data, 99);
  // "addr len format [width height]"
  // width/height if format not PIXFORMAT_JPEG=0/5
  // allowed formats 
  int format = 0;
  uint32_t addr = 0;
  int len = 0;
  int height = 0;
  int width = 0;

  int res = sscanf(tmp, "%u %d %d %d %d",
    &addr, &len, &format, &height, &width);

  if (!format){
    format = PIXFORMAT_JPEG;
  } else {
    format--;
  }

  if (res < 2){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: SetPicture expects 'addr len format [width height]'"));
    return;  // Command Error
  }
  if (!wc_check_format(format)){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid format %d"), format+1);
    return;  // Command Error
  }
  if (format != PIXFORMAT_JPEG && (!width || !height)){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: SetPicture: format %d needs width and height"), format+1);
    return;  // Command Error
  }

  bool allocres = pic_alloc(p, width, height, len, format, 1);
  if (!allocres){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: SetPicture alloc failed"));
    return;  // Command Error
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: SetPicture addr:%u len:%d format%d [width%d height%d]"), addr, len, format, width, height);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: dest addr:%u len:%d/%d format%d [width%d height%d]"), p->buff, p->len, p->allocatedLen, p->format, p->width, p->height);

  // don't over copy if someone screws up height/width/size calc
  // also, our buffer MAY have more space than required...
  int copylen = (len < p->allocatedLen)?len:p->allocatedLen;
  // copy Berry data.  We can't free it, and Berry will
  memcpy(p->buff, (void *)addr, copylen);
  ResponseCmndDone();
}


// so that we can test scripting functions
void CmndWebcamSetMotionDetect(void) {
  int res = 0;
  // returns stuff if in is -ve?
  switch(XdrvMailbox.index){
    case 1:
      // original features
      res = WcSetMotionDetect(XdrvMailbox.payload);
      break;
    case 2:
      res = Wc.lenDiffLimit = XdrvMailbox.payload;
      break;
    case 3:
      if (XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 255){
        wc_motion.pixelThreshold = XdrvMailbox.payload;
      }
      res = wc_motion.pixelThreshold;
      break;
    case 4:
      if (XdrvMailbox.payload >= 0){
        wc_motion.pixel_trigger_limit = XdrvMailbox.payload;
      }
      res = wc_motion.pixel_trigger_limit;
      break;
    case 5: {
      int scale = wc_motion.scale;
      if (scale == 3) scale += wc_motion.swscale;
      if (XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 7){
        scale = XdrvMailbox.payload;
        if (scale < 0) scale = 0;
        if (scale > 7) scale = 7;
        if (scale <= 3){
          wc_motion.scale = scale;
          wc_motion.swscale = 0;
        } else {
          wc_motion.scale = 3;
          wc_motion.swscale = scale - 4;
        }
      }
      res = scale;
    } break;
    case 6: // enable use of a difference frame - readable
      if (XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 1){
        wc_motion.enable_diffbuff = XdrvMailbox.payload & 1;
      }
      res = wc_motion.enable_diffbuff;
      break;
    case 7: // ammount of changed picture (accumulated diff)
      if (XdrvMailbox.payload >= 0){
        wc_motion.motion_trigger_limit = XdrvMailbox.payload;
      }
      res = wc_motion.motion_trigger_limit;
      break;
    case 8:{ // set mask feature.  must be done AFTER setting resolution or scale...
      int auto_mask_count = 1;
      int auto_mask_pixel_threshold = 10;
      int auto_mask_pixel_expansion = 4;

      if (0 == XdrvMailbox.data_len) {
        res = wc_motion.enable_mask? 1:0;
        break;
      } else {
        char tmp[40];
        strncpy(tmp, XdrvMailbox.data, 10);
        char *p = tmp;
        char *arg = strtok(tmp, " ");
        auto_mask_count = atoi(arg);
        arg = strtok(nullptr, " ");
        if (arg){
          auto_mask_pixel_threshold = atoi(arg);
          arg = strtok(nullptr, " ");
          if (arg){
            auto_mask_pixel_expansion = atoi(arg);
          }
        }
      }

      if (!auto_mask_count){
        wc_motion.enable_mask = 0;
        res = 0;
        break;
      }
      if (!wc_motion.enable_mask){
        TasAutoMutex localmutex(&WebcamMutex, "setMotionDetect", 30000);
        // force buffer allocation/length calc
        if (auto_mask_count > 1){
          wc_motion.enable_mask = 1;
          wc_motion.enable_diffbuff = 1; // enable the diff buff, we use if for automask
        }
        // force buffer allocation now
        WcDetectMotionFn(nullptr, 0);
      }

      if (wc_motion.mask && wc_motion.mask->buff){
        memset(wc_motion.mask->buff, 0, wc_motion.mask->len);
        if (XdrvMailbox.payload > 1){
          // if 2+, then represents count of motion detects to make mask from
          wc_motion.auto_mask = auto_mask_count; 
          wc_motion.auto_mask_pixel_threshold = auto_mask_pixel_threshold;
          wc_motion.auto_mask_pixel_expansion = auto_mask_pixel_expansion;
        }
        res = auto_mask_count;
      } else {
        AddLog(LOG_LEVEL_ERROR, PSTR("CAM: motion: unable to allocate mask buffer"));
        res = 0;
      }
    } break;
    case 9: // enable use of a background frame - readable
      // and trigger capture of next wc_motion image into background
      if (XdrvMailbox.payload >= 0 && XdrvMailbox.payload <= 1){
        wc_motion.enable_backgroundbuff = XdrvMailbox.payload & 1;
        wc_motion.capture_background = 1;
      }
      res = wc_motion.enable_diffbuff;
      break;
  }
  ResponseCmndNumber(res);
}

// wcGetMotionPixels1-n [1-n]
// if optional second argument is given the picture is copied to that picstore.
// so making it easy to convert/save.
void CmndWebcamGetMotionPixels(void) {
  // NOTE: the buffers returned here are static unless the frame size or scale changes.
  // use with care
  int width = Wc.width/(1<<wc_motion.scale);
  int height = Wc.height/(1<<wc_motion.scale);
  int swscalex = (1<<wc_motion.swscale);
  int scaledwidth = width/swscalex;
  int swscaley = (1<<wc_motion.swscale);
  int scaledheight = height/swscaley;

  int bnum = -1;
  if (-99 != XdrvMailbox.payload){
    bnum = XdrvMailbox.payload;
    if (bnum < 1 || bnum > MAX_PICSTORE) {
      return;  // Command Error
    }
  }

  uint8_t *t = nullptr;
  int len = 0;
  int format = 0;
  struct PICSTORE *p = nullptr;
  switch (XdrvMailbox.index){
    case 1:{
      p = wc_motion.last_motion;
    } break;
    case 2:{ // optional diff buffer
      p = wc_motion.diff;
    } break;
    case 3:{ // optional mask buffer
      p = wc_motion.mask;
    } break;
    case 4:{ // optional background buffer
      p = wc_motion.background;
    } break;
  }

  if (!p){
    return;  // Command Error
  }

  if (bnum > 1){
    bool res = pic_alloc(&Wc.picstore[bnum-1], p->width, p->height, 0, p->format, WC_ALLOC_ALWAYS);
    if (res){
      memcpy(Wc.picstore[bnum-1].buff, p->buff, p->len);
      p = &Wc.picstore[bnum-1];
    } else {
      return;  // Command Error
    }
  }

  char resp[100] = "0";
  snprintf_P(resp, sizeof(resp), PSTR("{\"buff\":%d,\"addr\":%d,\"len\":%d,\"w\":%d,\"h\":%d,\"format\":%d}"), 
      bnum, p->buff, p->len, p->width, p->height, p->format+1);
  Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, resp);
}

#endif  // !USE_WEBCAM_LEGACY
#endif  // USE_WEBCAM_MOTION
#endif  // USE_WEBCAM
#endif  // ESP32
