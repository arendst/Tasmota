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
    ( &(const bvalue[70]) {     /* constants */
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
    /* K20  */  be_nested_str_weak(__message_handler),
    /* K21  */  be_nested_str_weak(session),
    /* K22  */  be_nested_str_weak(counter_rcv),
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
    /* K40  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X20),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20frame_X3D),
    /* K42  */  be_nested_str_weak(inspect),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20rejected_X20duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K44  */  be_nested_str_weak(_X20counter_X3D),
    /* K45  */  be_nested_str_weak(decrypt),
    /* K46  */  be_nested_str_weak(raw),
    /* K47  */  be_nested_str_weak(payload_idx),
    /* K48  */  be_const_int(1),
    /* K49  */  be_nested_str_weak(MTR_X3A_X20idx_X3D_X25i_X20clear_X3D_X25s),
    /* K50  */  be_nested_str_weak(MTR_X3A_X20decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K51  */  be_nested_str_weak(protocol_id),
    /* K52  */  be_nested_str_weak(_X20opcode_X3D),
    /* K53  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K54  */  be_nested_str_weak(exchange_id),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20PROTOCOL_ID_SECURE_CHANNEL_X20),
    /* K56  */  be_nested_str_weak(im),
    /* K57  */  be_nested_str_weak(send_enqueued),
    /* K58  */  be_nested_str_weak(x_flag_r),
    /* K59  */  be_nested_str_weak(build_standalone_ack),
    /* K60  */  be_nested_str_weak(encode),
    /* K61  */  be_nested_str_weak(encrypt),
    /* K62  */  be_nested_str_weak(send_response),
    /* K63  */  be_nested_str_weak(remote_ip),
    /* K64  */  be_nested_str_weak(remote_port),
    /* K65  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K67  */  be_nested_str_weak(_X3B),
    /* K68  */  be_nested_str_weak(debug),
    /* K69  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[317]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x50140000,  //  0001  LDBOOL	R5	0	0
      0xA8020123,  //  0002  EXBLK	0	#0127
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
      0x7822005C,  //  001A  JMPF	R8	#0078
      0x88200D0A,  //  001B  GETMBR	R8	R6	K10
      0x1C201109,  //  001C  EQ	R8	R8	K9
      0x78220059,  //  001D  JMPF	R8	#0078
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
      0x88240116,  //  0037  GETMBR	R9	R0	K22
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
      0x883C0116,  //  0042  GETMBR	R15	R0	K22
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
      0x5C300400,  //  0071  MOVE	R12	R2
      0x5C340600,  //  0072  MOVE	R13	R3
      0x7C240800,  //  0073  CALL	R9	4
      0x50240200,  //  0074  LDBOOL	R9	1	0
      0xA8040001,  //  0075  EXBLK	1	1
      0x80041200,  //  0076  RET	1	R9
      0x700200AA,  //  0077  JMP		#0123
      0xB8220200,  //  0078  GETNGBL	R8	K1
      0x8C201102,  //  0079  GETMET	R8	R8	K2
      0x8C280919,  //  007A  GETMET	R10	R4	K25
      0x58300026,  //  007B  LDCONST	R12	K38
      0x88340D08,  //  007C  GETMBR	R13	R6	K8
      0x88380D18,  //  007D  GETMBR	R14	R6	K24
      0x7C280800,  //  007E  CALL	R10	4
      0x582C0011,  //  007F  LDCONST	R11	K17
      0x7C200600,  //  0080  CALL	R8	3
      0x8820010B,  //  0081  GETMBR	R8	R0	K11
      0x8820110C,  //  0082  GETMBR	R8	R8	K12
      0x8C201127,  //  0083  GETMET	R8	R8	K39
      0x88280D08,  //  0084  GETMBR	R10	R6	K8
      0x7C200400,  //  0085  CALL	R8	2
      0x4C240000,  //  0086  LDNIL	R9
      0x1C241009,  //  0087  EQ	R9	R8	R9
      0x78260013,  //  0088  JMPF	R9	#009D
      0xB8260200,  //  0089  GETNGBL	R9	K1
      0x8C241302,  //  008A  GETMET	R9	R9	K2
      0x602C0008,  //  008B  GETGBL	R11	G8
      0x88300D08,  //  008C  GETMBR	R12	R6	K8
      0x7C2C0200,  //  008D  CALL	R11	1
      0x002E500B,  //  008E  ADD	R11	K40	R11
      0x58300011,  //  008F  LDCONST	R12	K17
      0x7C240600,  //  0090  CALL	R9	3
      0xB8260200,  //  0091  GETNGBL	R9	K1
      0x8C241302,  //  0092  GETMET	R9	R9	K2
      0xB82E0A00,  //  0093  GETNGBL	R11	K5
      0x8C2C172A,  //  0094  GETMET	R11	R11	K42
      0x5C340C00,  //  0095  MOVE	R13	R6
      0x7C2C0400,  //  0096  CALL	R11	2
      0x002E520B,  //  0097  ADD	R11	K41	R11
      0x58300011,  //  0098  LDCONST	R12	K17
      0x7C240600,  //  0099  CALL	R9	3
      0x50240000,  //  009A  LDBOOL	R9	0	0
      0xA8040001,  //  009B  EXBLK	1	1
      0x80041200,  //  009C  RET	1	R9
      0x780A0000,  //  009D  JMPF	R2	#009F
      0x90222402,  //  009E  SETMBR	R8	K18	R2
      0x780E0000,  //  009F  JMPF	R3	#00A1
      0x90222603,  //  00A0  SETMBR	R8	K19	R3
      0x90222800,  //  00A1  SETMBR	R8	K20	R0
      0x901A2A08,  //  00A2  SETMBR	R6	K21	R8
      0x88241116,  //  00A3  GETMBR	R9	R8	K22
      0x8C241317,  //  00A4  GETMET	R9	R9	K23
      0x882C0D18,  //  00A5  GETMBR	R11	R6	K24
      0x50300200,  //  00A6  LDBOOL	R12	1	0
      0x7C240600,  //  00A7  CALL	R9	3
      0x74260011,  //  00A8  JMPT	R9	#00BB
      0xB8260200,  //  00A9  GETNGBL	R9	K1
      0x8C241302,  //  00AA  GETMET	R9	R9	K2
      0x602C0008,  //  00AB  GETGBL	R11	G8
      0x88300D18,  //  00AC  GETMBR	R12	R6	K24
      0x7C2C0200,  //  00AD  CALL	R11	1
      0x002E560B,  //  00AE  ADD	R11	K43	R11
      0x002C172C,  //  00AF  ADD	R11	R11	K44
      0x60300008,  //  00B0  GETGBL	R12	G8
      0x88341116,  //  00B1  GETMBR	R13	R8	K22
      0x8C341B1B,  //  00B2  GETMET	R13	R13	K27
      0x7C340200,  //  00B3  CALL	R13	1
      0x7C300200,  //  00B4  CALL	R12	1
      0x002C160C,  //  00B5  ADD	R11	R11	R12
      0x58300011,  //  00B6  LDCONST	R12	K17
      0x7C240600,  //  00B7  CALL	R9	3
      0x50240000,  //  00B8  LDBOOL	R9	0	0
      0xA8040001,  //  00B9  EXBLK	1	1
      0x80041200,  //  00BA  RET	1	R9
      0x8C240D2D,  //  00BB  GETMET	R9	R6	K45
      0x7C240200,  //  00BC  CALL	R9	1
      0x5C281200,  //  00BD  MOVE	R10	R9
      0x742A0002,  //  00BE  JMPT	R10	#00C2
      0x50280000,  //  00BF  LDBOOL	R10	0	0
      0xA8040001,  //  00C0  EXBLK	1	1
      0x80041400,  //  00C1  RET	1	R10
      0x88280D2F,  //  00C2  GETMBR	R10	R6	K47
      0x04281530,  //  00C3  SUB	R10	R10	K48
      0x402A120A,  //  00C4  CONNECT	R10	K9	R10
      0x882C0D2E,  //  00C5  GETMBR	R11	R6	K46
      0x9428160A,  //  00C6  GETIDX	R10	R11	R10
      0x901A5C0A,  //  00C7  SETMBR	R6	K46	R10
      0x88280D2E,  //  00C8  GETMBR	R10	R6	K46
      0x40281409,  //  00C9  CONNECT	R10	R10	R9
      0xB82A0200,  //  00CA  GETNGBL	R10	K1
      0x8C281502,  //  00CB  GETMET	R10	R10	K2
      0x8C300919,  //  00CC  GETMET	R12	R4	K25
      0x58380031,  //  00CD  LDCONST	R14	K49
      0x883C0D2F,  //  00CE  GETMBR	R15	R6	K47
      0x88400D2E,  //  00CF  GETMBR	R16	R6	K46
      0x8C402104,  //  00D0  GETMET	R16	R16	K4
      0x7C400200,  //  00D1  CALL	R16	1
      0x7C300800,  //  00D2  CALL	R12	4
      0x58340011,  //  00D3  LDCONST	R13	K17
      0x7C280600,  //  00D4  CALL	R10	3
      0x8C280D1C,  //  00D5  GETMET	R10	R6	K28
      0x7C280200,  //  00D6  CALL	R10	1
      0xB82A0200,  //  00D7  GETNGBL	R10	K1
      0x8C281502,  //  00D8  GETMET	R10	R10	K2
      0x60300008,  //  00D9  GETGBL	R12	G8
      0x88340D33,  //  00DA  GETMBR	R13	R6	K51
      0x7C300200,  //  00DB  CALL	R12	1
      0x0032640C,  //  00DC  ADD	R12	K50	R12
      0x00301934,  //  00DD  ADD	R12	R12	K52
      0x60340008,  //  00DE  GETGBL	R13	G8
      0x88380D1F,  //  00DF  GETMBR	R14	R6	K31
      0x7C340200,  //  00E0  CALL	R13	1
      0x0030180D,  //  00E1  ADD	R12	R12	R13
      0x00301935,  //  00E2  ADD	R12	R12	K53
      0x60340008,  //  00E3  GETGBL	R13	G8
      0x88380D36,  //  00E4  GETMBR	R14	R6	K54
      0x543EFFFE,  //  00E5  LDINT	R15	65535
      0x2C381C0F,  //  00E6  AND	R14	R14	R15
      0x7C340200,  //  00E7  CALL	R13	1
      0x0030180D,  //  00E8  ADD	R12	R12	R13
      0x58340011,  //  00E9  LDCONST	R13	K17
      0x7C280600,  //  00EA  CALL	R10	3
      0x8828010B,  //  00EB  GETMBR	R10	R0	K11
      0x8C28151D,  //  00EC  GETMET	R10	R10	K29
      0x88300D1E,  //  00ED  GETMBR	R12	R6	K30
      0x7C280400,  //  00EE  CALL	R10	2
      0x88280D33,  //  00EF  GETMBR	R10	R6	K51
      0x1C2C1509,  //  00F0  EQ	R11	R10	K9
      0x782E000A,  //  00F1  JMPF	R11	#00FD
      0xB82E0200,  //  00F2  GETNGBL	R11	K1
      0x8C2C1702,  //  00F3  GETMET	R11	R11	K2
      0xB8360A00,  //  00F4  GETNGBL	R13	K5
      0x8C341B2A,  //  00F5  GETMET	R13	R13	K42
      0x5C3C0C00,  //  00F6  MOVE	R15	R6
      0x7C340400,  //  00F7  CALL	R13	2
      0x00366E0D,  //  00F8  ADD	R13	K55	R13
      0x58380011,  //  00F9  LDCONST	R14	K17
      0x7C2C0600,  //  00FA  CALL	R11	3
      0x50140200,  //  00FB  LDBOOL	R5	1	0
      0x70020025,  //  00FC  JMP		#0123
      0x1C2C1530,  //  00FD  EQ	R11	R10	K48
      0x782E001B,  //  00FE  JMPF	R11	#011B
      0x882C0138,  //  00FF  GETMBR	R11	R0	K56
      0x8C2C1725,  //  0100  GETMET	R11	R11	K37
      0x5C340C00,  //  0101  MOVE	R13	R6
      0x5C380400,  //  0102  MOVE	R14	R2
      0x5C3C0600,  //  0103  MOVE	R15	R3
      0x7C2C0800,  //  0104  CALL	R11	4
      0x5C141600,  //  0105  MOVE	R5	R11
      0x78160004,  //  0106  JMPF	R5	#010C
      0x882C0138,  //  0107  GETMBR	R11	R0	K56
      0x8C2C1739,  //  0108  GETMET	R11	R11	K57
      0x5C340000,  //  0109  MOVE	R13	R0
      0x7C2C0400,  //  010A  CALL	R11	2
      0x7002000D,  //  010B  JMP		#011A
      0x882C0D3A,  //  010C  GETMBR	R11	R6	K58
      0x782E000B,  //  010D  JMPF	R11	#011A
      0x8C2C0D3B,  //  010E  GETMET	R11	R6	K59
      0x7C2C0200,  //  010F  CALL	R11	1
      0x8C30173C,  //  0110  GETMET	R12	R11	K60
      0x7C300200,  //  0111  CALL	R12	1
      0x8C30173D,  //  0112  GETMET	R12	R11	K61
      0x7C300200,  //  0113  CALL	R12	1
      0x8C30013E,  //  0114  GETMET	R12	R0	K62
      0x8838172E,  //  0115  GETMBR	R14	R11	K46
      0x883C173F,  //  0116  GETMBR	R15	R11	K63
      0x88401740,  //  0117  GETMBR	R16	R11	K64
      0x88441718,  //  0118  GETMBR	R17	R11	K24
      0x7C300A00,  //  0119  CALL	R12	5
      0x70020007,  //  011A  JMP		#0123
      0xB82E0200,  //  011B  GETNGBL	R11	K1
      0x8C2C1702,  //  011C  GETMET	R11	R11	K2
      0x60340008,  //  011D  GETGBL	R13	G8
      0x5C381400,  //  011E  MOVE	R14	R10
      0x7C340200,  //  011F  CALL	R13	1
      0x0036820D,  //  0120  ADD	R13	K65	R13
      0x58380011,  //  0121  LDCONST	R14	K17
      0x7C2C0600,  //  0122  CALL	R11	3
      0xA8040001,  //  0123  EXBLK	1	1
      0x80040A00,  //  0124  RET	1	R5
      0xA8040001,  //  0125  EXBLK	1	1
      0x70020014,  //  0126  JMP		#013C
      0xAC180002,  //  0127  CATCH	R6	0	2
      0x70020011,  //  0128  JMP		#013B
      0xB8220200,  //  0129  GETNGBL	R8	K1
      0x8C201102,  //  012A  GETMET	R8	R8	K2
      0x60280008,  //  012B  GETGBL	R10	G8
      0x5C2C0C00,  //  012C  MOVE	R11	R6
      0x7C280200,  //  012D  CALL	R10	1
      0x002A840A,  //  012E  ADD	R10	K66	R10
      0x00281543,  //  012F  ADD	R10	R10	K67
      0x602C0008,  //  0130  GETGBL	R11	G8
      0x5C300E00,  //  0131  MOVE	R12	R7
      0x7C2C0200,  //  0132  CALL	R11	1
      0x0028140B,  //  0133  ADD	R10	R10	R11
      0x7C200400,  //  0134  CALL	R8	2
      0xA4228800,  //  0135  IMPORT	R8	K68
      0x8C241145,  //  0136  GETMET	R9	R8	K69
      0x7C240200,  //  0137  CALL	R9	1
      0x50240000,  //  0138  LDBOOL	R9	0	0
      0x80041200,  //  0139  RET	1	R9
      0x70020000,  //  013A  JMP		#013C
      0xB0080000,  //  013B  RAISE	2	R0	R0
      0x80000000,  //  013C  RET	0
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
** Solidified class: Matter_MessageHandler
********************************************************************/
be_local_class(Matter_MessageHandler,
    4,
    NULL,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(commissioning, 2), be_const_var(1) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_MessageHandler_every_250ms_closure) },
        { be_const_key_weak(counter_rcv, -1), be_const_var(3) },
        { be_const_key_weak(add_session, -1), be_const_closure(Matter_MessageHandler_add_session_closure) },
        { be_const_key_weak(init, 9), be_const_closure(Matter_MessageHandler_init_closure) },
        { be_const_key_weak(im, 4), be_const_var(2) },
        { be_const_key_weak(send_response, 6), be_const_closure(Matter_MessageHandler_send_response_closure) },
        { be_const_key_weak(msg_received, -1), be_const_closure(Matter_MessageHandler_msg_received_closure) },
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
