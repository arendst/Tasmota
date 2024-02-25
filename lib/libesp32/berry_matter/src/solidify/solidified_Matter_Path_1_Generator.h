/* Solidification of Matter_Path_1_Generator.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_PathGenerator;

/********************************************************************
** Solidified function: next
********************************************************************/
be_local_closure(Matter_PathGenerator_next,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(path_in),
    /* K1   */  be_nested_str_weak(pi),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(_next_endpoint),
    /* K4   */  be_nested_str_weak(endpoint_found),
    /* K5   */  be_nested_str_weak(attribute),
    /* K6   */  be_nested_str_weak(_next_cluster),
    /* K7   */  be_nested_str_weak(cluster_found),
    /* K8   */  be_nested_str_weak(_next_attribute),
    /* K9   */  be_nested_str_weak(attribute_found),
    /* K10  */  be_nested_str_weak(path_concrete),
    /* K11  */  be_nested_str_weak(reset),
    /* K12  */  be_nested_str_weak(endpoint),
    /* K13  */  be_nested_str_weak(get_endpoint),
    }),
    be_str_weak(next),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x4C040000,  //  0004  LDNIL	R1
      0x80040200,  //  0005  RET	1	R1
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x20040202,  //  0008  NE	R1	R1	R2
      0x7806002F,  //  0009  JMPF	R1	#003A
      0x88040101,  //  000A  GETMBR	R1	R0	K1
      0x4C080000,  //  000B  LDNIL	R2
      0x1C040202,  //  000C  EQ	R1	R1	R2
      0x74060003,  //  000D  JMPT	R1	#0012
      0x88040102,  //  000E  GETMBR	R1	R0	K2
      0x50080000,  //  000F  LDBOOL	R2	0	0
      0x1C040202,  //  0010  EQ	R1	R1	R2
      0x78060002,  //  0011  JMPF	R1	#0015
      0x8C040103,  //  0012  GETMET	R1	R0	K3
      0x7C040200,  //  0013  CALL	R1	1
      0x7001FFF0,  //  0014  JMP		#0006
      0x50040200,  //  0015  LDBOOL	R1	1	0
      0x90020801,  //  0016  SETMBR	R0	K4	R1
      0x88040102,  //  0017  GETMBR	R1	R0	K2
      0x4C080000,  //  0018  LDNIL	R2
      0x1C040202,  //  0019  EQ	R1	R1	R2
      0x74060003,  //  001A  JMPT	R1	#001F
      0x88040105,  //  001B  GETMBR	R1	R0	K5
      0x50080000,  //  001C  LDBOOL	R2	0	0
      0x1C040202,  //  001D  EQ	R1	R1	R2
      0x78060002,  //  001E  JMPF	R1	#0022
      0x8C040106,  //  001F  GETMET	R1	R0	K6
      0x7C040200,  //  0020  CALL	R1	1
      0x7001FFE3,  //  0021  JMP		#0006
      0x50040200,  //  0022  LDBOOL	R1	1	0
      0x90020E01,  //  0023  SETMBR	R0	K7	R1
      0x8C040108,  //  0024  GETMET	R1	R0	K8
      0x7C040200,  //  0025  CALL	R1	1
      0x88040105,  //  0026  GETMBR	R1	R0	K5
      0x50080000,  //  0027  LDBOOL	R2	0	0
      0x1C040202,  //  0028  EQ	R1	R1	R2
      0x78060000,  //  0029  JMPF	R1	#002B
      0x7001FFDA,  //  002A  JMP		#0006
      0x50040200,  //  002B  LDBOOL	R1	1	0
      0x90021201,  //  002C  SETMBR	R0	K9	R1
      0x8804010A,  //  002D  GETMBR	R1	R0	K10
      0x8C08030B,  //  002E  GETMET	R2	R1	K11
      0x7C080200,  //  002F  CALL	R2	1
      0x88080101,  //  0030  GETMBR	R2	R0	K1
      0x8C08050D,  //  0031  GETMET	R2	R2	K13
      0x7C080200,  //  0032  CALL	R2	1
      0x90061802,  //  0033  SETMBR	R1	K12	R2
      0x88080102,  //  0034  GETMBR	R2	R0	K2
      0x90060402,  //  0035  SETMBR	R1	K2	R2
      0x88080105,  //  0036  GETMBR	R2	R0	K5
      0x90060A02,  //  0037  SETMBR	R1	K5	R2
      0x80040200,  //  0038  RET	1	R1
      0x7001FFCB,  //  0039  JMP		#0006
      0x8C04010B,  //  003A  GETMET	R1	R0	K11
      0x7C040200,  //  003B  CALL	R1	1
      0x4C040000,  //  003C  LDNIL	R1
      0x80040200,  //  003D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _next_cluster
********************************************************************/
be_local_closure(Matter_PathGenerator__next_cluster,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(cluster),
    /* K1   */  be_nested_str_weak(clusters),
    /* K2   */  be_nested_str_weak(path_in),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_const_int(1),
    }),
    be_str_weak(_next_cluster),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x50040000,  //  0004  LDBOOL	R1	0	0
      0x80040200,  //  0005  RET	1	R1
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x88080102,  //  0007  GETMBR	R2	R0	K2
      0x88080500,  //  0008  GETMBR	R2	R2	K0
      0x4C0C0000,  //  0009  LDNIL	R3
      0x90020603,  //  000A  SETMBR	R0	K3	R3
      0x540DFFFE,  //  000B  LDINT	R3	-1
      0x88100100,  //  000C  GETMBR	R4	R0	K0
      0x4C140000,  //  000D  LDNIL	R5
      0x20100805,  //  000E  NE	R4	R4	R5
      0x78120003,  //  000F  JMPF	R4	#0014
      0x8C100304,  //  0010  GETMET	R4	R1	K4
      0x88180100,  //  0011  GETMBR	R6	R0	K0
      0x7C100400,  //  0012  CALL	R4	2
      0x5C0C0800,  //  0013  MOVE	R3	R4
      0x4C100000,  //  0014  LDNIL	R4
      0x20100604,  //  0015  NE	R4	R3	R4
      0x78120011,  //  0016  JMPF	R4	#0029
      0x00100705,  //  0017  ADD	R4	R3	K5
      0x6014000C,  //  0018  GETGBL	R5	G12
      0x5C180200,  //  0019  MOVE	R6	R1
      0x7C140200,  //  001A  CALL	R5	1
      0x14100805,  //  001B  LT	R4	R4	R5
      0x7812000B,  //  001C  JMPF	R4	#0029
      0x000C0705,  //  001D  ADD	R3	R3	K5
      0x94100203,  //  001E  GETIDX	R4	R1	R3
      0x90020004,  //  001F  SETMBR	R0	K0	R4
      0x4C100000,  //  0020  LDNIL	R4
      0x1C100404,  //  0021  EQ	R4	R2	R4
      0x74120002,  //  0022  JMPT	R4	#0026
      0x88100100,  //  0023  GETMBR	R4	R0	K0
      0x1C100404,  //  0024  EQ	R4	R2	R4
      0x78120001,  //  0025  JMPF	R4	#0028
      0x88100100,  //  0026  GETMBR	R4	R0	K0
      0x80040800,  //  0027  RET	1	R4
      0x7001FFED,  //  0028  JMP		#0017
      0x50100000,  //  0029  LDBOOL	R4	0	0
      0x90020004,  //  002A  SETMBR	R0	K0	R4
      0x50100000,  //  002B  LDBOOL	R4	0	0
      0x80040800,  //  002C  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(Matter_PathGenerator_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(path_concrete),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Path),
    /* K3   */  be_nested_str_weak(reset),
    /* K4   */  be_nested_str_weak(path_in),
    /* K5   */  be_nested_str_weak(session),
    /* K6   */  be_nested_str_weak(endpoint_found),
    /* K7   */  be_nested_str_weak(cluster_found),
    /* K8   */  be_nested_str_weak(attribute_found),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB80E0200,  //  0000  GETNGBL	R3	K1
      0x8C0C0702,  //  0001  GETMET	R3	R3	K2
      0x7C0C0200,  //  0002  CALL	R3	1
      0x90020003,  //  0003  SETMBR	R0	K0	R3
      0x8C0C0103,  //  0004  GETMET	R3	R0	K3
      0x7C0C0200,  //  0005  CALL	R3	1
      0x90020801,  //  0006  SETMBR	R0	K4	R1
      0x90020A02,  //  0007  SETMBR	R0	K5	R2
      0x500C0000,  //  0008  LDBOOL	R3	0	0
      0x90020C03,  //  0009  SETMBR	R0	K6	R3
      0x500C0000,  //  000A  LDBOOL	R3	0	0
      0x90020E03,  //  000B  SETMBR	R0	K7	R3
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0x90021003,  //  000D  SETMBR	R0	K8	R3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pi
********************************************************************/
be_local_closure(Matter_PathGenerator_get_pi,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(pi),
    }),
    be_str_weak(get_pi),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_PathGenerator_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(Matter_PathGenerator_reset,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(path_in),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(path_concrete),
    /* K3   */  be_nested_str_weak(reset),
    /* K4   */  be_nested_str_weak(pi),
    /* K5   */  be_nested_str_weak(cluster),
    /* K6   */  be_nested_str_weak(attribute),
    /* K7   */  be_nested_str_weak(clusters),
    }),
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x90020201,  //  0002  SETMBR	R0	K1	R1
      0x88080102,  //  0003  GETMBR	R2	R0	K2
      0x8C080503,  //  0004  GETMET	R2	R2	K3
      0x7C080200,  //  0005  CALL	R2	1
      0x90020801,  //  0006  SETMBR	R0	K4	R1
      0x90020A01,  //  0007  SETMBR	R0	K5	R1
      0x90020C01,  //  0008  SETMBR	R0	K6	R1
      0x90020E01,  //  0009  SETMBR	R0	K7	R1
      0x90020E01,  //  000A  SETMBR	R0	K7	R1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _next_endpoint
********************************************************************/
be_local_closure(Matter_PathGenerator__next_endpoint,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(pi),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(plugins),
    /* K3   */  be_nested_str_weak(path_in),
    /* K4   */  be_nested_str_weak(endpoint),
    /* K5   */  be_nested_str_weak(cluster),
    /* K6   */  be_nested_str_weak(attribute),
    /* K7   */  be_nested_str_weak(find),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(get_endpoint),
    /* K10  */  be_nested_str_weak(clusters),
    /* K11  */  be_nested_str_weak(get_cluster_list_sorted),
    }),
    be_str_weak(_next_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x50040000,  //  0004  LDBOOL	R1	0	0
      0x80040200,  //  0005  RET	1	R1
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x88040302,  //  0007  GETMBR	R1	R1	K2
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x88080504,  //  0009  GETMBR	R2	R2	K4
      0x4C0C0000,  //  000A  LDNIL	R3
      0x90020A03,  //  000B  SETMBR	R0	K5	R3
      0x4C0C0000,  //  000C  LDNIL	R3
      0x90020C03,  //  000D  SETMBR	R0	K6	R3
      0x540DFFFE,  //  000E  LDINT	R3	-1
      0x88100100,  //  000F  GETMBR	R4	R0	K0
      0x4C140000,  //  0010  LDNIL	R5
      0x20100805,  //  0011  NE	R4	R4	R5
      0x78120003,  //  0012  JMPF	R4	#0017
      0x8C100307,  //  0013  GETMET	R4	R1	K7
      0x88180100,  //  0014  GETMBR	R6	R0	K0
      0x7C100400,  //  0015  CALL	R4	2
      0x5C0C0800,  //  0016  MOVE	R3	R4
      0x4C100000,  //  0017  LDNIL	R4
      0x20100604,  //  0018  NE	R4	R3	R4
      0x78120017,  //  0019  JMPF	R4	#0032
      0x00100708,  //  001A  ADD	R4	R3	K8
      0x6014000C,  //  001B  GETGBL	R5	G12
      0x5C180200,  //  001C  MOVE	R6	R1
      0x7C140200,  //  001D  CALL	R5	1
      0x14100805,  //  001E  LT	R4	R4	R5
      0x78120011,  //  001F  JMPF	R4	#0032
      0x000C0708,  //  0020  ADD	R3	R3	K8
      0x94100203,  //  0021  GETIDX	R4	R1	R3
      0x90020004,  //  0022  SETMBR	R0	K0	R4
      0x4C100000,  //  0023  LDNIL	R4
      0x1C100404,  //  0024  EQ	R4	R2	R4
      0x74120004,  //  0025  JMPT	R4	#002B
      0x88100100,  //  0026  GETMBR	R4	R0	K0
      0x8C100909,  //  0027  GETMET	R4	R4	K9
      0x7C100200,  //  0028  CALL	R4	1
      0x1C100404,  //  0029  EQ	R4	R2	R4
      0x78120005,  //  002A  JMPF	R4	#0031
      0x88100100,  //  002B  GETMBR	R4	R0	K0
      0x8C10090B,  //  002C  GETMET	R4	R4	K11
      0x7C100200,  //  002D  CALL	R4	1
      0x90021404,  //  002E  SETMBR	R0	K10	R4
      0x88100100,  //  002F  GETMBR	R4	R0	K0
      0x80040800,  //  0030  RET	1	R4
      0x7001FFE7,  //  0031  JMP		#001A
      0x50100000,  //  0032  LDBOOL	R4	0	0
      0x90020004,  //  0033  SETMBR	R0	K0	R4
      0x50100000,  //  0034  LDBOOL	R4	0	0
      0x80040800,  //  0035  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _next_attribute
********************************************************************/
be_local_closure(Matter_PathGenerator__next_attribute,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(attribute),
    /* K1   */  be_nested_str_weak(pi),
    /* K2   */  be_nested_str_weak(get_attribute_list),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(path_in),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(_next_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x50040000,  //  0004  LDBOOL	R1	0	0
      0x80040200,  //  0005  RET	1	R1
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x8C040302,  //  0007  GETMET	R1	R1	K2
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x7C040400,  //  0009  CALL	R1	2
      0x88080104,  //  000A  GETMBR	R2	R0	K4
      0x88080500,  //  000B  GETMBR	R2	R2	K0
      0x540DFFFE,  //  000C  LDINT	R3	-1
      0x88100100,  //  000D  GETMBR	R4	R0	K0
      0x4C140000,  //  000E  LDNIL	R5
      0x20100805,  //  000F  NE	R4	R4	R5
      0x78120003,  //  0010  JMPF	R4	#0015
      0x8C100305,  //  0011  GETMET	R4	R1	K5
      0x88180100,  //  0012  GETMBR	R6	R0	K0
      0x7C100400,  //  0013  CALL	R4	2
      0x5C0C0800,  //  0014  MOVE	R3	R4
      0x4C100000,  //  0015  LDNIL	R4
      0x20100604,  //  0016  NE	R4	R3	R4
      0x78120011,  //  0017  JMPF	R4	#002A
      0x00100706,  //  0018  ADD	R4	R3	K6
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x5C180200,  //  001A  MOVE	R6	R1
      0x7C140200,  //  001B  CALL	R5	1
      0x14100805,  //  001C  LT	R4	R4	R5
      0x7812000B,  //  001D  JMPF	R4	#002A
      0x000C0706,  //  001E  ADD	R3	R3	K6
      0x94100203,  //  001F  GETIDX	R4	R1	R3
      0x90020004,  //  0020  SETMBR	R0	K0	R4
      0x4C100000,  //  0021  LDNIL	R4
      0x1C100404,  //  0022  EQ	R4	R2	R4
      0x74120002,  //  0023  JMPT	R4	#0027
      0x88100100,  //  0024  GETMBR	R4	R0	K0
      0x1C100404,  //  0025  EQ	R4	R2	R4
      0x78120001,  //  0026  JMPF	R4	#0029
      0x88100100,  //  0027  GETMBR	R4	R0	K0
      0x80040800,  //  0028  RET	1	R4
      0x7001FFED,  //  0029  JMP		#0018
      0x50100000,  //  002A  LDBOOL	R4	0	0
      0x90020004,  //  002B  SETMBR	R0	K0	R4
      0x50100000,  //  002C  LDBOOL	R4	0	0
      0x80040800,  //  002D  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_PathGenerator
********************************************************************/
be_local_class(Matter_PathGenerator,
    11,
    NULL,
    be_nested_map(19,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_next_cluster, -1), be_const_closure(Matter_PathGenerator__next_cluster_closure) },
        { be_const_key_weak(_next_endpoint, -1), be_const_closure(Matter_PathGenerator__next_endpoint_closure) },
        { be_const_key_weak(endpoint_found, 0), be_const_var(7) },
        { be_const_key_weak(start, -1), be_const_closure(Matter_PathGenerator_start_closure) },
        { be_const_key_weak(session, -1), be_const_var(2) },
        { be_const_key_weak(get_pi, 12), be_const_closure(Matter_PathGenerator_get_pi_closure) },
        { be_const_key_weak(pi, -1), be_const_var(3) },
        { be_const_key_weak(attribute, 10), be_const_var(5) },
        { be_const_key_weak(cluster_found, 14), be_const_var(8) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(attribute_found, -1), be_const_var(9) },
        { be_const_key_weak(clusters, -1), be_const_var(6) },
        { be_const_key_weak(path_in, -1), be_const_var(1) },
        { be_const_key_weak(init, 11), be_const_closure(Matter_PathGenerator_init_closure) },
        { be_const_key_weak(path_concrete, -1), be_const_var(10) },
        { be_const_key_weak(next, 9), be_const_closure(Matter_PathGenerator_next_closure) },
        { be_const_key_weak(cluster, 4), be_const_var(4) },
        { be_const_key_weak(reset, 1), be_const_closure(Matter_PathGenerator_reset_closure) },
        { be_const_key_weak(_next_attribute, -1), be_const_closure(Matter_PathGenerator__next_attribute_closure) },
    })),
    be_str_weak(Matter_PathGenerator)
);
/*******************************************************************/

void be_load_Matter_PathGenerator_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_PathGenerator);
    be_setglobal(vm, "Matter_PathGenerator");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
