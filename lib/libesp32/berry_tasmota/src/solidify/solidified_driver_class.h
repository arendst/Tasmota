/* Solidification of driver_class.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Driver;

/********************************************************************
** Solidified function: add_cmd
********************************************************************/
be_local_closure(class_Driver_add_cmd,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
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
        &be_const_str__X3Clambda_X3E,
        &be_const_str_solidified,
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
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(add_cmd),
    }),
    &be_const_str_add_cmd,
    &be_const_str_solidified,
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
    14,
    NULL,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(web_add_config_button, 12), be_const_var(6) },
        { be_const_key(web_add_main_button, -1), be_const_var(4) },
        { be_const_key(web_add_handler, 10), be_const_var(2) },
        { be_const_key(save_before_restart, 5), be_const_var(8) },
        { be_const_key(add_cmd, -1), be_const_closure(class_Driver_add_cmd_closure) },
        { be_const_key(web_add_button, -1), be_const_var(3) },
        { be_const_key(web_add_management_button, -1), be_const_var(5) },
        { be_const_key(display, -1), be_const_var(13) },
        { be_const_key(after_teleperiod, -1), be_const_var(11) },
        { be_const_key(every_100ms, -1), be_const_var(1) },
        { be_const_key(web_add_console_button, 13), be_const_var(7) },
        { be_const_key(button_pressed, -1), be_const_var(12) },
        { be_const_key(every_second, -1), be_const_var(0) },
        { be_const_key(web_sensor, -1), be_const_var(9) },
        { be_const_key(json_append, 1), be_const_var(10) },
    })),
    (bstring*) &be_const_str_Driver
);
/********************************************************************/
/* End of solidification */
