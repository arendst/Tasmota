/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: set_time
********************************************************************/
be_local_closure(lv_clock_icon_set_time,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(hour),
    /* K2   */  be_nested_str(minute),
    /* K3   */  be_nested_str(sec),
    /* K4   */  be_nested_str(format),
    /* K5   */  be_nested_str(_X2502d_X25s_X2502d),
    /* K6   */  be_const_int(2),
    /* K7   */  be_nested_str(_X3A),
    /* K8   */  be_nested_str(_X20),
    /* K9   */  be_nested_str(set_text),
    }),
    &be_const_str_set_time,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x88140101,  //  0001  GETMBR	R5	R0	K1
      0x20140205,  //  0002  NE	R5	R1	R5
      0x74160005,  //  0003  JMPT	R5	#000A
      0x88140102,  //  0004  GETMBR	R5	R0	K2
      0x20140405,  //  0005  NE	R5	R2	R5
      0x74160002,  //  0006  JMPT	R5	#000A
      0x88140103,  //  0007  GETMBR	R5	R0	K3
      0x20140605,  //  0008  NE	R5	R3	R5
      0x7816000F,  //  0009  JMPF	R5	#001A
      0x8C140904,  //  000A  GETMET	R5	R4	K4
      0x581C0005,  //  000B  LDCONST	R7	K5
      0x5C200200,  //  000C  MOVE	R8	R1
      0x10240706,  //  000D  MOD	R9	R3	K6
      0x78260001,  //  000E  JMPF	R9	#0011
      0x58240007,  //  000F  LDCONST	R9	K7
      0x70020000,  //  0010  JMP		#0012
      0x58240008,  //  0011  LDCONST	R9	K8
      0x5C280400,  //  0012  MOVE	R10	R2
      0x7C140A00,  //  0013  CALL	R5	5
      0x90020201,  //  0014  SETMBR	R0	K1	R1
      0x90020402,  //  0015  SETMBR	R0	K2	R2
      0x90020603,  //  0016  SETMBR	R0	K3	R3
      0x8C180109,  //  0017  GETMET	R6	R0	K9
      0x5C200A00,  //  0018  MOVE	R8	R5
      0x7C180400,  //  0019  CALL	R6	2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(lv_clock_icon_every_second,   /* name */
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
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(time_dump),
    /* K2   */  be_nested_str(rtc),
    /* K3   */  be_nested_str(local),
    /* K4   */  be_nested_str(year),
    /* K5   */  be_nested_str(set_time),
    /* K6   */  be_nested_str(hour),
    /* K7   */  be_nested_str(min),
    /* K8   */  be_nested_str(sec),
    }),
    &be_const_str_every_second,
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
be_local_closure(lv_clock_icon_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
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
        /* K0   */  be_nested_str(before_del),
        }),
        &be_const_str__X3Clambda_X3E,
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
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str(init),
    /* K1   */  be_nested_str(lv),
    /* K2   */  be_nested_str(seg7_font),
    /* K3   */  be_nested_str(set_style_text_font),
    /* K4   */  be_nested_str(PART_MAIN),
    /* K5   */  be_nested_str(STATE_DEFAULT),
    /* K6   */  be_nested_str(get_height),
    /* K7   */  be_nested_str(set_text),
    /* K8   */  be_nested_str(_X2D_X2D_X3A_X2D_X2D),
    /* K9   */  be_nested_str(refr_size),
    /* K10  */  be_nested_str(get_width),
    /* K11  */  be_nested_str(set_y),
    /* K12  */  be_const_int(2),
    /* K13  */  be_nested_str(get_style_pad_right),
    /* K14  */  be_nested_str(set_x),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str(set_style_pad_right),
    /* K17  */  be_nested_str(set_style_bg_color),
    /* K18  */  be_nested_str(color),
    /* K19  */  be_nested_str(COLOR_BLACK),
    /* K20  */  be_nested_str(add_event_cb),
    /* K21  */  be_nested_str(EVENT_DELETE),
    /* K22  */  be_const_int(0),
    /* K23  */  be_nested_str(tasmota),
    /* K24  */  be_nested_str(add_driver),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[89]) {  /* code */
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
      0x4C0C0000,  //  0015  LDNIL	R3
      0x200C0203,  //  0016  NE	R3	R1	R3
      0x780E0034,  //  0017  JMPF	R3	#004D
      0x8C0C0306,  //  0018  GETMET	R3	R1	K6
      0x7C0C0200,  //  0019  CALL	R3	1
      0x8C100107,  //  001A  GETMET	R4	R0	K7
      0x58180008,  //  001B  LDCONST	R6	K8
      0x7C100400,  //  001C  CALL	R4	2
      0x8C100109,  //  001D  GETMET	R4	R0	K9
      0x7C100200,  //  001E  CALL	R4	1
      0x8C10010A,  //  001F  GETMET	R4	R0	K10
      0x7C100200,  //  0020  CALL	R4	1
      0x8C14010B,  //  0021  GETMET	R5	R0	K11
      0x8C1C0306,  //  0022  GETMET	R7	R1	K6
      0x7C1C0200,  //  0023  CALL	R7	1
      0x8C200106,  //  0024  GETMET	R8	R0	K6
      0x7C200200,  //  0025  CALL	R8	1
      0x041C0E08,  //  0026  SUB	R7	R7	R8
      0x0C1C0F0C,  //  0027  DIV	R7	R7	K12
      0x7C140400,  //  0028  CALL	R5	2
      0x8C14030D,  //  0029  GETMET	R5	R1	K13
      0xB81E0200,  //  002A  GETNGBL	R7	K1
      0x881C0F04,  //  002B  GETMBR	R7	R7	K4
      0xB8220200,  //  002C  GETNGBL	R8	K1
      0x88201105,  //  002D  GETMBR	R8	R8	K5
      0x301C0E08,  //  002E  OR	R7	R7	R8
      0x7C140400,  //  002F  CALL	R5	2
      0x8C18010E,  //  0030  GETMET	R6	R0	K14
      0x8C20030A,  //  0031  GETMET	R8	R1	K10
      0x7C200200,  //  0032  CALL	R8	1
      0x04201004,  //  0033  SUB	R8	R8	R4
      0x04201005,  //  0034  SUB	R8	R8	R5
      0x0420110F,  //  0035  SUB	R8	R8	K15
      0x7C180400,  //  0036  CALL	R6	2
      0x8C180310,  //  0037  GETMET	R6	R1	K16
      0x00200A04,  //  0038  ADD	R8	R5	R4
      0x54260005,  //  0039  LDINT	R9	6
      0x00201009,  //  003A  ADD	R8	R8	R9
      0xB8260200,  //  003B  GETNGBL	R9	K1
      0x88241304,  //  003C  GETMBR	R9	R9	K4
      0xB82A0200,  //  003D  GETNGBL	R10	K1
      0x88281505,  //  003E  GETMBR	R10	R10	K5
      0x3024120A,  //  003F  OR	R9	R9	R10
      0x7C180600,  //  0040  CALL	R6	3
      0x8C180111,  //  0041  GETMET	R6	R0	K17
      0xB8220200,  //  0042  GETNGBL	R8	K1
      0x8C201112,  //  0043  GETMET	R8	R8	K18
      0xB82A0200,  //  0044  GETNGBL	R10	K1
      0x88281513,  //  0045  GETMBR	R10	R10	K19
      0x7C200400,  //  0046  CALL	R8	2
      0xB8260200,  //  0047  GETNGBL	R9	K1
      0x88241304,  //  0048  GETMBR	R9	R9	K4
      0xB82A0200,  //  0049  GETNGBL	R10	K1
      0x88281505,  //  004A  GETMBR	R10	R10	K5
      0x3024120A,  //  004B  OR	R9	R9	R10
      0x7C180600,  //  004C  CALL	R6	3
      0x8C0C0114,  //  004D  GETMET	R3	R0	K20
      0x84140000,  //  004E  CLOSURE	R5	P0
      0xB81A0200,  //  004F  GETNGBL	R6	K1
      0x88180D15,  //  0050  GETMBR	R6	R6	K21
      0x581C0016,  //  0051  LDCONST	R7	K22
      0x7C0C0800,  //  0052  CALL	R3	4
      0xB80E2E00,  //  0053  GETNGBL	R3	K23
      0x8C0C0718,  //  0054  GETMET	R3	R3	K24
      0x5C140000,  //  0055  MOVE	R5	R0
      0x7C0C0400,  //  0056  CALL	R3	2
      0xA0000000,  //  0057  CLOSE	R0
      0x80000000,  //  0058  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: before_del
********************************************************************/
be_local_closure(lv_clock_icon_before_del,   /* name */
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
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(remove_driver),
    }),
    &be_const_str_before_del,
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
** Solidified class: lv_clock_icon
********************************************************************/
extern const bclass be_class_lv_label;
be_local_class(lv_clock_icon,
    3,
    &be_class_lv_label,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(sec, -1), be_const_var(2) },
        { be_const_key(hour, -1), be_const_var(0) },
        { be_const_key(before_del, 6), be_const_closure(lv_clock_icon_before_del_closure) },
        { be_const_key(every_second, -1), be_const_closure(lv_clock_icon_every_second_closure) },
        { be_const_key(minute, -1), be_const_var(1) },
        { be_const_key(set_time, 2), be_const_closure(lv_clock_icon_set_time_closure) },
        { be_const_key(init, -1), be_const_closure(lv_clock_icon_init_closure) },
    })),
    be_str_literal("lv_clock_icon")
);
/*******************************************************************/

void be_load_lv_clock_icon_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_clock_icon);
    be_setglobal(vm, "lv_clock_icon");
    be_pop(vm, 1);
}

#endif // USE_LVGL