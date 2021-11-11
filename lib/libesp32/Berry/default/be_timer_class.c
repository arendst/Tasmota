/********************************************************************
 * Tasmota lib
 * 
 * class Timer
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(tostring,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_string("string", 398550328, 6),
    /* K1   */  be_nested_string("format", -1180859054, 6),
    /* K2   */  be_nested_string("<instance: %s(%s, %s, %s)", 257363333, 25),
    /* K3   */  be_nested_string("due", -399437003, 3),
    /* K4   */  be_nested_string("f", -485742695, 1),
    /* K5   */  be_nested_string("id", 926444256, 2),
    }),
    (be_nested_const_str("tostring", -1995258651, 8)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[19]) {  /* code */
      0xA4060000,  //  0000  IMPORT R1  K0
      0x8C080301,  //  0001  GETMET R2  R1  K1
      0x58100002,  //  0002  LDCONST  R4  K2
      0x60140008,  //  0003  GETGBL R5  G8
      0x60180006,  //  0004  GETGBL R6  G6
      0x5C1C0000,  //  0005  MOVE R7  R0
      0x7C180200,  //  0006  CALL R6  1
      0x7C140200,  //  0007  CALL R5  1
      0x60180008,  //  0008  GETGBL R6  G8
      0x881C0103,  //  0009  GETMBR R7  R0  K3
      0x7C180200,  //  000A  CALL R6  1
      0x601C0008,  //  000B  GETGBL R7  G8
      0x88200104,  //  000C  GETMBR R8  R0  K4
      0x7C1C0200,  //  000D  CALL R7  1
      0x60200008,  //  000E  GETGBL R8  G8
      0x88240105,  //  000F  GETMBR R9  R0  K5
      0x7C200200,  //  0010  CALL R8  1
      0x7C080C00,  //  0011  CALL R2  6
      0x80040400,  //  0012  RET  1 R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("due", -399437003, 3),
    /* K1   */  be_nested_string("f", -485742695, 1),
    /* K2   */  be_nested_string("id", 926444256, 2),
    }),
    ((bstring*) &be_const_str_init),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x90020001,  //  0000  SETMBR R0  K0  R1
      0x90020202,  //  0001  SETMBR R0  K1  R2
      0x90020403,  //  0002  SETMBR R0  K2  R3
      0x80000000,  //  0003  RET  0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Timer
********************************************************************/
be_local_class(Timer,
    3,
    NULL,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("tostring", -1995258651, 8, 4), be_const_closure(tostring_closure) },
        { be_nested_key("id", 926444256, 2, 2), be_const_var(2) },
        { be_nested_key("f", -485742695, 1, -1), be_const_var(1) },
        { be_nested_key("due", -399437003, 3, -1), be_const_var(0) },
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(init_closure) },
    })),
    (be_nested_const_str("Timer", -346839614, 5))
);
/*******************************************************************/

void be_load_Timer_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Timer);
    be_setglobal(vm, "Timer");
    be_pop(vm, 1);
}
