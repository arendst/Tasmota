/* Solidification of Matter_MessageHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_MessageHandler;

/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(Matter_MessageHandler_msg_received,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[72]) {     /* constants */
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
    /* K16  */  be_nested_str_weak(_X20session_id_X20_X3D_X20),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(_ip),
    /* K19  */  be_nested_str_weak(_port),
    /* K20  */  be_nested_str_weak(_message_handler),
    /* K21  */  be_nested_str_weak(session),
    /* K22  */  be_nested_str_weak(_counter_insecure_rcv),
    /* K23  */  be_nested_str_weak(validate),
    /* K24  */  be_nested_str_weak(message_counter),
    /* K25  */  be_nested_str_weak(format),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20rejected_X20duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K27  */  be_nested_str_weak(val),
    /* K28  */  be_nested_str_weak(decode_payload),
    /* K29  */  be_nested_str_weak(packet_ack),
    /* K30  */  be_nested_str_weak(ack_message_counter),
    /* K31  */  be_nested_str_weak(opcode),
    /* K32  */  be_nested_str_weak(get_opcode_name),
    /* K33  */  be_nested_str_weak(0x_X2502X),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X25s_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K35  */  be_const_int(2),
    /* K36  */  be_nested_str_weak(commissioning),
    /* K37  */  be_nested_str_weak(process_incoming),
    /* K38  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K39  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20frame_X3D),
    /* K42  */  be_nested_str_weak(inspect),
    /* K43  */  be_nested_str_weak(counter_rcv),
    /* K44  */  be_nested_str_weak(MTR_X3A_X20rejected_X20duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K45  */  be_nested_str_weak(_X20counter_X3D),
    /* K46  */  be_nested_str_weak(decrypt),
    /* K47  */  be_nested_str_weak(raw),
    /* K48  */  be_nested_str_weak(payload_idx),
    /* K49  */  be_const_int(1),
    /* K50  */  be_nested_str_weak(MTR_X3A_X20idx_X3D_X25i_X20clear_X3D_X25s),
    /* K51  */  be_nested_str_weak(MTR_X3A_X20decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K52  */  be_nested_str_weak(protocol_id),
    /* K53  */  be_nested_str_weak(_X20opcode_X3D),
    /* K54  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K55  */  be_nested_str_weak(exchange_id),
    /* K56  */  be_nested_str_weak(MTR_X3A_X20PROTOCOL_ID_SECURE_CHANNEL_X20),
    /* K57  */  be_nested_str_weak(im),
    /* K58  */  be_nested_str_weak(process_incoming_ack),
    /* K59  */  be_nested_str_weak(send_enqueued),
    /* K60  */  be_nested_str_weak(x_flag_r),
    /* K61  */  be_nested_str_weak(build_standalone_ack),
    /* K62  */  be_nested_str_weak(encode_frame),
    /* K63  */  be_nested_str_weak(encrypt),
    /* K64  */  be_nested_str_weak(send_response),
    /* K65  */  be_nested_str_weak(remote_ip),
    /* K66  */  be_nested_str_weak(remote_port),
    /* K67  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K69  */  be_nested_str_weak(_X3B),
    /* K70  */  be_nested_str_weak(debug),
    /* K71  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[328]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x50140000,  //  0001  LDBOOL	R5	0	0
      0xA802012E,  //  0002  EXBLK	0	#0132
      0xB81A0200,  //  0003  GETNGBL	R6	K1
      0x8C180D02,  //  0004  GETMET	R6	R6	K2
      0x8C200304,  //  0005  GETMET	R8	R1	K4
      0x7C200200,  //  0006  CALL	R8	1
      0x00220608,  //  0007  ADD	R8	K3	R8
      0x54260003,  //  0008  LDINT	R9	4
      0x7C180600,  //  0009  CALL	R6	3
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x8C180D06,  //  000B  GETMET	R6	R6	K6
      0x5C200000,  //  000C  MOVE	R8	R0
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280400,  //  000E  MOVE	R10	R2
      0x5C2C0600,  //  000F  MOVE	R11	R3
      0x7C180A00,  //  0010  CALL	R6	5
      0x8C1C0D07,  //  0011  GETMET	R7	R6	K7
      0x7C1C0200,  //  0012  CALL	R7	1
      0x5C200E00,  //  0013  MOVE	R8	R7
      0x74220002,  //  0014  JMPT	R8	#0018
      0x50200000,  //  0015  LDBOOL	R8	0	0
      0xA8040001,  //  0016  EXBLK	1	1
      0x80041000,  //  0017  RET	1	R8
      0x88200D08,  //  0018  GETMBR	R8	R6	K8
      0x1C201109,  //  0019  EQ	R8	R8	K9
      0x7822005A,  //  001A  JMPF	R8	#0076
      0x88200D0A,  //  001B  GETMBR	R8	R6	K10
      0x1C201109,  //  001C  EQ	R8	R8	K9
      0x78220057,  //  001D  JMPF	R8	#0076
      0x8820010B,  //  001E  GETMBR	R8	R0	K11
      0x8820110C,  //  001F  GETMBR	R8	R8	K12
      0x8C20110D,  //  0020  GETMET	R8	R8	K13
      0x88280D0E,  //  0021  GETMBR	R10	R6	K14
      0x542E0059,  //  0022  LDINT	R11	90
      0x7C200600,  //  0023  CALL	R8	3
      0xB8260200,  //  0024  GETNGBL	R9	K1
      0x8C241302,  //  0025  GETMET	R9	R9	K2
      0x602C0008,  //  0026  GETGBL	R11	G8
      0x88300D0E,  //  0027  GETMBR	R12	R6	K14
      0x7C2C0200,  //  0028  CALL	R11	1
      0x002E1E0B,  //  0029  ADD	R11	K15	R11
      0x002C1710,  //  002A  ADD	R11	R11	K16
      0x60300008,  //  002B  GETGBL	R12	G8
      0x88341108,  //  002C  GETMBR	R13	R8	K8
      0x7C300200,  //  002D  CALL	R12	1
      0x002C160C,  //  002E  ADD	R11	R11	R12
      0x58300011,  //  002F  LDCONST	R12	K17
      0x7C240600,  //  0030  CALL	R9	3
      0x780A0000,  //  0031  JMPF	R2	#0033
      0x90222402,  //  0032  SETMBR	R8	K18	R2
      0x780E0000,  //  0033  JMPF	R3	#0035
      0x90222603,  //  0034  SETMBR	R8	K19	R3
      0x90222800,  //  0035  SETMBR	R8	K20	R0
      0x901A2A08,  //  0036  SETMBR	R6	K21	R8
      0x88241116,  //  0037  GETMBR	R9	R8	K22
      0x8C241317,  //  0038  GETMET	R9	R9	K23
      0x882C0D18,  //  0039  GETMBR	R11	R6	K24
      0x50300000,  //  003A  LDBOOL	R12	0	0
      0x7C240600,  //  003B  CALL	R9	3
      0x7426000D,  //  003C  JMPT	R9	#004B
      0xB8260200,  //  003D  GETNGBL	R9	K1
      0x8C241302,  //  003E  GETMET	R9	R9	K2
      0x8C2C0919,  //  003F  GETMET	R11	R4	K25
      0x5834001A,  //  0040  LDCONST	R13	K26
      0x88380D18,  //  0041  GETMBR	R14	R6	K24
      0x883C1116,  //  0042  GETMBR	R15	R8	K22
      0x8C3C1F1B,  //  0043  GETMET	R15	R15	K27
      0x7C3C0200,  //  0044  CALL	R15	1
      0x7C2C0800,  //  0045  CALL	R11	4
      0x58300011,  //  0046  LDCONST	R12	K17
      0x7C240600,  //  0047  CALL	R9	3
      0x50240000,  //  0048  LDBOOL	R9	0	0
      0xA8040001,  //  0049  EXBLK	1	1
      0x80041200,  //  004A  RET	1	R9
      0x8C240D1C,  //  004B  GETMET	R9	R6	K28
      0x7C240200,  //  004C  CALL	R9	1
      0x74260002,  //  004D  JMPT	R9	#0051
      0x50240000,  //  004E  LDBOOL	R9	0	0
      0xA8040001,  //  004F  EXBLK	1	1
      0x80041200,  //  0050  RET	1	R9
      0x8824010B,  //  0051  GETMBR	R9	R0	K11
      0x8C24131D,  //  0052  GETMET	R9	R9	K29
      0x882C0D1E,  //  0053  GETMBR	R11	R6	K30
      0x7C240400,  //  0054  CALL	R9	2
      0x88240D1F,  //  0055  GETMBR	R9	R6	K31
      0x542A000F,  //  0056  LDINT	R10	16
      0x2024120A,  //  0057  NE	R9	R9	R10
      0x78260014,  //  0058  JMPF	R9	#006E
      0xB8260A00,  //  0059  GETNGBL	R9	K5
      0x8C241320,  //  005A  GETMET	R9	R9	K32
      0x882C0D1F,  //  005B  GETMBR	R11	R6	K31
      0x7C240400,  //  005C  CALL	R9	2
      0x5C281200,  //  005D  MOVE	R10	R9
      0x742A0004,  //  005E  JMPT	R10	#0064
      0x8C280919,  //  005F  GETMET	R10	R4	K25
      0x58300021,  //  0060  LDCONST	R12	K33
      0x88340D1F,  //  0061  GETMBR	R13	R6	K31
      0x7C280600,  //  0062  CALL	R10	3
      0x5C241400,  //  0063  MOVE	R9	R10
      0xB82A0200,  //  0064  GETNGBL	R10	K1
      0x8C281502,  //  0065  GETMET	R10	R10	K2
      0x8C300919,  //  0066  GETMET	R12	R4	K25
      0x58380022,  //  0067  LDCONST	R14	K34
      0x5C3C1200,  //  0068  MOVE	R15	R9
      0x5C400400,  //  0069  MOVE	R16	R2
      0x5C440600,  //  006A  MOVE	R17	R3
      0x7C300A00,  //  006B  CALL	R12	5
      0x58340023,  //  006C  LDCONST	R13	K35
      0x7C280600,  //  006D  CALL	R10	3
      0x88240124,  //  006E  GETMBR	R9	R0	K36
      0x8C241325,  //  006F  GETMET	R9	R9	K37
      0x5C2C0C00,  //  0070  MOVE	R11	R6
      0x7C240400,  //  0071  CALL	R9	2
      0x50240200,  //  0072  LDBOOL	R9	1	0
      0xA8040001,  //  0073  EXBLK	1	1
      0x80041200,  //  0074  RET	1	R9
      0x700200B7,  //  0075  JMP		#012E
      0xB8220200,  //  0076  GETNGBL	R8	K1
      0x8C201102,  //  0077  GETMET	R8	R8	K2
      0x8C280919,  //  0078  GETMET	R10	R4	K25
      0x58300026,  //  0079  LDCONST	R12	K38
      0x88340D08,  //  007A  GETMBR	R13	R6	K8
      0x88380D18,  //  007B  GETMBR	R14	R6	K24
      0x7C280800,  //  007C  CALL	R10	4
      0x582C0011,  //  007D  LDCONST	R11	K17
      0x7C200600,  //  007E  CALL	R8	3
      0x8820010B,  //  007F  GETMBR	R8	R0	K11
      0x8820110C,  //  0080  GETMBR	R8	R8	K12
      0x8C201127,  //  0081  GETMET	R8	R8	K39
      0x88280D08,  //  0082  GETMBR	R10	R6	K8
      0x7C200400,  //  0083  CALL	R8	2
      0x4C240000,  //  0084  LDNIL	R9
      0x1C241009,  //  0085  EQ	R9	R8	R9
      0x78260013,  //  0086  JMPF	R9	#009B
      0xB8260200,  //  0087  GETNGBL	R9	K1
      0x8C241302,  //  0088  GETMET	R9	R9	K2
      0x602C0008,  //  0089  GETGBL	R11	G8
      0x88300D08,  //  008A  GETMBR	R12	R6	K8
      0x7C2C0200,  //  008B  CALL	R11	1
      0x002E500B,  //  008C  ADD	R11	K40	R11
      0x58300023,  //  008D  LDCONST	R12	K35
      0x7C240600,  //  008E  CALL	R9	3
      0xB8260200,  //  008F  GETNGBL	R9	K1
      0x8C241302,  //  0090  GETMET	R9	R9	K2
      0xB82E0A00,  //  0091  GETNGBL	R11	K5
      0x8C2C172A,  //  0092  GETMET	R11	R11	K42
      0x5C340C00,  //  0093  MOVE	R13	R6
      0x7C2C0400,  //  0094  CALL	R11	2
      0x002E520B,  //  0095  ADD	R11	K41	R11
      0x58300011,  //  0096  LDCONST	R12	K17
      0x7C240600,  //  0097  CALL	R9	3
      0x50240000,  //  0098  LDBOOL	R9	0	0
      0xA8040001,  //  0099  EXBLK	1	1
      0x80041200,  //  009A  RET	1	R9
      0x780A0000,  //  009B  JMPF	R2	#009D
      0x90222402,  //  009C  SETMBR	R8	K18	R2
      0x780E0000,  //  009D  JMPF	R3	#009F
      0x90222603,  //  009E  SETMBR	R8	K19	R3
      0x90222800,  //  009F  SETMBR	R8	K20	R0
      0x901A2A08,  //  00A0  SETMBR	R6	K21	R8
      0x8824112B,  //  00A1  GETMBR	R9	R8	K43
      0x8C241317,  //  00A2  GETMET	R9	R9	K23
      0x882C0D18,  //  00A3  GETMBR	R11	R6	K24
      0x50300200,  //  00A4  LDBOOL	R12	1	0
      0x7C240600,  //  00A5  CALL	R9	3
      0x74260011,  //  00A6  JMPT	R9	#00B9
      0xB8260200,  //  00A7  GETNGBL	R9	K1
      0x8C241302,  //  00A8  GETMET	R9	R9	K2
      0x602C0008,  //  00A9  GETGBL	R11	G8
      0x88300D18,  //  00AA  GETMBR	R12	R6	K24
      0x7C2C0200,  //  00AB  CALL	R11	1
      0x002E580B,  //  00AC  ADD	R11	K44	R11
      0x002C172D,  //  00AD  ADD	R11	R11	K45
      0x60300008,  //  00AE  GETGBL	R12	G8
      0x8834112B,  //  00AF  GETMBR	R13	R8	K43
      0x8C341B1B,  //  00B0  GETMET	R13	R13	K27
      0x7C340200,  //  00B1  CALL	R13	1
      0x7C300200,  //  00B2  CALL	R12	1
      0x002C160C,  //  00B3  ADD	R11	R11	R12
      0x58300011,  //  00B4  LDCONST	R12	K17
      0x7C240600,  //  00B5  CALL	R9	3
      0x50240000,  //  00B6  LDBOOL	R9	0	0
      0xA8040001,  //  00B7  EXBLK	1	1
      0x80041200,  //  00B8  RET	1	R9
      0x8C240D2E,  //  00B9  GETMET	R9	R6	K46
      0x7C240200,  //  00BA  CALL	R9	1
      0x5C281200,  //  00BB  MOVE	R10	R9
      0x742A0002,  //  00BC  JMPT	R10	#00C0
      0x50280000,  //  00BD  LDBOOL	R10	0	0
      0xA8040001,  //  00BE  EXBLK	1	1
      0x80041400,  //  00BF  RET	1	R10
      0x88280D30,  //  00C0  GETMBR	R10	R6	K48
      0x04281531,  //  00C1  SUB	R10	R10	K49
      0x402A120A,  //  00C2  CONNECT	R10	K9	R10
      0x882C0D2F,  //  00C3  GETMBR	R11	R6	K47
      0x9428160A,  //  00C4  GETIDX	R10	R11	R10
      0x901A5E0A,  //  00C5  SETMBR	R6	K47	R10
      0x88280D2F,  //  00C6  GETMBR	R10	R6	K47
      0x40281409,  //  00C7  CONNECT	R10	R10	R9
      0xB82A0200,  //  00C8  GETNGBL	R10	K1
      0x8C281502,  //  00C9  GETMET	R10	R10	K2
      0x8C300919,  //  00CA  GETMET	R12	R4	K25
      0x58380032,  //  00CB  LDCONST	R14	K50
      0x883C0D30,  //  00CC  GETMBR	R15	R6	K48
      0x88400D2F,  //  00CD  GETMBR	R16	R6	K47
      0x8C402104,  //  00CE  GETMET	R16	R16	K4
      0x7C400200,  //  00CF  CALL	R16	1
      0x7C300800,  //  00D0  CALL	R12	4
      0x54360003,  //  00D1  LDINT	R13	4
      0x7C280600,  //  00D2  CALL	R10	3
      0x8C280D1C,  //  00D3  GETMET	R10	R6	K28
      0x7C280200,  //  00D4  CALL	R10	1
      0xB82A0200,  //  00D5  GETNGBL	R10	K1
      0x8C281502,  //  00D6  GETMET	R10	R10	K2
      0x60300008,  //  00D7  GETGBL	R12	G8
      0x88340D34,  //  00D8  GETMBR	R13	R6	K52
      0x7C300200,  //  00D9  CALL	R12	1
      0x0032660C,  //  00DA  ADD	R12	K51	R12
      0x00301935,  //  00DB  ADD	R12	R12	K53
      0x60340008,  //  00DC  GETGBL	R13	G8
      0x88380D1F,  //  00DD  GETMBR	R14	R6	K31
      0x7C340200,  //  00DE  CALL	R13	1
      0x0030180D,  //  00DF  ADD	R12	R12	R13
      0x00301936,  //  00E0  ADD	R12	R12	K54
      0x60340008,  //  00E1  GETGBL	R13	G8
      0x88380D37,  //  00E2  GETMBR	R14	R6	K55
      0x543EFFFE,  //  00E3  LDINT	R15	65535
      0x2C381C0F,  //  00E4  AND	R14	R14	R15
      0x7C340200,  //  00E5  CALL	R13	1
      0x0030180D,  //  00E6  ADD	R12	R12	R13
      0x58340011,  //  00E7  LDCONST	R13	K17
      0x7C280600,  //  00E8  CALL	R10	3
      0x8828010B,  //  00E9  GETMBR	R10	R0	K11
      0x8C28151D,  //  00EA  GETMET	R10	R10	K29
      0x88300D1E,  //  00EB  GETMBR	R12	R6	K30
      0x7C280400,  //  00EC  CALL	R10	2
      0x88280D34,  //  00ED  GETMBR	R10	R6	K52
      0x1C2C1509,  //  00EE  EQ	R11	R10	K9
      0x782E0018,  //  00EF  JMPF	R11	#0109
      0xB82E0200,  //  00F0  GETNGBL	R11	K1
      0x8C2C1702,  //  00F1  GETMET	R11	R11	K2
      0xB8360A00,  //  00F2  GETNGBL	R13	K5
      0x8C341B2A,  //  00F3  GETMET	R13	R13	K42
      0x5C3C0C00,  //  00F4  MOVE	R15	R6
      0x7C340400,  //  00F5  CALL	R13	2
      0x0036700D,  //  00F6  ADD	R13	K56	R13
      0x58380011,  //  00F7  LDCONST	R14	K17
      0x7C2C0600,  //  00F8  CALL	R11	3
      0x882C0D1F,  //  00F9  GETMBR	R11	R6	K31
      0x5432000F,  //  00FA  LDINT	R12	16
      0x1C2C160C,  //  00FB  EQ	R11	R11	R12
      0x782E0009,  //  00FC  JMPF	R11	#0107
      0x882C0139,  //  00FD  GETMBR	R11	R0	K57
      0x8C2C173A,  //  00FE  GETMET	R11	R11	K58
      0x5C340C00,  //  00FF  MOVE	R13	R6
      0x7C2C0400,  //  0100  CALL	R11	2
      0x5C141600,  //  0101  MOVE	R5	R11
      0x78160003,  //  0102  JMPF	R5	#0107
      0x882C0139,  //  0103  GETMBR	R11	R0	K57
      0x8C2C173B,  //  0104  GETMET	R11	R11	K59
      0x5C340000,  //  0105  MOVE	R13	R0
      0x7C2C0400,  //  0106  CALL	R11	2
      0x50140200,  //  0107  LDBOOL	R5	1	0
      0x70020024,  //  0108  JMP		#012E
      0x1C2C1531,  //  0109  EQ	R11	R10	K49
      0x782E001A,  //  010A  JMPF	R11	#0126
      0x882C0139,  //  010B  GETMBR	R11	R0	K57
      0x8C2C1725,  //  010C  GETMET	R11	R11	K37
      0x5C340C00,  //  010D  MOVE	R13	R6
      0x7C2C0400,  //  010E  CALL	R11	2
      0x5C141600,  //  010F  MOVE	R5	R11
      0x78160004,  //  0110  JMPF	R5	#0116
      0x882C0139,  //  0111  GETMBR	R11	R0	K57
      0x8C2C173B,  //  0112  GETMET	R11	R11	K59
      0x5C340000,  //  0113  MOVE	R13	R0
      0x7C2C0400,  //  0114  CALL	R11	2
      0x7002000D,  //  0115  JMP		#0124
      0x882C0D3C,  //  0116  GETMBR	R11	R6	K60
      0x782E000B,  //  0117  JMPF	R11	#0124
      0x8C2C0D3D,  //  0118  GETMET	R11	R6	K61
      0x7C2C0200,  //  0119  CALL	R11	1
      0x8C30173E,  //  011A  GETMET	R12	R11	K62
      0x7C300200,  //  011B  CALL	R12	1
      0x8C30173F,  //  011C  GETMET	R12	R11	K63
      0x7C300200,  //  011D  CALL	R12	1
      0x8C300140,  //  011E  GETMET	R12	R0	K64
      0x8838172F,  //  011F  GETMBR	R14	R11	K47
      0x883C1741,  //  0120  GETMBR	R15	R11	K65
      0x88401742,  //  0121  GETMBR	R16	R11	K66
      0x88441718,  //  0122  GETMBR	R17	R11	K24
      0x7C300A00,  //  0123  CALL	R12	5
      0x50140200,  //  0124  LDBOOL	R5	1	0
      0x70020007,  //  0125  JMP		#012E
      0xB82E0200,  //  0126  GETNGBL	R11	K1
      0x8C2C1702,  //  0127  GETMET	R11	R11	K2
      0x60340008,  //  0128  GETGBL	R13	G8
      0x5C381400,  //  0129  MOVE	R14	R10
      0x7C340200,  //  012A  CALL	R13	1
      0x0036860D,  //  012B  ADD	R13	K67	R13
      0x58380011,  //  012C  LDCONST	R14	K17
      0x7C2C0600,  //  012D  CALL	R11	3
      0xA8040001,  //  012E  EXBLK	1	1
      0x80040A00,  //  012F  RET	1	R5
      0xA8040001,  //  0130  EXBLK	1	1
      0x70020014,  //  0131  JMP		#0147
      0xAC180002,  //  0132  CATCH	R6	0	2
      0x70020011,  //  0133  JMP		#0146
      0xB8220200,  //  0134  GETNGBL	R8	K1
      0x8C201102,  //  0135  GETMET	R8	R8	K2
      0x60280008,  //  0136  GETGBL	R10	G8
      0x5C2C0C00,  //  0137  MOVE	R11	R6
      0x7C280200,  //  0138  CALL	R10	1
      0x002A880A,  //  0139  ADD	R10	K68	R10
      0x00281545,  //  013A  ADD	R10	R10	K69
      0x602C0008,  //  013B  GETGBL	R11	G8
      0x5C300E00,  //  013C  MOVE	R12	R7
      0x7C2C0200,  //  013D  CALL	R11	1
      0x0028140B,  //  013E  ADD	R10	R10	R11
      0x7C200400,  //  013F  CALL	R8	2
      0xA4228C00,  //  0140  IMPORT	R8	K70
      0x8C241147,  //  0141  GETMET	R9	R8	K71
      0x7C240200,  //  0142  CALL	R9	1
      0x50240000,  //  0143  LDBOOL	R9	0	0
      0x80041200,  //  0144  RET	1	R9
      0x70020000,  //  0145  JMP		#0147
      0xB0080000,  //  0146  RAISE	2	R0	R0
      0x80000000,  //  0147  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(Matter_MessageHandler_every_250ms,   /* name */
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
    /* K0   */  be_nested_str_weak(im),
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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(commissioning),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Commisioning_Context),
    /* K4   */  be_nested_str_weak(im),
    /* K5   */  be_nested_str_weak(IM),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0xB80A0400,  //  0001  GETNGBL	R2	K2
      0x8C080503,  //  0002  GETMET	R2	R2	K3
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C080400,  //  0004  CALL	R2	2
      0x90020202,  //  0005  SETMBR	R0	K1	R2
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080505,  //  0007  GETMET	R2	R2	K5
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x90020802,  //  000A  SETMBR	R0	K4	R2
      0x80000000,  //  000B  RET	0
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
** Solidified class: Matter_MessageHandler
********************************************************************/
be_local_class(Matter_MessageHandler,
    3,
    NULL,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(send_response, 5), be_const_closure(Matter_MessageHandler_send_response_closure) },
        { be_const_key_weak(msg_received, -1), be_const_closure(Matter_MessageHandler_msg_received_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_MessageHandler_every_250ms_closure) },
        { be_const_key_weak(commissioning, 8), be_const_var(1) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_MessageHandler_init_closure) },
        { be_const_key_weak(device, 4), be_const_var(0) },
        { be_const_key_weak(im, 0), be_const_var(2) },
        { be_const_key_weak(add_session, -1), be_const_closure(Matter_MessageHandler_add_session_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_MessageHandler_every_second_closure) },
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
