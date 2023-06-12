/* Solidification of Matter_Plugin_Shutter.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Shutter;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_read_attribute,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_nested_str_weak(update_inverted),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(create_TLV),
    /* K9   */  be_nested_str_weak(U1),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(shadow_shutter_inverted),
    /* K13  */  be_nested_str_weak(shadow_shutter_pos),
    /* K14  */  be_nested_str_weak(shadow_shutter_direction),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(shadow_shutter_target),
    /* K17  */  be_nested_str_weak(U4),
    /* K18  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[152]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x4C1C0000,  //  0005  LDNIL	R7
      0x54220101,  //  0006  LDINT	R8	258
      0x1C200A08,  //  0007  EQ	R8	R5	R8
      0x78220085,  //  0008  JMPF	R8	#008F
      0x8C200105,  //  0009  GETMET	R8	R0	K5
      0x7C200200,  //  000A  CALL	R8	1
      0x8C200106,  //  000B  GETMET	R8	R0	K6
      0x7C200200,  //  000C  CALL	R8	1
      0x1C200D07,  //  000D  EQ	R8	R6	K7
      0x78220005,  //  000E  JMPF	R8	#0015
      0x8C200908,  //  000F  GETMET	R8	R4	K8
      0x88280909,  //  0010  GETMBR	R10	R4	K9
      0x542E00FE,  //  0011  LDINT	R11	255
      0x7C200600,  //  0012  CALL	R8	3
      0x80041000,  //  0013  RET	1	R8
      0x70020078,  //  0014  JMP		#008E
      0x54220004,  //  0015  LDINT	R8	5
      0x1C200C08,  //  0016  EQ	R8	R6	R8
      0x78220005,  //  0017  JMPF	R8	#001E
      0x8C200908,  //  0018  GETMET	R8	R4	K8
      0x8828090A,  //  0019  GETMBR	R10	R4	K10
      0x582C0007,  //  001A  LDCONST	R11	K7
      0x7C200600,  //  001B  CALL	R8	3
      0x80041000,  //  001C  RET	1	R8
      0x7002006F,  //  001D  JMP		#008E
      0x54220006,  //  001E  LDINT	R8	7
      0x1C200C08,  //  001F  EQ	R8	R6	R8
      0x78220006,  //  0020  JMPF	R8	#0028
      0x8C200908,  //  0021  GETMET	R8	R4	K8
      0x88280909,  //  0022  GETMBR	R10	R4	K9
      0x542E0007,  //  0023  LDINT	R11	8
      0x002E160B,  //  0024  ADD	R11	K11	R11
      0x7C200600,  //  0025  CALL	R8	3
      0x80041000,  //  0026  RET	1	R8
      0x70020065,  //  0027  JMP		#008E
      0x5422000C,  //  0028  LDINT	R8	13
      0x1C200C08,  //  0029  EQ	R8	R6	R8
      0x78220005,  //  002A  JMPF	R8	#0031
      0x8C200908,  //  002B  GETMET	R8	R4	K8
      0x88280909,  //  002C  GETMBR	R10	R4	K9
      0x542E00FE,  //  002D  LDINT	R11	255
      0x7C200600,  //  002E  CALL	R8	3
      0x80041000,  //  002F  RET	1	R8
      0x7002005C,  //  0030  JMP		#008E
      0x5422000D,  //  0031  LDINT	R8	14
      0x1C200C08,  //  0032  EQ	R8	R6	R8
      0x78220013,  //  0033  JMPF	R8	#0048
      0x8820010C,  //  0034  GETMBR	R8	R0	K12
      0x1C201107,  //  0035  EQ	R8	R8	K7
      0x78220006,  //  0036  JMPF	R8	#003E
      0x54220063,  //  0037  LDINT	R8	100
      0x8824010D,  //  0038  GETMBR	R9	R0	K13
      0x04201009,  //  0039  SUB	R8	R8	R9
      0x54260063,  //  003A  LDINT	R9	100
      0x08201009,  //  003B  MUL	R8	R8	R9
      0x5C1C1000,  //  003C  MOVE	R7	R8
      0x70020003,  //  003D  JMP		#0042
      0x8820010D,  //  003E  GETMBR	R8	R0	K13
      0x54260063,  //  003F  LDINT	R9	100
      0x08201009,  //  0040  MUL	R8	R8	R9
      0x5C1C1000,  //  0041  MOVE	R7	R8
      0x8C200908,  //  0042  GETMET	R8	R4	K8
      0x8828090A,  //  0043  GETMBR	R10	R4	K10
      0x5C2C0E00,  //  0044  MOVE	R11	R7
      0x7C200600,  //  0045  CALL	R8	3
      0x80041000,  //  0046  RET	1	R8
      0x70020045,  //  0047  JMP		#008E
      0x54220009,  //  0048  LDINT	R8	10
      0x1C200C08,  //  0049  EQ	R8	R6	R8
      0x78220010,  //  004A  JMPF	R8	#005C
      0x8820010E,  //  004B  GETMBR	R8	R0	K14
      0x1C201107,  //  004C  EQ	R8	R8	K7
      0x78220001,  //  004D  JMPF	R8	#0050
      0x58200007,  //  004E  LDCONST	R8	K7
      0x70020005,  //  004F  JMP		#0056
      0x8820010E,  //  0050  GETMBR	R8	R0	K14
      0x24201107,  //  0051  GT	R8	R8	K7
      0x78220001,  //  0052  JMPF	R8	#0055
      0x5820000B,  //  0053  LDCONST	R8	K11
      0x70020000,  //  0054  JMP		#0056
      0x5820000F,  //  0055  LDCONST	R8	K15
      0x8C240908,  //  0056  GETMET	R9	R4	K8
      0x882C0909,  //  0057  GETMBR	R11	R4	K9
      0x5C301000,  //  0058  MOVE	R12	R8
      0x7C240600,  //  0059  CALL	R9	3
      0x80041200,  //  005A  RET	1	R9
      0x70020031,  //  005B  JMP		#008E
      0x5422000A,  //  005C  LDINT	R8	11
      0x1C200C08,  //  005D  EQ	R8	R6	R8
      0x78220013,  //  005E  JMPF	R8	#0073
      0x8820010C,  //  005F  GETMBR	R8	R0	K12
      0x1C201107,  //  0060  EQ	R8	R8	K7
      0x78220006,  //  0061  JMPF	R8	#0069
      0x54220063,  //  0062  LDINT	R8	100
      0x88240110,  //  0063  GETMBR	R9	R0	K16
      0x04201009,  //  0064  SUB	R8	R8	R9
      0x54260063,  //  0065  LDINT	R9	100
      0x08201009,  //  0066  MUL	R8	R8	R9
      0x5C1C1000,  //  0067  MOVE	R7	R8
      0x70020003,  //  0068  JMP		#006D
      0x88200110,  //  0069  GETMBR	R8	R0	K16
      0x54260063,  //  006A  LDINT	R9	100
      0x08201009,  //  006B  MUL	R8	R8	R9
      0x5C1C1000,  //  006C  MOVE	R7	R8
      0x8C200908,  //  006D  GETMET	R8	R4	K8
      0x8828090A,  //  006E  GETMBR	R10	R4	K10
      0x5C2C0E00,  //  006F  MOVE	R11	R7
      0x7C200600,  //  0070  CALL	R8	3
      0x80041000,  //  0071  RET	1	R8
      0x7002001A,  //  0072  JMP		#008E
      0x54220016,  //  0073  LDINT	R8	23
      0x1C200C08,  //  0074  EQ	R8	R6	R8
      0x78220005,  //  0075  JMPF	R8	#007C
      0x8C200908,  //  0076  GETMET	R8	R4	K8
      0x88280909,  //  0077  GETMBR	R10	R4	K9
      0x582C0007,  //  0078  LDCONST	R11	K7
      0x7C200600,  //  0079  CALL	R8	3
      0x80041000,  //  007A  RET	1	R8
      0x70020011,  //  007B  JMP		#008E
      0x5422FFFB,  //  007C  LDINT	R8	65532
      0x1C200C08,  //  007D  EQ	R8	R6	R8
      0x78220006,  //  007E  JMPF	R8	#0086
      0x8C200908,  //  007F  GETMET	R8	R4	K8
      0x88280911,  //  0080  GETMBR	R10	R4	K17
      0x542E0003,  //  0081  LDINT	R11	4
      0x002E160B,  //  0082  ADD	R11	K11	R11
      0x7C200600,  //  0083  CALL	R8	3
      0x80041000,  //  0084  RET	1	R8
      0x70020007,  //  0085  JMP		#008E
      0x5422FFFC,  //  0086  LDINT	R8	65533
      0x1C200C08,  //  0087  EQ	R8	R6	R8
      0x78220004,  //  0088  JMPF	R8	#008E
      0x8C200908,  //  0089  GETMET	R8	R4	K8
      0x88280911,  //  008A  GETMBR	R10	R4	K17
      0x542E0004,  //  008B  LDINT	R11	5
      0x7C200600,  //  008C  CALL	R8	3
      0x80041000,  //  008D  RET	1	R8
      0x70020007,  //  008E  JMP		#0097
      0x60200003,  //  008F  GETGBL	R8	G3
      0x5C240000,  //  0090  MOVE	R9	R0
      0x7C200200,  //  0091  CALL	R8	1
      0x8C201112,  //  0092  GETMET	R8	R8	K18
      0x5C280200,  //  0093  MOVE	R10	R1
      0x5C2C0400,  //  0094  MOVE	R11	R2
      0x7C200600,  //  0095  CALL	R8	3
      0x80041000,  //  0096  RET	1	R8
      0x80000000,  //  0097  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_parse_sensors,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(Shutter),
    /* K2   */  be_nested_str_weak(tasmota_shutter_index),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(contains),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_nested_str_weak(Position),
    /* K7   */  be_nested_str_weak(shadow_shutter_pos),
    /* K8   */  be_nested_str_weak(attribute_updated),
    /* K9   */  be_nested_str_weak(Direction),
    /* K10  */  be_nested_str_weak(shadow_shutter_direction),
    /* K11  */  be_nested_str_weak(Target),
    /* K12  */  be_nested_str_weak(shadow_shutter_target),
    }),
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0008,  //  0001  GETGBL	R3	G8
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x00100903,  //  0003  ADD	R4	R4	K3
      0x7C0C0200,  //  0004  CALL	R3	1
      0x000E0203,  //  0005  ADD	R3	K1	R3
      0x8C100304,  //  0006  GETMET	R4	R1	K4
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0x7812002A,  //  0009  JMPF	R4	#0035
      0x94100203,  //  000A  GETIDX	R4	R1	R3
      0x8C140905,  //  000B  GETMET	R5	R4	K5
      0x581C0006,  //  000C  LDCONST	R7	K6
      0x7C140400,  //  000D  CALL	R5	2
      0x4C180000,  //  000E  LDNIL	R6
      0x20180A06,  //  000F  NE	R6	R5	R6
      0x781A0007,  //  0010  JMPF	R6	#0019
      0x88180107,  //  0011  GETMBR	R6	R0	K7
      0x20180A06,  //  0012  NE	R6	R5	R6
      0x781A0003,  //  0013  JMPF	R6	#0018
      0x8C180108,  //  0014  GETMET	R6	R0	K8
      0x54220101,  //  0015  LDINT	R8	258
      0x5426000D,  //  0016  LDINT	R9	14
      0x7C180600,  //  0017  CALL	R6	3
      0x90020E05,  //  0018  SETMBR	R0	K7	R5
      0x8C180905,  //  0019  GETMET	R6	R4	K5
      0x58200009,  //  001A  LDCONST	R8	K9
      0x7C180400,  //  001B  CALL	R6	2
      0x4C1C0000,  //  001C  LDNIL	R7
      0x201C0C07,  //  001D  NE	R7	R6	R7
      0x781E0007,  //  001E  JMPF	R7	#0027
      0x881C010A,  //  001F  GETMBR	R7	R0	K10
      0x201C0C07,  //  0020  NE	R7	R6	R7
      0x781E0003,  //  0021  JMPF	R7	#0026
      0x8C1C0108,  //  0022  GETMET	R7	R0	K8
      0x54260101,  //  0023  LDINT	R9	258
      0x542A0009,  //  0024  LDINT	R10	10
      0x7C1C0600,  //  0025  CALL	R7	3
      0x90021406,  //  0026  SETMBR	R0	K10	R6
      0x8C1C0905,  //  0027  GETMET	R7	R4	K5
      0x5824000B,  //  0028  LDCONST	R9	K11
      0x7C1C0400,  //  0029  CALL	R7	2
      0x4C200000,  //  002A  LDNIL	R8
      0x20200E08,  //  002B  NE	R8	R7	R8
      0x78220007,  //  002C  JMPF	R8	#0035
      0x8820010C,  //  002D  GETMBR	R8	R0	K12
      0x20200E08,  //  002E  NE	R8	R7	R8
      0x78220003,  //  002F  JMPF	R8	#0034
      0x8C200108,  //  0030  GETMET	R8	R0	K8
      0x542A0101,  //  0031  LDINT	R10	258
      0x542E000A,  //  0032  LDINT	R11	11
      0x7C200600,  //  0033  CALL	R8	3
      0x90021807,  //  0034  SETMBR	R0	K12	R7
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_update_shadow,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(update_inverted),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(cmd),
    /* K3   */  be_nested_str_weak(ShutterPosition),
    /* K4   */  be_nested_str_weak(tasmota_shutter_index),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(parse_sensors),
    /* K7   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0xB8060200,  //  0002  GETNGBL	R1	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x600C0008,  //  0004  GETGBL	R3	G8
      0x88100104,  //  0005  GETMBR	R4	R0	K4
      0x00100905,  //  0006  ADD	R4	R4	K5
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E0603,  //  0008  ADD	R3	K3	R3
      0x50100200,  //  0009  LDBOOL	R4	1	0
      0x7C040600,  //  000A  CALL	R1	3
      0x78060002,  //  000B  JMPF	R1	#000F
      0x8C080106,  //  000C  GETMET	R2	R0	K6
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x60080003,  //  000F  GETGBL	R2	G3
      0x5C0C0000,  //  0010  MOVE	R3	R0
      0x7C080200,  //  0011  CALL	R2	1
      0x8C080507,  //  0012  GETMET	R2	R2	K7
      0x7C080200,  //  0013  CALL	R2	1
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_invoke_request,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(cmd),
    /* K9   */  be_nested_str_weak(ShutterStopOpen),
    /* K10  */  be_nested_str_weak(tasmota_shutter_index),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(update_shadow),
    /* K13  */  be_nested_str_weak(ShutterStopClose),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(ShutterStop),
    /* K16  */  be_nested_str_weak(log),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20Tilt_X20_X3D_X20),
    /* K18  */  be_nested_str_weak(findsubval),
    /* K19  */  be_nested_str_weak(shadow_shutter_inverted),
    /* K20  */  be_nested_str_weak(ShutterPosition),
    /* K21  */  be_nested_str_weak(_X20),
    /* K22  */  be_nested_str_weak(pos_X25_X3A),
    /* K23  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[116]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220101,  //  0005  LDINT	R8	258
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220061,  //  0007  JMPF	R8	#006A
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x7822000D,  //  000B  JMPF	R8	#001A
      0xB8220E00,  //  000C  GETNGBL	R8	K7
      0x8C201108,  //  000D  GETMET	R8	R8	K8
      0x60280008,  //  000E  GETGBL	R10	G8
      0x882C010A,  //  000F  GETMBR	R11	R0	K10
      0x002C170B,  //  0010  ADD	R11	R11	K11
      0x7C280200,  //  0011  CALL	R10	1
      0x002A120A,  //  0012  ADD	R10	K9	R10
      0x502C0200,  //  0013  LDBOOL	R11	1	0
      0x7C200600,  //  0014  CALL	R8	3
      0x8C20010C,  //  0015  GETMET	R8	R0	K12
      0x7C200200,  //  0016  CALL	R8	1
      0x50200200,  //  0017  LDBOOL	R8	1	0
      0x80041000,  //  0018  RET	1	R8
      0x7002004E,  //  0019  JMP		#0069
      0x1C200F0B,  //  001A  EQ	R8	R7	K11
      0x7822000D,  //  001B  JMPF	R8	#002A
      0xB8220E00,  //  001C  GETNGBL	R8	K7
      0x8C201108,  //  001D  GETMET	R8	R8	K8
      0x60280008,  //  001E  GETGBL	R10	G8
      0x882C010A,  //  001F  GETMBR	R11	R0	K10
      0x002C170B,  //  0020  ADD	R11	R11	K11
      0x7C280200,  //  0021  CALL	R10	1
      0x002A1A0A,  //  0022  ADD	R10	K13	R10
      0x502C0200,  //  0023  LDBOOL	R11	1	0
      0x7C200600,  //  0024  CALL	R8	3
      0x8C20010C,  //  0025  GETMET	R8	R0	K12
      0x7C200200,  //  0026  CALL	R8	1
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x7002003E,  //  0029  JMP		#0069
      0x1C200F0E,  //  002A  EQ	R8	R7	K14
      0x7822000D,  //  002B  JMPF	R8	#003A
      0xB8220E00,  //  002C  GETNGBL	R8	K7
      0x8C201108,  //  002D  GETMET	R8	R8	K8
      0x60280008,  //  002E  GETGBL	R10	G8
      0x882C010A,  //  002F  GETMBR	R11	R0	K10
      0x002C170B,  //  0030  ADD	R11	R11	K11
      0x7C280200,  //  0031  CALL	R10	1
      0x002A1E0A,  //  0032  ADD	R10	K15	R10
      0x502C0200,  //  0033  LDBOOL	R11	1	0
      0x7C200600,  //  0034  CALL	R8	3
      0x8C20010C,  //  0035  GETMET	R8	R0	K12
      0x7C200200,  //  0036  CALL	R8	1
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x80041000,  //  0038  RET	1	R8
      0x7002002E,  //  0039  JMP		#0069
      0x54220004,  //  003A  LDINT	R8	5
      0x1C200E08,  //  003B  EQ	R8	R7	R8
      0x7822002B,  //  003C  JMPF	R8	#0069
      0xB8220E00,  //  003D  GETNGBL	R8	K7
      0x8C201110,  //  003E  GETMET	R8	R8	K16
      0x60280008,  //  003F  GETGBL	R10	G8
      0x5C2C0400,  //  0040  MOVE	R11	R2
      0x7C280200,  //  0041  CALL	R10	1
      0x002A220A,  //  0042  ADD	R10	K17	R10
      0x582C000E,  //  0043  LDCONST	R11	K14
      0x7C200600,  //  0044  CALL	R8	3
      0x8C200512,  //  0045  GETMET	R8	R2	K18
      0x58280006,  //  0046  LDCONST	R10	K6
      0x7C200400,  //  0047  CALL	R8	2
      0x4C240000,  //  0048  LDNIL	R9
      0x20241009,  //  0049  NE	R9	R8	R9
      0x7826001B,  //  004A  JMPF	R9	#0067
      0x54260063,  //  004B  LDINT	R9	100
      0x0C201009,  //  004C  DIV	R8	R8	R9
      0x88240113,  //  004D  GETMBR	R9	R0	K19
      0x1C241306,  //  004E  EQ	R9	R9	K6
      0x78260001,  //  004F  JMPF	R9	#0052
      0x54260063,  //  0050  LDINT	R9	100
      0x04201208,  //  0051  SUB	R8	R9	R8
      0xB8260E00,  //  0052  GETNGBL	R9	K7
      0x8C241308,  //  0053  GETMET	R9	R9	K8
      0x602C0008,  //  0054  GETGBL	R11	G8
      0x8830010A,  //  0055  GETMBR	R12	R0	K10
      0x0030190B,  //  0056  ADD	R12	R12	K11
      0x7C2C0200,  //  0057  CALL	R11	1
      0x002E280B,  //  0058  ADD	R11	K20	R11
      0x002C1715,  //  0059  ADD	R11	R11	K21
      0x60300008,  //  005A  GETGBL	R12	G8
      0x5C341000,  //  005B  MOVE	R13	R8
      0x7C300200,  //  005C  CALL	R12	1
      0x002C160C,  //  005D  ADD	R11	R11	R12
      0x50300200,  //  005E  LDBOOL	R12	1	0
      0x7C240600,  //  005F  CALL	R9	3
      0x60240008,  //  0060  GETGBL	R9	G8
      0x5C281000,  //  0061  MOVE	R10	R8
      0x7C240200,  //  0062  CALL	R9	1
      0x00262C09,  //  0063  ADD	R9	K22	R9
      0x900E2009,  //  0064  SETMBR	R3	K16	R9
      0x8C24010C,  //  0065  GETMET	R9	R0	K12
      0x7C240200,  //  0066  CALL	R9	1
      0x50240200,  //  0067  LDBOOL	R9	1	0
      0x80041200,  //  0068  RET	1	R9
      0x70020008,  //  0069  JMP		#0073
      0x60200003,  //  006A  GETGBL	R8	G3
      0x5C240000,  //  006B  MOVE	R9	R0
      0x7C200200,  //  006C  CALL	R8	1
      0x8C201117,  //  006D  GETMET	R8	R8	K23
      0x5C280200,  //  006E  MOVE	R10	R1
      0x5C2C0400,  //  006F  MOVE	R11	R2
      0x5C300600,  //  0070  MOVE	R12	R3
      0x7C200800,  //  0071  CALL	R8	4
      0x80041000,  //  0072  RET	1	R8
      0x80000000,  //  0073  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_parse_configuration,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_shutter_index),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(shadow_shutter_inverted),
    }),
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C080301,  //  0000  GETMET	R2	R1	K1
      0x88100102,  //  0001  GETMBR	R4	R0	K2
      0x7C080400,  //  0002  CALL	R2	2
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C080403,  //  0006  EQ	R2	R2	R3
      0x780A0000,  //  0007  JMPF	R2	#0009
      0x90020103,  //  0008  SETMBR	R0	K0	K3
      0x5409FFFE,  //  0009  LDINT	R2	-1
      0x90020802,  //  000A  SETMBR	R0	K4	R2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_inverted
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_update_inverted,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(shadow_shutter_inverted),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(cmd),
    /* K3   */  be_nested_str_weak(Status_X2013),
    /* K4   */  be_nested_str_weak(contains),
    /* K5   */  be_nested_str_weak(StatusSHT),
    /* K6   */  be_nested_str_weak(find),
    /* K7   */  be_nested_str_weak(SHT),
    /* K8   */  be_nested_str_weak(tasmota_shutter_index),
    /* K9   */  be_nested_str_weak(Opt),
    /* K10  */  be_const_int(1),
    }),
    be_str_weak(update_inverted),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x5409FFFE,  //  0001  LDINT	R2	-1
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806001F,  //  0003  JMPF	R1	#0024
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x580C0003,  //  0006  LDCONST	R3	K3
      0x50100200,  //  0007  LDBOOL	R4	1	0
      0x7C040600,  //  0008  CALL	R1	3
      0x8C080304,  //  0009  GETMET	R2	R1	K4
      0x58100005,  //  000A  LDCONST	R4	K5
      0x7C080400,  //  000B  CALL	R2	2
      0x780A0016,  //  000C  JMPF	R2	#0024
      0x94040305,  //  000D  GETIDX	R1	R1	K5
      0x8C080306,  //  000E  GETMET	R2	R1	K6
      0x60100008,  //  000F  GETGBL	R4	G8
      0x88140108,  //  0010  GETMBR	R5	R0	K8
      0x7C100200,  //  0011  CALL	R4	1
      0x00120E04,  //  0012  ADD	R4	K7	R4
      0x60140013,  //  0013  GETGBL	R5	G19
      0x7C140000,  //  0014  CALL	R5	0
      0x7C080600,  //  0015  CALL	R2	3
      0x8C080506,  //  0016  GETMET	R2	R2	K6
      0x58100009,  //  0017  LDCONST	R4	K9
      0x7C080400,  //  0018  CALL	R2	2
      0x4C0C0000,  //  0019  LDNIL	R3
      0x200C0403,  //  001A  NE	R3	R2	R3
      0x780E0007,  //  001B  JMPF	R3	#0024
      0x600C0009,  //  001C  GETGBL	R3	G9
      0x6010000C,  //  001D  GETGBL	R4	G12
      0x5C140400,  //  001E  MOVE	R5	R2
      0x7C100200,  //  001F  CALL	R4	1
      0x0410090A,  //  0020  SUB	R4	R4	K10
      0x94100404,  //  0021  GETIDX	R4	R2	R4
      0x7C0C0200,  //  0022  CALL	R3	1
      0x90020003,  //  0023  SETMBR	R0	K0	R3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_Shutter__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(_X3Clambda_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040009,  //  0000  GETGBL	R1	G9
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Shutter
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Shutter,
    6,
    &be_class_Matter_Plugin_Device,
    be_nested_map(19,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, 4), be_const_closure(Matter_Plugin_Shutter_read_attribute_closure) },
        { be_const_key_weak(shadow_shutter_pos, 18), be_const_var(1) },
        { be_const_key_weak(tasmota_shutter_index, -1), be_const_var(0) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(Matter_Plugin_Shutter_parse_sensors_closure) },
        { be_const_key_weak(shadow_shutter_direction, -1), be_const_var(4) },
        { be_const_key_weak(shadow_shutter_target, -1), be_const_var(2) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Shutter_invoke_request_closure) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Enter_X20Relay_X3Cx_X3E_X20number) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(258, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(5),
        be_const_int(7),
        be_const_int(10),
        be_const_int(11),
        be_const_int(13),
        be_const_int(14),
        be_const_int(23),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(update_shadow, 5), be_const_closure(Matter_Plugin_Shutter_update_shadow_closure) },
        { be_const_key_weak(NAME, 6), be_nested_str_weak(Shutter) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(shutter) },
        { be_const_key_weak(shadow_shutter_inverted, -1), be_const_var(5) },
        { be_const_key_weak(ARG, 7), be_nested_str_weak(shutter) },
        { be_const_key_weak(shadow_shutter_tilt, 8), be_const_var(3) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(Matter_Plugin_Shutter_parse_configuration_closure) },
        { be_const_key_weak(update_inverted, -1), be_const_closure(Matter_Plugin_Shutter_update_inverted_closure) },
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(Matter_Plugin_Shutter__X3Clambda_X3E_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(514, -1), be_const_int(2) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Shutter)
);
/*******************************************************************/

void be_load_Matter_Plugin_Shutter_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Shutter);
    be_setglobal(vm, "Matter_Plugin_Shutter");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
