/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_wifi_bars_icon_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str(init),
    /* K1   */  be_nested_str(set_style_line_color),
    /* K2   */  be_nested_str(lv),
    /* K3   */  be_nested_str(color),
    /* K4   */  be_nested_str(COLOR_WHITE),
    /* K5   */  be_nested_str(PART_MAIN),
    /* K6   */  be_nested_str(STATE_DEFAULT),
    /* K7   */  be_nested_str(set_style_bg_color),
    /* K8   */  be_nested_str(COLOR_BLACK),
    /* K9   */  be_nested_str(get_height),
    /* K10  */  be_nested_str(get_style_pad_right),
    /* K11  */  be_nested_str(set_height),
    /* K12  */  be_nested_str(set_width),
    /* K13  */  be_nested_str(set_x),
    /* K14  */  be_nested_str(get_width),
    /* K15  */  be_nested_str(set_style_pad_right),
    /* K16  */  be_const_int(1),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0xB8120400,  //  0007  GETNGBL	R4	K2
      0x8C100903,  //  0008  GETMET	R4	R4	K3
      0xB81A0400,  //  0009  GETNGBL	R6	K2
      0x88180D04,  //  000A  GETMBR	R6	R6	K4
      0x7C100400,  //  000B  CALL	R4	2
      0xB8160400,  //  000C  GETNGBL	R5	K2
      0x88140B05,  //  000D  GETMBR	R5	R5	K5
      0xB81A0400,  //  000E  GETNGBL	R6	K2
      0x88180D06,  //  000F  GETMBR	R6	R6	K6
      0x30140A06,  //  0010  OR	R5	R5	R6
      0x7C080600,  //  0011  CALL	R2	3
      0x8C080107,  //  0012  GETMET	R2	R0	K7
      0xB8120400,  //  0013  GETNGBL	R4	K2
      0x8C100903,  //  0014  GETMET	R4	R4	K3
      0xB81A0400,  //  0015  GETNGBL	R6	K2
      0x88180D08,  //  0016  GETMBR	R6	R6	K8
      0x7C100400,  //  0017  CALL	R4	2
      0xB8160400,  //  0018  GETNGBL	R5	K2
      0x88140B05,  //  0019  GETMBR	R5	R5	K5
      0xB81A0400,  //  001A  GETNGBL	R6	K2
      0x88180D06,  //  001B  GETMBR	R6	R6	K6
      0x30140A06,  //  001C  OR	R5	R5	R6
      0x7C080600,  //  001D  CALL	R2	3
      0x4C080000,  //  001E  LDNIL	R2
      0x20080202,  //  001F  NE	R2	R1	R2
      0x780A001D,  //  0020  JMPF	R2	#003F
      0x8C080309,  //  0021  GETMET	R2	R1	K9
      0x7C080200,  //  0022  CALL	R2	1
      0x8C0C030A,  //  0023  GETMET	R3	R1	K10
      0xB8160400,  //  0024  GETNGBL	R5	K2
      0x88140B05,  //  0025  GETMBR	R5	R5	K5
      0xB81A0400,  //  0026  GETNGBL	R6	K2
      0x88180D06,  //  0027  GETMBR	R6	R6	K6
      0x30140A06,  //  0028  OR	R5	R5	R6
      0x7C0C0400,  //  0029  CALL	R3	2
      0x8C10010B,  //  002A  GETMET	R4	R0	K11
      0x5C180400,  //  002B  MOVE	R6	R2
      0x7C100400,  //  002C  CALL	R4	2
      0x8C10010C,  //  002D  GETMET	R4	R0	K12
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x8C10010D,  //  0030  GETMET	R4	R0	K13
      0x8C18030E,  //  0031  GETMET	R6	R1	K14
      0x7C180200,  //  0032  CALL	R6	1
      0x04180C02,  //  0033  SUB	R6	R6	R2
      0x04180C03,  //  0034  SUB	R6	R6	R3
      0x7C100400,  //  0035  CALL	R4	2
      0x8C10030F,  //  0036  GETMET	R4	R1	K15
      0x00180602,  //  0037  ADD	R6	R3	R2
      0x00180D10,  //  0038  ADD	R6	R6	K16
      0xB81E0400,  //  0039  GETNGBL	R7	K2
      0x881C0F05,  //  003A  GETMBR	R7	R7	K5
      0xB8220400,  //  003B  GETNGBL	R8	K2
      0x88201106,  //  003C  GETMBR	R8	R8	K6
      0x301C0E08,  //  003D  OR	R7	R7	R8
      0x7C100600,  //  003E  CALL	R4	3
      0x80000000,  //  003F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_wifi_bars_icon
********************************************************************/
extern const bclass be_class_lv_wifi_bars;
be_local_class(lv_wifi_bars_icon,
    0,
    &be_class_lv_wifi_bars,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(lv_wifi_bars_icon_init_closure) },
    })),
    be_str_literal("lv_wifi_bars_icon")
);
/*******************************************************************/

void be_load_lv_wifi_bars_icon_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_wifi_bars_icon);
    be_setglobal(vm, "lv_wifi_bars_icon");
    be_pop(vm, 1);
}

#endif // USE_LVGL