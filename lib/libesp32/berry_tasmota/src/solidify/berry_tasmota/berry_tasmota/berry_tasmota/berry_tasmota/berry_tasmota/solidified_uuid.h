/* Solidification of uuid.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(_anonymous_,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(math),
    /* K1   */  be_nested_str(_X2508x_X2D_X2504x_X2D_X2504x_X2D_X2504x_X2D_X2504x_X2508x),
    /* K2   */  be_nested_str(rand),
    }),
    &be_const_str__anonymous_,
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0xA4020000,  //  0000  IMPORT	R0	K0
      0x60040018,  //  0001  GETGBL	R1	G24
      0x58080001,  //  0002  LDCONST	R2	K1
      0x8C0C0102,  //  0003  GETMET	R3	R0	K2
      0x7C0C0200,  //  0004  CALL	R3	1
      0x8C100102,  //  0005  GETMET	R4	R0	K2
      0x7C100200,  //  0006  CALL	R4	1
      0x5416FFFE,  //  0007  LDINT	R5	65535
      0x2C100805,  //  0008  AND	R4	R4	R5
      0x8C140102,  //  0009  GETMET	R5	R0	K2
      0x7C140200,  //  000A  CALL	R5	1
      0x541A0FFE,  //  000B  LDINT	R6	4095
      0x2C140A06,  //  000C  AND	R5	R5	R6
      0x541A3FFF,  //  000D  LDINT	R6	16384
      0x30140A06,  //  000E  OR	R5	R5	R6
      0x8C180102,  //  000F  GETMET	R6	R0	K2
      0x7C180200,  //  0010  CALL	R6	1
      0x541E3FFE,  //  0011  LDINT	R7	16383
      0x2C180C07,  //  0012  AND	R6	R6	R7
      0x541E7FFF,  //  0013  LDINT	R7	32768
      0x30180C07,  //  0014  OR	R6	R6	R7
      0x8C1C0102,  //  0015  GETMET	R7	R0	K2
      0x7C1C0200,  //  0016  CALL	R7	1
      0x5422FFFE,  //  0017  LDINT	R8	65535
      0x2C1C0E08,  //  0018  AND	R7	R7	R8
      0x8C200102,  //  0019  GETMET	R8	R0	K2
      0x7C200200,  //  001A  CALL	R8	1
      0x7C040E00,  //  001B  CALL	R1	7
      0x80040200,  //  001C  RET	1	R1
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
        { be_const_key(uuid4, -1), be_const_closure(_anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(uuid);
/********************************************************************/
/********************************************************************/
/* End of solidification */
