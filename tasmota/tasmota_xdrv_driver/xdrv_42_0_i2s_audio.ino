/*
  xdrv_42_i2s_audio.ino - Audio dac support for Tasmota

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
#if ESP8266 || (ESP_IDF_VERSION_MAJOR < 5)
#if (defined(USE_I2S_AUDIO) || defined(USE_TTGO_WATCH) || defined(USE_M5STACK_CORE2) || defined(ESP32S3_BOX))
/*********************************************************************************************\
 * I2S support using an external DAC or a speaker connected to GPIO03 using a transistor
 *
 * Uses fixed GPIOs for ESP8266:
 * I2S Out Data         GPIO03 (Rx)
 * I2S Out Bit Clock    GPIO15
 * I2S Out Word Select  GPIO02
 * I2S In Data          GPIO12
 * I2S In Bit Clock     GPIO13
 * I2S In Word Select   GPIO14
\*********************************************************************************************/

#define XDRV_42           42

#define USE_I2S_EXTERNAL_DAC   1
//#define USE_I2S_NO_DAC                         // Add support for transistor-based output without DAC
//#define USE_I2S_WEBRADIO                       // Add support for web radio
//#define USE_I2S_SAY_TIME                       // Add support for english speaking clock
//#define USE_I2S_RTTTL                          // Add support for Rtttl playback
//#define USE_LSB                                // Add support for LSBJ chips, e.g. TM8211/PT8211
// Microphone support
//#define USE_I2S_MIC                            // Add support for I2S microphone
  //#define MIC_CHANNELS 1                       // 1 = mono (I2S_CHANNEL_FMT_ONLY_RIGHT), 2 = stereo (I2S_CHANNEL_FMT_RIGHT_LEFT)
  //#define MICSRATE 32000                       // Set sample rate
  //#define USE_INMP441                          // Add support for INMP441 MEMS microphone
  //#define MIC_PDM                              // Set microphone as PDM (only on ESP32)
//#define USE_SHINE                              // Use MP3 encoding (only on ESP32 with PSRAM)
//#define MP3_MIC_STREAM                         // Add support for streaming microphone via http (only on ESP32 with PSRAM)
  //#define MP3_STREAM_PORT 81                   // Choose MP3 stream port (default = 81)
//#define I2S_BRIDGE                             // Add support for UDP PCM audio bridge
  //#define I2S_BRIDGE_PORT    6970              // Set bridge port (default = 6970)

#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#ifdef USE_I2S_NO_DAC
  #include "AudioOutputI2SNoDAC.h"  // Transistor-driven lower quality connected to RX pin
#else
  #include "AudioOutputI2S.h"       // External I2S DAC IC
#endif  // USE_I2S_NO_DAC
#include <ESP8266SAM.h>
#include "AudioFileSourceFS.h"
#ifdef USE_I2S_SAY_TIME
  #include "AudioGeneratorTalkie.h"
#endif  // USE_I2S_SAY_TIME
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorAAC.h"

#ifdef ESP32
#include <driver/i2s.h>
#endif

#undef AUDIO_PWR_ON
#undef AUDIO_PWR_OFF
#define AUDIO_PWR_ON
#define AUDIO_PWR_OFF

#ifdef ESP8266
#define i2s_port_t uint8_t
#endif

#define MODE_MIC 1
#define MODE_SPK 2

#ifndef MICSRATE
#define MICSRATE 32000
#endif


typedef union {
  uint8_t data;
  struct {
    uint8_t master : 1;
    uint8_t enabled : 1;
    uint8_t swap_mic : 1;
    uint8_t mode : 2;
  };
} BRIDGE_MODE;


struct AUDIO_I2S_t {
  uint8_t is2_volume; // should be in settings
  i2s_port_t i2s_port;
  int8_t mclk = -1;
  int8_t bclk = -1;
  int8_t ws = -1;
  int8_t dout = -1;
  int8_t din = -1;
  AudioGeneratorMP3 *mp3 = nullptr;
  AudioFileSourceFS *file;
#ifdef USE_I2S_NO_DAC
  AudioOutputI2SNoDAC *out;
#else
  AudioOutputI2S *out;
#endif  // USE_I2S_NO_DAC
  AudioFileSourceID3 *id3;
  AudioGeneratorMP3 *decoder = NULL;
  void *mp3ram = NULL;
#ifdef USE_I2S_WEBRADIO
  AudioFileSourceICYStream *ifile = NULL;
  AudioFileSourceBuffer *buff = NULL;
  char wr_title[64];
  void *preallocateBuffer = NULL;
  void *preallocateCodec = NULL;
  uint32_t retryms = 0;
#endif  // USE_I2S_WEBRADIO

#ifdef ESP32
  TaskHandle_t mp3_task_h;
  TaskHandle_t mic_task_h;
#endif // ESP32
  uint32_t mic_size;
  uint32_t mic_rate;
  uint8_t *mic_buff;
  char mic_path[32];
  uint8_t mic_channels;
  File fwp;
  uint8_t mic_stop;
  int8_t mic_error;
  int8_t mic_mclk = -1;
  int8_t mic_bclk = -1;
  int8_t mic_ws = -1;
  int8_t mic_din = -1;
  int8_t mic_dout = -1;
  uint8_t mic_gain = 1;
  bool use_stream = false;
  i2s_port_t mic_port;


#ifdef USE_SHINE
  uint32_t recdur;
  uint8_t  stream_active;
  uint8_t  stream_enable;
  WiFiClient client;
  ESP8266WebServer *MP3Server;
#endif

  uint8_t mode;

#ifdef I2S_BRIDGE
  BRIDGE_MODE bridge_mode;
  WiFiUDP i2s_bridge_udp;
  WiFiUDP i2s_bridgec_udp;
  IPAddress i2s_bridge_ip;
  TaskHandle_t i2s_bridge_h;
  int8_t ptt_pin = -1;
#endif


} audio_i2s;

#ifndef MIC_CHANNELS
#define MIC_CHANNELS 1
#endif

#ifdef USE_TTGO_WATCH
#undef AUDIO_PWR_ON
#undef AUDIO_PWR_OFF
#define AUDIO_PWR_ON TTGO_audio_power(true);
#define AUDIO_PWR_OFF TTGO_audio_power(false);
#endif  // USE_TTGO_WATCH

#ifdef USE_M5STACK_CORE2
// leave this predefined currently
#undef AUDIO_PWR_ON
#undef AUDIO_PWR_OFF
#define AUDIO_PWR_ON Core2AudioPower(true);
#define AUDIO_PWR_OFF Core2AudioPower(false);
#undef DAC_IIS_BCK
#undef DAC_IIS_WS
#undef DAC_IIS_DOUT
#undef DAC_IIS_DIN
#define DAC_IIS_BCK       12
#define DAC_IIS_WS        0
#define DAC_IIS_DOUT      2
#define DAC_IIS_DIN       34
#undef MICSRATE
#define MICSRATE 32000
#undef MIC_CHANNELS
#define MIC_CHANNELS 1
#endif  // USE_M5STACK_CORE2


#ifdef ESP32S3_BOX
#undef AUDIO_PWR_ON
#undef AUDIO_PWR_OFF
#define AUDIO_PWR_ON S3boxAudioPower(true);
#define AUDIO_PWR_OFF S3boxAudioPower(false);
#undef MIC_CHANNELS
#define MIC_CHANNELS 2
#endif // ESP32S3_BOX

extern FS *ufsp;
extern FS *ffsp;

#ifdef ESP8266
const int preallocateBufferSize = 5*1024;
const int preallocateCodecSize = 29192; // MP3 codec max mem needed
#endif  // ESP8266

#ifdef ESP32
const int preallocateBufferSize = 16*1024;
const int preallocateCodecSize = 29192; // MP3 codec max mem needed
//const int preallocateCodecSize = 85332; // AAC+SBR codec max mem needed
#endif  // ESP32

enum : int { APLL_AUTO = -1, APLL_ENABLE = 1, APLL_DISABLE = 0 };
enum : int { EXTERNAL_I2S = 0, INTERNAL_DAC = 1, INTERNAL_PDM = 2 };

#ifdef ESP8266
#define I2S_MCLK_MULTIPLE_128 0
#endif

void sayTime(int hour, int minutes);
void Cmd_MicRec(void);
void Cmd_wav2mp3(void);
void Cmd_Time(void);
#ifdef USE_I2S_RTTTL
void Rtttl(char *buffer);
void Cmd_I2SRtttl(void);
#endif

void copy_micpars(uint32_t port) {
  audio_i2s.mic_mclk = audio_i2s.mclk;
  audio_i2s.mic_bclk = audio_i2s.bclk;
  audio_i2s.mic_ws = audio_i2s.ws;
  audio_i2s.mic_dout = audio_i2s.dout;
  audio_i2s.mic_din = audio_i2s.din;
  audio_i2s.mic_port = (i2s_port_t)port;
}

int32_t I2S_Init_0(void) {

  audio_i2s.i2s_port = (i2s_port_t)0;
  audio_i2s.mic_port = (i2s_port_t)0;

#if USE_I2S_EXTERNAL_DAC
  // use i2s
#if (defined(USE_I2S_NO_DAC) && defined(DAC_IIS_DOUT)) || (defined(DAC_IIS_BCK) && defined(DAC_IIS_WS) && defined(DAC_IIS_DOUT))
    audio_i2s.i2s_port = (i2s_port_t)0;
#ifdef USE_I2S_NO_DAC
    audio_i2s.out = new AudioOutputI2SNoDAC();
#else
    audio_i2s.out = new AudioOutputI2S();
#endif

    audio_i2s.bclk = DAC_IIS_BCK;
    audio_i2s.ws = DAC_IIS_WS;
    audio_i2s.dout = DAC_IIS_DOUT;
    audio_i2s.din = DAC_IIS_DIN;

    copy_micpars(0);

#else
#ifdef USE_I2S_NO_DAC
  if (PinUsed(GPIO_I2S_DOUT)) {
#else
  if (PinUsed(GPIO_I2S_BCLK) && PinUsed(GPIO_I2S_WS) && PinUsed(GPIO_I2S_DOUT)) {
#endif // USE_I2S_NO_DAC
    audio_i2s.i2s_port = (i2s_port_t)0;
#ifdef USE_I2S_NO_DAC
    audio_i2s.out = new AudioOutputI2SNoDAC();
#else
    //audio_i2s.out = new AudioOutputI2S(audio_i2s.i2s_port);
    audio_i2s.out = new AudioOutputI2S(audio_i2s.i2s_port, EXTERNAL_I2S, 8, APLL_DISABLE, I2S_MCLK_MULTIPLE_128, 12000000);
#endif // USE_I2S_NO_DAC
    audio_i2s.mclk = Pin(GPIO_I2S_MCLK);
    audio_i2s.bclk = Pin(GPIO_I2S_BCLK);
    audio_i2s.ws = Pin(GPIO_I2S_WS);
    audio_i2s.dout = Pin(GPIO_I2S_DOUT);
    audio_i2s.din = Pin(GPIO_I2S_DIN);

    copy_micpars(0);

    // check if 2 ports used, use second for micro
    if (PinUsed(GPIO_I2S_BCLK, 1) && PinUsed(GPIO_I2S_WS, 1) && PinUsed(GPIO_I2S_DIN, 1)) {
      audio_i2s.mic_bclk = -1;
      audio_i2s.mic_bclk = Pin(GPIO_I2S_BCLK, 1);
      audio_i2s.mic_ws = Pin(GPIO_I2S_WS, 1);
      audio_i2s.mic_dout = -1;
      audio_i2s.mic_din = Pin(GPIO_I2S_DIN, 1);
      audio_i2s.mic_port = (i2s_port_t)1;
    }
  } else if (PinUsed(GPIO_I2S_BCLK, 1) && PinUsed(GPIO_I2S_WS, 1) && PinUsed(GPIO_I2S_DOUT, 1)) {
    audio_i2s.i2s_port = (i2s_port_t)1;
#ifdef USE_I2S_NO_DAC
    audio_i2s.out = new AudioOutputI2SNoDAC();
#else
    //audio_i2s.out = new AudioOutputI2S(audio_i2s.i2s_port);
    audio_i2s.out = new AudioOutputI2S(audio_i2s.i2s_port, EXTERNAL_I2S, 8, APLL_DISABLE, I2S_MCLK_MULTIPLE_128, 12000000);
#endif // USE_I2S_NO_DAC
    audio_i2s.mclk = Pin(GPIO_I2S_MCLK, 1);
    audio_i2s.bclk = Pin(GPIO_I2S_BCLK, 1);
    audio_i2s.ws = Pin(GPIO_I2S_WS, 1);
    audio_i2s.dout = Pin(GPIO_I2S_DOUT, 1);
    audio_i2s.din = Pin(GPIO_I2S_DIN, 1);

    copy_micpars(1);

  } else {
    return -1;
  }
#ifdef ESP8266
  // esp8266 have fixed pins
  if  ((audio_i2s.bclk != 15) || (audio_i2s.ws != 2) || (audio_i2s.dout != 3)) {
    return -2;
  }
#endif // ESP8266
#endif // defined(DAC_IIS_BCK)

  audio_i2s.out->SetPinout(audio_i2s.bclk, audio_i2s.ws, audio_i2s.dout, audio_i2s.mclk, audio_i2s.din);

  AddLog(LOG_LEVEL_INFO, PSTR("Init audio I2S: port=%d, bclk=%d, ws=%d, dout=%d, mclk=%d, din=%d"), audio_i2s.i2s_port, audio_i2s.bclk, audio_i2s.ws, audio_i2s.dout, audio_i2s.mclk, audio_i2s.din);
  if (audio_i2s.mic_port != 0) {
    AddLog(LOG_LEVEL_INFO, PSTR("Init audio I2S mic: port=%d, bclk=%d, ws=%d, din=%d"), audio_i2s.mic_port, audio_i2s.mic_bclk, audio_i2s.mic_ws, audio_i2s.mic_din);
  }

#ifdef USE_I2S_LSB
  audio_i2s.out->SetLsbJustified(true);
#endif // Allow supporting LSBJ chips, e.g. TM8211/PT8211

#else

#ifdef USE_I2S_NO_DAC
  audio_i2s.out = new AudioOutputI2SNoDAC();
#else
  audio_i2s.out = new AudioOutputI2S(0, 1);    // Internal DAC port 0
#endif  // USE_I2S_NO_DAC

#endif  // USE_I2S_EXTERNAL_DAC

  audio_i2s.mode = MODE_SPK;

  return 0;
}

void I2S_Init(void) {

  if (I2S_Init_0()) {
    return;
  }

#if defined(ESP32) && defined(ESP32S3_BOX)
  S3boxInit();
#endif

#ifdef USE_W8960
  W8960_Init1();
#endif

  audio_i2s.is2_volume = 10;
  audio_i2s.out->SetGain(((float)audio_i2s.is2_volume / 100.0) * 4.0);
  audio_i2s.out->begin();
  audio_i2s.out->stop();
  audio_i2s.mp3ram = nullptr;

#ifdef ESP32
  if (UsePSRAM()) {
    audio_i2s.mp3ram = heap_caps_malloc(preallocateCodecSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  }

#ifdef USE_I2S_WEBRADIO
  if (UsePSRAM()) {
    audio_i2s.preallocateBuffer = heap_caps_malloc(preallocateBufferSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    audio_i2s.preallocateCodec = heap_caps_malloc(preallocateCodecSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    audio_i2s.preallocateBuffer = malloc(preallocateBufferSize);
    audio_i2s.preallocateCodec = malloc(preallocateCodecSize);
  }
  if (!audio_i2s.preallocateBuffer || !audio_i2s.preallocateCodec) {
    //Serial.printf_P(PSTR("FATAL ERROR:  Unable to preallocate %d bytes for app\n"), preallocateBufferSize+preallocateCodecSize);
  }
#endif  // USE_I2S_WEBRADIO

  audio_i2s.mic_channels = MIC_CHANNELS;
  audio_i2s.mic_rate = MICSRATE;

#ifdef USE_I2S_COMMON_IO
  i2s_set_clk(audio_i2s.mic_port, audio_i2s.mic_rate, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_STEREO);
#endif

#endif  // ESP32

}

#ifdef ESP32
void mp3_task(void *arg) {
  while (1) {
    while (audio_i2s.mp3->isRunning()) {
      if (!audio_i2s.mp3->loop()) {
        audio_i2s.mp3->stop();
        mp3_delete();
        audio_i2s.out->stop();
        if (audio_i2s.mp3_task_h) {
          vTaskDelete(audio_i2s.mp3_task_h);
          audio_i2s.mp3_task_h = 0;
        }
        //mp3_task_h=nullptr;
      }
      delay(1);
    }
  }
}
#endif  // ESP32

#ifdef USE_I2S_WEBRADIO
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *str) {
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) isUnicode; // Punt this ball for now
  (void) ptr;
  if (strstr_P(type, PSTR("Title"))) {
    strncpy(audio_i2s.wr_title, str, sizeof(audio_i2s.wr_title));
    audio_i2s.wr_title[sizeof(audio_i2s.wr_title)-1] = 0;
    //AddLog(LOG_LEVEL_INFO,PSTR("WR-Title: %s"),wr_title);
  } else {
    // Who knows what to do?  Not me!
  }
}

void StatusCallback(void *cbData, int code, const char *string) {
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) code;
  (void) ptr;
  //strncpy_P(status, string, sizeof(status)-1);
  //status[sizeof(status)-1] = 0;
}

void Webradio(const char *url) {
  if (audio_i2s.decoder || audio_i2s.mp3) return;
  if (!audio_i2s.out) return;
  AUDIO_PWR_ON
  audio_i2s.ifile = new AudioFileSourceICYStream(url);
  audio_i2s.ifile->RegisterMetadataCB(MDCallback, NULL);
  audio_i2s.buff = new AudioFileSourceBuffer(audio_i2s.ifile, audio_i2s.preallocateBuffer, preallocateBufferSize);
  audio_i2s.buff->RegisterStatusCB(StatusCallback, NULL);
  audio_i2s.decoder = new AudioGeneratorMP3(audio_i2s.preallocateCodec, preallocateCodecSize);
  audio_i2s.decoder->RegisterStatusCB(StatusCallback, NULL);
  audio_i2s.decoder->begin(audio_i2s.buff, audio_i2s.out);
  if (!audio_i2s.decoder->isRunning()) {
  //  Serial.printf_P(PSTR("Can't connect to URL"));
    StopPlaying();
  //  strcpy_P(status, PSTR("Unable to connect to URL"));
    audio_i2s.retryms = millis() + 2000;
  }

  xTaskCreatePinnedToCore(mp3_task2, "MP3-2", 8192, NULL, 3, &audio_i2s.mp3_task_h, 1);
}

void mp3_task2(void *arg){
  while (1) {
    if (audio_i2s.decoder && audio_i2s.decoder->isRunning()) {
      if (!audio_i2s.decoder->loop()) {
        StopPlaying();
        //retryms = millis() + 2000;
      }
      delay(1);
    }
  }
}

void StopPlaying() {

  if (audio_i2s.mp3_task_h) {
    vTaskDelete(audio_i2s.mp3_task_h);
    audio_i2s.mp3_task_h = nullptr;
  }

  if (audio_i2s.decoder) {
    audio_i2s.decoder->stop();
    delete audio_i2s.decoder;
    audio_i2s.decoder = NULL;
  }

  if (audio_i2s.buff) {
    audio_i2s.buff->close();
    delete audio_i2s.buff;
    audio_i2s.buff = NULL;
  }

  if (audio_i2s.ifile) {
    audio_i2s.ifile->close();
    delete audio_i2s.ifile;
    audio_i2s.ifile = NULL;
  }
  AUDIO_PWR_OFF
}

void Cmd_WebRadio(void) {
  if (!audio_i2s.out) return;

  if (audio_i2s.decoder) {
    StopPlaying();
  }
  if (XdrvMailbox.data_len > 0) {
    Webradio(XdrvMailbox.data);
    ResponseCmndChar(XdrvMailbox.data);
  } else {
    ResponseCmndChar_P(PSTR("Stopped"));
  }
}

#ifdef USE_WEBSERVER
const char HTTP_WEBRADIO[] PROGMEM =
   "{s}" "I2S_WR-Title" "{m}%s{e}";

void I2S_WR_Show(bool json) {
    if (audio_i2s.decoder) {
      if (json) {
        ResponseAppend_P(PSTR(",\"WebRadio\":{\"Title\":\"%s\"}"), audio_i2s.wr_title);
      } else {
        WSContentSend_PD(HTTP_WEBRADIO,audio_i2s.wr_title);
      }
    }
}
#endif  // USE_WEBSERVER

#endif  // USE_I2S_WEBRADIO


#ifdef ESP32
void Play_mp3(const char *path) {
#ifdef USE_UFILESYS
  if (audio_i2s.decoder || audio_i2s.mp3) return;
  if (!audio_i2s.out) return;

  bool I2S_Task;

  if (*path=='+') {
    I2S_Task = true;
    path++;
  } else {
    I2S_Task = false;
  }

  FS *mp3fsp = ufsp;

  if (!strncmp(path, "/ffs", 4)) {
    path += 4;
    mp3fsp = ffsp;
  }

  if (!mp3fsp->exists(path)) {
    return;
  }

  AUDIO_PWR_ON

  audio_i2s.file = new AudioFileSourceFS(*mp3fsp, path);

  audio_i2s.id3 = new AudioFileSourceID3(audio_i2s.file);

  if (audio_i2s.mp3ram) {
    audio_i2s.mp3 = new AudioGeneratorMP3(audio_i2s.mp3ram, preallocateCodecSize);
  } else {
    audio_i2s.mp3 = new AudioGeneratorMP3();
  }
  audio_i2s.mp3->begin(audio_i2s.id3, audio_i2s.out);

  if (I2S_Task) {
    xTaskCreatePinnedToCore(mp3_task, "MP3", 8192, NULL, 3, &audio_i2s.mp3_task_h, 1);
  } else {
#define MP3_TIMEOUT 30000
    uint32_t tout = millis();
    while (audio_i2s.mp3->isRunning()) {
      if (!audio_i2s.mp3->loop()) {
        audio_i2s.mp3->stop();
        break;
      }
      OsWatchLoop();
      if (millis()-tout > MP3_TIMEOUT) {
        break;
      }
    }
    audio_i2s.out->stop();
    mp3_delete();
  }

#endif  // USE_UFILESYS
}

void mp3_delete(void) {
  delete audio_i2s.file;
  delete audio_i2s.id3;
  delete audio_i2s.mp3;
  audio_i2s.mp3=nullptr;
  AUDIO_PWR_OFF
}
#endif  // ESP32

void Say(char *text) {

  if (!audio_i2s.out) return;

  AUDIO_PWR_ON

  audio_i2s.out->begin();
  ESP8266SAM *sam = new ESP8266SAM;
  sam->Say(audio_i2s.out, text);
  delete sam;
  audio_i2s.out->stop();

  AUDIO_PWR_OFF
}


const char kI2SAudio_Commands[] PROGMEM = "I2S|"
  "Say|Gain"
#ifdef USE_I2S_SAY_TIME
  "|Time"
#endif // USE_I2S_SAY_TIME
#ifdef USE_I2S_RTTTL
  "|Rtttl"
#endif
#ifdef ESP32
  "|Play"
#ifdef USE_I2S_WEBRADIO
  "|WR"
#endif  // USE_I2S_WEBRADIO
#if defined(USE_SHINE) && ( (defined(USE_I2S_AUDIO) && defined(USE_I2S_MIC)) || defined(USE_M5STACK_CORE2) || defined(ESP32S3_BOX) )
  "|REC"
  "|MGain"
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
  "|STREAM"
#endif // MP3_MIC_STREAM
#ifdef I2S_BRIDGE
  "|BRIDGE"
#endif // I2S_BRIDGE
#endif // USE_SHINE
#endif  // ESP32
;

void (* const I2SAudio_Command[])(void) PROGMEM = {
  &Cmd_Say, &Cmd_Gain
#ifdef USE_I2S_SAY_TIME
  ,&Cmd_Time
#endif // USE_I2S_SAY_TIME
#ifdef USE_I2S_RTTTL
  ,&Cmd_I2SRtttl
#endif
#ifdef ESP32
  ,&Cmd_Play
#ifdef USE_I2S_WEBRADIO
  ,&Cmd_WebRadio
#endif // USE_I2S_WEBRADIO
#if  defined(USE_SHINE) && ( (defined(USE_I2S_AUDIO) && defined(USE_I2S_MIC)) || defined(USE_M5STACK_CORE2) || defined(ESP32S3_BOX) )
  ,&Cmd_MicRec
  ,&Cmd_MicGain
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
  ,&Cmd_MP3Stream
#endif // MP3_MIC_STREAM
#ifdef I2S_BRIDGE
  ,&Cmd_I2SBridge
#endif // I2S_BRIDGE
#endif // USE_SHINE
#endif // ESP32
};

void Cmd_Play(void) {
  if (XdrvMailbox.data_len > 0) {
    Play_mp3(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void Cmd_Gain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    if (audio_i2s.out) {
      audio_i2s.is2_volume=XdrvMailbox.payload;
      audio_i2s.out->SetGain(((float)(audio_i2s.is2_volume-2)/100.0)*4.0);
    }
  }
  ResponseCmndNumber(audio_i2s.is2_volume);
}

void Cmd_Say(void) {
  if (XdrvMailbox.data_len > 0) {
    Say(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

#ifdef USE_I2S_RTTTL
void Cmd_I2SRtttl(void) {
  if (XdrvMailbox.data_len > 0) {
    Rtttl(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}
#endif

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

void i2s_mp3_loop(void);
void i2s_mp3_init(void);
void MP3ShowStream(void);

bool Xdrv42(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kI2SAudio_Commands, I2SAudio_Command);
      break;
    case FUNC_INIT:
      I2S_Init();
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
      //MP3ShowStream();
      break;
    case FUNC_LOOP:
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
      i2s_mp3_loop();
#endif
#if defined(I2S_BRIDGE) && ( (defined(USE_I2S_AUDIO) && defined(USE_I2S_MIC)) || defined(USE_M5STACK_CORE2) || defined(ESP32S3_BOX) )
      i2s_bridge_loop();
#endif
      break;
    case FUNC_WEB_ADD_HANDLER:
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
      audio_i2s.stream_enable = 1;
      i2s_mp3_init(1);
#endif
#if defined(I2S_BRIDGE) && ( (defined(USE_I2S_AUDIO) && defined(USE_I2S_MIC)) || defined(USE_M5STACK_CORE2) || defined(ESP32S3_BOX) )
      I2SBridgeInit();
#endif
      break;

#ifdef USE_WEBSERVER
#ifdef USE_I2S_WEBRADIO
    case FUNC_WEB_SENSOR:
      I2S_WR_Show(false);
      break;
#endif  // USE_I2S_WEBRADIO
#endif  // USE_WEBSERVER
#ifdef USE_I2S_WEBRADIO
    case FUNC_JSON_APPEND:
      I2S_WR_Show(true);
    break;
#endif  // USE_I2S_WEBRADIO
  }
  return result;
}

#endif // USE_I2S_AUDIO
#endif // ESP8266 || (ESP_IDF_VERSION_MAJOR < 5)
