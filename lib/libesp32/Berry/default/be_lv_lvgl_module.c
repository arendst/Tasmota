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
static int lv_get_ver_res(bvm *vm) {
  return lv_disp_get_ver_res(lv_disp_get_default());
}

/* `lv` methods */
const lvbe_call_c_t lv_func[] = {
  // resolution
  { "get_hor_res", (void*) &lv_get_hor_res, "i", "" },
  { "get_ver_res", (void*) &lv_get_ver_res, "i", "" },

  // layers
  { "layer_sys", (void*) &lv_layer_sys, "lv_obj", "" },
  { "layer_top", (void*) &lv_layer_top, "lv_obj", "" },

  // screens
  { "scr_act", (void*) &lv_scr_act, "lv_obj", "" },
  { "scr_load", (void*) &lv_scr_load, "", "(lv_obj)" },
  { "scr_load_anim", (void*) &lv_scr_load_anim, "", "(lv_obj)iiib" },
};
const size_t lv_func_size = sizeof(lv_func) / sizeof(lv_func[0]);




typedef struct be_constint_t {
    const char * name;
    int32_t      value;
} be_constint_t;

const be_constint_t lv0_constants[] = {

    { "ALIGN_CENTER", LV_ALIGN_CENTER },
    { "ALIGN_IN_BOTTOM_LEFT", LV_ALIGN_IN_BOTTOM_LEFT },
    { "ALIGN_IN_BOTTOM_MID", LV_ALIGN_IN_BOTTOM_MID },
    { "ALIGN_IN_BOTTOM_RIGHT", LV_ALIGN_IN_BOTTOM_RIGHT },
    { "ALIGN_IN_LEFT_MID", LV_ALIGN_IN_LEFT_MID },
    { "ALIGN_IN_RIGHT_MID", LV_ALIGN_IN_RIGHT_MID },
    { "ALIGN_IN_TOP_LEFT", LV_ALIGN_IN_TOP_LEFT },
    { "ALIGN_IN_TOP_MID", LV_ALIGN_IN_TOP_MID },
    { "ALIGN_IN_TOP_RIGHT", LV_ALIGN_IN_TOP_RIGHT },
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
    { "ANIM_OFF", LV_ANIM_OFF },
    { "ANIM_ON", LV_ANIM_ON },
    { "AQUA", 65535 },
    { "ARC_PART_BG", LV_ARC_PART_BG },
    { "ARC_PART_INDIC", LV_ARC_PART_INDIC },
    { "ARC_PART_KNOB", LV_ARC_PART_KNOB },
    { "ARC_TYPE_NORMAL", LV_ARC_TYPE_NORMAL },
    { "ARC_TYPE_REVERSE", LV_ARC_TYPE_REVERSE },
    { "ARC_TYPE_SYMMETRIC", LV_ARC_TYPE_SYMMETRIC },
    { "BAR_TYPE_CUSTOM", LV_BAR_TYPE_CUSTOM },
    { "BAR_TYPE_NORMAL", LV_BAR_TYPE_NORMAL },
    { "BAR_TYPE_SYMMETRICAL", LV_BAR_TYPE_SYMMETRICAL },
    { "BLACK", 0 },
    { "BLEND_MODE_ADDITIVE", LV_BLEND_MODE_ADDITIVE },
    { "BLEND_MODE_NORMAL", LV_BLEND_MODE_NORMAL },
    { "BLEND_MODE_SUBTRACTIVE", LV_BLEND_MODE_SUBTRACTIVE },
    { "BLUE", 255 },
    { "BORDER_SIDE_BOTTOM", LV_BORDER_SIDE_BOTTOM },
    { "BORDER_SIDE_FULL", LV_BORDER_SIDE_FULL },
    { "BORDER_SIDE_INTERNAL", LV_BORDER_SIDE_INTERNAL },
    { "BORDER_SIDE_LEFT", LV_BORDER_SIDE_LEFT },
    { "BORDER_SIDE_NONE", LV_BORDER_SIDE_NONE },
    { "BORDER_SIDE_RIGHT", LV_BORDER_SIDE_RIGHT },
    { "BORDER_SIDE_TOP", LV_BORDER_SIDE_TOP },
    { "BTNMATRIX_CTRL_CHECKABLE", LV_BTNMATRIX_CTRL_CHECKABLE },
    { "BTNMATRIX_CTRL_CHECK_STATE", LV_BTNMATRIX_CTRL_CHECK_STATE },
    { "BTNMATRIX_CTRL_CLICK_TRIG", LV_BTNMATRIX_CTRL_CLICK_TRIG },
    { "BTNMATRIX_CTRL_DISABLED", LV_BTNMATRIX_CTRL_DISABLED },
    { "BTNMATRIX_CTRL_HIDDEN", LV_BTNMATRIX_CTRL_HIDDEN },
    { "BTNMATRIX_CTRL_NO_REPEAT", LV_BTNMATRIX_CTRL_NO_REPEAT },
    { "BTN_STATE_CHECKED_DISABLED", LV_BTN_STATE_CHECKED_DISABLED },
    { "BTN_STATE_CHECKED_PRESSED", LV_BTN_STATE_CHECKED_PRESSED },
    { "BTN_STATE_CHECKED_RELEASED", LV_BTN_STATE_CHECKED_RELEASED },
    { "BTN_STATE_DISABLED", LV_BTN_STATE_DISABLED },
    { "BTN_STATE_PRESSED", LV_BTN_STATE_PRESSED },
    { "BTN_STATE_RELEASED", LV_BTN_STATE_RELEASED },
    { "CALENDAR_PART_BG", LV_CALENDAR_PART_BG },
    { "CALENDAR_PART_DATE", LV_CALENDAR_PART_DATE },
    { "CALENDAR_PART_DAY_NAMES", LV_CALENDAR_PART_DAY_NAMES },
    { "CALENDAR_PART_HEADER", LV_CALENDAR_PART_HEADER },
    { "CHART_AXIS_DRAW_LAST_TICK", LV_CHART_AXIS_DRAW_LAST_TICK },
    { "CHART_AXIS_INVERSE_LABELS_ORDER", LV_CHART_AXIS_INVERSE_LABELS_ORDER },
    { "CHART_AXIS_PRIMARY_Y", LV_CHART_AXIS_PRIMARY_Y },
    { "CHART_AXIS_SECONDARY_Y", LV_CHART_AXIS_SECONDARY_Y },
    { "CHART_AXIS_SKIP_LAST_TICK", LV_CHART_AXIS_SKIP_LAST_TICK },
    { "CHART_CURSOR_DOWN", LV_CHART_CURSOR_DOWN },
    { "CHART_CURSOR_LEFT", LV_CHART_CURSOR_LEFT },
    { "CHART_CURSOR_NONE", LV_CHART_CURSOR_NONE },
    { "CHART_CURSOR_RIGHT", LV_CHART_CURSOR_RIGHT },
    { "CHART_CURSOR_UP", LV_CHART_CURSOR_UP },
    { "CHART_PART_BG", LV_CHART_PART_BG },
    { "CHART_PART_CURSOR", LV_CHART_PART_CURSOR },
    { "CHART_PART_SERIES", LV_CHART_PART_SERIES },
    { "CHART_PART_SERIES_BG", LV_CHART_PART_SERIES_BG },
    { "CHART_TYPE_COLUMN", LV_CHART_TYPE_COLUMN },
    { "CHART_TYPE_LINE", LV_CHART_TYPE_LINE },
    { "CHART_TYPE_NONE", LV_CHART_TYPE_NONE },
    { "CHART_UPDATE_MODE_CIRCULAR", LV_CHART_UPDATE_MODE_CIRCULAR },
    { "CHART_UPDATE_MODE_SHIFT", LV_CHART_UPDATE_MODE_SHIFT },
    { "CHECKBOX_PART_BG", LV_CHECKBOX_PART_BG },
    { "CHECKBOX_PART_BULLET", LV_CHECKBOX_PART_BULLET },
    { "CPICKER_COLOR_MODE_HUE", LV_CPICKER_COLOR_MODE_HUE },
    { "CPICKER_COLOR_MODE_SATURATION", LV_CPICKER_COLOR_MODE_SATURATION },
    { "CPICKER_COLOR_MODE_VALUE", LV_CPICKER_COLOR_MODE_VALUE },
    { "CPICKER_PART_KNOB", LV_CPICKER_PART_KNOB },
    { "CPICKER_PART_MAIN", LV_CPICKER_PART_MAIN },
    { "CPICKER_TYPE_DISC", LV_CPICKER_TYPE_DISC },
    { "CPICKER_TYPE_RECT", LV_CPICKER_TYPE_RECT },
    { "CYAN", 65535 },
    { "DISP_ROT_180", LV_DISP_ROT_180 },
    { "DISP_ROT_270", LV_DISP_ROT_270 },
    { "DISP_ROT_90", LV_DISP_ROT_90 },
    { "DISP_ROT_NONE", LV_DISP_ROT_NONE },
    { "DISP_SIZE_EXTRA_LARGE", LV_DISP_SIZE_EXTRA_LARGE },
    { "DISP_SIZE_LARGE", LV_DISP_SIZE_LARGE },
    { "DISP_SIZE_MEDIUM", LV_DISP_SIZE_MEDIUM },
    { "DISP_SIZE_SMALL", LV_DISP_SIZE_SMALL },
    { "DRAG_DIR_BOTH", LV_DRAG_DIR_BOTH },
    { "DRAG_DIR_HOR", LV_DRAG_DIR_HOR },
    { "DRAG_DIR_ONE", LV_DRAG_DIR_ONE },
    { "DRAG_DIR_VER", LV_DRAG_DIR_VER },
    { "DROPDOWN_DIR_DOWN", LV_DROPDOWN_DIR_DOWN },
    { "DROPDOWN_DIR_LEFT", LV_DROPDOWN_DIR_LEFT },
    { "DROPDOWN_DIR_RIGHT", LV_DROPDOWN_DIR_RIGHT },
    { "DROPDOWN_DIR_UP", LV_DROPDOWN_DIR_UP },
    { "DROPDOWN_PART_LIST", LV_DROPDOWN_PART_LIST },
    { "DROPDOWN_PART_MAIN", LV_DROPDOWN_PART_MAIN },
    { "DROPDOWN_PART_SCROLLBAR", LV_DROPDOWN_PART_SCROLLBAR },
    { "DROPDOWN_PART_SELECTED", LV_DROPDOWN_PART_SELECTED },
    { "EVENT_APPLY", LV_EVENT_APPLY },
    { "EVENT_CANCEL", LV_EVENT_CANCEL },
    { "EVENT_CLICKED", LV_EVENT_CLICKED },
    { "EVENT_DEFOCUSED", LV_EVENT_DEFOCUSED },
    { "EVENT_DELETE", LV_EVENT_DELETE },
    { "EVENT_DRAG_BEGIN", LV_EVENT_DRAG_BEGIN },
    { "EVENT_DRAG_END", LV_EVENT_DRAG_END },
    { "EVENT_DRAG_THROW_BEGIN", LV_EVENT_DRAG_THROW_BEGIN },
    { "EVENT_FOCUSED", LV_EVENT_FOCUSED },
    { "EVENT_GESTURE", LV_EVENT_GESTURE },
    { "EVENT_INSERT", LV_EVENT_INSERT },
    { "EVENT_KEY", LV_EVENT_KEY },
    { "EVENT_LEAVE", LV_EVENT_LEAVE },
    { "EVENT_LONG_PRESSED", LV_EVENT_LONG_PRESSED },
    { "EVENT_LONG_PRESSED_REPEAT", LV_EVENT_LONG_PRESSED_REPEAT },
    { "EVENT_PRESSED", LV_EVENT_PRESSED },
    { "EVENT_PRESSING", LV_EVENT_PRESSING },
    { "EVENT_PRESS_LOST", LV_EVENT_PRESS_LOST },
    { "EVENT_REFRESH", LV_EVENT_REFRESH },
    { "EVENT_RELEASED", LV_EVENT_RELEASED },
    { "EVENT_SHORT_CLICKED", LV_EVENT_SHORT_CLICKED },
    { "EVENT_VALUE_CHANGED", LV_EVENT_VALUE_CHANGED },
    { "FIT_MAX", LV_FIT_MAX },
    { "FIT_NONE", LV_FIT_NONE },
    { "FIT_PARENT", LV_FIT_PARENT },
    { "FIT_TIGHT", LV_FIT_TIGHT },
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
    { "GAUGE_PART_MAIN", LV_GAUGE_PART_MAIN },
    { "GAUGE_PART_MAJOR", LV_GAUGE_PART_MAJOR },
    { "GAUGE_PART_NEEDLE", LV_GAUGE_PART_NEEDLE },
    { "GESTURE_DIR_BOTTOM", LV_GESTURE_DIR_BOTTOM },
    { "GESTURE_DIR_LEFT", LV_GESTURE_DIR_LEFT },
    { "GESTURE_DIR_RIGHT", LV_GESTURE_DIR_RIGHT },
    { "GESTURE_DIR_TOP", LV_GESTURE_DIR_TOP },
    { "GRAD_DIR_HOR", LV_GRAD_DIR_HOR },
    { "GRAD_DIR_NONE", LV_GRAD_DIR_NONE },
    { "GRAD_DIR_VER", LV_GRAD_DIR_VER },
    { "GRAY", 8421504 },
    { "GREEN", 32768 },
    { "INDEV_STATE_PR", LV_INDEV_STATE_PR },
    { "INDEV_STATE_REL", LV_INDEV_STATE_REL },
    { "KEYBOARD_MODE_NUM", LV_KEYBOARD_MODE_NUM },
    { "KEYBOARD_MODE_SPECIAL", LV_KEYBOARD_MODE_SPECIAL },
    { "KEYBOARD_MODE_TEXT_LOWER", LV_KEYBOARD_MODE_TEXT_LOWER },
    { "KEYBOARD_MODE_TEXT_UPPER", LV_KEYBOARD_MODE_TEXT_UPPER },
    { "KEYBOARD_PART_BG", LV_KEYBOARD_PART_BG },
    { "KEYBOARD_PART_BTN", LV_KEYBOARD_PART_BTN },
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
    { "LABEL_ALIGN_AUTO", LV_LABEL_ALIGN_AUTO },
    { "LABEL_ALIGN_CENTER", LV_LABEL_ALIGN_CENTER },
    { "LABEL_ALIGN_LEFT", LV_LABEL_ALIGN_LEFT },
    { "LABEL_ALIGN_RIGHT", LV_LABEL_ALIGN_RIGHT },
    { "LABEL_LONG_BREAK", LV_LABEL_LONG_BREAK },
    { "LABEL_LONG_CROP", LV_LABEL_LONG_CROP },
    { "LABEL_LONG_DOT", LV_LABEL_LONG_DOT },
    { "LABEL_LONG_EXPAND", LV_LABEL_LONG_EXPAND },
    { "LABEL_LONG_SROLL", LV_LABEL_LONG_SROLL },
    { "LABEL_LONG_SROLL_CIRC", LV_LABEL_LONG_SROLL_CIRC },
    { "LAYOUT_CENTER", LV_LAYOUT_CENTER },
    { "LAYOUT_COLUMN_LEFT", LV_LAYOUT_COLUMN_LEFT },
    { "LAYOUT_COLUMN_MID", LV_LAYOUT_COLUMN_MID },
    { "LAYOUT_COLUMN_RIGHT", LV_LAYOUT_COLUMN_RIGHT },
    { "LAYOUT_GRID", LV_LAYOUT_GRID },
    { "LAYOUT_OFF", LV_LAYOUT_OFF },
    { "LAYOUT_PRETTY_BOTTOM", LV_LAYOUT_PRETTY_BOTTOM },
    { "LAYOUT_PRETTY_MID", LV_LAYOUT_PRETTY_MID },
    { "LAYOUT_PRETTY_TOP", LV_LAYOUT_PRETTY_TOP },
    { "LAYOUT_ROW_BOTTOM", LV_LAYOUT_ROW_BOTTOM },
    { "LAYOUT_ROW_MID", LV_LAYOUT_ROW_MID },
    { "LAYOUT_ROW_TOP", LV_LAYOUT_ROW_TOP },
    { "LED_PART_MAIN", LV_LED_PART_MAIN },
    { "LIME", 65280 },
    { "LINEMETER_PART_MAIN", LV_LINEMETER_PART_MAIN },
    { "LIST_PART_BG", LV_LIST_PART_BG },
    { "LIST_PART_EDGE_FLASH", LV_LIST_PART_EDGE_FLASH },
    { "LIST_PART_SCROLLBAR", LV_LIST_PART_SCROLLBAR },
    { "MAGENTA", 16711935 },
    { "MAROON", 8388608 },
    { "MSGBOX_PART_BG", LV_MSGBOX_PART_BG },
    { "MSGBOX_PART_BTN", LV_MSGBOX_PART_BTN },
    { "MSGBOX_PART_BTN_BG", LV_MSGBOX_PART_BTN_BG },
    { "NAVY", 128 },
    { "OBJMASK_PART_MAIN", LV_OBJMASK_PART_MAIN },
    { "OBJ_PART_ALL", LV_OBJ_PART_ALL },
    { "OBJ_PART_MAIN", LV_OBJ_PART_MAIN },
    { "OLIVE", 8421376 },
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
    { "PAGE_EDGE_BOTTOM", LV_PAGE_EDGE_BOTTOM },
    { "PAGE_EDGE_LEFT", LV_PAGE_EDGE_LEFT },
    { "PAGE_EDGE_RIGHT", LV_PAGE_EDGE_RIGHT },
    { "PAGE_EDGE_TOP", LV_PAGE_EDGE_TOP },
    { "PAGE_PART_BG", LV_PAGE_PART_BG },
    { "PAGE_PART_EDGE_FLASH", LV_PAGE_PART_EDGE_FLASH },
    { "PAGE_PART_SCROLLABLE", LV_PAGE_PART_SCROLLABLE },
    { "PAGE_PART_SCROLLBAR", LV_PAGE_PART_SCROLLBAR },
    { "PROTECT_CHILD_CHG", LV_PROTECT_CHILD_CHG },
    { "PROTECT_CLICK_FOCUS", LV_PROTECT_CLICK_FOCUS },
    { "PROTECT_EVENT_TO_DISABLED", LV_PROTECT_EVENT_TO_DISABLED },
    { "PROTECT_FOLLOW", LV_PROTECT_FOLLOW },
    { "PROTECT_NONE", LV_PROTECT_NONE },
    { "PROTECT_PARENT", LV_PROTECT_PARENT },
    { "PROTECT_POS", LV_PROTECT_POS },
    { "PROTECT_PRESS_LOST", LV_PROTECT_PRESS_LOST },
    { "PURPLE", 8388736 },
    { "RED", 16711680 },
    { "ROLLER_MODE_INFINITE", LV_ROLLER_MODE_INFINITE },
    { "ROLLER_MODE_NORMAL", LV_ROLLER_MODE_NORMAL },
    { "ROLLER_PART_BG", LV_ROLLER_PART_BG },
    { "ROLLER_PART_SELECTED", LV_ROLLER_PART_SELECTED },
    { "SCROLLBAR_MODE_AUTO", LV_SCROLLBAR_MODE_AUTO },
    { "SCROLLBAR_MODE_DRAG", LV_SCROLLBAR_MODE_DRAG },
    { "SCROLLBAR_MODE_HIDE", LV_SCROLLBAR_MODE_HIDE },
    { "SCROLLBAR_MODE_OFF", LV_SCROLLBAR_MODE_OFF },
    { "SCROLLBAR_MODE_ON", LV_SCROLLBAR_MODE_ON },
    { "SCROLLBAR_MODE_UNHIDE", LV_SCROLLBAR_MODE_UNHIDE },
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
    { "SILVER", 12632256 },
    { "SLIDER_PART_BG", LV_SLIDER_PART_BG },
    { "SLIDER_PART_INDIC", LV_SLIDER_PART_INDIC },
    { "SLIDER_PART_KNOB", LV_SLIDER_PART_KNOB },
    { "SLIDER_TYPE_NORMAL", LV_SLIDER_TYPE_NORMAL },
    { "SLIDER_TYPE_RANGE", LV_SLIDER_TYPE_RANGE },
    { "SLIDER_TYPE_SYMMETRICAL", LV_SLIDER_TYPE_SYMMETRICAL },
    { "SPINBOX_PART_BG", LV_SPINBOX_PART_BG },
    { "SPINBOX_PART_CURSOR", LV_SPINBOX_PART_CURSOR },
    { "SPINNER_DIR_BACKWARD", LV_SPINNER_DIR_BACKWARD },
    { "SPINNER_DIR_FORWARD", LV_SPINNER_DIR_FORWARD },
    { "SPINNER_PART_BG", LV_SPINNER_PART_BG },
    { "SPINNER_PART_INDIC", LV_SPINNER_PART_INDIC },
    { "SPINNER_TYPE_CONSTANT_ARC", LV_SPINNER_TYPE_CONSTANT_ARC },
    { "SPINNER_TYPE_FILLSPIN_ARC", LV_SPINNER_TYPE_FILLSPIN_ARC },
    { "SPINNER_TYPE_SPINNING_ARC", LV_SPINNER_TYPE_SPINNING_ARC },
    { "STATE_CHECKED", LV_STATE_CHECKED },
    { "STATE_DEFAULT", LV_STATE_DEFAULT },
    { "STATE_DISABLED", LV_STATE_DISABLED },
    { "STATE_EDITED", LV_STATE_EDITED },
    { "STATE_FOCUSED", LV_STATE_FOCUSED },
    { "STATE_HOVERED", LV_STATE_HOVERED },
    { "STATE_PRESSED", LV_STATE_PRESSED },
    { "STYLE_BG_BLEND_MODE", LV_STYLE_BG_BLEND_MODE },
    { "STYLE_BG_COLOR", LV_STYLE_BG_COLOR },
    { "STYLE_BG_GRAD_COLOR", LV_STYLE_BG_GRAD_COLOR },
    { "STYLE_BG_GRAD_DIR", LV_STYLE_BG_GRAD_DIR },
    { "STYLE_BG_GRAD_STOP", LV_STYLE_BG_GRAD_STOP },
    { "STYLE_BG_MAIN_STOP", LV_STYLE_BG_MAIN_STOP },
    { "STYLE_BG_OPA", LV_STYLE_BG_OPA },
    { "STYLE_BORDER_BLEND_MODE", LV_STYLE_BORDER_BLEND_MODE },
    { "STYLE_BORDER_COLOR", LV_STYLE_BORDER_COLOR },
    { "STYLE_BORDER_OPA", LV_STYLE_BORDER_OPA },
    { "STYLE_BORDER_POST", LV_STYLE_BORDER_POST },
    { "STYLE_BORDER_SIDE", LV_STYLE_BORDER_SIDE },
    { "STYLE_BORDER_WIDTH", LV_STYLE_BORDER_WIDTH },
    { "STYLE_CLIP_CORNER", LV_STYLE_CLIP_CORNER },
    { "STYLE_IMAGE_BLEND_MODE", LV_STYLE_IMAGE_BLEND_MODE },
    { "STYLE_IMAGE_OPA", LV_STYLE_IMAGE_OPA },
    { "STYLE_IMAGE_RECOLOR", LV_STYLE_IMAGE_RECOLOR },
    { "STYLE_IMAGE_RECOLOR_OPA", LV_STYLE_IMAGE_RECOLOR_OPA },
    { "STYLE_LINE_BLEND_MODE", LV_STYLE_LINE_BLEND_MODE },
    { "STYLE_LINE_COLOR", LV_STYLE_LINE_COLOR },
    { "STYLE_LINE_DASH_GAP", LV_STYLE_LINE_DASH_GAP },
    { "STYLE_LINE_DASH_WIDTH", LV_STYLE_LINE_DASH_WIDTH },
    { "STYLE_LINE_OPA", LV_STYLE_LINE_OPA },
    { "STYLE_LINE_ROUNDED", LV_STYLE_LINE_ROUNDED },
    { "STYLE_LINE_WIDTH", LV_STYLE_LINE_WIDTH },
    { "STYLE_MARGIN_BOTTOM", LV_STYLE_MARGIN_BOTTOM },
    { "STYLE_MARGIN_LEFT", LV_STYLE_MARGIN_LEFT },
    { "STYLE_MARGIN_RIGHT", LV_STYLE_MARGIN_RIGHT },
    { "STYLE_MARGIN_TOP", LV_STYLE_MARGIN_TOP },
    { "STYLE_OPA_SCALE", LV_STYLE_OPA_SCALE },
    { "STYLE_OUTLINE_BLEND_MODE", LV_STYLE_OUTLINE_BLEND_MODE },
    { "STYLE_OUTLINE_COLOR", LV_STYLE_OUTLINE_COLOR },
    { "STYLE_OUTLINE_OPA", LV_STYLE_OUTLINE_OPA },
    { "STYLE_OUTLINE_PAD", LV_STYLE_OUTLINE_PAD },
    { "STYLE_OUTLINE_WIDTH", LV_STYLE_OUTLINE_WIDTH },
    { "STYLE_PAD_BOTTOM", LV_STYLE_PAD_BOTTOM },
    { "STYLE_PAD_INNER", LV_STYLE_PAD_INNER },
    { "STYLE_PAD_LEFT", LV_STYLE_PAD_LEFT },
    { "STYLE_PAD_RIGHT", LV_STYLE_PAD_RIGHT },
    { "STYLE_PAD_TOP", LV_STYLE_PAD_TOP },
    { "STYLE_PATTERN_BLEND_MODE", LV_STYLE_PATTERN_BLEND_MODE },
    { "STYLE_PATTERN_IMAGE", LV_STYLE_PATTERN_IMAGE },
    { "STYLE_PATTERN_OPA", LV_STYLE_PATTERN_OPA },
    { "STYLE_PATTERN_RECOLOR", LV_STYLE_PATTERN_RECOLOR },
    { "STYLE_PATTERN_RECOLOR_OPA", LV_STYLE_PATTERN_RECOLOR_OPA },
    { "STYLE_PATTERN_REPEAT", LV_STYLE_PATTERN_REPEAT },
    { "STYLE_RADIUS", LV_STYLE_RADIUS },
    { "STYLE_SCALE_BORDER_WIDTH", LV_STYLE_SCALE_BORDER_WIDTH },
    { "STYLE_SCALE_END_BORDER_WIDTH", LV_STYLE_SCALE_END_BORDER_WIDTH },
    { "STYLE_SCALE_END_COLOR", LV_STYLE_SCALE_END_COLOR },
    { "STYLE_SCALE_END_LINE_WIDTH", LV_STYLE_SCALE_END_LINE_WIDTH },
    { "STYLE_SCALE_GRAD_COLOR", LV_STYLE_SCALE_GRAD_COLOR },
    { "STYLE_SCALE_WIDTH", LV_STYLE_SCALE_WIDTH },
    { "STYLE_SHADOW_BLEND_MODE", LV_STYLE_SHADOW_BLEND_MODE },
    { "STYLE_SHADOW_COLOR", LV_STYLE_SHADOW_COLOR },
    { "STYLE_SHADOW_OFS_X", LV_STYLE_SHADOW_OFS_X },
    { "STYLE_SHADOW_OFS_Y", LV_STYLE_SHADOW_OFS_Y },
    { "STYLE_SHADOW_OPA", LV_STYLE_SHADOW_OPA },
    { "STYLE_SHADOW_SPREAD", LV_STYLE_SHADOW_SPREAD },
    { "STYLE_SHADOW_WIDTH", LV_STYLE_SHADOW_WIDTH },
    { "STYLE_SIZE", LV_STYLE_SIZE },
    { "STYLE_TEXT_BLEND_MODE", LV_STYLE_TEXT_BLEND_MODE },
    { "STYLE_TEXT_COLOR", LV_STYLE_TEXT_COLOR },
    { "STYLE_TEXT_DECOR", LV_STYLE_TEXT_DECOR },
    { "STYLE_TEXT_FONT", LV_STYLE_TEXT_FONT },
    { "STYLE_TEXT_LETTER_SPACE", LV_STYLE_TEXT_LETTER_SPACE },
    { "STYLE_TEXT_LINE_SPACE", LV_STYLE_TEXT_LINE_SPACE },
    { "STYLE_TEXT_OPA", LV_STYLE_TEXT_OPA },
    { "STYLE_TEXT_SEL_BG_COLOR", LV_STYLE_TEXT_SEL_BG_COLOR },
    { "STYLE_TEXT_SEL_COLOR", LV_STYLE_TEXT_SEL_COLOR },
    { "STYLE_TRANSFORM_ANGLE", LV_STYLE_TRANSFORM_ANGLE },
    { "STYLE_TRANSFORM_HEIGHT", LV_STYLE_TRANSFORM_HEIGHT },
    { "STYLE_TRANSFORM_WIDTH", LV_STYLE_TRANSFORM_WIDTH },
    { "STYLE_TRANSFORM_ZOOM", LV_STYLE_TRANSFORM_ZOOM },
    { "STYLE_TRANSITION_DELAY", LV_STYLE_TRANSITION_DELAY },
    { "STYLE_TRANSITION_PATH", LV_STYLE_TRANSITION_PATH },
    { "STYLE_TRANSITION_PROP_1", LV_STYLE_TRANSITION_PROP_1 },
    { "STYLE_TRANSITION_PROP_2", LV_STYLE_TRANSITION_PROP_2 },
    { "STYLE_TRANSITION_PROP_3", LV_STYLE_TRANSITION_PROP_3 },
    { "STYLE_TRANSITION_PROP_4", LV_STYLE_TRANSITION_PROP_4 },
    { "STYLE_TRANSITION_PROP_5", LV_STYLE_TRANSITION_PROP_5 },
    { "STYLE_TRANSITION_PROP_6", LV_STYLE_TRANSITION_PROP_6 },
    { "STYLE_TRANSITION_TIME", LV_STYLE_TRANSITION_TIME },
    { "STYLE_VALUE_ALIGN", LV_STYLE_VALUE_ALIGN },
    { "STYLE_VALUE_BLEND_MODE", LV_STYLE_VALUE_BLEND_MODE },
    { "STYLE_VALUE_COLOR", LV_STYLE_VALUE_COLOR },
    { "STYLE_VALUE_FONT", LV_STYLE_VALUE_FONT },
    { "STYLE_VALUE_LETTER_SPACE", LV_STYLE_VALUE_LETTER_SPACE },
    { "STYLE_VALUE_LINE_SPACE", LV_STYLE_VALUE_LINE_SPACE },
    { "STYLE_VALUE_OFS_X", LV_STYLE_VALUE_OFS_X },
    { "STYLE_VALUE_OFS_Y", LV_STYLE_VALUE_OFS_Y },
    { "STYLE_VALUE_OPA", LV_STYLE_VALUE_OPA },
    { "STYLE_VALUE_STR", LV_STYLE_VALUE_STR },
    { "SWITCH_PART_BG", LV_SWITCH_PART_BG },
    { "SWITCH_PART_INDIC", LV_SWITCH_PART_INDIC },
    { "SWITCH_PART_KNOB", LV_SWITCH_PART_KNOB },
    { "TABLE_PART_BG", LV_TABLE_PART_BG },
    { "TABLE_PART_CELL1", LV_TABLE_PART_CELL1 },
    { "TABLE_PART_CELL2", LV_TABLE_PART_CELL2 },
    { "TABLE_PART_CELL3", LV_TABLE_PART_CELL3 },
    { "TABLE_PART_CELL4", LV_TABLE_PART_CELL4 },
    { "TABVIEW_PART_BG", LV_TABVIEW_PART_BG },
    { "TABVIEW_PART_BG_SCROLLABLE", LV_TABVIEW_PART_BG_SCROLLABLE },
    { "TABVIEW_PART_INDIC", LV_TABVIEW_PART_INDIC },
    { "TABVIEW_PART_TAB_BG", LV_TABVIEW_PART_TAB_BG },
    { "TABVIEW_PART_TAB_BTN", LV_TABVIEW_PART_TAB_BTN },
    { "TABVIEW_TAB_POS_BOTTOM", LV_TABVIEW_TAB_POS_BOTTOM },
    { "TABVIEW_TAB_POS_LEFT", LV_TABVIEW_TAB_POS_LEFT },
    { "TABVIEW_TAB_POS_NONE", LV_TABVIEW_TAB_POS_NONE },
    { "TABVIEW_TAB_POS_RIGHT", LV_TABVIEW_TAB_POS_RIGHT },
    { "TABVIEW_TAB_POS_TOP", LV_TABVIEW_TAB_POS_TOP },
    { "TEAL", 32896 },
    { "TEXTAREA_CURSOR_LAST", LV_TEXTAREA_CURSOR_LAST },
    { "TEXTAREA_PART_BG", LV_TEXTAREA_PART_BG },
    { "TEXTAREA_PART_CURSOR", LV_TEXTAREA_PART_CURSOR },
    { "TEXTAREA_PART_EDGE_FLASH", LV_TEXTAREA_PART_EDGE_FLASH },
    { "TEXTAREA_PART_PLACEHOLDER", LV_TEXTAREA_PART_PLACEHOLDER },
    { "TEXTAREA_PART_SCROLLBAR", LV_TEXTAREA_PART_SCROLLBAR },
    { "TEXT_DECOR_NONE", LV_TEXT_DECOR_NONE },
    { "TEXT_DECOR_STRIKETHROUGH", LV_TEXT_DECOR_STRIKETHROUGH },
    { "TEXT_DECOR_UNDERLINE", LV_TEXT_DECOR_UNDERLINE },
    { "TILEVIEW_PART_BG", LV_TILEVIEW_PART_BG },
    { "TILEVIEW_PART_EDGE_FLASH", LV_TILEVIEW_PART_EDGE_FLASH },
    { "TILEVIEW_PART_SCROLLBAR", LV_TILEVIEW_PART_SCROLLBAR },
    { "TXT_CMD_STATE_IN", LV_TXT_CMD_STATE_IN },
    { "TXT_CMD_STATE_PAR", LV_TXT_CMD_STATE_PAR },
    { "TXT_CMD_STATE_WAIT", LV_TXT_CMD_STATE_WAIT },
    { "TXT_FLAG_CENTER", LV_TXT_FLAG_CENTER },
    { "TXT_FLAG_EXPAND", LV_TXT_FLAG_EXPAND },
    { "TXT_FLAG_FIT", LV_TXT_FLAG_FIT },
    { "TXT_FLAG_NONE", LV_TXT_FLAG_NONE },
    { "TXT_FLAG_RECOLOR", LV_TXT_FLAG_RECOLOR },
    { "TXT_FLAG_RIGHT", LV_TXT_FLAG_RIGHT },
    { "WHITE", 16777215 },
    { "WIN_PART_BG", LV_WIN_PART_BG },
    { "WIN_PART_CONTENT_SCROLLABLE", LV_WIN_PART_CONTENT_SCROLLABLE },
    { "WIN_PART_HEADER", LV_WIN_PART_HEADER },
    { "WIN_PART_SCROLLBAR", LV_WIN_PART_SCROLLBAR },
    { "YELLOW", 16776960 },

};

const size_t lv0_constants_size = sizeof(lv0_constants)/sizeof(lv0_constants[0]);


#if !BE_USE_PRECOMPILED_OBJECT
//#if 1           // TODO we will do pre-compiled later


be_native_module_attr_table(lvgl) {
    // Symbols    
    be_native_module_str("SYMBOL_AUDIO", "\xef\x80\x81"),
    be_native_module_str("SYMBOL_VIDEO", "\xef\x80\x88"),
    be_native_module_str("SYMBOL_LIST", "\xef\x80\x8b"),
    be_native_module_str("SYMBOL_OK", "\xef\x80\x8c"),
    be_native_module_str("SYMBOL_CLOSE", "\xef\x80\x8d"),
    be_native_module_str("SYMBOL_POWER", "\xef\x80\x91"),
    be_native_module_str("SYMBOL_SETTINGS", "\xef\x80\x93"),
    be_native_module_str("SYMBOL_HOME", "\xef\x80\x95"),
    be_native_module_str("SYMBOL_DOWNLOAD", "\xef\x80\x99"),
    be_native_module_str("SYMBOL_DRIVE", "\xef\x80\x9c"),
    be_native_module_str("SYMBOL_REFRESH", "\xef\x80\xa1"),
    be_native_module_str("SYMBOL_MUTE", "\xef\x80\xa6"),
    be_native_module_str("SYMBOL_VOLUME_MID", "\xef\x80\xa7"),
    be_native_module_str("SYMBOL_VOLUME_MAX", "\xef\x80\xa8"),
    be_native_module_str("SYMBOL_IMAGE", "\xef\x80\xbe"),
    be_native_module_str("SYMBOL_EDIT", "\xef\x8C\x84"),
    be_native_module_str("SYMBOL_PREV", "\xef\x81\x88"),
    be_native_module_str("SYMBOL_PLAY", "\xef\x81\x8b"),
    be_native_module_str("SYMBOL_PAUSE", "\xef\x81\x8c"),
    be_native_module_str("SYMBOL_STOP", "\xef\x81\x8d"),
    be_native_module_str("SYMBOL_NEXT", "\xef\x81\x91"),
    be_native_module_str("SYMBOL_EJECT", "\xef\x81\x92"),
    be_native_module_str("SYMBOL_LEFT", "\xef\x81\x93"),
    be_native_module_str("SYMBOL_RIGHT", "\xef\x81\x94"),
    be_native_module_str("SYMBOL_PLUS", "\xef\x81\xa7"),
    be_native_module_str("SYMBOL_MINUS", "\xef\x81\xa8"),
    be_native_module_str("SYMBOL_EYE_OPEN", "\xef\x81\xae"),
    be_native_module_str("SYMBOL_EYE_CLOSE", "\xef\x81\xb0"),
    be_native_module_str("SYMBOL_WARNING", "\xef\x81\xb1"),
    be_native_module_str("SYMBOL_SHUFFLE", "\xef\x81\xb4"),
    be_native_module_str("SYMBOL_UP", "\xef\x81\xb7"),
    be_native_module_str("SYMBOL_DOWN", "\xef\x81\xb8"),
    be_native_module_str("SYMBOL_LOOP", "\xef\x81\xb9"),
    be_native_module_str("SYMBOL_DIRECTORY", "\xef\x81\xbb"),
    be_native_module_str("SYMBOL_UPLOAD", "\xef\x82\x93"),
    be_native_module_str("SYMBOL_CALL", "\xef\x82\x95"),
    be_native_module_str("SYMBOL_CUT", "\xef\x83\x84"),
    be_native_module_str("SYMBOL_COPY", "\xef\x83\x85"),
    be_native_module_str("SYMBOL_SAVE", "\xef\x83\x87"),
    be_native_module_str("SYMBOL_CHARGE", "\xef\x83\xa7"),
    be_native_module_str("SYMBOL_PASTE", "\xef\x83\xAA"),
    be_native_module_str("SYMBOL_BELL", "\xef\x83\xb3"),
    be_native_module_str("SYMBOL_KEYBOARD", "\xef\x84\x9c"),
    be_native_module_str("SYMBOL_GPS", "\xef\x84\xa4"),
    be_native_module_str("SYMBOL_FILE", "\xef\x85\x9b"),
    be_native_module_str("SYMBOL_WIFI", "\xef\x87\xab"),
    be_native_module_str("SYMBOL_BATTERY_FULL", "\xef\x89\x80"),
    be_native_module_str("SYMBOL_BATTERY_3", "\xef\x89\x81"),
    be_native_module_str("SYMBOL_BATTERY_2", "\xef\x89\x82"),
    be_native_module_str("SYMBOL_BATTERY_1", "\xef\x89\x83"),
    be_native_module_str("SYMBOL_BATTERY_EMPTY", "\xef\x89\x84"),
    be_native_module_str("SYMBOL_USB", "\xef\x8a\x87"),
    be_native_module_str("SYMBOL_BLUETOOTH", "\xef\x8a\x93"),
    be_native_module_str("SYMBOL_TRASH", "\xef\x8B\xAD"),
    be_native_module_str("SYMBOL_BACKSPACE", "\xef\x95\x9A"),
    be_native_module_str("SYMBOL_SD_CARD", "\xef\x9F\x82"),
    be_native_module_str("SYMBOL_NEW_LINE", "\xef\xA2\xA2"),

    be_native_module_str("SYMBOL_DUMMY", "\xEF\xA3\xBF"),

    be_native_module_str("SYMBOL_BULLET", "\xE2\x80\xA2"),


/* `lvgl` module functions */



    be_native_module_function("member", lv0_member),
    be_native_module_function("start", lv0_start),

    be_native_module_function("register_button_encoder", lv0_register_button_encoder),

    be_native_module_function("montserrat_font", lv0_load_montserrat_font),
    be_native_module_function("seg7_font", lv0_load_seg7_font),
    be_native_module_function("load_font", lv0_load_font),
    be_native_module_function("load_freetype_font", lv0_load_freetype_font),

    be_native_module_function("screenshot", lv0_screenshot),


};

be_define_native_module(lvgl, NULL);

#else

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
#endif

#endif // USE_LVGL

/********************************************************************/
