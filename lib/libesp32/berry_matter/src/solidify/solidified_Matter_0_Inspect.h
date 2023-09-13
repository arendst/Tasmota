/* Solidification of Matter_0_Inspect.h */
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
** Solidified function: jitter
********************************************************************/
be_local_closure(matter_jitter,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(random),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_const_int(0),
    /* K4   */  be_const_int(2147483647),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(millis),
    }),
    be_str_weak(jitter),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x54120003,  //  0002  LDINT	R4	4
      0x7C080400,  //  0003  CALL	R2	2
      0x8C080502,  //  0004  GETMET	R2	R2	K2
      0x58100003,  //  0005  LDCONST	R4	K3
      0x54160003,  //  0006  LDINT	R5	4
      0x7C080600,  //  0007  CALL	R2	3
      0x2C080504,  //  0008  AND	R2	R2	K4
      0xB80E0A00,  //  0009  GETNGBL	R3	K5
      0x8C0C0706,  //  000A  GETMET	R3	R3	K6
      0x10140400,  //  000B  MOD	R5	R2	R0
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80040600,  //  000D  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: inspect
********************************************************************/
be_local_closure(matter_inspect,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(members),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(function),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(stop_iteration),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(sort),
    /* K8   */  be_nested_str_weak(_X27_X25s_X27_X3A_X20_X25s),
    /* K9   */  be_nested_str_weak(_X7B),
    /* K10  */  be_nested_str_weak(concat),
    /* K11  */  be_nested_str_weak(_X2C_X20),
    /* K12  */  be_nested_str_weak(_X7D),
    /* K13  */  be_nested_str_weak(Exception_X3A),
    /* K14  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(inspect),
    &be_const_str_solidified,
    ( &(const binstruction[84]) {  /* code */
      0xA8020043,  //  0000  EXBLK	0	#0045
      0xA4060000,  //  0001  IMPORT	R1	K0
      0x60080012,  //  0002  GETGBL	R2	G18
      0x7C080000,  //  0003  CALL	R2	0
      0x600C0010,  //  0004  GETGBL	R3	G16
      0x8C100301,  //  0005  GETMET	R4	R1	K1
      0x5C180000,  //  0006  MOVE	R6	R0
      0x7C100400,  //  0007  CALL	R4	2
      0x7C0C0200,  //  0008  CALL	R3	1
      0xA802000E,  //  0009  EXBLK	0	#0019
      0x5C100600,  //  000A  MOVE	R4	R3
      0x7C100000,  //  000B  CALL	R4	0
      0x8C140302,  //  000C  GETMET	R5	R1	K2
      0x5C1C0000,  //  000D  MOVE	R7	R0
      0x5C200800,  //  000E  MOVE	R8	R4
      0x7C140600,  //  000F  CALL	R5	3
      0x60180004,  //  0010  GETGBL	R6	G4
      0x5C1C0A00,  //  0011  MOVE	R7	R5
      0x7C180200,  //  0012  CALL	R6	1
      0x20180D03,  //  0013  NE	R6	R6	K3
      0x781A0002,  //  0014  JMPF	R6	#0018
      0x8C180504,  //  0015  GETMET	R6	R2	K4
      0x5C200800,  //  0016  MOVE	R8	R4
      0x7C180400,  //  0017  CALL	R6	2
      0x7001FFF0,  //  0018  JMP		#000A
      0x580C0005,  //  0019  LDCONST	R3	K5
      0xAC0C0200,  //  001A  CATCH	R3	1	0
      0xB0080000,  //  001B  RAISE	2	R0	R0
      0xB80E0C00,  //  001C  GETNGBL	R3	K6
      0x8C0C0707,  //  001D  GETMET	R3	R3	K7
      0x5C140400,  //  001E  MOVE	R5	R2
      0x7C0C0400,  //  001F  CALL	R3	2
      0x5C080600,  //  0020  MOVE	R2	R3
      0x600C0012,  //  0021  GETGBL	R3	G18
      0x7C0C0000,  //  0022  CALL	R3	0
      0x60100010,  //  0023  GETGBL	R4	G16
      0x5C140400,  //  0024  MOVE	R5	R2
      0x7C100200,  //  0025  CALL	R4	1
      0xA8020011,  //  0026  EXBLK	0	#0039
      0x5C140800,  //  0027  MOVE	R5	R4
      0x7C140000,  //  0028  CALL	R5	0
      0x8C180302,  //  0029  GETMET	R6	R1	K2
      0x5C200000,  //  002A  MOVE	R8	R0
      0x5C240A00,  //  002B  MOVE	R9	R5
      0x7C180600,  //  002C  CALL	R6	3
      0x8C1C0704,  //  002D  GETMET	R7	R3	K4
      0x60240018,  //  002E  GETGBL	R9	G24
      0x58280008,  //  002F  LDCONST	R10	K8
      0x602C0008,  //  0030  GETGBL	R11	G8
      0x5C300A00,  //  0031  MOVE	R12	R5
      0x7C2C0200,  //  0032  CALL	R11	1
      0x60300008,  //  0033  GETGBL	R12	G8
      0x5C340C00,  //  0034  MOVE	R13	R6
      0x7C300200,  //  0035  CALL	R12	1
      0x7C240600,  //  0036  CALL	R9	3
      0x7C1C0400,  //  0037  CALL	R7	2
      0x7001FFED,  //  0038  JMP		#0027
      0x58100005,  //  0039  LDCONST	R4	K5
      0xAC100200,  //  003A  CATCH	R4	1	0
      0xB0080000,  //  003B  RAISE	2	R0	R0
      0x8C10070A,  //  003C  GETMET	R4	R3	K10
      0x5818000B,  //  003D  LDCONST	R6	K11
      0x7C100400,  //  003E  CALL	R4	2
      0x00121204,  //  003F  ADD	R4	K9	R4
      0x0010090C,  //  0040  ADD	R4	R4	K12
      0xA8040001,  //  0041  EXBLK	1	1
      0x80040800,  //  0042  RET	1	R4
      0xA8040001,  //  0043  EXBLK	1	1
      0x7002000D,  //  0044  JMP		#0053
      0xAC040002,  //  0045  CATCH	R1	0	2
      0x7002000A,  //  0046  JMP		#0052
      0x600C0008,  //  0047  GETGBL	R3	G8
      0x5C100200,  //  0048  MOVE	R4	R1
      0x7C0C0200,  //  0049  CALL	R3	1
      0x000E1A03,  //  004A  ADD	R3	K13	R3
      0x000C070E,  //  004B  ADD	R3	R3	K14
      0x60100008,  //  004C  GETGBL	R4	G8
      0x5C140400,  //  004D  MOVE	R5	R2
      0x7C100200,  //  004E  CALL	R4	1
      0x000C0604,  //  004F  ADD	R3	R3	R4
      0x80040600,  //  0050  RET	1	R3
      0x70020000,  //  0051  JMP		#0053
      0xB0080000,  //  0052  RAISE	2	R0	R0
      0x80000000,  //  0053  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: consolidate_clusters
********************************************************************/
be_local_closure(matter_consolidate_clusters,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(CLUSTERS),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(copy),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(find),
    }),
    be_str_weak(consolidate_clusters),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x88080500,  //  0003  GETMBR	R2	R2	K0
      0x600C0013,  //  0004  GETGBL	R3	G19
      0x7C0C0000,  //  0005  CALL	R3	0
      0x60100010,  //  0006  GETGBL	R4	G16
      0x8C140501,  //  0007  GETMET	R5	R2	K1
      0x7C140200,  //  0008  CALL	R5	1
      0x7C100200,  //  0009  CALL	R4	1
      0xA8020006,  //  000A  EXBLK	0	#0012
      0x5C140800,  //  000B  MOVE	R5	R4
      0x7C140000,  //  000C  CALL	R5	0
      0x94180405,  //  000D  GETIDX	R6	R2	R5
      0x8C180D02,  //  000E  GETMET	R6	R6	K2
      0x7C180200,  //  000F  CALL	R6	1
      0x980C0A06,  //  0010  SETIDX	R3	R5	R6
      0x7001FFF8,  //  0011  JMP		#000B
      0x58100003,  //  0012  LDCONST	R4	K3
      0xAC100200,  //  0013  CATCH	R4	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x60100010,  //  0015  GETGBL	R4	G16
      0x8C140301,  //  0016  GETMET	R5	R1	K1
      0x7C140200,  //  0017  CALL	R5	1
      0x7C100200,  //  0018  CALL	R4	1
      0xA802000E,  //  0019  EXBLK	0	#0029
      0x5C140800,  //  001A  MOVE	R5	R4
      0x7C140000,  //  001B  CALL	R5	0
      0x8C180704,  //  001C  GETMET	R6	R3	K4
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x4C1C0000,  //  001F  LDNIL	R7
      0x1C1C0C07,  //  0020  EQ	R7	R6	R7
      0x781E0002,  //  0021  JMPF	R7	#0025
      0x601C0012,  //  0022  GETGBL	R7	G18
      0x7C1C0000,  //  0023  CALL	R7	0
      0x5C180E00,  //  0024  MOVE	R6	R7
      0x941C0205,  //  0025  GETIDX	R7	R1	R5
      0x001C0C07,  //  0026  ADD	R7	R6	R7
      0x980C0A07,  //  0027  SETIDX	R3	R5	R7
      0x7001FFF0,  //  0028  JMP		#001A
      0x58100003,  //  0029  LDCONST	R4	K3
      0xAC100200,  //  002A  CATCH	R4	1	0
      0xB0080000,  //  002B  RAISE	2	R0	R0
      0x80040600,  //  002C  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: UC_LIST
********************************************************************/
be_local_closure(matter_UC_LIST,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    1,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(UPDATE_COMMANDS),
    }),
    be_str_weak(UC_LIST),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x88080500,  //  0003  GETMBR	R2	R2	K0
      0x000C0401,  //  0004  ADD	R3	R2	R1
      0x80040600,  //  0005  RET	1	R3
    })
  )
);
/*******************************************************************/

/********************************************************************/
/* End of solidification */
