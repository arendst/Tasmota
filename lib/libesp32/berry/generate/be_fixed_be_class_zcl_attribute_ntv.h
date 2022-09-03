#include "be_constobj.h"

static be_define_const_map_slots(be_class_zcl_attribute_ntv_map) {
    { be_const_key_weak(_def, -1), be_const_comptr(&be_zigbee_zcl_attribute_struct) },
    { be_const_key_weak(_get_val, -1), be_const_func(be_zigbee_zcl_attribute_ntv_get_val) },
    { be_const_key_weak(_set_key, 0), be_const_func(be_zigbee_zcl_attribute_ntv_set_key) },
    { be_const_key_weak(_deinit, -1), be_const_func(&be_zigbee_zcl_attribute_ntv_deinit) },
    { be_const_key_weak(_set_val, -1), be_const_func(be_zigbee_zcl_attribute_ntv_set_val) },
    { be_const_key_weak(_get_key, -1), be_const_func(be_zigbee_zcl_attribute_ntv_get_key) },
    { be_const_key_weak(_init, 1), be_const_func(&be_zigbee_zcl_attribute_ntv_init) },
};

static be_define_const_map(
    be_class_zcl_attribute_ntv_map,
    7
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_zcl_attribute_ntv,
    0,
    (bclass *)&be_class_ctypes_bytes,
    zcl_attribute_ntv
);
