/********************************************************************
 * Tasmota LVGL Font class
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

extern int lco_init(bvm *vm);           // generic function
extern int lco_tostring(bvm *vm);       // generic function
extern int lco_toint(bvm *vm);       // generic function


#include "../generate/be_fixed_be_class_lv_color.h"

void be_load_lvgl_color_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_color);
    be_setglobal(vm, "lv_color");
    be_pop(vm, 1);
}

/* @const_object_info_begin
class be_class_lv_color (scope: global, name: lv_color) {
    .p, var
    init, func(lco_init)
    tostring, func(lco_tostring)
    toint, func(lco_toint)
}
@const_object_info_end */

#endif // USE_LVGL