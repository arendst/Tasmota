/* Solidification of Matter_0_Inspect.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: sort
********************************************************************/
be_local_closure(module_matter_sort,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL, 
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
be_local_closure(module_matter_jitter,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL, 
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
be_local_closure(module_matter_inspect,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL, 
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(members),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(function),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(push),
    /* K6   */  be_nested_str_weak(stop_iteration),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(sort),
    /* K9   */  be_nested_str_weak(_X27_X25s_X27_X3A_X20_X25s),
    /* K10  */  be_nested_str_weak(_X7B),
    /* K11  */  be_nested_str_weak(concat),
    /* K12  */  be_nested_str_weak(_X2C_X20),
    /* K13  */  be_nested_str_weak(_X7D),
    /* K14  */  be_nested_str_weak(Exception_X3A),
    /* K15  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(inspect),
    &be_const_str_solidified,
    ( &(const binstruction[99]) {  /* code */
      0xA8020052,  //  0000  EXBLK	0	#0054
      0xA4060000,  //  0001  IMPORT	R1	K0
      0x60080012,  //  0002  GETGBL	R2	G18
      0x7C080000,  //  0003  CALL	R2	0
      0x5C0C0000,  //  0004  MOVE	R3	R0
      0x4C100000,  //  0005  LDNIL	R4
      0x20100604,  //  0006  NE	R4	R3	R4
      0x78120022,  //  0007  JMPF	R4	#002B
      0x60100010,  //  0008  GETGBL	R4	G16
      0x8C140301,  //  0009  GETMET	R5	R1	K1
      0x5C1C0000,  //  000A  MOVE	R7	R0
      0x7C140400,  //  000B  CALL	R5	2
      0x7C100200,  //  000C  CALL	R4	1
      0xA8020014,  //  000D  EXBLK	0	#0023
      0x5C140800,  //  000E  MOVE	R5	R4
      0x7C140000,  //  000F  CALL	R5	0
      0x8C180302,  //  0010  GETMET	R6	R1	K2
      0x5C200000,  //  0011  MOVE	R8	R0
      0x5C240A00,  //  0012  MOVE	R9	R5
      0x7C180600,  //  0013  CALL	R6	3
      0x601C0004,  //  0014  GETGBL	R7	G4
      0x5C200C00,  //  0015  MOVE	R8	R6
      0x7C1C0200,  //  0016  CALL	R7	1
      0x201C0F03,  //  0017  NE	R7	R7	K3
      0x781E0008,  //  0018  JMPF	R7	#0022
      0x8C1C0504,  //  0019  GETMET	R7	R2	K4
      0x5C240A00,  //  001A  MOVE	R9	R5
      0x7C1C0400,  //  001B  CALL	R7	2
      0x4C200000,  //  001C  LDNIL	R8
      0x1C1C0E08,  //  001D  EQ	R7	R7	R8
      0x781E0002,  //  001E  JMPF	R7	#0022
      0x8C1C0505,  //  001F  GETMET	R7	R2	K5
      0x5C240A00,  //  0020  MOVE	R9	R5
      0x7C1C0400,  //  0021  CALL	R7	2
      0x7001FFEA,  //  0022  JMP		#000E
      0x58100006,  //  0023  LDCONST	R4	K6
      0xAC100200,  //  0024  CATCH	R4	1	0
      0xB0080000,  //  0025  RAISE	2	R0	R0
      0x60100003,  //  0026  GETGBL	R4	G3
      0x5C140600,  //  0027  MOVE	R5	R3
      0x7C100200,  //  0028  CALL	R4	1
      0x5C0C0800,  //  0029  MOVE	R3	R4
      0x7001FFD9,  //  002A  JMP		#0005
      0xB8120E00,  //  002B  GETNGBL	R4	K7
      0x8C100908,  //  002C  GETMET	R4	R4	K8
      0x5C180400,  //  002D  MOVE	R6	R2
      0x7C100400,  //  002E  CALL	R4	2
      0x5C080800,  //  002F  MOVE	R2	R4
      0x60100012,  //  0030  GETGBL	R4	G18
      0x7C100000,  //  0031  CALL	R4	0
      0x60140010,  //  0032  GETGBL	R5	G16
      0x5C180400,  //  0033  MOVE	R6	R2
      0x7C140200,  //  0034  CALL	R5	1
      0xA8020011,  //  0035  EXBLK	0	#0048
      0x5C180A00,  //  0036  MOVE	R6	R5
      0x7C180000,  //  0037  CALL	R6	0
      0x8C1C0302,  //  0038  GETMET	R7	R1	K2
      0x5C240000,  //  0039  MOVE	R9	R0
      0x5C280C00,  //  003A  MOVE	R10	R6
      0x7C1C0600,  //  003B  CALL	R7	3
      0x8C200905,  //  003C  GETMET	R8	R4	K5
      0x60280018,  //  003D  GETGBL	R10	G24
      0x582C0009,  //  003E  LDCONST	R11	K9
      0x60300008,  //  003F  GETGBL	R12	G8
      0x5C340C00,  //  0040  MOVE	R13	R6
      0x7C300200,  //  0041  CALL	R12	1
      0x60340008,  //  0042  GETGBL	R13	G8
      0x5C380E00,  //  0043  MOVE	R14	R7
      0x7C340200,  //  0044  CALL	R13	1
      0x7C280600,  //  0045  CALL	R10	3
      0x7C200400,  //  0046  CALL	R8	2
      0x7001FFED,  //  0047  JMP		#0036
      0x58140006,  //  0048  LDCONST	R5	K6
      0xAC140200,  //  0049  CATCH	R5	1	0
      0xB0080000,  //  004A  RAISE	2	R0	R0
      0x8C14090B,  //  004B  GETMET	R5	R4	K11
      0x581C000C,  //  004C  LDCONST	R7	K12
      0x7C140400,  //  004D  CALL	R5	2
      0x00161405,  //  004E  ADD	R5	K10	R5
      0x00140B0D,  //  004F  ADD	R5	R5	K13
      0xA8040001,  //  0050  EXBLK	1	1
      0x80040A00,  //  0051  RET	1	R5
      0xA8040001,  //  0052  EXBLK	1	1
      0x7002000D,  //  0053  JMP		#0062
      0xAC040002,  //  0054  CATCH	R1	0	2
      0x7002000A,  //  0055  JMP		#0061
      0x600C0008,  //  0056  GETGBL	R3	G8
      0x5C100200,  //  0057  MOVE	R4	R1
      0x7C0C0200,  //  0058  CALL	R3	1
      0x000E1C03,  //  0059  ADD	R3	K14	R3
      0x000C070F,  //  005A  ADD	R3	R3	K15
      0x60100008,  //  005B  GETGBL	R4	G8
      0x5C140400,  //  005C  MOVE	R5	R2
      0x7C100200,  //  005D  CALL	R4	1
      0x000C0604,  //  005E  ADD	R3	R3	R4
      0x80040600,  //  005F  RET	1	R3
      0x70020000,  //  0060  JMP		#0062
      0xB0080000,  //  0061  RAISE	2	R0	R0
      0x80000000,  //  0062  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: consolidate_clusters
********************************************************************/
be_local_closure(module_matter_consolidate_clusters,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(CLUSTERS),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(copy),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(push),
    }),
    be_str_weak(consolidate_clusters),
    &be_const_str_solidified,
    ( &(const binstruction[100]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x780A0004,  //  0005  JMPF	R2	#000B
      0x60080003,  //  0006  GETGBL	R2	G3
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x7C080200,  //  0008  CALL	R2	1
      0x88080500,  //  0009  GETMBR	R2	R2	K0
      0x70020001,  //  000A  JMP		#000D
      0x60080013,  //  000B  GETGBL	R2	G19
      0x7C080000,  //  000C  CALL	R2	0
      0x600C0013,  //  000D  GETGBL	R3	G19
      0x7C0C0000,  //  000E  CALL	R3	0
      0x60100010,  //  000F  GETGBL	R4	G16
      0x8C140501,  //  0010  GETMET	R5	R2	K1
      0x7C140200,  //  0011  CALL	R5	1
      0x7C100200,  //  0012  CALL	R4	1
      0xA8020006,  //  0013  EXBLK	0	#001B
      0x5C140800,  //  0014  MOVE	R5	R4
      0x7C140000,  //  0015  CALL	R5	0
      0x94180405,  //  0016  GETIDX	R6	R2	R5
      0x8C180D02,  //  0017  GETMET	R6	R6	K2
      0x7C180200,  //  0018  CALL	R6	1
      0x980C0A06,  //  0019  SETIDX	R3	R5	R6
      0x7001FFF8,  //  001A  JMP		#0014
      0x58100003,  //  001B  LDCONST	R4	K3
      0xAC100200,  //  001C  CATCH	R4	1	0
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x60100010,  //  001E  GETGBL	R4	G16
      0x8C140301,  //  001F  GETMET	R5	R1	K1
      0x7C140200,  //  0020  CALL	R5	1
      0x7C100200,  //  0021  CALL	R4	1
      0xA802000E,  //  0022  EXBLK	0	#0032
      0x5C140800,  //  0023  MOVE	R5	R4
      0x7C140000,  //  0024  CALL	R5	0
      0x8C180704,  //  0025  GETMET	R6	R3	K4
      0x5C200A00,  //  0026  MOVE	R8	R5
      0x7C180400,  //  0027  CALL	R6	2
      0x4C1C0000,  //  0028  LDNIL	R7
      0x1C1C0C07,  //  0029  EQ	R7	R6	R7
      0x781E0002,  //  002A  JMPF	R7	#002E
      0x601C0012,  //  002B  GETGBL	R7	G18
      0x7C1C0000,  //  002C  CALL	R7	0
      0x5C180E00,  //  002D  MOVE	R6	R7
      0x941C0205,  //  002E  GETIDX	R7	R1	R5
      0x001C0C07,  //  002F  ADD	R7	R6	R7
      0x980C0A07,  //  0030  SETIDX	R3	R5	R7
      0x7001FFF0,  //  0031  JMP		#0023
      0x58100003,  //  0032  LDCONST	R4	K3
      0xAC100200,  //  0033  CATCH	R4	1	0
      0xB0080000,  //  0034  RAISE	2	R0	R0
      0x60100012,  //  0035  GETGBL	R4	G18
      0x7C100000,  //  0036  CALL	R4	0
      0x5416FFF7,  //  0037  LDINT	R5	65528
      0x40140805,  //  0038  CONNECT	R5	R4	R5
      0x5416FFF8,  //  0039  LDINT	R5	65529
      0x40140805,  //  003A  CONNECT	R5	R4	R5
      0x5416FFF9,  //  003B  LDINT	R5	65530
      0x40140805,  //  003C  CONNECT	R5	R4	R5
      0x5416FFFA,  //  003D  LDINT	R5	65531
      0x40140805,  //  003E  CONNECT	R5	R4	R5
      0x5416FFFB,  //  003F  LDINT	R5	65532
      0x40140805,  //  0040  CONNECT	R5	R4	R5
      0x5416FFFC,  //  0041  LDINT	R5	65533
      0x40140805,  //  0042  CONNECT	R5	R4	R5
      0x60140010,  //  0043  GETGBL	R5	G16
      0x8C180301,  //  0044  GETMET	R6	R1	K1
      0x7C180200,  //  0045  CALL	R6	1
      0x7C140200,  //  0046  CALL	R5	1
      0xA8020017,  //  0047  EXBLK	0	#0060
      0x5C180A00,  //  0048  MOVE	R6	R5
      0x7C180000,  //  0049  CALL	R6	0
      0x601C0010,  //  004A  GETGBL	R7	G16
      0x5C200800,  //  004B  MOVE	R8	R4
      0x7C1C0200,  //  004C  CALL	R7	1
      0xA802000D,  //  004D  EXBLK	0	#005C
      0x5C200E00,  //  004E  MOVE	R8	R7
      0x7C200000,  //  004F  CALL	R8	0
      0x94240606,  //  0050  GETIDX	R9	R3	R6
      0x8C241304,  //  0051  GETMET	R9	R9	K4
      0x5C2C1000,  //  0052  MOVE	R11	R8
      0x7C240400,  //  0053  CALL	R9	2
      0x4C280000,  //  0054  LDNIL	R10
      0x1C24120A,  //  0055  EQ	R9	R9	R10
      0x78260003,  //  0056  JMPF	R9	#005B
      0x94240606,  //  0057  GETIDX	R9	R3	R6
      0x8C241305,  //  0058  GETMET	R9	R9	K5
      0x5C2C1000,  //  0059  MOVE	R11	R8
      0x7C240400,  //  005A  CALL	R9	2
      0x7001FFF1,  //  005B  JMP		#004E
      0x581C0003,  //  005C  LDCONST	R7	K3
      0xAC1C0200,  //  005D  CATCH	R7	1	0
      0xB0080000,  //  005E  RAISE	2	R0	R0
      0x7001FFE7,  //  005F  JMP		#0048
      0x58140003,  //  0060  LDCONST	R5	K3
      0xAC140200,  //  0061  CATCH	R5	1	0
      0xB0080000,  //  0062  RAISE	2	R0	R0
      0x80040600,  //  0063  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: UC_LIST
********************************************************************/
be_local_closure(module_matter_UC_LIST,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    1,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL, 
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
