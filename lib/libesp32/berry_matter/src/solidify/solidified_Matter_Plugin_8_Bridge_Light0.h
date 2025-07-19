/* Solidification of Matter_Plugin_8_Bridge_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Light0;

/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Light0
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light0;
be_local_class(Matter_Plugin_Bridge_Light0,
    0,
    &be_class_Matter_Plugin_Light0,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(3000) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_light0) },
        { be_const_key_weak(BRIDGE, -1), be_const_bool(1) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Light0)
);
/********************************************************************/
/* End of solidification */
