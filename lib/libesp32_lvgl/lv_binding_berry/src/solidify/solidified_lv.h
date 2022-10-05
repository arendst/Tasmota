/* Solidification of lv.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: lv_module_init
********************************************************************/
be_local_closure(lv_module_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(lv),
    /* K1   */  be_nested_str_weak(member),
    }),
    be_str_weak(lv_module_init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x6004000B,  //  0000  GETGBL	R1	G11
      0x58080000,  //  0001  LDCONST	R2	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x90060202,  //  0004  SETMBR	R1	K1	R2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
