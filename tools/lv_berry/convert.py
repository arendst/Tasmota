import re
import sys

lv_widgets_file = "lv_widgets.h"
lv_module_file = "lv_module.h"

out_prefix = "../../tasmota/lvgl_berry/"
lvgl_prefix = "../../lib/libesp32/Berry/default/"
lvgl_prefix = "../../lib/libesp32/Berry/default/"

be_lv_defines = "be_lv_defines.h"
be_lv_c_mapping = "be_lv_c_mapping.h"
be_lv_widgets_libs = "be_lvgl_widgets_lib.c"
be_lv_lvgl_module = "be_lv_lvgl_module.c"

# detect a function definition all
# Ex: 'void lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent);'
# Group 1: 'void'
# Group 2: 'lv_obj_set_parent'
# Group 3: 'lv_obj_t * obj, lv_obj_t * parent'
parse_func_def = re.compile("(.*?)\s(\w+)\((.*?)\)")

# parse call argument type
# Ex: 'const lv_obj_t * parent' -> 'const ', 'lv_obj_t', ' * ', 'parent'
# Ex: 'bool auto_fit' -> '', 'bool', ' ', 'auto_fit'
parse_arg = re.compile("(\w+\s+)?(\w+)([\*\s]+)(\w+)")

return_types = {
  "void": "",
  "bool": "b",
  "uint8_t": "i",
  "uint16_t": "i",
  "int16_t": "i",
  "uint32_t": "i",
  "int32_t": "i",
  "void *": ".",
  "const void *": ".",
  "char *": "s",
  "const char *": "s",
  "lv_obj_user_data_t": "i",

  "lv_coord_t": "i",
  "lv_res_t": "i",
  "lv_style_int_t": "i",
  "lv_opa_t": "i",
  "lv_drag_dir_t": "i",
  "lv_bidi_dir_t": "i",
  "lv_state_t": "i",
  "lv_arc_type_t":"i",
  "lv_chart_axis_t":"i",
  "lv_bar_type_t":"i",
  "lv_btn_state_t":"i",
  "lv_point_t":"i",
  "lv_chart_type_t":"i",
  "lv_fit_t":"i",
  "lv_cpicker_color_mode_t":"i",
  "lv_dropdown_dir_t":"i",
  "lv_layout_t":"i",
  "lv_color_hsv_t":"i",
  "lv_label_long_mode_t": "i",
  "lv_label_align_t": "i",
  "lv_scrollbar_mode_t": "i",
  "lv_spinbox_part_t": "i",
  "lv_spinner_style_t": "i",
  "lv_anim_value_t": "i",
  "lv_switch_part_t": "i",
  "lv_label_align_t": "i",
  "lv_tabview_btns_pos_t": "i",
  "lv_textarea_style_t": "i",
  "lv_slider_type_t": "i",
  "lv_spinner_type_t": "i",
  "lv_spinner_dir_t": "i",
  "lv_blend_mode_t": "i",
  "lv_grad_dir_t": "i",
  "lv_border_side_t": "i",
  "lv_align_t": "i",
  "lv_keyboard_mode_t": "i",
  # "lv_group_focus_cb_t": "i",
  "lv_indev_type_t": "i",


  "lv_obj_t *": "lv_obj",
  "lv_color_t": "lv_color",
  "lv_style_t *": "lv_style",
  "lv_group_t *": "lv_group",
  #"lv_disp_t*": "lv_disp",
  #"lv_style_list_t*": "",
}

lv = {}
lvs = []   # special case for case for lv_style
lv0 = []        # function in lvlg module
lv_module = []
lv_cb_types = []  # list of callback types that will need each a separate C callback, later sorted by first argument

lv_widgets = ['arc', 'bar', 'btn', 'btnmatrix', 'calendar', 'canvas', 'chart', 'checkbox',
             'cont', 'cpicker', 'dropdown', 'gauge', 'img', 'imgbtn', 'keyboard', 'label', 'led', 'line',
             'linemeter', 'list', 'msgbox', 'objmask', 'templ', 'page', 'roller', 'slider', 'spinbox',
             'spinner', 'switch', 'table', 'tabview', 'textarea', 'tileview', 'win']
lv_prefix = ['obj', 'group', 'style', 'indev'] + lv_widgets

def try_int(s):
  try:
    v = int(s)
    return v
  except ValueError:
    return None

# convert a C return type to the string representation for the Berry parser
# using return_types
def c_convert_ret_type(c_ret):
  return return_types.get(c_ret, "")

# parse widgets files containing most function calls
with open(lv_widgets_file) as f:
  for l_raw in f:
    l_raw = re.sub('//.*$', '', l_raw) # remove trailing comments
    l_raw = re.sub('\s+', ' ', l_raw) # replace any multi-space with a single space
    l_raw = l_raw.strip(" \t\n\r")    # remove leading or trailing spaces
    l_raw = re.sub('static ', '', l_raw)
    l_raw = re.sub('inline ', '', l_raw)
    if (len(l_raw) == 0): continue

    g = parse_func_def.search(l_raw)
    if g:
      # if match, we parse the line
      # Ex: 'void lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent);'
      ret_type = g.group(1)   # return type of the function
      # Ex: ret_type -> 'void'

      if ret_type not in return_types:
        print(f"  // Skipping unsupported return type: {ret_type}")
        continue

      # convert return type
      c_ret = c_convert_ret_type(ret_type)

      # convert arguments
      c_args = ""
      args_raw = [ x.strip(" \t\n\r") for x in g.group(3).split(",") ]  # split by comma and strip
      for arg_raw in args_raw:
        # Ex: 'const lv_obj_t * parent' -> 'const ', 'lv_obj_t', ' * ', 'parent'
        # Ex: 'bool auto_fit' -> '', 'bool', ' ', 'auto_fit'
        ga = parse_arg.search(arg_raw)
        if ga:    # parsing ok?
          ga_type = ga.group(2)
          ga_ptr = ( ga.group(3).strip(" \t\n\r") == "*" )    # boolean
          ga_name = ga.group(4)
          ga_type_ptr = ga_type
          if ga_ptr: ga_type_ptr += " *"
          if ga_type_ptr in return_types:
            ga_type = return_types[ga_type_ptr]
          else:
            # remove the trailing '_t' of type name if any
            ga_type = re.sub(r"_t$", "", ga_type)
          
          # if the type is a single letter, we just add it
          if len(ga_type) == 1:
            c_args += ga_type
          else:
            if ga_type.endswith("_cb"):
              # it's a callback type, we encode it differently
              if ga_type not in lv_cb_types:
                lv_cb_types.append(ga_type)
              c_args += "&" + str(lv_cb_types.index(ga_type))
            else:
              # we have a high-level type that we treat as a class name, enclose in parenthesis
              c_args += "(" + ga_type + ")"

      # analyze function name and determine if it needs to be assigned to a specific class
      func_name = g.group(2)
      # Ex: func_name -> 'lv_obj_set_parent'
      if func_name.startswith("_"): continue            # skip low-level
      if func_name.startswith("lv_debug_"): continue    # skip debug

      c_func_name = ""

      found = False
      for subtype in lv_prefix:
        if func_name == "lv_style_init": continue   # no need for init as it would collied with native init (and called behind the scene anyways)
        if func_name.startswith("lv_" + subtype + "_"):
          be_name = re.sub("^lv_" + subtype + "_", '', func_name)
          c_func_name = "lvbe_" + subtype + "_" + be_name
          if subtype not in lv: lv[subtype] = []    # add entry
          # special case if the function is create, we change the return type to macth the class
          # if be_name == 'create':
          #   c_ret = "lv_" + subtype
          #   lv0.append( [ c_func_name, c_ret, c_args, func_name, subtype ] )  # also add to lvgl
          lv[subtype].append( [ c_func_name, c_ret, c_args, func_name, be_name ] )
          found = True
          break

      if found: continue

print("| callback types"+str(lv_cb_types))

# parse module file containing numerical constants
with open(lv_module_file) as f:
  for l_raw in f:
    l_raw = l_raw.strip(" \t\n\r")    # remove leading or trailing spaces
    if l_raw.startswith("//"):
      lv_module.append( [ None, l_raw ] )   # if key in None then add comment line
    l_raw = re.sub('//.*$', '', l_raw) # remove trailing comments
    l_raw = re.sub('\s+', '', l_raw) # remove all spaces
    l_raw = re.sub(',.*$', '', l_raw) # remove comma and anything after it
    if (len(l_raw) == 0): continue

    k_v = l_raw.split("=")
    if len(k_v) > 2:
      print(f"Error: cannot match {l_raw}")
      continue
    # extract the key name
    k = k_v[0]
    if k.startswith("_"):
      continue      # skip any label starting with '_'
    k = re.sub('^LV_', '', k) # remove remove any LV_ prefix
    if len(k_v) == 2:   # value is included
      v = k_v[1]
      if k is None or v is None: continue
      v_num = try_int(eval(v))
    else:             # no value, we use the C value instead
      v_num = None
      v = None


    if not k.isidentifier():
      print(f"Error: {k} is not an identifier")
      continue
    
    if v_num is not None:
      lv_module.append( [k, v_num] )
    else:
      lv_module.append( [k, v] )    # keep as string or None

# recursively try to match value
# TODO

# print(lv)

      #print(f"orig {func_name} berry {c_func_name}")

      #print(g.group(1))
      #print(g.group(2))
      #print(g.group(3))


sys.stdout = open(out_prefix + be_lv_defines, 'w')


print("/********************************************************************")
print(" * Generated code, don't edit")
print(" *******************************************************************/")
print(" // Configuration")

for subtype in lv_widgets:
  define = f"BE_LV_WIDGET_" + subtype.upper()
  print(f"#ifndef {define}")
  print(f"#define {define} 1")
  print(f"#endif")

print("/********************************************************************/")
sys.stdout.close()


sys.stdout = open(out_prefix + be_lv_c_mapping, 'w')
print("""
/********************************************************************
 * Generated code, don't edit
  *******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


""")

for subtype, flv in lv.items():
  print(f"/* `lv_{subtype}` methods */")
  if subtype in lv_widgets:
    print(f"#if BE_LV_WIDGET_{subtype.upper()}")
  print(f"const lvbe_call_c_t lv_{subtype}_func[] = {{")

  func_out = {} # used to sort output
  for f in flv:
    c_func_name = f[0]
    c_ret_type = f[1]
    c_argc = f[2]
    if c_argc is not None: c_argc = "\"" + c_argc + "\""
    orig_func_name = f[3]
    be_name = f[4]
    if c_func_name.endswith("_create"):
      c_ret_type = f"+lv_{subtype}"
    func_out[be_name] = f"  {{ \"{be_name}\", (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'} }},"

  for be_name in sorted(func_out):
    print(func_out[be_name])

  print(f"}};")
  if subtype in lv_widgets:
    print(f"#endif // BE_LV_WIDGET_{subtype.upper()}")
  print(f"")

# print the global map of classes
print(f"""
// map of clases
const lvbe_call_c_classes_t lv_classes[] = {{""")

for subtype in sorted(lv):
# for subtype, flv in lv.items():
  if subtype in lv_widgets:
    print(f"#if BE_LV_WIDGET_{subtype.upper()}")
  print(f"  {{ \"lv_{subtype}\", lv_{subtype}_func, sizeof(lv_{subtype}_func) / sizeof(lv_{subtype}_func[0]) }},")
  if subtype in lv_widgets:
    print(f"#endif // BE_LV_WIDGET_{subtype.upper()}")

print(f"""}};
const size_t lv_classes_size = sizeof(lv_classes) / sizeof(lv_classes[0]);
""")

# previous generation calls

# keep only create
for subtype, flv in lv.items():
  print(f"  /* `lv_{subtype}` methods */")
  for f in flv:
    c_func_name = f[0]
    c_ret_type = f[1]
    c_argc = f[2]
    if c_argc is not None: c_argc = "\"" + c_argc + "\""
    orig_func_name = f[3]
    if c_func_name.endswith("_create"):
      c_ret_type = f"+lv_{subtype}"
      if subtype in lv_widgets:
        print(f"#if BE_LV_WIDGET_{subtype.upper()}")
      print(f"  int {c_func_name}(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'}); }}")
      if subtype in lv_widgets:
        print(f"#endif // BE_LV_WIDGET_{subtype.upper()}")
  print()

print()
for subtype in lv:
  print(f"  extern void be_load_lv_{subtype}_lib(bvm *vm);")
print()
print(f"  void be_load_lv_all_lib(bvm *vm) {{")

for subtype in lv:
  define = f"BE_LV_WIDGET_" + subtype.upper()
  if subtype in lv_widgets:
    print(f"#if {define}")
  print(f"    be_load_lv_{subtype}_lib(vm);")
  if subtype in lv_widgets:
    print(f"#endif")

print(f"  }};")

print("#ifdef __cplusplus")
print("} /* extern \"C\" */")
print("#endif")
print("/********************************************************************/")
sys.stdout.close()


sys.stdout = open(lvgl_prefix + be_lv_widgets_libs, 'w')
print("""
/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
 
 /********************************************************************
 * Tasmota LVGL classes for widgets
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"
#include "be_lvgl.h"

extern int lv0_init(bvm *vm);

extern int lco_init(bvm *vm);
extern int lco_tostring(bvm *vm);

extern int lvx_init_2(bvm *vm);           // generic function
extern int lvx_member(bvm *vm);
extern int lvx_tostring(bvm *vm);       // generic function

extern int lvs_init(bvm *vm);
extern int lvs_tostring(bvm *vm);

BE_EXPORT_VARIABLE extern const bclass be_class_lv_obj;
""")

for subtype, flv in lv.items():
  print(f"/* `lv_{subtype}` external functions definitions */")
  for f in flv:
    c_func_name = f[0]
    print(f"extern int {c_func_name}(bvm *vm);")
  
  print()

print("""
#if BE_USE_PRECOMPILED_OBJECT
""")

for subtype, flv in lv.items():
  print(f"#include \"../generate/be_fixed_be_class_lv_{subtype}.h\"")

print("""
#endif
""")

print()

for subtype, flv in lv.items():
  # class definitions
  print(f"void be_load_lv_{subtype}_lib(bvm *vm) {{")
  print("#if !BE_USE_PRECOMPILED_OBJECT")
  print(f"  static const bnfuncinfo members[] = {{")
  print(f"    {{ \".p\", NULL }},")

  if subtype == "style":
    print(f"    {{ \"init\", lvs_init }},")
    print(f"    {{ \"tostring\", lvs_tostring }},")
  else:
    if subtype != 'indev':    # indev has no create
      print(f"    {{ \"init\", lvbe_{subtype}_create }},")
    else:
      print(f"    {{ \"init\", lv0_init }},")
    print(f"    {{ \"tostring\", lvx_tostring }},")
  print(f"    {{ \"member\", lvx_member }},")

  print()
  # for f in flv:

  #   c_func_name = f[0]
  #   be_name = f[4]
  #   print(f"    {{ \"{be_name}\", {c_func_name} }},")

  print()
  print(f"    // {{ NULL, (bntvfunc) BE_CLOSURE }}, /* mark section for berry closures */")
  print()
  print(f"    {{ NULL, NULL }}")
  print(f"  }};")
  print(f"  be_regclass(vm, \"lv_{subtype}\", members);")
  if subtype != "obj" and subtype != "group":   # lv_obj and lv_group do not inherit from lv_obj
    print()
    print(f"  be_getglobal(vm, \"lv_{subtype}\");")
    print(f"  be_getglobal(vm, \"lv_obj\");")
    print(f"  be_setsuper(vm, -2);")
    print(f"  be_pop(vm, 2);")
  
  print("#else")
  print(f"    be_pushntvclass(vm, &be_class_lv_{subtype});")
  print(f"    be_setglobal(vm, \"lv_{subtype}\");")
  print(f"    be_pop(vm, 1);")
  print("#endif")

  print(f"}};")
  print()
  print(f"/* @const_object_info_begin")
  if subtype != "obj" and subtype != "group":   # lv_obj and lv_group do not inherit from lv_obj
    print(f"class be_class_lv_{subtype} (scope: global, name: lv_{subtype}, super: be_class_lv_obj) {{")
  else:
    print(f"class be_class_lv_{subtype} (scope: global, name: lv_{subtype}) {{")

  print(f"    .p, var")
  if subtype == "style":
    print(f"    init, func(lvs_init)")
    print(f"    tostring, func(lvs_tostring)")
  else:
    if subtype != 'indev':    # indev has no create
      print(f"    init, func(lvbe_{subtype}_create)")
    else:
      print(f"    init, func(lv0_init)")
    print(f"    tostring, func(lvx_tostring)")
  print(f"    member, func(lvx_member)")
  print()

  # for f in flv:
  #   c_func_name = f[0]
  #   be_name = f[4]
  #   print(f"    {be_name}, func({c_func_name})")

  print(f"}}")
  print(f"@const_object_info_end */")
  print()

print("""
#endif // USE_LVGL
""")
sys.stdout.close()


sys.stdout = open(lvgl_prefix + be_lv_lvgl_module, 'w')
print("/********************************************************************")
print(" * Generated code, don't edit")
print(" *******************************************************************/")

print("""/********************************************************************
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

""")

print("""

typedef struct be_constint_t {
    const char * name;
    int32_t      value;
} be_constint_t;

const be_constint_t lv0_constants[] = {
""")

lv_module2 = {}
for k_v in lv_module:
  (k,v) = k_v
  if k is not None:
    lv_module2[k] = v

for k in sorted(lv_module2):
  v = lv_module2[k]
  if v is not None:
    print(f"    {{ \"{k}\", {v} }},")
  else:
    print(f"    {{ \"{k}\", LV_{k} }},")

print("""
};

const size_t lv0_constants_size = sizeof(lv0_constants)/sizeof(lv0_constants[0]);
""")

for f in lv0:
  c_func_name = f[0]
  print(f"extern int {c_func_name}(bvm *vm);")
  

print("""
#if !BE_USE_PRECOMPILED_OBJECT
//#if 1           // TODO we will do pre-compiled later


be_native_module_attr_table(lvgl) {
    // Symbols    
    be_native_module_str("SYMBOL_AUDIO", "\\xef\\x80\\x81"),
    be_native_module_str("SYMBOL_VIDEO", "\\xef\\x80\\x88"),
    be_native_module_str("SYMBOL_LIST", "\\xef\\x80\\x8b"),
    be_native_module_str("SYMBOL_OK", "\\xef\\x80\\x8c"),
    be_native_module_str("SYMBOL_CLOSE", "\\xef\\x80\\x8d"),
    be_native_module_str("SYMBOL_POWER", "\\xef\\x80\\x91"),
    be_native_module_str("SYMBOL_SETTINGS", "\\xef\\x80\\x93"),
    be_native_module_str("SYMBOL_HOME", "\\xef\\x80\\x95"),
    be_native_module_str("SYMBOL_DOWNLOAD", "\\xef\\x80\\x99"),
    be_native_module_str("SYMBOL_DRIVE", "\\xef\\x80\\x9c"),
    be_native_module_str("SYMBOL_REFRESH", "\\xef\\x80\\xa1"),
    be_native_module_str("SYMBOL_MUTE", "\\xef\\x80\\xa6"),
    be_native_module_str("SYMBOL_VOLUME_MID", "\\xef\\x80\\xa7"),
    be_native_module_str("SYMBOL_VOLUME_MAX", "\\xef\\x80\\xa8"),
    be_native_module_str("SYMBOL_IMAGE", "\\xef\\x80\\xbe"),
    be_native_module_str("SYMBOL_EDIT", "\\xef\\x8C\\x84"),
    be_native_module_str("SYMBOL_PREV", "\\xef\\x81\\x88"),
    be_native_module_str("SYMBOL_PLAY", "\\xef\\x81\\x8b"),
    be_native_module_str("SYMBOL_PAUSE", "\\xef\\x81\\x8c"),
    be_native_module_str("SYMBOL_STOP", "\\xef\\x81\\x8d"),
    be_native_module_str("SYMBOL_NEXT", "\\xef\\x81\\x91"),
    be_native_module_str("SYMBOL_EJECT", "\\xef\\x81\\x92"),
    be_native_module_str("SYMBOL_LEFT", "\\xef\\x81\\x93"),
    be_native_module_str("SYMBOL_RIGHT", "\\xef\\x81\\x94"),
    be_native_module_str("SYMBOL_PLUS", "\\xef\\x81\\xa7"),
    be_native_module_str("SYMBOL_MINUS", "\\xef\\x81\\xa8"),
    be_native_module_str("SYMBOL_EYE_OPEN", "\\xef\\x81\\xae"),
    be_native_module_str("SYMBOL_EYE_CLOSE", "\\xef\\x81\\xb0"),
    be_native_module_str("SYMBOL_WARNING", "\\xef\\x81\\xb1"),
    be_native_module_str("SYMBOL_SHUFFLE", "\\xef\\x81\\xb4"),
    be_native_module_str("SYMBOL_UP", "\\xef\\x81\\xb7"),
    be_native_module_str("SYMBOL_DOWN", "\\xef\\x81\\xb8"),
    be_native_module_str("SYMBOL_LOOP", "\\xef\\x81\\xb9"),
    be_native_module_str("SYMBOL_DIRECTORY", "\\xef\\x81\\xbb"),
    be_native_module_str("SYMBOL_UPLOAD", "\\xef\\x82\\x93"),
    be_native_module_str("SYMBOL_CALL", "\\xef\\x82\\x95"),
    be_native_module_str("SYMBOL_CUT", "\\xef\\x83\\x84"),
    be_native_module_str("SYMBOL_COPY", "\\xef\\x83\\x85"),
    be_native_module_str("SYMBOL_SAVE", "\\xef\\x83\\x87"),
    be_native_module_str("SYMBOL_CHARGE", "\\xef\\x83\\xa7"),
    be_native_module_str("SYMBOL_PASTE", "\\xef\\x83\\xAA"),
    be_native_module_str("SYMBOL_BELL", "\\xef\\x83\\xb3"),
    be_native_module_str("SYMBOL_KEYBOARD", "\\xef\\x84\\x9c"),
    be_native_module_str("SYMBOL_GPS", "\\xef\\x84\\xa4"),
    be_native_module_str("SYMBOL_FILE", "\\xef\\x85\\x9b"),
    be_native_module_str("SYMBOL_WIFI", "\\xef\\x87\\xab"),
    be_native_module_str("SYMBOL_BATTERY_FULL", "\\xef\\x89\\x80"),
    be_native_module_str("SYMBOL_BATTERY_3", "\\xef\\x89\\x81"),
    be_native_module_str("SYMBOL_BATTERY_2", "\\xef\\x89\\x82"),
    be_native_module_str("SYMBOL_BATTERY_1", "\\xef\\x89\\x83"),
    be_native_module_str("SYMBOL_BATTERY_EMPTY", "\\xef\\x89\\x84"),
    be_native_module_str("SYMBOL_USB", "\\xef\\x8a\\x87"),
    be_native_module_str("SYMBOL_BLUETOOTH", "\\xef\\x8a\\x93"),
    be_native_module_str("SYMBOL_TRASH", "\\xef\\x8B\\xAD"),
    be_native_module_str("SYMBOL_BACKSPACE", "\\xef\\x95\\x9A"),
    be_native_module_str("SYMBOL_SD_CARD", "\\xef\\x9F\\x82"),
    be_native_module_str("SYMBOL_NEW_LINE", "\\xef\\xA2\\xA2"),

    be_native_module_str("SYMBOL_DUMMY", "\\xEF\\xA3\\xBF"),

    be_native_module_str("SYMBOL_BULLET", "\\xE2\\x80\\xA2"),

""")

print("/* `lvgl` module functions */")
print()


# for subtype, flv in lv.items():
#   if subtype != 'xxx': continue
#   print(f"/* `lv_{subtype}` external functions definitions */")
#   for f in flv:
#     c_func_name = f[0]
#     print(f"extern int {c_func_name}(bvm *vm);")
  
#   print()

# for k_v in lv_module:
#   (k,v) = k_v
#   if k is None:
#     print(v)      # comment line
#     continue

#   print(f"    be_native_module_int(\"{k}\", {v}),")

print("""

    be_native_module_function("member", lv0_member),
    be_native_module_function("start", lv0_start),

    be_native_module_function("register_button_encoder", lv0_register_button_encoder),

    be_native_module_function("montserrat_font", lv0_load_montserrat_font),
    be_native_module_function("seg7_font", lv0_load_seg7_font),
    be_native_module_function("load_font", lv0_load_font),
    be_native_module_function("load_freetype_font", lv0_load_freetype_font),

    be_native_module_function("screenshot", lv0_screenshot),
""")

for f in lv0:

  c_func_name = f[0]
  be_name = f[4]
  print(f"    be_native_module_function(\"{be_name}\", {c_func_name}),")

print("""
};

be_define_native_module(lvgl, NULL);

#else

be_define_local_const_str(SYMBOL_AUDIO, "\\xef\\x80\\x81", 0, 3);
be_define_local_const_str(SYMBOL_VIDEO, "\\xef\\x80\\x88", 0, 3);
be_define_local_const_str(SYMBOL_LIST, "\\xef\\x80\\x8b", 0, 3);
be_define_local_const_str(SYMBOL_OK, "\\xef\\x80\\x8c", 0, 3);
be_define_local_const_str(SYMBOL_CLOSE, "\\xef\\x80\\x8d", 0, 3);
be_define_local_const_str(SYMBOL_POWER, "\\xef\\x80\\x91", 0, 3);
be_define_local_const_str(SYMBOL_SETTINGS, "\\xef\\x80\\x93", 0, 3);
be_define_local_const_str(SYMBOL_HOME, "\\xef\\x80\\x95", 0, 3);
be_define_local_const_str(SYMBOL_DOWNLOAD, "\\xef\\x80\\x99", 0, 3);
be_define_local_const_str(SYMBOL_DRIVE, "\\xef\\x80\\x9c", 0, 3);
be_define_local_const_str(SYMBOL_REFRESH, "\\xef\\x80\\xa1", 0, 3);
be_define_local_const_str(SYMBOL_MUTE, "\\xef\\x80\\xa6", 0, 3);
be_define_local_const_str(SYMBOL_VOLUME_MID, "\\xef\\x80\\xa7", 0, 3);
be_define_local_const_str(SYMBOL_VOLUME_MAX, "\\xef\\x80\\xa8", 0, 3);
be_define_local_const_str(SYMBOL_IMAGE, "\\xef\\x80\\xbe", 0, 3);
be_define_local_const_str(SYMBOL_EDIT, "\\xef\\x8C\\x84", 0, 3);
be_define_local_const_str(SYMBOL_PREV, "\\xef\\x81\\x88", 0, 3);
be_define_local_const_str(SYMBOL_PLAY, "\\xef\\x81\\x8b", 0, 3);
be_define_local_const_str(SYMBOL_PAUSE, "\\xef\\x81\\x8c", 0, 3);
be_define_local_const_str(SYMBOL_STOP, "\\xef\\x81\\x8d", 0, 3);
be_define_local_const_str(SYMBOL_NEXT, "\\xef\\x81\\x91", 0, 3);
be_define_local_const_str(SYMBOL_EJECT, "\\xef\\x81\\x92", 0, 3);
be_define_local_const_str(SYMBOL_LEFT, "\\xef\\x81\\x93", 0, 3);
be_define_local_const_str(SYMBOL_RIGHT, "\\xef\\x81\\x94", 0, 3);
be_define_local_const_str(SYMBOL_PLUS, "\\xef\\x81\\xa7", 0, 3);
be_define_local_const_str(SYMBOL_MINUS, "\\xef\\x81\\xa8", 0, 3);
be_define_local_const_str(SYMBOL_EYE_OPEN, "\\xef\\x81\\xae", 0, 3);
be_define_local_const_str(SYMBOL_EYE_CLOSE, "\\xef\\x81\\xb0", 0, 3);
be_define_local_const_str(SYMBOL_WARNING, "\\xef\\x81\\xb1", 0, 3);
be_define_local_const_str(SYMBOL_SHUFFLE, "\\xef\\x81\\xb4", 0, 3);
be_define_local_const_str(SYMBOL_UP, "\\xef\\x81\\xb7", 0, 3);
be_define_local_const_str(SYMBOL_DOWN, "\\xef\\x81\\xb8", 0, 3);
be_define_local_const_str(SYMBOL_LOOP, "\\xef\\x81\\xb9", 0, 3);
be_define_local_const_str(SYMBOL_DIRECTORY, "\\xef\\x81\\xbb", 0, 3);
be_define_local_const_str(SYMBOL_UPLOAD, "\\xef\\x82\\x93", 0, 3);
be_define_local_const_str(SYMBOL_CALL, "\\xef\\x82\\x95", 0, 3);
be_define_local_const_str(SYMBOL_CUT, "\\xef\\x83\\x84", 0, 3);
be_define_local_const_str(SYMBOL_COPY, "\\xef\\x83\\x85", 0, 3);
be_define_local_const_str(SYMBOL_SAVE, "\\xef\\x83\\x87", 0, 3);
be_define_local_const_str(SYMBOL_CHARGE, "\\xef\\x83\\xa7", 0, 3);
be_define_local_const_str(SYMBOL_PASTE, "\\xef\\x83\\xAA", 0, 3);
be_define_local_const_str(SYMBOL_BELL, "\\xef\\x83\\xb3", 0, 3);
be_define_local_const_str(SYMBOL_KEYBOARD, "\\xef\\x84\\x9c", 0, 3);
be_define_local_const_str(SYMBOL_GPS, "\\xef\\x84\\xa4", 0, 3);
be_define_local_const_str(SYMBOL_FILE, "\\xef\\x85\\x9b", 0, 3);
be_define_local_const_str(SYMBOL_WIFI, "\\xef\\x87\\xab", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_FULL, "\\xef\\x89\\x80", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_3, "\\xef\\x89\\x81", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_2, "\\xef\\x89\\x82", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_1, "\\xef\\x89\\x83", 0, 3);
be_define_local_const_str(SYMBOL_BATTERY_EMPTY, "\\xef\\x89\\x84", 0, 3);
be_define_local_const_str(SYMBOL_USB, "\\xef\\x8a\\x87", 0, 3);
be_define_local_const_str(SYMBOL_BLUETOOTH, "\\xef\\x8a\\x93", 0, 3);
be_define_local_const_str(SYMBOL_TRASH, "\\xef\\x8B\\xAD", 0, 3);
be_define_local_const_str(SYMBOL_BACKSPACE, "\\xef\\x95\\x9A", 0, 3);
be_define_local_const_str(SYMBOL_SD_CARD, "\\xef\\x9F\\x82", 0, 3);
be_define_local_const_str(SYMBOL_NEW_LINE, "\\xef\\xA2\\xA2", 0, 3);

be_define_local_const_str(SYMBOL_DUMMY, "\\xEF\\xA3\\xBF", 0, 3);

be_define_local_const_str(SYMBOL_BULLET, "\\xE2\\x80\\xA2", 0, 3);


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
""")


# for k_v in lv_module:
#   (k,v) = k_v
#   if k is None:
#     continue

#   print(f"    {k}, int({v})")

print("""
    member, func(lv0_member)
    start, func(lv0_start)

    register_button_encoder, func(lv0_register_button_encoder)

    montserrat_font, func(lv0_load_montserrat_font)
    seg7_font, func(lv0_load_seg7_font)
    load_font, func(lv0_load_font)
    load_freetype_font, func(lv0_load_freetype_font)

    screenshot, func(lv0_screenshot)

""")

for f in lv0:

  c_func_name = f[0]
  be_name = f[4]
  print(f"    {be_name}, func({c_func_name})")

print("""
}
@const_object_info_end */
#include "../generate/be_fixed_lvgl.h"
#endif

#endif // USE_LVGL
""")

print("/********************************************************************/")
sys.stdout.close()

