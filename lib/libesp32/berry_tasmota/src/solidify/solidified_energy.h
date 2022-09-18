/* Solidification of energy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(energy_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(_energy),
    /* K2   */  be_nested_str(energy_struct),
    /* K3   */  be_nested_str(_ptr),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0400,  //  0001  GETNGBL	R2	K2
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x7C080200,  //  0003  CALL	R2	1
      0x90060202,  //  0004  SETMBR	R1	K1	R2
      0x80040000,  //  0005  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read
********************************************************************/
be_local_closure(energy_read,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(_energy),
    /* K1   */  be_nested_str(tomap),
    }),
    &be_const_str_read,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0xB8020000,  //  0000  GETNGBL	R0	K0
      0x8C000101,  //  0001  GETMET	R0	R0	K1
      0x7C000200,  //  0002  CALL	R0	1
      0x80040000,  //  0003  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(energy_member,   /* name */
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
    /* K0   */  be_nested_str(_energy),
    }),
    &be_const_str_member,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040200,  //  0001  GETMBR	R1	R1	R0
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(energy_setmember,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(_energy),
    }),
    &be_const_str_setmember,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x90080001,  //  0001  SETMBR	R2	R0	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
