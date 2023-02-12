/* Solidification of Matter_Session.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Session;

/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(Matter_Session_save,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(__store),
    /* K1   */  be_nested_str_weak(save),
    }),
    be_str_weak(save),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_expire_in_seconds
********************************************************************/
be_local_closure(Matter_Session_set_expire_in_seconds,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(rtc),
    /* K2   */  be_nested_str_weak(utc),
    /* K3   */  be_nested_str_weak(set_expire_time),
    }),
    be_str_weak(set_expire_in_seconds),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0203,  //  0001  EQ	R3	R1	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x80000600,  //  0003  RET	0
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C0C0403,  //  0005  EQ	R3	R2	R3
      0x780E0003,  //  0006  JMPF	R3	#000B
      0xB80E0000,  //  0007  GETNGBL	R3	K0
      0x8C0C0701,  //  0008  GETMET	R3	R3	K1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x94080702,  //  000A  GETIDX	R2	R3	K2
      0x8C0C0103,  //  000B  GETMET	R3	R0	K3
      0x00140401,  //  000C  ADD	R5	R2	R1
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_i2r_privacy
********************************************************************/
be_local_closure(Matter_Session_get_i2r_privacy,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(_i2r_privacy),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(HKDF_SHA256),
    /* K3   */  be_nested_str_weak(derive),
    /* K4   */  be_nested_str_weak(get_i2r),
    /* K5   */  be_nested_str_weak(fromstring),
    /* K6   */  be_nested_str_weak(PrivacyKey),
    }),
    be_str_weak(get_i2r_privacy),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806000F,  //  0003  JMPF	R1	#0014
      0xA4060200,  //  0004  IMPORT	R1	K1
      0x8C080302,  //  0005  GETMET	R2	R1	K2
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x8C100104,  //  0008  GETMET	R4	R0	K4
      0x7C100200,  //  0009  CALL	R4	1
      0x60140015,  //  000A  GETGBL	R5	G21
      0x7C140000,  //  000B  CALL	R5	0
      0x60180015,  //  000C  GETGBL	R6	G21
      0x7C180000,  //  000D  CALL	R6	0
      0x8C180D05,  //  000E  GETMET	R6	R6	K5
      0x58200006,  //  000F  LDCONST	R8	K6
      0x7C180400,  //  0010  CALL	R6	2
      0x541E000F,  //  0011  LDINT	R7	16
      0x7C080A00,  //  0012  CALL	R2	5
      0x90020002,  //  0013  SETMBR	R0	K0	R2
      0x88040100,  //  0014  GETMBR	R1	R0	K0
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has_expired
********************************************************************/
be_local_closure(Matter_Session_has_expired,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(rtc),
    /* K2   */  be_nested_str_weak(utc),
    /* K3   */  be_nested_str_weak(expiration),
    }),
    be_str_weak(has_expired),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0003,  //  0002  JMPF	R2	#0007
      0xB80A0000,  //  0003  GETNGBL	R2	K0
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x94040502,  //  0006  GETIDX	R1	R2	K2
      0x88080103,  //  0007  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0008  LDNIL	R3
      0x20080403,  //  0009  NE	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x88080103,  //  000B  GETMBR	R2	R0	K3
      0x28080202,  //  000C  GE	R2	R1	R2
      0x80040400,  //  000D  RET	1	R2
      0x50080000,  //  000E  LDBOOL	R2	0	0
      0x80040400,  //  000F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_icac
********************************************************************/
be_local_closure(Matter_Session_get_icac,   /* name */
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
    /* K0   */  be_nested_str_weak(icac),
    }),
    be_str_weak(get_icac),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_deviceid
********************************************************************/
be_local_closure(Matter_Session_get_deviceid,   /* name */
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
    /* K0   */  be_nested_str_weak(deviceid),
    }),
    be_str_weak(get_deviceid),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_noc
********************************************************************/
be_local_closure(Matter_Session_get_noc,   /* name */
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
    /* K0   */  be_nested_str_weak(noc),
    }),
    be_str_weak(get_noc),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: close
********************************************************************/
be_local_closure(Matter_Session_close,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(_persist),
    /* K1   */  be_nested_str_weak(local_session_id),
    /* K2   */  be_nested_str_weak(_future_local_session_id),
    /* K3   */  be_nested_str_weak(initiator_session_id),
    /* K4   */  be_nested_str_weak(_future_initiator_session_id),
    /* K5   */  be_nested_str_weak(source_node_id),
    /* K6   */  be_nested_str_weak(counter_rcv),
    /* K7   */  be_nested_str_weak(reset),
    /* K8   */  be_nested_str_weak(counter_snd),
    /* K9   */  be_nested_str_weak(i2rkey),
    /* K10  */  be_nested_str_weak(_i2r_privacy),
    /* K11  */  be_nested_str_weak(r2ikey),
    /* K12  */  be_nested_str_weak(attestation_challenge),
    /* K13  */  be_nested_str_weak(fabric_label),
    /* K14  */  be_nested_str_weak(),
    /* K15  */  be_nested_str_weak(introspect),
    /* K16  */  be_nested_str_weak(members),
    /* K17  */  be_nested_str_weak(get),
    /* K18  */  be_nested_str_weak(function),
    /* K19  */  be_nested_str_weak(instance),
    /* K20  */  be_const_int(0),
    /* K21  */  be_nested_str_weak(_),
    /* K22  */  be_const_int(1),
    /* K23  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(close),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080102,  //  0001  GETMBR	R2	R0	K2
      0x90020202,  //  0002  SETMBR	R0	K1	R2
      0x88080104,  //  0003  GETMBR	R2	R0	K4
      0x90020602,  //  0004  SETMBR	R0	K3	R2
      0x4C080000,  //  0005  LDNIL	R2
      0x90020A02,  //  0006  SETMBR	R0	K5	R2
      0x88080106,  //  0007  GETMBR	R2	R0	K6
      0x8C080507,  //  0008  GETMET	R2	R2	K7
      0x7C080200,  //  0009  CALL	R2	1
      0x88080108,  //  000A  GETMBR	R2	R0	K8
      0x8C080507,  //  000B  GETMET	R2	R2	K7
      0x7C080200,  //  000C  CALL	R2	1
      0x4C080000,  //  000D  LDNIL	R2
      0x90021202,  //  000E  SETMBR	R0	K9	R2
      0x4C080000,  //  000F  LDNIL	R2
      0x90021402,  //  0010  SETMBR	R0	K10	R2
      0x4C080000,  //  0011  LDNIL	R2
      0x90021602,  //  0012  SETMBR	R0	K11	R2
      0x4C080000,  //  0013  LDNIL	R2
      0x90021802,  //  0014  SETMBR	R0	K12	R2
      0x90021B0E,  //  0015  SETMBR	R0	K13	K14
      0xA40A1E00,  //  0016  IMPORT	R2	K15
      0x600C0010,  //  0017  GETGBL	R3	G16
      0x8C100510,  //  0018  GETMET	R4	R2	K16
      0x5C180000,  //  0019  MOVE	R6	R0
      0x7C100400,  //  001A  CALL	R4	2
      0x7C0C0200,  //  001B  CALL	R3	1
      0xA8020018,  //  001C  EXBLK	0	#0036
      0x5C100600,  //  001D  MOVE	R4	R3
      0x7C100000,  //  001E  CALL	R4	0
      0x8C140511,  //  001F  GETMET	R5	R2	K17
      0x5C1C0000,  //  0020  MOVE	R7	R0
      0x5C200800,  //  0021  MOVE	R8	R4
      0x7C140600,  //  0022  CALL	R5	3
      0x60180004,  //  0023  GETGBL	R6	G4
      0x5C1C0A00,  //  0024  MOVE	R7	R5
      0x7C180200,  //  0025  CALL	R6	1
      0x20180D12,  //  0026  NE	R6	R6	K18
      0x781A000C,  //  0027  JMPF	R6	#0035
      0x60180004,  //  0028  GETGBL	R6	G4
      0x5C1C0A00,  //  0029  MOVE	R7	R5
      0x7C180200,  //  002A  CALL	R6	1
      0x20180D13,  //  002B  NE	R6	R6	K19
      0x781A0007,  //  002C  JMPF	R6	#0035
      0x94180914,  //  002D  GETIDX	R6	R4	K20
      0x1C180D15,  //  002E  EQ	R6	R6	K21
      0x781A0004,  //  002F  JMPF	R6	#0035
      0x94180916,  //  0030  GETIDX	R6	R4	K22
      0x20180D15,  //  0031  NE	R6	R6	K21
      0x781A0001,  //  0032  JMPF	R6	#0035
      0x4C180000,  //  0033  LDNIL	R6
      0x90000806,  //  0034  SETMBR	R0	R4	R6
      0x7001FFE6,  //  0035  JMP		#001D
      0x580C0017,  //  0036  LDCONST	R3	K23
      0xAC0C0200,  //  0037  CATCH	R3	1	0
      0xB0080000,  //  0038  RAISE	2	R0	R0
      0x90020001,  //  0039  SETMBR	R0	K0	R1
      0x80000000,  //  003A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fromjson
********************************************************************/
be_local_closure(Matter_Session_fromjson,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Session),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Session),
    /* K5   */  be_nested_str_weak(keys),
    /* K6   */  be_nested_str_weak(counter_rcv),
    /* K7   */  be_nested_str_weak(reset),
    /* K8   */  be_nested_str_weak(counter_snd),
    /* K9   */  be_nested_str_weak(find),
    /* K10  */  be_nested_str_weak(0x),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(set),
    /* K13  */  be_nested_str_weak(fromhex),
    /* K14  */  be_const_int(2),
    /* K15  */  be_const_int(2147483647),
    /* K16  */  be_nested_str_weak(_X24_X24),
    /* K17  */  be_nested_str_weak(fromb64),
    /* K18  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(fromjson),
    &be_const_str_solidified,
    ( &(const binstruction[88]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0xB8160600,  //  0003  GETNGBL	R5	K3
      0x8C140B04,  //  0004  GETMET	R5	R5	K4
      0x5C1C0000,  //  0005  MOVE	R7	R0
      0x7C140400,  //  0006  CALL	R5	2
      0x60180010,  //  0007  GETGBL	R6	G16
      0x8C1C0305,  //  0008  GETMET	R7	R1	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x7C180200,  //  000A  CALL	R6	1
      0xA8020047,  //  000B  EXBLK	0	#0054
      0x5C1C0C00,  //  000C  MOVE	R7	R6
      0x7C1C0000,  //  000D  CALL	R7	0
      0x94200207,  //  000E  GETIDX	R8	R1	R7
      0x1C240F06,  //  000F  EQ	R9	R7	K6
      0x78260006,  //  0010  JMPF	R9	#0018
      0x88240B06,  //  0011  GETMBR	R9	R5	K6
      0x8C241307,  //  0012  GETMET	R9	R9	K7
      0x602C0009,  //  0013  GETGBL	R11	G9
      0x5C301000,  //  0014  MOVE	R12	R8
      0x7C2C0200,  //  0015  CALL	R11	1
      0x7C240400,  //  0016  CALL	R9	2
      0x7002003A,  //  0017  JMP		#0053
      0x1C240F08,  //  0018  EQ	R9	R7	K8
      0x78260006,  //  0019  JMPF	R9	#0021
      0x88240B08,  //  001A  GETMBR	R9	R5	K8
      0x8C241307,  //  001B  GETMET	R9	R9	K7
      0x602C0009,  //  001C  GETGBL	R11	G9
      0x5C301000,  //  001D  MOVE	R12	R8
      0x7C2C0200,  //  001E  CALL	R11	1
      0x7C240400,  //  001F  CALL	R9	2
      0x70020031,  //  0020  JMP		#0053
      0x60240004,  //  0021  GETGBL	R9	G4
      0x5C281000,  //  0022  MOVE	R10	R8
      0x7C240200,  //  0023  CALL	R9	1
      0x1C241301,  //  0024  EQ	R9	R9	K1
      0x78260027,  //  0025  JMPF	R9	#004E
      0x8C240709,  //  0026  GETMET	R9	R3	K9
      0x5C2C1000,  //  0027  MOVE	R11	R8
      0x5830000A,  //  0028  LDCONST	R12	K10
      0x7C240600,  //  0029  CALL	R9	3
      0x1C24130B,  //  002A  EQ	R9	R9	K11
      0x7826000A,  //  002B  JMPF	R9	#0037
      0x8C24090C,  //  002C  GETMET	R9	R4	K12
      0x5C2C0A00,  //  002D  MOVE	R11	R5
      0x5C300E00,  //  002E  MOVE	R12	R7
      0x60340015,  //  002F  GETGBL	R13	G21
      0x7C340000,  //  0030  CALL	R13	0
      0x8C341B0D,  //  0031  GETMET	R13	R13	K13
      0x403E1D0F,  //  0032  CONNECT	R15	K14	K15
      0x943C100F,  //  0033  GETIDX	R15	R8	R15
      0x7C340400,  //  0034  CALL	R13	2
      0x7C240800,  //  0035  CALL	R9	4
      0x70020015,  //  0036  JMP		#004D
      0x8C240709,  //  0037  GETMET	R9	R3	K9
      0x5C2C1000,  //  0038  MOVE	R11	R8
      0x58300010,  //  0039  LDCONST	R12	K16
      0x7C240600,  //  003A  CALL	R9	3
      0x1C24130B,  //  003B  EQ	R9	R9	K11
      0x7826000A,  //  003C  JMPF	R9	#0048
      0x8C24090C,  //  003D  GETMET	R9	R4	K12
      0x5C2C0A00,  //  003E  MOVE	R11	R5
      0x5C300E00,  //  003F  MOVE	R12	R7
      0x60340015,  //  0040  GETGBL	R13	G21
      0x7C340000,  //  0041  CALL	R13	0
      0x8C341B11,  //  0042  GETMET	R13	R13	K17
      0x403E1D0F,  //  0043  CONNECT	R15	K14	K15
      0x943C100F,  //  0044  GETIDX	R15	R8	R15
      0x7C340400,  //  0045  CALL	R13	2
      0x7C240800,  //  0046  CALL	R9	4
      0x70020004,  //  0047  JMP		#004D
      0x8C24090C,  //  0048  GETMET	R9	R4	K12
      0x5C2C0A00,  //  0049  MOVE	R11	R5
      0x5C300E00,  //  004A  MOVE	R12	R7
      0x5C341000,  //  004B  MOVE	R13	R8
      0x7C240800,  //  004C  CALL	R9	4
      0x70020004,  //  004D  JMP		#0053
      0x8C24090C,  //  004E  GETMET	R9	R4	K12
      0x5C2C0A00,  //  004F  MOVE	R11	R5
      0x5C300E00,  //  0050  MOVE	R12	R7
      0x5C341000,  //  0051  MOVE	R13	R8
      0x7C240800,  //  0052  CALL	R9	4
      0x7001FFB7,  //  0053  JMP		#000C
      0x58180012,  //  0054  LDCONST	R6	K18
      0xAC180200,  //  0055  CATCH	R6	1	0
      0xB0080000,  //  0056  RAISE	2	R0	R0
      0x80040A00,  //  0057  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ipk_epoch_key
********************************************************************/
be_local_closure(Matter_Session_get_ipk_epoch_key,   /* name */
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
    /* K0   */  be_nested_str_weak(ipk_epoch_key),
    }),
    be_str_weak(get_ipk_epoch_key),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tojson
********************************************************************/
be_local_closure(Matter_Session_tojson,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(members),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_nested_str_weak(function),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(_),
    /* K8   */  be_nested_str_weak(push),
    /* K9   */  be_nested_str_weak(stop_iteration),
    /* K10  */  be_nested_str_weak(matter),
    /* K11  */  be_nested_str_weak(sort),
    /* K12  */  be_nested_str_weak(counter_rcv),
    /* K13  */  be_nested_str_weak(val),
    /* K14  */  be_nested_str_weak(counter_snd),
    /* K15  */  be_nested_str_weak(_X24_X24),
    /* K16  */  be_nested_str_weak(tob64),
    /* K17  */  be_nested_str_weak(format),
    /* K18  */  be_nested_str_weak(_X25s_X3A_X25s),
    /* K19  */  be_nested_str_weak(dump),
    /* K20  */  be_nested_str_weak(_X7B),
    /* K21  */  be_nested_str_weak(concat),
    /* K22  */  be_nested_str_weak(_X2C),
    /* K23  */  be_nested_str_weak(_X7D),
    }),
    be_str_weak(tojson),
    &be_const_str_solidified,
    ( &(const binstruction[98]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x60100012,  //  0003  GETGBL	R4	G18
      0x7C100000,  //  0004  CALL	R4	0
      0x60140010,  //  0005  GETGBL	R5	G16
      0x8C180703,  //  0006  GETMET	R6	R3	K3
      0x5C200000,  //  0007  MOVE	R8	R0
      0x7C180400,  //  0008  CALL	R6	2
      0x7C140200,  //  0009  CALL	R5	1
      0xA8020011,  //  000A  EXBLK	0	#001D
      0x5C180A00,  //  000B  MOVE	R6	R5
      0x7C180000,  //  000C  CALL	R6	0
      0x8C1C0704,  //  000D  GETMET	R7	R3	K4
      0x5C240000,  //  000E  MOVE	R9	R0
      0x5C280C00,  //  000F  MOVE	R10	R6
      0x7C1C0600,  //  0010  CALL	R7	3
      0x60200004,  //  0011  GETGBL	R8	G4
      0x5C240E00,  //  0012  MOVE	R9	R7
      0x7C200200,  //  0013  CALL	R8	1
      0x20201105,  //  0014  NE	R8	R8	K5
      0x78220005,  //  0015  JMPF	R8	#001C
      0x94200D06,  //  0016  GETIDX	R8	R6	K6
      0x20201107,  //  0017  NE	R8	R8	K7
      0x78220002,  //  0018  JMPF	R8	#001C
      0x8C200908,  //  0019  GETMET	R8	R4	K8
      0x5C280C00,  //  001A  MOVE	R10	R6
      0x7C200400,  //  001B  CALL	R8	2
      0x7001FFED,  //  001C  JMP		#000B
      0x58140009,  //  001D  LDCONST	R5	K9
      0xAC140200,  //  001E  CATCH	R5	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0xB8161400,  //  0020  GETNGBL	R5	K10
      0x8C140B0B,  //  0021  GETMET	R5	R5	K11
      0x5C1C0800,  //  0022  MOVE	R7	R4
      0x7C140400,  //  0023  CALL	R5	2
      0x5C100A00,  //  0024  MOVE	R4	R5
      0x60140012,  //  0025  GETGBL	R5	G18
      0x7C140000,  //  0026  CALL	R5	0
      0x60180010,  //  0027  GETGBL	R6	G16
      0x5C1C0800,  //  0028  MOVE	R7	R4
      0x7C180200,  //  0029  CALL	R6	1
      0xA802002D,  //  002A  EXBLK	0	#0059
      0x5C1C0C00,  //  002B  MOVE	R7	R6
      0x7C1C0000,  //  002C  CALL	R7	0
      0x8C200704,  //  002D  GETMET	R8	R3	K4
      0x5C280000,  //  002E  MOVE	R10	R0
      0x5C2C0E00,  //  002F  MOVE	R11	R7
      0x7C200600,  //  0030  CALL	R8	3
      0x4C240000,  //  0031  LDNIL	R9
      0x1C241009,  //  0032  EQ	R9	R8	R9
      0x78260000,  //  0033  JMPF	R9	#0035
      0x7001FFF5,  //  0034  JMP		#002B
      0x1C240F0C,  //  0035  EQ	R9	R7	K12
      0x78260003,  //  0036  JMPF	R9	#003B
      0x8C24110D,  //  0037  GETMET	R9	R8	K13
      0x7C240200,  //  0038  CALL	R9	1
      0x5C201200,  //  0039  MOVE	R8	R9
      0x70020006,  //  003A  JMP		#0042
      0x1C240F0E,  //  003B  EQ	R9	R7	K14
      0x78260004,  //  003C  JMPF	R9	#0042
      0x8C24110D,  //  003D  GETMET	R9	R8	K13
      0x7C240200,  //  003E  CALL	R9	1
      0x542A00FF,  //  003F  LDINT	R10	256
      0x0024120A,  //  0040  ADD	R9	R9	R10
      0x5C201200,  //  0041  MOVE	R8	R9
      0x6024000F,  //  0042  GETGBL	R9	G15
      0x5C281000,  //  0043  MOVE	R10	R8
      0x602C0015,  //  0044  GETGBL	R11	G21
      0x7C240400,  //  0045  CALL	R9	2
      0x78260003,  //  0046  JMPF	R9	#004B
      0x8C241110,  //  0047  GETMET	R9	R8	K16
      0x7C240200,  //  0048  CALL	R9	1
      0x00261E09,  //  0049  ADD	R9	K15	R9
      0x5C201200,  //  004A  MOVE	R8	R9
      0x8C240B08,  //  004B  GETMET	R9	R5	K8
      0x8C2C0511,  //  004C  GETMET	R11	R2	K17
      0x58340012,  //  004D  LDCONST	R13	K18
      0x8C380313,  //  004E  GETMET	R14	R1	K19
      0x60400008,  //  004F  GETGBL	R16	G8
      0x5C440E00,  //  0050  MOVE	R17	R7
      0x7C400200,  //  0051  CALL	R16	1
      0x7C380400,  //  0052  CALL	R14	2
      0x8C3C0313,  //  0053  GETMET	R15	R1	K19
      0x5C441000,  //  0054  MOVE	R17	R8
      0x7C3C0400,  //  0055  CALL	R15	2
      0x7C2C0800,  //  0056  CALL	R11	4
      0x7C240400,  //  0057  CALL	R9	2
      0x7001FFD1,  //  0058  JMP		#002B
      0x58180009,  //  0059  LDCONST	R6	K9
      0xAC180200,  //  005A  CATCH	R6	1	0
      0xB0080000,  //  005B  RAISE	2	R0	R0
      0x8C180B15,  //  005C  GETMET	R6	R5	K21
      0x58200016,  //  005D  LDCONST	R8	K22
      0x7C180400,  //  005E  CALL	R6	2
      0x001A2806,  //  005F  ADD	R6	K20	R6
      0x00180D17,  //  0060  ADD	R6	R6	K23
      0x80040C00,  //  0061  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ac
********************************************************************/
be_local_closure(Matter_Session_get_ac,   /* name */
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
    /* K0   */  be_nested_str_weak(attestation_challenge),
    }),
    be_str_weak(get_ac),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_no_expiration
********************************************************************/
be_local_closure(Matter_Session_set_no_expiration,   /* name */
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
    /* K0   */  be_nested_str_weak(expiration),
    }),
    be_str_weak(set_no_expiration),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_r2i
********************************************************************/
be_local_closure(Matter_Session_get_r2i,   /* name */
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
    /* K0   */  be_nested_str_weak(r2ikey),
    }),
    be_str_weak(get_r2i),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric_compressed
********************************************************************/
be_local_closure(Matter_Session_get_fabric_compressed,   /* name */
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
    /* K0   */  be_nested_str_weak(fabric_compressed),
    }),
    be_str_weak(get_fabric_compressed),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fabric_label
********************************************************************/
be_local_closure(Matter_Session_set_fabric_label,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(fabric_label),
    }),
    be_str_weak(set_fabric_label),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080500,  //  0003  EQ	R2	R2	K0
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x90020201,  //  0005  SETMBR	R0	K1	R1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ipk_epoch_key
********************************************************************/
be_local_closure(Matter_Session_set_ipk_epoch_key,   /* name */
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
    /* K0   */  be_nested_str_weak(ipk_epoch_key),
    }),
    be_str_weak(set_ipk_epoch_key),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fabric_device
********************************************************************/
be_local_closure(Matter_Session_set_fabric_device,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(fabric),
    /* K1   */  be_nested_str_weak(deviceid),
    /* K2   */  be_nested_str_weak(fabric_compressed),
    /* K3   */  be_nested_str_weak(__store),
    /* K4   */  be_nested_str_weak(remove_redundant_session),
    }),
    be_str_weak(set_fabric_device),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x8C100904,  //  0004  GETMET	R4	R4	K4
      0x5C180000,  //  0005  MOVE	R6	R0
      0x7C100400,  //  0006  CALL	R4	2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_persist
********************************************************************/
be_local_closure(Matter_Session_set_persist,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_persist),
    }),
    be_str_weak(set_persist),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gen_CSR
********************************************************************/
be_local_closure(Matter_Session_gen_CSR,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_pk),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(EC_P256),
    /* K3   */  be_nested_str_weak(public_key),
    /* K4   */  be_nested_str_weak(3070020100300E310C300A060355040A0C034353523059301306072A8648CE3D020106082A8648CE3D030107034200),
    /* K5   */  be_nested_str_weak(A000),
    /* K6   */  be_nested_str_weak(300C06082A8648CE3D0403020500),
    /* K7   */  be_nested_str_weak(ecdsa_sign_sha256_asn1),
    /* K8   */  be_nested_str_weak(add),
    /* K9   */  be_const_int(3),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(0),
    }),
    be_str_weak(gen_CSR),
    &be_const_str_solidified,
    ( &(const binstruction[73]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0xA40A0200,  //  0002  IMPORT	R2	K1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x7C0C0200,  //  0004  CALL	R3	1
      0x8C100703,  //  0005  GETMET	R4	R3	K3
      0x5C180200,  //  0006  MOVE	R6	R1
      0x7C100400,  //  0007  CALL	R4	2
      0x60140015,  //  0008  GETGBL	R5	G21
      0x58180004,  //  0009  LDCONST	R6	K4
      0x7C140200,  //  000A  CALL	R5	1
      0x40180A04,  //  000B  CONNECT	R6	R5	R4
      0x60180015,  //  000C  GETGBL	R6	G21
      0x581C0005,  //  000D  LDCONST	R7	K5
      0x7C180200,  //  000E  CALL	R6	1
      0x40180A06,  //  000F  CONNECT	R6	R5	R6
      0x60180015,  //  0010  GETGBL	R6	G21
      0x581C0006,  //  0011  LDCONST	R7	K6
      0x7C180200,  //  0012  CALL	R6	1
      0x8C1C0707,  //  0013  GETMET	R7	R3	K7
      0x5C240200,  //  0014  MOVE	R9	R1
      0x5C280A00,  //  0015  MOVE	R10	R5
      0x7C1C0600,  //  0016  CALL	R7	3
      0x60200015,  //  0017  GETGBL	R8	G21
      0x5426007F,  //  0018  LDINT	R9	128
      0x7C200200,  //  0019  CALL	R8	1
      0x8C241108,  //  001A  GETMET	R9	R8	K8
      0x582C0009,  //  001B  LDCONST	R11	K9
      0x5830000A,  //  001C  LDCONST	R12	K10
      0x7C240600,  //  001D  CALL	R9	3
      0x8C241108,  //  001E  GETMET	R9	R8	K8
      0x602C000C,  //  001F  GETGBL	R11	G12
      0x5C300E00,  //  0020  MOVE	R12	R7
      0x7C2C0200,  //  0021  CALL	R11	1
      0x002C170A,  //  0022  ADD	R11	R11	K10
      0x5830000A,  //  0023  LDCONST	R12	K10
      0x7C240600,  //  0024  CALL	R9	3
      0x8C241108,  //  0025  GETMET	R9	R8	K8
      0x582C000B,  //  0026  LDCONST	R11	K11
      0x5830000A,  //  0027  LDCONST	R12	K10
      0x7C240600,  //  0028  CALL	R9	3
      0x40241007,  //  0029  CONNECT	R9	R8	R7
      0x6024000C,  //  002A  GETGBL	R9	G12
      0x5C280A00,  //  002B  MOVE	R10	R5
      0x7C240200,  //  002C  CALL	R9	1
      0x6028000C,  //  002D  GETGBL	R10	G12
      0x5C2C0C00,  //  002E  MOVE	R11	R6
      0x7C280200,  //  002F  CALL	R10	1
      0x0024120A,  //  0030  ADD	R9	R9	R10
      0x6028000C,  //  0031  GETGBL	R10	G12
      0x5C2C1000,  //  0032  MOVE	R11	R8
      0x7C280200,  //  0033  CALL	R10	1
      0x0024120A,  //  0034  ADD	R9	R9	R10
      0x60280015,  //  0035  GETGBL	R10	G21
      0x542E00CF,  //  0036  LDINT	R11	208
      0x7C280200,  //  0037  CALL	R10	1
      0x8C2C1508,  //  0038  GETMET	R11	R10	K8
      0x5436002F,  //  0039  LDINT	R13	48
      0x5838000A,  //  003A  LDCONST	R14	K10
      0x7C2C0600,  //  003B  CALL	R11	3
      0x8C2C1508,  //  003C  GETMET	R11	R10	K8
      0x54360080,  //  003D  LDINT	R13	129
      0x5838000A,  //  003E  LDCONST	R14	K10
      0x7C2C0600,  //  003F  CALL	R11	3
      0x8C2C1508,  //  0040  GETMET	R11	R10	K8
      0x543600FE,  //  0041  LDINT	R13	255
      0x2C34120D,  //  0042  AND	R13	R9	R13
      0x5838000A,  //  0043  LDCONST	R14	K10
      0x7C2C0600,  //  0044  CALL	R11	3
      0x402C1405,  //  0045  CONNECT	R11	R10	R5
      0x402C1406,  //  0046  CONNECT	R11	R10	R6
      0x402C1408,  //  0047  CONNECT	R11	R10	R8
      0x80041400,  //  0048  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ca
********************************************************************/
be_local_closure(Matter_Session_set_ca,   /* name */
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
    /* K0   */  be_nested_str_weak(root_ca_certificate),
    }),
    be_str_weak(set_ca),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_expire_time
********************************************************************/
be_local_closure(Matter_Session_set_expire_time,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(expiration),
    }),
    be_str_weak(set_expire_time),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_i2r
********************************************************************/
be_local_closure(Matter_Session_get_i2r,   /* name */
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
    /* K0   */  be_nested_str_weak(i2rkey),
    }),
    be_str_weak(get_i2r),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_keys
********************************************************************/
be_local_closure(Matter_Session_set_keys,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(i2rkey),
    /* K1   */  be_nested_str_weak(_i2r_privacy),
    /* K2   */  be_nested_str_weak(r2ikey),
    /* K3   */  be_nested_str_weak(attestation_challenge),
    /* K4   */  be_nested_str_weak(session_timestamp),
    }),
    be_str_weak(set_keys),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x4C140000,  //  0001  LDNIL	R5
      0x90020205,  //  0002  SETMBR	R0	K1	R5
      0x90020402,  //  0003  SETMBR	R0	K2	R2
      0x90020603,  //  0004  SETMBR	R0	K3	R3
      0x90020804,  //  0005  SETMBR	R0	K4	R4
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ca
********************************************************************/
be_local_closure(Matter_Session_get_ca,   /* name */
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
    /* K0   */  be_nested_str_weak(root_ca_certificate),
    }),
    be_str_weak(get_ca),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pk
********************************************************************/
be_local_closure(Matter_Session_get_pk,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(no_private_key),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(random),
    }),
    be_str_weak(get_pk),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060004,  //  0001  JMPT	R1	#0007
      0xA4060200,  //  0002  IMPORT	R1	K1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x5412001F,  //  0004  LDINT	R4	32
      0x7C080400,  //  0005  CALL	R2	2
      0x90020002,  //  0006  SETMBR	R0	K0	R2
      0x88040100,  //  0007  GETMBR	R1	R0	K0
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ca_pub
********************************************************************/
be_local_closure(Matter_Session_get_ca_pub,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(root_ca_certificate),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(parse),
    /* K4   */  be_nested_str_weak(findsubval),
    }),
    be_str_weak(get_ca_pub),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060008,  //  0001  JMPF	R1	#000B
      0xB8060200,  //  0002  GETNGBL	R1	K1
      0x88040302,  //  0003  GETMBR	R1	R1	K2
      0x8C040303,  //  0004  GETMET	R1	R1	K3
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x7C040400,  //  0006  CALL	R1	2
      0x8C080304,  //  0007  GETMET	R2	R1	K4
      0x54120008,  //  0008  LDINT	R4	9
      0x7C080400,  //  0009  CALL	R2	2
      0x80040400,  //  000A  RET	1	R2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Session_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(__store),
    /* K1   */  be_nested_str_weak(mode),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_nested_str_weak(initiator_session_id),
    /* K5   */  be_nested_str_weak(counter_rcv),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(Counter),
    /* K8   */  be_nested_str_weak(counter_snd),
    /* K9   */  be_nested_str_weak(_counter_insecure_rcv),
    /* K10  */  be_nested_str_weak(_counter_insecure_snd),
    /* K11  */  be_nested_str_weak(breadcrumb),
    /* K12  */  be_nested_str_weak(int64),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020302,  //  0001  SETMBR	R0	K1	K2
      0x90020602,  //  0002  SETMBR	R0	K3	R2
      0x90020803,  //  0003  SETMBR	R0	K4	R3
      0xB8120C00,  //  0004  GETNGBL	R4	K6
      0x8C100907,  //  0005  GETMET	R4	R4	K7
      0x7C100200,  //  0006  CALL	R4	1
      0x90020A04,  //  0007  SETMBR	R0	K5	R4
      0xB8120C00,  //  0008  GETNGBL	R4	K6
      0x8C100907,  //  0009  GETMET	R4	R4	K7
      0x7C100200,  //  000A  CALL	R4	1
      0x90021004,  //  000B  SETMBR	R0	K8	R4
      0xB8120C00,  //  000C  GETNGBL	R4	K6
      0x8C100907,  //  000D  GETMET	R4	R4	K7
      0x7C100200,  //  000E  CALL	R4	1
      0x90021204,  //  000F  SETMBR	R0	K9	R4
      0xB8120C00,  //  0010  GETNGBL	R4	K6
      0x8C100907,  //  0011  GETMET	R4	R4	K7
      0x7C100200,  //  0012  CALL	R4	1
      0x90021404,  //  0013  SETMBR	R0	K10	R4
      0xB8121800,  //  0014  GETNGBL	R4	K12
      0x7C100000,  //  0015  CALL	R4	0
      0x90021604,  //  0016  SETMBR	R0	K11	R4
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ipk_group_key
********************************************************************/
be_local_closure(Matter_Session_get_ipk_group_key,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(ipk_epoch_key),
    /* K1   */  be_nested_str_weak(fabric_compressed),
    /* K2   */  be_nested_str_weak(crypto),
    /* K3   */  be_nested_str_weak(HKDF_SHA256),
    /* K4   */  be_nested_str_weak(fromstring),
    /* K5   */  be_nested_str_weak(__GROUP_KEY),
    /* K6   */  be_nested_str_weak(derive),
    }),
    be_str_weak(get_ipk_group_key),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x74060003,  //  0003  JMPT	R1	#0008
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x78060001,  //  0007  JMPF	R1	#000A
      0x4C040000,  //  0008  LDNIL	R1
      0x80040200,  //  0009  RET	1	R1
      0xA4060400,  //  000A  IMPORT	R1	K2
      0x8C080303,  //  000B  GETMET	R2	R1	K3
      0x7C080200,  //  000C  CALL	R2	1
      0x600C0015,  //  000D  GETGBL	R3	G21
      0x7C0C0000,  //  000E  CALL	R3	0
      0x8C0C0704,  //  000F  GETMET	R3	R3	K4
      0x88140105,  //  0010  GETMBR	R5	R0	K5
      0x7C0C0400,  //  0011  CALL	R3	2
      0x8C100506,  //  0012  GETMET	R4	R2	K6
      0x88180100,  //  0013  GETMBR	R6	R0	K0
      0x881C0101,  //  0014  GETMBR	R7	R0	K1
      0x5C200600,  //  0015  MOVE	R8	R3
      0x5426000F,  //  0016  LDINT	R9	16
      0x7C100A00,  //  0017  CALL	R4	5
      0x80040800,  //  0018  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_noc
********************************************************************/
be_local_closure(Matter_Session_set_noc,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(noc),
    /* K1   */  be_nested_str_weak(icac),
    }),
    be_str_weak(set_noc),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_fabric
********************************************************************/
be_local_closure(Matter_Session_get_fabric,   /* name */
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
    /* K0   */  be_nested_str_weak(fabric),
    }),
    be_str_weak(get_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_mode
********************************************************************/
be_local_closure(Matter_Session_get_mode,   /* name */
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
    /* K0   */  be_nested_str_weak(mode),
    }),
    be_str_weak(get_mode),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_mode
********************************************************************/
be_local_closure(Matter_Session_set_mode,   /* name */
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
    /* K0   */  be_nested_str_weak(mode),
    }),
    be_str_weak(set_mode),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Session
********************************************************************/
be_local_class(Matter_Session,
    36,
    NULL,
    be_nested_map(72,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(save, -1), be_const_closure(Matter_Session_save_closure) },
        { be_const_key_weak(set_mode, -1), be_const_closure(Matter_Session_set_mode_closure) },
        { be_const_key_weak(root_ca_certificate, -1), be_const_var(19) },
        { be_const_key_weak(admin_subject, 29), be_const_var(29) },
        { be_const_key_weak(get_i2r_privacy, 25), be_const_closure(Matter_Session_get_i2r_privacy_closure) },
        { be_const_key_weak(has_expired, -1), be_const_closure(Matter_Session_has_expired_closure) },
        { be_const_key_weak(set_no_expiration, -1), be_const_closure(Matter_Session_set_no_expiration_closure) },
        { be_const_key_weak(get_deviceid, 61), be_const_closure(Matter_Session_get_deviceid_closure) },
        { be_const_key_weak(local_session_id, -1), be_const_var(2) },
        { be_const_key_weak(get_mode, -1), be_const_closure(Matter_Session_get_mode_closure) },
        { be_const_key_weak(set_fabric_device, -1), be_const_closure(Matter_Session_set_fabric_device_closure) },
        { be_const_key_weak(fromjson, -1), be_const_static_closure(Matter_Session_fromjson_closure) },
        { be_const_key_weak(get_ipk_epoch_key, -1), be_const_closure(Matter_Session_get_ipk_epoch_key_closure) },
        { be_const_key_weak(get_fabric, 33), be_const_closure(Matter_Session_get_fabric_closure) },
        { be_const_key_weak(tojson, -1), be_const_closure(Matter_Session_tojson_closure) },
        { be_const_key_weak(set_noc, 37), be_const_closure(Matter_Session_set_noc_closure) },
        { be_const_key_weak(get_ac, -1), be_const_closure(Matter_Session_get_ac_closure) },
        { be_const_key_weak(_counter_insecure_snd, 6), be_const_var(11) },
        { be_const_key_weak(initiator_session_id, -1), be_const_var(3) },
        { be_const_key_weak(get_fabric_compressed, -1), be_const_closure(Matter_Session_get_fabric_compressed_closure) },
        { be_const_key_weak(set_fabric_label, -1), be_const_closure(Matter_Session_set_fabric_label_closure) },
        { be_const_key_weak(shared_secret, 8), be_const_var(24) },
        { be_const_key_weak(get_ipk_group_key, 43), be_const_closure(Matter_Session_get_ipk_group_key_closure) },
        { be_const_key_weak(init, 42), be_const_closure(Matter_Session_init_closure) },
        { be_const_key_weak(_chunked_attr_reports, -1), be_const_var(35) },
        { be_const_key_weak(set_persist, -1), be_const_closure(Matter_Session_set_persist_closure) },
        { be_const_key_weak(_future_initiator_session_id, -1), be_const_var(6) },
        { be_const_key_weak(_Msg2, -1), be_const_var(32) },
        { be_const_key_weak(fabric, -1), be_const_var(25) },
        { be_const_key_weak(close, -1), be_const_closure(Matter_Session_close_closure) },
        { be_const_key_weak(_i2r_privacy, -1), be_const_var(14) },
        { be_const_key_weak(ipk_epoch_key, -1), be_const_var(22) },
        { be_const_key_weak(get_ca, 10), be_const_closure(Matter_Session_get_ca_closure) },
        { be_const_key_weak(resumption_id, -1), be_const_var(23) },
        { be_const_key_weak(gen_CSR, 38), be_const_closure(Matter_Session_gen_CSR_closure) },
        { be_const_key_weak(source_node_id, -1), be_const_var(5) },
        { be_const_key_weak(set_ca, -1), be_const_closure(Matter_Session_set_ca_closure) },
        { be_const_key_weak(set_ipk_epoch_key, -1), be_const_closure(Matter_Session_set_ipk_epoch_key_closure) },
        { be_const_key_weak(_future_local_session_id, -1), be_const_var(7) },
        { be_const_key_weak(get_r2i, 31), be_const_closure(Matter_Session_get_r2i_closure) },
        { be_const_key_weak(icac, -1), be_const_var(21) },
        { be_const_key_weak(fabric_label, -1), be_const_var(28) },
        { be_const_key_weak(attestation_challenge, -1), be_const_var(15) },
        { be_const_key_weak(peer_node_id, 49), be_const_var(16) },
        { be_const_key_weak(get_noc, 53), be_const_closure(Matter_Session_get_noc_closure) },
        { be_const_key_weak(set_expire_time, 40), be_const_closure(Matter_Session_set_expire_time_closure) },
        { be_const_key_weak(deviceid, -1), be_const_var(27) },
        { be_const_key_weak(__CASE, -1), be_const_int(2) },
        { be_const_key_weak(get_icac, 50), be_const_closure(Matter_Session_get_icac_closure) },
        { be_const_key_weak(_Msg1, -1), be_const_var(31) },
        { be_const_key_weak(set_keys, 58), be_const_closure(Matter_Session_set_keys_closure) },
        { be_const_key_weak(noc, 23), be_const_var(20) },
        { be_const_key_weak(__PASE, -1), be_const_int(1) },
        { be_const_key_weak(breadcrumb, 26), be_const_var(17) },
        { be_const_key_weak(__GROUP_KEY, -1), be_nested_str_weak(GroupKey_X20v1_X2E0) },
        { be_const_key_weak(counter_snd, -1), be_const_var(9) },
        { be_const_key_weak(fabric_compressed, 32), be_const_var(26) },
        { be_const_key_weak(get_pk, -1), be_const_closure(Matter_Session_get_pk_closure) },
        { be_const_key_weak(expiration, -1), be_const_var(34) },
        { be_const_key_weak(r2ikey, -1), be_const_var(13) },
        { be_const_key_weak(get_ca_pub, -1), be_const_closure(Matter_Session_get_ca_pub_closure) },
        { be_const_key_weak(counter_rcv, -1), be_const_var(8) },
        { be_const_key_weak(_persist, 22), be_const_var(33) },
        { be_const_key_weak(i2rkey, 18), be_const_var(12) },
        { be_const_key_weak(session_timestamp, 15), be_const_var(4) },
        { be_const_key_weak(set_expire_in_seconds, 13), be_const_closure(Matter_Session_set_expire_in_seconds_closure) },
        { be_const_key_weak(mode, -1), be_const_var(1) },
        { be_const_key_weak(get_i2r, 9), be_const_closure(Matter_Session_get_i2r_closure) },
        { be_const_key_weak(__store, -1), be_const_var(0) },
        { be_const_key_weak(_counter_insecure_rcv, -1), be_const_var(10) },
        { be_const_key_weak(no_private_key, 2), be_const_var(18) },
        { be_const_key_weak(admin_vendor, 1), be_const_var(30) },
    })),
    be_str_weak(Matter_Session)
);
/*******************************************************************/

void be_load_Matter_Session_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Session);
    be_setglobal(vm, "Matter_Session");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Session_Store;

/********************************************************************
** Solidified function: remove_expired
********************************************************************/
be_local_closure(Matter_Session_Store_remove_expired,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(has_expired),
    /* K3   */  be_nested_str_weak(_persist),
    /* K4   */  be_nested_str_weak(remove),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(save),
    }),
    be_str_weak(remove_expired),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x58080000,  //  0001  LDCONST	R2	K0
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140101,  //  0004  GETMBR	R5	R0	K1
      0x7C100200,  //  0005  CALL	R4	1
      0x14100404,  //  0006  LT	R4	R2	R4
      0x7812000D,  //  0007  JMPF	R4	#0016
      0x94100602,  //  0008  GETIDX	R4	R3	R2
      0x8C100902,  //  0009  GETMET	R4	R4	K2
      0x7C100200,  //  000A  CALL	R4	1
      0x78120007,  //  000B  JMPF	R4	#0014
      0x94100602,  //  000C  GETIDX	R4	R3	R2
      0x88100903,  //  000D  GETMBR	R4	R4	K3
      0x78120000,  //  000E  JMPF	R4	#0010
      0x50040200,  //  000F  LDBOOL	R1	1	0
      0x8C100704,  //  0010  GETMET	R4	R3	K4
      0x5C180400,  //  0011  MOVE	R6	R2
      0x7C100400,  //  0012  CALL	R4	2
      0x70020000,  //  0013  JMP		#0015
      0x00080505,  //  0014  ADD	R2	R2	K5
      0x7001FFEC,  //  0015  JMP		#0003
      0x78060001,  //  0016  JMPF	R1	#0019
      0x8C100106,  //  0017  GETMET	R4	R0	K6
      0x7C100200,  //  0018  CALL	R4	1
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_redundant_session
********************************************************************/
be_local_closure(Matter_Session_Store_remove_redundant_session,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(fabric),
    /* K3   */  be_nested_str_weak(deviceid),
    /* K4   */  be_nested_str_weak(remove),
    /* K5   */  be_const_int(1),
    }),
    be_str_weak(remove_redundant_session),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x6010000C,  //  0002  GETGBL	R4	G12
      0x88140101,  //  0003  GETMBR	R5	R0	K1
      0x7C100200,  //  0004  CALL	R4	1
      0x14100404,  //  0005  LT	R4	R2	R4
      0x78120010,  //  0006  JMPF	R4	#0018
      0x94100602,  //  0007  GETIDX	R4	R3	R2
      0x20140801,  //  0008  NE	R5	R4	R1
      0x7816000B,  //  0009  JMPF	R5	#0016
      0x88140902,  //  000A  GETMBR	R5	R4	K2
      0x88180302,  //  000B  GETMBR	R6	R1	K2
      0x1C140A06,  //  000C  EQ	R5	R5	R6
      0x78160007,  //  000D  JMPF	R5	#0016
      0x88140903,  //  000E  GETMBR	R5	R4	K3
      0x88180303,  //  000F  GETMBR	R6	R1	K3
      0x1C140A06,  //  0010  EQ	R5	R5	R6
      0x78160003,  //  0011  JMPF	R5	#0016
      0x8C140704,  //  0012  GETMET	R5	R3	K4
      0x5C1C0400,  //  0013  MOVE	R7	R2
      0x7C140400,  //  0014  CALL	R5	2
      0x70020000,  //  0015  JMP		#0017
      0x00080505,  //  0016  ADD	R2	R2	K5
      0x7001FFE9,  //  0017  JMP		#0002
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_session
********************************************************************/
be_local_closure(Matter_Session_Store_add_session,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_expire_in_seconds),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_session),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0403,  //  0001  NE	R3	R2	R3
      0x780E0002,  //  0002  JMPF	R3	#0006
      0x8C0C0300,  //  0003  GETMET	R3	R1	K0
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0007  GETMET	R3	R3	K2
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sessions_active
********************************************************************/
be_local_closure(Matter_Session_Store_sessions_active,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(get_deviceid),
    /* K3   */  be_nested_str_weak(get_fabric),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_const_int(1),
    }),
    be_str_weak(sessions_active),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x58080000,  //  0002  LDCONST	R2	K0
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x88100101,  //  0004  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x140C0403,  //  0006  LT	R3	R2	R3
      0x780E000C,  //  0007  JMPF	R3	#0015
      0x880C0101,  //  0008  GETMBR	R3	R0	K1
      0x940C0602,  //  0009  GETIDX	R3	R3	R2
      0x8C100702,  //  000A  GETMET	R4	R3	K2
      0x7C100200,  //  000B  CALL	R4	1
      0x78120005,  //  000C  JMPF	R4	#0013
      0x8C100703,  //  000D  GETMET	R4	R3	K3
      0x7C100200,  //  000E  CALL	R4	1
      0x78120002,  //  000F  JMPF	R4	#0013
      0x8C100304,  //  0010  GETMET	R4	R1	K4
      0x5C180600,  //  0011  MOVE	R6	R3
      0x7C100400,  //  0012  CALL	R4	2
      0x00080505,  //  0013  ADD	R2	R2	K5
      0x7001FFED,  //  0014  JMP		#0003
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Session_Store_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(remove_expired),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(Matter_Session_Store_save,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(remove_expired),
    /* K2   */  be_nested_str_weak(sessions),
    /* K3   */  be_nested_str_weak(_persist),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(tojson),
    /* K6   */  be_nested_str_weak(stop_iteration),
    /* K7   */  be_nested_str_weak(_X5B),
    /* K8   */  be_nested_str_weak(concat),
    /* K9   */  be_nested_str_weak(_X2C),
    /* K10  */  be_nested_str_weak(_X5D),
    /* K11  */  be_nested_str_weak(string),
    /* K12  */  be_nested_str_weak(FILENAME),
    /* K13  */  be_nested_str_weak(w),
    /* K14  */  be_nested_str_weak(write),
    /* K15  */  be_nested_str_weak(close),
    /* K16  */  be_nested_str_weak(tasmota),
    /* K17  */  be_nested_str_weak(log),
    /* K18  */  be_nested_str_weak(format),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20Saved_X20_X25i_X20session_X28s_X29),
    /* K20  */  be_const_int(2),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K22  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(save),
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x60080012,  //  0003  GETGBL	R2	G18
      0x7C080000,  //  0004  CALL	R2	0
      0x600C0010,  //  0005  GETGBL	R3	G16
      0x88100102,  //  0006  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020008,  //  0008  EXBLK	0	#0012
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0x88140903,  //  000B  GETMBR	R5	R4	K3
      0x78160003,  //  000C  JMPF	R5	#0011
      0x8C140504,  //  000D  GETMET	R5	R2	K4
      0x8C1C0905,  //  000E  GETMET	R7	R4	K5
      0x7C1C0200,  //  000F  CALL	R7	1
      0x7C140400,  //  0010  CALL	R5	2
      0x7001FFF6,  //  0011  JMP		#0009
      0x580C0006,  //  0012  LDCONST	R3	K6
      0xAC0C0200,  //  0013  CATCH	R3	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x600C000C,  //  0015  GETGBL	R3	G12
      0x5C100400,  //  0016  MOVE	R4	R2
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C100508,  //  0018  GETMET	R4	R2	K8
      0x58180009,  //  0019  LDCONST	R6	K9
      0x7C100400,  //  001A  CALL	R4	2
      0x00120E04,  //  001B  ADD	R4	K7	R4
      0x0010090A,  //  001C  ADD	R4	R4	K10
      0x5C080800,  //  001D  MOVE	R2	R4
      0xA8020015,  //  001E  EXBLK	0	#0035
      0xA4121600,  //  001F  IMPORT	R4	K11
      0x60140011,  //  0020  GETGBL	R5	G17
      0x8818010C,  //  0021  GETMBR	R6	R0	K12
      0x581C000D,  //  0022  LDCONST	R7	K13
      0x7C140400,  //  0023  CALL	R5	2
      0x8C180B0E,  //  0024  GETMET	R6	R5	K14
      0x5C200400,  //  0025  MOVE	R8	R2
      0x7C180400,  //  0026  CALL	R6	2
      0x8C180B0F,  //  0027  GETMET	R6	R5	K15
      0x7C180200,  //  0028  CALL	R6	1
      0xB81A2000,  //  0029  GETNGBL	R6	K16
      0x8C180D11,  //  002A  GETMET	R6	R6	K17
      0x8C200912,  //  002B  GETMET	R8	R4	K18
      0x58280013,  //  002C  LDCONST	R10	K19
      0x5C2C0600,  //  002D  MOVE	R11	R3
      0x7C200600,  //  002E  CALL	R8	3
      0x58240014,  //  002F  LDCONST	R9	K20
      0x7C180600,  //  0030  CALL	R6	3
      0xA8040001,  //  0031  EXBLK	1	1
      0x80040400,  //  0032  RET	1	R2
      0xA8040001,  //  0033  EXBLK	1	1
      0x70020011,  //  0034  JMP		#0047
      0xAC100002,  //  0035  CATCH	R4	0	2
      0x7002000E,  //  0036  JMP		#0046
      0xB81A2000,  //  0037  GETNGBL	R6	K16
      0x8C180D11,  //  0038  GETMET	R6	R6	K17
      0x60200008,  //  0039  GETGBL	R8	G8
      0x5C240800,  //  003A  MOVE	R9	R4
      0x7C200200,  //  003B  CALL	R8	1
      0x00222A08,  //  003C  ADD	R8	K21	R8
      0x00201116,  //  003D  ADD	R8	R8	K22
      0x60240008,  //  003E  GETGBL	R9	G8
      0x5C280A00,  //  003F  MOVE	R10	R5
      0x7C240200,  //  0040  CALL	R9	1
      0x00201009,  //  0041  ADD	R8	R8	R9
      0x58240014,  //  0042  LDCONST	R9	K20
      0x7C180600,  //  0043  CALL	R6	3
      0x80040400,  //  0044  RET	1	R2
      0x70020000,  //  0045  JMP		#0047
      0xB0080000,  //  0046  RAISE	2	R0	R0
      0x80000000,  //  0047  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_session_by_resumption_id
********************************************************************/
be_local_closure(Matter_Session_Store_find_session_by_resumption_id,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(resumption_id),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(find_session_by_resumption_id),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x5C080200,  //  0000  MOVE	R2	R1
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x4C080000,  //  0002  LDNIL	R2
      0x80040400,  //  0003  RET	1	R2
      0x58080000,  //  0004  LDCONST	R2	K0
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x6010000C,  //  0006  GETGBL	R4	G12
      0x5C140600,  //  0007  MOVE	R5	R3
      0x7C100200,  //  0008  CALL	R4	1
      0x14100404,  //  0009  LT	R4	R2	R4
      0x78120007,  //  000A  JMPF	R4	#0013
      0x94100602,  //  000B  GETIDX	R4	R3	R2
      0x88100902,  //  000C  GETMBR	R4	R4	K2
      0x1C100801,  //  000D  EQ	R4	R4	R1
      0x78120001,  //  000E  JMPF	R4	#0011
      0x94100602,  //  000F  GETIDX	R4	R3	R2
      0x80040800,  //  0010  RET	1	R4
      0x00080503,  //  0011  ADD	R2	R2	K3
      0x7001FFF2,  //  0012  JMP		#0006
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gen_local_session_id
********************************************************************/
be_local_closure(Matter_Session_Store_gen_local_session_id,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(random),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(get_session_by_local_session_id),
    }),
    be_str_weak(gen_local_session_id),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x50080200,  //  0001  LDBOOL	R2	1	0
      0x780A000E,  //  0002  JMPF	R2	#0012
      0x8C080301,  //  0003  GETMET	R2	R1	K1
      0x58100002,  //  0004  LDCONST	R4	K2
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080503,  //  0006  GETMET	R2	R2	K3
      0x58100004,  //  0007  LDCONST	R4	K4
      0x58140002,  //  0008  LDCONST	R5	K2
      0x7C080600,  //  0009  CALL	R2	3
      0x8C0C0105,  //  000A  GETMET	R3	R0	K5
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x4C100000,  //  000D  LDNIL	R4
      0x1C0C0604,  //  000E  EQ	R3	R3	R4
      0x780E0000,  //  000F  JMPF	R3	#0011
      0x80040400,  //  0010  RET	1	R2
      0x7001FFEE,  //  0011  JMP		#0001
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_session_by_local_session_id
********************************************************************/
be_local_closure(Matter_Session_Store_get_session_by_local_session_id,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(get_session_by_local_session_id),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x7C080200,  //  0007  CALL	R2	1
      0x580C0001,  //  0008  LDCONST	R3	K1
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x14140602,  //  000A  LT	R5	R3	R2
      0x78160007,  //  000B  JMPF	R5	#0014
      0x94140803,  //  000C  GETIDX	R5	R4	R3
      0x88140B02,  //  000D  GETMBR	R5	R5	K2
      0x1C140A01,  //  000E  EQ	R5	R5	R1
      0x78160001,  //  000F  JMPF	R5	#0012
      0x94140803,  //  0010  GETIDX	R5	R4	R3
      0x80040A00,  //  0011  RET	1	R5
      0x000C0703,  //  0012  ADD	R3	R3	K3
      0x7001FFF5,  //  0013  JMP		#000A
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Session_Store_init,   /* name */
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
    /* K0   */  be_nested_str_weak(sessions),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_session_source_id_unsecure
********************************************************************/
be_local_closure(Matter_Session_Store_find_session_source_id_unsecure,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_session_by_source_node_id),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Session),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(source_node_id),
    /* K5   */  be_nested_str_weak(sessions),
    /* K6   */  be_nested_str_weak(push),
    /* K7   */  be_nested_str_weak(set_expire_in_seconds),
    }),
    be_str_weak(find_session_source_id_unsecure),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100604,  //  0004  EQ	R4	R3	R4
      0x7812000B,  //  0005  JMPF	R4	#0012
      0xB8120200,  //  0006  GETNGBL	R4	K1
      0x8C100902,  //  0007  GETMET	R4	R4	K2
      0x5C180000,  //  0008  MOVE	R6	R0
      0x581C0003,  //  0009  LDCONST	R7	K3
      0x58200003,  //  000A  LDCONST	R8	K3
      0x7C100800,  //  000B  CALL	R4	4
      0x5C0C0800,  //  000C  MOVE	R3	R4
      0x900E0801,  //  000D  SETMBR	R3	K4	R1
      0x88100105,  //  000E  GETMBR	R4	R0	K5
      0x8C100906,  //  000F  GETMET	R4	R4	K6
      0x5C180600,  //  0010  MOVE	R6	R3
      0x7C100400,  //  0011  CALL	R4	2
      0x8C100707,  //  0012  GETMET	R4	R3	K7
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C100400,  //  0014  CALL	R4	2
      0x80040600,  //  0015  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_session
********************************************************************/
be_local_closure(Matter_Session_Store_remove_session,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(remove),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(remove_session),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x6010000C,  //  0002  GETGBL	R4	G12
      0x88140101,  //  0003  GETMBR	R5	R0	K1
      0x7C100200,  //  0004  CALL	R4	1
      0x14100404,  //  0005  LT	R4	R2	R4
      0x78120008,  //  0006  JMPF	R4	#0010
      0x94100602,  //  0007  GETIDX	R4	R3	R2
      0x1C100801,  //  0008  EQ	R4	R4	R1
      0x78120003,  //  0009  JMPF	R4	#000E
      0x8C100702,  //  000A  GETMET	R4	R3	K2
      0x5C180400,  //  000B  MOVE	R6	R2
      0x7C100400,  //  000C  CALL	R4	2
      0x70020000,  //  000D  JMP		#000F
      0x00080503,  //  000E  ADD	R2	R2	K3
      0x7001FFF1,  //  000F  JMP		#0002
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_session
********************************************************************/
be_local_closure(Matter_Session_Store_create_session,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_session_by_local_session_id),
    /* K1   */  be_nested_str_weak(remove_session),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Session),
    /* K4   */  be_nested_str_weak(sessions),
    /* K5   */  be_nested_str_weak(push),
    }),
    be_str_weak(create_session),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x20100604,  //  0004  NE	R4	R3	R4
      0x78120002,  //  0005  JMPF	R4	#0009
      0x8C100101,  //  0006  GETMET	R4	R0	K1
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0xB8120400,  //  0009  GETNGBL	R4	K2
      0x8C100903,  //  000A  GETMET	R4	R4	K3
      0x5C180000,  //  000B  MOVE	R6	R0
      0x5C1C0200,  //  000C  MOVE	R7	R1
      0x5C200400,  //  000D  MOVE	R8	R2
      0x7C100800,  //  000E  CALL	R4	4
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x88100104,  //  0010  GETMBR	R4	R0	K4
      0x8C100905,  //  0011  GETMET	R4	R4	K5
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C100400,  //  0013  CALL	R4	2
      0x80040600,  //  0014  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(Matter_Session_Store_load,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(sessions),
    /* K2   */  be_nested_str_weak(FILENAME),
    /* K3   */  be_nested_str_weak(read),
    /* K4   */  be_nested_str_weak(close),
    /* K5   */  be_nested_str_weak(json),
    /* K6   */  be_nested_str_weak(load),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(gc),
    /* K9   */  be_nested_str_weak(matter),
    /* K10  */  be_nested_str_weak(Session),
    /* K11  */  be_nested_str_weak(fromjson),
    /* K12  */  be_nested_str_weak(add_session),
    /* K13  */  be_nested_str_weak(stop_iteration),
    /* K14  */  be_nested_str_weak(log),
    /* K15  */  be_nested_str_weak(format),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20Loaded_X20_X25i_X20session_X28s_X29),
    /* K17  */  be_const_int(2),
    /* K18  */  be_nested_str_weak(io_error),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
    /* K20  */  be_nested_str_weak(_X7C),
    /* K21  */  be_nested_str_weak(remove_expired),
    }),
    be_str_weak(load),
    &be_const_str_solidified,
    ( &(const binstruction[76]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA8020033,  //  0001  EXBLK	0	#0036
      0x60080012,  //  0002  GETGBL	R2	G18
      0x7C080000,  //  0003  CALL	R2	0
      0x90020202,  //  0004  SETMBR	R0	K1	R2
      0x60080011,  //  0005  GETGBL	R2	G17
      0x880C0102,  //  0006  GETMBR	R3	R0	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C100504,  //  000A  GETMET	R4	R2	K4
      0x7C100200,  //  000B  CALL	R4	1
      0xA4120A00,  //  000C  IMPORT	R4	K5
      0x8C140906,  //  000D  GETMET	R5	R4	K6
      0x5C1C0600,  //  000E  MOVE	R7	R3
      0x7C140400,  //  000F  CALL	R5	2
      0x4C0C0000,  //  0010  LDNIL	R3
      0xB81A0E00,  //  0011  GETNGBL	R6	K7
      0x8C180D08,  //  0012  GETMET	R6	R6	K8
      0x7C180200,  //  0013  CALL	R6	1
      0x60180010,  //  0014  GETGBL	R6	G16
      0x5C1C0A00,  //  0015  MOVE	R7	R5
      0x7C180200,  //  0016  CALL	R6	1
      0xA802000E,  //  0017  EXBLK	0	#0027
      0x5C1C0C00,  //  0018  MOVE	R7	R6
      0x7C1C0000,  //  0019  CALL	R7	0
      0xB8221200,  //  001A  GETNGBL	R8	K9
      0x8820110A,  //  001B  GETMBR	R8	R8	K10
      0x8C20110B,  //  001C  GETMET	R8	R8	K11
      0x5C280000,  //  001D  MOVE	R10	R0
      0x5C2C0E00,  //  001E  MOVE	R11	R7
      0x7C200600,  //  001F  CALL	R8	3
      0x4C240000,  //  0020  LDNIL	R9
      0x20241009,  //  0021  NE	R9	R8	R9
      0x78260002,  //  0022  JMPF	R9	#0026
      0x8C24010C,  //  0023  GETMET	R9	R0	K12
      0x5C2C1000,  //  0024  MOVE	R11	R8
      0x7C240400,  //  0025  CALL	R9	2
      0x7001FFF0,  //  0026  JMP		#0018
      0x5818000D,  //  0027  LDCONST	R6	K13
      0xAC180200,  //  0028  CATCH	R6	1	0
      0xB0080000,  //  0029  RAISE	2	R0	R0
      0xB81A0E00,  //  002A  GETNGBL	R6	K7
      0x8C180D0E,  //  002B  GETMET	R6	R6	K14
      0x8C20030F,  //  002C  GETMET	R8	R1	K15
      0x58280010,  //  002D  LDCONST	R10	K16
      0x602C000C,  //  002E  GETGBL	R11	G12
      0x88300101,  //  002F  GETMBR	R12	R0	K1
      0x7C2C0200,  //  0030  CALL	R11	1
      0x7C200600,  //  0031  CALL	R8	3
      0x58240011,  //  0032  LDCONST	R9	K17
      0x7C180600,  //  0033  CALL	R6	3
      0xA8040001,  //  0034  EXBLK	1	1
      0x70020012,  //  0035  JMP		#0049
      0xAC080002,  //  0036  CATCH	R2	0	2
      0x7002000F,  //  0037  JMP		#0048
      0x20100512,  //  0038  NE	R4	R2	K18
      0x7812000C,  //  0039  JMPF	R4	#0047
      0xB8120E00,  //  003A  GETNGBL	R4	K7
      0x8C10090E,  //  003B  GETMET	R4	R4	K14
      0x60180008,  //  003C  GETGBL	R6	G8
      0x5C1C0400,  //  003D  MOVE	R7	R2
      0x7C180200,  //  003E  CALL	R6	1
      0x001A2606,  //  003F  ADD	R6	K19	R6
      0x00180D14,  //  0040  ADD	R6	R6	K20
      0x601C0008,  //  0041  GETGBL	R7	G8
      0x5C200600,  //  0042  MOVE	R8	R3
      0x7C1C0200,  //  0043  CALL	R7	1
      0x00180C07,  //  0044  ADD	R6	R6	R7
      0x581C0011,  //  0045  LDCONST	R7	K17
      0x7C100600,  //  0046  CALL	R4	3
      0x70020000,  //  0047  JMP		#0049
      0xB0080000,  //  0048  RAISE	2	R0	R0
      0x8C080115,  //  0049  GETMET	R2	R0	K21
      0x7C080200,  //  004A  CALL	R2	1
      0x80000000,  //  004B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_session_by_source_node_id
********************************************************************/
be_local_closure(Matter_Session_Store_get_session_by_source_node_id,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(source_node_id),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(get_session_by_source_node_id),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x7C080200,  //  0007  CALL	R2	1
      0x580C0001,  //  0008  LDCONST	R3	K1
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x14140602,  //  000A  LT	R5	R3	R2
      0x78160007,  //  000B  JMPF	R5	#0014
      0x94140803,  //  000C  GETIDX	R5	R4	R3
      0x88140B02,  //  000D  GETMBR	R5	R5	K2
      0x1C140A01,  //  000E  EQ	R5	R5	R1
      0x78160001,  //  000F  JMPF	R5	#0012
      0x94140803,  //  0010  GETIDX	R5	R4	R3
      0x80040A00,  //  0011  RET	1	R5
      0x000C0703,  //  0012  ADD	R3	R3	K3
      0x7001FFF5,  //  0013  JMP		#000A
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Session_Store
********************************************************************/
be_local_class(Matter_Session_Store,
    1,
    NULL,
    be_nested_map(17,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(remove_expired, 6), be_const_closure(Matter_Session_Store_remove_expired_closure) },
        { be_const_key_weak(remove_redundant_session, -1), be_const_closure(Matter_Session_Store_remove_redundant_session_closure) },
        { be_const_key_weak(add_session, -1), be_const_closure(Matter_Session_Store_add_session_closure) },
        { be_const_key_weak(sessions, 12), be_const_var(0) },
        { be_const_key_weak(sessions_active, -1), be_const_closure(Matter_Session_Store_sessions_active_closure) },
        { be_const_key_weak(every_second, 9), be_const_closure(Matter_Session_Store_every_second_closure) },
        { be_const_key_weak(find_session_by_resumption_id, -1), be_const_closure(Matter_Session_Store_find_session_by_resumption_id_closure) },
        { be_const_key_weak(load, -1), be_const_closure(Matter_Session_Store_load_closure) },
        { be_const_key_weak(gen_local_session_id, -1), be_const_closure(Matter_Session_Store_gen_local_session_id_closure) },
        { be_const_key_weak(get_session_by_local_session_id, -1), be_const_closure(Matter_Session_Store_get_session_by_local_session_id_closure) },
        { be_const_key_weak(FILENAME, -1), be_nested_str_weak(_matter_sessions_X2Ejson) },
        { be_const_key_weak(save, 10), be_const_closure(Matter_Session_Store_save_closure) },
        { be_const_key_weak(find_session_source_id_unsecure, -1), be_const_closure(Matter_Session_Store_find_session_source_id_unsecure_closure) },
        { be_const_key_weak(remove_session, -1), be_const_closure(Matter_Session_Store_remove_session_closure) },
        { be_const_key_weak(create_session, -1), be_const_closure(Matter_Session_Store_create_session_closure) },
        { be_const_key_weak(init, 7), be_const_closure(Matter_Session_Store_init_closure) },
        { be_const_key_weak(get_session_by_source_node_id, -1), be_const_closure(Matter_Session_Store_get_session_by_source_node_id_closure) },
    })),
    be_str_weak(Matter_Session_Store)
);
/*******************************************************************/

void be_load_Matter_Session_Store_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Session_Store);
    be_setglobal(vm, "Matter_Session_Store");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
