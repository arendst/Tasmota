#include "be_constobj.h"

static be_define_const_map_slots(be_class_ctypes_classes_map) {
    { be_const_key(lv_draw_mask_line_param_cfg_t, 1), be_const_int(0) },
    { be_const_key(lv_draw_mask_radius_param_t, 8), be_const_int(0) },
    { be_const_key(lv_draw_mask_radius_param_cfg_t, 12), be_const_int(0) },
    { be_const_key(lv_draw_mask_saved_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_line_param_t, 9), be_const_int(0) },
    { be_const_key(lv_area_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_map_param_cfg_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_map_param_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_line_dsc_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_label_dsc_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_common_dsc_t, 2), be_const_int(0) },
    { be_const_key(lv_draw_rect_dsc_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_fade_param_cfg_t, -1), be_const_int(0) },
    { be_const_key(lv_sqrt_res_t, 7), be_const_int(0) },
    { be_const_key(lv_point_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_angle_param_cfg_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_fade_param_t, -1), be_const_int(0) },
    { be_const_key(lv_draw_mask_angle_param_t, 15), be_const_int(0) },
    { be_const_key(lv_draw_img_dsc_t, -1), be_const_int(0) },
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
