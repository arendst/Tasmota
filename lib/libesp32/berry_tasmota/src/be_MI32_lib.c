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

#include "be_fixed_be_class_MI32.h"

/* @const_object_info_begin
class be_class_MI32 (scope: global, name: MI32) {
  devices,    func(be_MI32_devices)
  get_name,   func(be_MI32_get_name)
  get_MAC,    func(be_MI32_get_MAC)
  set_bat,    func(be_MI32_set_bat)
  set_hum,    func(be_MI32_set_hum)
  set_temp,   func(be_MI32_set_temp)
}
@const_object_info_end */

extern int be_BLE_reg_conn_cb(bvm *vm);
extern int be_BLE_reg_adv_cb(bvm *vm);
extern int be_BLE_set_MAC(bvm *vm);
extern int be_BLE_set_characteristic(bvm *vm);
extern int be_BLE_run(bvm *vm);
extern int be_BLE_set_service(bvm *vm);
extern int be_BLE_adv_watch(bvm *vm);
extern int be_BLE_adv_block(bvm *vm);

#include "be_fixed_be_class_BLE.h"

/* @const_object_info_begin
class be_class_BLE (scope: global, name: BLE, strings: weak) {
  conn_cb,    func(be_BLE_reg_conn_cb)
  set_svc,    func(be_BLE_set_service)
  run,        func(be_BLE_run)
  set_chr,    func(be_BLE_set_characteristic)
  adv_cb,     func(be_BLE_reg_adv_cb)
  set_MAC,    func(be_BLE_set_MAC)
  adv_watch,  func(be_BLE_adv_watch)
  adv_block,  func(be_BLE_adv_block)
}
@const_object_info_end */

#endif //USE_MI_ESP32