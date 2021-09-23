/********************************************************************
 * Tasmota LVGL Font class
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

extern int lvx_init(bvm *vm);           // generic function
extern int lvx_tostring(bvm *vm);       // generic function

#include "../generate/be_fixed_be_class_lv_font.h"

void be_load_lvgl_font_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_font);
    be_setglobal(vm, "lv_font");
    be_pop(vm, 1);
}

/* @const_object_info_begin
class be_class_lv_font (scope: global, name: lv_font) {
    .p, var
    init, func(lvx_init)
    tostring, func(lvx_tostring)
}
@const_object_info_end */

#endif // USE_LVGL