/* Solidification of lv_tasmota_widgets.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_lv_clock;

/********************************************************************
** Solidified function: set_time
********************************************************************/
be_local_closure(lv_clock_set_time,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(hour),
    /* K1   */  be_nested_str_weak(minute),
    /* K2   */  be_nested_str_weak(sec),
    /* K3   */  be_nested_str_weak(_X2502d_X25s_X2502d),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(_X3A),
    /* K6   */  be_nested_str_weak(_X20),
    /* K7   */  be_nested_str_weak(set_text),
    }),
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
be_local_closure(lv_clock_every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(time_dump),
    /* K2   */  be_nested_str_weak(rtc),
    /* K3   */  be_nested_str_weak(local),
    /* K4   */  be_nested_str_weak(year),
    /* K5   */  be_nested_str_weak(set_time),
    /* K6   */  be_nested_str_weak(hour),
    /* K7   */  be_nested_str_weak(min),
    /* K8   */  be_nested_str_weak(sec),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0xB80E0000,  //  0002  GETNGBL	R3	K0
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x7C0C0200,  //  0004  CALL	R3	1
      0x940C0703,  //  0005  GETIDX	R3	R3	K3
      0x7C040400,  //  0006  CALL	R1	2
      0x94080304,  //  0007  GETIDX	R2	R1	K4
      0x540E07B1,  //  0008  LDINT	R3	1970
      0x20080403,  //  0009  NE	R2	R2	R3
      0x780A0004,  //  000A  JMPF	R2	#0010
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x94100306,  //  000C  GETIDX	R4	R1	K6
      0x94140307,  //  000D  GETIDX	R5	R1	K7
      0x94180308,  //  000E  GETIDX	R6	R1	K8
      0x7C080800,  //  000F  CALL	R2	4
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_clock_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
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
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(lv),
    /* K2   */  be_nested_str_weak(seg7_font),
    /* K3   */  be_nested_str_weak(set_style_text_font),
    /* K4   */  be_nested_str_weak(PART_MAIN),
    /* K5   */  be_nested_str_weak(STATE_DEFAULT),
    /* K6   */  be_nested_str_weak(set_text),
    /* K7   */  be_nested_str_weak(_X2D_X2D_X3A_X2D_X2D),
    /* K8   */  be_nested_str_weak(add_event_cb),
    /* K9   */  be_nested_str_weak(EVENT_DELETE),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(tasmota),
    /* K12  */  be_nested_str_weak(add_driver),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0200,  //  0006  GETNGBL	R2	K1
      0x8C080502,  //  0007  GETMET	R2	R2	K2
      0x5412000F,  //  0008  LDINT	R4	16
      0x7C080400,  //  0009  CALL	R2	2
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0007,  //  000C  JMPF	R3	#0015
      0x8C0C0103,  //  000D  GETMET	R3	R0	K3
      0x5C140400,  //  000E  MOVE	R5	R2
      0xB81A0200,  //  000F  GETNGBL	R6	K1
      0x88180D04,  //  0010  GETMBR	R6	R6	K4
      0xB81E0200,  //  0011  GETNGBL	R7	K1
      0x881C0F05,  //  0012  GETMBR	R7	R7	K5
      0x30180C07,  //  0013  OR	R6	R6	R7
      0x7C0C0600,  //  0014  CALL	R3	3
      0x8C0C0106,  //  0015  GETMET	R3	R0	K6
      0x58140007,  //  0016  LDCONST	R5	K7
      0x7C0C0400,  //  0017  CALL	R3	2
      0x8C0C0108,  //  0018  GETMET	R3	R0	K8
      0x84140000,  //  0019  CLOSURE	R5	P0
      0xB81A0200,  //  001A  GETNGBL	R6	K1
      0x88180D09,  //  001B  GETMBR	R6	R6	K9
      0x581C000A,  //  001C  LDCONST	R7	K10
      0x7C0C0800,  //  001D  CALL	R3	4
      0xB80E1600,  //  001E  GETNGBL	R3	K11
      0x8C0C070C,  //  001F  GETMET	R3	R3	K12
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
be_local_closure(lv_clock_before_del,   /* name */
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
    /* K1   */  be_nested_str_weak(remove_driver),
    }),
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
        { be_const_key_weak(before_del, 6), be_const_closure(lv_clock_before_del_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(lv_clock_every_second_closure) },
        { be_const_key_weak(minute, -1), be_const_var(1) },
        { be_const_key_weak(set_time, 2), be_const_closure(lv_clock_set_time_closure) },
        { be_const_key_weak(init, -1), be_const_closure(lv_clock_init_closure) },
    })),
    be_str_weak(lv_clock)
);
/*******************************************************************/

void be_load_lv_clock_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_clock);
    be_setglobal(vm, "lv_clock");
    be_pop(vm, 1);
}

extern const bclass be_class_lv_clock_icon;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_clock_icon_init,   /* name */
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
        { be_const_key_weak(init, -1), be_const_closure(lv_clock_icon_init_closure) },
    })),
    be_str_weak(lv_clock_icon)
);
/*******************************************************************/

void be_load_lv_clock_icon_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_clock_icon);
    be_setglobal(vm, "lv_clock_icon");
    be_pop(vm, 1);
}

extern const bclass be_class_lv_signal_arcs;

/********************************************************************
** Solidified function: widget_event
********************************************************************/
be_local_closure(lv_signal_arcs_widget_event,   /* name */
  be_nested_proto(
    29,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
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
    ( &(const bvalue[37]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(obj_event_base),
    /* K2   */  be_nested_str_weak(RES_OK),
    /* K3   */  be_nested_str_weak(get_code),
    /* K4   */  be_nested_str_weak(math),
    /* K5   */  be_nested_str_weak(get_height),
    /* K6   */  be_nested_str_weak(get_width),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(EVENT_DRAW_MAIN),
    /* K10  */  be_nested_str_weak(arc_dsc),
    /* K11  */  be_nested_str_weak(get_param),
    /* K12  */  be_nested_str_weak(get_layer),
    /* K13  */  be_nested_str_weak(get_coords),
    /* K14  */  be_nested_str_weak(area),
    /* K15  */  be_nested_str_weak(x1),
    /* K16  */  be_nested_str_weak(y1),
    /* K17  */  be_nested_str_weak(draw_arc_dsc_init),
    /* K18  */  be_nested_str_weak(init_draw_arc_dsc),
    /* K19  */  be_nested_str_weak(PART_MAIN),
    /* K20  */  be_nested_str_weak(rounded),
    /* K21  */  be_const_int(1),
    /* K22  */  be_nested_str_weak(width),
    /* K23  */  be_nested_str_weak(get_style_line_color),
    /* K24  */  be_nested_str_weak(STATE_DEFAULT),
    /* K25  */  be_nested_str_weak(get_style_bg_color),
    /* K26  */  be_nested_str_weak(deg),
    /* K27  */  be_nested_str_weak(acos),
    /* K28  */  be_nested_str_weak(center_x),
    /* K29  */  be_nested_str_weak(center_y),
    /* K30  */  be_nested_str_weak(color),
    /* K31  */  be_nested_str_weak(percentage),
    /* K32  */  be_nested_str_weak(radius),
    /* K33  */  be_const_int(0),
    /* K34  */  be_nested_str_weak(start_angle),
    /* K35  */  be_nested_str_weak(end_angle),
    /* K36  */  be_nested_str_weak(draw_arc),
    }),
    be_str_weak(widget_event),
    &be_const_str_solidified,
    ( &(const binstruction[171]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x7C0C0600,  //  0004  CALL	R3	3
      0xB8120000,  //  0005  GETNGBL	R4	K0
      0x88100902,  //  0006  GETMBR	R4	R4	K2
      0x200C0604,  //  0007  NE	R3	R3	R4
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x80000600,  //  0009  RET	0
      0x8C0C0503,  //  000A  GETMET	R3	R2	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0xA4120800,  //  000C  IMPORT	R4	K4
      0x84140000,  //  000D  CLOSURE	R5	P0
      0x8C180105,  //  000E  GETMET	R6	R0	K5
      0x7C180200,  //  000F  CALL	R6	1
      0x8C1C0106,  //  0010  GETMET	R7	R0	K6
      0x7C1C0200,  //  0011  CALL	R7	1
      0x5C200A00,  //  0012  MOVE	R8	R5
      0x54260007,  //  0013  LDINT	R9	8
      0x0C240C09,  //  0014  DIV	R9	R6	R9
      0x7C200200,  //  0015  CALL	R8	1
      0x5C240A00,  //  0016  MOVE	R9	R5
      0x08281107,  //  0017  MUL	R10	R8	K7
      0x04280C0A,  //  0018  SUB	R10	R6	R10
      0x0C281508,  //  0019  DIV	R10	R10	K8
      0x7C240200,  //  001A  CALL	R9	1
      0x0C281307,  //  001B  DIV	R10	R9	K7
      0xB82E0000,  //  001C  GETNGBL	R11	K0
      0x882C1709,  //  001D  GETMBR	R11	R11	K9
      0x1C2C060B,  //  001E  EQ	R11	R3	R11
      0x782E0089,  //  001F  JMPF	R11	#00AA
      0x882C010A,  //  0020  GETMBR	R11	R0	K10
      0x8C30050B,  //  0021  GETMET	R12	R2	K11
      0x7C300200,  //  0022  CALL	R12	1
      0x8C34050C,  //  0023  GETMET	R13	R2	K12
      0x7C340200,  //  0024  CALL	R13	1
      0x8C38010D,  //  0025  GETMET	R14	R0	K13
      0x8840010E,  //  0026  GETMBR	R16	R0	K14
      0x7C380400,  //  0027  CALL	R14	2
      0x8838010E,  //  0028  GETMBR	R14	R0	K14
      0x88381D0F,  //  0029  GETMBR	R14	R14	K15
      0x883C010E,  //  002A  GETMBR	R15	R0	K14
      0x883C1F10,  //  002B  GETMBR	R15	R15	K16
      0xB8420000,  //  002C  GETNGBL	R16	K0
      0x8C402111,  //  002D  GETMET	R16	R16	K17
      0x5C481600,  //  002E  MOVE	R18	R11
      0x7C400400,  //  002F  CALL	R16	2
      0x8C400112,  //  0030  GETMET	R16	R0	K18
      0xB84A0000,  //  0031  GETNGBL	R18	K0
      0x88482513,  //  0032  GETMBR	R18	R18	K19
      0x5C4C1600,  //  0033  MOVE	R19	R11
      0x7C400600,  //  0034  CALL	R16	3
      0x902E2915,  //  0035  SETMBR	R11	K20	K21
      0x08401308,  //  0036  MUL	R16	R9	K8
      0x00402115,  //  0037  ADD	R16	R16	K21
      0x54460003,  //  0038  LDINT	R17	4
      0x0C402011,  //  0039  DIV	R16	R16	R17
      0x902E2C10,  //  003A  SETMBR	R11	K22	R16
      0x8C400117,  //  003B  GETMET	R16	R0	K23
      0xB84A0000,  //  003C  GETNGBL	R18	K0
      0x88482513,  //  003D  GETMBR	R18	R18	K19
      0xB84E0000,  //  003E  GETNGBL	R19	K0
      0x884C2718,  //  003F  GETMBR	R19	R19	K24
      0x30482413,  //  0040  OR	R18	R18	R19
      0x7C400400,  //  0041  CALL	R16	2
      0x8C440119,  //  0042  GETMET	R17	R0	K25
      0xB84E0000,  //  0043  GETNGBL	R19	K0
      0x884C2713,  //  0044  GETMBR	R19	R19	K19
      0xB8520000,  //  0045  GETNGBL	R20	K0
      0x88502918,  //  0046  GETMBR	R20	R20	K24
      0x304C2614,  //  0047  OR	R19	R19	R20
      0x7C440400,  //  0048  CALL	R17	2
      0x04480C09,  //  0049  SUB	R18	R6	R9
      0x0C4C0F07,  //  004A  DIV	R19	R7	K7
      0x044C260A,  //  004B  SUB	R19	R19	R10
      0x60500009,  //  004C  GETGBL	R20	G9
      0x54560059,  //  004D  LDINT	R21	90
      0x8C58091A,  //  004E  GETMET	R22	R4	K26
      0x8C60091B,  //  004F  GETMET	R24	R4	K27
      0x6068000A,  //  0050  GETGBL	R26	G10
      0x5C6C2600,  //  0051  MOVE	R27	R19
      0x7C680200,  //  0052  CALL	R26	1
      0x606C000A,  //  0053  GETGBL	R27	G10
      0x5C702400,  //  0054  MOVE	R28	R18
      0x7C6C0200,  //  0055  CALL	R27	1
      0x0C68341B,  //  0056  DIV	R26	R26	R27
      0x7C600400,  //  0057  CALL	R24	2
      0x7C580400,  //  0058  CALL	R22	2
      0x04542A16,  //  0059  SUB	R21	R21	R22
      0x7C500200,  //  005A  CALL	R20	1
      0x5456002C,  //  005B  LDINT	R21	45
      0x24542815,  //  005C  GT	R21	R20	R21
      0x78560000,  //  005D  JMPF	R21	#005F
      0x5452002C,  //  005E  LDINT	R20	45
      0x0C540F07,  //  005F  DIV	R21	R7	K7
      0x00541C15,  //  0060  ADD	R21	R14	R21
      0x902E3815,  //  0061  SETMBR	R11	K28	R21
      0x00541E06,  //  0062  ADD	R21	R15	R6
      0x04542B15,  //  0063  SUB	R21	R21	K21
      0x04542A0A,  //  0064  SUB	R21	R21	R10
      0x902E3A15,  //  0065  SETMBR	R11	K29	R21
      0x8854011F,  //  0066  GETMBR	R21	R0	K31
      0x545A0018,  //  0067  LDINT	R22	25
      0x28542A16,  //  0068  GE	R21	R21	R22
      0x78560001,  //  0069  JMPF	R21	#006C
      0x5C542000,  //  006A  MOVE	R21	R16
      0x70020000,  //  006B  JMP		#006D
      0x5C542200,  //  006C  MOVE	R21	R17
      0x902E3C15,  //  006D  SETMBR	R11	K30	R21
      0x00541208,  //  006E  ADD	R21	R9	R8
      0x08564215,  //  006F  MUL	R21	K33	R21
      0x00542A0A,  //  0070  ADD	R21	R21	R10
      0x902E4015,  //  0071  SETMBR	R11	K32	R21
      0x902E4521,  //  0072  SETMBR	R11	K34	K33
      0x54560131,  //  0073  LDINT	R21	306
      0x902E4615,  //  0074  SETMBR	R11	K35	R21
      0xB8560000,  //  0075  GETNGBL	R21	K0
      0x8C542B24,  //  0076  GETMET	R21	R21	K36
      0x5C5C1A00,  //  0077  MOVE	R23	R13
      0x5C601600,  //  0078  MOVE	R24	R11
      0x7C540600,  //  0079  CALL	R21	3
      0x00541208,  //  007A  ADD	R21	R9	R8
      0x08562A15,  //  007B  MUL	R21	K21	R21
      0x00542A0A,  //  007C  ADD	R21	R21	R10
      0x04542B15,  //  007D  SUB	R21	R21	K21
      0x902E4015,  //  007E  SETMBR	R11	K32	R21
      0x5456010D,  //  007F  LDINT	R21	270
      0x04542A14,  //  0080  SUB	R21	R21	R20
      0x902E4415,  //  0081  SETMBR	R11	K34	R21
      0x5456010D,  //  0082  LDINT	R21	270
      0x00542A14,  //  0083  ADD	R21	R21	R20
      0x902E4615,  //  0084  SETMBR	R11	K35	R21
      0x8854011F,  //  0085  GETMBR	R21	R0	K31
      0x545A0031,  //  0086  LDINT	R22	50
      0x28542A16,  //  0087  GE	R21	R21	R22
      0x78560001,  //  0088  JMPF	R21	#008B
      0x5C542000,  //  0089  MOVE	R21	R16
      0x70020000,  //  008A  JMP		#008C
      0x5C542200,  //  008B  MOVE	R21	R17
      0x902E3C15,  //  008C  SETMBR	R11	K30	R21
      0xB8560000,  //  008D  GETNGBL	R21	K0
      0x8C542B24,  //  008E  GETMET	R21	R21	K36
      0x5C5C1A00,  //  008F  MOVE	R23	R13
      0x5C601600,  //  0090  MOVE	R24	R11
      0x7C540600,  //  0091  CALL	R21	3
      0x00541208,  //  0092  ADD	R21	R9	R8
      0x08560E15,  //  0093  MUL	R21	K7	R21
      0x00542A0A,  //  0094  ADD	R21	R21	R10
      0x04542B07,  //  0095  SUB	R21	R21	K7
      0x902E4015,  //  0096  SETMBR	R11	K32	R21
      0x5456010D,  //  0097  LDINT	R21	270
      0x04542A14,  //  0098  SUB	R21	R21	R20
      0x902E4415,  //  0099  SETMBR	R11	K34	R21
      0x5456010D,  //  009A  LDINT	R21	270
      0x00542A14,  //  009B  ADD	R21	R21	R20
      0x902E4615,  //  009C  SETMBR	R11	K35	R21
      0x8854011F,  //  009D  GETMBR	R21	R0	K31
      0x545A004A,  //  009E  LDINT	R22	75
      0x28542A16,  //  009F  GE	R21	R21	R22
      0x78560001,  //  00A0  JMPF	R21	#00A3
      0x5C542000,  //  00A1  MOVE	R21	R16
      0x70020000,  //  00A2  JMP		#00A4
      0x5C542200,  //  00A3  MOVE	R21	R17
      0x902E3C15,  //  00A4  SETMBR	R11	K30	R21
      0xB8560000,  //  00A5  GETNGBL	R21	K0
      0x8C542B24,  //  00A6  GETMET	R21	R21	K36
      0x5C5C1A00,  //  00A7  MOVE	R23	R13
      0x5C601600,  //  00A8  MOVE	R24	R11
      0x7C540600,  //  00A9  CALL	R21	3
      0x80000000,  //  00AA  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_percentage
********************************************************************/
be_local_closure(lv_signal_arcs_get_percentage,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(percentage),
    }),
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
** Solidified function: init
********************************************************************/
be_local_closure(lv_signal_arcs_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lvgl),
    /* K1   */  be_nested_str_weak(create_custom_widget),
    /* K2   */  be_nested_str_weak(percentage),
    /* K3   */  be_nested_str_weak(area),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(arc_dsc),
    /* K6   */  be_nested_str_weak(draw_arc_dsc),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x540A0063,  //  0005  LDINT	R2	100
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0xB80A0800,  //  0007  GETNGBL	R2	K4
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x7C080200,  //  0009  CALL	R2	1
      0x90020602,  //  000A  SETMBR	R0	K3	R2
      0xB80A0800,  //  000B  GETNGBL	R2	K4
      0x8C080506,  //  000C  GETMET	R2	R2	K6
      0x7C080200,  //  000D  CALL	R2	1
      0x90020A02,  //  000E  SETMBR	R0	K5	R2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_percentage
********************************************************************/
be_local_closure(lv_signal_arcs_set_percentage,   /* name */
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
    /* K0   */  be_nested_str_weak(percentage),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(invalidate),
    }),
    be_str_weak(set_percentage),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x540E0018,  //  0001  LDINT	R3	25
      0x0C080403,  //  0002  DIV	R2	R2	R3
      0x540E0063,  //  0003  LDINT	R3	100
      0x240C0203,  //  0004  GT	R3	R1	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x54060063,  //  0006  LDINT	R1	100
      0x140C0301,  //  0007  LT	R3	R1	K1
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x58040001,  //  0009  LDCONST	R1	K1
      0x90020001,  //  000A  SETMBR	R0	K0	R1
      0x540E0018,  //  000B  LDINT	R3	25
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
** Solidified class: lv_signal_arcs
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_arcs,
    3,
    &be_class_lv_obj,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(widget_event, 2), be_const_closure(lv_signal_arcs_widget_event_closure) },
        { be_const_key_weak(get_percentage, 3), be_const_closure(lv_signal_arcs_get_percentage_closure) },
        { be_const_key_weak(set_percentage, 6), be_const_closure(lv_signal_arcs_set_percentage_closure) },
        { be_const_key_weak(arc_dsc, -1), be_const_var(2) },
        { be_const_key_weak(percentage, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(lv_signal_arcs_init_closure) },
        { be_const_key_weak(area, -1), be_const_var(1) },
    })),
    be_str_weak(lv_signal_arcs)
);
/*******************************************************************/

void be_load_lv_signal_arcs_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_signal_arcs);
    be_setglobal(vm, "lv_signal_arcs");
    be_pop(vm, 1);
}

extern const bclass be_class_lv_wifi_arcs;

/********************************************************************
** Solidified function: before_del
********************************************************************/
be_local_closure(lv_wifi_arcs_before_del,   /* name */
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
    /* K1   */  be_nested_str_weak(remove_driver),
    }),
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
be_local_closure(lv_wifi_arcs_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(add_driver),
    /* K3   */  be_nested_str_weak(set_percentage),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0200,  //  0006  GETNGBL	R2	K1
      0x8C080502,  //  0007  GETMET	R2	R2	K2
      0x5C100000,  //  0008  MOVE	R4	R0
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080103,  //  000A  GETMET	R2	R0	K3
      0x58100004,  //  000B  LDCONST	R4	K4
      0x7C080400,  //  000C  CALL	R2	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(lv_wifi_arcs_every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(wifi),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(quality),
    /* K4   */  be_nested_str_weak(ip),
    /* K5   */  be_nested_str_weak(set_percentage),
    /* K6   */  be_const_int(0),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x58100003,  //  0004  LDCONST	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x8C0C0302,  //  0006  GETMET	R3	R1	K2
      0x58140004,  //  0007  LDCONST	R5	K4
      0x7C0C0400,  //  0008  CALL	R3	2
      0x4C100000,  //  0009  LDNIL	R4
      0x1C100604,  //  000A  EQ	R4	R3	R4
      0x78120003,  //  000B  JMPF	R4	#0010
      0x8C100105,  //  000C  GETMET	R4	R0	K5
      0x58180006,  //  000D  LDCONST	R6	K6
      0x7C100400,  //  000E  CALL	R4	2
      0x70020005,  //  000F  JMP		#0016
      0x4C100000,  //  0010  LDNIL	R4
      0x20100404,  //  0011  NE	R4	R2	R4
      0x78120002,  //  0012  JMPF	R4	#0016
      0x8C100105,  //  0013  GETMET	R4	R0	K5
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
        { be_const_key_weak(before_del, 1), be_const_closure(lv_wifi_arcs_before_del_closure) },
        { be_const_key_weak(every_second, 2), be_const_closure(lv_wifi_arcs_every_second_closure) },
        { be_const_key_weak(init, -1), be_const_closure(lv_wifi_arcs_init_closure) },
    })),
    be_str_weak(lv_wifi_arcs)
);
/*******************************************************************/

void be_load_lv_wifi_arcs_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_wifi_arcs);
    be_setglobal(vm, "lv_wifi_arcs");
    be_pop(vm, 1);
}

extern const bclass be_class_lv_wifi_arcs_icon;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_wifi_arcs_icon_init,   /* name */
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
        { be_const_key_weak(init, -1), be_const_closure(lv_wifi_arcs_icon_init_closure) },
    })),
    be_str_weak(lv_wifi_arcs_icon)
);
/*******************************************************************/

void be_load_lv_wifi_arcs_icon_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_wifi_arcs_icon);
    be_setglobal(vm, "lv_wifi_arcs_icon");
    be_pop(vm, 1);
}

extern const bclass be_class_lv_signal_bars;

/********************************************************************
** Solidified function: set_percentage
********************************************************************/
be_local_closure(lv_signal_bars_set_percentage,   /* name */
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
    /* K0   */  be_nested_str_weak(percentage),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(invalidate),
    }),
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
be_local_closure(lv_signal_bars_get_percentage,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(percentage),
    }),
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
be_local_closure(lv_signal_bars_widget_event,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
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
    ( &(const bvalue[35]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(obj_event_base),
    /* K2   */  be_nested_str_weak(RES_OK),
    /* K3   */  be_nested_str_weak(get_code),
    /* K4   */  be_nested_str_weak(get_height),
    /* K5   */  be_nested_str_weak(get_width),
    /* K6   */  be_const_int(3),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(EVENT_DRAW_MAIN),
    /* K9   */  be_nested_str_weak(line_dsc),
    /* K10  */  be_nested_str_weak(get_param),
    /* K11  */  be_nested_str_weak(get_layer),
    /* K12  */  be_nested_str_weak(get_coords),
    /* K13  */  be_nested_str_weak(area),
    /* K14  */  be_nested_str_weak(x1),
    /* K15  */  be_nested_str_weak(y1),
    /* K16  */  be_nested_str_weak(draw_line_dsc_init),
    /* K17  */  be_nested_str_weak(init_draw_line_dsc),
    /* K18  */  be_nested_str_weak(PART_MAIN),
    /* K19  */  be_nested_str_weak(round_start),
    /* K20  */  be_const_int(1),
    /* K21  */  be_nested_str_weak(round_end),
    /* K22  */  be_nested_str_weak(width),
    /* K23  */  be_nested_str_weak(get_style_line_color),
    /* K24  */  be_nested_str_weak(STATE_DEFAULT),
    /* K25  */  be_nested_str_weak(get_style_bg_color),
    /* K26  */  be_const_int(0),
    /* K27  */  be_nested_str_weak(color),
    /* K28  */  be_nested_str_weak(percentage),
    /* K29  */  be_nested_str_weak(p1_y),
    /* K30  */  be_nested_str_weak(p1_x),
    /* K31  */  be_nested_str_weak(p2_y),
    /* K32  */  be_nested_str_weak(p2_x),
    /* K33  */  be_nested_str_weak(draw_line),
    /* K34  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(widget_event),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x7C0C0600,  //  0004  CALL	R3	3
      0xB8120000,  //  0005  GETNGBL	R4	K0
      0x88100902,  //  0006  GETMBR	R4	R4	K2
      0x200C0604,  //  0007  NE	R3	R3	R4
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x80000600,  //  0009  RET	0
      0x8C0C0503,  //  000A  GETMET	R3	R2	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x84100000,  //  000C  CLOSURE	R4	P0
      0x8C140104,  //  000D  GETMET	R5	R0	K4
      0x7C140200,  //  000E  CALL	R5	1
      0x8C180105,  //  000F  GETMET	R6	R0	K5
      0x7C180200,  //  0010  CALL	R6	1
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x5422000E,  //  0012  LDINT	R8	15
      0x0C200C08,  //  0013  DIV	R8	R6	R8
      0x7C1C0200,  //  0014  CALL	R7	1
      0x5C200800,  //  0015  MOVE	R8	R4
      0x08240F06,  //  0016  MUL	R9	R7	K6
      0x04240C09,  //  0017  SUB	R9	R6	R9
      0x542A0003,  //  0018  LDINT	R10	4
      0x0C24120A,  //  0019  DIV	R9	R9	R10
      0x7C200200,  //  001A  CALL	R8	1
      0x0C241107,  //  001B  DIV	R9	R8	K7
      0xB82A0000,  //  001C  GETNGBL	R10	K0
      0x88281508,  //  001D  GETMBR	R10	R10	K8
      0x1C28060A,  //  001E  EQ	R10	R3	R10
      0x782A0051,  //  001F  JMPF	R10	#0072
      0x88280109,  //  0020  GETMBR	R10	R0	K9
      0x8C2C050A,  //  0021  GETMET	R11	R2	K10
      0x7C2C0200,  //  0022  CALL	R11	1
      0x8C30050B,  //  0023  GETMET	R12	R2	K11
      0x7C300200,  //  0024  CALL	R12	1
      0x8C34010C,  //  0025  GETMET	R13	R0	K12
      0x883C010D,  //  0026  GETMBR	R15	R0	K13
      0x7C340400,  //  0027  CALL	R13	2
      0x8834010D,  //  0028  GETMBR	R13	R0	K13
      0x88341B0E,  //  0029  GETMBR	R13	R13	K14
      0x8838010D,  //  002A  GETMBR	R14	R0	K13
      0x88381D0F,  //  002B  GETMBR	R14	R14	K15
      0xB83E0000,  //  002C  GETNGBL	R15	K0
      0x8C3C1F10,  //  002D  GETMET	R15	R15	K16
      0x5C441400,  //  002E  MOVE	R17	R10
      0x7C3C0400,  //  002F  CALL	R15	2
      0x8C3C0111,  //  0030  GETMET	R15	R0	K17
      0xB8460000,  //  0031  GETNGBL	R17	K0
      0x88442312,  //  0032  GETMBR	R17	R17	K18
      0x5C481400,  //  0033  MOVE	R18	R10
      0x7C3C0600,  //  0034  CALL	R15	3
      0x902A2714,  //  0035  SETMBR	R10	K19	K20
      0x902A2B14,  //  0036  SETMBR	R10	K21	K20
      0x902A2C08,  //  0037  SETMBR	R10	K22	R8
      0x8C3C0117,  //  0038  GETMET	R15	R0	K23
      0xB8460000,  //  0039  GETNGBL	R17	K0
      0x88442312,  //  003A  GETMBR	R17	R17	K18
      0xB84A0000,  //  003B  GETNGBL	R18	K0
      0x88482518,  //  003C  GETMBR	R18	R18	K24
      0x30442212,  //  003D  OR	R17	R17	R18
      0x7C3C0400,  //  003E  CALL	R15	2
      0x8C400119,  //  003F  GETMET	R16	R0	K25
      0xB84A0000,  //  0040  GETNGBL	R18	K0
      0x88482512,  //  0041  GETMBR	R18	R18	K18
      0xB84E0000,  //  0042  GETNGBL	R19	K0
      0x884C2718,  //  0043  GETMBR	R19	R19	K24
      0x30482413,  //  0044  OR	R18	R18	R19
      0x7C400400,  //  0045  CALL	R16	2
      0x60440010,  //  0046  GETGBL	R17	G16
      0x404A3506,  //  0047  CONNECT	R18	K26	K6
      0x7C440200,  //  0048  CALL	R17	1
      0xA8020024,  //  0049  EXBLK	0	#006F
      0x5C482200,  //  004A  MOVE	R18	R17
      0x7C480000,  //  004B  CALL	R18	0
      0x884C011C,  //  004C  GETMBR	R19	R0	K28
      0x00502514,  //  004D  ADD	R20	R18	K20
      0x54560013,  //  004E  LDINT	R21	20
      0x08502815,  //  004F  MUL	R20	R20	R21
      0x284C2614,  //  0050  GE	R19	R19	R20
      0x784E0001,  //  0051  JMPF	R19	#0054
      0x5C4C1E00,  //  0052  MOVE	R19	R15
      0x70020000,  //  0053  JMP		#0055
      0x5C4C2000,  //  0054  MOVE	R19	R16
      0x902A3613,  //  0055  SETMBR	R10	K27	R19
      0x004C1C05,  //  0056  ADD	R19	R14	R5
      0x044C2714,  //  0057  SUB	R19	R19	K20
      0x044C2609,  //  0058  SUB	R19	R19	R9
      0x902A3A13,  //  0059  SETMBR	R10	K29	R19
      0x004C1007,  //  005A  ADD	R19	R8	R7
      0x084C2413,  //  005B  MUL	R19	R18	R19
      0x004C1A13,  //  005C  ADD	R19	R13	R19
      0x004C2609,  //  005D  ADD	R19	R19	R9
      0x902A3C13,  //  005E  SETMBR	R10	K30	R19
      0x044E0C12,  //  005F  SUB	R19	K6	R18
      0x04500A08,  //  0060  SUB	R20	R5	R8
      0x084C2614,  //  0061  MUL	R19	R19	R20
      0x54520003,  //  0062  LDINT	R20	4
      0x0C4C2614,  //  0063  DIV	R19	R19	R20
      0x004C1C13,  //  0064  ADD	R19	R14	R19
      0x004C2609,  //  0065  ADD	R19	R19	R9
      0x902A3E13,  //  0066  SETMBR	R10	K31	R19
      0x884C151E,  //  0067  GETMBR	R19	R10	K30
      0x902A4013,  //  0068  SETMBR	R10	K32	R19
      0xB84E0000,  //  0069  GETNGBL	R19	K0
      0x8C4C2721,  //  006A  GETMET	R19	R19	K33
      0x5C541800,  //  006B  MOVE	R21	R12
      0x5C581400,  //  006C  MOVE	R22	R10
      0x7C4C0600,  //  006D  CALL	R19	3
      0x7001FFDA,  //  006E  JMP		#004A
      0x58440022,  //  006F  LDCONST	R17	K34
      0xAC440200,  //  0070  CATCH	R17	1	0
      0xB0080000,  //  0071  RAISE	2	R0	R0
      0x80000000,  //  0072  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_signal_bars_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lvgl),
    /* K1   */  be_nested_str_weak(create_custom_widget),
    /* K2   */  be_nested_str_weak(percentage),
    /* K3   */  be_nested_str_weak(area),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(line_dsc),
    /* K6   */  be_nested_str_weak(draw_line_dsc),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x540A0063,  //  0005  LDINT	R2	100
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0xB80A0800,  //  0007  GETNGBL	R2	K4
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x7C080200,  //  0009  CALL	R2	1
      0x90020602,  //  000A  SETMBR	R0	K3	R2
      0xB80A0800,  //  000B  GETNGBL	R2	K4
      0x8C080506,  //  000C  GETMET	R2	R2	K6
      0x7C080200,  //  000D  CALL	R2	1
      0x90020A02,  //  000E  SETMBR	R0	K5	R2
      0x80000000,  //  000F  RET	0
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
        { be_const_key_weak(set_percentage, 3), be_const_closure(lv_signal_bars_set_percentage_closure) },
        { be_const_key_weak(get_percentage, -1), be_const_closure(lv_signal_bars_get_percentage_closure) },
        { be_const_key_weak(percentage, -1), be_const_var(0) },
        { be_const_key_weak(area, 6), be_const_var(1) },
        { be_const_key_weak(line_dsc, 2), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(lv_signal_bars_init_closure) },
        { be_const_key_weak(widget_event, -1), be_const_closure(lv_signal_bars_widget_event_closure) },
    })),
    be_str_weak(lv_signal_bars)
);
/*******************************************************************/

void be_load_lv_signal_bars_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_signal_bars);
    be_setglobal(vm, "lv_signal_bars");
    be_pop(vm, 1);
}

extern const bclass be_class_lv_wifi_bars;

/********************************************************************
** Solidified function: before_del
********************************************************************/
be_local_closure(lv_wifi_bars_before_del,   /* name */
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
    /* K1   */  be_nested_str_weak(remove_driver),
    }),
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
be_local_closure(lv_wifi_bars_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(add_driver),
    /* K3   */  be_nested_str_weak(set_percentage),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0200,  //  0006  GETNGBL	R2	K1
      0x8C080502,  //  0007  GETMET	R2	R2	K2
      0x5C100000,  //  0008  MOVE	R4	R0
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080103,  //  000A  GETMET	R2	R0	K3
      0x58100004,  //  000B  LDCONST	R4	K4
      0x7C080400,  //  000C  CALL	R2	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(lv_wifi_bars_every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(wifi),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(quality),
    /* K4   */  be_nested_str_weak(ip),
    /* K5   */  be_nested_str_weak(set_percentage),
    /* K6   */  be_const_int(0),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x58100003,  //  0004  LDCONST	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x8C0C0302,  //  0006  GETMET	R3	R1	K2
      0x58140004,  //  0007  LDCONST	R5	K4
      0x7C0C0400,  //  0008  CALL	R3	2
      0x4C100000,  //  0009  LDNIL	R4
      0x1C100604,  //  000A  EQ	R4	R3	R4
      0x78120003,  //  000B  JMPF	R4	#0010
      0x8C100105,  //  000C  GETMET	R4	R0	K5
      0x58180006,  //  000D  LDCONST	R6	K6
      0x7C100400,  //  000E  CALL	R4	2
      0x70020005,  //  000F  JMP		#0016
      0x4C100000,  //  0010  LDNIL	R4
      0x20100404,  //  0011  NE	R4	R2	R4
      0x78120002,  //  0012  JMPF	R4	#0016
      0x8C100105,  //  0013  GETMET	R4	R0	K5
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
        { be_const_key_weak(before_del, 1), be_const_closure(lv_wifi_bars_before_del_closure) },
        { be_const_key_weak(every_second, 2), be_const_closure(lv_wifi_bars_every_second_closure) },
        { be_const_key_weak(init, -1), be_const_closure(lv_wifi_bars_init_closure) },
    })),
    be_str_weak(lv_wifi_bars)
);
/*******************************************************************/

void be_load_lv_wifi_bars_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_wifi_bars);
    be_setglobal(vm, "lv_wifi_bars");
    be_pop(vm, 1);
}

extern const bclass be_class_lv_wifi_bars_icon;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_wifi_bars_icon_init,   /* name */
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
        { be_const_key_weak(init, -1), be_const_closure(lv_wifi_bars_icon_init_closure) },
    })),
    be_str_weak(lv_wifi_bars_icon)
);
/*******************************************************************/

void be_load_lv_wifi_bars_icon_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_wifi_bars_icon);
    be_setglobal(vm, "lv_wifi_bars_icon");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
