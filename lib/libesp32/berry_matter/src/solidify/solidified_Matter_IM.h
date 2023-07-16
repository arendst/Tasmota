/* Solidification of Matter_IM.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM;

/********************************************************************
** Solidified function: expire_sendqueue
********************************************************************/
be_local_closure(Matter_IM_expire_sendqueue,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(time_reached),
    /* K4   */  be_nested_str_weak(expiration),
    /* K5   */  be_nested_str_weak(reached_timeout),
    /* K6   */  be_nested_str_weak(remove),
    /* K7   */  be_const_int(1),
    }),
    be_str_weak(expire_sendqueue),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A000F,  //  0005  JMPF	R2	#0016
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0xB80E0400,  //  0008  GETNGBL	R3	K2
      0x8C0C0703,  //  0009  GETMET	R3	R3	K3
      0x88140504,  //  000A  GETMBR	R5	R2	K4
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x8C0C0505,  //  000D  GETMET	R3	R2	K5
      0x7C0C0200,  //  000E  CALL	R3	1
      0x880C0101,  //  000F  GETMBR	R3	R0	K1
      0x8C0C0706,  //  0010  GETMET	R3	R3	K6
      0x5C140200,  //  0011  MOVE	R5	R1
      0x7C0C0400,  //  0012  CALL	R3	2
      0x70020000,  //  0013  JMP		#0015
      0x00040307,  //  0014  ADD	R1	R1	K7
      0x7001FFEA,  //  0015  JMP		#0001
      0x4C080000,  //  0016  LDNIL	R2
      0x80040400,  //  0017  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_request
********************************************************************/
be_local_closure(Matter_IM_process_invoke_request,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[46]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(invoke_request_start),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(Path),
    /* K6   */  be_nested_str_weak(msg),
    /* K7   */  be_nested_str_weak(InvokeRequestMessage),
    /* K8   */  be_nested_str_weak(from_TLV),
    /* K9   */  be_nested_str_weak(invoke_requests),
    /* K10  */  be_nested_str_weak(InvokeResponseMessage),
    /* K11  */  be_nested_str_weak(suppress_response),
    /* K12  */  be_nested_str_weak(invoke_responses),
    /* K13  */  be_nested_str_weak(endpoint),
    /* K14  */  be_nested_str_weak(command_path),
    /* K15  */  be_nested_str_weak(cluster),
    /* K16  */  be_nested_str_weak(command),
    /* K17  */  be_nested_str_weak(status),
    /* K18  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K19  */  be_nested_str_weak(get_command_name),
    /* K20  */  be_nested_str_weak(invoke_request),
    /* K21  */  be_nested_str_weak(session),
    /* K22  */  be_nested_str_weak(command_fields),
    /* K23  */  be_nested_str_weak(_X28),
    /* K24  */  be_nested_str_weak(_X29_X20),
    /* K25  */  be_nested_str_weak(),
    /* K26  */  be_nested_str_weak(tasmota),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
    /* K28  */  be_nested_str_weak(local_session_id),
    /* K29  */  be_const_int(0),
    /* K30  */  be_const_int(2),
    /* K31  */  be_const_int(3),
    /* K32  */  be_nested_str_weak(InvokeResponseIB),
    /* K33  */  be_nested_str_weak(SUCCESS),
    /* K34  */  be_nested_str_weak(CommandStatusIB),
    /* K35  */  be_nested_str_weak(CommandPathIB),
    /* K36  */  be_nested_str_weak(StatusIB),
    /* K37  */  be_nested_str_weak(push),
    /* K38  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20OK_X20exch_X3D_X25i),
    /* K39  */  be_nested_str_weak(exchange_id),
    /* K40  */  be_nested_str_weak(CommandDataIB),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20Status_X3D0x_X2502X_X20exch_X3D_X25i),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20_Ignore_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i),
    /* K44  */  be_nested_str_weak(stop_iteration),
    /* K45  */  be_nested_str_weak(send_invoke_response),
    }),
    be_str_weak(process_invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[281]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0xB80E0800,  //  0005  GETNGBL	R3	K4
      0x8C0C0705,  //  0006  GETMET	R3	R3	K5
      0x7C0C0200,  //  0007  CALL	R3	1
      0x900E0C01,  //  0008  SETMBR	R3	K6	R1
      0xB8120800,  //  0009  GETNGBL	R4	K4
      0x8C100907,  //  000A  GETMET	R4	R4	K7
      0x7C100200,  //  000B  CALL	R4	1
      0x8C100908,  //  000C  GETMET	R4	R4	K8
      0x5C180400,  //  000D  MOVE	R6	R2
      0x7C100400,  //  000E  CALL	R4	2
      0x88140909,  //  000F  GETMBR	R5	R4	K9
      0x4C180000,  //  0010  LDNIL	R6
      0x20140A06,  //  0011  NE	R5	R5	R6
      0x78160104,  //  0012  JMPF	R5	#0118
      0xB8160800,  //  0013  GETNGBL	R5	K4
      0x8C140B0A,  //  0014  GETMET	R5	R5	K10
      0x7C140200,  //  0015  CALL	R5	1
      0x50180000,  //  0016  LDBOOL	R6	0	0
      0x90161606,  //  0017  SETMBR	R5	K11	R6
      0x60180012,  //  0018  GETGBL	R6	G18
      0x7C180000,  //  0019  CALL	R6	0
      0x90161806,  //  001A  SETMBR	R5	K12	R6
      0x60180010,  //  001B  GETGBL	R6	G16
      0x881C0909,  //  001C  GETMBR	R7	R4	K9
      0x7C180200,  //  001D  CALL	R6	1
      0xA80200E7,  //  001E  EXBLK	0	#0107
      0x5C1C0C00,  //  001F  MOVE	R7	R6
      0x7C1C0000,  //  0020  CALL	R7	0
      0x88200F0E,  //  0021  GETMBR	R8	R7	K14
      0x8820110D,  //  0022  GETMBR	R8	R8	K13
      0x900E1A08,  //  0023  SETMBR	R3	K13	R8
      0x88200F0E,  //  0024  GETMBR	R8	R7	K14
      0x8820110F,  //  0025  GETMBR	R8	R8	K15
      0x900E1E08,  //  0026  SETMBR	R3	K15	R8
      0x88200F0E,  //  0027  GETMBR	R8	R7	K14
      0x88201110,  //  0028  GETMBR	R8	R8	K16
      0x900E2008,  //  0029  SETMBR	R3	K16	R8
      0xB8220800,  //  002A  GETNGBL	R8	K4
      0x88201112,  //  002B  GETMBR	R8	R8	K18
      0x900E2208,  //  002C  SETMBR	R3	K17	R8
      0xB8220800,  //  002D  GETNGBL	R8	K4
      0x8C201113,  //  002E  GETMET	R8	R8	K19
      0x8828070F,  //  002F  GETMBR	R10	R3	K15
      0x882C0710,  //  0030  GETMBR	R11	R3	K16
      0x7C200600,  //  0031  CALL	R8	3
      0x60240008,  //  0032  GETGBL	R9	G8
      0x5C280600,  //  0033  MOVE	R10	R3
      0x7C240200,  //  0034  CALL	R9	1
      0x88280100,  //  0035  GETMBR	R10	R0	K0
      0x8C281514,  //  0036  GETMET	R10	R10	K20
      0x88300315,  //  0037  GETMBR	R12	R1	K21
      0x88340F16,  //  0038  GETMBR	R13	R7	K22
      0x5C380600,  //  0039  MOVE	R14	R3
      0x7C280800,  //  003A  CALL	R10	4
      0x882C0702,  //  003B  GETMBR	R11	R3	K2
      0x4C300000,  //  003C  LDNIL	R12
      0x202C160C,  //  003D  NE	R11	R11	R12
      0x782E0005,  //  003E  JMPF	R11	#0045
      0x602C0008,  //  003F  GETGBL	R11	G8
      0x88300702,  //  0040  GETMBR	R12	R3	K2
      0x7C2C0200,  //  0041  CALL	R11	1
      0x002E2E0B,  //  0042  ADD	R11	K23	R11
      0x002C1718,  //  0043  ADD	R11	R11	K24
      0x70020000,  //  0044  JMP		#0046
      0x582C0019,  //  0045  LDCONST	R11	K25
      0xB8323400,  //  0046  GETNGBL	R12	K26
      0x8C301902,  //  0047  GETMET	R12	R12	K2
      0x60380018,  //  0048  GETGBL	R14	G24
      0x583C001B,  //  0049  LDCONST	R15	K27
      0x88400315,  //  004A  GETMBR	R16	R1	K21
      0x8840211C,  //  004B  GETMBR	R16	R16	K28
      0x5C441200,  //  004C  MOVE	R17	R9
      0x78220001,  //  004D  JMPF	R8	#0050
      0x5C481000,  //  004E  MOVE	R18	R8
      0x70020000,  //  004F  JMP		#0051
      0x58480019,  //  0050  LDCONST	R18	K25
      0x5C4C1600,  //  0051  MOVE	R19	R11
      0x7C380A00,  //  0052  CALL	R14	5
      0x883C070D,  //  0053  GETMBR	R15	R3	K13
      0x203C1F1D,  //  0054  NE	R15	R15	K29
      0x783E0001,  //  0055  JMPF	R15	#0058
      0x583C001E,  //  0056  LDCONST	R15	K30
      0x70020000,  //  0057  JMP		#0059
      0x583C001F,  //  0058  LDCONST	R15	K31
      0x7C300600,  //  0059  CALL	R12	3
      0x4C300000,  //  005A  LDNIL	R12
      0x900E040C,  //  005B  SETMBR	R3	K2	R12
      0xB8320800,  //  005C  GETNGBL	R12	K4
      0x8C301920,  //  005D  GETMET	R12	R12	K32
      0x7C300200,  //  005E  CALL	R12	1
      0x50340200,  //  005F  LDBOOL	R13	1	0
      0x1C34140D,  //  0060  EQ	R13	R10	R13
      0x74360004,  //  0061  JMPT	R13	#0067
      0x88340711,  //  0062  GETMBR	R13	R3	K17
      0xB83A0800,  //  0063  GETNGBL	R14	K4
      0x88381D21,  //  0064  GETMBR	R14	R14	K33
      0x1C341A0E,  //  0065  EQ	R13	R13	R14
      0x7836002D,  //  0066  JMPF	R13	#0095
      0xB8360800,  //  0067  GETNGBL	R13	K4
      0x8C341B22,  //  0068  GETMET	R13	R13	K34
      0x7C340200,  //  0069  CALL	R13	1
      0x9032220D,  //  006A  SETMBR	R12	K17	R13
      0x88341911,  //  006B  GETMBR	R13	R12	K17
      0xB83A0800,  //  006C  GETNGBL	R14	K4
      0x8C381D23,  //  006D  GETMET	R14	R14	K35
      0x7C380200,  //  006E  CALL	R14	1
      0x90361C0E,  //  006F  SETMBR	R13	K14	R14
      0x88341911,  //  0070  GETMBR	R13	R12	K17
      0x88341B0E,  //  0071  GETMBR	R13	R13	K14
      0x8838070D,  //  0072  GETMBR	R14	R3	K13
      0x90361A0E,  //  0073  SETMBR	R13	K13	R14
      0x88341911,  //  0074  GETMBR	R13	R12	K17
      0x88341B0E,  //  0075  GETMBR	R13	R13	K14
      0x8838070F,  //  0076  GETMBR	R14	R3	K15
      0x90361E0E,  //  0077  SETMBR	R13	K15	R14
      0x88341911,  //  0078  GETMBR	R13	R12	K17
      0x88341B0E,  //  0079  GETMBR	R13	R13	K14
      0x88380710,  //  007A  GETMBR	R14	R3	K16
      0x9036200E,  //  007B  SETMBR	R13	K16	R14
      0x88341911,  //  007C  GETMBR	R13	R12	K17
      0xB83A0800,  //  007D  GETNGBL	R14	K4
      0x8C381D24,  //  007E  GETMET	R14	R14	K36
      0x7C380200,  //  007F  CALL	R14	1
      0x9036220E,  //  0080  SETMBR	R13	K17	R14
      0x88341911,  //  0081  GETMBR	R13	R12	K17
      0x88341B11,  //  0082  GETMBR	R13	R13	K17
      0xB83A0800,  //  0083  GETNGBL	R14	K4
      0x88381D21,  //  0084  GETMBR	R14	R14	K33
      0x9036220E,  //  0085  SETMBR	R13	K17	R14
      0x88340B0C,  //  0086  GETMBR	R13	R5	K12
      0x8C341B25,  //  0087  GETMET	R13	R13	K37
      0x5C3C1800,  //  0088  MOVE	R15	R12
      0x7C340400,  //  0089  CALL	R13	2
      0xB8363400,  //  008A  GETNGBL	R13	K26
      0x8C341B02,  //  008B  GETMET	R13	R13	K2
      0x603C0018,  //  008C  GETGBL	R15	G24
      0x58400026,  //  008D  LDCONST	R16	K38
      0x88440315,  //  008E  GETMBR	R17	R1	K21
      0x8844231C,  //  008F  GETMBR	R17	R17	K28
      0x88480327,  //  0090  GETMBR	R18	R1	K39
      0x7C3C0600,  //  0091  CALL	R15	3
      0x5840001F,  //  0092  LDCONST	R16	K31
      0x7C340600,  //  0093  CALL	R13	3
      0x70020070,  //  0094  JMP		#0106
      0x4C340000,  //  0095  LDNIL	R13
      0x2034140D,  //  0096  NE	R13	R10	R13
      0x78360031,  //  0097  JMPF	R13	#00CA
      0xB8360800,  //  0098  GETNGBL	R13	K4
      0x8C341B28,  //  0099  GETMET	R13	R13	K40
      0x7C340200,  //  009A  CALL	R13	1
      0x9032200D,  //  009B  SETMBR	R12	K16	R13
      0x88341910,  //  009C  GETMBR	R13	R12	K16
      0xB83A0800,  //  009D  GETNGBL	R14	K4
      0x8C381D23,  //  009E  GETMET	R14	R14	K35
      0x7C380200,  //  009F  CALL	R14	1
      0x90361C0E,  //  00A0  SETMBR	R13	K14	R14
      0x88341910,  //  00A1  GETMBR	R13	R12	K16
      0x88341B0E,  //  00A2  GETMBR	R13	R13	K14
      0x8838070D,  //  00A3  GETMBR	R14	R3	K13
      0x90361A0E,  //  00A4  SETMBR	R13	K13	R14
      0x88341910,  //  00A5  GETMBR	R13	R12	K16
      0x88341B0E,  //  00A6  GETMBR	R13	R13	K14
      0x8838070F,  //  00A7  GETMBR	R14	R3	K15
      0x90361E0E,  //  00A8  SETMBR	R13	K15	R14
      0x88341910,  //  00A9  GETMBR	R13	R12	K16
      0x88341B0E,  //  00AA  GETMBR	R13	R13	K14
      0x88380710,  //  00AB  GETMBR	R14	R3	K16
      0x9036200E,  //  00AC  SETMBR	R13	K16	R14
      0x88341910,  //  00AD  GETMBR	R13	R12	K16
      0x90362C0A,  //  00AE  SETMBR	R13	K22	R10
      0x88340B0C,  //  00AF  GETMBR	R13	R5	K12
      0x8C341B25,  //  00B0  GETMET	R13	R13	K37
      0x5C3C1800,  //  00B1  MOVE	R15	R12
      0x7C340400,  //  00B2  CALL	R13	2
      0xB8360800,  //  00B3  GETNGBL	R13	K4
      0x8C341B13,  //  00B4  GETMET	R13	R13	K19
      0x883C070F,  //  00B5  GETMBR	R15	R3	K15
      0x88400710,  //  00B6  GETMBR	R16	R3	K16
      0x7C340600,  //  00B7  CALL	R13	3
      0x5C201A00,  //  00B8  MOVE	R8	R13
      0xB8363400,  //  00B9  GETNGBL	R13	K26
      0x8C341B02,  //  00BA  GETMET	R13	R13	K2
      0x603C0018,  //  00BB  GETGBL	R15	G24
      0x58400029,  //  00BC  LDCONST	R16	K41
      0x88440315,  //  00BD  GETMBR	R17	R1	K21
      0x8844231C,  //  00BE  GETMBR	R17	R17	K28
      0x60480008,  //  00BF  GETGBL	R18	G8
      0x5C4C0600,  //  00C0  MOVE	R19	R3
      0x7C480200,  //  00C1  CALL	R18	1
      0x78220001,  //  00C2  JMPF	R8	#00C5
      0x5C4C1000,  //  00C3  MOVE	R19	R8
      0x70020000,  //  00C4  JMP		#00C6
      0x584C0019,  //  00C5  LDCONST	R19	K25
      0x7C3C0800,  //  00C6  CALL	R15	4
      0x5840001F,  //  00C7  LDCONST	R16	K31
      0x7C340600,  //  00C8  CALL	R13	3
      0x7002003B,  //  00C9  JMP		#0106
      0x88340711,  //  00CA  GETMBR	R13	R3	K17
      0x4C380000,  //  00CB  LDNIL	R14
      0x20341A0E,  //  00CC  NE	R13	R13	R14
      0x7836002D,  //  00CD  JMPF	R13	#00FC
      0xB8360800,  //  00CE  GETNGBL	R13	K4
      0x8C341B22,  //  00CF  GETMET	R13	R13	K34
      0x7C340200,  //  00D0  CALL	R13	1
      0x9032220D,  //  00D1  SETMBR	R12	K17	R13
      0x88341911,  //  00D2  GETMBR	R13	R12	K17
      0xB83A0800,  //  00D3  GETNGBL	R14	K4
      0x8C381D23,  //  00D4  GETMET	R14	R14	K35
      0x7C380200,  //  00D5  CALL	R14	1
      0x90361C0E,  //  00D6  SETMBR	R13	K14	R14
      0x88341911,  //  00D7  GETMBR	R13	R12	K17
      0x88341B0E,  //  00D8  GETMBR	R13	R13	K14
      0x8838070D,  //  00D9  GETMBR	R14	R3	K13
      0x90361A0E,  //  00DA  SETMBR	R13	K13	R14
      0x88341911,  //  00DB  GETMBR	R13	R12	K17
      0x88341B0E,  //  00DC  GETMBR	R13	R13	K14
      0x8838070F,  //  00DD  GETMBR	R14	R3	K15
      0x90361E0E,  //  00DE  SETMBR	R13	K15	R14
      0x88341911,  //  00DF  GETMBR	R13	R12	K17
      0x88341B0E,  //  00E0  GETMBR	R13	R13	K14
      0x88380710,  //  00E1  GETMBR	R14	R3	K16
      0x9036200E,  //  00E2  SETMBR	R13	K16	R14
      0x88341911,  //  00E3  GETMBR	R13	R12	K17
      0xB83A0800,  //  00E4  GETNGBL	R14	K4
      0x8C381D24,  //  00E5  GETMET	R14	R14	K36
      0x7C380200,  //  00E6  CALL	R14	1
      0x9036220E,  //  00E7  SETMBR	R13	K17	R14
      0x88341911,  //  00E8  GETMBR	R13	R12	K17
      0x88341B11,  //  00E9  GETMBR	R13	R13	K17
      0x88380711,  //  00EA  GETMBR	R14	R3	K17
      0x9036220E,  //  00EB  SETMBR	R13	K17	R14
      0x88340B0C,  //  00EC  GETMBR	R13	R5	K12
      0x8C341B25,  //  00ED  GETMET	R13	R13	K37
      0x5C3C1800,  //  00EE  MOVE	R15	R12
      0x7C340400,  //  00EF  CALL	R13	2
      0xB8363400,  //  00F0  GETNGBL	R13	K26
      0x8C341B02,  //  00F1  GETMET	R13	R13	K2
      0x603C0018,  //  00F2  GETGBL	R15	G24
      0x5840002A,  //  00F3  LDCONST	R16	K42
      0x88440315,  //  00F4  GETMBR	R17	R1	K21
      0x8844231C,  //  00F5  GETMBR	R17	R17	K28
      0x88480711,  //  00F6  GETMBR	R18	R3	K17
      0x884C0327,  //  00F7  GETMBR	R19	R1	K39
      0x7C3C0800,  //  00F8  CALL	R15	4
      0x5840001F,  //  00F9  LDCONST	R16	K31
      0x7C340600,  //  00FA  CALL	R13	3
      0x70020009,  //  00FB  JMP		#0106
      0xB8363400,  //  00FC  GETNGBL	R13	K26
      0x8C341B02,  //  00FD  GETMET	R13	R13	K2
      0x603C0018,  //  00FE  GETGBL	R15	G24
      0x5840002B,  //  00FF  LDCONST	R16	K43
      0x88440315,  //  0100  GETMBR	R17	R1	K21
      0x8844231C,  //  0101  GETMBR	R17	R17	K28
      0x88480327,  //  0102  GETMBR	R18	R1	K39
      0x7C3C0600,  //  0103  CALL	R15	3
      0x5840001F,  //  0104  LDCONST	R16	K31
      0x7C340600,  //  0105  CALL	R13	3
      0x7001FF17,  //  0106  JMP		#001F
      0x5818002C,  //  0107  LDCONST	R6	K44
      0xAC180200,  //  0108  CATCH	R6	1	0
      0xB0080000,  //  0109  RAISE	2	R0	R0
      0x6018000C,  //  010A  GETGBL	R6	G12
      0x881C0B0C,  //  010B  GETMBR	R7	R5	K12
      0x7C180200,  //  010C  CALL	R6	1
      0x24180D1D,  //  010D  GT	R6	R6	K29
      0x781A0004,  //  010E  JMPF	R6	#0114
      0x8C18012D,  //  010F  GETMET	R6	R0	K45
      0x5C200200,  //  0110  MOVE	R8	R1
      0x5C240A00,  //  0111  MOVE	R9	R5
      0x7C180600,  //  0112  CALL	R6	3
      0x70020001,  //  0113  JMP		#0116
      0x50180000,  //  0114  LDBOOL	R6	0	0
      0x80040C00,  //  0115  RET	1	R6
      0x50180200,  //  0116  LDBOOL	R6	1	0
      0x80040C00,  //  0117  RET	1	R6
      0x80000000,  //  0118  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_request
********************************************************************/
be_local_closure(Matter_IM_subscribe_request,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(SubscribeRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(keep_subscriptions),
    /* K4   */  be_nested_str_weak(subs_shop),
    /* K5   */  be_nested_str_weak(remove_by_session),
    /* K6   */  be_nested_str_weak(session),
    /* K7   */  be_nested_str_weak(new_subscription),
    /* K8   */  be_nested_str_weak(Path),
    /* K9   */  be_nested_str_weak(msg),
    /* K10  */  be_nested_str_weak(attributes_requests),
    /* K11  */  be_nested_str_weak(endpoint),
    /* K12  */  be_nested_str_weak(cluster),
    /* K13  */  be_nested_str_weak(attribute),
    /* K14  */  be_nested_str_weak(push),
    /* K15  */  be_nested_str_weak(stop_iteration),
    /* K16  */  be_nested_str_weak(tasmota),
    /* K17  */  be_nested_str_weak(log),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20_X3ESubscribe_X20_X28_X256i_X29_X20_X25s_X20_X28min_X3D_X25i_X2C_X20max_X3D_X25i_X2C_X20keep_X3D_X25i_X29_X20sub_X3D_X25i),
    /* K19  */  be_nested_str_weak(local_session_id),
    /* K20  */  be_nested_str_weak(concat),
    /* K21  */  be_nested_str_weak(_X20),
    /* K22  */  be_nested_str_weak(min_interval),
    /* K23  */  be_nested_str_weak(max_interval),
    /* K24  */  be_const_int(1),
    /* K25  */  be_const_int(0),
    /* K26  */  be_nested_str_weak(subscription_id),
    /* K27  */  be_const_int(3),
    /* K28  */  be_nested_str_weak(_inner_process_read_request),
    /* K29  */  be_nested_str_weak(send_subscribe_response),
    }),
    be_str_weak(subscribe_request),
    &be_const_str_solidified,
    ( &(const binstruction[78]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x88100703,  //  0006  GETMBR	R4	R3	K3
      0x74120003,  //  0007  JMPT	R4	#000C
      0x88100104,  //  0008  GETMBR	R4	R0	K4
      0x8C100905,  //  0009  GETMET	R4	R4	K5
      0x88180306,  //  000A  GETMBR	R6	R1	K6
      0x7C100400,  //  000B  CALL	R4	2
      0x88100104,  //  000C  GETMBR	R4	R0	K4
      0x8C100907,  //  000D  GETMET	R4	R4	K7
      0x88180306,  //  000E  GETMBR	R6	R1	K6
      0x5C1C0600,  //  000F  MOVE	R7	R3
      0x7C100600,  //  0010  CALL	R4	3
      0x60140012,  //  0011  GETGBL	R5	G18
      0x7C140000,  //  0012  CALL	R5	0
      0xB81A0000,  //  0013  GETNGBL	R6	K0
      0x8C180D08,  //  0014  GETMET	R6	R6	K8
      0x7C180200,  //  0015  CALL	R6	1
      0x901A1201,  //  0016  SETMBR	R6	K9	R1
      0x601C0010,  //  0017  GETGBL	R7	G16
      0x8820070A,  //  0018  GETMBR	R8	R3	K10
      0x7C1C0200,  //  0019  CALL	R7	1
      0xA802000D,  //  001A  EXBLK	0	#0029
      0x5C200E00,  //  001B  MOVE	R8	R7
      0x7C200000,  //  001C  CALL	R8	0
      0x8824110B,  //  001D  GETMBR	R9	R8	K11
      0x901A1609,  //  001E  SETMBR	R6	K11	R9
      0x8824110C,  //  001F  GETMBR	R9	R8	K12
      0x901A1809,  //  0020  SETMBR	R6	K12	R9
      0x8824110D,  //  0021  GETMBR	R9	R8	K13
      0x901A1A09,  //  0022  SETMBR	R6	K13	R9
      0x8C240B0E,  //  0023  GETMET	R9	R5	K14
      0x602C0008,  //  0024  GETGBL	R11	G8
      0x5C300C00,  //  0025  MOVE	R12	R6
      0x7C2C0200,  //  0026  CALL	R11	1
      0x7C240400,  //  0027  CALL	R9	2
      0x7001FFF1,  //  0028  JMP		#001B
      0x581C000F,  //  0029  LDCONST	R7	K15
      0xAC1C0200,  //  002A  CATCH	R7	1	0
      0xB0080000,  //  002B  RAISE	2	R0	R0
      0xB81E2000,  //  002C  GETNGBL	R7	K16
      0x8C1C0F11,  //  002D  GETMET	R7	R7	K17
      0x60240018,  //  002E  GETGBL	R9	G24
      0x58280012,  //  002F  LDCONST	R10	K18
      0x882C0306,  //  0030  GETMBR	R11	R1	K6
      0x882C1713,  //  0031  GETMBR	R11	R11	K19
      0x8C300B14,  //  0032  GETMET	R12	R5	K20
      0x58380015,  //  0033  LDCONST	R14	K21
      0x7C300400,  //  0034  CALL	R12	2
      0x88340916,  //  0035  GETMBR	R13	R4	K22
      0x88380917,  //  0036  GETMBR	R14	R4	K23
      0x883C0703,  //  0037  GETMBR	R15	R3	K3
      0x783E0001,  //  0038  JMPF	R15	#003B
      0x583C0018,  //  0039  LDCONST	R15	K24
      0x70020000,  //  003A  JMP		#003C
      0x583C0019,  //  003B  LDCONST	R15	K25
      0x8840091A,  //  003C  GETMBR	R16	R4	K26
      0x7C240E00,  //  003D  CALL	R9	7
      0x5828001B,  //  003E  LDCONST	R10	K27
      0x7C1C0600,  //  003F  CALL	R7	3
      0x8C1C011C,  //  0040  GETMET	R7	R0	K28
      0x88240306,  //  0041  GETMBR	R9	R1	K6
      0x5C280600,  //  0042  MOVE	R10	R3
      0x502C0200,  //  0043  LDBOOL	R11	1	0
      0x7C1C0800,  //  0044  CALL	R7	4
      0x8820091A,  //  0045  GETMBR	R8	R4	K26
      0x901E3408,  //  0046  SETMBR	R7	K26	R8
      0x8C20011D,  //  0047  GETMET	R8	R0	K29
      0x5C280200,  //  0048  MOVE	R10	R1
      0x5C2C0E00,  //  0049  MOVE	R11	R7
      0x5C300800,  //  004A  MOVE	R12	R4
      0x7C200800,  //  004B  CALL	R8	4
      0x50200200,  //  004C  LDBOOL	R8	1	0
      0x80041000,  //  004D  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_request
********************************************************************/
be_local_closure(Matter_IM_process_write_request,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        17,                          /* nstack */
        5,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[26]) {     /* constants */
        /* K0   */  be_nested_str_weak(matter),
        /* K1   */  be_nested_str_weak(get_attribute_name),
        /* K2   */  be_nested_str_weak(cluster),
        /* K3   */  be_nested_str_weak(attribute),
        /* K4   */  be_nested_str_weak(_X20_X28),
        /* K5   */  be_nested_str_weak(_X29),
        /* K6   */  be_nested_str_weak(),
        /* K7   */  be_nested_str_weak(status),
        /* K8   */  be_nested_str_weak(UNSUPPORTED_WRITE),
        /* K9   */  be_nested_str_weak(write_attribute),
        /* K10  */  be_nested_str_weak(session),
        /* K11  */  be_nested_str_weak(SUCCESS),
        /* K12  */  be_nested_str_weak(AttributeStatusIB),
        /* K13  */  be_nested_str_weak(path),
        /* K14  */  be_nested_str_weak(AttributePathIB),
        /* K15  */  be_nested_str_weak(StatusIB),
        /* K16  */  be_nested_str_weak(endpoint),
        /* K17  */  be_nested_str_weak(write_responses),
        /* K18  */  be_nested_str_weak(push),
        /* K19  */  be_nested_str_weak(tasmota),
        /* K20  */  be_nested_str_weak(log),
        /* K21  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K22  */  be_const_int(0),
        /* K23  */  be_const_int(2),
        /* K24  */  be_const_int(3),
        /* K25  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20IGNORED),
        }),
        be_str_weak(write_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[101]) {  /* code */
          0xB8160000,  //  0000  GETNGBL	R5	K0
          0x8C140B01,  //  0001  GETMET	R5	R5	K1
          0x881C0502,  //  0002  GETMBR	R7	R2	K2
          0x88200503,  //  0003  GETMBR	R8	R2	K3
          0x7C140600,  //  0004  CALL	R5	3
          0x78160002,  //  0005  JMPF	R5	#0009
          0x001A0805,  //  0006  ADD	R6	K4	R5
          0x00180D05,  //  0007  ADD	R6	R6	K5
          0x70020000,  //  0008  JMP		#000A
          0x58180006,  //  0009  LDCONST	R6	K6
          0x5C140C00,  //  000A  MOVE	R5	R6
          0xB81A0000,  //  000B  GETNGBL	R6	K0
          0x88180D08,  //  000C  GETMBR	R6	R6	K8
          0x900A0E06,  //  000D  SETMBR	R2	K7	R6
          0x4C180000,  //  000E  LDNIL	R6
          0x20180206,  //  000F  NE	R6	R1	R6
          0x781A0006,  //  0010  JMPF	R6	#0018
          0x8C180309,  //  0011  GETMET	R6	R1	K9
          0x68200000,  //  0012  GETUPV	R8	U0
          0x8820110A,  //  0013  GETMBR	R8	R8	K10
          0x5C240400,  //  0014  MOVE	R9	R2
          0x5C280600,  //  0015  MOVE	R10	R3
          0x7C180800,  //  0016  CALL	R6	4
          0x70020000,  //  0017  JMP		#0019
          0x4C180000,  //  0018  LDNIL	R6
          0x781A0002,  //  0019  JMPF	R6	#001D
          0xB81E0000,  //  001A  GETNGBL	R7	K0
          0x881C0F0B,  //  001B  GETMBR	R7	R7	K11
          0x900A0E07,  //  001C  SETMBR	R2	K7	R7
          0x881C0507,  //  001D  GETMBR	R7	R2	K7
          0x4C200000,  //  001E  LDNIL	R8
          0x201C0E08,  //  001F  NE	R7	R7	R8
          0x781E0037,  //  0020  JMPF	R7	#0059
          0x78120035,  //  0021  JMPF	R4	#0058
          0xB81E0000,  //  0022  GETNGBL	R7	K0
          0x8C1C0F0C,  //  0023  GETMET	R7	R7	K12
          0x7C1C0200,  //  0024  CALL	R7	1
          0xB8220000,  //  0025  GETNGBL	R8	K0
          0x8C20110E,  //  0026  GETMET	R8	R8	K14
          0x7C200200,  //  0027  CALL	R8	1
          0x901E1A08,  //  0028  SETMBR	R7	K13	R8
          0xB8220000,  //  0029  GETNGBL	R8	K0
          0x8C20110F,  //  002A  GETMET	R8	R8	K15
          0x7C200200,  //  002B  CALL	R8	1
          0x901E0E08,  //  002C  SETMBR	R7	K7	R8
          0x88200F0D,  //  002D  GETMBR	R8	R7	K13
          0x88240510,  //  002E  GETMBR	R9	R2	K16
          0x90222009,  //  002F  SETMBR	R8	K16	R9
          0x88200F0D,  //  0030  GETMBR	R8	R7	K13
          0x88240502,  //  0031  GETMBR	R9	R2	K2
          0x90220409,  //  0032  SETMBR	R8	K2	R9
          0x88200F0D,  //  0033  GETMBR	R8	R7	K13
          0x88240503,  //  0034  GETMBR	R9	R2	K3
          0x90220609,  //  0035  SETMBR	R8	K3	R9
          0x88200F07,  //  0036  GETMBR	R8	R7	K7
          0x88240507,  //  0037  GETMBR	R9	R2	K7
          0x90220E09,  //  0038  SETMBR	R8	K7	R9
          0x88200111,  //  0039  GETMBR	R8	R0	K17
          0x8C201112,  //  003A  GETMET	R8	R8	K18
          0x5C280E00,  //  003B  MOVE	R10	R7
          0x7C200400,  //  003C  CALL	R8	2
          0xB8222600,  //  003D  GETNGBL	R8	K19
          0x8C201114,  //  003E  GETMET	R8	R8	K20
          0x60280018,  //  003F  GETGBL	R10	G24
          0x582C0015,  //  0040  LDCONST	R11	K21
          0x60300008,  //  0041  GETGBL	R12	G8
          0x5C340400,  //  0042  MOVE	R13	R2
          0x7C300200,  //  0043  CALL	R12	1
          0x5C340A00,  //  0044  MOVE	R13	R5
          0x88380507,  //  0045  GETMBR	R14	R2	K7
          0x883C0507,  //  0046  GETMBR	R15	R2	K7
          0xB8420000,  //  0047  GETNGBL	R16	K0
          0x8840210B,  //  0048  GETMBR	R16	R16	K11
          0x1C3C1E10,  //  0049  EQ	R15	R15	R16
          0x783E0001,  //  004A  JMPF	R15	#004D
          0x583C000B,  //  004B  LDCONST	R15	K11
          0x70020000,  //  004C  JMP		#004E
          0x583C0006,  //  004D  LDCONST	R15	K6
          0x7C280A00,  //  004E  CALL	R10	5
          0x882C0510,  //  004F  GETMBR	R11	R2	K16
          0x202C1716,  //  0050  NE	R11	R11	K22
          0x782E0001,  //  0051  JMPF	R11	#0054
          0x582C0017,  //  0052  LDCONST	R11	K23
          0x70020000,  //  0053  JMP		#0055
          0x582C0018,  //  0054  LDCONST	R11	K24
          0x7C200600,  //  0055  CALL	R8	3
          0x50200200,  //  0056  LDBOOL	R8	1	0
          0x80041000,  //  0057  RET	1	R8
          0x7002000A,  //  0058  JMP		#0064
          0xB81E2600,  //  0059  GETNGBL	R7	K19
          0x8C1C0F14,  //  005A  GETMET	R7	R7	K20
          0x60240018,  //  005B  GETGBL	R9	G24
          0x58280019,  //  005C  LDCONST	R10	K25
          0x602C0008,  //  005D  GETGBL	R11	G8
          0x5C300400,  //  005E  MOVE	R12	R2
          0x7C2C0200,  //  005F  CALL	R11	1
          0x5C300A00,  //  0060  MOVE	R12	R5
          0x7C240600,  //  0061  CALL	R9	3
          0x58280018,  //  0062  LDCONST	R10	K24
          0x7C1C0600,  //  0063  CALL	R7	3
          0x80000000,  //  0064  RET	0
        })
      ),
      be_nested_proto(
        9,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 6),
          be_local_const_upval(1, 8),
          be_local_const_upval(1, 12),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x68100001,  //  0001  GETUPV	R4	U1
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x681C0002,  //  0004  GETUPV	R7	U2
          0x5C200400,  //  0005  MOVE	R8	R2
          0x7C0C0A00,  //  0006  CALL	R3	5
          0x80040600,  //  0007  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(WriteRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(suppress_response),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(get_active_endpoints),
    /* K6   */  be_nested_str_weak(Path),
    /* K7   */  be_nested_str_weak(write_requests),
    /* K8   */  be_nested_str_weak(WriteResponseMessage),
    /* K9   */  be_nested_str_weak(write_responses),
    /* K10  */  be_nested_str_weak(path),
    /* K11  */  be_nested_str_weak(data),
    /* K12  */  be_nested_str_weak(endpoint),
    /* K13  */  be_nested_str_weak(cluster),
    /* K14  */  be_nested_str_weak(attribute),
    /* K15  */  be_nested_str_weak(status),
    /* K16  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K17  */  be_nested_str_weak(INVALID_ACTION),
    /* K18  */  be_nested_str_weak(get_attribute_name),
    /* K19  */  be_nested_str_weak(tasmota),
    /* K20  */  be_nested_str_weak(log),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20),
    /* K22  */  be_nested_str_weak(_X20_X28),
    /* K23  */  be_nested_str_weak(_X29),
    /* K24  */  be_nested_str_weak(),
    /* K25  */  be_const_int(3),
    /* K26  */  be_nested_str_weak(process_attribute_expansion),
    /* K27  */  be_nested_str_weak(stop_iteration),
    /* K28  */  be_nested_str_weak(send_write_response),
    }),
    be_str_weak(process_write_request),
    &be_const_str_solidified,
    ( &(const binstruction[103]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x88100703,  //  0006  GETMBR	R4	R3	K3
      0x88140104,  //  0007  GETMBR	R5	R0	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x7C140200,  //  0009  CALL	R5	1
      0x84180000,  //  000A  CLOSURE	R6	P0
      0xB81E0000,  //  000B  GETNGBL	R7	K0
      0x8C1C0F06,  //  000C  GETMET	R7	R7	K6
      0x7C1C0200,  //  000D  CALL	R7	1
      0x88200707,  //  000E  GETMBR	R8	R3	K7
      0x4C240000,  //  000F  LDNIL	R9
      0x20201009,  //  0010  NE	R8	R8	R9
      0x78220051,  //  0011  JMPF	R8	#0064
      0xB8220000,  //  0012  GETNGBL	R8	K0
      0x8C201108,  //  0013  GETMET	R8	R8	K8
      0x7C200200,  //  0014  CALL	R8	1
      0x60240012,  //  0015  GETGBL	R9	G18
      0x7C240000,  //  0016  CALL	R9	0
      0x90221209,  //  0017  SETMBR	R8	K9	R9
      0x60240010,  //  0018  GETGBL	R9	G16
      0x88280707,  //  0019  GETMBR	R10	R3	K7
      0x7C240200,  //  001A  CALL	R9	1
      0xA802003D,  //  001B  EXBLK	0	#005A
      0x5C281200,  //  001C  MOVE	R10	R9
      0x7C280000,  //  001D  CALL	R10	0
      0x882C150A,  //  001E  GETMBR	R11	R10	K10
      0x8830150B,  //  001F  GETMBR	R12	R10	K11
      0x8834170C,  //  0020  GETMBR	R13	R11	K12
      0x901E180D,  //  0021  SETMBR	R7	K12	R13
      0x8834170D,  //  0022  GETMBR	R13	R11	K13
      0x901E1A0D,  //  0023  SETMBR	R7	K13	R13
      0x8834170E,  //  0024  GETMBR	R13	R11	K14
      0x901E1C0D,  //  0025  SETMBR	R7	K14	R13
      0xB8360000,  //  0026  GETNGBL	R13	K0
      0x88341B10,  //  0027  GETMBR	R13	R13	K16
      0x901E1E0D,  //  0028  SETMBR	R7	K15	R13
      0x88340F0D,  //  0029  GETMBR	R13	R7	K13
      0x4C380000,  //  002A  LDNIL	R14
      0x1C341A0E,  //  002B  EQ	R13	R13	R14
      0x74360003,  //  002C  JMPT	R13	#0031
      0x88340F0E,  //  002D  GETMBR	R13	R7	K14
      0x4C380000,  //  002E  LDNIL	R14
      0x1C341A0E,  //  002F  EQ	R13	R13	R14
      0x7836000A,  //  0030  JMPF	R13	#003C
      0xB8360000,  //  0031  GETNGBL	R13	K0
      0x88341B11,  //  0032  GETMBR	R13	R13	K17
      0x901E1E0D,  //  0033  SETMBR	R7	K15	R13
      0x5C340C00,  //  0034  MOVE	R13	R6
      0x5C381000,  //  0035  MOVE	R14	R8
      0x4C3C0000,  //  0036  LDNIL	R15
      0x5C400E00,  //  0037  MOVE	R16	R7
      0x4C440000,  //  0038  LDNIL	R17
      0x50480200,  //  0039  LDBOOL	R18	1	0
      0x7C340A00,  //  003A  CALL	R13	5
      0x7001FFDF,  //  003B  JMP		#001C
      0x88340F0C,  //  003C  GETMBR	R13	R7	K12
      0x4C380000,  //  003D  LDNIL	R14
      0x1C341A0E,  //  003E  EQ	R13	R13	R14
      0x78360012,  //  003F  JMPF	R13	#0053
      0xB8360000,  //  0040  GETNGBL	R13	K0
      0x8C341B12,  //  0041  GETMET	R13	R13	K18
      0x883C0F0D,  //  0042  GETMBR	R15	R7	K13
      0x88400F0E,  //  0043  GETMBR	R16	R7	K14
      0x7C340600,  //  0044  CALL	R13	3
      0xB83A2600,  //  0045  GETNGBL	R14	K19
      0x8C381D14,  //  0046  GETMET	R14	R14	K20
      0x60400008,  //  0047  GETGBL	R16	G8
      0x5C440E00,  //  0048  MOVE	R17	R7
      0x7C400200,  //  0049  CALL	R16	1
      0x00422A10,  //  004A  ADD	R16	K21	R16
      0x78360002,  //  004B  JMPF	R13	#004F
      0x00462C0D,  //  004C  ADD	R17	K22	R13
      0x00442317,  //  004D  ADD	R17	R17	K23
      0x70020000,  //  004E  JMP		#0050
      0x58440018,  //  004F  LDCONST	R17	K24
      0x00402011,  //  0050  ADD	R16	R16	R17
      0x58440019,  //  0051  LDCONST	R17	K25
      0x7C380600,  //  0052  CALL	R14	3
      0x88340104,  //  0053  GETMBR	R13	R0	K4
      0x8C341B1A,  //  0054  GETMET	R13	R13	K26
      0x5C3C0E00,  //  0055  MOVE	R15	R7
      0x84400001,  //  0056  CLOSURE	R16	P1
      0x7C340600,  //  0057  CALL	R13	3
      0xA0240000,  //  0058  CLOSE	R9
      0x7001FFC1,  //  0059  JMP		#001C
      0x5824001B,  //  005A  LDCONST	R9	K27
      0xAC240200,  //  005B  CATCH	R9	1	0
      0xB0080000,  //  005C  RAISE	2	R0	R0
      0x5C240800,  //  005D  MOVE	R9	R4
      0x74260003,  //  005E  JMPT	R9	#0063
      0x8C24011C,  //  005F  GETMET	R9	R0	K28
      0x5C2C0200,  //  0060  MOVE	R11	R1
      0x5C301000,  //  0061  MOVE	R12	R8
      0x7C240600,  //  0062  CALL	R9	3
      0xA0200000,  //  0063  CLOSE	R8
      0x50200200,  //  0064  LDBOOL	R8	1	0
      0xA0000000,  //  0065  CLOSE	R0
      0x80041000,  //  0066  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_write_response
********************************************************************/
be_local_closure(Matter_IM_send_write_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_WriteResponse),
    }),
    be_str_weak(send_write_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_sendqueue_by_exchangeid
********************************************************************/
be_local_closure(Matter_IM_remove_sendqueue_by_exchangeid,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(get_exchangeid),
    /* K3   */  be_nested_str_weak(remove),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(remove_sendqueue_by_exchangeid),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x58080000,  //  0004  LDCONST	R2	K0
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E000C,  //  0009  JMPF	R3	#0017
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x940C0602,  //  000B  GETIDX	R3	R3	R2
      0x8C0C0702,  //  000C  GETMET	R3	R3	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x1C0C0601,  //  000E  EQ	R3	R3	R1
      0x780E0004,  //  000F  JMPF	R3	#0015
      0x880C0101,  //  0010  GETMBR	R3	R0	K1
      0x8C0C0703,  //  0011  GETMET	R3	R3	K3
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x70020000,  //  0014  JMP		#0016
      0x00080504,  //  0015  ADD	R2	R2	K4
      0x7001FFED,  //  0016  JMP		#0005
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_IM_every_second,   /* name */
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
    /* K0   */  be_nested_str_weak(expire_sendqueue),
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
** Solidified function: send_ack_now
********************************************************************/
be_local_closure(Matter_IM_send_ack_now,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(_message_handler),
    /* K2   */  be_nested_str_weak(send_encrypted_ack),
    }),
    be_str_weak(send_ack_now),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x88080300,  //  0004  GETMBR	R2	R1	K0
      0x88080501,  //  0005  GETMBR	R2	R2	K1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x5C100200,  //  0007  MOVE	R4	R1
      0x50140000,  //  0008  LDBOOL	R5	0	0
      0x7C080600,  //  0009  CALL	R2	3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(Matter_IM_every_250ms,   /* name */
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
    /* K0   */  be_nested_str_weak(subs_shop),
    /* K1   */  be_nested_str_weak(every_250ms),
    }),
    be_str_weak(every_250ms),
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
** Solidified function: send_subscribe_response
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_SubscribeResponse),
    }),
    be_str_weak(send_subscribe_response),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0xB81A0400,  //  0002  GETNGBL	R6	K2
      0x8C180D03,  //  0003  GETMET	R6	R6	K3
      0x5C200200,  //  0004  MOVE	R8	R1
      0x5C240400,  //  0005  MOVE	R9	R2
      0x5C280600,  //  0006  MOVE	R10	R3
      0x7C180800,  //  0007  CALL	R6	4
      0x7C100400,  //  0008  CALL	R4	2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_invoke_response
********************************************************************/
be_local_closure(Matter_IM_send_invoke_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_InvokeResponse),
    }),
    be_str_weak(send_invoke_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_status
********************************************************************/
be_local_closure(Matter_IM_send_status,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_Status),
    }),
    be_str_weak(send_status),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_response
********************************************************************/
be_local_closure(Matter_IM_subscribe_response,   /* name */
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
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(SubscribeResponseMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(subscribe_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming
********************************************************************/
be_local_closure(Matter_IM_process_incoming,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(parse),
    /* K3   */  be_nested_str_weak(raw),
    /* K4   */  be_nested_str_weak(app_payload_idx),
    /* K5   */  be_nested_str_weak(findsubval),
    /* K6   */  be_nested_str_weak(opcode),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(process_status_response),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(send_ack_now),
    /* K11  */  be_nested_str_weak(process_read_request),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(subscribe_request),
    /* K14  */  be_nested_str_weak(subscribe_response),
    /* K15  */  be_nested_str_weak(report_data),
    /* K16  */  be_nested_str_weak(process_write_request),
    /* K17  */  be_nested_str_weak(process_write_response),
    /* K18  */  be_nested_str_weak(process_invoke_request),
    /* K19  */  be_nested_str_weak(process_invoke_response),
    /* K20  */  be_nested_str_weak(process_timed_request),
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[107]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x88100303,  //  0003  GETMBR	R4	R1	K3
      0x88140304,  //  0004  GETMBR	R5	R1	K4
      0x7C080600,  //  0005  CALL	R2	3
      0x8C0C0505,  //  0006  GETMET	R3	R2	K5
      0x541600FE,  //  0007  LDINT	R5	255
      0x7C0C0400,  //  0008  CALL	R3	2
      0x88100306,  //  0009  GETMBR	R4	R1	K6
      0x1C140907,  //  000A  EQ	R5	R4	K7
      0x78160005,  //  000B  JMPF	R5	#0012
      0x8C140108,  //  000C  GETMET	R5	R0	K8
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x5C200400,  //  000E  MOVE	R8	R2
      0x7C140600,  //  000F  CALL	R5	3
      0x80040A00,  //  0010  RET	1	R5
      0x70020056,  //  0011  JMP		#0069
      0x1C140909,  //  0012  EQ	R5	R4	K9
      0x78160008,  //  0013  JMPF	R5	#001D
      0x8C14010A,  //  0014  GETMET	R5	R0	K10
      0x5C1C0200,  //  0015  MOVE	R7	R1
      0x7C140400,  //  0016  CALL	R5	2
      0x8C14010B,  //  0017  GETMET	R5	R0	K11
      0x5C1C0200,  //  0018  MOVE	R7	R1
      0x5C200400,  //  0019  MOVE	R8	R2
      0x7C140600,  //  001A  CALL	R5	3
      0x80040A00,  //  001B  RET	1	R5
      0x7002004B,  //  001C  JMP		#0069
      0x1C14090C,  //  001D  EQ	R5	R4	K12
      0x78160008,  //  001E  JMPF	R5	#0028
      0x8C14010A,  //  001F  GETMET	R5	R0	K10
      0x5C1C0200,  //  0020  MOVE	R7	R1
      0x7C140400,  //  0021  CALL	R5	2
      0x8C14010D,  //  0022  GETMET	R5	R0	K13
      0x5C1C0200,  //  0023  MOVE	R7	R1
      0x5C200400,  //  0024  MOVE	R8	R2
      0x7C140600,  //  0025  CALL	R5	3
      0x80040A00,  //  0026  RET	1	R5
      0x70020040,  //  0027  JMP		#0069
      0x54160003,  //  0028  LDINT	R5	4
      0x1C140805,  //  0029  EQ	R5	R4	R5
      0x78160005,  //  002A  JMPF	R5	#0031
      0x8C14010E,  //  002B  GETMET	R5	R0	K14
      0x5C1C0200,  //  002C  MOVE	R7	R1
      0x5C200400,  //  002D  MOVE	R8	R2
      0x7C140600,  //  002E  CALL	R5	3
      0x80040A00,  //  002F  RET	1	R5
      0x70020037,  //  0030  JMP		#0069
      0x54160004,  //  0031  LDINT	R5	5
      0x1C140805,  //  0032  EQ	R5	R4	R5
      0x78160005,  //  0033  JMPF	R5	#003A
      0x8C14010F,  //  0034  GETMET	R5	R0	K15
      0x5C1C0200,  //  0035  MOVE	R7	R1
      0x5C200400,  //  0036  MOVE	R8	R2
      0x7C140600,  //  0037  CALL	R5	3
      0x80040A00,  //  0038  RET	1	R5
      0x7002002E,  //  0039  JMP		#0069
      0x54160005,  //  003A  LDINT	R5	6
      0x1C140805,  //  003B  EQ	R5	R4	R5
      0x78160008,  //  003C  JMPF	R5	#0046
      0x8C14010A,  //  003D  GETMET	R5	R0	K10
      0x5C1C0200,  //  003E  MOVE	R7	R1
      0x7C140400,  //  003F  CALL	R5	2
      0x8C140110,  //  0040  GETMET	R5	R0	K16
      0x5C1C0200,  //  0041  MOVE	R7	R1
      0x5C200400,  //  0042  MOVE	R8	R2
      0x7C140600,  //  0043  CALL	R5	3
      0x80040A00,  //  0044  RET	1	R5
      0x70020022,  //  0045  JMP		#0069
      0x54160006,  //  0046  LDINT	R5	7
      0x1C140805,  //  0047  EQ	R5	R4	R5
      0x78160005,  //  0048  JMPF	R5	#004F
      0x8C140111,  //  0049  GETMET	R5	R0	K17
      0x5C1C0200,  //  004A  MOVE	R7	R1
      0x5C200400,  //  004B  MOVE	R8	R2
      0x7C140600,  //  004C  CALL	R5	3
      0x80040A00,  //  004D  RET	1	R5
      0x70020019,  //  004E  JMP		#0069
      0x54160007,  //  004F  LDINT	R5	8
      0x1C140805,  //  0050  EQ	R5	R4	R5
      0x78160005,  //  0051  JMPF	R5	#0058
      0x8C140112,  //  0052  GETMET	R5	R0	K18
      0x5C1C0200,  //  0053  MOVE	R7	R1
      0x5C200400,  //  0054  MOVE	R8	R2
      0x7C140600,  //  0055  CALL	R5	3
      0x80040A00,  //  0056  RET	1	R5
      0x70020010,  //  0057  JMP		#0069
      0x54160008,  //  0058  LDINT	R5	9
      0x1C140805,  //  0059  EQ	R5	R4	R5
      0x78160005,  //  005A  JMPF	R5	#0061
      0x8C140113,  //  005B  GETMET	R5	R0	K19
      0x5C1C0200,  //  005C  MOVE	R7	R1
      0x5C200400,  //  005D  MOVE	R8	R2
      0x7C140600,  //  005E  CALL	R5	3
      0x80040A00,  //  005F  RET	1	R5
      0x70020007,  //  0060  JMP		#0069
      0x54160009,  //  0061  LDINT	R5	10
      0x1C140805,  //  0062  EQ	R5	R4	R5
      0x78160004,  //  0063  JMPF	R5	#0069
      0x8C140114,  //  0064  GETMET	R5	R0	K20
      0x5C1C0200,  //  0065  MOVE	R7	R1
      0x5C200400,  //  0066  MOVE	R8	R2
      0x7C140600,  //  0067  CALL	R5	3
      0x80040A00,  //  0068  RET	1	R5
      0x50140000,  //  0069  LDBOOL	R5	0	0
      0x80040A00,  //  006A  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _inner_process_read_request
********************************************************************/
be_local_closure(Matter_IM__inner_process_read_request,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        23,                          /* nstack */
        4,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 1),
          be_local_const_upval(1, 3),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[38]) {     /* constants */
        /* K0   */  be_nested_str_weak(matter),
        /* K1   */  be_nested_str_weak(TLV),
        /* K2   */  be_nested_str_weak(get_attribute_name),
        /* K3   */  be_nested_str_weak(cluster),
        /* K4   */  be_nested_str_weak(attribute),
        /* K5   */  be_nested_str_weak(_X20_X28),
        /* K6   */  be_nested_str_weak(_X29),
        /* K7   */  be_nested_str_weak(),
        /* K8   */  be_nested_str_weak(read_attribute),
        /* K9   */  be_nested_str_weak(AttributeReportIB),
        /* K10  */  be_nested_str_weak(attribute_data),
        /* K11  */  be_nested_str_weak(AttributeDataIB),
        /* K12  */  be_nested_str_weak(data_version),
        /* K13  */  be_const_int(1),
        /* K14  */  be_nested_str_weak(path),
        /* K15  */  be_nested_str_weak(AttributePathIB),
        /* K16  */  be_nested_str_weak(endpoint),
        /* K17  */  be_nested_str_weak(data),
        /* K18  */  be_nested_str_weak(to_TLV),
        /* K19  */  be_nested_str_weak(encode_len),
        /* K20  */  be_nested_str_weak(tlv2raw),
        /* K21  */  be_nested_str_weak(tasmota),
        /* K22  */  be_nested_str_weak(log),
        /* K23  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
        /* K24  */  be_nested_str_weak(local_session_id),
        /* K25  */  be_const_int(3),
        /* K26  */  be_nested_str_weak(status),
        /* K27  */  be_nested_str_weak(attribute_status),
        /* K28  */  be_nested_str_weak(AttributeStatusIB),
        /* K29  */  be_nested_str_weak(StatusIB),
        /* K30  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K31  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
        /* K32  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20IGNORED),
        /* K33  */  be_nested_str_weak(attribute_reports),
        /* K34  */  be_const_int(0),
        /* K35  */  be_nested_str_weak(push),
        /* K36  */  be_nested_str_weak(IM_ReportData),
        /* K37  */  be_nested_str_weak(MAX_MESSAGE),
        }),
        be_str_weak(read_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[205]) {  /* code */
          0xB8120000,  //  0000  GETNGBL	R4	K0
          0x88100901,  //  0001  GETMBR	R4	R4	K1
          0xB8160000,  //  0002  GETNGBL	R5	K0
          0x8C140B02,  //  0003  GETMET	R5	R5	K2
          0x881C0503,  //  0004  GETMBR	R7	R2	K3
          0x88200504,  //  0005  GETMBR	R8	R2	K4
          0x7C140600,  //  0006  CALL	R5	3
          0x78160002,  //  0007  JMPF	R5	#000B
          0x001A0A05,  //  0008  ADD	R6	K5	R5
          0x00180D06,  //  0009  ADD	R6	R6	K6
          0x70020000,  //  000A  JMP		#000C
          0x58180007,  //  000B  LDCONST	R6	K7
          0x5C140C00,  //  000C  MOVE	R5	R6
          0x4C180000,  //  000D  LDNIL	R6
          0x20180206,  //  000E  NE	R6	R1	R6
          0x781A0004,  //  000F  JMPF	R6	#0015
          0x8C180308,  //  0010  GETMET	R6	R1	K8
          0x68200000,  //  0011  GETUPV	R8	U0
          0x5C240400,  //  0012  MOVE	R9	R2
          0x7C180600,  //  0013  CALL	R6	3
          0x70020000,  //  0014  JMP		#0016
          0x4C180000,  //  0015  LDNIL	R6
          0x501C0200,  //  0016  LDBOOL	R7	1	0
          0x4C200000,  //  0017  LDNIL	R8
          0x4C240000,  //  0018  LDNIL	R9
          0x20240C09,  //  0019  NE	R9	R6	R9
          0x7826003A,  //  001A  JMPF	R9	#0056
          0x60240008,  //  001B  GETGBL	R9	G8
          0x5C280C00,  //  001C  MOVE	R10	R6
          0x7C240200,  //  001D  CALL	R9	1
          0xB82A0000,  //  001E  GETNGBL	R10	K0
          0x8C281509,  //  001F  GETMET	R10	R10	K9
          0x7C280200,  //  0020  CALL	R10	1
          0xB82E0000,  //  0021  GETNGBL	R11	K0
          0x8C2C170B,  //  0022  GETMET	R11	R11	K11
          0x7C2C0200,  //  0023  CALL	R11	1
          0x902A140B,  //  0024  SETMBR	R10	K10	R11
          0x882C150A,  //  0025  GETMBR	R11	R10	K10
          0x902E190D,  //  0026  SETMBR	R11	K12	K13
          0x882C150A,  //  0027  GETMBR	R11	R10	K10
          0xB8320000,  //  0028  GETNGBL	R12	K0
          0x8C30190F,  //  0029  GETMET	R12	R12	K15
          0x7C300200,  //  002A  CALL	R12	1
          0x902E1C0C,  //  002B  SETMBR	R11	K14	R12
          0x882C150A,  //  002C  GETMBR	R11	R10	K10
          0x882C170E,  //  002D  GETMBR	R11	R11	K14
          0x88300510,  //  002E  GETMBR	R12	R2	K16
          0x902E200C,  //  002F  SETMBR	R11	K16	R12
          0x882C150A,  //  0030  GETMBR	R11	R10	K10
          0x882C170E,  //  0031  GETMBR	R11	R11	K14
          0x88300503,  //  0032  GETMBR	R12	R2	K3
          0x902E060C,  //  0033  SETMBR	R11	K3	R12
          0x882C150A,  //  0034  GETMBR	R11	R10	K10
          0x882C170E,  //  0035  GETMBR	R11	R11	K14
          0x88300504,  //  0036  GETMBR	R12	R2	K4
          0x902E080C,  //  0037  SETMBR	R11	K4	R12
          0x882C150A,  //  0038  GETMBR	R11	R10	K10
          0x902E2206,  //  0039  SETMBR	R11	K17	R6
          0x8C2C1512,  //  003A  GETMET	R11	R10	K18
          0x7C2C0200,  //  003B  CALL	R11	1
          0x8C301713,  //  003C  GETMET	R12	R11	K19
          0x7C300200,  //  003D  CALL	R12	1
          0x60340015,  //  003E  GETGBL	R13	G21
          0x5C381800,  //  003F  MOVE	R14	R12
          0x7C340200,  //  0040  CALL	R13	1
          0x8C381714,  //  0041  GETMET	R14	R11	K20
          0x5C401A00,  //  0042  MOVE	R16	R13
          0x7C380400,  //  0043  CALL	R14	2
          0x5C201C00,  //  0044  MOVE	R8	R14
          0x68380001,  //  0045  GETUPV	R14	U1
          0x743A000D,  //  0046  JMPT	R14	#0055
          0xB83A2A00,  //  0047  GETNGBL	R14	K21
          0x8C381D16,  //  0048  GETMET	R14	R14	K22
          0x60400018,  //  0049  GETGBL	R16	G24
          0x58440017,  //  004A  LDCONST	R17	K23
          0x68480000,  //  004B  GETUPV	R18	U0
          0x88482518,  //  004C  GETMBR	R18	R18	K24
          0x604C0008,  //  004D  GETGBL	R19	G8
          0x5C500400,  //  004E  MOVE	R20	R2
          0x7C4C0200,  //  004F  CALL	R19	1
          0x5C500A00,  //  0050  MOVE	R20	R5
          0x5C541200,  //  0051  MOVE	R21	R9
          0x7C400A00,  //  0052  CALL	R16	5
          0x58440019,  //  0053  LDCONST	R17	K25
          0x7C380600,  //  0054  CALL	R14	3
          0x70020055,  //  0055  JMP		#00AC
          0x8824051A,  //  0056  GETMBR	R9	R2	K26
          0x4C280000,  //  0057  LDNIL	R10
          0x2024120A,  //  0058  NE	R9	R9	R10
          0x78260043,  //  0059  JMPF	R9	#009E
          0x780E0041,  //  005A  JMPF	R3	#009D
          0xB8260000,  //  005B  GETNGBL	R9	K0
          0x8C241309,  //  005C  GETMET	R9	R9	K9
          0x7C240200,  //  005D  CALL	R9	1
          0xB82A0000,  //  005E  GETNGBL	R10	K0
          0x8C28151C,  //  005F  GETMET	R10	R10	K28
          0x7C280200,  //  0060  CALL	R10	1
          0x9026360A,  //  0061  SETMBR	R9	K27	R10
          0x8828131B,  //  0062  GETMBR	R10	R9	K27
          0xB82E0000,  //  0063  GETNGBL	R11	K0
          0x8C2C170F,  //  0064  GETMET	R11	R11	K15
          0x7C2C0200,  //  0065  CALL	R11	1
          0x902A1C0B,  //  0066  SETMBR	R10	K14	R11
          0x8828131B,  //  0067  GETMBR	R10	R9	K27
          0xB82E0000,  //  0068  GETNGBL	R11	K0
          0x8C2C171D,  //  0069  GETMET	R11	R11	K29
          0x7C2C0200,  //  006A  CALL	R11	1
          0x902A340B,  //  006B  SETMBR	R10	K26	R11
          0x8828131B,  //  006C  GETMBR	R10	R9	K27
          0x8828150E,  //  006D  GETMBR	R10	R10	K14
          0x882C0510,  //  006E  GETMBR	R11	R2	K16
          0x902A200B,  //  006F  SETMBR	R10	K16	R11
          0x8828131B,  //  0070  GETMBR	R10	R9	K27
          0x8828150E,  //  0071  GETMBR	R10	R10	K14
          0x882C0503,  //  0072  GETMBR	R11	R2	K3
          0x902A060B,  //  0073  SETMBR	R10	K3	R11
          0x8828131B,  //  0074  GETMBR	R10	R9	K27
          0x8828150E,  //  0075  GETMBR	R10	R10	K14
          0x882C0504,  //  0076  GETMBR	R11	R2	K4
          0x902A080B,  //  0077  SETMBR	R10	K4	R11
          0x8828131B,  //  0078  GETMBR	R10	R9	K27
          0x8828151A,  //  0079  GETMBR	R10	R10	K26
          0x882C051A,  //  007A  GETMBR	R11	R2	K26
          0x902A340B,  //  007B  SETMBR	R10	K26	R11
          0x8C281312,  //  007C  GETMET	R10	R9	K18
          0x7C280200,  //  007D  CALL	R10	1
          0x8C2C1513,  //  007E  GETMET	R11	R10	K19
          0x7C2C0200,  //  007F  CALL	R11	1
          0x60300015,  //  0080  GETGBL	R12	G21
          0x5C341600,  //  0081  MOVE	R13	R11
          0x7C300200,  //  0082  CALL	R12	1
          0x8C341514,  //  0083  GETMET	R13	R10	K20
          0x5C3C1800,  //  0084  MOVE	R15	R12
          0x7C340400,  //  0085  CALL	R13	2
          0x5C201A00,  //  0086  MOVE	R8	R13
          0xB8362A00,  //  0087  GETNGBL	R13	K21
          0x8C341B16,  //  0088  GETMET	R13	R13	K22
          0x603C0018,  //  0089  GETGBL	R15	G24
          0x5840001E,  //  008A  LDCONST	R16	K30
          0x68440000,  //  008B  GETUPV	R17	U0
          0x88442318,  //  008C  GETMBR	R17	R17	K24
          0x60480008,  //  008D  GETGBL	R18	G8
          0x5C4C0400,  //  008E  MOVE	R19	R2
          0x7C480200,  //  008F  CALL	R18	1
          0x5C4C0A00,  //  0090  MOVE	R19	R5
          0x8850051A,  //  0091  GETMBR	R20	R2	K26
          0x8854051A,  //  0092  GETMBR	R21	R2	K26
          0xB85A0000,  //  0093  GETNGBL	R22	K0
          0x88582D1F,  //  0094  GETMBR	R22	R22	K31
          0x1C542A16,  //  0095  EQ	R21	R21	R22
          0x78560001,  //  0096  JMPF	R21	#0099
          0x5854001F,  //  0097  LDCONST	R21	K31
          0x70020000,  //  0098  JMP		#009A
          0x58540007,  //  0099  LDCONST	R21	K7
          0x7C3C0C00,  //  009A  CALL	R15	6
          0x58400019,  //  009B  LDCONST	R16	K25
          0x7C340600,  //  009C  CALL	R13	3
          0x7002000D,  //  009D  JMP		#00AC
          0xB8262A00,  //  009E  GETNGBL	R9	K21
          0x8C241316,  //  009F  GETMET	R9	R9	K22
          0x602C0018,  //  00A0  GETGBL	R11	G24
          0x58300020,  //  00A1  LDCONST	R12	K32
          0x68340000,  //  00A2  GETUPV	R13	U0
          0x88341B18,  //  00A3  GETMBR	R13	R13	K24
          0x60380008,  //  00A4  GETGBL	R14	G8
          0x5C3C0400,  //  00A5  MOVE	R15	R2
          0x7C380200,  //  00A6  CALL	R14	1
          0x5C3C0A00,  //  00A7  MOVE	R15	R5
          0x7C2C0800,  //  00A8  CALL	R11	4
          0x58300019,  //  00A9  LDCONST	R12	K25
          0x7C240600,  //  00AA  CALL	R9	3
          0x501C0000,  //  00AB  LDBOOL	R7	0	0
          0x7822001E,  //  00AC  JMPF	R8	#00CC
          0x6024000C,  //  00AD  GETGBL	R9	G12
          0x88280121,  //  00AE  GETMBR	R10	R0	K33
          0x7C240200,  //  00AF  CALL	R9	1
          0x1C241322,  //  00B0  EQ	R9	R9	K34
          0x78260004,  //  00B1  JMPF	R9	#00B7
          0x88240121,  //  00B2  GETMBR	R9	R0	K33
          0x8C241323,  //  00B3  GETMET	R9	R9	K35
          0x5C2C1000,  //  00B4  MOVE	R11	R8
          0x7C240400,  //  00B5  CALL	R9	2
          0x70020014,  //  00B6  JMP		#00CC
          0x5425FFFE,  //  00B7  LDINT	R9	-1
          0x88280121,  //  00B8  GETMBR	R10	R0	K33
          0x94241409,  //  00B9  GETIDX	R9	R10	R9
          0x602C000C,  //  00BA  GETGBL	R11	G12
          0x5C301200,  //  00BB  MOVE	R12	R9
          0x7C2C0200,  //  00BC  CALL	R11	1
          0x6030000C,  //  00BD  GETGBL	R12	G12
          0x5C341000,  //  00BE  MOVE	R13	R8
          0x7C300200,  //  00BF  CALL	R12	1
          0x002C160C,  //  00C0  ADD	R11	R11	R12
          0xB8320000,  //  00C1  GETNGBL	R12	K0
          0x88301924,  //  00C2  GETMBR	R12	R12	K36
          0x88301925,  //  00C3  GETMBR	R12	R12	K37
          0x182C160C,  //  00C4  LE	R11	R11	R12
          0x782E0001,  //  00C5  JMPF	R11	#00C8
          0x402C1208,  //  00C6  CONNECT	R11	R9	R8
          0x70020003,  //  00C7  JMP		#00CC
          0x88280121,  //  00C8  GETMBR	R10	R0	K33
          0x8C281523,  //  00C9  GETMET	R10	R10	K35
          0x5C301000,  //  00CA  MOVE	R12	R8
          0x7C280400,  //  00CB  CALL	R10	2
          0x80040E00,  //  00CC  RET	1	R7
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 4),
          be_local_const_upval(1, 7),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x68100001,  //  0001  GETUPV	R4	U1
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(get_active_endpoints),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Path),
    /* K4   */  be_nested_str_weak(ReportDataMessage),
    /* K5   */  be_nested_str_weak(attribute_reports),
    /* K6   */  be_nested_str_weak(attributes_requests),
    /* K7   */  be_nested_str_weak(endpoint),
    /* K8   */  be_nested_str_weak(cluster),
    /* K9   */  be_nested_str_weak(attribute),
    /* K10  */  be_nested_str_weak(status),
    /* K11  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K12  */  be_nested_str_weak(get_attribute_name),
    /* K13  */  be_nested_str_weak(tasmota),
    /* K14  */  be_nested_str_weak(log),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s),
    /* K16  */  be_nested_str_weak(local_session_id),
    /* K17  */  be_nested_str_weak(_X20_X28),
    /* K18  */  be_nested_str_weak(_X29),
    /* K19  */  be_nested_str_weak(),
    /* K20  */  be_const_int(3),
    /* K21  */  be_nested_str_weak(process_attribute_expansion),
    /* K22  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_inner_process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
      0x84100000,  //  0000  CLOSURE	R4	P0
      0x88140100,  //  0001  GETMBR	R5	R0	K0
      0x8C140B01,  //  0002  GETMET	R5	R5	K1
      0x7C140200,  //  0003  CALL	R5	1
      0xB81A0400,  //  0004  GETNGBL	R6	K2
      0x8C180D03,  //  0005  GETMET	R6	R6	K3
      0x7C180200,  //  0006  CALL	R6	1
      0xB81E0400,  //  0007  GETNGBL	R7	K2
      0x8C1C0F04,  //  0008  GETMET	R7	R7	K4
      0x7C1C0200,  //  0009  CALL	R7	1
      0x60200012,  //  000A  GETGBL	R8	G18
      0x7C200000,  //  000B  CALL	R8	0
      0x901E0A08,  //  000C  SETMBR	R7	K5	R8
      0x60200010,  //  000D  GETGBL	R8	G16
      0x88240506,  //  000E  GETMBR	R9	R2	K6
      0x7C200200,  //  000F  CALL	R8	1
      0xA8020046,  //  0010  EXBLK	0	#0058
      0x5C241000,  //  0011  MOVE	R9	R8
      0x7C240000,  //  0012  CALL	R9	0
      0x88281307,  //  0013  GETMBR	R10	R9	K7
      0x901A0E0A,  //  0014  SETMBR	R6	K7	R10
      0x88281308,  //  0015  GETMBR	R10	R9	K8
      0x901A100A,  //  0016  SETMBR	R6	K8	R10
      0x88281309,  //  0017  GETMBR	R10	R9	K9
      0x901A120A,  //  0018  SETMBR	R6	K9	R10
      0xB82A0400,  //  0019  GETNGBL	R10	K2
      0x8828150B,  //  001A  GETMBR	R10	R10	K11
      0x901A140A,  //  001B  SETMBR	R6	K10	R10
      0x88280D07,  //  001C  GETMBR	R10	R6	K7
      0x4C2C0000,  //  001D  LDNIL	R11
      0x1C28140B,  //  001E  EQ	R10	R10	R11
      0x742A0007,  //  001F  JMPT	R10	#0028
      0x88280D08,  //  0020  GETMBR	R10	R6	K8
      0x4C2C0000,  //  0021  LDNIL	R11
      0x1C28140B,  //  0022  EQ	R10	R10	R11
      0x742A0003,  //  0023  JMPT	R10	#0028
      0x88280D09,  //  0024  GETMBR	R10	R6	K9
      0x4C2C0000,  //  0025  LDNIL	R11
      0x1C28140B,  //  0026  EQ	R10	R10	R11
      0x782A0029,  //  0027  JMPF	R10	#0052
      0x88280D08,  //  0028  GETMBR	R10	R6	K8
      0x4C2C0000,  //  0029  LDNIL	R11
      0x2028140B,  //  002A  NE	R10	R10	R11
      0x782A001A,  //  002B  JMPF	R10	#0047
      0x88280D09,  //  002C  GETMBR	R10	R6	K9
      0x4C2C0000,  //  002D  LDNIL	R11
      0x2028140B,  //  002E  NE	R10	R10	R11
      0x782A0016,  //  002F  JMPF	R10	#0047
      0xB82A0400,  //  0030  GETNGBL	R10	K2
      0x8C28150C,  //  0031  GETMET	R10	R10	K12
      0x88300D08,  //  0032  GETMBR	R12	R6	K8
      0x88340D09,  //  0033  GETMBR	R13	R6	K9
      0x7C280600,  //  0034  CALL	R10	3
      0xB82E1A00,  //  0035  GETNGBL	R11	K13
      0x8C2C170E,  //  0036  GETMET	R11	R11	K14
      0x60340018,  //  0037  GETGBL	R13	G24
      0x5838000F,  //  0038  LDCONST	R14	K15
      0x883C0310,  //  0039  GETMBR	R15	R1	K16
      0x60400008,  //  003A  GETGBL	R16	G8
      0x5C440C00,  //  003B  MOVE	R17	R6
      0x7C400200,  //  003C  CALL	R16	1
      0x782A0002,  //  003D  JMPF	R10	#0041
      0x0046220A,  //  003E  ADD	R17	K17	R10
      0x00442312,  //  003F  ADD	R17	R17	K18
      0x70020000,  //  0040  JMP		#0042
      0x58440013,  //  0041  LDCONST	R17	K19
      0x00402011,  //  0042  ADD	R16	R16	R17
      0x7C340600,  //  0043  CALL	R13	3
      0x58380014,  //  0044  LDCONST	R14	K20
      0x7C2C0600,  //  0045  CALL	R11	3
      0x7002000A,  //  0046  JMP		#0052
      0xB82A1A00,  //  0047  GETNGBL	R10	K13
      0x8C28150E,  //  0048  GETMET	R10	R10	K14
      0x60300018,  //  0049  GETGBL	R12	G24
      0x5834000F,  //  004A  LDCONST	R13	K15
      0x88380310,  //  004B  GETMBR	R14	R1	K16
      0x603C0008,  //  004C  GETGBL	R15	G8
      0x5C400C00,  //  004D  MOVE	R16	R6
      0x7C3C0200,  //  004E  CALL	R15	1
      0x7C300600,  //  004F  CALL	R12	3
      0x58340014,  //  0050  LDCONST	R13	K20
      0x7C280600,  //  0051  CALL	R10	3
      0x88280100,  //  0052  GETMBR	R10	R0	K0
      0x8C281515,  //  0053  GETMET	R10	R10	K21
      0x5C300C00,  //  0054  MOVE	R12	R6
      0x84340001,  //  0055  CLOSURE	R13	P1
      0x7C280600,  //  0056  CALL	R10	3
      0x7001FFB8,  //  0057  JMP		#0011
      0x58200016,  //  0058  LDCONST	R8	K22
      0xAC200200,  //  0059  CATCH	R8	1	0
      0xB0080000,  //  005A  RAISE	2	R0	R0
      0xA0000000,  //  005B  CLOSE	R0
      0x80040E00,  //  005C  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_sendqueue_by_exchangeid
********************************************************************/
be_local_closure(Matter_IM_find_sendqueue_by_exchangeid,   /* name */
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
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(get_exchangeid),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(find_sendqueue_by_exchangeid),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x58080000,  //  0005  LDCONST	R2	K0
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x88100101,  //  0007  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0008  CALL	R3	1
      0x140C0403,  //  0009  LT	R3	R2	R3
      0x780E0008,  //  000A  JMPF	R3	#0014
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x940C0602,  //  000C  GETIDX	R3	R3	R2
      0x8C100702,  //  000D  GETMET	R4	R3	K2
      0x7C100200,  //  000E  CALL	R4	1
      0x1C100801,  //  000F  EQ	R4	R4	R1
      0x78120000,  //  0010  JMPF	R4	#0012
      0x80040600,  //  0011  RET	1	R3
      0x00080503,  //  0012  ADD	R2	R2	K3
      0x7001FFF1,  //  0013  JMP		#0006
      0x4C0C0000,  //  0014  LDNIL	R3
      0x80040600,  //  0015  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming_ack
********************************************************************/
be_local_closure(Matter_IM_process_incoming_ack,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(find_sendqueue_by_exchangeid),
    /* K1   */  be_nested_str_weak(exchange_id),
    /* K2   */  be_nested_str_weak(ack_received),
    }),
    be_str_weak(process_incoming_ack),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x88100301,  //  0001  GETMBR	R4	R1	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x780A0003,  //  0003  JMPF	R2	#0008
      0x8C0C0502,  //  0004  GETMET	R3	R2	K2
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C0C0400,  //  0006  CALL	R3	2
      0x80040600,  //  0007  RET	1	R3
      0x500C0000,  //  0008  LDBOOL	R3	0	0
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_read_request
********************************************************************/
be_local_closure(Matter_IM_process_read_request,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(read_request_start),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(ReadRequestMessage),
    /* K6   */  be_nested_str_weak(from_TLV),
    /* K7   */  be_nested_str_weak(attributes_requests),
    /* K8   */  be_nested_str_weak(_inner_process_read_request),
    /* K9   */  be_nested_str_weak(session),
    /* K10  */  be_nested_str_weak(send_report_data),
    }),
    be_str_weak(process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0xB80E0800,  //  0005  GETNGBL	R3	K4
      0x8C0C0705,  //  0006  GETMET	R3	R3	K5
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0706,  //  0008  GETMET	R3	R3	K6
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x88100707,  //  000B  GETMBR	R4	R3	K7
      0x4C140000,  //  000C  LDNIL	R5
      0x20100805,  //  000D  NE	R4	R4	R5
      0x78120007,  //  000E  JMPF	R4	#0017
      0x8C100108,  //  000F  GETMET	R4	R0	K8
      0x88180309,  //  0010  GETMBR	R6	R1	K9
      0x5C1C0600,  //  0011  MOVE	R7	R3
      0x7C100600,  //  0012  CALL	R4	3
      0x8C14010A,  //  0013  GETMET	R5	R0	K10
      0x5C1C0200,  //  0014  MOVE	R7	R1
      0x5C200800,  //  0015  MOVE	R8	R4
      0x7C140600,  //  0016  CALL	R5	3
      0x50100200,  //  0017  LDBOOL	R4	1	0
      0x80040800,  //  0018  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_report_data
********************************************************************/
be_local_closure(Matter_IM_send_report_data,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_ReportData),
    }),
    be_str_weak(send_report_data),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_status_response
********************************************************************/
be_local_closure(Matter_IM_process_status_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(findsubval),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(find_sendqueue_by_exchangeid),
    /* K3   */  be_nested_str_weak(exchange_id),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(SUCCESS),
    /* K6   */  be_nested_str_weak(status_ok_received),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20_X3EOK_X20_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i_X20not_X20found),
    /* K10  */  be_nested_str_weak(session),
    /* K11  */  be_nested_str_weak(local_session_id),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20ERROR_X20_X3D_X200x_X2502X),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(status_error_received),
    /* K15  */  be_nested_str_weak(remove_sendqueue_by_exchangeid),
    }),
    be_str_weak(process_status_response),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x8C0C0500,  //  0000  GETMET	R3	R2	K0
      0x58140001,  //  0001  LDCONST	R5	K1
      0x541A00FE,  //  0002  LDINT	R6	255
      0x7C0C0600,  //  0003  CALL	R3	3
      0x8C100102,  //  0004  GETMET	R4	R0	K2
      0x88180303,  //  0005  GETMBR	R6	R1	K3
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x88140B05,  //  0008  GETMBR	R5	R5	K5
      0x1C140605,  //  0009  EQ	R5	R3	R5
      0x78160010,  //  000A  JMPF	R5	#001C
      0x78120004,  //  000B  JMPF	R4	#0011
      0x8C140906,  //  000C  GETMET	R5	R4	K6
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x7C140400,  //  000E  CALL	R5	2
      0x80040A00,  //  000F  RET	1	R5
      0x70020009,  //  0010  JMP		#001B
      0xB8160E00,  //  0011  GETNGBL	R5	K7
      0x8C140B08,  //  0012  GETMET	R5	R5	K8
      0x601C0018,  //  0013  GETGBL	R7	G24
      0x58200009,  //  0014  LDCONST	R8	K9
      0x8824030A,  //  0015  GETMBR	R9	R1	K10
      0x8824130B,  //  0016  GETMBR	R9	R9	K11
      0x88280303,  //  0017  GETMBR	R10	R1	K3
      0x7C1C0600,  //  0018  CALL	R7	3
      0x54220003,  //  0019  LDINT	R8	4
      0x7C140600,  //  001A  CALL	R5	3
      0x7002000E,  //  001B  JMP		#002B
      0xB8160E00,  //  001C  GETNGBL	R5	K7
      0x8C140B08,  //  001D  GETMET	R5	R5	K8
      0x601C0018,  //  001E  GETGBL	R7	G24
      0x5820000C,  //  001F  LDCONST	R8	K12
      0x5C240600,  //  0020  MOVE	R9	R3
      0x7C1C0400,  //  0021  CALL	R7	2
      0x5820000D,  //  0022  LDCONST	R8	K13
      0x7C140600,  //  0023  CALL	R5	3
      0x78120005,  //  0024  JMPF	R4	#002B
      0x8C14090E,  //  0025  GETMET	R5	R4	K14
      0x5C1C0200,  //  0026  MOVE	R7	R1
      0x7C140400,  //  0027  CALL	R5	2
      0x8C14010F,  //  0028  GETMET	R5	R0	K15
      0x881C0303,  //  0029  GETMBR	R7	R1	K3
      0x7C140400,  //  002A  CALL	R5	2
      0x50140000,  //  002B  LDBOOL	R5	0	0
      0x80040A00,  //  002C  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_response
********************************************************************/
be_local_closure(Matter_IM_process_write_response,   /* name */
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
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(WriteResponseMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(process_write_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_update
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(ReadRequestMessage),
    /* K3   */  be_nested_str_weak(fabric_filtered),
    /* K4   */  be_nested_str_weak(attributes_requests),
    /* K5   */  be_nested_str_weak(updates),
    /* K6   */  be_nested_str_weak(AttributePathIB),
    /* K7   */  be_nested_str_weak(endpoint),
    /* K8   */  be_nested_str_weak(cluster),
    /* K9   */  be_nested_str_weak(attribute),
    /* K10  */  be_nested_str_weak(push),
    /* K11  */  be_nested_str_weak(stop_iteration),
    /* K12  */  be_nested_str_weak(tasmota),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Data_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K15  */  be_nested_str_weak(local_session_id),
    /* K16  */  be_nested_str_weak(subscription_id),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(is_keep_alive),
    /* K19  */  be_nested_str_weak(_inner_process_read_request),
    /* K20  */  be_nested_str_weak(suppress_response),
    /* K21  */  be_nested_str_weak(IM_ReportDataSubscribed),
    /* K22  */  be_nested_str_weak(_message_handler),
    /* K23  */  be_nested_str_weak(send_queue),
    /* K24  */  be_nested_str_weak(send_enqueued),
    }),
    be_str_weak(send_subscribe_update),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x50100000,  //  0004  LDBOOL	R4	0	0
      0x900E0604,  //  0005  SETMBR	R3	K3	R4
      0x60100012,  //  0006  GETGBL	R4	G18
      0x7C100000,  //  0007  CALL	R4	0
      0x900E0804,  //  0008  SETMBR	R3	K4	R4
      0x60100010,  //  0009  GETGBL	R4	G16
      0x88140305,  //  000A  GETMBR	R5	R1	K5
      0x7C100200,  //  000B  CALL	R4	1
      0xA802000F,  //  000C  EXBLK	0	#001D
      0x5C140800,  //  000D  MOVE	R5	R4
      0x7C140000,  //  000E  CALL	R5	0
      0xB81A0200,  //  000F  GETNGBL	R6	K1
      0x8C180D06,  //  0010  GETMET	R6	R6	K6
      0x7C180200,  //  0011  CALL	R6	1
      0x881C0B07,  //  0012  GETMBR	R7	R5	K7
      0x901A0E07,  //  0013  SETMBR	R6	K7	R7
      0x881C0B08,  //  0014  GETMBR	R7	R5	K8
      0x901A1007,  //  0015  SETMBR	R6	K8	R7
      0x881C0B09,  //  0016  GETMBR	R7	R5	K9
      0x901A1207,  //  0017  SETMBR	R6	K9	R7
      0x881C0704,  //  0018  GETMBR	R7	R3	K4
      0x8C1C0F0A,  //  0019  GETMET	R7	R7	K10
      0x5C240C00,  //  001A  MOVE	R9	R6
      0x7C1C0400,  //  001B  CALL	R7	2
      0x7001FFEF,  //  001C  JMP		#000D
      0x5810000B,  //  001D  LDCONST	R4	K11
      0xAC100200,  //  001E  CATCH	R4	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0xB8121800,  //  0020  GETNGBL	R4	K12
      0x8C10090D,  //  0021  GETMET	R4	R4	K13
      0x60180018,  //  0022  GETGBL	R6	G24
      0x581C000E,  //  0023  LDCONST	R7	K14
      0x8820050F,  //  0024  GETMBR	R8	R2	K15
      0x88240310,  //  0025  GETMBR	R9	R1	K16
      0x7C180600,  //  0026  CALL	R6	3
      0x581C0011,  //  0027  LDCONST	R7	K17
      0x7C100600,  //  0028  CALL	R4	3
      0x50100000,  //  0029  LDBOOL	R4	0	0
      0x90062404,  //  002A  SETMBR	R1	K18	R4
      0x8C100113,  //  002B  GETMET	R4	R0	K19
      0x5C180400,  //  002C  MOVE	R6	R2
      0x5C1C0600,  //  002D  MOVE	R7	R3
      0x7C100600,  //  002E  CALL	R4	3
      0x50140000,  //  002F  LDBOOL	R5	0	0
      0x90122805,  //  0030  SETMBR	R4	K20	R5
      0x88140310,  //  0031  GETMBR	R5	R1	K16
      0x90122005,  //  0032  SETMBR	R4	K16	R5
      0xB8160200,  //  0033  GETNGBL	R5	K1
      0x8C140B15,  //  0034  GETMET	R5	R5	K21
      0x881C0516,  //  0035  GETMBR	R7	R2	K22
      0x5C200400,  //  0036  MOVE	R8	R2
      0x5C240800,  //  0037  MOVE	R9	R4
      0x5C280200,  //  0038  MOVE	R10	R1
      0x7C140A00,  //  0039  CALL	R5	5
      0x88180117,  //  003A  GETMBR	R6	R0	K23
      0x8C180D0A,  //  003B  GETMET	R6	R6	K10
      0x5C200A00,  //  003C  MOVE	R8	R5
      0x7C180400,  //  003D  CALL	R6	2
      0x8C180118,  //  003E  GETMET	R6	R0	K24
      0x88200516,  //  003F  GETMBR	R8	R2	K22
      0x7C180400,  //  0040  CALL	R6	2
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_enqueued
********************************************************************/
be_local_closure(Matter_IM_send_enqueued,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(finish),
    /* K3   */  be_nested_str_weak(ready),
    /* K4   */  be_nested_str_weak(send_im),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20remove_X20IM_X20message_X20exch_X3D),
    /* K8   */  be_nested_str_weak(resp),
    /* K9   */  be_nested_str_weak(exchange_id),
    /* K10  */  be_nested_str_weak(remove),
    /* K11  */  be_const_int(1),
    }),
    be_str_weak(send_enqueued),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E001A,  //  0005  JMPF	R3	#0021
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x88100702,  //  0008  GETMBR	R4	R3	K2
      0x74120004,  //  0009  JMPT	R4	#000F
      0x88100703,  //  000A  GETMBR	R4	R3	K3
      0x78120002,  //  000B  JMPF	R4	#000F
      0x8C100704,  //  000C  GETMET	R4	R3	K4
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0x88100702,  //  000F  GETMBR	R4	R3	K2
      0x7812000D,  //  0010  JMPF	R4	#001F
      0xB8120A00,  //  0011  GETNGBL	R4	K5
      0x8C100906,  //  0012  GETMET	R4	R4	K6
      0x60180008,  //  0013  GETGBL	R6	G8
      0x881C0708,  //  0014  GETMBR	R7	R3	K8
      0x881C0F09,  //  0015  GETMBR	R7	R7	K9
      0x7C180200,  //  0016  CALL	R6	1
      0x001A0E06,  //  0017  ADD	R6	K7	R6
      0x541E0003,  //  0018  LDINT	R7	4
      0x7C100600,  //  0019  CALL	R4	3
      0x88100101,  //  001A  GETMBR	R4	R0	K1
      0x8C10090A,  //  001B  GETMET	R4	R4	K10
      0x5C180400,  //  001C  MOVE	R6	R2
      0x7C100400,  //  001D  CALL	R4	2
      0x70020000,  //  001E  JMP		#0020
      0x0008050B,  //  001F  ADD	R2	R2	K11
      0x7001FFDF,  //  0020  JMP		#0001
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_timed_request
********************************************************************/
be_local_closure(Matter_IM_process_timed_request,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TimedRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20TimedRequest_X3D_X25i),
    /* K6   */  be_nested_str_weak(session),
    /* K7   */  be_nested_str_weak(local_session_id),
    /* K8   */  be_nested_str_weak(timeout),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(send_status),
    /* K11  */  be_nested_str_weak(SUCCESS),
    }),
    be_str_weak(process_timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0xB8120600,  //  0006  GETNGBL	R4	K3
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x60180018,  //  0008  GETGBL	R6	G24
      0x581C0005,  //  0009  LDCONST	R7	K5
      0x88200306,  //  000A  GETMBR	R8	R1	K6
      0x88201107,  //  000B  GETMBR	R8	R8	K7
      0x88240708,  //  000C  GETMBR	R9	R3	K8
      0x7C180600,  //  000D  CALL	R6	3
      0x581C0009,  //  000E  LDCONST	R7	K9
      0x7C100600,  //  000F  CALL	R4	3
      0x8C10010A,  //  0010  GETMET	R4	R0	K10
      0x5C180200,  //  0011  MOVE	R6	R1
      0xB81E0000,  //  0012  GETNGBL	R7	K0
      0x881C0F0B,  //  0013  GETMBR	R7	R7	K11
      0x7C100600,  //  0014  CALL	R4	3
      0x50100200,  //  0015  LDBOOL	R4	1	0
      0x80040800,  //  0016  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: report_data
********************************************************************/
be_local_closure(Matter_IM_report_data,   /* name */
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
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(ReportDataMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(report_data),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_response
********************************************************************/
be_local_closure(Matter_IM_process_invoke_response,   /* name */
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
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(InvokeResponseMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(process_invoke_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_init,   /* name */
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
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(subs_shop),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(IM_Subscription_Shop),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0xB80A0600,  //  0004  GETNGBL	R2	K3
      0x8C080504,  //  0005  GETMET	R2	R2	K4
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_heartbeat
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_heartbeat,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Alive_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K4   */  be_nested_str_weak(local_session_id),
    /* K5   */  be_nested_str_weak(subscription_id),
    /* K6   */  be_const_int(3),
    /* K7   */  be_nested_str_weak(is_keep_alive),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(ReportDataMessage),
    /* K10  */  be_nested_str_weak(suppress_response),
    /* K11  */  be_nested_str_weak(IM_SubscribedHeartbeat),
    /* K12  */  be_nested_str_weak(_message_handler),
    /* K13  */  be_nested_str_weak(send_queue),
    /* K14  */  be_nested_str_weak(push),
    /* K15  */  be_nested_str_weak(send_enqueued),
    }),
    be_str_weak(send_subscribe_heartbeat),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x60140018,  //  0003  GETGBL	R5	G24
      0x58180003,  //  0004  LDCONST	R6	K3
      0x881C0504,  //  0005  GETMBR	R7	R2	K4
      0x88200305,  //  0006  GETMBR	R8	R1	K5
      0x7C140600,  //  0007  CALL	R5	3
      0x58180006,  //  0008  LDCONST	R6	K6
      0x7C0C0600,  //  0009  CALL	R3	3
      0x500C0200,  //  000A  LDBOOL	R3	1	0
      0x90060E03,  //  000B  SETMBR	R1	K7	R3
      0xB80E1000,  //  000C  GETNGBL	R3	K8
      0x8C0C0709,  //  000D  GETMET	R3	R3	K9
      0x7C0C0200,  //  000E  CALL	R3	1
      0x50100200,  //  000F  LDBOOL	R4	1	0
      0x900E1404,  //  0010  SETMBR	R3	K10	R4
      0x88100305,  //  0011  GETMBR	R4	R1	K5
      0x900E0A04,  //  0012  SETMBR	R3	K5	R4
      0xB8121000,  //  0013  GETNGBL	R4	K8
      0x8C10090B,  //  0014  GETMET	R4	R4	K11
      0x8818050C,  //  0015  GETMBR	R6	R2	K12
      0x5C1C0400,  //  0016  MOVE	R7	R2
      0x5C200600,  //  0017  MOVE	R8	R3
      0x5C240200,  //  0018  MOVE	R9	R1
      0x7C100A00,  //  0019  CALL	R4	5
      0x8814010D,  //  001A  GETMBR	R5	R0	K13
      0x8C140B0E,  //  001B  GETMET	R5	R5	K14
      0x5C1C0800,  //  001C  MOVE	R7	R4
      0x7C140400,  //  001D  CALL	R5	2
      0x8C14010F,  //  001E  GETMET	R5	R0	K15
      0x881C050C,  //  001F  GETMBR	R7	R2	K12
      0x7C140400,  //  0020  CALL	R5	2
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM
********************************************************************/
be_local_class(Matter_IM,
    3,
    NULL,
    be_nested_map(31,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(send_subscribe_heartbeat, -1), be_const_closure(Matter_IM_send_subscribe_heartbeat_closure) },
        { be_const_key_weak(subs_shop, 23), be_const_var(1) },
        { be_const_key_weak(send_queue, 30), be_const_var(2) },
        { be_const_key_weak(process_invoke_request, -1), be_const_closure(Matter_IM_process_invoke_request_closure) },
        { be_const_key_weak(subscribe_request, -1), be_const_closure(Matter_IM_subscribe_request_closure) },
        { be_const_key_weak(process_write_request, 11), be_const_closure(Matter_IM_process_write_request_closure) },
        { be_const_key_weak(send_write_response, -1), be_const_closure(Matter_IM_send_write_response_closure) },
        { be_const_key_weak(remove_sendqueue_by_exchangeid, -1), be_const_closure(Matter_IM_remove_sendqueue_by_exchangeid_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_IM_every_second_closure) },
        { be_const_key_weak(send_ack_now, -1), be_const_closure(Matter_IM_send_ack_now_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_IM_every_250ms_closure) },
        { be_const_key_weak(send_invoke_response, -1), be_const_closure(Matter_IM_send_invoke_response_closure) },
        { be_const_key_weak(process_invoke_response, 21), be_const_closure(Matter_IM_process_invoke_response_closure) },
        { be_const_key_weak(_inner_process_read_request, -1), be_const_closure(Matter_IM__inner_process_read_request_closure) },
        { be_const_key_weak(subscribe_response, 22), be_const_closure(Matter_IM_subscribe_response_closure) },
        { be_const_key_weak(process_incoming, -1), be_const_closure(Matter_IM_process_incoming_closure) },
        { be_const_key_weak(send_subscribe_response, 18), be_const_closure(Matter_IM_send_subscribe_response_closure) },
        { be_const_key_weak(find_sendqueue_by_exchangeid, -1), be_const_closure(Matter_IM_find_sendqueue_by_exchangeid_closure) },
        { be_const_key_weak(process_incoming_ack, 13), be_const_closure(Matter_IM_process_incoming_ack_closure) },
        { be_const_key_weak(process_read_request, -1), be_const_closure(Matter_IM_process_read_request_closure) },
        { be_const_key_weak(send_report_data, -1), be_const_closure(Matter_IM_send_report_data_closure) },
        { be_const_key_weak(process_timed_request, -1), be_const_closure(Matter_IM_process_timed_request_closure) },
        { be_const_key_weak(process_status_response, -1), be_const_closure(Matter_IM_process_status_response_closure) },
        { be_const_key_weak(send_subscribe_update, 28), be_const_closure(Matter_IM_send_subscribe_update_closure) },
        { be_const_key_weak(send_enqueued, -1), be_const_closure(Matter_IM_send_enqueued_closure) },
        { be_const_key_weak(process_write_response, 12), be_const_closure(Matter_IM_process_write_response_closure) },
        { be_const_key_weak(report_data, 0), be_const_closure(Matter_IM_report_data_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(send_status, -1), be_const_closure(Matter_IM_send_status_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_init_closure) },
        { be_const_key_weak(expire_sendqueue, -1), be_const_closure(Matter_IM_expire_sendqueue_closure) },
    })),
    be_str_weak(Matter_IM)
);
/*******************************************************************/

void be_load_Matter_IM_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM);
    be_setglobal(vm, "Matter_IM");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
