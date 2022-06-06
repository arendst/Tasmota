#include "be_constobj.h"

static be_define_const_map_slots(be_class_OneWire_map) {
    { be_const_key_weak(reset, -1), be_const_func(b_onewire_reset) },
    { be_const_key_weak(search, 10), be_const_func(b_onewire_search) },
    { be_const_key_weak(depower, -1), be_const_func(b_onewire_depower) },
    { be_const_key_weak(init, 2), be_const_func(b_onewire_init) },
    { be_const_key_weak(deinit, -1), be_const_func(b_onewire_deinit) },
    { be_const_key_weak(reset_search, -1), be_const_func(b_onewire_reset_search) },
    { be_const_key_weak(skip, -1), be_const_func(b_onewire_skip) },
    { be_const_key_weak(_X2Ep, -1), be_const_var(0) },
    { be_const_key_weak(write, -1), be_const_func(b_onewire_write) },
    { be_const_key_weak(read, -1), be_const_func(b_onewire_read) },
    { be_const_key_weak(select, 11), be_const_func(b_onewire_select) },
    { be_const_key_weak(target_search, -1), be_const_func(b_onewire_target_search) },
};

static be_define_const_map(
    be_class_OneWire_map,
    12
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_OneWire,
    1,
    NULL,
    OneWire
);
