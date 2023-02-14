import re
import sys

lv_widgets_file = "../mapping/lv_funcs.h"
lv_module_file = "../mapping/lv_enum.h"

out_prefix = "../generate/"
lvgl_prefix = "../generate/"

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
parse_arg = re.compile("(\w+\s+)?(\w+)([\*\s]+)(\w+)(\[\])?")

return_types = {
  "void": "",
  "bool": "b",
  "int": "i",
  "uint8_t": "i",
  "uint16_t": "i",
  "int16_t": "i",
  "uint32_t": "i",
  "int32_t": "i",
  "void *": ".",
  "const void *": ".",
  "char *": "c",
  "uint8_t *": "c",
  "const char *": "s",
  "retchar *": "s",
  "constchar *": "s",       # special construct
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
  "lv_draw_mask_res_t": "i",
  "lv_img_size_mode_t": "i",
  "lv_palette_t": "i",
  "lv_style_prop_t": "i",
  "lv_dither_mode_t": "i",
  "lv_chart_update_mode_t": "i",
  "lv_style_res_t": "i",
  # layouts
  "lv_flex_align_t": "i",
  "lv_flex_flow_t": "i",
  "lv_grid_align_t": "i",

  "_lv_event_dsc_t *": "c",
  # lv_anim
  "lv_anim_t *": "lv_anim",
  "lv_anim_enable_t": "i",
  "lv_anim_exec_xcb_t": "c",
  "lv_anim_custom_exec_cb_t": "c",
  "lv_anim_get_value_cb_t": "c",
  "lv_anim_path_cb_t": "c",
  "lv_anim_ready_cb_t": "c",
  "lv_anim_start_cb_t": "c",

  # arrays
  "constchar * []": "str_arr",
  "char * []": "str_arr",
  "lv_coord_t []": "lv_coord_arr",
  "lv_point_t []": "lv_point_arr",

  # "lv_signal_cb_t": "c",
  # "lv_design_cb_t": "c",
  # "lv_event_cb_t": "c",
  # "lv_group_focus_cb_t": "c",

  # ctypes objects
  "lv_area_t *": "lv_area",
  "lv_meter_scale_t *": "lv_meter_scale",
  "lv_meter_indicator_t *": "lv_meter_indicator",
  "lv_obj_class_t *": "lv_obj_class",
  "lv_chart_series_t *": "lv_chart_series",
  "lv_chart_cursor_t *": "lv_chart_cursor",

  "_lv_obj_t *": "lv_obj",
  "lv_obj_t *": "lv_obj",
  "lv_color_t": "lv_color",
  "lv_style_t *": "lv_style",
  "lv_group_t *": "lv_group",
  "lv_font_t *": "lv_font",
  "lv_theme_t *": "lv_theme",
  "lv_disp_t *": "lv_disp",
  "lv_indev_t *": "lv_indev",
  "lv_img_header_t *": "lv_img_header",
  "lv_img_dsc_t *": "lv_img_dsc",
  "lv_ts_calibration_t *": "lv_ts_calibration",
  "lv_style_transition_dsc_t *": "lv_style_transition_dsc",
  "lv_draw_ctx_t *": "lv_draw_ctx",
  "_lv_draw_ctx_t *": "lv_draw_ctx",
  "_lv_draw_layer_ctx_t *": "lv_draw_layer_ctx",
  "lv_grad_dsc_t *": "lv_grad_dsc",
  "lv_sqrt_res_t *": "lv_sqrt_res",
  # "lv_color_hsv_t *": "lv_color_hsv",
  "lv_color_filter_dsc_t *": "lv_color_filter_dsc",
  "lv_timer_t *": "lv_timer",
  "_lv_timer_t *": "lv_timer",
  "lv_coord_t *": "c",      # treat as a simple pointer, decoding needs to be done at Berry level
  "char **": "c",      # treat as a simple pointer, decoding needs to be done at Berry level
  "constchar **": "c",      # treat as a simple pointer, decoding needs to be done at Berry level

  # callbacks
  "lv_group_focus_cb_t": "lv_group_focus_cb",
  "lv_event_cb_t": "lv_event_cb",
  "lv_timer_cb_t": "lv_timer_cb",
}

lv = {}
lvs = []   # special case for case for lv_style
lv0 = []        # function in lvlg module
lv_module = []
lv_cb_types = ['lv_group_focus_cb', 'lv_event_cb', 'lv_timer_cb',
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

# add qrcode
lv_widgets = lv_widgets + [ 'qrcode' ]

lv_prefix = ['obj', 'group', 'style', 'indev', 'disp', 'timer', 'anim'] + lv_widgets

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

# contains any custom attribute we need to add to a widget
lv_widget_custom_ptr = {
  "spinner": {
    "_arc_anim_start_angle": "comptr(&arc_anim_start_angle)",
    "_arc_anim_end_angle": "comptr(&arc_anim_end_angle)",
  }
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
    l_raw = re.sub('const\s+char\s*\*', 'constchar *', l_raw)
    l_raw = re.sub('^char\s*\*', 'retchar *', l_raw)    # special case for returning a char*
    l_raw = re.sub('const ', '', l_raw)
    l_raw = re.sub('struct ', '', l_raw)
    if (len(l_raw) == 0): continue

    g = parse_func_def.search(l_raw)
    if g:
      # print(l_raw, g.group(3))
      # if match, we parse the line
      # Ex: 'void lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent);'
      ret_type = g.group(1)   # return type of the function
      # Ex: ret_type -> 'void'

      ret_type_without_t = re.sub(r"_t$", "", ret_type)
      if ret_type in return_types:
        # convert return type
        c_ret = c_convert_ret_type(ret_type)
      elif ret_type_without_t in lv_cb_types:
        c_ret = "C"   # general callback, if not already captured by explicit type
      else:
        print(f"  // Skipping unsupported return type: {ret_type}")
        continue


      # convert arguments
      c_args = ""
      args_raw = [ x.strip(" \t\n\r") for x in g.group(3).split(",") ]  # split by comma and strip
      # print(args_raw)
      func_name = g.group(2)
      for arg_raw in args_raw:
        # Ex: 'const lv_obj_t * parent' -> 'const ', 'lv_obj_t', ' * ', 'parent', ''
        # Ex: 'bool auto_fit' -> '', 'bool', ' ', 'auto_fit', ''
        # Ex: 'const lv_coord_t value[]' -> 'const', 'lv_coord_t', '', 'value', '[]'
        ga = parse_arg.search(arg_raw)
        # print(f"g={g} ga={ga}")
        if ga or arg_raw == '...':    # parsing ok? Special case for '...' which can't be captured easily in regex
          if arg_raw != '...':
            ga_type = ga.group(2)
            ga_ptr = ( ga.group(3).strip(" \t\n\r") == "*" )    # boolean
            ga_name = ga.group(4)
            ga_array = ga.group(5)
            ga_type_ptr = ga_type
            if ga_ptr: ga_type_ptr += " *"
            if ga_array: ga_type_ptr += " []"
            if ga_type_ptr in return_types:
              ga_type = return_types[ga_type_ptr]
            else:
              # remove the trailing '_t' of type name if any
              ga_type = re.sub(r"_t$", "", ga_type)
            
            # if the type is a single letter, we just add it
            if len(ga_type) == 1 and ga_type != 'C':  # callbacks are different
              c_args += ga_type
            else:
              if ga_type.endswith("_cb"):
                if 'remove_' in func_name:    # if the call is to remove the cb, just treat as an 'anything' parameter
                  c_args += "."
                else:
                  # it's a callback type, we encode it differently
                  if ga_type not in lv_cb_types:
                    lv_cb_types.append(ga_type)
                  c_args += "^" + ga_type + "^"
              else:
                # we have a high-level type that we treat as a class name, enclose in parenthesis
                c_args += "(" + "lv." + ga_type + ")"
          else:
            # '...'
            c_args += "[......]"  # allow 6 additional parameters

      # analyze function name and determine if it needs to be assigned to a specific class
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
#include "be_mapping.h"
""")

for subtype, flv in lv.items():
  print(f"/* `lv_{subtype}` methods */")
  if subtype in lv_widgets:
    print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}")
  print(f"const be_ntv_func_def_t lv_{subtype}_func[] = {{")

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
      func_out[be_name] = f"  {{ \"{be_name}\", {{ (const void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'} }} }},"

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
const be_ntv_class_def_t lv_classes[] = {{""")

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
      c_ret_type = "+_p"  # constructor, init method does not return any value
      if subtype in lv_widgets:
        print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}")
        print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'}); }}")
        print(f"#endif // BE_LV_WIDGET_{subtype.upper()}")
      else:
        print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'}); }}")

print("""
// create font either empty or from parameter on stack
int lvbe_font_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+_p", ""); }
int lvbe_theme_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+_p", ""); }
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

#include "lvgl.h"

extern int lv0_init(bvm *vm);

extern int lco_init(bvm *vm);           // generic function
extern int lco_tostring(bvm *vm);       // generic function
extern int lco_toint(bvm *vm);          // generic function

extern int lv_x_member(bvm *vm);
extern int lv_x_tostring(bvm *vm);       // generic function

extern int lv_be_style_init(bvm *vm);
extern int lv_be_anim_init(bvm *vm);
extern int lv_x_tostring(bvm *vm);

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
#include "be_fixed_be_class_lv_style.h"
/* @const_object_info_begin
class be_class_lv_style (scope: global, name: lv_style, strings: weak) {
    _p, var
    init, func(lv_be_style_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_obj
********************************************************************/
#include "be_fixed_be_class_lv_obj.h"
/* @const_object_info_begin
class be_class_lv_obj (scope: global, name: lv_obj, strings: weak) {
    _p, var
    _class, comptr(&lv_obj_class)
    init, func(be_ntv_lv_obj_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_group
********************************************************************/
#include "be_fixed_be_class_lv_group.h"
/* @const_object_info_begin
class be_class_lv_group (scope: global, name: lv_group, strings: weak) {
    _p, var
    init, func(be_ntv_lv_group_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_indev
********************************************************************/
#include "be_fixed_be_class_lv_indev.h"
/* @const_object_info_begin
class be_class_lv_indev (scope: global, name: lv_indev, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_disp
********************************************************************/
#include "be_fixed_be_class_lv_disp.h"
/* @const_object_info_begin
class be_class_lv_disp (scope: global, name: lv_disp, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_timer
********************************************************************/
#include "be_fixed_be_class_lv_timer.h"
/* @const_object_info_begin
class be_class_lv_timer (scope: global, name: lv_timer, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_anim
********************************************************************/
#include "be_fixed_be_class_lv_anim.h"
/* @const_object_info_begin
class be_class_lv_anim (scope: global, name: lv_anim, strings: weak) {
    _p, var
    init, func(lv_be_anim_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_font
********************************************************************/
#include "be_fixed_be_class_lv_font.h"
/* @const_object_info_begin
class be_class_lv_font (scope: global, name: lv_font, strings: weak) {
    _p, var
    init, func(lvbe_font_create)
    tostring, func(lv_x_tostring)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_theme
********************************************************************/
#include "be_fixed_be_class_lv_theme.h"
/* @const_object_info_begin
class be_class_lv_theme (scope: global, name: lv_theme, strings: weak) {
    _p, var
    init, func(lvbe_theme_create)
    tostring, func(lv_x_tostring)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_color
********************************************************************/
#include "be_fixed_be_class_lv_color.h"
/* @const_object_info_begin
class be_class_lv_color (scope: global, name: lv_color, strings: weak) {
    _p, var
    init, func(lco_init)
    tostring, func(lco_tostring)
    toint, func(lco_toint)
}
@const_object_info_end */

/* define extern function for arc_anim_start_angle and arc_anim_end_angle*/
extern void arc_anim_start_angle(void * obj, int32_t v);
extern void arc_anim_end_angle(void * obj, int32_t v);

""")


for subtype, flv in lv.items():
  # special version for widgets
  if subtype in lv_widgets:
    super_class = lv_widget_inheritance.get(subtype, "obj")    # get superclass, default to lv_obj
    print(f"""/********************************************************************
** Solidified class: lv_{subtype}
********************************************************************/
#include "be_fixed_be_class_lv_{subtype}.h"
/* @const_object_info_begin
class be_class_lv_{subtype} (scope: global, name: lv_{subtype}, super: be_class_lv_{super_class}, strings: weak) {{
    _class, comptr(&lv_{subtype}_class)
    init, func(be_ntv_lv_{subtype}_init)""")

    if subtype in lv_widget_custom_ptr:
      for k, v in lv_widget_custom_ptr[subtype].items():
        print(f"    {k}, {v}")

    print(f"""}}
@const_object_info_end */
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

#include "lvgl.h"
#include "be_mapping.h"
#include "be_ctypes.h"
#include "lv_berry.h"
#include "lv_theme_haspmota.h"

// declare accessors for non-const ints
int32_t be_LV_LAYOUT_GRID(void) { return LV_LAYOUT_GRID; };              BE_VAR_CTYPE_DECLARE(be_LV_LAYOUT_GRID, "i");
int32_t be_LV_LAYOUT_FLEX(void) { return LV_LAYOUT_FLEX; };              BE_VAR_CTYPE_DECLARE(be_LV_LAYOUT_FLEX, "i");

extern int lv0_member(bvm *vm);     // resolve virtual members
extern int lv0_load_font(bvm *vm);

extern lv_ts_calibration_t * lv_get_ts_calibration(void);

static int lv_get_hor_res(void) {
  return lv_disp_get_hor_res(lv_disp_get_default());
}
static int lv_get_ver_res(void) {
  return lv_disp_get_ver_res(lv_disp_get_default());
}

/* `lv` methods */
const be_ntv_func_def_t lv_func[] = {
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

  func_out[be_name] = f"  {{ \"{be_name}\", {{ (const void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'} }} }},"

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

const be_const_member_t lv0_constants[] = {
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
    v_macro = "be_cconst_int"
    if v[0] == '"': v_prefix = "$"; v_macro = "be_cconst_string"
    if v[0] == '&': v_prefix = "&"; v_macro = "be_cconst_ptr"
    if v[0] == '@': v_prefix = "@"; v_macro = "be_cconst_ptr"; v = "&" + v[1:]
    if v[0] == '>': v_prefix = ">"; v_macro = "be_ctype"; v = v[1:]
    print(f"    {{ \"{v_prefix}{k}\", {v_macro}({v}) }},")
  else:
    print(f"    {{ \"{k}\", be_cconst_int(LV_{k}) }},")

print("""
};

const size_t lv0_constants_size = sizeof(lv0_constants)/sizeof(lv0_constants[0]);

#include "../src/solidify/solidified_lv.h"

/* @const_object_info_begin
module lv (scope: global, file: lv) {
    init, closure(lv_module_init_closure)
    member, func(lv0_member)
}
@const_object_info_end */
#include "be_fixed_lv.h"
""")

print("/********************************************************************/")
sys.stdout.close()

