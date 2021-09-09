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

lv_src_prefix = "../../lib/libesp32_lvgl/LVGL/src/"
# find all headers
headers_names = glob.glob(lv_src_prefix + "**/*.h", recursive=True)

def clean_source(raw):
  raw = comment_remover(raw)    # remove comments
  # convert cr/lf or cr to lf
  raw = re.sub('\r\n ', '\n', raw)
  raw = re.sub('\r', '\n', raw)
  # group multilines into a single line, i.e. if line ends with '\', put in a single line
  raw = re.sub('\\\\\n', ' ', raw)
  # remove preprocessor directives
  raw = re.sub('\n[ \t]*#[^\n]*(?=\n)', '', raw)
  raw = re.sub('^[ \t]*#[^\n]*\n', '', raw)
  raw = re.sub('\n[ \t]*#[^\n]*$', '', raw)

  # remove extern "C" {}
  raw = re.sub('extern\s+"C"\s+{(.*)}', '\\1', raw, flags=re.DOTALL)

  # remove empty lines
  raw = re.sub('\n[ \t]*(?=\n)', '', raw)
  raw = re.sub('^[ \t]*\n', '', raw)  # remove first empty line
  raw = re.sub('\n[ \t]*$', '', raw)  # remove last empty line
  return raw

# ################################################################################
# Parse 'enum'
# ################################################################################
output_filename = "lv_enum.h"
sys.stdout = open(output_filename, 'w')
print("""
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
COLOR_PURPLE=0x800080

// following are #define, not enum
LV_RADIUS_CIRCLE
LV_TEXTAREA_CURSOR_LAST
LV_STYLE_PROP_ALL
""")



# unit test
#headers_names = [ '../../lib/libesp32_lvgl/LVGL/src/lv_widgets/lv_btn.h' ]
#headers_names = [ '../../lib/libesp32_lvgl/LVGL/src/lv_core/lv_style.h' ]
#
for header_name in headers_names:
  with open(header_name) as f:
    raw = clean_source(f.read())

    # extract enums
    enums = re.findall('enum\s+{(.*?)}', raw, flags=re.DOTALL)
    for enum in enums:  # iterate on all matches
      # remove enums defined via a macro
      enum = re.sub('\S+\((.*?),.*?\),', '\\1,', enum)  # turn 'LV_STYLE_PROP_INIT(LV_STYLE_SIZE, 0x0, LV_STYLE_ID_VALUE + 3, LV_STYLE_ATTR_NONE),' into 'LV_STYLE_SIZE'
      #
      enum_elt = enum.split(",")
      for enum_item in enum_elt:
        # remove any space
        enum_item = re.sub('[ \t\n]', '', enum_item)
        # remove anything after '='
        enum_item = re.sub('=.*$', '', enum_item)

        # item is ready
        exclude = False
        for exclude_prefix in ["_", "LV_BIDI_DIR_", "LV_FONT_", "LV_IMG_CF_RESERVED_", "LV_IMG_CF_USER_",
                               "LV_SIGNAL_", "LV_TEMPL_", "LV_TASK_PRIO_", "LV_THEME_", "LV_KEYBOARD_MODE_TEXT_ARABIC"]:
          if enum_item.startswith(exclude_prefix): exclude = True
        if exclude: continue

        print(enum_item)
sys.stdout.close()
