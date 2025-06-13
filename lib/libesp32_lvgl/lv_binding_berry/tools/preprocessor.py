#!/usr/bin/env python3
"""
LVGL Header Preprocessor

Extracts function signatures and enums from LVGL header files.
Generates mapping files for Berry scripting integration.
"""

import re
import sys
import glob
import argparse
from pathlib import Path
from typing import List, Set, Tuple, Optional
import logging

# Configure logging
logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')
logger = logging.getLogger(__name__)


class LVGLPreprocessor:
    """Main preprocessor class for LVGL headers."""
    
    def __init__(self, lv_src_prefix: str = "../../lvgl/src/"):
        self.lv_src_prefix = Path(lv_src_prefix)
        self.headers_exclude_suffix = {
            "_private.h",
            "lv_lottie.h", 
            "lv_obj_property.h",
            "lv_obj_property_names.h",
            "lv_style_properties.h",
            "lv_3dtexture.h",
        }
        
        # Function exclusion patterns
        self.function_exclude_patterns = [
            r"^_",                        # skip if function name starts with '_'
            r"^lv_debug",                 # all debug functions
            r"^lv_init", r"^lv_deinit",
            r"^lv_templ_",
            r"^lv_imagebutton_get_src_",  # LV_IMGBTN_TILED == 0
            r"^lv_imagebitton_set_src_tiled",  # !LV_IMGBTN_TILED
            r"^lv_refr_get_fps_",         # no LV_USE_PERF_MONITOR
            r"^lv_image_cache_",
            r"^lv_image_decoder_",
            r"^lv_image_cf_",
            r"^lv_image_buf_",
            r"^lv_indev_scroll_",
            r"^lv_pow",
            r"^lv_keyboard_def_event_cb", # need to fix conditional include
            r"^lv_refr_reset_fps_counter",
            r"^lv_refr_get_fps_avg",
            r"^lv_anim_path_",            # callbacks for animation are moved to constants
            r"^lv_obj_set_property",      # LV_USE_OBJ_PROPERTY 0
            r"^lv_obj_set_properties",
            r"^lv_obj_get_property",
            r"^lv_win_",
            r"^lv_obj.*name",             # we don't enable #if LV_USE_OBJ_NAME
        ]
        
        # Enum exclusion patterns
        self.enum_exclude_prefixes = {
            "_", "LV_BIDI_DIR_", "LV_FONT_", "LV_SIGNAL_", "LV_TEMPL_", 
            "LV_TASK_PRIO_", "LV_THEME_", "LV_LRU_", "LV_VECTOR_",
            "LV_KEYBOARD_MODE_TEXT_ARABIC", "LV_DRAW_TASK_TYPE_3D", 
            "LV_DRAW_TASK_TYPE_VECTOR",
        }

    def comment_remover(self, text: str) -> str:
        """Remove C/C++ style comments from source code."""
        def replacer(match):
            s = match.group(0)
            return " " if s.startswith('/') else s
        
        pattern = re.compile(
            r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
            re.DOTALL | re.MULTILINE
        )
        return re.sub(pattern, replacer, text)

    def list_files(self, prefix: Path, glob_patterns: List[str]) -> List[Path]:
        """Compute a sorted list of files from a prefix and glob patterns."""
        files = []
        for pattern in glob_patterns:
            files.extend(Path(prefix).glob(pattern))
        return sorted(files)

    def clean_source(self, raw: str) -> str:
        """Clean source code by removing comments, preprocessor directives, etc."""
        raw = self.comment_remover(raw)
        
        # Normalize line endings
        raw = re.sub(r'\r\n', '\n', raw)
        raw = re.sub(r'\r', '\n', raw)
        
        # Handle line continuations
        raw = re.sub(r'\\\n', ' ', raw)
        
        # Remove preprocessor directives
        raw = re.sub(r'\n[ \t]*#[^\n]*(?=\n)', '', raw)
        raw = re.sub(r'^[ \t]*#[^\n]*\n', '', raw)
        raw = re.sub(r'\n[ \t]*#[^\n]*$', '', raw)

        # Remove extern "C" blocks
        raw = re.sub(r'extern\s+"C"\s+{(.*)}', r'\1', raw, flags=re.DOTALL)

        # Remove empty lines
        raw = re.sub(r'\n[ \t]*(?=\n)', '', raw)
        raw = re.sub(r'^[ \t]*\n', '', raw)
        raw = re.sub(r'\n[ \t]*$', '', raw)
        
        return raw

    def extract_functions(self, source: str) -> List[str]:
        """Extract function signatures from cleaned source code."""
        # Remove content within braces
        while True:
            source, repl_count = re.subn(r'\{[^{]*?\}', ';', source, flags=re.DOTALL)
            if repl_count == 0:
                break

        # Find function signatures
        pattern = r'(^|;|})\s*([^;{}]+\(.*?\))\s*(?=(;|{))'
        matches = re.findall(pattern, source, flags=re.DOTALL)
        
        functions = []
        for match in matches:
            func_def = match[1]
            # Clean up whitespace
            func_def = re.sub(r'[ \t\r\n]+', ' ', func_def)
            
            # Remove LVGL-specific attributes
            func_def = re.sub(r'LV_ATTRIBUTE_FAST_MEM ', '', func_def)
            func_def = re.sub(r'LV_ATTRIBUTE_TIMER_HANDLER ', '', func_def)
            func_def = re.sub(r'extern ', '', func_def)
            
            # Skip excluded function types
            if any(func_def.startswith(prefix) for prefix in ["typedef", "_LV_", "LV_"]):
                continue
                
            # Extract function name
            name_match = re.search(r'\s(\w+)\([^\(]*$', func_def)
            if not name_match:
                continue
                
            func_name = name_match.group(1)
            
            # Check exclusion patterns
            if any(re.search(pattern, func_name) for pattern in self.function_exclude_patterns):
                continue
                
            functions.append(func_def)
            
        return functions

    def extract_enums(self, source: str) -> Set[str]:
        """Extract enum values from cleaned source code."""
        enum_values = set()
        
        # Find enum definitions
        enum_matches = re.findall(r'enum\s+\w*\s*{(.*?)}', source, flags=re.DOTALL)
        
        for enum_content in enum_matches:
            # Skip LV_PROPERTY_ID enums (disabled feature)
            if 'LV_PROPERTY_ID' in enum_content:
                continue
                
            # Remove macro-defined enums
            enum_content = re.sub(r'\S+\((.*?),.*?\),', r'\1,', enum_content)
            
            # Split by commas and clean up
            for item in enum_content.split(','):
                item = re.sub(r'[ \t\n]', '', item)  # Remove whitespace
                item = re.sub(r'=.*$', '', item)     # Remove assignment
                
                if not item:  # Skip empty items
                    continue
                    
                # Check exclusion patterns
                if any(item.startswith(prefix) for prefix in self.enum_exclude_prefixes):
                    continue
                    
                enum_values.add(item)
        
        # Extract LV_EXPORT_CONST_INT constants
        const_ints = re.findall(r'LV_EXPORT_CONST_INT\((\w+)\)', source, flags=re.DOTALL)
        enum_values.update(const_ints)
        
        return enum_values

    def get_function_headers(self) -> List[Path]:
        """Get list of header files for function extraction."""
        patterns = [
            "lv_api*.h",
            "widgets/*/*.h",
            "libs/qrcode/lv_qrcode.h",
            "core/*.h",
            "indev/lv_indev.h",
            "layouts/*/*.h",
            "themes/lv_theme.h",
            "draw/lv_draw_arc.h",
            "draw/lv_draw_label.h", 
            "draw/lv_draw_line.h",
            "draw/lv_draw_mask.h",
            "draw/lv_draw_rect.h",
            "draw/lv_draw_triangle.h",
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
            "../lvgl.h",
        ]
        
        headers = self.list_files(self.lv_src_prefix, patterns)
        
        # Add additional headers
        additional_paths = [
            Path("../../LVGL_assets/src/lv_theme_haspmota.h"),
            Path("../src/lv_berry.h"),
            Path("../src/lv_colorwheel.h"),
        ]
        
        for path in additional_paths:
            if path.exists():
                headers.append(path)
        
        # Filter out excluded files
        return [h for h in headers if not any(str(h).endswith(suffix) for suffix in self.headers_exclude_suffix)]

    def get_enum_headers(self) -> List[Path]:
        """Get list of header files for enum extraction."""
        patterns = [
            "core/*.h",
            "draw/*.h", 
            "hal/*.h",
            "misc/*.h",
            "widgets/*/*.h",
            "display/lv_display.h",
            "layouts/**/*.h",
        ]
        
        headers = self.list_files(self.lv_src_prefix, patterns)
        return [h for h in headers if not any(str(h).endswith(suffix) for suffix in self.headers_exclude_suffix)]

    def generate_functions_header(self, output_path: Path):
        """Generate the functions header file."""
        logger.info(f"Generating functions header: {output_path}")
        
        headers = self.get_function_headers()
        
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write("""
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
            
            for header_path in headers:
                try:
                    with open(header_path, encoding='utf-8-sig') as header_file:
                        f.write(f"// {header_path}\n")
                        
                        raw_content = self.clean_source(header_file.read())
                        functions = self.extract_functions(raw_content)
                        
                        for func in functions:
                            f.write(f"{func}\n")
                        f.write("\n")
                        
                except Exception as e:
                    logger.error(f"Error processing {header_path}: {e}")

    def generate_enums_header(self, output_path: Path):
        """Generate the enums header file."""
        logger.info(f"Generating enums header: {output_path}")
        
        headers = self.get_enum_headers()
        
        with open(output_path, 'w', encoding='utf-8') as f:
            # Write the static content first
            f.write(self._get_static_enum_content())
            
            # Process headers for dynamic enums
            for header_path in headers:
                try:
                    with open(header_path, encoding='utf-8-sig') as header_file:
                        f.write(f"// File: {header_path}\n")
                        
                        raw_content = self.clean_source(header_file.read())
                        enum_values = self.extract_enums(raw_content)
                        
                        for enum_value in sorted(enum_values):
                            f.write(f"{enum_value}\n")
                        f.write("\n")
                        
                except Exception as e:
                    logger.error(f"Error processing {header_path}: {e}")

    def _get_static_enum_content(self) -> str:
        """Get the static content for enum header."""
        return """// ======================================================================
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
COLOR_GREY=0x808080         // OpenHASP
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
COLOR_FUCHSIA=0xFF00FF      // OpenHASP
COLOR_ORANGE=0xFFA500       // OpenHASP
COLOR_PURPLE=0x800080
// Below are OpenHASP additions
COLOR_PERU=0xCD853F
COLOR_SIENNA=0xA0522D
COLOR_BROWN=0xA52A2A
COLOR_SNOW=0xFFFAFA
COLOR_IVORY=0xFFFFF0
COLOR_LINEN=0xFAF0E6
COLOR_BEIGE=0xF5F5DC
COLOR_AZURE=0xF0FFFF
COLOR_PINK=0xFFC0CB
COLOR_PLUM=0xDDA0DD
COLOR_ORCHID=0xDA70D6
COLOR_VIOLET=0xEE82EE
COLOR_INDIGO=0x4B0082
COLOR_BLUSH=0xB00000
COLOR_TOMATO=0xFF6347
COLOR_SALMON=0xFA8072
COLOR_CORAL=0xFF7F50
COLOR_GOLD=0xFFD700
COLOR_KHAKI=0xF0E68C
COLOR_BISQUE=0xFFE4C4
COLOR_WHEAT=0xF5DEB3
COLOR_TAN=0xD2B48C

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

// LVGL 9.3
LV_LABEL_LONG_WRAP=LV_LABEL_LONG_MODE_WRAP
LV_LABEL_LONG_DOT=LV_LABEL_LONG_MODE_DOTS
LV_LABEL_LONG_SCROLL=LV_LABEL_LONG_MODE_SCROLL
LV_LABEL_LONG_SCROLL_CIRCULAR=LV_LABEL_LONG_MODE_SCROLL_CIRCULAR
LV_LABEL_LONG_CLIP=LV_LABEL_LONG_MODE_CLIP
LV_ANIM_OFF=LV_ANIM_OFF
LV_ANIM_ON=LV_ANIM_ON

// ======================================================================
// Generated from headers
// ======================================================================
"""

    def run(self, functions_output: str = "../mapping/lv_funcs.h", 
            enums_output: str = "../mapping/lv_enum.h"):
        """Run the complete preprocessing pipeline."""
        functions_path = Path(functions_output)
        enums_path = Path(enums_output)
        
        # Create output directories if they don't exist
        functions_path.parent.mkdir(parents=True, exist_ok=True)
        enums_path.parent.mkdir(parents=True, exist_ok=True)
        
        # Generate both files
        self.generate_functions_header(functions_path)
        self.generate_enums_header(enums_path)
        
        logger.info("Preprocessing complete!")


def main():
    """Main entry point with command line argument parsing."""
    parser = argparse.ArgumentParser(description="LVGL Header Preprocessor")
    parser.add_argument("--lv-src", default="../../lvgl/src/", 
                       help="Path to LVGL source directory")
    parser.add_argument("--functions-output", default="../mapping/lv_funcs.h",
                       help="Output path for functions header")
    parser.add_argument("--enums-output", default="../mapping/lv_enum.h", 
                       help="Output path for enums header")
    parser.add_argument("--verbose", "-v", action="store_true",
                       help="Enable verbose logging")
    
    args = parser.parse_args()
    
    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
    
    try:
        preprocessor = LVGLPreprocessor(args.lv_src)
        preprocessor.run(args.functions_output, args.enums_output)
    except Exception as e:
        logger.error(f"Preprocessing failed: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
