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
    22,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[45]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Path),
    /* K3   */  be_nested_str_weak(InvokeRequestMessage),
    /* K4   */  be_nested_str_weak(from_TLV),
    /* K5   */  be_nested_str_weak(invoke_requests),
    /* K6   */  be_nested_str_weak(InvokeResponseMessage),
    /* K7   */  be_nested_str_weak(suppress_response),
    /* K8   */  be_nested_str_weak(invoke_responses),
    /* K9   */  be_nested_str_weak(endpoint),
    /* K10  */  be_nested_str_weak(command_path),
    /* K11  */  be_nested_str_weak(cluster),
    /* K12  */  be_nested_str_weak(command),
    /* K13  */  be_nested_str_weak(status),
    /* K14  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K15  */  be_nested_str_weak(get_command_name),
    /* K16  */  be_nested_str_weak(device),
    /* K17  */  be_nested_str_weak(invoke_request),
    /* K18  */  be_nested_str_weak(session),
    /* K19  */  be_nested_str_weak(command_fields),
    /* K20  */  be_nested_str_weak(log),
    /* K21  */  be_nested_str_weak(_X28),
    /* K22  */  be_nested_str_weak(_X29_X20),
    /* K23  */  be_nested_str_weak(),
    /* K24  */  be_nested_str_weak(tasmota),
    /* K25  */  be_nested_str_weak(format),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
    /* K27  */  be_nested_str_weak(local_session_id),
    /* K28  */  be_const_int(0),
    /* K29  */  be_const_int(2),
    /* K30  */  be_const_int(3),
    /* K31  */  be_nested_str_weak(InvokeResponseIB),
    /* K32  */  be_nested_str_weak(SUCCESS),
    /* K33  */  be_nested_str_weak(CommandStatusIB),
    /* K34  */  be_nested_str_weak(CommandPathIB),
    /* K35  */  be_nested_str_weak(StatusIB),
    /* K36  */  be_nested_str_weak(push),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20OK_X20exch_X3D_X25i),
    /* K38  */  be_nested_str_weak(exchange_id),
    /* K39  */  be_nested_str_weak(CommandDataIB),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20Status_X3D0x_X2502X_X20exch_X3D_X25i),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20_Ignore_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i),
    /* K43  */  be_nested_str_weak(stop_iteration),
    /* K44  */  be_nested_str_weak(send_invoke_response),
    }),
    be_str_weak(process_invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[276]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0xB8160200,  //  0004  GETNGBL	R5	K1
      0x8C140B03,  //  0005  GETMET	R5	R5	K3
      0x7C140200,  //  0006  CALL	R5	1
      0x8C140B04,  //  0007  GETMET	R5	R5	K4
      0x5C1C0400,  //  0008  MOVE	R7	R2
      0x7C140400,  //  0009  CALL	R5	2
      0x88180B05,  //  000A  GETMBR	R6	R5	K5
      0x4C1C0000,  //  000B  LDNIL	R7
      0x20180C07,  //  000C  NE	R6	R6	R7
      0x781A0104,  //  000D  JMPF	R6	#0113
      0xB81A0200,  //  000E  GETNGBL	R6	K1
      0x8C180D06,  //  000F  GETMET	R6	R6	K6
      0x7C180200,  //  0010  CALL	R6	1
      0x501C0000,  //  0011  LDBOOL	R7	0	0
      0x901A0E07,  //  0012  SETMBR	R6	K7	R7
      0x601C0012,  //  0013  GETGBL	R7	G18
      0x7C1C0000,  //  0014  CALL	R7	0
      0x901A1007,  //  0015  SETMBR	R6	K8	R7
      0x601C0010,  //  0016  GETGBL	R7	G16
      0x88200B05,  //  0017  GETMBR	R8	R5	K5
      0x7C1C0200,  //  0018  CALL	R7	1
      0xA80200E7,  //  0019  EXBLK	0	#0102
      0x5C200E00,  //  001A  MOVE	R8	R7
      0x7C200000,  //  001B  CALL	R8	0
      0x8824110A,  //  001C  GETMBR	R9	R8	K10
      0x88241309,  //  001D  GETMBR	R9	R9	K9
      0x90121209,  //  001E  SETMBR	R4	K9	R9
      0x8824110A,  //  001F  GETMBR	R9	R8	K10
      0x8824130B,  //  0020  GETMBR	R9	R9	K11
      0x90121609,  //  0021  SETMBR	R4	K11	R9
      0x8824110A,  //  0022  GETMBR	R9	R8	K10
      0x8824130C,  //  0023  GETMBR	R9	R9	K12
      0x90121809,  //  0024  SETMBR	R4	K12	R9
      0xB8260200,  //  0025  GETNGBL	R9	K1
      0x8824130E,  //  0026  GETMBR	R9	R9	K14
      0x90121A09,  //  0027  SETMBR	R4	K13	R9
      0xB8260200,  //  0028  GETNGBL	R9	K1
      0x8C24130F,  //  0029  GETMET	R9	R9	K15
      0x882C090B,  //  002A  GETMBR	R11	R4	K11
      0x8830090C,  //  002B  GETMBR	R12	R4	K12
      0x7C240600,  //  002C  CALL	R9	3
      0x60280008,  //  002D  GETGBL	R10	G8
      0x5C2C0800,  //  002E  MOVE	R11	R4
      0x7C280200,  //  002F  CALL	R10	1
      0x882C0110,  //  0030  GETMBR	R11	R0	K16
      0x8C2C1711,  //  0031  GETMET	R11	R11	K17
      0x88340312,  //  0032  GETMBR	R13	R1	K18
      0x88381113,  //  0033  GETMBR	R14	R8	K19
      0x5C3C0800,  //  0034  MOVE	R15	R4
      0x7C2C0800,  //  0035  CALL	R11	4
      0x88300914,  //  0036  GETMBR	R12	R4	K20
      0x4C340000,  //  0037  LDNIL	R13
      0x2030180D,  //  0038  NE	R12	R12	R13
      0x78320005,  //  0039  JMPF	R12	#0040
      0x60300008,  //  003A  GETGBL	R12	G8
      0x88340914,  //  003B  GETMBR	R13	R4	K20
      0x7C300200,  //  003C  CALL	R12	1
      0x00322A0C,  //  003D  ADD	R12	K21	R12
      0x00301916,  //  003E  ADD	R12	R12	K22
      0x70020000,  //  003F  JMP		#0041
      0x58300017,  //  0040  LDCONST	R12	K23
      0xB8363000,  //  0041  GETNGBL	R13	K24
      0x8C341B14,  //  0042  GETMET	R13	R13	K20
      0x8C3C0719,  //  0043  GETMET	R15	R3	K25
      0x5844001A,  //  0044  LDCONST	R17	K26
      0x88480312,  //  0045  GETMBR	R18	R1	K18
      0x8848251B,  //  0046  GETMBR	R18	R18	K27
      0x5C4C1400,  //  0047  MOVE	R19	R10
      0x78260001,  //  0048  JMPF	R9	#004B
      0x5C501200,  //  0049  MOVE	R20	R9
      0x70020000,  //  004A  JMP		#004C
      0x58500017,  //  004B  LDCONST	R20	K23
      0x5C541800,  //  004C  MOVE	R21	R12
      0x7C3C0C00,  //  004D  CALL	R15	6
      0x88400909,  //  004E  GETMBR	R16	R4	K9
      0x2040211C,  //  004F  NE	R16	R16	K28
      0x78420001,  //  0050  JMPF	R16	#0053
      0x5840001D,  //  0051  LDCONST	R16	K29
      0x70020000,  //  0052  JMP		#0054
      0x5840001E,  //  0053  LDCONST	R16	K30
      0x7C340600,  //  0054  CALL	R13	3
      0x4C340000,  //  0055  LDNIL	R13
      0x9012280D,  //  0056  SETMBR	R4	K20	R13
      0xB8360200,  //  0057  GETNGBL	R13	K1
      0x8C341B1F,  //  0058  GETMET	R13	R13	K31
      0x7C340200,  //  0059  CALL	R13	1
      0x50380200,  //  005A  LDBOOL	R14	1	0
      0x1C38160E,  //  005B  EQ	R14	R11	R14
      0x743A0004,  //  005C  JMPT	R14	#0062
      0x8838090D,  //  005D  GETMBR	R14	R4	K13
      0xB83E0200,  //  005E  GETNGBL	R15	K1
      0x883C1F20,  //  005F  GETMBR	R15	R15	K32
      0x1C381C0F,  //  0060  EQ	R14	R14	R15
      0x783A002D,  //  0061  JMPF	R14	#0090
      0xB83A0200,  //  0062  GETNGBL	R14	K1
      0x8C381D21,  //  0063  GETMET	R14	R14	K33
      0x7C380200,  //  0064  CALL	R14	1
      0x90361A0E,  //  0065  SETMBR	R13	K13	R14
      0x88381B0D,  //  0066  GETMBR	R14	R13	K13
      0xB83E0200,  //  0067  GETNGBL	R15	K1
      0x8C3C1F22,  //  0068  GETMET	R15	R15	K34
      0x7C3C0200,  //  0069  CALL	R15	1
      0x903A140F,  //  006A  SETMBR	R14	K10	R15
      0x88381B0D,  //  006B  GETMBR	R14	R13	K13
      0x88381D0A,  //  006C  GETMBR	R14	R14	K10
      0x883C0909,  //  006D  GETMBR	R15	R4	K9
      0x903A120F,  //  006E  SETMBR	R14	K9	R15
      0x88381B0D,  //  006F  GETMBR	R14	R13	K13
      0x88381D0A,  //  0070  GETMBR	R14	R14	K10
      0x883C090B,  //  0071  GETMBR	R15	R4	K11
      0x903A160F,  //  0072  SETMBR	R14	K11	R15
      0x88381B0D,  //  0073  GETMBR	R14	R13	K13
      0x88381D0A,  //  0074  GETMBR	R14	R14	K10
      0x883C090C,  //  0075  GETMBR	R15	R4	K12
      0x903A180F,  //  0076  SETMBR	R14	K12	R15
      0x88381B0D,  //  0077  GETMBR	R14	R13	K13
      0xB83E0200,  //  0078  GETNGBL	R15	K1
      0x8C3C1F23,  //  0079  GETMET	R15	R15	K35
      0x7C3C0200,  //  007A  CALL	R15	1
      0x903A1A0F,  //  007B  SETMBR	R14	K13	R15
      0x88381B0D,  //  007C  GETMBR	R14	R13	K13
      0x88381D0D,  //  007D  GETMBR	R14	R14	K13
      0xB83E0200,  //  007E  GETNGBL	R15	K1
      0x883C1F20,  //  007F  GETMBR	R15	R15	K32
      0x903A1A0F,  //  0080  SETMBR	R14	K13	R15
      0x88380D08,  //  0081  GETMBR	R14	R6	K8
      0x8C381D24,  //  0082  GETMET	R14	R14	K36
      0x5C401A00,  //  0083  MOVE	R16	R13
      0x7C380400,  //  0084  CALL	R14	2
      0xB83A3000,  //  0085  GETNGBL	R14	K24
      0x8C381D14,  //  0086  GETMET	R14	R14	K20
      0x8C400719,  //  0087  GETMET	R16	R3	K25
      0x58480025,  //  0088  LDCONST	R18	K37
      0x884C0312,  //  0089  GETMBR	R19	R1	K18
      0x884C271B,  //  008A  GETMBR	R19	R19	K27
      0x88500326,  //  008B  GETMBR	R20	R1	K38
      0x7C400800,  //  008C  CALL	R16	4
      0x5844001E,  //  008D  LDCONST	R17	K30
      0x7C380600,  //  008E  CALL	R14	3
      0x70020070,  //  008F  JMP		#0101
      0x4C380000,  //  0090  LDNIL	R14
      0x2038160E,  //  0091  NE	R14	R11	R14
      0x783A0031,  //  0092  JMPF	R14	#00C5
      0xB83A0200,  //  0093  GETNGBL	R14	K1
      0x8C381D27,  //  0094  GETMET	R14	R14	K39
      0x7C380200,  //  0095  CALL	R14	1
      0x9036180E,  //  0096  SETMBR	R13	K12	R14
      0x88381B0C,  //  0097  GETMBR	R14	R13	K12
      0xB83E0200,  //  0098  GETNGBL	R15	K1
      0x8C3C1F22,  //  0099  GETMET	R15	R15	K34
      0x7C3C0200,  //  009A  CALL	R15	1
      0x903A140F,  //  009B  SETMBR	R14	K10	R15
      0x88381B0C,  //  009C  GETMBR	R14	R13	K12
      0x88381D0A,  //  009D  GETMBR	R14	R14	K10
      0x883C0909,  //  009E  GETMBR	R15	R4	K9
      0x903A120F,  //  009F  SETMBR	R14	K9	R15
      0x88381B0C,  //  00A0  GETMBR	R14	R13	K12
      0x88381D0A,  //  00A1  GETMBR	R14	R14	K10
      0x883C090B,  //  00A2  GETMBR	R15	R4	K11
      0x903A160F,  //  00A3  SETMBR	R14	K11	R15
      0x88381B0C,  //  00A4  GETMBR	R14	R13	K12
      0x88381D0A,  //  00A5  GETMBR	R14	R14	K10
      0x883C090C,  //  00A6  GETMBR	R15	R4	K12
      0x903A180F,  //  00A7  SETMBR	R14	K12	R15
      0x88381B0C,  //  00A8  GETMBR	R14	R13	K12
      0x903A260B,  //  00A9  SETMBR	R14	K19	R11
      0x88380D08,  //  00AA  GETMBR	R14	R6	K8
      0x8C381D24,  //  00AB  GETMET	R14	R14	K36
      0x5C401A00,  //  00AC  MOVE	R16	R13
      0x7C380400,  //  00AD  CALL	R14	2
      0xB83A0200,  //  00AE  GETNGBL	R14	K1
      0x8C381D0F,  //  00AF  GETMET	R14	R14	K15
      0x8840090B,  //  00B0  GETMBR	R16	R4	K11
      0x8844090C,  //  00B1  GETMBR	R17	R4	K12
      0x7C380600,  //  00B2  CALL	R14	3
      0x5C241C00,  //  00B3  MOVE	R9	R14
      0xB83A3000,  //  00B4  GETNGBL	R14	K24
      0x8C381D14,  //  00B5  GETMET	R14	R14	K20
      0x8C400719,  //  00B6  GETMET	R16	R3	K25
      0x58480028,  //  00B7  LDCONST	R18	K40
      0x884C0312,  //  00B8  GETMBR	R19	R1	K18
      0x884C271B,  //  00B9  GETMBR	R19	R19	K27
      0x60500008,  //  00BA  GETGBL	R20	G8
      0x5C540800,  //  00BB  MOVE	R21	R4
      0x7C500200,  //  00BC  CALL	R20	1
      0x78260001,  //  00BD  JMPF	R9	#00C0
      0x5C541200,  //  00BE  MOVE	R21	R9
      0x70020000,  //  00BF  JMP		#00C1
      0x58540017,  //  00C0  LDCONST	R21	K23
      0x7C400A00,  //  00C1  CALL	R16	5
      0x5844001E,  //  00C2  LDCONST	R17	K30
      0x7C380600,  //  00C3  CALL	R14	3
      0x7002003B,  //  00C4  JMP		#0101
      0x8838090D,  //  00C5  GETMBR	R14	R4	K13
      0x4C3C0000,  //  00C6  LDNIL	R15
      0x20381C0F,  //  00C7  NE	R14	R14	R15
      0x783A002D,  //  00C8  JMPF	R14	#00F7
      0xB83A0200,  //  00C9  GETNGBL	R14	K1
      0x8C381D21,  //  00CA  GETMET	R14	R14	K33
      0x7C380200,  //  00CB  CALL	R14	1
      0x90361A0E,  //  00CC  SETMBR	R13	K13	R14
      0x88381B0D,  //  00CD  GETMBR	R14	R13	K13
      0xB83E0200,  //  00CE  GETNGBL	R15	K1
      0x8C3C1F22,  //  00CF  GETMET	R15	R15	K34
      0x7C3C0200,  //  00D0  CALL	R15	1
      0x903A140F,  //  00D1  SETMBR	R14	K10	R15
      0x88381B0D,  //  00D2  GETMBR	R14	R13	K13
      0x88381D0A,  //  00D3  GETMBR	R14	R14	K10
      0x883C0909,  //  00D4  GETMBR	R15	R4	K9
      0x903A120F,  //  00D5  SETMBR	R14	K9	R15
      0x88381B0D,  //  00D6  GETMBR	R14	R13	K13
      0x88381D0A,  //  00D7  GETMBR	R14	R14	K10
      0x883C090B,  //  00D8  GETMBR	R15	R4	K11
      0x903A160F,  //  00D9  SETMBR	R14	K11	R15
      0x88381B0D,  //  00DA  GETMBR	R14	R13	K13
      0x88381D0A,  //  00DB  GETMBR	R14	R14	K10
      0x883C090C,  //  00DC  GETMBR	R15	R4	K12
      0x903A180F,  //  00DD  SETMBR	R14	K12	R15
      0x88381B0D,  //  00DE  GETMBR	R14	R13	K13
      0xB83E0200,  //  00DF  GETNGBL	R15	K1
      0x8C3C1F23,  //  00E0  GETMET	R15	R15	K35
      0x7C3C0200,  //  00E1  CALL	R15	1
      0x903A1A0F,  //  00E2  SETMBR	R14	K13	R15
      0x88381B0D,  //  00E3  GETMBR	R14	R13	K13
      0x88381D0D,  //  00E4  GETMBR	R14	R14	K13
      0x883C090D,  //  00E5  GETMBR	R15	R4	K13
      0x903A1A0F,  //  00E6  SETMBR	R14	K13	R15
      0x88380D08,  //  00E7  GETMBR	R14	R6	K8
      0x8C381D24,  //  00E8  GETMET	R14	R14	K36
      0x5C401A00,  //  00E9  MOVE	R16	R13
      0x7C380400,  //  00EA  CALL	R14	2
      0xB83A3000,  //  00EB  GETNGBL	R14	K24
      0x8C381D14,  //  00EC  GETMET	R14	R14	K20
      0x8C400719,  //  00ED  GETMET	R16	R3	K25
      0x58480029,  //  00EE  LDCONST	R18	K41
      0x884C0312,  //  00EF  GETMBR	R19	R1	K18
      0x884C271B,  //  00F0  GETMBR	R19	R19	K27
      0x8850090D,  //  00F1  GETMBR	R20	R4	K13
      0x88540326,  //  00F2  GETMBR	R21	R1	K38
      0x7C400A00,  //  00F3  CALL	R16	5
      0x5844001E,  //  00F4  LDCONST	R17	K30
      0x7C380600,  //  00F5  CALL	R14	3
      0x70020009,  //  00F6  JMP		#0101
      0xB83A3000,  //  00F7  GETNGBL	R14	K24
      0x8C381D14,  //  00F8  GETMET	R14	R14	K20
      0x8C400719,  //  00F9  GETMET	R16	R3	K25
      0x5848002A,  //  00FA  LDCONST	R18	K42
      0x884C0312,  //  00FB  GETMBR	R19	R1	K18
      0x884C271B,  //  00FC  GETMBR	R19	R19	K27
      0x88500326,  //  00FD  GETMBR	R20	R1	K38
      0x7C400800,  //  00FE  CALL	R16	4
      0x5844001E,  //  00FF  LDCONST	R17	K30
      0x7C380600,  //  0100  CALL	R14	3
      0x7001FF17,  //  0101  JMP		#001A
      0x581C002B,  //  0102  LDCONST	R7	K43
      0xAC1C0200,  //  0103  CATCH	R7	1	0
      0xB0080000,  //  0104  RAISE	2	R0	R0
      0x601C000C,  //  0105  GETGBL	R7	G12
      0x88200D08,  //  0106  GETMBR	R8	R6	K8
      0x7C1C0200,  //  0107  CALL	R7	1
      0x241C0F1C,  //  0108  GT	R7	R7	K28
      0x781E0004,  //  0109  JMPF	R7	#010F
      0x8C1C012C,  //  010A  GETMET	R7	R0	K44
      0x5C240200,  //  010B  MOVE	R9	R1
      0x5C280C00,  //  010C  MOVE	R10	R6
      0x7C1C0600,  //  010D  CALL	R7	3
      0x70020001,  //  010E  JMP		#0111
      0x501C0000,  //  010F  LDBOOL	R7	0	0
      0x80040E00,  //  0110  RET	1	R7
      0x501C0200,  //  0111  LDBOOL	R7	1	0
      0x80040E00,  //  0112  RET	1	R7
      0x80000000,  //  0113  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_request
********************************************************************/
be_local_closure(Matter_IM_subscribe_request,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[31]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(SubscribeRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(keep_subscriptions),
    /* K5   */  be_nested_str_weak(subs_shop),
    /* K6   */  be_nested_str_weak(remove_by_session),
    /* K7   */  be_nested_str_weak(session),
    /* K8   */  be_nested_str_weak(new_subscription),
    /* K9   */  be_nested_str_weak(Path),
    /* K10  */  be_nested_str_weak(attributes_requests),
    /* K11  */  be_nested_str_weak(endpoint),
    /* K12  */  be_nested_str_weak(cluster),
    /* K13  */  be_nested_str_weak(attribute),
    /* K14  */  be_nested_str_weak(push),
    /* K15  */  be_nested_str_weak(stop_iteration),
    /* K16  */  be_nested_str_weak(tasmota),
    /* K17  */  be_nested_str_weak(log),
    /* K18  */  be_nested_str_weak(format),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20_X3ESubscribe_X20_X28_X256i_X29_X20_X25s_X20_X28min_X3D_X25i_X2C_X20max_X3D_X25i_X2C_X20keep_X3D_X25i_X29_X20sub_X3D_X25i),
    /* K20  */  be_nested_str_weak(local_session_id),
    /* K21  */  be_nested_str_weak(concat),
    /* K22  */  be_nested_str_weak(_X20),
    /* K23  */  be_nested_str_weak(min_interval),
    /* K24  */  be_nested_str_weak(max_interval),
    /* K25  */  be_const_int(1),
    /* K26  */  be_const_int(0),
    /* K27  */  be_nested_str_weak(subscription_id),
    /* K28  */  be_const_int(3),
    /* K29  */  be_nested_str_weak(_inner_process_read_request),
    /* K30  */  be_nested_str_weak(send_subscribe_response),
    }),
    be_str_weak(subscribe_request),
    &be_const_str_solidified,
    ( &(const binstruction[78]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x88140904,  //  0007  GETMBR	R5	R4	K4
      0x74160003,  //  0008  JMPT	R5	#000D
      0x88140105,  //  0009  GETMBR	R5	R0	K5
      0x8C140B06,  //  000A  GETMET	R5	R5	K6
      0x881C0307,  //  000B  GETMBR	R7	R1	K7
      0x7C140400,  //  000C  CALL	R5	2
      0x88140105,  //  000D  GETMBR	R5	R0	K5
      0x8C140B08,  //  000E  GETMET	R5	R5	K8
      0x881C0307,  //  000F  GETMBR	R7	R1	K7
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x60180012,  //  0012  GETGBL	R6	G18
      0x7C180000,  //  0013  CALL	R6	0
      0xB81E0200,  //  0014  GETNGBL	R7	K1
      0x8C1C0F09,  //  0015  GETMET	R7	R7	K9
      0x7C1C0200,  //  0016  CALL	R7	1
      0x60200010,  //  0017  GETGBL	R8	G16
      0x8824090A,  //  0018  GETMBR	R9	R4	K10
      0x7C200200,  //  0019  CALL	R8	1
      0xA802000D,  //  001A  EXBLK	0	#0029
      0x5C241000,  //  001B  MOVE	R9	R8
      0x7C240000,  //  001C  CALL	R9	0
      0x8828130B,  //  001D  GETMBR	R10	R9	K11
      0x901E160A,  //  001E  SETMBR	R7	K11	R10
      0x8828130C,  //  001F  GETMBR	R10	R9	K12
      0x901E180A,  //  0020  SETMBR	R7	K12	R10
      0x8828130D,  //  0021  GETMBR	R10	R9	K13
      0x901E1A0A,  //  0022  SETMBR	R7	K13	R10
      0x8C280D0E,  //  0023  GETMET	R10	R6	K14
      0x60300008,  //  0024  GETGBL	R12	G8
      0x5C340E00,  //  0025  MOVE	R13	R7
      0x7C300200,  //  0026  CALL	R12	1
      0x7C280400,  //  0027  CALL	R10	2
      0x7001FFF1,  //  0028  JMP		#001B
      0x5820000F,  //  0029  LDCONST	R8	K15
      0xAC200200,  //  002A  CATCH	R8	1	0
      0xB0080000,  //  002B  RAISE	2	R0	R0
      0xB8222000,  //  002C  GETNGBL	R8	K16
      0x8C201111,  //  002D  GETMET	R8	R8	K17
      0x8C280712,  //  002E  GETMET	R10	R3	K18
      0x58300013,  //  002F  LDCONST	R12	K19
      0x88340307,  //  0030  GETMBR	R13	R1	K7
      0x88341B14,  //  0031  GETMBR	R13	R13	K20
      0x8C380D15,  //  0032  GETMET	R14	R6	K21
      0x58400016,  //  0033  LDCONST	R16	K22
      0x7C380400,  //  0034  CALL	R14	2
      0x883C0B17,  //  0035  GETMBR	R15	R5	K23
      0x88400B18,  //  0036  GETMBR	R16	R5	K24
      0x88440904,  //  0037  GETMBR	R17	R4	K4
      0x78460001,  //  0038  JMPF	R17	#003B
      0x58440019,  //  0039  LDCONST	R17	K25
      0x70020000,  //  003A  JMP		#003C
      0x5844001A,  //  003B  LDCONST	R17	K26
      0x88480B1B,  //  003C  GETMBR	R18	R5	K27
      0x7C281000,  //  003D  CALL	R10	8
      0x582C001C,  //  003E  LDCONST	R11	K28
      0x7C200600,  //  003F  CALL	R8	3
      0x8C20011D,  //  0040  GETMET	R8	R0	K29
      0x88280307,  //  0041  GETMBR	R10	R1	K7
      0x5C2C0800,  //  0042  MOVE	R11	R4
      0x50300200,  //  0043  LDBOOL	R12	1	0
      0x7C200800,  //  0044  CALL	R8	4
      0x88240B1B,  //  0045  GETMBR	R9	R5	K27
      0x90223609,  //  0046  SETMBR	R8	K27	R9
      0x8C24011E,  //  0047  GETMET	R9	R0	K30
      0x5C2C0200,  //  0048  MOVE	R11	R1
      0x5C301000,  //  0049  MOVE	R12	R8
      0x5C340A00,  //  004A  MOVE	R13	R5
      0x7C240800,  //  004B  CALL	R9	4
      0x50240200,  //  004C  LDBOOL	R9	1	0
      0x80041200,  //  004D  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_request
********************************************************************/
be_local_closure(Matter_IM_process_write_request,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        19,                          /* nstack */
        5,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[28]) {     /* constants */
        /* K0   */  be_nested_str_weak(string),
        /* K1   */  be_nested_str_weak(matter),
        /* K2   */  be_nested_str_weak(get_attribute_name),
        /* K3   */  be_nested_str_weak(cluster),
        /* K4   */  be_nested_str_weak(attribute),
        /* K5   */  be_nested_str_weak(_X20_X28),
        /* K6   */  be_nested_str_weak(_X29),
        /* K7   */  be_nested_str_weak(),
        /* K8   */  be_nested_str_weak(status),
        /* K9   */  be_nested_str_weak(UNSUPPORTED_WRITE),
        /* K10  */  be_nested_str_weak(write_attribute),
        /* K11  */  be_nested_str_weak(session),
        /* K12  */  be_nested_str_weak(SUCCESS),
        /* K13  */  be_nested_str_weak(AttributeStatusIB),
        /* K14  */  be_nested_str_weak(path),
        /* K15  */  be_nested_str_weak(AttributePathIB),
        /* K16  */  be_nested_str_weak(StatusIB),
        /* K17  */  be_nested_str_weak(endpoint),
        /* K18  */  be_nested_str_weak(write_responses),
        /* K19  */  be_nested_str_weak(push),
        /* K20  */  be_nested_str_weak(tasmota),
        /* K21  */  be_nested_str_weak(log),
        /* K22  */  be_nested_str_weak(format),
        /* K23  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K24  */  be_const_int(0),
        /* K25  */  be_const_int(2),
        /* K26  */  be_const_int(3),
        /* K27  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20IGNORED),
        }),
        be_str_weak(write_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[102]) {  /* code */
          0xA4160000,  //  0000  IMPORT	R5	K0
          0xB81A0200,  //  0001  GETNGBL	R6	K1
          0x8C180D02,  //  0002  GETMET	R6	R6	K2
          0x88200503,  //  0003  GETMBR	R8	R2	K3
          0x88240504,  //  0004  GETMBR	R9	R2	K4
          0x7C180600,  //  0005  CALL	R6	3
          0x781A0002,  //  0006  JMPF	R6	#000A
          0x001E0A06,  //  0007  ADD	R7	K5	R6
          0x001C0F06,  //  0008  ADD	R7	R7	K6
          0x70020000,  //  0009  JMP		#000B
          0x581C0007,  //  000A  LDCONST	R7	K7
          0x5C180E00,  //  000B  MOVE	R6	R7
          0xB81E0200,  //  000C  GETNGBL	R7	K1
          0x881C0F09,  //  000D  GETMBR	R7	R7	K9
          0x900A1007,  //  000E  SETMBR	R2	K8	R7
          0x4C1C0000,  //  000F  LDNIL	R7
          0x201C0207,  //  0010  NE	R7	R1	R7
          0x781E0006,  //  0011  JMPF	R7	#0019
          0x8C1C030A,  //  0012  GETMET	R7	R1	K10
          0x68240000,  //  0013  GETUPV	R9	U0
          0x8824130B,  //  0014  GETMBR	R9	R9	K11
          0x5C280400,  //  0015  MOVE	R10	R2
          0x5C2C0600,  //  0016  MOVE	R11	R3
          0x7C1C0800,  //  0017  CALL	R7	4
          0x70020000,  //  0018  JMP		#001A
          0x4C1C0000,  //  0019  LDNIL	R7
          0x781E0002,  //  001A  JMPF	R7	#001E
          0xB8220200,  //  001B  GETNGBL	R8	K1
          0x8820110C,  //  001C  GETMBR	R8	R8	K12
          0x900A1008,  //  001D  SETMBR	R2	K8	R8
          0x88200508,  //  001E  GETMBR	R8	R2	K8
          0x4C240000,  //  001F  LDNIL	R9
          0x20201009,  //  0020  NE	R8	R8	R9
          0x78220037,  //  0021  JMPF	R8	#005A
          0x78120035,  //  0022  JMPF	R4	#0059
          0xB8220200,  //  0023  GETNGBL	R8	K1
          0x8C20110D,  //  0024  GETMET	R8	R8	K13
          0x7C200200,  //  0025  CALL	R8	1
          0xB8260200,  //  0026  GETNGBL	R9	K1
          0x8C24130F,  //  0027  GETMET	R9	R9	K15
          0x7C240200,  //  0028  CALL	R9	1
          0x90221C09,  //  0029  SETMBR	R8	K14	R9
          0xB8260200,  //  002A  GETNGBL	R9	K1
          0x8C241310,  //  002B  GETMET	R9	R9	K16
          0x7C240200,  //  002C  CALL	R9	1
          0x90221009,  //  002D  SETMBR	R8	K8	R9
          0x8824110E,  //  002E  GETMBR	R9	R8	K14
          0x88280511,  //  002F  GETMBR	R10	R2	K17
          0x9026220A,  //  0030  SETMBR	R9	K17	R10
          0x8824110E,  //  0031  GETMBR	R9	R8	K14
          0x88280503,  //  0032  GETMBR	R10	R2	K3
          0x9026060A,  //  0033  SETMBR	R9	K3	R10
          0x8824110E,  //  0034  GETMBR	R9	R8	K14
          0x88280504,  //  0035  GETMBR	R10	R2	K4
          0x9026080A,  //  0036  SETMBR	R9	K4	R10
          0x88241108,  //  0037  GETMBR	R9	R8	K8
          0x88280508,  //  0038  GETMBR	R10	R2	K8
          0x9026100A,  //  0039  SETMBR	R9	K8	R10
          0x88240112,  //  003A  GETMBR	R9	R0	K18
          0x8C241313,  //  003B  GETMET	R9	R9	K19
          0x5C2C1000,  //  003C  MOVE	R11	R8
          0x7C240400,  //  003D  CALL	R9	2
          0xB8262800,  //  003E  GETNGBL	R9	K20
          0x8C241315,  //  003F  GETMET	R9	R9	K21
          0x8C2C0B16,  //  0040  GETMET	R11	R5	K22
          0x58340017,  //  0041  LDCONST	R13	K23
          0x60380008,  //  0042  GETGBL	R14	G8
          0x5C3C0400,  //  0043  MOVE	R15	R2
          0x7C380200,  //  0044  CALL	R14	1
          0x5C3C0C00,  //  0045  MOVE	R15	R6
          0x88400508,  //  0046  GETMBR	R16	R2	K8
          0x88440508,  //  0047  GETMBR	R17	R2	K8
          0xB84A0200,  //  0048  GETNGBL	R18	K1
          0x8848250C,  //  0049  GETMBR	R18	R18	K12
          0x1C442212,  //  004A  EQ	R17	R17	R18
          0x78460001,  //  004B  JMPF	R17	#004E
          0x5844000C,  //  004C  LDCONST	R17	K12
          0x70020000,  //  004D  JMP		#004F
          0x58440007,  //  004E  LDCONST	R17	K7
          0x7C2C0C00,  //  004F  CALL	R11	6
          0x88300511,  //  0050  GETMBR	R12	R2	K17
          0x20301918,  //  0051  NE	R12	R12	K24
          0x78320001,  //  0052  JMPF	R12	#0055
          0x58300019,  //  0053  LDCONST	R12	K25
          0x70020000,  //  0054  JMP		#0056
          0x5830001A,  //  0055  LDCONST	R12	K26
          0x7C240600,  //  0056  CALL	R9	3
          0x50240200,  //  0057  LDBOOL	R9	1	0
          0x80041200,  //  0058  RET	1	R9
          0x7002000A,  //  0059  JMP		#0065
          0xB8222800,  //  005A  GETNGBL	R8	K20
          0x8C201115,  //  005B  GETMET	R8	R8	K21
          0x8C280B16,  //  005C  GETMET	R10	R5	K22
          0x5830001B,  //  005D  LDCONST	R12	K27
          0x60340008,  //  005E  GETGBL	R13	G8
          0x5C380400,  //  005F  MOVE	R14	R2
          0x7C340200,  //  0060  CALL	R13	1
          0x5C380C00,  //  0061  MOVE	R14	R6
          0x7C280800,  //  0062  CALL	R10	4
          0x582C001A,  //  0063  LDCONST	R11	K26
          0x7C200600,  //  0064  CALL	R8	3
          0x80000000,  //  0065  RET	0
        })
      ),
      be_nested_proto(
        9,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 7),
          be_local_const_upval(1, 9),
          be_local_const_upval(1, 13),
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
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(WriteRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(suppress_response),
    /* K5   */  be_nested_str_weak(device),
    /* K6   */  be_nested_str_weak(get_active_endpoints),
    /* K7   */  be_nested_str_weak(Path),
    /* K8   */  be_nested_str_weak(write_requests),
    /* K9   */  be_nested_str_weak(WriteResponseMessage),
    /* K10  */  be_nested_str_weak(write_responses),
    /* K11  */  be_nested_str_weak(path),
    /* K12  */  be_nested_str_weak(data),
    /* K13  */  be_nested_str_weak(endpoint),
    /* K14  */  be_nested_str_weak(cluster),
    /* K15  */  be_nested_str_weak(attribute),
    /* K16  */  be_nested_str_weak(status),
    /* K17  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K18  */  be_nested_str_weak(INVALID_ACTION),
    /* K19  */  be_nested_str_weak(get_attribute_name),
    /* K20  */  be_nested_str_weak(tasmota),
    /* K21  */  be_nested_str_weak(log),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20),
    /* K23  */  be_nested_str_weak(_X20_X28),
    /* K24  */  be_nested_str_weak(_X29),
    /* K25  */  be_nested_str_weak(),
    /* K26  */  be_const_int(3),
    /* K27  */  be_nested_str_weak(process_attribute_expansion),
    /* K28  */  be_nested_str_weak(stop_iteration),
    /* K29  */  be_nested_str_weak(send_write_response),
    }),
    be_str_weak(process_write_request),
    &be_const_str_solidified,
    ( &(const binstruction[104]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x88140904,  //  0007  GETMBR	R5	R4	K4
      0x88180105,  //  0008  GETMBR	R6	R0	K5
      0x8C180D06,  //  0009  GETMET	R6	R6	K6
      0x7C180200,  //  000A  CALL	R6	1
      0x841C0000,  //  000B  CLOSURE	R7	P0
      0xB8220200,  //  000C  GETNGBL	R8	K1
      0x8C201107,  //  000D  GETMET	R8	R8	K7
      0x7C200200,  //  000E  CALL	R8	1
      0x88240908,  //  000F  GETMBR	R9	R4	K8
      0x4C280000,  //  0010  LDNIL	R10
      0x2024120A,  //  0011  NE	R9	R9	R10
      0x78260051,  //  0012  JMPF	R9	#0065
      0xB8260200,  //  0013  GETNGBL	R9	K1
      0x8C241309,  //  0014  GETMET	R9	R9	K9
      0x7C240200,  //  0015  CALL	R9	1
      0x60280012,  //  0016  GETGBL	R10	G18
      0x7C280000,  //  0017  CALL	R10	0
      0x9026140A,  //  0018  SETMBR	R9	K10	R10
      0x60280010,  //  0019  GETGBL	R10	G16
      0x882C0908,  //  001A  GETMBR	R11	R4	K8
      0x7C280200,  //  001B  CALL	R10	1
      0xA802003D,  //  001C  EXBLK	0	#005B
      0x5C2C1400,  //  001D  MOVE	R11	R10
      0x7C2C0000,  //  001E  CALL	R11	0
      0x8830170B,  //  001F  GETMBR	R12	R11	K11
      0x8834170C,  //  0020  GETMBR	R13	R11	K12
      0x8838190D,  //  0021  GETMBR	R14	R12	K13
      0x90221A0E,  //  0022  SETMBR	R8	K13	R14
      0x8838190E,  //  0023  GETMBR	R14	R12	K14
      0x90221C0E,  //  0024  SETMBR	R8	K14	R14
      0x8838190F,  //  0025  GETMBR	R14	R12	K15
      0x90221E0E,  //  0026  SETMBR	R8	K15	R14
      0xB83A0200,  //  0027  GETNGBL	R14	K1
      0x88381D11,  //  0028  GETMBR	R14	R14	K17
      0x9022200E,  //  0029  SETMBR	R8	K16	R14
      0x8838110E,  //  002A  GETMBR	R14	R8	K14
      0x4C3C0000,  //  002B  LDNIL	R15
      0x1C381C0F,  //  002C  EQ	R14	R14	R15
      0x743A0003,  //  002D  JMPT	R14	#0032
      0x8838110F,  //  002E  GETMBR	R14	R8	K15
      0x4C3C0000,  //  002F  LDNIL	R15
      0x1C381C0F,  //  0030  EQ	R14	R14	R15
      0x783A000A,  //  0031  JMPF	R14	#003D
      0xB83A0200,  //  0032  GETNGBL	R14	K1
      0x88381D12,  //  0033  GETMBR	R14	R14	K18
      0x9022200E,  //  0034  SETMBR	R8	K16	R14
      0x5C380E00,  //  0035  MOVE	R14	R7
      0x5C3C1200,  //  0036  MOVE	R15	R9
      0x4C400000,  //  0037  LDNIL	R16
      0x5C441000,  //  0038  MOVE	R17	R8
      0x4C480000,  //  0039  LDNIL	R18
      0x504C0200,  //  003A  LDBOOL	R19	1	0
      0x7C380A00,  //  003B  CALL	R14	5
      0x7001FFDF,  //  003C  JMP		#001D
      0x8838110D,  //  003D  GETMBR	R14	R8	K13
      0x4C3C0000,  //  003E  LDNIL	R15
      0x1C381C0F,  //  003F  EQ	R14	R14	R15
      0x783A0012,  //  0040  JMPF	R14	#0054
      0xB83A0200,  //  0041  GETNGBL	R14	K1
      0x8C381D13,  //  0042  GETMET	R14	R14	K19
      0x8840110E,  //  0043  GETMBR	R16	R8	K14
      0x8844110F,  //  0044  GETMBR	R17	R8	K15
      0x7C380600,  //  0045  CALL	R14	3
      0xB83E2800,  //  0046  GETNGBL	R15	K20
      0x8C3C1F15,  //  0047  GETMET	R15	R15	K21
      0x60440008,  //  0048  GETGBL	R17	G8
      0x5C481000,  //  0049  MOVE	R18	R8
      0x7C440200,  //  004A  CALL	R17	1
      0x00462C11,  //  004B  ADD	R17	K22	R17
      0x783A0002,  //  004C  JMPF	R14	#0050
      0x004A2E0E,  //  004D  ADD	R18	K23	R14
      0x00482518,  //  004E  ADD	R18	R18	K24
      0x70020000,  //  004F  JMP		#0051
      0x58480019,  //  0050  LDCONST	R18	K25
      0x00442212,  //  0051  ADD	R17	R17	R18
      0x5848001A,  //  0052  LDCONST	R18	K26
      0x7C3C0600,  //  0053  CALL	R15	3
      0x88380105,  //  0054  GETMBR	R14	R0	K5
      0x8C381D1B,  //  0055  GETMET	R14	R14	K27
      0x5C401000,  //  0056  MOVE	R16	R8
      0x84440001,  //  0057  CLOSURE	R17	P1
      0x7C380600,  //  0058  CALL	R14	3
      0xA0280000,  //  0059  CLOSE	R10
      0x7001FFC1,  //  005A  JMP		#001D
      0x5828001C,  //  005B  LDCONST	R10	K28
      0xAC280200,  //  005C  CATCH	R10	1	0
      0xB0080000,  //  005D  RAISE	2	R0	R0
      0x5C280A00,  //  005E  MOVE	R10	R5
      0x742A0003,  //  005F  JMPT	R10	#0064
      0x8C28011D,  //  0060  GETMET	R10	R0	K29
      0x5C300200,  //  0061  MOVE	R12	R1
      0x5C341200,  //  0062  MOVE	R13	R9
      0x7C280600,  //  0063  CALL	R10	3
      0xA0240000,  //  0064  CLOSE	R9
      0x50240200,  //  0065  LDBOOL	R9	1	0
      0xA0000000,  //  0066  CLOSE	R0
      0x80041200,  //  0067  RET	1	R9
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
    ( &(const binstruction[ 7]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100200,  //  0003  MOVE	R4	R1
      0x50140000,  //  0004  LDBOOL	R5	0	0
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
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
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(SubscribeResponseMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(subscribe_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x50140000,  //  0007  LDBOOL	R5	0	0
      0x80040A00,  //  0008  RET	1	R5
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
    ( &(const binstruction[110]) {  /* code */
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
      0x70020059,  //  0011  JMP		#006C
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
      0x7002004E,  //  001C  JMP		#006C
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
      0x70020043,  //  0027  JMP		#006C
      0x54160003,  //  0028  LDINT	R5	4
      0x1C140805,  //  0029  EQ	R5	R4	R5
      0x78160005,  //  002A  JMPF	R5	#0031
      0x8C14010E,  //  002B  GETMET	R5	R0	K14
      0x5C1C0200,  //  002C  MOVE	R7	R1
      0x5C200400,  //  002D  MOVE	R8	R2
      0x7C140600,  //  002E  CALL	R5	3
      0x80040A00,  //  002F  RET	1	R5
      0x7002003A,  //  0030  JMP		#006C
      0x54160004,  //  0031  LDINT	R5	5
      0x1C140805,  //  0032  EQ	R5	R4	R5
      0x78160005,  //  0033  JMPF	R5	#003A
      0x8C14010F,  //  0034  GETMET	R5	R0	K15
      0x5C1C0200,  //  0035  MOVE	R7	R1
      0x5C200400,  //  0036  MOVE	R8	R2
      0x7C140600,  //  0037  CALL	R5	3
      0x80040A00,  //  0038  RET	1	R5
      0x70020031,  //  0039  JMP		#006C
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
      0x70020025,  //  0045  JMP		#006C
      0x54160006,  //  0046  LDINT	R5	7
      0x1C140805,  //  0047  EQ	R5	R4	R5
      0x78160005,  //  0048  JMPF	R5	#004F
      0x8C140111,  //  0049  GETMET	R5	R0	K17
      0x5C1C0200,  //  004A  MOVE	R7	R1
      0x5C200400,  //  004B  MOVE	R8	R2
      0x7C140600,  //  004C  CALL	R5	3
      0x80040A00,  //  004D  RET	1	R5
      0x7002001C,  //  004E  JMP		#006C
      0x54160007,  //  004F  LDINT	R5	8
      0x1C140805,  //  0050  EQ	R5	R4	R5
      0x78160008,  //  0051  JMPF	R5	#005B
      0x8C14010A,  //  0052  GETMET	R5	R0	K10
      0x5C1C0200,  //  0053  MOVE	R7	R1
      0x7C140400,  //  0054  CALL	R5	2
      0x8C140112,  //  0055  GETMET	R5	R0	K18
      0x5C1C0200,  //  0056  MOVE	R7	R1
      0x5C200400,  //  0057  MOVE	R8	R2
      0x7C140600,  //  0058  CALL	R5	3
      0x80040A00,  //  0059  RET	1	R5
      0x70020010,  //  005A  JMP		#006C
      0x54160008,  //  005B  LDINT	R5	9
      0x1C140805,  //  005C  EQ	R5	R4	R5
      0x78160005,  //  005D  JMPF	R5	#0064
      0x8C140113,  //  005E  GETMET	R5	R0	K19
      0x5C1C0200,  //  005F  MOVE	R7	R1
      0x5C200400,  //  0060  MOVE	R8	R2
      0x7C140600,  //  0061  CALL	R5	3
      0x80040A00,  //  0062  RET	1	R5
      0x70020007,  //  0063  JMP		#006C
      0x54160009,  //  0064  LDINT	R5	10
      0x1C140805,  //  0065  EQ	R5	R4	R5
      0x78160004,  //  0066  JMPF	R5	#006C
      0x8C140114,  //  0067  GETMET	R5	R0	K20
      0x5C1C0200,  //  0068  MOVE	R7	R1
      0x5C200400,  //  0069  MOVE	R8	R2
      0x7C140600,  //  006A  CALL	R5	3
      0x80040A00,  //  006B  RET	1	R5
      0x50140000,  //  006C  LDBOOL	R5	0	0
      0x80040A00,  //  006D  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _inner_process_read_request
********************************************************************/
be_local_closure(Matter_IM__inner_process_read_request,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        25,                          /* nstack */
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
        ( &(const bvalue[40]) {     /* constants */
        /* K0   */  be_nested_str_weak(string),
        /* K1   */  be_nested_str_weak(matter),
        /* K2   */  be_nested_str_weak(TLV),
        /* K3   */  be_nested_str_weak(get_attribute_name),
        /* K4   */  be_nested_str_weak(cluster),
        /* K5   */  be_nested_str_weak(attribute),
        /* K6   */  be_nested_str_weak(_X20_X28),
        /* K7   */  be_nested_str_weak(_X29),
        /* K8   */  be_nested_str_weak(),
        /* K9   */  be_nested_str_weak(read_attribute),
        /* K10  */  be_nested_str_weak(AttributeReportIB),
        /* K11  */  be_nested_str_weak(attribute_data),
        /* K12  */  be_nested_str_weak(AttributeDataIB),
        /* K13  */  be_nested_str_weak(data_version),
        /* K14  */  be_const_int(1),
        /* K15  */  be_nested_str_weak(path),
        /* K16  */  be_nested_str_weak(AttributePathIB),
        /* K17  */  be_nested_str_weak(endpoint),
        /* K18  */  be_nested_str_weak(data),
        /* K19  */  be_nested_str_weak(to_TLV),
        /* K20  */  be_nested_str_weak(encode_len),
        /* K21  */  be_nested_str_weak(tlv2raw),
        /* K22  */  be_nested_str_weak(tasmota),
        /* K23  */  be_nested_str_weak(log),
        /* K24  */  be_nested_str_weak(format),
        /* K25  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
        /* K26  */  be_nested_str_weak(local_session_id),
        /* K27  */  be_const_int(3),
        /* K28  */  be_nested_str_weak(status),
        /* K29  */  be_nested_str_weak(attribute_status),
        /* K30  */  be_nested_str_weak(AttributeStatusIB),
        /* K31  */  be_nested_str_weak(StatusIB),
        /* K32  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K33  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
        /* K34  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20IGNORED),
        /* K35  */  be_nested_str_weak(attribute_reports),
        /* K36  */  be_const_int(0),
        /* K37  */  be_nested_str_weak(push),
        /* K38  */  be_nested_str_weak(IM_ReportData),
        /* K39  */  be_nested_str_weak(MAX_MESSAGE),
        }),
        be_str_weak(read_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[206]) {  /* code */
          0xA4120000,  //  0000  IMPORT	R4	K0
          0xB8160200,  //  0001  GETNGBL	R5	K1
          0x88140B02,  //  0002  GETMBR	R5	R5	K2
          0xB81A0200,  //  0003  GETNGBL	R6	K1
          0x8C180D03,  //  0004  GETMET	R6	R6	K3
          0x88200504,  //  0005  GETMBR	R8	R2	K4
          0x88240505,  //  0006  GETMBR	R9	R2	K5
          0x7C180600,  //  0007  CALL	R6	3
          0x781A0002,  //  0008  JMPF	R6	#000C
          0x001E0C06,  //  0009  ADD	R7	K6	R6
          0x001C0F07,  //  000A  ADD	R7	R7	K7
          0x70020000,  //  000B  JMP		#000D
          0x581C0008,  //  000C  LDCONST	R7	K8
          0x5C180E00,  //  000D  MOVE	R6	R7
          0x4C1C0000,  //  000E  LDNIL	R7
          0x201C0207,  //  000F  NE	R7	R1	R7
          0x781E0004,  //  0010  JMPF	R7	#0016
          0x8C1C0309,  //  0011  GETMET	R7	R1	K9
          0x68240000,  //  0012  GETUPV	R9	U0
          0x5C280400,  //  0013  MOVE	R10	R2
          0x7C1C0600,  //  0014  CALL	R7	3
          0x70020000,  //  0015  JMP		#0017
          0x4C1C0000,  //  0016  LDNIL	R7
          0x50200200,  //  0017  LDBOOL	R8	1	0
          0x4C240000,  //  0018  LDNIL	R9
          0x4C280000,  //  0019  LDNIL	R10
          0x20280E0A,  //  001A  NE	R10	R7	R10
          0x782A003A,  //  001B  JMPF	R10	#0057
          0x60280008,  //  001C  GETGBL	R10	G8
          0x5C2C0E00,  //  001D  MOVE	R11	R7
          0x7C280200,  //  001E  CALL	R10	1
          0xB82E0200,  //  001F  GETNGBL	R11	K1
          0x8C2C170A,  //  0020  GETMET	R11	R11	K10
          0x7C2C0200,  //  0021  CALL	R11	1
          0xB8320200,  //  0022  GETNGBL	R12	K1
          0x8C30190C,  //  0023  GETMET	R12	R12	K12
          0x7C300200,  //  0024  CALL	R12	1
          0x902E160C,  //  0025  SETMBR	R11	K11	R12
          0x8830170B,  //  0026  GETMBR	R12	R11	K11
          0x90321B0E,  //  0027  SETMBR	R12	K13	K14
          0x8830170B,  //  0028  GETMBR	R12	R11	K11
          0xB8360200,  //  0029  GETNGBL	R13	K1
          0x8C341B10,  //  002A  GETMET	R13	R13	K16
          0x7C340200,  //  002B  CALL	R13	1
          0x90321E0D,  //  002C  SETMBR	R12	K15	R13
          0x8830170B,  //  002D  GETMBR	R12	R11	K11
          0x8830190F,  //  002E  GETMBR	R12	R12	K15
          0x88340511,  //  002F  GETMBR	R13	R2	K17
          0x9032220D,  //  0030  SETMBR	R12	K17	R13
          0x8830170B,  //  0031  GETMBR	R12	R11	K11
          0x8830190F,  //  0032  GETMBR	R12	R12	K15
          0x88340504,  //  0033  GETMBR	R13	R2	K4
          0x9032080D,  //  0034  SETMBR	R12	K4	R13
          0x8830170B,  //  0035  GETMBR	R12	R11	K11
          0x8830190F,  //  0036  GETMBR	R12	R12	K15
          0x88340505,  //  0037  GETMBR	R13	R2	K5
          0x90320A0D,  //  0038  SETMBR	R12	K5	R13
          0x8830170B,  //  0039  GETMBR	R12	R11	K11
          0x90322407,  //  003A  SETMBR	R12	K18	R7
          0x8C301713,  //  003B  GETMET	R12	R11	K19
          0x7C300200,  //  003C  CALL	R12	1
          0x8C341914,  //  003D  GETMET	R13	R12	K20
          0x7C340200,  //  003E  CALL	R13	1
          0x60380015,  //  003F  GETGBL	R14	G21
          0x5C3C1A00,  //  0040  MOVE	R15	R13
          0x7C380200,  //  0041  CALL	R14	1
          0x8C3C1915,  //  0042  GETMET	R15	R12	K21
          0x5C441C00,  //  0043  MOVE	R17	R14
          0x7C3C0400,  //  0044  CALL	R15	2
          0x5C241E00,  //  0045  MOVE	R9	R15
          0x683C0001,  //  0046  GETUPV	R15	U1
          0x743E000D,  //  0047  JMPT	R15	#0056
          0xB83E2C00,  //  0048  GETNGBL	R15	K22
          0x8C3C1F17,  //  0049  GETMET	R15	R15	K23
          0x8C440918,  //  004A  GETMET	R17	R4	K24
          0x584C0019,  //  004B  LDCONST	R19	K25
          0x68500000,  //  004C  GETUPV	R20	U0
          0x8850291A,  //  004D  GETMBR	R20	R20	K26
          0x60540008,  //  004E  GETGBL	R21	G8
          0x5C580400,  //  004F  MOVE	R22	R2
          0x7C540200,  //  0050  CALL	R21	1
          0x5C580C00,  //  0051  MOVE	R22	R6
          0x5C5C1400,  //  0052  MOVE	R23	R10
          0x7C440C00,  //  0053  CALL	R17	6
          0x5848001B,  //  0054  LDCONST	R18	K27
          0x7C3C0600,  //  0055  CALL	R15	3
          0x70020055,  //  0056  JMP		#00AD
          0x8828051C,  //  0057  GETMBR	R10	R2	K28
          0x4C2C0000,  //  0058  LDNIL	R11
          0x2028140B,  //  0059  NE	R10	R10	R11
          0x782A0043,  //  005A  JMPF	R10	#009F
          0x780E0041,  //  005B  JMPF	R3	#009E
          0xB82A0200,  //  005C  GETNGBL	R10	K1
          0x8C28150A,  //  005D  GETMET	R10	R10	K10
          0x7C280200,  //  005E  CALL	R10	1
          0xB82E0200,  //  005F  GETNGBL	R11	K1
          0x8C2C171E,  //  0060  GETMET	R11	R11	K30
          0x7C2C0200,  //  0061  CALL	R11	1
          0x902A3A0B,  //  0062  SETMBR	R10	K29	R11
          0x882C151D,  //  0063  GETMBR	R11	R10	K29
          0xB8320200,  //  0064  GETNGBL	R12	K1
          0x8C301910,  //  0065  GETMET	R12	R12	K16
          0x7C300200,  //  0066  CALL	R12	1
          0x902E1E0C,  //  0067  SETMBR	R11	K15	R12
          0x882C151D,  //  0068  GETMBR	R11	R10	K29
          0xB8320200,  //  0069  GETNGBL	R12	K1
          0x8C30191F,  //  006A  GETMET	R12	R12	K31
          0x7C300200,  //  006B  CALL	R12	1
          0x902E380C,  //  006C  SETMBR	R11	K28	R12
          0x882C151D,  //  006D  GETMBR	R11	R10	K29
          0x882C170F,  //  006E  GETMBR	R11	R11	K15
          0x88300511,  //  006F  GETMBR	R12	R2	K17
          0x902E220C,  //  0070  SETMBR	R11	K17	R12
          0x882C151D,  //  0071  GETMBR	R11	R10	K29
          0x882C170F,  //  0072  GETMBR	R11	R11	K15
          0x88300504,  //  0073  GETMBR	R12	R2	K4
          0x902E080C,  //  0074  SETMBR	R11	K4	R12
          0x882C151D,  //  0075  GETMBR	R11	R10	K29
          0x882C170F,  //  0076  GETMBR	R11	R11	K15
          0x88300505,  //  0077  GETMBR	R12	R2	K5
          0x902E0A0C,  //  0078  SETMBR	R11	K5	R12
          0x882C151D,  //  0079  GETMBR	R11	R10	K29
          0x882C171C,  //  007A  GETMBR	R11	R11	K28
          0x8830051C,  //  007B  GETMBR	R12	R2	K28
          0x902E380C,  //  007C  SETMBR	R11	K28	R12
          0x8C2C1513,  //  007D  GETMET	R11	R10	K19
          0x7C2C0200,  //  007E  CALL	R11	1
          0x8C301714,  //  007F  GETMET	R12	R11	K20
          0x7C300200,  //  0080  CALL	R12	1
          0x60340015,  //  0081  GETGBL	R13	G21
          0x5C381800,  //  0082  MOVE	R14	R12
          0x7C340200,  //  0083  CALL	R13	1
          0x8C381715,  //  0084  GETMET	R14	R11	K21
          0x5C401A00,  //  0085  MOVE	R16	R13
          0x7C380400,  //  0086  CALL	R14	2
          0x5C241C00,  //  0087  MOVE	R9	R14
          0xB83A2C00,  //  0088  GETNGBL	R14	K22
          0x8C381D17,  //  0089  GETMET	R14	R14	K23
          0x8C400918,  //  008A  GETMET	R16	R4	K24
          0x58480020,  //  008B  LDCONST	R18	K32
          0x684C0000,  //  008C  GETUPV	R19	U0
          0x884C271A,  //  008D  GETMBR	R19	R19	K26
          0x60500008,  //  008E  GETGBL	R20	G8
          0x5C540400,  //  008F  MOVE	R21	R2
          0x7C500200,  //  0090  CALL	R20	1
          0x5C540C00,  //  0091  MOVE	R21	R6
          0x8858051C,  //  0092  GETMBR	R22	R2	K28
          0x885C051C,  //  0093  GETMBR	R23	R2	K28
          0xB8620200,  //  0094  GETNGBL	R24	K1
          0x88603121,  //  0095  GETMBR	R24	R24	K33
          0x1C5C2E18,  //  0096  EQ	R23	R23	R24
          0x785E0001,  //  0097  JMPF	R23	#009A
          0x585C0021,  //  0098  LDCONST	R23	K33
          0x70020000,  //  0099  JMP		#009B
          0x585C0008,  //  009A  LDCONST	R23	K8
          0x7C400E00,  //  009B  CALL	R16	7
          0x5844001B,  //  009C  LDCONST	R17	K27
          0x7C380600,  //  009D  CALL	R14	3
          0x7002000D,  //  009E  JMP		#00AD
          0xB82A2C00,  //  009F  GETNGBL	R10	K22
          0x8C281517,  //  00A0  GETMET	R10	R10	K23
          0x8C300918,  //  00A1  GETMET	R12	R4	K24
          0x58380022,  //  00A2  LDCONST	R14	K34
          0x683C0000,  //  00A3  GETUPV	R15	U0
          0x883C1F1A,  //  00A4  GETMBR	R15	R15	K26
          0x60400008,  //  00A5  GETGBL	R16	G8
          0x5C440400,  //  00A6  MOVE	R17	R2
          0x7C400200,  //  00A7  CALL	R16	1
          0x5C440C00,  //  00A8  MOVE	R17	R6
          0x7C300A00,  //  00A9  CALL	R12	5
          0x5834001B,  //  00AA  LDCONST	R13	K27
          0x7C280600,  //  00AB  CALL	R10	3
          0x50200000,  //  00AC  LDBOOL	R8	0	0
          0x7826001E,  //  00AD  JMPF	R9	#00CD
          0x6028000C,  //  00AE  GETGBL	R10	G12
          0x882C0123,  //  00AF  GETMBR	R11	R0	K35
          0x7C280200,  //  00B0  CALL	R10	1
          0x1C281524,  //  00B1  EQ	R10	R10	K36
          0x782A0004,  //  00B2  JMPF	R10	#00B8
          0x88280123,  //  00B3  GETMBR	R10	R0	K35
          0x8C281525,  //  00B4  GETMET	R10	R10	K37
          0x5C301200,  //  00B5  MOVE	R12	R9
          0x7C280400,  //  00B6  CALL	R10	2
          0x70020014,  //  00B7  JMP		#00CD
          0x5429FFFE,  //  00B8  LDINT	R10	-1
          0x882C0123,  //  00B9  GETMBR	R11	R0	K35
          0x9428160A,  //  00BA  GETIDX	R10	R11	R10
          0x6030000C,  //  00BB  GETGBL	R12	G12
          0x5C341400,  //  00BC  MOVE	R13	R10
          0x7C300200,  //  00BD  CALL	R12	1
          0x6034000C,  //  00BE  GETGBL	R13	G12
          0x5C381200,  //  00BF  MOVE	R14	R9
          0x7C340200,  //  00C0  CALL	R13	1
          0x0030180D,  //  00C1  ADD	R12	R12	R13
          0xB8360200,  //  00C2  GETNGBL	R13	K1
          0x88341B26,  //  00C3  GETMBR	R13	R13	K38
          0x88341B27,  //  00C4  GETMBR	R13	R13	K39
          0x1830180D,  //  00C5  LE	R12	R12	R13
          0x78320001,  //  00C6  JMPF	R12	#00C9
          0x40301409,  //  00C7  CONNECT	R12	R10	R9
          0x70020003,  //  00C8  JMP		#00CD
          0x882C0123,  //  00C9  GETMBR	R11	R0	K35
          0x8C2C1725,  //  00CA  GETMET	R11	R11	K37
          0x5C341200,  //  00CB  MOVE	R13	R9
          0x7C2C0400,  //  00CC  CALL	R11	2
          0x80041000,  //  00CD  RET	1	R8
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 5),
          be_local_const_upval(1, 8),
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
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(get_active_endpoints),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Path),
    /* K5   */  be_nested_str_weak(ReportDataMessage),
    /* K6   */  be_nested_str_weak(attribute_reports),
    /* K7   */  be_nested_str_weak(attributes_requests),
    /* K8   */  be_nested_str_weak(endpoint),
    /* K9   */  be_nested_str_weak(cluster),
    /* K10  */  be_nested_str_weak(attribute),
    /* K11  */  be_nested_str_weak(status),
    /* K12  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K13  */  be_nested_str_weak(get_attribute_name),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(format),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s),
    /* K18  */  be_nested_str_weak(local_session_id),
    /* K19  */  be_nested_str_weak(_X20_X28),
    /* K20  */  be_nested_str_weak(_X29),
    /* K21  */  be_nested_str_weak(),
    /* K22  */  be_const_int(3),
    /* K23  */  be_nested_str_weak(process_attribute_expansion),
    /* K24  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_inner_process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x84140000,  //  0001  CLOSURE	R5	P0
      0x88180101,  //  0002  GETMBR	R6	R0	K1
      0x8C180D02,  //  0003  GETMET	R6	R6	K2
      0x7C180200,  //  0004  CALL	R6	1
      0xB81E0600,  //  0005  GETNGBL	R7	K3
      0x8C1C0F04,  //  0006  GETMET	R7	R7	K4
      0x7C1C0200,  //  0007  CALL	R7	1
      0xB8220600,  //  0008  GETNGBL	R8	K3
      0x8C201105,  //  0009  GETMET	R8	R8	K5
      0x7C200200,  //  000A  CALL	R8	1
      0x60240012,  //  000B  GETGBL	R9	G18
      0x7C240000,  //  000C  CALL	R9	0
      0x90220C09,  //  000D  SETMBR	R8	K6	R9
      0x60240010,  //  000E  GETGBL	R9	G16
      0x88280507,  //  000F  GETMBR	R10	R2	K7
      0x7C240200,  //  0010  CALL	R9	1
      0xA8020046,  //  0011  EXBLK	0	#0059
      0x5C281200,  //  0012  MOVE	R10	R9
      0x7C280000,  //  0013  CALL	R10	0
      0x882C1508,  //  0014  GETMBR	R11	R10	K8
      0x901E100B,  //  0015  SETMBR	R7	K8	R11
      0x882C1509,  //  0016  GETMBR	R11	R10	K9
      0x901E120B,  //  0017  SETMBR	R7	K9	R11
      0x882C150A,  //  0018  GETMBR	R11	R10	K10
      0x901E140B,  //  0019  SETMBR	R7	K10	R11
      0xB82E0600,  //  001A  GETNGBL	R11	K3
      0x882C170C,  //  001B  GETMBR	R11	R11	K12
      0x901E160B,  //  001C  SETMBR	R7	K11	R11
      0x882C0F08,  //  001D  GETMBR	R11	R7	K8
      0x4C300000,  //  001E  LDNIL	R12
      0x1C2C160C,  //  001F  EQ	R11	R11	R12
      0x742E0007,  //  0020  JMPT	R11	#0029
      0x882C0F09,  //  0021  GETMBR	R11	R7	K9
      0x4C300000,  //  0022  LDNIL	R12
      0x1C2C160C,  //  0023  EQ	R11	R11	R12
      0x742E0003,  //  0024  JMPT	R11	#0029
      0x882C0F0A,  //  0025  GETMBR	R11	R7	K10
      0x4C300000,  //  0026  LDNIL	R12
      0x1C2C160C,  //  0027  EQ	R11	R11	R12
      0x782E0029,  //  0028  JMPF	R11	#0053
      0x882C0F09,  //  0029  GETMBR	R11	R7	K9
      0x4C300000,  //  002A  LDNIL	R12
      0x202C160C,  //  002B  NE	R11	R11	R12
      0x782E001A,  //  002C  JMPF	R11	#0048
      0x882C0F0A,  //  002D  GETMBR	R11	R7	K10
      0x4C300000,  //  002E  LDNIL	R12
      0x202C160C,  //  002F  NE	R11	R11	R12
      0x782E0016,  //  0030  JMPF	R11	#0048
      0xB82E0600,  //  0031  GETNGBL	R11	K3
      0x8C2C170D,  //  0032  GETMET	R11	R11	K13
      0x88340F09,  //  0033  GETMBR	R13	R7	K9
      0x88380F0A,  //  0034  GETMBR	R14	R7	K10
      0x7C2C0600,  //  0035  CALL	R11	3
      0xB8321C00,  //  0036  GETNGBL	R12	K14
      0x8C30190F,  //  0037  GETMET	R12	R12	K15
      0x8C380910,  //  0038  GETMET	R14	R4	K16
      0x58400011,  //  0039  LDCONST	R16	K17
      0x88440312,  //  003A  GETMBR	R17	R1	K18
      0x60480008,  //  003B  GETGBL	R18	G8
      0x5C4C0E00,  //  003C  MOVE	R19	R7
      0x7C480200,  //  003D  CALL	R18	1
      0x782E0002,  //  003E  JMPF	R11	#0042
      0x004E260B,  //  003F  ADD	R19	K19	R11
      0x004C2714,  //  0040  ADD	R19	R19	K20
      0x70020000,  //  0041  JMP		#0043
      0x584C0015,  //  0042  LDCONST	R19	K21
      0x00482413,  //  0043  ADD	R18	R18	R19
      0x7C380800,  //  0044  CALL	R14	4
      0x583C0016,  //  0045  LDCONST	R15	K22
      0x7C300600,  //  0046  CALL	R12	3
      0x7002000A,  //  0047  JMP		#0053
      0xB82E1C00,  //  0048  GETNGBL	R11	K14
      0x8C2C170F,  //  0049  GETMET	R11	R11	K15
      0x8C340910,  //  004A  GETMET	R13	R4	K16
      0x583C0011,  //  004B  LDCONST	R15	K17
      0x88400312,  //  004C  GETMBR	R16	R1	K18
      0x60440008,  //  004D  GETGBL	R17	G8
      0x5C480E00,  //  004E  MOVE	R18	R7
      0x7C440200,  //  004F  CALL	R17	1
      0x7C340800,  //  0050  CALL	R13	4
      0x58380016,  //  0051  LDCONST	R14	K22
      0x7C2C0600,  //  0052  CALL	R11	3
      0x882C0101,  //  0053  GETMBR	R11	R0	K1
      0x8C2C1717,  //  0054  GETMET	R11	R11	K23
      0x5C340E00,  //  0055  MOVE	R13	R7
      0x84380001,  //  0056  CLOSURE	R14	P1
      0x7C2C0600,  //  0057  CALL	R11	3
      0x7001FFB8,  //  0058  JMP		#0012
      0x58240018,  //  0059  LDCONST	R9	K24
      0xAC240200,  //  005A  CATCH	R9	1	0
      0xB0080000,  //  005B  RAISE	2	R0	R0
      0xA0000000,  //  005C  CLOSE	R0
      0x80041000,  //  005D  RET	1	R8
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
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(find_sendqueue_by_exchangeid),
    /* K2   */  be_nested_str_weak(exchange_id),
    /* K3   */  be_nested_str_weak(ack_received),
    }),
    be_str_weak(process_incoming_ack),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0101,  //  0001  GETMET	R3	R0	K1
      0x88140302,  //  0002  GETMBR	R5	R1	K2
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0003,  //  0004  JMPF	R3	#0009
      0x8C100703,  //  0005  GETMET	R4	R3	K3
      0x5C180200,  //  0006  MOVE	R6	R1
      0x7C100400,  //  0007  CALL	R4	2
      0x80040800,  //  0008  RET	1	R4
      0x50100000,  //  0009  LDBOOL	R4	0	0
      0x80040800,  //  000A  RET	1	R4
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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(ReadRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(attributes_requests),
    /* K4   */  be_nested_str_weak(_inner_process_read_request),
    /* K5   */  be_nested_str_weak(session),
    /* K6   */  be_nested_str_weak(send_report_data),
    }),
    be_str_weak(process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x88100703,  //  0006  GETMBR	R4	R3	K3
      0x4C140000,  //  0007  LDNIL	R5
      0x20100805,  //  0008  NE	R4	R4	R5
      0x78120007,  //  0009  JMPF	R4	#0012
      0x8C100104,  //  000A  GETMET	R4	R0	K4
      0x88180305,  //  000B  GETMBR	R6	R1	K5
      0x5C1C0600,  //  000C  MOVE	R7	R3
      0x7C100600,  //  000D  CALL	R4	3
      0x8C140106,  //  000E  GETMET	R5	R0	K6
      0x5C1C0200,  //  000F  MOVE	R7	R1
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x50100200,  //  0012  LDBOOL	R4	1	0
      0x80040800,  //  0013  RET	1	R4
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
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(find_sendqueue_by_exchangeid),
    /* K4   */  be_nested_str_weak(exchange_id),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(SUCCESS),
    /* K7   */  be_nested_str_weak(status_ok_received),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(format),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20_X3EOK_X20_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i_X20not_X20found),
    /* K12  */  be_nested_str_weak(session),
    /* K13  */  be_nested_str_weak(local_session_id),
    /* K14  */  be_const_int(3),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20ERROR_X20_X3D_X200x_X2502X),
    /* K16  */  be_nested_str_weak(status_error_received),
    /* K17  */  be_nested_str_weak(remove_sendqueue_by_exchangeid),
    }),
    be_str_weak(process_status_response),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100501,  //  0001  GETMET	R4	R2	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x541E00FE,  //  0003  LDINT	R7	255
      0x7C100600,  //  0004  CALL	R4	3
      0x8C140103,  //  0005  GETMET	R5	R0	K3
      0x881C0304,  //  0006  GETMBR	R7	R1	K4
      0x7C140400,  //  0007  CALL	R5	2
      0xB81A0A00,  //  0008  GETNGBL	R6	K5
      0x88180D06,  //  0009  GETMBR	R6	R6	K6
      0x1C180806,  //  000A  EQ	R6	R4	R6
      0x781A0010,  //  000B  JMPF	R6	#001D
      0x78160004,  //  000C  JMPF	R5	#0012
      0x8C180B07,  //  000D  GETMET	R6	R5	K7
      0x5C200200,  //  000E  MOVE	R8	R1
      0x7C180400,  //  000F  CALL	R6	2
      0x80040C00,  //  0010  RET	1	R6
      0x70020009,  //  0011  JMP		#001C
      0xB81A1000,  //  0012  GETNGBL	R6	K8
      0x8C180D09,  //  0013  GETMET	R6	R6	K9
      0x8C20070A,  //  0014  GETMET	R8	R3	K10
      0x5828000B,  //  0015  LDCONST	R10	K11
      0x882C030C,  //  0016  GETMBR	R11	R1	K12
      0x882C170D,  //  0017  GETMBR	R11	R11	K13
      0x88300304,  //  0018  GETMBR	R12	R1	K4
      0x7C200800,  //  0019  CALL	R8	4
      0x5824000E,  //  001A  LDCONST	R9	K14
      0x7C180600,  //  001B  CALL	R6	3
      0x7002000E,  //  001C  JMP		#002C
      0xB81A1000,  //  001D  GETNGBL	R6	K8
      0x8C180D09,  //  001E  GETMET	R6	R6	K9
      0x8C20070A,  //  001F  GETMET	R8	R3	K10
      0x5828000F,  //  0020  LDCONST	R10	K15
      0x5C2C0800,  //  0021  MOVE	R11	R4
      0x7C200600,  //  0022  CALL	R8	3
      0x5824000E,  //  0023  LDCONST	R9	K14
      0x7C180600,  //  0024  CALL	R6	3
      0x78160005,  //  0025  JMPF	R5	#002C
      0x8C180B10,  //  0026  GETMET	R6	R5	K16
      0x5C200200,  //  0027  MOVE	R8	R1
      0x7C180400,  //  0028  CALL	R6	2
      0x8C180111,  //  0029  GETMET	R6	R0	K17
      0x88200304,  //  002A  GETMBR	R8	R1	K4
      0x7C180400,  //  002B  CALL	R6	2
      0x50180000,  //  002C  LDBOOL	R6	0	0
      0x80040C00,  //  002D  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_response
********************************************************************/
be_local_closure(Matter_IM_process_write_response,   /* name */
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
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(WriteResponseMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(process_write_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x50140000,  //  0007  LDBOOL	R5	0	0
      0x80040A00,  //  0008  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_update
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_update,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(ReadRequestMessage),
    /* K4   */  be_nested_str_weak(fabric_filtered),
    /* K5   */  be_nested_str_weak(attributes_requests),
    /* K6   */  be_nested_str_weak(updates),
    /* K7   */  be_nested_str_weak(AttributePathIB),
    /* K8   */  be_nested_str_weak(endpoint),
    /* K9   */  be_nested_str_weak(cluster),
    /* K10  */  be_nested_str_weak(attribute),
    /* K11  */  be_nested_str_weak(push),
    /* K12  */  be_nested_str_weak(stop_iteration),
    /* K13  */  be_nested_str_weak(tasmota),
    /* K14  */  be_nested_str_weak(log),
    /* K15  */  be_nested_str_weak(format),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Data_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K17  */  be_nested_str_weak(local_session_id),
    /* K18  */  be_nested_str_weak(subscription_id),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(is_keep_alive),
    /* K21  */  be_nested_str_weak(_inner_process_read_request),
    /* K22  */  be_nested_str_weak(suppress_response),
    /* K23  */  be_nested_str_weak(IM_ReportDataSubscribed),
    /* K24  */  be_nested_str_weak(_message_handler),
    /* K25  */  be_nested_str_weak(send_queue),
    /* K26  */  be_nested_str_weak(send_enqueued),
    }),
    be_str_weak(send_subscribe_update),
    &be_const_str_solidified,
    ( &(const binstruction[67]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x7C100200,  //  0004  CALL	R4	1
      0x50140000,  //  0005  LDBOOL	R5	0	0
      0x90120805,  //  0006  SETMBR	R4	K4	R5
      0x60140012,  //  0007  GETGBL	R5	G18
      0x7C140000,  //  0008  CALL	R5	0
      0x90120A05,  //  0009  SETMBR	R4	K5	R5
      0x60140010,  //  000A  GETGBL	R5	G16
      0x88180306,  //  000B  GETMBR	R6	R1	K6
      0x7C140200,  //  000C  CALL	R5	1
      0xA802000F,  //  000D  EXBLK	0	#001E
      0x5C180A00,  //  000E  MOVE	R6	R5
      0x7C180000,  //  000F  CALL	R6	0
      0xB81E0400,  //  0010  GETNGBL	R7	K2
      0x8C1C0F07,  //  0011  GETMET	R7	R7	K7
      0x7C1C0200,  //  0012  CALL	R7	1
      0x88200D08,  //  0013  GETMBR	R8	R6	K8
      0x901E1008,  //  0014  SETMBR	R7	K8	R8
      0x88200D09,  //  0015  GETMBR	R8	R6	K9
      0x901E1208,  //  0016  SETMBR	R7	K9	R8
      0x88200D0A,  //  0017  GETMBR	R8	R6	K10
      0x901E1408,  //  0018  SETMBR	R7	K10	R8
      0x88200905,  //  0019  GETMBR	R8	R4	K5
      0x8C20110B,  //  001A  GETMET	R8	R8	K11
      0x5C280E00,  //  001B  MOVE	R10	R7
      0x7C200400,  //  001C  CALL	R8	2
      0x7001FFEF,  //  001D  JMP		#000E
      0x5814000C,  //  001E  LDCONST	R5	K12
      0xAC140200,  //  001F  CATCH	R5	1	0
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0xB8161A00,  //  0021  GETNGBL	R5	K13
      0x8C140B0E,  //  0022  GETMET	R5	R5	K14
      0x8C1C050F,  //  0023  GETMET	R7	R2	K15
      0x58240010,  //  0024  LDCONST	R9	K16
      0x88280711,  //  0025  GETMBR	R10	R3	K17
      0x882C0312,  //  0026  GETMBR	R11	R1	K18
      0x7C1C0800,  //  0027  CALL	R7	4
      0x58200013,  //  0028  LDCONST	R8	K19
      0x7C140600,  //  0029  CALL	R5	3
      0x50140000,  //  002A  LDBOOL	R5	0	0
      0x90062805,  //  002B  SETMBR	R1	K20	R5
      0x8C140115,  //  002C  GETMET	R5	R0	K21
      0x5C1C0600,  //  002D  MOVE	R7	R3
      0x5C200800,  //  002E  MOVE	R8	R4
      0x7C140600,  //  002F  CALL	R5	3
      0x50180000,  //  0030  LDBOOL	R6	0	0
      0x90162C06,  //  0031  SETMBR	R5	K22	R6
      0x88180312,  //  0032  GETMBR	R6	R1	K18
      0x90162406,  //  0033  SETMBR	R5	K18	R6
      0xB81A0400,  //  0034  GETNGBL	R6	K2
      0x8C180D17,  //  0035  GETMET	R6	R6	K23
      0x88200718,  //  0036  GETMBR	R8	R3	K24
      0x5C240600,  //  0037  MOVE	R9	R3
      0x5C280A00,  //  0038  MOVE	R10	R5
      0x5C2C0200,  //  0039  MOVE	R11	R1
      0x7C180A00,  //  003A  CALL	R6	5
      0x881C0119,  //  003B  GETMBR	R7	R0	K25
      0x8C1C0F0B,  //  003C  GETMET	R7	R7	K11
      0x5C240C00,  //  003D  MOVE	R9	R6
      0x7C1C0400,  //  003E  CALL	R7	2
      0x8C1C011A,  //  003F  GETMET	R7	R0	K26
      0x88240718,  //  0040  GETMBR	R9	R3	K24
      0x7C1C0400,  //  0041  CALL	R7	2
      0x80000000,  //  0042  RET	0
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
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TimedRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(format),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20TimedRequest_X3D_X25i),
    /* K8   */  be_nested_str_weak(session),
    /* K9   */  be_nested_str_weak(local_session_id),
    /* K10  */  be_nested_str_weak(timeout),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(send_status),
    /* K13  */  be_nested_str_weak(SUCCESS),
    }),
    be_str_weak(process_timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x8C1C0706,  //  0009  GETMET	R7	R3	K6
      0x58240007,  //  000A  LDCONST	R9	K7
      0x88280308,  //  000B  GETMBR	R10	R1	K8
      0x88281509,  //  000C  GETMBR	R10	R10	K9
      0x882C090A,  //  000D  GETMBR	R11	R4	K10
      0x7C1C0800,  //  000E  CALL	R7	4
      0x5820000B,  //  000F  LDCONST	R8	K11
      0x7C140600,  //  0010  CALL	R5	3
      0x8C14010C,  //  0011  GETMET	R5	R0	K12
      0x5C1C0200,  //  0012  MOVE	R7	R1
      0xB8220200,  //  0013  GETNGBL	R8	K1
      0x8820110D,  //  0014  GETMBR	R8	R8	K13
      0x7C140600,  //  0015  CALL	R5	3
      0x50140200,  //  0016  LDBOOL	R5	1	0
      0x80040A00,  //  0017  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: report_data
********************************************************************/
be_local_closure(Matter_IM_report_data,   /* name */
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
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(ReportDataMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(report_data),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x50140000,  //  0007  LDBOOL	R5	0	0
      0x80040A00,  //  0008  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_response
********************************************************************/
be_local_closure(Matter_IM_process_invoke_response,   /* name */
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
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(InvokeResponseMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(process_invoke_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x50140000,  //  0007  LDBOOL	R5	0	0
      0x80040A00,  //  0008  RET	1	R5
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
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(format),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Alive_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K6   */  be_nested_str_weak(local_session_id),
    /* K7   */  be_nested_str_weak(subscription_id),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(is_keep_alive),
    /* K10  */  be_nested_str_weak(matter),
    /* K11  */  be_nested_str_weak(ReportDataMessage),
    /* K12  */  be_nested_str_weak(suppress_response),
    /* K13  */  be_nested_str_weak(IM_SubscribedHeartbeat),
    /* K14  */  be_nested_str_weak(_message_handler),
    /* K15  */  be_nested_str_weak(send_queue),
    /* K16  */  be_nested_str_weak(push),
    /* K17  */  be_nested_str_weak(send_enqueued),
    }),
    be_str_weak(send_subscribe_heartbeat),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x8C180504,  //  0004  GETMET	R6	R2	K4
      0x58200005,  //  0005  LDCONST	R8	K5
      0x88240706,  //  0006  GETMBR	R9	R3	K6
      0x88280307,  //  0007  GETMBR	R10	R1	K7
      0x7C180800,  //  0008  CALL	R6	4
      0x581C0008,  //  0009  LDCONST	R7	K8
      0x7C100600,  //  000A  CALL	R4	3
      0x50100200,  //  000B  LDBOOL	R4	1	0
      0x90061204,  //  000C  SETMBR	R1	K9	R4
      0xB8121400,  //  000D  GETNGBL	R4	K10
      0x8C10090B,  //  000E  GETMET	R4	R4	K11
      0x7C100200,  //  000F  CALL	R4	1
      0x50140200,  //  0010  LDBOOL	R5	1	0
      0x90121805,  //  0011  SETMBR	R4	K12	R5
      0x88140307,  //  0012  GETMBR	R5	R1	K7
      0x90120E05,  //  0013  SETMBR	R4	K7	R5
      0xB8161400,  //  0014  GETNGBL	R5	K10
      0x8C140B0D,  //  0015  GETMET	R5	R5	K13
      0x881C070E,  //  0016  GETMBR	R7	R3	K14
      0x5C200600,  //  0017  MOVE	R8	R3
      0x5C240800,  //  0018  MOVE	R9	R4
      0x5C280200,  //  0019  MOVE	R10	R1
      0x7C140A00,  //  001A  CALL	R5	5
      0x8818010F,  //  001B  GETMBR	R6	R0	K15
      0x8C180D10,  //  001C  GETMET	R6	R6	K16
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x8C180111,  //  001F  GETMET	R6	R0	K17
      0x8820070E,  //  0020  GETMBR	R8	R3	K14
      0x7C180400,  //  0021  CALL	R6	2
      0x80000000,  //  0022  RET	0
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
