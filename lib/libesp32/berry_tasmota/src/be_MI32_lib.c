/********************************************************************
 * Tasmota lib
 * 
 * To use: `import MI32`
 *******************************************************************/
#include "be_constobj.h"
#include "be_mapping.h"

#ifdef USE_MI_ESP32
extern int be_MI32_devices(void);
BE_FUNC_CTYPE_DECLARE(be_MI32_devices, "i", "");

extern void be_MI32_set_bat(int slot, int bat_val);
BE_FUNC_CTYPE_DECLARE(be_MI32_set_bat, "", "ii");

extern const char * be_MI32_get_name(int slot);
BE_FUNC_CTYPE_DECLARE(be_MI32_get_name, "s", "i");

extern uint8_t *be_MI32_get_MAC(int32_t slot, size_t *size);
BE_FUNC_CTYPE_DECLARE(be_MI32_get_MAC, "&", "i");

extern void be_MI32_set_hum(int slot, int hum_val);
BE_FUNC_CTYPE_DECLARE(be_MI32_set_hum, "", "ii");

extern void be_MI32_set_temp(int slot, int temp_val);
BE_FUNC_CTYPE_DECLARE(be_MI32_set_temp, "", "ii");

extern bbool be_MI32_widget(const char *sbuf, void* function);
BE_FUNC_CTYPE_DECLARE(be_MI32_widget, "b", "[sc]");

#include "be_fixed_MI32.h"

/* @const_object_info_begin
module MI32 (scope: global) {
  devices,    ctype_func(be_MI32_devices)
  get_name,   ctype_func(be_MI32_get_name)
  get_MAC,    ctype_func(be_MI32_get_MAC)
  set_bat,    ctype_func(be_MI32_set_bat)
  set_hum,    ctype_func(be_MI32_set_hum)
  set_temp,   ctype_func(be_MI32_set_temp)
  widget,     ctype_func(be_MI32_widget)
}
@const_object_info_end */

/********************************************************************
 * Tasmota lib
 * 
 * To use: `import BLE`
 *******************************************************************/

extern int be_BLE_init(bvm *vm);

extern int be_BLE_info(bvm *vm);

extern void be_BLE_loop(void);
BE_FUNC_CTYPE_DECLARE(be_BLE_loop, "", "");

extern void be_BLE_reg_conn_cb(void* function, uint8_t *buffer);
BE_FUNC_CTYPE_DECLARE(be_BLE_reg_conn_cb, "", "cc");

extern void be_BLE_reg_adv_cb(void* function, uint8_t *buffer);
BE_FUNC_CTYPE_DECLARE(be_BLE_reg_adv_cb, "", "c[c]");

extern void be_BLE_reg_server_cb(void* function, uint8_t *buffer);
BE_FUNC_CTYPE_DECLARE(be_BLE_reg_server_cb, "", "c[c]");

extern void be_BLE_set_MAC(struct bvm *vm, uint8_t *buf, size_t size, uint8_t type);
BE_FUNC_CTYPE_DECLARE(be_BLE_set_MAC, "", "@(bytes)~[i]");

extern void be_BLE_set_characteristic(struct bvm *vm, const char *Chr);
BE_FUNC_CTYPE_DECLARE(be_BLE_set_characteristic, "", "@s");

extern void be_BLE_run(struct bvm *vm, uint8_t operation, bbool response, int32_t arg1);
BE_FUNC_CTYPE_DECLARE(be_BLE_run, "", "@i[bi]");

extern void be_BLE_store(uint8_t *buf, size_t size);
BE_FUNC_CTYPE_DECLARE(be_BLE_store, "", "(bytes)~");

extern void be_BLE_set_service(struct bvm *vm, const char *Svc, bbool discoverAttributes);
BE_FUNC_CTYPE_DECLARE(be_BLE_set_service, "", "@s[b]");

extern void be_BLE_adv_watch(struct bvm *vm, uint8_t *buf, size_t size, uint8_t type);
BE_FUNC_CTYPE_DECLARE(be_BLE_adv_watch, "", "@(bytes)~[i]");


#include "be_fixed_BLE.h"

/* @const_object_info_begin
module BLE (scope: global) {
  init,       func(be_BLE_init)
  loop,       ctype_func(be_BLE_loop)
  info,       func(be_BLE_info)
  conn_cb,    ctype_func(be_BLE_reg_conn_cb)
  set_svc,    ctype_func(be_BLE_set_service)
  run,        ctype_func(be_BLE_run)
  store,      ctype_func(be_BLE_store)
  set_chr,    ctype_func(be_BLE_set_characteristic)
  adv_cb,     ctype_func(be_BLE_reg_adv_cb)
  set_MAC,    ctype_func(be_BLE_set_MAC)
  adv_watch,  ctype_func(be_BLE_adv_watch)
  serv_cb,    ctype_func(be_BLE_reg_server_cb)
}
@const_object_info_end */

#endif //USE_MI_ESP32