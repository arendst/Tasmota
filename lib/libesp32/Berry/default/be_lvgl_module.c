/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
/********************************************************************
 * LVGL Module
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"
#include "be_lvgl.h"

extern int lv0_member(bvm *vm);     // resolve virtual members

extern int lv0_start(bvm *vm);

extern int lv0_register_button_encoder(bvm *vm);  // add buttons with encoder logic

extern int lv0_load_montserrat_font(bvm *vm);
extern int lv0_load_seg7_font(bvm *vm);
extern int lv0_load_font(bvm *vm);
extern int lv0_load_freetype_font(bvm *vm);

extern int lv0_screenshot(bvm *vm);

static int lv_get_hor_res(void) {
  return lv_disp_get_hor_res(lv_disp_get_default());
}
static int lv_get_ver_res(void) {
  return lv_disp_get_ver_res(lv_disp_get_default());
}

/* `lv` methods */
const lvbe_call_c_t lv_func[] = {

  { "clamp_height", (void*) &lv_clamp_height, "i", "iiii" },
  { "clamp_width", (void*) &lv_clamp_width, "i", "iiii" },
  { "color_mix", (void*) &lv_color_mix, "lv_color", "(lv_color)(lv_color)i" },
  { "dpx", (void*) &lv_dpx, "i", "i" },
  { "draw_arc", (void*) &lv_draw_arc, "", "iiiii(lv_area)(lv_draw_arc_dsc)" },
  { "draw_arc_dsc_init", (void*) &lv_draw_arc_dsc_init, "", "(lv_draw_arc_dsc)" },
  { "draw_arc_get_area", (void*) &lv_draw_arc_get_area, "", "iiiiiib(lv_area)" },
  { "draw_img", (void*) &lv_draw_img, "", "(lv_area)(lv_area).(lv_draw_img_dsc)" },
  { "draw_img_dsc_init", (void*) &lv_draw_img_dsc_init, "", "(lv_draw_img_dsc)" },
  { "draw_label", (void*) &lv_draw_label, "", "(lv_area)(lv_area)(lv_draw_label_dsc)s(lv_draw_label_hint)" },
  { "draw_label_dsc_init", (void*) &lv_draw_label_dsc_init, "", "(lv_draw_label_dsc)" },
  { "draw_letter", (void*) &lv_draw_letter, "", "(lv_point)(lv_area)(lv_font)i(lv_color)ii" },
  { "draw_line", (void*) &lv_draw_line, "", "(lv_point)(lv_point)(lv_area)(lv_draw_line_dsc)" },
  { "draw_line_dsc_init", (void*) &lv_draw_line_dsc_init, "", "(lv_draw_line_dsc)" },
  { "draw_mask_add", (void*) &lv_draw_mask_add, "i", ".." },
  { "draw_mask_angle_init", (void*) &lv_draw_mask_angle_init, "", "(lv_draw_mask_angle_param)iiii" },
  { "draw_mask_fade_init", (void*) &lv_draw_mask_fade_init, "", "(lv_draw_mask_fade_param)(lv_area)iiii" },
  { "draw_mask_get_cnt", (void*) &lv_draw_mask_get_cnt, "i", "" },
  { "draw_mask_line_angle_init", (void*) &lv_draw_mask_line_angle_init, "", "(lv_draw_mask_line_param)iiii" },
  { "draw_mask_line_points_init", (void*) &lv_draw_mask_line_points_init, "", "(lv_draw_mask_line_param)iiiii" },
  { "draw_mask_map_init", (void*) &lv_draw_mask_map_init, "", "(lv_draw_mask_map_param)(lv_area)(lv_opa)" },
  { "draw_mask_radius_init", (void*) &lv_draw_mask_radius_init, "", "(lv_draw_mask_radius_param)(lv_area)ib" },
  { "draw_mask_remove_custom", (void*) &lv_draw_mask_remove_custom, ".", "." },
  { "draw_mask_remove_id", (void*) &lv_draw_mask_remove_id, ".", "i" },
  { "draw_polygon", (void*) &lv_draw_polygon, "", "ii(lv_area)(lv_draw_rect_dsc)" },
  { "draw_rect", (void*) &lv_draw_rect, "", "(lv_area)(lv_area)(lv_draw_rect_dsc)" },
  { "draw_rect_dsc_init", (void*) &lv_draw_rect_dsc_init, "", "(lv_draw_rect_dsc)" },
  { "draw_triangle", (void*) &lv_draw_triangle, "", "i(lv_area)(lv_draw_rect_dsc)" },
  { "event_register_id", (void*) &lv_event_register_id, "i", "" },
  { "event_send", (void*) &lv_event_send, "i", "(lv_obj)i." },
  { "event_set_cover_res", (void*) &lv_event_set_cover_res, "", "(lv_event)(lv_cover_res)" },
  { "event_set_ext_draw_size", (void*) &lv_event_set_ext_draw_size, "", "(lv_event)i" },
  { "get_hor_res", (void*) &lv_get_hor_res, "i", "" },
  { "get_ver_res", (void*) &lv_get_ver_res, "i", "" },
  { "group_get_default", (void*) &lv_group_get_default, "lv_group", "" },
  { "img_src_get_type", (void*) &lv_img_src_get_type, "i", "." },
  { "indev_get_obj_act", (void*) &lv_indev_get_obj_act, "lv_obj", "" },
  { "indev_read_timer_cb", (void*) &lv_indev_read_timer_cb, "", "(lv_timer)" },
  { "layer_sys", (void*) &lv_layer_sys, "lv_obj", "" },
  { "layer_top", (void*) &lv_layer_top, "lv_obj", "" },
  { "layout_register", (void*) &lv_layout_register, "i", "^lv_layout_update_cb^." },
  { "obj_class_create_obj", (void*) &lv_obj_class_create_obj, "lv_obj", "(_lv_obj_class)(lv_obj)" },
  { "obj_del_anim_ready_cb", (void*) &lv_obj_del_anim_ready_cb, "", "(lv_anim)" },
  { "obj_draw_dsc_init", (void*) &lv_obj_draw_dsc_init, "", "(lv_obj_draw_part_dsc)(lv_area)" },
  { "obj_enable_style_refresh", (void*) &lv_obj_enable_style_refresh, "", "b" },
  { "obj_event_base", (void*) &lv_obj_event_base, "i", "(lv_obj_class)(lv_event)" },
  { "obj_report_style_change", (void*) &lv_obj_report_style_change, "", "(lv_style)" },
  { "obj_style_get_selector_part", (void*) &lv_obj_style_get_selector_part, "i", "(lv_style_selector)" },
  { "obj_style_get_selector_state", (void*) &lv_obj_style_get_selector_state, "i", "(lv_style_selector)" },
  { "refr_now", (void*) &lv_refr_now, "", "(lv_disp)" },
  { "scr_act", (void*) &lv_scr_act, "lv_obj", "" },
  { "scr_load", (void*) &lv_scr_load, "", "(lv_obj)" },
  { "scr_load_anim", (void*) &lv_scr_load_anim, "", "(lv_obj)(lv_scr_load_anim)iib" },
  { "theme_apply", (void*) &lv_theme_apply, "", "(lv_obj)" },
  { "theme_get_color_primary", (void*) &lv_theme_get_color_primary, "lv_color", "(lv_obj)" },
  { "theme_get_color_secondary", (void*) &lv_theme_get_color_secondary, "lv_color", "(lv_obj)" },
  { "theme_get_font_large", (void*) &lv_theme_get_font_large, "lv_font", "(lv_obj)" },
  { "theme_get_font_normal", (void*) &lv_theme_get_font_normal, "lv_font", "(lv_obj)" },
  { "theme_get_font_small", (void*) &lv_theme_get_font_small, "lv_font", "(lv_obj)" },
  { "theme_set_apply_cb", (void*) &lv_theme_set_apply_cb, "", "(lv_theme)^lv_theme_apply_cb^" },
  { "theme_set_parent", (void*) &lv_theme_set_parent, "", "(lv_theme)(lv_theme)" },

};
const size_t lv_func_size = sizeof(lv_func) / sizeof(lv_func[0]);




typedef struct be_constint_t {
    const char * name;
    int32_t      value;
} be_constint_t;

const be_constint_t lv0_constants[] = {

    { "ALIGN_BOTTOM_LEFT", LV_ALIGN_BOTTOM_LEFT },
    { "ALIGN_BOTTOM_MID", LV_ALIGN_BOTTOM_MID },
    { "ALIGN_BOTTOM_RIGHT", LV_ALIGN_BOTTOM_RIGHT },
    { "ALIGN_CENTER", LV_ALIGN_CENTER },
    { "ALIGN_DEFAULT", LV_ALIGN_DEFAULT },
    { "ALIGN_LEFT_MID", LV_ALIGN_LEFT_MID },
    { "ALIGN_OUT_BOTTOM_LEFT", LV_ALIGN_OUT_BOTTOM_LEFT },
    { "ALIGN_OUT_BOTTOM_MID", LV_ALIGN_OUT_BOTTOM_MID },
    { "ALIGN_OUT_BOTTOM_RIGHT", LV_ALIGN_OUT_BOTTOM_RIGHT },
    { "ALIGN_OUT_LEFT_BOTTOM", LV_ALIGN_OUT_LEFT_BOTTOM },
    { "ALIGN_OUT_LEFT_MID", LV_ALIGN_OUT_LEFT_MID },
    { "ALIGN_OUT_LEFT_TOP", LV_ALIGN_OUT_LEFT_TOP },
    { "ALIGN_OUT_RIGHT_BOTTOM", LV_ALIGN_OUT_RIGHT_BOTTOM },
    { "ALIGN_OUT_RIGHT_MID", LV_ALIGN_OUT_RIGHT_MID },
    { "ALIGN_OUT_RIGHT_TOP", LV_ALIGN_OUT_RIGHT_TOP },
    { "ALIGN_OUT_TOP_LEFT", LV_ALIGN_OUT_TOP_LEFT },
    { "ALIGN_OUT_TOP_MID", LV_ALIGN_OUT_TOP_MID },
    { "ALIGN_OUT_TOP_RIGHT", LV_ALIGN_OUT_TOP_RIGHT },
    { "ALIGN_RIGHT_MID", LV_ALIGN_RIGHT_MID },
    { "ALIGN_TOP_LEFT", LV_ALIGN_TOP_LEFT },
    { "ALIGN_TOP_MID", LV_ALIGN_TOP_MID },
    { "ALIGN_TOP_RIGHT", LV_ALIGN_TOP_RIGHT },
    { "ANIM_IMG_PART_MAIN", LV_ANIM_IMG_PART_MAIN },
    { "ANIM_OFF", LV_ANIM_OFF },
    { "ANIM_ON", LV_ANIM_ON },
    { "ARC_MODE_NORMAL", LV_ARC_MODE_NORMAL },
    { "ARC_MODE_REVERSE", LV_ARC_MODE_REVERSE },
    { "ARC_MODE_SYMMETRICAL", LV_ARC_MODE_SYMMETRICAL },
    { "BAR_MODE_NORMAL", LV_BAR_MODE_NORMAL },
    { "BAR_MODE_RANGE", LV_BAR_MODE_RANGE },
    { "BAR_MODE_SYMMETRICAL", LV_BAR_MODE_SYMMETRICAL },
    { "BASE_DIR_AUTO", LV_BASE_DIR_AUTO },
    { "BASE_DIR_LTR", LV_BASE_DIR_LTR },
    { "BASE_DIR_NEUTRAL", LV_BASE_DIR_NEUTRAL },
    { "BASE_DIR_RTL", LV_BASE_DIR_RTL },
    { "BASE_DIR_WEAK", LV_BASE_DIR_WEAK },
    { "BLEND_MODE_ADDITIVE", LV_BLEND_MODE_ADDITIVE },
    { "BLEND_MODE_NORMAL", LV_BLEND_MODE_NORMAL },
    { "BLEND_MODE_SUBTRACTIVE", LV_BLEND_MODE_SUBTRACTIVE },
    { "BORDER_SIDE_BOTTOM", LV_BORDER_SIDE_BOTTOM },
    { "BORDER_SIDE_FULL", LV_BORDER_SIDE_FULL },
    { "BORDER_SIDE_INTERNAL", LV_BORDER_SIDE_INTERNAL },
    { "BORDER_SIDE_LEFT", LV_BORDER_SIDE_LEFT },
    { "BORDER_SIDE_NONE", LV_BORDER_SIDE_NONE },
    { "BORDER_SIDE_RIGHT", LV_BORDER_SIDE_RIGHT },
    { "BORDER_SIDE_TOP", LV_BORDER_SIDE_TOP },
    { "BTNMATRIX_CTRL_CHECKABLE", LV_BTNMATRIX_CTRL_CHECKABLE },
    { "BTNMATRIX_CTRL_CHECKED", LV_BTNMATRIX_CTRL_CHECKED },
    { "BTNMATRIX_CTRL_CLICK_TRIG", LV_BTNMATRIX_CTRL_CLICK_TRIG },
    { "BTNMATRIX_CTRL_CUSTOM_1", LV_BTNMATRIX_CTRL_CUSTOM_1 },
    { "BTNMATRIX_CTRL_CUSTOM_2", LV_BTNMATRIX_CTRL_CUSTOM_2 },
    { "BTNMATRIX_CTRL_DISABLED", LV_BTNMATRIX_CTRL_DISABLED },
    { "BTNMATRIX_CTRL_HIDDEN", LV_BTNMATRIX_CTRL_HIDDEN },
    { "BTNMATRIX_CTRL_NO_REPEAT", LV_BTNMATRIX_CTRL_NO_REPEAT },
    { "BTNMATRIX_CTRL_RECOLOR", LV_BTNMATRIX_CTRL_RECOLOR },
    { "CHART_AXIS_PRIMARY_X", LV_CHART_AXIS_PRIMARY_X },
    { "CHART_AXIS_PRIMARY_Y", LV_CHART_AXIS_PRIMARY_Y },
    { "CHART_AXIS_SECONDARY_X", LV_CHART_AXIS_SECONDARY_X },
    { "CHART_AXIS_SECONDARY_Y", LV_CHART_AXIS_SECONDARY_Y },
    { "CHART_TYPE_BAR", LV_CHART_TYPE_BAR },
    { "CHART_TYPE_LINE", LV_CHART_TYPE_LINE },
    { "CHART_TYPE_NONE", LV_CHART_TYPE_NONE },
    { "CHART_TYPE_SCATTER", LV_CHART_TYPE_SCATTER },
    { "CHART_UPDATE_MODE_CIRCULAR", LV_CHART_UPDATE_MODE_CIRCULAR },
    { "CHART_UPDATE_MODE_SHIFT", LV_CHART_UPDATE_MODE_SHIFT },
    { "COLORWHEEL_MODE_HUE", LV_COLORWHEEL_MODE_HUE },
    { "COLORWHEEL_MODE_SATURATION", LV_COLORWHEEL_MODE_SATURATION },
    { "COLORWHEEL_MODE_VALUE", LV_COLORWHEEL_MODE_VALUE },
    { "COLOR_AQUA", 65535 },
    { "COLOR_BLACK", 0 },
    { "COLOR_BLUE", 255 },
    { "COLOR_CYAN", 65535 },
    { "COLOR_GRAY", 8421504 },
    { "COLOR_GREEN", 32768 },
    { "COLOR_LIME", 65280 },
    { "COLOR_MAGENTA", 16711935 },
    { "COLOR_MAROON", 8388608 },
    { "COLOR_NAVY", 128 },
    { "COLOR_OLIVE", 8421376 },
    { "COLOR_PURPLE", 8388736 },
    { "COLOR_RED", 16711680 },
    { "COLOR_SILVER", 12632256 },
    { "COLOR_TEAL", 32896 },
    { "COLOR_WHITE", 16777215 },
    { "COLOR_YELLOW", 16776960 },
    { "COVER_RES_COVER", LV_COVER_RES_COVER },
    { "COVER_RES_MASKED", LV_COVER_RES_MASKED },
    { "COVER_RES_NOT_COVER", LV_COVER_RES_NOT_COVER },
    { "DIR_ALL", LV_DIR_ALL },
    { "DIR_BOTTOM", LV_DIR_BOTTOM },
    { "DIR_HOR", LV_DIR_HOR },
    { "DIR_LEFT", LV_DIR_LEFT },
    { "DIR_NONE", LV_DIR_NONE },
    { "DIR_RIGHT", LV_DIR_RIGHT },
    { "DIR_TOP", LV_DIR_TOP },
    { "DIR_VER", LV_DIR_VER },
    { "DISP_ROT_180", LV_DISP_ROT_180 },
    { "DISP_ROT_270", LV_DISP_ROT_270 },
    { "DISP_ROT_90", LV_DISP_ROT_90 },
    { "DISP_ROT_NONE", LV_DISP_ROT_NONE },
    { "DRAW_MASK_LINE_SIDE_BOTTOM", LV_DRAW_MASK_LINE_SIDE_BOTTOM },
    { "DRAW_MASK_LINE_SIDE_LEFT", LV_DRAW_MASK_LINE_SIDE_LEFT },
    { "DRAW_MASK_LINE_SIDE_RIGHT", LV_DRAW_MASK_LINE_SIDE_RIGHT },
    { "DRAW_MASK_LINE_SIDE_TOP", LV_DRAW_MASK_LINE_SIDE_TOP },
    { "DRAW_MASK_RES_CHANGED", LV_DRAW_MASK_RES_CHANGED },
    { "DRAW_MASK_RES_FULL_COVER", LV_DRAW_MASK_RES_FULL_COVER },
    { "DRAW_MASK_RES_TRANSP", LV_DRAW_MASK_RES_TRANSP },
    { "DRAW_MASK_RES_UNKNOWN", LV_DRAW_MASK_RES_UNKNOWN },
    { "DRAW_MASK_TYPE_ANGLE", LV_DRAW_MASK_TYPE_ANGLE },
    { "DRAW_MASK_TYPE_FADE", LV_DRAW_MASK_TYPE_FADE },
    { "DRAW_MASK_TYPE_LINE", LV_DRAW_MASK_TYPE_LINE },
    { "DRAW_MASK_TYPE_MAP", LV_DRAW_MASK_TYPE_MAP },
    { "DRAW_MASK_TYPE_RADIUS", LV_DRAW_MASK_TYPE_RADIUS },
    { "EVENT_ALL", LV_EVENT_ALL },
    { "EVENT_CANCEL", LV_EVENT_CANCEL },
    { "EVENT_CHILD_CHANGED", LV_EVENT_CHILD_CHANGED },
    { "EVENT_CLICKED", LV_EVENT_CLICKED },
    { "EVENT_COVER_CHECK", LV_EVENT_COVER_CHECK },
    { "EVENT_DEFOCUSED", LV_EVENT_DEFOCUSED },
    { "EVENT_DELETE", LV_EVENT_DELETE },
    { "EVENT_DRAW_MAIN", LV_EVENT_DRAW_MAIN },
    { "EVENT_DRAW_MAIN_BEGIN", LV_EVENT_DRAW_MAIN_BEGIN },
    { "EVENT_DRAW_MAIN_END", LV_EVENT_DRAW_MAIN_END },
    { "EVENT_DRAW_PART_BEGIN", LV_EVENT_DRAW_PART_BEGIN },
    { "EVENT_DRAW_PART_END", LV_EVENT_DRAW_PART_END },
    { "EVENT_DRAW_POST", LV_EVENT_DRAW_POST },
    { "EVENT_DRAW_POST_BEGIN", LV_EVENT_DRAW_POST_BEGIN },
    { "EVENT_DRAW_POST_END", LV_EVENT_DRAW_POST_END },
    { "EVENT_FOCUSED", LV_EVENT_FOCUSED },
    { "EVENT_GESTURE", LV_EVENT_GESTURE },
    { "EVENT_GET_SELF_SIZE", LV_EVENT_GET_SELF_SIZE },
    { "EVENT_HIT_TEST", LV_EVENT_HIT_TEST },
    { "EVENT_INSERT", LV_EVENT_INSERT },
    { "EVENT_KEY", LV_EVENT_KEY },
    { "EVENT_LAYOUT_CHANGED", LV_EVENT_LAYOUT_CHANGED },
    { "EVENT_LEAVE", LV_EVENT_LEAVE },
    { "EVENT_LONG_PRESSED", LV_EVENT_LONG_PRESSED },
    { "EVENT_LONG_PRESSED_REPEAT", LV_EVENT_LONG_PRESSED_REPEAT },
    { "EVENT_PRESSED", LV_EVENT_PRESSED },
    { "EVENT_PRESSING", LV_EVENT_PRESSING },
    { "EVENT_PRESS_LOST", LV_EVENT_PRESS_LOST },
    { "EVENT_READY", LV_EVENT_READY },
    { "EVENT_REFRESH", LV_EVENT_REFRESH },
    { "EVENT_REFR_EXT_DRAW_SIZE", LV_EVENT_REFR_EXT_DRAW_SIZE },
    { "EVENT_RELEASED", LV_EVENT_RELEASED },
    { "EVENT_SCROLL", LV_EVENT_SCROLL },
    { "EVENT_SCROLL_BEGIN", LV_EVENT_SCROLL_BEGIN },
    { "EVENT_SCROLL_END", LV_EVENT_SCROLL_END },
    { "EVENT_SHORT_CLICKED", LV_EVENT_SHORT_CLICKED },
    { "EVENT_SIZE_CHANGED", LV_EVENT_SIZE_CHANGED },
    { "EVENT_STYLE_CHANGED", LV_EVENT_STYLE_CHANGED },
    { "EVENT_VALUE_CHANGED", LV_EVENT_VALUE_CHANGED },
    { "FLEX_ALIGN_CENTER", LV_FLEX_ALIGN_CENTER },
    { "FLEX_ALIGN_END", LV_FLEX_ALIGN_END },
    { "FLEX_ALIGN_SPACE_AROUND", LV_FLEX_ALIGN_SPACE_AROUND },
    { "FLEX_ALIGN_SPACE_BETWEEN", LV_FLEX_ALIGN_SPACE_BETWEEN },
    { "FLEX_ALIGN_SPACE_EVENLY", LV_FLEX_ALIGN_SPACE_EVENLY },
    { "FLEX_ALIGN_START", LV_FLEX_ALIGN_START },
    { "FLEX_FLOW_COLUMN", LV_FLEX_FLOW_COLUMN },
    { "FLEX_FLOW_COLUMN_REVERSE", LV_FLEX_FLOW_COLUMN_REVERSE },
    { "FLEX_FLOW_COLUMN_WRAP", LV_FLEX_FLOW_COLUMN_WRAP },
    { "FLEX_FLOW_COLUMN_WRAP_REVERSE", LV_FLEX_FLOW_COLUMN_WRAP_REVERSE },
    { "FLEX_FLOW_ROW", LV_FLEX_FLOW_ROW },
    { "FLEX_FLOW_ROW_REVERSE", LV_FLEX_FLOW_ROW_REVERSE },
    { "FLEX_FLOW_ROW_WRAP", LV_FLEX_FLOW_ROW_WRAP },
    { "FLEX_FLOW_ROW_WRAP_REVERSE", LV_FLEX_FLOW_ROW_WRAP_REVERSE },
    { "FS_MODE_RD", LV_FS_MODE_RD },
    { "FS_MODE_WR", LV_FS_MODE_WR },
    { "FS_RES_BUSY", LV_FS_RES_BUSY },
    { "FS_RES_DENIED", LV_FS_RES_DENIED },
    { "FS_RES_FS_ERR", LV_FS_RES_FS_ERR },
    { "FS_RES_FULL", LV_FS_RES_FULL },
    { "FS_RES_HW_ERR", LV_FS_RES_HW_ERR },
    { "FS_RES_INV_PARAM", LV_FS_RES_INV_PARAM },
    { "FS_RES_LOCKED", LV_FS_RES_LOCKED },
    { "FS_RES_NOT_EX", LV_FS_RES_NOT_EX },
    { "FS_RES_NOT_IMP", LV_FS_RES_NOT_IMP },
    { "FS_RES_OK", LV_FS_RES_OK },
    { "FS_RES_OUT_OF_MEM", LV_FS_RES_OUT_OF_MEM },
    { "FS_RES_TOUT", LV_FS_RES_TOUT },
    { "FS_RES_UNKNOWN", LV_FS_RES_UNKNOWN },
    { "FS_SEEK_CUR", LV_FS_SEEK_CUR },
    { "FS_SEEK_END", LV_FS_SEEK_END },
    { "FS_SEEK_SET", LV_FS_SEEK_SET },
    { "GRAD_DIR_HOR", LV_GRAD_DIR_HOR },
    { "GRAD_DIR_NONE", LV_GRAD_DIR_NONE },
    { "GRAD_DIR_VER", LV_GRAD_DIR_VER },
    { "GRID_ALIGN_CENTER", LV_GRID_ALIGN_CENTER },
    { "GRID_ALIGN_END", LV_GRID_ALIGN_END },
    { "GRID_ALIGN_SPACE_AROUND", LV_GRID_ALIGN_SPACE_AROUND },
    { "GRID_ALIGN_SPACE_BETWEEN", LV_GRID_ALIGN_SPACE_BETWEEN },
    { "GRID_ALIGN_SPACE_EVENLY", LV_GRID_ALIGN_SPACE_EVENLY },
    { "GRID_ALIGN_START", LV_GRID_ALIGN_START },
    { "GRID_ALIGN_STRETCH", LV_GRID_ALIGN_STRETCH },
    { "GROUP_REFOCUS_POLICY_NEXT", LV_GROUP_REFOCUS_POLICY_NEXT },
    { "GROUP_REFOCUS_POLICY_PREV", LV_GROUP_REFOCUS_POLICY_PREV },
    { "IMGBTN_STATE_CHECKED_DISABLED", LV_IMGBTN_STATE_CHECKED_DISABLED },
    { "IMGBTN_STATE_CHECKED_PRESSED", LV_IMGBTN_STATE_CHECKED_PRESSED },
    { "IMGBTN_STATE_CHECKED_RELEASED", LV_IMGBTN_STATE_CHECKED_RELEASED },
    { "IMGBTN_STATE_DISABLED", LV_IMGBTN_STATE_DISABLED },
    { "IMGBTN_STATE_PRESSED", LV_IMGBTN_STATE_PRESSED },
    { "IMGBTN_STATE_RELEASED", LV_IMGBTN_STATE_RELEASED },
    { "IMG_CF_ALPHA_1BIT", LV_IMG_CF_ALPHA_1BIT },
    { "IMG_CF_ALPHA_2BIT", LV_IMG_CF_ALPHA_2BIT },
    { "IMG_CF_ALPHA_4BIT", LV_IMG_CF_ALPHA_4BIT },
    { "IMG_CF_ALPHA_8BIT", LV_IMG_CF_ALPHA_8BIT },
    { "IMG_CF_INDEXED_1BIT", LV_IMG_CF_INDEXED_1BIT },
    { "IMG_CF_INDEXED_2BIT", LV_IMG_CF_INDEXED_2BIT },
    { "IMG_CF_INDEXED_4BIT", LV_IMG_CF_INDEXED_4BIT },
    { "IMG_CF_INDEXED_8BIT", LV_IMG_CF_INDEXED_8BIT },
    { "IMG_CF_RAW", LV_IMG_CF_RAW },
    { "IMG_CF_RAW_ALPHA", LV_IMG_CF_RAW_ALPHA },
    { "IMG_CF_RAW_CHROMA_KEYED", LV_IMG_CF_RAW_CHROMA_KEYED },
    { "IMG_CF_TRUE_COLOR", LV_IMG_CF_TRUE_COLOR },
    { "IMG_CF_TRUE_COLOR_ALPHA", LV_IMG_CF_TRUE_COLOR_ALPHA },
    { "IMG_CF_TRUE_COLOR_CHROMA_KEYED", LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED },
    { "IMG_CF_UNKNOWN", LV_IMG_CF_UNKNOWN },
    { "IMG_SRC_FILE", LV_IMG_SRC_FILE },
    { "IMG_SRC_SYMBOL", LV_IMG_SRC_SYMBOL },
    { "IMG_SRC_UNKNOWN", LV_IMG_SRC_UNKNOWN },
    { "IMG_SRC_VARIABLE", LV_IMG_SRC_VARIABLE },
    { "INDEV_STATE_PRESSED", LV_INDEV_STATE_PRESSED },
    { "INDEV_STATE_RELEASED", LV_INDEV_STATE_RELEASED },
    { "INDEV_TYPE_BUTTON", LV_INDEV_TYPE_BUTTON },
    { "INDEV_TYPE_ENCODER", LV_INDEV_TYPE_ENCODER },
    { "INDEV_TYPE_KEYPAD", LV_INDEV_TYPE_KEYPAD },
    { "INDEV_TYPE_NONE", LV_INDEV_TYPE_NONE },
    { "INDEV_TYPE_POINTER", LV_INDEV_TYPE_POINTER },
    { "KEY_BACKSPACE", LV_KEY_BACKSPACE },
    { "KEY_DEL", LV_KEY_DEL },
    { "KEY_DOWN", LV_KEY_DOWN },
    { "KEY_END", LV_KEY_END },
    { "KEY_ENTER", LV_KEY_ENTER },
    { "KEY_ESC", LV_KEY_ESC },
    { "KEY_HOME", LV_KEY_HOME },
    { "KEY_LEFT", LV_KEY_LEFT },
    { "KEY_NEXT", LV_KEY_NEXT },
    { "KEY_PREV", LV_KEY_PREV },
    { "KEY_RIGHT", LV_KEY_RIGHT },
    { "KEY_UP", LV_KEY_UP },
    { "LABEL_LONG_CLIP", LV_LABEL_LONG_CLIP },
    { "LABEL_LONG_DOT", LV_LABEL_LONG_DOT },
    { "LABEL_LONG_SCROLL", LV_LABEL_LONG_SCROLL },
    { "LABEL_LONG_SCROLL_CIRCULAR", LV_LABEL_LONG_SCROLL_CIRCULAR },
    { "LABEL_LONG_WRAP", LV_LABEL_LONG_WRAP },
    { "METER_INDICATOR_TYPE_ARC", LV_METER_INDICATOR_TYPE_ARC },
    { "METER_INDICATOR_TYPE_NEEDLE_IMG", LV_METER_INDICATOR_TYPE_NEEDLE_IMG },
    { "METER_INDICATOR_TYPE_NEEDLE_LINE", LV_METER_INDICATOR_TYPE_NEEDLE_LINE },
    { "METER_INDICATOR_TYPE_SCALE_LINES", LV_METER_INDICATOR_TYPE_SCALE_LINES },
    { "OBJ_CLASS_EDITABLE_FALSE", LV_OBJ_CLASS_EDITABLE_FALSE },
    { "OBJ_CLASS_EDITABLE_INHERIT", LV_OBJ_CLASS_EDITABLE_INHERIT },
    { "OBJ_CLASS_EDITABLE_TRUE", LV_OBJ_CLASS_EDITABLE_TRUE },
    { "OBJ_CLASS_GROUP_DEF_FALSE", LV_OBJ_CLASS_GROUP_DEF_FALSE },
    { "OBJ_CLASS_GROUP_DEF_INHERIT", LV_OBJ_CLASS_GROUP_DEF_INHERIT },
    { "OBJ_CLASS_GROUP_DEF_TRUE", LV_OBJ_CLASS_GROUP_DEF_TRUE },
    { "OBJ_FLAG_ADV_HITTEST", LV_OBJ_FLAG_ADV_HITTEST },
    { "OBJ_FLAG_CHECKABLE", LV_OBJ_FLAG_CHECKABLE },
    { "OBJ_FLAG_CLICKABLE", LV_OBJ_FLAG_CLICKABLE },
    { "OBJ_FLAG_CLICK_FOCUSABLE", LV_OBJ_FLAG_CLICK_FOCUSABLE },
    { "OBJ_FLAG_EVENT_BUBBLE", LV_OBJ_FLAG_EVENT_BUBBLE },
    { "OBJ_FLAG_FLOATING", LV_OBJ_FLAG_FLOATING },
    { "OBJ_FLAG_GESTURE_BUBBLE", LV_OBJ_FLAG_GESTURE_BUBBLE },
    { "OBJ_FLAG_HIDDEN", LV_OBJ_FLAG_HIDDEN },
    { "OBJ_FLAG_IGNORE_LAYOUT", LV_OBJ_FLAG_IGNORE_LAYOUT },
    { "OBJ_FLAG_LAYOUT_1", LV_OBJ_FLAG_LAYOUT_1 },
    { "OBJ_FLAG_LAYOUT_2", LV_OBJ_FLAG_LAYOUT_2 },
    { "OBJ_FLAG_PRESS_LOCK", LV_OBJ_FLAG_PRESS_LOCK },
    { "OBJ_FLAG_SCROLLABLE", LV_OBJ_FLAG_SCROLLABLE },
    { "OBJ_FLAG_SCROLL_CHAIN", LV_OBJ_FLAG_SCROLL_CHAIN },
    { "OBJ_FLAG_SCROLL_ELASTIC", LV_OBJ_FLAG_SCROLL_ELASTIC },
    { "OBJ_FLAG_SCROLL_MOMENTUM", LV_OBJ_FLAG_SCROLL_MOMENTUM },
    { "OBJ_FLAG_SCROLL_ONE", LV_OBJ_FLAG_SCROLL_ONE },
    { "OBJ_FLAG_SCROLL_ON_FOCUS", LV_OBJ_FLAG_SCROLL_ON_FOCUS },
    { "OBJ_FLAG_SNAPABLE", LV_OBJ_FLAG_SNAPABLE },
    { "OBJ_FLAG_USER_1", LV_OBJ_FLAG_USER_1 },
    { "OBJ_FLAG_USER_2", LV_OBJ_FLAG_USER_2 },
    { "OBJ_FLAG_USER_3", LV_OBJ_FLAG_USER_3 },
    { "OBJ_FLAG_USER_4", LV_OBJ_FLAG_USER_4 },
    { "OBJ_FLAG_WIDGET_1", LV_OBJ_FLAG_WIDGET_1 },
    { "OBJ_FLAG_WIDGET_2", LV_OBJ_FLAG_WIDGET_2 },
    { "OBJ_TREE_WALK_END", LV_OBJ_TREE_WALK_END },
    { "OBJ_TREE_WALK_NEXT", LV_OBJ_TREE_WALK_NEXT },
    { "OBJ_TREE_WALK_SKIP_CHILDREN", LV_OBJ_TREE_WALK_SKIP_CHILDREN },
    { "OPA_0", LV_OPA_0 },
    { "OPA_10", LV_OPA_10 },
    { "OPA_100", LV_OPA_100 },
    { "OPA_20", LV_OPA_20 },
    { "OPA_30", LV_OPA_30 },
    { "OPA_40", LV_OPA_40 },
    { "OPA_50", LV_OPA_50 },
    { "OPA_60", LV_OPA_60 },
    { "OPA_70", LV_OPA_70 },
    { "OPA_80", LV_OPA_80 },
    { "OPA_90", LV_OPA_90 },
    { "OPA_COVER", LV_OPA_COVER },
    { "OPA_TRANSP", LV_OPA_TRANSP },
    { "PALETTE_AMBER", LV_PALETTE_AMBER },
    { "PALETTE_BLUE", LV_PALETTE_BLUE },
    { "PALETTE_BLUE_GREY", LV_PALETTE_BLUE_GREY },
    { "PALETTE_BROWN", LV_PALETTE_BROWN },
    { "PALETTE_CYAN", LV_PALETTE_CYAN },
    { "PALETTE_DEEP_ORANGE", LV_PALETTE_DEEP_ORANGE },
    { "PALETTE_DEEP_PURPLE", LV_PALETTE_DEEP_PURPLE },
    { "PALETTE_GREEN", LV_PALETTE_GREEN },
    { "PALETTE_GREY", LV_PALETTE_GREY },
    { "PALETTE_INDIGO", LV_PALETTE_INDIGO },
    { "PALETTE_LIGHT_BLUE", LV_PALETTE_LIGHT_BLUE },
    { "PALETTE_LIGHT_GREEN", LV_PALETTE_LIGHT_GREEN },
    { "PALETTE_LIME", LV_PALETTE_LIME },
    { "PALETTE_NONE", LV_PALETTE_NONE },
    { "PALETTE_ORANGE", LV_PALETTE_ORANGE },
    { "PALETTE_PINK", LV_PALETTE_PINK },
    { "PALETTE_PURPLE", LV_PALETTE_PURPLE },
    { "PALETTE_RED", LV_PALETTE_RED },
    { "PALETTE_TEAL", LV_PALETTE_TEAL },
    { "PALETTE_YELLOW", LV_PALETTE_YELLOW },
    { "PART_ANY", LV_PART_ANY },
    { "PART_CURSOR", LV_PART_CURSOR },
    { "PART_CUSTOM_FIRST", LV_PART_CUSTOM_FIRST },
    { "PART_INDICATOR", LV_PART_INDICATOR },
    { "PART_ITEMS", LV_PART_ITEMS },
    { "PART_KNOB", LV_PART_KNOB },
    { "PART_MAIN", LV_PART_MAIN },
    { "PART_SCROLLBAR", LV_PART_SCROLLBAR },
    { "PART_SELECTED", LV_PART_SELECTED },
    { "PART_TEXTAREA_PLACEHOLDER", LV_PART_TEXTAREA_PLACEHOLDER },
    { "PART_TICKS", LV_PART_TICKS },
    { "RADIUS_CIRCLE", LV_RADIUS_CIRCLE },
    { "RES_INV", LV_RES_INV },
    { "RES_OK", LV_RES_OK },
    { "ROLLER_MODE_INFINITE", LV_ROLLER_MODE_INFINITE },
    { "ROLLER_MODE_NORMAL", LV_ROLLER_MODE_NORMAL },
    { "SCROLLBAR_MODE_ACTIVE", LV_SCROLLBAR_MODE_ACTIVE },
    { "SCROLLBAR_MODE_AUTO", LV_SCROLLBAR_MODE_AUTO },
    { "SCROLLBAR_MODE_OFF", LV_SCROLLBAR_MODE_OFF },
    { "SCROLLBAR_MODE_ON", LV_SCROLLBAR_MODE_ON },
    { "SCROLL_SNAP_CENTER", LV_SCROLL_SNAP_CENTER },
    { "SCROLL_SNAP_END", LV_SCROLL_SNAP_END },
    { "SCROLL_SNAP_NONE", LV_SCROLL_SNAP_NONE },
    { "SCROLL_SNAP_START", LV_SCROLL_SNAP_START },
    { "SCR_LOAD_ANIM_FADE_ON", LV_SCR_LOAD_ANIM_FADE_ON },
    { "SCR_LOAD_ANIM_MOVE_BOTTOM", LV_SCR_LOAD_ANIM_MOVE_BOTTOM },
    { "SCR_LOAD_ANIM_MOVE_LEFT", LV_SCR_LOAD_ANIM_MOVE_LEFT },
    { "SCR_LOAD_ANIM_MOVE_RIGHT", LV_SCR_LOAD_ANIM_MOVE_RIGHT },
    { "SCR_LOAD_ANIM_MOVE_TOP", LV_SCR_LOAD_ANIM_MOVE_TOP },
    { "SCR_LOAD_ANIM_NONE", LV_SCR_LOAD_ANIM_NONE },
    { "SCR_LOAD_ANIM_OVER_BOTTOM", LV_SCR_LOAD_ANIM_OVER_BOTTOM },
    { "SCR_LOAD_ANIM_OVER_LEFT", LV_SCR_LOAD_ANIM_OVER_LEFT },
    { "SCR_LOAD_ANIM_OVER_RIGHT", LV_SCR_LOAD_ANIM_OVER_RIGHT },
    { "SCR_LOAD_ANIM_OVER_TOP", LV_SCR_LOAD_ANIM_OVER_TOP },
    { "SIZE_CONTENT", LV_SIZE_CONTENT },
    { "SLIDER_MODE_NORMAL", LV_SLIDER_MODE_NORMAL },
    { "SLIDER_MODE_RANGE", LV_SLIDER_MODE_RANGE },
    { "SLIDER_MODE_SYMMETRICAL", LV_SLIDER_MODE_SYMMETRICAL },
    { "SPAN_MODE_BREAK", LV_SPAN_MODE_BREAK },
    { "SPAN_MODE_EXPAND", LV_SPAN_MODE_EXPAND },
    { "SPAN_MODE_FIXED", LV_SPAN_MODE_FIXED },
    { "SPAN_OVERFLOW_CLIP", LV_SPAN_OVERFLOW_CLIP },
    { "SPAN_OVERFLOW_ELLIPSIS", LV_SPAN_OVERFLOW_ELLIPSIS },
    { "STATE_ANY", LV_STATE_ANY },
    { "STATE_CHECKED", LV_STATE_CHECKED },
    { "STATE_DEFAULT", LV_STATE_DEFAULT },
    { "STATE_DISABLED", LV_STATE_DISABLED },
    { "STATE_EDITED", LV_STATE_EDITED },
    { "STATE_FOCUSED", LV_STATE_FOCUSED },
    { "STATE_FOCUS_KEY", LV_STATE_FOCUS_KEY },
    { "STATE_HOVERED", LV_STATE_HOVERED },
    { "STATE_PRESSED", LV_STATE_PRESSED },
    { "STATE_SCROLLED", LV_STATE_SCROLLED },
    { "STATE_USER_1", LV_STATE_USER_1 },
    { "STATE_USER_2", LV_STATE_USER_2 },
    { "STATE_USER_3", LV_STATE_USER_3 },
    { "STATE_USER_4", LV_STATE_USER_4 },
    { "STYLE_ALIGN", LV_STYLE_ALIGN },
    { "STYLE_ANIM_SPEED", LV_STYLE_ANIM_SPEED },
    { "STYLE_ANIM_TIME", LV_STYLE_ANIM_TIME },
    { "STYLE_ARC_COLOR", LV_STYLE_ARC_COLOR },
    { "STYLE_ARC_COLOR_FILTERED", LV_STYLE_ARC_COLOR_FILTERED },
    { "STYLE_ARC_IMG_SRC", LV_STYLE_ARC_IMG_SRC },
    { "STYLE_ARC_OPA", LV_STYLE_ARC_OPA },
    { "STYLE_ARC_ROUNDED", LV_STYLE_ARC_ROUNDED },
    { "STYLE_ARC_WIDTH", LV_STYLE_ARC_WIDTH },
    { "STYLE_BASE_DIR", LV_STYLE_BASE_DIR },
    { "STYLE_BG_COLOR", LV_STYLE_BG_COLOR },
    { "STYLE_BG_COLOR_FILTERED", LV_STYLE_BG_COLOR_FILTERED },
    { "STYLE_BG_GRAD_COLOR", LV_STYLE_BG_GRAD_COLOR },
    { "STYLE_BG_GRAD_COLOR_FILTERED", LV_STYLE_BG_GRAD_COLOR_FILTERED },
    { "STYLE_BG_GRAD_DIR", LV_STYLE_BG_GRAD_DIR },
    { "STYLE_BG_GRAD_STOP", LV_STYLE_BG_GRAD_STOP },
    { "STYLE_BG_IMG_OPA", LV_STYLE_BG_IMG_OPA },
    { "STYLE_BG_IMG_RECOLOR", LV_STYLE_BG_IMG_RECOLOR },
    { "STYLE_BG_IMG_RECOLOR_FILTERED", LV_STYLE_BG_IMG_RECOLOR_FILTERED },
    { "STYLE_BG_IMG_RECOLOR_OPA", LV_STYLE_BG_IMG_RECOLOR_OPA },
    { "STYLE_BG_IMG_SRC", LV_STYLE_BG_IMG_SRC },
    { "STYLE_BG_IMG_TILED", LV_STYLE_BG_IMG_TILED },
    { "STYLE_BG_MAIN_STOP", LV_STYLE_BG_MAIN_STOP },
    { "STYLE_BG_OPA", LV_STYLE_BG_OPA },
    { "STYLE_BLEND_MODE", LV_STYLE_BLEND_MODE },
    { "STYLE_BORDER_COLOR", LV_STYLE_BORDER_COLOR },
    { "STYLE_BORDER_COLOR_FILTERED", LV_STYLE_BORDER_COLOR_FILTERED },
    { "STYLE_BORDER_OPA", LV_STYLE_BORDER_OPA },
    { "STYLE_BORDER_POST", LV_STYLE_BORDER_POST },
    { "STYLE_BORDER_SIDE", LV_STYLE_BORDER_SIDE },
    { "STYLE_BORDER_WIDTH", LV_STYLE_BORDER_WIDTH },
    { "STYLE_CLIP_CORNER", LV_STYLE_CLIP_CORNER },
    { "STYLE_COLOR_FILTER_DSC", LV_STYLE_COLOR_FILTER_DSC },
    { "STYLE_COLOR_FILTER_OPA", LV_STYLE_COLOR_FILTER_OPA },
    { "STYLE_HEIGHT", LV_STYLE_HEIGHT },
    { "STYLE_IMG_OPA", LV_STYLE_IMG_OPA },
    { "STYLE_IMG_RECOLOR", LV_STYLE_IMG_RECOLOR },
    { "STYLE_IMG_RECOLOR_FILTERED", LV_STYLE_IMG_RECOLOR_FILTERED },
    { "STYLE_IMG_RECOLOR_OPA", LV_STYLE_IMG_RECOLOR_OPA },
    { "STYLE_LAYOUT", LV_STYLE_LAYOUT },
    { "STYLE_LINE_COLOR", LV_STYLE_LINE_COLOR },
    { "STYLE_LINE_COLOR_FILTERED", LV_STYLE_LINE_COLOR_FILTERED },
    { "STYLE_LINE_DASH_GAP", LV_STYLE_LINE_DASH_GAP },
    { "STYLE_LINE_DASH_WIDTH", LV_STYLE_LINE_DASH_WIDTH },
    { "STYLE_LINE_OPA", LV_STYLE_LINE_OPA },
    { "STYLE_LINE_ROUNDED", LV_STYLE_LINE_ROUNDED },
    { "STYLE_LINE_WIDTH", LV_STYLE_LINE_WIDTH },
    { "STYLE_MAX_HEIGHT", LV_STYLE_MAX_HEIGHT },
    { "STYLE_MAX_WIDTH", LV_STYLE_MAX_WIDTH },
    { "STYLE_MIN_HEIGHT", LV_STYLE_MIN_HEIGHT },
    { "STYLE_MIN_WIDTH", LV_STYLE_MIN_WIDTH },
    { "STYLE_OPA", LV_STYLE_OPA },
    { "STYLE_OUTLINE_COLOR", LV_STYLE_OUTLINE_COLOR },
    { "STYLE_OUTLINE_COLOR_FILTERED", LV_STYLE_OUTLINE_COLOR_FILTERED },
    { "STYLE_OUTLINE_OPA", LV_STYLE_OUTLINE_OPA },
    { "STYLE_OUTLINE_PAD", LV_STYLE_OUTLINE_PAD },
    { "STYLE_OUTLINE_WIDTH", LV_STYLE_OUTLINE_WIDTH },
    { "STYLE_PAD_BOTTOM", LV_STYLE_PAD_BOTTOM },
    { "STYLE_PAD_COLUMN", LV_STYLE_PAD_COLUMN },
    { "STYLE_PAD_LEFT", LV_STYLE_PAD_LEFT },
    { "STYLE_PAD_RIGHT", LV_STYLE_PAD_RIGHT },
    { "STYLE_PAD_ROW", LV_STYLE_PAD_ROW },
    { "STYLE_PAD_TOP", LV_STYLE_PAD_TOP },
    { "STYLE_PROP_ANY", LV_STYLE_PROP_ANY },
    { "STYLE_PROP_INV", LV_STYLE_PROP_INV },
    { "STYLE_RADIUS", LV_STYLE_RADIUS },
    { "STYLE_SHADOW_COLOR", LV_STYLE_SHADOW_COLOR },
    { "STYLE_SHADOW_COLOR_FILTERED", LV_STYLE_SHADOW_COLOR_FILTERED },
    { "STYLE_SHADOW_OFS_X", LV_STYLE_SHADOW_OFS_X },
    { "STYLE_SHADOW_OFS_Y", LV_STYLE_SHADOW_OFS_Y },
    { "STYLE_SHADOW_OPA", LV_STYLE_SHADOW_OPA },
    { "STYLE_SHADOW_SPREAD", LV_STYLE_SHADOW_SPREAD },
    { "STYLE_SHADOW_WIDTH", LV_STYLE_SHADOW_WIDTH },
    { "STYLE_TEXT_ALIGN", LV_STYLE_TEXT_ALIGN },
    { "STYLE_TEXT_COLOR", LV_STYLE_TEXT_COLOR },
    { "STYLE_TEXT_COLOR_FILTERED", LV_STYLE_TEXT_COLOR_FILTERED },
    { "STYLE_TEXT_DECOR", LV_STYLE_TEXT_DECOR },
    { "STYLE_TEXT_FONT", LV_STYLE_TEXT_FONT },
    { "STYLE_TEXT_LETTER_SPACE", LV_STYLE_TEXT_LETTER_SPACE },
    { "STYLE_TEXT_LINE_SPACE", LV_STYLE_TEXT_LINE_SPACE },
    { "STYLE_TEXT_OPA", LV_STYLE_TEXT_OPA },
    { "STYLE_TRANSFORM_ANGLE", LV_STYLE_TRANSFORM_ANGLE },
    { "STYLE_TRANSFORM_HEIGHT", LV_STYLE_TRANSFORM_HEIGHT },
    { "STYLE_TRANSFORM_WIDTH", LV_STYLE_TRANSFORM_WIDTH },
    { "STYLE_TRANSFORM_ZOOM", LV_STYLE_TRANSFORM_ZOOM },
    { "STYLE_TRANSITION", LV_STYLE_TRANSITION },
    { "STYLE_TRANSLATE_X", LV_STYLE_TRANSLATE_X },
    { "STYLE_TRANSLATE_Y", LV_STYLE_TRANSLATE_Y },
    { "STYLE_WIDTH", LV_STYLE_WIDTH },
    { "STYLE_X", LV_STYLE_X },
    { "STYLE_Y", LV_STYLE_Y },
    { "TABLE_CELL_CTRL_CUSTOM_1", LV_TABLE_CELL_CTRL_CUSTOM_1 },
    { "TABLE_CELL_CTRL_CUSTOM_2", LV_TABLE_CELL_CTRL_CUSTOM_2 },
    { "TABLE_CELL_CTRL_CUSTOM_3", LV_TABLE_CELL_CTRL_CUSTOM_3 },
    { "TABLE_CELL_CTRL_CUSTOM_4", LV_TABLE_CELL_CTRL_CUSTOM_4 },
    { "TABLE_CELL_CTRL_MERGE_RIGHT", LV_TABLE_CELL_CTRL_MERGE_RIGHT },
    { "TABLE_CELL_CTRL_TEXT_CROP", LV_TABLE_CELL_CTRL_TEXT_CROP },
    { "TEXTAREA_CURSOR_LAST", LV_TEXTAREA_CURSOR_LAST },
    { "TEXT_ALIGN_AUTO", LV_TEXT_ALIGN_AUTO },
    { "TEXT_ALIGN_CENTER", LV_TEXT_ALIGN_CENTER },
    { "TEXT_ALIGN_LEFT", LV_TEXT_ALIGN_LEFT },
    { "TEXT_ALIGN_RIGHT", LV_TEXT_ALIGN_RIGHT },
    { "TEXT_CMD_STATE_IN", LV_TEXT_CMD_STATE_IN },
    { "TEXT_CMD_STATE_PAR", LV_TEXT_CMD_STATE_PAR },
    { "TEXT_CMD_STATE_WAIT", LV_TEXT_CMD_STATE_WAIT },
    { "TEXT_DECOR_NONE", LV_TEXT_DECOR_NONE },
    { "TEXT_DECOR_STRIKETHROUGH", LV_TEXT_DECOR_STRIKETHROUGH },
    { "TEXT_DECOR_UNDERLINE", LV_TEXT_DECOR_UNDERLINE },
    { "TEXT_FLAG_EXPAND", LV_TEXT_FLAG_EXPAND },
    { "TEXT_FLAG_FIT", LV_TEXT_FLAG_FIT },
    { "TEXT_FLAG_NONE", LV_TEXT_FLAG_NONE },
    { "TEXT_FLAG_RECOLOR", LV_TEXT_FLAG_RECOLOR },

};

const size_t lv0_constants_size = sizeof(lv0_constants)/sizeof(lv0_constants[0]);

be_define_local_const_str(SYMBOL_AUDIO, "\xef\x80\x81", 0, 3);
be_define_local_const_str(SYMBOL_VIDEO, "\xef\x80\x88", 0, 3);
be_define_local_const_str(SYMBOL_LIST, "\xef\x80\x8b", 0, 3);
be_define_local_const_str(SYMBOL_OK, "\xef\x80\x8c", 0, 3);
be_define_local_const_str(SYMBOL_CLOSE, "\xef\x80\x8d", 0, 3);
be_define_local_const_str(SYMBOL_POWER, "\xef\x80\x91", 0, 3);
be_define_local_const_str(SYMBOL_SETTINGS, "\xef\x80\x93", 0, 3);
be_define_local_const_str(SYMBOL_HOME, "\xef\x80\x95", 0, 3);
be_define_local_const_str(SYMBOL_DOWNLOAD, "\xef\x80\x99", 0, 3);
be_define_local_const_str(SYMBOL_DRIVE, "\xef\x80\x9c", 0, 3);
be_define_local_const_str(SYMBOL_REFRESH, "\xef\x80\xa1", 0, 3);
be_define_local_const_str(SYMBOL_MUTE, "\xef\x80\xa6", 0, 3);
be_define_local_const_str(SYMBOL_VOLUME_MID, "\xef\x80\xa7", 0, 3);
be_define_local_const_str(SYMBOL_VOLUME_MAX, "\xef\x80\xa8", 0, 3);
be_define_local_const_str(SYMBOL_IMAGE, "\xef\x80\xbe", 0, 3);
be_define_local_const_str(SYMBOL_EDIT, "\xef\x8C\x84", 0, 3);
be_define_local_const_str(SYMBOL_PREV, "\xef\x81\x88", 0, 3);
be_define_local_const_str(SYMBOL_PLAY, "\xef\x81\x8b", 0, 3);
be_define_local_const_str(SYMBOL_PAUSE, "\xef\x81\x8c", 0, 3);
be_define_local_const_str(SYMBOL_STOP, "\xef\x81\x8d", 0, 3);
be_define_local_const_str(SYMBOL_NEXT, "\xef\x81\x91", 0, 3);
be_define_local_const_str(SYMBOL_EJECT, "\xef\x81\x92", 0, 3);
be_define_local_const_str(SYMBOL_LEFT, "\xef\x81\x93", 0, 3);
be_define_local_const_str(SYMBOL_RIGHT, "\xef\x81\x94", 0, 3);
be_define_local_const_str(SYMBOL_PLUS, "\xef\x81\xa7", 0, 3);
be_define_local_const_str(SYMBOL_MINUS, "\xef\x81\xa8", 0, 3);
be_define_local_const_str(SYMBOL_EYE_OPEN, "\xef\x81\xae", 0, 3);
be_define_local_const_str(SYMBOL_EYE_CLOSE, "\xef\x81\xb0", 0, 3);
be_define_local_const_str(SYMBOL_WARNING, "\xef\x81\xb1", 0, 3);
be_define_local_const_str(SYMBOL_SHUFFLE, "\xef\x81\xb4", 0, 3);
be_define_local_const_str(SYMBOL_UP, "\xef\x81\xb7", 0, 3);
be_define_local_const_str(SYMBOL_DOWN, "\xef\x81\xb8", 0, 3);
be_define_local_const_str(SYMBOL_LOOP, "\xef\x81\xb9", 0, 3);
be_define_local_const_str(SYMBOL_DIRECTORY, "\xef\x81\xbb", 0, 3);
be_define_local_const_str(SYMBOL_UPLOAD, "\xef\x82\x93", 0, 3);
be_define_local_const_str(SYMBOL_CALL, "\xef\x82\x95", 0, 3);
be_define_local_const_str(SYMBOL_CUT, "\xef\x83\x84", 0, 3);
be_define_local_const_str(SYMBOL_COPY, "\xef\x83\x85", 0, 3);
be_define_local_const_str(SYMBOL_SAVE, "\xef\x83\x87", 0, 3);
be_define_local_const_str(SYMBOL_CHARGE, "\xef\x83\xa7", 0, 3);
be_define_local_const_str(SYMBOL_PASTE, "\xef\x83\xAA", 0, 3);
be_define_local_const_str(SYMBOL_BELL, "\xef\x83\xb3", 0, 3);
be_define_local_const_str(SYMBOL_KEYBOARD, "\xef\x84\x9c", 0, 3);
be_define_local_const_str(SYMBOL_GPS, "\xef\x84\xa4", 0, 3);
be_define_local_const_str(SYMBOL_FILE, "\xef\x85\x9b", 0, 3);
be_define_local_const_str(SYMBOL_WIFI, "\xef\x87\xab", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_FULL, "\xef\x89\x80", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_3, "\xef\x89\x81", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_2, "\xef\x89\x82", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_1, "\xef\x89\x83", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_EMPTY, "\xef\x89\x84", 0, 3);
be_define_local_const_str(SYMBOL_USB, "\xef\x8a\x87", 0, 3);
be_define_local_const_str(SYMBOL_BLUETOOTH, "\xef\x8a\x93", 0, 3);
be_define_local_const_str(SYMBOL_TRASH, "\xef\x8B\xAD", 0, 3);
be_define_local_const_str(SYMBOL_BACKSPACE, "\xef\x95\x9A", 0, 3);
be_define_local_const_str(SYMBOL_SD_CARD, "\xef\x9F\x82", 0, 3);
be_define_local_const_str(SYMBOL_NEW_LINE, "\xef\xA2\xA2", 0, 3);

be_define_local_const_str(SYMBOL_DUMMY, "\xEF\xA3\xBF", 0, 3);

be_define_local_const_str(SYMBOL_BULLET, "\xE2\x80\xA2", 0, 3);


/* @const_object_info_begin
module lvgl (scope: global) {
    SYMBOL_AUDIO, str(&be_local_const_str_SYMBOL_AUDIO)
    SYMBOL_VIDEO, str(&be_local_const_str_SYMBOL_VIDEO)
    SYMBOL_LIST, str(&be_local_const_str_SYMBOL_LIST)
    SYMBOL_OK, str(&be_local_const_str_SYMBOL_OK)
    SYMBOL_CLOSE, str(&be_local_const_str_SYMBOL_CLOSE)
    SYMBOL_POWER, str(&be_local_const_str_SYMBOL_POWER)
    SYMBOL_SETTINGS, str(&be_local_const_str_SYMBOL_SETTINGS)
    SYMBOL_HOME, str(&be_local_const_str_SYMBOL_HOME)
    SYMBOL_DOWNLOAD, str(&be_local_const_str_SYMBOL_DOWNLOAD)
    SYMBOL_DRIVE, str(&be_local_const_str_SYMBOL_DRIVE)
    SYMBOL_REFRESH, str(&be_local_const_str_SYMBOL_REFRESH)
    SYMBOL_MUTE, str(&be_local_const_str_SYMBOL_MUTE)
    SYMBOL_VOLUME_MID, str(&be_local_const_str_SYMBOL_VOLUME_MID)
    SYMBOL_VOLUME_MAX, str(&be_local_const_str_SYMBOL_VOLUME_MAX)
    SYMBOL_IMAGE, str(&be_local_const_str_SYMBOL_IMAGE)
    SYMBOL_EDIT, str(&be_local_const_str_SYMBOL_EDIT)
    SYMBOL_PREV, str(&be_local_const_str_SYMBOL_PREV)
    SYMBOL_PLAY, str(&be_local_const_str_SYMBOL_PLAY)
    SYMBOL_PAUSE, str(&be_local_const_str_SYMBOL_PAUSE)
    SYMBOL_STOP, str(&be_local_const_str_SYMBOL_STOP)
    SYMBOL_NEXT, str(&be_local_const_str_SYMBOL_NEXT)
    SYMBOL_EJECT, str(&be_local_const_str_SYMBOL_EJECT)
    SYMBOL_LEFT, str(&be_local_const_str_SYMBOL_LEFT)
    SYMBOL_RIGHT, str(&be_local_const_str_SYMBOL_RIGHT)
    SYMBOL_PLUS, str(&be_local_const_str_SYMBOL_PLUS)
    SYMBOL_MINUS, str(&be_local_const_str_SYMBOL_MINUS)
    SYMBOL_EYE_OPEN, str(&be_local_const_str_SYMBOL_EYE_OPEN)
    SYMBOL_EYE_CLOSE, str(&be_local_const_str_SYMBOL_EYE_CLOSE)
    SYMBOL_WARNING, str(&be_local_const_str_SYMBOL_WARNING)
    SYMBOL_SHUFFLE, str(&be_local_const_str_SYMBOL_SHUFFLE)
    SYMBOL_UP, str(&be_local_const_str_SYMBOL_UP)
    SYMBOL_DOWN, str(&be_local_const_str_SYMBOL_DOWN)
    SYMBOL_LOOP, str(&be_local_const_str_SYMBOL_LOOP)
    SYMBOL_DIRECTORY, str(&be_local_const_str_SYMBOL_DIRECTORY)
    SYMBOL_UPLOAD, str(&be_local_const_str_SYMBOL_UPLOAD)
    SYMBOL_CALL, str(&be_local_const_str_SYMBOL_CALL)
    SYMBOL_CUT, str(&be_local_const_str_SYMBOL_CUT)
    SYMBOL_COPY, str(&be_local_const_str_SYMBOL_COPY)
    SYMBOL_SAVE, str(&be_local_const_str_SYMBOL_SAVE)
    SYMBOL_CHARGE, str(&be_local_const_str_SYMBOL_CHARGE)
    SYMBOL_PASTE, str(&be_local_const_str_SYMBOL_PASTE)
    SYMBOL_BELL, str(&be_local_const_str_SYMBOL_BELL)
    SYMBOL_KEYBOARD, str(&be_local_const_str_SYMBOL_KEYBOARD)
    SYMBOL_GPS, str(&be_local_const_str_SYMBOL_GPS)
    SYMBOL_FILE, str(&be_local_const_str_SYMBOL_FILE)
    SYMBOL_WIFI, str(&be_local_const_str_SYMBOL_WIFI)
    SYMBOL_BATTERY_FULL, str(&be_local_const_str_SYMBOL_BATTERY_FULL)
    SYMBOL_BATTERY_3, str(&be_local_const_str_SYMBOL_BATTERY_3)
    SYMBOL_BATTERY_2, str(&be_local_const_str_SYMBOL_BATTERY_2)
    SYMBOL_BATTERY_1, str(&be_local_const_str_SYMBOL_BATTERY_1)
    SYMBOL_BATTERY_EMPTY, str(&be_local_const_str_SYMBOL_BATTERY_EMPTY)
    SYMBOL_USB, str(&be_local_const_str_SYMBOL_USB)
    SYMBOL_BLUETOOTH, str(&be_local_const_str_SYMBOL_BLUETOOTH)
    SYMBOL_TRASH, str(&be_local_const_str_SYMBOL_TRASH)
    SYMBOL_BACKSPACE, str(&be_local_const_str_SYMBOL_BACKSPACE)
    SYMBOL_SD_CARD, str(&be_local_const_str_SYMBOL_SD_CARD)
    SYMBOL_NEW_LINE, str(&be_local_const_str_SYMBOL_NEW_LINE)

    SYMBOL_DUMMY, str(&be_local_const_str_SYMBOL_DUMMY)

    SYMBOL_BULLET, str(&be_local_const_str_SYMBOL_BULLET)


    member, func(lv0_member)
    start, func(lv0_start)

    register_button_encoder, func(lv0_register_button_encoder)

    montserrat_font, func(lv0_load_montserrat_font)
    seg7_font, func(lv0_load_seg7_font)
    load_font, func(lv0_load_font)
    load_freetype_font, func(lv0_load_freetype_font)

    screenshot, func(lv0_screenshot)
}
@const_object_info_end */
#include "../generate/be_fixed_lvgl.h"

#endif // USE_LVGL

/********************************************************************/
