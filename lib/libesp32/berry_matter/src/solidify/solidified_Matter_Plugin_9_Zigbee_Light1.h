/* Solidification of Matter_Plugin_9_Zigbee_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Zigbee_Light1;

/********************************************************************
** Solidified class: Matter_Plugin_Zigbee_Light1
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_class(Matter_Plugin_Zigbee_Light1,
    1,
    &be_class_Matter_Plugin_Light1,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(SCHEMA, -1), be_nested_str_weak(zigbee_device_X7Cl_X3ADevice_X7Ch_X3ADevice) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(z_light1) },
        { be_const_key_weak(VIRTUAL, -1), be_const_bool(1) },
        { be_const_key_weak(DISPLAY_NAME, 4), be_nested_str_weak(Zig_X20Light_X201_X20Dimmer) },
        { be_const_key_weak(zigbee_mapper, 5), be_const_var(0) },
        { be_const_key_weak(ZIGBEE, -1), be_const_bool(1) },
    })),
    be_str_weak(Matter_Plugin_Zigbee_Light1)
);
/********************************************************************/
/* End of solidification */
