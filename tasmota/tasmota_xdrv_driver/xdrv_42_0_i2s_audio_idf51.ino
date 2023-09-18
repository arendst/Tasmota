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
#include "driver/i2s_pdm.h"
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

#include <layer3.h>
#include <types.h>

#undef AUDIO_PWR_ON
#undef AUDIO_PWR_OFF
#define AUDIO_PWR_ON    I2SAudioPower(true);
#define AUDIO_PWR_OFF   I2SAudioPower(false);

#define USE_I2S_SAY
#define USE_I2S_SAY_TIME
#define USE_I2S_RTTTL

/*********************************************************************************************\
 * Driver Settings in memory
\*********************************************************************************************/

typedef struct{
  struct{
    uint32_t version : 8 = 0;

    // runtime options, will be saved but ignored on setting read
    uint32_t duplex : 1 = 0; // depends on GPIO setting and SOC caps, DIN and DOUT on same port in GPIO means -> try to use duplex if possible
    uint32_t tx : 1 = 0; // depends on GPIO setting
    uint32_t rx : 1 = 0; // depends on GPIO setting
  } sys;
  struct {
    uint32_t mode : 2 = 0;        // bit 0+1 STD = 0, PDM = 1, TDM = 2
    uint32_t apll : 1 = 1;        // bit 2 - will be ignored on unsupported SOC's
    uint32_t mono : 1 = 0;        // bit 3  0 = stereo, 1 = mono
    uint32_t codec : 1 = 0;       // bit 4 - S3 box only, unused for now
    uint32_t webradio : 1 = 1;    // bit 5 - allocate buffer for webradio
    uint32_t spare01 : 1 = 1;      // bit 6 - request duplex, means RX and TX on 1 slot
    uint32_t lsbJustified : 1;    // bit 7 - allow supporting LSBJ chips, e.g. TM8211/PT8211
    uint32_t volume : 8 = 10;     // bit 8-15
    uint32_t spare02 : 16;        // bit 16-31
  } tx;
  struct {
    struct{
    uint16_t sample_rate = 32000;
    uint8_t gain = 30;
    uint8_t mode = 0;   //STD = 0, PDM = 1, TDM = 2

    uint8_t slot_mask : 2 = 1; // left = 1 /right = 2 /both = 3
    uint8_t slot_mode : 1 = 0; // mono/stereo - 1 is added for both
    uint8_t codec : 1 = 0; // unused for now
    uint8_t mp3_encoder : 1 = 1; // will be ignored without PS-RAM
    };
  } rx;
} tI2SSettings;

typedef union {
  uint8_t data;
  struct {
    uint8_t master : 1;
    uint8_t enabled : 1;
    uint8_t swap_mic : 1;
    uint8_t mode : 2;
  };
} BRIDGE_MODE;

class TasmotaAudioOutputI2S;

struct AUDIO_I2S_t {
  tI2SSettings *Settings;

  i2s_chan_handle_t rx_handle = nullptr;

  AudioGeneratorMP3 *mp3 = nullptr;
  AudioFileSourceFS *file;

  TasmotaAudioOutputI2S *out;

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


  TaskHandle_t mp3_task_handle;
  TaskHandle_t mic_task_handle;

  uint32_t mic_size;
  uint8_t *mic_buff;
  char mic_path[32];
  File fwp;
  uint8_t mic_stop;
  int8_t mic_error;
  bool use_stream = false;


// SHINE
  uint32_t recdur;
  uint8_t  stream_active;
  uint8_t  stream_enable;
  WiFiClient client;
  ESP8266WebServer *MP3Server;

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

enum : int { EXTERNAL_I2S = 0, INTERNAL_DAC = 1, INTERNAL_PDM = 2 };

void sayTime(int hour, int minutes);
void Cmndwav2mp3(void);
void Cmd_Time(void);

void Rtttl(char *buffer);
void CmndI2SRtttl(void);

/*********************************************************************************************\
 * Class for outputting sound as endpoint for ESP8266Audio library
\*********************************************************************************************/

class TasmotaAudioOutputI2S : public AudioOutput
{
  public:
  TasmotaAudioOutputI2S(){
    hertz = 16000;
    i2sOn = false;
    bps = I2S_DATA_BIT_WIDTH_16BIT;
    mono = (audio_i2s.Settings->tx.mono == 1);
    channels = mono ? I2S_SLOT_MODE_MONO : I2S_SLOT_MODE_STEREO;
    output_mode = EXTERNAL_I2S;
    tx_is_enabled = false;
  }

  ~TasmotaAudioOutputI2S(){
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
    if(i2sOn == false){
      if(audio_i2s.Settings->sys.duplex == 0 && audio_i2s.Settings->sys.rx == 1){
      this->startI2SChannel();
    }
    }
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
    if(audio_i2s.Settings->sys.duplex == 0 && audio_i2s.Settings->sys.rx == 1){
      i2s_del_channel(tx_chan);
      i2sOn = false;
    }
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
      gpio_num_t _DIN = I2S_GPIO_UNUSED;

      i2s_chan_config_t tx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
      if(audio_i2s.Settings->sys.duplex == 1){
        _DIN = (gpio_num_t)Pin(GPIO_I2S_DIN);
        i2s_new_channel(&tx_chan_cfg, &tx_chan, &audio_i2s.rx_handle);
      }
      else{
        i2s_new_channel(&tx_chan_cfg, &tx_chan, NULL);
      }

      i2s_std_config_t tx_std_cfg = {
          .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(hertz),
          .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bps, (i2s_slot_mode_t)channels),
          .gpio_cfg = {
              .mclk = (gpio_num_t)Pin(GPIO_I2S_MCLK),
              .bclk = (gpio_num_t)Pin(GPIO_I2S_BCLK),
              .ws   = (gpio_num_t)Pin(GPIO_I2S_WS),
              .dout = (gpio_num_t)Pin(GPIO_I2S_DOUT),
              .din  =  _DIN,
              .invert_flags = {
                  .mclk_inv = false,
                  .bclk_inv = false,
                  .ws_inv   = false,
              },
          },
        };
      i2sOn = (i2s_channel_init_std_mode(tx_chan, &tx_std_cfg) == 0);
      AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: TX channel with %i bit width on %i channels initialized"),bps, channels);
      if(audio_i2s.Settings->sys.duplex == 1){
        i2s_channel_init_std_mode(audio_i2s.rx_handle, &tx_std_cfg);
        AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: RX channel added in full duplex mode"));
      }
      return i2sOn;
    }

    int updateClockConfig(){
      i2s_channel_disable(tx_chan);
      i2s_std_clk_config_t clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(hertz);
#ifdef SOC_I2S_SUPPORTS_APLL
      if(audio_i2s.Settings->tx.apll == 1){
        clk_cfg.clk_src = I2S_CLK_SRC_APLL;
      }
#endif
      int result = i2s_channel_reconfig_std_clock(tx_chan, &clk_cfg );
      if(tx_is_enabled) i2s_channel_enable(tx_chan);
      return result;
    }
};

/*********************************************************************************************\
 * microphone related functions
\*********************************************************************************************/


uint32_t I2sMicInit(uint8_t enable) {
  esp_err_t err = ESP_OK;
  i2s_slot_mode_t slot_mode = (audio_i2s.Settings->rx.slot_mode == 0) ? I2S_SLOT_MODE_MONO : I2S_SLOT_MODE_STEREO;
  gpio_num_t _CLK;

  if(audio_i2s.Settings->sys.duplex == 1 && audio_i2s.rx_handle != nullptr){
    return 0; // no need to en- or disable when in full duplex mode and already initialized
  }

  if(audio_i2s.rx_handle == nullptr){
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    err = i2s_new_channel(&chan_cfg, NULL, &audio_i2s.rx_handle);
    switch (audio_i2s.Settings->rx.mode){
      case 1:
          {
          _CLK = (gpio_num_t)Pin(GPIO_I2S_WS,1); //legacy setting for Core2, might be wrong
          if(_CLK == -1){
            _CLK =  (gpio_num_t)Pin(GPIO_I2S_WS); //fallback to other port, might be wrong
          }
          i2s_pdm_rx_config_t pdm_rx_cfg = {
          .clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(audio_i2s.Settings->rx.sample_rate),
          /* The default mono slot is the left slot (whose 'select pin' of the PDM microphone is pulled down) */
          .slot_cfg = I2S_PDM_RX_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, slot_mode),
          .gpio_cfg = {
              .clk = _CLK,
              .din = (gpio_num_t)Pin(GPIO_I2S_DIN),
              .invert_flags = {
                  .clk_inv = false,
              },
          },
        };
        pdm_rx_cfg.slot_cfg.slot_mask = (i2s_pdm_slot_mask_t)audio_i2s.Settings->rx.slot_mask;
        err = i2s_channel_init_pdm_rx_mode(audio_i2s.rx_handle, &pdm_rx_cfg);}
        AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: RX channel in PDM mode, CLK: %i, DIN: %i, 16 bit width, %i channel(s), err code: %u"),_CLK, Pin(GPIO_I2S_DIN), slot_mode, err);
        break;
      default: // same as 0
          {        
          i2s_std_config_t rx_std_cfg = {
          .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(audio_i2s.Settings->rx.sample_rate),
          .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, slot_mode),
          .gpio_cfg = {
              .mclk = (gpio_num_t)Pin(GPIO_I2S_MCLK),
              .bclk = (gpio_num_t)Pin(GPIO_I2S_BCLK),
              .ws   = (gpio_num_t)Pin(GPIO_I2S_WS),
              .dout = I2S_GPIO_UNUSED,
              .din  = (gpio_num_t)Pin(GPIO_I2S_DIN),
              .invert_flags = {
                  .mclk_inv = false,
                  .bclk_inv = false,
                  .ws_inv   = false,
                  },
              },
          };
          rx_std_cfg.slot_cfg.slot_mask = (i2s_std_slot_mask_t)audio_i2s.Settings->rx.slot_mask;
          i2s_channel_init_std_mode(audio_i2s.rx_handle, &rx_std_cfg);}
          AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: RX channel in standard mode with 16 bit width on %i channel(s) initialized"),slot_mode);
      break;
    }

  }
  if(enable == 0){
    int _err = i2s_channel_disable(audio_i2s.rx_handle);
    i2s_del_channel(audio_i2s.rx_handle);
    audio_i2s.rx_handle = nullptr;
    AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: RX channel disable: %i"),_err);
  }
  else{
    err = i2s_channel_enable(audio_i2s.rx_handle);
    AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: RX channel enable: %i"),err);
  }
  return err;
}

// micro to mp3 file or stream
void I2sMicTask(void *arg){
  int8_t error = 0;
  uint8_t *ucp;
  int written;
  shine_config_t  config;
  shine_t s = nullptr;
  uint16_t samples_per_pass;
  File mp3_out = (File)nullptr;
  int16_t *buffer = nullptr;
  uint16_t bytesize;
  uint16_t bwritten;
  uint32_t ctime;
  uint32_t gain = audio_i2s.Settings->rx.gain;

  if (!audio_i2s.use_stream) {
    mp3_out = ufsp->open(audio_i2s.mic_path, "w");
    if (!mp3_out) {
      error = 1;
      goto exit;
    }
  } else {
    if (!audio_i2s.stream_active) {
      error = 2;
      audio_i2s.use_stream = 0;
      goto exit;
    }
    audio_i2s.client.flush();
    audio_i2s.client.setTimeout(3);
    audio_i2s.client.print("HTTP/1.1 200 OK\r\n"
    "Content-Type: audio/mpeg;\r\n\r\n");

   //  Webserver->send(200, "application/octet-stream", "");
    //"Content-Type: audio/mp3;\r\n\r\n");
  }

  shine_set_config_mpeg_defaults(&config.mpeg);

  if (audio_i2s.Settings->rx.slot_mode == 0) {
    config.mpeg.mode = MONO;
  } else {
    config.mpeg.mode = STEREO;
  }
  config.mpeg.bitr = 128;
  config.wave.samplerate = audio_i2s.Settings->rx.sample_rate;
  config.wave.channels = (channels)(audio_i2s.Settings->rx.slot_mode + 1);

  if (shine_check_config(config.wave.samplerate, config.mpeg.bitr) < 0) {
    error = 3;
    goto exit;
  }

  s = shine_initialise(&config);
  if (!s) {
    error = 4;
    goto exit;
  }

  samples_per_pass = shine_samples_per_pass(s);
  bytesize = samples_per_pass * 2 * (audio_i2s.Settings->rx.slot_mode + 1);

  buffer = (int16_t*)malloc(bytesize);
  if (!buffer) {
    error = 5;
    goto exit;
  }

  ctime = TasmotaGlobal.uptime;


  while (!audio_i2s.mic_stop) {
      size_t bytes_read;
      i2s_channel_read(audio_i2s.rx_handle, (void*)buffer, bytesize, &bytes_read, (100 / portTICK_PERIOD_MS));

      if (gain > 1) {
        // set gain
        for (uint32_t cnt = 0; cnt < bytes_read / 2; cnt++) {
          buffer[cnt] *= gain;
        }
      }
      ucp = shine_encode_buffer_interleaved(s, buffer, &written);

      if (!audio_i2s.use_stream) {
        bwritten = mp3_out.write(ucp, written);
        if (bwritten != written) {
          break;
        }
      } else {
        audio_i2s.client.write((const char*)ucp, written);

        if (!audio_i2s.client.connected()) {
          break;
        }
      }
      audio_i2s.recdur = TasmotaGlobal.uptime - ctime;
  }

  ucp = shine_flush(s, &written);

  if (!audio_i2s.use_stream) {
    mp3_out.write(ucp, written);
  } else {
    audio_i2s.client.write((const char*)ucp, written);
  }


exit:
  if (s) {
    shine_close(s);
  }
  if (mp3_out) {
    mp3_out.close();
    AddLog(LOG_LEVEL_INFO, PSTR("I2S: MP3 file closed"));
  }
  if (buffer) {
    free(buffer);
  }

  if (audio_i2s.use_stream) {
    audio_i2s.client.stop();
  }

  I2sMicInit(0);
  audio_i2s.mic_stop = 0;
  audio_i2s.mic_error = error;
  AddLog(LOG_LEVEL_INFO, PSTR("mp3task result code: %d"), error);
  audio_i2s.mic_task_handle = 0;
  audio_i2s.recdur = 0;
  audio_i2s.stream_active = 0;
  vTaskDelete(NULL);

}

int32_t I2sRecordShine(char *path) {
  esp_err_t err = ESP_OK;

  if (audio_i2s.decoder || audio_i2s.mp3) return 0;

  strlcpy(audio_i2s.mic_path, path, sizeof(audio_i2s.mic_path));
  audio_i2s.mic_stop = 0;
  uint32_t stack = 4096;
  audio_i2s.use_stream = !strcmp(audio_i2s.mic_path, "stream.mp3");

  if (audio_i2s.use_stream) {
    stack = 8000;
  }
  I2sMicInit(1);

  err = xTaskCreatePinnedToCore(I2sMicTask, "MIC", stack, NULL, 3, &audio_i2s.mic_task_handle, 1);

  return err;
}

/*********************************************************************************************\
 * Driver Settings load and save using filesystem
\*********************************************************************************************/
// error codes
enum {
  I2S_OK = 0,
  I2S_ERR_OUTPUT_NOT_CONFIGURE,
  I2S_ERR_DECODER_IN_USE,
  I2S_ERR_FILE_NOT_FOUND,
};

// signal to an external Berry driver that we turn audio power on or off
void I2SAudioPower(bool power) {
  callBerryEventDispatcher(PSTR("audio"), PSTR("power"), power, nullptr, 0);
}

void I2SSettingsLoad(bool erase) {
    
  audio_i2s.Settings = new tI2SSettings();

#ifndef USE_UFILESYS
  AddLog(LOG_LEVEL_INFO, PSTR("CFG: I2S use defaults as file system not enabled"));
#else
  char filename[20];
  // Use for drivers:
  snprintf_P(filename, sizeof(filename), TASM_FILE_DRIVER, XDRV_42);
  if (erase) {
    TfsDeleteFile(filename);  // Use defaults
  }
  else if (TfsLoadFile(filename, (uint8_t*)audio_i2s.Settings, sizeof(tI2SSettings))) {
    AddLog(LOG_LEVEL_INFO, PSTR("CFG: I2S loaded from file"));
  }
  else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: I2S use defaults as file system not ready or file not found"));
    I2SSettingsSave();
  }
#endif  // USE_UFILESYS
}

void I2SSettingsSave(void) {
#ifdef USE_UFILESYS
  char filename[20];
  // Use for drivers:
  snprintf_P(filename, sizeof(filename), TASM_FILE_DRIVER, XDRV_42);
  if (TfsSaveFile(filename, (const uint8_t*)audio_i2s.Settings, sizeof(tI2SSettings))) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: I2S saved to file"));
  } else {
    // File system not ready: No flash space reserved for file system
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: ERROR I2S file system not ready or unable to save file"));
  }
#endif  // USE_UFILESYS
}

/*********************************************************************************************\
 * Driver init
\*********************************************************************************************/

void I2sCheckCfg(void){
  bool useDuplexMode = ((Pin(GPIO_I2S_DIN) != -1) && (Pin(GPIO_I2S_DOUT) != -1)); // din and dout must be configured on port 0 for full duplex
  // AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: DIN %i , DOUT %i"),Pin(GPIO_I2S_DIN),Pin(GPIO_I2S_DOUT) );
  if(useDuplexMode){
    if(audio_i2s.Settings->rx.mode == 1 || audio_i2s.Settings->tx.mode == 1 ){
      useDuplexMode = false;
      AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: PDM forbids full duplex mode"));
    }
    audio_i2s.Settings->sys.duplex = useDuplexMode ? 1 : 0;
    if(useDuplexMode){
      AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: will try to use full duplex mode"));
    }
  }
  if(Pin(GPIO_I2S_DIN) != -1 || Pin(GPIO_I2S_DIN,1) != -1){ // micro could be port 0 or 1
    audio_i2s.Settings->sys.rx = 1;
    AddLog(LOG_LEVEL_DEBUG, PSTR("CFG: I2S RX config = mode: %i, channels: %i, gain: %i, sample rate: %i"), audio_i2s.Settings->rx.mode, (uint8_t)(audio_i2s.Settings->rx.slot_mode + 1), audio_i2s.Settings->rx.gain, audio_i2s.Settings->rx.sample_rate);
  }
  else{
    audio_i2s.Settings->sys.rx = 0;
    audio_i2s.Settings->rx.mp3_encoder = 0; // do not allocate buffer
  }
  if(Pin(GPIO_I2S_DOUT) != -1){ // output is only supported on port 0
    audio_i2s.Settings->sys.tx = 1;
  }
  else{
    audio_i2s.Settings->sys.tx = 0;
    audio_i2s.Settings->tx.webradio = 0;  // do not allocate buffer
  }

  AddLog(LOG_LEVEL_INFO, PSTR("I2S: init pins bclk=%d, ws=%d, dout=%d, mclk=%d, din=%d"), Pin(GPIO_I2S_BCLK) , Pin(GPIO_I2S_WS), Pin(GPIO_I2S_DOUT), Pin(GPIO_I2S_MCLK), Pin(GPIO_I2S_DIN));

}

bool I2sPinInit(void) {
  int result = 0;

  if(Pin(GPIO_I2S_DIN) + Pin(GPIO_I2S_DIN,1) + Pin(GPIO_I2S_DOUT) + Pin(GPIO_I2S_DOUT,1) == -4){
    return false;
  }

  I2SSettingsLoad(false);
  I2sCheckCfg();

  if(audio_i2s.Settings->sys.rx == 1 && audio_i2s.Settings->sys.duplex == 0){
    result += I2sMicInit(0);
  }

  if(audio_i2s.Settings->sys.tx == 1){
    audio_i2s.out = new TasmotaAudioOutputI2S;
    int err = audio_i2s.out->SetPinout() ? 0 : 1;
    result += err;
  }

  return (result == 0);
}

void I2sInit(void) {

  if (I2sPinInit() == false) {
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: no valid pin config"));
    return;
  }

  if(audio_i2s.out != nullptr){
    audio_i2s.out->SetGain(((float)audio_i2s.Settings->tx.volume / 100.0) * 4.0);
    audio_i2s.out->begin();
    audio_i2s.out->stop();
  }
  audio_i2s.mp3ram = nullptr;

  if(audio_i2s.Settings->rx.mp3_encoder == 1){
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: will allocate buffer for mp3 encoder"));
    if (UsePSRAM()) {
      audio_i2s.mp3ram = heap_caps_malloc(preallocateCodecSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    }
    else{
      audio_i2s.Settings->rx.mp3_encoder = 0; // no PS-RAM -> no MP3 encoding
    }
  }

  if(audio_i2s.Settings->tx.webradio == 1){
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: will allocate buffer for webradio "));
    if (UsePSRAM()) {
      audio_i2s.preallocateBuffer = heap_caps_malloc(preallocateBufferSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      audio_i2s.preallocateCodec = heap_caps_malloc(preallocateCodecSize, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    } else {
      audio_i2s.preallocateBuffer = malloc(preallocateBufferSize);
      audio_i2s.preallocateCodec = malloc(preallocateCodecSize);
    }
  }
}

/*********************************************************************************************\
 * Driver features and commands
\*********************************************************************************************/

void I2sMp3Task(void *arg) {
  while (1) {
    while (audio_i2s.mp3->isRunning()) {
      if (!audio_i2s.mp3->loop()) {
        audio_i2s.mp3->stop();
        mp3_delete();
        audio_i2s.out->stop();
        if (audio_i2s.mp3_task_handle) {
          vTaskDelete(audio_i2s.mp3_task_handle);
          audio_i2s.mp3_task_handle = 0;
        }
        //mp3_task_handle=nullptr;
      }
     vTaskDelay(pdMS_TO_TICKS(1));
    }
  }
}

void I2sMDCallback(void *cbData, const char *type, bool isUnicode, const char *str) {
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

void I2sStatusCallback(void *cbData, int code, const char *string) {
  const char *ptr = reinterpret_cast<const char *>(cbData);
  (void) code;
  (void) ptr;
  //strncpy_P(status, string, sizeof(status)-1);
  //status[sizeof(status)-1] = 0;
}

void Webradio(const char *url) {
  if (audio_i2s.decoder || audio_i2s.mp3) return;
  if (!audio_i2s.out) return;
  if (audio_i2s.Settings->tx.webradio == 0) return;
  I2SAudioPower(true);
  audio_i2s.ifile = new AudioFileSourceICYStream(url);
  audio_i2s.ifile->RegisterMetadataCB(I2sMDCallback, NULL);
  audio_i2s.buff = new AudioFileSourceBuffer(audio_i2s.ifile, audio_i2s.preallocateBuffer, preallocateBufferSize);
  audio_i2s.buff->RegisterStatusCB(I2sStatusCallback, NULL);
  audio_i2s.decoder = new AudioGeneratorMP3(audio_i2s.preallocateCodec, preallocateCodecSize);
  audio_i2s.decoder->RegisterStatusCB(I2sStatusCallback, NULL);
  audio_i2s.decoder->begin(audio_i2s.buff, audio_i2s.out);
  if (!audio_i2s.decoder->isRunning()) {
  //  Serial.printf_P(PSTR("Can't connect to URL"));
    I2sStopPlaying();
  //  strcpy_P(status, PSTR("Unable to connect to URL"));
    audio_i2s.retryms = millis() + 2000;
  }

  AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: will launch webradio task"));
  xTaskCreatePinnedToCore(I2sMp3Task2, "MP3-2", 8192, NULL, 3, &audio_i2s.mp3_task_handle, 1);
}

void I2sMp3Task2(void *arg){
  while (1) {
    if (audio_i2s.decoder && audio_i2s.decoder->isRunning()) {
      if (!audio_i2s.decoder->loop()) {
        I2sStopPlaying();
        //retryms = millis() + 2000;
      }
      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }
}

void I2sStopPlaying() {

  if (audio_i2s.mp3_task_handle) {
    vTaskDelete(audio_i2s.mp3_task_handle);
    audio_i2s.mp3_task_handle = nullptr;
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
  I2SAudioPower(false);
}

#ifdef USE_WEBSERVER
const char HTTP_WEBRADIO[] PROGMEM =
   "{s}" "I2S_WR-Title" "{m}%s{e}";

void I2sWrShow(bool json) {
    if (audio_i2s.decoder) {
      if (json) {
        ResponseAppend_P(PSTR(",\"WebRadio\":{\"Title\":\"%s\"}"), audio_i2s.wr_title);
      } else {
        WSContentSend_PD(HTTP_WEBRADIO,audio_i2s.wr_title);
      }
    }
}
#endif  // USE_WEBSERVER


// Play_mp3 - Play a MP3 file from filesystem
//
// Returns I2S_error_t
int32_t I2SPlayMp3(const char *path) {
  if (!audio_i2s.out) return I2S_ERR_OUTPUT_NOT_CONFIGURE;
  if (audio_i2s.decoder || audio_i2s.mp3) return I2S_ERR_DECODER_IN_USE;

  // check if the filename starts with '/', if not add it
  char fname[64];
  if (path[0] != '/') {
    snprintf(fname, sizeof(fname), "/%s", path);
  } else {
    snprintf(fname, sizeof(fname), "%s", path);
  }
  if (!ufsp->exists(fname)) { return I2S_ERR_FILE_NOT_FOUND; }

  I2SAudioPower(true);

  audio_i2s.file = new AudioFileSourceFS(*ufsp, fname);

  audio_i2s.id3 = new AudioFileSourceID3(audio_i2s.file);

  if (audio_i2s.mp3ram) {
    audio_i2s.mp3 = new AudioGeneratorMP3(audio_i2s.mp3ram, preallocateCodecSize);
  } else {
    audio_i2s.mp3 = new AudioGeneratorMP3();
  }
  audio_i2s.mp3->begin(audio_i2s.id3, audio_i2s.out);

  // Always use a task
  xTaskCreatePinnedToCore(I2sMp3Task, "MP3", 8192, NULL, 3, &audio_i2s.mp3_task_handle, 1);
  return I2S_OK;
}

void mp3_delete(void) {
  delete audio_i2s.file;
  delete audio_i2s.id3;
  delete audio_i2s.mp3;
  audio_i2s.mp3=nullptr;
  I2SAudioPower(false);
}

void Say(char *text) {

  if (!audio_i2s.out) return;

  I2SAudioPower(true);

  ESP8266SAM *sam = new ESP8266SAM;

  sam->Say(audio_i2s.out, text);
  delete sam;
  audio_i2s.out->stop();

  I2SAudioPower(false);
}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

const char kI2SAudio_Commands[] PROGMEM = "I2S|"
  "Gain|Play|WR|Rec|MGain|Stop"
#ifdef USE_I2S_SAY
  "|Say"
#ifdef USE_I2S_SAY_TIME
  "|Time"
#endif // USE_I2S_SAY_TIME
#endif // USE_I2S_SAY

#ifdef USE_I2S_RTTTL
  "|Rtttl"
#endif
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
  "|Stream"
#endif // MP3_MIC_STREAM
#ifdef I2S_BRIDGE
  "|Bridge"
#endif // I2S_BRIDGE
;

void (* const I2SAudio_Command[])(void) PROGMEM = {
  &CmndI2SGain, &CmndI2SPlay, &CmndI2SWebRadio, &CmndI2SMicRec, &CmndI2SMicGain, &CmndI2SStop,
#ifdef USE_I2S_SAY
  &CmndI2SSay,
#ifdef USE_I2S_SAY_TIME
  &Cmd_Time,
#endif // USE_I2S_SAY_TIME
#endif // USE_I2S_SAY

#ifdef USE_I2S_RTTTL
  &CmndI2SI2SRtttl,
#endif
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
  &CmndI2SMP3Stream,
#endif // MP3_MIC_STREAM
#ifdef I2S_BRIDGE
  &CmndI2SI2SBridge,
#endif // I2S_BRIDGE
};

void CmndI2SStop(void) {
  I2sStopPlaying();
  ResponseCmndDone();
}

void CmndI2SWebRadio(void) {
  if (!audio_i2s.out) return;

  if (audio_i2s.decoder) {
    I2sStopPlaying();
  }
  if (XdrvMailbox.data_len > 0) {
    Webradio(XdrvMailbox.data);
    ResponseCmndChar(XdrvMailbox.data);
  } else {
    ResponseCmndChar_P(PSTR("Stopped"));
  }
}

void CmndI2SPlay(void) {
  if (XdrvMailbox.data_len > 0) {
    int32_t err = I2SPlayMp3(XdrvMailbox.data);
    // display return message
    switch (err) {
      case I2S_OK:
        ResponseCmndDone();
        break;
      case I2S_ERR_OUTPUT_NOT_CONFIGURE:
        ResponseCmndChar("I2S output not configured");
        break;
      case I2S_ERR_DECODER_IN_USE:
        ResponseCmndChar("Decoder already in use");
        break;
      case I2S_ERR_FILE_NOT_FOUND:
        ResponseCmndChar("File not found");
        break;
      default:
        ResponseCmndChar("Unknown error");
        break;
    }
  } else {
    ResponseCmndChar("Missing filename");
  }
}

void CmndI2SGain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
    if (audio_i2s.out) {
      audio_i2s.Settings->tx.volume = XdrvMailbox.payload;
      audio_i2s.out->SetGain(((float)(audio_i2s.Settings->tx.volume-2)/100.0)*4.0);
    }
  }
  ResponseCmndNumber(audio_i2s.Settings->tx.volume);
}

void CmndI2SSay(void) {
  if (XdrvMailbox.data_len > 0) {
    Say(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndI2SI2SRtttl(void) {
  if (XdrvMailbox.data_len > 0) {
    Rtttl(XdrvMailbox.data);
  }
  ResponseCmndChar(XdrvMailbox.data);
}

void CmndI2SMicRec(void) {
if (audio_i2s.Settings->rx.mp3_encoder == 1) {
  if (XdrvMailbox.data_len > 0) {
    if (!strncmp(XdrvMailbox.data, "-?", 2)) {
      Response_P("{\"I2SREC-duration\":%d}", audio_i2s.recdur);
    } else {
      I2sRecordShine(XdrvMailbox.data);
      ResponseCmndChar(XdrvMailbox.data);
    }
  } else {
    if (audio_i2s.mic_task_handle) {
      // stop task
      audio_i2s.mic_stop = 1;
      while (audio_i2s.mic_stop) {
        delay(1);
      }
      ResponseCmndChar_P(PSTR("Stopped"));
    }
  }
}
else{
  if(audio_i2s.Settings->sys.rx == 1){
    ResponseCmndChar_P(PSTR("need PSRAM for MP3 recording"));
  }
  else{
    ResponseCmndChar_P(PSTR("no mic configured"));
  }
}
}

// mic gain in factor not percent
void CmndI2SMicGain(void) {
  if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 256)) {
      audio_i2s.Settings->rx.gain = XdrvMailbox.payload;
  }
  ResponseCmndNumber(audio_i2s.Settings->rx.gain);
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

void I2sMp3Loop(void);
void I2sMp3Init(void);
void MP3ShowStream(void);

bool Xdrv42(uint32_t function) {
  bool result = false;

  switch (function) {
    case FUNC_COMMAND:
      result = DecodeCommand(kI2SAudio_Commands, I2SAudio_Command);
      break;
    case FUNC_INIT:
      I2sInit();
      break;
    case FUNC_WEB_ADD_MAIN_BUTTON:
      //MP3ShowStream();
      break;
    case FUNC_LOOP:
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
      I2sMp3Loop();
#endif
#if defined(I2S_BRIDGE)
      i2s_bridge_loop();
#endif
      break;
    case FUNC_WEB_ADD_HANDLER:
#if defined(USE_SHINE) && defined(MP3_MIC_STREAM)
      audio_i2s.stream_enable = 1;
      I2sMp3Init(1);
#endif
#if defined(I2S_BRIDGE)
      I2SBridgeInit();
#endif
      break;

#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      I2sWrShow(false);
      break;
#endif  // USE_WEBSERVER
    case FUNC_JSON_APPEND:
      I2sWrShow(true);
    break;
  }
  return result;
}

#endif  // USE_I2S_AUDIO
#endif //ESP_IDF_VERSION_MAJOR >= 5
