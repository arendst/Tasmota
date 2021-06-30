/********************************************************************
 * Tasmota lib
 * 
 * To use: `d = Driver()`
 * 
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
  class Driver2 : Driver
    var every_second
    var every_100ms
    var web_add_handler
    var web_add_button
    var web_add_main_button
    var web_add_management_button
    var web_add_config_button
    var web_add_console_button
    var save_before_restart
    var web_sensor
    var json_append
    var button_pressed
    var display

    def get_tasmota()
      import global
      return global.tasmota
    end

    def add_cmd(c, f)
      var tasmota = self.get_tasmota()
      tasmota.add_cmd(c, / cmd, idx, payload, payload_json -> f(self, cmd, idx, payload, payload_json))
    end
  end
********************************************************************/

/********************************************************************
** Solidified function: get_tasmota
********************************************************************/
be_local_closure(get_tasmota,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* upvals */
      { { .s=be_nested_const_str("global", 503252654, 6) }, BE_STRING},
      { { .s=be_nested_const_str("tasmota", 424643812, 7) }, BE_STRING},
    }),
    (be_nested_const_str("get_tasmota", 334356779, 11)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[ 3]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	R256
      0x88080301,  //  0001  GETMBR	R2	R1	R257
      0x80040400,  //  0002  RET	1	R2
    })
  )
);
/*******************************************************************/

/********************************************************************
** Solidified function: add_cmd
********************************************************************/
be_local_closure(add_cmd,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    be_nested_proto(
      10,                          /* nstack */
      4,                          /* argc */
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
      (be_nested_const_str("stdin", -1529146723, 5)),
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
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* upvals */
      { { .s=be_nested_const_str("get_tasmota", 334356779, 11) }, BE_STRING},
      { { .s=be_nested_const_str("add_cmd", -933336417, 7) }, BE_STRING},
    }),
    (be_nested_const_str("add_cmd", -933336417, 7)),
    (be_nested_const_str("stdin", -1529146723, 5)),
    ( &(const binstruction[ 8]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	R256
      0x7C0C0200,  //  0001  CALL	R3	1
      0x8C100701,  //  0002  GETMET	R4	R3	R257
      0x5C180200,  //  0003  MOVE	R6	R1
      0x841C0000,  //  0004  CLOSURE	R7	P0
      0x7C100600,  //  0005  CALL	R4	3
      0xA0000000,  //  0006  CLOSE	0
      0x80000000,  //  0007  RET	0	R0
    })
  )
);
/*******************************************************************/

#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_fixed_be_class_tasmota_driver.h"
#endif

void be_load_driverlib(bvm *vm) {
#if !BE_USE_PRECOMPILED_OBJECT
    static const bnfuncinfo members[] = {
        { "every_second", NULL },
        { "every_100ms", NULL },
        { "web_add_handler", NULL },
        { "web_add_button", NULL },
        { "web_add_main_button", NULL },
        { "web_add_management_button", NULL },
        { "web_add_config_button", NULL },
        { "web_add_console_button", NULL },
        { "save_before_restart", NULL },
        { "web_sensor", NULL },
        { "json_append", NULL },
        { "button_pressed", NULL },
        { "display", NULL },

        { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */
        { "get_tasmota", (bntvfunc) &get_tasmota_closure },
        { "add_cmd", (bntvfunc) &add_cmd_closure },
        
        { NULL, NULL }
    };
    be_regclass(vm, "Driver", members);
#else
    be_pushntvclass(vm, &be_class_tasmota_driver);
    be_setglobal(vm, "Driver");
    be_pop(vm, 1);
#endif
}
/* @const_object_info_begin

class be_class_tasmota_driver (scope: global, name: Driver) {
    every_second, var
    every_100ms, var
    web_add_handler, var
    web_add_button, var
    web_add_main_button, var
    web_add_management_button, var
    web_add_config_button, var
    web_add_console_button, var
    save_before_restart, var
    web_sensor, var
    json_append, var
    button_pressed, var
    display, var

    get_tasmota, closure(get_tasmota_closure)

    add_cmd, closure(add_cmd_closure)
}
@const_object_info_end */
