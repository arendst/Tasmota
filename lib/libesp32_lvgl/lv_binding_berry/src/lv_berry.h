

#ifndef __LV_BERRY__
#define __LV_BERRY__

#ifdef __cplusplus
extern "C" {
#endif

#include "berry.h"
#include "lvgl.h"

extern void be_load_lvgl_classes(bvm *vm);

// TODO temporary fix
extern void lv_image_set_tasmota_logo(lv_obj_t * img);

// used for ToushScreen calibration
typedef struct lv_ts_calibration_t {
  lv_coord_t        raw_x;
  lv_coord_t        raw_y;
  lv_coord_t        x;
  lv_coord_t        y;
  lv_indev_state_t  state;
} lv_ts_calibration_t;
extern lv_ts_calibration_t lv_ts_calibration;

// temporarily fix lv_span_get_style()
extern lv_style_t * lv_span_get_style(lv_span_t * span);

// add accessor for lv_bar->indic_area
extern lv_area_t * lv_bar_get_indic_area(lv_obj_t * bar);

// add accessor for lv_line points array
extern lv_point_t * lv_line_get_points(lv_obj_t * line);
extern int lv_line_get_points_num(lv_obj_t * line);

#ifdef __cplusplus
}
#endif
#endif // __LV_BERRY__
