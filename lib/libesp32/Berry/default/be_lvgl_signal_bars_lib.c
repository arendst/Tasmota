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
    23,                          /* nstack */
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
          be_const_int(1),
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
    ( &(const bvalue[34]) {     /* constants */
      be_nested_string("global", 503252654, 6),
      be_nested_string("lv", 1529997255, 2),
      be_nested_string("get_height", -723211773, 10),
      be_nested_string("get_width", -1001549996, 9),
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
    ( &(const binstruction[111]) {  /* code */
      0x840C0000,  //  0000  CLOSURE	R3	P0
      0xA4120000,  //  0001  IMPORT	R4	R256
      0x88140901,  //  0002  GETMBR	R5	R4	R257
      0x8C180102,  //  0003  GETMET	R6	R0	R258
      0x7C180200,  //  0004  CALL	R6	1
      0x8C1C0103,  //  0005  GETMET	R7	R0	R259
      0x7C1C0200,  //  0006  CALL	R7	1
      0x5C200600,  //  0007  MOVE	R8	R3
      0x5426000E,  //  0008  LDINT	R9	15
      0x0C240E09,  //  0009  DIV	R9	R7	R9
      0x7C200200,  //  000A  CALL	R8	1
      0x5C240600,  //  000B  MOVE	R9	R3
      0x08281104,  //  000C  MUL	R10	R8	R260
      0x04280E0A,  //  000D  SUB	R10	R7	R10
      0x542E0003,  //  000E  LDINT	R11	4
      0x0C28140B,  //  000F  DIV	R10	R10	R11
      0x7C240200,  //  0010  CALL	R9	1
      0x0C281305,  //  0011  DIV	R10	R9	R261
      0x882C0B06,  //  0012  GETMBR	R11	R5	R262
      0x1C2C040B,  //  0013  EQ	R11	R2	R11
      0x782E0007,  //  0014  JMPF	R11	#001D
      0x882C0107,  //  0015  GETMBR	R11	R0	R263
      0x8C2C1708,  //  0016  GETMET	R11	R11	R264
      0x5C340000,  //  0017  MOVE	R13	R0
      0x5C380200,  //  0018  MOVE	R14	R1
      0x5C3C0400,  //  0019  MOVE	R15	R2
      0x7C2C0800,  //  001A  CALL	R11	4
      0x80041600,  //  001B  RET	1	R11
      0x7002004F,  //  001C  JMP		#006D
      0x882C0B09,  //  001D  GETMBR	R11	R5	R265
      0x1C2C040B,  //  001E  EQ	R11	R2	R11
      0x782E004C,  //  001F  JMPF	R11	#006D
      0x8C2C010A,  //  0020  GETMET	R11	R0	R266
      0x5C340200,  //  0021  MOVE	R13	R1
      0x7C2C0400,  //  0022  CALL	R11	2
      0x882C030B,  //  0023  GETMBR	R11	R1	R267
      0x8830030C,  //  0024  GETMBR	R12	R1	R268
      0x8C340B0D,  //  0025  GETMET	R13	R5	R269
      0x883C010E,  //  0026  GETMBR	R15	R0	R270
      0x7C340400,  //  0027  CALL	R13	2
      0x8C34010F,  //  0028  GETMET	R13	R0	R271
      0x883C0B10,  //  0029  GETMBR	R15	R5	R272
      0x8840010E,  //  002A  GETMBR	R16	R0	R270
      0x7C340600,  //  002B  CALL	R13	3
      0x8834010E,  //  002C  GETMBR	R13	R0	R270
      0x90362312,  //  002D  SETMBR	R13	R273	R274
      0x8834010E,  //  002E  GETMBR	R13	R0	R270
      0x90362712,  //  002F  SETMBR	R13	R275	R274
      0x8834010E,  //  0030  GETMBR	R13	R0	R270
      0x90362809,  //  0031  SETMBR	R13	R276	R9
      0x8C340115,  //  0032  GETMET	R13	R0	R277
      0x883C0B10,  //  0033  GETMBR	R15	R5	R272
      0x88400B16,  //  0034  GETMBR	R16	R5	R278
      0x7C340600,  //  0035  CALL	R13	3
      0x8C380117,  //  0036  GETMET	R14	R0	R279
      0x88400B10,  //  0037  GETMBR	R16	R5	R272
      0x88440B16,  //  0038  GETMBR	R17	R5	R278
      0x7C380600,  //  0039  CALL	R14	3
      0x603C0000,  //  003A  GETGBL	R15	G0
      0x40423104,  //  003B  CONNECT	R16	R280	R260
      0x7C3C0200,  //  003C  CALL	R15	1
      0xA802002B,  //  003D  EXBLK	0	#006A
      0x5C401E00,  //  003E  MOVE	R16	R15
      0x7C400000,  //  003F  CALL	R16	0
      0x8844010E,  //  0040  GETMBR	R17	R0	R270
      0x8848011A,  //  0041  GETMBR	R18	R0	R282
      0x004C2112,  //  0042  ADD	R19	R16	R274
      0x54520013,  //  0043  LDINT	R20	20
      0x084C2614,  //  0044  MUL	R19	R19	R20
      0x28482413,  //  0045  GE	R18	R18	R19
      0x784A0001,  //  0046  JMPF	R18	#0049
      0x5C481A00,  //  0047  MOVE	R18	R13
      0x70020000,  //  0048  JMP		#004A
      0x5C481C00,  //  0049  MOVE	R18	R14
      0x90463212,  //  004A  SETMBR	R17	R281	R18
      0x8844011B,  //  004B  GETMBR	R17	R0	R283
      0x00481806,  //  004C  ADD	R18	R12	R6
      0x04482512,  //  004D  SUB	R18	R18	R274
      0x0448240A,  //  004E  SUB	R18	R18	R10
      0x90463812,  //  004F  SETMBR	R17	R284	R18
      0x8844011B,  //  0050  GETMBR	R17	R0	R283
      0x00481208,  //  0051  ADD	R18	R9	R8
      0x08482012,  //  0052  MUL	R18	R16	R18
      0x00481612,  //  0053  ADD	R18	R11	R18
      0x0048240A,  //  0054  ADD	R18	R18	R10
      0x90463A12,  //  0055  SETMBR	R17	R285	R18
      0x8844011E,  //  0056  GETMBR	R17	R0	R286
      0x044A0810,  //  0057  SUB	R18	R260	R16
      0x044C0C09,  //  0058  SUB	R19	R6	R9
      0x08482413,  //  0059  MUL	R18	R18	R19
      0x544E0003,  //  005A  LDINT	R19	4
      0x0C482413,  //  005B  DIV	R18	R18	R19
      0x00481812,  //  005C  ADD	R18	R12	R18
      0x0048240A,  //  005D  ADD	R18	R18	R10
      0x90463812,  //  005E  SETMBR	R17	R284	R18
      0x8844011E,  //  005F  GETMBR	R17	R0	R286
      0x8848011B,  //  0060  GETMBR	R18	R0	R283
      0x8848251D,  //  0061  GETMBR	R18	R18	R285
      0x90463A12,  //  0062  SETMBR	R17	R285	R18
      0x8C440B1F,  //  0063  GETMET	R17	R5	R287
      0x884C011B,  //  0064  GETMBR	R19	R0	R283
      0x8850011E,  //  0065  GETMBR	R20	R0	R286
      0x5C540200,  //  0066  MOVE	R21	R1
      0x8858010E,  //  0067  GETMBR	R22	R0	R270
      0x7C440A00,  //  0068  CALL	R17	5
      0x7001FFD3,  //  0069  JMP		#003E
      0x583C0020,  //  006A  LDCONST	R15	K32
      0xAC3C0200,  //  006B  CATCH	R15	1	0
      0xB0080000,  //  006C  RAISE	2	R0	R0
      0x882C0B21,  //  006D  GETMBR	R11	R5	R289
      0x80041600,  //  006E  RET	1	R11
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