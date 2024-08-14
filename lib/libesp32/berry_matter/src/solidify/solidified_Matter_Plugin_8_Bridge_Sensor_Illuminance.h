/* Solidification of Matter_Plugin_8_Bridge_Sensor_Illuminance.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Illuminance;

/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Sensor_Illuminance
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Illuminance;
be_local_class(Matter_Plugin_Bridge_Sensor_Illuminance,
    0,
    &be_class_Matter_Plugin_Sensor_Illuminance,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(BRIDGE, -1), be_const_bool(1) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_illuminance) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor_Illuminance)
);
/********************************************************************/
/* End of solidification */
