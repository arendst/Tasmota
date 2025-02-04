/* Solidification of Wire.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: read_bytes
********************************************************************/
be_local_closure(class_Wire_read_bytes,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(_begin_transmission),
    /* K1   */  be_nested_str(_write),
    /* K2   */  be_nested_str(_end_transmission),
    /* K3   */  be_nested_str(_request_from),
    /* K4   */  be_nested_str(_available),
    /* K5   */  be_nested_str(append),
    /* K6   */  be_nested_str(_read),
    }),
    &be_const_str_read_bytes,
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x7C100400,  //  0002  CALL	R4	2
      0x8C100101,  //  0003  GETMET	R4	R0	K1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x7C100400,  //  0005  CALL	R4	2
      0x8C100102,  //  0006  GETMET	R4	R0	K2
      0x50180200,  //  0007  LDBOOL	R6	1	0
      0x7C100400,  //  0008  CALL	R4	2
      0x8C100103,  //  0009  GETMET	R4	R0	K3
      0x5C180200,  //  000A  MOVE	R6	R1
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C100600,  //  000C  CALL	R4	3
      0x60100015,  //  000D  GETGBL	R4	G21
      0x5C140600,  //  000E  MOVE	R5	R3
      0x7C100200,  //  000F  CALL	R4	1
      0x8C140104,  //  0010  GETMET	R5	R0	K4
      0x7C140200,  //  0011  CALL	R5	1
      0x78160004,  //  0012  JMPF	R5	#0018
      0x8C140905,  //  0013  GETMET	R5	R4	K5
      0x8C1C0106,  //  0014  GETMET	R7	R0	K6
      0x7C1C0200,  //  0015  CALL	R7	1
      0x7C140400,  //  0016  CALL	R5	2
      0x7001FFF7,  //  0017  JMP		#0010
      0x8C140102,  //  0018  GETMET	R5	R0	K2
      0x501C0200,  //  0019  LDBOOL	R7	1	0
      0x7C140400,  //  001A  CALL	R5	2
      0x80040800,  //  001B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_bytes
********************************************************************/
be_local_closure(class_Wire_write_bytes,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_begin_transmission),
    /* K1   */  be_nested_str(_write),
    /* K2   */  be_nested_str(_end_transmission),
    }),
    &be_const_str_write_bytes,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x7C100400,  //  0002  CALL	R4	2
      0x8C100101,  //  0003  GETMET	R4	R0	K1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x7C100400,  //  0005  CALL	R4	2
      0x8C100101,  //  0006  GETMET	R4	R0	K1
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0x8C100102,  //  0009  GETMET	R4	R0	K2
      0x7C100200,  //  000A  CALL	R4	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
