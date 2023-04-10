/* Solidification of Matter_MessageHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_MessageHandler;

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
    ( &(const bvalue[78]) {     /* constants */
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
    /* K26  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K27  */  be_nested_str_weak(val),
    /* K28  */  be_nested_str_weak(x_flag_r),
    /* K29  */  be_nested_str_weak(build_standalone_ack),
    /* K30  */  be_nested_str_weak(encode_frame),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X7Breliable_X7D),
    /* K32  */  be_nested_str_weak(ack_message_counter),
    /* K33  */  be_nested_str_weak(send_response),
    /* K34  */  be_nested_str_weak(raw),
    /* K35  */  be_nested_str_weak(remote_ip),
    /* K36  */  be_nested_str_weak(remote_port),
    /* K37  */  be_nested_str_weak(decode_payload),
    /* K38  */  be_nested_str_weak(received_ack),
    /* K39  */  be_nested_str_weak(opcode),
    /* K40  */  be_nested_str_weak(get_opcode_name),
    /* K41  */  be_nested_str_weak(0x_X2502X),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20rid_X3D_X25i_X20exch_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K43  */  be_nested_str_weak(exchange_id),
    /* K44  */  be_const_int(2),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K46  */  be_nested_str_weak(_X7Breliable_X7D_X20),
    /* K47  */  be_nested_str_weak(),
    /* K48  */  be_nested_str_weak(commissioning),
    /* K49  */  be_nested_str_weak(process_incoming),
    /* K50  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i),
    /* K51  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K52  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
    /* K54  */  be_nested_str_weak(counter_rcv_validate),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K56  */  be_nested_str_weak(_X20counter_X3D),
    /* K57  */  be_nested_str_weak(counter_rcv),
    /* K58  */  be_nested_str_weak(encrypt),
    /* K59  */  be_nested_str_weak(decrypt),
    /* K60  */  be_nested_str_weak(payload_idx),
    /* K61  */  be_const_int(1),
    /* K62  */  be_nested_str_weak(MTR_X3A_X20idx_X3D_X25i_X20clear_X3D_X25s),
    /* K63  */  be_nested_str_weak(MTR_X3A_X20_X3E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K64  */  be_nested_str_weak(protocol_id),
    /* K65  */  be_nested_str_weak(_X20opcode_X3D),
    /* K66  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K67  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20_X5B_X2502X_X2F_X2502X_X5D_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20PROTOCOL_ID_SECURE_CHANNEL_X20),
    /* K69  */  be_nested_str_weak(inspect),
    /* K70  */  be_nested_str_weak(im),
    /* K71  */  be_nested_str_weak(process_incoming_ack),
    /* K72  */  be_nested_str_weak(send_enqueued),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K75  */  be_nested_str_weak(_X3B),
    /* K76  */  be_nested_str_weak(debug),
    /* K77  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[459]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x50140000,  //  0001  LDBOOL	R5	0	0
      0xA80201B1,  //  0002  EXBLK	0	#01B5
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
      0x782200A9,  //  001A  JMPF	R8	#00C5
      0x88200D0A,  //  001B  GETMBR	R8	R6	K10
      0x1C201109,  //  001C  EQ	R8	R8	K9
      0x782200A6,  //  001D  JMPF	R8	#00C5
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
      0x74260027,  //  003C  JMPT	R9	#0065
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
      0x88240D1C,  //  0048  GETMBR	R9	R6	K28
      0x78260017,  //  0049  JMPF	R9	#0062
      0x8C240D1D,  //  004A  GETMET	R9	R6	K29
      0x502C0000,  //  004B  LDBOOL	R11	0	0
      0x7C240400,  //  004C  CALL	R9	2
      0x8C28131E,  //  004D  GETMET	R10	R9	K30
      0x7C280200,  //  004E  CALL	R10	1
      0xB82A0200,  //  004F  GETNGBL	R10	K1
      0x8C281502,  //  0050  GETMET	R10	R10	K2
      0x8C300919,  //  0051  GETMET	R12	R4	K25
      0x5838001F,  //  0052  LDCONST	R14	K31
      0x883C1315,  //  0053  GETMBR	R15	R9	K21
      0x883C1F08,  //  0054  GETMBR	R15	R15	K8
      0x88401320,  //  0055  GETMBR	R16	R9	K32
      0x88441318,  //  0056  GETMBR	R17	R9	K24
      0x7C300A00,  //  0057  CALL	R12	5
      0x54360003,  //  0058  LDINT	R13	4
      0x7C280600,  //  0059  CALL	R10	3
      0x8C280121,  //  005A  GETMET	R10	R0	K33
      0x88301322,  //  005B  GETMBR	R12	R9	K34
      0x88341323,  //  005C  GETMBR	R13	R9	K35
      0x88381324,  //  005D  GETMBR	R14	R9	K36
      0x4C3C0000,  //  005E  LDNIL	R15
      0x88401315,  //  005F  GETMBR	R16	R9	K21
      0x88402108,  //  0060  GETMBR	R16	R16	K8
      0x7C280C00,  //  0061  CALL	R10	6
      0x50240000,  //  0062  LDBOOL	R9	0	0
      0xA8040001,  //  0063  EXBLK	1	1
      0x80041200,  //  0064  RET	1	R9
      0x8C240D25,  //  0065  GETMET	R9	R6	K37
      0x7C240200,  //  0066  CALL	R9	1
      0x74260002,  //  0067  JMPT	R9	#006B
      0x50240000,  //  0068  LDBOOL	R9	0	0
      0xA8040001,  //  0069  EXBLK	1	1
      0x80041200,  //  006A  RET	1	R9
      0x8824010B,  //  006B  GETMBR	R9	R0	K11
      0x8C241326,  //  006C  GETMET	R9	R9	K38
      0x882C0D20,  //  006D  GETMBR	R11	R6	K32
      0x7C240400,  //  006E  CALL	R9	2
      0x88240D27,  //  006F  GETMBR	R9	R6	K39
      0x542A000F,  //  0070  LDINT	R10	16
      0x2024120A,  //  0071  NE	R9	R9	R10
      0x78260018,  //  0072  JMPF	R9	#008C
      0xB8260A00,  //  0073  GETNGBL	R9	K5
      0x8C241328,  //  0074  GETMET	R9	R9	K40
      0x882C0D27,  //  0075  GETMBR	R11	R6	K39
      0x7C240400,  //  0076  CALL	R9	2
      0x5C281200,  //  0077  MOVE	R10	R9
      0x742A0004,  //  0078  JMPT	R10	#007E
      0x8C280919,  //  0079  GETMET	R10	R4	K25
      0x58300029,  //  007A  LDCONST	R12	K41
      0x88340D27,  //  007B  GETMBR	R13	R6	K39
      0x7C280600,  //  007C  CALL	R10	3
      0x5C241400,  //  007D  MOVE	R9	R10
      0xB82A0200,  //  007E  GETNGBL	R10	K1
      0x8C281502,  //  007F  GETMET	R10	R10	K2
      0x8C300919,  //  0080  GETMET	R12	R4	K25
      0x5838002A,  //  0081  LDCONST	R14	K42
      0x883C1108,  //  0082  GETMBR	R15	R8	K8
      0x5C401200,  //  0083  MOVE	R16	R9
      0x88440D18,  //  0084  GETMBR	R17	R6	K24
      0x88480D2B,  //  0085  GETMBR	R18	R6	K43
      0x5C4C0400,  //  0086  MOVE	R19	R2
      0x5C500600,  //  0087  MOVE	R20	R3
      0x7C301000,  //  0088  CALL	R12	8
      0x5834002C,  //  0089  LDCONST	R13	K44
      0x7C280600,  //  008A  CALL	R10	3
      0x70020013,  //  008B  JMP		#00A0
      0xB8260200,  //  008C  GETNGBL	R9	K1
      0x8C241302,  //  008D  GETMET	R9	R9	K2
      0x8C2C0919,  //  008E  GETMET	R11	R4	K25
      0x5834002D,  //  008F  LDCONST	R13	K45
      0x88381108,  //  0090  GETMBR	R14	R8	K8
      0x883C0D18,  //  0091  GETMBR	R15	R6	K24
      0x88400D1C,  //  0092  GETMBR	R16	R6	K28
      0x78420001,  //  0093  JMPF	R16	#0096
      0x5840002E,  //  0094  LDCONST	R16	K46
      0x70020000,  //  0095  JMP		#0097
      0x5840002F,  //  0096  LDCONST	R16	K47
      0x88440D2B,  //  0097  GETMBR	R17	R6	K43
      0x60480008,  //  0098  GETGBL	R18	G8
      0x884C0D20,  //  0099  GETMBR	R19	R6	K32
      0x7C480200,  //  009A  CALL	R18	1
      0x5C4C0400,  //  009B  MOVE	R19	R2
      0x5C500600,  //  009C  MOVE	R20	R3
      0x7C2C1200,  //  009D  CALL	R11	9
      0x58300011,  //  009E  LDCONST	R12	K17
      0x7C240600,  //  009F  CALL	R9	3
      0x88240130,  //  00A0  GETMBR	R9	R0	K48
      0x8C241331,  //  00A1  GETMET	R9	R9	K49
      0x5C2C0C00,  //  00A2  MOVE	R11	R6
      0x7C240400,  //  00A3  CALL	R9	2
      0x5C141200,  //  00A4  MOVE	R5	R9
      0x5C240A00,  //  00A5  MOVE	R9	R5
      0x74260019,  //  00A6  JMPT	R9	#00C1
      0x88240D1C,  //  00A7  GETMBR	R9	R6	K28
      0x78260017,  //  00A8  JMPF	R9	#00C1
      0x8C240D1D,  //  00A9  GETMET	R9	R6	K29
      0x502C0000,  //  00AA  LDBOOL	R11	0	0
      0x7C240400,  //  00AB  CALL	R9	2
      0x8C28131E,  //  00AC  GETMET	R10	R9	K30
      0x7C280200,  //  00AD  CALL	R10	1
      0xB82A0200,  //  00AE  GETNGBL	R10	K1
      0x8C281502,  //  00AF  GETMET	R10	R10	K2
      0x8C300919,  //  00B0  GETMET	R12	R4	K25
      0x58380032,  //  00B1  LDCONST	R14	K50
      0x883C1315,  //  00B2  GETMBR	R15	R9	K21
      0x883C1F08,  //  00B3  GETMBR	R15	R15	K8
      0x88401320,  //  00B4  GETMBR	R16	R9	K32
      0x88441318,  //  00B5  GETMBR	R17	R9	K24
      0x7C300A00,  //  00B6  CALL	R12	5
      0x58340011,  //  00B7  LDCONST	R13	K17
      0x7C280600,  //  00B8  CALL	R10	3
      0x8C280121,  //  00B9  GETMET	R10	R0	K33
      0x88301322,  //  00BA  GETMBR	R12	R9	K34
      0x88341323,  //  00BB  GETMBR	R13	R9	K35
      0x88381324,  //  00BC  GETMBR	R14	R9	K36
      0x4C3C0000,  //  00BD  LDNIL	R15
      0x88401315,  //  00BE  GETMBR	R16	R9	K21
      0x88402108,  //  00BF  GETMBR	R16	R16	K8
      0x7C280C00,  //  00C0  CALL	R10	6
      0x50240200,  //  00C1  LDBOOL	R9	1	0
      0xA8040001,  //  00C2  EXBLK	1	1
      0x80041200,  //  00C3  RET	1	R9
      0x700200EB,  //  00C4  JMP		#01B1
      0xB8220200,  //  00C5  GETNGBL	R8	K1
      0x8C201102,  //  00C6  GETMET	R8	R8	K2
      0x8C280919,  //  00C7  GETMET	R10	R4	K25
      0x58300033,  //  00C8  LDCONST	R12	K51
      0x88340D08,  //  00C9  GETMBR	R13	R6	K8
      0x88380D18,  //  00CA  GETMBR	R14	R6	K24
      0x7C280800,  //  00CB  CALL	R10	4
      0x582C0011,  //  00CC  LDCONST	R11	K17
      0x7C200600,  //  00CD  CALL	R8	3
      0x8820010B,  //  00CE  GETMBR	R8	R0	K11
      0x8820110C,  //  00CF  GETMBR	R8	R8	K12
      0x8C201134,  //  00D0  GETMET	R8	R8	K52
      0x88280D08,  //  00D1  GETMBR	R10	R6	K8
      0x7C200400,  //  00D2  CALL	R8	2
      0x4C240000,  //  00D3  LDNIL	R9
      0x1C241009,  //  00D4  EQ	R9	R8	R9
      0x7826000A,  //  00D5  JMPF	R9	#00E1
      0xB8260200,  //  00D6  GETNGBL	R9	K1
      0x8C241302,  //  00D7  GETMET	R9	R9	K2
      0x602C0008,  //  00D8  GETGBL	R11	G8
      0x88300D08,  //  00D9  GETMBR	R12	R6	K8
      0x7C2C0200,  //  00DA  CALL	R11	1
      0x002E6A0B,  //  00DB  ADD	R11	K53	R11
      0x5830002C,  //  00DC  LDCONST	R12	K44
      0x7C240600,  //  00DD  CALL	R9	3
      0x50240000,  //  00DE  LDBOOL	R9	0	0
      0xA8040001,  //  00DF  EXBLK	1	1
      0x80041200,  //  00E0  RET	1	R9
      0x780A0000,  //  00E1  JMPF	R2	#00E3
      0x90222402,  //  00E2  SETMBR	R8	K18	R2
      0x780E0000,  //  00E3  JMPF	R3	#00E5
      0x90222603,  //  00E4  SETMBR	R8	K19	R3
      0x90222800,  //  00E5  SETMBR	R8	K20	R0
      0x901A2A08,  //  00E6  SETMBR	R6	K21	R8
      0x8C241136,  //  00E7  GETMET	R9	R8	K54
      0x882C0D18,  //  00E8  GETMBR	R11	R6	K24
      0x50300200,  //  00E9  LDBOOL	R12	1	0
      0x7C240600,  //  00EA  CALL	R9	3
      0x7426002B,  //  00EB  JMPT	R9	#0118
      0xB8260200,  //  00EC  GETNGBL	R9	K1
      0x8C241302,  //  00ED  GETMET	R9	R9	K2
      0x602C0008,  //  00EE  GETGBL	R11	G8
      0x88300D18,  //  00EF  GETMBR	R12	R6	K24
      0x7C2C0200,  //  00F0  CALL	R11	1
      0x002E6E0B,  //  00F1  ADD	R11	K55	R11
      0x002C1738,  //  00F2  ADD	R11	R11	K56
      0x60300008,  //  00F3  GETGBL	R12	G8
      0x88341139,  //  00F4  GETMBR	R13	R8	K57
      0x7C300200,  //  00F5  CALL	R12	1
      0x002C160C,  //  00F6  ADD	R11	R11	R12
      0x58300011,  //  00F7  LDCONST	R12	K17
      0x7C240600,  //  00F8  CALL	R9	3
      0x88240D1C,  //  00F9  GETMBR	R9	R6	K28
      0x78260019,  //  00FA  JMPF	R9	#0115
      0x8C240D1D,  //  00FB  GETMET	R9	R6	K29
      0x502C0000,  //  00FC  LDBOOL	R11	0	0
      0x7C240400,  //  00FD  CALL	R9	2
      0x8C28131E,  //  00FE  GETMET	R10	R9	K30
      0x7C280200,  //  00FF  CALL	R10	1
      0x8C28133A,  //  0100  GETMET	R10	R9	K58
      0x7C280200,  //  0101  CALL	R10	1
      0xB82A0200,  //  0102  GETNGBL	R10	K1
      0x8C281502,  //  0103  GETMET	R10	R10	K2
      0x8C300919,  //  0104  GETMET	R12	R4	K25
      0x5838001F,  //  0105  LDCONST	R14	K31
      0x883C1315,  //  0106  GETMBR	R15	R9	K21
      0x883C1F08,  //  0107  GETMBR	R15	R15	K8
      0x88401320,  //  0108  GETMBR	R16	R9	K32
      0x88441318,  //  0109  GETMBR	R17	R9	K24
      0x7C300A00,  //  010A  CALL	R12	5
      0x54360003,  //  010B  LDINT	R13	4
      0x7C280600,  //  010C  CALL	R10	3
      0x8C280121,  //  010D  GETMET	R10	R0	K33
      0x88301322,  //  010E  GETMBR	R12	R9	K34
      0x88341323,  //  010F  GETMBR	R13	R9	K35
      0x88381324,  //  0110  GETMBR	R14	R9	K36
      0x4C3C0000,  //  0111  LDNIL	R15
      0x88401315,  //  0112  GETMBR	R16	R9	K21
      0x88402108,  //  0113  GETMBR	R16	R16	K8
      0x7C280C00,  //  0114  CALL	R10	6
      0x50240000,  //  0115  LDBOOL	R9	0	0
      0xA8040001,  //  0116  EXBLK	1	1
      0x80041200,  //  0117  RET	1	R9
      0x8C240D3B,  //  0118  GETMET	R9	R6	K59
      0x7C240200,  //  0119  CALL	R9	1
      0x5C281200,  //  011A  MOVE	R10	R9
      0x742A0002,  //  011B  JMPT	R10	#011F
      0x50280000,  //  011C  LDBOOL	R10	0	0
      0xA8040001,  //  011D  EXBLK	1	1
      0x80041400,  //  011E  RET	1	R10
      0x88280D3C,  //  011F  GETMBR	R10	R6	K60
      0x0428153D,  //  0120  SUB	R10	R10	K61
      0x402A120A,  //  0121  CONNECT	R10	K9	R10
      0x882C0D22,  //  0122  GETMBR	R11	R6	K34
      0x9428160A,  //  0123  GETIDX	R10	R11	R10
      0x901A440A,  //  0124  SETMBR	R6	K34	R10
      0x88280D22,  //  0125  GETMBR	R10	R6	K34
      0x40281409,  //  0126  CONNECT	R10	R10	R9
      0xB82A0200,  //  0127  GETNGBL	R10	K1
      0x8C281502,  //  0128  GETMET	R10	R10	K2
      0x8C300919,  //  0129  GETMET	R12	R4	K25
      0x5838003E,  //  012A  LDCONST	R14	K62
      0x883C0D3C,  //  012B  GETMBR	R15	R6	K60
      0x88400D22,  //  012C  GETMBR	R16	R6	K34
      0x8C402104,  //  012D  GETMET	R16	R16	K4
      0x7C400200,  //  012E  CALL	R16	1
      0x7C300800,  //  012F  CALL	R12	4
      0x54360003,  //  0130  LDINT	R13	4
      0x7C280600,  //  0131  CALL	R10	3
      0x8C280D25,  //  0132  GETMET	R10	R6	K37
      0x7C280200,  //  0133  CALL	R10	1
      0xB82A0200,  //  0134  GETNGBL	R10	K1
      0x8C281502,  //  0135  GETMET	R10	R10	K2
      0x60300008,  //  0136  GETGBL	R12	G8
      0x88340D40,  //  0137  GETMBR	R13	R6	K64
      0x7C300200,  //  0138  CALL	R12	1
      0x00327E0C,  //  0139  ADD	R12	K63	R12
      0x00301941,  //  013A  ADD	R12	R12	K65
      0x60340008,  //  013B  GETGBL	R13	G8
      0x88380D27,  //  013C  GETMBR	R14	R6	K39
      0x7C340200,  //  013D  CALL	R13	1
      0x0030180D,  //  013E  ADD	R12	R12	R13
      0x00301942,  //  013F  ADD	R12	R12	K66
      0x60340008,  //  0140  GETGBL	R13	G8
      0x88380D2B,  //  0141  GETMBR	R14	R6	K43
      0x543EFFFE,  //  0142  LDINT	R15	65535
      0x2C381C0F,  //  0143  AND	R14	R14	R15
      0x7C340200,  //  0144  CALL	R13	1
      0x0030180D,  //  0145  ADD	R12	R12	R13
      0x58340011,  //  0146  LDCONST	R13	K17
      0x7C280600,  //  0147  CALL	R10	3
      0xB82A0200,  //  0148  GETNGBL	R10	K1
      0x8C281502,  //  0149  GETMET	R10	R10	K2
      0x8C300919,  //  014A  GETMET	R12	R4	K25
      0x58380043,  //  014B  LDCONST	R14	K67
      0x883C1108,  //  014C  GETMBR	R15	R8	K8
      0x88400D40,  //  014D  GETMBR	R16	R6	K64
      0x88440D27,  //  014E  GETMBR	R17	R6	K39
      0x88480D18,  //  014F  GETMBR	R18	R6	K24
      0x884C0D2B,  //  0150  GETMBR	R19	R6	K43
      0x60500008,  //  0151  GETGBL	R20	G8
      0x88540D20,  //  0152  GETMBR	R21	R6	K32
      0x7C500200,  //  0153  CALL	R20	1
      0x88540D1C,  //  0154  GETMBR	R21	R6	K28
      0x78560001,  //  0155  JMPF	R21	#0158
      0x5854002E,  //  0156  LDCONST	R21	K46
      0x70020000,  //  0157  JMP		#0159
      0x5854002F,  //  0158  LDCONST	R21	K47
      0x5C580400,  //  0159  MOVE	R22	R2
      0x5C5C0600,  //  015A  MOVE	R23	R3
      0x7C301600,  //  015B  CALL	R12	11
      0x58340011,  //  015C  LDCONST	R13	K17
      0x7C280600,  //  015D  CALL	R10	3
      0x8828010B,  //  015E  GETMBR	R10	R0	K11
      0x8C281526,  //  015F  GETMET	R10	R10	K38
      0x88300D20,  //  0160  GETMBR	R12	R6	K32
      0x7C280400,  //  0161  CALL	R10	2
      0x88280D40,  //  0162  GETMBR	R10	R6	K64
      0x1C2C1509,  //  0163  EQ	R11	R10	K9
      0x782E0018,  //  0164  JMPF	R11	#017E
      0xB82E0200,  //  0165  GETNGBL	R11	K1
      0x8C2C1702,  //  0166  GETMET	R11	R11	K2
      0xB8360A00,  //  0167  GETNGBL	R13	K5
      0x8C341B45,  //  0168  GETMET	R13	R13	K69
      0x5C3C0C00,  //  0169  MOVE	R15	R6
      0x7C340400,  //  016A  CALL	R13	2
      0x0036880D,  //  016B  ADD	R13	K68	R13
      0x58380011,  //  016C  LDCONST	R14	K17
      0x7C2C0600,  //  016D  CALL	R11	3
      0x882C0D27,  //  016E  GETMBR	R11	R6	K39
      0x5432000F,  //  016F  LDINT	R12	16
      0x1C2C160C,  //  0170  EQ	R11	R11	R12
      0x782E0009,  //  0171  JMPF	R11	#017C
      0x882C0146,  //  0172  GETMBR	R11	R0	K70
      0x8C2C1747,  //  0173  GETMET	R11	R11	K71
      0x5C340C00,  //  0174  MOVE	R13	R6
      0x7C2C0400,  //  0175  CALL	R11	2
      0x5C141600,  //  0176  MOVE	R5	R11
      0x78160003,  //  0177  JMPF	R5	#017C
      0x882C0146,  //  0178  GETMBR	R11	R0	K70
      0x8C2C1748,  //  0179  GETMET	R11	R11	K72
      0x5C340000,  //  017A  MOVE	R13	R0
      0x7C2C0400,  //  017B  CALL	R11	2
      0x50140200,  //  017C  LDBOOL	R5	1	0
      0x70020032,  //  017D  JMP		#01B1
      0x1C2C153D,  //  017E  EQ	R11	R10	K61
      0x782E0028,  //  017F  JMPF	R11	#01A9
      0x882C0146,  //  0180  GETMBR	R11	R0	K70
      0x8C2C1731,  //  0181  GETMET	R11	R11	K49
      0x5C340C00,  //  0182  MOVE	R13	R6
      0x7C2C0400,  //  0183  CALL	R11	2
      0x5C141600,  //  0184  MOVE	R5	R11
      0x78160004,  //  0185  JMPF	R5	#018B
      0x882C0146,  //  0186  GETMBR	R11	R0	K70
      0x8C2C1748,  //  0187  GETMET	R11	R11	K72
      0x5C340000,  //  0188  MOVE	R13	R0
      0x7C2C0400,  //  0189  CALL	R11	2
      0x7002001B,  //  018A  JMP		#01A7
      0x882C0D1C,  //  018B  GETMBR	R11	R6	K28
      0x782E0019,  //  018C  JMPF	R11	#01A7
      0x8C2C0D1D,  //  018D  GETMET	R11	R6	K29
      0x50340200,  //  018E  LDBOOL	R13	1	0
      0x7C2C0400,  //  018F  CALL	R11	2
      0x8C30171E,  //  0190  GETMET	R12	R11	K30
      0x7C300200,  //  0191  CALL	R12	1
      0x8C30173A,  //  0192  GETMET	R12	R11	K58
      0x7C300200,  //  0193  CALL	R12	1
      0xB8320200,  //  0194  GETNGBL	R12	K1
      0x8C301902,  //  0195  GETMET	R12	R12	K2
      0x8C380919,  //  0196  GETMET	R14	R4	K25
      0x5840001F,  //  0197  LDCONST	R16	K31
      0x88441715,  //  0198  GETMBR	R17	R11	K21
      0x88442308,  //  0199  GETMBR	R17	R17	K8
      0x88481720,  //  019A  GETMBR	R18	R11	K32
      0x884C1718,  //  019B  GETMBR	R19	R11	K24
      0x7C380A00,  //  019C  CALL	R14	5
      0x583C0011,  //  019D  LDCONST	R15	K17
      0x7C300600,  //  019E  CALL	R12	3
      0x8C300121,  //  019F  GETMET	R12	R0	K33
      0x88381722,  //  01A0  GETMBR	R14	R11	K34
      0x883C1723,  //  01A1  GETMBR	R15	R11	K35
      0x88401724,  //  01A2  GETMBR	R16	R11	K36
      0x88441718,  //  01A3  GETMBR	R17	R11	K24
      0x88481715,  //  01A4  GETMBR	R18	R11	K21
      0x88482508,  //  01A5  GETMBR	R18	R18	K8
      0x7C300C00,  //  01A6  CALL	R12	6
      0x50140200,  //  01A7  LDBOOL	R5	1	0
      0x70020007,  //  01A8  JMP		#01B1
      0xB82E0200,  //  01A9  GETNGBL	R11	K1
      0x8C2C1702,  //  01AA  GETMET	R11	R11	K2
      0x60340008,  //  01AB  GETGBL	R13	G8
      0x5C381400,  //  01AC  MOVE	R14	R10
      0x7C340200,  //  01AD  CALL	R13	1
      0x0036920D,  //  01AE  ADD	R13	K73	R13
      0x58380011,  //  01AF  LDCONST	R14	K17
      0x7C2C0600,  //  01B0  CALL	R11	3
      0xA8040001,  //  01B1  EXBLK	1	1
      0x80040A00,  //  01B2  RET	1	R5
      0xA8040001,  //  01B3  EXBLK	1	1
      0x70020014,  //  01B4  JMP		#01CA
      0xAC180002,  //  01B5  CATCH	R6	0	2
      0x70020011,  //  01B6  JMP		#01C9
      0xB8220200,  //  01B7  GETNGBL	R8	K1
      0x8C201102,  //  01B8  GETMET	R8	R8	K2
      0x60280008,  //  01B9  GETGBL	R10	G8
      0x5C2C0C00,  //  01BA  MOVE	R11	R6
      0x7C280200,  //  01BB  CALL	R10	1
      0x002A940A,  //  01BC  ADD	R10	K74	R10
      0x0028154B,  //  01BD  ADD	R10	R10	K75
      0x602C0008,  //  01BE  GETGBL	R11	G8
      0x5C300E00,  //  01BF  MOVE	R12	R7
      0x7C2C0200,  //  01C0  CALL	R11	1
      0x0028140B,  //  01C1  ADD	R10	R10	R11
      0x7C200400,  //  01C2  CALL	R8	2
      0xA4229800,  //  01C3  IMPORT	R8	K76
      0x8C24114D,  //  01C4  GETMET	R9	R8	K77
      0x7C240200,  //  01C5  CALL	R9	1
      0x50240000,  //  01C6  LDBOOL	R9	0	0
      0x80041200,  //  01C7  RET	1	R9
      0x70020000,  //  01C8  JMP		#01CA
      0xB0080000,  //  01C9  RAISE	2	R0	R0
      0x80000000,  //  01CA  RET	0
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
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(commissioning, 5), be_const_var(1) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_MessageHandler_every_second_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_MessageHandler_every_250ms_closure) },
        { be_const_key_weak(device, 4), be_const_var(0) },
        { be_const_key_weak(im, 6), be_const_var(2) },
        { be_const_key_weak(msg_received, 2), be_const_closure(Matter_MessageHandler_msg_received_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_MessageHandler_init_closure) },
        { be_const_key_weak(send_response, -1), be_const_closure(Matter_MessageHandler_send_response_closure) },
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
