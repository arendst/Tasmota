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
    14,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(longaddr),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X3Cinstance_X3A_X20_X25s_X280x_X2504X_X2C_X200x_X2508X_X2508X_X2C_X20name_X3A_X27_X25s_X27_X2C_X20model_X3A_X27_X25s_X27_X2C_X20manufacturer_X3A_X27_X25s_X27_X29_X3E),
    /* K5   */  be_nested_str_weak(shortaddr),
    /* K6   */  be_nested_str_weak(get),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(name),
    /* K9   */  be_nested_str_weak(model),
    /* K10  */  be_nested_str_weak(manufacturer),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x8C100503,  //  0003  GETMET	R4	R2	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x601C0005,  //  0005  GETGBL	R7	G5
      0x5C200000,  //  0006  MOVE	R8	R0
      0x7C1C0200,  //  0007  CALL	R7	1
      0x88200105,  //  0008  GETMBR	R8	R0	K5
      0x8C240706,  //  0009  GETMET	R9	R3	K6
      0x542E0003,  //  000A  LDINT	R11	4
      0x54320003,  //  000B  LDINT	R12	4
      0x7C240600,  //  000C  CALL	R9	3
      0x8C280706,  //  000D  GETMET	R10	R3	K6
      0x58300007,  //  000E  LDCONST	R12	K7
      0x54360003,  //  000F  LDINT	R13	4
      0x7C280600,  //  0010  CALL	R10	3
      0x882C0108,  //  0011  GETMBR	R11	R0	K8
      0x88300109,  //  0012  GETMBR	R12	R0	K9
      0x8834010A,  //  0013  GETMBR	R13	R0	K10
      0x7C101200,  //  0014  CALL	R4	9
      0x80040800,  //  0015  RET	1	R4
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
