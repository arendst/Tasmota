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
    27,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[62]) {     /* constants */
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
    /* K36  */  be_nested_str_weak(_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X3E_X25s_X3C_X2Fa_X3E),
    /* K37  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K38  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2778_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K39  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2722_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25i_X3C_X2Fb_X3E_X3C_X2Ftd_X3E),
    /* K40  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X2778_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X25i_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X25s_X27_X20placeholder_X3D_X27_X28optional_X29_X27_X3E_X3C_X2Ftd_X3E),
    /* K41  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E),
    /* K42  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X25i_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X25s_X27_X3E_X3C_X2Ftd_X3E),
    /* K43  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X2715_X27_X20style_X3D_X27text_X2Dalign_X3Acenter_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27del_X25i_X27_X20style_X3D_X27background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3B_X27_X20onclick_X3D_X22return_X20confirm_X28_X27Confirm_X20removing_X20endpoint_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K44  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K45  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27config_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X20configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    /* K46  */  be_nested_str_weak(show_plugins_hints_js),
    /* K47  */  be_nested_str_weak(_CLASSES_TYPES),
    /* K48  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BAdd_X20to_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K49  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3EAdd_X20local_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K50  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X27100_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EName_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EParameter_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K51  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27nam_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X27_X20placeholder_X3D_X27_X28optional_X29_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cselect_X20id_X3D_X27pi_X27_X20name_X3D_X27pi_X27_X20onchange_X3D_X27otm_X28_X22arg_X22_X2Cthis_X2Evalue_X29_X27_X3E),
    /* K52  */  be_nested_str_weak(plugin_option),
    /* K53  */  be_nested_str_weak(_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E),
    /* K54  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20id_X3D_X27arg_X27_X20name_X3D_X27arg_X27_X20size_X3D_X271_X27_X20value_X3D_X27_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E),
    /* K55  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K56  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27addep_X27_X20class_X3D_X27button_X20bgrn_X27_X3ECreate_X20new_X20endpoint_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K57  */  be_nested_str_weak(_X3Chr_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20Tasmota_X20or_X20OpenBK_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatteradd_X27_X20method_X3D_X27get_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K58  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2730_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3Ehttp_X3A_X2F_X2F_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27url_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X27_X20required_X20placeholder_X3D_X27IP_X20or_X20domain_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2710_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X2F_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E),
    /* K59  */  be_nested_str_weak(_X3Cbutton_X20class_X3D_X27button_X20bgrn_X27_X3EAuto_X2Dconfigure_X20remote_X20Tasmota_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Chr_X3E),
    /* K60  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20RESET_X20the_X20configuration_X20to_X20the_X20default_X2E_X20You_X20will_X20need_X20to_X20associate_X20again_X2E_X22_X29_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27auto_X27_X20class_X3D_X27button_X20bred_X27_X3EReset_X20all_X20and_X20Auto_X2Ddiscover_X3C_X2Fbutton_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Fform_X3E),
    /* K61  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    }),
    be_str_weak(show_plugins_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[341]) {  /* code */
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
      0xA8020077,  //  009F  EXBLK	0	#0118
      0x5C241000,  //  00A0  MOVE	R9	R8
      0x7C240000,  //  00A1  CALL	R9	0
      0x8C280316,  //  00A2  GETMET	R10	R1	K22
      0x5C301200,  //  00A3  MOVE	R12	R9
      0x7C280400,  //  00A4  CALL	R10	2
      0x8C2C0303,  //  00A5  GETMET	R11	R1	K3
      0x60340018,  //  00A6  GETGBL	R13	G24
      0x58380024,  //  00A7  LDCONST	R14	K36
      0x5C3C1400,  //  00A8  MOVE	R15	R10
      0x5C401400,  //  00A9  MOVE	R16	R10
      0x7C340600,  //  00AA  CALL	R13	3
      0x7C2C0400,  //  00AB  CALL	R11	2
      0x8C2C0303,  //  00AC  GETMET	R11	R1	K3
      0x58340025,  //  00AD  LDCONST	R13	K37
      0x7C2C0400,  //  00AE  CALL	R11	2
      0x8C2C0303,  //  00AF  GETMET	R11	R1	K3
      0x58340026,  //  00B0  LDCONST	R13	K38
      0x7C2C0400,  //  00B1  CALL	R11	2
      0x50180000,  //  00B2  LDBOOL	R6	0	0
      0x5814000C,  //  00B3  LDCONST	R5	K12
      0x602C000C,  //  00B4  GETGBL	R11	G12
      0x5C300800,  //  00B5  MOVE	R12	R4
      0x7C2C0200,  //  00B6  CALL	R11	1
      0x142C0A0B,  //  00B7  LT	R11	R5	R11
      0x782E005A,  //  00B8  JMPF	R11	#0114
      0x942C0805,  //  00B9  GETIDX	R11	R4	R5
      0x88300107,  //  00BA  GETMBR	R12	R0	K7
      0x88301908,  //  00BB  GETMBR	R12	R12	K8
      0x8C30190D,  //  00BC  GETMET	R12	R12	K13
      0x60380008,  //  00BD  GETGBL	R14	G8
      0x5C3C1600,  //  00BE  MOVE	R15	R11
      0x7C380200,  //  00BF  CALL	R14	1
      0x7C300400,  //  00C0  CALL	R12	2
      0x8C34190D,  //  00C1  GETMET	R13	R12	K13
      0x583C000E,  //  00C2  LDCONST	R15	K14
      0x7C340400,  //  00C3  CALL	R13	2
      0x5C381A00,  //  00C4  MOVE	R14	R13
      0x743A0001,  //  00C5  JMPT	R14	#00C8
      0x00140B0F,  //  00C6  ADD	R5	R5	K15
      0x7001FFEB,  //  00C7  JMP		#00B4
      0x8C38050D,  //  00C8  GETMET	R14	R2	K13
      0x5C401A00,  //  00C9  MOVE	R16	R13
      0x58440010,  //  00CA  LDCONST	R17	K16
      0x7C380600,  //  00CB  CALL	R14	3
      0x20381D0C,  //  00CC  NE	R14	R14	K12
      0x783A0001,  //  00CD  JMPF	R14	#00D0
      0x00140B0F,  //  00CE  ADD	R5	R5	K15
      0x7001FFE3,  //  00CF  JMP		#00B4
      0x8C38190D,  //  00D0  GETMET	R14	R12	K13
      0x58400020,  //  00D1  LDCONST	R16	K32
      0x7C380400,  //  00D2  CALL	R14	2
      0x20381C09,  //  00D3  NE	R14	R14	R9
      0x783A0001,  //  00D4  JMPF	R14	#00D7
      0x00140B0F,  //  00D5  ADD	R5	R5	K15
      0x7001FFDC,  //  00D6  JMP		#00B4
      0x88380107,  //  00D7  GETMBR	R14	R0	K7
      0x88381D11,  //  00D8  GETMBR	R14	R14	K17
      0x8C381D0D,  //  00D9  GETMET	R14	R14	K13
      0x5C401A00,  //  00DA  MOVE	R16	R13
      0x7C380400,  //  00DB  CALL	R14	2
      0x583C0012,  //  00DC  LDCONST	R15	K18
      0x4C400000,  //  00DD  LDNIL	R16
      0x20401C10,  //  00DE  NE	R16	R14	R16
      0x78420004,  //  00DF  JMPF	R16	#00E5
      0x8C401D13,  //  00E0  GETMET	R16	R14	K19
      0x5C481C00,  //  00E1  MOVE	R18	R14
      0x5C4C1800,  //  00E2  MOVE	R19	R12
      0x7C400600,  //  00E3  CALL	R16	3
      0x5C3C2000,  //  00E4  MOVE	R15	R16
      0x50180200,  //  00E5  LDBOOL	R6	1	0
      0x8C400303,  //  00E6  GETMET	R16	R1	K3
      0x60480018,  //  00E7  GETGBL	R18	G24
      0x584C0027,  //  00E8  LDCONST	R19	K39
      0x5C501600,  //  00E9  MOVE	R20	R11
      0x7C480400,  //  00EA  CALL	R18	2
      0x7C400400,  //  00EB  CALL	R16	2
      0x8C400303,  //  00EC  GETMET	R16	R1	K3
      0x60480018,  //  00ED  GETGBL	R18	G24
      0x584C0028,  //  00EE  LDCONST	R19	K40
      0x5C501600,  //  00EF  MOVE	R20	R11
      0x8C540316,  //  00F0  GETMET	R21	R1	K22
      0x8C5C190D,  //  00F1  GETMET	R23	R12	K13
      0x58640017,  //  00F2  LDCONST	R25	K23
      0x58680012,  //  00F3  LDCONST	R26	K18
      0x7C5C0600,  //  00F4  CALL	R23	3
      0x7C540400,  //  00F5  CALL	R21	2
      0x7C480600,  //  00F6  CALL	R18	3
      0x7C400400,  //  00F7  CALL	R16	2
      0x8C400303,  //  00F8  GETMET	R16	R1	K3
      0x60480018,  //  00F9  GETGBL	R18	G24
      0x584C0029,  //  00FA  LDCONST	R19	K41
      0x8C500119,  //  00FB  GETMET	R20	R0	K25
      0x8C58190D,  //  00FC  GETMET	R22	R12	K13
      0x5860000E,  //  00FD  LDCONST	R24	K14
      0x58640012,  //  00FE  LDCONST	R25	K18
      0x7C580600,  //  00FF  CALL	R22	3
      0x7C500400,  //  0100  CALL	R20	2
      0x7C480400,  //  0101  CALL	R18	2
      0x7C400400,  //  0102  CALL	R16	2
      0x8C400303,  //  0103  GETMET	R16	R1	K3
      0x60480018,  //  0104  GETGBL	R18	G24
      0x584C002A,  //  0105  LDCONST	R19	K42
      0x5C501600,  //  0106  MOVE	R20	R11
      0x8C540316,  //  0107  GETMET	R21	R1	K22
      0x5C5C1E00,  //  0108  MOVE	R23	R15
      0x7C540400,  //  0109  CALL	R21	2
      0x7C480600,  //  010A  CALL	R18	3
      0x7C400400,  //  010B  CALL	R16	2
      0x8C400303,  //  010C  GETMET	R16	R1	K3
      0x60480018,  //  010D  GETGBL	R18	G24
      0x584C002B,  //  010E  LDCONST	R19	K43
      0x5C501600,  //  010F  MOVE	R20	R11
      0x7C480400,  //  0110  CALL	R18	2
      0x7C400400,  //  0111  CALL	R16	2
      0x00140B0F,  //  0112  ADD	R5	R5	K15
      0x7001FF9F,  //  0113  JMP		#00B4
      0x8C2C0303,  //  0114  GETMET	R11	R1	K3
      0x5834002C,  //  0115  LDCONST	R13	K44
      0x7C2C0400,  //  0116  CALL	R11	2
      0x7001FF87,  //  0117  JMP		#00A0
      0x58200022,  //  0118  LDCONST	R8	K34
      0xAC200200,  //  0119  CATCH	R8	1	0
      0xB0080000,  //  011A  RAISE	2	R0	R0
      0x5C200C00,  //  011B  MOVE	R8	R6
      0x74220002,  //  011C  JMPT	R8	#0120
      0x8C200303,  //  011D  GETMET	R8	R1	K3
      0x5828001E,  //  011E  LDCONST	R10	K30
      0x7C200400,  //  011F  CALL	R8	2
      0x8C200303,  //  0120  GETMET	R8	R1	K3
      0x5828002D,  //  0121  LDCONST	R10	K45
      0x7C200400,  //  0122  CALL	R8	2
      0x8C20012E,  //  0123  GETMET	R8	R0	K46
      0x8828012F,  //  0124  GETMBR	R10	R0	K47
      0x7C200400,  //  0125  CALL	R8	2
      0x8C200303,  //  0126  GETMET	R8	R1	K3
      0x58280030,  //  0127  LDCONST	R10	K48
      0x7C200400,  //  0128  CALL	R8	2
      0x8C200303,  //  0129  GETMET	R8	R1	K3
      0x58280031,  //  012A  LDCONST	R10	K49
      0x7C200400,  //  012B  CALL	R8	2
      0x8C200303,  //  012C  GETMET	R8	R1	K3
      0x58280032,  //  012D  LDCONST	R10	K50
      0x7C200400,  //  012E  CALL	R8	2
      0x8C200303,  //  012F  GETMET	R8	R1	K3
      0x58280033,  //  0130  LDCONST	R10	K51
      0x7C200400,  //  0131  CALL	R8	2
      0x8C200134,  //  0132  GETMET	R8	R0	K52
      0x58280012,  //  0133  LDCONST	R10	K18
      0x882C012F,  //  0134  GETMBR	R11	R0	K47
      0x7C200600,  //  0135  CALL	R8	3
      0x8C200303,  //  0136  GETMET	R8	R1	K3
      0x58280035,  //  0137  LDCONST	R10	K53
      0x7C200400,  //  0138  CALL	R8	2
      0x8C200303,  //  0139  GETMET	R8	R1	K3
      0x58280036,  //  013A  LDCONST	R10	K54
      0x7C200400,  //  013B  CALL	R8	2
      0x8C200303,  //  013C  GETMET	R8	R1	K3
      0x58280037,  //  013D  LDCONST	R10	K55
      0x7C200400,  //  013E  CALL	R8	2
      0x8C200303,  //  013F  GETMET	R8	R1	K3
      0x58280038,  //  0140  LDCONST	R10	K56
      0x7C200400,  //  0141  CALL	R8	2
      0x8C200303,  //  0142  GETMET	R8	R1	K3
      0x58280039,  //  0143  LDCONST	R10	K57
      0x7C200400,  //  0144  CALL	R8	2
      0x8C200303,  //  0145  GETMET	R8	R1	K3
      0x5828003A,  //  0146  LDCONST	R10	K58
      0x7C200400,  //  0147  CALL	R8	2
      0x8C200303,  //  0148  GETMET	R8	R1	K3
      0x58280037,  //  0149  LDCONST	R10	K55
      0x7C200400,  //  014A  CALL	R8	2
      0x8C200303,  //  014B  GETMET	R8	R1	K3
      0x5828003B,  //  014C  LDCONST	R10	K59
      0x7C200400,  //  014D  CALL	R8	2
      0x8C200303,  //  014E  GETMET	R8	R1	K3
      0x5828003C,  //  014F  LDCONST	R10	K60
      0x7C200400,  //  0150  CALL	R8	2
      0x8C200303,  //  0151  GETMET	R8	R1	K3
      0x5828003D,  //  0152  LDCONST	R10	K61
      0x7C200400,  //  0153  CALL	R8	2
      0x80000000,  //  0154  RET	0
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
    24,                          /* nstack */
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
    /* K23  */  be_nested_str_weak(_X3Cp_X3E_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X3E_X25s_X3C_X2Fa_X3E_X3C_X2Fp_X3E),
    /* K24  */  be_nested_str_weak(html_escape),
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
      0x5C1C0200,  //  000A  MOVE	R7	R1
      0x5C200800,  //  000B  MOVE	R8	R4
      0x7C140600,  //  000C  CALL	R5	3
      0x8C180B07,  //  000D  GETMET	R6	R5	K7
      0x58200008,  //  000E  LDCONST	R8	K8
      0x5C240800,  //  000F  MOVE	R9	R4
      0x7C180600,  //  0010  CALL	R6	3
      0x4C1C0000,  //  0011  LDNIL	R7
      0x201C0C07,  //  0012  NE	R7	R6	R7
      0x781E0003,  //  0013  JMPF	R7	#0018
      0x8C1C0709,  //  0014  GETMET	R7	R3	K9
      0x5C240C00,  //  0015  MOVE	R9	R6
      0x7C1C0400,  //  0016  CALL	R7	2
      0x5C180E00,  //  0017  MOVE	R6	R7
      0x4C1C0000,  //  0018  LDNIL	R7
      0x201C0C07,  //  0019  NE	R7	R6	R7
      0x781E0003,  //  001A  JMPF	R7	#001F
      0x8C1C0D0A,  //  001B  GETMET	R7	R6	K10
      0x5824000B,  //  001C  LDCONST	R9	K11
      0x7C1C0400,  //  001D  CALL	R7	2
      0x5C180E00,  //  001E  MOVE	R6	R7
      0x4C1C0000,  //  001F  LDNIL	R7
      0x4C200000,  //  0020  LDNIL	R8
      0x20200C08,  //  0021  NE	R8	R6	R8
      0x78220012,  //  0022  JMPF	R8	#0036
      0x8C200B07,  //  0023  GETMET	R8	R5	K7
      0x5828000C,  //  0024  LDCONST	R10	K12
      0x5C2C0800,  //  0025  MOVE	R11	R4
      0x7C200600,  //  0026  CALL	R8	3
      0x5C1C1000,  //  0027  MOVE	R7	R8
      0x4C200000,  //  0028  LDNIL	R8
      0x20200E08,  //  0029  NE	R8	R7	R8
      0x78220003,  //  002A  JMPF	R8	#002F
      0x8C200709,  //  002B  GETMET	R8	R3	K9
      0x5C280E00,  //  002C  MOVE	R10	R7
      0x7C200400,  //  002D  CALL	R8	2
      0x5C1C1000,  //  002E  MOVE	R7	R8
      0x4C200000,  //  002F  LDNIL	R8
      0x20200E08,  //  0030  NE	R8	R7	R8
      0x78220003,  //  0031  JMPF	R8	#0036
      0x8C200F0A,  //  0032  GETMET	R8	R7	K10
      0x5828000D,  //  0033  LDCONST	R10	K13
      0x7C200400,  //  0034  CALL	R8	2
      0x5C1C1000,  //  0035  MOVE	R7	R8
      0x4C200000,  //  0036  LDNIL	R8
      0x20200C08,  //  0037  NE	R8	R6	R8
      0x782200A1,  //  0038  JMPF	R8	#00DB
      0x4C200000,  //  0039  LDNIL	R8
      0x20200E08,  //  003A  NE	R8	R7	R8
      0x7822009E,  //  003B  JMPF	R8	#00DB
      0xB8221C00,  //  003C  GETNGBL	R8	K14
      0x8C20110F,  //  003D  GETMET	R8	R8	K15
      0x60280018,  //  003E  GETGBL	R10	G24
      0x582C0010,  //  003F  LDCONST	R11	K16
      0x5C300200,  //  0040  MOVE	R12	R1
      0x60340008,  //  0041  GETGBL	R13	G8
      0x5C380C00,  //  0042  MOVE	R14	R6
      0x7C340200,  //  0043  CALL	R13	1
      0x60380008,  //  0044  GETGBL	R14	G8
      0x5C3C0E00,  //  0045  MOVE	R15	R7
      0x7C380200,  //  0046  CALL	R14	1
      0x7C280800,  //  0047  CALL	R10	4
      0x582C0011,  //  0048  LDCONST	R11	K17
      0x7C200600,  //  0049  CALL	R8	3
      0x8C200112,  //  004A  GETMET	R8	R0	K18
      0x5C280C00,  //  004B  MOVE	R10	R6
      0x5C2C0E00,  //  004C  MOVE	R11	R7
      0x7C200600,  //  004D  CALL	R8	3
      0x8C240113,  //  004E  GETMET	R9	R0	K19
      0x882C0114,  //  004F  GETMBR	R11	R0	K20
      0x7C240400,  //  0050  CALL	R9	2
      0x8C240515,  //  0051  GETMET	R9	R2	K21
      0x582C0016,  //  0052  LDCONST	R11	K22
      0x7C240400,  //  0053  CALL	R9	2
      0x8C240515,  //  0054  GETMET	R9	R2	K21
      0x602C0018,  //  0055  GETGBL	R11	G24
      0x58300017,  //  0056  LDCONST	R12	K23
      0x8C340518,  //  0057  GETMET	R13	R2	K24
      0x5C3C0200,  //  0058  MOVE	R15	R1
      0x7C340400,  //  0059  CALL	R13	2
      0x8C380518,  //  005A  GETMET	R14	R2	K24
      0x5C400200,  //  005B  MOVE	R16	R1
      0x7C380400,  //  005C  CALL	R14	2
      0x7C2C0600,  //  005D  CALL	R11	3
      0x7C240400,  //  005E  CALL	R9	2
      0x8C240515,  //  005F  GETMET	R9	R2	K21
      0x582C0019,  //  0060  LDCONST	R11	K25
      0x7C240400,  //  0061  CALL	R9	2
      0x8C240515,  //  0062  GETMET	R9	R2	K21
      0x602C0018,  //  0063  GETGBL	R11	G24
      0x5830001A,  //  0064  LDCONST	R12	K26
      0x8C340518,  //  0065  GETMET	R13	R2	K24
      0x5C3C0200,  //  0066  MOVE	R15	R1
      0x7C340400,  //  0067  CALL	R13	2
      0x7C2C0400,  //  0068  CALL	R11	2
      0x7C240400,  //  0069  CALL	R9	2
      0x5824001B,  //  006A  LDCONST	R9	K27
      0x6028000C,  //  006B  GETGBL	R10	G12
      0x5C2C1000,  //  006C  MOVE	R11	R8
      0x7C280200,  //  006D  CALL	R10	1
      0x1428120A,  //  006E  LT	R10	R9	R10
      0x782A003E,  //  006F  JMPF	R10	#00AF
      0x94281009,  //  0070  GETIDX	R10	R8	R9
      0x8C2C150A,  //  0071  GETMET	R11	R10	K10
      0x5834001C,  //  0072  LDCONST	R13	K28
      0x58380002,  //  0073  LDCONST	R14	K2
      0x7C2C0600,  //  0074  CALL	R11	3
      0x20301702,  //  0075  NE	R12	R11	K2
      0x78320000,  //  0076  JMPF	R12	#0078
      0x002E3A0B,  //  0077  ADD	R11	K29	R11
      0x8830011E,  //  0078  GETMBR	R12	R0	K30
      0x8830191F,  //  0079  GETMBR	R12	R12	K31
      0x8C30190A,  //  007A  GETMET	R12	R12	K10
      0x5C381600,  //  007B  MOVE	R14	R11
      0x7C300400,  //  007C  CALL	R12	2
      0x58340002,  //  007D  LDCONST	R13	K2
      0x4C380000,  //  007E  LDNIL	R14
      0x2038180E,  //  007F  NE	R14	R12	R14
      0x783A0004,  //  0080  JMPF	R14	#0086
      0x8C381920,  //  0081  GETMET	R14	R12	K32
      0x5C401800,  //  0082  MOVE	R16	R12
      0x5C441400,  //  0083  MOVE	R17	R10
      0x7C380600,  //  0084  CALL	R14	3
      0x5C341C00,  //  0085  MOVE	R13	R14
      0x8C380515,  //  0086  GETMET	R14	R2	K21
      0x60400018,  //  0087  GETGBL	R16	G24
      0x58440021,  //  0088  LDCONST	R17	K33
      0x5C481200,  //  0089  MOVE	R18	R9
      0x7C400400,  //  008A  CALL	R16	2
      0x7C380400,  //  008B  CALL	R14	2
      0x8C380515,  //  008C  GETMET	R14	R2	K21
      0x60400018,  //  008D  GETGBL	R16	G24
      0x58440022,  //  008E  LDCONST	R17	K34
      0x5C481200,  //  008F  MOVE	R18	R9
      0x5C4C1200,  //  0090  MOVE	R19	R9
      0x7C400600,  //  0091  CALL	R16	3
      0x7C380400,  //  0092  CALL	R14	2
      0x8C380123,  //  0093  GETMET	R14	R0	K35
      0x5C401600,  //  0094  MOVE	R16	R11
      0x88440114,  //  0095  GETMBR	R17	R0	K20
      0x7C380600,  //  0096  CALL	R14	3
      0x8C380515,  //  0097  GETMET	R14	R2	K21
      0x58400024,  //  0098  LDCONST	R16	K36
      0x7C380400,  //  0099  CALL	R14	2
      0x8C380515,  //  009A  GETMET	R14	R2	K21
      0x60400018,  //  009B  GETGBL	R16	G24
      0x58440025,  //  009C  LDCONST	R17	K37
      0x5C481200,  //  009D  MOVE	R18	R9
      0x5C4C1200,  //  009E  MOVE	R19	R9
      0x8C500518,  //  009F  GETMET	R20	R2	K24
      0x5C581A00,  //  00A0  MOVE	R22	R13
      0x7C500400,  //  00A1  CALL	R20	2
      0x78320003,  //  00A2  JMPF	R12	#00A7
      0x8C540518,  //  00A3  GETMET	R21	R2	K24
      0x885C1926,  //  00A4  GETMBR	R23	R12	K38
      0x7C540400,  //  00A5  CALL	R21	2
      0x70020000,  //  00A6  JMP		#00A8
      0x58540002,  //  00A7  LDCONST	R21	K2
      0x7C400A00,  //  00A8  CALL	R16	5
      0x7C380400,  //  00A9  CALL	R14	2
      0x8C380515,  //  00AA  GETMET	R14	R2	K21
      0x58400027,  //  00AB  LDCONST	R16	K39
      0x7C380400,  //  00AC  CALL	R14	2
      0x00241328,  //  00AD  ADD	R9	R9	K40
      0x7001FFBB,  //  00AE  JMP		#006B
      0x8C280515,  //  00AF  GETMET	R10	R2	K21
      0x60300018,  //  00B0  GETGBL	R12	G24
      0x58340021,  //  00B1  LDCONST	R13	K33
      0x5C381200,  //  00B2  MOVE	R14	R9
      0x7C300400,  //  00B3  CALL	R12	2
      0x7C280400,  //  00B4  CALL	R10	2
      0x8C280515,  //  00B5  GETMET	R10	R2	K21
      0x60300018,  //  00B6  GETGBL	R12	G24
      0x58340022,  //  00B7  LDCONST	R13	K34
      0x5C381200,  //  00B8  MOVE	R14	R9
      0x5C3C1200,  //  00B9  MOVE	R15	R9
      0x7C300600,  //  00BA  CALL	R12	3
      0x7C280400,  //  00BB  CALL	R10	2
      0x8C280123,  //  00BC  GETMET	R10	R0	K35
      0x58300002,  //  00BD  LDCONST	R12	K2
      0x88340114,  //  00BE  GETMBR	R13	R0	K20
      0x7C280600,  //  00BF  CALL	R10	3
      0x8C280515,  //  00C0  GETMET	R10	R2	K21
      0x58300024,  //  00C1  LDCONST	R12	K36
      0x7C280400,  //  00C2  CALL	R10	2
      0x8C280515,  //  00C3  GETMET	R10	R2	K21
      0x60300018,  //  00C4  GETGBL	R12	G24
      0x58340029,  //  00C5  LDCONST	R13	K41
      0x5C381200,  //  00C6  MOVE	R14	R9
      0x5C3C1200,  //  00C7  MOVE	R15	R9
      0x58400002,  //  00C8  LDCONST	R16	K2
      0x7C300800,  //  00C9  CALL	R12	4
      0x7C280400,  //  00CA  CALL	R10	2
      0x8C280515,  //  00CB  GETMET	R10	R2	K21
      0x58300027,  //  00CC  LDCONST	R12	K39
      0x7C280400,  //  00CD  CALL	R10	2
      0x8C280515,  //  00CE  GETMET	R10	R2	K21
      0x5830002A,  //  00CF  LDCONST	R12	K42
      0x7C280400,  //  00D0  CALL	R10	2
      0x8C280515,  //  00D1  GETMET	R10	R2	K21
      0x5830002B,  //  00D2  LDCONST	R12	K43
      0x7C280400,  //  00D3  CALL	R10	2
      0x8C280515,  //  00D4  GETMET	R10	R2	K21
      0x5830002C,  //  00D5  LDCONST	R12	K44
      0x7C280400,  //  00D6  CALL	R10	2
      0x8C280515,  //  00D7  GETMET	R10	R2	K21
      0x5830002D,  //  00D8  LDCONST	R12	K45
      0x7C280400,  //  00D9  CALL	R10	2
      0x70020007,  //  00DA  JMP		#00E3
      0x8C200515,  //  00DB  GETMET	R8	R2	K21
      0x60280018,  //  00DC  GETGBL	R10	G24
      0x582C002E,  //  00DD  LDCONST	R11	K46
      0x8C300518,  //  00DE  GETMET	R12	R2	K24
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
    /* K9   */  be_nested_str_weak(_X2Dhttp),
    /* K10  */  be_nested_str_weak(_X3Coption_X20value_X3D_X27_X27_X20disabled_X3E_X2D_X2D_X2D_X20Tasmota_X20Remote_X20_X2D_X2D_X2D_X3C_X2Foption_X3E),
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
    14,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
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
    /* K17  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27ztdm_X20htrm_X27_X3E_X3Ctd_X3E_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20title_X3D_X27http_X3A_X2F_X2F_X25s_X2F_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X27_X3E_X25s_X3C_X2Fa_X3E_X3C_X2Ftd_X3E),
    /* K18  */  be_nested_str_weak(web_last_seen),
    /* K19  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27htrm_X27_X3E_X3Ctd_X20colspan_X3D_X272_X27_X3E),
    /* K20  */  be_nested_str_weak(web_values),
    /* K21  */  be_nested_str_weak(_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K22  */  be_nested_str_weak(stop_iteration),
    /* K23  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Chr_X3E),
    }),
    be_str_weak(show_bridge_status),
    &be_const_str_solidified,
    ( &(const binstruction[106]) {  /* code */
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
      0xA8020026,  //  003B  EXBLK	0	#0063
      0x5C140800,  //  003C  MOVE	R5	R4
      0x7C140000,  //  003D  CALL	R5	0
      0x8C180310,  //  003E  GETMET	R6	R1	K16
      0x5C200A00,  //  003F  MOVE	R8	R5
      0x7C180400,  //  0040  CALL	R6	2
      0x8C1C030B,  //  0041  GETMET	R7	R1	K11
      0x60240018,  //  0042  GETGBL	R9	G24
      0x58280011,  //  0043  LDCONST	R10	K17
      0x5C2C0C00,  //  0044  MOVE	R11	R6
      0x5C300C00,  //  0045  MOVE	R12	R6
      0x5C340C00,  //  0046  MOVE	R13	R6
      0x7C240800,  //  0047  CALL	R9	4
      0x7C1C0400,  //  0048  CALL	R7	2
      0x941C0405,  //  0049  GETIDX	R7	R2	R5
      0x941C0F01,  //  004A  GETIDX	R7	R7	K1
      0x881C0F06,  //  004B  GETMBR	R7	R7	K6
      0x8C20030B,  //  004C  GETMET	R8	R1	K11
      0x8C280F12,  //  004D  GETMET	R10	R7	K18
      0x7C280200,  //  004E  CALL	R10	1
      0x7C200400,  //  004F  CALL	R8	2
      0x60200010,  //  0050  GETGBL	R8	G16
      0x94240405,  //  0051  GETIDX	R9	R2	R5
      0x7C200200,  //  0052  CALL	R8	1
      0xA802000A,  //  0053  EXBLK	0	#005F
      0x5C241000,  //  0054  MOVE	R9	R8
      0x7C240000,  //  0055  CALL	R9	0
      0x8C28030B,  //  0056  GETMET	R10	R1	K11
      0x58300013,  //  0057  LDCONST	R12	K19
      0x7C280400,  //  0058  CALL	R10	2
      0x8C281314,  //  0059  GETMET	R10	R9	K20
      0x7C280200,  //  005A  CALL	R10	1
      0x8C28030B,  //  005B  GETMET	R10	R1	K11
      0x58300015,  //  005C  LDCONST	R12	K21
      0x7C280400,  //  005D  CALL	R10	2
      0x7001FFF4,  //  005E  JMP		#0054
      0x58200016,  //  005F  LDCONST	R8	K22
      0xAC200200,  //  0060  CATCH	R8	1	0
      0xB0080000,  //  0061  RAISE	2	R0	R0
      0x7001FFD8,  //  0062  JMP		#003C
      0x58100016,  //  0063  LDCONST	R4	K22
      0xAC100200,  //  0064  CATCH	R4	1	0
      0xB0080000,  //  0065  RAISE	2	R0	R0
      0x8C10030B,  //  0066  GETMET	R4	R1	K11
      0x58180017,  //  0067  LDCONST	R6	K23
      0x7C100400,  //  0068  CALL	R4	2
      0x80000000,  //  0069  RET	0
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
    ( &(const bvalue[99]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(check_privileged_access),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(partition_core),
    /* K4   */  be_nested_str_weak(persist),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(arg_size),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20Arg_X25i_X20_X27_X25s_X27_X20_X3D_X20_X27_X25s_X27),
    /* K11  */  be_nested_str_weak(arg_name),
    /* K12  */  be_nested_str_weak(arg),
    /* K13  */  be_nested_str_weak(stop_iteration),
    /* K14  */  be_nested_str_weak(has_arg),
    /* K15  */  be_nested_str_weak(passcode),
    /* K16  */  be_nested_str_weak(discriminator),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20_X2Fmatterc_X20received_X20_X27_X25s_X27_X20command),
    /* K18  */  be_const_int(3),
    /* K19  */  be_nested_str_weak(device),
    /* K20  */  be_nested_str_weak(root_passcode),
    /* K21  */  be_nested_str_weak(root_discriminator),
    /* K22  */  be_nested_str_weak(ipv4only),
    /* K23  */  be_nested_str_weak(ipv4),
    /* K24  */  be_nested_str_weak(on),
    /* K25  */  be_nested_str_weak(save_param),
    /* K26  */  be_nested_str_weak(redirect),
    /* K27  */  be_nested_str_weak(_X2F_X3Frst_X3D),
    /* K28  */  be_nested_str_weak(save),
    /* K29  */  be_nested_str_weak(menable),
    /* K30  */  be_nested_str_weak(comm),
    /* K31  */  be_nested_str_weak(matter_enabled),
    /* K32  */  be_nested_str_weak(enable),
    /* K33  */  be_nested_str_weak(cmd),
    /* K34  */  be_nested_str_weak(SetOption),
    /* K35  */  be_nested_str_weak(matter),
    /* K36  */  be_nested_str_weak(MATTER_OPTION),
    /* K37  */  be_nested_str_weak(_X201),
    /* K38  */  be_nested_str_weak(disable),
    /* K39  */  be_nested_str_weak(_X200),
    /* K40  */  be_nested_str_weak(commissioning_open),
    /* K41  */  be_nested_str_weak(start_root_basic_commissioning),
    /* K42  */  be_nested_str_weak(stop_basic_commissioning),
    /* K43  */  be_nested_str_weak(_X2F),
    /* K44  */  be_nested_str_weak(del_fabric),
    /* K45  */  be_nested_str_weak(sessions),
    /* K46  */  be_nested_str_weak(fabrics),
    /* K47  */  be_nested_str_weak(get_fabric_index),
    /* K48  */  be_nested_str_weak(remove_fabric),
    /* K49  */  be_nested_str_weak(_X2Fmatterc_X3F),
    /* K50  */  be_nested_str_weak(auto),
    /* K51  */  be_nested_str_weak(plugins_persist),
    /* K52  */  be_nested_str_weak(config),
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
    /* K73  */  be_nested_str_weak(MTR_X3A_X20config_X20_X3D_X20_X25s),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20config_X20error_X20_X3D_X20_X25s),
    /* K75  */  be_nested_str_weak(addep),
    /* K76  */  be_nested_str_weak(pi),
    /* K77  */  be_nested_str_weak(MTR_X3A_X20add_X20endpoint_X20typ_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K78  */  be_nested_str_weak(bridge_add_endpoint),
    /* K79  */  be_nested_str_weak(addrem),
    /* K80  */  be_nested_str_weak(url),
    /* K81  */  be_nested_str_weak(value_error),
    /* K82  */  be_nested_str_weak(url_X20shouldn_X27t_X20be_X20null),
    /* K83  */  be_nested_str_weak(equal_map),
    /* K84  */  be_nested_str_weak(MTR_X3A_X20remote_X20add_X20url_X3D_X27_X25s_X27_X20type_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K85  */  be_nested_str_weak(del),
    /* K86  */  be_nested_str_weak(bridge_remove_endpoint),
    /* K87  */  be_nested_str_weak(content_start),
    /* K88  */  be_nested_str_weak(Parameter_X20error),
    /* K89  */  be_nested_str_weak(content_send_style),
    /* K90  */  be_nested_str_weak(content_send),
    /* K91  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EError_X3A_X3C_X2Fb_X3E_X25s_X3C_X2Fp_X3E),
    /* K92  */  be_nested_str_weak(html_escape),
    /* K93  */  be_nested_str_weak(content_button),
    /* K94  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K95  */  be_nested_str_weak(content_stop),
    /* K96  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K97  */  be_const_int(2),
    /* K98  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
    }),
    be_str_weak(page_part_ctl),
    &be_const_str_solidified,
    ( &(const binstruction[688]) {  /* code */
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
      0xA8020285,  //  000A  EXBLK	0	#0291
      0x60180010,  //  000B  GETGBL	R6	G16
      0x8C1C0306,  //  000C  GETMET	R7	R1	K6
      0x7C1C0200,  //  000D  CALL	R7	1
      0x041C0F07,  //  000E  SUB	R7	R7	K7
      0x401E0A07,  //  000F  CONNECT	R7	K5	R7
      0x7C180200,  //  0010  CALL	R6	1
      0xA802000F,  //  0011  EXBLK	0	#0022
      0x5C1C0C00,  //  0012  MOVE	R7	R6
      0x7C1C0000,  //  0013  CALL	R7	0
      0xB8221000,  //  0014  GETNGBL	R8	K8
      0x8C201109,  //  0015  GETMET	R8	R8	K9
      0x60280018,  //  0016  GETGBL	R10	G24
      0x582C000A,  //  0017  LDCONST	R11	K10
      0x5C300E00,  //  0018  MOVE	R12	R7
      0x8C34030B,  //  0019  GETMET	R13	R1	K11
      0x5C3C0E00,  //  001A  MOVE	R15	R7
      0x7C340400,  //  001B  CALL	R13	2
      0x8C38030C,  //  001C  GETMET	R14	R1	K12
      0x5C400E00,  //  001D  MOVE	R16	R7
      0x7C380400,  //  001E  CALL	R14	2
      0x7C280800,  //  001F  CALL	R10	4
      0x7C200400,  //  0020  CALL	R8	2
      0x7001FFEF,  //  0021  JMP		#0012
      0x5818000D,  //  0022  LDCONST	R6	K13
      0xAC180200,  //  0023  CATCH	R6	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x8C18030E,  //  0025  GETMET	R6	R1	K14
      0x5820000F,  //  0026  LDCONST	R8	K15
      0x7C180400,  //  0027  CALL	R6	2
      0x741A0003,  //  0028  JMPT	R6	#002D
      0x8C18030E,  //  0029  GETMET	R6	R1	K14
      0x58200010,  //  002A  LDCONST	R8	K16
      0x7C180400,  //  002B  CALL	R6	2
      0x781A002A,  //  002C  JMPF	R6	#0058
      0xB81A1000,  //  002D  GETNGBL	R6	K8
      0x8C180D09,  //  002E  GETMET	R6	R6	K9
      0x60200018,  //  002F  GETGBL	R8	G24
      0x58240011,  //  0030  LDCONST	R9	K17
      0x5828000F,  //  0031  LDCONST	R10	K15
      0x7C200400,  //  0032  CALL	R8	2
      0x58240012,  //  0033  LDCONST	R9	K18
      0x7C180600,  //  0034  CALL	R6	3
      0x8C18030E,  //  0035  GETMET	R6	R1	K14
      0x5820000F,  //  0036  LDCONST	R8	K15
      0x7C180400,  //  0037  CALL	R6	2
      0x781A0006,  //  0038  JMPF	R6	#0040
      0x88180113,  //  0039  GETMBR	R6	R0	K19
      0x601C0009,  //  003A  GETGBL	R7	G9
      0x8C20030C,  //  003B  GETMET	R8	R1	K12
      0x5828000F,  //  003C  LDCONST	R10	K15
      0x7C200400,  //  003D  CALL	R8	2
      0x7C1C0200,  //  003E  CALL	R7	1
      0x901A2807,  //  003F  SETMBR	R6	K20	R7
      0x8C18030E,  //  0040  GETMET	R6	R1	K14
      0x58200010,  //  0041  LDCONST	R8	K16
      0x7C180400,  //  0042  CALL	R6	2
      0x781A0006,  //  0043  JMPF	R6	#004B
      0x88180113,  //  0044  GETMBR	R6	R0	K19
      0x601C0009,  //  0045  GETGBL	R7	G9
      0x8C20030C,  //  0046  GETMET	R8	R1	K12
      0x58280010,  //  0047  LDCONST	R10	K16
      0x7C200400,  //  0048  CALL	R8	2
      0x7C1C0200,  //  0049  CALL	R7	1
      0x901A2A07,  //  004A  SETMBR	R6	K21	R7
      0x88180113,  //  004B  GETMBR	R6	R0	K19
      0x8C1C030C,  //  004C  GETMET	R7	R1	K12
      0x58240017,  //  004D  LDCONST	R9	K23
      0x7C1C0400,  //  004E  CALL	R7	2
      0x1C1C0F18,  //  004F  EQ	R7	R7	K24
      0x901A2C07,  //  0050  SETMBR	R6	K22	R7
      0x88180113,  //  0051  GETMBR	R6	R0	K19
      0x8C180D19,  //  0052  GETMET	R6	R6	K25
      0x7C180200,  //  0053  CALL	R6	1
      0x8C18031A,  //  0054  GETMET	R6	R1	K26
      0x5820001B,  //  0055  LDCONST	R8	K27
      0x7C180400,  //  0056  CALL	R6	2
      0x70020223,  //  0057  JMP		#027C
      0x8C18030E,  //  0058  GETMET	R6	R1	K14
      0x5820001C,  //  0059  LDCONST	R8	K28
      0x7C180400,  //  005A  CALL	R6	2
      0x781A0047,  //  005B  JMPF	R6	#00A4
      0x8C18030E,  //  005C  GETMET	R6	R1	K14
      0x5820001D,  //  005D  LDCONST	R8	K29
      0x7C180400,  //  005E  CALL	R6	2
      0x8C1C030E,  //  005F  GETMET	R7	R1	K14
      0x5824001E,  //  0060  LDCONST	R9	K30
      0x7C1C0400,  //  0061  CALL	R7	2
      0x8C20011F,  //  0062  GETMET	R8	R0	K31
      0x7C200200,  //  0063  CALL	R8	1
      0x20200C08,  //  0064  NE	R8	R6	R8
      0x78220027,  //  0065  JMPF	R8	#008E
      0x781A0011,  //  0066  JMPF	R6	#0079
      0xB8221000,  //  0067  GETNGBL	R8	K8
      0x8C201109,  //  0068  GETMET	R8	R8	K9
      0x60280018,  //  0069  GETGBL	R10	G24
      0x582C0011,  //  006A  LDCONST	R11	K17
      0x58300020,  //  006B  LDCONST	R12	K32
      0x7C280400,  //  006C  CALL	R10	2
      0x582C0012,  //  006D  LDCONST	R11	K18
      0x7C200600,  //  006E  CALL	R8	3
      0xB8221000,  //  006F  GETNGBL	R8	K8
      0x8C201121,  //  0070  GETMET	R8	R8	K33
      0x60280008,  //  0071  GETGBL	R10	G8
      0xB82E4600,  //  0072  GETNGBL	R11	K35
      0x882C1724,  //  0073  GETMBR	R11	R11	K36
      0x7C280200,  //  0074  CALL	R10	1
      0x002A440A,  //  0075  ADD	R10	K34	R10
      0x00281525,  //  0076  ADD	R10	R10	K37
      0x7C200400,  //  0077  CALL	R8	2
      0x70020010,  //  0078  JMP		#008A
      0xB8221000,  //  0079  GETNGBL	R8	K8
      0x8C201109,  //  007A  GETMET	R8	R8	K9
      0x60280018,  //  007B  GETGBL	R10	G24
      0x582C0011,  //  007C  LDCONST	R11	K17
      0x58300026,  //  007D  LDCONST	R12	K38
      0x7C280400,  //  007E  CALL	R10	2
      0x582C0012,  //  007F  LDCONST	R11	K18
      0x7C200600,  //  0080  CALL	R8	3
      0xB8221000,  //  0081  GETNGBL	R8	K8
      0x8C201121,  //  0082  GETMET	R8	R8	K33
      0x60280008,  //  0083  GETGBL	R10	G8
      0xB82E4600,  //  0084  GETNGBL	R11	K35
      0x882C1724,  //  0085  GETMBR	R11	R11	K36
      0x7C280200,  //  0086  CALL	R10	1
      0x002A440A,  //  0087  ADD	R10	K34	R10
      0x00281527,  //  0088  ADD	R10	R10	K39
      0x7C200400,  //  0089  CALL	R8	2
      0x8C20031A,  //  008A  GETMET	R8	R1	K26
      0x5828001B,  //  008B  LDCONST	R10	K27
      0x7C200400,  //  008C  CALL	R8	2
      0x70020014,  //  008D  JMP		#00A3
      0x88200113,  //  008E  GETMBR	R8	R0	K19
      0x88201128,  //  008F  GETMBR	R8	R8	K40
      0x4C240000,  //  0090  LDNIL	R9
      0x20201009,  //  0091  NE	R8	R8	R9
      0x20200E08,  //  0092  NE	R8	R7	R8
      0x7822000B,  //  0093  JMPF	R8	#00A0
      0x781E0003,  //  0094  JMPF	R7	#0099
      0x88200113,  //  0095  GETMBR	R8	R0	K19
      0x8C201129,  //  0096  GETMET	R8	R8	K41
      0x7C200200,  //  0097  CALL	R8	1
      0x70020002,  //  0098  JMP		#009C
      0x88200113,  //  0099  GETMBR	R8	R0	K19
      0x8C20112A,  //  009A  GETMET	R8	R8	K42
      0x7C200200,  //  009B  CALL	R8	1
      0x8C20031A,  //  009C  GETMET	R8	R1	K26
      0x5828002B,  //  009D  LDCONST	R10	K43
      0x7C200400,  //  009E  CALL	R8	2
      0x70020002,  //  009F  JMP		#00A3
      0x8C20031A,  //  00A0  GETMET	R8	R1	K26
      0x5828002B,  //  00A1  LDCONST	R10	K43
      0x7C200400,  //  00A2  CALL	R8	2
      0x700201D7,  //  00A3  JMP		#027C
      0x8C18030E,  //  00A4  GETMET	R6	R1	K14
      0x5820002C,  //  00A5  LDCONST	R8	K44
      0x7C180400,  //  00A6  CALL	R6	2
      0x781A0026,  //  00A7  JMPF	R6	#00CF
      0xB81A1000,  //  00A8  GETNGBL	R6	K8
      0x8C180D09,  //  00A9  GETMET	R6	R6	K9
      0x60200018,  //  00AA  GETGBL	R8	G24
      0x58240011,  //  00AB  LDCONST	R9	K17
      0x5828002C,  //  00AC  LDCONST	R10	K44
      0x7C200400,  //  00AD  CALL	R8	2
      0x58240012,  //  00AE  LDCONST	R9	K18
      0x7C180600,  //  00AF  CALL	R6	3
      0x60180009,  //  00B0  GETGBL	R6	G9
      0x8C1C030C,  //  00B1  GETMET	R7	R1	K12
      0x5824002C,  //  00B2  LDCONST	R9	K44
      0x7C1C0400,  //  00B3  CALL	R7	2
      0x7C180200,  //  00B4  CALL	R6	1
      0x581C0005,  //  00B5  LDCONST	R7	K5
      0x88200113,  //  00B6  GETMBR	R8	R0	K19
      0x8820112D,  //  00B7  GETMBR	R8	R8	K45
      0x8820112E,  //  00B8  GETMBR	R8	R8	K46
      0x6024000C,  //  00B9  GETGBL	R9	G12
      0x5C281000,  //  00BA  MOVE	R10	R8
      0x7C240200,  //  00BB  CALL	R9	1
      0x14240E09,  //  00BC  LT	R9	R7	R9
      0x7826000C,  //  00BD  JMPF	R9	#00CB
      0x94241007,  //  00BE  GETIDX	R9	R8	R7
      0x8C24132F,  //  00BF  GETMET	R9	R9	K47
      0x7C240200,  //  00C0  CALL	R9	1
      0x1C241206,  //  00C1  EQ	R9	R9	R6
      0x78260005,  //  00C2  JMPF	R9	#00C9
      0x88240113,  //  00C3  GETMBR	R9	R0	K19
      0x8C241330,  //  00C4  GETMET	R9	R9	K48
      0x942C1007,  //  00C5  GETIDX	R11	R8	R7
      0x7C240400,  //  00C6  CALL	R9	2
      0x70020002,  //  00C7  JMP		#00CB
      0x70020000,  //  00C8  JMP		#00CA
      0x001C0F07,  //  00C9  ADD	R7	R7	K7
      0x7001FFED,  //  00CA  JMP		#00B9
      0x8C24031A,  //  00CB  GETMET	R9	R1	K26
      0x582C0031,  //  00CC  LDCONST	R11	K49
      0x7C240400,  //  00CD  CALL	R9	2
      0x700201AC,  //  00CE  JMP		#027C
      0x8C18030E,  //  00CF  GETMET	R6	R1	K14
      0x58200032,  //  00D0  LDCONST	R8	K50
      0x7C180400,  //  00D1  CALL	R6	2
      0x781A0011,  //  00D2  JMPF	R6	#00E5
      0xB81A1000,  //  00D3  GETNGBL	R6	K8
      0x8C180D09,  //  00D4  GETMET	R6	R6	K9
      0x60200018,  //  00D5  GETGBL	R8	G24
      0x58240011,  //  00D6  LDCONST	R9	K17
      0x58280032,  //  00D7  LDCONST	R10	K50
      0x7C200400,  //  00D8  CALL	R8	2
      0x58240012,  //  00D9  LDCONST	R9	K18
      0x7C180600,  //  00DA  CALL	R6	3
      0x88180113,  //  00DB  GETMBR	R6	R0	K19
      0x501C0000,  //  00DC  LDBOOL	R7	0	0
      0x901A6607,  //  00DD  SETMBR	R6	K51	R7
      0x88180113,  //  00DE  GETMBR	R6	R0	K19
      0x8C180D19,  //  00DF  GETMET	R6	R6	K25
      0x7C180200,  //  00E0  CALL	R6	1
      0x8C18031A,  //  00E1  GETMET	R6	R1	K26
      0x5820001B,  //  00E2  LDCONST	R8	K27
      0x7C180400,  //  00E3  CALL	R6	2
      0x70020196,  //  00E4  JMP		#027C
      0x8C18030E,  //  00E5  GETMET	R6	R1	K14
      0x58200034,  //  00E6  LDCONST	R8	K52
      0x7C180400,  //  00E7  CALL	R6	2
      0x781A00D6,  //  00E8  JMPF	R6	#01C0
      0xB81A1000,  //  00E9  GETNGBL	R6	K8
      0x8C180D09,  //  00EA  GETMET	R6	R6	K9
      0x60200018,  //  00EB  GETGBL	R8	G24
      0x58240011,  //  00EC  LDCONST	R9	K17
      0x58280034,  //  00ED  LDCONST	R10	K52
      0x7C200400,  //  00EE  CALL	R8	2
      0x58240012,  //  00EF  LDCONST	R9	K18
      0x7C180600,  //  00F0  CALL	R6	3
      0x50180000,  //  00F1  LDBOOL	R6	0	0
      0x601C0010,  //  00F2  GETGBL	R7	G16
      0x8C200306,  //  00F3  GETMET	R8	R1	K6
      0x7C200200,  //  00F4  CALL	R8	1
      0x04201107,  //  00F5  SUB	R8	R8	K7
      0x40220A08,  //  00F6  CONNECT	R8	K5	R8
      0x7C1C0200,  //  00F7  CALL	R7	1
      0xA80200A0,  //  00F8  EXBLK	0	#019A
      0x5C200E00,  //  00F9  MOVE	R8	R7
      0x7C200000,  //  00FA  CALL	R8	0
      0x8C24030B,  //  00FB  GETMET	R9	R1	K11
      0x5C2C1000,  //  00FC  MOVE	R11	R8
      0x7C240400,  //  00FD  CALL	R9	2
      0x8C280535,  //  00FE  GETMET	R10	R2	K53
      0x5C301200,  //  00FF  MOVE	R12	R9
      0x5834000C,  //  0100  LDCONST	R13	K12
      0x7C280600,  //  0101  CALL	R10	3
      0x1C281505,  //  0102  EQ	R10	R10	K5
      0x782A005B,  //  0103  JMPF	R10	#0160
      0x60280009,  //  0104  GETGBL	R10	G9
      0x402E2536,  //  0105  CONNECT	R11	K18	K54
      0x942C120B,  //  0106  GETIDX	R11	R9	R11
      0x7C280200,  //  0107  CALL	R10	1
      0x8C2C030C,  //  0108  GETMET	R11	R1	K12
      0x5C341000,  //  0109  MOVE	R13	R8
      0x7C2C0400,  //  010A  CALL	R11	2
      0x88300113,  //  010B  GETMBR	R12	R0	K19
      0x88301937,  //  010C  GETMBR	R12	R12	K55
      0x8C301935,  //  010D  GETMET	R12	R12	K53
      0x60380008,  //  010E  GETGBL	R14	G8
      0x5C3C1400,  //  010F  MOVE	R15	R10
      0x7C380200,  //  0110  CALL	R14	1
      0x7C300400,  //  0111  CALL	R12	2
      0x4C340000,  //  0112  LDNIL	R13
      0x2034180D,  //  0113  NE	R13	R12	R13
      0x78360041,  //  0114  JMPF	R13	#0157
      0x88340113,  //  0115  GETMBR	R13	R0	K19
      0x88341B38,  //  0116  GETMBR	R13	R13	K56
      0x8C341B35,  //  0117  GETMET	R13	R13	K53
      0x8C3C1935,  //  0118  GETMET	R15	R12	K53
      0x58440039,  //  0119  LDCONST	R17	K57
      0x5848003A,  //  011A  LDCONST	R18	K58
      0x7C3C0600,  //  011B  CALL	R15	3
      0x7C340400,  //  011C  CALL	R13	2
      0x4C380000,  //  011D  LDNIL	R14
      0x20381A0E,  //  011E  NE	R14	R13	R14
      0x783A0035,  //  011F  JMPF	R14	#0156
      0xB83A1000,  //  0120  GETNGBL	R14	K8
      0x8C381D09,  //  0121  GETMET	R14	R14	K9
      0x60400018,  //  0122  GETGBL	R16	G24
      0x5844003B,  //  0123  LDCONST	R17	K59
      0x5C481400,  //  0124  MOVE	R18	R10
      0x5C4C1600,  //  0125  MOVE	R19	R11
      0x7C400600,  //  0126  CALL	R16	3
      0x58440012,  //  0127  LDCONST	R17	K18
      0x7C380600,  //  0128  CALL	R14	3
      0x8C381B3C,  //  0129  GETMET	R14	R13	K60
      0x5C401A00,  //  012A  MOVE	R16	R13
      0x5C441800,  //  012B  MOVE	R17	R12
      0x7C380600,  //  012C  CALL	R14	3
      0x203C1C0B,  //  012D  NE	R15	R14	R11
      0xB8421000,  //  012E  GETNGBL	R16	K8
      0x8C402109,  //  012F  GETMET	R16	R16	K9
      0x60480018,  //  0130  GETGBL	R18	G24
      0x584C003D,  //  0131  LDCONST	R19	K61
      0x5C501400,  //  0132  MOVE	R20	R10
      0x5C541C00,  //  0133  MOVE	R21	R14
      0x5C581600,  //  0134  MOVE	R22	R11
      0x205C1C0B,  //  0135  NE	R23	R14	R11
      0x785E0001,  //  0136  JMPF	R23	#0139
      0x585C003E,  //  0137  LDCONST	R23	K62
      0x70020000,  //  0138  JMP		#013A
      0x585C003A,  //  0139  LDCONST	R23	K58
      0x7C480A00,  //  013A  CALL	R18	5
      0x584C0012,  //  013B  LDCONST	R19	K18
      0x7C400600,  //  013C  CALL	R16	3
      0x783E0017,  //  013D  JMPF	R15	#0156
      0x50180200,  //  013E  LDBOOL	R6	1	0
      0x8C401B3F,  //  013F  GETMET	R16	R13	K63
      0x5C481A00,  //  0140  MOVE	R18	R13
      0x5C4C1800,  //  0141  MOVE	R19	R12
      0x5C501600,  //  0142  MOVE	R20	R11
      0x7C400800,  //  0143  CALL	R16	4
      0x88400113,  //  0144  GETMBR	R16	R0	K19
      0x8C402140,  //  0145  GETMET	R16	R16	K64
      0x5C481400,  //  0146  MOVE	R18	R10
      0x7C400400,  //  0147  CALL	R16	2
      0x7842000C,  //  0148  JMPF	R16	#0156
      0xB8461000,  //  0149  GETNGBL	R17	K8
      0x8C442309,  //  014A  GETMET	R17	R17	K9
      0x604C0018,  //  014B  GETGBL	R19	G24
      0x58500041,  //  014C  LDCONST	R20	K65
      0x5C541800,  //  014D  MOVE	R21	R12
      0x5C581400,  //  014E  MOVE	R22	R10
      0x5C5C2000,  //  014F  MOVE	R23	R16
      0x7C4C0800,  //  0150  CALL	R19	4
      0x58500012,  //  0151  LDCONST	R20	K18
      0x7C440600,  //  0152  CALL	R17	3
      0x8C442142,  //  0153  GETMET	R17	R16	K66
      0x5C4C1800,  //  0154  MOVE	R19	R12
      0x7C440400,  //  0155  CALL	R17	2
      0x70020007,  //  0156  JMP		#015F
      0xB8361000,  //  0157  GETNGBL	R13	K8
      0x8C341B09,  //  0158  GETMET	R13	R13	K9
      0x603C0018,  //  0159  GETGBL	R15	G24
      0x58400043,  //  015A  LDCONST	R16	K67
      0x5C441400,  //  015B  MOVE	R17	R10
      0x7C3C0400,  //  015C  CALL	R15	2
      0x58400012,  //  015D  LDCONST	R16	K18
      0x7C340600,  //  015E  CALL	R13	3
      0x70020038,  //  015F  JMP		#0199
      0x8C280535,  //  0160  GETMET	R10	R2	K53
      0x5C301200,  //  0161  MOVE	R12	R9
      0x58340044,  //  0162  LDCONST	R13	K68
      0x7C280600,  //  0163  CALL	R10	3
      0x1C281505,  //  0164  EQ	R10	R10	K5
      0x782A0032,  //  0165  JMPF	R10	#0199
      0x60280009,  //  0166  GETGBL	R10	G9
      0x402E2536,  //  0167  CONNECT	R11	K18	K54
      0x942C120B,  //  0168  GETIDX	R11	R9	R11
      0x7C280200,  //  0169  CALL	R10	1
      0x8C2C030C,  //  016A  GETMET	R11	R1	K12
      0x5C341000,  //  016B  MOVE	R13	R8
      0x7C2C0400,  //  016C  CALL	R11	2
      0x88300113,  //  016D  GETMBR	R12	R0	K19
      0x88301937,  //  016E  GETMBR	R12	R12	K55
      0x8C301935,  //  016F  GETMET	R12	R12	K53
      0x60380008,  //  0170  GETGBL	R14	G8
      0x5C3C1400,  //  0171  MOVE	R15	R10
      0x7C380200,  //  0172  CALL	R14	1
      0x7C300400,  //  0173  CALL	R12	2
      0x4C340000,  //  0174  LDNIL	R13
      0x2034180D,  //  0175  NE	R13	R12	R13
      0x78360021,  //  0176  JMPF	R13	#0199
      0x8C341935,  //  0177  GETMET	R13	R12	K53
      0x583C0045,  //  0178  LDCONST	R15	K69
      0x5840003A,  //  0179  LDCONST	R16	K58
      0x7C340600,  //  017A  CALL	R13	3
      0x20381A0B,  //  017B  NE	R14	R13	R11
      0x783A001B,  //  017C  JMPF	R14	#0199
      0x50180200,  //  017D  LDBOOL	R6	1	0
      0x883C0113,  //  017E  GETMBR	R15	R0	K19
      0x8C3C1F40,  //  017F  GETMET	R15	R15	K64
      0x5C441400,  //  0180  MOVE	R17	R10
      0x7C3C0400,  //  0181  CALL	R15	2
      0x783E0015,  //  0182  JMPF	R15	#0199
      0x8C401F46,  //  0183  GETMET	R16	R15	K70
      0x5C481600,  //  0184  MOVE	R18	R11
      0x7C400400,  //  0185  CALL	R16	2
      0x782E0001,  //  0186  JMPF	R11	#0189
      0x98328A0B,  //  0187  SETIDX	R12	K69	R11
      0x70020002,  //  0188  JMP		#018C
      0x8C401947,  //  0189  GETMET	R16	R12	K71
      0x58480045,  //  018A  LDCONST	R18	K69
      0x7C400400,  //  018B  CALL	R16	2
      0xB8421000,  //  018C  GETNGBL	R16	K8
      0x8C402109,  //  018D  GETMET	R16	R16	K9
      0x60480018,  //  018E  GETGBL	R18	G24
      0x584C0048,  //  018F  LDCONST	R19	K72
      0x5C501800,  //  0190  MOVE	R20	R12
      0x5C541400,  //  0191  MOVE	R21	R10
      0x5C581E00,  //  0192  MOVE	R22	R15
      0x7C480800,  //  0193  CALL	R18	4
      0x584C0012,  //  0194  LDCONST	R19	K18
      0x7C400600,  //  0195  CALL	R16	3
      0x8C401F42,  //  0196  GETMET	R16	R15	K66
      0x5C481800,  //  0197  MOVE	R18	R12
      0x7C400400,  //  0198  CALL	R16	2
      0x7001FF5E,  //  0199  JMP		#00F9
      0x581C000D,  //  019A  LDCONST	R7	K13
      0xAC1C0200,  //  019B  CATCH	R7	1	0
      0xB0080000,  //  019C  RAISE	2	R0	R0
      0xB81E1000,  //  019D  GETNGBL	R7	K8
      0x8C1C0F09,  //  019E  GETMET	R7	R7	K9
      0x60240018,  //  019F  GETGBL	R9	G24
      0x58280049,  //  01A0  LDCONST	R10	K73
      0x602C0008,  //  01A1  GETGBL	R11	G8
      0x88300113,  //  01A2  GETMBR	R12	R0	K19
      0x88301937,  //  01A3  GETMBR	R12	R12	K55
      0x7C2C0200,  //  01A4  CALL	R11	1
      0x7C240400,  //  01A5  CALL	R9	2
      0x58280012,  //  01A6  LDCONST	R10	K18
      0x7C1C0600,  //  01A7  CALL	R7	3
      0x78160008,  //  01A8  JMPF	R5	#01B2
      0xB81E1000,  //  01A9  GETNGBL	R7	K8
      0x8C1C0F09,  //  01AA  GETMET	R7	R7	K9
      0x60240018,  //  01AB  GETGBL	R9	G24
      0x5828004A,  //  01AC  LDCONST	R10	K74
      0x5C2C0A00,  //  01AD  MOVE	R11	R5
      0x7C240400,  //  01AE  CALL	R9	2
      0x58280012,  //  01AF  LDCONST	R10	K18
      0x7C1C0600,  //  01B0  CALL	R7	3
      0x7002000C,  //  01B1  JMP		#01BF
      0x741A0002,  //  01B2  JMPT	R6	#01B6
      0x881C0113,  //  01B3  GETMBR	R7	R0	K19
      0x881C0F33,  //  01B4  GETMBR	R7	R7	K51
      0x741E0005,  //  01B5  JMPT	R7	#01BC
      0x881C0113,  //  01B6  GETMBR	R7	R0	K19
      0x50200200,  //  01B7  LDBOOL	R8	1	0
      0x901E6608,  //  01B8  SETMBR	R7	K51	R8
      0x881C0113,  //  01B9  GETMBR	R7	R0	K19
      0x8C1C0F19,  //  01BA  GETMET	R7	R7	K25
      0x7C1C0200,  //  01BB  CALL	R7	1
      0x8C1C031A,  //  01BC  GETMET	R7	R1	K26
      0x58240031,  //  01BD  LDCONST	R9	K49
      0x7C1C0400,  //  01BE  CALL	R7	2
      0x700200BB,  //  01BF  JMP		#027C
      0x8C18030E,  //  01C0  GETMET	R6	R1	K14
      0x5820004B,  //  01C1  LDCONST	R8	K75
      0x7C180400,  //  01C2  CALL	R6	2
      0x781A002B,  //  01C3  JMPF	R6	#01F0
      0x8C18030C,  //  01C4  GETMET	R6	R1	K12
      0x5820004C,  //  01C5  LDCONST	R8	K76
      0x7C180400,  //  01C6  CALL	R6	2
      0x8C1C030C,  //  01C7  GETMET	R7	R1	K12
      0x5824000C,  //  01C8  LDCONST	R9	K12
      0x7C1C0400,  //  01C9  CALL	R7	2
      0x8C20030C,  //  01CA  GETMET	R8	R1	K12
      0x58280044,  //  01CB  LDCONST	R10	K68
      0x7C200400,  //  01CC  CALL	R8	2
      0xB8261000,  //  01CD  GETNGBL	R9	K8
      0x8C241309,  //  01CE  GETMET	R9	R9	K9
      0x602C0018,  //  01CF  GETGBL	R11	G24
      0x5830004D,  //  01D0  LDCONST	R12	K77
      0x5C340C00,  //  01D1  MOVE	R13	R6
      0x5C380E00,  //  01D2  MOVE	R14	R7
      0x7C2C0600,  //  01D3  CALL	R11	3
      0x58300012,  //  01D4  LDCONST	R12	K18
      0x7C240600,  //  01D5  CALL	R9	3
      0x88240113,  //  01D6  GETMBR	R9	R0	K19
      0x88241338,  //  01D7  GETMBR	R9	R9	K56
      0x8C241335,  //  01D8  GETMET	R9	R9	K53
      0x5C2C0C00,  //  01D9  MOVE	R11	R6
      0x7C240400,  //  01DA  CALL	R9	2
      0x4C280000,  //  01DB  LDNIL	R10
      0x2028120A,  //  01DC  NE	R10	R9	R10
      0x782A000D,  //  01DD  JMPF	R10	#01EC
      0x60280013,  //  01DE  GETGBL	R10	G19
      0x7C280000,  //  01DF  CALL	R10	0
      0x78220000,  //  01E0  JMPF	R8	#01E2
      0x982A8A08,  //  01E1  SETIDX	R10	K69	R8
      0x8C2C133F,  //  01E2  GETMET	R11	R9	K63
      0x5C341200,  //  01E3  MOVE	R13	R9
      0x5C381400,  //  01E4  MOVE	R14	R10
      0x5C3C0E00,  //  01E5  MOVE	R15	R7
      0x7C2C0800,  //  01E6  CALL	R11	4
      0x882C0113,  //  01E7  GETMBR	R11	R0	K19
      0x8C2C174E,  //  01E8  GETMET	R11	R11	K78
      0x5C340C00,  //  01E9  MOVE	R13	R6
      0x5C381400,  //  01EA  MOVE	R14	R10
      0x7C2C0600,  //  01EB  CALL	R11	3
      0x8C28031A,  //  01EC  GETMET	R10	R1	K26
      0x58300031,  //  01ED  LDCONST	R12	K49
      0x7C280400,  //  01EE  CALL	R10	2
      0x7002008B,  //  01EF  JMP		#027C
      0x8C18030E,  //  01F0  GETMET	R6	R1	K14
      0x5820004F,  //  01F1  LDCONST	R8	K79
      0x7C180400,  //  01F2  CALL	R6	2
      0x781A005E,  //  01F3  JMPF	R6	#0253
      0x8C18030C,  //  01F4  GETMET	R6	R1	K12
      0x58200050,  //  01F5  LDCONST	R8	K80
      0x7C180400,  //  01F6  CALL	R6	2
      0x4C1C0000,  //  01F7  LDNIL	R7
      0x1C1C0C07,  //  01F8  EQ	R7	R6	R7
      0x741E0001,  //  01F9  JMPT	R7	#01FC
      0x1C1C0D3A,  //  01FA  EQ	R7	R6	K58
      0x781E0000,  //  01FB  JMPF	R7	#01FD
      0xB006A352,  //  01FC  RAISE	1	K81	K82
      0x581C0005,  //  01FD  LDCONST	R7	K5
      0x60200008,  //  01FE  GETGBL	R8	G8
      0x5C240E00,  //  01FF  MOVE	R9	R7
      0x7C200200,  //  0200  CALL	R8	1
      0x8C24030E,  //  0201  GETMET	R9	R1	K14
      0x002E9808,  //  0202  ADD	R11	K76	R8
      0x7C240400,  //  0203  CALL	R9	2
      0x78260049,  //  0204  JMPF	R9	#024F
      0x8C24030C,  //  0205  GETMET	R9	R1	K12
      0x002E9808,  //  0206  ADD	R11	K76	R8
      0x7C240400,  //  0207  CALL	R9	2
      0x8C28030C,  //  0208  GETMET	R10	R1	K12
      0x00321808,  //  0209  ADD	R12	K12	R8
      0x7C280400,  //  020A  CALL	R10	2
      0x8C2C030C,  //  020B  GETMET	R11	R1	K12
      0x00368808,  //  020C  ADD	R13	K68	R8
      0x7C2C0400,  //  020D  CALL	R11	2
      0x2030133A,  //  020E  NE	R12	R9	K58
      0x78320038,  //  020F  JMPF	R12	#0249
      0x88300113,  //  0210  GETMBR	R12	R0	K19
      0x88301938,  //  0211  GETMBR	R12	R12	K56
      0x8C301935,  //  0212  GETMET	R12	R12	K53
      0x5C381200,  //  0213  MOVE	R14	R9
      0x7C300400,  //  0214  CALL	R12	2
      0x4C340000,  //  0215  LDNIL	R13
      0x2034180D,  //  0216  NE	R13	R12	R13
      0x78360030,  //  0217  JMPF	R13	#0249
      0x60340013,  //  0218  GETGBL	R13	G19
      0x7C340000,  //  0219  CALL	R13	0
      0x9836A006,  //  021A  SETIDX	R13	K80	R6
      0x98367209,  //  021B  SETIDX	R13	K57	R9
      0x782E0000,  //  021C  JMPF	R11	#021E
      0x98368A0B,  //  021D  SETIDX	R13	K69	R11
      0x8C38193F,  //  021E  GETMET	R14	R12	K63
      0x5C401800,  //  021F  MOVE	R16	R12
      0x5C441A00,  //  0220  MOVE	R17	R13
      0x5C481400,  //  0221  MOVE	R18	R10
      0x7C380800,  //  0222  CALL	R14	4
      0x50380000,  //  0223  LDBOOL	R14	0	0
      0x603C0010,  //  0224  GETGBL	R15	G16
      0x88400113,  //  0225  GETMBR	R16	R0	K19
      0x88402137,  //  0226  GETMBR	R16	R16	K55
      0x7C3C0200,  //  0227  CALL	R15	1
      0xA802000B,  //  0228  EXBLK	0	#0235
      0x5C401E00,  //  0229  MOVE	R16	R15
      0x7C400000,  //  022A  CALL	R16	0
      0x8C440153,  //  022B  GETMET	R17	R0	K83
      0x5C4C2000,  //  022C  MOVE	R19	R16
      0x5C501A00,  //  022D  MOVE	R20	R13
      0x7C440600,  //  022E  CALL	R17	3
      0x78460001,  //  022F  JMPF	R17	#0232
      0x50380200,  //  0230  LDBOOL	R14	1	0
      0x70020000,  //  0231  JMP		#0233
      0x7001FFF5,  //  0232  JMP		#0229
      0xA8040001,  //  0233  EXBLK	1	1
      0x70020002,  //  0234  JMP		#0238
      0x583C000D,  //  0235  LDCONST	R15	K13
      0xAC3C0200,  //  0236  CATCH	R15	1	0
      0xB0080000,  //  0237  RAISE	2	R0	R0
      0x5C3C1C00,  //  0238  MOVE	R15	R14
      0x743E000E,  //  0239  JMPT	R15	#0249
      0xB83E1000,  //  023A  GETNGBL	R15	K8
      0x8C3C1F09,  //  023B  GETMET	R15	R15	K9
      0x60440018,  //  023C  GETGBL	R17	G24
      0x58480054,  //  023D  LDCONST	R18	K84
      0x5C4C0C00,  //  023E  MOVE	R19	R6
      0x5C501200,  //  023F  MOVE	R20	R9
      0x5C541400,  //  0240  MOVE	R21	R10
      0x7C440800,  //  0241  CALL	R17	4
      0x58480012,  //  0242  LDCONST	R18	K18
      0x7C3C0600,  //  0243  CALL	R15	3
      0x883C0113,  //  0244  GETMBR	R15	R0	K19
      0x8C3C1F4E,  //  0245  GETMET	R15	R15	K78
      0x5C441200,  //  0246  MOVE	R17	R9
      0x5C481A00,  //  0247  MOVE	R18	R13
      0x7C3C0600,  //  0248  CALL	R15	3
      0x001C0F07,  //  0249  ADD	R7	R7	K7
      0x60300008,  //  024A  GETGBL	R12	G8
      0x5C340E00,  //  024B  MOVE	R13	R7
      0x7C300200,  //  024C  CALL	R12	1
      0x5C201800,  //  024D  MOVE	R8	R12
      0x7001FFB1,  //  024E  JMP		#0201
      0x8C24031A,  //  024F  GETMET	R9	R1	K26
      0x582C0031,  //  0250  LDCONST	R11	K49
      0x7C240400,  //  0251  CALL	R9	2
      0x70020028,  //  0252  JMP		#027C
      0x4C180000,  //  0253  LDNIL	R6
      0x601C0010,  //  0254  GETGBL	R7	G16
      0x8C200306,  //  0255  GETMET	R8	R1	K6
      0x7C200200,  //  0256  CALL	R8	1
      0x04201107,  //  0257  SUB	R8	R8	K7
      0x40220A08,  //  0258  CONNECT	R8	K5	R8
      0x7C1C0200,  //  0259  CALL	R7	1
      0xA8020013,  //  025A  EXBLK	0	#026F
      0x5C200E00,  //  025B  MOVE	R8	R7
      0x7C200000,  //  025C  CALL	R8	0
      0x8C24030B,  //  025D  GETMET	R9	R1	K11
      0x5C2C1000,  //  025E  MOVE	R11	R8
      0x7C240400,  //  025F  CALL	R9	2
      0x8C280535,  //  0260  GETMET	R10	R2	K53
      0x5C301200,  //  0261  MOVE	R12	R9
      0x58340055,  //  0262  LDCONST	R13	K85
      0x7C280600,  //  0263  CALL	R10	3
      0x1C281505,  //  0264  EQ	R10	R10	K5
      0x782A0005,  //  0265  JMPF	R10	#026C
      0x60280009,  //  0266  GETGBL	R10	G9
      0x402E2536,  //  0267  CONNECT	R11	K18	K54
      0x942C120B,  //  0268  GETIDX	R11	R9	R11
      0x7C280200,  //  0269  CALL	R10	1
      0x5C181400,  //  026A  MOVE	R6	R10
      0x70020000,  //  026B  JMP		#026D
      0x7001FFED,  //  026C  JMP		#025B
      0xA8040001,  //  026D  EXBLK	1	1
      0x70020002,  //  026E  JMP		#0272
      0x581C000D,  //  026F  LDCONST	R7	K13
      0xAC1C0200,  //  0270  CATCH	R7	1	0
      0xB0080000,  //  0271  RAISE	2	R0	R0
      0x4C1C0000,  //  0272  LDNIL	R7
      0x201C0C07,  //  0273  NE	R7	R6	R7
      0x781E0006,  //  0274  JMPF	R7	#027C
      0x881C0113,  //  0275  GETMBR	R7	R0	K19
      0x8C1C0F56,  //  0276  GETMET	R7	R7	K86
      0x5C240C00,  //  0277  MOVE	R9	R6
      0x7C1C0400,  //  0278  CALL	R7	2
      0x8C1C031A,  //  0279  GETMET	R7	R1	K26
      0x58240031,  //  027A  LDCONST	R9	K49
      0x7C1C0400,  //  027B  CALL	R7	2
      0x78160011,  //  027C  JMPF	R5	#028F
      0x8C180357,  //  027D  GETMET	R6	R1	K87
      0x58200058,  //  027E  LDCONST	R8	K88
      0x7C180400,  //  027F  CALL	R6	2
      0x8C180359,  //  0280  GETMET	R6	R1	K89
      0x7C180200,  //  0281  CALL	R6	1
      0x8C18035A,  //  0282  GETMET	R6	R1	K90
      0x60200018,  //  0283  GETGBL	R8	G24
      0x5824005B,  //  0284  LDCONST	R9	K91
      0x8C28035C,  //  0285  GETMET	R10	R1	K92
      0x5C300A00,  //  0286  MOVE	R12	R5
      0x7C280400,  //  0287  CALL	R10	2
      0x7C200400,  //  0288  CALL	R8	2
      0x7C180400,  //  0289  CALL	R6	2
      0x8C18035D,  //  028A  GETMET	R6	R1	K93
      0x8820035E,  //  028B  GETMBR	R8	R1	K94
      0x7C180400,  //  028C  CALL	R6	2
      0x8C18035F,  //  028D  GETMET	R6	R1	K95
      0x7C180200,  //  028E  CALL	R6	1
      0xA8040001,  //  028F  EXBLK	1	1
      0x7002001D,  //  0290  JMP		#02AF
      0xAC180002,  //  0291  CATCH	R6	0	2
      0x7002001A,  //  0292  JMP		#02AE
      0xB8221000,  //  0293  GETNGBL	R8	K8
      0x8C201109,  //  0294  GETMET	R8	R8	K9
      0x60280018,  //  0295  GETGBL	R10	G24
      0x582C0060,  //  0296  LDCONST	R11	K96
      0x5C300C00,  //  0297  MOVE	R12	R6
      0x5C340E00,  //  0298  MOVE	R13	R7
      0x7C280600,  //  0299  CALL	R10	3
      0x582C0061,  //  029A  LDCONST	R11	K97
      0x7C200600,  //  029B  CALL	R8	3
      0x8C200357,  //  029C  GETMET	R8	R1	K87
      0x58280058,  //  029D  LDCONST	R10	K88
      0x7C200400,  //  029E  CALL	R8	2
      0x8C200359,  //  029F  GETMET	R8	R1	K89
      0x7C200200,  //  02A0  CALL	R8	1
      0x8C20035A,  //  02A1  GETMET	R8	R1	K90
      0x60280018,  //  02A2  GETGBL	R10	G24
      0x582C0062,  //  02A3  LDCONST	R11	K98
      0x5C300C00,  //  02A4  MOVE	R12	R6
      0x5C340E00,  //  02A5  MOVE	R13	R7
      0x7C280600,  //  02A6  CALL	R10	3
      0x7C200400,  //  02A7  CALL	R8	2
      0x8C20035D,  //  02A8  GETMET	R8	R1	K93
      0x8828035E,  //  02A9  GETMBR	R10	R1	K94
      0x7C200400,  //  02AA  CALL	R8	2
      0x8C20035F,  //  02AB  GETMET	R8	R1	K95
      0x7C200200,  //  02AC  CALL	R8	1
      0x70020000,  //  02AD  JMP		#02AF
      0xB0080000,  //  02AE  RAISE	2	R0	R0
      0x80000000,  //  02AF  RET	0
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
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
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
    /* K12  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27save_X27_X20class_X3D_X27button_X20bgrn_X27_X3ESave_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_enable),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
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
      0x78120010,  //  0016  JMPF	R4	#0028
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
      0x8C100302,  //  0028  GETMET	R4	R1	K2
      0x5818000C,  //  0029  LDCONST	R6	K12
      0x7C100400,  //  002A  CALL	R4	2
      0x80000000,  //  002B  RET	0
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
        { be_const_key_weak(_CLASSES_TYPES, 4), be_nested_str_weak(_X7Crelay_X7Clight0_X7Clight1_X7Clight2_X7Clight3_X7Cshutter_X7Cshutter_X2Btilt_X7Ctemperature_X7Cpressure_X7Cilluminance_X7Chumidity_X7Coccupancy_X7Conoff_X7Ccontact) },
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
