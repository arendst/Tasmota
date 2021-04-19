#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_chart_map) {
    { be_const_key(init, -1), be_const_func(lvbe_chart_create) },
    { be_const_key(set_secondary_y_tick_length, -1), be_const_func(lvbe_chart_set_secondary_y_tick_length) },
    { be_const_key(create, -1), be_const_func(lvbe_chart_create) },
    { be_const_key(set_x_tick_length, -1), be_const_func(lvbe_chart_set_x_tick_length) },
    { be_const_key(set_next, -1), be_const_func(lvbe_chart_set_next) },
    { be_const_key(set_x_start_point, 14), be_const_func(lvbe_chart_set_x_start_point) },
    { be_const_key(set_point_id, 23), be_const_func(lvbe_chart_set_point_id) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(set_ext_array, 12), be_const_func(lvbe_chart_set_ext_array) },
    { be_const_key(get_cursor_point, 4), be_const_func(lvbe_chart_get_cursor_point) },
    { be_const_key(hide_series, -1), be_const_func(lvbe_chart_hide_series) },
    { be_const_key(refresh, -1), be_const_func(lvbe_chart_refresh) },
    { be_const_key(set_point_count, -1), be_const_func(lvbe_chart_set_point_count) },
    { be_const_key(set_secondary_y_tick_texts, -1), be_const_func(lvbe_chart_set_secondary_y_tick_texts) },
    { be_const_key(set_points, 27), be_const_func(lvbe_chart_set_points) },
    { be_const_key(set_update_mode, -1), be_const_func(lvbe_chart_set_update_mode) },
    { be_const_key(set_y_tick_texts, 1), be_const_func(lvbe_chart_set_y_tick_texts) },
    { be_const_key(set_cursor_point, -1), be_const_func(lvbe_chart_set_cursor_point) },
    { be_const_key(get_point_count, -1), be_const_func(lvbe_chart_get_point_count) },
    { be_const_key(init_points, 20), be_const_func(lvbe_chart_init_points) },
    { be_const_key(remove_series, -1), be_const_func(lvbe_chart_remove_series) },
    { be_const_key(set_series_axis, 8), be_const_func(lvbe_chart_set_series_axis) },
    { be_const_key(set_type, -1), be_const_func(lvbe_chart_set_type) },
    { be_const_key(clear_series, -1), be_const_func(lvbe_chart_clear_series) },
    { be_const_key(set_div_line_count, -1), be_const_func(lvbe_chart_set_div_line_count) },
    { be_const_key(get_point_id, 31), be_const_func(lvbe_chart_get_point_id) },
    { be_const_key(tostring, 17), be_const_func(lvx_tostring) },
    { be_const_key(set_y_range, -1), be_const_func(lvbe_chart_set_y_range) },
    { be_const_key(get_type, -1), be_const_func(lvbe_chart_get_type) },
    { be_const_key(get_series_axis, 30), be_const_func(lvbe_chart_get_series_axis) },
    { be_const_key(set_y_tick_length, -1), be_const_func(lvbe_chart_set_y_tick_length) },
    { be_const_key(get_nearest_index_from_coord, -1), be_const_func(lvbe_chart_get_nearest_index_from_coord) },
    { be_const_key(set_x_tick_texts, -1), be_const_func(lvbe_chart_set_x_tick_texts) },
    { be_const_key(get_x_from_index, -1), be_const_func(lvbe_chart_get_x_from_index) },
    { be_const_key(get_x_start_point, -1), be_const_func(lvbe_chart_get_x_start_point) },
    { be_const_key(get_series_area, 33), be_const_func(lvbe_chart_get_series_area) },
    { be_const_key(get_y_from_index, -1), be_const_func(lvbe_chart_get_y_from_index) },
};

static be_define_const_map(
    be_class_lv_chart_map,
    37
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_chart,
    1,
    (bclass *)&be_class_lv_obj,
    lv_chart
);
