/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
/********************************************************************
 * LVGL Module
 *******************************************************************/
#include "be_object.h"
#include "be_string.h"
#include "be_gc.h"

#ifdef USE_LVGL

#include "lvgl.h"

extern int lv0_start(bvm *vm);
extern int lv0_load_montserrat_font(bvm *vm);

extern int lv0_scr_act(bvm *vm);
extern int lv0_layer_top(bvm *vm);
extern int lv0_layer_sys(bvm *vm);
extern int lv0_get_hor_res(bvm *vm);
extern int lv0_get_ver_res(bvm *vm);
extern int lv0_screenshot(bvm *vm);


#if !BE_USE_PRECOMPILED_OBJECT
//#if 1           // TODO we will do pre-compiled later


be_native_module_attr_table(lvgl) {
    // Symbols    be_native_module_str("SYMBOL_AUDIO", "\xef\x80\x81"),
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

    // connection type
    be_native_module_int("SPI", 0),
    be_native_module_int("I2C", 1),
    // connection sub_type
    be_native_module_int("HSPI", 0),
    be_native_module_int("VSPI", 1),
    be_native_module_int("SSPI", 2),

/* `lvgl` module functions */

    be_native_module_int("ALIGN_CENTER", 0),
    be_native_module_int("ALIGN_IN_TOP_LEFT", 1),
    be_native_module_int("ALIGN_IN_TOP_MID", 2),
    be_native_module_int("ALIGN_IN_TOP_RIGHT", 3),
    be_native_module_int("ALIGN_IN_BOTTOM_LEFT", 4),
    be_native_module_int("ALIGN_IN_BOTTOM_MID", 5),
    be_native_module_int("ALIGN_IN_BOTTOM_RIGHT", 6),
    be_native_module_int("ALIGN_IN_LEFT_MID", 7),
    be_native_module_int("ALIGN_IN_RIGHT_MID", 8),
    be_native_module_int("ALIGN_OUT_TOP_LEFT", 9),
    be_native_module_int("ALIGN_OUT_TOP_MID", 10),
    be_native_module_int("ALIGN_OUT_TOP_RIGHT", 11),
    be_native_module_int("ALIGN_OUT_BOTTOM_LEFT", 12),
    be_native_module_int("ALIGN_OUT_BOTTOM_MID", 13),
    be_native_module_int("ALIGN_OUT_BOTTOM_RIGHT", 14),
    be_native_module_int("ALIGN_OUT_LEFT_TOP", 15),
    be_native_module_int("ALIGN_OUT_LEFT_MID", 16),
    be_native_module_int("ALIGN_OUT_LEFT_BOTTOM", 17),
    be_native_module_int("ALIGN_OUT_RIGHT_TOP", 18),
    be_native_module_int("ALIGN_OUT_RIGHT_MID", 19),
    be_native_module_int("ALIGN_OUT_RIGHT_BOTTOM", 20),
    be_native_module_int("DISP_ROT_NONE", 0),
    be_native_module_int("DISP_ROT_90", 1),
    be_native_module_int("DISP_ROT_180", 2),
    be_native_module_int("DISP_ROT_270", 3),
    be_native_module_int("DISP_SIZE_SMALL", 0),
    be_native_module_int("DISP_SIZE_MEDIUM", 1),
    be_native_module_int("DISP_SIZE_LARGE", 2),
    be_native_module_int("DISP_SIZE_EXTRA_LARGE", 3),
    be_native_module_int("DRAG_DIR_HOR", 1),
    be_native_module_int("DRAG_DIR_VER", 2),
    be_native_module_int("DRAG_DIR_BOTH", 3),
    be_native_module_int("DRAG_DIR_ONE", 4),
    be_native_module_int("GESTURE_DIR_TOP", 0),
    be_native_module_int("GESTURE_DIR_BOTTOM", 1),
    be_native_module_int("GESTURE_DIR_LEFT", 2),
    be_native_module_int("GESTURE_DIR_RIGHT", 3),
    be_native_module_int("ANIM_OFF", 0),
    be_native_module_int("ANIM_ON", 1),
    be_native_module_int("BLEND_MODE_NORMAL", 0),
    be_native_module_int("BLEND_MODE_ADDITIVE", 1),
    be_native_module_int("BLEND_MODE_SUBTRACTIVE", 2),
// Obj parts
    be_native_module_int("OBJ_PART_MAIN", 0),
    be_native_module_int("OBJ_PART_VIRTUAL_FIRST", 1),
    be_native_module_int("OBJ_PART_REAL_FIRST", 64),
    be_native_module_int("OBJ_PART_ALL", 255),
// LV State
    be_native_module_int("STATE_DEFAULT", 0),
    be_native_module_int("STATE_CHECKED", 1),
    be_native_module_int("STATE_FOCUSED", 2),
    be_native_module_int("STATE_EDITED", 4),
    be_native_module_int("STATE_HOVERED", 8),
    be_native_module_int("STATE_PRESSED", 16),
    be_native_module_int("STATE_DISABLED", 32),
// OPA opacity
    be_native_module_int("OPA_TRANSP", 0),
    be_native_module_int("OPA_0", 0),
    be_native_module_int("OPA_10", 25),
    be_native_module_int("OPA_20", 51),
    be_native_module_int("OPA_30", 76),
    be_native_module_int("OPA_40", 102),
    be_native_module_int("OPA_50", 127),
    be_native_module_int("OPA_60", 153),
    be_native_module_int("OPA_70", 178),
    be_native_module_int("OPA_80", 204),
    be_native_module_int("OPA_90", 229),
    be_native_module_int("OPA_100", 255),
    be_native_module_int("OPA_COVER", 255),
// LV Colors - we store in 24 bits format and will convert at runtime
    be_native_module_int("WHITE", 16777215),
    be_native_module_int("SILVER", 12632256),
    be_native_module_int("GRAY", 8421504),
    be_native_module_int("BLACK", 0),
    be_native_module_int("RED", 16711680),
    be_native_module_int("MAROON", 8388608),
    be_native_module_int("YELLOW", 16776960),
    be_native_module_int("OLIVE", 8421376),
    be_native_module_int("LIME", 65280),
    be_native_module_int("GREEN", 32768),
    be_native_module_int("CYAN", 65535),
    be_native_module_int("AQUA", 65535),
    be_native_module_int("TEAL", 32896),
    be_native_module_int("BLUE", 255),
    be_native_module_int("NAVY", 128),
    be_native_module_int("MAGENTA", 16711935),
    be_native_module_int("PURPLE", 8388736),
// LV Groups
    be_native_module_int("KEY_UP", 17),
    be_native_module_int("KEY_DOWN", 18),
    be_native_module_int("KEY_RIGHT", 19),
    be_native_module_int("KEY_LEFT", 20),
    be_native_module_int("KEY_ESC", 27),
    be_native_module_int("KEY_DEL", 127),
    be_native_module_int("KEY_BACKSPACE", 8),
    be_native_module_int("KEY_ENTER", 10),
    be_native_module_int("KEY_NEXT", 9),
    be_native_module_int("KEY_PREV", 11),
    be_native_module_int("KEY_HOME", 2),
    be_native_module_int("KEY_END", 3),
// LV Style
    be_native_module_int("BORDER_SIDE_NONE", 0),
    be_native_module_int("BORDER_SIDE_BOTTOM", 1),
    be_native_module_int("BORDER_SIDE_TOP", 2),
    be_native_module_int("BORDER_SIDE_LEFT", 4),
    be_native_module_int("BORDER_SIDE_RIGHT", 8),
    be_native_module_int("BORDER_SIDE_FULL", 15),
    be_native_module_int("BORDER_SIDE_INTERNAL", 16),
    be_native_module_int("GRAD_DIR_NONE", 0),
    be_native_module_int("GRAD_DIR_VER", 1),
    be_native_module_int("GRAD_DIR_HOR", 2),
    be_native_module_int("TEXT_DECOR_NONE", 0),
    be_native_module_int("TEXT_DECOR_UNDERLINE", 1),
    be_native_module_int("TEXT_DECOR_STRIKETHROUGH", 2),
// LV Styles parts
    be_native_module_int("STYLE_RADIUS", 1),
    be_native_module_int("STYLE_CLIP_CORNER", 2),
    be_native_module_int("STYLE_SIZE", 3),
    be_native_module_int("STYLE_TRANSFORM_WIDTH", 4),
    be_native_module_int("STYLE_TRANSFORM_HEIGHT", 5),
    be_native_module_int("STYLE_TRANSFORM_ANGLE", 6),
    be_native_module_int("STYLE_TRANSFORM_ZOOM", 7),
    be_native_module_int("STYLE_OPA_SCALE", 32780),
    be_native_module_int("STYLE_PAD_TOP", 16),
    be_native_module_int("STYLE_PAD_BOTTOM", 17),
    be_native_module_int("STYLE_PAD_LEFT", 18),
    be_native_module_int("STYLE_PAD_RIGHT", 19),
    be_native_module_int("STYLE_PAD_INNER", 20),
    be_native_module_int("STYLE_MARGIN_TOP", 21),
    be_native_module_int("STYLE_MARGIN_BOTTOM", 22),
    be_native_module_int("STYLE_MARGIN_LEFT", 23),
    be_native_module_int("STYLE_MARGIN_RIGHT", 24),
    be_native_module_int("STYLE_BG_BLEND_MODE", 32),
    be_native_module_int("STYLE_BG_MAIN_STOP", 33),
    be_native_module_int("STYLE_BG_GRAD_STOP", 34),
    be_native_module_int("STYLE_BG_GRAD_DIR", 35),
    be_native_module_int("STYLE_BG_COLOR", 41),
    be_native_module_int("STYLE_BG_GRAD_COLOR", 42),
    be_native_module_int("STYLE_BG_OPA", 44),
    be_native_module_int("STYLE_BORDER_WIDTH", 48),
    be_native_module_int("STYLE_BORDER_SIDE", 49),
    be_native_module_int("STYLE_BORDER_BLEND_MODE", 50),
    be_native_module_int("STYLE_BORDER_POST", 51),
    be_native_module_int("STYLE_BORDER_COLOR", 57),
    be_native_module_int("STYLE_BORDER_OPA", 60),
    be_native_module_int("STYLE_OUTLINE_WIDTH", 64),
    be_native_module_int("STYLE_OUTLINE_PAD", 65),
    be_native_module_int("STYLE_OUTLINE_BLEND_MODE", 66),
    be_native_module_int("STYLE_OUTLINE_COLOR", 73),
    be_native_module_int("STYLE_OUTLINE_OPA", 76),
    be_native_module_int("STYLE_SHADOW_WIDTH", 80),
    be_native_module_int("STYLE_SHADOW_OFS_X", 81),
    be_native_module_int("STYLE_SHADOW_OFS_Y", 82),
    be_native_module_int("STYLE_SHADOW_SPREAD", 83),
    be_native_module_int("STYLE_SHADOW_BLEND_MODE", 84),
    be_native_module_int("STYLE_SHADOW_COLOR", 89),
    be_native_module_int("STYLE_SHADOW_OPA", 92),
    be_native_module_int("STYLE_PATTERN_BLEND_MODE", 96),
    be_native_module_int("STYLE_PATTERN_REPEAT", 97),
    be_native_module_int("STYLE_PATTERN_RECOLOR", 105),
    be_native_module_int("STYLE_PATTERN_OPA", 108),
    be_native_module_int("STYLE_PATTERN_RECOLOR_OPA", 109),
    be_native_module_int("STYLE_PATTERN_IMAGE", 110),
    be_native_module_int("STYLE_VALUE_LETTER_SPACE", 112),
    be_native_module_int("STYLE_VALUE_LINE_SPACE", 113),
    be_native_module_int("STYLE_VALUE_BLEND_MODE", 114),
    be_native_module_int("STYLE_VALUE_OFS_X", 115),
    be_native_module_int("STYLE_VALUE_OFS_Y", 116),
    be_native_module_int("STYLE_VALUE_ALIGN", 117),
    be_native_module_int("STYLE_VALUE_COLOR", 121),
    be_native_module_int("STYLE_VALUE_OPA", 124),
    be_native_module_int("STYLE_VALUE_FONT", 126),
    be_native_module_int("STYLE_VALUE_STR", 127),
    be_native_module_int("STYLE_TEXT_LETTER_SPACE", 32896),
    be_native_module_int("STYLE_TEXT_LINE_SPACE", 32897),
    be_native_module_int("STYLE_TEXT_DECOR", 32898),
    be_native_module_int("STYLE_TEXT_BLEND_MODE", 32899),
    be_native_module_int("STYLE_TEXT_COLOR", 32905),
    be_native_module_int("STYLE_TEXT_SEL_COLOR", 32906),
    be_native_module_int("STYLE_TEXT_SEL_BG_COLOR", 32907),
    be_native_module_int("STYLE_TEXT_OPA", 32908),
    be_native_module_int("STYLE_TEXT_FONT", 32910),
    be_native_module_int("STYLE_LINE_WIDTH", 144),
    be_native_module_int("STYLE_LINE_BLEND_MODE", 145),
    be_native_module_int("STYLE_LINE_DASH_WIDTH", 146),
    be_native_module_int("STYLE_LINE_DASH_GAP", 147),
    be_native_module_int("STYLE_LINE_ROUNDED", 148),
    be_native_module_int("STYLE_LINE_COLOR", 153),
    be_native_module_int("STYLE_LINE_OPA", 156),
    be_native_module_int("STYLE_IMAGE_BLEND_MODE", 32928),
    be_native_module_int("STYLE_IMAGE_RECOLOR", 32937),
    be_native_module_int("STYLE_IMAGE_OPA", 32940),
    be_native_module_int("STYLE_IMAGE_RECOLOR_OPA", 32941),
    be_native_module_int("STYLE_TRANSITION_TIME", 176),
    be_native_module_int("STYLE_TRANSITION_DELAY", 177),
    be_native_module_int("STYLE_TRANSITION_PROP_1", 178),
    be_native_module_int("STYLE_TRANSITION_PROP_2", 179),
    be_native_module_int("STYLE_TRANSITION_PROP_3", 180),
    be_native_module_int("STYLE_TRANSITION_PROP_4", 181),
    be_native_module_int("STYLE_TRANSITION_PROP_5", 182),
    be_native_module_int("STYLE_TRANSITION_PROP_6", 183),
    be_native_module_int("STYLE_TRANSITION_PATH", 190),
    be_native_module_int("STYLE_SCALE_WIDTH", 192),
    be_native_module_int("STYLE_SCALE_BORDER_WIDTH", 193),
    be_native_module_int("STYLE_SCALE_END_BORDER_WIDTH", 194),
    be_native_module_int("STYLE_SCALE_END_LINE_WIDTH", 195),
    be_native_module_int("STYLE_SCALE_GRAD_COLOR", 201),
    be_native_module_int("STYLE_SCALE_END_COLOR", 202),
    be_native_module_int("TXT_FLAG_NONE", 0),
    be_native_module_int("TXT_FLAG_RECOLOR", 1),
    be_native_module_int("TXT_FLAG_EXPAND", 2),
    be_native_module_int("TXT_FLAG_CENTER", 4),
    be_native_module_int("TXT_FLAG_RIGHT", 8),
    be_native_module_int("TXT_FLAG_FIT", 16),
    be_native_module_int("TXT_CMD_STATE_WAIT", 0),
    be_native_module_int("TXT_CMD_STATE_PAR", 1),
    be_native_module_int("TXT_CMD_STATE_IN", 2),
    be_native_module_int("FS_RES_OK", 0),
    be_native_module_int("FS_RES_HW_ERR", 1),
    be_native_module_int("FS_RES_FS_ERR", 2),
    be_native_module_int("FS_RES_NOT_EX", 3),
    be_native_module_int("FS_RES_FULL", 4),
    be_native_module_int("FS_RES_LOCKED", 5),
    be_native_module_int("FS_RES_DENIED", 6),
    be_native_module_int("FS_RES_BUSY", 7),
    be_native_module_int("FS_RES_TOUT", 8),
    be_native_module_int("FS_RES_NOT_IMP", 9),
    be_native_module_int("FS_RES_OUT_OF_MEM", 10),
    be_native_module_int("FS_RES_INV_PARAM", 11),
    be_native_module_int("FS_RES_UNKNOWN", 12),
    be_native_module_int("FS_MODE_WR", 1),
    be_native_module_int("FS_MODE_RD", 2),
    be_native_module_int("EVENT_PRESSED", 0),
    be_native_module_int("EVENT_PRESSING", 1),
    be_native_module_int("EVENT_PRESS_LOST", 2),
    be_native_module_int("EVENT_SHORT_CLICKED", 3),
    be_native_module_int("EVENT_LONG_PRESSED", 4),
    be_native_module_int("EVENT_LONG_PRESSED_REPEAT", 5),
    be_native_module_int("EVENT_CLICKED", 6),
    be_native_module_int("EVENT_RELEASED", 7),
    be_native_module_int("EVENT_DRAG_BEGIN", 8),
    be_native_module_int("EVENT_DRAG_END", 9),
    be_native_module_int("EVENT_DRAG_THROW_BEGIN", 10),
    be_native_module_int("EVENT_GESTURE", 11),
    be_native_module_int("EVENT_KEY", 12),
    be_native_module_int("EVENT_FOCUSED", 13),
    be_native_module_int("EVENT_DEFOCUSED", 14),
    be_native_module_int("EVENT_LEAVE", 15),
    be_native_module_int("EVENT_VALUE_CHANGED", 16),
    be_native_module_int("EVENT_INSERT", 17),
    be_native_module_int("EVENT_REFRESH", 18),
    be_native_module_int("EVENT_APPLY", 19),
    be_native_module_int("EVENT_CANCEL", 20),
    be_native_module_int("EVENT_DELETE", 21),
    be_native_module_int("PROTECT_NONE", 0),
    be_native_module_int("PROTECT_CHILD_CHG", 1),
    be_native_module_int("PROTECT_PARENT", 2),
    be_native_module_int("PROTECT_POS", 4),
    be_native_module_int("PROTECT_FOLLOW", 8),
    be_native_module_int("PROTECT_PRESS_LOST", 16),
    be_native_module_int("PROTECT_CLICK_FOCUS", 32),
    be_native_module_int("PROTECT_EVENT_TO_DISABLED", 64),
// LV Widgets
// LV Arc
    be_native_module_int("ARC_TYPE_NORMAL", 0),
    be_native_module_int("ARC_TYPE_SYMMETRIC", 1),
    be_native_module_int("ARC_TYPE_REVERSE", 2),
    be_native_module_int("ARC_PART_BG", 0),
    be_native_module_int("ARC_PART_INDIC", 1),
    be_native_module_int("ARC_PART_KNOB", 2),
// LV Bar
    be_native_module_int("BAR_TYPE_NORMAL", 0),
    be_native_module_int("BAR_TYPE_SYMMETRICAL", 1),
    be_native_module_int("BAR_TYPE_CUSTOM", 2),
// Lv Btn
    be_native_module_int("BTN_STATE_RELEASED", 0),
    be_native_module_int("BTN_STATE_PRESSED", 1),
    be_native_module_int("BTN_STATE_DISABLED", 2),
    be_native_module_int("BTN_STATE_CHECKED_RELEASED", 3),
    be_native_module_int("BTN_STATE_CHECKED_PRESSED", 4),
    be_native_module_int("BTN_STATE_CHECKED_DISABLED", 5),
// Lv BtnMatrix
    be_native_module_int("BTNMATRIX_CTRL_HIDDEN", 8),
    be_native_module_int("BTNMATRIX_CTRL_NO_REPEAT", 16),
    be_native_module_int("BTNMATRIX_CTRL_DISABLED", 32),
    be_native_module_int("BTNMATRIX_CTRL_CHECKABLE", 64),
    be_native_module_int("BTNMATRIX_CTRL_CHECK_STATE", 128),
    be_native_module_int("BTNMATRIX_CTRL_CLICK_TRIG", 256),
// LV Calendar
    be_native_module_int("CALENDAR_PART_BG", 0),
    be_native_module_int("CALENDAR_PART_HEADER", 1),
    be_native_module_int("CALENDAR_PART_DAY_NAMES", 2),
    be_native_module_int("CALENDAR_PART_DATE", 3),
// LV Chart
    be_native_module_int("CHART_TYPE_NONE", 0),
    be_native_module_int("CHART_TYPE_LINE", 1),
    be_native_module_int("CHART_TYPE_COLUMN", 2),
    be_native_module_int("CHART_UPDATE_MODE_SHIFT", 0),
    be_native_module_int("CHART_UPDATE_MODE_CIRCULAR", 1),
    be_native_module_int("CHART_AXIS_PRIMARY_Y", 0),
    be_native_module_int("CHART_AXIS_SECONDARY_Y", 1),
    be_native_module_int("CHART_CURSOR_NONE", 0),
    be_native_module_int("CHART_CURSOR_RIGHT", 1),
    be_native_module_int("CHART_CURSOR_UP", 2),
    be_native_module_int("CHART_CURSOR_LEFT", 4),
    be_native_module_int("CHART_CURSOR_DOWN", 8),
    be_native_module_int("CHART_AXIS_SKIP_LAST_TICK", 0),
    be_native_module_int("CHART_AXIS_DRAW_LAST_TICK", 1),
    be_native_module_int("CHART_AXIS_INVERSE_LABELS_ORDER", 2),
    be_native_module_int("CHART_PART_BG", 0),
    be_native_module_int("CHART_PART_SERIES_BG", 1),
    be_native_module_int("CHART_PART_SERIES", 2),
    be_native_module_int("CHART_PART_CURSOR", 3),
// LV Checkbox
    be_native_module_int("CHECKBOX_PART_BG", 0),
    be_native_module_int("CHECKBOX_PART_BULLET", 64),
// LV Cont
    be_native_module_int("LAYOUT_OFF", 0),
    be_native_module_int("LAYOUT_CENTER", 1),
    be_native_module_int("LAYOUT_COLUMN_LEFT", 2),
    be_native_module_int("LAYOUT_COLUMN_MID", 3),
    be_native_module_int("LAYOUT_COLUMN_RIGHT", 4),
    be_native_module_int("LAYOUT_ROW_TOP", 5),
    be_native_module_int("LAYOUT_ROW_MID", 6),
    be_native_module_int("LAYOUT_ROW_BOTTOM", 7),
    be_native_module_int("LAYOUT_PRETTY_TOP", 8),
    be_native_module_int("LAYOUT_PRETTY_MID", 9),
    be_native_module_int("LAYOUT_PRETTY_BOTTOM", 10),
    be_native_module_int("LAYOUT_GRID", 11),
    be_native_module_int("FIT_NONE", 0),
    be_native_module_int("FIT_TIGHT", 1),
    be_native_module_int("FIT_PARENT", 2),
    be_native_module_int("FIT_MAX", 3),
// LV Cpicker
    be_native_module_int("CPICKER_TYPE_RECT", 0),
    be_native_module_int("CPICKER_TYPE_DISC", 1),
    be_native_module_int("CPICKER_COLOR_MODE_HUE", 0),
    be_native_module_int("CPICKER_COLOR_MODE_SATURATION", 1),
    be_native_module_int("CPICKER_COLOR_MODE_VALUE", 2),
    be_native_module_int("CPICKER_PART_MAIN", 0),
    be_native_module_int("CPICKER_PART_KNOB", 1),
// LV Dropdown
    be_native_module_int("DROPDOWN_DIR_DOWN", 0),
    be_native_module_int("DROPDOWN_DIR_UP", 1),
    be_native_module_int("DROPDOWN_DIR_LEFT", 2),
    be_native_module_int("DROPDOWN_DIR_RIGHT", 3),
    be_native_module_int("DROPDOWN_PART_MAIN", 0),
    be_native_module_int("DROPDOWN_PART_LIST", 64),
    be_native_module_int("DROPDOWN_PART_SCROLLBAR", 65),
    be_native_module_int("DROPDOWN_PART_SELECTED", 66),
// LV Gauge
    be_native_module_int("GAUGE_PART_MAIN", 0),
    be_native_module_int("GAUGE_PART_MAJOR", 1),
    be_native_module_int("GAUGE_PART_NEEDLE", 2),
// LV Img
// LV Imgbtn
// LV Keyboard
    be_native_module_int("KEYBOARD_MODE_TEXT_LOWER", 0),
    be_native_module_int("KEYBOARD_MODE_TEXT_UPPER", 1),
    be_native_module_int("KEYBOARD_MODE_SPECIAL", 2),
    be_native_module_int("KEYBOARD_MODE_NUM", 3),
    be_native_module_int("KEYBOARD_PART_BG", 0),
    be_native_module_int("KEYBOARD_PART_BTN", 1),
// LV Label
    be_native_module_int("LABEL_LONG_EXPAND", 0),
    be_native_module_int("LABEL_LONG_BREAK", 1),
    be_native_module_int("LABEL_LONG_DOT", 2),
    be_native_module_int("LABEL_LONG_SROLL", 3),
    be_native_module_int("LABEL_LONG_SROLL_CIRC", 4),
    be_native_module_int("LABEL_LONG_CROP", 5),
    be_native_module_int("LABEL_ALIGN_LEFT", 0),
    be_native_module_int("LABEL_ALIGN_CENTER", 1),
    be_native_module_int("LABEL_ALIGN_RIGHT", 2),
    be_native_module_int("LABEL_ALIGN_AUTO", 3),
// LV Led
    be_native_module_int("LED_PART_MAIN", 0),
// LV Line
    be_native_module_int("LINEMETER_PART_MAIN", 0),
// LV List
    be_native_module_int("LIST_PART_BG", 0),
    be_native_module_int("LIST_PART_SCROLLBAR", 1),
    be_native_module_int("LIST_PART_EDGE_FLASH", 2),
// LV Msgbox
// enum {
//     LV_MSGBOX_PART_BG = LV_CONT_PART_MAIN,
//     LV_MSGBOX_PART_BTN_BG = _LV_CONT_PART_REAL_LAST,
//     LV_MSGBOX_PART_BTN,
// };
// LV Objmask
    be_native_module_int("OBJMASK_PART_MAIN", 0),
// LV Templ
    be_native_module_int("TEMPL_STYLE_X", 0),
    be_native_module_int("TEMPL_STYLE_Y", 1),
// LV Page
    be_native_module_int("SCROLLBAR_MODE_OFF", 0),
    be_native_module_int("SCROLLBAR_MODE_ON", 1),
    be_native_module_int("SCROLLBAR_MODE_DRAG", 2),
    be_native_module_int("SCROLLBAR_MODE_AUTO", 3),
    be_native_module_int("SCROLLBAR_MODE_HIDE", 4),
    be_native_module_int("SCROLLBAR_MODE_UNHIDE", 8),
    be_native_module_int("PAGE_EDGE_LEFT", 1),
    be_native_module_int("PAGE_EDGE_TOP", 2),
    be_native_module_int("PAGE_EDGE_RIGHT", 4),
    be_native_module_int("PAGE_EDGE_BOTTOM", 8),
// enum {
//     LV_PAGE_PART_BG = LV_CONT_PART_MAIN,
//     LV_PAGE_PART_SCROLLBAR = _LV_OBJ_PART_VIRTUAL_LAST,
//     LV_PAGE_PART_EDGE_FLASH,
//     _LV_PAGE_PART_VIRTUAL_LAST,
//     LV_PAGE_PART_SCROLLABLE = _LV_OBJ_PART_REAL_LAST,
//     _LV_PAGE_PART_REAL_LAST,
// };
// LV Roller
    be_native_module_int("ROLLER_MODE_NORMAL", 0),
    be_native_module_int("ROLLER_MODE_INFINITE", 1),
// enum {
//     LV_ROLLER_PART_BG = LV_PAGE_PART_BG,
//     LV_ROLLER_PART_SELECTED = _LV_PAGE_PART_VIRTUAL_LAST,
//     _LV_ROLLER_PART_VIRTUAL_LAST,
// };
// LV Slider
    be_native_module_int("SLIDER_TYPE_NORMAL", 0),
    be_native_module_int("SLIDER_TYPE_SYMMETRICAL", 1),
    be_native_module_int("SLIDER_TYPE_RANGE", 2),
// enum {
//     LV_SLIDER_PART_BG, /** Slider background style. */
//     LV_SLIDER_PART_INDIC, /** Slider indicator (filled area) style. */
//     LV_SLIDER_PART_KNOB, /** Slider knob style. */
// };
// LV Spinbox
// enum {
//     LV_SPINBOX_PART_BG = LV_TEXTAREA_PART_BG,
//     LV_SPINBOX_PART_CURSOR = LV_TEXTAREA_PART_CURSOR,
//     _LV_SPINBOX_PART_VIRTUAL_LAST = _LV_TEXTAREA_PART_VIRTUAL_LAST,
//     _LV_SPINBOX_PART_REAL_LAST = _LV_TEXTAREA_PART_REAL_LAST,
// };
// LV Spinner
    be_native_module_int("SPINNER_TYPE_SPINNING_ARC", 0),
    be_native_module_int("SPINNER_TYPE_FILLSPIN_ARC", 1),
    be_native_module_int("SPINNER_TYPE_CONSTANT_ARC", 2),
    be_native_module_int("SPINNER_DIR_FORWARD", 0),
    be_native_module_int("SPINNER_DIR_BACKWARD", 1),
// enum {
//     LV_SPINNER_PART_BG = LV_ARC_PART_BG,
//     LV_SPINNER_PART_INDIC = LV_ARC_PART_INDIC,
//     _LV_SPINNER_PART_VIRTUAL_LAST,
//     _LV_SPINNER_PART_REAL_LAST = _LV_ARC_PART_REAL_LAST,
// };
// LV Switch
// enum {
//     LV_SWITCH_PART_BG = LV_BAR_PART_BG,                 /**< Switch background. */
//     LV_SWITCH_PART_INDIC = LV_BAR_PART_INDIC,           /**< Switch fill area. */
//     LV_SWITCH_PART_KNOB = _LV_BAR_PART_VIRTUAL_LAST,    /**< Switch knob. */
//     _LV_SWITCH_PART_VIRTUAL_LAST
// };
// LV Table
// enum {
//     LV_TABLE_PART_BG,     /* Because of this member, LV_PART.*CELL1 has enum value of 1,        */
//     LV_TABLE_PART_CELL1,  /*   LV_PART.*CELL2 has an enum value of 2 and so on up to the maximum */
//     LV_TABLE_PART_CELL2,  /*   number of styles specified by LV_TABLE_CELL_STYLE_CNT            */
//     LV_TABLE_PART_CELL3,
//     LV_TABLE_PART_CELL4,  /* CELL 5-16 are not needed to be defined, the values in this enum
//                              are there for backward compatibility */
// };
// LV Tabview
    be_native_module_int("TABVIEW_TAB_POS_NONE", 0),
    be_native_module_int("TABVIEW_TAB_POS_TOP", 1),
    be_native_module_int("TABVIEW_TAB_POS_BOTTOM", 2),
    be_native_module_int("TABVIEW_TAB_POS_LEFT", 3),
    be_native_module_int("TABVIEW_TAB_POS_RIGHT", 4),
// enum {
//     LV_TABVIEW_PART_BG = LV_OBJ_PART_MAIN,
//     _LV_TABVIEW_PART_VIRTUAL_LAST = _LV_OBJ_PART_VIRTUAL_LAST,
//     LV_TABVIEW_PART_BG_SCROLLABLE = _LV_OBJ_PART_REAL_LAST,
//     LV_TABVIEW_PART_TAB_BG,
//     LV_TABVIEW_PART_TAB_BTN,
//     LV_TABVIEW_PART_INDIC,
//     _LV_TABVIEW_PART_REAL_LAST,
// };
// LV Textarea
    be_native_module_int("TEXTAREA_CURSOR_LAST", 32767),
// enum {
//     LV_TEXTAREA_PART_BG = LV_PAGE_PART_BG, /**< Text area background style */
//     LV_TEXTAREA_PART_SCROLLBAR = LV_PAGE_PART_SCROLLBAR, /**< Scrollbar style */
//     LV_TEXTAREA_PART_EDGE_FLASH = LV_PAGE_PART_EDGE_FLASH, /**< Edge flash style */
//     LV_TEXTAREA_PART_CURSOR = _LV_PAGE_PART_VIRTUAL_LAST, /**< Cursor style */
//     LV_TEXTAREA_PART_PLACEHOLDER, /**< Placeholder style */
//     _LV_TEXTAREA_PART_VIRTUAL_LAST,
//     _LV_TEXTAREA_PART_REAL_LAST = _LV_PAGE_PART_REAL_LAST,
// };
// LV Tileview
// enum {
//     LV_TILEVIEW_PART_BG = LV_PAGE_PART_BG,
//     LV_TILEVIEW_PART_SCROLLBAR = LV_PAGE_PART_SCROLLBAR,
//     LV_TILEVIEW_PART_EDGE_FLASH = LV_PAGE_PART_EDGE_FLASH,
//     _LV_TILEVIEW_PART_VIRTUAL_LAST = _LV_PAGE_PART_VIRTUAL_LAST,
//     _LV_TILEVIEW_PART_REAL_LAST = _LV_PAGE_PART_REAL_LAST
// };
// LV Win
// enum {
//     LV_WIN_PART_BG = LV_OBJ_PART_MAIN, /**< Window object background style. */
//     _LV_WIN_PART_VIRTUAL_LAST,
//     LV_WIN_PART_HEADER = _LV_OBJ_PART_REAL_LAST, /**< Window titlebar background style. */
//     LV_WIN_PART_CONTENT_SCROLLABLE, /**< Window content style. */
//     LV_WIN_PART_SCROLLBAR, /**< Window scrollbar style. */
//     _LV_WIN_PART_REAL_LAST
// };


    be_native_module_function("start", lv0_start),
    be_native_module_function("montserrat_font", lv0_load_montserrat_font),

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

be_define_local_const_str(SYMBOL_AUDIO, "\xef\x80\x81", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_VIDEO, "\xef\x80\x88", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_LIST, "\xef\x80\x8b", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_OK, "\xef\x80\x8c", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_CLOSE, "\xef\x80\x8d", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_POWER, "\xef\x80\x91", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_SETTINGS, "\xef\x80\x93", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_HOME, "\xef\x80\x95", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_DOWNLOAD, "\xef\x80\x99", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_DRIVE, "\xef\x80\x9c", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_REFRESH, "\xef\x80\xa1", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_MUTE, "\xef\x80\xa6", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_VOLUME_MID, "\xef\x80\xa7", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_VOLUME_MAX, "\xef\x80\xa8", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_IMAGE, "\xef\x80\xbe", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_EDIT, "\xef\x8C\x84", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_PREV, "\xef\x81\x88", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_PLAY, "\xef\x81\x8b", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_PAUSE, "\xef\x81\x8c", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_STOP, "\xef\x81\x8d", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_NEXT, "\xef\x81\x91", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_EJECT, "\xef\x81\x92", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_LEFT, "\xef\x81\x93", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_RIGHT, "\xef\x81\x94", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_PLUS, "\xef\x81\xa7", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_MINUS, "\xef\x81\xa8", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_EYE_OPEN, "\xef\x81\xae", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_EYE_CLOSE, "\xef\x81\xb0", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_WARNING, "\xef\x81\xb1", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_SHUFFLE, "\xef\x81\xb4", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_UP, "\xef\x81\xb7", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_DOWN, "\xef\x81\xb8", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_LOOP, "\xef\x81\xb9", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_DIRECTORY, "\xef\x81\xbb", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_UPLOAD, "\xef\x82\x93", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_CALL, "\xef\x82\x95", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_CUT, "\xef\x83\x84", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_COPY, "\xef\x83\x85", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_SAVE, "\xef\x83\x87", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_CHARGE, "\xef\x83\xa7", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_PASTE, "\xef\x83\xAA", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_BELL, "\xef\x83\xb3", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_KEYBOARD, "\xef\x84\x9c", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_GPS, "\xef\x84\xa4", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_FILE, "\xef\x85\x9b", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_WIFI, "\xef\x87\xab", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_BATTERY_FULL, "\xef\x89\x80", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_BATTERY_3, "\xef\x89\x81", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_BATTERY_2, "\xef\x89\x82", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_BATTERY_1, "\xef\x89\x83", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_BATTERY_EMPTY, "\xef\x89\x84", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_USB, "\xef\x8a\x87", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_BLUETOOTH, "\xef\x8a\x93", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_TRASH, "\xef\x8B\xAD", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_BACKSPACE, "\xef\x95\x9A", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_SD_CARD, "\xef\x9F\x82", 0, 0, 3, 0);
be_define_local_const_str(SYMBOL_NEW_LINE, "\xef\xA2\xA2", 0, 0, 3, 0);

be_define_local_const_str(SYMBOL_DUMMY, "\xEF\xA3\xBF", 0, 0, 3, 0);

be_define_local_const_str(SYMBOL_BULLET, "\xE2\x80\xA2", 0, 0, 3, 0);


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

    SPI, int(0)
    I2C, int(1)
    HSPI, int(0)
    VSPI, int(1)
    SSPI, int(2)

    ALIGN_CENTER, int(0)
    ALIGN_IN_TOP_LEFT, int(1)
    ALIGN_IN_TOP_MID, int(2)
    ALIGN_IN_TOP_RIGHT, int(3)
    ALIGN_IN_BOTTOM_LEFT, int(4)
    ALIGN_IN_BOTTOM_MID, int(5)
    ALIGN_IN_BOTTOM_RIGHT, int(6)
    ALIGN_IN_LEFT_MID, int(7)
    ALIGN_IN_RIGHT_MID, int(8)
    ALIGN_OUT_TOP_LEFT, int(9)
    ALIGN_OUT_TOP_MID, int(10)
    ALIGN_OUT_TOP_RIGHT, int(11)
    ALIGN_OUT_BOTTOM_LEFT, int(12)
    ALIGN_OUT_BOTTOM_MID, int(13)
    ALIGN_OUT_BOTTOM_RIGHT, int(14)
    ALIGN_OUT_LEFT_TOP, int(15)
    ALIGN_OUT_LEFT_MID, int(16)
    ALIGN_OUT_LEFT_BOTTOM, int(17)
    ALIGN_OUT_RIGHT_TOP, int(18)
    ALIGN_OUT_RIGHT_MID, int(19)
    ALIGN_OUT_RIGHT_BOTTOM, int(20)
    DISP_ROT_NONE, int(0)
    DISP_ROT_90, int(1)
    DISP_ROT_180, int(2)
    DISP_ROT_270, int(3)
    DISP_SIZE_SMALL, int(0)
    DISP_SIZE_MEDIUM, int(1)
    DISP_SIZE_LARGE, int(2)
    DISP_SIZE_EXTRA_LARGE, int(3)
    DRAG_DIR_HOR, int(1)
    DRAG_DIR_VER, int(2)
    DRAG_DIR_BOTH, int(3)
    DRAG_DIR_ONE, int(4)
    GESTURE_DIR_TOP, int(0)
    GESTURE_DIR_BOTTOM, int(1)
    GESTURE_DIR_LEFT, int(2)
    GESTURE_DIR_RIGHT, int(3)
    ANIM_OFF, int(0)
    ANIM_ON, int(1)
    BLEND_MODE_NORMAL, int(0)
    BLEND_MODE_ADDITIVE, int(1)
    BLEND_MODE_SUBTRACTIVE, int(2)
    OBJ_PART_MAIN, int(0)
    OBJ_PART_VIRTUAL_FIRST, int(1)
    OBJ_PART_REAL_FIRST, int(64)
    OBJ_PART_ALL, int(255)
    STATE_DEFAULT, int(0)
    STATE_CHECKED, int(1)
    STATE_FOCUSED, int(2)
    STATE_EDITED, int(4)
    STATE_HOVERED, int(8)
    STATE_PRESSED, int(16)
    STATE_DISABLED, int(32)
    OPA_TRANSP, int(0)
    OPA_0, int(0)
    OPA_10, int(25)
    OPA_20, int(51)
    OPA_30, int(76)
    OPA_40, int(102)
    OPA_50, int(127)
    OPA_60, int(153)
    OPA_70, int(178)
    OPA_80, int(204)
    OPA_90, int(229)
    OPA_100, int(255)
    OPA_COVER, int(255)
    WHITE, int(16777215)
    SILVER, int(12632256)
    GRAY, int(8421504)
    BLACK, int(0)
    RED, int(16711680)
    MAROON, int(8388608)
    YELLOW, int(16776960)
    OLIVE, int(8421376)
    LIME, int(65280)
    GREEN, int(32768)
    CYAN, int(65535)
    AQUA, int(65535)
    TEAL, int(32896)
    BLUE, int(255)
    NAVY, int(128)
    MAGENTA, int(16711935)
    PURPLE, int(8388736)
    KEY_UP, int(17)
    KEY_DOWN, int(18)
    KEY_RIGHT, int(19)
    KEY_LEFT, int(20)
    KEY_ESC, int(27)
    KEY_DEL, int(127)
    KEY_BACKSPACE, int(8)
    KEY_ENTER, int(10)
    KEY_NEXT, int(9)
    KEY_PREV, int(11)
    KEY_HOME, int(2)
    KEY_END, int(3)
    BORDER_SIDE_NONE, int(0)
    BORDER_SIDE_BOTTOM, int(1)
    BORDER_SIDE_TOP, int(2)
    BORDER_SIDE_LEFT, int(4)
    BORDER_SIDE_RIGHT, int(8)
    BORDER_SIDE_FULL, int(15)
    BORDER_SIDE_INTERNAL, int(16)
    GRAD_DIR_NONE, int(0)
    GRAD_DIR_VER, int(1)
    GRAD_DIR_HOR, int(2)
    TEXT_DECOR_NONE, int(0)
    TEXT_DECOR_UNDERLINE, int(1)
    TEXT_DECOR_STRIKETHROUGH, int(2)
    STYLE_RADIUS, int(1)
    STYLE_CLIP_CORNER, int(2)
    STYLE_SIZE, int(3)
    STYLE_TRANSFORM_WIDTH, int(4)
    STYLE_TRANSFORM_HEIGHT, int(5)
    STYLE_TRANSFORM_ANGLE, int(6)
    STYLE_TRANSFORM_ZOOM, int(7)
    STYLE_OPA_SCALE, int(32780)
    STYLE_PAD_TOP, int(16)
    STYLE_PAD_BOTTOM, int(17)
    STYLE_PAD_LEFT, int(18)
    STYLE_PAD_RIGHT, int(19)
    STYLE_PAD_INNER, int(20)
    STYLE_MARGIN_TOP, int(21)
    STYLE_MARGIN_BOTTOM, int(22)
    STYLE_MARGIN_LEFT, int(23)
    STYLE_MARGIN_RIGHT, int(24)
    STYLE_BG_BLEND_MODE, int(32)
    STYLE_BG_MAIN_STOP, int(33)
    STYLE_BG_GRAD_STOP, int(34)
    STYLE_BG_GRAD_DIR, int(35)
    STYLE_BG_COLOR, int(41)
    STYLE_BG_GRAD_COLOR, int(42)
    STYLE_BG_OPA, int(44)
    STYLE_BORDER_WIDTH, int(48)
    STYLE_BORDER_SIDE, int(49)
    STYLE_BORDER_BLEND_MODE, int(50)
    STYLE_BORDER_POST, int(51)
    STYLE_BORDER_COLOR, int(57)
    STYLE_BORDER_OPA, int(60)
    STYLE_OUTLINE_WIDTH, int(64)
    STYLE_OUTLINE_PAD, int(65)
    STYLE_OUTLINE_BLEND_MODE, int(66)
    STYLE_OUTLINE_COLOR, int(73)
    STYLE_OUTLINE_OPA, int(76)
    STYLE_SHADOW_WIDTH, int(80)
    STYLE_SHADOW_OFS_X, int(81)
    STYLE_SHADOW_OFS_Y, int(82)
    STYLE_SHADOW_SPREAD, int(83)
    STYLE_SHADOW_BLEND_MODE, int(84)
    STYLE_SHADOW_COLOR, int(89)
    STYLE_SHADOW_OPA, int(92)
    STYLE_PATTERN_BLEND_MODE, int(96)
    STYLE_PATTERN_REPEAT, int(97)
    STYLE_PATTERN_RECOLOR, int(105)
    STYLE_PATTERN_OPA, int(108)
    STYLE_PATTERN_RECOLOR_OPA, int(109)
    STYLE_PATTERN_IMAGE, int(110)
    STYLE_VALUE_LETTER_SPACE, int(112)
    STYLE_VALUE_LINE_SPACE, int(113)
    STYLE_VALUE_BLEND_MODE, int(114)
    STYLE_VALUE_OFS_X, int(115)
    STYLE_VALUE_OFS_Y, int(116)
    STYLE_VALUE_ALIGN, int(117)
    STYLE_VALUE_COLOR, int(121)
    STYLE_VALUE_OPA, int(124)
    STYLE_VALUE_FONT, int(126)
    STYLE_VALUE_STR, int(127)
    STYLE_TEXT_LETTER_SPACE, int(32896)
    STYLE_TEXT_LINE_SPACE, int(32897)
    STYLE_TEXT_DECOR, int(32898)
    STYLE_TEXT_BLEND_MODE, int(32899)
    STYLE_TEXT_COLOR, int(32905)
    STYLE_TEXT_SEL_COLOR, int(32906)
    STYLE_TEXT_SEL_BG_COLOR, int(32907)
    STYLE_TEXT_OPA, int(32908)
    STYLE_TEXT_FONT, int(32910)
    STYLE_LINE_WIDTH, int(144)
    STYLE_LINE_BLEND_MODE, int(145)
    STYLE_LINE_DASH_WIDTH, int(146)
    STYLE_LINE_DASH_GAP, int(147)
    STYLE_LINE_ROUNDED, int(148)
    STYLE_LINE_COLOR, int(153)
    STYLE_LINE_OPA, int(156)
    STYLE_IMAGE_BLEND_MODE, int(32928)
    STYLE_IMAGE_RECOLOR, int(32937)
    STYLE_IMAGE_OPA, int(32940)
    STYLE_IMAGE_RECOLOR_OPA, int(32941)
    STYLE_TRANSITION_TIME, int(176)
    STYLE_TRANSITION_DELAY, int(177)
    STYLE_TRANSITION_PROP_1, int(178)
    STYLE_TRANSITION_PROP_2, int(179)
    STYLE_TRANSITION_PROP_3, int(180)
    STYLE_TRANSITION_PROP_4, int(181)
    STYLE_TRANSITION_PROP_5, int(182)
    STYLE_TRANSITION_PROP_6, int(183)
    STYLE_TRANSITION_PATH, int(190)
    STYLE_SCALE_WIDTH, int(192)
    STYLE_SCALE_BORDER_WIDTH, int(193)
    STYLE_SCALE_END_BORDER_WIDTH, int(194)
    STYLE_SCALE_END_LINE_WIDTH, int(195)
    STYLE_SCALE_GRAD_COLOR, int(201)
    STYLE_SCALE_END_COLOR, int(202)
    TXT_FLAG_NONE, int(0)
    TXT_FLAG_RECOLOR, int(1)
    TXT_FLAG_EXPAND, int(2)
    TXT_FLAG_CENTER, int(4)
    TXT_FLAG_RIGHT, int(8)
    TXT_FLAG_FIT, int(16)
    TXT_CMD_STATE_WAIT, int(0)
    TXT_CMD_STATE_PAR, int(1)
    TXT_CMD_STATE_IN, int(2)
    FS_RES_OK, int(0)
    FS_RES_HW_ERR, int(1)
    FS_RES_FS_ERR, int(2)
    FS_RES_NOT_EX, int(3)
    FS_RES_FULL, int(4)
    FS_RES_LOCKED, int(5)
    FS_RES_DENIED, int(6)
    FS_RES_BUSY, int(7)
    FS_RES_TOUT, int(8)
    FS_RES_NOT_IMP, int(9)
    FS_RES_OUT_OF_MEM, int(10)
    FS_RES_INV_PARAM, int(11)
    FS_RES_UNKNOWN, int(12)
    FS_MODE_WR, int(1)
    FS_MODE_RD, int(2)
    EVENT_PRESSED, int(0)
    EVENT_PRESSING, int(1)
    EVENT_PRESS_LOST, int(2)
    EVENT_SHORT_CLICKED, int(3)
    EVENT_LONG_PRESSED, int(4)
    EVENT_LONG_PRESSED_REPEAT, int(5)
    EVENT_CLICKED, int(6)
    EVENT_RELEASED, int(7)
    EVENT_DRAG_BEGIN, int(8)
    EVENT_DRAG_END, int(9)
    EVENT_DRAG_THROW_BEGIN, int(10)
    EVENT_GESTURE, int(11)
    EVENT_KEY, int(12)
    EVENT_FOCUSED, int(13)
    EVENT_DEFOCUSED, int(14)
    EVENT_LEAVE, int(15)
    EVENT_VALUE_CHANGED, int(16)
    EVENT_INSERT, int(17)
    EVENT_REFRESH, int(18)
    EVENT_APPLY, int(19)
    EVENT_CANCEL, int(20)
    EVENT_DELETE, int(21)
    PROTECT_NONE, int(0)
    PROTECT_CHILD_CHG, int(1)
    PROTECT_PARENT, int(2)
    PROTECT_POS, int(4)
    PROTECT_FOLLOW, int(8)
    PROTECT_PRESS_LOST, int(16)
    PROTECT_CLICK_FOCUS, int(32)
    PROTECT_EVENT_TO_DISABLED, int(64)
    ARC_TYPE_NORMAL, int(0)
    ARC_TYPE_SYMMETRIC, int(1)
    ARC_TYPE_REVERSE, int(2)
    ARC_PART_BG, int(0)
    ARC_PART_INDIC, int(1)
    ARC_PART_KNOB, int(2)
    BAR_TYPE_NORMAL, int(0)
    BAR_TYPE_SYMMETRICAL, int(1)
    BAR_TYPE_CUSTOM, int(2)
    BTN_STATE_RELEASED, int(0)
    BTN_STATE_PRESSED, int(1)
    BTN_STATE_DISABLED, int(2)
    BTN_STATE_CHECKED_RELEASED, int(3)
    BTN_STATE_CHECKED_PRESSED, int(4)
    BTN_STATE_CHECKED_DISABLED, int(5)
    BTNMATRIX_CTRL_HIDDEN, int(8)
    BTNMATRIX_CTRL_NO_REPEAT, int(16)
    BTNMATRIX_CTRL_DISABLED, int(32)
    BTNMATRIX_CTRL_CHECKABLE, int(64)
    BTNMATRIX_CTRL_CHECK_STATE, int(128)
    BTNMATRIX_CTRL_CLICK_TRIG, int(256)
    CALENDAR_PART_BG, int(0)
    CALENDAR_PART_HEADER, int(1)
    CALENDAR_PART_DAY_NAMES, int(2)
    CALENDAR_PART_DATE, int(3)
    CHART_TYPE_NONE, int(0)
    CHART_TYPE_LINE, int(1)
    CHART_TYPE_COLUMN, int(2)
    CHART_UPDATE_MODE_SHIFT, int(0)
    CHART_UPDATE_MODE_CIRCULAR, int(1)
    CHART_AXIS_PRIMARY_Y, int(0)
    CHART_AXIS_SECONDARY_Y, int(1)
    CHART_CURSOR_NONE, int(0)
    CHART_CURSOR_RIGHT, int(1)
    CHART_CURSOR_UP, int(2)
    CHART_CURSOR_LEFT, int(4)
    CHART_CURSOR_DOWN, int(8)
    CHART_AXIS_SKIP_LAST_TICK, int(0)
    CHART_AXIS_DRAW_LAST_TICK, int(1)
    CHART_AXIS_INVERSE_LABELS_ORDER, int(2)
    CHART_PART_BG, int(0)
    CHART_PART_SERIES_BG, int(1)
    CHART_PART_SERIES, int(2)
    CHART_PART_CURSOR, int(3)
    CHECKBOX_PART_BG, int(0)
    CHECKBOX_PART_BULLET, int(64)
    LAYOUT_OFF, int(0)
    LAYOUT_CENTER, int(1)
    LAYOUT_COLUMN_LEFT, int(2)
    LAYOUT_COLUMN_MID, int(3)
    LAYOUT_COLUMN_RIGHT, int(4)
    LAYOUT_ROW_TOP, int(5)
    LAYOUT_ROW_MID, int(6)
    LAYOUT_ROW_BOTTOM, int(7)
    LAYOUT_PRETTY_TOP, int(8)
    LAYOUT_PRETTY_MID, int(9)
    LAYOUT_PRETTY_BOTTOM, int(10)
    LAYOUT_GRID, int(11)
    FIT_NONE, int(0)
    FIT_TIGHT, int(1)
    FIT_PARENT, int(2)
    FIT_MAX, int(3)
    CPICKER_TYPE_RECT, int(0)
    CPICKER_TYPE_DISC, int(1)
    CPICKER_COLOR_MODE_HUE, int(0)
    CPICKER_COLOR_MODE_SATURATION, int(1)
    CPICKER_COLOR_MODE_VALUE, int(2)
    CPICKER_PART_MAIN, int(0)
    CPICKER_PART_KNOB, int(1)
    DROPDOWN_DIR_DOWN, int(0)
    DROPDOWN_DIR_UP, int(1)
    DROPDOWN_DIR_LEFT, int(2)
    DROPDOWN_DIR_RIGHT, int(3)
    DROPDOWN_PART_MAIN, int(0)
    DROPDOWN_PART_LIST, int(64)
    DROPDOWN_PART_SCROLLBAR, int(65)
    DROPDOWN_PART_SELECTED, int(66)
    GAUGE_PART_MAIN, int(0)
    GAUGE_PART_MAJOR, int(1)
    GAUGE_PART_NEEDLE, int(2)
    KEYBOARD_MODE_TEXT_LOWER, int(0)
    KEYBOARD_MODE_TEXT_UPPER, int(1)
    KEYBOARD_MODE_SPECIAL, int(2)
    KEYBOARD_MODE_NUM, int(3)
    KEYBOARD_PART_BG, int(0)
    KEYBOARD_PART_BTN, int(1)
    LABEL_LONG_EXPAND, int(0)
    LABEL_LONG_BREAK, int(1)
    LABEL_LONG_DOT, int(2)
    LABEL_LONG_SROLL, int(3)
    LABEL_LONG_SROLL_CIRC, int(4)
    LABEL_LONG_CROP, int(5)
    LABEL_ALIGN_LEFT, int(0)
    LABEL_ALIGN_CENTER, int(1)
    LABEL_ALIGN_RIGHT, int(2)
    LABEL_ALIGN_AUTO, int(3)
    LED_PART_MAIN, int(0)
    LINEMETER_PART_MAIN, int(0)
    LIST_PART_BG, int(0)
    LIST_PART_SCROLLBAR, int(1)
    LIST_PART_EDGE_FLASH, int(2)
    OBJMASK_PART_MAIN, int(0)
    TEMPL_STYLE_X, int(0)
    TEMPL_STYLE_Y, int(1)
    SCROLLBAR_MODE_OFF, int(0)
    SCROLLBAR_MODE_ON, int(1)
    SCROLLBAR_MODE_DRAG, int(2)
    SCROLLBAR_MODE_AUTO, int(3)
    SCROLLBAR_MODE_HIDE, int(4)
    SCROLLBAR_MODE_UNHIDE, int(8)
    PAGE_EDGE_LEFT, int(1)
    PAGE_EDGE_TOP, int(2)
    PAGE_EDGE_RIGHT, int(4)
    PAGE_EDGE_BOTTOM, int(8)
    ROLLER_MODE_NORMAL, int(0)
    ROLLER_MODE_INFINITE, int(1)
    SLIDER_TYPE_NORMAL, int(0)
    SLIDER_TYPE_SYMMETRICAL, int(1)
    SLIDER_TYPE_RANGE, int(2)
    SPINNER_TYPE_SPINNING_ARC, int(0)
    SPINNER_TYPE_FILLSPIN_ARC, int(1)
    SPINNER_TYPE_CONSTANT_ARC, int(2)
    SPINNER_DIR_FORWARD, int(0)
    SPINNER_DIR_BACKWARD, int(1)
    TABVIEW_TAB_POS_NONE, int(0)
    TABVIEW_TAB_POS_TOP, int(1)
    TABVIEW_TAB_POS_BOTTOM, int(2)
    TABVIEW_TAB_POS_LEFT, int(3)
    TABVIEW_TAB_POS_RIGHT, int(4)
    TEXTAREA_CURSOR_LAST, int(32767)

    start, func(lv0_start)
    montserrat_font, func(lv0_load_montserrat_font)

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
