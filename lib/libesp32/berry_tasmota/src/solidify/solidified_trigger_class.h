/* Solidification of trigger_class.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Trigger' ktab size: 8, total: 14 (saved 48 bytes)
static const bvalue be_ktab_class_Trigger[8] = {
  /* K0   */  be_nested_str(trig),
  /* K1   */  be_nested_str(f),
  /* K2   */  be_nested_str(id),
  /* K3   */  be_nested_str(o),
  /* K4   */  be_nested_str(_X3Cinstance_X3A_X20_X25s_X28_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str(time_reached),
  /* K7   */  be_nested_str(next),
};


extern const bclass be_class_Trigger;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Trigger_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Trigger,     /* shared constants */
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
be_local_closure(class_Trigger_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Trigger,     /* shared constants */
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080004,  //  0001  LDCONST	R2	K4
      0x600C0008,  //  0002  GETGBL	R3	G8
      0x60100006,  //  0003  GETGBL	R4	G6
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C100200,  //  0005  CALL	R4	1
      0x7C0C0200,  //  0006  CALL	R3	1
      0x60100008,  //  0007  GETGBL	R4	G8
      0x88140100,  //  0008  GETMBR	R5	R0	K0
      0x7C100200,  //  0009  CALL	R4	1
      0x60140008,  //  000A  GETGBL	R5	G8
      0x88180101,  //  000B  GETMBR	R6	R0	K1
      0x7C140200,  //  000C  CALL	R5	1
      0x60180008,  //  000D  GETGBL	R6	G8
      0x881C0102,  //  000E  GETMBR	R7	R0	K2
      0x7C180200,  //  000F  CALL	R6	1
      0x7C040A00,  //  0010  CALL	R1	5
      0x80040200,  //  0011  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: time_reached
********************************************************************/
be_local_closure(class_Trigger_time_reached,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Trigger,     /* shared constants */
    &be_const_str_time_reached,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x78060007,  //  0001  JMPF	R1	#000A
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x24040305,  //  0003  GT	R1	R1	K5
      0x78060004,  //  0004  JMPF	R1	#000A
      0x88040103,  //  0005  GETMBR	R1	R0	K3
      0x8C040306,  //  0006  GETMET	R1	R1	K6
      0x880C0100,  //  0007  GETMBR	R3	R0	K0
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
be_local_closure(class_Trigger_next,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Trigger,     /* shared constants */
    &be_const_str_next,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x78060003,  //  0001  JMPF	R1	#0006
      0x88040103,  //  0002  GETMBR	R1	R0	K3
      0x8C040307,  //  0003  GETMET	R1	R1	K7
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
        { be_const_key(next, -1), be_const_closure(class_Trigger_next_closure) },
        { be_const_key(trig, 7), be_const_var(0) },
        { be_const_key(time_reached, -1), be_const_closure(class_Trigger_time_reached_closure) },
        { be_const_key(tostring, 4), be_const_closure(class_Trigger_tostring_closure) },
        { be_const_key(o, -1), be_const_var(3) },
        { be_const_key(init, -1), be_const_closure(class_Trigger_init_closure) },
    })),
    (bstring*) &be_const_str_Trigger
);
/********************************************************************/
/* End of solidification */
