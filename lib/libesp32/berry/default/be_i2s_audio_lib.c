/********************************************************************
 * Tasmota I2S audio classes
 * 
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_I2S
#ifdef USE_I2S_AUDIO_BERRY

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


#include "../generate/be_fixed_be_class_audio_output.h"
#include "../generate/be_fixed_be_class_audio_output_i2s.h"
#include "../generate/be_fixed_be_class_audio_generator.h"
#include "../generate/be_fixed_be_class_audio_generator_wav.h"
#include "../generate/be_fixed_be_class_audio_generator_mp3.h"
#include "../generate/be_fixed_be_class_audio_file_source.h"
#include "../generate/be_fixed_be_class_audio_file_source_fs.h"

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

/* @const_object_info_begin

class be_class_audio_output (scope: global, name: AudioOutput) {
    .p, var
}

class be_class_audio_generator (scope: global, name: AudioGenerator) {
    .p, var
}

class be_class_audio_file_source (scope: global, name: AudioFileSource) {
    .p, var
}

class be_class_audio_output_i2s (scope: global, name: AudioOutputI2S, super: be_class_audio_output) {
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