#include "be_constobj.h"

static be_define_const_map_slots(be_class_zcl_attribute_list_ntv_map) {
    { be_const_key_weak(new_tail, -1), be_const_func(&be_zigbee_zcl_attribute_list_ntv_new_tail) },
    { be_const_key_weak(new_head, 0), be_const_func(&be_zigbee_zcl_attribute_list_ntv_new_head) },
    { be_const_key_weak(_def, -1), be_const_comptr(&be_zigbee_zcl_attribute_list_struct) },
    { be_const_key_weak(_init, 1), be_const_func(&be_zigbee_zcl_attribute_list_ntv_init) },
    { be_const_key_weak(size, 3), be_const_func(&be_zigbee_zcl_attribute_list_ntv_size) },
    { be_const_key_weak(remove, 2), be_const_func(&be_zigbee_zcl_attribute_list_ntv_remove) },
    { be_const_key_weak(item, -1), be_const_func(&be_zigbee_zcl_attribute_list_ntv_item) },
    { be_const_key_weak(_deinit, -1), be_const_func(&be_zigbee_zcl_attribute_list_ntv_deinit) },
};

static be_define_const_map(
    be_class_zcl_attribute_list_ntv_map,
    8
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_zcl_attribute_list_ntv,
    0,
    (bclass *)&be_class_ctypes_bytes,
    zcl_attribute_list_ntv
);
