/**
 * @file lv_api_map_v9_1.h
 *
 */

#ifndef LV_API_MAP_V9_1_H
#define LV_API_MAP_V9_1_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "misc/lv_types.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *      MACROS
 **********************/

#define _LV_EVENT_LAST  LV_EVENT_LAST
#define _lv_event_code_t lv_event_code_t
#define _lv_event_mark_deleted lv_event_mark_deleted
#define lv_obj_add_event lv_obj_add_event_cb

#define _LV_STYLE_LAST_BUILT_IN_PROP LV_STYLE_LAST_BUILT_IN_PROP
#define _LV_FLEX_REVERSE LV_FLEX_REVERSE
#define _LV_FLEX_WRAP  LV_FLEX_WRAP
#define _LV_FLEX_COLUMN LV_FLEX_COLUMN

#define _lv_area_is_equal lv_area_is_equal
#define _lv_area_is_in lv_area_is_in
#define _lv_area_intersect lv_area_intersect
#define _lv_area_is_point_on lv_area_is_point_on
#define _lv_area_join lv_area_join
#define _lv_image_buf_get_transformed_area lv_image_buf_get_transformed_area

#define _lv_ll_init lv_ll_init
#define _lv_ll_ins_head lv_ll_ins_head
#define _lv_ll_ins_prev lv_ll_ins_prev
#define _lv_ll_ins_tail lv_ll_ins_tail
#define _lv_ll_get_head lv_ll_get_head
#define _lv_ll_get_tail lv_ll_get_tail
#define _lv_ll_get_next lv_ll_get_next
#define _lv_ll_get_prev lv_ll_get_prev
#define _lv_ll_get_len lv_ll_get_len
#define _lv_ll_move_before lv_ll_move_before
#define _lv_ll_is_empty lv_ll_is_empty
#define _lv_ll_clear lv_ll_clear
#define _lv_ll_remove lv_ll_remove
#define _lv_ll_chg_list lv_ll_chg_list
#define _LV_LL_READ LV_LL_READ
#define _LV_LL_READ_BACK LV_LL_READ_BACK

#define _lv_obj_scroll_by_raw lv_obj_scroll_by_raw
#define _lv_obj_get_ext_draw_size lv_obj_get_ext_draw_size
#define _lv_indev_scroll_handler lv_indev_scroll_handler

#define _lv_display_refr_timer lv_disp_refr_timer
#define _lv_disp_refr_timer lv_disp_refr_timer
#define _lv_disp_get_refr_timer lv_disp_get_refr_timer

#define lv_obj_get_child_by_id  lv_obj_find_by_id
#define lv_obj_update_flag      lv_obj_set_flag

#define _lv_inv_area lv_inv_area
#define lv_chart_set_all_value lv_chart_set_all_values
#define lv_calendar_set_showed_date lv_calendar_set_month_shown
#define lv_chart_set_range lv_chart_set_axis_range
#define lv_chart_set_value_by_id lv_chart_set_series_value_by_id
#define lv_chart_get_x_array lv_chart_get_series_x_array
#define lv_chart_get_y_array lv_chart_get_series_y_array
#define lv_chart_set_ext_x_array lv_chart_set_series_ext_x_array
#define lv_chart_set_ext_y_array lv_chart_set_series_ext_y_array

#if defined(LV_FS_DEFAULT_DRIVE_LETTER)
#warning LV_FS_DEFAULT_DRIVE_LETTER is deprecated. Rename to LV_FS_DEFAULT_DRIVER_LETTER

#if LV_FS_DEFAULT_DRIVER_LETTER == '\0'
#undef LV_FS_DEFAULT_DRIVER_LETTER
#define LV_FS_DEFAULT_DRIVER_LETTER LV_FS_DEFAULT_DRIVE_LETTER
#endif

#endif /* defined(LV_FS_DEFAULT_DRIVE_LETTER) */

#define LV_LABEL_LONG_WRAP             LV_LABEL_LONG_MODE_WRAP
#define LV_LABEL_LONG_DOT              LV_LABEL_LONG_MODE_DOTS
#define LV_LABEL_LONG_SCROLL           LV_LABEL_LONG_MODE_SCROLL
#define LV_LABEL_LONG_SCROLL_CIRCULAR  LV_LABEL_LONG_MODE_SCROLL_CIRCULAR
#define LV_LABEL_LONG_CLIP             LV_LABEL_LONG_MODE_CLIP

#define lv_anim_set_time                lv_anim_set_duration
#define lv_anim_set_playback_time       lv_anim_set_reverse_duration
#define lv_anim_set_playback_delay      lv_anim_set_reverse_delay
#define lv_anim_set_playback_duration   lv_anim_set_reverse_duration

#define lv_gradient_init_stops  lv_grad_init_stops
#define lv_gradient_stop_t      lv_grad_stop_t

#define lv_spangroup_new_span lv_spangroup_add_span
#define lv_spangroup_refr_mode lv_spangroup_refresh

#define lv_slider_set_left_value lv_slider_set_start_value

#define lv_calendar_header_arrow_create     lv_calendar_add_header_arrow
#define lv_calendar_header_dropdown_create  lv_calendar_add_header_dropdown

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* LV_API_MAP_V9_1_H */
