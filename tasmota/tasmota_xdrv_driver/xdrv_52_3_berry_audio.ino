/*
  xdrv_52_3_berry_audio.ino - Berry scripting language, support for I2S audio

  Copyright (C) 2021 Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

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

#ifdef USE_I2S_AUDIO_BERRY

// #include "AudioFileSourceSPIFFS.h"
// #include "AudioFileSourceID3.h"
#include "AudioOutputI2S.h"
#include "AudioGeneratorWAV.h"
#include "AudioGeneratorMP3.h"
#include "AudioFileSourceFS.h"

#include <berry.h>


/*********************************************************************************************\
 * AudioOutput class
 * 
\*********************************************************************************************/
extern "C" {

  //
  // AudioOutputI2S(bclkPin: int, wclkPin: int, doutPin: int[, port:int, dmabuf:int, mode: int])
  //
  int i2s_output_i2s_init(bvm *vm) {
    int argc = be_top(vm);
    if (argc > 3) {
      int bclkPin = be_toint(vm, 2);
      int wclkPin = be_toint(vm, 3);
      int doutPin = be_toint(vm, 4);
      int port = 0;
      if (argc > 4) {
        port = be_toint(vm, 5);
      }
      int dma_buf_count = 8;  // number of dma buffers of 64 bytes
      if (argc > 5) {
        dma_buf_count = be_toint(vm, 6);
      }
      int mode = 0;   // EXTERNAL_I2S
      if (argc > 6) {
        mode = be_toint(vm, 7);
      }
      // AudioOutputI2S(int port=0, int output_mode=EXTERNAL_I2S, int dma_buf_count = 8, int use_apll=APLL_DISABLE);
      AudioOutputI2S * audio = new AudioOutputI2S(port, mode, dma_buf_count);
      if (0 == mode) {
        audio->SetPinout(bclkPin, wclkPin, doutPin);    // return value has no useful information for us
      }
      be_pushcomptr(vm, (void*) audio);
      be_setmember(vm, 1, ".p");
      be_return_nil(vm);
    }

    be_raise(vm, kTypeError, nullptr);
  }

  int i2s_output_i2s_deinit(bvm *vm) {
    int argc = be_top(vm);
    be_getmember(vm, 1, ".p");
    AudioOutputI2S * audio = (AudioOutputI2S *) be_tocomptr(vm, -1);
    if (audio) {
      delete audio;
      // clear
      be_pushcomptr(vm, (void*) NULL);
      be_setmember(vm, 1, ".p");
    }

    be_return_nil(vm);
  }

  int i2s_output_i2s_stop(bvm *vm) {
    int argc = be_top(vm);
    be_getmember(vm, 1, ".p");
    AudioOutputI2S * audio = (AudioOutputI2S *) be_tocomptr(vm, -1);
    if (audio) {
      audio->stop();
    }
    be_return_nil(vm);
  }

  //
  // AudioGeneratorWAV()
  //
  int i2s_generator_wav_init(bvm *vm) {
    AudioGeneratorWAV * wav = new AudioGeneratorWAV();
    be_pushcomptr(vm, (void*) wav);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  AudioGeneratorWAV * i2s_generator_wav_get(bvm *vm) {
    be_getmember(vm, 1, ".p");
    AudioGeneratorWAV * wav = (AudioGeneratorWAV *) be_tocomptr(vm, -1);
    return wav;
  }

  int i2s_generator_wav_deinit(bvm *vm) {
    int argc = be_top(vm);
    AudioGeneratorWAV * wav = i2s_generator_wav_get(vm);
    if (wav) {
      delete wav;
      // clear
      be_pushcomptr(vm, (void*) NULL);
      be_setmember(vm, 1, ".p");
    }

    be_return_nil(vm);
  }

  int i2s_generator_wav_begin(bvm *vm) {
    int argc = be_top(vm);
    if (argc > 2) {
      AudioGeneratorWAV * wav = i2s_generator_wav_get(vm);
      be_getmember(vm, 2, ".p");
      AudioFileSource * source = (AudioFileSource*) be_tocomptr(vm, -1);
      be_getmember(vm, 3, ".p");
      AudioOutput * output = (AudioOutput*) be_tocomptr(vm, -1);
      be_pop(vm, 2);

      bool ret = wav->begin(source, output);
      be_pushbool(vm, ret);
      be_return(vm);
    }
    be_return_nil(vm);
  }

  int i2s_generator_wav_loop(bvm *vm) {
    AudioGeneratorWAV * wav = i2s_generator_wav_get(vm);
    bool ret = wav->loop();
    be_pushbool(vm, ret);
    be_return(vm);
  }

  int i2s_generator_wav_stop(bvm *vm) {
    AudioGeneratorWAV * wav = i2s_generator_wav_get(vm);
    bool ret = wav->stop();
    be_pushbool(vm, ret);
    be_return(vm);
  }

  int i2s_generator_wav_isrunning(bvm *vm) {
    AudioGeneratorWAV * wav = i2s_generator_wav_get(vm);
    bool ret = wav->isRunning();
    be_pushbool(vm, ret);
    be_return(vm);
  }

  //
  // AudioGeneratorMP3()
  //
  int i2s_generator_mp3_init(bvm *vm) {
    AudioGeneratorMP3 * mp3 = new AudioGeneratorMP3();
    be_pushcomptr(vm, (void*) mp3);
    be_setmember(vm, 1, ".p");
    be_return_nil(vm);
  }

  AudioGeneratorMP3 * i2s_generator_mp3_get(bvm *vm) {
    be_getmember(vm, 1, ".p");
    AudioGeneratorMP3 * mp3 = (AudioGeneratorMP3 *) be_tocomptr(vm, -1);
    return mp3;
  }

  int i2s_generator_mp3_deinit(bvm *vm) {
    int argc = be_top(vm);
    AudioGeneratorMP3 * mp3 = i2s_generator_mp3_get(vm);
    if (mp3) {
      delete mp3;
      // clear
      be_pushcomptr(vm, (void*) NULL);
      be_setmember(vm, 1, ".p");
    }

    be_return_nil(vm);
  }

  int i2s_generator_mp3_begin(bvm *vm) {
    int argc = be_top(vm);
    if (argc > 2) {
      AudioGeneratorMP3 * mp3 = i2s_generator_mp3_get(vm);
      be_getmember(vm, 2, ".p");
      AudioFileSource * source = (AudioFileSource*) be_tocomptr(vm, -1);
      be_getmember(vm, 3, ".p");
      AudioOutput * output = (AudioOutput*) be_tocomptr(vm, -1);
      be_pop(vm, 2);

      bool ret = mp3->begin(source, output);
      be_pushbool(vm, ret);
      be_return(vm);
    }
    be_return_nil(vm);
  }

  int i2s_generator_mp3_loop(bvm *vm) {
    AudioGeneratorMP3 * mp3 = i2s_generator_mp3_get(vm);
    bool ret = mp3->loop();
    be_pushbool(vm, ret);
    be_return(vm);
  }

  int i2s_generator_mp3_stop(bvm *vm) {
    AudioGeneratorMP3 * mp3 = i2s_generator_mp3_get(vm);
    bool ret = mp3->stop();
    be_pushbool(vm, ret);
    be_return(vm);
  }

  int i2s_generator_mp3_isrunning(bvm *vm) {
    AudioGeneratorMP3 * mp3 = i2s_generator_mp3_get(vm);
    bool ret = mp3->isRunning();
    be_pushbool(vm, ret);
    be_return(vm);
  }

  // File Source FS
  //
#ifdef USE_UFILESYS
  int i2s_file_source_fs_init(bvm *vm) {
    int argc = be_top(vm);
    if (argc > 1 && be_isstring(vm, 2)) {
      const char * file_name = be_tostring(vm, 2);
      AudioFileSourceFS * file_source = new AudioFileSourceFS(*ufsp, file_name);
      be_pushcomptr(vm, (void*) file_source);
      be_setmember(vm, 1, ".p");
      be_return_nil(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int i2s_file_source_fs_deinit(bvm *vm) {
    int argc = be_top(vm);
    be_getmember(vm, 1, ".p");
    AudioFileSourceFS * file_source = (AudioFileSourceFS *) be_tocomptr(vm, -1);
    if (file_source) {
      delete file_source;
      // clear
      be_pushcomptr(vm, (void*) NULL);
      be_setmember(vm, 1, ".p");
    }

    be_return_nil(vm);
  }
#endif // USE_UFILESYS
}

#endif  // USE_I2S_AUDIO_BERRY
#endif  // USE_BERRY
