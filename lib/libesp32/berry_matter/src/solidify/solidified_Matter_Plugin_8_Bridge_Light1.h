/* Solidification of Matter_Plugin_8_Bridge_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Light1;

/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Light1
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_class(Matter_Plugin_Bridge_Light1,
    0,
    &be_class_Matter_Plugin_Light1,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(BRIDGE, 3), be_const_bool(1) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_light1) },
        { be_const_key_weak(SCHEMA, -1), be_nested_str_weak(relay_X7Cl_X3ARelay_X20number_X7Ct_X3Ai_X7Ch_X3ARelay_X3Cx_X3E_X20number) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(3000) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Light1)
);
/********************************************************************/
/* End of solidification */
