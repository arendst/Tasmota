/* Solidification of Matter_Plugin_2_Sensor_GenericSwitch_Btn.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_GenericSwitch_Btn' ktab size: 25, total: 36 (saved 88 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_GenericSwitch_Btn[25] = {
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
  /* K13  */  be_nested_str_weak(attribute_updated),
  /* K14  */  be_nested_str_weak(parse_configuration),
  /* K15  */  be_nested_str_weak(tasmota_switch_index),
  /* K16  */  be_nested_str_weak(find),
  /* K17  */  be_nested_str_weak(ARG),
  /* K18  */  be_nested_str_weak(_X2C_X22Switch_X22_X3A_X25s),
  /* K19  */  be_nested_str_weak(shadow_onoff),
  /* K20  */  be_nested_str_weak(set_position),
  /* K21  */  be_nested_str_weak(publish_event),
  /* K22  */  be_nested_str_weak(EVENT_INFO),
  /* K23  */  be_nested_str_weak(Matter_TLV_item),
  /* K24  */  be_const_int(3),
};


extern const bclass be_class_Matter_Plugin_Sensor_GenericSwitch_Btn;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_GenericSwitch_Btn,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E003A,  //  0004  LDINT	R7	59
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E001F,  //  0006  JMPF	R7	#0027
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E0005,  //  0008  JMPF	R7	#000F
      0x8C1C0705,  //  0009  GETMET	R7	R3	K5
      0x88240906,  //  000A  GETMBR	R9	R4	K6
      0x58280007,  //  000B  LDCONST	R10	K7
      0x7C1C0600,  //  000C  CALL	R7	3
      0x80040E00,  //  000D  RET	1	R7
      0x70020017,  //  000E  JMP		#0027
      0x1C1C0D08,  //  000F  EQ	R7	R6	K8
      0x781E0005,  //  0010  JMPF	R7	#0017
      0x8C1C0709,  //  0011  GETMET	R7	R3	K9
      0x88240906,  //  0012  GETMBR	R9	R4	K6
      0x8828010A,  //  0013  GETMBR	R10	R0	K10
      0x7C1C0600,  //  0014  CALL	R7	3
      0x80040E00,  //  0015  RET	1	R7
      0x7002000F,  //  0016  JMP		#0027
      0x1C1C0D07,  //  0017  EQ	R7	R6	K7
      0x781E0005,  //  0018  JMPF	R7	#001F
      0x8C1C0705,  //  0019  GETMET	R7	R3	K5
      0x88240906,  //  001A  GETMBR	R9	R4	K6
      0x542A0004,  //  001B  LDINT	R10	5
      0x7C1C0600,  //  001C  CALL	R7	3
      0x80040E00,  //  001D  RET	1	R7
      0x70020007,  //  001E  JMP		#0027
      0x541EFFFB,  //  001F  LDINT	R7	65532
      0x1C1C0C07,  //  0020  EQ	R7	R6	R7
      0x781E0004,  //  0021  JMPF	R7	#0027
      0x8C1C0705,  //  0022  GETMET	R7	R3	K5
      0x8824090B,  //  0023  GETMBR	R9	R4	K11
      0x542A0015,  //  0024  LDINT	R10	22
      0x7C1C0600,  //  0025  CALL	R7	3
      0x80040E00,  //  0026  RET	1	R7
      0x601C0003,  //  0027  GETGBL	R7	G3
      0x5C200000,  //  0028  MOVE	R8	R0
      0x7C1C0200,  //  0029  CALL	R7	1
      0x8C1C0F0C,  //  002A  GETMET	R7	R7	K12
      0x5C240200,  //  002B  MOVE	R9	R1
      0x5C280400,  //  002C  MOVE	R10	R2
      0x5C2C0600,  //  002D  MOVE	R11	R3
      0x7C1C0800,  //  002E  CALL	R7	4
      0x80040E00,  //  002F  RET	1	R7
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
    8,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_GenericSwitch_Btn,     /* shared constants */
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
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_set_position,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_GenericSwitch_Btn,     /* shared constants */
    be_str_weak(set_position),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8808010A,  //  0000  GETMBR	R2	R0	K10
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0004,  //  0002  JMPF	R2	#0008
      0x8C08010D,  //  0003  GETMET	R2	R0	K13
      0x5412003A,  //  0004  LDINT	R4	59
      0x58140008,  //  0005  LDCONST	R5	K8
      0x7C080600,  //  0006  CALL	R2	3
      0x90021401,  //  0007  SETMBR	R0	K10	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_parse_configuration,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_GenericSwitch_Btn,     /* shared constants */
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050E,  //  0003  GETMET	R2	R2	K14
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080009,  //  0006  GETGBL	R2	G9
      0x8C0C0310,  //  0007  GETMET	R3	R1	K16
      0x88140111,  //  0008  GETMBR	R5	R0	K17
      0x58180008,  //  0009  LDCONST	R6	K8
      0x7C0C0600,  //  000A  CALL	R3	3
      0x7C080200,  //  000B  CALL	R2	1
      0x90021E02,  //  000C  SETMBR	R0	K15	R2
      0x8808010F,  //  000D  GETMBR	R2	R0	K15
      0x18080504,  //  000E  LE	R2	R2	K4
      0x780A0000,  //  000F  JMPF	R2	#0011
      0x90021F08,  //  0010  SETMBR	R0	K15	K8
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: append_state_json
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_append_state_json,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_GenericSwitch_Btn,     /* shared constants */
    be_str_weak(append_state_json),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080012,  //  0001  LDCONST	R2	K18
      0x600C0009,  //  0002  GETGBL	R3	G9
      0x88100113,  //  0003  GETMBR	R4	R0	K19
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
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_button_handler,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_GenericSwitch_Btn,     /* shared constants */
    be_str_weak(button_handler),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0x1C140504,  //  0000  EQ	R5	R2	K4
      0x74160001,  //  0001  JMPT	R5	#0004
      0x1C140508,  //  0002  EQ	R5	R2	K8
      0x78160002,  //  0003  JMPF	R5	#0007
      0x8C140114,  //  0004  GETMET	R5	R0	K20
      0x5C1C0600,  //  0005  MOVE	R7	R3
      0x7C140400,  //  0006  CALL	R5	2
      0x1C140508,  //  0007  EQ	R5	R2	K8
      0x78160040,  //  0008  JMPF	R5	#004A
      0x780E0010,  //  0009  JMPF	R3	#001B
      0x8C140115,  //  000A  GETMET	R5	R0	K21
      0x541E003A,  //  000B  LDINT	R7	59
      0x58200008,  //  000C  LDCONST	R8	K8
      0xB8260000,  //  000D  GETNGBL	R9	K0
      0x88241316,  //  000E  GETMBR	R9	R9	K22
      0xB82A0000,  //  000F  GETNGBL	R10	K0
      0x88281501,  //  0010  GETMBR	R10	R10	K1
      0x8C281517,  //  0011  GETMET	R10	R10	K23
      0x7C280200,  //  0012  CALL	R10	1
      0x8C281505,  //  0013  GETMET	R10	R10	K5
      0xB8320000,  //  0014  GETNGBL	R12	K0
      0x88301901,  //  0015  GETMBR	R12	R12	K1
      0x88301906,  //  0016  GETMBR	R12	R12	K6
      0x58340008,  //  0017  LDCONST	R13	K8
      0x7C280600,  //  0018  CALL	R10	3
      0x7C140A00,  //  0019  CALL	R5	5
      0x7002000F,  //  001A  JMP		#002B
      0x8C140115,  //  001B  GETMET	R5	R0	K21
      0x541E003A,  //  001C  LDINT	R7	59
      0x58200018,  //  001D  LDCONST	R8	K24
      0xB8260000,  //  001E  GETNGBL	R9	K0
      0x88241316,  //  001F  GETMBR	R9	R9	K22
      0xB82A0000,  //  0020  GETNGBL	R10	K0
      0x88281501,  //  0021  GETMBR	R10	R10	K1
      0x8C281517,  //  0022  GETMET	R10	R10	K23
      0x7C280200,  //  0023  CALL	R10	1
      0x8C281505,  //  0024  GETMET	R10	R10	K5
      0xB8320000,  //  0025  GETNGBL	R12	K0
      0x88301901,  //  0026  GETMBR	R12	R12	K1
      0x88301906,  //  0027  GETMBR	R12	R12	K6
      0x58340008,  //  0028  LDCONST	R13	K8
      0x7C280600,  //  0029  CALL	R10	3
      0x7C140A00,  //  002A  CALL	R5	5
      0x1C140708,  //  002B  EQ	R5	R3	K8
      0x7816001B,  //  002C  JMPF	R5	#0049
      0x24140904,  //  002D  GT	R5	R4	K4
      0x78160019,  //  002E  JMPF	R5	#0049
      0x8C140115,  //  002F  GETMET	R5	R0	K21
      0x541E003A,  //  0030  LDINT	R7	59
      0x54220004,  //  0031  LDINT	R8	5
      0xB8260000,  //  0032  GETNGBL	R9	K0
      0x88241316,  //  0033  GETMBR	R9	R9	K22
      0xB82A0000,  //  0034  GETNGBL	R10	K0
      0x88281501,  //  0035  GETMBR	R10	R10	K1
      0x8C281517,  //  0036  GETMET	R10	R10	K23
      0x7C280200,  //  0037  CALL	R10	1
      0x8C281505,  //  0038  GETMET	R10	R10	K5
      0xB8320000,  //  0039  GETNGBL	R12	K0
      0x88301901,  //  003A  GETMBR	R12	R12	K1
      0x88301906,  //  003B  GETMBR	R12	R12	K6
      0x58340008,  //  003C  LDCONST	R13	K8
      0x7C280600,  //  003D  CALL	R10	3
      0xB82E0000,  //  003E  GETNGBL	R11	K0
      0x882C1701,  //  003F  GETMBR	R11	R11	K1
      0x8C2C1717,  //  0040  GETMET	R11	R11	K23
      0x7C2C0200,  //  0041  CALL	R11	1
      0x8C2C1705,  //  0042  GETMET	R11	R11	K5
      0xB8360000,  //  0043  GETNGBL	R13	K0
      0x88341B01,  //  0044  GETMBR	R13	R13	K1
      0x88341B06,  //  0045  GETMBR	R13	R13	K6
      0x00380908,  //  0046  ADD	R14	R4	K8
      0x7C2C0600,  //  0047  CALL	R11	3
      0x7C140C00,  //  0048  CALL	R5	6
      0x7002001D,  //  0049  JMP		#0068
      0x1C140507,  //  004A  EQ	R5	R2	K7
      0x7816001B,  //  004B  JMPF	R5	#0068
      0x24140904,  //  004C  GT	R5	R4	K4
      0x78160019,  //  004D  JMPF	R5	#0068
      0x8C140115,  //  004E  GETMET	R5	R0	K21
      0x541E003A,  //  004F  LDINT	R7	59
      0x54220005,  //  0050  LDINT	R8	6
      0xB8260000,  //  0051  GETNGBL	R9	K0
      0x88241316,  //  0052  GETMBR	R9	R9	K22
      0xB82A0000,  //  0053  GETNGBL	R10	K0
      0x88281501,  //  0054  GETMBR	R10	R10	K1
      0x8C281517,  //  0055  GETMET	R10	R10	K23
      0x7C280200,  //  0056  CALL	R10	1
      0x8C281505,  //  0057  GETMET	R10	R10	K5
      0xB8320000,  //  0058  GETNGBL	R12	K0
      0x88301901,  //  0059  GETMBR	R12	R12	K1
      0x88301906,  //  005A  GETMBR	R12	R12	K6
      0x58340008,  //  005B  LDCONST	R13	K8
      0x7C280600,  //  005C  CALL	R10	3
      0xB82E0000,  //  005D  GETNGBL	R11	K0
      0x882C1701,  //  005E  GETMBR	R11	R11	K1
      0x8C2C1717,  //  005F  GETMET	R11	R11	K23
      0x7C2C0200,  //  0060  CALL	R11	1
      0x8C2C1705,  //  0061  GETMET	R11	R11	K5
      0xB8360000,  //  0062  GETNGBL	R13	K0
      0x88341B01,  //  0063  GETMBR	R13	R13	K1
      0x88341B06,  //  0064  GETMBR	R13	R13	K6
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
        { be_const_key_int(5, 1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, -1), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, 2), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(59, 0), be_const_bytes_instance(000000010002FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Sensor_GenericSwitch_Btn)
);
/********************************************************************/
/* End of solidification */
