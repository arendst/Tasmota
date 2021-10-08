/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

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
be_local_closure(init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_string("_lvgl", -1605747813, 5),
    /* K1   */  be_nested_string("create_custom_widget", 1140594778, 20),
    /* K2   */  be_nested_string("percentage", -1756136011, 10),
    /* K3   */  be_nested_string("p1", -1605446022, 2),
    /* K4   */  be_nested_string("lv_point", -174745506, 8),
    /* K5   */  be_nested_string("p2", -1622223641, 2),
    /* K6   */  be_nested_string("area", -1693507260, 4),
    /* K7   */  be_nested_string("lv_area", -1773816895, 7),
    /* K8   */  be_nested_string("line_dsc", -200476318, 8),
    /* K9   */  be_nested_string("lv_draw_line_dsc", -1872162060, 16),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[20]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x540A0063,  //  0005  LDINT	R2	100
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0xB80A0800,  //  0007  GETNGBL	R2	K4
      0x7C080000,  //  0008  CALL	R2	0
      0x90020602,  //  0009  SETMBR	R0	K3	R2
      0xB80A0800,  //  000A  GETNGBL	R2	K4
      0x7C080000,  //  000B  CALL	R2	0
      0x90020A02,  //  000C  SETMBR	R0	K5	R2
      0xB80A0E00,  //  000D  GETNGBL	R2	K7
      0x7C080000,  //  000E  CALL	R2	0
      0x90020C02,  //  000F  SETMBR	R0	K6	R2
      0xB80A1200,  //  0010  GETNGBL	R2	K9
      0x7C080000,  //  0011  CALL	R2	0
      0x90021002,  //  0012  SETMBR	R0	K8	R2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_event
********************************************************************/
be_local_closure(widget_event,   /* name */
  be_nested_proto(
    23,                          /* nstack */
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
    ( &(const bvalue[38]) {     /* constants */
    /* K0   */  be_nested_string("lv", 1529997255, 2),
    /* K1   */  be_nested_string("obj_event_base", 1624064363, 14),
    /* K2   */  be_nested_string("RES_OK", 1233817284, 6),
    /* K3   */  be_nested_string("code", -114201356, 4),
    /* K4   */  be_nested_string("get_height", -723211773, 10),
    /* K5   */  be_nested_string("get_width", -1001549996, 9),
    /* K6   */  be_const_int(3),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_string("EVENT_DRAW_MAIN", 1955620614, 15),
    /* K9   */  be_nested_string("lv_area", -1773816895, 7),
    /* K10  */  be_nested_string("param", 1309554226, 5),
    /* K11  */  be_nested_string("get_coords", 1044089006, 10),
    /* K12  */  be_nested_string("area", -1693507260, 4),
    /* K13  */  be_nested_string("x1", 274927234, 2),
    /* K14  */  be_nested_string("y1", -1939865569, 2),
    /* K15  */  be_nested_string("draw_line_dsc_init", -428273650, 18),
    /* K16  */  be_nested_string("line_dsc", -200476318, 8),
    /* K17  */  be_nested_string("init_draw_line_dsc", -1787031256, 18),
    /* K18  */  be_nested_string("PART_MAIN", -1821475788, 9),
    /* K19  */  be_nested_string("round_start", -1345482912, 11),
    /* K20  */  be_const_int(1),
    /* K21  */  be_nested_string("round_end", 985288225, 9),
    /* K22  */  be_nested_string("width", -1786286561, 5),
    /* K23  */  be_nested_string("get_style_line_color", 805371932, 20),
    /* K24  */  be_nested_string("STATE_DEFAULT", 712406428, 13),
    /* K25  */  be_nested_string("get_style_bg_color", 964794381, 18),
    /* K26  */  be_nested_string("event_send", 598925582, 10),
    /* K27  */  be_nested_string("EVENT_DRAW_PART_BEGIN", -903102272, 21),
    /* K28  */  be_const_int(0),
    /* K29  */  be_nested_string("color", 1031692888, 5),
    /* K30  */  be_nested_string("percentage", -1756136011, 10),
    /* K31  */  be_nested_string("p1", -1605446022, 2),
    /* K32  */  be_nested_string("y", -66302220, 1),
    /* K33  */  be_nested_string("x", -49524601, 1),
    /* K34  */  be_nested_string("p2", -1622223641, 2),
    /* K35  */  be_nested_string("draw_line", 1634465686, 9),
    /* K36  */  be_nested_string("stop_iteration", -121173395, 14),
    /* K37  */  be_nested_string("EVENT_DRAW_PART_END", -993342004, 19),
    }),
    (be_nested_const_str("widget_event", 1951408186, 12)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[137]) {  /* code */
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
      0x782A0068,  //  001E  JMPF	R10	#0088
      0xB82A1200,  //  001F  GETNGBL	R10	K9
      0x882C050A,  //  0020  GETMBR	R11	R2	K10
      0x7C280200,  //  0021  CALL	R10	1
      0x8C2C010B,  //  0022  GETMET	R11	R0	K11
      0x8834010C,  //  0023  GETMBR	R13	R0	K12
      0x7C2C0400,  //  0024  CALL	R11	2
      0x882C010C,  //  0025  GETMBR	R11	R0	K12
      0x882C170D,  //  0026  GETMBR	R11	R11	K13
      0x8830010C,  //  0027  GETMBR	R12	R0	K12
      0x8830190E,  //  0028  GETMBR	R12	R12	K14
      0xB8360000,  //  0029  GETNGBL	R13	K0
      0x8C341B0F,  //  002A  GETMET	R13	R13	K15
      0x883C0110,  //  002B  GETMBR	R15	R0	K16
      0x7C340400,  //  002C  CALL	R13	2
      0x8C340111,  //  002D  GETMET	R13	R0	K17
      0xB83E0000,  //  002E  GETNGBL	R15	K0
      0x883C1F12,  //  002F  GETMBR	R15	R15	K18
      0x88400110,  //  0030  GETMBR	R16	R0	K16
      0x7C340600,  //  0031  CALL	R13	3
      0x88340110,  //  0032  GETMBR	R13	R0	K16
      0x90362714,  //  0033  SETMBR	R13	K19	K20
      0x88340110,  //  0034  GETMBR	R13	R0	K16
      0x90362B14,  //  0035  SETMBR	R13	K21	K20
      0x88340110,  //  0036  GETMBR	R13	R0	K16
      0x90362C08,  //  0037  SETMBR	R13	K22	R8
      0x8C340117,  //  0038  GETMET	R13	R0	K23
      0xB83E0000,  //  0039  GETNGBL	R15	K0
      0x883C1F12,  //  003A  GETMBR	R15	R15	K18
      0xB8420000,  //  003B  GETNGBL	R16	K0
      0x88402118,  //  003C  GETMBR	R16	R16	K24
      0x303C1E10,  //  003D  OR	R15	R15	R16
      0x7C340400,  //  003E  CALL	R13	2
      0x8C380119,  //  003F  GETMET	R14	R0	K25
      0xB8420000,  //  0040  GETNGBL	R16	K0
      0x88402112,  //  0041  GETMBR	R16	R16	K18
      0xB8460000,  //  0042  GETNGBL	R17	K0
      0x88442318,  //  0043  GETMBR	R17	R17	K24
      0x30402011,  //  0044  OR	R16	R16	R17
      0x7C380400,  //  0045  CALL	R14	2
      0xB83E0000,  //  0046  GETNGBL	R15	K0
      0x8C3C1F1A,  //  0047  GETMET	R15	R15	K26
      0x5C440000,  //  0048  MOVE	R17	R0
      0xB84A0000,  //  0049  GETNGBL	R18	K0
      0x8848251B,  //  004A  GETMBR	R18	R18	K27
      0x884C0110,  //  004B  GETMBR	R19	R0	K16
      0x7C3C0800,  //  004C  CALL	R15	4
      0x603C0010,  //  004D  GETGBL	R15	G16
      0x40423906,  //  004E  CONNECT	R16	K28	K6
      0x7C3C0200,  //  004F  CALL	R15	1
      0xA802002C,  //  0050  EXBLK	0	#007E
      0x5C401E00,  //  0051  MOVE	R16	R15
      0x7C400000,  //  0052  CALL	R16	0
      0x88440110,  //  0053  GETMBR	R17	R0	K16
      0x8848011E,  //  0054  GETMBR	R18	R0	K30
      0x004C2114,  //  0055  ADD	R19	R16	K20
      0x54520013,  //  0056  LDINT	R20	20
      0x084C2614,  //  0057  MUL	R19	R19	R20
      0x28482413,  //  0058  GE	R18	R18	R19
      0x784A0001,  //  0059  JMPF	R18	#005C
      0x5C481A00,  //  005A  MOVE	R18	R13
      0x70020000,  //  005B  JMP		#005D
      0x5C481C00,  //  005C  MOVE	R18	R14
      0x90463A12,  //  005D  SETMBR	R17	K29	R18
      0x8844011F,  //  005E  GETMBR	R17	R0	K31
      0x00481805,  //  005F  ADD	R18	R12	R5
      0x04482514,  //  0060  SUB	R18	R18	K20
      0x04482409,  //  0061  SUB	R18	R18	R9
      0x90464012,  //  0062  SETMBR	R17	K32	R18
      0x8844011F,  //  0063  GETMBR	R17	R0	K31
      0x00481007,  //  0064  ADD	R18	R8	R7
      0x08482012,  //  0065  MUL	R18	R16	R18
      0x00481612,  //  0066  ADD	R18	R11	R18
      0x00482409,  //  0067  ADD	R18	R18	R9
      0x90464212,  //  0068  SETMBR	R17	K33	R18
      0x88440122,  //  0069  GETMBR	R17	R0	K34
      0x044A0C10,  //  006A  SUB	R18	K6	R16
      0x044C0A08,  //  006B  SUB	R19	R5	R8
      0x08482413,  //  006C  MUL	R18	R18	R19
      0x544E0003,  //  006D  LDINT	R19	4
      0x0C482413,  //  006E  DIV	R18	R18	R19
      0x00481812,  //  006F  ADD	R18	R12	R18
      0x00482409,  //  0070  ADD	R18	R18	R9
      0x90464012,  //  0071  SETMBR	R17	K32	R18
      0x88440122,  //  0072  GETMBR	R17	R0	K34
      0x8848011F,  //  0073  GETMBR	R18	R0	K31
      0x88482521,  //  0074  GETMBR	R18	R18	K33
      0x90464212,  //  0075  SETMBR	R17	K33	R18
      0xB8460000,  //  0076  GETNGBL	R17	K0
      0x8C442323,  //  0077  GETMET	R17	R17	K35
      0x884C011F,  //  0078  GETMBR	R19	R0	K31
      0x88500122,  //  0079  GETMBR	R20	R0	K34
      0x5C541400,  //  007A  MOVE	R21	R10
      0x88580110,  //  007B  GETMBR	R22	R0	K16
      0x7C440A00,  //  007C  CALL	R17	5
      0x7001FFD2,  //  007D  JMP		#0051
      0x583C0024,  //  007E  LDCONST	R15	K36
      0xAC3C0200,  //  007F  CATCH	R15	1	0
      0xB0080000,  //  0080  RAISE	2	R0	R0
      0xB83E0000,  //  0081  GETNGBL	R15	K0
      0x8C3C1F1A,  //  0082  GETMET	R15	R15	K26
      0x5C440000,  //  0083  MOVE	R17	R0
      0xB84A0000,  //  0084  GETNGBL	R18	K0
      0x88482525,  //  0085  GETMBR	R18	R18	K37
      0x884C0110,  //  0086  GETMBR	R19	R0	K16
      0x7C3C0800,  //  0087  CALL	R15	4
      0x80000000,  //  0088  RET	0
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
** Solidified class: lv_signal_bars
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_bars,
    5,
    &be_class_lv_obj,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("percentage", -1756136011, 10, 4), be_const_var(0) },
        { be_nested_key("p1", -1605446022, 2, 3), be_const_var(1) },
        { be_nested_key("p2", -1622223641, 2, -1), be_const_var(2) },
        { be_nested_key("area", -1693507260, 4, -1), be_const_var(3) },
        { be_nested_key("line_dsc", -200476318, 8, -1), be_const_var(4) },
        { be_nested_key("set_percentage", -1342944572, 14, -1), be_const_closure(set_percentage_closure) },
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(init_closure) },
        { be_nested_key("widget_event", 1951408186, 12, -1), be_const_closure(widget_event_closure) },
        { be_nested_key("get_percentage", -1414483304, 14, 5), be_const_closure(get_percentage_closure) },
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