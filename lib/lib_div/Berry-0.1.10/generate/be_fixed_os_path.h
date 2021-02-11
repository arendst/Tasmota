#include "be_constobj.h"

static be_define_const_map_slots(m_libpath_map) {
    { be_const_key(isdir, -1), be_const_func(m_path_isdir) },
    { be_const_key(join, 2), be_const_func(m_path_join) },
    { be_const_key(exists, -1), be_const_func(m_path_exists) },
    { be_const_key(split, -1), be_const_func(m_path_split) },
    { be_const_key(splitext, -1), be_const_func(m_path_splitext) },
    { be_const_key(isfile, -1), be_const_func(m_path_isfile) },
};

static be_define_const_map(
    m_libpath_map,
    6
);

static be_define_const_module(
    m_libpath,
    "path"
);
