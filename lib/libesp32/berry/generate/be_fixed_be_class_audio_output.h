#include "be_constobj.h"

static be_define_const_map_slots(be_class_audio_output_map) {
    { be_const_key(set_bits_per_sample, -1), be_const_func(be_audio_output_set_bits_per_sample) },
    { be_const_key(flush, -1), be_const_func(be_audio_output_flush) },
    { be_const_key(consume_stereo, -1), be_const_func(be_audio_output_consume_stereo) },
    { be_const_key(init, -1), be_const_func(be_audio_output_init) },
    { be_const_key(consume_mono, -1), be_const_func(be_audio_output_consume_mono) },
    { be_const_key(stop, -1), be_const_func(be_audio_output_stop) },
    { be_const_key(begin, -1), be_const_func(be_audio_output_begin) },
    { be_const_key(_X2Ep, -1), be_const_var(0) },
    { be_const_key(set_channels, -1), be_const_func(be_audio_output_set_channels) },
    { be_const_key(consume_silence, 4), be_const_func(be_audio_output_consume_silence) },
    { be_const_key(set_rate, 2), be_const_func(be_audio_output_set_rate) },
    { be_const_key(set_gain, 0), be_const_func(be_audio_output_set_gain) },
};

static be_define_const_map(
    be_class_audio_output_map,
    12
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_audio_output,
    1,
    NULL,
    AudioOutput
);
