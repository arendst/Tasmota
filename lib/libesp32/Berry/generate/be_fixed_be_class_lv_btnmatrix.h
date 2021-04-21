#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_btnmatrix_map) {
    { be_const_key(clear_btn_ctrl, 13), be_const_func(lvbe_btnmatrix_clear_btn_ctrl) },
    { be_const_key(set_align, -1), be_const_func(lvbe_btnmatrix_set_align) },
    { be_const_key(get_btn_text, 7), be_const_func(lvbe_btnmatrix_get_btn_text) },
    { be_const_key(get_active_btn_text, -1), be_const_func(lvbe_btnmatrix_get_active_btn_text) },
    { be_const_key(create, 3), be_const_func(lvbe_btnmatrix_create) },
    { be_const_key(set_btn_ctrl_all, -1), be_const_func(lvbe_btnmatrix_set_btn_ctrl_all) },
    { be_const_key(clear_btn_ctrl_all, 12), be_const_func(lvbe_btnmatrix_clear_btn_ctrl_all) },
    { be_const_key(get_one_check, -1), be_const_func(lvbe_btnmatrix_get_one_check) },
    { be_const_key(get_active_btn, -1), be_const_func(lvbe_btnmatrix_get_active_btn) },
    { be_const_key(get_align, 22), be_const_func(lvbe_btnmatrix_get_align) },
    { be_const_key(set_focused_btn, 4), be_const_func(lvbe_btnmatrix_set_focused_btn) },
    { be_const_key(set_map, -1), be_const_func(lvbe_btnmatrix_set_map) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(set_one_check, 20), be_const_func(lvbe_btnmatrix_set_one_check) },
    { be_const_key(init, -1), be_const_func(lvbe_btnmatrix_create) },
    { be_const_key(get_recolor, -1), be_const_func(lvbe_btnmatrix_get_recolor) },
    { be_const_key(set_recolor, 8), be_const_func(lvbe_btnmatrix_set_recolor) },
    { be_const_key(set_btn_ctrl, -1), be_const_func(lvbe_btnmatrix_set_btn_ctrl) },
    { be_const_key(get_focused_btn, -1), be_const_func(lvbe_btnmatrix_get_focused_btn) },
    { be_const_key(set_btn_width, 0), be_const_func(lvbe_btnmatrix_set_btn_width) },
    { be_const_key(get_btn_ctrl, -1), be_const_func(lvbe_btnmatrix_get_btn_ctrl) },
    { be_const_key(set_ctrl_map, 5), be_const_func(lvbe_btnmatrix_set_ctrl_map) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
};

static be_define_const_map(
    be_class_lv_btnmatrix_map,
    23
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_btnmatrix,
    1,
    (bclass *)&be_class_lv_obj,
    lv_btnmatrix
);
