/********************************************************************
 * Tasmota LVGL Font class
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

extern int lco_init(bvm *vm);           // generic function
extern int lco_tostring(bvm *vm);       // generic function


#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_fixed_be_class_lv_color.h"
#endif

void be_load_lvgl_color_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
    static const bnfuncinfo members[] = {
        { ".p", NULL },               // keeping track of styles to avoid GC
        { "init", lco_init },
        { "tostring", lco_tostring },

        // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */
        
        { NULL, NULL }
    };
    be_regclass(vm, "lv_color", members);
#else
    be_pushntvclass(vm, &be_class_lv_color);
    be_setglobal(vm, "lv_color");
    be_pop(vm, 1);
#endif
}

/* @const_object_info_begin
class be_class_lv_color (scope: global, name: lv_color) {
    .p, var
    init, func(lco_init)
    tostring, func(lco_tostring)
}
@const_object_info_end */

#endif // USE_LVGL