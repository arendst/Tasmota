/* Solidification of Matter_Module.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(module_matter_setmember,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(_X2Ematter),
    }),
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x58140002,  //  0002  LDCONST	R5	K2
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0002,  //  0004  JMPT	R3	#0008
      0x600C0013,  //  0005  GETGBL	R3	G19
      0x7C0C0000,  //  0006  CALL	R3	0
      0x900A0403,  //  0007  SETMBR	R2	K2	R3
      0x880C0502,  //  0008  GETMBR	R3	R2	K2
      0x980C0001,  //  0009  SETIDX	R3	R0	R1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(module_matter_member,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(undefined),
    /* K2   */  be_nested_str_weak(contains),
    /* K3   */  be_nested_str_weak(_X2Ematter),
    }),
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x880C0303,  //  0006  GETMBR	R3	R1	K3
      0x8C0C0702,  //  0007  GETMET	R3	R3	K2
      0x5C140000,  //  0008  MOVE	R5	R0
      0x7C0C0400,  //  0009  CALL	R3	2
      0x780E0003,  //  000A  JMPF	R3	#000F
      0x880C0303,  //  000B  GETMBR	R3	R1	K3
      0x940C0600,  //  000C  GETIDX	R3	R3	R0
      0x80040600,  //  000D  RET	1	R3
      0x70020000,  //  000E  JMP		#0010
      0x80040400,  //  000F  RET	1	R2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
