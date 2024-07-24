/* Solidification of crypto_pbkdf2_hmac_sha256.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: _f
********************************************************************/
be_local_closure(_f,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
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
        /* K0   */  be_nested_str_weak(size),
        /* K1   */  be_const_int(0),
        /* K2   */  be_const_int(1),
        }),
        be_str_weak(xor),
        &be_const_str_solidified,
        ( &(const binstruction[19]) {  /* code */
          0x60080000,  //  0000  GETGBL	R2	G0
          0x8C0C0100,  //  0001  GETMET	R3	R0	K0
          0x7C0C0200,  //  0002  CALL	R3	1
          0x8C100300,  //  0003  GETMET	R4	R1	K0
          0x7C100200,  //  0004  CALL	R4	1
          0x1C0C0604,  //  0005  EQ	R3	R3	R4
          0x7C080200,  //  0006  CALL	R2	1
          0x8C080100,  //  0007  GETMET	R2	R0	K0
          0x7C080200,  //  0008  CALL	R2	1
          0x580C0001,  //  0009  LDCONST	R3	K1
          0x14100602,  //  000A  LT	R4	R3	R2
          0x78120005,  //  000B  JMPF	R4	#0012
          0x94100003,  //  000C  GETIDX	R4	R0	R3
          0x94140203,  //  000D  GETIDX	R5	R1	R3
          0x34100805,  //  000E  XOR	R4	R4	R5
          0x98000604,  //  000F  SETIDX	R0	R3	R4
          0x000C0702,  //  0010  ADD	R3	R3	K2
          0x7001FFF7,  //  0011  JMP		#000A
          0x80000000,  //  0012  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(HMAC_SHA256),
    /* K2   */  be_nested_str_weak(update),
    /* K3   */  be_nested_str_weak(out),
    /* K4   */  be_nested_str_weak(setbytes),
    /* K5   */  be_const_int(0),
    /* K6   */  be_const_int(2),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(yield),
    }),
    be_str_weak(_f),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x84100000,  //  0000  CLOSURE	R4	P0
      0xA4160000,  //  0001  IMPORT	R5	K0
      0x8C180B01,  //  0002  GETMET	R6	R5	K1
      0x5C200000,  //  0003  MOVE	R8	R0
      0x7C180400,  //  0004  CALL	R6	2
      0x8C1C0D02,  //  0005  GETMET	R7	R6	K2
      0x5C240200,  //  0006  MOVE	R9	R1
      0x7C1C0400,  //  0007  CALL	R7	2
      0x8C1C0D03,  //  0008  GETMET	R7	R6	K3
      0x7C1C0200,  //  0009  CALL	R7	1
      0x8C200704,  //  000A  GETMET	R8	R3	K4
      0x58280005,  //  000B  LDCONST	R10	K5
      0x5C2C0E00,  //  000C  MOVE	R11	R7
      0x7C200600,  //  000D  CALL	R8	3
      0x58200006,  //  000E  LDCONST	R8	K6
      0x18241002,  //  000F  LE	R9	R8	R2
      0x78260012,  //  0010  JMPF	R9	#0024
      0x8C240B01,  //  0011  GETMET	R9	R5	K1
      0x5C2C0000,  //  0012  MOVE	R11	R0
      0x7C240400,  //  0013  CALL	R9	2
      0x5C181200,  //  0014  MOVE	R6	R9
      0x8C240D02,  //  0015  GETMET	R9	R6	K2
      0x5C2C0E00,  //  0016  MOVE	R11	R7
      0x7C240400,  //  0017  CALL	R9	2
      0x8C240D03,  //  0018  GETMET	R9	R6	K3
      0x7C240200,  //  0019  CALL	R9	1
      0x5C1C1200,  //  001A  MOVE	R7	R9
      0x5C240800,  //  001B  MOVE	R9	R4
      0x5C280600,  //  001C  MOVE	R10	R3
      0x5C2C0E00,  //  001D  MOVE	R11	R7
      0x7C240400,  //  001E  CALL	R9	2
      0x00201107,  //  001F  ADD	R8	R8	K7
      0xB8261000,  //  0020  GETNGBL	R9	K8
      0x8C241309,  //  0021  GETMET	R9	R9	K9
      0x7C240200,  //  0022  CALL	R9	1
      0x7001FFEA,  //  0023  JMP		#000F
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: PBKDF2_HMAC_SHA256
********************************************************************/
be_local_closure(PBKDF2_HMAC_SHA256,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(fromstring),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(size),
    /* K4   */  be_nested_str_weak(copy),
    /* K5   */  be_nested_str_weak(add),
    /* K6   */  be_nested_str_weak(resize),
    /* K7   */  be_nested_str_weak(_f),
    /* K8   */  be_const_int(0),
    }),
    be_str_weak(PBKDF2_HMAC_SHA256),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x60140004,  //  0000  GETGBL	R5	G4
      0x5C180200,  //  0001  MOVE	R6	R1
      0x7C140200,  //  0002  CALL	R5	1
      0x1C140B00,  //  0003  EQ	R5	R5	K0
      0x78160005,  //  0004  JMPF	R5	#000B
      0x60140015,  //  0005  GETGBL	R5	G21
      0x7C140000,  //  0006  CALL	R5	0
      0x8C140B01,  //  0007  GETMET	R5	R5	K1
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x7C140400,  //  0009  CALL	R5	2
      0x5C040A00,  //  000A  MOVE	R1	R5
      0x60140004,  //  000B  GETGBL	R5	G4
      0x5C180400,  //  000C  MOVE	R6	R2
      0x7C140200,  //  000D  CALL	R5	1
      0x1C140B00,  //  000E  EQ	R5	R5	K0
      0x78160005,  //  000F  JMPF	R5	#0016
      0x60140015,  //  0010  GETGBL	R5	G21
      0x7C140000,  //  0011  CALL	R5	0
      0x8C140B01,  //  0012  GETMET	R5	R5	K1
      0x5C1C0400,  //  0013  MOVE	R7	R2
      0x7C140400,  //  0014  CALL	R5	2
      0x5C080A00,  //  0015  MOVE	R2	R5
      0x60140015,  //  0016  GETGBL	R5	G21
      0x7C140000,  //  0017  CALL	R5	0
      0x58180002,  //  0018  LDCONST	R6	K2
      0x8C1C0B03,  //  0019  GETMET	R7	R5	K3
      0x7C1C0200,  //  001A  CALL	R7	1
      0x141C0E04,  //  001B  LT	R7	R7	R4
      0x781E0013,  //  001C  JMPF	R7	#0031
      0x8C1C0504,  //  001D  GETMET	R7	R2	K4
      0x7C1C0200,  //  001E  CALL	R7	1
      0x8C200F05,  //  001F  GETMET	R8	R7	K5
      0x5C280C00,  //  0020  MOVE	R10	R6
      0x542DFFFB,  //  0021  LDINT	R11	-4
      0x7C200600,  //  0022  CALL	R8	3
      0x60200015,  //  0023  GETGBL	R8	G21
      0x7C200000,  //  0024  CALL	R8	0
      0x8C201106,  //  0025  GETMET	R8	R8	K6
      0x542A001F,  //  0026  LDINT	R10	32
      0x7C200400,  //  0027  CALL	R8	2
      0x8C240107,  //  0028  GETMET	R9	R0	K7
      0x5C2C0200,  //  0029  MOVE	R11	R1
      0x5C300E00,  //  002A  MOVE	R12	R7
      0x5C340600,  //  002B  MOVE	R13	R3
      0x5C381000,  //  002C  MOVE	R14	R8
      0x7C240A00,  //  002D  CALL	R9	5
      0x00140A08,  //  002E  ADD	R5	R5	R8
      0x00180D02,  //  002F  ADD	R6	R6	K2
      0x7001FFE7,  //  0030  JMP		#0019
      0x041C0902,  //  0031  SUB	R7	R4	K2
      0x401E1007,  //  0032  CONNECT	R7	K8	R7
      0x941C0A07,  //  0033  GETIDX	R7	R5	R7
      0x80040E00,  //  0034  RET	1	R7
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
