/* Solidification of Matter_UI.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_UI;

/********************************************************************
** Solidified function: equal_map
********************************************************************/
be_local_closure(Matter_UI_equal_map,   /* name */
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
be_local_closure(Matter_UI_page_part_mgr_adv,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(check_privileged_access),
    /* K2   */  be_nested_str_weak(content_start),
    /* K3   */  be_nested_str_weak(Matter_X20Advanced_X20Configuration),
    /* K4   */  be_nested_str_weak(content_send_style),
    /* K5   */  be_nested_str_weak(matter_enabled),
    /* K6   */  be_nested_str_weak(show_passcode_form),
    /* K7   */  be_nested_str_weak(show_fabric_info),
    /* K8   */  be_nested_str_weak(content_button),
    /* K9   */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K10  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr_adv),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
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
      0x8C080308,  //  0012  GETMET	R2	R1	K8
      0x88100309,  //  0013  GETMBR	R4	R1	K9
      0x7C080400,  //  0014  CALL	R2	2
      0x8C08030A,  //  0015  GETMET	R2	R1	K10
      0x7C080200,  //  0016  CALL	R2	1
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_mgr
********************************************************************/
be_local_closure(Matter_UI_page_part_mgr,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(check_privileged_access),
    /* K2   */  be_nested_str_weak(content_start),
    /* K3   */  be_nested_str_weak(Matter),
    /* K4   */  be_nested_str_weak(content_send_style),
    /* K5   */  be_nested_str_weak(show_enable),
    /* K6   */  be_nested_str_weak(matter_enabled),
    /* K7   */  be_nested_str_weak(show_plugins_configuration),
    /* K8   */  be_nested_str_weak(content_send),
    /* K9   */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K10  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cform_X20id_X3D_X27butmat_X27_X20style_X3D_X27display_X3A_X20block_X3B_X27_X20action_X3D_X27mattera_X27_X20method_X3D_X27get_X27_X3E_X3Cbutton_X20name_X3D_X27_X27_X3EAdvanced_X20Configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K11  */  be_nested_str_weak(content_button),
    /* K12  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K13  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
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
      0x8C080308,  //  0015  GETMET	R2	R1	K8
      0x5810000A,  //  0016  LDCONST	R4	K10
      0x7C080400,  //  0017  CALL	R2	2
      0x8C08030B,  //  0018  GETMET	R2	R1	K11
      0x8810030C,  //  0019  GETMBR	R4	R1	K12
      0x7C080400,  //  001A  CALL	R2	2
      0x8C08030D,  //  001B  GETMET	R2	R1	K13
      0x7C080200,  //  001C  CALL	R2	1
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: plugin_name
********************************************************************/
be_local_closure(Matter_UI_plugin_name,   /* name */
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
be_local_closure(Matter_UI_matter_enabled,   /* name */
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
be_local_closure(Matter_UI_generate_config_from_status,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
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
    /* K16  */  be_nested_str_weak(autoconf_sensors_list),
    }),
    be_str_weak(generate_config_from_status),
    &be_const_str_solidified,
    ( &(const binstruction[96]) {  /* code */
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
      0x8C201110,  //  005B  GETMET	R8	R8	K16
      0x5C280200,  //  005C  MOVE	R10	R1
      0x7C200400,  //  005D  CALL	R8	2
      0x000C0608,  //  005E  ADD	R3	R3	R8
      0x80040600,  //  005F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_sensor
********************************************************************/
be_local_closure(Matter_UI_web_sensor,   /* name */
  be_nested_proto(
    10,                          /* nstack */
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
    /* K15  */  be_nested_str_weak(is_root_commissioning_open),
    /* K16  */  be_nested_str_weak(show_commissioning_info),
    }),
    be_str_weak(web_sensor),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x780A0024,  //  0003  JMPF	R2	#0029
      0x88080102,  //  0004  GETMBR	R2	R0	K2
      0x88080503,  //  0005  GETMBR	R2	R2	K3
      0x8C080504,  //  0006  GETMET	R2	R2	K4
      0x7C080200,  //  0007  CALL	R2	1
      0x1C0C0505,  //  0008  EQ	R3	R2	K5
      0x780E0006,  //  0009  JMPF	R3	#0011
      0x8C0C0306,  //  000A  GETMET	R3	R1	K6
      0x60140018,  //  000B  GETGBL	R5	G24
      0x58180007,  //  000C  LDCONST	R6	K7
      0x581C0008,  //  000D  LDCONST	R7	K8
      0x7C140400,  //  000E  CALL	R5	2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x7002000F,  //  0010  JMP		#0021
      0x240C0509,  //  0011  GT	R3	R2	K9
      0x8C100306,  //  0012  GETMET	R4	R1	K6
      0x60180018,  //  0013  GETGBL	R6	G24
      0x581C0007,  //  0014  LDCONST	R7	K7
      0x60200008,  //  0015  GETGBL	R8	G8
      0x5C240400,  //  0016  MOVE	R9	R2
      0x7C200200,  //  0017  CALL	R8	1
      0x00221408,  //  0018  ADD	R8	K10	R8
      0x0020110B,  //  0019  ADD	R8	R8	K11
      0x780E0001,  //  001A  JMPF	R3	#001D
      0x5824000C,  //  001B  LDCONST	R9	K12
      0x70020000,  //  001C  JMP		#001E
      0x5824000D,  //  001D  LDCONST	R9	K13
      0x00201009,  //  001E  ADD	R8	R8	R9
      0x7C180400,  //  001F  CALL	R6	2
      0x7C100400,  //  0020  CALL	R4	2
      0x8C0C010E,  //  0021  GETMET	R3	R0	K14
      0x7C0C0200,  //  0022  CALL	R3	1
      0x880C0102,  //  0023  GETMBR	R3	R0	K2
      0x8C0C070F,  //  0024  GETMET	R3	R3	K15
      0x7C0C0200,  //  0025  CALL	R3	1
      0x780E0001,  //  0026  JMPF	R3	#0029
      0x8C0C0110,  //  0027  GETMET	R3	R0	K16
      0x7C0C0200,  //  0028  CALL	R3	1
      0x80000000,  //  0029  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_mgr_add
********************************************************************/
be_local_closure(Matter_UI_page_part_mgr_add,   /* name */
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
be_local_closure(Matter_UI_show_plugins_configuration,   /* name */
  be_nested_proto(
    28,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[63]) {     /* constants */
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
    /* K20  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25i_X3C_X2Fb_X3E_X3C_X2Ftd_X3E),
    /* K21  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X3E_X3C_X2Ftd_X3E),
    /* K22  */  be_nested_str_weak(html_escape),
    /* K23  */  be_nested_str_weak(name),
    /* K24  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Ftd_X3E),
    /* K25  */  be_nested_str_weak(plugin_name),
    /* K26  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X20placeholder_X3D_X27_X25s_X27_X3E_X3C_X2Ftd_X3E),
    /* K27  */  be_nested_str_weak(ARG_HINT),
    /* K28  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27text_X2Dalign_X3Acenter_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27del_X25i_X27_X20style_X3D_X27background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3B_X27_X20onclick_X3D_X22return_X20confirm_X28_X27Confirm_X20removing_X20endpoint_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K29  */  be_nested_str_weak(_X3C_X2Ftable_X3E),
    /* K30  */  be_nested_str_weak(_X3Cp_X3E_X26lt_X3Bnone_X26gt_X3B_X3C_X2Fp_X3E),
    /* K31  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K32  */  be_nested_str_weak(url),
    /* K33  */  be_nested_str_weak(push),
    /* K34  */  be_nested_str_weak(stop_iteration),
    /* K35  */  be_nested_str_weak(sort_distinct),
    /* K36  */  be_nested_str_weak(get_plugin_remote_info),
    /* K37  */  be_nested_str_weak(_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20title_X3D_X27http_X3A_X2F_X2F_X25s_X2F_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X3E_X25s_X3C_X2Fa_X3E),
    /* K38  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K39  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2778_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K40  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2722_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25i_X3C_X2Fb_X3E_X3C_X2Ftd_X3E),
    /* K41  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X2778_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X20placeholder_X3D_X27_X28optional_X29_X27_X3E_X3C_X2Ftd_X3E),
    /* K42  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E),
    /* K43  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X25i_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X25s_X27_X3E_X3C_X2Ftd_X3E),
    /* K44  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X2715_X27_X20style_X3D_X27text_X2Dalign_X3Acenter_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27del_X25i_X27_X20style_X3D_X27background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3B_X27_X20onclick_X3D_X22return_X20confirm_X28_X27Confirm_X20removing_X20endpoint_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K45  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K46  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27config_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X20configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    /* K47  */  be_nested_str_weak(show_plugins_hints_js),
    /* K48  */  be_nested_str_weak(_CLASSES_TYPES),
    /* K49  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BAdd_X20to_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K50  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3EAdd_X20local_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K51  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X27100_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EName_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EParameter_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K52  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X27_X20placeholder_X3D_X27_X28optional_X29_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cselect_X20id_X3D_X27pi_X27_X20name_X3D_X27pi_X27_X20onchange_X3D_X27otm_X28_X22arg_X22_X2Cthis_X2Evalue_X29_X27_X3E),
    /* K53  */  be_nested_str_weak(plugin_option),
    /* K54  */  be_nested_str_weak(_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E),
    /* K55  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20id_X3D_X27arg_X27_X20name_X3D_X27arg_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E),
    /* K56  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K57  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27addep_X27_X20class_X3D_X27button_X20bgrn_X27_X3ECreate_X20new_X20endpoint_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K58  */  be_nested_str_weak(_X3Chr_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20Tasmota_X20or_X20OpenBK_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatteradd_X27_X20method_X3D_X27get_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K59  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2730_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3Ehttp_X3A_X2F_X2F_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27url_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X27_X20required_X20placeholder_X3D_X27IP_X20or_X20domain_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2710_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X2F_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E),
    /* K60  */  be_nested_str_weak(_X3Cbutton_X20class_X3D_X27button_X20bgrn_X27_X3EAuto_X2Dconfigure_X20remote_X20Tasmota_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Chr_X3E),
    /* K61  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20RESET_X20the_X20configuration_X20to_X20the_X20default_X2E_X20You_X20will_X20need_X20to_X20associate_X20again_X2E_X22_X29_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27auto_X27_X20class_X3D_X27button_X20bred_X27_X3EReset_X20all_X20and_X20Auto_X2Ddiscover_X3C_X2Fbutton_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Fform_X3E),
    /* K62  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    }),
    be_str_weak(show_plugins_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[352]) {  /* code */
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
      0x781E0059,  //  001C  JMPF	R7	#0077
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
      0x4C300000,  //  003A  LDNIL	R12
      0x2030140C,  //  003B  NE	R12	R10	R12
      0x78320004,  //  003C  JMPF	R12	#0042
      0x8C301513,  //  003D  GETMET	R12	R10	K19
      0x5C381400,  //  003E  MOVE	R14	R10
      0x5C3C1000,  //  003F  MOVE	R15	R8
      0x7C300600,  //  0040  CALL	R12	3
      0x5C2C1800,  //  0041  MOVE	R11	R12
      0x50180200,  //  0042  LDBOOL	R6	1	0
      0x8C300303,  //  0043  GETMET	R12	R1	K3
      0x60380018,  //  0044  GETGBL	R14	G24
      0x583C0014,  //  0045  LDCONST	R15	K20
      0x5C400E00,  //  0046  MOVE	R16	R7
      0x7C380400,  //  0047  CALL	R14	2
      0x7C300400,  //  0048  CALL	R12	2
      0x8C300303,  //  0049  GETMET	R12	R1	K3
      0x60380018,  //  004A  GETGBL	R14	G24
      0x583C0015,  //  004B  LDCONST	R15	K21
      0x5C400E00,  //  004C  MOVE	R16	R7
      0x8C440316,  //  004D  GETMET	R17	R1	K22
      0x8C4C110D,  //  004E  GETMET	R19	R8	K13
      0x58540017,  //  004F  LDCONST	R21	K23
      0x58580012,  //  0050  LDCONST	R22	K18
      0x7C4C0600,  //  0051  CALL	R19	3
      0x7C440400,  //  0052  CALL	R17	2
      0x7C380600,  //  0053  CALL	R14	3
      0x7C300400,  //  0054  CALL	R12	2
      0x8C300303,  //  0055  GETMET	R12	R1	K3
      0x60380018,  //  0056  GETGBL	R14	G24
      0x583C0018,  //  0057  LDCONST	R15	K24
      0x8C400119,  //  0058  GETMET	R16	R0	K25
      0x8C48110D,  //  0059  GETMET	R18	R8	K13
      0x5850000E,  //  005A  LDCONST	R20	K14
      0x58540012,  //  005B  LDCONST	R21	K18
      0x7C480600,  //  005C  CALL	R18	3
      0x7C400400,  //  005D  CALL	R16	2
      0x7C380400,  //  005E  CALL	R14	2
      0x7C300400,  //  005F  CALL	R12	2
      0x8C300303,  //  0060  GETMET	R12	R1	K3
      0x60380018,  //  0061  GETGBL	R14	G24
      0x583C001A,  //  0062  LDCONST	R15	K26
      0x5C400E00,  //  0063  MOVE	R16	R7
      0x8C440316,  //  0064  GETMET	R17	R1	K22
      0x5C4C1600,  //  0065  MOVE	R19	R11
      0x7C440400,  //  0066  CALL	R17	2
      0x782A0003,  //  0067  JMPF	R10	#006C
      0x8C480316,  //  0068  GETMET	R18	R1	K22
      0x8850151B,  //  0069  GETMBR	R20	R10	K27
      0x7C480400,  //  006A  CALL	R18	2
      0x70020000,  //  006B  JMP		#006D
      0x58480012,  //  006C  LDCONST	R18	K18
      0x7C380800,  //  006D  CALL	R14	4
      0x7C300400,  //  006E  CALL	R12	2
      0x8C300303,  //  006F  GETMET	R12	R1	K3
      0x60380018,  //  0070  GETGBL	R14	G24
      0x583C001C,  //  0071  LDCONST	R15	K28
      0x5C400E00,  //  0072  MOVE	R16	R7
      0x7C380400,  //  0073  CALL	R14	2
      0x7C300400,  //  0074  CALL	R12	2
      0x00140B0F,  //  0075  ADD	R5	R5	K15
      0x7001FFA0,  //  0076  JMP		#0018
      0x8C1C0303,  //  0077  GETMET	R7	R1	K3
      0x5824001D,  //  0078  LDCONST	R9	K29
      0x7C1C0400,  //  0079  CALL	R7	2
      0x5C1C0C00,  //  007A  MOVE	R7	R6
      0x741E0002,  //  007B  JMPT	R7	#007F
      0x8C1C0303,  //  007C  GETMET	R7	R1	K3
      0x5824001E,  //  007D  LDCONST	R9	K30
      0x7C1C0400,  //  007E  CALL	R7	2
      0x8C1C0303,  //  007F  GETMET	R7	R1	K3
      0x5824001F,  //  0080  LDCONST	R9	K31
      0x7C1C0400,  //  0081  CALL	R7	2
      0x601C0012,  //  0082  GETGBL	R7	G18
      0x7C1C0000,  //  0083  CALL	R7	0
      0x60200010,  //  0084  GETGBL	R8	G16
      0x88240107,  //  0085  GETMBR	R9	R0	K7
      0x88241308,  //  0086  GETMBR	R9	R9	K8
      0x7C200200,  //  0087  CALL	R8	1
      0xA802000B,  //  0088  EXBLK	0	#0095
      0x5C241000,  //  0089  MOVE	R9	R8
      0x7C240000,  //  008A  CALL	R9	0
      0x8C28130D,  //  008B  GETMET	R10	R9	K13
      0x58300020,  //  008C  LDCONST	R12	K32
      0x7C280400,  //  008D  CALL	R10	2
      0x4C2C0000,  //  008E  LDNIL	R11
      0x202C140B,  //  008F  NE	R11	R10	R11
      0x782E0002,  //  0090  JMPF	R11	#0094
      0x8C2C0F21,  //  0091  GETMET	R11	R7	K33
      0x5C341400,  //  0092  MOVE	R13	R10
      0x7C2C0400,  //  0093  CALL	R11	2
      0x7001FFF3,  //  0094  JMP		#0089
      0x58200022,  //  0095  LDCONST	R8	K34
      0xAC200200,  //  0096  CATCH	R8	1	0
      0xB0080000,  //  0097  RAISE	2	R0	R0
      0x88200107,  //  0098  GETMBR	R8	R0	K7
      0x8C201123,  //  0099  GETMET	R8	R8	K35
      0x5C280E00,  //  009A  MOVE	R10	R7
      0x7C200400,  //  009B  CALL	R8	2
      0x60200010,  //  009C  GETGBL	R8	G16
      0x5C240E00,  //  009D  MOVE	R9	R7
      0x7C200200,  //  009E  CALL	R8	1
      0xA8020082,  //  009F  EXBLK	0	#0123
      0x5C241000,  //  00A0  MOVE	R9	R8
      0x7C240000,  //  00A1  CALL	R9	0
      0x8C280316,  //  00A2  GETMET	R10	R1	K22
      0x5C301200,  //  00A3  MOVE	R12	R9
      0x7C280400,  //  00A4  CALL	R10	2
      0x8C2C0316,  //  00A5  GETMET	R11	R1	K22
      0x88340107,  //  00A6  GETMBR	R13	R0	K7
      0x8C341B24,  //  00A7  GETMET	R13	R13	K36
      0x5C3C1200,  //  00A8  MOVE	R15	R9
      0x7C340400,  //  00A9  CALL	R13	2
      0x8C341B0D,  //  00AA  GETMET	R13	R13	K13
      0x583C0017,  //  00AB  LDCONST	R15	K23
      0x5C401200,  //  00AC  MOVE	R16	R9
      0x7C340600,  //  00AD  CALL	R13	3
      0x7C2C0400,  //  00AE  CALL	R11	2
      0x8C300303,  //  00AF  GETMET	R12	R1	K3
      0x60380018,  //  00B0  GETGBL	R14	G24
      0x583C0025,  //  00B1  LDCONST	R15	K37
      0x5C401400,  //  00B2  MOVE	R16	R10
      0x5C441400,  //  00B3  MOVE	R17	R10
      0x5C481600,  //  00B4  MOVE	R18	R11
      0x7C380800,  //  00B5  CALL	R14	4
      0x7C300400,  //  00B6  CALL	R12	2
      0x8C300303,  //  00B7  GETMET	R12	R1	K3
      0x58380026,  //  00B8  LDCONST	R14	K38
      0x7C300400,  //  00B9  CALL	R12	2
      0x8C300303,  //  00BA  GETMET	R12	R1	K3
      0x58380027,  //  00BB  LDCONST	R14	K39
      0x7C300400,  //  00BC  CALL	R12	2
      0x50180000,  //  00BD  LDBOOL	R6	0	0
      0x5814000C,  //  00BE  LDCONST	R5	K12
      0x6030000C,  //  00BF  GETGBL	R12	G12
      0x5C340800,  //  00C0  MOVE	R13	R4
      0x7C300200,  //  00C1  CALL	R12	1
      0x14300A0C,  //  00C2  LT	R12	R5	R12
      0x7832005A,  //  00C3  JMPF	R12	#011F
      0x94300805,  //  00C4  GETIDX	R12	R4	R5
      0x88340107,  //  00C5  GETMBR	R13	R0	K7
      0x88341B08,  //  00C6  GETMBR	R13	R13	K8
      0x8C341B0D,  //  00C7  GETMET	R13	R13	K13
      0x603C0008,  //  00C8  GETGBL	R15	G8
      0x5C401800,  //  00C9  MOVE	R16	R12
      0x7C3C0200,  //  00CA  CALL	R15	1
      0x7C340400,  //  00CB  CALL	R13	2
      0x8C381B0D,  //  00CC  GETMET	R14	R13	K13
      0x5840000E,  //  00CD  LDCONST	R16	K14
      0x7C380400,  //  00CE  CALL	R14	2
      0x5C3C1C00,  //  00CF  MOVE	R15	R14
      0x743E0001,  //  00D0  JMPT	R15	#00D3
      0x00140B0F,  //  00D1  ADD	R5	R5	K15
      0x7001FFEB,  //  00D2  JMP		#00BF
      0x8C3C050D,  //  00D3  GETMET	R15	R2	K13
      0x5C441C00,  //  00D4  MOVE	R17	R14
      0x58480010,  //  00D5  LDCONST	R18	K16
      0x7C3C0600,  //  00D6  CALL	R15	3
      0x203C1F0C,  //  00D7  NE	R15	R15	K12
      0x783E0001,  //  00D8  JMPF	R15	#00DB
      0x00140B0F,  //  00D9  ADD	R5	R5	K15
      0x7001FFE3,  //  00DA  JMP		#00BF
      0x8C3C1B0D,  //  00DB  GETMET	R15	R13	K13
      0x58440020,  //  00DC  LDCONST	R17	K32
      0x7C3C0400,  //  00DD  CALL	R15	2
      0x203C1E09,  //  00DE  NE	R15	R15	R9
      0x783E0001,  //  00DF  JMPF	R15	#00E2
      0x00140B0F,  //  00E0  ADD	R5	R5	K15
      0x7001FFDC,  //  00E1  JMP		#00BF
      0x883C0107,  //  00E2  GETMBR	R15	R0	K7
      0x883C1F11,  //  00E3  GETMBR	R15	R15	K17
      0x8C3C1F0D,  //  00E4  GETMET	R15	R15	K13
      0x5C441C00,  //  00E5  MOVE	R17	R14
      0x7C3C0400,  //  00E6  CALL	R15	2
      0x58400012,  //  00E7  LDCONST	R16	K18
      0x4C440000,  //  00E8  LDNIL	R17
      0x20441E11,  //  00E9  NE	R17	R15	R17
      0x78460004,  //  00EA  JMPF	R17	#00F0
      0x8C441F13,  //  00EB  GETMET	R17	R15	K19
      0x5C4C1E00,  //  00EC  MOVE	R19	R15
      0x5C501A00,  //  00ED  MOVE	R20	R13
      0x7C440600,  //  00EE  CALL	R17	3
      0x5C402200,  //  00EF  MOVE	R16	R17
      0x50180200,  //  00F0  LDBOOL	R6	1	0
      0x8C440303,  //  00F1  GETMET	R17	R1	K3
      0x604C0018,  //  00F2  GETGBL	R19	G24
      0x58500028,  //  00F3  LDCONST	R20	K40
      0x5C541800,  //  00F4  MOVE	R21	R12
      0x7C4C0400,  //  00F5  CALL	R19	2
      0x7C440400,  //  00F6  CALL	R17	2
      0x8C440303,  //  00F7  GETMET	R17	R1	K3
      0x604C0018,  //  00F8  GETGBL	R19	G24
      0x58500029,  //  00F9  LDCONST	R20	K41
      0x5C541800,  //  00FA  MOVE	R21	R12
      0x8C580316,  //  00FB  GETMET	R22	R1	K22
      0x8C601B0D,  //  00FC  GETMET	R24	R13	K13
      0x58680017,  //  00FD  LDCONST	R26	K23
      0x586C0012,  //  00FE  LDCONST	R27	K18
      0x7C600600,  //  00FF  CALL	R24	3
      0x7C580400,  //  0100  CALL	R22	2
      0x7C4C0600,  //  0101  CALL	R19	3
      0x7C440400,  //  0102  CALL	R17	2
      0x8C440303,  //  0103  GETMET	R17	R1	K3
      0x604C0018,  //  0104  GETGBL	R19	G24
      0x5850002A,  //  0105  LDCONST	R20	K42
      0x8C540119,  //  0106  GETMET	R21	R0	K25
      0x8C5C1B0D,  //  0107  GETMET	R23	R13	K13
      0x5864000E,  //  0108  LDCONST	R25	K14
      0x58680012,  //  0109  LDCONST	R26	K18
      0x7C5C0600,  //  010A  CALL	R23	3
      0x7C540400,  //  010B  CALL	R21	2
      0x7C4C0400,  //  010C  CALL	R19	2
      0x7C440400,  //  010D  CALL	R17	2
      0x8C440303,  //  010E  GETMET	R17	R1	K3
      0x604C0018,  //  010F  GETGBL	R19	G24
      0x5850002B,  //  0110  LDCONST	R20	K43
      0x5C541800,  //  0111  MOVE	R21	R12
      0x8C580316,  //  0112  GETMET	R22	R1	K22
      0x5C602000,  //  0113  MOVE	R24	R16
      0x7C580400,  //  0114  CALL	R22	2
      0x7C4C0600,  //  0115  CALL	R19	3
      0x7C440400,  //  0116  CALL	R17	2
      0x8C440303,  //  0117  GETMET	R17	R1	K3
      0x604C0018,  //  0118  GETGBL	R19	G24
      0x5850002C,  //  0119  LDCONST	R20	K44
      0x5C541800,  //  011A  MOVE	R21	R12
      0x7C4C0400,  //  011B  CALL	R19	2
      0x7C440400,  //  011C  CALL	R17	2
      0x00140B0F,  //  011D  ADD	R5	R5	K15
      0x7001FF9F,  //  011E  JMP		#00BF
      0x8C300303,  //  011F  GETMET	R12	R1	K3
      0x5838002D,  //  0120  LDCONST	R14	K45
      0x7C300400,  //  0121  CALL	R12	2
      0x7001FF7C,  //  0122  JMP		#00A0
      0x58200022,  //  0123  LDCONST	R8	K34
      0xAC200200,  //  0124  CATCH	R8	1	0
      0xB0080000,  //  0125  RAISE	2	R0	R0
      0x5C200C00,  //  0126  MOVE	R8	R6
      0x74220002,  //  0127  JMPT	R8	#012B
      0x8C200303,  //  0128  GETMET	R8	R1	K3
      0x5828001E,  //  0129  LDCONST	R10	K30
      0x7C200400,  //  012A  CALL	R8	2
      0x8C200303,  //  012B  GETMET	R8	R1	K3
      0x5828002E,  //  012C  LDCONST	R10	K46
      0x7C200400,  //  012D  CALL	R8	2
      0x8C20012F,  //  012E  GETMET	R8	R0	K47
      0x88280130,  //  012F  GETMBR	R10	R0	K48
      0x7C200400,  //  0130  CALL	R8	2
      0x8C200303,  //  0131  GETMET	R8	R1	K3
      0x58280031,  //  0132  LDCONST	R10	K49
      0x7C200400,  //  0133  CALL	R8	2
      0x8C200303,  //  0134  GETMET	R8	R1	K3
      0x58280032,  //  0135  LDCONST	R10	K50
      0x7C200400,  //  0136  CALL	R8	2
      0x8C200303,  //  0137  GETMET	R8	R1	K3
      0x58280033,  //  0138  LDCONST	R10	K51
      0x7C200400,  //  0139  CALL	R8	2
      0x8C200303,  //  013A  GETMET	R8	R1	K3
      0x58280034,  //  013B  LDCONST	R10	K52
      0x7C200400,  //  013C  CALL	R8	2
      0x8C200135,  //  013D  GETMET	R8	R0	K53
      0x58280012,  //  013E  LDCONST	R10	K18
      0x882C0130,  //  013F  GETMBR	R11	R0	K48
      0x7C200600,  //  0140  CALL	R8	3
      0x8C200303,  //  0141  GETMET	R8	R1	K3
      0x58280036,  //  0142  LDCONST	R10	K54
      0x7C200400,  //  0143  CALL	R8	2
      0x8C200303,  //  0144  GETMET	R8	R1	K3
      0x58280037,  //  0145  LDCONST	R10	K55
      0x7C200400,  //  0146  CALL	R8	2
      0x8C200303,  //  0147  GETMET	R8	R1	K3
      0x58280038,  //  0148  LDCONST	R10	K56
      0x7C200400,  //  0149  CALL	R8	2
      0x8C200303,  //  014A  GETMET	R8	R1	K3
      0x58280039,  //  014B  LDCONST	R10	K57
      0x7C200400,  //  014C  CALL	R8	2
      0x8C200303,  //  014D  GETMET	R8	R1	K3
      0x5828003A,  //  014E  LDCONST	R10	K58
      0x7C200400,  //  014F  CALL	R8	2
      0x8C200303,  //  0150  GETMET	R8	R1	K3
      0x5828003B,  //  0151  LDCONST	R10	K59
      0x7C200400,  //  0152  CALL	R8	2
      0x8C200303,  //  0153  GETMET	R8	R1	K3
      0x58280038,  //  0154  LDCONST	R10	K56
      0x7C200400,  //  0155  CALL	R8	2
      0x8C200303,  //  0156  GETMET	R8	R1	K3
      0x5828003C,  //  0157  LDCONST	R10	K60
      0x7C200400,  //  0158  CALL	R8	2
      0x8C200303,  //  0159  GETMET	R8	R1	K3
      0x5828003D,  //  015A  LDCONST	R10	K61
      0x7C200400,  //  015B  CALL	R8	2
      0x8C200303,  //  015C  GETMET	R8	R1	K3
      0x5828003E,  //  015D  LDCONST	R10	K62
      0x7C200400,  //  015E  CALL	R8	2
      0x80000000,  //  015F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_commissioning_info
********************************************************************/
be_local_closure(Matter_UI_show_commissioning_info,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(commissioning_open),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(millis),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(content_send),
    /* K7   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BCommissioning_X20open_X20for_X20_X25i_X20min_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K8   */  be_nested_str_weak(compute_manual_pairing_code),
    /* K9   */  be_nested_str_weak(_X3Cp_X3EManual_X20pairing_X20code_X3A_X3Cbr_X3E_X3Cb_X3E_X25s_X2D_X25s_X2D_X25s_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Chr_X3E),
    /* K10  */  be_const_int(3),
    /* K11  */  be_const_int(2147483647),
    /* K12  */  be_nested_str_weak(_X3Cdiv_X3E_X3Ccenter_X3E),
    /* K13  */  be_nested_str_weak(compute_qrcode_content),
    /* K14  */  be_nested_str_weak(show_qrcode),
    /* K15  */  be_nested_str_weak(_X3Cp_X3E_X20_X25s_X3C_X2Fp_X3E),
    /* K16  */  be_nested_str_weak(_X3C_X2Fdiv_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_commissioning_info),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x88080502,  //  0002  GETMBR	R2	R2	K2
      0xB80E0600,  //  0003  GETNGBL	R3	K3
      0x8C0C0704,  //  0004  GETMET	R3	R3	K4
      0x7C0C0200,  //  0005  CALL	R3	1
      0x04080403,  //  0006  SUB	R2	R2	R3
      0x540E03E7,  //  0007  LDINT	R3	1000
      0x0C080403,  //  0008  DIV	R2	R2	R3
      0x140C0505,  //  0009  LT	R3	R2	K5
      0x780E0000,  //  000A  JMPF	R3	#000C
      0x58080005,  //  000B  LDCONST	R2	K5
      0x540E001D,  //  000C  LDINT	R3	30
      0x000C0403,  //  000D  ADD	R3	R2	R3
      0x5412003B,  //  000E  LDINT	R4	60
      0x0C0C0604,  //  000F  DIV	R3	R3	R4
      0x8C100306,  //  0010  GETMET	R4	R1	K6
      0x60180018,  //  0011  GETGBL	R6	G24
      0x581C0007,  //  0012  LDCONST	R7	K7
      0x5C200600,  //  0013  MOVE	R8	R3
      0x7C180400,  //  0014  CALL	R6	2
      0x7C100400,  //  0015  CALL	R4	2
      0x88100101,  //  0016  GETMBR	R4	R0	K1
      0x8C100908,  //  0017  GETMET	R4	R4	K8
      0x7C100200,  //  0018  CALL	R4	1
      0x8C140306,  //  0019  GETMET	R5	R1	K6
      0x601C0018,  //  001A  GETGBL	R7	G24
      0x58200009,  //  001B  LDCONST	R8	K9
      0x40260B0A,  //  001C  CONNECT	R9	K5	K10
      0x94240809,  //  001D  GETIDX	R9	R4	R9
      0x542A0003,  //  001E  LDINT	R10	4
      0x542E0005,  //  001F  LDINT	R11	6
      0x4028140B,  //  0020  CONNECT	R10	R10	R11
      0x9428080A,  //  0021  GETIDX	R10	R4	R10
      0x542E0006,  //  0022  LDINT	R11	7
      0x402C170B,  //  0023  CONNECT	R11	R11	K11
      0x942C080B,  //  0024  GETIDX	R11	R4	R11
      0x7C1C0800,  //  0025  CALL	R7	4
      0x7C140400,  //  0026  CALL	R5	2
      0x8C140306,  //  0027  GETMET	R5	R1	K6
      0x581C000C,  //  0028  LDCONST	R7	K12
      0x7C140400,  //  0029  CALL	R5	2
      0x88140101,  //  002A  GETMBR	R5	R0	K1
      0x8C140B0D,  //  002B  GETMET	R5	R5	K13
      0x7C140200,  //  002C  CALL	R5	1
      0x8C18010E,  //  002D  GETMET	R6	R0	K14
      0x5C200A00,  //  002E  MOVE	R8	R5
      0x7C180400,  //  002F  CALL	R6	2
      0x8C180306,  //  0030  GETMET	R6	R1	K6
      0x60200018,  //  0031  GETGBL	R8	G24
      0x5824000F,  //  0032  LDCONST	R9	K15
      0x5C280A00,  //  0033  MOVE	R10	R5
      0x7C200400,  //  0034  CALL	R8	2
      0x7C180400,  //  0035  CALL	R6	2
      0x8C180306,  //  0036  GETMET	R6	R1	K6
      0x58200010,  //  0037  LDCONST	R8	K16
      0x7C180400,  //  0038  CALL	R6	2
      0x80000000,  //  0039  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_remote_autoconf
********************************************************************/
be_local_closure(Matter_UI_show_remote_autoconf,   /* name */
  be_nested_proto(
    25,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[47]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Plugin_Bridge_HTTP),
    /* K5   */  be_nested_str_weak(PROBE_TIMEOUT),
    /* K6   */  be_nested_str_weak(HTTP_remote),
    /* K7   */  be_nested_str_weak(call_sync),
    /* K8   */  be_nested_str_weak(Status_X208),
    /* K9   */  be_nested_str_weak(load),
    /* K10  */  be_nested_str_weak(find),
    /* K11  */  be_nested_str_weak(StatusSNS),
    /* K12  */  be_nested_str_weak(Status_X2011),
    /* K13  */  be_nested_str_weak(StatusSTS),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20probed_X20_X27_X25s_X27_X20status8_X3D_X25s_X20satus11_X3D_X25s),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(generate_config_from_status),
    /* K19  */  be_nested_str_weak(show_plugins_hints_js),
    /* K20  */  be_nested_str_weak(_CLASSES_TYPES2),
    /* K21  */  be_nested_str_weak(content_send),
    /* K22  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Remote_X20Device_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K23  */  be_nested_str_weak(html_escape),
    /* K24  */  be_nested_str_weak(_X3Cp_X3E_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X3E_X25s_X3C_X2Fa_X3E_X3C_X2Fp_X3E),
    /* K25  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X27100_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EName_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EParameter_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K26  */  be_nested_str_weak(_X3Cinput_X20name_X3D_X27url_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X25s_X27_X3E),
    /* K27  */  be_const_int(0),
    /* K28  */  be_nested_str_weak(type),
    /* K29  */  be_nested_str_weak(http_),
    /* K30  */  be_nested_str_weak(device),
    /* K31  */  be_nested_str_weak(plugins_classes),
    /* K32  */  be_nested_str_weak(ui_conf_to_string),
    /* K33  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X27_X20placeholder_X3D_X27_X28optional_X29_X27_X3E_X3C_X2Ftd_X3E),
    /* K34  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cselect_X20name_X3D_X27pi_X25i_X27_X20onchange_X3D_X27otm_X28_X22arg_X25i_X22_X2Cthis_X2Evalue_X29_X27_X3E),
    /* K35  */  be_nested_str_weak(plugin_option),
    /* K36  */  be_nested_str_weak(_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E),
    /* K37  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27text_X27_X20id_X3D_X27arg_X25i_X27_X20name_X3D_X27arg_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X20placeholder_X3D_X27_X25s_X27_X3E),
    /* K38  */  be_nested_str_weak(ARG_HINT),
    /* K39  */  be_nested_str_weak(_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K40  */  be_const_int(1),
    /* K41  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27text_X27_X20id_X3D_X27arg_X25i_X27_X20name_X3D_X27arg_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X3E),
    /* K42  */  be_nested_str_weak(_X3C_X2Ftable_X3E),
    /* K43  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K44  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27addrem_X27_X20class_X3D_X27button_X20bgrn_X27_X3EAdd_X20endpoints_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K45  */  be_nested_str_weak(_X3C_X2Fform_X3E_X3C_X2Ffieldset_X3E),
    /* K46  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3EUnable_X20to_X20connect_X20to_X20_X27_X25s_X27_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_remote_autoconf),
    &be_const_str_solidified,
    ( &(const binstruction[228]) {  /* code */
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
      0x782200A0,  //  0039  JMPF	R8	#00DB
      0x4C200000,  //  003A  LDNIL	R8
      0x20200E08,  //  003B  NE	R8	R7	R8
      0x7822009D,  //  003C  JMPF	R8	#00DB
      0xB8221C00,  //  003D  GETNGBL	R8	K14
      0x8C20110F,  //  003E  GETMET	R8	R8	K15
      0x60280018,  //  003F  GETGBL	R10	G24
      0x582C0010,  //  0040  LDCONST	R11	K16
      0x5C300200,  //  0041  MOVE	R12	R1
      0x60340008,  //  0042  GETGBL	R13	G8
      0x5C380C00,  //  0043  MOVE	R14	R6
      0x7C340200,  //  0044  CALL	R13	1
      0x60380008,  //  0045  GETGBL	R14	G8
      0x5C3C0E00,  //  0046  MOVE	R15	R7
      0x7C380200,  //  0047  CALL	R14	1
      0x7C280800,  //  0048  CALL	R10	4
      0x582C0011,  //  0049  LDCONST	R11	K17
      0x7C200600,  //  004A  CALL	R8	3
      0x8C200112,  //  004B  GETMET	R8	R0	K18
      0x5C280C00,  //  004C  MOVE	R10	R6
      0x5C2C0E00,  //  004D  MOVE	R11	R7
      0x7C200600,  //  004E  CALL	R8	3
      0x8C240113,  //  004F  GETMET	R9	R0	K19
      0x882C0114,  //  0050  GETMBR	R11	R0	K20
      0x7C240400,  //  0051  CALL	R9	2
      0x8C240515,  //  0052  GETMET	R9	R2	K21
      0x582C0016,  //  0053  LDCONST	R11	K22
      0x7C240400,  //  0054  CALL	R9	2
      0x8C240517,  //  0055  GETMET	R9	R2	K23
      0x5C2C0200,  //  0056  MOVE	R11	R1
      0x7C240400,  //  0057  CALL	R9	2
      0x8C280515,  //  0058  GETMET	R10	R2	K21
      0x60300018,  //  0059  GETGBL	R12	G24
      0x58340018,  //  005A  LDCONST	R13	K24
      0x5C381200,  //  005B  MOVE	R14	R9
      0x5C3C1200,  //  005C  MOVE	R15	R9
      0x7C300600,  //  005D  CALL	R12	3
      0x7C280400,  //  005E  CALL	R10	2
      0x8C280515,  //  005F  GETMET	R10	R2	K21
      0x58300019,  //  0060  LDCONST	R12	K25
      0x7C280400,  //  0061  CALL	R10	2
      0x8C280515,  //  0062  GETMET	R10	R2	K21
      0x60300018,  //  0063  GETGBL	R12	G24
      0x5834001A,  //  0064  LDCONST	R13	K26
      0x8C380517,  //  0065  GETMET	R14	R2	K23
      0x5C400200,  //  0066  MOVE	R16	R1
      0x7C380400,  //  0067  CALL	R14	2
      0x7C300400,  //  0068  CALL	R12	2
      0x7C280400,  //  0069  CALL	R10	2
      0x5828001B,  //  006A  LDCONST	R10	K27
      0x602C000C,  //  006B  GETGBL	R11	G12
      0x5C301000,  //  006C  MOVE	R12	R8
      0x7C2C0200,  //  006D  CALL	R11	1
      0x142C140B,  //  006E  LT	R11	R10	R11
      0x782E003E,  //  006F  JMPF	R11	#00AF
      0x942C100A,  //  0070  GETIDX	R11	R8	R10
      0x8C30170A,  //  0071  GETMET	R12	R11	K10
      0x5838001C,  //  0072  LDCONST	R14	K28
      0x583C0002,  //  0073  LDCONST	R15	K2
      0x7C300600,  //  0074  CALL	R12	3
      0x20341902,  //  0075  NE	R13	R12	K2
      0x78360000,  //  0076  JMPF	R13	#0078
      0x00323A0C,  //  0077  ADD	R12	K29	R12
      0x8834011E,  //  0078  GETMBR	R13	R0	K30
      0x88341B1F,  //  0079  GETMBR	R13	R13	K31
      0x8C341B0A,  //  007A  GETMET	R13	R13	K10
      0x5C3C1800,  //  007B  MOVE	R15	R12
      0x7C340400,  //  007C  CALL	R13	2
      0x58380002,  //  007D  LDCONST	R14	K2
      0x4C3C0000,  //  007E  LDNIL	R15
      0x203C1A0F,  //  007F  NE	R15	R13	R15
      0x783E0004,  //  0080  JMPF	R15	#0086
      0x8C3C1B20,  //  0081  GETMET	R15	R13	K32
      0x5C441A00,  //  0082  MOVE	R17	R13
      0x5C481600,  //  0083  MOVE	R18	R11
      0x7C3C0600,  //  0084  CALL	R15	3
      0x5C381E00,  //  0085  MOVE	R14	R15
      0x8C3C0515,  //  0086  GETMET	R15	R2	K21
      0x60440018,  //  0087  GETGBL	R17	G24
      0x58480021,  //  0088  LDCONST	R18	K33
      0x5C4C1400,  //  0089  MOVE	R19	R10
      0x7C440400,  //  008A  CALL	R17	2
      0x7C3C0400,  //  008B  CALL	R15	2
      0x8C3C0515,  //  008C  GETMET	R15	R2	K21
      0x60440018,  //  008D  GETGBL	R17	G24
      0x58480022,  //  008E  LDCONST	R18	K34
      0x5C4C1400,  //  008F  MOVE	R19	R10
      0x5C501400,  //  0090  MOVE	R20	R10
      0x7C440600,  //  0091  CALL	R17	3
      0x7C3C0400,  //  0092  CALL	R15	2
      0x8C3C0123,  //  0093  GETMET	R15	R0	K35
      0x5C441800,  //  0094  MOVE	R17	R12
      0x88480114,  //  0095  GETMBR	R18	R0	K20
      0x7C3C0600,  //  0096  CALL	R15	3
      0x8C3C0515,  //  0097  GETMET	R15	R2	K21
      0x58440024,  //  0098  LDCONST	R17	K36
      0x7C3C0400,  //  0099  CALL	R15	2
      0x8C3C0515,  //  009A  GETMET	R15	R2	K21
      0x60440018,  //  009B  GETGBL	R17	G24
      0x58480025,  //  009C  LDCONST	R18	K37
      0x5C4C1400,  //  009D  MOVE	R19	R10
      0x5C501400,  //  009E  MOVE	R20	R10
      0x8C540517,  //  009F  GETMET	R21	R2	K23
      0x5C5C1C00,  //  00A0  MOVE	R23	R14
      0x7C540400,  //  00A1  CALL	R21	2
      0x78360003,  //  00A2  JMPF	R13	#00A7
      0x8C580517,  //  00A3  GETMET	R22	R2	K23
      0x88601B26,  //  00A4  GETMBR	R24	R13	K38
      0x7C580400,  //  00A5  CALL	R22	2
      0x70020000,  //  00A6  JMP		#00A8
      0x58580002,  //  00A7  LDCONST	R22	K2
      0x7C440A00,  //  00A8  CALL	R17	5
      0x7C3C0400,  //  00A9  CALL	R15	2
      0x8C3C0515,  //  00AA  GETMET	R15	R2	K21
      0x58440027,  //  00AB  LDCONST	R17	K39
      0x7C3C0400,  //  00AC  CALL	R15	2
      0x00281528,  //  00AD  ADD	R10	R10	K40
      0x7001FFBB,  //  00AE  JMP		#006B
      0x8C2C0515,  //  00AF  GETMET	R11	R2	K21
      0x60340018,  //  00B0  GETGBL	R13	G24
      0x58380021,  //  00B1  LDCONST	R14	K33
      0x5C3C1400,  //  00B2  MOVE	R15	R10
      0x7C340400,  //  00B3  CALL	R13	2
      0x7C2C0400,  //  00B4  CALL	R11	2
      0x8C2C0515,  //  00B5  GETMET	R11	R2	K21
      0x60340018,  //  00B6  GETGBL	R13	G24
      0x58380022,  //  00B7  LDCONST	R14	K34
      0x5C3C1400,  //  00B8  MOVE	R15	R10
      0x5C401400,  //  00B9  MOVE	R16	R10
      0x7C340600,  //  00BA  CALL	R13	3
      0x7C2C0400,  //  00BB  CALL	R11	2
      0x8C2C0123,  //  00BC  GETMET	R11	R0	K35
      0x58340002,  //  00BD  LDCONST	R13	K2
      0x88380114,  //  00BE  GETMBR	R14	R0	K20
      0x7C2C0600,  //  00BF  CALL	R11	3
      0x8C2C0515,  //  00C0  GETMET	R11	R2	K21
      0x58340024,  //  00C1  LDCONST	R13	K36
      0x7C2C0400,  //  00C2  CALL	R11	2
      0x8C2C0515,  //  00C3  GETMET	R11	R2	K21
      0x60340018,  //  00C4  GETGBL	R13	G24
      0x58380029,  //  00C5  LDCONST	R14	K41
      0x5C3C1400,  //  00C6  MOVE	R15	R10
      0x5C401400,  //  00C7  MOVE	R16	R10
      0x58440002,  //  00C8  LDCONST	R17	K2
      0x7C340800,  //  00C9  CALL	R13	4
      0x7C2C0400,  //  00CA  CALL	R11	2
      0x8C2C0515,  //  00CB  GETMET	R11	R2	K21
      0x58340027,  //  00CC  LDCONST	R13	K39
      0x7C2C0400,  //  00CD  CALL	R11	2
      0x8C2C0515,  //  00CE  GETMET	R11	R2	K21
      0x5834002A,  //  00CF  LDCONST	R13	K42
      0x7C2C0400,  //  00D0  CALL	R11	2
      0x8C2C0515,  //  00D1  GETMET	R11	R2	K21
      0x5834002B,  //  00D2  LDCONST	R13	K43
      0x7C2C0400,  //  00D3  CALL	R11	2
      0x8C2C0515,  //  00D4  GETMET	R11	R2	K21
      0x5834002C,  //  00D5  LDCONST	R13	K44
      0x7C2C0400,  //  00D6  CALL	R11	2
      0x8C2C0515,  //  00D7  GETMET	R11	R2	K21
      0x5834002D,  //  00D8  LDCONST	R13	K45
      0x7C2C0400,  //  00D9  CALL	R11	2
      0x70020007,  //  00DA  JMP		#00E3
      0x8C200515,  //  00DB  GETMET	R8	R2	K21
      0x60280018,  //  00DC  GETGBL	R10	G24
      0x582C002E,  //  00DD  LDCONST	R11	K46
      0x8C300517,  //  00DE  GETMET	R12	R2	K23
      0x5C380200,  //  00DF  MOVE	R14	R1
      0x7C300400,  //  00E0  CALL	R12	2
      0x7C280400,  //  00E1  CALL	R10	2
      0x7C200400,  //  00E2  CALL	R8	2
      0x80000000,  //  00E3  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_fabric_info
********************************************************************/
be_local_closure(Matter_UI_show_fabric_info,   /* name */
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
    /* K25  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27del_X27_X20class_X3D_X27button_X20bgrn_X27_X3EDelete_X20Fabric_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E),
    /* K26  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K27  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(show_fabric_info),
    &be_const_str_solidified,
    ( &(const binstruction[100]) {  /* code */
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
      0x70020050,  //  000E  JMP		#0060
      0x50080200,  //  000F  LDBOOL	R2	1	0
      0x600C0010,  //  0010  GETGBL	R3	G16
      0x88100103,  //  0011  GETMBR	R4	R0	K3
      0x88100904,  //  0012  GETMBR	R4	R4	K4
      0x88100907,  //  0013  GETMBR	R4	R4	K7
      0x8C100908,  //  0014  GETMET	R4	R4	K8
      0x7C100200,  //  0015  CALL	R4	1
      0x7C0C0200,  //  0016  CALL	R3	1
      0xA8020044,  //  0017  EXBLK	0	#005D
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
      0x8C200301,  //  0059  GETMET	R8	R1	K1
      0x5828001A,  //  005A  LDCONST	R10	K26
      0x7C200400,  //  005B  CALL	R8	2
      0x7001FFBA,  //  005C  JMP		#0018
      0x580C001B,  //  005D  LDCONST	R3	K27
      0xAC0C0200,  //  005E  CATCH	R3	1	0
      0xB0080000,  //  005F  RAISE	2	R0	R0
      0x8C080301,  //  0060  GETMET	R2	R1	K1
      0x5810001A,  //  0061  LDCONST	R4	K26
      0x7C080400,  //  0062  CALL	R2	2
      0x80000000,  //  0063  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_get_arg
********************************************************************/
be_local_closure(Matter_UI_web_get_arg,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(has_arg),
    /* K2   */  be_nested_str_weak(mtc0),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(stop_basic_commissioning),
    /* K5   */  be_nested_str_weak(mtc1),
    /* K6   */  be_nested_str_weak(start_root_basic_commissioning),
    }),
    be_str_weak(web_get_arg),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x88080103,  //  0005  GETMBR	R2	R0	K3
      0x8C080504,  //  0006  GETMET	R2	R2	K4
      0x7C080200,  //  0007  CALL	R2	1
      0x70020006,  //  0008  JMP		#0010
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x58100005,  //  000A  LDCONST	R4	K5
      0x7C080400,  //  000B  CALL	R2	2
      0x780A0002,  //  000C  JMPF	R2	#0010
      0x88080103,  //  000D  GETMBR	R2	R0	K3
      0x8C080506,  //  000E  GETMET	R2	R2	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: plugin_option
********************************************************************/
be_local_closure(Matter_UI_plugin_option,   /* name */
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
be_local_closure(Matter_UI_web_add_config_button,   /* name */
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
be_local_closure(Matter_UI_init,   /* name */
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
be_local_closure(Matter_UI_show_bridge_status,   /* name */
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
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(device),
    /* K3   */  be_nested_str_weak(plugins),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(Plugin_Bridge_HTTP),
    /* K6   */  be_nested_str_weak(http_remote),
    /* K7   */  be_nested_str_weak(addr),
    /* K8   */  be_nested_str_weak(contains),
    /* K9   */  be_nested_str_weak(push),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(content_send),
    /* K12  */  be_nested_str_weak(_X3Chr_X3E),
    /* K13  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
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
    ( &(const binstruction[116]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140102,  //  0004  GETMBR	R5	R0	K2
      0x88140B03,  //  0005  GETMBR	R5	R5	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x14100604,  //  0007  LT	R4	R3	R4
      0x7812001D,  //  0008  JMPF	R4	#0027
      0x88100102,  //  0009  GETMBR	R4	R0	K2
      0x88100903,  //  000A  GETMBR	R4	R4	K3
      0x94100803,  //  000B  GETIDX	R4	R4	R3
      0x6014000F,  //  000C  GETGBL	R5	G15
      0x5C180800,  //  000D  MOVE	R6	R4
      0xB81E0800,  //  000E  GETNGBL	R7	K4
      0x881C0F05,  //  000F  GETMBR	R7	R7	K5
      0x7C140400,  //  0010  CALL	R5	2
      0x78160012,  //  0011  JMPF	R5	#0025
      0x4C140000,  //  0012  LDNIL	R5
      0x1C140405,  //  0013  EQ	R5	R2	R5
      0x78160002,  //  0014  JMPF	R5	#0018
      0x60140013,  //  0015  GETGBL	R5	G19
      0x7C140000,  //  0016  CALL	R5	0
      0x5C080A00,  //  0017  MOVE	R2	R5
      0x88140906,  //  0018  GETMBR	R5	R4	K6
      0x88140B07,  //  0019  GETMBR	R5	R5	K7
      0x8C180508,  //  001A  GETMET	R6	R2	K8
      0x5C200A00,  //  001B  MOVE	R8	R5
      0x7C180400,  //  001C  CALL	R6	2
      0x741A0002,  //  001D  JMPT	R6	#0021
      0x60180012,  //  001E  GETGBL	R6	G18
      0x7C180000,  //  001F  CALL	R6	0
      0x98080A06,  //  0020  SETIDX	R2	R5	R6
      0x94180405,  //  0021  GETIDX	R6	R2	R5
      0x8C180D09,  //  0022  GETMET	R6	R6	K9
      0x5C200800,  //  0023  MOVE	R8	R4
      0x7C180400,  //  0024  CALL	R6	2
      0x000C070A,  //  0025  ADD	R3	R3	K10
      0x7001FFDB,  //  0026  JMP		#0003
      0x4C100000,  //  0027  LDNIL	R4
      0x1C100404,  //  0028  EQ	R4	R2	R4
      0x78120000,  //  0029  JMPF	R4	#002B
      0x80000800,  //  002A  RET	0
      0x8C10030B,  //  002B  GETMET	R4	R1	K11
      0x5818000C,  //  002C  LDCONST	R6	K12
      0x7C100400,  //  002D  CALL	R4	2
      0x8C10030B,  //  002E  GETMET	R4	R1	K11
      0x5818000D,  //  002F  LDCONST	R6	K13
      0x7C100400,  //  0030  CALL	R4	2
      0x8C10030B,  //  0031  GETMET	R4	R1	K11
      0xB81A0800,  //  0032  GETNGBL	R6	K4
      0x88180D0E,  //  0033  GETMBR	R6	R6	K14
      0x7C100400,  //  0034  CALL	R4	2
      0x60100010,  //  0035  GETGBL	R4	G16
      0x88140102,  //  0036  GETMBR	R5	R0	K2
      0x8C140B0F,  //  0037  GETMET	R5	R5	K15
      0x5C1C0400,  //  0038  MOVE	R7	R2
      0x7C140400,  //  0039  CALL	R5	2
      0x7C100200,  //  003A  CALL	R4	1
      0xA8020030,  //  003B  EXBLK	0	#006D
      0x5C140800,  //  003C  MOVE	R5	R4
      0x7C140000,  //  003D  CALL	R5	0
      0x8C180310,  //  003E  GETMET	R6	R1	K16
      0x5C200A00,  //  003F  MOVE	R8	R5
      0x7C180400,  //  0040  CALL	R6	2
      0x8C1C0310,  //  0041  GETMET	R7	R1	K16
      0x88240102,  //  0042  GETMBR	R9	R0	K2
      0x8C241311,  //  0043  GETMET	R9	R9	K17
      0x5C2C0A00,  //  0044  MOVE	R11	R5
      0x7C240400,  //  0045  CALL	R9	2
      0x8C241312,  //  0046  GETMET	R9	R9	K18
      0x582C0013,  //  0047  LDCONST	R11	K19
      0x5C300A00,  //  0048  MOVE	R12	R5
      0x7C240600,  //  0049  CALL	R9	3
      0x7C1C0400,  //  004A  CALL	R7	2
      0x8C20030B,  //  004B  GETMET	R8	R1	K11
      0x60280018,  //  004C  GETGBL	R10	G24
      0x582C0014,  //  004D  LDCONST	R11	K20
      0x5C300C00,  //  004E  MOVE	R12	R6
      0x5C340C00,  //  004F  MOVE	R13	R6
      0x5C380E00,  //  0050  MOVE	R14	R7
      0x7C280800,  //  0051  CALL	R10	4
      0x7C200400,  //  0052  CALL	R8	2
      0x94200405,  //  0053  GETIDX	R8	R2	R5
      0x94201101,  //  0054  GETIDX	R8	R8	K1
      0x88201106,  //  0055  GETMBR	R8	R8	K6
      0x8C24030B,  //  0056  GETMET	R9	R1	K11
      0x8C2C1115,  //  0057  GETMET	R11	R8	K21
      0x7C2C0200,  //  0058  CALL	R11	1
      0x7C240400,  //  0059  CALL	R9	2
      0x60240010,  //  005A  GETGBL	R9	G16
      0x94280405,  //  005B  GETIDX	R10	R2	R5
      0x7C240200,  //  005C  CALL	R9	1
      0xA802000A,  //  005D  EXBLK	0	#0069
      0x5C281200,  //  005E  MOVE	R10	R9
      0x7C280000,  //  005F  CALL	R10	0
      0x8C2C030B,  //  0060  GETMET	R11	R1	K11
      0x58340016,  //  0061  LDCONST	R13	K22
      0x7C2C0400,  //  0062  CALL	R11	2
      0x8C2C1517,  //  0063  GETMET	R11	R10	K23
      0x7C2C0200,  //  0064  CALL	R11	1
      0x8C2C030B,  //  0065  GETMET	R11	R1	K11
      0x58340018,  //  0066  LDCONST	R13	K24
      0x7C2C0400,  //  0067  CALL	R11	2
      0x7001FFF4,  //  0068  JMP		#005E
      0x58240019,  //  0069  LDCONST	R9	K25
      0xAC240200,  //  006A  CATCH	R9	1	0
      0xB0080000,  //  006B  RAISE	2	R0	R0
      0x7001FFCE,  //  006C  JMP		#003C
      0x58100019,  //  006D  LDCONST	R4	K25
      0xAC100200,  //  006E  CATCH	R4	1	0
      0xB0080000,  //  006F  RAISE	2	R0	R0
      0x8C10030B,  //  0070  GETMET	R4	R1	K11
      0x5818001A,  //  0071  LDCONST	R6	K26
      0x7C100400,  //  0072  CALL	R4	2
      0x80000000,  //  0073  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_add_handler
********************************************************************/
be_local_closure(Matter_UI_web_add_handler,   /* name */
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
be_local_closure(Matter_UI_show_passcode_form,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(content_send),
    /* K2   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Advanced_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K3   */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20cause_X20a_X20restart_X2E_X22_X29_X3B_X27_X3E_X3Cp_X3EPasscode_X3A_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X271_X27_X20max_X3D_X2799999998_X27_X20name_X3D_X27passcode_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K5   */  be_nested_str_weak(device),
    /* K6   */  be_nested_str_weak(root_passcode),
    /* K7   */  be_nested_str_weak(_X3Cp_X3EDistinguish_X20id_X3A_X3C_X2Fp_X3E),
    /* K8   */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X270_X27_X20max_X3D_X274095_X27_X20name_X3D_X27discriminator_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K9   */  be_nested_str_weak(root_discriminator),
    /* K10  */  be_nested_str_weak(ipv4only),
    /* K11  */  be_nested_str_weak(_X20checked),
    /* K12  */  be_nested_str_weak(),
    /* K13  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20type_X3D_X27checkbox_X27_X20name_X3D_X27ipv4_X27_X25s_X3EIPv4_X20only_X3C_X2Fp_X3E),
    /* K14  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27passcode_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_passcode_form),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080301,  //  0004  GETMET	R2	R1	K1
      0x58100003,  //  0005  LDCONST	R4	K3
      0x7C080400,  //  0006  CALL	R2	2
      0x8C080301,  //  0007  GETMET	R2	R1	K1
      0x60100018,  //  0008  GETGBL	R4	G24
      0x58140004,  //  0009  LDCONST	R5	K4
      0x88180105,  //  000A  GETMBR	R6	R0	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x7C100400,  //  000C  CALL	R4	2
      0x7C080400,  //  000D  CALL	R2	2
      0x8C080301,  //  000E  GETMET	R2	R1	K1
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x8C080301,  //  0011  GETMET	R2	R1	K1
      0x60100018,  //  0012  GETGBL	R4	G24
      0x58140008,  //  0013  LDCONST	R5	K8
      0x88180105,  //  0014  GETMBR	R6	R0	K5
      0x88180D09,  //  0015  GETMBR	R6	R6	K9
      0x7C100400,  //  0016  CALL	R4	2
      0x7C080400,  //  0017  CALL	R2	2
      0x88080105,  //  0018  GETMBR	R2	R0	K5
      0x8808050A,  //  0019  GETMBR	R2	R2	K10
      0x780A0001,  //  001A  JMPF	R2	#001D
      0x5808000B,  //  001B  LDCONST	R2	K11
      0x70020000,  //  001C  JMP		#001E
      0x5808000C,  //  001D  LDCONST	R2	K12
      0x8C0C0301,  //  001E  GETMET	R3	R1	K1
      0x60140018,  //  001F  GETGBL	R5	G24
      0x5818000D,  //  0020  LDCONST	R6	K13
      0x5C1C0400,  //  0021  MOVE	R7	R2
      0x7C140400,  //  0022  CALL	R5	2
      0x7C0C0400,  //  0023  CALL	R3	2
      0x8C0C0301,  //  0024  GETMET	R3	R1	K1
      0x5814000E,  //  0025  LDCONST	R5	K14
      0x7C0C0400,  //  0026  CALL	R3	2
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_ctl
********************************************************************/
be_local_closure(Matter_UI_page_part_ctl,   /* name */
  be_nested_proto(
    24,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[100]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(check_privileged_access),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(partition_core),
    /* K4   */  be_nested_str_weak(persist),
    /* K5   */  be_nested_str_weak(has_arg),
    /* K6   */  be_nested_str_weak(passcode),
    /* K7   */  be_nested_str_weak(discriminator),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20_X2Fmatterc_X20received_X20_X27_X25s_X27_X20command),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(device),
    /* K13  */  be_nested_str_weak(root_passcode),
    /* K14  */  be_nested_str_weak(arg),
    /* K15  */  be_nested_str_weak(root_discriminator),
    /* K16  */  be_nested_str_weak(ipv4only),
    /* K17  */  be_nested_str_weak(ipv4),
    /* K18  */  be_nested_str_weak(on),
    /* K19  */  be_nested_str_weak(save_param),
    /* K20  */  be_nested_str_weak(redirect),
    /* K21  */  be_nested_str_weak(_X2F_X3Frst_X3D),
    /* K22  */  be_nested_str_weak(save),
    /* K23  */  be_nested_str_weak(menable),
    /* K24  */  be_nested_str_weak(comm),
    /* K25  */  be_nested_str_weak(nobridge),
    /* K26  */  be_nested_str_weak(disable_bridge_mode),
    /* K27  */  be_nested_str_weak(matter_enabled),
    /* K28  */  be_nested_str_weak(enable),
    /* K29  */  be_nested_str_weak(cmd),
    /* K30  */  be_nested_str_weak(SetOption),
    /* K31  */  be_nested_str_weak(matter),
    /* K32  */  be_nested_str_weak(MATTER_OPTION),
    /* K33  */  be_nested_str_weak(_X201),
    /* K34  */  be_nested_str_weak(disable),
    /* K35  */  be_nested_str_weak(_X200),
    /* K36  */  be_nested_str_weak(commissioning_open),
    /* K37  */  be_nested_str_weak(start_root_basic_commissioning),
    /* K38  */  be_nested_str_weak(stop_basic_commissioning),
    /* K39  */  be_nested_str_weak(_X2F),
    /* K40  */  be_nested_str_weak(del_fabric),
    /* K41  */  be_const_int(0),
    /* K42  */  be_nested_str_weak(sessions),
    /* K43  */  be_nested_str_weak(fabrics),
    /* K44  */  be_nested_str_weak(get_fabric_index),
    /* K45  */  be_nested_str_weak(remove_fabric),
    /* K46  */  be_const_int(1),
    /* K47  */  be_nested_str_weak(_X2Fmatterc_X3F),
    /* K48  */  be_nested_str_weak(auto),
    /* K49  */  be_nested_str_weak(plugins_persist),
    /* K50  */  be_nested_str_weak(config),
    /* K51  */  be_nested_str_weak(arg_size),
    /* K52  */  be_nested_str_weak(arg_name),
    /* K53  */  be_nested_str_weak(find),
    /* K54  */  be_const_int(2147483647),
    /* K55  */  be_nested_str_weak(plugins_config),
    /* K56  */  be_nested_str_weak(plugins_classes),
    /* K57  */  be_nested_str_weak(type),
    /* K58  */  be_nested_str_weak(),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20ep_X3D_X25i_X20arg_X3D_X25s),
    /* K60  */  be_nested_str_weak(ui_conf_to_string),
    /* K61  */  be_nested_str_weak(MTR_X3A_X20ep_X3D_X25i_X20prev_arg_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27_X20_X25s),
    /* K62  */  be_nested_str_weak(changed),
    /* K63  */  be_nested_str_weak(ui_string_to_conf),
    /* K64  */  be_nested_str_weak(find_plugin_by_endpoint),
    /* K65  */  be_nested_str_weak(MTR_X3A_X20apply_X20conf_X20_X27_X25s_X27_X20_X28_X25i_X29_X20to_X20_X25s),
    /* K66  */  be_nested_str_weak(parse_configuration),
    /* K67  */  be_nested_str_weak(MTR_X3A_X20ep_X3D_X25i_X20not_X20found),
    /* K68  */  be_nested_str_weak(nam),
    /* K69  */  be_nested_str_weak(name),
    /* K70  */  be_nested_str_weak(set_name),
    /* K71  */  be_nested_str_weak(remove),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20apply_X20name_X20_X27_X25s_X27_X20_X28_X25i_X29_X20to_X20_X25s),
    /* K73  */  be_nested_str_weak(stop_iteration),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20config_X20_X3D_X20_X25s),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20config_X20error_X20_X3D_X20_X25s),
    /* K76  */  be_nested_str_weak(addep),
    /* K77  */  be_nested_str_weak(pi),
    /* K78  */  be_nested_str_weak(MTR_X3A_X20add_X20endpoint_X20typ_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K79  */  be_nested_str_weak(bridge_add_endpoint),
    /* K80  */  be_nested_str_weak(addrem),
    /* K81  */  be_nested_str_weak(url),
    /* K82  */  be_nested_str_weak(value_error),
    /* K83  */  be_nested_str_weak(url_X20shouldn_X27t_X20be_X20null),
    /* K84  */  be_nested_str_weak(equal_map),
    /* K85  */  be_nested_str_weak(MTR_X3A_X20remote_X20add_X20url_X3D_X27_X25s_X27_X20type_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K86  */  be_nested_str_weak(del),
    /* K87  */  be_nested_str_weak(bridge_remove_endpoint),
    /* K88  */  be_nested_str_weak(content_start),
    /* K89  */  be_nested_str_weak(Parameter_X20error),
    /* K90  */  be_nested_str_weak(content_send_style),
    /* K91  */  be_nested_str_weak(content_send),
    /* K92  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EError_X3A_X3C_X2Fb_X3E_X25s_X3C_X2Fp_X3E),
    /* K93  */  be_nested_str_weak(html_escape),
    /* K94  */  be_nested_str_weak(content_button),
    /* K95  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K96  */  be_nested_str_weak(content_stop),
    /* K97  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K98  */  be_const_int(2),
    /* K99  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
    }),
    be_str_weak(page_part_ctl),
    &be_const_str_solidified,
    ( &(const binstruction[675]) {  /* code */
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
      0xA8020278,  //  000A  EXBLK	0	#0284
      0x8C180305,  //  000B  GETMET	R6	R1	K5
      0x58200006,  //  000C  LDCONST	R8	K6
      0x7C180400,  //  000D  CALL	R6	2
      0x741A0003,  //  000E  JMPT	R6	#0013
      0x8C180305,  //  000F  GETMET	R6	R1	K5
      0x58200007,  //  0010  LDCONST	R8	K7
      0x7C180400,  //  0011  CALL	R6	2
      0x781A002A,  //  0012  JMPF	R6	#003E
      0xB81A1000,  //  0013  GETNGBL	R6	K8
      0x8C180D09,  //  0014  GETMET	R6	R6	K9
      0x60200018,  //  0015  GETGBL	R8	G24
      0x5824000A,  //  0016  LDCONST	R9	K10
      0x58280006,  //  0017  LDCONST	R10	K6
      0x7C200400,  //  0018  CALL	R8	2
      0x5824000B,  //  0019  LDCONST	R9	K11
      0x7C180600,  //  001A  CALL	R6	3
      0x8C180305,  //  001B  GETMET	R6	R1	K5
      0x58200006,  //  001C  LDCONST	R8	K6
      0x7C180400,  //  001D  CALL	R6	2
      0x781A0006,  //  001E  JMPF	R6	#0026
      0x8818010C,  //  001F  GETMBR	R6	R0	K12
      0x601C0009,  //  0020  GETGBL	R7	G9
      0x8C20030E,  //  0021  GETMET	R8	R1	K14
      0x58280006,  //  0022  LDCONST	R10	K6
      0x7C200400,  //  0023  CALL	R8	2
      0x7C1C0200,  //  0024  CALL	R7	1
      0x901A1A07,  //  0025  SETMBR	R6	K13	R7
      0x8C180305,  //  0026  GETMET	R6	R1	K5
      0x58200007,  //  0027  LDCONST	R8	K7
      0x7C180400,  //  0028  CALL	R6	2
      0x781A0006,  //  0029  JMPF	R6	#0031
      0x8818010C,  //  002A  GETMBR	R6	R0	K12
      0x601C0009,  //  002B  GETGBL	R7	G9
      0x8C20030E,  //  002C  GETMET	R8	R1	K14
      0x58280007,  //  002D  LDCONST	R10	K7
      0x7C200400,  //  002E  CALL	R8	2
      0x7C1C0200,  //  002F  CALL	R7	1
      0x901A1E07,  //  0030  SETMBR	R6	K15	R7
      0x8818010C,  //  0031  GETMBR	R6	R0	K12
      0x8C1C030E,  //  0032  GETMET	R7	R1	K14
      0x58240011,  //  0033  LDCONST	R9	K17
      0x7C1C0400,  //  0034  CALL	R7	2
      0x1C1C0F12,  //  0035  EQ	R7	R7	K18
      0x901A2007,  //  0036  SETMBR	R6	K16	R7
      0x8818010C,  //  0037  GETMBR	R6	R0	K12
      0x8C180D13,  //  0038  GETMET	R6	R6	K19
      0x7C180200,  //  0039  CALL	R6	1
      0x8C180314,  //  003A  GETMET	R6	R1	K20
      0x58200015,  //  003B  LDCONST	R8	K21
      0x7C180400,  //  003C  CALL	R6	2
      0x70020230,  //  003D  JMP		#026F
      0x8C180305,  //  003E  GETMET	R6	R1	K5
      0x58200016,  //  003F  LDCONST	R8	K22
      0x7C180400,  //  0040  CALL	R6	2
      0x781A0054,  //  0041  JMPF	R6	#0097
      0x8C180305,  //  0042  GETMET	R6	R1	K5
      0x58200017,  //  0043  LDCONST	R8	K23
      0x7C180400,  //  0044  CALL	R6	2
      0x8C1C0305,  //  0045  GETMET	R7	R1	K5
      0x58240018,  //  0046  LDCONST	R9	K24
      0x7C1C0400,  //  0047  CALL	R7	2
      0x8C20030E,  //  0048  GETMET	R8	R1	K14
      0x58280019,  //  0049  LDCONST	R10	K25
      0x7C200400,  //  004A  CALL	R8	2
      0x1C201112,  //  004B  EQ	R8	R8	K18
      0x8824010C,  //  004C  GETMBR	R9	R0	K12
      0x8824131A,  //  004D  GETMBR	R9	R9	K26
      0x20241208,  //  004E  NE	R9	R9	R8
      0x78260004,  //  004F  JMPF	R9	#0055
      0x8824010C,  //  0050  GETMBR	R9	R0	K12
      0x90263408,  //  0051  SETMBR	R9	K26	R8
      0x8824010C,  //  0052  GETMBR	R9	R0	K12
      0x8C241313,  //  0053  GETMET	R9	R9	K19
      0x7C240200,  //  0054  CALL	R9	1
      0x8C24011B,  //  0055  GETMET	R9	R0	K27
      0x7C240200,  //  0056  CALL	R9	1
      0x20240C09,  //  0057  NE	R9	R6	R9
      0x78260027,  //  0058  JMPF	R9	#0081
      0x781A0011,  //  0059  JMPF	R6	#006C
      0xB8261000,  //  005A  GETNGBL	R9	K8
      0x8C241309,  //  005B  GETMET	R9	R9	K9
      0x602C0018,  //  005C  GETGBL	R11	G24
      0x5830000A,  //  005D  LDCONST	R12	K10
      0x5834001C,  //  005E  LDCONST	R13	K28
      0x7C2C0400,  //  005F  CALL	R11	2
      0x5830000B,  //  0060  LDCONST	R12	K11
      0x7C240600,  //  0061  CALL	R9	3
      0xB8261000,  //  0062  GETNGBL	R9	K8
      0x8C24131D,  //  0063  GETMET	R9	R9	K29
      0x602C0008,  //  0064  GETGBL	R11	G8
      0xB8323E00,  //  0065  GETNGBL	R12	K31
      0x88301920,  //  0066  GETMBR	R12	R12	K32
      0x7C2C0200,  //  0067  CALL	R11	1
      0x002E3C0B,  //  0068  ADD	R11	K30	R11
      0x002C1721,  //  0069  ADD	R11	R11	K33
      0x7C240400,  //  006A  CALL	R9	2
      0x70020010,  //  006B  JMP		#007D
      0xB8261000,  //  006C  GETNGBL	R9	K8
      0x8C241309,  //  006D  GETMET	R9	R9	K9
      0x602C0018,  //  006E  GETGBL	R11	G24
      0x5830000A,  //  006F  LDCONST	R12	K10
      0x58340022,  //  0070  LDCONST	R13	K34
      0x7C2C0400,  //  0071  CALL	R11	2
      0x5830000B,  //  0072  LDCONST	R12	K11
      0x7C240600,  //  0073  CALL	R9	3
      0xB8261000,  //  0074  GETNGBL	R9	K8
      0x8C24131D,  //  0075  GETMET	R9	R9	K29
      0x602C0008,  //  0076  GETGBL	R11	G8
      0xB8323E00,  //  0077  GETNGBL	R12	K31
      0x88301920,  //  0078  GETMBR	R12	R12	K32
      0x7C2C0200,  //  0079  CALL	R11	1
      0x002E3C0B,  //  007A  ADD	R11	K30	R11
      0x002C1723,  //  007B  ADD	R11	R11	K35
      0x7C240400,  //  007C  CALL	R9	2
      0x8C240314,  //  007D  GETMET	R9	R1	K20
      0x582C0015,  //  007E  LDCONST	R11	K21
      0x7C240400,  //  007F  CALL	R9	2
      0x70020014,  //  0080  JMP		#0096
      0x8824010C,  //  0081  GETMBR	R9	R0	K12
      0x88241324,  //  0082  GETMBR	R9	R9	K36
      0x4C280000,  //  0083  LDNIL	R10
      0x2024120A,  //  0084  NE	R9	R9	R10
      0x20240E09,  //  0085  NE	R9	R7	R9
      0x7826000B,  //  0086  JMPF	R9	#0093
      0x781E0003,  //  0087  JMPF	R7	#008C
      0x8824010C,  //  0088  GETMBR	R9	R0	K12
      0x8C241325,  //  0089  GETMET	R9	R9	K37
      0x7C240200,  //  008A  CALL	R9	1
      0x70020002,  //  008B  JMP		#008F
      0x8824010C,  //  008C  GETMBR	R9	R0	K12
      0x8C241326,  //  008D  GETMET	R9	R9	K38
      0x7C240200,  //  008E  CALL	R9	1
      0x8C240314,  //  008F  GETMET	R9	R1	K20
      0x582C0027,  //  0090  LDCONST	R11	K39
      0x7C240400,  //  0091  CALL	R9	2
      0x70020002,  //  0092  JMP		#0096
      0x8C240314,  //  0093  GETMET	R9	R1	K20
      0x582C0027,  //  0094  LDCONST	R11	K39
      0x7C240400,  //  0095  CALL	R9	2
      0x700201D7,  //  0096  JMP		#026F
      0x8C180305,  //  0097  GETMET	R6	R1	K5
      0x58200028,  //  0098  LDCONST	R8	K40
      0x7C180400,  //  0099  CALL	R6	2
      0x781A0026,  //  009A  JMPF	R6	#00C2
      0xB81A1000,  //  009B  GETNGBL	R6	K8
      0x8C180D09,  //  009C  GETMET	R6	R6	K9
      0x60200018,  //  009D  GETGBL	R8	G24
      0x5824000A,  //  009E  LDCONST	R9	K10
      0x58280028,  //  009F  LDCONST	R10	K40
      0x7C200400,  //  00A0  CALL	R8	2
      0x5824000B,  //  00A1  LDCONST	R9	K11
      0x7C180600,  //  00A2  CALL	R6	3
      0x60180009,  //  00A3  GETGBL	R6	G9
      0x8C1C030E,  //  00A4  GETMET	R7	R1	K14
      0x58240028,  //  00A5  LDCONST	R9	K40
      0x7C1C0400,  //  00A6  CALL	R7	2
      0x7C180200,  //  00A7  CALL	R6	1
      0x581C0029,  //  00A8  LDCONST	R7	K41
      0x8820010C,  //  00A9  GETMBR	R8	R0	K12
      0x8820112A,  //  00AA  GETMBR	R8	R8	K42
      0x8820112B,  //  00AB  GETMBR	R8	R8	K43
      0x6024000C,  //  00AC  GETGBL	R9	G12
      0x5C281000,  //  00AD  MOVE	R10	R8
      0x7C240200,  //  00AE  CALL	R9	1
      0x14240E09,  //  00AF  LT	R9	R7	R9
      0x7826000C,  //  00B0  JMPF	R9	#00BE
      0x94241007,  //  00B1  GETIDX	R9	R8	R7
      0x8C24132C,  //  00B2  GETMET	R9	R9	K44
      0x7C240200,  //  00B3  CALL	R9	1
      0x1C241206,  //  00B4  EQ	R9	R9	R6
      0x78260005,  //  00B5  JMPF	R9	#00BC
      0x8824010C,  //  00B6  GETMBR	R9	R0	K12
      0x8C24132D,  //  00B7  GETMET	R9	R9	K45
      0x942C1007,  //  00B8  GETIDX	R11	R8	R7
      0x7C240400,  //  00B9  CALL	R9	2
      0x70020002,  //  00BA  JMP		#00BE
      0x70020000,  //  00BB  JMP		#00BD
      0x001C0F2E,  //  00BC  ADD	R7	R7	K46
      0x7001FFED,  //  00BD  JMP		#00AC
      0x8C240314,  //  00BE  GETMET	R9	R1	K20
      0x582C002F,  //  00BF  LDCONST	R11	K47
      0x7C240400,  //  00C0  CALL	R9	2
      0x700201AC,  //  00C1  JMP		#026F
      0x8C180305,  //  00C2  GETMET	R6	R1	K5
      0x58200030,  //  00C3  LDCONST	R8	K48
      0x7C180400,  //  00C4  CALL	R6	2
      0x781A0011,  //  00C5  JMPF	R6	#00D8
      0xB81A1000,  //  00C6  GETNGBL	R6	K8
      0x8C180D09,  //  00C7  GETMET	R6	R6	K9
      0x60200018,  //  00C8  GETGBL	R8	G24
      0x5824000A,  //  00C9  LDCONST	R9	K10
      0x58280030,  //  00CA  LDCONST	R10	K48
      0x7C200400,  //  00CB  CALL	R8	2
      0x5824000B,  //  00CC  LDCONST	R9	K11
      0x7C180600,  //  00CD  CALL	R6	3
      0x8818010C,  //  00CE  GETMBR	R6	R0	K12
      0x501C0000,  //  00CF  LDBOOL	R7	0	0
      0x901A6207,  //  00D0  SETMBR	R6	K49	R7
      0x8818010C,  //  00D1  GETMBR	R6	R0	K12
      0x8C180D13,  //  00D2  GETMET	R6	R6	K19
      0x7C180200,  //  00D3  CALL	R6	1
      0x8C180314,  //  00D4  GETMET	R6	R1	K20
      0x58200015,  //  00D5  LDCONST	R8	K21
      0x7C180400,  //  00D6  CALL	R6	2
      0x70020196,  //  00D7  JMP		#026F
      0x8C180305,  //  00D8  GETMET	R6	R1	K5
      0x58200032,  //  00D9  LDCONST	R8	K50
      0x7C180400,  //  00DA  CALL	R6	2
      0x781A00D6,  //  00DB  JMPF	R6	#01B3
      0xB81A1000,  //  00DC  GETNGBL	R6	K8
      0x8C180D09,  //  00DD  GETMET	R6	R6	K9
      0x60200018,  //  00DE  GETGBL	R8	G24
      0x5824000A,  //  00DF  LDCONST	R9	K10
      0x58280032,  //  00E0  LDCONST	R10	K50
      0x7C200400,  //  00E1  CALL	R8	2
      0x5824000B,  //  00E2  LDCONST	R9	K11
      0x7C180600,  //  00E3  CALL	R6	3
      0x50180000,  //  00E4  LDBOOL	R6	0	0
      0x601C0010,  //  00E5  GETGBL	R7	G16
      0x8C200333,  //  00E6  GETMET	R8	R1	K51
      0x7C200200,  //  00E7  CALL	R8	1
      0x0420112E,  //  00E8  SUB	R8	R8	K46
      0x40225208,  //  00E9  CONNECT	R8	K41	R8
      0x7C1C0200,  //  00EA  CALL	R7	1
      0xA80200A0,  //  00EB  EXBLK	0	#018D
      0x5C200E00,  //  00EC  MOVE	R8	R7
      0x7C200000,  //  00ED  CALL	R8	0
      0x8C240334,  //  00EE  GETMET	R9	R1	K52
      0x5C2C1000,  //  00EF  MOVE	R11	R8
      0x7C240400,  //  00F0  CALL	R9	2
      0x8C280535,  //  00F1  GETMET	R10	R2	K53
      0x5C301200,  //  00F2  MOVE	R12	R9
      0x5834000E,  //  00F3  LDCONST	R13	K14
      0x7C280600,  //  00F4  CALL	R10	3
      0x1C281529,  //  00F5  EQ	R10	R10	K41
      0x782A005B,  //  00F6  JMPF	R10	#0153
      0x60280009,  //  00F7  GETGBL	R10	G9
      0x402E1736,  //  00F8  CONNECT	R11	K11	K54
      0x942C120B,  //  00F9  GETIDX	R11	R9	R11
      0x7C280200,  //  00FA  CALL	R10	1
      0x8C2C030E,  //  00FB  GETMET	R11	R1	K14
      0x5C341000,  //  00FC  MOVE	R13	R8
      0x7C2C0400,  //  00FD  CALL	R11	2
      0x8830010C,  //  00FE  GETMBR	R12	R0	K12
      0x88301937,  //  00FF  GETMBR	R12	R12	K55
      0x8C301935,  //  0100  GETMET	R12	R12	K53
      0x60380008,  //  0101  GETGBL	R14	G8
      0x5C3C1400,  //  0102  MOVE	R15	R10
      0x7C380200,  //  0103  CALL	R14	1
      0x7C300400,  //  0104  CALL	R12	2
      0x4C340000,  //  0105  LDNIL	R13
      0x2034180D,  //  0106  NE	R13	R12	R13
      0x78360041,  //  0107  JMPF	R13	#014A
      0x8834010C,  //  0108  GETMBR	R13	R0	K12
      0x88341B38,  //  0109  GETMBR	R13	R13	K56
      0x8C341B35,  //  010A  GETMET	R13	R13	K53
      0x8C3C1935,  //  010B  GETMET	R15	R12	K53
      0x58440039,  //  010C  LDCONST	R17	K57
      0x5848003A,  //  010D  LDCONST	R18	K58
      0x7C3C0600,  //  010E  CALL	R15	3
      0x7C340400,  //  010F  CALL	R13	2
      0x4C380000,  //  0110  LDNIL	R14
      0x20381A0E,  //  0111  NE	R14	R13	R14
      0x783A0035,  //  0112  JMPF	R14	#0149
      0xB83A1000,  //  0113  GETNGBL	R14	K8
      0x8C381D09,  //  0114  GETMET	R14	R14	K9
      0x60400018,  //  0115  GETGBL	R16	G24
      0x5844003B,  //  0116  LDCONST	R17	K59
      0x5C481400,  //  0117  MOVE	R18	R10
      0x5C4C1600,  //  0118  MOVE	R19	R11
      0x7C400600,  //  0119  CALL	R16	3
      0x5844000B,  //  011A  LDCONST	R17	K11
      0x7C380600,  //  011B  CALL	R14	3
      0x8C381B3C,  //  011C  GETMET	R14	R13	K60
      0x5C401A00,  //  011D  MOVE	R16	R13
      0x5C441800,  //  011E  MOVE	R17	R12
      0x7C380600,  //  011F  CALL	R14	3
      0x203C1C0B,  //  0120  NE	R15	R14	R11
      0xB8421000,  //  0121  GETNGBL	R16	K8
      0x8C402109,  //  0122  GETMET	R16	R16	K9
      0x60480018,  //  0123  GETGBL	R18	G24
      0x584C003D,  //  0124  LDCONST	R19	K61
      0x5C501400,  //  0125  MOVE	R20	R10
      0x5C541C00,  //  0126  MOVE	R21	R14
      0x5C581600,  //  0127  MOVE	R22	R11
      0x205C1C0B,  //  0128  NE	R23	R14	R11
      0x785E0001,  //  0129  JMPF	R23	#012C
      0x585C003E,  //  012A  LDCONST	R23	K62
      0x70020000,  //  012B  JMP		#012D
      0x585C003A,  //  012C  LDCONST	R23	K58
      0x7C480A00,  //  012D  CALL	R18	5
      0x584C000B,  //  012E  LDCONST	R19	K11
      0x7C400600,  //  012F  CALL	R16	3
      0x783E0017,  //  0130  JMPF	R15	#0149
      0x50180200,  //  0131  LDBOOL	R6	1	0
      0x8C401B3F,  //  0132  GETMET	R16	R13	K63
      0x5C481A00,  //  0133  MOVE	R18	R13
      0x5C4C1800,  //  0134  MOVE	R19	R12
      0x5C501600,  //  0135  MOVE	R20	R11
      0x7C400800,  //  0136  CALL	R16	4
      0x8840010C,  //  0137  GETMBR	R16	R0	K12
      0x8C402140,  //  0138  GETMET	R16	R16	K64
      0x5C481400,  //  0139  MOVE	R18	R10
      0x7C400400,  //  013A  CALL	R16	2
      0x7842000C,  //  013B  JMPF	R16	#0149
      0xB8461000,  //  013C  GETNGBL	R17	K8
      0x8C442309,  //  013D  GETMET	R17	R17	K9
      0x604C0018,  //  013E  GETGBL	R19	G24
      0x58500041,  //  013F  LDCONST	R20	K65
      0x5C541800,  //  0140  MOVE	R21	R12
      0x5C581400,  //  0141  MOVE	R22	R10
      0x5C5C2000,  //  0142  MOVE	R23	R16
      0x7C4C0800,  //  0143  CALL	R19	4
      0x5850000B,  //  0144  LDCONST	R20	K11
      0x7C440600,  //  0145  CALL	R17	3
      0x8C442142,  //  0146  GETMET	R17	R16	K66
      0x5C4C1800,  //  0147  MOVE	R19	R12
      0x7C440400,  //  0148  CALL	R17	2
      0x70020007,  //  0149  JMP		#0152
      0xB8361000,  //  014A  GETNGBL	R13	K8
      0x8C341B09,  //  014B  GETMET	R13	R13	K9
      0x603C0018,  //  014C  GETGBL	R15	G24
      0x58400043,  //  014D  LDCONST	R16	K67
      0x5C441400,  //  014E  MOVE	R17	R10
      0x7C3C0400,  //  014F  CALL	R15	2
      0x5840000B,  //  0150  LDCONST	R16	K11
      0x7C340600,  //  0151  CALL	R13	3
      0x70020038,  //  0152  JMP		#018C
      0x8C280535,  //  0153  GETMET	R10	R2	K53
      0x5C301200,  //  0154  MOVE	R12	R9
      0x58340044,  //  0155  LDCONST	R13	K68
      0x7C280600,  //  0156  CALL	R10	3
      0x1C281529,  //  0157  EQ	R10	R10	K41
      0x782A0032,  //  0158  JMPF	R10	#018C
      0x60280009,  //  0159  GETGBL	R10	G9
      0x402E1736,  //  015A  CONNECT	R11	K11	K54
      0x942C120B,  //  015B  GETIDX	R11	R9	R11
      0x7C280200,  //  015C  CALL	R10	1
      0x8C2C030E,  //  015D  GETMET	R11	R1	K14
      0x5C341000,  //  015E  MOVE	R13	R8
      0x7C2C0400,  //  015F  CALL	R11	2
      0x8830010C,  //  0160  GETMBR	R12	R0	K12
      0x88301937,  //  0161  GETMBR	R12	R12	K55
      0x8C301935,  //  0162  GETMET	R12	R12	K53
      0x60380008,  //  0163  GETGBL	R14	G8
      0x5C3C1400,  //  0164  MOVE	R15	R10
      0x7C380200,  //  0165  CALL	R14	1
      0x7C300400,  //  0166  CALL	R12	2
      0x4C340000,  //  0167  LDNIL	R13
      0x2034180D,  //  0168  NE	R13	R12	R13
      0x78360021,  //  0169  JMPF	R13	#018C
      0x8C341935,  //  016A  GETMET	R13	R12	K53
      0x583C0045,  //  016B  LDCONST	R15	K69
      0x5840003A,  //  016C  LDCONST	R16	K58
      0x7C340600,  //  016D  CALL	R13	3
      0x20381A0B,  //  016E  NE	R14	R13	R11
      0x783A001B,  //  016F  JMPF	R14	#018C
      0x50180200,  //  0170  LDBOOL	R6	1	0
      0x883C010C,  //  0171  GETMBR	R15	R0	K12
      0x8C3C1F40,  //  0172  GETMET	R15	R15	K64
      0x5C441400,  //  0173  MOVE	R17	R10
      0x7C3C0400,  //  0174  CALL	R15	2
      0x783E0015,  //  0175  JMPF	R15	#018C
      0x8C401F46,  //  0176  GETMET	R16	R15	K70
      0x5C481600,  //  0177  MOVE	R18	R11
      0x7C400400,  //  0178  CALL	R16	2
      0x782E0001,  //  0179  JMPF	R11	#017C
      0x98328A0B,  //  017A  SETIDX	R12	K69	R11
      0x70020002,  //  017B  JMP		#017F
      0x8C401947,  //  017C  GETMET	R16	R12	K71
      0x58480045,  //  017D  LDCONST	R18	K69
      0x7C400400,  //  017E  CALL	R16	2
      0xB8421000,  //  017F  GETNGBL	R16	K8
      0x8C402109,  //  0180  GETMET	R16	R16	K9
      0x60480018,  //  0181  GETGBL	R18	G24
      0x584C0048,  //  0182  LDCONST	R19	K72
      0x5C501800,  //  0183  MOVE	R20	R12
      0x5C541400,  //  0184  MOVE	R21	R10
      0x5C581E00,  //  0185  MOVE	R22	R15
      0x7C480800,  //  0186  CALL	R18	4
      0x584C000B,  //  0187  LDCONST	R19	K11
      0x7C400600,  //  0188  CALL	R16	3
      0x8C401F42,  //  0189  GETMET	R16	R15	K66
      0x5C481800,  //  018A  MOVE	R18	R12
      0x7C400400,  //  018B  CALL	R16	2
      0x7001FF5E,  //  018C  JMP		#00EC
      0x581C0049,  //  018D  LDCONST	R7	K73
      0xAC1C0200,  //  018E  CATCH	R7	1	0
      0xB0080000,  //  018F  RAISE	2	R0	R0
      0xB81E1000,  //  0190  GETNGBL	R7	K8
      0x8C1C0F09,  //  0191  GETMET	R7	R7	K9
      0x60240018,  //  0192  GETGBL	R9	G24
      0x5828004A,  //  0193  LDCONST	R10	K74
      0x602C0008,  //  0194  GETGBL	R11	G8
      0x8830010C,  //  0195  GETMBR	R12	R0	K12
      0x88301937,  //  0196  GETMBR	R12	R12	K55
      0x7C2C0200,  //  0197  CALL	R11	1
      0x7C240400,  //  0198  CALL	R9	2
      0x5828000B,  //  0199  LDCONST	R10	K11
      0x7C1C0600,  //  019A  CALL	R7	3
      0x78160008,  //  019B  JMPF	R5	#01A5
      0xB81E1000,  //  019C  GETNGBL	R7	K8
      0x8C1C0F09,  //  019D  GETMET	R7	R7	K9
      0x60240018,  //  019E  GETGBL	R9	G24
      0x5828004B,  //  019F  LDCONST	R10	K75
      0x5C2C0A00,  //  01A0  MOVE	R11	R5
      0x7C240400,  //  01A1  CALL	R9	2
      0x5828000B,  //  01A2  LDCONST	R10	K11
      0x7C1C0600,  //  01A3  CALL	R7	3
      0x7002000C,  //  01A4  JMP		#01B2
      0x741A0002,  //  01A5  JMPT	R6	#01A9
      0x881C010C,  //  01A6  GETMBR	R7	R0	K12
      0x881C0F31,  //  01A7  GETMBR	R7	R7	K49
      0x741E0005,  //  01A8  JMPT	R7	#01AF
      0x881C010C,  //  01A9  GETMBR	R7	R0	K12
      0x50200200,  //  01AA  LDBOOL	R8	1	0
      0x901E6208,  //  01AB  SETMBR	R7	K49	R8
      0x881C010C,  //  01AC  GETMBR	R7	R0	K12
      0x8C1C0F13,  //  01AD  GETMET	R7	R7	K19
      0x7C1C0200,  //  01AE  CALL	R7	1
      0x8C1C0314,  //  01AF  GETMET	R7	R1	K20
      0x5824002F,  //  01B0  LDCONST	R9	K47
      0x7C1C0400,  //  01B1  CALL	R7	2
      0x700200BB,  //  01B2  JMP		#026F
      0x8C180305,  //  01B3  GETMET	R6	R1	K5
      0x5820004C,  //  01B4  LDCONST	R8	K76
      0x7C180400,  //  01B5  CALL	R6	2
      0x781A002B,  //  01B6  JMPF	R6	#01E3
      0x8C18030E,  //  01B7  GETMET	R6	R1	K14
      0x5820004D,  //  01B8  LDCONST	R8	K77
      0x7C180400,  //  01B9  CALL	R6	2
      0x8C1C030E,  //  01BA  GETMET	R7	R1	K14
      0x5824000E,  //  01BB  LDCONST	R9	K14
      0x7C1C0400,  //  01BC  CALL	R7	2
      0x8C20030E,  //  01BD  GETMET	R8	R1	K14
      0x58280044,  //  01BE  LDCONST	R10	K68
      0x7C200400,  //  01BF  CALL	R8	2
      0xB8261000,  //  01C0  GETNGBL	R9	K8
      0x8C241309,  //  01C1  GETMET	R9	R9	K9
      0x602C0018,  //  01C2  GETGBL	R11	G24
      0x5830004E,  //  01C3  LDCONST	R12	K78
      0x5C340C00,  //  01C4  MOVE	R13	R6
      0x5C380E00,  //  01C5  MOVE	R14	R7
      0x7C2C0600,  //  01C6  CALL	R11	3
      0x5830000B,  //  01C7  LDCONST	R12	K11
      0x7C240600,  //  01C8  CALL	R9	3
      0x8824010C,  //  01C9  GETMBR	R9	R0	K12
      0x88241338,  //  01CA  GETMBR	R9	R9	K56
      0x8C241335,  //  01CB  GETMET	R9	R9	K53
      0x5C2C0C00,  //  01CC  MOVE	R11	R6
      0x7C240400,  //  01CD  CALL	R9	2
      0x4C280000,  //  01CE  LDNIL	R10
      0x2028120A,  //  01CF  NE	R10	R9	R10
      0x782A000D,  //  01D0  JMPF	R10	#01DF
      0x60280013,  //  01D1  GETGBL	R10	G19
      0x7C280000,  //  01D2  CALL	R10	0
      0x78220000,  //  01D3  JMPF	R8	#01D5
      0x982A8A08,  //  01D4  SETIDX	R10	K69	R8
      0x8C2C133F,  //  01D5  GETMET	R11	R9	K63
      0x5C341200,  //  01D6  MOVE	R13	R9
      0x5C381400,  //  01D7  MOVE	R14	R10
      0x5C3C0E00,  //  01D8  MOVE	R15	R7
      0x7C2C0800,  //  01D9  CALL	R11	4
      0x882C010C,  //  01DA  GETMBR	R11	R0	K12
      0x8C2C174F,  //  01DB  GETMET	R11	R11	K79
      0x5C340C00,  //  01DC  MOVE	R13	R6
      0x5C381400,  //  01DD  MOVE	R14	R10
      0x7C2C0600,  //  01DE  CALL	R11	3
      0x8C280314,  //  01DF  GETMET	R10	R1	K20
      0x5830002F,  //  01E0  LDCONST	R12	K47
      0x7C280400,  //  01E1  CALL	R10	2
      0x7002008B,  //  01E2  JMP		#026F
      0x8C180305,  //  01E3  GETMET	R6	R1	K5
      0x58200050,  //  01E4  LDCONST	R8	K80
      0x7C180400,  //  01E5  CALL	R6	2
      0x781A005E,  //  01E6  JMPF	R6	#0246
      0x8C18030E,  //  01E7  GETMET	R6	R1	K14
      0x58200051,  //  01E8  LDCONST	R8	K81
      0x7C180400,  //  01E9  CALL	R6	2
      0x4C1C0000,  //  01EA  LDNIL	R7
      0x1C1C0C07,  //  01EB  EQ	R7	R6	R7
      0x741E0001,  //  01EC  JMPT	R7	#01EF
      0x1C1C0D3A,  //  01ED  EQ	R7	R6	K58
      0x781E0000,  //  01EE  JMPF	R7	#01F0
      0xB006A553,  //  01EF  RAISE	1	K82	K83
      0x581C0029,  //  01F0  LDCONST	R7	K41
      0x60200008,  //  01F1  GETGBL	R8	G8
      0x5C240E00,  //  01F2  MOVE	R9	R7
      0x7C200200,  //  01F3  CALL	R8	1
      0x8C240305,  //  01F4  GETMET	R9	R1	K5
      0x002E9A08,  //  01F5  ADD	R11	K77	R8
      0x7C240400,  //  01F6  CALL	R9	2
      0x78260049,  //  01F7  JMPF	R9	#0242
      0x8C24030E,  //  01F8  GETMET	R9	R1	K14
      0x002E9A08,  //  01F9  ADD	R11	K77	R8
      0x7C240400,  //  01FA  CALL	R9	2
      0x8C28030E,  //  01FB  GETMET	R10	R1	K14
      0x00321C08,  //  01FC  ADD	R12	K14	R8
      0x7C280400,  //  01FD  CALL	R10	2
      0x8C2C030E,  //  01FE  GETMET	R11	R1	K14
      0x00368808,  //  01FF  ADD	R13	K68	R8
      0x7C2C0400,  //  0200  CALL	R11	2
      0x2030133A,  //  0201  NE	R12	R9	K58
      0x78320038,  //  0202  JMPF	R12	#023C
      0x8830010C,  //  0203  GETMBR	R12	R0	K12
      0x88301938,  //  0204  GETMBR	R12	R12	K56
      0x8C301935,  //  0205  GETMET	R12	R12	K53
      0x5C381200,  //  0206  MOVE	R14	R9
      0x7C300400,  //  0207  CALL	R12	2
      0x4C340000,  //  0208  LDNIL	R13
      0x2034180D,  //  0209  NE	R13	R12	R13
      0x78360030,  //  020A  JMPF	R13	#023C
      0x60340013,  //  020B  GETGBL	R13	G19
      0x7C340000,  //  020C  CALL	R13	0
      0x9836A206,  //  020D  SETIDX	R13	K81	R6
      0x98367209,  //  020E  SETIDX	R13	K57	R9
      0x782E0000,  //  020F  JMPF	R11	#0211
      0x98368A0B,  //  0210  SETIDX	R13	K69	R11
      0x8C38193F,  //  0211  GETMET	R14	R12	K63
      0x5C401800,  //  0212  MOVE	R16	R12
      0x5C441A00,  //  0213  MOVE	R17	R13
      0x5C481400,  //  0214  MOVE	R18	R10
      0x7C380800,  //  0215  CALL	R14	4
      0x50380000,  //  0216  LDBOOL	R14	0	0
      0x603C0010,  //  0217  GETGBL	R15	G16
      0x8840010C,  //  0218  GETMBR	R16	R0	K12
      0x88402137,  //  0219  GETMBR	R16	R16	K55
      0x7C3C0200,  //  021A  CALL	R15	1
      0xA802000B,  //  021B  EXBLK	0	#0228
      0x5C401E00,  //  021C  MOVE	R16	R15
      0x7C400000,  //  021D  CALL	R16	0
      0x8C440154,  //  021E  GETMET	R17	R0	K84
      0x5C4C2000,  //  021F  MOVE	R19	R16
      0x5C501A00,  //  0220  MOVE	R20	R13
      0x7C440600,  //  0221  CALL	R17	3
      0x78460001,  //  0222  JMPF	R17	#0225
      0x50380200,  //  0223  LDBOOL	R14	1	0
      0x70020000,  //  0224  JMP		#0226
      0x7001FFF5,  //  0225  JMP		#021C
      0xA8040001,  //  0226  EXBLK	1	1
      0x70020002,  //  0227  JMP		#022B
      0x583C0049,  //  0228  LDCONST	R15	K73
      0xAC3C0200,  //  0229  CATCH	R15	1	0
      0xB0080000,  //  022A  RAISE	2	R0	R0
      0x5C3C1C00,  //  022B  MOVE	R15	R14
      0x743E000E,  //  022C  JMPT	R15	#023C
      0xB83E1000,  //  022D  GETNGBL	R15	K8
      0x8C3C1F09,  //  022E  GETMET	R15	R15	K9
      0x60440018,  //  022F  GETGBL	R17	G24
      0x58480055,  //  0230  LDCONST	R18	K85
      0x5C4C0C00,  //  0231  MOVE	R19	R6
      0x5C501200,  //  0232  MOVE	R20	R9
      0x5C541400,  //  0233  MOVE	R21	R10
      0x7C440800,  //  0234  CALL	R17	4
      0x5848000B,  //  0235  LDCONST	R18	K11
      0x7C3C0600,  //  0236  CALL	R15	3
      0x883C010C,  //  0237  GETMBR	R15	R0	K12
      0x8C3C1F4F,  //  0238  GETMET	R15	R15	K79
      0x5C441200,  //  0239  MOVE	R17	R9
      0x5C481A00,  //  023A  MOVE	R18	R13
      0x7C3C0600,  //  023B  CALL	R15	3
      0x001C0F2E,  //  023C  ADD	R7	R7	K46
      0x60300008,  //  023D  GETGBL	R12	G8
      0x5C340E00,  //  023E  MOVE	R13	R7
      0x7C300200,  //  023F  CALL	R12	1
      0x5C201800,  //  0240  MOVE	R8	R12
      0x7001FFB1,  //  0241  JMP		#01F4
      0x8C240314,  //  0242  GETMET	R9	R1	K20
      0x582C002F,  //  0243  LDCONST	R11	K47
      0x7C240400,  //  0244  CALL	R9	2
      0x70020028,  //  0245  JMP		#026F
      0x4C180000,  //  0246  LDNIL	R6
      0x601C0010,  //  0247  GETGBL	R7	G16
      0x8C200333,  //  0248  GETMET	R8	R1	K51
      0x7C200200,  //  0249  CALL	R8	1
      0x0420112E,  //  024A  SUB	R8	R8	K46
      0x40225208,  //  024B  CONNECT	R8	K41	R8
      0x7C1C0200,  //  024C  CALL	R7	1
      0xA8020013,  //  024D  EXBLK	0	#0262
      0x5C200E00,  //  024E  MOVE	R8	R7
      0x7C200000,  //  024F  CALL	R8	0
      0x8C240334,  //  0250  GETMET	R9	R1	K52
      0x5C2C1000,  //  0251  MOVE	R11	R8
      0x7C240400,  //  0252  CALL	R9	2
      0x8C280535,  //  0253  GETMET	R10	R2	K53
      0x5C301200,  //  0254  MOVE	R12	R9
      0x58340056,  //  0255  LDCONST	R13	K86
      0x7C280600,  //  0256  CALL	R10	3
      0x1C281529,  //  0257  EQ	R10	R10	K41
      0x782A0005,  //  0258  JMPF	R10	#025F
      0x60280009,  //  0259  GETGBL	R10	G9
      0x402E1736,  //  025A  CONNECT	R11	K11	K54
      0x942C120B,  //  025B  GETIDX	R11	R9	R11
      0x7C280200,  //  025C  CALL	R10	1
      0x5C181400,  //  025D  MOVE	R6	R10
      0x70020000,  //  025E  JMP		#0260
      0x7001FFED,  //  025F  JMP		#024E
      0xA8040001,  //  0260  EXBLK	1	1
      0x70020002,  //  0261  JMP		#0265
      0x581C0049,  //  0262  LDCONST	R7	K73
      0xAC1C0200,  //  0263  CATCH	R7	1	0
      0xB0080000,  //  0264  RAISE	2	R0	R0
      0x4C1C0000,  //  0265  LDNIL	R7
      0x201C0C07,  //  0266  NE	R7	R6	R7
      0x781E0006,  //  0267  JMPF	R7	#026F
      0x881C010C,  //  0268  GETMBR	R7	R0	K12
      0x8C1C0F57,  //  0269  GETMET	R7	R7	K87
      0x5C240C00,  //  026A  MOVE	R9	R6
      0x7C1C0400,  //  026B  CALL	R7	2
      0x8C1C0314,  //  026C  GETMET	R7	R1	K20
      0x5824002F,  //  026D  LDCONST	R9	K47
      0x7C1C0400,  //  026E  CALL	R7	2
      0x78160011,  //  026F  JMPF	R5	#0282
      0x8C180358,  //  0270  GETMET	R6	R1	K88
      0x58200059,  //  0271  LDCONST	R8	K89
      0x7C180400,  //  0272  CALL	R6	2
      0x8C18035A,  //  0273  GETMET	R6	R1	K90
      0x7C180200,  //  0274  CALL	R6	1
      0x8C18035B,  //  0275  GETMET	R6	R1	K91
      0x60200018,  //  0276  GETGBL	R8	G24
      0x5824005C,  //  0277  LDCONST	R9	K92
      0x8C28035D,  //  0278  GETMET	R10	R1	K93
      0x5C300A00,  //  0279  MOVE	R12	R5
      0x7C280400,  //  027A  CALL	R10	2
      0x7C200400,  //  027B  CALL	R8	2
      0x7C180400,  //  027C  CALL	R6	2
      0x8C18035E,  //  027D  GETMET	R6	R1	K94
      0x8820035F,  //  027E  GETMBR	R8	R1	K95
      0x7C180400,  //  027F  CALL	R6	2
      0x8C180360,  //  0280  GETMET	R6	R1	K96
      0x7C180200,  //  0281  CALL	R6	1
      0xA8040001,  //  0282  EXBLK	1	1
      0x7002001D,  //  0283  JMP		#02A2
      0xAC180002,  //  0284  CATCH	R6	0	2
      0x7002001A,  //  0285  JMP		#02A1
      0xB8221000,  //  0286  GETNGBL	R8	K8
      0x8C201109,  //  0287  GETMET	R8	R8	K9
      0x60280018,  //  0288  GETGBL	R10	G24
      0x582C0061,  //  0289  LDCONST	R11	K97
      0x5C300C00,  //  028A  MOVE	R12	R6
      0x5C340E00,  //  028B  MOVE	R13	R7
      0x7C280600,  //  028C  CALL	R10	3
      0x582C0062,  //  028D  LDCONST	R11	K98
      0x7C200600,  //  028E  CALL	R8	3
      0x8C200358,  //  028F  GETMET	R8	R1	K88
      0x58280059,  //  0290  LDCONST	R10	K89
      0x7C200400,  //  0291  CALL	R8	2
      0x8C20035A,  //  0292  GETMET	R8	R1	K90
      0x7C200200,  //  0293  CALL	R8	1
      0x8C20035B,  //  0294  GETMET	R8	R1	K91
      0x60280018,  //  0295  GETGBL	R10	G24
      0x582C0063,  //  0296  LDCONST	R11	K99
      0x5C300C00,  //  0297  MOVE	R12	R6
      0x5C340E00,  //  0298  MOVE	R13	R7
      0x7C280600,  //  0299  CALL	R10	3
      0x7C200400,  //  029A  CALL	R8	2
      0x8C20035E,  //  029B  GETMET	R8	R1	K94
      0x8828035F,  //  029C  GETMBR	R10	R1	K95
      0x7C200400,  //  029D  CALL	R8	2
      0x8C200360,  //  029E  GETMET	R8	R1	K96
      0x7C200200,  //  029F  CALL	R8	1
      0x70020000,  //  02A0  JMP		#02A2
      0xB0080000,  //  02A1  RAISE	2	R0	R0
      0x80000000,  //  02A2  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_qrcode
********************************************************************/
be_local_closure(Matter_UI_show_qrcode,   /* name */
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
be_local_closure(Matter_UI_show_enable,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(matter_enabled),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X20style_X3D_X27width_X3A320px_X3B_X27_X3ECheck_X20the_X20_X3Ca_X20href_X3D_X27https_X3A_X2F_X2Ftasmota_X2Egithub_X2Eio_X2Fdocs_X2FMatter_X2F_X27_X20target_X3D_X27_blank_X27_X3EMatter_X20documentation_X3C_X2Fa_X3E_X2E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E),
    /* K4   */  be_nested_str_weak(checked),
    /* K5   */  be_nested_str_weak(),
    /* K6   */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20id_X3D_X27menable_X27_X20type_X3D_X27checkbox_X27_X20name_X3D_X27menable_X27_X20_X25s_X3E),
    /* K7   */  be_nested_str_weak(_X3Clabel_X20for_X3D_X27menable_X27_X3E_X3Cb_X3EMatter_X20enable_X3C_X2Fb_X3E_X3C_X2Flabel_X3E_X3C_X2Fp_X3E),
    /* K8   */  be_nested_str_weak(device),
    /* K9   */  be_nested_str_weak(commissioning_open),
    /* K10  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20id_X3D_X27comm_X27_X20type_X3D_X27checkbox_X27_X20name_X3D_X27comm_X27_X20_X25s_X3E),
    /* K11  */  be_nested_str_weak(_X3Clabel_X20for_X3D_X27comm_X27_X3E_X3Cb_X3ECommissioning_X20open_X3C_X2Fb_X3E_X3C_X2Flabel_X3E_X3C_X2Fp_X3E),
    /* K12  */  be_nested_str_weak(disable_bridge_mode),
    /* K13  */  be_nested_str_weak(_X20checked),
    /* K14  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20type_X3D_X27checkbox_X27_X20name_X3D_X27nobridge_X27_X25s_X3E_X3Cb_X3EForce_X20Static_X20endpoints_X3C_X2Fb_X3E_X20_X28non_X2Dbridge_X29_X3C_X2Fp_X3E),
    /* K15  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27save_X27_X20class_X3D_X27button_X20bgrn_X27_X3ESave_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_enable),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
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
      0x7812001C,  //  0016  JMPF	R4	#0034
      0x88100108,  //  0017  GETMBR	R4	R0	K8
      0x88100909,  //  0018  GETMBR	R4	R4	K9
      0x4C140000,  //  0019  LDNIL	R5
      0x20100805,  //  001A  NE	R4	R4	R5
      0x78120001,  //  001B  JMPF	R4	#001E
      0x58100004,  //  001C  LDCONST	R4	K4
      0x70020000,  //  001D  JMP		#001F
      0x58100005,  //  001E  LDCONST	R4	K5
      0x8C140302,  //  001F  GETMET	R5	R1	K2
      0x601C0018,  //  0020  GETGBL	R7	G24
      0x5820000A,  //  0021  LDCONST	R8	K10
      0x5C240800,  //  0022  MOVE	R9	R4
      0x7C1C0400,  //  0023  CALL	R7	2
      0x7C140400,  //  0024  CALL	R5	2
      0x8C140302,  //  0025  GETMET	R5	R1	K2
      0x581C000B,  //  0026  LDCONST	R7	K11
      0x7C140400,  //  0027  CALL	R5	2
      0x88140108,  //  0028  GETMBR	R5	R0	K8
      0x88140B0C,  //  0029  GETMBR	R5	R5	K12
      0x78160001,  //  002A  JMPF	R5	#002D
      0x5814000D,  //  002B  LDCONST	R5	K13
      0x70020000,  //  002C  JMP		#002E
      0x58140005,  //  002D  LDCONST	R5	K5
      0x8C180302,  //  002E  GETMET	R6	R1	K2
      0x60200018,  //  002F  GETGBL	R8	G24
      0x5824000E,  //  0030  LDCONST	R9	K14
      0x5C280A00,  //  0031  MOVE	R10	R5
      0x7C200400,  //  0032  CALL	R8	2
      0x7C180400,  //  0033  CALL	R6	2
      0x8C100302,  //  0034  GETMET	R4	R1	K2
      0x5818000F,  //  0035  LDCONST	R6	K15
      0x7C100400,  //  0036  CALL	R4	2
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_plugins_hints_js
********************************************************************/
be_local_closure(Matter_UI_show_plugins_hints_js,   /* name */
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
        { be_const_key_weak(equal_map, -1), be_const_static_closure(Matter_UI_equal_map_closure) },
        { be_const_key_weak(page_part_mgr_adv, -1), be_const_closure(Matter_UI_page_part_mgr_adv_closure) },
        { be_const_key_weak(_CLASSES_TYPES2, -1), be_nested_str_weak(_X7Chttp_relay_X7Chttp_light0_X7Chttp_light1_X7Chttp_light2_X7Chttp_light3_X7Chttp_temperature_X7Chttp_pressure_X7Chttp_illuminance_X7Chttp_humidity_X7Chttp_occupancy_X7Chttp_contact) },
        { be_const_key_weak(page_part_mgr, 25), be_const_closure(Matter_UI_page_part_mgr_closure) },
        { be_const_key_weak(show_plugins_hints_js, -1), be_const_closure(Matter_UI_show_plugins_hints_js_closure) },
        { be_const_key_weak(show_enable, -1), be_const_closure(Matter_UI_show_enable_closure) },
        { be_const_key_weak(matter_enabled, 24), be_const_closure(Matter_UI_matter_enabled_closure) },
        { be_const_key_weak(generate_config_from_status, -1), be_const_closure(Matter_UI_generate_config_from_status_closure) },
        { be_const_key_weak(show_qrcode, 21), be_const_closure(Matter_UI_show_qrcode_closure) },
        { be_const_key_weak(page_part_mgr_add, -1), be_const_closure(Matter_UI_page_part_mgr_add_closure) },
        { be_const_key_weak(show_plugins_configuration, -1), be_const_closure(Matter_UI_show_plugins_configuration_closure) },
        { be_const_key_weak(show_commissioning_info, -1), be_const_closure(Matter_UI_show_commissioning_info_closure) },
        { be_const_key_weak(page_part_ctl, 18), be_const_closure(Matter_UI_page_part_ctl_closure) },
        { be_const_key_weak(show_fabric_info, -1), be_const_closure(Matter_UI_show_fabric_info_closure) },
        { be_const_key_weak(_CLASSES_TYPES, 4), be_nested_str_weak(_X7Crelay_X7Clight0_X7Clight1_X7Clight2_X7Clight3_X7Cshutter_X7Cshutter_X2Btilt_X7Ctemperature_X7Cpressure_X7Cilluminance_X7Chumidity_X7Coccupancy_X7Conoff_X7Ccontact_X7C_X2Dvirtual_X7Cv_relay_X7Cv_light0_X7Cv_light1_X7Cv_light2_X7Cv_light3_X7Cv_temp_X7Cv_pressure_X7Cv_illuminance_X7Cv_humidity_X7Cv_contact) },
        { be_const_key_weak(web_get_arg, -1), be_const_closure(Matter_UI_web_get_arg_closure) },
        { be_const_key_weak(plugin_option, 5), be_const_closure(Matter_UI_plugin_option_closure) },
        { be_const_key_weak(web_add_config_button, -1), be_const_closure(Matter_UI_web_add_config_button_closure) },
        { be_const_key_weak(show_passcode_form, -1), be_const_closure(Matter_UI_show_passcode_form_closure) },
        { be_const_key_weak(show_remote_autoconf, 8), be_const_closure(Matter_UI_show_remote_autoconf_closure) },
        { be_const_key_weak(web_add_handler, -1), be_const_closure(Matter_UI_web_add_handler_closure) },
        { be_const_key_weak(show_bridge_status, 23), be_const_closure(Matter_UI_show_bridge_status_closure) },
        { be_const_key_weak(plugin_name, 12), be_const_closure(Matter_UI_plugin_name_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_UI_init_closure) },
        { be_const_key_weak(web_sensor, -1), be_const_closure(Matter_UI_web_sensor_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_UI)
);
/*******************************************************************/

void be_load_Matter_UI_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_UI);
    be_setglobal(vm, "Matter_UI");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
