

#ifndef __LV_BERRY__
#define __LV_BERRY__

#ifdef __cplusplus
extern "C" {
#endif

#include "berry.h"
#include "lvgl.h"

/*********************************************************************************************\
 * Support for lv_color
\*********************************************************************************************/
inline lv_color_t lv_color_from_uint32(uint32_t ucol) {
  lv_color_t * col = (lv_color_t*) &ucol;
  return *col;
}
inline uint32_t lv_color_to_uint32(lv_color_t col) {
  uint16_t *p = (uint16_t*) &col;
  return *p;
}

extern void be_load_lvgl_classes(bvm *vm);

// TODO temporary fix
extern void lv_img_set_tasmota_logo(lv_obj_t * img);

// used for ToushScreen calibration
typedef struct lv_ts_calibration_t {
  lv_coord_t        raw_x;
  lv_coord_t        raw_y;
  lv_coord_t        x;
  lv_coord_t        y;
  lv_indev_state_t  state;
} lv_ts_calibration_t;
extern lv_ts_calibration_t lv_ts_calibration;


#ifdef __cplusplus
}
#endif
#endif // __LV_BERRY__
