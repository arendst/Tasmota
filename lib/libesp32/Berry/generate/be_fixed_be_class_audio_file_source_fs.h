#include "be_constobj.h"

static be_define_const_map_slots(be_class_audio_file_source_fs_map) {
    { be_const_key(deinit, -1), be_const_func(i2s_file_source_fs_deinit) },
    { be_const_key(init, -1), be_const_func(i2s_file_source_fs_init) },
};

static be_define_const_map(
    be_class_audio_file_source_fs_map,
    2
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_audio_file_source_fs,
    0,
    (bclass *)&be_class_audio_file_source,
    AudioFileSourceFS
);
