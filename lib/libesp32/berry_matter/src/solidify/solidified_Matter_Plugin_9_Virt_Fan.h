/* Solidification of Matter_Plugin_9_Virt_Fan.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Virt_Fan;

/********************************************************************
** Solidified class: Matter_Plugin_Virt_Fan
********************************************************************/
extern const bclass be_class_Matter_Plugin_Fan;
be_local_class(Matter_Plugin_Virt_Fan,
    0,
    &be_class_Matter_Plugin_Fan,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(VIRTUAL, 3), be_const_bool(1) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(v_X2EFan) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(v_fan) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(_Not_X20used_) },
        { be_const_key_weak(ARG, 2), be_nested_str_weak() },
    })),
    be_str_weak(Matter_Plugin_Virt_Fan)
);
/********************************************************************/
/* End of solidification */
