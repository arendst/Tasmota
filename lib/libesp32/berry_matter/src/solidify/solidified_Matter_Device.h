/* Solidification of Matter_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Device;

/********************************************************************
** Solidified function: compute_manual_pairing_code
********************************************************************/
be_local_closure(Matter_Device_compute_manual_pairing_code,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(discriminator),
    /* K2   */  be_nested_str_weak(passcode),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X251i_X2505i_X2504i),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(Verhoeff),
    /* K7   */  be_nested_str_weak(checksum),
    }),
    be_str_weak(compute_manual_pairing_code),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x540E0FFE,  //  0002  LDINT	R3	4095
      0x2C080403,  //  0003  AND	R2	R2	R3
      0x540E0009,  //  0004  LDINT	R3	10
      0x3C080403,  //  0005  SHR	R2	R2	R3
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x541202FF,  //  0007  LDINT	R4	768
      0x2C0C0604,  //  0008  AND	R3	R3	R4
      0x54120005,  //  0009  LDINT	R4	6
      0x380C0604,  //  000A  SHL	R3	R3	R4
      0x88100102,  //  000B  GETMBR	R4	R0	K2
      0x54163FFE,  //  000C  LDINT	R5	16383
      0x2C100805,  //  000D  AND	R4	R4	R5
      0x300C0604,  //  000E  OR	R3	R3	R4
      0x88100102,  //  000F  GETMBR	R4	R0	K2
      0x5416000D,  //  0010  LDINT	R5	14
      0x3C100805,  //  0011  SHR	R4	R4	R5
      0x8C140303,  //  0012  GETMET	R5	R1	K3
      0x581C0004,  //  0013  LDCONST	R7	K4
      0x5C200400,  //  0014  MOVE	R8	R2
      0x5C240600,  //  0015  MOVE	R9	R3
      0x5C280800,  //  0016  MOVE	R10	R4
      0x7C140A00,  //  0017  CALL	R5	5
      0xB81A0A00,  //  0018  GETNGBL	R6	K5
      0x88180D06,  //  0019  GETMBR	R6	R6	K6
      0x8C180D07,  //  001A  GETMET	R6	R6	K7
      0x5C200A00,  //  001B  MOVE	R8	R5
      0x7C180400,  //  001C  CALL	R6	2
      0x00140A06,  //  001D  ADD	R5	R5	R6
      0x80040A00,  //  001E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_attribute_expansion
********************************************************************/
be_local_closure(Matter_Device_process_attribute_expansion,   /* name */
  be_nested_proto(
    33,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(format),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20process_attribute_expansion_X20_X25s),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(plugins),
    /* K10  */  be_nested_str_weak(get_endpoints),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20ep_list_X20_X25s_X20_X25s),
    /* K12  */  be_nested_str_weak(find),
    /* K13  */  be_nested_str_weak(get_cluster_list),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20cluster_list_X20_X25s_X20_X25s),
    /* K15  */  be_nested_str_weak(get_attribute_list),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20attr_list_X20_X25s_X20_X25s),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20expansion_X20_X5B_X2502X_X5D_X2504X_X2F_X2504X),
    /* K18  */  be_nested_str_weak(stop_iteration),
    /* K19  */  be_nested_str_weak(status),
    /* K20  */  be_nested_str_weak(matter),
    /* K21  */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    /* K22  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    /* K23  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K24  */  be_nested_str_weak(UNREPORTABLE_ATTRIBUTE),
    }),
    be_str_weak(process_attribute_expansion),
    &be_const_str_solidified,
    ( &(const binstruction[216]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x88100301,  //  0001  GETMBR	R4	R1	K1
      0x60140012,  //  0002  GETGBL	R5	G18
      0x7C140000,  //  0003  CALL	R5	0
      0x40180A04,  //  0004  CONNECT	R6	R5	R4
      0x50180000,  //  0005  LDBOOL	R6	0	0
      0x881C0302,  //  0006  GETMBR	R7	R1	K2
      0x60200012,  //  0007  GETGBL	R8	G18
      0x7C200000,  //  0008  CALL	R8	0
      0x40241007,  //  0009  CONNECT	R9	R8	R7
      0x50240000,  //  000A  LDBOOL	R9	0	0
      0x88280303,  //  000B  GETMBR	R10	R1	K3
      0x602C0012,  //  000C  GETGBL	R11	G18
      0x7C2C0000,  //  000D  CALL	R11	0
      0x4030160A,  //  000E  CONNECT	R12	R11	R10
      0x50300000,  //  000F  LDBOOL	R12	0	0
      0x88340301,  //  0010  GETMBR	R13	R1	K1
      0x4C380000,  //  0011  LDNIL	R14
      0x20341A0E,  //  0012  NE	R13	R13	R14
      0x78360007,  //  0013  JMPF	R13	#001C
      0x88340302,  //  0014  GETMBR	R13	R1	K2
      0x4C380000,  //  0015  LDNIL	R14
      0x20341A0E,  //  0016  NE	R13	R13	R14
      0x78360003,  //  0017  JMPF	R13	#001C
      0x88340303,  //  0018  GETMBR	R13	R1	K3
      0x4C380000,  //  0019  LDNIL	R14
      0x20341A0E,  //  001A  NE	R13	R13	R14
      0x74360000,  //  001B  JMPT	R13	#001D
      0x50340001,  //  001C  LDBOOL	R13	0	1
      0x50340200,  //  001D  LDBOOL	R13	1	0
      0xB83A0800,  //  001E  GETNGBL	R14	K4
      0x8C381D05,  //  001F  GETMET	R14	R14	K5
      0x8C400706,  //  0020  GETMET	R16	R3	K6
      0x58480007,  //  0021  LDCONST	R18	K7
      0x604C0008,  //  0022  GETGBL	R19	G8
      0x5C500200,  //  0023  MOVE	R20	R1
      0x7C4C0200,  //  0024  CALL	R19	1
      0x7C400600,  //  0025  CALL	R16	3
      0x58440008,  //  0026  LDCONST	R17	K8
      0x7C380600,  //  0027  CALL	R14	3
      0x60380010,  //  0028  GETGBL	R14	G16
      0x883C0109,  //  0029  GETMBR	R15	R0	K9
      0x7C380200,  //  002A  CALL	R14	1
      0xA802008C,  //  002B  EXBLK	0	#00B9
      0x5C3C1C00,  //  002C  MOVE	R15	R14
      0x7C3C0000,  //  002D  CALL	R15	0
      0x8C401F0A,  //  002E  GETMET	R16	R15	K10
      0x7C400200,  //  002F  CALL	R16	1
      0xB8460800,  //  0030  GETNGBL	R17	K4
      0x8C442305,  //  0031  GETMET	R17	R17	K5
      0x8C4C0706,  //  0032  GETMET	R19	R3	K6
      0x5854000B,  //  0033  LDCONST	R21	K11
      0x60580008,  //  0034  GETGBL	R22	G8
      0x5C5C1E00,  //  0035  MOVE	R23	R15
      0x7C580200,  //  0036  CALL	R22	1
      0x605C0008,  //  0037  GETGBL	R23	G8
      0x5C602000,  //  0038  MOVE	R24	R16
      0x7C5C0200,  //  0039  CALL	R23	1
      0x7C4C0800,  //  003A  CALL	R19	4
      0x58500008,  //  003B  LDCONST	R20	K8
      0x7C440600,  //  003C  CALL	R17	3
      0x4C440000,  //  003D  LDNIL	R17
      0x20440811,  //  003E  NE	R17	R4	R17
      0x78460009,  //  003F  JMPF	R17	#004A
      0x8C44210C,  //  0040  GETMET	R17	R16	K12
      0x5C4C0800,  //  0041  MOVE	R19	R4
      0x7C440400,  //  0042  CALL	R17	2
      0x4C480000,  //  0043  LDNIL	R18
      0x20442212,  //  0044  NE	R17	R17	R18
      0x78460002,  //  0045  JMPF	R17	#0049
      0x5C400A00,  //  0046  MOVE	R16	R5
      0x50180200,  //  0047  LDBOOL	R6	1	0
      0x70020000,  //  0048  JMP		#004A
      0x7001FFE1,  //  0049  JMP		#002C
      0x60440010,  //  004A  GETGBL	R17	G16
      0x5C482000,  //  004B  MOVE	R18	R16
      0x7C440200,  //  004C  CALL	R17	1
      0xA8020066,  //  004D  EXBLK	0	#00B5
      0x5C482200,  //  004E  MOVE	R18	R17
      0x7C480000,  //  004F  CALL	R18	0
      0x8C4C1F0D,  //  0050  GETMET	R19	R15	K13
      0x5C542400,  //  0051  MOVE	R21	R18
      0x7C4C0400,  //  0052  CALL	R19	2
      0xB8520800,  //  0053  GETNGBL	R20	K4
      0x8C502905,  //  0054  GETMET	R20	R20	K5
      0x8C580706,  //  0055  GETMET	R22	R3	K6
      0x5860000E,  //  0056  LDCONST	R24	K14
      0x60640008,  //  0057  GETGBL	R25	G8
      0x5C682400,  //  0058  MOVE	R26	R18
      0x7C640200,  //  0059  CALL	R25	1
      0x60680008,  //  005A  GETGBL	R26	G8
      0x5C6C2600,  //  005B  MOVE	R27	R19
      0x7C680200,  //  005C  CALL	R26	1
      0x7C580800,  //  005D  CALL	R22	4
      0x585C0008,  //  005E  LDCONST	R23	K8
      0x7C500600,  //  005F  CALL	R20	3
      0x4C500000,  //  0060  LDNIL	R20
      0x20500E14,  //  0061  NE	R20	R7	R20
      0x78520009,  //  0062  JMPF	R20	#006D
      0x8C50270C,  //  0063  GETMET	R20	R19	K12
      0x5C580E00,  //  0064  MOVE	R22	R7
      0x7C500400,  //  0065  CALL	R20	2
      0x4C540000,  //  0066  LDNIL	R21
      0x20502815,  //  0067  NE	R20	R20	R21
      0x78520002,  //  0068  JMPF	R20	#006C
      0x5C4C1000,  //  0069  MOVE	R19	R8
      0x50240200,  //  006A  LDBOOL	R9	1	0
      0x70020000,  //  006B  JMP		#006D
      0x7001FFE0,  //  006C  JMP		#004E
      0x60500010,  //  006D  GETGBL	R20	G16
      0x5C542600,  //  006E  MOVE	R21	R19
      0x7C500200,  //  006F  CALL	R20	1
      0xA802003F,  //  0070  EXBLK	0	#00B1
      0x5C542800,  //  0071  MOVE	R21	R20
      0x7C540000,  //  0072  CALL	R21	0
      0x8C581F0F,  //  0073  GETMET	R22	R15	K15
      0x5C602400,  //  0074  MOVE	R24	R18
      0x5C640E00,  //  0075  MOVE	R25	R7
      0x7C580600,  //  0076  CALL	R22	3
      0xB85E0800,  //  0077  GETNGBL	R23	K4
      0x8C5C2F05,  //  0078  GETMET	R23	R23	K5
      0x8C640706,  //  0079  GETMET	R25	R3	K6
      0x586C0010,  //  007A  LDCONST	R27	K16
      0x60700008,  //  007B  GETGBL	R28	G8
      0x5C742A00,  //  007C  MOVE	R29	R21
      0x7C700200,  //  007D  CALL	R28	1
      0x60740008,  //  007E  GETGBL	R29	G8
      0x5C782C00,  //  007F  MOVE	R30	R22
      0x7C740200,  //  0080  CALL	R29	1
      0x7C640800,  //  0081  CALL	R25	4
      0x58680008,  //  0082  LDCONST	R26	K8
      0x7C5C0600,  //  0083  CALL	R23	3
      0x4C5C0000,  //  0084  LDNIL	R23
      0x205C1417,  //  0085  NE	R23	R10	R23
      0x785E0028,  //  0086  JMPF	R23	#00B0
      0x8C5C2D0C,  //  0087  GETMET	R23	R22	K12
      0x5C641400,  //  0088  MOVE	R25	R10
      0x7C5C0400,  //  0089  CALL	R23	2
      0x4C600000,  //  008A  LDNIL	R24
      0x205C2E18,  //  008B  NE	R23	R23	R24
      0x785E0002,  //  008C  JMPF	R23	#0090
      0x5C581600,  //  008D  MOVE	R22	R11
      0x50300200,  //  008E  LDBOOL	R12	1	0
      0x70020000,  //  008F  JMP		#0091
      0x7001FFDF,  //  0090  JMP		#0071
      0x605C0010,  //  0091  GETGBL	R23	G16
      0x5C602C00,  //  0092  MOVE	R24	R22
      0x7C5C0200,  //  0093  CALL	R23	1
      0xA8020017,  //  0094  EXBLK	0	#00AD
      0x5C602E00,  //  0095  MOVE	R24	R23
      0x7C600000,  //  0096  CALL	R24	0
      0xB8660800,  //  0097  GETNGBL	R25	K4
      0x8C643305,  //  0098  GETMET	R25	R25	K5
      0x8C6C0706,  //  0099  GETMET	R27	R3	K6
      0x58740011,  //  009A  LDCONST	R29	K17
      0x5C782400,  //  009B  MOVE	R30	R18
      0x5C7C2A00,  //  009C  MOVE	R31	R21
      0x5C803000,  //  009D  MOVE	R32	R24
      0x7C6C0A00,  //  009E  CALL	R27	5
      0x58700008,  //  009F  LDCONST	R28	K8
      0x7C640600,  //  00A0  CALL	R25	3
      0x90060212,  //  00A1  SETMBR	R1	K1	R18
      0x90060415,  //  00A2  SETMBR	R1	K2	R21
      0x90060618,  //  00A3  SETMBR	R1	K3	R24
      0x5C640400,  //  00A4  MOVE	R25	R2
      0x5C681E00,  //  00A5  MOVE	R26	R15
      0x5C6C0200,  //  00A6  MOVE	R27	R1
      0x5C701A00,  //  00A7  MOVE	R28	R13
      0x7C640600,  //  00A8  CALL	R25	3
      0x78660001,  //  00A9  JMPF	R25	#00AC
      0xA8040004,  //  00AA  EXBLK	1	4
      0x80003400,  //  00AB  RET	0
      0x7001FFE7,  //  00AC  JMP		#0095
      0x585C0012,  //  00AD  LDCONST	R23	K18
      0xAC5C0200,  //  00AE  CATCH	R23	1	0
      0xB0080000,  //  00AF  RAISE	2	R0	R0
      0x7001FFBF,  //  00B0  JMP		#0071
      0x58500012,  //  00B1  LDCONST	R20	K18
      0xAC500200,  //  00B2  CATCH	R20	1	0
      0xB0080000,  //  00B3  RAISE	2	R0	R0
      0x7001FF98,  //  00B4  JMP		#004E
      0x58440012,  //  00B5  LDCONST	R17	K18
      0xAC440200,  //  00B6  CATCH	R17	1	0
      0xB0080000,  //  00B7  RAISE	2	R0	R0
      0x7001FF72,  //  00B8  JMP		#002C
      0x58380012,  //  00B9  LDCONST	R14	K18
      0xAC380200,  //  00BA  CATCH	R14	1	0
      0xB0080000,  //  00BB  RAISE	2	R0	R0
      0x78360019,  //  00BC  JMPF	R13	#00D7
      0x5C380C00,  //  00BD  MOVE	R14	R6
      0x743A0003,  //  00BE  JMPT	R14	#00C3
      0xB83A2800,  //  00BF  GETNGBL	R14	K20
      0x88381D15,  //  00C0  GETMBR	R14	R14	K21
      0x9006260E,  //  00C1  SETMBR	R1	K19	R14
      0x7002000E,  //  00C2  JMP		#00D2
      0x5C381200,  //  00C3  MOVE	R14	R9
      0x743A0003,  //  00C4  JMPT	R14	#00C9
      0xB83A2800,  //  00C5  GETNGBL	R14	K20
      0x88381D16,  //  00C6  GETMBR	R14	R14	K22
      0x9006260E,  //  00C7  SETMBR	R1	K19	R14
      0x70020008,  //  00C8  JMP		#00D2
      0x5C381800,  //  00C9  MOVE	R14	R12
      0x743A0003,  //  00CA  JMPT	R14	#00CF
      0xB83A2800,  //  00CB  GETNGBL	R14	K20
      0x88381D17,  //  00CC  GETMBR	R14	R14	K23
      0x9006260E,  //  00CD  SETMBR	R1	K19	R14
      0x70020002,  //  00CE  JMP		#00D2
      0xB83A2800,  //  00CF  GETNGBL	R14	K20
      0x88381D18,  //  00D0  GETMBR	R14	R14	K24
      0x9006260E,  //  00D1  SETMBR	R1	K19	R14
      0x5C380400,  //  00D2  MOVE	R14	R2
      0x4C3C0000,  //  00D3  LDNIL	R15
      0x5C400200,  //  00D4  MOVE	R16	R1
      0x50440200,  //  00D5  LDBOOL	R17	1	0
      0x7C380600,  //  00D6  CALL	R14	3
      0x80000000,  //  00D7  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_param
********************************************************************/
be_local_closure(Matter_Device_save_param,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(dump),
    /* K2   */  be_nested_str_weak(distinguish),
    /* K3   */  be_nested_str_weak(discriminator),
    /* K4   */  be_nested_str_weak(passcode),
    /* K5   */  be_nested_str_weak(string),
    /* K6   */  be_nested_str_weak(FILENAME),
    /* K7   */  be_nested_str_weak(w),
    /* K8   */  be_nested_str_weak(write),
    /* K9   */  be_nested_str_weak(close),
    /* K10  */  be_nested_str_weak(tasmota),
    /* K11  */  be_nested_str_weak(log),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K13  */  be_nested_str_weak(_X7C),
    /* K14  */  be_const_int(2),
    }),
    be_str_weak(save_param),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C100000,  //  0003  CALL	R4	0
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x98120405,  //  0005  SETIDX	R4	K2	R5
      0x88140104,  //  0006  GETMBR	R5	R0	K4
      0x98120805,  //  0007  SETIDX	R4	K4	R5
      0x7C080400,  //  0008  CALL	R2	2
      0xA802000D,  //  0009  EXBLK	0	#0018
      0xA40E0A00,  //  000A  IMPORT	R3	K5
      0x60100011,  //  000B  GETGBL	R4	G17
      0x88140106,  //  000C  GETMBR	R5	R0	K6
      0x58180007,  //  000D  LDCONST	R6	K7
      0x7C100400,  //  000E  CALL	R4	2
      0x8C140908,  //  000F  GETMET	R5	R4	K8
      0x5C1C0400,  //  0010  MOVE	R7	R2
      0x7C140400,  //  0011  CALL	R5	2
      0x8C140909,  //  0012  GETMET	R5	R4	K9
      0x7C140200,  //  0013  CALL	R5	1
      0xA8040001,  //  0014  EXBLK	1	1
      0x80040400,  //  0015  RET	1	R2
      0xA8040001,  //  0016  EXBLK	1	1
      0x70020011,  //  0017  JMP		#002A
      0xAC0C0002,  //  0018  CATCH	R3	0	2
      0x7002000E,  //  0019  JMP		#0029
      0xB8161400,  //  001A  GETNGBL	R5	K10
      0x8C140B0B,  //  001B  GETMET	R5	R5	K11
      0x601C0008,  //  001C  GETGBL	R7	G8
      0x5C200600,  //  001D  MOVE	R8	R3
      0x7C1C0200,  //  001E  CALL	R7	1
      0x001E1807,  //  001F  ADD	R7	K12	R7
      0x001C0F0D,  //  0020  ADD	R7	R7	K13
      0x60200008,  //  0021  GETGBL	R8	G8
      0x5C240800,  //  0022  MOVE	R9	R4
      0x7C200200,  //  0023  CALL	R8	1
      0x001C0E08,  //  0024  ADD	R7	R7	R8
      0x5820000E,  //  0025  LDCONST	R8	K14
      0x7C140600,  //  0026  CALL	R5	3
      0x80040400,  //  0027  RET	1	R2
      0x70020000,  //  0028  JMP		#002A
      0xB0080000,  //  0029  RAISE	2	R0	R0
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_udp
********************************************************************/
be_local_closure(Matter_Device_start_udp,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(msg_received),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20starting_X20UDP_X20server_X20on_X20port_X3A_X20),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(UDPServer),
    /* K7   */  be_nested_str_weak(),
    /* K8   */  be_nested_str_weak(start),
    }),
    be_str_weak(start_udp),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540615A3,  //  0006  LDINT	R1	5540
      0xB80A0200,  //  0007  GETNGBL	R2	K1
      0x8C080502,  //  0008  GETMET	R2	R2	K2
      0x60100008,  //  0009  GETGBL	R4	G8
      0x5C140200,  //  000A  MOVE	R5	R1
      0x7C100200,  //  000B  CALL	R4	1
      0x00120604,  //  000C  ADD	R4	K3	R4
      0x58140004,  //  000D  LDCONST	R5	K4
      0x7C080600,  //  000E  CALL	R2	3
      0xB80A0A00,  //  000F  GETNGBL	R2	K5
      0x8C080506,  //  0010  GETMET	R2	R2	K6
      0x58100007,  //  0011  LDCONST	R4	K7
      0x5C140200,  //  0012  MOVE	R5	R1
      0x7C080600,  //  0013  CALL	R2	3
      0x90020002,  //  0014  SETMBR	R0	K0	R2
      0x88080100,  //  0015  GETMBR	R2	R0	K0
      0x8C080508,  //  0016  GETMET	R2	R2	K8
      0x84100000,  //  0017  CLOSURE	R4	P0
      0x7C080400,  //  0018  CALL	R2	2
      0xA0000000,  //  0019  CLOSE	R0
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_param
********************************************************************/
be_local_closure(Matter_Device_load_param,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(FILENAME),
    /* K3   */  be_nested_str_weak(read),
    /* K4   */  be_nested_str_weak(close),
    /* K5   */  be_nested_str_weak(json),
    /* K6   */  be_nested_str_weak(load),
    /* K7   */  be_nested_str_weak(discriminator),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_nested_str_weak(distinguish),
    /* K10  */  be_nested_str_weak(passcode),
    /* K11  */  be_nested_str_weak(io_error),
    /* K12  */  be_nested_str_weak(tasmota),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
    /* K15  */  be_nested_str_weak(_X7C),
    /* K16  */  be_const_int(2),
    /* K17  */  be_nested_str_weak(random),
    /* K18  */  be_nested_str_weak(get),
    /* K19  */  be_const_int(0),
    /* K20  */  be_nested_str_weak(PASSCODE_DEFAULT),
    /* K21  */  be_nested_str_weak(save_param),
    }),
    be_str_weak(load_param),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA8020014,  //  0002  EXBLK	0	#0018
      0x600C0011,  //  0003  GETGBL	R3	G17
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C100703,  //  0006  GETMET	R4	R3	K3
      0x7C100200,  //  0007  CALL	R4	1
      0x8C140704,  //  0008  GETMET	R5	R3	K4
      0x7C140200,  //  0009  CALL	R5	1
      0xA4160A00,  //  000A  IMPORT	R5	K5
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x5C200800,  //  000C  MOVE	R8	R4
      0x7C180400,  //  000D  CALL	R6	2
      0x8C1C0D08,  //  000E  GETMET	R7	R6	K8
      0x58240009,  //  000F  LDCONST	R9	K9
      0x7C1C0400,  //  0010  CALL	R7	2
      0x90020E07,  //  0011  SETMBR	R0	K7	R7
      0x8C1C0D08,  //  0012  GETMET	R7	R6	K8
      0x5824000A,  //  0013  LDCONST	R9	K10
      0x7C1C0400,  //  0014  CALL	R7	2
      0x90021407,  //  0015  SETMBR	R0	K10	R7
      0xA8040001,  //  0016  EXBLK	1	1
      0x70020012,  //  0017  JMP		#002B
      0xAC0C0002,  //  0018  CATCH	R3	0	2
      0x7002000F,  //  0019  JMP		#002A
      0x2014070B,  //  001A  NE	R5	R3	K11
      0x7816000C,  //  001B  JMPF	R5	#0029
      0xB8161800,  //  001C  GETNGBL	R5	K12
      0x8C140B0D,  //  001D  GETMET	R5	R5	K13
      0x601C0008,  //  001E  GETGBL	R7	G8
      0x5C200600,  //  001F  MOVE	R8	R3
      0x7C1C0200,  //  0020  CALL	R7	1
      0x001E1C07,  //  0021  ADD	R7	K14	R7
      0x001C0F0F,  //  0022  ADD	R7	R7	K15
      0x60200008,  //  0023  GETGBL	R8	G8
      0x5C240800,  //  0024  MOVE	R9	R4
      0x7C200200,  //  0025  CALL	R8	1
      0x001C0E08,  //  0026  ADD	R7	R7	R8
      0x58200010,  //  0027  LDCONST	R8	K16
      0x7C140600,  //  0028  CALL	R5	3
      0x70020000,  //  0029  JMP		#002B
      0xB0080000,  //  002A  RAISE	2	R0	R0
      0x500C0000,  //  002B  LDBOOL	R3	0	0
      0x88100107,  //  002C  GETMBR	R4	R0	K7
      0x4C140000,  //  002D  LDNIL	R5
      0x1C100805,  //  002E  EQ	R4	R4	R5
      0x7812000A,  //  002F  JMPF	R4	#003B
      0x8C100511,  //  0030  GETMET	R4	R2	K17
      0x58180010,  //  0031  LDCONST	R6	K16
      0x7C100400,  //  0032  CALL	R4	2
      0x8C100912,  //  0033  GETMET	R4	R4	K18
      0x58180013,  //  0034  LDCONST	R6	K19
      0x581C0010,  //  0035  LDCONST	R7	K16
      0x7C100600,  //  0036  CALL	R4	3
      0x54160FFE,  //  0037  LDINT	R5	4095
      0x2C100805,  //  0038  AND	R4	R4	R5
      0x90020E04,  //  0039  SETMBR	R0	K7	R4
      0x500C0200,  //  003A  LDBOOL	R3	1	0
      0x8810010A,  //  003B  GETMBR	R4	R0	K10
      0x4C140000,  //  003C  LDNIL	R5
      0x1C100805,  //  003D  EQ	R4	R4	R5
      0x78120002,  //  003E  JMPF	R4	#0042
      0x88100114,  //  003F  GETMBR	R4	R0	K20
      0x90021404,  //  0040  SETMBR	R0	K10	R4
      0x500C0200,  //  0041  LDBOOL	R3	1	0
      0x780E0001,  //  0042  JMPF	R3	#0045
      0x8C100115,  //  0043  GETMET	R4	R0	K21
      0x7C100200,  //  0044  CALL	R4	1
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_dicovery_deferred
********************************************************************/
be_local_closure(Matter_Device_start_operational_dicovery_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_operational_dicovery),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_timer),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(start_operational_dicovery_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Matter_Device_stop,   /* name */
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
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(stop),
    }),
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060002,  //  0001  JMPF	R1	#0005
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x8C040301,  //  0003  GETMET	R1	R1	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Device_invoke_request,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(plugins),
    /* K2   */  be_nested_str_weak(invoke_request),
    /* K3   */  be_nested_str_weak(status),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x58100000,  //  0000  LDCONST	R4	K0
      0x6014000C,  //  0001  GETGBL	R5	G12
      0x88180101,  //  0002  GETMBR	R6	R0	K1
      0x7C140200,  //  0003  CALL	R5	1
      0x14140805,  //  0004  LT	R5	R4	R5
      0x78160011,  //  0005  JMPF	R5	#0018
      0x88140101,  //  0006  GETMBR	R5	R0	K1
      0x94140A04,  //  0007  GETIDX	R5	R5	R4
      0x8C180B02,  //  0008  GETMET	R6	R5	K2
      0x5C200200,  //  0009  MOVE	R8	R1
      0x5C240400,  //  000A  MOVE	R9	R2
      0x5C280600,  //  000B  MOVE	R10	R3
      0x7C180800,  //  000C  CALL	R6	4
      0x4C1C0000,  //  000D  LDNIL	R7
      0x201C0C07,  //  000E  NE	R7	R6	R7
      0x741E0004,  //  000F  JMPT	R7	#0015
      0x881C0703,  //  0010  GETMBR	R7	R3	K3
      0xB8220800,  //  0011  GETNGBL	R8	K4
      0x88201105,  //  0012  GETMBR	R8	R8	K5
      0x201C0E08,  //  0013  NE	R7	R7	R8
      0x781E0000,  //  0014  JMPF	R7	#0016
      0x80040C00,  //  0015  RET	1	R6
      0x00100906,  //  0016  ADD	R4	R4	K6
      0x7001FFE8,  //  0017  JMP		#0001
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_mdns_announce_hostnames
********************************************************************/
be_local_closure(Matter_Device_start_mdns_announce_hostnames,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(_start_mdns_announce),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        /* K4   */  be_nested_str_weak(matter_device_mdns),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x50080000,  //  0002  LDBOOL	R2	0	0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020200,  //  0004  GETNGBL	R0	K1
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x58080003,  //  0006  LDCONST	R2	K3
          0x580C0004,  //  0007  LDCONST	R3	K4
          0x7C000600,  //  0008  CALL	R0	3
          0x80000000,  //  0009  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(_start_mdns_announce),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        /* K4   */  be_nested_str_weak(matter_device_mdns),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x50080200,  //  0002  LDBOOL	R2	1	0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020200,  //  0004  GETNGBL	R0	K1
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x58080003,  //  0006  LDCONST	R2	K3
          0x580C0004,  //  0007  LDCONST	R3	K4
          0x7C000600,  //  0008  CALL	R0	3
          0x80000000,  //  0009  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(wifi),
    /* K2   */  be_nested_str_weak(up),
    /* K3   */  be_nested_str_weak(_start_mdns_announce),
    /* K4   */  be_nested_str_weak(add_rule),
    /* K5   */  be_nested_str_weak(Wifi_X23Connected),
    /* K6   */  be_nested_str_weak(eth),
    /* K7   */  be_nested_str_weak(Eth_X23Connected),
    }),
    be_str_weak(start_mdns_announce_hostnames),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x94040302,  //  0003  GETIDX	R1	R1	K2
      0x78060003,  //  0004  JMPF	R1	#0009
      0x8C040103,  //  0005  GETMET	R1	R0	K3
      0x500C0000,  //  0006  LDBOOL	R3	0	0
      0x7C040400,  //  0007  CALL	R1	2
      0x70020005,  //  0008  JMP		#000F
      0xB8060000,  //  0009  GETNGBL	R1	K0
      0x8C040304,  //  000A  GETMET	R1	R1	K4
      0x580C0005,  //  000B  LDCONST	R3	K5
      0x84100000,  //  000C  CLOSURE	R4	P0
      0x5C140000,  //  000D  MOVE	R5	R0
      0x7C040800,  //  000E  CALL	R1	4
      0xB8060000,  //  000F  GETNGBL	R1	K0
      0x8C040306,  //  0010  GETMET	R1	R1	K6
      0x7C040200,  //  0011  CALL	R1	1
      0x94040302,  //  0012  GETIDX	R1	R1	K2
      0x78060003,  //  0013  JMPF	R1	#0018
      0x8C040103,  //  0014  GETMET	R1	R0	K3
      0x500C0200,  //  0015  LDBOOL	R3	1	0
      0x7C040400,  //  0016  CALL	R1	2
      0x70020005,  //  0017  JMP		#001E
      0xB8060000,  //  0018  GETNGBL	R1	K0
      0x8C040304,  //  0019  GETMET	R1	R1	K4
      0x580C0007,  //  001A  LDCONST	R3	K7
      0x84100001,  //  001B  CLOSURE	R4	P1
      0x5C140000,  //  001C  MOVE	R5	R0
      0x7C040800,  //  001D  CALL	R1	4
      0xA0000000,  //  001E  CLOSE	R0
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery_all_sessions
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery_all_sessions,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(get_deviceid),
    /* K2   */  be_nested_str_weak(get_fabric),
    /* K3   */  be_nested_str_weak(mdns_announce_op_discovery),
    /* K4   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(mdns_announce_op_discovery_all_sessions),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x88080500,  //  0002  GETMBR	R2	R2	K0
      0x7C040200,  //  0003  CALL	R1	1
      0xA802000B,  //  0004  EXBLK	0	#0011
      0x5C080200,  //  0005  MOVE	R2	R1
      0x7C080000,  //  0006  CALL	R2	0
      0x8C0C0501,  //  0007  GETMET	R3	R2	K1
      0x7C0C0200,  //  0008  CALL	R3	1
      0x780E0005,  //  0009  JMPF	R3	#0010
      0x8C0C0502,  //  000A  GETMET	R3	R2	K2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x780E0002,  //  000C  JMPF	R3	#0010
      0x8C0C0103,  //  000D  GETMET	R3	R0	K3
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x7001FFF3,  //  0010  JMP		#0005
      0x58040004,  //  0011  LDCONST	R1	K4
      0xAC040200,  //  0012  CATCH	R1	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Device_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(every_second),
    /* K2   */  be_nested_str_weak(msg_handler),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x8C040301,  //  0004  GETMET	R1	R1	K1
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Device_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 6]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_udp),
        /* K1   */  be_nested_str_weak(UDP_PORT),
        /* K2   */  be_nested_str_weak(tasmota),
        /* K3   */  be_nested_str_weak(remove_rule),
        /* K4   */  be_nested_str_weak(Wifi_X23Connected),
        /* K5   */  be_nested_str_weak(matter_device_udp),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[11]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080000,  //  0002  GETUPV	R2	U0
          0x88080501,  //  0003  GETMBR	R2	R2	K1
          0x7C000400,  //  0004  CALL	R0	2
          0xB8020400,  //  0005  GETNGBL	R0	K2
          0x8C000103,  //  0006  GETMET	R0	R0	K3
          0x58080004,  //  0007  LDCONST	R2	K4
          0x580C0005,  //  0008  LDCONST	R3	K5
          0x7C000600,  //  0009  CALL	R0	3
          0x80000000,  //  000A  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 6]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_udp),
        /* K1   */  be_nested_str_weak(UDP_PORT),
        /* K2   */  be_nested_str_weak(tasmota),
        /* K3   */  be_nested_str_weak(remove_rule),
        /* K4   */  be_nested_str_weak(Eth_X23Connected),
        /* K5   */  be_nested_str_weak(matter_device_udp),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[11]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080000,  //  0002  GETUPV	R2	U0
          0x88080501,  //  0003  GETMBR	R2	R2	K1
          0x7C000400,  //  0004  CALL	R0	2
          0xB8020400,  //  0005  GETNGBL	R0	K2
          0x8C000103,  //  0006  GETMET	R0	R0	K3
          0x58080004,  //  0007  LDCONST	R2	K4
          0x580C0005,  //  0008  LDCONST	R3	K5
          0x7C000600,  //  0009  CALL	R0	3
          0x80000000,  //  000A  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[39]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(get_option),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(MATTER_OPTION),
    /* K6   */  be_nested_str_weak(UI),
    /* K7   */  be_nested_str_weak(plugins),
    /* K8   */  be_nested_str_weak(vendorid),
    /* K9   */  be_nested_str_weak(VENDOR_ID),
    /* K10  */  be_nested_str_weak(productid),
    /* K11  */  be_nested_str_weak(PRODUCT_ID),
    /* K12  */  be_nested_str_weak(iterations),
    /* K13  */  be_nested_str_weak(PBKDF_ITERATIONS),
    /* K14  */  be_nested_str_weak(load_param),
    /* K15  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K16  */  be_nested_str_weak(random),
    /* K17  */  be_nested_str_weak(tohex),
    /* K18  */  be_nested_str_weak(commissioning_instance_eth),
    /* K19  */  be_nested_str_weak(sessions),
    /* K20  */  be_nested_str_weak(Session_Store),
    /* K21  */  be_nested_str_weak(load),
    /* K22  */  be_nested_str_weak(msg_handler),
    /* K23  */  be_nested_str_weak(MessageHandler),
    /* K24  */  be_nested_str_weak(ui),
    /* K25  */  be_nested_str_weak(push),
    /* K26  */  be_nested_str_weak(Plugin_core),
    /* K27  */  be_nested_str_weak(Plugin_Relay),
    /* K28  */  be_nested_str_weak(start_mdns_announce_hostnames),
    /* K29  */  be_nested_str_weak(wifi),
    /* K30  */  be_nested_str_weak(up),
    /* K31  */  be_nested_str_weak(start_udp),
    /* K32  */  be_nested_str_weak(UDP_PORT),
    /* K33  */  be_nested_str_weak(add_rule),
    /* K34  */  be_nested_str_weak(Wifi_X23Connected),
    /* K35  */  be_nested_str_weak(eth),
    /* K36  */  be_nested_str_weak(Eth_X23Connected),
    /* K37  */  be_nested_str_weak(start_basic_commissioning),
    /* K38  */  be_nested_str_weak(add_driver),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[107]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0xB8160800,  //  0004  GETNGBL	R5	K4
      0x88140B05,  //  0005  GETMBR	R5	R5	K5
      0x7C0C0400,  //  0006  CALL	R3	2
      0x740E0004,  //  0007  JMPT	R3	#000D
      0xB80E0800,  //  0008  GETNGBL	R3	K4
      0x8C0C0706,  //  0009  GETMET	R3	R3	K6
      0x5C140000,  //  000A  MOVE	R5	R0
      0x7C0C0400,  //  000B  CALL	R3	2
      0x80000600,  //  000C  RET	0
      0x600C0012,  //  000D  GETGBL	R3	G18
      0x7C0C0000,  //  000E  CALL	R3	0
      0x90020E03,  //  000F  SETMBR	R0	K7	R3
      0x880C0109,  //  0010  GETMBR	R3	R0	K9
      0x90021003,  //  0011  SETMBR	R0	K8	R3
      0x880C010B,  //  0012  GETMBR	R3	R0	K11
      0x90021403,  //  0013  SETMBR	R0	K10	R3
      0x880C010D,  //  0014  GETMBR	R3	R0	K13
      0x90021803,  //  0015  SETMBR	R0	K12	R3
      0x8C0C010E,  //  0016  GETMET	R3	R0	K14
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C0310,  //  0018  GETMET	R3	R1	K16
      0x54160007,  //  0019  LDINT	R5	8
      0x7C0C0400,  //  001A  CALL	R3	2
      0x8C0C0711,  //  001B  GETMET	R3	R3	K17
      0x7C0C0200,  //  001C  CALL	R3	1
      0x90021E03,  //  001D  SETMBR	R0	K15	R3
      0x8C0C0310,  //  001E  GETMET	R3	R1	K16
      0x54160007,  //  001F  LDINT	R5	8
      0x7C0C0400,  //  0020  CALL	R3	2
      0x8C0C0711,  //  0021  GETMET	R3	R3	K17
      0x7C0C0200,  //  0022  CALL	R3	1
      0x90022403,  //  0023  SETMBR	R0	K18	R3
      0xB80E0800,  //  0024  GETNGBL	R3	K4
      0x8C0C0714,  //  0025  GETMET	R3	R3	K20
      0x7C0C0200,  //  0026  CALL	R3	1
      0x90022603,  //  0027  SETMBR	R0	K19	R3
      0x880C0113,  //  0028  GETMBR	R3	R0	K19
      0x8C0C0715,  //  0029  GETMET	R3	R3	K21
      0x7C0C0200,  //  002A  CALL	R3	1
      0xB80E0800,  //  002B  GETNGBL	R3	K4
      0x8C0C0717,  //  002C  GETMET	R3	R3	K23
      0x5C140000,  //  002D  MOVE	R5	R0
      0x7C0C0400,  //  002E  CALL	R3	2
      0x90022C03,  //  002F  SETMBR	R0	K22	R3
      0xB80E0800,  //  0030  GETNGBL	R3	K4
      0x8C0C0706,  //  0031  GETMET	R3	R3	K6
      0x5C140000,  //  0032  MOVE	R5	R0
      0x7C0C0400,  //  0033  CALL	R3	2
      0x90023003,  //  0034  SETMBR	R0	K24	R3
      0x880C0107,  //  0035  GETMBR	R3	R0	K7
      0x8C0C0719,  //  0036  GETMET	R3	R3	K25
      0xB8160800,  //  0037  GETNGBL	R5	K4
      0x8C140B1A,  //  0038  GETMET	R5	R5	K26
      0x5C1C0000,  //  0039  MOVE	R7	R0
      0x7C140400,  //  003A  CALL	R5	2
      0x7C0C0400,  //  003B  CALL	R3	2
      0x880C0107,  //  003C  GETMBR	R3	R0	K7
      0x8C0C0719,  //  003D  GETMET	R3	R3	K25
      0xB8160800,  //  003E  GETNGBL	R5	K4
      0x8C140B1B,  //  003F  GETMET	R5	R5	K27
      0x5C1C0000,  //  0040  MOVE	R7	R0
      0x7C140400,  //  0041  CALL	R5	2
      0x7C0C0400,  //  0042  CALL	R3	2
      0x8C0C011C,  //  0043  GETMET	R3	R0	K28
      0x7C0C0200,  //  0044  CALL	R3	1
      0xB80E0400,  //  0045  GETNGBL	R3	K2
      0x8C0C071D,  //  0046  GETMET	R3	R3	K29
      0x7C0C0200,  //  0047  CALL	R3	1
      0x940C071E,  //  0048  GETIDX	R3	R3	K30
      0x780E0003,  //  0049  JMPF	R3	#004E
      0x8C0C011F,  //  004A  GETMET	R3	R0	K31
      0x88140120,  //  004B  GETMBR	R5	R0	K32
      0x7C0C0400,  //  004C  CALL	R3	2
      0x70020005,  //  004D  JMP		#0054
      0xB80E0400,  //  004E  GETNGBL	R3	K2
      0x8C0C0721,  //  004F  GETMET	R3	R3	K33
      0x58140022,  //  0050  LDCONST	R5	K34
      0x84180000,  //  0051  CLOSURE	R6	P0
      0x5C1C0000,  //  0052  MOVE	R7	R0
      0x7C0C0800,  //  0053  CALL	R3	4
      0xB80E0400,  //  0054  GETNGBL	R3	K2
      0x8C0C0723,  //  0055  GETMET	R3	R3	K35
      0x7C0C0200,  //  0056  CALL	R3	1
      0x940C071E,  //  0057  GETIDX	R3	R3	K30
      0x780E0003,  //  0058  JMPF	R3	#005D
      0x8C0C011F,  //  0059  GETMET	R3	R0	K31
      0x88140120,  //  005A  GETMBR	R5	R0	K32
      0x7C0C0400,  //  005B  CALL	R3	2
      0x70020005,  //  005C  JMP		#0063
      0xB80E0400,  //  005D  GETNGBL	R3	K2
      0x8C0C0721,  //  005E  GETMET	R3	R3	K33
      0x58140024,  //  005F  LDCONST	R5	K36
      0x84180001,  //  0060  CLOSURE	R6	P1
      0x5C1C0000,  //  0061  MOVE	R7	R0
      0x7C0C0800,  //  0062  CALL	R3	4
      0x8C0C0125,  //  0063  GETMET	R3	R0	K37
      0x7C0C0200,  //  0064  CALL	R3	1
      0xB80E0400,  //  0065  GETNGBL	R3	K2
      0x8C0C0726,  //  0066  GETMET	R3	R3	K38
      0x5C140000,  //  0067  MOVE	R5	R0
      0x7C0C0400,  //  0068  CALL	R3	2
      0xA0000000,  //  0069  CLOSE	R0
      0x80000000,  //  006A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_start_basic_commissioning,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(compute_pbkdf),
    /* K1   */  be_nested_str_weak(passcode),
    }),
    be_str_weak(start_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_dicovery
********************************************************************/
be_local_closure(Matter_Device_start_operational_dicovery,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(mdns),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(salt),
    /* K4   */  be_nested_str_weak(w0),
    /* K5   */  be_nested_str_weak(w1),
    /* K6   */  be_nested_str_weak(L),
    /* K7   */  be_nested_str_weak(set_no_expiration),
    /* K8   */  be_nested_str_weak(set_persist),
    /* K9   */  be_nested_str_weak(close),
    /* K10  */  be_nested_str_weak(sessions),
    /* K11  */  be_nested_str_weak(save),
    /* K12  */  be_nested_str_weak(mdns_announce_op_discovery),
    }),
    be_str_weak(start_operational_dicovery),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0x4C140000,  //  0003  LDNIL	R5
      0x90020605,  //  0004  SETMBR	R0	K3	R5
      0x4C140000,  //  0005  LDNIL	R5
      0x90020805,  //  0006  SETMBR	R0	K4	R5
      0x4C140000,  //  0007  LDNIL	R5
      0x90020A05,  //  0008  SETMBR	R0	K5	R5
      0x4C140000,  //  0009  LDNIL	R5
      0x90020C05,  //  000A  SETMBR	R0	K6	R5
      0x8C140307,  //  000B  GETMET	R5	R1	K7
      0x7C140200,  //  000C  CALL	R5	1
      0x8C140308,  //  000D  GETMET	R5	R1	K8
      0x501C0200,  //  000E  LDBOOL	R7	1	0
      0x7C140400,  //  000F  CALL	R5	2
      0x8C140309,  //  0010  GETMET	R5	R1	K9
      0x7C140200,  //  0011  CALL	R5	1
      0x8814010A,  //  0012  GETMBR	R5	R0	K10
      0x8C140B0B,  //  0013  GETMET	R5	R5	K11
      0x7C140200,  //  0014  CALL	R5	1
      0x8C14010C,  //  0015  GETMET	R5	R0	K12
      0x5C1C0200,  //  0016  MOVE	R7	R1
      0x7C140400,  //  0017  CALL	R5	2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete,   /* name */
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
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X20Commissioning_X20complete_X20_X2A_X2A_X2A),
    /* K3   */  be_const_int(2),
    }),
    be_str_weak(start_commissioning_complete),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_qrcode_content
********************************************************************/
be_local_closure(Matter_Device_compute_qrcode_content,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(resize),
    /* K1   */  be_nested_str_weak(setbits),
    /* K2   */  be_const_int(3),
    /* K3   */  be_nested_str_weak(vendorid),
    /* K4   */  be_nested_str_weak(productid),
    /* K5   */  be_nested_str_weak(discriminator),
    /* K6   */  be_nested_str_weak(passcode),
    /* K7   */  be_const_int(134217727),
    /* K8   */  be_nested_str_weak(MT_X3A),
    /* K9   */  be_nested_str_weak(matter),
    /* K10  */  be_nested_str_weak(Base38),
    /* K11  */  be_nested_str_weak(encode),
    }),
    be_str_weak(compute_qrcode_content),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x7C040000,  //  0001  CALL	R1	0
      0x8C040300,  //  0002  GETMET	R1	R1	K0
      0x540E000A,  //  0003  LDINT	R3	11
      0x7C040400,  //  0004  CALL	R1	2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x5416000F,  //  0007  LDINT	R5	16
      0x88180103,  //  0008  GETMBR	R6	R0	K3
      0x7C080800,  //  0009  CALL	R2	4
      0x8C080301,  //  000A  GETMET	R2	R1	K1
      0x54120012,  //  000B  LDINT	R4	19
      0x5416000F,  //  000C  LDINT	R5	16
      0x88180104,  //  000D  GETMBR	R6	R0	K4
      0x7C080800,  //  000E  CALL	R2	4
      0x8C080301,  //  000F  GETMET	R2	R1	K1
      0x54120024,  //  0010  LDINT	R4	37
      0x54160007,  //  0011  LDINT	R5	8
      0x541A0003,  //  0012  LDINT	R6	4
      0x7C080800,  //  0013  CALL	R2	4
      0x8C080301,  //  0014  GETMET	R2	R1	K1
      0x5412002C,  //  0015  LDINT	R4	45
      0x5416000B,  //  0016  LDINT	R5	12
      0x88180105,  //  0017  GETMBR	R6	R0	K5
      0x541E0FFE,  //  0018  LDINT	R7	4095
      0x2C180C07,  //  0019  AND	R6	R6	R7
      0x7C080800,  //  001A  CALL	R2	4
      0x8C080301,  //  001B  GETMET	R2	R1	K1
      0x54120038,  //  001C  LDINT	R4	57
      0x5416001A,  //  001D  LDINT	R5	27
      0x88180106,  //  001E  GETMBR	R6	R0	K6
      0x2C180D07,  //  001F  AND	R6	R6	K7
      0x7C080800,  //  0020  CALL	R2	4
      0xB80A1200,  //  0021  GETNGBL	R2	K9
      0x8808050A,  //  0022  GETMBR	R2	R2	K10
      0x8C08050B,  //  0023  GETMET	R2	R2	K11
      0x5C100200,  //  0024  MOVE	R4	R1
      0x7C080400,  //  0025  CALL	R2	2
      0x000A1002,  //  0026  ADD	R2	K8	R2
      0x80040400,  //  0027  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: finish_commissioning
********************************************************************/
be_local_closure(Matter_Device_finish_commissioning,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(finish_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(get_deviceid),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(reverse),
    /* K5   */  be_nested_str_weak(get_fabric_compressed),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_nested_str_weak(_X2D),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20Operational_X20Discovery_X20node_X20_X3D_X20),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(eth),
    /* K13  */  be_nested_str_weak(find),
    /* K14  */  be_nested_str_weak(up),
    /* K15  */  be_nested_str_weak(format),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20adding_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K17  */  be_nested_str_weak(hostname_eth),
    /* K18  */  be_const_int(3),
    /* K19  */  be_nested_str_weak(add_service),
    /* K20  */  be_nested_str_weak(_matter),
    /* K21  */  be_nested_str_weak(_tcp),
    /* K22  */  be_nested_str_weak(_I),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K24  */  be_nested_str_weak(add_subtype),
    /* K25  */  be_nested_str_weak(wifi),
    /* K26  */  be_nested_str_weak(hostname_wifi),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K28  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_announce_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[122]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA8020064,  //  0002  EXBLK	0	#0068
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140305,  //  0009  GETMET	R5	R1	K5
      0x7C140200,  //  000A  CALL	R5	1
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x7C180200,  //  000C  CALL	R6	1
      0x00180D07,  //  000D  ADD	R6	R6	K7
      0x8C1C0906,  //  000E  GETMET	R7	R4	K6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x00180C07,  //  0010  ADD	R6	R6	R7
      0xB81E1000,  //  0011  GETNGBL	R7	K8
      0x8C1C0F09,  //  0012  GETMET	R7	R7	K9
      0x00261406,  //  0013  ADD	R9	K10	R6
      0x5828000B,  //  0014  LDCONST	R10	K11
      0x7C1C0600,  //  0015  CALL	R7	3
      0xB81E1000,  //  0016  GETNGBL	R7	K8
      0x8C1C0F0C,  //  0017  GETMET	R7	R7	K12
      0x7C1C0200,  //  0018  CALL	R7	1
      0x8C1C0F0D,  //  0019  GETMET	R7	R7	K13
      0x5824000E,  //  001A  LDCONST	R9	K14
      0x7C1C0400,  //  001B  CALL	R7	2
      0x781E0020,  //  001C  JMPF	R7	#003E
      0xB81E1000,  //  001D  GETNGBL	R7	K8
      0x8C1C0F09,  //  001E  GETMET	R7	R7	K9
      0x8C24070F,  //  001F  GETMET	R9	R3	K15
      0x582C0010,  //  0020  LDCONST	R11	K16
      0x5830000C,  //  0021  LDCONST	R12	K12
      0x5C340C00,  //  0022  MOVE	R13	R6
      0x88380111,  //  0023  GETMBR	R14	R0	K17
      0x7C240A00,  //  0024  CALL	R9	5
      0x58280012,  //  0025  LDCONST	R10	K18
      0x7C1C0600,  //  0026  CALL	R7	3
      0x8C1C0513,  //  0027  GETMET	R7	R2	K19
      0x58240014,  //  0028  LDCONST	R9	K20
      0x58280015,  //  0029  LDCONST	R10	K21
      0x542E15A3,  //  002A  LDINT	R11	5540
      0x4C300000,  //  002B  LDNIL	R12
      0x5C340C00,  //  002C  MOVE	R13	R6
      0x88380111,  //  002D  GETMBR	R14	R0	K17
      0x7C1C0E00,  //  002E  CALL	R7	7
      0x8C1C0B06,  //  002F  GETMET	R7	R5	K6
      0x7C1C0200,  //  0030  CALL	R7	1
      0x001E2C07,  //  0031  ADD	R7	K22	R7
      0xB8221000,  //  0032  GETNGBL	R8	K8
      0x8C201109,  //  0033  GETMET	R8	R8	K9
      0x002A2E07,  //  0034  ADD	R10	K23	R7
      0x582C0012,  //  0035  LDCONST	R11	K18
      0x7C200600,  //  0036  CALL	R8	3
      0x8C200518,  //  0037  GETMET	R8	R2	K24
      0x58280014,  //  0038  LDCONST	R10	K20
      0x582C0015,  //  0039  LDCONST	R11	K21
      0x5C300C00,  //  003A  MOVE	R12	R6
      0x88340111,  //  003B  GETMBR	R13	R0	K17
      0x5C380E00,  //  003C  MOVE	R14	R7
      0x7C200C00,  //  003D  CALL	R8	6
      0xB81E1000,  //  003E  GETNGBL	R7	K8
      0x8C1C0F19,  //  003F  GETMET	R7	R7	K25
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C1C0F0D,  //  0041  GETMET	R7	R7	K13
      0x5824000E,  //  0042  LDCONST	R9	K14
      0x7C1C0400,  //  0043  CALL	R7	2
      0x781E0020,  //  0044  JMPF	R7	#0066
      0xB81E1000,  //  0045  GETNGBL	R7	K8
      0x8C1C0F09,  //  0046  GETMET	R7	R7	K9
      0x8C24070F,  //  0047  GETMET	R9	R3	K15
      0x582C0010,  //  0048  LDCONST	R11	K16
      0x58300019,  //  0049  LDCONST	R12	K25
      0x5C340C00,  //  004A  MOVE	R13	R6
      0x8838011A,  //  004B  GETMBR	R14	R0	K26
      0x7C240A00,  //  004C  CALL	R9	5
      0x58280012,  //  004D  LDCONST	R10	K18
      0x7C1C0600,  //  004E  CALL	R7	3
      0x8C1C0513,  //  004F  GETMET	R7	R2	K19
      0x58240014,  //  0050  LDCONST	R9	K20
      0x58280015,  //  0051  LDCONST	R10	K21
      0x542E15A3,  //  0052  LDINT	R11	5540
      0x4C300000,  //  0053  LDNIL	R12
      0x5C340C00,  //  0054  MOVE	R13	R6
      0x8838011A,  //  0055  GETMBR	R14	R0	K26
      0x7C1C0E00,  //  0056  CALL	R7	7
      0x8C1C0B06,  //  0057  GETMET	R7	R5	K6
      0x7C1C0200,  //  0058  CALL	R7	1
      0x001E2C07,  //  0059  ADD	R7	K22	R7
      0xB8221000,  //  005A  GETNGBL	R8	K8
      0x8C201109,  //  005B  GETMET	R8	R8	K9
      0x002A2E07,  //  005C  ADD	R10	K23	R7
      0x582C0012,  //  005D  LDCONST	R11	K18
      0x7C200600,  //  005E  CALL	R8	3
      0x8C200518,  //  005F  GETMET	R8	R2	K24
      0x58280014,  //  0060  LDCONST	R10	K20
      0x582C0015,  //  0061  LDCONST	R11	K21
      0x5C300C00,  //  0062  MOVE	R12	R6
      0x8834011A,  //  0063  GETMBR	R13	R0	K26
      0x5C380E00,  //  0064  MOVE	R14	R7
      0x7C200C00,  //  0065  CALL	R8	6
      0xA8040001,  //  0066  EXBLK	1	1
      0x70020010,  //  0067  JMP		#0079
      0xAC100002,  //  0068  CATCH	R4	0	2
      0x7002000D,  //  0069  JMP		#0078
      0xB81A1000,  //  006A  GETNGBL	R6	K8
      0x8C180D09,  //  006B  GETMET	R6	R6	K9
      0x60200008,  //  006C  GETGBL	R8	G8
      0x5C240800,  //  006D  MOVE	R9	R4
      0x7C200200,  //  006E  CALL	R8	1
      0x00223608,  //  006F  ADD	R8	K27	R8
      0x0020111C,  //  0070  ADD	R8	R8	K28
      0x60240008,  //  0071  GETGBL	R9	G8
      0x5C280A00,  //  0072  MOVE	R10	R5
      0x7C240200,  //  0073  CALL	R9	1
      0x00201009,  //  0074  ADD	R8	R8	R9
      0x5824000B,  //  0075  LDCONST	R9	K11
      0x7C180600,  //  0076  CALL	R6	3
      0x70020000,  //  0077  JMP		#0079
      0xB0080000,  //  0078  RAISE	2	R0	R0
      0x80000000,  //  0079  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete_deferred
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_commissioning_complete),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_timer),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(start_commissioning_complete_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_pbkdf
********************************************************************/
be_local_closure(Matter_Device_compute_pbkdf,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(salt),
    /* K2   */  be_nested_str_weak(random),
    /* K3   */  be_nested_str_weak(add),
    /* K4   */  be_nested_str_weak(PBKDF2_HMAC_SHA256),
    /* K5   */  be_nested_str_weak(derive),
    /* K6   */  be_nested_str_weak(iterations),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(w0),
    /* K9   */  be_nested_str_weak(EC_P256),
    /* K10  */  be_nested_str_weak(mod),
    /* K11  */  be_nested_str_weak(w1),
    /* K12  */  be_nested_str_weak(L),
    /* K13  */  be_nested_str_weak(public_key),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20salt_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K19  */  be_nested_str_weak(tohex),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20passcode_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20w0_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20w1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20L_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    }),
    be_str_weak(compute_pbkdf),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0502,  //  0001  GETMET	R3	R2	K2
      0x5416000F,  //  0002  LDINT	R5	16
      0x7C0C0400,  //  0003  CALL	R3	2
      0x90020203,  //  0004  SETMBR	R0	K1	R3
      0x600C0015,  //  0005  GETGBL	R3	G21
      0x7C0C0000,  //  0006  CALL	R3	0
      0x8C0C0703,  //  0007  GETMET	R3	R3	K3
      0x5C140200,  //  0008  MOVE	R5	R1
      0x541A0003,  //  0009  LDINT	R6	4
      0x7C0C0600,  //  000A  CALL	R3	3
      0x8C100504,  //  000B  GETMET	R4	R2	K4
      0x7C100200,  //  000C  CALL	R4	1
      0x8C100905,  //  000D  GETMET	R4	R4	K5
      0x5C180600,  //  000E  MOVE	R6	R3
      0x881C0101,  //  000F  GETMBR	R7	R0	K1
      0x88200106,  //  0010  GETMBR	R8	R0	K6
      0x5426004F,  //  0011  LDINT	R9	80
      0x7C100A00,  //  0012  CALL	R4	5
      0x54160026,  //  0013  LDINT	R5	39
      0x40160E05,  //  0014  CONNECT	R5	K7	R5
      0x94140805,  //  0015  GETIDX	R5	R4	R5
      0x541A0027,  //  0016  LDINT	R6	40
      0x541E004E,  //  0017  LDINT	R7	79
      0x40180C07,  //  0018  CONNECT	R6	R6	R7
      0x94180806,  //  0019  GETIDX	R6	R4	R6
      0x8C1C0509,  //  001A  GETMET	R7	R2	K9
      0x7C1C0200,  //  001B  CALL	R7	1
      0x8C1C0F0A,  //  001C  GETMET	R7	R7	K10
      0x5C240A00,  //  001D  MOVE	R9	R5
      0x7C1C0400,  //  001E  CALL	R7	2
      0x90021007,  //  001F  SETMBR	R0	K8	R7
      0x8C1C0509,  //  0020  GETMET	R7	R2	K9
      0x7C1C0200,  //  0021  CALL	R7	1
      0x8C1C0F0A,  //  0022  GETMET	R7	R7	K10
      0x5C240C00,  //  0023  MOVE	R9	R6
      0x7C1C0400,  //  0024  CALL	R7	2
      0x90021607,  //  0025  SETMBR	R0	K11	R7
      0x8C1C0509,  //  0026  GETMET	R7	R2	K9
      0x7C1C0200,  //  0027  CALL	R7	1
      0x8C1C0F0D,  //  0028  GETMET	R7	R7	K13
      0x8824010B,  //  0029  GETMBR	R9	R0	K11
      0x7C1C0400,  //  002A  CALL	R7	2
      0x90021807,  //  002B  SETMBR	R0	K12	R7
      0xB81E1C00,  //  002C  GETNGBL	R7	K14
      0x8C1C0F0F,  //  002D  GETMET	R7	R7	K15
      0x58240010,  //  002E  LDCONST	R9	K16
      0x58280011,  //  002F  LDCONST	R10	K17
      0x7C1C0600,  //  0030  CALL	R7	3
      0xB81E1C00,  //  0031  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0032  GETMET	R7	R7	K15
      0x88240101,  //  0033  GETMBR	R9	R0	K1
      0x8C241313,  //  0034  GETMET	R9	R9	K19
      0x7C240200,  //  0035  CALL	R9	1
      0x00262409,  //  0036  ADD	R9	K18	R9
      0x58280011,  //  0037  LDCONST	R10	K17
      0x7C1C0600,  //  0038  CALL	R7	3
      0xB81E1C00,  //  0039  GETNGBL	R7	K14
      0x8C1C0F0F,  //  003A  GETMET	R7	R7	K15
      0x8C240713,  //  003B  GETMET	R9	R3	K19
      0x7C240200,  //  003C  CALL	R9	1
      0x00262809,  //  003D  ADD	R9	K20	R9
      0x58280011,  //  003E  LDCONST	R10	K17
      0x7C1C0600,  //  003F  CALL	R7	3
      0xB81E1C00,  //  0040  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0041  GETMET	R7	R7	K15
      0x88240108,  //  0042  GETMBR	R9	R0	K8
      0x8C241313,  //  0043  GETMET	R9	R9	K19
      0x7C240200,  //  0044  CALL	R9	1
      0x00262A09,  //  0045  ADD	R9	K21	R9
      0x58280011,  //  0046  LDCONST	R10	K17
      0x7C1C0600,  //  0047  CALL	R7	3
      0xB81E1C00,  //  0048  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0049  GETMET	R7	R7	K15
      0x8824010B,  //  004A  GETMBR	R9	R0	K11
      0x8C241313,  //  004B  GETMET	R9	R9	K19
      0x7C240200,  //  004C  CALL	R9	1
      0x00262C09,  //  004D  ADD	R9	K22	R9
      0x58280011,  //  004E  LDCONST	R10	K17
      0x7C1C0600,  //  004F  CALL	R7	3
      0xB81E1C00,  //  0050  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0051  GETMET	R7	R7	K15
      0x8824010C,  //  0052  GETMBR	R9	R0	K12
      0x8C241313,  //  0053  GETMET	R9	R9	K19
      0x7C240200,  //  0054  CALL	R9	1
      0x00262E09,  //  0055  ADD	R9	K23	R9
      0x58280011,  //  0056  LDCONST	R10	K17
      0x7C1C0600,  //  0057  CALL	R7	3
      0xB81E1C00,  //  0058  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0059  GETMET	R7	R7	K15
      0x58240010,  //  005A  LDCONST	R9	K16
      0x58280011,  //  005B  LDCONST	R10	K17
      0x7C1C0600,  //  005C  CALL	R7	3
      0x80000000,  //  005D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active_endpoints
********************************************************************/
be_local_closure(Matter_Device_get_active_endpoints,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins),
    /* K1   */  be_nested_str_weak(get_endpoints),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_active_endpoints),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA802001B,  //  0005  EXBLK	0	#0022
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140901,  //  0008  GETMET	R5	R4	K1
      0x7C140200,  //  0009  CALL	R5	1
      0x60180010,  //  000A  GETGBL	R6	G16
      0x5C1C0A00,  //  000B  MOVE	R7	R5
      0x7C180200,  //  000C  CALL	R6	1
      0xA802000F,  //  000D  EXBLK	0	#001E
      0x5C1C0C00,  //  000E  MOVE	R7	R6
      0x7C1C0000,  //  000F  CALL	R7	0
      0x78060002,  //  0010  JMPF	R1	#0014
      0x1C200F02,  //  0011  EQ	R8	R7	K2
      0x78220000,  //  0012  JMPF	R8	#0014
      0x7001FFF9,  //  0013  JMP		#000E
      0x8C200503,  //  0014  GETMET	R8	R2	K3
      0x5C280E00,  //  0015  MOVE	R10	R7
      0x7C200400,  //  0016  CALL	R8	2
      0x4C240000,  //  0017  LDNIL	R9
      0x1C201009,  //  0018  EQ	R8	R8	R9
      0x78220002,  //  0019  JMPF	R8	#001D
      0x8C200504,  //  001A  GETMET	R8	R2	K4
      0x5C280E00,  //  001B  MOVE	R10	R7
      0x7C200400,  //  001C  CALL	R8	2
      0x7001FFEF,  //  001D  JMP		#000E
      0x58180005,  //  001E  LDCONST	R6	K5
      0xAC180200,  //  001F  CATCH	R6	1	0
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0x7001FFE3,  //  0021  JMP		#0006
      0x580C0005,  //  0022  LDCONST	R3	K5
      0xAC0C0200,  //  0023  CATCH	R3	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x80040400,  //  0025  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(Matter_Device_msg_received,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(msg_handler),
    /* K1   */  be_nested_str_weak(msg_received),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0x5C180200,  //  0002  MOVE	R6	R1
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x5C200600,  //  0004  MOVE	R8	R3
      0x7C100800,  //  0005  CALL	R4	4
      0x80040800,  //  0006  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _start_mdns_announce
********************************************************************/
be_local_closure(Matter_Device__start_mdns_announce,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[48]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(start),
    /* K3   */  be_nested_str_weak(VP),
    /* K4   */  be_nested_str_weak(vendorid),
    /* K5   */  be_nested_str_weak(_X2B),
    /* K6   */  be_nested_str_weak(productid),
    /* K7   */  be_nested_str_weak(D),
    /* K8   */  be_nested_str_weak(discriminator),
    /* K9   */  be_nested_str_weak(CM),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(T),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str_weak(SII),
    /* K14  */  be_nested_str_weak(SAI),
    /* K15  */  be_nested_str_weak(tasmota),
    /* K16  */  be_nested_str_weak(eth),
    /* K17  */  be_nested_str_weak(hostname_eth),
    /* K18  */  be_nested_str_weak(replace),
    /* K19  */  be_nested_str_weak(find),
    /* K20  */  be_nested_str_weak(mac),
    /* K21  */  be_nested_str_weak(_X3A),
    /* K22  */  be_nested_str_weak(),
    /* K23  */  be_nested_str_weak(add_hostname),
    /* K24  */  be_nested_str_weak(ip6local),
    /* K25  */  be_nested_str_weak(ip),
    /* K26  */  be_nested_str_weak(ip6),
    /* K27  */  be_nested_str_weak(add_service),
    /* K28  */  be_nested_str_weak(_matterc),
    /* K29  */  be_nested_str_weak(_udp),
    /* K30  */  be_nested_str_weak(commissioning_instance_eth),
    /* K31  */  be_nested_str_weak(log),
    /* K32  */  be_nested_str_weak(format),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20starting_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K34  */  be_nested_str_weak(wifi),
    /* K35  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K36  */  be_nested_str_weak(hostname_wifi),
    /* K37  */  be_const_int(2),
    /* K38  */  be_nested_str_weak(_L),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K40  */  be_const_int(3),
    /* K41  */  be_nested_str_weak(add_subtype),
    /* K42  */  be_nested_str_weak(_S),
    /* K43  */  be_nested_str_weak(_V),
    /* K44  */  be_nested_str_weak(_CM1),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K46  */  be_nested_str_weak(_X7C),
    /* K47  */  be_nested_str_weak(mdns_announce_op_discovery_all_sessions),
    }),
    be_str_weak(_start_mdns_announce),
    &be_const_str_solidified,
    ( &(const binstruction[292]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100502,  //  0002  GETMET	R4	R2	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x60100013,  //  0004  GETGBL	R4	G19
      0x7C100000,  //  0005  CALL	R4	0
      0x60140008,  //  0006  GETGBL	R5	G8
      0x88180104,  //  0007  GETMBR	R6	R0	K4
      0x7C140200,  //  0008  CALL	R5	1
      0x00140B05,  //  0009  ADD	R5	R5	K5
      0x60180008,  //  000A  GETGBL	R6	G8
      0x881C0106,  //  000B  GETMBR	R7	R0	K6
      0x7C180200,  //  000C  CALL	R6	1
      0x00140A06,  //  000D  ADD	R5	R5	R6
      0x98120605,  //  000E  SETIDX	R4	K3	R5
      0x88140108,  //  000F  GETMBR	R5	R0	K8
      0x98120E05,  //  0010  SETIDX	R4	K7	R5
      0x9812130A,  //  0011  SETIDX	R4	K9	K10
      0x9812170C,  //  0012  SETIDX	R4	K11	K12
      0x54161387,  //  0013  LDINT	R5	5000
      0x98121A05,  //  0014  SETIDX	R4	K13	R5
      0x5416012B,  //  0015  LDINT	R5	300
      0x98121C05,  //  0016  SETIDX	R4	K14	R5
      0xA80200F7,  //  0017  EXBLK	0	#0110
      0x7806007A,  //  0018  JMPF	R1	#0094
      0xB8161E00,  //  0019  GETNGBL	R5	K15
      0x8C140B10,  //  001A  GETMET	R5	R5	K16
      0x7C140200,  //  001B  CALL	R5	1
      0x8C180712,  //  001C  GETMET	R6	R3	K18
      0x8C200B13,  //  001D  GETMET	R8	R5	K19
      0x58280014,  //  001E  LDCONST	R10	K20
      0x7C200400,  //  001F  CALL	R8	2
      0x58240015,  //  0020  LDCONST	R9	K21
      0x58280016,  //  0021  LDCONST	R10	K22
      0x7C180800,  //  0022  CALL	R6	4
      0x90022206,  //  0023  SETMBR	R0	K17	R6
      0x8C180517,  //  0024  GETMET	R6	R2	K23
      0x88200111,  //  0025  GETMBR	R8	R0	K17
      0x8C240B13,  //  0026  GETMET	R9	R5	K19
      0x582C0018,  //  0027  LDCONST	R11	K24
      0x58300016,  //  0028  LDCONST	R12	K22
      0x7C240600,  //  0029  CALL	R9	3
      0x8C280B13,  //  002A  GETMET	R10	R5	K19
      0x58300019,  //  002B  LDCONST	R12	K25
      0x58340016,  //  002C  LDCONST	R13	K22
      0x7C280600,  //  002D  CALL	R10	3
      0x8C2C0B13,  //  002E  GETMET	R11	R5	K19
      0x5834001A,  //  002F  LDCONST	R13	K26
      0x58380016,  //  0030  LDCONST	R14	K22
      0x7C2C0600,  //  0031  CALL	R11	3
      0x7C180A00,  //  0032  CALL	R6	5
      0x8C18051B,  //  0033  GETMET	R6	R2	K27
      0x5820001C,  //  0034  LDCONST	R8	K28
      0x5824001D,  //  0035  LDCONST	R9	K29
      0x542A15A3,  //  0036  LDINT	R10	5540
      0x5C2C0800,  //  0037  MOVE	R11	R4
      0x8830011E,  //  0038  GETMBR	R12	R0	K30
      0x88340111,  //  0039  GETMBR	R13	R0	K17
      0x7C180E00,  //  003A  CALL	R6	7
      0xB81A1E00,  //  003B  GETNGBL	R6	K15
      0x8C180D1F,  //  003C  GETMET	R6	R6	K31
      0x8C200720,  //  003D  GETMET	R8	R3	K32
      0x58280021,  //  003E  LDCONST	R10	K33
      0x78060001,  //  003F  JMPF	R1	#0042
      0x582C0010,  //  0040  LDCONST	R11	K16
      0x70020000,  //  0041  JMP		#0043
      0x582C0022,  //  0042  LDCONST	R11	K34
      0x78060001,  //  0043  JMPF	R1	#0046
      0x8830011E,  //  0044  GETMBR	R12	R0	K30
      0x70020000,  //  0045  JMP		#0047
      0x88300123,  //  0046  GETMBR	R12	R0	K35
      0x78060001,  //  0047  JMPF	R1	#004A
      0x88340111,  //  0048  GETMBR	R13	R0	K17
      0x70020000,  //  0049  JMP		#004B
      0x88340124,  //  004A  GETMBR	R13	R0	K36
      0x7C200A00,  //  004B  CALL	R8	5
      0x58240025,  //  004C  LDCONST	R9	K37
      0x7C180600,  //  004D  CALL	R6	3
      0x60180008,  //  004E  GETGBL	R6	G8
      0x881C0108,  //  004F  GETMBR	R7	R0	K8
      0x54220FFE,  //  0050  LDINT	R8	4095
      0x2C1C0E08,  //  0051  AND	R7	R7	R8
      0x7C180200,  //  0052  CALL	R6	1
      0x001A4C06,  //  0053  ADD	R6	K38	R6
      0xB81E1E00,  //  0054  GETNGBL	R7	K15
      0x8C1C0F1F,  //  0055  GETMET	R7	R7	K31
      0x00264E06,  //  0056  ADD	R9	K39	R6
      0x58280028,  //  0057  LDCONST	R10	K40
      0x7C1C0600,  //  0058  CALL	R7	3
      0x8C1C0529,  //  0059  GETMET	R7	R2	K41
      0x5824001C,  //  005A  LDCONST	R9	K28
      0x5828001D,  //  005B  LDCONST	R10	K29
      0x882C011E,  //  005C  GETMBR	R11	R0	K30
      0x88300111,  //  005D  GETMBR	R12	R0	K17
      0x5C340C00,  //  005E  MOVE	R13	R6
      0x7C1C0C00,  //  005F  CALL	R7	6
      0x601C0008,  //  0060  GETGBL	R7	G8
      0x88200108,  //  0061  GETMBR	R8	R0	K8
      0x54260EFF,  //  0062  LDINT	R9	3840
      0x2C201009,  //  0063  AND	R8	R8	R9
      0x54260007,  //  0064  LDINT	R9	8
      0x3C201009,  //  0065  SHR	R8	R8	R9
      0x7C1C0200,  //  0066  CALL	R7	1
      0x001E5407,  //  0067  ADD	R7	K42	R7
      0x5C180E00,  //  0068  MOVE	R6	R7
      0xB81E1E00,  //  0069  GETNGBL	R7	K15
      0x8C1C0F1F,  //  006A  GETMET	R7	R7	K31
      0x00264E06,  //  006B  ADD	R9	K39	R6
      0x58280028,  //  006C  LDCONST	R10	K40
      0x7C1C0600,  //  006D  CALL	R7	3
      0x8C1C0529,  //  006E  GETMET	R7	R2	K41
      0x5824001C,  //  006F  LDCONST	R9	K28
      0x5828001D,  //  0070  LDCONST	R10	K29
      0x882C011E,  //  0071  GETMBR	R11	R0	K30
      0x88300111,  //  0072  GETMBR	R12	R0	K17
      0x5C340C00,  //  0073  MOVE	R13	R6
      0x7C1C0C00,  //  0074  CALL	R7	6
      0x601C0008,  //  0075  GETGBL	R7	G8
      0x88200104,  //  0076  GETMBR	R8	R0	K4
      0x7C1C0200,  //  0077  CALL	R7	1
      0x001E5607,  //  0078  ADD	R7	K43	R7
      0x5C180E00,  //  0079  MOVE	R6	R7
      0xB81E1E00,  //  007A  GETNGBL	R7	K15
      0x8C1C0F1F,  //  007B  GETMET	R7	R7	K31
      0x00264E06,  //  007C  ADD	R9	K39	R6
      0x58280028,  //  007D  LDCONST	R10	K40
      0x7C1C0600,  //  007E  CALL	R7	3
      0x8C1C0529,  //  007F  GETMET	R7	R2	K41
      0x5824001C,  //  0080  LDCONST	R9	K28
      0x5828001D,  //  0081  LDCONST	R10	K29
      0x882C011E,  //  0082  GETMBR	R11	R0	K30
      0x88300111,  //  0083  GETMBR	R12	R0	K17
      0x5C340C00,  //  0084  MOVE	R13	R6
      0x7C1C0C00,  //  0085  CALL	R7	6
      0x5818002C,  //  0086  LDCONST	R6	K44
      0xB81E1E00,  //  0087  GETNGBL	R7	K15
      0x8C1C0F1F,  //  0088  GETMET	R7	R7	K31
      0x00264E06,  //  0089  ADD	R9	K39	R6
      0x58280028,  //  008A  LDCONST	R10	K40
      0x7C1C0600,  //  008B  CALL	R7	3
      0x8C1C0529,  //  008C  GETMET	R7	R2	K41
      0x5824001C,  //  008D  LDCONST	R9	K28
      0x5828001D,  //  008E  LDCONST	R10	K29
      0x882C011E,  //  008F  GETMBR	R11	R0	K30
      0x88300111,  //  0090  GETMBR	R12	R0	K17
      0x5C340C00,  //  0091  MOVE	R13	R6
      0x7C1C0C00,  //  0092  CALL	R7	6
      0x70020079,  //  0093  JMP		#010E
      0xB8161E00,  //  0094  GETNGBL	R5	K15
      0x8C140B22,  //  0095  GETMET	R5	R5	K34
      0x7C140200,  //  0096  CALL	R5	1
      0x8C180712,  //  0097  GETMET	R6	R3	K18
      0x8C200B13,  //  0098  GETMET	R8	R5	K19
      0x58280014,  //  0099  LDCONST	R10	K20
      0x7C200400,  //  009A  CALL	R8	2
      0x58240015,  //  009B  LDCONST	R9	K21
      0x58280016,  //  009C  LDCONST	R10	K22
      0x7C180800,  //  009D  CALL	R6	4
      0x90024806,  //  009E  SETMBR	R0	K36	R6
      0x8C180517,  //  009F  GETMET	R6	R2	K23
      0x88200124,  //  00A0  GETMBR	R8	R0	K36
      0x8C240B13,  //  00A1  GETMET	R9	R5	K19
      0x582C0018,  //  00A2  LDCONST	R11	K24
      0x58300016,  //  00A3  LDCONST	R12	K22
      0x7C240600,  //  00A4  CALL	R9	3
      0x8C280B13,  //  00A5  GETMET	R10	R5	K19
      0x58300019,  //  00A6  LDCONST	R12	K25
      0x58340016,  //  00A7  LDCONST	R13	K22
      0x7C280600,  //  00A8  CALL	R10	3
      0x8C2C0B13,  //  00A9  GETMET	R11	R5	K19
      0x5834001A,  //  00AA  LDCONST	R13	K26
      0x58380016,  //  00AB  LDCONST	R14	K22
      0x7C2C0600,  //  00AC  CALL	R11	3
      0x7C180A00,  //  00AD  CALL	R6	5
      0x8C18051B,  //  00AE  GETMET	R6	R2	K27
      0x5820001C,  //  00AF  LDCONST	R8	K28
      0x5824001D,  //  00B0  LDCONST	R9	K29
      0x542A15A3,  //  00B1  LDINT	R10	5540
      0x5C2C0800,  //  00B2  MOVE	R11	R4
      0x88300123,  //  00B3  GETMBR	R12	R0	K35
      0x88340124,  //  00B4  GETMBR	R13	R0	K36
      0x7C180E00,  //  00B5  CALL	R6	7
      0xB81A1E00,  //  00B6  GETNGBL	R6	K15
      0x8C180D1F,  //  00B7  GETMET	R6	R6	K31
      0x8C200720,  //  00B8  GETMET	R8	R3	K32
      0x58280021,  //  00B9  LDCONST	R10	K33
      0x78060001,  //  00BA  JMPF	R1	#00BD
      0x582C0010,  //  00BB  LDCONST	R11	K16
      0x70020000,  //  00BC  JMP		#00BE
      0x582C0022,  //  00BD  LDCONST	R11	K34
      0x78060001,  //  00BE  JMPF	R1	#00C1
      0x8830011E,  //  00BF  GETMBR	R12	R0	K30
      0x70020000,  //  00C0  JMP		#00C2
      0x88300123,  //  00C1  GETMBR	R12	R0	K35
      0x78060001,  //  00C2  JMPF	R1	#00C5
      0x88340111,  //  00C3  GETMBR	R13	R0	K17
      0x70020000,  //  00C4  JMP		#00C6
      0x88340124,  //  00C5  GETMBR	R13	R0	K36
      0x7C200A00,  //  00C6  CALL	R8	5
      0x58240025,  //  00C7  LDCONST	R9	K37
      0x7C180600,  //  00C8  CALL	R6	3
      0x60180008,  //  00C9  GETGBL	R6	G8
      0x881C0108,  //  00CA  GETMBR	R7	R0	K8
      0x54220FFE,  //  00CB  LDINT	R8	4095
      0x2C1C0E08,  //  00CC  AND	R7	R7	R8
      0x7C180200,  //  00CD  CALL	R6	1
      0x001A4C06,  //  00CE  ADD	R6	K38	R6
      0xB81E1E00,  //  00CF  GETNGBL	R7	K15
      0x8C1C0F1F,  //  00D0  GETMET	R7	R7	K31
      0x00264E06,  //  00D1  ADD	R9	K39	R6
      0x58280028,  //  00D2  LDCONST	R10	K40
      0x7C1C0600,  //  00D3  CALL	R7	3
      0x8C1C0529,  //  00D4  GETMET	R7	R2	K41
      0x5824001C,  //  00D5  LDCONST	R9	K28
      0x5828001D,  //  00D6  LDCONST	R10	K29
      0x882C0123,  //  00D7  GETMBR	R11	R0	K35
      0x88300124,  //  00D8  GETMBR	R12	R0	K36
      0x5C340C00,  //  00D9  MOVE	R13	R6
      0x7C1C0C00,  //  00DA  CALL	R7	6
      0x601C0008,  //  00DB  GETGBL	R7	G8
      0x88200108,  //  00DC  GETMBR	R8	R0	K8
      0x54260EFF,  //  00DD  LDINT	R9	3840
      0x2C201009,  //  00DE  AND	R8	R8	R9
      0x54260007,  //  00DF  LDINT	R9	8
      0x3C201009,  //  00E0  SHR	R8	R8	R9
      0x7C1C0200,  //  00E1  CALL	R7	1
      0x001E5407,  //  00E2  ADD	R7	K42	R7
      0x5C180E00,  //  00E3  MOVE	R6	R7
      0xB81E1E00,  //  00E4  GETNGBL	R7	K15
      0x8C1C0F1F,  //  00E5  GETMET	R7	R7	K31
      0x00264E06,  //  00E6  ADD	R9	K39	R6
      0x58280028,  //  00E7  LDCONST	R10	K40
      0x7C1C0600,  //  00E8  CALL	R7	3
      0x8C1C0529,  //  00E9  GETMET	R7	R2	K41
      0x5824001C,  //  00EA  LDCONST	R9	K28
      0x5828001D,  //  00EB  LDCONST	R10	K29
      0x882C0123,  //  00EC  GETMBR	R11	R0	K35
      0x88300124,  //  00ED  GETMBR	R12	R0	K36
      0x5C340C00,  //  00EE  MOVE	R13	R6
      0x7C1C0C00,  //  00EF  CALL	R7	6
      0x601C0008,  //  00F0  GETGBL	R7	G8
      0x88200104,  //  00F1  GETMBR	R8	R0	K4
      0x7C1C0200,  //  00F2  CALL	R7	1
      0x001E5607,  //  00F3  ADD	R7	K43	R7
      0x5C180E00,  //  00F4  MOVE	R6	R7
      0xB81E1E00,  //  00F5  GETNGBL	R7	K15
      0x8C1C0F1F,  //  00F6  GETMET	R7	R7	K31
      0x00264E06,  //  00F7  ADD	R9	K39	R6
      0x58280028,  //  00F8  LDCONST	R10	K40
      0x7C1C0600,  //  00F9  CALL	R7	3
      0x8C1C0529,  //  00FA  GETMET	R7	R2	K41
      0x5824001C,  //  00FB  LDCONST	R9	K28
      0x5828001D,  //  00FC  LDCONST	R10	K29
      0x882C0123,  //  00FD  GETMBR	R11	R0	K35
      0x88300124,  //  00FE  GETMBR	R12	R0	K36
      0x5C340C00,  //  00FF  MOVE	R13	R6
      0x7C1C0C00,  //  0100  CALL	R7	6
      0x5818002C,  //  0101  LDCONST	R6	K44
      0xB81E1E00,  //  0102  GETNGBL	R7	K15
      0x8C1C0F1F,  //  0103  GETMET	R7	R7	K31
      0x00264E06,  //  0104  ADD	R9	K39	R6
      0x58280028,  //  0105  LDCONST	R10	K40
      0x7C1C0600,  //  0106  CALL	R7	3
      0x8C1C0529,  //  0107  GETMET	R7	R2	K41
      0x5824001C,  //  0108  LDCONST	R9	K28
      0x5828001D,  //  0109  LDCONST	R10	K29
      0x882C0123,  //  010A  GETMBR	R11	R0	K35
      0x88300124,  //  010B  GETMBR	R12	R0	K36
      0x5C340C00,  //  010C  MOVE	R13	R6
      0x7C1C0C00,  //  010D  CALL	R7	6
      0xA8040001,  //  010E  EXBLK	1	1
      0x70020010,  //  010F  JMP		#0121
      0xAC140002,  //  0110  CATCH	R5	0	2
      0x7002000D,  //  0111  JMP		#0120
      0xB81E1E00,  //  0112  GETNGBL	R7	K15
      0x8C1C0F1F,  //  0113  GETMET	R7	R7	K31
      0x60240008,  //  0114  GETGBL	R9	G8
      0x5C280A00,  //  0115  MOVE	R10	R5
      0x7C240200,  //  0116  CALL	R9	1
      0x00265A09,  //  0117  ADD	R9	K45	R9
      0x0024132E,  //  0118  ADD	R9	R9	K46
      0x60280008,  //  0119  GETGBL	R10	G8
      0x5C2C0C00,  //  011A  MOVE	R11	R6
      0x7C280200,  //  011B  CALL	R10	1
      0x0024120A,  //  011C  ADD	R9	R9	R10
      0x58280025,  //  011D  LDCONST	R10	K37
      0x7C1C0600,  //  011E  CALL	R7	3
      0x70020000,  //  011F  JMP		#0121
      0xB0080000,  //  0120  RAISE	2	R0	R0
      0x8C14012F,  //  0121  GETMET	R5	R0	K47
      0x7C140200,  //  0122  CALL	R5	1
      0x80000000,  //  0123  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: packet_ack
********************************************************************/
be_local_closure(Matter_Device_packet_ack,   /* name */
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
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(packet_ack),
    }),
    be_str_weak(packet_ack),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_send
********************************************************************/
be_local_closure(Matter_Device_msg_send,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(send_response),
    }),
    be_str_weak(msg_send),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x8C140B01,  //  0001  GETMET	R5	R5	K1
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x5C200400,  //  0003  MOVE	R8	R2
      0x5C240600,  //  0004  MOVE	R9	R3
      0x5C280800,  //  0005  MOVE	R10	R4
      0x7C140A00,  //  0006  CALL	R5	5
      0x80040A00,  //  0007  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Device
********************************************************************/
be_local_class(Matter_Device,
    18,
    NULL,
    be_nested_map(49,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(compute_manual_pairing_code, 12), be_const_closure(Matter_Device_compute_manual_pairing_code_closure) },
        { be_const_key_weak(msg_handler, 45), be_const_var(2) },
        { be_const_key_weak(FILENAME, 20), be_nested_str_weak(_matter_device_X2Ejson) },
        { be_const_key_weak(plugins, 40), be_const_var(0) },
        { be_const_key_weak(process_attribute_expansion, -1), be_const_closure(Matter_Device_process_attribute_expansion_closure) },
        { be_const_key_weak(msg_send, -1), be_const_closure(Matter_Device_msg_send_closure) },
        { be_const_key_weak(packet_ack, -1), be_const_closure(Matter_Device_packet_ack_closure) },
        { be_const_key_weak(save_param, 48), be_const_closure(Matter_Device_save_param_closure) },
        { be_const_key_weak(_start_mdns_announce, -1), be_const_closure(Matter_Device__start_mdns_announce_closure) },
        { be_const_key_weak(start_operational_dicovery_deferred, -1), be_const_closure(Matter_Device_start_operational_dicovery_deferred_closure) },
        { be_const_key_weak(ui, 47), be_const_var(4) },
        { be_const_key_weak(w0, -1), be_const_var(15) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Device_invoke_request_closure) },
        { be_const_key_weak(passcode, 15), be_const_var(12) },
        { be_const_key_weak(start_mdns_announce_hostnames, -1), be_const_closure(Matter_Device_start_mdns_announce_hostnames_closure) },
        { be_const_key_weak(hostname_eth, 31), be_const_var(8) },
        { be_const_key_weak(L, -1), be_const_var(17) },
        { be_const_key_weak(every_second, 8), be_const_closure(Matter_Device_every_second_closure) },
        { be_const_key_weak(VENDOR_ID, -1), be_const_int(65521) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Device_init_closure) },
        { be_const_key_weak(get_active_endpoints, -1), be_const_closure(Matter_Device_get_active_endpoints_closure) },
        { be_const_key_weak(iterations, 43), be_const_var(13) },
        { be_const_key_weak(commissioning_instance_eth, -1), be_const_var(6) },
        { be_const_key_weak(start_operational_dicovery, 41), be_const_closure(Matter_Device_start_operational_dicovery_closure) },
        { be_const_key_weak(start_commissioning_complete, 16), be_const_closure(Matter_Device_start_commissioning_complete_closure) },
        { be_const_key_weak(start_basic_commissioning, 42), be_const_closure(Matter_Device_start_basic_commissioning_closure) },
        { be_const_key_weak(productid, -1), be_const_var(10) },
        { be_const_key_weak(PRODUCT_ID, -1), be_const_int(32768) },
        { be_const_key_weak(PBKDF_ITERATIONS, 36), be_const_int(1000) },
        { be_const_key_weak(salt, 5), be_const_var(14) },
        { be_const_key_weak(w1, -1), be_const_var(16) },
        { be_const_key_weak(start_commissioning_complete_deferred, -1), be_const_closure(Matter_Device_start_commissioning_complete_deferred_closure) },
        { be_const_key_weak(mdns_announce_op_discovery, 27), be_const_closure(Matter_Device_mdns_announce_op_discovery_closure) },
        { be_const_key_weak(discriminator, -1), be_const_var(11) },
        { be_const_key_weak(commissioning_instance_wifi, -1), be_const_var(5) },
        { be_const_key_weak(start_udp, 34), be_const_closure(Matter_Device_start_udp_closure) },
        { be_const_key_weak(UDP_PORT, -1), be_const_int(5540) },
        { be_const_key_weak(compute_pbkdf, -1), be_const_closure(Matter_Device_compute_pbkdf_closure) },
        { be_const_key_weak(hostname_wifi, -1), be_const_var(7) },
        { be_const_key_weak(udp_server, 2), be_const_var(1) },
        { be_const_key_weak(finish_commissioning, -1), be_const_closure(Matter_Device_finish_commissioning_closure) },
        { be_const_key_weak(compute_qrcode_content, -1), be_const_closure(Matter_Device_compute_qrcode_content_closure) },
        { be_const_key_weak(PASSCODE_DEFAULT, -1), be_const_int(20202021) },
        { be_const_key_weak(vendorid, 44), be_const_var(9) },
        { be_const_key_weak(sessions, -1), be_const_var(3) },
        { be_const_key_weak(mdns_announce_op_discovery_all_sessions, -1), be_const_closure(Matter_Device_mdns_announce_op_discovery_all_sessions_closure) },
        { be_const_key_weak(msg_received, 6), be_const_closure(Matter_Device_msg_received_closure) },
        { be_const_key_weak(stop, -1), be_const_closure(Matter_Device_stop_closure) },
        { be_const_key_weak(load_param, -1), be_const_closure(Matter_Device_load_param_closure) },
    })),
    be_str_weak(Matter_Device)
);
/*******************************************************************/

void be_load_Matter_Device_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Device);
    be_setglobal(vm, "Matter_Device");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
