/* Solidification of zigbee_zcl_attribute.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_zcl_attribute;

/********************************************************************
** Solidified function: tomap
********************************************************************/
be_local_closure(zcl_attribute_tomap,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(tomap),
    /* K1   */  be_nested_str_weak(cluster),
    /* K2   */  be_nested_str_weak(attr_id),
    /* K3   */  be_nested_str_weak(cmd),
    /* K4   */  be_nested_str_weak(direction),
    /* K5   */  be_nested_str_weak(key),
    /* K6   */  be_nested_str_weak(val),
    }),
    be_str_weak(tomap),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x88080101,  //  0005  GETMBR	R2	R0	K1
      0x98060202,  //  0006  SETIDX	R1	K1	R2
      0x88080102,  //  0007  GETMBR	R2	R0	K2
      0x98060402,  //  0008  SETIDX	R1	K2	R2
      0x88080103,  //  0009  GETMBR	R2	R0	K3
      0x98060602,  //  000A  SETIDX	R1	K3	R2
      0x88080104,  //  000B  GETMBR	R2	R0	K4
      0x98060802,  //  000C  SETIDX	R1	K4	R2
      0x88080105,  //  000D  GETMBR	R2	R0	K5
      0x98060A02,  //  000E  SETIDX	R1	K5	R2
      0x88080106,  //  000F  GETMBR	R2	R0	K6
      0x98060C02,  //  0010  SETIDX	R1	K6	R2
      0x80040200,  //  0011  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: key_tostring
********************************************************************/
be_local_closure(zcl_attribute_key_tostring,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(_X3Cundefined_X3E),
    /* K2   */  be_nested_str_weak(key),
    /* K3   */  be_nested_str_weak(key_suffix),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(cluster),
    /* K6   */  be_nested_str_weak(attr_id),
    /* K7   */  be_nested_str_weak(_X2504X_X2F_X2504X),
    /* K8   */  be_nested_str_weak(_X2B),
    /* K9   */  be_nested_str_weak(cmd),
    /* K10  */  be_nested_str_weak(direction),
    /* K11  */  be_nested_str_weak(_X2504X_X25s_X2502X),
    /* K12  */  be_nested_str_weak(_X3F),
    /* K13  */  be_nested_str_weak(_X21),
    }),
    be_str_weak(key_tostring),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x58080001,  //  0001  LDCONST	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x4C100000,  //  0003  LDNIL	R4
      0x20100604,  //  0004  NE	R4	R3	R4
      0x78120008,  //  0005  JMPF	R4	#000F
      0x5C080600,  //  0006  MOVE	R2	R3
      0x88100103,  //  0007  GETMBR	R4	R0	K3
      0x24100904,  //  0008  GT	R4	R4	K4
      0x78120003,  //  0009  JMPF	R4	#000E
      0x60100008,  //  000A  GETGBL	R4	G8
      0x88140103,  //  000B  GETMBR	R5	R0	K3
      0x7C100200,  //  000C  CALL	R4	1
      0x00080404,  //  000D  ADD	R2	R2	R4
      0x70020035,  //  000E  JMP		#0045
      0x88100105,  //  000F  GETMBR	R4	R0	K5
      0x4C140000,  //  0010  LDNIL	R5
      0x20100805,  //  0011  NE	R4	R4	R5
      0x78120012,  //  0012  JMPF	R4	#0026
      0x88100106,  //  0013  GETMBR	R4	R0	K6
      0x4C140000,  //  0014  LDNIL	R5
      0x20100805,  //  0015  NE	R4	R4	R5
      0x7812000E,  //  0016  JMPF	R4	#0026
      0x60100018,  //  0017  GETGBL	R4	G24
      0x58140007,  //  0018  LDCONST	R5	K7
      0x88180105,  //  0019  GETMBR	R6	R0	K5
      0x881C0106,  //  001A  GETMBR	R7	R0	K6
      0x7C100600,  //  001B  CALL	R4	3
      0x5C080800,  //  001C  MOVE	R2	R4
      0x88100103,  //  001D  GETMBR	R4	R0	K3
      0x24100904,  //  001E  GT	R4	R4	K4
      0x78120004,  //  001F  JMPF	R4	#0025
      0x60100008,  //  0020  GETGBL	R4	G8
      0x88140103,  //  0021  GETMBR	R5	R0	K3
      0x7C100200,  //  0022  CALL	R4	1
      0x00121004,  //  0023  ADD	R4	K8	R4
      0x00080404,  //  0024  ADD	R2	R2	R4
      0x7002001E,  //  0025  JMP		#0045
      0x88100105,  //  0026  GETMBR	R4	R0	K5
      0x4C140000,  //  0027  LDNIL	R5
      0x20100805,  //  0028  NE	R4	R4	R5
      0x7812001A,  //  0029  JMPF	R4	#0045
      0x88100109,  //  002A  GETMBR	R4	R0	K9
      0x4C140000,  //  002B  LDNIL	R5
      0x20100805,  //  002C  NE	R4	R4	R5
      0x78120016,  //  002D  JMPF	R4	#0045
      0x8810010A,  //  002E  GETMBR	R4	R0	K10
      0x4C140000,  //  002F  LDNIL	R5
      0x20100805,  //  0030  NE	R4	R4	R5
      0x78120012,  //  0031  JMPF	R4	#0045
      0x60100018,  //  0032  GETGBL	R4	G24
      0x5814000B,  //  0033  LDCONST	R5	K11
      0x88180105,  //  0034  GETMBR	R6	R0	K5
      0x881C010A,  //  0035  GETMBR	R7	R0	K10
      0x781E0001,  //  0036  JMPF	R7	#0039
      0x581C000C,  //  0037  LDCONST	R7	K12
      0x70020000,  //  0038  JMP		#003A
      0x581C000D,  //  0039  LDCONST	R7	K13
      0x88200109,  //  003A  GETMBR	R8	R0	K9
      0x7C100800,  //  003B  CALL	R4	4
      0x5C080800,  //  003C  MOVE	R2	R4
      0x88100103,  //  003D  GETMBR	R4	R0	K3
      0x24100904,  //  003E  GT	R4	R4	K4
      0x78120004,  //  003F  JMPF	R4	#0045
      0x60100008,  //  0040  GETGBL	R4	G8
      0x88140103,  //  0041  GETMBR	R5	R0	K3
      0x7C100200,  //  0042  CALL	R4	1
      0x00121004,  //  0043  ADD	R4	K8	R4
      0x00080404,  //  0044  ADD	R2	R2	R4
      0x80040400,  //  0045  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(zcl_attribute_setmember,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(cluster),
    /* K1   */  be_nested_str_weak(_cluster),
    /* K2   */  be_nested_str_weak(attr_id),
    /* K3   */  be_nested_str_weak(_attr_id),
    /* K4   */  be_nested_str_weak(_iscmd),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(cmd),
    /* K7   */  be_nested_str_weak(_cmd),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(direction),
    /* K10  */  be_nested_str_weak(_direction),
    /* K11  */  be_nested_str_weak(val),
    /* K12  */  be_nested_str_weak(_set_val),
    /* K13  */  be_nested_str_weak(key),
    /* K14  */  be_nested_str_weak(_set_key),
    /* K15  */  be_nested_str_weak(setmember),
    }),
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0007,  //  0001  JMPF	R3	#000A
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0403,  //  0003  EQ	R3	R2	R3
      0x780E0002,  //  0004  JMPF	R3	#0008
      0x540EFFFE,  //  0005  LDINT	R3	65535
      0x90020203,  //  0006  SETMBR	R0	K1	R3
      0x70020000,  //  0007  JMP		#0009
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x70020036,  //  0009  JMP		#0041
      0x1C0C0302,  //  000A  EQ	R3	R1	K2
      0x780E0008,  //  000B  JMPF	R3	#0015
      0x4C0C0000,  //  000C  LDNIL	R3
      0x1C0C0403,  //  000D  EQ	R3	R2	R3
      0x780E0002,  //  000E  JMPF	R3	#0012
      0x540EFFFE,  //  000F  LDINT	R3	65535
      0x90020603,  //  0010  SETMBR	R0	K3	R3
      0x70020001,  //  0011  JMP		#0014
      0x90020602,  //  0012  SETMBR	R0	K3	R2
      0x90020905,  //  0013  SETMBR	R0	K4	K5
      0x7002002B,  //  0014  JMP		#0041
      0x1C0C0306,  //  0015  EQ	R3	R1	K6
      0x780E0008,  //  0016  JMPF	R3	#0020
      0x4C0C0000,  //  0017  LDNIL	R3
      0x1C0C0403,  //  0018  EQ	R3	R2	R3
      0x780E0002,  //  0019  JMPF	R3	#001D
      0x540E00FE,  //  001A  LDINT	R3	255
      0x90020E03,  //  001B  SETMBR	R0	K7	R3
      0x70020001,  //  001C  JMP		#001F
      0x90020E02,  //  001D  SETMBR	R0	K7	R2
      0x90020908,  //  001E  SETMBR	R0	K4	K8
      0x70020020,  //  001F  JMP		#0041
      0x1C0C0309,  //  0020  EQ	R3	R1	K9
      0x780E000B,  //  0021  JMPF	R3	#002E
      0x4C0C0000,  //  0022  LDNIL	R3
      0x1C0C0403,  //  0023  EQ	R3	R2	R3
      0x780E0001,  //  0024  JMPF	R3	#0027
      0x90021505,  //  0025  SETMBR	R0	K10	K5
      0x70020005,  //  0026  JMP		#002D
      0x780A0001,  //  0027  JMPF	R2	#002A
      0x580C0008,  //  0028  LDCONST	R3	K8
      0x70020000,  //  0029  JMP		#002B
      0x580C0005,  //  002A  LDCONST	R3	K5
      0x90021403,  //  002B  SETMBR	R0	K10	R3
      0x90020908,  //  002C  SETMBR	R0	K4	K8
      0x70020012,  //  002D  JMP		#0041
      0x1C0C030B,  //  002E  EQ	R3	R1	K11
      0x780E0003,  //  002F  JMPF	R3	#0034
      0x8C0C010C,  //  0030  GETMET	R3	R0	K12
      0x5C140400,  //  0031  MOVE	R5	R2
      0x7C0C0400,  //  0032  CALL	R3	2
      0x7002000C,  //  0033  JMP		#0041
      0x1C0C030D,  //  0034  EQ	R3	R1	K13
      0x780E0003,  //  0035  JMPF	R3	#003A
      0x8C0C010E,  //  0036  GETMET	R3	R0	K14
      0x5C140400,  //  0037  MOVE	R5	R2
      0x7C0C0400,  //  0038  CALL	R3	2
      0x70020006,  //  0039  JMP		#0041
      0x600C0003,  //  003A  GETGBL	R3	G3
      0x5C100000,  //  003B  MOVE	R4	R0
      0x7C0C0200,  //  003C  CALL	R3	1
      0x8C0C070F,  //  003D  GETMET	R3	R3	K15
      0x5C140200,  //  003E  MOVE	R5	R1
      0x5C180400,  //  003F  MOVE	R6	R2
      0x7C0C0600,  //  0040  CALL	R3	3
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(zcl_attribute_tostring,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(dump),
    /* K4   */  be_nested_str_weak(key_tostring),
    /* K5   */  be_nested_str_weak(_X3A),
    /* K6   */  be_nested_str_weak(get),
    /* K7   */  be_nested_str_weak(val),
    /* K8   */  be_nested_str_weak(tojson),
    /* K9   */  be_nested_str_weak(cluster),
    /* K10  */  be_nested_str_weak(attr_id),
    /* K11  */  be_nested_str_weak(key),
    /* K12  */  be_nested_str_weak(_X28_X25s_X29),
    /* K13  */  be_nested_str_weak(_X2C_X25s_X3A_X22_X2504X_X2F_X2504X_X22),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x8C100503,  //  0003  GETMET	R4	R2	K3
      0x8C180104,  //  0004  GETMET	R6	R0	K4
      0x7C180200,  //  0005  CALL	R6	1
      0x7C100400,  //  0006  CALL	R4	2
      0x00100905,  //  0007  ADD	R4	R4	K5
      0x8C140306,  //  0008  GETMET	R5	R1	K6
      0x881C0107,  //  0009  GETMBR	R7	R0	K7
      0x58200008,  //  000A  LDCONST	R8	K8
      0x7C140600,  //  000B  CALL	R5	3
      0x4C180000,  //  000C  LDNIL	R6
      0x20140A06,  //  000D  NE	R5	R5	R6
      0x78160004,  //  000E  JMPF	R5	#0014
      0x88140107,  //  000F  GETMBR	R5	R0	K7
      0x8C140B08,  //  0010  GETMET	R5	R5	K8
      0x7C140200,  //  0011  CALL	R5	1
      0x00100805,  //  0012  ADD	R4	R4	R5
      0x70020003,  //  0013  JMP		#0018
      0x8C140503,  //  0014  GETMET	R5	R2	K3
      0x881C0107,  //  0015  GETMBR	R7	R0	K7
      0x7C140400,  //  0016  CALL	R5	2
      0x00100805,  //  0017  ADD	R4	R4	R5
      0x88140109,  //  0018  GETMBR	R5	R0	K9
      0x4C180000,  //  0019  LDNIL	R6
      0x20140A06,  //  001A  NE	R5	R5	R6
      0x78160015,  //  001B  JMPF	R5	#0032
      0x8814010A,  //  001C  GETMBR	R5	R0	K10
      0x4C180000,  //  001D  LDNIL	R6
      0x20140A06,  //  001E  NE	R5	R5	R6
      0x78160011,  //  001F  JMPF	R5	#0032
      0x8814010B,  //  0020  GETMBR	R5	R0	K11
      0x4C180000,  //  0021  LDNIL	R6
      0x20140A06,  //  0022  NE	R5	R5	R6
      0x7816000D,  //  0023  JMPF	R5	#0032
      0x60140018,  //  0024  GETGBL	R5	G24
      0x5818000C,  //  0025  LDCONST	R6	K12
      0x8C1C0104,  //  0026  GETMET	R7	R0	K4
      0x7C1C0200,  //  0027  CALL	R7	1
      0x7C140400,  //  0028  CALL	R5	2
      0x60180018,  //  0029  GETGBL	R6	G24
      0x581C000D,  //  002A  LDCONST	R7	K13
      0x8C200503,  //  002B  GETMET	R8	R2	K3
      0x5C280A00,  //  002C  MOVE	R10	R5
      0x7C200400,  //  002D  CALL	R8	2
      0x88240109,  //  002E  GETMBR	R9	R0	K9
      0x8828010A,  //  002F  GETMBR	R10	R0	K10
      0x7C180800,  //  0030  CALL	R6	4
      0x00100806,  //  0031  ADD	R4	R4	R6
      0x80040800,  //  0032  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(zcl_attribute_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(_init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x780A0001,  //  0008  JMPF	R2	#000B
      0x8C080101,  //  0009  GETMET	R2	R0	K1
      0x7C080200,  //  000A  CALL	R2	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: deinit
********************************************************************/
be_local_closure(zcl_attribute_deinit,   /* name */
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
    /* K0   */  be_nested_str_weak(ismapped),
    /* K1   */  be_nested_str_weak(_deinit),
    }),
    be_str_weak(deinit),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060001,  //  0002  JMPT	R1	#0005
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(zcl_attribute_member,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(cluster),
    /* K1   */  be_nested_str_weak(_cluster),
    /* K2   */  be_nested_str_weak(attr_id),
    /* K3   */  be_nested_str_weak(_attr_id),
    /* K4   */  be_nested_str_weak(_iscmd),
    /* K5   */  be_nested_str_weak(cmd),
    /* K6   */  be_nested_str_weak(_cmd),
    /* K7   */  be_nested_str_weak(direction),
    /* K8   */  be_nested_str_weak(_direction),
    /* K9   */  be_nested_str_weak(cmd_general),
    /* K10  */  be_nested_str_weak(_cmd_general),
    /* K11  */  be_nested_str_weak(val),
    /* K12  */  be_nested_str_weak(_get_val),
    /* K13  */  be_nested_str_weak(tohex),
    /* K14  */  be_nested_str_weak(key),
    /* K15  */  be_nested_str_weak(_get_key),
    /* K16  */  be_nested_str_weak(member),
    }),
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0x1C080300,  //  0000  EQ	R2	R1	K0
      0x780A0008,  //  0001  JMPF	R2	#000B
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x540EFFFE,  //  0003  LDINT	R3	65535
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x70020000,  //  0007  JMP		#0009
      0x4C0C0000,  //  0008  LDNIL	R3
      0x80040600,  //  0009  RET	1	R3
      0x7002003E,  //  000A  JMP		#004A
      0x1C080302,  //  000B  EQ	R2	R1	K2
      0x780A000A,  //  000C  JMPF	R2	#0018
      0x88080103,  //  000D  GETMBR	R2	R0	K3
      0x540EFFFE,  //  000E  LDINT	R3	65535
      0x200C0403,  //  000F  NE	R3	R2	R3
      0x780E0003,  //  0010  JMPF	R3	#0015
      0x880C0104,  //  0011  GETMBR	R3	R0	K4
      0x740E0001,  //  0012  JMPT	R3	#0015
      0x5C0C0400,  //  0013  MOVE	R3	R2
      0x70020000,  //  0014  JMP		#0016
      0x4C0C0000,  //  0015  LDNIL	R3
      0x80040600,  //  0016  RET	1	R3
      0x70020031,  //  0017  JMP		#004A
      0x1C080305,  //  0018  EQ	R2	R1	K5
      0x780A000A,  //  0019  JMPF	R2	#0025
      0x88080106,  //  001A  GETMBR	R2	R0	K6
      0x540E00FE,  //  001B  LDINT	R3	255
      0x200C0403,  //  001C  NE	R3	R2	R3
      0x780E0003,  //  001D  JMPF	R3	#0022
      0x880C0104,  //  001E  GETMBR	R3	R0	K4
      0x780E0001,  //  001F  JMPF	R3	#0022
      0x5C0C0400,  //  0020  MOVE	R3	R2
      0x70020000,  //  0021  JMP		#0023
      0x4C0C0000,  //  0022  LDNIL	R3
      0x80040600,  //  0023  RET	1	R3
      0x70020024,  //  0024  JMP		#004A
      0x1C080307,  //  0025  EQ	R2	R1	K7
      0x780A0002,  //  0026  JMPF	R2	#002A
      0x88080108,  //  0027  GETMBR	R2	R0	K8
      0x80040400,  //  0028  RET	1	R2
      0x7002001F,  //  0029  JMP		#004A
      0x1C080309,  //  002A  EQ	R2	R1	K9
      0x780A0002,  //  002B  JMPF	R2	#002F
      0x8808010A,  //  002C  GETMBR	R2	R0	K10
      0x80040400,  //  002D  RET	1	R2
      0x7002001A,  //  002E  JMP		#004A
      0x1C08030B,  //  002F  EQ	R2	R1	K11
      0x780A000B,  //  0030  JMPF	R2	#003D
      0x8C08010C,  //  0031  GETMET	R2	R0	K12
      0x7C080200,  //  0032  CALL	R2	1
      0x600C000F,  //  0033  GETGBL	R3	G15
      0x5C100400,  //  0034  MOVE	R4	R2
      0x60140015,  //  0035  GETGBL	R5	G21
      0x7C0C0400,  //  0036  CALL	R3	2
      0x780E0002,  //  0037  JMPF	R3	#003B
      0x8C0C050D,  //  0038  GETMET	R3	R2	K13
      0x7C0C0200,  //  0039  CALL	R3	1
      0x5C080600,  //  003A  MOVE	R2	R3
      0x80040400,  //  003B  RET	1	R2
      0x7002000C,  //  003C  JMP		#004A
      0x1C08030E,  //  003D  EQ	R2	R1	K14
      0x780A0003,  //  003E  JMPF	R2	#0043
      0x8C08010F,  //  003F  GETMET	R2	R0	K15
      0x7C080200,  //  0040  CALL	R2	1
      0x80040400,  //  0041  RET	1	R2
      0x70020006,  //  0042  JMP		#004A
      0x60080003,  //  0043  GETGBL	R2	G3
      0x5C0C0000,  //  0044  MOVE	R3	R0
      0x7C080200,  //  0045  CALL	R2	1
      0x8C080510,  //  0046  GETMET	R2	R2	K16
      0x5C100200,  //  0047  MOVE	R4	R1
      0x7C080400,  //  0048  CALL	R2	2
      0x80040400,  //  0049  RET	1	R2
      0x80000000,  //  004A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: zcl_attribute
********************************************************************/
extern const bclass be_class_zcl_attribute_ntv;
be_local_class(zcl_attribute,
    0,
    &be_class_zcl_attribute_ntv,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tomap, -1), be_const_closure(zcl_attribute_tomap_closure) },
        { be_const_key_weak(key_tostring, -1), be_const_closure(zcl_attribute_key_tostring_closure) },
        { be_const_key_weak(init, -1), be_const_closure(zcl_attribute_init_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(zcl_attribute_tostring_closure) },
        { be_const_key_weak(deinit, 2), be_const_closure(zcl_attribute_deinit_closure) },
        { be_const_key_weak(setmember, 4), be_const_closure(zcl_attribute_setmember_closure) },
        { be_const_key_weak(member, -1), be_const_closure(zcl_attribute_member_closure) },
    })),
    be_str_weak(zcl_attribute)
);
/*******************************************************************/

void be_load_zcl_attribute_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_zcl_attribute);
    be_setglobal(vm, "zcl_attribute");
    be_pop(vm, 1);
}

extern const bclass be_class_zcl_attribute_list;

/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(zcl_attribute_list_member,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(groupaddr),
    /* K1   */  be_nested_str_weak(_groupaddr),
    /* K2   */  be_nested_str_weak(src_ep),
    /* K3   */  be_nested_str_weak(_src_ep),
    /* K4   */  be_nested_str_weak(lqi),
    /* K5   */  be_nested_str_weak(_lqi),
    /* K6   */  be_nested_str_weak(member),
    }),
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x1C080300,  //  0000  EQ	R2	R1	K0
      0x780A0008,  //  0001  JMPF	R2	#000B
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x540EFFFE,  //  0003  LDINT	R3	65535
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x70020000,  //  0007  JMP		#0009
      0x4C0C0000,  //  0008  LDNIL	R3
      0x80040600,  //  0009  RET	1	R3
      0x7002001C,  //  000A  JMP		#0028
      0x1C080302,  //  000B  EQ	R2	R1	K2
      0x780A0008,  //  000C  JMPF	R2	#0016
      0x88080103,  //  000D  GETMBR	R2	R0	K3
      0x540E00FE,  //  000E  LDINT	R3	255
      0x200C0403,  //  000F  NE	R3	R2	R3
      0x780E0001,  //  0010  JMPF	R3	#0013
      0x5C0C0400,  //  0011  MOVE	R3	R2
      0x70020000,  //  0012  JMP		#0014
      0x4C0C0000,  //  0013  LDNIL	R3
      0x80040600,  //  0014  RET	1	R3
      0x70020011,  //  0015  JMP		#0028
      0x1C080304,  //  0016  EQ	R2	R1	K4
      0x780A0008,  //  0017  JMPF	R2	#0021
      0x88080105,  //  0018  GETMBR	R2	R0	K5
      0x540E00FE,  //  0019  LDINT	R3	255
      0x200C0403,  //  001A  NE	R3	R2	R3
      0x780E0001,  //  001B  JMPF	R3	#001E
      0x5C0C0400,  //  001C  MOVE	R3	R2
      0x70020000,  //  001D  JMP		#001F
      0x4C0C0000,  //  001E  LDNIL	R3
      0x80040600,  //  001F  RET	1	R3
      0x70020006,  //  0020  JMP		#0028
      0x60080003,  //  0021  GETGBL	R2	G3
      0x5C0C0000,  //  0022  MOVE	R3	R0
      0x7C080200,  //  0023  CALL	R2	1
      0x8C080506,  //  0024  GETMET	R2	R2	K6
      0x5C100200,  //  0025  MOVE	R4	R1
      0x7C080400,  //  0026  CALL	R2	2
      0x80040400,  //  0027  RET	1	R2
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(zcl_attribute_list_setmember,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(groupaddr),
    /* K1   */  be_nested_str_weak(_groupaddr),
    /* K2   */  be_nested_str_weak(src_ep),
    /* K3   */  be_nested_str_weak(_src_ep),
    /* K4   */  be_nested_str_weak(lqi),
    /* K5   */  be_nested_str_weak(_lqi),
    /* K6   */  be_nested_str_weak(setmember),
    }),
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0007,  //  0001  JMPF	R3	#000A
      0x4C0C0000,  //  0002  LDNIL	R3
      0x200C0403,  //  0003  NE	R3	R2	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x70020000,  //  0006  JMP		#0008
      0x540EFFFE,  //  0007  LDINT	R3	65535
      0x90020203,  //  0008  SETMBR	R0	K1	R3
      0x7002001A,  //  0009  JMP		#0025
      0x1C0C0302,  //  000A  EQ	R3	R1	K2
      0x780E0007,  //  000B  JMPF	R3	#0014
      0x4C0C0000,  //  000C  LDNIL	R3
      0x200C0403,  //  000D  NE	R3	R2	R3
      0x780E0001,  //  000E  JMPF	R3	#0011
      0x5C0C0400,  //  000F  MOVE	R3	R2
      0x70020000,  //  0010  JMP		#0012
      0x540E00FE,  //  0011  LDINT	R3	255
      0x90020603,  //  0012  SETMBR	R0	K3	R3
      0x70020010,  //  0013  JMP		#0025
      0x1C0C0304,  //  0014  EQ	R3	R1	K4
      0x780E0007,  //  0015  JMPF	R3	#001E
      0x4C0C0000,  //  0016  LDNIL	R3
      0x200C0403,  //  0017  NE	R3	R2	R3
      0x780E0001,  //  0018  JMPF	R3	#001B
      0x5C0C0400,  //  0019  MOVE	R3	R2
      0x70020000,  //  001A  JMP		#001C
      0x540E00FE,  //  001B  LDINT	R3	255
      0x90020A03,  //  001C  SETMBR	R0	K5	R3
      0x70020006,  //  001D  JMP		#0025
      0x600C0003,  //  001E  GETGBL	R3	G3
      0x5C100000,  //  001F  MOVE	R4	R0
      0x7C0C0200,  //  0020  CALL	R3	1
      0x8C0C0706,  //  0021  GETMET	R3	R3	K6
      0x5C140200,  //  0022  MOVE	R5	R1
      0x5C180400,  //  0023  MOVE	R6	R2
      0x7C0C0600,  //  0024  CALL	R3	3
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(zcl_attribute_list_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(_init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x780A0001,  //  0008  JMPF	R2	#000B
      0x8C080101,  //  0009  GETMET	R2	R0	K1
      0x7C080200,  //  000A  CALL	R2	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: deinit
********************************************************************/
be_local_closure(zcl_attribute_list_deinit,   /* name */
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
    /* K0   */  be_nested_str_weak(ismapped),
    /* K1   */  be_nested_str_weak(_deinit),
    }),
    be_str_weak(deinit),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060001,  //  0002  JMPT	R1	#0005
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(zcl_attribute_list_tostring,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(shortaddr),
    /* K3   */  be_nested_str_weak(push),
    /* K4   */  be_nested_str_weak(_X22Device_X22_X3A_X220x_X2504X_X22),
    /* K5   */  be_nested_str_weak(groupaddr),
    /* K6   */  be_nested_str_weak(_X22Group_X22_X3A_X220x_X2504X_X22),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(size),
    /* K9   */  be_nested_str_weak(tostring),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(src_ep),
    /* K12  */  be_nested_str_weak(_X22Endpoint_X22_X3A_X25i),
    /* K13  */  be_nested_str_weak(lqi),
    /* K14  */  be_nested_str_weak(_X22LinkQuality_X22_X3A_X25i),
    /* K15  */  be_nested_str_weak(_X7B),
    /* K16  */  be_nested_str_weak(concat),
    /* K17  */  be_nested_str_weak(_X2C),
    /* K18  */  be_nested_str_weak(_X7D),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0012,  //  0002  GETGBL	R3	G18
      0x7C0C0000,  //  0003  CALL	R3	0
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x4C140000,  //  0005  LDNIL	R5
      0x20100805,  //  0006  NE	R4	R4	R5
      0x78120005,  //  0007  JMPF	R4	#000E
      0x8C100703,  //  0008  GETMET	R4	R3	K3
      0x60180018,  //  0009  GETGBL	R6	G24
      0x581C0004,  //  000A  LDCONST	R7	K4
      0x88200102,  //  000B  GETMBR	R8	R0	K2
      0x7C180400,  //  000C  CALL	R6	2
      0x7C100400,  //  000D  CALL	R4	2
      0x88100105,  //  000E  GETMBR	R4	R0	K5
      0x4C140000,  //  000F  LDNIL	R5
      0x20100805,  //  0010  NE	R4	R4	R5
      0x78120005,  //  0011  JMPF	R4	#0018
      0x8C100703,  //  0012  GETMET	R4	R3	K3
      0x60180018,  //  0013  GETGBL	R6	G24
      0x581C0006,  //  0014  LDCONST	R7	K6
      0x88200105,  //  0015  GETMBR	R8	R0	K5
      0x7C180400,  //  0016  CALL	R6	2
      0x7C100400,  //  0017  CALL	R4	2
      0x58100007,  //  0018  LDCONST	R4	K7
      0x8C140108,  //  0019  GETMET	R5	R0	K8
      0x7C140200,  //  001A  CALL	R5	1
      0x14180805,  //  001B  LT	R6	R4	R5
      0x781A0006,  //  001C  JMPF	R6	#0024
      0x8C180703,  //  001D  GETMET	R6	R3	K3
      0x94200004,  //  001E  GETIDX	R8	R0	R4
      0x8C201109,  //  001F  GETMET	R8	R8	K9
      0x7C200200,  //  0020  CALL	R8	1
      0x7C180400,  //  0021  CALL	R6	2
      0x0010090A,  //  0022  ADD	R4	R4	K10
      0x7001FFF6,  //  0023  JMP		#001B
      0x8818010B,  //  0024  GETMBR	R6	R0	K11
      0x4C1C0000,  //  0025  LDNIL	R7
      0x20180C07,  //  0026  NE	R6	R6	R7
      0x781A0005,  //  0027  JMPF	R6	#002E
      0x8C180703,  //  0028  GETMET	R6	R3	K3
      0x60200018,  //  0029  GETGBL	R8	G24
      0x5824000C,  //  002A  LDCONST	R9	K12
      0x8828010B,  //  002B  GETMBR	R10	R0	K11
      0x7C200400,  //  002C  CALL	R8	2
      0x7C180400,  //  002D  CALL	R6	2
      0x8818010D,  //  002E  GETMBR	R6	R0	K13
      0x4C1C0000,  //  002F  LDNIL	R7
      0x20180C07,  //  0030  NE	R6	R6	R7
      0x781A0005,  //  0031  JMPF	R6	#0038
      0x8C180703,  //  0032  GETMET	R6	R3	K3
      0x60200018,  //  0033  GETGBL	R8	G24
      0x5824000E,  //  0034  LDCONST	R9	K14
      0x8828010D,  //  0035  GETMBR	R10	R0	K13
      0x7C200400,  //  0036  CALL	R8	2
      0x7C180400,  //  0037  CALL	R6	2
      0x8C180710,  //  0038  GETMET	R6	R3	K16
      0x58200011,  //  0039  LDCONST	R8	K17
      0x7C180400,  //  003A  CALL	R6	2
      0x001A1E06,  //  003B  ADD	R6	K15	R6
      0x00180D12,  //  003C  ADD	R6	R6	K18
      0x80040C00,  //  003D  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: zcl_attribute_list
********************************************************************/
extern const bclass be_class_zcl_attribute_list_ntv;
be_local_class(zcl_attribute_list,
    1,
    &be_class_zcl_attribute_list_ntv,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(shortaddr, -1), be_const_var(0) },
        { be_const_key_weak(member, 2), be_const_closure(zcl_attribute_list_member_closure) },
        { be_const_key_weak(tostring, 5), be_const_closure(zcl_attribute_list_tostring_closure) },
        { be_const_key_weak(init, -1), be_const_closure(zcl_attribute_list_init_closure) },
        { be_const_key_weak(deinit, -1), be_const_closure(zcl_attribute_list_deinit_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(zcl_attribute_list_setmember_closure) },
    })),
    be_str_weak(zcl_attribute_list)
);
/*******************************************************************/

void be_load_zcl_attribute_list_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_zcl_attribute_list);
    be_setglobal(vm, "zcl_attribute_list");
    be_pop(vm, 1);
}

extern const bclass be_class_zcl_attributes;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(zcl_attributes_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(shortaddr),
    /* K1   */  be_nested_str_weak(groupaddr),
    /* K2   */  be_nested_str_weak(lqi),
    /* K3   */  be_nested_str_weak(src_ep),
    /* K4   */  be_nested_str_weak(attr_list),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x90020604,  //  0003  SETMBR	R0	K3	R4
      0x6018000F,  //  0004  GETGBL	R6	G15
      0x881C0104,  //  0005  GETMBR	R7	R0	K4
      0x60200012,  //  0006  GETGBL	R8	G18
      0x7C180400,  //  0007  CALL	R6	2
      0x781A0001,  //  0008  JMPF	R6	#000B
      0x90020805,  //  0009  SETMBR	R0	K4	R5
      0x70020002,  //  000A  JMP		#000E
      0x60180012,  //  000B  GETGBL	R6	G18
      0x7C180000,  //  000C  CALL	R6	0
      0x90020806,  //  000D  SETMBR	R0	K4	R6
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(zcl_attributes_tostring,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(shortaddr),
    /* K3   */  be_nested_str_weak(push),
    /* K4   */  be_nested_str_weak(_X22Device_X22_X3A_X220x_X2504X_X22),
    /* K5   */  be_nested_str_weak(_X22Group_X22_X3A_X220x_X2504X_X22),
    /* K6   */  be_nested_str_weak(groupaddr),
    /* K7   */  be_nested_str_weak(attr_list),
    /* K8   */  be_nested_str_weak(tostring),
    /* K9   */  be_nested_str_weak(stop_iteration),
    /* K10  */  be_nested_str_weak(src_ep),
    /* K11  */  be_nested_str_weak(_X22Endpoint_X22_X3A_X25i),
    /* K12  */  be_nested_str_weak(lqi),
    /* K13  */  be_nested_str_weak(_X22LinkQuality_X22_X3A_X25i),
    /* K14  */  be_nested_str_weak(_X7B),
    /* K15  */  be_nested_str_weak(concat),
    /* K16  */  be_nested_str_weak(_X2C),
    /* K17  */  be_nested_str_weak(_X7D),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0012,  //  0002  GETGBL	R3	G18
      0x7C0C0000,  //  0003  CALL	R3	0
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x4C140000,  //  0005  LDNIL	R5
      0x20100805,  //  0006  NE	R4	R4	R5
      0x78120005,  //  0007  JMPF	R4	#000E
      0x8C100703,  //  0008  GETMET	R4	R3	K3
      0x60180018,  //  0009  GETGBL	R6	G24
      0x581C0004,  //  000A  LDCONST	R7	K4
      0x88200102,  //  000B  GETMBR	R8	R0	K2
      0x7C180400,  //  000C  CALL	R6	2
      0x7C100400,  //  000D  CALL	R4	2
      0x88100102,  //  000E  GETMBR	R4	R0	K2
      0x4C140000,  //  000F  LDNIL	R5
      0x20100805,  //  0010  NE	R4	R4	R5
      0x78120005,  //  0011  JMPF	R4	#0018
      0x8C100703,  //  0012  GETMET	R4	R3	K3
      0x60180018,  //  0013  GETGBL	R6	G24
      0x581C0005,  //  0014  LDCONST	R7	K5
      0x88200106,  //  0015  GETMBR	R8	R0	K6
      0x7C180400,  //  0016  CALL	R6	2
      0x7C100400,  //  0017  CALL	R4	2
      0x60100010,  //  0018  GETGBL	R4	G16
      0x88140107,  //  0019  GETMBR	R5	R0	K7
      0x7C100200,  //  001A  CALL	R4	1
      0xA8020006,  //  001B  EXBLK	0	#0023
      0x5C140800,  //  001C  MOVE	R5	R4
      0x7C140000,  //  001D  CALL	R5	0
      0x8C180703,  //  001E  GETMET	R6	R3	K3
      0x8C200B08,  //  001F  GETMET	R8	R5	K8
      0x7C200200,  //  0020  CALL	R8	1
      0x7C180400,  //  0021  CALL	R6	2
      0x7001FFF8,  //  0022  JMP		#001C
      0x58100009,  //  0023  LDCONST	R4	K9
      0xAC100200,  //  0024  CATCH	R4	1	0
      0xB0080000,  //  0025  RAISE	2	R0	R0
      0x8810010A,  //  0026  GETMBR	R4	R0	K10
      0x4C140000,  //  0027  LDNIL	R5
      0x20100805,  //  0028  NE	R4	R4	R5
      0x78120005,  //  0029  JMPF	R4	#0030
      0x8C100703,  //  002A  GETMET	R4	R3	K3
      0x60180018,  //  002B  GETGBL	R6	G24
      0x581C000B,  //  002C  LDCONST	R7	K11
      0x8820010A,  //  002D  GETMBR	R8	R0	K10
      0x7C180400,  //  002E  CALL	R6	2
      0x7C100400,  //  002F  CALL	R4	2
      0x8810010C,  //  0030  GETMBR	R4	R0	K12
      0x4C140000,  //  0031  LDNIL	R5
      0x20100805,  //  0032  NE	R4	R4	R5
      0x78120005,  //  0033  JMPF	R4	#003A
      0x8C100703,  //  0034  GETMET	R4	R3	K3
      0x60180018,  //  0035  GETGBL	R6	G24
      0x581C000D,  //  0036  LDCONST	R7	K13
      0x8820010C,  //  0037  GETMBR	R8	R0	K12
      0x7C180400,  //  0038  CALL	R6	2
      0x7C100400,  //  0039  CALL	R4	2
      0x8C10070F,  //  003A  GETMET	R4	R3	K15
      0x58180010,  //  003B  LDCONST	R6	K16
      0x7C100400,  //  003C  CALL	R4	2
      0x00121C04,  //  003D  ADD	R4	K14	R4
      0x00100911,  //  003E  ADD	R4	R4	K17
      0x80040800,  //  003F  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push
********************************************************************/
be_local_closure(zcl_attributes_push,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(attr_list),
    /* K1   */  be_nested_str_weak(push),
    }),
    be_str_weak(push),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: zcl_attributes
********************************************************************/
be_local_class(zcl_attributes,
    5,
    NULL,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(groupaddr, 6), be_const_var(1) },
        { be_const_key_weak(push, 4), be_const_closure(zcl_attributes_push_closure) },
        { be_const_key_weak(shortaddr, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(zcl_attributes_init_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(zcl_attributes_tostring_closure) },
        { be_const_key_weak(lqi, 1), be_const_var(2) },
        { be_const_key_weak(attr_list, -1), be_const_var(4) },
        { be_const_key_weak(src_ep, 0), be_const_var(3) },
    })),
    be_str_weak(zcl_attributes)
);
/*******************************************************************/

void be_load_zcl_attributes_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_zcl_attributes);
    be_setglobal(vm, "zcl_attributes");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
