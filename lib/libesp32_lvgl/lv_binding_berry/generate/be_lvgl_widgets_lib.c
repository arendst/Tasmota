
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


extern int be_ntv_lv_style_init(bvm *vm);
extern int be_ntv_lv_font_init(bvm *vm);
extern int be_ntv_lv_color_init(bvm *vm);
extern int be_ntv_lv_theme_init(bvm *vm);
extern int be_ntv_lv_group_init(bvm *vm);
extern int be_ntv_lv_obj_init(bvm *vm);
extern int be_ntv_lv_event_init(bvm *vm);
extern int be_ntv_lv_display_init(bvm *vm);
extern int be_ntv_lv_indev_init(bvm *vm);
extern int be_ntv_lv_qrcode_init(bvm *vm);
extern int be_ntv_lv_anim_init(bvm *vm);
extern int be_ntv_lv_timer_init(bvm *vm);
extern int be_ntv_lv_animimg_init(bvm *vm);
extern int be_ntv_lv_arc_init(bvm *vm);
extern int be_ntv_lv_bar_init(bvm *vm);
extern int be_ntv_lv_button_init(bvm *vm);
extern int be_ntv_lv_buttonmatrix_init(bvm *vm);
extern int be_ntv_lv_calendar_init(bvm *vm);
extern int be_ntv_lv_canvas_init(bvm *vm);
extern int be_ntv_lv_chart_init(bvm *vm);
extern int be_ntv_lv_checkbox_init(bvm *vm);
extern int be_ntv_lv_dropdown_init(bvm *vm);
extern int be_ntv_lv_image_init(bvm *vm);
extern int be_ntv_lv_imagebutton_init(bvm *vm);
extern int be_ntv_lv_keyboard_init(bvm *vm);
extern int be_ntv_lv_label_init(bvm *vm);
extern int be_ntv_lv_led_init(bvm *vm);
extern int be_ntv_lv_line_init(bvm *vm);
extern int be_ntv_lv_list_init(bvm *vm);
extern int be_ntv_lv_menu_init(bvm *vm);
extern int be_ntv_lv_menu_page_init(bvm *vm);
extern int be_ntv_lv_menu_cont_init(bvm *vm);
extern int be_ntv_lv_menu_section_init(bvm *vm);
extern int be_ntv_lv_menu_separator_init(bvm *vm);
extern int be_ntv_lv_msgbox_init(bvm *vm);
extern int be_ntv_lv_roller_init(bvm *vm);
extern int be_ntv_lv_scale_init(bvm *vm);
extern int be_ntv_lv_scale_section_init(bvm *vm);
extern int be_ntv_lv_slider_init(bvm *vm);
extern int be_ntv_lv_span_init(bvm *vm);
extern int be_ntv_lv_spangroup_init(bvm *vm);
extern int be_ntv_lv_spinbox_init(bvm *vm);
extern int be_ntv_lv_spinner_init(bvm *vm);
extern int be_ntv_lv_switch_init(bvm *vm);
extern int be_ntv_lv_table_init(bvm *vm);
extern int be_ntv_lv_tabview_init(bvm *vm);
extern int be_ntv_lv_textarea_init(bvm *vm);
extern int be_ntv_lv_tileview_init(bvm *vm);
extern int be_ntv_lv_colorwheel_init(bvm *vm);

extern const bclass be_class_lv_anim;
extern const bclass be_class_lv_animimg;
extern const bclass be_class_lv_arc;
extern const bclass be_class_lv_bar;
extern const bclass be_class_lv_button;
extern const bclass be_class_lv_buttonmatrix;
extern const bclass be_class_lv_calendar;
extern const bclass be_class_lv_canvas;
extern const bclass be_class_lv_chart;
extern const bclass be_class_lv_checkbox;
extern const bclass be_class_lv_color;
extern const bclass be_class_lv_colorwheel;
extern const bclass be_class_lv_display;
extern const bclass be_class_lv_dropdown;
extern const bclass be_class_lv_event;
extern const bclass be_class_lv_font;
extern const bclass be_class_lv_group;
extern const bclass be_class_lv_image;
extern const bclass be_class_lv_imagebutton;
extern const bclass be_class_lv_indev;
extern const bclass be_class_lv_keyboard;
extern const bclass be_class_lv_label;
extern const bclass be_class_lv_led;
extern const bclass be_class_lv_line;
extern const bclass be_class_lv_list;
extern const bclass be_class_lv_menu;
extern const bclass be_class_lv_menu_cont;
extern const bclass be_class_lv_menu_page;
extern const bclass be_class_lv_menu_section;
extern const bclass be_class_lv_menu_separator;
extern const bclass be_class_lv_msgbox;
extern const bclass be_class_lv_obj;
extern const bclass be_class_lv_qrcode;
extern const bclass be_class_lv_roller;
extern const bclass be_class_lv_scale;
extern const bclass be_class_lv_scale_section;
extern const bclass be_class_lv_slider;
extern const bclass be_class_lv_span;
extern const bclass be_class_lv_spangroup;
extern const bclass be_class_lv_spinbox;
extern const bclass be_class_lv_spinner;
extern const bclass be_class_lv_style;
extern const bclass be_class_lv_switch;
extern const bclass be_class_lv_table;
extern const bclass be_class_lv_tabview;
extern const bclass be_class_lv_textarea;
extern const bclass be_class_lv_theme;
extern const bclass be_class_lv_tileview;
extern const bclass be_class_lv_timer;


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


/********************************************************************
** Solidified class: lv_qrcode
********************************************************************/
#include "be_fixed_be_class_lv_qrcode.h"
/* @const_object_info_begin
class be_class_lv_qrcode (scope: global, name: lv_qrcode, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_qrcode_init)
    _class, comptr(&lv_qrcode_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_animimg
********************************************************************/
#include "be_fixed_be_class_lv_animimg.h"
/* @const_object_info_begin
class be_class_lv_animimg (scope: global, name: lv_animimg, super: be_class_lv_image, strings: weak) {
    init, func(be_ntv_lv_animimg_init)
    _class, comptr(&lv_animimg_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_arc
********************************************************************/
#include "be_fixed_be_class_lv_arc.h"
/* @const_object_info_begin
class be_class_lv_arc (scope: global, name: lv_arc, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_arc_init)
    _class, comptr(&lv_arc_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_bar
********************************************************************/
#include "be_fixed_be_class_lv_bar.h"
/* @const_object_info_begin
class be_class_lv_bar (scope: global, name: lv_bar, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_bar_init)
    _class, comptr(&lv_bar_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_button
********************************************************************/
#include "be_fixed_be_class_lv_button.h"
/* @const_object_info_begin
class be_class_lv_button (scope: global, name: lv_button, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_button_init)
    _class, comptr(&lv_button_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_buttonmatrix
********************************************************************/
#include "be_fixed_be_class_lv_buttonmatrix.h"
/* @const_object_info_begin
class be_class_lv_buttonmatrix (scope: global, name: lv_buttonmatrix, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_buttonmatrix_init)
    _class, comptr(&lv_buttonmatrix_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_calendar
********************************************************************/
#include "be_fixed_be_class_lv_calendar.h"
/* @const_object_info_begin
class be_class_lv_calendar (scope: global, name: lv_calendar, super: be_class_lv_buttonmatrix, strings: weak) {
    init, func(be_ntv_lv_calendar_init)
    _class, comptr(&lv_calendar_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_canvas
********************************************************************/
#include "be_fixed_be_class_lv_canvas.h"
/* @const_object_info_begin
class be_class_lv_canvas (scope: global, name: lv_canvas, super: be_class_lv_image, strings: weak) {
    init, func(be_ntv_lv_canvas_init)
    _class, comptr(&lv_canvas_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_chart
********************************************************************/
#include "be_fixed_be_class_lv_chart.h"
/* @const_object_info_begin
class be_class_lv_chart (scope: global, name: lv_chart, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_chart_init)
    _class, comptr(&lv_chart_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_checkbox
********************************************************************/
#include "be_fixed_be_class_lv_checkbox.h"
/* @const_object_info_begin
class be_class_lv_checkbox (scope: global, name: lv_checkbox, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_checkbox_init)
    _class, comptr(&lv_checkbox_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_dropdown
********************************************************************/
#include "be_fixed_be_class_lv_dropdown.h"
/* @const_object_info_begin
class be_class_lv_dropdown (scope: global, name: lv_dropdown, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_dropdown_init)
    _class, comptr(&lv_dropdown_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_image
********************************************************************/
#include "be_fixed_be_class_lv_image.h"
/* @const_object_info_begin
class be_class_lv_image (scope: global, name: lv_image, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_image_init)
    _class, comptr(&lv_image_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_imagebutton
********************************************************************/
#include "be_fixed_be_class_lv_imagebutton.h"
/* @const_object_info_begin
class be_class_lv_imagebutton (scope: global, name: lv_imagebutton, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_imagebutton_init)
    _class, comptr(&lv_imagebutton_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_keyboard
********************************************************************/
#include "be_fixed_be_class_lv_keyboard.h"
/* @const_object_info_begin
class be_class_lv_keyboard (scope: global, name: lv_keyboard, super: be_class_lv_buttonmatrix, strings: weak) {
    init, func(be_ntv_lv_keyboard_init)
    _class, comptr(&lv_keyboard_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_label
********************************************************************/
#include "be_fixed_be_class_lv_label.h"
/* @const_object_info_begin
class be_class_lv_label (scope: global, name: lv_label, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_label_init)
    _class, comptr(&lv_label_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_led
********************************************************************/
#include "be_fixed_be_class_lv_led.h"
/* @const_object_info_begin
class be_class_lv_led (scope: global, name: lv_led, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_led_init)
    _class, comptr(&lv_led_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_line
********************************************************************/
#include "be_fixed_be_class_lv_line.h"
/* @const_object_info_begin
class be_class_lv_line (scope: global, name: lv_line, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_line_init)
    _class, comptr(&lv_line_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_list
********************************************************************/
#include "be_fixed_be_class_lv_list.h"
/* @const_object_info_begin
class be_class_lv_list (scope: global, name: lv_list, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_list_init)
    _class, comptr(&lv_list_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_menu
********************************************************************/
#include "be_fixed_be_class_lv_menu.h"
/* @const_object_info_begin
class be_class_lv_menu (scope: global, name: lv_menu, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_menu_init)
    _class, comptr(&lv_menu_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_menu_page
********************************************************************/
#include "be_fixed_be_class_lv_menu_page.h"
/* @const_object_info_begin
class be_class_lv_menu_page (scope: global, name: lv_menu_page, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_menu_page_init)
    _class, comptr(&lv_menu_page_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_menu_cont
********************************************************************/
#include "be_fixed_be_class_lv_menu_cont.h"
/* @const_object_info_begin
class be_class_lv_menu_cont (scope: global, name: lv_menu_cont, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_menu_cont_init)
    _class, comptr(&lv_menu_cont_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_menu_section
********************************************************************/
#include "be_fixed_be_class_lv_menu_section.h"
/* @const_object_info_begin
class be_class_lv_menu_section (scope: global, name: lv_menu_section, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_menu_section_init)
    _class, comptr(&lv_menu_section_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_menu_separator
********************************************************************/
#include "be_fixed_be_class_lv_menu_separator.h"
/* @const_object_info_begin
class be_class_lv_menu_separator (scope: global, name: lv_menu_separator, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_menu_separator_init)
    _class, comptr(&lv_menu_separator_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_msgbox
********************************************************************/
#include "be_fixed_be_class_lv_msgbox.h"
/* @const_object_info_begin
class be_class_lv_msgbox (scope: global, name: lv_msgbox, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_msgbox_init)
    _class, comptr(&lv_msgbox_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_roller
********************************************************************/
#include "be_fixed_be_class_lv_roller.h"
/* @const_object_info_begin
class be_class_lv_roller (scope: global, name: lv_roller, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_roller_init)
    _class, comptr(&lv_roller_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_scale
********************************************************************/
#include "be_fixed_be_class_lv_scale.h"
/* @const_object_info_begin
class be_class_lv_scale (scope: global, name: lv_scale, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_scale_init)
    _class, comptr(&lv_scale_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_scale_section
********************************************************************/
#include "be_fixed_be_class_lv_scale_section.h"
/* @const_object_info_begin
class be_class_lv_scale_section (scope: global, name: lv_scale_section, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_scale_section_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_slider
********************************************************************/
#include "be_fixed_be_class_lv_slider.h"
/* @const_object_info_begin
class be_class_lv_slider (scope: global, name: lv_slider, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_slider_init)
    _class, comptr(&lv_slider_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_span
********************************************************************/
#include "be_fixed_be_class_lv_span.h"
/* @const_object_info_begin
class be_class_lv_span (scope: global, name: lv_span, strings: weak) {
    _p, var
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
    init, func(be_ntv_lv_span_init)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_spangroup
********************************************************************/
#include "be_fixed_be_class_lv_spangroup.h"
/* @const_object_info_begin
class be_class_lv_spangroup (scope: global, name: lv_spangroup, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_spangroup_init)
    _class, comptr(&lv_spangroup_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_spinbox
********************************************************************/
#include "be_fixed_be_class_lv_spinbox.h"
/* @const_object_info_begin
class be_class_lv_spinbox (scope: global, name: lv_spinbox, super: be_class_lv_textarea, strings: weak) {
    init, func(be_ntv_lv_spinbox_init)
    _class, comptr(&lv_spinbox_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_spinner
********************************************************************/
#include "be_fixed_be_class_lv_spinner.h"
/* @const_object_info_begin
class be_class_lv_spinner (scope: global, name: lv_spinner, super: be_class_lv_arc, strings: weak) {
    init, func(be_ntv_lv_spinner_init)
    _class, comptr(&lv_spinner_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_switch
********************************************************************/
#include "be_fixed_be_class_lv_switch.h"
/* @const_object_info_begin
class be_class_lv_switch (scope: global, name: lv_switch, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_switch_init)
    _class, comptr(&lv_switch_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_table
********************************************************************/
#include "be_fixed_be_class_lv_table.h"
/* @const_object_info_begin
class be_class_lv_table (scope: global, name: lv_table, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_table_init)
    _class, comptr(&lv_table_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_tabview
********************************************************************/
#include "be_fixed_be_class_lv_tabview.h"
/* @const_object_info_begin
class be_class_lv_tabview (scope: global, name: lv_tabview, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_tabview_init)
    _class, comptr(&lv_tabview_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_textarea
********************************************************************/
#include "be_fixed_be_class_lv_textarea.h"
/* @const_object_info_begin
class be_class_lv_textarea (scope: global, name: lv_textarea, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_textarea_init)
    _class, comptr(&lv_textarea_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_tileview
********************************************************************/
#include "be_fixed_be_class_lv_tileview.h"
/* @const_object_info_begin
class be_class_lv_tileview (scope: global, name: lv_tileview, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_tileview_init)
    _class, comptr(&lv_tileview_class)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_colorwheel
********************************************************************/
#include "be_fixed_be_class_lv_colorwheel.h"
/* @const_object_info_begin
class be_class_lv_colorwheel (scope: global, name: lv_colorwheel, super: be_class_lv_obj, strings: weak) {
    init, func(be_ntv_lv_colorwheel_init)
    _class, comptr(&lv_colorwheel_class)
}
@const_object_info_end */

