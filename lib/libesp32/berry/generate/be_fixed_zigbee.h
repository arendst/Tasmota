#include "be_constobj.h"

static be_define_const_map_slots(m_libzigbee_map) {
    { be_const_key_weak(init, -1), be_const_func(zigbee_init) },
};

static be_define_const_map(
    m_libzigbee_map,
    1
);

static be_define_const_module(
    m_libzigbee,
    "zigbee"
);

BE_EXPORT_VARIABLE be_define_const_native_module(zigbee);
