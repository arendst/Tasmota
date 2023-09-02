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

#if ESP_IDF_VERSION_MAJOR >= 5
#if defined(USE_I2S_AUDIO) 

#define XDRV_42           42

#include "driver/i2s_std.h"
#include "driver/gpio.h"

#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"

#include <ESP8266SAM.h>
#include "AudioFileSourceFS.h"
#include "AudioGeneratorTalkie.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceBuffer.h"
#include "AudioGeneratorAAC.h"


#undef AUDIO_PWR_ON
#undef AUDIO_PWR_OFF
#define AUDIO_PWR_ON
#define AUDIO_PWR_OFF

#define USE_I2S_RTTTL
#define USE_I2S_SAY_TIME


class AudioOutputI2S : public AudioOutput
{
  public:
  AudioOutputI2S(){
    hertz = 44100;
    i2sOn = false;
    bps = I2S_DATA_BIT_WIDTH_16BIT;
    channels = I2S_SLOT_MODE_STEREO;
    mono = false;
    output_mode = EXTERNAL_I2S;
    tx_is_enabled = false;
  }

  ~AudioOutputI2S(){
    if(i2sOn){
        this->stop();
        i2s_del_channel(tx_chan);
    }
  }

  bool SetBitsPerSample(int bits)
  {
    if ( (bits != 16) && (bits != 8) ) return false;
    this->bps = bits;
    return true;
  }

  bool SetChannels(int channels)
  {
    if ( (channels < 1) || (channels > 2) ) return false;
    if (channels == (int)this->channels) return true;
    this->channels = channels;
    return true;
  }

  bool SetRate(int hz){
    if (hz == (int)this->hertz) return true;
    this->hertz = hz;
    if(i2sOn){
      int result = updateClockConfig();
      AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: SetRate: %i - %i"),hz, result);
    }
    return true;
  }

  bool SetPinout(){
    return this->startI2SChannel();
  }

  bool begin(){
    if(tx_is_enabled) return true;
    int result = i2s_channel_enable(tx_chan);
    if(result != 0){
       AddLog(LOG_LEVEL_INFO,PSTR("I2S: Could not enable i2s_channel: %i"), result);
       return false;
    }
    tx_is_enabled = true;
    return true;
  }
  bool stop(){
    i2s_channel_disable(tx_chan);
    tx_is_enabled = false;
    return true;
  }

  bool ConsumeSample(int16_t sample[2])
  {
    if (!tx_is_enabled)
      return false;

    int16_t ms[2];

    ms[0] = sample[0];
    ms[1] = sample[1];
    MakeSampleStereo16( ms );

    if (this->mono) {
      // Average the two samples and overwrite
      int32_t ttl = ms[LEFTCHANNEL] + ms[RIGHTCHANNEL];
      ms[LEFTCHANNEL] = ms[RIGHTCHANNEL] = (ttl>>1) & 0xffff;
    }
      uint32_t s32;
      if (output_mode == INTERNAL_DAC)
      {
        int16_t l = Amplify(ms[LEFTCHANNEL]) + 0x8000;
        int16_t r = Amplify(ms[RIGHTCHANNEL]) + 0x8000;
        s32 = (r << 16) | (l & 0xffff);
      }
      else
      {
        s32 = ((Amplify(ms[RIGHTCHANNEL])) << 16) | (Amplify(ms[LEFTCHANNEL]) & 0xffff);
      }

      size_t i2s_bytes_written;
      i2s_channel_write(tx_chan, (const void*)&s32, sizeof(uint32_t), &i2s_bytes_written, 0);
      return i2s_bytes_written;
  }

  private:
    enum : int { EXTERNAL_I2S = 0, INTERNAL_DAC = 1, INTERNAL_PDM = 2 };
    int output_mode;
    bool i2sOn;
    bool mono;

    i2s_chan_handle_t tx_chan;
    bool tx_is_enabled;

    bool startI2SChannel(){
      i2s_chan_config_t tx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
      i2s_new_channel(&tx_chan_cfg, &tx_chan, NULL);
      i2s_std_config_t tx_std_cfg = {
          .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(hertz),
          .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bps, (i2s_slot_mode_t)channels),
          .gpio_cfg = {
              .mclk = (gpio_num_t)Pin(GPIO_I2S_MCLK),
              .bclk = (gpio_num_t)Pin(GPIO_I2S_BCLK),
              .ws   = (gpio_num_t)Pin(GPIO_I2S_WS),
              .dout = (gpio_num_t)Pin(GPIO_I2S_DOUT),
              .din  = (gpio_num_t)Pin(GPIO_I2S_DIN),
              .invert_flags = {
                  .mclk_inv = false,
                  .bclk_inv = false,
                  .ws_inv   = false,
              },
          },
        };
      i2sOn = (i2s_channel_init_std_mode(tx_chan, &tx_std_cfg) == 0);
      return i2sOn;
    }

    int updateClockConfig(){
      i2s_channel_disable(tx_chan);
      i2s_std_clk_config_t clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(hertz);
#ifdef SOC_I2S_SUPPORTS_APLL
      clk_cfg.clk_src = I2S_CLK_SRC_APLL;
#endif
      int result = i2s_channel_reconfig_std_clock(tx_chan, &clk_cfg );
      if(tx_is_enabled) i2s_channel_enable(tx_chan);
      return result;
    }
};

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

  AudioGeneratorMP3 *mp3 = nullptr;
  AudioFileSourceFS *file;

  AudioOutputI2S *out;

  AudioFileSourceID3 *id3;
  AudioGeneratorMP3 *decoder = NULL;
  void *mp3ram = NULL;

  // Webradio
  AudioFileSourceICYStream *ifile = NULL;
  AudioFileSourceBuffer *buff = NULL;
  char wr_title[64];
  void *preallocateBuffer = NULL;
  void *preallocateCodec = NULL;
  uint32_t retryms = 0;


  TaskHandle_t mp3_task_h;
  TaskHandle_t mic_task_h;

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


// SHINE
  uint32_t recdur;
  uint8_t  stream_active;
  uint8_t  stream_enable;
  WiFiClient client;
  ESP8266WebServer *MP3Server;

  uint8_t mode;

// I2S_BRIDGE
  BRIDGE_MODE bridge_mode;
  WiFiUDP i2s_bridge_udp;
  WiFiUDP i2s_bridgec_udp;
  IPAddress i2s_bridge_ip;
  TaskHandle_t i2s_bridge_h;
  int8_t ptt_pin = -1;


} audio_i2s;

extern FS *ufsp;
extern FS *ffsp;

const int preallocateBufferSize = 16*1024;
const int preallocateCodecSize = 29192; // MP3 codec max mem needed
//const int preallocateCodecSize = 85332; // AAC+SBR codec max mem needed


enum : int { APLL_AUTO = -1, APLL_ENABLE = 1, APLL_DISABLE = 0 };
enum : int { EXTERNAL_I2S = 0, INTERNAL_DAC = 1, INTERNAL_PDM = 2 };


void sayTime(int hour, int minutes);
void Cmd_MicRec(void);
void Cmd_wav2mp3(void);
void Cmd_Time(void);

void Rtttl(char *buffer);
void Cmd_I2SRtttl(void);


// void copy_micpars(uint32_t port) {
//   audio_i2s.mic_mclk = audio_i2s.mclk;
//   audio_i2s.mic_bclk = audio_i2s.bclk;
//   audio_i2s.mic_ws = audio_i2s.ws;
//   audio_i2s.mic_dout = audio_i2s.dout;
//   audio_i2s.mic_din = audio_i2s.din;
//   audio_i2s.mic_port = (i2s_port_t)port;
// }

int32_t I2S_Init_0(void) {
  if(Pin(GPIO_I2S_BCLK) == -1 || Pin(GPIO_I2S_WS) == -1 || Pin(GPIO_I2S_DOUT) == -1){
    return -1;
  }
  audio_i2s.out = new AudioOutputI2S;
  bool result = audio_i2s.out->SetPinout();

  if (result){
    AddLog(LOG_LEVEL_INFO, PSTR("I2S: init pins bclk=%d, ws=%d, dout=%d, mclk=%d, din=%d"), Pin(GPIO_I2S_BCLK) , Pin(GPIO_I2S_WS), Pin(GPIO_I2S_DOUT), Pin(GPIO_I2S_MCLK), Pin(GPIO_I2S_DIN));
  }
  else{
    return -1;
  }
  // if (audio_i2s.mic_port != 0) {
  //   AddLog(LOG_LEVEL_INFO, PSTR("Init audio I2S mic: port=%d, bclk=%d, ws=%d, din=%d"), audio_i2s.mic_port, audio_i2s.mic_bclk, audio_i2s.mic_ws, audio_i2s.mic_din);
  // }

  // audio_i2s.mode = MODE_SPK;

  return 0;
}

void I2S_Init(void) {

  if (I2S_Init_0() != 0) {
    return;
  }

  audio_i2s.is2_volume = 10;
  audio_i2s.out->SetGain(((float)audio_i2s.is2_volume / 100.0) * 4.0);
  audio_i2s.out->begin();
  audio_i2s.out->stop();
  audio_i2s.mp3ram = nullptr;

  if (UsePSRAM()) {
    audio_i2s.mp3ram = heap_caps_malloc(preallocateCodecSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  }

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

  // audio_i2s.mic_channels = MIC_CHANNELS;
  // audio_i2s.mic_rate = MICSRATE;

}

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
     vTaskDelay(pdMS_TO_TICKS(1));
    }
  }
}

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
      vTaskDelay(pdMS_TO_TICKS(1));
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

void Play_mp3(const char *path) {
  if (audio_i2s.decoder || audio_i2s.mp3) return;
  if (!audio_i2s.out) return;

  FS *mp3fsp = ufsp;

  if (!strncmp(path, "/ffs", 4)) {
    path += 4;
    mp3fsp = ffsp;
  }

  if (!mp3fsp->exists(path)) {
    AddLog(LOG_LEVEL_INFO,PSTR("MP3-Title not found: %s"),path);
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

  // Always use a task
  xTaskCreatePinnedToCore(mp3_task, "MP3", 8192, NULL, 3, &audio_i2s.mp3_task_h, 1);
}

void mp3_delete(void) {
  delete audio_i2s.file;
  delete audio_i2s.id3;
  delete audio_i2s.mp3;
  audio_i2s.mp3=nullptr;
  AUDIO_PWR_OFF
}

void Say(char *text) {

  if (!audio_i2s.out) return;

  AUDIO_PWR_ON

  ESP8266SAM *sam = new ESP8266SAM;

  sam->Say(audio_i2s.out, text);
  delete sam;
  audio_i2s.out->stop();

  AUDIO_PWR_OFF
}


const char kI2SAudio_Commands[] PROGMEM = "I2S|"
  "Say|Gain|Time|Rtttl|Play|WR"
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
;

void (* const I2SAudio_Command[])(void) PROGMEM = {
  &Cmd_Say, &Cmd_Gain,&Cmd_Time,&Cmd_I2SRtttl,&Cmd_Play,&Cmd_WebRadio
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

void Cmd_I2SRtttl(void) {
  if (XdrvMailbox.data_len > 0) {
    Rtttl(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

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

#endif  // USE_I2S_AUDIO
#endif //ESP_IDF_VERSION_MAJOR >= 5