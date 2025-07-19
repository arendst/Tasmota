/* Solidification of Matter_Plugin_8_Bridge_Sensor_Temp.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Temp;

/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Sensor_Temp
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Temp;
be_local_class(Matter_Plugin_Bridge_Sensor_Temp,
    0,
    &be_class_Matter_Plugin_Sensor_Temp,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(BRIDGE, -1), be_const_bool(1) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_temperature) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor_Temp)
);
/********************************************************************/
/* End of solidification */
