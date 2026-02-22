/* Solidification of Matter_Plugin_9_Zigbee_Light2.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Zigbee_Light2;

/********************************************************************
** Solidified class: Matter_Plugin_Zigbee_Light2
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light2;
be_local_class(Matter_Plugin_Zigbee_Light2,
    1,
    &be_class_Matter_Plugin_Light2,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(SCHEMA, -1), be_nested_str_weak(zigbee_device_X7Cl_X3ADevice_X7Ch_X3ADevice) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(z_light2) },
        { be_const_key_weak(VIRTUAL, -1), be_const_bool(1) },
        { be_const_key_weak(DISPLAY_NAME, 4), be_nested_str_weak(Zig_X20Light_X202_X20CT) },
        { be_const_key_weak(zigbee_mapper, 5), be_const_var(0) },
        { be_const_key_weak(ZIGBEE, -1), be_const_bool(1) },
    })),
    be_str_weak(Matter_Plugin_Zigbee_Light2)
);
/********************************************************************/
/* End of solidification */
