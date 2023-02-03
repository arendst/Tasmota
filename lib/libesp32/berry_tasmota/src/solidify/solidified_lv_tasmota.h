/* Solidification of lv_tasmota.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_tasmota_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[33]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(start),
    /* K2   */  be_nested_str_weak(splash_init),
    /* K3   */  be_nested_str_weak(splash_remove),
    /* K4   */  be_nested_str_weak(splash),
    /* K5   */  be_nested_str_weak(_splash),
    /* K6   */  be_nested_str_weak(font_montserrat),
    /* K7   */  be_nested_str_weak(montserrat_font),
    /* K8   */  be_nested_str_weak(font_seg7),
    /* K9   */  be_nested_str_weak(seg7_font),
    /* K10  */  be_nested_str_weak(font_embedded),
    /* K11  */  be_nested_str_weak(load_freetype_font),
    /* K12  */  be_nested_str_weak(register_button_encoder),
    /* K13  */  be_nested_str_weak(screenshot),
    /* K14  */  be_nested_str_weak(clock),
    /* K15  */  be_nested_str_weak(lv_clock),
    /* K16  */  be_nested_str_weak(clock_icon),
    /* K17  */  be_nested_str_weak(lv_clock_icon),
    /* K18  */  be_nested_str_weak(signal_arcs),
    /* K19  */  be_nested_str_weak(lv_signal_arcs),
    /* K20  */  be_nested_str_weak(signal_bars),
    /* K21  */  be_nested_str_weak(lv_signal_bars),
    /* K22  */  be_nested_str_weak(wifi_arcs_icon),
    /* K23  */  be_nested_str_weak(lv_wifi_arcs_icon),
    /* K24  */  be_nested_str_weak(wifi_arcs),
    /* K25  */  be_nested_str_weak(lv_wifi_arcs),
    /* K26  */  be_nested_str_weak(wifi_bars_icon),
    /* K27  */  be_nested_str_weak(lv_wifi_bars_icon),
    /* K28  */  be_nested_str_weak(wifi_bars),
    /* K29  */  be_nested_str_weak(lv_wifi_bars),
    /* K30  */  be_nested_str_weak(tasmota),
    /* K31  */  be_nested_str_weak(get_option),
    /* K32  */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x90060202,  //  0002  SETMBR	R1	K1	R2
      0x88080102,  //  0003  GETMBR	R2	R0	K2
      0x90060402,  //  0004  SETMBR	R1	K2	R2
      0x88080103,  //  0005  GETMBR	R2	R0	K3
      0x90060602,  //  0006  SETMBR	R1	K3	R2
      0x88080104,  //  0007  GETMBR	R2	R0	K4
      0x90060802,  //  0008  SETMBR	R1	K4	R2
      0x4C080000,  //  0009  LDNIL	R2
      0x90060A02,  //  000A  SETMBR	R1	K5	R2
      0x88080106,  //  000B  GETMBR	R2	R0	K6
      0x90060C02,  //  000C  SETMBR	R1	K6	R2
      0x88080106,  //  000D  GETMBR	R2	R0	K6
      0x90060E02,  //  000E  SETMBR	R1	K7	R2
      0x88080108,  //  000F  GETMBR	R2	R0	K8
      0x90061002,  //  0010  SETMBR	R1	K8	R2
      0x88080108,  //  0011  GETMBR	R2	R0	K8
      0x90061202,  //  0012  SETMBR	R1	K9	R2
      0x8808010A,  //  0013  GETMBR	R2	R0	K10
      0x90061402,  //  0014  SETMBR	R1	K10	R2
      0x8808010B,  //  0015  GETMBR	R2	R0	K11
      0x90061602,  //  0016  SETMBR	R1	K11	R2
      0x8808010C,  //  0017  GETMBR	R2	R0	K12
      0x90061802,  //  0018  SETMBR	R1	K12	R2
      0x8808010D,  //  0019  GETMBR	R2	R0	K13
      0x90061A02,  //  001A  SETMBR	R1	K13	R2
      0xB80A1E00,  //  001B  GETNGBL	R2	K15
      0x90061C02,  //  001C  SETMBR	R1	K14	R2
      0xB80A2200,  //  001D  GETNGBL	R2	K17
      0x90062002,  //  001E  SETMBR	R1	K16	R2
      0xB80A2600,  //  001F  GETNGBL	R2	K19
      0x90062402,  //  0020  SETMBR	R1	K18	R2
      0xB80A2A00,  //  0021  GETNGBL	R2	K21
      0x90062802,  //  0022  SETMBR	R1	K20	R2
      0xB80A2E00,  //  0023  GETNGBL	R2	K23
      0x90062C02,  //  0024  SETMBR	R1	K22	R2
      0xB80A3200,  //  0025  GETNGBL	R2	K25
      0x90063002,  //  0026  SETMBR	R1	K24	R2
      0xB80A3600,  //  0027  GETNGBL	R2	K27
      0x90063402,  //  0028  SETMBR	R1	K26	R2
      0xB80A3A00,  //  0029  GETNGBL	R2	K29
      0x90063802,  //  002A  SETMBR	R1	K28	R2
      0xB80A3C00,  //  002B  GETNGBL	R2	K30
      0x8C08051F,  //  002C  GETMET	R2	R2	K31
      0x54120086,  //  002D  LDINT	R4	135
      0x7C080400,  //  002E  CALL	R2	2
      0x1C080520,  //  002F  EQ	R2	R2	K32
      0x780A0001,  //  0030  JMPF	R2	#0033
      0x8C080302,  //  0031  GETMET	R2	R1	K2
      0x7C080200,  //  0032  CALL	R2	1
      0x4C080000,  //  0033  LDNIL	R2
      0x80040400,  //  0034  RET	1	R2
    })
  )
);
/*******************************************************************/


extern const bclass be_class_splash_runner;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(splash_runner_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(add_driver),
    }),
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
be_local_closure(splash_runner_display,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(display),
    /* K1   */  be_nested_str_weak(dim),
    /* K2   */  be_nested_str_weak(started),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(remove_driver),
    /* K5   */  be_nested_str_weak(lv),
    /* K6   */  be_nested_str_weak(splash),
    }),
    be_str_weak(display),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0x1C180301,  //  0001  EQ	R6	R1	K1
      0x781A0009,  //  0002  JMPF	R6	#000D
      0x8C180B02,  //  0003  GETMET	R6	R5	K2
      0x7C180200,  //  0004  CALL	R6	1
      0x781A0006,  //  0005  JMPF	R6	#000D
      0xB81A0600,  //  0006  GETNGBL	R6	K3
      0x8C180D04,  //  0007  GETMET	R6	R6	K4
      0x5C200000,  //  0008  MOVE	R8	R0
      0x7C180400,  //  0009  CALL	R6	2
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x8C180D06,  //  000B  GETMET	R6	R6	K6
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
        { be_const_key_weak(display, -1), be_const_closure(splash_runner_display_closure) },
        { be_const_key_weak(init, 0), be_const_closure(splash_runner_init_closure) },
    })),
    be_str_weak(splash_runner)
);

/********************************************************************
** Solidified function: splash_init
********************************************************************/
be_local_closure(lv_tasmota_splash_init,   /* name */
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
be_local_closure(lv_tasmota_splash_remove,   /* name */
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
be_local_closure(lv_tasmota_splash,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[42]) {     /* constants */
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
    /* K25  */  be_nested_str_weak(set_align),
    /* K26  */  be_nested_str_weak(ALIGN_LEFT_MID),
    /* K27  */  be_nested_str_weak(set_x),
    /* K28  */  be_nested_str_weak(label),
    /* K29  */  be_nested_str_weak(set_style_text_color),
    /* K30  */  be_nested_str_weak(set_text),
    /* K31  */  be_nested_str_weak(TASMOTA),
    /* K32  */  be_nested_str_weak(get_hor_res),
    /* K33  */  be_nested_str_weak(set_style_text_font),
    /* K34  */  be_nested_str_weak(driver_name),
    /* K35  */  be_nested_str_weak(ALIGN_BOTTOM_MID),
    /* K36  */  be_const_int(16777215),
    /* K37  */  be_nested_str_weak(refr_now),
    /* K38  */  be_nested_str_weak(_splash),
    /* K39  */  be_nested_str_weak(tasmota),
    /* K40  */  be_nested_str_weak(set_timer),
    /* K41  */  be_nested_str_weak(splash_remove),
    }),
    be_str_weak(splash),
    &be_const_str_solidified,
    ( &(const binstruction[166]) {  /* code */
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
      0x7C180400,  //  0053  CALL	R6	2
      0x8C180B1B,  //  0054  GETMET	R6	R5	K27
      0x5421FFF3,  //  0055  LDINT	R8	-12
      0x7C180400,  //  0056  CALL	R6	2
      0xB81A0400,  //  0057  GETNGBL	R6	K2
      0x8C180D1C,  //  0058  GETMET	R6	R6	K28
      0x5C200200,  //  0059  MOVE	R8	R1
      0x7C180400,  //  005A  CALL	R6	2
      0x8C1C0D1D,  //  005B  GETMET	R7	R6	K29
      0x5C240800,  //  005C  MOVE	R9	R4
      0x5828000A,  //  005D  LDCONST	R10	K10
      0x7C1C0600,  //  005E  CALL	R7	3
      0x8C1C0D1E,  //  005F  GETMET	R7	R6	K30
      0x5824001F,  //  0060  LDCONST	R9	K31
      0x7C1C0400,  //  0061  CALL	R7	2
      0xB81E0400,  //  0062  GETNGBL	R7	K2
      0x8C1C0F20,  //  0063  GETMET	R7	R7	K32
      0x7C1C0200,  //  0064  CALL	R7	1
      0x542200C7,  //  0065  LDINT	R8	200
      0x281C0E08,  //  0066  GE	R7	R7	R8
      0x781E0007,  //  0067  JMPF	R7	#0070
      0x4C1C0000,  //  0068  LDNIL	R7
      0x201C0407,  //  0069  NE	R7	R2	R7
      0x781E0003,  //  006A  JMPF	R7	#006F
      0x8C1C0D21,  //  006B  GETMET	R7	R6	K33
      0x5C240400,  //  006C  MOVE	R9	R2
      0x5828000A,  //  006D  LDCONST	R10	K10
      0x7C1C0600,  //  006E  CALL	R7	3
      0x70020006,  //  006F  JMP		#0077
      0x4C1C0000,  //  0070  LDNIL	R7
      0x201C0607,  //  0071  NE	R7	R3	R7
      0x781E0003,  //  0072  JMPF	R7	#0077
      0x8C1C0D21,  //  0073  GETMET	R7	R6	K33
      0x5C240600,  //  0074  MOVE	R9	R3
      0x5828000A,  //  0075  LDCONST	R10	K10
      0x7C1C0600,  //  0076  CALL	R7	3
      0x8C1C0D19,  //  0077  GETMET	R7	R6	K25
      0xB8260400,  //  0078  GETNGBL	R9	K2
      0x8824131A,  //  0079  GETMBR	R9	R9	K26
      0x7C1C0400,  //  007A  CALL	R7	2
      0x8C1C0D1B,  //  007B  GETMET	R7	R6	K27
      0x54260029,  //  007C  LDINT	R9	42
      0x7C1C0400,  //  007D  CALL	R7	2
      0x8C1C0122,  //  007E  GETMET	R7	R0	K34
      0x7C1C0200,  //  007F  CALL	R7	1
      0x4C200000,  //  0080  LDNIL	R8
      0x6024000C,  //  0081  GETGBL	R9	G12
      0x5C280E00,  //  0082  MOVE	R10	R7
      0x7C240200,  //  0083  CALL	R9	1
      0x2424130A,  //  0084  GT	R9	R9	K10
      0x78260012,  //  0085  JMPF	R9	#0099
      0xB8260400,  //  0086  GETNGBL	R9	K2
      0x8C24131C,  //  0087  GETMET	R9	R9	K28
      0x5C2C0200,  //  0088  MOVE	R11	R1
      0x7C240400,  //  0089  CALL	R9	2
      0x5C201200,  //  008A  MOVE	R8	R9
      0x8C241119,  //  008B  GETMET	R9	R8	K25
      0xB82E0400,  //  008C  GETNGBL	R11	K2
      0x882C1723,  //  008D  GETMBR	R11	R11	K35
      0x7C240400,  //  008E  CALL	R9	2
      0x8C24111D,  //  008F  GETMET	R9	R8	K29
      0xB82E0400,  //  0090  GETNGBL	R11	K2
      0x8C2C1707,  //  0091  GETMET	R11	R11	K7
      0x58340024,  //  0092  LDCONST	R13	K36
      0x7C2C0400,  //  0093  CALL	R11	2
      0x5830000A,  //  0094  LDCONST	R12	K10
      0x7C240600,  //  0095  CALL	R9	3
      0x8C24111E,  //  0096  GETMET	R9	R8	K30
      0x5C2C0E00,  //  0097  MOVE	R11	R7
      0x7C240400,  //  0098  CALL	R9	2
      0xB8260400,  //  0099  GETNGBL	R9	K2
      0x8C241325,  //  009A  GETMET	R9	R9	K37
      0x582C000A,  //  009B  LDCONST	R11	K10
      0x7C240400,  //  009C  CALL	R9	2
      0xB8260400,  //  009D  GETNGBL	R9	K2
      0x90264C01,  //  009E  SETMBR	R9	K38	R1
      0xB8264E00,  //  009F  GETNGBL	R9	K39
      0x8C241328,  //  00A0  GETMET	R9	R9	K40
      0x542E1387,  //  00A1  LDINT	R11	5000
      0xB8320400,  //  00A2  GETNGBL	R12	K2
      0x88301929,  //  00A3  GETMBR	R12	R12	K41
      0x7C240600,  //  00A4  CALL	R9	3
      0x80000000,  //  00A5  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
