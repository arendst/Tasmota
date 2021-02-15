#include "be_constobj.h"

static be_define_const_map_slots(m_libos_map) {
    { be_const_key(listdir, 1), be_const_func(m_listdir) },
    { be_const_key(mkdir, -1), be_const_func(m_mkdir) },
    { be_const_key(chdir, -1), be_const_func(m_chdir) },
    { be_const_key(exit, 2), be_const_func(m_exit) },
    { be_const_key(system, -1), be_const_func(m_system) },
    { be_const_key(remove, 3), be_const_func(m_remove) },
    { be_const_key(path, -1), be_const_module(m_libpath) },
    { be_const_key(getcwd, -1), be_const_func(m_getcwd) },
};

static be_define_const_map(
    m_libos_map,
    8
);

static be_define_const_module(
    m_libos,
    "os"
);

BE_EXPORT_VARIABLE be_define_const_native_module(os, NULL);
