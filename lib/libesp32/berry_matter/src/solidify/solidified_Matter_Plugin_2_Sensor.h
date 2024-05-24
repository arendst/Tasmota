/* Solidification of Matter_Plugin_2_Sensor.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor;

/********************************************************************
** Solidified function: update_virtual
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_closure(class_Matter_Plugin_Sensor_update_virtual,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor, 
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
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_closure(class_Matter_Plugin_Sensor_parse_sensors,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(VIRTUAL),
    /* K1   */  be_nested_str_weak(tasmota_sensor_matcher),
    /* K2   */  be_nested_str_weak(pre_value),
    /* K3   */  be_nested_str_weak(match),
    /* K4   */  be_nested_str_weak(shadow_value),
    /* K5   */  be_nested_str_weak(value_changed),
    }),
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0012,  //  0001  JMPT	R2	#0015
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x780A0010,  //  0003  JMPF	R2	#0015
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x6010000A,  //  0005  GETGBL	R4	G10
      0x88140101,  //  0006  GETMBR	R5	R0	K1
      0x8C140B03,  //  0007  GETMET	R5	R5	K3
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x7C140400,  //  0009  CALL	R5	2
      0x7C100200,  //  000A  CALL	R4	1
      0x7C080400,  //  000B  CALL	R2	2
      0x4C0C0000,  //  000C  LDNIL	R3
      0x200C0403,  //  000D  NE	R3	R2	R3
      0x780E0005,  //  000E  JMPF	R3	#0015
      0x880C0104,  //  000F  GETMBR	R3	R0	K4
      0x200C0403,  //  0010  NE	R3	R2	R3
      0x780E0002,  //  0011  JMPF	R3	#0015
      0x8C0C0105,  //  0012  GETMET	R3	R0	K5
      0x7C0C0200,  //  0013  CALL	R3	1
      0x90020802,  //  0014  SETMBR	R0	K4	R2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_closure(class_Matter_Plugin_Sensor_parse_configuration,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor, 
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
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_closure(class_Matter_Plugin_Sensor_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(add_read_sensors_schedule),
    /* K2   */  be_nested_str_weak(UPDATE_TIME),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x8C100301,  //  0008  GETMET	R4	R1	K1
      0x88180102,  //  0009  GETMBR	R6	R0	K2
      0x7C100400,  //  000A  CALL	R4	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pre_value
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_closure(class_Matter_Plugin_Sensor_pre_value,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor, 
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
** Solidified function: value_changed
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_closure(class_Matter_Plugin_Sensor_value_changed,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor, 
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
        { be_const_key_weak(value_changed, -1), be_const_closure(class_Matter_Plugin_Sensor_value_changed_closure) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Sensor_update_virtual_closure) },
        { be_const_key_weak(ARG_HINT, 9), be_nested_str_weak(Filter_X20pattern) },
        { be_const_key_weak(tasmota_sensor_matcher, -1), be_const_var(1) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(class_Matter_Plugin_Sensor_parse_sensors_closure) },
        { be_const_key_weak(JSON_NAME, 8), be_nested_str_weak() },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(class_Matter_Plugin_Sensor_parse_configuration_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Sensor_init_closure) },
        { be_const_key_weak(pre_value, 12), be_const_closure(class_Matter_Plugin_Sensor_pre_value_closure) },
        { be_const_key_weak(shadow_value, -1), be_const_var(2) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(tasmota_sensor_filter, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Plugin_Sensor)
);
/********************************************************************/
/* End of solidification */
