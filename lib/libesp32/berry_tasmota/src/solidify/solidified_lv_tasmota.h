/* Solidification of lv_tasmota.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(module_lv_tasmota_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[36]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(start),
    /* K2   */  be_nested_str_weak(_constants),
    /* K3   */  be_nested_str_weak(splash_init),
    /* K4   */  be_nested_str_weak(splash_remove),
    /* K5   */  be_nested_str_weak(splash),
    /* K6   */  be_nested_str_weak(_splash),
    /* K7   */  be_nested_str_weak(font_montserrat),
    /* K8   */  be_nested_str_weak(montserrat_font),
    /* K9   */  be_nested_str_weak(font_seg7),
    /* K10  */  be_nested_str_weak(seg7_font),
    /* K11  */  be_nested_str_weak(font_embedded),
    /* K12  */  be_nested_str_weak(load_freetype_font),
    /* K13  */  be_nested_str_weak(register_button_encoder),
    /* K14  */  be_nested_str_weak(screenshot),
    /* K15  */  be_nested_str_weak(version),
    /* K16  */  be_nested_str_weak(version_major),
    /* K17  */  be_nested_str_weak(clock),
    /* K18  */  be_nested_str_weak(lv_clock),
    /* K19  */  be_nested_str_weak(clock_icon),
    /* K20  */  be_nested_str_weak(lv_clock_icon),
    /* K21  */  be_nested_str_weak(signal_arcs),
    /* K22  */  be_nested_str_weak(lv_signal_arcs),
    /* K23  */  be_nested_str_weak(signal_bars),
    /* K24  */  be_nested_str_weak(lv_signal_bars),
    /* K25  */  be_nested_str_weak(wifi_arcs_icon),
    /* K26  */  be_nested_str_weak(lv_wifi_arcs_icon),
    /* K27  */  be_nested_str_weak(wifi_arcs),
    /* K28  */  be_nested_str_weak(lv_wifi_arcs),
    /* K29  */  be_nested_str_weak(wifi_bars_icon),
    /* K30  */  be_nested_str_weak(lv_wifi_bars_icon),
    /* K31  */  be_nested_str_weak(wifi_bars),
    /* K32  */  be_nested_str_weak(lv_wifi_bars),
    /* K33  */  be_nested_str_weak(tasmota),
    /* K34  */  be_nested_str_weak(get_option),
    /* K35  */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x90060202,  //  0002  SETMBR	R1	K1	R2
      0x88080102,  //  0003  GETMBR	R2	R0	K2
      0x90060402,  //  0004  SETMBR	R1	K2	R2
      0x88080103,  //  0005  GETMBR	R2	R0	K3
      0x90060602,  //  0006  SETMBR	R1	K3	R2
      0x88080104,  //  0007  GETMBR	R2	R0	K4
      0x90060802,  //  0008  SETMBR	R1	K4	R2
      0x88080105,  //  0009  GETMBR	R2	R0	K5
      0x90060A02,  //  000A  SETMBR	R1	K5	R2
      0x4C080000,  //  000B  LDNIL	R2
      0x90060C02,  //  000C  SETMBR	R1	K6	R2
      0x88080107,  //  000D  GETMBR	R2	R0	K7
      0x90060E02,  //  000E  SETMBR	R1	K7	R2
      0x88080107,  //  000F  GETMBR	R2	R0	K7
      0x90061002,  //  0010  SETMBR	R1	K8	R2
      0x88080109,  //  0011  GETMBR	R2	R0	K9
      0x90061202,  //  0012  SETMBR	R1	K9	R2
      0x88080109,  //  0013  GETMBR	R2	R0	K9
      0x90061402,  //  0014  SETMBR	R1	K10	R2
      0x8808010B,  //  0015  GETMBR	R2	R0	K11
      0x90061602,  //  0016  SETMBR	R1	K11	R2
      0x8808010C,  //  0017  GETMBR	R2	R0	K12
      0x90061802,  //  0018  SETMBR	R1	K12	R2
      0x8808010D,  //  0019  GETMBR	R2	R0	K13
      0x90061A02,  //  001A  SETMBR	R1	K13	R2
      0x8808010E,  //  001B  GETMBR	R2	R0	K14
      0x90061C02,  //  001C  SETMBR	R1	K14	R2
      0x8C080310,  //  001D  GETMET	R2	R1	K16
      0x7C080200,  //  001E  CALL	R2	1
      0x90061E02,  //  001F  SETMBR	R1	K15	R2
      0xB80A2400,  //  0020  GETNGBL	R2	K18
      0x90062202,  //  0021  SETMBR	R1	K17	R2
      0xB80A2800,  //  0022  GETNGBL	R2	K20
      0x90062602,  //  0023  SETMBR	R1	K19	R2
      0xB80A2C00,  //  0024  GETNGBL	R2	K22
      0x90062A02,  //  0025  SETMBR	R1	K21	R2
      0xB80A3000,  //  0026  GETNGBL	R2	K24
      0x90062E02,  //  0027  SETMBR	R1	K23	R2
      0xB80A3400,  //  0028  GETNGBL	R2	K26
      0x90063202,  //  0029  SETMBR	R1	K25	R2
      0xB80A3800,  //  002A  GETNGBL	R2	K28
      0x90063602,  //  002B  SETMBR	R1	K27	R2
      0xB80A3C00,  //  002C  GETNGBL	R2	K30
      0x90063A02,  //  002D  SETMBR	R1	K29	R2
      0xB80A4000,  //  002E  GETNGBL	R2	K32
      0x90063E02,  //  002F  SETMBR	R1	K31	R2
      0xB80A4200,  //  0030  GETNGBL	R2	K33
      0x8C080522,  //  0031  GETMET	R2	R2	K34
      0x54120086,  //  0032  LDINT	R4	135
      0x7C080400,  //  0033  CALL	R2	2
      0x1C080523,  //  0034  EQ	R2	R2	K35
      0x780A0001,  //  0035  JMPF	R2	#0038
      0x8C080303,  //  0036  GETMET	R2	R1	K3
      0x7C080200,  //  0037  CALL	R2	1
      0x4C080000,  //  0038  LDNIL	R2
      0x80040400,  //  0039  RET	1	R2
    })
  )
);
/*******************************************************************/

// compact class 'splash_runner' ktab size: 8, total: 9 (saved 8 bytes)
static const bvalue be_ktab_class_splash_runner[8] = {
  /* K0   */  be_nested_str_weak(tasmota),
  /* K1   */  be_nested_str_weak(add_driver),
  /* K2   */  be_nested_str_weak(display),
  /* K3   */  be_nested_str_weak(dim),
  /* K4   */  be_nested_str_weak(started),
  /* K5   */  be_nested_str_weak(remove_driver),
  /* K6   */  be_nested_str_weak(lv),
  /* K7   */  be_nested_str_weak(splash),
};


extern const bclass be_class_splash_runner;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_splash_runner_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_splash_runner,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: display
********************************************************************/
be_local_closure(class_splash_runner_display,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_splash_runner,     /* shared constants */
    be_str_weak(display),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xA4160400,  //  0000  IMPORT	R5	K2
      0x1C180303,  //  0001  EQ	R6	R1	K3
      0x781A0009,  //  0002  JMPF	R6	#000D
      0x8C180B04,  //  0003  GETMET	R6	R5	K4
      0x7C180200,  //  0004  CALL	R6	1
      0x781A0006,  //  0005  JMPF	R6	#000D
      0xB81A0000,  //  0006  GETNGBL	R6	K0
      0x8C180D05,  //  0007  GETMET	R6	R6	K5
      0x5C200000,  //  0008  MOVE	R8	R0
      0x7C180400,  //  0009  CALL	R6	2
      0xB81A0C00,  //  000A  GETNGBL	R6	K6
      0x8C180D07,  //  000B  GETMET	R6	R6	K7
      0x7C180200,  //  000C  CALL	R6	1
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: splash_runner
********************************************************************/
be_local_class(splash_runner,
    0,
    NULL,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(display, -1), be_const_closure(class_splash_runner_display_closure) },
        { be_const_key_weak(init, 0), be_const_closure(class_splash_runner_init_closure) },
    })),
    be_str_weak(splash_runner)
);

/********************************************************************
** Solidified function: splash_init
********************************************************************/
be_local_closure(module_lv_tasmota_splash_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(display),
    /* K1   */  be_nested_str_weak(started),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(splash),
    /* K4   */  be_const_class(be_class_splash_runner),
    }),
    be_str_weak(splash_init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA4020000,  //  0000  IMPORT	R0	K0
      0x8C040101,  //  0001  GETMET	R1	R0	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x78060003,  //  0003  JMPF	R1	#0008
      0xB8060400,  //  0004  GETNGBL	R1	K2
      0x8C040303,  //  0005  GETMET	R1	R1	K3
      0x7C040200,  //  0006  CALL	R1	1
      0x70020003,  //  0007  JMP		#000C
      0x58040004,  //  0008  LDCONST	R1	K4
      0xB4000004,  //  0009  CLASS	K4
      0x5C080200,  //  000A  MOVE	R2	R1
      0x7C080000,  //  000B  CALL	R2	0
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: splash_remove
********************************************************************/
be_local_closure(module_lv_tasmota_splash_remove,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(_splash),
    /* K2   */  be_nested_str_weak(del),
    }),
    be_str_weak(splash_remove),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB8020000,  //  0000  GETNGBL	R0	K0
      0x88000101,  //  0001  GETMBR	R0	R0	K1
      0x78020004,  //  0002  JMPF	R0	#0008
      0xB8060000,  //  0003  GETNGBL	R1	K0
      0x4C080000,  //  0004  LDNIL	R2
      0x90060202,  //  0005  SETMBR	R1	K1	R2
      0x8C040102,  //  0006  GETMET	R1	R0	K2
      0x7C040200,  //  0007  CALL	R1	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: splash
********************************************************************/
be_local_closure(module_lv_tasmota_splash,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[43]) {     /* constants */
    /* K0   */  be_nested_str_weak(display),
    /* K1   */  be_nested_str_weak(started),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(start),
    /* K4   */  be_nested_str_weak(obj),
    /* K5   */  be_nested_str_weak(scr_act),
    /* K6   */  be_nested_str_weak(montserrat_font),
    /* K7   */  be_nested_str_weak(color),
    /* K8   */  be_nested_str_weak(COLOR_WHITE),
    /* K9   */  be_nested_str_weak(set_style_bg_color),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(set_style_radius),
    /* K12  */  be_nested_str_weak(set_style_pad_all),
    /* K13  */  be_nested_str_weak(set_style_border_width),
    /* K14  */  be_nested_str_weak(set_size),
    /* K15  */  be_nested_str_weak(pct),
    /* K16  */  be_nested_str_weak(refr_pos),
    /* K17  */  be_nested_str_weak(refr_size),
    /* K18  */  be_nested_str_weak(set_user_data),
    /* K19  */  be_const_int(1399876712),
    /* K20  */  be_nested_str_weak(img),
    /* K21  */  be_nested_str_weak(set_tasmota_logo),
    /* K22  */  be_nested_str_weak(set_zoom),
    /* K23  */  be_nested_str_weak(set_style_img_recolor_opa),
    /* K24  */  be_nested_str_weak(set_style_img_recolor),
    /* K25  */  be_nested_str_weak(align),
    /* K26  */  be_nested_str_weak(ALIGN_LEFT_MID),
    /* K27  */  be_nested_str_weak(label),
    /* K28  */  be_nested_str_weak(set_style_text_color),
    /* K29  */  be_nested_str_weak(set_text),
    /* K30  */  be_nested_str_weak(TASMOTA),
    /* K31  */  be_nested_str_weak(get_hor_res),
    /* K32  */  be_nested_str_weak(set_style_text_font),
    /* K33  */  be_nested_str_weak(set_align),
    /* K34  */  be_nested_str_weak(set_x),
    /* K35  */  be_nested_str_weak(driver_name),
    /* K36  */  be_nested_str_weak(ALIGN_BOTTOM_MID),
    /* K37  */  be_const_int(16777215),
    /* K38  */  be_nested_str_weak(refr_now),
    /* K39  */  be_nested_str_weak(_splash),
    /* K40  */  be_nested_str_weak(tasmota),
    /* K41  */  be_nested_str_weak(set_timer),
    /* K42  */  be_nested_str_weak(splash_remove),
    }),
    be_str_weak(splash),
    &be_const_str_solidified,
    ( &(const binstruction[165]) {  /* code */
      0xA4020000,  //  0000  IMPORT	R0	K0
      0x8C040101,  //  0001  GETMET	R1	R0	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x74060000,  //  0003  JMPT	R1	#0005
      0x80000200,  //  0004  RET	0
      0xB8060400,  //  0005  GETNGBL	R1	K2
      0x8C040303,  //  0006  GETMET	R1	R1	K3
      0x7C040200,  //  0007  CALL	R1	1
      0xB8060400,  //  0008  GETNGBL	R1	K2
      0x8C040304,  //  0009  GETMET	R1	R1	K4
      0xB80E0400,  //  000A  GETNGBL	R3	K2
      0x8C0C0705,  //  000B  GETMET	R3	R3	K5
      0x7C0C0200,  //  000C  CALL	R3	1
      0x7C040400,  //  000D  CALL	R1	2
      0xB80A0400,  //  000E  GETNGBL	R2	K2
      0x8C080506,  //  000F  GETMET	R2	R2	K6
      0x5412001B,  //  0010  LDINT	R4	28
      0x7C080400,  //  0011  CALL	R2	2
      0xB80E0400,  //  0012  GETNGBL	R3	K2
      0x8C0C0706,  //  0013  GETMET	R3	R3	K6
      0x54160013,  //  0014  LDINT	R5	20
      0x7C0C0400,  //  0015  CALL	R3	2
      0xB8120400,  //  0016  GETNGBL	R4	K2
      0x8C100907,  //  0017  GETMET	R4	R4	K7
      0xB81A0400,  //  0018  GETNGBL	R6	K2
      0x88180D08,  //  0019  GETMBR	R6	R6	K8
      0x7C100400,  //  001A  CALL	R4	2
      0x8C140309,  //  001B  GETMET	R5	R1	K9
      0xB81E0400,  //  001C  GETNGBL	R7	K2
      0x8C1C0F07,  //  001D  GETMET	R7	R7	K7
      0x54260065,  //  001E  LDINT	R9	102
      0x7C1C0400,  //  001F  CALL	R7	2
      0x5820000A,  //  0020  LDCONST	R8	K10
      0x7C140600,  //  0021  CALL	R5	3
      0x8C14030B,  //  0022  GETMET	R5	R1	K11
      0x581C000A,  //  0023  LDCONST	R7	K10
      0x5820000A,  //  0024  LDCONST	R8	K10
      0x7C140600,  //  0025  CALL	R5	3
      0x8C14030C,  //  0026  GETMET	R5	R1	K12
      0x581C000A,  //  0027  LDCONST	R7	K10
      0x5820000A,  //  0028  LDCONST	R8	K10
      0x7C140600,  //  0029  CALL	R5	3
      0x8C14030D,  //  002A  GETMET	R5	R1	K13
      0x581C000A,  //  002B  LDCONST	R7	K10
      0x5820000A,  //  002C  LDCONST	R8	K10
      0x7C140600,  //  002D  CALL	R5	3
      0x8C14030E,  //  002E  GETMET	R5	R1	K14
      0xB81E0400,  //  002F  GETNGBL	R7	K2
      0x8C1C0F0F,  //  0030  GETMET	R7	R7	K15
      0x54260063,  //  0031  LDINT	R9	100
      0x7C1C0400,  //  0032  CALL	R7	2
      0xB8220400,  //  0033  GETNGBL	R8	K2
      0x8C20110F,  //  0034  GETMET	R8	R8	K15
      0x542A0063,  //  0035  LDINT	R10	100
      0x7C200400,  //  0036  CALL	R8	2
      0x7C140600,  //  0037  CALL	R5	3
      0x8C140310,  //  0038  GETMET	R5	R1	K16
      0x7C140200,  //  0039  CALL	R5	1
      0x8C140311,  //  003A  GETMET	R5	R1	K17
      0x7C140200,  //  003B  CALL	R5	1
      0x8C140312,  //  003C  GETMET	R5	R1	K18
      0x581C0013,  //  003D  LDCONST	R7	K19
      0x7C140400,  //  003E  CALL	R5	2
      0xB8160400,  //  003F  GETNGBL	R5	K2
      0x8C140B14,  //  0040  GETMET	R5	R5	K20
      0x5C1C0200,  //  0041  MOVE	R7	R1
      0x7C140400,  //  0042  CALL	R5	2
      0x8C180B15,  //  0043  GETMET	R6	R5	K21
      0x7C180200,  //  0044  CALL	R6	1
      0x8C180B16,  //  0045  GETMET	R6	R5	K22
      0x54220095,  //  0046  LDINT	R8	150
      0x7C180400,  //  0047  CALL	R6	2
      0x8C180B17,  //  0048  GETMET	R6	R5	K23
      0x542200FE,  //  0049  LDINT	R8	255
      0x5824000A,  //  004A  LDCONST	R9	K10
      0x7C180600,  //  004B  CALL	R6	3
      0x8C180B18,  //  004C  GETMET	R6	R5	K24
      0x5C200800,  //  004D  MOVE	R8	R4
      0x5824000A,  //  004E  LDCONST	R9	K10
      0x7C180600,  //  004F  CALL	R6	3
      0x8C180B19,  //  0050  GETMET	R6	R5	K25
      0xB8220400,  //  0051  GETNGBL	R8	K2
      0x8820111A,  //  0052  GETMBR	R8	R8	K26
      0x5425FFF3,  //  0053  LDINT	R9	-12
      0x5828000A,  //  0054  LDCONST	R10	K10
      0x7C180800,  //  0055  CALL	R6	4
      0xB81A0400,  //  0056  GETNGBL	R6	K2
      0x8C180D1B,  //  0057  GETMET	R6	R6	K27
      0x5C200200,  //  0058  MOVE	R8	R1
      0x7C180400,  //  0059  CALL	R6	2
      0x8C1C0D1C,  //  005A  GETMET	R7	R6	K28
      0x5C240800,  //  005B  MOVE	R9	R4
      0x5828000A,  //  005C  LDCONST	R10	K10
      0x7C1C0600,  //  005D  CALL	R7	3
      0x8C1C0D1D,  //  005E  GETMET	R7	R6	K29
      0x5824001E,  //  005F  LDCONST	R9	K30
      0x7C1C0400,  //  0060  CALL	R7	2
      0xB81E0400,  //  0061  GETNGBL	R7	K2
      0x8C1C0F1F,  //  0062  GETMET	R7	R7	K31
      0x7C1C0200,  //  0063  CALL	R7	1
      0x542200C7,  //  0064  LDINT	R8	200
      0x281C0E08,  //  0065  GE	R7	R7	R8
      0x781E0007,  //  0066  JMPF	R7	#006F
      0x4C1C0000,  //  0067  LDNIL	R7
      0x201C0407,  //  0068  NE	R7	R2	R7
      0x781E0003,  //  0069  JMPF	R7	#006E
      0x8C1C0D20,  //  006A  GETMET	R7	R6	K32
      0x5C240400,  //  006B  MOVE	R9	R2
      0x5828000A,  //  006C  LDCONST	R10	K10
      0x7C1C0600,  //  006D  CALL	R7	3
      0x70020006,  //  006E  JMP		#0076
      0x4C1C0000,  //  006F  LDNIL	R7
      0x201C0607,  //  0070  NE	R7	R3	R7
      0x781E0003,  //  0071  JMPF	R7	#0076
      0x8C1C0D20,  //  0072  GETMET	R7	R6	K32
      0x5C240600,  //  0073  MOVE	R9	R3
      0x5828000A,  //  0074  LDCONST	R10	K10
      0x7C1C0600,  //  0075  CALL	R7	3
      0x8C1C0D21,  //  0076  GETMET	R7	R6	K33
      0xB8260400,  //  0077  GETNGBL	R9	K2
      0x8824131A,  //  0078  GETMBR	R9	R9	K26
      0x7C1C0400,  //  0079  CALL	R7	2
      0x8C1C0D22,  //  007A  GETMET	R7	R6	K34
      0x54260029,  //  007B  LDINT	R9	42
      0x7C1C0400,  //  007C  CALL	R7	2
      0x8C1C0123,  //  007D  GETMET	R7	R0	K35
      0x7C1C0200,  //  007E  CALL	R7	1
      0x4C200000,  //  007F  LDNIL	R8
      0x6024000C,  //  0080  GETGBL	R9	G12
      0x5C280E00,  //  0081  MOVE	R10	R7
      0x7C240200,  //  0082  CALL	R9	1
      0x2424130A,  //  0083  GT	R9	R9	K10
      0x78260012,  //  0084  JMPF	R9	#0098
      0xB8260400,  //  0085  GETNGBL	R9	K2
      0x8C24131B,  //  0086  GETMET	R9	R9	K27
      0x5C2C0200,  //  0087  MOVE	R11	R1
      0x7C240400,  //  0088  CALL	R9	2
      0x5C201200,  //  0089  MOVE	R8	R9
      0x8C241121,  //  008A  GETMET	R9	R8	K33
      0xB82E0400,  //  008B  GETNGBL	R11	K2
      0x882C1724,  //  008C  GETMBR	R11	R11	K36
      0x7C240400,  //  008D  CALL	R9	2
      0x8C24111C,  //  008E  GETMET	R9	R8	K28
      0xB82E0400,  //  008F  GETNGBL	R11	K2
      0x8C2C1707,  //  0090  GETMET	R11	R11	K7
      0x58340025,  //  0091  LDCONST	R13	K37
      0x7C2C0400,  //  0092  CALL	R11	2
      0x5830000A,  //  0093  LDCONST	R12	K10
      0x7C240600,  //  0094  CALL	R9	3
      0x8C24111D,  //  0095  GETMET	R9	R8	K29
      0x5C2C0E00,  //  0096  MOVE	R11	R7
      0x7C240400,  //  0097  CALL	R9	2
      0xB8260400,  //  0098  GETNGBL	R9	K2
      0x8C241326,  //  0099  GETMET	R9	R9	K38
      0x582C000A,  //  009A  LDCONST	R11	K10
      0x7C240400,  //  009B  CALL	R9	2
      0xB8260400,  //  009C  GETNGBL	R9	K2
      0x90264E01,  //  009D  SETMBR	R9	K39	R1
      0xB8265000,  //  009E  GETNGBL	R9	K40
      0x8C241329,  //  009F  GETMET	R9	R9	K41
      0x542E1387,  //  00A0  LDINT	R11	5000
      0xB8320400,  //  00A1  GETNGBL	R12	K2
      0x8830192A,  //  00A2  GETMBR	R12	R12	K42
      0x7C240600,  //  00A3  CALL	R9	3
      0x80000000,  //  00A4  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
