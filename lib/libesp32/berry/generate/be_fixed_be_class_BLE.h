#include "be_constobj.h"

static be_define_const_map_slots(be_class_BLE_map) {
    { be_const_key_weak(adv_block, -1), be_const_func(be_BLE_adv_block) },
    { be_const_key_weak(conn_cb, -1), be_const_func(be_BLE_reg_conn_cb) },
    { be_const_key_weak(adv_cb, 5), be_const_func(be_BLE_reg_adv_cb) },
    { be_const_key_weak(set_MAC, -1), be_const_func(be_BLE_set_MAC) },
    { be_const_key_weak(adv_watch, -1), be_const_func(be_BLE_adv_watch) },
    { be_const_key_weak(run, -1), be_const_func(be_BLE_run) },
    { be_const_key_weak(set_svc, 4), be_const_func(be_BLE_set_service) },
    { be_const_key_weak(set_chr, 3), be_const_func(be_BLE_set_characteristic) },
};

static be_define_const_map(
    be_class_BLE_map,
    8
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_BLE,
    0,
    NULL,
    BLE
);
