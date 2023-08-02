/* Solidification of zigbee.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(zb_device_tostring,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(longaddr),
    /* K2   */  be_nested_str_weak(_X3Cinstance_X3A_X20_X25s_X280x_X2504X_X2C_X200x_X2508X_X2508X_X2C_X20name_X3A_X27_X25s_X27_X2C_X20model_X3A_X27_X25s_X27_X2C_X20manufacturer_X3A_X27_X25s_X27_X29_X3E),
    /* K3   */  be_nested_str_weak(shortaddr),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(name),
    /* K7   */  be_nested_str_weak(model),
    /* K8   */  be_nested_str_weak(manufacturer),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x600C0018,  //  0002  GETGBL	R3	G24
      0x58100002,  //  0003  LDCONST	R4	K2
      0x60140005,  //  0004  GETGBL	R5	G5
      0x5C180000,  //  0005  MOVE	R6	R0
      0x7C140200,  //  0006  CALL	R5	1
      0x88180103,  //  0007  GETMBR	R6	R0	K3
      0x8C1C0504,  //  0008  GETMET	R7	R2	K4
      0x54260003,  //  0009  LDINT	R9	4
      0x542A0003,  //  000A  LDINT	R10	4
      0x7C1C0600,  //  000B  CALL	R7	3
      0x8C200504,  //  000C  GETMET	R8	R2	K4
      0x58280005,  //  000D  LDCONST	R10	K5
      0x542E0003,  //  000E  LDINT	R11	4
      0x7C200600,  //  000F  CALL	R8	3
      0x88240106,  //  0010  GETMBR	R9	R0	K6
      0x88280107,  //  0011  GETMBR	R10	R0	K7
      0x882C0108,  //  0012  GETMBR	R11	R0	K8
      0x7C0C1000,  //  0013  CALL	R3	8
      0x80040600,  //  0014  RET	1	R3
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
