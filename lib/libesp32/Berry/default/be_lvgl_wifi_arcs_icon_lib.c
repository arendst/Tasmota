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
    11,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_string("init", 380752755, 4),
    /* K1   */  be_nested_string("set_style_local_bg_color", -498263023, 24),
    /* K2   */  be_nested_string("lv", 1529997255, 2),
    /* K3   */  be_nested_string("OBJ_PART_MAIN", 658062838, 13),
    /* K4   */  be_nested_string("STATE_DEFAULT", 712406428, 13),
    /* K5   */  be_nested_string("lv_color", 1419148319, 8),
    /* K6   */  be_nested_string("COLOR_BLACK", 264427940, 11),
    /* K7   */  be_nested_string("get_height", -723211773, 10),
    /* K8   */  be_nested_string("get_style_pad_right", -1144679830, 19),
    /* K9   */  be_nested_string("set_height", 1080207399, 10),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_string("set_width", 484671920, 9),
    /* K12  */  be_nested_string("set_x", 1849400772, 5),
    /* K13  */  be_nested_string("get_width", -1001549996, 9),
    /* K14  */  be_nested_string("set_style_local_pad_right", 1126712366, 25),
    /* K15  */  be_const_int(1),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[52]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x8C0C0101,  //  0007  GETMET	R3	R0	K1
      0xB8160400,  //  0008  GETNGBL	R5	K2
      0x88140B03,  //  0009  GETMBR	R5	R5	K3
      0xB81A0400,  //  000A  GETNGBL	R6	K2
      0x88180D04,  //  000B  GETMBR	R6	R6	K4
      0xB81E0A00,  //  000C  GETNGBL	R7	K5
      0xB8220400,  //  000D  GETNGBL	R8	K2
      0x88201106,  //  000E  GETMBR	R8	R8	K6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x7C0C0800,  //  0010  CALL	R3	4
      0x4C0C0000,  //  0011  LDNIL	R3
      0x200C0203,  //  0012  NE	R3	R1	R3
      0x780E001E,  //  0013  JMPF	R3	#0033
      0x8C0C0307,  //  0014  GETMET	R3	R1	K7
      0x7C0C0200,  //  0015  CALL	R3	1
      0x8C100308,  //  0016  GETMET	R4	R1	K8
      0xB81A0400,  //  0017  GETNGBL	R6	K2
      0x88180D03,  //  0018  GETMBR	R6	R6	K3
      0xB81E0400,  //  0019  GETNGBL	R7	K2
      0x881C0F04,  //  001A  GETMBR	R7	R7	K4
      0x7C100600,  //  001B  CALL	R4	3
      0x8C140109,  //  001C  GETMET	R5	R0	K9
      0x5C1C0600,  //  001D  MOVE	R7	R3
      0x7C140400,  //  001E  CALL	R5	2
      0x54160003,  //  001F  LDINT	R5	4
      0x08140605,  //  0020  MUL	R5	R3	R5
      0x0C140B0A,  //  0021  DIV	R5	R5	K10
      0x8C18010B,  //  0022  GETMET	R6	R0	K11
      0x5C200A00,  //  0023  MOVE	R8	R5
      0x7C180400,  //  0024  CALL	R6	2
      0x8C18010C,  //  0025  GETMET	R6	R0	K12
      0x8C20030D,  //  0026  GETMET	R8	R1	K13
      0x7C200200,  //  0027  CALL	R8	1
      0x04201005,  //  0028  SUB	R8	R8	R5
      0x04201004,  //  0029  SUB	R8	R8	R4
      0x7C180400,  //  002A  CALL	R6	2
      0x8C18030E,  //  002B  GETMET	R6	R1	K14
      0xB8220400,  //  002C  GETNGBL	R8	K2
      0x88201103,  //  002D  GETMBR	R8	R8	K3
      0xB8260400,  //  002E  GETNGBL	R9	K2
      0x88241304,  //  002F  GETMBR	R9	R9	K4
      0x00280805,  //  0030  ADD	R10	R4	R5
      0x0028150F,  //  0031  ADD	R10	R10	K15
      0x7C180800,  //  0032  CALL	R6	4
      0x80000000,  //  0033  RET	0
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