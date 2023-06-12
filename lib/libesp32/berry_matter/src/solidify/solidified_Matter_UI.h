/* Solidification of Matter_UI.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_UI;

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
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Advanced_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20cause_X20a_X20restart_X2E_X22_X29_X3B_X27_X3E_X3Cp_X3EPasscode_X3A_X3C_X2Fp_X3E),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X271_X27_X20max_X3D_X2799999998_X27_X20name_X3D_X27passcode_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K7   */  be_nested_str_weak(device),
    /* K8   */  be_nested_str_weak(root_passcode),
    /* K9   */  be_nested_str_weak(_X3Cp_X3EDistinguish_X20id_X3A_X3C_X2Fp_X3E),
    /* K10  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X270_X27_X20max_X3D_X274095_X27_X20name_X3D_X27discriminator_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K11  */  be_nested_str_weak(root_discriminator),
    /* K12  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20type_X3D_X27checkbox_X27_X20name_X3D_X27ipv4_X27_X25s_X3EIPv4_X20only_X3C_X2Fp_X3E),
    /* K13  */  be_nested_str_weak(ipv4only),
    /* K14  */  be_nested_str_weak(_X20checked),
    /* K15  */  be_nested_str_weak(),
    /* K16  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27passcode_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_passcode_form),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C0C0302,  //  0005  GETMET	R3	R1	K2
      0x58140004,  //  0006  LDCONST	R5	K4
      0x7C0C0400,  //  0007  CALL	R3	2
      0x8C0C0302,  //  0008  GETMET	R3	R1	K2
      0x8C140505,  //  0009  GETMET	R5	R2	K5
      0x581C0006,  //  000A  LDCONST	R7	K6
      0x88200107,  //  000B  GETMBR	R8	R0	K7
      0x88201108,  //  000C  GETMBR	R8	R8	K8
      0x7C140600,  //  000D  CALL	R5	3
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C0C0302,  //  000F  GETMET	R3	R1	K2
      0x58140009,  //  0010  LDCONST	R5	K9
      0x7C0C0400,  //  0011  CALL	R3	2
      0x8C0C0302,  //  0012  GETMET	R3	R1	K2
      0x8C140505,  //  0013  GETMET	R5	R2	K5
      0x581C000A,  //  0014  LDCONST	R7	K10
      0x88200107,  //  0015  GETMBR	R8	R0	K7
      0x8820110B,  //  0016  GETMBR	R8	R8	K11
      0x7C140600,  //  0017  CALL	R5	3
      0x7C0C0400,  //  0018  CALL	R3	2
      0x8C0C0302,  //  0019  GETMET	R3	R1	K2
      0x8C140505,  //  001A  GETMET	R5	R2	K5
      0x581C000C,  //  001B  LDCONST	R7	K12
      0x88200107,  //  001C  GETMBR	R8	R0	K7
      0x8820110D,  //  001D  GETMBR	R8	R8	K13
      0x78220001,  //  001E  JMPF	R8	#0021
      0x5820000E,  //  001F  LDCONST	R8	K14
      0x70020000,  //  0020  JMP		#0022
      0x5820000F,  //  0021  LDCONST	R8	K15
      0x7C140600,  //  0022  CALL	R5	3
      0x7C0C0400,  //  0023  CALL	R3	2
      0x8C0C0302,  //  0024  GETMET	R3	R1	K2
      0x58140010,  //  0025  LDCONST	R5	K16
      0x7C0C0400,  //  0026  CALL	R3	2
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_mgr_adv
********************************************************************/
be_local_closure(Matter_UI_page_part_mgr_adv,   /* name */
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
    /* K4   */  be_nested_str_weak(Matter_X20Advanced_X20Configuration),
    /* K5   */  be_nested_str_weak(content_send_style),
    /* K6   */  be_nested_str_weak(matter_enabled),
    /* K7   */  be_nested_str_weak(show_passcode_form),
    /* K8   */  be_nested_str_weak(show_fabric_info),
    /* K9   */  be_nested_str_weak(content_button),
    /* K10  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K11  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr_adv),
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
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(plugins),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(Plugin_Bridge_HTTP),
    /* K7   */  be_nested_str_weak(http_remote),
    /* K8   */  be_nested_str_weak(addr),
    /* K9   */  be_nested_str_weak(contains),
    /* K10  */  be_nested_str_weak(push),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(content_send),
    /* K13  */  be_nested_str_weak(_X3Chr_X3E),
    /* K14  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K15  */  be_nested_str_weak(_STYLESHEET),
    /* K16  */  be_nested_str_weak(keys),
    /* K17  */  be_nested_str_weak(format),
    /* K18  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27ztdm_X20htrm_X27_X3E_X3Ctd_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Ftd_X3E),
    /* K19  */  be_nested_str_weak(html_escape),
    /* K20  */  be_nested_str_weak(web_last_seen),
    /* K21  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27htrm_X27_X3E_X3Ctd_X20colspan_X3D_X272_X27_X3E),
    /* K22  */  be_nested_str_weak(web_values),
    /* K23  */  be_nested_str_weak(_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K24  */  be_nested_str_weak(stop_iteration),
    /* K25  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Chr_X3E),
    }),
    be_str_weak(show_bridge_status),
    &be_const_str_solidified,
    ( &(const binstruction[102]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x58100002,  //  0003  LDCONST	R4	K2
      0x6014000C,  //  0004  GETGBL	R5	G12
      0x88180103,  //  0005  GETMBR	R6	R0	K3
      0x88180D04,  //  0006  GETMBR	R6	R6	K4
      0x7C140200,  //  0007  CALL	R5	1
      0x14140805,  //  0008  LT	R5	R4	R5
      0x7816001D,  //  0009  JMPF	R5	#0028
      0x88140103,  //  000A  GETMBR	R5	R0	K3
      0x88140B04,  //  000B  GETMBR	R5	R5	K4
      0x94140A04,  //  000C  GETIDX	R5	R5	R4
      0x6018000F,  //  000D  GETGBL	R6	G15
      0x5C1C0A00,  //  000E  MOVE	R7	R5
      0xB8220A00,  //  000F  GETNGBL	R8	K5
      0x88201106,  //  0010  GETMBR	R8	R8	K6
      0x7C180400,  //  0011  CALL	R6	2
      0x781A0012,  //  0012  JMPF	R6	#0026
      0x4C180000,  //  0013  LDNIL	R6
      0x1C180606,  //  0014  EQ	R6	R3	R6
      0x781A0002,  //  0015  JMPF	R6	#0019
      0x60180013,  //  0016  GETGBL	R6	G19
      0x7C180000,  //  0017  CALL	R6	0
      0x5C0C0C00,  //  0018  MOVE	R3	R6
      0x88180B07,  //  0019  GETMBR	R6	R5	K7
      0x88180D08,  //  001A  GETMBR	R6	R6	K8
      0x8C1C0709,  //  001B  GETMET	R7	R3	K9
      0x5C240C00,  //  001C  MOVE	R9	R6
      0x7C1C0400,  //  001D  CALL	R7	2
      0x741E0002,  //  001E  JMPT	R7	#0022
      0x601C0012,  //  001F  GETGBL	R7	G18
      0x7C1C0000,  //  0020  CALL	R7	0
      0x980C0C07,  //  0021  SETIDX	R3	R6	R7
      0x941C0606,  //  0022  GETIDX	R7	R3	R6
      0x8C1C0F0A,  //  0023  GETMET	R7	R7	K10
      0x5C240A00,  //  0024  MOVE	R9	R5
      0x7C1C0400,  //  0025  CALL	R7	2
      0x0010090B,  //  0026  ADD	R4	R4	K11
      0x7001FFDB,  //  0027  JMP		#0004
      0x4C140000,  //  0028  LDNIL	R5
      0x1C140605,  //  0029  EQ	R5	R3	R5
      0x78160000,  //  002A  JMPF	R5	#002C
      0x80000A00,  //  002B  RET	0
      0x8C14030C,  //  002C  GETMET	R5	R1	K12
      0x581C000D,  //  002D  LDCONST	R7	K13
      0x7C140400,  //  002E  CALL	R5	2
      0x8C14030C,  //  002F  GETMET	R5	R1	K12
      0x581C000E,  //  0030  LDCONST	R7	K14
      0x7C140400,  //  0031  CALL	R5	2
      0x8C14030C,  //  0032  GETMET	R5	R1	K12
      0xB81E0A00,  //  0033  GETNGBL	R7	K5
      0x881C0F0F,  //  0034  GETMBR	R7	R7	K15
      0x7C140400,  //  0035  CALL	R5	2
      0x60140010,  //  0036  GETGBL	R5	G16
      0x8C180710,  //  0037  GETMET	R6	R3	K16
      0x7C180200,  //  0038  CALL	R6	1
      0x7C140200,  //  0039  CALL	R5	1
      0xA8020023,  //  003A  EXBLK	0	#005F
      0x5C180A00,  //  003B  MOVE	R6	R5
      0x7C180000,  //  003C  CALL	R6	0
      0x8C1C030C,  //  003D  GETMET	R7	R1	K12
      0x8C240511,  //  003E  GETMET	R9	R2	K17
      0x582C0012,  //  003F  LDCONST	R11	K18
      0x8C300313,  //  0040  GETMET	R12	R1	K19
      0x5C380C00,  //  0041  MOVE	R14	R6
      0x7C300400,  //  0042  CALL	R12	2
      0x7C240600,  //  0043  CALL	R9	3
      0x7C1C0400,  //  0044  CALL	R7	2
      0x941C0606,  //  0045  GETIDX	R7	R3	R6
      0x941C0F02,  //  0046  GETIDX	R7	R7	K2
      0x881C0F07,  //  0047  GETMBR	R7	R7	K7
      0x8C20030C,  //  0048  GETMET	R8	R1	K12
      0x8C280F14,  //  0049  GETMET	R10	R7	K20
      0x7C280200,  //  004A  CALL	R10	1
      0x7C200400,  //  004B  CALL	R8	2
      0x60200010,  //  004C  GETGBL	R8	G16
      0x94240606,  //  004D  GETIDX	R9	R3	R6
      0x7C200200,  //  004E  CALL	R8	1
      0xA802000A,  //  004F  EXBLK	0	#005B
      0x5C241000,  //  0050  MOVE	R9	R8
      0x7C240000,  //  0051  CALL	R9	0
      0x8C28030C,  //  0052  GETMET	R10	R1	K12
      0x58300015,  //  0053  LDCONST	R12	K21
      0x7C280400,  //  0054  CALL	R10	2
      0x8C281316,  //  0055  GETMET	R10	R9	K22
      0x7C280200,  //  0056  CALL	R10	1
      0x8C28030C,  //  0057  GETMET	R10	R1	K12
      0x58300017,  //  0058  LDCONST	R12	K23
      0x7C280400,  //  0059  CALL	R10	2
      0x7001FFF4,  //  005A  JMP		#0050
      0x58200018,  //  005B  LDCONST	R8	K24
      0xAC200200,  //  005C  CATCH	R8	1	0
      0xB0080000,  //  005D  RAISE	2	R0	R0
      0x7001FFDB,  //  005E  JMP		#003B
      0x58140018,  //  005F  LDCONST	R5	K24
      0xAC140200,  //  0060  CATCH	R5	1	0
      0xB0080000,  //  0061  RAISE	2	R0	R0
      0x8C14030C,  //  0062  GETMET	R5	R1	K12
      0x581C0019,  //  0063  LDCONST	R7	K25
      0x7C140400,  //  0064  CALL	R5	2
      0x80000000,  //  0065  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: plugin_option
********************************************************************/
be_local_closure(Matter_UI_plugin_option,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    3,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
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
    /* K13  */  be_nested_str_weak(format),
    /* K14  */  be_nested_str_weak(_X3Coption_X20value_X3D_X27_X25s_X27_X25s_X3E_X25s_X3C_X2Foption_X3E),
    /* K15  */  be_nested_str_weak(_X20selected),
    /* K16  */  be_const_int(1),
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
      0x8C2C090D,  //  002B  GETMET	R11	R4	K13
      0x5834000E,  //  002C  LDCONST	R13	K14
      0x5C380E00,  //  002D  MOVE	R14	R7
      0x1C3C0E01,  //  002E  EQ	R15	R7	R1
      0x783E0001,  //  002F  JMPF	R15	#0032
      0x583C000F,  //  0030  LDCONST	R15	K15
      0x70020000,  //  0031  JMP		#0033
      0x583C0006,  //  0032  LDCONST	R15	K6
      0x5C401000,  //  0033  MOVE	R16	R8
      0x7C2C0A00,  //  0034  CALL	R11	5
      0x7C240400,  //  0035  CALL	R9	2
      0x00180D10,  //  0036  ADD	R6	R6	K16
      0x7001FFDB,  //  0037  JMP		#0014
      0x80000000,  //  0038  RET	0
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
** Solidified function: page_part_ctl
********************************************************************/
be_local_closure(Matter_UI_page_part_ctl,   /* name */
  be_nested_proto(
    25,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[97]) {     /* constants */
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
    /* K10  */  be_nested_str_weak(format),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20Arg_X25i_X20_X27_X25s_X27_X20_X3D_X20_X27_X25s_X27),
    /* K12  */  be_nested_str_weak(arg_name),
    /* K13  */  be_nested_str_weak(arg),
    /* K14  */  be_nested_str_weak(stop_iteration),
    /* K15  */  be_nested_str_weak(has_arg),
    /* K16  */  be_nested_str_weak(passcode),
    /* K17  */  be_nested_str_weak(discriminator),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20_X2Fmatterc_X20received_X20_X27_X25s_X27_X20command),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(device),
    /* K21  */  be_nested_str_weak(root_passcode),
    /* K22  */  be_nested_str_weak(root_discriminator),
    /* K23  */  be_nested_str_weak(ipv4only),
    /* K24  */  be_nested_str_weak(ipv4),
    /* K25  */  be_nested_str_weak(on),
    /* K26  */  be_nested_str_weak(save_param),
    /* K27  */  be_nested_str_weak(redirect),
    /* K28  */  be_nested_str_weak(_X2F_X3Frst_X3D),
    /* K29  */  be_nested_str_weak(save),
    /* K30  */  be_nested_str_weak(menable),
    /* K31  */  be_nested_str_weak(comm),
    /* K32  */  be_nested_str_weak(matter_enabled),
    /* K33  */  be_nested_str_weak(enable),
    /* K34  */  be_nested_str_weak(cmd),
    /* K35  */  be_nested_str_weak(SetOption),
    /* K36  */  be_nested_str_weak(matter),
    /* K37  */  be_nested_str_weak(MATTER_OPTION),
    /* K38  */  be_nested_str_weak(_X201),
    /* K39  */  be_nested_str_weak(disable),
    /* K40  */  be_nested_str_weak(_X200),
    /* K41  */  be_nested_str_weak(commissioning_open),
    /* K42  */  be_nested_str_weak(start_root_basic_commissioning),
    /* K43  */  be_nested_str_weak(stop_basic_commissioning),
    /* K44  */  be_nested_str_weak(_X2F),
    /* K45  */  be_nested_str_weak(del_fabric),
    /* K46  */  be_nested_str_weak(sessions),
    /* K47  */  be_nested_str_weak(fabrics),
    /* K48  */  be_nested_str_weak(get_fabric_index),
    /* K49  */  be_nested_str_weak(remove_fabric),
    /* K50  */  be_nested_str_weak(_X2Fmatterc_X3F),
    /* K51  */  be_nested_str_weak(auto),
    /* K52  */  be_nested_str_weak(plugins_persist),
    /* K53  */  be_nested_str_weak(config),
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
    /* K69  */  be_nested_str_weak(MTR_X3A_X20config_X20_X3D_X20_X25s),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20config_X20error_X20_X3D_X20_X25s),
    /* K71  */  be_nested_str_weak(_X2Fcn_X3F),
    /* K72  */  be_nested_str_weak(addep),
    /* K73  */  be_nested_str_weak(pi),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20add_X20endpoint_X20typ_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K75  */  be_nested_str_weak(bridge_add_endpoint),
    /* K76  */  be_nested_str_weak(addrem),
    /* K77  */  be_nested_str_weak(url),
    /* K78  */  be_nested_str_weak(value_error),
    /* K79  */  be_nested_str_weak(url_X20shouldn_X27t_X20be_X20null),
    /* K80  */  be_nested_str_weak(equal_map),
    /* K81  */  be_nested_str_weak(MTR_X3A_X20remote_X20add_X20url_X3D_X27_X25s_X27_X20type_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K82  */  be_nested_str_weak(del),
    /* K83  */  be_nested_str_weak(MTR_X3A_X20deleting_X20endpoint_X20_X25i),
    /* K84  */  be_const_int(2),
    /* K85  */  be_nested_str_weak(bridge_remove_endpoint),
    /* K86  */  be_nested_str_weak(content_start),
    /* K87  */  be_nested_str_weak(Parameter_X20error),
    /* K88  */  be_nested_str_weak(content_send_style),
    /* K89  */  be_nested_str_weak(content_send),
    /* K90  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EError_X3A_X3C_X2Fb_X3E_X25s_X3C_X2Fp_X3E),
    /* K91  */  be_nested_str_weak(html_escape),
    /* K92  */  be_nested_str_weak(content_button),
    /* K93  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K94  */  be_nested_str_weak(content_stop),
    /* K95  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K96  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
    }),
    be_str_weak(page_part_ctl),
    &be_const_str_solidified,
    ( &(const binstruction[628]) {  /* code */
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
      0xA8020249,  //  000A  EXBLK	0	#0255
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
      0x8C28050A,  //  0016  GETMET	R10	R2	K10
      0x5830000B,  //  0017  LDCONST	R12	K11
      0x5C340E00,  //  0018  MOVE	R13	R7
      0x8C38030C,  //  0019  GETMET	R14	R1	K12
      0x5C400E00,  //  001A  MOVE	R16	R7
      0x7C380400,  //  001B  CALL	R14	2
      0x8C3C030D,  //  001C  GETMET	R15	R1	K13
      0x5C440E00,  //  001D  MOVE	R17	R7
      0x7C3C0400,  //  001E  CALL	R15	2
      0x7C280A00,  //  001F  CALL	R10	5
      0x7C200400,  //  0020  CALL	R8	2
      0x7001FFEF,  //  0021  JMP		#0012
      0x5818000E,  //  0022  LDCONST	R6	K14
      0xAC180200,  //  0023  CATCH	R6	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x8C18030F,  //  0025  GETMET	R6	R1	K15
      0x58200010,  //  0026  LDCONST	R8	K16
      0x7C180400,  //  0027  CALL	R6	2
      0x741A0003,  //  0028  JMPT	R6	#002D
      0x8C18030F,  //  0029  GETMET	R6	R1	K15
      0x58200011,  //  002A  LDCONST	R8	K17
      0x7C180400,  //  002B  CALL	R6	2
      0x781A002A,  //  002C  JMPF	R6	#0058
      0xB81A1000,  //  002D  GETNGBL	R6	K8
      0x8C180D09,  //  002E  GETMET	R6	R6	K9
      0x8C20050A,  //  002F  GETMET	R8	R2	K10
      0x58280012,  //  0030  LDCONST	R10	K18
      0x582C0010,  //  0031  LDCONST	R11	K16
      0x7C200600,  //  0032  CALL	R8	3
      0x58240013,  //  0033  LDCONST	R9	K19
      0x7C180600,  //  0034  CALL	R6	3
      0x8C18030F,  //  0035  GETMET	R6	R1	K15
      0x58200010,  //  0036  LDCONST	R8	K16
      0x7C180400,  //  0037  CALL	R6	2
      0x781A0006,  //  0038  JMPF	R6	#0040
      0x88180114,  //  0039  GETMBR	R6	R0	K20
      0x601C0009,  //  003A  GETGBL	R7	G9
      0x8C20030D,  //  003B  GETMET	R8	R1	K13
      0x58280010,  //  003C  LDCONST	R10	K16
      0x7C200400,  //  003D  CALL	R8	2
      0x7C1C0200,  //  003E  CALL	R7	1
      0x901A2A07,  //  003F  SETMBR	R6	K21	R7
      0x8C18030F,  //  0040  GETMET	R6	R1	K15
      0x58200011,  //  0041  LDCONST	R8	K17
      0x7C180400,  //  0042  CALL	R6	2
      0x781A0006,  //  0043  JMPF	R6	#004B
      0x88180114,  //  0044  GETMBR	R6	R0	K20
      0x601C0009,  //  0045  GETGBL	R7	G9
      0x8C20030D,  //  0046  GETMET	R8	R1	K13
      0x58280011,  //  0047  LDCONST	R10	K17
      0x7C200400,  //  0048  CALL	R8	2
      0x7C1C0200,  //  0049  CALL	R7	1
      0x901A2C07,  //  004A  SETMBR	R6	K22	R7
      0x88180114,  //  004B  GETMBR	R6	R0	K20
      0x8C1C030D,  //  004C  GETMET	R7	R1	K13
      0x58240018,  //  004D  LDCONST	R9	K24
      0x7C1C0400,  //  004E  CALL	R7	2
      0x1C1C0F19,  //  004F  EQ	R7	R7	K25
      0x901A2E07,  //  0050  SETMBR	R6	K23	R7
      0x88180114,  //  0051  GETMBR	R6	R0	K20
      0x8C180D1A,  //  0052  GETMET	R6	R6	K26
      0x7C180200,  //  0053  CALL	R6	1
      0x8C18031B,  //  0054  GETMET	R6	R1	K27
      0x5820001C,  //  0055  LDCONST	R8	K28
      0x7C180400,  //  0056  CALL	R6	2
      0x700201E7,  //  0057  JMP		#0240
      0x8C18030F,  //  0058  GETMET	R6	R1	K15
      0x5820001D,  //  0059  LDCONST	R8	K29
      0x7C180400,  //  005A  CALL	R6	2
      0x781A0047,  //  005B  JMPF	R6	#00A4
      0x8C18030F,  //  005C  GETMET	R6	R1	K15
      0x5820001E,  //  005D  LDCONST	R8	K30
      0x7C180400,  //  005E  CALL	R6	2
      0x8C1C030F,  //  005F  GETMET	R7	R1	K15
      0x5824001F,  //  0060  LDCONST	R9	K31
      0x7C1C0400,  //  0061  CALL	R7	2
      0x8C200120,  //  0062  GETMET	R8	R0	K32
      0x7C200200,  //  0063  CALL	R8	1
      0x20200C08,  //  0064  NE	R8	R6	R8
      0x78220027,  //  0065  JMPF	R8	#008E
      0x781A0011,  //  0066  JMPF	R6	#0079
      0xB8221000,  //  0067  GETNGBL	R8	K8
      0x8C201109,  //  0068  GETMET	R8	R8	K9
      0x8C28050A,  //  0069  GETMET	R10	R2	K10
      0x58300012,  //  006A  LDCONST	R12	K18
      0x58340021,  //  006B  LDCONST	R13	K33
      0x7C280600,  //  006C  CALL	R10	3
      0x582C0013,  //  006D  LDCONST	R11	K19
      0x7C200600,  //  006E  CALL	R8	3
      0xB8221000,  //  006F  GETNGBL	R8	K8
      0x8C201122,  //  0070  GETMET	R8	R8	K34
      0x60280008,  //  0071  GETGBL	R10	G8
      0xB82E4800,  //  0072  GETNGBL	R11	K36
      0x882C1725,  //  0073  GETMBR	R11	R11	K37
      0x7C280200,  //  0074  CALL	R10	1
      0x002A460A,  //  0075  ADD	R10	K35	R10
      0x00281526,  //  0076  ADD	R10	R10	K38
      0x7C200400,  //  0077  CALL	R8	2
      0x70020010,  //  0078  JMP		#008A
      0xB8221000,  //  0079  GETNGBL	R8	K8
      0x8C201109,  //  007A  GETMET	R8	R8	K9
      0x8C28050A,  //  007B  GETMET	R10	R2	K10
      0x58300012,  //  007C  LDCONST	R12	K18
      0x58340027,  //  007D  LDCONST	R13	K39
      0x7C280600,  //  007E  CALL	R10	3
      0x582C0013,  //  007F  LDCONST	R11	K19
      0x7C200600,  //  0080  CALL	R8	3
      0xB8221000,  //  0081  GETNGBL	R8	K8
      0x8C201122,  //  0082  GETMET	R8	R8	K34
      0x60280008,  //  0083  GETGBL	R10	G8
      0xB82E4800,  //  0084  GETNGBL	R11	K36
      0x882C1725,  //  0085  GETMBR	R11	R11	K37
      0x7C280200,  //  0086  CALL	R10	1
      0x002A460A,  //  0087  ADD	R10	K35	R10
      0x00281528,  //  0088  ADD	R10	R10	K40
      0x7C200400,  //  0089  CALL	R8	2
      0x8C20031B,  //  008A  GETMET	R8	R1	K27
      0x5828001C,  //  008B  LDCONST	R10	K28
      0x7C200400,  //  008C  CALL	R8	2
      0x70020014,  //  008D  JMP		#00A3
      0x88200114,  //  008E  GETMBR	R8	R0	K20
      0x88201129,  //  008F  GETMBR	R8	R8	K41
      0x4C240000,  //  0090  LDNIL	R9
      0x20201009,  //  0091  NE	R8	R8	R9
      0x20200E08,  //  0092  NE	R8	R7	R8
      0x7822000B,  //  0093  JMPF	R8	#00A0
      0x781E0003,  //  0094  JMPF	R7	#0099
      0x88200114,  //  0095  GETMBR	R8	R0	K20
      0x8C20112A,  //  0096  GETMET	R8	R8	K42
      0x7C200200,  //  0097  CALL	R8	1
      0x70020002,  //  0098  JMP		#009C
      0x88200114,  //  0099  GETMBR	R8	R0	K20
      0x8C20112B,  //  009A  GETMET	R8	R8	K43
      0x7C200200,  //  009B  CALL	R8	1
      0x8C20031B,  //  009C  GETMET	R8	R1	K27
      0x5828002C,  //  009D  LDCONST	R10	K44
      0x7C200400,  //  009E  CALL	R8	2
      0x70020002,  //  009F  JMP		#00A3
      0x8C20031B,  //  00A0  GETMET	R8	R1	K27
      0x5828002C,  //  00A1  LDCONST	R10	K44
      0x7C200400,  //  00A2  CALL	R8	2
      0x7002019B,  //  00A3  JMP		#0240
      0x8C18030F,  //  00A4  GETMET	R6	R1	K15
      0x5820002D,  //  00A5  LDCONST	R8	K45
      0x7C180400,  //  00A6  CALL	R6	2
      0x781A0026,  //  00A7  JMPF	R6	#00CF
      0xB81A1000,  //  00A8  GETNGBL	R6	K8
      0x8C180D09,  //  00A9  GETMET	R6	R6	K9
      0x8C20050A,  //  00AA  GETMET	R8	R2	K10
      0x58280012,  //  00AB  LDCONST	R10	K18
      0x582C002D,  //  00AC  LDCONST	R11	K45
      0x7C200600,  //  00AD  CALL	R8	3
      0x58240013,  //  00AE  LDCONST	R9	K19
      0x7C180600,  //  00AF  CALL	R6	3
      0x60180009,  //  00B0  GETGBL	R6	G9
      0x8C1C030D,  //  00B1  GETMET	R7	R1	K13
      0x5824002D,  //  00B2  LDCONST	R9	K45
      0x7C1C0400,  //  00B3  CALL	R7	2
      0x7C180200,  //  00B4  CALL	R6	1
      0x581C0005,  //  00B5  LDCONST	R7	K5
      0x88200114,  //  00B6  GETMBR	R8	R0	K20
      0x8820112E,  //  00B7  GETMBR	R8	R8	K46
      0x8820112F,  //  00B8  GETMBR	R8	R8	K47
      0x6024000C,  //  00B9  GETGBL	R9	G12
      0x5C281000,  //  00BA  MOVE	R10	R8
      0x7C240200,  //  00BB  CALL	R9	1
      0x14240E09,  //  00BC  LT	R9	R7	R9
      0x7826000C,  //  00BD  JMPF	R9	#00CB
      0x94241007,  //  00BE  GETIDX	R9	R8	R7
      0x8C241330,  //  00BF  GETMET	R9	R9	K48
      0x7C240200,  //  00C0  CALL	R9	1
      0x1C241206,  //  00C1  EQ	R9	R9	R6
      0x78260005,  //  00C2  JMPF	R9	#00C9
      0x88240114,  //  00C3  GETMBR	R9	R0	K20
      0x8C241331,  //  00C4  GETMET	R9	R9	K49
      0x942C1007,  //  00C5  GETIDX	R11	R8	R7
      0x7C240400,  //  00C6  CALL	R9	2
      0x70020002,  //  00C7  JMP		#00CB
      0x70020000,  //  00C8  JMP		#00CA
      0x001C0F07,  //  00C9  ADD	R7	R7	K7
      0x7001FFED,  //  00CA  JMP		#00B9
      0x8C24031B,  //  00CB  GETMET	R9	R1	K27
      0x582C0032,  //  00CC  LDCONST	R11	K50
      0x7C240400,  //  00CD  CALL	R9	2
      0x70020170,  //  00CE  JMP		#0240
      0x8C18030F,  //  00CF  GETMET	R6	R1	K15
      0x58200033,  //  00D0  LDCONST	R8	K51
      0x7C180400,  //  00D1  CALL	R6	2
      0x781A0011,  //  00D2  JMPF	R6	#00E5
      0xB81A1000,  //  00D3  GETNGBL	R6	K8
      0x8C180D09,  //  00D4  GETMET	R6	R6	K9
      0x8C20050A,  //  00D5  GETMET	R8	R2	K10
      0x58280012,  //  00D6  LDCONST	R10	K18
      0x582C0033,  //  00D7  LDCONST	R11	K51
      0x7C200600,  //  00D8  CALL	R8	3
      0x58240013,  //  00D9  LDCONST	R9	K19
      0x7C180600,  //  00DA  CALL	R6	3
      0x88180114,  //  00DB  GETMBR	R6	R0	K20
      0x501C0000,  //  00DC  LDBOOL	R7	0	0
      0x901A6807,  //  00DD  SETMBR	R6	K52	R7
      0x88180114,  //  00DE  GETMBR	R6	R0	K20
      0x8C180D1A,  //  00DF  GETMET	R6	R6	K26
      0x7C180200,  //  00E0  CALL	R6	1
      0x8C18031B,  //  00E1  GETMET	R6	R1	K27
      0x5820001C,  //  00E2  LDCONST	R8	K28
      0x7C180400,  //  00E3  CALL	R6	2
      0x7002015A,  //  00E4  JMP		#0240
      0x8C18030F,  //  00E5  GETMET	R6	R1	K15
      0x58200035,  //  00E6  LDCONST	R8	K53
      0x7C180400,  //  00E7  CALL	R6	2
      0x781A009C,  //  00E8  JMPF	R6	#0186
      0xB81A1000,  //  00E9  GETNGBL	R6	K8
      0x8C180D09,  //  00EA  GETMET	R6	R6	K9
      0x8C20050A,  //  00EB  GETMET	R8	R2	K10
      0x58280012,  //  00EC  LDCONST	R10	K18
      0x582C0035,  //  00ED  LDCONST	R11	K53
      0x7C200600,  //  00EE  CALL	R8	3
      0x58240013,  //  00EF  LDCONST	R9	K19
      0x7C180600,  //  00F0  CALL	R6	3
      0x50180000,  //  00F1  LDBOOL	R6	0	0
      0x601C0010,  //  00F2  GETGBL	R7	G16
      0x8C200306,  //  00F3  GETMET	R8	R1	K6
      0x7C200200,  //  00F4  CALL	R8	1
      0x04201107,  //  00F5  SUB	R8	R8	K7
      0x40220A08,  //  00F6  CONNECT	R8	K5	R8
      0x7C1C0200,  //  00F7  CALL	R7	1
      0xA8020066,  //  00F8  EXBLK	0	#0160
      0x5C200E00,  //  00F9  MOVE	R8	R7
      0x7C200000,  //  00FA  CALL	R8	0
      0x8C24030C,  //  00FB  GETMET	R9	R1	K12
      0x5C2C1000,  //  00FC  MOVE	R11	R8
      0x7C240400,  //  00FD  CALL	R9	2
      0x8C280536,  //  00FE  GETMET	R10	R2	K54
      0x5C301200,  //  00FF  MOVE	R12	R9
      0x5834000D,  //  0100  LDCONST	R13	K13
      0x7C280600,  //  0101  CALL	R10	3
      0x1C281505,  //  0102  EQ	R10	R10	K5
      0x782A005A,  //  0103  JMPF	R10	#015F
      0x60280009,  //  0104  GETGBL	R10	G9
      0x402E2737,  //  0105  CONNECT	R11	K19	K55
      0x942C120B,  //  0106  GETIDX	R11	R9	R11
      0x7C280200,  //  0107  CALL	R10	1
      0x8C2C030D,  //  0108  GETMET	R11	R1	K13
      0x5C341000,  //  0109  MOVE	R13	R8
      0x7C2C0400,  //  010A  CALL	R11	2
      0x88300114,  //  010B  GETMBR	R12	R0	K20
      0x88301938,  //  010C  GETMBR	R12	R12	K56
      0x8C301936,  //  010D  GETMET	R12	R12	K54
      0x60380008,  //  010E  GETGBL	R14	G8
      0x5C3C1400,  //  010F  MOVE	R15	R10
      0x7C380200,  //  0110  CALL	R14	1
      0x7C300400,  //  0111  CALL	R12	2
      0x4C340000,  //  0112  LDNIL	R13
      0x2034180D,  //  0113  NE	R13	R12	R13
      0x78360041,  //  0114  JMPF	R13	#0157
      0x88340114,  //  0115  GETMBR	R13	R0	K20
      0x88341B39,  //  0116  GETMBR	R13	R13	K57
      0x8C341B36,  //  0117  GETMET	R13	R13	K54
      0x8C3C1936,  //  0118  GETMET	R15	R12	K54
      0x5844003A,  //  0119  LDCONST	R17	K58
      0x5848003B,  //  011A  LDCONST	R18	K59
      0x7C3C0600,  //  011B  CALL	R15	3
      0x7C340400,  //  011C  CALL	R13	2
      0x4C380000,  //  011D  LDNIL	R14
      0x20381A0E,  //  011E  NE	R14	R13	R14
      0x783A0035,  //  011F  JMPF	R14	#0156
      0xB83A1000,  //  0120  GETNGBL	R14	K8
      0x8C381D09,  //  0121  GETMET	R14	R14	K9
      0x8C40050A,  //  0122  GETMET	R16	R2	K10
      0x5848003C,  //  0123  LDCONST	R18	K60
      0x5C4C1400,  //  0124  MOVE	R19	R10
      0x5C501600,  //  0125  MOVE	R20	R11
      0x7C400800,  //  0126  CALL	R16	4
      0x58440013,  //  0127  LDCONST	R17	K19
      0x7C380600,  //  0128  CALL	R14	3
      0x8C381B3D,  //  0129  GETMET	R14	R13	K61
      0x5C401A00,  //  012A  MOVE	R16	R13
      0x5C441800,  //  012B  MOVE	R17	R12
      0x7C380600,  //  012C  CALL	R14	3
      0x203C1C0B,  //  012D  NE	R15	R14	R11
      0xB8421000,  //  012E  GETNGBL	R16	K8
      0x8C402109,  //  012F  GETMET	R16	R16	K9
      0x8C48050A,  //  0130  GETMET	R18	R2	K10
      0x5850003E,  //  0131  LDCONST	R20	K62
      0x5C541400,  //  0132  MOVE	R21	R10
      0x5C581C00,  //  0133  MOVE	R22	R14
      0x5C5C1600,  //  0134  MOVE	R23	R11
      0x20601C0B,  //  0135  NE	R24	R14	R11
      0x78620001,  //  0136  JMPF	R24	#0139
      0x5860003F,  //  0137  LDCONST	R24	K63
      0x70020000,  //  0138  JMP		#013A
      0x5860003B,  //  0139  LDCONST	R24	K59
      0x7C480C00,  //  013A  CALL	R18	6
      0x584C0013,  //  013B  LDCONST	R19	K19
      0x7C400600,  //  013C  CALL	R16	3
      0x783E0017,  //  013D  JMPF	R15	#0156
      0x50180200,  //  013E  LDBOOL	R6	1	0
      0x8C401B40,  //  013F  GETMET	R16	R13	K64
      0x5C481A00,  //  0140  MOVE	R18	R13
      0x5C4C1800,  //  0141  MOVE	R19	R12
      0x5C501600,  //  0142  MOVE	R20	R11
      0x7C400800,  //  0143  CALL	R16	4
      0x88400114,  //  0144  GETMBR	R16	R0	K20
      0x8C402141,  //  0145  GETMET	R16	R16	K65
      0x5C481400,  //  0146  MOVE	R18	R10
      0x7C400400,  //  0147  CALL	R16	2
      0x7842000C,  //  0148  JMPF	R16	#0156
      0xB8461000,  //  0149  GETNGBL	R17	K8
      0x8C442309,  //  014A  GETMET	R17	R17	K9
      0x8C4C050A,  //  014B  GETMET	R19	R2	K10
      0x58540042,  //  014C  LDCONST	R21	K66
      0x5C581800,  //  014D  MOVE	R22	R12
      0x5C5C1400,  //  014E  MOVE	R23	R10
      0x5C602000,  //  014F  MOVE	R24	R16
      0x7C4C0A00,  //  0150  CALL	R19	5
      0x58500013,  //  0151  LDCONST	R20	K19
      0x7C440600,  //  0152  CALL	R17	3
      0x8C442143,  //  0153  GETMET	R17	R16	K67
      0x5C4C1800,  //  0154  MOVE	R19	R12
      0x7C440400,  //  0155  CALL	R17	2
      0x70020007,  //  0156  JMP		#015F
      0xB8361000,  //  0157  GETNGBL	R13	K8
      0x8C341B09,  //  0158  GETMET	R13	R13	K9
      0x8C3C050A,  //  0159  GETMET	R15	R2	K10
      0x58440044,  //  015A  LDCONST	R17	K68
      0x5C481400,  //  015B  MOVE	R18	R10
      0x7C3C0600,  //  015C  CALL	R15	3
      0x58400013,  //  015D  LDCONST	R16	K19
      0x7C340600,  //  015E  CALL	R13	3
      0x7001FF98,  //  015F  JMP		#00F9
      0x581C000E,  //  0160  LDCONST	R7	K14
      0xAC1C0200,  //  0161  CATCH	R7	1	0
      0xB0080000,  //  0162  RAISE	2	R0	R0
      0xB81E1000,  //  0163  GETNGBL	R7	K8
      0x8C1C0F09,  //  0164  GETMET	R7	R7	K9
      0x8C24050A,  //  0165  GETMET	R9	R2	K10
      0x582C0045,  //  0166  LDCONST	R11	K69
      0x60300008,  //  0167  GETGBL	R12	G8
      0x88340114,  //  0168  GETMBR	R13	R0	K20
      0x88341B38,  //  0169  GETMBR	R13	R13	K56
      0x7C300200,  //  016A  CALL	R12	1
      0x7C240600,  //  016B  CALL	R9	3
      0x58280013,  //  016C  LDCONST	R10	K19
      0x7C1C0600,  //  016D  CALL	R7	3
      0x78160008,  //  016E  JMPF	R5	#0178
      0xB81E1000,  //  016F  GETNGBL	R7	K8
      0x8C1C0F09,  //  0170  GETMET	R7	R7	K9
      0x8C24050A,  //  0171  GETMET	R9	R2	K10
      0x582C0046,  //  0172  LDCONST	R11	K70
      0x5C300A00,  //  0173  MOVE	R12	R5
      0x7C240600,  //  0174  CALL	R9	3
      0x58280013,  //  0175  LDCONST	R10	K19
      0x7C1C0600,  //  0176  CALL	R7	3
      0x7002000C,  //  0177  JMP		#0185
      0x741A0002,  //  0178  JMPT	R6	#017C
      0x881C0114,  //  0179  GETMBR	R7	R0	K20
      0x881C0F34,  //  017A  GETMBR	R7	R7	K52
      0x741E0005,  //  017B  JMPT	R7	#0182
      0x881C0114,  //  017C  GETMBR	R7	R0	K20
      0x50200200,  //  017D  LDBOOL	R8	1	0
      0x901E6808,  //  017E  SETMBR	R7	K52	R8
      0x881C0114,  //  017F  GETMBR	R7	R0	K20
      0x8C1C0F1A,  //  0180  GETMET	R7	R7	K26
      0x7C1C0200,  //  0181  CALL	R7	1
      0x8C1C031B,  //  0182  GETMET	R7	R1	K27
      0x58240047,  //  0183  LDCONST	R9	K71
      0x7C1C0400,  //  0184  CALL	R7	2
      0x700200B9,  //  0185  JMP		#0240
      0x8C18030F,  //  0186  GETMET	R6	R1	K15
      0x58200048,  //  0187  LDCONST	R8	K72
      0x7C180400,  //  0188  CALL	R6	2
      0x781A0026,  //  0189  JMPF	R6	#01B1
      0x8C18030D,  //  018A  GETMET	R6	R1	K13
      0x58200049,  //  018B  LDCONST	R8	K73
      0x7C180400,  //  018C  CALL	R6	2
      0x8C1C030D,  //  018D  GETMET	R7	R1	K13
      0x5824000D,  //  018E  LDCONST	R9	K13
      0x7C1C0400,  //  018F  CALL	R7	2
      0xB8221000,  //  0190  GETNGBL	R8	K8
      0x8C201109,  //  0191  GETMET	R8	R8	K9
      0x8C28050A,  //  0192  GETMET	R10	R2	K10
      0x5830004A,  //  0193  LDCONST	R12	K74
      0x5C340C00,  //  0194  MOVE	R13	R6
      0x5C380E00,  //  0195  MOVE	R14	R7
      0x7C280800,  //  0196  CALL	R10	4
      0x582C0013,  //  0197  LDCONST	R11	K19
      0x7C200600,  //  0198  CALL	R8	3
      0x88200114,  //  0199  GETMBR	R8	R0	K20
      0x88201139,  //  019A  GETMBR	R8	R8	K57
      0x8C201136,  //  019B  GETMET	R8	R8	K54
      0x5C280C00,  //  019C  MOVE	R10	R6
      0x7C200400,  //  019D  CALL	R8	2
      0x4C240000,  //  019E  LDNIL	R9
      0x20241009,  //  019F  NE	R9	R8	R9
      0x7826000B,  //  01A0  JMPF	R9	#01AD
      0x60240013,  //  01A1  GETGBL	R9	G19
      0x7C240000,  //  01A2  CALL	R9	0
      0x8C281140,  //  01A3  GETMET	R10	R8	K64
      0x5C301000,  //  01A4  MOVE	R12	R8
      0x5C341200,  //  01A5  MOVE	R13	R9
      0x5C380E00,  //  01A6  MOVE	R14	R7
      0x7C280800,  //  01A7  CALL	R10	4
      0x88280114,  //  01A8  GETMBR	R10	R0	K20
      0x8C28154B,  //  01A9  GETMET	R10	R10	K75
      0x5C300C00,  //  01AA  MOVE	R12	R6
      0x5C341200,  //  01AB  MOVE	R13	R9
      0x7C280600,  //  01AC  CALL	R10	3
      0x8C24031B,  //  01AD  GETMET	R9	R1	K27
      0x582C0032,  //  01AE  LDCONST	R11	K50
      0x7C240400,  //  01AF  CALL	R9	2
      0x7002008E,  //  01B0  JMP		#0240
      0x8C18030F,  //  01B1  GETMET	R6	R1	K15
      0x5820004C,  //  01B2  LDCONST	R8	K76
      0x7C180400,  //  01B3  CALL	R6	2
      0x781A0059,  //  01B4  JMPF	R6	#020F
      0x8C18030D,  //  01B5  GETMET	R6	R1	K13
      0x5820004D,  //  01B6  LDCONST	R8	K77
      0x7C180400,  //  01B7  CALL	R6	2
      0x4C1C0000,  //  01B8  LDNIL	R7
      0x1C1C0C07,  //  01B9  EQ	R7	R6	R7
      0x741E0001,  //  01BA  JMPT	R7	#01BD
      0x1C1C0D3B,  //  01BB  EQ	R7	R6	K59
      0x781E0000,  //  01BC  JMPF	R7	#01BE
      0xB0069D4F,  //  01BD  RAISE	1	K78	K79
      0x581C0005,  //  01BE  LDCONST	R7	K5
      0x60200008,  //  01BF  GETGBL	R8	G8
      0x5C240E00,  //  01C0  MOVE	R9	R7
      0x7C200200,  //  01C1  CALL	R8	1
      0x8C24030F,  //  01C2  GETMET	R9	R1	K15
      0x002E9208,  //  01C3  ADD	R11	K73	R8
      0x7C240400,  //  01C4  CALL	R9	2
      0x78260044,  //  01C5  JMPF	R9	#020B
      0x8C24030D,  //  01C6  GETMET	R9	R1	K13
      0x002E9208,  //  01C7  ADD	R11	K73	R8
      0x7C240400,  //  01C8  CALL	R9	2
      0x8C28030D,  //  01C9  GETMET	R10	R1	K13
      0x00321A08,  //  01CA  ADD	R12	K13	R8
      0x7C280400,  //  01CB  CALL	R10	2
      0x202C133B,  //  01CC  NE	R11	R9	K59
      0x782E0036,  //  01CD  JMPF	R11	#0205
      0x882C0114,  //  01CE  GETMBR	R11	R0	K20
      0x882C1739,  //  01CF  GETMBR	R11	R11	K57
      0x8C2C1736,  //  01D0  GETMET	R11	R11	K54
      0x5C341200,  //  01D1  MOVE	R13	R9
      0x7C2C0400,  //  01D2  CALL	R11	2
      0x4C300000,  //  01D3  LDNIL	R12
      0x2030160C,  //  01D4  NE	R12	R11	R12
      0x7832002E,  //  01D5  JMPF	R12	#0205
      0x60300013,  //  01D6  GETGBL	R12	G19
      0x7C300000,  //  01D7  CALL	R12	0
      0x98329A06,  //  01D8  SETIDX	R12	K77	R6
      0x98327409,  //  01D9  SETIDX	R12	K58	R9
      0x8C341740,  //  01DA  GETMET	R13	R11	K64
      0x5C3C1600,  //  01DB  MOVE	R15	R11
      0x5C401800,  //  01DC  MOVE	R16	R12
      0x5C441400,  //  01DD  MOVE	R17	R10
      0x7C340800,  //  01DE  CALL	R13	4
      0x50340000,  //  01DF  LDBOOL	R13	0	0
      0x60380010,  //  01E0  GETGBL	R14	G16
      0x883C0114,  //  01E1  GETMBR	R15	R0	K20
      0x883C1F38,  //  01E2  GETMBR	R15	R15	K56
      0x7C380200,  //  01E3  CALL	R14	1
      0xA802000B,  //  01E4  EXBLK	0	#01F1
      0x5C3C1C00,  //  01E5  MOVE	R15	R14
      0x7C3C0000,  //  01E6  CALL	R15	0
      0x8C400150,  //  01E7  GETMET	R16	R0	K80
      0x5C481E00,  //  01E8  MOVE	R18	R15
      0x5C4C1800,  //  01E9  MOVE	R19	R12
      0x7C400600,  //  01EA  CALL	R16	3
      0x78420001,  //  01EB  JMPF	R16	#01EE
      0x50340200,  //  01EC  LDBOOL	R13	1	0
      0x70020000,  //  01ED  JMP		#01EF
      0x7001FFF5,  //  01EE  JMP		#01E5
      0xA8040001,  //  01EF  EXBLK	1	1
      0x70020002,  //  01F0  JMP		#01F4
      0x5838000E,  //  01F1  LDCONST	R14	K14
      0xAC380200,  //  01F2  CATCH	R14	1	0
      0xB0080000,  //  01F3  RAISE	2	R0	R0
      0x5C381A00,  //  01F4  MOVE	R14	R13
      0x743A000E,  //  01F5  JMPT	R14	#0205
      0xB83A1000,  //  01F6  GETNGBL	R14	K8
      0x8C381D09,  //  01F7  GETMET	R14	R14	K9
      0x8C40050A,  //  01F8  GETMET	R16	R2	K10
      0x58480051,  //  01F9  LDCONST	R18	K81
      0x5C4C0C00,  //  01FA  MOVE	R19	R6
      0x5C501200,  //  01FB  MOVE	R20	R9
      0x5C541400,  //  01FC  MOVE	R21	R10
      0x7C400A00,  //  01FD  CALL	R16	5
      0x58440013,  //  01FE  LDCONST	R17	K19
      0x7C380600,  //  01FF  CALL	R14	3
      0x88380114,  //  0200  GETMBR	R14	R0	K20
      0x8C381D4B,  //  0201  GETMET	R14	R14	K75
      0x5C401200,  //  0202  MOVE	R16	R9
      0x5C441800,  //  0203  MOVE	R17	R12
      0x7C380600,  //  0204  CALL	R14	3
      0x001C0F07,  //  0205  ADD	R7	R7	K7
      0x602C0008,  //  0206  GETGBL	R11	G8
      0x5C300E00,  //  0207  MOVE	R12	R7
      0x7C2C0200,  //  0208  CALL	R11	1
      0x5C201600,  //  0209  MOVE	R8	R11
      0x7001FFB6,  //  020A  JMP		#01C2
      0x8C24031B,  //  020B  GETMET	R9	R1	K27
      0x582C0032,  //  020C  LDCONST	R11	K50
      0x7C240400,  //  020D  CALL	R9	2
      0x70020030,  //  020E  JMP		#0240
      0x4C180000,  //  020F  LDNIL	R6
      0x601C0010,  //  0210  GETGBL	R7	G16
      0x8C200306,  //  0211  GETMET	R8	R1	K6
      0x7C200200,  //  0212  CALL	R8	1
      0x04201107,  //  0213  SUB	R8	R8	K7
      0x40220A08,  //  0214  CONNECT	R8	K5	R8
      0x7C1C0200,  //  0215  CALL	R7	1
      0xA8020013,  //  0216  EXBLK	0	#022B
      0x5C200E00,  //  0217  MOVE	R8	R7
      0x7C200000,  //  0218  CALL	R8	0
      0x8C24030C,  //  0219  GETMET	R9	R1	K12
      0x5C2C1000,  //  021A  MOVE	R11	R8
      0x7C240400,  //  021B  CALL	R9	2
      0x8C280536,  //  021C  GETMET	R10	R2	K54
      0x5C301200,  //  021D  MOVE	R12	R9
      0x58340052,  //  021E  LDCONST	R13	K82
      0x7C280600,  //  021F  CALL	R10	3
      0x1C281505,  //  0220  EQ	R10	R10	K5
      0x782A0005,  //  0221  JMPF	R10	#0228
      0x60280009,  //  0222  GETGBL	R10	G9
      0x402E2737,  //  0223  CONNECT	R11	K19	K55
      0x942C120B,  //  0224  GETIDX	R11	R9	R11
      0x7C280200,  //  0225  CALL	R10	1
      0x5C181400,  //  0226  MOVE	R6	R10
      0x70020000,  //  0227  JMP		#0229
      0x7001FFED,  //  0228  JMP		#0217
      0xA8040001,  //  0229  EXBLK	1	1
      0x70020002,  //  022A  JMP		#022E
      0x581C000E,  //  022B  LDCONST	R7	K14
      0xAC1C0200,  //  022C  CATCH	R7	1	0
      0xB0080000,  //  022D  RAISE	2	R0	R0
      0x4C1C0000,  //  022E  LDNIL	R7
      0x201C0C07,  //  022F  NE	R7	R6	R7
      0x781E000E,  //  0230  JMPF	R7	#0240
      0xB81E1000,  //  0231  GETNGBL	R7	K8
      0x8C1C0F09,  //  0232  GETMET	R7	R7	K9
      0x8C24050A,  //  0233  GETMET	R9	R2	K10
      0x582C0053,  //  0234  LDCONST	R11	K83
      0x5C300C00,  //  0235  MOVE	R12	R6
      0x7C240600,  //  0236  CALL	R9	3
      0x58280054,  //  0237  LDCONST	R10	K84
      0x7C1C0600,  //  0238  CALL	R7	3
      0x881C0114,  //  0239  GETMBR	R7	R0	K20
      0x8C1C0F55,  //  023A  GETMET	R7	R7	K85
      0x5C240C00,  //  023B  MOVE	R9	R6
      0x7C1C0400,  //  023C  CALL	R7	2
      0x8C1C031B,  //  023D  GETMET	R7	R1	K27
      0x58240032,  //  023E  LDCONST	R9	K50
      0x7C1C0400,  //  023F  CALL	R7	2
      0x78160011,  //  0240  JMPF	R5	#0253
      0x8C180356,  //  0241  GETMET	R6	R1	K86
      0x58200057,  //  0242  LDCONST	R8	K87
      0x7C180400,  //  0243  CALL	R6	2
      0x8C180358,  //  0244  GETMET	R6	R1	K88
      0x7C180200,  //  0245  CALL	R6	1
      0x8C180359,  //  0246  GETMET	R6	R1	K89
      0x8C20050A,  //  0247  GETMET	R8	R2	K10
      0x5828005A,  //  0248  LDCONST	R10	K90
      0x8C2C035B,  //  0249  GETMET	R11	R1	K91
      0x5C340A00,  //  024A  MOVE	R13	R5
      0x7C2C0400,  //  024B  CALL	R11	2
      0x7C200600,  //  024C  CALL	R8	3
      0x7C180400,  //  024D  CALL	R6	2
      0x8C18035C,  //  024E  GETMET	R6	R1	K92
      0x8820035D,  //  024F  GETMBR	R8	R1	K93
      0x7C180400,  //  0250  CALL	R6	2
      0x8C18035E,  //  0251  GETMET	R6	R1	K94
      0x7C180200,  //  0252  CALL	R6	1
      0xA8040001,  //  0253  EXBLK	1	1
      0x7002001D,  //  0254  JMP		#0273
      0xAC180002,  //  0255  CATCH	R6	0	2
      0x7002001A,  //  0256  JMP		#0272
      0xB8221000,  //  0257  GETNGBL	R8	K8
      0x8C201109,  //  0258  GETMET	R8	R8	K9
      0x8C28050A,  //  0259  GETMET	R10	R2	K10
      0x5830005F,  //  025A  LDCONST	R12	K95
      0x5C340C00,  //  025B  MOVE	R13	R6
      0x5C380E00,  //  025C  MOVE	R14	R7
      0x7C280800,  //  025D  CALL	R10	4
      0x582C0054,  //  025E  LDCONST	R11	K84
      0x7C200600,  //  025F  CALL	R8	3
      0x8C200356,  //  0260  GETMET	R8	R1	K86
      0x58280057,  //  0261  LDCONST	R10	K87
      0x7C200400,  //  0262  CALL	R8	2
      0x8C200358,  //  0263  GETMET	R8	R1	K88
      0x7C200200,  //  0264  CALL	R8	1
      0x8C200359,  //  0265  GETMET	R8	R1	K89
      0x8C28050A,  //  0266  GETMET	R10	R2	K10
      0x58300060,  //  0267  LDCONST	R12	K96
      0x5C340C00,  //  0268  MOVE	R13	R6
      0x5C380E00,  //  0269  MOVE	R14	R7
      0x7C280800,  //  026A  CALL	R10	4
      0x7C200400,  //  026B  CALL	R8	2
      0x8C20035C,  //  026C  GETMET	R8	R1	K92
      0x8828035D,  //  026D  GETMBR	R10	R1	K93
      0x7C200400,  //  026E  CALL	R8	2
      0x8C20035E,  //  026F  GETMET	R8	R1	K94
      0x7C200200,  //  0270  CALL	R8	1
      0x70020000,  //  0271  JMP		#0273
      0xB0080000,  //  0272  RAISE	2	R0	R0
      0x80000000,  //  0273  RET	0
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
    ( &(const bvalue[45]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(json),
    /* K3   */  be_nested_str_weak(),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(Plugin_Bridge_HTTP),
    /* K6   */  be_nested_str_weak(PROBE_TIMEOUT),
    /* K7   */  be_nested_str_weak(HTTP_remote),
    /* K8   */  be_nested_str_weak(call_sync),
    /* K9   */  be_nested_str_weak(Status_X208),
    /* K10  */  be_nested_str_weak(load),
    /* K11  */  be_nested_str_weak(find),
    /* K12  */  be_nested_str_weak(StatusSNS),
    /* K13  */  be_nested_str_weak(Status_X2011),
    /* K14  */  be_nested_str_weak(StatusSTS),
    /* K15  */  be_nested_str_weak(tasmota),
    /* K16  */  be_nested_str_weak(log),
    /* K17  */  be_nested_str_weak(format),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20probed_X20_X27_X25s_X27_X20status8_X3D_X25s_X20satus11_X3D_X25s),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(generate_config_from_status),
    /* K21  */  be_nested_str_weak(content_send),
    /* K22  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Remote_X20Device_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K23  */  be_nested_str_weak(_X3Cp_X3E_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X22_X3E_X25s_X3C_X2Fa_X3E_X3C_X2Fp_X3E),
    /* K24  */  be_nested_str_weak(html_escape),
    /* K25  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X27145_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X3EParameter_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K26  */  be_nested_str_weak(_X3Cinput_X20name_X3D_X27url_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X25s_X27_X3E),
    /* K27  */  be_const_int(0),
    /* K28  */  be_nested_str_weak(type),
    /* K29  */  be_nested_str_weak(http_),
    /* K30  */  be_nested_str_weak(device),
    /* K31  */  be_nested_str_weak(plugins_classes),
    /* K32  */  be_nested_str_weak(ui_conf_to_string),
    /* K33  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cselect_X20name_X3D_X27pi_X25i_X27_X3E),
    /* K34  */  be_nested_str_weak(plugin_option),
    /* K35  */  be_nested_str_weak(_CLASSES_TYPES2),
    /* K36  */  be_nested_str_weak(_X3C_X2Fselect_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E),
    /* K37  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X25i_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X25s_X27_X3E),
    /* K38  */  be_nested_str_weak(_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K39  */  be_const_int(1),
    /* K40  */  be_nested_str_weak(_X3C_X2Ftable_X3E),
    /* K41  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K42  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27addrem_X27_X20class_X3D_X27button_X20bgrn_X27_X3EAdd_X20endpoints_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K43  */  be_nested_str_weak(_X3C_X2Fform_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K44  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3EUnable_X20to_X20connect_X20to_X20_X27_X25s_X27_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_remote_autoconf),
    &be_const_str_solidified,
    ( &(const binstruction[204]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0x1C140303,  //  0003  EQ	R5	R1	K3
      0x78160000,  //  0004  JMPF	R5	#0006
      0x80000A00,  //  0005  RET	0
      0xB8160800,  //  0006  GETNGBL	R5	K4
      0x88140B05,  //  0007  GETMBR	R5	R5	K5
      0x88140B06,  //  0008  GETMBR	R5	R5	K6
      0xB81A0800,  //  0009  GETNGBL	R6	K4
      0x8C180D07,  //  000A  GETMET	R6	R6	K7
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240A00,  //  000C  MOVE	R9	R5
      0x7C180600,  //  000D  CALL	R6	3
      0x8C1C0D08,  //  000E  GETMET	R7	R6	K8
      0x58240009,  //  000F  LDCONST	R9	K9
      0x5C280A00,  //  0010  MOVE	R10	R5
      0x7C1C0600,  //  0011  CALL	R7	3
      0x4C200000,  //  0012  LDNIL	R8
      0x20200E08,  //  0013  NE	R8	R7	R8
      0x78220003,  //  0014  JMPF	R8	#0019
      0x8C20090A,  //  0015  GETMET	R8	R4	K10
      0x5C280E00,  //  0016  MOVE	R10	R7
      0x7C200400,  //  0017  CALL	R8	2
      0x5C1C1000,  //  0018  MOVE	R7	R8
      0x4C200000,  //  0019  LDNIL	R8
      0x20200E08,  //  001A  NE	R8	R7	R8
      0x78220003,  //  001B  JMPF	R8	#0020
      0x8C200F0B,  //  001C  GETMET	R8	R7	K11
      0x5828000C,  //  001D  LDCONST	R10	K12
      0x7C200400,  //  001E  CALL	R8	2
      0x5C1C1000,  //  001F  MOVE	R7	R8
      0x4C200000,  //  0020  LDNIL	R8
      0x4C240000,  //  0021  LDNIL	R9
      0x20240E09,  //  0022  NE	R9	R7	R9
      0x78260012,  //  0023  JMPF	R9	#0037
      0x8C240D08,  //  0024  GETMET	R9	R6	K8
      0x582C000D,  //  0025  LDCONST	R11	K13
      0x5C300A00,  //  0026  MOVE	R12	R5
      0x7C240600,  //  0027  CALL	R9	3
      0x5C201200,  //  0028  MOVE	R8	R9
      0x4C240000,  //  0029  LDNIL	R9
      0x20241009,  //  002A  NE	R9	R8	R9
      0x78260003,  //  002B  JMPF	R9	#0030
      0x8C24090A,  //  002C  GETMET	R9	R4	K10
      0x5C2C1000,  //  002D  MOVE	R11	R8
      0x7C240400,  //  002E  CALL	R9	2
      0x5C201200,  //  002F  MOVE	R8	R9
      0x4C240000,  //  0030  LDNIL	R9
      0x20241009,  //  0031  NE	R9	R8	R9
      0x78260003,  //  0032  JMPF	R9	#0037
      0x8C24110B,  //  0033  GETMET	R9	R8	K11
      0x582C000E,  //  0034  LDCONST	R11	K14
      0x7C240400,  //  0035  CALL	R9	2
      0x5C201200,  //  0036  MOVE	R8	R9
      0x4C240000,  //  0037  LDNIL	R9
      0x20240E09,  //  0038  NE	R9	R7	R9
      0x78260088,  //  0039  JMPF	R9	#00C3
      0x4C240000,  //  003A  LDNIL	R9
      0x20241009,  //  003B  NE	R9	R8	R9
      0x78260085,  //  003C  JMPF	R9	#00C3
      0xB8261E00,  //  003D  GETNGBL	R9	K15
      0x8C241310,  //  003E  GETMET	R9	R9	K16
      0x8C2C0711,  //  003F  GETMET	R11	R3	K17
      0x58340012,  //  0040  LDCONST	R13	K18
      0x5C380200,  //  0041  MOVE	R14	R1
      0x603C0008,  //  0042  GETGBL	R15	G8
      0x5C400E00,  //  0043  MOVE	R16	R7
      0x7C3C0200,  //  0044  CALL	R15	1
      0x60400008,  //  0045  GETGBL	R16	G8
      0x5C441000,  //  0046  MOVE	R17	R8
      0x7C400200,  //  0047  CALL	R16	1
      0x7C2C0A00,  //  0048  CALL	R11	5
      0x58300013,  //  0049  LDCONST	R12	K19
      0x7C240600,  //  004A  CALL	R9	3
      0x8C240114,  //  004B  GETMET	R9	R0	K20
      0x5C2C0E00,  //  004C  MOVE	R11	R7
      0x5C301000,  //  004D  MOVE	R12	R8
      0x7C240600,  //  004E  CALL	R9	3
      0x8C280515,  //  004F  GETMET	R10	R2	K21
      0x58300016,  //  0050  LDCONST	R12	K22
      0x7C280400,  //  0051  CALL	R10	2
      0x8C280515,  //  0052  GETMET	R10	R2	K21
      0x8C300711,  //  0053  GETMET	R12	R3	K17
      0x58380017,  //  0054  LDCONST	R14	K23
      0x8C3C0518,  //  0055  GETMET	R15	R2	K24
      0x5C440200,  //  0056  MOVE	R17	R1
      0x7C3C0400,  //  0057  CALL	R15	2
      0x8C400518,  //  0058  GETMET	R16	R2	K24
      0x5C480200,  //  0059  MOVE	R18	R1
      0x7C400400,  //  005A  CALL	R16	2
      0x7C300800,  //  005B  CALL	R12	4
      0x7C280400,  //  005C  CALL	R10	2
      0x8C280515,  //  005D  GETMET	R10	R2	K21
      0x58300019,  //  005E  LDCONST	R12	K25
      0x7C280400,  //  005F  CALL	R10	2
      0x8C280515,  //  0060  GETMET	R10	R2	K21
      0x8C300711,  //  0061  GETMET	R12	R3	K17
      0x5838001A,  //  0062  LDCONST	R14	K26
      0x8C3C0518,  //  0063  GETMET	R15	R2	K24
      0x5C440200,  //  0064  MOVE	R17	R1
      0x7C3C0400,  //  0065  CALL	R15	2
      0x7C300600,  //  0066  CALL	R12	3
      0x7C280400,  //  0067  CALL	R10	2
      0x5828001B,  //  0068  LDCONST	R10	K27
      0x602C000C,  //  0069  GETGBL	R11	G12
      0x5C301200,  //  006A  MOVE	R12	R9
      0x7C2C0200,  //  006B  CALL	R11	1
      0x142C140B,  //  006C  LT	R11	R10	R11
      0x782E0030,  //  006D  JMPF	R11	#009F
      0x942C120A,  //  006E  GETIDX	R11	R9	R10
      0x8C30170B,  //  006F  GETMET	R12	R11	K11
      0x5838001C,  //  0070  LDCONST	R14	K28
      0x583C0003,  //  0071  LDCONST	R15	K3
      0x7C300600,  //  0072  CALL	R12	3
      0x20341903,  //  0073  NE	R13	R12	K3
      0x78360000,  //  0074  JMPF	R13	#0076
      0x00323A0C,  //  0075  ADD	R12	K29	R12
      0x8834011E,  //  0076  GETMBR	R13	R0	K30
      0x88341B1F,  //  0077  GETMBR	R13	R13	K31
      0x8C341B0B,  //  0078  GETMET	R13	R13	K11
      0x5C3C1800,  //  0079  MOVE	R15	R12
      0x7C340400,  //  007A  CALL	R13	2
      0x58380003,  //  007B  LDCONST	R14	K3
      0x4C3C0000,  //  007C  LDNIL	R15
      0x203C1A0F,  //  007D  NE	R15	R13	R15
      0x783E0004,  //  007E  JMPF	R15	#0084
      0x8C3C1B20,  //  007F  GETMET	R15	R13	K32
      0x5C441A00,  //  0080  MOVE	R17	R13
      0x5C481600,  //  0081  MOVE	R18	R11
      0x7C3C0600,  //  0082  CALL	R15	3
      0x5C381E00,  //  0083  MOVE	R14	R15
      0x8C3C0515,  //  0084  GETMET	R15	R2	K21
      0x8C440711,  //  0085  GETMET	R17	R3	K17
      0x584C0021,  //  0086  LDCONST	R19	K33
      0x5C501400,  //  0087  MOVE	R20	R10
      0x7C440600,  //  0088  CALL	R17	3
      0x7C3C0400,  //  0089  CALL	R15	2
      0x8C3C0122,  //  008A  GETMET	R15	R0	K34
      0x5C441800,  //  008B  MOVE	R17	R12
      0x88480123,  //  008C  GETMBR	R18	R0	K35
      0x7C3C0600,  //  008D  CALL	R15	3
      0x8C3C0515,  //  008E  GETMET	R15	R2	K21
      0x58440024,  //  008F  LDCONST	R17	K36
      0x7C3C0400,  //  0090  CALL	R15	2
      0x8C3C0515,  //  0091  GETMET	R15	R2	K21
      0x8C440711,  //  0092  GETMET	R17	R3	K17
      0x584C0025,  //  0093  LDCONST	R19	K37
      0x5C501400,  //  0094  MOVE	R20	R10
      0x8C540518,  //  0095  GETMET	R21	R2	K24
      0x5C5C1C00,  //  0096  MOVE	R23	R14
      0x7C540400,  //  0097  CALL	R21	2
      0x7C440800,  //  0098  CALL	R17	4
      0x7C3C0400,  //  0099  CALL	R15	2
      0x8C3C0515,  //  009A  GETMET	R15	R2	K21
      0x58440026,  //  009B  LDCONST	R17	K38
      0x7C3C0400,  //  009C  CALL	R15	2
      0x00281527,  //  009D  ADD	R10	R10	K39
      0x7001FFC9,  //  009E  JMP		#0069
      0x8C2C0515,  //  009F  GETMET	R11	R2	K21
      0x8C340711,  //  00A0  GETMET	R13	R3	K17
      0x583C0021,  //  00A1  LDCONST	R15	K33
      0x5C401400,  //  00A2  MOVE	R16	R10
      0x7C340600,  //  00A3  CALL	R13	3
      0x7C2C0400,  //  00A4  CALL	R11	2
      0x8C2C0122,  //  00A5  GETMET	R11	R0	K34
      0x58340003,  //  00A6  LDCONST	R13	K3
      0x88380123,  //  00A7  GETMBR	R14	R0	K35
      0x7C2C0600,  //  00A8  CALL	R11	3
      0x8C2C0515,  //  00A9  GETMET	R11	R2	K21
      0x58340024,  //  00AA  LDCONST	R13	K36
      0x7C2C0400,  //  00AB  CALL	R11	2
      0x8C2C0515,  //  00AC  GETMET	R11	R2	K21
      0x8C340711,  //  00AD  GETMET	R13	R3	K17
      0x583C0025,  //  00AE  LDCONST	R15	K37
      0x5C401400,  //  00AF  MOVE	R16	R10
      0x58440003,  //  00B0  LDCONST	R17	K3
      0x7C340800,  //  00B1  CALL	R13	4
      0x7C2C0400,  //  00B2  CALL	R11	2
      0x8C2C0515,  //  00B3  GETMET	R11	R2	K21
      0x58340026,  //  00B4  LDCONST	R13	K38
      0x7C2C0400,  //  00B5  CALL	R11	2
      0x8C2C0515,  //  00B6  GETMET	R11	R2	K21
      0x58340028,  //  00B7  LDCONST	R13	K40
      0x7C2C0400,  //  00B8  CALL	R11	2
      0x8C2C0515,  //  00B9  GETMET	R11	R2	K21
      0x58340029,  //  00BA  LDCONST	R13	K41
      0x7C2C0400,  //  00BB  CALL	R11	2
      0x8C2C0515,  //  00BC  GETMET	R11	R2	K21
      0x5834002A,  //  00BD  LDCONST	R13	K42
      0x7C2C0400,  //  00BE  CALL	R11	2
      0x8C2C0515,  //  00BF  GETMET	R11	R2	K21
      0x5834002B,  //  00C0  LDCONST	R13	K43
      0x7C2C0400,  //  00C1  CALL	R11	2
      0x70020007,  //  00C2  JMP		#00CB
      0x8C240515,  //  00C3  GETMET	R9	R2	K21
      0x8C2C0711,  //  00C4  GETMET	R11	R3	K17
      0x5834002C,  //  00C5  LDCONST	R13	K44
      0x8C380518,  //  00C6  GETMET	R14	R2	K24
      0x5C400200,  //  00C7  MOVE	R16	R1
      0x7C380400,  //  00C8  CALL	R14	2
      0x7C2C0600,  //  00C9  CALL	R11	3
      0x7C240400,  //  00CA  CALL	R9	2
      0x80000000,  //  00CB  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_mgr_add
********************************************************************/
be_local_closure(Matter_UI_page_part_mgr_add,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(check_privileged_access),
    /* K3   */  be_nested_str_weak(content_start),
    /* K4   */  be_nested_str_weak(Matter_X20Create_X20new_X20endpoint),
    /* K5   */  be_nested_str_weak(content_send_style),
    /* K6   */  be_nested_str_weak(arg),
    /* K7   */  be_nested_str_weak(url),
    /* K8   */  be_nested_str_weak(matter_enabled),
    /* K9   */  be_nested_str_weak(show_remote_autoconf),
    /* K10  */  be_nested_str_weak(content_button),
    /* K11  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K12  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr_add),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
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
      0x8C0C0306,  //  000C  GETMET	R3	R1	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C100108,  //  000F  GETMET	R4	R0	K8
      0x7C100200,  //  0010  CALL	R4	1
      0x78120002,  //  0011  JMPF	R4	#0015
      0x8C100109,  //  0012  GETMET	R4	R0	K9
      0x5C180600,  //  0013  MOVE	R6	R3
      0x7C100400,  //  0014  CALL	R4	2
      0x8C10030A,  //  0015  GETMET	R4	R1	K10
      0x8818030B,  //  0016  GETMBR	R6	R1	K11
      0x7C100400,  //  0017  CALL	R4	2
      0x8C10030C,  //  0018  GETMET	R4	R1	K12
      0x7C100200,  //  0019  CALL	R4	1
      0x80000000,  //  001A  RET	0
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(matter_enabled),
    /* K3   */  be_nested_str_weak(content_send),
    /* K4   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X20style_X3D_X27width_X3A320px_X3B_X27_X3ECheck_X20the_X20_X3Ca_X20href_X3D_X27https_X3A_X2F_X2Ftasmota_X2Egithub_X2Eio_X2Fdocs_X2FMatter_X2F_X27_X20target_X3D_X27_blank_X27_X3EMatter_X20documentation_X3C_X2Fa_X3E_X2E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20id_X3D_X27menable_X27_X20type_X3D_X27checkbox_X27_X20name_X3D_X27menable_X27_X20_X25s_X3E),
    /* K7   */  be_nested_str_weak(checked),
    /* K8   */  be_nested_str_weak(),
    /* K9   */  be_nested_str_weak(_X3Clabel_X20for_X3D_X27menable_X27_X3E_X3Cb_X3EMatter_X20enable_X3C_X2Fb_X3E_X3C_X2Flabel_X3E_X3C_X2Fp_X3E),
    /* K10  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20id_X3D_X27comm_X27_X20type_X3D_X27checkbox_X27_X20name_X3D_X27comm_X27_X20_X25s_X3E),
    /* K11  */  be_nested_str_weak(device),
    /* K12  */  be_nested_str_weak(commissioning_open),
    /* K13  */  be_nested_str_weak(_X3Clabel_X20for_X3D_X27comm_X27_X3E_X3Cb_X3ECommissioning_X20open_X3C_X2Fb_X3E_X3C_X2Flabel_X3E_X3C_X2Fp_X3E),
    /* K14  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27save_X27_X20class_X3D_X27button_X20bgrn_X27_X3ESave_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_enable),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x8C100303,  //  0003  GETMET	R4	R1	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x7C100400,  //  0005  CALL	R4	2
      0x8C100303,  //  0006  GETMET	R4	R1	K3
      0x8C180505,  //  0007  GETMET	R6	R2	K5
      0x58200006,  //  0008  LDCONST	R8	K6
      0x8C240102,  //  0009  GETMET	R9	R0	K2
      0x7C240200,  //  000A  CALL	R9	1
      0x78260001,  //  000B  JMPF	R9	#000E
      0x58240007,  //  000C  LDCONST	R9	K7
      0x70020000,  //  000D  JMP		#000F
      0x58240008,  //  000E  LDCONST	R9	K8
      0x7C180600,  //  000F  CALL	R6	3
      0x7C100400,  //  0010  CALL	R4	2
      0x8C100303,  //  0011  GETMET	R4	R1	K3
      0x58180009,  //  0012  LDCONST	R6	K9
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100102,  //  0014  GETMET	R4	R0	K2
      0x7C100200,  //  0015  CALL	R4	1
      0x7812000F,  //  0016  JMPF	R4	#0027
      0x8C100303,  //  0017  GETMET	R4	R1	K3
      0x8C180505,  //  0018  GETMET	R6	R2	K5
      0x5820000A,  //  0019  LDCONST	R8	K10
      0x8824010B,  //  001A  GETMBR	R9	R0	K11
      0x8824130C,  //  001B  GETMBR	R9	R9	K12
      0x4C280000,  //  001C  LDNIL	R10
      0x2024120A,  //  001D  NE	R9	R9	R10
      0x78260001,  //  001E  JMPF	R9	#0021
      0x58240007,  //  001F  LDCONST	R9	K7
      0x70020000,  //  0020  JMP		#0022
      0x58240008,  //  0021  LDCONST	R9	K8
      0x7C180600,  //  0022  CALL	R6	3
      0x7C100400,  //  0023  CALL	R4	2
      0x8C100303,  //  0024  GETMET	R4	R1	K3
      0x5818000D,  //  0025  LDCONST	R6	K13
      0x7C100400,  //  0026  CALL	R4	2
      0x8C100303,  //  0027  GETMET	R4	R1	K3
      0x5818000E,  //  0028  LDCONST	R6	K14
      0x7C100400,  //  0029  CALL	R4	2
      0x80000000,  //  002A  RET	0
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
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(device),
    /* K3   */  be_nested_str_weak(commissioning_open),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(millis),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(content_send),
    /* K8   */  be_nested_str_weak(format),
    /* K9   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BCommissioning_X20open_X20for_X20_X25i_X20min_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K10  */  be_nested_str_weak(compute_manual_pairing_code),
    /* K11  */  be_nested_str_weak(_X3Cp_X3EManual_X20pairing_X20code_X3A_X3Cbr_X3E_X3Cb_X3E_X25s_X2D_X25s_X2D_X25s_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Chr_X3E),
    /* K12  */  be_const_int(3),
    /* K13  */  be_const_int(2147483647),
    /* K14  */  be_nested_str_weak(_X3Cdiv_X3E_X3Ccenter_X3E),
    /* K15  */  be_nested_str_weak(compute_qrcode_content),
    /* K16  */  be_nested_str_weak(show_qrcode),
    /* K17  */  be_nested_str_weak(_X3Cp_X3E_X20_X25s_X3C_X2Fp_X3E),
    /* K18  */  be_nested_str_weak(_X3C_X2Fdiv_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_commissioning_info),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
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
      0x8C180307,  //  0028  GETMET	R6	R1	K7
      0x5820000E,  //  0029  LDCONST	R8	K14
      0x7C180400,  //  002A  CALL	R6	2
      0x88180102,  //  002B  GETMBR	R6	R0	K2
      0x8C180D0F,  //  002C  GETMET	R6	R6	K15
      0x7C180200,  //  002D  CALL	R6	1
      0x8C1C0110,  //  002E  GETMET	R7	R0	K16
      0x5C240C00,  //  002F  MOVE	R9	R6
      0x7C1C0400,  //  0030  CALL	R7	2
      0x8C1C0307,  //  0031  GETMET	R7	R1	K7
      0x8C240508,  //  0032  GETMET	R9	R2	K8
      0x582C0011,  //  0033  LDCONST	R11	K17
      0x5C300C00,  //  0034  MOVE	R12	R6
      0x7C240600,  //  0035  CALL	R9	3
      0x7C1C0400,  //  0036  CALL	R7	2
      0x8C1C0307,  //  0037  GETMET	R7	R1	K7
      0x58240012,  //  0038  LDCONST	R9	K18
      0x7C1C0400,  //  0039  CALL	R7	2
      0x80000000,  //  003A  RET	0
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
** Solidified function: show_plugins_configuration
********************************************************************/
be_local_closure(Matter_UI_show_plugins_configuration,   /* name */
  be_nested_proto(
    26,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[59]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(content_send),
    /* K4   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BCurrent_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K5   */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3Cp_X3E_X3Cb_X3ELocal_X20sensors_X20and_X20devices_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X3E_X23_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X3EParameter_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(plugins_config),
    /* K8   */  be_nested_str_weak(remove),
    /* K9   */  be_nested_str_weak(0),
    /* K10  */  be_nested_str_weak(k2l_num),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(find),
    /* K13  */  be_nested_str_weak(type),
    /* K14  */  be_const_int(1),
    /* K15  */  be_nested_str_weak(http_),
    /* K16  */  be_nested_str_weak(plugins_classes),
    /* K17  */  be_nested_str_weak(),
    /* K18  */  be_nested_str_weak(ui_conf_to_string),
    /* K19  */  be_nested_str_weak(format),
    /* K20  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X25i_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K21  */  be_nested_str_weak(_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K22  */  be_nested_str_weak(plugin_name),
    /* K23  */  be_nested_str_weak(_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X25i_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X25s_X27_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K24  */  be_nested_str_weak(html_escape),
    /* K25  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27text_X2Dalign_X3Acenter_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27del_X25i_X27_X20style_X3D_X27background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3B_X27_X20onclick_X3D_X22return_X20confirm_X28_X27Confirm_X20removing_X20endpoint_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K26  */  be_nested_str_weak(_X3C_X2Ftable_X3E),
    /* K27  */  be_nested_str_weak(_X3Cp_X3E_X26lt_X3Bnone_X26gt_X3B_X3C_X2Fp_X3E),
    /* K28  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K29  */  be_nested_str_weak(url),
    /* K30  */  be_nested_str_weak(push),
    /* K31  */  be_nested_str_weak(stop_iteration),
    /* K32  */  be_nested_str_weak(sort_distinct),
    /* K33  */  be_nested_str_weak(tasmota),
    /* K34  */  be_nested_str_weak(log),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20remotes_X3A_X20),
    /* K36  */  be_const_int(3),
    /* K37  */  be_nested_str_weak(_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X22_X3E_X25s_X3C_X2Fa_X3E),
    /* K38  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K39  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K40  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2722_X27_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X25i_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K41  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X27115_X27_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Fselect_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K42  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X2715_X27_X20style_X3D_X27text_X2Dalign_X3Acenter_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27del_X25i_X27_X20style_X3D_X27background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3B_X27_X20onclick_X3D_X22return_X20confirm_X28_X27Confirm_X20removing_X20endpoint_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K43  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K44  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27config_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X20configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    /* K45  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BAdd_X20to_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K46  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3EAdd_X20local_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X27145_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X3EParameter_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K47  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cselect_X20name_X3D_X27pi_X27_X3E),
    /* K48  */  be_nested_str_weak(plugin_option),
    /* K49  */  be_nested_str_weak(_CLASSES_TYPES),
    /* K50  */  be_nested_str_weak(_X3C_X2Fselect_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K51  */  be_nested_str_weak(_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X27_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E),
    /* K52  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K53  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27addep_X27_X20class_X3D_X27button_X20bgrn_X27_X3ECreate_X20new_X20endpoint_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K54  */  be_nested_str_weak(_X3Chr_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20Tasmota_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatteradd_X27_X20method_X3D_X27get_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K55  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2730_X27_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3Ehttp_X3A_X2F_X2F_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27url_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X27_X20required_X20placeholder_X3D_X27IP_X20or_X20domain_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2710_X27_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X2F_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E),
    /* K56  */  be_nested_str_weak(_X3Cbutton_X20class_X3D_X27button_X20bgrn_X27_X3EAuto_X2Dconfigure_X20remote_X20Tasmota_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Chr_X3E),
    /* K57  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20RESET_X20the_X20configuration_X20to_X20the_X20default_X2E_X20You_X20will_X20need_X20to_X20associate_X20again_X2E_X22_X29_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27auto_X27_X20class_X3D_X27button_X20bred_X27_X3EReset_X20all_X20and_X20Auto_X2Ddiscover_X3C_X2Fbutton_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Fform_X3E),
    /* K58  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    }),
    be_str_weak(show_plugins_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[311]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x8C100303,  //  0003  GETMET	R4	R1	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x7C100400,  //  0005  CALL	R4	2
      0x8C100303,  //  0006  GETMET	R4	R1	K3
      0x58180005,  //  0007  LDCONST	R6	K5
      0x7C100400,  //  0008  CALL	R4	2
      0x88100106,  //  0009  GETMBR	R4	R0	K6
      0x88100907,  //  000A  GETMBR	R4	R4	K7
      0x8C100908,  //  000B  GETMET	R4	R4	K8
      0x58180009,  //  000C  LDCONST	R6	K9
      0x7C100400,  //  000D  CALL	R4	2
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x8C10090A,  //  000F  GETMET	R4	R4	K10
      0x88180106,  //  0010  GETMBR	R6	R0	K6
      0x88180D07,  //  0011  GETMBR	R6	R6	K7
      0x7C100400,  //  0012  CALL	R4	2
      0x5814000B,  //  0013  LDCONST	R5	K11
      0x50180000,  //  0014  LDBOOL	R6	0	0
      0x601C000C,  //  0015  GETGBL	R7	G12
      0x5C200800,  //  0016  MOVE	R8	R4
      0x7C1C0200,  //  0017  CALL	R7	1
      0x141C0A07,  //  0018  LT	R7	R5	R7
      0x781E0047,  //  0019  JMPF	R7	#0062
      0x941C0805,  //  001A  GETIDX	R7	R4	R5
      0x88200106,  //  001B  GETMBR	R8	R0	K6
      0x88201107,  //  001C  GETMBR	R8	R8	K7
      0x8C20110C,  //  001D  GETMET	R8	R8	K12
      0x60280008,  //  001E  GETGBL	R10	G8
      0x5C2C0E00,  //  001F  MOVE	R11	R7
      0x7C280200,  //  0020  CALL	R10	1
      0x7C200400,  //  0021  CALL	R8	2
      0x8C24110C,  //  0022  GETMET	R9	R8	K12
      0x582C000D,  //  0023  LDCONST	R11	K13
      0x7C240400,  //  0024  CALL	R9	2
      0x5C281200,  //  0025  MOVE	R10	R9
      0x742A0001,  //  0026  JMPT	R10	#0029
      0x00140B0E,  //  0027  ADD	R5	R5	K14
      0x7001FFEB,  //  0028  JMP		#0015
      0x8C28050C,  //  0029  GETMET	R10	R2	K12
      0x5C301200,  //  002A  MOVE	R12	R9
      0x5834000F,  //  002B  LDCONST	R13	K15
      0x7C280600,  //  002C  CALL	R10	3
      0x1C28150B,  //  002D  EQ	R10	R10	K11
      0x782A0001,  //  002E  JMPF	R10	#0031
      0x00140B0E,  //  002F  ADD	R5	R5	K14
      0x7001FFE3,  //  0030  JMP		#0015
      0x88280106,  //  0031  GETMBR	R10	R0	K6
      0x88281510,  //  0032  GETMBR	R10	R10	K16
      0x8C28150C,  //  0033  GETMET	R10	R10	K12
      0x5C301200,  //  0034  MOVE	R12	R9
      0x7C280400,  //  0035  CALL	R10	2
      0x582C0011,  //  0036  LDCONST	R11	K17
      0x4C300000,  //  0037  LDNIL	R12
      0x2030140C,  //  0038  NE	R12	R10	R12
      0x78320004,  //  0039  JMPF	R12	#003F
      0x8C301512,  //  003A  GETMET	R12	R10	K18
      0x5C381400,  //  003B  MOVE	R14	R10
      0x5C3C1000,  //  003C  MOVE	R15	R8
      0x7C300600,  //  003D  CALL	R12	3
      0x5C2C1800,  //  003E  MOVE	R11	R12
      0x50180200,  //  003F  LDBOOL	R6	1	0
      0x8C300303,  //  0040  GETMET	R12	R1	K3
      0x8C380513,  //  0041  GETMET	R14	R2	K19
      0x58400014,  //  0042  LDCONST	R16	K20
      0x5C440E00,  //  0043  MOVE	R17	R7
      0x7C380600,  //  0044  CALL	R14	3
      0x7C300400,  //  0045  CALL	R12	2
      0x8C300303,  //  0046  GETMET	R12	R1	K3
      0x8C380513,  //  0047  GETMET	R14	R2	K19
      0x58400015,  //  0048  LDCONST	R16	K21
      0x8C440116,  //  0049  GETMET	R17	R0	K22
      0x8C4C110C,  //  004A  GETMET	R19	R8	K12
      0x5854000D,  //  004B  LDCONST	R21	K13
      0x58580011,  //  004C  LDCONST	R22	K17
      0x7C4C0600,  //  004D  CALL	R19	3
      0x7C440400,  //  004E  CALL	R17	2
      0x7C380600,  //  004F  CALL	R14	3
      0x7C300400,  //  0050  CALL	R12	2
      0x8C300303,  //  0051  GETMET	R12	R1	K3
      0x8C380513,  //  0052  GETMET	R14	R2	K19
      0x58400017,  //  0053  LDCONST	R16	K23
      0x5C440E00,  //  0054  MOVE	R17	R7
      0x8C480318,  //  0055  GETMET	R18	R1	K24
      0x5C501600,  //  0056  MOVE	R20	R11
      0x7C480400,  //  0057  CALL	R18	2
      0x7C380800,  //  0058  CALL	R14	4
      0x7C300400,  //  0059  CALL	R12	2
      0x8C300303,  //  005A  GETMET	R12	R1	K3
      0x8C380513,  //  005B  GETMET	R14	R2	K19
      0x58400019,  //  005C  LDCONST	R16	K25
      0x5C440E00,  //  005D  MOVE	R17	R7
      0x7C380600,  //  005E  CALL	R14	3
      0x7C300400,  //  005F  CALL	R12	2
      0x00140B0E,  //  0060  ADD	R5	R5	K14
      0x7001FFB2,  //  0061  JMP		#0015
      0x8C1C0303,  //  0062  GETMET	R7	R1	K3
      0x5824001A,  //  0063  LDCONST	R9	K26
      0x7C1C0400,  //  0064  CALL	R7	2
      0x5C1C0C00,  //  0065  MOVE	R7	R6
      0x741E0002,  //  0066  JMPT	R7	#006A
      0x8C1C0303,  //  0067  GETMET	R7	R1	K3
      0x5824001B,  //  0068  LDCONST	R9	K27
      0x7C1C0400,  //  0069  CALL	R7	2
      0x8C1C0303,  //  006A  GETMET	R7	R1	K3
      0x5824001C,  //  006B  LDCONST	R9	K28
      0x7C1C0400,  //  006C  CALL	R7	2
      0x601C0012,  //  006D  GETGBL	R7	G18
      0x7C1C0000,  //  006E  CALL	R7	0
      0x60200010,  //  006F  GETGBL	R8	G16
      0x88240106,  //  0070  GETMBR	R9	R0	K6
      0x88241307,  //  0071  GETMBR	R9	R9	K7
      0x7C200200,  //  0072  CALL	R8	1
      0xA802000B,  //  0073  EXBLK	0	#0080
      0x5C241000,  //  0074  MOVE	R9	R8
      0x7C240000,  //  0075  CALL	R9	0
      0x8C28130C,  //  0076  GETMET	R10	R9	K12
      0x5830001D,  //  0077  LDCONST	R12	K29
      0x7C280400,  //  0078  CALL	R10	2
      0x4C2C0000,  //  0079  LDNIL	R11
      0x202C140B,  //  007A  NE	R11	R10	R11
      0x782E0002,  //  007B  JMPF	R11	#007F
      0x8C2C0F1E,  //  007C  GETMET	R11	R7	K30
      0x5C341400,  //  007D  MOVE	R13	R10
      0x7C2C0400,  //  007E  CALL	R11	2
      0x7001FFF3,  //  007F  JMP		#0074
      0x5820001F,  //  0080  LDCONST	R8	K31
      0xAC200200,  //  0081  CATCH	R8	1	0
      0xB0080000,  //  0082  RAISE	2	R0	R0
      0x88200106,  //  0083  GETMBR	R8	R0	K6
      0x8C201120,  //  0084  GETMET	R8	R8	K32
      0x5C280E00,  //  0085  MOVE	R10	R7
      0x7C200400,  //  0086  CALL	R8	2
      0xB8224200,  //  0087  GETNGBL	R8	K33
      0x8C201122,  //  0088  GETMET	R8	R8	K34
      0x60280008,  //  0089  GETGBL	R10	G8
      0x5C2C0E00,  //  008A  MOVE	R11	R7
      0x7C280200,  //  008B  CALL	R10	1
      0x002A460A,  //  008C  ADD	R10	K35	R10
      0x582C0024,  //  008D  LDCONST	R11	K36
      0x7C200600,  //  008E  CALL	R8	3
      0x60200010,  //  008F  GETGBL	R8	G16
      0x5C240E00,  //  0090  MOVE	R9	R7
      0x7C200200,  //  0091  CALL	R8	1
      0xA802006C,  //  0092  EXBLK	0	#0100
      0x5C241000,  //  0093  MOVE	R9	R8
      0x7C240000,  //  0094  CALL	R9	0
      0x8C280303,  //  0095  GETMET	R10	R1	K3
      0x8C300513,  //  0096  GETMET	R12	R2	K19
      0x58380025,  //  0097  LDCONST	R14	K37
      0x8C3C0318,  //  0098  GETMET	R15	R1	K24
      0x5C441200,  //  0099  MOVE	R17	R9
      0x7C3C0400,  //  009A  CALL	R15	2
      0x8C400318,  //  009B  GETMET	R16	R1	K24
      0x5C481200,  //  009C  MOVE	R18	R9
      0x7C400400,  //  009D  CALL	R16	2
      0x7C300800,  //  009E  CALL	R12	4
      0x7C280400,  //  009F  CALL	R10	2
      0x8C280303,  //  00A0  GETMET	R10	R1	K3
      0x58300026,  //  00A1  LDCONST	R12	K38
      0x7C280400,  //  00A2  CALL	R10	2
      0x8C280303,  //  00A3  GETMET	R10	R1	K3
      0x58300027,  //  00A4  LDCONST	R12	K39
      0x7C280400,  //  00A5  CALL	R10	2
      0x50180000,  //  00A6  LDBOOL	R6	0	0
      0x5814000B,  //  00A7  LDCONST	R5	K11
      0x6028000C,  //  00A8  GETGBL	R10	G12
      0x5C2C0800,  //  00A9  MOVE	R11	R4
      0x7C280200,  //  00AA  CALL	R10	1
      0x14280A0A,  //  00AB  LT	R10	R5	R10
      0x782A004E,  //  00AC  JMPF	R10	#00FC
      0x94280805,  //  00AD  GETIDX	R10	R4	R5
      0x882C0106,  //  00AE  GETMBR	R11	R0	K6
      0x882C1707,  //  00AF  GETMBR	R11	R11	K7
      0x8C2C170C,  //  00B0  GETMET	R11	R11	K12
      0x60340008,  //  00B1  GETGBL	R13	G8
      0x5C381400,  //  00B2  MOVE	R14	R10
      0x7C340200,  //  00B3  CALL	R13	1
      0x7C2C0400,  //  00B4  CALL	R11	2
      0x8C30170C,  //  00B5  GETMET	R12	R11	K12
      0x5838000D,  //  00B6  LDCONST	R14	K13
      0x7C300400,  //  00B7  CALL	R12	2
      0x5C341800,  //  00B8  MOVE	R13	R12
      0x74360001,  //  00B9  JMPT	R13	#00BC
      0x00140B0E,  //  00BA  ADD	R5	R5	K14
      0x7001FFEB,  //  00BB  JMP		#00A8
      0x8C34050C,  //  00BC  GETMET	R13	R2	K12
      0x5C3C1800,  //  00BD  MOVE	R15	R12
      0x5840000F,  //  00BE  LDCONST	R16	K15
      0x7C340600,  //  00BF  CALL	R13	3
      0x20341B0B,  //  00C0  NE	R13	R13	K11
      0x78360001,  //  00C1  JMPF	R13	#00C4
      0x00140B0E,  //  00C2  ADD	R5	R5	K14
      0x7001FFE3,  //  00C3  JMP		#00A8
      0x8C34170C,  //  00C4  GETMET	R13	R11	K12
      0x583C001D,  //  00C5  LDCONST	R15	K29
      0x7C340400,  //  00C6  CALL	R13	2
      0x20341A09,  //  00C7  NE	R13	R13	R9
      0x78360001,  //  00C8  JMPF	R13	#00CB
      0x00140B0E,  //  00C9  ADD	R5	R5	K14
      0x7001FFDC,  //  00CA  JMP		#00A8
      0x88340106,  //  00CB  GETMBR	R13	R0	K6
      0x88341B10,  //  00CC  GETMBR	R13	R13	K16
      0x8C341B0C,  //  00CD  GETMET	R13	R13	K12
      0x5C3C1800,  //  00CE  MOVE	R15	R12
      0x7C340400,  //  00CF  CALL	R13	2
      0x58380011,  //  00D0  LDCONST	R14	K17
      0x4C3C0000,  //  00D1  LDNIL	R15
      0x203C1A0F,  //  00D2  NE	R15	R13	R15
      0x783E0004,  //  00D3  JMPF	R15	#00D9
      0x8C3C1B12,  //  00D4  GETMET	R15	R13	K18
      0x5C441A00,  //  00D5  MOVE	R17	R13
      0x5C481600,  //  00D6  MOVE	R18	R11
      0x7C3C0600,  //  00D7  CALL	R15	3
      0x5C381E00,  //  00D8  MOVE	R14	R15
      0x50180200,  //  00D9  LDBOOL	R6	1	0
      0x8C3C0303,  //  00DA  GETMET	R15	R1	K3
      0x8C440513,  //  00DB  GETMET	R17	R2	K19
      0x584C0028,  //  00DC  LDCONST	R19	K40
      0x5C501400,  //  00DD  MOVE	R20	R10
      0x7C440600,  //  00DE  CALL	R17	3
      0x7C3C0400,  //  00DF  CALL	R15	2
      0x8C3C0303,  //  00E0  GETMET	R15	R1	K3
      0x8C440513,  //  00E1  GETMET	R17	R2	K19
      0x584C0029,  //  00E2  LDCONST	R19	K41
      0x8C500116,  //  00E3  GETMET	R20	R0	K22
      0x8C58170C,  //  00E4  GETMET	R22	R11	K12
      0x5860000D,  //  00E5  LDCONST	R24	K13
      0x58640011,  //  00E6  LDCONST	R25	K17
      0x7C580600,  //  00E7  CALL	R22	3
      0x7C500400,  //  00E8  CALL	R20	2
      0x7C440600,  //  00E9  CALL	R17	3
      0x7C3C0400,  //  00EA  CALL	R15	2
      0x8C3C0303,  //  00EB  GETMET	R15	R1	K3
      0x8C440513,  //  00EC  GETMET	R17	R2	K19
      0x584C0017,  //  00ED  LDCONST	R19	K23
      0x5C501400,  //  00EE  MOVE	R20	R10
      0x8C540318,  //  00EF  GETMET	R21	R1	K24
      0x5C5C1C00,  //  00F0  MOVE	R23	R14
      0x7C540400,  //  00F1  CALL	R21	2
      0x7C440800,  //  00F2  CALL	R17	4
      0x7C3C0400,  //  00F3  CALL	R15	2
      0x8C3C0303,  //  00F4  GETMET	R15	R1	K3
      0x8C440513,  //  00F5  GETMET	R17	R2	K19
      0x584C002A,  //  00F6  LDCONST	R19	K42
      0x5C501400,  //  00F7  MOVE	R20	R10
      0x7C440600,  //  00F8  CALL	R17	3
      0x7C3C0400,  //  00F9  CALL	R15	2
      0x00140B0E,  //  00FA  ADD	R5	R5	K14
      0x7001FFAB,  //  00FB  JMP		#00A8
      0x8C280303,  //  00FC  GETMET	R10	R1	K3
      0x5830002B,  //  00FD  LDCONST	R12	K43
      0x7C280400,  //  00FE  CALL	R10	2
      0x7001FF92,  //  00FF  JMP		#0093
      0x5820001F,  //  0100  LDCONST	R8	K31
      0xAC200200,  //  0101  CATCH	R8	1	0
      0xB0080000,  //  0102  RAISE	2	R0	R0
      0x5C200C00,  //  0103  MOVE	R8	R6
      0x74220002,  //  0104  JMPT	R8	#0108
      0x8C200303,  //  0105  GETMET	R8	R1	K3
      0x5828001B,  //  0106  LDCONST	R10	K27
      0x7C200400,  //  0107  CALL	R8	2
      0x8C200303,  //  0108  GETMET	R8	R1	K3
      0x5828002C,  //  0109  LDCONST	R10	K44
      0x7C200400,  //  010A  CALL	R8	2
      0x8C200303,  //  010B  GETMET	R8	R1	K3
      0x5828002D,  //  010C  LDCONST	R10	K45
      0x7C200400,  //  010D  CALL	R8	2
      0x8C200303,  //  010E  GETMET	R8	R1	K3
      0x5828002E,  //  010F  LDCONST	R10	K46
      0x7C200400,  //  0110  CALL	R8	2
      0x8C200303,  //  0111  GETMET	R8	R1	K3
      0x5828002F,  //  0112  LDCONST	R10	K47
      0x7C200400,  //  0113  CALL	R8	2
      0x8C200130,  //  0114  GETMET	R8	R0	K48
      0x58280011,  //  0115  LDCONST	R10	K17
      0x882C0131,  //  0116  GETMBR	R11	R0	K49
      0x7C200600,  //  0117  CALL	R8	3
      0x8C200303,  //  0118  GETMET	R8	R1	K3
      0x58280032,  //  0119  LDCONST	R10	K50
      0x7C200400,  //  011A  CALL	R8	2
      0x8C200303,  //  011B  GETMET	R8	R1	K3
      0x58280033,  //  011C  LDCONST	R10	K51
      0x7C200400,  //  011D  CALL	R8	2
      0x8C200303,  //  011E  GETMET	R8	R1	K3
      0x58280034,  //  011F  LDCONST	R10	K52
      0x7C200400,  //  0120  CALL	R8	2
      0x8C200303,  //  0121  GETMET	R8	R1	K3
      0x58280035,  //  0122  LDCONST	R10	K53
      0x7C200400,  //  0123  CALL	R8	2
      0x8C200303,  //  0124  GETMET	R8	R1	K3
      0x58280036,  //  0125  LDCONST	R10	K54
      0x7C200400,  //  0126  CALL	R8	2
      0x8C200303,  //  0127  GETMET	R8	R1	K3
      0x58280037,  //  0128  LDCONST	R10	K55
      0x7C200400,  //  0129  CALL	R8	2
      0x8C200303,  //  012A  GETMET	R8	R1	K3
      0x58280034,  //  012B  LDCONST	R10	K52
      0x7C200400,  //  012C  CALL	R8	2
      0x8C200303,  //  012D  GETMET	R8	R1	K3
      0x58280038,  //  012E  LDCONST	R10	K56
      0x7C200400,  //  012F  CALL	R8	2
      0x8C200303,  //  0130  GETMET	R8	R1	K3
      0x58280039,  //  0131  LDCONST	R10	K57
      0x7C200400,  //  0132  CALL	R8	2
      0x8C200303,  //  0133  GETMET	R8	R1	K3
      0x5828003A,  //  0134  LDCONST	R10	K58
      0x7C200400,  //  0135  CALL	R8	2
      0x80000000,  //  0136  RET	0
    })
  )
);
/*******************************************************************/


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
** Solidified function: web_sensor
********************************************************************/
be_local_closure(Matter_UI_web_sensor,   /* name */
  be_nested_proto(
    12,                          /* nstack */
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
    /* K2   */  be_nested_str_weak(matter_enabled),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(sessions),
    /* K5   */  be_nested_str_weak(count_active_fabrics),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(content_send),
    /* K8   */  be_nested_str_weak(format),
    /* K9   */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27text_X2Dalign_X3Aright_X3Bfont_X2Dsize_X3A11px_X3Bcolor_X3A_X23aaa_X3Bpadding_X3A0px_X3B_X27_X3E_X25s_X3C_X2Fdiv_X3E),
    /* K10  */  be_nested_str_weak(Matter_X3A_X20No_X20active_X20association),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(Matter_X3A_X20),
    /* K13  */  be_nested_str_weak(_X20active_X20association),
    /* K14  */  be_nested_str_weak(s),
    /* K15  */  be_nested_str_weak(),
    /* K16  */  be_nested_str_weak(show_bridge_status),
    /* K17  */  be_nested_str_weak(is_root_commissioning_open),
    /* K18  */  be_nested_str_weak(show_commissioning_info),
    }),
    be_str_weak(web_sensor),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0102,  //  0002  GETMET	R3	R0	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x780E0024,  //  0004  JMPF	R3	#002A
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x880C0704,  //  0006  GETMBR	R3	R3	K4
      0x8C0C0705,  //  0007  GETMET	R3	R3	K5
      0x7C0C0200,  //  0008  CALL	R3	1
      0x1C100706,  //  0009  EQ	R4	R3	K6
      0x78120006,  //  000A  JMPF	R4	#0012
      0x8C100307,  //  000B  GETMET	R4	R1	K7
      0x8C180508,  //  000C  GETMET	R6	R2	K8
      0x58200009,  //  000D  LDCONST	R8	K9
      0x5824000A,  //  000E  LDCONST	R9	K10
      0x7C180600,  //  000F  CALL	R6	3
      0x7C100400,  //  0010  CALL	R4	2
      0x7002000F,  //  0011  JMP		#0022
      0x2410070B,  //  0012  GT	R4	R3	K11
      0x8C140307,  //  0013  GETMET	R5	R1	K7
      0x8C1C0508,  //  0014  GETMET	R7	R2	K8
      0x58240009,  //  0015  LDCONST	R9	K9
      0x60280008,  //  0016  GETGBL	R10	G8
      0x5C2C0600,  //  0017  MOVE	R11	R3
      0x7C280200,  //  0018  CALL	R10	1
      0x002A180A,  //  0019  ADD	R10	K12	R10
      0x0028150D,  //  001A  ADD	R10	R10	K13
      0x78120001,  //  001B  JMPF	R4	#001E
      0x582C000E,  //  001C  LDCONST	R11	K14
      0x70020000,  //  001D  JMP		#001F
      0x582C000F,  //  001E  LDCONST	R11	K15
      0x0028140B,  //  001F  ADD	R10	R10	R11
      0x7C1C0600,  //  0020  CALL	R7	3
      0x7C140400,  //  0021  CALL	R5	2
      0x8C100110,  //  0022  GETMET	R4	R0	K16
      0x7C100200,  //  0023  CALL	R4	1
      0x88100103,  //  0024  GETMBR	R4	R0	K3
      0x8C100911,  //  0025  GETMET	R4	R4	K17
      0x7C100200,  //  0026  CALL	R4	1
      0x78120001,  //  0027  JMPF	R4	#002A
      0x8C100112,  //  0028  GETMET	R4	R0	K18
      0x7C100200,  //  0029  CALL	R4	1
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_fabric_info
********************************************************************/
be_local_closure(Matter_UI_show_fabric_info,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BFabrics_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X3EAssociated_X20fabrics_X3A_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(sessions),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3ENone_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K8   */  be_nested_str_weak(fabrics),
    /* K9   */  be_nested_str_weak(persistables),
    /* K10  */  be_nested_str_weak(_X3Chr_X3E),
    /* K11  */  be_nested_str_weak(fabric_label),
    /* K12  */  be_nested_str_weak(_X3CNo_X20label_X3E),
    /* K13  */  be_nested_str_weak(html_escape),
    /* K14  */  be_nested_str_weak(format),
    /* K15  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3B_X23_X25i_X20_X25s_X3C_X2Fb_X3E_X20_X28_X25s_X29_X26nbsp_X3B_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K16  */  be_nested_str_weak(get_fabric_index),
    /* K17  */  be_nested_str_weak(get_admin_vendor_name),
    /* K18  */  be_nested_str_weak(get_fabric_id),
    /* K19  */  be_nested_str_weak(copy),
    /* K20  */  be_nested_str_weak(reverse),
    /* K21  */  be_nested_str_weak(get_device_id),
    /* K22  */  be_nested_str_weak(Fabric_X3A_X20_X25s_X3Cbr_X3E),
    /* K23  */  be_nested_str_weak(tohex),
    /* K24  */  be_nested_str_weak(Device_X3A_X20_X25s_X3Cbr_X3E_X26nbsp_X3B),
    /* K25  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22Are_X20you_X20sure_X3F_X22_X29_X3B_X27_X3E),
    /* K26  */  be_nested_str_weak(_X3Cinput_X20name_X3D_X27del_fabric_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K27  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27del_X27_X20class_X3D_X27button_X20bgrn_X27_X3EDelete_X20Fabric_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E),
    /* K28  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K29  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(show_fabric_info),
    &be_const_str_solidified,
    ( &(const binstruction[101]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100104,  //  0006  GETMBR	R4	R0	K4
      0x88100905,  //  0007  GETMBR	R4	R4	K5
      0x88100905,  //  0008  GETMBR	R4	R4	K5
      0x7C0C0200,  //  0009  CALL	R3	1
      0x1C0C0706,  //  000A  EQ	R3	R3	K6
      0x780E0003,  //  000B  JMPF	R3	#0010
      0x8C0C0302,  //  000C  GETMET	R3	R1	K2
      0x58140007,  //  000D  LDCONST	R5	K7
      0x7C0C0400,  //  000E  CALL	R3	2
      0x70020050,  //  000F  JMP		#0061
      0x500C0200,  //  0010  LDBOOL	R3	1	0
      0x60100010,  //  0011  GETGBL	R4	G16
      0x88140104,  //  0012  GETMBR	R5	R0	K4
      0x88140B05,  //  0013  GETMBR	R5	R5	K5
      0x88140B08,  //  0014  GETMBR	R5	R5	K8
      0x8C140B09,  //  0015  GETMET	R5	R5	K9
      0x7C140200,  //  0016  CALL	R5	1
      0x7C100200,  //  0017  CALL	R4	1
      0xA8020044,  //  0018  EXBLK	0	#005E
      0x5C140800,  //  0019  MOVE	R5	R4
      0x7C140000,  //  001A  CALL	R5	0
      0x5C180600,  //  001B  MOVE	R6	R3
      0x741A0002,  //  001C  JMPT	R6	#0020
      0x8C180302,  //  001D  GETMET	R6	R1	K2
      0x5820000A,  //  001E  LDCONST	R8	K10
      0x7C180400,  //  001F  CALL	R6	2
      0x500C0000,  //  0020  LDBOOL	R3	0	0
      0x88180B0B,  //  0021  GETMBR	R6	R5	K11
      0x5C1C0C00,  //  0022  MOVE	R7	R6
      0x741E0000,  //  0023  JMPT	R7	#0025
      0x5818000C,  //  0024  LDCONST	R6	K12
      0x8C1C030D,  //  0025  GETMET	R7	R1	K13
      0x5C240C00,  //  0026  MOVE	R9	R6
      0x7C1C0400,  //  0027  CALL	R7	2
      0x5C180E00,  //  0028  MOVE	R6	R7
      0x8C1C0302,  //  0029  GETMET	R7	R1	K2
      0x8C24050E,  //  002A  GETMET	R9	R2	K14
      0x582C000F,  //  002B  LDCONST	R11	K15
      0x8C300B10,  //  002C  GETMET	R12	R5	K16
      0x7C300200,  //  002D  CALL	R12	1
      0x5C340C00,  //  002E  MOVE	R13	R6
      0x8C380B11,  //  002F  GETMET	R14	R5	K17
      0x7C380200,  //  0030  CALL	R14	1
      0x7C240A00,  //  0031  CALL	R9	5
      0x7C1C0400,  //  0032  CALL	R7	2
      0x8C1C0B12,  //  0033  GETMET	R7	R5	K18
      0x7C1C0200,  //  0034  CALL	R7	1
      0x8C1C0F13,  //  0035  GETMET	R7	R7	K19
      0x7C1C0200,  //  0036  CALL	R7	1
      0x8C1C0F14,  //  0037  GETMET	R7	R7	K20
      0x7C1C0200,  //  0038  CALL	R7	1
      0x8C200B15,  //  0039  GETMET	R8	R5	K21
      0x7C200200,  //  003A  CALL	R8	1
      0x8C201113,  //  003B  GETMET	R8	R8	K19
      0x7C200200,  //  003C  CALL	R8	1
      0x8C201114,  //  003D  GETMET	R8	R8	K20
      0x7C200200,  //  003E  CALL	R8	1
      0x8C240302,  //  003F  GETMET	R9	R1	K2
      0x8C2C050E,  //  0040  GETMET	R11	R2	K14
      0x58340016,  //  0041  LDCONST	R13	K22
      0x8C380F17,  //  0042  GETMET	R14	R7	K23
      0x7C380200,  //  0043  CALL	R14	1
      0x7C2C0600,  //  0044  CALL	R11	3
      0x7C240400,  //  0045  CALL	R9	2
      0x8C240302,  //  0046  GETMET	R9	R1	K2
      0x8C2C050E,  //  0047  GETMET	R11	R2	K14
      0x58340018,  //  0048  LDCONST	R13	K24
      0x8C381117,  //  0049  GETMET	R14	R8	K23
      0x7C380200,  //  004A  CALL	R14	1
      0x7C2C0600,  //  004B  CALL	R11	3
      0x7C240400,  //  004C  CALL	R9	2
      0x8C240302,  //  004D  GETMET	R9	R1	K2
      0x582C0019,  //  004E  LDCONST	R11	K25
      0x7C240400,  //  004F  CALL	R9	2
      0x8C240302,  //  0050  GETMET	R9	R1	K2
      0x8C2C050E,  //  0051  GETMET	R11	R2	K14
      0x5834001A,  //  0052  LDCONST	R13	K26
      0x8C380B10,  //  0053  GETMET	R14	R5	K16
      0x7C380200,  //  0054  CALL	R14	1
      0x7C2C0600,  //  0055  CALL	R11	3
      0x7C240400,  //  0056  CALL	R9	2
      0x8C240302,  //  0057  GETMET	R9	R1	K2
      0x582C001B,  //  0058  LDCONST	R11	K27
      0x7C240400,  //  0059  CALL	R9	2
      0x8C240302,  //  005A  GETMET	R9	R1	K2
      0x582C001C,  //  005B  LDCONST	R11	K28
      0x7C240400,  //  005C  CALL	R9	2
      0x7001FFBA,  //  005D  JMP		#0019
      0x5810001D,  //  005E  LDCONST	R4	K29
      0xAC100200,  //  005F  CATCH	R4	1	0
      0xB0080000,  //  0060  RAISE	2	R0	R0
      0x8C0C0302,  //  0061  GETMET	R3	R1	K2
      0x5814001C,  //  0062  LDCONST	R5	K28
      0x7C0C0400,  //  0063  CALL	R3	2
      0x80000000,  //  0064  RET	0
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(check_privileged_access),
    /* K3   */  be_nested_str_weak(content_start),
    /* K4   */  be_nested_str_weak(Matter),
    /* K5   */  be_nested_str_weak(content_send_style),
    /* K6   */  be_nested_str_weak(show_enable),
    /* K7   */  be_nested_str_weak(matter_enabled),
    /* K8   */  be_nested_str_weak(show_plugins_configuration),
    /* K9   */  be_nested_str_weak(content_send),
    /* K10  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K11  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cform_X20id_X3D_X27butmat_X27_X20style_X3D_X27display_X3A_X20block_X3B_X27_X20action_X3D_X27mattera_X27_X20method_X3D_X27get_X27_X3E_X3Cbutton_X20name_X3D_X27_X27_X3EAdvanced_X20Configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K12  */  be_nested_str_weak(content_button),
    /* K13  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K14  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
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
      0x8C0C0107,  //  000E  GETMET	R3	R0	K7
      0x7C0C0200,  //  000F  CALL	R3	1
      0x780E0001,  //  0010  JMPF	R3	#0013
      0x8C0C0108,  //  0011  GETMET	R3	R0	K8
      0x7C0C0200,  //  0012  CALL	R3	1
      0x8C0C0309,  //  0013  GETMET	R3	R1	K9
      0x5814000A,  //  0014  LDCONST	R5	K10
      0x7C0C0400,  //  0015  CALL	R3	2
      0x8C0C0309,  //  0016  GETMET	R3	R1	K9
      0x5814000B,  //  0017  LDCONST	R5	K11
      0x7C0C0400,  //  0018  CALL	R3	2
      0x8C0C030C,  //  0019  GETMET	R3	R1	K12
      0x8814030D,  //  001A  GETMBR	R5	R1	K13
      0x7C0C0400,  //  001B  CALL	R3	2
      0x8C0C030E,  //  001C  GETMET	R3	R1	K14
      0x7C0C0200,  //  001D  CALL	R3	1
      0x80000000,  //  001E  RET	0
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
    be_nested_map(25,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(web_add_config_button, -1), be_const_closure(Matter_UI_web_add_config_button_closure) },
        { be_const_key_weak(page_part_mgr, -1), be_const_closure(Matter_UI_page_part_mgr_closure) },
        { be_const_key_weak(page_part_mgr_adv, -1), be_const_closure(Matter_UI_page_part_mgr_adv_closure) },
        { be_const_key_weak(show_bridge_status, -1), be_const_closure(Matter_UI_show_bridge_status_closure) },
        { be_const_key_weak(plugin_option, -1), be_const_closure(Matter_UI_plugin_option_closure) },
        { be_const_key_weak(init, 8), be_const_closure(Matter_UI_init_closure) },
        { be_const_key_weak(page_part_ctl, 7), be_const_closure(Matter_UI_page_part_ctl_closure) },
        { be_const_key_weak(show_fabric_info, 9), be_const_closure(Matter_UI_show_fabric_info_closure) },
        { be_const_key_weak(web_get_arg, -1), be_const_closure(Matter_UI_web_get_arg_closure) },
        { be_const_key_weak(equal_map, -1), be_const_static_closure(Matter_UI_equal_map_closure) },
        { be_const_key_weak(page_part_mgr_add, -1), be_const_closure(Matter_UI_page_part_mgr_add_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(web_add_handler, 21), be_const_closure(Matter_UI_web_add_handler_closure) },
        { be_const_key_weak(matter_enabled, -1), be_const_closure(Matter_UI_matter_enabled_closure) },
        { be_const_key_weak(show_enable, -1), be_const_closure(Matter_UI_show_enable_closure) },
        { be_const_key_weak(show_commissioning_info, -1), be_const_closure(Matter_UI_show_commissioning_info_closure) },
        { be_const_key_weak(_CLASSES_TYPES2, 18), be_nested_str_weak(_X7Chttp_relay_X7Chttp_light0_X7Chttp_light1_X7Chttp_light2_X7Chttp_light3_X7Chttp_temperature_X7Chttp_pressure_X7Chttp_illuminance_X7Chttp_humidity_X7Chttp_occupancy) },
        { be_const_key_weak(show_remote_autoconf, 13), be_const_closure(Matter_UI_show_remote_autoconf_closure) },
        { be_const_key_weak(show_plugins_configuration, 23), be_const_closure(Matter_UI_show_plugins_configuration_closure) },
        { be_const_key_weak(show_passcode_form, 16), be_const_closure(Matter_UI_show_passcode_form_closure) },
        { be_const_key_weak(_CLASSES_TYPES, -1), be_nested_str_weak(_X7Crelay_X7Clight0_X7Clight1_X7Clight2_X7Clight3_X7Cshutter_X7Cshutter_X2Btilt_X7Ctemperature_X7Cpressure_X7Cilluminance_X7Chumidity_X7Coccupancy_X7Ccontact__X7Conoff) },
        { be_const_key_weak(plugin_name, -1), be_const_closure(Matter_UI_plugin_name_closure) },
        { be_const_key_weak(web_sensor, -1), be_const_closure(Matter_UI_web_sensor_closure) },
        { be_const_key_weak(show_qrcode, -1), be_const_closure(Matter_UI_show_qrcode_closure) },
        { be_const_key_weak(generate_config_from_status, 1), be_const_closure(Matter_UI_generate_config_from_status_closure) },
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
/* Solidification of Matter_UI.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_UI;

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
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Advanced_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20cause_X20a_X20restart_X2E_X22_X29_X3B_X27_X3E_X3Cp_X3EPasscode_X3A_X3C_X2Fp_X3E),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X271_X27_X20max_X3D_X2799999998_X27_X20name_X3D_X27passcode_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K7   */  be_nested_str_weak(device),
    /* K8   */  be_nested_str_weak(root_passcode),
    /* K9   */  be_nested_str_weak(_X3Cp_X3EDistinguish_X20id_X3A_X3C_X2Fp_X3E),
    /* K10  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X270_X27_X20max_X3D_X274095_X27_X20name_X3D_X27discriminator_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K11  */  be_nested_str_weak(root_discriminator),
    /* K12  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20type_X3D_X27checkbox_X27_X20name_X3D_X27ipv4_X27_X25s_X3EIPv4_X20only_X3C_X2Fp_X3E),
    /* K13  */  be_nested_str_weak(ipv4only),
    /* K14  */  be_nested_str_weak(_X20checked),
    /* K15  */  be_nested_str_weak(),
    /* K16  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27passcode_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_passcode_form),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C0C0302,  //  0005  GETMET	R3	R1	K2
      0x58140004,  //  0006  LDCONST	R5	K4
      0x7C0C0400,  //  0007  CALL	R3	2
      0x8C0C0302,  //  0008  GETMET	R3	R1	K2
      0x8C140505,  //  0009  GETMET	R5	R2	K5
      0x581C0006,  //  000A  LDCONST	R7	K6
      0x88200107,  //  000B  GETMBR	R8	R0	K7
      0x88201108,  //  000C  GETMBR	R8	R8	K8
      0x7C140600,  //  000D  CALL	R5	3
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C0C0302,  //  000F  GETMET	R3	R1	K2
      0x58140009,  //  0010  LDCONST	R5	K9
      0x7C0C0400,  //  0011  CALL	R3	2
      0x8C0C0302,  //  0012  GETMET	R3	R1	K2
      0x8C140505,  //  0013  GETMET	R5	R2	K5
      0x581C000A,  //  0014  LDCONST	R7	K10
      0x88200107,  //  0015  GETMBR	R8	R0	K7
      0x8820110B,  //  0016  GETMBR	R8	R8	K11
      0x7C140600,  //  0017  CALL	R5	3
      0x7C0C0400,  //  0018  CALL	R3	2
      0x8C0C0302,  //  0019  GETMET	R3	R1	K2
      0x8C140505,  //  001A  GETMET	R5	R2	K5
      0x581C000C,  //  001B  LDCONST	R7	K12
      0x88200107,  //  001C  GETMBR	R8	R0	K7
      0x8820110D,  //  001D  GETMBR	R8	R8	K13
      0x78220001,  //  001E  JMPF	R8	#0021
      0x5820000E,  //  001F  LDCONST	R8	K14
      0x70020000,  //  0020  JMP		#0022
      0x5820000F,  //  0021  LDCONST	R8	K15
      0x7C140600,  //  0022  CALL	R5	3
      0x7C0C0400,  //  0023  CALL	R3	2
      0x8C0C0302,  //  0024  GETMET	R3	R1	K2
      0x58140010,  //  0025  LDCONST	R5	K16
      0x7C0C0400,  //  0026  CALL	R3	2
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_mgr_adv
********************************************************************/
be_local_closure(Matter_UI_page_part_mgr_adv,   /* name */
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
    /* K4   */  be_nested_str_weak(Matter_X20Advanced_X20Configuration),
    /* K5   */  be_nested_str_weak(content_send_style),
    /* K6   */  be_nested_str_weak(matter_enabled),
    /* K7   */  be_nested_str_weak(show_passcode_form),
    /* K8   */  be_nested_str_weak(show_fabric_info),
    /* K9   */  be_nested_str_weak(content_button),
    /* K10  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K11  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr_adv),
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
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(plugins),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(Plugin_Bridge_HTTP),
    /* K7   */  be_nested_str_weak(http_remote),
    /* K8   */  be_nested_str_weak(addr),
    /* K9   */  be_nested_str_weak(contains),
    /* K10  */  be_nested_str_weak(push),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(content_send),
    /* K13  */  be_nested_str_weak(_X3Chr_X3E),
    /* K14  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K15  */  be_nested_str_weak(_STYLESHEET),
    /* K16  */  be_nested_str_weak(keys),
    /* K17  */  be_nested_str_weak(format),
    /* K18  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27ztdm_X20htrm_X27_X3E_X3Ctd_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Ftd_X3E),
    /* K19  */  be_nested_str_weak(html_escape),
    /* K20  */  be_nested_str_weak(web_last_seen),
    /* K21  */  be_nested_str_weak(_X3Ctr_X20class_X3D_X27htrm_X27_X3E_X3Ctd_X20colspan_X3D_X272_X27_X3E),
    /* K22  */  be_nested_str_weak(web_values),
    /* K23  */  be_nested_str_weak(_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K24  */  be_nested_str_weak(stop_iteration),
    /* K25  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Chr_X3E),
    }),
    be_str_weak(show_bridge_status),
    &be_const_str_solidified,
    ( &(const binstruction[102]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x58100002,  //  0003  LDCONST	R4	K2
      0x6014000C,  //  0004  GETGBL	R5	G12
      0x88180103,  //  0005  GETMBR	R6	R0	K3
      0x88180D04,  //  0006  GETMBR	R6	R6	K4
      0x7C140200,  //  0007  CALL	R5	1
      0x14140805,  //  0008  LT	R5	R4	R5
      0x7816001D,  //  0009  JMPF	R5	#0028
      0x88140103,  //  000A  GETMBR	R5	R0	K3
      0x88140B04,  //  000B  GETMBR	R5	R5	K4
      0x94140A04,  //  000C  GETIDX	R5	R5	R4
      0x6018000F,  //  000D  GETGBL	R6	G15
      0x5C1C0A00,  //  000E  MOVE	R7	R5
      0xB8220A00,  //  000F  GETNGBL	R8	K5
      0x88201106,  //  0010  GETMBR	R8	R8	K6
      0x7C180400,  //  0011  CALL	R6	2
      0x781A0012,  //  0012  JMPF	R6	#0026
      0x4C180000,  //  0013  LDNIL	R6
      0x1C180606,  //  0014  EQ	R6	R3	R6
      0x781A0002,  //  0015  JMPF	R6	#0019
      0x60180013,  //  0016  GETGBL	R6	G19
      0x7C180000,  //  0017  CALL	R6	0
      0x5C0C0C00,  //  0018  MOVE	R3	R6
      0x88180B07,  //  0019  GETMBR	R6	R5	K7
      0x88180D08,  //  001A  GETMBR	R6	R6	K8
      0x8C1C0709,  //  001B  GETMET	R7	R3	K9
      0x5C240C00,  //  001C  MOVE	R9	R6
      0x7C1C0400,  //  001D  CALL	R7	2
      0x741E0002,  //  001E  JMPT	R7	#0022
      0x601C0012,  //  001F  GETGBL	R7	G18
      0x7C1C0000,  //  0020  CALL	R7	0
      0x980C0C07,  //  0021  SETIDX	R3	R6	R7
      0x941C0606,  //  0022  GETIDX	R7	R3	R6
      0x8C1C0F0A,  //  0023  GETMET	R7	R7	K10
      0x5C240A00,  //  0024  MOVE	R9	R5
      0x7C1C0400,  //  0025  CALL	R7	2
      0x0010090B,  //  0026  ADD	R4	R4	K11
      0x7001FFDB,  //  0027  JMP		#0004
      0x4C140000,  //  0028  LDNIL	R5
      0x1C140605,  //  0029  EQ	R5	R3	R5
      0x78160000,  //  002A  JMPF	R5	#002C
      0x80000A00,  //  002B  RET	0
      0x8C14030C,  //  002C  GETMET	R5	R1	K12
      0x581C000D,  //  002D  LDCONST	R7	K13
      0x7C140400,  //  002E  CALL	R5	2
      0x8C14030C,  //  002F  GETMET	R5	R1	K12
      0x581C000E,  //  0030  LDCONST	R7	K14
      0x7C140400,  //  0031  CALL	R5	2
      0x8C14030C,  //  0032  GETMET	R5	R1	K12
      0xB81E0A00,  //  0033  GETNGBL	R7	K5
      0x881C0F0F,  //  0034  GETMBR	R7	R7	K15
      0x7C140400,  //  0035  CALL	R5	2
      0x60140010,  //  0036  GETGBL	R5	G16
      0x8C180710,  //  0037  GETMET	R6	R3	K16
      0x7C180200,  //  0038  CALL	R6	1
      0x7C140200,  //  0039  CALL	R5	1
      0xA8020023,  //  003A  EXBLK	0	#005F
      0x5C180A00,  //  003B  MOVE	R6	R5
      0x7C180000,  //  003C  CALL	R6	0
      0x8C1C030C,  //  003D  GETMET	R7	R1	K12
      0x8C240511,  //  003E  GETMET	R9	R2	K17
      0x582C0012,  //  003F  LDCONST	R11	K18
      0x8C300313,  //  0040  GETMET	R12	R1	K19
      0x5C380C00,  //  0041  MOVE	R14	R6
      0x7C300400,  //  0042  CALL	R12	2
      0x7C240600,  //  0043  CALL	R9	3
      0x7C1C0400,  //  0044  CALL	R7	2
      0x941C0606,  //  0045  GETIDX	R7	R3	R6
      0x941C0F02,  //  0046  GETIDX	R7	R7	K2
      0x881C0F07,  //  0047  GETMBR	R7	R7	K7
      0x8C20030C,  //  0048  GETMET	R8	R1	K12
      0x8C280F14,  //  0049  GETMET	R10	R7	K20
      0x7C280200,  //  004A  CALL	R10	1
      0x7C200400,  //  004B  CALL	R8	2
      0x60200010,  //  004C  GETGBL	R8	G16
      0x94240606,  //  004D  GETIDX	R9	R3	R6
      0x7C200200,  //  004E  CALL	R8	1
      0xA802000A,  //  004F  EXBLK	0	#005B
      0x5C241000,  //  0050  MOVE	R9	R8
      0x7C240000,  //  0051  CALL	R9	0
      0x8C28030C,  //  0052  GETMET	R10	R1	K12
      0x58300015,  //  0053  LDCONST	R12	K21
      0x7C280400,  //  0054  CALL	R10	2
      0x8C281316,  //  0055  GETMET	R10	R9	K22
      0x7C280200,  //  0056  CALL	R10	1
      0x8C28030C,  //  0057  GETMET	R10	R1	K12
      0x58300017,  //  0058  LDCONST	R12	K23
      0x7C280400,  //  0059  CALL	R10	2
      0x7001FFF4,  //  005A  JMP		#0050
      0x58200018,  //  005B  LDCONST	R8	K24
      0xAC200200,  //  005C  CATCH	R8	1	0
      0xB0080000,  //  005D  RAISE	2	R0	R0
      0x7001FFDB,  //  005E  JMP		#003B
      0x58140018,  //  005F  LDCONST	R5	K24
      0xAC140200,  //  0060  CATCH	R5	1	0
      0xB0080000,  //  0061  RAISE	2	R0	R0
      0x8C14030C,  //  0062  GETMET	R5	R1	K12
      0x581C0019,  //  0063  LDCONST	R7	K25
      0x7C140400,  //  0064  CALL	R5	2
      0x80000000,  //  0065  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: plugin_option
********************************************************************/
be_local_closure(Matter_UI_plugin_option,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    3,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
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
    /* K13  */  be_nested_str_weak(format),
    /* K14  */  be_nested_str_weak(_X3Coption_X20value_X3D_X27_X25s_X27_X25s_X3E_X25s_X3C_X2Foption_X3E),
    /* K15  */  be_nested_str_weak(_X20selected),
    /* K16  */  be_const_int(1),
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
      0x8C2C090D,  //  002B  GETMET	R11	R4	K13
      0x5834000E,  //  002C  LDCONST	R13	K14
      0x5C380E00,  //  002D  MOVE	R14	R7
      0x1C3C0E01,  //  002E  EQ	R15	R7	R1
      0x783E0001,  //  002F  JMPF	R15	#0032
      0x583C000F,  //  0030  LDCONST	R15	K15
      0x70020000,  //  0031  JMP		#0033
      0x583C0006,  //  0032  LDCONST	R15	K6
      0x5C401000,  //  0033  MOVE	R16	R8
      0x7C2C0A00,  //  0034  CALL	R11	5
      0x7C240400,  //  0035  CALL	R9	2
      0x00180D10,  //  0036  ADD	R6	R6	K16
      0x7001FFDB,  //  0037  JMP		#0014
      0x80000000,  //  0038  RET	0
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
** Solidified function: page_part_ctl
********************************************************************/
be_local_closure(Matter_UI_page_part_ctl,   /* name */
  be_nested_proto(
    25,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[97]) {     /* constants */
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
    /* K10  */  be_nested_str_weak(format),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20Arg_X25i_X20_X27_X25s_X27_X20_X3D_X20_X27_X25s_X27),
    /* K12  */  be_nested_str_weak(arg_name),
    /* K13  */  be_nested_str_weak(arg),
    /* K14  */  be_nested_str_weak(stop_iteration),
    /* K15  */  be_nested_str_weak(has_arg),
    /* K16  */  be_nested_str_weak(passcode),
    /* K17  */  be_nested_str_weak(discriminator),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20_X2Fmatterc_X20received_X20_X27_X25s_X27_X20command),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(device),
    /* K21  */  be_nested_str_weak(root_passcode),
    /* K22  */  be_nested_str_weak(root_discriminator),
    /* K23  */  be_nested_str_weak(ipv4only),
    /* K24  */  be_nested_str_weak(ipv4),
    /* K25  */  be_nested_str_weak(on),
    /* K26  */  be_nested_str_weak(save_param),
    /* K27  */  be_nested_str_weak(redirect),
    /* K28  */  be_nested_str_weak(_X2F_X3Frst_X3D),
    /* K29  */  be_nested_str_weak(save),
    /* K30  */  be_nested_str_weak(menable),
    /* K31  */  be_nested_str_weak(comm),
    /* K32  */  be_nested_str_weak(matter_enabled),
    /* K33  */  be_nested_str_weak(enable),
    /* K34  */  be_nested_str_weak(cmd),
    /* K35  */  be_nested_str_weak(SetOption),
    /* K36  */  be_nested_str_weak(matter),
    /* K37  */  be_nested_str_weak(MATTER_OPTION),
    /* K38  */  be_nested_str_weak(_X201),
    /* K39  */  be_nested_str_weak(disable),
    /* K40  */  be_nested_str_weak(_X200),
    /* K41  */  be_nested_str_weak(commissioning_open),
    /* K42  */  be_nested_str_weak(start_root_basic_commissioning),
    /* K43  */  be_nested_str_weak(stop_basic_commissioning),
    /* K44  */  be_nested_str_weak(_X2F),
    /* K45  */  be_nested_str_weak(del_fabric),
    /* K46  */  be_nested_str_weak(sessions),
    /* K47  */  be_nested_str_weak(fabrics),
    /* K48  */  be_nested_str_weak(get_fabric_index),
    /* K49  */  be_nested_str_weak(remove_fabric),
    /* K50  */  be_nested_str_weak(_X2Fmatterc_X3F),
    /* K51  */  be_nested_str_weak(auto),
    /* K52  */  be_nested_str_weak(plugins_persist),
    /* K53  */  be_nested_str_weak(config),
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
    /* K69  */  be_nested_str_weak(MTR_X3A_X20config_X20_X3D_X20_X25s),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20config_X20error_X20_X3D_X20_X25s),
    /* K71  */  be_nested_str_weak(_X2Fcn_X3F),
    /* K72  */  be_nested_str_weak(addep),
    /* K73  */  be_nested_str_weak(pi),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20add_X20endpoint_X20typ_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K75  */  be_nested_str_weak(bridge_add_endpoint),
    /* K76  */  be_nested_str_weak(addrem),
    /* K77  */  be_nested_str_weak(url),
    /* K78  */  be_nested_str_weak(value_error),
    /* K79  */  be_nested_str_weak(url_X20shouldn_X27t_X20be_X20null),
    /* K80  */  be_nested_str_weak(equal_map),
    /* K81  */  be_nested_str_weak(MTR_X3A_X20remote_X20add_X20url_X3D_X27_X25s_X27_X20type_X3D_X27_X25s_X27_X20arg_X3D_X27_X25s_X27),
    /* K82  */  be_nested_str_weak(del),
    /* K83  */  be_nested_str_weak(MTR_X3A_X20deleting_X20endpoint_X20_X25i),
    /* K84  */  be_const_int(2),
    /* K85  */  be_nested_str_weak(bridge_remove_endpoint),
    /* K86  */  be_nested_str_weak(content_start),
    /* K87  */  be_nested_str_weak(Parameter_X20error),
    /* K88  */  be_nested_str_weak(content_send_style),
    /* K89  */  be_nested_str_weak(content_send),
    /* K90  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EError_X3A_X3C_X2Fb_X3E_X25s_X3C_X2Fp_X3E),
    /* K91  */  be_nested_str_weak(html_escape),
    /* K92  */  be_nested_str_weak(content_button),
    /* K93  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K94  */  be_nested_str_weak(content_stop),
    /* K95  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K96  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
    }),
    be_str_weak(page_part_ctl),
    &be_const_str_solidified,
    ( &(const binstruction[628]) {  /* code */
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
      0xA8020249,  //  000A  EXBLK	0	#0255
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
      0x8C28050A,  //  0016  GETMET	R10	R2	K10
      0x5830000B,  //  0017  LDCONST	R12	K11
      0x5C340E00,  //  0018  MOVE	R13	R7
      0x8C38030C,  //  0019  GETMET	R14	R1	K12
      0x5C400E00,  //  001A  MOVE	R16	R7
      0x7C380400,  //  001B  CALL	R14	2
      0x8C3C030D,  //  001C  GETMET	R15	R1	K13
      0x5C440E00,  //  001D  MOVE	R17	R7
      0x7C3C0400,  //  001E  CALL	R15	2
      0x7C280A00,  //  001F  CALL	R10	5
      0x7C200400,  //  0020  CALL	R8	2
      0x7001FFEF,  //  0021  JMP		#0012
      0x5818000E,  //  0022  LDCONST	R6	K14
      0xAC180200,  //  0023  CATCH	R6	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x8C18030F,  //  0025  GETMET	R6	R1	K15
      0x58200010,  //  0026  LDCONST	R8	K16
      0x7C180400,  //  0027  CALL	R6	2
      0x741A0003,  //  0028  JMPT	R6	#002D
      0x8C18030F,  //  0029  GETMET	R6	R1	K15
      0x58200011,  //  002A  LDCONST	R8	K17
      0x7C180400,  //  002B  CALL	R6	2
      0x781A002A,  //  002C  JMPF	R6	#0058
      0xB81A1000,  //  002D  GETNGBL	R6	K8
      0x8C180D09,  //  002E  GETMET	R6	R6	K9
      0x8C20050A,  //  002F  GETMET	R8	R2	K10
      0x58280012,  //  0030  LDCONST	R10	K18
      0x582C0010,  //  0031  LDCONST	R11	K16
      0x7C200600,  //  0032  CALL	R8	3
      0x58240013,  //  0033  LDCONST	R9	K19
      0x7C180600,  //  0034  CALL	R6	3
      0x8C18030F,  //  0035  GETMET	R6	R1	K15
      0x58200010,  //  0036  LDCONST	R8	K16
      0x7C180400,  //  0037  CALL	R6	2
      0x781A0006,  //  0038  JMPF	R6	#0040
      0x88180114,  //  0039  GETMBR	R6	R0	K20
      0x601C0009,  //  003A  GETGBL	R7	G9
      0x8C20030D,  //  003B  GETMET	R8	R1	K13
      0x58280010,  //  003C  LDCONST	R10	K16
      0x7C200400,  //  003D  CALL	R8	2
      0x7C1C0200,  //  003E  CALL	R7	1
      0x901A2A07,  //  003F  SETMBR	R6	K21	R7
      0x8C18030F,  //  0040  GETMET	R6	R1	K15
      0x58200011,  //  0041  LDCONST	R8	K17
      0x7C180400,  //  0042  CALL	R6	2
      0x781A0006,  //  0043  JMPF	R6	#004B
      0x88180114,  //  0044  GETMBR	R6	R0	K20
      0x601C0009,  //  0045  GETGBL	R7	G9
      0x8C20030D,  //  0046  GETMET	R8	R1	K13
      0x58280011,  //  0047  LDCONST	R10	K17
      0x7C200400,  //  0048  CALL	R8	2
      0x7C1C0200,  //  0049  CALL	R7	1
      0x901A2C07,  //  004A  SETMBR	R6	K22	R7
      0x88180114,  //  004B  GETMBR	R6	R0	K20
      0x8C1C030D,  //  004C  GETMET	R7	R1	K13
      0x58240018,  //  004D  LDCONST	R9	K24
      0x7C1C0400,  //  004E  CALL	R7	2
      0x1C1C0F19,  //  004F  EQ	R7	R7	K25
      0x901A2E07,  //  0050  SETMBR	R6	K23	R7
      0x88180114,  //  0051  GETMBR	R6	R0	K20
      0x8C180D1A,  //  0052  GETMET	R6	R6	K26
      0x7C180200,  //  0053  CALL	R6	1
      0x8C18031B,  //  0054  GETMET	R6	R1	K27
      0x5820001C,  //  0055  LDCONST	R8	K28
      0x7C180400,  //  0056  CALL	R6	2
      0x700201E7,  //  0057  JMP		#0240
      0x8C18030F,  //  0058  GETMET	R6	R1	K15
      0x5820001D,  //  0059  LDCONST	R8	K29
      0x7C180400,  //  005A  CALL	R6	2
      0x781A0047,  //  005B  JMPF	R6	#00A4
      0x8C18030F,  //  005C  GETMET	R6	R1	K15
      0x5820001E,  //  005D  LDCONST	R8	K30
      0x7C180400,  //  005E  CALL	R6	2
      0x8C1C030F,  //  005F  GETMET	R7	R1	K15
      0x5824001F,  //  0060  LDCONST	R9	K31
      0x7C1C0400,  //  0061  CALL	R7	2
      0x8C200120,  //  0062  GETMET	R8	R0	K32
      0x7C200200,  //  0063  CALL	R8	1
      0x20200C08,  //  0064  NE	R8	R6	R8
      0x78220027,  //  0065  JMPF	R8	#008E
      0x781A0011,  //  0066  JMPF	R6	#0079
      0xB8221000,  //  0067  GETNGBL	R8	K8
      0x8C201109,  //  0068  GETMET	R8	R8	K9
      0x8C28050A,  //  0069  GETMET	R10	R2	K10
      0x58300012,  //  006A  LDCONST	R12	K18
      0x58340021,  //  006B  LDCONST	R13	K33
      0x7C280600,  //  006C  CALL	R10	3
      0x582C0013,  //  006D  LDCONST	R11	K19
      0x7C200600,  //  006E  CALL	R8	3
      0xB8221000,  //  006F  GETNGBL	R8	K8
      0x8C201122,  //  0070  GETMET	R8	R8	K34
      0x60280008,  //  0071  GETGBL	R10	G8
      0xB82E4800,  //  0072  GETNGBL	R11	K36
      0x882C1725,  //  0073  GETMBR	R11	R11	K37
      0x7C280200,  //  0074  CALL	R10	1
      0x002A460A,  //  0075  ADD	R10	K35	R10
      0x00281526,  //  0076  ADD	R10	R10	K38
      0x7C200400,  //  0077  CALL	R8	2
      0x70020010,  //  0078  JMP		#008A
      0xB8221000,  //  0079  GETNGBL	R8	K8
      0x8C201109,  //  007A  GETMET	R8	R8	K9
      0x8C28050A,  //  007B  GETMET	R10	R2	K10
      0x58300012,  //  007C  LDCONST	R12	K18
      0x58340027,  //  007D  LDCONST	R13	K39
      0x7C280600,  //  007E  CALL	R10	3
      0x582C0013,  //  007F  LDCONST	R11	K19
      0x7C200600,  //  0080  CALL	R8	3
      0xB8221000,  //  0081  GETNGBL	R8	K8
      0x8C201122,  //  0082  GETMET	R8	R8	K34
      0x60280008,  //  0083  GETGBL	R10	G8
      0xB82E4800,  //  0084  GETNGBL	R11	K36
      0x882C1725,  //  0085  GETMBR	R11	R11	K37
      0x7C280200,  //  0086  CALL	R10	1
      0x002A460A,  //  0087  ADD	R10	K35	R10
      0x00281528,  //  0088  ADD	R10	R10	K40
      0x7C200400,  //  0089  CALL	R8	2
      0x8C20031B,  //  008A  GETMET	R8	R1	K27
      0x5828001C,  //  008B  LDCONST	R10	K28
      0x7C200400,  //  008C  CALL	R8	2
      0x70020014,  //  008D  JMP		#00A3
      0x88200114,  //  008E  GETMBR	R8	R0	K20
      0x88201129,  //  008F  GETMBR	R8	R8	K41
      0x4C240000,  //  0090  LDNIL	R9
      0x20201009,  //  0091  NE	R8	R8	R9
      0x20200E08,  //  0092  NE	R8	R7	R8
      0x7822000B,  //  0093  JMPF	R8	#00A0
      0x781E0003,  //  0094  JMPF	R7	#0099
      0x88200114,  //  0095  GETMBR	R8	R0	K20
      0x8C20112A,  //  0096  GETMET	R8	R8	K42
      0x7C200200,  //  0097  CALL	R8	1
      0x70020002,  //  0098  JMP		#009C
      0x88200114,  //  0099  GETMBR	R8	R0	K20
      0x8C20112B,  //  009A  GETMET	R8	R8	K43
      0x7C200200,  //  009B  CALL	R8	1
      0x8C20031B,  //  009C  GETMET	R8	R1	K27
      0x5828002C,  //  009D  LDCONST	R10	K44
      0x7C200400,  //  009E  CALL	R8	2
      0x70020002,  //  009F  JMP		#00A3
      0x8C20031B,  //  00A0  GETMET	R8	R1	K27
      0x5828002C,  //  00A1  LDCONST	R10	K44
      0x7C200400,  //  00A2  CALL	R8	2
      0x7002019B,  //  00A3  JMP		#0240
      0x8C18030F,  //  00A4  GETMET	R6	R1	K15
      0x5820002D,  //  00A5  LDCONST	R8	K45
      0x7C180400,  //  00A6  CALL	R6	2
      0x781A0026,  //  00A7  JMPF	R6	#00CF
      0xB81A1000,  //  00A8  GETNGBL	R6	K8
      0x8C180D09,  //  00A9  GETMET	R6	R6	K9
      0x8C20050A,  //  00AA  GETMET	R8	R2	K10
      0x58280012,  //  00AB  LDCONST	R10	K18
      0x582C002D,  //  00AC  LDCONST	R11	K45
      0x7C200600,  //  00AD  CALL	R8	3
      0x58240013,  //  00AE  LDCONST	R9	K19
      0x7C180600,  //  00AF  CALL	R6	3
      0x60180009,  //  00B0  GETGBL	R6	G9
      0x8C1C030D,  //  00B1  GETMET	R7	R1	K13
      0x5824002D,  //  00B2  LDCONST	R9	K45
      0x7C1C0400,  //  00B3  CALL	R7	2
      0x7C180200,  //  00B4  CALL	R6	1
      0x581C0005,  //  00B5  LDCONST	R7	K5
      0x88200114,  //  00B6  GETMBR	R8	R0	K20
      0x8820112E,  //  00B7  GETMBR	R8	R8	K46
      0x8820112F,  //  00B8  GETMBR	R8	R8	K47
      0x6024000C,  //  00B9  GETGBL	R9	G12
      0x5C281000,  //  00BA  MOVE	R10	R8
      0x7C240200,  //  00BB  CALL	R9	1
      0x14240E09,  //  00BC  LT	R9	R7	R9
      0x7826000C,  //  00BD  JMPF	R9	#00CB
      0x94241007,  //  00BE  GETIDX	R9	R8	R7
      0x8C241330,  //  00BF  GETMET	R9	R9	K48
      0x7C240200,  //  00C0  CALL	R9	1
      0x1C241206,  //  00C1  EQ	R9	R9	R6
      0x78260005,  //  00C2  JMPF	R9	#00C9
      0x88240114,  //  00C3  GETMBR	R9	R0	K20
      0x8C241331,  //  00C4  GETMET	R9	R9	K49
      0x942C1007,  //  00C5  GETIDX	R11	R8	R7
      0x7C240400,  //  00C6  CALL	R9	2
      0x70020002,  //  00C7  JMP		#00CB
      0x70020000,  //  00C8  JMP		#00CA
      0x001C0F07,  //  00C9  ADD	R7	R7	K7
      0x7001FFED,  //  00CA  JMP		#00B9
      0x8C24031B,  //  00CB  GETMET	R9	R1	K27
      0x582C0032,  //  00CC  LDCONST	R11	K50
      0x7C240400,  //  00CD  CALL	R9	2
      0x70020170,  //  00CE  JMP		#0240
      0x8C18030F,  //  00CF  GETMET	R6	R1	K15
      0x58200033,  //  00D0  LDCONST	R8	K51
      0x7C180400,  //  00D1  CALL	R6	2
      0x781A0011,  //  00D2  JMPF	R6	#00E5
      0xB81A1000,  //  00D3  GETNGBL	R6	K8
      0x8C180D09,  //  00D4  GETMET	R6	R6	K9
      0x8C20050A,  //  00D5  GETMET	R8	R2	K10
      0x58280012,  //  00D6  LDCONST	R10	K18
      0x582C0033,  //  00D7  LDCONST	R11	K51
      0x7C200600,  //  00D8  CALL	R8	3
      0x58240013,  //  00D9  LDCONST	R9	K19
      0x7C180600,  //  00DA  CALL	R6	3
      0x88180114,  //  00DB  GETMBR	R6	R0	K20
      0x501C0000,  //  00DC  LDBOOL	R7	0	0
      0x901A6807,  //  00DD  SETMBR	R6	K52	R7
      0x88180114,  //  00DE  GETMBR	R6	R0	K20
      0x8C180D1A,  //  00DF  GETMET	R6	R6	K26
      0x7C180200,  //  00E0  CALL	R6	1
      0x8C18031B,  //  00E1  GETMET	R6	R1	K27
      0x5820001C,  //  00E2  LDCONST	R8	K28
      0x7C180400,  //  00E3  CALL	R6	2
      0x7002015A,  //  00E4  JMP		#0240
      0x8C18030F,  //  00E5  GETMET	R6	R1	K15
      0x58200035,  //  00E6  LDCONST	R8	K53
      0x7C180400,  //  00E7  CALL	R6	2
      0x781A009C,  //  00E8  JMPF	R6	#0186
      0xB81A1000,  //  00E9  GETNGBL	R6	K8
      0x8C180D09,  //  00EA  GETMET	R6	R6	K9
      0x8C20050A,  //  00EB  GETMET	R8	R2	K10
      0x58280012,  //  00EC  LDCONST	R10	K18
      0x582C0035,  //  00ED  LDCONST	R11	K53
      0x7C200600,  //  00EE  CALL	R8	3
      0x58240013,  //  00EF  LDCONST	R9	K19
      0x7C180600,  //  00F0  CALL	R6	3
      0x50180000,  //  00F1  LDBOOL	R6	0	0
      0x601C0010,  //  00F2  GETGBL	R7	G16
      0x8C200306,  //  00F3  GETMET	R8	R1	K6
      0x7C200200,  //  00F4  CALL	R8	1
      0x04201107,  //  00F5  SUB	R8	R8	K7
      0x40220A08,  //  00F6  CONNECT	R8	K5	R8
      0x7C1C0200,  //  00F7  CALL	R7	1
      0xA8020066,  //  00F8  EXBLK	0	#0160
      0x5C200E00,  //  00F9  MOVE	R8	R7
      0x7C200000,  //  00FA  CALL	R8	0
      0x8C24030C,  //  00FB  GETMET	R9	R1	K12
      0x5C2C1000,  //  00FC  MOVE	R11	R8
      0x7C240400,  //  00FD  CALL	R9	2
      0x8C280536,  //  00FE  GETMET	R10	R2	K54
      0x5C301200,  //  00FF  MOVE	R12	R9
      0x5834000D,  //  0100  LDCONST	R13	K13
      0x7C280600,  //  0101  CALL	R10	3
      0x1C281505,  //  0102  EQ	R10	R10	K5
      0x782A005A,  //  0103  JMPF	R10	#015F
      0x60280009,  //  0104  GETGBL	R10	G9
      0x402E2737,  //  0105  CONNECT	R11	K19	K55
      0x942C120B,  //  0106  GETIDX	R11	R9	R11
      0x7C280200,  //  0107  CALL	R10	1
      0x8C2C030D,  //  0108  GETMET	R11	R1	K13
      0x5C341000,  //  0109  MOVE	R13	R8
      0x7C2C0400,  //  010A  CALL	R11	2
      0x88300114,  //  010B  GETMBR	R12	R0	K20
      0x88301938,  //  010C  GETMBR	R12	R12	K56
      0x8C301936,  //  010D  GETMET	R12	R12	K54
      0x60380008,  //  010E  GETGBL	R14	G8
      0x5C3C1400,  //  010F  MOVE	R15	R10
      0x7C380200,  //  0110  CALL	R14	1
      0x7C300400,  //  0111  CALL	R12	2
      0x4C340000,  //  0112  LDNIL	R13
      0x2034180D,  //  0113  NE	R13	R12	R13
      0x78360041,  //  0114  JMPF	R13	#0157
      0x88340114,  //  0115  GETMBR	R13	R0	K20
      0x88341B39,  //  0116  GETMBR	R13	R13	K57
      0x8C341B36,  //  0117  GETMET	R13	R13	K54
      0x8C3C1936,  //  0118  GETMET	R15	R12	K54
      0x5844003A,  //  0119  LDCONST	R17	K58
      0x5848003B,  //  011A  LDCONST	R18	K59
      0x7C3C0600,  //  011B  CALL	R15	3
      0x7C340400,  //  011C  CALL	R13	2
      0x4C380000,  //  011D  LDNIL	R14
      0x20381A0E,  //  011E  NE	R14	R13	R14
      0x783A0035,  //  011F  JMPF	R14	#0156
      0xB83A1000,  //  0120  GETNGBL	R14	K8
      0x8C381D09,  //  0121  GETMET	R14	R14	K9
      0x8C40050A,  //  0122  GETMET	R16	R2	K10
      0x5848003C,  //  0123  LDCONST	R18	K60
      0x5C4C1400,  //  0124  MOVE	R19	R10
      0x5C501600,  //  0125  MOVE	R20	R11
      0x7C400800,  //  0126  CALL	R16	4
      0x58440013,  //  0127  LDCONST	R17	K19
      0x7C380600,  //  0128  CALL	R14	3
      0x8C381B3D,  //  0129  GETMET	R14	R13	K61
      0x5C401A00,  //  012A  MOVE	R16	R13
      0x5C441800,  //  012B  MOVE	R17	R12
      0x7C380600,  //  012C  CALL	R14	3
      0x203C1C0B,  //  012D  NE	R15	R14	R11
      0xB8421000,  //  012E  GETNGBL	R16	K8
      0x8C402109,  //  012F  GETMET	R16	R16	K9
      0x8C48050A,  //  0130  GETMET	R18	R2	K10
      0x5850003E,  //  0131  LDCONST	R20	K62
      0x5C541400,  //  0132  MOVE	R21	R10
      0x5C581C00,  //  0133  MOVE	R22	R14
      0x5C5C1600,  //  0134  MOVE	R23	R11
      0x20601C0B,  //  0135  NE	R24	R14	R11
      0x78620001,  //  0136  JMPF	R24	#0139
      0x5860003F,  //  0137  LDCONST	R24	K63
      0x70020000,  //  0138  JMP		#013A
      0x5860003B,  //  0139  LDCONST	R24	K59
      0x7C480C00,  //  013A  CALL	R18	6
      0x584C0013,  //  013B  LDCONST	R19	K19
      0x7C400600,  //  013C  CALL	R16	3
      0x783E0017,  //  013D  JMPF	R15	#0156
      0x50180200,  //  013E  LDBOOL	R6	1	0
      0x8C401B40,  //  013F  GETMET	R16	R13	K64
      0x5C481A00,  //  0140  MOVE	R18	R13
      0x5C4C1800,  //  0141  MOVE	R19	R12
      0x5C501600,  //  0142  MOVE	R20	R11
      0x7C400800,  //  0143  CALL	R16	4
      0x88400114,  //  0144  GETMBR	R16	R0	K20
      0x8C402141,  //  0145  GETMET	R16	R16	K65
      0x5C481400,  //  0146  MOVE	R18	R10
      0x7C400400,  //  0147  CALL	R16	2
      0x7842000C,  //  0148  JMPF	R16	#0156
      0xB8461000,  //  0149  GETNGBL	R17	K8
      0x8C442309,  //  014A  GETMET	R17	R17	K9
      0x8C4C050A,  //  014B  GETMET	R19	R2	K10
      0x58540042,  //  014C  LDCONST	R21	K66
      0x5C581800,  //  014D  MOVE	R22	R12
      0x5C5C1400,  //  014E  MOVE	R23	R10
      0x5C602000,  //  014F  MOVE	R24	R16
      0x7C4C0A00,  //  0150  CALL	R19	5
      0x58500013,  //  0151  LDCONST	R20	K19
      0x7C440600,  //  0152  CALL	R17	3
      0x8C442143,  //  0153  GETMET	R17	R16	K67
      0x5C4C1800,  //  0154  MOVE	R19	R12
      0x7C440400,  //  0155  CALL	R17	2
      0x70020007,  //  0156  JMP		#015F
      0xB8361000,  //  0157  GETNGBL	R13	K8
      0x8C341B09,  //  0158  GETMET	R13	R13	K9
      0x8C3C050A,  //  0159  GETMET	R15	R2	K10
      0x58440044,  //  015A  LDCONST	R17	K68
      0x5C481400,  //  015B  MOVE	R18	R10
      0x7C3C0600,  //  015C  CALL	R15	3
      0x58400013,  //  015D  LDCONST	R16	K19
      0x7C340600,  //  015E  CALL	R13	3
      0x7001FF98,  //  015F  JMP		#00F9
      0x581C000E,  //  0160  LDCONST	R7	K14
      0xAC1C0200,  //  0161  CATCH	R7	1	0
      0xB0080000,  //  0162  RAISE	2	R0	R0
      0xB81E1000,  //  0163  GETNGBL	R7	K8
      0x8C1C0F09,  //  0164  GETMET	R7	R7	K9
      0x8C24050A,  //  0165  GETMET	R9	R2	K10
      0x582C0045,  //  0166  LDCONST	R11	K69
      0x60300008,  //  0167  GETGBL	R12	G8
      0x88340114,  //  0168  GETMBR	R13	R0	K20
      0x88341B38,  //  0169  GETMBR	R13	R13	K56
      0x7C300200,  //  016A  CALL	R12	1
      0x7C240600,  //  016B  CALL	R9	3
      0x58280013,  //  016C  LDCONST	R10	K19
      0x7C1C0600,  //  016D  CALL	R7	3
      0x78160008,  //  016E  JMPF	R5	#0178
      0xB81E1000,  //  016F  GETNGBL	R7	K8
      0x8C1C0F09,  //  0170  GETMET	R7	R7	K9
      0x8C24050A,  //  0171  GETMET	R9	R2	K10
      0x582C0046,  //  0172  LDCONST	R11	K70
      0x5C300A00,  //  0173  MOVE	R12	R5
      0x7C240600,  //  0174  CALL	R9	3
      0x58280013,  //  0175  LDCONST	R10	K19
      0x7C1C0600,  //  0176  CALL	R7	3
      0x7002000C,  //  0177  JMP		#0185
      0x741A0002,  //  0178  JMPT	R6	#017C
      0x881C0114,  //  0179  GETMBR	R7	R0	K20
      0x881C0F34,  //  017A  GETMBR	R7	R7	K52
      0x741E0005,  //  017B  JMPT	R7	#0182
      0x881C0114,  //  017C  GETMBR	R7	R0	K20
      0x50200200,  //  017D  LDBOOL	R8	1	0
      0x901E6808,  //  017E  SETMBR	R7	K52	R8
      0x881C0114,  //  017F  GETMBR	R7	R0	K20
      0x8C1C0F1A,  //  0180  GETMET	R7	R7	K26
      0x7C1C0200,  //  0181  CALL	R7	1
      0x8C1C031B,  //  0182  GETMET	R7	R1	K27
      0x58240047,  //  0183  LDCONST	R9	K71
      0x7C1C0400,  //  0184  CALL	R7	2
      0x700200B9,  //  0185  JMP		#0240
      0x8C18030F,  //  0186  GETMET	R6	R1	K15
      0x58200048,  //  0187  LDCONST	R8	K72
      0x7C180400,  //  0188  CALL	R6	2
      0x781A0026,  //  0189  JMPF	R6	#01B1
      0x8C18030D,  //  018A  GETMET	R6	R1	K13
      0x58200049,  //  018B  LDCONST	R8	K73
      0x7C180400,  //  018C  CALL	R6	2
      0x8C1C030D,  //  018D  GETMET	R7	R1	K13
      0x5824000D,  //  018E  LDCONST	R9	K13
      0x7C1C0400,  //  018F  CALL	R7	2
      0xB8221000,  //  0190  GETNGBL	R8	K8
      0x8C201109,  //  0191  GETMET	R8	R8	K9
      0x8C28050A,  //  0192  GETMET	R10	R2	K10
      0x5830004A,  //  0193  LDCONST	R12	K74
      0x5C340C00,  //  0194  MOVE	R13	R6
      0x5C380E00,  //  0195  MOVE	R14	R7
      0x7C280800,  //  0196  CALL	R10	4
      0x582C0013,  //  0197  LDCONST	R11	K19
      0x7C200600,  //  0198  CALL	R8	3
      0x88200114,  //  0199  GETMBR	R8	R0	K20
      0x88201139,  //  019A  GETMBR	R8	R8	K57
      0x8C201136,  //  019B  GETMET	R8	R8	K54
      0x5C280C00,  //  019C  MOVE	R10	R6
      0x7C200400,  //  019D  CALL	R8	2
      0x4C240000,  //  019E  LDNIL	R9
      0x20241009,  //  019F  NE	R9	R8	R9
      0x7826000B,  //  01A0  JMPF	R9	#01AD
      0x60240013,  //  01A1  GETGBL	R9	G19
      0x7C240000,  //  01A2  CALL	R9	0
      0x8C281140,  //  01A3  GETMET	R10	R8	K64
      0x5C301000,  //  01A4  MOVE	R12	R8
      0x5C341200,  //  01A5  MOVE	R13	R9
      0x5C380E00,  //  01A6  MOVE	R14	R7
      0x7C280800,  //  01A7  CALL	R10	4
      0x88280114,  //  01A8  GETMBR	R10	R0	K20
      0x8C28154B,  //  01A9  GETMET	R10	R10	K75
      0x5C300C00,  //  01AA  MOVE	R12	R6
      0x5C341200,  //  01AB  MOVE	R13	R9
      0x7C280600,  //  01AC  CALL	R10	3
      0x8C24031B,  //  01AD  GETMET	R9	R1	K27
      0x582C0032,  //  01AE  LDCONST	R11	K50
      0x7C240400,  //  01AF  CALL	R9	2
      0x7002008E,  //  01B0  JMP		#0240
      0x8C18030F,  //  01B1  GETMET	R6	R1	K15
      0x5820004C,  //  01B2  LDCONST	R8	K76
      0x7C180400,  //  01B3  CALL	R6	2
      0x781A0059,  //  01B4  JMPF	R6	#020F
      0x8C18030D,  //  01B5  GETMET	R6	R1	K13
      0x5820004D,  //  01B6  LDCONST	R8	K77
      0x7C180400,  //  01B7  CALL	R6	2
      0x4C1C0000,  //  01B8  LDNIL	R7
      0x1C1C0C07,  //  01B9  EQ	R7	R6	R7
      0x741E0001,  //  01BA  JMPT	R7	#01BD
      0x1C1C0D3B,  //  01BB  EQ	R7	R6	K59
      0x781E0000,  //  01BC  JMPF	R7	#01BE
      0xB0069D4F,  //  01BD  RAISE	1	K78	K79
      0x581C0005,  //  01BE  LDCONST	R7	K5
      0x60200008,  //  01BF  GETGBL	R8	G8
      0x5C240E00,  //  01C0  MOVE	R9	R7
      0x7C200200,  //  01C1  CALL	R8	1
      0x8C24030F,  //  01C2  GETMET	R9	R1	K15
      0x002E9208,  //  01C3  ADD	R11	K73	R8
      0x7C240400,  //  01C4  CALL	R9	2
      0x78260044,  //  01C5  JMPF	R9	#020B
      0x8C24030D,  //  01C6  GETMET	R9	R1	K13
      0x002E9208,  //  01C7  ADD	R11	K73	R8
      0x7C240400,  //  01C8  CALL	R9	2
      0x8C28030D,  //  01C9  GETMET	R10	R1	K13
      0x00321A08,  //  01CA  ADD	R12	K13	R8
      0x7C280400,  //  01CB  CALL	R10	2
      0x202C133B,  //  01CC  NE	R11	R9	K59
      0x782E0036,  //  01CD  JMPF	R11	#0205
      0x882C0114,  //  01CE  GETMBR	R11	R0	K20
      0x882C1739,  //  01CF  GETMBR	R11	R11	K57
      0x8C2C1736,  //  01D0  GETMET	R11	R11	K54
      0x5C341200,  //  01D1  MOVE	R13	R9
      0x7C2C0400,  //  01D2  CALL	R11	2
      0x4C300000,  //  01D3  LDNIL	R12
      0x2030160C,  //  01D4  NE	R12	R11	R12
      0x7832002E,  //  01D5  JMPF	R12	#0205
      0x60300013,  //  01D6  GETGBL	R12	G19
      0x7C300000,  //  01D7  CALL	R12	0
      0x98329A06,  //  01D8  SETIDX	R12	K77	R6
      0x98327409,  //  01D9  SETIDX	R12	K58	R9
      0x8C341740,  //  01DA  GETMET	R13	R11	K64
      0x5C3C1600,  //  01DB  MOVE	R15	R11
      0x5C401800,  //  01DC  MOVE	R16	R12
      0x5C441400,  //  01DD  MOVE	R17	R10
      0x7C340800,  //  01DE  CALL	R13	4
      0x50340000,  //  01DF  LDBOOL	R13	0	0
      0x60380010,  //  01E0  GETGBL	R14	G16
      0x883C0114,  //  01E1  GETMBR	R15	R0	K20
      0x883C1F38,  //  01E2  GETMBR	R15	R15	K56
      0x7C380200,  //  01E3  CALL	R14	1
      0xA802000B,  //  01E4  EXBLK	0	#01F1
      0x5C3C1C00,  //  01E5  MOVE	R15	R14
      0x7C3C0000,  //  01E6  CALL	R15	0
      0x8C400150,  //  01E7  GETMET	R16	R0	K80
      0x5C481E00,  //  01E8  MOVE	R18	R15
      0x5C4C1800,  //  01E9  MOVE	R19	R12
      0x7C400600,  //  01EA  CALL	R16	3
      0x78420001,  //  01EB  JMPF	R16	#01EE
      0x50340200,  //  01EC  LDBOOL	R13	1	0
      0x70020000,  //  01ED  JMP		#01EF
      0x7001FFF5,  //  01EE  JMP		#01E5
      0xA8040001,  //  01EF  EXBLK	1	1
      0x70020002,  //  01F0  JMP		#01F4
      0x5838000E,  //  01F1  LDCONST	R14	K14
      0xAC380200,  //  01F2  CATCH	R14	1	0
      0xB0080000,  //  01F3  RAISE	2	R0	R0
      0x5C381A00,  //  01F4  MOVE	R14	R13
      0x743A000E,  //  01F5  JMPT	R14	#0205
      0xB83A1000,  //  01F6  GETNGBL	R14	K8
      0x8C381D09,  //  01F7  GETMET	R14	R14	K9
      0x8C40050A,  //  01F8  GETMET	R16	R2	K10
      0x58480051,  //  01F9  LDCONST	R18	K81
      0x5C4C0C00,  //  01FA  MOVE	R19	R6
      0x5C501200,  //  01FB  MOVE	R20	R9
      0x5C541400,  //  01FC  MOVE	R21	R10
      0x7C400A00,  //  01FD  CALL	R16	5
      0x58440013,  //  01FE  LDCONST	R17	K19
      0x7C380600,  //  01FF  CALL	R14	3
      0x88380114,  //  0200  GETMBR	R14	R0	K20
      0x8C381D4B,  //  0201  GETMET	R14	R14	K75
      0x5C401200,  //  0202  MOVE	R16	R9
      0x5C441800,  //  0203  MOVE	R17	R12
      0x7C380600,  //  0204  CALL	R14	3
      0x001C0F07,  //  0205  ADD	R7	R7	K7
      0x602C0008,  //  0206  GETGBL	R11	G8
      0x5C300E00,  //  0207  MOVE	R12	R7
      0x7C2C0200,  //  0208  CALL	R11	1
      0x5C201600,  //  0209  MOVE	R8	R11
      0x7001FFB6,  //  020A  JMP		#01C2
      0x8C24031B,  //  020B  GETMET	R9	R1	K27
      0x582C0032,  //  020C  LDCONST	R11	K50
      0x7C240400,  //  020D  CALL	R9	2
      0x70020030,  //  020E  JMP		#0240
      0x4C180000,  //  020F  LDNIL	R6
      0x601C0010,  //  0210  GETGBL	R7	G16
      0x8C200306,  //  0211  GETMET	R8	R1	K6
      0x7C200200,  //  0212  CALL	R8	1
      0x04201107,  //  0213  SUB	R8	R8	K7
      0x40220A08,  //  0214  CONNECT	R8	K5	R8
      0x7C1C0200,  //  0215  CALL	R7	1
      0xA8020013,  //  0216  EXBLK	0	#022B
      0x5C200E00,  //  0217  MOVE	R8	R7
      0x7C200000,  //  0218  CALL	R8	0
      0x8C24030C,  //  0219  GETMET	R9	R1	K12
      0x5C2C1000,  //  021A  MOVE	R11	R8
      0x7C240400,  //  021B  CALL	R9	2
      0x8C280536,  //  021C  GETMET	R10	R2	K54
      0x5C301200,  //  021D  MOVE	R12	R9
      0x58340052,  //  021E  LDCONST	R13	K82
      0x7C280600,  //  021F  CALL	R10	3
      0x1C281505,  //  0220  EQ	R10	R10	K5
      0x782A0005,  //  0221  JMPF	R10	#0228
      0x60280009,  //  0222  GETGBL	R10	G9
      0x402E2737,  //  0223  CONNECT	R11	K19	K55
      0x942C120B,  //  0224  GETIDX	R11	R9	R11
      0x7C280200,  //  0225  CALL	R10	1
      0x5C181400,  //  0226  MOVE	R6	R10
      0x70020000,  //  0227  JMP		#0229
      0x7001FFED,  //  0228  JMP		#0217
      0xA8040001,  //  0229  EXBLK	1	1
      0x70020002,  //  022A  JMP		#022E
      0x581C000E,  //  022B  LDCONST	R7	K14
      0xAC1C0200,  //  022C  CATCH	R7	1	0
      0xB0080000,  //  022D  RAISE	2	R0	R0
      0x4C1C0000,  //  022E  LDNIL	R7
      0x201C0C07,  //  022F  NE	R7	R6	R7
      0x781E000E,  //  0230  JMPF	R7	#0240
      0xB81E1000,  //  0231  GETNGBL	R7	K8
      0x8C1C0F09,  //  0232  GETMET	R7	R7	K9
      0x8C24050A,  //  0233  GETMET	R9	R2	K10
      0x582C0053,  //  0234  LDCONST	R11	K83
      0x5C300C00,  //  0235  MOVE	R12	R6
      0x7C240600,  //  0236  CALL	R9	3
      0x58280054,  //  0237  LDCONST	R10	K84
      0x7C1C0600,  //  0238  CALL	R7	3
      0x881C0114,  //  0239  GETMBR	R7	R0	K20
      0x8C1C0F55,  //  023A  GETMET	R7	R7	K85
      0x5C240C00,  //  023B  MOVE	R9	R6
      0x7C1C0400,  //  023C  CALL	R7	2
      0x8C1C031B,  //  023D  GETMET	R7	R1	K27
      0x58240032,  //  023E  LDCONST	R9	K50
      0x7C1C0400,  //  023F  CALL	R7	2
      0x78160011,  //  0240  JMPF	R5	#0253
      0x8C180356,  //  0241  GETMET	R6	R1	K86
      0x58200057,  //  0242  LDCONST	R8	K87
      0x7C180400,  //  0243  CALL	R6	2
      0x8C180358,  //  0244  GETMET	R6	R1	K88
      0x7C180200,  //  0245  CALL	R6	1
      0x8C180359,  //  0246  GETMET	R6	R1	K89
      0x8C20050A,  //  0247  GETMET	R8	R2	K10
      0x5828005A,  //  0248  LDCONST	R10	K90
      0x8C2C035B,  //  0249  GETMET	R11	R1	K91
      0x5C340A00,  //  024A  MOVE	R13	R5
      0x7C2C0400,  //  024B  CALL	R11	2
      0x7C200600,  //  024C  CALL	R8	3
      0x7C180400,  //  024D  CALL	R6	2
      0x8C18035C,  //  024E  GETMET	R6	R1	K92
      0x8820035D,  //  024F  GETMBR	R8	R1	K93
      0x7C180400,  //  0250  CALL	R6	2
      0x8C18035E,  //  0251  GETMET	R6	R1	K94
      0x7C180200,  //  0252  CALL	R6	1
      0xA8040001,  //  0253  EXBLK	1	1
      0x7002001D,  //  0254  JMP		#0273
      0xAC180002,  //  0255  CATCH	R6	0	2
      0x7002001A,  //  0256  JMP		#0272
      0xB8221000,  //  0257  GETNGBL	R8	K8
      0x8C201109,  //  0258  GETMET	R8	R8	K9
      0x8C28050A,  //  0259  GETMET	R10	R2	K10
      0x5830005F,  //  025A  LDCONST	R12	K95
      0x5C340C00,  //  025B  MOVE	R13	R6
      0x5C380E00,  //  025C  MOVE	R14	R7
      0x7C280800,  //  025D  CALL	R10	4
      0x582C0054,  //  025E  LDCONST	R11	K84
      0x7C200600,  //  025F  CALL	R8	3
      0x8C200356,  //  0260  GETMET	R8	R1	K86
      0x58280057,  //  0261  LDCONST	R10	K87
      0x7C200400,  //  0262  CALL	R8	2
      0x8C200358,  //  0263  GETMET	R8	R1	K88
      0x7C200200,  //  0264  CALL	R8	1
      0x8C200359,  //  0265  GETMET	R8	R1	K89
      0x8C28050A,  //  0266  GETMET	R10	R2	K10
      0x58300060,  //  0267  LDCONST	R12	K96
      0x5C340C00,  //  0268  MOVE	R13	R6
      0x5C380E00,  //  0269  MOVE	R14	R7
      0x7C280800,  //  026A  CALL	R10	4
      0x7C200400,  //  026B  CALL	R8	2
      0x8C20035C,  //  026C  GETMET	R8	R1	K92
      0x8828035D,  //  026D  GETMBR	R10	R1	K93
      0x7C200400,  //  026E  CALL	R8	2
      0x8C20035E,  //  026F  GETMET	R8	R1	K94
      0x7C200200,  //  0270  CALL	R8	1
      0x70020000,  //  0271  JMP		#0273
      0xB0080000,  //  0272  RAISE	2	R0	R0
      0x80000000,  //  0273  RET	0
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
    ( &(const bvalue[45]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(json),
    /* K3   */  be_nested_str_weak(),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(Plugin_Bridge_HTTP),
    /* K6   */  be_nested_str_weak(PROBE_TIMEOUT),
    /* K7   */  be_nested_str_weak(HTTP_remote),
    /* K8   */  be_nested_str_weak(call_sync),
    /* K9   */  be_nested_str_weak(Status_X208),
    /* K10  */  be_nested_str_weak(load),
    /* K11  */  be_nested_str_weak(find),
    /* K12  */  be_nested_str_weak(StatusSNS),
    /* K13  */  be_nested_str_weak(Status_X2011),
    /* K14  */  be_nested_str_weak(StatusSTS),
    /* K15  */  be_nested_str_weak(tasmota),
    /* K16  */  be_nested_str_weak(log),
    /* K17  */  be_nested_str_weak(format),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20probed_X20_X27_X25s_X27_X20status8_X3D_X25s_X20satus11_X3D_X25s),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(generate_config_from_status),
    /* K21  */  be_nested_str_weak(content_send),
    /* K22  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Remote_X20Device_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K23  */  be_nested_str_weak(_X3Cp_X3E_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X22_X3E_X25s_X3C_X2Fa_X3E_X3C_X2Fp_X3E),
    /* K24  */  be_nested_str_weak(html_escape),
    /* K25  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X27145_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X3EParameter_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K26  */  be_nested_str_weak(_X3Cinput_X20name_X3D_X27url_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X25s_X27_X3E),
    /* K27  */  be_const_int(0),
    /* K28  */  be_nested_str_weak(type),
    /* K29  */  be_nested_str_weak(http_),
    /* K30  */  be_nested_str_weak(device),
    /* K31  */  be_nested_str_weak(plugins_classes),
    /* K32  */  be_nested_str_weak(ui_conf_to_string),
    /* K33  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cselect_X20name_X3D_X27pi_X25i_X27_X3E),
    /* K34  */  be_nested_str_weak(plugin_option),
    /* K35  */  be_nested_str_weak(_CLASSES_TYPES2),
    /* K36  */  be_nested_str_weak(_X3C_X2Fselect_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E),
    /* K37  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X25i_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X25s_X27_X3E),
    /* K38  */  be_nested_str_weak(_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K39  */  be_const_int(1),
    /* K40  */  be_nested_str_weak(_X3C_X2Ftable_X3E),
    /* K41  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K42  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27addrem_X27_X20class_X3D_X27button_X20bgrn_X27_X3EAdd_X20endpoints_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K43  */  be_nested_str_weak(_X3C_X2Fform_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K44  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3EUnable_X20to_X20connect_X20to_X20_X27_X25s_X27_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_remote_autoconf),
    &be_const_str_solidified,
    ( &(const binstruction[204]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0x1C140303,  //  0003  EQ	R5	R1	K3
      0x78160000,  //  0004  JMPF	R5	#0006
      0x80000A00,  //  0005  RET	0
      0xB8160800,  //  0006  GETNGBL	R5	K4
      0x88140B05,  //  0007  GETMBR	R5	R5	K5
      0x88140B06,  //  0008  GETMBR	R5	R5	K6
      0xB81A0800,  //  0009  GETNGBL	R6	K4
      0x8C180D07,  //  000A  GETMET	R6	R6	K7
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240A00,  //  000C  MOVE	R9	R5
      0x7C180600,  //  000D  CALL	R6	3
      0x8C1C0D08,  //  000E  GETMET	R7	R6	K8
      0x58240009,  //  000F  LDCONST	R9	K9
      0x5C280A00,  //  0010  MOVE	R10	R5
      0x7C1C0600,  //  0011  CALL	R7	3
      0x4C200000,  //  0012  LDNIL	R8
      0x20200E08,  //  0013  NE	R8	R7	R8
      0x78220003,  //  0014  JMPF	R8	#0019
      0x8C20090A,  //  0015  GETMET	R8	R4	K10
      0x5C280E00,  //  0016  MOVE	R10	R7
      0x7C200400,  //  0017  CALL	R8	2
      0x5C1C1000,  //  0018  MOVE	R7	R8
      0x4C200000,  //  0019  LDNIL	R8
      0x20200E08,  //  001A  NE	R8	R7	R8
      0x78220003,  //  001B  JMPF	R8	#0020
      0x8C200F0B,  //  001C  GETMET	R8	R7	K11
      0x5828000C,  //  001D  LDCONST	R10	K12
      0x7C200400,  //  001E  CALL	R8	2
      0x5C1C1000,  //  001F  MOVE	R7	R8
      0x4C200000,  //  0020  LDNIL	R8
      0x4C240000,  //  0021  LDNIL	R9
      0x20240E09,  //  0022  NE	R9	R7	R9
      0x78260012,  //  0023  JMPF	R9	#0037
      0x8C240D08,  //  0024  GETMET	R9	R6	K8
      0x582C000D,  //  0025  LDCONST	R11	K13
      0x5C300A00,  //  0026  MOVE	R12	R5
      0x7C240600,  //  0027  CALL	R9	3
      0x5C201200,  //  0028  MOVE	R8	R9
      0x4C240000,  //  0029  LDNIL	R9
      0x20241009,  //  002A  NE	R9	R8	R9
      0x78260003,  //  002B  JMPF	R9	#0030
      0x8C24090A,  //  002C  GETMET	R9	R4	K10
      0x5C2C1000,  //  002D  MOVE	R11	R8
      0x7C240400,  //  002E  CALL	R9	2
      0x5C201200,  //  002F  MOVE	R8	R9
      0x4C240000,  //  0030  LDNIL	R9
      0x20241009,  //  0031  NE	R9	R8	R9
      0x78260003,  //  0032  JMPF	R9	#0037
      0x8C24110B,  //  0033  GETMET	R9	R8	K11
      0x582C000E,  //  0034  LDCONST	R11	K14
      0x7C240400,  //  0035  CALL	R9	2
      0x5C201200,  //  0036  MOVE	R8	R9
      0x4C240000,  //  0037  LDNIL	R9
      0x20240E09,  //  0038  NE	R9	R7	R9
      0x78260088,  //  0039  JMPF	R9	#00C3
      0x4C240000,  //  003A  LDNIL	R9
      0x20241009,  //  003B  NE	R9	R8	R9
      0x78260085,  //  003C  JMPF	R9	#00C3
      0xB8261E00,  //  003D  GETNGBL	R9	K15
      0x8C241310,  //  003E  GETMET	R9	R9	K16
      0x8C2C0711,  //  003F  GETMET	R11	R3	K17
      0x58340012,  //  0040  LDCONST	R13	K18
      0x5C380200,  //  0041  MOVE	R14	R1
      0x603C0008,  //  0042  GETGBL	R15	G8
      0x5C400E00,  //  0043  MOVE	R16	R7
      0x7C3C0200,  //  0044  CALL	R15	1
      0x60400008,  //  0045  GETGBL	R16	G8
      0x5C441000,  //  0046  MOVE	R17	R8
      0x7C400200,  //  0047  CALL	R16	1
      0x7C2C0A00,  //  0048  CALL	R11	5
      0x58300013,  //  0049  LDCONST	R12	K19
      0x7C240600,  //  004A  CALL	R9	3
      0x8C240114,  //  004B  GETMET	R9	R0	K20
      0x5C2C0E00,  //  004C  MOVE	R11	R7
      0x5C301000,  //  004D  MOVE	R12	R8
      0x7C240600,  //  004E  CALL	R9	3
      0x8C280515,  //  004F  GETMET	R10	R2	K21
      0x58300016,  //  0050  LDCONST	R12	K22
      0x7C280400,  //  0051  CALL	R10	2
      0x8C280515,  //  0052  GETMET	R10	R2	K21
      0x8C300711,  //  0053  GETMET	R12	R3	K17
      0x58380017,  //  0054  LDCONST	R14	K23
      0x8C3C0518,  //  0055  GETMET	R15	R2	K24
      0x5C440200,  //  0056  MOVE	R17	R1
      0x7C3C0400,  //  0057  CALL	R15	2
      0x8C400518,  //  0058  GETMET	R16	R2	K24
      0x5C480200,  //  0059  MOVE	R18	R1
      0x7C400400,  //  005A  CALL	R16	2
      0x7C300800,  //  005B  CALL	R12	4
      0x7C280400,  //  005C  CALL	R10	2
      0x8C280515,  //  005D  GETMET	R10	R2	K21
      0x58300019,  //  005E  LDCONST	R12	K25
      0x7C280400,  //  005F  CALL	R10	2
      0x8C280515,  //  0060  GETMET	R10	R2	K21
      0x8C300711,  //  0061  GETMET	R12	R3	K17
      0x5838001A,  //  0062  LDCONST	R14	K26
      0x8C3C0518,  //  0063  GETMET	R15	R2	K24
      0x5C440200,  //  0064  MOVE	R17	R1
      0x7C3C0400,  //  0065  CALL	R15	2
      0x7C300600,  //  0066  CALL	R12	3
      0x7C280400,  //  0067  CALL	R10	2
      0x5828001B,  //  0068  LDCONST	R10	K27
      0x602C000C,  //  0069  GETGBL	R11	G12
      0x5C301200,  //  006A  MOVE	R12	R9
      0x7C2C0200,  //  006B  CALL	R11	1
      0x142C140B,  //  006C  LT	R11	R10	R11
      0x782E0030,  //  006D  JMPF	R11	#009F
      0x942C120A,  //  006E  GETIDX	R11	R9	R10
      0x8C30170B,  //  006F  GETMET	R12	R11	K11
      0x5838001C,  //  0070  LDCONST	R14	K28
      0x583C0003,  //  0071  LDCONST	R15	K3
      0x7C300600,  //  0072  CALL	R12	3
      0x20341903,  //  0073  NE	R13	R12	K3
      0x78360000,  //  0074  JMPF	R13	#0076
      0x00323A0C,  //  0075  ADD	R12	K29	R12
      0x8834011E,  //  0076  GETMBR	R13	R0	K30
      0x88341B1F,  //  0077  GETMBR	R13	R13	K31
      0x8C341B0B,  //  0078  GETMET	R13	R13	K11
      0x5C3C1800,  //  0079  MOVE	R15	R12
      0x7C340400,  //  007A  CALL	R13	2
      0x58380003,  //  007B  LDCONST	R14	K3
      0x4C3C0000,  //  007C  LDNIL	R15
      0x203C1A0F,  //  007D  NE	R15	R13	R15
      0x783E0004,  //  007E  JMPF	R15	#0084
      0x8C3C1B20,  //  007F  GETMET	R15	R13	K32
      0x5C441A00,  //  0080  MOVE	R17	R13
      0x5C481600,  //  0081  MOVE	R18	R11
      0x7C3C0600,  //  0082  CALL	R15	3
      0x5C381E00,  //  0083  MOVE	R14	R15
      0x8C3C0515,  //  0084  GETMET	R15	R2	K21
      0x8C440711,  //  0085  GETMET	R17	R3	K17
      0x584C0021,  //  0086  LDCONST	R19	K33
      0x5C501400,  //  0087  MOVE	R20	R10
      0x7C440600,  //  0088  CALL	R17	3
      0x7C3C0400,  //  0089  CALL	R15	2
      0x8C3C0122,  //  008A  GETMET	R15	R0	K34
      0x5C441800,  //  008B  MOVE	R17	R12
      0x88480123,  //  008C  GETMBR	R18	R0	K35
      0x7C3C0600,  //  008D  CALL	R15	3
      0x8C3C0515,  //  008E  GETMET	R15	R2	K21
      0x58440024,  //  008F  LDCONST	R17	K36
      0x7C3C0400,  //  0090  CALL	R15	2
      0x8C3C0515,  //  0091  GETMET	R15	R2	K21
      0x8C440711,  //  0092  GETMET	R17	R3	K17
      0x584C0025,  //  0093  LDCONST	R19	K37
      0x5C501400,  //  0094  MOVE	R20	R10
      0x8C540518,  //  0095  GETMET	R21	R2	K24
      0x5C5C1C00,  //  0096  MOVE	R23	R14
      0x7C540400,  //  0097  CALL	R21	2
      0x7C440800,  //  0098  CALL	R17	4
      0x7C3C0400,  //  0099  CALL	R15	2
      0x8C3C0515,  //  009A  GETMET	R15	R2	K21
      0x58440026,  //  009B  LDCONST	R17	K38
      0x7C3C0400,  //  009C  CALL	R15	2
      0x00281527,  //  009D  ADD	R10	R10	K39
      0x7001FFC9,  //  009E  JMP		#0069
      0x8C2C0515,  //  009F  GETMET	R11	R2	K21
      0x8C340711,  //  00A0  GETMET	R13	R3	K17
      0x583C0021,  //  00A1  LDCONST	R15	K33
      0x5C401400,  //  00A2  MOVE	R16	R10
      0x7C340600,  //  00A3  CALL	R13	3
      0x7C2C0400,  //  00A4  CALL	R11	2
      0x8C2C0122,  //  00A5  GETMET	R11	R0	K34
      0x58340003,  //  00A6  LDCONST	R13	K3
      0x88380123,  //  00A7  GETMBR	R14	R0	K35
      0x7C2C0600,  //  00A8  CALL	R11	3
      0x8C2C0515,  //  00A9  GETMET	R11	R2	K21
      0x58340024,  //  00AA  LDCONST	R13	K36
      0x7C2C0400,  //  00AB  CALL	R11	2
      0x8C2C0515,  //  00AC  GETMET	R11	R2	K21
      0x8C340711,  //  00AD  GETMET	R13	R3	K17
      0x583C0025,  //  00AE  LDCONST	R15	K37
      0x5C401400,  //  00AF  MOVE	R16	R10
      0x58440003,  //  00B0  LDCONST	R17	K3
      0x7C340800,  //  00B1  CALL	R13	4
      0x7C2C0400,  //  00B2  CALL	R11	2
      0x8C2C0515,  //  00B3  GETMET	R11	R2	K21
      0x58340026,  //  00B4  LDCONST	R13	K38
      0x7C2C0400,  //  00B5  CALL	R11	2
      0x8C2C0515,  //  00B6  GETMET	R11	R2	K21
      0x58340028,  //  00B7  LDCONST	R13	K40
      0x7C2C0400,  //  00B8  CALL	R11	2
      0x8C2C0515,  //  00B9  GETMET	R11	R2	K21
      0x58340029,  //  00BA  LDCONST	R13	K41
      0x7C2C0400,  //  00BB  CALL	R11	2
      0x8C2C0515,  //  00BC  GETMET	R11	R2	K21
      0x5834002A,  //  00BD  LDCONST	R13	K42
      0x7C2C0400,  //  00BE  CALL	R11	2
      0x8C2C0515,  //  00BF  GETMET	R11	R2	K21
      0x5834002B,  //  00C0  LDCONST	R13	K43
      0x7C2C0400,  //  00C1  CALL	R11	2
      0x70020007,  //  00C2  JMP		#00CB
      0x8C240515,  //  00C3  GETMET	R9	R2	K21
      0x8C2C0711,  //  00C4  GETMET	R11	R3	K17
      0x5834002C,  //  00C5  LDCONST	R13	K44
      0x8C380518,  //  00C6  GETMET	R14	R2	K24
      0x5C400200,  //  00C7  MOVE	R16	R1
      0x7C380400,  //  00C8  CALL	R14	2
      0x7C2C0600,  //  00C9  CALL	R11	3
      0x7C240400,  //  00CA  CALL	R9	2
      0x80000000,  //  00CB  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: page_part_mgr_add
********************************************************************/
be_local_closure(Matter_UI_page_part_mgr_add,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(check_privileged_access),
    /* K3   */  be_nested_str_weak(content_start),
    /* K4   */  be_nested_str_weak(Matter_X20Create_X20new_X20endpoint),
    /* K5   */  be_nested_str_weak(content_send_style),
    /* K6   */  be_nested_str_weak(arg),
    /* K7   */  be_nested_str_weak(url),
    /* K8   */  be_nested_str_weak(matter_enabled),
    /* K9   */  be_nested_str_weak(show_remote_autoconf),
    /* K10  */  be_nested_str_weak(content_button),
    /* K11  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K12  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr_add),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
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
      0x8C0C0306,  //  000C  GETMET	R3	R1	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C100108,  //  000F  GETMET	R4	R0	K8
      0x7C100200,  //  0010  CALL	R4	1
      0x78120002,  //  0011  JMPF	R4	#0015
      0x8C100109,  //  0012  GETMET	R4	R0	K9
      0x5C180600,  //  0013  MOVE	R6	R3
      0x7C100400,  //  0014  CALL	R4	2
      0x8C10030A,  //  0015  GETMET	R4	R1	K10
      0x8818030B,  //  0016  GETMBR	R6	R1	K11
      0x7C100400,  //  0017  CALL	R4	2
      0x8C10030C,  //  0018  GETMET	R4	R1	K12
      0x7C100200,  //  0019  CALL	R4	1
      0x80000000,  //  001A  RET	0
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(matter_enabled),
    /* K3   */  be_nested_str_weak(content_send),
    /* K4   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X20style_X3D_X27width_X3A320px_X3B_X27_X3ECheck_X20the_X20_X3Ca_X20href_X3D_X27https_X3A_X2F_X2Ftasmota_X2Egithub_X2Eio_X2Fdocs_X2FMatter_X2F_X27_X20target_X3D_X27_blank_X27_X3EMatter_X20documentation_X3C_X2Fa_X3E_X2E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20id_X3D_X27menable_X27_X20type_X3D_X27checkbox_X27_X20name_X3D_X27menable_X27_X20_X25s_X3E),
    /* K7   */  be_nested_str_weak(checked),
    /* K8   */  be_nested_str_weak(),
    /* K9   */  be_nested_str_weak(_X3Clabel_X20for_X3D_X27menable_X27_X3E_X3Cb_X3EMatter_X20enable_X3C_X2Fb_X3E_X3C_X2Flabel_X3E_X3C_X2Fp_X3E),
    /* K10  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20id_X3D_X27comm_X27_X20type_X3D_X27checkbox_X27_X20name_X3D_X27comm_X27_X20_X25s_X3E),
    /* K11  */  be_nested_str_weak(device),
    /* K12  */  be_nested_str_weak(commissioning_open),
    /* K13  */  be_nested_str_weak(_X3Clabel_X20for_X3D_X27comm_X27_X3E_X3Cb_X3ECommissioning_X20open_X3C_X2Fb_X3E_X3C_X2Flabel_X3E_X3C_X2Fp_X3E),
    /* K14  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27save_X27_X20class_X3D_X27button_X20bgrn_X27_X3ESave_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_enable),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x8C100303,  //  0003  GETMET	R4	R1	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x7C100400,  //  0005  CALL	R4	2
      0x8C100303,  //  0006  GETMET	R4	R1	K3
      0x8C180505,  //  0007  GETMET	R6	R2	K5
      0x58200006,  //  0008  LDCONST	R8	K6
      0x8C240102,  //  0009  GETMET	R9	R0	K2
      0x7C240200,  //  000A  CALL	R9	1
      0x78260001,  //  000B  JMPF	R9	#000E
      0x58240007,  //  000C  LDCONST	R9	K7
      0x70020000,  //  000D  JMP		#000F
      0x58240008,  //  000E  LDCONST	R9	K8
      0x7C180600,  //  000F  CALL	R6	3
      0x7C100400,  //  0010  CALL	R4	2
      0x8C100303,  //  0011  GETMET	R4	R1	K3
      0x58180009,  //  0012  LDCONST	R6	K9
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100102,  //  0014  GETMET	R4	R0	K2
      0x7C100200,  //  0015  CALL	R4	1
      0x7812000F,  //  0016  JMPF	R4	#0027
      0x8C100303,  //  0017  GETMET	R4	R1	K3
      0x8C180505,  //  0018  GETMET	R6	R2	K5
      0x5820000A,  //  0019  LDCONST	R8	K10
      0x8824010B,  //  001A  GETMBR	R9	R0	K11
      0x8824130C,  //  001B  GETMBR	R9	R9	K12
      0x4C280000,  //  001C  LDNIL	R10
      0x2024120A,  //  001D  NE	R9	R9	R10
      0x78260001,  //  001E  JMPF	R9	#0021
      0x58240007,  //  001F  LDCONST	R9	K7
      0x70020000,  //  0020  JMP		#0022
      0x58240008,  //  0021  LDCONST	R9	K8
      0x7C180600,  //  0022  CALL	R6	3
      0x7C100400,  //  0023  CALL	R4	2
      0x8C100303,  //  0024  GETMET	R4	R1	K3
      0x5818000D,  //  0025  LDCONST	R6	K13
      0x7C100400,  //  0026  CALL	R4	2
      0x8C100303,  //  0027  GETMET	R4	R1	K3
      0x5818000E,  //  0028  LDCONST	R6	K14
      0x7C100400,  //  0029  CALL	R4	2
      0x80000000,  //  002A  RET	0
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
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(device),
    /* K3   */  be_nested_str_weak(commissioning_open),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(millis),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(content_send),
    /* K8   */  be_nested_str_weak(format),
    /* K9   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BCommissioning_X20open_X20for_X20_X25i_X20min_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K10  */  be_nested_str_weak(compute_manual_pairing_code),
    /* K11  */  be_nested_str_weak(_X3Cp_X3EManual_X20pairing_X20code_X3A_X3Cbr_X3E_X3Cb_X3E_X25s_X2D_X25s_X2D_X25s_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Chr_X3E),
    /* K12  */  be_const_int(3),
    /* K13  */  be_const_int(2147483647),
    /* K14  */  be_nested_str_weak(_X3Cdiv_X3E_X3Ccenter_X3E),
    /* K15  */  be_nested_str_weak(compute_qrcode_content),
    /* K16  */  be_nested_str_weak(show_qrcode),
    /* K17  */  be_nested_str_weak(_X3Cp_X3E_X20_X25s_X3C_X2Fp_X3E),
    /* K18  */  be_nested_str_weak(_X3C_X2Fdiv_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_commissioning_info),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
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
      0x8C180307,  //  0028  GETMET	R6	R1	K7
      0x5820000E,  //  0029  LDCONST	R8	K14
      0x7C180400,  //  002A  CALL	R6	2
      0x88180102,  //  002B  GETMBR	R6	R0	K2
      0x8C180D0F,  //  002C  GETMET	R6	R6	K15
      0x7C180200,  //  002D  CALL	R6	1
      0x8C1C0110,  //  002E  GETMET	R7	R0	K16
      0x5C240C00,  //  002F  MOVE	R9	R6
      0x7C1C0400,  //  0030  CALL	R7	2
      0x8C1C0307,  //  0031  GETMET	R7	R1	K7
      0x8C240508,  //  0032  GETMET	R9	R2	K8
      0x582C0011,  //  0033  LDCONST	R11	K17
      0x5C300C00,  //  0034  MOVE	R12	R6
      0x7C240600,  //  0035  CALL	R9	3
      0x7C1C0400,  //  0036  CALL	R7	2
      0x8C1C0307,  //  0037  GETMET	R7	R1	K7
      0x58240012,  //  0038  LDCONST	R9	K18
      0x7C1C0400,  //  0039  CALL	R7	2
      0x80000000,  //  003A  RET	0
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
** Solidified function: show_plugins_configuration
********************************************************************/
be_local_closure(Matter_UI_show_plugins_configuration,   /* name */
  be_nested_proto(
    26,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[59]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(content_send),
    /* K4   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BCurrent_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K5   */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3Cp_X3E_X3Cb_X3ELocal_X20sensors_X20and_X20devices_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X3E_X23_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X3EParameter_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(plugins_config),
    /* K8   */  be_nested_str_weak(remove),
    /* K9   */  be_nested_str_weak(0),
    /* K10  */  be_nested_str_weak(k2l_num),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(find),
    /* K13  */  be_nested_str_weak(type),
    /* K14  */  be_const_int(1),
    /* K15  */  be_nested_str_weak(http_),
    /* K16  */  be_nested_str_weak(plugins_classes),
    /* K17  */  be_nested_str_weak(),
    /* K18  */  be_nested_str_weak(ui_conf_to_string),
    /* K19  */  be_nested_str_weak(format),
    /* K20  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X25i_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K21  */  be_nested_str_weak(_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K22  */  be_nested_str_weak(plugin_name),
    /* K23  */  be_nested_str_weak(_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X25i_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X25s_X27_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K24  */  be_nested_str_weak(html_escape),
    /* K25  */  be_nested_str_weak(_X3Ctd_X20style_X3D_X27text_X2Dalign_X3Acenter_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27del_X25i_X27_X20style_X3D_X27background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3B_X27_X20onclick_X3D_X22return_X20confirm_X28_X27Confirm_X20removing_X20endpoint_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K26  */  be_nested_str_weak(_X3C_X2Ftable_X3E),
    /* K27  */  be_nested_str_weak(_X3Cp_X3E_X26lt_X3Bnone_X26gt_X3B_X3C_X2Fp_X3E),
    /* K28  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K29  */  be_nested_str_weak(url),
    /* K30  */  be_nested_str_weak(push),
    /* K31  */  be_nested_str_weak(stop_iteration),
    /* K32  */  be_nested_str_weak(sort_distinct),
    /* K33  */  be_nested_str_weak(tasmota),
    /* K34  */  be_nested_str_weak(log),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20remotes_X3A_X20),
    /* K36  */  be_const_int(3),
    /* K37  */  be_nested_str_weak(_X26_X23x1F517_X3B_X20_X3Ca_X20target_X3D_X27_blank_X27_X20href_X3D_X22http_X3A_X2F_X2F_X25s_X2F_X22_X3E_X25s_X3C_X2Fa_X3E),
    /* K38  */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K39  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2725_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X27115_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2715_X27_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K40  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2722_X27_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X25i_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K41  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X27115_X27_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X25s_X3C_X2Fb_X3E_X3C_X2Fselect_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K42  */  be_nested_str_weak(_X3Ctd_X20width_X3D_X2715_X27_X20style_X3D_X27text_X2Dalign_X3Acenter_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27del_X25i_X27_X20style_X3D_X27background_X3Anone_X3Bborder_X3Anone_X3Bline_X2Dheight_X3A1_X3B_X27_X20onclick_X3D_X22return_X20confirm_X28_X27Confirm_X20removing_X20endpoint_X27_X29_X22_X3E_X26_X23128293_X3B_X3C_X2Fbutton_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K43  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K44  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27config_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X20configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    /* K45  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BAdd_X20to_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K46  */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3EAdd_X20local_X20sensor_X20or_X20device_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E_X3Ctr_X3E_X3Ctd_X20width_X3D_X27145_X27_X3EType_X3C_X2Ftd_X3E_X3Ctd_X3EParameter_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K47  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cselect_X20name_X3D_X27pi_X27_X3E),
    /* K48  */  be_nested_str_weak(plugin_option),
    /* K49  */  be_nested_str_weak(_CLASSES_TYPES),
    /* K50  */  be_nested_str_weak(_X3C_X2Fselect_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K51  */  be_nested_str_weak(_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X27_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E),
    /* K52  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K53  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27addep_X27_X20class_X3D_X27button_X20bgrn_X27_X3ECreate_X20new_X20endpoint_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K54  */  be_nested_str_weak(_X3Chr_X3E_X3Cp_X3E_X3Cb_X3EAdd_X20Remote_X20Tasmota_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Cform_X20action_X3D_X27_X2Fmatteradd_X27_X20method_X3D_X27get_X27_X3E_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K55  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2730_X27_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3Ehttp_X3A_X2F_X2F_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27url_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X27_X20required_X20placeholder_X3D_X27IP_X20or_X20domain_X27_X3E_X3C_X2Ftd_X3E_X3Ctd_X20width_X3D_X2710_X27_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cb_X3E_X2F_X3C_X2Fb_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E_X3C_X2Ftr_X3E_X3C_X2Ftable_X3E),
    /* K56  */  be_nested_str_weak(_X3Cbutton_X20class_X3D_X27button_X20bgrn_X27_X3EAuto_X2Dconfigure_X20remote_X20Tasmota_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3Chr_X3E),
    /* K57  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20RESET_X20the_X20configuration_X20to_X20the_X20default_X2E_X20You_X20will_X20need_X20to_X20associate_X20again_X2E_X22_X29_X3B_X27_X3E_X3Cbutton_X20name_X3D_X27auto_X27_X20class_X3D_X27button_X20bred_X27_X3EReset_X20all_X20and_X20Auto_X2Ddiscover_X3C_X2Fbutton_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Fform_X3E),
    /* K58  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E),
    }),
    be_str_weak(show_plugins_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[311]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x8C100303,  //  0003  GETMET	R4	R1	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x7C100400,  //  0005  CALL	R4	2
      0x8C100303,  //  0006  GETMET	R4	R1	K3
      0x58180005,  //  0007  LDCONST	R6	K5
      0x7C100400,  //  0008  CALL	R4	2
      0x88100106,  //  0009  GETMBR	R4	R0	K6
      0x88100907,  //  000A  GETMBR	R4	R4	K7
      0x8C100908,  //  000B  GETMET	R4	R4	K8
      0x58180009,  //  000C  LDCONST	R6	K9
      0x7C100400,  //  000D  CALL	R4	2
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x8C10090A,  //  000F  GETMET	R4	R4	K10
      0x88180106,  //  0010  GETMBR	R6	R0	K6
      0x88180D07,  //  0011  GETMBR	R6	R6	K7
      0x7C100400,  //  0012  CALL	R4	2
      0x5814000B,  //  0013  LDCONST	R5	K11
      0x50180000,  //  0014  LDBOOL	R6	0	0
      0x601C000C,  //  0015  GETGBL	R7	G12
      0x5C200800,  //  0016  MOVE	R8	R4
      0x7C1C0200,  //  0017  CALL	R7	1
      0x141C0A07,  //  0018  LT	R7	R5	R7
      0x781E0047,  //  0019  JMPF	R7	#0062
      0x941C0805,  //  001A  GETIDX	R7	R4	R5
      0x88200106,  //  001B  GETMBR	R8	R0	K6
      0x88201107,  //  001C  GETMBR	R8	R8	K7
      0x8C20110C,  //  001D  GETMET	R8	R8	K12
      0x60280008,  //  001E  GETGBL	R10	G8
      0x5C2C0E00,  //  001F  MOVE	R11	R7
      0x7C280200,  //  0020  CALL	R10	1
      0x7C200400,  //  0021  CALL	R8	2
      0x8C24110C,  //  0022  GETMET	R9	R8	K12
      0x582C000D,  //  0023  LDCONST	R11	K13
      0x7C240400,  //  0024  CALL	R9	2
      0x5C281200,  //  0025  MOVE	R10	R9
      0x742A0001,  //  0026  JMPT	R10	#0029
      0x00140B0E,  //  0027  ADD	R5	R5	K14
      0x7001FFEB,  //  0028  JMP		#0015
      0x8C28050C,  //  0029  GETMET	R10	R2	K12
      0x5C301200,  //  002A  MOVE	R12	R9
      0x5834000F,  //  002B  LDCONST	R13	K15
      0x7C280600,  //  002C  CALL	R10	3
      0x1C28150B,  //  002D  EQ	R10	R10	K11
      0x782A0001,  //  002E  JMPF	R10	#0031
      0x00140B0E,  //  002F  ADD	R5	R5	K14
      0x7001FFE3,  //  0030  JMP		#0015
      0x88280106,  //  0031  GETMBR	R10	R0	K6
      0x88281510,  //  0032  GETMBR	R10	R10	K16
      0x8C28150C,  //  0033  GETMET	R10	R10	K12
      0x5C301200,  //  0034  MOVE	R12	R9
      0x7C280400,  //  0035  CALL	R10	2
      0x582C0011,  //  0036  LDCONST	R11	K17
      0x4C300000,  //  0037  LDNIL	R12
      0x2030140C,  //  0038  NE	R12	R10	R12
      0x78320004,  //  0039  JMPF	R12	#003F
      0x8C301512,  //  003A  GETMET	R12	R10	K18
      0x5C381400,  //  003B  MOVE	R14	R10
      0x5C3C1000,  //  003C  MOVE	R15	R8
      0x7C300600,  //  003D  CALL	R12	3
      0x5C2C1800,  //  003E  MOVE	R11	R12
      0x50180200,  //  003F  LDBOOL	R6	1	0
      0x8C300303,  //  0040  GETMET	R12	R1	K3
      0x8C380513,  //  0041  GETMET	R14	R2	K19
      0x58400014,  //  0042  LDCONST	R16	K20
      0x5C440E00,  //  0043  MOVE	R17	R7
      0x7C380600,  //  0044  CALL	R14	3
      0x7C300400,  //  0045  CALL	R12	2
      0x8C300303,  //  0046  GETMET	R12	R1	K3
      0x8C380513,  //  0047  GETMET	R14	R2	K19
      0x58400015,  //  0048  LDCONST	R16	K21
      0x8C440116,  //  0049  GETMET	R17	R0	K22
      0x8C4C110C,  //  004A  GETMET	R19	R8	K12
      0x5854000D,  //  004B  LDCONST	R21	K13
      0x58580011,  //  004C  LDCONST	R22	K17
      0x7C4C0600,  //  004D  CALL	R19	3
      0x7C440400,  //  004E  CALL	R17	2
      0x7C380600,  //  004F  CALL	R14	3
      0x7C300400,  //  0050  CALL	R12	2
      0x8C300303,  //  0051  GETMET	R12	R1	K3
      0x8C380513,  //  0052  GETMET	R14	R2	K19
      0x58400017,  //  0053  LDCONST	R16	K23
      0x5C440E00,  //  0054  MOVE	R17	R7
      0x8C480318,  //  0055  GETMET	R18	R1	K24
      0x5C501600,  //  0056  MOVE	R20	R11
      0x7C480400,  //  0057  CALL	R18	2
      0x7C380800,  //  0058  CALL	R14	4
      0x7C300400,  //  0059  CALL	R12	2
      0x8C300303,  //  005A  GETMET	R12	R1	K3
      0x8C380513,  //  005B  GETMET	R14	R2	K19
      0x58400019,  //  005C  LDCONST	R16	K25
      0x5C440E00,  //  005D  MOVE	R17	R7
      0x7C380600,  //  005E  CALL	R14	3
      0x7C300400,  //  005F  CALL	R12	2
      0x00140B0E,  //  0060  ADD	R5	R5	K14
      0x7001FFB2,  //  0061  JMP		#0015
      0x8C1C0303,  //  0062  GETMET	R7	R1	K3
      0x5824001A,  //  0063  LDCONST	R9	K26
      0x7C1C0400,  //  0064  CALL	R7	2
      0x5C1C0C00,  //  0065  MOVE	R7	R6
      0x741E0002,  //  0066  JMPT	R7	#006A
      0x8C1C0303,  //  0067  GETMET	R7	R1	K3
      0x5824001B,  //  0068  LDCONST	R9	K27
      0x7C1C0400,  //  0069  CALL	R7	2
      0x8C1C0303,  //  006A  GETMET	R7	R1	K3
      0x5824001C,  //  006B  LDCONST	R9	K28
      0x7C1C0400,  //  006C  CALL	R7	2
      0x601C0012,  //  006D  GETGBL	R7	G18
      0x7C1C0000,  //  006E  CALL	R7	0
      0x60200010,  //  006F  GETGBL	R8	G16
      0x88240106,  //  0070  GETMBR	R9	R0	K6
      0x88241307,  //  0071  GETMBR	R9	R9	K7
      0x7C200200,  //  0072  CALL	R8	1
      0xA802000B,  //  0073  EXBLK	0	#0080
      0x5C241000,  //  0074  MOVE	R9	R8
      0x7C240000,  //  0075  CALL	R9	0
      0x8C28130C,  //  0076  GETMET	R10	R9	K12
      0x5830001D,  //  0077  LDCONST	R12	K29
      0x7C280400,  //  0078  CALL	R10	2
      0x4C2C0000,  //  0079  LDNIL	R11
      0x202C140B,  //  007A  NE	R11	R10	R11
      0x782E0002,  //  007B  JMPF	R11	#007F
      0x8C2C0F1E,  //  007C  GETMET	R11	R7	K30
      0x5C341400,  //  007D  MOVE	R13	R10
      0x7C2C0400,  //  007E  CALL	R11	2
      0x7001FFF3,  //  007F  JMP		#0074
      0x5820001F,  //  0080  LDCONST	R8	K31
      0xAC200200,  //  0081  CATCH	R8	1	0
      0xB0080000,  //  0082  RAISE	2	R0	R0
      0x88200106,  //  0083  GETMBR	R8	R0	K6
      0x8C201120,  //  0084  GETMET	R8	R8	K32
      0x5C280E00,  //  0085  MOVE	R10	R7
      0x7C200400,  //  0086  CALL	R8	2
      0xB8224200,  //  0087  GETNGBL	R8	K33
      0x8C201122,  //  0088  GETMET	R8	R8	K34
      0x60280008,  //  0089  GETGBL	R10	G8
      0x5C2C0E00,  //  008A  MOVE	R11	R7
      0x7C280200,  //  008B  CALL	R10	1
      0x002A460A,  //  008C  ADD	R10	K35	R10
      0x582C0024,  //  008D  LDCONST	R11	K36
      0x7C200600,  //  008E  CALL	R8	3
      0x60200010,  //  008F  GETGBL	R8	G16
      0x5C240E00,  //  0090  MOVE	R9	R7
      0x7C200200,  //  0091  CALL	R8	1
      0xA802006C,  //  0092  EXBLK	0	#0100
      0x5C241000,  //  0093  MOVE	R9	R8
      0x7C240000,  //  0094  CALL	R9	0
      0x8C280303,  //  0095  GETMET	R10	R1	K3
      0x8C300513,  //  0096  GETMET	R12	R2	K19
      0x58380025,  //  0097  LDCONST	R14	K37
      0x8C3C0318,  //  0098  GETMET	R15	R1	K24
      0x5C441200,  //  0099  MOVE	R17	R9
      0x7C3C0400,  //  009A  CALL	R15	2
      0x8C400318,  //  009B  GETMET	R16	R1	K24
      0x5C481200,  //  009C  MOVE	R18	R9
      0x7C400400,  //  009D  CALL	R16	2
      0x7C300800,  //  009E  CALL	R12	4
      0x7C280400,  //  009F  CALL	R10	2
      0x8C280303,  //  00A0  GETMET	R10	R1	K3
      0x58300026,  //  00A1  LDCONST	R12	K38
      0x7C280400,  //  00A2  CALL	R10	2
      0x8C280303,  //  00A3  GETMET	R10	R1	K3
      0x58300027,  //  00A4  LDCONST	R12	K39
      0x7C280400,  //  00A5  CALL	R10	2
      0x50180000,  //  00A6  LDBOOL	R6	0	0
      0x5814000B,  //  00A7  LDCONST	R5	K11
      0x6028000C,  //  00A8  GETGBL	R10	G12
      0x5C2C0800,  //  00A9  MOVE	R11	R4
      0x7C280200,  //  00AA  CALL	R10	1
      0x14280A0A,  //  00AB  LT	R10	R5	R10
      0x782A004E,  //  00AC  JMPF	R10	#00FC
      0x94280805,  //  00AD  GETIDX	R10	R4	R5
      0x882C0106,  //  00AE  GETMBR	R11	R0	K6
      0x882C1707,  //  00AF  GETMBR	R11	R11	K7
      0x8C2C170C,  //  00B0  GETMET	R11	R11	K12
      0x60340008,  //  00B1  GETGBL	R13	G8
      0x5C381400,  //  00B2  MOVE	R14	R10
      0x7C340200,  //  00B3  CALL	R13	1
      0x7C2C0400,  //  00B4  CALL	R11	2
      0x8C30170C,  //  00B5  GETMET	R12	R11	K12
      0x5838000D,  //  00B6  LDCONST	R14	K13
      0x7C300400,  //  00B7  CALL	R12	2
      0x5C341800,  //  00B8  MOVE	R13	R12
      0x74360001,  //  00B9  JMPT	R13	#00BC
      0x00140B0E,  //  00BA  ADD	R5	R5	K14
      0x7001FFEB,  //  00BB  JMP		#00A8
      0x8C34050C,  //  00BC  GETMET	R13	R2	K12
      0x5C3C1800,  //  00BD  MOVE	R15	R12
      0x5840000F,  //  00BE  LDCONST	R16	K15
      0x7C340600,  //  00BF  CALL	R13	3
      0x20341B0B,  //  00C0  NE	R13	R13	K11
      0x78360001,  //  00C1  JMPF	R13	#00C4
      0x00140B0E,  //  00C2  ADD	R5	R5	K14
      0x7001FFE3,  //  00C3  JMP		#00A8
      0x8C34170C,  //  00C4  GETMET	R13	R11	K12
      0x583C001D,  //  00C5  LDCONST	R15	K29
      0x7C340400,  //  00C6  CALL	R13	2
      0x20341A09,  //  00C7  NE	R13	R13	R9
      0x78360001,  //  00C8  JMPF	R13	#00CB
      0x00140B0E,  //  00C9  ADD	R5	R5	K14
      0x7001FFDC,  //  00CA  JMP		#00A8
      0x88340106,  //  00CB  GETMBR	R13	R0	K6
      0x88341B10,  //  00CC  GETMBR	R13	R13	K16
      0x8C341B0C,  //  00CD  GETMET	R13	R13	K12
      0x5C3C1800,  //  00CE  MOVE	R15	R12
      0x7C340400,  //  00CF  CALL	R13	2
      0x58380011,  //  00D0  LDCONST	R14	K17
      0x4C3C0000,  //  00D1  LDNIL	R15
      0x203C1A0F,  //  00D2  NE	R15	R13	R15
      0x783E0004,  //  00D3  JMPF	R15	#00D9
      0x8C3C1B12,  //  00D4  GETMET	R15	R13	K18
      0x5C441A00,  //  00D5  MOVE	R17	R13
      0x5C481600,  //  00D6  MOVE	R18	R11
      0x7C3C0600,  //  00D7  CALL	R15	3
      0x5C381E00,  //  00D8  MOVE	R14	R15
      0x50180200,  //  00D9  LDBOOL	R6	1	0
      0x8C3C0303,  //  00DA  GETMET	R15	R1	K3
      0x8C440513,  //  00DB  GETMET	R17	R2	K19
      0x584C0028,  //  00DC  LDCONST	R19	K40
      0x5C501400,  //  00DD  MOVE	R20	R10
      0x7C440600,  //  00DE  CALL	R17	3
      0x7C3C0400,  //  00DF  CALL	R15	2
      0x8C3C0303,  //  00E0  GETMET	R15	R1	K3
      0x8C440513,  //  00E1  GETMET	R17	R2	K19
      0x584C0029,  //  00E2  LDCONST	R19	K41
      0x8C500116,  //  00E3  GETMET	R20	R0	K22
      0x8C58170C,  //  00E4  GETMET	R22	R11	K12
      0x5860000D,  //  00E5  LDCONST	R24	K13
      0x58640011,  //  00E6  LDCONST	R25	K17
      0x7C580600,  //  00E7  CALL	R22	3
      0x7C500400,  //  00E8  CALL	R20	2
      0x7C440600,  //  00E9  CALL	R17	3
      0x7C3C0400,  //  00EA  CALL	R15	2
      0x8C3C0303,  //  00EB  GETMET	R15	R1	K3
      0x8C440513,  //  00EC  GETMET	R17	R2	K19
      0x584C0017,  //  00ED  LDCONST	R19	K23
      0x5C501400,  //  00EE  MOVE	R20	R10
      0x8C540318,  //  00EF  GETMET	R21	R1	K24
      0x5C5C1C00,  //  00F0  MOVE	R23	R14
      0x7C540400,  //  00F1  CALL	R21	2
      0x7C440800,  //  00F2  CALL	R17	4
      0x7C3C0400,  //  00F3  CALL	R15	2
      0x8C3C0303,  //  00F4  GETMET	R15	R1	K3
      0x8C440513,  //  00F5  GETMET	R17	R2	K19
      0x584C002A,  //  00F6  LDCONST	R19	K42
      0x5C501400,  //  00F7  MOVE	R20	R10
      0x7C440600,  //  00F8  CALL	R17	3
      0x7C3C0400,  //  00F9  CALL	R15	2
      0x00140B0E,  //  00FA  ADD	R5	R5	K14
      0x7001FFAB,  //  00FB  JMP		#00A8
      0x8C280303,  //  00FC  GETMET	R10	R1	K3
      0x5830002B,  //  00FD  LDCONST	R12	K43
      0x7C280400,  //  00FE  CALL	R10	2
      0x7001FF92,  //  00FF  JMP		#0093
      0x5820001F,  //  0100  LDCONST	R8	K31
      0xAC200200,  //  0101  CATCH	R8	1	0
      0xB0080000,  //  0102  RAISE	2	R0	R0
      0x5C200C00,  //  0103  MOVE	R8	R6
      0x74220002,  //  0104  JMPT	R8	#0108
      0x8C200303,  //  0105  GETMET	R8	R1	K3
      0x5828001B,  //  0106  LDCONST	R10	K27
      0x7C200400,  //  0107  CALL	R8	2
      0x8C200303,  //  0108  GETMET	R8	R1	K3
      0x5828002C,  //  0109  LDCONST	R10	K44
      0x7C200400,  //  010A  CALL	R8	2
      0x8C200303,  //  010B  GETMET	R8	R1	K3
      0x5828002D,  //  010C  LDCONST	R10	K45
      0x7C200400,  //  010D  CALL	R8	2
      0x8C200303,  //  010E  GETMET	R8	R1	K3
      0x5828002E,  //  010F  LDCONST	R10	K46
      0x7C200400,  //  0110  CALL	R8	2
      0x8C200303,  //  0111  GETMET	R8	R1	K3
      0x5828002F,  //  0112  LDCONST	R10	K47
      0x7C200400,  //  0113  CALL	R8	2
      0x8C200130,  //  0114  GETMET	R8	R0	K48
      0x58280011,  //  0115  LDCONST	R10	K17
      0x882C0131,  //  0116  GETMBR	R11	R0	K49
      0x7C200600,  //  0117  CALL	R8	3
      0x8C200303,  //  0118  GETMET	R8	R1	K3
      0x58280032,  //  0119  LDCONST	R10	K50
      0x7C200400,  //  011A  CALL	R8	2
      0x8C200303,  //  011B  GETMET	R8	R1	K3
      0x58280033,  //  011C  LDCONST	R10	K51
      0x7C200400,  //  011D  CALL	R8	2
      0x8C200303,  //  011E  GETMET	R8	R1	K3
      0x58280034,  //  011F  LDCONST	R10	K52
      0x7C200400,  //  0120  CALL	R8	2
      0x8C200303,  //  0121  GETMET	R8	R1	K3
      0x58280035,  //  0122  LDCONST	R10	K53
      0x7C200400,  //  0123  CALL	R8	2
      0x8C200303,  //  0124  GETMET	R8	R1	K3
      0x58280036,  //  0125  LDCONST	R10	K54
      0x7C200400,  //  0126  CALL	R8	2
      0x8C200303,  //  0127  GETMET	R8	R1	K3
      0x58280037,  //  0128  LDCONST	R10	K55
      0x7C200400,  //  0129  CALL	R8	2
      0x8C200303,  //  012A  GETMET	R8	R1	K3
      0x58280034,  //  012B  LDCONST	R10	K52
      0x7C200400,  //  012C  CALL	R8	2
      0x8C200303,  //  012D  GETMET	R8	R1	K3
      0x58280038,  //  012E  LDCONST	R10	K56
      0x7C200400,  //  012F  CALL	R8	2
      0x8C200303,  //  0130  GETMET	R8	R1	K3
      0x58280039,  //  0131  LDCONST	R10	K57
      0x7C200400,  //  0132  CALL	R8	2
      0x8C200303,  //  0133  GETMET	R8	R1	K3
      0x5828003A,  //  0134  LDCONST	R10	K58
      0x7C200400,  //  0135  CALL	R8	2
      0x80000000,  //  0136  RET	0
    })
  )
);
/*******************************************************************/


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
** Solidified function: web_sensor
********************************************************************/
be_local_closure(Matter_UI_web_sensor,   /* name */
  be_nested_proto(
    12,                          /* nstack */
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
    /* K2   */  be_nested_str_weak(matter_enabled),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(sessions),
    /* K5   */  be_nested_str_weak(count_active_fabrics),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(content_send),
    /* K8   */  be_nested_str_weak(format),
    /* K9   */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27text_X2Dalign_X3Aright_X3Bfont_X2Dsize_X3A11px_X3Bcolor_X3A_X23aaa_X3Bpadding_X3A0px_X3B_X27_X3E_X25s_X3C_X2Fdiv_X3E),
    /* K10  */  be_nested_str_weak(Matter_X3A_X20No_X20active_X20association),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(Matter_X3A_X20),
    /* K13  */  be_nested_str_weak(_X20active_X20association),
    /* K14  */  be_nested_str_weak(s),
    /* K15  */  be_nested_str_weak(),
    /* K16  */  be_nested_str_weak(show_bridge_status),
    /* K17  */  be_nested_str_weak(is_root_commissioning_open),
    /* K18  */  be_nested_str_weak(show_commissioning_info),
    }),
    be_str_weak(web_sensor),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0102,  //  0002  GETMET	R3	R0	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x780E0024,  //  0004  JMPF	R3	#002A
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x880C0704,  //  0006  GETMBR	R3	R3	K4
      0x8C0C0705,  //  0007  GETMET	R3	R3	K5
      0x7C0C0200,  //  0008  CALL	R3	1
      0x1C100706,  //  0009  EQ	R4	R3	K6
      0x78120006,  //  000A  JMPF	R4	#0012
      0x8C100307,  //  000B  GETMET	R4	R1	K7
      0x8C180508,  //  000C  GETMET	R6	R2	K8
      0x58200009,  //  000D  LDCONST	R8	K9
      0x5824000A,  //  000E  LDCONST	R9	K10
      0x7C180600,  //  000F  CALL	R6	3
      0x7C100400,  //  0010  CALL	R4	2
      0x7002000F,  //  0011  JMP		#0022
      0x2410070B,  //  0012  GT	R4	R3	K11
      0x8C140307,  //  0013  GETMET	R5	R1	K7
      0x8C1C0508,  //  0014  GETMET	R7	R2	K8
      0x58240009,  //  0015  LDCONST	R9	K9
      0x60280008,  //  0016  GETGBL	R10	G8
      0x5C2C0600,  //  0017  MOVE	R11	R3
      0x7C280200,  //  0018  CALL	R10	1
      0x002A180A,  //  0019  ADD	R10	K12	R10
      0x0028150D,  //  001A  ADD	R10	R10	K13
      0x78120001,  //  001B  JMPF	R4	#001E
      0x582C000E,  //  001C  LDCONST	R11	K14
      0x70020000,  //  001D  JMP		#001F
      0x582C000F,  //  001E  LDCONST	R11	K15
      0x0028140B,  //  001F  ADD	R10	R10	R11
      0x7C1C0600,  //  0020  CALL	R7	3
      0x7C140400,  //  0021  CALL	R5	2
      0x8C100110,  //  0022  GETMET	R4	R0	K16
      0x7C100200,  //  0023  CALL	R4	1
      0x88100103,  //  0024  GETMBR	R4	R0	K3
      0x8C100911,  //  0025  GETMET	R4	R4	K17
      0x7C100200,  //  0026  CALL	R4	1
      0x78120001,  //  0027  JMPF	R4	#002A
      0x8C100112,  //  0028  GETMET	R4	R0	K18
      0x7C100200,  //  0029  CALL	R4	1
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_fabric_info
********************************************************************/
be_local_closure(Matter_UI_show_fabric_info,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BFabrics_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3Cp_X3EAssociated_X20fabrics_X3A_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(sessions),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3ENone_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K8   */  be_nested_str_weak(fabrics),
    /* K9   */  be_nested_str_weak(persistables),
    /* K10  */  be_nested_str_weak(_X3Chr_X3E),
    /* K11  */  be_nested_str_weak(fabric_label),
    /* K12  */  be_nested_str_weak(_X3CNo_X20label_X3E),
    /* K13  */  be_nested_str_weak(html_escape),
    /* K14  */  be_nested_str_weak(format),
    /* K15  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3B_X23_X25i_X20_X25s_X3C_X2Fb_X3E_X20_X28_X25s_X29_X26nbsp_X3B_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K16  */  be_nested_str_weak(get_fabric_index),
    /* K17  */  be_nested_str_weak(get_admin_vendor_name),
    /* K18  */  be_nested_str_weak(get_fabric_id),
    /* K19  */  be_nested_str_weak(copy),
    /* K20  */  be_nested_str_weak(reverse),
    /* K21  */  be_nested_str_weak(get_device_id),
    /* K22  */  be_nested_str_weak(Fabric_X3A_X20_X25s_X3Cbr_X3E),
    /* K23  */  be_nested_str_weak(tohex),
    /* K24  */  be_nested_str_weak(Device_X3A_X20_X25s_X3Cbr_X3E_X26nbsp_X3B),
    /* K25  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22Are_X20you_X20sure_X3F_X22_X29_X3B_X27_X3E),
    /* K26  */  be_nested_str_weak(_X3Cinput_X20name_X3D_X27del_fabric_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K27  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27del_X27_X20class_X3D_X27button_X20bgrn_X27_X3EDelete_X20Fabric_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E),
    /* K28  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K29  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(show_fabric_info),
    &be_const_str_solidified,
    ( &(const binstruction[101]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100104,  //  0006  GETMBR	R4	R0	K4
      0x88100905,  //  0007  GETMBR	R4	R4	K5
      0x88100905,  //  0008  GETMBR	R4	R4	K5
      0x7C0C0200,  //  0009  CALL	R3	1
      0x1C0C0706,  //  000A  EQ	R3	R3	K6
      0x780E0003,  //  000B  JMPF	R3	#0010
      0x8C0C0302,  //  000C  GETMET	R3	R1	K2
      0x58140007,  //  000D  LDCONST	R5	K7
      0x7C0C0400,  //  000E  CALL	R3	2
      0x70020050,  //  000F  JMP		#0061
      0x500C0200,  //  0010  LDBOOL	R3	1	0
      0x60100010,  //  0011  GETGBL	R4	G16
      0x88140104,  //  0012  GETMBR	R5	R0	K4
      0x88140B05,  //  0013  GETMBR	R5	R5	K5
      0x88140B08,  //  0014  GETMBR	R5	R5	K8
      0x8C140B09,  //  0015  GETMET	R5	R5	K9
      0x7C140200,  //  0016  CALL	R5	1
      0x7C100200,  //  0017  CALL	R4	1
      0xA8020044,  //  0018  EXBLK	0	#005E
      0x5C140800,  //  0019  MOVE	R5	R4
      0x7C140000,  //  001A  CALL	R5	0
      0x5C180600,  //  001B  MOVE	R6	R3
      0x741A0002,  //  001C  JMPT	R6	#0020
      0x8C180302,  //  001D  GETMET	R6	R1	K2
      0x5820000A,  //  001E  LDCONST	R8	K10
      0x7C180400,  //  001F  CALL	R6	2
      0x500C0000,  //  0020  LDBOOL	R3	0	0
      0x88180B0B,  //  0021  GETMBR	R6	R5	K11
      0x5C1C0C00,  //  0022  MOVE	R7	R6
      0x741E0000,  //  0023  JMPT	R7	#0025
      0x5818000C,  //  0024  LDCONST	R6	K12
      0x8C1C030D,  //  0025  GETMET	R7	R1	K13
      0x5C240C00,  //  0026  MOVE	R9	R6
      0x7C1C0400,  //  0027  CALL	R7	2
      0x5C180E00,  //  0028  MOVE	R6	R7
      0x8C1C0302,  //  0029  GETMET	R7	R1	K2
      0x8C24050E,  //  002A  GETMET	R9	R2	K14
      0x582C000F,  //  002B  LDCONST	R11	K15
      0x8C300B10,  //  002C  GETMET	R12	R5	K16
      0x7C300200,  //  002D  CALL	R12	1
      0x5C340C00,  //  002E  MOVE	R13	R6
      0x8C380B11,  //  002F  GETMET	R14	R5	K17
      0x7C380200,  //  0030  CALL	R14	1
      0x7C240A00,  //  0031  CALL	R9	5
      0x7C1C0400,  //  0032  CALL	R7	2
      0x8C1C0B12,  //  0033  GETMET	R7	R5	K18
      0x7C1C0200,  //  0034  CALL	R7	1
      0x8C1C0F13,  //  0035  GETMET	R7	R7	K19
      0x7C1C0200,  //  0036  CALL	R7	1
      0x8C1C0F14,  //  0037  GETMET	R7	R7	K20
      0x7C1C0200,  //  0038  CALL	R7	1
      0x8C200B15,  //  0039  GETMET	R8	R5	K21
      0x7C200200,  //  003A  CALL	R8	1
      0x8C201113,  //  003B  GETMET	R8	R8	K19
      0x7C200200,  //  003C  CALL	R8	1
      0x8C201114,  //  003D  GETMET	R8	R8	K20
      0x7C200200,  //  003E  CALL	R8	1
      0x8C240302,  //  003F  GETMET	R9	R1	K2
      0x8C2C050E,  //  0040  GETMET	R11	R2	K14
      0x58340016,  //  0041  LDCONST	R13	K22
      0x8C380F17,  //  0042  GETMET	R14	R7	K23
      0x7C380200,  //  0043  CALL	R14	1
      0x7C2C0600,  //  0044  CALL	R11	3
      0x7C240400,  //  0045  CALL	R9	2
      0x8C240302,  //  0046  GETMET	R9	R1	K2
      0x8C2C050E,  //  0047  GETMET	R11	R2	K14
      0x58340018,  //  0048  LDCONST	R13	K24
      0x8C381117,  //  0049  GETMET	R14	R8	K23
      0x7C380200,  //  004A  CALL	R14	1
      0x7C2C0600,  //  004B  CALL	R11	3
      0x7C240400,  //  004C  CALL	R9	2
      0x8C240302,  //  004D  GETMET	R9	R1	K2
      0x582C0019,  //  004E  LDCONST	R11	K25
      0x7C240400,  //  004F  CALL	R9	2
      0x8C240302,  //  0050  GETMET	R9	R1	K2
      0x8C2C050E,  //  0051  GETMET	R11	R2	K14
      0x5834001A,  //  0052  LDCONST	R13	K26
      0x8C380B10,  //  0053  GETMET	R14	R5	K16
      0x7C380200,  //  0054  CALL	R14	1
      0x7C2C0600,  //  0055  CALL	R11	3
      0x7C240400,  //  0056  CALL	R9	2
      0x8C240302,  //  0057  GETMET	R9	R1	K2
      0x582C001B,  //  0058  LDCONST	R11	K27
      0x7C240400,  //  0059  CALL	R9	2
      0x8C240302,  //  005A  GETMET	R9	R1	K2
      0x582C001C,  //  005B  LDCONST	R11	K28
      0x7C240400,  //  005C  CALL	R9	2
      0x7001FFBA,  //  005D  JMP		#0019
      0x5810001D,  //  005E  LDCONST	R4	K29
      0xAC100200,  //  005F  CATCH	R4	1	0
      0xB0080000,  //  0060  RAISE	2	R0	R0
      0x8C0C0302,  //  0061  GETMET	R3	R1	K2
      0x5814001C,  //  0062  LDCONST	R5	K28
      0x7C0C0400,  //  0063  CALL	R3	2
      0x80000000,  //  0064  RET	0
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(check_privileged_access),
    /* K3   */  be_nested_str_weak(content_start),
    /* K4   */  be_nested_str_weak(Matter),
    /* K5   */  be_nested_str_weak(content_send_style),
    /* K6   */  be_nested_str_weak(show_enable),
    /* K7   */  be_nested_str_weak(matter_enabled),
    /* K8   */  be_nested_str_weak(show_plugins_configuration),
    /* K9   */  be_nested_str_weak(content_send),
    /* K10  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27display_X3A_X20block_X3B_X27_X3E_X3C_X2Fdiv_X3E),
    /* K11  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cform_X20id_X3D_X27butmat_X27_X20style_X3D_X27display_X3A_X20block_X3B_X27_X20action_X3D_X27mattera_X27_X20method_X3D_X27get_X27_X3E_X3Cbutton_X20name_X3D_X27_X27_X3EAdvanced_X20Configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K12  */  be_nested_str_weak(content_button),
    /* K13  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K14  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
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
      0x8C0C0107,  //  000E  GETMET	R3	R0	K7
      0x7C0C0200,  //  000F  CALL	R3	1
      0x780E0001,  //  0010  JMPF	R3	#0013
      0x8C0C0108,  //  0011  GETMET	R3	R0	K8
      0x7C0C0200,  //  0012  CALL	R3	1
      0x8C0C0309,  //  0013  GETMET	R3	R1	K9
      0x5814000A,  //  0014  LDCONST	R5	K10
      0x7C0C0400,  //  0015  CALL	R3	2
      0x8C0C0309,  //  0016  GETMET	R3	R1	K9
      0x5814000B,  //  0017  LDCONST	R5	K11
      0x7C0C0400,  //  0018  CALL	R3	2
      0x8C0C030C,  //  0019  GETMET	R3	R1	K12
      0x8814030D,  //  001A  GETMBR	R5	R1	K13
      0x7C0C0400,  //  001B  CALL	R3	2
      0x8C0C030E,  //  001C  GETMET	R3	R1	K14
      0x7C0C0200,  //  001D  CALL	R3	1
      0x80000000,  //  001E  RET	0
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
    be_nested_map(25,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(web_add_config_button, -1), be_const_closure(Matter_UI_web_add_config_button_closure) },
        { be_const_key_weak(page_part_mgr, -1), be_const_closure(Matter_UI_page_part_mgr_closure) },
        { be_const_key_weak(page_part_mgr_adv, -1), be_const_closure(Matter_UI_page_part_mgr_adv_closure) },
        { be_const_key_weak(show_bridge_status, -1), be_const_closure(Matter_UI_show_bridge_status_closure) },
        { be_const_key_weak(plugin_option, -1), be_const_closure(Matter_UI_plugin_option_closure) },
        { be_const_key_weak(init, 8), be_const_closure(Matter_UI_init_closure) },
        { be_const_key_weak(page_part_ctl, 7), be_const_closure(Matter_UI_page_part_ctl_closure) },
        { be_const_key_weak(show_fabric_info, 9), be_const_closure(Matter_UI_show_fabric_info_closure) },
        { be_const_key_weak(web_get_arg, -1), be_const_closure(Matter_UI_web_get_arg_closure) },
        { be_const_key_weak(equal_map, -1), be_const_static_closure(Matter_UI_equal_map_closure) },
        { be_const_key_weak(page_part_mgr_add, -1), be_const_closure(Matter_UI_page_part_mgr_add_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(web_add_handler, 21), be_const_closure(Matter_UI_web_add_handler_closure) },
        { be_const_key_weak(matter_enabled, -1), be_const_closure(Matter_UI_matter_enabled_closure) },
        { be_const_key_weak(show_enable, -1), be_const_closure(Matter_UI_show_enable_closure) },
        { be_const_key_weak(show_commissioning_info, -1), be_const_closure(Matter_UI_show_commissioning_info_closure) },
        { be_const_key_weak(_CLASSES_TYPES2, 18), be_nested_str_weak(_X7Chttp_relay_X7Chttp_light0_X7Chttp_light1_X7Chttp_light2_X7Chttp_light3_X7Chttp_temperature_X7Chttp_pressure_X7Chttp_illuminance_X7Chttp_humidity_X7Chttp_occupancy) },
        { be_const_key_weak(show_remote_autoconf, 13), be_const_closure(Matter_UI_show_remote_autoconf_closure) },
        { be_const_key_weak(show_plugins_configuration, 23), be_const_closure(Matter_UI_show_plugins_configuration_closure) },
        { be_const_key_weak(show_passcode_form, 16), be_const_closure(Matter_UI_show_passcode_form_closure) },
        { be_const_key_weak(_CLASSES_TYPES, -1), be_nested_str_weak(_X7Crelay_X7Clight0_X7Clight1_X7Clight2_X7Clight3_X7Cshutter_X7Cshutter_X2Btilt_X7Ctemperature_X7Cpressure_X7Cilluminance_X7Chumidity_X7Coccupancy_X7Ccontact__X7Conoff) },
        { be_const_key_weak(plugin_name, -1), be_const_closure(Matter_UI_plugin_name_closure) },
        { be_const_key_weak(web_sensor, -1), be_const_closure(Matter_UI_web_sensor_closure) },
        { be_const_key_weak(show_qrcode, -1), be_const_closure(Matter_UI_show_qrcode_closure) },
        { be_const_key_weak(generate_config_from_status, 1), be_const_closure(Matter_UI_generate_config_from_status_closure) },
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
