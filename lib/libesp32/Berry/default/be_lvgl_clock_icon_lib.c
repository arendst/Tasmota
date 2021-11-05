/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: set_time
********************************************************************/
be_local_closure(set_time,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_string("string", 398550328, 6),
    /* K1   */  be_nested_string("hour", -1241306097, 4),
    /* K2   */  be_nested_string("minute", 954666857, 6),
    /* K3   */  be_nested_string("sec", -1155074638, 3),
    /* K4   */  be_nested_string("format", -1180859054, 6),
    /* K5   */  be_nested_string("%02d%s%02d", 1587999717, 10),
    /* K6   */  be_const_int(2),
    /* K7   */  be_nested_string(":", 1057798253, 1),
    /* K8   */  be_nested_string(" ", 621580159, 1),
    /* K9   */  be_nested_string("set_text", 1849641155, 8),
    }),
    (be_nested_const_str("set_time", 900236405, 8)),
    ((bstring*) &be_const_str_input),
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
be_local_closure(every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_string("tasmota", 424643812, 7),
    /* K1   */  be_nested_string("time_dump", -964556549, 9),
    /* K2   */  be_nested_string("rtc", 1070575216, 3),
    /* K3   */  be_nested_string("local", -1673304312, 5),
    /* K4   */  be_nested_string("year", -1367388900, 4),
    /* K5   */  be_nested_string("set_time", 900236405, 8),
    /* K6   */  be_nested_string("hour", -1241306097, 4),
    /* K7   */  be_nested_string("min", -913357481, 3),
    /* K8   */  be_nested_string("sec", -1155074638, 3),
    }),
    (be_nested_const_str("every_second", 2075451465, 12)),
    ((bstring*) &be_const_str_input),
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
be_local_closure(init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_string("init", 380752755, 4),
    /* K1   */  be_nested_string("lv", 1529997255, 2),
    /* K2   */  be_nested_string("seg7_font", -195276607, 9),
    /* K3   */  be_nested_string("set_style_text_font", 1028590019, 19),
    /* K4   */  be_nested_string("PART_MAIN", -1821475788, 9),
    /* K5   */  be_nested_string("STATE_DEFAULT", 712406428, 13),
    /* K6   */  be_nested_string("get_height", -723211773, 10),
    /* K7   */  be_nested_string("set_text", 1849641155, 8),
    /* K8   */  be_nested_string("--:--", 1370615441, 5),
    /* K9   */  be_nested_string("refr_size", 1958144468, 9),
    /* K10  */  be_nested_string("get_width", -1001549996, 9),
    /* K11  */  be_nested_string("set_y", 1866178391, 5),
    /* K12  */  be_const_int(2),
    /* K13  */  be_nested_string("get_style_pad_right", -1144679830, 19),
    /* K14  */  be_nested_string("set_x", 1849400772, 5),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_string("set_style_pad_right", -980898242, 19),
    /* K17  */  be_nested_string("set_style_bg_color", 1689513089, 18),
    /* K18  */  be_nested_string("color", 1031692888, 5),
    /* K19  */  be_nested_string("COLOR_BLACK", 264427940, 11),
    /* K20  */  be_nested_string("tasmota", 424643812, 7),
    /* K21  */  be_nested_string("add_driver", 1654458371, 10),
    }),
    ((bstring*) &be_const_str_init),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[82]) {  /* code */
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
      0xB80E2800,  //  004D  GETNGBL	R3	K20
      0x8C0C0715,  //  004E  GETMET	R3	R3	K21
      0x5C140000,  //  004F  MOVE	R5	R0
      0x7C0C0400,  //  0050  CALL	R3	2
      0x80000000,  //  0051  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: del
********************************************************************/
be_local_closure(del,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("del", -816214454, 3),
    /* K1   */  be_nested_string("tasmota", 424643812, 7),
    /* K2   */  be_nested_string("remove_driver", 1030243768, 13),
    }),
    (be_nested_const_str("del", -816214454, 3)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[10]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0xB8060200,  //  0005  GETNGBL	R1	K1
      0x8C040302,  //  0006  GETMET	R1	R1	K2
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x7C040400,  //  0008  CALL	R1	2
      0x80000000,  //  0009  RET	0
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
        { be_nested_key("sec", -1155074638, 3, -1), be_const_var(2) },
        { be_nested_key("hour", -1241306097, 4, -1), be_const_var(0) },
        { be_nested_key("set_time", 900236405, 8, 6), be_const_closure(set_time_closure) },
        { be_nested_key("every_second", 2075451465, 12, -1), be_const_closure(every_second_closure) },
        { be_nested_key("minute", 954666857, 6, -1), be_const_var(1) },
        { be_nested_key("init", 380752755, 4, 2), be_const_closure(init_closure) },
        { be_nested_key("del", -816214454, 3, -1), be_const_closure(del_closure) },
    })),
    (be_nested_const_str("lv_clock_icon", -1037751086, 13))
);
/*******************************************************************/

void be_load_lv_clock_icon_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_clock_icon);
    be_setglobal(vm, "lv_clock_icon");
    be_pop(vm, 1);
}

#endif // USE_LVGL