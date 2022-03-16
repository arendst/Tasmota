#include "be_constobj.h"

static be_define_const_map_slots(m_liblv_map) {
    { be_const_key(init, -1), be_const_closure(lv_lv_module_init_closure) },
    { be_const_key(member, 0), be_const_func(lv0_member) },
};

static be_define_const_map(
    m_liblv_map,
    2
);

static be_define_const_module(
    m_liblv,
    "lv"
);

BE_EXPORT_VARIABLE be_define_const_native_module(lv);
