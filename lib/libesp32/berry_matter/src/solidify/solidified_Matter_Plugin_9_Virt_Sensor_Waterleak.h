/* Solidification of Matter_Plugin_9_Virt_Sensor_Waterleak.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Virt_Sensor_Waterleak;

/********************************************************************
** Solidified class: Matter_Plugin_Virt_Sensor_Waterleak
********************************************************************/
extern const bclass be_class_Matter_Plugin_Virt_Sensor_Waterleak;
be_local_class(Matter_Plugin_Virt_Sensor_Waterleak,
    0,
    &be_class_Matter_Plugin_Virt_Sensor_Waterleak,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(VIRTUAL, 3), be_const_bool(1) },
        { be_const_key(DISPLAY_NAME, -1), be_nested_str(v_X2EWaterleak) },
        { be_const_key(TYPE, -1), be_nested_str(v_waterleak) },
        { be_const_key(ARG_HINT, -1), be_nested_str(_Not_X20used_) },
        { be_const_key(ARG, 2), be_nested_str() },
    })),
    (bstring*) &be_const_str_Matter_Plugin_Virt_Sensor_Waterleak
);
/*******************************************************************/

void be_load_Matter_Plugin_Virt_Sensor_Waterleak_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Virt_Sensor_Waterleak);
    be_setglobal(vm, "Matter_Plugin_Virt_Sensor_Waterleak");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
