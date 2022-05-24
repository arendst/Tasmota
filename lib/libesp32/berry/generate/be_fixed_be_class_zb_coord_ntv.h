#include "be_constobj.h"

static be_define_const_map_slots(be_class_zb_coord_ntv_map) {
    { be_const_key(iter, -1), be_const_func(zc_iter) },
    { be_const_key(zcl_frame, -1), be_const_class(be_class_zcl_frame) },
    { be_const_key(zb_device, 3), be_const_class(be_class_zb_device) },
    { be_const_key(info, -1), be_const_func(zc_info) },
    { be_const_key(size, 1), be_const_ctype_func(zc_size) },
    { be_const_key(item, -1), be_const_func(zc_item) },
    { be_const_key(abort, 4), be_const_ctype_func(zc_abort) },
};

static be_define_const_map(
    be_class_zb_coord_ntv_map,
    7
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_zb_coord_ntv,
    0,
    NULL,
    zb_coord_ntv
);
