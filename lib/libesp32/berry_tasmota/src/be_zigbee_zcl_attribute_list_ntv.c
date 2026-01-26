/********************************************************************
 * Zigbee zcl_attribute_list native format
 * 
 *******************************************************************/
#ifdef USE_ZIGBEE

#include "be_constobj.h"
#include "be_mapping.h"
#include "be_ctypes.h"

extern const bclass be_class_ctypes_bytes;
extern const be_ctypes_structure_t be_zigbee_zcl_attribute_list_struct;

extern int be_zigbee_zcl_attribute_list_ntv_init(struct bvm *vm);
extern int be_zigbee_zcl_attribute_list_ntv_deinit(struct bvm *vm);
extern int be_zigbee_zcl_attribute_list_ntv_size(struct bvm *vm);
extern int be_zigbee_zcl_attribute_list_ntv_item(struct bvm *vm);
extern int be_zigbee_zcl_attribute_list_ntv_remove(struct bvm *vm);
extern int be_zigbee_zcl_attribute_list_ntv_new_head(struct bvm *vm);
extern int be_zigbee_zcl_attribute_list_ntv_new_tail(struct bvm *vm);

#include "be_fixed_be_class_zcl_attribute_list_ntv.h"

/* @const_object_info_begin
class be_class_zcl_attribute_list_ntv (scope: global, name: zcl_attribute_list_ntv, super: be_class_ctypes_bytes, strings: weak) {
  _def, comptr(&be_zigbee_zcl_attribute_list_struct)
  _init, func(&be_zigbee_zcl_attribute_list_ntv_init)
  _deinit, func(&be_zigbee_zcl_attribute_list_ntv_deinit)

  size, func(&be_zigbee_zcl_attribute_list_ntv_size)
  item, func(&be_zigbee_zcl_attribute_list_ntv_item)
  new_head, func(&be_zigbee_zcl_attribute_list_ntv_new_head)
  new_tail, func(&be_zigbee_zcl_attribute_list_ntv_new_tail)
  remove, func(&be_zigbee_zcl_attribute_list_ntv_remove)
}
@const_object_info_end */

#endif // USE_ZIGBEE
