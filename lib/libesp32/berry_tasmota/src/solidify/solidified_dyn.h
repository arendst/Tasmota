/* Solidification of dyn.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_dyn;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(dyn_tostring,   /* name */
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
    /* K0   */  be_nested_str(_attr),
    /* K1   */  be_nested_str(tostring),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(dyn_member,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_attr),
    /* K1   */  be_nested_str(contains),
    /* K2   */  be_nested_str(undefined),
    }),
    &be_const_str_member,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x88080100,  //  0005  GETMBR	R2	R0	K0
      0x94080401,  //  0006  GETIDX	R2	R2	R1
      0x80040400,  //  0007  RET	1	R2
      0x70020001,  //  0008  JMP		#000B
      0xA40A0400,  //  0009  IMPORT	R2	K2
      0x80040400,  //  000A  RET	1	R2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(dyn_setmember,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(_attr),
    }),
    &be_const_str_setmember,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(dyn_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(_attr),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: dyn
********************************************************************/
be_local_class(dyn,
    1,
    NULL,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(tostring, 2), be_const_closure(dyn_tostring_closure) },
        { be_const_key(member, 3), be_const_closure(dyn_member_closure) },
        { be_const_key(init, 4), be_const_closure(dyn_init_closure) },
        { be_const_key(setmember, -1), be_const_closure(dyn_setmember_closure) },
        { be_const_key(_attr, -1), be_const_var(0) },
    })),
    (bstring*) &be_const_str_dyn
);
/*******************************************************************/

void be_load_dyn_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_dyn);
    be_setglobal(vm, "dyn");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
