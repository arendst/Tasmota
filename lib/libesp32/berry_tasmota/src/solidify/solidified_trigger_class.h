/* Solidification of trigger_class.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Trigger_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(trig),
    /* K1   */  be_nested_str(f),
    /* K2   */  be_nested_str(id),
    /* K3   */  be_nested_str(o),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x90020604,  //  0003  SETMBR	R0	K3	R4
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Trigger_tostring,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(format),
    /* K2   */  be_nested_str(_X3Cinstance_X3A_X20_X25s_X28_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
    /* K3   */  be_nested_str(trig),
    /* K4   */  be_nested_str(f),
    /* K5   */  be_nested_str(id),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x60140008,  //  0003  GETGBL	R5	G8
      0x60180006,  //  0004  GETGBL	R6	G6
      0x5C1C0000,  //  0005  MOVE	R7	R0
      0x7C180200,  //  0006  CALL	R6	1
      0x7C140200,  //  0007  CALL	R5	1
      0x60180008,  //  0008  GETGBL	R6	G8
      0x881C0103,  //  0009  GETMBR	R7	R0	K3
      0x7C180200,  //  000A  CALL	R6	1
      0x601C0008,  //  000B  GETGBL	R7	G8
      0x88200104,  //  000C  GETMBR	R8	R0	K4
      0x7C1C0200,  //  000D  CALL	R7	1
      0x60200008,  //  000E  GETGBL	R8	G8
      0x88240105,  //  000F  GETMBR	R9	R0	K5
      0x7C200200,  //  0010  CALL	R8	1
      0x7C080C00,  //  0011  CALL	R2	6
      0x80040400,  //  0012  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: time_reached
********************************************************************/
be_local_closure(Trigger_time_reached,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(o),
    /* K1   */  be_nested_str(trig),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str(time_reached),
    }),
    &be_const_str_time_reached,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060007,  //  0001  JMPF	R1	#000A
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x24040302,  //  0003  GT	R1	R1	K2
      0x78060004,  //  0004  JMPF	R1	#000A
      0x88040100,  //  0005  GETMBR	R1	R0	K0
      0x8C040303,  //  0006  GETMET	R1	R1	K3
      0x880C0101,  //  0007  GETMBR	R3	R0	K1
      0x7C040400,  //  0008  CALL	R1	2
      0x80040200,  //  0009  RET	1	R1
      0x50040000,  //  000A  LDBOOL	R1	0	0
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next
********************************************************************/
be_local_closure(Trigger_next,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(o),
    /* K1   */  be_nested_str(next),
    }),
    &be_const_str_next,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060003,  //  0001  JMPF	R1	#0006
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x8C040301,  //  0003  GETMET	R1	R1	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x80040200,  //  0005  RET	1	R1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Trigger
********************************************************************/
be_local_class(Trigger,
    4,
    NULL,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(id, 2), be_const_var(2) },
        { be_const_key(f, -1), be_const_var(1) },
        { be_const_key(next, -1), be_const_closure(Trigger_next_closure) },
        { be_const_key(trig, 7), be_const_var(0) },
        { be_const_key(time_reached, -1), be_const_closure(Trigger_time_reached_closure) },
        { be_const_key(tostring, 4), be_const_closure(Trigger_tostring_closure) },
        { be_const_key(o, -1), be_const_var(3) },
        { be_const_key(init, -1), be_const_closure(Trigger_init_closure) },
    })),
    (bstring*) &be_const_str_Trigger
);
/*******************************************************************/

void be_load_Trigger_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Trigger);
    be_setglobal(vm, "Trigger");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
