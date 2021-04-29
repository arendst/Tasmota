#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_table_map) {
    { be_const_key(init, -1), be_const_func(lvbe_table_create) },
    { be_const_key(set_cell_crop, -1), be_const_func(lvbe_table_set_cell_crop) },
    { be_const_key(set_cell_value, -1), be_const_func(lvbe_table_set_cell_value) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(set_cell_merge_right, -1), be_const_func(lvbe_table_set_cell_merge_right) },
    { be_const_key(set_cell_type, -1), be_const_func(lvbe_table_set_cell_type) },
    { be_const_key(get_col_width, 15), be_const_func(lvbe_table_get_col_width) },
    { be_const_key(get_row_cnt, 12), be_const_func(lvbe_table_get_row_cnt) },
    { be_const_key(set_row_cnt, -1), be_const_func(lvbe_table_set_row_cnt) },
    { be_const_key(get_cell_crop, 0), be_const_func(lvbe_table_get_cell_crop) },
    { be_const_key(get_cell_align, 14), be_const_func(lvbe_table_get_cell_align) },
    { be_const_key(create, -1), be_const_func(lvbe_table_create) },
    { be_const_key(get_cell_merge_right, -1), be_const_func(lvbe_table_get_cell_merge_right) },
    { be_const_key(set_col_width, -1), be_const_func(lvbe_table_set_col_width) },
    { be_const_key(set_cell_align, -1), be_const_func(lvbe_table_set_cell_align) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(set_cell_value_fmt, 10), be_const_func(lvbe_table_set_cell_value_fmt) },
    { be_const_key(set_col_cnt, -1), be_const_func(lvbe_table_set_col_cnt) },
    { be_const_key(get_col_cnt, 17), be_const_func(lvbe_table_get_col_cnt) },
    { be_const_key(get_cell_type, 6), be_const_func(lvbe_table_get_cell_type) },
    { be_const_key(get_cell_value, 7), be_const_func(lvbe_table_get_cell_value) },
    { be_const_key(get_pressed_cell, 13), be_const_func(lvbe_table_get_pressed_cell) },
};

static be_define_const_map(
    be_class_lv_table_map,
    22
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_table,
    1,
    (bclass *)&be_class_lv_obj,
    lv_table
);
