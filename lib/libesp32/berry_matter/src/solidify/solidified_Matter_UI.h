/* Solidification of Matter_UI.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_UI;

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
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BMatter_X20Passcode_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(compute_manual_pairing_code),
    /* K6   */  be_nested_str_weak(format),
    /* K7   */  be_nested_str_weak(_X3Cp_X3EManual_X20pairing_X20code_X3A_X3Cbr_X3E_X3Cb_X3E_X25s_X2D_X25s_X2D_X25s_X3C_X2Fb_X3E_X3C_X2Fp_X3E_X3Chr_X3E),
    /* K8   */  be_const_int(0),
    /* K9   */  be_const_int(3),
    /* K10  */  be_const_int(2147483647),
    /* K11  */  be_nested_str_weak(compute_qrcode_content),
    /* K12  */  be_nested_str_weak(_X3Cdiv_X20id_X3D_X22qrcode_X22_X3E_X3C_X2Fdiv_X3E),
    /* K13  */  be_nested_str_weak(_X3Cscript_X20type_X3D_X22text_X2Fjavascript_X22_X3E_X20new_X20QRCode_X28document_X2EgetElementById_X28_X22qrcode_X22_X29_X2C_X20_X22_X25s_X22_X29_X3B_X3C_X2Fscript_X3E),
    /* K14  */  be_nested_str_weak(_X3Cp_X3E_X25s_X3C_X2Fp_X3E_X3Chr_X3E),
    /* K15  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20_X3E),
    /* K16  */  be_nested_str_weak(_X3Cp_X3EPasscode_X3A_X3C_X2Fp_X3E),
    /* K17  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X271_X27_X20max_X3D_X2799999998_X27_X20name_X3D_X27passcode_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K18  */  be_nested_str_weak(passcode),
    /* K19  */  be_nested_str_weak(_X3Cp_X3EDistinguish_X20id_X3A_X3C_X2Fp_X3E),
    /* K20  */  be_nested_str_weak(_X3Cinput_X20type_X3D_X27number_X27_X20min_X3D_X270_X27_X20max_X3D_X272047_X27_X20name_X3D_X27discriminator_X27_X20value_X3D_X27_X25i_X27_X3E),
    /* K21  */  be_nested_str_weak(discriminator),
    /* K22  */  be_nested_str_weak(_X3Cp_X3E_X3Cinput_X20type_X3D_X27checkbox_X27_X20name_X3D_X27ipv4_X27_X25s_X3EIPv4_X20only_X3C_X2Fp_X3E),
    /* K23  */  be_nested_str_weak(ipv4only),
    /* K24  */  be_nested_str_weak(_X20checked),
    /* K25  */  be_nested_str_weak(),
    /* K26  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3Cbutton_X20name_X3D_X27passcode_X27_X20class_X3D_X27button_X20bgrn_X27_X3EChange_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E),
    /* K27  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    }),
    be_str_weak(show_commissioning_info),
    &be_const_str_solidified,
    ( &(const binstruction[81]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x880C0104,  //  0005  GETMBR	R3	R0	K4
      0x8C0C0705,  //  0006  GETMET	R3	R3	K5
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C100302,  //  0008  GETMET	R4	R1	K2
      0x8C180506,  //  0009  GETMET	R6	R2	K6
      0x58200007,  //  000A  LDCONST	R8	K7
      0x40261109,  //  000B  CONNECT	R9	K8	K9
      0x94240609,  //  000C  GETIDX	R9	R3	R9
      0x542A0003,  //  000D  LDINT	R10	4
      0x542E0005,  //  000E  LDINT	R11	6
      0x4028140B,  //  000F  CONNECT	R10	R10	R11
      0x9428060A,  //  0010  GETIDX	R10	R3	R10
      0x542E0006,  //  0011  LDINT	R11	7
      0x402C170A,  //  0012  CONNECT	R11	R11	K10
      0x942C060B,  //  0013  GETIDX	R11	R3	R11
      0x7C180A00,  //  0014  CALL	R6	5
      0x7C100400,  //  0015  CALL	R4	2
      0x88100104,  //  0016  GETMBR	R4	R0	K4
      0x8C10090B,  //  0017  GETMET	R4	R4	K11
      0x7C100200,  //  0018  CALL	R4	1
      0x8C140302,  //  0019  GETMET	R5	R1	K2
      0x581C000C,  //  001A  LDCONST	R7	K12
      0x7C140400,  //  001B  CALL	R5	2
      0x8C140302,  //  001C  GETMET	R5	R1	K2
      0x8C1C0506,  //  001D  GETMET	R7	R2	K6
      0x5824000D,  //  001E  LDCONST	R9	K13
      0x5C280800,  //  001F  MOVE	R10	R4
      0x7C1C0600,  //  0020  CALL	R7	3
      0x7C140400,  //  0021  CALL	R5	2
      0x8C140302,  //  0022  GETMET	R5	R1	K2
      0x8C1C0506,  //  0023  GETMET	R7	R2	K6
      0x5824000E,  //  0024  LDCONST	R9	K14
      0x5C280800,  //  0025  MOVE	R10	R4
      0x7C1C0600,  //  0026  CALL	R7	3
      0x7C140400,  //  0027  CALL	R5	2
      0x8C140302,  //  0028  GETMET	R5	R1	K2
      0x581C000F,  //  0029  LDCONST	R7	K15
      0x7C140400,  //  002A  CALL	R5	2
      0x8C140302,  //  002B  GETMET	R5	R1	K2
      0x581C0010,  //  002C  LDCONST	R7	K16
      0x7C140400,  //  002D  CALL	R5	2
      0x8C140302,  //  002E  GETMET	R5	R1	K2
      0x8C1C0506,  //  002F  GETMET	R7	R2	K6
      0x58240011,  //  0030  LDCONST	R9	K17
      0x88280104,  //  0031  GETMBR	R10	R0	K4
      0x88281512,  //  0032  GETMBR	R10	R10	K18
      0x7C1C0600,  //  0033  CALL	R7	3
      0x7C140400,  //  0034  CALL	R5	2
      0x8C140302,  //  0035  GETMET	R5	R1	K2
      0x581C0013,  //  0036  LDCONST	R7	K19
      0x7C140400,  //  0037  CALL	R5	2
      0x8C140302,  //  0038  GETMET	R5	R1	K2
      0x8C1C0506,  //  0039  GETMET	R7	R2	K6
      0x58240014,  //  003A  LDCONST	R9	K20
      0x88280104,  //  003B  GETMBR	R10	R0	K4
      0x88281515,  //  003C  GETMBR	R10	R10	K21
      0x7C1C0600,  //  003D  CALL	R7	3
      0x7C140400,  //  003E  CALL	R5	2
      0x8C140302,  //  003F  GETMET	R5	R1	K2
      0x8C1C0506,  //  0040  GETMET	R7	R2	K6
      0x58240016,  //  0041  LDCONST	R9	K22
      0x88280104,  //  0042  GETMBR	R10	R0	K4
      0x88281517,  //  0043  GETMBR	R10	R10	K23
      0x782A0001,  //  0044  JMPF	R10	#0047
      0x58280018,  //  0045  LDCONST	R10	K24
      0x70020000,  //  0046  JMP		#0048
      0x58280019,  //  0047  LDCONST	R10	K25
      0x7C1C0600,  //  0048  CALL	R7	3
      0x7C140400,  //  0049  CALL	R5	2
      0x8C140302,  //  004A  GETMET	R5	R1	K2
      0x581C001A,  //  004B  LDCONST	R7	K26
      0x7C140400,  //  004C  CALL	R5	2
      0x8C140302,  //  004D  GETMET	R5	R1	K2
      0x581C001B,  //  004E  LDCONST	R7	K27
      0x7C140400,  //  004F  CALL	R5	2
      0x80000000,  //  0050  RET	0
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
    ( &(const struct bproto*[ 3]) {
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
        /* K0   */  be_nested_str_weak(page_qrcode_min_js),
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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(on),
    /* K2   */  be_nested_str_weak(_X2Fmatterc),
    /* K3   */  be_nested_str_weak(HTTP_GET),
    /* K4   */  be_nested_str_weak(HTTP_POST),
    /* K5   */  be_nested_str_weak(_X2Fqrcode_X2Emin_X2Ejs),
    }),
    be_str_weak(web_add_handler),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
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
      0xA0000000,  //  0010  CLOSE	R0
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show_session_info
********************************************************************/
be_local_closure(Matter_UI_show_session_info,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BSessions_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K4   */  be_nested_str_weak(_X3Cp_X3EExisting_X20sessions_X3A_X3C_X2Fp_X3E),
    /* K5   */  be_nested_str_weak(device),
    /* K6   */  be_nested_str_weak(sessions),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(_X3Cp_X3E_X3Cb_X3ENone_X3C_X2Fb_X3E_X3C_X2Fp_X3E),
    /* K9   */  be_nested_str_weak(fabric),
    /* K10  */  be_nested_str_weak(format),
    /* K11  */  be_nested_str_weak(_X3Cfieldset_X3E_X3Clegend_X3E_X3Cb_X3E_X26nbsp_X3BSession_X20_X25i_X26nbsp_X3B_X3C_X2Fb_X3E_X3C_X2Flegend_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K12  */  be_nested_str_weak(local_session_id),
    /* K13  */  be_nested_str_weak(_X3Chr_X3E),
    /* K14  */  be_nested_str_weak(copy),
    /* K15  */  be_nested_str_weak(reverse),
    /* K16  */  be_nested_str_weak(deviceid),
    /* K17  */  be_nested_str_weak(Fabric_X3A_X20_X25s_X3Cbr_X3E),
    /* K18  */  be_nested_str_weak(tohex),
    /* K19  */  be_nested_str_weak(Device_X3A_X20_X25s_X3Cbr_X3E_X26nbsp_X3B),
    /* K20  */  be_nested_str_weak(_X3Cform_X20action_X3D_X27_X2Fmatterc_X27_X20method_X3D_X27post_X27_X20),
    /* K21  */  be_nested_str_weak(onsubmit_X3D_X27return_X20confirm_X28_X22This_X20will_X20cause_X20a_X20restart_X2E_X22_X29_X3B_X27_X3E),
    /* K22  */  be_nested_str_weak(_X3Cinput_X20name_X3D_X27del_session_X27_X20type_X3D_X27hidden_X27_X20value_X3D_X27_X25d_X27_X3E),
    /* K23  */  be_nested_str_weak(_X3Cbutton_X20name_X3D_X27del_X27_X20class_X3D_X27button_X20bgrn_X27_X3EDelete_X20Session_X3C_X2Fbutton_X3E_X3C_X2Fform_X3E_X3C_X2Fp_X3E),
    /* K24  */  be_nested_str_weak(_X3Cp_X3E_X3C_X2Fp_X3E_X3C_X2Ffieldset_X3E_X3Cp_X3E_X3C_X2Fp_X3E),
    /* K25  */  be_const_int(1),
    }),
    be_str_weak(show_session_info),
    &be_const_str_solidified,
    ( &(const binstruction[92]) {  /* code */
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
      0x70020044,  //  0012  JMP		#0058
      0x58100007,  //  0013  LDCONST	R4	K7
      0x6014000C,  //  0014  GETGBL	R5	G12
      0x88180105,  //  0015  GETMBR	R6	R0	K5
      0x88180D06,  //  0016  GETMBR	R6	R6	K6
      0x88180D06,  //  0017  GETMBR	R6	R6	K6
      0x7C140200,  //  0018  CALL	R5	1
      0x14180805,  //  0019  LT	R6	R4	R5
      0x781A003C,  //  001A  JMPF	R6	#0058
      0x88180105,  //  001B  GETMBR	R6	R0	K5
      0x88180D06,  //  001C  GETMBR	R6	R6	K6
      0x88180D06,  //  001D  GETMBR	R6	R6	K6
      0x94180C04,  //  001E  GETIDX	R6	R6	R4
      0x881C0D09,  //  001F  GETMBR	R7	R6	K9
      0x781E0034,  //  0020  JMPF	R7	#0056
      0x8C1C0502,  //  0021  GETMET	R7	R2	K2
      0x8C24070A,  //  0022  GETMET	R9	R3	K10
      0x582C000B,  //  0023  LDCONST	R11	K11
      0x88300D0C,  //  0024  GETMBR	R12	R6	K12
      0x7C240600,  //  0025  CALL	R9	3
      0x7C1C0400,  //  0026  CALL	R7	2
      0x201C0907,  //  0027  NE	R7	R4	K7
      0x781E0002,  //  0028  JMPF	R7	#002C
      0x8C1C0502,  //  0029  GETMET	R7	R2	K2
      0x5824000D,  //  002A  LDCONST	R9	K13
      0x7C1C0400,  //  002B  CALL	R7	2
      0x881C0D09,  //  002C  GETMBR	R7	R6	K9
      0x8C1C0F0E,  //  002D  GETMET	R7	R7	K14
      0x7C1C0200,  //  002E  CALL	R7	1
      0x8C1C0F0F,  //  002F  GETMET	R7	R7	K15
      0x7C1C0200,  //  0030  CALL	R7	1
      0x88200D10,  //  0031  GETMBR	R8	R6	K16
      0x8C20110E,  //  0032  GETMET	R8	R8	K14
      0x7C200200,  //  0033  CALL	R8	1
      0x8C20110F,  //  0034  GETMET	R8	R8	K15
      0x7C200200,  //  0035  CALL	R8	1
      0x8C240502,  //  0036  GETMET	R9	R2	K2
      0x8C2C070A,  //  0037  GETMET	R11	R3	K10
      0x58340011,  //  0038  LDCONST	R13	K17
      0x8C380F12,  //  0039  GETMET	R14	R7	K18
      0x7C380200,  //  003A  CALL	R14	1
      0x7C2C0600,  //  003B  CALL	R11	3
      0x7C240400,  //  003C  CALL	R9	2
      0x8C240502,  //  003D  GETMET	R9	R2	K2
      0x8C2C070A,  //  003E  GETMET	R11	R3	K10
      0x58340013,  //  003F  LDCONST	R13	K19
      0x8C381112,  //  0040  GETMET	R14	R8	K18
      0x7C380200,  //  0041  CALL	R14	1
      0x7C2C0600,  //  0042  CALL	R11	3
      0x7C240400,  //  0043  CALL	R9	2
      0x8C240502,  //  0044  GETMET	R9	R2	K2
      0x582C0014,  //  0045  LDCONST	R11	K20
      0x7C240400,  //  0046  CALL	R9	2
      0x8C240502,  //  0047  GETMET	R9	R2	K2
      0x582C0015,  //  0048  LDCONST	R11	K21
      0x7C240400,  //  0049  CALL	R9	2
      0x8C240502,  //  004A  GETMET	R9	R2	K2
      0x8C2C070A,  //  004B  GETMET	R11	R3	K10
      0x58340016,  //  004C  LDCONST	R13	K22
      0x88380D0C,  //  004D  GETMBR	R14	R6	K12
      0x7C2C0600,  //  004E  CALL	R11	3
      0x7C240400,  //  004F  CALL	R9	2
      0x8C240502,  //  0050  GETMET	R9	R2	K2
      0x582C0017,  //  0051  LDCONST	R11	K23
      0x7C240400,  //  0052  CALL	R9	2
      0x8C240502,  //  0053  GETMET	R9	R2	K2
      0x582C0018,  //  0054  LDCONST	R11	K24
      0x7C240400,  //  0055  CALL	R9	2
      0x00100919,  //  0056  ADD	R4	R4	K25
      0x7001FFC0,  //  0057  JMP		#0019
      0x8C100502,  //  0058  GETMET	R4	R2	K2
      0x58180018,  //  0059  LDCONST	R6	K24
      0x7C100400,  //  005A  CALL	R4	2
      0x80000000,  //  005B  RET	0
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
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[43]) {     /* constants */
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
    /* K10  */  be_nested_str_weak(arg),
    /* K11  */  be_nested_str_weak(ipv4only),
    /* K12  */  be_nested_str_weak(ipv4),
    /* K13  */  be_nested_str_weak(on),
    /* K14  */  be_nested_str_weak(save_param),
    /* K15  */  be_nested_str_weak(redirect),
    /* K16  */  be_nested_str_weak(_X2F_X3Frst_X3D),
    /* K17  */  be_nested_str_weak(enable),
    /* K18  */  be_nested_str_weak(tasmota),
    /* K19  */  be_nested_str_weak(cmd),
    /* K20  */  be_nested_str_weak(SetOption),
    /* K21  */  be_nested_str_weak(matter),
    /* K22  */  be_nested_str_weak(MATTER_OPTION),
    /* K23  */  be_nested_str_weak(_X201),
    /* K24  */  be_nested_str_weak(disable),
    /* K25  */  be_nested_str_weak(_X200),
    /* K26  */  be_nested_str_weak(del_session),
    /* K27  */  be_nested_str_weak(sessions),
    /* K28  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K29  */  be_nested_str_weak(remove_session),
    /* K30  */  be_nested_str_weak(save),
    /* K31  */  be_nested_str_weak(log),
    /* K32  */  be_nested_str_weak(format),
    /* K33  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K34  */  be_const_int(2),
    /* K35  */  be_nested_str_weak(content_start),
    /* K36  */  be_nested_str_weak(Parameter_X20error),
    /* K37  */  be_nested_str_weak(content_send_style),
    /* K38  */  be_nested_str_weak(content_send),
    /* K39  */  be_nested_str_weak(_X3Cp_X20style_X3D_X27width_X3A340px_X3B_X27_X3E_X3Cb_X3EException_X3A_X3C_X2Fb_X3E_X3Cbr_X3E_X27_X25s_X27_X3Cbr_X3E_X25s_X3C_X2Fp_X3E),
    /* K40  */  be_nested_str_weak(content_button),
    /* K41  */  be_nested_str_weak(BUTTON_MANAGEMENT),
    /* K42  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_ctl),
    &be_const_str_solidified,
    ( &(const binstruction[150]) {  /* code */
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
      0xA802006A,  //  000B  EXBLK	0	#0077
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
      0x8C20030A,  //  001A  GETMET	R8	R1	K10
      0x58280007,  //  001B  LDCONST	R10	K7
      0x7C200400,  //  001C  CALL	R8	2
      0x7C1C0200,  //  001D  CALL	R7	1
      0x901A0E07,  //  001E  SETMBR	R6	K7	R7
      0x8C180306,  //  001F  GETMET	R6	R1	K6
      0x58200008,  //  0020  LDCONST	R8	K8
      0x7C180400,  //  0021  CALL	R6	2
      0x781A0006,  //  0022  JMPF	R6	#002A
      0x88180109,  //  0023  GETMBR	R6	R0	K9
      0x601C0009,  //  0024  GETGBL	R7	G9
      0x8C20030A,  //  0025  GETMET	R8	R1	K10
      0x58280008,  //  0026  LDCONST	R10	K8
      0x7C200400,  //  0027  CALL	R8	2
      0x7C1C0200,  //  0028  CALL	R7	1
      0x901A1007,  //  0029  SETMBR	R6	K8	R7
      0x88180109,  //  002A  GETMBR	R6	R0	K9
      0x8C1C030A,  //  002B  GETMET	R7	R1	K10
      0x5824000C,  //  002C  LDCONST	R9	K12
      0x7C1C0400,  //  002D  CALL	R7	2
      0x1C1C0F0D,  //  002E  EQ	R7	R7	K13
      0x901A1607,  //  002F  SETMBR	R6	K11	R7
      0x88180109,  //  0030  GETMBR	R6	R0	K9
      0x8C180D0E,  //  0031  GETMET	R6	R6	K14
      0x7C180200,  //  0032  CALL	R6	1
      0x8C18030F,  //  0033  GETMET	R6	R1	K15
      0x58200010,  //  0034  LDCONST	R8	K16
      0x7C180400,  //  0035  CALL	R6	2
      0x7002003D,  //  0036  JMP		#0075
      0x8C180306,  //  0037  GETMET	R6	R1	K6
      0x58200011,  //  0038  LDCONST	R8	K17
      0x7C180400,  //  0039  CALL	R6	2
      0x781A000C,  //  003A  JMPF	R6	#0048
      0xB81A2400,  //  003B  GETNGBL	R6	K18
      0x8C180D13,  //  003C  GETMET	R6	R6	K19
      0x60200008,  //  003D  GETGBL	R8	G8
      0xB8262A00,  //  003E  GETNGBL	R9	K21
      0x88241316,  //  003F  GETMBR	R9	R9	K22
      0x7C200200,  //  0040  CALL	R8	1
      0x00222808,  //  0041  ADD	R8	K20	R8
      0x00201117,  //  0042  ADD	R8	R8	K23
      0x7C180400,  //  0043  CALL	R6	2
      0x8C18030F,  //  0044  GETMET	R6	R1	K15
      0x58200010,  //  0045  LDCONST	R8	K16
      0x7C180400,  //  0046  CALL	R6	2
      0x7002002C,  //  0047  JMP		#0075
      0x8C180306,  //  0048  GETMET	R6	R1	K6
      0x58200018,  //  0049  LDCONST	R8	K24
      0x7C180400,  //  004A  CALL	R6	2
      0x781A000C,  //  004B  JMPF	R6	#0059
      0xB81A2400,  //  004C  GETNGBL	R6	K18
      0x8C180D13,  //  004D  GETMET	R6	R6	K19
      0x60200008,  //  004E  GETGBL	R8	G8
      0xB8262A00,  //  004F  GETNGBL	R9	K21
      0x88241316,  //  0050  GETMBR	R9	R9	K22
      0x7C200200,  //  0051  CALL	R8	1
      0x00222808,  //  0052  ADD	R8	K20	R8
      0x00201119,  //  0053  ADD	R8	R8	K25
      0x7C180400,  //  0054  CALL	R6	2
      0x8C18030F,  //  0055  GETMET	R6	R1	K15
      0x58200010,  //  0056  LDCONST	R8	K16
      0x7C180400,  //  0057  CALL	R6	2
      0x7002001B,  //  0058  JMP		#0075
      0x8C180306,  //  0059  GETMET	R6	R1	K6
      0x5820001A,  //  005A  LDCONST	R8	K26
      0x7C180400,  //  005B  CALL	R6	2
      0x781A0017,  //  005C  JMPF	R6	#0075
      0x88180109,  //  005D  GETMBR	R6	R0	K9
      0x88180D1B,  //  005E  GETMBR	R6	R6	K27
      0x8C180D1C,  //  005F  GETMET	R6	R6	K28
      0x60200009,  //  0060  GETGBL	R8	G9
      0x8C24030A,  //  0061  GETMET	R9	R1	K10
      0x582C001A,  //  0062  LDCONST	R11	K26
      0x7C240400,  //  0063  CALL	R9	2
      0x7C200200,  //  0064  CALL	R8	1
      0x7C180400,  //  0065  CALL	R6	2
      0x4C1C0000,  //  0066  LDNIL	R7
      0x201C0C07,  //  0067  NE	R7	R6	R7
      0x781E0008,  //  0068  JMPF	R7	#0072
      0x881C0109,  //  0069  GETMBR	R7	R0	K9
      0x881C0F1B,  //  006A  GETMBR	R7	R7	K27
      0x8C1C0F1D,  //  006B  GETMET	R7	R7	K29
      0x5C240C00,  //  006C  MOVE	R9	R6
      0x7C1C0400,  //  006D  CALL	R7	2
      0x881C0109,  //  006E  GETMBR	R7	R0	K9
      0x881C0F1B,  //  006F  GETMBR	R7	R7	K27
      0x8C1C0F1E,  //  0070  GETMET	R7	R7	K30
      0x7C1C0200,  //  0071  CALL	R7	1
      0x8C1C030F,  //  0072  GETMET	R7	R1	K15
      0x58240010,  //  0073  LDCONST	R9	K16
      0x7C1C0400,  //  0074  CALL	R7	2
      0xA8040001,  //  0075  EXBLK	1	1
      0x7002001D,  //  0076  JMP		#0095
      0xAC180002,  //  0077  CATCH	R6	0	2
      0x7002001A,  //  0078  JMP		#0094
      0xB8222400,  //  0079  GETNGBL	R8	K18
      0x8C20111F,  //  007A  GETMET	R8	R8	K31
      0x8C280520,  //  007B  GETMET	R10	R2	K32
      0x58300021,  //  007C  LDCONST	R12	K33
      0x5C340C00,  //  007D  MOVE	R13	R6
      0x5C380E00,  //  007E  MOVE	R14	R7
      0x7C280800,  //  007F  CALL	R10	4
      0x582C0022,  //  0080  LDCONST	R11	K34
      0x7C200600,  //  0081  CALL	R8	3
      0x8C200323,  //  0082  GETMET	R8	R1	K35
      0x58280024,  //  0083  LDCONST	R10	K36
      0x7C200400,  //  0084  CALL	R8	2
      0x8C200325,  //  0085  GETMET	R8	R1	K37
      0x7C200200,  //  0086  CALL	R8	1
      0x8C200326,  //  0087  GETMET	R8	R1	K38
      0x8C280520,  //  0088  GETMET	R10	R2	K32
      0x58300027,  //  0089  LDCONST	R12	K39
      0x5C340C00,  //  008A  MOVE	R13	R6
      0x5C380E00,  //  008B  MOVE	R14	R7
      0x7C280800,  //  008C  CALL	R10	4
      0x7C200400,  //  008D  CALL	R8	2
      0x8C200328,  //  008E  GETMET	R8	R1	K40
      0x88280329,  //  008F  GETMBR	R10	R1	K41
      0x7C200400,  //  0090  CALL	R8	2
      0x8C20032A,  //  0091  GETMET	R8	R1	K42
      0x7C200200,  //  0092  CALL	R8	1
      0x70020000,  //  0093  JMP		#0095
      0xB0080000,  //  0094  RAISE	2	R0	R0
      0x80000000,  //  0095  RET	0
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
** Solidified function: page_qrcode_min_js
********************************************************************/
be_local_closure(Matter_UI_page_qrcode_min_js,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(content_open),
    /* K2   */  be_nested_str_weak(text_X2Fjavascript),
    /* K3   */  be_nested_str_weak(content_send),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(_QRCODE_MINJS),
    }),
    be_str_weak(page_qrcode_min_js),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x541200C7,  //  0002  LDINT	R4	200
      0x58140002,  //  0003  LDCONST	R5	K2
      0x7C080600,  //  0004  CALL	R2	3
      0x8C080303,  //  0005  GETMET	R2	R1	K3
      0xB8120800,  //  0006  GETNGBL	R4	K4
      0x88100905,  //  0007  GETMBR	R4	R4	K5
      0x7C080400,  //  0008  CALL	R2	2
      0x80000000,  //  0009  RET	0
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(check_privileged_access),
    /* K3   */  be_nested_str_weak(content_start),
    /* K4   */  be_nested_str_weak(Matter),
    /* K5   */  be_nested_str_weak(content_send_style),
    /* K6   */  be_nested_str_weak(content_send),
    /* K7   */  be_nested_str_weak(_X3Cscript_X20type_X3D_X22text_X2Fjavascript_X22_X20src_X3D_X22qrcode_X2Emin_X2Ejs_X22_X3E_X3C_X2Fscript_X3E),
    /* K8   */  be_nested_str_weak(show_enable),
    /* K9   */  be_nested_str_weak(show_commissioning_info),
    /* K10  */  be_nested_str_weak(show_session_info),
    /* K11  */  be_nested_str_weak(content_button),
    /* K12  */  be_nested_str_weak(BUTTON_CONFIGURATION),
    /* K13  */  be_nested_str_weak(content_stop),
    }),
    be_str_weak(page_part_mgr),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
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
      0x8C0C0108,  //  000F  GETMET	R3	R0	K8
      0x7C0C0200,  //  0010  CALL	R3	1
      0x780E0003,  //  0011  JMPF	R3	#0016
      0x8C0C0109,  //  0012  GETMET	R3	R0	K9
      0x7C0C0200,  //  0013  CALL	R3	1
      0x8C0C010A,  //  0014  GETMET	R3	R0	K10
      0x7C0C0200,  //  0015  CALL	R3	1
      0x8C0C030B,  //  0016  GETMET	R3	R1	K11
      0x8814030C,  //  0017  GETMBR	R5	R1	K12
      0x7C0C0400,  //  0018  CALL	R3	2
      0x8C0C030D,  //  0019  GETMET	R3	R1	K13
      0x7C0C0200,  //  001A  CALL	R3	1
      0x80000000,  //  001B  RET	0
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
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Matter_UI_init_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(web_add_handler, -1), be_const_closure(Matter_UI_web_add_handler_closure) },
        { be_const_key_weak(page_part_ctl, -1), be_const_closure(Matter_UI_page_part_ctl_closure) },
        { be_const_key_weak(show_enable, -1), be_const_closure(Matter_UI_show_enable_closure) },
        { be_const_key_weak(show_commissioning_info, 7), be_const_closure(Matter_UI_show_commissioning_info_closure) },
        { be_const_key_weak(show_session_info, 3), be_const_closure(Matter_UI_show_session_info_closure) },
        { be_const_key_weak(web_add_config_button, 0), be_const_closure(Matter_UI_web_add_config_button_closure) },
        { be_const_key_weak(page_qrcode_min_js, -1), be_const_closure(Matter_UI_page_qrcode_min_js_closure) },
        { be_const_key_weak(page_part_mgr, -1), be_const_closure(Matter_UI_page_part_mgr_closure) },
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
