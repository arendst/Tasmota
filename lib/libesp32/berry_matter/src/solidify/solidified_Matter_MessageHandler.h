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
    24,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[77]) {     /* constants */
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
    /* K26  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Rejected_X20duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K27  */  be_nested_str_weak(val),
    /* K28  */  be_nested_str_weak(decode_payload),
    /* K29  */  be_nested_str_weak(received_ack),
    /* K30  */  be_nested_str_weak(ack_message_counter),
    /* K31  */  be_nested_str_weak(opcode),
    /* K32  */  be_nested_str_weak(get_opcode_name),
    /* K33  */  be_nested_str_weak(0x_X2502X),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20rid_X3D_X25i_X20exch_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K35  */  be_nested_str_weak(exchange_id),
    /* K36  */  be_const_int(2),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K38  */  be_nested_str_weak(x_flag_r),
    /* K39  */  be_nested_str_weak(_X7Breliable_X7D_X20),
    /* K40  */  be_nested_str_weak(),
    /* K41  */  be_nested_str_weak(commissioning),
    /* K42  */  be_nested_str_weak(process_incoming),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K44  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
    /* K46  */  be_nested_str_weak(counter_rcv_validate),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Rejected_X20duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K48  */  be_nested_str_weak(_X20counter_X3D),
    /* K49  */  be_nested_str_weak(counter_rcv),
    /* K50  */  be_nested_str_weak(decrypt),
    /* K51  */  be_nested_str_weak(raw),
    /* K52  */  be_nested_str_weak(payload_idx),
    /* K53  */  be_const_int(1),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20idx_X3D_X25i_X20clear_X3D_X25s),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20_X3E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K56  */  be_nested_str_weak(protocol_id),
    /* K57  */  be_nested_str_weak(_X20opcode_X3D),
    /* K58  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20_X5B_X2502X_X2F_X2502X_X5D_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K60  */  be_nested_str_weak(MTR_X3A_X20PROTOCOL_ID_SECURE_CHANNEL_X20),
    /* K61  */  be_nested_str_weak(inspect),
    /* K62  */  be_nested_str_weak(im),
    /* K63  */  be_nested_str_weak(process_incoming_ack),
    /* K64  */  be_nested_str_weak(send_enqueued),
    /* K65  */  be_nested_str_weak(build_standalone_ack),
    /* K66  */  be_nested_str_weak(encode_frame),
    /* K67  */  be_nested_str_weak(encrypt),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X7Breliable_X7D),
    /* K69  */  be_nested_str_weak(send_response),
    /* K70  */  be_nested_str_weak(remote_ip),
    /* K71  */  be_nested_str_weak(remote_port),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K74  */  be_nested_str_weak(_X3B),
    /* K75  */  be_nested_str_weak(debug),
    /* K76  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[376]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x50140000,  //  0001  LDBOOL	R5	0	0
      0xA802015E,  //  0002  EXBLK	0	#0162
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
      0x78220072,  //  001A  JMPF	R8	#008E
      0x88200D0A,  //  001B  GETMBR	R8	R6	K10
      0x1C201109,  //  001C  EQ	R8	R8	K9
      0x7822006F,  //  001D  JMPF	R8	#008E
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
      0x78260018,  //  0058  JMPF	R9	#0072
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
      0x883C1108,  //  0068  GETMBR	R15	R8	K8
      0x5C401200,  //  0069  MOVE	R16	R9
      0x88440D18,  //  006A  GETMBR	R17	R6	K24
      0x88480D23,  //  006B  GETMBR	R18	R6	K35
      0x5C4C0400,  //  006C  MOVE	R19	R2
      0x5C500600,  //  006D  MOVE	R20	R3
      0x7C301000,  //  006E  CALL	R12	8
      0x58340024,  //  006F  LDCONST	R13	K36
      0x7C280600,  //  0070  CALL	R10	3
      0x70020013,  //  0071  JMP		#0086
      0xB8260200,  //  0072  GETNGBL	R9	K1
      0x8C241302,  //  0073  GETMET	R9	R9	K2
      0x8C2C0919,  //  0074  GETMET	R11	R4	K25
      0x58340025,  //  0075  LDCONST	R13	K37
      0x88381108,  //  0076  GETMBR	R14	R8	K8
      0x883C0D18,  //  0077  GETMBR	R15	R6	K24
      0x88400D26,  //  0078  GETMBR	R16	R6	K38
      0x78420001,  //  0079  JMPF	R16	#007C
      0x58400027,  //  007A  LDCONST	R16	K39
      0x70020000,  //  007B  JMP		#007D
      0x58400028,  //  007C  LDCONST	R16	K40
      0x88440D23,  //  007D  GETMBR	R17	R6	K35
      0x60480008,  //  007E  GETGBL	R18	G8
      0x884C0D1E,  //  007F  GETMBR	R19	R6	K30
      0x7C480200,  //  0080  CALL	R18	1
      0x5C4C0400,  //  0081  MOVE	R19	R2
      0x5C500600,  //  0082  MOVE	R20	R3
      0x7C2C1200,  //  0083  CALL	R11	9
      0x58300011,  //  0084  LDCONST	R12	K17
      0x7C240600,  //  0085  CALL	R9	3
      0x88240129,  //  0086  GETMBR	R9	R0	K41
      0x8C24132A,  //  0087  GETMET	R9	R9	K42
      0x5C2C0C00,  //  0088  MOVE	R11	R6
      0x7C240400,  //  0089  CALL	R9	2
      0x50240200,  //  008A  LDBOOL	R9	1	0
      0xA8040001,  //  008B  EXBLK	1	1
      0x80041200,  //  008C  RET	1	R9
      0x700200CF,  //  008D  JMP		#015E
      0xB8220200,  //  008E  GETNGBL	R8	K1
      0x8C201102,  //  008F  GETMET	R8	R8	K2
      0x8C280919,  //  0090  GETMET	R10	R4	K25
      0x5830002B,  //  0091  LDCONST	R12	K43
      0x88340D08,  //  0092  GETMBR	R13	R6	K8
      0x88380D18,  //  0093  GETMBR	R14	R6	K24
      0x7C280800,  //  0094  CALL	R10	4
      0x582C0011,  //  0095  LDCONST	R11	K17
      0x7C200600,  //  0096  CALL	R8	3
      0x8820010B,  //  0097  GETMBR	R8	R0	K11
      0x8820110C,  //  0098  GETMBR	R8	R8	K12
      0x8C20112C,  //  0099  GETMET	R8	R8	K44
      0x88280D08,  //  009A  GETMBR	R10	R6	K8
      0x7C200400,  //  009B  CALL	R8	2
      0x4C240000,  //  009C  LDNIL	R9
      0x1C241009,  //  009D  EQ	R9	R8	R9
      0x7826000A,  //  009E  JMPF	R9	#00AA
      0xB8260200,  //  009F  GETNGBL	R9	K1
      0x8C241302,  //  00A0  GETMET	R9	R9	K2
      0x602C0008,  //  00A1  GETGBL	R11	G8
      0x88300D08,  //  00A2  GETMBR	R12	R6	K8
      0x7C2C0200,  //  00A3  CALL	R11	1
      0x002E5A0B,  //  00A4  ADD	R11	K45	R11
      0x58300024,  //  00A5  LDCONST	R12	K36
      0x7C240600,  //  00A6  CALL	R9	3
      0x50240000,  //  00A7  LDBOOL	R9	0	0
      0xA8040001,  //  00A8  EXBLK	1	1
      0x80041200,  //  00A9  RET	1	R9
      0x780A0000,  //  00AA  JMPF	R2	#00AC
      0x90222402,  //  00AB  SETMBR	R8	K18	R2
      0x780E0000,  //  00AC  JMPF	R3	#00AE
      0x90222603,  //  00AD  SETMBR	R8	K19	R3
      0x90222800,  //  00AE  SETMBR	R8	K20	R0
      0x901A2A08,  //  00AF  SETMBR	R6	K21	R8
      0x8C24112E,  //  00B0  GETMET	R9	R8	K46
      0x882C0D18,  //  00B1  GETMBR	R11	R6	K24
      0x50300200,  //  00B2  LDBOOL	R12	1	0
      0x7C240600,  //  00B3  CALL	R9	3
      0x7426000F,  //  00B4  JMPT	R9	#00C5
      0xB8260200,  //  00B5  GETNGBL	R9	K1
      0x8C241302,  //  00B6  GETMET	R9	R9	K2
      0x602C0008,  //  00B7  GETGBL	R11	G8
      0x88300D18,  //  00B8  GETMBR	R12	R6	K24
      0x7C2C0200,  //  00B9  CALL	R11	1
      0x002E5E0B,  //  00BA  ADD	R11	K47	R11
      0x002C1730,  //  00BB  ADD	R11	R11	K48
      0x60300008,  //  00BC  GETGBL	R12	G8
      0x88341131,  //  00BD  GETMBR	R13	R8	K49
      0x7C300200,  //  00BE  CALL	R12	1
      0x002C160C,  //  00BF  ADD	R11	R11	R12
      0x58300011,  //  00C0  LDCONST	R12	K17
      0x7C240600,  //  00C1  CALL	R9	3
      0x50240000,  //  00C2  LDBOOL	R9	0	0
      0xA8040001,  //  00C3  EXBLK	1	1
      0x80041200,  //  00C4  RET	1	R9
      0x8C240D32,  //  00C5  GETMET	R9	R6	K50
      0x7C240200,  //  00C6  CALL	R9	1
      0x5C281200,  //  00C7  MOVE	R10	R9
      0x742A0002,  //  00C8  JMPT	R10	#00CC
      0x50280000,  //  00C9  LDBOOL	R10	0	0
      0xA8040001,  //  00CA  EXBLK	1	1
      0x80041400,  //  00CB  RET	1	R10
      0x88280D34,  //  00CC  GETMBR	R10	R6	K52
      0x04281535,  //  00CD  SUB	R10	R10	K53
      0x402A120A,  //  00CE  CONNECT	R10	K9	R10
      0x882C0D33,  //  00CF  GETMBR	R11	R6	K51
      0x9428160A,  //  00D0  GETIDX	R10	R11	R10
      0x901A660A,  //  00D1  SETMBR	R6	K51	R10
      0x88280D33,  //  00D2  GETMBR	R10	R6	K51
      0x40281409,  //  00D3  CONNECT	R10	R10	R9
      0xB82A0200,  //  00D4  GETNGBL	R10	K1
      0x8C281502,  //  00D5  GETMET	R10	R10	K2
      0x8C300919,  //  00D6  GETMET	R12	R4	K25
      0x58380036,  //  00D7  LDCONST	R14	K54
      0x883C0D34,  //  00D8  GETMBR	R15	R6	K52
      0x88400D33,  //  00D9  GETMBR	R16	R6	K51
      0x8C402104,  //  00DA  GETMET	R16	R16	K4
      0x7C400200,  //  00DB  CALL	R16	1
      0x7C300800,  //  00DC  CALL	R12	4
      0x54360003,  //  00DD  LDINT	R13	4
      0x7C280600,  //  00DE  CALL	R10	3
      0x8C280D1C,  //  00DF  GETMET	R10	R6	K28
      0x7C280200,  //  00E0  CALL	R10	1
      0xB82A0200,  //  00E1  GETNGBL	R10	K1
      0x8C281502,  //  00E2  GETMET	R10	R10	K2
      0x60300008,  //  00E3  GETGBL	R12	G8
      0x88340D38,  //  00E4  GETMBR	R13	R6	K56
      0x7C300200,  //  00E5  CALL	R12	1
      0x00326E0C,  //  00E6  ADD	R12	K55	R12
      0x00301939,  //  00E7  ADD	R12	R12	K57
      0x60340008,  //  00E8  GETGBL	R13	G8
      0x88380D1F,  //  00E9  GETMBR	R14	R6	K31
      0x7C340200,  //  00EA  CALL	R13	1
      0x0030180D,  //  00EB  ADD	R12	R12	R13
      0x0030193A,  //  00EC  ADD	R12	R12	K58
      0x60340008,  //  00ED  GETGBL	R13	G8
      0x88380D23,  //  00EE  GETMBR	R14	R6	K35
      0x543EFFFE,  //  00EF  LDINT	R15	65535
      0x2C381C0F,  //  00F0  AND	R14	R14	R15
      0x7C340200,  //  00F1  CALL	R13	1
      0x0030180D,  //  00F2  ADD	R12	R12	R13
      0x58340011,  //  00F3  LDCONST	R13	K17
      0x7C280600,  //  00F4  CALL	R10	3
      0xB82A0200,  //  00F5  GETNGBL	R10	K1
      0x8C281502,  //  00F6  GETMET	R10	R10	K2
      0x8C300919,  //  00F7  GETMET	R12	R4	K25
      0x5838003B,  //  00F8  LDCONST	R14	K59
      0x883C1108,  //  00F9  GETMBR	R15	R8	K8
      0x88400D38,  //  00FA  GETMBR	R16	R6	K56
      0x88440D1F,  //  00FB  GETMBR	R17	R6	K31
      0x88480D18,  //  00FC  GETMBR	R18	R6	K24
      0x884C0D23,  //  00FD  GETMBR	R19	R6	K35
      0x60500008,  //  00FE  GETGBL	R20	G8
      0x88540D1E,  //  00FF  GETMBR	R21	R6	K30
      0x7C500200,  //  0100  CALL	R20	1
      0x88540D26,  //  0101  GETMBR	R21	R6	K38
      0x78560001,  //  0102  JMPF	R21	#0105
      0x58540027,  //  0103  LDCONST	R21	K39
      0x70020000,  //  0104  JMP		#0106
      0x58540028,  //  0105  LDCONST	R21	K40
      0x5C580400,  //  0106  MOVE	R22	R2
      0x5C5C0600,  //  0107  MOVE	R23	R3
      0x7C301600,  //  0108  CALL	R12	11
      0x58340011,  //  0109  LDCONST	R13	K17
      0x7C280600,  //  010A  CALL	R10	3
      0x8828010B,  //  010B  GETMBR	R10	R0	K11
      0x8C28151D,  //  010C  GETMET	R10	R10	K29
      0x88300D1E,  //  010D  GETMBR	R12	R6	K30
      0x7C280400,  //  010E  CALL	R10	2
      0x88280D38,  //  010F  GETMBR	R10	R6	K56
      0x1C2C1509,  //  0110  EQ	R11	R10	K9
      0x782E0018,  //  0111  JMPF	R11	#012B
      0xB82E0200,  //  0112  GETNGBL	R11	K1
      0x8C2C1702,  //  0113  GETMET	R11	R11	K2
      0xB8360A00,  //  0114  GETNGBL	R13	K5
      0x8C341B3D,  //  0115  GETMET	R13	R13	K61
      0x5C3C0C00,  //  0116  MOVE	R15	R6
      0x7C340400,  //  0117  CALL	R13	2
      0x0036780D,  //  0118  ADD	R13	K60	R13
      0x58380011,  //  0119  LDCONST	R14	K17
      0x7C2C0600,  //  011A  CALL	R11	3
      0x882C0D1F,  //  011B  GETMBR	R11	R6	K31
      0x5432000F,  //  011C  LDINT	R12	16
      0x1C2C160C,  //  011D  EQ	R11	R11	R12
      0x782E0009,  //  011E  JMPF	R11	#0129
      0x882C013E,  //  011F  GETMBR	R11	R0	K62
      0x8C2C173F,  //  0120  GETMET	R11	R11	K63
      0x5C340C00,  //  0121  MOVE	R13	R6
      0x7C2C0400,  //  0122  CALL	R11	2
      0x5C141600,  //  0123  MOVE	R5	R11
      0x78160003,  //  0124  JMPF	R5	#0129
      0x882C013E,  //  0125  GETMBR	R11	R0	K62
      0x8C2C1740,  //  0126  GETMET	R11	R11	K64
      0x5C340000,  //  0127  MOVE	R13	R0
      0x7C2C0400,  //  0128  CALL	R11	2
      0x50140200,  //  0129  LDBOOL	R5	1	0
      0x70020032,  //  012A  JMP		#015E
      0x1C2C1535,  //  012B  EQ	R11	R10	K53
      0x782E0028,  //  012C  JMPF	R11	#0156
      0x882C013E,  //  012D  GETMBR	R11	R0	K62
      0x8C2C172A,  //  012E  GETMET	R11	R11	K42
      0x5C340C00,  //  012F  MOVE	R13	R6
      0x7C2C0400,  //  0130  CALL	R11	2
      0x5C141600,  //  0131  MOVE	R5	R11
      0x78160004,  //  0132  JMPF	R5	#0138
      0x882C013E,  //  0133  GETMBR	R11	R0	K62
      0x8C2C1740,  //  0134  GETMET	R11	R11	K64
      0x5C340000,  //  0135  MOVE	R13	R0
      0x7C2C0400,  //  0136  CALL	R11	2
      0x7002001B,  //  0137  JMP		#0154
      0x882C0D26,  //  0138  GETMBR	R11	R6	K38
      0x782E0019,  //  0139  JMPF	R11	#0154
      0x8C2C0D41,  //  013A  GETMET	R11	R6	K65
      0x50340200,  //  013B  LDBOOL	R13	1	0
      0x7C2C0400,  //  013C  CALL	R11	2
      0x8C301742,  //  013D  GETMET	R12	R11	K66
      0x7C300200,  //  013E  CALL	R12	1
      0x8C301743,  //  013F  GETMET	R12	R11	K67
      0x7C300200,  //  0140  CALL	R12	1
      0xB8320200,  //  0141  GETNGBL	R12	K1
      0x8C301902,  //  0142  GETMET	R12	R12	K2
      0x8C380919,  //  0143  GETMET	R14	R4	K25
      0x58400044,  //  0144  LDCONST	R16	K68
      0x88441715,  //  0145  GETMBR	R17	R11	K21
      0x88442308,  //  0146  GETMBR	R17	R17	K8
      0x8848171E,  //  0147  GETMBR	R18	R11	K30
      0x884C1718,  //  0148  GETMBR	R19	R11	K24
      0x7C380A00,  //  0149  CALL	R14	5
      0x583C0011,  //  014A  LDCONST	R15	K17
      0x7C300600,  //  014B  CALL	R12	3
      0x8C300145,  //  014C  GETMET	R12	R0	K69
      0x88381733,  //  014D  GETMBR	R14	R11	K51
      0x883C1746,  //  014E  GETMBR	R15	R11	K70
      0x88401747,  //  014F  GETMBR	R16	R11	K71
      0x88441718,  //  0150  GETMBR	R17	R11	K24
      0x88481715,  //  0151  GETMBR	R18	R11	K21
      0x88482508,  //  0152  GETMBR	R18	R18	K8
      0x7C300C00,  //  0153  CALL	R12	6
      0x50140200,  //  0154  LDBOOL	R5	1	0
      0x70020007,  //  0155  JMP		#015E
      0xB82E0200,  //  0156  GETNGBL	R11	K1
      0x8C2C1702,  //  0157  GETMET	R11	R11	K2
      0x60340008,  //  0158  GETGBL	R13	G8
      0x5C381400,  //  0159  MOVE	R14	R10
      0x7C340200,  //  015A  CALL	R13	1
      0x0036900D,  //  015B  ADD	R13	K72	R13
      0x58380011,  //  015C  LDCONST	R14	K17
      0x7C2C0600,  //  015D  CALL	R11	3
      0xA8040001,  //  015E  EXBLK	1	1
      0x80040A00,  //  015F  RET	1	R5
      0xA8040001,  //  0160  EXBLK	1	1
      0x70020014,  //  0161  JMP		#0177
      0xAC180002,  //  0162  CATCH	R6	0	2
      0x70020011,  //  0163  JMP		#0176
      0xB8220200,  //  0164  GETNGBL	R8	K1
      0x8C201102,  //  0165  GETMET	R8	R8	K2
      0x60280008,  //  0166  GETGBL	R10	G8
      0x5C2C0C00,  //  0167  MOVE	R11	R6
      0x7C280200,  //  0168  CALL	R10	1
      0x002A920A,  //  0169  ADD	R10	K73	R10
      0x0028154A,  //  016A  ADD	R10	R10	K74
      0x602C0008,  //  016B  GETGBL	R11	G8
      0x5C300E00,  //  016C  MOVE	R12	R7
      0x7C2C0200,  //  016D  CALL	R11	1
      0x0028140B,  //  016E  ADD	R10	R10	R11
      0x7C200400,  //  016F  CALL	R8	2
      0xA4229600,  //  0170  IMPORT	R8	K75
      0x8C24114C,  //  0171  GETMET	R9	R8	K76
      0x7C240200,  //  0172  CALL	R9	1
      0x50240000,  //  0173  LDBOOL	R9	0	0
      0x80041200,  //  0174  RET	1	R9
      0x70020000,  //  0175  JMP		#0177
      0xB0080000,  //  0176  RAISE	2	R0	R0
      0x80000000,  //  0177  RET	0
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
    13,                          /* nstack */
    6,                          /* argc */
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
    ( &(const binstruction[ 9]) {  /* code */
      0x88180100,  //  0000  GETMBR	R6	R0	K0
      0x8C180D01,  //  0001  GETMET	R6	R6	K1
      0x5C200200,  //  0002  MOVE	R8	R1
      0x5C240400,  //  0003  MOVE	R9	R2
      0x5C280600,  //  0004  MOVE	R10	R3
      0x5C2C0800,  //  0005  MOVE	R11	R4
      0x5C300A00,  //  0006  MOVE	R12	R5
      0x7C180C00,  //  0007  CALL	R6	6
      0x80000000,  //  0008  RET	0
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
