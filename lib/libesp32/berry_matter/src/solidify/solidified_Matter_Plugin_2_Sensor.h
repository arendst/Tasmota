/* Solidification of Matter_Plugin_2_Sensor.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor' ktab size: 37, total: 54 (saved 136 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor[37] = {
  /* K0   */  be_nested_str_weak(init),
  /* K1   */  be_nested_str_weak(add_read_sensors_schedule),
  /* K2   */  be_nested_str_weak(UPDATE_TIME),
  /* K3   */  be_nested_str_weak(find),
  /* K4   */  be_nested_str_weak(JSON_NAME),
  /* K5   */  be_nested_str_weak(bool),
  /* K6   */  be_nested_str_weak(shadow_value),
  /* K7   */  be_nested_str_weak(value_changed),
  /* K8   */  be_nested_str_weak(update_virtual),
  /* K9   */  be_nested_str_weak(parse_configuration),
  /* K10  */  be_nested_str_weak(tasmota_sensor_filter),
  /* K11  */  be_nested_str_weak(ARG),
  /* K12  */  be_nested_str_weak(tasmota_sensor_matcher),
  /* K13  */  be_nested_str_weak(tasmota),
  /* K14  */  be_nested_str_weak(Rule_Matcher),
  /* K15  */  be_nested_str_weak(parse),
  /* K16  */  be_nested_str_weak(temp_unit),
  /* K17  */  be_nested_str_weak(TEMP_C),
  /* K18  */  be_nested_str_weak(pressure_unit),
  /* K19  */  be_nested_str_weak(PRESSURE_HPA),
  /* K20  */  be_nested_str_weak(webserver),
  /* K21  */  be_nested_str_weak(get_name),
  /* K22  */  be_nested_str_weak(filter_name_html),
  /* K23  */  be_nested_str_weak(content_send),
  /* K24  */  be_nested_str_weak(PREFIX),
  /* K25  */  be_nested_str_weak(html_escape),
  /* K26  */  be_nested_str_weak(),
  /* K27  */  be_nested_str_weak(VIRTUAL),
  /* K28  */  be_nested_str_weak(match),
  /* K29  */  be_nested_str_weak(pre_value),
  /* K30  */  be_nested_str_weak(string),
  /* K31  */  be_nested_str_weak(split),
  /* K32  */  be_nested_str_weak(_X23),
  /* K33  */  be_const_int(0),
  /* K34  */  be_nested_str_weak(contains),
  /* K35  */  be_nested_str_weak(TempUnit),
  /* K36  */  be_nested_str_weak(PressureUnit),
};


extern const bclass be_class_Matter_Plugin_Sensor;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
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
** Solidified function: update_virtual
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_update_virtual,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x8C080303,  //  0000  GETMET	R2	R1	K3
      0x88100104,  //  0001  GETMBR	R4	R0	K4
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E000E,  //  0005  JMPF	R3	#0015
      0x600C0004,  //  0006  GETGBL	R3	G4
      0x5C100400,  //  0007  MOVE	R4	R2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x1C0C0705,  //  0009  EQ	R3	R3	K5
      0x780E0003,  //  000A  JMPF	R3	#000F
      0x600C0009,  //  000B  GETGBL	R3	G9
      0x5C100400,  //  000C  MOVE	R4	R2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x5C080600,  //  000E  MOVE	R2	R3
      0x880C0106,  //  000F  GETMBR	R3	R0	K6
      0x200C0602,  //  0010  NE	R3	R3	R2
      0x780E0002,  //  0011  JMPF	R3	#0015
      0x8C0C0107,  //  0012  GETMET	R3	R0	K7
      0x7C0C0200,  //  0013  CALL	R3	1
      0x90020C02,  //  0014  SETMBR	R0	K6	R2
      0x600C0003,  //  0015  GETGBL	R3	G3
      0x5C100000,  //  0016  MOVE	R4	R0
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C0708,  //  0018  GETMET	R3	R3	K8
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_parse_configuration,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080509,  //  0003  GETMET	R2	R2	K9
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080303,  //  0006  GETMET	R2	R1	K3
      0x8810010B,  //  0007  GETMBR	R4	R0	K11
      0x7C080400,  //  0008  CALL	R2	2
      0x90021402,  //  0009  SETMBR	R0	K10	R2
      0x8808010A,  //  000A  GETMBR	R2	R0	K10
      0x780A0005,  //  000B  JMPF	R2	#0012
      0xB80A1A00,  //  000C  GETNGBL	R2	K13
      0x8808050E,  //  000D  GETMBR	R2	R2	K14
      0x8C08050F,  //  000E  GETMET	R2	R2	K15
      0x8810010A,  //  000F  GETMBR	R4	R0	K10
      0x7C080400,  //  0010  CALL	R2	2
      0x90021802,  //  0011  SETMBR	R0	K12	R2
      0x88080111,  //  0012  GETMBR	R2	R0	K17
      0x90022002,  //  0013  SETMBR	R0	K16	R2
      0x88080113,  //  0014  GETMBR	R2	R0	K19
      0x90022402,  //  0015  SETMBR	R0	K18	R2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values_prefix
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
    be_str_weak(web_values_prefix),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA4062800,  //  0000  IMPORT	R1	K20
      0x8C080115,  //  0001  GETMET	R2	R0	K21
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0002,  //  0004  JMPT	R3	#0008
      0x8C0C0116,  //  0005  GETMET	R3	R0	K22
      0x7C0C0200,  //  0006  CALL	R3	1
      0x5C080600,  //  0007  MOVE	R2	R3
      0x8C0C0317,  //  0008  GETMET	R3	R1	K23
      0x60140018,  //  0009  GETGBL	R5	G24
      0x88180118,  //  000A  GETMBR	R6	R0	K24
      0x780A0003,  //  000B  JMPF	R2	#0010
      0x8C1C0319,  //  000C  GETMET	R7	R1	K25
      0x5C240400,  //  000D  MOVE	R9	R2
      0x7C1C0400,  //  000E  CALL	R7	2
      0x70020000,  //  000F  JMP		#0011
      0x581C001A,  //  0010  LDCONST	R7	K26
      0x7C140400,  //  0011  CALL	R5	2
      0x7C0C0400,  //  0012  CALL	R3	2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_parse_sensors,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x8808011B,  //  0000  GETMBR	R2	R0	K27
      0x740A001D,  //  0001  JMPT	R2	#0020
      0x8808010C,  //  0002  GETMBR	R2	R0	K12
      0x780A001B,  //  0003  JMPF	R2	#0020
      0x8808010C,  //  0004  GETMBR	R2	R0	K12
      0x8C08051C,  //  0005  GETMET	R2	R2	K28
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x600C000F,  //  0008  GETGBL	R3	G15
      0x5C100400,  //  0009  MOVE	R4	R2
      0x60140013,  //  000A  GETGBL	R5	G19
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0003,  //  000C  JMPF	R3	#0011
      0x8C0C0503,  //  000D  GETMET	R3	R2	K3
      0x88140104,  //  000E  GETMBR	R5	R0	K4
      0x7C0C0400,  //  000F  CALL	R3	2
      0x5C080600,  //  0010  MOVE	R2	R3
      0x8C0C011D,  //  0011  GETMET	R3	R0	K29
      0x6014000A,  //  0012  GETGBL	R5	G10
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C140200,  //  0014  CALL	R5	1
      0x7C0C0400,  //  0015  CALL	R3	2
      0x5C080600,  //  0016  MOVE	R2	R3
      0x4C0C0000,  //  0017  LDNIL	R3
      0x200C0403,  //  0018  NE	R3	R2	R3
      0x780E0005,  //  0019  JMPF	R3	#0020
      0x880C0106,  //  001A  GETMBR	R3	R0	K6
      0x200C0403,  //  001B  NE	R3	R2	R3
      0x780E0002,  //  001C  JMPF	R3	#0020
      0x8C0C0107,  //  001D  GETMET	R3	R0	K7
      0x7C0C0200,  //  001E  CALL	R3	1
      0x90020C02,  //  001F  SETMBR	R0	K6	R2
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: filter_name_html
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_filter_name_html,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
    be_str_weak(filter_name_html),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8804010A,  //  0000  GETMBR	R1	R0	K10
      0x78060009,  //  0001  JMPF	R1	#000C
      0xA4063C00,  //  0002  IMPORT	R1	K30
      0xA40A2800,  //  0003  IMPORT	R2	K20
      0x8C0C0519,  //  0004  GETMET	R3	R2	K25
      0x8C14031F,  //  0005  GETMET	R5	R1	K31
      0x881C010A,  //  0006  GETMBR	R7	R0	K10
      0x58200020,  //  0007  LDCONST	R8	K32
      0x7C140600,  //  0008  CALL	R5	3
      0x94140B21,  //  0009  GETIDX	R5	R5	K33
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80040600,  //  000B  RET	1	R3
      0x80063400,  //  000C  RET	1	K26
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_parse_status,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
    be_str_weak(parse_status),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x540E0009,  //  0000  LDINT	R3	10
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E001E,  //  0002  JMPF	R3	#0022
      0x8C0C0322,  //  0003  GETMET	R3	R1	K34
      0x58140023,  //  0004  LDCONST	R5	K35
      0x7C0C0400,  //  0005  CALL	R3	2
      0x780E0001,  //  0006  JMPF	R3	#0009
      0x940C0323,  //  0007  GETIDX	R3	R1	K35
      0x90022003,  //  0008  SETMBR	R0	K16	R3
      0x8C0C0322,  //  0009  GETMET	R3	R1	K34
      0x58140024,  //  000A  LDCONST	R5	K36
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0001,  //  000C  JMPF	R3	#000F
      0x940C0324,  //  000D  GETIDX	R3	R1	K36
      0x90022403,  //  000E  SETMBR	R0	K18	R3
      0x880C010C,  //  000F  GETMBR	R3	R0	K12
      0x780E0010,  //  0010  JMPF	R3	#0022
      0x8C0C011D,  //  0011  GETMET	R3	R0	K29
      0x6014000A,  //  0012  GETGBL	R5	G10
      0x8818010C,  //  0013  GETMBR	R6	R0	K12
      0x8C180D1C,  //  0014  GETMET	R6	R6	K28
      0x5C200200,  //  0015  MOVE	R8	R1
      0x7C180400,  //  0016  CALL	R6	2
      0x7C140200,  //  0017  CALL	R5	1
      0x7C0C0400,  //  0018  CALL	R3	2
      0x4C100000,  //  0019  LDNIL	R4
      0x20100604,  //  001A  NE	R4	R3	R4
      0x78120005,  //  001B  JMPF	R4	#0022
      0x88100106,  //  001C  GETMBR	R4	R0	K6
      0x20100604,  //  001D  NE	R4	R3	R4
      0x78120002,  //  001E  JMPF	R4	#0022
      0x8C100107,  //  001F  GETMET	R4	R0	K7
      0x7C100200,  //  0020  CALL	R4	1
      0x90020C03,  //  0021  SETMBR	R0	K6	R3
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor,
    5,
    &be_class_Matter_Plugin_Device,
    be_nested_map(22,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tasmota_sensor_filter, -1), be_const_var(0) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Filter_X20pattern) },
        { be_const_key_weak(parse_status, 4), be_const_closure(class_Matter_Plugin_Sensor_parse_status_closure) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Sensor_update_virtual_closure) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(class_Matter_Plugin_Sensor_parse_configuration_closure) },
        { be_const_key_weak(pressure_unit, -1), be_const_var(4) },
        { be_const_key_weak(tasmota_sensor_matcher, -1), be_const_var(1) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(filter) },
        { be_const_key_weak(PRESSURE_MMHG, -1), be_nested_str_weak(mmHg) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Sensor_init_closure) },
        { be_const_key_weak(TEMP_F, -1), be_nested_str_weak(F) },
        { be_const_key_weak(TEMP_C, -1), be_nested_str_weak(C) },
        { be_const_key_weak(temp_unit, 3), be_const_var(3) },
        { be_const_key_weak(UPDATE_CMD, 16), be_nested_str_weak(Status_X2010) },
        { be_const_key_weak(PRESSURE_HPA, 10), be_nested_str_weak(hPa) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(class_Matter_Plugin_Sensor_web_values_prefix_closure) },
        { be_const_key_weak(PRESSURE_INHG, -1), be_nested_str_weak(inHg) },
        { be_const_key_weak(JSON_NAME, 13), be_nested_str_weak() },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(class_Matter_Plugin_Sensor_parse_sensors_closure) },
        { be_const_key_weak(filter_name_html, -1), be_const_closure(class_Matter_Plugin_Sensor_filter_name_html_closure) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(shadow_value, 2), be_const_var(2) },
    })),
    be_str_weak(Matter_Plugin_Sensor)
);
/********************************************************************/
/* End of solidification */
