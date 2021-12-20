#include "be_constobj.h"

static be_define_const_map_slots(m_libcb_map) {
    { be_const_key(make_cb, -1), be_const_func(be_cb_make_cb) },
    { be_const_key(list_handlers, 3), be_const_func(be_cb_list_handlers) },
    { be_const_key(get_cb_list, -1), be_const_func(be_cb_get_cb_list) },
    { be_const_key(gen_cb, -1), be_const_func(be_cb_gen_cb) },
    { be_const_key(add_handler, -1), be_const_func(be_cb_add_handler) },
};

static be_define_const_map(
    m_libcb_map,
    5
);

static be_define_const_module(
    m_libcb,
    "cb"
);

BE_EXPORT_VARIABLE be_define_const_native_module(cb);
