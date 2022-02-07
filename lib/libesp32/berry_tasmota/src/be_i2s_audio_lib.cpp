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
AudioOutput* be_audio_output_init_ntv(void) {
  return new AudioOutput();
}
int32_t be_audio_output_init(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_init_ntv, "+.p", "");
}

// AudioOutput.set_rate(rate_hz:int) -> bool
int be_audio_output_set_rate_ntv(AudioOutput* out, int hz) {
  return out->SetRate(hz);
}
int32_t be_audio_output_set_rate(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_set_rate_ntv, "b", ".i");
}

// AudioOutput.set_bits_per_sample(bits_per_sample:int) -> bool
int be_audio_output_set_bits_per_sample_ntv(AudioOutput* out, int bps) {
  return out->SetBitsPerSample(bps);
}
int32_t be_audio_output_set_bits_per_sample(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_set_bits_per_sample_ntv, "b", ".i");
}

// AudioOutput.set_channels(channels:int) -> bool
int be_audio_output_set_channels_ntv(AudioOutput* out, int channels) {
  return out->SetChannels(channels);
}
int32_t be_audio_output_set_channels(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_set_channels_ntv, "b", ".i");
}

// AudioOutput.set_gain(gain:real) -> bool
int be_audio_output_set_gain_ntv(AudioOutput* out, float gain) {
  return out->SetGain(gain);
}
int32_t be_audio_output_set_gain(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_set_gain_ntv, "b", ".f");
}

// AudioOutput.begin() -> bool
int be_audio_output_begin_ntv(AudioOutput* out) {
  return out->begin();
}
int32_t be_audio_output_begin(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_begin_ntv, "b", ".");
}
// AudioOutput.stop() -> bool
int be_audio_output_stop_ntv(AudioOutput* out) {
  return out->stop();
}
int32_t be_audio_output_stop(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_stop_ntv, "b", ".");
}
// AudioOutput.flush() -> bool
void be_audio_output_flush_ntv(AudioOutput* out) {
  out->flush();
}
int32_t be_audio_output_flush(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_flush_ntv, "", ".");
}

// AudioOutput.consume_mono(bytes) -> int
int be_audio_output_consume_mono_ntv(AudioOutput* out, uint16_t *pcm, int bytes_len, int index) {
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
int32_t be_audio_output_consume_mono(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_consume_mono_ntv, "i", ".(bytes)~i");
}

// AudioOutput.consume_stereo(bytes) -> int
int be_audio_output_consume_stereo_ntv(AudioOutput* out, uint16_t *pcm, int bytes_len, int index) {
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
int32_t be_audio_output_consume_stereo(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_consume_stereo_ntv, "i", ".(bytes)~i");
}

// AudioOutput.consume_silence() -> int, push silence frames
int be_audio_output_consume_silence_ntv(AudioOutput* out) {
  int n = 0;
  int16_t ms[2] = {0, 0};
  while (true) {
    if (!out->ConsumeSample(ms)) { break; }
    n++;
  }
  return n;
}
int32_t be_audio_output_consume_silence(struct bvm *vm) {
  return be_call_c_func(vm, (void*) &be_audio_output_consume_silence_ntv, "i", ".");
}

#include "AudioOutputI2S.h"

extern "C" {
  
#include "be_fixed_be_class_audio_output.h"
#include "be_fixed_be_class_audio_output_i2s.h"
#include "be_fixed_be_class_audio_generator.h"
#include "be_fixed_be_class_audio_generator_wav.h"
#include "be_fixed_be_class_audio_generator_mp3.h"
#include "be_fixed_be_class_audio_file_source.h"
#include "be_fixed_be_class_audio_file_source_fs.h"

  void be_load_driver_audio_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_audio_output);
    be_setglobal(vm, "AudioOutput");
    be_pop(vm, 1);

    be_pushntvclass(vm, &be_class_audio_output_i2s);
    be_setglobal(vm, "AudioOutputI2S");
    be_pop(vm, 1);

    be_pushntvclass(vm, &be_class_audio_generator_wav);
    be_setglobal(vm, "AudioGeneratorWAV");
    be_pop(vm, 1);

    be_pushntvclass(vm, &be_class_audio_generator_mp3);
    be_setglobal(vm, "AudioGeneratorMP3");
    be_pop(vm, 1);

  #ifdef USE_UFILESYS
    be_pushntvclass(vm, &be_class_audio_file_source_fs);
    be_setglobal(vm, "AudioFileSourceFS");
    be_pop(vm, 1);
  #endif // USE_UFILESYS
  }
}

/* @const_object_info_begin

class be_class_audio_output (scope: global, name: AudioOutput) {
    .p, var
    init, func(be_audio_output_init)

    begin, func(be_audio_output_begin)
    stop, func(be_audio_output_stop)
    flush, func(be_audio_output_flush)

    consume_mono, func(be_audio_output_consume_mono)
    consume_stereo, func(be_audio_output_consume_stereo)
    consume_silence, func(be_audio_output_consume_silence)

    set_rate, func(be_audio_output_set_rate)
    set_bits_per_sample, func(be_audio_output_set_bits_per_sample)
    set_channels, func(be_audio_output_set_channels)
    set_gain, func(be_audio_output_set_gain)
}

class be_class_audio_generator (scope: global, name: AudioGenerator) {
    .p, var
}

class be_class_audio_file_source (scope: global, name: AudioFileSource) {
    .p, var
}

class be_class_audio_output_i2s (scope: global, name: AudioOutputI2S, super: be_class_audio_output) {
    EXTERNAL_I2S, int(AudioOutputI2S::EXTERNAL_I2S)
    INTERNAL_DAC, int(AudioOutputI2S::INTERNAL_DAC)
    INTERNAL_PDM, int(AudioOutputI2S::INTERNAL_PDM)

    init, func(i2s_output_i2s_init)
    deinit, func(i2s_output_i2s_deinit)
    stop, func(i2s_output_i2s_stop)
}

class be_class_audio_generator_wav (scope: global, name: AudioGeneratorWAV, super: be_class_audio_generator) {
    init, func(i2s_generator_wav_init)
    deinit, func(i2s_generator_wav_deinit)
    begin, func(i2s_generator_wav_begin)
    loop, func(i2s_generator_wav_loop)
    stop, func(i2s_generator_wav_stop)
    isrunning, func(i2s_generator_wav_isrunning)
}

class be_class_audio_generator_mp3 (scope: global, name: AudioGeneratorMP3, super: be_class_audio_generator) {
    init, func(i2s_generator_mp3_init)
    deinit, func(i2s_generator_mp3_deinit)
    begin, func(i2s_generator_mp3_begin)
    loop, func(i2s_generator_mp3_loop)
    stop, func(i2s_generator_mp3_stop)
    isrunning, func(i2s_generator_mp3_isrunning)
}

class be_class_audio_file_source_fs (scope: global, name: AudioFileSourceFS, super: be_class_audio_file_source) {
    init, func(i2s_file_source_fs_init)
    deinit, func(i2s_file_source_fs_deinit)
}

@const_object_info_end */

#endif // USE_I2S_AUDIO_BERRY
#endif // USE_I2S