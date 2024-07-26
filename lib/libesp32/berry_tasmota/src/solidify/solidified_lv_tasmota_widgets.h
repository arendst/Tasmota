/* Solidification of lv_tasmota_widgets.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'lv_clock' ktab size: 27, total: 32 (saved 40 bytes)
static const bvalue be_ktab_class_lv_clock[27] = {
  /* K0   */  be_nested_str_weak(hour),
  /* K1   */  be_nested_str_weak(minute),
  /* K2   */  be_nested_str_weak(sec),
  /* K3   */  be_nested_str_weak(_X2502d_X25s_X2502d),
  /* K4   */  be_const_int(2),
  /* K5   */  be_nested_str_weak(_X3A),
  /* K6   */  be_nested_str_weak(_X20),
  /* K7   */  be_nested_str_weak(set_text),
  /* K8   */  be_nested_str_weak(tasmota),
  /* K9   */  be_nested_str_weak(time_dump),
  /* K10  */  be_nested_str_weak(rtc),
  /* K11  */  be_nested_str_weak(local),
  /* K12  */  be_nested_str_weak(year),
  /* K13  */  be_nested_str_weak(set_time),
  /* K14  */  be_nested_str_weak(min),
  /* K15  */  be_nested_str_weak(init),
  /* K16  */  be_nested_str_weak(lv),
  /* K17  */  be_nested_str_weak(seg7_font),
  /* K18  */  be_nested_str_weak(set_style_text_font),
  /* K19  */  be_nested_str_weak(PART_MAIN),
  /* K20  */  be_nested_str_weak(STATE_DEFAULT),
  /* K21  */  be_nested_str_weak(_X2D_X2D_X3A_X2D_X2D),
  /* K22  */  be_nested_str_weak(add_event_cb),
  /* K23  */  be_nested_str_weak(EVENT_DELETE),
  /* K24  */  be_const_int(0),
  /* K25  */  be_nested_str_weak(add_driver),
  /* K26  */  be_nested_str_weak(remove_driver),
};


extern const bclass be_class_lv_clock;

/********************************************************************
** Solidified function: set_time
********************************************************************/
be_local_closure(class_lv_clock_set_time,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_clock,     /* shared constants */
    be_str_weak(set_time),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x20100204,  //  0001  NE	R4	R1	R4
      0x74120005,  //  0002  JMPT	R4	#0009
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x20100404,  //  0004  NE	R4	R2	R4
      0x74120002,  //  0005  JMPT	R4	#0009
      0x88100102,  //  0006  GETMBR	R4	R0	K2
      0x20100604,  //  0007  NE	R4	R3	R4
      0x7812000F,  //  0008  JMPF	R4	#0019
      0x60100018,  //  0009  GETGBL	R4	G24
      0x58140003,  //  000A  LDCONST	R5	K3
      0x5C180200,  //  000B  MOVE	R6	R1
      0x101C0704,  //  000C  MOD	R7	R3	K4
      0x781E0001,  //  000D  JMPF	R7	#0010
      0x581C0005,  //  000E  LDCONST	R7	K5
      0x70020000,  //  000F  JMP		#0011
      0x581C0006,  //  0010  LDCONST	R7	K6
      0x5C200400,  //  0011  MOVE	R8	R2
      0x7C100800,  //  0012  CALL	R4	4
      0x90020001,  //  0013  SETMBR	R0	K0	R1
      0x90020202,  //  0014  SETMBR	R0	K1	R2
      0x90020403,  //  0015  SETMBR	R0	K2	R3
      0x8C140107,  //  0016  GETMET	R5	R0	K7
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x7C140400,  //  0018  CALL	R5	2
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_lv_clock_every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_clock,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8061000,  //  0000  GETNGBL	R1	K8
      0x8C040309,  //  0001  GETMET	R1	R1	K9
      0xB80E1000,  //  0002  GETNGBL	R3	K8
      0x8C0C070A,  //  0003  GETMET	R3	R3	K10
      0x7C0C0200,  //  0004  CALL	R3	1
      0x940C070B,  //  0005  GETIDX	R3	R3	K11
      0x7C040400,  //  0006  CALL	R1	2
      0x9408030C,  //  0007  GETIDX	R2	R1	K12
      0x540E07B1,  //  0008  LDINT	R3	1970
      0x20080403,  //  0009  NE	R2	R2	R3
      0x780A0004,  //  000A  JMPF	R2	#0010
      0x8C08010D,  //  000B  GETMET	R2	R0	K13
      0x94100300,  //  000C  GETIDX	R4	R1	K0
      0x9414030E,  //  000D  GETIDX	R5	R1	K14
      0x94180302,  //  000E  GETIDX	R6	R1	K2
      0x7C080800,  //  000F  CALL	R2	4
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_clock_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
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
        /* K0   */  be_nested_str_weak(before_del),
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
    &be_ktab_class_lv_clock,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08050F,  //  0003  GETMET	R2	R2	K15
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A2000,  //  0006  GETNGBL	R2	K16
      0x8C080511,  //  0007  GETMET	R2	R2	K17
      0x5412000F,  //  0008  LDINT	R4	16
      0x7C080400,  //  0009  CALL	R2	2
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0007,  //  000C  JMPF	R3	#0015
      0x8C0C0112,  //  000D  GETMET	R3	R0	K18
      0x5C140400,  //  000E  MOVE	R5	R2
      0xB81A2000,  //  000F  GETNGBL	R6	K16
      0x88180D13,  //  0010  GETMBR	R6	R6	K19
      0xB81E2000,  //  0011  GETNGBL	R7	K16
      0x881C0F14,  //  0012  GETMBR	R7	R7	K20
      0x30180C07,  //  0013  OR	R6	R6	R7
      0x7C0C0600,  //  0014  CALL	R3	3
      0x8C0C0107,  //  0015  GETMET	R3	R0	K7
      0x58140015,  //  0016  LDCONST	R5	K21
      0x7C0C0400,  //  0017  CALL	R3	2
      0x8C0C0116,  //  0018  GETMET	R3	R0	K22
      0x84140000,  //  0019  CLOSURE	R5	P0
      0xB81A2000,  //  001A  GETNGBL	R6	K16
      0x88180D17,  //  001B  GETMBR	R6	R6	K23
      0x581C0018,  //  001C  LDCONST	R7	K24
      0x7C0C0800,  //  001D  CALL	R3	4
      0xB80E1000,  //  001E  GETNGBL	R3	K8
      0x8C0C0719,  //  001F  GETMET	R3	R3	K25
      0x5C140000,  //  0020  MOVE	R5	R0
      0x7C0C0400,  //  0021  CALL	R3	2
      0xA0000000,  //  0022  CLOSE	R0
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: before_del
********************************************************************/
be_local_closure(class_lv_clock_before_del,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_clock,     /* shared constants */
    be_str_weak(before_del),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8061000,  //  0000  GETNGBL	R1	K8
      0x8C04031A,  //  0001  GETMET	R1	R1	K26
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_clock
********************************************************************/
extern const bclass be_class_lv_label;
be_local_class(lv_clock,
    3,
    &be_class_lv_label,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(sec, -1), be_const_var(2) },
        { be_const_key_weak(hour, -1), be_const_var(0) },
        { be_const_key_weak(before_del, 6), be_const_closure(class_lv_clock_before_del_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_lv_clock_every_second_closure) },
        { be_const_key_weak(minute, -1), be_const_var(1) },
        { be_const_key_weak(set_time, 2), be_const_closure(class_lv_clock_set_time_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_lv_clock_init_closure) },
    })),
    be_str_weak(lv_clock)
);

extern const bclass be_class_lv_clock_icon;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_clock_icon_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(get_height),
    /* K2   */  be_nested_str_weak(refr_size),
    /* K3   */  be_nested_str_weak(get_width),
    /* K4   */  be_nested_str_weak(set_y),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(get_style_pad_right),
    /* K7   */  be_nested_str_weak(lv),
    /* K8   */  be_nested_str_weak(PART_MAIN),
    /* K9   */  be_nested_str_weak(STATE_DEFAULT),
    /* K10  */  be_nested_str_weak(set_x),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(set_style_pad_right),
    /* K13  */  be_nested_str_weak(set_style_bg_color),
    /* K14  */  be_nested_str_weak(color),
    /* K15  */  be_nested_str_weak(COLOR_BLACK),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[60]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x20080202,  //  0007  NE	R2	R1	R2
      0x780A0031,  //  0008  JMPF	R2	#003B
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x7C080200,  //  000A  CALL	R2	1
      0x8C0C0102,  //  000B  GETMET	R3	R0	K2
      0x7C0C0200,  //  000C  CALL	R3	1
      0x8C0C0103,  //  000D  GETMET	R3	R0	K3
      0x7C0C0200,  //  000E  CALL	R3	1
      0x8C100104,  //  000F  GETMET	R4	R0	K4
      0x8C180301,  //  0010  GETMET	R6	R1	K1
      0x7C180200,  //  0011  CALL	R6	1
      0x8C1C0101,  //  0012  GETMET	R7	R0	K1
      0x7C1C0200,  //  0013  CALL	R7	1
      0x04180C07,  //  0014  SUB	R6	R6	R7
      0x0C180D05,  //  0015  DIV	R6	R6	K5
      0x7C100400,  //  0016  CALL	R4	2
      0x8C100306,  //  0017  GETMET	R4	R1	K6
      0xB81A0E00,  //  0018  GETNGBL	R6	K7
      0x88180D08,  //  0019  GETMBR	R6	R6	K8
      0xB81E0E00,  //  001A  GETNGBL	R7	K7
      0x881C0F09,  //  001B  GETMBR	R7	R7	K9
      0x30180C07,  //  001C  OR	R6	R6	R7
      0x7C100400,  //  001D  CALL	R4	2
      0x8C14010A,  //  001E  GETMET	R5	R0	K10
      0x8C1C0303,  //  001F  GETMET	R7	R1	K3
      0x7C1C0200,  //  0020  CALL	R7	1
      0x041C0E03,  //  0021  SUB	R7	R7	R3
      0x041C0E04,  //  0022  SUB	R7	R7	R4
      0x041C0F0B,  //  0023  SUB	R7	R7	K11
      0x7C140400,  //  0024  CALL	R5	2
      0x8C14030C,  //  0025  GETMET	R5	R1	K12
      0x001C0803,  //  0026  ADD	R7	R4	R3
      0x54220005,  //  0027  LDINT	R8	6
      0x001C0E08,  //  0028  ADD	R7	R7	R8
      0xB8220E00,  //  0029  GETNGBL	R8	K7
      0x88201108,  //  002A  GETMBR	R8	R8	K8
      0xB8260E00,  //  002B  GETNGBL	R9	K7
      0x88241309,  //  002C  GETMBR	R9	R9	K9
      0x30201009,  //  002D  OR	R8	R8	R9
      0x7C140600,  //  002E  CALL	R5	3
      0x8C14010D,  //  002F  GETMET	R5	R0	K13
      0xB81E0E00,  //  0030  GETNGBL	R7	K7
      0x8C1C0F0E,  //  0031  GETMET	R7	R7	K14
      0xB8260E00,  //  0032  GETNGBL	R9	K7
      0x8824130F,  //  0033  GETMBR	R9	R9	K15
      0x7C1C0400,  //  0034  CALL	R7	2
      0xB8220E00,  //  0035  GETNGBL	R8	K7
      0x88201108,  //  0036  GETMBR	R8	R8	K8
      0xB8260E00,  //  0037  GETNGBL	R9	K7
      0x88241309,  //  0038  GETMBR	R9	R9	K9
      0x30201009,  //  0039  OR	R8	R8	R9
      0x7C140600,  //  003A  CALL	R5	3
      0x80000000,  //  003B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_clock_icon
********************************************************************/
extern const bclass be_class_lv_clock;
be_local_class(lv_clock_icon,
    0,
    &be_class_lv_clock,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_lv_clock_icon_init_closure) },
    })),
    be_str_weak(lv_clock_icon)
);
// compact class 'lv_signal_arcs' ktab size: 43, total: 52 (saved 72 bytes)
static const bvalue be_ktab_class_lv_signal_arcs[43] = {
  /* K0   */  be_nested_str_weak(get_code),
  /* K1   */  be_nested_str_weak(math),
  /* K2   */  be_nested_str_weak(get_height),
  /* K3   */  be_nested_str_weak(get_width),
  /* K4   */  be_const_int(2),
  /* K5   */  be_const_int(3),
  /* K6   */  be_nested_str_weak(lv),
  /* K7   */  be_nested_str_weak(EVENT_DRAW_MAIN),
  /* K8   */  be_nested_str_weak(arc_dsc),
  /* K9   */  be_nested_str_weak(get_param),
  /* K10  */  be_nested_str_weak(get_layer),
  /* K11  */  be_nested_str_weak(get_coords),
  /* K12  */  be_nested_str_weak(area),
  /* K13  */  be_nested_str_weak(x1),
  /* K14  */  be_nested_str_weak(y1),
  /* K15  */  be_nested_str_weak(draw_arc_dsc_init),
  /* K16  */  be_nested_str_weak(init_draw_arc_dsc),
  /* K17  */  be_nested_str_weak(PART_MAIN),
  /* K18  */  be_nested_str_weak(rounded),
  /* K19  */  be_const_int(1),
  /* K20  */  be_nested_str_weak(width),
  /* K21  */  be_nested_str_weak(get_style_line_color),
  /* K22  */  be_nested_str_weak(STATE_DEFAULT),
  /* K23  */  be_nested_str_weak(get_style_bg_color),
  /* K24  */  be_nested_str_weak(deg),
  /* K25  */  be_nested_str_weak(acos),
  /* K26  */  be_nested_str_weak(center_x),
  /* K27  */  be_nested_str_weak(center_y),
  /* K28  */  be_nested_str_weak(color),
  /* K29  */  be_nested_str_weak(percentage),
  /* K30  */  be_nested_str_weak(radius),
  /* K31  */  be_const_int(0),
  /* K32  */  be_nested_str_weak(start_angle),
  /* K33  */  be_nested_str_weak(end_angle),
  /* K34  */  be_nested_str_weak(draw_arc),
  /* K35  */  be_nested_str_weak(init),
  /* K36  */  be_nested_str_weak(set_style_bg_opa),
  /* K37  */  be_nested_str_weak(set_style_border_width),
  /* K38  */  be_nested_str_weak(set_style_pad_all),
  /* K39  */  be_nested_str_weak(draw_arc_dsc),
  /* K40  */  be_nested_str_weak(add_event_cb),
  /* K41  */  be_nested_str_weak(widget_event),
  /* K42  */  be_nested_str_weak(invalidate),
};


extern const bclass be_class_lv_signal_arcs;

/********************************************************************
** Solidified function: widget_event
********************************************************************/
be_local_closure(class_lv_signal_arcs_widget_event,   /* name */
  be_nested_proto(
    28,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_const_int(1),
        }),
        be_str_weak(atleast1),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x28040100,  //  0000  GE	R1	R0	K0
          0x78060001,  //  0001  JMPF	R1	#0004
          0x80040000,  //  0002  RET	1	R0
          0x70020000,  //  0003  JMP		#0005
          0x80060000,  //  0004  RET	1	K0
          0x80000000,  //  0005  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lv_signal_arcs,     /* shared constants */
    be_str_weak(widget_event),
    &be_const_str_solidified,
    ( &(const binstruction[161]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x7C080200,  //  0001  CALL	R2	1
      0xA40E0200,  //  0002  IMPORT	R3	K1
      0x84100000,  //  0003  CLOSURE	R4	P0
      0x8C140102,  //  0004  GETMET	R5	R0	K2
      0x7C140200,  //  0005  CALL	R5	1
      0x8C180103,  //  0006  GETMET	R6	R0	K3
      0x7C180200,  //  0007  CALL	R6	1
      0x5C1C0800,  //  0008  MOVE	R7	R4
      0x54220007,  //  0009  LDINT	R8	8
      0x0C200A08,  //  000A  DIV	R8	R5	R8
      0x7C1C0200,  //  000B  CALL	R7	1
      0x5C200800,  //  000C  MOVE	R8	R4
      0x08240F04,  //  000D  MUL	R9	R7	K4
      0x04240A09,  //  000E  SUB	R9	R5	R9
      0x0C241305,  //  000F  DIV	R9	R9	K5
      0x7C200200,  //  0010  CALL	R8	1
      0x0C241104,  //  0011  DIV	R9	R8	K4
      0xB82A0C00,  //  0012  GETNGBL	R10	K6
      0x88281507,  //  0013  GETMBR	R10	R10	K7
      0x1C28040A,  //  0014  EQ	R10	R2	R10
      0x782A0089,  //  0015  JMPF	R10	#00A0
      0x88280108,  //  0016  GETMBR	R10	R0	K8
      0x8C2C0309,  //  0017  GETMET	R11	R1	K9
      0x7C2C0200,  //  0018  CALL	R11	1
      0x8C30030A,  //  0019  GETMET	R12	R1	K10
      0x7C300200,  //  001A  CALL	R12	1
      0x8C34010B,  //  001B  GETMET	R13	R0	K11
      0x883C010C,  //  001C  GETMBR	R15	R0	K12
      0x7C340400,  //  001D  CALL	R13	2
      0x8834010C,  //  001E  GETMBR	R13	R0	K12
      0x88341B0D,  //  001F  GETMBR	R13	R13	K13
      0x8838010C,  //  0020  GETMBR	R14	R0	K12
      0x88381D0E,  //  0021  GETMBR	R14	R14	K14
      0xB83E0C00,  //  0022  GETNGBL	R15	K6
      0x8C3C1F0F,  //  0023  GETMET	R15	R15	K15
      0x5C441400,  //  0024  MOVE	R17	R10
      0x7C3C0400,  //  0025  CALL	R15	2
      0x8C3C0110,  //  0026  GETMET	R15	R0	K16
      0xB8460C00,  //  0027  GETNGBL	R17	K6
      0x88442311,  //  0028  GETMBR	R17	R17	K17
      0x5C481400,  //  0029  MOVE	R18	R10
      0x7C3C0600,  //  002A  CALL	R15	3
      0x902A2513,  //  002B  SETMBR	R10	K18	K19
      0x083C1105,  //  002C  MUL	R15	R8	K5
      0x003C1F13,  //  002D  ADD	R15	R15	K19
      0x54420003,  //  002E  LDINT	R16	4
      0x0C3C1E10,  //  002F  DIV	R15	R15	R16
      0x902A280F,  //  0030  SETMBR	R10	K20	R15
      0x8C3C0115,  //  0031  GETMET	R15	R0	K21
      0xB8460C00,  //  0032  GETNGBL	R17	K6
      0x88442311,  //  0033  GETMBR	R17	R17	K17
      0xB84A0C00,  //  0034  GETNGBL	R18	K6
      0x88482516,  //  0035  GETMBR	R18	R18	K22
      0x30442212,  //  0036  OR	R17	R17	R18
      0x7C3C0400,  //  0037  CALL	R15	2
      0x8C400117,  //  0038  GETMET	R16	R0	K23
      0xB84A0C00,  //  0039  GETNGBL	R18	K6
      0x88482511,  //  003A  GETMBR	R18	R18	K17
      0xB84E0C00,  //  003B  GETNGBL	R19	K6
      0x884C2716,  //  003C  GETMBR	R19	R19	K22
      0x30482413,  //  003D  OR	R18	R18	R19
      0x7C400400,  //  003E  CALL	R16	2
      0x04440A08,  //  003F  SUB	R17	R5	R8
      0x0C480D04,  //  0040  DIV	R18	R6	K4
      0x04482409,  //  0041  SUB	R18	R18	R9
      0x604C0009,  //  0042  GETGBL	R19	G9
      0x54520059,  //  0043  LDINT	R20	90
      0x8C540718,  //  0044  GETMET	R21	R3	K24
      0x8C5C0719,  //  0045  GETMET	R23	R3	K25
      0x6064000A,  //  0046  GETGBL	R25	G10
      0x5C682400,  //  0047  MOVE	R26	R18
      0x7C640200,  //  0048  CALL	R25	1
      0x6068000A,  //  0049  GETGBL	R26	G10
      0x5C6C2200,  //  004A  MOVE	R27	R17
      0x7C680200,  //  004B  CALL	R26	1
      0x0C64321A,  //  004C  DIV	R25	R25	R26
      0x7C5C0400,  //  004D  CALL	R23	2
      0x7C540400,  //  004E  CALL	R21	2
      0x04502815,  //  004F  SUB	R20	R20	R21
      0x7C4C0200,  //  0050  CALL	R19	1
      0x5452002C,  //  0051  LDINT	R20	45
      0x24502614,  //  0052  GT	R20	R19	R20
      0x78520000,  //  0053  JMPF	R20	#0055
      0x544E002C,  //  0054  LDINT	R19	45
      0x0C500D04,  //  0055  DIV	R20	R6	K4
      0x00501A14,  //  0056  ADD	R20	R13	R20
      0x902A3414,  //  0057  SETMBR	R10	K26	R20
      0x00501C05,  //  0058  ADD	R20	R14	R5
      0x04502913,  //  0059  SUB	R20	R20	K19
      0x04502809,  //  005A  SUB	R20	R20	R9
      0x902A3614,  //  005B  SETMBR	R10	K27	R20
      0x8850011D,  //  005C  GETMBR	R20	R0	K29
      0x54560018,  //  005D  LDINT	R21	25
      0x28502815,  //  005E  GE	R20	R20	R21
      0x78520001,  //  005F  JMPF	R20	#0062
      0x5C501E00,  //  0060  MOVE	R20	R15
      0x70020000,  //  0061  JMP		#0063
      0x5C502000,  //  0062  MOVE	R20	R16
      0x902A3814,  //  0063  SETMBR	R10	K28	R20
      0x00501007,  //  0064  ADD	R20	R8	R7
      0x08523E14,  //  0065  MUL	R20	K31	R20
      0x00502809,  //  0066  ADD	R20	R20	R9
      0x902A3C14,  //  0067  SETMBR	R10	K30	R20
      0x902A411F,  //  0068  SETMBR	R10	K32	K31
      0x54520131,  //  0069  LDINT	R20	306
      0x902A4214,  //  006A  SETMBR	R10	K33	R20
      0xB8520C00,  //  006B  GETNGBL	R20	K6
      0x8C502922,  //  006C  GETMET	R20	R20	K34
      0x5C581800,  //  006D  MOVE	R22	R12
      0x5C5C1400,  //  006E  MOVE	R23	R10
      0x7C500600,  //  006F  CALL	R20	3
      0x00501007,  //  0070  ADD	R20	R8	R7
      0x08522614,  //  0071  MUL	R20	K19	R20
      0x00502809,  //  0072  ADD	R20	R20	R9
      0x04502913,  //  0073  SUB	R20	R20	K19
      0x902A3C14,  //  0074  SETMBR	R10	K30	R20
      0x5452010D,  //  0075  LDINT	R20	270
      0x04502813,  //  0076  SUB	R20	R20	R19
      0x902A4014,  //  0077  SETMBR	R10	K32	R20
      0x5452010D,  //  0078  LDINT	R20	270
      0x00502813,  //  0079  ADD	R20	R20	R19
      0x902A4214,  //  007A  SETMBR	R10	K33	R20
      0x8850011D,  //  007B  GETMBR	R20	R0	K29
      0x54560031,  //  007C  LDINT	R21	50
      0x28502815,  //  007D  GE	R20	R20	R21
      0x78520001,  //  007E  JMPF	R20	#0081
      0x5C501E00,  //  007F  MOVE	R20	R15
      0x70020000,  //  0080  JMP		#0082
      0x5C502000,  //  0081  MOVE	R20	R16
      0x902A3814,  //  0082  SETMBR	R10	K28	R20
      0xB8520C00,  //  0083  GETNGBL	R20	K6
      0x8C502922,  //  0084  GETMET	R20	R20	K34
      0x5C581800,  //  0085  MOVE	R22	R12
      0x5C5C1400,  //  0086  MOVE	R23	R10
      0x7C500600,  //  0087  CALL	R20	3
      0x00501007,  //  0088  ADD	R20	R8	R7
      0x08520814,  //  0089  MUL	R20	K4	R20
      0x00502809,  //  008A  ADD	R20	R20	R9
      0x04502904,  //  008B  SUB	R20	R20	K4
      0x902A3C14,  //  008C  SETMBR	R10	K30	R20
      0x5452010D,  //  008D  LDINT	R20	270
      0x04502813,  //  008E  SUB	R20	R20	R19
      0x902A4014,  //  008F  SETMBR	R10	K32	R20
      0x5452010D,  //  0090  LDINT	R20	270
      0x00502813,  //  0091  ADD	R20	R20	R19
      0x902A4214,  //  0092  SETMBR	R10	K33	R20
      0x8850011D,  //  0093  GETMBR	R20	R0	K29
      0x5456004A,  //  0094  LDINT	R21	75
      0x28502815,  //  0095  GE	R20	R20	R21
      0x78520001,  //  0096  JMPF	R20	#0099
      0x5C501E00,  //  0097  MOVE	R20	R15
      0x70020000,  //  0098  JMP		#009A
      0x5C502000,  //  0099  MOVE	R20	R16
      0x902A3814,  //  009A  SETMBR	R10	K28	R20
      0xB8520C00,  //  009B  GETNGBL	R20	K6
      0x8C502922,  //  009C  GETMET	R20	R20	K34
      0x5C581800,  //  009D  MOVE	R22	R12
      0x5C5C1400,  //  009E  MOVE	R23	R10
      0x7C500600,  //  009F  CALL	R20	3
      0x80000000,  //  00A0  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_percentage
********************************************************************/
be_local_closure(class_lv_signal_arcs_get_percentage,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_signal_arcs,     /* shared constants */
    be_str_weak(get_percentage),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804011D,  //  0000  GETMBR	R1	R0	K29
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_signal_arcs_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_signal_arcs,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080523,  //  0003  GETMET	R2	R2	K35
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080124,  //  0006  GETMET	R2	R0	K36
      0x5810001F,  //  0007  LDCONST	R4	K31
      0x5814001F,  //  0008  LDCONST	R5	K31
      0x7C080600,  //  0009  CALL	R2	3
      0x8C080125,  //  000A  GETMET	R2	R0	K37
      0x5810001F,  //  000B  LDCONST	R4	K31
      0x5814001F,  //  000C  LDCONST	R5	K31
      0x7C080600,  //  000D  CALL	R2	3
      0x8C080126,  //  000E  GETMET	R2	R0	K38
      0x5810001F,  //  000F  LDCONST	R4	K31
      0x5814001F,  //  0010  LDCONST	R5	K31
      0x7C080600,  //  0011  CALL	R2	3
      0x540A0063,  //  0012  LDINT	R2	100
      0x90023A02,  //  0013  SETMBR	R0	K29	R2
      0xB80A0C00,  //  0014  GETNGBL	R2	K6
      0x8C08050C,  //  0015  GETMET	R2	R2	K12
      0x7C080200,  //  0016  CALL	R2	1
      0x90021802,  //  0017  SETMBR	R0	K12	R2
      0xB80A0C00,  //  0018  GETNGBL	R2	K6
      0x8C080527,  //  0019  GETMET	R2	R2	K39
      0x7C080200,  //  001A  CALL	R2	1
      0x90021002,  //  001B  SETMBR	R0	K8	R2
      0x8C080128,  //  001C  GETMET	R2	R0	K40
      0x88100129,  //  001D  GETMBR	R4	R0	K41
      0xB8160C00,  //  001E  GETNGBL	R5	K6
      0x88140B07,  //  001F  GETMBR	R5	R5	K7
      0x5818001F,  //  0020  LDCONST	R6	K31
      0x7C080800,  //  0021  CALL	R2	4
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_percentage
********************************************************************/
be_local_closure(class_lv_signal_arcs_set_percentage,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_signal_arcs,     /* shared constants */
    be_str_weak(set_percentage),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8808011D,  //  0000  GETMBR	R2	R0	K29
      0x540E0018,  //  0001  LDINT	R3	25
      0x0C080403,  //  0002  DIV	R2	R2	R3
      0x540E0063,  //  0003  LDINT	R3	100
      0x240C0203,  //  0004  GT	R3	R1	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x54060063,  //  0006  LDINT	R1	100
      0x140C031F,  //  0007  LT	R3	R1	K31
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x5804001F,  //  0009  LDCONST	R1	K31
      0x90023A01,  //  000A  SETMBR	R0	K29	R1
      0x540E0018,  //  000B  LDINT	R3	25
      0x0C0C0203,  //  000C  DIV	R3	R1	R3
      0x200C0403,  //  000D  NE	R3	R2	R3
      0x780E0001,  //  000E  JMPF	R3	#0011
      0x8C0C012A,  //  000F  GETMET	R3	R0	K42
      0x7C0C0200,  //  0010  CALL	R3	1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_signal_arcs
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_arcs,
    3,
    &be_class_lv_obj,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(widget_event, 2), be_const_closure(class_lv_signal_arcs_widget_event_closure) },
        { be_const_key_weak(get_percentage, 3), be_const_closure(class_lv_signal_arcs_get_percentage_closure) },
        { be_const_key_weak(set_percentage, 6), be_const_closure(class_lv_signal_arcs_set_percentage_closure) },
        { be_const_key_weak(arc_dsc, -1), be_const_var(2) },
        { be_const_key_weak(percentage, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(class_lv_signal_arcs_init_closure) },
        { be_const_key_weak(area, -1), be_const_var(1) },
    })),
    be_str_weak(lv_signal_arcs)
);
// compact class 'lv_wifi_arcs' ktab size: 10, total: 14 (saved 32 bytes)
static const bvalue be_ktab_class_lv_wifi_arcs[10] = {
  /* K0   */  be_nested_str_weak(tasmota),
  /* K1   */  be_nested_str_weak(remove_driver),
  /* K2   */  be_nested_str_weak(init),
  /* K3   */  be_nested_str_weak(add_driver),
  /* K4   */  be_nested_str_weak(set_percentage),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(wifi),
  /* K7   */  be_nested_str_weak(find),
  /* K8   */  be_nested_str_weak(quality),
  /* K9   */  be_nested_str_weak(ip),
};


extern const bclass be_class_lv_wifi_arcs;

/********************************************************************
** Solidified function: before_del
********************************************************************/
be_local_closure(class_lv_wifi_arcs_before_del,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_wifi_arcs,     /* shared constants */
    be_str_weak(before_del),
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
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_wifi_arcs_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_wifi_arcs,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0000,  //  0006  GETNGBL	R2	K0
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x5C100000,  //  0008  MOVE	R4	R0
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080104,  //  000A  GETMET	R2	R0	K4
      0x58100005,  //  000B  LDCONST	R4	K5
      0x7C080400,  //  000C  CALL	R2	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_lv_wifi_arcs_every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_wifi_arcs,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040306,  //  0001  GETMET	R1	R1	K6
      0x7C040200,  //  0002  CALL	R1	1
      0x8C080307,  //  0003  GETMET	R2	R1	K7
      0x58100008,  //  0004  LDCONST	R4	K8
      0x7C080400,  //  0005  CALL	R2	2
      0x8C0C0307,  //  0006  GETMET	R3	R1	K7
      0x58140009,  //  0007  LDCONST	R5	K9
      0x7C0C0400,  //  0008  CALL	R3	2
      0x4C100000,  //  0009  LDNIL	R4
      0x1C100604,  //  000A  EQ	R4	R3	R4
      0x78120003,  //  000B  JMPF	R4	#0010
      0x8C100104,  //  000C  GETMET	R4	R0	K4
      0x58180005,  //  000D  LDCONST	R6	K5
      0x7C100400,  //  000E  CALL	R4	2
      0x70020005,  //  000F  JMP		#0016
      0x4C100000,  //  0010  LDNIL	R4
      0x20100404,  //  0011  NE	R4	R2	R4
      0x78120002,  //  0012  JMPF	R4	#0016
      0x8C100104,  //  0013  GETMET	R4	R0	K4
      0x5C180400,  //  0014  MOVE	R6	R2
      0x7C100400,  //  0015  CALL	R4	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_wifi_arcs
********************************************************************/
extern const bclass be_class_lv_signal_arcs;
be_local_class(lv_wifi_arcs,
    0,
    &be_class_lv_signal_arcs,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(before_del, 1), be_const_closure(class_lv_wifi_arcs_before_del_closure) },
        { be_const_key_weak(every_second, 2), be_const_closure(class_lv_wifi_arcs_every_second_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_lv_wifi_arcs_init_closure) },
    })),
    be_str_weak(lv_wifi_arcs)
);

extern const bclass be_class_lv_wifi_arcs_icon;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_wifi_arcs_icon_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(set_style_line_color),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(color),
    /* K4   */  be_nested_str_weak(COLOR_WHITE),
    /* K5   */  be_nested_str_weak(PART_MAIN),
    /* K6   */  be_nested_str_weak(STATE_DEFAULT),
    /* K7   */  be_nested_str_weak(set_style_bg_color),
    /* K8   */  be_nested_str_weak(COLOR_BLACK),
    /* K9   */  be_nested_str_weak(get_height),
    /* K10  */  be_nested_str_weak(get_style_pad_right),
    /* K11  */  be_nested_str_weak(set_height),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(set_width),
    /* K14  */  be_nested_str_weak(set_x),
    /* K15  */  be_nested_str_weak(get_width),
    /* K16  */  be_nested_str_weak(set_style_pad_right),
    /* K17  */  be_const_int(1),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[67]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0xB8120400,  //  0007  GETNGBL	R4	K2
      0x8C100903,  //  0008  GETMET	R4	R4	K3
      0xB81A0400,  //  0009  GETNGBL	R6	K2
      0x88180D04,  //  000A  GETMBR	R6	R6	K4
      0x7C100400,  //  000B  CALL	R4	2
      0xB8160400,  //  000C  GETNGBL	R5	K2
      0x88140B05,  //  000D  GETMBR	R5	R5	K5
      0xB81A0400,  //  000E  GETNGBL	R6	K2
      0x88180D06,  //  000F  GETMBR	R6	R6	K6
      0x30140A06,  //  0010  OR	R5	R5	R6
      0x7C080600,  //  0011  CALL	R2	3
      0x8C080107,  //  0012  GETMET	R2	R0	K7
      0xB8120400,  //  0013  GETNGBL	R4	K2
      0x8C100903,  //  0014  GETMET	R4	R4	K3
      0xB81A0400,  //  0015  GETNGBL	R6	K2
      0x88180D08,  //  0016  GETMBR	R6	R6	K8
      0x7C100400,  //  0017  CALL	R4	2
      0xB8160400,  //  0018  GETNGBL	R5	K2
      0x88140B05,  //  0019  GETMBR	R5	R5	K5
      0xB81A0400,  //  001A  GETNGBL	R6	K2
      0x88180D06,  //  001B  GETMBR	R6	R6	K6
      0x30140A06,  //  001C  OR	R5	R5	R6
      0x7C080600,  //  001D  CALL	R2	3
      0x4C080000,  //  001E  LDNIL	R2
      0x20080202,  //  001F  NE	R2	R1	R2
      0x780A0020,  //  0020  JMPF	R2	#0042
      0x8C080309,  //  0021  GETMET	R2	R1	K9
      0x7C080200,  //  0022  CALL	R2	1
      0x8C0C030A,  //  0023  GETMET	R3	R1	K10
      0xB8160400,  //  0024  GETNGBL	R5	K2
      0x88140B05,  //  0025  GETMBR	R5	R5	K5
      0xB81A0400,  //  0026  GETNGBL	R6	K2
      0x88180D06,  //  0027  GETMBR	R6	R6	K6
      0x30140A06,  //  0028  OR	R5	R5	R6
      0x7C0C0400,  //  0029  CALL	R3	2
      0x8C10010B,  //  002A  GETMET	R4	R0	K11
      0x5C180400,  //  002B  MOVE	R6	R2
      0x7C100400,  //  002C  CALL	R4	2
      0x54120003,  //  002D  LDINT	R4	4
      0x08100404,  //  002E  MUL	R4	R2	R4
      0x0C10090C,  //  002F  DIV	R4	R4	K12
      0x8C14010D,  //  0030  GETMET	R5	R0	K13
      0x5C1C0800,  //  0031  MOVE	R7	R4
      0x7C140400,  //  0032  CALL	R5	2
      0x8C14010E,  //  0033  GETMET	R5	R0	K14
      0x8C1C030F,  //  0034  GETMET	R7	R1	K15
      0x7C1C0200,  //  0035  CALL	R7	1
      0x041C0E04,  //  0036  SUB	R7	R7	R4
      0x041C0E03,  //  0037  SUB	R7	R7	R3
      0x7C140400,  //  0038  CALL	R5	2
      0x8C140310,  //  0039  GETMET	R5	R1	K16
      0x001C0604,  //  003A  ADD	R7	R3	R4
      0x001C0F11,  //  003B  ADD	R7	R7	K17
      0xB8220400,  //  003C  GETNGBL	R8	K2
      0x88201105,  //  003D  GETMBR	R8	R8	K5
      0xB8260400,  //  003E  GETNGBL	R9	K2
      0x88241306,  //  003F  GETMBR	R9	R9	K6
      0x30201009,  //  0040  OR	R8	R8	R9
      0x7C140600,  //  0041  CALL	R5	3
      0x80000000,  //  0042  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_wifi_arcs_icon
********************************************************************/
extern const bclass be_class_lv_wifi_arcs;
be_local_class(lv_wifi_arcs_icon,
    0,
    &be_class_lv_wifi_arcs,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_lv_wifi_arcs_icon_init_closure) },
    })),
    be_str_weak(lv_wifi_arcs_icon)
);
// compact class 'lv_signal_bars' ktab size: 40, total: 49 (saved 72 bytes)
static const bvalue be_ktab_class_lv_signal_bars[40] = {
  /* K0   */  be_nested_str_weak(percentage),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str_weak(invalidate),
  /* K3   */  be_nested_str_weak(get_code),
  /* K4   */  be_nested_str_weak(get_height),
  /* K5   */  be_nested_str_weak(get_width),
  /* K6   */  be_const_int(3),
  /* K7   */  be_const_int(2),
  /* K8   */  be_nested_str_weak(lv),
  /* K9   */  be_nested_str_weak(EVENT_DRAW_MAIN),
  /* K10  */  be_nested_str_weak(line_dsc),
  /* K11  */  be_nested_str_weak(get_param),
  /* K12  */  be_nested_str_weak(get_layer),
  /* K13  */  be_nested_str_weak(get_coords),
  /* K14  */  be_nested_str_weak(area),
  /* K15  */  be_nested_str_weak(x1),
  /* K16  */  be_nested_str_weak(y1),
  /* K17  */  be_nested_str_weak(draw_line_dsc_init),
  /* K18  */  be_nested_str_weak(init_draw_line_dsc),
  /* K19  */  be_nested_str_weak(PART_MAIN),
  /* K20  */  be_nested_str_weak(round_start),
  /* K21  */  be_const_int(1),
  /* K22  */  be_nested_str_weak(round_end),
  /* K23  */  be_nested_str_weak(width),
  /* K24  */  be_nested_str_weak(get_style_line_color),
  /* K25  */  be_nested_str_weak(STATE_DEFAULT),
  /* K26  */  be_nested_str_weak(get_style_bg_color),
  /* K27  */  be_nested_str_weak(color),
  /* K28  */  be_nested_str_weak(p1_y),
  /* K29  */  be_nested_str_weak(p1_x),
  /* K30  */  be_nested_str_weak(p2_y),
  /* K31  */  be_nested_str_weak(p2_x),
  /* K32  */  be_nested_str_weak(draw_line),
  /* K33  */  be_nested_str_weak(stop_iteration),
  /* K34  */  be_nested_str_weak(init),
  /* K35  */  be_nested_str_weak(set_style_bg_opa),
  /* K36  */  be_nested_str_weak(set_style_border_width),
  /* K37  */  be_nested_str_weak(draw_line_dsc),
  /* K38  */  be_nested_str_weak(add_event_cb),
  /* K39  */  be_nested_str_weak(widget_event),
};


extern const bclass be_class_lv_signal_bars;

/********************************************************************
** Solidified function: set_percentage
********************************************************************/
be_local_closure(class_lv_signal_bars_set_percentage,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_signal_bars,     /* shared constants */
    be_str_weak(set_percentage),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x540E0013,  //  0001  LDINT	R3	20
      0x0C080403,  //  0002  DIV	R2	R2	R3
      0x540E0063,  //  0003  LDINT	R3	100
      0x240C0203,  //  0004  GT	R3	R1	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x54060063,  //  0006  LDINT	R1	100
      0x140C0301,  //  0007  LT	R3	R1	K1
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x58040001,  //  0009  LDCONST	R1	K1
      0x90020001,  //  000A  SETMBR	R0	K0	R1
      0x540E0013,  //  000B  LDINT	R3	20
      0x0C0C0203,  //  000C  DIV	R3	R1	R3
      0x200C0403,  //  000D  NE	R3	R2	R3
      0x780E0001,  //  000E  JMPF	R3	#0011
      0x8C0C0102,  //  000F  GETMET	R3	R0	K2
      0x7C0C0200,  //  0010  CALL	R3	1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_percentage
********************************************************************/
be_local_closure(class_lv_signal_bars_get_percentage,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_signal_bars,     /* shared constants */
    be_str_weak(get_percentage),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_event
********************************************************************/
be_local_closure(class_lv_signal_bars_widget_event,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_const_int(1),
        }),
        be_str_weak(atleast1),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x28040100,  //  0000  GE	R1	R0	K0
          0x78060001,  //  0001  JMPF	R1	#0004
          0x80040000,  //  0002  RET	1	R0
          0x70020000,  //  0003  JMP		#0005
          0x80060000,  //  0004  RET	1	K0
          0x80000000,  //  0005  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_lv_signal_bars,     /* shared constants */
    be_str_weak(widget_event),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0x8C080303,  //  0000  GETMET	R2	R1	K3
      0x7C080200,  //  0001  CALL	R2	1
      0x840C0000,  //  0002  CLOSURE	R3	P0
      0x8C100104,  //  0003  GETMET	R4	R0	K4
      0x7C100200,  //  0004  CALL	R4	1
      0x8C140105,  //  0005  GETMET	R5	R0	K5
      0x7C140200,  //  0006  CALL	R5	1
      0x5C180600,  //  0007  MOVE	R6	R3
      0x541E000E,  //  0008  LDINT	R7	15
      0x0C1C0A07,  //  0009  DIV	R7	R5	R7
      0x7C180200,  //  000A  CALL	R6	1
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x08200D06,  //  000C  MUL	R8	R6	K6
      0x04200A08,  //  000D  SUB	R8	R5	R8
      0x54260003,  //  000E  LDINT	R9	4
      0x0C201009,  //  000F  DIV	R8	R8	R9
      0x7C1C0200,  //  0010  CALL	R7	1
      0x0C200F07,  //  0011  DIV	R8	R7	K7
      0xB8261000,  //  0012  GETNGBL	R9	K8
      0x88241309,  //  0013  GETMBR	R9	R9	K9
      0x1C240409,  //  0014  EQ	R9	R2	R9
      0x78260051,  //  0015  JMPF	R9	#0068
      0x8824010A,  //  0016  GETMBR	R9	R0	K10
      0x8C28030B,  //  0017  GETMET	R10	R1	K11
      0x7C280200,  //  0018  CALL	R10	1
      0x8C2C030C,  //  0019  GETMET	R11	R1	K12
      0x7C2C0200,  //  001A  CALL	R11	1
      0x8C30010D,  //  001B  GETMET	R12	R0	K13
      0x8838010E,  //  001C  GETMBR	R14	R0	K14
      0x7C300400,  //  001D  CALL	R12	2
      0x8830010E,  //  001E  GETMBR	R12	R0	K14
      0x8830190F,  //  001F  GETMBR	R12	R12	K15
      0x8834010E,  //  0020  GETMBR	R13	R0	K14
      0x88341B10,  //  0021  GETMBR	R13	R13	K16
      0xB83A1000,  //  0022  GETNGBL	R14	K8
      0x8C381D11,  //  0023  GETMET	R14	R14	K17
      0x5C401200,  //  0024  MOVE	R16	R9
      0x7C380400,  //  0025  CALL	R14	2
      0x8C380112,  //  0026  GETMET	R14	R0	K18
      0xB8421000,  //  0027  GETNGBL	R16	K8
      0x88402113,  //  0028  GETMBR	R16	R16	K19
      0x5C441200,  //  0029  MOVE	R17	R9
      0x7C380600,  //  002A  CALL	R14	3
      0x90262915,  //  002B  SETMBR	R9	K20	K21
      0x90262D15,  //  002C  SETMBR	R9	K22	K21
      0x90262E07,  //  002D  SETMBR	R9	K23	R7
      0x8C380118,  //  002E  GETMET	R14	R0	K24
      0xB8421000,  //  002F  GETNGBL	R16	K8
      0x88402113,  //  0030  GETMBR	R16	R16	K19
      0xB8461000,  //  0031  GETNGBL	R17	K8
      0x88442319,  //  0032  GETMBR	R17	R17	K25
      0x30402011,  //  0033  OR	R16	R16	R17
      0x7C380400,  //  0034  CALL	R14	2
      0x8C3C011A,  //  0035  GETMET	R15	R0	K26
      0xB8461000,  //  0036  GETNGBL	R17	K8
      0x88442313,  //  0037  GETMBR	R17	R17	K19
      0xB84A1000,  //  0038  GETNGBL	R18	K8
      0x88482519,  //  0039  GETMBR	R18	R18	K25
      0x30442212,  //  003A  OR	R17	R17	R18
      0x7C3C0400,  //  003B  CALL	R15	2
      0x60400010,  //  003C  GETGBL	R16	G16
      0x40460306,  //  003D  CONNECT	R17	K1	K6
      0x7C400200,  //  003E  CALL	R16	1
      0xA8020024,  //  003F  EXBLK	0	#0065
      0x5C442000,  //  0040  MOVE	R17	R16
      0x7C440000,  //  0041  CALL	R17	0
      0x88480100,  //  0042  GETMBR	R18	R0	K0
      0x004C2315,  //  0043  ADD	R19	R17	K21
      0x54520013,  //  0044  LDINT	R20	20
      0x084C2614,  //  0045  MUL	R19	R19	R20
      0x28482413,  //  0046  GE	R18	R18	R19
      0x784A0001,  //  0047  JMPF	R18	#004A
      0x5C481C00,  //  0048  MOVE	R18	R14
      0x70020000,  //  0049  JMP		#004B
      0x5C481E00,  //  004A  MOVE	R18	R15
      0x90263612,  //  004B  SETMBR	R9	K27	R18
      0x00481A04,  //  004C  ADD	R18	R13	R4
      0x04482515,  //  004D  SUB	R18	R18	K21
      0x04482408,  //  004E  SUB	R18	R18	R8
      0x90263812,  //  004F  SETMBR	R9	K28	R18
      0x00480E06,  //  0050  ADD	R18	R7	R6
      0x08482212,  //  0051  MUL	R18	R17	R18
      0x00481812,  //  0052  ADD	R18	R12	R18
      0x00482408,  //  0053  ADD	R18	R18	R8
      0x90263A12,  //  0054  SETMBR	R9	K29	R18
      0x044A0C11,  //  0055  SUB	R18	K6	R17
      0x044C0807,  //  0056  SUB	R19	R4	R7
      0x08482413,  //  0057  MUL	R18	R18	R19
      0x544E0003,  //  0058  LDINT	R19	4
      0x0C482413,  //  0059  DIV	R18	R18	R19
      0x00481A12,  //  005A  ADD	R18	R13	R18
      0x00482408,  //  005B  ADD	R18	R18	R8
      0x90263C12,  //  005C  SETMBR	R9	K30	R18
      0x8848131D,  //  005D  GETMBR	R18	R9	K29
      0x90263E12,  //  005E  SETMBR	R9	K31	R18
      0xB84A1000,  //  005F  GETNGBL	R18	K8
      0x8C482520,  //  0060  GETMET	R18	R18	K32
      0x5C501600,  //  0061  MOVE	R20	R11
      0x5C541200,  //  0062  MOVE	R21	R9
      0x7C480600,  //  0063  CALL	R18	3
      0x7001FFDA,  //  0064  JMP		#0040
      0x58400021,  //  0065  LDCONST	R16	K33
      0xAC400200,  //  0066  CATCH	R16	1	0
      0xB0080000,  //  0067  RAISE	2	R0	R0
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_signal_bars_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_signal_bars,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080522,  //  0003  GETMET	R2	R2	K34
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080123,  //  0006  GETMET	R2	R0	K35
      0x58100001,  //  0007  LDCONST	R4	K1
      0x58140001,  //  0008  LDCONST	R5	K1
      0x7C080600,  //  0009  CALL	R2	3
      0x8C080124,  //  000A  GETMET	R2	R0	K36
      0x58100001,  //  000B  LDCONST	R4	K1
      0x58140001,  //  000C  LDCONST	R5	K1
      0x7C080600,  //  000D  CALL	R2	3
      0x540A0063,  //  000E  LDINT	R2	100
      0x90020002,  //  000F  SETMBR	R0	K0	R2
      0xB80A1000,  //  0010  GETNGBL	R2	K8
      0x8C08050E,  //  0011  GETMET	R2	R2	K14
      0x7C080200,  //  0012  CALL	R2	1
      0x90021C02,  //  0013  SETMBR	R0	K14	R2
      0xB80A1000,  //  0014  GETNGBL	R2	K8
      0x8C080525,  //  0015  GETMET	R2	R2	K37
      0x7C080200,  //  0016  CALL	R2	1
      0x90021402,  //  0017  SETMBR	R0	K10	R2
      0x8C080126,  //  0018  GETMET	R2	R0	K38
      0x88100127,  //  0019  GETMBR	R4	R0	K39
      0xB8161000,  //  001A  GETNGBL	R5	K8
      0x88140B09,  //  001B  GETMBR	R5	R5	K9
      0x58180001,  //  001C  LDCONST	R6	K1
      0x7C080800,  //  001D  CALL	R2	4
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_signal_bars
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_bars,
    3,
    &be_class_lv_obj,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_percentage, 3), be_const_closure(class_lv_signal_bars_set_percentage_closure) },
        { be_const_key_weak(get_percentage, -1), be_const_closure(class_lv_signal_bars_get_percentage_closure) },
        { be_const_key_weak(percentage, -1), be_const_var(0) },
        { be_const_key_weak(area, 6), be_const_var(1) },
        { be_const_key_weak(line_dsc, 2), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(class_lv_signal_bars_init_closure) },
        { be_const_key_weak(widget_event, -1), be_const_closure(class_lv_signal_bars_widget_event_closure) },
    })),
    be_str_weak(lv_signal_bars)
);
// compact class 'lv_wifi_bars' ktab size: 10, total: 14 (saved 32 bytes)
static const bvalue be_ktab_class_lv_wifi_bars[10] = {
  /* K0   */  be_nested_str_weak(tasmota),
  /* K1   */  be_nested_str_weak(remove_driver),
  /* K2   */  be_nested_str_weak(init),
  /* K3   */  be_nested_str_weak(add_driver),
  /* K4   */  be_nested_str_weak(set_percentage),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(wifi),
  /* K7   */  be_nested_str_weak(find),
  /* K8   */  be_nested_str_weak(quality),
  /* K9   */  be_nested_str_weak(ip),
};


extern const bclass be_class_lv_wifi_bars;

/********************************************************************
** Solidified function: before_del
********************************************************************/
be_local_closure(class_lv_wifi_bars_before_del,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_wifi_bars,     /* shared constants */
    be_str_weak(before_del),
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
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_wifi_bars_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_wifi_bars,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0000,  //  0006  GETNGBL	R2	K0
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x5C100000,  //  0008  MOVE	R4	R0
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080104,  //  000A  GETMET	R2	R0	K4
      0x58100005,  //  000B  LDCONST	R4	K5
      0x7C080400,  //  000C  CALL	R2	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_lv_wifi_bars_every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_lv_wifi_bars,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040306,  //  0001  GETMET	R1	R1	K6
      0x7C040200,  //  0002  CALL	R1	1
      0x8C080307,  //  0003  GETMET	R2	R1	K7
      0x58100008,  //  0004  LDCONST	R4	K8
      0x7C080400,  //  0005  CALL	R2	2
      0x8C0C0307,  //  0006  GETMET	R3	R1	K7
      0x58140009,  //  0007  LDCONST	R5	K9
      0x7C0C0400,  //  0008  CALL	R3	2
      0x4C100000,  //  0009  LDNIL	R4
      0x1C100604,  //  000A  EQ	R4	R3	R4
      0x78120003,  //  000B  JMPF	R4	#0010
      0x8C100104,  //  000C  GETMET	R4	R0	K4
      0x58180005,  //  000D  LDCONST	R6	K5
      0x7C100400,  //  000E  CALL	R4	2
      0x70020005,  //  000F  JMP		#0016
      0x4C100000,  //  0010  LDNIL	R4
      0x20100404,  //  0011  NE	R4	R2	R4
      0x78120002,  //  0012  JMPF	R4	#0016
      0x8C100104,  //  0013  GETMET	R4	R0	K4
      0x5C180400,  //  0014  MOVE	R6	R2
      0x7C100400,  //  0015  CALL	R4	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_wifi_bars
********************************************************************/
extern const bclass be_class_lv_signal_bars;
be_local_class(lv_wifi_bars,
    0,
    &be_class_lv_signal_bars,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(before_del, 1), be_const_closure(class_lv_wifi_bars_before_del_closure) },
        { be_const_key_weak(every_second, 2), be_const_closure(class_lv_wifi_bars_every_second_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_lv_wifi_bars_init_closure) },
    })),
    be_str_weak(lv_wifi_bars)
);

extern const bclass be_class_lv_wifi_bars_icon;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_lv_wifi_bars_icon_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(set_style_line_color),
    /* K2   */  be_nested_str_weak(lv),
    /* K3   */  be_nested_str_weak(color),
    /* K4   */  be_nested_str_weak(COLOR_WHITE),
    /* K5   */  be_nested_str_weak(PART_MAIN),
    /* K6   */  be_nested_str_weak(STATE_DEFAULT),
    /* K7   */  be_nested_str_weak(set_style_bg_color),
    /* K8   */  be_nested_str_weak(COLOR_BLACK),
    /* K9   */  be_nested_str_weak(get_height),
    /* K10  */  be_nested_str_weak(get_style_pad_right),
    /* K11  */  be_nested_str_weak(set_height),
    /* K12  */  be_nested_str_weak(set_width),
    /* K13  */  be_nested_str_weak(set_x),
    /* K14  */  be_nested_str_weak(get_width),
    /* K15  */  be_nested_str_weak(set_style_pad_right),
    /* K16  */  be_const_int(1),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0xB8120400,  //  0007  GETNGBL	R4	K2
      0x8C100903,  //  0008  GETMET	R4	R4	K3
      0xB81A0400,  //  0009  GETNGBL	R6	K2
      0x88180D04,  //  000A  GETMBR	R6	R6	K4
      0x7C100400,  //  000B  CALL	R4	2
      0xB8160400,  //  000C  GETNGBL	R5	K2
      0x88140B05,  //  000D  GETMBR	R5	R5	K5
      0xB81A0400,  //  000E  GETNGBL	R6	K2
      0x88180D06,  //  000F  GETMBR	R6	R6	K6
      0x30140A06,  //  0010  OR	R5	R5	R6
      0x7C080600,  //  0011  CALL	R2	3
      0x8C080107,  //  0012  GETMET	R2	R0	K7
      0xB8120400,  //  0013  GETNGBL	R4	K2
      0x8C100903,  //  0014  GETMET	R4	R4	K3
      0xB81A0400,  //  0015  GETNGBL	R6	K2
      0x88180D08,  //  0016  GETMBR	R6	R6	K8
      0x7C100400,  //  0017  CALL	R4	2
      0xB8160400,  //  0018  GETNGBL	R5	K2
      0x88140B05,  //  0019  GETMBR	R5	R5	K5
      0xB81A0400,  //  001A  GETNGBL	R6	K2
      0x88180D06,  //  001B  GETMBR	R6	R6	K6
      0x30140A06,  //  001C  OR	R5	R5	R6
      0x7C080600,  //  001D  CALL	R2	3
      0x4C080000,  //  001E  LDNIL	R2
      0x20080202,  //  001F  NE	R2	R1	R2
      0x780A001D,  //  0020  JMPF	R2	#003F
      0x8C080309,  //  0021  GETMET	R2	R1	K9
      0x7C080200,  //  0022  CALL	R2	1
      0x8C0C030A,  //  0023  GETMET	R3	R1	K10
      0xB8160400,  //  0024  GETNGBL	R5	K2
      0x88140B05,  //  0025  GETMBR	R5	R5	K5
      0xB81A0400,  //  0026  GETNGBL	R6	K2
      0x88180D06,  //  0027  GETMBR	R6	R6	K6
      0x30140A06,  //  0028  OR	R5	R5	R6
      0x7C0C0400,  //  0029  CALL	R3	2
      0x8C10010B,  //  002A  GETMET	R4	R0	K11
      0x5C180400,  //  002B  MOVE	R6	R2
      0x7C100400,  //  002C  CALL	R4	2
      0x8C10010C,  //  002D  GETMET	R4	R0	K12
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x8C10010D,  //  0030  GETMET	R4	R0	K13
      0x8C18030E,  //  0031  GETMET	R6	R1	K14
      0x7C180200,  //  0032  CALL	R6	1
      0x04180C02,  //  0033  SUB	R6	R6	R2
      0x04180C03,  //  0034  SUB	R6	R6	R3
      0x7C100400,  //  0035  CALL	R4	2
      0x8C10030F,  //  0036  GETMET	R4	R1	K15
      0x00180602,  //  0037  ADD	R6	R3	R2
      0x00180D10,  //  0038  ADD	R6	R6	K16
      0xB81E0400,  //  0039  GETNGBL	R7	K2
      0x881C0F05,  //  003A  GETMBR	R7	R7	K5
      0xB8220400,  //  003B  GETNGBL	R8	K2
      0x88201106,  //  003C  GETMBR	R8	R8	K6
      0x301C0E08,  //  003D  OR	R7	R7	R8
      0x7C100600,  //  003E  CALL	R4	3
      0x80000000,  //  003F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_wifi_bars_icon
********************************************************************/
extern const bclass be_class_lv_wifi_bars;
be_local_class(lv_wifi_bars_icon,
    0,
    &be_class_lv_wifi_bars,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_lv_wifi_bars_icon_init_closure) },
    })),
    be_str_weak(lv_wifi_bars_icon)
);
/********************************************************************/
/* End of solidification */
