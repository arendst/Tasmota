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
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lvgl),
    /* K1   */  be_nested_str_weak(create_custom_widget),
    /* K2   */  be_nested_str_weak(percentage),
    /* K3   */  be_nested_str_weak(p1),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(point),
    /* K6   */  be_nested_str_weak(p2),
    /* K7   */  be_nested_str_weak(area),
    /* K8   */  be_nested_str_weak(arc_dsc),
    /* K9   */  be_nested_str_weak(draw_arc_dsc),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x540A0063,  //  0005  LDINT	R2	100
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0xB80A0800,  //  0007  GETNGBL	R2	K4
      0x8C080505,  //  0008  GETMET	R2	R2	K5
      0x7C080200,  //  0009  CALL	R2	1
      0x90020602,  //  000A  SETMBR	R0	K3	R2
      0xB80A0800,  //  000B  GETNGBL	R2	K4
      0x8C080505,  //  000C  GETMET	R2	R2	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x90020C02,  //  000E  SETMBR	R0	K6	R2
      0xB80A0800,  //  000F  GETNGBL	R2	K4
      0x8C080507,  //  0010  GETMET	R2	R2	K7
      0x7C080200,  //  0011  CALL	R2	1
      0x90020E02,  //  0012  SETMBR	R0	K7	R2
      0xB80A0800,  //  0013  GETNGBL	R2	K4
      0x8C080509,  //  0014  GETMET	R2	R2	K9
      0x7C080200,  //  0015  CALL	R2	1
      0x90021002,  //  0016  SETMBR	R0	K8	R2
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_event
********************************************************************/
be_local_closure(lv_signal_arcs_widget_event,   /* name */
  be_nested_proto(
    27,                          /* nstack */
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
    /* K3   */  be_nested_str_weak(code),
    /* K4   */  be_nested_str_weak(math),
    /* K5   */  be_nested_str_weak(get_height),
    /* K6   */  be_nested_str_weak(get_width),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(EVENT_DRAW_MAIN),
    /* K10  */  be_nested_str_weak(draw_ctx),
    /* K11  */  be_nested_str_weak(param),
    /* K12  */  be_nested_str_weak(get_coords),
    /* K13  */  be_nested_str_weak(area),
    /* K14  */  be_nested_str_weak(x1),
    /* K15  */  be_nested_str_weak(y1),
    /* K16  */  be_nested_str_weak(draw_arc_dsc_init),
    /* K17  */  be_nested_str_weak(arc_dsc),
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
    /* K28  */  be_nested_str_weak(p1),
    /* K29  */  be_nested_str_weak(x),
    /* K30  */  be_nested_str_weak(y),
    /* K31  */  be_nested_str_weak(color),
    /* K32  */  be_nested_str_weak(percentage),
    /* K33  */  be_nested_str_weak(draw_arc),
    /* K34  */  be_const_int(0),
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
      0x880C0503,  //  000A  GETMBR	R3	R2	K3
      0xA4120800,  //  000B  IMPORT	R4	K4
      0x84140000,  //  000C  CLOSURE	R5	P0
      0x8C180105,  //  000D  GETMET	R6	R0	K5
      0x7C180200,  //  000E  CALL	R6	1
      0x8C1C0106,  //  000F  GETMET	R7	R0	K6
      0x7C1C0200,  //  0010  CALL	R7	1
      0x5C200A00,  //  0011  MOVE	R8	R5
      0x54260007,  //  0012  LDINT	R9	8
      0x0C240C09,  //  0013  DIV	R9	R6	R9
      0x7C200200,  //  0014  CALL	R8	1
      0x5C240A00,  //  0015  MOVE	R9	R5
      0x08281107,  //  0016  MUL	R10	R8	K7
      0x04280C0A,  //  0017  SUB	R10	R6	R10
      0x0C281508,  //  0018  DIV	R10	R10	K8
      0x7C240200,  //  0019  CALL	R9	1
      0x0C281307,  //  001A  DIV	R10	R9	K7
      0xB82E0000,  //  001B  GETNGBL	R11	K0
      0x882C1709,  //  001C  GETMBR	R11	R11	K9
      0x1C2C060B,  //  001D  EQ	R11	R3	R11
      0x782E008A,  //  001E  JMPF	R11	#00AA
      0xB82E0000,  //  001F  GETNGBL	R11	K0
      0x8C2C170A,  //  0020  GETMET	R11	R11	K10
      0x8834050B,  //  0021  GETMBR	R13	R2	K11
      0x7C2C0400,  //  0022  CALL	R11	2
      0x8C30010C,  //  0023  GETMET	R12	R0	K12
      0x8838010D,  //  0024  GETMBR	R14	R0	K13
      0x7C300400,  //  0025  CALL	R12	2
      0x8830010D,  //  0026  GETMBR	R12	R0	K13
      0x8830190E,  //  0027  GETMBR	R12	R12	K14
      0x8834010D,  //  0028  GETMBR	R13	R0	K13
      0x88341B0F,  //  0029  GETMBR	R13	R13	K15
      0xB83A0000,  //  002A  GETNGBL	R14	K0
      0x8C381D10,  //  002B  GETMET	R14	R14	K16
      0x88400111,  //  002C  GETMBR	R16	R0	K17
      0x7C380400,  //  002D  CALL	R14	2
      0x8C380112,  //  002E  GETMET	R14	R0	K18
      0xB8420000,  //  002F  GETNGBL	R16	K0
      0x88402113,  //  0030  GETMBR	R16	R16	K19
      0x88440111,  //  0031  GETMBR	R17	R0	K17
      0x7C380600,  //  0032  CALL	R14	3
      0x88380111,  //  0033  GETMBR	R14	R0	K17
      0x903A2915,  //  0034  SETMBR	R14	K20	K21
      0x88380111,  //  0035  GETMBR	R14	R0	K17
      0x083C1308,  //  0036  MUL	R15	R9	K8
      0x003C1F15,  //  0037  ADD	R15	R15	K21
      0x54420003,  //  0038  LDINT	R16	4
      0x0C3C1E10,  //  0039  DIV	R15	R15	R16
      0x903A2C0F,  //  003A  SETMBR	R14	K22	R15
      0x8C380117,  //  003B  GETMET	R14	R0	K23
      0xB8420000,  //  003C  GETNGBL	R16	K0
      0x88402113,  //  003D  GETMBR	R16	R16	K19
      0xB8460000,  //  003E  GETNGBL	R17	K0
      0x88442318,  //  003F  GETMBR	R17	R17	K24
      0x30402011,  //  0040  OR	R16	R16	R17
      0x7C380400,  //  0041  CALL	R14	2
      0x8C3C0119,  //  0042  GETMET	R15	R0	K25
      0xB8460000,  //  0043  GETNGBL	R17	K0
      0x88442313,  //  0044  GETMBR	R17	R17	K19
      0xB84A0000,  //  0045  GETNGBL	R18	K0
      0x88482518,  //  0046  GETMBR	R18	R18	K24
      0x30442212,  //  0047  OR	R17	R17	R18
      0x7C3C0400,  //  0048  CALL	R15	2
      0x04400C09,  //  0049  SUB	R16	R6	R9
      0x0C440F07,  //  004A  DIV	R17	R7	K7
      0x0444220A,  //  004B  SUB	R17	R17	R10
      0x60480009,  //  004C  GETGBL	R18	G9
      0x544E0059,  //  004D  LDINT	R19	90
      0x8C50091A,  //  004E  GETMET	R20	R4	K26
      0x8C58091B,  //  004F  GETMET	R22	R4	K27
      0x6060000A,  //  0050  GETGBL	R24	G10
      0x5C642200,  //  0051  MOVE	R25	R17
      0x7C600200,  //  0052  CALL	R24	1
      0x6064000A,  //  0053  GETGBL	R25	G10
      0x5C682000,  //  0054  MOVE	R26	R16
      0x7C640200,  //  0055  CALL	R25	1
      0x0C603019,  //  0056  DIV	R24	R24	R25
      0x7C580400,  //  0057  CALL	R22	2
      0x7C500400,  //  0058  CALL	R20	2
      0x044C2614,  //  0059  SUB	R19	R19	R20
      0x7C480200,  //  005A  CALL	R18	1
      0x544E002C,  //  005B  LDINT	R19	45
      0x244C2413,  //  005C  GT	R19	R18	R19
      0x784E0000,  //  005D  JMPF	R19	#005F
      0x544A002C,  //  005E  LDINT	R18	45
      0x884C011C,  //  005F  GETMBR	R19	R0	K28
      0x0C500F07,  //  0060  DIV	R20	R7	K7
      0x00501814,  //  0061  ADD	R20	R12	R20
      0x904E3A14,  //  0062  SETMBR	R19	K29	R20
      0x884C011C,  //  0063  GETMBR	R19	R0	K28
      0x00501A06,  //  0064  ADD	R20	R13	R6
      0x04502915,  //  0065  SUB	R20	R20	K21
      0x0450280A,  //  0066  SUB	R20	R20	R10
      0x904E3C14,  //  0067  SETMBR	R19	K30	R20
      0x884C0111,  //  0068  GETMBR	R19	R0	K17
      0x88500120,  //  0069  GETMBR	R20	R0	K32
      0x54560018,  //  006A  LDINT	R21	25
      0x28502815,  //  006B  GE	R20	R20	R21
      0x78520001,  //  006C  JMPF	R20	#006F
      0x5C501C00,  //  006D  MOVE	R20	R14
      0x70020000,  //  006E  JMP		#0070
      0x5C501E00,  //  006F  MOVE	R20	R15
      0x904E3E14,  //  0070  SETMBR	R19	K31	R20
      0xB84E0000,  //  0071  GETNGBL	R19	K0
      0x8C4C2721,  //  0072  GETMET	R19	R19	K33
      0x5C541600,  //  0073  MOVE	R21	R11
      0x88580111,  //  0074  GETMBR	R22	R0	K17
      0x885C011C,  //  0075  GETMBR	R23	R0	K28
      0x00601208,  //  0076  ADD	R24	R9	R8
      0x08624418,  //  0077  MUL	R24	K34	R24
      0x0060300A,  //  0078  ADD	R24	R24	R10
      0x58640022,  //  0079  LDCONST	R25	K34
      0x546A0167,  //  007A  LDINT	R26	360
      0x7C4C0E00,  //  007B  CALL	R19	7
      0x884C0111,  //  007C  GETMBR	R19	R0	K17
      0x88500120,  //  007D  GETMBR	R20	R0	K32
      0x54560031,  //  007E  LDINT	R21	50
      0x28502815,  //  007F  GE	R20	R20	R21
      0x78520001,  //  0080  JMPF	R20	#0083
      0x5C501C00,  //  0081  MOVE	R20	R14
      0x70020000,  //  0082  JMP		#0084
      0x5C501E00,  //  0083  MOVE	R20	R15
      0x904E3E14,  //  0084  SETMBR	R19	K31	R20
      0xB84E0000,  //  0085  GETNGBL	R19	K0
      0x8C4C2721,  //  0086  GETMET	R19	R19	K33
      0x5C541600,  //  0087  MOVE	R21	R11
      0x88580111,  //  0088  GETMBR	R22	R0	K17
      0x885C011C,  //  0089  GETMBR	R23	R0	K28
      0x00601208,  //  008A  ADD	R24	R9	R8
      0x08622A18,  //  008B  MUL	R24	K21	R24
      0x0060300A,  //  008C  ADD	R24	R24	R10
      0x04603115,  //  008D  SUB	R24	R24	K21
      0x5466010D,  //  008E  LDINT	R25	270
      0x04643212,  //  008F  SUB	R25	R25	R18
      0x546A010D,  //  0090  LDINT	R26	270
      0x00683412,  //  0091  ADD	R26	R26	R18
      0x7C4C0E00,  //  0092  CALL	R19	7
      0x884C0111,  //  0093  GETMBR	R19	R0	K17
      0x88500120,  //  0094  GETMBR	R20	R0	K32
      0x5456004A,  //  0095  LDINT	R21	75
      0x28502815,  //  0096  GE	R20	R20	R21
      0x78520001,  //  0097  JMPF	R20	#009A
      0x5C501C00,  //  0098  MOVE	R20	R14
      0x70020000,  //  0099  JMP		#009B
      0x5C501E00,  //  009A  MOVE	R20	R15
      0x904E3E14,  //  009B  SETMBR	R19	K31	R20
      0xB84E0000,  //  009C  GETNGBL	R19	K0
      0x8C4C2721,  //  009D  GETMET	R19	R19	K33
      0x5C541600,  //  009E  MOVE	R21	R11
      0x88580111,  //  009F  GETMBR	R22	R0	K17
      0x885C011C,  //  00A0  GETMBR	R23	R0	K28
      0x00601208,  //  00A1  ADD	R24	R9	R8
      0x08620E18,  //  00A2  MUL	R24	K7	R24
      0x0060300A,  //  00A3  ADD	R24	R24	R10
      0x04603107,  //  00A4  SUB	R24	R24	K7
      0x5466010D,  //  00A5  LDINT	R25	270
      0x04643212,  //  00A6  SUB	R25	R25	R18
      0x546A010D,  //  00A7  LDINT	R26	270
      0x00683412,  //  00A8  ADD	R26	R26	R18
      0x7C4C0E00,  //  00A9  CALL	R19	7
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
** Solidified class: lv_signal_arcs
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_arcs,
    5,
    &be_class_lv_obj,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(percentage, -1), be_const_var(0) },
        { be_const_key_weak(area, 4), be_const_var(3) },
        { be_const_key_weak(p2, -1), be_const_var(2) },
        { be_const_key_weak(get_percentage, -1), be_const_closure(lv_signal_arcs_get_percentage_closure) },
        { be_const_key_weak(p1, -1), be_const_var(1) },
        { be_const_key_weak(arc_dsc, -1), be_const_var(4) },
        { be_const_key_weak(init, 5), be_const_closure(lv_signal_arcs_init_closure) },
        { be_const_key_weak(widget_event, -1), be_const_closure(lv_signal_arcs_widget_event_closure) },
        { be_const_key_weak(set_percentage, 3), be_const_closure(lv_signal_arcs_set_percentage_closure) },
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
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(_lvgl),
    /* K1   */  be_nested_str_weak(create_custom_widget),
    /* K2   */  be_nested_str_weak(percentage),
    /* K3   */  be_nested_str_weak(p1),
    /* K4   */  be_nested_str_weak(lv),
    /* K5   */  be_nested_str_weak(point),
    /* K6   */  be_nested_str_weak(p2),
    /* K7   */  be_nested_str_weak(area),
    /* K8   */  be_nested_str_weak(line_dsc),
    /* K9   */  be_nested_str_weak(draw_line_dsc),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x540A0063,  //  0005  LDINT	R2	100
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0xB80A0800,  //  0007  GETNGBL	R2	K4
      0x8C080505,  //  0008  GETMET	R2	R2	K5
      0x7C080200,  //  0009  CALL	R2	1
      0x90020602,  //  000A  SETMBR	R0	K3	R2
      0xB80A0800,  //  000B  GETNGBL	R2	K4
      0x8C080505,  //  000C  GETMET	R2	R2	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x90020C02,  //  000E  SETMBR	R0	K6	R2
      0xB80A0800,  //  000F  GETNGBL	R2	K4
      0x8C080507,  //  0010  GETMET	R2	R2	K7
      0x7C080200,  //  0011  CALL	R2	1
      0x90020E02,  //  0012  SETMBR	R0	K7	R2
      0xB80A0800,  //  0013  GETNGBL	R2	K4
      0x8C080509,  //  0014  GETMET	R2	R2	K9
      0x7C080200,  //  0015  CALL	R2	1
      0x90021002,  //  0016  SETMBR	R0	K8	R2
      0x80000000,  //  0017  RET	0
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
    ( &(const bvalue[38]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(obj_event_base),
    /* K2   */  be_nested_str_weak(RES_OK),
    /* K3   */  be_nested_str_weak(code),
    /* K4   */  be_nested_str_weak(get_height),
    /* K5   */  be_nested_str_weak(get_width),
    /* K6   */  be_const_int(3),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(EVENT_DRAW_MAIN),
    /* K9   */  be_nested_str_weak(draw_ctx),
    /* K10  */  be_nested_str_weak(param),
    /* K11  */  be_nested_str_weak(get_coords),
    /* K12  */  be_nested_str_weak(area),
    /* K13  */  be_nested_str_weak(x1),
    /* K14  */  be_nested_str_weak(y1),
    /* K15  */  be_nested_str_weak(draw_line_dsc_init),
    /* K16  */  be_nested_str_weak(line_dsc),
    /* K17  */  be_nested_str_weak(init_draw_line_dsc),
    /* K18  */  be_nested_str_weak(PART_MAIN),
    /* K19  */  be_nested_str_weak(round_start),
    /* K20  */  be_const_int(1),
    /* K21  */  be_nested_str_weak(round_end),
    /* K22  */  be_nested_str_weak(width),
    /* K23  */  be_nested_str_weak(get_style_line_color),
    /* K24  */  be_nested_str_weak(STATE_DEFAULT),
    /* K25  */  be_nested_str_weak(get_style_bg_color),
    /* K26  */  be_nested_str_weak(event_send),
    /* K27  */  be_nested_str_weak(EVENT_DRAW_PART_BEGIN),
    /* K28  */  be_const_int(0),
    /* K29  */  be_nested_str_weak(color),
    /* K30  */  be_nested_str_weak(percentage),
    /* K31  */  be_nested_str_weak(p1),
    /* K32  */  be_nested_str_weak(y),
    /* K33  */  be_nested_str_weak(x),
    /* K34  */  be_nested_str_weak(p2),
    /* K35  */  be_nested_str_weak(draw_line),
    /* K36  */  be_nested_str_weak(stop_iteration),
    /* K37  */  be_nested_str_weak(EVENT_DRAW_PART_END),
    }),
    be_str_weak(widget_event),
    &be_const_str_solidified,
    ( &(const binstruction[138]) {  /* code */
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
      0x880C0503,  //  000A  GETMBR	R3	R2	K3
      0x84100000,  //  000B  CLOSURE	R4	P0
      0x8C140104,  //  000C  GETMET	R5	R0	K4
      0x7C140200,  //  000D  CALL	R5	1
      0x8C180105,  //  000E  GETMET	R6	R0	K5
      0x7C180200,  //  000F  CALL	R6	1
      0x5C1C0800,  //  0010  MOVE	R7	R4
      0x5422000E,  //  0011  LDINT	R8	15
      0x0C200C08,  //  0012  DIV	R8	R6	R8
      0x7C1C0200,  //  0013  CALL	R7	1
      0x5C200800,  //  0014  MOVE	R8	R4
      0x08240F06,  //  0015  MUL	R9	R7	K6
      0x04240C09,  //  0016  SUB	R9	R6	R9
      0x542A0003,  //  0017  LDINT	R10	4
      0x0C24120A,  //  0018  DIV	R9	R9	R10
      0x7C200200,  //  0019  CALL	R8	1
      0x0C241107,  //  001A  DIV	R9	R8	K7
      0xB82A0000,  //  001B  GETNGBL	R10	K0
      0x88281508,  //  001C  GETMBR	R10	R10	K8
      0x1C28060A,  //  001D  EQ	R10	R3	R10
      0x782A0069,  //  001E  JMPF	R10	#0089
      0xB82A0000,  //  001F  GETNGBL	R10	K0
      0x8C281509,  //  0020  GETMET	R10	R10	K9
      0x8830050A,  //  0021  GETMBR	R12	R2	K10
      0x7C280400,  //  0022  CALL	R10	2
      0x8C2C010B,  //  0023  GETMET	R11	R0	K11
      0x8834010C,  //  0024  GETMBR	R13	R0	K12
      0x7C2C0400,  //  0025  CALL	R11	2
      0x882C010C,  //  0026  GETMBR	R11	R0	K12
      0x882C170D,  //  0027  GETMBR	R11	R11	K13
      0x8830010C,  //  0028  GETMBR	R12	R0	K12
      0x8830190E,  //  0029  GETMBR	R12	R12	K14
      0xB8360000,  //  002A  GETNGBL	R13	K0
      0x8C341B0F,  //  002B  GETMET	R13	R13	K15
      0x883C0110,  //  002C  GETMBR	R15	R0	K16
      0x7C340400,  //  002D  CALL	R13	2
      0x8C340111,  //  002E  GETMET	R13	R0	K17
      0xB83E0000,  //  002F  GETNGBL	R15	K0
      0x883C1F12,  //  0030  GETMBR	R15	R15	K18
      0x88400110,  //  0031  GETMBR	R16	R0	K16
      0x7C340600,  //  0032  CALL	R13	3
      0x88340110,  //  0033  GETMBR	R13	R0	K16
      0x90362714,  //  0034  SETMBR	R13	K19	K20
      0x88340110,  //  0035  GETMBR	R13	R0	K16
      0x90362B14,  //  0036  SETMBR	R13	K21	K20
      0x88340110,  //  0037  GETMBR	R13	R0	K16
      0x90362C08,  //  0038  SETMBR	R13	K22	R8
      0x8C340117,  //  0039  GETMET	R13	R0	K23
      0xB83E0000,  //  003A  GETNGBL	R15	K0
      0x883C1F12,  //  003B  GETMBR	R15	R15	K18
      0xB8420000,  //  003C  GETNGBL	R16	K0
      0x88402118,  //  003D  GETMBR	R16	R16	K24
      0x303C1E10,  //  003E  OR	R15	R15	R16
      0x7C340400,  //  003F  CALL	R13	2
      0x8C380119,  //  0040  GETMET	R14	R0	K25
      0xB8420000,  //  0041  GETNGBL	R16	K0
      0x88402112,  //  0042  GETMBR	R16	R16	K18
      0xB8460000,  //  0043  GETNGBL	R17	K0
      0x88442318,  //  0044  GETMBR	R17	R17	K24
      0x30402011,  //  0045  OR	R16	R16	R17
      0x7C380400,  //  0046  CALL	R14	2
      0xB83E0000,  //  0047  GETNGBL	R15	K0
      0x8C3C1F1A,  //  0048  GETMET	R15	R15	K26
      0x5C440000,  //  0049  MOVE	R17	R0
      0xB84A0000,  //  004A  GETNGBL	R18	K0
      0x8848251B,  //  004B  GETMBR	R18	R18	K27
      0x884C0110,  //  004C  GETMBR	R19	R0	K16
      0x7C3C0800,  //  004D  CALL	R15	4
      0x603C0010,  //  004E  GETGBL	R15	G16
      0x40423906,  //  004F  CONNECT	R16	K28	K6
      0x7C3C0200,  //  0050  CALL	R15	1
      0xA802002C,  //  0051  EXBLK	0	#007F
      0x5C401E00,  //  0052  MOVE	R16	R15
      0x7C400000,  //  0053  CALL	R16	0
      0x88440110,  //  0054  GETMBR	R17	R0	K16
      0x8848011E,  //  0055  GETMBR	R18	R0	K30
      0x004C2114,  //  0056  ADD	R19	R16	K20
      0x54520013,  //  0057  LDINT	R20	20
      0x084C2614,  //  0058  MUL	R19	R19	R20
      0x28482413,  //  0059  GE	R18	R18	R19
      0x784A0001,  //  005A  JMPF	R18	#005D
      0x5C481A00,  //  005B  MOVE	R18	R13
      0x70020000,  //  005C  JMP		#005E
      0x5C481C00,  //  005D  MOVE	R18	R14
      0x90463A12,  //  005E  SETMBR	R17	K29	R18
      0x8844011F,  //  005F  GETMBR	R17	R0	K31
      0x00481805,  //  0060  ADD	R18	R12	R5
      0x04482514,  //  0061  SUB	R18	R18	K20
      0x04482409,  //  0062  SUB	R18	R18	R9
      0x90464012,  //  0063  SETMBR	R17	K32	R18
      0x8844011F,  //  0064  GETMBR	R17	R0	K31
      0x00481007,  //  0065  ADD	R18	R8	R7
      0x08482012,  //  0066  MUL	R18	R16	R18
      0x00481612,  //  0067  ADD	R18	R11	R18
      0x00482409,  //  0068  ADD	R18	R18	R9
      0x90464212,  //  0069  SETMBR	R17	K33	R18
      0x88440122,  //  006A  GETMBR	R17	R0	K34
      0x044A0C10,  //  006B  SUB	R18	K6	R16
      0x044C0A08,  //  006C  SUB	R19	R5	R8
      0x08482413,  //  006D  MUL	R18	R18	R19
      0x544E0003,  //  006E  LDINT	R19	4
      0x0C482413,  //  006F  DIV	R18	R18	R19
      0x00481812,  //  0070  ADD	R18	R12	R18
      0x00482409,  //  0071  ADD	R18	R18	R9
      0x90464012,  //  0072  SETMBR	R17	K32	R18
      0x88440122,  //  0073  GETMBR	R17	R0	K34
      0x8848011F,  //  0074  GETMBR	R18	R0	K31
      0x88482521,  //  0075  GETMBR	R18	R18	K33
      0x90464212,  //  0076  SETMBR	R17	K33	R18
      0xB8460000,  //  0077  GETNGBL	R17	K0
      0x8C442323,  //  0078  GETMET	R17	R17	K35
      0x5C4C1400,  //  0079  MOVE	R19	R10
      0x88500110,  //  007A  GETMBR	R20	R0	K16
      0x8854011F,  //  007B  GETMBR	R21	R0	K31
      0x88580122,  //  007C  GETMBR	R22	R0	K34
      0x7C440A00,  //  007D  CALL	R17	5
      0x7001FFD2,  //  007E  JMP		#0052
      0x583C0024,  //  007F  LDCONST	R15	K36
      0xAC3C0200,  //  0080  CATCH	R15	1	0
      0xB0080000,  //  0081  RAISE	2	R0	R0
      0xB83E0000,  //  0082  GETNGBL	R15	K0
      0x8C3C1F1A,  //  0083  GETMET	R15	R15	K26
      0x5C440000,  //  0084  MOVE	R17	R0
      0xB84A0000,  //  0085  GETNGBL	R18	K0
      0x88482525,  //  0086  GETMBR	R18	R18	K37
      0x884C0110,  //  0087  GETMBR	R19	R0	K16
      0x7C3C0800,  //  0088  CALL	R15	4
      0x80000000,  //  0089  RET	0
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
** Solidified class: lv_signal_bars
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_bars,
    5,
    &be_class_lv_obj,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(percentage, 4), be_const_var(0) },
        { be_const_key_weak(p1, 5), be_const_var(1) },
        { be_const_key_weak(p2, -1), be_const_var(2) },
        { be_const_key_weak(get_percentage, -1), be_const_closure(lv_signal_bars_get_percentage_closure) },
        { be_const_key_weak(line_dsc, -1), be_const_var(4) },
        { be_const_key_weak(area, -1), be_const_var(3) },
        { be_const_key_weak(init, -1), be_const_closure(lv_signal_bars_init_closure) },
        { be_const_key_weak(widget_event, -1), be_const_closure(lv_signal_bars_widget_event_closure) },
        { be_const_key_weak(set_percentage, 3), be_const_closure(lv_signal_bars_set_percentage_closure) },
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
