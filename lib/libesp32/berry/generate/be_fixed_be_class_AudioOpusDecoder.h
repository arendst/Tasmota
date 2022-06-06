#include "be_constobj.h"

static be_define_const_map_slots(be_class_AudioOpusDecoder_map) {
    { be_const_key_weak(deinit, -1), be_const_func(be_audio_opus_decoder_deinit) },
    { be_const_key_weak(decode, 2), be_const_func(be_audio_opus_decoder_decode) },
    { be_const_key_weak(_X2Ep, -1), be_const_var(0) },
    { be_const_key_weak(init, 1), be_const_func(be_audio_opus_decoder_init) },
};

static be_define_const_map(
    be_class_AudioOpusDecoder_map,
    4
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_AudioOpusDecoder,
    1,
    NULL,
    AudioOpusDecoder
);
