#include "be_constobj.h"

static be_define_const_map_slots(m_libpath_map) {
    { be_const_key(exists, -1), be_const_func(m_path_exists) },
    { be_const_key(last_modified, 0), be_const_func(m_path_last_modified) },
};

static be_define_const_map(
    m_libpath_map,
    2
);

static be_define_const_module(
    m_libpath,
    "path"
);

BE_EXPORT_VARIABLE be_define_const_native_module(path, NULL);
