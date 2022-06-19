#include "be_constobj.h"

static be_define_const_map_slots(be_class_AudioFileSourceFS_map) {
    { be_const_key_weak(deinit, -1), be_const_func(i2s_file_source_fs_deinit) },
    { be_const_key_weak(init, -1), be_const_func(i2s_file_source_fs_init) },
};

static be_define_const_map(
    be_class_AudioFileSourceFS_map,
    2
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_AudioFileSourceFS,
    0,
    (bclass *)&be_class_AudioFileSource,
    AudioFileSourceFS
);
