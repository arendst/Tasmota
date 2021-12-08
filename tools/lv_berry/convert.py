import re
import sys

lv_widgets_file = "lv_funcs.h"
lv_module_file = "lv_enum.h"

out_prefix = "../../tasmota/lvgl_berry/"
lvgl_prefix = "../../lib/libesp32/Berry/default/"

be_lv_defines = "be_lv_defines.h"
be_lv_c_mapping = "be_lv_c_mapping.h"
be_lv_widgets_libs = "be_lvgl_widgets_lib.c"
be_lv_lvgl_module = "be_lvgl_module.c"

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

  "lv_objmask_mask_t *": ".",

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
  "lv_indev_type_t": "i",
  "lv_event_t": "i",
  "lv_signal_t": "i",
  "lv_img_cf_t": "i",
  "lv_draw_mask_line_side_t": "i",
  "lv_style_property_t": "i",
  "lv_dir_t": "i",
  "lv_part_t": "i",
  "lv_base_dir_t": "i",
  "lv_text_decor_t": "i",
  "lv_text_align_t": "i",
  "lv_arc_mode_t": "i",
  "lv_bar_mode_t": "i",
  "lv_event_code_t": "i",
  "lv_obj_flag_t": "i",
  "lv_slider_mode_t": "i",
  "lv_scroll_snap_t": "i",
  "lv_style_value_t": "i",
  "lv_img_src_t": "i",
  "lv_colorwheel_mode_t": "i",
  "lv_scr_load_anim_t": "i",
  "lv_style_selector_t": "i",

  "_lv_event_dsc_t *": "i",

  # "lv_signal_cb_t": "c",
  # "lv_design_cb_t": "c",
  # "lv_event_cb_t": "c",
  # "lv_group_focus_cb_t": "c",

  # ctypes objects
  "lv_area_t *": "lv_area",
  "lv_meter_scale_t *": "lv_meter_scale",
  "lv_meter_indicator_t *": "lv_meter_indicator",
  "lv_obj_class_t *": "lv_obj_class",

  "_lv_obj_t *": "lv_obj",
  "lv_obj_t *": "lv_obj",
  "lv_color_t": "lv_color",
  "lv_style_t *": "lv_style",
  "lv_group_t *": "lv_group",
  "lv_font_t *": "lv_font",
  "lv_theme_t *": "lv_theme",
  "lv_disp_t *": "lv_disp",
  "lv_indev_t *": "lv_indev",
  #"lv_disp_t*": "lv_disp",
  #"lv_style_list_t*": "",

  # callbacks
  "lv_group_focus_cb_t": "lv_group_focus_cb",
  "lv_event_cb_t": "lv_event_cb",
  # "lv_signal_cb_t": "lv_signal_cb",   # removed in LVGL8
  # "lv_design_cb_t": "lv_design_cb",   # removed in LVGL8
  # "lv_gauge_format_cb_t": "lv_gauge_format_cb", # removed in LVGL8
}

lv = {}
lvs = []   # special case for case for lv_style
lv0 = []        # function in lvlg module
lv_module = []
lv_cb_types = ['lv_group_focus_cb', 'lv_event_cb',
               'lv_constructor_cb',                 # 'constructor_cb', addition to LVGL8, also works for 'destructor_cb'
               ]
# list of callback types that will need each a separate C callback

# For LVGL8, need to add synthetic lv_style, lv_font, lv_color, lv_theme
lv['style'] = []
lv['font'] = []
lv['color'] = []
lv['theme'] = []

# standard widgets
lv_widgets = ['arc', 'bar', 'btn', 'btnmatrix', 'canvas', 'checkbox',
              'dropdown', 'img', 'label', 'line', 'roller', 'slider',
              'switch', 'table', 'textarea' ]
# extra widgets

lv_widgets = lv_widgets + [ 'chart', 'colorwheel', 'imgbtn', 'led', 'meter', 'msgbox', 'spinbox', 'spinner' ]
lv_prefix = ['obj', 'group', 'style', 'indev', 'disp'] + lv_widgets

# define here widget inheritance because it's hard to deduce from source
lv_widget_inheritance = {
  "animing": "img",
  "calendar": "btnmatrix",
  "keyboard": "btnmatrix",
  "list_btn": "btn",
  "list_text": "label",
  "spinbox": "textarea",
  "spinner": "arc",             # lv_spinner is a subclass of lv_arc
  "canvas": "img",
  "roller_label": "label",
}

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
    l_raw = re.sub('LV_ATTRIBUTE_FAST_MEM ', '', l_raw) # remove LV_ATTRIBUTE_FAST_MEM marker
    l_raw = re.sub('\s+', ' ', l_raw) # replace any multi-space with a single space
    l_raw = l_raw.strip(" \t\n\r")    # remove leading or trailing spaces
    l_raw = re.sub('static ', '', l_raw)
    l_raw = re.sub('inline ', '', l_raw)
    l_raw = re.sub('const ', '', l_raw)
    l_raw = re.sub('struct ', '', l_raw)
    if (len(l_raw) == 0): continue

    g = parse_func_def.search(l_raw)
    if g:
      # if match, we parse the line
      # Ex: 'void lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent);'
      ret_type = g.group(1)   # return type of the function
      # Ex: ret_type -> 'void'

      ret_type_without_t = re.sub(r"_t$", "", ret_type)
      if ret_type in return_types:
        # convert return type
        c_ret = c_convert_ret_type(ret_type)
      elif ret_type_without_t in lv_cb_types:
        c_ret = "c"   # general callback, if not already captured by explicit type
      else:
        print(f"  // Skipping unsupported return type: {ret_type}")
        continue


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
          if len(ga_type) == 1 and ga_type != 'c':  # callbacks are different
            c_args += ga_type
          else:
            if ga_type.endswith("_cb"):
              # it's a callback type, we encode it differently
              if ga_type not in lv_cb_types:
                lv_cb_types.append(ga_type)
              c_args += "^" + ga_type + "^"
            else:
              # we have a high-level type that we treat as a class name, enclose in parenthesis
              c_args += "(" + "lv." + ga_type + ")"

      # analyze function name and determine if it needs to be assigned to a specific class
      func_name = g.group(2)
      # Ex: func_name -> 'lv_obj_set_parent'
      if func_name.startswith("_"): continue            # skip low-level
      if func_name.startswith("lv_debug_"): continue    # skip debug

      c_func_name = ""

      if func_name == "lv_style_init":
        continue   # no need for init as it would collied with native init (and called behind the scene anyways)

      found = False
      for subtype in lv_prefix:
        if func_name.startswith("lv_" + subtype + "_"):
          be_name = re.sub("^lv_" + subtype + "_", '', func_name)
          c_func_name = "lvbe_" + subtype + "_" + be_name
          if subtype not in lv: lv[subtype] = []    # add entry
          is_right_type = c_args.startswith(f"(lv.lv_{subtype})")  # check if first arg matches class
          is_obj_arg1 = c_args.startswith(f"(lv.lv_obj)")          # or first arg is lv_obj
          is_group_create = (subtype == 'group') and (func_name == 'lv_group_create')
          if is_right_type or is_obj_arg1 or is_group_create:
            lv[subtype].append( [ c_func_name, c_ret, c_args, func_name, be_name ] )
            found = True
            break

      if found: continue
      # not found, we treat it as lv top level function
      be_name = re.sub("^lv_", '', func_name)
      lv0.append( [ func_name, c_ret, c_args, func_name, be_name ] )
      # print(lv0[-1])

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
    v = None
    if len(k_v) == 2:   # value is included
      v = k_v[1]
      if k is None or v is None: continue   # we skip if key is void or value is void

    if not k.isidentifier():
      print(f"Error: {k} is not an identifier")
      continue

    lv_module.append( [k, v] )    # keep as string or None


sys.stdout = open(out_prefix + be_lv_c_mapping, 'w')
print("""
/********************************************************************
 * Generated code, don't edit
  *******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "be_ctypes.h"
""")

for subtype, flv in lv.items():
  print(f"/* `lv_{subtype}` methods */")
  if subtype in lv_widgets:
    print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}")
  print(f"const lvbe_call_c_t lv_{subtype}_func[] = {{")

  func_out = {} # used to sort output
  for f in flv:
    c_func_name = f[0]
    c_ret_type = f[1]
    c_argc = f[2]
    if c_argc is not None: c_argc = "\"" + c_argc + "\""
    orig_func_name = f[3]
    be_name = f[4]
    # if c_ret_type is an object, prefix with `lv.`
    if len(c_ret_type) > 1: c_ret_type = "lv." + c_ret_type

    if c_func_name.endswith("_create"):
      pass
      # c_ret_type = f"+lv_{subtype}"
    else:
      func_out[be_name] = f"  {{ \"{be_name}\", (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'} }},"

  for be_name in sorted(func_out):
    print(func_out[be_name])

  print(f"}};")
  if subtype in lv_widgets:
    print(f"#endif // BE_LV_WIDGET_{subtype.upper()}")
  print(f"")

# extern classes
for subtype in sorted(lv):
  print(f"extern const bclass be_class_lv_{subtype};");
print()

# print the global map of classes
print(f"""
// map of clases
const lvbe_call_c_classes_t lv_classes[] = {{""")

for subtype in sorted(lv):
# for subtype, flv in lv.items():
  if subtype in lv_widgets:
    print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}")
  print(f"  {{ \"lv_{subtype}\", &be_class_lv_{subtype}, lv_{subtype}_func, sizeof(lv_{subtype}_func) / sizeof(lv_{subtype}_func[0]) }},")
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

    # if c_ret_type is an object, prefix with `lv.`
    if len(c_ret_type) > 1: c_ret_type = "lv." + c_ret_type

    if c_func_name.endswith("_create"):
      c_ret_type = "+"  # constructor, init method does not return any value
      if subtype in lv_widgets:
        print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}")
        print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'}); }}")
        print(f"#endif // BE_LV_WIDGET_{subtype.upper()}")
      else:
        print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'}); }}")

print("""
// create font either empty or from parameter on stack
int lvbe_font_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+lv_font", ""); }
int lvbe_theme_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+lv_theme", ""); }
""")

print()
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

extern int lv0_init(bvm *vm);

extern int lco_init(bvm *vm);           // generic function
extern int lco_tostring(bvm *vm);       // generic function
extern int lco_toint(bvm *vm);          // generic function

extern int lvx_member(bvm *vm);
extern int lvx_tostring(bvm *vm);       // generic function

extern int lvs_init(bvm *vm);
extern int lvs_tostring(bvm *vm);

BE_EXPORT_VARIABLE extern const bclass be_class_lv_obj;

extern int lvbe_font_create(bvm *vm);
extern int lvbe_theme_create(bvm *vm);

""")

# expose all extern definitions:
# Ex:
#
# /* `lv_canvas` external functions definitions */
# extern int lvbe_canvas_create(bvm *vm);
# extern int lvbe_canvas_set_buffer(bvm *vm);
# ...
#
for subtype, flv in lv.items():
  print(f"/* `lv_{subtype}` external functions definitions */")
  for f in flv:
    c_func_name = f[0]
    print(f"extern int {c_func_name}(bvm *vm);")
  
  print()

for subtype, flv in lv.items():
  print(f"""extern int be_ntv_lv_{subtype}_init(bvm *vm);""")

print()

# extern classes
for subtype in sorted(lv):
  print(f"extern const bclass be_class_lv_{subtype};");
print()

# Define specific classes for lv_obj
#
print("""
/********************************************************************
** Solidified class: lv_style
********************************************************************/
be_local_class(lv_style,
    1,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_func(lvs_init) },
        { be_nested_key("tostring", -1995258651, 8, -1), be_const_func(lvs_tostring) },
        { be_nested_key("_p", 1594591802, 2, -1), be_const_var(0) },
        { be_nested_key("member", 719708611, 6, 0), be_const_func(lvx_member) },
    })),
    (be_nested_const_str("lv_style", -143355747, 8))
);
/*******************************************************************/

/********************************************************************
** Solidified class: lv_obj
********************************************************************/
be_local_class(lv_obj,
    1,
    NULL,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("tostring", -1995258651, 8, 3), be_const_func(lvx_tostring) },
        { be_nested_key("member", 719708611, 6, -1), be_const_func(lvx_member) },
        { be_nested_key("_p", 1594591802, 2, -1), be_const_var(0) },
        { be_nested_key("init", 380752755, 4, 4), be_const_func(be_ntv_lv_obj_init) },
        { be_nested_key("_class", -1562820946, 6, -1), be_const_comptr(&lv_obj_class) },
    })),
    (be_nested_const_str("lv_obj", -37134147, 6))
);
/*******************************************************************/

/********************************************************************
** Solidified class: lv_group
********************************************************************/
be_local_class(lv_group,
    1,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_func(be_ntv_lv_group_init) },
        { be_nested_key("tostring", -1995258651, 8, -1), be_const_func(lvx_tostring) },
        { be_nested_key("_p", 1594591802, 2, -1), be_const_var(0) },
        { be_nested_key("member", 719708611, 6, 0), be_const_func(lvx_member) },
    })),
    (be_nested_const_str("lv_group", -442928277, 8))
);
/*******************************************************************/

/********************************************************************
** Solidified class: lv_indev
********************************************************************/
be_local_class(lv_indev,
    1,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_func(lv0_init) },
        { be_nested_key("tostring", -1995258651, 8, -1), be_const_func(lvx_tostring) },
        { be_nested_key("_p", 1594591802, 2, -1), be_const_var(0) },
        { be_nested_key("member", 719708611, 6, 0), be_const_func(lvx_member) },
    })),
    (be_nested_const_str("lv_indev", 225602374, 8))
);
/*******************************************************************/

/********************************************************************
** Solidified class: lv_disp
********************************************************************/
be_local_class(lv_disp,
    1,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_func(lv0_init) },
        { be_nested_key("tostring", -1995258651, 8, -1), be_const_func(lvx_tostring) },
        { be_nested_key("_p", 1594591802, 2, -1), be_const_var(0) },
        { be_nested_key("member", 719708611, 6, 0), be_const_func(lvx_member) },
    })),
    (be_nested_const_str("lv_disp", 609712084, 8))
);
/*******************************************************************/

/********************************************************************
** Solidified class: lv_font
********************************************************************/
be_local_class(lv_font,
    1,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_func(lvbe_font_create) },
        { be_nested_key("tostring", -1995258651, 8, -1), be_const_func(lvx_tostring) },
        { be_nested_key("_p", 1594591802, 2, -1), be_const_var(0) },
    })),
    (be_nested_const_str("lv_font", 1550958453, 7))
);
/*******************************************************************/

/********************************************************************
** Solidified class: lv_theme
********************************************************************/
be_local_class(lv_theme,
    1,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_func(lvbe_theme_create) },
        { be_nested_key("tostring", -1995258651, 8, -1), be_const_func(lvx_tostring) },
        { be_nested_key("_p", 1594591802, 2, -1), be_const_var(0) },
    })),
    (be_nested_const_str("lv_theme", 1550958453, 7))
);
/*******************************************************************/

/********************************************************************
** Solidified class: lv_color
********************************************************************/
be_local_class(lv_color,
    1,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("toint", -681784387, 5, -1), be_const_func(lco_toint) },
        { be_nested_key("tostring", -1995258651, 8, 0), be_const_func(lco_tostring) },
        { be_nested_key("_p", 1594591802, 2, -1), be_const_var(0) },
        { be_nested_key("init", 380752755, 4, -1), be_const_func(lco_init) },
    })),
    (be_nested_const_str("lv_color", 1419148319, 8))
);
/*******************************************************************/
""")


for subtype, flv in lv.items():
  # special version for widgets
  if subtype in lv_widgets:
    super_class = lv_widget_inheritance.get(subtype, "obj")    # get superclass, default to lv_obj
    print(f"""/********************************************************************
** Solidified class: lv_{subtype}
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_{subtype},
    0,
    &be_class_lv_{super_class},
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {{
        {{ be_nested_key("_class", -1562820946, 6, -1), be_const_comptr(&lv_{subtype}_class) }},
        {{ be_nested_key("init", 380752755, 4, -1), be_const_func(be_ntv_lv_{subtype}_init) }},
    }})),
    (be_nested_const_str("lv_{subtype}", 1612829968, 6))
);
/*******************************************************************/
""")
  # class definitions
  print(f"""void be_load_lv_{subtype}_class(bvm *vm) {{
    be_pushntvclass(vm, &be_class_lv_{subtype});
    be_setglobal(vm, \"lv_{subtype}\");
    be_pop(vm, 1);
}}
""")

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
#include "lv_theme_openhasp.h"

extern int lv0_member(bvm *vm);     // resolve virtual members

extern int lv0_start(bvm *vm);

extern int lv0_register_button_encoder(bvm *vm);  // add buttons with encoder logic

extern int lv0_load_montserrat_font(bvm *vm);
extern int lv0_load_seg7_font(bvm *vm);
extern int lv0_load_robotocondensed_latin1_font(bvm *vm);
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
""")

func_out = {} # used to sort output
for f in lv0:
  c_func_name = f[0]
  c_ret_type = f[1]
  c_argc = f[2]
  if c_argc is not None: c_argc = "\"" + c_argc + "\""
  orig_func_name = f[3]
  be_name = f[4]

  # if c_ret_type is an object, prefix with `lv.`
  if len(c_ret_type) > 1: c_ret_type = "lv." + c_ret_type

  func_out[be_name] = f"  {{ \"{be_name}\", (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'} }},"

for be_name in sorted(func_out):
  print(func_out[be_name])




  # // resolution
  # { "get_hor_res", (void*) &lv_get_hor_res, "i", "" },
  # { "get_ver_res", (void*) &lv_get_ver_res, "i", "" },

  # // layers
  # { "layer_sys", (void*) &lv_layer_sys, "lv_obj", "" },
  # { "layer_top", (void*) &lv_layer_top, "lv_obj", "" },

  # // screens
  # { "scr_act", (void*) &lv_scr_act, "lv_obj", "" },
  # { "scr_load", (void*) &lv_scr_load, "", "(lv_obj)" },
  # { "scr_load_anim", (void*) &lv_scr_load_anim, "", "(lv_obj)iiib" },
print("""
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

# print the enums, symbols and functions
# Ex: { "ALIGN_BOTTOM_LEFT", LV_ALIGN_BOTTOM_LEFT },
#
# Encoding:
# 1. `COLOR_WHITE=0xFFFFFF` enum with explicit value
# 2. `LV_EVENT_ALL` enum with implicit value with same name resolved by C compiler
# 3. `$SYMBOL_OK="\xef\x80\x8c"` if starts with `$` then it's a string
# 4. `&seg7_font=lv0_load_seg7_font` if starts with `&` then it's a native function
#
# We need to sort ignoring the first char if it's not a letter
for k in sorted(lv_module2):
  v = lv_module2[k]
  # check the type of the value
  # if first char is '"' then it's a string and prefix with '$'
  # if first char is '&' then it's a function and prefix with '&'
  # if no value, then it's an enum, prefix with `LV_`
  # otherwise it's an int, leave if unchanged
  if v is not None:
    v_prefix = ""
    if v[0] == '"': v_prefix = "$"
    if v[0] == '&': v_prefix = "&"
    print(f"    {{ \"{v_prefix}{k}\", (int32_t) {v} }},")
  else:
    print(f"    {{ \"{k}\", LV_{k} }},")

print("""
};

const size_t lv0_constants_size = sizeof(lv0_constants)/sizeof(lv0_constants[0]);

/* generated */
be_local_module(lv,
    "lv",
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("member", 719708611, 6, -1), be_const_func(lv0_member) },
        { be_nested_key("start", 1697318111, 5, 0), be_const_func(lv0_start) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(lv);

#endif // USE_LVGL
""")

print("/********************************************************************/")
sys.stdout.close()

