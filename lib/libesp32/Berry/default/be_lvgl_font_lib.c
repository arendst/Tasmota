/********************************************************************
 * Tasmota LVGL Font class
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

extern int lvx_tostring(bvm *vm);       // generic function
extern int lvx_init_ctor(bvm *vm, void * func);
extern int be_call_c_func(bvm *vm, void * func, const char * return_type, const char * arg_type);


// create font either empty or from parameter on stack
int lvbe_font_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+lv_group", ""); }

#include "../generate/be_fixed_be_class_lv_font.h"

void be_load_lvgl_font_lib(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_font);
    be_setglobal(vm, "lv_font");
    be_pop(vm, 1);
}

/* @const_object_info_begin
class be_class_lv_font (scope: global, name: lv_font) {
    _p, var
    init, func(lvbe_font_create)
    tostring, func(lvx_tostring)
}
@const_object_info_end */

#endif // USE_LVGL