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
    8,                          /* nstack */
    3,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
      be_nested_string("global", 503252654, 6),    /* R256 - K0 */
      be_nested_string("lv_signal_bars", -780994737, 14),    /* R257 - K1 */
      be_nested_string("init", 380752755, 4),    /* R258 - K2 */
      be_nested_string("tasmota", 424643812, 7),    /* R259 - K3 */
      be_nested_string("add_driver", 1654458371, 10),    /* R260 - K4 */
    }),
    (be_nested_const_str("init", 380752755, 4)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[14]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	R256
      0x60100014,  //  0001  GETGBL	R4	G20
      0x5C140000,  //  0002  MOVE	R5	R0
      0x88180701,  //  0003  GETMBR	R6	R3	R257
      0x7C100400,  //  0004  CALL	R4	2
      0x8C100902,  //  0005  GETMET	R4	R4	R258
      0x5C180200,  //  0006  MOVE	R6	R1
      0x5C1C0400,  //  0007  MOVE	R7	R2
      0x7C100600,  //  0008  CALL	R4	3
      0x88100703,  //  0009  GETMBR	R4	R3	R259
      0x8C100904,  //  000A  GETMET	R4	R4	R260
      0x5C180000,  //  000B  MOVE	R6	R0
      0x7C100400,  //  000C  CALL	R4	2
      0x80000000,  //  000D  RET	0	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
      be_nested_string("global", 503252654, 6),    /* R256 - K0 */
      be_nested_string("tasmota", 424643812, 7),    /* R257 - K1 */
      be_nested_string("wifi", 120087624, 4),    /* R258 - K2 */
      be_nested_string("find", -1108310694, 4),    /* R259 - K3 */
      be_nested_string("quality", -1697296346, 7),    /* R260 - K4 */
      be_nested_string("set_percentage", -1342944572, 14),    /* R261 - K5 */
    }),
    (be_nested_const_str("every_second", 2075451465, 12)),
    (be_nested_const_str("input", -103256197, 5)),
    ( &(const binstruction[14]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	R256
      0x88080301,  //  0001  GETMBR	R2	R1	R257
      0x8C080502,  //  0002  GETMET	R2	R2	R258
      0x7C080200,  //  0003  CALL	R2	1
      0x8C0C0503,  //  0004  GETMET	R3	R2	R259
      0x58140004,  //  0005  LDCONST	R5	K4
      0x7C0C0400,  //  0006  CALL	R3	2
      0x4C100000,  //  0007  LDNIL	4
      0x20100604,  //  0008  NE	R4	R3	R4
      0x78120002,  //  0009  JMPF	R4	#000D
      0x8C100105,  //  000A  GETMET	R4	R0	R261
      0x5C180600,  //  000B  MOVE	R6	R3
      0x7C100400,  //  000C  CALL	R4	2
      0x80000000,  //  000D  RET	0	R0
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