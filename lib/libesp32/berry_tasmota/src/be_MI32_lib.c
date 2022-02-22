/********************************************************************
 * Tasmota lib
 * 
 * To use: `import MI32`
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_MI_ESP32
extern int be_MI32_devices(bvm *vm);
extern int be_MI32_set_bat(bvm *vm);
extern int be_MI32_get_name(bvm *vm);
extern int be_MI32_get_MAC(bvm *vm);
extern int be_MI32_set_hum(bvm *vm);
extern int be_MI32_set_temp(bvm *vm);

/********************************************************************
** Solidified class: MI32
********************************************************************/
be_local_class(MI32,
    0,
    NULL,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("devices", -1593144448, 7, -1), be_const_func(be_MI32_devices) },
        { be_nested_key("set_bat", -1558299945, 7, -1), be_const_func(be_MI32_set_bat) },
        { be_nested_key("set_hum", 964296026, 7, 4), be_const_func(be_MI32_set_hum) },
        { be_nested_key("get_MAC", 2091521771, 7, -1), be_const_func(be_MI32_get_MAC) },
        { be_nested_key("set_temp", 1952131250, 8, -1), be_const_func(be_MI32_set_temp) },
        { be_nested_key("get_name", 1616902907, 8, 3), be_const_func(be_MI32_get_name) },
    })),
    (be_nested_const_str("MI32", -220693882, 4))
);
/*******************************************************************/

void be_load_MI32_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_MI32);
    be_setglobal(vm, "MI32");
    be_pop(vm, 1);
}

extern int be_BLE_reg_conn_cb(bvm *vm);
extern int be_BLE_reg_adv_cb(bvm *vm);
extern int be_BLE_set_MAC(bvm *vm);
extern int be_BLE_set_characteristic(bvm *vm);
extern int be_BLE_run(bvm *vm);
extern int be_BLE_set_service(bvm *vm);
/********************************************************************
** Solidified class: BLE
********************************************************************/
be_local_class(BLE,
    0,
    NULL,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("conn_cb", 1381122945, 7, -1), be_const_func(be_BLE_reg_conn_cb) },
        { be_nested_key("set_svc", 752734654, 7, -1), be_const_func(be_BLE_set_service) },
        { be_nested_key("run", 718098122, 3, -1), be_const_func(be_BLE_run) },
        { be_nested_key("set_chr", 102133743, 7, 0), be_const_func(be_BLE_set_characteristic) },
        { be_nested_key("adv_cb", 1957890034, 6, 1), be_const_func(be_BLE_reg_adv_cb) },
        { be_nested_key("set_MAC", 1617581015, 7, -1), be_const_func(be_BLE_set_MAC) },
    })),
    (be_nested_const_str("BLE", -361123990, 3))
);
/*******************************************************************/

void be_load_BLE_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_BLE);
    be_setglobal(vm, "BLE");
    be_pop(vm, 1);
}

#endif //USE_MI_ESP32
