/* Solidification of Matter_Plugin_Temp_Sensor.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Temp_Sensor;

/********************************************************************
** Solidified function: get_temperature
********************************************************************/
be_local_closure(Matter_Plugin_Temp_Sensor_get_temperature,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(shadow_temperature),
    }),
    be_str_weak(get_temperature),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Plugin_Temp_Sensor_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_temperature),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(Matter_Plugin_Temp_Sensor_parse_sensors,   /* name */
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
    /* K1   */  be_nested_str_weak(match),
    /* K2   */  be_nested_str_weak(shadow_temperature),
    /* K3   */  be_nested_str_weak(attribute_updated),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0011,  //  0001  JMPF	R2	#0014
      0x6008000A,  //  0002  GETGBL	R2	G10
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C0C0400,  //  0006  CALL	R3	2
      0x7C080200,  //  0007  CALL	R2	1
      0x4C0C0000,  //  0008  LDNIL	R3
      0x200C0403,  //  0009  NE	R3	R2	R3
      0x780E0008,  //  000A  JMPF	R3	#0014
      0x880C0102,  //  000B  GETMBR	R3	R0	K2
      0x200C0403,  //  000C  NE	R3	R2	R3
      0x780E0004,  //  000D  JMPF	R3	#0013
      0x8C0C0103,  //  000E  GETMET	R3	R0	K3
      0x4C140000,  //  000F  LDNIL	R5
      0x541A0401,  //  0010  LDINT	R6	1026
      0x581C0004,  //  0011  LDCONST	R7	K4
      0x7C0C0800,  //  0012  CALL	R3	4
      0x90020402,  //  0013  SETMBR	R0	K2	R2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Temp_Sensor_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(tasmota_sensor_filter),
    /* K2   */  be_nested_str_weak(tasmota_sensor_matcher),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(Rule_Matcher),
    /* K5   */  be_nested_str_weak(parse),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x7C100600,  //  0006  CALL	R4	3
      0x90020203,  //  0007  SETMBR	R0	K1	R3
      0xB8120600,  //  0008  GETNGBL	R4	K3
      0x88100904,  //  0009  GETMBR	R4	R4	K4
      0x8C100905,  //  000A  GETMET	R4	R4	K5
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x90020404,  //  000D  SETMBR	R0	K2	R4
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Temp_Sensor_read_attribute,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_temperature),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(I2),
    /* K9   */  be_nested_str_weak(NULL),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[55]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0401,  //  0005  LDINT	R7	1026
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0025,  //  0007  JMPF	R7	#002E
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E0013,  //  0009  JMPF	R7	#001E
      0x881C0106,  //  000A  GETMBR	R7	R0	K6
      0x4C200000,  //  000B  LDNIL	R8
      0x201C0E08,  //  000C  NE	R7	R7	R8
      0x781E0009,  //  000D  JMPF	R7	#0018
      0x8C1C0907,  //  000E  GETMET	R7	R4	K7
      0x88240908,  //  000F  GETMBR	R9	R4	K8
      0x60280009,  //  0010  GETGBL	R10	G9
      0x882C0106,  //  0011  GETMBR	R11	R0	K6
      0x54320063,  //  0012  LDINT	R12	100
      0x082C160C,  //  0013  MUL	R11	R11	R12
      0x7C280200,  //  0014  CALL	R10	1
      0x7C1C0600,  //  0015  CALL	R7	3
      0x80040E00,  //  0016  RET	1	R7
      0x70020004,  //  0017  JMP		#001D
      0x8C1C0907,  //  0018  GETMET	R7	R4	K7
      0x88240909,  //  0019  GETMBR	R9	R4	K9
      0x4C280000,  //  001A  LDNIL	R10
      0x7C1C0600,  //  001B  CALL	R7	3
      0x80040E00,  //  001C  RET	1	R7
      0x7002000E,  //  001D  JMP		#002D
      0x1C1C0D0A,  //  001E  EQ	R7	R6	K10
      0x781E0005,  //  001F  JMPF	R7	#0026
      0x8C1C0907,  //  0020  GETMET	R7	R4	K7
      0x88240908,  //  0021  GETMBR	R9	R4	K8
      0x5429EC77,  //  0022  LDINT	R10	-5000
      0x7C1C0600,  //  0023  CALL	R7	3
      0x80040E00,  //  0024  RET	1	R7
      0x70020006,  //  0025  JMP		#002D
      0x1C1C0D0B,  //  0026  EQ	R7	R6	K11
      0x781E0004,  //  0027  JMPF	R7	#002D
      0x8C1C0907,  //  0028  GETMET	R7	R4	K7
      0x88240908,  //  0029  GETMBR	R9	R4	K8
      0x542A3A97,  //  002A  LDINT	R10	15000
      0x7C1C0600,  //  002B  CALL	R7	3
      0x80040E00,  //  002C  RET	1	R7
      0x70020007,  //  002D  JMP		#0036
      0x601C0003,  //  002E  GETGBL	R7	G3
      0x5C200000,  //  002F  MOVE	R8	R0
      0x7C1C0200,  //  0030  CALL	R7	1
      0x8C1C0F0C,  //  0031  GETMET	R7	R7	K12
      0x5C240200,  //  0032  MOVE	R9	R1
      0x5C280400,  //  0033  MOVE	R10	R2
      0x7C1C0600,  //  0034  CALL	R7	3
      0x80040E00,  //  0035  RET	1	R7
      0x80000000,  //  0036  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Temp_Sensor
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Temp_Sensor,
    3,
    &be_class_Matter_Plugin_Device,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(770, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Temp_Sensor_read_attribute_closure) },
        { be_const_key_weak(get_temperature, 1), be_const_closure(Matter_Plugin_Temp_Sensor_get_temperature_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Temp_Sensor_init_closure) },
        { be_const_key_weak(CLUSTERS, 9), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(1026, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(every_second, 3), be_const_closure(Matter_Plugin_Temp_Sensor_every_second_closure) },
        { be_const_key_weak(tasmota_sensor_filter, 8), be_const_var(0) },
        { be_const_key_weak(tasmota_sensor_matcher, -1), be_const_var(1) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(Matter_Plugin_Temp_Sensor_parse_sensors_closure) },
        { be_const_key_weak(shadow_temperature, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_Plugin_Temp_Sensor)
);
/*******************************************************************/

void be_load_Matter_Plugin_Temp_Sensor_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Temp_Sensor);
    be_setglobal(vm, "Matter_Plugin_Temp_Sensor");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
