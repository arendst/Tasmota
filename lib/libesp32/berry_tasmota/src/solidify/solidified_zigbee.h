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
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(longaddr),
    /* K3   */  be_nested_str_weak(_X3Cinstance_X3A_X20_X25s_X280x_X2504X_X2C_X200x_X2508X_X2508X_X2C_X20name_X3A_X27_X25s_X27_X2C_X20model_X3A_X27_X25s_X27_X2C_X20manufacturer_X3A_X27_X25s_X27_X29_X3E),
    /* K4   */  be_nested_str_weak(shortaddr),
    /* K5   */  be_nested_str_weak(get),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(name),
    /* K8   */  be_nested_str_weak(model),
    /* K9   */  be_nested_str_weak(manufacturer),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x60100018,  //  0003  GETGBL	R4	G24
      0x58140003,  //  0004  LDCONST	R5	K3
      0x60180005,  //  0005  GETGBL	R6	G5
      0x5C1C0000,  //  0006  MOVE	R7	R0
      0x7C180200,  //  0007  CALL	R6	1
      0x881C0104,  //  0008  GETMBR	R7	R0	K4
      0x8C200705,  //  0009  GETMET	R8	R3	K5
      0x542A0003,  //  000A  LDINT	R10	4
      0x542E0003,  //  000B  LDINT	R11	4
      0x7C200600,  //  000C  CALL	R8	3
      0x8C240705,  //  000D  GETMET	R9	R3	K5
      0x582C0006,  //  000E  LDCONST	R11	K6
      0x54320003,  //  000F  LDINT	R12	4
      0x7C240600,  //  0010  CALL	R9	3
      0x88280107,  //  0011  GETMBR	R10	R0	K7
      0x882C0108,  //  0012  GETMBR	R11	R0	K8
      0x88300109,  //  0013  GETMBR	R12	R0	K9
      0x7C101000,  //  0014  CALL	R4	8
      0x80040800,  //  0015  RET	1	R4
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
