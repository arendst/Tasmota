/* Solidification of zigbee_zcl_attribute.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'zcl_attribute' ktab size: 43, total: 70 (saved 216 bytes)
static const bvalue be_ktab_class_zcl_attribute[43] = {
  /* K0   */  be_nested_str_weak(tomap),
  /* K1   */  be_nested_str_weak(cluster),
  /* K2   */  be_nested_str_weak(attr_id),
  /* K3   */  be_nested_str_weak(cmd),
  /* K4   */  be_nested_str_weak(direction),
  /* K5   */  be_nested_str_weak(key),
  /* K6   */  be_nested_str_weak(val),
  /* K7   */  be_nested_str_weak(_X3Cundefined_X3E),
  /* K8   */  be_nested_str_weak(key_suffix),
  /* K9   */  be_const_int(1),
  /* K10  */  be_nested_str_weak(_X2504X_X2F_X2504X),
  /* K11  */  be_nested_str_weak(_X2B),
  /* K12  */  be_nested_str_weak(_X2504X_X25s_X2502X),
  /* K13  */  be_nested_str_weak(_X3F),
  /* K14  */  be_nested_str_weak(_X21),
  /* K15  */  be_nested_str_weak(_cluster),
  /* K16  */  be_nested_str_weak(_attr_id),
  /* K17  */  be_nested_str_weak(_iscmd),
  /* K18  */  be_const_int(0),
  /* K19  */  be_nested_str_weak(_cmd),
  /* K20  */  be_nested_str_weak(_direction),
  /* K21  */  be_nested_str_weak(_set_val),
  /* K22  */  be_nested_str_weak(_set_key),
  /* K23  */  be_nested_str_weak(setmember),
  /* K24  */  be_nested_str_weak(introspect),
  /* K25  */  be_nested_str_weak(json),
  /* K26  */  be_nested_str_weak(dump),
  /* K27  */  be_nested_str_weak(key_tostring),
  /* K28  */  be_nested_str_weak(_X3A),
  /* K29  */  be_nested_str_weak(get),
  /* K30  */  be_nested_str_weak(tojson),
  /* K31  */  be_nested_str_weak(_X28_X25s_X29),
  /* K32  */  be_nested_str_weak(_X2C_X25s_X3A_X22_X2504X_X2F_X2504X_X22),
  /* K33  */  be_nested_str_weak(init),
  /* K34  */  be_nested_str_weak(_init),
  /* K35  */  be_nested_str_weak(ismapped),
  /* K36  */  be_nested_str_weak(_deinit),
  /* K37  */  be_nested_str_weak(cmd_general),
  /* K38  */  be_nested_str_weak(_cmd_general),
  /* K39  */  be_nested_str_weak(_get_val),
  /* K40  */  be_nested_str_weak(tohex),
  /* K41  */  be_nested_str_weak(_get_key),
  /* K42  */  be_nested_str_weak(member),
};


extern const bclass be_class_zcl_attribute;

/********************************************************************
** Solidified function: tomap
********************************************************************/
be_local_closure(class_zcl_attribute_tomap,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute,     /* shared constants */
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
be_local_closure(class_zcl_attribute_key_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute,     /* shared constants */
    be_str_weak(key_tostring),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0x58040007,  //  0000  LDCONST	R1	K7
      0x88080105,  //  0001  GETMBR	R2	R0	K5
      0x4C0C0000,  //  0002  LDNIL	R3
      0x200C0403,  //  0003  NE	R3	R2	R3
      0x780E0008,  //  0004  JMPF	R3	#000E
      0x5C040400,  //  0005  MOVE	R1	R2
      0x880C0108,  //  0006  GETMBR	R3	R0	K8
      0x240C0709,  //  0007  GT	R3	R3	K9
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x600C0008,  //  0009  GETGBL	R3	G8
      0x88100108,  //  000A  GETMBR	R4	R0	K8
      0x7C0C0200,  //  000B  CALL	R3	1
      0x00040203,  //  000C  ADD	R1	R1	R3
      0x70020035,  //  000D  JMP		#0044
      0x880C0101,  //  000E  GETMBR	R3	R0	K1
      0x4C100000,  //  000F  LDNIL	R4
      0x200C0604,  //  0010  NE	R3	R3	R4
      0x780E0012,  //  0011  JMPF	R3	#0025
      0x880C0102,  //  0012  GETMBR	R3	R0	K2
      0x4C100000,  //  0013  LDNIL	R4
      0x200C0604,  //  0014  NE	R3	R3	R4
      0x780E000E,  //  0015  JMPF	R3	#0025
      0x600C0018,  //  0016  GETGBL	R3	G24
      0x5810000A,  //  0017  LDCONST	R4	K10
      0x88140101,  //  0018  GETMBR	R5	R0	K1
      0x88180102,  //  0019  GETMBR	R6	R0	K2
      0x7C0C0600,  //  001A  CALL	R3	3
      0x5C040600,  //  001B  MOVE	R1	R3
      0x880C0108,  //  001C  GETMBR	R3	R0	K8
      0x240C0709,  //  001D  GT	R3	R3	K9
      0x780E0004,  //  001E  JMPF	R3	#0024
      0x600C0008,  //  001F  GETGBL	R3	G8
      0x88100108,  //  0020  GETMBR	R4	R0	K8
      0x7C0C0200,  //  0021  CALL	R3	1
      0x000E1603,  //  0022  ADD	R3	K11	R3
      0x00040203,  //  0023  ADD	R1	R1	R3
      0x7002001E,  //  0024  JMP		#0044
      0x880C0101,  //  0025  GETMBR	R3	R0	K1
      0x4C100000,  //  0026  LDNIL	R4
      0x200C0604,  //  0027  NE	R3	R3	R4
      0x780E001A,  //  0028  JMPF	R3	#0044
      0x880C0103,  //  0029  GETMBR	R3	R0	K3
      0x4C100000,  //  002A  LDNIL	R4
      0x200C0604,  //  002B  NE	R3	R3	R4
      0x780E0016,  //  002C  JMPF	R3	#0044
      0x880C0104,  //  002D  GETMBR	R3	R0	K4
      0x4C100000,  //  002E  LDNIL	R4
      0x200C0604,  //  002F  NE	R3	R3	R4
      0x780E0012,  //  0030  JMPF	R3	#0044
      0x600C0018,  //  0031  GETGBL	R3	G24
      0x5810000C,  //  0032  LDCONST	R4	K12
      0x88140101,  //  0033  GETMBR	R5	R0	K1
      0x88180104,  //  0034  GETMBR	R6	R0	K4
      0x781A0001,  //  0035  JMPF	R6	#0038
      0x5818000D,  //  0036  LDCONST	R6	K13
      0x70020000,  //  0037  JMP		#0039
      0x5818000E,  //  0038  LDCONST	R6	K14
      0x881C0103,  //  0039  GETMBR	R7	R0	K3
      0x7C0C0800,  //  003A  CALL	R3	4
      0x5C040600,  //  003B  MOVE	R1	R3
      0x880C0108,  //  003C  GETMBR	R3	R0	K8
      0x240C0709,  //  003D  GT	R3	R3	K9
      0x780E0004,  //  003E  JMPF	R3	#0044
      0x600C0008,  //  003F  GETGBL	R3	G8
      0x88100108,  //  0040  GETMBR	R4	R0	K8
      0x7C0C0200,  //  0041  CALL	R3	1
      0x000E1603,  //  0042  ADD	R3	K11	R3
      0x00040203,  //  0043  ADD	R1	R1	R3
      0x80040200,  //  0044  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(class_zcl_attribute_setmember,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute,     /* shared constants */
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x1C0C0301,  //  0000  EQ	R3	R1	K1
      0x780E0007,  //  0001  JMPF	R3	#000A
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0403,  //  0003  EQ	R3	R2	R3
      0x780E0002,  //  0004  JMPF	R3	#0008
      0x540EFFFE,  //  0005  LDINT	R3	65535
      0x90021E03,  //  0006  SETMBR	R0	K15	R3
      0x70020000,  //  0007  JMP		#0009
      0x90021E02,  //  0008  SETMBR	R0	K15	R2
      0x70020036,  //  0009  JMP		#0041
      0x1C0C0302,  //  000A  EQ	R3	R1	K2
      0x780E0008,  //  000B  JMPF	R3	#0015
      0x4C0C0000,  //  000C  LDNIL	R3
      0x1C0C0403,  //  000D  EQ	R3	R2	R3
      0x780E0002,  //  000E  JMPF	R3	#0012
      0x540EFFFE,  //  000F  LDINT	R3	65535
      0x90022003,  //  0010  SETMBR	R0	K16	R3
      0x70020001,  //  0011  JMP		#0014
      0x90022002,  //  0012  SETMBR	R0	K16	R2
      0x90022312,  //  0013  SETMBR	R0	K17	K18
      0x7002002B,  //  0014  JMP		#0041
      0x1C0C0303,  //  0015  EQ	R3	R1	K3
      0x780E0008,  //  0016  JMPF	R3	#0020
      0x4C0C0000,  //  0017  LDNIL	R3
      0x1C0C0403,  //  0018  EQ	R3	R2	R3
      0x780E0002,  //  0019  JMPF	R3	#001D
      0x540E00FE,  //  001A  LDINT	R3	255
      0x90022603,  //  001B  SETMBR	R0	K19	R3
      0x70020001,  //  001C  JMP		#001F
      0x90022602,  //  001D  SETMBR	R0	K19	R2
      0x90022309,  //  001E  SETMBR	R0	K17	K9
      0x70020020,  //  001F  JMP		#0041
      0x1C0C0304,  //  0020  EQ	R3	R1	K4
      0x780E000B,  //  0021  JMPF	R3	#002E
      0x4C0C0000,  //  0022  LDNIL	R3
      0x1C0C0403,  //  0023  EQ	R3	R2	R3
      0x780E0001,  //  0024  JMPF	R3	#0027
      0x90022912,  //  0025  SETMBR	R0	K20	K18
      0x70020005,  //  0026  JMP		#002D
      0x780A0001,  //  0027  JMPF	R2	#002A
      0x580C0009,  //  0028  LDCONST	R3	K9
      0x70020000,  //  0029  JMP		#002B
      0x580C0012,  //  002A  LDCONST	R3	K18
      0x90022803,  //  002B  SETMBR	R0	K20	R3
      0x90022309,  //  002C  SETMBR	R0	K17	K9
      0x70020012,  //  002D  JMP		#0041
      0x1C0C0306,  //  002E  EQ	R3	R1	K6
      0x780E0003,  //  002F  JMPF	R3	#0034
      0x8C0C0115,  //  0030  GETMET	R3	R0	K21
      0x5C140400,  //  0031  MOVE	R5	R2
      0x7C0C0400,  //  0032  CALL	R3	2
      0x7002000C,  //  0033  JMP		#0041
      0x1C0C0305,  //  0034  EQ	R3	R1	K5
      0x780E0003,  //  0035  JMPF	R3	#003A
      0x8C0C0116,  //  0036  GETMET	R3	R0	K22
      0x5C140400,  //  0037  MOVE	R5	R2
      0x7C0C0400,  //  0038  CALL	R3	2
      0x70020006,  //  0039  JMP		#0041
      0x600C0003,  //  003A  GETGBL	R3	G3
      0x5C100000,  //  003B  MOVE	R4	R0
      0x7C0C0200,  //  003C  CALL	R3	1
      0x8C0C0717,  //  003D  GETMET	R3	R3	K23
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
be_local_closure(class_zcl_attribute_tostring,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0xA4063000,  //  0000  IMPORT	R1	K24
      0xA40A3200,  //  0001  IMPORT	R2	K25
      0x8C0C051A,  //  0002  GETMET	R3	R2	K26
      0x8C14011B,  //  0003  GETMET	R5	R0	K27
      0x7C140200,  //  0004  CALL	R5	1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x000C071C,  //  0006  ADD	R3	R3	K28
      0x8C10031D,  //  0007  GETMET	R4	R1	K29
      0x88180106,  //  0008  GETMBR	R6	R0	K6
      0x581C001E,  //  0009  LDCONST	R7	K30
      0x7C100600,  //  000A  CALL	R4	3
      0x4C140000,  //  000B  LDNIL	R5
      0x20100805,  //  000C  NE	R4	R4	R5
      0x78120004,  //  000D  JMPF	R4	#0013
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x8C10091E,  //  000F  GETMET	R4	R4	K30
      0x7C100200,  //  0010  CALL	R4	1
      0x000C0604,  //  0011  ADD	R3	R3	R4
      0x70020003,  //  0012  JMP		#0017
      0x8C10051A,  //  0013  GETMET	R4	R2	K26
      0x88180106,  //  0014  GETMBR	R6	R0	K6
      0x7C100400,  //  0015  CALL	R4	2
      0x000C0604,  //  0016  ADD	R3	R3	R4
      0x88100101,  //  0017  GETMBR	R4	R0	K1
      0x4C140000,  //  0018  LDNIL	R5
      0x20100805,  //  0019  NE	R4	R4	R5
      0x78120015,  //  001A  JMPF	R4	#0031
      0x88100102,  //  001B  GETMBR	R4	R0	K2
      0x4C140000,  //  001C  LDNIL	R5
      0x20100805,  //  001D  NE	R4	R4	R5
      0x78120011,  //  001E  JMPF	R4	#0031
      0x88100105,  //  001F  GETMBR	R4	R0	K5
      0x4C140000,  //  0020  LDNIL	R5
      0x20100805,  //  0021  NE	R4	R4	R5
      0x7812000D,  //  0022  JMPF	R4	#0031
      0x60100018,  //  0023  GETGBL	R4	G24
      0x5814001F,  //  0024  LDCONST	R5	K31
      0x8C18011B,  //  0025  GETMET	R6	R0	K27
      0x7C180200,  //  0026  CALL	R6	1
      0x7C100400,  //  0027  CALL	R4	2
      0x60140018,  //  0028  GETGBL	R5	G24
      0x58180020,  //  0029  LDCONST	R6	K32
      0x8C1C051A,  //  002A  GETMET	R7	R2	K26
      0x5C240800,  //  002B  MOVE	R9	R4
      0x7C1C0400,  //  002C  CALL	R7	2
      0x88200101,  //  002D  GETMBR	R8	R0	K1
      0x88240102,  //  002E  GETMBR	R9	R0	K2
      0x7C140800,  //  002F  CALL	R5	4
      0x000C0605,  //  0030  ADD	R3	R3	R5
      0x80040600,  //  0031  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_zcl_attribute_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080521,  //  0003  GETMET	R2	R2	K33
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x780A0001,  //  0008  JMPF	R2	#000B
      0x8C080122,  //  0009  GETMET	R2	R0	K34
      0x7C080200,  //  000A  CALL	R2	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: deinit
********************************************************************/
be_local_closure(class_zcl_attribute_deinit,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute,     /* shared constants */
    be_str_weak(deinit),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040123,  //  0000  GETMET	R1	R0	K35
      0x7C040200,  //  0001  CALL	R1	1
      0x74060001,  //  0002  JMPT	R1	#0005
      0x8C040124,  //  0003  GETMET	R1	R0	K36
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(class_zcl_attribute_member,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute,     /* shared constants */
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0x1C080301,  //  0000  EQ	R2	R1	K1
      0x780A0008,  //  0001  JMPF	R2	#000B
      0x8808010F,  //  0002  GETMBR	R2	R0	K15
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
      0x88080110,  //  000D  GETMBR	R2	R0	K16
      0x540EFFFE,  //  000E  LDINT	R3	65535
      0x200C0403,  //  000F  NE	R3	R2	R3
      0x780E0003,  //  0010  JMPF	R3	#0015
      0x880C0111,  //  0011  GETMBR	R3	R0	K17
      0x740E0001,  //  0012  JMPT	R3	#0015
      0x5C0C0400,  //  0013  MOVE	R3	R2
      0x70020000,  //  0014  JMP		#0016
      0x4C0C0000,  //  0015  LDNIL	R3
      0x80040600,  //  0016  RET	1	R3
      0x70020031,  //  0017  JMP		#004A
      0x1C080303,  //  0018  EQ	R2	R1	K3
      0x780A000A,  //  0019  JMPF	R2	#0025
      0x88080113,  //  001A  GETMBR	R2	R0	K19
      0x540E00FE,  //  001B  LDINT	R3	255
      0x200C0403,  //  001C  NE	R3	R2	R3
      0x780E0003,  //  001D  JMPF	R3	#0022
      0x880C0111,  //  001E  GETMBR	R3	R0	K17
      0x780E0001,  //  001F  JMPF	R3	#0022
      0x5C0C0400,  //  0020  MOVE	R3	R2
      0x70020000,  //  0021  JMP		#0023
      0x4C0C0000,  //  0022  LDNIL	R3
      0x80040600,  //  0023  RET	1	R3
      0x70020024,  //  0024  JMP		#004A
      0x1C080304,  //  0025  EQ	R2	R1	K4
      0x780A0002,  //  0026  JMPF	R2	#002A
      0x88080114,  //  0027  GETMBR	R2	R0	K20
      0x80040400,  //  0028  RET	1	R2
      0x7002001F,  //  0029  JMP		#004A
      0x1C080325,  //  002A  EQ	R2	R1	K37
      0x780A0002,  //  002B  JMPF	R2	#002F
      0x88080126,  //  002C  GETMBR	R2	R0	K38
      0x80040400,  //  002D  RET	1	R2
      0x7002001A,  //  002E  JMP		#004A
      0x1C080306,  //  002F  EQ	R2	R1	K6
      0x780A000B,  //  0030  JMPF	R2	#003D
      0x8C080127,  //  0031  GETMET	R2	R0	K39
      0x7C080200,  //  0032  CALL	R2	1
      0x600C000F,  //  0033  GETGBL	R3	G15
      0x5C100400,  //  0034  MOVE	R4	R2
      0x60140015,  //  0035  GETGBL	R5	G21
      0x7C0C0400,  //  0036  CALL	R3	2
      0x780E0002,  //  0037  JMPF	R3	#003B
      0x8C0C0528,  //  0038  GETMET	R3	R2	K40
      0x7C0C0200,  //  0039  CALL	R3	1
      0x5C080600,  //  003A  MOVE	R2	R3
      0x80040400,  //  003B  RET	1	R2
      0x7002000C,  //  003C  JMP		#004A
      0x1C080305,  //  003D  EQ	R2	R1	K5
      0x780A0003,  //  003E  JMPF	R2	#0043
      0x8C080129,  //  003F  GETMET	R2	R0	K41
      0x7C080200,  //  0040  CALL	R2	1
      0x80040400,  //  0041  RET	1	R2
      0x70020006,  //  0042  JMP		#004A
      0x60080003,  //  0043  GETGBL	R2	G3
      0x5C0C0000,  //  0044  MOVE	R3	R0
      0x7C080200,  //  0045  CALL	R2	1
      0x8C08052A,  //  0046  GETMET	R2	R2	K42
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
        { be_const_key_weak(tomap, -1), be_const_closure(class_zcl_attribute_tomap_closure) },
        { be_const_key_weak(key_tostring, -1), be_const_closure(class_zcl_attribute_key_tostring_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_zcl_attribute_init_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_zcl_attribute_tostring_closure) },
        { be_const_key_weak(deinit, 2), be_const_closure(class_zcl_attribute_deinit_closure) },
        { be_const_key_weak(setmember, 4), be_const_closure(class_zcl_attribute_setmember_closure) },
        { be_const_key_weak(member, -1), be_const_closure(class_zcl_attribute_member_closure) },
    })),
    be_str_weak(zcl_attribute)
);
// compact class 'zcl_attribute_list' ktab size: 27, total: 36 (saved 72 bytes)
static const bvalue be_ktab_class_zcl_attribute_list[27] = {
  /* K0   */  be_nested_str_weak(groupaddr),
  /* K1   */  be_nested_str_weak(_groupaddr),
  /* K2   */  be_nested_str_weak(src_ep),
  /* K3   */  be_nested_str_weak(_src_ep),
  /* K4   */  be_nested_str_weak(lqi),
  /* K5   */  be_nested_str_weak(_lqi),
  /* K6   */  be_nested_str_weak(member),
  /* K7   */  be_nested_str_weak(setmember),
  /* K8   */  be_nested_str_weak(init),
  /* K9   */  be_nested_str_weak(_init),
  /* K10  */  be_nested_str_weak(ismapped),
  /* K11  */  be_nested_str_weak(_deinit),
  /* K12  */  be_nested_str_weak(json),
  /* K13  */  be_nested_str_weak(shortaddr),
  /* K14  */  be_nested_str_weak(push),
  /* K15  */  be_nested_str_weak(_X22Device_X22_X3A_X220x_X2504X_X22),
  /* K16  */  be_nested_str_weak(_X22Group_X22_X3A_X220x_X2504X_X22),
  /* K17  */  be_const_int(0),
  /* K18  */  be_nested_str_weak(size),
  /* K19  */  be_nested_str_weak(tostring),
  /* K20  */  be_const_int(1),
  /* K21  */  be_nested_str_weak(_X22Endpoint_X22_X3A_X25s),
  /* K22  */  be_nested_str_weak(_X22LinkQuality_X22_X3A_X25s),
  /* K23  */  be_nested_str_weak(_X7B),
  /* K24  */  be_nested_str_weak(concat),
  /* K25  */  be_nested_str_weak(_X2C),
  /* K26  */  be_nested_str_weak(_X7D),
};


extern const bclass be_class_zcl_attribute_list;

/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(class_zcl_attribute_list_member,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute_list,     /* shared constants */
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
be_local_closure(class_zcl_attribute_list_setmember,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute_list,     /* shared constants */
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
      0x8C0C0707,  //  0021  GETMET	R3	R3	K7
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
be_local_closure(class_zcl_attribute_list_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute_list,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080508,  //  0003  GETMET	R2	R2	K8
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x780A0001,  //  0008  JMPF	R2	#000B
      0x8C080109,  //  0009  GETMET	R2	R0	K9
      0x7C080200,  //  000A  CALL	R2	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: deinit
********************************************************************/
be_local_closure(class_zcl_attribute_list_deinit,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute_list,     /* shared constants */
    be_str_weak(deinit),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C04010A,  //  0000  GETMET	R1	R0	K10
      0x7C040200,  //  0001  CALL	R1	1
      0x74060001,  //  0002  JMPT	R1	#0005
      0x8C04010B,  //  0003  GETMET	R1	R0	K11
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_zcl_attribute_list_tostring,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zcl_attribute_list,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0xA4061800,  //  0000  IMPORT	R1	K12
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x880C010D,  //  0004  GETMBR	R3	R0	K13
      0x4C100000,  //  0005  LDNIL	R4
      0x200C0604,  //  0006  NE	R3	R3	R4
      0x780E0005,  //  0007  JMPF	R3	#000E
      0x8C0C050E,  //  0008  GETMET	R3	R2	K14
      0x60140018,  //  0009  GETGBL	R5	G24
      0x5818000F,  //  000A  LDCONST	R6	K15
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C140400,  //  000C  CALL	R5	2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x880C0100,  //  000E  GETMBR	R3	R0	K0
      0x4C100000,  //  000F  LDNIL	R4
      0x200C0604,  //  0010  NE	R3	R3	R4
      0x780E0005,  //  0011  JMPF	R3	#0018
      0x8C0C050E,  //  0012  GETMET	R3	R2	K14
      0x60140018,  //  0013  GETGBL	R5	G24
      0x58180010,  //  0014  LDCONST	R6	K16
      0x5C1C0600,  //  0015  MOVE	R7	R3
      0x7C140400,  //  0016  CALL	R5	2
      0x7C0C0400,  //  0017  CALL	R3	2
      0x58100011,  //  0018  LDCONST	R4	K17
      0x8C140112,  //  0019  GETMET	R5	R0	K18
      0x7C140200,  //  001A  CALL	R5	1
      0x14180805,  //  001B  LT	R6	R4	R5
      0x781A0006,  //  001C  JMPF	R6	#0024
      0x8C18050E,  //  001D  GETMET	R6	R2	K14
      0x94200004,  //  001E  GETIDX	R8	R0	R4
      0x8C201113,  //  001F  GETMET	R8	R8	K19
      0x7C200200,  //  0020  CALL	R8	1
      0x7C180400,  //  0021  CALL	R6	2
      0x00100914,  //  0022  ADD	R4	R4	K20
      0x7001FFF6,  //  0023  JMP		#001B
      0x880C0102,  //  0024  GETMBR	R3	R0	K2
      0x4C180000,  //  0025  LDNIL	R6
      0x200C0606,  //  0026  NE	R3	R3	R6
      0x780E0005,  //  0027  JMPF	R3	#002E
      0x8C18050E,  //  0028  GETMET	R6	R2	K14
      0x60200018,  //  0029  GETGBL	R8	G24
      0x58240015,  //  002A  LDCONST	R9	K21
      0x5C280600,  //  002B  MOVE	R10	R3
      0x7C200400,  //  002C  CALL	R8	2
      0x7C180400,  //  002D  CALL	R6	2
      0x880C0104,  //  002E  GETMBR	R3	R0	K4
      0x4C180000,  //  002F  LDNIL	R6
      0x200C0606,  //  0030  NE	R3	R3	R6
      0x780E0005,  //  0031  JMPF	R3	#0038
      0x8C18050E,  //  0032  GETMET	R6	R2	K14
      0x60200018,  //  0033  GETGBL	R8	G24
      0x58240016,  //  0034  LDCONST	R9	K22
      0x5C280600,  //  0035  MOVE	R10	R3
      0x7C200400,  //  0036  CALL	R8	2
      0x7C180400,  //  0037  CALL	R6	2
      0x8C180518,  //  0038  GETMET	R6	R2	K24
      0x58200019,  //  0039  LDCONST	R8	K25
      0x7C180400,  //  003A  CALL	R6	2
      0x001A2E06,  //  003B  ADD	R6	K23	R6
      0x00180D1A,  //  003C  ADD	R6	R6	K26
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
        { be_const_key_weak(member, 2), be_const_closure(class_zcl_attribute_list_member_closure) },
        { be_const_key_weak(tostring, 5), be_const_closure(class_zcl_attribute_list_tostring_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_zcl_attribute_list_init_closure) },
        { be_const_key_weak(deinit, -1), be_const_closure(class_zcl_attribute_list_deinit_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(class_zcl_attribute_list_setmember_closure) },
    })),
    be_str_weak(zcl_attribute_list)
);
/********************************************************************/
/* End of solidification */
