/* Solidification of Matter_Plugin_2_Sensor.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor;

/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_update_virtual,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(JSON_NAME),
    /* K2   */  be_nested_str_weak(bool),
    /* K3   */  be_nested_str_weak(shadow_value),
    /* K4   */  be_nested_str_weak(value_changed),
    /* K5   */  be_nested_str_weak(update_virtual),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E000E,  //  0005  JMPF	R3	#0015
      0x600C0004,  //  0006  GETGBL	R3	G4
      0x5C100400,  //  0007  MOVE	R4	R2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x1C0C0702,  //  0009  EQ	R3	R3	K2
      0x780E0003,  //  000A  JMPF	R3	#000F
      0x600C0009,  //  000B  GETGBL	R3	G9
      0x5C100400,  //  000C  MOVE	R4	R2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x5C080600,  //  000E  MOVE	R2	R3
      0x880C0103,  //  000F  GETMBR	R3	R0	K3
      0x200C0602,  //  0010  NE	R3	R3	R2
      0x780E0002,  //  0011  JMPF	R3	#0015
      0x8C0C0104,  //  0012  GETMET	R3	R0	K4
      0x7C0C0200,  //  0013  CALL	R3	1
      0x90020602,  //  0014  SETMBR	R0	K3	R2
      0x600C0003,  //  0015  GETGBL	R3	G3
      0x5C100000,  //  0016  MOVE	R4	R0
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C0705,  //  0018  GETMET	R3	R3	K5
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_parse_sensors,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_sensor_matcher),
    /* K1   */  be_nested_str_weak(pre_value),
    /* K2   */  be_nested_str_weak(match),
    /* K3   */  be_nested_str_weak(shadow_value),
    /* K4   */  be_nested_str_weak(value_changed),
    }),
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0010,  //  0001  JMPF	R2	#0013
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x6010000A,  //  0003  GETGBL	R4	G10
      0x88140100,  //  0004  GETMBR	R5	R0	K0
      0x8C140B02,  //  0005  GETMET	R5	R5	K2
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C140400,  //  0007  CALL	R5	2
      0x7C100200,  //  0008  CALL	R4	1
      0x7C080400,  //  0009  CALL	R2	2
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0005,  //  000C  JMPF	R3	#0013
      0x880C0103,  //  000D  GETMBR	R3	R0	K3
      0x200C0403,  //  000E  NE	R3	R2	R3
      0x780E0002,  //  000F  JMPF	R3	#0013
      0x8C0C0104,  //  0010  GETMET	R3	R0	K4
      0x7C0C0200,  //  0011  CALL	R3	1
      0x90020602,  //  0012  SETMBR	R0	K3	R2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_parse_configuration,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_sensor_filter),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    /* K3   */  be_nested_str_weak(tasmota_sensor_matcher),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(Rule_Matcher),
    /* K6   */  be_nested_str_weak(parse),
    }),
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8C080301,  //  0000  GETMET	R2	R1	K1
      0x88100102,  //  0001  GETMBR	R4	R0	K2
      0x7C080400,  //  0002  CALL	R2	2
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x780A0005,  //  0005  JMPF	R2	#000C
      0xB80A0800,  //  0006  GETNGBL	R2	K4
      0x88080505,  //  0007  GETMBR	R2	R2	K5
      0x8C080506,  //  0008  GETMET	R2	R2	K6
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x7C080400,  //  000A  CALL	R2	2
      0x90020602,  //  000B  SETMBR	R0	K3	R2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_pre_value,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(pre_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80040200,  //  0000  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: append_state_json
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_append_state_json,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(shadow_value),
    /* K1   */  be_nested_str_weak(null),
    /* K2   */  be_nested_str_weak(_X2C_X22_X25s_X22_X3A_X25s),
    /* K3   */  be_nested_str_weak(JSON_NAME),
    }),
    be_str_weak(append_state_json),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x70020000,  //  0005  JMP		#0007
      0x58040001,  //  0006  LDCONST	R1	K1
      0x60080018,  //  0007  GETGBL	R2	G24
      0x580C0002,  //  0008  LDCONST	R3	K2
      0x88100103,  //  0009  GETMBR	R4	R0	K3
      0x5C140200,  //  000A  MOVE	R5	R1
      0x7C080600,  //  000B  CALL	R2	3
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: value_changed
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_value_changed,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(value_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor,
    3,
    &be_class_Matter_Plugin_Device,
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG, 1), be_nested_str_weak(filter) },
        { be_const_key_weak(value_changed, -1), be_const_closure(Matter_Plugin_Sensor_value_changed_closure) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(Matter_Plugin_Sensor_update_virtual_closure) },
        { be_const_key_weak(ARG_HINT, 8), be_nested_str_weak(Filter_X20pattern) },
        { be_const_key_weak(tasmota_sensor_matcher, -1), be_const_var(1) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(Matter_Plugin_Sensor_parse_sensors_closure) },
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak() },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(Matter_Plugin_Sensor_parse_configuration_closure) },
        { be_const_key_weak(pre_value, 12), be_const_closure(Matter_Plugin_Sensor_pre_value_closure) },
        { be_const_key_weak(append_state_json, -1), be_const_closure(Matter_Plugin_Sensor_append_state_json_closure) },
        { be_const_key_weak(shadow_value, 9), be_const_var(2) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(tasmota_sensor_filter, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Plugin_Sensor)
);
/*******************************************************************/

void be_load_Matter_Plugin_Sensor_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Sensor);
    be_setglobal(vm, "Matter_Plugin_Sensor");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
