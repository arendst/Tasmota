/* Solidification of Matter_Plugin_2_Sensor_GenericSwitch_Btn.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_GenericSwitch_Btn' ktab size: 22, total: 30 (saved 64 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_GenericSwitch_Btn[22] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_const_int(1),
  /* K2   */  be_nested_str_weak(set_position),
  /* K3   */  be_nested_str_weak(publish_event),
  /* K4   */  be_nested_str_weak(matter),
  /* K5   */  be_nested_str_weak(TLV),
  /* K6   */  be_nested_str_weak(Matter_TLV_item),
  /* K7   */  be_nested_str_weak(set),
  /* K8   */  be_const_int(3),
  /* K9   */  be_const_int(2),
  /* K10  */  be_nested_str_weak(parse_configuration),
  /* K11  */  be_nested_str_weak(tasmota_switch_index),
  /* K12  */  be_nested_str_weak(find),
  /* K13  */  be_nested_str_weak(button),
  /* K14  */  be_nested_str_weak(cluster),
  /* K15  */  be_nested_str_weak(attribute),
  /* K16  */  be_nested_str_weak(set_or_nil),
  /* K17  */  be_nested_str_weak(shadow_position),
  /* K18  */  be_nested_str_weak(read_attribute),
  /* K19  */  be_nested_str_weak(_X2C_X22Switch_X22_X3A_X25s),
  /* K20  */  be_nested_str_weak(shadow_onoff),
  /* K21  */  be_nested_str_weak(attribute_updated),
};


extern const bclass be_class_Matter_Plugin_Sensor_GenericSwitch_Btn;

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
    ( &(const binstruction[89]) {  /* code */
      0x1C140500,  //  0000  EQ	R5	R2	K0
      0x74160001,  //  0001  JMPT	R5	#0004
      0x1C140501,  //  0002  EQ	R5	R2	K1
      0x78160002,  //  0003  JMPF	R5	#0007
      0x8C140102,  //  0004  GETMET	R5	R0	K2
      0x5C1C0600,  //  0005  MOVE	R7	R3
      0x7C140400,  //  0006  CALL	R5	2
      0x1C140501,  //  0007  EQ	R5	R2	K1
      0x78160035,  //  0008  JMPF	R5	#003F
      0x780E000D,  //  0009  JMPF	R3	#0018
      0x8C140103,  //  000A  GETMET	R5	R0	K3
      0x541E003A,  //  000B  LDINT	R7	59
      0x58200001,  //  000C  LDCONST	R8	K1
      0x58240001,  //  000D  LDCONST	R9	K1
      0xB82A0800,  //  000E  GETNGBL	R10	K4
      0x88281505,  //  000F  GETMBR	R10	R10	K5
      0x8C281506,  //  0010  GETMET	R10	R10	K6
      0x7C280200,  //  0011  CALL	R10	1
      0x8C281507,  //  0012  GETMET	R10	R10	K7
      0x54320003,  //  0013  LDINT	R12	4
      0x58340001,  //  0014  LDCONST	R13	K1
      0x7C280600,  //  0015  CALL	R10	3
      0x7C140A00,  //  0016  CALL	R5	5
      0x7002000C,  //  0017  JMP		#0025
      0x8C140103,  //  0018  GETMET	R5	R0	K3
      0x541E003A,  //  0019  LDINT	R7	59
      0x58200008,  //  001A  LDCONST	R8	K8
      0x58240001,  //  001B  LDCONST	R9	K1
      0xB82A0800,  //  001C  GETNGBL	R10	K4
      0x88281505,  //  001D  GETMBR	R10	R10	K5
      0x8C281506,  //  001E  GETMET	R10	R10	K6
      0x7C280200,  //  001F  CALL	R10	1
      0x8C281507,  //  0020  GETMET	R10	R10	K7
      0x54320003,  //  0021  LDINT	R12	4
      0x58340001,  //  0022  LDCONST	R13	K1
      0x7C280600,  //  0023  CALL	R10	3
      0x7C140A00,  //  0024  CALL	R5	5
      0x1C140701,  //  0025  EQ	R5	R3	K1
      0x78160016,  //  0026  JMPF	R5	#003E
      0x24140900,  //  0027  GT	R5	R4	K0
      0x78160014,  //  0028  JMPF	R5	#003E
      0x8C140103,  //  0029  GETMET	R5	R0	K3
      0x541E003A,  //  002A  LDINT	R7	59
      0x54220004,  //  002B  LDINT	R8	5
      0x58240001,  //  002C  LDCONST	R9	K1
      0xB82A0800,  //  002D  GETNGBL	R10	K4
      0x88281505,  //  002E  GETMBR	R10	R10	K5
      0x8C281506,  //  002F  GETMET	R10	R10	K6
      0x7C280200,  //  0030  CALL	R10	1
      0x8C281507,  //  0031  GETMET	R10	R10	K7
      0x54320003,  //  0032  LDINT	R12	4
      0x58340001,  //  0033  LDCONST	R13	K1
      0x7C280600,  //  0034  CALL	R10	3
      0xB82E0800,  //  0035  GETNGBL	R11	K4
      0x882C1705,  //  0036  GETMBR	R11	R11	K5
      0x8C2C1706,  //  0037  GETMET	R11	R11	K6
      0x7C2C0200,  //  0038  CALL	R11	1
      0x8C2C1707,  //  0039  GETMET	R11	R11	K7
      0x54360003,  //  003A  LDINT	R13	4
      0x00380901,  //  003B  ADD	R14	R4	K1
      0x7C2C0600,  //  003C  CALL	R11	3
      0x7C140C00,  //  003D  CALL	R5	6
      0x70020018,  //  003E  JMP		#0058
      0x1C140509,  //  003F  EQ	R5	R2	K9
      0x78160016,  //  0040  JMPF	R5	#0058
      0x24140900,  //  0041  GT	R5	R4	K0
      0x78160014,  //  0042  JMPF	R5	#0058
      0x8C140103,  //  0043  GETMET	R5	R0	K3
      0x541E003A,  //  0044  LDINT	R7	59
      0x54220005,  //  0045  LDINT	R8	6
      0x58240001,  //  0046  LDCONST	R9	K1
      0xB82A0800,  //  0047  GETNGBL	R10	K4
      0x88281505,  //  0048  GETMBR	R10	R10	K5
      0x8C281506,  //  0049  GETMET	R10	R10	K6
      0x7C280200,  //  004A  CALL	R10	1
      0x8C281507,  //  004B  GETMET	R10	R10	K7
      0x54320003,  //  004C  LDINT	R12	4
      0x58340001,  //  004D  LDCONST	R13	K1
      0x7C280600,  //  004E  CALL	R10	3
      0xB82E0800,  //  004F  GETNGBL	R11	K4
      0x882C1705,  //  0050  GETMBR	R11	R11	K5
      0x8C2C1706,  //  0051  GETMET	R11	R11	K6
      0x7C2C0200,  //  0052  CALL	R11	1
      0x8C2C1707,  //  0053  GETMET	R11	R11	K7
      0x54360003,  //  0054  LDINT	R13	4
      0x5C380800,  //  0055  MOVE	R14	R4
      0x7C2C0600,  //  0056  CALL	R11	3
      0x7C140C00,  //  0057  CALL	R5	6
      0x80000000,  //  0058  RET	0
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
      0x8C08050A,  //  0003  GETMET	R2	R2	K10
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080009,  //  0006  GETGBL	R2	G9
      0x8C0C030C,  //  0007  GETMET	R3	R1	K12
      0x5814000D,  //  0008  LDCONST	R5	K13
      0x58180001,  //  0009  LDCONST	R6	K1
      0x7C0C0600,  //  000A  CALL	R3	3
      0x7C080200,  //  000B  CALL	R2	1
      0x90021602,  //  000C  SETMBR	R0	K11	R2
      0x8808010B,  //  000D  GETMBR	R2	R0	K11
      0x18080500,  //  000E  LE	R2	R2	K0
      0x780A0000,  //  000F  JMPF	R2	#0011
      0x90021701,  //  0010  SETMBR	R0	K11	K1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
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
    ( &(const binstruction[46]) {  /* code */
      0x8810050E,  //  0000  GETMBR	R4	R2	K14
      0x8814050F,  //  0001  GETMBR	R5	R2	K15
      0x541A003A,  //  0002  LDINT	R6	59
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A001F,  //  0004  JMPF	R6	#0025
      0x1C180B00,  //  0005  EQ	R6	R5	K0
      0x781A0005,  //  0006  JMPF	R6	#000D
      0x8C180707,  //  0007  GETMET	R6	R3	K7
      0x54220003,  //  0008  LDINT	R8	4
      0x58240009,  //  0009  LDCONST	R9	K9
      0x7C180600,  //  000A  CALL	R6	3
      0x80040C00,  //  000B  RET	1	R6
      0x70020017,  //  000C  JMP		#0025
      0x1C180B01,  //  000D  EQ	R6	R5	K1
      0x781A0005,  //  000E  JMPF	R6	#0015
      0x8C180710,  //  000F  GETMET	R6	R3	K16
      0x54220003,  //  0010  LDINT	R8	4
      0x88240111,  //  0011  GETMBR	R9	R0	K17
      0x7C180600,  //  0012  CALL	R6	3
      0x80040C00,  //  0013  RET	1	R6
      0x7002000F,  //  0014  JMP		#0025
      0x1C180B09,  //  0015  EQ	R6	R5	K9
      0x781A0005,  //  0016  JMPF	R6	#001D
      0x8C180707,  //  0017  GETMET	R6	R3	K7
      0x54220003,  //  0018  LDINT	R8	4
      0x54260004,  //  0019  LDINT	R9	5
      0x7C180600,  //  001A  CALL	R6	3
      0x80040C00,  //  001B  RET	1	R6
      0x70020007,  //  001C  JMP		#0025
      0x541AFFFB,  //  001D  LDINT	R6	65532
      0x1C180A06,  //  001E  EQ	R6	R5	R6
      0x781A0004,  //  001F  JMPF	R6	#0025
      0x8C180707,  //  0020  GETMET	R6	R3	K7
      0x54220005,  //  0021  LDINT	R8	6
      0x54260015,  //  0022  LDINT	R9	22
      0x7C180600,  //  0023  CALL	R6	3
      0x80040C00,  //  0024  RET	1	R6
      0x60180003,  //  0025  GETGBL	R6	G3
      0x5C1C0000,  //  0026  MOVE	R7	R0
      0x7C180200,  //  0027  CALL	R6	1
      0x8C180D12,  //  0028  GETMET	R6	R6	K18
      0x5C200200,  //  0029  MOVE	R8	R1
      0x5C240400,  //  002A  MOVE	R9	R2
      0x5C280600,  //  002B  MOVE	R10	R3
      0x7C180800,  //  002C  CALL	R6	4
      0x80040C00,  //  002D  RET	1	R6
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
      0x58080013,  //  0001  LDCONST	R2	K19
      0x600C0009,  //  0002  GETGBL	R3	G9
      0x88100114,  //  0003  GETMBR	R4	R0	K20
      0x7C0C0200,  //  0004  CALL	R3	1
      0x7C040400,  //  0005  CALL	R1	2
      0x80040200,  //  0006  RET	1	R1
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
      0x88080111,  //  0000  GETMBR	R2	R0	K17
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0004,  //  0002  JMPF	R2	#0008
      0x8C080115,  //  0003  GETMET	R2	R0	K21
      0x5412003A,  //  0004  LDINT	R4	59
      0x58140001,  //  0005  LDCONST	R5	K1
      0x7C080600,  //  0006  CALL	R2	3
      0x90022201,  //  0007  SETMBR	R0	K17	R1
      0x80000000,  //  0008  RET	0
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
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(button_handler, -1), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_button_handler_closure) },
        { be_const_key_weak(shadow_position, 5), be_const_var(1) },
        { be_const_key_weak(TYPES, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(15, -1), be_const_int(3) },
    }))    ) } )) },
        { be_const_key_weak(append_state_json, 7), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_append_state_json_closure) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_parse_configuration_closure) },
        { be_const_key_weak(TYPE, 4), be_nested_str_weak(gensw_btn) },
        { be_const_key_weak(SCHEMA, 8), be_nested_str_weak(button_X7Cl_X3AButton_X7Ct_X3Ai_X7Ch_X3AButton_X3Cx_X3E_X20number_X7Cr_X3A1) },
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
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_read_attribute_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Generic_X20Switch_X2FButton) },
        { be_const_key_weak(tasmota_switch_index, 3), be_const_var(0) },
        { be_const_key_weak(set_position, -1), be_const_closure(class_Matter_Plugin_Sensor_GenericSwitch_Btn_set_position_closure) },
    })),
    be_str_weak(Matter_Plugin_Sensor_GenericSwitch_Btn)
);
/********************************************************************/
/* End of solidification */
