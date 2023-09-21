/********************************************************************
 * Tasmota I2S audio classes
 * 
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_I2S
#ifdef USE_I2S_AUDIO_BERRY

#include "be_mapping.h"
#include "AudioOutput.h"

extern "C" void berry_log_C(const char * berry_buf, ...);

extern "C" {
  extern int i2s_output_i2s_init(bvm *vm);
  extern int i2s_output_i2s_deinit(bvm *vm);
  extern int i2s_output_i2s_stop(bvm *vm);

  extern int i2s_generator_wav_init(bvm *vm);
  extern int i2s_generator_wav_deinit(bvm *vm);
  extern int i2s_generator_wav_begin(bvm *vm);
  extern int i2s_generator_wav_loop(bvm *vm);
  extern int i2s_generator_wav_stop(bvm *vm);
  extern int i2s_generator_wav_isrunning(bvm *vm);

  extern int i2s_generator_mp3_init(bvm *vm);
  extern int i2s_generator_mp3_deinit(bvm *vm);
  extern int i2s_generator_mp3_begin(bvm *vm);
  extern int i2s_generator_mp3_loop(bvm *vm);
  extern int i2s_generator_mp3_stop(bvm *vm);
  extern int i2s_generator_mp3_isrunning(bvm *vm);

#ifdef USE_UFILESYS
  extern int i2s_file_source_fs_init(bvm *vm);
  extern int i2s_file_source_fs_deinit(bvm *vm);
#endif // USE_UFILESYS
}

// AudioOutput.set_rate(rate_hz:int) -> bool
AudioOutput* be_audio_output_init(void) {
  return new AudioOutput();
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_init, "+.p", "");

// AudioOutput.set_rate(rate_hz:int) -> bool
int be_audio_output_set_rate(AudioOutput* out, int hz) {
  return out->SetRate(hz);
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_set_rate, "b", ".i");

// AudioOutput.set_bits_per_sample(bits_per_sample:int) -> bool
int be_audio_output_set_bits_per_sample(AudioOutput* out, int bps) {
  return out->SetBitsPerSample(bps);
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_set_bits_per_sample, "b", ".i");

// AudioOutput.set_channels(channels:int) -> bool
int be_audio_output_set_channels(AudioOutput* out, int channels) {
  return out->SetChannels(channels);
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_set_channels, "b", ".i");

// AudioOutput.set_gain(gain:real) -> bool
int be_audio_output_set_gain(AudioOutput* out, float gain) {
  return out->SetGain(gain);
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_set_gain, "b", ".f");

// AudioOutput.begin() -> bool
int be_audio_output_begin(AudioOutput* out) {
  return out->begin();
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_begin, "b", ".");

// AudioOutput.stop() -> bool
int be_audio_output_stop(AudioOutput* out) {
  return out->stop();
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_stop, "b", ".");

// AudioOutput.flush() -> bool
void be_audio_output_flush(AudioOutput* out) {
  out->flush();
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_flush, "", ".");

// AudioOutput.consume_mono(bytes) -> int
int be_audio_output_consume_mono(AudioOutput* out, uint16_t *pcm, int bytes_len, int index) {
  int pcm_len = bytes_len / 2;
  int n;
  // berry_log_C("be_audio_output_consume_mono_ntv out=%p pcm=%p bytes_len=%i index=%i", out, pcm, bytes_len, index);
  for (n = 0; index + n < pcm_len; n++) {
    int16_t ms[2];
    ms[AudioOutput::LEFTCHANNEL] = ms[AudioOutput::RIGHTCHANNEL] = pcm[index + n];
    if (!out->ConsumeSample(ms)) { break; }
  }
  return n;
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_consume_mono, "i", ".(bytes)~i");

// AudioOutput.consume_stereo(bytes) -> int
int be_audio_output_consume_stereo(AudioOutput* out, uint16_t *pcm, int bytes_len, int index) {
  int pcm_len = bytes_len / 4;  // 2 samples LEFT+RIGHT of 2 bytes each
  int n;
  // berry_log_C("be_audio_output_consume_stereo_ntv out=%p pcm=%p bytes_len=%i index=%i", out, pcm, bytes_len, index);
  for (n = 0; index + n < pcm_len; n++) {
    int16_t ms[2];
    ms[AudioOutput::LEFTCHANNEL] = pcm[index + n + n];
    ms[AudioOutput::RIGHTCHANNEL] = pcm[index + n + n + 1];
    if (!out->ConsumeSample(ms)) { break; }
  }
  return n;
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_consume_stereo, "i", ".(bytes)~i");

// AudioOutput.consume_silence() -> int, push silence frames
int be_audio_output_consume_silence(AudioOutput* out) {
  int n = 0;
  int16_t ms[2] = {0, 0};
  while (true) {
    if (!out->ConsumeSample(ms)) { break; }
    n++;
  }
  return n;
}
BE_FUNC_CTYPE_DECLARE(be_audio_output_consume_silence, "i", ".");

#include "AudioOutputI2S.h"

// AudioOutputI2S.set_lsb_justified(gain:real) -> nil
int i2s_output_i2s_set_lsb_justified(AudioOutputI2S* out, bbool lsbJustified) {
  return out->SetLsbJustified(lsbJustified);
}
BE_FUNC_CTYPE_DECLARE(i2s_output_i2s_set_lsb_justified, "b", ".b");

extern "C" {
  
#include "be_fixed_be_class_AudioOutput.h"
#include "be_fixed_be_class_AudioOutputI2S.h"
#include "be_fixed_be_class_AudioGenerator.h"
#include "be_fixed_be_class_AudioGeneratorWAV.h"
#include "be_fixed_be_class_AudioGeneratorMP3.h"
#include "be_fixed_be_class_AudioFileSource.h"
#include "be_fixed_be_class_AudioFileSourceFS.h"

}
/* @const_object_info_begin

class be_class_AudioOutput (scope: global, name: AudioOutput, strings: weak) {
    .p, var
    init, ctype_func(be_audio_output_init)

    begin, ctype_func(be_audio_output_begin)
    stop, ctype_func(be_audio_output_stop)
    flush, ctype_func(be_audio_output_flush)

    consume_mono, ctype_func(be_audio_output_consume_mono)
    consume_stereo, ctype_func(be_audio_output_consume_stereo)
    consume_silence, ctype_func(be_audio_output_consume_silence)

    set_rate, ctype_func(be_audio_output_set_rate)
    set_bits_per_sample, ctype_func(be_audio_output_set_bits_per_sample)
    set_channels, ctype_func(be_audio_output_set_channels)
    set_gain, ctype_func(be_audio_output_set_gain)
}

class be_class_AudioGenerator (scope: global, name: AudioGenerator, strings: weak) {
    .p, var
}

class be_class_AudioFileSource (scope: global, name: AudioFileSource, strings: weak) {
    .p, var
}

class be_class_AudioOutputI2S (scope: global, name: AudioOutputI2S, super: be_class_AudioOutput, strings: weak) {
    EXTERNAL_I2S, int(AudioOutputI2S::EXTERNAL_I2S)
    INTERNAL_DAC, int(AudioOutputI2S::INTERNAL_DAC)
    INTERNAL_PDM, int(AudioOutputI2S::INTERNAL_PDM)

    init, func(i2s_output_i2s_init)
    deinit, func(i2s_output_i2s_deinit)
    stop, func(i2s_output_i2s_stop)

    set_lsb_justified, ctype_func(i2s_output_i2s_set_lsb_justified)
}

class be_class_AudioGeneratorWAV (scope: global, name: AudioGeneratorWAV, super: be_class_AudioGenerator, strings: weak) {
    init, func(i2s_generator_wav_init)
    deinit, func(i2s_generator_wav_deinit)
    begin, func(i2s_generator_wav_begin)
    loop, func(i2s_generator_wav_loop)
    stop, func(i2s_generator_wav_stop)
    isrunning, func(i2s_generator_wav_isrunning)
}

class be_class_AudioGeneratorMP3 (scope: global, name: AudioGeneratorMP3, super: be_class_AudioGenerator, strings: weak) {
    init, func(i2s_generator_mp3_init)
    deinit, func(i2s_generator_mp3_deinit)
    begin, func(i2s_generator_mp3_begin)
    loop, func(i2s_generator_mp3_loop)
    stop, func(i2s_generator_mp3_stop)
    isrunning, func(i2s_generator_mp3_isrunning)
}

class be_class_AudioFileSourceFS (scope: global, name: AudioFileSourceFS, super: be_class_AudioFileSource, strings: weak) {
    init, func(i2s_file_source_fs_init)
    deinit, func(i2s_file_source_fs_deinit)
}

@const_object_info_end */

#endif // USE_I2S_AUDIO_BERRY
#endif // USE_I2S