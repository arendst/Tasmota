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
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[69]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(__ip),
    /* K19  */  be_nested_str_weak(__port),
    /* K20  */  be_nested_str_weak(session),
    /* K21  */  be_nested_str_weak(counter_rcv),
    /* K22  */  be_nested_str_weak(validate),
    /* K23  */  be_nested_str_weak(message_counter),
    /* K24  */  be_nested_str_weak(format),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20rejected_X20duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K26  */  be_nested_str_weak(val),
    /* K27  */  be_nested_str_weak(decode_payload),
    /* K28  */  be_nested_str_weak(packet_ack),
    /* K29  */  be_nested_str_weak(ack_message_counter),
    /* K30  */  be_nested_str_weak(opcode),
    /* K31  */  be_nested_str_weak(get_opcode_name),
    /* K32  */  be_nested_str_weak(0x_X2502X),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X25s_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K34  */  be_const_int(2),
    /* K35  */  be_nested_str_weak(commissioning),
    /* K36  */  be_nested_str_weak(process_incoming),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K38  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X20),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20frame_X3D),
    /* K41  */  be_nested_str_weak(inspect),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20rejected_X20duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K43  */  be_nested_str_weak(_X20counter_X3D),
    /* K44  */  be_nested_str_weak(decrypt),
    /* K45  */  be_nested_str_weak(raw),
    /* K46  */  be_nested_str_weak(payload_idx),
    /* K47  */  be_const_int(1),
    /* K48  */  be_nested_str_weak(MTR_X3A_X20idx_X3D_X25i_X20clear_X3D_X25s),
    /* K49  */  be_nested_str_weak(MTR_X3A_X20decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K50  */  be_nested_str_weak(protocol_id),
    /* K51  */  be_nested_str_weak(_X20opcode_X3D),
    /* K52  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K53  */  be_nested_str_weak(exchange_id),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20PROTOCOL_ID_SECURE_CHANNEL_X20),
    /* K55  */  be_nested_str_weak(im),
    /* K56  */  be_nested_str_weak(send_enqueued),
    /* K57  */  be_nested_str_weak(x_flag_r),
    /* K58  */  be_nested_str_weak(build_standalone_ack),
    /* K59  */  be_nested_str_weak(encode),
    /* K60  */  be_nested_str_weak(encrypt),
    /* K61  */  be_nested_str_weak(send_response),
    /* K62  */  be_nested_str_weak(remote_ip),
    /* K63  */  be_nested_str_weak(remote_port),
    /* K64  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K65  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K66  */  be_nested_str_weak(_X3B),
    /* K67  */  be_nested_str_weak(debug),
    /* K68  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[313]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x50140000,  //  0001  LDBOOL	R5	0	0
      0xA802011F,  //  0002  EXBLK	0	#0123
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
      0x7822005B,  //  001A  JMPF	R8	#0077
      0x88200D0A,  //  001B  GETMBR	R8	R6	K10
      0x1C201109,  //  001C  EQ	R8	R8	K9
      0x78220058,  //  001D  JMPF	R8	#0077
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
      0x901A2808,  //  0035  SETMBR	R6	K20	R8
      0x88240115,  //  0036  GETMBR	R9	R0	K21
      0x8C241316,  //  0037  GETMET	R9	R9	K22
      0x882C0D17,  //  0038  GETMBR	R11	R6	K23
      0x50300000,  //  0039  LDBOOL	R12	0	0
      0x7C240600,  //  003A  CALL	R9	3
      0x7426000D,  //  003B  JMPT	R9	#004A
      0xB8260200,  //  003C  GETNGBL	R9	K1
      0x8C241302,  //  003D  GETMET	R9	R9	K2
      0x8C2C0918,  //  003E  GETMET	R11	R4	K24
      0x58340019,  //  003F  LDCONST	R13	K25
      0x88380D17,  //  0040  GETMBR	R14	R6	K23
      0x883C0115,  //  0041  GETMBR	R15	R0	K21
      0x8C3C1F1A,  //  0042  GETMET	R15	R15	K26
      0x7C3C0200,  //  0043  CALL	R15	1
      0x7C2C0800,  //  0044  CALL	R11	4
      0x58300011,  //  0045  LDCONST	R12	K17
      0x7C240600,  //  0046  CALL	R9	3
      0x50240000,  //  0047  LDBOOL	R9	0	0
      0xA8040001,  //  0048  EXBLK	1	1
      0x80041200,  //  0049  RET	1	R9
      0x8C240D1B,  //  004A  GETMET	R9	R6	K27
      0x7C240200,  //  004B  CALL	R9	1
      0x74260002,  //  004C  JMPT	R9	#0050
      0x50240000,  //  004D  LDBOOL	R9	0	0
      0xA8040001,  //  004E  EXBLK	1	1
      0x80041200,  //  004F  RET	1	R9
      0x8824010B,  //  0050  GETMBR	R9	R0	K11
      0x8C24131C,  //  0051  GETMET	R9	R9	K28
      0x882C0D1D,  //  0052  GETMBR	R11	R6	K29
      0x7C240400,  //  0053  CALL	R9	2
      0x88240D1E,  //  0054  GETMBR	R9	R6	K30
      0x542A000F,  //  0055  LDINT	R10	16
      0x2024120A,  //  0056  NE	R9	R9	R10
      0x78260014,  //  0057  JMPF	R9	#006D
      0xB8260A00,  //  0058  GETNGBL	R9	K5
      0x8C24131F,  //  0059  GETMET	R9	R9	K31
      0x882C0D1E,  //  005A  GETMBR	R11	R6	K30
      0x7C240400,  //  005B  CALL	R9	2
      0x5C281200,  //  005C  MOVE	R10	R9
      0x742A0004,  //  005D  JMPT	R10	#0063
      0x8C280918,  //  005E  GETMET	R10	R4	K24
      0x58300020,  //  005F  LDCONST	R12	K32
      0x88340D1E,  //  0060  GETMBR	R13	R6	K30
      0x7C280600,  //  0061  CALL	R10	3
      0x5C241400,  //  0062  MOVE	R9	R10
      0xB82A0200,  //  0063  GETNGBL	R10	K1
      0x8C281502,  //  0064  GETMET	R10	R10	K2
      0x8C300918,  //  0065  GETMET	R12	R4	K24
      0x58380021,  //  0066  LDCONST	R14	K33
      0x5C3C1200,  //  0067  MOVE	R15	R9
      0x5C400400,  //  0068  MOVE	R16	R2
      0x5C440600,  //  0069  MOVE	R17	R3
      0x7C300A00,  //  006A  CALL	R12	5
      0x58340022,  //  006B  LDCONST	R13	K34
      0x7C280600,  //  006C  CALL	R10	3
      0x88240123,  //  006D  GETMBR	R9	R0	K35
      0x8C241324,  //  006E  GETMET	R9	R9	K36
      0x5C2C0C00,  //  006F  MOVE	R11	R6
      0x5C300400,  //  0070  MOVE	R12	R2
      0x5C340600,  //  0071  MOVE	R13	R3
      0x7C240800,  //  0072  CALL	R9	4
      0x50240200,  //  0073  LDBOOL	R9	1	0
      0xA8040001,  //  0074  EXBLK	1	1
      0x80041200,  //  0075  RET	1	R9
      0x700200A7,  //  0076  JMP		#011F
      0xB8220200,  //  0077  GETNGBL	R8	K1
      0x8C201102,  //  0078  GETMET	R8	R8	K2
      0x8C280918,  //  0079  GETMET	R10	R4	K24
      0x58300025,  //  007A  LDCONST	R12	K37
      0x88340D08,  //  007B  GETMBR	R13	R6	K8
      0x88380D17,  //  007C  GETMBR	R14	R6	K23
      0x7C280800,  //  007D  CALL	R10	4
      0x582C0011,  //  007E  LDCONST	R11	K17
      0x7C200600,  //  007F  CALL	R8	3
      0x8820010B,  //  0080  GETMBR	R8	R0	K11
      0x8820110C,  //  0081  GETMBR	R8	R8	K12
      0x8C201126,  //  0082  GETMET	R8	R8	K38
      0x88280D08,  //  0083  GETMBR	R10	R6	K8
      0x7C200400,  //  0084  CALL	R8	2
      0x4C240000,  //  0085  LDNIL	R9
      0x1C241009,  //  0086  EQ	R9	R8	R9
      0x78260013,  //  0087  JMPF	R9	#009C
      0xB8260200,  //  0088  GETNGBL	R9	K1
      0x8C241302,  //  0089  GETMET	R9	R9	K2
      0x602C0008,  //  008A  GETGBL	R11	G8
      0x88300D08,  //  008B  GETMBR	R12	R6	K8
      0x7C2C0200,  //  008C  CALL	R11	1
      0x002E4E0B,  //  008D  ADD	R11	K39	R11
      0x58300011,  //  008E  LDCONST	R12	K17
      0x7C240600,  //  008F  CALL	R9	3
      0xB8260200,  //  0090  GETNGBL	R9	K1
      0x8C241302,  //  0091  GETMET	R9	R9	K2
      0xB82E0A00,  //  0092  GETNGBL	R11	K5
      0x8C2C1729,  //  0093  GETMET	R11	R11	K41
      0x5C340C00,  //  0094  MOVE	R13	R6
      0x7C2C0400,  //  0095  CALL	R11	2
      0x002E500B,  //  0096  ADD	R11	K40	R11
      0x58300011,  //  0097  LDCONST	R12	K17
      0x7C240600,  //  0098  CALL	R9	3
      0x50240000,  //  0099  LDBOOL	R9	0	0
      0xA8040001,  //  009A  EXBLK	1	1
      0x80041200,  //  009B  RET	1	R9
      0x780A0000,  //  009C  JMPF	R2	#009E
      0x90222402,  //  009D  SETMBR	R8	K18	R2
      0x780E0000,  //  009E  JMPF	R3	#00A0
      0x90222603,  //  009F  SETMBR	R8	K19	R3
      0x901A2808,  //  00A0  SETMBR	R6	K20	R8
      0x88241115,  //  00A1  GETMBR	R9	R8	K21
      0x8C241316,  //  00A2  GETMET	R9	R9	K22
      0x882C0D17,  //  00A3  GETMBR	R11	R6	K23
      0x50300200,  //  00A4  LDBOOL	R12	1	0
      0x7C240600,  //  00A5  CALL	R9	3
      0x74260011,  //  00A6  JMPT	R9	#00B9
      0xB8260200,  //  00A7  GETNGBL	R9	K1
      0x8C241302,  //  00A8  GETMET	R9	R9	K2
      0x602C0008,  //  00A9  GETGBL	R11	G8
      0x88300D17,  //  00AA  GETMBR	R12	R6	K23
      0x7C2C0200,  //  00AB  CALL	R11	1
      0x002E540B,  //  00AC  ADD	R11	K42	R11
      0x002C172B,  //  00AD  ADD	R11	R11	K43
      0x60300008,  //  00AE  GETGBL	R12	G8
      0x88341115,  //  00AF  GETMBR	R13	R8	K21
      0x8C341B1A,  //  00B0  GETMET	R13	R13	K26
      0x7C340200,  //  00B1  CALL	R13	1
      0x7C300200,  //  00B2  CALL	R12	1
      0x002C160C,  //  00B3  ADD	R11	R11	R12
      0x58300011,  //  00B4  LDCONST	R12	K17
      0x7C240600,  //  00B5  CALL	R9	3
      0x50240000,  //  00B6  LDBOOL	R9	0	0
      0xA8040001,  //  00B7  EXBLK	1	1
      0x80041200,  //  00B8  RET	1	R9
      0x8C240D2C,  //  00B9  GETMET	R9	R6	K44
      0x7C240200,  //  00BA  CALL	R9	1
      0x5C281200,  //  00BB  MOVE	R10	R9
      0x742A0002,  //  00BC  JMPT	R10	#00C0
      0x50280000,  //  00BD  LDBOOL	R10	0	0
      0xA8040001,  //  00BE  EXBLK	1	1
      0x80041400,  //  00BF  RET	1	R10
      0x88280D2E,  //  00C0  GETMBR	R10	R6	K46
      0x0428152F,  //  00C1  SUB	R10	R10	K47
      0x402A120A,  //  00C2  CONNECT	R10	K9	R10
      0x882C0D2D,  //  00C3  GETMBR	R11	R6	K45
      0x9428160A,  //  00C4  GETIDX	R10	R11	R10
      0x901A5A0A,  //  00C5  SETMBR	R6	K45	R10
      0x88280D2D,  //  00C6  GETMBR	R10	R6	K45
      0x40281409,  //  00C7  CONNECT	R10	R10	R9
      0xB82A0200,  //  00C8  GETNGBL	R10	K1
      0x8C281502,  //  00C9  GETMET	R10	R10	K2
      0x8C300918,  //  00CA  GETMET	R12	R4	K24
      0x58380030,  //  00CB  LDCONST	R14	K48
      0x883C0D2E,  //  00CC  GETMBR	R15	R6	K46
      0x88400D2D,  //  00CD  GETMBR	R16	R6	K45
      0x8C402104,  //  00CE  GETMET	R16	R16	K4
      0x7C400200,  //  00CF  CALL	R16	1
      0x7C300800,  //  00D0  CALL	R12	4
      0x58340011,  //  00D1  LDCONST	R13	K17
      0x7C280600,  //  00D2  CALL	R10	3
      0x8C280D1B,  //  00D3  GETMET	R10	R6	K27
      0x7C280200,  //  00D4  CALL	R10	1
      0xB82A0200,  //  00D5  GETNGBL	R10	K1
      0x8C281502,  //  00D6  GETMET	R10	R10	K2
      0x60300008,  //  00D7  GETGBL	R12	G8
      0x88340D32,  //  00D8  GETMBR	R13	R6	K50
      0x7C300200,  //  00D9  CALL	R12	1
      0x0032620C,  //  00DA  ADD	R12	K49	R12
      0x00301933,  //  00DB  ADD	R12	R12	K51
      0x60340008,  //  00DC  GETGBL	R13	G8
      0x88380D1E,  //  00DD  GETMBR	R14	R6	K30
      0x7C340200,  //  00DE  CALL	R13	1
      0x0030180D,  //  00DF  ADD	R12	R12	R13
      0x00301934,  //  00E0  ADD	R12	R12	K52
      0x60340008,  //  00E1  GETGBL	R13	G8
      0x88380D35,  //  00E2  GETMBR	R14	R6	K53
      0x7C340200,  //  00E3  CALL	R13	1
      0x0030180D,  //  00E4  ADD	R12	R12	R13
      0x58340011,  //  00E5  LDCONST	R13	K17
      0x7C280600,  //  00E6  CALL	R10	3
      0x8828010B,  //  00E7  GETMBR	R10	R0	K11
      0x8C28151C,  //  00E8  GETMET	R10	R10	K28
      0x88300D1D,  //  00E9  GETMBR	R12	R6	K29
      0x7C280400,  //  00EA  CALL	R10	2
      0x88280D32,  //  00EB  GETMBR	R10	R6	K50
      0x1C2C1509,  //  00EC  EQ	R11	R10	K9
      0x782E000A,  //  00ED  JMPF	R11	#00F9
      0xB82E0200,  //  00EE  GETNGBL	R11	K1
      0x8C2C1702,  //  00EF  GETMET	R11	R11	K2
      0xB8360A00,  //  00F0  GETNGBL	R13	K5
      0x8C341B29,  //  00F1  GETMET	R13	R13	K41
      0x5C3C0C00,  //  00F2  MOVE	R15	R6
      0x7C340400,  //  00F3  CALL	R13	2
      0x00366C0D,  //  00F4  ADD	R13	K54	R13
      0x58380011,  //  00F5  LDCONST	R14	K17
      0x7C2C0600,  //  00F6  CALL	R11	3
      0x50140200,  //  00F7  LDBOOL	R5	1	0
      0x70020025,  //  00F8  JMP		#011F
      0x1C2C152F,  //  00F9  EQ	R11	R10	K47
      0x782E001B,  //  00FA  JMPF	R11	#0117
      0x882C0137,  //  00FB  GETMBR	R11	R0	K55
      0x8C2C1724,  //  00FC  GETMET	R11	R11	K36
      0x5C340C00,  //  00FD  MOVE	R13	R6
      0x5C380400,  //  00FE  MOVE	R14	R2
      0x5C3C0600,  //  00FF  MOVE	R15	R3
      0x7C2C0800,  //  0100  CALL	R11	4
      0x5C141600,  //  0101  MOVE	R5	R11
      0x78160004,  //  0102  JMPF	R5	#0108
      0x882C0137,  //  0103  GETMBR	R11	R0	K55
      0x8C2C1738,  //  0104  GETMET	R11	R11	K56
      0x5C340000,  //  0105  MOVE	R13	R0
      0x7C2C0400,  //  0106  CALL	R11	2
      0x7002000D,  //  0107  JMP		#0116
      0x882C0D39,  //  0108  GETMBR	R11	R6	K57
      0x782E000B,  //  0109  JMPF	R11	#0116
      0x8C2C0D3A,  //  010A  GETMET	R11	R6	K58
      0x7C2C0200,  //  010B  CALL	R11	1
      0x8C30173B,  //  010C  GETMET	R12	R11	K59
      0x7C300200,  //  010D  CALL	R12	1
      0x8C30173C,  //  010E  GETMET	R12	R11	K60
      0x7C300200,  //  010F  CALL	R12	1
      0x8C30013D,  //  0110  GETMET	R12	R0	K61
      0x8838172D,  //  0111  GETMBR	R14	R11	K45
      0x883C173E,  //  0112  GETMBR	R15	R11	K62
      0x8840173F,  //  0113  GETMBR	R16	R11	K63
      0x88441717,  //  0114  GETMBR	R17	R11	K23
      0x7C300A00,  //  0115  CALL	R12	5
      0x70020007,  //  0116  JMP		#011F
      0xB82E0200,  //  0117  GETNGBL	R11	K1
      0x8C2C1702,  //  0118  GETMET	R11	R11	K2
      0x60340008,  //  0119  GETGBL	R13	G8
      0x5C381400,  //  011A  MOVE	R14	R10
      0x7C340200,  //  011B  CALL	R13	1
      0x0036800D,  //  011C  ADD	R13	K64	R13
      0x58380011,  //  011D  LDCONST	R14	K17
      0x7C2C0600,  //  011E  CALL	R11	3
      0xA8040001,  //  011F  EXBLK	1	1
      0x80040A00,  //  0120  RET	1	R5
      0xA8040001,  //  0121  EXBLK	1	1
      0x70020014,  //  0122  JMP		#0138
      0xAC180002,  //  0123  CATCH	R6	0	2
      0x70020011,  //  0124  JMP		#0137
      0xB8220200,  //  0125  GETNGBL	R8	K1
      0x8C201102,  //  0126  GETMET	R8	R8	K2
      0x60280008,  //  0127  GETGBL	R10	G8
      0x5C2C0C00,  //  0128  MOVE	R11	R6
      0x7C280200,  //  0129  CALL	R10	1
      0x002A820A,  //  012A  ADD	R10	K65	R10
      0x00281542,  //  012B  ADD	R10	R10	K66
      0x602C0008,  //  012C  GETGBL	R11	G8
      0x5C300E00,  //  012D  MOVE	R12	R7
      0x7C2C0200,  //  012E  CALL	R11	1
      0x0028140B,  //  012F  ADD	R10	R10	R11
      0x7C200400,  //  0130  CALL	R8	2
      0xA4228600,  //  0131  IMPORT	R8	K67
      0x8C241144,  //  0132  GETMET	R9	R8	K68
      0x7C240200,  //  0133  CALL	R9	1
      0x50240000,  //  0134  LDBOOL	R9	0	0
      0x80041200,  //  0135  RET	1	R9
      0x70020000,  //  0136  JMP		#0138
      0xB0080000,  //  0137  RAISE	2	R0	R0
      0x80000000,  //  0138  RET	0
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
      0x5C100200,  //  0008  MOVE	R4	R1
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
