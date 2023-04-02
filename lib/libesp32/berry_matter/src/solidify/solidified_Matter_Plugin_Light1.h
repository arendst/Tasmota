/* Solidification of Matter_Plugin_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light1;

/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light1_update_shadow,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(bri),
    /* K4   */  be_nested_str_weak(power),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(scale_uint),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(shadow_bri),
    /* K9   */  be_nested_str_weak(shadow_onoff),
    /* K10  */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x58140003,  //  0004  LDCONST	R5	K3
      0x4C180000,  //  0005  LDNIL	R6
      0x7C0C0600,  //  0006  CALL	R3	3
      0x8C100502,  //  0007  GETMET	R4	R2	K2
      0x58180004,  //  0008  LDCONST	R6	K4
      0x4C1C0000,  //  0009  LDNIL	R7
      0x7C100600,  //  000A  CALL	R4	3
      0x4C140000,  //  000B  LDNIL	R5
      0x20140605,  //  000C  NE	R5	R3	R5
      0x78160009,  //  000D  JMPF	R5	#0018
      0xB8160A00,  //  000E  GETNGBL	R5	K5
      0x8C140B06,  //  000F  GETMET	R5	R5	K6
      0x5C1C0600,  //  0010  MOVE	R7	R3
      0x58200007,  //  0011  LDCONST	R8	K7
      0x542600FE,  //  0012  LDINT	R9	255
      0x58280007,  //  0013  LDCONST	R10	K7
      0x542E00FD,  //  0014  LDINT	R11	254
      0x7C140C00,  //  0015  CALL	R5	6
      0x5C0C0A00,  //  0016  MOVE	R3	R5
      0x70020000,  //  0017  JMP		#0019
      0x880C0108,  //  0018  GETMBR	R3	R0	K8
      0x88140109,  //  0019  GETMBR	R5	R0	K9
      0x20140805,  //  001A  NE	R5	R4	R5
      0x78160005,  //  001B  JMPF	R5	#0022
      0x8C14010A,  //  001C  GETMET	R5	R0	K10
      0x4C1C0000,  //  001D  LDNIL	R7
      0x54220005,  //  001E  LDINT	R8	6
      0x58240007,  //  001F  LDCONST	R9	K7
      0x7C140800,  //  0020  CALL	R5	4
      0x90021204,  //  0021  SETMBR	R0	K9	R4
      0x88140108,  //  0022  GETMBR	R5	R0	K8
      0x20140605,  //  0023  NE	R5	R3	R5
      0x78160005,  //  0024  JMPF	R5	#002B
      0x8C14010A,  //  0025  GETMET	R5	R0	K10
      0x4C1C0000,  //  0026  LDNIL	R7
      0x54220007,  //  0027  LDINT	R8	8
      0x58240007,  //  0028  LDCONST	R9	K7
      0x7C140800,  //  0029  CALL	R5	4
      0x90021003,  //  002A  SETMBR	R0	K8	R3
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Plugin_Light1_every_second,   /* name */
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
    /* K0   */  be_nested_str_weak(update_shadow),
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
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light1_invoke_request,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_const_int(3),
    /* K6   */  be_const_int(0),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(Matter_TLV_struct),
    /* K9   */  be_nested_str_weak(add_TLV),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_nested_str_weak(set),
    /* K12  */  be_nested_str_weak(power),
    /* K13  */  be_nested_str_weak(update_shadow),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(shadow_onoff),
    /* K16  */  be_nested_str_weak(findsubval),
    /* K17  */  be_nested_str_weak(tasmota),
    /* K18  */  be_nested_str_weak(scale_uint),
    /* K19  */  be_nested_str_weak(bri),
    /* K20  */  be_nested_str_weak(log),
    /* K21  */  be_nested_str_weak(bri_X3A),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[182]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x1C200D05,  //  0005  EQ	R8	R6	K5
      0x78220016,  //  0006  JMPF	R8	#001E
      0x1C200F06,  //  0007  EQ	R8	R7	K6
      0x78220002,  //  0008  JMPF	R8	#000C
      0x50200200,  //  0009  LDBOOL	R8	1	0
      0x80041000,  //  000A  RET	1	R8
      0x70020010,  //  000B  JMP		#001D
      0x1C200F07,  //  000C  EQ	R8	R7	K7
      0x78220009,  //  000D  JMPF	R8	#0018
      0x8C200B08,  //  000E  GETMET	R8	R5	K8
      0x7C200200,  //  000F  CALL	R8	1
      0x8C241109,  //  0010  GETMET	R9	R8	K9
      0x582C0006,  //  0011  LDCONST	R11	K6
      0x88300B0A,  //  0012  GETMBR	R12	R5	K10
      0x58340006,  //  0013  LDCONST	R13	K6
      0x7C240800,  //  0014  CALL	R9	4
      0x900E0906,  //  0015  SETMBR	R3	K4	K6
      0x80041000,  //  0016  RET	1	R8
      0x70020004,  //  0017  JMP		#001D
      0x5422003F,  //  0018  LDINT	R8	64
      0x1C200E08,  //  0019  EQ	R8	R7	R8
      0x78220001,  //  001A  JMPF	R8	#001D
      0x50200200,  //  001B  LDBOOL	R8	1	0
      0x80041000,  //  001C  RET	1	R8
      0x70020096,  //  001D  JMP		#00B5
      0x54220003,  //  001E  LDINT	R8	4
      0x1C200C08,  //  001F  EQ	R8	R6	R8
      0x78220002,  //  0020  JMPF	R8	#0024
      0x50200200,  //  0021  LDBOOL	R8	1	0
      0x80041000,  //  0022  RET	1	R8
      0x70020090,  //  0023  JMP		#00B5
      0x54220004,  //  0024  LDINT	R8	5
      0x1C200C08,  //  0025  EQ	R8	R6	R8
      0x78220002,  //  0026  JMPF	R8	#002A
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x7002008A,  //  0029  JMP		#00B5
      0x54220005,  //  002A  LDINT	R8	6
      0x1C200C08,  //  002B  EQ	R8	R6	R8
      0x78220029,  //  002C  JMPF	R8	#0057
      0x1C200F06,  //  002D  EQ	R8	R7	K6
      0x7822000A,  //  002E  JMPF	R8	#003A
      0x8C20090B,  //  002F  GETMET	R8	R4	K11
      0x60280013,  //  0030  GETGBL	R10	G19
      0x7C280000,  //  0031  CALL	R10	0
      0x502C0000,  //  0032  LDBOOL	R11	0	0
      0x982A180B,  //  0033  SETIDX	R10	K12	R11
      0x7C200400,  //  0034  CALL	R8	2
      0x8C20010D,  //  0035  GETMET	R8	R0	K13
      0x7C200200,  //  0036  CALL	R8	1
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x80041000,  //  0038  RET	1	R8
      0x7002001B,  //  0039  JMP		#0056
      0x1C200F07,  //  003A  EQ	R8	R7	K7
      0x7822000A,  //  003B  JMPF	R8	#0047
      0x8C20090B,  //  003C  GETMET	R8	R4	K11
      0x60280013,  //  003D  GETGBL	R10	G19
      0x7C280000,  //  003E  CALL	R10	0
      0x502C0200,  //  003F  LDBOOL	R11	1	0
      0x982A180B,  //  0040  SETIDX	R10	K12	R11
      0x7C200400,  //  0041  CALL	R8	2
      0x8C20010D,  //  0042  GETMET	R8	R0	K13
      0x7C200200,  //  0043  CALL	R8	1
      0x50200200,  //  0044  LDBOOL	R8	1	0
      0x80041000,  //  0045  RET	1	R8
      0x7002000E,  //  0046  JMP		#0056
      0x1C200F0E,  //  0047  EQ	R8	R7	K14
      0x7822000C,  //  0048  JMPF	R8	#0056
      0x8C20090B,  //  0049  GETMET	R8	R4	K11
      0x60280013,  //  004A  GETGBL	R10	G19
      0x7C280000,  //  004B  CALL	R10	0
      0x882C010F,  //  004C  GETMBR	R11	R0	K15
      0x782E0000,  //  004D  JMPF	R11	#004F
      0x502C0001,  //  004E  LDBOOL	R11	0	1
      0x502C0200,  //  004F  LDBOOL	R11	1	0
      0x982A180B,  //  0050  SETIDX	R10	K12	R11
      0x7C200400,  //  0051  CALL	R8	2
      0x8C20010D,  //  0052  GETMET	R8	R0	K13
      0x7C200200,  //  0053  CALL	R8	1
      0x50200200,  //  0054  LDBOOL	R8	1	0
      0x80041000,  //  0055  RET	1	R8
      0x7002005D,  //  0056  JMP		#00B5
      0x54220007,  //  0057  LDINT	R8	8
      0x1C200C08,  //  0058  EQ	R8	R6	R8
      0x7822005A,  //  0059  JMPF	R8	#00B5
      0x1C200F06,  //  005A  EQ	R8	R7	K6
      0x78220019,  //  005B  JMPF	R8	#0076
      0x8C200510,  //  005C  GETMET	R8	R2	K16
      0x58280006,  //  005D  LDCONST	R10	K6
      0x7C200400,  //  005E  CALL	R8	2
      0xB8262200,  //  005F  GETNGBL	R9	K17
      0x8C241312,  //  0060  GETMET	R9	R9	K18
      0x5C2C1000,  //  0061  MOVE	R11	R8
      0x58300006,  //  0062  LDCONST	R12	K6
      0x543600FD,  //  0063  LDINT	R13	254
      0x58380006,  //  0064  LDCONST	R14	K6
      0x543E00FE,  //  0065  LDINT	R15	255
      0x7C240C00,  //  0066  CALL	R9	6
      0x8C28090B,  //  0067  GETMET	R10	R4	K11
      0x60300013,  //  0068  GETGBL	R12	G19
      0x7C300000,  //  0069  CALL	R12	0
      0x98322609,  //  006A  SETIDX	R12	K19	R9
      0x7C280400,  //  006B  CALL	R10	2
      0x8C28010D,  //  006C  GETMET	R10	R0	K13
      0x7C280200,  //  006D  CALL	R10	1
      0x60280008,  //  006E  GETGBL	R10	G8
      0x5C2C1000,  //  006F  MOVE	R11	R8
      0x7C280200,  //  0070  CALL	R10	1
      0x002A2A0A,  //  0071  ADD	R10	K21	R10
      0x900E280A,  //  0072  SETMBR	R3	K20	R10
      0x50280200,  //  0073  LDBOOL	R10	1	0
      0x80041400,  //  0074  RET	1	R10
      0x7002003E,  //  0075  JMP		#00B5
      0x1C200F07,  //  0076  EQ	R8	R7	K7
      0x78220002,  //  0077  JMPF	R8	#007B
      0x50200200,  //  0078  LDBOOL	R8	1	0
      0x80041000,  //  0079  RET	1	R8
      0x70020039,  //  007A  JMP		#00B5
      0x1C200F0E,  //  007B  EQ	R8	R7	K14
      0x78220002,  //  007C  JMPF	R8	#0080
      0x50200200,  //  007D  LDBOOL	R8	1	0
      0x80041000,  //  007E  RET	1	R8
      0x70020034,  //  007F  JMP		#00B5
      0x1C200F05,  //  0080  EQ	R8	R7	K5
      0x78220002,  //  0081  JMPF	R8	#0085
      0x50200200,  //  0082  LDBOOL	R8	1	0
      0x80041000,  //  0083  RET	1	R8
      0x7002002F,  //  0084  JMP		#00B5
      0x54220003,  //  0085  LDINT	R8	4
      0x1C200E08,  //  0086  EQ	R8	R7	R8
      0x7822001B,  //  0087  JMPF	R8	#00A4
      0x8C200510,  //  0088  GETMET	R8	R2	K16
      0x58280006,  //  0089  LDCONST	R10	K6
      0x7C200400,  //  008A  CALL	R8	2
      0xB8262200,  //  008B  GETNGBL	R9	K17
      0x8C241312,  //  008C  GETMET	R9	R9	K18
      0x5C2C1000,  //  008D  MOVE	R11	R8
      0x58300006,  //  008E  LDCONST	R12	K6
      0x543600FD,  //  008F  LDINT	R13	254
      0x58380006,  //  0090  LDCONST	R14	K6
      0x543E00FE,  //  0091  LDINT	R15	255
      0x7C240C00,  //  0092  CALL	R9	6
      0x24281306,  //  0093  GT	R10	R9	K6
      0x8C2C090B,  //  0094  GETMET	R11	R4	K11
      0x60340013,  //  0095  GETGBL	R13	G19
      0x7C340000,  //  0096  CALL	R13	0
      0x98362609,  //  0097  SETIDX	R13	K19	R9
      0x9836180A,  //  0098  SETIDX	R13	K12	R10
      0x7C2C0400,  //  0099  CALL	R11	2
      0x8C2C010D,  //  009A  GETMET	R11	R0	K13
      0x7C2C0200,  //  009B  CALL	R11	1
      0x602C0008,  //  009C  GETGBL	R11	G8
      0x5C301000,  //  009D  MOVE	R12	R8
      0x7C2C0200,  //  009E  CALL	R11	1
      0x002E2A0B,  //  009F  ADD	R11	K21	R11
      0x900E280B,  //  00A0  SETMBR	R3	K20	R11
      0x502C0200,  //  00A1  LDBOOL	R11	1	0
      0x80041600,  //  00A2  RET	1	R11
      0x70020010,  //  00A3  JMP		#00B5
      0x54220004,  //  00A4  LDINT	R8	5
      0x1C200E08,  //  00A5  EQ	R8	R7	R8
      0x78220002,  //  00A6  JMPF	R8	#00AA
      0x50200200,  //  00A7  LDBOOL	R8	1	0
      0x80041000,  //  00A8  RET	1	R8
      0x7002000A,  //  00A9  JMP		#00B5
      0x54220005,  //  00AA  LDINT	R8	6
      0x1C200E08,  //  00AB  EQ	R8	R7	R8
      0x78220002,  //  00AC  JMPF	R8	#00B0
      0x50200200,  //  00AD  LDBOOL	R8	1	0
      0x80041000,  //  00AE  RET	1	R8
      0x70020004,  //  00AF  JMP		#00B5
      0x54220006,  //  00B0  LDINT	R8	7
      0x1C200E08,  //  00B1  EQ	R8	R7	R8
      0x78220001,  //  00B2  JMPF	R8	#00B5
      0x50200200,  //  00B3  LDBOOL	R8	1	0
      0x80041000,  //  00B4  RET	1	R8
      0x80000000,  //  00B5  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light1_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(3),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(U2),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(U1),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(BOOL),
    /* K13  */  be_nested_str_weak(shadow_onoff),
    /* K14  */  be_nested_str_weak(shadow_bri),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[189]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x1C1C0B05,  //  0005  EQ	R7	R5	K5
      0x781E0021,  //  0006  JMPF	R7	#0029
      0x1C1C0D06,  //  0007  EQ	R7	R6	K6
      0x781E0005,  //  0008  JMPF	R7	#000F
      0x8C1C0907,  //  0009  GETMET	R7	R4	K7
      0x88240908,  //  000A  GETMBR	R9	R4	K8
      0x58280006,  //  000B  LDCONST	R10	K6
      0x7C1C0600,  //  000C  CALL	R7	3
      0x80040E00,  //  000D  RET	1	R7
      0x70020018,  //  000E  JMP		#0028
      0x1C1C0D09,  //  000F  EQ	R7	R6	K9
      0x781E0005,  //  0010  JMPF	R7	#0017
      0x8C1C0907,  //  0011  GETMET	R7	R4	K7
      0x8824090A,  //  0012  GETMBR	R9	R4	K10
      0x58280006,  //  0013  LDCONST	R10	K6
      0x7C1C0600,  //  0014  CALL	R7	3
      0x80040E00,  //  0015  RET	1	R7
      0x70020010,  //  0016  JMP		#0028
      0x541EFFFB,  //  0017  LDINT	R7	65532
      0x1C1C0C07,  //  0018  EQ	R7	R6	R7
      0x781E0005,  //  0019  JMPF	R7	#0020
      0x8C1C0907,  //  001A  GETMET	R7	R4	K7
      0x8824090B,  //  001B  GETMBR	R9	R4	K11
      0x58280006,  //  001C  LDCONST	R10	K6
      0x7C1C0600,  //  001D  CALL	R7	3
      0x80040E00,  //  001E  RET	1	R7
      0x70020007,  //  001F  JMP		#0028
      0x541EFFFC,  //  0020  LDINT	R7	65533
      0x1C1C0C07,  //  0021  EQ	R7	R6	R7
      0x781E0004,  //  0022  JMPF	R7	#0028
      0x8C1C0907,  //  0023  GETMET	R7	R4	K7
      0x8824090B,  //  0024  GETMBR	R9	R4	K11
      0x542A0003,  //  0025  LDINT	R10	4
      0x7C1C0600,  //  0026  CALL	R7	3
      0x80040E00,  //  0027  RET	1	R7
      0x70020092,  //  0028  JMP		#00BC
      0x541E0003,  //  0029  LDINT	R7	4
      0x1C1C0A07,  //  002A  EQ	R7	R5	R7
      0x781E0016,  //  002B  JMPF	R7	#0043
      0x1C1C0D06,  //  002C  EQ	R7	R6	K6
      0x781E0002,  //  002D  JMPF	R7	#0031
      0x4C1C0000,  //  002E  LDNIL	R7
      0x80040E00,  //  002F  RET	1	R7
      0x70020010,  //  0030  JMP		#0042
      0x541EFFFB,  //  0031  LDINT	R7	65532
      0x1C1C0C07,  //  0032  EQ	R7	R6	R7
      0x781E0005,  //  0033  JMPF	R7	#003A
      0x8C1C0907,  //  0034  GETMET	R7	R4	K7
      0x8824090B,  //  0035  GETMBR	R9	R4	K11
      0x58280006,  //  0036  LDCONST	R10	K6
      0x7C1C0600,  //  0037  CALL	R7	3
      0x80040E00,  //  0038  RET	1	R7
      0x70020007,  //  0039  JMP		#0042
      0x541EFFFC,  //  003A  LDINT	R7	65533
      0x1C1C0C07,  //  003B  EQ	R7	R6	R7
      0x781E0004,  //  003C  JMPF	R7	#0042
      0x8C1C0907,  //  003D  GETMET	R7	R4	K7
      0x8824090B,  //  003E  GETMBR	R9	R4	K11
      0x542A0003,  //  003F  LDINT	R10	4
      0x7C1C0600,  //  0040  CALL	R7	3
      0x80040E00,  //  0041  RET	1	R7
      0x70020078,  //  0042  JMP		#00BC
      0x541E0004,  //  0043  LDINT	R7	5
      0x1C1C0A07,  //  0044  EQ	R7	R5	R7
      0x781E0011,  //  0045  JMPF	R7	#0058
      0x541EFFFB,  //  0046  LDINT	R7	65532
      0x1C1C0C07,  //  0047  EQ	R7	R6	R7
      0x781E0005,  //  0048  JMPF	R7	#004F
      0x8C1C0907,  //  0049  GETMET	R7	R4	K7
      0x8824090B,  //  004A  GETMBR	R9	R4	K11
      0x58280006,  //  004B  LDCONST	R10	K6
      0x7C1C0600,  //  004C  CALL	R7	3
      0x80040E00,  //  004D  RET	1	R7
      0x70020007,  //  004E  JMP		#0057
      0x541EFFFC,  //  004F  LDINT	R7	65533
      0x1C1C0C07,  //  0050  EQ	R7	R6	R7
      0x781E0004,  //  0051  JMPF	R7	#0057
      0x8C1C0907,  //  0052  GETMET	R7	R4	K7
      0x8824090B,  //  0053  GETMBR	R9	R4	K11
      0x542A0003,  //  0054  LDINT	R10	4
      0x7C1C0600,  //  0055  CALL	R7	3
      0x80040E00,  //  0056  RET	1	R7
      0x70020063,  //  0057  JMP		#00BC
      0x541E0005,  //  0058  LDINT	R7	6
      0x1C1C0A07,  //  0059  EQ	R7	R5	R7
      0x781E0019,  //  005A  JMPF	R7	#0075
      0x1C1C0D06,  //  005B  EQ	R7	R6	K6
      0x781E0005,  //  005C  JMPF	R7	#0063
      0x8C1C0907,  //  005D  GETMET	R7	R4	K7
      0x8824090C,  //  005E  GETMBR	R9	R4	K12
      0x8828010D,  //  005F  GETMBR	R10	R0	K13
      0x7C1C0600,  //  0060  CALL	R7	3
      0x80040E00,  //  0061  RET	1	R7
      0x70020010,  //  0062  JMP		#0074
      0x541EFFFB,  //  0063  LDINT	R7	65532
      0x1C1C0C07,  //  0064  EQ	R7	R6	R7
      0x781E0005,  //  0065  JMPF	R7	#006C
      0x8C1C0907,  //  0066  GETMET	R7	R4	K7
      0x8824090B,  //  0067  GETMBR	R9	R4	K11
      0x58280006,  //  0068  LDCONST	R10	K6
      0x7C1C0600,  //  0069  CALL	R7	3
      0x80040E00,  //  006A  RET	1	R7
      0x70020007,  //  006B  JMP		#0074
      0x541EFFFC,  //  006C  LDINT	R7	65533
      0x1C1C0C07,  //  006D  EQ	R7	R6	R7
      0x781E0004,  //  006E  JMPF	R7	#0074
      0x8C1C0907,  //  006F  GETMET	R7	R4	K7
      0x8824090B,  //  0070  GETMBR	R9	R4	K11
      0x542A0003,  //  0071  LDINT	R10	4
      0x7C1C0600,  //  0072  CALL	R7	3
      0x80040E00,  //  0073  RET	1	R7
      0x70020046,  //  0074  JMP		#00BC
      0x541E0007,  //  0075  LDINT	R7	8
      0x1C1C0A07,  //  0076  EQ	R7	R5	R7
      0x781E003B,  //  0077  JMPF	R7	#00B4
      0x1C1C0D06,  //  0078  EQ	R7	R6	K6
      0x781E0005,  //  0079  JMPF	R7	#0080
      0x8C1C0907,  //  007A  GETMET	R7	R4	K7
      0x8824090A,  //  007B  GETMBR	R9	R4	K10
      0x8828010E,  //  007C  GETMBR	R10	R0	K14
      0x7C1C0600,  //  007D  CALL	R7	3
      0x80040E00,  //  007E  RET	1	R7
      0x70020032,  //  007F  JMP		#00B3
      0x1C1C0D0F,  //  0080  EQ	R7	R6	K15
      0x781E0005,  //  0081  JMPF	R7	#0088
      0x8C1C0907,  //  0082  GETMET	R7	R4	K7
      0x8824090A,  //  0083  GETMBR	R9	R4	K10
      0x58280006,  //  0084  LDCONST	R10	K6
      0x7C1C0600,  //  0085  CALL	R7	3
      0x80040E00,  //  0086  RET	1	R7
      0x7002002A,  //  0087  JMP		#00B3
      0x1C1C0D05,  //  0088  EQ	R7	R6	K5
      0x781E0005,  //  0089  JMPF	R7	#0090
      0x8C1C0907,  //  008A  GETMET	R7	R4	K7
      0x8824090A,  //  008B  GETMBR	R9	R4	K10
      0x542A00FD,  //  008C  LDINT	R10	254
      0x7C1C0600,  //  008D  CALL	R7	3
      0x80040E00,  //  008E  RET	1	R7
      0x70020022,  //  008F  JMP		#00B3
      0x541E000E,  //  0090  LDINT	R7	15
      0x1C1C0C07,  //  0091  EQ	R7	R6	R7
      0x781E0005,  //  0092  JMPF	R7	#0099
      0x8C1C0907,  //  0093  GETMET	R7	R4	K7
      0x8824090A,  //  0094  GETMBR	R9	R4	K10
      0x58280006,  //  0095  LDCONST	R10	K6
      0x7C1C0600,  //  0096  CALL	R7	3
      0x80040E00,  //  0097  RET	1	R7
      0x70020019,  //  0098  JMP		#00B3
      0x541E0010,  //  0099  LDINT	R7	17
      0x1C1C0C07,  //  009A  EQ	R7	R6	R7
      0x781E0005,  //  009B  JMPF	R7	#00A2
      0x8C1C0907,  //  009C  GETMET	R7	R4	K7
      0x8824090A,  //  009D  GETMBR	R9	R4	K10
      0x8828010E,  //  009E  GETMBR	R10	R0	K14
      0x7C1C0600,  //  009F  CALL	R7	3
      0x80040E00,  //  00A0  RET	1	R7
      0x70020010,  //  00A1  JMP		#00B3
      0x541EFFFB,  //  00A2  LDINT	R7	65532
      0x1C1C0C07,  //  00A3  EQ	R7	R6	R7
      0x781E0005,  //  00A4  JMPF	R7	#00AB
      0x8C1C0907,  //  00A5  GETMET	R7	R4	K7
      0x8824090B,  //  00A6  GETMBR	R9	R4	K11
      0x58280009,  //  00A7  LDCONST	R10	K9
      0x7C1C0600,  //  00A8  CALL	R7	3
      0x80040E00,  //  00A9  RET	1	R7
      0x70020007,  //  00AA  JMP		#00B3
      0x541EFFFC,  //  00AB  LDINT	R7	65533
      0x1C1C0C07,  //  00AC  EQ	R7	R6	R7
      0x781E0004,  //  00AD  JMPF	R7	#00B3
      0x8C1C0907,  //  00AE  GETMET	R7	R4	K7
      0x8824090B,  //  00AF  GETMBR	R9	R4	K11
      0x542A0004,  //  00B0  LDINT	R10	5
      0x7C1C0600,  //  00B1  CALL	R7	3
      0x80040E00,  //  00B2  RET	1	R7
      0x70020007,  //  00B3  JMP		#00BC
      0x601C0003,  //  00B4  GETGBL	R7	G3
      0x5C200000,  //  00B5  MOVE	R8	R0
      0x7C1C0200,  //  00B6  CALL	R7	1
      0x8C1C0F10,  //  00B7  GETMET	R7	R7	K16
      0x5C240200,  //  00B8  MOVE	R9	R1
      0x5C280400,  //  00B9  MOVE	R10	R2
      0x7C1C0600,  //  00BA  CALL	R7	3
      0x80040E00,  //  00BB  RET	1	R7
      0x80000000,  //  00BC  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light1_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_bri),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x90020302,  //  0007  SETMBR	R0	K1	K2
      0x500C0000,  //  0008  LDBOOL	R3	0	0
      0x90020603,  //  0009  SETMBR	R0	K3	R3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light1
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Light1,
    2,
    &be_class_Matter_Plugin,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light1_init_closure) },
        { be_const_key_weak(shadow_onoff, -1), be_const_var(1) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(257, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Plugin_Light1_every_second_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(8, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(2),
        be_const_int(3),
        be_const_int(15),
        be_const_int(17),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light1_invoke_request_closure) },
        { be_const_key_weak(read_attribute, 0), be_const_closure(Matter_Plugin_Light1_read_attribute_closure) },
        { be_const_key_weak(update_shadow, 8), be_const_closure(Matter_Plugin_Light1_update_shadow_closure) },
        { be_const_key_weak(shadow_bri, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Plugin_Light1)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light1_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light1);
    be_setglobal(vm, "Matter_Plugin_Light1");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
