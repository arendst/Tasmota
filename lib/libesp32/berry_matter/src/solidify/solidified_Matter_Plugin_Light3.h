/* Solidification of Matter_Plugin_Light3.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light3;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light3_init,   /* name */
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
    /* K1   */  be_nested_str_weak(shadow_hue),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(shadow_sat),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x90020302,  //  0007  SETMBR	R0	K1	K2
      0x90020702,  //  0008  SETMBR	R0	K3	K2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light3_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(shadow_hue),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(shadow_sat),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E02FF,  //  0005  LDINT	R7	768
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0057,  //  0007  JMPF	R7	#0060
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E0005,  //  0009  JMPF	R7	#0010
      0x8C1C0906,  //  000A  GETMET	R7	R4	K6
      0x88240907,  //  000B  GETMBR	R9	R4	K7
      0x88280108,  //  000C  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000D  CALL	R7	3
      0x80040E00,  //  000E  RET	1	R7
      0x7002004E,  //  000F  JMP		#005F
      0x1C1C0D09,  //  0010  EQ	R7	R6	K9
      0x781E0005,  //  0011  JMPF	R7	#0018
      0x8C1C0906,  //  0012  GETMET	R7	R4	K6
      0x88240907,  //  0013  GETMBR	R9	R4	K7
      0x8828010A,  //  0014  GETMBR	R10	R0	K10
      0x7C1C0600,  //  0015  CALL	R7	3
      0x80040E00,  //  0016  RET	1	R7
      0x70020046,  //  0017  JMP		#005F
      0x541E0006,  //  0018  LDINT	R7	7
      0x1C1C0C07,  //  0019  EQ	R7	R6	R7
      0x781E0005,  //  001A  JMPF	R7	#0021
      0x8C1C0906,  //  001B  GETMET	R7	R4	K6
      0x88240907,  //  001C  GETMBR	R9	R4	K7
      0x58280005,  //  001D  LDCONST	R10	K5
      0x7C1C0600,  //  001E  CALL	R7	3
      0x80040E00,  //  001F  RET	1	R7
      0x7002003D,  //  0020  JMP		#005F
      0x541E0007,  //  0021  LDINT	R7	8
      0x1C1C0C07,  //  0022  EQ	R7	R6	R7
      0x781E0005,  //  0023  JMPF	R7	#002A
      0x8C1C0906,  //  0024  GETMET	R7	R4	K6
      0x88240907,  //  0025  GETMBR	R9	R4	K7
      0x58280005,  //  0026  LDCONST	R10	K5
      0x7C1C0600,  //  0027  CALL	R7	3
      0x80040E00,  //  0028  RET	1	R7
      0x70020034,  //  0029  JMP		#005F
      0x541E000E,  //  002A  LDINT	R7	15
      0x1C1C0C07,  //  002B  EQ	R7	R6	R7
      0x781E0005,  //  002C  JMPF	R7	#0033
      0x8C1C0906,  //  002D  GETMET	R7	R4	K6
      0x88240907,  //  002E  GETMBR	R9	R4	K7
      0x58280005,  //  002F  LDCONST	R10	K5
      0x7C1C0600,  //  0030  CALL	R7	3
      0x80040E00,  //  0031  RET	1	R7
      0x7002002B,  //  0032  JMP		#005F
      0x541E4000,  //  0033  LDINT	R7	16385
      0x1C1C0C07,  //  0034  EQ	R7	R6	R7
      0x781E0005,  //  0035  JMPF	R7	#003C
      0x8C1C0906,  //  0036  GETMET	R7	R4	K6
      0x88240907,  //  0037  GETMBR	R9	R4	K7
      0x58280005,  //  0038  LDCONST	R10	K5
      0x7C1C0600,  //  0039  CALL	R7	3
      0x80040E00,  //  003A  RET	1	R7
      0x70020022,  //  003B  JMP		#005F
      0x541E4009,  //  003C  LDINT	R7	16394
      0x1C1C0C07,  //  003D  EQ	R7	R6	R7
      0x781E0005,  //  003E  JMPF	R7	#0045
      0x8C1C0906,  //  003F  GETMET	R7	R4	K6
      0x88240907,  //  0040  GETMBR	R9	R4	K7
      0x58280005,  //  0041  LDCONST	R10	K5
      0x7C1C0600,  //  0042  CALL	R7	3
      0x80040E00,  //  0043  RET	1	R7
      0x70020019,  //  0044  JMP		#005F
      0x541E000F,  //  0045  LDINT	R7	16
      0x1C1C0C07,  //  0046  EQ	R7	R6	R7
      0x781E0005,  //  0047  JMPF	R7	#004E
      0x8C1C0906,  //  0048  GETMET	R7	R4	K6
      0x88240907,  //  0049  GETMBR	R9	R4	K7
      0x58280005,  //  004A  LDCONST	R10	K5
      0x7C1C0600,  //  004B  CALL	R7	3
      0x80040E00,  //  004C  RET	1	R7
      0x70020010,  //  004D  JMP		#005F
      0x541EFFFB,  //  004E  LDINT	R7	65532
      0x1C1C0C07,  //  004F  EQ	R7	R6	R7
      0x781E0005,  //  0050  JMPF	R7	#0057
      0x8C1C0906,  //  0051  GETMET	R7	R4	K6
      0x8824090B,  //  0052  GETMBR	R9	R4	K11
      0x58280009,  //  0053  LDCONST	R10	K9
      0x7C1C0600,  //  0054  CALL	R7	3
      0x80040E00,  //  0055  RET	1	R7
      0x70020007,  //  0056  JMP		#005F
      0x541EFFFC,  //  0057  LDINT	R7	65533
      0x1C1C0C07,  //  0058  EQ	R7	R6	R7
      0x781E0004,  //  0059  JMPF	R7	#005F
      0x8C1C0906,  //  005A  GETMET	R7	R4	K6
      0x8824090B,  //  005B  GETMBR	R9	R4	K11
      0x542A0004,  //  005C  LDINT	R10	5
      0x7C1C0600,  //  005D  CALL	R7	3
      0x80040E00,  //  005E  RET	1	R7
      0x70020007,  //  005F  JMP		#0068
      0x601C0003,  //  0060  GETGBL	R7	G3
      0x5C200000,  //  0061  MOVE	R8	R0
      0x7C1C0200,  //  0062  CALL	R7	1
      0x8C1C0F0C,  //  0063  GETMET	R7	R7	K12
      0x5C240200,  //  0064  MOVE	R9	R1
      0x5C280400,  //  0065  MOVE	R10	R2
      0x7C1C0600,  //  0066  CALL	R7	3
      0x80040E00,  //  0067  RET	1	R7
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light3_invoke_request,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(scale_uint),
    /* K9   */  be_nested_str_weak(set),
    /* K10  */  be_nested_str_weak(hue),
    /* K11  */  be_nested_str_weak(update_shadow),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(hue_X3A),
    /* K14  */  be_const_int(1),
    /* K15  */  be_const_int(2),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(sat),
    /* K18  */  be_nested_str_weak(sat_X3A),
    /* K19  */  be_nested_str_weak(_X20sat_X3A),
    /* K20  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[148]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x542202FF,  //  0005  LDINT	R8	768
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220081,  //  0007  JMPF	R8	#008A
      0x1C200F05,  //  0008  EQ	R8	R7	K5
      0x78220019,  //  0009  JMPF	R8	#0024
      0x8C200506,  //  000A  GETMET	R8	R2	K6
      0x58280005,  //  000B  LDCONST	R10	K5
      0x7C200400,  //  000C  CALL	R8	2
      0xB8260E00,  //  000D  GETNGBL	R9	K7
      0x8C241308,  //  000E  GETMET	R9	R9	K8
      0x5C2C1000,  //  000F  MOVE	R11	R8
      0x58300005,  //  0010  LDCONST	R12	K5
      0x543600FD,  //  0011  LDINT	R13	254
      0x58380005,  //  0012  LDCONST	R14	K5
      0x543E0167,  //  0013  LDINT	R15	360
      0x7C240C00,  //  0014  CALL	R9	6
      0x8C280909,  //  0015  GETMET	R10	R4	K9
      0x60300013,  //  0016  GETGBL	R12	G19
      0x7C300000,  //  0017  CALL	R12	0
      0x98321409,  //  0018  SETIDX	R12	K10	R9
      0x7C280400,  //  0019  CALL	R10	2
      0x8C28010B,  //  001A  GETMET	R10	R0	K11
      0x7C280200,  //  001B  CALL	R10	1
      0x60280008,  //  001C  GETGBL	R10	G8
      0x5C2C1000,  //  001D  MOVE	R11	R8
      0x7C280200,  //  001E  CALL	R10	1
      0x002A1A0A,  //  001F  ADD	R10	K13	R10
      0x900E180A,  //  0020  SETMBR	R3	K12	R10
      0x50280200,  //  0021  LDBOOL	R10	1	0
      0x80041400,  //  0022  RET	1	R10
      0x70020064,  //  0023  JMP		#0089
      0x1C200F0E,  //  0024  EQ	R8	R7	K14
      0x78220002,  //  0025  JMPF	R8	#0029
      0x50200200,  //  0026  LDBOOL	R8	1	0
      0x80041000,  //  0027  RET	1	R8
      0x7002005F,  //  0028  JMP		#0089
      0x1C200F0F,  //  0029  EQ	R8	R7	K15
      0x78220002,  //  002A  JMPF	R8	#002E
      0x50200200,  //  002B  LDBOOL	R8	1	0
      0x80041000,  //  002C  RET	1	R8
      0x7002005A,  //  002D  JMP		#0089
      0x1C200F10,  //  002E  EQ	R8	R7	K16
      0x78220019,  //  002F  JMPF	R8	#004A
      0x8C200506,  //  0030  GETMET	R8	R2	K6
      0x58280005,  //  0031  LDCONST	R10	K5
      0x7C200400,  //  0032  CALL	R8	2
      0xB8260E00,  //  0033  GETNGBL	R9	K7
      0x8C241308,  //  0034  GETMET	R9	R9	K8
      0x5C2C1000,  //  0035  MOVE	R11	R8
      0x58300005,  //  0036  LDCONST	R12	K5
      0x543600FD,  //  0037  LDINT	R13	254
      0x58380005,  //  0038  LDCONST	R14	K5
      0x543E00FE,  //  0039  LDINT	R15	255
      0x7C240C00,  //  003A  CALL	R9	6
      0x8C280909,  //  003B  GETMET	R10	R4	K9
      0x60300013,  //  003C  GETGBL	R12	G19
      0x7C300000,  //  003D  CALL	R12	0
      0x98322209,  //  003E  SETIDX	R12	K17	R9
      0x7C280400,  //  003F  CALL	R10	2
      0x8C28010B,  //  0040  GETMET	R10	R0	K11
      0x7C280200,  //  0041  CALL	R10	1
      0x60280008,  //  0042  GETGBL	R10	G8
      0x5C2C1000,  //  0043  MOVE	R11	R8
      0x7C280200,  //  0044  CALL	R10	1
      0x002A240A,  //  0045  ADD	R10	K18	R10
      0x900E180A,  //  0046  SETMBR	R3	K12	R10
      0x50280200,  //  0047  LDBOOL	R10	1	0
      0x80041400,  //  0048  RET	1	R10
      0x7002003E,  //  0049  JMP		#0089
      0x54220003,  //  004A  LDINT	R8	4
      0x1C200E08,  //  004B  EQ	R8	R7	R8
      0x78220002,  //  004C  JMPF	R8	#0050
      0x50200200,  //  004D  LDBOOL	R8	1	0
      0x80041000,  //  004E  RET	1	R8
      0x70020038,  //  004F  JMP		#0089
      0x54220004,  //  0050  LDINT	R8	5
      0x1C200E08,  //  0051  EQ	R8	R7	R8
      0x78220002,  //  0052  JMPF	R8	#0056
      0x50200200,  //  0053  LDBOOL	R8	1	0
      0x80041000,  //  0054  RET	1	R8
      0x70020032,  //  0055  JMP		#0089
      0x54220005,  //  0056  LDINT	R8	6
      0x1C200E08,  //  0057  EQ	R8	R7	R8
      0x7822002A,  //  0058  JMPF	R8	#0084
      0x8C200506,  //  0059  GETMET	R8	R2	K6
      0x58280005,  //  005A  LDCONST	R10	K5
      0x7C200400,  //  005B  CALL	R8	2
      0xB8260E00,  //  005C  GETNGBL	R9	K7
      0x8C241308,  //  005D  GETMET	R9	R9	K8
      0x5C2C1000,  //  005E  MOVE	R11	R8
      0x58300005,  //  005F  LDCONST	R12	K5
      0x543600FD,  //  0060  LDINT	R13	254
      0x58380005,  //  0061  LDCONST	R14	K5
      0x543E0167,  //  0062  LDINT	R15	360
      0x7C240C00,  //  0063  CALL	R9	6
      0x8C280506,  //  0064  GETMET	R10	R2	K6
      0x5830000E,  //  0065  LDCONST	R12	K14
      0x7C280400,  //  0066  CALL	R10	2
      0xB82E0E00,  //  0067  GETNGBL	R11	K7
      0x8C2C1708,  //  0068  GETMET	R11	R11	K8
      0x5C341400,  //  0069  MOVE	R13	R10
      0x58380005,  //  006A  LDCONST	R14	K5
      0x543E00FD,  //  006B  LDINT	R15	254
      0x58400005,  //  006C  LDCONST	R16	K5
      0x544600FE,  //  006D  LDINT	R17	255
      0x7C2C0C00,  //  006E  CALL	R11	6
      0x8C300909,  //  006F  GETMET	R12	R4	K9
      0x60380013,  //  0070  GETGBL	R14	G19
      0x7C380000,  //  0071  CALL	R14	0
      0x983A1409,  //  0072  SETIDX	R14	K10	R9
      0x983A220B,  //  0073  SETIDX	R14	K17	R11
      0x7C300400,  //  0074  CALL	R12	2
      0x8C30010B,  //  0075  GETMET	R12	R0	K11
      0x7C300200,  //  0076  CALL	R12	1
      0x60300008,  //  0077  GETGBL	R12	G8
      0x5C341000,  //  0078  MOVE	R13	R8
      0x7C300200,  //  0079  CALL	R12	1
      0x00321A0C,  //  007A  ADD	R12	K13	R12
      0x00301913,  //  007B  ADD	R12	R12	K19
      0x60340008,  //  007C  GETGBL	R13	G8
      0x5C381400,  //  007D  MOVE	R14	R10
      0x7C340200,  //  007E  CALL	R13	1
      0x0030180D,  //  007F  ADD	R12	R12	R13
      0x900E180C,  //  0080  SETMBR	R3	K12	R12
      0x50300200,  //  0081  LDBOOL	R12	1	0
      0x80041800,  //  0082  RET	1	R12
      0x70020004,  //  0083  JMP		#0089
      0x54220046,  //  0084  LDINT	R8	71
      0x1C200E08,  //  0085  EQ	R8	R7	R8
      0x78220001,  //  0086  JMPF	R8	#0089
      0x50200200,  //  0087  LDBOOL	R8	1	0
      0x80041000,  //  0088  RET	1	R8
      0x70020008,  //  0089  JMP		#0093
      0x60200003,  //  008A  GETGBL	R8	G3
      0x5C240000,  //  008B  MOVE	R9	R0
      0x7C200200,  //  008C  CALL	R8	1
      0x8C201114,  //  008D  GETMET	R8	R8	K20
      0x5C280200,  //  008E  MOVE	R10	R1
      0x5C2C0400,  //  008F  MOVE	R11	R2
      0x5C300600,  //  0090  MOVE	R12	R3
      0x7C200800,  //  0091  CALL	R8	4
      0x80041000,  //  0092  RET	1	R8
      0x80000000,  //  0093  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light3_update_shadow,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(update_shadow),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(hue),
    /* K5   */  be_nested_str_weak(sat),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(scale_uint),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(shadow_hue),
    /* K10  */  be_nested_str_weak(shadow_sat),
    /* K11  */  be_nested_str_weak(attribute_updated),
    /* K12  */  be_const_int(1),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080003,  //  0001  GETGBL	R2	G3
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x8C080302,  //  0006  GETMET	R2	R1	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x58140004,  //  0009  LDCONST	R5	K4
      0x4C180000,  //  000A  LDNIL	R6
      0x7C0C0600,  //  000B  CALL	R3	3
      0x8C100503,  //  000C  GETMET	R4	R2	K3
      0x58180005,  //  000D  LDCONST	R6	K5
      0x4C1C0000,  //  000E  LDNIL	R7
      0x7C100600,  //  000F  CALL	R4	3
      0x4C140000,  //  0010  LDNIL	R5
      0x20140605,  //  0011  NE	R5	R3	R5
      0x78160009,  //  0012  JMPF	R5	#001D
      0xB8160C00,  //  0013  GETNGBL	R5	K6
      0x8C140B07,  //  0014  GETMET	R5	R5	K7
      0x5C1C0600,  //  0015  MOVE	R7	R3
      0x58200008,  //  0016  LDCONST	R8	K8
      0x54260167,  //  0017  LDINT	R9	360
      0x58280008,  //  0018  LDCONST	R10	K8
      0x542E00FD,  //  0019  LDINT	R11	254
      0x7C140C00,  //  001A  CALL	R5	6
      0x5C0C0A00,  //  001B  MOVE	R3	R5
      0x70020000,  //  001C  JMP		#001E
      0x880C0109,  //  001D  GETMBR	R3	R0	K9
      0x4C140000,  //  001E  LDNIL	R5
      0x20140805,  //  001F  NE	R5	R4	R5
      0x78160009,  //  0020  JMPF	R5	#002B
      0xB8160C00,  //  0021  GETNGBL	R5	K6
      0x8C140B07,  //  0022  GETMET	R5	R5	K7
      0x5C1C0800,  //  0023  MOVE	R7	R4
      0x58200008,  //  0024  LDCONST	R8	K8
      0x542600FE,  //  0025  LDINT	R9	255
      0x58280008,  //  0026  LDCONST	R10	K8
      0x542E00FD,  //  0027  LDINT	R11	254
      0x7C140C00,  //  0028  CALL	R5	6
      0x5C100A00,  //  0029  MOVE	R4	R5
      0x70020000,  //  002A  JMP		#002C
      0x8810010A,  //  002B  GETMBR	R4	R0	K10
      0x88140109,  //  002C  GETMBR	R5	R0	K9
      0x20140605,  //  002D  NE	R5	R3	R5
      0x78160005,  //  002E  JMPF	R5	#0035
      0x8C14010B,  //  002F  GETMET	R5	R0	K11
      0x4C1C0000,  //  0030  LDNIL	R7
      0x542202FF,  //  0031  LDINT	R8	768
      0x58240008,  //  0032  LDCONST	R9	K8
      0x7C140800,  //  0033  CALL	R5	4
      0x90021203,  //  0034  SETMBR	R0	K9	R3
      0x8814010A,  //  0035  GETMBR	R5	R0	K10
      0x20140805,  //  0036  NE	R5	R4	R5
      0x78160005,  //  0037  JMPF	R5	#003E
      0x8C14010B,  //  0038  GETMET	R5	R0	K11
      0x4C1C0000,  //  0039  LDNIL	R7
      0x542202FF,  //  003A  LDINT	R8	768
      0x5824000C,  //  003B  LDCONST	R9	K12
      0x7C140800,  //  003C  CALL	R5	4
      0x90021404,  //  003D  SETMBR	R0	K10	R4
      0x80000000,  //  003E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light3
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_class(Matter_Plugin_Light3,
    2,
    &be_class_Matter_Plugin_Light1,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(CLUSTERS, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(768, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(7),
        be_const_int(8),
        be_const_int(15),
        be_const_int(16385),
        be_const_int(16394),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light3_invoke_request_closure) },
        { be_const_key_weak(TYPES, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(269, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(init, 1), be_const_closure(Matter_Plugin_Light3_init_closure) },
        { be_const_key_weak(shadow_hue, -1), be_const_var(0) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Light3_update_shadow_closure) },
        { be_const_key_weak(shadow_sat, -1), be_const_var(1) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light3_read_attribute_closure) },
    })),
    be_str_weak(Matter_Plugin_Light3)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light3_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light3);
    be_setglobal(vm, "Matter_Plugin_Light3");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
