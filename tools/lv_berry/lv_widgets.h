/*
  Callback types:

lv_group_focus_cb_t
lv_signal_cb_t
lv_group_style_mod_cb_t
lv_design_cb_t
lv_event_cb_t

typedef void (*lv_group_focus_cb_t)(struct _lv_group_t *);
typedef lv_res_t (*lv_signal_cb_t)(struct _lv_obj_t * obj, lv_signal_t sign, void * param);
typedef void (*lv_group_style_mod_cb_t)(struct _lv_group_t *, lv_style_t *);
typedef lv_design_res_t (*lv_design_cb_t)(struct _lv_obj_t * obj, const lv_area_t * clip_area, lv_design_mode_t mode);
typedef void (*lv_event_cb_t)(struct _lv_obj_t * obj, lv_event_t event);


General form of callback
typedef uint32_t (*lvbe_callback)(struct _lv_obj_t * obj, uint32_t v1, uint32_t v2);
 */


// Custome Tasmota code
void lv_img_set_tasmota_logo(lv_obj_t * img);

// ======================================================================
// Style
// ======================================================================
void lv_style_init(lv_style_t * style);
void lv_style_copy(lv_style_t * style_dest, const lv_style_t * style_src);
// void lv_style_copy(lv_style_t * dest, const lv_style_t * src);
void lv_style_list_init(lv_style_list_t * list);
void lv_style_list_copy(lv_style_list_t * list_dest, const lv_style_list_t * list_src);
static inline lv_style_t * lv_style_list_get_style(lv_style_list_t * list, uint8_t id)
void lv_style_reset(lv_style_t * style);
bool lv_style_remove_prop(lv_style_t * style, lv_style_property_t prop);
lv_style_t * lv_style_list_get_local_style(lv_style_list_t * list);
// bool lv_debug_check_style(const lv_style_t * style);
// bool lv_debug_check_style_list(const lv_style_list_t * list);

// ======================================================================
// Group
// ======================================================================
// LV Group
lv_group_t * lv_group_create(void);
void lv_group_del(lv_group_t * group);
void lv_group_add_obj(lv_group_t * group, lv_obj_t * obj);
void lv_group_remove_obj(lv_obj_t * obj);
void lv_group_remove_all_objs(lv_group_t * group);
void lv_group_focus_obj(lv_obj_t * obj);
void lv_group_focus_next(lv_group_t * group);
void lv_group_focus_prev(lv_group_t * group);
void lv_group_focus_freeze(lv_group_t * group, bool en);
lv_res_t lv_group_send_data(lv_group_t * group, uint32_t c);
void lv_group_set_focus_cb(lv_group_t * group, lv_group_focus_cb_t focus_cb);
void lv_group_set_refocus_policy(lv_group_t * group, lv_group_refocus_policy_t policy);
void lv_group_set_editing(lv_group_t * group, bool edit);
void lv_group_set_click_focus(lv_group_t * group, bool en);
void lv_group_set_wrap(lv_group_t * group, bool en);
lv_obj_t * lv_group_get_focused(const lv_group_t * group);
lv_group_user_data_t * lv_group_get_user_data(lv_group_t * group);
lv_group_focus_cb_t lv_group_get_focus_cb(const lv_group_t * group);
bool lv_group_get_editing(const lv_group_t * group);
bool lv_group_get_click_focus(const lv_group_t * group);
bool lv_group_get_wrap(lv_group_t * group);

// ======================================================================
// Indev - Input devicce
// ======================================================================
// void _lv_indev_read_task(lv_task_t * task);
lv_indev_t * lv_indev_get_act(void);
lv_indev_type_t lv_indev_get_type(const lv_indev_t * indev);
// void lv_indev_reset(lv_indev_t * indev, lv_obj_t * obj);
// void lv_indev_reset_long_press(lv_indev_t * indev);
void lv_indev_enable(lv_indev_t * indev, bool en);
// void lv_indev_set_cursor(lv_indev_t * indev, lv_obj_t * cur_obj);
void lv_indev_set_group(lv_indev_t * indev, lv_group_t * group);
// void lv_indev_set_button_points(lv_indev_t * indev, const lv_point_t points[]);
// void lv_indev_get_point(const lv_indev_t * indev, lv_point_t * point);
// lv_gesture_dir_t lv_indev_get_gesture_dir(const lv_indev_t * indev);
// uint32_t lv_indev_get_key(const lv_indev_t * indev);
// bool lv_indev_is_dragging(const lv_indev_t * indev);
// void lv_indev_get_vect(const lv_indev_t * indev, lv_point_t * point);
// lv_res_t lv_indev_finish_drag(lv_indev_t * indev);
// void lv_indev_wait_release(lv_indev_t * indev);
lv_obj_t * lv_indev_get_obj_act(void);
lv_obj_t * lv_indev_search_obj(lv_obj_t * obj, lv_point_t * point);
// lv_task_t * lv_indev_get_read_task(lv_disp_t * indev);

// ======================================================================
// Object
// ======================================================================
// LV Object
lv_obj_t * lv_obj_create(lv_obj_t * parent, const lv_obj_t * copy);
lv_res_t lv_obj_del(lv_obj_t * obj);
void lv_obj_del_anim_ready_cb(lv_anim_t * a);
void lv_obj_del_async(struct _lv_obj_t * obj);
void lv_obj_clean(lv_obj_t * obj);
void lv_obj_invalidate_area(const lv_obj_t * obj, const lv_area_t * area);
void lv_obj_invalidate(const lv_obj_t * obj);
bool lv_obj_area_is_visible(const lv_obj_t * obj, lv_area_t * area);
bool lv_obj_is_visible(const lv_obj_t * obj);
void lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent);
void lv_obj_move_foreground(lv_obj_t * obj);
void lv_obj_move_background(lv_obj_t * obj);
void lv_obj_set_pos(lv_obj_t * obj, lv_coord_t x, lv_coord_t y);
void lv_obj_set_x(lv_obj_t * obj, lv_coord_t x);
void lv_obj_set_y(lv_obj_t * obj, lv_coord_t y);
void lv_obj_set_size(lv_obj_t * obj, lv_coord_t w, lv_coord_t h);
void lv_obj_set_width(lv_obj_t * obj, lv_coord_t w);
void lv_obj_set_height(lv_obj_t * obj, lv_coord_t h);
void lv_obj_set_width_fit(lv_obj_t * obj, lv_coord_t w);
void lv_obj_set_height_fit(lv_obj_t * obj, lv_coord_t h);
void lv_obj_set_width_margin(lv_obj_t * obj, lv_coord_t w);
void lv_obj_set_height_margin(lv_obj_t * obj, lv_coord_t h);
void lv_obj_align(lv_obj_t * obj, const lv_obj_t * base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
void lv_obj_align_x(lv_obj_t * obj, const lv_obj_t * base, lv_align_t align, lv_coord_t x_ofs);
void lv_obj_align_y(lv_obj_t * obj, const lv_obj_t * base, lv_align_t align, lv_coord_t y_ofs);
void lv_obj_align_mid(lv_obj_t * obj, const lv_obj_t * base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);
void lv_obj_align_mid_x(lv_obj_t * obj, const lv_obj_t * base, lv_align_t align, lv_coord_t x_ofs);
void lv_obj_align_mid_y(lv_obj_t * obj, const lv_obj_t * base, lv_align_t align, lv_coord_t y_ofs);
void lv_obj_realign(lv_obj_t * obj);
void lv_obj_set_auto_realign(lv_obj_t * obj, bool en);
void lv_obj_set_ext_click_area(lv_obj_t * obj, lv_coord_t left, lv_coord_t right, lv_coord_t top, lv_coord_t bottom);
void lv_obj_add_style(lv_obj_t * obj, uint8_t part, lv_style_t * style);
void lv_obj_remove_style(lv_obj_t * obj, uint8_t part, lv_style_t * style);
void lv_obj_clean_style_list(lv_obj_t * obj, uint8_t part);
void lv_obj_reset_style_list(lv_obj_t * obj, uint8_t part);
void lv_obj_refresh_style(lv_obj_t * obj, uint8_t part, lv_style_property_t prop);
void lv_obj_report_style_mod(lv_style_t * style);
void _lv_obj_set_style_local_color(lv_obj_t * obj, uint8_t type, lv_style_property_t prop, lv_color_t color);
void _lv_obj_set_style_local_int(lv_obj_t * obj, uint8_t type, lv_style_property_t prop, lv_style_int_t value);
void _lv_obj_set_style_local_opa(lv_obj_t * obj, uint8_t type, lv_style_property_t prop, lv_opa_t opa);
void _lv_obj_set_style_local_ptr(lv_obj_t * obj, uint8_t type, lv_style_property_t prop, const void * value);
bool lv_obj_remove_style_local_prop(lv_obj_t * obj, uint8_t part, lv_style_property_t prop);
void _lv_obj_disable_style_caching(lv_obj_t * obj, bool dis);
void lv_obj_set_hidden(lv_obj_t * obj, bool en);
void lv_obj_set_adv_hittest(lv_obj_t * obj, bool en);
void lv_obj_set_click(lv_obj_t * obj, bool en);
void lv_obj_set_top(lv_obj_t * obj, bool en);
void lv_obj_set_drag(lv_obj_t * obj, bool en);
void lv_obj_set_drag_dir(lv_obj_t * obj, lv_drag_dir_t drag_dir);
void lv_obj_set_drag_throw(lv_obj_t * obj, bool en);
void lv_obj_set_drag_parent(lv_obj_t * obj, bool en);
void lv_obj_set_focus_parent(lv_obj_t * obj, bool en);
void lv_obj_set_gesture_parent(lv_obj_t * obj, bool en);
void lv_obj_set_parent_event(lv_obj_t * obj, bool en);
void lv_obj_set_base_dir(lv_obj_t * obj, lv_bidi_dir_t dir);
void lv_obj_add_protect(lv_obj_t * obj, uint8_t prot);
void lv_obj_clear_protect(lv_obj_t * obj, uint8_t prot);
void lv_obj_set_state(lv_obj_t * obj, lv_state_t state);
void lv_obj_add_state(lv_obj_t * obj, lv_state_t state);
void lv_obj_clear_state(lv_obj_t * obj, lv_state_t state);
void lv_obj_finish_transitions(lv_obj_t * obj, uint8_t part);
void lv_obj_set_event_cb(lv_obj_t * obj, lv_event_cb_t event_cb);
lv_res_t lv_event_send(lv_obj_t * obj, lv_event_t event, const void * data);
lv_res_t lv_event_send_refresh(lv_obj_t * obj);
void lv_event_send_refresh_recursive(lv_obj_t * obj);
// lv_res_t lv_event_send_func(lv_event_cb_t event_xcb, lv_obj_t * obj, lv_event_t event, const void * data);
const void * lv_event_get_data(void);
void lv_obj_set_signal_cb(lv_obj_t * obj, lv_signal_cb_t signal_cb);
lv_res_t lv_signal_send(lv_obj_t * obj, lv_signal_t signal, void * param);
void lv_obj_set_design_cb(lv_obj_t * obj, lv_design_cb_t design_cb);
void * lv_obj_allocate_ext_attr(lv_obj_t * obj, uint16_t ext_size);
void lv_obj_refresh_ext_draw_pad(lv_obj_t * obj);
lv_obj_t * lv_obj_get_screen(const lv_obj_t * obj);
lv_disp_t * lv_obj_get_disp(const lv_obj_t * obj);
lv_obj_t * lv_obj_get_parent(const lv_obj_t * obj);
lv_obj_t * lv_obj_get_child(const lv_obj_t * obj, const lv_obj_t * child);
lv_obj_t * lv_obj_get_child_back(const lv_obj_t * obj, const lv_obj_t * child);
uint16_t lv_obj_count_children(const lv_obj_t * obj);
uint16_t lv_obj_count_children_recursive(const lv_obj_t * obj);
void lv_obj_get_coords(const lv_obj_t * obj, lv_area_t * cords_p);
void lv_obj_get_inner_coords(const lv_obj_t * obj, lv_area_t * coords_p);
lv_coord_t lv_obj_get_x(const lv_obj_t * obj);
lv_coord_t lv_obj_get_y(const lv_obj_t * obj);
lv_coord_t lv_obj_get_width(const lv_obj_t * obj);
lv_coord_t lv_obj_get_height(const lv_obj_t * obj);
lv_coord_t lv_obj_get_width_fit(const lv_obj_t * obj);
lv_coord_t lv_obj_get_height_fit(const lv_obj_t * obj);
lv_coord_t lv_obj_get_height_margin(lv_obj_t * obj);
lv_coord_t lv_obj_get_width_margin(lv_obj_t * obj);
lv_coord_t lv_obj_get_width_grid(lv_obj_t * obj, uint8_t div, uint8_t span);
lv_coord_t lv_obj_get_height_grid(lv_obj_t * obj, uint8_t div, uint8_t span);
bool lv_obj_get_auto_realign(const lv_obj_t * obj);
lv_coord_t lv_obj_get_ext_click_pad_left(const lv_obj_t * obj);
lv_coord_t lv_obj_get_ext_click_pad_right(const lv_obj_t * obj);
lv_coord_t lv_obj_get_ext_click_pad_top(const lv_obj_t * obj);
lv_coord_t lv_obj_get_ext_click_pad_bottom(const lv_obj_t * obj);
lv_coord_t lv_obj_get_ext_draw_pad(const lv_obj_t * obj);
lv_style_list_t * lv_obj_get_style_list(const lv_obj_t * obj, uint8_t part);
lv_style_int_t _lv_obj_get_style_int(const lv_obj_t * obj, uint8_t part, lv_style_property_t prop);
lv_color_t _lv_obj_get_style_color(const lv_obj_t * obj, uint8_t part, lv_style_property_t prop);
lv_opa_t _lv_obj_get_style_opa(const lv_obj_t * obj, uint8_t part, lv_style_property_t prop);
const void * _lv_obj_get_style_ptr(const lv_obj_t * obj, uint8_t part, lv_style_property_t prop);
lv_style_t * lv_obj_get_local_style(lv_obj_t * obj, uint8_t part);
//#include "lv_obj_style_dec.h"
bool lv_obj_get_hidden(const lv_obj_t * obj);
bool lv_obj_get_adv_hittest(const lv_obj_t * obj);
bool lv_obj_get_click(const lv_obj_t * obj);
bool lv_obj_get_top(const lv_obj_t * obj);
bool lv_obj_get_drag(const lv_obj_t * obj);
lv_drag_dir_t lv_obj_get_drag_dir(const lv_obj_t * obj);
bool lv_obj_get_drag_throw(const lv_obj_t * obj);
bool lv_obj_get_drag_parent(const lv_obj_t * obj);
bool lv_obj_get_focus_parent(const lv_obj_t * obj);
bool lv_obj_get_parent_event(const lv_obj_t * obj);
bool lv_obj_get_gesture_parent(const lv_obj_t * obj);
lv_bidi_dir_t lv_obj_get_base_dir(const lv_obj_t * obj);
uint8_t lv_obj_get_protect(const lv_obj_t * obj);
bool lv_obj_is_protected(const lv_obj_t * obj, uint8_t prot);
lv_state_t lv_obj_get_state(const lv_obj_t * obj, uint8_t part);
lv_signal_cb_t lv_obj_get_signal_cb(const lv_obj_t * obj);
lv_design_cb_t lv_obj_get_design_cb(const lv_obj_t * obj);
lv_event_cb_t lv_obj_get_event_cb(const lv_obj_t * obj);
bool lv_obj_is_point_on_coords(lv_obj_t * obj, const lv_point_t * point);
bool lv_obj_hittest(lv_obj_t * obj, lv_point_t * point);
void * lv_obj_get_ext_attr(const lv_obj_t * obj);
void lv_obj_get_type(const lv_obj_t * obj, lv_obj_type_t * buf);
lv_obj_user_data_t lv_obj_get_user_data(const lv_obj_t * obj);
lv_obj_user_data_t * lv_obj_get_user_data_ptr(const lv_obj_t * obj);
void lv_obj_set_user_data(lv_obj_t * obj, lv_obj_user_data_t data);
void * lv_obj_get_group(const lv_obj_t * obj);
bool lv_obj_is_focused(const lv_obj_t * obj);
lv_obj_t * lv_obj_get_focused_obj(const lv_obj_t * obj);
lv_res_t lv_obj_handle_get_type_signal(lv_obj_type_t * buf, const char * name);
void lv_obj_init_draw_rect_dsc(lv_obj_t * obj, uint8_t type, lv_draw_rect_dsc_t * draw_dsc);
void lv_obj_init_draw_label_dsc(lv_obj_t * obj, uint8_t type, lv_draw_label_dsc_t * draw_dsc);
void lv_obj_init_draw_img_dsc(lv_obj_t * obj, uint8_t part, lv_draw_img_dsc_t * draw_dsc);
void lv_obj_init_draw_line_dsc(lv_obj_t * obj, uint8_t part, lv_draw_line_dsc_t * draw_dsc);
lv_coord_t lv_obj_get_draw_rect_ext_pad_size(lv_obj_t * obj, uint8_t part);
void lv_obj_fade_in(lv_obj_t * obj, uint32_t time, uint32_t delay);
void lv_obj_fade_out(lv_obj_t * obj, uint32_t time, uint32_t delay);
bool lv_debug_check_obj_type(const lv_obj_t * obj, const char * obj_type);
bool lv_debug_check_obj_valid(const lv_obj_t * obj);

// Generated style methods
static inline lv_style_int_t lv_obj_get_style_radius(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_RADIUS); }
static inline void lv_obj_set_style_local_radius(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_RADIUS | (state << 8), value); }
static inline void lv_style_set_radius(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_RADIUS | (state << 8), value); }
static inline bool lv_obj_get_style_clip_corner(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_CLIP_CORNER); }
static inline void lv_obj_set_style_local_clip_corner(lv_obj_t * obj, uint8_t part, lv_state_t state, bool value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_CLIP_CORNER | (state << 8), value); }
static inline void lv_style_set_clip_corner(lv_style_t * style, lv_state_t state, bool value) { _lv_style_set_int(style, LV_STYLE_CLIP_CORNER | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_size(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SIZE); }
static inline void lv_obj_set_style_local_size(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SIZE | (state << 8), value); }
static inline void lv_style_set_size(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_SIZE | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transform_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSFORM_WIDTH); }
static inline void lv_obj_set_style_local_transform_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSFORM_WIDTH | (state << 8), value); }
static inline void lv_style_set_transform_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSFORM_WIDTH | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transform_height(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSFORM_HEIGHT); }
static inline void lv_obj_set_style_local_transform_height(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSFORM_HEIGHT | (state << 8), value); }
static inline void lv_style_set_transform_height(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSFORM_HEIGHT | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transform_angle(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSFORM_ANGLE); }
static inline void lv_obj_set_style_local_transform_angle(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSFORM_ANGLE | (state << 8), value); }
static inline void lv_style_set_transform_angle(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSFORM_ANGLE | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transform_zoom(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSFORM_ZOOM); }
static inline void lv_obj_set_style_local_transform_zoom(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSFORM_ZOOM | (state << 8), value); }
static inline void lv_style_set_transform_zoom(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSFORM_ZOOM | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_opa_scale(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_OPA_SCALE); }
static inline void lv_obj_set_style_local_opa_scale(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_OPA_SCALE | (state << 8), value); }
static inline void lv_style_set_opa_scale(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_OPA_SCALE | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_pad_top(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_PAD_TOP); }
static inline void lv_obj_set_style_local_pad_top(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_PAD_TOP | (state << 8), value); }
static inline void lv_style_set_pad_top(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_PAD_TOP | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_pad_bottom(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_PAD_BOTTOM); }
static inline void lv_obj_set_style_local_pad_bottom(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_PAD_BOTTOM | (state << 8), value); }
static inline void lv_style_set_pad_bottom(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_PAD_BOTTOM | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_pad_left(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_PAD_LEFT); }
static inline void lv_obj_set_style_local_pad_left(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_PAD_LEFT | (state << 8), value); }
static inline void lv_style_set_pad_left(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_PAD_LEFT | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_pad_right(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_PAD_RIGHT); }
static inline void lv_obj_set_style_local_pad_right(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_PAD_RIGHT | (state << 8), value); }
static inline void lv_style_set_pad_right(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_PAD_RIGHT | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_pad_inner(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_PAD_INNER); }
static inline void lv_obj_set_style_local_pad_inner(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_PAD_INNER | (state << 8), value); }
static inline void lv_style_set_pad_inner(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_PAD_INNER | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_margin_top(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_MARGIN_TOP); }
static inline void lv_obj_set_style_local_margin_top(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_MARGIN_TOP | (state << 8), value); }
static inline void lv_style_set_margin_top(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_MARGIN_TOP | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_margin_bottom(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_MARGIN_BOTTOM); }
static inline void lv_obj_set_style_local_margin_bottom(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_MARGIN_BOTTOM | (state << 8), value); }
static inline void lv_style_set_margin_bottom(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_MARGIN_BOTTOM | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_margin_left(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_MARGIN_LEFT); }
static inline void lv_obj_set_style_local_margin_left(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_MARGIN_LEFT | (state << 8), value); }
static inline void lv_style_set_margin_left(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_MARGIN_LEFT | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_margin_right(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_MARGIN_RIGHT); }
static inline void lv_obj_set_style_local_margin_right(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_MARGIN_RIGHT | (state << 8), value); }
static inline void lv_style_set_margin_right(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_MARGIN_RIGHT | (state << 8), value); }
static inline lv_blend_mode_t lv_obj_get_style_bg_blend_mode(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_BG_BLEND_MODE); }
static inline void lv_obj_set_style_local_bg_blend_mode(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_blend_mode_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_BG_BLEND_MODE | (state << 8), value); }
static inline void lv_style_set_bg_blend_mode(lv_style_t * style, lv_state_t state, lv_blend_mode_t value) { _lv_style_set_int(style, LV_STYLE_BG_BLEND_MODE | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_bg_main_stop(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_BG_MAIN_STOP); }
static inline void lv_obj_set_style_local_bg_main_stop(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_BG_MAIN_STOP | (state << 8), value); }
static inline void lv_style_set_bg_main_stop(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_BG_MAIN_STOP | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_bg_grad_stop(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_BG_GRAD_STOP); }
static inline void lv_obj_set_style_local_bg_grad_stop(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_BG_GRAD_STOP | (state << 8), value); }
static inline void lv_style_set_bg_grad_stop(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_BG_GRAD_STOP | (state << 8), value); }
static inline lv_grad_dir_t lv_obj_get_style_bg_grad_dir(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_BG_GRAD_DIR); }
static inline void lv_obj_set_style_local_bg_grad_dir(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_grad_dir_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_BG_GRAD_DIR | (state << 8), value); }
static inline void lv_style_set_bg_grad_dir(lv_style_t * style, lv_state_t state, lv_grad_dir_t value) { _lv_style_set_int(style, LV_STYLE_BG_GRAD_DIR | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_bg_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_BG_COLOR); }
static inline void lv_obj_set_style_local_bg_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_BG_COLOR | (state << 8), value); }
static inline void lv_style_set_bg_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_BG_COLOR | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_bg_grad_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_BG_GRAD_COLOR); }
static inline void lv_obj_set_style_local_bg_grad_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_BG_GRAD_COLOR | (state << 8), value); }
static inline void lv_style_set_bg_grad_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_BG_GRAD_COLOR | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_bg_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_BG_OPA); }
static inline void lv_obj_set_style_local_bg_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_BG_OPA | (state << 8), value); }
static inline void lv_style_set_bg_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_BG_OPA | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_border_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_BORDER_WIDTH); }
static inline void lv_obj_set_style_local_border_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_BORDER_WIDTH | (state << 8), value); }
static inline void lv_style_set_border_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_BORDER_WIDTH | (state << 8), value); }
static inline lv_border_side_t lv_obj_get_style_border_side(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_BORDER_SIDE); }
static inline void lv_obj_set_style_local_border_side(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_border_side_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_BORDER_SIDE | (state << 8), value); }
static inline void lv_style_set_border_side(lv_style_t * style, lv_state_t state, lv_border_side_t value) { _lv_style_set_int(style, LV_STYLE_BORDER_SIDE | (state << 8), value); }
static inline lv_blend_mode_t lv_obj_get_style_border_blend_mode(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_BORDER_BLEND_MODE); }
static inline void lv_obj_set_style_local_border_blend_mode(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_blend_mode_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_BORDER_BLEND_MODE | (state << 8), value); }
static inline void lv_style_set_border_blend_mode(lv_style_t * style, lv_state_t state, lv_blend_mode_t value) { _lv_style_set_int(style, LV_STYLE_BORDER_BLEND_MODE | (state << 8), value); }
static inline bool lv_obj_get_style_border_post(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_BORDER_POST); }
static inline void lv_obj_set_style_local_border_post(lv_obj_t * obj, uint8_t part, lv_state_t state, bool value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_BORDER_POST | (state << 8), value); }
static inline void lv_style_set_border_post(lv_style_t * style, lv_state_t state, bool value) { _lv_style_set_int(style, LV_STYLE_BORDER_POST | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_border_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_BORDER_COLOR); }
static inline void lv_obj_set_style_local_border_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_BORDER_COLOR | (state << 8), value); }
static inline void lv_style_set_border_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_BORDER_COLOR | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_border_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_BORDER_OPA); }
static inline void lv_obj_set_style_local_border_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_BORDER_OPA | (state << 8), value); }
static inline void lv_style_set_border_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_BORDER_OPA | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_outline_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_OUTLINE_WIDTH); }
static inline void lv_obj_set_style_local_outline_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_OUTLINE_WIDTH | (state << 8), value); }
static inline void lv_style_set_outline_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_OUTLINE_WIDTH | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_outline_pad(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_OUTLINE_PAD); }
static inline void lv_obj_set_style_local_outline_pad(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_OUTLINE_PAD | (state << 8), value); }
static inline void lv_style_set_outline_pad(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_OUTLINE_PAD | (state << 8), value); }
static inline lv_blend_mode_t lv_obj_get_style_outline_blend_mode(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_OUTLINE_BLEND_MODE); }
static inline void lv_obj_set_style_local_outline_blend_mode(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_blend_mode_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_OUTLINE_BLEND_MODE | (state << 8), value); }
static inline void lv_style_set_outline_blend_mode(lv_style_t * style, lv_state_t state, lv_blend_mode_t value) { _lv_style_set_int(style, LV_STYLE_OUTLINE_BLEND_MODE | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_outline_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_OUTLINE_COLOR); }
static inline void lv_obj_set_style_local_outline_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_OUTLINE_COLOR | (state << 8), value); }
static inline void lv_style_set_outline_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_OUTLINE_COLOR | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_outline_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_OUTLINE_OPA); }
static inline void lv_obj_set_style_local_outline_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_OUTLINE_OPA | (state << 8), value); }
static inline void lv_style_set_outline_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_OUTLINE_OPA | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_shadow_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SHADOW_WIDTH); }
static inline void lv_obj_set_style_local_shadow_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SHADOW_WIDTH | (state << 8), value); }
static inline void lv_style_set_shadow_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_SHADOW_WIDTH | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_shadow_ofs_x(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SHADOW_OFS_X); }
static inline void lv_obj_set_style_local_shadow_ofs_x(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SHADOW_OFS_X | (state << 8), value); }
static inline void lv_style_set_shadow_ofs_x(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_SHADOW_OFS_X | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_shadow_ofs_y(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SHADOW_OFS_Y); }
static inline void lv_obj_set_style_local_shadow_ofs_y(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SHADOW_OFS_Y | (state << 8), value); }
static inline void lv_style_set_shadow_ofs_y(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_SHADOW_OFS_Y | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_shadow_spread(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SHADOW_SPREAD); }
static inline void lv_obj_set_style_local_shadow_spread(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SHADOW_SPREAD | (state << 8), value); }
static inline void lv_style_set_shadow_spread(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_SHADOW_SPREAD | (state << 8), value); }
static inline lv_blend_mode_t lv_obj_get_style_shadow_blend_mode(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SHADOW_BLEND_MODE); }
static inline void lv_obj_set_style_local_shadow_blend_mode(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_blend_mode_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SHADOW_BLEND_MODE | (state << 8), value); }
static inline void lv_style_set_shadow_blend_mode(lv_style_t * style, lv_state_t state, lv_blend_mode_t value) { _lv_style_set_int(style, LV_STYLE_SHADOW_BLEND_MODE | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_shadow_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_SHADOW_COLOR); }
static inline void lv_obj_set_style_local_shadow_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_SHADOW_COLOR | (state << 8), value); }
static inline void lv_style_set_shadow_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_SHADOW_COLOR | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_shadow_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_SHADOW_OPA); }
static inline void lv_obj_set_style_local_shadow_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_SHADOW_OPA | (state << 8), value); }
static inline void lv_style_set_shadow_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_SHADOW_OPA | (state << 8), value); }
static inline bool lv_obj_get_style_pattern_repeat(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_PATTERN_REPEAT); }
static inline void lv_obj_set_style_local_pattern_repeat(lv_obj_t * obj, uint8_t part, lv_state_t state, bool value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_PATTERN_REPEAT | (state << 8), value); }
static inline void lv_style_set_pattern_repeat(lv_style_t * style, lv_state_t state, bool value) { _lv_style_set_int(style, LV_STYLE_PATTERN_REPEAT | (state << 8), value); }
static inline lv_blend_mode_t lv_obj_get_style_pattern_blend_mode(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_PATTERN_BLEND_MODE); }
static inline void lv_obj_set_style_local_pattern_blend_mode(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_blend_mode_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_PATTERN_BLEND_MODE | (state << 8), value); }
static inline void lv_style_set_pattern_blend_mode(lv_style_t * style, lv_state_t state, lv_blend_mode_t value) { _lv_style_set_int(style, LV_STYLE_PATTERN_BLEND_MODE | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_pattern_recolor(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_PATTERN_RECOLOR); }
static inline void lv_obj_set_style_local_pattern_recolor(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_PATTERN_RECOLOR | (state << 8), value); }
static inline void lv_style_set_pattern_recolor(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_PATTERN_RECOLOR | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_pattern_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_PATTERN_OPA); }
static inline void lv_obj_set_style_local_pattern_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_PATTERN_OPA | (state << 8), value); }
static inline void lv_style_set_pattern_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_PATTERN_OPA | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_pattern_recolor_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_PATTERN_RECOLOR_OPA); }
static inline void lv_obj_set_style_local_pattern_recolor_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_PATTERN_RECOLOR_OPA | (state << 8), value); }
static inline void lv_style_set_pattern_recolor_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_PATTERN_RECOLOR_OPA | (state << 8), value); }
static inline const void * lv_obj_get_style_pattern_image(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_ptr(obj, part, LV_STYLE_PATTERN_IMAGE); }
static inline void lv_obj_set_style_local_pattern_image(lv_obj_t * obj, uint8_t part, lv_state_t state, const void * value) { _lv_obj_set_style_local_ptr(obj, part, LV_STYLE_PATTERN_IMAGE | (state << 8), value); }
static inline void lv_style_set_pattern_image(lv_style_t * style, lv_state_t state, const void * value) { _lv_style_set_ptr(style, LV_STYLE_PATTERN_IMAGE | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_value_letter_space(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_VALUE_LETTER_SPACE); }
static inline void lv_obj_set_style_local_value_letter_space(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_VALUE_LETTER_SPACE | (state << 8), value); }
static inline void lv_style_set_value_letter_space(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_VALUE_LETTER_SPACE | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_value_line_space(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_VALUE_LINE_SPACE); }
static inline void lv_obj_set_style_local_value_line_space(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_VALUE_LINE_SPACE | (state << 8), value); }
static inline void lv_style_set_value_line_space(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_VALUE_LINE_SPACE | (state << 8), value); }
static inline lv_blend_mode_t lv_obj_get_style_value_blend_mode(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_VALUE_BLEND_MODE); }
static inline void lv_obj_set_style_local_value_blend_mode(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_blend_mode_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_VALUE_BLEND_MODE | (state << 8), value); }
static inline void lv_style_set_value_blend_mode(lv_style_t * style, lv_state_t state, lv_blend_mode_t value) { _lv_style_set_int(style, LV_STYLE_VALUE_BLEND_MODE | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_value_ofs_x(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_VALUE_OFS_X); }
static inline void lv_obj_set_style_local_value_ofs_x(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_VALUE_OFS_X | (state << 8), value); }
static inline void lv_style_set_value_ofs_x(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_VALUE_OFS_X | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_value_ofs_y(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_VALUE_OFS_Y); }
static inline void lv_obj_set_style_local_value_ofs_y(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_VALUE_OFS_Y | (state << 8), value); }
static inline void lv_style_set_value_ofs_y(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_VALUE_OFS_Y | (state << 8), value); }
static inline lv_align_t lv_obj_get_style_value_align(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_VALUE_ALIGN); }
static inline void lv_obj_set_style_local_value_align(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_align_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_VALUE_ALIGN | (state << 8), value); }
static inline void lv_style_set_value_align(lv_style_t * style, lv_state_t state, lv_align_t value) { _lv_style_set_int(style, LV_STYLE_VALUE_ALIGN | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_value_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_VALUE_COLOR); }
static inline void lv_obj_set_style_local_value_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_VALUE_COLOR | (state << 8), value); }
static inline void lv_style_set_value_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_VALUE_COLOR | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_value_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_VALUE_OPA); }
static inline void lv_obj_set_style_local_value_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_VALUE_OPA | (state << 8), value); }
static inline void lv_style_set_value_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_VALUE_OPA | (state << 8), value); }
static inline const lv_font_t * lv_obj_get_style_value_font(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_ptr(obj, part, LV_STYLE_VALUE_FONT); }
static inline void lv_obj_set_style_local_value_font(lv_obj_t * obj, uint8_t part, lv_state_t state, const lv_font_t * value) { _lv_obj_set_style_local_ptr(obj, part, LV_STYLE_VALUE_FONT | (state << 8), value); }
static inline void lv_style_set_value_font(lv_style_t * style, lv_state_t state, const lv_font_t * value) { _lv_style_set_ptr(style, LV_STYLE_VALUE_FONT | (state << 8), value); }
static inline const char * lv_obj_get_style_value_str(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_ptr(obj, part, LV_STYLE_VALUE_STR); }
static inline void lv_obj_set_style_local_value_str(lv_obj_t * obj, uint8_t part, lv_state_t state, const char * value) { _lv_obj_set_style_local_ptr(obj, part, LV_STYLE_VALUE_STR | (state << 8), value); }
static inline void lv_style_set_value_str(lv_style_t * style, lv_state_t state, const char * value) { _lv_style_set_ptr(style, LV_STYLE_VALUE_STR | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_text_letter_space(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TEXT_LETTER_SPACE); }
static inline void lv_obj_set_style_local_text_letter_space(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TEXT_LETTER_SPACE | (state << 8), value); }
static inline void lv_style_set_text_letter_space(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TEXT_LETTER_SPACE | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_text_line_space(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TEXT_LINE_SPACE); }
static inline void lv_obj_set_style_local_text_line_space(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TEXT_LINE_SPACE | (state << 8), value); }
static inline void lv_style_set_text_line_space(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TEXT_LINE_SPACE | (state << 8), value); }
static inline lv_text_decor_t lv_obj_get_style_text_decor(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TEXT_DECOR); }
static inline void lv_obj_set_style_local_text_decor(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_text_decor_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TEXT_DECOR | (state << 8), value); }
static inline void lv_style_set_text_decor(lv_style_t * style, lv_state_t state, lv_text_decor_t value) { _lv_style_set_int(style, LV_STYLE_TEXT_DECOR | (state << 8), value); }
static inline lv_blend_mode_t lv_obj_get_style_text_blend_mode(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TEXT_BLEND_MODE); }
static inline void lv_obj_set_style_local_text_blend_mode(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_blend_mode_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TEXT_BLEND_MODE | (state << 8), value); }
static inline void lv_style_set_text_blend_mode(lv_style_t * style, lv_state_t state, lv_blend_mode_t value) { _lv_style_set_int(style, LV_STYLE_TEXT_BLEND_MODE | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_text_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_TEXT_COLOR); }
static inline void lv_obj_set_style_local_text_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_TEXT_COLOR | (state << 8), value); }
static inline void lv_style_set_text_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_TEXT_COLOR | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_text_sel_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_TEXT_SEL_COLOR); }
static inline void lv_obj_set_style_local_text_sel_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_TEXT_SEL_COLOR | (state << 8), value); }
static inline void lv_style_set_text_sel_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_TEXT_SEL_COLOR | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_text_sel_bg_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_TEXT_SEL_BG_COLOR); }
static inline void lv_obj_set_style_local_text_sel_bg_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_TEXT_SEL_BG_COLOR | (state << 8), value); }
static inline void lv_style_set_text_sel_bg_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_TEXT_SEL_BG_COLOR | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_text_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_TEXT_OPA); }
static inline void lv_obj_set_style_local_text_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_TEXT_OPA | (state << 8), value); }
static inline void lv_style_set_text_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_TEXT_OPA | (state << 8), value); }
static inline const lv_font_t * lv_obj_get_style_text_font(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_ptr(obj, part, LV_STYLE_TEXT_FONT); }
static inline void lv_obj_set_style_local_text_font(lv_obj_t * obj, uint8_t part, lv_state_t state, const lv_font_t * value) { _lv_obj_set_style_local_ptr(obj, part, LV_STYLE_TEXT_FONT | (state << 8), value); }
static inline void lv_style_set_text_font(lv_style_t * style, lv_state_t state, const lv_font_t * value) { _lv_style_set_ptr(style, LV_STYLE_TEXT_FONT | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_line_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_LINE_WIDTH); }
static inline void lv_obj_set_style_local_line_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_LINE_WIDTH | (state << 8), value); }
static inline void lv_style_set_line_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_LINE_WIDTH | (state << 8), value); }
static inline lv_blend_mode_t lv_obj_get_style_line_blend_mode(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_LINE_BLEND_MODE); }
static inline void lv_obj_set_style_local_line_blend_mode(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_blend_mode_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_LINE_BLEND_MODE | (state << 8), value); }
static inline void lv_style_set_line_blend_mode(lv_style_t * style, lv_state_t state, lv_blend_mode_t value) { _lv_style_set_int(style, LV_STYLE_LINE_BLEND_MODE | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_line_dash_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_LINE_DASH_WIDTH); }
static inline void lv_obj_set_style_local_line_dash_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_LINE_DASH_WIDTH | (state << 8), value); }
static inline void lv_style_set_line_dash_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_LINE_DASH_WIDTH | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_line_dash_gap(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_LINE_DASH_GAP); }
static inline void lv_obj_set_style_local_line_dash_gap(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_LINE_DASH_GAP | (state << 8), value); }
static inline void lv_style_set_line_dash_gap(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_LINE_DASH_GAP | (state << 8), value); }
static inline bool lv_obj_get_style_line_rounded(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_LINE_ROUNDED); }
static inline void lv_obj_set_style_local_line_rounded(lv_obj_t * obj, uint8_t part, lv_state_t state, bool value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_LINE_ROUNDED | (state << 8), value); }
static inline void lv_style_set_line_rounded(lv_style_t * style, lv_state_t state, bool value) { _lv_style_set_int(style, LV_STYLE_LINE_ROUNDED | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_line_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_LINE_COLOR); }
static inline void lv_obj_set_style_local_line_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_LINE_COLOR | (state << 8), value); }
static inline void lv_style_set_line_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_LINE_COLOR | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_line_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_LINE_OPA); }
static inline void lv_obj_set_style_local_line_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_LINE_OPA | (state << 8), value); }
static inline void lv_style_set_line_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_LINE_OPA | (state << 8), value); }
static inline lv_blend_mode_t lv_obj_get_style_image_blend_mode(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_IMAGE_BLEND_MODE); }
static inline void lv_obj_set_style_local_image_blend_mode(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_blend_mode_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_IMAGE_BLEND_MODE | (state << 8), value); }
static inline void lv_style_set_image_blend_mode(lv_style_t * style, lv_state_t state, lv_blend_mode_t value) { _lv_style_set_int(style, LV_STYLE_IMAGE_BLEND_MODE | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_image_recolor(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_IMAGE_RECOLOR); }
static inline void lv_obj_set_style_local_image_recolor(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_IMAGE_RECOLOR | (state << 8), value); }
static inline void lv_style_set_image_recolor(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_IMAGE_RECOLOR | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_image_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_IMAGE_OPA); }
static inline void lv_obj_set_style_local_image_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_IMAGE_OPA | (state << 8), value); }
static inline void lv_style_set_image_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_IMAGE_OPA | (state << 8), value); }
static inline lv_opa_t lv_obj_get_style_image_recolor_opa(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_opa(obj, part, LV_STYLE_IMAGE_RECOLOR_OPA); }
static inline void lv_obj_set_style_local_image_recolor_opa(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_opa_t value) { _lv_obj_set_style_local_opa(obj, part, LV_STYLE_IMAGE_RECOLOR_OPA | (state << 8), value); }
static inline void lv_style_set_image_recolor_opa(lv_style_t * style, lv_state_t state, lv_opa_t value) { _lv_style_set_opa(style, LV_STYLE_IMAGE_RECOLOR_OPA | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transition_time(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSITION_TIME); }
static inline void lv_obj_set_style_local_transition_time(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSITION_TIME | (state << 8), value); }
static inline void lv_style_set_transition_time(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSITION_TIME | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transition_delay(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSITION_DELAY); }
static inline void lv_obj_set_style_local_transition_delay(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSITION_DELAY | (state << 8), value); }
static inline void lv_style_set_transition_delay(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSITION_DELAY | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transition_prop_1(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSITION_PROP_1); }
static inline void lv_obj_set_style_local_transition_prop_1(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSITION_PROP_1 | (state << 8), value); }
static inline void lv_style_set_transition_prop_1(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSITION_PROP_1 | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transition_prop_2(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSITION_PROP_2); }
static inline void lv_obj_set_style_local_transition_prop_2(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSITION_PROP_2 | (state << 8), value); }
static inline void lv_style_set_transition_prop_2(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSITION_PROP_2 | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transition_prop_3(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSITION_PROP_3); }
static inline void lv_obj_set_style_local_transition_prop_3(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSITION_PROP_3 | (state << 8), value); }
static inline void lv_style_set_transition_prop_3(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSITION_PROP_3 | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transition_prop_4(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSITION_PROP_4); }
static inline void lv_obj_set_style_local_transition_prop_4(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSITION_PROP_4 | (state << 8), value); }
static inline void lv_style_set_transition_prop_4(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSITION_PROP_4 | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transition_prop_5(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSITION_PROP_5); }
static inline void lv_obj_set_style_local_transition_prop_5(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSITION_PROP_5 | (state << 8), value); }
static inline void lv_style_set_transition_prop_5(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSITION_PROP_5 | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_transition_prop_6(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_TRANSITION_PROP_6); }
static inline void lv_obj_set_style_local_transition_prop_6(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_TRANSITION_PROP_6 | (state << 8), value); }
static inline void lv_style_set_transition_prop_6(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_TRANSITION_PROP_6 | (state << 8), value); }

static inline const lv_anim_path_t * lv_obj_get_style_transition_path(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_ptr(obj, part, LV_STYLE_TRANSITION_PATH); }
static inline void lv_obj_set_style_local_transition_path(lv_obj_t * obj, uint8_t part, lv_state_t state, const lv_anim_path_t * value) { _lv_obj_set_style_local_ptr(obj, part, LV_STYLE_TRANSITION_PATH | (state << 8), value); }
static inline void lv_style_set_transition_path(lv_style_t * style, lv_state_t state, const lv_anim_path_t * value) { _lv_style_set_ptr(style, LV_STYLE_TRANSITION_PATH | (state << 8), value); }




static inline lv_style_int_t lv_obj_get_style_scale_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SCALE_WIDTH); }
static inline void lv_obj_set_style_local_scale_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SCALE_WIDTH | (state << 8), value); }
static inline void lv_style_set_scale_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_SCALE_WIDTH | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_scale_border_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SCALE_BORDER_WIDTH); }
static inline void lv_obj_set_style_local_scale_border_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SCALE_BORDER_WIDTH | (state << 8), value); }
static inline void lv_style_set_scale_border_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_SCALE_BORDER_WIDTH | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_scale_end_border_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SCALE_END_BORDER_WIDTH); }
static inline void lv_obj_set_style_local_scale_end_border_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SCALE_END_BORDER_WIDTH | (state << 8), value); }
static inline void lv_style_set_scale_end_border_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_SCALE_END_BORDER_WIDTH | (state << 8), value); }
static inline lv_style_int_t lv_obj_get_style_scale_end_line_width(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_int(obj, part, LV_STYLE_SCALE_END_LINE_WIDTH); }
static inline void lv_obj_set_style_local_scale_end_line_width(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_style_int_t value) { _lv_obj_set_style_local_int(obj, part, LV_STYLE_SCALE_END_LINE_WIDTH | (state << 8), value); }
static inline void lv_style_set_scale_end_line_width(lv_style_t * style, lv_state_t state, lv_style_int_t value) { _lv_style_set_int(style, LV_STYLE_SCALE_END_LINE_WIDTH | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_scale_grad_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_SCALE_GRAD_COLOR); }
static inline void lv_obj_set_style_local_scale_grad_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_SCALE_GRAD_COLOR | (state << 8), value); }
static inline void lv_style_set_scale_grad_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_SCALE_GRAD_COLOR | (state << 8), value); }
static inline lv_color_t lv_obj_get_style_scale_end_color(const lv_obj_t * obj, uint8_t part) { return _lv_obj_get_style_color(obj, part, LV_STYLE_SCALE_END_COLOR); }
static inline void lv_obj_set_style_local_scale_end_color(lv_obj_t * obj, uint8_t part, lv_state_t state, lv_color_t value) { _lv_obj_set_style_local_color(obj, part, LV_STYLE_SCALE_END_COLOR | (state << 8), value); }
static inline void lv_style_set_scale_end_color(lv_style_t * style, lv_state_t state, lv_color_t value) { _lv_style_set_color(style, LV_STYLE_SCALE_END_COLOR | (state << 8), value); }


// ======================================================================
// Widgets
// ======================================================================
// LV Arc
lv_obj_t * lv_arc_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_arc_set_start_angle(lv_obj_t * arc, uint16_t start);
void lv_arc_set_end_angle(lv_obj_t * arc, uint16_t end);
void lv_arc_set_angles(lv_obj_t * arc, uint16_t start, uint16_t end);
void lv_arc_set_bg_start_angle(lv_obj_t * arc, uint16_t start);
void lv_arc_set_bg_end_angle(lv_obj_t * arc, uint16_t end);
void lv_arc_set_bg_angles(lv_obj_t * arc, uint16_t start, uint16_t end);
void lv_arc_set_rotation(lv_obj_t * arc, uint16_t rotation_angle);
void lv_arc_set_type(lv_obj_t * arc, lv_arc_type_t type);
void lv_arc_set_value(lv_obj_t * arc, int16_t value);
void lv_arc_set_range(lv_obj_t * arc, int16_t min, int16_t max);
void lv_arc_set_chg_rate(lv_obj_t * arc, uint16_t threshold);
void lv_arc_set_adjustable(lv_obj_t * arc, bool adjustable);
uint16_t lv_arc_get_angle_start(lv_obj_t * arc);
uint16_t lv_arc_get_angle_end(lv_obj_t * arc);
uint16_t lv_arc_get_bg_angle_start(lv_obj_t * arc);
uint16_t lv_arc_get_bg_angle_end(lv_obj_t * arc);
lv_arc_type_t lv_arc_get_type(const lv_obj_t * arc);
int16_t lv_arc_get_value(const lv_obj_t * arc);
int16_t lv_arc_get_min_value(const lv_obj_t * arc);
int16_t lv_arc_get_max_value(const lv_obj_t * arc);
bool lv_arc_is_dragged(const lv_obj_t * arc);
bool lv_arc_get_adjustable(lv_obj_t * arc);

// LV Bar
lv_obj_t * lv_bar_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_bar_set_value(lv_obj_t * bar, int16_t value, lv_anim_enable_t anim);
void lv_bar_set_start_value(lv_obj_t * bar, int16_t start_value, lv_anim_enable_t anim);
void lv_bar_set_range(lv_obj_t * bar, int16_t min, int16_t max);
void lv_bar_set_type(lv_obj_t * bar, lv_bar_type_t type);
void lv_bar_set_anim_time(lv_obj_t * bar, uint16_t anim_time);
int16_t lv_bar_get_value(const lv_obj_t * bar);
int16_t lv_bar_get_start_value(const lv_obj_t * bar);
int16_t lv_bar_get_min_value(const lv_obj_t * bar);
int16_t lv_bar_get_max_value(const lv_obj_t * bar);
lv_bar_type_t lv_bar_get_type(lv_obj_t * bar);
uint16_t lv_bar_get_anim_time(const lv_obj_t * bar);

// LV Btn
lv_obj_t * lv_btn_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_btn_set_checkable(lv_obj_t * btn, bool tgl);
void lv_btn_set_state(lv_obj_t * btn, lv_btn_state_t state);
void lv_btn_toggle(lv_obj_t * btn);
static inline void lv_btn_set_layout(lv_obj_t * btn, lv_layout_t layout)
static inline void lv_btn_set_fit4(lv_obj_t * btn, lv_fit_t left, lv_fit_t right, lv_fit_t top, lv_fit_t bottom)
static inline void lv_btn_set_fit2(lv_obj_t * btn, lv_fit_t hor, lv_fit_t ver)
static inline void lv_btn_set_fit(lv_obj_t * btn, lv_fit_t fit)
lv_btn_state_t lv_btn_get_state(const lv_obj_t * btn);
bool lv_btn_get_checkable(const lv_obj_t * btn);
static inline lv_layout_t lv_btn_get_layout(const lv_obj_t * btn)
static inline lv_fit_t lv_btn_get_fit_left(const lv_obj_t * btn)
static inline lv_fit_t lv_btn_get_fit_right(const lv_obj_t * btn)
static inline lv_fit_t lv_btn_get_fit_top(const lv_obj_t * btn)
static inline lv_fit_t lv_btn_get_fit_bottom(const lv_obj_t * btn)

// LV BtnMatrix
lv_obj_t * lv_btnmatrix_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_btnmatrix_set_map(lv_obj_t * btnm, const char * map[]);
void lv_btnmatrix_set_ctrl_map(lv_obj_t * btnm, const lv_btnmatrix_ctrl_t ctrl_map[]);
void lv_btnmatrix_set_focused_btn(lv_obj_t * btnm, uint16_t id);
void lv_btnmatrix_set_recolor(const lv_obj_t * btnm, bool en);
void lv_btnmatrix_set_btn_ctrl(lv_obj_t * btnm, uint16_t btn_id, lv_btnmatrix_ctrl_t ctrl);
void lv_btnmatrix_clear_btn_ctrl(const lv_obj_t * btnm, uint16_t btn_id, lv_btnmatrix_ctrl_t ctrl);
void lv_btnmatrix_set_btn_ctrl_all(lv_obj_t * btnm, lv_btnmatrix_ctrl_t ctrl);
void lv_btnmatrix_clear_btn_ctrl_all(lv_obj_t * btnm, lv_btnmatrix_ctrl_t ctrl);
void lv_btnmatrix_set_btn_width(lv_obj_t * btnm, uint16_t btn_id, uint8_t width);
void lv_btnmatrix_set_one_check(lv_obj_t * btnm, bool one_chk);
void lv_btnmatrix_set_align(lv_obj_t * btnm, lv_label_align_t align);
const char ** lv_btnmatrix_get_map_array(const lv_obj_t * btnm);
bool lv_btnmatrix_get_recolor(const lv_obj_t * btnm);
uint16_t lv_btnmatrix_get_active_btn(const lv_obj_t * btnm);
const char * lv_btnmatrix_get_active_btn_text(const lv_obj_t * btnm);
uint16_t lv_btnmatrix_get_focused_btn(const lv_obj_t * btnm);
const char * lv_btnmatrix_get_btn_text(const lv_obj_t * btnm, uint16_t btn_id);
bool lv_btnmatrix_get_btn_ctrl(lv_obj_t * btnm, uint16_t btn_id, lv_btnmatrix_ctrl_t ctrl);
bool lv_btnmatrix_get_one_check(const lv_obj_t * btnm);
lv_label_align_t lv_btnmatrix_get_align(const lv_obj_t * btnm);

// LV Calendar
// typedef struct {
//     uint16_t year;
//     int8_t month;
//     int8_t day;
// } lv_calendar_date_t;

lv_obj_t * lv_calendar_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_calendar_set_today_date(lv_obj_t * calendar, lv_calendar_date_t * today);
void lv_calendar_set_showed_date(lv_obj_t * calendar, lv_calendar_date_t * showed);
void lv_calendar_set_highlighted_dates(lv_obj_t * calendar, lv_calendar_date_t highlighted[], uint16_t date_num);
void lv_calendar_set_day_names(lv_obj_t * calendar, const char ** day_names);
void lv_calendar_set_month_names(lv_obj_t * calendar, const char ** month_names);
lv_calendar_date_t * lv_calendar_get_today_date(const lv_obj_t * calendar);
lv_calendar_date_t * lv_calendar_get_showed_date(const lv_obj_t * calendar);
lv_calendar_date_t * lv_calendar_get_pressed_date(const lv_obj_t * calendar);
lv_calendar_date_t * lv_calendar_get_highlighted_dates(const lv_obj_t * calendar);
uint16_t lv_calendar_get_highlighted_dates_num(const lv_obj_t * calendar);
const char ** lv_calendar_get_day_names(const lv_obj_t * calendar);
const char ** lv_calendar_get_month_names(const lv_obj_t * calendar);
uint8_t lv_calendar_get_day_of_week(uint32_t year, uint32_t month, uint32_t day);

// LV Canvas
lv_obj_t * lv_canvas_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_canvas_set_buffer(lv_obj_t * canvas, void * buf, lv_coord_t w, lv_coord_t h, lv_img_cf_t cf);
void lv_canvas_set_px(lv_obj_t * canvas, lv_coord_t x, lv_coord_t y, lv_color_t c);
void lv_canvas_set_palette(lv_obj_t * canvas, uint8_t id, lv_color_t c);
lv_color_t lv_canvas_get_px(lv_obj_t * canvas, lv_coord_t x, lv_coord_t y);
lv_img_dsc_t * lv_canvas_get_img(lv_obj_t * canvas);
void lv_canvas_copy_buf(lv_obj_t * canvas, const void * to_copy, lv_coord_t x, lv_coord_t y, lv_coord_t w,                        lv_coord_t h);
void lv_canvas_transform(lv_obj_t * canvas, lv_img_dsc_t * img, int16_t angle, uint16_t zoom, lv_coord_t offset_x,                         lv_coord_t offset_y,                         int32_t pivot_x, int32_t pivot_y, bool antialias);
void lv_canvas_blur_hor(lv_obj_t * canvas, const lv_area_t * area, uint16_t r);
void lv_canvas_blur_ver(lv_obj_t * canvas, const lv_area_t * area, uint16_t r);
void lv_canvas_fill_bg(lv_obj_t * canvas, lv_color_t color, lv_opa_t opa);
void lv_canvas_draw_rect(lv_obj_t * canvas, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h,                         const lv_draw_rect_dsc_t * rect_dsc);
void lv_canvas_draw_text(lv_obj_t * canvas, lv_coord_t x, lv_coord_t y, lv_coord_t max_w,                         lv_draw_label_dsc_t * label_draw_dsc,                         const char * txt, lv_label_align_t align);
void lv_canvas_draw_img(lv_obj_t * canvas, lv_coord_t x, lv_coord_t y, const void * src,                        const lv_draw_img_dsc_t * img_draw_dsc);
void lv_canvas_draw_line(lv_obj_t * canvas, const lv_point_t points[], uint32_t point_cnt,                         const lv_draw_line_dsc_t * line_draw_dsc);
void lv_canvas_draw_polygon(lv_obj_t * canvas, const lv_point_t points[], uint32_t point_cnt,                            const lv_draw_rect_dsc_t * poly_draw_dsc);
void lv_canvas_draw_arc(lv_obj_t * canvas, lv_coord_t x, lv_coord_t y, lv_coord_t r, int32_t start_angle,                        int32_t end_angle, const lv_draw_line_dsc_t * arc_draw_dsc);

// LV Chart
lv_obj_t * lv_chart_create(lv_obj_t * par, const lv_obj_t * copy);
lv_chart_series_t * lv_chart_add_series(lv_obj_t * chart, lv_color_t color);
void lv_chart_remove_series(lv_obj_t * chart, lv_chart_series_t * series);
lv_chart_cursor_t * lv_chart_add_cursor(lv_obj_t * chart, lv_color_t color, lv_cursor_direction_t dir);
void lv_chart_clear_series(lv_obj_t * chart, lv_chart_series_t * series);
void lv_chart_hide_series(lv_obj_t * chart, lv_chart_series_t * series, bool hide);
void lv_chart_set_div_line_count(lv_obj_t * chart, uint8_t hdiv, uint8_t vdiv);
void lv_chart_set_y_range(lv_obj_t * chart, lv_chart_axis_t axis, lv_coord_t ymin, lv_coord_t ymax);
void lv_chart_set_type(lv_obj_t * chart, lv_chart_type_t type);
void lv_chart_set_point_count(lv_obj_t * chart, uint16_t point_cnt);
void lv_chart_init_points(lv_obj_t * chart, lv_chart_series_t * ser, lv_coord_t y);
void lv_chart_set_points(lv_obj_t * chart, lv_chart_series_t * ser, lv_coord_t y_array[]);
void lv_chart_set_next(lv_obj_t * chart, lv_chart_series_t * ser, lv_coord_t y);
void lv_chart_set_update_mode(lv_obj_t * chart, lv_chart_update_mode_t update_mode);
void lv_chart_set_x_tick_length(lv_obj_t * chart, uint8_t major_tick_len, uint8_t minor_tick_len);
void lv_chart_set_y_tick_length(lv_obj_t * chart, uint8_t major_tick_len, uint8_t minor_tick_len);
void lv_chart_set_secondary_y_tick_length(lv_obj_t * chart, uint8_t major_tick_len, uint8_t minor_tick_len);
void lv_chart_set_x_tick_texts(lv_obj_t * chart, const char * list_of_values, uint8_t num_tick_marks,                               lv_chart_axis_options_t options);
void lv_chart_set_secondary_y_tick_texts(lv_obj_t * chart, const char * list_of_values, uint8_t num_tick_marks,                                         lv_chart_axis_options_t options);
void lv_chart_set_y_tick_texts(lv_obj_t * chart, const char * list_of_values, uint8_t num_tick_marks,                               lv_chart_axis_options_t options);
void lv_chart_set_x_start_point(lv_obj_t * chart, lv_chart_series_t * ser, uint16_t id);
void lv_chart_set_ext_array(lv_obj_t * chart, lv_chart_series_t * ser, lv_coord_t array[], uint16_t point_cnt);
void lv_chart_set_point_id(lv_obj_t * chart, lv_chart_series_t * ser, lv_coord_t value, uint16_t id);
void lv_chart_set_series_axis(lv_obj_t * chart, lv_chart_series_t * ser, lv_chart_axis_t axis);
void lv_chart_set_cursor_point(lv_obj_t * chart, lv_chart_cursor_t * cursor, lv_point_t * point);
lv_chart_type_t lv_chart_get_type(const lv_obj_t * chart);
uint16_t lv_chart_get_point_count(const lv_obj_t * chart);
uint16_t lv_chart_get_x_start_point(lv_chart_series_t * ser);
lv_coord_t lv_chart_get_point_id(lv_obj_t * chart, lv_chart_series_t * ser, uint16_t id);
lv_chart_axis_t lv_chart_get_series_axis(lv_obj_t * chart, lv_chart_series_t * ser);
void lv_chart_get_series_area(lv_obj_t * chart, lv_area_t * series_area);
lv_point_t lv_chart_get_cursor_point(lv_obj_t * chart, lv_chart_cursor_t * cursor);
uint16_t lv_chart_get_nearest_index_from_coord(lv_obj_t * chart, lv_coord_t x);
lv_coord_t lv_chart_get_x_from_index(lv_obj_t * chart, lv_chart_series_t * ser, uint16_t id);
lv_coord_t lv_chart_get_y_from_index(lv_obj_t * chart, lv_chart_series_t * ser, uint16_t id);
void lv_chart_refresh(lv_obj_t * chart);

// LV Checkbox
lv_obj_t * lv_checkbox_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_checkbox_set_text(lv_obj_t * cb, const char * txt);
void lv_checkbox_set_text_static(lv_obj_t * cb, const char * txt);
void lv_checkbox_set_checked(lv_obj_t * cb, bool checked);
void lv_checkbox_set_disabled(lv_obj_t * cb);
void lv_checkbox_set_state(lv_obj_t * cb, lv_btn_state_t state);
const char * lv_checkbox_get_text(const lv_obj_t * cb);
static inline bool lv_checkbox_is_checked(const lv_obj_t * cb)
static inline bool lv_checkbox_is_inactive(const lv_obj_t * cb)
static inline lv_btn_state_t lv_checkbox_get_state(const lv_obj_t * cb)

// LV Cont
lv_obj_t * lv_cont_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_cont_set_layout(lv_obj_t * cont, lv_layout_t layout);
void lv_cont_set_fit4(lv_obj_t * cont, lv_fit_t left, lv_fit_t right, lv_fit_t top, lv_fit_t bottom);
static inline void lv_cont_set_fit2(lv_obj_t * cont, lv_fit_t hor, lv_fit_t ver)
// {
//     lv_cont_set_fit4(cont, hor, hor, ver, ver);
// }
static inline void lv_cont_set_fit(lv_obj_t * cont, lv_fit_t fit)
// {
//     lv_cont_set_fit4(cont, fit, fit, fit, fit);
// }
lv_layout_t lv_cont_get_layout(const lv_obj_t * cont);
lv_fit_t lv_cont_get_fit_left(const lv_obj_t * cont);
lv_fit_t lv_cont_get_fit_right(const lv_obj_t * cont);
lv_fit_t lv_cont_get_fit_top(const lv_obj_t * cont);
lv_fit_t lv_cont_get_fit_bottom(const lv_obj_t * cont);

// LV Cpicker
lv_obj_t * lv_cpicker_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_cpicker_set_type(lv_obj_t * cpicker, lv_cpicker_type_t type);
bool lv_cpicker_set_hue(lv_obj_t * cpicker, uint16_t hue);
bool lv_cpicker_set_saturation(lv_obj_t * cpicker, uint8_t saturation);
bool lv_cpicker_set_value(lv_obj_t * cpicker, uint8_t val);
bool lv_cpicker_set_hsv(lv_obj_t * cpicker, lv_color_hsv_t hsv);
bool lv_cpicker_set_color(lv_obj_t * cpicker, lv_color_t color);
void lv_cpicker_set_color_mode(lv_obj_t * cpicker, lv_cpicker_color_mode_t mode);
void lv_cpicker_set_color_mode_fixed(lv_obj_t * cpicker, bool fixed);
void lv_cpicker_set_knob_colored(lv_obj_t * cpicker, bool en);
lv_cpicker_color_mode_t lv_cpicker_get_color_mode(lv_obj_t * cpicker);
bool lv_cpicker_get_color_mode_fixed(lv_obj_t * cpicker);
uint16_t lv_cpicker_get_hue(lv_obj_t * cpicker);
uint8_t lv_cpicker_get_saturation(lv_obj_t * cpicker);
uint8_t lv_cpicker_get_value(lv_obj_t * cpicker);
lv_color_hsv_t lv_cpicker_get_hsv(lv_obj_t * cpicker);
lv_color_t lv_cpicker_get_color(lv_obj_t * cpicker);
bool lv_cpicker_get_knob_colored(lv_obj_t * cpicker);

// LV Dropdown
lv_obj_t * lv_dropdown_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_dropdown_set_text(lv_obj_t * ddlist, const char * txt);
void lv_dropdown_clear_options(lv_obj_t * ddlist);
void lv_dropdown_set_options(lv_obj_t * ddlist, const char * options);
void lv_dropdown_set_options_static(lv_obj_t * ddlist, const char * options);
void lv_dropdown_add_option(lv_obj_t * ddlist, const char * option, uint32_t pos);
void lv_dropdown_set_selected(lv_obj_t * ddlist, uint16_t sel_opt);
void lv_dropdown_set_dir(lv_obj_t * ddlist, lv_dropdown_dir_t dir);
void lv_dropdown_set_max_height(lv_obj_t * ddlist, lv_coord_t h);
void lv_dropdown_set_symbol(lv_obj_t * ddlist, const char * symbol);
void lv_dropdown_set_show_selected(lv_obj_t * ddlist, bool show);
const char * lv_dropdown_get_text(lv_obj_t * ddlist);
const char * lv_dropdown_get_options(const lv_obj_t * ddlist);
uint16_t lv_dropdown_get_selected(const lv_obj_t * ddlist);
uint16_t lv_dropdown_get_option_cnt(const lv_obj_t * ddlist);
void lv_dropdown_get_selected_str(const lv_obj_t * ddlist, char * buf, uint32_t buf_size);
lv_coord_t lv_dropdown_get_max_height(const lv_obj_t * ddlist);
const char * lv_dropdown_get_symbol(lv_obj_t * ddlist);
lv_dropdown_dir_t lv_dropdown_get_dir(const lv_obj_t * ddlist);
bool lv_dropdown_get_show_selected(lv_obj_t * ddlist);
void lv_dropdown_open(lv_obj_t * ddlist);
void lv_dropdown_close(lv_obj_t * ddlist);

// LV Gauge
lv_obj_t * lv_gauge_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_gauge_set_needle_count(lv_obj_t * gauge, uint8_t needle_cnt, const lv_color_t colors[]);
void lv_gauge_set_value(lv_obj_t * gauge, uint8_t needle_id, int32_t value);
static inline void lv_gauge_set_range(lv_obj_t * gauge, int32_t min, int32_t max)
static inline void lv_gauge_set_critical_value(lv_obj_t * gauge, int32_t value)
void lv_gauge_set_scale(lv_obj_t * gauge, uint16_t angle, uint8_t line_cnt, uint8_t label_cnt);
static inline void lv_gauge_set_angle_offset(lv_obj_t * gauge, uint16_t angle)
void lv_gauge_set_needle_img(lv_obj_t * gauge, const void * img, lv_coord_t pivot_x, lv_coord_t pivot_y);
void lv_gauge_set_formatter_cb(lv_obj_t * gauge, lv_gauge_format_cb_t format_cb);
int32_t lv_gauge_get_value(const lv_obj_t * gauge, uint8_t needle);
uint8_t lv_gauge_get_needle_count(const lv_obj_t * gauge);
static inline int32_t lv_gauge_get_min_value(const lv_obj_t * lmeter)
static inline int32_t lv_gauge_get_max_value(const lv_obj_t * lmeter)
static inline int32_t lv_gauge_get_critical_value(const lv_obj_t * gauge)
uint8_t lv_gauge_get_label_count(const lv_obj_t * gauge);
static inline uint16_t lv_gauge_get_line_count(const lv_obj_t * gauge)
static inline uint16_t lv_gauge_get_scale_angle(const lv_obj_t * gauge)
static inline uint16_t lv_gauge_get_angle_offset(lv_obj_t * gauge)
const void * lv_gauge_get_needle_img(lv_obj_t * gauge);
lv_coord_t lv_gauge_get_needle_img_pivot_x(lv_obj_t * gauge);
lv_coord_t lv_gauge_get_needle_img_pivot_y(lv_obj_t * gauge);

// LV Img
lv_obj_t * lv_img_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_img_set_src(lv_obj_t * img, const void * src_img);
void lv_img_set_auto_size(lv_obj_t * img, bool autosize_en);
void lv_img_set_offset_x(lv_obj_t * img, lv_coord_t x);
void lv_img_set_offset_y(lv_obj_t * img, lv_coord_t y);
void lv_img_set_pivot(lv_obj_t * img, lv_coord_t pivot_x, lv_coord_t pivot_y);
void lv_img_set_angle(lv_obj_t * img, int16_t angle);
void lv_img_set_zoom(lv_obj_t * img, uint16_t zoom);
void lv_img_set_antialias(lv_obj_t * img, bool antialias);
const void * lv_img_get_src(lv_obj_t * img);
const char * lv_img_get_file_name(const lv_obj_t * img);
bool lv_img_get_auto_size(const lv_obj_t * img);
lv_coord_t lv_img_get_offset_x(lv_obj_t * img);
lv_coord_t lv_img_get_offset_y(lv_obj_t * img);
uint16_t lv_img_get_angle(lv_obj_t * img);
void lv_img_get_pivot(lv_obj_t * img, lv_point_t * center);
uint16_t lv_img_get_zoom(lv_obj_t * img);
bool lv_img_get_antialias(lv_obj_t * img);

// LV Imgbtn
lv_obj_t * lv_imgbtn_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_imgbtn_set_src(lv_obj_t * imgbtn, lv_btn_state_t state, const void * src);
void lv_imgbtn_set_src_tiled(lv_obj_t * imgbtn, lv_btn_state_t state, const void * src_left, const void * src_mid,
void lv_imgbtn_set_state(lv_obj_t * imgbtn, lv_btn_state_t state);
void lv_imgbtn_toggle(lv_obj_t * imgbtn);
static inline void lv_imgbtn_set_checkable(lv_obj_t * imgbtn, bool tgl)
const void * lv_imgbtn_get_src(lv_obj_t * imgbtn, lv_btn_state_t state);
// const void * lv_imgbtn_get_src_left(lv_obj_t * imgbtn, lv_btn_state_t state);
// const void * lv_imgbtn_get_src_middle(lv_obj_t * imgbtn, lv_btn_state_t state);
// const void * lv_imgbtn_get_src_right(lv_obj_t * imgbtn, lv_btn_state_t state);
static inline lv_btn_state_t lv_imgbtn_get_state(const lv_obj_t * imgbtn)

// LV Keyboard
lv_obj_t * lv_keyboard_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_keyboard_set_textarea(lv_obj_t * kb, lv_obj_t * ta);
void lv_keyboard_set_mode(lv_obj_t * kb, lv_keyboard_mode_t mode);
void lv_keyboard_set_cursor_manage(lv_obj_t * kb, bool en);
void lv_keyboard_set_map(lv_obj_t * kb, lv_keyboard_mode_t mode, const char * map[]);
void lv_keyboard_set_ctrl_map(lv_obj_t * kb, lv_keyboard_mode_t mode, const lv_btnmatrix_ctrl_t ctrl_map[]);
lv_obj_t * lv_keyboard_get_textarea(const lv_obj_t * kb);
lv_keyboard_mode_t lv_keyboard_get_mode(const lv_obj_t * kb);
bool lv_keyboard_get_cursor_manage(const lv_obj_t * kb);
static inline const char ** lv_keyboard_get_map_array(const lv_obj_t * kb)
void lv_keyboard_def_event_cb(lv_obj_t * kb, lv_event_t event);

// LV Label
lv_obj_t * lv_label_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_label_set_text(lv_obj_t * label, const char * text);
void lv_label_set_text_fmt(lv_obj_t * label, const char * fmt, ...);
void lv_label_set_text_static(lv_obj_t * label, const char * text);
void lv_label_set_long_mode(lv_obj_t * label, lv_label_long_mode_t long_mode);
void lv_label_set_align(lv_obj_t * label, lv_label_align_t align);
void lv_label_set_recolor(lv_obj_t * label, bool en);
void lv_label_set_anim_speed(lv_obj_t * label, uint16_t anim_speed);
void lv_label_set_text_sel_start(lv_obj_t * label, uint32_t index);
void lv_label_set_text_sel_end(lv_obj_t * label, uint32_t index);
char * lv_label_get_text(const lv_obj_t * label);
lv_label_long_mode_t lv_label_get_long_mode(const lv_obj_t * label);
lv_label_align_t lv_label_get_align(const lv_obj_t * label);
bool lv_label_get_recolor(const lv_obj_t * label);
uint16_t lv_label_get_anim_speed(const lv_obj_t * label);
void lv_label_get_letter_pos(const lv_obj_t * label, uint32_t index, lv_point_t * pos);
uint32_t lv_label_get_letter_on(const lv_obj_t * label, lv_point_t * pos);
bool lv_label_is_char_under_pos(const lv_obj_t * label, lv_point_t * pos);
uint32_t lv_label_get_text_sel_start(const lv_obj_t * label);
uint32_t lv_label_get_text_sel_end(const lv_obj_t * label);
lv_style_list_t * lv_label_get_style(lv_obj_t * label, uint8_t type);
void lv_label_ins_text(lv_obj_t * label, uint32_t pos, const char * txt);
void lv_label_cut_text(lv_obj_t * label, uint32_t pos, uint32_t cnt);
void lv_label_refr_text(lv_obj_t * label);

// LV Led
lv_obj_t * lv_led_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_led_set_bright(lv_obj_t * led, uint8_t bright);
void lv_led_on(lv_obj_t * led);
void lv_led_off(lv_obj_t * led);
void lv_led_toggle(lv_obj_t * led);
uint8_t lv_led_get_bright(const lv_obj_t * led);

// LV Line
lv_obj_t * lv_line_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_line_set_points(lv_obj_t * line, const lv_point_t point_a[], uint16_t point_num);
void lv_line_set_auto_size(lv_obj_t * line, bool en);
void lv_line_set_y_invert(lv_obj_t * line, bool en);
bool lv_line_get_auto_size(const lv_obj_t * line);
bool lv_line_get_y_invert(const lv_obj_t * line);

// LV Linemeter
lv_obj_t * lv_linemeter_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_linemeter_set_value(lv_obj_t * lmeter, int32_t value);
void lv_linemeter_set_range(lv_obj_t * lmeter, int32_t min, int32_t max);
void lv_linemeter_set_scale(lv_obj_t * lmeter, uint16_t angle, uint16_t line_cnt);
void lv_linemeter_set_angle_offset(lv_obj_t * lmeter, uint16_t angle);
void lv_linemeter_set_mirror(lv_obj_t * lmeter, bool mirror);
int32_t lv_linemeter_get_value(const lv_obj_t * lmeter);
int32_t lv_linemeter_get_min_value(const lv_obj_t * lmeter);
int32_t lv_linemeter_get_max_value(const lv_obj_t * lmeter);
uint16_t lv_linemeter_get_line_count(const lv_obj_t * lmeter);
uint16_t lv_linemeter_get_scale_angle(const lv_obj_t * lmeter);
uint16_t lv_linemeter_get_angle_offset(lv_obj_t * lmeter);
void lv_linemeter_draw_scale(lv_obj_t * lmeter, const lv_area_t * clip_area, uint8_t part);
bool lv_linemeter_get_mirror(lv_obj_t * lmeter);

// LV List
lv_obj_t * lv_list_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_list_clean(lv_obj_t * list);
lv_obj_t * lv_list_add_btn(lv_obj_t * list, const void * img_src, const char * txt);
bool lv_list_remove(const lv_obj_t * list, uint16_t index);
void lv_list_focus_btn(lv_obj_t * list, lv_obj_t * btn);
static inline void lv_list_set_scrollbar_mode(lv_obj_t * list, lv_scrollbar_mode_t mode)
static inline void lv_list_set_scroll_propagation(lv_obj_t * list, bool en)
static inline void lv_list_set_edge_flash(lv_obj_t * list, bool en)
static inline void lv_list_set_anim_time(lv_obj_t * list, uint16_t anim_time)
void lv_list_set_layout(lv_obj_t * list, lv_layout_t layout);
const char * lv_list_get_btn_text(const lv_obj_t * btn);
lv_obj_t * lv_list_get_btn_label(const lv_obj_t * btn);
lv_obj_t * lv_list_get_btn_img(const lv_obj_t * btn);
lv_obj_t * lv_list_get_prev_btn(const lv_obj_t * list, lv_obj_t * prev_btn);
lv_obj_t * lv_list_get_next_btn(const lv_obj_t * list, lv_obj_t * prev_btn);
int32_t lv_list_get_btn_index(const lv_obj_t * list, const lv_obj_t * btn);
uint16_t lv_list_get_size(const lv_obj_t * list);
lv_obj_t * lv_list_get_btn_selected(const lv_obj_t * list);
lv_layout_t lv_list_get_layout(lv_obj_t * list);
static inline lv_scrollbar_mode_t lv_list_get_scrollbar_mode(const lv_obj_t * list)
static inline bool lv_list_get_scroll_propagation(lv_obj_t * list)
static inline bool lv_list_get_edge_flash(lv_obj_t * list)
static inline uint16_t lv_list_get_anim_time(const lv_obj_t * list)
void lv_list_up(const lv_obj_t * list);
void lv_list_down(const lv_obj_t * list);
void lv_list_focus(const lv_obj_t * btn, lv_anim_enable_t anim);

// LV Msgbox
lv_obj_t * lv_msgbox_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_msgbox_add_btns(lv_obj_t * mbox, const char * btn_mapaction[]);
void lv_msgbox_set_text(lv_obj_t * mbox, const char * txt);
void lv_msgbox_set_text_fmt(lv_obj_t * mbox, const char * fmt, ...);
void lv_msgbox_set_anim_time(lv_obj_t * mbox, uint16_t anim_time);
void lv_msgbox_start_auto_close(lv_obj_t * mbox, uint16_t delay);
void lv_msgbox_stop_auto_close(lv_obj_t * mbox);
void lv_msgbox_set_recolor(lv_obj_t * mbox, bool en);
const char * lv_msgbox_get_text(const lv_obj_t * mbox);
uint16_t lv_msgbox_get_active_btn(lv_obj_t * mbox);
const char * lv_msgbox_get_active_btn_text(lv_obj_t * mbox);
uint16_t lv_msgbox_get_anim_time(const lv_obj_t * mbox);
bool lv_msgbox_get_recolor(const lv_obj_t * mbox);
lv_obj_t * lv_msgbox_get_btnmatrix(lv_obj_t * mbox);

// LV Objmask
lv_obj_t * lv_objmask_create(lv_obj_t * par, const lv_obj_t * copy);
lv_objmask_mask_t * lv_objmask_add_mask(lv_obj_t * objmask, void * param);
void lv_objmask_update_mask(lv_obj_t * objmask, lv_objmask_mask_t * mask, void * param);
void lv_objmask_remove_mask(lv_obj_t * objmask, lv_objmask_mask_t * mask);

// LV Templ
// lv_obj_t * lv_templ_create(lv_obj_t * par, const lv_obj_t * copy);
// void lv_templ_set_style(lv_obj_t * templ, lv_templ_style_t type, const lv_style_t * style);
// lv_style_t * lv_templ_get_style(const lv_obj_t * templ, lv_templ_style_t type);

// LV Page
lv_obj_t * lv_page_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_page_clean(lv_obj_t * page);
lv_obj_t * lv_page_get_scrollable(const lv_obj_t * page);
uint16_t lv_page_get_anim_time(const lv_obj_t * page);
void lv_page_set_scrollbar_mode(lv_obj_t * page, lv_scrollbar_mode_t sb_mode);
void lv_page_set_anim_time(lv_obj_t * page, uint16_t anim_time);
void lv_page_set_scroll_propagation(lv_obj_t * page, bool en);
void lv_page_set_edge_flash(lv_obj_t * page, bool en);
static inline void lv_page_set_scrollable_fit4(lv_obj_t * page, lv_fit_t left, lv_fit_t right, lv_fit_t top,
static inline void lv_page_set_scrollable_fit2(lv_obj_t * page, lv_fit_t hor, lv_fit_t ver)
static inline void lv_page_set_scrollable_fit(lv_obj_t * page, lv_fit_t fit)
static inline void lv_page_set_scrl_width(lv_obj_t * page, lv_coord_t w)
static inline void lv_page_set_scrl_height(lv_obj_t * page, lv_coord_t h)
static inline void lv_page_set_scrl_layout(lv_obj_t * page, lv_layout_t layout)
lv_scrollbar_mode_t lv_page_get_scrollbar_mode(const lv_obj_t * page);
bool lv_page_get_scroll_propagation(lv_obj_t * page);
bool lv_page_get_edge_flash(lv_obj_t * page);
lv_coord_t lv_page_get_width_fit(lv_obj_t * page);
lv_coord_t lv_page_get_height_fit(lv_obj_t * page);
lv_coord_t lv_page_get_width_grid(lv_obj_t * page, uint8_t div, uint8_t span);
lv_coord_t lv_page_get_height_grid(lv_obj_t * page, uint8_t div, uint8_t span);
static inline lv_coord_t lv_page_get_scrl_width(const lv_obj_t * page)
static inline lv_coord_t lv_page_get_scrl_height(const lv_obj_t * page)
static inline lv_layout_t lv_page_get_scrl_layout(const lv_obj_t * page)
static inline lv_fit_t lv_page_get_scrl_fit_left(const lv_obj_t * page)
static inline lv_fit_t lv_page_get_scrl_fit_right(const lv_obj_t * page)
static inline lv_fit_t lv_page_get_scrl_fit_top(const lv_obj_t * page)
static inline lv_fit_t lv_page_get_scrl_fit_bottom(const lv_obj_t * page)
bool lv_page_on_edge(lv_obj_t * page, lv_page_edge_t edge);
void lv_page_glue_obj(lv_obj_t * obj, bool glue);
void lv_page_focus(lv_obj_t * page, const lv_obj_t * obj, lv_anim_enable_t anim_en);
void lv_page_scroll_hor(lv_obj_t * page, lv_coord_t dist);
void lv_page_scroll_ver(lv_obj_t * page, lv_coord_t dist);
void lv_page_start_edge_flash(lv_obj_t * page, lv_page_edge_t edge);

// LV Roller
lv_obj_t * lv_roller_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_roller_set_options(lv_obj_t * roller, const char * options, lv_roller_mode_t mode);
void lv_roller_set_align(lv_obj_t * roller, lv_label_align_t align);
void lv_roller_set_selected(lv_obj_t * roller, uint16_t sel_opt, lv_anim_enable_t anim);
void lv_roller_set_visible_row_count(lv_obj_t * roller, uint8_t row_cnt);
void lv_roller_set_auto_fit(lv_obj_t * roller, bool auto_fit);
static inline void lv_roller_set_anim_time(lv_obj_t * roller, uint16_t anim_time)
uint16_t lv_roller_get_selected(const lv_obj_t * roller);
uint16_t lv_roller_get_option_cnt(const lv_obj_t * roller);
void lv_roller_get_selected_str(const lv_obj_t * roller, char * buf, uint32_t buf_size);
lv_label_align_t lv_roller_get_align(const lv_obj_t * roller);
bool lv_roller_get_auto_fit(lv_obj_t * roller);
const char * lv_roller_get_options(const lv_obj_t * roller);
static inline uint16_t lv_roller_get_anim_time(const lv_obj_t * roller)

// LV Slider
lv_obj_t * lv_slider_create(lv_obj_t * par, const lv_obj_t * copy);
static inline void lv_slider_set_value(lv_obj_t * slider, int16_t value, lv_anim_enable_t anim)
static inline void lv_slider_set_left_value(lv_obj_t * slider, int16_t left_value, lv_anim_enable_t anim)
static inline void lv_slider_set_range(lv_obj_t * slider, int16_t min, int16_t max)
static inline void lv_slider_set_anim_time(lv_obj_t * slider, uint16_t anim_time)
static inline void lv_slider_set_type(lv_obj_t * slider, lv_slider_type_t type)
int16_t lv_slider_get_value(const lv_obj_t * slider);
static inline int16_t lv_slider_get_left_value(const lv_obj_t * slider)
static inline int16_t lv_slider_get_min_value(const lv_obj_t * slider)
static inline int16_t lv_slider_get_max_value(const lv_obj_t * slider)
bool lv_slider_is_dragged(const lv_obj_t * slider);
static inline uint16_t lv_slider_get_anim_time(lv_obj_t * slider)
static inline lv_slider_type_t lv_slider_get_type(lv_obj_t * slider)

// LV Spinbox
lv_obj_t * lv_spinbox_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_spinbox_set_rollover(lv_obj_t * spinbox, bool b);
void lv_spinbox_set_value(lv_obj_t * spinbox, int32_t i);
void lv_spinbox_set_digit_format(lv_obj_t * spinbox, uint8_t digit_count, uint8_t separator_position);
void lv_spinbox_set_step(lv_obj_t * spinbox, uint32_t step);
void lv_spinbox_set_range(lv_obj_t * spinbox, int32_t range_min, int32_t range_max);
void lv_spinbox_set_padding_left(lv_obj_t * spinbox, uint8_t padding);
bool lv_spinbox_get_rollover(lv_obj_t * spinbox);
int32_t lv_spinbox_get_value(lv_obj_t * spinbox);
static inline int32_t lv_spinbox_get_step(lv_obj_t * spinbox)
void lv_spinbox_step_next(lv_obj_t * spinbox);
void lv_spinbox_step_prev(lv_obj_t * spinbox);
void lv_spinbox_increment(lv_obj_t * spinbox);
void lv_spinbox_decrement(lv_obj_t * spinbox);

// LV Spinner
lv_obj_t * lv_spinner_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_spinner_set_arc_length(lv_obj_t * spinner, lv_anim_value_t deg);
void lv_spinner_set_spin_time(lv_obj_t * spinner, uint16_t time);
void lv_spinner_set_type(lv_obj_t * spinner, lv_spinner_type_t type);
void lv_spinner_set_dir(lv_obj_t * spinner, lv_spinner_dir_t dir);
lv_anim_value_t lv_spinner_get_arc_length(const lv_obj_t * spinner);
uint16_t lv_spinner_get_spin_time(const lv_obj_t * spinner);
lv_spinner_type_t lv_spinner_get_type(lv_obj_t * spinner);
lv_spinner_dir_t lv_spinner_get_dir(lv_obj_t * spinner);
void lv_spinner_anim_cb(void * ptr, lv_anim_value_t val);

// LV Switch
lv_obj_t * lv_switch_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_switch_on(lv_obj_t * sw, lv_anim_enable_t anim);
void lv_switch_off(lv_obj_t * sw, lv_anim_enable_t anim);
bool lv_switch_toggle(lv_obj_t * sw, lv_anim_enable_t anim);
static inline void lv_switch_set_anim_time(lv_obj_t * sw, uint16_t anim_time)
static inline bool lv_switch_get_state(const lv_obj_t * sw)
static inline uint16_t lv_switch_get_anim_time(const lv_obj_t * sw)

// LV Table
lv_obj_t * lv_table_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_table_set_cell_value(lv_obj_t * table, uint16_t row, uint16_t col, const char * txt);
void lv_table_set_cell_value_fmt(lv_obj_t * table, uint16_t row, uint16_t col, const char * fmt, ...);
void lv_table_set_row_cnt(lv_obj_t * table, uint16_t row_cnt);
void lv_table_set_col_cnt(lv_obj_t * table, uint16_t col_cnt);
void lv_table_set_col_width(lv_obj_t * table, uint16_t col_id, lv_coord_t w);
void lv_table_set_cell_align(lv_obj_t * table, uint16_t row, uint16_t col, lv_label_align_t align);
void lv_table_set_cell_type(lv_obj_t * table, uint16_t row, uint16_t col, uint8_t type);
void lv_table_set_cell_crop(lv_obj_t * table, uint16_t row, uint16_t col, bool crop);
void lv_table_set_cell_merge_right(lv_obj_t * table, uint16_t row, uint16_t col, bool en);
const char * lv_table_get_cell_value(lv_obj_t * table, uint16_t row, uint16_t col);
uint16_t lv_table_get_row_cnt(lv_obj_t * table);
uint16_t lv_table_get_col_cnt(lv_obj_t * table);
lv_coord_t lv_table_get_col_width(lv_obj_t * table, uint16_t col_id);
lv_label_align_t lv_table_get_cell_align(lv_obj_t * table, uint16_t row, uint16_t col);
lv_label_align_t lv_table_get_cell_type(lv_obj_t * table, uint16_t row, uint16_t col);
lv_label_align_t lv_table_get_cell_crop(lv_obj_t * table, uint16_t row, uint16_t col);
bool lv_table_get_cell_merge_right(lv_obj_t * table, uint16_t row, uint16_t col);
lv_res_t lv_table_get_pressed_cell(lv_obj_t * table, uint16_t * row, uint16_t * col);

// LV Tabview
lv_obj_t * lv_tabview_create(lv_obj_t * par, const lv_obj_t * copy);
lv_obj_t * lv_tabview_add_tab(lv_obj_t * tabview, const char * name);
void lv_tabview_clean_tab(lv_obj_t * tab);
void lv_tabview_set_tab_act(lv_obj_t * tabview, uint16_t id, lv_anim_enable_t anim);
void lv_tabview_set_tab_name(lv_obj_t * tabview, uint16_t id, char * name);
void lv_tabview_set_anim_time(lv_obj_t * tabview, uint16_t anim_time);
void lv_tabview_set_btns_pos(lv_obj_t * tabview, lv_tabview_btns_pos_t btns_pos);
uint16_t lv_tabview_get_tab_act(const lv_obj_t * tabview);
uint16_t lv_tabview_get_tab_count(const lv_obj_t * tabview);
lv_obj_t * lv_tabview_get_tab(const lv_obj_t * tabview, uint16_t id);
uint16_t lv_tabview_get_anim_time(const lv_obj_t * tabview);
lv_tabview_btns_pos_t lv_tabview_get_btns_pos(const lv_obj_t * tabview);

// LV Textarea
lv_obj_t * lv_textarea_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_textarea_add_char(lv_obj_t * ta, uint32_t c);
void lv_textarea_add_text(lv_obj_t * ta, const char * txt);
void lv_textarea_del_char(lv_obj_t * ta);
void lv_textarea_del_char_forward(lv_obj_t * ta);
void lv_textarea_set_text(lv_obj_t * ta, const char * txt);
void lv_textarea_set_placeholder_text(lv_obj_t * ta, const char * txt);
void lv_textarea_set_cursor_pos(lv_obj_t * ta, int32_t pos);
void lv_textarea_set_cursor_hidden(lv_obj_t * ta, bool hide);
void lv_textarea_set_cursor_click_pos(lv_obj_t * ta, bool en);
void lv_textarea_set_pwd_mode(lv_obj_t * ta, bool en);
void lv_textarea_set_one_line(lv_obj_t * ta, bool en);
void lv_textarea_set_text_align(lv_obj_t * ta, lv_label_align_t align);
void lv_textarea_set_accepted_chars(lv_obj_t * ta, const char * list);
void lv_textarea_set_max_length(lv_obj_t * ta, uint32_t num);
void lv_textarea_set_insert_replace(lv_obj_t * ta, const char * txt);
static inline void lv_textarea_set_scrollbar_mode(lv_obj_t * ta, lv_scrollbar_mode_t mode)
static inline void lv_textarea_set_scroll_propagation(lv_obj_t * ta, bool en)
static inline void lv_textarea_set_edge_flash(lv_obj_t * ta, bool en)
void lv_textarea_set_text_sel(lv_obj_t * ta, bool en);
void lv_textarea_set_pwd_show_time(lv_obj_t * ta, uint16_t time);
void lv_textarea_set_cursor_blink_time(lv_obj_t * ta, uint16_t time);
const char * lv_textarea_get_text(const lv_obj_t * ta);
const char * lv_textarea_get_placeholder_text(lv_obj_t * ta);
lv_obj_t * lv_textarea_get_label(const lv_obj_t * ta);
uint32_t lv_textarea_get_cursor_pos(const lv_obj_t * ta);
bool lv_textarea_get_cursor_hidden(const lv_obj_t * ta);
bool lv_textarea_get_cursor_click_pos(lv_obj_t * ta);
bool lv_textarea_get_pwd_mode(const lv_obj_t * ta);
bool lv_textarea_get_one_line(const lv_obj_t * ta);
const char * lv_textarea_get_accepted_chars(lv_obj_t * ta);
uint32_t lv_textarea_get_max_length(lv_obj_t * ta);
static inline lv_scrollbar_mode_t lv_textarea_get_scrollbar_mode(const lv_obj_t * ta)
static inline bool lv_textarea_get_scroll_propagation(lv_obj_t * ta)
static inline bool lv_textarea_get_edge_flash(lv_obj_t * ta)
bool lv_textarea_text_is_selected(const lv_obj_t * ta);
bool lv_textarea_get_text_sel_en(lv_obj_t * ta);
uint16_t lv_textarea_get_pwd_show_time(lv_obj_t * ta);
uint16_t lv_textarea_get_cursor_blink_time(lv_obj_t * ta);
void lv_textarea_clear_selection(lv_obj_t * ta);
void lv_textarea_cursor_right(lv_obj_t * ta);
void lv_textarea_cursor_left(lv_obj_t * ta);
void lv_textarea_cursor_down(lv_obj_t * ta);
void lv_textarea_cursor_up(lv_obj_t * ta);

// LV Tileview
lv_obj_t * lv_tileview_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_tileview_add_element(lv_obj_t * tileview, lv_obj_t * element);
void lv_tileview_set_valid_positions(lv_obj_t * tileview, const lv_point_t valid_pos[], uint16_t valid_pos_cnt);
void lv_tileview_set_tile_act(lv_obj_t * tileview, lv_coord_t x, lv_coord_t y, lv_anim_enable_t anim);
static inline void lv_tileview_set_edge_flash(lv_obj_t * tileview, bool en)
static inline void lv_tileview_set_anim_time(lv_obj_t * tileview, uint16_t anim_time)
void lv_tileview_get_tile_act(lv_obj_t * tileview, lv_coord_t * x, lv_coord_t * y);
static inline bool lv_tileview_get_edge_flash(lv_obj_t * tileview)
static inline uint16_t lv_tileview_get_anim_time(lv_obj_t * tileview)

// LV Win
lv_obj_t * lv_win_create(lv_obj_t * par, const lv_obj_t * copy);
void lv_win_clean(lv_obj_t * win);
lv_obj_t * lv_win_add_btn_right(lv_obj_t * win, const void * img_src);
lv_obj_t * lv_win_add_btn_left(lv_obj_t * win, const void * img_src);
void lv_win_close_event_cb(lv_obj_t * btn, lv_event_t event);
void lv_win_set_title(lv_obj_t * win, const char * title);
void lv_win_set_header_height(lv_obj_t * win, lv_coord_t size);
void lv_win_set_btn_width(lv_obj_t * win, lv_coord_t width);
void lv_win_set_content_size(lv_obj_t * win, lv_coord_t w, lv_coord_t h);
void lv_win_set_layout(lv_obj_t * win, lv_layout_t layout);
void lv_win_set_scrollbar_mode(lv_obj_t * win, lv_scrollbar_mode_t sb_mode);
void lv_win_set_anim_time(lv_obj_t * win, uint16_t anim_time);
void lv_win_set_drag(lv_obj_t * win, bool en);
void lv_win_title_set_alignment(lv_obj_t * win, uint8_t alignment);
const char * lv_win_get_title(const lv_obj_t * win);
lv_obj_t * lv_win_get_content(const lv_obj_t * win);
lv_coord_t lv_win_get_header_height(const lv_obj_t * win);
lv_coord_t lv_win_get_btn_width(lv_obj_t * win);
lv_obj_t * lv_win_get_from_btn(const lv_obj_t * ctrl_btn);
lv_layout_t lv_win_get_layout(lv_obj_t * win);
lv_scrollbar_mode_t lv_win_get_sb_mode(lv_obj_t * win);
uint16_t lv_win_get_anim_time(const lv_obj_t * win);
lv_coord_t lv_win_get_width(lv_obj_t * win);
static inline bool lv_win_get_drag(const lv_obj_t * win)
uint8_t lv_win_title_get_alignment(lv_obj_t * win);
void lv_win_focus(lv_obj_t * win, lv_obj_t * obj, lv_anim_enable_t anim_en);
static inline void lv_win_scroll_hor(lv_obj_t * win, lv_coord_t dist)
static inline void lv_win_scroll_ver(lv_obj_t * win, lv_coord_t dist)

