/* Solidification of uuid.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(uuid__anonymous_,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    0,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(math),
    /* K2   */  be_nested_str(_X2508x_X2D_X2504x_X2D_X2504x_X2D_X2504x_X2D_X2504x_X2508x),
    /* K3   */  be_nested_str(rand),
    }),
    &be_const_str__anonymous_,
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA4020000,  //  0000  IMPORT	R0	K0
      0xA4060200,  //  0001  IMPORT	R1	K1
      0x60080018,  //  0002  GETGBL	R2	G24
      0x580C0002,  //  0003  LDCONST	R3	K2
      0x8C100303,  //  0004  GETMET	R4	R1	K3
      0x7C100200,  //  0005  CALL	R4	1
      0x8C140303,  //  0006  GETMET	R5	R1	K3
      0x7C140200,  //  0007  CALL	R5	1
      0x541AFFFE,  //  0008  LDINT	R6	65535
      0x2C140A06,  //  0009  AND	R5	R5	R6
      0x8C180303,  //  000A  GETMET	R6	R1	K3
      0x7C180200,  //  000B  CALL	R6	1
      0x541E0FFE,  //  000C  LDINT	R7	4095
      0x2C180C07,  //  000D  AND	R6	R6	R7
      0x541E3FFF,  //  000E  LDINT	R7	16384
      0x30180C07,  //  000F  OR	R6	R6	R7
      0x8C1C0303,  //  0010  GETMET	R7	R1	K3
      0x7C1C0200,  //  0011  CALL	R7	1
      0x54223FFE,  //  0012  LDINT	R8	16383
      0x2C1C0E08,  //  0013  AND	R7	R7	R8
      0x54227FFF,  //  0014  LDINT	R8	32768
      0x301C0E08,  //  0015  OR	R7	R7	R8
      0x8C200303,  //  0016  GETMET	R8	R1	K3
      0x7C200200,  //  0017  CALL	R8	1
      0x5426FFFE,  //  0018  LDINT	R9	65535
      0x2C201009,  //  0019  AND	R8	R8	R9
      0x8C240303,  //  001A  GETMET	R9	R1	K3
      0x7C240200,  //  001B  CALL	R9	1
      0x7C080E00,  //  001C  CALL	R2	7
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
/********************************************************************/
/* End of solidification */
