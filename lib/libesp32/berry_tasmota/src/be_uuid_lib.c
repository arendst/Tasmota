/********************************************************************
 * uuid module
 * 
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(uuid__anonymous_,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(math),
    /* K2   */  be_nested_str(format),
    /* K3   */  be_nested_str(_X2508x_X2D_X2504x_X2D_X2504x_X2D_X2504x_X2D_X2504x_X2508x),
    /* K4   */  be_nested_str(rand),
    }),
    &be_const_str__anonymous_,
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA4020000,  //  0000  IMPORT	R0	K0
      0xA4060200,  //  0001  IMPORT	R1	K1
      0x8C080102,  //  0002  GETMET	R2	R0	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x8C140304,  //  0004  GETMET	R5	R1	K4
      0x7C140200,  //  0005  CALL	R5	1
      0x8C180304,  //  0006  GETMET	R6	R1	K4
      0x7C180200,  //  0007  CALL	R6	1
      0x541EFFFE,  //  0008  LDINT	R7	65535
      0x2C180C07,  //  0009  AND	R6	R6	R7
      0x8C1C0304,  //  000A  GETMET	R7	R1	K4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x54220FFE,  //  000C  LDINT	R8	4095
      0x2C1C0E08,  //  000D  AND	R7	R7	R8
      0x54223FFF,  //  000E  LDINT	R8	16384
      0x301C0E08,  //  000F  OR	R7	R7	R8
      0x8C200304,  //  0010  GETMET	R8	R1	K4
      0x7C200200,  //  0011  CALL	R8	1
      0x54263FFE,  //  0012  LDINT	R9	16383
      0x2C201009,  //  0013  AND	R8	R8	R9
      0x54267FFF,  //  0014  LDINT	R9	32768
      0x30201009,  //  0015  OR	R8	R8	R9
      0x8C240304,  //  0016  GETMET	R9	R1	K4
      0x7C240200,  //  0017  CALL	R9	1
      0x542AFFFE,  //  0018  LDINT	R10	65535
      0x2C24120A,  //  0019  AND	R9	R9	R10
      0x8C280304,  //  001A  GETMET	R10	R1	K4
      0x7C280200,  //  001B  CALL	R10	1
      0x7C081000,  //  001C  CALL	R2	8
      0x80040400,  //  001D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: uuid
********************************************************************/
be_local_module(uuid,
    "uuid",
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(uuid4, -1), be_const_closure(uuid__anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(uuid);
/********************************************************************/
