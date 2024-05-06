import re
import sys
import glob

# https://stackoverflow.com/a/241506
def comment_remover(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)

# compute a sorted list of files from a prefix and a list of glob patterns
def list_files(prefix, glob_list):
  r = []
  for g in glob_list:
    r += glob.glob(prefix + g, recursive=True)
  return sorted(r)

def clean_source(raw):
  raw = comment_remover(raw)    # remove comments
  # convert cr/lf or cr to lf
  raw = re.sub(r'\r\n ', '\n', raw)
  raw = re.sub(r'\r', '\n', raw)
  # group multilines into a single line, i.e. if line ends with '\', put in a single line
  raw = re.sub(r'\\\n', ' ', raw)
  # remove preprocessor directives
  raw = re.sub(r'\n[ \t]*#[^\n]*(?=\n)', '', raw)
  raw = re.sub(r'^[ \t]*#[^\n]*\n', '', raw)
  raw = re.sub(r'\n[ \t]*#[^\n]*$', '', raw)

  # remove extern "C" {}
  raw = re.sub(r'extern\s+"C"\s+{(.*)}', '\\1', raw, flags=re.DOTALL)

  # remove empty lines
  raw = re.sub(r'\n[ \t]*(?=\n)', '', raw)
  raw = re.sub(r'^[ \t]*\n', '', raw)  # remove first empty line
  raw = re.sub(r'\n[ \t]*$', '', raw)  # remove last empty line
  return raw

# ################################################################################
# Parse function signatures
# ################################################################################

lv_src_prefix = "../../lvgl/src/"
lv_fun_globs = [ 
                  "lv_api*.h",
                  "widgets/*/*.h",   # all widgets
                  "libs/qrcode/lv_qrcode.h",
                  "core/*.h",
                  "indev/lv_indev.h",
                  "layouts/*/*.h",
                  # "draw/*.h",
                  "themes/lv_theme.h",
                  "draw/lv_draw_arc.h",
                  "draw/lv_draw_label.h",
                  "draw/lv_draw_line.h",
                  "draw/lv_draw_mask.h",
                  "draw/lv_draw_rect.h",
                  "draw/lv_draw_triangle.h",
                  # "draw/lv_draw_vector.h",
                  "draw/lv_draw.h",
                  "display/*.h",
                  "misc/lv_anim.h",
                  "misc/lv_area.h",
                  "misc/lv_color.h",
                  "misc/lv_color_op.h",
                  "misc/lv_palette.h",
                  "misc/lv_event.h",
                  "misc/lv_style_gen.h",
                  "misc/lv_style.h",
                  "misc/lv_timer.h",
                  "misc/lv_text.h",
                  "font/lv_font.h",
                  # add version information
                  "../lvgl.h",
              ]
headers_names = list_files(lv_src_prefix, lv_fun_globs)
headers_names += list_files("../../LVGL_assets/src/", ["lv_theme_haspmota.h"])
headers_names += list_files("../src/", ["lv_berry.h"])

output_filename = "../mapping/lv_funcs.h"
sys.stdout = open(output_filename, 'w', encoding='utf-8')

print("""
// Automatically generated from LVGL source with `python3 preprocessor.py`
// Extract function signatures from LVGL APIs in headers

// Custom Tasmota functions
lv_ts_calibration_t * lv_get_ts_calibration(void);

// ======================================================================
// LV top level functions
// ======================================================================

// resolution
lv_coord_t lv_get_hor_res(void);
lv_coord_t lv_get_ver_res(void);

// ======================================================================
// Generated from headers
// ======================================================================

""")

for header_name in headers_names:
  with open(header_name, encoding='utf-8') as f:
    print("// " + header_name)
    raw = clean_source(f.read())

    # remove anything in '{' '}'
    while True:
      (raw, repl) = re.subn(r'\{[^{]*?\}', ';', raw, flags=re.DOTALL)  # replace with ';' to make pattern matching still work
      if (repl == 0): break  # no more replace, stop

    raw_f = re.findall(r'(^|;|})\s*([^;{}]+\(.*?\))\s*(?=(;|{))', raw, flags=re.DOTALL)
    fun_defs = [ x[1] for x in raw_f]
    # remove any CRLF or multi-space
    fun_defs = [ re.sub(r'[ \t\r\n]+', ' ', x) for x in fun_defs]

    # parse individual
    for fun in fun_defs:
      # remove LV_ATTRIBUTE_FAST_MEM 
      fun = re.sub(r'LV_ATTRIBUTE_FAST_MEM ', '', fun)
      # remove LV_ATTRIBUTE_TIMER_HANDLER 
      fun = re.sub(r'LV_ATTRIBUTE_TIMER_HANDLER ', '', fun)
      # remove extern 
      fun = re.sub(r'extern ', '', fun)
      exclude = False
      for exclude_prefix in ["typedef", "_LV_", "LV_"]:
        if fun.startswith(exclude_prefix): exclude = True
      if exclude: continue

      # extrac the function name
      fun_name = re.search(r'\s(\w+)\([^\(]*$', fun)
      if fun_name != None:
        fun_name = fun_name.group(1)    # we now have the function name
        
        # exclude specific names
        for exclude_pattern in [
              "^_",    # skip if function name starts with '_'
              "^lv_debug",    # all debug functions
              "^lv_init", "^lv_deinit",
              "^lv_templ_",
              "^lv_imagebutton_get_src_",    # LV_IMGBTN_TILED == 0
              "^lv_imagebitton_set_src_tiled",# !LV_IMGBTN_TILED
              #"^lv_disp_",
              "^lv_refr_get_fps_",      # no LV_USE_PERF_MONITOR
              "^lv_image_cache_",
              "^lv_image_decoder_",
              "^lv_image_cf_",
              "^lv_image_buf_",
              "^lv_indev_scroll_",
              "^lv_pow",
              "^lv_keyboard_def_event_cb",  # need to fix conditional include
              # "^lv_event_get_",            # event_getters not needed
              "^lv_refr_reset_fps_counter",
              "^lv_refr_get_fps_avg",
              "^lv_anim_path_",             # callbacks for animation are moved to constants
              # LV_USE_OBJ_PROPERTY 0
              "^lv_obj_set_property",
              "^lv_obj_set_properties",
              "^lv_obj_get_property",
              "^lv_win_",
            ]:
          if re.search(exclude_pattern, fun_name): exclude = True
        if exclude: continue
      
      print(fun)
  print()

sys.stdout.close()

# ################################################################################
# Parse 'enum'
# ################################################################################

lv_src_prefix = "../../lvgl/src/"
lv_fun_globs = [ 
                  "core/*.h",
                  "draw/*.h",
                  "hal/*.h",
                  "misc/*.h",
                  "widgets/*/*.h",
                  "display/lv_display.h",
                  "layouts/**/*.h",
              ]
headers_names = list_files(lv_src_prefix, lv_fun_globs)

output_filename = "../mapping/lv_enum.h"
sys.stdout = open(output_filename, 'w', encoding='utf-8')
print("""// ======================================================================
// Functions
// ======================================================================

load_font=@lv0_load_font

// lv_anim_path_functions
anim_path_bounce=&lv_anim_path_bounce
anim_path_ease_in=&lv_anim_path_ease_in
anim_path_ease_in_out=&lv_anim_path_ease_in_out
anim_path_ease_out=&lv_anim_path_ease_out
anim_path_linear=&lv_anim_path_linear
anim_path_overshoot=&lv_anim_path_overshoot
anim_path_step=&lv_anim_path_step
LV_LAYOUT_GRID=>be_LV_LAYOUT_GRID
LV_LAYOUT_FLEX=>be_LV_LAYOUT_FLEX

// ======================================================================
// Colors
// ======================================================================
// LV Colors - we store in 24 bits format and will convert at runtime
// This is specific treatment because we keep colors in 24 bits format
COLOR_WHITE=0xFFFFFF
COLOR_SILVER=0xC0C0C0
COLOR_GRAY=0x808080
COLOR_BLACK=0x000000
COLOR_RED=0xFF0000
COLOR_MAROON=0x800000
COLOR_YELLOW=0xFFFF00
COLOR_OLIVE=0x808000
COLOR_LIME=0x00FF00
COLOR_GREEN=0x008000
COLOR_CYAN=0x00FFFF
COLOR_AQUA=0x00FFFF
COLOR_TEAL=0x008080
COLOR_BLUE=0x0000FF
COLOR_NAVY=0x000080
COLOR_MAGENTA=0xFF00FF
COLOR_ORANGE=0xFF7F00
COLOR_PURPLE=0x800080

// Freetype
FT_FONT_STYLE_NORMAL=FT_FONT_STYLE_NORMAL
FT_FONT_STYLE_ITALIC=FT_FONT_STYLE_ITALIC
FT_FONT_STYLE_BOLD=FT_FONT_STYLE_BOLD

// following are #define, not enum
LV_GRID_FR=LV_GRID_FR(0)

// ======================================================================
// Symbols
// ======================================================================

SYMBOL_AUDIO="\\xef\\x80\\x81"
SYMBOL_VIDEO="\\xef\\x80\\x88"
SYMBOL_LIST="\\xef\\x80\\x8b"
SYMBOL_OK="\\xef\\x80\\x8c"
SYMBOL_CLOSE="\\xef\\x80\\x8d"
SYMBOL_POWER="\\xef\\x80\\x91"
SYMBOL_SETTINGS="\\xef\\x80\\x93"
SYMBOL_HOME="\\xef\\x80\\x95"
SYMBOL_DOWNLOAD="\\xef\\x80\\x99"
SYMBOL_DRIVE="\\xef\\x80\\x9c"
SYMBOL_REFRESH="\\xef\\x80\\xa1"
SYMBOL_MUTE="\\xef\\x80\\xa6"
SYMBOL_VOLUME_MID="\\xef\\x80\\xa7"
SYMBOL_VOLUME_MAX="\\xef\\x80\\xa8"
SYMBOL_IMAGE="\\xef\\x80\\xbe"
SYMBOL_EDIT="\\xef\\x8C\\x84"
SYMBOL_PREV="\\xef\\x81\\x88"
SYMBOL_PLAY="\\xef\\x81\\x8b"
SYMBOL_PAUSE="\\xef\\x81\\x8c"
SYMBOL_STOP="\\xef\\x81\\x8d"
SYMBOL_NEXT="\\xef\\x81\\x91"
SYMBOL_EJECT="\\xef\\x81\\x92"
SYMBOL_LEFT="\\xef\\x81\\x93"
SYMBOL_RIGHT="\\xef\\x81\\x94"
SYMBOL_PLUS="\\xef\\x81\\xa7"
SYMBOL_MINUS="\\xef\\x81\\xa8"
SYMBOL_EYE_OPEN="\\xef\\x81\\xae"
SYMBOL_EYE_CLOSE="\\xef\\x81\\xb0"
SYMBOL_WARNING="\\xef\\x81\\xb1"
SYMBOL_SHUFFLE="\\xef\\x81\\xb4"
SYMBOL_UP="\\xef\\x81\\xb7"
SYMBOL_DOWN="\\xef\\x81\\xb8"
SYMBOL_LOOP="\\xef\\x81\\xb9"
SYMBOL_DIRECTORY="\\xef\\x81\\xbb"
SYMBOL_UPLOAD="\\xef\\x82\\x93"
SYMBOL_CALL="\\xef\\x82\\x95"
SYMBOL_CUT="\\xef\\x83\\x84"
SYMBOL_COPY="\\xef\\x83\\x85"
SYMBOL_SAVE="\\xef\\x83\\x87"
SYMBOL_CHARGE="\\xef\\x83\\xa7"
SYMBOL_PASTE="\\xef\\x83\\xAA"
SYMBOL_BELL="\\xef\\x83\\xb3"
SYMBOL_KEYBOARD="\\xef\\x84\\x9c"
SYMBOL_GPS="\\xef\\x84\\xa4"
SYMBOL_FILE="\\xef\\x85\\x9b"
SYMBOL_WIFI="\\xef\\x87\\xab"
SYMBOL_BATTERY_FULL="\\xef\\x89\\x80"
SYMBOL_BATTERY_3="\\xef\\x89\\x81"
SYMBOL_BATTERY_2="\\xef\\x89\\x82"
SYMBOL_BATTERY_1="\\xef\\x89\\x83"
SYMBOL_BATTERY_EMPTY="\\xef\\x89\\x84"
SYMBOL_USB="\\xef\\x8a\\x87"
SYMBOL_BLUETOOTH="\\xef\\x8a\\x93"
SYMBOL_TRASH="\\xef\\x8B\\xAD"
SYMBOL_BACKSPACE="\\xef\\x95\\x9A"
SYMBOL_SD_CARD="\\xef\\x9F\\x82"
SYMBOL_NEW_LINE="\\xef\\xA2\\xA2"

SYMBOL_DUMMY="\\xEF\\xA3\\xBF"

SYMBOL_BULLET="\\xE2\\x80\\xA2"
      
// LVGL 8 to 9 compatibility
      
LV_DISP_ROTATION_0=LV_DISPLAY_ROTATION_0
LV_DISP_ROTATION_90=LV_DISPLAY_ROTATION_90
LV_DISP_ROTATION_180=LV_DISPLAY_ROTATION_180
LV_DISP_ROTATION_270=LV_DISPLAY_ROTATION_270

LV_DISP_RENDER_MODE_PARTIAL=LV_DISPLAY_RENDER_MODE_PARTIAL
LV_DISP_RENDER_MODE_DIRECT=LV_DISPLAY_RENDER_MODE_DIRECT
LV_DISP_RENDER_MODE_FULL=LV_DISPLAY_RENDER_MODE_FULL

LV_BTNMATRIX_BTN_NONE=LV_BUTTONMATRIX_BUTTON_NONE

LV_BTNMATRIX_CTRL_HIDDEN=LV_BUTTONMATRIX_CTRL_HIDDEN
LV_BTNMATRIX_CTRL_NO_REPEAT=LV_BUTTONMATRIX_CTRL_NO_REPEAT
LV_BTNMATRIX_CTRL_DISABLED=LV_BUTTONMATRIX_CTRL_DISABLED
LV_BTNMATRIX_CTRL_CHECKABLE=LV_BUTTONMATRIX_CTRL_CHECKABLE
LV_BTNMATRIX_CTRL_CHECKED=LV_BUTTONMATRIX_CTRL_CHECKED
LV_BTNMATRIX_CTRL_CLICK_TRIG=LV_BUTTONMATRIX_CTRL_CLICK_TRIG
LV_BTNMATRIX_CTRL_POPOVER=LV_BUTTONMATRIX_CTRL_POPOVER
LV_BTNMATRIX_CTRL_CUSTOM_1=LV_BUTTONMATRIX_CTRL_CUSTOM_1
LV_BTNMATRIX_CTRL_CUSTOM_2=LV_BUTTONMATRIX_CTRL_CUSTOM_2

LV_RES_OK=LV_RESULT_OK
LV_RES_INV=LV_RESULT_INVALID

LV_INDEV_STATE_PR=LV_INDEV_STATE_PRESSED
LV_INDEV_STATE_REL=LV_INDEV_STATE_RELEASED

LV_STYLE_ANIM_TIME=LV_STYLE_ANIM_DURATION
LV_STYLE_IMG_OPA=LV_STYLE_IMAGE_OPA
LV_STYLE_IMG_RECOLOR=LV_STYLE_IMAGE_RECOLOR
LV_STYLE_IMG_RECOLOR_OPA=LV_STYLE_IMAGE_RECOLOR_OPA
LV_STYLE_SHADOW_OFS_X=LV_STYLE_SHADOW_OFFSET_X
LV_STYLE_SHADOW_OFS_Y=LV_STYLE_SHADOW_OFFSET_Y
LV_STYLE_TRANSFORM_ANGLE=LV_STYLE_TRANSFORM_ROTATION

LV_ZOOM_NONE=LV_SCALE_NONE

// ======================================================================
// Generated from headers
// ======================================================================
""")

for header_name in headers_names:
  with open(header_name) as f:
    raw = clean_source(f.read())

    print(f"// File: {header_name}")
    # extract enums
    enums = re.findall(r'enum\s+\w*\s*{(.*?)}', raw, flags=re.DOTALL)
    for enum in enums:  # iterate on all matches
      # exclude LV_PROPERTY_ID
      # we compile with `#define LV_USE_OBJ_PROPERTY 0`
      # and remove all instances of `LV_PROPERTY_ID(OBJ, FLAG_START,                 LV_PROPERTY_TYPE_INT,       0),`
      if re.search('LV_PROPERTY_ID', enum): continue
      # remove enums defined via a macro
      enum = re.sub(r'\S+\((.*?),.*?\),', '\\1,', enum)  # turn 'LV_STYLE_PROP_INIT(LV_STYLE_SIZE, 0x0, LV_STYLE_ID_VALUE + 3, LV_STYLE_ATTR_NONE),' into 'LV_STYLE_SIZE'
      #
      enum_elt = enum.split(",")
      for enum_item in enum_elt:
        # remove any space
        enum_item = re.sub(r'[ \t\n]', '', enum_item)
        # remove anything after '='
        enum_item = re.sub(r'=.*$', '', enum_item)

        # item is ready
        exclude = False
        for exclude_prefix in ["_", "LV_BIDI_DIR_", "LV_FONT_",
                               "LV_SIGNAL_", "LV_TEMPL_", "LV_TASK_PRIO_", "LV_THEME_",
                               "LV_LRU_",
                               "LV_VECTOR_",
                               "LV_KEYBOARD_MODE_TEXT_ARABIC"]:
          if enum_item.startswith(exclude_prefix): exclude = True
        if exclude: continue

        print(enum_item)

    # extract `LV_EXPORT_CONST_INT()` int constants
    constints = re.findall(r'LV_EXPORT_CONST_INT\((\w+)\)', raw, flags=re.DOTALL)
    for constint in constints:
      print(constint)
sys.stdout.close()
