/* Solidification of Matter_Path_1_PathGenerator.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_PathGenerator' ktab size: 37, total: 80 (saved 344 bytes)
static const bvalue be_ktab_class_Matter_PathGenerator[37] = {
  /* K0   */  be_nested_str_weak(path_concrete),
  /* K1   */  be_nested_str_weak(reset),
  /* K2   */  be_nested_str_weak(pi),
  /* K3   */  be_nested_str_weak(cluster),
  /* K4   */  be_nested_str_weak(attribute),
  /* K5   */  be_nested_str_weak(clusters),
  /* K6   */  be_nested_str_weak(device),
  /* K7   */  be_nested_str_weak(path_in_endpoint),
  /* K8   */  be_nested_str_weak(path_in_cluster),
  /* K9   */  be_nested_str_weak(path_in_attribute),
  /* K10  */  be_nested_str_weak(is_direct),
  /* K11  */  be_nested_str_weak(_next_endpoint),
  /* K12  */  be_nested_str_weak(endpoint_found),
  /* K13  */  be_nested_str_weak(_next_cluster),
  /* K14  */  be_nested_str_weak(cluster_found),
  /* K15  */  be_nested_str_weak(_next_attribute),
  /* K16  */  be_nested_str_weak(attribute_found),
  /* K17  */  be_nested_str_weak(endpoint),
  /* K18  */  be_nested_str_weak(get_endpoint),
  /* K19  */  be_nested_str_weak(fabric_filtered),
  /* K20  */  be_nested_str_weak(path_in_fabric_filtered),
  /* K21  */  be_nested_str_weak(status),
  /* K22  */  be_nested_str_weak(_default_status_error),
  /* K23  */  be_nested_str_weak(find),
  /* K24  */  be_const_int(1),
  /* K25  */  be_nested_str_weak(matter),
  /* K26  */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
  /* K27  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
  /* K28  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
  /* K29  */  be_nested_str_weak(UNREPORTABLE_ATTRIBUTE),
  /* K30  */  be_nested_str_weak(get_attribute_list_bytes),
  /* K31  */  be_const_int(2),
  /* K32  */  be_const_int(0),
  /* K33  */  be_nested_str_weak(get),
  /* K34  */  be_nested_str_weak(plugins),
  /* K35  */  be_nested_str_weak(get_cluster_list_sorted),
  /* K36  */  be_nested_str_weak(Path),
};


extern const bclass be_class_Matter_PathGenerator;

/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(class_Matter_PathGenerator_reset,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x50080000,  //  0004  LDBOOL	R2	0	0
      0x90020402,  //  0005  SETMBR	R0	K2	R2
      0x90020601,  //  0006  SETMBR	R0	K3	R1
      0x90020801,  //  0007  SETMBR	R0	K4	R1
      0x90020A01,  //  0008  SETMBR	R0	K5	R1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_PathGenerator_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020C01,  //  0000  SETMBR	R0	K6	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_direct
********************************************************************/
be_local_closure(class_Matter_PathGenerator_is_direct,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(is_direct),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060007,  //  0003  JMPF	R1	#000C
      0x88040108,  //  0004  GETMBR	R1	R0	K8
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060003,  //  0007  JMPF	R1	#000C
      0x88040109,  //  0008  GETMBR	R1	R0	K9
      0x4C080000,  //  0009  LDNIL	R2
      0x20040202,  //  000A  NE	R1	R1	R2
      0x74060000,  //  000B  JMPT	R1	#000D
      0x50040001,  //  000C  LDBOOL	R1	0	1
      0x50040200,  //  000D  LDBOOL	R1	1	0
      0x80040200,  //  000E  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_finished
********************************************************************/
be_local_closure(class_Matter_PathGenerator_is_finished,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(is_finished),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0x20040202,  //  0002  NE	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next_attribute
********************************************************************/
be_local_closure(class_Matter_PathGenerator_next_attribute,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(next_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[95]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x50080200,  //  0001  LDBOOL	R2	1	0
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x74060006,  //  0003  JMPT	R1	#000B
      0x88040102,  //  0004  GETMBR	R1	R0	K2
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060006,  //  0007  JMPF	R1	#000F
      0x8C04010A,  //  0008  GETMET	R1	R0	K10
      0x7C040200,  //  0009  CALL	R1	1
      0x78060003,  //  000A  JMPF	R1	#000F
      0x8C040101,  //  000B  GETMET	R1	R0	K1
      0x7C040200,  //  000C  CALL	R1	1
      0x4C040000,  //  000D  LDNIL	R1
      0x80040200,  //  000E  RET	1	R1
      0x88040102,  //  000F  GETMBR	R1	R0	K2
      0x50080000,  //  0010  LDBOOL	R2	0	0
      0x20040202,  //  0011  NE	R1	R1	R2
      0x78060033,  //  0012  JMPF	R1	#0047
      0x88040102,  //  0013  GETMBR	R1	R0	K2
      0x4C080000,  //  0014  LDNIL	R2
      0x1C040202,  //  0015  EQ	R1	R1	R2
      0x74060003,  //  0016  JMPT	R1	#001B
      0x88040103,  //  0017  GETMBR	R1	R0	K3
      0x50080000,  //  0018  LDBOOL	R2	0	0
      0x1C040202,  //  0019  EQ	R1	R1	R2
      0x78060002,  //  001A  JMPF	R1	#001E
      0x8C04010B,  //  001B  GETMET	R1	R0	K11
      0x7C040200,  //  001C  CALL	R1	1
      0x7001FFF0,  //  001D  JMP		#000F
      0x50040200,  //  001E  LDBOOL	R1	1	0
      0x90021801,  //  001F  SETMBR	R0	K12	R1
      0x88040103,  //  0020  GETMBR	R1	R0	K3
      0x4C080000,  //  0021  LDNIL	R2
      0x1C040202,  //  0022  EQ	R1	R1	R2
      0x74060003,  //  0023  JMPT	R1	#0028
      0x88040104,  //  0024  GETMBR	R1	R0	K4
      0x50080000,  //  0025  LDBOOL	R2	0	0
      0x1C040202,  //  0026  EQ	R1	R1	R2
      0x78060002,  //  0027  JMPF	R1	#002B
      0x8C04010D,  //  0028  GETMET	R1	R0	K13
      0x7C040200,  //  0029  CALL	R1	1
      0x7001FFE3,  //  002A  JMP		#000F
      0x50040200,  //  002B  LDBOOL	R1	1	0
      0x90021C01,  //  002C  SETMBR	R0	K14	R1
      0x8C04010F,  //  002D  GETMET	R1	R0	K15
      0x7C040200,  //  002E  CALL	R1	1
      0x88040104,  //  002F  GETMBR	R1	R0	K4
      0x50080000,  //  0030  LDBOOL	R2	0	0
      0x1C040202,  //  0031  EQ	R1	R1	R2
      0x78060000,  //  0032  JMPF	R1	#0034
      0x7001FFDA,  //  0033  JMP		#000F
      0x50040200,  //  0034  LDBOOL	R1	1	0
      0x90022001,  //  0035  SETMBR	R0	K16	R1
      0x88040100,  //  0036  GETMBR	R1	R0	K0
      0x8C080301,  //  0037  GETMET	R2	R1	K1
      0x7C080200,  //  0038  CALL	R2	1
      0x88080102,  //  0039  GETMBR	R2	R0	K2
      0x8C080512,  //  003A  GETMET	R2	R2	K18
      0x7C080200,  //  003B  CALL	R2	1
      0x90062202,  //  003C  SETMBR	R1	K17	R2
      0x88080103,  //  003D  GETMBR	R2	R0	K3
      0x90060602,  //  003E  SETMBR	R1	K3	R2
      0x88080104,  //  003F  GETMBR	R2	R0	K4
      0x90060802,  //  0040  SETMBR	R1	K4	R2
      0x88080114,  //  0041  GETMBR	R2	R0	K20
      0x90062602,  //  0042  SETMBR	R1	K19	R2
      0x4C080000,  //  0043  LDNIL	R2
      0x90062A02,  //  0044  SETMBR	R1	K21	R2
      0x80040200,  //  0045  RET	1	R1
      0x7001FFC7,  //  0046  JMP		#000F
      0x8C04010A,  //  0047  GETMET	R1	R0	K10
      0x7C040200,  //  0048  CALL	R1	1
      0x78060010,  //  0049  JMPF	R1	#005B
      0x88040100,  //  004A  GETMBR	R1	R0	K0
      0x8C080301,  //  004B  GETMET	R2	R1	K1
      0x7C080200,  //  004C  CALL	R2	1
      0x88080107,  //  004D  GETMBR	R2	R0	K7
      0x90062202,  //  004E  SETMBR	R1	K17	R2
      0x88080108,  //  004F  GETMBR	R2	R0	K8
      0x90060602,  //  0050  SETMBR	R1	K3	R2
      0x88080109,  //  0051  GETMBR	R2	R0	K9
      0x90060802,  //  0052  SETMBR	R1	K4	R2
      0x88080114,  //  0053  GETMBR	R2	R0	K20
      0x90062602,  //  0054  SETMBR	R1	K19	R2
      0x8C080116,  //  0055  GETMET	R2	R0	K22
      0x7C080200,  //  0056  CALL	R2	1
      0x90062A02,  //  0057  SETMBR	R1	K21	R2
      0x50080200,  //  0058  LDBOOL	R2	1	0
      0x90020402,  //  0059  SETMBR	R0	K2	R2
      0x80040200,  //  005A  RET	1	R1
      0x8C040101,  //  005B  GETMET	R1	R0	K1
      0x7C040200,  //  005C  CALL	R1	1
      0x4C040000,  //  005D  LDNIL	R1
      0x80040200,  //  005E  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _next_cluster
********************************************************************/
be_local_closure(class_Matter_PathGenerator__next_cluster,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(_next_cluster),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x88040103,  //  0000  GETMBR	R1	R0	K3
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x50040000,  //  0004  LDBOOL	R1	0	0
      0x80040200,  //  0005  RET	1	R1
      0x88040105,  //  0006  GETMBR	R1	R0	K5
      0x88080108,  //  0007  GETMBR	R2	R0	K8
      0x4C0C0000,  //  0008  LDNIL	R3
      0x90020803,  //  0009  SETMBR	R0	K4	R3
      0x540DFFFE,  //  000A  LDINT	R3	-1
      0x88100103,  //  000B  GETMBR	R4	R0	K3
      0x4C140000,  //  000C  LDNIL	R5
      0x20100805,  //  000D  NE	R4	R4	R5
      0x78120003,  //  000E  JMPF	R4	#0013
      0x8C100317,  //  000F  GETMET	R4	R1	K23
      0x88180103,  //  0010  GETMBR	R6	R0	K3
      0x7C100400,  //  0011  CALL	R4	2
      0x5C0C0800,  //  0012  MOVE	R3	R4
      0x4C100000,  //  0013  LDNIL	R4
      0x20100604,  //  0014  NE	R4	R3	R4
      0x78120011,  //  0015  JMPF	R4	#0028
      0x00100718,  //  0016  ADD	R4	R3	K24
      0x6014000C,  //  0017  GETGBL	R5	G12
      0x5C180200,  //  0018  MOVE	R6	R1
      0x7C140200,  //  0019  CALL	R5	1
      0x14100805,  //  001A  LT	R4	R4	R5
      0x7812000B,  //  001B  JMPF	R4	#0028
      0x000C0718,  //  001C  ADD	R3	R3	K24
      0x94100203,  //  001D  GETIDX	R4	R1	R3
      0x90020604,  //  001E  SETMBR	R0	K3	R4
      0x4C100000,  //  001F  LDNIL	R4
      0x1C100404,  //  0020  EQ	R4	R2	R4
      0x74120002,  //  0021  JMPT	R4	#0025
      0x88100103,  //  0022  GETMBR	R4	R0	K3
      0x1C100404,  //  0023  EQ	R4	R2	R4
      0x78120001,  //  0024  JMPF	R4	#0027
      0x88100103,  //  0025  GETMBR	R4	R0	K3
      0x80040800,  //  0026  RET	1	R4
      0x7001FFED,  //  0027  JMP		#0016
      0x50100000,  //  0028  LDBOOL	R4	0	0
      0x90020604,  //  0029  SETMBR	R0	K3	R4
      0x50100000,  //  002A  LDBOOL	R4	0	0
      0x80040800,  //  002B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _default_status_error
********************************************************************/
be_local_closure(class_Matter_PathGenerator__default_status_error,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(_default_status_error),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C04010A,  //  0000  GETMET	R1	R0	K10
      0x7C040200,  //  0001  CALL	R1	1
      0x78060011,  //  0002  JMPF	R1	#0015
      0x8804010C,  //  0003  GETMBR	R1	R0	K12
      0x74060002,  //  0004  JMPT	R1	#0008
      0xB8063200,  //  0005  GETNGBL	R1	K25
      0x8804031A,  //  0006  GETMBR	R1	R1	K26
      0x80040200,  //  0007  RET	1	R1
      0x8804010E,  //  0008  GETMBR	R1	R0	K14
      0x74060002,  //  0009  JMPT	R1	#000D
      0xB8063200,  //  000A  GETNGBL	R1	K25
      0x8804031B,  //  000B  GETMBR	R1	R1	K27
      0x80040200,  //  000C  RET	1	R1
      0x88040110,  //  000D  GETMBR	R1	R0	K16
      0x74060002,  //  000E  JMPT	R1	#0012
      0xB8063200,  //  000F  GETNGBL	R1	K25
      0x8804031C,  //  0010  GETMBR	R1	R1	K28
      0x80040200,  //  0011  RET	1	R1
      0xB8063200,  //  0012  GETNGBL	R1	K25
      0x8804031D,  //  0013  GETMBR	R1	R1	K29
      0x80040200,  //  0014  RET	1	R1
      0x4C040000,  //  0015  LDNIL	R1
      0x80040200,  //  0016  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _next_attribute
********************************************************************/
be_local_closure(class_Matter_PathGenerator__next_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(_next_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x50040000,  //  0004  LDBOOL	R1	0	0
      0x80040200,  //  0005  RET	1	R1
      0x88040102,  //  0006  GETMBR	R1	R0	K2
      0x8C04031E,  //  0007  GETMET	R1	R1	K30
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x7C040400,  //  0009  CALL	R1	2
      0x4C080000,  //  000A  LDNIL	R2
      0x20080202,  //  000B  NE	R2	R1	R2
      0x780A0004,  //  000C  JMPF	R2	#0012
      0x6008000C,  //  000D  GETGBL	R2	G12
      0x5C0C0200,  //  000E  MOVE	R3	R1
      0x7C080200,  //  000F  CALL	R2	1
      0x0C08051F,  //  0010  DIV	R2	R2	K31
      0x70020000,  //  0011  JMP		#0013
      0x58080020,  //  0012  LDCONST	R2	K32
      0x880C0109,  //  0013  GETMBR	R3	R0	K9
      0x5411FFFE,  //  0014  LDINT	R4	-1
      0x88140104,  //  0015  GETMBR	R5	R0	K4
      0x4C180000,  //  0016  LDNIL	R6
      0x20140A06,  //  0017  NE	R5	R5	R6
      0x78160012,  //  0018  JMPF	R5	#002C
      0x58140020,  //  0019  LDCONST	R5	K32
      0x50180200,  //  001A  LDBOOL	R6	1	0
      0x781A000F,  //  001B  JMPF	R6	#002C
      0x14180A02,  //  001C  LT	R6	R5	R2
      0x781A000A,  //  001D  JMPF	R6	#0029
      0x8C180321,  //  001E  GETMET	R6	R1	K33
      0x08200B1F,  //  001F  MUL	R8	R5	K31
      0x5425FFFD,  //  0020  LDINT	R9	-2
      0x7C180600,  //  0021  CALL	R6	3
      0x881C0104,  //  0022  GETMBR	R7	R0	K4
      0x1C180C07,  //  0023  EQ	R6	R6	R7
      0x781A0001,  //  0024  JMPF	R6	#0027
      0x5C100A00,  //  0025  MOVE	R4	R5
      0x70020004,  //  0026  JMP		#002C
      0x00140B18,  //  0027  ADD	R5	R5	K24
      0x70020001,  //  0028  JMP		#002B
      0x4C100000,  //  0029  LDNIL	R4
      0x70020000,  //  002A  JMP		#002C
      0x7001FFED,  //  002B  JMP		#001A
      0x4C140000,  //  002C  LDNIL	R5
      0x20140805,  //  002D  NE	R5	R4	R5
      0x78160011,  //  002E  JMPF	R5	#0041
      0x00140918,  //  002F  ADD	R5	R4	K24
      0x14140A02,  //  0030  LT	R5	R5	R2
      0x7816000E,  //  0031  JMPF	R5	#0041
      0x00100918,  //  0032  ADD	R4	R4	K24
      0x8C140321,  //  0033  GETMET	R5	R1	K33
      0x081C091F,  //  0034  MUL	R7	R4	K31
      0x5421FFFD,  //  0035  LDINT	R8	-2
      0x7C140600,  //  0036  CALL	R5	3
      0x90020805,  //  0037  SETMBR	R0	K4	R5
      0x4C140000,  //  0038  LDNIL	R5
      0x1C140605,  //  0039  EQ	R5	R3	R5
      0x74160002,  //  003A  JMPT	R5	#003E
      0x88140104,  //  003B  GETMBR	R5	R0	K4
      0x1C140605,  //  003C  EQ	R5	R3	R5
      0x78160001,  //  003D  JMPF	R5	#0040
      0x88140104,  //  003E  GETMBR	R5	R0	K4
      0x80040A00,  //  003F  RET	1	R5
      0x7001FFED,  //  0040  JMP		#002F
      0x50140000,  //  0041  LDBOOL	R5	0	0
      0x90020805,  //  0042  SETMBR	R0	K4	R5
      0x50140000,  //  0043  LDBOOL	R5	0	0
      0x80040A00,  //  0044  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _next_endpoint
********************************************************************/
be_local_closure(class_Matter_PathGenerator__next_endpoint,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(_next_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x50040000,  //  0004  LDBOOL	R1	0	0
      0x80040200,  //  0005  RET	1	R1
      0x88040106,  //  0006  GETMBR	R1	R0	K6
      0x88040322,  //  0007  GETMBR	R1	R1	K34
      0x88080107,  //  0008  GETMBR	R2	R0	K7
      0x4C0C0000,  //  0009  LDNIL	R3
      0x90020603,  //  000A  SETMBR	R0	K3	R3
      0x4C0C0000,  //  000B  LDNIL	R3
      0x90020803,  //  000C  SETMBR	R0	K4	R3
      0x540DFFFE,  //  000D  LDINT	R3	-1
      0x88100102,  //  000E  GETMBR	R4	R0	K2
      0x4C140000,  //  000F  LDNIL	R5
      0x20100805,  //  0010  NE	R4	R4	R5
      0x78120003,  //  0011  JMPF	R4	#0016
      0x8C100317,  //  0012  GETMET	R4	R1	K23
      0x88180102,  //  0013  GETMBR	R6	R0	K2
      0x7C100400,  //  0014  CALL	R4	2
      0x5C0C0800,  //  0015  MOVE	R3	R4
      0x4C100000,  //  0016  LDNIL	R4
      0x20100604,  //  0017  NE	R4	R3	R4
      0x78120017,  //  0018  JMPF	R4	#0031
      0x00100718,  //  0019  ADD	R4	R3	K24
      0x6014000C,  //  001A  GETGBL	R5	G12
      0x5C180200,  //  001B  MOVE	R6	R1
      0x7C140200,  //  001C  CALL	R5	1
      0x14100805,  //  001D  LT	R4	R4	R5
      0x78120011,  //  001E  JMPF	R4	#0031
      0x000C0718,  //  001F  ADD	R3	R3	K24
      0x94100203,  //  0020  GETIDX	R4	R1	R3
      0x90020404,  //  0021  SETMBR	R0	K2	R4
      0x4C100000,  //  0022  LDNIL	R4
      0x1C100404,  //  0023  EQ	R4	R2	R4
      0x74120004,  //  0024  JMPT	R4	#002A
      0x88100102,  //  0025  GETMBR	R4	R0	K2
      0x8C100912,  //  0026  GETMET	R4	R4	K18
      0x7C100200,  //  0027  CALL	R4	1
      0x1C100404,  //  0028  EQ	R4	R2	R4
      0x78120005,  //  0029  JMPF	R4	#0030
      0x88100102,  //  002A  GETMBR	R4	R0	K2
      0x8C100923,  //  002B  GETMET	R4	R4	K35
      0x7C100200,  //  002C  CALL	R4	1
      0x90020A04,  //  002D  SETMBR	R0	K5	R4
      0x88100102,  //  002E  GETMBR	R4	R0	K2
      0x80040800,  //  002F  RET	1	R4
      0x7001FFE7,  //  0030  JMP		#0019
      0x50100000,  //  0031  LDBOOL	R4	0	0
      0x90020404,  //  0032  SETMBR	R0	K2	R4
      0x50100000,  //  0033  LDBOOL	R4	0	0
      0x80040800,  //  0034  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pi
********************************************************************/
be_local_closure(class_Matter_PathGenerator_get_pi,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(get_pi),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x74060003,  //  0003  JMPT	R1	#0008
      0x88040102,  //  0004  GETMBR	R1	R0	K2
      0x50080200,  //  0005  LDBOOL	R2	1	0
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x78060001,  //  0007  JMPF	R1	#000A
      0x4C040000,  //  0008  LDNIL	R1
      0x70020000,  //  0009  JMP		#000B
      0x88040102,  //  000A  GETMBR	R1	R0	K2
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_Matter_PathGenerator_start,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_PathGenerator,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xB8163200,  //  0000  GETNGBL	R5	K25
      0x8C140B24,  //  0001  GETMET	R5	R5	K36
      0x7C140200,  //  0002  CALL	R5	1
      0x90020005,  //  0003  SETMBR	R0	K0	R5
      0x8C140101,  //  0004  GETMET	R5	R0	K1
      0x7C140200,  //  0005  CALL	R5	1
      0x90020E01,  //  0006  SETMBR	R0	K7	R1
      0x90021002,  //  0007  SETMBR	R0	K8	R2
      0x90021203,  //  0008  SETMBR	R0	K9	R3
      0x60140017,  //  0009  GETGBL	R5	G23
      0x5C180800,  //  000A  MOVE	R6	R4
      0x7C140200,  //  000B  CALL	R5	1
      0x90022805,  //  000C  SETMBR	R0	K20	R5
      0x4C140000,  //  000D  LDNIL	R5
      0x90020405,  //  000E  SETMBR	R0	K2	R5
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x90021805,  //  0010  SETMBR	R0	K12	R5
      0x50140000,  //  0011  LDBOOL	R5	0	0
      0x90021C05,  //  0012  SETMBR	R0	K14	R5
      0x50140000,  //  0013  LDBOOL	R5	0	0
      0x90022005,  //  0014  SETMBR	R0	K16	R5
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_PathGenerator
********************************************************************/
be_local_class(Matter_PathGenerator,
    13,
    NULL,
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(reset, -1), be_const_closure(class_Matter_PathGenerator_reset_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_Matter_PathGenerator_start_closure) },
        { be_const_key_weak(pi, -1), be_const_var(5) },
        { be_const_key_weak(init, 15), be_const_closure(class_Matter_PathGenerator_init_closure) },
        { be_const_key_weak(path_in_endpoint, -1), be_const_var(1) },
        { be_const_key_weak(path_in_cluster, -1), be_const_var(2) },
        { be_const_key_weak(get_pi, -1), be_const_closure(class_Matter_PathGenerator_get_pi_closure) },
        { be_const_key_weak(is_direct, 9), be_const_closure(class_Matter_PathGenerator_is_direct_closure) },
        { be_const_key_weak(is_finished, -1), be_const_closure(class_Matter_PathGenerator_is_finished_closure) },
        { be_const_key_weak(next_attribute, 22), be_const_closure(class_Matter_PathGenerator_next_attribute_closure) },
        { be_const_key_weak(cluster_found, 17), be_const_var(10) },
        { be_const_key_weak(endpoint_found, 6), be_const_var(9) },
        { be_const_key_weak(_next_attribute, -1), be_const_closure(class_Matter_PathGenerator__next_attribute_closure) },
        { be_const_key_weak(cluster, -1), be_const_var(6) },
        { be_const_key_weak(_next_cluster, -1), be_const_closure(class_Matter_PathGenerator__next_cluster_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(path_concrete, -1), be_const_var(12) },
        { be_const_key_weak(clusters, -1), be_const_var(8) },
        { be_const_key_weak(path_in_fabric_filtered, -1), be_const_var(4) },
        { be_const_key_weak(_default_status_error, -1), be_const_closure(class_Matter_PathGenerator__default_status_error_closure) },
        { be_const_key_weak(attribute_found, 12), be_const_var(11) },
        { be_const_key_weak(_next_endpoint, -1), be_const_closure(class_Matter_PathGenerator__next_endpoint_closure) },
        { be_const_key_weak(path_in_attribute, -1), be_const_var(3) },
        { be_const_key_weak(attribute, 1), be_const_var(7) },
    })),
    be_str_weak(Matter_PathGenerator)
);
/********************************************************************/
/* End of solidification */
