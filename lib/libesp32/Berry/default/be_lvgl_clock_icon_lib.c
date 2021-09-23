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
    (be_nested_const_str("input", -103256197, 5)),
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
    (be_nested_const_str("input", -103256197, 5)),
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
    13,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_string("init", 380752755, 4),
    /* K1   */  be_nested_string("lv", 1529997255, 2),
    /* K2   */  be_nested_string("seg7_font", -195276607, 9),
    /* K3   */  be_nested_string("set_style_local_text_font", 954295699, 25),
    /* K4   */  be_nested_string("OBJ_PART_MAIN", 658062838, 13),
    /* K5   */  be_nested_string("STATE_DEFAULT", 712406428, 13),
    /* K6   */  be_nested_string("get_height", -723211773, 10),
    /* K7   */  be_nested_string("set_text", 1849641155, 8),
    /* K8   */  be_nested_string("--:--", 1370615441, 5),
    /* K9   */  be_nested_string("get_width", -1001549996, 9),
    /* K10  */  be_nested_string("set_y", 1866178391, 5),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_string("get_style_pad_right", -1144679830, 19),
    /* K13  */  be_nested_string("set_x", 1849400772, 5),
    /* K14  */  be_const_int(3),
    /* K15  */  be_nested_string("set_style_local_pad_right", 1126712366, 25),
    /* K16  */  be_nested_string("set_style_local_bg_color", -498263023, 24),
    /* K17  */  be_nested_string("lv_color", 1419148319, 8),
    /* K18  */  be_nested_string("COLOR_BLACK", 264427940, 11),
    /* K19  */  be_nested_string("tasmota", 424643812, 7),
    /* K20  */  be_nested_string("add_driver", 1654458371, 10),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[76]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0xB80E0200,  //  0007  GETNGBL	R3	K1
      0x8C0C0702,  //  0008  GETMET	R3	R3	K2
      0x5416000F,  //  0009  LDINT	R5	16
      0x7C0C0400,  //  000A  CALL	R3	2
      0x4C100000,  //  000B  LDNIL	R4
      0x20100604,  //  000C  NE	R4	R3	R4
      0x78120006,  //  000D  JMPF	R4	#0015
      0x8C100103,  //  000E  GETMET	R4	R0	K3
      0xB81A0200,  //  000F  GETNGBL	R6	K1
      0x88180D04,  //  0010  GETMBR	R6	R6	K4
      0xB81E0200,  //  0011  GETNGBL	R7	K1
      0x881C0F05,  //  0012  GETMBR	R7	R7	K5
      0x5C200600,  //  0013  MOVE	R8	R3
      0x7C100800,  //  0014  CALL	R4	4
      0x4C100000,  //  0015  LDNIL	R4
      0x20100204,  //  0016  NE	R4	R1	R4
      0x7812002E,  //  0017  JMPF	R4	#0047
      0x8C100306,  //  0018  GETMET	R4	R1	K6
      0x7C100200,  //  0019  CALL	R4	1
      0x8C140107,  //  001A  GETMET	R5	R0	K7
      0x581C0008,  //  001B  LDCONST	R7	K8
      0x7C140400,  //  001C  CALL	R5	2
      0x8C140109,  //  001D  GETMET	R5	R0	K9
      0x7C140200,  //  001E  CALL	R5	1
      0x8C18010A,  //  001F  GETMET	R6	R0	K10
      0x8C200306,  //  0020  GETMET	R8	R1	K6
      0x7C200200,  //  0021  CALL	R8	1
      0x8C240106,  //  0022  GETMET	R9	R0	K6
      0x7C240200,  //  0023  CALL	R9	1
      0x04201009,  //  0024  SUB	R8	R8	R9
      0x0C20110B,  //  0025  DIV	R8	R8	K11
      0x7C180400,  //  0026  CALL	R6	2
      0x8C18030C,  //  0027  GETMET	R6	R1	K12
      0xB8220200,  //  0028  GETNGBL	R8	K1
      0x88201104,  //  0029  GETMBR	R8	R8	K4
      0xB8260200,  //  002A  GETNGBL	R9	K1
      0x88241305,  //  002B  GETMBR	R9	R9	K5
      0x7C180600,  //  002C  CALL	R6	3
      0x8C1C010D,  //  002D  GETMET	R7	R0	K13
      0x8C240309,  //  002E  GETMET	R9	R1	K9
      0x7C240200,  //  002F  CALL	R9	1
      0x04241205,  //  0030  SUB	R9	R9	R5
      0x04241206,  //  0031  SUB	R9	R9	R6
      0x0424130E,  //  0032  SUB	R9	R9	K14
      0x7C1C0400,  //  0033  CALL	R7	2
      0x8C1C030F,  //  0034  GETMET	R7	R1	K15
      0xB8260200,  //  0035  GETNGBL	R9	K1
      0x88241304,  //  0036  GETMBR	R9	R9	K4
      0xB82A0200,  //  0037  GETNGBL	R10	K1
      0x88281505,  //  0038  GETMBR	R10	R10	K5
      0x002C0C05,  //  0039  ADD	R11	R6	R5
      0x54320005,  //  003A  LDINT	R12	6
      0x002C160C,  //  003B  ADD	R11	R11	R12
      0x7C1C0800,  //  003C  CALL	R7	4
      0x8C1C0110,  //  003D  GETMET	R7	R0	K16
      0xB8260200,  //  003E  GETNGBL	R9	K1
      0x88241304,  //  003F  GETMBR	R9	R9	K4
      0xB82A0200,  //  0040  GETNGBL	R10	K1
      0x88281505,  //  0041  GETMBR	R10	R10	K5
      0xB82E2200,  //  0042  GETNGBL	R11	K17
      0xB8320200,  //  0043  GETNGBL	R12	K1
      0x88301912,  //  0044  GETMBR	R12	R12	K18
      0x7C2C0200,  //  0045  CALL	R11	1
      0x7C1C0800,  //  0046  CALL	R7	4
      0xB8122600,  //  0047  GETNGBL	R4	K19
      0x8C100914,  //  0048  GETMET	R4	R4	K20
      0x5C180000,  //  0049  MOVE	R6	R0
      0x7C100400,  //  004A  CALL	R4	2
      0x80000000,  //  004B  RET	0
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
    (be_nested_const_str("input", -103256197, 5)),
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