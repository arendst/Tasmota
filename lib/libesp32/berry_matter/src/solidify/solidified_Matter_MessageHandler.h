/* Solidification of Matter_MessageHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_MessageHandler;

/********************************************************************
** Solidified function: send_response
********************************************************************/
be_local_closure(Matter_MessageHandler_send_response,   /* name */
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
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(msg_send),
    }),
    be_str_weak(send_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x8C140B01,  //  0001  GETMET	R5	R5	K1
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x5C200400,  //  0003  MOVE	R8	R2
      0x5C240600,  //  0004  MOVE	R9	R3
      0x5C280800,  //  0005  MOVE	R10	R4
      0x7C140A00,  //  0006  CALL	R5	5
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(Matter_MessageHandler_msg_received,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[59]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20raw_X3D),
    /* K4   */  be_nested_str_weak(tohex),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(Frame),
    /* K7   */  be_nested_str_weak(decode_header),
    /* K8   */  be_nested_str_weak(local_session_id),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(sec_sesstype),
    /* K11  */  be_nested_str_weak(device),
    /* K12  */  be_nested_str_weak(sessions),
    /* K13  */  be_nested_str_weak(find_session_source_id_unsecure),
    /* K14  */  be_nested_str_weak(source_node_id),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20find_X20session_X20by_X20source_node_id_X20_X3D_X20),
    /* K16  */  be_nested_str_weak(session_id_X20_X3D_X20),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(session),
    /* K19  */  be_nested_str_weak(counter_rcv),
    /* K20  */  be_nested_str_weak(validate),
    /* K21  */  be_nested_str_weak(message_counter),
    /* K22  */  be_nested_str_weak(format),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20rejected_X20duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K24  */  be_nested_str_weak(val),
    /* K25  */  be_nested_str_weak(decode_payload),
    /* K26  */  be_nested_str_weak(packet_ack),
    /* K27  */  be_nested_str_weak(ack_message_counter),
    /* K28  */  be_nested_str_weak(opcode),
    /* K29  */  be_nested_str_weak(get_opcode_name),
    /* K30  */  be_nested_str_weak(0x_X2502X),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X20_X20_X20_X20_X25s_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K32  */  be_const_int(2),
    /* K33  */  be_nested_str_weak(commissioning),
    /* K34  */  be_nested_str_weak(process_incoming),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K36  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X20),
    /* K38  */  be_nested_str_weak(MTR_X3A_X20frame_X3D),
    /* K39  */  be_nested_str_weak(inspect),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20rejected_X20duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K41  */  be_nested_str_weak(_X20counter_X3D),
    /* K42  */  be_nested_str_weak(decrypt),
    /* K43  */  be_nested_str_weak(raw),
    /* K44  */  be_nested_str_weak(payload_idx),
    /* K45  */  be_const_int(1),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20idx_X3D_X25i_X20clear_X3D_X25s),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K48  */  be_nested_str_weak(protocol_id),
    /* K49  */  be_nested_str_weak(_X20opcode_X3D),
    /* K50  */  be_nested_str_weak(_X20exchange_id),
    /* K51  */  be_nested_str_weak(exchange_id),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20PROTOCOL_ID_SECURE_CHANNEL_X20),
    /* K53  */  be_nested_str_weak(im),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K56  */  be_nested_str_weak(_X3B),
    /* K57  */  be_nested_str_weak(debug),
    /* K58  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[289]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xA8020108,  //  0001  EXBLK	0	#010B
      0xB8160200,  //  0002  GETNGBL	R5	K1
      0x8C140B02,  //  0003  GETMET	R5	R5	K2
      0x8C1C0304,  //  0004  GETMET	R7	R1	K4
      0x7C1C0200,  //  0005  CALL	R7	1
      0x001E0607,  //  0006  ADD	R7	K3	R7
      0x54220003,  //  0007  LDINT	R8	4
      0x7C140600,  //  0008  CALL	R5	3
      0xB8160A00,  //  0009  GETNGBL	R5	K5
      0x8C140B06,  //  000A  GETMET	R5	R5	K6
      0x5C1C0000,  //  000B  MOVE	R7	R0
      0x5C200200,  //  000C  MOVE	R8	R1
      0x7C140600,  //  000D  CALL	R5	3
      0x8C180B07,  //  000E  GETMET	R6	R5	K7
      0x7C180200,  //  000F  CALL	R6	1
      0x5C1C0C00,  //  0010  MOVE	R7	R6
      0x741E0002,  //  0011  JMPT	R7	#0015
      0x501C0000,  //  0012  LDBOOL	R7	0	0
      0xA8040001,  //  0013  EXBLK	1	1
      0x80040E00,  //  0014  RET	1	R7
      0x881C0B08,  //  0015  GETMBR	R7	R5	K8
      0x1C1C0F09,  //  0016  EQ	R7	R7	K9
      0x781E0057,  //  0017  JMPF	R7	#0070
      0x881C0B0A,  //  0018  GETMBR	R7	R5	K10
      0x1C1C0F09,  //  0019  EQ	R7	R7	K9
      0x781E0054,  //  001A  JMPF	R7	#0070
      0x881C010B,  //  001B  GETMBR	R7	R0	K11
      0x881C0F0C,  //  001C  GETMBR	R7	R7	K12
      0x8C1C0F0D,  //  001D  GETMET	R7	R7	K13
      0x88240B0E,  //  001E  GETMBR	R9	R5	K14
      0x542A0059,  //  001F  LDINT	R10	90
      0x7C1C0600,  //  0020  CALL	R7	3
      0xB8220200,  //  0021  GETNGBL	R8	K1
      0x8C201102,  //  0022  GETMET	R8	R8	K2
      0x60280008,  //  0023  GETGBL	R10	G8
      0x882C0B0E,  //  0024  GETMBR	R11	R5	K14
      0x7C280200,  //  0025  CALL	R10	1
      0x002A1E0A,  //  0026  ADD	R10	K15	R10
      0x00281510,  //  0027  ADD	R10	R10	K16
      0x602C0008,  //  0028  GETGBL	R11	G8
      0x88300F08,  //  0029  GETMBR	R12	R7	K8
      0x7C2C0200,  //  002A  CALL	R11	1
      0x0028140B,  //  002B  ADD	R10	R10	R11
      0x582C0011,  //  002C  LDCONST	R11	K17
      0x7C200600,  //  002D  CALL	R8	3
      0x90162407,  //  002E  SETMBR	R5	K18	R7
      0x88200113,  //  002F  GETMBR	R8	R0	K19
      0x8C201114,  //  0030  GETMET	R8	R8	K20
      0x88280B15,  //  0031  GETMBR	R10	R5	K21
      0x502C0000,  //  0032  LDBOOL	R11	0	0
      0x7C200600,  //  0033  CALL	R8	3
      0x7422000D,  //  0034  JMPT	R8	#0043
      0xB8220200,  //  0035  GETNGBL	R8	K1
      0x8C201102,  //  0036  GETMET	R8	R8	K2
      0x8C280916,  //  0037  GETMET	R10	R4	K22
      0x58300017,  //  0038  LDCONST	R12	K23
      0x88340B15,  //  0039  GETMBR	R13	R5	K21
      0x88380113,  //  003A  GETMBR	R14	R0	K19
      0x8C381D18,  //  003B  GETMET	R14	R14	K24
      0x7C380200,  //  003C  CALL	R14	1
      0x7C280800,  //  003D  CALL	R10	4
      0x582C0011,  //  003E  LDCONST	R11	K17
      0x7C200600,  //  003F  CALL	R8	3
      0x50200000,  //  0040  LDBOOL	R8	0	0
      0xA8040001,  //  0041  EXBLK	1	1
      0x80041000,  //  0042  RET	1	R8
      0x8C200B19,  //  0043  GETMET	R8	R5	K25
      0x7C200200,  //  0044  CALL	R8	1
      0x74220002,  //  0045  JMPT	R8	#0049
      0x50200000,  //  0046  LDBOOL	R8	0	0
      0xA8040001,  //  0047  EXBLK	1	1
      0x80041000,  //  0048  RET	1	R8
      0x8820010B,  //  0049  GETMBR	R8	R0	K11
      0x8C20111A,  //  004A  GETMET	R8	R8	K26
      0x88280B1B,  //  004B  GETMBR	R10	R5	K27
      0x7C200400,  //  004C  CALL	R8	2
      0x88200B1C,  //  004D  GETMBR	R8	R5	K28
      0x5426000F,  //  004E  LDINT	R9	16
      0x20201009,  //  004F  NE	R8	R8	R9
      0x78220014,  //  0050  JMPF	R8	#0066
      0xB8220A00,  //  0051  GETNGBL	R8	K5
      0x8C20111D,  //  0052  GETMET	R8	R8	K29
      0x88280B1C,  //  0053  GETMBR	R10	R5	K28
      0x7C200400,  //  0054  CALL	R8	2
      0x5C241000,  //  0055  MOVE	R9	R8
      0x74260004,  //  0056  JMPT	R9	#005C
      0x8C240916,  //  0057  GETMET	R9	R4	K22
      0x582C001E,  //  0058  LDCONST	R11	K30
      0x88300B1C,  //  0059  GETMBR	R12	R5	K28
      0x7C240600,  //  005A  CALL	R9	3
      0x5C201200,  //  005B  MOVE	R8	R9
      0xB8260200,  //  005C  GETNGBL	R9	K1
      0x8C241302,  //  005D  GETMET	R9	R9	K2
      0x8C2C0916,  //  005E  GETMET	R11	R4	K22
      0x5834001F,  //  005F  LDCONST	R13	K31
      0x5C381000,  //  0060  MOVE	R14	R8
      0x5C3C0400,  //  0061  MOVE	R15	R2
      0x5C400600,  //  0062  MOVE	R16	R3
      0x7C2C0A00,  //  0063  CALL	R11	5
      0x58300020,  //  0064  LDCONST	R12	K32
      0x7C240600,  //  0065  CALL	R9	3
      0x88200121,  //  0066  GETMBR	R8	R0	K33
      0x8C201122,  //  0067  GETMET	R8	R8	K34
      0x5C280A00,  //  0068  MOVE	R10	R5
      0x5C2C0400,  //  0069  MOVE	R11	R2
      0x5C300600,  //  006A  MOVE	R12	R3
      0x7C200800,  //  006B  CALL	R8	4
      0x50200200,  //  006C  LDBOOL	R8	1	0
      0xA8040001,  //  006D  EXBLK	1	1
      0x80041000,  //  006E  RET	1	R8
      0x70020095,  //  006F  JMP		#0106
      0xB81E0200,  //  0070  GETNGBL	R7	K1
      0x8C1C0F02,  //  0071  GETMET	R7	R7	K2
      0x8C240916,  //  0072  GETMET	R9	R4	K22
      0x582C0023,  //  0073  LDCONST	R11	K35
      0x88300B08,  //  0074  GETMBR	R12	R5	K8
      0x88340B15,  //  0075  GETMBR	R13	R5	K21
      0x7C240800,  //  0076  CALL	R9	4
      0x58280011,  //  0077  LDCONST	R10	K17
      0x7C1C0600,  //  0078  CALL	R7	3
      0x881C010B,  //  0079  GETMBR	R7	R0	K11
      0x881C0F0C,  //  007A  GETMBR	R7	R7	K12
      0x8C1C0F24,  //  007B  GETMET	R7	R7	K36
      0x88240B08,  //  007C  GETMBR	R9	R5	K8
      0x7C1C0400,  //  007D  CALL	R7	2
      0x4C200000,  //  007E  LDNIL	R8
      0x1C200E08,  //  007F  EQ	R8	R7	R8
      0x78220013,  //  0080  JMPF	R8	#0095
      0xB8220200,  //  0081  GETNGBL	R8	K1
      0x8C201102,  //  0082  GETMET	R8	R8	K2
      0x60280008,  //  0083  GETGBL	R10	G8
      0x882C0B08,  //  0084  GETMBR	R11	R5	K8
      0x7C280200,  //  0085  CALL	R10	1
      0x002A4A0A,  //  0086  ADD	R10	K37	R10
      0x582C0011,  //  0087  LDCONST	R11	K17
      0x7C200600,  //  0088  CALL	R8	3
      0xB8220200,  //  0089  GETNGBL	R8	K1
      0x8C201102,  //  008A  GETMET	R8	R8	K2
      0xB82A0A00,  //  008B  GETNGBL	R10	K5
      0x8C281527,  //  008C  GETMET	R10	R10	K39
      0x5C300A00,  //  008D  MOVE	R12	R5
      0x7C280400,  //  008E  CALL	R10	2
      0x002A4C0A,  //  008F  ADD	R10	K38	R10
      0x582C0011,  //  0090  LDCONST	R11	K17
      0x7C200600,  //  0091  CALL	R8	3
      0x50200000,  //  0092  LDBOOL	R8	0	0
      0xA8040001,  //  0093  EXBLK	1	1
      0x80041000,  //  0094  RET	1	R8
      0x90162407,  //  0095  SETMBR	R5	K18	R7
      0x88200F13,  //  0096  GETMBR	R8	R7	K19
      0x8C201114,  //  0097  GETMET	R8	R8	K20
      0x88280B15,  //  0098  GETMBR	R10	R5	K21
      0x502C0200,  //  0099  LDBOOL	R11	1	0
      0x7C200600,  //  009A  CALL	R8	3
      0x74220011,  //  009B  JMPT	R8	#00AE
      0xB8220200,  //  009C  GETNGBL	R8	K1
      0x8C201102,  //  009D  GETMET	R8	R8	K2
      0x60280008,  //  009E  GETGBL	R10	G8
      0x882C0B15,  //  009F  GETMBR	R11	R5	K21
      0x7C280200,  //  00A0  CALL	R10	1
      0x002A500A,  //  00A1  ADD	R10	K40	R10
      0x00281529,  //  00A2  ADD	R10	R10	K41
      0x602C0008,  //  00A3  GETGBL	R11	G8
      0x88300F13,  //  00A4  GETMBR	R12	R7	K19
      0x8C301918,  //  00A5  GETMET	R12	R12	K24
      0x7C300200,  //  00A6  CALL	R12	1
      0x7C2C0200,  //  00A7  CALL	R11	1
      0x0028140B,  //  00A8  ADD	R10	R10	R11
      0x582C0011,  //  00A9  LDCONST	R11	K17
      0x7C200600,  //  00AA  CALL	R8	3
      0x50200000,  //  00AB  LDBOOL	R8	0	0
      0xA8040001,  //  00AC  EXBLK	1	1
      0x80041000,  //  00AD  RET	1	R8
      0x8C200B2A,  //  00AE  GETMET	R8	R5	K42
      0x7C200200,  //  00AF  CALL	R8	1
      0x5C241000,  //  00B0  MOVE	R9	R8
      0x74260002,  //  00B1  JMPT	R9	#00B5
      0x50240000,  //  00B2  LDBOOL	R9	0	0
      0xA8040001,  //  00B3  EXBLK	1	1
      0x80041200,  //  00B4  RET	1	R9
      0x88240B2C,  //  00B5  GETMBR	R9	R5	K44
      0x0424132D,  //  00B6  SUB	R9	R9	K45
      0x40261209,  //  00B7  CONNECT	R9	K9	R9
      0x88280B2B,  //  00B8  GETMBR	R10	R5	K43
      0x94241409,  //  00B9  GETIDX	R9	R10	R9
      0x90165609,  //  00BA  SETMBR	R5	K43	R9
      0x88240B2B,  //  00BB  GETMBR	R9	R5	K43
      0x40241208,  //  00BC  CONNECT	R9	R9	R8
      0xB8260200,  //  00BD  GETNGBL	R9	K1
      0x8C241302,  //  00BE  GETMET	R9	R9	K2
      0x8C2C0916,  //  00BF  GETMET	R11	R4	K22
      0x5834002E,  //  00C0  LDCONST	R13	K46
      0x88380B2C,  //  00C1  GETMBR	R14	R5	K44
      0x883C0B2B,  //  00C2  GETMBR	R15	R5	K43
      0x8C3C1F04,  //  00C3  GETMET	R15	R15	K4
      0x7C3C0200,  //  00C4  CALL	R15	1
      0x7C2C0800,  //  00C5  CALL	R11	4
      0x58300011,  //  00C6  LDCONST	R12	K17
      0x7C240600,  //  00C7  CALL	R9	3
      0x8C240B19,  //  00C8  GETMET	R9	R5	K25
      0x7C240200,  //  00C9  CALL	R9	1
      0xB8260200,  //  00CA  GETNGBL	R9	K1
      0x8C241302,  //  00CB  GETMET	R9	R9	K2
      0x602C0008,  //  00CC  GETGBL	R11	G8
      0x88300B30,  //  00CD  GETMBR	R12	R5	K48
      0x7C2C0200,  //  00CE  CALL	R11	1
      0x002E5E0B,  //  00CF  ADD	R11	K47	R11
      0x002C1731,  //  00D0  ADD	R11	R11	K49
      0x60300008,  //  00D1  GETGBL	R12	G8
      0x88340B1C,  //  00D2  GETMBR	R13	R5	K28
      0x7C300200,  //  00D3  CALL	R12	1
      0x002C160C,  //  00D4  ADD	R11	R11	R12
      0x002C1732,  //  00D5  ADD	R11	R11	K50
      0x60300008,  //  00D6  GETGBL	R12	G8
      0x88340B33,  //  00D7  GETMBR	R13	R5	K51
      0x7C300200,  //  00D8  CALL	R12	1
      0x002C160C,  //  00D9  ADD	R11	R11	R12
      0x58300011,  //  00DA  LDCONST	R12	K17
      0x7C240600,  //  00DB  CALL	R9	3
      0x8824010B,  //  00DC  GETMBR	R9	R0	K11
      0x8C24131A,  //  00DD  GETMET	R9	R9	K26
      0x882C0B1B,  //  00DE  GETMBR	R11	R5	K27
      0x7C240400,  //  00DF  CALL	R9	2
      0x88240B30,  //  00E0  GETMBR	R9	R5	K48
      0x1C281309,  //  00E1  EQ	R10	R9	K9
      0x782A000C,  //  00E2  JMPF	R10	#00F0
      0xB82A0200,  //  00E3  GETNGBL	R10	K1
      0x8C281502,  //  00E4  GETMET	R10	R10	K2
      0xB8320A00,  //  00E5  GETNGBL	R12	K5
      0x8C301927,  //  00E6  GETMET	R12	R12	K39
      0x5C380A00,  //  00E7  MOVE	R14	R5
      0x7C300400,  //  00E8  CALL	R12	2
      0x0032680C,  //  00E9  ADD	R12	K52	R12
      0x58340011,  //  00EA  LDCONST	R13	K17
      0x7C280600,  //  00EB  CALL	R10	3
      0x50280200,  //  00EC  LDBOOL	R10	1	0
      0xA8040001,  //  00ED  EXBLK	1	1
      0x80041400,  //  00EE  RET	1	R10
      0x70020015,  //  00EF  JMP		#0106
      0x1C28132D,  //  00F0  EQ	R10	R9	K45
      0x782A0008,  //  00F1  JMPF	R10	#00FB
      0x88280135,  //  00F2  GETMBR	R10	R0	K53
      0x8C281522,  //  00F3  GETMET	R10	R10	K34
      0x5C300A00,  //  00F4  MOVE	R12	R5
      0x5C340400,  //  00F5  MOVE	R13	R2
      0x5C380600,  //  00F6  MOVE	R14	R3
      0x7C280800,  //  00F7  CALL	R10	4
      0xA8040001,  //  00F8  EXBLK	1	1
      0x80041400,  //  00F9  RET	1	R10
      0x7002000A,  //  00FA  JMP		#0106
      0xB82A0200,  //  00FB  GETNGBL	R10	K1
      0x8C281502,  //  00FC  GETMET	R10	R10	K2
      0x60300008,  //  00FD  GETGBL	R12	G8
      0x5C341200,  //  00FE  MOVE	R13	R9
      0x7C300200,  //  00FF  CALL	R12	1
      0x00326C0C,  //  0100  ADD	R12	K54	R12
      0x58340011,  //  0101  LDCONST	R13	K17
      0x7C280600,  //  0102  CALL	R10	3
      0x50280000,  //  0103  LDBOOL	R10	0	0
      0xA8040001,  //  0104  EXBLK	1	1
      0x80041400,  //  0105  RET	1	R10
      0x501C0200,  //  0106  LDBOOL	R7	1	0
      0xA8040001,  //  0107  EXBLK	1	1
      0x80040E00,  //  0108  RET	1	R7
      0xA8040001,  //  0109  EXBLK	1	1
      0x70020014,  //  010A  JMP		#0120
      0xAC140002,  //  010B  CATCH	R5	0	2
      0x70020011,  //  010C  JMP		#011F
      0xB81E0200,  //  010D  GETNGBL	R7	K1
      0x8C1C0F02,  //  010E  GETMET	R7	R7	K2
      0x60240008,  //  010F  GETGBL	R9	G8
      0x5C280A00,  //  0110  MOVE	R10	R5
      0x7C240200,  //  0111  CALL	R9	1
      0x00266E09,  //  0112  ADD	R9	K55	R9
      0x00241338,  //  0113  ADD	R9	R9	K56
      0x60280008,  //  0114  GETGBL	R10	G8
      0x5C2C0C00,  //  0115  MOVE	R11	R6
      0x7C280200,  //  0116  CALL	R10	1
      0x0024120A,  //  0117  ADD	R9	R9	R10
      0x7C1C0400,  //  0118  CALL	R7	2
      0xA41E7200,  //  0119  IMPORT	R7	K57
      0x8C200F3A,  //  011A  GETMET	R8	R7	K58
      0x7C200200,  //  011B  CALL	R8	1
      0x50200000,  //  011C  LDBOOL	R8	0	0
      0x80041000,  //  011D  RET	1	R8
      0x70020000,  //  011E  JMP		#0120
      0xB0080000,  //  011F  RAISE	2	R0	R0
      0x80000000,  //  0120  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_session
********************************************************************/
be_local_closure(Matter_MessageHandler_add_session,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    7,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20add_session_X20local_session_id_X3D_X25i_X20initiator_session_id_X3D_X25i),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(sessions),
    /* K8   */  be_nested_str_weak(create_session),
    /* K9   */  be_nested_str_weak(set_keys),
    }),
    be_str_weak(add_session),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xA41E0000,  //  0000  IMPORT	R7	K0
      0xB8220200,  //  0001  GETNGBL	R8	K1
      0x8C201102,  //  0002  GETMET	R8	R8	K2
      0x8C280F03,  //  0003  GETMET	R10	R7	K3
      0x58300004,  //  0004  LDCONST	R12	K4
      0x5C340200,  //  0005  MOVE	R13	R1
      0x5C380400,  //  0006  MOVE	R14	R2
      0x7C280800,  //  0007  CALL	R10	4
      0x582C0005,  //  0008  LDCONST	R11	K5
      0x7C200600,  //  0009  CALL	R8	3
      0x88200106,  //  000A  GETMBR	R8	R0	K6
      0x88201107,  //  000B  GETMBR	R8	R8	K7
      0x8C201108,  //  000C  GETMET	R8	R8	K8
      0x5C280200,  //  000D  MOVE	R10	R1
      0x5C2C0400,  //  000E  MOVE	R11	R2
      0x7C200600,  //  000F  CALL	R8	3
      0x8C241109,  //  0010  GETMET	R9	R8	K9
      0x5C2C0600,  //  0011  MOVE	R11	R3
      0x5C300800,  //  0012  MOVE	R12	R4
      0x5C340A00,  //  0013  MOVE	R13	R5
      0x5C380C00,  //  0014  MOVE	R14	R6
      0x7C240A00,  //  0015  CALL	R9	5
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_MessageHandler_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(commissioning),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Commisioning_Context),
    /* K4   */  be_nested_str_weak(im),
    /* K5   */  be_nested_str_weak(IM),
    /* K6   */  be_nested_str_weak(counter_rcv),
    /* K7   */  be_nested_str_weak(Counter),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0xB80A0400,  //  0001  GETNGBL	R2	K2
      0x8C080503,  //  0002  GETMET	R2	R2	K3
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C080400,  //  0004  CALL	R2	2
      0x90020202,  //  0005  SETMBR	R0	K1	R2
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080505,  //  0007  GETMET	R2	R2	K5
      0x5C100000,  //  0008  MOVE	R4	R0
      0x7C080400,  //  0009  CALL	R2	2
      0x90020802,  //  000A  SETMBR	R0	K4	R2
      0xB80A0400,  //  000B  GETNGBL	R2	K2
      0x8C080507,  //  000C  GETMET	R2	R2	K7
      0x7C080200,  //  000D  CALL	R2	1
      0x90020C02,  //  000E  SETMBR	R0	K6	R2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_MessageHandler_every_second,   /* name */
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
    /* K0   */  be_nested_str_weak(commissioning),
    /* K1   */  be_nested_str_weak(every_second),
    /* K2   */  be_nested_str_weak(im),
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
** Solidified class: Matter_MessageHandler
********************************************************************/
be_local_class(Matter_MessageHandler,
    4,
    NULL,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_MessageHandler_every_second_closure) },
        { be_const_key_weak(msg_received, -1), be_const_closure(Matter_MessageHandler_msg_received_closure) },
        { be_const_key_weak(counter_rcv, -1), be_const_var(3) },
        { be_const_key_weak(commissioning, 0), be_const_var(1) },
        { be_const_key_weak(device, 5), be_const_var(0) },
        { be_const_key_weak(init, 8), be_const_closure(Matter_MessageHandler_init_closure) },
        { be_const_key_weak(send_response, 4), be_const_closure(Matter_MessageHandler_send_response_closure) },
        { be_const_key_weak(add_session, 2), be_const_closure(Matter_MessageHandler_add_session_closure) },
        { be_const_key_weak(im, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_MessageHandler)
);
/*******************************************************************/

void be_load_Matter_MessageHandler_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_MessageHandler);
    be_setglobal(vm, "Matter_MessageHandler");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
