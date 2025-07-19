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
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(VIRTUAL, 3), be_const_bool(1) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(v_X2EWaterleak) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(v_waterleak) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(_Not_X20used_) },
        { be_const_key_weak(ARG, 2), be_nested_str_weak() },
    })),
    be_str_weak(Matter_Plugin_Virt_Sensor_Waterleak)
);
/********************************************************************/
/* End of solidification */
