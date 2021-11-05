/********************************************************************
 * Tasmota lib
 * 
 * To use: `d = Driver()`
 * 
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    ((bstring*) &be_const_str_init),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_tasmota
********************************************************************/
be_local_closure(get_tasmota,   /* name */
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
    /* K0   */  be_nested_string("tasmota", 424643812, 7),
    }),
    (be_nested_const_str("get_tasmota", 334356779, 11)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 2]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_cmd
********************************************************************/
be_local_closure(add_cmd,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        10,                          /* nstack */
        4,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 2),
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        (be_nested_const_str("<lambda>", 607256038, 8)),
        ((bstring*) &be_const_str_input),
        ( &(const binstruction[ 8]) {  /* code */
          0x68100000,  //  0000  GETUPV	R4	U0
          0x68140001,  //  0001  GETUPV	R5	U1
          0x5C180000,  //  0002  MOVE	R6	R0
          0x5C1C0200,  //  0003  MOVE	R7	R1
          0x5C200400,  //  0004  MOVE	R8	R2
          0x5C240600,  //  0005  MOVE	R9	R3
          0x7C100A00,  //  0006  CALL	R4	5
          0x80040800,  //  0007  RET	1	R4
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("tasmota", 424643812, 7),
    /* K1   */  be_nested_string("add_cmd", -933336417, 7),
    }),
    (be_nested_const_str("add_cmd", -933336417, 7)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 7]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x84180000,  //  0003  CLOSURE	R6	P0
      0x7C0C0600,  //  0004  CALL	R3	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Driver
********************************************************************/
be_local_class(Driver,
    13,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("web_add_main_button", -334599632, 19, 14), be_const_var(4) },
        { be_nested_key("web_add_console_button", -813531104, 22, -1), be_const_var(7) },
        { be_nested_key("web_add_management_button", -1556090110, 25, 8), be_const_var(5) },
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(init_closure) },
        { be_nested_key("json_append", -1292948012, 11, -1), be_const_var(10) },
        { be_nested_key("web_add_config_button", 639674325, 21, 7), be_const_var(6) },
        { be_nested_key("every_100ms", 1546407804, 11, -1), be_const_var(1) },
        { be_nested_key("display", 1164572437, 7, -1), be_const_var(12) },
        { be_nested_key("web_add_button", -757092238, 14, 13), be_const_var(3) },
        { be_nested_key("every_second", 2075451465, 12, -1), be_const_var(0) },
        { be_nested_key("save_before_restart", 1253239338, 19, -1), be_const_var(8) },
        { be_nested_key("get_tasmota", 334356779, 11, -1), be_const_closure(get_tasmota_closure) },
        { be_nested_key("web_sensor", -1394870324, 10, 6), be_const_var(9) },
        { be_nested_key("web_add_handler", -304792334, 15, -1), be_const_var(2) },
        { be_nested_key("button_pressed", 1694209616, 14, 1), be_const_var(11) },
        { be_nested_key("add_cmd", -933336417, 7, -1), be_const_closure(add_cmd_closure) },
    })),
    (be_nested_const_str("Driver", -718580993, 6))
);
/*******************************************************************/

void be_load_Driver_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Driver);
    be_setglobal(vm, "Driver");
    be_pop(vm, 1);
}
