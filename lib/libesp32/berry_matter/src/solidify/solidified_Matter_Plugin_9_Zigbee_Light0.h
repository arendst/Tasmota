/* Solidification of Matter_Plugin_9_Zigbee_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Zigbee_Light0;

/********************************************************************
** Solidified class: Matter_Plugin_Zigbee_Light0
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light0;
be_local_class(Matter_Plugin_Zigbee_Light0,
    1,
    &be_class_Matter_Plugin_Light0,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(zigbee_mapper, -1), be_const_var(0) },
        { be_const_key_weak(ZIGBEE_NAME, -1), be_nested_str_weak(Power) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(z_light0) },
        { be_const_key_weak(SCHEMA, 4), be_nested_str_weak(zigbee_device_X7Cl_X3ADevice_X7Ch_X3ADevice) },
        { be_const_key_weak(ZIGBEE, 2), be_const_bool(1) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Zig_X20Light_X200_X20OnOff) },
        { be_const_key_weak(VIRTUAL, -1), be_const_bool(1) },
    })),
    be_str_weak(Matter_Plugin_Zigbee_Light0)
);
/********************************************************************/
/* End of solidification */
