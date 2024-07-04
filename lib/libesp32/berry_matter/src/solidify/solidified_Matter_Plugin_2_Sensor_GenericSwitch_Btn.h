/* Solidification of Matter_Plugin_2_Sensor_GenericSwitch_Btn.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_GenericSwitch_Btn;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_GenericSwitch_Btn;
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_GenericSwitch_Btn, 
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(set),
    /* K6   */  be_nested_str_weak(U1),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(set_or_nil),
    /* K10  */  be_nested_str_weak(shadow_position),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E003A,  //  0004  LDINT	R7	59
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0024,  //  0006  JMPF	R7	#002C
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E0005,  //  0008  JMPF	R7	#000F
      0x8C1C0705,  //  0009  GETMET	R7	R3	K5
      0x88240906,  //  000A  GETMBR	R9	R4	K6
      0x58280007,  //  000B  LDCONST	R10	K7
      0x7C1C0600,  //  000C  CALL	R7	3
      0x80040E00,  //  000D  RET	1	R7
      0x7002001C,  //  000E  JMP		#002C
      0x1C1C0D08,  //  000F  EQ	R7	R6	K8
      0x781E0005,  //  0010  JMPF	R7	#0017
      0x8C1C0709,  //  0011  GETMET	R7	R3	K9
      0x88240906,  //  0012  GETMBR	R9	R4	K6
      0x8828010A,  //  0013  GETMBR	R10	R0	K10
      0x7C1C0600,  //  0014  CALL	R7	3
      0x80040E00,  //  0015  RET	1	R7
      0x70020014,  //  0016  JMP		#002C
      0x1C1C0D07,  //  0017  EQ	R7	R6	K7
      0x781E0005,  //  0018  JMPF	R7	#001F
      0x8C1C0705,  //  0019  GETMET	R7	R3	K5
      0x88240906,  //  001A  GETMBR	R9	R4	K6
      0x542A0004,  //  001B  LDINT	R10	5
      0x7C1C0600,  //  001C  CALL	R7	3
      0x80040E00,  //  001D  RET	1	R7
      0x7002000C,  //  001E  JMP		#002C
      0x541EFFFB,  //  001F  LDINT	R7	65532
      0x1C1C0C07,  //  0020  EQ	R7	R6	R7
      0x781E0009,  //  0021  JMPF	R7	#002C
      0x8C1C0705,  //  0022  GETMET	R7	R3	K5
      0x8824090B,  //  0023  GETMBR	R9	R4	K11
      0x542A0003,  //  0024  LDINT	R10	4
      0x302A0E0A,  //  0025  OR	R10	K7	R10
      0x542E0007,  //  0026  LDINT	R11	8
      0x3028140B,  //  0027  OR	R10	R10	R11
      0x542E000F,  //  0028  LDINT	R11	16
      0x3028140B,  //  0029  OR	R10	R10	R11
      0x7C1C0600,  //  002A  CALL	R7	3
      0x80040E00,  //  002B  RET	1	R7
      0x601C0003,  //  002C  GETGBL	R7	G3
      0x5C200000,  //  002D  MOVE	R8	R0
      0x7C1C0200,  //  002E  CALL	R7	1
      0x8C1C0F0C,  //  002F  GETMET	R7	R7	K12
      0x5C240200,  //  0030  MOVE	R9	R1
      0x5C280400,  //  0031  MOVE	R10	R2
      0x5C2C0600,  //  0032  MOVE	R11	R3
      0x7C1C0800,  //  0033  CALL	R7	4
      0x80040E00,  //  0034  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL, 
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(_X3Clambda_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040009,  //  0000  GETGBL	R1	G9
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_position
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_GenericSwitch_Btn;
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_set_position,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_GenericSwitch_Btn, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(shadow_position),
    /* K1   */  be_nested_str_weak(attribute_updated),
    /* K2   */  be_const_int(1),
    }),
    be_str_weak(set_position),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0004,  //  0002  JMPF	R2	#0008
      0x8C080101,  //  0003  GETMET	R2	R0	K1
      0x5412003A,  //  0004  LDINT	R4	59
      0x58140002,  //  0005  LDCONST	R5	K2
      0x7C080600,  //  0006  CALL	R2	3
      0x90020001,  //  0007  SETMBR	R0	K0	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_GenericSwitch_Btn;
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_parse_configuration,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_GenericSwitch_Btn, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_switch_index),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x8C0C0301,  //  0001  GETMET	R3	R1	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x58180003,  //  0003  LDCONST	R6	K3
      0x7C0C0600,  //  0004  CALL	R3	3
      0x7C080200,  //  0005  CALL	R2	1
      0x90020002,  //  0006  SETMBR	R0	K0	R2
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x18080504,  //  0008  LE	R2	R2	K4
      0x780A0000,  //  0009  JMPF	R2	#000B
      0x90020103,  //  000A  SETMBR	R0	K0	K3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: append_state_json
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_GenericSwitch_Btn;
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_append_state_json,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_GenericSwitch_Btn, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_X2C_X22Switch_X22_X3A_X25s),
    /* K1   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(append_state_json),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080000,  //  0001  LDCONST	R2	K0
      0x600C0009,  //  0002  GETGBL	R3	G9
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0004  CALL	R3	1
      0x7C040400,  //  0005  CALL	R1	2
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: button_handler
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_GenericSwitch_Btn;
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_button_handler,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_GenericSwitch_Btn, 
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(set_position),
    /* K3   */  be_nested_str_weak(publish_event),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(EVENT_INFO),
    /* K6   */  be_nested_str_weak(TLV),
    /* K7   */  be_nested_str_weak(Matter_TLV_item),
    /* K8   */  be_nested_str_weak(set),
    /* K9   */  be_nested_str_weak(U1),
    /* K10  */  be_const_int(3),
    /* K11  */  be_const_int(2),
    }),
    be_str_weak(button_handler),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0x1C140500,  //  0000  EQ	R5	R2	K0
      0x74160001,  //  0001  JMPT	R5	#0004
      0x1C140501,  //  0002  EQ	R5	R2	K1
      0x78160002,  //  0003  JMPF	R5	#0007
      0x8C140102,  //  0004  GETMET	R5	R0	K2
      0x5C1C0600,  //  0005  MOVE	R7	R3
      0x7C140400,  //  0006  CALL	R5	2
      0x1C140501,  //  0007  EQ	R5	R2	K1
      0x78160040,  //  0008  JMPF	R5	#004A
      0x780E0010,  //  0009  JMPF	R3	#001B
      0x8C140103,  //  000A  GETMET	R5	R0	K3
      0x541E003A,  //  000B  LDINT	R7	59
      0x58200001,  //  000C  LDCONST	R8	K1
      0xB8260800,  //  000D  GETNGBL	R9	K4
      0x88241305,  //  000E  GETMBR	R9	R9	K5
      0xB82A0800,  //  000F  GETNGBL	R10	K4
      0x88281506,  //  0010  GETMBR	R10	R10	K6
      0x8C281507,  //  0011  GETMET	R10	R10	K7
      0x7C280200,  //  0012  CALL	R10	1
      0x8C281508,  //  0013  GETMET	R10	R10	K8
      0xB8320800,  //  0014  GETNGBL	R12	K4
      0x88301906,  //  0015  GETMBR	R12	R12	K6
      0x88301909,  //  0016  GETMBR	R12	R12	K9
      0x58340001,  //  0017  LDCONST	R13	K1
      0x7C280600,  //  0018  CALL	R10	3
      0x7C140A00,  //  0019  CALL	R5	5
      0x7002000F,  //  001A  JMP		#002B
      0x8C140103,  //  001B  GETMET	R5	R0	K3
      0x541E003A,  //  001C  LDINT	R7	59
      0x5820000A,  //  001D  LDCONST	R8	K10
      0xB8260800,  //  001E  GETNGBL	R9	K4
      0x88241305,  //  001F  GETMBR	R9	R9	K5
      0xB82A0800,  //  0020  GETNGBL	R10	K4
      0x88281506,  //  0021  GETMBR	R10	R10	K6
      0x8C281507,  //  0022  GETMET	R10	R10	K7
      0x7C280200,  //  0023  CALL	R10	1
      0x8C281508,  //  0024  GETMET	R10	R10	K8
      0xB8320800,  //  0025  GETNGBL	R12	K4
      0x88301906,  //  0026  GETMBR	R12	R12	K6
      0x88301909,  //  0027  GETMBR	R12	R12	K9
      0x58340001,  //  0028  LDCONST	R13	K1
      0x7C280600,  //  0029  CALL	R10	3
      0x7C140A00,  //  002A  CALL	R5	5
      0x1C140701,  //  002B  EQ	R5	R3	K1
      0x7816001B,  //  002C  JMPF	R5	#0049
      0x24140900,  //  002D  GT	R5	R4	K0
      0x78160019,  //  002E  JMPF	R5	#0049
      0x8C140103,  //  002F  GETMET	R5	R0	K3
      0x541E003A,  //  0030  LDINT	R7	59
      0x54220004,  //  0031  LDINT	R8	5
      0xB8260800,  //  0032  GETNGBL	R9	K4
      0x88241305,  //  0033  GETMBR	R9	R9	K5
      0xB82A0800,  //  0034  GETNGBL	R10	K4
      0x88281506,  //  0035  GETMBR	R10	R10	K6
      0x8C281507,  //  0036  GETMET	R10	R10	K7
      0x7C280200,  //  0037  CALL	R10	1
      0x8C281508,  //  0038  GETMET	R10	R10	K8
      0xB8320800,  //  0039  GETNGBL	R12	K4
      0x88301906,  //  003A  GETMBR	R12	R12	K6
      0x88301909,  //  003B  GETMBR	R12	R12	K9
      0x58340001,  //  003C  LDCONST	R13	K1
      0x7C280600,  //  003D  CALL	R10	3
      0xB82E0800,  //  003E  GETNGBL	R11	K4
      0x882C1706,  //  003F  GETMBR	R11	R11	K6
      0x8C2C1707,  //  0040  GETMET	R11	R11	K7
      0x7C2C0200,  //  0041  CALL	R11	1
      0x8C2C1708,  //  0042  GETMET	R11	R11	K8
      0xB8360800,  //  0043  GETNGBL	R13	K4
      0x88341B06,  //  0044  GETMBR	R13	R13	K6
      0x88341B09,  //  0045  GETMBR	R13	R13	K9
      0x00380901,  //  0046  ADD	R14	R4	K1
      0x7C2C0600,  //  0047  CALL	R11	3
      0x7C140C00,  //  0048  CALL	R5	6
      0x7002001D,  //  0049  JMP		#0068
      0x1C14050B,  //  004A  EQ	R5	R2	K11
      0x7816001B,  //  004B  JMPF	R5	#0068
      0x24140900,  //  004C  GT	R5	R4	K0
      0x78160019,  //  004D  JMPF	R5	#0068
      0x8C140103,  //  004E  GETMET	R5	R0	K3
      0x541E003A,  //  004F  LDINT	R7	59
      0x54220005,  //  0050  LDINT	R8	6
      0xB8260800,  //  0051  GETNGBL	R9	K4
      0x88241305,  //  0052  GETMBR	R9	R9	K5
      0xB82A0800,  //  0053  GETNGBL	R10	K4
      0x88281506,  //  0054  GETMBR	R10	R10	K6
      0x8C281507,  //  0055  GETMET	R10	R10	K7
      0x7C280200,  //  0056  CALL	R10	1
      0x8C281508,  //  0057  GETMET	R10	R10	K8
      0xB8320800,  //  0058  GETNGBL	R12	K4
      0x88301906,  //  0059  GETMBR	R12	R12	K6
      0x88301909,  //  005A  GETMBR	R12	R12	K9
      0x58340001,  //  005B  LDCONST	R13	K1
      0x7C280600,  //  005C  CALL	R10	3
      0xB82E0800,  //  005D  GETNGBL	R11	K4
      0x882C1706,  //  005E  GETMBR	R11	R11	K6
      0x8C2C1707,  //  005F  GETMET	R11	R11	K7
      0x7C2C0200,  //  0060  CALL	R11	1
      0x8C2C1708,  //  0061  GETMET	R11	R11	K8
      0xB8360800,  //  0062  GETNGBL	R13	K4
      0x88341B06,  //  0063  GETMBR	R13	R13	K6
      0x88341B09,  //  0064  GETMBR	R13	R13	K9
      0x5C380800,  //  0065  MOVE	R14	R4
      0x7C2C0600,  //  0066  CALL	R11	3
      0x7C140C00,  //  0067  CALL	R5	6
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_GenericSwitch_Btn
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor_GenericSwitch_Btn,
    2,
    &be_class_Matter_Plugin_Device,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, 4), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_read_attribute_closure) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(button) },
        { be_const_key_weak(ARG_TYPE, 6), be_const_static_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn__X3Clambda_X3E_closure) },
        { be_const_key_weak(TYPE, 9), be_nested_str_weak(gensw_btn) },
        { be_const_key_weak(button_handler, 13), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_button_handler_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Generic_X20Switch_X2FButton) },
        { be_const_key_weak(append_state_json, 12), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_append_state_json_closure) },
        { be_const_key_weak(set_position, -1), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_set_position_closure) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_parse_configuration_closure) },
        { be_const_key_weak(shadow_position, -1), be_const_var(1) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(15, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(ARG_HINT, 8), be_nested_str_weak(Button_X3Cx_X3E_X20number) },
        { be_const_key_weak(tasmota_switch_index, -1), be_const_var(0) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(59, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Sensor_GenericSwitch_Btn)
);
/********************************************************************/
/* End of solidification */
