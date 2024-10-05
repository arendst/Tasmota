/* Solidification of Matter_Plugin_9_Zigbee_Occupancy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Zigbee_Occupancy;

/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(class_Matter_Plugin_Zigbee_Occupancy__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(_X3Clambda_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040008,  //  0000  GETGBL	R1	G8
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Zigbee_Occupancy
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_class(Matter_Plugin_Zigbee_Occupancy,
    1,
    &be_class_Matter_Plugin_Sensor_Occupancy,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG, 2), be_nested_str_weak(zigbee_device) },
        { be_const_key_weak(ARG_TYPE, 8), be_const_static_closure(class_Matter_Plugin_Zigbee_Occupancy__X3Clambda_X3E_closure) },
        { be_const_key_weak(ARG_HINT, 7), be_nested_str_weak(Device) },
        { be_const_key_weak(ZIGBEE, 6), be_const_bool(1) },
        { be_const_key_weak(ZIGBEE_NAME, -1), be_nested_str_weak(Occupancy) },
        { be_const_key_weak(VIRTUAL, -1), be_const_bool(1) },
        { be_const_key_weak(zigbee_mapper, -1), be_const_var(0) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Zig_X20Occupancy) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(z_occupancy) },
    })),
    be_str_weak(Matter_Plugin_Zigbee_Occupancy)
);
/********************************************************************/
/* End of solidification */
