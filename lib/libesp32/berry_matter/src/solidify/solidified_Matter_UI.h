/* Solidification of Matter_UI.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// ktab too big for class 'Matter_UI' - skipping

extern const bclass be_class_Matter_UI;

/********************************************************************
** Solidified function: equal_map
********************************************************************/
be_local_closure(class_Matter_UI_equal_map,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_UI),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(contains),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(equal_map),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C0010,  //  0001  GETGBL	R3	G16
      0x8C100101,  //  0002  GETMET	R4	R0	K1
      0x7C100200,  //  0003  CALL	R4	1
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA8020010,  //  0005  EXBLK	0	#0017
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140302,  //  0008  GETMET	R5	R1	K2
      0x5C1C0800,  //  0009  MOVE	R7	R4
      0x7C140400,  //  000A  CALL	R5	2
      0x74160002,  //  000B  JMPT	R5	#000F
      0x50140000,  //  000C  LDBOOL	R5	0	0
      0xA8040001,  //  000D  EXBLK	1	1
      0x80040A00,  //  000E  RET	1	R5
      0x94140204,  //  000F  GETIDX	R5	R1	R4
      0x94180004,  //  0010  GETIDX	R6	R0	R4
      0x20140A06,  //  0011  NE	R5	R5	R6
      0x78160002,  //  0012  JMPF	R5	#0016
      0x50140000,  //  0013  LDBOOL	R5	0	0
      0xA8040001,  //  0014  EXBLK	1	1
      0x80040A00,  //  0015  RET	1	R5
      0x7001FFEE,  //  0016  JMP		#0006
      0x580C0003,  //  0017  LDCONST	R3	K3
      0xAC0C0200,  //  0018  CATCH	R3	1	0
      0xB0080000,  //  0019  RAISE	2	R0	R0
      0x600C0010,  //  001A  GETGBL	R3	G16
      0x8C100301,  //  001B  GETMET	R4	R1	K1
      0x7C100200,  //  001C  CALL	R4	1
      0x7C0C0200,  //  001D  CALL	R3	1
      0xA8020010,  //  001E  EXBLK	0	#0030
      0x5C100600,  //  001F  MOVE	R4	R3
      0x7C100000,  //  0020  CALL	R4	0
      0x8C140102,  //  0021  GETMET	R5	R0	K2
      0x5C1C0800,  //  0022  MOVE	R7	R4
      0x7C140400,  //  0023  CALL	R5	2
      0x74160002,  //  0024  JMPT	R5	#0028
      0x50140000,  //  0025  LDBOOL	R5	0	0
      0xA8040001,  //  0026  EXBLK	1	1
      0x80040A00,  //  0027  RET	1	R5
      0x94140204,  //  0028  GETIDX	R5	R1	R4
      0x94180004,  //  0029  GETIDX	R6	R0	R4
      0x20140A06,  //  002A  NE	R5	R5	R6
      0x78160002,  //  002B  JMPF	R5	#002F
      0x50140000,  //  002C  LDBOOL	R5	0	0
      0xA8040001,  //  002D  EXBLK	1	1
      0x80040A00,  //  002E  RET	1	R5
      0x7001FFEE,  //  002F  JMP		#001F
      0x580C0003,  //  0030  LDCONST	R3	K3
      0xAC0C0200,  //  0031  CATCH	R3	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x500C0200,  //  0033  LDBOOL	R3	1	0
      0x80040600,  //  0034  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_mgr_adv
********************************************************************/
be_local_closure(class_Matter_UI_page_part_mgr_adv,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(check_privileged_access),
    /* K2   */  be_nested_str_weak(content_start),
    /* K3   */  be_nested_str_weak(Matter_X20Advanced_X20Configuration),
    /* K4   */  be_nested_str_weak(content_send_style),
    /* K5   */  be_nested_str_weak(matter_enabled),
    /* K6   */  be_nested_str_weak(show_passcode_form),
    /* K7   */  be_nested_str_weak(show_fabric_info),
    /* K8   */  be_nested_str_weak(web_add_config_button),
    /* K9   */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr_adv),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0001,  //  0003  JMPT	R2	#0006
      0x4C080000,  //  0004  LDNIL	R2
      0x80040400,  //  0005  RET	1	R2
      0x8C080302,  //  0006  GETMET	R2	R1	K2
      0x58100003,  //  0007  LDCONST	R4	K3
      0x7C080400,  //  0008  CALL	R2	2
      0x8C080304,  //  0009  GETMET	R2	R1	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x8C080107,  //  0010  GETMET	R2	R0	K7
      0x7C080200,  //  0011  CALL	R2	1
      0x8C080108,  //  0012  GETMET	R2	R0	K8
      0x7C080200,  //  0013  CALL	R2	1
      0x8C080309,  //  0014  GETMET	R2	R1	K9
      0x7C080200,  //  0015  CALL	R2	1
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_mgr
********************************************************************/
be_local_closure(class_Matter_UI_page_part_mgr,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(check_privileged_access),
    /* K2   */  be_nested_str_weak(content_start),
    /* K3   */  be_nested_str_weak(Matter),
    /* K4   */  be_nested_str_weak(content_send_style),
    /* K5   */  be_nested_str_weak(show_enable),
    /* K6   */  be_nested_str_weak(matter_enabled),
    /* K7   */  be_nested_str_weak(show_plugins_configuration),
    /* K8   */  be_nested_str_weak(content_send),
    /* K9   */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cform_X20id_X3D_X27butmat_X27_X20style_X3D_X27display_X3A_X20block_X3B_X27_X20action_X3D_X27mattera_X27_X20method_X3D_X27get_X27_X3E_X3Cbutton_X20name_X3D_X27_X27_X3EAdvanced_X20Configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K10  */  be_nested_str_weak(content_button),
    /* K11  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K12  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0001,  //  0003  JMPT	R2	#0006
      0x4C080000,  //  0004  LDNIL	R2
      0x80040400,  //  0005  RET	1	R2
      0x8C080302,  //  0006  GETMET	R2	R1	K2
      0x58100003,  //  0007  LDCONST	R4	K3
      0x7C080400,  //  0008  CALL	R2	2
      0x8C080304,  //  0009  GETMET	R2	R1	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x8C080106,  //  000D  GETMET	R2	R0	K6
      0x7C080200,  //  000E  CALL	R2	1
      0x780A0001,  //  000F  JMPF	R2	#0012
      0x8C080107,  //  0010  GETMET	R2	R0	K7
      0x7C080200,  //  0011  CALL	R2	1
      0x8C080308,  //  0012  GETMET	R2	R1	K8
      0x58100009,  //  0013  LDCONST	R4	K9
      0x7C080400,  //  0014  CALL	R2	2
      0x8C08030A,  //  0015  GETMET	R2	R1	K10
      0x8810030B,  //  0016  GETMBR	R4	R1	K11
      0x7C080400,  //  0017  CALL	R2	2
      0x8C08030C,  //  0018  GETMET	R2	R1	K12
      0x7C080200,  //  0019  CALL	R2	1
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: plugin_name
********************************************************************/
be_local_closure(class_Matter_UI_plugin_name,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    3,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(get_plugin_class_displayname),
    }),
    be_str_weak(plugin_name),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0000,  //  0001  JMPF	R3	#0003
      0x80060000,  //  0002  RET	1	K0
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0004  GETMET	R3	R3	K2
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C0C0400,  //  0006  CALL	R3	2
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: matter_enabled
********************************************************************/
be_local_closure(class_Matter_UI_matter_enabled,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(get_option),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(MATTER_OPTION),
    }),
    be_str_weak(matter_enabled),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040017,  //  0000  GETGBL	R1	G23
      0xB80A0000,  //  0001  GETNGBL	R2	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0xB8120400,  //  0003  GETNGBL	R4	K2
      0x88100903,  //  0004  GETMBR	R4	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x7C040200,  //  0006  CALL	R1	1
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_config_from_status
********************************************************************/
be_local_closure(class_Matter_UI_generate_config_from_status,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(POWER),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(HSBColor),
    /* K5   */  be_nested_str_weak(CT),
    /* K6   */  be_nested_str_weak(Dimmer),
    /* K7   */  be_nested_str_weak(push),
    /* K8   */  be_nested_str_weak(type),
    /* K9   */  be_nested_str_weak(light0),
    /* K10  */  be_nested_str_weak(relay),
    /* K11  */  be_nested_str_weak(stop_iteration),
    /* K12  */  be_nested_str_weak(light1),
    /* K13  */  be_nested_str_weak(light2),
    /* K14  */  be_nested_str_weak(light3),
    /* K15  */  be_nested_str_weak(device),
    /* K16  */  be_nested_str_weak(autoconf),
    /* K17  */  be_nested_str_weak(autoconf_sensors_list),
    }),
    be_str_weak(generate_config_from_status),
    &be_const_str_solidified,
    ( &(const binstruction[97]) {  /* code */
      0x600C0012,  //  0000  GETGBL	R3	G18
      0x7C0C0000,  //  0001  CALL	R3	0
      0x58100000,  //  0002  LDCONST	R4	K0
      0x8C140501,  //  0003  GETMET	R5	R2	K1
      0x581C0002,  //  0004  LDCONST	R7	K2
      0x7C140400,  //  0005  CALL	R5	2
      0x78160001,  //  0006  JMPF	R5	#0009
      0x58100003,  //  0007  LDCONST	R4	K3
      0x7002000E,  //  0008  JMP		#0018
      0x58140003,  //  0009  LDCONST	R5	K3
      0x50180200,  //  000A  LDBOOL	R6	1	0
      0x781A000B,  //  000B  JMPF	R6	#0018
      0x8C180501,  //  000C  GETMET	R6	R2	K1
      0x60200008,  //  000D  GETGBL	R8	G8
      0x5C240A00,  //  000E  MOVE	R9	R5
      0x7C200200,  //  000F  CALL	R8	1
      0x00220408,  //  0010  ADD	R8	K2	R8
      0x7C180400,  //  0011  CALL	R6	2
      0x781A0002,  //  0012  JMPF	R6	#0016
      0x5C100A00,  //  0013  MOVE	R4	R5
      0x00140B03,  //  0014  ADD	R5	R5	K3
      0x70020000,  //  0015  JMP		#0017
      0x70020000,  //  0016  JMP		#0018
      0x7001FFF1,  //  0017  JMP		#000A
      0x4C140000,  //  0018  LDNIL	R5
      0x4C180000,  //  0019  LDNIL	R6
      0x4C1C0000,  //  001A  LDNIL	R7
      0x8C200501,  //  001B  GETMET	R8	R2	K1
      0x58280004,  //  001C  LDCONST	R10	K4
      0x7C200400,  //  001D  CALL	R8	2
      0x78220002,  //  001E  JMPF	R8	#0022
      0x5C1C0800,  //  001F  MOVE	R7	R4
      0x04100903,  //  0020  SUB	R4	R4	K3
      0x7002000C,  //  0021  JMP		#002F
      0x8C200501,  //  0022  GETMET	R8	R2	K1
      0x58280005,  //  0023  LDCONST	R10	K5
      0x7C200400,  //  0024  CALL	R8	2
      0x78220002,  //  0025  JMPF	R8	#0029
      0x5C180800,  //  0026  MOVE	R6	R4
      0x04100903,  //  0027  SUB	R4	R4	K3
      0x70020005,  //  0028  JMP		#002F
      0x8C200501,  //  0029  GETMET	R8	R2	K1
      0x58280006,  //  002A  LDCONST	R10	K6
      0x7C200400,  //  002B  CALL	R8	2
      0x78220001,  //  002C  JMPF	R8	#002F
      0x5C140800,  //  002D  MOVE	R5	R4
      0x04100903,  //  002E  SUB	R4	R4	K3
      0x60200010,  //  002F  GETGBL	R8	G16
      0x40260604,  //  0030  CONNECT	R9	K3	R4
      0x7C200200,  //  0031  CALL	R8	1
      0xA8020008,  //  0032  EXBLK	0	#003C
      0x5C241000,  //  0033  MOVE	R9	R8
      0x7C240000,  //  0034  CALL	R9	0
      0x8C280707,  //  0035  GETMET	R10	R3	K7
      0x60300013,  //  0036  GETGBL	R12	G19
      0x7C300000,  //  0037  CALL	R12	0
      0x98321109,  //  0038  SETIDX	R12	K8	K9
      0x98321409,  //  0039  SETIDX	R12	K10	R9
      0x7C280400,  //  003A  CALL	R10	2
      0x7001FFF6,  //  003B  JMP		#0033
      0x5820000B,  //  003C  LDCONST	R8	K11
      0xAC200200,  //  003D  CATCH	R8	1	0
      0xB0080000,  //  003E  RAISE	2	R0	R0
      0x4C200000,  //  003F  LDNIL	R8
      0x20200A08,  //  0040  NE	R8	R5	R8
      0x78220005,  //  0041  JMPF	R8	#0048
      0x8C200707,  //  0042  GETMET	R8	R3	K7
      0x60280013,  //  0043  GETGBL	R10	G19
      0x7C280000,  //  0044  CALL	R10	0
      0x982A110C,  //  0045  SETIDX	R10	K8	K12
      0x982A1405,  //  0046  SETIDX	R10	K10	R5
      0x7C200400,  //  0047  CALL	R8	2
      0x4C200000,  //  0048  LDNIL	R8
      0x20200C08,  //  0049  NE	R8	R6	R8
      0x78220005,  //  004A  JMPF	R8	#0051
      0x8C200707,  //  004B  GETMET	R8	R3	K7
      0x60280013,  //  004C  GETGBL	R10	G19
      0x7C280000,  //  004D  CALL	R10	0
      0x982A110D,  //  004E  SETIDX	R10	K8	K13
      0x982A1406,  //  004F  SETIDX	R10	K10	R6
      0x7C200400,  //  0050  CALL	R8	2
      0x4C200000,  //  0051  LDNIL	R8
      0x20200E08,  //  0052  NE	R8	R7	R8
      0x78220005,  //  0053  JMPF	R8	#005A
      0x8C200707,  //  0054  GETMET	R8	R3	K7
      0x60280013,  //  0055  GETGBL	R10	G19
      0x7C280000,  //  0056  CALL	R10	0
      0x982A110E,  //  0057  SETIDX	R10	K8	K14
      0x982A1407,  //  0058  SETIDX	R10	K10	R7
      0x7C200400,  //  0059  CALL	R8	2
      0x8820010F,  //  005A  GETMBR	R8	R0	K15
      0x88201110,  //  005B  GETMBR	R8	R8	K16
      0x8C201111,  //  005C  GETMET	R8	R8	K17
      0x5C280200,  //  005D  MOVE	R10	R1
      0x7C200400,  //  005E  CALL	R8	2
      0x000C0608,  //  005F  ADD	R3	R3	R8
      0x80040600,  //  0060  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_sensor
********************************************************************/
be_local_closure(class_Matter_UI_web_sensor,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(matter_enabled),
    /* K2   */  be_nested_str_weak(device),
    /* K3   */  be_nested_str_weak(sessions),
    /* K4   */  be_nested_str_weak(count_active_fabrics),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(content_send),
    /* K7   */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27text_X2Dalign_X3Aright_X3Bfont_X2Dsize_X3A11px_X3Bcolor_X3A_X23aaa_X3Bpadding_X3A0px_X3B_X27_X3E_X25s_X3C_X2Fdiv_X3E),
    /* K8   */  be_nested_str_weak(Matter_X3A_X20No_X20active_X20association),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(Matter_X3A_X20),
    /* K11  */  be_nested_str_weak(_X20active_X20association),
    /* K12  */  be_nested_str_weak(s),
    /* K13  */  be_nested_str_weak(),
    /* K14  */  be_nested_str_weak(show_bridge_status),
    /* K15  */  be_nested_str_weak(commissioning),
    /* K16  */  be_nested_str_weak(is_root_commissioning_open),
    /* K17  */  be_nested_str_weak(show_commissioning_info),
    }),
    be_str_weak(web_sensor),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x780A002C,  //  0003  JMPF	R2	#0031
      0x88080102,  //  0004  GETMBR	R2	R0	K2
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x4C0C0000,  //  0006  LDNIL	R3
      0x20080403,  //  0007  NE	R2	R2	R3
      0x780A0004,  //  0008  JMPF	R2	#000E
      0x88080102,  //  0009  GETMBR	R2	R0	K2
      0x88080503,  //  000A  GETMBR	R2	R2	K3
      0x8C080504,  //  000B  GETMET	R2	R2	K4
      0x7C080200,  //  000C  CALL	R2	1
      0x70020000,  //  000D  JMP		#000F
      0x58080005,  //  000E  LDCONST	R2	K5
      0x1C0C0505,  //  000F  EQ	R3	R2	K5
      0x780E0006,  //  0010  JMPF	R3	#0018
      0x8C0C0306,  //  0011  GETMET	R3	R1	K6
      0x60140018,  //  0012  GETGBL	R5	G24
      0x58180007,  //  0013  LDCONST	R6	K7
      0x581C0008,  //  0014  LDCONST	R7	K8
      0x7C140400,  //  0015  CALL	R5	2
      0x7C0C0400,  //  0016  CALL	R3	2
      0x7002000F,  //  0017  JMP		#0028
      0x240C0509,  //  0018  GT	R3	R2	K9
      0x8C100306,  //  0019  GETMET	R4	R1	K6
      0x60180018,  //  001A  GETGBL	R6	G24
      0x581C0007,  //  001B  LDCONST	R7	K7
      0x60200008,  //  001C  GETGBL	R8	G8
      0x5C240400,  //  001D  MOVE	R9	R2
      0x7C200200,  //  001E  CALL	R8	1
      0x00221408,  //  001F  ADD	R8	K10	R8
      0x0020110B,  //  0020  ADD	R8	R8	K11
      0x780E0001,  //  0021  JMPF	R3	#0024
      0x5824000C,  //  0022  LDCONST	R9	K12
      0x70020000,  //  0023  JMP		#0025
      0x5824000D,  //  0024  LDCONST	R9	K13
      0x00201009,  //  0025  ADD	R8	R8	R9
      0x7C180400,  //  0026  CALL	R6	2
      0x7C100400,  //  0027  CALL	R4	2
      0x8C0C010E,  //  0028  GETMET	R3	R0	K14
      0x7C0C0200,  //  0029  CALL	R3	1
      0x880C0102,  //  002A  GETMBR	R3	R0	K2
      0x880C070F,  //  002B  GETMBR	R3	R3	K15
      0x8C0C0710,  //  002C  GETMET	R3	R3	K16
      0x7C0C0200,  //  002D  CALL	R3	1
      0x780E0001,  //  002E  JMPF	R3	#0031
      0x8C0C0111,  //  002F  GETMET	R3	R0	K17
      0x7C0C0200,  //  0030  CALL	R3	1
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_mgr_add
********************************************************************/
be_local_closure(class_Matter_UI_page_part_mgr_add,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(check_privileged_access),
    /* K2   */  be_nested_str_weak(content_start),
    /* K3   */  be_nested_str_weak(Matter_X20Create_X20new_X20endpoint),
    /* K4   */  be_nested_str_weak(content_send_style),
    /* K5   */  be_nested_str_weak(arg),
    /* K6   */  be_nested_str_weak(url),
    /* K7   */  be_nested_str_weak(matter_enabled),
    /* K8   */  be_nested_str_weak(show_remote_autoconf),
    /* K9   */  be_nested_str_weak(content_button),
    /* K10  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K11  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr_add),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0001,  //  0003  JMPT	R2	#0006
      0x4C080000,  //  0004  LDNIL	R2
      0x80040400,  //  0005  RET	1	R2
      0x8C080302,  //  0006  GETMET	R2	R1	K2
      0x58100003,  //  0007  LDCONST	R4	K3
      0x7C080400,  //  0008  CALL	R2	2
      0x8C080304,  //  0009  GETMET	R2	R1	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x8C080305,  //  000B  GETMET	R2	R1	K5
      0x58100006,  //  000C  LDCONST	R4	K6
      0x7C080400,  //  000D  CALL	R2	2
      0x8C0C0107,  //  000E  GETMET	R3	R0	K7
      0x7C0C0200,  //  000F  CALL	R3	1
      0x780E0002,  //  0010  JMPF	R3	#0014
      0x8C0C0108,  //  0011  GETMET	R3	R0	K8
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x8C0C0309,  //  0014  GETMET	R3	R1	K9
      0x8814030A,  //  0015  GETMBR	R5	R1	K10
      0x7C0C0400,  //  0016  CALL	R3	2
      0x8C0C030B,  //  0017  GETMET	R3	R1	K11
      0x7C0C0200,  //  0018  CALL	R3	1
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_plugins_configuration
********************************************************************/
be_local_closure(class_Matter_UI_show_plugins_configuration,   /* name */
  be_nested_proto(
    29,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[51]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(content_send),
    /* K4   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BCurrent_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K5   */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Cp_X3E_X3Cb_X3ELocal_X20sensors_X20and_X20devices_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K6   */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X23_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2778_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EName_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EParameter_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K7   */  be_nested_str_weak(device),
    /* K8   */  be_nested_str_weak(plugins_config),
    /* K9   */  be_nested_str_weak(remove),
    /* K10  */  be_nested_str_weak(0),
    /* K11  */  be_nested_str_weak(k2l_num),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str_weak(find),
    /* K14  */  be_nested_str_weak(type),
    /* K15  */  be_const_int(1),
    /* K16  */  be_nested_str_weak(http_),
    /* K17  */  be_nested_str_weak(plugins_classes),
    /* K18  */  be_nested_str_weak(),
    /* K19  */  be_nested_str_weak(ui_conf_to_string),
    /* K20  */  be_nested_str_weak(ARG_HINT),
    /* K21  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25i_X3C_X2Fb_X3E_X3C_X2Ftd_X3E),
    /* K22  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X3E_X3C_X2Ftd_X3E),
    /* K23  */  be_nested_str_weak(html_escape),
    /* K24  */  be_nested_str_weak(name),
    /* K25  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Ftd_X3E),
    /* K26  */  be_nested_str_weak(plugin_name),
    /* K27  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X20placeholder_X3D_X27_X25s_X27_X20title_X3D_X27_X25s_X27_X3E_X3C_X2Ftd_X3E),
    /* K28  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27text_X2Dalign_X3Acenter_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27del_X25i_X27_X20title_X3D_X27Delete_X20Endpoint_X20_X25i_X27_X20style_X3D_X27background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3B_X27_X20onclick_X3D_X22return_X20confirm_X28_X27Confirm_X20removing_X20endpoint_X20_X25i_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K29  */  be_nested_str_weak(_X3C_X2Ftable_X3E),
    /* K30  */  be_nested_str_weak(_X3Cp_X3E_X26lt_X3Bnone_X26gt_X3B_X3C_X2Fp_X3E),
    /* K31  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K32  */  be_nested_str_weak(url),
    /* K33  */  be_nested_str_weak(push),
    /* K34  */  be_nested_str_weak(stop_iteration),
    /* K35  */  be_nested_str_weak(sort_distinct),
    /* K36  */  be_nested_str_weak(get_plugin_remote_info),
    /* K37  */  be_nested_str_weak(_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20title_X3D_X27http_X3A_X2F_X2F_X25s_X2F_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X3E_X25s_X3C_X2Fa_X3E),
    /* K38  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2778_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K39  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2722_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25i_X3C_X2Fb_X3E_X3C_X2Ftd_X3E),
    /* K40  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X2778_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X20placeholder_X3D_X27_X28optional_X29_X27_X3E_X3C_X2Ftd_X3E),
    /* K41  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E),
    /* K42  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X25i_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X25s_X27_X20title_X3D_X27_X25s_X27_X3E_X3C_X2Ftd_X3E),
    /* K43  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X2715_X27_X20style_X3D_X27text_X2Dalign_X3Acenter_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27del_X25i_X27_X20style_X3D_X27background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3B_X27_X20onclick_X3D_X22return_X20confirm_X28_X27Confirm_X20removing_X20endpoint_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K44  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K45  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27config_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X20configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    /* K46  */  be_nested_str_weak(show_plugins_hints_js),
    /* K47  */  be_nested_str_weak(_CLASSES_TYPES),
    /* K48  */  be_nested_str_long(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BAdd_X20to_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20local_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X27100_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EName_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EParameter_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3Ctr_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X27_X20placeholder_X3D_X27_X28optional_X29_X27_X20title_X3D_X27_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cselect_X20id_X3D_X27pi_X27_X20name_X3D_X27pi_X27_X20onchange_X3D_X27otm_X28_X22arg_X22_X2Cthis_X2Evalue_X29_X27_X3E),
    /* K49  */  be_nested_str_weak(plugin_option),
    /* K50  */  be_nested_str_long(_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20id_X3D_X27arg_X27_X20name_X3D_X27arg_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E_X3Cbutton_X20name_X3D_X27addep_X27_X20class_X3D_X27button_X20bgrn_X27_X3ECreate_X20new_X20endpoint_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Chr_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20Tasmota_X20or_X20OpenBK_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatteradd_X27_X20method_X3D_X27get_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X2730_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3Ehttp_X3A_X2F_X2F_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27url_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X27_X20required_X20placeholder_X3D_X27IP_X20or_X20domain_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2710_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X2F_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E_X3Cbutton_X20class_X3D_X27button_X20bgrn_X27_X3EAuto_X2Dconfigure_X20remote_X20Tasmota_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Chr_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20RESET_X20the_X20configuration_X20to_X20the_X20default_X2E_X20You_X20will_X20need_X20to_X20associate_X20again_X2E_X22_X29_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27auto_X27_X20class_X3D_X27button_X20bred_X27_X3EReset_X20all_X20and_X20Auto_X2Ddiscover_X3C_X2Fbutton_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    }),
    be_str_weak(show_plugins_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[322]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x8C100303,  //  0003  GETMET	R4	R1	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x7C100400,  //  0005  CALL	R4	2
      0x8C100303,  //  0006  GETMET	R4	R1	K3
      0x58180005,  //  0007  LDCONST	R6	K5
      0x7C100400,  //  0008  CALL	R4	2
      0x8C100303,  //  0009  GETMET	R4	R1	K3
      0x58180006,  //  000A  LDCONST	R6	K6
      0x7C100400,  //  000B  CALL	R4	2
      0x88100107,  //  000C  GETMBR	R4	R0	K7
      0x88100908,  //  000D  GETMBR	R4	R4	K8
      0x8C100909,  //  000E  GETMET	R4	R4	K9
      0x5818000A,  //  000F  LDCONST	R6	K10
      0x7C100400,  //  0010  CALL	R4	2
      0x88100107,  //  0011  GETMBR	R4	R0	K7
      0x8C10090B,  //  0012  GETMET	R4	R4	K11
      0x88180107,  //  0013  GETMBR	R6	R0	K7
      0x88180D08,  //  0014  GETMBR	R6	R6	K8
      0x7C100400,  //  0015  CALL	R4	2
      0x5814000C,  //  0016  LDCONST	R5	K12
      0x50180000,  //  0017  LDBOOL	R6	0	0
      0x601C000C,  //  0018  GETGBL	R7	G12
      0x5C200800,  //  0019  MOVE	R8	R4
      0x7C1C0200,  //  001A  CALL	R7	1
      0x141C0A07,  //  001B  LT	R7	R5	R7
      0x781E005D,  //  001C  JMPF	R7	#007B
      0x941C0805,  //  001D  GETIDX	R7	R4	R5
      0x88200107,  //  001E  GETMBR	R8	R0	K7
      0x88201108,  //  001F  GETMBR	R8	R8	K8
      0x8C20110D,  //  0020  GETMET	R8	R8	K13
      0x60280008,  //  0021  GETGBL	R10	G8
      0x5C2C0E00,  //  0022  MOVE	R11	R7
      0x7C280200,  //  0023  CALL	R10	1
      0x7C200400,  //  0024  CALL	R8	2
      0x8C24110D,  //  0025  GETMET	R9	R8	K13
      0x582C000E,  //  0026  LDCONST	R11	K14
      0x7C240400,  //  0027  CALL	R9	2
      0x5C281200,  //  0028  MOVE	R10	R9
      0x742A0001,  //  0029  JMPT	R10	#002C
      0x00140B0F,  //  002A  ADD	R5	R5	K15
      0x7001FFEB,  //  002B  JMP		#0018
      0x8C28050D,  //  002C  GETMET	R10	R2	K13
      0x5C301200,  //  002D  MOVE	R12	R9
      0x58340010,  //  002E  LDCONST	R13	K16
      0x7C280600,  //  002F  CALL	R10	3
      0x1C28150C,  //  0030  EQ	R10	R10	K12
      0x782A0001,  //  0031  JMPF	R10	#0034
      0x00140B0F,  //  0032  ADD	R5	R5	K15
      0x7001FFE3,  //  0033  JMP		#0018
      0x88280107,  //  0034  GETMBR	R10	R0	K7
      0x88281511,  //  0035  GETMBR	R10	R10	K17
      0x8C28150D,  //  0036  GETMET	R10	R10	K13
      0x5C301200,  //  0037  MOVE	R12	R9
      0x7C280400,  //  0038  CALL	R10	2
      0x582C0012,  //  0039  LDCONST	R11	K18
      0x58300012,  //  003A  LDCONST	R12	K18
      0x4C340000,  //  003B  LDNIL	R13
      0x2034140D,  //  003C  NE	R13	R10	R13
      0x78360005,  //  003D  JMPF	R13	#0044
      0x8C341513,  //  003E  GETMET	R13	R10	K19
      0x5C3C1400,  //  003F  MOVE	R15	R10
      0x5C401000,  //  0040  MOVE	R16	R8
      0x7C340600,  //  0041  CALL	R13	3
      0x5C2C1A00,  //  0042  MOVE	R11	R13
      0x88301514,  //  0043  GETMBR	R12	R10	K20
      0x50180200,  //  0044  LDBOOL	R6	1	0
      0x8C340303,  //  0045  GETMET	R13	R1	K3
      0x603C0018,  //  0046  GETGBL	R15	G24
      0x58400015,  //  0047  LDCONST	R16	K21
      0x5C440E00,  //  0048  MOVE	R17	R7
      0x7C3C0400,  //  0049  CALL	R15	2
      0x7C340400,  //  004A  CALL	R13	2
      0x8C340303,  //  004B  GETMET	R13	R1	K3
      0x603C0018,  //  004C  GETGBL	R15	G24
      0x58400016,  //  004D  LDCONST	R16	K22
      0x5C440E00,  //  004E  MOVE	R17	R7
      0x8C480317,  //  004F  GETMET	R18	R1	K23
      0x8C50110D,  //  0050  GETMET	R20	R8	K13
      0x58580018,  //  0051  LDCONST	R22	K24
      0x585C0012,  //  0052  LDCONST	R23	K18
      0x7C500600,  //  0053  CALL	R20	3
      0x7C480400,  //  0054  CALL	R18	2
      0x7C3C0600,  //  0055  CALL	R15	3
      0x7C340400,  //  0056  CALL	R13	2
      0x8C340303,  //  0057  GETMET	R13	R1	K3
      0x603C0018,  //  0058  GETGBL	R15	G24
      0x58400019,  //  0059  LDCONST	R16	K25
      0x8C44011A,  //  005A  GETMET	R17	R0	K26
      0x8C4C110D,  //  005B  GETMET	R19	R8	K13
      0x5854000E,  //  005C  LDCONST	R21	K14
      0x58580012,  //  005D  LDCONST	R22	K18
      0x7C4C0600,  //  005E  CALL	R19	3
      0x7C440400,  //  005F  CALL	R17	2
      0x7C3C0400,  //  0060  CALL	R15	2
      0x7C340400,  //  0061  CALL	R13	2
      0x8C340303,  //  0062  GETMET	R13	R1	K3
      0x603C0018,  //  0063  GETGBL	R15	G24
      0x5840001B,  //  0064  LDCONST	R16	K27
      0x5C440E00,  //  0065  MOVE	R17	R7
      0x8C480317,  //  0066  GETMET	R18	R1	K23
      0x5C501600,  //  0067  MOVE	R20	R11
      0x7C480400,  //  0068  CALL	R18	2
      0x8C4C0317,  //  0069  GETMET	R19	R1	K23
      0x5C541800,  //  006A  MOVE	R21	R12
      0x7C4C0400,  //  006B  CALL	R19	2
      0x8C500317,  //  006C  GETMET	R20	R1	K23
      0x5C581800,  //  006D  MOVE	R22	R12
      0x7C500400,  //  006E  CALL	R20	2
      0x7C3C0A00,  //  006F  CALL	R15	5
      0x7C340400,  //  0070  CALL	R13	2
      0x8C340303,  //  0071  GETMET	R13	R1	K3
      0x603C0018,  //  0072  GETGBL	R15	G24
      0x5840001C,  //  0073  LDCONST	R16	K28
      0x5C440E00,  //  0074  MOVE	R17	R7
      0x5C480E00,  //  0075  MOVE	R18	R7
      0x5C4C0E00,  //  0076  MOVE	R19	R7
      0x7C3C0800,  //  0077  CALL	R15	4
      0x7C340400,  //  0078  CALL	R13	2
      0x00140B0F,  //  0079  ADD	R5	R5	K15
      0x7001FF9C,  //  007A  JMP		#0018
      0x8C1C0303,  //  007B  GETMET	R7	R1	K3
      0x5824001D,  //  007C  LDCONST	R9	K29
      0x7C1C0400,  //  007D  CALL	R7	2
      0x5C1C0C00,  //  007E  MOVE	R7	R6
      0x741E0002,  //  007F  JMPT	R7	#0083
      0x8C1C0303,  //  0080  GETMET	R7	R1	K3
      0x5824001E,  //  0081  LDCONST	R9	K30
      0x7C1C0400,  //  0082  CALL	R7	2
      0x8C1C0303,  //  0083  GETMET	R7	R1	K3
      0x5824001F,  //  0084  LDCONST	R9	K31
      0x7C1C0400,  //  0085  CALL	R7	2
      0x601C0012,  //  0086  GETGBL	R7	G18
      0x7C1C0000,  //  0087  CALL	R7	0
      0x60200010,  //  0088  GETGBL	R8	G16
      0x88240107,  //  0089  GETMBR	R9	R0	K7
      0x88241308,  //  008A  GETMBR	R9	R9	K8
      0x7C200200,  //  008B  CALL	R8	1
      0xA802000B,  //  008C  EXBLK	0	#0099
      0x5C241000,  //  008D  MOVE	R9	R8
      0x7C240000,  //  008E  CALL	R9	0
      0x8C28130D,  //  008F  GETMET	R10	R9	K13
      0x58300020,  //  0090  LDCONST	R12	K32
      0x7C280400,  //  0091  CALL	R10	2
      0x4C2C0000,  //  0092  LDNIL	R11
      0x202C140B,  //  0093  NE	R11	R10	R11
      0x782E0002,  //  0094  JMPF	R11	#0098
      0x8C2C0F21,  //  0095  GETMET	R11	R7	K33
      0x5C341400,  //  0096  MOVE	R13	R10
      0x7C2C0400,  //  0097  CALL	R11	2
      0x7001FFF3,  //  0098  JMP		#008D
      0x58200022,  //  0099  LDCONST	R8	K34
      0xAC200200,  //  009A  CATCH	R8	1	0
      0xB0080000,  //  009B  RAISE	2	R0	R0
      0x88200107,  //  009C  GETMBR	R8	R0	K7
      0x8C201123,  //  009D  GETMET	R8	R8	K35
      0x5C280E00,  //  009E  MOVE	R10	R7
      0x7C200400,  //  009F  CALL	R8	2
      0x60200010,  //  00A0  GETGBL	R8	G16
      0x5C240E00,  //  00A1  MOVE	R9	R7
      0x7C200200,  //  00A2  CALL	R8	1
      0xA8020084,  //  00A3  EXBLK	0	#0129
      0x5C241000,  //  00A4  MOVE	R9	R8
      0x7C240000,  //  00A5  CALL	R9	0
      0x8C280317,  //  00A6  GETMET	R10	R1	K23
      0x5C301200,  //  00A7  MOVE	R12	R9
      0x7C280400,  //  00A8  CALL	R10	2
      0x8C2C0317,  //  00A9  GETMET	R11	R1	K23
      0x88340107,  //  00AA  GETMBR	R13	R0	K7
      0x8C341B24,  //  00AB  GETMET	R13	R13	K36
      0x5C3C1200,  //  00AC  MOVE	R15	R9
      0x7C340400,  //  00AD  CALL	R13	2
      0x8C341B0D,  //  00AE  GETMET	R13	R13	K13
      0x583C0018,  //  00AF  LDCONST	R15	K24
      0x5C401200,  //  00B0  MOVE	R16	R9
      0x7C340600,  //  00B1  CALL	R13	3
      0x7C2C0400,  //  00B2  CALL	R11	2
      0x8C300303,  //  00B3  GETMET	R12	R1	K3
      0x60380018,  //  00B4  GETGBL	R14	G24
      0x583C0025,  //  00B5  LDCONST	R15	K37
      0x5C401400,  //  00B6  MOVE	R16	R10
      0x5C441400,  //  00B7  MOVE	R17	R10
      0x5C481600,  //  00B8  MOVE	R18	R11
      0x7C380800,  //  00B9  CALL	R14	4
      0x7C300400,  //  00BA  CALL	R12	2
      0x8C300303,  //  00BB  GETMET	R12	R1	K3
      0x58380026,  //  00BC  LDCONST	R14	K38
      0x7C300400,  //  00BD  CALL	R12	2
      0x50180000,  //  00BE  LDBOOL	R6	0	0
      0x5814000C,  //  00BF  LDCONST	R5	K12
      0x6030000C,  //  00C0  GETGBL	R12	G12
      0x5C340800,  //  00C1  MOVE	R13	R4
      0x7C300200,  //  00C2  CALL	R12	1
      0x14300A0C,  //  00C3  LT	R12	R5	R12
      0x7832005F,  //  00C4  JMPF	R12	#0125
      0x94300805,  //  00C5  GETIDX	R12	R4	R5
      0x88340107,  //  00C6  GETMBR	R13	R0	K7
      0x88341B08,  //  00C7  GETMBR	R13	R13	K8
      0x8C341B0D,  //  00C8  GETMET	R13	R13	K13
      0x603C0008,  //  00C9  GETGBL	R15	G8
      0x5C401800,  //  00CA  MOVE	R16	R12
      0x7C3C0200,  //  00CB  CALL	R15	1
      0x7C340400,  //  00CC  CALL	R13	2
      0x8C381B0D,  //  00CD  GETMET	R14	R13	K13
      0x5840000E,  //  00CE  LDCONST	R16	K14
      0x7C380400,  //  00CF  CALL	R14	2
      0x5C3C1C00,  //  00D0  MOVE	R15	R14
      0x743E0001,  //  00D1  JMPT	R15	#00D4
      0x00140B0F,  //  00D2  ADD	R5	R5	K15
      0x7001FFEB,  //  00D3  JMP		#00C0
      0x8C3C050D,  //  00D4  GETMET	R15	R2	K13
      0x5C441C00,  //  00D5  MOVE	R17	R14
      0x58480010,  //  00D6  LDCONST	R18	K16
      0x7C3C0600,  //  00D7  CALL	R15	3
      0x203C1F0C,  //  00D8  NE	R15	R15	K12
      0x783E0001,  //  00D9  JMPF	R15	#00DC
      0x00140B0F,  //  00DA  ADD	R5	R5	K15
      0x7001FFE3,  //  00DB  JMP		#00C0
      0x8C3C1B0D,  //  00DC  GETMET	R15	R13	K13
      0x58440020,  //  00DD  LDCONST	R17	K32
      0x7C3C0400,  //  00DE  CALL	R15	2
      0x203C1E09,  //  00DF  NE	R15	R15	R9
      0x783E0001,  //  00E0  JMPF	R15	#00E3
      0x00140B0F,  //  00E1  ADD	R5	R5	K15
      0x7001FFDC,  //  00E2  JMP		#00C0
      0x883C0107,  //  00E3  GETMBR	R15	R0	K7
      0x883C1F11,  //  00E4  GETMBR	R15	R15	K17
      0x8C3C1F0D,  //  00E5  GETMET	R15	R15	K13
      0x5C441C00,  //  00E6  MOVE	R17	R14
      0x7C3C0400,  //  00E7  CALL	R15	2
      0x58400012,  //  00E8  LDCONST	R16	K18
      0x58440012,  //  00E9  LDCONST	R17	K18
      0x4C480000,  //  00EA  LDNIL	R18
      0x20481E12,  //  00EB  NE	R18	R15	R18
      0x784A0005,  //  00EC  JMPF	R18	#00F3
      0x8C481F13,  //  00ED  GETMET	R18	R15	K19
      0x5C501E00,  //  00EE  MOVE	R20	R15
      0x5C541A00,  //  00EF  MOVE	R21	R13
      0x7C480600,  //  00F0  CALL	R18	3
      0x5C402400,  //  00F1  MOVE	R16	R18
      0x88441F14,  //  00F2  GETMBR	R17	R15	K20
      0x50180200,  //  00F3  LDBOOL	R6	1	0
      0x8C480303,  //  00F4  GETMET	R18	R1	K3
      0x60500018,  //  00F5  GETGBL	R20	G24
      0x58540027,  //  00F6  LDCONST	R21	K39
      0x5C581800,  //  00F7  MOVE	R22	R12
      0x7C500400,  //  00F8  CALL	R20	2
      0x7C480400,  //  00F9  CALL	R18	2
      0x8C480303,  //  00FA  GETMET	R18	R1	K3
      0x60500018,  //  00FB  GETGBL	R20	G24
      0x58540028,  //  00FC  LDCONST	R21	K40
      0x5C581800,  //  00FD  MOVE	R22	R12
      0x8C5C0317,  //  00FE  GETMET	R23	R1	K23
      0x8C641B0D,  //  00FF  GETMET	R25	R13	K13
      0x586C0018,  //  0100  LDCONST	R27	K24
      0x58700012,  //  0101  LDCONST	R28	K18
      0x7C640600,  //  0102  CALL	R25	3
      0x7C5C0400,  //  0103  CALL	R23	2
      0x7C500600,  //  0104  CALL	R20	3
      0x7C480400,  //  0105  CALL	R18	2
      0x8C480303,  //  0106  GETMET	R18	R1	K3
      0x60500018,  //  0107  GETGBL	R20	G24
      0x58540029,  //  0108  LDCONST	R21	K41
      0x8C58011A,  //  0109  GETMET	R22	R0	K26
      0x8C601B0D,  //  010A  GETMET	R24	R13	K13
      0x5868000E,  //  010B  LDCONST	R26	K14
      0x586C0012,  //  010C  LDCONST	R27	K18
      0x7C600600,  //  010D  CALL	R24	3
      0x7C580400,  //  010E  CALL	R22	2
      0x7C500400,  //  010F  CALL	R20	2
      0x7C480400,  //  0110  CALL	R18	2
      0x8C480303,  //  0111  GETMET	R18	R1	K3
      0x60500018,  //  0112  GETGBL	R20	G24
      0x5854002A,  //  0113  LDCONST	R21	K42
      0x5C581800,  //  0114  MOVE	R22	R12
      0x8C5C0317,  //  0115  GETMET	R23	R1	K23
      0x5C642000,  //  0116  MOVE	R25	R16
      0x7C5C0400,  //  0117  CALL	R23	2
      0x8C600317,  //  0118  GETMET	R24	R1	K23
      0x5C682200,  //  0119  MOVE	R26	R17
      0x7C600400,  //  011A  CALL	R24	2
      0x7C500800,  //  011B  CALL	R20	4
      0x7C480400,  //  011C  CALL	R18	2
      0x8C480303,  //  011D  GETMET	R18	R1	K3
      0x60500018,  //  011E  GETGBL	R20	G24
      0x5854002B,  //  011F  LDCONST	R21	K43
      0x5C581800,  //  0120  MOVE	R22	R12
      0x7C500400,  //  0121  CALL	R20	2
      0x7C480400,  //  0122  CALL	R18	2
      0x00140B0F,  //  0123  ADD	R5	R5	K15
      0x7001FF9A,  //  0124  JMP		#00C0
      0x8C300303,  //  0125  GETMET	R12	R1	K3
      0x5838002C,  //  0126  LDCONST	R14	K44
      0x7C300400,  //  0127  CALL	R12	2
      0x7001FF7A,  //  0128  JMP		#00A4
      0x58200022,  //  0129  LDCONST	R8	K34
      0xAC200200,  //  012A  CATCH	R8	1	0
      0xB0080000,  //  012B  RAISE	2	R0	R0
      0x5C200C00,  //  012C  MOVE	R8	R6
      0x74220002,  //  012D  JMPT	R8	#0131
      0x8C200303,  //  012E  GETMET	R8	R1	K3
      0x5828001E,  //  012F  LDCONST	R10	K30
      0x7C200400,  //  0130  CALL	R8	2
      0x8C200303,  //  0131  GETMET	R8	R1	K3
      0x5828002D,  //  0132  LDCONST	R10	K45
      0x7C200400,  //  0133  CALL	R8	2
      0x8C20012E,  //  0134  GETMET	R8	R0	K46
      0x8828012F,  //  0135  GETMBR	R10	R0	K47
      0x7C200400,  //  0136  CALL	R8	2
      0x8C200303,  //  0137  GETMET	R8	R1	K3
      0x58280030,  //  0138  LDCONST	R10	K48
      0x7C200400,  //  0139  CALL	R8	2
      0x8C200131,  //  013A  GETMET	R8	R0	K49
      0x58280012,  //  013B  LDCONST	R10	K18
      0x882C012F,  //  013C  GETMBR	R11	R0	K47
      0x7C200600,  //  013D  CALL	R8	3
      0x8C200303,  //  013E  GETMET	R8	R1	K3
      0x58280032,  //  013F  LDCONST	R10	K50
      0x7C200400,  //  0140  CALL	R8	2
      0x80000000,  //  0141  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_commissioning_info
********************************************************************/
be_local_closure(class_Matter_UI_show_commissioning_info,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(commissioning),
    /* K3   */  be_nested_str_weak(commissioning_open),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(millis),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(content_send),
    /* K8   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BCommissioning_X20open_X20for_X20_X25i_X20min_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K9   */  be_nested_str_weak(compute_manual_pairing_code),
    /* K10  */  be_nested_str_weak(_X3Cp_X3EManual_X20pairing_X20code_X3A_X3Cbr_X3E_X3Cb_X3E_X25s_X2D_X25s_X2D_X25s_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Chr_X3E),
    /* K11  */  be_const_int(3),
    /* K12  */  be_const_int(2147483647),
    /* K13  */  be_nested_str_weak(_X3Cdiv_X3E_X3Ccenter_X3E),
    /* K14  */  be_nested_str_weak(compute_qrcode_content),
    /* K15  */  be_nested_str_weak(show_qrcode),
    /* K16  */  be_nested_str_weak(_X3Cp_X3E_X20_X25s_X3C_X2Fp_X3E),
    /* K17  */  be_nested_str_weak(_X3C_X2Fdiv_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_commissioning_info),
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x88080502,  //  0002  GETMBR	R2	R2	K2
      0x88080503,  //  0003  GETMBR	R2	R2	K3
      0xB80E0800,  //  0004  GETNGBL	R3	K4
      0x8C0C0705,  //  0005  GETMET	R3	R3	K5
      0x7C0C0200,  //  0006  CALL	R3	1
      0x04080403,  //  0007  SUB	R2	R2	R3
      0x540E03E7,  //  0008  LDINT	R3	1000
      0x0C080403,  //  0009  DIV	R2	R2	R3
      0x140C0506,  //  000A  LT	R3	R2	K6
      0x780E0000,  //  000B  JMPF	R3	#000D
      0x58080006,  //  000C  LDCONST	R2	K6
      0x540E001D,  //  000D  LDINT	R3	30
      0x000C0403,  //  000E  ADD	R3	R2	R3
      0x5412003B,  //  000F  LDINT	R4	60
      0x0C0C0604,  //  0010  DIV	R3	R3	R4
      0x8C100307,  //  0011  GETMET	R4	R1	K7
      0x60180018,  //  0012  GETGBL	R6	G24
      0x581C0008,  //  0013  LDCONST	R7	K8
      0x5C200600,  //  0014  MOVE	R8	R3
      0x7C180400,  //  0015  CALL	R6	2
      0x7C100400,  //  0016  CALL	R4	2
      0x88100101,  //  0017  GETMBR	R4	R0	K1
      0x88100902,  //  0018  GETMBR	R4	R4	K2
      0x8C100909,  //  0019  GETMET	R4	R4	K9
      0x7C100200,  //  001A  CALL	R4	1
      0x8C140307,  //  001B  GETMET	R5	R1	K7
      0x601C0018,  //  001C  GETGBL	R7	G24
      0x5820000A,  //  001D  LDCONST	R8	K10
      0x40260D0B,  //  001E  CONNECT	R9	K6	K11
      0x94240809,  //  001F  GETIDX	R9	R4	R9
      0x542A0003,  //  0020  LDINT	R10	4
      0x542E0005,  //  0021  LDINT	R11	6
      0x4028140B,  //  0022  CONNECT	R10	R10	R11
      0x9428080A,  //  0023  GETIDX	R10	R4	R10
      0x542E0006,  //  0024  LDINT	R11	7
      0x402C170C,  //  0025  CONNECT	R11	R11	K12
      0x942C080B,  //  0026  GETIDX	R11	R4	R11
      0x7C1C0800,  //  0027  CALL	R7	4
      0x7C140400,  //  0028  CALL	R5	2
      0x8C140307,  //  0029  GETMET	R5	R1	K7
      0x581C000D,  //  002A  LDCONST	R7	K13
      0x7C140400,  //  002B  CALL	R5	2
      0x88140101,  //  002C  GETMBR	R5	R0	K1
      0x88140B02,  //  002D  GETMBR	R5	R5	K2
      0x8C140B0E,  //  002E  GETMET	R5	R5	K14
      0x7C140200,  //  002F  CALL	R5	1
      0x8C18010F,  //  0030  GETMET	R6	R0	K15
      0x5C200A00,  //  0031  MOVE	R8	R5
      0x7C180400,  //  0032  CALL	R6	2
      0x8C180307,  //  0033  GETMET	R6	R1	K7
      0x60200018,  //  0034  GETGBL	R8	G24
      0x58240010,  //  0035  LDCONST	R9	K16
      0x5C280A00,  //  0036  MOVE	R10	R5
      0x7C200400,  //  0037  CALL	R8	2
      0x7C180400,  //  0038  CALL	R6	2
      0x8C180307,  //  0039  GETMET	R6	R1	K7
      0x58200011,  //  003A  LDCONST	R8	K17
      0x7C180400,  //  003B  CALL	R6	2
      0x80000000,  //  003C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_remote_autoconf
********************************************************************/
be_local_closure(class_Matter_UI_show_remote_autoconf,   /* name */
  be_nested_proto(
    27,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[43]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Plugin_Device),
    /* K5   */  be_nested_str_weak(PROBE_TIMEOUT),
    /* K6   */  be_nested_str_weak(HTTP_remote),
    /* K7   */  be_nested_str_weak(call_sync),
    /* K8   */  be_nested_str_weak(Status_X2010),
    /* K9   */  be_nested_str_weak(load),
    /* K10  */  be_nested_str_weak(find),
    /* K11  */  be_nested_str_weak(StatusSNS),
    /* K12  */  be_nested_str_weak(Status_X2011),
    /* K13  */  be_nested_str_weak(StatusSTS),
    /* K14  */  be_nested_str_weak(log),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20probed_X20_X27_X25s_X27_X20status10_X3D_X25s_X20satus11_X3D_X25s),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(generate_config_from_status),
    /* K18  */  be_nested_str_weak(show_plugins_hints_js),
    /* K19  */  be_nested_str_weak(_CLASSES_TYPES2),
    /* K20  */  be_nested_str_weak(content_send),
    /* K21  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Remote_X20Device_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K22  */  be_nested_str_weak(html_escape),
    /* K23  */  be_nested_str_weak(_X3Cp_X3E_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X3E_X25s_X3C_X2Fa_X3E_X3C_X2Fp_X3E),
    /* K24  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X27100_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EName_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EParameter_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K25  */  be_nested_str_weak(_X3Cinput_X20name_X3D_X27url_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X25s_X27_X3E),
    /* K26  */  be_const_int(0),
    /* K27  */  be_nested_str_weak(type),
    /* K28  */  be_nested_str_weak(http_),
    /* K29  */  be_nested_str_weak(device),
    /* K30  */  be_nested_str_weak(plugins_classes),
    /* K31  */  be_nested_str_weak(ui_conf_to_string),
    /* K32  */  be_nested_str_weak(ARG_HINT),
    /* K33  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X27_X20placeholder_X3D_X27_X28optional_X29_X27_X3E_X3C_X2Ftd_X3E),
    /* K34  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cselect_X20name_X3D_X27pi_X25i_X27_X20onchange_X3D_X27otm_X28_X22arg_X25i_X22_X2Cthis_X2Evalue_X29_X27_X3E),
    /* K35  */  be_nested_str_weak(plugin_option),
    /* K36  */  be_nested_str_weak(_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E),
    /* K37  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27text_X27_X20id_X3D_X27arg_X25i_X27_X20name_X3D_X27arg_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X20placeholder_X3D_X27_X25s_X27_X20title_X3D_X27_X25s_X27_X3E),
    /* K38  */  be_nested_str_weak(_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K39  */  be_const_int(1),
    /* K40  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27text_X27_X20id_X3D_X27arg_X25i_X27_X20name_X3D_X27arg_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X3E),
    /* K41  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E_X3Cbutton_X20name_X3D_X27addrem_X27_X20class_X3D_X27button_X20bgrn_X27_X3EAdd_X20endpoints_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fform_X3E_X3C_X2Ffieldset_X3E),
    /* K42  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3EUnable_X20to_X20connect_X20to_X20_X27_X25s_X27_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_remote_autoconf),
    &be_const_str_solidified,
    ( &(const binstruction[220]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x1C100302,  //  0002  EQ	R4	R1	K2
      0x78120000,  //  0003  JMPF	R4	#0005
      0x80000800,  //  0004  RET	0
      0xB8120600,  //  0005  GETNGBL	R4	K3
      0x88100904,  //  0006  GETMBR	R4	R4	K4
      0x88100905,  //  0007  GETMBR	R4	R4	K5
      0xB8160600,  //  0008  GETNGBL	R5	K3
      0x8C140B06,  //  0009  GETMET	R5	R5	K6
      0x4C1C0000,  //  000A  LDNIL	R7
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240800,  //  000C  MOVE	R9	R4
      0x7C140800,  //  000D  CALL	R5	4
      0x8C180B07,  //  000E  GETMET	R6	R5	K7
      0x58200008,  //  000F  LDCONST	R8	K8
      0x5C240800,  //  0010  MOVE	R9	R4
      0x7C180600,  //  0011  CALL	R6	3
      0x4C1C0000,  //  0012  LDNIL	R7
      0x201C0C07,  //  0013  NE	R7	R6	R7
      0x781E0003,  //  0014  JMPF	R7	#0019
      0x8C1C0709,  //  0015  GETMET	R7	R3	K9
      0x5C240C00,  //  0016  MOVE	R9	R6
      0x7C1C0400,  //  0017  CALL	R7	2
      0x5C180E00,  //  0018  MOVE	R6	R7
      0x4C1C0000,  //  0019  LDNIL	R7
      0x201C0C07,  //  001A  NE	R7	R6	R7
      0x781E0003,  //  001B  JMPF	R7	#0020
      0x8C1C0D0A,  //  001C  GETMET	R7	R6	K10
      0x5824000B,  //  001D  LDCONST	R9	K11
      0x7C1C0400,  //  001E  CALL	R7	2
      0x5C180E00,  //  001F  MOVE	R6	R7
      0x4C1C0000,  //  0020  LDNIL	R7
      0x4C200000,  //  0021  LDNIL	R8
      0x20200C08,  //  0022  NE	R8	R6	R8
      0x78220012,  //  0023  JMPF	R8	#0037
      0x8C200B07,  //  0024  GETMET	R8	R5	K7
      0x5828000C,  //  0025  LDCONST	R10	K12
      0x5C2C0800,  //  0026  MOVE	R11	R4
      0x7C200600,  //  0027  CALL	R8	3
      0x5C1C1000,  //  0028  MOVE	R7	R8
      0x4C200000,  //  0029  LDNIL	R8
      0x20200E08,  //  002A  NE	R8	R7	R8
      0x78220003,  //  002B  JMPF	R8	#0030
      0x8C200709,  //  002C  GETMET	R8	R3	K9
      0x5C280E00,  //  002D  MOVE	R10	R7
      0x7C200400,  //  002E  CALL	R8	2
      0x5C1C1000,  //  002F  MOVE	R7	R8
      0x4C200000,  //  0030  LDNIL	R8
      0x20200E08,  //  0031  NE	R8	R7	R8
      0x78220003,  //  0032  JMPF	R8	#0037
      0x8C200F0A,  //  0033  GETMET	R8	R7	K10
      0x5828000D,  //  0034  LDCONST	R10	K13
      0x7C200400,  //  0035  CALL	R8	2
      0x5C1C1000,  //  0036  MOVE	R7	R8
      0x4C200000,  //  0037  LDNIL	R8
      0x20200C08,  //  0038  NE	R8	R6	R8
      0x78220098,  //  0039  JMPF	R8	#00D3
      0x4C200000,  //  003A  LDNIL	R8
      0x20200E08,  //  003B  NE	R8	R7	R8
      0x78220095,  //  003C  JMPF	R8	#00D3
      0xB8221C00,  //  003D  GETNGBL	R8	K14
      0x60240018,  //  003E  GETGBL	R9	G24
      0x5828000F,  //  003F  LDCONST	R10	K15
      0x5C2C0200,  //  0040  MOVE	R11	R1
      0x60300008,  //  0041  GETGBL	R12	G8
      0x5C340C00,  //  0042  MOVE	R13	R6
      0x7C300200,  //  0043  CALL	R12	1
      0x60340008,  //  0044  GETGBL	R13	G8
      0x5C380E00,  //  0045  MOVE	R14	R7
      0x7C340200,  //  0046  CALL	R13	1
      0x7C240800,  //  0047  CALL	R9	4
      0x58280010,  //  0048  LDCONST	R10	K16
      0x7C200400,  //  0049  CALL	R8	2
      0x8C200111,  //  004A  GETMET	R8	R0	K17
      0x5C280C00,  //  004B  MOVE	R10	R6
      0x5C2C0E00,  //  004C  MOVE	R11	R7
      0x7C200600,  //  004D  CALL	R8	3
      0x8C240112,  //  004E  GETMET	R9	R0	K18
      0x882C0113,  //  004F  GETMBR	R11	R0	K19
      0x7C240400,  //  0050  CALL	R9	2
      0x8C240514,  //  0051  GETMET	R9	R2	K20
      0x582C0015,  //  0052  LDCONST	R11	K21
      0x7C240400,  //  0053  CALL	R9	2
      0x8C240516,  //  0054  GETMET	R9	R2	K22
      0x5C2C0200,  //  0055  MOVE	R11	R1
      0x7C240400,  //  0056  CALL	R9	2
      0x8C280514,  //  0057  GETMET	R10	R2	K20
      0x60300018,  //  0058  GETGBL	R12	G24
      0x58340017,  //  0059  LDCONST	R13	K23
      0x5C381200,  //  005A  MOVE	R14	R9
      0x5C3C1200,  //  005B  MOVE	R15	R9
      0x7C300600,  //  005C  CALL	R12	3
      0x7C280400,  //  005D  CALL	R10	2
      0x8C280514,  //  005E  GETMET	R10	R2	K20
      0x58300018,  //  005F  LDCONST	R12	K24
      0x7C280400,  //  0060  CALL	R10	2
      0x8C280514,  //  0061  GETMET	R10	R2	K20
      0x60300018,  //  0062  GETGBL	R12	G24
      0x58340019,  //  0063  LDCONST	R13	K25
      0x8C380516,  //  0064  GETMET	R14	R2	K22
      0x5C400200,  //  0065  MOVE	R16	R1
      0x7C380400,  //  0066  CALL	R14	2
      0x7C300400,  //  0067  CALL	R12	2
      0x7C280400,  //  0068  CALL	R10	2
      0x5828001A,  //  0069  LDCONST	R10	K26
      0x602C000C,  //  006A  GETGBL	R11	G12
      0x5C301000,  //  006B  MOVE	R12	R8
      0x7C2C0200,  //  006C  CALL	R11	1
      0x142C140B,  //  006D  LT	R11	R10	R11
      0x782E0040,  //  006E  JMPF	R11	#00B0
      0x942C100A,  //  006F  GETIDX	R11	R8	R10
      0x8C30170A,  //  0070  GETMET	R12	R11	K10
      0x5838001B,  //  0071  LDCONST	R14	K27
      0x583C0002,  //  0072  LDCONST	R15	K2
      0x7C300600,  //  0073  CALL	R12	3
      0x20341902,  //  0074  NE	R13	R12	K2
      0x78360000,  //  0075  JMPF	R13	#0077
      0x0032380C,  //  0076  ADD	R12	K28	R12
      0x8834011D,  //  0077  GETMBR	R13	R0	K29
      0x88341B1E,  //  0078  GETMBR	R13	R13	K30
      0x8C341B0A,  //  0079  GETMET	R13	R13	K10
      0x5C3C1800,  //  007A  MOVE	R15	R12
      0x7C340400,  //  007B  CALL	R13	2
      0x58380002,  //  007C  LDCONST	R14	K2
      0x583C0002,  //  007D  LDCONST	R15	K2
      0x4C400000,  //  007E  LDNIL	R16
      0x20401A10,  //  007F  NE	R16	R13	R16
      0x78420005,  //  0080  JMPF	R16	#0087
      0x8C401B1F,  //  0081  GETMET	R16	R13	K31
      0x5C481A00,  //  0082  MOVE	R18	R13
      0x5C4C1600,  //  0083  MOVE	R19	R11
      0x7C400600,  //  0084  CALL	R16	3
      0x5C382000,  //  0085  MOVE	R14	R16
      0x883C1B20,  //  0086  GETMBR	R15	R13	K32
      0x8C400514,  //  0087  GETMET	R16	R2	K20
      0x60480018,  //  0088  GETGBL	R18	G24
      0x584C0021,  //  0089  LDCONST	R19	K33
      0x5C501400,  //  008A  MOVE	R20	R10
      0x7C480400,  //  008B  CALL	R18	2
      0x7C400400,  //  008C  CALL	R16	2
      0x8C400514,  //  008D  GETMET	R16	R2	K20
      0x60480018,  //  008E  GETGBL	R18	G24
      0x584C0022,  //  008F  LDCONST	R19	K34
      0x5C501400,  //  0090  MOVE	R20	R10
      0x5C541400,  //  0091  MOVE	R21	R10
      0x7C480600,  //  0092  CALL	R18	3
      0x7C400400,  //  0093  CALL	R16	2
      0x8C400123,  //  0094  GETMET	R16	R0	K35
      0x5C481800,  //  0095  MOVE	R18	R12
      0x884C0113,  //  0096  GETMBR	R19	R0	K19
      0x7C400600,  //  0097  CALL	R16	3
      0x8C400514,  //  0098  GETMET	R16	R2	K20
      0x58480024,  //  0099  LDCONST	R18	K36
      0x7C400400,  //  009A  CALL	R16	2
      0x8C400514,  //  009B  GETMET	R16	R2	K20
      0x60480018,  //  009C  GETGBL	R18	G24
      0x584C0025,  //  009D  LDCONST	R19	K37
      0x5C501400,  //  009E  MOVE	R20	R10
      0x5C541400,  //  009F  MOVE	R21	R10
      0x8C580516,  //  00A0  GETMET	R22	R2	K22
      0x5C601C00,  //  00A1  MOVE	R24	R14
      0x7C580400,  //  00A2  CALL	R22	2
      0x8C5C0516,  //  00A3  GETMET	R23	R2	K22
      0x5C641E00,  //  00A4  MOVE	R25	R15
      0x7C5C0400,  //  00A5  CALL	R23	2
      0x8C600516,  //  00A6  GETMET	R24	R2	K22
      0x5C681E00,  //  00A7  MOVE	R26	R15
      0x7C600400,  //  00A8  CALL	R24	2
      0x7C480C00,  //  00A9  CALL	R18	6
      0x7C400400,  //  00AA  CALL	R16	2
      0x8C400514,  //  00AB  GETMET	R16	R2	K20
      0x58480026,  //  00AC  LDCONST	R18	K38
      0x7C400400,  //  00AD  CALL	R16	2
      0x00281527,  //  00AE  ADD	R10	R10	K39
      0x7001FFB9,  //  00AF  JMP		#006A
      0x8C2C0514,  //  00B0  GETMET	R11	R2	K20
      0x60340018,  //  00B1  GETGBL	R13	G24
      0x58380021,  //  00B2  LDCONST	R14	K33
      0x5C3C1400,  //  00B3  MOVE	R15	R10
      0x7C340400,  //  00B4  CALL	R13	2
      0x7C2C0400,  //  00B5  CALL	R11	2
      0x8C2C0514,  //  00B6  GETMET	R11	R2	K20
      0x60340018,  //  00B7  GETGBL	R13	G24
      0x58380022,  //  00B8  LDCONST	R14	K34
      0x5C3C1400,  //  00B9  MOVE	R15	R10
      0x5C401400,  //  00BA  MOVE	R16	R10
      0x7C340600,  //  00BB  CALL	R13	3
      0x7C2C0400,  //  00BC  CALL	R11	2
      0x8C2C0123,  //  00BD  GETMET	R11	R0	K35
      0x58340002,  //  00BE  LDCONST	R13	K2
      0x88380113,  //  00BF  GETMBR	R14	R0	K19
      0x7C2C0600,  //  00C0  CALL	R11	3
      0x8C2C0514,  //  00C1  GETMET	R11	R2	K20
      0x58340024,  //  00C2  LDCONST	R13	K36
      0x7C2C0400,  //  00C3  CALL	R11	2
      0x8C2C0514,  //  00C4  GETMET	R11	R2	K20
      0x60340018,  //  00C5  GETGBL	R13	G24
      0x58380028,  //  00C6  LDCONST	R14	K40
      0x5C3C1400,  //  00C7  MOVE	R15	R10
      0x5C401400,  //  00C8  MOVE	R16	R10
      0x58440002,  //  00C9  LDCONST	R17	K2
      0x7C340800,  //  00CA  CALL	R13	4
      0x7C2C0400,  //  00CB  CALL	R11	2
      0x8C2C0514,  //  00CC  GETMET	R11	R2	K20
      0x58340026,  //  00CD  LDCONST	R13	K38
      0x7C2C0400,  //  00CE  CALL	R11	2
      0x8C2C0514,  //  00CF  GETMET	R11	R2	K20
      0x58340029,  //  00D0  LDCONST	R13	K41
      0x7C2C0400,  //  00D1  CALL	R11	2
      0x70020007,  //  00D2  JMP		#00DB
      0x8C200514,  //  00D3  GETMET	R8	R2	K20
      0x60280018,  //  00D4  GETGBL	R10	G24
      0x582C002A,  //  00D5  LDCONST	R11	K42
      0x8C300516,  //  00D6  GETMET	R12	R2	K22
      0x5C380200,  //  00D7  MOVE	R14	R1
      0x7C300400,  //  00D8  CALL	R12	2
      0x7C280400,  //  00D9  CALL	R10	2
      0x7C200400,  //  00DA  CALL	R8	2
      0x80000000,  //  00DB  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_fabric_info
********************************************************************/
be_local_closure(class_Matter_UI_show_fabric_info,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(content_send),
    /* K2   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BFabrics_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X3EAssociated_X20fabrics_X3A_X3C_X2Fp_X3E),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(sessions),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3ENone_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K7   */  be_nested_str_weak(fabrics),
    /* K8   */  be_nested_str_weak(persistables),
    /* K9   */  be_nested_str_weak(_X3Chr_X3E),
    /* K10  */  be_nested_str_weak(fabric_label),
    /* K11  */  be_nested_str_weak(_X3CNo_X20label_X3E),
    /* K12  */  be_nested_str_weak(html_escape),
    /* K13  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3B_X23_X25i_X20_X25s_X3C_X2Fb_X3E_X20_X28_X25s_X29_X26nbsp_X3B_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K14  */  be_nested_str_weak(get_fabric_index),
    /* K15  */  be_nested_str_weak(get_admin_vendor_name),
    /* K16  */  be_nested_str_weak(get_fabric_id),
    /* K17  */  be_nested_str_weak(copy),
    /* K18  */  be_nested_str_weak(reverse),
    /* K19  */  be_nested_str_weak(get_device_id),
    /* K20  */  be_nested_str_weak(Fabric_X3A_X20_X25s_X3Cbr_X3E),
    /* K21  */  be_nested_str_weak(tohex),
    /* K22  */  be_nested_str_weak(Device_X3A_X20_X25s_X3Cbr_X3E_X26nbsp_X3B),
    /* K23  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22Are_X20you_X20sure_X3F_X22_X29_X3B_X27_X3E),
    /* K24  */  be_nested_str_weak(_X3Cinput_X20name_X3D_X27del_fabric_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K25  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27del_X27_X20class_X3D_X27button_X20bgrn_X27_X3EDelete_X20Fabric_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K26  */  be_nested_str_weak(stop_iteration),
    /* K27  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_fabric_info),
    &be_const_str_solidified,
    ( &(const binstruction[97]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x6008000C,  //  0004  GETGBL	R2	G12
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x880C0704,  //  0006  GETMBR	R3	R3	K4
      0x880C0704,  //  0007  GETMBR	R3	R3	K4
      0x7C080200,  //  0008  CALL	R2	1
      0x1C080505,  //  0009  EQ	R2	R2	K5
      0x780A0003,  //  000A  JMPF	R2	#000F
      0x8C080301,  //  000B  GETMET	R2	R1	K1
      0x58100006,  //  000C  LDCONST	R4	K6
      0x7C080400,  //  000D  CALL	R2	2
      0x7002004D,  //  000E  JMP		#005D
      0x50080200,  //  000F  LDBOOL	R2	1	0
      0x600C0010,  //  0010  GETGBL	R3	G16
      0x88100103,  //  0011  GETMBR	R4	R0	K3
      0x88100904,  //  0012  GETMBR	R4	R4	K4
      0x88100907,  //  0013  GETMBR	R4	R4	K7
      0x8C100908,  //  0014  GETMET	R4	R4	K8
      0x7C100200,  //  0015  CALL	R4	1
      0x7C0C0200,  //  0016  CALL	R3	1
      0xA8020041,  //  0017  EXBLK	0	#005A
      0x5C100600,  //  0018  MOVE	R4	R3
      0x7C100000,  //  0019  CALL	R4	0
      0x5C140400,  //  001A  MOVE	R5	R2
      0x74160002,  //  001B  JMPT	R5	#001F
      0x8C140301,  //  001C  GETMET	R5	R1	K1
      0x581C0009,  //  001D  LDCONST	R7	K9
      0x7C140400,  //  001E  CALL	R5	2
      0x50080000,  //  001F  LDBOOL	R2	0	0
      0x8814090A,  //  0020  GETMBR	R5	R4	K10
      0x5C180A00,  //  0021  MOVE	R6	R5
      0x741A0000,  //  0022  JMPT	R6	#0024
      0x5814000B,  //  0023  LDCONST	R5	K11
      0x8C18030C,  //  0024  GETMET	R6	R1	K12
      0x5C200A00,  //  0025  MOVE	R8	R5
      0x7C180400,  //  0026  CALL	R6	2
      0x5C140C00,  //  0027  MOVE	R5	R6
      0x8C180301,  //  0028  GETMET	R6	R1	K1
      0x60200018,  //  0029  GETGBL	R8	G24
      0x5824000D,  //  002A  LDCONST	R9	K13
      0x8C28090E,  //  002B  GETMET	R10	R4	K14
      0x7C280200,  //  002C  CALL	R10	1
      0x5C2C0A00,  //  002D  MOVE	R11	R5
      0x8C30090F,  //  002E  GETMET	R12	R4	K15
      0x7C300200,  //  002F  CALL	R12	1
      0x7C200800,  //  0030  CALL	R8	4
      0x7C180400,  //  0031  CALL	R6	2
      0x8C180910,  //  0032  GETMET	R6	R4	K16
      0x7C180200,  //  0033  CALL	R6	1
      0x8C180D11,  //  0034  GETMET	R6	R6	K17
      0x7C180200,  //  0035  CALL	R6	1
      0x8C180D12,  //  0036  GETMET	R6	R6	K18
      0x7C180200,  //  0037  CALL	R6	1
      0x8C1C0913,  //  0038  GETMET	R7	R4	K19
      0x7C1C0200,  //  0039  CALL	R7	1
      0x8C1C0F11,  //  003A  GETMET	R7	R7	K17
      0x7C1C0200,  //  003B  CALL	R7	1
      0x8C1C0F12,  //  003C  GETMET	R7	R7	K18
      0x7C1C0200,  //  003D  CALL	R7	1
      0x8C200301,  //  003E  GETMET	R8	R1	K1
      0x60280018,  //  003F  GETGBL	R10	G24
      0x582C0014,  //  0040  LDCONST	R11	K20
      0x8C300D15,  //  0041  GETMET	R12	R6	K21
      0x7C300200,  //  0042  CALL	R12	1
      0x7C280400,  //  0043  CALL	R10	2
      0x7C200400,  //  0044  CALL	R8	2
      0x8C200301,  //  0045  GETMET	R8	R1	K1
      0x60280018,  //  0046  GETGBL	R10	G24
      0x582C0016,  //  0047  LDCONST	R11	K22
      0x8C300F15,  //  0048  GETMET	R12	R7	K21
      0x7C300200,  //  0049  CALL	R12	1
      0x7C280400,  //  004A  CALL	R10	2
      0x7C200400,  //  004B  CALL	R8	2
      0x8C200301,  //  004C  GETMET	R8	R1	K1
      0x58280017,  //  004D  LDCONST	R10	K23
      0x7C200400,  //  004E  CALL	R8	2
      0x8C200301,  //  004F  GETMET	R8	R1	K1
      0x60280018,  //  0050  GETGBL	R10	G24
      0x582C0018,  //  0051  LDCONST	R11	K24
      0x8C30090E,  //  0052  GETMET	R12	R4	K14
      0x7C300200,  //  0053  CALL	R12	1
      0x7C280400,  //  0054  CALL	R10	2
      0x7C200400,  //  0055  CALL	R8	2
      0x8C200301,  //  0056  GETMET	R8	R1	K1
      0x58280019,  //  0057  LDCONST	R10	K25
      0x7C200400,  //  0058  CALL	R8	2
      0x7001FFBD,  //  0059  JMP		#0018
      0x580C001A,  //  005A  LDCONST	R3	K26
      0xAC0C0200,  //  005B  CATCH	R3	1	0
      0xB0080000,  //  005C  RAISE	2	R0	R0
      0x8C080301,  //  005D  GETMET	R2	R1	K1
      0x5810001B,  //  005E  LDCONST	R4	K27
      0x7C080400,  //  005F  CALL	R2	2
      0x80000000,  //  0060  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_get_arg
********************************************************************/
be_local_closure(class_Matter_UI_web_get_arg,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(has_arg),
    /* K2   */  be_nested_str_weak(mtc0),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(commissioning),
    /* K5   */  be_nested_str_weak(stop_basic_commissioning),
    /* K6   */  be_nested_str_weak(mtc1),
    /* K7   */  be_nested_str_weak(start_root_basic_commissioning),
    }),
    be_str_weak(web_get_arg),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0004,  //  0004  JMPF	R2	#000A
      0x88080103,  //  0005  GETMBR	R2	R0	K3
      0x88080504,  //  0006  GETMBR	R2	R2	K4
      0x8C080505,  //  0007  GETMET	R2	R2	K5
      0x7C080200,  //  0008  CALL	R2	1
      0x70020006,  //  0009  JMP		#0011
      0x8C080301,  //  000A  GETMET	R2	R1	K1
      0x58100006,  //  000B  LDCONST	R4	K6
      0x7C080400,  //  000C  CALL	R2	2
      0x780A0002,  //  000D  JMPF	R2	#0011
      0x88080103,  //  000E  GETMBR	R2	R0	K3
      0x8C080507,  //  000F  GETMET	R2	R2	K7
      0x7C080200,  //  0010  CALL	R2	1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: plugin_option
********************************************************************/
be_local_closure(class_Matter_UI_plugin_option,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    3,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(split),
    /* K3   */  be_nested_str_weak(_X7C),
    /* K4   */  be_nested_str_weak(stop_iteration),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(),
    /* K7   */  be_nested_str_weak(content_send),
    /* K8   */  be_nested_str_weak(_X3Coption_X20value_X3D_X27_X27_X3E_X3C_X2Foption_X3E),
    /* K9   */  be_nested_str_weak(_X2Dvirtual),
    /* K10  */  be_nested_str_weak(_X3Coption_X20value_X3D_X27_X27_X20disabled_X3E_X2D_X2D_X2D_X20Virtual_X20Devices_X20_X2D_X2D_X2D_X3C_X2Foption_X3E),
    /* K11  */  be_nested_str_weak(device),
    /* K12  */  be_nested_str_weak(get_plugin_class_displayname),
    /* K13  */  be_nested_str_weak(_X3Coption_X20value_X3D_X27_X25s_X27_X25s_X3E_X25s_X3C_X2Foption_X3E),
    /* K14  */  be_nested_str_weak(_X20selected),
    /* K15  */  be_const_int(1),
    }),
    be_str_weak(plugin_option),
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0x60140012,  //  0002  GETGBL	R5	G18
      0x7C140000,  //  0003  CALL	R5	0
      0x60180010,  //  0004  GETGBL	R6	G16
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x7C180200,  //  0006  CALL	R6	1
      0xA8020007,  //  0007  EXBLK	0	#0010
      0x5C1C0C00,  //  0008  MOVE	R7	R6
      0x7C1C0000,  //  0009  CALL	R7	0
      0x8C200902,  //  000A  GETMET	R8	R4	K2
      0x5C280E00,  //  000B  MOVE	R10	R7
      0x582C0003,  //  000C  LDCONST	R11	K3
      0x7C200600,  //  000D  CALL	R8	3
      0x00140A08,  //  000E  ADD	R5	R5	R8
      0x7001FFF7,  //  000F  JMP		#0008
      0x58180004,  //  0010  LDCONST	R6	K4
      0xAC180200,  //  0011  CATCH	R6	1	0
      0xB0080000,  //  0012  RAISE	2	R0	R0
      0x58180005,  //  0013  LDCONST	R6	K5
      0x601C000C,  //  0014  GETGBL	R7	G12
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x7C1C0200,  //  0016  CALL	R7	1
      0x141C0C07,  //  0017  LT	R7	R6	R7
      0x781E001E,  //  0018  JMPF	R7	#0038
      0x941C0A06,  //  0019  GETIDX	R7	R5	R6
      0x1C200F06,  //  001A  EQ	R8	R7	K6
      0x78220003,  //  001B  JMPF	R8	#0020
      0x8C200707,  //  001C  GETMET	R8	R3	K7
      0x58280008,  //  001D  LDCONST	R10	K8
      0x7C200400,  //  001E  CALL	R8	2
      0x70020015,  //  001F  JMP		#0036
      0x1C200F09,  //  0020  EQ	R8	R7	K9
      0x78220003,  //  0021  JMPF	R8	#0026
      0x8C200707,  //  0022  GETMET	R8	R3	K7
      0x5828000A,  //  0023  LDCONST	R10	K10
      0x7C200400,  //  0024  CALL	R8	2
      0x7002000F,  //  0025  JMP		#0036
      0x8820010B,  //  0026  GETMBR	R8	R0	K11
      0x8C20110C,  //  0027  GETMET	R8	R8	K12
      0x5C280E00,  //  0028  MOVE	R10	R7
      0x7C200400,  //  0029  CALL	R8	2
      0x8C240707,  //  002A  GETMET	R9	R3	K7
      0x602C0018,  //  002B  GETGBL	R11	G24
      0x5830000D,  //  002C  LDCONST	R12	K13
      0x5C340E00,  //  002D  MOVE	R13	R7
      0x1C380E01,  //  002E  EQ	R14	R7	R1
      0x783A0001,  //  002F  JMPF	R14	#0032
      0x5838000E,  //  0030  LDCONST	R14	K14
      0x70020000,  //  0031  JMP		#0033
      0x58380006,  //  0032  LDCONST	R14	K6
      0x5C3C1000,  //  0033  MOVE	R15	R8
      0x7C2C0800,  //  0034  CALL	R11	4
      0x7C240400,  //  0035  CALL	R9	2
      0x00180D0F,  //  0036  ADD	R6	R6	K15
      0x7001FFDB,  //  0037  JMP		#0014
      0x80000000,  //  0038  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_add_config_button
********************************************************************/
be_local_closure(class_Matter_UI_web_add_config_button,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(content_send),
    /* K2   */  be_nested_str_weak(_X3Cp_X3E_X3Cform_X20id_X3Dac_X20action_X3D_X27matterc_X27_X20style_X3D_X27display_X3A_X20block_X3B_X27_X20method_X3D_X27get_X27_X3E_X3Cbutton_X3E),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(_LOGO),
    /* K5   */  be_nested_str_weak(_X20Configure_X20Matter_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(web_add_config_button),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080301,  //  0004  GETMET	R2	R1	K1
      0xB8120600,  //  0005  GETNGBL	R4	K3
      0x88100904,  //  0006  GETMBR	R4	R4	K4
      0x7C080400,  //  0007  CALL	R2	2
      0x8C080301,  //  0008  GETMET	R2	R1	K1
      0x58100005,  //  0009  LDCONST	R4	K5
      0x7C080400,  //  000A  CALL	R2	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_UI_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(add_driver),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C080400,  //  0004  CALL	R2	2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_bridge_status
********************************************************************/
be_local_closure(class_Matter_UI_show_bridge_status,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(plugins),
    /* K2   */  be_nested_str_weak(webserver),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(BRIDGE),
    /* K5   */  be_nested_str_weak(http_remote),
    /* K6   */  be_nested_str_weak(addr),
    /* K7   */  be_nested_str_weak(contains),
    /* K8   */  be_nested_str_weak(push),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(content_send),
    /* K11  */  be_nested_str_weak(_X3Chr_X3E),
    /* K12  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(_STYLESHEET),
    /* K15  */  be_nested_str_weak(k2l),
    /* K16  */  be_nested_str_weak(html_escape),
    /* K17  */  be_nested_str_weak(get_plugin_remote_info),
    /* K18  */  be_nested_str_weak(find),
    /* K19  */  be_nested_str_weak(name),
    /* K20  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27ztdm_X20htrm_X27_X3E_X3Ctd_X3E_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20title_X3D_X27http_X3A_X2F_X2F_X25s_X2F_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X27_X3E_X25s_X3C_X2Fa_X3E_X3C_X2Ftd_X3E),
    /* K21  */  be_nested_str_weak(web_last_seen),
    /* K22  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27htrm_X27_X3E_X3Ctd_X20colspan_X3D_X272_X27_X3E),
    /* K23  */  be_nested_str_weak(web_values),
    /* K24  */  be_nested_str_weak(_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K25  */  be_nested_str_weak(stop_iteration),
    /* K26  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Chr_X3E),
    }),
    be_str_weak(show_bridge_status),
    &be_const_str_solidified,
    ( &(const binstruction[118]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C040202,  //  0003  EQ	R1	R1	R2
      0x78060000,  //  0004  JMPF	R1	#0006
      0x80000200,  //  0005  RET	0
      0xA4060400,  //  0006  IMPORT	R1	K2
      0x4C080000,  //  0007  LDNIL	R2
      0x580C0003,  //  0008  LDCONST	R3	K3
      0x6010000C,  //  0009  GETGBL	R4	G12
      0x88140100,  //  000A  GETMBR	R5	R0	K0
      0x88140B01,  //  000B  GETMBR	R5	R5	K1
      0x7C100200,  //  000C  CALL	R4	1
      0x14100604,  //  000D  LT	R4	R3	R4
      0x78120019,  //  000E  JMPF	R4	#0029
      0x88100100,  //  000F  GETMBR	R4	R0	K0
      0x88100901,  //  0010  GETMBR	R4	R4	K1
      0x94100803,  //  0011  GETIDX	R4	R4	R3
      0x88140904,  //  0012  GETMBR	R5	R4	K4
      0x78160012,  //  0013  JMPF	R5	#0027
      0x4C140000,  //  0014  LDNIL	R5
      0x1C140405,  //  0015  EQ	R5	R2	R5
      0x78160002,  //  0016  JMPF	R5	#001A
      0x60140013,  //  0017  GETGBL	R5	G19
      0x7C140000,  //  0018  CALL	R5	0
      0x5C080A00,  //  0019  MOVE	R2	R5
      0x88140905,  //  001A  GETMBR	R5	R4	K5
      0x88140B06,  //  001B  GETMBR	R5	R5	K6
      0x8C180507,  //  001C  GETMET	R6	R2	K7
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x741A0002,  //  001F  JMPT	R6	#0023
      0x60180012,  //  0020  GETGBL	R6	G18
      0x7C180000,  //  0021  CALL	R6	0
      0x98080A06,  //  0022  SETIDX	R2	R5	R6
      0x94180405,  //  0023  GETIDX	R6	R2	R5
      0x8C180D08,  //  0024  GETMET	R6	R6	K8
      0x5C200800,  //  0025  MOVE	R8	R4
      0x7C180400,  //  0026  CALL	R6	2
      0x000C0709,  //  0027  ADD	R3	R3	K9
      0x7001FFDF,  //  0028  JMP		#0009
      0x4C100000,  //  0029  LDNIL	R4
      0x1C100404,  //  002A  EQ	R4	R2	R4
      0x78120000,  //  002B  JMPF	R4	#002D
      0x80000800,  //  002C  RET	0
      0x8C10030A,  //  002D  GETMET	R4	R1	K10
      0x5818000B,  //  002E  LDCONST	R6	K11
      0x7C100400,  //  002F  CALL	R4	2
      0x8C10030A,  //  0030  GETMET	R4	R1	K10
      0x5818000C,  //  0031  LDCONST	R6	K12
      0x7C100400,  //  0032  CALL	R4	2
      0x8C10030A,  //  0033  GETMET	R4	R1	K10
      0xB81A1A00,  //  0034  GETNGBL	R6	K13
      0x88180D0E,  //  0035  GETMBR	R6	R6	K14
      0x7C100400,  //  0036  CALL	R4	2
      0x60100010,  //  0037  GETGBL	R4	G16
      0x88140100,  //  0038  GETMBR	R5	R0	K0
      0x8C140B0F,  //  0039  GETMET	R5	R5	K15
      0x5C1C0400,  //  003A  MOVE	R7	R2
      0x7C140400,  //  003B  CALL	R5	2
      0x7C100200,  //  003C  CALL	R4	1
      0xA8020030,  //  003D  EXBLK	0	#006F
      0x5C140800,  //  003E  MOVE	R5	R4
      0x7C140000,  //  003F  CALL	R5	0
      0x8C180310,  //  0040  GETMET	R6	R1	K16
      0x5C200A00,  //  0041  MOVE	R8	R5
      0x7C180400,  //  0042  CALL	R6	2
      0x8C1C0310,  //  0043  GETMET	R7	R1	K16
      0x88240100,  //  0044  GETMBR	R9	R0	K0
      0x8C241311,  //  0045  GETMET	R9	R9	K17
      0x5C2C0A00,  //  0046  MOVE	R11	R5
      0x7C240400,  //  0047  CALL	R9	2
      0x8C241312,  //  0048  GETMET	R9	R9	K18
      0x582C0013,  //  0049  LDCONST	R11	K19
      0x5C300A00,  //  004A  MOVE	R12	R5
      0x7C240600,  //  004B  CALL	R9	3
      0x7C1C0400,  //  004C  CALL	R7	2
      0x8C20030A,  //  004D  GETMET	R8	R1	K10
      0x60280018,  //  004E  GETGBL	R10	G24
      0x582C0014,  //  004F  LDCONST	R11	K20
      0x5C300C00,  //  0050  MOVE	R12	R6
      0x5C340C00,  //  0051  MOVE	R13	R6
      0x5C380E00,  //  0052  MOVE	R14	R7
      0x7C280800,  //  0053  CALL	R10	4
      0x7C200400,  //  0054  CALL	R8	2
      0x94200405,  //  0055  GETIDX	R8	R2	R5
      0x94201103,  //  0056  GETIDX	R8	R8	K3
      0x88201105,  //  0057  GETMBR	R8	R8	K5
      0x8C24030A,  //  0058  GETMET	R9	R1	K10
      0x8C2C1115,  //  0059  GETMET	R11	R8	K21
      0x7C2C0200,  //  005A  CALL	R11	1
      0x7C240400,  //  005B  CALL	R9	2
      0x60240010,  //  005C  GETGBL	R9	G16
      0x94280405,  //  005D  GETIDX	R10	R2	R5
      0x7C240200,  //  005E  CALL	R9	1
      0xA802000A,  //  005F  EXBLK	0	#006B
      0x5C281200,  //  0060  MOVE	R10	R9
      0x7C280000,  //  0061  CALL	R10	0
      0x8C2C030A,  //  0062  GETMET	R11	R1	K10
      0x58340016,  //  0063  LDCONST	R13	K22
      0x7C2C0400,  //  0064  CALL	R11	2
      0x8C2C1517,  //  0065  GETMET	R11	R10	K23
      0x7C2C0200,  //  0066  CALL	R11	1
      0x8C2C030A,  //  0067  GETMET	R11	R1	K10
      0x58340018,  //  0068  LDCONST	R13	K24
      0x7C2C0400,  //  0069  CALL	R11	2
      0x7001FFF4,  //  006A  JMP		#0060
      0x58240019,  //  006B  LDCONST	R9	K25
      0xAC240200,  //  006C  CATCH	R9	1	0
      0xB0080000,  //  006D  RAISE	2	R0	R0
      0x7001FFCE,  //  006E  JMP		#003E
      0x58100019,  //  006F  LDCONST	R4	K25
      0xAC100200,  //  0070  CATCH	R4	1	0
      0xB0080000,  //  0071  RAISE	2	R0	R0
      0x8C10030A,  //  0072  GETMET	R4	R1	K10
      0x5818001A,  //  0073  LDCONST	R6	K26
      0x7C100400,  //  0074  CALL	R4	2
      0x80000000,  //  0075  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_add_handler
********************************************************************/
be_local_closure(class_Matter_UI_web_add_handler,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 4]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(page_part_mgr),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(page_part_ctl),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(page_part_mgr_adv),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(page_part_mgr_add),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(on),
    /* K2   */  be_nested_str_weak(_X2Fmatterc),
    /* K3   */  be_nested_str_weak(HTTP_GET),
    /* K4   */  be_nested_str_weak(HTTP_POST),
    /* K5   */  be_nested_str_weak(_X2Fmattera),
    /* K6   */  be_nested_str_weak(_X2Fmatteradd),
    }),
    be_str_weak(web_add_handler),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x88180303,  //  0004  GETMBR	R6	R1	K3
      0x7C080800,  //  0005  CALL	R2	4
      0x8C080301,  //  0006  GETMET	R2	R1	K1
      0x58100002,  //  0007  LDCONST	R4	K2
      0x84140001,  //  0008  CLOSURE	R5	P1
      0x88180304,  //  0009  GETMBR	R6	R1	K4
      0x7C080800,  //  000A  CALL	R2	4
      0x8C080301,  //  000B  GETMET	R2	R1	K1
      0x58100005,  //  000C  LDCONST	R4	K5
      0x84140002,  //  000D  CLOSURE	R5	P2
      0x88180303,  //  000E  GETMBR	R6	R1	K3
      0x7C080800,  //  000F  CALL	R2	4
      0x8C080301,  //  0010  GETMET	R2	R1	K1
      0x58100006,  //  0011  LDCONST	R4	K6
      0x84140003,  //  0012  CLOSURE	R5	P3
      0x88180303,  //  0013  GETMBR	R6	R1	K3
      0x7C080800,  //  0014  CALL	R2	4
      0xA0000000,  //  0015  CLOSE	R0
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_passcode_form
********************************************************************/
be_local_closure(class_Matter_UI_show_passcode_form,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(content_send),
    /* K2   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Advanced_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20cause_X20a_X20restart_X2E_X22_X29_X3B_X27_X3E_X3Cp_X3EPasscode_X3A_X3C_X2Fp_X3E),
    /* K3   */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X271_X27_X20max_X3D_X2799999998_X27_X20name_X3D_X27passcode_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(root_passcode),
    /* K6   */  be_nested_str_weak(_X3Cp_X3EDistinguish_X20id_X3A_X3C_X2Fp_X3E),
    /* K7   */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X270_X27_X20max_X3D_X274095_X27_X20name_X3D_X27discriminator_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K8   */  be_nested_str_weak(root_discriminator),
    /* K9   */  be_nested_str_weak(ipv4only),
    /* K10  */  be_nested_str_weak(_X20checked),
    /* K11  */  be_nested_str_weak(),
    /* K12  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20type_X3D_X27checkbox_X27_X20name_X3D_X27ipv4_X27_X25s_X3EIPv4_X20only_X3C_X2Fp_X3E),
    /* K13  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27passcode_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_passcode_form),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080301,  //  0004  GETMET	R2	R1	K1
      0x60100018,  //  0005  GETGBL	R4	G24
      0x58140003,  //  0006  LDCONST	R5	K3
      0x88180104,  //  0007  GETMBR	R6	R0	K4
      0x88180D05,  //  0008  GETMBR	R6	R6	K5
      0x7C100400,  //  0009  CALL	R4	2
      0x7C080400,  //  000A  CALL	R2	2
      0x8C080301,  //  000B  GETMET	R2	R1	K1
      0x58100006,  //  000C  LDCONST	R4	K6
      0x7C080400,  //  000D  CALL	R2	2
      0x8C080301,  //  000E  GETMET	R2	R1	K1
      0x60100018,  //  000F  GETGBL	R4	G24
      0x58140007,  //  0010  LDCONST	R5	K7
      0x88180104,  //  0011  GETMBR	R6	R0	K4
      0x88180D08,  //  0012  GETMBR	R6	R6	K8
      0x7C100400,  //  0013  CALL	R4	2
      0x7C080400,  //  0014  CALL	R2	2
      0x88080104,  //  0015  GETMBR	R2	R0	K4
      0x88080509,  //  0016  GETMBR	R2	R2	K9
      0x780A0001,  //  0017  JMPF	R2	#001A
      0x5808000A,  //  0018  LDCONST	R2	K10
      0x70020000,  //  0019  JMP		#001B
      0x5808000B,  //  001A  LDCONST	R2	K11
      0x8C0C0301,  //  001B  GETMET	R3	R1	K1
      0x60140018,  //  001C  GETGBL	R5	G24
      0x5818000C,  //  001D  LDCONST	R6	K12
      0x5C1C0400,  //  001E  MOVE	R7	R2
      0x7C140400,  //  001F  CALL	R5	2
      0x7C0C0400,  //  0020  CALL	R3	2
      0x8C0C0301,  //  0021  GETMET	R3	R1	K1
      0x5814000D,  //  0022  LDCONST	R5	K13
      0x7C0C0400,  //  0023  CALL	R3	2
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_ctl
********************************************************************/
be_local_closure(class_Matter_UI_page_part_ctl,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[103]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(check_privileged_access),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(partition_core),
    /* K4   */  be_nested_str_weak(persist),
    /* K5   */  be_nested_str_weak(has_arg),
    /* K6   */  be_nested_str_weak(passcode),
    /* K7   */  be_nested_str_weak(discriminator),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20_X2Fmatterc_X20received_X20_X27_X25s_X27_X20command),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(device),
    /* K12  */  be_nested_str_weak(root_passcode),
    /* K13  */  be_nested_str_weak(arg),
    /* K14  */  be_nested_str_weak(root_discriminator),
    /* K15  */  be_nested_str_weak(ipv4only),
    /* K16  */  be_nested_str_weak(ipv4),
    /* K17  */  be_nested_str_weak(on),
    /* K18  */  be_nested_str_weak(save_param),
    /* K19  */  be_nested_str_weak(redirect),
    /* K20  */  be_nested_str_weak(_X2F_X3Frst_X3D),
    /* K21  */  be_nested_str_weak(save),
    /* K22  */  be_nested_str_weak(menable),
    /* K23  */  be_nested_str_weak(comm),
    /* K24  */  be_nested_str_weak(nobridge),
    /* K25  */  be_nested_str_weak(disable_bridge_mode),
    /* K26  */  be_nested_str_weak(matter_enabled),
    /* K27  */  be_nested_str_weak(enable),
    /* K28  */  be_nested_str_weak(tasmota),
    /* K29  */  be_nested_str_weak(cmd),
    /* K30  */  be_nested_str_weak(SetOption),
    /* K31  */  be_nested_str_weak(matter),
    /* K32  */  be_nested_str_weak(MATTER_OPTION),
    /* K33  */  be_nested_str_weak(_X201),
    /* K34  */  be_nested_str_weak(disable),
    /* K35  */  be_nested_str_weak(_X200),
    /* K36  */  be_nested_str_weak(commissioning),
    /* K37  */  be_nested_str_weak(commissioning_open),
    /* K38  */  be_nested_str_weak(start_root_basic_commissioning),
    /* K39  */  be_nested_str_weak(stop_basic_commissioning),
    /* K40  */  be_nested_str_weak(_X2F),
    /* K41  */  be_nested_str_weak(del_fabric),
    /* K42  */  be_const_int(0),
    /* K43  */  be_nested_str_weak(sessions),
    /* K44  */  be_nested_str_weak(fabrics),
    /* K45  */  be_nested_str_weak(get_fabric_index),
    /* K46  */  be_nested_str_weak(remove_fabric),
    /* K47  */  be_const_int(1),
    /* K48  */  be_nested_str_weak(_X2Fmattera_X3F),
    /* K49  */  be_nested_str_weak(auto),
    /* K50  */  be_nested_str_weak(reset_param),
    /* K51  */  be_nested_str_weak(config),
    /* K52  */  be_nested_str_weak(arg_size),
    /* K53  */  be_nested_str_weak(arg_name),
    /* K54  */  be_nested_str_weak(find),
    /* K55  */  be_const_int(2147483647),
    /* K56  */  be_nested_str_weak(plugins_config),
    /* K57  */  be_nested_str_weak(plugins_classes),
    /* K58  */  be_nested_str_weak(type),
    /* K59  */  be_nested_str_weak(),
    /* K60  */  be_nested_str_weak(MTR_X3A_X20ep_X3D_X25i_X20arg_X3D_X25s),
    /* K61  */  be_nested_str_weak(ui_conf_to_string),
    /* K62  */  be_nested_str_weak(MTR_X3A_X20ep_X3D_X25i_X20prev_arg_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27_X20_X25s),
    /* K63  */  be_nested_str_weak(changed),
    /* K64  */  be_nested_str_weak(ui_string_to_conf),
    /* K65  */  be_nested_str_weak(find_plugin_by_endpoint),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20apply_X20conf_X20_X27_X25s_X27_X20_X28_X25i_X29_X20to_X20_X25s),
    /* K67  */  be_nested_str_weak(parse_configuration),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20ep_X3D_X25i_X20not_X20found),
    /* K69  */  be_nested_str_weak(nam),
    /* K70  */  be_nested_str_weak(name),
    /* K71  */  be_nested_str_weak(set_name),
    /* K72  */  be_nested_str_weak(remove),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20apply_X20name_X20_X27_X25s_X27_X20_X28_X25i_X29_X20to_X20_X25s),
    /* K74  */  be_nested_str_weak(stop_iteration),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20config_X20_X3D_X20_X25s),
    /* K76  */  be_nested_str_weak(MTR_X3A_X20config_X20error_X20_X3D_X20_X25s),
    /* K77  */  be_nested_str_weak(plugins_persist),
    /* K78  */  be_nested_str_weak(_X2Fmatterc_X3F),
    /* K79  */  be_nested_str_weak(addep),
    /* K80  */  be_nested_str_weak(pi),
    /* K81  */  be_nested_str_weak(MTR_X3A_X20add_X20endpoint_X20typ_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K82  */  be_nested_str_weak(bridge_add_endpoint),
    /* K83  */  be_nested_str_weak(addrem),
    /* K84  */  be_nested_str_weak(url),
    /* K85  */  be_nested_str_weak(value_error),
    /* K86  */  be_nested_str_weak(url_X20shouldn_X27t_X20be_X20null),
    /* K87  */  be_nested_str_weak(equal_map),
    /* K88  */  be_nested_str_weak(MTR_X3A_X20remote_X20add_X20url_X3D_X27_X25s_X27_X20type_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K89  */  be_nested_str_weak(del),
    /* K90  */  be_nested_str_weak(bridge_remove_endpoint),
    /* K91  */  be_nested_str_weak(content_start),
    /* K92  */  be_nested_str_weak(Parameter_X20error),
    /* K93  */  be_nested_str_weak(content_send_style),
    /* K94  */  be_nested_str_weak(content_send),
    /* K95  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EError_X3A_X3C_X2Fb_X3E_X25s_X3C_X2Fp_X3E),
    /* K96  */  be_nested_str_weak(html_escape),
    /* K97  */  be_nested_str_weak(content_button),
    /* K98  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K99  */  be_nested_str_weak(content_stop),
    /* K100 */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K101 */  be_const_int(2),
    /* K102 */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
    }),
    be_str_weak(page_part_ctl),
    &be_const_str_solidified,
    ( &(const binstruction[658]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0001,  //  0003  JMPT	R2	#0006
      0x4C080000,  //  0004  LDNIL	R2
      0x80040400,  //  0005  RET	1	R2
      0xA40A0400,  //  0006  IMPORT	R2	K2
      0xA40E0600,  //  0007  IMPORT	R3	K3
      0xA4120800,  //  0008  IMPORT	R4	K4
      0x4C140000,  //  0009  LDNIL	R5
      0xA8020268,  //  000A  EXBLK	0	#0274
      0x8C180305,  //  000B  GETMET	R6	R1	K5
      0x58200006,  //  000C  LDCONST	R8	K6
      0x7C180400,  //  000D  CALL	R6	2
      0x741A0003,  //  000E  JMPT	R6	#0013
      0x8C180305,  //  000F  GETMET	R6	R1	K5
      0x58200007,  //  0010  LDCONST	R8	K7
      0x7C180400,  //  0011  CALL	R6	2
      0x781A0029,  //  0012  JMPF	R6	#003D
      0xB81A1000,  //  0013  GETNGBL	R6	K8
      0x601C0018,  //  0014  GETGBL	R7	G24
      0x58200009,  //  0015  LDCONST	R8	K9
      0x58240006,  //  0016  LDCONST	R9	K6
      0x7C1C0400,  //  0017  CALL	R7	2
      0x5820000A,  //  0018  LDCONST	R8	K10
      0x7C180400,  //  0019  CALL	R6	2
      0x8C180305,  //  001A  GETMET	R6	R1	K5
      0x58200006,  //  001B  LDCONST	R8	K6
      0x7C180400,  //  001C  CALL	R6	2
      0x781A0006,  //  001D  JMPF	R6	#0025
      0x8818010B,  //  001E  GETMBR	R6	R0	K11
      0x601C0009,  //  001F  GETGBL	R7	G9
      0x8C20030D,  //  0020  GETMET	R8	R1	K13
      0x58280006,  //  0021  LDCONST	R10	K6
      0x7C200400,  //  0022  CALL	R8	2
      0x7C1C0200,  //  0023  CALL	R7	1
      0x901A1807,  //  0024  SETMBR	R6	K12	R7
      0x8C180305,  //  0025  GETMET	R6	R1	K5
      0x58200007,  //  0026  LDCONST	R8	K7
      0x7C180400,  //  0027  CALL	R6	2
      0x781A0006,  //  0028  JMPF	R6	#0030
      0x8818010B,  //  0029  GETMBR	R6	R0	K11
      0x601C0009,  //  002A  GETGBL	R7	G9
      0x8C20030D,  //  002B  GETMET	R8	R1	K13
      0x58280007,  //  002C  LDCONST	R10	K7
      0x7C200400,  //  002D  CALL	R8	2
      0x7C1C0200,  //  002E  CALL	R7	1
      0x901A1C07,  //  002F  SETMBR	R6	K14	R7
      0x8818010B,  //  0030  GETMBR	R6	R0	K11
      0x8C1C030D,  //  0031  GETMET	R7	R1	K13
      0x58240010,  //  0032  LDCONST	R9	K16
      0x7C1C0400,  //  0033  CALL	R7	2
      0x1C1C0F11,  //  0034  EQ	R7	R7	K17
      0x901A1E07,  //  0035  SETMBR	R6	K15	R7
      0x8818010B,  //  0036  GETMBR	R6	R0	K11
      0x8C180D12,  //  0037  GETMET	R6	R6	K18
      0x7C180200,  //  0038  CALL	R6	1
      0x8C180313,  //  0039  GETMET	R6	R1	K19
      0x58200014,  //  003A  LDCONST	R8	K20
      0x7C180400,  //  003B  CALL	R6	2
      0x70020221,  //  003C  JMP		#025F
      0x8C180305,  //  003D  GETMET	R6	R1	K5
      0x58200015,  //  003E  LDCONST	R8	K21
      0x7C180400,  //  003F  CALL	R6	2
      0x781A0054,  //  0040  JMPF	R6	#0096
      0x8C180305,  //  0041  GETMET	R6	R1	K5
      0x58200016,  //  0042  LDCONST	R8	K22
      0x7C180400,  //  0043  CALL	R6	2
      0x8C1C0305,  //  0044  GETMET	R7	R1	K5
      0x58240017,  //  0045  LDCONST	R9	K23
      0x7C1C0400,  //  0046  CALL	R7	2
      0x8C20030D,  //  0047  GETMET	R8	R1	K13
      0x58280018,  //  0048  LDCONST	R10	K24
      0x7C200400,  //  0049  CALL	R8	2
      0x1C201111,  //  004A  EQ	R8	R8	K17
      0x8824010B,  //  004B  GETMBR	R9	R0	K11
      0x88241319,  //  004C  GETMBR	R9	R9	K25
      0x20241208,  //  004D  NE	R9	R9	R8
      0x78260004,  //  004E  JMPF	R9	#0054
      0x8824010B,  //  004F  GETMBR	R9	R0	K11
      0x90263208,  //  0050  SETMBR	R9	K25	R8
      0x8824010B,  //  0051  GETMBR	R9	R0	K11
      0x8C241312,  //  0052  GETMET	R9	R9	K18
      0x7C240200,  //  0053  CALL	R9	1
      0x8C24011A,  //  0054  GETMET	R9	R0	K26
      0x7C240200,  //  0055  CALL	R9	1
      0x20240C09,  //  0056  NE	R9	R6	R9
      0x78260025,  //  0057  JMPF	R9	#007E
      0x781A0010,  //  0058  JMPF	R6	#006A
      0xB8261000,  //  0059  GETNGBL	R9	K8
      0x60280018,  //  005A  GETGBL	R10	G24
      0x582C0009,  //  005B  LDCONST	R11	K9
      0x5830001B,  //  005C  LDCONST	R12	K27
      0x7C280400,  //  005D  CALL	R10	2
      0x582C000A,  //  005E  LDCONST	R11	K10
      0x7C240400,  //  005F  CALL	R9	2
      0xB8263800,  //  0060  GETNGBL	R9	K28
      0x8C24131D,  //  0061  GETMET	R9	R9	K29
      0x602C0008,  //  0062  GETGBL	R11	G8
      0xB8323E00,  //  0063  GETNGBL	R12	K31
      0x88301920,  //  0064  GETMBR	R12	R12	K32
      0x7C2C0200,  //  0065  CALL	R11	1
      0x002E3C0B,  //  0066  ADD	R11	K30	R11
      0x002C1721,  //  0067  ADD	R11	R11	K33
      0x7C240400,  //  0068  CALL	R9	2
      0x7002000F,  //  0069  JMP		#007A
      0xB8261000,  //  006A  GETNGBL	R9	K8
      0x60280018,  //  006B  GETGBL	R10	G24
      0x582C0009,  //  006C  LDCONST	R11	K9
      0x58300022,  //  006D  LDCONST	R12	K34
      0x7C280400,  //  006E  CALL	R10	2
      0x582C000A,  //  006F  LDCONST	R11	K10
      0x7C240400,  //  0070  CALL	R9	2
      0xB8263800,  //  0071  GETNGBL	R9	K28
      0x8C24131D,  //  0072  GETMET	R9	R9	K29
      0x602C0008,  //  0073  GETGBL	R11	G8
      0xB8323E00,  //  0074  GETNGBL	R12	K31
      0x88301920,  //  0075  GETMBR	R12	R12	K32
      0x7C2C0200,  //  0076  CALL	R11	1
      0x002E3C0B,  //  0077  ADD	R11	K30	R11
      0x002C1723,  //  0078  ADD	R11	R11	K35
      0x7C240400,  //  0079  CALL	R9	2
      0x8C240313,  //  007A  GETMET	R9	R1	K19
      0x582C0014,  //  007B  LDCONST	R11	K20
      0x7C240400,  //  007C  CALL	R9	2
      0x70020016,  //  007D  JMP		#0095
      0x8824010B,  //  007E  GETMBR	R9	R0	K11
      0x88241324,  //  007F  GETMBR	R9	R9	K36
      0x88241325,  //  0080  GETMBR	R9	R9	K37
      0x4C280000,  //  0081  LDNIL	R10
      0x2024120A,  //  0082  NE	R9	R9	R10
      0x20240E09,  //  0083  NE	R9	R7	R9
      0x7826000C,  //  0084  JMPF	R9	#0092
      0x781E0003,  //  0085  JMPF	R7	#008A
      0x8824010B,  //  0086  GETMBR	R9	R0	K11
      0x8C241326,  //  0087  GETMET	R9	R9	K38
      0x7C240200,  //  0088  CALL	R9	1
      0x70020003,  //  0089  JMP		#008E
      0x8824010B,  //  008A  GETMBR	R9	R0	K11
      0x88241324,  //  008B  GETMBR	R9	R9	K36
      0x8C241327,  //  008C  GETMET	R9	R9	K39
      0x7C240200,  //  008D  CALL	R9	1
      0x8C240313,  //  008E  GETMET	R9	R1	K19
      0x582C0028,  //  008F  LDCONST	R11	K40
      0x7C240400,  //  0090  CALL	R9	2
      0x70020002,  //  0091  JMP		#0095
      0x8C240313,  //  0092  GETMET	R9	R1	K19
      0x582C0028,  //  0093  LDCONST	R11	K40
      0x7C240400,  //  0094  CALL	R9	2
      0x700201C8,  //  0095  JMP		#025F
      0x8C180305,  //  0096  GETMET	R6	R1	K5
      0x58200029,  //  0097  LDCONST	R8	K41
      0x7C180400,  //  0098  CALL	R6	2
      0x781A0025,  //  0099  JMPF	R6	#00C0
      0xB81A1000,  //  009A  GETNGBL	R6	K8
      0x601C0018,  //  009B  GETGBL	R7	G24
      0x58200009,  //  009C  LDCONST	R8	K9
      0x58240029,  //  009D  LDCONST	R9	K41
      0x7C1C0400,  //  009E  CALL	R7	2
      0x5820000A,  //  009F  LDCONST	R8	K10
      0x7C180400,  //  00A0  CALL	R6	2
      0x60180009,  //  00A1  GETGBL	R6	G9
      0x8C1C030D,  //  00A2  GETMET	R7	R1	K13
      0x58240029,  //  00A3  LDCONST	R9	K41
      0x7C1C0400,  //  00A4  CALL	R7	2
      0x7C180200,  //  00A5  CALL	R6	1
      0x581C002A,  //  00A6  LDCONST	R7	K42
      0x8820010B,  //  00A7  GETMBR	R8	R0	K11
      0x8820112B,  //  00A8  GETMBR	R8	R8	K43
      0x8820112C,  //  00A9  GETMBR	R8	R8	K44
      0x6024000C,  //  00AA  GETGBL	R9	G12
      0x5C281000,  //  00AB  MOVE	R10	R8
      0x7C240200,  //  00AC  CALL	R9	1
      0x14240E09,  //  00AD  LT	R9	R7	R9
      0x7826000C,  //  00AE  JMPF	R9	#00BC
      0x94241007,  //  00AF  GETIDX	R9	R8	R7
      0x8C24132D,  //  00B0  GETMET	R9	R9	K45
      0x7C240200,  //  00B1  CALL	R9	1
      0x1C241206,  //  00B2  EQ	R9	R9	R6
      0x78260005,  //  00B3  JMPF	R9	#00BA
      0x8824010B,  //  00B4  GETMBR	R9	R0	K11
      0x8C24132E,  //  00B5  GETMET	R9	R9	K46
      0x942C1007,  //  00B6  GETIDX	R11	R8	R7
      0x7C240400,  //  00B7  CALL	R9	2
      0x70020002,  //  00B8  JMP		#00BC
      0x70020000,  //  00B9  JMP		#00BB
      0x001C0F2F,  //  00BA  ADD	R7	R7	K47
      0x7001FFED,  //  00BB  JMP		#00AA
      0x8C240313,  //  00BC  GETMET	R9	R1	K19
      0x582C0030,  //  00BD  LDCONST	R11	K48
      0x7C240400,  //  00BE  CALL	R9	2
      0x7002019E,  //  00BF  JMP		#025F
      0x8C180305,  //  00C0  GETMET	R6	R1	K5
      0x58200031,  //  00C1  LDCONST	R8	K49
      0x7C180400,  //  00C2  CALL	R6	2
      0x781A000D,  //  00C3  JMPF	R6	#00D2
      0xB81A1000,  //  00C4  GETNGBL	R6	K8
      0x601C0018,  //  00C5  GETGBL	R7	G24
      0x58200009,  //  00C6  LDCONST	R8	K9
      0x58240031,  //  00C7  LDCONST	R9	K49
      0x7C1C0400,  //  00C8  CALL	R7	2
      0x5820000A,  //  00C9  LDCONST	R8	K10
      0x7C180400,  //  00CA  CALL	R6	2
      0x8818010B,  //  00CB  GETMBR	R6	R0	K11
      0x8C180D32,  //  00CC  GETMET	R6	R6	K50
      0x7C180200,  //  00CD  CALL	R6	1
      0x8C180313,  //  00CE  GETMET	R6	R1	K19
      0x58200014,  //  00CF  LDCONST	R8	K20
      0x7C180400,  //  00D0  CALL	R6	2
      0x7002018C,  //  00D1  JMP		#025F
      0x8C180305,  //  00D2  GETMET	R6	R1	K5
      0x58200033,  //  00D3  LDCONST	R8	K51
      0x7C180400,  //  00D4  CALL	R6	2
      0x781A00CE,  //  00D5  JMPF	R6	#01A5
      0xB81A1000,  //  00D6  GETNGBL	R6	K8
      0x601C0018,  //  00D7  GETGBL	R7	G24
      0x58200009,  //  00D8  LDCONST	R8	K9
      0x58240033,  //  00D9  LDCONST	R9	K51
      0x7C1C0400,  //  00DA  CALL	R7	2
      0x5820000A,  //  00DB  LDCONST	R8	K10
      0x7C180400,  //  00DC  CALL	R6	2
      0x50180000,  //  00DD  LDBOOL	R6	0	0
      0x601C0010,  //  00DE  GETGBL	R7	G16
      0x8C200334,  //  00DF  GETMET	R8	R1	K52
      0x7C200200,  //  00E0  CALL	R8	1
      0x0420112F,  //  00E1  SUB	R8	R8	K47
      0x40225408,  //  00E2  CONNECT	R8	K42	R8
      0x7C1C0200,  //  00E3  CALL	R7	1
      0xA802009B,  //  00E4  EXBLK	0	#0181
      0x5C200E00,  //  00E5  MOVE	R8	R7
      0x7C200000,  //  00E6  CALL	R8	0
      0x8C240335,  //  00E7  GETMET	R9	R1	K53
      0x5C2C1000,  //  00E8  MOVE	R11	R8
      0x7C240400,  //  00E9  CALL	R9	2
      0x8C280536,  //  00EA  GETMET	R10	R2	K54
      0x5C301200,  //  00EB  MOVE	R12	R9
      0x5834000D,  //  00EC  LDCONST	R13	K13
      0x7C280600,  //  00ED  CALL	R10	3
      0x1C28152A,  //  00EE  EQ	R10	R10	K42
      0x782A0057,  //  00EF  JMPF	R10	#0148
      0x60280009,  //  00F0  GETGBL	R10	G9
      0x402E1537,  //  00F1  CONNECT	R11	K10	K55
      0x942C120B,  //  00F2  GETIDX	R11	R9	R11
      0x7C280200,  //  00F3  CALL	R10	1
      0x8C2C030D,  //  00F4  GETMET	R11	R1	K13
      0x5C341000,  //  00F5  MOVE	R13	R8
      0x7C2C0400,  //  00F6  CALL	R11	2
      0x8830010B,  //  00F7  GETMBR	R12	R0	K11
      0x88301938,  //  00F8  GETMBR	R12	R12	K56
      0x8C301936,  //  00F9  GETMET	R12	R12	K54
      0x60380008,  //  00FA  GETGBL	R14	G8
      0x5C3C1400,  //  00FB  MOVE	R15	R10
      0x7C380200,  //  00FC  CALL	R14	1
      0x7C300400,  //  00FD  CALL	R12	2
      0x4C340000,  //  00FE  LDNIL	R13
      0x2034180D,  //  00FF  NE	R13	R12	R13
      0x7836003E,  //  0100  JMPF	R13	#0140
      0x8834010B,  //  0101  GETMBR	R13	R0	K11
      0x88341B39,  //  0102  GETMBR	R13	R13	K57
      0x8C341B36,  //  0103  GETMET	R13	R13	K54
      0x8C3C1936,  //  0104  GETMET	R15	R12	K54
      0x5844003A,  //  0105  LDCONST	R17	K58
      0x5848003B,  //  0106  LDCONST	R18	K59
      0x7C3C0600,  //  0107  CALL	R15	3
      0x7C340400,  //  0108  CALL	R13	2
      0x4C380000,  //  0109  LDNIL	R14
      0x20381A0E,  //  010A  NE	R14	R13	R14
      0x783A0032,  //  010B  JMPF	R14	#013F
      0xB83A1000,  //  010C  GETNGBL	R14	K8
      0x603C0018,  //  010D  GETGBL	R15	G24
      0x5840003C,  //  010E  LDCONST	R16	K60
      0x5C441400,  //  010F  MOVE	R17	R10
      0x5C481600,  //  0110  MOVE	R18	R11
      0x7C3C0600,  //  0111  CALL	R15	3
      0x5840000A,  //  0112  LDCONST	R16	K10
      0x7C380400,  //  0113  CALL	R14	2
      0x8C381B3D,  //  0114  GETMET	R14	R13	K61
      0x5C401A00,  //  0115  MOVE	R16	R13
      0x5C441800,  //  0116  MOVE	R17	R12
      0x7C380600,  //  0117  CALL	R14	3
      0x203C1C0B,  //  0118  NE	R15	R14	R11
      0xB8421000,  //  0119  GETNGBL	R16	K8
      0x60440018,  //  011A  GETGBL	R17	G24
      0x5848003E,  //  011B  LDCONST	R18	K62
      0x5C4C1400,  //  011C  MOVE	R19	R10
      0x5C501C00,  //  011D  MOVE	R20	R14
      0x5C541600,  //  011E  MOVE	R21	R11
      0x20581C0B,  //  011F  NE	R22	R14	R11
      0x785A0001,  //  0120  JMPF	R22	#0123
      0x5858003F,  //  0121  LDCONST	R22	K63
      0x70020000,  //  0122  JMP		#0124
      0x5858003B,  //  0123  LDCONST	R22	K59
      0x7C440A00,  //  0124  CALL	R17	5
      0x5848000A,  //  0125  LDCONST	R18	K10
      0x7C400400,  //  0126  CALL	R16	2
      0x783E0016,  //  0127  JMPF	R15	#013F
      0x50180200,  //  0128  LDBOOL	R6	1	0
      0x8C401B40,  //  0129  GETMET	R16	R13	K64
      0x5C481A00,  //  012A  MOVE	R18	R13
      0x5C4C1800,  //  012B  MOVE	R19	R12
      0x5C501600,  //  012C  MOVE	R20	R11
      0x7C400800,  //  012D  CALL	R16	4
      0x8840010B,  //  012E  GETMBR	R16	R0	K11
      0x8C402141,  //  012F  GETMET	R16	R16	K65
      0x5C481400,  //  0130  MOVE	R18	R10
      0x7C400400,  //  0131  CALL	R16	2
      0x7842000B,  //  0132  JMPF	R16	#013F
      0xB8461000,  //  0133  GETNGBL	R17	K8
      0x60480018,  //  0134  GETGBL	R18	G24
      0x584C0042,  //  0135  LDCONST	R19	K66
      0x5C501800,  //  0136  MOVE	R20	R12
      0x5C541400,  //  0137  MOVE	R21	R10
      0x5C582000,  //  0138  MOVE	R22	R16
      0x7C480800,  //  0139  CALL	R18	4
      0x584C000A,  //  013A  LDCONST	R19	K10
      0x7C440400,  //  013B  CALL	R17	2
      0x8C442143,  //  013C  GETMET	R17	R16	K67
      0x5C4C1800,  //  013D  MOVE	R19	R12
      0x7C440400,  //  013E  CALL	R17	2
      0x70020006,  //  013F  JMP		#0147
      0xB8361000,  //  0140  GETNGBL	R13	K8
      0x60380018,  //  0141  GETGBL	R14	G24
      0x583C0044,  //  0142  LDCONST	R15	K68
      0x5C401400,  //  0143  MOVE	R16	R10
      0x7C380400,  //  0144  CALL	R14	2
      0x583C000A,  //  0145  LDCONST	R15	K10
      0x7C340400,  //  0146  CALL	R13	2
      0x70020037,  //  0147  JMP		#0180
      0x8C280536,  //  0148  GETMET	R10	R2	K54
      0x5C301200,  //  0149  MOVE	R12	R9
      0x58340045,  //  014A  LDCONST	R13	K69
      0x7C280600,  //  014B  CALL	R10	3
      0x1C28152A,  //  014C  EQ	R10	R10	K42
      0x782A0031,  //  014D  JMPF	R10	#0180
      0x60280009,  //  014E  GETGBL	R10	G9
      0x402E1537,  //  014F  CONNECT	R11	K10	K55
      0x942C120B,  //  0150  GETIDX	R11	R9	R11
      0x7C280200,  //  0151  CALL	R10	1
      0x8C2C030D,  //  0152  GETMET	R11	R1	K13
      0x5C341000,  //  0153  MOVE	R13	R8
      0x7C2C0400,  //  0154  CALL	R11	2
      0x8830010B,  //  0155  GETMBR	R12	R0	K11
      0x88301938,  //  0156  GETMBR	R12	R12	K56
      0x8C301936,  //  0157  GETMET	R12	R12	K54
      0x60380008,  //  0158  GETGBL	R14	G8
      0x5C3C1400,  //  0159  MOVE	R15	R10
      0x7C380200,  //  015A  CALL	R14	1
      0x7C300400,  //  015B  CALL	R12	2
      0x4C340000,  //  015C  LDNIL	R13
      0x2034180D,  //  015D  NE	R13	R12	R13
      0x78360020,  //  015E  JMPF	R13	#0180
      0x8C341936,  //  015F  GETMET	R13	R12	K54
      0x583C0046,  //  0160  LDCONST	R15	K70
      0x5840003B,  //  0161  LDCONST	R16	K59
      0x7C340600,  //  0162  CALL	R13	3
      0x20381A0B,  //  0163  NE	R14	R13	R11
      0x783A001A,  //  0164  JMPF	R14	#0180
      0x50180200,  //  0165  LDBOOL	R6	1	0
      0x883C010B,  //  0166  GETMBR	R15	R0	K11
      0x8C3C1F41,  //  0167  GETMET	R15	R15	K65
      0x5C441400,  //  0168  MOVE	R17	R10
      0x7C3C0400,  //  0169  CALL	R15	2
      0x783E0014,  //  016A  JMPF	R15	#0180
      0x8C401F47,  //  016B  GETMET	R16	R15	K71
      0x5C481600,  //  016C  MOVE	R18	R11
      0x7C400400,  //  016D  CALL	R16	2
      0x782E0001,  //  016E  JMPF	R11	#0171
      0x98328C0B,  //  016F  SETIDX	R12	K70	R11
      0x70020002,  //  0170  JMP		#0174
      0x8C401948,  //  0171  GETMET	R16	R12	K72
      0x58480046,  //  0172  LDCONST	R18	K70
      0x7C400400,  //  0173  CALL	R16	2
      0xB8421000,  //  0174  GETNGBL	R16	K8
      0x60440018,  //  0175  GETGBL	R17	G24
      0x58480049,  //  0176  LDCONST	R18	K73
      0x5C4C1800,  //  0177  MOVE	R19	R12
      0x5C501400,  //  0178  MOVE	R20	R10
      0x5C541E00,  //  0179  MOVE	R21	R15
      0x7C440800,  //  017A  CALL	R17	4
      0x5848000A,  //  017B  LDCONST	R18	K10
      0x7C400400,  //  017C  CALL	R16	2
      0x8C401F43,  //  017D  GETMET	R16	R15	K67
      0x5C481800,  //  017E  MOVE	R18	R12
      0x7C400400,  //  017F  CALL	R16	2
      0x7001FF63,  //  0180  JMP		#00E5
      0x581C004A,  //  0181  LDCONST	R7	K74
      0xAC1C0200,  //  0182  CATCH	R7	1	0
      0xB0080000,  //  0183  RAISE	2	R0	R0
      0xB81E1000,  //  0184  GETNGBL	R7	K8
      0x60200018,  //  0185  GETGBL	R8	G24
      0x5824004B,  //  0186  LDCONST	R9	K75
      0x60280008,  //  0187  GETGBL	R10	G8
      0x882C010B,  //  0188  GETMBR	R11	R0	K11
      0x882C1738,  //  0189  GETMBR	R11	R11	K56
      0x7C280200,  //  018A  CALL	R10	1
      0x7C200400,  //  018B  CALL	R8	2
      0x5824000A,  //  018C  LDCONST	R9	K10
      0x7C1C0400,  //  018D  CALL	R7	2
      0x78160007,  //  018E  JMPF	R5	#0197
      0xB81E1000,  //  018F  GETNGBL	R7	K8
      0x60200018,  //  0190  GETGBL	R8	G24
      0x5824004C,  //  0191  LDCONST	R9	K76
      0x5C280A00,  //  0192  MOVE	R10	R5
      0x7C200400,  //  0193  CALL	R8	2
      0x5824000A,  //  0194  LDCONST	R9	K10
      0x7C1C0400,  //  0195  CALL	R7	2
      0x7002000C,  //  0196  JMP		#01A4
      0x741A0002,  //  0197  JMPT	R6	#019B
      0x881C010B,  //  0198  GETMBR	R7	R0	K11
      0x881C0F4D,  //  0199  GETMBR	R7	R7	K77
      0x741E0005,  //  019A  JMPT	R7	#01A1
      0x881C010B,  //  019B  GETMBR	R7	R0	K11
      0x50200200,  //  019C  LDBOOL	R8	1	0
      0x901E9A08,  //  019D  SETMBR	R7	K77	R8
      0x881C010B,  //  019E  GETMBR	R7	R0	K11
      0x8C1C0F12,  //  019F  GETMET	R7	R7	K18
      0x7C1C0200,  //  01A0  CALL	R7	1
      0x8C1C0313,  //  01A1  GETMET	R7	R1	K19
      0x5824004E,  //  01A2  LDCONST	R9	K78
      0x7C1C0400,  //  01A3  CALL	R7	2
      0x700200B9,  //  01A4  JMP		#025F
      0x8C180305,  //  01A5  GETMET	R6	R1	K5
      0x5820004F,  //  01A6  LDCONST	R8	K79
      0x7C180400,  //  01A7  CALL	R6	2
      0x781A002A,  //  01A8  JMPF	R6	#01D4
      0x8C18030D,  //  01A9  GETMET	R6	R1	K13
      0x58200050,  //  01AA  LDCONST	R8	K80
      0x7C180400,  //  01AB  CALL	R6	2
      0x8C1C030D,  //  01AC  GETMET	R7	R1	K13
      0x5824000D,  //  01AD  LDCONST	R9	K13
      0x7C1C0400,  //  01AE  CALL	R7	2
      0x8C20030D,  //  01AF  GETMET	R8	R1	K13
      0x58280045,  //  01B0  LDCONST	R10	K69
      0x7C200400,  //  01B1  CALL	R8	2
      0xB8261000,  //  01B2  GETNGBL	R9	K8
      0x60280018,  //  01B3  GETGBL	R10	G24
      0x582C0051,  //  01B4  LDCONST	R11	K81
      0x5C300C00,  //  01B5  MOVE	R12	R6
      0x5C340E00,  //  01B6  MOVE	R13	R7
      0x7C280600,  //  01B7  CALL	R10	3
      0x582C000A,  //  01B8  LDCONST	R11	K10
      0x7C240400,  //  01B9  CALL	R9	2
      0x8824010B,  //  01BA  GETMBR	R9	R0	K11
      0x88241339,  //  01BB  GETMBR	R9	R9	K57
      0x8C241336,  //  01BC  GETMET	R9	R9	K54
      0x5C2C0C00,  //  01BD  MOVE	R11	R6
      0x7C240400,  //  01BE  CALL	R9	2
      0x4C280000,  //  01BF  LDNIL	R10
      0x2028120A,  //  01C0  NE	R10	R9	R10
      0x782A000D,  //  01C1  JMPF	R10	#01D0
      0x60280013,  //  01C2  GETGBL	R10	G19
      0x7C280000,  //  01C3  CALL	R10	0
      0x78220000,  //  01C4  JMPF	R8	#01C6
      0x982A8C08,  //  01C5  SETIDX	R10	K70	R8
      0x8C2C1340,  //  01C6  GETMET	R11	R9	K64
      0x5C341200,  //  01C7  MOVE	R13	R9
      0x5C381400,  //  01C8  MOVE	R14	R10
      0x5C3C0E00,  //  01C9  MOVE	R15	R7
      0x7C2C0800,  //  01CA  CALL	R11	4
      0x882C010B,  //  01CB  GETMBR	R11	R0	K11
      0x8C2C1752,  //  01CC  GETMET	R11	R11	K82
      0x5C340C00,  //  01CD  MOVE	R13	R6
      0x5C381400,  //  01CE  MOVE	R14	R10
      0x7C2C0600,  //  01CF  CALL	R11	3
      0x8C280313,  //  01D0  GETMET	R10	R1	K19
      0x5830004E,  //  01D1  LDCONST	R12	K78
      0x7C280400,  //  01D2  CALL	R10	2
      0x7002008A,  //  01D3  JMP		#025F
      0x8C180305,  //  01D4  GETMET	R6	R1	K5
      0x58200053,  //  01D5  LDCONST	R8	K83
      0x7C180400,  //  01D6  CALL	R6	2
      0x781A005D,  //  01D7  JMPF	R6	#0236
      0x8C18030D,  //  01D8  GETMET	R6	R1	K13
      0x58200054,  //  01D9  LDCONST	R8	K84
      0x7C180400,  //  01DA  CALL	R6	2
      0x4C1C0000,  //  01DB  LDNIL	R7
      0x1C1C0C07,  //  01DC  EQ	R7	R6	R7
      0x741E0001,  //  01DD  JMPT	R7	#01E0
      0x1C1C0D3B,  //  01DE  EQ	R7	R6	K59
      0x781E0000,  //  01DF  JMPF	R7	#01E1
      0xB006AB56,  //  01E0  RAISE	1	K85	K86
      0x581C002A,  //  01E1  LDCONST	R7	K42
      0x60200008,  //  01E2  GETGBL	R8	G8
      0x5C240E00,  //  01E3  MOVE	R9	R7
      0x7C200200,  //  01E4  CALL	R8	1
      0x8C240305,  //  01E5  GETMET	R9	R1	K5
      0x002EA008,  //  01E6  ADD	R11	K80	R8
      0x7C240400,  //  01E7  CALL	R9	2
      0x78260048,  //  01E8  JMPF	R9	#0232
      0x8C24030D,  //  01E9  GETMET	R9	R1	K13
      0x002EA008,  //  01EA  ADD	R11	K80	R8
      0x7C240400,  //  01EB  CALL	R9	2
      0x8C28030D,  //  01EC  GETMET	R10	R1	K13
      0x00321A08,  //  01ED  ADD	R12	K13	R8
      0x7C280400,  //  01EE  CALL	R10	2
      0x8C2C030D,  //  01EF  GETMET	R11	R1	K13
      0x00368A08,  //  01F0  ADD	R13	K69	R8
      0x7C2C0400,  //  01F1  CALL	R11	2
      0x2030133B,  //  01F2  NE	R12	R9	K59
      0x78320037,  //  01F3  JMPF	R12	#022C
      0x8830010B,  //  01F4  GETMBR	R12	R0	K11
      0x88301939,  //  01F5  GETMBR	R12	R12	K57
      0x8C301936,  //  01F6  GETMET	R12	R12	K54
      0x5C381200,  //  01F7  MOVE	R14	R9
      0x7C300400,  //  01F8  CALL	R12	2
      0x4C340000,  //  01F9  LDNIL	R13
      0x2034180D,  //  01FA  NE	R13	R12	R13
      0x7836002F,  //  01FB  JMPF	R13	#022C
      0x60340013,  //  01FC  GETGBL	R13	G19
      0x7C340000,  //  01FD  CALL	R13	0
      0x9836A806,  //  01FE  SETIDX	R13	K84	R6
      0x98367409,  //  01FF  SETIDX	R13	K58	R9
      0x782E0000,  //  0200  JMPF	R11	#0202
      0x98368C0B,  //  0201  SETIDX	R13	K70	R11
      0x8C381940,  //  0202  GETMET	R14	R12	K64
      0x5C401800,  //  0203  MOVE	R16	R12
      0x5C441A00,  //  0204  MOVE	R17	R13
      0x5C481400,  //  0205  MOVE	R18	R10
      0x7C380800,  //  0206  CALL	R14	4
      0x50380000,  //  0207  LDBOOL	R14	0	0
      0x603C0010,  //  0208  GETGBL	R15	G16
      0x8840010B,  //  0209  GETMBR	R16	R0	K11
      0x88402138,  //  020A  GETMBR	R16	R16	K56
      0x7C3C0200,  //  020B  CALL	R15	1
      0xA802000B,  //  020C  EXBLK	0	#0219
      0x5C401E00,  //  020D  MOVE	R16	R15
      0x7C400000,  //  020E  CALL	R16	0
      0x8C440157,  //  020F  GETMET	R17	R0	K87
      0x5C4C2000,  //  0210  MOVE	R19	R16
      0x5C501A00,  //  0211  MOVE	R20	R13
      0x7C440600,  //  0212  CALL	R17	3
      0x78460001,  //  0213  JMPF	R17	#0216
      0x50380200,  //  0214  LDBOOL	R14	1	0
      0x70020000,  //  0215  JMP		#0217
      0x7001FFF5,  //  0216  JMP		#020D
      0xA8040001,  //  0217  EXBLK	1	1
      0x70020002,  //  0218  JMP		#021C
      0x583C004A,  //  0219  LDCONST	R15	K74
      0xAC3C0200,  //  021A  CATCH	R15	1	0
      0xB0080000,  //  021B  RAISE	2	R0	R0
      0x5C3C1C00,  //  021C  MOVE	R15	R14
      0x743E000D,  //  021D  JMPT	R15	#022C
      0xB83E1000,  //  021E  GETNGBL	R15	K8
      0x60400018,  //  021F  GETGBL	R16	G24
      0x58440058,  //  0220  LDCONST	R17	K88
      0x5C480C00,  //  0221  MOVE	R18	R6
      0x5C4C1200,  //  0222  MOVE	R19	R9
      0x5C501400,  //  0223  MOVE	R20	R10
      0x7C400800,  //  0224  CALL	R16	4
      0x5844000A,  //  0225  LDCONST	R17	K10
      0x7C3C0400,  //  0226  CALL	R15	2
      0x883C010B,  //  0227  GETMBR	R15	R0	K11
      0x8C3C1F52,  //  0228  GETMET	R15	R15	K82
      0x5C441200,  //  0229  MOVE	R17	R9
      0x5C481A00,  //  022A  MOVE	R18	R13
      0x7C3C0600,  //  022B  CALL	R15	3
      0x001C0F2F,  //  022C  ADD	R7	R7	K47
      0x60300008,  //  022D  GETGBL	R12	G8
      0x5C340E00,  //  022E  MOVE	R13	R7
      0x7C300200,  //  022F  CALL	R12	1
      0x5C201800,  //  0230  MOVE	R8	R12
      0x7001FFB2,  //  0231  JMP		#01E5
      0x8C240313,  //  0232  GETMET	R9	R1	K19
      0x582C004E,  //  0233  LDCONST	R11	K78
      0x7C240400,  //  0234  CALL	R9	2
      0x70020028,  //  0235  JMP		#025F
      0x4C180000,  //  0236  LDNIL	R6
      0x601C0010,  //  0237  GETGBL	R7	G16
      0x8C200334,  //  0238  GETMET	R8	R1	K52
      0x7C200200,  //  0239  CALL	R8	1
      0x0420112F,  //  023A  SUB	R8	R8	K47
      0x40225408,  //  023B  CONNECT	R8	K42	R8
      0x7C1C0200,  //  023C  CALL	R7	1
      0xA8020013,  //  023D  EXBLK	0	#0252
      0x5C200E00,  //  023E  MOVE	R8	R7
      0x7C200000,  //  023F  CALL	R8	0
      0x8C240335,  //  0240  GETMET	R9	R1	K53
      0x5C2C1000,  //  0241  MOVE	R11	R8
      0x7C240400,  //  0242  CALL	R9	2
      0x8C280536,  //  0243  GETMET	R10	R2	K54
      0x5C301200,  //  0244  MOVE	R12	R9
      0x58340059,  //  0245  LDCONST	R13	K89
      0x7C280600,  //  0246  CALL	R10	3
      0x1C28152A,  //  0247  EQ	R10	R10	K42
      0x782A0005,  //  0248  JMPF	R10	#024F
      0x60280009,  //  0249  GETGBL	R10	G9
      0x402E1537,  //  024A  CONNECT	R11	K10	K55
      0x942C120B,  //  024B  GETIDX	R11	R9	R11
      0x7C280200,  //  024C  CALL	R10	1
      0x5C181400,  //  024D  MOVE	R6	R10
      0x70020000,  //  024E  JMP		#0250
      0x7001FFED,  //  024F  JMP		#023E
      0xA8040001,  //  0250  EXBLK	1	1
      0x70020002,  //  0251  JMP		#0255
      0x581C004A,  //  0252  LDCONST	R7	K74
      0xAC1C0200,  //  0253  CATCH	R7	1	0
      0xB0080000,  //  0254  RAISE	2	R0	R0
      0x4C1C0000,  //  0255  LDNIL	R7
      0x201C0C07,  //  0256  NE	R7	R6	R7
      0x781E0006,  //  0257  JMPF	R7	#025F
      0x881C010B,  //  0258  GETMBR	R7	R0	K11
      0x8C1C0F5A,  //  0259  GETMET	R7	R7	K90
      0x5C240C00,  //  025A  MOVE	R9	R6
      0x7C1C0400,  //  025B  CALL	R7	2
      0x8C1C0313,  //  025C  GETMET	R7	R1	K19
      0x5824004E,  //  025D  LDCONST	R9	K78
      0x7C1C0400,  //  025E  CALL	R7	2
      0x78160011,  //  025F  JMPF	R5	#0272
      0x8C18035B,  //  0260  GETMET	R6	R1	K91
      0x5820005C,  //  0261  LDCONST	R8	K92
      0x7C180400,  //  0262  CALL	R6	2
      0x8C18035D,  //  0263  GETMET	R6	R1	K93
      0x7C180200,  //  0264  CALL	R6	1
      0x8C18035E,  //  0265  GETMET	R6	R1	K94
      0x60200018,  //  0266  GETGBL	R8	G24
      0x5824005F,  //  0267  LDCONST	R9	K95
      0x8C280360,  //  0268  GETMET	R10	R1	K96
      0x5C300A00,  //  0269  MOVE	R12	R5
      0x7C280400,  //  026A  CALL	R10	2
      0x7C200400,  //  026B  CALL	R8	2
      0x7C180400,  //  026C  CALL	R6	2
      0x8C180361,  //  026D  GETMET	R6	R1	K97
      0x88200362,  //  026E  GETMBR	R8	R1	K98
      0x7C180400,  //  026F  CALL	R6	2
      0x8C180363,  //  0270  GETMET	R6	R1	K99
      0x7C180200,  //  0271  CALL	R6	1
      0xA8040001,  //  0272  EXBLK	1	1
      0x7002001C,  //  0273  JMP		#0291
      0xAC180002,  //  0274  CATCH	R6	0	2
      0x70020019,  //  0275  JMP		#0290
      0xB8221000,  //  0276  GETNGBL	R8	K8
      0x60240018,  //  0277  GETGBL	R9	G24
      0x58280064,  //  0278  LDCONST	R10	K100
      0x5C2C0C00,  //  0279  MOVE	R11	R6
      0x5C300E00,  //  027A  MOVE	R12	R7
      0x7C240600,  //  027B  CALL	R9	3
      0x58280065,  //  027C  LDCONST	R10	K101
      0x7C200400,  //  027D  CALL	R8	2
      0x8C20035B,  //  027E  GETMET	R8	R1	K91
      0x5828005C,  //  027F  LDCONST	R10	K92
      0x7C200400,  //  0280  CALL	R8	2
      0x8C20035D,  //  0281  GETMET	R8	R1	K93
      0x7C200200,  //  0282  CALL	R8	1
      0x8C20035E,  //  0283  GETMET	R8	R1	K94
      0x60280018,  //  0284  GETGBL	R10	G24
      0x582C0066,  //  0285  LDCONST	R11	K102
      0x5C300C00,  //  0286  MOVE	R12	R6
      0x5C340E00,  //  0287  MOVE	R13	R7
      0x7C280600,  //  0288  CALL	R10	3
      0x7C200400,  //  0289  CALL	R8	2
      0x8C200361,  //  028A  GETMET	R8	R1	K97
      0x88280362,  //  028B  GETMBR	R10	R1	K98
      0x7C200400,  //  028C  CALL	R8	2
      0x8C200363,  //  028D  GETMET	R8	R1	K99
      0x7C200200,  //  028E  CALL	R8	1
      0x70020000,  //  028F  JMP		#0291
      0xB0080000,  //  0290  RAISE	2	R0	R0
      0x80000000,  //  0291  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_qrcode
********************************************************************/
be_local_closure(class_Matter_UI_show_qrcode,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(_X20),
    /* K2   */  be_nested_str_weak(_XE2_X96_X84),
    /* K3   */  be_nested_str_weak(_XE2_X96_X80),
    /* K4   */  be_nested_str_weak(_XE2_X96_X88),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(QRCode),
    /* K7   */  be_nested_str_weak(encode_str),
    /* K8   */  be_nested_str_weak(bitmap),
    /* K9   */  be_nested_str_weak(size),
    /* K10  */  be_nested_str_weak(content_send),
    /* K11  */  be_nested_str_weak(_X3Cstyle_X3E_X2Eqr_X7Bfont_X2Dfamily_X3Amonospace_X3B_X20margin_X3A0_X3B_X20padding_X3A0_X3B_X20white_X2Dspace_X3Apre_X3B_X20font_X2Dsize_X3A18px_X3B_X20color_X3A_X23fff_X3B_X20line_X2Dheight_X3A100_X25_X3B_X7D_X3C_X2Fstyle_X3E),
    /* K12  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27transform_X3Ascale_X28_X2E8_X2C1_X29_X3B_X20display_X3Ainline_X2Dblock_X3B_X27_X3E),
    /* K13  */  be_nested_str_weak(_X3Cdiv_X20class_X3D_X27qr_X27_X3E),
    /* K14  */  be_nested_str_weak(),
    /* K15  */  be_const_int(0),
    /* K16  */  be_const_int(1),
    /* K17  */  be_nested_str_weak(stop_iteration),
    /* K18  */  be_nested_str_weak(_X3C_X2Fdiv_X3E),
    /* K19  */  be_const_int(2),
    /* K20  */  be_nested_str_weak(_X3Cdiv_X20class_X3D_X27qr_X27_X20style_X3D_X27background_X2Dcolor_X3A_X23000_X3B_X27_X3E),
    /* K21  */  be_nested_str_weak(_X2F_X3Cdiv_X3E),
    }),
    be_str_weak(show_qrcode),
    &be_const_str_solidified,
    ( &(const binstruction[120]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0xB81E0A00,  //  0005  GETNGBL	R7	K5
      0x881C0F06,  //  0006  GETMBR	R7	R7	K6
      0x8C1C0F07,  //  0007  GETMET	R7	R7	K7
      0x5C240200,  //  0008  MOVE	R9	R1
      0x7C1C0400,  //  0009  CALL	R7	2
      0x94200F08,  //  000A  GETIDX	R8	R7	K8
      0x94240F09,  //  000B  GETIDX	R9	R7	K9
      0x8C28050A,  //  000C  GETMET	R10	R2	K10
      0x5830000B,  //  000D  LDCONST	R12	K11
      0x7C280400,  //  000E  CALL	R10	2
      0x8C28050A,  //  000F  GETMET	R10	R2	K10
      0x5830000C,  //  0010  LDCONST	R12	K12
      0x7C280400,  //  0011  CALL	R10	2
      0x5828000D,  //  0012  LDCONST	R10	K13
      0x8C2C050A,  //  0013  GETMET	R11	R2	K10
      0x5C341400,  //  0014  MOVE	R13	R10
      0x7C2C0400,  //  0015  CALL	R11	2
      0x5828000E,  //  0016  LDCONST	R10	K14
      0x602C0010,  //  0017  GETGBL	R11	G16
      0x00301310,  //  0018  ADD	R12	R9	K16
      0x40321E0C,  //  0019  CONNECT	R12	K15	R12
      0x7C2C0200,  //  001A  CALL	R11	1
      0xA8020003,  //  001B  EXBLK	0	#0020
      0x5C301600,  //  001C  MOVE	R12	R11
      0x7C300000,  //  001D  CALL	R12	0
      0x00281404,  //  001E  ADD	R10	R10	R4
      0x7001FFFB,  //  001F  JMP		#001C
      0x582C0011,  //  0020  LDCONST	R11	K17
      0xAC2C0200,  //  0021  CATCH	R11	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x00281512,  //  0023  ADD	R10	R10	K18
      0x8C2C050A,  //  0024  GETMET	R11	R2	K10
      0x5C341400,  //  0025  MOVE	R13	R10
      0x7C2C0400,  //  0026  CALL	R11	2
      0x602C0010,  //  0027  GETGBL	R11	G16
      0x00301310,  //  0028  ADD	R12	R9	K16
      0x0C301913,  //  0029  DIV	R12	R12	K19
      0x04301910,  //  002A  SUB	R12	R12	K16
      0x40321E0C,  //  002B  CONNECT	R12	K15	R12
      0x7C2C0200,  //  002C  CALL	R11	1
      0xA802002E,  //  002D  EXBLK	0	#005D
      0x5C301600,  //  002E  MOVE	R12	R11
      0x7C300000,  //  002F  CALL	R12	0
      0x00362806,  //  0030  ADD	R13	K20	R6
      0x5C281A00,  //  0031  MOVE	R10	R13
      0x60340010,  //  0032  GETGBL	R13	G16
      0x04381310,  //  0033  SUB	R14	R9	K16
      0x403A1E0E,  //  0034  CONNECT	R14	K15	R14
      0x7C340200,  //  0035  CALL	R13	1
      0xA802001C,  //  0036  EXBLK	0	#0054
      0x5C381A00,  //  0037  MOVE	R14	R13
      0x7C380000,  //  0038  CALL	R14	0
      0x083C1913,  //  0039  MUL	R15	R12	K19
      0x943C100F,  //  003A  GETIDX	R15	R8	R15
      0x943C1E0E,  //  003B  GETIDX	R15	R15	R14
      0x1C3C1F01,  //  003C  EQ	R15	R15	K1
      0x08401913,  //  003D  MUL	R16	R12	K19
      0x00402110,  //  003E  ADD	R16	R16	K16
      0x14402009,  //  003F  LT	R16	R16	R9
      0x78420005,  //  0040  JMPF	R16	#0047
      0x08401913,  //  0041  MUL	R16	R12	K19
      0x00402110,  //  0042  ADD	R16	R16	K16
      0x94401010,  //  0043  GETIDX	R16	R8	R16
      0x9440200E,  //  0044  GETIDX	R16	R16	R14
      0x1C402101,  //  0045  EQ	R16	R16	K1
      0x70020000,  //  0046  JMP		#0048
      0x50400200,  //  0047  LDBOOL	R16	1	0
      0x783E0004,  //  0048  JMPF	R15	#004E
      0x78420001,  //  0049  JMPF	R16	#004C
      0x5C440C00,  //  004A  MOVE	R17	R6
      0x70020000,  //  004B  JMP		#004D
      0x5C440A00,  //  004C  MOVE	R17	R5
      0x70020003,  //  004D  JMP		#0052
      0x78420001,  //  004E  JMPF	R16	#0051
      0x5C440800,  //  004F  MOVE	R17	R4
      0x70020000,  //  0050  JMP		#0052
      0x5C440600,  //  0051  MOVE	R17	R3
      0x00281411,  //  0052  ADD	R10	R10	R17
      0x7001FFE2,  //  0053  JMP		#0037
      0x58340011,  //  0054  LDCONST	R13	K17
      0xAC340200,  //  0055  CATCH	R13	1	0
      0xB0080000,  //  0056  RAISE	2	R0	R0
      0x00281406,  //  0057  ADD	R10	R10	R6
      0x00281512,  //  0058  ADD	R10	R10	K18
      0x8C34050A,  //  0059  GETMET	R13	R2	K10
      0x5C3C1400,  //  005A  MOVE	R15	R10
      0x7C340400,  //  005B  CALL	R13	2
      0x7001FFD0,  //  005C  JMP		#002E
      0x582C0011,  //  005D  LDCONST	R11	K17
      0xAC2C0200,  //  005E  CATCH	R11	1	0
      0xB0080000,  //  005F  RAISE	2	R0	R0
      0x102C1313,  //  0060  MOD	R11	R9	K19
      0x1C2C170F,  //  0061  EQ	R11	R11	K15
      0x782E0010,  //  0062  JMPF	R11	#0074
      0x58280014,  //  0063  LDCONST	R10	K20
      0x602C0010,  //  0064  GETGBL	R11	G16
      0x00301310,  //  0065  ADD	R12	R9	K16
      0x40321E0C,  //  0066  CONNECT	R12	K15	R12
      0x7C2C0200,  //  0067  CALL	R11	1
      0xA8020003,  //  0068  EXBLK	0	#006D
      0x5C301600,  //  0069  MOVE	R12	R11
      0x7C300000,  //  006A  CALL	R12	0
      0x00281405,  //  006B  ADD	R10	R10	R5
      0x7001FFFB,  //  006C  JMP		#0069
      0x582C0011,  //  006D  LDCONST	R11	K17
      0xAC2C0200,  //  006E  CATCH	R11	1	0
      0xB0080000,  //  006F  RAISE	2	R0	R0
      0x00281515,  //  0070  ADD	R10	R10	K21
      0x8C2C050A,  //  0071  GETMET	R11	R2	K10
      0x5C341400,  //  0072  MOVE	R13	R10
      0x7C2C0400,  //  0073  CALL	R11	2
      0x8C2C050A,  //  0074  GETMET	R11	R2	K10
      0x58340012,  //  0075  LDCONST	R13	K18
      0x7C2C0400,  //  0076  CALL	R11	2
      0x80000000,  //  0077  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_enable
********************************************************************/
be_local_closure(class_Matter_UI_show_enable,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(matter_enabled),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X20style_X3D_X27width_X3A320px_X3B_X27_X3ECheck_X20the_X20_X3Ca_X20href_X3D_X27https_X3A_X2F_X2Ftasmota_X2Egithub_X2Eio_X2Fdocs_X2FMatter_X2F_X27_X20target_X3D_X27_blank_X27_X3EMatter_X20documentation_X3C_X2Fa_X3E_X2E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E),
    /* K4   */  be_nested_str_weak(checked),
    /* K5   */  be_nested_str_weak(),
    /* K6   */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20id_X3D_X27menable_X27_X20type_X3D_X27checkbox_X27_X20name_X3D_X27menable_X27_X20_X25s_X3E),
    /* K7   */  be_nested_str_weak(_X3Clabel_X20for_X3D_X27menable_X27_X3E_X3Cb_X3EMatter_X20enable_X3C_X2Fb_X3E_X3C_X2Flabel_X3E_X3C_X2Fp_X3E),
    /* K8   */  be_nested_str_weak(device),
    /* K9   */  be_nested_str_weak(commissioning),
    /* K10  */  be_nested_str_weak(commissioning_open),
    /* K11  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20id_X3D_X27comm_X27_X20type_X3D_X27checkbox_X27_X20name_X3D_X27comm_X27_X20_X25s_X3E),
    /* K12  */  be_nested_str_weak(_X3Clabel_X20for_X3D_X27comm_X27_X3E_X3Cb_X3ECommissioning_X20open_X3C_X2Fb_X3E_X3C_X2Flabel_X3E_X3C_X2Fp_X3E),
    /* K13  */  be_nested_str_weak(disable_bridge_mode),
    /* K14  */  be_nested_str_weak(_X20checked),
    /* K15  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20type_X3D_X27checkbox_X27_X20name_X3D_X27nobridge_X27_X25s_X3E_X3Cb_X3EForce_X20Static_X20endpoints_X3C_X2Fb_X3E_X20_X28non_X2Dbridge_X29_X3C_X2Fp_X3E),
    /* K16  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27save_X27_X20class_X3D_X27button_X20bgrn_X27_X3ESave_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_enable),
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C0C0101,  //  0005  GETMET	R3	R0	K1
      0x7C0C0200,  //  0006  CALL	R3	1
      0x780E0001,  //  0007  JMPF	R3	#000A
      0x580C0004,  //  0008  LDCONST	R3	K4
      0x70020000,  //  0009  JMP		#000B
      0x580C0005,  //  000A  LDCONST	R3	K5
      0x8C100302,  //  000B  GETMET	R4	R1	K2
      0x60180018,  //  000C  GETGBL	R6	G24
      0x581C0006,  //  000D  LDCONST	R7	K6
      0x5C200600,  //  000E  MOVE	R8	R3
      0x7C180400,  //  000F  CALL	R6	2
      0x7C100400,  //  0010  CALL	R4	2
      0x8C100302,  //  0011  GETMET	R4	R1	K2
      0x58180007,  //  0012  LDCONST	R6	K7
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100101,  //  0014  GETMET	R4	R0	K1
      0x7C100200,  //  0015  CALL	R4	1
      0x7812001D,  //  0016  JMPF	R4	#0035
      0x88100108,  //  0017  GETMBR	R4	R0	K8
      0x88100909,  //  0018  GETMBR	R4	R4	K9
      0x8810090A,  //  0019  GETMBR	R4	R4	K10
      0x4C140000,  //  001A  LDNIL	R5
      0x20100805,  //  001B  NE	R4	R4	R5
      0x78120001,  //  001C  JMPF	R4	#001F
      0x58100004,  //  001D  LDCONST	R4	K4
      0x70020000,  //  001E  JMP		#0020
      0x58100005,  //  001F  LDCONST	R4	K5
      0x8C140302,  //  0020  GETMET	R5	R1	K2
      0x601C0018,  //  0021  GETGBL	R7	G24
      0x5820000B,  //  0022  LDCONST	R8	K11
      0x5C240800,  //  0023  MOVE	R9	R4
      0x7C1C0400,  //  0024  CALL	R7	2
      0x7C140400,  //  0025  CALL	R5	2
      0x8C140302,  //  0026  GETMET	R5	R1	K2
      0x581C000C,  //  0027  LDCONST	R7	K12
      0x7C140400,  //  0028  CALL	R5	2
      0x88140108,  //  0029  GETMBR	R5	R0	K8
      0x88140B0D,  //  002A  GETMBR	R5	R5	K13
      0x78160001,  //  002B  JMPF	R5	#002E
      0x5814000E,  //  002C  LDCONST	R5	K14
      0x70020000,  //  002D  JMP		#002F
      0x58140005,  //  002E  LDCONST	R5	K5
      0x8C180302,  //  002F  GETMET	R6	R1	K2
      0x60200018,  //  0030  GETGBL	R8	G24
      0x5824000F,  //  0031  LDCONST	R9	K15
      0x5C280A00,  //  0032  MOVE	R10	R5
      0x7C200400,  //  0033  CALL	R8	2
      0x7C180400,  //  0034  CALL	R6	2
      0x8C100302,  //  0035  GETMET	R4	R1	K2
      0x58180010,  //  0036  LDCONST	R6	K16
      0x7C100400,  //  0037  CALL	R4	2
      0x80000000,  //  0038  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_plugins_hints_js
********************************************************************/
be_local_closure(class_Matter_UI_show_plugins_hints_js,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    3,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(split),
    /* K4   */  be_nested_str_weak(_X7C),
    /* K5   */  be_nested_str_weak(stop_iteration),
    /* K6   */  be_nested_str_weak(),
    /* K7   */  be_nested_str_weak(device),
    /* K8   */  be_nested_str_weak(plugins_classes),
    /* K9   */  be_nested_str_weak(find),
    /* K10  */  be_nested_str_weak(ARG_HINT),
    /* K11  */  be_nested_str_weak(push),
    /* K12  */  be_nested_str_weak(content_send),
    /* K13  */  be_nested_str_weak(_X3Cscript_X20type_X3D_X27text_X2Fjavascript_X27_X3Evar_X20hm_X3D_X25s_X3Bvar_X20hl_X3D_X25s_X3B_X3C_X2Fscript_X3E),
    /* K14  */  be_nested_str_weak(dump),
    /* K15  */  be_nested_str_weak(matter),
    /* K16  */  be_nested_str_weak(_ADD_ENDPOINT_JS),
    }),
    be_str_weak(show_plugins_hints_js),
    &be_const_str_solidified,
    ( &(const binstruction[79]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0x60140012,  //  0003  GETGBL	R5	G18
      0x7C140000,  //  0004  CALL	R5	0
      0x60180010,  //  0005  GETGBL	R6	G16
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C180200,  //  0007  CALL	R6	1
      0xA8020007,  //  0008  EXBLK	0	#0011
      0x5C1C0C00,  //  0009  MOVE	R7	R6
      0x7C1C0000,  //  000A  CALL	R7	0
      0x8C200903,  //  000B  GETMET	R8	R4	K3
      0x5C280E00,  //  000C  MOVE	R10	R7
      0x582C0004,  //  000D  LDCONST	R11	K4
      0x7C200600,  //  000E  CALL	R8	3
      0x00140A08,  //  000F  ADD	R5	R5	R8
      0x7001FFF7,  //  0010  JMP		#0009
      0x58180005,  //  0011  LDCONST	R6	K5
      0xAC180200,  //  0012  CATCH	R6	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x60180013,  //  0014  GETGBL	R6	G19
      0x7C180000,  //  0015  CALL	R6	0
      0x601C0012,  //  0016  GETGBL	R7	G18
      0x7C1C0000,  //  0017  CALL	R7	0
      0x60200010,  //  0018  GETGBL	R8	G16
      0x5C240A00,  //  0019  MOVE	R9	R5
      0x7C200200,  //  001A  CALL	R8	1
      0xA802001F,  //  001B  EXBLK	0	#003C
      0x5C241000,  //  001C  MOVE	R9	R8
      0x7C240000,  //  001D  CALL	R9	0
      0x1C281306,  //  001E  EQ	R10	R9	K6
      0x782A0000,  //  001F  JMPF	R10	#0021
      0x7001FFFA,  //  0020  JMP		#001C
      0x88280107,  //  0021  GETMBR	R10	R0	K7
      0x88281508,  //  0022  GETMBR	R10	R10	K8
      0x8C281509,  //  0023  GETMET	R10	R10	K9
      0x5C301200,  //  0024  MOVE	R12	R9
      0x7C280400,  //  0025  CALL	R10	2
      0x4C2C0000,  //  0026  LDNIL	R11
      0x202C140B,  //  0027  NE	R11	R10	R11
      0x782E0011,  //  0028  JMPF	R11	#003B
      0x882C150A,  //  0029  GETMBR	R11	R10	K10
      0x4C300000,  //  002A  LDNIL	R12
      0x2030160C,  //  002B  NE	R12	R11	R12
      0x7832000D,  //  002C  JMPF	R12	#003B
      0x8C300F09,  //  002D  GETMET	R12	R7	K9
      0x5C381600,  //  002E  MOVE	R14	R11
      0x7C300400,  //  002F  CALL	R12	2
      0x4C340000,  //  0030  LDNIL	R13
      0x1C34180D,  //  0031  EQ	R13	R12	R13
      0x78360006,  //  0032  JMPF	R13	#003A
      0x6034000C,  //  0033  GETGBL	R13	G12
      0x5C380E00,  //  0034  MOVE	R14	R7
      0x7C340200,  //  0035  CALL	R13	1
      0x5C301A00,  //  0036  MOVE	R12	R13
      0x8C340F0B,  //  0037  GETMET	R13	R7	K11
      0x5C3C1600,  //  0038  MOVE	R15	R11
      0x7C340400,  //  0039  CALL	R13	2
      0x9818120C,  //  003A  SETIDX	R6	R9	R12
      0x7001FFDF,  //  003B  JMP		#001C
      0x58200005,  //  003C  LDCONST	R8	K5
      0xAC200200,  //  003D  CATCH	R8	1	0
      0xB0080000,  //  003E  RAISE	2	R0	R0
      0x8C20050C,  //  003F  GETMET	R8	R2	K12
      0x60280018,  //  0040  GETGBL	R10	G24
      0x582C000D,  //  0041  LDCONST	R11	K13
      0x8C30070E,  //  0042  GETMET	R12	R3	K14
      0x5C380C00,  //  0043  MOVE	R14	R6
      0x7C300400,  //  0044  CALL	R12	2
      0x8C34070E,  //  0045  GETMET	R13	R3	K14
      0x5C3C0E00,  //  0046  MOVE	R15	R7
      0x7C340400,  //  0047  CALL	R13	2
      0x7C280600,  //  0048  CALL	R10	3
      0x7C200400,  //  0049  CALL	R8	2
      0x8C20050C,  //  004A  GETMET	R8	R2	K12
      0xB82A1E00,  //  004B  GETNGBL	R10	K15
      0x88281510,  //  004C  GETMBR	R10	R10	K16
      0x7C200400,  //  004D  CALL	R8	2
      0x80000000,  //  004E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_UI
********************************************************************/
be_local_class(Matter_UI,
    1,
    NULL,
    be_nested_map(26,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(equal_map, -1), be_const_static_closure(class_Matter_UI_equal_map_closure) },
        { be_const_key_weak(page_part_mgr_adv, -1), be_const_closure(class_Matter_UI_page_part_mgr_adv_closure) },
        { be_const_key_weak(_CLASSES_TYPES2, -1), be_nested_str_weak(_X7Chttp_relay_X7Chttp_light0_X7Chttp_light1_X7Chttp_light2_X7Chttp_light3_X7Chttp_temperature_X7Chttp_pressure_X7Chttp_illuminance_X7Chttp_humidity_X7Chttp_occupancy_X7Chttp_contact_X7Chttp_flow_X7Chttp_rain_X7Chttp_waterleak_X7Chttp_airquality) },
        { be_const_key_weak(page_part_mgr, 25), be_const_closure(class_Matter_UI_page_part_mgr_closure) },
        { be_const_key_weak(show_plugins_hints_js, -1), be_const_closure(class_Matter_UI_show_plugins_hints_js_closure) },
        { be_const_key_weak(show_enable, -1), be_const_closure(class_Matter_UI_show_enable_closure) },
        { be_const_key_weak(matter_enabled, 24), be_const_closure(class_Matter_UI_matter_enabled_closure) },
        { be_const_key_weak(generate_config_from_status, -1), be_const_closure(class_Matter_UI_generate_config_from_status_closure) },
        { be_const_key_weak(show_qrcode, 21), be_const_closure(class_Matter_UI_show_qrcode_closure) },
        { be_const_key_weak(page_part_mgr_add, -1), be_const_closure(class_Matter_UI_page_part_mgr_add_closure) },
        { be_const_key_weak(show_plugins_configuration, -1), be_const_closure(class_Matter_UI_show_plugins_configuration_closure) },
        { be_const_key_weak(show_commissioning_info, -1), be_const_closure(class_Matter_UI_show_commissioning_info_closure) },
        { be_const_key_weak(page_part_ctl, 18), be_const_closure(class_Matter_UI_page_part_ctl_closure) },
        { be_const_key_weak(show_fabric_info, -1), be_const_closure(class_Matter_UI_show_fabric_info_closure) },
        { be_const_key_weak(_CLASSES_TYPES, 4), be_nested_str_long(_X7Crelay_X7Clight0_X7Clight1_X7Clight2_X7Clight3_X7Cshutter_X7Cshutter_X2Btilt_X7Cgensw_btn_X7Ctemperature_X7Cpressure_X7Cilluminance_X7Chumidity_X7Coccupancy_X7Conoff_X7Ccontact_X7Cflow_X7Crain_X7Cwaterleak_X7Cairquality_X7C_X2Dvirtual_X7Cv_relay_X7Cv_light0_X7Cv_light1_X7Cv_light2_X7Cv_light3_X7Cv_fan_X7Cv_temp_X7Cv_pressure_X7Cv_illuminance_X7Cv_humidity_X7Cv_occupancy_X7Cv_contact_X7Cv_flow_X7Cv_rain_X7Cv_waterleak_X7Cv_airquality) },
        { be_const_key_weak(web_get_arg, -1), be_const_closure(class_Matter_UI_web_get_arg_closure) },
        { be_const_key_weak(plugin_option, 5), be_const_closure(class_Matter_UI_plugin_option_closure) },
        { be_const_key_weak(web_add_config_button, -1), be_const_closure(class_Matter_UI_web_add_config_button_closure) },
        { be_const_key_weak(show_passcode_form, -1), be_const_closure(class_Matter_UI_show_passcode_form_closure) },
        { be_const_key_weak(show_remote_autoconf, 8), be_const_closure(class_Matter_UI_show_remote_autoconf_closure) },
        { be_const_key_weak(web_add_handler, -1), be_const_closure(class_Matter_UI_web_add_handler_closure) },
        { be_const_key_weak(show_bridge_status, 23), be_const_closure(class_Matter_UI_show_bridge_status_closure) },
        { be_const_key_weak(plugin_name, 12), be_const_closure(class_Matter_UI_plugin_name_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_UI_init_closure) },
        { be_const_key_weak(web_sensor, -1), be_const_closure(class_Matter_UI_web_sensor_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_UI)
);
/********************************************************************/
/* End of solidification */
