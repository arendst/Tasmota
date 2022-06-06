#include "be_constobj.h"

static be_define_const_map_slots(be_class_zcl_frame_ntv_map) {
    { be_const_key_weak(_set_bytes, -1), be_const_ctype_func(zfn_set_bytes) },
    { be_const_key_weak(_def, 2), be_const_comptr(&be_zigbee_zcl_frame_struct) },
    { be_const_key_weak(_get_bytes, -1), be_const_ctype_func(zfn_get_bytes) },
};

static be_define_const_map(
    be_class_zcl_frame_ntv_map,
    3
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_zcl_frame_ntv,
    0,
    (bclass *)&be_class_ctypes_bytes,
    zcl_frame_ntv
);
