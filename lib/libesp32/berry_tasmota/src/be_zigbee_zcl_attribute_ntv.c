/********************************************************************
 * Zigbee zcl_attribute native format
 * 
 *******************************************************************/
#ifdef USE_ZIGBEE

#include "be_constobj.h"
#include "be_mapping.h"
#include "be_ctypes.h"

extern const bclass be_class_ctypes_bytes;
extern const be_ctypes_structure_t be_zigbee_zcl_attribute_struct;

extern int be_zigbee_zcl_attribute_ntv_init(struct bvm *vm);
extern int be_zigbee_zcl_attribute_ntv_deinit(struct bvm *vm);
extern int be_zigbee_zcl_attribute_ntv_get_val(struct bvm *vm);
extern int be_zigbee_zcl_attribute_ntv_set_val(struct bvm *vm);
extern int be_zigbee_zcl_attribute_ntv_get_key(struct bvm *vm);
extern int be_zigbee_zcl_attribute_ntv_set_key(struct bvm *vm);

#include "be_fixed_be_class_zcl_attribute_ntv.h"

/* @const_object_info_begin
class be_class_zcl_attribute_ntv (scope: global, name: zcl_attribute_ntv, super: be_class_ctypes_bytes, strings: weak) {
  _def, comptr(&be_zigbee_zcl_attribute_struct)
  _init, func(&be_zigbee_zcl_attribute_ntv_init)
  _deinit, func(&be_zigbee_zcl_attribute_ntv_deinit)

  _get_val, func(be_zigbee_zcl_attribute_ntv_get_val)
  _set_val, func(be_zigbee_zcl_attribute_ntv_set_val)
  _get_key, func(be_zigbee_zcl_attribute_ntv_get_key)
  _set_key, func(be_zigbee_zcl_attribute_ntv_set_key)
}
@const_object_info_end */

#endif // USE_LIGHT
