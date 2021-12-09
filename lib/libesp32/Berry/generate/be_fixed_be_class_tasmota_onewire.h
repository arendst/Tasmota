#include "be_constobj.h"

static be_define_const_map_slots(be_class_tasmota_onewire_map) {
    { be_const_key(reset, -1), be_const_func(b_onewire_reset) },
    { be_const_key(search, 10), be_const_func(b_onewire_search) },
    { be_const_key(depower, -1), be_const_func(b_onewire_depower) },
    { be_const_key(init, 2), be_const_func(b_onewire_init) },
    { be_const_key(deinit, -1), be_const_func(b_onewire_deinit) },
    { be_const_key(reset_search, -1), be_const_func(b_onewire_reset_search) },
    { be_const_key(skip, -1), be_const_func(b_onewire_skip) },
    { be_const_key(_X2Ep, -1), be_const_var(0) },
    { be_const_key(write, -1), be_const_func(b_onewire_write) },
    { be_const_key(read, -1), be_const_func(b_onewire_read) },
    { be_const_key(select, 11), be_const_func(b_onewire_select) },
    { be_const_key(target_search, -1), be_const_func(b_onewire_target_search) },
};

static be_define_const_map(
    be_class_tasmota_onewire_map,
    12
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_tasmota_onewire,
    1,
    NULL,
    OneWire
);
