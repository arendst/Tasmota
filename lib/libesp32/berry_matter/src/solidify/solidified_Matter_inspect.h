/* Solidification of Matter_inspect.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: sort
********************************************************************/
be_local_closure(matter_sort,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_const_int(1),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(sort),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x04080500,  //  0004  SUB	R2	R2	K0
      0x400A0002,  //  0005  CONNECT	R2	K0	R2
      0x7C040200,  //  0006  CALL	R1	1
      0xA8020010,  //  0007  EXBLK	0	#0019
      0x5C080200,  //  0008  MOVE	R2	R1
      0x7C080000,  //  0009  CALL	R2	0
      0x940C0002,  //  000A  GETIDX	R3	R0	R2
      0x5C100400,  //  000B  MOVE	R4	R2
      0x24140901,  //  000C  GT	R5	R4	K1
      0x78160008,  //  000D  JMPF	R5	#0017
      0x04140900,  //  000E  SUB	R5	R4	K0
      0x94140005,  //  000F  GETIDX	R5	R0	R5
      0x24140A03,  //  0010  GT	R5	R5	R3
      0x78160004,  //  0011  JMPF	R5	#0017
      0x04140900,  //  0012  SUB	R5	R4	K0
      0x94140005,  //  0013  GETIDX	R5	R0	R5
      0x98000805,  //  0014  SETIDX	R0	R4	R5
      0x04100900,  //  0015  SUB	R4	R4	K0
      0x7001FFF4,  //  0016  JMP		#000C
      0x98000803,  //  0017  SETIDX	R0	R4	R3
      0x7001FFEE,  //  0018  JMP		#0008
      0x58040002,  //  0019  LDCONST	R1	K2
      0xAC040200,  //  001A  CATCH	R1	1	0
      0xB0080000,  //  001B  RAISE	2	R0	R0
      0x80040000,  //  001C  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: inspect
********************************************************************/
be_local_closure(matter_inspect,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_nested_str_weak(members),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_nested_str_weak(function),
    /* K5   */  be_nested_str_weak(push),
    /* K6   */  be_nested_str_weak(stop_iteration),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(sort),
    /* K9   */  be_nested_str_weak(format),
    /* K10  */  be_nested_str_weak(_X27_X25s_X27_X3A_X20_X25s),
    /* K11  */  be_nested_str_weak(_X7B),
    /* K12  */  be_nested_str_weak(concat),
    /* K13  */  be_nested_str_weak(_X2C_X20),
    /* K14  */  be_nested_str_weak(_X7D),
    /* K15  */  be_nested_str_weak(Exception_X3A),
    /* K16  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(inspect),
    &be_const_str_solidified,
    ( &(const binstruction[85]) {  /* code */
      0xA8020044,  //  0000  EXBLK	0	#0046
      0xA4060000,  //  0001  IMPORT	R1	K0
      0xA40A0200,  //  0002  IMPORT	R2	K1
      0x600C0012,  //  0003  GETGBL	R3	G18
      0x7C0C0000,  //  0004  CALL	R3	0
      0x60100010,  //  0005  GETGBL	R4	G16
      0x8C140502,  //  0006  GETMET	R5	R2	K2
      0x5C1C0000,  //  0007  MOVE	R7	R0
      0x7C140400,  //  0008  CALL	R5	2
      0x7C100200,  //  0009  CALL	R4	1
      0xA802000E,  //  000A  EXBLK	0	#001A
      0x5C140800,  //  000B  MOVE	R5	R4
      0x7C140000,  //  000C  CALL	R5	0
      0x8C180503,  //  000D  GETMET	R6	R2	K3
      0x5C200000,  //  000E  MOVE	R8	R0
      0x5C240A00,  //  000F  MOVE	R9	R5
      0x7C180600,  //  0010  CALL	R6	3
      0x601C0004,  //  0011  GETGBL	R7	G4
      0x5C200C00,  //  0012  MOVE	R8	R6
      0x7C1C0200,  //  0013  CALL	R7	1
      0x201C0F04,  //  0014  NE	R7	R7	K4
      0x781E0002,  //  0015  JMPF	R7	#0019
      0x8C1C0705,  //  0016  GETMET	R7	R3	K5
      0x5C240A00,  //  0017  MOVE	R9	R5
      0x7C1C0400,  //  0018  CALL	R7	2
      0x7001FFF0,  //  0019  JMP		#000B
      0x58100006,  //  001A  LDCONST	R4	K6
      0xAC100200,  //  001B  CATCH	R4	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0xB8120E00,  //  001D  GETNGBL	R4	K7
      0x8C100908,  //  001E  GETMET	R4	R4	K8
      0x5C180600,  //  001F  MOVE	R6	R3
      0x7C100400,  //  0020  CALL	R4	2
      0x5C0C0800,  //  0021  MOVE	R3	R4
      0x60100012,  //  0022  GETGBL	R4	G18
      0x7C100000,  //  0023  CALL	R4	0
      0x60140010,  //  0024  GETGBL	R5	G16
      0x5C180600,  //  0025  MOVE	R6	R3
      0x7C140200,  //  0026  CALL	R5	1
      0xA8020011,  //  0027  EXBLK	0	#003A
      0x5C180A00,  //  0028  MOVE	R6	R5
      0x7C180000,  //  0029  CALL	R6	0
      0x8C1C0503,  //  002A  GETMET	R7	R2	K3
      0x5C240000,  //  002B  MOVE	R9	R0
      0x5C280C00,  //  002C  MOVE	R10	R6
      0x7C1C0600,  //  002D  CALL	R7	3
      0x8C200905,  //  002E  GETMET	R8	R4	K5
      0x8C280309,  //  002F  GETMET	R10	R1	K9
      0x5830000A,  //  0030  LDCONST	R12	K10
      0x60340008,  //  0031  GETGBL	R13	G8
      0x5C380C00,  //  0032  MOVE	R14	R6
      0x7C340200,  //  0033  CALL	R13	1
      0x60380008,  //  0034  GETGBL	R14	G8
      0x5C3C0E00,  //  0035  MOVE	R15	R7
      0x7C380200,  //  0036  CALL	R14	1
      0x7C280800,  //  0037  CALL	R10	4
      0x7C200400,  //  0038  CALL	R8	2
      0x7001FFED,  //  0039  JMP		#0028
      0x58140006,  //  003A  LDCONST	R5	K6
      0xAC140200,  //  003B  CATCH	R5	1	0
      0xB0080000,  //  003C  RAISE	2	R0	R0
      0x8C14090C,  //  003D  GETMET	R5	R4	K12
      0x581C000D,  //  003E  LDCONST	R7	K13
      0x7C140400,  //  003F  CALL	R5	2
      0x00161605,  //  0040  ADD	R5	K11	R5
      0x00140B0E,  //  0041  ADD	R5	R5	K14
      0xA8040001,  //  0042  EXBLK	1	1
      0x80040A00,  //  0043  RET	1	R5
      0xA8040001,  //  0044  EXBLK	1	1
      0x7002000D,  //  0045  JMP		#0054
      0xAC040002,  //  0046  CATCH	R1	0	2
      0x7002000A,  //  0047  JMP		#0053
      0x600C0008,  //  0048  GETGBL	R3	G8
      0x5C100200,  //  0049  MOVE	R4	R1
      0x7C0C0200,  //  004A  CALL	R3	1
      0x000E1E03,  //  004B  ADD	R3	K15	R3
      0x000C0710,  //  004C  ADD	R3	R3	K16
      0x60100008,  //  004D  GETGBL	R4	G8
      0x5C140400,  //  004E  MOVE	R5	R2
      0x7C100200,  //  004F  CALL	R4	1
      0x000C0604,  //  0050  ADD	R3	R3	R4
      0x80040600,  //  0051  RET	1	R3
      0x70020000,  //  0052  JMP		#0054
      0xB0080000,  //  0053  RAISE	2	R0	R0
      0x80000000,  //  0054  RET	0
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
