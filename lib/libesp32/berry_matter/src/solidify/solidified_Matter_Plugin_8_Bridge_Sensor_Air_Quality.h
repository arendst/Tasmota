/* Solidification of Matter_Plugin_8_Bridge_Sensor_Air_Quality.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Air_Quality;

/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Sensor_Air_Quality
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;
be_local_class(Matter_Plugin_Bridge_Sensor_Air_Quality,
    0,
    &be_class_Matter_Plugin_Sensor_Air_Quality,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(BRIDGE, 2), be_const_bool(1) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_airquality) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(UPDATE_CMD, -1), be_nested_str_weak(Status_X2010) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor_Air_Quality)
);
/********************************************************************/
/* End of solidification */
