/* Solidification of Matter_Plugin_9_Virt_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Virt_Light1;

/********************************************************************
** Solidified class: Matter_Plugin_Virt_Light1
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_class(Matter_Plugin_Virt_Light1,
    0,
    &be_class_Matter_Plugin_Light1,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(VIRTUAL, 3), be_const_bool(1) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(v_X2ELight_X201_X20Dimmer) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(v_light1) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(_Not_X20used_) },
        { be_const_key_weak(ARG, 2), be_nested_str_weak() },
    })),
    be_str_weak(Matter_Plugin_Virt_Light1)
);
/*******************************************************************/

void be_load_Matter_Plugin_Virt_Light1_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Virt_Light1);
    be_setglobal(vm, "Matter_Plugin_Virt_Light1");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
