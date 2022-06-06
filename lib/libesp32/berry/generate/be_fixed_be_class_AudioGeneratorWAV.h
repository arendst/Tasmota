#include "be_constobj.h"

static be_define_const_map_slots(be_class_AudioGeneratorWAV_map) {
    { be_const_key_weak(begin, -1), be_const_func(i2s_generator_wav_begin) },
    { be_const_key_weak(loop, -1), be_const_func(i2s_generator_wav_loop) },
    { be_const_key_weak(isrunning, -1), be_const_func(i2s_generator_wav_isrunning) },
    { be_const_key_weak(init, 1), be_const_func(i2s_generator_wav_init) },
    { be_const_key_weak(deinit, -1), be_const_func(i2s_generator_wav_deinit) },
    { be_const_key_weak(stop, -1), be_const_func(i2s_generator_wav_stop) },
};

static be_define_const_map(
    be_class_AudioGeneratorWAV_map,
    6
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_AudioGeneratorWAV,
    0,
    (bclass *)&be_class_AudioGenerator,
    AudioGeneratorWAV
);
