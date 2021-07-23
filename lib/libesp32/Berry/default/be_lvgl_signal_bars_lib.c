/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

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
      be_nested_string("percentage", -1756136011, 10),
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
      be_nested_string("percentage", -1756136011, 10),
      be_const_int(0),
      be_nested_string("invalidate", -1645232368, 10),
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
** Solidified function: my_design_cb
********************************************************************/
be_local_closure(my_design_cb,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    3,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[35]) {     /* constants */
      be_nested_string("global", 503252654, 6),
      be_nested_string("lv", 1529997255, 2),
      be_nested_string("get_height", -723211773, 10),
      be_nested_string("get_width", -1001549996, 9),
      be_nested_string("atleast1", 1956331672, 8),
      be_const_int(3),
      be_const_int(2),
      be_nested_string("DESIGN_COVER_CHK", -1298406708, 16),
      be_nested_string("ancestor_design", 421545719, 15),
      be_nested_string("call", -1276017495, 4),
      be_nested_string("DESIGN_DRAW_MAIN", -904475754, 16),
      be_nested_string("get_coords", 1044089006, 10),
      be_nested_string("x1", 274927234, 2),
      be_nested_string("y1", -1939865569, 2),
      be_nested_string("draw_line_dsc_init", -428273650, 18),
      be_nested_string("line_dsc", -200476318, 8),
      be_nested_string("init_draw_line_dsc", -1787031256, 18),
      be_nested_string("OBJ_PART_MAIN", 658062838, 13),
      be_nested_string("round_start", -1345482912, 11),
      be_const_int(1),
      be_nested_string("round_end", 985288225, 9),
      be_nested_string("width", -1786286561, 5),
      be_nested_string("get_style_line_color", 805371932, 20),
      be_nested_string("STATE_DEFAULT", 712406428, 13),
      be_nested_string("get_style_bg_color", 964794381, 18),
      be_const_int(0),
      be_nested_string("color", 1031692888, 5),
      be_nested_string("percentage", -1756136011, 10),
      be_nested_string("p1", -1605446022, 2),
      be_nested_string("y", -66302220, 1),
      be_nested_string("x", -49524601, 1),
      be_nested_string("p2", -1622223641, 2),
      be_nested_string("draw_line", 1634465686, 9),
      be_nested_string("stop_iteration", -121173395, 14),
      be_nested_string("DESIGN_RES_OK", -1265307123, 13),
    }),
    (be_nested_const_str("my_design_cb", -1173588798, 12)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[110]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	R256
      0x88100701,  //  0001  GETMBR	R4	R3	R257
      0x8C140102,  //  0002  GETMET	R5	R0	R258
      0x7C140200,  //  0003  CALL	R5	1
      0x8C180103,  //  0004  GETMET	R6	R0	R259
      0x7C180200,  //  0005  CALL	R6	1
      0x8C1C0104,  //  0006  GETMET	R7	R0	R260
      0x5426000E,  //  0007  LDINT	R9	15
      0x0C240C09,  //  0008  DIV	R9	R6	R9
      0x7C1C0400,  //  0009  CALL	R7	2
      0x8C200104,  //  000A  GETMET	R8	R0	R260
      0x08280F05,  //  000B  MUL	R10	R7	R261
      0x04280C0A,  //  000C  SUB	R10	R6	R10
      0x542E0003,  //  000D  LDINT	R11	4
      0x0C28140B,  //  000E  DIV	R10	R10	R11
      0x7C200400,  //  000F  CALL	R8	2
      0x0C241106,  //  0010  DIV	R9	R8	R262
      0x88280907,  //  0011  GETMBR	R10	R4	R263
      0x1C28040A,  //  0012  EQ	R10	R2	R10
      0x782A0007,  //  0013  JMPF	R10	#001C
      0x88280108,  //  0014  GETMBR	R10	R0	R264
      0x8C281509,  //  0015  GETMET	R10	R10	R265
      0x5C300000,  //  0016  MOVE	R12	R0
      0x5C340200,  //  0017  MOVE	R13	R1
      0x5C380400,  //  0018  MOVE	R14	R2
      0x7C280800,  //  0019  CALL	R10	4
      0x80041400,  //  001A  RET	1	R10
      0x7002004F,  //  001B  JMP		#006C
      0x8828090A,  //  001C  GETMBR	R10	R4	R266
      0x1C28040A,  //  001D  EQ	R10	R2	R10
      0x782A004C,  //  001E  JMPF	R10	#006C
      0x8C28010B,  //  001F  GETMET	R10	R0	R267
      0x5C300200,  //  0020  MOVE	R12	R1
      0x7C280400,  //  0021  CALL	R10	2
      0x8828030C,  //  0022  GETMBR	R10	R1	R268
      0x882C030D,  //  0023  GETMBR	R11	R1	R269
      0x8C30090E,  //  0024  GETMET	R12	R4	R270
      0x8838010F,  //  0025  GETMBR	R14	R0	R271
      0x7C300400,  //  0026  CALL	R12	2
      0x8C300110,  //  0027  GETMET	R12	R0	R272
      0x88380911,  //  0028  GETMBR	R14	R4	R273
      0x883C010F,  //  0029  GETMBR	R15	R0	R271
      0x7C300600,  //  002A  CALL	R12	3
      0x8830010F,  //  002B  GETMBR	R12	R0	R271
      0x90322513,  //  002C  SETMBR	R12	R274	R275
      0x8830010F,  //  002D  GETMBR	R12	R0	R271
      0x90322913,  //  002E  SETMBR	R12	R276	R275
      0x8830010F,  //  002F  GETMBR	R12	R0	R271
      0x90322A08,  //  0030  SETMBR	R12	R277	R8
      0x8C300116,  //  0031  GETMET	R12	R0	R278
      0x88380911,  //  0032  GETMBR	R14	R4	R273
      0x883C0917,  //  0033  GETMBR	R15	R4	R279
      0x7C300600,  //  0034  CALL	R12	3
      0x8C340118,  //  0035  GETMET	R13	R0	R280
      0x883C0911,  //  0036  GETMBR	R15	R4	R273
      0x88400917,  //  0037  GETMBR	R16	R4	R279
      0x7C340600,  //  0038  CALL	R13	3
      0x60380000,  //  0039  GETGBL	R14	G0
      0x403E3305,  //  003A  CONNECT	R15	R281	R261
      0x7C380200,  //  003B  CALL	R14	1
      0xA802002B,  //  003C  EXBLK	0	#0069
      0x5C3C1C00,  //  003D  MOVE	R15	R14
      0x7C3C0000,  //  003E  CALL	R15	0
      0x8840010F,  //  003F  GETMBR	R16	R0	R271
      0x8844011B,  //  0040  GETMBR	R17	R0	R283
      0x00481F13,  //  0041  ADD	R18	R15	R275
      0x544E0013,  //  0042  LDINT	R19	20
      0x08482413,  //  0043  MUL	R18	R18	R19
      0x28442212,  //  0044  GE	R17	R17	R18
      0x78460001,  //  0045  JMPF	R17	#0048
      0x5C441800,  //  0046  MOVE	R17	R12
      0x70020000,  //  0047  JMP		#0049
      0x5C441A00,  //  0048  MOVE	R17	R13
      0x90423411,  //  0049  SETMBR	R16	R282	R17
      0x8840011C,  //  004A  GETMBR	R16	R0	R284
      0x00441605,  //  004B  ADD	R17	R11	R5
      0x04442313,  //  004C  SUB	R17	R17	R275
      0x04442209,  //  004D  SUB	R17	R17	R9
      0x90423A11,  //  004E  SETMBR	R16	R285	R17
      0x8840011C,  //  004F  GETMBR	R16	R0	R284
      0x00441007,  //  0050  ADD	R17	R8	R7
      0x08441E11,  //  0051  MUL	R17	R15	R17
      0x00441411,  //  0052  ADD	R17	R10	R17
      0x00442209,  //  0053  ADD	R17	R17	R9
      0x90423C11,  //  0054  SETMBR	R16	R286	R17
      0x8840011F,  //  0055  GETMBR	R16	R0	R287
      0x04460A0F,  //  0056  SUB	R17	R261	R15
      0x04480A08,  //  0057  SUB	R18	R5	R8
      0x08442212,  //  0058  MUL	R17	R17	R18
      0x544A0003,  //  0059  LDINT	R18	4
      0x0C442212,  //  005A  DIV	R17	R17	R18
      0x00441611,  //  005B  ADD	R17	R11	R17
      0x00442209,  //  005C  ADD	R17	R17	R9
      0x90423A11,  //  005D  SETMBR	R16	R285	R17
      0x8840011F,  //  005E  GETMBR	R16	R0	R287
      0x8844011C,  //  005F  GETMBR	R17	R0	R284
      0x8844231E,  //  0060  GETMBR	R17	R17	R286
      0x90423C11,  //  0061  SETMBR	R16	R286	R17
      0x8C400920,  //  0062  GETMET	R16	R4	R288
      0x8848011C,  //  0063  GETMBR	R18	R0	R284
      0x884C011F,  //  0064  GETMBR	R19	R0	R287
      0x5C500200,  //  0065  MOVE	R20	R1
      0x8854010F,  //  0066  GETMBR	R21	R0	R271
      0x7C400A00,  //  0067  CALL	R16	5
      0x7001FFD3,  //  0068  JMP		#003D
      0x58380021,  //  0069  LDCONST	R14	K33
      0xAC380200,  //  006A  CATCH	R14	1	0
      0xB0080000,  //  006B  RAISE	2	R0	R0
      0x88280922,  //  006C  GETMBR	R10	R4	R290
      0x80041400,  //  006D  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: atleast1
********************************************************************/
be_local_closure(atleast1,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
      be_const_int(1),
    }),
    (be_nested_const_str("atleast1", 1956331672, 8)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[ 6]) {  /* code */
      0x28080300,  //  0000  GE	R2	R1	R256
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x80040200,  //  0002  RET	1	R1
      0x70020000,  //  0003  JMP		#0005
      0x80060000,  //  0004  RET	1	R256
      0x80000000,  //  0005  RET	0	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
      be_nested_string("global", 503252654, 6),
      be_nested_string("lv", 1529997255, 2),
      be_nested_string("lv_obj", -37134147, 6),
      be_nested_string("init", 380752755, 4),
      be_nested_string("ancestor_design", 421545719, 15),
      be_nested_string("get_design_cb", -825649242, 13),
      be_nested_string("set_design_cb", 1469311634, 13),
      be_nested_string("my_design_cb", -1173588798, 12),
      be_nested_string("percentage", -1756136011, 10),
      be_nested_string("p1", -1605446022, 2),
      be_nested_string("lv_point", -174745506, 8),
      be_nested_string("p2", -1622223641, 2),
      be_nested_string("line_dsc", -200476318, 8),
      be_nested_string("lv_draw_line_dsc", -1872162060, 16),
      be_nested_string("area", -1693507260, 4),
      be_nested_string("lv_area", -1773816895, 7),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[31]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	R256
      0x88100701,  //  0001  GETMBR	R4	R3	R257
      0x60140014,  //  0002  GETGBL	R5	G20
      0x5C180000,  //  0003  MOVE	R6	R0
      0x881C0702,  //  0004  GETMBR	R7	R3	R258
      0x7C140400,  //  0005  CALL	R5	2
      0x8C140B03,  //  0006  GETMET	R5	R5	R259
      0x5C1C0200,  //  0007  MOVE	R7	R1
      0x5C200400,  //  0008  MOVE	R8	R2
      0x7C140600,  //  0009  CALL	R5	3
      0x8C140105,  //  000A  GETMET	R5	R0	R261
      0x7C140200,  //  000B  CALL	R5	1
      0x90020805,  //  000C  SETMBR	R0	R260	R5
      0x8C140106,  //  000D  GETMET	R5	R0	R262
      0x881C0107,  //  000E  GETMBR	R7	R0	R263
      0x7C140400,  //  000F  CALL	R5	2
      0x54160063,  //  0010  LDINT	R5	100
      0x90021005,  //  0011  SETMBR	R0	R264	R5
      0x8C14070A,  //  0012  GETMET	R5	R3	R266
      0x7C140200,  //  0013  CALL	R5	1
      0x90021205,  //  0014  SETMBR	R0	R265	R5
      0x8C14070A,  //  0015  GETMET	R5	R3	R266
      0x7C140200,  //  0016  CALL	R5	1
      0x90021605,  //  0017  SETMBR	R0	R267	R5
      0x8C14070D,  //  0018  GETMET	R5	R3	R269
      0x7C140200,  //  0019  CALL	R5	1
      0x90021805,  //  001A  SETMBR	R0	R268	R5
      0x8C14070F,  //  001B  GETMET	R5	R3	R271
      0x7C140200,  //  001C  CALL	R5	1
      0x90021C05,  //  001D  SETMBR	R0	R270	R5
      0x80000000,  //  001E  RET	0	R0
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
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("p1", -1605446022, 2, -1), be_const_index(2) },
        { be_nested_key("get_percentage", -1414483304, 14, 5), be_const_closure(get_percentage_closure) },
        { be_nested_key("set_percentage", -1342944572, 14, -1), be_const_closure(set_percentage_closure) },
        { be_nested_key("p2", -1622223641, 2, -1), be_const_index(3) },
        { be_nested_key("ancestor_design", 421545719, 15, -1), be_const_index(0) },
        { be_nested_key("percentage", -1756136011, 10, 4), be_const_index(1) },
        { be_nested_key("line_dsc", -200476318, 8, -1), be_const_index(4) },
        { be_nested_key("my_design_cb", -1173588798, 12, -1), be_const_closure(my_design_cb_closure) },
        { be_nested_key("atleast1", 1956331672, 8, -1), be_const_closure(atleast1_closure) },
        { be_nested_key("init", 380752755, 4, 8), be_const_closure(init_closure) },
        { be_nested_key("area", -1693507260, 4, 3), be_const_index(5) },
    })),
    (be_nested_const_str("lv_signal_bars", -780994737, 14))
);
/*******************************************************************/

void be_load_lv_signal_bars_class(bvm *vm) {
    be_pushntvclass(vm, &be_lv_signal_bars_class);
    be_setglobal(vm, "lv_signal_bars");
    be_pop(vm, 1);
}

#endif // USE_LVGL