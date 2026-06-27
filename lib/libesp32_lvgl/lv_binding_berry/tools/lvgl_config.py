#!/usr/bin/env python3
"""
Central configuration for the LVGL -> Berry binding generator.

This module is the SINGLE place to adjust when porting the binding to a new
LVGL version. It is imported by both phases of the pipeline:

    preprocessor.py  (phase 1) - which LVGL headers to scan, and what to exclude
    convert.py       (phase 2) - widget/class model, type mapping, synonyms

Nothing here performs any logic; it is pure declarative data. The generation
logic and the (version-independent) be_mapping encoding vocabulary live in the
respective scripts.

Sections
    1. Phase 1 - source scanning (preprocessor.py)
    2. Phase 1 - static prelude content injected into the generated headers
    3. Phase 2 - widget / class model (convert.py)
    4. Phase 2 - C-to-Berry type mapping (convert.py)
"""


# ======================================================================
# 3. Phase 2 - widget / class model (convert.py)
# ======================================================================

# the generated mapping tables.
COMPRESS_LV_TYPE = True

# Standard widgets, in the order they should appear in the generated reference.
# Note: more specific prefixes must come before less specific ones so that the
# longest match wins (e.g. 'scale_section' before 'scale').
LV_WIDGETS = [
    'obj',
    'arc', 'bar', 'button', 'buttonmatrix', 'canvas', 'checkbox',
    'dropdown', 'image', 'label', 'line', 'roller', 'slider',
    'switch', 'table', 'textarea',
    # added in LVGL 9
    'spangroup', 'span',
    'scale_section', 'scale',   # 'scale_section' before 'scale' to match the more selective one first
    'arclabel',                 # 9.5.0
    # extra widgets
    'chart', 'imagebutton', 'led', 'msgbox', 'spinbox', 'spinner',
    'keyboard', 'tabview', 'tileview', 'list',
    'animimg', 'calendar',
    'menu_page', 'menu_cont', 'menu_section', 'menu_separator', 'menu_sidebar_cont',
    'menu_main_cont', 'menu_sidebar_header', 'menu_main_header_cont', 'menu',
    # qrcode
    'qrcode',
    # ad-hoc colorwheel back-ported from LVGL8 to LVGL9
    'colorwheel',
]

# Widgets that do not have a backing ``lv_<name>_class`` C object.
LV_WIDGETS_NO_CLASS = ['span', 'scale_section']

# Non-widget classes that also receive methods, plus all the widgets above.
LV_PREFIX = ['group', 'style', 'indev', 'display', 'timer', 'anim', 'event', 'span'] + LV_WIDGETS

# Widget inheritance, defined explicitly because it is hard to deduce reliably
# from the LVGL source. ``None`` means the widget does not inherit from lv_obj.
LV_WIDGET_INHERITANCE = {
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

# Custom attributes to add to specific widgets (name -> coc member declaration).
LV_WIDGET_CUSTOM_PTR = {
    # "spinner": {
    #   "_arc_anim_start_angle": "comptr(&arc_anim_start_angle)",
    #   "_arc_anim_end_angle": "comptr(&arc_anim_end_angle)",
    # }
}

# Synonyms: map an LVGL 8 (or otherwise legacy) name to its LVGL 9 equivalent.
# For each generated function we also emit aliases for every legacy name that
# resolves to it (see ``get_synonyms``).
SYNONYM_FUNCTIONS = {
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

# Synthetic classes seeded (in this order) before parsing, so they keep a
# stable position at the front of the generated output. These LVGL8-style
# classes have no auto-detected `_create` function.
SYNTHETIC_CLASSES = ['style', 'font', 'color', 'theme']


# ======================================================================
# 4. Phase 2 - C-to-Berry type mapping (convert.py)
# ----------------------------------------------------------------------
# The single-letter codes used as values below are the be_mapping encoding
# (see the `Sig` class in convert.py for what each one means).
# ======================================================================

# Types we silently skip (no warning): mapping them would be too complex,
# so any function using or returning one of these types is dropped.
SKIPPING_TYPES = [
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
    "lv_screen_create_cb_t",    # new in 9.4.0
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
    # new in 9.5.0
    "lv_draw_dsc_base_t *",
    "lv_draw_blur_dsc_t *",
    "lv_indev_key_remap_cb_t",
]

# C type -> Berry encoding. Commented-out entries are kept on purpose to
# document types that were deliberately removed or are handled elsewhere.
RETURN_TYPES = {
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
    # "lv_text_cmd_state_t *": "c",     # not used anymore in 9.4.0
    "lv_font_info_t *": "lv_font_info",
    "lv_switch_orientation_t": "i",
    "lv_slider_orientation_t": "i",
    "lv_draw_letter_dsc_t *": "lv_draw_letter_dsc",
    # new in 9.4.0
    "lv_image_colorkey_t *": "c",
    "lv_arclabel_dir_t": "i",
    "lv_arclabel_text_align_t": "i",
    "lv_anim_timeline_t *": "lv_anim_timeline_dsc",
    # new in 9.5.0
    # "lv_blur_quality_t": "i",
    "lv_arclabel_overflow_t": "i",

    # callbacks
    "lv_group_focus_cb_t": "lv_group_focus_cb",
    "lv_event_cb_t": "lv_event_cb",
    "lv_timer_cb_t": "lv_timer_cb",
}

# Callback type prefixes (without the trailing `_t`) recognized as a generic
# callback when used as a *return* type.
GENERIC_CALLBACK_TYPES = ['lv_group_focus_cb', 'lv_event_cb', 'lv_timer_cb',
                          'lv_constructor_cb',   # addition to LVGL8, also works for 'destructor_cb'
                          'lv_group_edge_cb',    # new in LVGL9
                          ]

# Callback types known to exist as *arguments* from the start, used to seed
# the end-of-run report so it matches historical output.
CALLBACKS_SEEN_SEED = ['lv_group_focus_cb', 'lv_event_cb', 'lv_timer_cb',
                       'lv_constructor_cb',   # 'constructor_cb', also works for 'destructor_cb'
                       ]


# ======================================================================
# 1. Phase 1 - source scanning (preprocessor.py)
# ======================================================================

# Default location of the LVGL source tree (relative to tools/).
LV_SRC_PREFIX = "../../lvgl/src/"

# Header files to skip entirely (matched by suffix).
HEADERS_EXCLUDE_SUFFIX = {
    "_private.h",
    "lv_lottie.h", 
    "lv_obj_property.h",
    "lv_obj_property_names.h",
    "lv_style_properties.h",
    "lv_3dtexture.h",
    "lv_observer.h",
    "lv_ime_pinyin.h",
    "lv_gif.h",
}

# Function-name exclusion patterns (regex). A function whose name matches
# any of these is dropped from the binding.
FUNCTION_EXCLUDE_PATTERNS = [
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
    r".*_bind_.*",                # 9.4.0 remove observer methods
    r".*_get_.*_by_name",         # 9.4.0
    r".*_translation_",           # 9.4.0
    r".*_drop_shadow.*",          # 9.5.0
    r".*_blur_.*",                # 9.5.0
    r".*_set_gesture_.*",         # 9.5.0
]

# Enum-value exclusion prefixes. A constant starting with any of these is
# dropped.
ENUM_EXCLUDE_PREFIXES = {
    "_", "LV_BIDI_DIR_", "LV_FONT_", "LV_SIGNAL_", "LV_TEMPL_", 
    "LV_TASK_PRIO_", "LV_THEME_", "LV_LRU_", "LV_VECTOR_",
    "LV_KEYBOARD_MODE_TEXT_ARABIC", "LV_DRAW_TASK_TYPE_3D", 
    "LV_DRAW_TASK_TYPE_VECTOR",
    "LV_EVENT_TRANSLATION_"
}


# Glob patterns (relative to LV_SRC_PREFIX) for headers scanned for functions.
FUNCTION_HEADER_PATTERNS = [
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

# Extra header files (relative to tools/) appended when present.
FUNCTION_HEADER_EXTRA_PATHS = [
    "../../LVGL_assets/src/lv_theme_haspmota.h",
    "../src/lv_berry.h",
    "../src/lv_colorwheel.h",
]

# Glob patterns (relative to LV_SRC_PREFIX) for headers scanned for enums.
ENUM_HEADER_PATTERNS = [
    "core/*.h",
    "draw/*.h", 
    "hal/*.h",
    "misc/*.h",
    "widgets/*/*.h",
    "display/lv_display.h",
    "layouts/**/*.h",
]


# ======================================================================
# 2. Phase 1 - static prelude content injected into the generated headers
# ----------------------------------------------------------------------
# These large literal blocks are emitted verbatim ahead of the content
# extracted from the LVGL source. The enum prelude in particular carries
# the LVGL 8 -> 9 compatibility aliases, which change between versions.
# ======================================================================

FUNCTIONS_HEADER_PRELUDE = """
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

"""

ENUM_PRELUDE = """// ======================================================================
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
