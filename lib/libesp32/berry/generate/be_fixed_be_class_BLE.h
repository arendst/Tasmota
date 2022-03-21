#include "be_constobj.h"

static be_define_const_map_slots(be_class_BLE_map) {
    { be_const_key(set_chr, -1), be_const_func(be_BLE_set_characteristic) },
    { be_const_key(set_svc, -1), be_const_func(be_BLE_set_service) },
    { be_const_key(run, -1), be_const_func(be_BLE_run) },
    { be_const_key(conn_cb, 0), be_const_func(be_BLE_reg_conn_cb) },
    { be_const_key(adv_cb, 1), be_const_func(be_BLE_reg_adv_cb) },
    { be_const_key(set_MAC, -1), be_const_func(be_BLE_set_MAC) },
};

static be_define_const_map(
    be_class_BLE_map,
    6
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_BLE,
    0,
    NULL,
    BLE
);
