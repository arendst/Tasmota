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
    26,                          /* nstack */
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
    ( &(const bvalue[35]) {     /* constants */
    /* K0   */  be_nested_string("math", -293037681, 4),
    /* K1   */  be_nested_string("get_height", -723211773, 10),
    /* K2   */  be_nested_string("get_width", -1001549996, 9),
    /* K3   */  be_const_int(2),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_string("lv", 1529997255, 2),
    /* K6   */  be_nested_string("DESIGN_COVER_CHK", -1298406708, 16),
    /* K7   */  be_nested_string("ancestor_design", 421545719, 15),
    /* K8   */  be_nested_string("call", -1276017495, 4),
    /* K9   */  be_nested_string("DESIGN_DRAW_MAIN", -904475754, 16),
    /* K10  */  be_nested_string("get_coords", 1044089006, 10),
    /* K11  */  be_nested_string("area", -1693507260, 4),
    /* K12  */  be_nested_string("x1", 274927234, 2),
    /* K13  */  be_nested_string("y1", -1939865569, 2),
    /* K14  */  be_nested_string("draw_line_dsc_init", -428273650, 18),
    /* K15  */  be_nested_string("line_dsc", -200476318, 8),
    /* K16  */  be_nested_string("init_draw_line_dsc", -1787031256, 18),
    /* K17  */  be_nested_string("OBJ_PART_MAIN", 658062838, 13),
    /* K18  */  be_nested_string("round_start", -1345482912, 11),
    /* K19  */  be_const_int(1),
    /* K20  */  be_nested_string("round_end", 985288225, 9),
    /* K21  */  be_nested_string("width", -1786286561, 5),
    /* K22  */  be_nested_string("get_style_line_color", 805371932, 20),
    /* K23  */  be_nested_string("STATE_DEFAULT", 712406428, 13),
    /* K24  */  be_nested_string("get_style_bg_color", 964794381, 18),
    /* K25  */  be_nested_string("deg", -967213025, 3),
    /* K26  */  be_nested_string("acos", 1006755615, 4),
    /* K27  */  be_nested_string("p1", -1605446022, 2),
    /* K28  */  be_nested_string("x", -49524601, 1),
    /* K29  */  be_nested_string("y", -66302220, 1),
    /* K30  */  be_nested_string("color", 1031692888, 5),
    /* K31  */  be_nested_string("percentage", -1756136011, 10),
    /* K32  */  be_nested_string("draw_arc", 1828251676, 8),
    /* K33  */  be_const_int(0),
    /* K34  */  be_nested_string("DESIGN_RES_OK", -1265307123, 13),
    }),
    (be_nested_const_str("my_design_cb", -1173588798, 12)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[179]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x84100000,  //  0001  CLOSURE	R4	P0
      0x8C140101,  //  0002  GETMET	R5	R0	K1
      0x7C140200,  //  0003  CALL	R5	1
      0x8C180102,  //  0004  GETMET	R6	R0	K2
      0x7C180200,  //  0005  CALL	R6	1
      0x5C1C0800,  //  0006  MOVE	R7	R4
      0x54220007,  //  0007  LDINT	R8	8
      0x0C200A08,  //  0008  DIV	R8	R5	R8
      0x7C1C0200,  //  0009  CALL	R7	1
      0x5C200800,  //  000A  MOVE	R8	R4
      0x08240F03,  //  000B  MUL	R9	R7	K3
      0x04240A09,  //  000C  SUB	R9	R5	R9
      0x0C241304,  //  000D  DIV	R9	R9	K4
      0x7C200200,  //  000E  CALL	R8	1
      0x0C241103,  //  000F  DIV	R9	R8	K3
      0xB82A0A00,  //  0010  GETNGBL	R10	K5
      0x88281506,  //  0011  GETMBR	R10	R10	K6
      0x1C28040A,  //  0012  EQ	R10	R2	R10
      0x782A0007,  //  0013  JMPF	R10	#001C
      0x88280107,  //  0014  GETMBR	R10	R0	K7
      0x8C281508,  //  0015  GETMET	R10	R10	K8
      0x5C300000,  //  0016  MOVE	R12	R0
      0x5C340200,  //  0017  MOVE	R13	R1
      0x5C380400,  //  0018  MOVE	R14	R2
      0x7C280800,  //  0019  CALL	R10	4
      0x80041400,  //  001A  RET	1	R10
      0x70020093,  //  001B  JMP		#00B0
      0xB82A0A00,  //  001C  GETNGBL	R10	K5
      0x88281509,  //  001D  GETMBR	R10	R10	K9
      0x1C28040A,  //  001E  EQ	R10	R2	R10
      0x782A008F,  //  001F  JMPF	R10	#00B0
      0x8C28010A,  //  0020  GETMET	R10	R0	K10
      0x8830010B,  //  0021  GETMBR	R12	R0	K11
      0x7C280400,  //  0022  CALL	R10	2
      0x8828010B,  //  0023  GETMBR	R10	R0	K11
      0x8828150C,  //  0024  GETMBR	R10	R10	K12
      0x882C010B,  //  0025  GETMBR	R11	R0	K11
      0x882C170D,  //  0026  GETMBR	R11	R11	K13
      0xB8320A00,  //  0027  GETNGBL	R12	K5
      0x8C30190E,  //  0028  GETMET	R12	R12	K14
      0x8838010F,  //  0029  GETMBR	R14	R0	K15
      0x7C300400,  //  002A  CALL	R12	2
      0x8C300110,  //  002B  GETMET	R12	R0	K16
      0xB83A0A00,  //  002C  GETNGBL	R14	K5
      0x88381D11,  //  002D  GETMBR	R14	R14	K17
      0x883C010F,  //  002E  GETMBR	R15	R0	K15
      0x7C300600,  //  002F  CALL	R12	3
      0x8830010F,  //  0030  GETMBR	R12	R0	K15
      0x90322513,  //  0031  SETMBR	R12	K18	K19
      0x8830010F,  //  0032  GETMBR	R12	R0	K15
      0x90322913,  //  0033  SETMBR	R12	K20	K19
      0x8830010F,  //  0034  GETMBR	R12	R0	K15
      0x08341104,  //  0035  MUL	R13	R8	K4
      0x00341B13,  //  0036  ADD	R13	R13	K19
      0x543A0003,  //  0037  LDINT	R14	4
      0x0C341A0E,  //  0038  DIV	R13	R13	R14
      0x90322A0D,  //  0039  SETMBR	R12	K21	R13
      0x8C300116,  //  003A  GETMET	R12	R0	K22
      0xB83A0A00,  //  003B  GETNGBL	R14	K5
      0x88381D11,  //  003C  GETMBR	R14	R14	K17
      0xB83E0A00,  //  003D  GETNGBL	R15	K5
      0x883C1F17,  //  003E  GETMBR	R15	R15	K23
      0x7C300600,  //  003F  CALL	R12	3
      0x8C340118,  //  0040  GETMET	R13	R0	K24
      0xB83E0A00,  //  0041  GETNGBL	R15	K5
      0x883C1F11,  //  0042  GETMBR	R15	R15	K17
      0xB8420A00,  //  0043  GETNGBL	R16	K5
      0x88402117,  //  0044  GETMBR	R16	R16	K23
      0x7C340600,  //  0045  CALL	R13	3
      0x04380A08,  //  0046  SUB	R14	R5	R8
      0x0C3C0D03,  //  0047  DIV	R15	R6	K3
      0x043C1E09,  //  0048  SUB	R15	R15	R9
      0x60400009,  //  0049  GETGBL	R16	G9
      0x54460059,  //  004A  LDINT	R17	90
      0x8C480719,  //  004B  GETMET	R18	R3	K25
      0x8C50071A,  //  004C  GETMET	R20	R3	K26
      0x6058000A,  //  004D  GETGBL	R22	G10
      0x5C5C1E00,  //  004E  MOVE	R23	R15
      0x7C580200,  //  004F  CALL	R22	1
      0x605C000A,  //  0050  GETGBL	R23	G10
      0x5C601C00,  //  0051  MOVE	R24	R14
      0x7C5C0200,  //  0052  CALL	R23	1
      0x0C582C17,  //  0053  DIV	R22	R22	R23
      0x7C500400,  //  0054  CALL	R20	2
      0x7C480400,  //  0055  CALL	R18	2
      0x04442212,  //  0056  SUB	R17	R17	R18
      0x7C400200,  //  0057  CALL	R16	1
      0x5446002C,  //  0058  LDINT	R17	45
      0x24442011,  //  0059  GT	R17	R16	R17
      0x78460000,  //  005A  JMPF	R17	#005C
      0x5442002C,  //  005B  LDINT	R16	45
      0x8844011B,  //  005C  GETMBR	R17	R0	K27
      0x0C480D03,  //  005D  DIV	R18	R6	K3
      0x00481412,  //  005E  ADD	R18	R10	R18
      0x90463812,  //  005F  SETMBR	R17	K28	R18
      0x8844011B,  //  0060  GETMBR	R17	R0	K27
      0x00481605,  //  0061  ADD	R18	R11	R5
      0x04482513,  //  0062  SUB	R18	R18	K19
      0x04482409,  //  0063  SUB	R18	R18	R9
      0x90463A12,  //  0064  SETMBR	R17	K29	R18
      0x8844010F,  //  0065  GETMBR	R17	R0	K15
      0x8848011F,  //  0066  GETMBR	R18	R0	K31
      0x544E0018,  //  0067  LDINT	R19	25
      0x28482413,  //  0068  GE	R18	R18	R19
      0x784A0001,  //  0069  JMPF	R18	#006C
      0x5C481800,  //  006A  MOVE	R18	R12
      0x70020000,  //  006B  JMP		#006D
      0x5C481A00,  //  006C  MOVE	R18	R13
      0x90463C12,  //  006D  SETMBR	R17	K30	R18
      0xB8460A00,  //  006E  GETNGBL	R17	K5
      0x8C442320,  //  006F  GETMET	R17	R17	K32
      0x884C011B,  //  0070  GETMBR	R19	R0	K27
      0x884C271C,  //  0071  GETMBR	R19	R19	K28
      0x8850011B,  //  0072  GETMBR	R20	R0	K27
      0x8850291D,  //  0073  GETMBR	R20	R20	K29
      0x00541007,  //  0074  ADD	R21	R8	R7
      0x08564215,  //  0075  MUL	R21	K33	R21
      0x00542A09,  //  0076  ADD	R21	R21	R9
      0x58580021,  //  0077  LDCONST	R22	K33
      0x545E0167,  //  0078  LDINT	R23	360
      0x5C600200,  //  0079  MOVE	R24	R1
      0x8864010F,  //  007A  GETMBR	R25	R0	K15
      0x7C441000,  //  007B  CALL	R17	8
      0x8844010F,  //  007C  GETMBR	R17	R0	K15
      0x8848011F,  //  007D  GETMBR	R18	R0	K31
      0x544E0031,  //  007E  LDINT	R19	50
      0x28482413,  //  007F  GE	R18	R18	R19
      0x784A0001,  //  0080  JMPF	R18	#0083
      0x5C481800,  //  0081  MOVE	R18	R12
      0x70020000,  //  0082  JMP		#0084
      0x5C481A00,  //  0083  MOVE	R18	R13
      0x90463C12,  //  0084  SETMBR	R17	K30	R18
      0xB8460A00,  //  0085  GETNGBL	R17	K5
      0x8C442320,  //  0086  GETMET	R17	R17	K32
      0x884C011B,  //  0087  GETMBR	R19	R0	K27
      0x884C271C,  //  0088  GETMBR	R19	R19	K28
      0x8850011B,  //  0089  GETMBR	R20	R0	K27
      0x8850291D,  //  008A  GETMBR	R20	R20	K29
      0x00541007,  //  008B  ADD	R21	R8	R7
      0x08562615,  //  008C  MUL	R21	K19	R21
      0x00542A09,  //  008D  ADD	R21	R21	R9
      0x04542B13,  //  008E  SUB	R21	R21	K19
      0x545A010D,  //  008F  LDINT	R22	270
      0x04582C10,  //  0090  SUB	R22	R22	R16
      0x545E010D,  //  0091  LDINT	R23	270
      0x005C2E10,  //  0092  ADD	R23	R23	R16
      0x5C600200,  //  0093  MOVE	R24	R1
      0x8864010F,  //  0094  GETMBR	R25	R0	K15
      0x7C441000,  //  0095  CALL	R17	8
      0x8844010F,  //  0096  GETMBR	R17	R0	K15
      0x8848011F,  //  0097  GETMBR	R18	R0	K31
      0x544E004A,  //  0098  LDINT	R19	75
      0x28482413,  //  0099  GE	R18	R18	R19
      0x784A0001,  //  009A  JMPF	R18	#009D
      0x5C481800,  //  009B  MOVE	R18	R12
      0x70020000,  //  009C  JMP		#009E
      0x5C481A00,  //  009D  MOVE	R18	R13
      0x90463C12,  //  009E  SETMBR	R17	K30	R18
      0xB8460A00,  //  009F  GETNGBL	R17	K5
      0x8C442320,  //  00A0  GETMET	R17	R17	K32
      0x884C011B,  //  00A1  GETMBR	R19	R0	K27
      0x884C271C,  //  00A2  GETMBR	R19	R19	K28
      0x8850011B,  //  00A3  GETMBR	R20	R0	K27
      0x8850291D,  //  00A4  GETMBR	R20	R20	K29
      0x00541007,  //  00A5  ADD	R21	R8	R7
      0x08560615,  //  00A6  MUL	R21	K3	R21
      0x00542A09,  //  00A7  ADD	R21	R21	R9
      0x04542B03,  //  00A8  SUB	R21	R21	K3
      0x545A010D,  //  00A9  LDINT	R22	270
      0x04582C10,  //  00AA  SUB	R22	R22	R16
      0x545E010D,  //  00AB  LDINT	R23	270
      0x005C2E10,  //  00AC  ADD	R23	R23	R16
      0x5C600200,  //  00AD  MOVE	R24	R1
      0x8864010F,  //  00AE  GETMBR	R25	R0	K15
      0x7C441000,  //  00AF  CALL	R17	8
      0xB82A0A00,  //  00B0  GETNGBL	R10	K5
      0x88281522,  //  00B1  GETMBR	R10	R10	K34
      0x80041400,  //  00B2  RET	1	R10
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
      0x540E0003,  //  0001  LDINT	R3	4
      0x0C080403,  //  0002  DIV	R2	R2	R3
      0x540E0063,  //  0003  LDINT	R3	100
      0x240C0203,  //  0004  GT	R3	R1	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x54060063,  //  0006  LDINT	R1	100
      0x140C0301,  //  0007  LT	R3	R1	K1
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x58040001,  //  0009  LDCONST	R1	K1
      0x90020001,  //  000A  SETMBR	R0	K0	R1
      0x540E0003,  //  000B  LDINT	R3	4
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
** Solidified class: lv_signal_arcs
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_arcs,
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
    (be_nested_const_str("lv_signal_arcs", -1455810308, 14))
);
/*******************************************************************/

void be_load_lv_signal_arcs_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_signal_arcs);
    be_setglobal(vm, "lv_signal_arcs");
    be_pop(vm, 1);
}

#endif // USE_LVGL