/* Solidification of Matter_Plugin_9_Zigbee_Occupancy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Zigbee_Occupancy;

/********************************************************************
** Solidified class: Matter_Plugin_Zigbee_Occupancy
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_class(Matter_Plugin_Zigbee_Occupancy,
    1,
    &be_class_Matter_Plugin_Sensor_Occupancy,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(zigbee_mapper, -1), be_const_var(0) },
        { be_const_key_weak(ZIGBEE_NAME, -1), be_nested_str_weak(Occupancy) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(z_occupancy) },
        { be_const_key_weak(SCHEMA, 4), be_nested_str_weak(zigbee_device_X7Cl_X3ADevice_X7Ch_X3ADevice) },
        { be_const_key_weak(ZIGBEE, 2), be_const_bool(1) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Zig_X20Occupancy) },
        { be_const_key_weak(VIRTUAL, -1), be_const_bool(1) },
    })),
    be_str_weak(Matter_Plugin_Zigbee_Occupancy)
);
/********************************************************************/
/* End of solidification */
