/* Solidification of Matter_UI.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_UI;

/********************************************************************
** Solidified function: show_fabric_info
********************************************************************/
be_local_closure(Matter_UI_show_fabric_info,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BFabrics_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(_X3Cp_X3EExisting_X20fabrics_X3A_X3C_X2Fp_X3E),
    /* K5   */  be_nested_str_weak(device),
    /* K6   */  be_nested_str_weak(sessions),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3ENone_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K9   */  be_nested_str_weak(fabrics),
    /* K10  */  be_nested_str_weak(persistables),
    /* K11  */  be_nested_str_weak(_X3Chr_X3E),
    /* K12  */  be_nested_str_weak(format),
    /* K13  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3B_X23_X25i_X20_X25s_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K14  */  be_nested_str_weak(get_fabric_index),
    /* K15  */  be_nested_str_weak(_X26lt_X3BNo_X20label_X26gt_X3B),
    /* K16  */  be_nested_str_weak(get_fabric_id),
    /* K17  */  be_nested_str_weak(copy),
    /* K18  */  be_nested_str_weak(reverse),
    /* K19  */  be_nested_str_weak(get_device_id),
    /* K20  */  be_nested_str_weak(Fabric_X3A_X20_X25s_X3Cbr_X3E),
    /* K21  */  be_nested_str_weak(tohex),
    /* K22  */  be_nested_str_weak(Device_X3A_X20_X25s_X3Cbr_X3E_X26nbsp_X3B),
    /* K23  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20),
    /* K24  */  be_nested_str_weak(onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20cause_X20a_X20restart_X2E_X22_X29_X3B_X27_X3E),
    /* K25  */  be_nested_str_weak(_X3Cinput_X20name_X3D_X27del_fabric_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X25s_X27_X3E),
    /* K26  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27del_X27_X20class_X3D_X27button_X20bgrn_X27_X3EDelete_X20Fabric_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E),
    /* K27  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K28  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(show_fabric_info),
    &be_const_str_solidified,
    ( &(const binstruction[97]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100502,  //  0002  GETMET	R4	R2	K2
      0x58180003,  //  0003  LDCONST	R6	K3
      0x7C100400,  //  0004  CALL	R4	2
      0x8C100502,  //  0005  GETMET	R4	R2	K2
      0x58180004,  //  0006  LDCONST	R6	K4
      0x7C100400,  //  0007  CALL	R4	2
      0x6010000C,  //  0008  GETGBL	R4	G12
      0x88140105,  //  0009  GETMBR	R5	R0	K5
      0x88140B06,  //  000A  GETMBR	R5	R5	K6
      0x88140B06,  //  000B  GETMBR	R5	R5	K6
      0x7C100200,  //  000C  CALL	R4	1
      0x1C100907,  //  000D  EQ	R4	R4	K7
      0x78120003,  //  000E  JMPF	R4	#0013
      0x8C100502,  //  000F  GETMET	R4	R2	K2
      0x58180008,  //  0010  LDCONST	R6	K8
      0x7C100400,  //  0011  CALL	R4	2
      0x70020049,  //  0012  JMP		#005D
      0x50100200,  //  0013  LDBOOL	R4	1	0
      0x60140010,  //  0014  GETGBL	R5	G16
      0x88180105,  //  0015  GETMBR	R6	R0	K5
      0x88180D06,  //  0016  GETMBR	R6	R6	K6
      0x88180D09,  //  0017  GETMBR	R6	R6	K9
      0x8C180D0A,  //  0018  GETMET	R6	R6	K10
      0x7C180200,  //  0019  CALL	R6	1
      0x7C140200,  //  001A  CALL	R5	1
      0xA802003D,  //  001B  EXBLK	0	#005A
      0x5C180A00,  //  001C  MOVE	R6	R5
      0x7C180000,  //  001D  CALL	R6	0
      0x5C1C0800,  //  001E  MOVE	R7	R4
      0x741E0002,  //  001F  JMPT	R7	#0023
      0x8C1C0502,  //  0020  GETMET	R7	R2	K2
      0x5824000B,  //  0021  LDCONST	R9	K11
      0x7C1C0400,  //  0022  CALL	R7	2
      0x50100000,  //  0023  LDBOOL	R4	0	0
      0x8C1C0502,  //  0024  GETMET	R7	R2	K2
      0x8C24070C,  //  0025  GETMET	R9	R3	K12
      0x582C000D,  //  0026  LDCONST	R11	K13
      0x8C300D0E,  //  0027  GETMET	R12	R6	K14
      0x7C300200,  //  0028  CALL	R12	1
      0x5834000F,  //  0029  LDCONST	R13	K15
      0x7C240800,  //  002A  CALL	R9	4
      0x7C1C0400,  //  002B  CALL	R7	2
      0x8C1C0D10,  //  002C  GETMET	R7	R6	K16
      0x7C1C0200,  //  002D  CALL	R7	1
      0x8C1C0F11,  //  002E  GETMET	R7	R7	K17
      0x7C1C0200,  //  002F  CALL	R7	1
      0x8C1C0F12,  //  0030  GETMET	R7	R7	K18
      0x7C1C0200,  //  0031  CALL	R7	1
      0x8C200D13,  //  0032  GETMET	R8	R6	K19
      0x7C200200,  //  0033  CALL	R8	1
      0x8C201111,  //  0034  GETMET	R8	R8	K17
      0x7C200200,  //  0035  CALL	R8	1
      0x8C201112,  //  0036  GETMET	R8	R8	K18
      0x7C200200,  //  0037  CALL	R8	1
      0x8C240502,  //  0038  GETMET	R9	R2	K2
      0x8C2C070C,  //  0039  GETMET	R11	R3	K12
      0x58340014,  //  003A  LDCONST	R13	K20
      0x8C380F15,  //  003B  GETMET	R14	R7	K21
      0x7C380200,  //  003C  CALL	R14	1
      0x7C2C0600,  //  003D  CALL	R11	3
      0x7C240400,  //  003E  CALL	R9	2
      0x8C240502,  //  003F  GETMET	R9	R2	K2
      0x8C2C070C,  //  0040  GETMET	R11	R3	K12
      0x58340016,  //  0041  LDCONST	R13	K22
      0x8C381115,  //  0042  GETMET	R14	R8	K21
      0x7C380200,  //  0043  CALL	R14	1
      0x7C2C0600,  //  0044  CALL	R11	3
      0x7C240400,  //  0045  CALL	R9	2
      0x8C240502,  //  0046  GETMET	R9	R2	K2
      0x582C0017,  //  0047  LDCONST	R11	K23
      0x7C240400,  //  0048  CALL	R9	2
      0x8C240502,  //  0049  GETMET	R9	R2	K2
      0x582C0018,  //  004A  LDCONST	R11	K24
      0x7C240400,  //  004B  CALL	R9	2
      0x8C240502,  //  004C  GETMET	R9	R2	K2
      0x8C2C070C,  //  004D  GETMET	R11	R3	K12
      0x58340019,  //  004E  LDCONST	R13	K25
      0x8C380F15,  //  004F  GETMET	R14	R7	K21
      0x7C380200,  //  0050  CALL	R14	1
      0x7C2C0600,  //  0051  CALL	R11	3
      0x7C240400,  //  0052  CALL	R9	2
      0x8C240502,  //  0053  GETMET	R9	R2	K2
      0x582C001A,  //  0054  LDCONST	R11	K26
      0x7C240400,  //  0055  CALL	R9	2
      0x8C240502,  //  0056  GETMET	R9	R2	K2
      0x582C001B,  //  0057  LDCONST	R11	K27
      0x7C240400,  //  0058  CALL	R9	2
      0x7001FFC1,  //  0059  JMP		#001C
      0x5814001C,  //  005A  LDCONST	R5	K28
      0xAC140200,  //  005B  CATCH	R5	1	0
      0xB0080000,  //  005C  RAISE	2	R0	R0
      0x8C100502,  //  005D  GETMET	R4	R2	K2
      0x5818001B,  //  005E  LDCONST	R6	K27
      0x7C100400,  //  005F  CALL	R4	2
      0x80000000,  //  0060  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_sensor
********************************************************************/
be_local_closure(Matter_UI_web_sensor,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(get_option),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(MATTER_OPTION),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(is_root_commissioning_open),
    /* K8   */  be_nested_str_weak(show_commissioning_info),
    /* K9   */  be_nested_str_weak(sessions),
    /* K10  */  be_nested_str_weak(count_active_fabrics),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(content_send),
    /* K13  */  be_nested_str_weak(format),
    /* K14  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27text_X2Dalign_X3Aright_X3Bfont_X2Dsize_X3A11px_X3Bcolor_X3A_X23aaa_X3B_X27_X3E_X25s_X3C_X2Fdiv_X3E),
    /* K15  */  be_nested_str_weak(No_X20active_X20associaition),
    /* K16  */  be_const_int(1),
    /* K17  */  be_nested_str_weak(_X20active_X20association),
    /* K18  */  be_nested_str_weak(s),
    /* K19  */  be_nested_str_weak(),
    /* K20  */  be_nested_str_weak(_X3Cbutton_X20onclick_X3D_X27la_X28_X22_X26mtc_X25i_X3D1_X22_X29_X3B_X27_X3E),
    /* K21  */  be_nested_str_weak(commissioning_open),
    /* K22  */  be_nested_str_weak(_LOGO),
    /* K23  */  be_nested_str_weak(_X20Open_X20Commissioning_X3C_X2Fbutton_X3E),
    /* K24  */  be_nested_str_weak(_X20Close_X20Commissioning_X3C_X2Fbutton_X3E),
    }),
    be_str_weak(web_sensor),
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0xB8160800,  //  0004  GETNGBL	R5	K4
      0x88140B05,  //  0005  GETMBR	R5	R5	K5
      0x7C0C0400,  //  0006  CALL	R3	2
      0x780E003E,  //  0007  JMPF	R3	#0047
      0x88100106,  //  0008  GETMBR	R4	R0	K6
      0x8C100907,  //  0009  GETMET	R4	R4	K7
      0x7C100200,  //  000A  CALL	R4	1
      0x78120001,  //  000B  JMPF	R4	#000E
      0x8C100108,  //  000C  GETMET	R4	R0	K8
      0x7C100200,  //  000D  CALL	R4	1
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x88100909,  //  000F  GETMBR	R4	R4	K9
      0x8C10090A,  //  0010  GETMET	R4	R4	K10
      0x7C100200,  //  0011  CALL	R4	1
      0x1C14090B,  //  0012  EQ	R5	R4	K11
      0x78160006,  //  0013  JMPF	R5	#001B
      0x8C14030C,  //  0014  GETMET	R5	R1	K12
      0x8C1C050D,  //  0015  GETMET	R7	R2	K13
      0x5824000E,  //  0016  LDCONST	R9	K14
      0x5828000F,  //  0017  LDCONST	R10	K15
      0x7C1C0600,  //  0018  CALL	R7	3
      0x7C140400,  //  0019  CALL	R5	2
      0x7002000E,  //  001A  JMP		#002A
      0x24140910,  //  001B  GT	R5	R4	K16
      0x8C18030C,  //  001C  GETMET	R6	R1	K12
      0x8C20050D,  //  001D  GETMET	R8	R2	K13
      0x5828000E,  //  001E  LDCONST	R10	K14
      0x602C0008,  //  001F  GETGBL	R11	G8
      0x5C300800,  //  0020  MOVE	R12	R4
      0x7C2C0200,  //  0021  CALL	R11	1
      0x002C1711,  //  0022  ADD	R11	R11	K17
      0x78160001,  //  0023  JMPF	R5	#0026
      0x58300012,  //  0024  LDCONST	R12	K18
      0x70020000,  //  0025  JMP		#0027
      0x58300013,  //  0026  LDCONST	R12	K19
      0x002C160C,  //  0027  ADD	R11	R11	R12
      0x7C200600,  //  0028  CALL	R8	3
      0x7C180400,  //  0029  CALL	R6	2
      0x8C14030C,  //  002A  GETMET	R5	R1	K12
      0x8C1C050D,  //  002B  GETMET	R7	R2	K13
      0x58240014,  //  002C  LDCONST	R9	K20
      0x88280106,  //  002D  GETMBR	R10	R0	K6
      0x88281515,  //  002E  GETMBR	R10	R10	K21
      0x4C2C0000,  //  002F  LDNIL	R11
      0x1C28140B,  //  0030  EQ	R10	R10	R11
      0x782A0001,  //  0031  JMPF	R10	#0034
      0x58280010,  //  0032  LDCONST	R10	K16
      0x70020000,  //  0033  JMP		#0035
      0x5828000B,  //  0034  LDCONST	R10	K11
      0x7C1C0600,  //  0035  CALL	R7	3
      0x7C140400,  //  0036  CALL	R5	2
      0x8C14030C,  //  0037  GETMET	R5	R1	K12
      0xB81E0800,  //  0038  GETNGBL	R7	K4
      0x881C0F16,  //  0039  GETMBR	R7	R7	K22
      0x7C140400,  //  003A  CALL	R5	2
      0x88140106,  //  003B  GETMBR	R5	R0	K6
      0x88140B15,  //  003C  GETMBR	R5	R5	K21
      0x4C180000,  //  003D  LDNIL	R6
      0x1C140A06,  //  003E  EQ	R5	R5	R6
      0x78160003,  //  003F  JMPF	R5	#0044
      0x8C14030C,  //  0040  GETMET	R5	R1	K12
      0x581C0017,  //  0041  LDCONST	R7	K23
      0x7C140400,  //  0042  CALL	R5	2
      0x70020002,  //  0043  JMP		#0047
      0x8C14030C,  //  0044  GETMET	R5	R1	K12
      0x581C0018,  //  0045  LDCONST	R7	K24
      0x7C140400,  //  0046  CALL	R5	2
      0x80000000,  //  0047  RET	0
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
** Solidified function: page_part_mgr
********************************************************************/
be_local_closure(Matter_UI_page_part_mgr,   /* name */
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
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(check_privileged_access),
    /* K3   */  be_nested_str_weak(content_start),
    /* K4   */  be_nested_str_weak(Matter),
    /* K5   */  be_nested_str_weak(content_send_style),
    /* K6   */  be_nested_str_weak(show_enable),
    /* K7   */  be_nested_str_weak(show_passcode_form),
    /* K8   */  be_nested_str_weak(show_fabric_info),
    /* K9   */  be_nested_str_weak(content_button),
    /* K10  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K11  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x740E0001,  //  0004  JMPT	R3	#0007
      0x4C0C0000,  //  0005  LDNIL	R3
      0x80040600,  //  0006  RET	1	R3
      0x8C0C0303,  //  0007  GETMET	R3	R1	K3
      0x58140004,  //  0008  LDCONST	R5	K4
      0x7C0C0400,  //  0009  CALL	R3	2
      0x8C0C0305,  //  000A  GETMET	R3	R1	K5
      0x7C0C0200,  //  000B  CALL	R3	1
      0x8C0C0106,  //  000C  GETMET	R3	R0	K6
      0x7C0C0200,  //  000D  CALL	R3	1
      0x780E0003,  //  000E  JMPF	R3	#0013
      0x8C0C0107,  //  000F  GETMET	R3	R0	K7
      0x7C0C0200,  //  0010  CALL	R3	1
      0x8C0C0108,  //  0011  GETMET	R3	R0	K8
      0x7C0C0200,  //  0012  CALL	R3	1
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
    ( &(const struct bproto*[ 2]) {
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
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(on),
    /* K2   */  be_nested_str_weak(_X2Fmatterc),
    /* K3   */  be_nested_str_weak(HTTP_GET),
    /* K4   */  be_nested_str_weak(HTTP_POST),
    }),
    be_str_weak(web_add_handler),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
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
      0xA0000000,  //  000B  CLOSE	R0
      0x80000000,  //  000C  RET	0
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
** Solidified function: show_passcode_form
********************************************************************/
be_local_closure(Matter_UI_show_passcode_form,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Passcode_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20_X3E),
    /* K5   */  be_nested_str_weak(_X3Cp_X3EPasscode_X3A_X3C_X2Fp_X3E),
    /* K6   */  be_nested_str_weak(format),
    /* K7   */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X271_X27_X20max_X3D_X2799999998_X27_X20name_X3D_X27passcode_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K8   */  be_nested_str_weak(device),
    /* K9   */  be_nested_str_weak(root_passcode),
    /* K10  */  be_nested_str_weak(_X3Cp_X3EDistinguish_X20id_X3A_X3C_X2Fp_X3E),
    /* K11  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X270_X27_X20max_X3D_X274095_X27_X20name_X3D_X27discriminator_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K12  */  be_nested_str_weak(root_discriminator),
    /* K13  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20type_X3D_X27checkbox_X27_X20name_X3D_X27ipv4_X27_X25s_X3EIPv4_X20only_X3C_X2Fp_X3E),
    /* K14  */  be_nested_str_weak(ipv4only),
    /* K15  */  be_nested_str_weak(_X20checked),
    /* K16  */  be_nested_str_weak(),
    /* K17  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27passcode_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E),
    /* K18  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_passcode_form),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C0C0302,  //  0005  GETMET	R3	R1	K2
      0x58140004,  //  0006  LDCONST	R5	K4
      0x7C0C0400,  //  0007  CALL	R3	2
      0x8C0C0302,  //  0008  GETMET	R3	R1	K2
      0x58140005,  //  0009  LDCONST	R5	K5
      0x7C0C0400,  //  000A  CALL	R3	2
      0x8C0C0302,  //  000B  GETMET	R3	R1	K2
      0x8C140506,  //  000C  GETMET	R5	R2	K6
      0x581C0007,  //  000D  LDCONST	R7	K7
      0x88200108,  //  000E  GETMBR	R8	R0	K8
      0x88201109,  //  000F  GETMBR	R8	R8	K9
      0x7C140600,  //  0010  CALL	R5	3
      0x7C0C0400,  //  0011  CALL	R3	2
      0x8C0C0302,  //  0012  GETMET	R3	R1	K2
      0x5814000A,  //  0013  LDCONST	R5	K10
      0x7C0C0400,  //  0014  CALL	R3	2
      0x8C0C0302,  //  0015  GETMET	R3	R1	K2
      0x8C140506,  //  0016  GETMET	R5	R2	K6
      0x581C000B,  //  0017  LDCONST	R7	K11
      0x88200108,  //  0018  GETMBR	R8	R0	K8
      0x8820110C,  //  0019  GETMBR	R8	R8	K12
      0x7C140600,  //  001A  CALL	R5	3
      0x7C0C0400,  //  001B  CALL	R3	2
      0x8C0C0302,  //  001C  GETMET	R3	R1	K2
      0x8C140506,  //  001D  GETMET	R5	R2	K6
      0x581C000D,  //  001E  LDCONST	R7	K13
      0x88200108,  //  001F  GETMBR	R8	R0	K8
      0x8820110E,  //  0020  GETMBR	R8	R8	K14
      0x78220001,  //  0021  JMPF	R8	#0024
      0x5820000F,  //  0022  LDCONST	R8	K15
      0x70020000,  //  0023  JMP		#0025
      0x58200010,  //  0024  LDCONST	R8	K16
      0x7C140600,  //  0025  CALL	R5	3
      0x7C0C0400,  //  0026  CALL	R3	2
      0x8C0C0302,  //  0027  GETMET	R3	R1	K2
      0x58140011,  //  0028  LDCONST	R5	K17
      0x7C0C0400,  //  0029  CALL	R3	2
      0x8C0C0302,  //  002A  GETMET	R3	R1	K2
      0x58140012,  //  002B  LDCONST	R5	K18
      0x7C0C0400,  //  002C  CALL	R3	2
      0x80000000,  //  002D  RET	0
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
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(get_option),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(MATTER_OPTION),
    /* K6   */  be_nested_str_weak(content_send),
    /* K7   */  be_nested_str_weak(format),
    /* K8   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20_X25s_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K9   */  be_nested_str_weak(Enabled),
    /* K10  */  be_nested_str_weak(Disabled),
    /* K11  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A320px_X3B_X27_X3EMatter_X20support_X20is_X20experimental_X2E_X3C_X2Fp_X3E),
    /* K12  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20cause_X20a_X20restart_X2E_X22_X29_X3B_X27_X3E),
    /* K13  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27_X25s_X27_X20class_X3D_X27button_X20bgrn_X27_X3E),
    /* K14  */  be_nested_str_weak(disable),
    /* K15  */  be_nested_str_weak(enable),
    /* K16  */  be_nested_str_weak(Disable),
    /* K17  */  be_nested_str_weak(Enable),
    /* K18  */  be_nested_str_weak(_X20Matter_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E),
    /* K19  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_enable),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0xB81A0800,  //  0004  GETNGBL	R6	K4
      0x88180D05,  //  0005  GETMBR	R6	R6	K5
      0x7C100400,  //  0006  CALL	R4	2
      0x8C140506,  //  0007  GETMET	R5	R2	K6
      0x8C1C0707,  //  0008  GETMET	R7	R3	K7
      0x58240008,  //  0009  LDCONST	R9	K8
      0x78120001,  //  000A  JMPF	R4	#000D
      0x58280009,  //  000B  LDCONST	R10	K9
      0x70020000,  //  000C  JMP		#000E
      0x5828000A,  //  000D  LDCONST	R10	K10
      0x7C1C0600,  //  000E  CALL	R7	3
      0x7C140400,  //  000F  CALL	R5	2
      0x8C140506,  //  0010  GETMET	R5	R2	K6
      0x581C000B,  //  0011  LDCONST	R7	K11
      0x7C140400,  //  0012  CALL	R5	2
      0x8C140506,  //  0013  GETMET	R5	R2	K6
      0x581C000C,  //  0014  LDCONST	R7	K12
      0x7C140400,  //  0015  CALL	R5	2
      0x8C140506,  //  0016  GETMET	R5	R2	K6
      0x8C1C0707,  //  0017  GETMET	R7	R3	K7
      0x5824000D,  //  0018  LDCONST	R9	K13
      0x78120001,  //  0019  JMPF	R4	#001C
      0x5828000E,  //  001A  LDCONST	R10	K14
      0x70020000,  //  001B  JMP		#001D
      0x5828000F,  //  001C  LDCONST	R10	K15
      0x7C1C0600,  //  001D  CALL	R7	3
      0x7C140400,  //  001E  CALL	R5	2
      0x8C140506,  //  001F  GETMET	R5	R2	K6
      0x78120001,  //  0020  JMPF	R4	#0023
      0x581C0010,  //  0021  LDCONST	R7	K16
      0x70020000,  //  0022  JMP		#0024
      0x581C0011,  //  0023  LDCONST	R7	K17
      0x7C140400,  //  0024  CALL	R5	2
      0x8C140506,  //  0025  GETMET	R5	R2	K6
      0x581C0012,  //  0026  LDCONST	R7	K18
      0x7C140400,  //  0027  CALL	R5	2
      0x8C140506,  //  0028  GETMET	R5	R2	K6
      0x581C0013,  //  0029  LDCONST	R7	K19
      0x7C140400,  //  002A  CALL	R5	2
      0x80040800,  //  002B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_commissioning_info
********************************************************************/
be_local_closure(Matter_UI_show_commissioning_info,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(device),
    /* K3   */  be_nested_str_weak(commissioning_open),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(millis),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(content_send),
    /* K8   */  be_nested_str_weak(format),
    /* K9   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3B_X5B_X20Commissioning_X20open_X20for_X20_X25i_X20min_X20_X5D_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K10  */  be_nested_str_weak(compute_manual_pairing_code),
    /* K11  */  be_nested_str_weak(_X3Cp_X3EManual_X20pairing_X20code_X3A_X3Cbr_X3E_X3Cb_X3E_X25s_X2D_X25s_X2D_X25s_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Chr_X3E),
    /* K12  */  be_const_int(3),
    /* K13  */  be_const_int(2147483647),
    /* K14  */  be_nested_str_weak(compute_qrcode_content),
    /* K15  */  be_nested_str_weak(show_qrcode),
    /* K16  */  be_nested_str_weak(_X3Cp_X3E_X20_X25s_X3C_X2Fp_X3E),
    /* K17  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_commissioning_info),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x880C0703,  //  0003  GETMBR	R3	R3	K3
      0xB8120800,  //  0004  GETNGBL	R4	K4
      0x8C100905,  //  0005  GETMET	R4	R4	K5
      0x7C100200,  //  0006  CALL	R4	1
      0x040C0604,  //  0007  SUB	R3	R3	R4
      0x541203E7,  //  0008  LDINT	R4	1000
      0x0C0C0604,  //  0009  DIV	R3	R3	R4
      0x14100706,  //  000A  LT	R4	R3	K6
      0x78120000,  //  000B  JMPF	R4	#000D
      0x580C0006,  //  000C  LDCONST	R3	K6
      0x5412001D,  //  000D  LDINT	R4	30
      0x00100604,  //  000E  ADD	R4	R3	R4
      0x5416003B,  //  000F  LDINT	R5	60
      0x0C100805,  //  0010  DIV	R4	R4	R5
      0x8C140307,  //  0011  GETMET	R5	R1	K7
      0x8C1C0508,  //  0012  GETMET	R7	R2	K8
      0x58240009,  //  0013  LDCONST	R9	K9
      0x5C280800,  //  0014  MOVE	R10	R4
      0x7C1C0600,  //  0015  CALL	R7	3
      0x7C140400,  //  0016  CALL	R5	2
      0x88140102,  //  0017  GETMBR	R5	R0	K2
      0x8C140B0A,  //  0018  GETMET	R5	R5	K10
      0x7C140200,  //  0019  CALL	R5	1
      0x8C180307,  //  001A  GETMET	R6	R1	K7
      0x8C200508,  //  001B  GETMET	R8	R2	K8
      0x5828000B,  //  001C  LDCONST	R10	K11
      0x402E0D0C,  //  001D  CONNECT	R11	K6	K12
      0x942C0A0B,  //  001E  GETIDX	R11	R5	R11
      0x54320003,  //  001F  LDINT	R12	4
      0x54360005,  //  0020  LDINT	R13	6
      0x4030180D,  //  0021  CONNECT	R12	R12	R13
      0x94300A0C,  //  0022  GETIDX	R12	R5	R12
      0x54360006,  //  0023  LDINT	R13	7
      0x40341B0D,  //  0024  CONNECT	R13	R13	K13
      0x94340A0D,  //  0025  GETIDX	R13	R5	R13
      0x7C200A00,  //  0026  CALL	R8	5
      0x7C180400,  //  0027  CALL	R6	2
      0x88180102,  //  0028  GETMBR	R6	R0	K2
      0x8C180D0E,  //  0029  GETMET	R6	R6	K14
      0x7C180200,  //  002A  CALL	R6	1
      0x8C1C010F,  //  002B  GETMET	R7	R0	K15
      0x5C240C00,  //  002C  MOVE	R9	R6
      0x7C1C0400,  //  002D  CALL	R7	2
      0x8C1C0307,  //  002E  GETMET	R7	R1	K7
      0x8C240508,  //  002F  GETMET	R9	R2	K8
      0x582C0010,  //  0030  LDCONST	R11	K16
      0x5C300C00,  //  0031  MOVE	R12	R6
      0x7C240600,  //  0032  CALL	R9	3
      0x7C1C0400,  //  0033  CALL	R7	2
      0x8C1C0307,  //  0034  GETMET	R7	R1	K7
      0x58240011,  //  0035  LDCONST	R9	K17
      0x7C1C0400,  //  0036  CALL	R7	2
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_ctl
********************************************************************/
be_local_closure(Matter_UI_page_part_ctl,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[51]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(check_privileged_access),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(partition_core),
    /* K4   */  be_nested_str_weak(persist),
    /* K5   */  be_nested_str_weak(Partition),
    /* K6   */  be_nested_str_weak(has_arg),
    /* K7   */  be_nested_str_weak(passcode),
    /* K8   */  be_nested_str_weak(discriminator),
    /* K9   */  be_nested_str_weak(device),
    /* K10  */  be_nested_str_weak(root_passcode),
    /* K11  */  be_nested_str_weak(arg),
    /* K12  */  be_nested_str_weak(root_discriminator),
    /* K13  */  be_nested_str_weak(ipv4only),
    /* K14  */  be_nested_str_weak(ipv4),
    /* K15  */  be_nested_str_weak(on),
    /* K16  */  be_nested_str_weak(save_param),
    /* K17  */  be_nested_str_weak(redirect),
    /* K18  */  be_nested_str_weak(_X2F_X3Frst_X3D),
    /* K19  */  be_nested_str_weak(enable),
    /* K20  */  be_nested_str_weak(tasmota),
    /* K21  */  be_nested_str_weak(cmd),
    /* K22  */  be_nested_str_weak(SetOption),
    /* K23  */  be_nested_str_weak(matter),
    /* K24  */  be_nested_str_weak(MATTER_OPTION),
    /* K25  */  be_nested_str_weak(_X201),
    /* K26  */  be_nested_str_weak(disable),
    /* K27  */  be_nested_str_weak(_X200),
    /* K28  */  be_nested_str_weak(del_fabric),
    /* K29  */  be_const_int(0),
    /* K30  */  be_nested_str_weak(sessions),
    /* K31  */  be_nested_str_weak(fabrics),
    /* K32  */  be_nested_str_weak(get_fabric_id),
    /* K33  */  be_nested_str_weak(copy),
    /* K34  */  be_nested_str_weak(reverse),
    /* K35  */  be_nested_str_weak(tohex),
    /* K36  */  be_nested_str_weak(remove),
    /* K37  */  be_const_int(1),
    /* K38  */  be_nested_str_weak(save_fabrics),
    /* K39  */  be_nested_str_weak(log),
    /* K40  */  be_nested_str_weak(format),
    /* K41  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K42  */  be_const_int(2),
    /* K43  */  be_nested_str_weak(content_start),
    /* K44  */  be_nested_str_weak(Parameter_X20error),
    /* K45  */  be_nested_str_weak(content_send_style),
    /* K46  */  be_nested_str_weak(content_send),
    /* K47  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
    /* K48  */  be_nested_str_weak(content_button),
    /* K49  */  be_nested_str_weak(BUTTON_MANAGEMENT),
    /* K50  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_ctl),
    &be_const_str_solidified,
    ( &(const binstruction[165]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0001,  //  0003  JMPT	R2	#0006
      0x4C080000,  //  0004  LDNIL	R2
      0x80040400,  //  0005  RET	1	R2
      0xA40A0400,  //  0006  IMPORT	R2	K2
      0xA40E0600,  //  0007  IMPORT	R3	K3
      0xA4120800,  //  0008  IMPORT	R4	K4
      0x8C140705,  //  0009  GETMET	R5	R3	K5
      0x7C140200,  //  000A  CALL	R5	1
      0xA8020079,  //  000B  EXBLK	0	#0086
      0x8C180306,  //  000C  GETMET	R6	R1	K6
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C180400,  //  000E  CALL	R6	2
      0x741A0003,  //  000F  JMPT	R6	#0014
      0x8C180306,  //  0010  GETMET	R6	R1	K6
      0x58200008,  //  0011  LDCONST	R8	K8
      0x7C180400,  //  0012  CALL	R6	2
      0x781A0022,  //  0013  JMPF	R6	#0037
      0x8C180306,  //  0014  GETMET	R6	R1	K6
      0x58200007,  //  0015  LDCONST	R8	K7
      0x7C180400,  //  0016  CALL	R6	2
      0x781A0006,  //  0017  JMPF	R6	#001F
      0x88180109,  //  0018  GETMBR	R6	R0	K9
      0x601C0009,  //  0019  GETGBL	R7	G9
      0x8C20030B,  //  001A  GETMET	R8	R1	K11
      0x58280007,  //  001B  LDCONST	R10	K7
      0x7C200400,  //  001C  CALL	R8	2
      0x7C1C0200,  //  001D  CALL	R7	1
      0x901A1407,  //  001E  SETMBR	R6	K10	R7
      0x8C180306,  //  001F  GETMET	R6	R1	K6
      0x58200008,  //  0020  LDCONST	R8	K8
      0x7C180400,  //  0021  CALL	R6	2
      0x781A0006,  //  0022  JMPF	R6	#002A
      0x88180109,  //  0023  GETMBR	R6	R0	K9
      0x601C0009,  //  0024  GETGBL	R7	G9
      0x8C20030B,  //  0025  GETMET	R8	R1	K11
      0x58280008,  //  0026  LDCONST	R10	K8
      0x7C200400,  //  0027  CALL	R8	2
      0x7C1C0200,  //  0028  CALL	R7	1
      0x901A1807,  //  0029  SETMBR	R6	K12	R7
      0x88180109,  //  002A  GETMBR	R6	R0	K9
      0x8C1C030B,  //  002B  GETMET	R7	R1	K11
      0x5824000E,  //  002C  LDCONST	R9	K14
      0x7C1C0400,  //  002D  CALL	R7	2
      0x1C1C0F0F,  //  002E  EQ	R7	R7	K15
      0x901A1A07,  //  002F  SETMBR	R6	K13	R7
      0x88180109,  //  0030  GETMBR	R6	R0	K9
      0x8C180D10,  //  0031  GETMET	R6	R6	K16
      0x7C180200,  //  0032  CALL	R6	1
      0x8C180311,  //  0033  GETMET	R6	R1	K17
      0x58200012,  //  0034  LDCONST	R8	K18
      0x7C180400,  //  0035  CALL	R6	2
      0x7002004C,  //  0036  JMP		#0084
      0x8C180306,  //  0037  GETMET	R6	R1	K6
      0x58200013,  //  0038  LDCONST	R8	K19
      0x7C180400,  //  0039  CALL	R6	2
      0x781A000C,  //  003A  JMPF	R6	#0048
      0xB81A2800,  //  003B  GETNGBL	R6	K20
      0x8C180D15,  //  003C  GETMET	R6	R6	K21
      0x60200008,  //  003D  GETGBL	R8	G8
      0xB8262E00,  //  003E  GETNGBL	R9	K23
      0x88241318,  //  003F  GETMBR	R9	R9	K24
      0x7C200200,  //  0040  CALL	R8	1
      0x00222C08,  //  0041  ADD	R8	K22	R8
      0x00201119,  //  0042  ADD	R8	R8	K25
      0x7C180400,  //  0043  CALL	R6	2
      0x8C180311,  //  0044  GETMET	R6	R1	K17
      0x58200012,  //  0045  LDCONST	R8	K18
      0x7C180400,  //  0046  CALL	R6	2
      0x7002003B,  //  0047  JMP		#0084
      0x8C180306,  //  0048  GETMET	R6	R1	K6
      0x5820001A,  //  0049  LDCONST	R8	K26
      0x7C180400,  //  004A  CALL	R6	2
      0x781A000C,  //  004B  JMPF	R6	#0059
      0xB81A2800,  //  004C  GETNGBL	R6	K20
      0x8C180D15,  //  004D  GETMET	R6	R6	K21
      0x60200008,  //  004E  GETGBL	R8	G8
      0xB8262E00,  //  004F  GETNGBL	R9	K23
      0x88241318,  //  0050  GETMBR	R9	R9	K24
      0x7C200200,  //  0051  CALL	R8	1
      0x00222C08,  //  0052  ADD	R8	K22	R8
      0x0020111B,  //  0053  ADD	R8	R8	K27
      0x7C180400,  //  0054  CALL	R6	2
      0x8C180311,  //  0055  GETMET	R6	R1	K17
      0x58200012,  //  0056  LDCONST	R8	K18
      0x7C180400,  //  0057  CALL	R6	2
      0x7002002A,  //  0058  JMP		#0084
      0x8C180306,  //  0059  GETMET	R6	R1	K6
      0x5820001C,  //  005A  LDCONST	R8	K28
      0x7C180400,  //  005B  CALL	R6	2
      0x781A0026,  //  005C  JMPF	R6	#0084
      0x8C18030B,  //  005D  GETMET	R6	R1	K11
      0x5820001C,  //  005E  LDCONST	R8	K28
      0x7C180400,  //  005F  CALL	R6	2
      0x581C001D,  //  0060  LDCONST	R7	K29
      0x88200109,  //  0061  GETMBR	R8	R0	K9
      0x8820111E,  //  0062  GETMBR	R8	R8	K30
      0x8820111F,  //  0063  GETMBR	R8	R8	K31
      0x50240000,  //  0064  LDBOOL	R9	0	0
      0x6028000C,  //  0065  GETGBL	R10	G12
      0x5C2C1000,  //  0066  MOVE	R11	R8
      0x7C280200,  //  0067  CALL	R10	1
      0x14280E0A,  //  0068  LT	R10	R7	R10
      0x782A0011,  //  0069  JMPF	R10	#007C
      0x94281007,  //  006A  GETIDX	R10	R8	R7
      0x8C281520,  //  006B  GETMET	R10	R10	K32
      0x7C280200,  //  006C  CALL	R10	1
      0x8C281521,  //  006D  GETMET	R10	R10	K33
      0x7C280200,  //  006E  CALL	R10	1
      0x8C281522,  //  006F  GETMET	R10	R10	K34
      0x7C280200,  //  0070  CALL	R10	1
      0x8C281523,  //  0071  GETMET	R10	R10	K35
      0x7C280200,  //  0072  CALL	R10	1
      0x1C2C1406,  //  0073  EQ	R11	R10	R6
      0x782E0004,  //  0074  JMPF	R11	#007A
      0x8C2C1124,  //  0075  GETMET	R11	R8	K36
      0x5C340E00,  //  0076  MOVE	R13	R7
      0x7C2C0400,  //  0077  CALL	R11	2
      0x50240200,  //  0078  LDBOOL	R9	1	0
      0x70020000,  //  0079  JMP		#007B
      0x001C0F25,  //  007A  ADD	R7	R7	K37
      0x7001FFE8,  //  007B  JMP		#0065
      0x78260003,  //  007C  JMPF	R9	#0081
      0x88280109,  //  007D  GETMBR	R10	R0	K9
      0x8828151E,  //  007E  GETMBR	R10	R10	K30
      0x8C281526,  //  007F  GETMET	R10	R10	K38
      0x7C280200,  //  0080  CALL	R10	1
      0x8C280311,  //  0081  GETMET	R10	R1	K17
      0x58300012,  //  0082  LDCONST	R12	K18
      0x7C280400,  //  0083  CALL	R10	2
      0xA8040001,  //  0084  EXBLK	1	1
      0x7002001D,  //  0085  JMP		#00A4
      0xAC180002,  //  0086  CATCH	R6	0	2
      0x7002001A,  //  0087  JMP		#00A3
      0xB8222800,  //  0088  GETNGBL	R8	K20
      0x8C201127,  //  0089  GETMET	R8	R8	K39
      0x8C280528,  //  008A  GETMET	R10	R2	K40
      0x58300029,  //  008B  LDCONST	R12	K41
      0x5C340C00,  //  008C  MOVE	R13	R6
      0x5C380E00,  //  008D  MOVE	R14	R7
      0x7C280800,  //  008E  CALL	R10	4
      0x582C002A,  //  008F  LDCONST	R11	K42
      0x7C200600,  //  0090  CALL	R8	3
      0x8C20032B,  //  0091  GETMET	R8	R1	K43
      0x5828002C,  //  0092  LDCONST	R10	K44
      0x7C200400,  //  0093  CALL	R8	2
      0x8C20032D,  //  0094  GETMET	R8	R1	K45
      0x7C200200,  //  0095  CALL	R8	1
      0x8C20032E,  //  0096  GETMET	R8	R1	K46
      0x8C280528,  //  0097  GETMET	R10	R2	K40
      0x5830002F,  //  0098  LDCONST	R12	K47
      0x5C340C00,  //  0099  MOVE	R13	R6
      0x5C380E00,  //  009A  MOVE	R14	R7
      0x7C280800,  //  009B  CALL	R10	4
      0x7C200400,  //  009C  CALL	R8	2
      0x8C200330,  //  009D  GETMET	R8	R1	K48
      0x88280331,  //  009E  GETMBR	R10	R1	K49
      0x7C200400,  //  009F  CALL	R8	2
      0x8C200332,  //  00A0  GETMET	R8	R1	K50
      0x7C200200,  //  00A1  CALL	R8	1
      0x70020000,  //  00A2  JMP		#00A4
      0xB0080000,  //  00A3  RAISE	2	R0	R0
      0x80000000,  //  00A4  RET	0
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
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(show_fabric_info, -1), be_const_closure(Matter_UI_show_fabric_info_closure) },
        { be_const_key_weak(page_part_ctl, -1), be_const_closure(Matter_UI_page_part_ctl_closure) },
        { be_const_key_weak(web_get_arg, -1), be_const_closure(Matter_UI_web_get_arg_closure) },
        { be_const_key_weak(page_part_mgr, 8), be_const_closure(Matter_UI_page_part_mgr_closure) },
        { be_const_key_weak(web_add_config_button, -1), be_const_closure(Matter_UI_web_add_config_button_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_UI_init_closure) },
        { be_const_key_weak(web_sensor, 10), be_const_closure(Matter_UI_web_sensor_closure) },
        { be_const_key_weak(web_add_handler, -1), be_const_closure(Matter_UI_web_add_handler_closure) },
        { be_const_key_weak(show_enable, 12), be_const_closure(Matter_UI_show_enable_closure) },
        { be_const_key_weak(show_passcode_form, 1), be_const_closure(Matter_UI_show_passcode_form_closure) },
        { be_const_key_weak(show_qrcode, 5), be_const_closure(Matter_UI_show_qrcode_closure) },
        { be_const_key_weak(show_commissioning_info, -1), be_const_closure(Matter_UI_show_commissioning_info_closure) },
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
