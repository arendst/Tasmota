/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_clock_icon_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str(init),
    /* K1   */  be_nested_str(get_height),
    /* K2   */  be_nested_str(refr_size),
    /* K3   */  be_nested_str(get_width),
    /* K4   */  be_nested_str(set_y),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str(get_style_pad_right),
    /* K7   */  be_nested_str(lv),
    /* K8   */  be_nested_str(PART_MAIN),
    /* K9   */  be_nested_str(STATE_DEFAULT),
    /* K10  */  be_nested_str(set_x),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str(set_style_pad_right),
    /* K13  */  be_nested_str(set_style_bg_color),
    /* K14  */  be_nested_str(color),
    /* K15  */  be_nested_str(COLOR_BLACK),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[60]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x20080202,  //  0007  NE	R2	R1	R2
      0x780A0031,  //  0008  JMPF	R2	#003B
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x7C080200,  //  000A  CALL	R2	1
      0x8C0C0102,  //  000B  GETMET	R3	R0	K2
      0x7C0C0200,  //  000C  CALL	R3	1
      0x8C0C0103,  //  000D  GETMET	R3	R0	K3
      0x7C0C0200,  //  000E  CALL	R3	1
      0x8C100104,  //  000F  GETMET	R4	R0	K4
      0x8C180301,  //  0010  GETMET	R6	R1	K1
      0x7C180200,  //  0011  CALL	R6	1
      0x8C1C0101,  //  0012  GETMET	R7	R0	K1
      0x7C1C0200,  //  0013  CALL	R7	1
      0x04180C07,  //  0014  SUB	R6	R6	R7
      0x0C180D05,  //  0015  DIV	R6	R6	K5
      0x7C100400,  //  0016  CALL	R4	2
      0x8C100306,  //  0017  GETMET	R4	R1	K6
      0xB81A0E00,  //  0018  GETNGBL	R6	K7
      0x88180D08,  //  0019  GETMBR	R6	R6	K8
      0xB81E0E00,  //  001A  GETNGBL	R7	K7
      0x881C0F09,  //  001B  GETMBR	R7	R7	K9
      0x30180C07,  //  001C  OR	R6	R6	R7
      0x7C100400,  //  001D  CALL	R4	2
      0x8C14010A,  //  001E  GETMET	R5	R0	K10
      0x8C1C0303,  //  001F  GETMET	R7	R1	K3
      0x7C1C0200,  //  0020  CALL	R7	1
      0x041C0E03,  //  0021  SUB	R7	R7	R3
      0x041C0E04,  //  0022  SUB	R7	R7	R4
      0x041C0F0B,  //  0023  SUB	R7	R7	K11
      0x7C140400,  //  0024  CALL	R5	2
      0x8C14030C,  //  0025  GETMET	R5	R1	K12
      0x001C0803,  //  0026  ADD	R7	R4	R3
      0x54220005,  //  0027  LDINT	R8	6
      0x001C0E08,  //  0028  ADD	R7	R7	R8
      0xB8220E00,  //  0029  GETNGBL	R8	K7
      0x88201108,  //  002A  GETMBR	R8	R8	K8
      0xB8260E00,  //  002B  GETNGBL	R9	K7
      0x88241309,  //  002C  GETMBR	R9	R9	K9
      0x30201009,  //  002D  OR	R8	R8	R9
      0x7C140600,  //  002E  CALL	R5	3
      0x8C14010D,  //  002F  GETMET	R5	R0	K13
      0xB81E0E00,  //  0030  GETNGBL	R7	K7
      0x8C1C0F0E,  //  0031  GETMET	R7	R7	K14
      0xB8260E00,  //  0032  GETNGBL	R9	K7
      0x8824130F,  //  0033  GETMBR	R9	R9	K15
      0x7C1C0400,  //  0034  CALL	R7	2
      0xB8220E00,  //  0035  GETNGBL	R8	K7
      0x88201108,  //  0036  GETMBR	R8	R8	K8
      0xB8260E00,  //  0037  GETNGBL	R9	K7
      0x88241309,  //  0038  GETMBR	R9	R9	K9
      0x30201009,  //  0039  OR	R8	R8	R9
      0x7C140600,  //  003A  CALL	R5	3
      0x80000000,  //  003B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_clock_icon
********************************************************************/
extern const bclass be_class_lv_clock;
be_local_class(lv_clock_icon,
    0,
    &be_class_lv_clock,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(lv_clock_icon_init_closure) },
    })),
    be_str_literal("lv_clock_icon")
);
/*******************************************************************/

void be_load_lv_clock_icon_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_clock_icon);
    be_setglobal(vm, "lv_clock_icon");
    be_pop(vm, 1);
}

#endif // USE_LVGL