#include "be_constobj.h"

static be_define_const_map_slots(be_class_AudioGeneratorMP3_map) {
    { be_const_key_weak(begin, -1), be_const_func(i2s_generator_mp3_begin) },
    { be_const_key_weak(loop, -1), be_const_func(i2s_generator_mp3_loop) },
    { be_const_key_weak(isrunning, -1), be_const_func(i2s_generator_mp3_isrunning) },
    { be_const_key_weak(init, 1), be_const_func(i2s_generator_mp3_init) },
    { be_const_key_weak(deinit, -1), be_const_func(i2s_generator_mp3_deinit) },
    { be_const_key_weak(stop, -1), be_const_func(i2s_generator_mp3_stop) },
};

static be_define_const_map(
    be_class_AudioGeneratorMP3_map,
    6
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_AudioGeneratorMP3,
    0,
    (bclass *)&be_class_AudioGenerator,
    AudioGeneratorMP3
);
