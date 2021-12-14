#include "be_constobj.h"

static be_define_const_map_slots(m_libcb_map) {
    { be_const_key(get_cb_list, -1), be_const_func(be_cb_get_cb_list) },
    { be_const_key(gen_cb, -1), be_const_func(be_cb_gen_cb) },
};

static be_define_const_map(
    m_libcb_map,
    2
);

static be_define_const_module(
    m_libcb,
    "cb"
);

BE_EXPORT_VARIABLE be_define_const_native_module(cb);
