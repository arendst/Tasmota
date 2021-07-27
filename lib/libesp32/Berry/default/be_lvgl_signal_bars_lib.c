/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: my_design_cb
********************************************************************/
be_local_closure(my_design_cb,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    3,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        1,                          /* argc */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
          be_const_int(1),    /* R256 - K0 */
        }),
        (be_nested_const_str("atleast1", 1956331672, 8)),
        (be_nested_const_str("input", -103256197, 5)),
        ( &(const binstruction[ 6]) {  /* code */
          0x28040100,  //  0000  GE	R1	R0	R256
          0x78060001,  //  0001  JMPF	R1	#0004
          0x80040000,  //  0002  RET	1	R0
          0x70020000,  //  0003  JMP		#0005
          0x80060000,  //  0004  RET	1	R256
          0x80000000,  //  0005  RET	0	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[33]) {     /* constants */
      be_nested_string("get_height", -723211773, 10),    /* R256 - K0 */
      be_nested_string("get_width", -1001549996, 9),    /* R257 - K1 */
      be_const_int(3),    /* R258 - K2 */
      be_const_int(2),    /* R259 - K3 */
      be_nested_string("lv", 1529997255, 2),    /* R260 - K4 */
      be_nested_string("DESIGN_COVER_CHK", -1298406708, 16),    /* R261 - K5 */
      be_nested_string("ancestor_design", 421545719, 15),    /* R262 - K6 */
      be_nested_string("call", -1276017495, 4),    /* R263 - K7 */
      be_nested_string("DESIGN_DRAW_MAIN", -904475754, 16),    /* R264 - K8 */
      be_nested_string("get_coords", 1044089006, 10),    /* R265 - K9 */
      be_nested_string("x1", 274927234, 2),    /* R266 - K10 */
      be_nested_string("y1", -1939865569, 2),    /* R267 - K11 */
      be_nested_string("draw_line_dsc_init", -428273650, 18),    /* R268 - K12 */
      be_nested_string("line_dsc", -200476318, 8),    /* R269 - K13 */
      be_nested_string("init_draw_line_dsc", -1787031256, 18),    /* R270 - K14 */
      be_nested_string("OBJ_PART_MAIN", 658062838, 13),    /* R271 - K15 */
      be_nested_string("round_start", -1345482912, 11),    /* R272 - K16 */
      be_const_int(1),    /* R273 - K17 */
      be_nested_string("round_end", 985288225, 9),    /* R274 - K18 */
      be_nested_string("width", -1786286561, 5),    /* R275 - K19 */
      be_nested_string("get_style_line_color", 805371932, 20),    /* R276 - K20 */
      be_nested_string("STATE_DEFAULT", 712406428, 13),    /* R277 - K21 */
      be_nested_string("get_style_bg_color", 964794381, 18),    /* R278 - K22 */
      be_const_int(0),    /* R279 - K23 */
      be_nested_string("color", 1031692888, 5),    /* R280 - K24 */
      be_nested_string("percentage", -1756136011, 10),    /* R281 - K25 */
      be_nested_string("p1", -1605446022, 2),    /* R282 - K26 */
      be_nested_string("y", -66302220, 1),    /* R283 - K27 */
      be_nested_string("x", -49524601, 1),    /* R284 - K28 */
      be_nested_string("p2", -1622223641, 2),    /* R285 - K29 */
      be_nested_string("draw_line", 1634465686, 9),    /* R286 - K30 */
      be_nested_string("stop_iteration", -121173395, 14),    /* R287 - K31 */
      be_nested_string("DESIGN_RES_OK", -1265307123, 13),    /* R288 - K32 */
    }),
    (be_nested_const_str("my_design_cb", -1173588798, 12)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[119]) {  /* code */
      0x840C0000,  //  0000  CLOSURE	R3	P0
      0x8C100100,  //  0001  GETMET	R4	R0	R256
      0x7C100200,  //  0002  CALL	R4	1
      0x8C140101,  //  0003  GETMET	R5	R0	R257
      0x7C140200,  //  0004  CALL	R5	1
      0x5C180600,  //  0005  MOVE	R6	R3
      0x541E000E,  //  0006  LDINT	R7	15
      0x0C1C0A07,  //  0007  DIV	R7	R5	R7
      0x7C180200,  //  0008  CALL	R6	1
      0x5C1C0600,  //  0009  MOVE	R7	R3
      0x08200D02,  //  000A  MUL	R8	R6	R258
      0x04200A08,  //  000B  SUB	R8	R5	R8
      0x54260003,  //  000C  LDINT	R9	4
      0x0C201009,  //  000D  DIV	R8	R8	R9
      0x7C1C0200,  //  000E  CALL	R7	1
      0x0C200F03,  //  000F  DIV	R8	R7	R259
      0xB8260800,  //  0010  GETNGBL	R9	R260
      0x88241305,  //  0011  GETMBR	R9	R9	R261
      0x1C240409,  //  0012  EQ	R9	R2	R9
      0x78260007,  //  0013  JMPF	R9	#001C
      0x88240106,  //  0014  GETMBR	R9	R0	R262
      0x8C241307,  //  0015  GETMET	R9	R9	R263
      0x5C2C0000,  //  0016  MOVE	R11	R0
      0x5C300200,  //  0017  MOVE	R12	R1
      0x5C340400,  //  0018  MOVE	R13	R2
      0x7C240800,  //  0019  CALL	R9	4
      0x80041200,  //  001A  RET	1	R9
      0x70020057,  //  001B  JMP		#0074
      0xB8260800,  //  001C  GETNGBL	R9	R260
      0x88241308,  //  001D  GETMBR	R9	R9	R264
      0x1C240409,  //  001E  EQ	R9	R2	R9
      0x78260053,  //  001F  JMPF	R9	#0074
      0x8C240109,  //  0020  GETMET	R9	R0	R265
      0x5C2C0200,  //  0021  MOVE	R11	R1
      0x7C240400,  //  0022  CALL	R9	2
      0x8824030A,  //  0023  GETMBR	R9	R1	R266
      0x8828030B,  //  0024  GETMBR	R10	R1	R267
      0xB82E0800,  //  0025  GETNGBL	R11	R260
      0x8C2C170C,  //  0026  GETMET	R11	R11	R268
      0x8834010D,  //  0027  GETMBR	R13	R0	R269
      0x7C2C0400,  //  0028  CALL	R11	2
      0x8C2C010E,  //  0029  GETMET	R11	R0	R270
      0xB8360800,  //  002A  GETNGBL	R13	R260
      0x88341B0F,  //  002B  GETMBR	R13	R13	R271
      0x8838010D,  //  002C  GETMBR	R14	R0	R269
      0x7C2C0600,  //  002D  CALL	R11	3
      0x882C010D,  //  002E  GETMBR	R11	R0	R269
      0x902E2111,  //  002F  SETMBR	R11	R272	R273
      0x882C010D,  //  0030  GETMBR	R11	R0	R269
      0x902E2511,  //  0031  SETMBR	R11	R274	R273
      0x882C010D,  //  0032  GETMBR	R11	R0	R269
      0x902E2607,  //  0033  SETMBR	R11	R275	R7
      0x8C2C0114,  //  0034  GETMET	R11	R0	R276
      0xB8360800,  //  0035  GETNGBL	R13	R260
      0x88341B0F,  //  0036  GETMBR	R13	R13	R271
      0xB83A0800,  //  0037  GETNGBL	R14	R260
      0x88381D15,  //  0038  GETMBR	R14	R14	R277
      0x7C2C0600,  //  0039  CALL	R11	3
      0x8C300116,  //  003A  GETMET	R12	R0	R278
      0xB83A0800,  //  003B  GETNGBL	R14	R260
      0x88381D0F,  //  003C  GETMBR	R14	R14	R271
      0xB83E0800,  //  003D  GETNGBL	R15	R260
      0x883C1F15,  //  003E  GETMBR	R15	R15	R277
      0x7C300600,  //  003F  CALL	R12	3
      0x60340000,  //  0040  GETGBL	R13	G0
      0x403A2F02,  //  0041  CONNECT	R14	R279	R258
      0x7C340200,  //  0042  CALL	R13	1
      0xA802002C,  //  0043  EXBLK	0	#0071
      0x5C381A00,  //  0044  MOVE	R14	R13
      0x7C380000,  //  0045  CALL	R14	0
      0x883C010D,  //  0046  GETMBR	R15	R0	R269
      0x88400119,  //  0047  GETMBR	R16	R0	R281
      0x00441D11,  //  0048  ADD	R17	R14	R273
      0x544A0013,  //  0049  LDINT	R18	20
      0x08442212,  //  004A  MUL	R17	R17	R18
      0x28402011,  //  004B  GE	R16	R16	R17
      0x78420001,  //  004C  JMPF	R16	#004F
      0x5C401600,  //  004D  MOVE	R16	R11
      0x70020000,  //  004E  JMP		#0050
      0x5C401800,  //  004F  MOVE	R16	R12
      0x903E3010,  //  0050  SETMBR	R15	R280	R16
      0x883C011A,  //  0051  GETMBR	R15	R0	R282
      0x00401404,  //  0052  ADD	R16	R10	R4
      0x04402111,  //  0053  SUB	R16	R16	R273
      0x04402008,  //  0054  SUB	R16	R16	R8
      0x903E3610,  //  0055  SETMBR	R15	R283	R16
      0x883C011A,  //  0056  GETMBR	R15	R0	R282
      0x00400E06,  //  0057  ADD	R16	R7	R6
      0x08401C10,  //  0058  MUL	R16	R14	R16
      0x00401210,  //  0059  ADD	R16	R9	R16
      0x00402008,  //  005A  ADD	R16	R16	R8
      0x903E3810,  //  005B  SETMBR	R15	R284	R16
      0x883C011D,  //  005C  GETMBR	R15	R0	R285
      0x0442040E,  //  005D  SUB	R16	R258	R14
      0x04440807,  //  005E  SUB	R17	R4	R7
      0x08402011,  //  005F  MUL	R16	R16	R17
      0x54460003,  //  0060  LDINT	R17	4
      0x0C402011,  //  0061  DIV	R16	R16	R17
      0x00401410,  //  0062  ADD	R16	R10	R16
      0x00402008,  //  0063  ADD	R16	R16	R8
      0x903E3610,  //  0064  SETMBR	R15	R283	R16
      0x883C011D,  //  0065  GETMBR	R15	R0	R285
      0x8840011A,  //  0066  GETMBR	R16	R0	R282
      0x8840211C,  //  0067  GETMBR	R16	R16	R284
      0x903E3810,  //  0068  SETMBR	R15	R284	R16
      0xB83E0800,  //  0069  GETNGBL	R15	R260
      0x8C3C1F1E,  //  006A  GETMET	R15	R15	R286
      0x8844011A,  //  006B  GETMBR	R17	R0	R282
      0x8848011D,  //  006C  GETMBR	R18	R0	R285
      0x5C4C0200,  //  006D  MOVE	R19	R1
      0x8850010D,  //  006E  GETMBR	R20	R0	R269
      0x7C3C0A00,  //  006F  CALL	R15	5
      0x7001FFD2,  //  0070  JMP		#0044
      0x5834001F,  //  0071  LDCONST	R13	K31
      0xAC340200,  //  0072  CATCH	R13	1	0
      0xB0080000,  //  0073  RAISE	2	R0	R0
      0xB8260800,  //  0074  GETNGBL	R9	R260
      0x88241320,  //  0075  GETMBR	R9	R9	R288
      0x80041200,  //  0076  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_percentage
********************************************************************/
be_local_closure(get_percentage,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
      be_nested_string("percentage", -1756136011, 10),    /* R256 - K0 */
    }),
    (be_nested_const_str("get_percentage", -1414483304, 14)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	R256
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
      be_nested_string("lv_obj", -37134147, 6),    /* R256 - K0 */
      be_nested_string("init", 380752755, 4),    /* R257 - K1 */
      be_nested_string("ancestor_design", 421545719, 15),    /* R258 - K2 */
      be_nested_string("get_design_cb", -825649242, 13),    /* R259 - K3 */
      be_nested_string("set_design_cb", 1469311634, 13),    /* R260 - K4 */
      be_nested_string("my_design_cb", -1173588798, 12),    /* R261 - K5 */
      be_nested_string("percentage", -1756136011, 10),    /* R262 - K6 */
      be_nested_string("p1", -1605446022, 2),    /* R263 - K7 */
      be_nested_string("lv_point", -174745506, 8),    /* R264 - K8 */
      be_nested_string("p2", -1622223641, 2),    /* R265 - K9 */
      be_nested_string("line_dsc", -200476318, 8),    /* R266 - K10 */
      be_nested_string("lv_draw_line_dsc", -1872162060, 16),    /* R267 - K11 */
      be_nested_string("area", -1693507260, 4),    /* R268 - K12 */
      be_nested_string("lv_area", -1773816895, 7),    /* R269 - K13 */
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[29]) {  /* code */
      0x600C0014,  //  0000  GETGBL	R3	G20
      0x5C100000,  //  0001  MOVE	R4	R0
      0xB8160000,  //  0002  GETNGBL	R5	R256
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C0C0701,  //  0004  GETMET	R3	R3	R257
      0x5C140200,  //  0005  MOVE	R5	R1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C0C0600,  //  0007  CALL	R3	3
      0x8C0C0103,  //  0008  GETMET	R3	R0	R259
      0x7C0C0200,  //  0009  CALL	R3	1
      0x90020403,  //  000A  SETMBR	R0	R258	R3
      0x8C0C0104,  //  000B  GETMET	R3	R0	R260
      0x88140105,  //  000C  GETMBR	R5	R0	R261
      0x7C0C0400,  //  000D  CALL	R3	2
      0x540E0063,  //  000E  LDINT	R3	100
      0x90020C03,  //  000F  SETMBR	R0	R262	R3
      0xB80E1000,  //  0010  GETNGBL	R3	R264
      0x7C0C0000,  //  0011  CALL	R3	0
      0x90020E03,  //  0012  SETMBR	R0	R263	R3
      0xB80E1000,  //  0013  GETNGBL	R3	R264
      0x7C0C0000,  //  0014  CALL	R3	0
      0x90021203,  //  0015  SETMBR	R0	R265	R3
      0xB80E1600,  //  0016  GETNGBL	R3	R267
      0x7C0C0000,  //  0017  CALL	R3	0
      0x90021403,  //  0018  SETMBR	R0	R266	R3
      0xB80E1A00,  //  0019  GETNGBL	R3	R269
      0x7C0C0000,  //  001A  CALL	R3	0
      0x90021803,  //  001B  SETMBR	R0	R268	R3
      0x80000000,  //  001C  RET	0	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_percentage
********************************************************************/
be_local_closure(set_percentage,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
      be_nested_string("percentage", -1756136011, 10),    /* R256 - K0 */
      be_const_int(0),    /* R257 - K1 */
      be_nested_string("invalidate", -1645232368, 10),    /* R258 - K2 */
    }),
    (be_nested_const_str("set_percentage", -1342944572, 14)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[18]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	R256
      0x540E0004,  //  0001  LDINT	R3	5
      0x0C080403,  //  0002  DIV	R2	R2	R3
      0x540E0063,  //  0003  LDINT	R3	100
      0x240C0203,  //  0004  GT	R3	R1	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x54060063,  //  0006  LDINT	R1	100
      0x140C0301,  //  0007  LT	R3	R1	R257
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x58040001,  //  0009  LDCONST	R1	K1
      0x90020001,  //  000A  SETMBR	R0	R256	R1
      0x540E0004,  //  000B  LDINT	R3	5
      0x0C0C0203,  //  000C  DIV	R3	R1	R3
      0x200C0403,  //  000D  NE	R3	R2	R3
      0x780E0001,  //  000E  JMPF	R3	#0011
      0x8C0C0102,  //  000F  GETMET	R3	R0	R258
      0x7C0C0200,  //  0010  CALL	R3	1
      0x80000000,  //  0011  RET	0	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_signal_bars
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_bars,
    6,
    &be_class_lv_obj,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("p1", -1605446022, 2, -1), be_const_index(2) },
        { be_nested_key("my_design_cb", -1173588798, 12, -1), be_const_closure(my_design_cb_closure) },
        { be_nested_key("get_percentage", -1414483304, 14, -1), be_const_closure(get_percentage_closure) },
        { be_nested_key("init", 380752755, 4, 7), be_const_closure(init_closure) },
        { be_nested_key("set_percentage", -1342944572, 14, 0), be_const_closure(set_percentage_closure) },
        { be_nested_key("percentage", -1756136011, 10, 3), be_const_index(1) },
        { be_nested_key("area", -1693507260, 4, -1), be_const_index(5) },
        { be_nested_key("p2", -1622223641, 2, -1), be_const_index(3) },
        { be_nested_key("line_dsc", -200476318, 8, 1), be_const_index(4) },
        { be_nested_key("ancestor_design", 421545719, 15, -1), be_const_index(0) },
    })),
    (be_nested_const_str("lv_signal_bars", -780994737, 14))
);
/*******************************************************************/

void be_load_lv_signal_bars_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_signal_bars);
    be_setglobal(vm, "lv_signal_bars");
    be_pop(vm, 1);
}

#endif // USE_LVGL