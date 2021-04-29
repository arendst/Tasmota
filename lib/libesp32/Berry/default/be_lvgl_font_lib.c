/********************************************************************
 * Tasmota LVGL Font class
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

extern int lvx_init(bvm *vm);           // generic function
extern int lvx_tostring(bvm *vm);       // generic function

#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_fixed_be_class_lv_font.h"
#endif

void be_load_lvgl_font_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
    static const bnfuncinfo members[] = {
        { ".p", NULL },               // keeping track of styles to avoid GC
        { "init", lvx_init },
        { "tostring", lvx_tostring },

        // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */
        
        { NULL, NULL }
    };
    be_regclass(vm, "lv_font", members);
#else
    be_pushntvclass(vm, &be_class_lv_font);
    be_setglobal(vm, "lv_font");
    be_pop(vm, 1);
#endif
}

/* @const_object_info_begin
class be_class_lv_font (scope: global, name: lv_font) {
    .p, var
    init, func(lvx_init)
    tostring, func(lvx_tostring)
}
@const_object_info_end */

#endif // USE_LVGL