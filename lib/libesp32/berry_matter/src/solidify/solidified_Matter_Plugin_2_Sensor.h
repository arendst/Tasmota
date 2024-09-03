/* Solidification of Matter_Plugin_2_Sensor.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor' ktab size: 49, total: 70 (saved 168 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor[49] = {
  /* K0   */  be_nested_str_weak(find),
  /* K1   */  be_nested_str_weak(JSON_NAME),
  /* K2   */  be_nested_str_weak(bool),
  /* K3   */  be_nested_str_weak(shadow_value),
  /* K4   */  be_nested_str_weak(value_changed),
  /* K5   */  be_nested_str_weak(update_virtual),
  /* K6   */  be_nested_str_weak(contains),
  /* K7   */  be_nested_str_weak(TempUnit),
  /* K8   */  be_nested_str_weak(temp_unit),
  /* K9   */  be_nested_str_weak(PressureUnit),
  /* K10  */  be_nested_str_weak(pressure_unit),
  /* K11  */  be_nested_str_weak(tasmota_sensor_matcher),
  /* K12  */  be_nested_str_weak(pre_value),
  /* K13  */  be_nested_str_weak(match),
  /* K14  */  be_nested_str_weak(tasmota_sensor_filter),
  /* K15  */  be_nested_str_weak(string),
  /* K16  */  be_nested_str_weak(webserver),
  /* K17  */  be_nested_str_weak(html_escape),
  /* K18  */  be_nested_str_weak(split),
  /* K19  */  be_nested_str_weak(_X23),
  /* K20  */  be_const_int(0),
  /* K21  */  be_nested_str_weak(),
  /* K22  */  be_nested_str_weak(init),
  /* K23  */  be_nested_str_weak(add_read_sensors_schedule),
  /* K24  */  be_nested_str_weak(UPDATE_TIME),
  /* K25  */  be_nested_str_weak(get_name),
  /* K26  */  be_nested_str_weak(filter_name_html),
  /* K27  */  be_nested_str_weak(content_send),
  /* K28  */  be_nested_str_weak(PREFIX),
  /* K29  */  be_nested_str_weak(parse_configuration),
  /* K30  */  be_nested_str_weak(ARG),
  /* K31  */  be_nested_str_weak(tasmota),
  /* K32  */  be_nested_str_weak(Rule_Matcher),
  /* K33  */  be_nested_str_weak(parse),
  /* K34  */  be_nested_str_weak(TEMP_C),
  /* K35  */  be_nested_str_weak(PRESSURE_HPA),
  /* K36  */  be_nested_str_weak(VIRTUAL),
  /* K37  */  be_nested_str_weak(math),
  /* K38  */  be_nested_str_weak(log),
  /* K39  */  be_nested_str_weak(MTR_X3A_X20zigbee_received_X20Ox_X2504X_X20attr_list_X3D_X25s_X20type_X28attr_list_X29_X3D_X25s),
  /* K40  */  be_nested_str_weak(zigbee_mapper),
  /* K41  */  be_nested_str_weak(shortaddr),
  /* K42  */  be_const_int(3),
  /* K43  */  be_nested_str_weak(key),
  /* K44  */  be_nested_str_weak(ZIGBEE_NAME),
  /* K45  */  be_nested_str_weak(val),
  /* K46  */  be_nested_str_weak(MTR_X3A_X20_X5B_X2502X_X5D_X20_X25s_X20updated_X20_X25s),
  /* K47  */  be_nested_str_weak(endpoint),
  /* K48  */  be_const_int(1),
};


extern const bclass be_class_Matter_Plugin_Sensor;

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
      0x8C0C0306,  //  0003  GETMET	R3	R1	K6
      0x58140007,  //  0004  LDCONST	R5	K7
      0x7C0C0400,  //  0005  CALL	R3	2
      0x780E0001,  //  0006  JMPF	R3	#0009
      0x940C0307,  //  0007  GETIDX	R3	R1	K7
      0x90021003,  //  0008  SETMBR	R0	K8	R3
      0x8C0C0306,  //  0009  GETMET	R3	R1	K6
      0x58140009,  //  000A  LDCONST	R5	K9
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0001,  //  000C  JMPF	R3	#000F
      0x940C0309,  //  000D  GETIDX	R3	R1	K9
      0x90021403,  //  000E  SETMBR	R0	K10	R3
      0x880C010B,  //  000F  GETMBR	R3	R0	K11
      0x780E0010,  //  0010  JMPF	R3	#0022
      0x8C0C010C,  //  0011  GETMET	R3	R0	K12
      0x6014000A,  //  0012  GETGBL	R5	G10
      0x8818010B,  //  0013  GETMBR	R6	R0	K11
      0x8C180D0D,  //  0014  GETMET	R6	R6	K13
      0x5C200200,  //  0015  MOVE	R8	R1
      0x7C180400,  //  0016  CALL	R6	2
      0x7C140200,  //  0017  CALL	R5	1
      0x7C0C0400,  //  0018  CALL	R3	2
      0x4C100000,  //  0019  LDNIL	R4
      0x20100604,  //  001A  NE	R4	R3	R4
      0x78120005,  //  001B  JMPF	R4	#0022
      0x88100103,  //  001C  GETMBR	R4	R0	K3
      0x20100604,  //  001D  NE	R4	R3	R4
      0x78120002,  //  001E  JMPF	R4	#0022
      0x8C100104,  //  001F  GETMET	R4	R0	K4
      0x7C100200,  //  0020  CALL	R4	1
      0x90020603,  //  0021  SETMBR	R0	K3	R3
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_pre_value,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
    be_str_weak(pre_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80040200,  //  0000  RET	1	R1
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
      0x8804010E,  //  0000  GETMBR	R1	R0	K14
      0x78060009,  //  0001  JMPF	R1	#000C
      0xA4061E00,  //  0002  IMPORT	R1	K15
      0xA40A2000,  //  0003  IMPORT	R2	K16
      0x8C0C0511,  //  0004  GETMET	R3	R2	K17
      0x8C140312,  //  0005  GETMET	R5	R1	K18
      0x881C010E,  //  0006  GETMBR	R7	R0	K14
      0x58200013,  //  0007  LDCONST	R8	K19
      0x7C140600,  //  0008  CALL	R5	3
      0x94140B14,  //  0009  GETIDX	R5	R5	K20
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80040600,  //  000B  RET	1	R3
      0x80062A00,  //  000C  RET	1	K21
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: value_changed
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_value_changed,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
    be_str_weak(value_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


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
      0x8C100916,  //  0003  GETMET	R4	R4	K22
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x8C100317,  //  0008  GETMET	R4	R1	K23
      0x88180118,  //  0009  GETMBR	R6	R0	K24
      0x7C100400,  //  000A  CALL	R4	2
      0x80000000,  //  000B  RET	0
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
      0xA4062000,  //  0000  IMPORT	R1	K16
      0x8C080119,  //  0001  GETMET	R2	R0	K25
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0002,  //  0004  JMPT	R3	#0008
      0x8C0C011A,  //  0005  GETMET	R3	R0	K26
      0x7C0C0200,  //  0006  CALL	R3	1
      0x5C080600,  //  0007  MOVE	R2	R3
      0x8C0C031B,  //  0008  GETMET	R3	R1	K27
      0x60140018,  //  0009  GETGBL	R5	G24
      0x8818011C,  //  000A  GETMBR	R6	R0	K28
      0x780A0003,  //  000B  JMPF	R2	#0010
      0x8C1C0311,  //  000C  GETMET	R7	R1	K17
      0x5C240400,  //  000D  MOVE	R9	R2
      0x7C1C0400,  //  000E  CALL	R7	2
      0x70020000,  //  000F  JMP		#0011
      0x581C0015,  //  0010  LDCONST	R7	K21
      0x7C140400,  //  0011  CALL	R5	2
      0x7C0C0400,  //  0012  CALL	R3	2
      0x80000000,  //  0013  RET	0
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
      0x8C08051D,  //  0003  GETMET	R2	R2	K29
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080300,  //  0006  GETMET	R2	R1	K0
      0x8810011E,  //  0007  GETMBR	R4	R0	K30
      0x7C080400,  //  0008  CALL	R2	2
      0x90021C02,  //  0009  SETMBR	R0	K14	R2
      0x8808010E,  //  000A  GETMBR	R2	R0	K14
      0x780A0005,  //  000B  JMPF	R2	#0012
      0xB80A3E00,  //  000C  GETNGBL	R2	K31
      0x88080520,  //  000D  GETMBR	R2	R2	K32
      0x8C080521,  //  000E  GETMET	R2	R2	K33
      0x8810010E,  //  000F  GETMBR	R4	R0	K14
      0x7C080400,  //  0010  CALL	R2	2
      0x90021602,  //  0011  SETMBR	R0	K11	R2
      0x88080122,  //  0012  GETMBR	R2	R0	K34
      0x90021002,  //  0013  SETMBR	R0	K8	R2
      0x88080123,  //  0014  GETMBR	R2	R0	K35
      0x90021402,  //  0015  SETMBR	R0	K10	R2
      0x80000000,  //  0016  RET	0
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
      0x88080124,  //  0000  GETMBR	R2	R0	K36
      0x740A001D,  //  0001  JMPT	R2	#0020
      0x8808010B,  //  0002  GETMBR	R2	R0	K11
      0x780A001B,  //  0003  JMPF	R2	#0020
      0x8808010B,  //  0004  GETMBR	R2	R0	K11
      0x8C08050D,  //  0005  GETMET	R2	R2	K13
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x600C000F,  //  0008  GETGBL	R3	G15
      0x5C100400,  //  0009  MOVE	R4	R2
      0x60140013,  //  000A  GETGBL	R5	G19
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0003,  //  000C  JMPF	R3	#0011
      0x8C0C0500,  //  000D  GETMET	R3	R2	K0
      0x88140101,  //  000E  GETMBR	R5	R0	K1
      0x7C0C0400,  //  000F  CALL	R3	2
      0x5C080600,  //  0010  MOVE	R2	R3
      0x8C0C010C,  //  0011  GETMET	R3	R0	K12
      0x6014000A,  //  0012  GETGBL	R5	G10
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C140200,  //  0014  CALL	R5	1
      0x7C0C0400,  //  0015  CALL	R3	2
      0x5C080600,  //  0016  MOVE	R2	R3
      0x4C0C0000,  //  0017  LDNIL	R3
      0x200C0403,  //  0018  NE	R3	R2	R3
      0x780E0005,  //  0019  JMPF	R3	#0020
      0x880C0103,  //  001A  GETMBR	R3	R0	K3
      0x200C0403,  //  001B  NE	R3	R2	R3
      0x780E0002,  //  001C  JMPF	R3	#0020
      0x8C0C0104,  //  001D  GETMET	R3	R0	K4
      0x7C0C0200,  //  001E  CALL	R3	1
      0x90020602,  //  001F  SETMBR	R0	K3	R2
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: zigbee_received
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_zigbee_received,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor,     /* shared constants */
    be_str_weak(zigbee_received),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0xA40E4A00,  //  0000  IMPORT	R3	K37
      0xB8124C00,  //  0001  GETNGBL	R4	K38
      0x60140018,  //  0002  GETGBL	R5	G24
      0x58180027,  //  0003  LDCONST	R6	K39
      0x881C0128,  //  0004  GETMBR	R7	R0	K40
      0x881C0F29,  //  0005  GETMBR	R7	R7	K41
      0x5C200400,  //  0006  MOVE	R8	R2
      0x60240004,  //  0007  GETGBL	R9	G4
      0x5C280400,  //  0008  MOVE	R10	R2
      0x7C240200,  //  0009  CALL	R9	1
      0x7C140800,  //  000A  CALL	R5	4
      0x5818002A,  //  000B  LDCONST	R6	K42
      0x7C100400,  //  000C  CALL	R4	2
      0x58100014,  //  000D  LDCONST	R4	K20
      0x6014000C,  //  000E  GETGBL	R5	G12
      0x5C180400,  //  000F  MOVE	R6	R2
      0x7C140200,  //  0010  CALL	R5	1
      0x14140805,  //  0011  LT	R5	R4	R5
      0x7816001B,  //  0012  JMPF	R5	#002F
      0x94140404,  //  0013  GETIDX	R5	R2	R4
      0x88180B2B,  //  0014  GETMBR	R6	R5	K43
      0x881C012C,  //  0015  GETMBR	R7	R0	K44
      0x1C180C07,  //  0016  EQ	R6	R6	R7
      0x781A0014,  //  0017  JMPF	R6	#002D
      0x8C18010C,  //  0018  GETMET	R6	R0	K12
      0x88200B2D,  //  0019  GETMBR	R8	R5	K45
      0x7C180400,  //  001A  CALL	R6	2
      0x601C0013,  //  001B  GETGBL	R7	G19
      0x7C1C0000,  //  001C  CALL	R7	0
      0x88200101,  //  001D  GETMBR	R8	R0	K1
      0x981C1006,  //  001E  SETIDX	R7	R8	R6
      0x8C200105,  //  001F  GETMET	R8	R0	K5
      0x5C280E00,  //  0020  MOVE	R10	R7
      0x7C200400,  //  0021  CALL	R8	2
      0xB8224C00,  //  0022  GETNGBL	R8	K38
      0x60240018,  //  0023  GETGBL	R9	G24
      0x5828002E,  //  0024  LDCONST	R10	K46
      0x882C012F,  //  0025  GETMBR	R11	R0	K47
      0x88300101,  //  0026  GETMBR	R12	R0	K1
      0x5C340E00,  //  0027  MOVE	R13	R7
      0x7C240800,  //  0028  CALL	R9	4
      0x5828002A,  //  0029  LDCONST	R10	K42
      0x7C200400,  //  002A  CALL	R8	2
      0x4C200000,  //  002B  LDNIL	R8
      0x80041000,  //  002C  RET	1	R8
      0x00100930,  //  002D  ADD	R4	R4	K48
      0x7001FFDE,  //  002E  JMP		#000E
      0x80000000,  //  002F  RET	0
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
    be_nested_map(25,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak() },
        { be_const_key_weak(pre_value, -1), be_const_closure(class_Matter_Plugin_Sensor_pre_value_closure) },
        { be_const_key_weak(zigbee_received, -1), be_const_closure(class_Matter_Plugin_Sensor_zigbee_received_closure) },
        { be_const_key_weak(parse_status, 2), be_const_closure(class_Matter_Plugin_Sensor_parse_status_closure) },
        { be_const_key_weak(TEMP_C, -1), be_nested_str_weak(C) },
        { be_const_key_weak(temp_unit, 23), be_const_var(3) },
        { be_const_key_weak(UPDATE_TIME, 10), be_const_int(5000) },
        { be_const_key_weak(TEMP_F, -1), be_nested_str_weak(F) },
        { be_const_key_weak(pressure_unit, -1), be_const_var(4) },
        { be_const_key_weak(UPDATE_CMD, -1), be_nested_str_weak(Status_X2010) },
        { be_const_key_weak(tasmota_sensor_filter, 1), be_const_var(0) },
        { be_const_key_weak(value_changed, 7), be_const_closure(class_Matter_Plugin_Sensor_value_changed_closure) },
        { be_const_key_weak(PRESSURE_INHG, -1), be_nested_str_weak(inHg) },
        { be_const_key_weak(parse_configuration, 22), be_const_closure(class_Matter_Plugin_Sensor_parse_configuration_closure) },
        { be_const_key_weak(tasmota_sensor_matcher, -1), be_const_var(1) },
        { be_const_key_weak(update_virtual, 14), be_const_closure(class_Matter_Plugin_Sensor_update_virtual_closure) },
        { be_const_key_weak(PRESSURE_MMHG, -1), be_nested_str_weak(mmHg) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(class_Matter_Plugin_Sensor_web_values_prefix_closure) },
        { be_const_key_weak(shadow_value, -1), be_const_var(2) },
        { be_const_key_weak(filter_name_html, 13), be_const_closure(class_Matter_Plugin_Sensor_filter_name_html_closure) },
        { be_const_key_weak(ARG_HINT, 12), be_nested_str_weak(Filter_X20pattern) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(class_Matter_Plugin_Sensor_parse_sensors_closure) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(filter) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Sensor_init_closure) },
        { be_const_key_weak(PRESSURE_HPA, -1), be_nested_str_weak(hPa) },
    })),
    be_str_weak(Matter_Plugin_Sensor)
);
/********************************************************************/
/* End of solidification */
