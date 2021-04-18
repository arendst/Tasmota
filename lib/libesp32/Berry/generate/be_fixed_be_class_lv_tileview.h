#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_tileview_map) {
    { be_const_key(get_tile_act, 2), be_const_func(lvbe_tileview_get_tile_act) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(init, -1), be_const_func(lvbe_tileview_create) },
    { be_const_key(add_element, 0), be_const_func(lvbe_tileview_add_element) },
    { be_const_key(get_edge_flash, -1), be_const_func(lvbe_tileview_get_edge_flash) },
    { be_const_key(set_valid_positions, 6), be_const_func(lvbe_tileview_set_valid_positions) },
    { be_const_key(create, -1), be_const_func(lvbe_tileview_create) },
    { be_const_key(set_tile_act, 9), be_const_func(lvbe_tileview_set_tile_act) },
    { be_const_key(set_edge_flash, -1), be_const_func(lvbe_tileview_set_edge_flash) },
    { be_const_key(dot_p, 10), be_const_int(0) },
    { be_const_key(get_anim_time, -1), be_const_func(lvbe_tileview_get_anim_time) },
    { be_const_key(set_anim_time, -1), be_const_func(lvbe_tileview_set_anim_time) },
};

static be_define_const_map(
    be_class_lv_tileview_map,
    12
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_tileview,
    1,
    (bclass *)&be_class_lv_obj,
    lv_tileview
);
