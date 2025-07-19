

#ifndef __LV_BERRY__
#define __LV_BERRY__

#ifdef __cplusplus
extern "C" {
#endif

#include "berry.h"
#include "lvgl.h"

extern void be_load_lvgl_classes(bvm *vm);

extern void lv_image_set_tasmota_logo(lv_obj_t * img);
extern void lv_image_set_tasmota_logo36(lv_obj_t * img);

// used for ToushScreen calibration
typedef struct lv_ts_calibration_t {
  lv_coord_t        raw_x;
  lv_coord_t        raw_y;
  lv_coord_t        x;
  lv_coord_t        y;
  lv_indev_state_t  state;
} lv_ts_calibration_t;
extern lv_ts_calibration_t lv_ts_calibration;

// add accessor for lv_bar->indic_area
extern lv_area_t * lv_bar_get_indic_area(lv_obj_t * bar);

#ifdef __cplusplus
}
#endif
#endif // __LV_BERRY__
