/* Solidification of Matter_Plugin_Bridge_HTTP.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_HTTP;

/********************************************************************
** Solidified function: get_types
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_get_types,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(TYPES),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(stop_iteration),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(get_types),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x60080010,  //  0002  GETGBL	R2	G16
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x88100100,  //  000A  GETMBR	R4	R0	K0
      0x94100803,  //  000B  GETIDX	R4	R4	R3
      0x98040604,  //  000C  SETIDX	R1	R3	R4
      0x7001FFF9,  //  000D  JMP		#0008
      0x58080002,  //  000E  LDCONST	R2	K2
      0xAC080200,  //  000F  CATCH	R2	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x540A0012,  //  0011  LDINT	R2	19
      0x98040503,  //  0012  SETIDX	R1	R2	K3
      0x80040200,  //  0013  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_reachable
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_is_reachable,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(tick),
    /* K2   */  be_nested_str_weak(reachable_tick),
    /* K3   */  be_nested_str_weak(call_remote),
    /* K4   */  be_nested_str_weak(),
    /* K5   */  be_nested_str_weak(reachable),
    }),
    be_str_weak(is_reachable),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x88080102,  //  0002  GETMBR	R2	R0	K2
      0x20040202,  //  0003  NE	R1	R1	R2
      0x78060006,  //  0004  JMPF	R1	#000C
      0x8C040103,  //  0005  GETMET	R1	R0	K3
      0x580C0004,  //  0006  LDCONST	R3	K4
      0x58100004,  //  0007  LDCONST	R4	K4
      0x7C040600,  //  0008  CALL	R1	3
      0x4C080000,  //  0009  LDNIL	R2
      0x20080202,  //  000A  NE	R2	R1	R2
      0x90020A02,  //  000B  SETMBR	R0	K5	R2
      0x88040105,  //  000C  GETMBR	R1	R0	K5
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_read_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(create_TLV),
    /* K6   */  be_nested_str_weak(BOOL),
    /* K7   */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A0038,  //  0004  LDINT	R6	57
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A000B,  //  0006  JMPF	R6	#0013
      0x1C180B04,  //  0007  EQ	R6	R5	K4
      0x781A0000,  //  0008  JMPF	R6	#000A
      0x70020007,  //  0009  JMP		#0012
      0x541A0010,  //  000A  LDINT	R6	17
      0x1C180A06,  //  000B  EQ	R6	R5	R6
      0x781A0004,  //  000C  JMPF	R6	#0012
      0x8C180705,  //  000D  GETMET	R6	R3	K5
      0x88200706,  //  000E  GETMBR	R8	R3	K6
      0x50240200,  //  000F  LDBOOL	R9	1	0
      0x7C180600,  //  0010  CALL	R6	3
      0x80040C00,  //  0011  RET	1	R6
      0x70020007,  //  0012  JMP		#001B
      0x60180003,  //  0013  GETGBL	R6	G3
      0x5C1C0000,  //  0014  MOVE	R7	R0
      0x7C180200,  //  0015  CALL	R6	1
      0x8C180D07,  //  0016  GETMET	R6	R6	K7
      0x5C200200,  //  0017  MOVE	R8	R1
      0x5C240400,  //  0018  MOVE	R9	R2
      0x7C180600,  //  0019  CALL	R6	3
      0x80040C00,  //  001A  RET	1	R6
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_status_11
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_get_status_11,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(tick),
    /* K2   */  be_nested_str_weak(tasmota_status_11),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(_tick),
    /* K5   */  be_nested_str_weak(call_remote),
    /* K6   */  be_nested_str_weak(Status),
    /* K7   */  be_nested_str_weak(11),
    }),
    be_str_weak(get_status_11),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x88080102,  //  0002  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C080403,  //  0004  EQ	R2	R2	R3
      0x740A0005,  //  0005  JMPT	R2	#000C
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x58100004,  //  0008  LDCONST	R4	K4
      0x7C080400,  //  0009  CALL	R2	2
      0x20080401,  //  000A  NE	R2	R2	R1
      0x780A000B,  //  000B  JMPF	R2	#0018
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x58100006,  //  000D  LDCONST	R4	K6
      0x58140007,  //  000E  LDCONST	R5	K7
      0x7C080600,  //  000F  CALL	R2	3
      0x780A0003,  //  0010  JMPF	R2	#0015
      0x980A0801,  //  0011  SETIDX	R2	K4	R1
      0x90020402,  //  0012  SETMBR	R0	K2	R2
      0x80040400,  //  0013  RET	1	R2
      0x70020001,  //  0014  JMP		#0017
      0x4C0C0000,  //  0015  LDNIL	R3
      0x80040600,  //  0016  RET	1	R3
      0x70020001,  //  0017  JMP		#001A
      0x88080102,  //  0018  GETMBR	R2	R0	K2
      0x80040400,  //  0019  RET	1	R2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ui_string_to_conf
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_ui_string_to_conf,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Plugin_Bridge_HTTP),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(split),
    /* K3   */  be_nested_str_weak(_X2C),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(ARG_HTTP),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(ui_string_to_conf),
    /* K8   */  be_const_int(0),
    }),
    be_str_weak(ui_string_to_conf),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0x8C140902,  //  0002  GETMET	R5	R4	K2
      0x001C0503,  //  0003  ADD	R7	R2	K3
      0x58200003,  //  0004  LDCONST	R8	K3
      0x58240004,  //  0005  LDCONST	R9	K4
      0x7C140800,  //  0006  CALL	R5	4
      0x88180705,  //  0007  GETMBR	R6	R3	K5
      0x941C0B06,  //  0008  GETIDX	R7	R5	K6
      0x98040C07,  //  0009  SETIDX	R1	R6	R7
      0x60180003,  //  000A  GETGBL	R6	G3
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C180200,  //  000C  CALL	R6	1
      0x8C180D07,  //  000D  GETMET	R6	R6	K7
      0x5C200000,  //  000E  MOVE	R8	R0
      0x5C240200,  //  000F  MOVE	R9	R1
      0x94280B08,  //  0010  GETIDX	R10	R5	K8
      0x7C180800,  //  0011  CALL	R6	4
      0x60180001,  //  0012  GETGBL	R6	G1
      0x581C0007,  //  0013  LDCONST	R7	K7
      0x5C200200,  //  0014  MOVE	R8	R1
      0x5C240400,  //  0015  MOVE	R9	R2
      0x7C180600,  //  0016  CALL	R6	3
      0x80040200,  //  0017  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: call_remote
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_call_remote,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[31]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_http),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(wifi),
    /* K5   */  be_nested_str_weak(up),
    /* K6   */  be_nested_str_weak(eth),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(webclient),
    /* K10  */  be_nested_str_weak(set_timeouts),
    /* K11  */  be_nested_str_weak(set_follow_redirects),
    /* K12  */  be_nested_str_weak(format),
    /* K13  */  be_nested_str_weak(_X25scm_X3Fcmnd_X3D_X25s_X25_X2520_X25s),
    /* K14  */  be_nested_str_weak(),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20GET_X20),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(begin),
    /* K19  */  be_nested_str_weak(GET),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20GET_X20code_X3D),
    /* K21  */  be_nested_str_weak(get_string),
    /* K22  */  be_nested_str_weak(close),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20GET_X20payload_X3D),
    /* K24  */  be_nested_str_weak(load),
    /* K25  */  be_nested_str_weak(reachable),
    /* K26  */  be_nested_str_weak(reachable_tick),
    /* K27  */  be_nested_str_weak(device),
    /* K28  */  be_nested_str_weak(tick),
    /* K29  */  be_const_int(1),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20GET_X20retrying),
    }),
    be_str_weak(call_remote),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x740E0001,  //  0001  JMPT	R3	#0004
      0x4C0C0000,  //  0002  LDNIL	R3
      0x80040600,  //  0003  RET	1	R3
      0xA40E0200,  //  0004  IMPORT	R3	K1
      0xA4120400,  //  0005  IMPORT	R4	K2
      0xB8160600,  //  0006  GETNGBL	R5	K3
      0x8C140B04,  //  0007  GETMET	R5	R5	K4
      0x7C140200,  //  0008  CALL	R5	1
      0x94140B05,  //  0009  GETIDX	R5	R5	K5
      0x74160006,  //  000A  JMPT	R5	#0012
      0xB8160600,  //  000B  GETNGBL	R5	K3
      0x8C140B06,  //  000C  GETMET	R5	R5	K6
      0x7C140200,  //  000D  CALL	R5	1
      0x94140B05,  //  000E  GETIDX	R5	R5	K5
      0x74160001,  //  000F  JMPT	R5	#0012
      0x4C140000,  //  0010  LDNIL	R5
      0x80040A00,  //  0011  RET	1	R5
      0x58140007,  //  0012  LDCONST	R5	K7
      0x24180B08,  //  0013  GT	R6	R5	K8
      0x781A0041,  //  0014  JMPF	R6	#0057
      0xB81A1200,  //  0015  GETNGBL	R6	K9
      0x7C180000,  //  0016  CALL	R6	0
      0x8C1C0D0A,  //  0017  GETMET	R7	R6	K10
      0x542603E7,  //  0018  LDINT	R9	1000
      0x542A03E7,  //  0019  LDINT	R10	1000
      0x7C1C0600,  //  001A  CALL	R7	3
      0x8C1C0D0B,  //  001B  GETMET	R7	R6	K11
      0x50240000,  //  001C  LDBOOL	R9	0	0
      0x7C1C0400,  //  001D  CALL	R7	2
      0x8C1C090C,  //  001E  GETMET	R7	R4	K12
      0x5824000D,  //  001F  LDCONST	R9	K13
      0x88280100,  //  0020  GETMBR	R10	R0	K0
      0x5C2C0200,  //  0021  MOVE	R11	R1
      0x780A0001,  //  0022  JMPF	R2	#0025
      0x5C300400,  //  0023  MOVE	R12	R2
      0x70020000,  //  0024  JMP		#0026
      0x5830000E,  //  0025  LDCONST	R12	K14
      0x7C1C0A00,  //  0026  CALL	R7	5
      0xB8220600,  //  0027  GETNGBL	R8	K3
      0x8C20110F,  //  0028  GETMET	R8	R8	K15
      0x002A2007,  //  0029  ADD	R10	K16	R7
      0x582C0011,  //  002A  LDCONST	R11	K17
      0x7C200600,  //  002B  CALL	R8	3
      0x8C200D12,  //  002C  GETMET	R8	R6	K18
      0x5C280E00,  //  002D  MOVE	R10	R7
      0x7C200400,  //  002E  CALL	R8	2
      0x8C200D13,  //  002F  GETMET	R8	R6	K19
      0x7C200200,  //  0030  CALL	R8	1
      0xB8260600,  //  0031  GETNGBL	R9	K3
      0x8C24130F,  //  0032  GETMET	R9	R9	K15
      0x602C0008,  //  0033  GETGBL	R11	G8
      0x5C301000,  //  0034  MOVE	R12	R8
      0x7C2C0200,  //  0035  CALL	R11	1
      0x002E280B,  //  0036  ADD	R11	K20	R11
      0x58300011,  //  0037  LDCONST	R12	K17
      0x7C240600,  //  0038  CALL	R9	3
      0x542600C7,  //  0039  LDINT	R9	200
      0x1C241009,  //  003A  EQ	R9	R8	R9
      0x78260011,  //  003B  JMPF	R9	#004E
      0x8C240D15,  //  003C  GETMET	R9	R6	K21
      0x7C240200,  //  003D  CALL	R9	1
      0x8C280D16,  //  003E  GETMET	R10	R6	K22
      0x7C280200,  //  003F  CALL	R10	1
      0xB82A0600,  //  0040  GETNGBL	R10	K3
      0x8C28150F,  //  0041  GETMET	R10	R10	K15
      0x00322E09,  //  0042  ADD	R12	K23	R9
      0x58340011,  //  0043  LDCONST	R13	K17
      0x7C280600,  //  0044  CALL	R10	3
      0x8C280718,  //  0045  GETMET	R10	R3	K24
      0x5C301200,  //  0046  MOVE	R12	R9
      0x7C280400,  //  0047  CALL	R10	2
      0x502C0200,  //  0048  LDBOOL	R11	1	0
      0x9002320B,  //  0049  SETMBR	R0	K25	R11
      0x882C011B,  //  004A  GETMBR	R11	R0	K27
      0x882C171C,  //  004B  GETMBR	R11	R11	K28
      0x9002340B,  //  004C  SETMBR	R0	K26	R11
      0x80041400,  //  004D  RET	1	R10
      0x8C240D16,  //  004E  GETMET	R9	R6	K22
      0x7C240200,  //  004F  CALL	R9	1
      0x04140B1D,  //  0050  SUB	R5	R5	K29
      0xB8260600,  //  0051  GETNGBL	R9	K3
      0x8C24130F,  //  0052  GETMET	R9	R9	K15
      0x582C001E,  //  0053  LDCONST	R11	K30
      0x58300011,  //  0054  LDCONST	R12	K17
      0x7C240600,  //  0055  CALL	R9	3
      0x7001FFBB,  //  0056  JMP		#0013
      0x50180000,  //  0057  LDBOOL	R6	0	0
      0x90023206,  //  0058  SETMBR	R0	K25	R6
      0x4C180000,  //  0059  LDNIL	R6
      0x80040C00,  //  005A  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_status_8
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_get_status_8,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(tick),
    /* K2   */  be_nested_str_weak(tasmota_status_8),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(_tick),
    /* K5   */  be_nested_str_weak(call_remote),
    /* K6   */  be_nested_str_weak(Status),
    /* K7   */  be_nested_str_weak(8),
    }),
    be_str_weak(get_status_8),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x88080102,  //  0002  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C080403,  //  0004  EQ	R2	R2	R3
      0x740A0005,  //  0005  JMPT	R2	#000C
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x58100004,  //  0008  LDCONST	R4	K4
      0x7C080400,  //  0009  CALL	R2	2
      0x20080401,  //  000A  NE	R2	R2	R1
      0x780A000B,  //  000B  JMPF	R2	#0018
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x58100006,  //  000D  LDCONST	R4	K6
      0x58140007,  //  000E  LDCONST	R5	K7
      0x7C080600,  //  000F  CALL	R2	3
      0x780A0003,  //  0010  JMPF	R2	#0015
      0x980A0801,  //  0011  SETIDX	R2	K4	R1
      0x90020402,  //  0012  SETMBR	R0	K2	R2
      0x80040400,  //  0013  RET	1	R2
      0x70020001,  //  0014  JMP		#0017
      0x4C0C0000,  //  0015  LDNIL	R3
      0x80040600,  //  0016  RET	1	R3
      0x70020001,  //  0017  JMP		#001A
      0x88080102,  //  0018  GETMBR	R2	R0	K2
      0x80040400,  //  0019  RET	1	R2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ui_conf_to_string
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_ui_conf_to_string,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Plugin_Bridge_HTTP),
    /* K1   */  be_nested_str_weak(ui_conf_to_string),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(ARG_HTTP),
    /* K4   */  be_nested_str_weak(),
    /* K5   */  be_nested_str_weak(_X2C),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20ui_conf_to_string),
    }),
    be_str_weak(ui_conf_to_string),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C0003,  //  0001  GETGBL	R3	G3
      0x5C100400,  //  0002  MOVE	R4	R2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x5C140000,  //  0005  MOVE	R5	R0
      0x5C180200,  //  0006  MOVE	R6	R1
      0x7C0C0600,  //  0007  CALL	R3	3
      0x60100008,  //  0008  GETGBL	R4	G8
      0x8C140302,  //  0009  GETMET	R5	R1	K2
      0x881C0503,  //  000A  GETMBR	R7	R2	K3
      0x58200004,  //  000B  LDCONST	R8	K4
      0x7C140600,  //  000C  CALL	R5	3
      0x7C100200,  //  000D  CALL	R4	1
      0x00140705,  //  000E  ADD	R5	R3	K5
      0x00140A04,  //  000F  ADD	R5	R5	R4
      0x60180001,  //  0010  GETGBL	R6	G1
      0x581C0006,  //  0011  LDCONST	R7	K6
      0x5C200200,  //  0012  MOVE	R8	R1
      0x5C240000,  //  0013  MOVE	R9	R0
      0x5C280A00,  //  0014  MOVE	R10	R5
      0x7C180800,  //  0015  CALL	R6	4
      0x80040A00,  //  0016  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_init,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(init),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(ARG_HTTP),
    /* K4   */  be_nested_str_weak(_X3A_X2F_X2F),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(http_X3A_X2F_X2F),
    /* K7   */  be_nested_str_weak(_X2F),
    /* K8   */  be_nested_str_weak(tasmota_http),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(log),
    /* K11  */  be_nested_str_weak(format),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20ERROR_X3A_X20_X27url_X27_X20is_X20not_X20configured_X20for_X20endpoint_X20_X25i),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(tasmota_status_8),
    /* K15  */  be_nested_str_weak(tasmota_status_11),
    /* K16  */  be_nested_str_weak(reachable),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x60140003,  //  0001  GETGBL	R5	G3
      0x5C180000,  //  0002  MOVE	R6	R0
      0x7C140200,  //  0003  CALL	R5	1
      0x8C140B01,  //  0004  GETMET	R5	R5	K1
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x5C200400,  //  0006  MOVE	R8	R2
      0x5C240600,  //  0007  MOVE	R9	R3
      0x7C140800,  //  0008  CALL	R5	4
      0x8C140702,  //  0009  GETMET	R5	R3	K2
      0x881C0103,  //  000A  GETMBR	R7	R0	K3
      0x7C140400,  //  000B  CALL	R5	2
      0x7816000A,  //  000C  JMPF	R5	#0018
      0x8C180902,  //  000D  GETMET	R6	R4	K2
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x58240004,  //  000F  LDCONST	R9	K4
      0x7C180600,  //  0010  CALL	R6	3
      0x14180D05,  //  0011  LT	R6	R6	K5
      0x781A0002,  //  0012  JMPF	R6	#0016
      0x001A0C05,  //  0013  ADD	R6	K6	R5
      0x00180D07,  //  0014  ADD	R6	R6	K7
      0x5C140C00,  //  0015  MOVE	R5	R6
      0x90021005,  //  0016  SETMBR	R0	K8	R5
      0x70020007,  //  0017  JMP		#0020
      0xB81A1200,  //  0018  GETNGBL	R6	K9
      0x8C180D0A,  //  0019  GETMET	R6	R6	K10
      0x8C20090B,  //  001A  GETMET	R8	R4	K11
      0x5828000C,  //  001B  LDCONST	R10	K12
      0x5C2C0400,  //  001C  MOVE	R11	R2
      0x7C200600,  //  001D  CALL	R8	3
      0x5824000D,  //  001E  LDCONST	R9	K13
      0x7C180600,  //  001F  CALL	R6	3
      0x4C180000,  //  0020  LDNIL	R6
      0x90021C06,  //  0021  SETMBR	R0	K14	R6
      0x4C180000,  //  0022  LDNIL	R6
      0x90021E06,  //  0023  SETMBR	R0	K15	R6
      0x50180000,  //  0024  LDBOOL	R6	0	0
      0x90022006,  //  0025  SETMBR	R0	K16	R6
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_HTTP
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Bridge_HTTP,
    5,
    &be_class_Matter_Plugin_Device,
    be_nested_map(21,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_types, 20), be_const_closure(Matter_Plugin_Bridge_HTTP_get_types_closure) },
        { be_const_key_weak(is_reachable, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_is_reachable_closure) },
        { be_const_key_weak(tasmota_http, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_init_closure) },
        { be_const_key_weak(reachable_tick, -1), be_const_var(4) },
        { be_const_key_weak(get_status_8, 6), be_const_closure(Matter_Plugin_Bridge_HTTP_get_status_8_closure) },
        { be_const_key_weak(tasmota_status_11, -1), be_const_var(2) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(17),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak() },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(3000) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak() },
        { be_const_key_weak(reachable, -1), be_const_var(3) },
        { be_const_key_weak(ARG_HTTP, 3), be_nested_str_weak(url) },
        { be_const_key_weak(ui_string_to_conf, -1), be_const_static_closure(Matter_Plugin_Bridge_HTTP_ui_string_to_conf_closure) },
        { be_const_key_weak(tasmota_status_8, 18), be_const_var(1) },
        { be_const_key_weak(ARG, 8), be_nested_str_weak() },
        { be_const_key_weak(call_remote, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_call_remote_closure) },
        { be_const_key_weak(get_status_11, 5), be_const_closure(Matter_Plugin_Bridge_HTTP_get_status_11_closure) },
        { be_const_key_weak(HTTP_TIMEOUT, 4), be_const_int(300) },
        { be_const_key_weak(ui_conf_to_string, -1), be_const_static_closure(Matter_Plugin_Bridge_HTTP_ui_conf_to_string_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_read_attribute_closure) },
    })),
    be_str_weak(Matter_Plugin_Bridge_HTTP)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_HTTP_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_HTTP);
    be_setglobal(vm, "Matter_Plugin_Bridge_HTTP");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
