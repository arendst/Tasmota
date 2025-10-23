/*
  xdrv_42_i2s_mp3mic.ino - Audio dac support for Tasmota

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


#if defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
#ifdef USE_I2S_AUDIO

class AudioEncoder
{
  public:
    AudioEncoder() {
      inBuffer = nullptr;
      outFrame = nullptr;
      samplesPerPass = 0;
      byteSize = 0;
    };
    virtual ~AudioEncoder() {};
    virtual uint32_t begin(uint32_t samplingRate, uint32_t inputChannels) { return 0; };
    virtual int encode(size_t samples) { return 0; };
    virtual size_t stop() { return 0; };
    virtual void setCB(void *cb){};

  public:
    int16_t *inBuffer;
    uint8_t *outFrame;
    uint32_t samplesPerPass;
    uint32_t byteSize;
    File *file;
    WiFiClient *client;
};

#ifdef MP3_MIC_STREAM
class AudioEncoderShineMP3 :  public AudioEncoder
{
  public:
    AudioEncoderShineMP3(File *rec_file, WiFiClient *wifi) {
      file = rec_file;
      client = wifi;
    };
    virtual ~AudioEncoderShineMP3() {
      if (s) {shine_close(s);}
      if(inBuffer){free(inBuffer); }
      if(file) {file->close();}
      if(client) {client->stop();}
    };
    virtual uint32_t begin(uint32_t samplingRate, uint32_t inputChannels) {
      shine_set_config_mpeg_defaults(&config.mpeg);
      if (inputChannels == 1) {
        config.mpeg.mode = MONO;
      } else {
        config.mpeg.mode = STEREO;
      }
      config.wave.samplerate = samplingRate;
      config.wave.channels = (channels)inputChannels;
      if (shine_check_config(config.wave.samplerate, config.mpeg.bitr) < 0) {return 3;}
      s = shine_initialise(&config);
      if (!s) {return 4; }

      samplesPerPass = shine_samples_per_pass(s);
      byteSize = samplesPerPass * 2 * inputChannels;

      inBuffer = (int16_t*)malloc(byteSize); // byteSize = samplesPerPass * sizeof(int16_t)
      if (!inBuffer) {return 5; }
      return 0;
    };

    virtual size_t stop() {
      int written;
      outFrame = shine_flush(s, &written);
      return written;
    }

    virtual int encode(size_t samples) {
      int written;
      outFrame = shine_encode_buffer_interleaved(s, inBuffer, &written);
      return write(written);
    };

  protected:
    int write(int len){
      size_t written = 0;
      if(file != nullptr){
        written = file->write(outFrame,len);
      } else if (client != nullptr){
        if (client->connected()) {
          written = client->write(outFrame, len);
        }
      }
      return (written != (size_t)len);
    }
    shine_config_t  config;
    shine_t s;
};
#endif //MP3_MIC_STREAM

#ifdef USE_I2S_OPUS

#include "libwebm/mkvmuxer/mkvwriter.h"
class AudioEncoderOpusWebm;
typedef  int (AudioEncoderOpusWebm::*packet_cb)(void *userdata, const void *buffer, size_t length);
class AudioEncoderOpusWebm : public AudioEncoder
{
  public:
    AudioEncoderOpusWebm(File *rec_file, WiFiClient *wifi) {
      file = rec_file;
      client = wifi;
    };

    virtual ~AudioEncoderOpusWebm() {
      if(inBuffer){ free(inBuffer); }
      if(outFrame){ free(outFrame); }
      opus_encoder_destroy(encoder);
      if(file) {file->close();}
      if(client) {client->stop();}
      if(wifiBuffer){ free(wifiBuffer); }
      delete muxer;
    };

    virtual uint32_t begin(uint32_t samplingRate, uint32_t inputChannels) {
      int error;
      encoder = opus_encoder_create(samplingRate, inputChannels, samplingRate > 16000 ? OPUS_APPLICATION_AUDIO : OPUS_APPLICATION_VOIP, &error);
      if (error != 0) { return error; }
      opus_encoder_ctl(encoder, OPUS_SET_COMPLEXITY(complexity));
      int _preskip;
      opus_encoder_ctl(encoder, OPUS_GET_LOOKAHEAD((opus_int32*)&_preskip));
      opusHeader.samplingRate = samplingRate;
      opusHeader.inputChannels = inputChannels;
      opusHeader.preSkip = _preskip;

      constexpr uint32_t frameDuration = 20; //ms
      samplesPerPass = samplingRate/(1000/frameDuration);
      byteSize = samplesPerPass * 2 * inputChannels;
      inBuffer = (int16_t*)malloc(byteSize);
      if (!inBuffer) {return 5; }
      outFrame = (uint8_t*)malloc(maxBytes);
      if (!outFrame) {return 5; }
      if(client){
        wifiBuffer = (uint8_t*)malloc(wifiBufferSize);
        if (!wifiBuffer) {return 5; }
      }

      if(file){
        muxer = new mkvmuxer::MkvWriter(file);
      } else if (client){
        muxer = new mkvmuxer::MkvWriter(this, &AudioEncoderOpusWebm::packet_cb);
      }
      timeCode = 0;
      if (!muxer) {return 6; }
      if (!muxerSegment.Init(muxer)) {return 7; }
      mkvmuxer::SegmentInfo* info = muxerSegment.GetSegmentInfo();
      if (info == nullptr) {return 8;}
	    info->set_writing_app("Tasmota");
	    // Add an audio track.
      trackNumber = muxerSegment.AddAudioTrack(samplingRate, inputChannels, 0);
      if (trackNumber == 0) {return 9; }
      mkvmuxer::AudioTrack* audio = static_cast<mkvmuxer::AudioTrack*>(muxerSegment.GetTrackByNumber(trackNumber));
      if (audio == nullptr) {return 10;}
      audio->set_codec_id(mkvmuxer::Tracks::kOpusCodecId);
      audio->set_bit_depth(16);
      audio->set_codec_delay(_preskip * 1000000 / samplingRate); // Delay built into the code during decoding in nanoseconds.
      audio->set_seek_pre_roll(80000000); // Amount of audio to discard after a seek - https://wiki.xiph.org/MatroskaOpus
      if (!audio->SetCodecPrivate( (const uint8_t*)&opusHeader, sizeof(opusHeader))) {return 11;}
      return 0;
    }

    virtual int encode(size_t samples) {
      opus_int32 len = opus_encode(encoder, inBuffer, samplesPerPass, outFrame, maxBytes);
      if (len < 0) { return -1; }
      if (len > 2) // ignore packets shorter than or equal to 2 bytes.
      {
        if(!muxerSegment.AddFrame(outFrame, len, trackNumber, timeCode, true)){
          return -1;
        }
      }
      timeCode += 20000 * 1000; // 20 ms in nanoseconds
      return 0;
    }

    virtual size_t stop() {
      bool success = muxerSegment.Finalize();
      muxer->Close();
      return success;
    }

  protected:
    OpusEncoder *encoder;
    const uint32_t maxBytes = 1296;
    const uint32_t complexity = 1; // 1-10 - low to high quality
    mkvmuxer::MkvWriter *muxer;
    mkvmuxer::Segment muxerSegment;

    uint64_t trackNumber = 0;
	  uint64_t timeCode = 0;

    struct __attribute__((packed)) {
    char signatureHead[8] = {'O', 'p', 'u', 's', 'H', 'e', 'a', 'd'};
    uint8_t version = 1;
    uint8_t inputChannels = 1;
    uint16_t preSkip = 3840;
    uint32_t samplingRate = 0;
    int16_t outputGain = 0;
    uint8_t channelMappingFamily = 0;
    } opusHeader;

    static constexpr size_t wifiBufferSize = CONFIG_LWIP_TCP_MSS; // should be <= TCP Maximum Segment Size
    uint8_t *wifiBuffer;
    size_t wifiPtr = 0;

    static int packet_cb(void *userdata, const void *buffer, size_t length){
      AudioEncoderOpusWebm *_enc = (AudioEncoderOpusWebm *)userdata;
      int result = length;
      if(_enc->wifiPtr + length > wifiBufferSize){
        size_t written;
        if (_enc->client->connected()) {
          written = _enc->client->write(_enc->wifiBuffer, _enc->wifiPtr);
        }
        result = written == _enc->wifiPtr ? length : -1; //TODO - maybe tolerate some send errors 
        // AddLog(LOG_LEVEL_DEBUG, PSTR(">> %u,  %u, %u"),written, _enc->wifiPtr, result);
        _enc->wifiPtr = 0;
      }
      memcpy(_enc->wifiBuffer + _enc->wifiPtr,(uint8_t*)buffer,length);
      _enc->wifiPtr += length;
      return result;
    }
};
#endif // USE_I2S_OPUS

// micro to mp3/webm - file or stream
void I2sMicTask(void *arg){
  int8_t error = 0;
  int written;

  AudioEncoder *mic_enc;
  File rec_file;
  File *rec_file_ptr = nullptr;

  uint16_t bwritten;
  uint32_t ctime;
  uint32_t gain = audio_i2s.Settings->rx.gain;
  uint32_t timeForOneRead;
  uint32_t __enctime;
  TickType_t xLastWakeTime;

  if (!audio_i2s_mp3.use_stream) {
    rec_file = ufsp->open(audio_i2s_mp3.mic_path, "w");
    if (!rec_file) {
      error = 1;
      goto exit;
    } else {
      rec_file_ptr = &rec_file;
    }
  } else {
    if (!audio_i2s_mp3.stream_active) {
      error = 2;
      goto exit;
    }
  }

  if(audio_i2s_mp3.encoder_type == MP3_ENCODER){
#ifdef MP3_MIC_STREAM
      mic_enc = new AudioEncoderShineMP3(rec_file_ptr, &audio_i2s_mp3.client);
#endif // MP3_MIC_STREAM
  } else {
#ifdef USE_I2S_OPUS
    mic_enc = new AudioEncoderOpusWebm(rec_file_ptr, &audio_i2s_mp3.client);
#endif // USE_I2S_OPUS
  }

  if (audio_i2s_mp3.use_stream) {
    audio_i2s_mp3.client.flush();
    audio_i2s_mp3.client.setTimeout(3);
    if(audio_i2s_mp3.encoder_type == MP3_ENCODER){
      audio_i2s_mp3.client.print("HTTP/1.1 200 OK\r\n"
      "Content-Type: audio/mpeg;\r\n\r\n");
    } else if (audio_i2s_mp3.encoder_type == OPUS_ENCODER){
      audio_i2s_mp3.client.print("HTTP/1.1 200 OK\r\n"
      "Content-Type: audio/webm; codecs=opus\r\n\r\n");
    }
  }

  error = mic_enc->begin(audio_i2s.Settings->rx.sample_rate, audio_i2s.Settings->rx.channels);
  if(error != 0){
    goto exit;
  }

  ctime = TasmotaGlobal.uptime;
  timeForOneRead = 1000 / ((audio_i2s.Settings->rx.sample_rate / (mic_enc->samplesPerPass * audio_i2s.Settings->rx.channels )));
  // timeForOneRead -= 1; // be very in time
  AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: samples %u, bytesize %u, time: %u"),mic_enc->samplesPerPass, mic_enc->byteSize, timeForOneRead);
  xLastWakeTime = xTaskGetTickCount();

  while (!audio_i2s_mp3.mic_stop) {
    size_t bytes_read;
    // bytes_read = audio_i2s.in->readMic((uint8_t*)mic_enc->inBuffer, mic_enc->byteSize, true /*dc_block*/, false /*apply_gain*/, true /*lowpass*/, nullptr /*peak_ptr*/);
    i2s_channel_read(audio_i2s.in->getRxHandle(), (void*)mic_enc->inBuffer, mic_enc->byteSize, &bytes_read, pdMS_TO_TICKS(1));

    if (gain > 1) {
      // set gain the "old way"
      int16_t _gain = gain / 16;
      for (uint32_t cnt = 0; cnt < bytes_read / 2; cnt++) {
        mic_enc->inBuffer[cnt] *= _gain;
      }
    }

    __enctime = millis();
    if(bytes_read != 0){
      written = mic_enc->encode(bytes_read >> 1); //transmit samples, written is an error code
    }
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("_mic: %u , %i, %i"), millis() - __enctime, written, bytes_read);
    if(written < 0){
      break;
    }

    if (audio_i2s_mp3.use_stream) {
      if (!audio_i2s_mp3.client.connected()) {
        break;
      }
    }

    audio_i2s_mp3.recdur = TasmotaGlobal.uptime - ctime;
    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(timeForOneRead));
  }

  written = mic_enc->stop();

  if (!audio_i2s_mp3.use_stream) {
    rec_file.write(mic_enc->outFrame, written);
  } else {
    audio_i2s_mp3.client.write((const char*)mic_enc->outFrame, written);
  }

exit:
  delete mic_enc;
  audio_i2s_mp3.use_stream = false;
  audio_i2s.in->stopRx();
  audio_i2s_mp3.mic_stop = 0;
  audio_i2s_mp3.mic_error = error;
  AddLog(LOG_LEVEL_INFO, PSTR("I2S: record task result code: %d, min bytes stack free: %u"), error, (uint32_t)uxTaskGetStackHighWaterMark(NULL)*4);
  audio_i2s_mp3.mic_task_handle = 0;
  audio_i2s_mp3.recdur = 0;
  audio_i2s_mp3.stream_active = false;
  vTaskDelete(NULL);
}

int32_t I2sRecord(char *path, uint32_t encoder_type) {
  esp_err_t err = ESP_OK;
  uint32_t stack = 8000;

  #ifdef USE_I2S_MP3
    if (audio_i2s_mp3.decoder) return 0;
  #endif

  switch(encoder_type){
#ifdef MP3_MIC_STREAM
    case MP3_ENCODER:
      switch(audio_i2s.Settings->rx.sample_rate){
        case 32000: case 48000: case 44100:
          break; // supported
        default:
        AddLog(LOG_LEVEL_ERROR, PSTR("I2S: unsupported sample rate for MP3 encoding: %d Hz"), audio_i2s.Settings->rx.sample_rate);
        return -1;
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: start MP3 encoding: %d Hz"), audio_i2s.Settings->rx.sample_rate);
      break;
#endif // MP3_MIC_STREAM
#ifdef USE_I2S_OPUS
    case OPUS_ENCODER:
      switch(audio_i2s.Settings->rx.sample_rate){
        case 48000: case 24000: case 16000: case 12000: case 8000:
          stack = audio_i2s.Settings->rx.sample_rate/2 + 30000; //not the exact value, but okay'ish
          break;
        default:
         AddLog(LOG_LEVEL_ERROR, PSTR("I2S: unsupported sample rate for OPUS encoding: %d Hz"), audio_i2s.Settings->rx.sample_rate);
         return -1;
      }
      AddLog(LOG_LEVEL_DEBUG, PSTR("I2S: start OPUS encoding: %d Hz"), audio_i2s.Settings->rx.sample_rate);
      break;
#endif // USE_I2S_OPUS
    default:
      AddLog(LOG_LEVEL_ERROR, PSTR("I2S: unsupported encoder"));
  }
  audio_i2s_mp3.encoder_type = encoder_type;

  if (audio_i2s_mp3.preallocateCodec != nullptr){
    AddLog(LOG_LEVEL_DEBUG,PSTR("I2S: free codec buffer"));
    free(audio_i2s_mp3.preallocateCodec);
    audio_i2s_mp3.preallocateCodec = nullptr;
  }

  if(!audio_i2s_mp3.use_stream){
    strlcpy(audio_i2s_mp3.mic_path, path, sizeof(audio_i2s_mp3.mic_path));
  }
  audio_i2s_mp3.mic_stop = 0;

  audio_i2s.in->startRx();
  err = xTaskCreatePinnedToCore(I2sMicTask, "MIC", stack, NULL, 3, &audio_i2s_mp3.mic_task_handle, 1);
  return err;
}

#endif // USE_I2S_AUDIO
#endif // defined(ESP32) && ESP_IDF_VERSION_MAJOR >= 5
