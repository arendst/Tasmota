
#ifdef ESP32
#ifdef ESP32S3_BOX
#include <driver/i2s.h>
#include <es8156.h>
#include <es8311.h>
#include <es7243e.h>
#include <es7210.h>

void S3boxAudioPower(uint8_t pwr) {
  pinMode(46 , OUTPUT);
  digitalWrite(46, pwr);
}

// box lite dac init
uint32_t ES8156_init() {
  uint32_t ret_val = ESP_OK;

  if (I2cSetDevice(ES8156_ADDR, 1)) {
    I2cSetActiveFound(ES8156_ADDR, "ES8156-I2C", 1);
    audio_hal_codec_config_t cfg = {
       .i2s_iface = {
         .mode = AUDIO_HAL_MODE_SLAVE,
         .bits = AUDIO_HAL_BIT_LENGTH_16BITS,
       }
    };
    ret_val |= es8156_codec_init(&Wire1, &cfg);
    ret_val |= es8156_codec_set_voice_volume(75);
  }
  return ret_val;
}

// box lite adc init
uint32_t es7243e_init() {
    uint32_t ret_val = ESP_OK;

    if (I2cSetDevice(ES7243_ADDR, 1)) {
      I2cSetActiveFound(ES7243_ADDR, "ES7243e-I2C", 1);

      audio_hal_codec_config_t cfg = {
        .i2s_iface = {
          .mode = AUDIO_HAL_MODE_SLAVE,
          .bits = AUDIO_HAL_BIT_LENGTH_16BITS,
        }
      };

      ret_val |= es7243e_adc_init(&Wire1, &cfg);
    }

    return ret_val;
}

// box adc init
uint32_t es7210_init() {
  uint32_t ret_val = ESP_OK;

  if (I2cSetDevice(ES7210_ADDR, 1)) {
    I2cSetActiveFound(ES7210_ADDR, "ES7210-I2C", 1);
    audio_hal_codec_config_t cfg = {
        .adc_input = AUDIO_HAL_ADC_INPUT_ALL,
        .codec_mode = AUDIO_HAL_CODEC_MODE_ENCODE,
        .i2s_iface = {
          .mode = AUDIO_HAL_MODE_SLAVE,
          .fmt = AUDIO_HAL_I2S_NORMAL,
          .samples = AUDIO_HAL_16K_SAMPLES,
          .bits = AUDIO_HAL_BIT_LENGTH_16BITS,
        },
    };

    ret_val |= es7210_adc_init(&Wire1, &cfg);
    ret_val |= es7210_adc_config_i2s(cfg.codec_mode, &cfg.i2s_iface);
    ret_val |= es7210_adc_set_gain((es7210_input_mics_t)(ES7210_INPUT_MIC1 | ES7210_INPUT_MIC2), (es7210_gain_value_t) GAIN_37_5DB);
    ret_val |= es7210_adc_set_gain((es7210_input_mics_t)(ES7210_INPUT_MIC3 | ES7210_INPUT_MIC4), (es7210_gain_value_t) GAIN_0DB);
    ret_val |= es7210_adc_ctrl_state(cfg.codec_mode, AUDIO_HAL_CTRL_START);
  }
  return ret_val;
}

// box dac init
uint32_t ES8311_init() {
  uint32_t ret_val = ESP_OK;

  if (I2cSetDevice(ES8311_ADDR, 1)) {
    I2cSetActiveFound(ES8311_ADDR, "ES8311-I2C", 1);
    audio_hal_codec_config_t cfg = {
        .dac_output = AUDIO_HAL_DAC_OUTPUT_LINE1,
        .codec_mode = AUDIO_HAL_CODEC_MODE_DECODE,
        .i2s_iface = {
          .mode = AUDIO_HAL_MODE_SLAVE,
          .fmt = AUDIO_HAL_I2S_NORMAL,
          .samples = AUDIO_HAL_16K_SAMPLES,
          .bits = AUDIO_HAL_BIT_LENGTH_16BITS,
        },
    };

    ret_val |= es8311_codec_init(&Wire1, &cfg);
    ret_val |= es8311_set_bits_per_sample(cfg.i2s_iface.bits);
    ret_val |= es8311_config_fmt((es_i2s_fmt_t)cfg.i2s_iface.fmt);
    ret_val |= es8311_codec_set_voice_volume(75);
    ret_val |= es8311_codec_ctrl_state(cfg.codec_mode, AUDIO_HAL_CTRL_START);

  }
  return ret_val;
}

void S3boxInit() {
  if (TasmotaGlobal.i2c_enabled_2) {
    // box lite
    ES8156_init();
    es7243e_init();
    // box full
    ES8311_init();
    es7210_init();
  }
}
#endif // ESP32S3_BOX

#ifdef USE_SHINE
#ifdef WAV2MP3

#include <layer3.h>
#include <types.h>

typedef uint8_t mp3buf_t;


// min freq = 16 KHz Stereo or 32 KHz Mono
int32_t wav2mp3(char *path) {
  int32_t error = 0;
  shine_config_t  config;
  shine_t s = nullptr;
  File wav_in = (File)nullptr;
  File mp3_out = (File)nullptr;
  uint8_t *ucp;
  int written;
  int16_t *buffer = nullptr;
  uint32_t bread;
  uint16_t samples_per_pass;
  char mpath[64];
  char *cp;
  uint8_t chans = 1;
  uint32_t sfreq = 16000;

  strlcpy(mpath, path, sizeof(mpath));

  wav_in = ufsp->open(mpath, FS_FILE_READ);
  if (!wav_in) {
    error = -1;
    goto exit;
  }

  // script>wav2mp3("/test2.wav")
  uint8_t wavHeader[sizeof(wavHTemplate)];
  wav_in.read((uint8_t*)wavHeader, sizeof(wavHTemplate));
  chans = wavHeader[22];
  sfreq = wavHeader[24]|(wavHeader[25]<<8)|(wavHeader[26]<<16)|(wavHeader[27]<<24);

  cp = strchr(mpath, '.');
  if (!cp) {
    error = -6;
    goto exit;
  }

  strcpy(cp, ".mp3");

  mp3_out = ufsp->open(mpath, FS_FILE_WRITE);
  if (!mp3_out) {
    error = -2;
    goto exit;
  }

  shine_set_config_mpeg_defaults(&config.mpeg);

  if (chans == 1) {
    config.mpeg.mode = MONO;
  } else {
    config.mpeg.mode = STEREO;
  }
  config.mpeg.bitr = 128;
  config.wave.samplerate = sfreq;
  config.wave.channels = (channels)chans;


  if (shine_check_config(config.wave.samplerate, config.mpeg.bitr) < 0) {
    error = -3;
    goto exit;
  }

  s = shine_initialise(&config);
  if (!s) {
    error = -4;
    goto exit;
  }

  samples_per_pass = shine_samples_per_pass(s);


  buffer = (int16_t*)malloc(samples_per_pass * 2 * chans);
  if (!buffer) {
    error = -5;
    goto exit;
  }

  AddLog(LOG_LEVEL_INFO, PSTR("mp3 encoding %d channels with freq %d Hz"), chans, sfreq);

  while (1) {
    bread = wav_in.read((uint8_t*)buffer, samples_per_pass * 2 * chans);
    if (!bread) {
      break;
    }
    ucp = shine_encode_buffer_interleaved(s, buffer, &written);
    mp3_out.write(ucp, written);
  }
  ucp = shine_flush(s, &written);
  mp3_out.write(ucp, written);

exit:
  if (s) {
    shine_close(s);
  }
  if (wav_in) {
    wav_in.close();
  }
  if (mp3_out) {
    mp3_out.close();
  }

  if (buffer) {
    free(buffer);
  }

  AddLog(LOG_LEVEL_INFO, PSTR("mp3 encoding exit with code: %d"), error);

  return error;
}
#endif  // WAV2MP3
#endif // USE_SHINE


#endif
