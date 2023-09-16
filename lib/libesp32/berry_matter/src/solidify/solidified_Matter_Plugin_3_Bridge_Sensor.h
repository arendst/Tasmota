/* Solidification of Matter_Plugin_3_Bridge_Sensor.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor;

/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_parse_configuration,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_sensor_filter),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    /* K3   */  be_nested_str_weak(tasmota_sensor_matcher),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(Rule_Matcher),
    /* K6   */  be_nested_str_weak(parse),
    /* K7   */  be_nested_str_weak(temp_unit),
    /* K8   */  be_nested_str_weak(TEMP_C),
    /* K9   */  be_nested_str_weak(pressure_unit),
    /* K10  */  be_nested_str_weak(PRESSURE_HPA),
    }),
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
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
      0x88080108,  //  000C  GETMBR	R2	R0	K8
      0x90020E02,  //  000D  SETMBR	R0	K7	R2
      0x8808010A,  //  000E  GETMBR	R2	R0	K10
      0x90021202,  //  000F  SETMBR	R0	K9	R2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: value_changed
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_value_changed,   /* name */
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
** Solidified function: web_values_prefix
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(get_name),
    /* K2   */  be_nested_str_weak(filter_name_html),
    /* K3   */  be_nested_str_weak(content_send),
    /* K4   */  be_nested_str_weak(PREFIX),
    /* K5   */  be_nested_str_weak(html_escape),
    /* K6   */  be_nested_str_weak(),
    }),
    be_str_weak(web_values_prefix),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0002,  //  0004  JMPT	R3	#0008
      0x8C0C0102,  //  0005  GETMET	R3	R0	K2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x5C080600,  //  0007  MOVE	R2	R3
      0x8C0C0303,  //  0008  GETMET	R3	R1	K3
      0x60140018,  //  0009  GETGBL	R5	G24
      0x88180104,  //  000A  GETMBR	R6	R0	K4
      0x780A0003,  //  000B  JMPF	R2	#0010
      0x8C1C0305,  //  000C  GETMET	R7	R1	K5
      0x5C240400,  //  000D  MOVE	R9	R2
      0x7C1C0400,  //  000E  CALL	R7	2
      0x70020000,  //  000F  JMP		#0011
      0x581C0006,  //  0010  LDCONST	R7	K6
      0x7C140400,  //  0011  CALL	R5	2
      0x7C0C0400,  //  0012  CALL	R3	2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_update
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_parse_update,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(contains),
    /* K1   */  be_nested_str_weak(TempUnit),
    /* K2   */  be_nested_str_weak(temp_unit),
    /* K3   */  be_nested_str_weak(PressureUnit),
    /* K4   */  be_nested_str_weak(pressure_unit),
    /* K5   */  be_nested_str_weak(tasmota_sensor_matcher),
    /* K6   */  be_nested_str_weak(pre_value),
    /* K7   */  be_nested_str_weak(match),
    /* K8   */  be_nested_str_weak(shadow_value),
    /* K9   */  be_nested_str_weak(value_changed),
    }),
    be_str_weak(parse_update),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x540E0007,  //  0000  LDINT	R3	8
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E001E,  //  0002  JMPF	R3	#0022
      0x8C0C0300,  //  0003  GETMET	R3	R1	K0
      0x58140001,  //  0004  LDCONST	R5	K1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x780E0001,  //  0006  JMPF	R3	#0009
      0x940C0301,  //  0007  GETIDX	R3	R1	K1
      0x90020403,  //  0008  SETMBR	R0	K2	R3
      0x8C0C0300,  //  0009  GETMET	R3	R1	K0
      0x58140003,  //  000A  LDCONST	R5	K3
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0001,  //  000C  JMPF	R3	#000F
      0x940C0303,  //  000D  GETIDX	R3	R1	K3
      0x90020803,  //  000E  SETMBR	R0	K4	R3
      0x880C0105,  //  000F  GETMBR	R3	R0	K5
      0x780E0010,  //  0010  JMPF	R3	#0022
      0x8C0C0106,  //  0011  GETMET	R3	R0	K6
      0x6014000A,  //  0012  GETGBL	R5	G10
      0x88180105,  //  0013  GETMBR	R6	R0	K5
      0x8C180D07,  //  0014  GETMET	R6	R6	K7
      0x5C200200,  //  0015  MOVE	R8	R1
      0x7C180400,  //  0016  CALL	R6	2
      0x7C140200,  //  0017  CALL	R5	1
      0x7C0C0400,  //  0018  CALL	R3	2
      0x4C100000,  //  0019  LDNIL	R4
      0x20100604,  //  001A  NE	R4	R3	R4
      0x78120005,  //  001B  JMPF	R4	#0022
      0x88100108,  //  001C  GETMBR	R4	R0	K8
      0x20100604,  //  001D  NE	R4	R3	R4
      0x78120002,  //  001E  JMPF	R4	#0022
      0x8C100109,  //  001F  GETMET	R4	R0	K9
      0x7C100200,  //  0020  CALL	R4	1
      0x90021003,  //  0021  SETMBR	R0	K8	R3
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: filter_name_html
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_filter_name_html,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_sensor_filter),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(webserver),
    /* K3   */  be_nested_str_weak(html_escape),
    /* K4   */  be_nested_str_weak(split),
    /* K5   */  be_nested_str_weak(_X23),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(),
    }),
    be_str_weak(filter_name_html),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060009,  //  0001  JMPF	R1	#000C
      0xA4060200,  //  0002  IMPORT	R1	K1
      0xA40A0400,  //  0003  IMPORT	R2	K2
      0x8C0C0503,  //  0004  GETMET	R3	R2	K3
      0x8C140304,  //  0005  GETMET	R5	R1	K4
      0x881C0100,  //  0006  GETMBR	R7	R0	K0
      0x58200005,  //  0007  LDCONST	R8	K5
      0x7C140600,  //  0008  CALL	R5	3
      0x94140B06,  //  0009  GETIDX	R5	R5	K6
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80040600,  //  000B  RET	1	R3
      0x80060E00,  //  000C  RET	1	K7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_pre_value,   /* name */
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
** Solidified class: Matter_Plugin_Bridge_Sensor
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_HTTP;
be_local_class(Matter_Plugin_Bridge_Sensor,
    5,
    &be_class_Matter_Plugin_Bridge_HTTP,
    be_nested_map(22,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tasmota_sensor_filter, -1), be_const_var(0) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Filter_X20pattern) },
        { be_const_key_weak(shadow_value, -1), be_const_var(2) },
        { be_const_key_weak(pre_value, 10), be_const_closure(Matter_Plugin_Bridge_Sensor_pre_value_closure) },
        { be_const_key_weak(ARG_HTTP, -1), be_nested_str_weak(url) },
        { be_const_key_weak(pressure_unit, -1), be_const_var(4) },
        { be_const_key_weak(tasmota_sensor_matcher, -1), be_const_var(1) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(filter) },
        { be_const_key_weak(PRESSURE_MMHG, -1), be_nested_str_weak(mmHg) },
        { be_const_key_weak(value_changed, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_value_changed_closure) },
        { be_const_key_weak(parse_update, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_parse_update_closure) },
        { be_const_key_weak(TEMP_C, -1), be_nested_str_weak(C) },
        { be_const_key_weak(temp_unit, 4), be_const_var(3) },
        { be_const_key_weak(PRESSURE_INHG, -1), be_nested_str_weak(inHg) },
        { be_const_key_weak(PRESSURE_HPA, 18), be_nested_str_weak(hPa) },
        { be_const_key_weak(PROBE_TIMEOUT, 16), be_const_int(1700) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_web_values_prefix_closure) },
        { be_const_key_weak(UPDATE_CMD, 13), be_nested_str_weak(Status_X208) },
        { be_const_key_weak(TEMP_F, -1), be_nested_str_weak(F) },
        { be_const_key_weak(filter_name_html, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_filter_name_html_closure) },
        { be_const_key_weak(UPDATE_TIME, 3), be_const_int(5000) },
        { be_const_key_weak(parse_configuration, 2), be_const_closure(Matter_Plugin_Bridge_Sensor_parse_configuration_closure) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_Sensor_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_Sensor);
    be_setglobal(vm, "Matter_Plugin_Bridge_Sensor");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
