/********************************************************************
 * Tasmota I2S audio classes
 * 
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_I2S
#ifdef USE_I2S_AUDIO_BERRY

#include "be_mapping.h"

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


// AudioOutputI2S.init() -> instance 
extern void* be_audio_output_i2s_init(void);
BE_FUNC_CTYPE_DECLARE(be_audio_output_i2s_init, "+.p", "");

// AudioOutputI2S.deinit()-> void
extern void* be_audio_output_i2s_deinit(void* instance);
BE_FUNC_CTYPE_DECLARE(be_audio_output_i2s_deinit, "", ".");

// AudioOutput.begin() -> bool
extern int be_audio_output_i2s_begin(void* out);
BE_FUNC_CTYPE_DECLARE(be_audio_output_i2s_begin, "b", ".");

// AudioOutput.stop() -> bool
extern int be_audio_output_i2s_stop(void* out);
BE_FUNC_CTYPE_DECLARE(be_audio_output_i2s_stop, "b", ".");

// AudioOutput.flush() -> bool
extern void be_audio_output_i2s_flush(void* out);
BE_FUNC_CTYPE_DECLARE(be_audio_output_i2s_flush, "", ".");

// AudioOutput.set_rate(rate_hz:int) -> bool
extern int be_audio_output_set_rate(void* out, int hz);
BE_FUNC_CTYPE_DECLARE(be_audio_output_set_rate, "b", ".i");

// AudioOutput.set_bits_per_sample(bits_per_sample:int) -> bool
extern int be_audio_output_set_bits_per_sample(void* out, int bps);
BE_FUNC_CTYPE_DECLARE(be_audio_output_set_bits_per_sample, "b", ".i");

// AudioOutput.set_channels(channels:int) -> bool
extern int be_audio_output_set_channels(void* out, int channels);
BE_FUNC_CTYPE_DECLARE(be_audio_output_set_channels, "b", ".i");

// AudioOutput.set_gain(gain:real) -> bool
extern int be_audio_output_set_gain(void* out, float gain);
BE_FUNC_CTYPE_DECLARE(be_audio_output_set_gain, "b", ".f");



// AudioOutput.consume_mono(bytes) -> int
extern int be_audio_output_consume_mono(void* out, uint16_t *pcm, int bytes_len, int index);
BE_FUNC_CTYPE_DECLARE(be_audio_output_consume_mono, "i", ".(bytes)~i");

// AudioOutput.consume_stereo(bytes) -> int
extern int be_audio_output_consume_stereo(void* out, uint16_t *pcm, int bytes_len, int index);
BE_FUNC_CTYPE_DECLARE(be_audio_output_consume_stereo, "i", ".(bytes)~i");

// AudioOutput.consume_silence() -> int, push silence frames
extern int be_audio_output_consume_silence(void* out);
BE_FUNC_CTYPE_DECLARE(be_audio_output_consume_silence, "i", ".");

// AudioOutputI2S.set_lsb_justified(gain:real) -> nil
extern int i2s_output_i2s_set_lsb_justified(void* out, bbool lsbJustified);
BE_FUNC_CTYPE_DECLARE(i2s_output_i2s_set_lsb_justified, "b", ".b");


#include "be_fixed_be_class_AudioOutputI2S.h"
#include "be_fixed_be_class_AudioGenerator.h"
#include "be_fixed_be_class_AudioGeneratorWAV.h"
#include "be_fixed_be_class_AudioGeneratorMP3.h"
#include "be_fixed_be_class_AudioFileSource.h"
#include "be_fixed_be_class_AudioFileSourceFS.h"

/* @const_object_info_begin

class be_class_AudioGenerator (scope: global, name: AudioGenerator, strings: weak) {
    .p, var
}

class be_class_AudioFileSource (scope: global, name: AudioFileSource, strings: weak) {
    .p, var
}

class be_class_AudioOutputI2S (scope: global, name: AudioOutputI2S, strings: weak) {
    .p, var
    init, ctype_func(be_audio_output_i2s_init)
    deinit, ctype_func(be_audio_output_i2s_deinit)

    begin, ctype_func(be_audio_output_i2s_begin)
    stop, ctype_func(be_audio_output_i2s_stop)
    flush, ctype_func(be_audio_output_i2s_flush)

    consume_mono, ctype_func(be_audio_output_consume_mono)
    consume_stereo, ctype_func(be_audio_output_consume_stereo)
    consume_silence, ctype_func(be_audio_output_consume_silence)

    set_rate, ctype_func(be_audio_output_set_rate)
    set_bits_per_sample, ctype_func(be_audio_output_set_bits_per_sample)
    set_channels, ctype_func(be_audio_output_set_channels)
    set_gain, ctype_func(be_audio_output_set_gain)
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