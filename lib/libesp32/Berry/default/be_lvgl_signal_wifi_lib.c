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
    7,                          /* nstack */
    3,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
      be_nested_string("lv_signal_bars", -780994737, 14),    /* R256 - K0 */
      be_nested_string("init", 380752755, 4),    /* R257 - K1 */
      be_nested_string("tasmota", 424643812, 7),    /* R258 - K2 */
      be_nested_string("add_driver", 1654458371, 10),    /* R259 - K3 */
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[13]) {  /* code */
      0x600C0014,  //  0000  GETGBL	R3	G20
      0x5C100000,  //  0001  MOVE	R4	R0
      0xB8160000,  //  0002  GETNGBL	R5	R256
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C0C0701,  //  0004  GETMET	R3	R3	R257
      0x5C140200,  //  0005  MOVE	R5	R1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C0C0600,  //  0007  CALL	R3	3
      0xB80E0400,  //  0008  GETNGBL	R3	R258
      0x8C0C0703,  //  0009  GETMET	R3	R3	R259
      0x5C140000,  //  000A  MOVE	R5	R0
      0x7C0C0400,  //  000B  CALL	R3	2
      0x80000000,  //  000C  RET	0	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(every_second,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
      be_nested_string("tasmota", 424643812, 7),    /* R256 - K0 */
      be_nested_string("wifi", 120087624, 4),    /* R257 - K1 */
      be_nested_string("find", -1108310694, 4),    /* R258 - K2 */
      be_nested_string("quality", -1697296346, 7),    /* R259 - K3 */
      be_nested_string("set_percentage", -1342944572, 14),    /* R260 - K4 */
    }),
    (be_nested_const_str("every_second", 2075451465, 12)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[13]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	R256
      0x8C040301,  //  0001  GETMET	R1	R1	R257
      0x7C040200,  //  0002  CALL	R1	1
      0x8C080302,  //  0003  GETMET	R2	R1	R258
      0x58100003,  //  0004  LDCONST	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x4C0C0000,  //  0006  LDNIL	3
      0x200C0403,  //  0007  NE	R3	R2	R3
      0x780E0002,  //  0008  JMPF	R3	#000C
      0x8C0C0104,  //  0009  GETMET	R3	R0	R260
      0x5C140400,  //  000A  MOVE	R5	R2
      0x7C0C0400,  //  000B  CALL	R3	2
      0x80000000,  //  000C  RET	0	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_wifi_bars
********************************************************************/
extern const bclass be_class_lv_signal_bars;
be_local_class(lv_wifi_bars,
    0,
    &be_class_lv_signal_bars,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(init_closure) },
        { be_nested_key("every_second", 2075451465, 12, 0), be_const_closure(every_second_closure) },
    })),
    (be_nested_const_str("lv_wifi_bars", 2109539196, 12))
);
/*******************************************************************/

void be_load_lv_wifi_bars_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_wifi_bars);
    be_setglobal(vm, "lv_wifi_bars");
    be_pop(vm, 1);
}

#endif // USE_LVGL