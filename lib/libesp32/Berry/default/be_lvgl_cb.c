/********************************************************************
 * Tasmota LVGL callbacks
 * 
 *******************************************************************/
#include "be_constobj.h"

extern int lv0_init(bvm *vm);
extern int lvx_tostring(bvm *vm);
extern int be_call_c_func(bvm *vm, void * func, const char * return_type, const char * arg_type);


int lv_cb_call_any(bvm *vm, const char * return_type, const char * arg_type) {
    int argc = be_top(vm);
    // get pointer to callback
    be_getmember(vm, 1, ".p");
    void * cb = be_tocomptr(vm, -1);
    be_pop(vm, 1);
    // remove first argument which is the instance `self`
    for (int i=1; i<argc; i++) {
        be_moveto(vm, i+1, i);
    }
    be_pop(vm, 1);
    // call callback
    return be_call_c_func(vm, cb, return_type, arg_type);
}

int lv_cb_call(bvm *vm) {
    return lv_cb_call_any(vm, "i", NULL);
}

// 'lv_group_focus_cb'
// typedef void (*lv_group_focus_cb_t)(struct _lv_group_t *);
int lv_group_focus_cb_call(bvm *vm) {
    return lv_cb_call_any(vm, "", "(lv_group)");
}

// 'lv_event_cb'
// typedef void (*lv_event_cb_t)(struct _lv_obj_t * obj, lv_event_t event);
int lv_event_cb_call(bvm *vm) {
    return lv_cb_call_any(vm, "", "(lv_obj)");
}

// 'lv_signal_cb'
// typedef lv_res_t (*lv_signal_cb_t)(struct _lv_obj_t * obj, lv_signal_t sign, void * param);
int lv_signal_cb_call(bvm *vm) {
    return lv_cb_call_any(vm, "i", "(lv_obj)i.");
}

// 'lv_design_cb'
// typedef lv_design_res_t (*lv_design_cb_t)(struct _lv_obj_t * obj, const lv_area_t * clip_area, lv_design_mode_t mode);
int lv_design_cb_call(bvm *vm) {
    return lv_cb_call_any(vm, "i", "(lv_obj)(lv_area)i");
}

// 'lv_gauge_format_cb'
// typedef void (*lv_gauge_format_cb_t)(lv_obj_t * gauge, char * buf, int bufsize, int32_t value);
int lv_gauge_format_cb_call(bvm *vm) {
    return lv_cb_call_any(vm, "", "(lv_obj).ii");
}


#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_fixed_be_lvgl_cb.h"
#endif

void be_load_lvgl_cb_lib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
    static const bnfuncinfo members[] = {
        { ".p", NULL },
        { "init", lv0_init },
        { "tostring", lvx_tostring },
        { "()", lv_cb_call },

        // { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */
        // { "get_tasmota", (bntvfunc) &get_tasmota_closure },
        // { "add_cmd", (bntvfunc) &add_cmd_closure },
        
        { NULL, NULL }
    };
    be_regclass(vm, "lv_cb", members);
#else
    be_pushntvclass(vm, &be_lvgl_cb);
    be_setglobal(vm, "lv_cb");
    be_pop(vm, 1);
#endif
}
/* @const_object_info_begin

class be_lvgl_cb (scope: global, name: lv_cb) {
    .p, var
    init, func(lv0_init)
    tostring, func(lvx_tostring)
    call, func(lv_cb_call)
}
@const_object_info_end */

// instanciate all callbacks as sub-classes
// 'lv_group_focus_cb'
// 'lv_event_cb'
// 'lv_signal_cb'
// 'lv_design_cb'
// 'lv_gauge_format_cb'
//

#include "../generate/be_fixed_be_lv_group_focus_cb.h"
#include "../generate/be_fixed_be_lv_event_cb.h"
#include "../generate/be_fixed_be_lv_signal_cb.h"
#include "../generate/be_fixed_be_lv_design_cb.h"
#include "../generate/be_fixed_be_lv_gauge_format_cb.h"

void be_load_lvgl_cb_all_lib(bvm *vm) {
    be_pushntvclass(vm, &be_lv_group_focus_cb);
    be_setglobal(vm, "lv_group_focus_cb");
    be_pop(vm, 1);

    be_pushntvclass(vm, &be_lv_event_cb);
    be_setglobal(vm, "lv_event_cb");
    be_pop(vm, 1);
    
    be_pushntvclass(vm, &be_lv_signal_cb);
    be_setglobal(vm, "lv_signal_cb");
    be_pop(vm, 1);

    be_pushntvclass(vm, &be_lv_design_cb);
    be_setglobal(vm, "lv_design_cb");
    be_pop(vm, 1);

    be_pushntvclass(vm, &be_lv_gauge_format_cb);
    be_setglobal(vm, "lv_gauge_format_cb");
    be_pop(vm, 1);
}
/* @const_object_info_begin

class be_lv_group_focus_cb (scope: global, name: lv_group_focus_cb, super: be_lvgl_cb) {
    call, func(lv_group_focus_cb_call)
}

class be_lv_event_cb (scope: global, name: lv_event_cb, super: be_lvgl_cb) {
    call, func(lv_event_cb_call)
}

class be_lv_signal_cb (scope: global, name: lv_signal_cb, super: be_lvgl_cb) {
    call, func(lv_signal_cb_call)
}

class be_lv_design_cb (scope: global, name: lv_design_cb, super: be_lvgl_cb) {
    call, func(lv_design_cb_call)
}

class be_lv_gauge_format_cb (scope: global, name: lv_gauge_format_cb, super: be_lvgl_cb) {
    call, func(lv_gauge_format_cb_call)
}

@const_object_info_end */
