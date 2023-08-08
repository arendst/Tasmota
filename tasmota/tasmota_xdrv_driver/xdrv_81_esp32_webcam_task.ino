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
#ifndef USE_WEBCAM_LEGACY

#define WC_USE_RGB_DECODE

/*********************************************************************************************\
 * ESP32 webcam based on example in Arduino-ESP32 library
 *
 * Template as used on ESP32-CAM WiFi + bluetooth Camera Module Development Board ESP32 With Camera Module OV2640 Geekcreit for Arduino
 * {"NAME":"AITHINKER CAM","GPIO":[4992,1,672,1,416,5088,1,1,1,6720,736,704,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,1,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":2}
 *
 * Supported commands:
 * WcInterrupt     = Control streaming, 0 = stop, 1 = start
 * WcResolution = Set resolution (max framerates at wcclock=20)
 * FPS quoted is (max, withonewebclient) at wcclock=20 on AIThinker esp32cam equivalent
 * FPS is mostly limited by sending data to webclients & rtsp, not by the camera.
 0 = FRAMESIZE_96X96,    // 96x96 (50fps, ~48fps) - at wcclock=30 (75, ~53)
 1 = FRAMESIZE_QQVGA,    // 160x120 (50fps ~47fps)
 2 = FRAMESIZE_QCIF,     // 176x144 (50fps ~47fps)
 3 = FRAMESIZE_HQVGA,    // 240x176 (50fps ~37fps?)
 4 = FRAMESIZE_240X240,  // 240x240 (50fps ~39fps)
 5 = FRAMESIZE_QVGA,     // 320x240 (50fps ~35fps)
 6 = FRAMESIZE_CIF,      // 400x296 (50fps ~31fps)
 7 = FRAMESIZE_HVGA,     // 480x320 (25fps ~24fps)
 8 = FRAMESIZE_VGA,      // 640x480 (25fps ~19fps)
 9 = FRAMESIZE_SVGA,     // 800x600 (25fps ~15.5fps)
 10 = FRAMESIZE_XGA,      // 1024x768 (12.5fps, ~10fps)
 11 = FRAMESIZE_HD,       // 1280x720 (12.5fps, ~9fps)
 12 = FRAMESIZE_SXGA,     // 1280x1024 (12.5fps, ~7.5fps)
 13 = FRAMESIZE_UXGA,     // 1600x1200 (12.5fps, ~5fps)
 // 3MP Sensors above this no yet supported with this driver
 14 = FRAMESIZE_FHD,      // 1920x1080
 15 = FRAMESIZE_P_HD,     //  720x1280
 16 = FRAMESIZE_P_3MP,    //  864x1536
 17 = FRAMESIZE_QXGA,     // 2048x1536
 // 5MP Sensors
 18 = FRAMESIZE_QHD,      // 2560x1440
 19 = FRAMESIZE_WQXGA,    // 2560x1600
 20 = FRAMESIZE_P_FHD,    // 1080x1920
 21 = FRAMESIZE_QSXGA,    // 2560x1920
 22 = FRAMESIZE_INVALID

Framerate: I found that achievable streamed framerate is VERY dependent on wifi.
e.g. latest BT router, but with android screensharing with TV, and framerate for resolution 0 was scatty between 20 and 40fps!!!!
As soon as the screenshare ended, back to 53fps at 30mhz clock.

 * WcMirror     = Mirror picture, 0 = no, 1 = yes
 * WcFlip       = Flip picture, 0 = no, 1 = yes
 * WcSaturation = Set picture Saturation -2 ... +2
 * WcBrightness = Set picture Brightness -2 ... +2
 * WcContrast   = Set picture Contrast -2 ... +2
 * WcSpecialEffekt = Set Special Picture Effect: 0 = off, 1 = , 2 = , 3 = , 4 = , 5 = , 6 =
 * WcAWB          = Auto White Balance, 0 = no, 1 = yes
 * WcWBMode       = White Balance Mode, 0 = auto, 1 =
 * WcAWBGain      = Auto White Balance Gain, 0 = no, 1 = yes
 * WcAEC          = Auto exposure control (Sensor), 0 = no, 1 = yes
 * WcAECDSP       = Auto exposure control (DSP), 0 = no, 1 = yes
 * WcAECValue     = Auto exposure control value, 0 ... 1024
 * WcAECLevel     = Auto exposure control level, -2 ... +2
 * WcAGC          = Auto gain control, 0 = no, 1 = yes
 * WcAGCGain      = Auto gain control gain, 0 .. 30
 * WcGainCeiling  = Gain ceiling, 0 .. 6 (0 = x2, 1 = x4, 2 = x8, 3 = x16, 4 = x32, 5 = x64, 6 = x128)
 * WcGammaCorrect = Auto Gamma Correct, 0 = no, 1 = yes
 * WcLensCorrect  = Auto Lens Correct, 0 = no, 1 = yes
 * WcWPC          = White Pixel Correct, 0 = no, 1 = yes
 * WcDCW          = Downscale, 0 = no, 1 = yes
 * WcBPC          = Black Pixel Correct, 0 = no, 1 = yes
 * WcColorbar     = Show Colorbar, 0 = no, 1 = yes
 * WcFeature      = Set extended Feature, 0 = off, 1 = reduce FPS, 2 = Nightmode
 * WcStats        = Show Statistics
 * WcInit         = Init Camera Interface
 * WcRtsp         = Control RTSP Server, 0=disable, 1=enable (forces restart) (if defined ENABLE_RTSPSERVER)

 * WcGetframeN    = read a picture from camera and store in slot N - return {"addr":123456,"len":12345,"w":160,"h":120, "format":5}
 * WcSavepicN     = save a picture 1-4 after WcGetframe or WcSavepic0 to get frame 1 and save. argument is filename
 * WcAppendpicN   = append a picture 1-4 to a file after WcGetframe or WcSavepic0 to get frame 1 and save. argument is filename
 * WcGetpistoreN  = read buff, addr, len of a stored image, returns json (e.g. for berry) {"addr":123456,"len":12345,"w":160,"h":120, "format":5}
 *    if N==0 take a picture to 1 first.
 * WcGetmotionpixelsN = (N=1..4) read addr, len, w, h as JSON {"addr":123456,"len":12345,"w":160,"h":120, "format":4} 
 *    motion(1)
 *    difference(2) buffer - e.g for berry
 *    mask(3)
 *    background(4)
 *     e.g. could be used to read pixels, or change pixels from berry.

 * WcMenuvideoon   = turn on video in main menu
 * WcMenuvideooff  = turn off video in main menu

 * WcSetOptionsNN  = call WcSetOptions function with (index, payload).
 *   - can oprate many of the functions above, plus:
 *     WcSetoptions24 <frames to skip at input> - e.g. WcSetoptions24 2 will give you only every 3rd frame
 *     WcSetoptions25 <camPixelFormat> - espcam format + 1.  0->default->JPEG.   1:2BPP/RGB565, 2:2BPP/YUV422, 3:1.5BPP/YUV420, 4:1BPP/GRAYSCALE 5:JPEG/COMPRESSED 6:3BPP/RGB888 7:RAW 8:3BP2P/RGB444 9:3BP2P/RGB555
 *        Maybe useful for tensorflow or similar?
 *        ***** I COULD NOT GET THIS TO WORK ON MY AiThinker style cam ******

 * WcConvertFrameN <format> <scale> - convert a wcgetframe in picstore from jpeg to <format> (0=2BPP/RGB565, 3=1BPP/GRAYSCALE, 5=3BPP/RGB888), <scale> (0-3)
 *     converts in place, replacing the stored frame with the new format.  Data can be retrieved using wcgetpicstoreN (e.g. for use in berry)
 *     will fail if it can't convert or allocate.
 * 
 * WcPowerOff - power down camera.  WcInit will bring it back up.
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
http://IP/wc.jpg - cam picture
http://IP/wc.mjpeg - cam picture
http://IP/snapshot.jpg - cam picture

http://IP:81/diff.mjpeg - motion or difference buffer stream as mjpeg (fully scaled, buf is motion if diff not enabled) - if enabled, else no data
http://IP:81/cam.mjpeg - cam stream.
http://IP:81/cam.jpg - cam stream.
http://IP:81/stream - cam stream.


Both webstream and RTSP can handle multiple clients.
Multiple clients will reduce the framerate.
Wifi use has a big effect on framerate.


 *
 * WcFeature Explanation
 * See https://hobbylad.files.wordpress.com/2020/02/ov2640-camera-module-software-application-notes.pdf
 * for more Information
 * 0: Normal Operation
 * 1: Reduced FPS: Set XCLK Divisor to 2. Reduces Framerate and also increases exposure time. This causes
 *    better low light performance. See above document, page 6
 * 2: Night Mode: Further increase exposure time and lower the Framerate depending on available light.
 *    See above Document, Page 8
 *
 * Only boards with PSRAM should be used.
 * To speed up cam processing cpu frequency should be better set to 240Mhz
 *
 * remarks for AI-THINKER
 * GPIO0 zero must be disconnected from any wire after programming because this pin drives the cam clock and does
 * not tolerate any capictive load
 * the AITHINKER module does not have CAM_RESET - so if you get the camera into a bad state, power off restart is the only way out.
 * flash led = gpio 4
 * red led = gpio 33
 * optional rtsp url: rtsp://xxx.xxx.xxx.xxx:8554/mjpeg/1
 * 
 * SH 2023-05-14 - added mutex for many webcam functions - this is to prevent multi-threaded access to the camera functions, which 
 * can case error 0x105 upon re-init.
 * Errors 0x103 and 0xffffffff could indicate CAM_PWDN incorrect.
 * Error 0x105 is likely caused by the camera not being powered long enough before init.
 * 
 * I2C use: if USE_I2C is enabled, you can set GPIO26 to I2c_SDA/2 and GPIO27 to I2C_SCL/2, and then use the shared I2C bus 2.
 * Then you can use cmd i2cscan2 to check for camera presence.
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

New commands:
WcGetFrame <n> or WcGetFrame<n> - reads the next frame into slot n (1-4)
WcSavePic<n> fname
WcAppendPic<n> fname
(n is 0-4) - 0 will trigger a read of a picture now,
These will save or append a picture to a file.  The picture must have been first read with WcGetFrame <n> if n > 0


*/
/*********************************************************************************************/



#define XDRV_99           99

#include "cam_hal.h"
#include "esp_camera.h"
#include "sensor.h"
#include "fb_gfx.h"
#include "camera_pins.h"
#include "esp_jpg_decode.h"
//#include "img_converters.h"

#ifdef USE_UFILESYS
extern FS *ufsp;
extern FS *ffsp;
#endif

bool HttpCheckPriviledgedAccess(bool);
extern ESP8266WebServer *Webserver;

SemaphoreHandle_t WebcamMutex = nullptr;




// these are the frame intervals at 20mhz
// empirically, they directly scale with frequency.
// for 30mhz, 50fps -> 75fps; for 15 mhz, 37.5fps, 
// Q: what happens at 30 mains?
const int nativeIntervals20ms[] = {
    20, //0 = FRAMESIZE_96X96,    // 96x96 (50fps, ~48fps) - at wcclock=30 (75, ~53)
    20, //1 = FRAMESIZE_QQVGA,    // 160x120 (50fps ~47fps)
    20, //2 = FRAMESIZE_QCIF,     // 176x144 (50fps ~47fps)
    20, //3 = FRAMESIZE_HQVGA,    // 240x176 (50fps ~37fps?)
    20, //4 = FRAMESIZE_240X240,  // 240x240 (50fps ~39fps)
    20, //5 = FRAMESIZE_QVGA,     // 320x240 (50fps ~35fps)
    20, //6 = FRAMESIZE_CIF,      // 400x296 (50fps ~31fps)
    20, //7 = FRAMESIZE_HVGA,     // 480x320 (25fps ~24fps)
    40, //8 = FRAMESIZE_VGA,      // 640x480 (25fps ~19fps)
    40, //9 = FRAMESIZE_SVGA,     // 800x600 (25fps ~15.5fps)
    80, //10 = FRAMESIZE_XGA,      // 1024x768 (12.5fps, ~10fps)
    80, //11 = FRAMESIZE_HD,       // 1280x720 (12.5fps, ~9fps)
    80, //12 = FRAMESIZE_SXGA,     // 1280x1024 (12.5fps, ~7.5fps)
    80, //13 = FRAMESIZE_UXGA,     // 1600x1200 (12.5fps, ~5fps)
};


// use mutex like:
// TasAutoMutex localmutex(&WebcamMutex, "somename", 200);
// in any function.  Will wait for mutex to be clear, and auto-release when the function exits.

#define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"

#ifndef MAX_PICSTORE
#define MAX_PICSTORE 4
#endif

// our basic picture store
// we use this both in wc_picstore and elsewhere
struct PICSTORE {
  uint8_t * buff;              /*!< Pointer to the pixel data */
  size_t len;                 /*!< Length of the buffer in bytes */
  uint16_t width;               /*!< Width of the buffer in pixels */
  uint16_t height;              /*!< Height of the buffer in pixels */
  int8_t format;         /*!< Format of the pixel data */
  struct timeval timestamp;   /*!< Timestamp since boot of the first DMA buffer of the frame */  
  int allocatedLen; // -1 or 0 indicates failure to allocate
};

#ifdef ENABLE_RTSPSERVER

#include <OV2640.h>
#include <SimStreamer.h>
#include <OV2640Streamer.h>
#include <CRtspSession.h>
#ifndef RTSP_FRAME_TIME
#define RTSP_FRAME_TIME 100
#endif // RTSP_FRAME_TIME

// RTSP streamer class
class localOV2640Streamer : public CStreamer {
    BufPtr f_ptr; // temp pointer to frame buffer
    int f_len;
public:
    localOV2640Streamer(SOCKET aClient, int width, int height);
    void setframe(BufPtr ptr, int len);
    void clearframe();
    virtual void    streamImage(uint32_t curMsec);
};

localOV2640Streamer::localOV2640Streamer(SOCKET aClient, int width, int height) : CStreamer(aClient, width, height) {
  clearframe();
  AddLog(LOG_LEVEL_DEBUG,PSTR("CAM: Created RTSP streamer width=%d, height=%d"), width, height);
}
void localOV2640Streamer::setframe(BufPtr ptr, int len) {
  f_ptr = ptr;
  f_len = len;
}
void localOV2640Streamer::clearframe() {
  f_ptr = nullptr;
  f_len = 0;
}
void localOV2640Streamer::streamImage(uint32_t curMsec){
  if (!f_ptr) return;
  streamFrame(f_ptr, f_len, curMsec);
  //AddLog(LOG_LEVEL_DEBUG,PSTR("CAM: RTSP Stream Frame %d"), f_len);
}

typedef struct tag_wc_rtspclient {
  localOV2640Streamer * volatile camStreamer;
  CRtspSession * volatile rtsp_session;
  WiFiClient rtsp_client;
  tag_wc_rtspclient * volatile p_next;
} wc_rtspclient;
#endif //ENABLE_RTSPSERVER

typedef struct tag_wc_client {
  WiFiClient client;
  tag_wc_client *p_next;
  uint8_t active;
  uint8_t type;
} wc_client;

struct {
  // status variables
  uint8_t  up = 0;
  uint8_t  psram = 0; // indicates if we found psram or not. Suspect always true for esp32cam modules
  volatile uint16_t width; // last JPEG frame height received or 0
  volatile uint16_t height; // last JPEG frame height received or 0
  volatile uint32_t last_frame_len; // last JPEG frame length or 0
  volatile int8_t  frame_processed; // set when a frame is finished processing
  unsigned int lastCamError; // set to the last cam error, or 0
  volatile uint32_t camtimediff; // last camtime interval
  volatile uint32_t frameIntervalsus; // expected cam interval in us
  volatile uint16_t lenDiff;
  volatile uint16_t lenDiffTriggered;
  volatile uint8_t  lenDiffTrigger;

  // control variables
  volatile int8_t  disable_cam; // set to 1 when TAS is busy doning something and has turned the camera off
  volatile int8_t  taskRunning; // 0 = not started, 1 = running, 2 = request stop
  volatile int8_t  taskGetFrame; // set to n to trigger capture of n frames to picstore
  volatile int8_t  taskTakePic; // set to n to trigger capture of n frames to picstore
  uint8_t  berryFrames;
  volatile uint16_t lenDiffLimit;
  volatile int32_t skipFrames;
  /* camPixelFormat is format +1, i.e.
  0->4->PIXFORMAT_JPEG
  1->0->PIXFORMAT_RGB565,    // 2BPP/RGB565 - valid?
  2->1->PIXFORMAT_YUV422,    // 2BPP/YUV422 - valid?
  3->2->PIXFORMAT_YUV420,    // 1.5BPP/YUV420 - invalid?
  4->3->PIXFORMAT_GRAYSCALE, // 1BPP/GRAYSCALE - valid?
  5->4->PIXFORMAT_JPEG,      // JPEG/COMPRESSED - valid
  6->5->PIXFORMAT_RGB888,    // 3BPP/RGB888 - invalid?
  7->6->PIXFORMAT_RAW,       // RAW - invalid?
  8->7->PIXFORMAT_RGB444,    // 3BP2P/RGB444 - invalid?
  9->8->>PIXFORMAT_RGB555,    // 3BP2P/RGB555 - invalid?
  */
  volatile uint8_t  camPixelFormat; // 

  // our (separate) webserver on port 81
  ESP8266WebServer *CamServer;
  // pointer to the first http streaming client in a list of multiple clients, or nullptr
  wc_client *client_p;
  struct PICSTORE picstore[MAX_PICSTORE];
  uint8_t lastBnum; // the last picture index stored
  struct PICSTORE snapshotStore;

#ifdef ENABLE_RTSPSERVER
  // TCP server on port 8554
  WiFiServer *rtspp;
  // pointer to the first rtsp client in a list of multiple clients, or nullptr
  wc_rtspclient * volatile rtspclient;
  uint8_t rtsp_start;
#endif // ENABLE_RTSPSERVER
} Wc;

struct {
  uint32_t camcnt = 0;
  uint32_t camfps = 0;
  uint32_t maxfps = 0;
  uint32_t camfail = 0;
  uint32_t jpegfail = 0;
  uint32_t clientfail = 0;
  float avgFPS = 0.0;
  uint16_t avgProcessingPerFrameMS;
  uint16_t avgFrameMS;
  uint32_t framesTotal;
  uint32_t framesLost;

  uint8_t webclientcount;
  uint8_t rtspclientcount;

  char name[7] = "Webcam";
} WcStats;


// allocate a PICSTORE buffer.
// for PIXFORMAT_JPEG:
//   set bpp_orsize to the size of the jpeg file
//   width and height are unused
// for pixel based formats
//   size is based on width*height*(bytes per pixel)+4
// allocstyle: 
//   0->always free & alloc
//   1->realloc only if not big enough
//   2->realloc only if not big enough, and copy data
#define WC_ALLOC_ALWAYS 0
#define WC_REALLOC_IF_SMALLER 1
#define WC_REALLOC_WITH_COPY 2

#define WC_USE_FAST_MEM 0x10

bool pic_alloc(struct PICSTORE *ps, int width, int height, int jpegsize, int format, int allocstyle){
  int len = 0;
  bool res = false;
  uint8_t* orgbuf = nullptr;
  int orglen = 0;

  if (!ps){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: pic_alloc ps null"));
    return false;
  }

  switch(format){
    default:                  len = 0; break;
    case PIXFORMAT_JPEG:      len = jpegsize; break;
    case PIXFORMAT_GRAYSCALE: len = width*height + 4; break;
    case PIXFORMAT_RGB888:    len = width*height*3 + 4; break;
    case PIXFORMAT_RGB565:    len = width*height*2 + 4; break;
  }

  if (!len){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: pic_alloc invalid format or len 0"));
    return false;
  }

  switch(allocstyle & 0xf){
    case WC_ALLOC_ALWAYS: // just free & allocate, unless no change
      if (len != ps->allocatedLen){
        ps->allocatedLen = -1; // force realloc
      }
      break;
    case WC_REALLOC_IF_SMALLER: // allocate if buf not big enough
      // will use ps->allocatedLen to check below
      break;
    case WC_REALLOC_WITH_COPY: // allocate and copy data if buf not big enough
      orgbuf = ps->buff;
      orglen = ps->allocatedLen;
      break;
  }

  if (ps->allocatedLen < len){
    free(ps->buff);
    memset(ps, 0, sizeof(*ps));
  }
  if (!ps->buff){
    if (len){
      if ((allocstyle & WC_USE_FAST_MEM) || !Wc.psram){
        ps->buff = (uint8_t *)malloc(len);
      } else {
        ps->buff = (uint8_t *)heap_caps_aligned_alloc(4, len, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      }
      if (ps->buff) ps->allocatedLen = len;
      if (orgbuf && orglen){
        memcpy(ps->buff, orgbuf, orglen);
      }
    }
  }
  if (ps->buff){
    ps->len = len;
    // don't change  if 0 in, as jpeg decode sets at start only
    if (width) ps->width = width;
    if (height) ps->height = height;
    ps->format = format;
  }

  if (ps->buff) res = true;
  return res;
}

// return true if pic had content.
bool pic_free(struct PICSTORE *ps){
  bool res = false;
  if (ps){
    if (ps->buff){
      free(ps->buff);
      res = true;
    }
    memset(ps, 0, sizeof(*ps));
  }
  return res;
}

bool pic_alloc_p(struct PICSTORE **pps, int width, int height, int jpegsize, int format, int allocstyle){
  if (!*pps){
    *pps = (struct PICSTORE *)malloc(sizeof(struct PICSTORE));
    memset(*pps, 0, sizeof(struct PICSTORE));
  }
  if (*pps){
    return pic_alloc(*pps, width, height, jpegsize, format, allocstyle);
  }
  return false;
}
bool pic_free_p(struct PICSTORE **pps){
  bool res = false;
  if (*pps){
    res = pic_free(*pps);
    free(*pps);
    *pps = nullptr;
  }
  return res;
}


bool wc_check_format(int format){
  switch(format){
    case PIXFORMAT_JPEG:      return true;
    case PIXFORMAT_GRAYSCALE: return true;
#ifdef WC_USE_RGB_DECODE    
    case PIXFORMAT_RGB888:    return true;
    case PIXFORMAT_RGB565:    return true;
#endif
  }
  return false;
}





/*********************************************************************************************/
// functions to encode into a jpeg buffer.
// we keep ONE allocated buffer for this, and expand it if the jpeg does not fit.
// single expanding jpeg buffer for encode into
// call encode_reset() to maybe recover from failure to allocate.

#define DEFAULT_INITIAL_JPEG_LEN 16384

// static store for taking pictures
struct PICSTORE OurOneJpeg = {0};

// static store for video.
// we only re-allocate if the jpeg is larger, to stop malloc churn.
struct PICSTORE VideoJpeg = {0};


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

// this is called from outside to prevent conflicts in the esp.
// specifically from 
// xdrv_50_filesystem.ino - bool TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len)
// support_esp.ino - void NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen)
void WcInterrupt(uint32_t state) {
  TasAutoMutex localmutex(&WebcamMutex, "WcInterrupt", 20000);
  // Stop camera ISR if active to fix TG1WDT_SYS_RESET
  if (!Wc.up) { return; }

  if (state) {
    // Re-enable interrupts
    cam_start();
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: cam_start()"));
    Wc.disable_cam = 0;
  } else {
    // Stop interrupts
    Wc.disable_cam = 1;
    cam_stop();
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: cam_stop()"));
  }
}


// wait timeout_ms for the value to clear
// use to wait for task to do something
#define WC_WAIT_INTERVAL_MS 10
bool WcWaitZero(volatile int8_t *val, int8_t initial, int timeout_ms){
  int loops = timeout_ms/WC_WAIT_INTERVAL_MS;
  if (!loops) loops = 1;
  *val = initial;
  while(*val && loops--){
    vTaskDelay(WC_WAIT_INTERVAL_MS / portTICK_PERIOD_MS);
  }
  if (*val) return false;
  return true; 
}

// TAS will disable us for short periods...
// this wait for TAS to re-enable it
void WcWaitEnable(){
  int timeout_ms = 1000;
  int loops = timeout_ms/WC_WAIT_INTERVAL_MS;
  if (!loops) loops = 1;
  while(Wc.disable_cam && loops--){
    vTaskDelay(WC_WAIT_INTERVAL_MS / portTICK_PERIOD_MS);
  }
}

bool WcPinUsed(void) {
  bool pin_used = true;
  for (uint32_t i = 0; i < MAX_WEBCAM_DATA; i++) {
    if (!PinUsed(GPIO_WEBCAM_DATA, i)) {
      pin_used = false;
    }
//    if (i < MAX_WEBCAM_HSD) {
//      if (!PinUsed(GPIO_WEBCAM_HSD, i)) {
//        pin_used = false;
//      }
//    }
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: i2c_enabled_2: %d"), TasmotaGlobal.i2c_enabled_2);

  if (!PinUsed(GPIO_WEBCAM_XCLK) || !PinUsed(GPIO_WEBCAM_PCLK) ||
      !PinUsed(GPIO_WEBCAM_VSYNC) || !PinUsed(GPIO_WEBCAM_HREF) ||
      ((!PinUsed(GPIO_WEBCAM_SIOD) || !PinUsed(GPIO_WEBCAM_SIOC)) && !TasmotaGlobal.i2c_enabled_2)    // preferred option is to reuse and share I2Cbus 2
      ) {
        pin_used = false;
  }
  return pin_used;
}

void WcFeature(int32_t value) {
  TasAutoMutex localmutex(&WebcamMutex, "WcFeature", 200);
  sensor_t * wc_s = esp_camera_sensor_get();
  if (!wc_s) { return; }

  if (value != 1) {
      // CLKRC: Set Clock Divider to 0 = fullspeed
      wc_s->set_reg(wc_s, 0x111, 0x3f, 0x00);
      vTaskDelay(200 / portTICK_PERIOD_MS);
  }
  if (value != 2) {
      // Stop Nightmode
      wc_s->set_reg(wc_s, 0x103, 0xff, 0x0a);   // COM1: Reset dummy frames
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x43);   // Reserved Reg
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x4b);   // Reserved Reg
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x43);   // Reserved Reg
  }

  switch(value) {
    case 1:
      // Reduce FPS
      // CLKRC: Set Clock Divider to 2
      wc_s->set_reg(wc_s, 0x111, 0x3f, 0x02);
      break;
    case 2:
      // Start Nightmode
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x4b);   // Reserved Reg
      wc_s->set_reg(wc_s, 0x103, 0xff, 0xcf);   // COM1: Allow 7 dummy frames
      break;
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Feature: %d"), value);
}

void WcApplySettings() {
  TasAutoMutex localmutex(&WebcamMutex, "WcApplySettings", 200);
  sensor_t * wc_s = esp_camera_sensor_get();
  if (!wc_s) { return; }

  wc_s->set_vflip(wc_s, Settings->webcam_config.flip);
  wc_s->set_hmirror(wc_s, Settings->webcam_config.mirror);

  wc_s->set_brightness(wc_s, Settings->webcam_config.brightness - 2);
  wc_s->set_saturation(wc_s, Settings->webcam_config.saturation - 2);
  wc_s->set_contrast(wc_s, Settings->webcam_config.contrast - 2);

  wc_s->set_special_effect(wc_s, Settings->webcam_config2.special_effect);

  wc_s->set_whitebal(wc_s, Settings->webcam_config.awb);
  wc_s->set_wb_mode(wc_s, Settings->webcam_config2.wb_mode);
  wc_s->set_awb_gain(wc_s, Settings->webcam_config.awb_gain);

  wc_s->set_exposure_ctrl(wc_s, Settings->webcam_config.aec);
  wc_s->set_aec_value(wc_s, Settings->webcam_config2.aec_value - 2);
  wc_s->set_ae_level(wc_s, Settings->webcam_config2.ae_level);
  wc_s->set_aec2(wc_s, Settings->webcam_config.aec2);

  wc_s->set_gain_ctrl(wc_s, Settings->webcam_config.agc);
  wc_s->set_agc_gain(wc_s, Settings->webcam_config2.agc_gain);
  wc_s->set_gainceiling(wc_s, (gainceiling_t)Settings->webcam_config2.gainceiling);

  wc_s->set_raw_gma(wc_s, Settings->webcam_config.raw_gma);
  wc_s->set_lenc(wc_s, Settings->webcam_config.lenc);
  wc_s->set_wpc(wc_s, Settings->webcam_config.wpc);
  wc_s->set_dcw(wc_s, Settings->webcam_config.dcw);
  wc_s->set_bpc(wc_s, Settings->webcam_config.bpc);

  WcFeature(Settings->webcam_config.feature);

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Settings updated"));
}

void WcSetDefaults(uint32_t upgrade) {
  if (!upgrade) {
    Settings->webcam_config.flip = 0;
    Settings->webcam_config.mirror = 0;

    Settings->webcam_config.saturation = 2; // = 0
    Settings->webcam_config.brightness = 2; // = 0
    Settings->webcam_config.contrast = 2;   // = 0
  }

  Settings->webcam_config2.special_effect = 0;
  Settings->webcam_config.colorbar = 0;

  Settings->webcam_config.awb = 1;        // white balance
  Settings->webcam_config2.wb_mode = 0;   // white balance mode
  Settings->webcam_config.awb_gain = 1;   // white blance gain

  Settings->webcam_config.aec = 1;          // autoexposure (sensor)
  Settings->webcam_config.aec2 = 1;         // autoexposure (dsp)
  Settings->webcam_config2.ae_level = 2;    // autoexposure level (-2 - +2, default 0)
  Settings->webcam_config2.aec_value = 204; // manual exposure value

  Settings->webcam_config.agc = 1;          // auto gain control
  Settings->webcam_config2.agc_gain = 5;    // manual gain control
  Settings->webcam_config2.gainceiling = 0; // auto gain ceiling

  Settings->webcam_config.raw_gma = 1;      // gamma correct
  Settings->webcam_config.lenc = 1;         // lens correction
  Settings->webcam_config.wpc = 1;          // white pixel correct
  Settings->webcam_config.dcw = 1;          // downsize en
  Settings->webcam_config.bpc = 0;          // black pixel correct?

  Settings->webcam_config.feature = 0;
  
  WcSetMotionDefaults();

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Defaults set"));

  if (Wc.up) { WcApplySettings(); }
}

// deinit and power down camera
void WcCamOff() {
  TasAutoMutex localmutex(&WebcamMutex, "WcCamOff", 30000);
  // deinit camera
  WcSetup(-1);
  // kill any existing clients
  WcEndStream();
  // kill any existing rtsp clients
  WcEndRTSP();

  gpio_num_t pin_pwdn;
  if (PinUsed(GPIO_WEBCAM_PWDN)){
    pin_pwdn = (gpio_num_t)Pin(GPIO_WEBCAM_PWDN);
  } else {
    pin_pwdn = (gpio_num_t)Pin(PWDN_GPIO_NUM);
  }
  gpio_config_t conf = { 0 };
  conf.pin_bit_mask = 1LL << pin_pwdn;
  conf.mode = GPIO_MODE_OUTPUT;
  gpio_config(&conf);

  // carefull, logic is inverted compared to reset pin
  gpio_set_level(pin_pwdn, 1);
  vTaskDelay(10 / portTICK_PERIOD_MS);
  Wc.lastCamError = 0x3;
}

uint32_t WcSetup(int32_t fsiz) {
  // we must stall until re-enabled
  WcWaitEnable();

  TasAutoMutex localmutex(&WebcamMutex, "WcSetup", 200);

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: WcSetup"));
  if (fsiz >= FRAMESIZE_FHD) { fsiz = FRAMESIZE_FHD - 1; }

  if (fsiz < 0) {
    if (Wc.up){    
      esp_camera_deinit();
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Deinit fsiz %d"), fsiz);
      Wc.up = 0;
    }
    Wc.lastCamError = 0x1;
    return 0;
  }

  if (Wc.up) {
    esp_camera_deinit();
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Deinit"));
    //return Wc.up;
  }
  Wc.up = 0;

//esp_log_level_set("*", ESP_LOG_VERBOSE);

  camera_config_t config;

  memset(&config, 0, sizeof(config));

  if (WcPinUsed()) {
    config.pin_d0 = Pin(GPIO_WEBCAM_DATA);        // Y2_GPIO_NUM;
    config.pin_d1 = Pin(GPIO_WEBCAM_DATA, 1);     // Y3_GPIO_NUM;
    config.pin_d2 = Pin(GPIO_WEBCAM_DATA, 2);     // Y4_GPIO_NUM;
    config.pin_d3 = Pin(GPIO_WEBCAM_DATA, 3);     // Y5_GPIO_NUM;
    config.pin_d4 = Pin(GPIO_WEBCAM_DATA, 4);     // Y6_GPIO_NUM;
    config.pin_d5 = Pin(GPIO_WEBCAM_DATA, 5);     // Y7_GPIO_NUM;
    config.pin_d6 = Pin(GPIO_WEBCAM_DATA, 6);     // Y8_GPIO_NUM;
    config.pin_d7 = Pin(GPIO_WEBCAM_DATA, 7);     // Y9_GPIO_NUM;
    config.pin_xclk = Pin(GPIO_WEBCAM_XCLK);      // XCLK_GPIO_NUM;
    config.pin_pclk = Pin(GPIO_WEBCAM_PCLK);      // PCLK_GPIO_NUM;
    config.pin_vsync = Pin(GPIO_WEBCAM_VSYNC);    // VSYNC_GPIO_NUM;
    config.pin_href = Pin(GPIO_WEBCAM_HREF);      // HREF_GPIO_NUM;
    config.pin_sccb_sda = Pin(GPIO_WEBCAM_SIOD);  // SIOD_GPIO_NUM; - unset to use shared I2C bus 2
    config.pin_sccb_scl = Pin(GPIO_WEBCAM_SIOC);  // SIOC_GPIO_NUM;
    if(TasmotaGlobal.i2c_enabled_2){              // configure SIOD and SIOC as SDA,2 and SCL,2
      config.sccb_i2c_port = 1;                   // reuse initialized bus 2, can be shared now
      if(config.pin_sccb_sda < 0){                // GPIO_WEBCAM_SIOD must not be set to really make it happen
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: Use I2C bus2"));
      }
    }
    config.pin_pwdn = Pin(GPIO_WEBCAM_PWDN);       // PWDN_GPIO_NUM;
    config.pin_reset = Pin(GPIO_WEBCAM_RESET);    // RESET_GPIO_NUM;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Template pin config"));
  } else if (Y2_GPIO_NUM != -1) {
    // Modell is set in camera_pins.h
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Compile flag pin config"));
  } else {
    // no valid config found -> abort
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: No pin config"));
    return 0;
  }

  // always power cycle the camera
  // this adds 400ms to start delay, but is worth it to solve random 0x105
  if (config.pin_pwdn){
    // this is only done in driver first init
    // so first run, we should configure as they do.
    gpio_config_t conf = { 0 };
    conf.pin_bit_mask = 1LL << config.pin_pwdn;
    conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&conf);

    int power_delay = 200;
    // if currently powered
    if (!gpio_get_level((gpio_num_t)config.pin_pwdn)){
      // power off for 200ms
      gpio_set_level((gpio_num_t)config.pin_pwdn, 1);
      vTaskDelay(power_delay / portTICK_PERIOD_MS);
    }

    // if camera currently powered off, power it up and wait for 200ms
    // this is a trial to see if it reduces 0x105
    // - and it solves the 0x105 issue!!!
    if (gpio_get_level((gpio_num_t)config.pin_pwdn)){
      gpio_set_level((gpio_num_t)config.pin_pwdn, 0);
      vTaskDelay(power_delay / portTICK_PERIOD_MS);
    }
  }

  int32_t ledc_channel = analogAttach(config.pin_xclk);
  if (ledc_channel < 0) {
    AddLog(LOG_LEVEL_ERROR, "CAM: cannot allocated ledc channel, remove a PWM GPIO");
  }
  config.ledc_channel = (ledc_channel_t) ledc_channel;
  AddLog(LOG_LEVEL_DEBUG_MORE, "CAM: XCLK on GPIO %i using ledc channel %i", config.pin_xclk, config.ledc_channel);
  config.ledc_timer = LEDC_TIMER_0;
//  config.xclk_freq_hz = 20000000;
  if (!Settings->webcam_clk) Settings->webcam_clk = 20;
  config.xclk_freq_hz = Settings->webcam_clk * 1000000;
  int pixFormat = PIXFORMAT_JPEG;
/* 2023-05-28 - AiThinker type cam module marked DM.
  tried everything here, and it seems you cannot get anything other than JPEG.
  even if it's set to another value at power on.  
  Maybe it will work with a different camera, or a board with a CAM_RESET line.
*/
  if (Wc.camPixelFormat) pixFormat = (Wc.camPixelFormat-1);
  
  config.pixel_format = (pixformat_t)pixFormat;
  if (config.pixel_format != PIXFORMAT_JPEG){
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Pixel format is %d, not JPEG"), config.pixel_format);
  }
  //;
  //esp_log_level_set("*", ESP_LOG_INFO);

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.

  Wc.psram = UsePSRAM();
  if (Wc.psram) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: PSRAM found"));
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_DRAM;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: PSRAM not found"));
  }


  esp_err_t err;
  // cannot hurt to retry...
  for (int i = 0; i < 3; i++){
    err = esp_camera_init(&config);
    if (err != ESP_OK) {
      AddLog(LOG_LEVEL_INFO, PSTR("CAM: InitErr 0x%x try %d"), err, (i+1));
      esp_camera_deinit();
      if (err == 0x105){
        // try a longer power off... and retry
        // power off for 500ms
        gpio_set_level((gpio_num_t)config.pin_pwdn, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
    } else {
      if (i){
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: InitOK try %d"), (i+1));
      }
      break;
    }
  }

  if (err != ESP_OK) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: InitErr 0x%x"), err);
    Wc.lastCamError = err;
    return 0;
  }

//  AddLog(LOG_LEVEL_INFO, PSTR("CAM: heap check 2: %d"),ESP_getFreeHeap());

  // drop down frame size for higher initial frame rate
  sensor_t * wc_s = esp_camera_sensor_get();
  // seems config.frame_size has no effect?
  // so this is where we set framesize....
  wc_s->set_framesize(wc_s, (framesize_t)fsiz);

  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) {
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: Init failed to get the frame on time"));
    Wc.lastCamError = 2;
    return 0;
  }
  Wc.width = wc_fb->width;
  Wc.height = wc_fb->height;
  esp_camera_fb_return(wc_fb);

  WcApplySettings();

  camera_sensor_info_t *info = esp_camera_sensor_get_info(&wc_s->id);

  AddLog(LOG_LEVEL_INFO, PSTR("CAM: %s Initialized"), info->name);
  Wc.up = 1;
  if (Wc.psram) { Wc.up = 2; }

  Wc.frameIntervalsus = (uint32_t)(((float)nativeIntervals20ms[fsiz]/((float)Settings->webcam_clk/20.0))*1000.0);
  WcStats.maxfps = (uint32_t)((float)1000000.0/(float)Wc.frameIntervalsus);

  Wc.lastCamError = ESP_OK;
  return Wc.up;
}

/*********************************************************************************************/

int32_t WcSetOptions(uint32_t sel, int32_t value) {
  int32_t res = 0;
  TasAutoMutex localmutex(&WebcamMutex, "WcSetOptions", 200);

  // don;t want sensor for this one.
  switch (sel) {
    case 24:
      // frames to ignore between reads
      if (value >= 0) { Wc.skipFrames = value; }
      return value;
      break;
    case 25:
      // pixelformat - native formats + 1, 0->jpeg
      if (value >= 0) { Wc.camPixelFormat = value; }
      if (Wc.up){
        WcSetup(Settings->webcam_config.resolution);
      }
      return value;
      break;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (!s) { return -99; }

  switch (sel) {
    case 0:
      if (value >= 0) { s->set_framesize(s, (framesize_t)value); }
      res = s->status.framesize;
      Wc.width = 0;
      Wc.height = 0;
      Wc.last_frame_len = 0;
      Wc.frameIntervalsus = (uint32_t)(((float)nativeIntervals20ms[value]/((float)Settings->webcam_clk/20.0))*1000.0);
      WcStats.maxfps = (uint32_t)((float)1000000.0/(float)Wc.frameIntervalsus);

      // WcFeature is lost on resolution change
      WcApplySettings();
      break;
    case 1:
      if (value >= 0) { s->set_special_effect(s, value); }
      res = s->status.special_effect;
      break;
    case 2:
      if (value >= 0) { s->set_vflip(s, value); }
      res = s->status.vflip;
      break;
    case 3:
      if (value >= 0) { s->set_hmirror(s, value); }
      res = s->status.hmirror;
      break;
    case 4:
      if (value >= -4) { s->set_contrast(s, value); }
      res = s->status.contrast;
      break;
    case 5:
      if (value >= -4) { s->set_brightness(s, value); }
      res = s->status.brightness;
      break;
    case 6:
      if (value >= -4) { s->set_saturation(s,value); }
      res = s->status.saturation;
      break;
    case 7:
      if (value >= 0) { s->set_whitebal(s, value); }
      res = s->status.awb;
      break;
    case 8:
      if (value >= 0) { s->set_wb_mode(s, value); }
      res = s->status.wb_mode;
      break;
    case 9:
      if (value >= 0) { s->set_awb_gain(s, value); }
      res = s->status.awb_gain;
      break;
    case 10:
      if (value >= 0) { s->set_exposure_ctrl(s, value); }
      res = s->status.aec;
      break;
    case 11:
      if (value >= 0) { s->set_aec_value(s, value); }
      res = s->status.aec_value;
      break;
    case 12:
      if (value >= 0) { s->set_ae_level(s, value); }
      res = s->status.ae_level;
      break;
    case 13:
      if (value >= 0) { s->set_aec2(s, value); }
      res = s->status.aec2;
      break;
    case 14:
      if (value >= 0) { s->set_gain_ctrl(s, value); }
      res = s->status.agc;
      break;
    case 15:
      if (value >= 0) { s->set_agc_gain(s, value); }
      res = s->status.agc_gain;
      break;
    case 16:
      if (value >= 0) { s->set_gainceiling(s, (gainceiling_t)value); }
      res = s->status.gainceiling;
      break;
    case 17:
      if (value >= 0) { s->set_raw_gma(s, value); }
      res = s->status.raw_gma;
      break;
    case 18:
      if (value >= 0) { s->set_lenc(s, value); }
      res = s->status.lenc;
      break;
    case 19:
      if (value >= 0) { s->set_wpc(s, value); }
      res = s->status.wpc;
      break;
    case 20:
      if (value >= 0) { s->set_dcw(s, value); }
      res = s->status.dcw;
      break;
    case 21:
      // blackpixelcontrol
      if (value >= 0) { s->set_bpc(s, value); }
      res = s->status.bpc;
      break;
    case 22:
      if (value >= 0) { s->set_colorbar(s, value); }
      res = s->status.colorbar;
      break;
    case 23:
      if (value >= 0) { WcFeature(value); }
      break;
  }

  return res;
}

// wait for a frame to be read, if thread is running
void WcWaitFrame(int maxtime_ms) {
  if (!Wc.taskRunning) return;
  // force a wait for a read
  WcWaitZero(&Wc.taskTakePic, -1, maxtime_ms);
}

uint32_t WcGetWidth(void) {
  if (Wc.taskRunning){
    if (!Wc.width){
      WcWaitFrame(1000);
    }
  }
  return Wc.width;
}

uint32_t WcGetHeight(void) {
  if (Wc.taskRunning){
    if (!Wc.width){
      WcWaitFrame(1000);
    }
  }
  return Wc.width;
}

/*********************************************************************************************/

struct WC_Motion {
  /////////////////////////////////////
  // configured by user
  uint16_t motion_detect; // time between detections
  uint32_t motion_trigger_limit; // last amount of difference measured (~100 for none, > ~1000 for motion?)
  uint8_t scale; /*0=native, 1=/2, 2=/4, 3=/8*/
  uint8_t swscale; // skips pixels 0=native, 1=/2, 2=/4, 3=/8 - after scale
  uint8_t enable_diffbuff; // enable create of a buffer containing the last difference image
  uint8_t enable_backgroundbuff;
  uint8_t capture_background;

  uint8_t pixelThreshold;
  uint32_t  pixel_trigger_limit; // pertenthousand changed pixels

  uint8_t enable_mask; // enable mask buffer
  uint32_t auto_mask; // number of motion runs to run automask over
  uint8_t auto_mask_pixel_threshold; // pixel change threshold to add pixel to mask
  uint8_t auto_mask_pixel_expansion; // number of pixels atound the detected pixel to set in mask (square)

  ////////////////////////////////////
  // variables used in detection

  // set to 0 each any time we restart (new last_motion_buffer), goes to after first processing 
  // set to -1 on malloc failure - will happen with larger frames
  int8_t motion_state;
  uint32_t motion_ltime;  // time of last detect
  uint32_t motion_trigger; // last amount of difference measured (~100 for none, > ~1000 for motion?)
  uint32_t motion_brightness; // last frame brightness read (~15000)

  // jpeg is decoded (with scale) into here.
  struct PICSTORE *frame;
  // the others are both scale and swscale
  // the last image - to compare against.
  struct PICSTORE *last_motion;
  // optional - the last difference.
  struct PICSTORE *diff;
  // optional - a mask to stop differences in set pixels triggering motion
  struct PICSTORE *mask;
  // Optional static background image - to compare against.
  struct PICSTORE *background;

  int scaledwidth;
  int scaledheight;
  uint32_t changedPixelPertenthousand;

  uint32_t required_motion_buffer_len; // required frame buffer len - used to prevent continual reallocation after failure

  ////////////////////////////////////
  // triggers picked up by wcloop()
  volatile uint8_t motion_processed; // set to 1 each time it's processed.
  volatile uint8_t motion_triggered; // motion was over trigger limit

  ////////////////////////////////////
  // status/debug
  int32_t last_duration;
} wc_motion;

void WcSetMotionDefaults(){
  wc_motion.motion_trigger_limit = 1000; // last amount of difference measured (~100 for none, > ~1000 for motion?)
  wc_motion.scale = 3;
  wc_motion.swscale = 0;
};


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

uint32_t WcGetPicstore(int32_t num, uint8_t **buff) {
  if (num<0) { return MAX_PICSTORE; }
  *buff = Wc.picstore[num].buff;
  return Wc.picstore[num].len;
}
uint32_t WcGetPicstorePtr(int32_t num, struct PICSTORE **p) {
  if (num < 0) { 
    *p = nullptr;
    return MAX_PICSTORE; 
  }
  if (num >= MAX_PICSTORE) {
    *p = nullptr;
    return MAX_PICSTORE; 
  } 
  *p = &Wc.picstore[num];
  return Wc.picstore[num].len;
}


uint32_t WcGetFrame(int32_t bnum) {
  if (bnum < 0) {
    if (bnum < -MAX_PICSTORE) { bnum=-1; }
    bnum = -bnum;
    // check before decrement
    if (bnum == Wc.lastBnum){
      Wc.lastBnum = 0;
    }
    bnum--;
    TasAutoMutex localmutex(&WebcamMutex, "WcGetFrame", 200);
    if (Wc.picstore[bnum].buff) { free(Wc.picstore[bnum].buff); }
    memset(&Wc.picstore[bnum], 0, sizeof(Wc.picstore[bnum]));
    Wc.picstore[bnum].len = 0;
    return 0;
  }

  if (!bnum) {    
    // used to be used to trigger read a frame to get width/height
    // can still get here via scripts
    return 0;
  }

  if (Wc.taskRunning){
    WcWaitZero(&Wc.taskGetFrame, bnum, 1000);
    return Wc.picstore[bnum - 1].len;
  }
  return 0;
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


//////////////// Handle authentication /////////////////


bool WebcamAuthenticate(void)
{
  if (strlen(SettingsText(SET_WEBPWD)) && (HTTP_MANAGER_RESET_ONLY != Web.state)) {
    return Wc.CamServer->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD));
  } else {
    return true;
  }
}

bool WebcamCheckPriviledgedAccess(bool autorequestauth = true)
{
  if(Settings->webcam_config2.auth == 0){
    return true;
  }
  if (autorequestauth && !WebcamAuthenticate()) {
    Wc.CamServer->requestAuthentication();
    return false;
  }
  return true;
}

///////////////////////////////////////////////////

void HandleImage(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  uint32_t bnum = Webserver->arg(F("p")).toInt();
  if ((bnum < 0) || (bnum > MAX_PICSTORE)) { bnum= 1; }
  int format = (int)PIXFORMAT_JPEG;

  if (bnum){
    // no picture present at this index
    if (!Wc.picstore[bnum-1].buff){
      Webserver->send(404,"",""); 
      return;
    } 
    format = (int)Wc.picstore[bnum-1].format;
  }

  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.";
  if(format == (int)PIXFORMAT_JPEG){
    response += "jpg\r\nContent-type: image/jpeg\r\n";
  } else {
    response += "bin\r\nContent-type: image/x-tas-binary\r\n";
  }
  response += "Content-Length: ";
  char tmp[12];

  if (!bnum) {
    if (Wc.taskRunning == 1){
      WcWaitZero(&Wc.taskTakePic, 1, 1000);
      TasAutoMutex localmutex(&WebcamMutex, "HandleImage", 200);
      if (Wc.snapshotStore.len) {
        response += itoa(Wc.snapshotStore.len, tmp, 10);
        response += "\r\n\r\n";

        WiFiClient client = Webserver->client();
        Webserver->sendContent(response);
        client.write((char *)Wc.snapshotStore.buff, Wc.snapshotStore.len);
        client.stop();
        pic_free(&Wc.snapshotStore);
      } else {
        Webserver->send(404,"",""); 
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: No image #: %d"), bnum);
        return;
      }
    } else {
      Webserver->send(404,"",""); 
      return;
    }
  } else {
    TasAutoMutex localmutex(&WebcamMutex, "HandleImage", 200);
    bnum--;
    if (!Wc.picstore[bnum].len) {
      Webserver->send(404,"",""); 
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: No image #: %d"), bnum);
      return;
    }
    response += itoa(Wc.picstore[bnum].len, tmp, 10);
    response += "\r\n\r\n";
    WiFiClient client = Webserver->client();
    Webserver->sendContent(response);
    client.write((char *)Wc.picstore[bnum].buff, Wc.picstore[bnum].len);
    client.stop();
  }

  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("CAM: Sending image #: %d"), bnum+1);
}

void HandleImageAny(struct PICSTORE *ps){
  if (!HttpCheckPriviledgedAccess()) { return; }

  if (!ps || !ps->buff) {
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: image not present"));
    Webserver->send(404,"",""); 
    return;
  }
  // convert mono to rgb8

  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  // use a malloc that we don't free to save memory creep
  // it is re-mallcoed if the frame does not fit.
  bool conv;

  // allocate a new picture every time to avoid holding up task thread by using OurOneJpeg
  struct PICSTORE psout = {0};

  if (ps->format != PIXFORMAT_JPEG) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: image will be encoded from %d"), ps->format);

    TasAutoMutex localmutex(&WebcamMutex, "HandleImagemotion", 2000);
    // use a malloc that we don't free to save memory creep
    // it is re-mallcoed if the frame does not fit.
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: %x %d %d %d"), ps->buff, ps->len, ps->width, ps->height);
    conv = WcencodeToJpeg(ps->buff, ps->len, ps->width, ps->height, (int)ps->format, 80, &psout);
    if (!conv) AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: image encode failed"));

    _jpg_buf_len = psout.len;
    _jpg_buf = psout.buff;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: encoded %X %d (%dx%d) -> %X %d (%d)"),
      ps->buff, ps->len, ps->width, ps->height,
      _jpg_buf, _jpg_buf_len,
      psout.allocatedLen
    );
  } else {
    _jpg_buf_len = ps->len;
    _jpg_buf = ps->buff;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: already jpeg %X %d (%dx%d) (%d)"),
      ps->buff, ps->len, ps->width, ps->height,
      ps->allocatedLen
    );
  }

  if (conv && _jpg_buf_len){
    WiFiClient client = Webserver->client();
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-disposition: inline; filename=cap.jpg\r\n";
    response += "Content-type: image/jpeg\r\n\r\n";
    Webserver->sendContent(response);
    client.write((char *)_jpg_buf, _jpg_buf_len);
    client.stop();
    pic_free(&psout);
    return; // don't send 500
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: image could not be encoded"));
  pic_free(&psout);

  Webserver->send(500,"",""); 
  return;
}

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


void HandleWebcamMjpeg(void) {
  HandleWebcamMjpegFn(0);
}

void HandleWebcamMjpegDiff(void) {
  HandleWebcamMjpegFn(1);
}

void HandleWebcamMjpegFn(int type) {
  if(!WebcamCheckPriviledgedAccess()){
    Wc.CamServer->send(403,"","");
    return;
  }
  TasAutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpeg", 200);
  wc_client *client = new wc_client;
  client->active = 1;
  client->type = type;
  client->p_next = Wc.client_p;
  client->client = Wc.CamServer->client();
  Wc.client_p = client;
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Create client"));
}

void HandleWebcamRoot(void) {
  if(!WebcamCheckPriviledgedAccess()){
    Wc.CamServer->send(403,"","");
    return;
  }
  //CamServer->redirect("http://" + String(ip) + ":81/cam.mjpeg");
  Wc.CamServer->sendHeader("Location", "/cam.mjpeg");
  Wc.CamServer->send(302, "", "");
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Root called"));
}

/*********************************************************************************************/

uint32_t WcSetStreamserver(uint32_t flag) {
  if (TasmotaGlobal.global_state.network_down) { 
    WcEndStream();
    return 0; 
  }

  if (flag) {
    if (!Wc.CamServer) {
      TasAutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpeg", 20000);
      Wc.CamServer = new ESP8266WebServer(81);
      Wc.CamServer->on("/", HandleWebcamRoot);
      Wc.CamServer->on("/diff.mjpeg", HandleWebcamMjpegDiff);
      Wc.CamServer->on("/cam.mjpeg", HandleWebcamMjpeg);
      Wc.CamServer->on("/cam.jpg", HandleWebcamMjpeg);
      Wc.CamServer->on("/stream", HandleWebcamMjpeg);
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stream init"));
      Wc.CamServer->begin();
    }
  } else {
    if (Wc.CamServer) {
      TasAutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpeg", 20000);
      WcEndStream();
      Wc.CamServer->stop();
      delete Wc.CamServer;
      Wc.CamServer = NULL;
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Stream exit"));
    }
  }
  return 0;
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
#endif


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

static void WCOperationTask(void *pvParameters);
static void WCStartOperationTask(){
  if (Wc.taskRunning == 0){
#ifdef BLE_ESP32_DEBUG
    AddLog(LOG_LEVEL_DEBUG,PSTR("CAM: Start operations"));
#endif

    xTaskCreatePinnedToCore(
      WCOperationTask,    /* Function to implement the task */
      "WCOperationTask",  /* Name of the task */
      4096,             /* Stack size in bytes */
      NULL,             /* Task input parameter */
      0,                /* Priority of the task */
      NULL,             /* Task handle. */
#ifdef CONFIG_FREERTOS_UNICORE
      0);               /* Core where the task should run */
#else
      1);               /* Core where the task should run */
#endif
    // wait for task to start
    int loops = 10;
    while(!Wc.taskRunning && loops--){
      vTaskDelay(10/ portTICK_PERIOD_MS);
    }
  }
}


// this IS as task.
// used to disconnect webcam frame read from tas loop
static void WCOperationTask(void *pvParameters){
  unsigned long loopcount = 0;

  AddLog(LOG_LEVEL_DEBUG,PSTR("CAM: WCOperationTask: Start task"));

  int framecount = 0;
  unsigned long laststatmillis = millis();
  bool jpeg_converted = false;
  uint32_t processing_total = 0;
  uint64_t last_camtime = 0;
  Wc.taskRunning = 1;
  WcStats.framesTotal = 0;
  WcStats.framesLost = 0;

  int32_t skipsWanted = Wc.skipFrames;


  // we set to Wc.taskRunning 2 to stop the task
  while (Wc.taskRunning == 1){
    loopcount++;
    uint32_t thismillis = millis();
    uint32_t statdur = thismillis - laststatmillis;

    // storage and settings disable cam.
    // we must stall until re-enabled
    // it asserts the mutex whilst stopping the cam, so it can't stop it mid-processing
    WcWaitEnable();
    // if camera is configured and working
    { // closure for auto mutex
      // note that this mutex can block the loop for a long time - 
      // e.g. if motion detect on a full big frame, up to a second
      TasAutoMutex localmutex(&WebcamMutex, "WebcamMjpeg", 30000);

      // only do anything if cam us up, and cam has not remained stopped
      if (Wc.up && !Wc.disable_cam) {
        size_t _jpg_buf_len = 0;
        uint8_t * _jpg_buf = NULL;

        // read a frame buffer pointer.  this will block until a frame is available
        camera_fb_t *wc_fb = esp_camera_fb_get();

        if (!wc_fb) {
          // add framecount so we show this right away if we were showing frames.
          if (!(loopcount % 100) || (statdur > 5000) || framecount){
            AddLog(LOG_LEVEL_DEBUG,PSTR("CAM: Frame fail")), 
            laststatmillis = thismillis;
            framecount = 0;
          }
          WcStats.camfail++;
          // if camera up, but just can't get a frame right now
          vTaskDelay(10/ portTICK_PERIOD_MS);
        } else {
          framecount++;
          uint32_t now = millis();
          uint64_t camtime = wc_fb->timestamp.tv_sec;
          camtime = camtime * 1000000;
          camtime += wc_fb->timestamp.tv_usec;

          if (!skipsWanted) skipsWanted = Wc.skipFrames;

          if (last_camtime){
            uint32_t camdiff = camtime - last_camtime;
            // this is an accurate indication of framerate, but also frame skip
            Wc.camtimediff = camdiff;
            int framesPassed = round(((float)camdiff)/(float)(Wc.frameIntervalsus));
            if (framesPassed){
              WcStats.framesTotal += framesPassed;
              WcStats.framesLost += (framesPassed - 1);
              if (skipsWanted > 0){
                skipsWanted -= (framesPassed - 1);
              }
              if (skipsWanted > 0) skipsWanted --;
            } else {
              AddLog(LOG_LEVEL_DEBUG,PSTR("CAM: Duplicate time in frame? diff %d intv %d"), camdiff, Wc.frameIntervalsus);
            }
          }
          last_camtime = camtime;

          // every 100 frames or 5s
          if (!(loopcount % 100) || (statdur > 5000)){
            float framespersec = ((float)framecount)/(((float)(thismillis - laststatmillis))/1000.0);
            AddLog(LOG_LEVEL_DEBUG,PSTR("CAM: avFPS %f %s FS:%d(%d) f:%u s:%u"), 
              framespersec, 
              jpeg_converted?"raw":"jpg", 
              wc_fb->len, 
              Wc.lenDiff,
              WcStats.framesTotal,
              WcStats.framesLost
              );
            WcStats.avgProcessingPerFrameMS = processing_total/framecount;
            WcStats.avgFPS = framespersec;
            WcStats.avgFrameMS = (uint8_t) (1000/framespersec);
            processing_total = 0;
            laststatmillis = thismillis;
            framecount = 0;
          }

          // we got a frame
          Wc.width = wc_fb->width;
          Wc.height = wc_fb->height;
          WcStats.camcnt++;

          // if we want to process this frame.
          // skipsWanted is the counter used for Wc.skipFrames use
          if (skipsWanted <= 0) skipsWanted = 0;
          if (!skipsWanted){
            int detectMotion = 0;
            // if we want simple mootion detect,
            if (wc_motion.motion_detect){
              if ((now - wc_motion.motion_ltime) > wc_motion.motion_detect) {
                wc_motion.motion_ltime = now;
                detectMotion = 1;
              }
            }

            // if we need a frame for web stream or rtsp
            if (Wc.client_p // pointer to first 
    #ifdef ENABLE_RTSPSERVER
                || Wc.rtspclient // pointer to first
    #endif
                || Wc.taskGetFrame // get one frame - from scripts
                || Wc.taskTakePic // get one frame - from scripts
                || detectMotion ) {

              // most cameras will supply as jpeg? it's what we ask for...
              jpeg_converted = false;
              bool free_jpeg = false;
              /* 2023-05-28 - AiThinker type cam module marked DM.
                Note: I tried everything to get pixels ina different form, but faiiled.
                So this code has NEVER run for me in anger directly off cam data.
              */

              if (wc_fb->format != PIXFORMAT_JPEG) {
                // note - don't free the jpeg, we re-use it.
                jpeg_converted = WcencodeToJpeg(wc_fb->buf, wc_fb->len, wc_fb->width, wc_fb->height, (int)wc_fb->format, 80, &VideoJpeg);
                _jpg_buf_len = VideoJpeg.len;
                _jpg_buf = VideoJpeg.buff;

                // this function is incredibly expensive - always allocates 128kbytes
                //jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
                // free_jpeg = true; // if using frame2jpg, we must free
                if (!jpeg_converted){
                  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: JPEG compression failed"));
                  WcStats.jpegfail++;
                }
              } else {
                _jpg_buf_len = wc_fb->len;
                _jpg_buf = wc_fb->buf;
              }

              if (_jpg_buf){
                int jpeg_len_diff = abs((int)Wc.last_frame_len - (int)_jpg_buf_len);
                int jpeg_len_diff_percent = 100*jpeg_len_diff/_jpg_buf_len;
                Wc.lenDiff = jpeg_len_diff_percent;
                if (Wc.lenDiffLimit && (Wc.lenDiffLimit < jpeg_len_diff_percent)){
                  Wc.lenDiffTrigger = 1;
                  Wc.lenDiffTriggered = jpeg_len_diff_percent;
                }

                Wc.last_frame_len = _jpg_buf_len;
                // we use -1 to just detect that Wc.width/height should have been set
                if (Wc.taskTakePic){
                  if (Wc.taskTakePic > 0){
                    if (pic_alloc(&Wc.snapshotStore, 0, 0, _jpg_buf_len, PIXFORMAT_JPEG, 1)){
                      memcpy(Wc.snapshotStore.buff, _jpg_buf, _jpg_buf_len);
                    }
                  }
                  Wc.taskTakePic = 0;
                }

                // request to store the frame into picstore
                if (Wc.taskGetFrame){
                  int bnum = Wc.taskGetFrame;
                  if ((bnum < 1) || (bnum > MAX_PICSTORE)) { bnum = 1; }
                  bnum--;
                  if (Wc.picstore[bnum].buff) { free(Wc.picstore[bnum].buff); }
                  memset(&Wc.picstore[bnum], 0, sizeof(Wc.picstore[bnum]));
                  Wc.picstore[bnum].buff = (uint8_t *)heap_caps_aligned_alloc(4, _jpg_buf_len+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
                  if (Wc.picstore[bnum].buff) {
                    memcpy(Wc.picstore[bnum].buff, _jpg_buf, _jpg_buf_len);
                    Wc.picstore[bnum].len = _jpg_buf_len;
                    Wc.lastBnum = bnum + 1;
                    Wc.picstore[bnum].format = (int8_t)PIXFORMAT_JPEG;
                    Wc.picstore[bnum].width = Wc.width;
                    Wc.picstore[bnum].height = Wc.height;

                    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Got frame %d"), Wc.lastBnum);
                  } else {
                    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Can't allocate picstore"));
                  }
                  Wc.taskGetFrame = 0;
                }

                // if motion detect triggered by timer
                if (detectMotion){
                  WcDetectMotionFn(_jpg_buf, _jpg_buf_len);
                }

                // if http streaming is active, we will have one or more clients
                wc_client *client = Wc.client_p;
                // iterate over clients
                uint8_t webclientcount = 0;
                size_t diff_jpg_buf_len = 0;
                uint8_t *diff_jpg_buf = NULL;

                while(client){
                  if (client->active){
                    uint32_t client_start = millis();

                    if (!client->client.connected()){
                      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client fail"));
                      client->active = 0;
                      WcStats.clientfail++;
                    }
                    if (1 == client->active) {
                      client->client.flush();
                      client->client.setTimeout(3);
                      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Start stream"));
                      client->client.print("HTTP/1.1 200 OK\r\n"
                        "Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n"
                        "\r\n");
                      client->active = 2;
                    }

                    if (2 == client->active) {
                      int type = client->type;
                      uint8_t *src = _jpg_buf;
                      int len = _jpg_buf_len;

                      // if this client wants motion images
                      if (type == 1){
                        // if we already coded to jpeg for another client
                        if (diff_jpg_buf){
                          src = diff_jpg_buf;
                          len = diff_jpg_buf_len;
                        } else {
                          src = nullptr;
                          // only for frames where motion is being detected,
                          // e.g. once per sec
                          if (detectMotion){
                            struct PICSTORE *ps = nullptr;
                            // by default, we send the diff buffer if present
                            if (wc_motion.diff && wc_motion.diff->buff){
                              ps = wc_motion.diff;
                            }
                            if (!ps){
                              // else we send the motion buffer (mono of video, but slower)
                              if (wc_motion.last_motion && wc_motion.last_motion->buff){
                                ps = wc_motion.last_motion;
                              }
                            }
                            if (ps){
                              // use a malloc that we don't free to save memory creep
                              // it is re-mallcoed if the frame does not fit.
                              bool conv = WcencodeToJpeg(ps->buff, ps->len, ps->width, ps->height, (int)ps->format, 80, &OurOneJpeg);

                              diff_jpg_buf_len = OurOneJpeg.len;
                              diff_jpg_buf = OurOneJpeg.buff;

                              if (conv && diff_jpg_buf_len){
                                src = diff_jpg_buf;
                                len = diff_jpg_buf_len;
                              }
                            }
                          }
                        }
                      }

                      if (src){
                        client->client.printf(
                          "--" BOUNDARY "\r\n"
                          "Content-Type: image/jpeg\r\n"
                          "Content-Length: %d\r\n"
                          "\r\n", static_cast<int>(len));
                        uint32_t tlen = client->client.write(src, len);
                        client->client.print("\r\n");
                      }
                    }
                    uint32_t client_end = millis();
                    // if it took more than 20s to send to the client, then kill it.
                    // this was observed on wifi rescan
                    if (client_end - client_start > 20000){
                      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Client timeout on send"));
                      WcStats.clientfail++;
                      client->client.stop();
                      client->active = 0;
                    }
                  }
                  client = client->p_next;
                  webclientcount++;
                }
                // if we allocates a jppeg image for motion, free now
                // NO!!! This is now a long-lived buffer!!!
                //if (diff_jpg_buf) free(diff_jpg_buf);
                WcStats.webclientcount = webclientcount;

    #ifdef ENABLE_RTSPSERVER
                // if rtsp is active, we will have one or more clients
                volatile wc_rtspclient *rtspclient = Wc.rtspclient;
                uint8_t rtspclientcount = 0;
                while (rtspclient) {
                  if (rtspclient->camStreamer && rtspclient->rtsp_session){
                    rtspclient->camStreamer->setframe(_jpg_buf, _jpg_buf_len);
                    rtspclient->rtsp_session->broadcastCurrentFrame(now);
                    rtspclient->camStreamer->clearframe();
                  }
                  rtspclient = rtspclient->p_next;
                  rtspclientcount++;
                }
                WcStats.rtspclientcount = rtspclientcount;
    #endif

                // free jpeg frame if we created it
                if (free_jpeg) { free(_jpg_buf); }

                // could trigger info to berry every fraem, if configured
                Wc.frame_processed = 1;

                uint32_t processingend = millis();
                uint32_t processingdur = processingend - now;
                processing_total += processingdur;
              } else {
                Wc.last_frame_len = 0;            
              }
            }
          }

          // free resources and return frame buffer for re-use AFTER we have used the data
          esp_camera_fb_return(wc_fb);
        }
      } else {
        if (Wc.up){
          // if camera up, but just can't get a frame right now because
          // else we may conflict with files or settings store, delay just a shorter time
          vTaskDelay(10/ portTICK_PERIOD_MS);
        } else {
          // be sure to clear this if camera is not running, else we could wait forever
          // when asking for a frame
          Wc.taskGetFrame = 0;
          // come around every 1/10s if not serving pictures
          vTaskDelay(100/ portTICK_PERIOD_MS);
        }
      }
    }
  }
  // wait 1/10 second
  vTaskDelay(100/ portTICK_PERIOD_MS);
  AddLog(LOG_LEVEL_DEBUG,PSTR("CAM: WCOperationTask: Left task"));

  Wc.taskRunning = 0;
  vTaskDelete( NULL );
}

void WcRemoveDeadCients(){
  // iterate over clients removing dead ones
  wc_client *client = Wc.client_p;
  wc_client **prev = &Wc.client_p;
  while(client){
    if (!client->active){
      // just in case...
      TasAutoMutex localmutex(&WebcamMutex, "WcLoop", 200);
      *prev = client->p_next;
      client->client.stop();
      wc_client *next = client->p_next;
      delete client;
      client = next;
    } else {
      prev = &client->p_next;
      client = client->p_next;
    }
  }
}


void WcLoop(void) {

  { // closure for automutex
    // we don't need one here
    //TasAutoMutex localmutex(&WebcamMutex, "WcLoop", 200);

    // if wc_motion.motion_trigger > wc_motion.motion_triggerlimit
    // set wc_motion.motion_triggerlimit low if you want every time it's processed
    // NOTE: there is no 'retrigger hold off' time.
    if (wc_motion.motion_triggered){
#ifdef USE_BERRY
      char t[80];
      snprintf(t, (size_t)39, "{\"val\":%d,\"bri\":%d,\"pix\":%d}", wc_motion.motion_trigger, wc_motion.motion_brightness, wc_motion.changedPixelPertenthousand);
      callBerryEventDispatcher("webcam", "motion", 0, t, strlen(t));
#endif
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Motion Triggered"));
      WcMotionLog();
      wc_motion.motion_triggered = 0;
    }
    if (Wc.lenDiffTrigger){
#ifdef USE_BERRY
      char t[40];
      snprintf(t, (size_t)39, "{\"diff\":%d}", Wc.lenDiffTriggered);
      callBerryEventDispatcher("webcam", "framesizechange", 0, t, strlen(t));
#endif
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Framesize Change > %d = %d"), Wc.lenDiffLimit, Wc.lenDiffTriggered);
      Wc.lenDiffTrigger = 0;
      WcMotionLog();
    }

    if (Wc.frame_processed){
#ifdef USE_BERRY
      if (Wc.berryFrames){
        char t[80];
        snprintf(t, (size_t)39, "{\"len\":%d}", Wc.last_frame_len);
        callBerryEventDispatcher("webcam", "frame", 0, t, strlen(t));
      }
#endif
      Wc.frame_processed = 0;
    }
  } // end closure for automutex

  if (Wc.CamServer) {
    // pretty sure this would be safe
    //TasAutoMutex localmutex(&WebcamMutex, "WcLoop", 200);
    Wc.CamServer->handleClient();
    WcRemoveDeadCients();
  }

#ifdef ENABLE_RTSPSERVER
  if (Settings->webcam_config.rtsp){
    if (!TasmotaGlobal.global_state.wifi_down) {
      // pretty sure we don;t need the mutex here
      TasAutoMutex localmutex(&WebcamMutex, "WcLoop2", 30000);
      if (!Wc.rtspp) {
        Wc.rtspp = new WiFiServer(8554);
        Wc.rtspp->begin();
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTSP init"));
      }

      wc_rtspclient * volatile rtspclient = Wc.rtspclient;
      wc_rtspclient * volatile *  prev = &Wc.rtspclient;
      uint8_t rtspclientcount = 0;
      while (rtspclient) {
        bool removed = false;
        if (rtspclient->rtsp_session){
          rtspclient->rtsp_session->handleRequests(0);
          // if a client has stopped, remove it.
          if (rtspclient->rtsp_session->m_stopped){
            delete rtspclient->rtsp_session;
            rtspclient->rtsp_session = nullptr;
            delete rtspclient->camStreamer;
            rtspclient->camStreamer = nullptr;
            //rtspclient->rtsp_client.stop();
            *prev = rtspclient->p_next;
            wc_rtspclient *next = rtspclient->p_next;
            delete rtspclient;
            rtspclient = next;
            AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTSP stopped"));
            removed = true;
          }
        }
        if (!removed){
          rtspclientcount++;
          prev = &rtspclient->p_next;
          rtspclient = rtspclient->p_next;
        }
      }
      WcStats.rtspclientcount = rtspclientcount;

      // accept new rtsp clients
      WiFiClient rtsp_client = Wc.rtspp->accept();
      if (rtsp_client) {
        wc_rtspclient *client = new wc_rtspclient;
        client->p_next = Wc.rtspclient;
        client->rtsp_client = rtsp_client;
        client->camStreamer = new localOV2640Streamer(&client->rtsp_client, Wc.width, Wc.height);
        client->rtsp_session = new CRtspSession(&client->rtsp_client, client->camStreamer); // our threads RTSP session and state
        AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTSP stream created"));
        Wc.rtspclient = client;
        WcStats.rtspclientcount++;
      }
    } else {
      // rtsp not enabled
      if (Wc.rtspp){
        // if running, stop cleanly.
        // stop all clients
        WcEndRTSP();
        delete Wc.rtspp;
        Wc.rtspp = nullptr;
      }
    }
  }
#endif // ENABLE_RTSPSERVER
}

// kill all rtsp clients
void WcEndRTSP(){
  // we should use a mutext here, in case we are currently sending
  TasAutoMutex localmutex(&WebcamMutex, "WcEndRTSP", 2000);
  wc_rtspclient * volatile rtspclient = Wc.rtspclient;
  wc_rtspclient * volatile * prev = &Wc.rtspclient;
  while (rtspclient) {
    delete rtspclient->rtsp_session;
    rtspclient->rtsp_session = nullptr;
    delete rtspclient->camStreamer;
    rtspclient->camStreamer = nullptr;
    //rtspclient->rtsp_client.stop();
    wc_rtspclient *next = rtspclient->p_next;
    delete rtspclient;
    rtspclient = next;
    AddLog(LOG_LEVEL_INFO, PSTR("CAM: RTSP stopped"));
  }
  Wc.rtspclient = nullptr;
  WcStats.rtspclientcount = 0;
}

// kill all http streaming clients
void WcEndStream(){
  // we should use a mutext here, in case we are currently sending
  TasAutoMutex localmutex(&WebcamMutex, "WcLoop2", 20000);
  // if http streaming is active
  wc_client *client = Wc.client_p;
  // iterate over clients
  while(client){
    client->active = 0;
    if (client->client){
      client->client.stop();
    }
    client = client->p_next;
  }
  WcRemoveDeadCients();
}


void WcPicSetup(void) {
  WebServer_on(PSTR("/wc.jpg"), HandleImage);
  WebServer_on(PSTR("/wc.mjpeg"), HandleImage);
  WebServer_on(PSTR("/snapshot.jpg"), HandleImage);
  WebServer_on(PSTR("/motiondiff.jpg"), HandleImagemotiondiff);
  WebServer_on(PSTR("/motionmask.jpg"), HandleImagemotionmask);
  WebServer_on(PSTR("/motionbuff.jpg"), HandleImagemotionbuff);
  WebServer_on(PSTR("/motionlbuff.jpg"), HandleImagemotionlbuff);
  WebServer_on(PSTR("/motionbackgroundbuff.jpg"), HandleImagemotionbackgroundbuff);
}


const char HTTP_WEBCAM_MENUVIDEOCONTROL[] PROGMEM = "<p></p><button onclick=\"fetch('/cs?c2=64&c1=%s').then(()=>{location.reload();});\" name>%s</button>";

void WcShowStream(void) {
  // if streaming is enabled (1 or 2), start stream server
  if (Settings->webcam_config.stream) {
//    if (!Wc.CamServer || !Wc.up) {
    if (!Wc.CamServer) {
      WcSetStreamserver(Settings->webcam_config.stream);
    }
  }

  if (!Wc.CamServer){
    WSContentSend_P(PSTR("<p></p><center>Cam Server Not Running'</center><p></p>"));
    WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcstream2", "Turn On Streaming");
  } else {
    if (!Wc.up){
      WSContentSend_P(PSTR("<p></p><center>Cam Not Running Err 0x%x</center><p></p>"), Wc.lastCamError);
      WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcinit", "Try WCINIT");
    } else {
      if (Settings->webcam_config.spare15) {
        WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcmenuvideoon", "Turn On Video");
      }
    }
  }

  // spare15 is 'hide cam on menu'
  if (!Settings->webcam_config.spare15 && Settings->webcam_config.stream && Wc.CamServer && Wc.up!=0) {
    // Give the webcam webserver some time to prepare the stream - catch error in JS
    WSContentSend_P(PSTR("<p></p><center><img onerror='setTimeout(()=>{this.src=this.src;},1000)' src='http://%_I:81/stream' alt='Webcam stream' style='width:99%%;'></center><p></p>"),(uint32_t)WiFi.localIP());
    WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcmenuvideooff", "Turn Off Video");
  }
}

void WcInit(void) {
  // .data is in union with the rest of the settings, so
  // this means 'i have no config'
  if (!Settings->webcam_config.data) {
    // set defaults...
    Settings->webcam_config.stream = 1;
    Settings->webcam_config.resolution = FRAMESIZE_QVGA;
    WcSetDefaults(0);
  }
  // previous webcam driver had only a small subset of possible config vars
  // in this case we have to only set the new variables to default values
  if(!Settings->webcam_config2.upgraded) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Upgrade settings"));
    WcSetDefaults(1);
    Settings->webcam_config2.upgraded = 1;
  }
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_PRFX_WEBCAM "WC"
#define D_CMND_WC_STREAM "Stream"
#define D_CMND_WC_RESOLUTION "Resolution"
#define D_CMND_WC_MIRROR "Mirror"
#define D_CMND_WC_FLIP "Flip"
#define D_CMND_WC_SATURATION "Saturation"
#define D_CMND_WC_BRIGHTNESS "Brightness"
#define D_CMND_WC_CONTRAST "Contrast"
#define D_CMND_WC_SPECIALEFFECT "SpecialEffect"

#define D_CMND_WC_AWB "AWB"
#define D_CMND_WC_WB_MODE "WBMode"
#define D_CMND_WC_AWB_GAIN "AWBGain"

#define D_CMND_WC_AEC "AEC"
#define D_CMND_WC_AEC_VALUE "AECValue"
#define D_CMND_WC_AE_LEVEL "AELevel"
#define D_CMND_WC_AEC2 "AECDSP"

#define D_CMND_WC_AGC "AGC"
#define D_CMND_WC_AGC_GAIN "AGCGain"
#define D_CMND_WC_GAINCEILING "GainCeiling"

#define D_CMND_WC_RAW_GMA "GammaCorrect"
#define D_CMND_WC_LENC "LensCorrect"

#define D_CMND_WC_WPC "WPC"
#define D_CMND_WC_DCW "DCW"
#define D_CMND_WC_BPC "BPC"

#define D_CMND_WC_COLORBAR "Colorbar"

#define D_CMND_WC_FEATURE "Feature"
#define D_CMND_WC_SETDEFAULTS "SetDefaults"
#define D_CMND_WC_STATS "Stats"

#define D_CMND_WC_INIT "Init"
#define D_CMND_RTSP "Rtsp"

#define D_CMND_WC_AUTH "Auth"
#define D_CMND_WC_CLK "Clock"

#define D_CMND_WC_STARTTASK "Starttask"
#define D_CMND_WC_STOPTASK "Stoptask"

#define D_CMND_WC_MENUVIDEOOFF "Menuvideooff"
#define D_CMND_WC_MENUVIDEOON "Menuvideoon"

// for testing to see what happens after cam_stop()
#define D_CMND_WC_INTERRUPT "Interrupt"

// mainly for testing functions which could be used by scripts.
#define D_CMND_WC_SETMOTIONDETECT "Setmotiondetect"
#define D_CMND_WC_GETFRAME "Getframe"
#define D_CMND_WC_GETPICSTORE "Getpicstore"

#define D_CMND_WC_BERRYFRAMES "Berryframes"

#define D_CMND_WC_SAVEPIC "SavePic"
#define D_CMND_WC_APPENDPIC "AppendPic"

#define D_CMND_WC_GETMOTIONPIXELS "GetMotionPixels"

#define D_CMND_WC_SETOPTIONS "SetOptions"
#define D_CMND_WC_CONVERTFRAME "ConvertFrame"
#define D_CMND_WC_SETPICTURE "SetPicture"

#define D_CMND_WC_POWEROFF "Poweroff"

const char kWCCommands[] PROGMEM =  D_PRFX_WEBCAM "|"  // Prefix
  "|" D_CMND_WC_STREAM "|" D_CMND_WC_RESOLUTION "|" D_CMND_WC_MIRROR "|" D_CMND_WC_FLIP "|"
  D_CMND_WC_SATURATION "|" D_CMND_WC_BRIGHTNESS "|" D_CMND_WC_CONTRAST "|" D_CMND_WC_SPECIALEFFECT "|"
  D_CMND_WC_AWB "|" D_CMND_WC_WB_MODE "|" D_CMND_WC_AWB_GAIN "|" D_CMND_WC_AEC "|"
  D_CMND_WC_AEC_VALUE "|" D_CMND_WC_AE_LEVEL "|" D_CMND_WC_AEC2 "|" D_CMND_WC_AGC "|"
  D_CMND_WC_AGC_GAIN "|" D_CMND_WC_GAINCEILING "|" D_CMND_WC_RAW_GMA "|" D_CMND_WC_LENC "|"
  D_CMND_WC_WPC "|" D_CMND_WC_DCW "|" D_CMND_WC_BPC "|" D_CMND_WC_COLORBAR "|" D_CMND_WC_FEATURE "|"
  D_CMND_WC_SETDEFAULTS "|" D_CMND_WC_STATS "|" D_CMND_WC_INIT "|" D_CMND_WC_AUTH "|" D_CMND_WC_CLK "|" 
  D_CMND_WC_STARTTASK "|" D_CMND_WC_STOPTASK "|" D_CMND_WC_MENUVIDEOOFF "|" D_CMND_WC_MENUVIDEOON "|" 
  D_CMND_WC_INTERRUPT "|" D_CMND_WC_SETMOTIONDETECT "|" D_CMND_WC_GETFRAME "|" D_CMND_WC_GETPICSTORE "|" 
  D_CMND_WC_BERRYFRAMES  "|" D_CMND_WC_SAVEPIC "|" D_CMND_WC_APPENDPIC  "|" D_CMND_WC_GETMOTIONPIXELS "|"
  D_CMND_WC_SETOPTIONS "|" D_CMND_WC_CONVERTFRAME "|" D_CMND_WC_SETPICTURE "|" D_CMND_WC_POWEROFF

#ifdef ENABLE_RTSPSERVER
  "|" D_CMND_RTSP
#endif // ENABLE_RTSPSERVER
  ;

void (* const WCCommand[])(void) PROGMEM = {
  &CmndWebcam, &CmndWebcamStream, &CmndWebcamResolution, &CmndWebcamMirror, &CmndWebcamFlip,
  &CmndWebcamSaturation, &CmndWebcamBrightness, &CmndWebcamContrast, &CmndWebcamSpecialEffect,
  &CmndWebcamAWB, &CmndWebcamWBMode, &CmndWebcamAWBGain, &CmndWebcamAEC, &CmndWebcamAECValue,
  &CmndWebcamAELevel, &CmndWebcamAEC2, &CmndWebcamAGC, &CmndWebcamAGCGain, &CmndWebcamGainCeiling,
  &CmndWebcamGammaCorrect, &CmndWebcamLensCorrect, &CmndWebcamWPC, &CmndWebcamDCW, &CmndWebcamBPC,
  &CmndWebcamColorbar, &CmndWebcamFeature, &CmndWebcamSetDefaults,
  &CmndWebcamStats, &CmndWebcamInit, &CmndWebcamAuth, &CmndWebcamClock,
  &CmndWebcamStartTask, &CmndWebcamStopTask, &CmndWebcamMenuVideoOff, &CmndWebcamMenuVideoOn,
  &CmndWebcamCamStartStop, &CmndWebcamSetMotionDetect, &CmndWebcamGetFrame, &CmndWebcamGetPicStore,
  &CmndWebcamBerryFrames,
  &CmdWebcamSavePic, &CmdWebcamAppendPic,
  &CmndWebcamGetMotionPixels, &CmndWebcamSetOptions,
  &CmndWebcamConvertFrame, &CmndWebcamSetPicture,
  &CmndWebcamPowerOff

#ifdef ENABLE_RTSPSERVER
  , &CmndWebRtsp
#endif // ENABLE_RTSPSERVER
  };

// plain 'WC' gives config
void CmndWebcam(void) {
  Response_P(PSTR("{\"" D_PRFX_WEBCAM "\":{\"" D_CMND_WC_STREAM "\":%d,\"" D_CMND_WC_RESOLUTION "\":%d,\"" D_CMND_WC_MIRROR "\":%d,\""
    D_CMND_WC_FLIP "\":%d,\""
    D_CMND_WC_SATURATION "\":%d,\"" D_CMND_WC_BRIGHTNESS "\":%d,\"" D_CMND_WC_CONTRAST "\":%d,\""
    D_CMND_WC_SPECIALEFFECT "\":%d,\"" D_CMND_WC_AWB  "\":%d,\"" D_CMND_WC_WB_MODE "\":%d,\""
    D_CMND_WC_AWB_GAIN "\":%d,\"" D_CMND_WC_AEC "\":%d,\"" D_CMND_WC_AEC_VALUE "\":%d,\""
    D_CMND_WC_AE_LEVEL "\":%d,\"" D_CMND_WC_AEC2 "\":%d,\"" D_CMND_WC_AGC "\":%d,\""
    D_CMND_WC_AGC_GAIN "\":%d,\"" D_CMND_WC_GAINCEILING "\":%d,\"" D_CMND_WC_RAW_GMA "\":%d,\""
    D_CMND_WC_LENC "\":%d,\"" D_CMND_WC_WPC "\":%d,\"" D_CMND_WC_DCW "\":%d,\"" D_CMND_WC_BPC "\":%d,\""
    D_CMND_WC_COLORBAR "\":%d,\"" D_CMND_WC_FEATURE "\":%d,\"" D_CMND_WC_AUTH "\":%d,\"" D_CMND_WC_CLK "\":%d,\""
    D_CMND_WC_MENUVIDEOOFF "\":%d"
#ifdef ENABLE_RTSPSERVER
  ",\"" D_CMND_RTSP "\":%d"
#endif // ENABLE_RTSPSERVER
  "}}"),
    Settings->webcam_config.stream, Settings->webcam_config.resolution, Settings->webcam_config.mirror,
    Settings->webcam_config.flip,
    Settings->webcam_config.saturation -2, Settings->webcam_config.brightness -2, Settings->webcam_config.contrast -2,
    Settings->webcam_config2.special_effect, Settings->webcam_config.awb, Settings->webcam_config2.wb_mode,
    Settings->webcam_config.awb_gain, Settings->webcam_config.aec, Settings->webcam_config2.aec_value,
    Settings->webcam_config2.ae_level -2, Settings->webcam_config.aec2, Settings->webcam_config.agc,
    Settings->webcam_config2.agc_gain, Settings->webcam_config2.gainceiling, Settings->webcam_config.raw_gma,
    Settings->webcam_config.lenc, Settings->webcam_config.wpc, Settings->webcam_config.dcw, Settings->webcam_config.bpc,
    Settings->webcam_config.colorbar, Settings->webcam_config.feature, Settings->webcam_config2.auth,
    Settings->webcam_clk, Settings->webcam_config.spare15
#ifdef ENABLE_RTSPSERVER
  , Settings->webcam_config.rtsp
#endif // ENABLE_RTSPSERVER
  );
}

// enable/disable informing Berry of frame arriving
void CmndWebcamBerryFrames(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Wc.berryFrames = XdrvMailbox.payload;
  }
  ResponseCmndStateText(Wc.berryFrames);
}

void CmndWebcamPowerOff(void){
  WcCamOff();
  ResponseCmndDone();
}

void CmndWebcamSetOptions(void){
  int res = WcSetOptions(XdrvMailbox.index, XdrvMailbox.payload);
  ResponseCmndNumber(res);
}

// NOTE input format is esp format + 1, and 0 -> jpeg
// wcConvertFrame1-4 0 [0] - option arg scale 0-3 -> valid on jpeg decode only
void CmndWebcamConvertFrame(void){
  int bnum = XdrvMailbox.index;
  // bnum is 1-4
  if ((bnum < 1) || (bnum > MAX_PICSTORE)){
    ResponseCmndError(); return;
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
    ResponseCmndError(); return;
  }
  struct PICSTORE *ps = &Wc.picstore[bnum-1];
  if (!ps->buff){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: No pic at %d"), bnum);
    ResponseCmndError(); return;
  }
  if (ps->format != PIXFORMAT_JPEG && format != PIXFORMAT_JPEG){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: ConvertFrame only go to or from JPEG"));
    ResponseCmndError(); return;
  }

  // takes INDEX into store
  bool res = WcConvertFrame(bnum-1, format, scale);
  res? ResponseCmndDone(): ResponseCmndError();
  return;
}

// Allows Berry to send native address, len, format, optional width, height
// "addr len format [width height]"
// give it a bad address, and it WILL die.
void CmndWebcamSetPicture(void){
  int bnum = XdrvMailbox.index;
  if (!XdrvMailbox.data_len || bnum < 1 || bnum > MAX_PICSTORE) {
    ResponseCmndError();
    return;
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
    ResponseCmndError(); return;
  }
  if (!wc_check_format(format)){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: Invalid format %d"), format+1);
    ResponseCmndError(); return;
  }
  if (format != PIXFORMAT_JPEG && (!width || !height)){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: SetPicture: format %d needs width and height"), format+1);
    ResponseCmndError(); return;
  }

  bool allocres = pic_alloc(p, width, height, len, format, 1);
  if (!allocres){
    AddLog(LOG_LEVEL_ERROR, PSTR("CAM: SetPicture alloc failed"));
    ResponseCmndError();
    return;
  }

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: SetPicture addr:%u len:%d format%d [width%d height%d]"), addr, len, format, width, height);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: dest addr:%u len:%d/%d format%d [width%d height%d]"), p->buff, p->len, p->allocatedLen, p->format, p->width, p->height);

  // don't over copy if someone screws up height/width/size calc
  // also, our buffer MAY have more space than required...
  int copylen = (len < p->allocatedLen)?len:p->allocatedLen;
  // copy Berry data.  We can't free it, and Berry will
  memcpy(p->buff, (void *)addr, copylen);
  ResponseCmndDone();
  return;
}

void CmndWebcamStartTask(void) {
  if (Wc.taskRunning == 0){
    WCStartOperationTask();
  }
  ResponseCmndDone();
}
void CmndWebcamStopTask(void) {
  if (Wc.taskRunning == 1){
    // set to 2, and wait until cleared
    WcWaitZero(&Wc.taskRunning, 2, 20000);
  }
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

// store a frame 1-4.  If frame '0' is requested, stores a frame in '1'
void CmndWebcamGetFrame(void) {
  // returns size
  int bnum = 0;
  if (XdrvMailbox.index){
    bnum = XdrvMailbox.index;
  } else {
    bnum = XdrvMailbox.payload;
  }
  if (bnum == 0) bnum = 1;
  uint32_t res = WcGetFrame(bnum);
  struct PICSTORE *p = nullptr;
  res = WcGetPicstorePtr(bnum-1, &p);
  char resp[100] = "0";
  if (p) {
    snprintf_P(resp, sizeof(resp), PSTR("{\"buff\":%d,\"addr\":%d,\"len\":%d,\"w\":%d,\"h\":%d,\"format\":%d}"), 
      bnum, p->buff, p->len, p->width, p->height, p->format+1);
  } else {
    snprintf_P(resp, sizeof(resp), PSTR("{\"maxstore\":%d}"), 
      res);
  }
  Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, resp);

  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Getframe %d -> %d"), bnum, res);
  ResponseCmndNumber((int)res);
}

// get a stored frame.  If frame '0' is requested, stores a frame in '1', and returns that
void CmndWebcamGetPicStore(void) {
  // returns size
  // use a dummy for buffer ptr
  uint8_t *t = nullptr;
  int bnum = XdrvMailbox.payload;
  if (bnum == -99){
    bnum = XdrvMailbox.index;
  }
  if (bnum < 0 || bnum > MAX_PICSTORE) {
    ResponseCmndError();
    return;
  }

  // if given 0, then get frame 1 first, and use frame 1 (the first frame, index 0).
  if (bnum == 0){
    bnum = 1;
    uint32_t res = WcGetFrame(bnum);
  }
  //uint32_t res = WcGetPicstore(bnum-1, &t);
  struct PICSTORE *p = nullptr;
  uint32_t res = WcGetPicstorePtr(bnum-1, &p);
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: PicStore %d at 0x%x"), bnum, p);

  char resp[100] = "0";
  if (p) {
    snprintf_P(resp, sizeof(resp), PSTR("{\"buff\":%d,\"addr\":%d,\"len\":%d,\"w\":%d,\"h\":%d,\"format\":%d}"), 
      bnum, p->buff, p->len, p->width, p->height, p->format+1);
  } else {
    snprintf_P(resp, sizeof(resp), PSTR("{\"maxstore\":%d}"), 
      res);
  }
  Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, resp);
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
      ResponseCmndError();
      return;
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
    ResponseCmndError();
    return;
  }

  if (bnum > 1){
    bool res = pic_alloc(&Wc.picstore[bnum-1], p->width, p->height, 0, p->format, WC_ALLOC_ALWAYS);
    if (res){
      memcpy(Wc.picstore[bnum-1].buff, p->buff, p->len);
      p = &Wc.picstore[bnum-1];
    } else {
      ResponseCmndError();
      return;
    }
  }

  char resp[100] = "0";
  snprintf_P(resp, sizeof(resp), PSTR("{\"buff\":%d,\"addr\":%d,\"len\":%d,\"w\":%d,\"h\":%d,\"format\":%d}"), 
      bnum, p->buff, p->len, p->width, p->height, p->format+1);
  Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, resp);
}

// todo - get raw pixels from camera.
// we probably need to specify size/window?
void CmndWebcamGetCamPixels(void) {
  // NOTE: the buffers returned here are static unless the frame size or scale changes.
  // use with care
  uint8_t *t = nullptr;
  int len = 0;
  switch (XdrvMailbox.index){
    case 1:{ // colour
    } break;
    case 2:{ // mono
    } break;
  }
  char resp[50] = "0";
  snprintf_P(resp, sizeof(resp), PSTR("{\"addr\":%d,\"len\":%d}"), t, len);
  Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, resp);
}

int WebcamSavePic(int append) {
  // returns size
  // use a dummy for buffer ptr
  uint8_t *buf = nullptr;
  int len = 0;
  int bnum = XdrvMailbox.index;
  if (bnum < 0){
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: savePic bnum %d"), bnum);
    return 0;
  }

  if (0 == XdrvMailbox.data_len){
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Failed Save Pic no fname"));
    return 0;
  }

  if (bnum == 0){
    // if bnum 0, get a picture now....
    // use the snapshot frame buffer
    Wc.taskTakePic = 1;
    int loops = 100;
    while (Wc.taskTakePic && (loops--)){
      vTaskDelay(10/ portTICK_PERIOD_MS);
    }
  }
  TasAutoMutex localmutex(&WebcamMutex, "SavePic", 2000);
  if (bnum == 0){
    buf = Wc.snapshotStore.buff;
    len = Wc.snapshotStore.len;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: savePic snapshotstore %d"), len);
  } else {
    len = WcGetPicstore(bnum - 1, &buf);
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: savePic PicStore %d -> %d"), bnum, len);
  }
  if (len){
#ifdef USE_UFILESYS
    // ffsp should recognise /sd/ if sd card present
    if (ffsp){
      File f = ffsp->open(XdrvMailbox.data, append?FS_FILE_APPEND:FS_FILE_WRITE); //
      if (f){
        f.write(buf, len);
        f.close();
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Save Pic %s"), XdrvMailbox.data);
        if (bnum == 0){
          pic_free(&Wc.snapshotStore);
        }
        return 1;
      }
    }
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Failed Save Pic %s"), XdrvMailbox.data);
#else
    AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Failed Save Pic %s - no USE_UFILESYS"), XdrvMailbox.data);
#endif
    if (bnum == 0){
      pic_free(&Wc.snapshotStore);
    }
    return 0;
  } 
  AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Failed Save Pic invalid index %d"), XdrvMailbox.payload);
  return 0;
}
// "WCSAVEPIC1 /temp.jpg" "WCSAVEPIC2 /temp.jpg"
void CmdWebcamSavePic(){
  WebcamSavePic(0)? ResponseCmndDone(): ResponseCmndError();
}
// "WCAPPENDPIC1 /temp.jpg" "WCAPPENDPIC2 /temp.jpg"
void CmdWebcamAppendPic(){
  WebcamSavePic(1)? ResponseCmndDone(): ResponseCmndError();
}

void CmndWebcamMenuVideoOff(void) {
  Settings->webcam_config.spare15 = 1;
  ResponseCmndStateText(Settings->webcam_config.spare15);
}

void CmndWebcamMenuVideoOn(void) {
  Settings->webcam_config.spare15 = 0;
  ResponseCmndStateText(Settings->webcam_config.spare15);
}

void CmndWebcamStream(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.stream = XdrvMailbox.payload;
    WcSetStreamserver(Settings->webcam_config.stream);
  } else {
    // we use this from a menu
    if (XdrvMailbox.index == 2){
      Settings->webcam_config.stream = 1;
      WcSetStreamserver(Settings->webcam_config.stream);
    }
  }
  ResponseCmndStateText(Settings->webcam_config.stream);
}

void CmndWebcamResolution(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < FRAMESIZE_FHD)) {
    Settings->webcam_config.resolution = XdrvMailbox.payload;
    WcSetOptions(0, Settings->webcam_config.resolution);
  }
  ResponseCmndNumber(Settings->webcam_config.resolution);
}

void CmndWebcamMirror(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.mirror = XdrvMailbox.payload;
    WcSetOptions(3, Settings->webcam_config.mirror);
  }
  ResponseCmndStateText(Settings->webcam_config.mirror);
}

void CmndWebcamFlip(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.flip = XdrvMailbox.payload;
    WcSetOptions(2, Settings->webcam_config.flip);
  }
  ResponseCmndStateText(Settings->webcam_config.flip);
}

void CmndWebcamSaturation(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config.saturation = XdrvMailbox.payload +2;
    WcSetOptions(6, Settings->webcam_config.saturation -2);
  }
  ResponseCmndNumber(Settings->webcam_config.saturation -2);
}

void CmndWebcamBrightness(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config.brightness = XdrvMailbox.payload +2;
    WcSetOptions(5, Settings->webcam_config.brightness -2);
  }
  ResponseCmndNumber(Settings->webcam_config.brightness -2);
}

void CmndWebcamContrast(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config.contrast = XdrvMailbox.payload +2;
    WcSetOptions(4, Settings->webcam_config.contrast -2);
  }
  ResponseCmndNumber(Settings->webcam_config.contrast -2);
}

void CmndWebcamSpecialEffect(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
    Settings->webcam_config2.special_effect = XdrvMailbox.payload;
    WcSetOptions(1, Settings->webcam_config2.special_effect);
  }
  ResponseCmndNumber(Settings->webcam_config2.special_effect);
}

void CmndWebcamAWB(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.awb = XdrvMailbox.payload;
    WcSetOptions(7, Settings->webcam_config.awb);
  }
  ResponseCmndStateText(Settings->webcam_config.awb);
}

void CmndWebcamWBMode(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 4)) {
    Settings->webcam_config2.wb_mode = XdrvMailbox.payload;
    WcSetOptions(8, Settings->webcam_config2.wb_mode);
  }
  ResponseCmndNumber(Settings->webcam_config2.wb_mode);
}

void CmndWebcamAWBGain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.awb_gain = XdrvMailbox.payload;
    WcSetOptions(9, Settings->webcam_config.awb_gain);
  }
  ResponseCmndStateText(Settings->webcam_config.awb_gain);
}

void CmndWebcamAEC(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.aec = XdrvMailbox.payload;
    WcSetOptions(10, Settings->webcam_config.aec);
  }
  ResponseCmndStateText(Settings->webcam_config.aec);
}

void CmndWebcamAECValue(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1200)) {
    Settings->webcam_config2.aec_value = XdrvMailbox.payload;
    WcSetOptions(11, Settings->webcam_config2.aec_value);
  }
  ResponseCmndNumber(Settings->webcam_config2.aec_value);
}

void CmndWebcamAELevel(void) {
  if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config2.ae_level = XdrvMailbox.payload + 2;
    WcSetOptions(12, Settings->webcam_config2.ae_level - 2);
  }
  ResponseCmndNumber(Settings->webcam_config2.ae_level - 2);
}

void CmndWebcamAEC2(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.aec2 = XdrvMailbox.payload;
    WcSetOptions(13, Settings->webcam_config.aec2);
  }
  ResponseCmndStateText(Settings->webcam_config.aec2);
}

void CmndWebcamAGC(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.agc = XdrvMailbox.payload;
    WcSetOptions(14, Settings->webcam_config.agc);
  }
  ResponseCmndStateText(Settings->webcam_config.agc);
}

void CmndWebcamAGCGain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 30)) {
    Settings->webcam_config2.agc_gain = XdrvMailbox.payload;
    WcSetOptions(15, Settings->webcam_config2.agc_gain);
  }
  ResponseCmndNumber(Settings->webcam_config2.agc_gain);
}

void CmndWebcamGainCeiling(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
    Settings->webcam_config2.gainceiling = XdrvMailbox.payload;
    WcSetOptions(16, Settings->webcam_config2.gainceiling);
  }
  ResponseCmndNumber(Settings->webcam_config2.gainceiling);
}

void CmndWebcamGammaCorrect(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.raw_gma = XdrvMailbox.payload;
    WcSetOptions(17, Settings->webcam_config.raw_gma);
  }
  ResponseCmndStateText(Settings->webcam_config.raw_gma);
}

void CmndWebcamLensCorrect(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.lenc = XdrvMailbox.payload;
    WcSetOptions(18, Settings->webcam_config.lenc);
  }
  ResponseCmndStateText(Settings->webcam_config.lenc);
}

void CmndWebcamWPC(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.wpc = XdrvMailbox.payload;
    WcSetOptions(19, Settings->webcam_config.wpc);
  }
  ResponseCmndStateText(Settings->webcam_config.wpc);
}

void CmndWebcamDCW(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.dcw = XdrvMailbox.payload;
    WcSetOptions(20, Settings->webcam_config.dcw);
  }
  ResponseCmndStateText(Settings->webcam_config.dcw);
}

void CmndWebcamBPC(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.bpc = XdrvMailbox.payload;
    WcSetOptions(21, Settings->webcam_config.bpc);
  }
  ResponseCmndStateText(Settings->webcam_config.bpc);
}

void CmndWebcamColorbar(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    Settings->webcam_config.colorbar = XdrvMailbox.payload;
    WcSetOptions(22, Settings->webcam_config.colorbar);
  }
  ResponseCmndStateText(Settings->webcam_config.colorbar);
}

void CmndWebcamFeature(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
    Settings->webcam_config.feature = XdrvMailbox.payload;
    WcSetOptions(23, Settings->webcam_config.feature);
  }
  ResponseCmndNumber(Settings->webcam_config.feature);
}

void CmndWebcamAuth(void){
  if((XdrvMailbox.payload >=0) && (XdrvMailbox.payload <= 1)){
    Settings->webcam_config2.auth = XdrvMailbox.payload;
  }
  ResponseCmndNumber(Settings->webcam_config2.auth);
}

void CmndWebcamClock(void){
  if((XdrvMailbox.payload >= 10) && (XdrvMailbox.payload <= 200)){
    Settings->webcam_clk = XdrvMailbox.payload;
    // if cam is up, must setup to apply
    if (Wc.up){
      WcSetup(Settings->webcam_config.resolution);
    }
  }
  ResponseCmndNumber(Settings->webcam_clk);
}

void CmndWebcamCamStartStop(void){
  if((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)){
    WcInterrupt(XdrvMailbox.payload);
  }
  ResponseCmndNumber(XdrvMailbox.payload);
}


void CmndWebcamInit(void) {
  WcSetup(Settings->webcam_config.resolution);
  WcSetStreamserver(Settings->webcam_config.stream);
  ResponseCmndDone();
}

void CmndWebcamSetDefaults(void) {
  WcSetDefaults(0);
  ResponseCmndDone();
}

#define D_WEBCAM_STATS_FPS "FPS"
#define D_WEBCAM_STATS_CAMFAIL "CamFail"
#define D_WEBCAM_STATS_JPEGFAIL "JpegFail"
#define D_WEBCAM_STATS_CLIENTFAIL "ClientFail"
#define D_WEBCAM_STATS_AVGFPS "AvgFPS"
#define D_WEBCAM_STATS_AVGPERIOD "AvgFPeriod"
#define D_WEBCAM_STATS_AVGPROC "AvgFProcessing"
#define D_WEBCAM_STATS_FRAMELEN "LastFrameBytes"
#define D_WEBCAM_STATS_WEBCLIENTS "WebClients"
#define D_WEBCAM_STATS_RTSPCLIENTS "RTSPClients"
#define D_WEBCAM_STATS_LASTCAMINTERVAL "CamInterval"
#define D_WEBCAM_STATS_CAMFRAMETIME "CamFrameTime"

void CmndWebcamStats(void) {
  Response_P(PSTR("{\"" D_PRFX_WEBCAM D_CMND_WC_STATS "\":{\"" D_WEBCAM_STATS_FPS "\":%d,\""
    D_WEBCAM_STATS_CAMFAIL "\":%d,\"" D_WEBCAM_STATS_JPEGFAIL "\":%d,\"" D_WEBCAM_STATS_CLIENTFAIL "\":%d,\""
    D_WEBCAM_STATS_AVGFPS "\":%f,\""
    D_WEBCAM_STATS_AVGPERIOD "\":%d,\""
    D_WEBCAM_STATS_AVGPROC "\":%d,\""
    D_WEBCAM_STATS_FRAMELEN "\":%d,\""
    D_WEBCAM_STATS_WEBCLIENTS "\":%d,\""
    D_WEBCAM_STATS_RTSPCLIENTS "\":%d,\""
    D_WEBCAM_STATS_LASTCAMINTERVAL "\":%d,\""
    D_WEBCAM_STATS_CAMFRAMETIME "\":%d"
  "}}"),
  WcStats.camfps, WcStats.camfail, WcStats.jpegfail, WcStats.clientfail,
  WcStats.avgFPS, WcStats.avgFrameMS, WcStats.avgProcessingPerFrameMS,
  Wc.last_frame_len,
  WcStats.webclientcount,
  WcStats.rtspclientcount,
  Wc.camtimediff,
  Wc.frameIntervalsus
  );
}

#ifdef ENABLE_RTSPSERVER
void CmndWebRtsp(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    if (Settings->webcam_config.rtsp != XdrvMailbox.payload){
      Settings->webcam_config.rtsp = XdrvMailbox.payload;
      if (!Settings->webcam_config.rtsp){
        // existing sessions will get stopped, and the server deleted in the loop.
      }
    }
  }
  ResponseCmndStateText(Settings->webcam_config.rtsp);
}
#endif // ENABLE_RTSPSERVER

void WcUpdateStats(void) {
  WcStats.camfps = WcStats.camcnt;
  WcStats.camcnt = 0;
}

#ifndef D_WEBCAM_STATE
#define D_WEBCAM_STATE "State"
#define D_WEBCAM_POWEREDOFF "PowerOff"
#define D_WEBCAM_FRAMEFAIL "Frame Failed"
#define D_WEBCAM_HWFAIL "H/W fail"
#endif

const char HTTP_WEBCAM_FPS[] PROGMEM = "{s}%s " D_FRAME_RATE "{m}%d/%d " D_UNIT_FPS  "{e}";
const char HTTP_WEBCAM_STATE[] PROGMEM = "{s}%s" D_WEBCAM_STATE "{m}%s - %X{e}";

void WcStatsShow(void) {
#ifdef USE_WEBSERVER
  switch(Wc.lastCamError){
    case ESP_OK: WSContentSend_PD(HTTP_WEBCAM_FPS, WcStats.name, WcStats.camfps, WcStats.maxfps); break;
    case 2: WSContentSend_PD(HTTP_WEBCAM_STATE, WcStats.name, D_WEBCAM_FRAMEFAIL, Wc.lastCamError); break;
    case 3: WSContentSend_PD(HTTP_WEBCAM_STATE, WcStats.name, D_WEBCAM_POWEREDOFF, Wc.lastCamError); break;
    default:  WSContentSend_PD(HTTP_WEBCAM_STATE, WcStats.name, D_WEBCAM_HWFAIL, Wc.lastCamError); break;
  }
#endif  // USE_WEBSERVER
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv99(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_LOOP:
      // note: this will wait for a frame.
      // and so slow down the whole of tas looping?
      WcLoop();
      break;
    case FUNC_WEB_ADD_HANDLER:
      WcPicSetup();
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
      WcShowStream();
      break;
    case FUNC_EVERY_SECOND:
      WcUpdateStats();
      break;
    case FUNC_WEB_SENSOR:
      WcStatsShow();
      break;
    case FUNC_COMMAND:
      result = DecodeCommand(kWCCommands, WCCommand);
      break;
    case FUNC_PRE_INIT:
      memset(&wc_motion, 0, sizeof(wc_motion));
      WcSetMotionDefaults();
      WcInit();
      break;
    case FUNC_INIT:
      // starts stream server if configured, and configured camera 
      WcSetup(Settings->webcam_config.resolution);
      WcSetStreamserver(Settings->webcam_config.stream);
      WCStartOperationTask();
      break;
    case FUNC_SAVE_BEFORE_RESTART: {
      // stop cam clock
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: FUNC_SAVE_BEFORE_RESTART"));
      // turn off camera
      CmndWebcamStopTask();
      // this stops the camera clock, and sets
      // a boolean which prevents us starting it
      WcInterrupt(0);

      if (Wc.up){
        // kill the camera driver, and power off camera
        WcCamOff();
        AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: Deinit Restart"));
      }
      WcSetStreamserver(0);
      // give it a moment for any tasks to finish
      vTaskDelay(100 / portTICK_PERIOD_MS);
      AddLog(LOG_LEVEL_DEBUG, PSTR("CAM: FUNC_SAVE_BEFORE_RESTART after delay"));
    } break;

  }
  return result;
}

#endif  // !USE_WEBCAM_LEGACY
#endif  // USE_WEBCAM
#endif  // ESP32
