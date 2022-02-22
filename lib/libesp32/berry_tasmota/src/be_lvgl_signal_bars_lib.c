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
    ( &(const bvalue[38]) {     /* constants */
    /* K0   */  be_nested_str(lv),
    /* K1   */  be_nested_str(obj_event_base),
    /* K2   */  be_nested_str(RES_OK),
    /* K3   */  be_nested_str(code),
    /* K4   */  be_nested_str(get_height),
    /* K5   */  be_nested_str(get_width),
    /* K6   */  be_const_int(3),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str(EVENT_DRAW_MAIN),
    /* K9   */  be_nested_str(draw_ctx),
    /* K10  */  be_nested_str(param),
    /* K11  */  be_nested_str(get_coords),
    /* K12  */  be_nested_str(area),
    /* K13  */  be_nested_str(x1),
    /* K14  */  be_nested_str(y1),
    /* K15  */  be_nested_str(draw_line_dsc_init),
    /* K16  */  be_nested_str(line_dsc),
    /* K17  */  be_nested_str(init_draw_line_dsc),
    /* K18  */  be_nested_str(PART_MAIN),
    /* K19  */  be_nested_str(round_start),
    /* K20  */  be_const_int(1),
    /* K21  */  be_nested_str(round_end),
    /* K22  */  be_nested_str(width),
    /* K23  */  be_nested_str(get_style_line_color),
    /* K24  */  be_nested_str(STATE_DEFAULT),
    /* K25  */  be_nested_str(get_style_bg_color),
    /* K26  */  be_nested_str(event_send),
    /* K27  */  be_nested_str(EVENT_DRAW_PART_BEGIN),
    /* K28  */  be_const_int(0),
    /* K29  */  be_nested_str(color),
    /* K30  */  be_nested_str(percentage),
    /* K31  */  be_nested_str(p1),
    /* K32  */  be_nested_str(y),
    /* K33  */  be_nested_str(x),
    /* K34  */  be_nested_str(p2),
    /* K35  */  be_nested_str(draw_line),
    /* K36  */  be_nested_str(stop_iteration),
    /* K37  */  be_nested_str(EVENT_DRAW_PART_END),
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
      0x8834010C,  //  0024  GETMBR	R13	R0	K12
      0x7C2C0400,  //  0025  CALL	R11	2
      0x882C010C,  //  0026  GETMBR	R11	R0	K12
      0x882C170D,  //  0027  GETMBR	R11	R11	K13
      0x8830010C,  //  0028  GETMBR	R12	R0	K12
      0x8830190E,  //  0029  GETMBR	R12	R12	K14
      0xB8360000,  //  002A  GETNGBL	R13	K0
      0x8C341B0F,  //  002B  GETMET	R13	R13	K15
      0x883C0110,  //  002C  GETMBR	R15	R0	K16
      0x7C340400,  //  002D  CALL	R13	2
      0x8C340111,  //  002E  GETMET	R13	R0	K17
      0xB83E0000,  //  002F  GETNGBL	R15	K0
      0x883C1F12,  //  0030  GETMBR	R15	R15	K18
      0x88400110,  //  0031  GETMBR	R16	R0	K16
      0x7C340600,  //  0032  CALL	R13	3
      0x88340110,  //  0033  GETMBR	R13	R0	K16
      0x90362714,  //  0034  SETMBR	R13	K19	K20
      0x88340110,  //  0035  GETMBR	R13	R0	K16
      0x90362B14,  //  0036  SETMBR	R13	K21	K20
      0x88340110,  //  0037  GETMBR	R13	R0	K16
      0x90362C08,  //  0038  SETMBR	R13	K22	R8
      0x8C340117,  //  0039  GETMET	R13	R0	K23
      0xB83E0000,  //  003A  GETNGBL	R15	K0
      0x883C1F12,  //  003B  GETMBR	R15	R15	K18
      0xB8420000,  //  003C  GETNGBL	R16	K0
      0x88402118,  //  003D  GETMBR	R16	R16	K24
      0x303C1E10,  //  003E  OR	R15	R15	R16
      0x7C340400,  //  003F  CALL	R13	2
      0x8C380119,  //  0040  GETMET	R14	R0	K25
      0xB8420000,  //  0041  GETNGBL	R16	K0
      0x88402112,  //  0042  GETMBR	R16	R16	K18
      0xB8460000,  //  0043  GETNGBL	R17	K0
      0x88442318,  //  0044  GETMBR	R17	R17	K24
      0x30402011,  //  0045  OR	R16	R16	R17
      0x7C380400,  //  0046  CALL	R14	2
      0xB83E0000,  //  0047  GETNGBL	R15	K0
      0x8C3C1F1A,  //  0048  GETMET	R15	R15	K26
      0x5C440000,  //  0049  MOVE	R17	R0
      0xB84A0000,  //  004A  GETNGBL	R18	K0
      0x8848251B,  //  004B  GETMBR	R18	R18	K27
      0x884C0110,  //  004C  GETMBR	R19	R0	K16
      0x7C3C0800,  //  004D  CALL	R15	4
      0x603C0010,  //  004E  GETGBL	R15	G16
      0x40423906,  //  004F  CONNECT	R16	K28	K6
      0x7C3C0200,  //  0050  CALL	R15	1
      0xA802002C,  //  0051  EXBLK	0	#007F
      0x5C401E00,  //  0052  MOVE	R16	R15
      0x7C400000,  //  0053  CALL	R16	0
      0x88440110,  //  0054  GETMBR	R17	R0	K16
      0x8848011E,  //  0055  GETMBR	R18	R0	K30
      0x004C2114,  //  0056  ADD	R19	R16	K20
      0x54520013,  //  0057  LDINT	R20	20
      0x084C2614,  //  0058  MUL	R19	R19	R20
      0x28482413,  //  0059  GE	R18	R18	R19
      0x784A0001,  //  005A  JMPF	R18	#005D
      0x5C481A00,  //  005B  MOVE	R18	R13
      0x70020000,  //  005C  JMP		#005E
      0x5C481C00,  //  005D  MOVE	R18	R14
      0x90463A12,  //  005E  SETMBR	R17	K29	R18
      0x8844011F,  //  005F  GETMBR	R17	R0	K31
      0x00481805,  //  0060  ADD	R18	R12	R5
      0x04482514,  //  0061  SUB	R18	R18	K20
      0x04482409,  //  0062  SUB	R18	R18	R9
      0x90464012,  //  0063  SETMBR	R17	K32	R18
      0x8844011F,  //  0064  GETMBR	R17	R0	K31
      0x00481007,  //  0065  ADD	R18	R8	R7
      0x08482012,  //  0066  MUL	R18	R16	R18
      0x00481612,  //  0067  ADD	R18	R11	R18
      0x00482409,  //  0068  ADD	R18	R18	R9
      0x90464212,  //  0069  SETMBR	R17	K33	R18
      0x88440122,  //  006A  GETMBR	R17	R0	K34
      0x044A0C10,  //  006B  SUB	R18	K6	R16
      0x044C0A08,  //  006C  SUB	R19	R5	R8
      0x08482413,  //  006D  MUL	R18	R18	R19
      0x544E0003,  //  006E  LDINT	R19	4
      0x0C482413,  //  006F  DIV	R18	R18	R19
      0x00481812,  //  0070  ADD	R18	R12	R18
      0x00482409,  //  0071  ADD	R18	R18	R9
      0x90464012,  //  0072  SETMBR	R17	K32	R18
      0x88440122,  //  0073  GETMBR	R17	R0	K34
      0x8848011F,  //  0074  GETMBR	R18	R0	K31
      0x88482521,  //  0075  GETMBR	R18	R18	K33
      0x90464212,  //  0076  SETMBR	R17	K33	R18
      0xB8460000,  //  0077  GETNGBL	R17	K0
      0x8C442323,  //  0078  GETMET	R17	R17	K35
      0x5C4C1400,  //  0079  MOVE	R19	R10
      0x88500110,  //  007A  GETMBR	R20	R0	K16
      0x8854011F,  //  007B  GETMBR	R21	R0	K31
      0x88580122,  //  007C  GETMBR	R22	R0	K34
      0x7C440A00,  //  007D  CALL	R17	5
      0x7001FFD2,  //  007E  JMP		#0052
      0x583C0024,  //  007F  LDCONST	R15	K36
      0xAC3C0200,  //  0080  CATCH	R15	1	0
      0xB0080000,  //  0081  RAISE	2	R0	R0
      0xB83E0000,  //  0082  GETNGBL	R15	K0
      0x8C3C1F1A,  //  0083  GETMET	R15	R15	K26
      0x5C440000,  //  0084  MOVE	R17	R0
      0xB84A0000,  //  0085  GETNGBL	R18	K0
      0x88482525,  //  0086  GETMBR	R18	R18	K37
      0x884C0110,  //  0087  GETMBR	R19	R0	K16
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
** Solidified class: lv_signal_bars
********************************************************************/
extern const bclass be_class_lv_obj;
be_local_class(lv_signal_bars,
    5,
    &be_class_lv_obj,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(percentage, 4), be_const_var(0) },
        { be_const_key(p1, 5), be_const_var(1) },
        { be_const_key(p2, -1), be_const_var(2) },
        { be_const_key(get_percentage, -1), be_const_closure(lv_signal_bars_get_percentage_closure) },
        { be_const_key(line_dsc, -1), be_const_var(4) },
        { be_const_key(area, -1), be_const_var(3) },
        { be_const_key(init, -1), be_const_closure(lv_signal_bars_init_closure) },
        { be_const_key(widget_event, -1), be_const_closure(lv_signal_bars_widget_event_closure) },
        { be_const_key(set_percentage, 3), be_const_closure(lv_signal_bars_set_percentage_closure) },
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