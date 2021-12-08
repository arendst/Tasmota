#include "be_constobj.h"

static be_define_const_map_slots(m_libpath_map) {
    { be_const_key(listdir, -1), be_const_func(m_path_listdir) },
    { be_const_key(last_modified, 2), be_const_func(m_path_last_modified) },
    { be_const_key(exists, 3), be_const_func(m_path_exists) },
    { be_const_key(remove, -1), be_const_func(m_path_remove) },
};

static be_define_const_map(
    m_libpath_map,
    4
);

static be_define_const_module(
    m_libpath,
    "path"
);

BE_EXPORT_VARIABLE be_define_const_native_module(path);
