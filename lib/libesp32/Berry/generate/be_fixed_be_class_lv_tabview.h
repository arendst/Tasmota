#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_tabview_map) {
    { be_const_key(init, -1), be_const_func(lvbe_tabview_create) },
    { be_const_key(get_anim_time, 13), be_const_func(lvbe_tabview_get_anim_time) },
    { be_const_key(set_anim_time, 8), be_const_func(lvbe_tabview_set_anim_time) },
    { be_const_key(add_tab, 6), be_const_func(lvbe_tabview_add_tab) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(set_btns_pos, 12), be_const_func(lvbe_tabview_set_btns_pos) },
    { be_const_key(get_tab_count, -1), be_const_func(lvbe_tabview_get_tab_count) },
    { be_const_key(get_tab_act, 11), be_const_func(lvbe_tabview_get_tab_act) },
    { be_const_key(create, -1), be_const_func(lvbe_tabview_create) },
    { be_const_key(get_btns_pos, -1), be_const_func(lvbe_tabview_get_btns_pos) },
    { be_const_key(set_tab_act, 1), be_const_func(lvbe_tabview_set_tab_act) },
    { be_const_key(clean_tab, -1), be_const_func(lvbe_tabview_clean_tab) },
    { be_const_key(get_tab, -1), be_const_func(lvbe_tabview_get_tab) },
    { be_const_key(tostring, 14), be_const_func(lvx_tostring) },
    { be_const_key(set_tab_name, -1), be_const_func(lvbe_tabview_set_tab_name) },
};

static be_define_const_map(
    be_class_lv_tabview_map,
    15
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_tabview,
    1,
    (bclass *)&be_class_lv_obj,
    lv_tabview
);
