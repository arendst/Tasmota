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
    28,                          /* nstack */
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
    ( &(const bvalue[36]) {     /* constants */
    /* K0   */  be_nested_string("lv", 1529997255, 2),
    /* K1   */  be_nested_string("obj_event_base", 1624064363, 14),
    /* K2   */  be_nested_string("RES_OK", 1233817284, 6),
    /* K3   */  be_nested_string("code", -114201356, 4),
    /* K4   */  be_nested_string("math", -293037681, 4),
    /* K5   */  be_nested_string("get_height", -723211773, 10),
    /* K6   */  be_nested_string("get_width", -1001549996, 9),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_string("EVENT_DRAW_MAIN", 1955620614, 15),
    /* K10  */  be_nested_string("lv_area", -1773816895, 7),
    /* K11  */  be_nested_string("param", 1309554226, 5),
    /* K12  */  be_nested_string("get_coords", 1044089006, 10),
    /* K13  */  be_nested_string("area", -1693507260, 4),
    /* K14  */  be_nested_string("x1", 274927234, 2),
    /* K15  */  be_nested_string("y1", -1939865569, 2),
    /* K16  */  be_nested_string("draw_line_dsc_init", -428273650, 18),
    /* K17  */  be_nested_string("line_dsc", -200476318, 8),
    /* K18  */  be_nested_string("init_draw_line_dsc", -1787031256, 18),
    /* K19  */  be_nested_string("PART_MAIN", -1821475788, 9),
    /* K20  */  be_nested_string("round_start", -1345482912, 11),
    /* K21  */  be_const_int(1),
    /* K22  */  be_nested_string("round_end", 985288225, 9),
    /* K23  */  be_nested_string("width", -1786286561, 5),
    /* K24  */  be_nested_string("get_style_line_color", 805371932, 20),
    /* K25  */  be_nested_string("STATE_DEFAULT", 712406428, 13),
    /* K26  */  be_nested_string("get_style_bg_color", 964794381, 18),
    /* K27  */  be_nested_string("deg", -967213025, 3),
    /* K28  */  be_nested_string("acos", 1006755615, 4),
    /* K29  */  be_nested_string("p1", -1605446022, 2),
    /* K30  */  be_nested_string("x", -49524601, 1),
    /* K31  */  be_nested_string("y", -66302220, 1),
    /* K32  */  be_nested_string("color", 1031692888, 5),
    /* K33  */  be_nested_string("percentage", -1756136011, 10),
    /* K34  */  be_nested_string("draw_arc", 1828251676, 8),
    /* K35  */  be_const_int(0),
    }),
    (be_nested_const_str("widget_event", 1951408186, 12)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[181]) {  /* code */
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
      0x782E0094,  //  001E  JMPF	R11	#00B4
      0xB82E1400,  //  001F  GETNGBL	R11	K10
      0x8830050B,  //  0020  GETMBR	R12	R2	K11
      0x7C2C0200,  //  0021  CALL	R11	1
      0x8C30010C,  //  0022  GETMET	R12	R0	K12
      0x8838010D,  //  0023  GETMBR	R14	R0	K13
      0x7C300400,  //  0024  CALL	R12	2
      0x8830010D,  //  0025  GETMBR	R12	R0	K13
      0x8830190E,  //  0026  GETMBR	R12	R12	K14
      0x8834010D,  //  0027  GETMBR	R13	R0	K13
      0x88341B0F,  //  0028  GETMBR	R13	R13	K15
      0xB83A0000,  //  0029  GETNGBL	R14	K0
      0x8C381D10,  //  002A  GETMET	R14	R14	K16
      0x88400111,  //  002B  GETMBR	R16	R0	K17
      0x7C380400,  //  002C  CALL	R14	2
      0x8C380112,  //  002D  GETMET	R14	R0	K18
      0xB8420000,  //  002E  GETNGBL	R16	K0
      0x88402113,  //  002F  GETMBR	R16	R16	K19
      0x88440111,  //  0030  GETMBR	R17	R0	K17
      0x7C380600,  //  0031  CALL	R14	3
      0x88380111,  //  0032  GETMBR	R14	R0	K17
      0x903A2915,  //  0033  SETMBR	R14	K20	K21
      0x88380111,  //  0034  GETMBR	R14	R0	K17
      0x903A2D15,  //  0035  SETMBR	R14	K22	K21
      0x88380111,  //  0036  GETMBR	R14	R0	K17
      0x083C1308,  //  0037  MUL	R15	R9	K8
      0x003C1F15,  //  0038  ADD	R15	R15	K21
      0x54420003,  //  0039  LDINT	R16	4
      0x0C3C1E10,  //  003A  DIV	R15	R15	R16
      0x903A2E0F,  //  003B  SETMBR	R14	K23	R15
      0x8C380118,  //  003C  GETMET	R14	R0	K24
      0xB8420000,  //  003D  GETNGBL	R16	K0
      0x88402113,  //  003E  GETMBR	R16	R16	K19
      0xB8460000,  //  003F  GETNGBL	R17	K0
      0x88442319,  //  0040  GETMBR	R17	R17	K25
      0x30402011,  //  0041  OR	R16	R16	R17
      0x7C380400,  //  0042  CALL	R14	2
      0x8C3C011A,  //  0043  GETMET	R15	R0	K26
      0xB8460000,  //  0044  GETNGBL	R17	K0
      0x88442313,  //  0045  GETMBR	R17	R17	K19
      0xB84A0000,  //  0046  GETNGBL	R18	K0
      0x88482519,  //  0047  GETMBR	R18	R18	K25
      0x30442212,  //  0048  OR	R17	R17	R18
      0x7C3C0400,  //  0049  CALL	R15	2
      0x04400C09,  //  004A  SUB	R16	R6	R9
      0x0C440F07,  //  004B  DIV	R17	R7	K7
      0x0444220A,  //  004C  SUB	R17	R17	R10
      0x60480009,  //  004D  GETGBL	R18	G9
      0x544E0059,  //  004E  LDINT	R19	90
      0x8C50091B,  //  004F  GETMET	R20	R4	K27
      0x8C58091C,  //  0050  GETMET	R22	R4	K28
      0x6060000A,  //  0051  GETGBL	R24	G10
      0x5C642200,  //  0052  MOVE	R25	R17
      0x7C600200,  //  0053  CALL	R24	1
      0x6064000A,  //  0054  GETGBL	R25	G10
      0x5C682000,  //  0055  MOVE	R26	R16
      0x7C640200,  //  0056  CALL	R25	1
      0x0C603019,  //  0057  DIV	R24	R24	R25
      0x7C580400,  //  0058  CALL	R22	2
      0x7C500400,  //  0059  CALL	R20	2
      0x044C2614,  //  005A  SUB	R19	R19	R20
      0x7C480200,  //  005B  CALL	R18	1
      0x544E002C,  //  005C  LDINT	R19	45
      0x244C2413,  //  005D  GT	R19	R18	R19
      0x784E0000,  //  005E  JMPF	R19	#0060
      0x544A002C,  //  005F  LDINT	R18	45
      0x884C011D,  //  0060  GETMBR	R19	R0	K29
      0x0C500F07,  //  0061  DIV	R20	R7	K7
      0x00501814,  //  0062  ADD	R20	R12	R20
      0x904E3C14,  //  0063  SETMBR	R19	K30	R20
      0x884C011D,  //  0064  GETMBR	R19	R0	K29
      0x00501A06,  //  0065  ADD	R20	R13	R6
      0x04502915,  //  0066  SUB	R20	R20	K21
      0x0450280A,  //  0067  SUB	R20	R20	R10
      0x904E3E14,  //  0068  SETMBR	R19	K31	R20
      0x884C0111,  //  0069  GETMBR	R19	R0	K17
      0x88500121,  //  006A  GETMBR	R20	R0	K33
      0x54560018,  //  006B  LDINT	R21	25
      0x28502815,  //  006C  GE	R20	R20	R21
      0x78520001,  //  006D  JMPF	R20	#0070
      0x5C501C00,  //  006E  MOVE	R20	R14
      0x70020000,  //  006F  JMP		#0071
      0x5C501E00,  //  0070  MOVE	R20	R15
      0x904E4014,  //  0071  SETMBR	R19	K32	R20
      0xB84E0000,  //  0072  GETNGBL	R19	K0
      0x8C4C2722,  //  0073  GETMET	R19	R19	K34
      0x8854011D,  //  0074  GETMBR	R21	R0	K29
      0x88542B1E,  //  0075  GETMBR	R21	R21	K30
      0x8858011D,  //  0076  GETMBR	R22	R0	K29
      0x88582D1F,  //  0077  GETMBR	R22	R22	K31
      0x005C1208,  //  0078  ADD	R23	R9	R8
      0x085E4617,  //  0079  MUL	R23	K35	R23
      0x005C2E0A,  //  007A  ADD	R23	R23	R10
      0x58600023,  //  007B  LDCONST	R24	K35
      0x54660167,  //  007C  LDINT	R25	360
      0x5C681600,  //  007D  MOVE	R26	R11
      0x886C0111,  //  007E  GETMBR	R27	R0	K17
      0x7C4C1000,  //  007F  CALL	R19	8
      0x884C0111,  //  0080  GETMBR	R19	R0	K17
      0x88500121,  //  0081  GETMBR	R20	R0	K33
      0x54560031,  //  0082  LDINT	R21	50
      0x28502815,  //  0083  GE	R20	R20	R21
      0x78520001,  //  0084  JMPF	R20	#0087
      0x5C501C00,  //  0085  MOVE	R20	R14
      0x70020000,  //  0086  JMP		#0088
      0x5C501E00,  //  0087  MOVE	R20	R15
      0x904E4014,  //  0088  SETMBR	R19	K32	R20
      0xB84E0000,  //  0089  GETNGBL	R19	K0
      0x8C4C2722,  //  008A  GETMET	R19	R19	K34
      0x8854011D,  //  008B  GETMBR	R21	R0	K29
      0x88542B1E,  //  008C  GETMBR	R21	R21	K30
      0x8858011D,  //  008D  GETMBR	R22	R0	K29
      0x88582D1F,  //  008E  GETMBR	R22	R22	K31
      0x005C1208,  //  008F  ADD	R23	R9	R8
      0x085E2A17,  //  0090  MUL	R23	K21	R23
      0x005C2E0A,  //  0091  ADD	R23	R23	R10
      0x045C2F15,  //  0092  SUB	R23	R23	K21
      0x5462010D,  //  0093  LDINT	R24	270
      0x04603012,  //  0094  SUB	R24	R24	R18
      0x5466010D,  //  0095  LDINT	R25	270
      0x00643212,  //  0096  ADD	R25	R25	R18
      0x5C681600,  //  0097  MOVE	R26	R11
      0x886C0111,  //  0098  GETMBR	R27	R0	K17
      0x7C4C1000,  //  0099  CALL	R19	8
      0x884C0111,  //  009A  GETMBR	R19	R0	K17
      0x88500121,  //  009B  GETMBR	R20	R0	K33
      0x5456004A,  //  009C  LDINT	R21	75
      0x28502815,  //  009D  GE	R20	R20	R21
      0x78520001,  //  009E  JMPF	R20	#00A1
      0x5C501C00,  //  009F  MOVE	R20	R14
      0x70020000,  //  00A0  JMP		#00A2
      0x5C501E00,  //  00A1  MOVE	R20	R15
      0x904E4014,  //  00A2  SETMBR	R19	K32	R20
      0xB84E0000,  //  00A3  GETNGBL	R19	K0
      0x8C4C2722,  //  00A4  GETMET	R19	R19	K34
      0x8854011D,  //  00A5  GETMBR	R21	R0	K29
      0x88542B1E,  //  00A6  GETMBR	R21	R21	K30
      0x8858011D,  //  00A7  GETMBR	R22	R0	K29
      0x88582D1F,  //  00A8  GETMBR	R22	R22	K31
      0x005C1208,  //  00A9  ADD	R23	R9	R8
      0x085E0E17,  //  00AA  MUL	R23	K7	R23
      0x005C2E0A,  //  00AB  ADD	R23	R23	R10
      0x045C2F07,  //  00AC  SUB	R23	R23	K7
      0x5462010D,  //  00AD  LDINT	R24	270
      0x04603012,  //  00AE  SUB	R24	R24	R18
      0x5466010D,  //  00AF  LDINT	R25	270
      0x00643212,  //  00B0  ADD	R25	R25	R18
      0x5C681600,  //  00B1  MOVE	R26	R11
      0x886C0111,  //  00B2  GETMBR	R27	R0	K17
      0x7C4C1000,  //  00B3  CALL	R19	8
      0x80000000,  //  00B4  RET	0
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
** Solidified class: lv_signal_arcs
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_arcs,
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
    (be_nested_const_str("lv_signal_arcs", -1455810308, 14))
);
/*******************************************************************/

void be_load_lv_signal_arcs_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_signal_arcs);
    be_setglobal(vm, "lv_signal_arcs");
    be_pop(vm, 1);
}

#endif // USE_LVGL