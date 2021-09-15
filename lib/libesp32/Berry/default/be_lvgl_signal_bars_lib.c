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
    0,                          /* varg */
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
        (be_nested_const_str("atleast1", 1956331672, 8)),
        (be_nested_const_str("input", -103256197, 5)),
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
    ( &(const bvalue[34]) {     /* constants */
    /* K0   */  be_nested_string("get_height", -723211773, 10),
    /* K1   */  be_nested_string("get_width", -1001549996, 9),
    /* K2   */  be_const_int(3),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_string("lv", 1529997255, 2),
    /* K5   */  be_nested_string("DESIGN_COVER_CHK", -1298406708, 16),
    /* K6   */  be_nested_string("ancestor_design", 421545719, 15),
    /* K7   */  be_nested_string("call", -1276017495, 4),
    /* K8   */  be_nested_string("DESIGN_DRAW_MAIN", -904475754, 16),
    /* K9   */  be_nested_string("get_coords", 1044089006, 10),
    /* K10  */  be_nested_string("area", -1693507260, 4),
    /* K11  */  be_nested_string("x1", 274927234, 2),
    /* K12  */  be_nested_string("y1", -1939865569, 2),
    /* K13  */  be_nested_string("draw_line_dsc_init", -428273650, 18),
    /* K14  */  be_nested_string("line_dsc", -200476318, 8),
    /* K15  */  be_nested_string("init_draw_line_dsc", -1787031256, 18),
    /* K16  */  be_nested_string("OBJ_PART_MAIN", 658062838, 13),
    /* K17  */  be_nested_string("round_start", -1345482912, 11),
    /* K18  */  be_const_int(1),
    /* K19  */  be_nested_string("round_end", 985288225, 9),
    /* K20  */  be_nested_string("width", -1786286561, 5),
    /* K21  */  be_nested_string("get_style_line_color", 805371932, 20),
    /* K22  */  be_nested_string("STATE_DEFAULT", 712406428, 13),
    /* K23  */  be_nested_string("get_style_bg_color", 964794381, 18),
    /* K24  */  be_const_int(0),
    /* K25  */  be_nested_string("color", 1031692888, 5),
    /* K26  */  be_nested_string("percentage", -1756136011, 10),
    /* K27  */  be_nested_string("p1", -1605446022, 2),
    /* K28  */  be_nested_string("y", -66302220, 1),
    /* K29  */  be_nested_string("x", -49524601, 1),
    /* K30  */  be_nested_string("p2", -1622223641, 2),
    /* K31  */  be_nested_string("draw_line", 1634465686, 9),
    /* K32  */  be_nested_string("stop_iteration", -121173395, 14),
    /* K33  */  be_nested_string("DESIGN_RES_OK", -1265307123, 13),
    }),
    (be_nested_const_str("my_design_cb", -1173588798, 12)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[121]) {  /* code */
      0x840C0000,  //  0000  CLOSURE	R3	P0
      0x8C100100,  //  0001  GETMET	R4	R0	K0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C140101,  //  0003  GETMET	R5	R0	K1
      0x7C140200,  //  0004  CALL	R5	1
      0x5C180600,  //  0005  MOVE	R6	R3
      0x541E000E,  //  0006  LDINT	R7	15
      0x0C1C0A07,  //  0007  DIV	R7	R5	R7
      0x7C180200,  //  0008  CALL	R6	1
      0x5C1C0600,  //  0009  MOVE	R7	R3
      0x08200D02,  //  000A  MUL	R8	R6	K2
      0x04200A08,  //  000B  SUB	R8	R5	R8
      0x54260003,  //  000C  LDINT	R9	4
      0x0C201009,  //  000D  DIV	R8	R8	R9
      0x7C1C0200,  //  000E  CALL	R7	1
      0x0C200F03,  //  000F  DIV	R8	R7	K3
      0xB8260800,  //  0010  GETNGBL	R9	K4
      0x88241305,  //  0011  GETMBR	R9	R9	K5
      0x1C240409,  //  0012  EQ	R9	R2	R9
      0x78260007,  //  0013  JMPF	R9	#001C
      0x88240106,  //  0014  GETMBR	R9	R0	K6
      0x8C241307,  //  0015  GETMET	R9	R9	K7
      0x5C2C0000,  //  0016  MOVE	R11	R0
      0x5C300200,  //  0017  MOVE	R12	R1
      0x5C340400,  //  0018  MOVE	R13	R2
      0x7C240800,  //  0019  CALL	R9	4
      0x80041200,  //  001A  RET	1	R9
      0x70020059,  //  001B  JMP		#0076
      0xB8260800,  //  001C  GETNGBL	R9	K4
      0x88241308,  //  001D  GETMBR	R9	R9	K8
      0x1C240409,  //  001E  EQ	R9	R2	R9
      0x78260055,  //  001F  JMPF	R9	#0076
      0x8C240109,  //  0020  GETMET	R9	R0	K9
      0x882C010A,  //  0021  GETMBR	R11	R0	K10
      0x7C240400,  //  0022  CALL	R9	2
      0x8824010A,  //  0023  GETMBR	R9	R0	K10
      0x8824130B,  //  0024  GETMBR	R9	R9	K11
      0x8828010A,  //  0025  GETMBR	R10	R0	K10
      0x8828150C,  //  0026  GETMBR	R10	R10	K12
      0xB82E0800,  //  0027  GETNGBL	R11	K4
      0x8C2C170D,  //  0028  GETMET	R11	R11	K13
      0x8834010E,  //  0029  GETMBR	R13	R0	K14
      0x7C2C0400,  //  002A  CALL	R11	2
      0x8C2C010F,  //  002B  GETMET	R11	R0	K15
      0xB8360800,  //  002C  GETNGBL	R13	K4
      0x88341B10,  //  002D  GETMBR	R13	R13	K16
      0x8838010E,  //  002E  GETMBR	R14	R0	K14
      0x7C2C0600,  //  002F  CALL	R11	3
      0x882C010E,  //  0030  GETMBR	R11	R0	K14
      0x902E2312,  //  0031  SETMBR	R11	K17	K18
      0x882C010E,  //  0032  GETMBR	R11	R0	K14
      0x902E2712,  //  0033  SETMBR	R11	K19	K18
      0x882C010E,  //  0034  GETMBR	R11	R0	K14
      0x902E2807,  //  0035  SETMBR	R11	K20	R7
      0x8C2C0115,  //  0036  GETMET	R11	R0	K21
      0xB8360800,  //  0037  GETNGBL	R13	K4
      0x88341B10,  //  0038  GETMBR	R13	R13	K16
      0xB83A0800,  //  0039  GETNGBL	R14	K4
      0x88381D16,  //  003A  GETMBR	R14	R14	K22
      0x7C2C0600,  //  003B  CALL	R11	3
      0x8C300117,  //  003C  GETMET	R12	R0	K23
      0xB83A0800,  //  003D  GETNGBL	R14	K4
      0x88381D10,  //  003E  GETMBR	R14	R14	K16
      0xB83E0800,  //  003F  GETNGBL	R15	K4
      0x883C1F16,  //  0040  GETMBR	R15	R15	K22
      0x7C300600,  //  0041  CALL	R12	3
      0x60340010,  //  0042  GETGBL	R13	G16
      0x403A3102,  //  0043  CONNECT	R14	K24	K2
      0x7C340200,  //  0044  CALL	R13	1
      0xA802002C,  //  0045  EXBLK	0	#0073
      0x5C381A00,  //  0046  MOVE	R14	R13
      0x7C380000,  //  0047  CALL	R14	0
      0x883C010E,  //  0048  GETMBR	R15	R0	K14
      0x8840011A,  //  0049  GETMBR	R16	R0	K26
      0x00441D12,  //  004A  ADD	R17	R14	K18
      0x544A0013,  //  004B  LDINT	R18	20
      0x08442212,  //  004C  MUL	R17	R17	R18
      0x28402011,  //  004D  GE	R16	R16	R17
      0x78420001,  //  004E  JMPF	R16	#0051
      0x5C401600,  //  004F  MOVE	R16	R11
      0x70020000,  //  0050  JMP		#0052
      0x5C401800,  //  0051  MOVE	R16	R12
      0x903E3210,  //  0052  SETMBR	R15	K25	R16
      0x883C011B,  //  0053  GETMBR	R15	R0	K27
      0x00401404,  //  0054  ADD	R16	R10	R4
      0x04402112,  //  0055  SUB	R16	R16	K18
      0x04402008,  //  0056  SUB	R16	R16	R8
      0x903E3810,  //  0057  SETMBR	R15	K28	R16
      0x883C011B,  //  0058  GETMBR	R15	R0	K27
      0x00400E06,  //  0059  ADD	R16	R7	R6
      0x08401C10,  //  005A  MUL	R16	R14	R16
      0x00401210,  //  005B  ADD	R16	R9	R16
      0x00402008,  //  005C  ADD	R16	R16	R8
      0x903E3A10,  //  005D  SETMBR	R15	K29	R16
      0x883C011E,  //  005E  GETMBR	R15	R0	K30
      0x0442040E,  //  005F  SUB	R16	K2	R14
      0x04440807,  //  0060  SUB	R17	R4	R7
      0x08402011,  //  0061  MUL	R16	R16	R17
      0x54460003,  //  0062  LDINT	R17	4
      0x0C402011,  //  0063  DIV	R16	R16	R17
      0x00401410,  //  0064  ADD	R16	R10	R16
      0x00402008,  //  0065  ADD	R16	R16	R8
      0x903E3810,  //  0066  SETMBR	R15	K28	R16
      0x883C011E,  //  0067  GETMBR	R15	R0	K30
      0x8840011B,  //  0068  GETMBR	R16	R0	K27
      0x8840211D,  //  0069  GETMBR	R16	R16	K29
      0x903E3A10,  //  006A  SETMBR	R15	K29	R16
      0xB83E0800,  //  006B  GETNGBL	R15	K4
      0x8C3C1F1F,  //  006C  GETMET	R15	R15	K31
      0x8844011B,  //  006D  GETMBR	R17	R0	K27
      0x8848011E,  //  006E  GETMBR	R18	R0	K30
      0x5C4C0200,  //  006F  MOVE	R19	R1
      0x8850010E,  //  0070  GETMBR	R20	R0	K14
      0x7C3C0A00,  //  0071  CALL	R15	5
      0x7001FFD2,  //  0072  JMP		#0046
      0x58340020,  //  0073  LDCONST	R13	K32
      0xAC340200,  //  0074  CATCH	R13	1	0
      0xB0080000,  //  0075  RAISE	2	R0	R0
      0xB8260800,  //  0076  GETNGBL	R9	K4
      0x88241321,  //  0077  GETMBR	R9	R9	K33
      0x80041200,  //  0078  RET	1	R9
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
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("percentage", -1756136011, 10),
    }),
    (be_nested_const_str("get_percentage", -1414483304, 14)),
    (be_nested_const_str("input", -103256197, 5)),
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
be_local_closure(init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_string("init", 380752755, 4),
    /* K1   */  be_nested_string("ancestor_design", 421545719, 15),
    /* K2   */  be_nested_string("get_design_cb", -825649242, 13),
    /* K3   */  be_nested_string("set_design_cb", 1469311634, 13),
    /* K4   */  be_nested_string("my_design_cb", -1173588798, 12),
    /* K5   */  be_nested_string("percentage", -1756136011, 10),
    /* K6   */  be_nested_string("p1", -1605446022, 2),
    /* K7   */  be_nested_string("lv_point", -174745506, 8),
    /* K8   */  be_nested_string("p2", -1622223641, 2),
    /* K9   */  be_nested_string("area", -1693507260, 4),
    /* K10  */  be_nested_string("lv_area", -1773816895, 7),
    /* K11  */  be_nested_string("line_dsc", -200476318, 8),
    /* K12  */  be_nested_string("lv_draw_line_dsc", -1872162060, 16),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[28]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x8C0C0102,  //  0007  GETMET	R3	R0	K2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x90020203,  //  0009  SETMBR	R0	K1	R3
      0x8C0C0103,  //  000A  GETMET	R3	R0	K3
      0x88140104,  //  000B  GETMBR	R5	R0	K4
      0x7C0C0400,  //  000C  CALL	R3	2
      0x540E0063,  //  000D  LDINT	R3	100
      0x90020A03,  //  000E  SETMBR	R0	K5	R3
      0xB80E0E00,  //  000F  GETNGBL	R3	K7
      0x7C0C0000,  //  0010  CALL	R3	0
      0x90020C03,  //  0011  SETMBR	R0	K6	R3
      0xB80E0E00,  //  0012  GETNGBL	R3	K7
      0x7C0C0000,  //  0013  CALL	R3	0
      0x90021003,  //  0014  SETMBR	R0	K8	R3
      0xB80E1400,  //  0015  GETNGBL	R3	K10
      0x7C0C0000,  //  0016  CALL	R3	0
      0x90021203,  //  0017  SETMBR	R0	K9	R3
      0xB80E1800,  //  0018  GETNGBL	R3	K12
      0x7C0C0000,  //  0019  CALL	R3	0
      0x90021603,  //  001A  SETMBR	R0	K11	R3
      0x80000000,  //  001B  RET	0
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
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("percentage", -1756136011, 10),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_string("invalidate", -1645232368, 10),
    }),
    (be_nested_const_str("set_percentage", -1342944572, 14)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[18]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x540E0004,  //  0001  LDINT	R3	5
      0x0C080403,  //  0002  DIV	R2	R2	R3
      0x540E0063,  //  0003  LDINT	R3	100
      0x240C0203,  //  0004  GT	R3	R1	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x54060063,  //  0006  LDINT	R1	100
      0x140C0301,  //  0007  LT	R3	R1	K1
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x58040001,  //  0009  LDCONST	R1	K1
      0x90020001,  //  000A  SETMBR	R0	K0	R1
      0x540E0004,  //  000B  LDINT	R3	5
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
** Solidified class: lv_signal_bars
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_bars,
    6,
    &be_class_lv_obj,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("p1", -1605446022, 2, -1), be_const_var(2) },
        { be_nested_key("my_design_cb", -1173588798, 12, -1), be_const_closure(my_design_cb_closure) },
        { be_nested_key("get_percentage", -1414483304, 14, -1), be_const_closure(get_percentage_closure) },
        { be_nested_key("init", 380752755, 4, 7), be_const_closure(init_closure) },
        { be_nested_key("set_percentage", -1342944572, 14, 0), be_const_closure(set_percentage_closure) },
        { be_nested_key("percentage", -1756136011, 10, 3), be_const_var(1) },
        { be_nested_key("area", -1693507260, 4, -1), be_const_var(4) },
        { be_nested_key("p2", -1622223641, 2, -1), be_const_var(3) },
        { be_nested_key("line_dsc", -200476318, 8, 1), be_const_var(5) },
        { be_nested_key("ancestor_design", 421545719, 15, -1), be_const_var(0) },
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