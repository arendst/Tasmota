#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_group_map) {
    { be_const_key(get_focus_cb, 17), be_const_func(lvbe_group_get_focus_cb) },
    { be_const_key(init, 7), be_const_func(lvbe_group_create) },
    { be_const_key(focus_next, -1), be_const_func(lvbe_group_focus_next) },
    { be_const_key(focus_freeze, -1), be_const_func(lvbe_group_focus_freeze) },
    { be_const_key(set_click_focus, -1), be_const_func(lvbe_group_set_click_focus) },
    { be_const_key(get_wrap, 8), be_const_func(lvbe_group_get_wrap) },
    { be_const_key(dot_p, -1), be_const_int(0) },
    { be_const_key(get_editing, 11), be_const_func(lvbe_group_get_editing) },
    { be_const_key(get_focused, -1), be_const_func(lvbe_group_get_focused) },
    { be_const_key(tostring, -1), be_const_func(lvx_tostring) },
    { be_const_key(create, 2), be_const_func(lvbe_group_create) },
    { be_const_key(set_refocus_policy, -1), be_const_func(lvbe_group_set_refocus_policy) },
    { be_const_key(send_data, 21), be_const_func(lvbe_group_send_data) },
    { be_const_key(add_obj, 22), be_const_func(lvbe_group_add_obj) },
    { be_const_key(set_focus_cb, 1), be_const_func(lvbe_group_set_focus_cb) },
    { be_const_key(set_editing, -1), be_const_func(lvbe_group_set_editing) },
    { be_const_key(set_wrap, 0), be_const_func(lvbe_group_set_wrap) },
    { be_const_key(remove_obj, -1), be_const_func(lvbe_group_remove_obj) },
    { be_const_key(focus_obj, -1), be_const_func(lvbe_group_focus_obj) },
    { be_const_key(remove_all_objs, 12), be_const_func(lvbe_group_remove_all_objs) },
    { be_const_key(focus_prev, -1), be_const_func(lvbe_group_focus_prev) },
    { be_const_key(get_click_focus, -1), be_const_func(lvbe_group_get_click_focus) },
    { be_const_key(del, -1), be_const_func(lvbe_group_del) },
};

static be_define_const_map(
    be_class_lv_group_map,
    23
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_group,
    1,
    NULL,
    lv_group
);
