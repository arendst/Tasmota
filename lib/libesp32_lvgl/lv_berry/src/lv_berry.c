#include "lv_berry.h"

extern void be_load_lv_color_class(bvm *vm);
extern void be_load_lv_font_class(bvm *vm);
extern void be_load_LVGL_glob_class(bvm *vm);

void be_load_lvgl_classes(bvm *vm) {
  be_load_lv_color_class(vm);
  be_load_lv_font_class(vm);
  be_load_LVGL_glob_class(vm);
}
