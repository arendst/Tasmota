/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: set_percentage
********************************************************************/
be_local_closure(lv_signal_bars_set_percentage,   /* name */
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
    /* K0   */  be_nested_str(percentage),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(invalidate),
    }),
    &be_const_str_set_percentage,
    &be_const_str_solidified,
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
be_local_closure(lv_signal_bars_init,   /* name */
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
    /* K0   */  be_nested_str(_lvgl),
    /* K1   */  be_nested_str(create_custom_widget),
    /* K2   */  be_nested_str(percentage),
    /* K3   */  be_nested_str(p1),
    /* K4   */  be_nested_str(lv),
    /* K5   */  be_nested_str(point),
    /* K6   */  be_nested_str(p2),
    /* K7   */  be_nested_str(area),
    /* K8   */  be_nested_str(line_dsc),
    /* K9   */  be_nested_str(draw_line_dsc),
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
be_local_closure(lv_signal_bars_widget_event,   /* name */
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
    ( &(const bvalue[37]) {     /* constants */
    /* K0   */  be_nested_str(lv),
    /* K1   */  be_nested_str(obj_event_base),
    /* K2   */  be_nested_str(RES_OK),
    /* K3   */  be_nested_str(code),
    /* K4   */  be_nested_str(get_height),
    /* K5   */  be_nested_str(get_width),
    /* K6   */  be_const_int(3),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str(EVENT_DRAW_MAIN),
    /* K9   */  be_nested_str(area),
    /* K10  */  be_nested_str(param),
    /* K11  */  be_nested_str(get_coords),
    /* K12  */  be_nested_str(x1),
    /* K13  */  be_nested_str(y1),
    /* K14  */  be_nested_str(draw_line_dsc_init),
    /* K15  */  be_nested_str(line_dsc),
    /* K16  */  be_nested_str(init_draw_line_dsc),
    /* K17  */  be_nested_str(PART_MAIN),
    /* K18  */  be_nested_str(round_start),
    /* K19  */  be_const_int(1),
    /* K20  */  be_nested_str(round_end),
    /* K21  */  be_nested_str(width),
    /* K22  */  be_nested_str(get_style_line_color),
    /* K23  */  be_nested_str(STATE_DEFAULT),
    /* K24  */  be_nested_str(get_style_bg_color),
    /* K25  */  be_nested_str(event_send),
    /* K26  */  be_nested_str(EVENT_DRAW_PART_BEGIN),
    /* K27  */  be_const_int(0),
    /* K28  */  be_nested_str(color),
    /* K29  */  be_nested_str(percentage),
    /* K30  */  be_nested_str(p1),
    /* K31  */  be_nested_str(y),
    /* K32  */  be_nested_str(x),
    /* K33  */  be_nested_str(p2),
    /* K34  */  be_nested_str(draw_line),
    /* K35  */  be_nested_str(stop_iteration),
    /* K36  */  be_nested_str(EVENT_DRAW_PART_END),
    }),
    &be_const_str_widget_event,
    &be_const_str_solidified,
    ( &(const binstruction[138]) {  /* code */
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
      0x782A0069,  //  001E  JMPF	R10	#0089
      0xB82A0000,  //  001F  GETNGBL	R10	K0
      0x8C281509,  //  0020  GETMET	R10	R10	K9
      0x8830050A,  //  0021  GETMBR	R12	R2	K10
      0x7C280400,  //  0022  CALL	R10	2
      0x8C2C010B,  //  0023  GETMET	R11	R0	K11
      0x88340109,  //  0024  GETMBR	R13	R0	K9
      0x7C2C0400,  //  0025  CALL	R11	2
      0x882C0109,  //  0026  GETMBR	R11	R0	K9
      0x882C170C,  //  0027  GETMBR	R11	R11	K12
      0x88300109,  //  0028  GETMBR	R12	R0	K9
      0x8830190D,  //  0029  GETMBR	R12	R12	K13
      0xB8360000,  //  002A  GETNGBL	R13	K0
      0x8C341B0E,  //  002B  GETMET	R13	R13	K14
      0x883C010F,  //  002C  GETMBR	R15	R0	K15
      0x7C340400,  //  002D  CALL	R13	2
      0x8C340110,  //  002E  GETMET	R13	R0	K16
      0xB83E0000,  //  002F  GETNGBL	R15	K0
      0x883C1F11,  //  0030  GETMBR	R15	R15	K17
      0x8840010F,  //  0031  GETMBR	R16	R0	K15
      0x7C340600,  //  0032  CALL	R13	3
      0x8834010F,  //  0033  GETMBR	R13	R0	K15
      0x90362513,  //  0034  SETMBR	R13	K18	K19
      0x8834010F,  //  0035  GETMBR	R13	R0	K15
      0x90362913,  //  0036  SETMBR	R13	K20	K19
      0x8834010F,  //  0037  GETMBR	R13	R0	K15
      0x90362A08,  //  0038  SETMBR	R13	K21	R8
      0x8C340116,  //  0039  GETMET	R13	R0	K22
      0xB83E0000,  //  003A  GETNGBL	R15	K0
      0x883C1F11,  //  003B  GETMBR	R15	R15	K17
      0xB8420000,  //  003C  GETNGBL	R16	K0
      0x88402117,  //  003D  GETMBR	R16	R16	K23
      0x303C1E10,  //  003E  OR	R15	R15	R16
      0x7C340400,  //  003F  CALL	R13	2
      0x8C380118,  //  0040  GETMET	R14	R0	K24
      0xB8420000,  //  0041  GETNGBL	R16	K0
      0x88402111,  //  0042  GETMBR	R16	R16	K17
      0xB8460000,  //  0043  GETNGBL	R17	K0
      0x88442317,  //  0044  GETMBR	R17	R17	K23
      0x30402011,  //  0045  OR	R16	R16	R17
      0x7C380400,  //  0046  CALL	R14	2
      0xB83E0000,  //  0047  GETNGBL	R15	K0
      0x8C3C1F19,  //  0048  GETMET	R15	R15	K25
      0x5C440000,  //  0049  MOVE	R17	R0
      0xB84A0000,  //  004A  GETNGBL	R18	K0
      0x8848251A,  //  004B  GETMBR	R18	R18	K26
      0x884C010F,  //  004C  GETMBR	R19	R0	K15
      0x7C3C0800,  //  004D  CALL	R15	4
      0x603C0010,  //  004E  GETGBL	R15	G16
      0x40423706,  //  004F  CONNECT	R16	K27	K6
      0x7C3C0200,  //  0050  CALL	R15	1
      0xA802002C,  //  0051  EXBLK	0	#007F
      0x5C401E00,  //  0052  MOVE	R16	R15
      0x7C400000,  //  0053  CALL	R16	0
      0x8844010F,  //  0054  GETMBR	R17	R0	K15
      0x8848011D,  //  0055  GETMBR	R18	R0	K29
      0x004C2113,  //  0056  ADD	R19	R16	K19
      0x54520013,  //  0057  LDINT	R20	20
      0x084C2614,  //  0058  MUL	R19	R19	R20
      0x28482413,  //  0059  GE	R18	R18	R19
      0x784A0001,  //  005A  JMPF	R18	#005D
      0x5C481A00,  //  005B  MOVE	R18	R13
      0x70020000,  //  005C  JMP		#005E
      0x5C481C00,  //  005D  MOVE	R18	R14
      0x90463812,  //  005E  SETMBR	R17	K28	R18
      0x8844011E,  //  005F  GETMBR	R17	R0	K30
      0x00481805,  //  0060  ADD	R18	R12	R5
      0x04482513,  //  0061  SUB	R18	R18	K19
      0x04482409,  //  0062  SUB	R18	R18	R9
      0x90463E12,  //  0063  SETMBR	R17	K31	R18
      0x8844011E,  //  0064  GETMBR	R17	R0	K30
      0x00481007,  //  0065  ADD	R18	R8	R7
      0x08482012,  //  0066  MUL	R18	R16	R18
      0x00481612,  //  0067  ADD	R18	R11	R18
      0x00482409,  //  0068  ADD	R18	R18	R9
      0x90464012,  //  0069  SETMBR	R17	K32	R18
      0x88440121,  //  006A  GETMBR	R17	R0	K33
      0x044A0C10,  //  006B  SUB	R18	K6	R16
      0x044C0A08,  //  006C  SUB	R19	R5	R8
      0x08482413,  //  006D  MUL	R18	R18	R19
      0x544E0003,  //  006E  LDINT	R19	4
      0x0C482413,  //  006F  DIV	R18	R18	R19
      0x00481812,  //  0070  ADD	R18	R12	R18
      0x00482409,  //  0071  ADD	R18	R18	R9
      0x90463E12,  //  0072  SETMBR	R17	K31	R18
      0x88440121,  //  0073  GETMBR	R17	R0	K33
      0x8848011E,  //  0074  GETMBR	R18	R0	K30
      0x88482520,  //  0075  GETMBR	R18	R18	K32
      0x90464012,  //  0076  SETMBR	R17	K32	R18
      0xB8460000,  //  0077  GETNGBL	R17	K0
      0x8C442322,  //  0078  GETMET	R17	R17	K34
      0x884C011E,  //  0079  GETMBR	R19	R0	K30
      0x88500121,  //  007A  GETMBR	R20	R0	K33
      0x5C541400,  //  007B  MOVE	R21	R10
      0x8858010F,  //  007C  GETMBR	R22	R0	K15
      0x7C440A00,  //  007D  CALL	R17	5
      0x7001FFD2,  //  007E  JMP		#0052
      0x583C0023,  //  007F  LDCONST	R15	K35
      0xAC3C0200,  //  0080  CATCH	R15	1	0
      0xB0080000,  //  0081  RAISE	2	R0	R0
      0xB83E0000,  //  0082  GETNGBL	R15	K0
      0x8C3C1F19,  //  0083  GETMET	R15	R15	K25
      0x5C440000,  //  0084  MOVE	R17	R0
      0xB84A0000,  //  0085  GETNGBL	R18	K0
      0x88482524,  //  0086  GETMBR	R18	R18	K36
      0x884C010F,  //  0087  GETMBR	R19	R0	K15
      0x7C3C0800,  //  0088  CALL	R15	4
      0x80000000,  //  0089  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_percentage
********************************************************************/
be_local_closure(lv_signal_bars_get_percentage,   /* name */
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
** Solidified class: lv_signal_bars
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_bars,
    5,
    &be_class_lv_obj,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(percentage, 4), be_const_var(0) },
        { be_const_key(p1, 3), be_const_var(1) },
        { be_const_key(p2, -1), be_const_var(2) },
        { be_const_key(area, -1), be_const_var(3) },
        { be_const_key(line_dsc, -1), be_const_var(4) },
        { be_const_key(set_percentage, -1), be_const_closure(lv_signal_bars_set_percentage_closure) },
        { be_const_key(init, -1), be_const_closure(lv_signal_bars_init_closure) },
        { be_const_key(widget_event, -1), be_const_closure(lv_signal_bars_widget_event_closure) },
        { be_const_key(get_percentage, 5), be_const_closure(lv_signal_bars_get_percentage_closure) },
    })),
    be_str_literal("lv_signal_bars")
);
/*******************************************************************/

void be_load_lv_signal_bars_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_signal_bars);
    be_setglobal(vm, "lv_signal_bars");
    be_pop(vm, 1);
}

#endif // USE_LVGL