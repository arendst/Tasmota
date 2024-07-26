/* Solidification of python_compat.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(_anonymous_,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(True),
    /* K2   */  be_nested_str(False),
    /* K3   */  be_nested_str(None),
    /* K4   */  be_nested_str(b),
    }),
    &be_const_str__anonymous_,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x50080200,  //  0001  LDBOOL	R2	1	0
      0x90060202,  //  0002  SETMBR	R1	K1	R2
      0x50080000,  //  0003  LDBOOL	R2	0	0
      0x90060402,  //  0004  SETMBR	R1	K2	R2
      0x4C080000,  //  0005  LDNIL	R2
      0x90060602,  //  0006  SETMBR	R1	K3	R2
      0x60080015,  //  0007  GETGBL	R2	G21
      0x90060802,  //  0008  SETMBR	R1	K4	R2
      0x80040000,  //  0009  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: python_compat
********************************************************************/
be_local_module(python_compat,
    "python_compat",
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(_anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(python_compat);
/********************************************************************/
/********************************************************************/
/* End of solidification */
