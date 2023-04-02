/* Solidification of Matter_Plugin_Light2.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light2;

/********************************************************************
** Solidified function: update_ct_minmax
********************************************************************/
be_local_closure(Matter_Plugin_Light2_update_ct_minmax,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(get_option),
    /* K2   */  be_nested_str_weak(ct_min),
    /* K3   */  be_nested_str_weak(ct_max),
    }),
    be_str_weak(update_ct_minmax),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x540E0051,  //  0002  LDINT	R3	82
      0x7C040400,  //  0003  CALL	R1	2
      0x78060001,  //  0004  JMPF	R1	#0007
      0x540A00C7,  //  0005  LDINT	R2	200
      0x70020000,  //  0006  JMP		#0008
      0x540A0098,  //  0007  LDINT	R2	153
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x540A017B,  //  000A  LDINT	R2	380
      0x70020000,  //  000B  JMP		#000D
      0x540A01F3,  //  000C  LDINT	R2	500
      0x90020602,  //  000D  SETMBR	R0	K3	R2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light2_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_bri),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(shadow_ct),
    /* K4   */  be_nested_str_weak(shadow_onoff),
    /* K5   */  be_nested_str_weak(update_ct_minmax),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x90020302,  //  0007  SETMBR	R0	K1	K2
      0x540E0144,  //  0008  LDINT	R3	325
      0x90020603,  //  0009  SETMBR	R0	K3	R3
      0x500C0000,  //  000A  LDBOOL	R3	0	0
      0x90020803,  //  000B  SETMBR	R0	K4	R3
      0x8C0C0105,  //  000C  GETMET	R3	R0	K5
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light2_invoke_request,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
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
    /* K22  */  be_nested_str_weak(ct_min),
    /* K23  */  be_nested_str_weak(ct_max),
    /* K24  */  be_nested_str_weak(ct),
    /* K25  */  be_nested_str_weak(ct_X3A),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[232]) {  /* code */
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
      0x700200C8,  //  001D  JMP		#00E7
      0x54220003,  //  001E  LDINT	R8	4
      0x1C200C08,  //  001F  EQ	R8	R6	R8
      0x78220002,  //  0020  JMPF	R8	#0024
      0x50200200,  //  0021  LDBOOL	R8	1	0
      0x80041000,  //  0022  RET	1	R8
      0x700200C2,  //  0023  JMP		#00E7
      0x54220004,  //  0024  LDINT	R8	5
      0x1C200C08,  //  0025  EQ	R8	R6	R8
      0x78220002,  //  0026  JMPF	R8	#002A
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x700200BC,  //  0029  JMP		#00E7
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
      0x7002008F,  //  0056  JMP		#00E7
      0x54220007,  //  0057  LDINT	R8	8
      0x1C200C08,  //  0058  EQ	R8	R6	R8
      0x7822005B,  //  0059  JMPF	R8	#00B6
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
      0x70020030,  //  00B5  JMP		#00E7
      0x542202FF,  //  00B6  LDINT	R8	768
      0x1C200C08,  //  00B7  EQ	R8	R6	R8
      0x7822002D,  //  00B8  JMPF	R8	#00E7
      0x54220009,  //  00B9  LDINT	R8	10
      0x1C200E08,  //  00BA  EQ	R8	R7	R8
      0x78220019,  //  00BB  JMPF	R8	#00D6
      0x8C200510,  //  00BC  GETMET	R8	R2	K16
      0x58280006,  //  00BD  LDCONST	R10	K6
      0x7C200400,  //  00BE  CALL	R8	2
      0x88240116,  //  00BF  GETMBR	R9	R0	K22
      0x14241009,  //  00C0  LT	R9	R8	R9
      0x78260000,  //  00C1  JMPF	R9	#00C3
      0x88200116,  //  00C2  GETMBR	R8	R0	K22
      0x88240117,  //  00C3  GETMBR	R9	R0	K23
      0x24241009,  //  00C4  GT	R9	R8	R9
      0x78260000,  //  00C5  JMPF	R9	#00C7
      0x88200117,  //  00C6  GETMBR	R8	R0	K23
      0x8C24090B,  //  00C7  GETMET	R9	R4	K11
      0x602C0013,  //  00C8  GETGBL	R11	G19
      0x7C2C0000,  //  00C9  CALL	R11	0
      0x982E3008,  //  00CA  SETIDX	R11	K24	R8
      0x7C240400,  //  00CB  CALL	R9	2
      0x8C24010D,  //  00CC  GETMET	R9	R0	K13
      0x7C240200,  //  00CD  CALL	R9	1
      0x60240008,  //  00CE  GETGBL	R9	G8
      0x5C281000,  //  00CF  MOVE	R10	R8
      0x7C240200,  //  00D0  CALL	R9	1
      0x00263209,  //  00D1  ADD	R9	K25	R9
      0x900E2809,  //  00D2  SETMBR	R3	K20	R9
      0x50240200,  //  00D3  LDBOOL	R9	1	0
      0x80041200,  //  00D4  RET	1	R9
      0x70020010,  //  00D5  JMP		#00E7
      0x54220046,  //  00D6  LDINT	R8	71
      0x1C200E08,  //  00D7  EQ	R8	R7	R8
      0x78220002,  //  00D8  JMPF	R8	#00DC
      0x50200200,  //  00D9  LDBOOL	R8	1	0
      0x80041000,  //  00DA  RET	1	R8
      0x7002000A,  //  00DB  JMP		#00E7
      0x5422004A,  //  00DC  LDINT	R8	75
      0x1C200E08,  //  00DD  EQ	R8	R7	R8
      0x78220002,  //  00DE  JMPF	R8	#00E2
      0x50200200,  //  00DF  LDBOOL	R8	1	0
      0x80041000,  //  00E0  RET	1	R8
      0x70020004,  //  00E1  JMP		#00E7
      0x5422004B,  //  00E2  LDINT	R8	76
      0x1C200E08,  //  00E3  EQ	R8	R7	R8
      0x78220001,  //  00E4  JMPF	R8	#00E7
      0x50200200,  //  00E5  LDBOOL	R8	1	0
      0x80041000,  //  00E6  RET	1	R8
      0x80000000,  //  00E7  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light2_update_shadow,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(update_ct_minmax),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(power),
    /* K5   */  be_nested_str_weak(bri),
    /* K6   */  be_nested_str_weak(ct),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(scale_uint),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(shadow_bri),
    /* K11  */  be_nested_str_weak(shadow_ct),
    /* K12  */  be_nested_str_weak(shadow_onoff),
    /* K13  */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x8C0C0503,  //  0005  GETMET	R3	R2	K3
      0x58140004,  //  0006  LDCONST	R5	K4
      0x4C180000,  //  0007  LDNIL	R6
      0x7C0C0600,  //  0008  CALL	R3	3
      0x8C100503,  //  0009  GETMET	R4	R2	K3
      0x58180005,  //  000A  LDCONST	R6	K5
      0x4C1C0000,  //  000B  LDNIL	R7
      0x7C100600,  //  000C  CALL	R4	3
      0x8C140503,  //  000D  GETMET	R5	R2	K3
      0x581C0006,  //  000E  LDCONST	R7	K6
      0x4C200000,  //  000F  LDNIL	R8
      0x7C140600,  //  0010  CALL	R5	3
      0x4C180000,  //  0011  LDNIL	R6
      0x20180806,  //  0012  NE	R6	R4	R6
      0x781A0009,  //  0013  JMPF	R6	#001E
      0xB81A0E00,  //  0014  GETNGBL	R6	K7
      0x8C180D08,  //  0015  GETMET	R6	R6	K8
      0x5C200800,  //  0016  MOVE	R8	R4
      0x58240009,  //  0017  LDCONST	R9	K9
      0x542A00FE,  //  0018  LDINT	R10	255
      0x582C0009,  //  0019  LDCONST	R11	K9
      0x543200FD,  //  001A  LDINT	R12	254
      0x7C180C00,  //  001B  CALL	R6	6
      0x5C100C00,  //  001C  MOVE	R4	R6
      0x70020000,  //  001D  JMP		#001F
      0x8810010A,  //  001E  GETMBR	R4	R0	K10
      0x4C180000,  //  001F  LDNIL	R6
      0x1C180A06,  //  0020  EQ	R6	R5	R6
      0x781A0000,  //  0021  JMPF	R6	#0023
      0x8814010B,  //  0022  GETMBR	R5	R0	K11
      0x8818010C,  //  0023  GETMBR	R6	R0	K12
      0x20180606,  //  0024  NE	R6	R3	R6
      0x781A0005,  //  0025  JMPF	R6	#002C
      0x8C18010D,  //  0026  GETMET	R6	R0	K13
      0x4C200000,  //  0027  LDNIL	R8
      0x54260005,  //  0028  LDINT	R9	6
      0x58280009,  //  0029  LDCONST	R10	K9
      0x7C180800,  //  002A  CALL	R6	4
      0x90021803,  //  002B  SETMBR	R0	K12	R3
      0x8818010A,  //  002C  GETMBR	R6	R0	K10
      0x20180806,  //  002D  NE	R6	R4	R6
      0x781A0005,  //  002E  JMPF	R6	#0035
      0x8C18010D,  //  002F  GETMET	R6	R0	K13
      0x4C200000,  //  0030  LDNIL	R8
      0x54260007,  //  0031  LDINT	R9	8
      0x58280009,  //  0032  LDCONST	R10	K9
      0x7C180800,  //  0033  CALL	R6	4
      0x90021404,  //  0034  SETMBR	R0	K10	R4
      0x8818010B,  //  0035  GETMBR	R6	R0	K11
      0x20180A06,  //  0036  NE	R6	R5	R6
      0x781A0005,  //  0037  JMPF	R6	#003E
      0x8C18010D,  //  0038  GETMET	R6	R0	K13
      0x4C200000,  //  0039  LDNIL	R8
      0x542602FF,  //  003A  LDINT	R9	768
      0x542A0006,  //  003B  LDINT	R10	7
      0x7C180800,  //  003C  CALL	R6	4
      0x90021605,  //  003D  SETMBR	R0	K11	R5
      0x80000000,  //  003E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Plugin_Light2_every_second,   /* name */
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
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light2_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
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
    /* K16  */  be_nested_str_weak(shadow_ct),
    /* K17  */  be_nested_str_weak(ct_min),
    /* K18  */  be_nested_str_weak(ct_max),
    /* K19  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[255]) {  /* code */
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
      0x700200D4,  //  0028  JMP		#00FE
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
      0x700200BA,  //  0042  JMP		#00FE
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
      0x700200A5,  //  0057  JMP		#00FE
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
      0x70020088,  //  0074  JMP		#00FE
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
      0x70020049,  //  00B3  JMP		#00FE
      0x541E02FF,  //  00B4  LDINT	R7	768
      0x1C1C0A07,  //  00B5  EQ	R7	R5	R7
      0x781E003E,  //  00B6  JMPF	R7	#00F6
      0x541E0006,  //  00B7  LDINT	R7	7
      0x1C1C0C07,  //  00B8  EQ	R7	R6	R7
      0x781E0005,  //  00B9  JMPF	R7	#00C0
      0x8C1C0907,  //  00BA  GETMET	R7	R4	K7
      0x8824090A,  //  00BB  GETMBR	R9	R4	K10
      0x88280110,  //  00BC  GETMBR	R10	R0	K16
      0x7C1C0600,  //  00BD  CALL	R7	3
      0x80040E00,  //  00BE  RET	1	R7
      0x70020034,  //  00BF  JMP		#00F5
      0x541E0007,  //  00C0  LDINT	R7	8
      0x1C1C0C07,  //  00C1  EQ	R7	R6	R7
      0x781E0005,  //  00C2  JMPF	R7	#00C9
      0x8C1C0907,  //  00C3  GETMET	R7	R4	K7
      0x8824090A,  //  00C4  GETMBR	R9	R4	K10
      0x5828000F,  //  00C5  LDCONST	R10	K15
      0x7C1C0600,  //  00C6  CALL	R7	3
      0x80040E00,  //  00C7  RET	1	R7
      0x7002002B,  //  00C8  JMP		#00F5
      0x541E000E,  //  00C9  LDINT	R7	15
      0x1C1C0C07,  //  00CA  EQ	R7	R6	R7
      0x781E0005,  //  00CB  JMPF	R7	#00D2
      0x8C1C0907,  //  00CC  GETMET	R7	R4	K7
      0x8824090A,  //  00CD  GETMBR	R9	R4	K10
      0x58280006,  //  00CE  LDCONST	R10	K6
      0x7C1C0600,  //  00CF  CALL	R7	3
      0x80040E00,  //  00D0  RET	1	R7
      0x70020022,  //  00D1  JMP		#00F5
      0x541E400A,  //  00D2  LDINT	R7	16395
      0x1C1C0C07,  //  00D3  EQ	R7	R6	R7
      0x781E0005,  //  00D4  JMPF	R7	#00DB
      0x8C1C0907,  //  00D5  GETMET	R7	R4	K7
      0x8824090A,  //  00D6  GETMBR	R9	R4	K10
      0x88280111,  //  00D7  GETMBR	R10	R0	K17
      0x7C1C0600,  //  00D8  CALL	R7	3
      0x80040E00,  //  00D9  RET	1	R7
      0x70020019,  //  00DA  JMP		#00F5
      0x541E400B,  //  00DB  LDINT	R7	16396
      0x1C1C0C07,  //  00DC  EQ	R7	R6	R7
      0x781E0005,  //  00DD  JMPF	R7	#00E4
      0x8C1C0907,  //  00DE  GETMET	R7	R4	K7
      0x8824090A,  //  00DF  GETMBR	R9	R4	K10
      0x88280112,  //  00E0  GETMBR	R10	R0	K18
      0x7C1C0600,  //  00E1  CALL	R7	3
      0x80040E00,  //  00E2  RET	1	R7
      0x70020010,  //  00E3  JMP		#00F5
      0x541EFFFB,  //  00E4  LDINT	R7	65532
      0x1C1C0C07,  //  00E5  EQ	R7	R6	R7
      0x781E0005,  //  00E6  JMPF	R7	#00ED
      0x8C1C0907,  //  00E7  GETMET	R7	R4	K7
      0x8824090B,  //  00E8  GETMBR	R9	R4	K11
      0x542A000F,  //  00E9  LDINT	R10	16
      0x7C1C0600,  //  00EA  CALL	R7	3
      0x80040E00,  //  00EB  RET	1	R7
      0x70020007,  //  00EC  JMP		#00F5
      0x541EFFFC,  //  00ED  LDINT	R7	65533
      0x1C1C0C07,  //  00EE  EQ	R7	R6	R7
      0x781E0004,  //  00EF  JMPF	R7	#00F5
      0x8C1C0907,  //  00F0  GETMET	R7	R4	K7
      0x8824090B,  //  00F1  GETMBR	R9	R4	K11
      0x542A0004,  //  00F2  LDINT	R10	5
      0x7C1C0600,  //  00F3  CALL	R7	3
      0x80040E00,  //  00F4  RET	1	R7
      0x70020007,  //  00F5  JMP		#00FE
      0x601C0003,  //  00F6  GETGBL	R7	G3
      0x5C200000,  //  00F7  MOVE	R8	R0
      0x7C1C0200,  //  00F8  CALL	R7	1
      0x8C1C0F13,  //  00F9  GETMET	R7	R7	K19
      0x5C240200,  //  00FA  MOVE	R9	R1
      0x5C280400,  //  00FB  MOVE	R10	R2
      0x7C1C0600,  //  00FC  CALL	R7	3
      0x80040E00,  //  00FD  RET	1	R7
      0x80000000,  //  00FE  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light2
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Light2,
    5,
    &be_class_Matter_Plugin,
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(shadow_onoff, 1), be_const_var(4) },
        { be_const_key_weak(ct_min, -1), be_const_var(2) },
        { be_const_key_weak(update_ct_minmax, -1), be_const_closure(Matter_Plugin_Light2_update_ct_minmax_closure) },
        { be_const_key_weak(invoke_request, 2), be_const_closure(Matter_Plugin_Light2_invoke_request_closure) },
        { be_const_key_weak(shadow_ct, -1), be_const_var(1) },
        { be_const_key_weak(ct_max, 3), be_const_var(3) },
        { be_const_key_weak(init, 4), be_const_closure(Matter_Plugin_Light2_init_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Light2_update_shadow_closure) },
        { be_const_key_weak(shadow_bri, -1), be_const_var(0) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Plugin_Light2_every_second_closure) },
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
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light2_read_attribute_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(268, -1), be_const_int(2) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Light2)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light2_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light2);
    be_setglobal(vm, "Matter_Plugin_Light2");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
