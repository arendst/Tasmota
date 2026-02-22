/* Solidification of Matter_UI.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// ktab too big for class 'Matter_UI' - skipping

extern const bclass be_class_Matter_UI;

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
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x88080106,  //  000D  GETMBR	R2	R0	K6
      0x780A0001,  //  000E  JMPF	R2	#0011
      0x8C080107,  //  000F  GETMET	R2	R0	K7
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080308,  //  0011  GETMET	R2	R1	K8
      0x58100009,  //  0012  LDCONST	R4	K9
      0x7C080400,  //  0013  CALL	R2	2
      0x8C08030A,  //  0014  GETMET	R2	R1	K10
      0x8810030B,  //  0015  GETMBR	R4	R1	K11
      0x7C080400,  //  0016  CALL	R2	2
      0x8C08030C,  //  0017  GETMET	R2	R1	K12
      0x7C080200,  //  0018  CALL	R2	1
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: extract_arg_name
********************************************************************/
be_local_closure(class_Matter_UI_extract_arg_name,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(_X7C),
    /* K4   */  be_const_int(0),
    /* K5   */  be_const_int(1),
    }),
    be_str_weak(extract_arg_name),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x740E0001,  //  0003  JMPT	R3	#0006
      0x1C0C0301,  //  0004  EQ	R3	R1	K1
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x4C0C0000,  //  0006  LDNIL	R3
      0x80040600,  //  0007  RET	1	R3
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x5C140200,  //  0009  MOVE	R5	R1
      0x58180003,  //  000A  LDCONST	R6	K3
      0x7C0C0600,  //  000B  CALL	R3	3
      0x24100704,  //  000C  GT	R4	R3	K4
      0x78120004,  //  000D  JMPF	R4	#0013
      0x04100705,  //  000E  SUB	R4	R3	K5
      0x40120804,  //  000F  CONNECT	R4	K4	R4
      0x94100204,  //  0010  GETIDX	R4	R1	R4
      0x80040800,  //  0011  RET	1	R4
      0x70020002,  //  0012  JMP		#0016
      0x14100704,  //  0013  LT	R4	R3	K4
      0x78120000,  //  0014  JMPF	R4	#0016
      0x80040200,  //  0015  RET	1	R1
      0x4C100000,  //  0016  LDNIL	R4
      0x80040800,  //  0017  RET	1	R4
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
    ( &(const bvalue[18]) {     /* constants */
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
    /* K11  */  be_nested_str_weak(_X2Dzigbee),
    /* K12  */  be_nested_str_weak(_X3Coption_X20value_X3D_X27_X27_X20disabled_X3E_X2D_X2D_X2D_X20Zigbee_X20Devices_X20_X2D_X2D_X2D_X3C_X2Foption_X3E),
    /* K13  */  be_nested_str_weak(device),
    /* K14  */  be_nested_str_weak(get_plugin_class_displayname),
    /* K15  */  be_nested_str_weak(_X3Coption_X20value_X3D_X27_X25s_X27_X25s_X3E_X25s_X3C_X2Foption_X3E),
    /* K16  */  be_nested_str_weak(_X20selected),
    /* K17  */  be_const_int(1),
    }),
    be_str_weak(plugin_option),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
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
      0x781E0024,  //  0018  JMPF	R7	#003E
      0x941C0A06,  //  0019  GETIDX	R7	R5	R6
      0x1C200F06,  //  001A  EQ	R8	R7	K6
      0x78220003,  //  001B  JMPF	R8	#0020
      0x8C200707,  //  001C  GETMET	R8	R3	K7
      0x58280008,  //  001D  LDCONST	R10	K8
      0x7C200400,  //  001E  CALL	R8	2
      0x7002001B,  //  001F  JMP		#003C
      0x1C200F09,  //  0020  EQ	R8	R7	K9
      0x78220003,  //  0021  JMPF	R8	#0026
      0x8C200707,  //  0022  GETMET	R8	R3	K7
      0x5828000A,  //  0023  LDCONST	R10	K10
      0x7C200400,  //  0024  CALL	R8	2
      0x70020015,  //  0025  JMP		#003C
      0x1C200F0B,  //  0026  EQ	R8	R7	K11
      0x78220003,  //  0027  JMPF	R8	#002C
      0x8C200707,  //  0028  GETMET	R8	R3	K7
      0x5828000C,  //  0029  LDCONST	R10	K12
      0x7C200400,  //  002A  CALL	R8	2
      0x7002000F,  //  002B  JMP		#003C
      0x8820010D,  //  002C  GETMBR	R8	R0	K13
      0x8C20110E,  //  002D  GETMET	R8	R8	K14
      0x5C280E00,  //  002E  MOVE	R10	R7
      0x7C200400,  //  002F  CALL	R8	2
      0x8C240707,  //  0030  GETMET	R9	R3	K7
      0x602C0018,  //  0031  GETGBL	R11	G24
      0x5830000F,  //  0032  LDCONST	R12	K15
      0x5C340E00,  //  0033  MOVE	R13	R7
      0x1C380E01,  //  0034  EQ	R14	R7	R1
      0x783A0001,  //  0035  JMPF	R14	#0038
      0x58380010,  //  0036  LDCONST	R14	K16
      0x70020000,  //  0037  JMP		#0039
      0x58380006,  //  0038  LDCONST	R14	K6
      0x5C3C1000,  //  0039  MOVE	R15	R8
      0x7C2C0800,  //  003A  CALL	R11	4
      0x7C240400,  //  003B  CALL	R9	2
      0x00180D11,  //  003C  ADD	R6	R6	K17
      0x7001FFD5,  //  003D  JMP		#0014
      0x80000000,  //  003E  RET	0
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
    ( &(const binstruction[53]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x580C0004,  //  0006  LDCONST	R3	K4
      0x70020000,  //  0007  JMP		#0009
      0x580C0005,  //  0008  LDCONST	R3	K5
      0x8C100302,  //  0009  GETMET	R4	R1	K2
      0x60180018,  //  000A  GETGBL	R6	G24
      0x581C0006,  //  000B  LDCONST	R7	K6
      0x5C200600,  //  000C  MOVE	R8	R3
      0x7C180400,  //  000D  CALL	R6	2
      0x7C100400,  //  000E  CALL	R4	2
      0x8C100302,  //  000F  GETMET	R4	R1	K2
      0x58180007,  //  0010  LDCONST	R6	K7
      0x7C100400,  //  0011  CALL	R4	2
      0x780A001D,  //  0012  JMPF	R2	#0031
      0x88100108,  //  0013  GETMBR	R4	R0	K8
      0x88100909,  //  0014  GETMBR	R4	R4	K9
      0x8810090A,  //  0015  GETMBR	R4	R4	K10
      0x4C140000,  //  0016  LDNIL	R5
      0x20100805,  //  0017  NE	R4	R4	R5
      0x78120001,  //  0018  JMPF	R4	#001B
      0x58100004,  //  0019  LDCONST	R4	K4
      0x70020000,  //  001A  JMP		#001C
      0x58100005,  //  001B  LDCONST	R4	K5
      0x8C140302,  //  001C  GETMET	R5	R1	K2
      0x601C0018,  //  001D  GETGBL	R7	G24
      0x5820000B,  //  001E  LDCONST	R8	K11
      0x5C240800,  //  001F  MOVE	R9	R4
      0x7C1C0400,  //  0020  CALL	R7	2
      0x7C140400,  //  0021  CALL	R5	2
      0x8C140302,  //  0022  GETMET	R5	R1	K2
      0x581C000C,  //  0023  LDCONST	R7	K12
      0x7C140400,  //  0024  CALL	R5	2
      0x88140108,  //  0025  GETMBR	R5	R0	K8
      0x88140B0D,  //  0026  GETMBR	R5	R5	K13
      0x78160001,  //  0027  JMPF	R5	#002A
      0x5814000E,  //  0028  LDCONST	R5	K14
      0x70020000,  //  0029  JMP		#002B
      0x58140005,  //  002A  LDCONST	R5	K5
      0x8C180302,  //  002B  GETMET	R6	R1	K2
      0x60200018,  //  002C  GETGBL	R8	G24
      0x5824000F,  //  002D  LDCONST	R9	K15
      0x5C280A00,  //  002E  MOVE	R10	R5
      0x7C200400,  //  002F  CALL	R8	2
      0x7C180400,  //  0030  CALL	R6	2
      0x8C100302,  //  0031  GETMET	R4	R1	K2
      0x58180010,  //  0032  LDCONST	R6	K16
      0x7C100400,  //  0033  CALL	R4	2
      0x80000000,  //  0034  RET	0
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
    ( &(const binstruction[25]) {  /* code */
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
      0x880C0107,  //  000E  GETMBR	R3	R0	K7
      0x780E0002,  //  000F  JMPF	R3	#0013
      0x8C0C0108,  //  0010  GETMET	R3	R0	K8
      0x5C140400,  //  0011  MOVE	R5	R2
      0x7C0C0400,  //  0012  CALL	R3	2
      0x8C0C0309,  //  0013  GETMET	R3	R1	K9
      0x8814030A,  //  0014  GETMBR	R5	R1	K10
      0x7C0C0400,  //  0015  CALL	R3	2
      0x8C0C030B,  //  0016  GETMET	R3	R1	K11
      0x7C0C0200,  //  0017  CALL	R3	1
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: handle_config_json
********************************************************************/
be_local_closure(class_Matter_UI_handle_config_json,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(arg),
    /* K3   */  be_nested_str_weak(config_json),
    /* K4   */  be_nested_str_weak(),
    /* K5   */  be_nested_str_weak(value_error),
    /* K6   */  be_nested_str_weak(Empty_X20configuration),
    /* K7   */  be_nested_str_weak(load),
    /* K8   */  be_nested_str_weak(Invalid_X20JSON_X20configuration),
    /* K9   */  be_nested_str_weak(keys),
    /* K10  */  be_nested_str_weak(find),
    /* K11  */  be_nested_str_weak(type),
    /* K12  */  be_nested_str_weak(Endpoint_X20),
    /* K13  */  be_nested_str_weak(_X20missing_X20type),
    /* K14  */  be_nested_str_weak(device),
    /* K15  */  be_nested_str_weak(plugins_classes),
    /* K16  */  be_nested_str_weak(Unknown_X20type_X20_X27),
    /* K17  */  be_nested_str_weak(_X27_X20for_X20endpoint_X20),
    /* K18  */  be_nested_str_weak(stop_iteration),
    /* K19  */  be_nested_str_weak(plugins_config),
    /* K20  */  be_nested_str_weak(adjust_next_ep),
    /* K21  */  be_nested_str_weak(reconcile_plugins),
    /* K22  */  be_nested_str_weak(plugins_persist),
    /* K23  */  be_nested_str_weak(save_param),
    /* K24  */  be_nested_str_weak(log),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20config_X20updated_X20via_X20config_json),
    /* K26  */  be_const_int(2),
    }),
    be_str_weak(handle_config_json),
    &be_const_str_solidified,
    ( &(const binstruction[73]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x4C100000,  //  0005  LDNIL	R4
      0x1C100604,  //  0006  EQ	R4	R3	R4
      0x74120001,  //  0007  JMPT	R4	#000A
      0x1C100704,  //  0008  EQ	R4	R3	K4
      0x78120000,  //  0009  JMPF	R4	#000B
      0xB0060B06,  //  000A  RAISE	1	K5	K6
      0x8C100507,  //  000B  GETMET	R4	R2	K7
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0x4C140000,  //  000E  LDNIL	R5
      0x1C140805,  //  000F  EQ	R5	R4	R5
      0x78160000,  //  0010  JMPF	R5	#0012
      0xB0060B08,  //  0011  RAISE	1	K5	K8
      0x60140010,  //  0012  GETGBL	R5	G16
      0x8C180909,  //  0013  GETMET	R6	R4	K9
      0x7C180200,  //  0014  CALL	R6	1
      0x7C140200,  //  0015  CALL	R5	1
      0xA8020018,  //  0016  EXBLK	0	#0030
      0x5C180A00,  //  0017  MOVE	R6	R5
      0x7C180000,  //  0018  CALL	R6	0
      0x941C0806,  //  0019  GETIDX	R7	R4	R6
      0x8C200F0A,  //  001A  GETMET	R8	R7	K10
      0x5828000B,  //  001B  LDCONST	R10	K11
      0x7C200400,  //  001C  CALL	R8	2
      0x4C240000,  //  001D  LDNIL	R9
      0x1C241009,  //  001E  EQ	R9	R8	R9
      0x78260002,  //  001F  JMPF	R9	#0023
      0x00261806,  //  0020  ADD	R9	K12	R6
      0x0024130D,  //  0021  ADD	R9	R9	K13
      0xB0060A09,  //  0022  RAISE	1	K5	R9
      0x8824010E,  //  0023  GETMBR	R9	R0	K14
      0x8824130F,  //  0024  GETMBR	R9	R9	K15
      0x8C24130A,  //  0025  GETMET	R9	R9	K10
      0x5C2C1000,  //  0026  MOVE	R11	R8
      0x7C240400,  //  0027  CALL	R9	2
      0x4C280000,  //  0028  LDNIL	R10
      0x1C24120A,  //  0029  EQ	R9	R9	R10
      0x78260003,  //  002A  JMPF	R9	#002F
      0x00262008,  //  002B  ADD	R9	K16	R8
      0x00241311,  //  002C  ADD	R9	R9	K17
      0x00241206,  //  002D  ADD	R9	R9	R6
      0xB0060A09,  //  002E  RAISE	1	K5	R9
      0x7001FFE6,  //  002F  JMP		#0017
      0x58140012,  //  0030  LDCONST	R5	K18
      0xAC140200,  //  0031  CATCH	R5	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x8814010E,  //  0033  GETMBR	R5	R0	K14
      0x88140B13,  //  0034  GETMBR	R5	R5	K19
      0x8818010E,  //  0035  GETMBR	R6	R0	K14
      0x901A2604,  //  0036  SETMBR	R6	K19	R4
      0x8818010E,  //  0037  GETMBR	R6	R0	K14
      0x8C180D14,  //  0038  GETMET	R6	R6	K20
      0x7C180200,  //  0039  CALL	R6	1
      0x8C180115,  //  003A  GETMET	R6	R0	K21
      0x5C200A00,  //  003B  MOVE	R8	R5
      0x5C240800,  //  003C  MOVE	R9	R4
      0x7C180600,  //  003D  CALL	R6	3
      0x8818010E,  //  003E  GETMBR	R6	R0	K14
      0x501C0200,  //  003F  LDBOOL	R7	1	0
      0x901A2C07,  //  0040  SETMBR	R6	K22	R7
      0x8818010E,  //  0041  GETMBR	R6	R0	K14
      0x8C180D17,  //  0042  GETMET	R6	R6	K23
      0x7C180200,  //  0043  CALL	R6	1
      0xB81A3000,  //  0044  GETNGBL	R6	K24
      0x581C0019,  //  0045  LDCONST	R7	K25
      0x5820001A,  //  0046  LDCONST	R8	K26
      0x7C180400,  //  0047  CALL	R6	2
      0x80000000,  //  0048  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_options
********************************************************************/
be_local_closure(class_Matter_UI_parse_options,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(split),
    /* K2   */  be_nested_str_weak(_X2C),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(_X3A),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(push),
    /* K7   */  be_nested_str_weak(value),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(label),
    /* K10  */  be_const_int(2147483647),
    /* K11  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(parse_options),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0012,  //  0001  GETGBL	R3	G18
      0x7C0C0000,  //  0002  CALL	R3	0
      0x8C100501,  //  0003  GETMET	R4	R2	K1
      0x5C180200,  //  0004  MOVE	R6	R1
      0x581C0002,  //  0005  LDCONST	R7	K2
      0x7C100600,  //  0006  CALL	R4	3
      0x60140010,  //  0007  GETGBL	R5	G16
      0x5C180800,  //  0008  MOVE	R6	R4
      0x7C140200,  //  0009  CALL	R5	1
      0xA8020020,  //  000A  EXBLK	0	#002C
      0x5C180A00,  //  000B  MOVE	R6	R5
      0x7C180000,  //  000C  CALL	R6	0
      0x8C1C0503,  //  000D  GETMET	R7	R2	K3
      0x5C240C00,  //  000E  MOVE	R9	R6
      0x58280004,  //  000F  LDCONST	R10	K4
      0x7C1C0600,  //  0010  CALL	R7	3
      0x28200F05,  //  0011  GE	R8	R7	K5
      0x7822000C,  //  0012  JMPF	R8	#0020
      0x8C200706,  //  0013  GETMET	R8	R3	K6
      0x60280013,  //  0014  GETGBL	R10	G19
      0x7C280000,  //  0015  CALL	R10	0
      0x042C0F08,  //  0016  SUB	R11	R7	K8
      0x402E0A0B,  //  0017  CONNECT	R11	K5	R11
      0x942C0C0B,  //  0018  GETIDX	R11	R6	R11
      0x982A0E0B,  //  0019  SETIDX	R10	K7	R11
      0x002C0F08,  //  001A  ADD	R11	R7	K8
      0x402C170A,  //  001B  CONNECT	R11	R11	K10
      0x942C0C0B,  //  001C  GETIDX	R11	R6	R11
      0x982A120B,  //  001D  SETIDX	R10	K9	R11
      0x7C200400,  //  001E  CALL	R8	2
      0x7002000A,  //  001F  JMP		#002B
      0x6020000C,  //  0020  GETGBL	R8	G12
      0x5C240C00,  //  0021  MOVE	R9	R6
      0x7C200200,  //  0022  CALL	R8	1
      0x24201105,  //  0023  GT	R8	R8	K5
      0x78220005,  //  0024  JMPF	R8	#002B
      0x8C200706,  //  0025  GETMET	R8	R3	K6
      0x60280013,  //  0026  GETGBL	R10	G19
      0x7C280000,  //  0027  CALL	R10	0
      0x982A0E06,  //  0028  SETIDX	R10	K7	R6
      0x982A1206,  //  0029  SETIDX	R10	K9	R6
      0x7C200400,  //  002A  CALL	R8	2
      0x7001FFDE,  //  002B  JMP		#000B
      0x5814000B,  //  002C  LDCONST	R5	K11
      0xAC140200,  //  002D  CATCH	R5	1	0
      0xB0080000,  //  002E  RAISE	2	R0	R0
      0x80040600,  //  002F  RET	1	R3
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(content_send),
    /* K2   */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cform_X20id_X3Dac_X20action_X3D_X27matterc_X27_X20style_X3D_X27display_X3A_X20block_X3B_X27_X20method_X3D_X27get_X27_X3E_X3Cbutton_X3E),
    /* K3   */  be_nested_str_long(_X3Csvg_X20style_X3D_X27vertical_X2Dalign_X3Amiddle_X3B_X27_X20width_X3D_X2724_X27_X20height_X3D_X2724_X27_X20xmlns_X3D_X27http_X3A_X2F_X2Fwww_X2Ew3_X2Eorg_X2F2000_X2Fsvg_X27_X20viewBox_X3D_X27100_X20100_X20240_X20240_X27_X3E_X3Cdefs_X3E_X3Cstyle_X3E_X2Ecls_X2D1_X7Bfill_X3Anone_X7D_X2Ecls_X2D2_X7Bfill_X3A_X23FFFFFF_X3B_X7D_X3C_X2Fstyle_X3E_X3C_X2Fdefs_X3E_X3Crect_X20class_X3D_X27cls_X2D1_X27_X20width_X3D_X27420_X27_X20height_X3D_X27420_X27_X2F_X3E_X3Cpath_X20class_X3D_X27cls_X2D2_X27_X20d_X3D_X27M167_X2C156_X2E88a71_X2C71_X2C0_X2C0_X2C0_X2C32_X2E1_X2C14_X2E73v_X2D62_X2E8l12_X2E79_X2D7_X2E38_X2C12_X2E78_X2C7_X2E38v62_X2E8a71_X2E09_X2C71_X2E09_X2C0_X2C0_X2C0_X2C32_X2E11_X2D14_X2E73L280_X2C170_X2E31a96_X2E92_X2C96_X2E92_X2C0_X2C0_X2C1_X2D136_X2E33_X2C0Zm28_X2E22_X2C160_X2E37A96_X2E92_X2C96_X2E92_X2C0_X2C0_X2C0_X2C127_X2C199_X2E19v26_X2E87a71_X2E06_X2C71_X2E06_X2C0_X2C0_X2C1_X2C28_X2E82_X2C20_X2E43l_X2D54_X2E39_X2C31_X2E4v14_X2E77L114_X2E22_X2C300l54_X2E38_X2D31_X2E4a71_X2C71_X2C0_X2C0_X2C1_X2C3_X2E29_X2C35_X2E17Zm101_X2E5_X2D118_X2E06a96_X2E93_X2C96_X2E93_X2C0_X2C0_X2C0_X2D68_X2E16_X2C118_X2E06l23_X2E27_X2D13_X2E44a71_X2E1_X2C71_X2E1_X2C0_X2C0_X2C1_X2C3_X2E29_X2D35_X2E17L309_X2E46_X2C300l12_X2E78_X2D7_X2E38V277_X2E89l_X2D54_X2E39_X2D31_X2E4a71_X2E13_X2C71_X2E13_X2C0_X2C0_X2C1_X2C28_X2E82_X2D20_X2E43Z_X27_X2F_X3E_X3C_X2Fsvg_X3E),
    /* K4   */  be_nested_str_weak(_X20Matter_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    }),
    be_str_weak(web_add_config_button),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080301,  //  0004  GETMET	R2	R1	K1
      0x58100003,  //  0005  LDCONST	R4	K3
      0x7C080400,  //  0006  CALL	R2	2
      0x8C080301,  //  0007  GETMET	R2	R1	K1
      0x58100004,  //  0008  LDCONST	R4	K4
      0x7C080400,  //  0009  CALL	R2	2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_UI_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(matter_enabled),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(add_driver),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C0C0400,  //  0005  CALL	R3	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


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
** Solidified function: collect_plugin_schemas
********************************************************************/
be_local_closure(class_Matter_UI_collect_plugin_schemas,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(SCHEMA),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_nested_str_weak(),
    /* K5   */  be_nested_str_weak(extract_arg_name),
    }),
    be_str_weak(collect_plugin_schemas),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0013,  //  0001  GETGBL	R3	G19
      0x7C0C0000,  //  0002  CALL	R3	0
      0x58100001,  //  0003  LDCONST	R4	K1
      0x50140200,  //  0004  LDBOOL	R5	1	0
      0x7816001C,  //  0005  JMPF	R5	#0023
      0x1C140901,  //  0006  EQ	R5	R4	K1
      0x78160001,  //  0007  JMPF	R5	#000A
      0x58140002,  //  0008  LDCONST	R5	K2
      0x70020003,  //  0009  JMP		#000E
      0x60140008,  //  000A  GETGBL	R5	G8
      0x5C180800,  //  000B  MOVE	R6	R4
      0x7C140200,  //  000C  CALL	R5	1
      0x00160405,  //  000D  ADD	R5	K2	R5
      0x8C180503,  //  000E  GETMET	R6	R2	K3
      0x5C200200,  //  000F  MOVE	R8	R1
      0x5C240A00,  //  0010  MOVE	R9	R5
      0x7C180600,  //  0011  CALL	R6	3
      0x4C1C0000,  //  0012  LDNIL	R7
      0x1C1C0C07,  //  0013  EQ	R7	R6	R7
      0x741E0001,  //  0014  JMPT	R7	#0017
      0x1C1C0D04,  //  0015  EQ	R7	R6	K4
      0x781E0000,  //  0016  JMPF	R7	#0018
      0x7002000A,  //  0017  JMP		#0023
      0x8C1C0105,  //  0018  GETMET	R7	R0	K5
      0x5C240C00,  //  0019  MOVE	R9	R6
      0x7C1C0400,  //  001A  CALL	R7	2
      0x4C200000,  //  001B  LDNIL	R8
      0x20200E08,  //  001C  NE	R8	R7	R8
      0x78220002,  //  001D  JMPF	R8	#0021
      0x20200F04,  //  001E  NE	R8	R7	K4
      0x78220000,  //  001F  JMPF	R8	#0021
      0x980C0E06,  //  0020  SETIDX	R3	R7	R6
      0x00100901,  //  0021  ADD	R4	R4	K1
      0x7001FFE0,  //  0022  JMP		#0004
      0x80040600,  //  0023  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_remote_autoconf
********************************************************************/
be_local_closure(class_Matter_UI_show_remote_autoconf,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[41]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(content_send),
    /* K19  */  be_nested_str_weak(_X3Cscript_X20type_X3D_X27text_X2Fjavascript_X27_X3E),
    /* K20  */  be_nested_str_weak(generate_schema_js),
    /* K21  */  be_nested_str_weak(generate_display_names_js),
    /* K22  */  be_nested_str_weak(_CLASSES_TYPES2),
    /* K23  */  be_nested_str_weak(keys),
    /* K24  */  be_nested_str_weak(type),
    /* K25  */  be_nested_str_weak(http_),
    /* K26  */  be_nested_str_weak(stop_iteration),
    /* K27  */  be_nested_str_weak(push),
    /* K28  */  be_nested_str_weak(var_X20remcfg_X3D_X25s_X3B),
    /* K29  */  be_nested_str_weak(dump),
    /* K30  */  be_nested_str_weak(var_X20remurl_X3D_X25s_X3B),
    /* K31  */  be_nested_str_weak(_X3C_X2Fscript_X3E),
    /* K32  */  be_nested_str_weak(_ADD_ENDPOINT_JS),
    /* K33  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Remote_X20Device_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K34  */  be_nested_str_weak(html_escape),
    /* K35  */  be_nested_str_weak(_X3Cp_X3E_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X3E_X25s_X3C_X2Fa_X3E_X3C_X2Fp_X3E),
    /* K36  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20submitRemote_X28this_X29_X27_X3E_X3Cinput_X20name_X3D_X27url_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27),
    /* K37  */  be_nested_str_long(_X27_X3E_X3Cinput_X20name_X3D_X27rem_json_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X27_X3E_X3Ctable_X20id_X3D_X27remTbl_X27_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X23_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2778_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EName_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E_X3Cbutton_X20name_X3D_X27addrem_X27_X20class_X3D_X27button_X20bgrn_X27_X3EAdd_X20endpoints_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K38  */  be_nested_str_long(_X3Cscript_X20type_X3D_X27text_X2Fjavascript_X27_X3Evar_X20ri_X3D0_X3Bremcfg_X2EforEach_X28function_X28c_X29_X7Bvar_X20tbl_X3Deb_X28_X27remTbl_X27_X29_X3Bif_X28tbl_X29tbl_X2EinsertAdjacentHTML_X28_X27beforeend_X27_X2CgenEpRows_X28_X27r_X27_X2Bri_X2Cc_X2Cfalse_X29_X29_X3Bri_X2B_X2B_X3B_X7D_X29_X3B_X28function_X28_X29_X7Bvar_X20tbl_X3Deb_X28_X27remTbl_X27_X29_X3Bif_X28tbl_X29tbl_X2EinsertAdjacentHTML_X28_X27beforeend_X27_X2CgenEpRows_X28_X27r_X27_X2Bri_X2C_X7Btype_X3A_X27_X27_X7D_X2Ctrue_X29_X29_X3Bri_X2B_X2B_X3B_X7D_X29_X28_X29_X3Bfunction_X20submitRemote_X28f_X29_X7Bvar_X20eps_X3D_X5B_X5D_X3Bfor_X28var_X20i_X3D0_X3Bi_X3Cri_X3Bi_X2B_X2B_X29_X7Bvar_X20ne_X3Deb_X28_X27epNamr_X27_X2Bi_X29_X3Bvar_X20te_X3Deb_X28_X27epTypr_X27_X2Bi_X29_X3Bvar_X20typ_X3Dte_X3Fte_X2Evalue_X3Aremcfg_X5Bi_X5D_X3Fremcfg_X5Bi_X5D_X2Etype_X3A_X27_X27_X3Bif_X28_X21typ_X29continue_X3Bvar_X20ep_X3D_X7Btype_X3Atyp_X7D_X3Bif_X28ne_X29_X7Bvar_X20v_X3Dne_X2Evalue_X2Etrim_X28_X29_X3Bif_X28v_X29ep_X2Ename_X3Dv_X3B_X7Dvar_X20schema_X3Dps_X3Fps_X5Btyp_X5D_X3Anull_X3Bif_X28schema_X29_X7Bfor_X28var_X20key_X20in_X20schema_X29_X7Bvar_X20def_X3DparseSchema_X28schema_X5Bkey_X5D_X29_X3Bvar_X20akey_X3Ddef_X2Ea_X7C_X7Ckey_X3Bvar_X20fe_X3Deb_X28_X27epr_X27_X2Bi_X2B_X27__X27_X2Bkey_X29_X3Bif_X28fe_X29_X7Bvar_X20v_X3Dfe_X2Etype_X3D_X3D_X3D_X27checkbox_X27_X3F_X28fe_X2Echecked_X3F1_X3A0_X29_X3Afe_X2Evalue_X3Bif_X28v_X21_X3D_X3D_X27_X27_X26_X26v_X21_X3D_X3Dnull_X26_X26v_X21_X3D_X3Dundefined_X29_X7Bep_X5Bakey_X5D_X3D_X28def_X2Et_X3D_X3D_X3D_X27i_X27_X29_X3FparseInt_X28v_X2C10_X29_X3Av_X3B_X7D_X7D_X7D_X7Deps_X2Epush_X28ep_X29_X3B_X7Df_X2Eelements_X5B_X27rem_json_X27_X5D_X2Evalue_X3DJSON_X2Estringify_X28eps_X29_X3Breturn_X20true_X3B_X7D_X3C_X2Fscript_X3E),
    /* K39  */  be_nested_str_weak(_X3C_X2Ffieldset_X3E),
    /* K40  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3EUnable_X20to_X20connect_X20to_X20_X27_X25s_X27_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_remote_autoconf),
    &be_const_str_solidified,
    ( &(const binstruction[183]) {  /* code */
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
      0x78220073,  //  0039  JMPF	R8	#00AE
      0x4C200000,  //  003A  LDNIL	R8
      0x20200E08,  //  003B  NE	R8	R7	R8
      0x78220070,  //  003C  JMPF	R8	#00AE
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
      0x8C240512,  //  004E  GETMET	R9	R2	K18
      0x582C0013,  //  004F  LDCONST	R11	K19
      0x7C240400,  //  0050  CALL	R9	2
      0x8C240114,  //  0051  GETMET	R9	R0	K20
      0x7C240200,  //  0052  CALL	R9	1
      0x8C240115,  //  0053  GETMET	R9	R0	K21
      0x602C0012,  //  0054  GETGBL	R11	G18
      0x7C2C0000,  //  0055  CALL	R11	0
      0x88300116,  //  0056  GETMBR	R12	R0	K22
      0x4030160C,  //  0057  CONNECT	R12	R11	R12
      0x7C240400,  //  0058  CALL	R9	2
      0x60240012,  //  0059  GETGBL	R9	G18
      0x7C240000,  //  005A  CALL	R9	0
      0x60280010,  //  005B  GETGBL	R10	G16
      0x5C2C1000,  //  005C  MOVE	R11	R8
      0x7C280200,  //  005D  CALL	R10	1
      0xA802001A,  //  005E  EXBLK	0	#007A
      0x5C2C1400,  //  005F  MOVE	R11	R10
      0x7C2C0000,  //  0060  CALL	R11	0
      0x60300013,  //  0061  GETGBL	R12	G19
      0x7C300000,  //  0062  CALL	R12	0
      0x60340010,  //  0063  GETGBL	R13	G16
      0x8C381717,  //  0064  GETMET	R14	R11	K23
      0x7C380200,  //  0065  CALL	R14	1
      0x7C340200,  //  0066  CALL	R13	1
      0xA802000A,  //  0067  EXBLK	0	#0073
      0x5C381A00,  //  0068  MOVE	R14	R13
      0x7C380000,  //  0069  CALL	R14	0
      0x1C3C1D18,  //  006A  EQ	R15	R14	K24
      0x783E0003,  //  006B  JMPF	R15	#0070
      0x943C1718,  //  006C  GETIDX	R15	R11	K24
      0x003E320F,  //  006D  ADD	R15	K25	R15
      0x9832300F,  //  006E  SETIDX	R12	K24	R15
      0x70020001,  //  006F  JMP		#0072
      0x943C160E,  //  0070  GETIDX	R15	R11	R14
      0x98301C0F,  //  0071  SETIDX	R12	R14	R15
      0x7001FFF4,  //  0072  JMP		#0068
      0x5834001A,  //  0073  LDCONST	R13	K26
      0xAC340200,  //  0074  CATCH	R13	1	0
      0xB0080000,  //  0075  RAISE	2	R0	R0
      0x8C34131B,  //  0076  GETMET	R13	R9	K27
      0x5C3C1800,  //  0077  MOVE	R15	R12
      0x7C340400,  //  0078  CALL	R13	2
      0x7001FFE4,  //  0079  JMP		#005F
      0x5828001A,  //  007A  LDCONST	R10	K26
      0xAC280200,  //  007B  CATCH	R10	1	0
      0xB0080000,  //  007C  RAISE	2	R0	R0
      0x8C280512,  //  007D  GETMET	R10	R2	K18
      0x60300018,  //  007E  GETGBL	R12	G24
      0x5834001C,  //  007F  LDCONST	R13	K28
      0x8C38071D,  //  0080  GETMET	R14	R3	K29
      0x5C401200,  //  0081  MOVE	R16	R9
      0x7C380400,  //  0082  CALL	R14	2
      0x7C300400,  //  0083  CALL	R12	2
      0x7C280400,  //  0084  CALL	R10	2
      0x8C280512,  //  0085  GETMET	R10	R2	K18
      0x60300018,  //  0086  GETGBL	R12	G24
      0x5834001E,  //  0087  LDCONST	R13	K30
      0x8C38071D,  //  0088  GETMET	R14	R3	K29
      0x5C400200,  //  0089  MOVE	R16	R1
      0x7C380400,  //  008A  CALL	R14	2
      0x7C300400,  //  008B  CALL	R12	2
      0x7C280400,  //  008C  CALL	R10	2
      0x8C280512,  //  008D  GETMET	R10	R2	K18
      0x5830001F,  //  008E  LDCONST	R12	K31
      0x7C280400,  //  008F  CALL	R10	2
      0x8C280512,  //  0090  GETMET	R10	R2	K18
      0x88300120,  //  0091  GETMBR	R12	R0	K32
      0x7C280400,  //  0092  CALL	R10	2
      0x8C280512,  //  0093  GETMET	R10	R2	K18
      0x58300021,  //  0094  LDCONST	R12	K33
      0x7C280400,  //  0095  CALL	R10	2
      0x8C280522,  //  0096  GETMET	R10	R2	K34
      0x5C300200,  //  0097  MOVE	R12	R1
      0x7C280400,  //  0098  CALL	R10	2
      0x8C2C0512,  //  0099  GETMET	R11	R2	K18
      0x60340018,  //  009A  GETGBL	R13	G24
      0x58380023,  //  009B  LDCONST	R14	K35
      0x5C3C1400,  //  009C  MOVE	R15	R10
      0x5C401400,  //  009D  MOVE	R16	R10
      0x7C340600,  //  009E  CALL	R13	3
      0x7C2C0400,  //  009F  CALL	R11	2
      0x8C2C0512,  //  00A0  GETMET	R11	R2	K18
      0x8C340522,  //  00A1  GETMET	R13	R2	K34
      0x5C3C0200,  //  00A2  MOVE	R15	R1
      0x7C340400,  //  00A3  CALL	R13	2
      0x0036480D,  //  00A4  ADD	R13	K36	R13
      0x00341B25,  //  00A5  ADD	R13	R13	K37
      0x7C2C0400,  //  00A6  CALL	R11	2
      0x8C2C0512,  //  00A7  GETMET	R11	R2	K18
      0x58340026,  //  00A8  LDCONST	R13	K38
      0x7C2C0400,  //  00A9  CALL	R11	2
      0x8C2C0512,  //  00AA  GETMET	R11	R2	K18
      0x58340027,  //  00AB  LDCONST	R13	K39
      0x7C2C0400,  //  00AC  CALL	R11	2
      0x70020007,  //  00AD  JMP		#00B6
      0x8C200512,  //  00AE  GETMET	R8	R2	K18
      0x60280018,  //  00AF  GETGBL	R10	G24
      0x582C0028,  //  00B0  LDCONST	R11	K40
      0x8C300522,  //  00B1  GETMET	R12	R2	K34
      0x5C380200,  //  00B2  MOVE	R14	R1
      0x7C300400,  //  00B3  CALL	R12	2
      0x7C280400,  //  00B4  CALL	R10	2
      0x7C200400,  //  00B5  CALL	R8	2
      0x80000000,  //  00B6  RET	0
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
** Solidified function: generate_schema_js
********************************************************************/
be_local_closure(class_Matter_UI_generate_schema_js,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(webserver),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(var_X20ps_X3D_X7B),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(plugins_classes),
    /* K6   */  be_nested_str_weak(keys),
    /* K7   */  be_nested_str_weak(collect_plugin_schemas),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(_X2C),
    /* K10  */  be_nested_str_weak(_X22_X25s_X22_X3A_X7B),
    /* K11  */  be_nested_str_weak(replace),
    /* K12  */  be_nested_str_weak(_X22),
    /* K13  */  be_nested_str_weak(_X5C_X22),
    /* K14  */  be_nested_str_weak(_X22_X25s_X22_X3A_X22_X25s_X22),
    /* K15  */  be_nested_str_weak(stop_iteration),
    /* K16  */  be_nested_str_weak(_X7D),
    /* K17  */  be_nested_str_weak(_X7D_X3B),
    }),
    be_str_weak(generate_schema_js),
    &be_const_str_solidified,
    ( &(const binstruction[79]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0502,  //  0002  GETMET	R3	R2	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x500C0200,  //  0005  LDBOOL	R3	1	0
      0x60100010,  //  0006  GETGBL	R4	G16
      0x88140104,  //  0007  GETMBR	R5	R0	K4
      0x88140B05,  //  0008  GETMBR	R5	R5	K5
      0x8C140B06,  //  0009  GETMET	R5	R5	K6
      0x7C140200,  //  000A  CALL	R5	1
      0x7C100200,  //  000B  CALL	R4	1
      0xA802003A,  //  000C  EXBLK	0	#0048
      0x5C140800,  //  000D  MOVE	R5	R4
      0x7C140000,  //  000E  CALL	R5	0
      0x88180104,  //  000F  GETMBR	R6	R0	K4
      0x88180D05,  //  0010  GETMBR	R6	R6	K5
      0x94180C05,  //  0011  GETIDX	R6	R6	R5
      0x8C1C0107,  //  0012  GETMET	R7	R0	K7
      0x5C240C00,  //  0013  MOVE	R9	R6
      0x7C1C0400,  //  0014  CALL	R7	2
      0x6020000C,  //  0015  GETGBL	R8	G12
      0x5C240E00,  //  0016  MOVE	R9	R7
      0x7C200200,  //  0017  CALL	R8	1
      0x24201108,  //  0018  GT	R8	R8	K8
      0x7822002C,  //  0019  JMPF	R8	#0047
      0x5C200600,  //  001A  MOVE	R8	R3
      0x74220002,  //  001B  JMPT	R8	#001F
      0x8C200502,  //  001C  GETMET	R8	R2	K2
      0x58280009,  //  001D  LDCONST	R10	K9
      0x7C200400,  //  001E  CALL	R8	2
      0x500C0000,  //  001F  LDBOOL	R3	0	0
      0x8C200502,  //  0020  GETMET	R8	R2	K2
      0x60280018,  //  0021  GETGBL	R10	G24
      0x582C000A,  //  0022  LDCONST	R11	K10
      0x5C300A00,  //  0023  MOVE	R12	R5
      0x7C280400,  //  0024  CALL	R10	2
      0x7C200400,  //  0025  CALL	R8	2
      0x50200200,  //  0026  LDBOOL	R8	1	0
      0x60240010,  //  0027  GETGBL	R9	G16
      0x8C280F06,  //  0028  GETMET	R10	R7	K6
      0x7C280200,  //  0029  CALL	R10	1
      0x7C240200,  //  002A  CALL	R9	1
      0xA8020014,  //  002B  EXBLK	0	#0041
      0x5C281200,  //  002C  MOVE	R10	R9
      0x7C280000,  //  002D  CALL	R10	0
      0x5C2C1000,  //  002E  MOVE	R11	R8
      0x742E0002,  //  002F  JMPT	R11	#0033
      0x8C2C0502,  //  0030  GETMET	R11	R2	K2
      0x58340009,  //  0031  LDCONST	R13	K9
      0x7C2C0400,  //  0032  CALL	R11	2
      0x50200000,  //  0033  LDBOOL	R8	0	0
      0x8C2C030B,  //  0034  GETMET	R11	R1	K11
      0x94340E0A,  //  0035  GETIDX	R13	R7	R10
      0x5838000C,  //  0036  LDCONST	R14	K12
      0x583C000D,  //  0037  LDCONST	R15	K13
      0x7C2C0800,  //  0038  CALL	R11	4
      0x8C300502,  //  0039  GETMET	R12	R2	K2
      0x60380018,  //  003A  GETGBL	R14	G24
      0x583C000E,  //  003B  LDCONST	R15	K14
      0x5C401400,  //  003C  MOVE	R16	R10
      0x5C441600,  //  003D  MOVE	R17	R11
      0x7C380600,  //  003E  CALL	R14	3
      0x7C300400,  //  003F  CALL	R12	2
      0x7001FFEA,  //  0040  JMP		#002C
      0x5824000F,  //  0041  LDCONST	R9	K15
      0xAC240200,  //  0042  CATCH	R9	1	0
      0xB0080000,  //  0043  RAISE	2	R0	R0
      0x8C240502,  //  0044  GETMET	R9	R2	K2
      0x582C0010,  //  0045  LDCONST	R11	K16
      0x7C240400,  //  0046  CALL	R9	2
      0x7001FFC4,  //  0047  JMP		#000D
      0x5810000F,  //  0048  LDCONST	R4	K15
      0xAC100200,  //  0049  CATCH	R4	1	0
      0xB0080000,  //  004A  RAISE	2	R0	R0
      0x8C100502,  //  004B  GETMET	R4	R2	K2
      0x58180011,  //  004C  LDCONST	R6	K17
      0x7C100400,  //  004D  CALL	R4	2
      0x80000000,  //  004E  RET	0
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
** Solidified function: page_part_ctl
********************************************************************/
be_local_closure(class_Matter_UI_page_part_ctl,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    1,                          /* argc */
    18,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[84]) {     /* constants */
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
    /* K51  */  be_nested_str_weak(config_json),
    /* K52  */  be_nested_str_weak(handle_config_json),
    /* K53  */  be_nested_str_weak(_X2Fmatterc_X3F),
    /* K54  */  be_nested_str_weak(addrem),
    /* K55  */  be_nested_str_weak(url),
    /* K56  */  be_nested_str_weak(),
    /* K57  */  be_nested_str_weak(value_error),
    /* K58  */  be_nested_str_weak(url_X20shouldn_X27t_X20be_X20null),
    /* K59  */  be_nested_str_weak(rem_json),
    /* K60  */  be_nested_str_weak(json),
    /* K61  */  be_nested_str_weak(load),
    /* K62  */  be_nested_str_weak(find),
    /* K63  */  be_nested_str_weak(type),
    /* K64  */  be_nested_str_weak(plugins_classes),
    /* K65  */  be_nested_str_weak(name),
    /* K66  */  be_nested_str_weak(keys),
    /* K67  */  be_nested_str_weak(stop_iteration),
    /* K68  */  be_nested_str_weak(plugins_config),
    /* K69  */  be_nested_str_weak(equal_map),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20remote_X20add_X20url_X3D_X27_X25s_X27_X20type_X3D_X27_X25s_X27),
    /* K71  */  be_nested_str_weak(bridge_add_endpoint),
    /* K72  */  be_nested_str_weak(content_start),
    /* K73  */  be_nested_str_weak(Parameter_X20error),
    /* K74  */  be_nested_str_weak(content_send_style),
    /* K75  */  be_nested_str_weak(content_send),
    /* K76  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EError_X3A_X3C_X2Fb_X3E_X25s_X3C_X2Fp_X3E),
    /* K77  */  be_nested_str_weak(html_escape),
    /* K78  */  be_nested_str_weak(content_button),
    /* K79  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K80  */  be_nested_str_weak(content_stop),
    /* K81  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K82  */  be_const_int(2),
    /* K83  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
    }),
    be_str_weak(page_part_ctl),
    &be_const_str_solidified,
    ( &(const binstruction[397]) {  /* code */
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
      0xA8020163,  //  000A  EXBLK	0	#016F
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
      0x7002011C,  //  003C  JMP		#015A
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
      0x8824011A,  //  0054  GETMBR	R9	R0	K26
      0x20240C09,  //  0055  NE	R9	R6	R9
      0x78260025,  //  0056  JMPF	R9	#007D
      0x781A0010,  //  0057  JMPF	R6	#0069
      0xB8261000,  //  0058  GETNGBL	R9	K8
      0x60280018,  //  0059  GETGBL	R10	G24
      0x582C0009,  //  005A  LDCONST	R11	K9
      0x5830001B,  //  005B  LDCONST	R12	K27
      0x7C280400,  //  005C  CALL	R10	2
      0x582C000A,  //  005D  LDCONST	R11	K10
      0x7C240400,  //  005E  CALL	R9	2
      0xB8263800,  //  005F  GETNGBL	R9	K28
      0x8C24131D,  //  0060  GETMET	R9	R9	K29
      0x602C0008,  //  0061  GETGBL	R11	G8
      0xB8323E00,  //  0062  GETNGBL	R12	K31
      0x88301920,  //  0063  GETMBR	R12	R12	K32
      0x7C2C0200,  //  0064  CALL	R11	1
      0x002E3C0B,  //  0065  ADD	R11	K30	R11
      0x002C1721,  //  0066  ADD	R11	R11	K33
      0x7C240400,  //  0067  CALL	R9	2
      0x7002000F,  //  0068  JMP		#0079
      0xB8261000,  //  0069  GETNGBL	R9	K8
      0x60280018,  //  006A  GETGBL	R10	G24
      0x582C0009,  //  006B  LDCONST	R11	K9
      0x58300022,  //  006C  LDCONST	R12	K34
      0x7C280400,  //  006D  CALL	R10	2
      0x582C000A,  //  006E  LDCONST	R11	K10
      0x7C240400,  //  006F  CALL	R9	2
      0xB8263800,  //  0070  GETNGBL	R9	K28
      0x8C24131D,  //  0071  GETMET	R9	R9	K29
      0x602C0008,  //  0072  GETGBL	R11	G8
      0xB8323E00,  //  0073  GETNGBL	R12	K31
      0x88301920,  //  0074  GETMBR	R12	R12	K32
      0x7C2C0200,  //  0075  CALL	R11	1
      0x002E3C0B,  //  0076  ADD	R11	K30	R11
      0x002C1723,  //  0077  ADD	R11	R11	K35
      0x7C240400,  //  0078  CALL	R9	2
      0x8C240313,  //  0079  GETMET	R9	R1	K19
      0x582C0014,  //  007A  LDCONST	R11	K20
      0x7C240400,  //  007B  CALL	R9	2
      0x70020017,  //  007C  JMP		#0095
      0x8824010B,  //  007D  GETMBR	R9	R0	K11
      0x88241324,  //  007E  GETMBR	R9	R9	K36
      0x88241325,  //  007F  GETMBR	R9	R9	K37
      0x4C280000,  //  0080  LDNIL	R10
      0x2024120A,  //  0081  NE	R9	R9	R10
      0x20240E09,  //  0082  NE	R9	R7	R9
      0x7826000D,  //  0083  JMPF	R9	#0092
      0x781E0004,  //  0084  JMPF	R7	#008A
      0x8824010B,  //  0085  GETMBR	R9	R0	K11
      0x88241324,  //  0086  GETMBR	R9	R9	K36
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
      0x700200C3,  //  0095  JMP		#015A
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
      0x70020099,  //  00BF  JMP		#015A
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
      0x70020087,  //  00D1  JMP		#015A
      0x8C180305,  //  00D2  GETMET	R6	R1	K5
      0x58200033,  //  00D3  LDCONST	R8	K51
      0x7C180400,  //  00D4  CALL	R6	2
      0x781A000C,  //  00D5  JMPF	R6	#00E3
      0xB81A1000,  //  00D6  GETNGBL	R6	K8
      0x601C0018,  //  00D7  GETGBL	R7	G24
      0x58200009,  //  00D8  LDCONST	R8	K9
      0x58240033,  //  00D9  LDCONST	R9	K51
      0x7C1C0400,  //  00DA  CALL	R7	2
      0x5820000A,  //  00DB  LDCONST	R8	K10
      0x7C180400,  //  00DC  CALL	R6	2
      0x8C180134,  //  00DD  GETMET	R6	R0	K52
      0x7C180200,  //  00DE  CALL	R6	1
      0x8C180313,  //  00DF  GETMET	R6	R1	K19
      0x58200035,  //  00E0  LDCONST	R8	K53
      0x7C180400,  //  00E1  CALL	R6	2
      0x70020076,  //  00E2  JMP		#015A
      0x8C180305,  //  00E3  GETMET	R6	R1	K5
      0x58200036,  //  00E4  LDCONST	R8	K54
      0x7C180400,  //  00E5  CALL	R6	2
      0x781A0072,  //  00E6  JMPF	R6	#015A
      0x8C18030D,  //  00E7  GETMET	R6	R1	K13
      0x58200037,  //  00E8  LDCONST	R8	K55
      0x7C180400,  //  00E9  CALL	R6	2
      0x4C1C0000,  //  00EA  LDNIL	R7
      0x1C1C0C07,  //  00EB  EQ	R7	R6	R7
      0x741E0001,  //  00EC  JMPT	R7	#00EF
      0x1C1C0D38,  //  00ED  EQ	R7	R6	K56
      0x781E0000,  //  00EE  JMPF	R7	#00F0
      0xB006733A,  //  00EF  RAISE	1	K57	K58
      0x8C1C030D,  //  00F0  GETMET	R7	R1	K13
      0x5824003B,  //  00F1  LDCONST	R9	K59
      0x7C1C0400,  //  00F2  CALL	R7	2
      0x4C200000,  //  00F3  LDNIL	R8
      0x20200E08,  //  00F4  NE	R8	R7	R8
      0x78220060,  //  00F5  JMPF	R8	#0157
      0x20200F38,  //  00F6  NE	R8	R7	K56
      0x7822005E,  //  00F7  JMPF	R8	#0157
      0xA4227800,  //  00F8  IMPORT	R8	K60
      0x8C24113D,  //  00F9  GETMET	R9	R8	K61
      0x5C2C0E00,  //  00FA  MOVE	R11	R7
      0x7C240400,  //  00FB  CALL	R9	2
      0x4C280000,  //  00FC  LDNIL	R10
      0x2028120A,  //  00FD  NE	R10	R9	R10
      0x782A0057,  //  00FE  JMPF	R10	#0157
      0x60280010,  //  00FF  GETGBL	R10	G16
      0x5C2C1200,  //  0100  MOVE	R11	R9
      0x7C280200,  //  0101  CALL	R10	1
      0xA8020050,  //  0102  EXBLK	0	#0154
      0x5C2C1400,  //  0103  MOVE	R11	R10
      0x7C2C0000,  //  0104  CALL	R11	0
      0x8C30173E,  //  0105  GETMET	R12	R11	K62
      0x5838003F,  //  0106  LDCONST	R14	K63
      0x583C0038,  //  0107  LDCONST	R15	K56
      0x7C300600,  //  0108  CALL	R12	3
      0x1C341938,  //  0109  EQ	R13	R12	K56
      0x78360000,  //  010A  JMPF	R13	#010C
      0x7001FFF6,  //  010B  JMP		#0103
      0x8834010B,  //  010C  GETMBR	R13	R0	K11
      0x88341B40,  //  010D  GETMBR	R13	R13	K64
      0x8C341B3E,  //  010E  GETMET	R13	R13	K62
      0x5C3C1800,  //  010F  MOVE	R15	R12
      0x7C340400,  //  0110  CALL	R13	2
      0x4C380000,  //  0111  LDNIL	R14
      0x1C381A0E,  //  0112  EQ	R14	R13	R14
      0x783A0000,  //  0113  JMPF	R14	#0115
      0x7001FFED,  //  0114  JMP		#0103
      0x60380013,  //  0115  GETGBL	R14	G19
      0x7C380000,  //  0116  CALL	R14	0
      0x983A6E06,  //  0117  SETIDX	R14	K55	R6
      0x983A7E0C,  //  0118  SETIDX	R14	K63	R12
      0x8C3C173E,  //  0119  GETMET	R15	R11	K62
      0x58440041,  //  011A  LDCONST	R17	K65
      0x7C3C0400,  //  011B  CALL	R15	2
      0x783E0000,  //  011C  JMPF	R15	#011E
      0x983A820F,  //  011D  SETIDX	R14	K65	R15
      0x60400010,  //  011E  GETGBL	R16	G16
      0x8C441742,  //  011F  GETMET	R17	R11	K66
      0x7C440200,  //  0120  CALL	R17	1
      0x7C400200,  //  0121  CALL	R16	1
      0xA8020008,  //  0122  EXBLK	0	#012C
      0x5C442000,  //  0123  MOVE	R17	R16
      0x7C440000,  //  0124  CALL	R17	0
      0x2048233F,  //  0125  NE	R18	R17	K63
      0x784A0003,  //  0126  JMPF	R18	#012B
      0x20482341,  //  0127  NE	R18	R17	K65
      0x784A0001,  //  0128  JMPF	R18	#012B
      0x94481611,  //  0129  GETIDX	R18	R11	R17
      0x98382212,  //  012A  SETIDX	R14	R17	R18
      0x7001FFF6,  //  012B  JMP		#0123
      0x58400043,  //  012C  LDCONST	R16	K67
      0xAC400200,  //  012D  CATCH	R16	1	0
      0xB0080000,  //  012E  RAISE	2	R0	R0
      0x50400000,  //  012F  LDBOOL	R16	0	0
      0x60440010,  //  0130  GETGBL	R17	G16
      0x8848010B,  //  0131  GETMBR	R18	R0	K11
      0x88482544,  //  0132  GETMBR	R18	R18	K68
      0x7C440200,  //  0133  CALL	R17	1
      0xA802000B,  //  0134  EXBLK	0	#0141
      0x5C482200,  //  0135  MOVE	R18	R17
      0x7C480000,  //  0136  CALL	R18	0
      0x8C4C0145,  //  0137  GETMET	R19	R0	K69
      0x5C542400,  //  0138  MOVE	R21	R18
      0x5C581C00,  //  0139  MOVE	R22	R14
      0x7C4C0600,  //  013A  CALL	R19	3
      0x784E0001,  //  013B  JMPF	R19	#013E
      0x50400200,  //  013C  LDBOOL	R16	1	0
      0x70020000,  //  013D  JMP		#013F
      0x7001FFF5,  //  013E  JMP		#0135
      0xA8040001,  //  013F  EXBLK	1	1
      0x70020002,  //  0140  JMP		#0144
      0x58440043,  //  0141  LDCONST	R17	K67
      0xAC440200,  //  0142  CATCH	R17	1	0
      0xB0080000,  //  0143  RAISE	2	R0	R0
      0x5C442000,  //  0144  MOVE	R17	R16
      0x7446000C,  //  0145  JMPT	R17	#0153
      0xB8461000,  //  0146  GETNGBL	R17	K8
      0x60480018,  //  0147  GETGBL	R18	G24
      0x584C0046,  //  0148  LDCONST	R19	K70
      0x5C500C00,  //  0149  MOVE	R20	R6
      0x5C541800,  //  014A  MOVE	R21	R12
      0x7C480600,  //  014B  CALL	R18	3
      0x584C000A,  //  014C  LDCONST	R19	K10
      0x7C440400,  //  014D  CALL	R17	2
      0x8844010B,  //  014E  GETMBR	R17	R0	K11
      0x8C442347,  //  014F  GETMET	R17	R17	K71
      0x5C4C1800,  //  0150  MOVE	R19	R12
      0x5C501C00,  //  0151  MOVE	R20	R14
      0x7C440600,  //  0152  CALL	R17	3
      0x7001FFAE,  //  0153  JMP		#0103
      0x58280043,  //  0154  LDCONST	R10	K67
      0xAC280200,  //  0155  CATCH	R10	1	0
      0xB0080000,  //  0156  RAISE	2	R0	R0
      0x8C200313,  //  0157  GETMET	R8	R1	K19
      0x58280035,  //  0158  LDCONST	R10	K53
      0x7C200400,  //  0159  CALL	R8	2
      0x78160011,  //  015A  JMPF	R5	#016D
      0x8C180348,  //  015B  GETMET	R6	R1	K72
      0x58200049,  //  015C  LDCONST	R8	K73
      0x7C180400,  //  015D  CALL	R6	2
      0x8C18034A,  //  015E  GETMET	R6	R1	K74
      0x7C180200,  //  015F  CALL	R6	1
      0x8C18034B,  //  0160  GETMET	R6	R1	K75
      0x60200018,  //  0161  GETGBL	R8	G24
      0x5824004C,  //  0162  LDCONST	R9	K76
      0x8C28034D,  //  0163  GETMET	R10	R1	K77
      0x5C300A00,  //  0164  MOVE	R12	R5
      0x7C280400,  //  0165  CALL	R10	2
      0x7C200400,  //  0166  CALL	R8	2
      0x7C180400,  //  0167  CALL	R6	2
      0x8C18034E,  //  0168  GETMET	R6	R1	K78
      0x8820034F,  //  0169  GETMBR	R8	R1	K79
      0x7C180400,  //  016A  CALL	R6	2
      0x8C180350,  //  016B  GETMET	R6	R1	K80
      0x7C180200,  //  016C  CALL	R6	1
      0xA8040001,  //  016D  EXBLK	1	1
      0x7002001C,  //  016E  JMP		#018C
      0xAC180002,  //  016F  CATCH	R6	0	2
      0x70020019,  //  0170  JMP		#018B
      0xB8221000,  //  0171  GETNGBL	R8	K8
      0x60240018,  //  0172  GETGBL	R9	G24
      0x58280051,  //  0173  LDCONST	R10	K81
      0x5C2C0C00,  //  0174  MOVE	R11	R6
      0x5C300E00,  //  0175  MOVE	R12	R7
      0x7C240600,  //  0176  CALL	R9	3
      0x58280052,  //  0177  LDCONST	R10	K82
      0x7C200400,  //  0178  CALL	R8	2
      0x8C200348,  //  0179  GETMET	R8	R1	K72
      0x58280049,  //  017A  LDCONST	R10	K73
      0x7C200400,  //  017B  CALL	R8	2
      0x8C20034A,  //  017C  GETMET	R8	R1	K74
      0x7C200200,  //  017D  CALL	R8	1
      0x8C20034B,  //  017E  GETMET	R8	R1	K75
      0x60280018,  //  017F  GETGBL	R10	G24
      0x582C0053,  //  0180  LDCONST	R11	K83
      0x5C300C00,  //  0181  MOVE	R12	R6
      0x5C340E00,  //  0182  MOVE	R13	R7
      0x7C280600,  //  0183  CALL	R10	3
      0x7C200400,  //  0184  CALL	R8	2
      0x8C20034E,  //  0185  GETMET	R8	R1	K78
      0x8828034F,  //  0186  GETMBR	R10	R1	K79
      0x7C200400,  //  0187  CALL	R8	2
      0x8C200350,  //  0188  GETMET	R8	R1	K80
      0x7C200200,  //  0189  CALL	R8	1
      0x70020000,  //  018A  JMP		#018C
      0xB0080000,  //  018B  RAISE	2	R0	R0
      0x80000000,  //  018C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_plugins_configuration
********************************************************************/
be_local_closure(class_Matter_UI_show_plugins_configuration,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[43]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(json),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(zigbee),
    /* K6   */  be_nested_str_weak(show_plugins_hints_js),
    /* K7   */  be_nested_str_weak(_CLASSES_TYPES_STD),
    /* K8   */  be_nested_str_weak(_CLASSES_TYPES),
    /* K9   */  be_nested_str_weak(_CLASSES_TYPES_VIRTUAL),
    /* K10  */  be_nested_str_weak(content_send),
    /* K11  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BConfiguration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K12  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E),
    /* K13  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3ELocal_X20sensors_X20and_X20devices_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K14  */  be_nested_str_weak(_X3Ctable_X20id_X3D_X27epTbl_X27_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K15  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X23_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2778_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EName_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K16  */  be_nested_str_weak(plugins_config),
    /* K17  */  be_nested_str_weak(remove),
    /* K18  */  be_nested_str_weak(0),
    /* K19  */  be_nested_str_weak(k2l_num),
    /* K20  */  be_const_int(0),
    /* K21  */  be_nested_str_weak(find),
    /* K22  */  be_nested_str_weak(type),
    /* K23  */  be_const_int(1),
    /* K24  */  be_nested_str_weak(http_),
    /* K25  */  be_nested_str_weak(_X3Cscript_X3Edocument_X2Ewrite_X28genEpRows_X28_X25i_X2Ccfg_X5B_X27_X25i_X27_X5D_X2Cfalse_X29_X29_X3C_X2Fscript_X3E),
    /* K26  */  be_nested_str_weak(_X3C_X2Ftable_X3E),
    /* K27  */  be_nested_str_weak(_X3Cp_X20id_X3D_X27epNone_X27_X3E_X26lt_X3Bnone_X26gt_X3B_X3C_X2Fp_X3E),
    /* K28  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27text_X2Dalign_X3Aright_X27_X3E_X3Ca_X20href_X3D_X27_X23_X27_X20onclick_X3D_X27addEp_X28_X29_X3Breturn_X20false_X27_X20style_X3D_X27color_X3Avar_X28_X2D_X2Dc_btn_X29_X27_X3E_X2B_X20Add_X20endpoint_X3C_X2Fa_X3E_X3C_X2Fp_X3E),
    /* K29  */  be_nested_str_weak(url),
    /* K30  */  be_nested_str_weak(push),
    /* K31  */  be_nested_str_weak(stop_iteration),
    /* K32  */  be_nested_str_weak(sort_distinct),
    /* K33  */  be_nested_str_weak(html_escape),
    /* K34  */  be_nested_str_weak(get_plugin_remote_info),
    /* K35  */  be_nested_str_weak(name),
    /* K36  */  be_nested_str_weak(_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20title_X3D_X27http_X3A_X2F_X2F_X25s_X2F_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X3E_X25s_X3C_X2Fa_X3E),
    /* K37  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2778_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K38  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K39  */  be_nested_str_weak(_X3Cp_X3E_X26lt_X3Bnone_X26gt_X3B_X3C_X2Fp_X3E),
    /* K40  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27config_X27_X20class_X3D_X27button_X20bgrn_X27_X20onclick_X3D_X27return_X20submitConfig_X28this_X2Eform_X29_X27_X3ESave_X20configuration_X3C_X2Fbutton_X3E_X3Cinput_X20type_X3D_X27hidden_X27_X20name_X3D_X27config_json_X27_X20id_X3D_X27config_json_X27_X3E_X3C_X2Fform_X3E),
    /* K41  */  be_nested_str_long(_X3Chr_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20Tasmota_X20or_X20OpenBK_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatteradd_X27_X20method_X3D_X27get_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X2730_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3Ehttp_X3A_X2F_X2F_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27url_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X27_X20required_X20placeholder_X3D_X27IP_X20or_X20domain_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2710_X27_X20style_X3D_X27font_X2Dsize_X3Asmaller_X3B_X27_X3E_X3Cb_X3E_X2F_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E_X3Cbutton_X20class_X3D_X27button_X20bgrn_X27_X3EAuto_X2Dconfigure_X20remote_X20Tasmota_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Chr_X3E),
    /* K42  */  be_nested_str_long(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20RESET_X20the_X20configuration_X20to_X20the_X20default_X2E_X20You_X20will_X20need_X20to_X20associate_X20again_X2E_X22_X29_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27auto_X27_X20class_X3D_X27button_X20bred_X27_X3EReset_X20all_X20and_X20Auto_X2Ddiscover_X3C_X2Fbutton_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    }),
    be_str_weak(show_plugins_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[220]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0xA4120600,  //  0003  IMPORT	R4	K3
      0x88140104,  //  0004  GETMBR	R5	R0	K4
      0x88140B05,  //  0005  GETMBR	R5	R5	K5
      0x78160007,  //  0006  JMPF	R5	#000F
      0x8C140106,  //  0007  GETMET	R5	R0	K6
      0x881C0107,  //  0008  GETMBR	R7	R0	K7
      0x88200104,  //  0009  GETMBR	R8	R0	K4
      0x88201105,  //  000A  GETMBR	R8	R8	K5
      0x88201108,  //  000B  GETMBR	R8	R8	K8
      0x88240109,  //  000C  GETMBR	R9	R0	K9
      0x7C140800,  //  000D  CALL	R5	4
      0x70020003,  //  000E  JMP		#0013
      0x8C140106,  //  000F  GETMET	R5	R0	K6
      0x881C0107,  //  0010  GETMBR	R7	R0	K7
      0x88200109,  //  0011  GETMBR	R8	R0	K9
      0x7C140600,  //  0012  CALL	R5	3
      0x8C14030A,  //  0013  GETMET	R5	R1	K10
      0x581C000B,  //  0014  LDCONST	R7	K11
      0x7C140400,  //  0015  CALL	R5	2
      0x8C14030A,  //  0016  GETMET	R5	R1	K10
      0x581C000C,  //  0017  LDCONST	R7	K12
      0x7C140400,  //  0018  CALL	R5	2
      0x8C14030A,  //  0019  GETMET	R5	R1	K10
      0x581C000D,  //  001A  LDCONST	R7	K13
      0x7C140400,  //  001B  CALL	R5	2
      0x8C14030A,  //  001C  GETMET	R5	R1	K10
      0x581C000E,  //  001D  LDCONST	R7	K14
      0x7C140400,  //  001E  CALL	R5	2
      0x8C14030A,  //  001F  GETMET	R5	R1	K10
      0x581C000F,  //  0020  LDCONST	R7	K15
      0x7C140400,  //  0021  CALL	R5	2
      0x88140104,  //  0022  GETMBR	R5	R0	K4
      0x88140B10,  //  0023  GETMBR	R5	R5	K16
      0x8C140B11,  //  0024  GETMET	R5	R5	K17
      0x581C0012,  //  0025  LDCONST	R7	K18
      0x7C140400,  //  0026  CALL	R5	2
      0x88140104,  //  0027  GETMBR	R5	R0	K4
      0x8C140B13,  //  0028  GETMET	R5	R5	K19
      0x881C0104,  //  0029  GETMBR	R7	R0	K4
      0x881C0F10,  //  002A  GETMBR	R7	R7	K16
      0x7C140400,  //  002B  CALL	R5	2
      0x58180014,  //  002C  LDCONST	R6	K20
      0x501C0000,  //  002D  LDBOOL	R7	0	0
      0x6020000C,  //  002E  GETGBL	R8	G12
      0x5C240A00,  //  002F  MOVE	R9	R5
      0x7C200200,  //  0030  CALL	R8	1
      0x14200C08,  //  0031  LT	R8	R6	R8
      0x78220020,  //  0032  JMPF	R8	#0054
      0x94200A06,  //  0033  GETIDX	R8	R5	R6
      0x88240104,  //  0034  GETMBR	R9	R0	K4
      0x88241310,  //  0035  GETMBR	R9	R9	K16
      0x8C241315,  //  0036  GETMET	R9	R9	K21
      0x602C0008,  //  0037  GETGBL	R11	G8
      0x5C301000,  //  0038  MOVE	R12	R8
      0x7C2C0200,  //  0039  CALL	R11	1
      0x7C240400,  //  003A  CALL	R9	2
      0x8C281315,  //  003B  GETMET	R10	R9	K21
      0x58300016,  //  003C  LDCONST	R12	K22
      0x7C280400,  //  003D  CALL	R10	2
      0x5C2C1400,  //  003E  MOVE	R11	R10
      0x742E0001,  //  003F  JMPT	R11	#0042
      0x00180D17,  //  0040  ADD	R6	R6	K23
      0x7001FFEB,  //  0041  JMP		#002E
      0x8C2C0515,  //  0042  GETMET	R11	R2	K21
      0x5C341400,  //  0043  MOVE	R13	R10
      0x58380018,  //  0044  LDCONST	R14	K24
      0x7C2C0600,  //  0045  CALL	R11	3
      0x1C2C1714,  //  0046  EQ	R11	R11	K20
      0x782E0001,  //  0047  JMPF	R11	#004A
      0x00180D17,  //  0048  ADD	R6	R6	K23
      0x7001FFE3,  //  0049  JMP		#002E
      0x501C0200,  //  004A  LDBOOL	R7	1	0
      0x8C2C030A,  //  004B  GETMET	R11	R1	K10
      0x60340018,  //  004C  GETGBL	R13	G24
      0x58380019,  //  004D  LDCONST	R14	K25
      0x5C3C1000,  //  004E  MOVE	R15	R8
      0x5C401000,  //  004F  MOVE	R16	R8
      0x7C340600,  //  0050  CALL	R13	3
      0x7C2C0400,  //  0051  CALL	R11	2
      0x00180D17,  //  0052  ADD	R6	R6	K23
      0x7001FFD9,  //  0053  JMP		#002E
      0x8C20030A,  //  0054  GETMET	R8	R1	K10
      0x5828001A,  //  0055  LDCONST	R10	K26
      0x7C200400,  //  0056  CALL	R8	2
      0x5C200E00,  //  0057  MOVE	R8	R7
      0x74220002,  //  0058  JMPT	R8	#005C
      0x8C20030A,  //  0059  GETMET	R8	R1	K10
      0x5828001B,  //  005A  LDCONST	R10	K27
      0x7C200400,  //  005B  CALL	R8	2
      0x8C20030A,  //  005C  GETMET	R8	R1	K10
      0x5828001C,  //  005D  LDCONST	R10	K28
      0x7C200400,  //  005E  CALL	R8	2
      0x60200012,  //  005F  GETGBL	R8	G18
      0x7C200000,  //  0060  CALL	R8	0
      0x60240010,  //  0061  GETGBL	R9	G16
      0x88280104,  //  0062  GETMBR	R10	R0	K4
      0x88281510,  //  0063  GETMBR	R10	R10	K16
      0x7C240200,  //  0064  CALL	R9	1
      0xA802000B,  //  0065  EXBLK	0	#0072
      0x5C281200,  //  0066  MOVE	R10	R9
      0x7C280000,  //  0067  CALL	R10	0
      0x8C2C1515,  //  0068  GETMET	R11	R10	K21
      0x5834001D,  //  0069  LDCONST	R13	K29
      0x7C2C0400,  //  006A  CALL	R11	2
      0x4C300000,  //  006B  LDNIL	R12
      0x2030160C,  //  006C  NE	R12	R11	R12
      0x78320002,  //  006D  JMPF	R12	#0071
      0x8C30111E,  //  006E  GETMET	R12	R8	K30
      0x5C381600,  //  006F  MOVE	R14	R11
      0x7C300400,  //  0070  CALL	R12	2
      0x7001FFF3,  //  0071  JMP		#0066
      0x5824001F,  //  0072  LDCONST	R9	K31
      0xAC240200,  //  0073  CATCH	R9	1	0
      0xB0080000,  //  0074  RAISE	2	R0	R0
      0x88240104,  //  0075  GETMBR	R9	R0	K4
      0x8C241320,  //  0076  GETMET	R9	R9	K32
      0x5C2C1000,  //  0077  MOVE	R11	R8
      0x7C240400,  //  0078  CALL	R9	2
      0x60240010,  //  0079  GETGBL	R9	G16
      0x5C281000,  //  007A  MOVE	R10	R8
      0x7C240200,  //  007B  CALL	R9	1
      0xA8020051,  //  007C  EXBLK	0	#00CF
      0x5C281200,  //  007D  MOVE	R10	R9
      0x7C280000,  //  007E  CALL	R10	0
      0x8C2C0321,  //  007F  GETMET	R11	R1	K33
      0x5C341400,  //  0080  MOVE	R13	R10
      0x7C2C0400,  //  0081  CALL	R11	2
      0x8C300321,  //  0082  GETMET	R12	R1	K33
      0x88380104,  //  0083  GETMBR	R14	R0	K4
      0x8C381D22,  //  0084  GETMET	R14	R14	K34
      0x5C401400,  //  0085  MOVE	R16	R10
      0x7C380400,  //  0086  CALL	R14	2
      0x8C381D15,  //  0087  GETMET	R14	R14	K21
      0x58400023,  //  0088  LDCONST	R16	K35
      0x5C441400,  //  0089  MOVE	R17	R10
      0x7C380600,  //  008A  CALL	R14	3
      0x7C300400,  //  008B  CALL	R12	2
      0x8C34030A,  //  008C  GETMET	R13	R1	K10
      0x603C0018,  //  008D  GETGBL	R15	G24
      0x58400024,  //  008E  LDCONST	R16	K36
      0x5C441600,  //  008F  MOVE	R17	R11
      0x5C481600,  //  0090  MOVE	R18	R11
      0x5C4C1800,  //  0091  MOVE	R19	R12
      0x7C3C0800,  //  0092  CALL	R15	4
      0x7C340400,  //  0093  CALL	R13	2
      0x8C34030A,  //  0094  GETMET	R13	R1	K10
      0x583C0025,  //  0095  LDCONST	R15	K37
      0x7C340400,  //  0096  CALL	R13	2
      0x501C0000,  //  0097  LDBOOL	R7	0	0
      0x58180014,  //  0098  LDCONST	R6	K20
      0x6034000C,  //  0099  GETGBL	R13	G12
      0x5C380A00,  //  009A  MOVE	R14	R5
      0x7C340200,  //  009B  CALL	R13	1
      0x14340C0D,  //  009C  LT	R13	R6	R13
      0x78360027,  //  009D  JMPF	R13	#00C6
      0x94340A06,  //  009E  GETIDX	R13	R5	R6
      0x88380104,  //  009F  GETMBR	R14	R0	K4
      0x88381D10,  //  00A0  GETMBR	R14	R14	K16
      0x8C381D15,  //  00A1  GETMET	R14	R14	K21
      0x60400008,  //  00A2  GETGBL	R16	G8
      0x5C441A00,  //  00A3  MOVE	R17	R13
      0x7C400200,  //  00A4  CALL	R16	1
      0x7C380400,  //  00A5  CALL	R14	2
      0x8C3C1D15,  //  00A6  GETMET	R15	R14	K21
      0x58440016,  //  00A7  LDCONST	R17	K22
      0x7C3C0400,  //  00A8  CALL	R15	2
      0x5C401E00,  //  00A9  MOVE	R16	R15
      0x74420001,  //  00AA  JMPT	R16	#00AD
      0x00180D17,  //  00AB  ADD	R6	R6	K23
      0x7001FFEB,  //  00AC  JMP		#0099
      0x8C400515,  //  00AD  GETMET	R16	R2	K21
      0x5C481E00,  //  00AE  MOVE	R18	R15
      0x584C0018,  //  00AF  LDCONST	R19	K24
      0x7C400600,  //  00B0  CALL	R16	3
      0x20402114,  //  00B1  NE	R16	R16	K20
      0x78420001,  //  00B2  JMPF	R16	#00B5
      0x00180D17,  //  00B3  ADD	R6	R6	K23
      0x7001FFE3,  //  00B4  JMP		#0099
      0x8C401D15,  //  00B5  GETMET	R16	R14	K21
      0x5848001D,  //  00B6  LDCONST	R18	K29
      0x7C400400,  //  00B7  CALL	R16	2
      0x2040200A,  //  00B8  NE	R16	R16	R10
      0x78420001,  //  00B9  JMPF	R16	#00BC
      0x00180D17,  //  00BA  ADD	R6	R6	K23
      0x7001FFDC,  //  00BB  JMP		#0099
      0x501C0200,  //  00BC  LDBOOL	R7	1	0
      0x8C40030A,  //  00BD  GETMET	R16	R1	K10
      0x60480018,  //  00BE  GETGBL	R18	G24
      0x584C0019,  //  00BF  LDCONST	R19	K25
      0x5C501A00,  //  00C0  MOVE	R20	R13
      0x5C541A00,  //  00C1  MOVE	R21	R13
      0x7C480600,  //  00C2  CALL	R18	3
      0x7C400400,  //  00C3  CALL	R16	2
      0x00180D17,  //  00C4  ADD	R6	R6	K23
      0x7001FFD2,  //  00C5  JMP		#0099
      0x8C34030A,  //  00C6  GETMET	R13	R1	K10
      0x583C0026,  //  00C7  LDCONST	R15	K38
      0x7C340400,  //  00C8  CALL	R13	2
      0x5C340E00,  //  00C9  MOVE	R13	R7
      0x74360002,  //  00CA  JMPT	R13	#00CE
      0x8C34030A,  //  00CB  GETMET	R13	R1	K10
      0x583C0027,  //  00CC  LDCONST	R15	K39
      0x7C340400,  //  00CD  CALL	R13	2
      0x7001FFAD,  //  00CE  JMP		#007D
      0x5824001F,  //  00CF  LDCONST	R9	K31
      0xAC240200,  //  00D0  CATCH	R9	1	0
      0xB0080000,  //  00D1  RAISE	2	R0	R0
      0x8C24030A,  //  00D2  GETMET	R9	R1	K10
      0x582C0028,  //  00D3  LDCONST	R11	K40
      0x7C240400,  //  00D4  CALL	R9	2
      0x8C24030A,  //  00D5  GETMET	R9	R1	K10
      0x582C0029,  //  00D6  LDCONST	R11	K41
      0x7C240400,  //  00D7  CALL	R9	2
      0x8C24030A,  //  00D8  GETMET	R9	R1	K10
      0x582C002A,  //  00D9  LDCONST	R11	K42
      0x7C240400,  //  00DA  CALL	R9	2
      0x80000000,  //  00DB  RET	0
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
    ( &(const binstruction[22]) {  /* code */
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
      0x88080105,  //  000B  GETMBR	R2	R0	K5
      0x780A0003,  //  000C  JMPF	R2	#0011
      0x8C080106,  //  000D  GETMET	R2	R0	K6
      0x7C080200,  //  000E  CALL	R2	1
      0x8C080107,  //  000F  GETMET	R2	R0	K7
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080108,  //  0011  GETMET	R2	R0	K8
      0x7C080200,  //  0012  CALL	R2	1
      0x8C080309,  //  0013  GETMET	R2	R1	K9
      0x7C080200,  //  0014  CALL	R2	1
      0x80000000,  //  0015  RET	0
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
    ( &(const binstruction[49]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x780A002C,  //  0002  JMPF	R2	#0030
      0x88080102,  //  0003  GETMBR	R2	R0	K2
      0x88080503,  //  0004  GETMBR	R2	R2	K3
      0x4C0C0000,  //  0005  LDNIL	R3
      0x20080403,  //  0006  NE	R2	R2	R3
      0x780A0004,  //  0007  JMPF	R2	#000D
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x88080503,  //  0009  GETMBR	R2	R2	K3
      0x8C080504,  //  000A  GETMET	R2	R2	K4
      0x7C080200,  //  000B  CALL	R2	1
      0x70020000,  //  000C  JMP		#000E
      0x58080005,  //  000D  LDCONST	R2	K5
      0x1C0C0505,  //  000E  EQ	R3	R2	K5
      0x780E0006,  //  000F  JMPF	R3	#0017
      0x8C0C0306,  //  0010  GETMET	R3	R1	K6
      0x60140018,  //  0011  GETGBL	R5	G24
      0x58180007,  //  0012  LDCONST	R6	K7
      0x581C0008,  //  0013  LDCONST	R7	K8
      0x7C140400,  //  0014  CALL	R5	2
      0x7C0C0400,  //  0015  CALL	R3	2
      0x7002000F,  //  0016  JMP		#0027
      0x240C0509,  //  0017  GT	R3	R2	K9
      0x8C100306,  //  0018  GETMET	R4	R1	K6
      0x60180018,  //  0019  GETGBL	R6	G24
      0x581C0007,  //  001A  LDCONST	R7	K7
      0x60200008,  //  001B  GETGBL	R8	G8
      0x5C240400,  //  001C  MOVE	R9	R2
      0x7C200200,  //  001D  CALL	R8	1
      0x00221408,  //  001E  ADD	R8	K10	R8
      0x0020110B,  //  001F  ADD	R8	R8	K11
      0x780E0001,  //  0020  JMPF	R3	#0023
      0x5824000C,  //  0021  LDCONST	R9	K12
      0x70020000,  //  0022  JMP		#0024
      0x5824000D,  //  0023  LDCONST	R9	K13
      0x00201009,  //  0024  ADD	R8	R8	R9
      0x7C180400,  //  0025  CALL	R6	2
      0x7C100400,  //  0026  CALL	R4	2
      0x8C0C010E,  //  0027  GETMET	R3	R0	K14
      0x7C0C0200,  //  0028  CALL	R3	1
      0x880C0102,  //  0029  GETMBR	R3	R0	K2
      0x880C070F,  //  002A  GETMBR	R3	R3	K15
      0x8C0C0710,  //  002B  GETMET	R3	R3	K16
      0x7C0C0200,  //  002C  CALL	R3	1
      0x780E0001,  //  002D  JMPF	R3	#0030
      0x8C0C0111,  //  002E  GETMET	R3	R0	K17
      0x7C0C0200,  //  002F  CALL	R3	1
      0x80000000,  //  0030  RET	0
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
** Solidified function: generate_config_js
********************************************************************/
be_local_closure(class_Matter_UI_generate_config_js,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(webserver),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(var_X20cfg_X3D),
    /* K4   */  be_nested_str_weak(dump),
    /* K5   */  be_nested_str_weak(device),
    /* K6   */  be_nested_str_weak(plugins_config),
    /* K7   */  be_nested_str_weak(_X3B),
    /* K8   */  be_nested_str_weak(var_X20nextep_X3D_X25i_X3B),
    /* K9   */  be_nested_str_weak(next_ep),
    }),
    be_str_weak(generate_config_js),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0502,  //  0002  GETMET	R3	R2	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C0C0502,  //  0005  GETMET	R3	R2	K2
      0x8C140304,  //  0006  GETMET	R5	R1	K4
      0x881C0105,  //  0007  GETMBR	R7	R0	K5
      0x881C0F06,  //  0008  GETMBR	R7	R7	K6
      0x7C140400,  //  0009  CALL	R5	2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x8C0C0502,  //  000B  GETMET	R3	R2	K2
      0x58140007,  //  000C  LDCONST	R5	K7
      0x7C0C0400,  //  000D  CALL	R3	2
      0x8C0C0502,  //  000E  GETMET	R3	R2	K2
      0x60140018,  //  000F  GETGBL	R5	G24
      0x58180008,  //  0010  LDCONST	R6	K8
      0x881C0105,  //  0011  GETMBR	R7	R0	K5
      0x881C0F09,  //  0012  GETMBR	R7	R7	K9
      0x7C140400,  //  0013  CALL	R5	2
      0x7C0C0400,  //  0014  CALL	R3	2
      0x80000000,  //  0015  RET	0
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
** Solidified function: generate_display_names_js
********************************************************************/
be_local_closure(class_Matter_UI_generate_display_names_js,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(split),
    /* K4   */  be_nested_str_weak(_X7C),
    /* K5   */  be_nested_str_weak(stop_iteration),
    /* K6   */  be_nested_str_weak(content_send),
    /* K7   */  be_nested_str_weak(var_X20dn_X3D_X7B),
    /* K8   */  be_nested_str_weak(),
    /* K9   */  be_nested_str_weak(_X2C),
    /* K10  */  be_nested_str_weak(_X2Dvirtual),
    /* K11  */  be_nested_str_weak(_X22_X2Dvirtual_X22_X3Atrue),
    /* K12  */  be_nested_str_weak(_X2Dzigbee),
    /* K13  */  be_nested_str_weak(_X22_X2Dzigbee_X22_X3Atrue),
    /* K14  */  be_nested_str_weak(device),
    /* K15  */  be_nested_str_weak(get_plugin_class_displayname),
    /* K16  */  be_nested_str_weak(_X22_X25s_X22_X3A_X22_X25s_X22),
    /* K17  */  be_nested_str_weak(_X7D_X3B),
    }),
    be_str_weak(generate_display_names_js),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
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
      0x8C180506,  //  0014  GETMET	R6	R2	K6
      0x58200007,  //  0015  LDCONST	R8	K7
      0x7C180400,  //  0016  CALL	R6	2
      0x50180200,  //  0017  LDBOOL	R6	1	0
      0x601C0010,  //  0018  GETGBL	R7	G16
      0x5C200A00,  //  0019  MOVE	R8	R5
      0x7C1C0200,  //  001A  CALL	R7	1
      0xA8020022,  //  001B  EXBLK	0	#003F
      0x5C200E00,  //  001C  MOVE	R8	R7
      0x7C200000,  //  001D  CALL	R8	0
      0x1C241108,  //  001E  EQ	R9	R8	K8
      0x78260000,  //  001F  JMPF	R9	#0021
      0x7001FFFA,  //  0020  JMP		#001C
      0x5C240C00,  //  0021  MOVE	R9	R6
      0x74260002,  //  0022  JMPT	R9	#0026
      0x8C240506,  //  0023  GETMET	R9	R2	K6
      0x582C0009,  //  0024  LDCONST	R11	K9
      0x7C240400,  //  0025  CALL	R9	2
      0x50180000,  //  0026  LDBOOL	R6	0	0
      0x1C24110A,  //  0027  EQ	R9	R8	K10
      0x78260003,  //  0028  JMPF	R9	#002D
      0x8C240506,  //  0029  GETMET	R9	R2	K6
      0x582C000B,  //  002A  LDCONST	R11	K11
      0x7C240400,  //  002B  CALL	R9	2
      0x70020010,  //  002C  JMP		#003E
      0x1C24110C,  //  002D  EQ	R9	R8	K12
      0x78260003,  //  002E  JMPF	R9	#0033
      0x8C240506,  //  002F  GETMET	R9	R2	K6
      0x582C000D,  //  0030  LDCONST	R11	K13
      0x7C240400,  //  0031  CALL	R9	2
      0x7002000A,  //  0032  JMP		#003E
      0x8824010E,  //  0033  GETMBR	R9	R0	K14
      0x8C24130F,  //  0034  GETMET	R9	R9	K15
      0x5C2C1000,  //  0035  MOVE	R11	R8
      0x7C240400,  //  0036  CALL	R9	2
      0x8C280506,  //  0037  GETMET	R10	R2	K6
      0x60300018,  //  0038  GETGBL	R12	G24
      0x58340010,  //  0039  LDCONST	R13	K16
      0x5C381000,  //  003A  MOVE	R14	R8
      0x5C3C1200,  //  003B  MOVE	R15	R9
      0x7C300600,  //  003C  CALL	R12	3
      0x7C280400,  //  003D  CALL	R10	2
      0x7001FFDC,  //  003E  JMP		#001C
      0x581C0005,  //  003F  LDCONST	R7	K5
      0xAC1C0200,  //  0040  CATCH	R7	1	0
      0xB0080000,  //  0041  RAISE	2	R0	R0
      0x8C1C0506,  //  0042  GETMET	R7	R2	K6
      0x58240011,  //  0043  LDCONST	R9	K17
      0x7C1C0400,  //  0044  CALL	R7	2
      0x80000000,  //  0045  RET	0
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
** Solidified function: reconcile_plugins
********************************************************************/
be_local_closure(class_Matter_UI_reconcile_plugins,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(keys),
    /* K1   */  be_nested_str_weak(stop_iteration),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(plugins),
    /* K5   */  be_nested_str_weak(get_endpoint),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(contains),
    /* K8   */  be_nested_str_weak(remove),
    /* K9   */  be_nested_str_weak(find),
    /* K10  */  be_nested_str_weak(type),
    /* K11  */  be_nested_str_weak(plugins_classes),
    /* K12  */  be_nested_str_weak(push),
    /* K13  */  be_nested_str_weak(parse_configuration),
    /* K14  */  be_nested_str_weak(name),
    /* K15  */  be_nested_str_weak(set_name),
    /* K16  */  be_nested_str_weak(log),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20adding_X20endpoint_X20_X3D_X20_X25i_X20type_X3A_X25s),
    /* K18  */  be_const_int(2),
    /* K19  */  be_nested_str_weak(clean_remotes),
    /* K20  */  be_nested_str_weak(signal_endpoints_changed),
    }),
    be_str_weak(reconcile_plugins),
    &be_const_str_solidified,
    ( &(const binstruction[181]) {  /* code */
      0x600C0013,  //  0000  GETGBL	R3	G19
      0x7C0C0000,  //  0001  CALL	R3	0
      0x60100010,  //  0002  GETGBL	R4	G16
      0x8C140500,  //  0003  GETMET	R5	R2	K0
      0x7C140200,  //  0004  CALL	R5	1
      0x7C100200,  //  0005  CALL	R4	1
      0xA8020007,  //  0006  EXBLK	0	#000F
      0x5C140800,  //  0007  MOVE	R5	R4
      0x7C140000,  //  0008  CALL	R5	0
      0x60180009,  //  0009  GETGBL	R6	G9
      0x5C1C0A00,  //  000A  MOVE	R7	R5
      0x7C180200,  //  000B  CALL	R6	1
      0x501C0200,  //  000C  LDBOOL	R7	1	0
      0x980C0C07,  //  000D  SETIDX	R3	R6	R7
      0x7001FFF7,  //  000E  JMP		#0007
      0x58100001,  //  000F  LDCONST	R4	K1
      0xAC100200,  //  0010  CATCH	R4	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x58100002,  //  0012  LDCONST	R4	K2
      0x6014000C,  //  0013  GETGBL	R5	G12
      0x88180103,  //  0014  GETMBR	R6	R0	K3
      0x88180D04,  //  0015  GETMBR	R6	R6	K4
      0x7C140200,  //  0016  CALL	R5	1
      0x14140805,  //  0017  LT	R5	R4	R5
      0x78160012,  //  0018  JMPF	R5	#002C
      0x88140103,  //  0019  GETMBR	R5	R0	K3
      0x88140B04,  //  001A  GETMBR	R5	R5	K4
      0x94140A04,  //  001B  GETIDX	R5	R5	R4
      0x8C180B05,  //  001C  GETMET	R6	R5	K5
      0x7C180200,  //  001D  CALL	R6	1
      0x241C0D06,  //  001E  GT	R7	R6	K6
      0x781E0009,  //  001F  JMPF	R7	#002A
      0x8C1C0707,  //  0020  GETMET	R7	R3	K7
      0x5C240C00,  //  0021  MOVE	R9	R6
      0x7C1C0400,  //  0022  CALL	R7	2
      0x741E0005,  //  0023  JMPT	R7	#002A
      0x881C0103,  //  0024  GETMBR	R7	R0	K3
      0x881C0F04,  //  0025  GETMBR	R7	R7	K4
      0x8C1C0F08,  //  0026  GETMET	R7	R7	K8
      0x5C240800,  //  0027  MOVE	R9	R4
      0x7C1C0400,  //  0028  CALL	R7	2
      0x70020000,  //  0029  JMP		#002B
      0x00100906,  //  002A  ADD	R4	R4	K6
      0x7001FFE6,  //  002B  JMP		#0013
      0x60140013,  //  002C  GETGBL	R5	G19
      0x7C140000,  //  002D  CALL	R5	0
      0x60180010,  //  002E  GETGBL	R6	G16
      0x881C0103,  //  002F  GETMBR	R7	R0	K3
      0x881C0F04,  //  0030  GETMBR	R7	R7	K4
      0x7C180200,  //  0031  CALL	R6	1
      0xA8020005,  //  0032  EXBLK	0	#0039
      0x5C1C0C00,  //  0033  MOVE	R7	R6
      0x7C1C0000,  //  0034  CALL	R7	0
      0x8C200F05,  //  0035  GETMET	R8	R7	K5
      0x7C200200,  //  0036  CALL	R8	1
      0x98141007,  //  0037  SETIDX	R5	R8	R7
      0x7001FFF9,  //  0038  JMP		#0033
      0x58180001,  //  0039  LDCONST	R6	K1
      0xAC180200,  //  003A  CATCH	R6	1	0
      0xB0080000,  //  003B  RAISE	2	R0	R0
      0x60180010,  //  003C  GETGBL	R6	G16
      0x8C1C0500,  //  003D  GETMET	R7	R2	K0
      0x7C1C0200,  //  003E  CALL	R7	1
      0x7C180200,  //  003F  CALL	R6	1
      0xA8020069,  //  0040  EXBLK	0	#00AB
      0x5C1C0C00,  //  0041  MOVE	R7	R6
      0x7C1C0000,  //  0042  CALL	R7	0
      0x60200009,  //  0043  GETGBL	R8	G9
      0x5C240E00,  //  0044  MOVE	R9	R7
      0x7C200200,  //  0045  CALL	R8	1
      0x94240407,  //  0046  GETIDX	R9	R2	R7
      0x8C281309,  //  0047  GETMET	R10	R9	K9
      0x5830000A,  //  0048  LDCONST	R12	K10
      0x7C280400,  //  0049  CALL	R10	2
      0x8C2C0B07,  //  004A  GETMET	R11	R5	K7
      0x5C341000,  //  004B  MOVE	R13	R8
      0x7C2C0400,  //  004C  CALL	R11	2
      0x782E0041,  //  004D  JMPF	R11	#0090
      0x8C2C0309,  //  004E  GETMET	R11	R1	K9
      0x5C340E00,  //  004F  MOVE	R13	R7
      0x7C2C0400,  //  0050  CALL	R11	2
      0x782E0003,  //  0051  JMPF	R11	#0056
      0x8C301709,  //  0052  GETMET	R12	R11	K9
      0x5838000A,  //  0053  LDCONST	R14	K10
      0x7C300400,  //  0054  CALL	R12	2
      0x70020000,  //  0055  JMP		#0057
      0x4C300000,  //  0056  LDNIL	R12
      0x2034180A,  //  0057  NE	R13	R12	R10
      0x78360028,  //  0058  JMPF	R13	#0082
      0x58340002,  //  0059  LDCONST	R13	K2
      0x6038000C,  //  005A  GETGBL	R14	G12
      0x883C0103,  //  005B  GETMBR	R15	R0	K3
      0x883C1F04,  //  005C  GETMBR	R15	R15	K4
      0x7C380200,  //  005D  CALL	R14	1
      0x14381A0E,  //  005E  LT	R14	R13	R14
      0x783A000E,  //  005F  JMPF	R14	#006F
      0x88380103,  //  0060  GETMBR	R14	R0	K3
      0x88381D04,  //  0061  GETMBR	R14	R14	K4
      0x94381C0D,  //  0062  GETIDX	R14	R14	R13
      0x8C381D05,  //  0063  GETMET	R14	R14	K5
      0x7C380200,  //  0064  CALL	R14	1
      0x1C381C08,  //  0065  EQ	R14	R14	R8
      0x783A0005,  //  0066  JMPF	R14	#006D
      0x88380103,  //  0067  GETMBR	R14	R0	K3
      0x88381D04,  //  0068  GETMBR	R14	R14	K4
      0x8C381D08,  //  0069  GETMET	R14	R14	K8
      0x5C401A00,  //  006A  MOVE	R16	R13
      0x7C380400,  //  006B  CALL	R14	2
      0x70020001,  //  006C  JMP		#006F
      0x00341B06,  //  006D  ADD	R13	R13	K6
      0x7001FFEA,  //  006E  JMP		#005A
      0x88380103,  //  006F  GETMBR	R14	R0	K3
      0x88381D0B,  //  0070  GETMBR	R14	R14	K11
      0x8C381D09,  //  0071  GETMET	R14	R14	K9
      0x5C401400,  //  0072  MOVE	R16	R10
      0x7C380400,  //  0073  CALL	R14	2
      0x4C3C0000,  //  0074  LDNIL	R15
      0x203C1C0F,  //  0075  NE	R15	R14	R15
      0x783E0009,  //  0076  JMPF	R15	#0081
      0x5C3C1C00,  //  0077  MOVE	R15	R14
      0x88400103,  //  0078  GETMBR	R16	R0	K3
      0x5C441000,  //  0079  MOVE	R17	R8
      0x5C481200,  //  007A  MOVE	R18	R9
      0x7C3C0600,  //  007B  CALL	R15	3
      0x88400103,  //  007C  GETMBR	R16	R0	K3
      0x88402104,  //  007D  GETMBR	R16	R16	K4
      0x8C40210C,  //  007E  GETMET	R16	R16	K12
      0x5C481E00,  //  007F  MOVE	R18	R15
      0x7C400400,  //  0080  CALL	R16	2
      0x7002000C,  //  0081  JMP		#008F
      0x94340A08,  //  0082  GETIDX	R13	R5	R8
      0x8C381B0D,  //  0083  GETMET	R14	R13	K13
      0x5C401200,  //  0084  MOVE	R16	R9
      0x7C380400,  //  0085  CALL	R14	2
      0x8C381309,  //  0086  GETMET	R14	R9	K9
      0x5840000E,  //  0087  LDCONST	R16	K14
      0x7C380400,  //  0088  CALL	R14	2
      0x4C3C0000,  //  0089  LDNIL	R15
      0x203C1C0F,  //  008A  NE	R15	R14	R15
      0x783E0002,  //  008B  JMPF	R15	#008F
      0x8C3C1B0F,  //  008C  GETMET	R15	R13	K15
      0x5C441C00,  //  008D  MOVE	R17	R14
      0x7C3C0400,  //  008E  CALL	R15	2
      0x70020019,  //  008F  JMP		#00AA
      0x882C0103,  //  0090  GETMBR	R11	R0	K3
      0x882C170B,  //  0091  GETMBR	R11	R11	K11
      0x8C2C1709,  //  0092  GETMET	R11	R11	K9
      0x5C341400,  //  0093  MOVE	R13	R10
      0x7C2C0400,  //  0094  CALL	R11	2
      0x4C300000,  //  0095  LDNIL	R12
      0x2030160C,  //  0096  NE	R12	R11	R12
      0x78320011,  //  0097  JMPF	R12	#00AA
      0x5C301600,  //  0098  MOVE	R12	R11
      0x88340103,  //  0099  GETMBR	R13	R0	K3
      0x5C381000,  //  009A  MOVE	R14	R8
      0x5C3C1200,  //  009B  MOVE	R15	R9
      0x7C300600,  //  009C  CALL	R12	3
      0x88340103,  //  009D  GETMBR	R13	R0	K3
      0x88341B04,  //  009E  GETMBR	R13	R13	K4
      0x8C341B0C,  //  009F  GETMET	R13	R13	K12
      0x5C3C1800,  //  00A0  MOVE	R15	R12
      0x7C340400,  //  00A1  CALL	R13	2
      0xB8362000,  //  00A2  GETNGBL	R13	K16
      0x60380018,  //  00A3  GETGBL	R14	G24
      0x583C0011,  //  00A4  LDCONST	R15	K17
      0x5C401000,  //  00A5  MOVE	R16	R8
      0x5C441400,  //  00A6  MOVE	R17	R10
      0x7C380600,  //  00A7  CALL	R14	3
      0x583C0012,  //  00A8  LDCONST	R15	K18
      0x7C340400,  //  00A9  CALL	R13	2
      0x7001FF95,  //  00AA  JMP		#0041
      0x58180001,  //  00AB  LDCONST	R6	K1
      0xAC180200,  //  00AC  CATCH	R6	1	0
      0xB0080000,  //  00AD  RAISE	2	R0	R0
      0x88180103,  //  00AE  GETMBR	R6	R0	K3
      0x8C180D13,  //  00AF  GETMET	R6	R6	K19
      0x7C180200,  //  00B0  CALL	R6	1
      0x88180103,  //  00B1  GETMBR	R6	R0	K3
      0x8C180D14,  //  00B2  GETMET	R6	R6	K20
      0x7C180200,  //  00B3  CALL	R6	1
      0x80000000,  //  00B4  RET	0
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
    ( &(const bvalue[26]) {     /* constants */
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
    /* K13  */  be_nested_str_long(_X3Cstyle_X3E_X2Ebxm_X7Bheight_X3A14px_X3Bwidth_X3A14px_X3Bdisplay_X3Ainline_X2Dblock_X3Bborder_X3A1px_X20solid_X20currentColor_X3Bbackground_X2Dcolor_X3Avar_X28_X2D_X2Dcl_X2C_X23fff_X29_X7D_X2Eztdm_X20td_X3Anot_X28_X3Afirst_X2Dchild_X29_X7Bwidth_X3A20px_X3Bfont_X2Dsize_X3A70_X25_X7D_X2Eztdm_X20td_X3Alast_X2Dchild_X7Bwidth_X3A45px_X7D_X2Eztdm_X20_X2Ebt_X7Bmargin_X2Dright_X3A10px_X3B_X7D_X2Ehtrm_X7Bline_X2Dheight_X3A20px_X7D_X3C_X2Fstyle_X3E),
    /* K14  */  be_nested_str_weak(k2l),
    /* K15  */  be_nested_str_weak(html_escape),
    /* K16  */  be_nested_str_weak(get_plugin_remote_info),
    /* K17  */  be_nested_str_weak(find),
    /* K18  */  be_nested_str_weak(name),
    /* K19  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27ztdm_X20htrm_X27_X3E_X3Ctd_X3E_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20title_X3D_X27http_X3A_X2F_X2F_X25s_X2F_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X3F_X22_X27_X3E_X25s_X3C_X2Fa_X3E_X3C_X2Ftd_X3E),
    /* K20  */  be_nested_str_weak(web_last_seen),
    /* K21  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27htrm_X27_X3E_X3Ctd_X20colspan_X3D_X272_X27_X3E),
    /* K22  */  be_nested_str_weak(web_values),
    /* K23  */  be_nested_str_weak(_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K24  */  be_nested_str_weak(stop_iteration),
    /* K25  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Chr_X3E),
    }),
    be_str_weak(show_bridge_status),
    &be_const_str_solidified,
    ( &(const binstruction[117]) {  /* code */
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
      0x5818000D,  //  0034  LDCONST	R6	K13
      0x7C100400,  //  0035  CALL	R4	2
      0x60100010,  //  0036  GETGBL	R4	G16
      0x88140100,  //  0037  GETMBR	R5	R0	K0
      0x8C140B0E,  //  0038  GETMET	R5	R5	K14
      0x5C1C0400,  //  0039  MOVE	R7	R2
      0x7C140400,  //  003A  CALL	R5	2
      0x7C100200,  //  003B  CALL	R4	1
      0xA8020030,  //  003C  EXBLK	0	#006E
      0x5C140800,  //  003D  MOVE	R5	R4
      0x7C140000,  //  003E  CALL	R5	0
      0x8C18030F,  //  003F  GETMET	R6	R1	K15
      0x5C200A00,  //  0040  MOVE	R8	R5
      0x7C180400,  //  0041  CALL	R6	2
      0x8C1C030F,  //  0042  GETMET	R7	R1	K15
      0x88240100,  //  0043  GETMBR	R9	R0	K0
      0x8C241310,  //  0044  GETMET	R9	R9	K16
      0x5C2C0A00,  //  0045  MOVE	R11	R5
      0x7C240400,  //  0046  CALL	R9	2
      0x8C241311,  //  0047  GETMET	R9	R9	K17
      0x582C0012,  //  0048  LDCONST	R11	K18
      0x5C300A00,  //  0049  MOVE	R12	R5
      0x7C240600,  //  004A  CALL	R9	3
      0x7C1C0400,  //  004B  CALL	R7	2
      0x8C20030A,  //  004C  GETMET	R8	R1	K10
      0x60280018,  //  004D  GETGBL	R10	G24
      0x582C0013,  //  004E  LDCONST	R11	K19
      0x5C300C00,  //  004F  MOVE	R12	R6
      0x5C340C00,  //  0050  MOVE	R13	R6
      0x5C380E00,  //  0051  MOVE	R14	R7
      0x7C280800,  //  0052  CALL	R10	4
      0x7C200400,  //  0053  CALL	R8	2
      0x94200405,  //  0054  GETIDX	R8	R2	R5
      0x94201103,  //  0055  GETIDX	R8	R8	K3
      0x88201105,  //  0056  GETMBR	R8	R8	K5
      0x8C24030A,  //  0057  GETMET	R9	R1	K10
      0x8C2C1114,  //  0058  GETMET	R11	R8	K20
      0x7C2C0200,  //  0059  CALL	R11	1
      0x7C240400,  //  005A  CALL	R9	2
      0x60240010,  //  005B  GETGBL	R9	G16
      0x94280405,  //  005C  GETIDX	R10	R2	R5
      0x7C240200,  //  005D  CALL	R9	1
      0xA802000A,  //  005E  EXBLK	0	#006A
      0x5C281200,  //  005F  MOVE	R10	R9
      0x7C280000,  //  0060  CALL	R10	0
      0x8C2C030A,  //  0061  GETMET	R11	R1	K10
      0x58340015,  //  0062  LDCONST	R13	K21
      0x7C2C0400,  //  0063  CALL	R11	2
      0x8C2C1516,  //  0064  GETMET	R11	R10	K22
      0x7C2C0200,  //  0065  CALL	R11	1
      0x8C2C030A,  //  0066  GETMET	R11	R1	K10
      0x58340017,  //  0067  LDCONST	R13	K23
      0x7C2C0400,  //  0068  CALL	R11	2
      0x7001FFF4,  //  0069  JMP		#005F
      0x58240018,  //  006A  LDCONST	R9	K24
      0xAC240200,  //  006B  CATCH	R9	1	0
      0xB0080000,  //  006C  RAISE	2	R0	R0
      0x7001FFCE,  //  006D  JMP		#003D
      0x58100018,  //  006E  LDCONST	R4	K24
      0xAC100200,  //  006F  CATCH	R4	1	0
      0xB0080000,  //  0070  RAISE	2	R0	R0
      0x8C10030A,  //  0071  GETMET	R4	R1	K10
      0x58180019,  //  0072  LDCONST	R6	K25
      0x7C100400,  //  0073  CALL	R4	2
      0x80000000,  //  0074  RET	0
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
    ( &(const binstruction[19]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0004,  //  0004  JMPF	R2	#000A
      0x88080103,  //  0005  GETMBR	R2	R0	K3
      0x88080504,  //  0006  GETMBR	R2	R2	K4
      0x8C080505,  //  0007  GETMET	R2	R2	K5
      0x7C080200,  //  0008  CALL	R2	1
      0x70020007,  //  0009  JMP		#0012
      0x8C080301,  //  000A  GETMET	R2	R1	K1
      0x58100006,  //  000B  LDCONST	R4	K6
      0x7C080400,  //  000C  CALL	R2	2
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x88080103,  //  000E  GETMBR	R2	R0	K3
      0x88080504,  //  000F  GETMBR	R2	R2	K4
      0x8C080507,  //  0010  GETMET	R2	R2	K7
      0x7C080200,  //  0011  CALL	R2	1
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_param_def
********************************************************************/
be_local_closure(class_Matter_UI_parse_param_def,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(split),
    /* K2   */  be_nested_str_weak(_X7C),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(arg),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(find),
    /* K7   */  be_nested_str_weak(_X3A),
    /* K8   */  be_const_int(2147483647),
    /* K9   */  be_nested_str_weak(l),
    /* K10  */  be_nested_str_weak(label),
    /* K11  */  be_nested_str_weak(t),
    /* K12  */  be_nested_str_weak(type),
    /* K13  */  be_nested_str_weak(h),
    /* K14  */  be_nested_str_weak(hint),
    /* K15  */  be_nested_str_weak(d),
    /* K16  */  be_nested_str_weak(default),
    /* K17  */  be_nested_str_weak(r),
    /* K18  */  be_nested_str_weak(required),
    /* K19  */  be_nested_str_weak(0),
    /* K20  */  be_nested_str_weak(o),
    /* K21  */  be_nested_str_weak(options),
    /* K22  */  be_nested_str_weak(parse_options),
    /* K23  */  be_nested_str_weak(stop_iteration),
    /* K24  */  be_nested_str_weak(contains),
    }),
    be_str_weak(parse_param_def),
    &be_const_str_solidified,
    ( &(const binstruction[90]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0013,  //  0001  GETGBL	R3	G19
      0x7C0C0000,  //  0002  CALL	R3	0
      0x8C100501,  //  0003  GETMET	R4	R2	K1
      0x5C180200,  //  0004  MOVE	R6	R1
      0x581C0002,  //  0005  LDCONST	R7	K2
      0x7C100600,  //  0006  CALL	R4	3
      0x6014000C,  //  0007  GETGBL	R5	G12
      0x5C180800,  //  0008  MOVE	R6	R4
      0x7C140200,  //  0009  CALL	R5	1
      0x24140B03,  //  000A  GT	R5	R5	K3
      0x78160001,  //  000B  JMPF	R5	#000E
      0x94140903,  //  000C  GETIDX	R5	R4	K3
      0x980E0805,  //  000D  SETIDX	R3	K4	R5
      0x60140010,  //  000E  GETGBL	R5	G16
      0x6018000C,  //  000F  GETGBL	R6	G12
      0x5C1C0800,  //  0010  MOVE	R7	R4
      0x7C180200,  //  0011  CALL	R6	1
      0x04180D05,  //  0012  SUB	R6	R6	K5
      0x401A0A06,  //  0013  CONNECT	R6	K5	R6
      0x7C140200,  //  0014  CALL	R5	1
      0xA802002A,  //  0015  EXBLK	0	#0041
      0x5C180A00,  //  0016  MOVE	R6	R5
      0x7C180000,  //  0017  CALL	R6	0
      0x941C0806,  //  0018  GETIDX	R7	R4	R6
      0x8C200506,  //  0019  GETMET	R8	R2	K6
      0x5C280E00,  //  001A  MOVE	R10	R7
      0x582C0007,  //  001B  LDCONST	R11	K7
      0x7C200600,  //  001C  CALL	R8	3
      0x28241103,  //  001D  GE	R9	R8	K3
      0x78260020,  //  001E  JMPF	R9	#0040
      0x04241105,  //  001F  SUB	R9	R8	K5
      0x40260609,  //  0020  CONNECT	R9	K3	R9
      0x94240E09,  //  0021  GETIDX	R9	R7	R9
      0x00281105,  //  0022  ADD	R10	R8	K5
      0x40281508,  //  0023  CONNECT	R10	R10	K8
      0x94280E0A,  //  0024  GETIDX	R10	R7	R10
      0x1C2C1309,  //  0025  EQ	R11	R9	K9
      0x782E0001,  //  0026  JMPF	R11	#0029
      0x980E140A,  //  0027  SETIDX	R3	K10	R10
      0x70020016,  //  0028  JMP		#0040
      0x1C2C130B,  //  0029  EQ	R11	R9	K11
      0x782E0001,  //  002A  JMPF	R11	#002D
      0x980E180A,  //  002B  SETIDX	R3	K12	R10
      0x70020012,  //  002C  JMP		#0040
      0x1C2C130D,  //  002D  EQ	R11	R9	K13
      0x782E0001,  //  002E  JMPF	R11	#0031
      0x980E1C0A,  //  002F  SETIDX	R3	K14	R10
      0x7002000E,  //  0030  JMP		#0040
      0x1C2C130F,  //  0031  EQ	R11	R9	K15
      0x782E0001,  //  0032  JMPF	R11	#0035
      0x980E200A,  //  0033  SETIDX	R3	K16	R10
      0x7002000A,  //  0034  JMP		#0040
      0x1C2C1311,  //  0035  EQ	R11	R9	K17
      0x782E0002,  //  0036  JMPF	R11	#003A
      0x202C1513,  //  0037  NE	R11	R10	K19
      0x980E240B,  //  0038  SETIDX	R3	K18	R11
      0x70020005,  //  0039  JMP		#0040
      0x1C2C1314,  //  003A  EQ	R11	R9	K20
      0x782E0003,  //  003B  JMPF	R11	#0040
      0x8C2C0116,  //  003C  GETMET	R11	R0	K22
      0x5C341400,  //  003D  MOVE	R13	R10
      0x7C2C0400,  //  003E  CALL	R11	2
      0x980E2A0B,  //  003F  SETIDX	R3	K21	R11
      0x7001FFD4,  //  0040  JMP		#0016
      0x58140017,  //  0041  LDCONST	R5	K23
      0xAC140200,  //  0042  CATCH	R5	1	0
      0xB0080000,  //  0043  RAISE	2	R0	R0
      0x8C140718,  //  0044  GETMET	R5	R3	K24
      0x581C000C,  //  0045  LDCONST	R7	K12
      0x7C140400,  //  0046  CALL	R5	2
      0x74160000,  //  0047  JMPT	R5	#0049
      0x980E190B,  //  0048  SETIDX	R3	K12	K11
      0x8C140718,  //  0049  GETMET	R5	R3	K24
      0x581C0012,  //  004A  LDCONST	R7	K18
      0x7C140400,  //  004B  CALL	R5	2
      0x74160001,  //  004C  JMPT	R5	#004F
      0x50140000,  //  004D  LDBOOL	R5	0	0
      0x980E2405,  //  004E  SETIDX	R3	K18	R5
      0x8C140718,  //  004F  GETMET	R5	R3	K24
      0x581C000A,  //  0050  LDCONST	R7	K10
      0x7C140400,  //  0051  CALL	R5	2
      0x74160005,  //  0052  JMPT	R5	#0059
      0x8C140718,  //  0053  GETMET	R5	R3	K24
      0x581C0004,  //  0054  LDCONST	R7	K4
      0x7C140400,  //  0055  CALL	R5	2
      0x78160001,  //  0056  JMPF	R5	#0059
      0x94140704,  //  0057  GETIDX	R5	R3	K4
      0x980E1405,  //  0058  SETIDX	R3	K10	R5
      0x80040600,  //  0059  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_plugins_hints_js
********************************************************************/
be_local_closure(class_Matter_UI_show_plugins_hints_js,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    3,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(content_send),
    /* K2   */  be_nested_str_weak(_X3Cscript_X20type_X3D_X27text_X2Fjavascript_X27_X3E),
    /* K3   */  be_nested_str_weak(generate_schema_js),
    /* K4   */  be_nested_str_weak(generate_config_js),
    /* K5   */  be_nested_str_weak(generate_display_names_js),
    /* K6   */  be_nested_str_weak(_X3C_X2Fscript_X3E),
    /* K7   */  be_nested_str_weak(_ADD_ENDPOINT_JS),
    }),
    be_str_weak(show_plugins_hints_js),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x58140002,  //  0002  LDCONST	R5	K2
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C0C0103,  //  0004  GETMET	R3	R0	K3
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C0104,  //  0006  GETMET	R3	R0	K4
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0105,  //  0008  GETMET	R3	R0	K5
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x8C0C0501,  //  000B  GETMET	R3	R2	K1
      0x58140006,  //  000C  LDCONST	R5	K6
      0x7C0C0400,  //  000D  CALL	R3	2
      0x8C0C0501,  //  000E  GETMET	R3	R2	K1
      0x88140107,  //  000F  GETMBR	R5	R0	K7
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_UI
********************************************************************/
be_local_class(Matter_UI,
    2,
    NULL,
    be_nested_map(37,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(matter_enabled, 26), be_const_var(1) },
        { be_const_key_weak(show_plugins_hints_js, 14), be_const_closure(class_Matter_UI_show_plugins_hints_js_closure) },
        { be_const_key_weak(parse_param_def, 11), be_const_closure(class_Matter_UI_parse_param_def_closure) },
        { be_const_key_weak(_CLASSES_TYPES_STD, -1), be_nested_str_weak(_X7Crelay_X7Clight0_X7Clight1_X7Clight2_X7Clight3_X7Cshutter_X7Cshutter_X2Btilt_X7Cgensw_btn_X7Ctemperature_X7Cpressure_X7Cilluminance_X7Chumidity_X7Coccupancy_X7Conoff_X7Ccontact_X7Cflow_X7Crain_X7Cwaterleak_X7Cairquality) },
        { be_const_key_weak(show_bridge_status, 25), be_const_closure(class_Matter_UI_show_bridge_status_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_UI_init_closure) },
        { be_const_key_weak(extract_arg_name, 35), be_const_closure(class_Matter_UI_extract_arg_name_closure) },
        { be_const_key_weak(_CLASSES_TYPES_VIRTUAL, -1), be_nested_str_weak(_X2Dvirtual_X7Cv_relay_X7Cv_light0_X7Cv_light1_X7Cv_light2_X7Cv_light3_X7Cv_fan_X7Cv_temp_X7Cv_pressure_X7Cv_illuminance_X7Cv_humidity_X7Cv_occupancy_X7Cv_contact_X7Cv_flow_X7Cv_rain_X7Cv_waterleak_X7Cv_airquality) },
        { be_const_key_weak(show_commissioning_info, -1), be_const_closure(class_Matter_UI_show_commissioning_info_closure) },
        { be_const_key_weak(parse_options, 13), be_const_closure(class_Matter_UI_parse_options_closure) },
        { be_const_key_weak(show_enable, 2), be_const_closure(class_Matter_UI_show_enable_closure) },
        { be_const_key_weak(web_add_config_button, 3), be_const_closure(class_Matter_UI_web_add_config_button_closure) },
        { be_const_key_weak(equal_map, -1), be_const_static_closure(class_Matter_UI_equal_map_closure) },
        { be_const_key_weak(show_qrcode, 24), be_const_closure(class_Matter_UI_show_qrcode_closure) },
        { be_const_key_weak(plugin_name, -1), be_const_closure(class_Matter_UI_plugin_name_closure) },
        { be_const_key_weak(_CLASSES_TYPES2, -1), be_nested_str_weak(_X7Chttp_relay_X7Chttp_light0_X7Chttp_light1_X7Chttp_light2_X7Chttp_light3_X7Chttp_temperature_X7Chttp_pressure_X7Chttp_illuminance_X7Chttp_humidity_X7Chttp_occupancy_X7Chttp_contact_X7Chttp_flow_X7Chttp_rain_X7Chttp_waterleak_X7Chttp_airquality) },
        { be_const_key_weak(page_part_mgr, 29), be_const_closure(class_Matter_UI_page_part_mgr_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(generate_schema_js, -1), be_const_closure(class_Matter_UI_generate_schema_js_closure) },
        { be_const_key_weak(page_part_ctl, -1), be_const_closure(class_Matter_UI_page_part_ctl_closure) },
        { be_const_key_weak(web_add_handler, -1), be_const_closure(class_Matter_UI_web_add_handler_closure) },
        { be_const_key_weak(generate_config_from_status, -1), be_const_closure(class_Matter_UI_generate_config_from_status_closure) },
        { be_const_key_weak(_ADD_ENDPOINT_JS, -1), be_nested_str_long(_X3Cscript_X20type_X3D_X27text_X2Fjavascript_X27_X3Efunction_X20parseSchema_X28s_X29_X7Bvar_X20r_X3D_X7Bt_X3A_X27t_X27_X2Cr_X3A0_X7D_X3Bvar_X20parts_X3Ds_X2Esplit_X28_X27_X7C_X27_X29_X3Bif_X28parts_X2Elength_X3E0_X26_X26parts_X5B0_X5D_X2EindexOf_X28_X27_X3A_X27_X29_X3D_X3D_X3D_X2D1_X29_X7Br_X2Ea_X3Dparts_X5B0_X5D_X3Bparts_X3Dparts_X2Eslice_X281_X29_X3B_X7Dparts_X2EforEach_X28function_X28p_X29_X7Bvar_X20i_X3Dp_X2EindexOf_X28_X27_X3A_X27_X29_X3Bif_X28i_X3C0_X29return_X3Bvar_X20k_X3Dp_X2Esubstring_X280_X2Ci_X29_X2Cv_X3Dp_X2Esubstring_X28i_X2B1_X29_X3Bif_X28k_X3D_X3D_X3D_X27l_X27_X29r_X2El_X3Dv_X3Belse_X20if_X28k_X3D_X3D_X3D_X27t_X27_X29r_X2Et_X3Dv_X3Belse_X20if_X28k_X3D_X3D_X3D_X27h_X27_X29r_X2Eh_X3Dv_X3Belse_X20if_X28k_X3D_X3D_X3D_X27d_X27_X29r_X2Ed_X3Dv_X3Belse_X20if_X28k_X3D_X3D_X3D_X27r_X27_X29r_X2Er_X3D_X28v_X21_X3D_X3D_X270_X27_X29_X3Belse_X20if_X28k_X3D_X3D_X3D_X27o_X27_X29_X7Br_X2Eo_X3D_X5B_X5D_X3Bv_X2Esplit_X28_X27_X2C_X27_X29_X2EforEach_X28function_X28opt_X29_X7Bvar_X20ov_X3Dopt_X2Esplit_X28_X27_X3A_X27_X29_X3Br_X2Eo_X2Epush_X28_X7Bv_X3Aov_X5B0_X5D_X2Cl_X3Aov_X5B1_X5D_X7C_X7Cov_X5B0_X5D_X7D_X29_X3B_X7D_X29_X3B_X7D_X7D_X29_X3Breturn_X20r_X3B_X7D_X3Bfunction_X20genFieldEp_X28prefix_X2Ckey_X2Cdef_X2Cval_X29_X7Bvar_X20fn_X3Dprefix_X2B_X27__X27_X2B_X28def_X2Ea_X7C_X7Ckey_X29_X3Bvar_X20req_X3Ddef_X2Er_X3F_X27_X20required_X27_X3A_X27_X27_X3Bvar_X20ti_X3Ddef_X2Eh_X3F_X27_X20title_X3D_X22_X27_X2Bdef_X2Eh_X2B_X27_X22_X27_X3A_X27_X27_X3Bvar_X20dv_X3D_X28val_X21_X3Dnull_X26_X26val_X21_X3D_X3D_X27_X27_X29_X3Fval_X3A_X28def_X2Ed_X7C_X7C_X27_X27_X29_X3Bif_X28def_X2Et_X3D_X3D_X3D_X27i_X27_X29return_X20_X27_X3Cinput_X20type_X3D_X22number_X22_X20id_X3D_X22_X27_X2Bfn_X2B_X27_X22_X20name_X3D_X22_X27_X2Bfn_X2B_X27_X22_X20value_X3D_X22_X27_X2Bdv_X2B_X27_X22_X20size_X3D_X221_X22_X27_X2Bti_X2Breq_X2B_X27_X3E_X27_X3Bif_X28def_X2Et_X3D_X3D_X3D_X27s_X27_X29_X7Bvar_X20h_X3D_X27_X3Cselect_X20id_X3D_X22_X27_X2Bfn_X2B_X27_X22_X20name_X3D_X22_X27_X2Bfn_X2B_X27_X22_X27_X2Breq_X2B_X27_X3E_X27_X3B_X28def_X2Eo_X7C_X7C_X5B_X5D_X29_X2EforEach_X28function_X28o_X29_X7Bvar_X20sel_X3D_X28String_X28o_X2Ev_X29_X3D_X3DString_X28dv_X29_X29_X3F_X27_X20selected_X27_X3A_X27_X27_X3Bh_X2B_X3D_X27_X3Coption_X20value_X3D_X22_X27_X2Bo_X2Ev_X2B_X27_X22_X27_X2Bsel_X2B_X27_X3E_X27_X2Bo_X2El_X2B_X27_X3C_X2Foption_X3E_X27_X3B_X7D_X29_X3Breturn_X20h_X2B_X27_X3C_X2Fselect_X3E_X27_X3B_X7Dif_X28def_X2Et_X3D_X3D_X3D_X27c_X27_X29_X7Bvar_X20chk_X3D_X28String_X28dv_X29_X3D_X3D_X3D_X271_X27_X7C_X7Cdv_X3D_X3D_X3D_X27true_X27_X7C_X7Cdv_X3D_X3D_X3Dtrue_X29_X3F_X27_X20checked_X27_X3A_X27_X27_X3Breturn_X20_X27_X3Cinput_X20type_X3D_X22checkbox_X22_X20id_X3D_X22_X27_X2Bfn_X2B_X27_X22_X20name_X3D_X22_X27_X2Bfn_X2B_X27_X22_X27_X2Bchk_X2B_X27_X20style_X3D_X22width_X3Aauto_X22_X3E_X27_X3B_X7Dreturn_X20_X27_X3Cinput_X20type_X3D_X22text_X22_X20id_X3D_X22_X27_X2Bfn_X2B_X27_X22_X20name_X3D_X22_X27_X2Bfn_X2B_X27_X22_X20value_X3D_X22_X27_X2Bdv_X2B_X27_X22_X20size_X3D_X221_X22_X27_X2Bti_X2Breq_X2B_X27_X3E_X27_X3B_X7D_X3Bfunction_X20typeOpts_X28sel_X29_X7Bvar_X20h_X3D_X27_X3Coption_X20value_X3D_X22_X22_X3E_X3C_X2Foption_X3E_X27_X3Bfor_X28var_X20t_X20in_X20dn_X29_X7Bif_X28t_X3D_X3D_X3D_X27_X2Dvirtual_X27_X29_X7Bh_X2B_X3D_X27_X3Coption_X20disabled_X3E_X2D_X2D_X2D_X20Virtual_X20_X2D_X2D_X2D_X3C_X2Foption_X3E_X27_X3Bcontinue_X3B_X7Dif_X28t_X3D_X3D_X3D_X27_X2Dzigbee_X27_X29_X7Bh_X2B_X3D_X27_X3Coption_X20disabled_X3E_X2D_X2D_X2D_X20Zigbee_X20_X2D_X2D_X2D_X3C_X2Foption_X3E_X27_X3Bcontinue_X3B_X7Dh_X2B_X3D_X27_X3Coption_X20value_X3D_X22_X27_X2Bt_X2B_X27_X22_X27_X2B_X28t_X3D_X3D_X3Dsel_X3F_X27_X20selected_X27_X3A_X27_X27_X29_X2B_X27_X3E_X27_X2Bdn_X5Bt_X5D_X2B_X27_X3C_X2Foption_X3E_X27_X3B_X7Dreturn_X20h_X3B_X7D_X3Bfunction_X20epParamRows_X28ep_X2Ctyp_X2Cconf_X29_X7Bvar_X20schema_X3Dps_X3Fps_X5Btyp_X5D_X3Anull_X3Bif_X28_X21schema_X7C_X7CObject_X2Ekeys_X28schema_X29_X2Elength_X3D_X3D_X3D0_X29return_X20_X27_X27_X3Bvar_X20h_X3D_X27_X27_X3Bfor_X28var_X20key_X20in_X20schema_X29_X7Bvar_X20def_X3DparseSchema_X28schema_X5Bkey_X5D_X29_X3Bvar_X20lbl_X3Ddef_X2El_X7C_X7Cdef_X2Ea_X7C_X7Ckey_X3Bvar_X20akey_X3Ddef_X2Ea_X7C_X7Ckey_X3Bvar_X20val_X3Dconf_X5Bakey_X5D_X3Bif_X28val_X3D_X3Dnull_X29val_X3D_X27_X27_X3Bh_X2B_X3D_X27_X3Ctr_X3E_X3Ctd_X3E_X3C_X2Ftd_X3E_X3Ctd_X20colspan_X3D_X222_X22_X20style_X3D_X22font_X2Dsize_X3Asmaller_X3Bpadding_X3A1px_X200_X22_X3E_X3Cspan_X20style_X3D_X22color_X3A_X23aaa_X22_X3E_X27_X2Blbl_X2B_X27_X3A_X20_X3C_X2Fspan_X3E_X27_X2BgenFieldEp_X28_X27ep_X27_X2Bep_X2Ckey_X2Cdef_X2Cval_X29_X2B_X27_X3C_X2Ftd_X3E_X3Ctd_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X27_X3B_X7Dreturn_X20h_X3B_X7D_X3Bfunction_X20genEpRows_X28ep_X2Cconf_X2CisNew_X29_X7Bvar_X20typ_X3Dconf_X2Etype_X7C_X7C_X27_X27_X3Bvar_X20nam_X3Dconf_X2Ename_X7C_X7C_X27_X27_X3Bvar_X20h_X3D_X27_X27_X3Bh_X2B_X3D_X27_X3Ctr_X20id_X3D_X22epr_X27_X2Bep_X2B_X27_X22_X3E_X3Ctd_X20style_X3D_X22font_X2Dsize_X3Asmaller_X22_X3E_X3Cb_X3E_X27_X2Bep_X2B_X27_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X22font_X2Dsize_X3Asmaller_X22_X3E_X3Cinput_X20type_X3D_X22text_X22_X20id_X3D_X22epNam_X27_X2Bep_X2B_X27_X22_X20size_X3D_X221_X22_X20value_X3D_X22_X27_X2Bnam_X2B_X27_X22_X20placeholder_X3D_X22_X28optional_X29_X22_X3E_X3C_X2Ftd_X3E_X27_X3Bif_X28isNew_X29_X7Bh_X2B_X3D_X27_X3Ctd_X20style_X3D_X22font_X2Dsize_X3Asmaller_X22_X3E_X3Cselect_X20id_X3D_X22epTyp_X27_X2Bep_X2B_X27_X22_X20onchange_X3D_X22chgType_X28_X27_X2Bep_X2B_X27_X2Cthis_X2Evalue_X29_X22_X3E_X27_X2BtypeOpts_X28typ_X29_X2B_X27_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E_X27_X3B_X7Delse_X7Bvar_X20dnam_X3Ddn_X5Btyp_X5D_X7C_X7C_X28typ_X2EindexOf_X28_X27http__X27_X29_X3D_X3D_X3D0_X3F_X27_X26_X23x1F517_X3B_X20_X27_X2B_X28dn_X5Btyp_X2Esubstring_X285_X29_X5D_X7C_X7Ctyp_X2Esubstring_X285_X29_X29_X3Atyp_X29_X3Bh_X2B_X3D_X27_X3Ctd_X20style_X3D_X22font_X2Dsize_X3Asmaller_X22_X3E_X3Cb_X3E_X27_X2Bdnam_X2B_X27_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X27_X3B_X7Dh_X2B_X3D_X27_X3Ctd_X20style_X3D_X22text_X2Dalign_X3Acenter_X22_X3E_X3Cbutton_X20type_X3D_X22button_X22_X20title_X3D_X22Delete_X22_X20style_X3D_X22background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3Bcursor_X3Apointer_X22_X20onclick_X3D_X22delEp_X28_X27_X2Bep_X2B_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X27_X3Bh_X2B_X3D_X27_X3Ctr_X20id_X3D_X22epp_X27_X2Bep_X2B_X27_X22_X3E_X3Ctd_X3E_X3C_X2Ftd_X3E_X3Ctd_X20colspan_X3D_X222_X22_X20style_X3D_X22padding_X3A0_X22_X3E_X3Cdiv_X20id_X3D_X22epPrm_X27_X2Bep_X2B_X27_X22_X20style_X3D_X22margin_X3A0_X3Bpadding_X3A0_X22_X3E_X27_X3Bvar_X20schema_X3Dps_X3Fps_X5Btyp_X5D_X3Anull_X3Bif_X28schema_X26_X26Object_X2Ekeys_X28schema_X29_X2Elength_X3E0_X29_X7Bvar_X20tbl_X3D_X27_X3Ctable_X20style_X3D_X22width_X3A100_X25_X3Bborder_X2Dspacing_X3A0_X22_X3E_X27_X3Bfor_X28var_X20key_X20in_X20schema_X29_X7Bvar_X20def_X3DparseSchema_X28schema_X5Bkey_X5D_X29_X3Bvar_X20lbl_X3Ddef_X2El_X7C_X7Cdef_X2Ea_X7C_X7Ckey_X3Bvar_X20akey_X3Ddef_X2Ea_X7C_X7Ckey_X3Bvar_X20val_X3Dconf_X5Bakey_X5D_X3Bif_X28val_X3D_X3Dnull_X29val_X3D_X27_X27_X3Btbl_X2B_X3D_X27_X3Ctr_X3E_X3Ctd_X20style_X3D_X22font_X2Dsize_X3Asmaller_X3Bcolor_X3A_X23aaa_X3Bwhite_X2Dspace_X3Anowrap_X3Bpadding_X3A0_X204px_X200_X200_X3Bwidth_X3A50_X25_X22_X3E_X27_X2Blbl_X2B_X27_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X22font_X2Dsize_X3Asmaller_X3Bpadding_X3A0_X22_X3E_X27_X2BgenFieldEp_X28_X27ep_X27_X2BString_X28ep_X29_X2Ckey_X2Cdef_X2Cval_X29_X2B_X27_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X27_X3B_X7Dtbl_X2B_X3D_X27_X3C_X2Ftable_X3E_X27_X3Bh_X2B_X3Dtbl_X3B_X7Dh_X2B_X3D_X27_X3C_X2Fdiv_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X27_X3Breturn_X20h_X3B_X7D_X3Bfunction_X20chgType_X28ep_X2Ctyp_X29_X7Bvar_X20el_X3Deb_X28_X27epPrm_X27_X2Bep_X29_X3Bif_X28_X21el_X29return_X3Bvar_X20schema_X3Dps_X3Fps_X5Btyp_X5D_X3Anull_X3Bif_X28_X21schema_X7C_X7CObject_X2Ekeys_X28schema_X29_X2Elength_X3D_X3D_X3D0_X29_X7Bel_X2EinnerHTML_X3D_X27_X27_X3Breturn_X3B_X7Dvar_X20tbl_X3D_X27_X3Ctable_X20style_X3D_X22width_X3A100_X25_X3Bborder_X2Dspacing_X3A0_X22_X3E_X27_X3Bfor_X28var_X20key_X20in_X20schema_X29_X7Bvar_X20def_X3DparseSchema_X28schema_X5Bkey_X5D_X29_X3Bvar_X20lbl_X3Ddef_X2El_X7C_X7Cdef_X2Ea_X7C_X7Ckey_X3Btbl_X2B_X3D_X27_X3Ctr_X3E_X3Ctd_X20style_X3D_X22font_X2Dsize_X3Asmaller_X3Bcolor_X3A_X23aaa_X3Bwhite_X2Dspace_X3Anowrap_X3Bpadding_X3A0_X204px_X200_X200_X3Bwidth_X3A50_X25_X22_X3E_X27_X2Blbl_X2B_X27_X3C_X2Ftd_X3E_X3Ctd_X20style_X3D_X22font_X2Dsize_X3Asmaller_X3Bpadding_X3A0_X22_X3E_X27_X2BgenFieldEp_X28_X27ep_X27_X2Bep_X2Ckey_X2Cdef_X2C_X27_X27_X29_X2B_X27_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X27_X3B_X7Dtbl_X2B_X3D_X27_X3C_X2Ftable_X3E_X27_X3Bel_X2EinnerHTML_X3Dtbl_X3B_X7D_X3Bfunction_X20delEp_X28ep_X29_X7Bif_X28_X21confirm_X28_X27Remove_X20endpoint_X20_X27_X2Bep_X2B_X27_X3F_X27_X29_X29return_X3Bdelete_X20cfg_X5BString_X28ep_X29_X5D_X3Bvar_X20r_X3Deb_X28_X27epr_X27_X2Bep_X29_X3Bif_X28r_X29r_X2Eremove_X28_X29_X3Bvar_X20p_X3Deb_X28_X27epp_X27_X2Bep_X29_X3Bif_X28p_X29p_X2Eremove_X28_X29_X3B_X7D_X3Bfunction_X20addEp_X28_X29_X7Bvar_X20ep_X3Dnextep_X3Bnextep_X3Dep_X2B1_X3Bcfg_X5BString_X28ep_X29_X5D_X3D_X7Btype_X3A_X27_X27_X7D_X3Bvar_X20tbl_X3Deb_X28_X27epTbl_X27_X29_X3Bif_X28_X21tbl_X29return_X3Bvar_X20none_X3Deb_X28_X27epNone_X27_X29_X3Bif_X28none_X29none_X2Eremove_X28_X29_X3Btbl_X2EinsertAdjacentHTML_X28_X27beforeend_X27_X2CgenEpRows_X28ep_X2C_X7Btype_X3A_X27_X27_X7D_X2Ctrue_X29_X29_X3B_X7D_X3Bfunction_X20submitConfig_X28f_X29_X7Bfor_X28var_X20k_X20in_X20cfg_X29_X7Bvar_X20ne_X3Deb_X28_X27epNam_X27_X2Bk_X29_X3Bif_X28ne_X29_X7Bvar_X20v_X3Dne_X2Evalue_X2Etrim_X28_X29_X3Bif_X28v_X29cfg_X5Bk_X5D_X2Ename_X3Dv_X3Belse_X20delete_X20cfg_X5Bk_X5D_X2Ename_X3B_X7Dvar_X20te_X3Deb_X28_X27epTyp_X27_X2Bk_X29_X3Bif_X28te_X29cfg_X5Bk_X5D_X2Etype_X3Dte_X2Evalue_X3Bvar_X20typ_X3Dcfg_X5Bk_X5D_X2Etype_X3Bif_X28_X21typ_X29continue_X3Bvar_X20schema_X3Dps_X3Fps_X5Btyp_X5D_X3Anull_X3Bif_X28schema_X29_X7Bfor_X28var_X20key_X20in_X20schema_X29_X7Bvar_X20def_X3DparseSchema_X28schema_X5Bkey_X5D_X29_X3Bvar_X20akey_X3Ddef_X2Ea_X7C_X7Ckey_X3Bvar_X20fe_X3Deb_X28_X27ep_X27_X2Bk_X2B_X27__X27_X2Bkey_X29_X3Bif_X28fe_X29_X7Bvar_X20v_X3Dfe_X2Etype_X3D_X3D_X3D_X27checkbox_X27_X3F_X28fe_X2Echecked_X3F1_X3A0_X29_X3Afe_X2Evalue_X3Bif_X28v_X21_X3D_X3D_X27_X27_X26_X26v_X21_X3D_X3Dnull_X26_X26v_X21_X3D_X3Dundefined_X29_X7Bcfg_X5Bk_X5D_X5Bakey_X5D_X3D_X28def_X2Et_X3D_X3D_X3D_X27i_X27_X29_X3FparseInt_X28v_X2C10_X29_X3Av_X3B_X7Delse_X7Bdelete_X20cfg_X5Bk_X5D_X5Bakey_X5D_X3B_X7D_X7D_X7D_X7D_X7Dfor_X28var_X20k_X20in_X20cfg_X29_X7Bif_X28_X21cfg_X5Bk_X5D_X2Etype_X29_X7Bdelete_X20cfg_X5Bk_X5D_X3B_X7D_X7Df_X2Eelements_X5B_X27config_json_X27_X5D_X2Evalue_X3DJSON_X2Estringify_X28cfg_X29_X3Breturn_X20true_X3B_X7D_X3B_X3C_X2Fscript_X3E) },
        { be_const_key_weak(plugin_option, 19), be_const_closure(class_Matter_UI_plugin_option_closure) },
        { be_const_key_weak(web_sensor, -1), be_const_closure(class_Matter_UI_web_sensor_closure) },
        { be_const_key_weak(page_part_mgr_adv, -1), be_const_closure(class_Matter_UI_page_part_mgr_adv_closure) },
        { be_const_key_weak(show_plugins_configuration, 5), be_const_closure(class_Matter_UI_show_plugins_configuration_closure) },
        { be_const_key_weak(show_passcode_form, -1), be_const_closure(class_Matter_UI_show_passcode_form_closure) },
        { be_const_key_weak(generate_config_js, -1), be_const_closure(class_Matter_UI_generate_config_js_closure) },
        { be_const_key_weak(show_fabric_info, -1), be_const_closure(class_Matter_UI_show_fabric_info_closure) },
        { be_const_key_weak(generate_display_names_js, -1), be_const_closure(class_Matter_UI_generate_display_names_js_closure) },
        { be_const_key_weak(collect_plugin_schemas, 8), be_const_closure(class_Matter_UI_collect_plugin_schemas_closure) },
        { be_const_key_weak(reconcile_plugins, -1), be_const_closure(class_Matter_UI_reconcile_plugins_closure) },
        { be_const_key_weak(show_remote_autoconf, 4), be_const_closure(class_Matter_UI_show_remote_autoconf_closure) },
        { be_const_key_weak(web_get_arg, -1), be_const_closure(class_Matter_UI_web_get_arg_closure) },
        { be_const_key_weak(handle_config_json, -1), be_const_closure(class_Matter_UI_handle_config_json_closure) },
        { be_const_key_weak(page_part_mgr_add, 1), be_const_closure(class_Matter_UI_page_part_mgr_add_closure) },
    })),
    be_str_weak(Matter_UI)
);
/********************************************************************/
/* End of solidification */
