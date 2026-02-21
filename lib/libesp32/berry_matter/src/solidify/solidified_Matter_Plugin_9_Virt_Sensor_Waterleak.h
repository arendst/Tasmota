/* Solidification of Matter_Plugin_9_Virt_Sensor_Waterleak.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Virt_Sensor_Waterleak;

/********************************************************************
** Solidified class: Matter_Plugin_Virt_Sensor_Waterleak
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Waterleak;
be_local_class(Matter_Plugin_Virt_Sensor_Waterleak,
    0,
    &be_class_Matter_Plugin_Sensor_Waterleak,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(VIRTUAL, -1), be_const_bool(1) },
        { be_const_key_weak(DISPLAY_NAME, 3), be_nested_str_weak(v_X2EWaterleak) },
        { be_const_key_weak(SCHEMA, -1), be_const_nil() },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(v_waterleak) },
    })),
    be_str_weak(Matter_Plugin_Virt_Sensor_Waterleak)
);
/********************************************************************/
/* End of solidification */
