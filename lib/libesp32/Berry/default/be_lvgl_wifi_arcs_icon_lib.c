/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_string("init", 380752755, 4),
    /* K1   */  be_nested_string("set_style_line_color", -629728320, 20),
    /* K2   */  be_nested_string("lv_color", 1419148319, 8),
    /* K3   */  be_nested_string("lv", 1529997255, 2),
    /* K4   */  be_nested_string("COLOR_WHITE", -1758096026, 11),
    /* K5   */  be_nested_string("PART_MAIN", -1821475788, 9),
    /* K6   */  be_nested_string("STATE_DEFAULT", 712406428, 13),
    /* K7   */  be_nested_string("set_style_bg_color", 1689513089, 18),
    /* K8   */  be_nested_string("COLOR_BLACK", 264427940, 11),
    /* K9   */  be_nested_string("get_height", -723211773, 10),
    /* K10  */  be_nested_string("get_style_pad_right", -1144679830, 19),
    /* K11  */  be_nested_string("set_height", 1080207399, 10),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_string("set_width", 484671920, 9),
    /* K14  */  be_nested_string("set_x", 1849400772, 5),
    /* K15  */  be_nested_string("get_width", -1001549996, 9),
    /* K16  */  be_nested_string("set_style_pad_right", -980898242, 19),
    /* K17  */  be_const_int(1),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[65]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0xB8120400,  //  0007  GETNGBL	R4	K2
      0xB8160600,  //  0008  GETNGBL	R5	K3
      0x88140B04,  //  0009  GETMBR	R5	R5	K4
      0x7C100200,  //  000A  CALL	R4	1
      0xB8160600,  //  000B  GETNGBL	R5	K3
      0x88140B05,  //  000C  GETMBR	R5	R5	K5
      0xB81A0600,  //  000D  GETNGBL	R6	K3
      0x88180D06,  //  000E  GETMBR	R6	R6	K6
      0x30140A06,  //  000F  OR	R5	R5	R6
      0x7C080600,  //  0010  CALL	R2	3
      0x8C080107,  //  0011  GETMET	R2	R0	K7
      0xB8120400,  //  0012  GETNGBL	R4	K2
      0xB8160600,  //  0013  GETNGBL	R5	K3
      0x88140B08,  //  0014  GETMBR	R5	R5	K8
      0x7C100200,  //  0015  CALL	R4	1
      0xB8160600,  //  0016  GETNGBL	R5	K3
      0x88140B05,  //  0017  GETMBR	R5	R5	K5
      0xB81A0600,  //  0018  GETNGBL	R6	K3
      0x88180D06,  //  0019  GETMBR	R6	R6	K6
      0x30140A06,  //  001A  OR	R5	R5	R6
      0x7C080600,  //  001B  CALL	R2	3
      0x4C080000,  //  001C  LDNIL	R2
      0x20080202,  //  001D  NE	R2	R1	R2
      0x780A0020,  //  001E  JMPF	R2	#0040
      0x8C080309,  //  001F  GETMET	R2	R1	K9
      0x7C080200,  //  0020  CALL	R2	1
      0x8C0C030A,  //  0021  GETMET	R3	R1	K10
      0xB8160600,  //  0022  GETNGBL	R5	K3
      0x88140B05,  //  0023  GETMBR	R5	R5	K5
      0xB81A0600,  //  0024  GETNGBL	R6	K3
      0x88180D06,  //  0025  GETMBR	R6	R6	K6
      0x30140A06,  //  0026  OR	R5	R5	R6
      0x7C0C0400,  //  0027  CALL	R3	2
      0x8C10010B,  //  0028  GETMET	R4	R0	K11
      0x5C180400,  //  0029  MOVE	R6	R2
      0x7C100400,  //  002A  CALL	R4	2
      0x54120003,  //  002B  LDINT	R4	4
      0x08100404,  //  002C  MUL	R4	R2	R4
      0x0C10090C,  //  002D  DIV	R4	R4	K12
      0x8C14010D,  //  002E  GETMET	R5	R0	K13
      0x5C1C0800,  //  002F  MOVE	R7	R4
      0x7C140400,  //  0030  CALL	R5	2
      0x8C14010E,  //  0031  GETMET	R5	R0	K14
      0x8C1C030F,  //  0032  GETMET	R7	R1	K15
      0x7C1C0200,  //  0033  CALL	R7	1
      0x041C0E04,  //  0034  SUB	R7	R7	R4
      0x041C0E03,  //  0035  SUB	R7	R7	R3
      0x7C140400,  //  0036  CALL	R5	2
      0x8C140310,  //  0037  GETMET	R5	R1	K16
      0x001C0604,  //  0038  ADD	R7	R3	R4
      0x001C0F11,  //  0039  ADD	R7	R7	K17
      0xB8220600,  //  003A  GETNGBL	R8	K3
      0x88201105,  //  003B  GETMBR	R8	R8	K5
      0xB8260600,  //  003C  GETNGBL	R9	K3
      0x88241306,  //  003D  GETMBR	R9	R9	K6
      0x30201009,  //  003E  OR	R8	R8	R9
      0x7C140600,  //  003F  CALL	R5	3
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_wifi_arcs_icon
********************************************************************/
extern const bclass be_class_lv_wifi_arcs;
be_local_class(lv_wifi_arcs_icon,
    0,
    &be_class_lv_wifi_arcs,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(init_closure) },
    })),
    (be_nested_const_str("lv_wifi_arcs_icon", 1507982909, 17))
);
/*******************************************************************/

void be_load_lv_wifi_arcs_icon_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_wifi_arcs_icon);
    be_setglobal(vm, "lv_wifi_arcs_icon");
    be_pop(vm, 1);
}

#endif // USE_LVGL