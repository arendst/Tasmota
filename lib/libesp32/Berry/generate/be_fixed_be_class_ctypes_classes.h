#include "be_constobj.h"

static be_define_const_map_slots(be_class_ctypes_classes_map) {
    { be_const_key(lv_draw_mask_radius_param_cfg, -1), be_const_int(0) },
    { be_const_key(lv_draw_img_dsc, 14), be_const_int(0) },
    { be_const_key(lv_draw_mask_radius_param, -1), be_const_int(0) },
    { be_const_key(lv_point, 4), be_const_int(0) },
    { be_const_key(lv_draw_mask_fade_param_cfg, 5), be_const_int(0) },
    { be_const_key(lv_draw_mask_map_param, -1), be_const_int(0) },
    { be_const_key(lv_draw_label_dsc, -1), be_const_int(0) },
    { be_const_key(lv_area, 17), be_const_int(0) },
    { be_const_key(lv_draw_mask_angle_param_cfg, 3), be_const_int(0) },
    { be_const_key(lv_draw_mask_line_param_cfg, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_saved, 12), be_const_int(0) },
    { be_const_key(lv_draw_mask_map_param_cfg, -1), be_const_int(0) },
    { be_const_key(lv_draw_rect_dsc, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_angle_param, -1), be_const_int(0) },
    { be_const_key(lv_draw_line_dsc, 15), be_const_int(0) },
    { be_const_key(lv_draw_mask_common_dsc, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_line_param, -1), be_const_int(0) },
    { be_const_key(lv_sqrt_res, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_fade_param, 13), be_const_int(0) },
};

static be_define_const_map(
    be_class_ctypes_classes_map,
    19
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_ctypes_classes,
    0,
    NULL,
    be_class_ctypes_classes
);
