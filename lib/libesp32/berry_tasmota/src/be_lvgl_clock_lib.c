/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: set_time
********************************************************************/
be_local_closure(lv_clock_set_time,   /* name */
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
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str(init),
    /* K1   */  be_nested_str(lv),
    /* K2   */  be_nested_str(seg7_font),
    /* K3   */  be_nested_str(set_style_text_font),
    /* K4   */  be_nested_str(PART_MAIN),
    /* K5   */  be_nested_str(STATE_DEFAULT),
    /* K6   */  be_nested_str(set_text),
    /* K7   */  be_nested_str(_X2D_X2D_X3A_X2D_X2D),
    /* K8   */  be_nested_str(add_event_cb),
    /* K9   */  be_nested_str(EVENT_DELETE),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str(tasmota),
    /* K12  */  be_nested_str(add_driver),
    }),
    &be_const_str_init,
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
** Solidified class: lv_clock
********************************************************************/
extern const bclass be_class_lv_label;
be_local_class(lv_clock,
    3,
    &be_class_lv_label,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(sec, -1), be_const_var(2) },
        { be_const_key(hour, -1), be_const_var(0) },
        { be_const_key(before_del, 6), be_const_closure(lv_clock_before_del_closure) },
        { be_const_key(every_second, -1), be_const_closure(lv_clock_every_second_closure) },
        { be_const_key(minute, -1), be_const_var(1) },
        { be_const_key(set_time, 2), be_const_closure(lv_clock_set_time_closure) },
        { be_const_key(init, -1), be_const_closure(lv_clock_init_closure) },
    })),
    be_str_literal("lv_clock")
);
/*******************************************************************/

void be_load_lv_clock_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_clock);
    be_setglobal(vm, "lv_clock");
    be_pop(vm, 1);
}

#endif // USE_LVGL