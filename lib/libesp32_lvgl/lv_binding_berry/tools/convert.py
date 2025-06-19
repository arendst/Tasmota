import re
import sys

lv_widgets_file = "../mapping/lv_funcs.h"
lv_module_file = "../mapping/lv_enum.h"

out_prefix = "../generate/"
lvgl_prefix = "../generate/"
doc_prefix = "../generate/"

be_lv_defines = "be_lv_defines.h"
be_lv_c_mapping = "be_lv_c_mapping.h"
be_lv_widgets_libs = "be_lvgl_widgets_lib.c"
be_lv_lvgl_module = "be_lvgl_module.c"
be_lv_lvgl_doc = "LVGL_API_Reference.md"


lv = {}
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
lv_widgets = ['obj',
              'arc', 'bar', 'button', 'buttonmatrix', 'canvas', 'checkbox',
              'dropdown', 'image', 'label', 'line', 'roller', 'slider',
              'switch', 'table', 'textarea',
              # added in LVGL 9
              'spangroup', 'span',
              'scale_section', 'scale',   # 'scale_section' needs to be before 'scale' to capture more selective first
              ]
lv_widgets_no_class = ['span', 'scale_section']      # widgets that don't have a lv_obj class
# extra widgets
lv_widgets = lv_widgets + [ 'chart', 'imagebutton', 'led', 'msgbox', 'spinbox', 'spinner', 'keyboard', 'tabview', 'tileview' , 'list',
                            'animimg', 'calendar',
                            'menu_page', 'menu_cont', 'menu_section', 'menu_separator', 'menu_sidebar_cont',
                            'menu_main_cont', 'menu_sidebar_header', 'menu_main_header_cont', 'menu']

# add qrcode
lv_widgets = lv_widgets + [ 'qrcode' ]

# adding ad-hoc colorwheel from LVGL8 to LVGL9
lv_widgets = lv_widgets + [ 'colorwheel' ]

lv_prefix = ['group', 'style', 'indev', 'display', 'timer', 'anim', 'event', 'span'] + lv_widgets

# define here widget inheritance because it's hard to deduce from source
lv_widget_inheritance = {
  "animimage": "image",
  "calendar": "buttonmatrix",
  "keyboard": "buttonmatrix",
  "list_button": "button",
  "list_text": "label",
  "spinbox": "textarea",
  "spinner": "arc",             # lv_spinner is a subclass of lv_arc
  "canvas": "image",
  "roller_label": "label",
  "animimg": "image",
  "span": None,
}

# contains any custom attribute we need to add to a widget
lv_widget_custom_ptr = {
  # "spinner": {
  #   "_arc_anim_start_angle": "comptr(&arc_anim_start_angle)",
  #   "_arc_anim_end_angle": "comptr(&arc_anim_end_angle)",
  # }
}


compress_lv_type = True     # if True, convert `lv.lv_<type>` to `lv.<type>` to reduce the size of mapping

class lvgl_function:
  def __init__(self, c_func_name, c_ret_type, c_argc, c_args_doc_list, orig_func_name, be_name):
    # change "lv.lv_" into "lv."
    c_args_doc = ', '.join(c_args_doc_list)
    if compress_lv_type:
      c_argc = re.sub(r"lv\.lv_", "lv.", c_argc)
      c_args_doc = re.sub(r"lv\.lv_", "lv.", c_args_doc)
    self.c_args_doc = c_args_doc

    self.c_func_name = c_func_name
    self.orig_func_name = orig_func_name
    self.be_name = be_name
    if c_ret_type == '.':   c_ret_type = 'c'        # if return type is '.', return a comptr
    self.c_argc = c_argc
    if len(c_ret_type) > 1:
      if compress_lv_type:
        c_ret_type = "lv." + re.sub(r"^lv_", "", c_ret_type)
      else:
        c_ret_type = "lv." + c_ret_type
    self.c_ret_type = c_ret_type
    self.c_ret_type_doc = type_mapper.return_type_to_doc(c_ret_type)

  # return a key/value pair: be_name, C string for this method or function
  def add_C_line_to_map(self, map):
    # if c_ret_type is an object, prefix with `lv.`
    be_ret_type = self.c_ret_type

    map[self.be_name] = f"  {{ \"{self.be_name}\", {{ (const void*) &{self.orig_func_name}, \"{be_ret_type}\", \"{self.c_argc}\" }} }},"

# enter some synonyms so that names in LVGL 8 are mapped to their LVGL 9 equivalent
synonym_functions = {
  # functions
  "scr_act": "screen_active",
  "set_zoom": "set_scale",
  "get_zoom": "get_scale",
  "set_angle": "set_rotation",
  "get_angle": "get_rotation",
  "clear_flag": "remove_flag",
  "clear_state": "remove_state",
  "set_style_img_opa": "set_style_image_opa",
  "set_style_img_recolor": "set_style_image_recolor",
  "set_style_img_recolor_opa": "set_style_image_recolor_opa",
  "set_bg_img_opa": "set_bg_image_opa",
  "set_bg_img_recolor": "set_bg_image_recolor",
  "set_bg_img_recolor_opa": "set_bg_image_recolor_opa",
  "set_bg_img_src": "set_bg_image_src",
  "set_bg_img_tiled": "set_bg_image_tiled",

  "del": "delete",
  "del_async": "delete_async",
  "clear_flag": "remove_flag",
  "clear_state": "remove_state",

  "set_disp": "set_display",
  "get_act": "active",
  "scr_act": "screen_active",
  "scr_load": "screen_load",
  "remove": "delete",
  "set_default": "set_default",
  "get_default": "get_default",
  "get_next": "get_next",
  "set_rotation": "set_rotation",
  "get_hor_res": "get_horizontal_resolution",
  "get_ver_res": "get_vertical_resolution",
  "get_physical_hor_res": "get_physical_horizontal_resolution",
  "get_physical_ver_res": "get_physical_vertical_resolution",
  "get_offset_x": "get_offset_x",
  "get_offset_y": "get_offset_y",
  "get_rotation": "get_rotation",
  "get_dpi": "get_dpi",
  "get_antialiasing": "get_antialiasing",
  "flush_ready": "flush_ready",
  "flush_is_last": "flush_is_last",
  "get_scr_act": "get_screen_active",
  "get_scr_prev": "get_screen_prev",
  "load_scr": "lv_screen_load",
  "get_layer_top": "get_layer_top",
  "get_layer_sys": "get_layer_sys",
  "send_event": "send_event",
  "set_theme": "set_theme",
  "get_theme": "get_theme",
  "get_inactive_time": "get_inactive_time",
  "trig_activity": "trigger_activity",
  "enable_invalidation": "enable_invalidation",
  "is_invalidation_enabled": "is_invalidation_enabled",

  "del_all": "delete_all",
  "set_ready_cb": "set_completed_cb",

  "get_size": "get_size",
  "get_width": "get_width",

  "set_btn_text": "set_button_text",
  "_btn_text": "get_button_text",
  "add_btn": "add_button",

  "get_tab_btns": "get_tab_bar",
  "get_tab_act": "get_tab_active",
  "set_act": "set_active",

  "get_tile_act": "get_tile_active",
  "set_tile_id": "set_tile_by_index",

  "set_visible_row_cnt": "set_visible_row_count",
  "get_option_cnt": "get_option_count",

  "set_col_cnt": "set_column_count",
  "set_row_cnt": "set_row_count",
  "get_col_cnt": "get_column_count",
  "get_row_cnt": "get_row_count",
  "set_col_width": "set_column_width",
  "get_col_width": "get_column_width",

  "get_option_cnt": "get_option_count",

  "get_child_cnt": "get_child_count",
  "get_disp": "get_display",
  "delete_anim_ready_cb": "delete_anim_completed_cb",

  "get_style_anim_time": "get_style_anim_duration",
  "get_style_img_opa": "get_style_image_opa",
  "get_style_img_recolor": "get_style_image_recolor",
  "get_style_img_recolor_filtered": "get_style_image_recolor_filtered",
  "get_style_img_recolor_opa": "get_style_image_recolor_opa",
  "get_style_shadow_ofs_x": "get_style_shadow_offset_x",
  "get_style_shadow_ofs_y": "get_style_shadow_offset_y",
  "get_style_transform_angle": "get_style_transform_rotation",

  "set_style_anim_time": "set_style_anim_duration",
  "set_style_img_opa": "set_style_image_opa",
  "set_style_img_recolor": "set_style_image_recolor",
  "set_style_img_recolor_opa": "set_style_image_recolor_opa",
  "set_style_shadow_ofs_x": "set_style_shadow_offset_x",
  "set_style_shadow_ofs_y": "set_style_shadow_offset_y",
  "set_style_transform_zoom": "set_style_transform_scale",
  "set_style_transform_angle": "set_style_transform_rotation",

  "set_anim_time": "set_anim_duration",
  "set_img_opa": "set_image_opa",
  "set_img_recolor": "set_image_recolor",
  "set_img_recolor_opa": "set_image_recolor_opa",
  "set_shadow_ofs_x": "set_shadow_offset_x",
  "set_shadow_ofs_y": "set_shadow_offset_y",
  "set_transform_angle": "set_transform_rotation",
  "set_transform_zoom": "set_transform_scale",

  "scr_load_anim": "screen_load_anim",

  "set_range": "set_axis_range",
}

def get_synonyms(name):
  return [k for k,v in synonym_functions.items() if v == name]

# The following class takes a C++ type and returns all information about Berry type
class type_mapper_class:
  # detect a function definition all
  # Ex: 'void lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent);'
  # Group 1: 'void'
  # Group 2: 'lv_obj_set_parent'
  # Group 3: 'lv_obj_t * obj, lv_obj_t * parent'
  parse_func_regex = re.compile(r"(.*?)\s(\w+)\((.*?)\)")
  
  # parse call argument type
  # Ex: 'const lv_obj_t * parent' -> 'const ', 'lv_obj_t', ' * ', 'parent'
  # Ex: 'bool auto_fit' -> '', 'bool', ' ', 'auto_fit'
  parse_arg_regex = re.compile(r"(\w+\s+)?(\w+)([\*\s]+)(\w+)(\[\])?")

  # the following types are skipped without warning, because it would be too complex to adapt (so we don't map any function using or returning these types)
  skipping_type = [
    "bvm *",                      # Berry
    "lv_global_t *",              # reading globals is not useful in Berry
    # "lv_event_dsc_t *",           # internal implementation, use functions instead
    "lv_draw_task_t *",           # skip low-level tasks for now
    "lv_draw_buf_t *",            # low-level
    "lv_calendar_date_t *",       # skip calendar for now
    "lv_vector_dsc_t",            # see later if we need this
    "lv_point_precise_t",         # see later if we need this
    "void **",                    # edge case of lv_animimg_get_src()
    "va_list",
    "lv_matrix_t *",
    "lv_event_list_t *",
    "lv_style_value_t *",
    "lv_fpoint_t *",
    "lv_draw_fill_dsc_t *",
    "lv_draw_mask_rect_dsc_t *",
    "lv_draw_triangle_dsc_t *",
    "lv_font_glyph_dsc_t *",
    "lv_buttonmatrix_ctrl_t []",
    "lv_group_edge_cb_t",
    "lv_obj_tree_walk_cb_t",
    "lv_display_flush_cb_t",
    "lv_display_flush_wait_cb_t",
    "lv_draw_glyph_dsc_t *",
    "lv_draw_unit_t *",
    "lv_draw_border_dsc_t *",
    "lv_draw_box_shadow_dsc_t *",
    "lv_anim_deleted_cb_t",
    "lv_timer_handler_resume_cb_t",
    "lv_theme_apply_cb_t",
    "lv_color32_t *",
    "lv_color16_t *",
    "lv_color_filter_cb_t",
    "lv_style_prop_t []",
    "lv_calendar_date_t []",
    "lv_indev_read_cb_t",
    "lv_vector_path_t *",
    "lv_vector_path_quality_t",
    "lv_color16_t",
    "uint8_t *",
    "lv_obj_t **",
  ]

  return_types = {
    # "void": "",           # treated in code directly, it doesn't work well with regex since there is no variable name
    "bool": "b",
    "int": "i",
    "uint8_t": "i",
    "uint16_t": "i",
    "int16_t": "i",
    "uint32_t": "i",
    "int32_t": "i",
    "void *": ".",
    # "const void *": ".",
    "char *": "c",
    # "uint8_t *": "c",
    "size_t": "i",
    # "const char *": "s",
    "retchar *": "s",
    "constchar *": "s",       # special construct
    # "lv_obj_user_data_t": "i",
    "lv_result_t": "i",
    # "float": "f",

    "lv_coord_t": "i",
    "lv_opa_t": "i",
    "lv_state_t": "i",
    "lv_chart_axis_t":"i",
    "lv_point_t":"i",
    "lv_chart_type_t":"i",
    # "lv_layout_t":"i",
    "lv_color_hsv_t":"i",
    "lv_label_long_mode_t": "i",
    "lv_scrollbar_mode_t": "i",
    "lv_blend_mode_t": "i",
    "lv_grad_dir_t": "i",
    "lv_border_side_t": "i",
    "lv_align_t": "i",
    "lv_keyboard_mode_t": "i",
    "lv_indev_type_t": "i",
    "lv_indev_mode_t": "i",
    "lv_indev_state_t": "i",
    # "lv_event_t": "i",
    "lv_dir_t": "i",
    "lv_part_t": "i",
    "lv_base_dir_t": "i",
    "lv_text_decor_t": "i",
    "lv_text_align_t": "i",
    "lv_arc_mode_t": "i",
    "lv_bar_mode_t": "i",
    "lv_bar_orientation_t": "i",
    "lv_event_code_t": "i",
    "lv_obj_flag_t": "i",
    "lv_slider_mode_t": "i",
    "lv_scroll_snap_t": "i",
    "lv_style_value_t": "i",
    # "lv_image_src_t": "i",
    "lv_style_selector_t": "i",
    # "lv_palette_t": "i",
    "lv_style_prop_t": "i",
    "lv_chart_update_mode_t": "i",
    "lv_style_res_t": "i",
    # LVGL 9
    "lv_image_align_t": "i",
    "lv_text_flag_t": "i",
    "lv_display_rotation_t": "i",
    "lv_color_format_t": "i",
    "lv_value_precise_t": "i",
    "lv_color32_t": "i",
    "lv_scale_mode_t": "i",
    "lv_span_overflow_t": "i",
    "lv_span_mode_t": "i",
    # "lv_vector_path_t *": "c",    # treat as opaque pointer
    # "lv_vector_dsc_t *": "c",     # treat as opaque pointer
    "lv_point_t *": "c",          # treat as opaque pointer
    "lv_hit_test_info_t *": "c",  # treat as opaque pointer
    "lv_screen_load_anim_t": "i",
    "lv_display_render_mode_t": "i",
    "lv_draw_task_type_t": "i",
    # "lv_vector_gradient_spread_t": "i",
    "lv_cover_res_t": "i",
    # "lv_vector_path_quality_t": "i",
    # "lv_vector_blend_t": "i",
    # "lv_vector_fill_t": "i",
    # "lv_vector_stroke_cap_t": "i",
    # "lv_vector_stroke_join_t": "i",
    "lv_font_kerning_t": "i",
    "lv_menu_mode_header_t": "i",
    "lv_menu_mode_root_back_button_t": "i",
    "lv_point_precise_t []": "lv_point_arr",
    "lv_obj_point_transform_flag_t": "i",
    "lv_palette_t": "i",

    "int32_t *": "lv_int_arr",
    "int32_t []": "lv_int_arr",
    "uint32_t *": "lv_int_arr",
    "lv_color_t *": "lv_color_arr",
    # "float *": "lv_float_arr",
    # layouts
    "lv_flex_align_t": "i",
    "lv_flex_flow_t": "i",
    "lv_grid_align_t": "i",

    # lv_anim
    "lv_anim_t *": "lv_anim",
    "lv_anim_enable_t": "i",
    "lv_anim_exec_xcb_t": "c",
    "lv_anim_custom_exec_cb_t": "c",
    "lv_anim_get_value_cb_t": "c",
    "lv_anim_path_cb_t": "c",
    "lv_anim_completed_cb_t": "c",
    "lv_anim_start_cb_t": "c",
    # lv_imagebutton
    "lv_imagebutton_state_t": "i",
    # lv_buttonmatrix
    "lv_buttonmatrix_ctrl_t": "i",

    "lv_group_refocus_policy_t": "i",
    "lv_roller_mode_t": "i",
    "lv_table_cell_ctrl_t": "i",

    # "lv_calendar_chinese_t": "c",

    # adding ad-hoc colorwheel from LVGL8 to LVGL9
    "lv_colorwheel_mode_t": "i",

    # arrays
    "constchar * []": "str_arr",
    # "char * []": "str_arr",
    # "lv_coord_t []": "lv_coord_arr",
    # "lv_point_t []": "lv_point_arr",

    # ctypes objects
    "lv_area_t *": "lv_area",
    "lv_obj_class_t *": "lv_obj_class",
    "lv_chart_series_t *": "lv_chart_series",
    "lv_chart_cursor_t *": "lv_chart_cursor",
    "lv_draw_label_dsc_t *": "lv_draw_label_dsc",
    "lv_draw_rect_dsc_t *": "lv_draw_rect_dsc",
    "lv_draw_line_dsc_t *": "lv_draw_line_dsc",
    "lv_draw_arc_dsc_t *": "lv_draw_arc_dsc",
    "lv_point_precise_t *": "lv_point_precise",
    "lv_draw_image_dsc_t *": "lv_draw_image_dsc",
    "lv_event_dsc_t *": "lv_event_dsc",
    "lv_span_coords_t": "lv_span_coords",

    "_lv_obj_t *": "lv_obj",
    "lv_obj_t *": "lv_obj",
    "lv_event_t *": "lv_event",
    "lv_color_t": "lv_color",
    "lv_style_t *": "lv_style",
    "lv_group_t *": "lv_group",
    "lv_font_t *": "lv_font",
    "lv_theme_t *": "lv_theme",
    "lv_display_t *": "lv_display",
    '_lv_display_t *': "lv_display",
    "lv_indev_t *": "lv_indev",
    "lv_point_t []": "lv_point_arr",
    "lv_span_t *": "lv_span",
    "lv_scale_section_t *": "lv_scale_section",  # treat as opaque pointer
    # "lv_image_header_t *": "lv_image_header",
    "lv_image_dsc_t *": "lv_image_dsc",
    "lv_ts_calibration_t *": "lv_ts_calibration",
    "lv_style_transition_dsc_t *": "lv_style_transition_dsc",
    "lv_layer_t *": "lv_layer",               # LVGL9
    # "_lv_draw_layer_ctx_t *": "lv_draw_layer_ctx",
    "lv_grad_dsc_t *": "lv_grad_dsc",
    "lv_color_filter_dsc_t *": "lv_color_filter_dsc",
    "lv_timer_t *": "lv_timer",
    # "_lv_timer_t *": "lv_timer",
    # "lv_coord_t *": "lv_coord_arr",
    # "char **": "lv_str_arr",           # treat as a simple pointer, decoding needs to be done at Berry level
    "constchar **": "c",      # treat as a simple pointer, decoding needs to be done at Berry level
    "void * []": "c",         # treat as a simple pointer, decoding needs to be done at Berry level
    "constchar * *": "c",
    # new in 9.3.0
    "lv_text_cmd_state_t *": "c",
    "lv_font_info_t *": "lv_font_info",
    "lv_switch_orientation_t": "i",
    "lv_slider_orientation_t": "i",
    "lv_draw_letter_dsc_t *": "lv_draw_letter_dsc",

    # callbacks
    "lv_group_focus_cb_t": "lv_group_focus_cb",
    "lv_event_cb_t": "lv_event_cb",
    "lv_timer_cb_t": "lv_timer_cb",
  }

  lv_cb_types = ['lv_group_focus_cb', 'lv_event_cb', 'lv_timer_cb',
                'lv_constructor_cb',                # 'constructor_cb', addition to LVGL8, also works for 'destructor_cb'
                'lv_group_edge_cb',                 # new in LVGL9
                ]

  # how to transform a type used for Berry mapping into user-explicit
  return_types_for_doc = {
    "i": "int",
    "b": "bool",
    "s": "string",
    "c": "comptr",
    "C": "callback",
    ".": "\\<any\\>",
  }

  def return_type_to_doc(self, type):
    return type_mapper_class.return_types_for_doc.get(type, type)

  def __init__(self) -> None:
    # create a map to count each time a C type is converted to Berry, and catch those that are never used
    # also provide some stats
    self.return_types_count = { k:0 for k in type_mapper_class.return_types.keys() }
    self.unhandled_types = {}

  # convert a C return type to the string representation for the Berry parser
  # using return_types
  def c_convert_type(self, c_ctype, try_without_t):
    # try name first
    be_ret = type_mapper_class.return_types.get(c_ctype)
    if be_ret:
      self.return_types_count[c_ctype] += 1
      return be_ret
  
    if try_without_t:
      c_ctype_without_t = re.sub(r"^(\w+?)_t", "\\1", c_ctype)
      # print(f">>> ctype '{c_ctype}' '{c_ctype_without_t}'")
      be_ret = type_mapper_class.return_types.get(c_ctype_without_t)
      if be_ret:
        self.return_types_count[c_ctype_without_t] += 1
        return be_ret
    
    return None     # fallback
  
  def type_is_unhandled(self, t):
    self.unhandled_types[t] = self.unhandled_types.get(t, 0) + 1

  def dump_return_types_stats(self, dump_all):
    for k in sorted(self.unhandled_types, key=self.unhandled_types.get, reverse=True):
      print(f"> Unhandled type '{k}' ({self.unhandled_types[k]})", file=sys.stderr)
    if dump_all:
      for k in sorted(self.return_types_count, key=self.return_types_count.get, reverse=True):
        print(f"# '{k}': {self.return_types_count[k]}", file=sys.stderr)

    else:
      # dump only unused
      return_types_count_filtered = [k for k,v in self.return_types_count.items() if v == 0]
      for k in return_types_count_filtered:
        print(f"# mapping not used '{k}'", file=sys.stderr)

  def clean_c_line(self, l_raw):
    l_raw = re.sub(r'//.*$', '', l_raw)                  # remove trailing comments
    l_raw = re.sub(r'LV_ATTRIBUTE_FAST_MEM ', '', l_raw) # remove LV_ATTRIBUTE_FAST_MEM marker
    l_raw = re.sub(r'\s+', ' ', l_raw)                   # replace any multi-space with a single space
    l_raw = l_raw.strip(" \t\n\r")                      # remove leading or trailing spaces
    l_raw = re.sub(r'static ', '', l_raw)                # remove `static` qualifier
    l_raw = re.sub(r'inline ', '', l_raw)                # remove `inline` qualifier
    l_raw = re.sub(r'const\s+char\s*\*', 'constchar *', l_raw)
    l_raw = re.sub(r'^char\s*\*', 'retchar *', l_raw)    # special case for returning a char*
    l_raw = re.sub(r'const ', '', l_raw)
    l_raw = re.sub(r'struct ', '', l_raw)
    return l_raw

  def parse_c_line(self, l_raw):
    g = type_mapper_class.parse_func_regex.search(l_raw)
    if g:
      c_return_type = g.group(1)
      c_func_name = g.group(2)
      c_arg_types = g.group(3)
      return (c_return_type, c_func_name, c_arg_types)
    else:
      return (None, None, None)

  def get_widget_return_type(self, c_type):
    c_ret = self.c_convert_type(c_type, False)
    c_ctype_without_t = re.sub(r"_t$", "", c_type)
    if c_ret:
      pass      # done
    elif c_type == "void":
      c_ret = ""
    elif c_ctype_without_t in type_mapper_class.lv_cb_types:
      c_ret = "C"   # general callback, if not already captured by explicit type
    elif c_type not in type_mapper_class.skipping_type:
      print(f"  // Skipping unsupported return type '{c_type}'", file=sys.stderr)
      self.type_is_unhandled(c_type)
      c_ret = None

    # print(f">>> get_widget_return_type '{c_type}' -> '{c_ret}'", file=sys.stderr)
    return c_ret
  
  def get_widget_arg_type(self, c_return_type, c_func_name, c_arg_types):
    # convert arguments
    c_args = ""
    c_args_doc = []
    args_raw = [ x.strip(" \t\n\r") for x in c_arg_types.split(",") ]  # split by comma and strip
    # print(args_raw)
    for arg_raw in args_raw:

      # check if `void`
      if arg_raw == 'void':      # Special case for 'void' which can't be captured easily in regex
        break

      if arg_raw == '...':          # Special case for '...' which can't be captured easily in regex
        # '...'
        c_args += "[......]"  # allow 6 additional parameters by default
        c_args_doc.append("[\\<any\\>]")
        break
      
      # Ex: 'const lv_obj_t * parent' -> 'const ', 'lv_obj_t', ' * ', 'parent', ''
      # Ex: 'bool auto_fit' -> '', 'bool', ' ', 'auto_fit', ''
      # Ex: 'const lv_coord_t value[]' -> 'const', 'lv_coord_t', '', 'value', '[]'
      ga = type_mapper_class.parse_arg_regex.search(arg_raw)
      if ga:                      # parsing ok?
        # ga.group(1) is first modifier like `const`, we don't use them
        ga_type = ga.group(2)     # main type
        ga_ptr = ( ga.group(3).strip(" \t\n\r") == "*" )      # (bool) is it a pointer
        ga_2ptr = ( ga.group(3).strip(" \t\n\r") == "**" )    # (bool) is it a pointer to a pointer
        ga_name = ga.group(4)     # main type name
        ga_array = ga.group(5)    # is it an array `[]`
        # print(f"raw='{arg_raw}' {ga_type=} {ga_ptr=} {ga_name=} {ga_array=}")

        ga_full_type = ga_type    # rebuild a complete type with modifier
        if ga_ptr:    ga_full_type += " *"
        if ga_2ptr:   ga_full_type += " **"
        if ga_array:  ga_full_type += " []"

        be_type = self.c_convert_type(ga_full_type, True)

        if be_type == None:
          # no match, unsupported type
          if ga_full_type not in type_mapper_class.skipping_type:
            self.type_is_unhandled(ga_full_type)
          # print(f"  // Unsupported argument type '{arg_raw}'", file=sys.stderr)
          return (None, None)

        # if the type is a single letter, we just add it
        if len(be_type) == 1 and be_type != 'C':  # callbacks are different
          c_args += be_type
          c_args_doc.append(type_mapper.return_type_to_doc(be_type))
        else:
          if be_type.endswith("_cb"):
            if 'remove_' in c_func_name:    # if the call is to remove the cb, just treat as an 'anything' parameter
              c_args += "."
              c_args_doc.append("\\<any\\>")
            else:
              # it's a callback type, we encode it differently
              if be_type not in lv_cb_types:
                lv_cb_types.append(be_type)
              c_args += "^" + be_type + "^"
              c_args_doc.append('\\<closure\\>')
          else:
            # we have a high-level type that we treat as a class name, enclose in parenthesis
            c_args += "(" + "lv." + be_type + ")"
            c_args_doc.append("lv." + be_type)
      else:
        print(f"  // unable to parse arguments '{arg_raw}'", file=sys.stderr)
    
    # print(f">>> get_widget_arg_type '{c_arg_types}' -> '{c_args}'", file=sys.stderr)
    return (c_args, c_args_doc)

type_mapper = type_mapper_class()

def try_int(s):
  try:
    v = int(s)
    return v
  except ValueError:
    return None

# parse widgets files containing most function calls
with open(lv_widgets_file) as f:
  for l_raw in f:
    l_raw = type_mapper.clean_c_line(l_raw)
    if (len(l_raw) == 0): continue

    (c_return_type, c_func_name, c_arg_types) = type_mapper.parse_c_line(l_raw)
    if (c_return_type == None): continue

    c_ret = type_mapper.get_widget_return_type(c_return_type)
    if c_ret == None:   continue      # skip if nothing to return

    # convert arguments
    (c_args, c_args_doc_list) = type_mapper.get_widget_arg_type(c_return_type, c_func_name, c_arg_types)
    if c_args == None: continue      # skip if nothing to return

    # analyze function name and determine if it needs to be assigned to a specific class
    # Ex: c_func_name -> 'lv_obj_set_parent'
    if c_func_name.startswith("_"): continue            # skip low-level
    if c_func_name.startswith("lv_debug_"): continue    # skip debug

    be_func_name = ""

    if c_func_name == "lv_style_init":
      continue   # no need for init as it would collied with native init (and called behind the scene anyways)

    found = False
    for subtype in lv_prefix:
      if c_func_name.startswith("lv_" + subtype + "_"):
        be_name = re.sub("^lv_" + subtype + "_", '', c_func_name)
        be_func_name = "lvbe_" + subtype + "_" + be_name
        if subtype not in lv: lv[subtype] = []    # add entry
        is_right_type = c_args.startswith(f"(lv.lv_{subtype})")   # check if first arg matches class
        is_obj_arg1 = c_args.startswith("(lv.lv_obj)")            # or first arg is lv_obj
        is_group_create = (subtype == 'group') and (c_func_name == 'lv_group_create')
        if is_right_type or is_obj_arg1 or is_group_create:
          # remove first argument which is implicit
          c_args_doc_list = c_args_doc_list[1:]
          lv[subtype].append(lvgl_function(be_func_name, c_ret, c_args, c_args_doc_list, c_func_name, be_name))
          # add synonyms
          syn = get_synonyms(be_name)
          for n in syn: lv[subtype].append(lvgl_function(be_func_name, c_ret, c_args, c_args_doc_list, c_func_name, n))

          found = True
          break

    if found: continue
    # not found, we treat it as lv top level function
    be_name = re.sub("^lv_", '', c_func_name)
    lv0.append(lvgl_function(be_func_name, c_ret, c_args, c_args_doc_list, c_func_name, be_name))
    # add synonyms
    syn = get_synonyms(be_name)
    for n in syn: lv0.append(lvgl_function(be_func_name, c_ret, c_args, c_args_doc_list, c_func_name, n))

print("| callback types"+str(lv_cb_types), file=sys.stderr)

# parse module file containing numerical constants
with open(lv_module_file) as f:
  for l_raw in f:
    l_raw = l_raw.strip(" \t\n\r")    # remove leading or trailing spaces
    if l_raw.startswith("//"):
      lv_module.append( [ None, l_raw ] )   # if key in None then add comment line
    l_raw = re.sub(r'//.*$', '', l_raw) # remove trailing comments
    l_raw = re.sub(r'\s+', '', l_raw) # remove all spaces
    l_raw = re.sub(r',.*$', '', l_raw) # remove comma and anything after it
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


# documentation in Markdown for `lv` module
sys.stdout = open(doc_prefix + be_lv_lvgl_doc, 'w')
print("""# LVGL Berry API Reference

[//]: # (**********************************************************************)
[//]: # (* Generated code, don't edit                                         *)
[//]: # (**********************************************************************)

""")

# Start with `lv` module
print("## module `lv`") 
print()
print("Method|Arguments|Return type|LVGL equivalent")
print(":---|:---|:---|:---")

for f in sorted(lv0, key=lambda x: x.be_name):
  print(f"{f.be_name}|{f.c_args_doc}|{f.c_ret_type_doc}|[{f.orig_func_name}](https://docs.lvgl.io/9.0/search.html?q={f.orig_func_name})")

print()

# Continue with core classes
print("## Core classes") 
print()

for subtype, flv in sorted(lv.items()):
  if subtype not in lv_widgets and len(flv) > 0:
    print(f"### class `lv.{subtype}`")
    print()
    print("Method|Arguments|Return type|LVGL equivalent")
    print(":---|:---|:---|:---")
    for f in sorted(flv, key=lambda x: x.be_name):
      if f.c_func_name.endswith("_create"):
        pass
        # skip create function that are handled in object constructor (no `create` method)
      else:
        print(f"{f.be_name}|{f.c_args_doc}|{f.c_ret_type_doc}|[{f.orig_func_name}](https://docs.lvgl.io/9.0/search.html?q={f.orig_func_name})")
    print()

# Continue with Widgets
print("## Widgets") 
print()
for subtype in lv_widgets:
  flv = lv.get(subtype)
  if flv:
    print(f"### widget `lv.{subtype}`")
    print()
    print("Method|Arguments|Return type|LVGL equivalent")
    print(":---|:---|:---|:---")
    for f in sorted(flv, key=lambda x: x.be_name):
      if f.c_func_name.endswith("_create"):
        pass
        # skip create function that are handled in object constructor (no `create` method)
      else:
        print(f"{f.be_name}|{f.c_args_doc}|{f.c_ret_type_doc}|[{f.orig_func_name}](https://docs.lvgl.io/9.0/search.html?q={f.orig_func_name})")
    print()


sys.stdout.close()
      
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
#include "../src/lv_colorwheel.h"
""")

for subtype, flv in lv.items():
  print(f"/* `lv_{subtype}` methods */")
  if subtype in lv_widgets:
    print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}")
  print(f"const be_ntv_func_def_t lv_{subtype}_func[] = {{")

  func_out = {} # used to sort output
  for f in sorted(flv, key=lambda x: x.be_name):

    if f.c_func_name.endswith("_create"):
      pass
      # skip create function that are handled in object constructor (no `create` method)
    else:
      f.add_C_line_to_map(func_out)

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
  create_found = False        # does the method contains an explicit `_create()` method
  for f in sorted(flv, key=lambda x: x.be_name):
    if f.c_func_name.endswith("_create"):
      create_found = True
      c_ret_type = "+_p"  # constructor, init method does not return any value
      if subtype in lv_widgets:
        print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}")
        print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{f.orig_func_name}, \"+_p\", \"{f.c_argc}\"); }}")
        print(f"#endif // BE_LV_WIDGET_{subtype.upper()}")
      else:
        print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{f.orig_func_name}, \"+_p\", \"{f.c_argc}\"); }}")

  if not create_found and subtype in lv_widgets:
    # there is no explicit create, add one
    print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}")
    print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{f.orig_func_name}, \"+_p\", \"{f.c_argc}\"); }}")
    print(f"#endif // BE_LV_WIDGET_{subtype.upper()}")


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
extern int lv_be_style_del(bvm *vm);
extern int lv_be_anim_init(bvm *vm);
extern int lv_x_tostring(bvm *vm);

BE_EXPORT_VARIABLE extern const bclass be_class_lv_obj;

extern int lvbe_font_create(bvm *vm);
extern int lvbe_theme_create(bvm *vm);

// adding ad-hoc colorwheel from LVGL8 to LVGL9
extern const lv_obj_class_t lv_colorwheel_class;

""")

# expose all extern definitions:
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
    del, func(lv_be_style_del)
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
** Solidified class: lv_display
********************************************************************/
#include "be_fixed_be_class_lv_display.h"
/* @const_object_info_begin
class be_class_lv_display (scope: global, name: lv_display, strings: weak) {
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
** Solidified class: lv_event
********************************************************************/
#include "be_fixed_be_class_lv_event.h"
/* @const_object_info_begin
class be_class_lv_event (scope: global, name: lv_event, strings: weak) {
    _p, var
    init, func(lv0_init)
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
  if subtype == 'obj':  continue    # 'obj' has a special implementation
  # special version for widgets
  if subtype in lv_widgets:
    super_class = lv_widget_inheritance.get(subtype, "obj")    # get superclass, default to lv_obj
    print(f"""/********************************************************************
** Solidified class: lv_{subtype}
********************************************************************/
#include "be_fixed_be_class_lv_{subtype}.h"
/* @const_object_info_begin""")
    if super_class is not None:
      print(f"class be_class_lv_{subtype} (scope: global, name: lv_{subtype}, super: be_class_lv_{super_class}, strings: weak) {{")
    else:
      print(f"class be_class_lv_{subtype} (scope: global, name: lv_{subtype}, strings: weak) {{")
      print(f"    _p, var")
      print(f"    tostring, func(lv_x_tostring)")
      print(f"    member, func(lv_x_member)")
    print(f"    init, func(be_ntv_lv_{subtype}_init)")

    if subtype not in lv_widgets_no_class:
      print(f"    _class, comptr(&lv_{subtype}_class)")
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
  return lv_display_get_horizontal_resolution(NULL);
}
static int lv_get_ver_res(void) {
  return lv_display_get_vertical_resolution(NULL);
}

/* `lv` methods */
const be_ntv_func_def_t lv_func[] = {
""")

func_out = {} # used to sort output
for f in lv0:
  f.add_C_line_to_map(func_out)
  # c_func_name = f[0]
  # c_ret_type = f[1]
  # c_argc = f[2]
  # if c_argc is not None: c_argc = "\"" + c_argc + "\""
  # orig_func_name = f[3]
  # be_name = f[4]

  # # if c_ret_type is an object, prefix with `lv.`
  # # if len(c_ret_type) > 1: c_ret_type = "lv." + c_ret_type
  # if len(c_ret_type) > 1: c_ret_type = "lv." + c_ret_type

  # func_out[be_name] = f"  {{ \"{be_name}\", {{ (const void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'} }} }},"


  # # any synonyms?
  # f_synonyms = get_synonyms(be_name)
  # if f_synonyms:
  #   for new_be_name in f_synonyms:
  #       func_out[new_be_name] = f"  {{ \"{new_be_name}\", {{ (const void*) &{orig_func_name}, \"{c_ret_type}\", { c_argc if c_argc else 'nullptr'} }} }},"


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

type_mapper.dump_return_types_stats(False)    # True would dump all stats, False only unused
