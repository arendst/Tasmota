#include "be_constobj.h"

static be_define_const_map_slots(be_class_AudioOutputI2S_map) {
    { be_const_key_weak(INTERNAL_PDM, -1), be_const_int(AudioOutputI2S::INTERNAL_PDM) },
    { be_const_key_weak(INTERNAL_DAC, 2), be_const_int(AudioOutputI2S::INTERNAL_DAC) },
    { be_const_key_weak(init, -1), be_const_func(i2s_output_i2s_init) },
    { be_const_key_weak(EXTERNAL_I2S, 1), be_const_int(AudioOutputI2S::EXTERNAL_I2S) },
    { be_const_key_weak(deinit, -1), be_const_func(i2s_output_i2s_deinit) },
    { be_const_key_weak(stop, -1), be_const_func(i2s_output_i2s_stop) },
};

static be_define_const_map(
    be_class_AudioOutputI2S_map,
    6
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_AudioOutputI2S,
    0,
    (bclass *)&be_class_AudioOutput,
    AudioOutputI2S
);
