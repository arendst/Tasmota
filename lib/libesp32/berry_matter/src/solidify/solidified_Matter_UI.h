/* Solidification of Matter_UI.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_UI;

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
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(check_privileged_access),
    /* K3   */  be_nested_str_weak(content_start),
    /* K4   */  be_nested_str_weak(Matter),
    /* K5   */  be_nested_str_weak(content_send_style),
    /* K6   */  be_nested_str_weak(show_enable),
    /* K7   */  be_nested_str_weak(show_passcode_form),
    /* K8   */  be_nested_str_weak(show_plugins_configuration),
    /* K9   */  be_nested_str_weak(show_fabric_info),
    /* K10  */  be_nested_str_weak(content_button),
    /* K11  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K12  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr),
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
      0x8C0C0106,  //  000C  GETMET	R3	R0	K6
      0x7C0C0200,  //  000D  CALL	R3	1
      0x780E0005,  //  000E  JMPF	R3	#0015
      0x8C0C0107,  //  000F  GETMET	R3	R0	K7
      0x7C0C0200,  //  0010  CALL	R3	1
      0x8C0C0108,  //  0011  GETMET	R3	R0	K8
      0x7C0C0200,  //  0012  CALL	R3	1
      0x8C0C0109,  //  0013  GETMET	R3	R0	K9
      0x7C0C0200,  //  0014  CALL	R3	1
      0x8C0C030A,  //  0015  GETMET	R3	R1	K10
      0x8814030B,  //  0016  GETMBR	R5	R1	K11
      0x7C0C0400,  //  0017  CALL	R3	2
      0x8C0C030C,  //  0018  GETMET	R3	R1	K12
      0x7C0C0200,  //  0019  CALL	R3	1
      0x80000000,  //  001A  RET	0
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
** Solidified function: page_part_ctl
********************************************************************/
be_local_closure(Matter_UI_page_part_ctl,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[71]) {     /* constants */
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
    /* K10  */  be_nested_str_weak(format),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20_X2Fmatterc_X20received_X20_X27_X25s_X27_X20command),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(device),
    /* K14  */  be_nested_str_weak(root_passcode),
    /* K15  */  be_nested_str_weak(arg),
    /* K16  */  be_nested_str_weak(root_discriminator),
    /* K17  */  be_nested_str_weak(ipv4only),
    /* K18  */  be_nested_str_weak(ipv4),
    /* K19  */  be_nested_str_weak(on),
    /* K20  */  be_nested_str_weak(save_param),
    /* K21  */  be_nested_str_weak(redirect),
    /* K22  */  be_nested_str_weak(_X2F_X3Frst_X3D),
    /* K23  */  be_nested_str_weak(enable),
    /* K24  */  be_nested_str_weak(cmd),
    /* K25  */  be_nested_str_weak(SetOption),
    /* K26  */  be_nested_str_weak(matter),
    /* K27  */  be_nested_str_weak(MATTER_OPTION),
    /* K28  */  be_nested_str_weak(_X201),
    /* K29  */  be_nested_str_weak(disable),
    /* K30  */  be_nested_str_weak(_X200),
    /* K31  */  be_nested_str_weak(del_fabric),
    /* K32  */  be_const_int(0),
    /* K33  */  be_nested_str_weak(sessions),
    /* K34  */  be_nested_str_weak(fabrics),
    /* K35  */  be_nested_str_weak(get_fabric_index),
    /* K36  */  be_nested_str_weak(remove_fabric),
    /* K37  */  be_const_int(1),
    /* K38  */  be_nested_str_weak(_X2Fmatterc_X3F),
    /* K39  */  be_nested_str_weak(auto),
    /* K40  */  be_nested_str_weak(plugins_persist),
    /* K41  */  be_nested_str_weak(config),
    /* K42  */  be_nested_str_weak(_X2503i),
    /* K43  */  be_nested_str_weak(ep),
    /* K44  */  be_nested_str_weak(pi),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20ep_X3D_X25i_X20type_X3D_X25s_X20arg_X3D_X25s),
    /* K46  */  be_nested_str_weak(),
    /* K47  */  be_nested_str_weak(plugins_classes),
    /* K48  */  be_nested_str_weak(find),
    /* K49  */  be_nested_str_weak(type),
    /* K50  */  be_nested_str_weak(ui_string_to_conf),
    /* K51  */  be_nested_str_weak(MTR_X3A_X20unknown_X20type_X20_X3D_X20_X25s),
    /* K52  */  be_const_int(2),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20skipping_X20parameter),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20config_X20_X3D_X20_X25s),
    /* K55  */  be_nested_str_weak(contains),
    /* K56  */  be_nested_str_weak(0),
    /* K57  */  be_nested_str_weak(Missing_X20endpoint_X200),
    /* K58  */  be_nested_str_weak(MTR_X3A_X20config_X20error_X20_X3D_X20_X25s),
    /* K59  */  be_nested_str_weak(plugins_config),
    /* K60  */  be_nested_str_weak(content_start),
    /* K61  */  be_nested_str_weak(Parameter_X20error),
    /* K62  */  be_nested_str_weak(content_send_style),
    /* K63  */  be_nested_str_weak(content_send),
    /* K64  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EError_X3A_X3C_X2Fb_X3E_X25s_X3C_X2Fp_X3E),
    /* K65  */  be_nested_str_weak(html_escape),
    /* K66  */  be_nested_str_weak(content_button),
    /* K67  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K68  */  be_nested_str_weak(content_stop),
    /* K69  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K70  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
    }),
    be_str_weak(page_part_ctl),
    &be_const_str_solidified,
    ( &(const binstruction[353]) {  /* code */
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
      0xA8020136,  //  000A  EXBLK	0	#0142
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
      0x8C20050A,  //  0015  GETMET	R8	R2	K10
      0x5828000B,  //  0016  LDCONST	R10	K11
      0x582C0006,  //  0017  LDCONST	R11	K6
      0x7C200600,  //  0018  CALL	R8	3
      0x5824000C,  //  0019  LDCONST	R9	K12
      0x7C180600,  //  001A  CALL	R6	3
      0x8C180305,  //  001B  GETMET	R6	R1	K5
      0x58200006,  //  001C  LDCONST	R8	K6
      0x7C180400,  //  001D  CALL	R6	2
      0x781A0006,  //  001E  JMPF	R6	#0026
      0x8818010D,  //  001F  GETMBR	R6	R0	K13
      0x601C0009,  //  0020  GETGBL	R7	G9
      0x8C20030F,  //  0021  GETMET	R8	R1	K15
      0x58280006,  //  0022  LDCONST	R10	K6
      0x7C200400,  //  0023  CALL	R8	2
      0x7C1C0200,  //  0024  CALL	R7	1
      0x901A1C07,  //  0025  SETMBR	R6	K14	R7
      0x8C180305,  //  0026  GETMET	R6	R1	K5
      0x58200007,  //  0027  LDCONST	R8	K7
      0x7C180400,  //  0028  CALL	R6	2
      0x781A0006,  //  0029  JMPF	R6	#0031
      0x8818010D,  //  002A  GETMBR	R6	R0	K13
      0x601C0009,  //  002B  GETGBL	R7	G9
      0x8C20030F,  //  002C  GETMET	R8	R1	K15
      0x58280007,  //  002D  LDCONST	R10	K7
      0x7C200400,  //  002E  CALL	R8	2
      0x7C1C0200,  //  002F  CALL	R7	1
      0x901A2007,  //  0030  SETMBR	R6	K16	R7
      0x8818010D,  //  0031  GETMBR	R6	R0	K13
      0x8C1C030F,  //  0032  GETMET	R7	R1	K15
      0x58240012,  //  0033  LDCONST	R9	K18
      0x7C1C0400,  //  0034  CALL	R7	2
      0x1C1C0F13,  //  0035  EQ	R7	R7	K19
      0x901A2207,  //  0036  SETMBR	R6	K17	R7
      0x8818010D,  //  0037  GETMBR	R6	R0	K13
      0x8C180D14,  //  0038  GETMET	R6	R6	K20
      0x7C180200,  //  0039  CALL	R6	1
      0x8C180315,  //  003A  GETMET	R6	R1	K21
      0x58200016,  //  003B  LDCONST	R8	K22
      0x7C180400,  //  003C  CALL	R6	2
      0x700200EE,  //  003D  JMP		#012D
      0x8C180305,  //  003E  GETMET	R6	R1	K5
      0x58200017,  //  003F  LDCONST	R8	K23
      0x7C180400,  //  0040  CALL	R6	2
      0x781A0014,  //  0041  JMPF	R6	#0057
      0xB81A1000,  //  0042  GETNGBL	R6	K8
      0x8C180D09,  //  0043  GETMET	R6	R6	K9
      0x8C20050A,  //  0044  GETMET	R8	R2	K10
      0x5828000B,  //  0045  LDCONST	R10	K11
      0x582C0017,  //  0046  LDCONST	R11	K23
      0x7C200600,  //  0047  CALL	R8	3
      0x5824000C,  //  0048  LDCONST	R9	K12
      0x7C180600,  //  0049  CALL	R6	3
      0xB81A1000,  //  004A  GETNGBL	R6	K8
      0x8C180D18,  //  004B  GETMET	R6	R6	K24
      0x60200008,  //  004C  GETGBL	R8	G8
      0xB8263400,  //  004D  GETNGBL	R9	K26
      0x8824131B,  //  004E  GETMBR	R9	R9	K27
      0x7C200200,  //  004F  CALL	R8	1
      0x00223208,  //  0050  ADD	R8	K25	R8
      0x0020111C,  //  0051  ADD	R8	R8	K28
      0x7C180400,  //  0052  CALL	R6	2
      0x8C180315,  //  0053  GETMET	R6	R1	K21
      0x58200016,  //  0054  LDCONST	R8	K22
      0x7C180400,  //  0055  CALL	R6	2
      0x700200D5,  //  0056  JMP		#012D
      0x8C180305,  //  0057  GETMET	R6	R1	K5
      0x5820001D,  //  0058  LDCONST	R8	K29
      0x7C180400,  //  0059  CALL	R6	2
      0x781A0014,  //  005A  JMPF	R6	#0070
      0xB81A1000,  //  005B  GETNGBL	R6	K8
      0x8C180D09,  //  005C  GETMET	R6	R6	K9
      0x8C20050A,  //  005D  GETMET	R8	R2	K10
      0x5828000B,  //  005E  LDCONST	R10	K11
      0x582C001D,  //  005F  LDCONST	R11	K29
      0x7C200600,  //  0060  CALL	R8	3
      0x5824000C,  //  0061  LDCONST	R9	K12
      0x7C180600,  //  0062  CALL	R6	3
      0xB81A1000,  //  0063  GETNGBL	R6	K8
      0x8C180D18,  //  0064  GETMET	R6	R6	K24
      0x60200008,  //  0065  GETGBL	R8	G8
      0xB8263400,  //  0066  GETNGBL	R9	K26
      0x8824131B,  //  0067  GETMBR	R9	R9	K27
      0x7C200200,  //  0068  CALL	R8	1
      0x00223208,  //  0069  ADD	R8	K25	R8
      0x0020111E,  //  006A  ADD	R8	R8	K30
      0x7C180400,  //  006B  CALL	R6	2
      0x8C180315,  //  006C  GETMET	R6	R1	K21
      0x58200016,  //  006D  LDCONST	R8	K22
      0x7C180400,  //  006E  CALL	R6	2
      0x700200BC,  //  006F  JMP		#012D
      0x8C180305,  //  0070  GETMET	R6	R1	K5
      0x5820001F,  //  0071  LDCONST	R8	K31
      0x7C180400,  //  0072  CALL	R6	2
      0x781A0026,  //  0073  JMPF	R6	#009B
      0xB81A1000,  //  0074  GETNGBL	R6	K8
      0x8C180D09,  //  0075  GETMET	R6	R6	K9
      0x8C20050A,  //  0076  GETMET	R8	R2	K10
      0x5828000B,  //  0077  LDCONST	R10	K11
      0x582C001F,  //  0078  LDCONST	R11	K31
      0x7C200600,  //  0079  CALL	R8	3
      0x5824000C,  //  007A  LDCONST	R9	K12
      0x7C180600,  //  007B  CALL	R6	3
      0x60180009,  //  007C  GETGBL	R6	G9
      0x8C1C030F,  //  007D  GETMET	R7	R1	K15
      0x5824001F,  //  007E  LDCONST	R9	K31
      0x7C1C0400,  //  007F  CALL	R7	2
      0x7C180200,  //  0080  CALL	R6	1
      0x581C0020,  //  0081  LDCONST	R7	K32
      0x8820010D,  //  0082  GETMBR	R8	R0	K13
      0x88201121,  //  0083  GETMBR	R8	R8	K33
      0x88201122,  //  0084  GETMBR	R8	R8	K34
      0x6024000C,  //  0085  GETGBL	R9	G12
      0x5C281000,  //  0086  MOVE	R10	R8
      0x7C240200,  //  0087  CALL	R9	1
      0x14240E09,  //  0088  LT	R9	R7	R9
      0x7826000C,  //  0089  JMPF	R9	#0097
      0x94241007,  //  008A  GETIDX	R9	R8	R7
      0x8C241323,  //  008B  GETMET	R9	R9	K35
      0x7C240200,  //  008C  CALL	R9	1
      0x1C241206,  //  008D  EQ	R9	R9	R6
      0x78260005,  //  008E  JMPF	R9	#0095
      0x8824010D,  //  008F  GETMBR	R9	R0	K13
      0x8C241324,  //  0090  GETMET	R9	R9	K36
      0x942C1007,  //  0091  GETIDX	R11	R8	R7
      0x7C240400,  //  0092  CALL	R9	2
      0x70020002,  //  0093  JMP		#0097
      0x70020000,  //  0094  JMP		#0096
      0x001C0F25,  //  0095  ADD	R7	R7	K37
      0x7001FFED,  //  0096  JMP		#0085
      0x8C240315,  //  0097  GETMET	R9	R1	K21
      0x582C0026,  //  0098  LDCONST	R11	K38
      0x7C240400,  //  0099  CALL	R9	2
      0x70020091,  //  009A  JMP		#012D
      0x8C180305,  //  009B  GETMET	R6	R1	K5
      0x58200027,  //  009C  LDCONST	R8	K39
      0x7C180400,  //  009D  CALL	R6	2
      0x781A0011,  //  009E  JMPF	R6	#00B1
      0xB81A1000,  //  009F  GETNGBL	R6	K8
      0x8C180D09,  //  00A0  GETMET	R6	R6	K9
      0x8C20050A,  //  00A1  GETMET	R8	R2	K10
      0x5828000B,  //  00A2  LDCONST	R10	K11
      0x582C0027,  //  00A3  LDCONST	R11	K39
      0x7C200600,  //  00A4  CALL	R8	3
      0x5824000C,  //  00A5  LDCONST	R9	K12
      0x7C180600,  //  00A6  CALL	R6	3
      0x8818010D,  //  00A7  GETMBR	R6	R0	K13
      0x501C0000,  //  00A8  LDBOOL	R7	0	0
      0x901A5007,  //  00A9  SETMBR	R6	K40	R7
      0x8818010D,  //  00AA  GETMBR	R6	R0	K13
      0x8C180D14,  //  00AB  GETMET	R6	R6	K20
      0x7C180200,  //  00AC  CALL	R6	1
      0x8C180315,  //  00AD  GETMET	R6	R1	K21
      0x58200016,  //  00AE  LDCONST	R8	K22
      0x7C180400,  //  00AF  CALL	R6	2
      0x7002007B,  //  00B0  JMP		#012D
      0x8C180305,  //  00B1  GETMET	R6	R1	K5
      0x58200029,  //  00B2  LDCONST	R8	K41
      0x7C180400,  //  00B3  CALL	R6	2
      0x781A0077,  //  00B4  JMPF	R6	#012D
      0x60180013,  //  00B5  GETGBL	R6	G19
      0x7C180000,  //  00B6  CALL	R6	0
      0xB81E1000,  //  00B7  GETNGBL	R7	K8
      0x8C1C0F09,  //  00B8  GETMET	R7	R7	K9
      0x8C24050A,  //  00B9  GETMET	R9	R2	K10
      0x582C000B,  //  00BA  LDCONST	R11	K11
      0x58300029,  //  00BB  LDCONST	R12	K41
      0x7C240600,  //  00BC  CALL	R9	3
      0x5828000C,  //  00BD  LDCONST	R10	K12
      0x7C1C0600,  //  00BE  CALL	R7	3
      0x581C0020,  //  00BF  LDCONST	R7	K32
      0x8C20050A,  //  00C0  GETMET	R8	R2	K10
      0x5828002A,  //  00C1  LDCONST	R10	K42
      0x5C2C0E00,  //  00C2  MOVE	R11	R7
      0x7C200600,  //  00C3  CALL	R8	3
      0x8C240305,  //  00C4  GETMET	R9	R1	K5
      0x002E5608,  //  00C5  ADD	R11	K43	R8
      0x7C240400,  //  00C6  CALL	R9	2
      0x78260040,  //  00C7  JMPF	R9	#0109
      0x8C24030F,  //  00C8  GETMET	R9	R1	K15
      0x002E5608,  //  00C9  ADD	R11	K43	R8
      0x7C240400,  //  00CA  CALL	R9	2
      0x60280009,  //  00CB  GETGBL	R10	G9
      0x5C2C1200,  //  00CC  MOVE	R11	R9
      0x7C280200,  //  00CD  CALL	R10	1
      0x8C2C030F,  //  00CE  GETMET	R11	R1	K15
      0x00365808,  //  00CF  ADD	R13	K44	R8
      0x7C2C0400,  //  00D0  CALL	R11	2
      0x8C30030F,  //  00D1  GETMET	R12	R1	K15
      0x003A1E08,  //  00D2  ADD	R14	K15	R8
      0x7C300400,  //  00D3  CALL	R12	2
      0xB8361000,  //  00D4  GETNGBL	R13	K8
      0x8C341B09,  //  00D5  GETMET	R13	R13	K9
      0x8C3C050A,  //  00D6  GETMET	R15	R2	K10
      0x5844002D,  //  00D7  LDCONST	R17	K45
      0x5C481200,  //  00D8  MOVE	R18	R9
      0x5C4C1600,  //  00D9  MOVE	R19	R11
      0x5C501800,  //  00DA  MOVE	R20	R12
      0x7C3C0A00,  //  00DB  CALL	R15	5
      0x5840000C,  //  00DC  LDCONST	R16	K12
      0x7C340600,  //  00DD  CALL	R13	3
      0x2034132E,  //  00DE  NE	R13	R9	K46
      0x7836001C,  //  00DF  JMPF	R13	#00FD
      0x2034172E,  //  00E0  NE	R13	R11	K46
      0x7836001A,  //  00E1  JMPF	R13	#00FD
      0x8834010D,  //  00E2  GETMBR	R13	R0	K13
      0x88341B2F,  //  00E3  GETMBR	R13	R13	K47
      0x8C341B30,  //  00E4  GETMET	R13	R13	K48
      0x5C3C1600,  //  00E5  MOVE	R15	R11
      0x7C340400,  //  00E6  CALL	R13	2
      0x4C380000,  //  00E7  LDNIL	R14
      0x20381A0E,  //  00E8  NE	R14	R13	R14
      0x783A0009,  //  00E9  JMPF	R14	#00F4
      0x60380013,  //  00EA  GETGBL	R14	G19
      0x7C380000,  //  00EB  CALL	R14	0
      0x983A620B,  //  00EC  SETIDX	R14	K49	R11
      0x8C3C1B32,  //  00ED  GETMET	R15	R13	K50
      0x5C441A00,  //  00EE  MOVE	R17	R13
      0x5C481C00,  //  00EF  MOVE	R18	R14
      0x5C4C1800,  //  00F0  MOVE	R19	R12
      0x7C3C0800,  //  00F1  CALL	R15	4
      0x9818120E,  //  00F2  SETIDX	R6	R9	R14
      0x70020007,  //  00F3  JMP		#00FC
      0xB83A1000,  //  00F4  GETNGBL	R14	K8
      0x8C381D09,  //  00F5  GETMET	R14	R14	K9
      0x8C40050A,  //  00F6  GETMET	R16	R2	K10
      0x58480033,  //  00F7  LDCONST	R18	K51
      0x5C4C1600,  //  00F8  MOVE	R19	R11
      0x7C400600,  //  00F9  CALL	R16	3
      0x58440034,  //  00FA  LDCONST	R17	K52
      0x7C380600,  //  00FB  CALL	R14	3
      0x70020004,  //  00FC  JMP		#0102
      0xB8361000,  //  00FD  GETNGBL	R13	K8
      0x8C341B09,  //  00FE  GETMET	R13	R13	K9
      0x583C0035,  //  00FF  LDCONST	R15	K53
      0x58400034,  //  0100  LDCONST	R16	K52
      0x7C340600,  //  0101  CALL	R13	3
      0x001C0F25,  //  0102  ADD	R7	R7	K37
      0x8C34050A,  //  0103  GETMET	R13	R2	K10
      0x583C002A,  //  0104  LDCONST	R15	K42
      0x5C400E00,  //  0105  MOVE	R16	R7
      0x7C340600,  //  0106  CALL	R13	3
      0x5C201A00,  //  0107  MOVE	R8	R13
      0x7001FFBA,  //  0108  JMP		#00C4
      0xB8261000,  //  0109  GETNGBL	R9	K8
      0x8C241309,  //  010A  GETMET	R9	R9	K9
      0x8C2C050A,  //  010B  GETMET	R11	R2	K10
      0x58340036,  //  010C  LDCONST	R13	K54
      0x60380008,  //  010D  GETGBL	R14	G8
      0x5C3C0C00,  //  010E  MOVE	R15	R6
      0x7C380200,  //  010F  CALL	R14	1
      0x7C2C0600,  //  0110  CALL	R11	3
      0x5830000C,  //  0111  LDCONST	R12	K12
      0x7C240600,  //  0112  CALL	R9	3
      0x8C240D37,  //  0113  GETMET	R9	R6	K55
      0x582C0038,  //  0114  LDCONST	R11	K56
      0x7C240400,  //  0115  CALL	R9	2
      0x74260000,  //  0116  JMPT	R9	#0118
      0x58140039,  //  0117  LDCONST	R5	K57
      0x78160008,  //  0118  JMPF	R5	#0122
      0xB8261000,  //  0119  GETNGBL	R9	K8
      0x8C241309,  //  011A  GETMET	R9	R9	K9
      0x8C2C050A,  //  011B  GETMET	R11	R2	K10
      0x5834003A,  //  011C  LDCONST	R13	K58
      0x5C380A00,  //  011D  MOVE	R14	R5
      0x7C2C0600,  //  011E  CALL	R11	3
      0x5830000C,  //  011F  LDCONST	R12	K12
      0x7C240600,  //  0120  CALL	R9	3
      0x7002000A,  //  0121  JMP		#012D
      0x8824010D,  //  0122  GETMBR	R9	R0	K13
      0x90267606,  //  0123  SETMBR	R9	K59	R6
      0x8824010D,  //  0124  GETMBR	R9	R0	K13
      0x50280200,  //  0125  LDBOOL	R10	1	0
      0x9026500A,  //  0126  SETMBR	R9	K40	R10
      0x8824010D,  //  0127  GETMBR	R9	R0	K13
      0x8C241314,  //  0128  GETMET	R9	R9	K20
      0x7C240200,  //  0129  CALL	R9	1
      0x8C240315,  //  012A  GETMET	R9	R1	K21
      0x582C0016,  //  012B  LDCONST	R11	K22
      0x7C240400,  //  012C  CALL	R9	2
      0x78160011,  //  012D  JMPF	R5	#0140
      0x8C18033C,  //  012E  GETMET	R6	R1	K60
      0x5820003D,  //  012F  LDCONST	R8	K61
      0x7C180400,  //  0130  CALL	R6	2
      0x8C18033E,  //  0131  GETMET	R6	R1	K62
      0x7C180200,  //  0132  CALL	R6	1
      0x8C18033F,  //  0133  GETMET	R6	R1	K63
      0x8C20050A,  //  0134  GETMET	R8	R2	K10
      0x58280040,  //  0135  LDCONST	R10	K64
      0x8C2C0341,  //  0136  GETMET	R11	R1	K65
      0x5C340A00,  //  0137  MOVE	R13	R5
      0x7C2C0400,  //  0138  CALL	R11	2
      0x7C200600,  //  0139  CALL	R8	3
      0x7C180400,  //  013A  CALL	R6	2
      0x8C180342,  //  013B  GETMET	R6	R1	K66
      0x88200343,  //  013C  GETMBR	R8	R1	K67
      0x7C180400,  //  013D  CALL	R6	2
      0x8C180344,  //  013E  GETMET	R6	R1	K68
      0x7C180200,  //  013F  CALL	R6	1
      0xA8040001,  //  0140  EXBLK	1	1
      0x7002001D,  //  0141  JMP		#0160
      0xAC180002,  //  0142  CATCH	R6	0	2
      0x7002001A,  //  0143  JMP		#015F
      0xB8221000,  //  0144  GETNGBL	R8	K8
      0x8C201109,  //  0145  GETMET	R8	R8	K9
      0x8C28050A,  //  0146  GETMET	R10	R2	K10
      0x58300045,  //  0147  LDCONST	R12	K69
      0x5C340C00,  //  0148  MOVE	R13	R6
      0x5C380E00,  //  0149  MOVE	R14	R7
      0x7C280800,  //  014A  CALL	R10	4
      0x582C0034,  //  014B  LDCONST	R11	K52
      0x7C200600,  //  014C  CALL	R8	3
      0x8C20033C,  //  014D  GETMET	R8	R1	K60
      0x5828003D,  //  014E  LDCONST	R10	K61
      0x7C200400,  //  014F  CALL	R8	2
      0x8C20033E,  //  0150  GETMET	R8	R1	K62
      0x7C200200,  //  0151  CALL	R8	1
      0x8C20033F,  //  0152  GETMET	R8	R1	K63
      0x8C28050A,  //  0153  GETMET	R10	R2	K10
      0x58300046,  //  0154  LDCONST	R12	K70
      0x5C340C00,  //  0155  MOVE	R13	R6
      0x5C380E00,  //  0156  MOVE	R14	R7
      0x7C280800,  //  0157  CALL	R10	4
      0x7C200400,  //  0158  CALL	R8	2
      0x8C200342,  //  0159  GETMET	R8	R1	K66
      0x88280343,  //  015A  GETMBR	R10	R1	K67
      0x7C200400,  //  015B  CALL	R8	2
      0x8C200344,  //  015C  GETMET	R8	R1	K68
      0x7C200200,  //  015D  CALL	R8	1
      0x70020000,  //  015E  JMP		#0160
      0xB0080000,  //  015F  RAISE	2	R0	R0
      0x80000000,  //  0160  RET	0
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
    /* K4   */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20cause_X20a_X20restart_X2E_X22_X29_X3B_X27_X3E),
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
    /* K11  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A320px_X3B_X27_X3ECheck_X20the_X20_X3Ca_X20href_X3D_X27https_X3A_X2F_X2Ftasmota_X2Egithub_X2Eio_X2Fdocs_X2FMatter_X2F_X27_X20target_X3D_X27_blank_X27_X3EMatter_X20documentation_X3C_X2Fa_X3E_X2E_X3C_X2Fp_X3E),
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
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(get_option),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(MATTER_OPTION),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(sessions),
    /* K8   */  be_nested_str_weak(count_active_fabrics),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(content_send),
    /* K11  */  be_nested_str_weak(format),
    /* K12  */  be_nested_str_weak(_X3Cdiv_X20style_X3D_X27text_X2Dalign_X3Aright_X3Bfont_X2Dsize_X3A11px_X3Bcolor_X3A_X23aaa_X3Bpadding_X3A0px_X3B_X27_X3E_X25s_X3C_X2Fdiv_X3E),
    /* K13  */  be_nested_str_weak(Matter_X3A_X20No_X20active_X20association),
    /* K14  */  be_const_int(1),
    /* K15  */  be_nested_str_weak(Matter_X3A_X20),
    /* K16  */  be_nested_str_weak(_X20active_X20association),
    /* K17  */  be_nested_str_weak(s),
    /* K18  */  be_nested_str_weak(),
    /* K19  */  be_nested_str_weak(show_bridge_status),
    /* K20  */  be_nested_str_weak(is_root_commissioning_open),
    /* K21  */  be_nested_str_weak(show_commissioning_info),
    /* K22  */  be_nested_str_weak(_X3Cbutton_X20onclick_X3D_X27la_X28_X22_X26mtc_X25i_X3D1_X22_X29_X3B_X27_X3E),
    /* K23  */  be_nested_str_weak(commissioning_open),
    /* K24  */  be_nested_str_weak(_LOGO),
    /* K25  */  be_nested_str_weak(_X20Open_X20Commissioning_X3C_X2Fbutton_X3E),
    /* K26  */  be_nested_str_weak(_X20Close_X20Commissioning_X3C_X2Fbutton_X3E),
    }),
    be_str_weak(web_sensor),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0xB8160800,  //  0004  GETNGBL	R5	K4
      0x88140B05,  //  0005  GETMBR	R5	R5	K5
      0x7C0C0400,  //  0006  CALL	R3	2
      0x780E0043,  //  0007  JMPF	R3	#004C
      0x88100106,  //  0008  GETMBR	R4	R0	K6
      0x88100907,  //  0009  GETMBR	R4	R4	K7
      0x8C100908,  //  000A  GETMET	R4	R4	K8
      0x7C100200,  //  000B  CALL	R4	1
      0x1C140909,  //  000C  EQ	R5	R4	K9
      0x78160006,  //  000D  JMPF	R5	#0015
      0x8C14030A,  //  000E  GETMET	R5	R1	K10
      0x8C1C050B,  //  000F  GETMET	R7	R2	K11
      0x5824000C,  //  0010  LDCONST	R9	K12
      0x5828000D,  //  0011  LDCONST	R10	K13
      0x7C1C0600,  //  0012  CALL	R7	3
      0x7C140400,  //  0013  CALL	R5	2
      0x7002000F,  //  0014  JMP		#0025
      0x2414090E,  //  0015  GT	R5	R4	K14
      0x8C18030A,  //  0016  GETMET	R6	R1	K10
      0x8C20050B,  //  0017  GETMET	R8	R2	K11
      0x5828000C,  //  0018  LDCONST	R10	K12
      0x602C0008,  //  0019  GETGBL	R11	G8
      0x5C300800,  //  001A  MOVE	R12	R4
      0x7C2C0200,  //  001B  CALL	R11	1
      0x002E1E0B,  //  001C  ADD	R11	K15	R11
      0x002C1710,  //  001D  ADD	R11	R11	K16
      0x78160001,  //  001E  JMPF	R5	#0021
      0x58300011,  //  001F  LDCONST	R12	K17
      0x70020000,  //  0020  JMP		#0022
      0x58300012,  //  0021  LDCONST	R12	K18
      0x002C160C,  //  0022  ADD	R11	R11	R12
      0x7C200600,  //  0023  CALL	R8	3
      0x7C180400,  //  0024  CALL	R6	2
      0x8C140113,  //  0025  GETMET	R5	R0	K19
      0x7C140200,  //  0026  CALL	R5	1
      0x88140106,  //  0027  GETMBR	R5	R0	K6
      0x8C140B14,  //  0028  GETMET	R5	R5	K20
      0x7C140200,  //  0029  CALL	R5	1
      0x78160001,  //  002A  JMPF	R5	#002D
      0x8C140115,  //  002B  GETMET	R5	R0	K21
      0x7C140200,  //  002C  CALL	R5	1
      0x1C140909,  //  002D  EQ	R5	R4	K9
      0x7816001C,  //  002E  JMPF	R5	#004C
      0x8C14030A,  //  002F  GETMET	R5	R1	K10
      0x8C1C050B,  //  0030  GETMET	R7	R2	K11
      0x58240016,  //  0031  LDCONST	R9	K22
      0x88280106,  //  0032  GETMBR	R10	R0	K6
      0x88281517,  //  0033  GETMBR	R10	R10	K23
      0x4C2C0000,  //  0034  LDNIL	R11
      0x1C28140B,  //  0035  EQ	R10	R10	R11
      0x782A0001,  //  0036  JMPF	R10	#0039
      0x5828000E,  //  0037  LDCONST	R10	K14
      0x70020000,  //  0038  JMP		#003A
      0x58280009,  //  0039  LDCONST	R10	K9
      0x7C1C0600,  //  003A  CALL	R7	3
      0x7C140400,  //  003B  CALL	R5	2
      0x8C14030A,  //  003C  GETMET	R5	R1	K10
      0xB81E0800,  //  003D  GETNGBL	R7	K4
      0x881C0F18,  //  003E  GETMBR	R7	R7	K24
      0x7C140400,  //  003F  CALL	R5	2
      0x88140106,  //  0040  GETMBR	R5	R0	K6
      0x88140B17,  //  0041  GETMBR	R5	R5	K23
      0x4C180000,  //  0042  LDNIL	R6
      0x1C140A06,  //  0043  EQ	R5	R5	R6
      0x78160003,  //  0044  JMPF	R5	#0049
      0x8C14030A,  //  0045  GETMET	R5	R1	K10
      0x581C0019,  //  0046  LDCONST	R7	K25
      0x7C140400,  //  0047  CALL	R5	2
      0x70020002,  //  0048  JMP		#004C
      0x8C14030A,  //  0049  GETMET	R5	R1	K10
      0x581C001A,  //  004A  LDCONST	R7	K26
      0x7C140400,  //  004B  CALL	R5	2
      0x80000000,  //  004C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_plugins_configuration
********************************************************************/
be_local_closure(Matter_UI_show_plugins_configuration,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[37]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BCurrent_X20Configuration_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27),
    /* K5   */  be_nested_str_weak(onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20RESET_X20the_X20configuration_X20to_X20the_X20default_X2E_X20You_X20will_X20need_X20to_X20associate_X20again_X2E_X22_X29_X3B_X27_X3E),
    /* K6   */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27auto_X27_X20class_X3D_X27button_X20bred_X27_X3EReset_X20to_X20default_X3C_X2Fbutton_X3E_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Fform_X3E),
    /* K7   */  be_nested_str_weak(onsubmit_X3D_X27return_X20confirm_X28_X22Changing_X20the_X20configuration_X20requires_X20to_X20associate_X20again_X2E_X22_X29_X3B_X27_X3E),
    /* K8   */  be_nested_str_weak(_X3Ctable_X20style_X3D_X27width_X3A100_X25_X27_X3E),
    /* K9   */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X20width_X3D_X2735_X27_X3E_X3Cb_X3EEp_X2E_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cb_X3EType_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3Ctd_X3E_X3Cb_X3EParam_X3C_X2Fb_X3E_X3C_X2Ftd_X3E_X3C_X2Ftr_X3E),
    /* K10  */  be_nested_str_weak(device),
    /* K11  */  be_nested_str_weak(k2l_num),
    /* K12  */  be_nested_str_weak(plugins_config),
    /* K13  */  be_const_int(0),
    /* K14  */  be_nested_str_weak(find),
    /* K15  */  be_nested_str_weak(type),
    /* K16  */  be_nested_str_weak(format),
    /* K17  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27ep_X2503i_X27_X20maxlength_X3D_X274_X27_X20size_X3D_X273_X27_X20value_X3D_X270_X27_X20readonly_X3E_X3C_X2Ftd_X3E),
    /* K18  */  be_nested_str_weak(_X3Ctd_X3E_X3Cselect_X20name_X3D_X27pi_X2503i_X27_X3E),
    /* K19  */  be_nested_str_weak(plugin_option),
    /* K20  */  be_nested_str_weak(),
    /* K21  */  be_nested_str_weak(_ROOT_TYPES),
    /* K22  */  be_nested_str_weak(_X3C_X2Fselect_X3E_X3C_X2Ftd_X3E),
    /* K23  */  be_nested_str_weak(_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X26nbsp_X3B_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K24  */  be_const_int(1),
    /* K25  */  be_nested_str_weak(plugins_classes),
    /* K26  */  be_nested_str_weak(ui_conf_to_string),
    /* K27  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27ep_X2503i_X27_X20maxlength_X3D_X274_X27_X20size_X3D_X273_X27_X20pattern_X3D_X27_X5B0_X2D9_X5D_X7B1_X2C4_X7D_X27_X20value_X3D_X27_X25i_X27_X3E_X3C_X2Ftd_X3E),
    /* K28  */  be_nested_str_weak(_CLASSES_TYPES),
    /* K29  */  be_nested_str_weak(_CLASSES_TYPES2),
    /* K30  */  be_nested_str_weak(_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X2503i_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X25s_X27_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K31  */  be_nested_str_weak(html_escape),
    /* K32  */  be_nested_str_weak(_X3Ctr_X3E_X3Ctd_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27ep_X2503i_X27_X20maxlength_X3D_X274_X27_X20size_X3D_X273_X27_X20pattern_X3D_X27_X5B0_X2D9_X5D_X7B1_X2C4_X7D_X27_X20value_X3D_X27_X27_X3E_X3C_X2Ftd_X3E),
    /* K33  */  be_nested_str_weak(_X3Ctd_X3E_X3Cfont_X20size_X3D_X27_X2D1_X27_X3E_X3Cinput_X20type_X3D_X27text_X27_X20name_X3D_X27arg_X2503i_X27_X20minlength_X3D_X270_X27_X20size_X3D_X278_X27_X20value_X3D_X27_X27_X3E_X3C_X2Ffont_X3E_X3C_X2Ftd_X3E),
    /* K34  */  be_nested_str_weak(_X3C_X2Ftable_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K35  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27config_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X20configuration_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E),
    /* K36  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_plugins_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[183]) {  /* code */
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
      0x58140006,  //  000C  LDCONST	R5	K6
      0x7C0C0400,  //  000D  CALL	R3	2
      0x8C0C0302,  //  000E  GETMET	R3	R1	K2
      0x58140004,  //  000F  LDCONST	R5	K4
      0x7C0C0400,  //  0010  CALL	R3	2
      0x8C0C0302,  //  0011  GETMET	R3	R1	K2
      0x58140007,  //  0012  LDCONST	R5	K7
      0x7C0C0400,  //  0013  CALL	R3	2
      0x8C0C0302,  //  0014  GETMET	R3	R1	K2
      0x58140008,  //  0015  LDCONST	R5	K8
      0x7C0C0400,  //  0016  CALL	R3	2
      0x8C0C0302,  //  0017  GETMET	R3	R1	K2
      0x58140009,  //  0018  LDCONST	R5	K9
      0x7C0C0400,  //  0019  CALL	R3	2
      0x880C010A,  //  001A  GETMBR	R3	R0	K10
      0x8C0C070B,  //  001B  GETMET	R3	R3	K11
      0x8814010A,  //  001C  GETMBR	R5	R0	K10
      0x88140B0C,  //  001D  GETMBR	R5	R5	K12
      0x7C0C0400,  //  001E  CALL	R3	2
      0x5810000D,  //  001F  LDCONST	R4	K13
      0x9414070D,  //  0020  GETIDX	R5	R3	K13
      0x1C140B0D,  //  0021  EQ	R5	R5	K13
      0x78160026,  //  0022  JMPF	R5	#004A
      0x94140604,  //  0023  GETIDX	R5	R3	R4
      0x8818010A,  //  0024  GETMBR	R6	R0	K10
      0x601C0008,  //  0025  GETGBL	R7	G8
      0x5C200A00,  //  0026  MOVE	R8	R5
      0x7C1C0200,  //  0027  CALL	R7	1
      0x88180D0C,  //  0028  GETMBR	R6	R6	K12
      0x94180C07,  //  0029  GETIDX	R6	R6	R7
      0x8C200D0E,  //  002A  GETMET	R8	R6	K14
      0x5828000F,  //  002B  LDCONST	R10	K15
      0x7C200400,  //  002C  CALL	R8	2
      0x5C1C1000,  //  002D  MOVE	R7	R8
      0x8C200302,  //  002E  GETMET	R8	R1	K2
      0x8C280510,  //  002F  GETMET	R10	R2	K16
      0x58300011,  //  0030  LDCONST	R12	K17
      0x5C340800,  //  0031  MOVE	R13	R4
      0x7C280600,  //  0032  CALL	R10	3
      0x7C200400,  //  0033  CALL	R8	2
      0x8C200302,  //  0034  GETMET	R8	R1	K2
      0x8C280510,  //  0035  GETMET	R10	R2	K16
      0x58300012,  //  0036  LDCONST	R12	K18
      0x5C340800,  //  0037  MOVE	R13	R4
      0x7C280600,  //  0038  CALL	R10	3
      0x7C200400,  //  0039  CALL	R8	2
      0x8C200113,  //  003A  GETMET	R8	R0	K19
      0x8C280D0E,  //  003B  GETMET	R10	R6	K14
      0x5830000F,  //  003C  LDCONST	R12	K15
      0x58340014,  //  003D  LDCONST	R13	K20
      0x7C280600,  //  003E  CALL	R10	3
      0x882C0115,  //  003F  GETMBR	R11	R0	K21
      0x7C200600,  //  0040  CALL	R8	3
      0x8C200302,  //  0041  GETMET	R8	R1	K2
      0x8C280510,  //  0042  GETMET	R10	R2	K16
      0x58300016,  //  0043  LDCONST	R12	K22
      0x7C280400,  //  0044  CALL	R10	2
      0x7C200400,  //  0045  CALL	R8	2
      0x8C200302,  //  0046  GETMET	R8	R1	K2
      0x58280017,  //  0047  LDCONST	R10	K23
      0x7C200400,  //  0048  CALL	R8	2
      0x00100918,  //  0049  ADD	R4	R4	K24
      0x6014000C,  //  004A  GETGBL	R5	G12
      0x5C180600,  //  004B  MOVE	R6	R3
      0x7C140200,  //  004C  CALL	R5	1
      0x14140805,  //  004D  LT	R5	R4	R5
      0x78160041,  //  004E  JMPF	R5	#0091
      0x94140604,  //  004F  GETIDX	R5	R3	R4
      0x8818010A,  //  0050  GETMBR	R6	R0	K10
      0x601C0008,  //  0051  GETGBL	R7	G8
      0x5C200A00,  //  0052  MOVE	R8	R5
      0x7C1C0200,  //  0053  CALL	R7	1
      0x88180D0C,  //  0054  GETMBR	R6	R6	K12
      0x94180C07,  //  0055  GETIDX	R6	R6	R7
      0x8C200D0E,  //  0056  GETMET	R8	R6	K14
      0x5828000F,  //  0057  LDCONST	R10	K15
      0x7C200400,  //  0058  CALL	R8	2
      0x5C1C1000,  //  0059  MOVE	R7	R8
      0x5C200E00,  //  005A  MOVE	R8	R7
      0x74220001,  //  005B  JMPT	R8	#005E
      0x00100918,  //  005C  ADD	R4	R4	K24
      0x7001FFEB,  //  005D  JMP		#004A
      0x8820010A,  //  005E  GETMBR	R8	R0	K10
      0x88201119,  //  005F  GETMBR	R8	R8	K25
      0x8C20110E,  //  0060  GETMET	R8	R8	K14
      0x5C280E00,  //  0061  MOVE	R10	R7
      0x7C200400,  //  0062  CALL	R8	2
      0x58240014,  //  0063  LDCONST	R9	K20
      0x4C280000,  //  0064  LDNIL	R10
      0x2028100A,  //  0065  NE	R10	R8	R10
      0x782A0004,  //  0066  JMPF	R10	#006C
      0x8C28111A,  //  0067  GETMET	R10	R8	K26
      0x5C301000,  //  0068  MOVE	R12	R8
      0x5C340C00,  //  0069  MOVE	R13	R6
      0x7C280600,  //  006A  CALL	R10	3
      0x5C241400,  //  006B  MOVE	R9	R10
      0x8C280302,  //  006C  GETMET	R10	R1	K2
      0x8C300510,  //  006D  GETMET	R12	R2	K16
      0x5838001B,  //  006E  LDCONST	R14	K27
      0x5C3C0800,  //  006F  MOVE	R15	R4
      0x5C400A00,  //  0070  MOVE	R16	R5
      0x7C300800,  //  0071  CALL	R12	4
      0x7C280400,  //  0072  CALL	R10	2
      0x8C280302,  //  0073  GETMET	R10	R1	K2
      0x8C300510,  //  0074  GETMET	R12	R2	K16
      0x58380012,  //  0075  LDCONST	R14	K18
      0x5C3C0800,  //  0076  MOVE	R15	R4
      0x7C300600,  //  0077  CALL	R12	3
      0x7C280400,  //  0078  CALL	R10	2
      0x8C280113,  //  0079  GETMET	R10	R0	K19
      0x8C300D0E,  //  007A  GETMET	R12	R6	K14
      0x5838000F,  //  007B  LDCONST	R14	K15
      0x583C0014,  //  007C  LDCONST	R15	K20
      0x7C300600,  //  007D  CALL	R12	3
      0x8834011C,  //  007E  GETMBR	R13	R0	K28
      0x8838011D,  //  007F  GETMBR	R14	R0	K29
      0x7C280800,  //  0080  CALL	R10	4
      0x8C280302,  //  0081  GETMET	R10	R1	K2
      0x8C300510,  //  0082  GETMET	R12	R2	K16
      0x58380016,  //  0083  LDCONST	R14	K22
      0x7C300400,  //  0084  CALL	R12	2
      0x7C280400,  //  0085  CALL	R10	2
      0x8C280302,  //  0086  GETMET	R10	R1	K2
      0x8C300510,  //  0087  GETMET	R12	R2	K16
      0x5838001E,  //  0088  LDCONST	R14	K30
      0x5C3C0800,  //  0089  MOVE	R15	R4
      0x8C40031F,  //  008A  GETMET	R16	R1	K31
      0x5C481200,  //  008B  MOVE	R18	R9
      0x7C400400,  //  008C  CALL	R16	2
      0x7C300800,  //  008D  CALL	R12	4
      0x7C280400,  //  008E  CALL	R10	2
      0x00100918,  //  008F  ADD	R4	R4	K24
      0x7001FFB8,  //  0090  JMP		#004A
      0x8C140302,  //  0091  GETMET	R5	R1	K2
      0x8C1C0510,  //  0092  GETMET	R7	R2	K16
      0x58240020,  //  0093  LDCONST	R9	K32
      0x5C280800,  //  0094  MOVE	R10	R4
      0x7C1C0600,  //  0095  CALL	R7	3
      0x7C140400,  //  0096  CALL	R5	2
      0x8C140302,  //  0097  GETMET	R5	R1	K2
      0x8C1C0510,  //  0098  GETMET	R7	R2	K16
      0x58240012,  //  0099  LDCONST	R9	K18
      0x5C280800,  //  009A  MOVE	R10	R4
      0x7C1C0600,  //  009B  CALL	R7	3
      0x7C140400,  //  009C  CALL	R5	2
      0x8C140113,  //  009D  GETMET	R5	R0	K19
      0x581C0014,  //  009E  LDCONST	R7	K20
      0x8820011C,  //  009F  GETMBR	R8	R0	K28
      0x8824011D,  //  00A0  GETMBR	R9	R0	K29
      0x7C140800,  //  00A1  CALL	R5	4
      0x8C140302,  //  00A2  GETMET	R5	R1	K2
      0x8C1C0510,  //  00A3  GETMET	R7	R2	K16
      0x58240016,  //  00A4  LDCONST	R9	K22
      0x7C1C0400,  //  00A5  CALL	R7	2
      0x7C140400,  //  00A6  CALL	R5	2
      0x8C140302,  //  00A7  GETMET	R5	R1	K2
      0x8C1C0510,  //  00A8  GETMET	R7	R2	K16
      0x58240021,  //  00A9  LDCONST	R9	K33
      0x5C280800,  //  00AA  MOVE	R10	R4
      0x7C1C0600,  //  00AB  CALL	R7	3
      0x7C140400,  //  00AC  CALL	R5	2
      0x8C140302,  //  00AD  GETMET	R5	R1	K2
      0x581C0022,  //  00AE  LDCONST	R7	K34
      0x7C140400,  //  00AF  CALL	R5	2
      0x8C140302,  //  00B0  GETMET	R5	R1	K2
      0x581C0023,  //  00B1  LDCONST	R7	K35
      0x7C140400,  //  00B2  CALL	R5	2
      0x8C140302,  //  00B3  GETMET	R5	R1	K2
      0x581C0024,  //  00B4  LDCONST	R7	K36
      0x7C140400,  //  00B5  CALL	R5	2
      0x80000000,  //  00B6  RET	0
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
    ( &(const bvalue[20]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(_X3C_X2Fdiv_X3E),
    /* K19  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_commissioning_info),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
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
      0x8C200508,  //  0029  GETMET	R8	R2	K8
      0x5828000E,  //  002A  LDCONST	R10	K14
      0x7C200400,  //  002B  CALL	R8	2
      0x7C180400,  //  002C  CALL	R6	2
      0x88180102,  //  002D  GETMBR	R6	R0	K2
      0x8C180D0F,  //  002E  GETMET	R6	R6	K15
      0x7C180200,  //  002F  CALL	R6	1
      0x8C1C0110,  //  0030  GETMET	R7	R0	K16
      0x5C240C00,  //  0031  MOVE	R9	R6
      0x7C1C0400,  //  0032  CALL	R7	2
      0x8C1C0307,  //  0033  GETMET	R7	R1	K7
      0x8C240508,  //  0034  GETMET	R9	R2	K8
      0x582C0011,  //  0035  LDCONST	R11	K17
      0x5C300C00,  //  0036  MOVE	R12	R6
      0x7C240600,  //  0037  CALL	R9	3
      0x7C1C0400,  //  0038  CALL	R7	2
      0x8C1C0307,  //  0039  GETMET	R7	R1	K7
      0x8C240508,  //  003A  GETMET	R9	R2	K8
      0x582C0012,  //  003B  LDCONST	R11	K18
      0x7C240400,  //  003C  CALL	R9	2
      0x7C1C0400,  //  003D  CALL	R7	2
      0x8C1C0307,  //  003E  GETMET	R7	R1	K7
      0x58240013,  //  003F  LDCONST	R9	K19
      0x7C1C0400,  //  0040  CALL	R7	2
      0x80000000,  //  0041  RET	0
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
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BFabrics_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(_X3Cp_X3EAssociated_X20fabrics_X3A_X3C_X2Fp_X3E),
    /* K5   */  be_nested_str_weak(device),
    /* K6   */  be_nested_str_weak(sessions),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3ENone_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K9   */  be_nested_str_weak(fabrics),
    /* K10  */  be_nested_str_weak(persistables),
    /* K11  */  be_nested_str_weak(_X3Chr_X3E),
    /* K12  */  be_nested_str_weak(fabric_label),
    /* K13  */  be_nested_str_weak(_X3CNo_X20label_X3E),
    /* K14  */  be_nested_str_weak(html_escape),
    /* K15  */  be_nested_str_weak(format),
    /* K16  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3B_X23_X25i_X20_X25s_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K17  */  be_nested_str_weak(get_fabric_index),
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
    ( &(const binstruction[102]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C0C0302,  //  0005  GETMET	R3	R1	K2
      0x58140004,  //  0006  LDCONST	R5	K4
      0x7C0C0400,  //  0007  CALL	R3	2
      0x600C000C,  //  0008  GETGBL	R3	G12
      0x88100105,  //  0009  GETMBR	R4	R0	K5
      0x88100906,  //  000A  GETMBR	R4	R4	K6
      0x88100906,  //  000B  GETMBR	R4	R4	K6
      0x7C0C0200,  //  000C  CALL	R3	1
      0x1C0C0707,  //  000D  EQ	R3	R3	K7
      0x780E0003,  //  000E  JMPF	R3	#0013
      0x8C0C0302,  //  000F  GETMET	R3	R1	K2
      0x58140008,  //  0010  LDCONST	R5	K8
      0x7C0C0400,  //  0011  CALL	R3	2
      0x7002004E,  //  0012  JMP		#0062
      0x500C0200,  //  0013  LDBOOL	R3	1	0
      0x60100010,  //  0014  GETGBL	R4	G16
      0x88140105,  //  0015  GETMBR	R5	R0	K5
      0x88140B06,  //  0016  GETMBR	R5	R5	K6
      0x88140B09,  //  0017  GETMBR	R5	R5	K9
      0x8C140B0A,  //  0018  GETMET	R5	R5	K10
      0x7C140200,  //  0019  CALL	R5	1
      0x7C100200,  //  001A  CALL	R4	1
      0xA8020042,  //  001B  EXBLK	0	#005F
      0x5C140800,  //  001C  MOVE	R5	R4
      0x7C140000,  //  001D  CALL	R5	0
      0x5C180600,  //  001E  MOVE	R6	R3
      0x741A0002,  //  001F  JMPT	R6	#0023
      0x8C180302,  //  0020  GETMET	R6	R1	K2
      0x5820000B,  //  0021  LDCONST	R8	K11
      0x7C180400,  //  0022  CALL	R6	2
      0x500C0000,  //  0023  LDBOOL	R3	0	0
      0x88180B0C,  //  0024  GETMBR	R6	R5	K12
      0x5C1C0C00,  //  0025  MOVE	R7	R6
      0x741E0000,  //  0026  JMPT	R7	#0028
      0x5818000D,  //  0027  LDCONST	R6	K13
      0x8C1C030E,  //  0028  GETMET	R7	R1	K14
      0x5C240C00,  //  0029  MOVE	R9	R6
      0x7C1C0400,  //  002A  CALL	R7	2
      0x5C180E00,  //  002B  MOVE	R6	R7
      0x8C1C0302,  //  002C  GETMET	R7	R1	K2
      0x8C24050F,  //  002D  GETMET	R9	R2	K15
      0x582C0010,  //  002E  LDCONST	R11	K16
      0x8C300B11,  //  002F  GETMET	R12	R5	K17
      0x7C300200,  //  0030  CALL	R12	1
      0x5C340C00,  //  0031  MOVE	R13	R6
      0x7C240800,  //  0032  CALL	R9	4
      0x7C1C0400,  //  0033  CALL	R7	2
      0x8C1C0B12,  //  0034  GETMET	R7	R5	K18
      0x7C1C0200,  //  0035  CALL	R7	1
      0x8C1C0F13,  //  0036  GETMET	R7	R7	K19
      0x7C1C0200,  //  0037  CALL	R7	1
      0x8C1C0F14,  //  0038  GETMET	R7	R7	K20
      0x7C1C0200,  //  0039  CALL	R7	1
      0x8C200B15,  //  003A  GETMET	R8	R5	K21
      0x7C200200,  //  003B  CALL	R8	1
      0x8C201113,  //  003C  GETMET	R8	R8	K19
      0x7C200200,  //  003D  CALL	R8	1
      0x8C201114,  //  003E  GETMET	R8	R8	K20
      0x7C200200,  //  003F  CALL	R8	1
      0x8C240302,  //  0040  GETMET	R9	R1	K2
      0x8C2C050F,  //  0041  GETMET	R11	R2	K15
      0x58340016,  //  0042  LDCONST	R13	K22
      0x8C380F17,  //  0043  GETMET	R14	R7	K23
      0x7C380200,  //  0044  CALL	R14	1
      0x7C2C0600,  //  0045  CALL	R11	3
      0x7C240400,  //  0046  CALL	R9	2
      0x8C240302,  //  0047  GETMET	R9	R1	K2
      0x8C2C050F,  //  0048  GETMET	R11	R2	K15
      0x58340018,  //  0049  LDCONST	R13	K24
      0x8C381117,  //  004A  GETMET	R14	R8	K23
      0x7C380200,  //  004B  CALL	R14	1
      0x7C2C0600,  //  004C  CALL	R11	3
      0x7C240400,  //  004D  CALL	R9	2
      0x8C240302,  //  004E  GETMET	R9	R1	K2
      0x582C0019,  //  004F  LDCONST	R11	K25
      0x7C240400,  //  0050  CALL	R9	2
      0x8C240302,  //  0051  GETMET	R9	R1	K2
      0x8C2C050F,  //  0052  GETMET	R11	R2	K15
      0x5834001A,  //  0053  LDCONST	R13	K26
      0x8C380B11,  //  0054  GETMET	R14	R5	K17
      0x7C380200,  //  0055  CALL	R14	1
      0x7C2C0600,  //  0056  CALL	R11	3
      0x7C240400,  //  0057  CALL	R9	2
      0x8C240302,  //  0058  GETMET	R9	R1	K2
      0x582C001B,  //  0059  LDCONST	R11	K27
      0x7C240400,  //  005A  CALL	R9	2
      0x8C240302,  //  005B  GETMET	R9	R1	K2
      0x582C001C,  //  005C  LDCONST	R11	K28
      0x7C240400,  //  005D  CALL	R9	2
      0x7001FFBC,  //  005E  JMP		#001C
      0x5810001D,  //  005F  LDCONST	R4	K29
      0xAC100200,  //  0060  CATCH	R4	1	0
      0xB0080000,  //  0061  RAISE	2	R0	R0
      0x8C0C0302,  //  0062  GETMET	R3	R1	K2
      0x5814001C,  //  0063  LDCONST	R5	K28
      0x7C0C0400,  //  0064  CALL	R3	2
      0x80000000,  //  0065  RET	0
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
    be_nested_map(19,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_CLASSES_TYPES, -1), be_nested_str_weak(_X7Crelay_X7Clight0_X7Clight1_X7Clight2_X7Clight3_X7Cshutter_X7Cshutter_X2Btilt_X7Ctemperature_X7Cpressure_X7Cilluminance_X7Chumidity) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(show_enable, -1), be_const_closure(Matter_UI_show_enable_closure) },
        { be_const_key_weak(plugin_option, -1), be_const_closure(Matter_UI_plugin_option_closure) },
        { be_const_key_weak(show_bridge_status, -1), be_const_closure(Matter_UI_show_bridge_status_closure) },
        { be_const_key_weak(show_qrcode, -1), be_const_closure(Matter_UI_show_qrcode_closure) },
        { be_const_key_weak(web_get_arg, -1), be_const_closure(Matter_UI_web_get_arg_closure) },
        { be_const_key_weak(show_plugins_configuration, 1), be_const_closure(Matter_UI_show_plugins_configuration_closure) },
        { be_const_key_weak(web_sensor, -1), be_const_closure(Matter_UI_web_sensor_closure) },
        { be_const_key_weak(show_passcode_form, 2), be_const_closure(Matter_UI_show_passcode_form_closure) },
        { be_const_key_weak(web_add_handler, -1), be_const_closure(Matter_UI_web_add_handler_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_UI_init_closure) },
        { be_const_key_weak(_ROOT_TYPES, 6), be_nested_str_weak(root) },
        { be_const_key_weak(page_part_ctl, 11), be_const_closure(Matter_UI_page_part_ctl_closure) },
        { be_const_key_weak(web_add_config_button, 8), be_const_closure(Matter_UI_web_add_config_button_closure) },
        { be_const_key_weak(page_part_mgr, 7), be_const_closure(Matter_UI_page_part_mgr_closure) },
        { be_const_key_weak(show_commissioning_info, -1), be_const_closure(Matter_UI_show_commissioning_info_closure) },
        { be_const_key_weak(_CLASSES_TYPES2, -1), be_nested_str_weak(_X2Dhttp_X7Chttp_relay_X7Chttp_light0_X7Chttp_light1_X7Chttp_light2_X7Chttp_light3_X7Chttp_temperature_X7Chttp_pressure_X7Chttp_illuminance_X7Chttp_humidity) },
        { be_const_key_weak(show_fabric_info, -1), be_const_closure(Matter_UI_show_fabric_info_closure) },
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
