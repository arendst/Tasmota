/* Solidification of Matter_Plugin_3_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light0;

/********************************************************************
** Solidified class: Matter_Plugin_Light0
********************************************************************/
extern const bclass be_class_Matter_Plugin_OnOff;
be_local_class(Matter_Plugin_Light0,
    0,
    &be_class_Matter_Plugin_OnOff,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X200_X20On) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light0) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(256, -1), be_const_int(2) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Light0)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light0_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light0);
    be_setglobal(vm, "Matter_Plugin_Light0");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
