/* Solidification of Matter_Plugin_9_Virt_Sensor_Rain.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Virt_Sensor_Rain;

/********************************************************************
** Solidified class: Matter_Plugin_Virt_Sensor_Rain
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Rain;
be_local_class(Matter_Plugin_Virt_Sensor_Rain,
    0,
    &be_class_Matter_Plugin_Sensor_Rain,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(VIRTUAL, -1), be_const_bool(1) },
        { be_const_key_weak(DISPLAY_NAME, 3), be_nested_str_weak(v_X2ERain) },
        { be_const_key_weak(SCHEMA, -1), be_const_nil() },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(v_rain) },
    })),
    be_str_weak(Matter_Plugin_Virt_Sensor_Rain)
);
/********************************************************************/
/* End of solidification */
