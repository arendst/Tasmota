/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: set_percentage
********************************************************************/
be_local_closure(lv_signal_arcs_set_percentage,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(percentage),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(invalidate),
    }),
    &be_const_str_set_percentage,
    &be_const_str_solidified,
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
be_local_closure(lv_signal_arcs_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(_lvgl),
    /* K1   */  be_nested_str(create_custom_widget),
    /* K2   */  be_nested_str(percentage),
    /* K3   */  be_nested_str(p1),
    /* K4   */  be_nested_str(lv),
    /* K5   */  be_nested_str(point),
    /* K6   */  be_nested_str(p2),
    /* K7   */  be_nested_str(area),
    /* K8   */  be_nested_str(arc_dsc),
    /* K9   */  be_nested_str(draw_arc_dsc),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x540A0063,  //  0005  LDINT	R2	100
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0xB80A0800,  //  0007  GETNGBL	R2	K4
      0x8C080505,  //  0008  GETMET	R2	R2	K5
      0x7C080200,  //  0009  CALL	R2	1
      0x90020602,  //  000A  SETMBR	R0	K3	R2
      0xB80A0800,  //  000B  GETNGBL	R2	K4
      0x8C080505,  //  000C  GETMET	R2	R2	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x90020C02,  //  000E  SETMBR	R0	K6	R2
      0xB80A0800,  //  000F  GETNGBL	R2	K4
      0x8C080507,  //  0010  GETMET	R2	R2	K7
      0x7C080200,  //  0011  CALL	R2	1
      0x90020E02,  //  0012  SETMBR	R0	K7	R2
      0xB80A0800,  //  0013  GETNGBL	R2	K4
      0x8C080509,  //  0014  GETMET	R2	R2	K9
      0x7C080200,  //  0015  CALL	R2	1
      0x90021002,  //  0016  SETMBR	R0	K8	R2
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: widget_event
********************************************************************/
be_local_closure(lv_signal_arcs_widget_event,   /* name */
  be_nested_proto(
    27,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
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
        &be_const_str_atleast1,
        &be_const_str_solidified,
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
    /* K0   */  be_nested_str(lv),
    /* K1   */  be_nested_str(obj_event_base),
    /* K2   */  be_nested_str(RES_OK),
    /* K3   */  be_nested_str(code),
    /* K4   */  be_nested_str(math),
    /* K5   */  be_nested_str(get_height),
    /* K6   */  be_nested_str(get_width),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str(EVENT_DRAW_MAIN),
    /* K10  */  be_nested_str(draw_ctx),
    /* K11  */  be_nested_str(param),
    /* K12  */  be_nested_str(get_coords),
    /* K13  */  be_nested_str(area),
    /* K14  */  be_nested_str(x1),
    /* K15  */  be_nested_str(y1),
    /* K16  */  be_nested_str(draw_arc_dsc_init),
    /* K17  */  be_nested_str(arc_dsc),
    /* K18  */  be_nested_str(init_draw_arc_dsc),
    /* K19  */  be_nested_str(PART_MAIN),
    /* K20  */  be_nested_str(rounded),
    /* K21  */  be_const_int(1),
    /* K22  */  be_nested_str(width),
    /* K23  */  be_nested_str(get_style_line_color),
    /* K24  */  be_nested_str(STATE_DEFAULT),
    /* K25  */  be_nested_str(get_style_bg_color),
    /* K26  */  be_nested_str(deg),
    /* K27  */  be_nested_str(acos),
    /* K28  */  be_nested_str(p1),
    /* K29  */  be_nested_str(x),
    /* K30  */  be_nested_str(y),
    /* K31  */  be_nested_str(color),
    /* K32  */  be_nested_str(percentage),
    /* K33  */  be_nested_str(draw_arc),
    /* K34  */  be_const_int(0),
    }),
    &be_const_str_widget_event,
    &be_const_str_solidified,
    ( &(const binstruction[171]) {  /* code */
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
      0x782E008A,  //  001E  JMPF	R11	#00AA
      0xB82E0000,  //  001F  GETNGBL	R11	K0
      0x8C2C170A,  //  0020  GETMET	R11	R11	K10
      0x8834050B,  //  0021  GETMBR	R13	R2	K11
      0x7C2C0400,  //  0022  CALL	R11	2
      0x8C30010C,  //  0023  GETMET	R12	R0	K12
      0x8838010D,  //  0024  GETMBR	R14	R0	K13
      0x7C300400,  //  0025  CALL	R12	2
      0x8830010D,  //  0026  GETMBR	R12	R0	K13
      0x8830190E,  //  0027  GETMBR	R12	R12	K14
      0x8834010D,  //  0028  GETMBR	R13	R0	K13
      0x88341B0F,  //  0029  GETMBR	R13	R13	K15
      0xB83A0000,  //  002A  GETNGBL	R14	K0
      0x8C381D10,  //  002B  GETMET	R14	R14	K16
      0x88400111,  //  002C  GETMBR	R16	R0	K17
      0x7C380400,  //  002D  CALL	R14	2
      0x8C380112,  //  002E  GETMET	R14	R0	K18
      0xB8420000,  //  002F  GETNGBL	R16	K0
      0x88402113,  //  0030  GETMBR	R16	R16	K19
      0x88440111,  //  0031  GETMBR	R17	R0	K17
      0x7C380600,  //  0032  CALL	R14	3
      0x88380111,  //  0033  GETMBR	R14	R0	K17
      0x903A2915,  //  0034  SETMBR	R14	K20	K21
      0x88380111,  //  0035  GETMBR	R14	R0	K17
      0x083C1308,  //  0036  MUL	R15	R9	K8
      0x003C1F15,  //  0037  ADD	R15	R15	K21
      0x54420003,  //  0038  LDINT	R16	4
      0x0C3C1E10,  //  0039  DIV	R15	R15	R16
      0x903A2C0F,  //  003A  SETMBR	R14	K22	R15
      0x8C380117,  //  003B  GETMET	R14	R0	K23
      0xB8420000,  //  003C  GETNGBL	R16	K0
      0x88402113,  //  003D  GETMBR	R16	R16	K19
      0xB8460000,  //  003E  GETNGBL	R17	K0
      0x88442318,  //  003F  GETMBR	R17	R17	K24
      0x30402011,  //  0040  OR	R16	R16	R17
      0x7C380400,  //  0041  CALL	R14	2
      0x8C3C0119,  //  0042  GETMET	R15	R0	K25
      0xB8460000,  //  0043  GETNGBL	R17	K0
      0x88442313,  //  0044  GETMBR	R17	R17	K19
      0xB84A0000,  //  0045  GETNGBL	R18	K0
      0x88482518,  //  0046  GETMBR	R18	R18	K24
      0x30442212,  //  0047  OR	R17	R17	R18
      0x7C3C0400,  //  0048  CALL	R15	2
      0x04400C09,  //  0049  SUB	R16	R6	R9
      0x0C440F07,  //  004A  DIV	R17	R7	K7
      0x0444220A,  //  004B  SUB	R17	R17	R10
      0x60480009,  //  004C  GETGBL	R18	G9
      0x544E0059,  //  004D  LDINT	R19	90
      0x8C50091A,  //  004E  GETMET	R20	R4	K26
      0x8C58091B,  //  004F  GETMET	R22	R4	K27
      0x6060000A,  //  0050  GETGBL	R24	G10
      0x5C642200,  //  0051  MOVE	R25	R17
      0x7C600200,  //  0052  CALL	R24	1
      0x6064000A,  //  0053  GETGBL	R25	G10
      0x5C682000,  //  0054  MOVE	R26	R16
      0x7C640200,  //  0055  CALL	R25	1
      0x0C603019,  //  0056  DIV	R24	R24	R25
      0x7C580400,  //  0057  CALL	R22	2
      0x7C500400,  //  0058  CALL	R20	2
      0x044C2614,  //  0059  SUB	R19	R19	R20
      0x7C480200,  //  005A  CALL	R18	1
      0x544E002C,  //  005B  LDINT	R19	45
      0x244C2413,  //  005C  GT	R19	R18	R19
      0x784E0000,  //  005D  JMPF	R19	#005F
      0x544A002C,  //  005E  LDINT	R18	45
      0x884C011C,  //  005F  GETMBR	R19	R0	K28
      0x0C500F07,  //  0060  DIV	R20	R7	K7
      0x00501814,  //  0061  ADD	R20	R12	R20
      0x904E3A14,  //  0062  SETMBR	R19	K29	R20
      0x884C011C,  //  0063  GETMBR	R19	R0	K28
      0x00501A06,  //  0064  ADD	R20	R13	R6
      0x04502915,  //  0065  SUB	R20	R20	K21
      0x0450280A,  //  0066  SUB	R20	R20	R10
      0x904E3C14,  //  0067  SETMBR	R19	K30	R20
      0x884C0111,  //  0068  GETMBR	R19	R0	K17
      0x88500120,  //  0069  GETMBR	R20	R0	K32
      0x54560018,  //  006A  LDINT	R21	25
      0x28502815,  //  006B  GE	R20	R20	R21
      0x78520001,  //  006C  JMPF	R20	#006F
      0x5C501C00,  //  006D  MOVE	R20	R14
      0x70020000,  //  006E  JMP		#0070
      0x5C501E00,  //  006F  MOVE	R20	R15
      0x904E3E14,  //  0070  SETMBR	R19	K31	R20
      0xB84E0000,  //  0071  GETNGBL	R19	K0
      0x8C4C2721,  //  0072  GETMET	R19	R19	K33
      0x5C541600,  //  0073  MOVE	R21	R11
      0x88580111,  //  0074  GETMBR	R22	R0	K17
      0x885C011C,  //  0075  GETMBR	R23	R0	K28
      0x00601208,  //  0076  ADD	R24	R9	R8
      0x08624418,  //  0077  MUL	R24	K34	R24
      0x0060300A,  //  0078  ADD	R24	R24	R10
      0x58640022,  //  0079  LDCONST	R25	K34
      0x546A0167,  //  007A  LDINT	R26	360
      0x7C4C0E00,  //  007B  CALL	R19	7
      0x884C0111,  //  007C  GETMBR	R19	R0	K17
      0x88500120,  //  007D  GETMBR	R20	R0	K32
      0x54560031,  //  007E  LDINT	R21	50
      0x28502815,  //  007F  GE	R20	R20	R21
      0x78520001,  //  0080  JMPF	R20	#0083
      0x5C501C00,  //  0081  MOVE	R20	R14
      0x70020000,  //  0082  JMP		#0084
      0x5C501E00,  //  0083  MOVE	R20	R15
      0x904E3E14,  //  0084  SETMBR	R19	K31	R20
      0xB84E0000,  //  0085  GETNGBL	R19	K0
      0x8C4C2721,  //  0086  GETMET	R19	R19	K33
      0x5C541600,  //  0087  MOVE	R21	R11
      0x88580111,  //  0088  GETMBR	R22	R0	K17
      0x885C011C,  //  0089  GETMBR	R23	R0	K28
      0x00601208,  //  008A  ADD	R24	R9	R8
      0x08622A18,  //  008B  MUL	R24	K21	R24
      0x0060300A,  //  008C  ADD	R24	R24	R10
      0x04603115,  //  008D  SUB	R24	R24	K21
      0x5466010D,  //  008E  LDINT	R25	270
      0x04643212,  //  008F  SUB	R25	R25	R18
      0x546A010D,  //  0090  LDINT	R26	270
      0x00683412,  //  0091  ADD	R26	R26	R18
      0x7C4C0E00,  //  0092  CALL	R19	7
      0x884C0111,  //  0093  GETMBR	R19	R0	K17
      0x88500120,  //  0094  GETMBR	R20	R0	K32
      0x5456004A,  //  0095  LDINT	R21	75
      0x28502815,  //  0096  GE	R20	R20	R21
      0x78520001,  //  0097  JMPF	R20	#009A
      0x5C501C00,  //  0098  MOVE	R20	R14
      0x70020000,  //  0099  JMP		#009B
      0x5C501E00,  //  009A  MOVE	R20	R15
      0x904E3E14,  //  009B  SETMBR	R19	K31	R20
      0xB84E0000,  //  009C  GETNGBL	R19	K0
      0x8C4C2721,  //  009D  GETMET	R19	R19	K33
      0x5C541600,  //  009E  MOVE	R21	R11
      0x88580111,  //  009F  GETMBR	R22	R0	K17
      0x885C011C,  //  00A0  GETMBR	R23	R0	K28
      0x00601208,  //  00A1  ADD	R24	R9	R8
      0x08620E18,  //  00A2  MUL	R24	K7	R24
      0x0060300A,  //  00A3  ADD	R24	R24	R10
      0x04603107,  //  00A4  SUB	R24	R24	K7
      0x5466010D,  //  00A5  LDINT	R25	270
      0x04643212,  //  00A6  SUB	R25	R25	R18
      0x546A010D,  //  00A7  LDINT	R26	270
      0x00683412,  //  00A8  ADD	R26	R26	R18
      0x7C4C0E00,  //  00A9  CALL	R19	7
      0x80000000,  //  00AA  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_percentage
********************************************************************/
be_local_closure(lv_signal_arcs_get_percentage,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(percentage),
    }),
    &be_const_str_get_percentage,
    &be_const_str_solidified,
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
        { be_const_key(percentage, -1), be_const_var(0) },
        { be_const_key(area, 4), be_const_var(3) },
        { be_const_key(p2, -1), be_const_var(2) },
        { be_const_key(get_percentage, -1), be_const_closure(lv_signal_arcs_get_percentage_closure) },
        { be_const_key(p1, -1), be_const_var(1) },
        { be_const_key(arc_dsc, -1), be_const_var(4) },
        { be_const_key(init, 5), be_const_closure(lv_signal_arcs_init_closure) },
        { be_const_key(widget_event, -1), be_const_closure(lv_signal_arcs_widget_event_closure) },
        { be_const_key(set_percentage, 3), be_const_closure(lv_signal_arcs_set_percentage_closure) },
    })),
    be_str_literal("lv_signal_arcs")
);
/*******************************************************************/

void be_load_lv_signal_arcs_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_signal_arcs);
    be_setglobal(vm, "lv_signal_arcs");
    be_pop(vm, 1);
}

#endif // USE_LVGL