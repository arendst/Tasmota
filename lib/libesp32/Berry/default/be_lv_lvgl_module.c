/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
/********************************************************************
 * LVGL Module
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

extern int lv0_member(bvm *vm);     // resolve virtual members

extern int lv0_start(bvm *vm);

extern int lv0_register_button_encoder(bvm *vm);  // add buttons with encoder logic

extern int lv0_load_montserrat_font(bvm *vm);
extern int lv0_load_seg7_font(bvm *vm);
extern int lv0_load_font(bvm *vm);
extern int lv0_load_freetype_font(bvm *vm);

extern int lv0_scr_act(bvm *vm);
extern int lv0_layer_top(bvm *vm);
extern int lv0_layer_sys(bvm *vm);
extern int lv0_get_hor_res(bvm *vm);
extern int lv0_get_ver_res(bvm *vm);
extern int lv0_screenshot(bvm *vm);



typedef struct lvbe_constant_t {
    const char * name;
    int32_t      value;
} lvbe_constant_t;

const lvbe_constant_t lv0_constants[] = {

    { "ALIGN_CENTER", 0 },
    { "ALIGN_IN_BOTTOM_LEFT", 4 },
    { "ALIGN_IN_BOTTOM_MID", 5 },
    { "ALIGN_IN_BOTTOM_RIGHT", 6 },
    { "ALIGN_IN_LEFT_MID", 7 },
    { "ALIGN_IN_RIGHT_MID", 8 },
    { "ALIGN_IN_TOP_LEFT", 1 },
    { "ALIGN_IN_TOP_MID", 2 },
    { "ALIGN_IN_TOP_RIGHT", 3 },
    { "ALIGN_OUT_BOTTOM_LEFT", 12 },
    { "ALIGN_OUT_BOTTOM_MID", 13 },
    { "ALIGN_OUT_BOTTOM_RIGHT", 14 },
    { "ALIGN_OUT_LEFT_BOTTOM", 17 },
    { "ALIGN_OUT_LEFT_MID", 16 },
    { "ALIGN_OUT_LEFT_TOP", 15 },
    { "ALIGN_OUT_RIGHT_BOTTOM", 20 },
    { "ALIGN_OUT_RIGHT_MID", 19 },
    { "ALIGN_OUT_RIGHT_TOP", 18 },
    { "ALIGN_OUT_TOP_LEFT", 9 },
    { "ALIGN_OUT_TOP_MID", 10 },
    { "ALIGN_OUT_TOP_RIGHT", 11 },
    { "ANIM_OFF", 0 },
    { "ANIM_ON", 1 },
    { "AQUA", 65535 },
    { "ARC_PART_BG", 0 },
    { "ARC_PART_INDIC", 1 },
    { "ARC_PART_KNOB", 2 },
    { "ARC_TYPE_NORMAL", 0 },
    { "ARC_TYPE_REVERSE", 2 },
    { "ARC_TYPE_SYMMETRIC", 1 },
    { "BAR_TYPE_CUSTOM", 2 },
    { "BAR_TYPE_NORMAL", 0 },
    { "BAR_TYPE_SYMMETRICAL", 1 },
    { "BLACK", 0 },
    { "BLEND_MODE_ADDITIVE", 1 },
    { "BLEND_MODE_NORMAL", 0 },
    { "BLEND_MODE_SUBTRACTIVE", 2 },
    { "BLUE", 255 },
    { "BORDER_SIDE_BOTTOM", 1 },
    { "BORDER_SIDE_FULL", 15 },
    { "BORDER_SIDE_INTERNAL", 16 },
    { "BORDER_SIDE_LEFT", 4 },
    { "BORDER_SIDE_NONE", 0 },
    { "BORDER_SIDE_RIGHT", 8 },
    { "BORDER_SIDE_TOP", 2 },
    { "BTNMATRIX_CTRL_CHECKABLE", 64 },
    { "BTNMATRIX_CTRL_CHECK_STATE", 128 },
    { "BTNMATRIX_CTRL_CLICK_TRIG", 256 },
    { "BTNMATRIX_CTRL_DISABLED", 32 },
    { "BTNMATRIX_CTRL_HIDDEN", 8 },
    { "BTNMATRIX_CTRL_NO_REPEAT", 16 },
    { "BTN_STATE_CHECKED_DISABLED", 5 },
    { "BTN_STATE_CHECKED_PRESSED", 4 },
    { "BTN_STATE_CHECKED_RELEASED", 3 },
    { "BTN_STATE_DISABLED", 2 },
    { "BTN_STATE_PRESSED", 1 },
    { "BTN_STATE_RELEASED", 0 },
    { "CALENDAR_PART_BG", 0 },
    { "CALENDAR_PART_DATE", 3 },
    { "CALENDAR_PART_DAY_NAMES", 2 },
    { "CALENDAR_PART_HEADER", 1 },
    { "CHART_AXIS_DRAW_LAST_TICK", 1 },
    { "CHART_AXIS_INVERSE_LABELS_ORDER", 2 },
    { "CHART_AXIS_PRIMARY_Y", 0 },
    { "CHART_AXIS_SECONDARY_Y", 1 },
    { "CHART_AXIS_SKIP_LAST_TICK", 0 },
    { "CHART_CURSOR_DOWN", 8 },
    { "CHART_CURSOR_LEFT", 4 },
    { "CHART_CURSOR_NONE", 0 },
    { "CHART_CURSOR_RIGHT", 1 },
    { "CHART_CURSOR_UP", 2 },
    { "CHART_PART_BG", 0 },
    { "CHART_PART_CURSOR", 3 },
    { "CHART_PART_SERIES", 2 },
    { "CHART_PART_SERIES_BG", 1 },
    { "CHART_TYPE_COLUMN", 2 },
    { "CHART_TYPE_LINE", 1 },
    { "CHART_TYPE_NONE", 0 },
    { "CHART_UPDATE_MODE_CIRCULAR", 1 },
    { "CHART_UPDATE_MODE_SHIFT", 0 },
    { "CHECKBOX_PART_BG", 0 },
    { "CHECKBOX_PART_BULLET", 64 },
    { "CPICKER_COLOR_MODE_HUE", 0 },
    { "CPICKER_COLOR_MODE_SATURATION", 1 },
    { "CPICKER_COLOR_MODE_VALUE", 2 },
    { "CPICKER_PART_KNOB", 1 },
    { "CPICKER_PART_MAIN", 0 },
    { "CPICKER_TYPE_DISC", 1 },
    { "CPICKER_TYPE_RECT", 0 },
    { "CYAN", 65535 },
    { "DISP_ROT_180", 2 },
    { "DISP_ROT_270", 3 },
    { "DISP_ROT_90", 1 },
    { "DISP_ROT_NONE", 0 },
    { "DISP_SIZE_EXTRA_LARGE", 3 },
    { "DISP_SIZE_LARGE", 2 },
    { "DISP_SIZE_MEDIUM", 1 },
    { "DISP_SIZE_SMALL", 0 },
    { "DRAG_DIR_BOTH", 3 },
    { "DRAG_DIR_HOR", 1 },
    { "DRAG_DIR_ONE", 4 },
    { "DRAG_DIR_VER", 2 },
    { "DROPDOWN_DIR_DOWN", 0 },
    { "DROPDOWN_DIR_LEFT", 2 },
    { "DROPDOWN_DIR_RIGHT", 3 },
    { "DROPDOWN_DIR_UP", 1 },
    { "DROPDOWN_PART_LIST", 64 },
    { "DROPDOWN_PART_MAIN", 0 },
    { "DROPDOWN_PART_SCROLLBAR", 65 },
    { "DROPDOWN_PART_SELECTED", 66 },
    { "EVENT_APPLY", 19 },
    { "EVENT_CANCEL", 20 },
    { "EVENT_CLICKED", 6 },
    { "EVENT_DEFOCUSED", 14 },
    { "EVENT_DELETE", 21 },
    { "EVENT_DRAG_BEGIN", 8 },
    { "EVENT_DRAG_END", 9 },
    { "EVENT_DRAG_THROW_BEGIN", 10 },
    { "EVENT_FOCUSED", 13 },
    { "EVENT_GESTURE", 11 },
    { "EVENT_INSERT", 17 },
    { "EVENT_KEY", 12 },
    { "EVENT_LEAVE", 15 },
    { "EVENT_LONG_PRESSED", 4 },
    { "EVENT_LONG_PRESSED_REPEAT", 5 },
    { "EVENT_PRESSED", 0 },
    { "EVENT_PRESSING", 1 },
    { "EVENT_PRESS_LOST", 2 },
    { "EVENT_REFRESH", 18 },
    { "EVENT_RELEASED", 7 },
    { "EVENT_SHORT_CLICKED", 3 },
    { "EVENT_VALUE_CHANGED", 16 },
    { "FIT_MAX", 3 },
    { "FIT_NONE", 0 },
    { "FIT_PARENT", 2 },
    { "FIT_TIGHT", 1 },
    { "FS_MODE_RD", 2 },
    { "FS_MODE_WR", 1 },
    { "FS_RES_BUSY", 7 },
    { "FS_RES_DENIED", 6 },
    { "FS_RES_FS_ERR", 2 },
    { "FS_RES_FULL", 4 },
    { "FS_RES_HW_ERR", 1 },
    { "FS_RES_INV_PARAM", 11 },
    { "FS_RES_LOCKED", 5 },
    { "FS_RES_NOT_EX", 3 },
    { "FS_RES_NOT_IMP", 9 },
    { "FS_RES_OK", 0 },
    { "FS_RES_OUT_OF_MEM", 10 },
    { "FS_RES_TOUT", 8 },
    { "FS_RES_UNKNOWN", 12 },
    { "GAUGE_PART_MAIN", 0 },
    { "GAUGE_PART_MAJOR", 1 },
    { "GAUGE_PART_NEEDLE", 2 },
    { "GESTURE_DIR_BOTTOM", 1 },
    { "GESTURE_DIR_LEFT", 2 },
    { "GESTURE_DIR_RIGHT", 3 },
    { "GESTURE_DIR_TOP", 0 },
    { "GRAD_DIR_HOR", 2 },
    { "GRAD_DIR_NONE", 0 },
    { "GRAD_DIR_VER", 1 },
    { "GRAY", 8421504 },
    { "GREEN", 32768 },
    { "INDEV_STATE_PR", 1 },
    { "INDEV_STATE_REL", 0 },
    { "KEYBOARD_MODE_NUM", 3 },
    { "KEYBOARD_MODE_SPECIAL", 2 },
    { "KEYBOARD_MODE_TEXT_LOWER", 0 },
    { "KEYBOARD_MODE_TEXT_UPPER", 1 },
    { "KEYBOARD_PART_BG", 0 },
    { "KEYBOARD_PART_BTN", 1 },
    { "KEY_BACKSPACE", 8 },
    { "KEY_DEL", 127 },
    { "KEY_DOWN", 18 },
    { "KEY_END", 3 },
    { "KEY_ENTER", 10 },
    { "KEY_ESC", 27 },
    { "KEY_HOME", 2 },
    { "KEY_LEFT", 20 },
    { "KEY_NEXT", 9 },
    { "KEY_PREV", 11 },
    { "KEY_RIGHT", 19 },
    { "KEY_UP", 17 },
    { "LABEL_ALIGN_AUTO", 3 },
    { "LABEL_ALIGN_CENTER", 1 },
    { "LABEL_ALIGN_LEFT", 0 },
    { "LABEL_ALIGN_RIGHT", 2 },
    { "LABEL_LONG_BREAK", 1 },
    { "LABEL_LONG_CROP", 5 },
    { "LABEL_LONG_DOT", 2 },
    { "LABEL_LONG_EXPAND", 0 },
    { "LABEL_LONG_SROLL", 3 },
    { "LABEL_LONG_SROLL_CIRC", 4 },
    { "LAYOUT_CENTER", 1 },
    { "LAYOUT_COLUMN_LEFT", 2 },
    { "LAYOUT_COLUMN_MID", 3 },
    { "LAYOUT_COLUMN_RIGHT", 4 },
    { "LAYOUT_GRID", 11 },
    { "LAYOUT_OFF", 0 },
    { "LAYOUT_PRETTY_BOTTOM", 10 },
    { "LAYOUT_PRETTY_MID", 9 },
    { "LAYOUT_PRETTY_TOP", 8 },
    { "LAYOUT_ROW_BOTTOM", 7 },
    { "LAYOUT_ROW_MID", 6 },
    { "LAYOUT_ROW_TOP", 5 },
    { "LED_PART_MAIN", 0 },
    { "LIME", 65280 },
    { "LINEMETER_PART_MAIN", 0 },
    { "LIST_PART_BG", 0 },
    { "LIST_PART_EDGE_FLASH", 2 },
    { "LIST_PART_SCROLLBAR", 1 },
    { "MAGENTA", 16711935 },
    { "MAROON", 8388608 },
    { "NAVY", 128 },
    { "OBJMASK_PART_MAIN", 0 },
    { "OBJ_PART_ALL", 255 },
    { "OBJ_PART_MAIN", 0 },
    { "OBJ_PART_REAL_FIRST", 64 },
    { "OBJ_PART_VIRTUAL_FIRST", 1 },
    { "OLIVE", 8421376 },
    { "OPA_0", 0 },
    { "OPA_10", 25 },
    { "OPA_100", 255 },
    { "OPA_20", 51 },
    { "OPA_30", 76 },
    { "OPA_40", 102 },
    { "OPA_50", 127 },
    { "OPA_60", 153 },
    { "OPA_70", 178 },
    { "OPA_80", 204 },
    { "OPA_90", 229 },
    { "OPA_COVER", 255 },
    { "OPA_TRANSP", 0 },
    { "PAGE_EDGE_BOTTOM", 8 },
    { "PAGE_EDGE_LEFT", 1 },
    { "PAGE_EDGE_RIGHT", 4 },
    { "PAGE_EDGE_TOP", 2 },
    { "PROTECT_CHILD_CHG", 1 },
    { "PROTECT_CLICK_FOCUS", 32 },
    { "PROTECT_EVENT_TO_DISABLED", 64 },
    { "PROTECT_FOLLOW", 8 },
    { "PROTECT_NONE", 0 },
    { "PROTECT_PARENT", 2 },
    { "PROTECT_POS", 4 },
    { "PROTECT_PRESS_LOST", 16 },
    { "PURPLE", 8388736 },
    { "RED", 16711680 },
    { "ROLLER_MODE_INFINITE", 1 },
    { "ROLLER_MODE_NORMAL", 0 },
    { "SCROLLBAR_MODE_AUTO", 3 },
    { "SCROLLBAR_MODE_DRAG", 2 },
    { "SCROLLBAR_MODE_HIDE", 4 },
    { "SCROLLBAR_MODE_OFF", 0 },
    { "SCROLLBAR_MODE_ON", 1 },
    { "SCROLLBAR_MODE_UNHIDE", 8 },
    { "SILVER", 12632256 },
    { "SLIDER_TYPE_NORMAL", 0 },
    { "SLIDER_TYPE_RANGE", 2 },
    { "SLIDER_TYPE_SYMMETRICAL", 1 },
    { "SPINNER_DIR_BACKWARD", 1 },
    { "SPINNER_DIR_FORWARD", 0 },
    { "SPINNER_TYPE_CONSTANT_ARC", 2 },
    { "SPINNER_TYPE_FILLSPIN_ARC", 1 },
    { "SPINNER_TYPE_SPINNING_ARC", 0 },
    { "STATE_CHECKED", 1 },
    { "STATE_DEFAULT", 0 },
    { "STATE_DISABLED", 32 },
    { "STATE_EDITED", 4 },
    { "STATE_FOCUSED", 2 },
    { "STATE_HOVERED", 8 },
    { "STATE_PRESSED", 16 },
    { "STYLE_BG_BLEND_MODE", 32 },
    { "STYLE_BG_COLOR", 41 },
    { "STYLE_BG_GRAD_COLOR", 42 },
    { "STYLE_BG_GRAD_DIR", 35 },
    { "STYLE_BG_GRAD_STOP", 34 },
    { "STYLE_BG_MAIN_STOP", 33 },
    { "STYLE_BG_OPA", 44 },
    { "STYLE_BORDER_BLEND_MODE", 50 },
    { "STYLE_BORDER_COLOR", 57 },
    { "STYLE_BORDER_OPA", 60 },
    { "STYLE_BORDER_POST", 51 },
    { "STYLE_BORDER_SIDE", 49 },
    { "STYLE_BORDER_WIDTH", 48 },
    { "STYLE_CLIP_CORNER", 2 },
    { "STYLE_IMAGE_BLEND_MODE", 32928 },
    { "STYLE_IMAGE_OPA", 32940 },
    { "STYLE_IMAGE_RECOLOR", 32937 },
    { "STYLE_IMAGE_RECOLOR_OPA", 32941 },
    { "STYLE_LINE_BLEND_MODE", 145 },
    { "STYLE_LINE_COLOR", 153 },
    { "STYLE_LINE_DASH_GAP", 147 },
    { "STYLE_LINE_DASH_WIDTH", 146 },
    { "STYLE_LINE_OPA", 156 },
    { "STYLE_LINE_ROUNDED", 148 },
    { "STYLE_LINE_WIDTH", 144 },
    { "STYLE_MARGIN_BOTTOM", 22 },
    { "STYLE_MARGIN_LEFT", 23 },
    { "STYLE_MARGIN_RIGHT", 24 },
    { "STYLE_MARGIN_TOP", 21 },
    { "STYLE_OPA_SCALE", 32780 },
    { "STYLE_OUTLINE_BLEND_MODE", 66 },
    { "STYLE_OUTLINE_COLOR", 73 },
    { "STYLE_OUTLINE_OPA", 76 },
    { "STYLE_OUTLINE_PAD", 65 },
    { "STYLE_OUTLINE_WIDTH", 64 },
    { "STYLE_PAD_BOTTOM", 17 },
    { "STYLE_PAD_INNER", 20 },
    { "STYLE_PAD_LEFT", 18 },
    { "STYLE_PAD_RIGHT", 19 },
    { "STYLE_PAD_TOP", 16 },
    { "STYLE_PATTERN_BLEND_MODE", 96 },
    { "STYLE_PATTERN_IMAGE", 110 },
    { "STYLE_PATTERN_OPA", 108 },
    { "STYLE_PATTERN_RECOLOR", 105 },
    { "STYLE_PATTERN_RECOLOR_OPA", 109 },
    { "STYLE_PATTERN_REPEAT", 97 },
    { "STYLE_RADIUS", 1 },
    { "STYLE_SCALE_BORDER_WIDTH", 193 },
    { "STYLE_SCALE_END_BORDER_WIDTH", 194 },
    { "STYLE_SCALE_END_COLOR", 202 },
    { "STYLE_SCALE_END_LINE_WIDTH", 195 },
    { "STYLE_SCALE_GRAD_COLOR", 201 },
    { "STYLE_SCALE_WIDTH", 192 },
    { "STYLE_SHADOW_BLEND_MODE", 84 },
    { "STYLE_SHADOW_COLOR", 89 },
    { "STYLE_SHADOW_OFS_X", 81 },
    { "STYLE_SHADOW_OFS_Y", 82 },
    { "STYLE_SHADOW_OPA", 92 },
    { "STYLE_SHADOW_SPREAD", 83 },
    { "STYLE_SHADOW_WIDTH", 80 },
    { "STYLE_SIZE", 3 },
    { "STYLE_TEXT_BLEND_MODE", 32899 },
    { "STYLE_TEXT_COLOR", 32905 },
    { "STYLE_TEXT_DECOR", 32898 },
    { "STYLE_TEXT_FONT", 32910 },
    { "STYLE_TEXT_LETTER_SPACE", 32896 },
    { "STYLE_TEXT_LINE_SPACE", 32897 },
    { "STYLE_TEXT_OPA", 32908 },
    { "STYLE_TEXT_SEL_BG_COLOR", 32907 },
    { "STYLE_TEXT_SEL_COLOR", 32906 },
    { "STYLE_TRANSFORM_ANGLE", 6 },
    { "STYLE_TRANSFORM_HEIGHT", 5 },
    { "STYLE_TRANSFORM_WIDTH", 4 },
    { "STYLE_TRANSFORM_ZOOM", 7 },
    { "STYLE_TRANSITION_DELAY", 177 },
    { "STYLE_TRANSITION_PATH", 190 },
    { "STYLE_TRANSITION_PROP_1", 178 },
    { "STYLE_TRANSITION_PROP_2", 179 },
    { "STYLE_TRANSITION_PROP_3", 180 },
    { "STYLE_TRANSITION_PROP_4", 181 },
    { "STYLE_TRANSITION_PROP_5", 182 },
    { "STYLE_TRANSITION_PROP_6", 183 },
    { "STYLE_TRANSITION_TIME", 176 },
    { "STYLE_VALUE_ALIGN", 117 },
    { "STYLE_VALUE_BLEND_MODE", 114 },
    { "STYLE_VALUE_COLOR", 121 },
    { "STYLE_VALUE_FONT", 126 },
    { "STYLE_VALUE_LETTER_SPACE", 112 },
    { "STYLE_VALUE_LINE_SPACE", 113 },
    { "STYLE_VALUE_OFS_X", 115 },
    { "STYLE_VALUE_OFS_Y", 116 },
    { "STYLE_VALUE_OPA", 124 },
    { "STYLE_VALUE_STR", 127 },
    { "TABVIEW_TAB_POS_BOTTOM", 2 },
    { "TABVIEW_TAB_POS_LEFT", 3 },
    { "TABVIEW_TAB_POS_NONE", 0 },
    { "TABVIEW_TAB_POS_RIGHT", 4 },
    { "TABVIEW_TAB_POS_TOP", 1 },
    { "TEAL", 32896 },
    { "TEMPL_STYLE_X", 0 },
    { "TEMPL_STYLE_Y", 1 },
    { "TEXTAREA_CURSOR_LAST", 32767 },
    { "TEXT_DECOR_NONE", 0 },
    { "TEXT_DECOR_STRIKETHROUGH", 2 },
    { "TEXT_DECOR_UNDERLINE", 1 },
    { "TXT_CMD_STATE_IN", 2 },
    { "TXT_CMD_STATE_PAR", 1 },
    { "TXT_CMD_STATE_WAIT", 0 },
    { "TXT_FLAG_CENTER", 4 },
    { "TXT_FLAG_EXPAND", 2 },
    { "TXT_FLAG_FIT", 16 },
    { "TXT_FLAG_NONE", 0 },
    { "TXT_FLAG_RECOLOR", 1 },
    { "TXT_FLAG_RIGHT", 8 },
    { "WHITE", 16777215 },
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


    // screen and layers
    be_native_module_function("scr_act", lv0_scr_act),
    be_native_module_function("layer_top", lv0_layer_top),
    be_native_module_function("layer_sys", lv0_layer_sys),
    be_native_module_function("get_hor_res", lv0_get_hor_res),
    be_native_module_function("get_ver_res", lv0_get_ver_res),
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

    scr_act, func(lv0_scr_act)
    layer_top, func(lv0_layer_top)
    layer_sys, func(lv0_layer_sys)
    get_hor_res, func(lv0_get_hor_res)
    get_ver_res, func(lv0_get_ver_res)
    screenshot, func(lv0_screenshot)



}
@const_object_info_end */
#include "../generate/be_fixed_lvgl.h"
#endif

#endif // USE_LVGL

/********************************************************************/
