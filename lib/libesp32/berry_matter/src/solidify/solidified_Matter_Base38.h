/* Solidification of Matter_Base38.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Base38;

/********************************************************************
** Solidified function: encode
********************************************************************/
be_local_closure(class_Matter_Base38_encode,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
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
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str_weak(0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_X2D_X2E),
        /* K1   */  be_const_int(0),
        /* K2   */  be_nested_str_weak(),
        /* K3   */  be_const_int(1),
        }),
        be_str_weak(b38_enc),
        &be_const_str_solidified,
        ( &(const binstruction[14]) {  /* code */
          0x58080000,  //  0000  LDCONST	R2	K0
          0x580C0001,  //  0001  LDCONST	R3	K1
          0x58100002,  //  0002  LDCONST	R4	K2
          0x14140601,  //  0003  LT	R5	R3	R1
          0x78160007,  //  0004  JMPF	R5	#000D
          0x54160025,  //  0005  LDINT	R5	38
          0x10140005,  //  0006  MOD	R5	R0	R5
          0x94140405,  //  0007  GETIDX	R5	R2	R5
          0x00100805,  //  0008  ADD	R4	R4	R5
          0x54160025,  //  0009  LDINT	R5	38
          0x0C000005,  //  000A  DIV	R0	R0	R5
          0x000C0703,  //  000B  ADD	R3	R3	K3
          0x7001FFF5,  //  000C  JMP		#0003
          0x80040800,  //  000D  RET	1	R4
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Base38),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_const_int(2),
    /* K4   */  be_const_int(1),
    /* K5   */  be_const_int(3),
    }),
    be_str_weak(encode),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x84080000,  //  0001  CLOSURE	R2	P0
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C100200,  //  0005  CALL	R4	1
      0x58140002,  //  0006  LDCONST	R5	K2
      0x14180604,  //  0007  LT	R6	R3	R4
      0x781A002F,  //  0008  JMPF	R6	#0039
      0x4C180000,  //  0009  LDNIL	R6
      0x001C0703,  //  000A  ADD	R7	R3	K3
      0x141C0E04,  //  000B  LT	R7	R7	R4
      0x781E0012,  //  000C  JMPF	R7	#0020
      0x941C0003,  //  000D  GETIDX	R7	R0	R3
      0x00200704,  //  000E  ADD	R8	R3	K4
      0x94200008,  //  000F  GETIDX	R8	R0	R8
      0x54260007,  //  0010  LDINT	R9	8
      0x38201009,  //  0011  SHL	R8	R8	R9
      0x301C0E08,  //  0012  OR	R7	R7	R8
      0x00200703,  //  0013  ADD	R8	R3	K3
      0x94200008,  //  0014  GETIDX	R8	R0	R8
      0x5426000F,  //  0015  LDINT	R9	16
      0x38201009,  //  0016  SHL	R8	R8	R9
      0x301C0E08,  //  0017  OR	R7	R7	R8
      0x5C180E00,  //  0018  MOVE	R6	R7
      0x5C1C0400,  //  0019  MOVE	R7	R2
      0x5C200C00,  //  001A  MOVE	R8	R6
      0x54260004,  //  001B  LDINT	R9	5
      0x7C1C0400,  //  001C  CALL	R7	2
      0x00140A07,  //  001D  ADD	R5	R5	R7
      0x000C0705,  //  001E  ADD	R3	R3	K5
      0x70020017,  //  001F  JMP		#0038
      0x001C0704,  //  0020  ADD	R7	R3	K4
      0x141C0E04,  //  0021  LT	R7	R7	R4
      0x781E000D,  //  0022  JMPF	R7	#0031
      0x941C0003,  //  0023  GETIDX	R7	R0	R3
      0x00200704,  //  0024  ADD	R8	R3	K4
      0x94200008,  //  0025  GETIDX	R8	R0	R8
      0x54260007,  //  0026  LDINT	R9	8
      0x38201009,  //  0027  SHL	R8	R8	R9
      0x301C0E08,  //  0028  OR	R7	R7	R8
      0x5C180E00,  //  0029  MOVE	R6	R7
      0x5C1C0400,  //  002A  MOVE	R7	R2
      0x5C200C00,  //  002B  MOVE	R8	R6
      0x54260003,  //  002C  LDINT	R9	4
      0x7C1C0400,  //  002D  CALL	R7	2
      0x00140A07,  //  002E  ADD	R5	R5	R7
      0x000C0703,  //  002F  ADD	R3	R3	K3
      0x70020006,  //  0030  JMP		#0038
      0x94180003,  //  0031  GETIDX	R6	R0	R3
      0x5C1C0400,  //  0032  MOVE	R7	R2
      0x5C200C00,  //  0033  MOVE	R8	R6
      0x58240003,  //  0034  LDCONST	R9	K3
      0x7C1C0400,  //  0035  CALL	R7	2
      0x00140A07,  //  0036  ADD	R5	R5	R7
      0x000C0704,  //  0037  ADD	R3	R3	K4
      0x7001FFCD,  //  0038  JMP		#0007
      0x80040A00,  //  0039  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Base38
********************************************************************/
be_local_class(Matter_Base38,
    0,
    NULL,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(encode, -1), be_const_static_closure(class_Matter_Base38_encode_closure) },
    })),
    be_str_weak(Matter_Base38)
);
/********************************************************************/
/* End of solidification */
