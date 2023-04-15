/* Solidification of Matter_MessageHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_MessageHandler;

/********************************************************************
** Solidified function: send_response_frame
********************************************************************/
be_local_closure(Matter_MessageHandler_send_response_frame,   /* name */
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
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(msg_send),
    }),
    be_str_weak(send_response_frame),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_encrypted_ack
********************************************************************/
be_local_closure(Matter_MessageHandler_send_encrypted_ack,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(x_flag_r),
    /* K2   */  be_nested_str_weak(build_standalone_ack),
    /* K3   */  be_nested_str_weak(encode_frame),
    /* K4   */  be_nested_str_weak(encrypt),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(format),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X2A_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X25s),
    /* K9   */  be_nested_str_weak(session),
    /* K10  */  be_nested_str_weak(local_session_id),
    /* K11  */  be_nested_str_weak(ack_message_counter),
    /* K12  */  be_nested_str_weak(message_counter),
    /* K13  */  be_nested_str_weak(_X7Breliable_X7D),
    /* K14  */  be_nested_str_weak(),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(send_encrypted_ack),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x88100301,  //  0001  GETMBR	R4	R1	K1
      0x78120018,  //  0002  JMPF	R4	#001C
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x5C180400,  //  0004  MOVE	R6	R2
      0x7C100400,  //  0005  CALL	R4	2
      0x8C140903,  //  0006  GETMET	R5	R4	K3
      0x7C140200,  //  0007  CALL	R5	1
      0x8C140904,  //  0008  GETMET	R5	R4	K4
      0x7C140200,  //  0009  CALL	R5	1
      0xB8160A00,  //  000A  GETNGBL	R5	K5
      0x8C140B06,  //  000B  GETMET	R5	R5	K6
      0x8C1C0707,  //  000C  GETMET	R7	R3	K7
      0x58240008,  //  000D  LDCONST	R9	K8
      0x88280909,  //  000E  GETMBR	R10	R4	K9
      0x8828150A,  //  000F  GETMBR	R10	R10	K10
      0x882C090B,  //  0010  GETMBR	R11	R4	K11
      0x8830090C,  //  0011  GETMBR	R12	R4	K12
      0x780A0001,  //  0012  JMPF	R2	#0015
      0x5834000D,  //  0013  LDCONST	R13	K13
      0x70020000,  //  0014  JMP		#0016
      0x5834000E,  //  0015  LDCONST	R13	K14
      0x7C1C0C00,  //  0016  CALL	R7	6
      0x5820000F,  //  0017  LDCONST	R8	K15
      0x7C140600,  //  0018  CALL	R5	3
      0x8C140110,  //  0019  GETMET	R5	R0	K16
      0x5C1C0800,  //  001A  MOVE	R7	R4
      0x7C140400,  //  001B  CALL	R5	2
      0x80000000,  //  001C  RET	0
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
** Solidified function: send_simple_ack
********************************************************************/
be_local_closure(Matter_MessageHandler_send_simple_ack,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(x_flag_r),
    /* K2   */  be_nested_str_weak(build_standalone_ack),
    /* K3   */  be_nested_str_weak(encode_frame),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(format),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X25s),
    /* K8   */  be_nested_str_weak(session),
    /* K9   */  be_nested_str_weak(local_session_id),
    /* K10  */  be_nested_str_weak(ack_message_counter),
    /* K11  */  be_nested_str_weak(message_counter),
    /* K12  */  be_nested_str_weak(_X7Breliable_X7D),
    /* K13  */  be_nested_str_weak(),
    /* K14  */  be_const_int(3),
    /* K15  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(send_simple_ack),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x88100301,  //  0001  GETMBR	R4	R1	K1
      0x78120016,  //  0002  JMPF	R4	#001A
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x5C180400,  //  0004  MOVE	R6	R2
      0x7C100400,  //  0005  CALL	R4	2
      0x8C140903,  //  0006  GETMET	R5	R4	K3
      0x7C140200,  //  0007  CALL	R5	1
      0xB8160800,  //  0008  GETNGBL	R5	K4
      0x8C140B05,  //  0009  GETMET	R5	R5	K5
      0x8C1C0706,  //  000A  GETMET	R7	R3	K6
      0x58240007,  //  000B  LDCONST	R9	K7
      0x88280908,  //  000C  GETMBR	R10	R4	K8
      0x88281509,  //  000D  GETMBR	R10	R10	K9
      0x882C090A,  //  000E  GETMBR	R11	R4	K10
      0x8830090B,  //  000F  GETMBR	R12	R4	K11
      0x780A0001,  //  0010  JMPF	R2	#0013
      0x5834000C,  //  0011  LDCONST	R13	K12
      0x70020000,  //  0012  JMP		#0014
      0x5834000D,  //  0013  LDCONST	R13	K13
      0x7C1C0C00,  //  0014  CALL	R7	6
      0x5820000E,  //  0015  LDCONST	R8	K14
      0x7C140600,  //  0016  CALL	R5	3
      0x8C14010F,  //  0017  GETMET	R5	R0	K15
      0x5C1C0800,  //  0018  MOVE	R7	R4
      0x7C140400,  //  0019  CALL	R5	2
      0x80000000,  //  001A  RET	0
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
    ( &(const bvalue[76]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20raw_X3D),
    /* K4   */  be_nested_str_weak(tohex),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(Frame),
    /* K7   */  be_nested_str_weak(decode_header),
    /* K8   */  be_nested_str_weak(sec_p),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20CONTROL_X20MESSAGE_X3D),
    /* K10  */  be_nested_str_weak(inspect),
    /* K11  */  be_nested_str_weak(device),
    /* K12  */  be_nested_str_weak(sessions),
    /* K13  */  be_nested_str_weak(find_session_source_id_unsecure),
    /* K14  */  be_nested_str_weak(source_node_id),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20find_X20session_X20by_X20source_node_id_X20_X3D_X20),
    /* K16  */  be_nested_str_weak(_X20session_id_X20_X3D_X20),
    /* K17  */  be_nested_str_weak(local_session_id),
    /* K18  */  be_const_int(2),
    /* K19  */  be_nested_str_weak(control_message),
    /* K20  */  be_nested_str_weak(process_incoming_control_message),
    /* K21  */  be_const_int(0),
    /* K22  */  be_nested_str_weak(sec_sesstype),
    /* K23  */  be_const_int(3),
    /* K24  */  be_nested_str_weak(_ip),
    /* K25  */  be_nested_str_weak(_port),
    /* K26  */  be_nested_str_weak(_message_handler),
    /* K27  */  be_nested_str_weak(session),
    /* K28  */  be_nested_str_weak(_counter_insecure_rcv),
    /* K29  */  be_nested_str_weak(validate),
    /* K30  */  be_nested_str_weak(message_counter),
    /* K31  */  be_nested_str_weak(format),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K33  */  be_nested_str_weak(val),
    /* K34  */  be_nested_str_weak(send_simple_ack),
    /* K35  */  be_nested_str_weak(decode_payload),
    /* K36  */  be_nested_str_weak(received_ack),
    /* K37  */  be_nested_str_weak(opcode),
    /* K38  */  be_nested_str_weak(get_opcode_name),
    /* K39  */  be_nested_str_weak(0x_X2502X),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20rid_X3D_X25i_X20exch_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K41  */  be_nested_str_weak(exchange_id),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K43  */  be_nested_str_weak(x_flag_r),
    /* K44  */  be_nested_str_weak(_X7Breliable_X7D_X20),
    /* K45  */  be_nested_str_weak(),
    /* K46  */  be_nested_str_weak(ack_message_counter),
    /* K47  */  be_nested_str_weak(commissioning),
    /* K48  */  be_nested_str_weak(process_incoming),
    /* K49  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K50  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K51  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
    /* K52  */  be_nested_str_weak(counter_rcv_validate),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K54  */  be_nested_str_weak(_X20counter_X3D),
    /* K55  */  be_nested_str_weak(counter_rcv),
    /* K56  */  be_nested_str_weak(send_encrypted_ack),
    /* K57  */  be_nested_str_weak(decrypt),
    /* K58  */  be_nested_str_weak(raw),
    /* K59  */  be_nested_str_weak(payload_idx),
    /* K60  */  be_const_int(1),
    /* K61  */  be_nested_str_weak(MTR_X3A_X20idx_X3D_X25i_X20clear_X3D_X25s),
    /* K62  */  be_nested_str_weak(MTR_X3A_X20_X3E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K63  */  be_nested_str_weak(protocol_id),
    /* K64  */  be_nested_str_weak(_X20opcode_X3D),
    /* K65  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20_X5B_X2502X_X2F_X2502X_X5D_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K67  */  be_nested_str_weak(MTR_X3A_X20PROTOCOL_ID_SECURE_CHANNEL_X20),
    /* K68  */  be_nested_str_weak(im),
    /* K69  */  be_nested_str_weak(process_incoming_ack),
    /* K70  */  be_nested_str_weak(send_enqueued),
    /* K71  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K73  */  be_nested_str_weak(_X3B),
    /* K74  */  be_nested_str_weak(debug),
    /* K75  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[403]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x50140000,  //  0001  LDBOOL	R5	0	0
      0xA8020179,  //  0002  EXBLK	0	#017D
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
      0x78220021,  //  0019  JMPF	R8	#003C
      0xB8220200,  //  001A  GETNGBL	R8	K1
      0x8C201102,  //  001B  GETMET	R8	R8	K2
      0xB82A0A00,  //  001C  GETNGBL	R10	K5
      0x8C28150A,  //  001D  GETMET	R10	R10	K10
      0x5C300C00,  //  001E  MOVE	R12	R6
      0x7C280400,  //  001F  CALL	R10	2
      0x002A120A,  //  0020  ADD	R10	K9	R10
      0x7C200400,  //  0021  CALL	R8	2
      0x8820010B,  //  0022  GETMBR	R8	R0	K11
      0x8820110C,  //  0023  GETMBR	R8	R8	K12
      0x8C20110D,  //  0024  GETMET	R8	R8	K13
      0x88280D0E,  //  0025  GETMBR	R10	R6	K14
      0x542E0059,  //  0026  LDINT	R11	90
      0x7C200600,  //  0027  CALL	R8	3
      0xB8260200,  //  0028  GETNGBL	R9	K1
      0x8C241302,  //  0029  GETMET	R9	R9	K2
      0x602C0008,  //  002A  GETGBL	R11	G8
      0x88300D0E,  //  002B  GETMBR	R12	R6	K14
      0x7C2C0200,  //  002C  CALL	R11	1
      0x002E1E0B,  //  002D  ADD	R11	K15	R11
      0x002C1710,  //  002E  ADD	R11	R11	K16
      0x60300008,  //  002F  GETGBL	R12	G8
      0x88341111,  //  0030  GETMBR	R13	R8	K17
      0x7C300200,  //  0031  CALL	R12	1
      0x002C160C,  //  0032  ADD	R11	R11	R12
      0x58300012,  //  0033  LDCONST	R12	K18
      0x7C240600,  //  0034  CALL	R9	3
      0x88240113,  //  0035  GETMBR	R9	R0	K19
      0x8C241314,  //  0036  GETMET	R9	R9	K20
      0x5C2C0C00,  //  0037  MOVE	R11	R6
      0x7C240400,  //  0038  CALL	R9	2
      0xA8040001,  //  0039  EXBLK	1	1
      0x80041200,  //  003A  RET	1	R9
      0x7002013C,  //  003B  JMP		#0179
      0x88200D11,  //  003C  GETMBR	R8	R6	K17
      0x1C201115,  //  003D  EQ	R8	R8	K21
      0x7822007D,  //  003E  JMPF	R8	#00BD
      0x88200D16,  //  003F  GETMBR	R8	R6	K22
      0x1C201115,  //  0040  EQ	R8	R8	K21
      0x7822007A,  //  0041  JMPF	R8	#00BD
      0x8820010B,  //  0042  GETMBR	R8	R0	K11
      0x8820110C,  //  0043  GETMBR	R8	R8	K12
      0x8C20110D,  //  0044  GETMET	R8	R8	K13
      0x88280D0E,  //  0045  GETMBR	R10	R6	K14
      0x542E0059,  //  0046  LDINT	R11	90
      0x7C200600,  //  0047  CALL	R8	3
      0xB8260200,  //  0048  GETNGBL	R9	K1
      0x8C241302,  //  0049  GETMET	R9	R9	K2
      0x602C0008,  //  004A  GETGBL	R11	G8
      0x88300D0E,  //  004B  GETMBR	R12	R6	K14
      0x7C2C0200,  //  004C  CALL	R11	1
      0x002E1E0B,  //  004D  ADD	R11	K15	R11
      0x002C1710,  //  004E  ADD	R11	R11	K16
      0x60300008,  //  004F  GETGBL	R12	G8
      0x88341111,  //  0050  GETMBR	R13	R8	K17
      0x7C300200,  //  0051  CALL	R12	1
      0x002C160C,  //  0052  ADD	R11	R11	R12
      0x58300017,  //  0053  LDCONST	R12	K23
      0x7C240600,  //  0054  CALL	R9	3
      0x780A0000,  //  0055  JMPF	R2	#0057
      0x90223002,  //  0056  SETMBR	R8	K24	R2
      0x780E0000,  //  0057  JMPF	R3	#0059
      0x90223203,  //  0058  SETMBR	R8	K25	R3
      0x90223400,  //  0059  SETMBR	R8	K26	R0
      0x901A3608,  //  005A  SETMBR	R6	K27	R8
      0x8824111C,  //  005B  GETMBR	R9	R8	K28
      0x8C24131D,  //  005C  GETMET	R9	R9	K29
      0x882C0D1E,  //  005D  GETMBR	R11	R6	K30
      0x50300000,  //  005E  LDBOOL	R12	0	0
      0x7C240600,  //  005F  CALL	R9	3
      0x74260011,  //  0060  JMPT	R9	#0073
      0xB8260200,  //  0061  GETNGBL	R9	K1
      0x8C241302,  //  0062  GETMET	R9	R9	K2
      0x8C2C091F,  //  0063  GETMET	R11	R4	K31
      0x58340020,  //  0064  LDCONST	R13	K32
      0x88380D1E,  //  0065  GETMBR	R14	R6	K30
      0x883C111C,  //  0066  GETMBR	R15	R8	K28
      0x8C3C1F21,  //  0067  GETMET	R15	R15	K33
      0x7C3C0200,  //  0068  CALL	R15	1
      0x7C2C0800,  //  0069  CALL	R11	4
      0x58300017,  //  006A  LDCONST	R12	K23
      0x7C240600,  //  006B  CALL	R9	3
      0x8C240122,  //  006C  GETMET	R9	R0	K34
      0x5C2C0C00,  //  006D  MOVE	R11	R6
      0x50300000,  //  006E  LDBOOL	R12	0	0
      0x7C240600,  //  006F  CALL	R9	3
      0x50240000,  //  0070  LDBOOL	R9	0	0
      0xA8040001,  //  0071  EXBLK	1	1
      0x80041200,  //  0072  RET	1	R9
      0x8C240D23,  //  0073  GETMET	R9	R6	K35
      0x7C240200,  //  0074  CALL	R9	1
      0x74260002,  //  0075  JMPT	R9	#0079
      0x50240000,  //  0076  LDBOOL	R9	0	0
      0xA8040001,  //  0077  EXBLK	1	1
      0x80041200,  //  0078  RET	1	R9
      0x8824010B,  //  0079  GETMBR	R9	R0	K11
      0x8C241324,  //  007A  GETMET	R9	R9	K36
      0x5C2C0C00,  //  007B  MOVE	R11	R6
      0x7C240400,  //  007C  CALL	R9	2
      0x88240D25,  //  007D  GETMBR	R9	R6	K37
      0x542A000F,  //  007E  LDINT	R10	16
      0x2024120A,  //  007F  NE	R9	R9	R10
      0x78260018,  //  0080  JMPF	R9	#009A
      0xB8260A00,  //  0081  GETNGBL	R9	K5
      0x8C241326,  //  0082  GETMET	R9	R9	K38
      0x882C0D25,  //  0083  GETMBR	R11	R6	K37
      0x7C240400,  //  0084  CALL	R9	2
      0x5C281200,  //  0085  MOVE	R10	R9
      0x742A0004,  //  0086  JMPT	R10	#008C
      0x8C28091F,  //  0087  GETMET	R10	R4	K31
      0x58300027,  //  0088  LDCONST	R12	K39
      0x88340D25,  //  0089  GETMBR	R13	R6	K37
      0x7C280600,  //  008A  CALL	R10	3
      0x5C241400,  //  008B  MOVE	R9	R10
      0xB82A0200,  //  008C  GETNGBL	R10	K1
      0x8C281502,  //  008D  GETMET	R10	R10	K2
      0x8C30091F,  //  008E  GETMET	R12	R4	K31
      0x58380028,  //  008F  LDCONST	R14	K40
      0x883C1111,  //  0090  GETMBR	R15	R8	K17
      0x5C401200,  //  0091  MOVE	R16	R9
      0x88440D1E,  //  0092  GETMBR	R17	R6	K30
      0x88480D29,  //  0093  GETMBR	R18	R6	K41
      0x5C4C0400,  //  0094  MOVE	R19	R2
      0x5C500600,  //  0095  MOVE	R20	R3
      0x7C301000,  //  0096  CALL	R12	8
      0x58340012,  //  0097  LDCONST	R13	K18
      0x7C280600,  //  0098  CALL	R10	3
      0x70020013,  //  0099  JMP		#00AE
      0xB8260200,  //  009A  GETNGBL	R9	K1
      0x8C241302,  //  009B  GETMET	R9	R9	K2
      0x8C2C091F,  //  009C  GETMET	R11	R4	K31
      0x5834002A,  //  009D  LDCONST	R13	K42
      0x88381111,  //  009E  GETMBR	R14	R8	K17
      0x883C0D1E,  //  009F  GETMBR	R15	R6	K30
      0x88400D2B,  //  00A0  GETMBR	R16	R6	K43
      0x78420001,  //  00A1  JMPF	R16	#00A4
      0x5840002C,  //  00A2  LDCONST	R16	K44
      0x70020000,  //  00A3  JMP		#00A5
      0x5840002D,  //  00A4  LDCONST	R16	K45
      0x88440D29,  //  00A5  GETMBR	R17	R6	K41
      0x60480008,  //  00A6  GETGBL	R18	G8
      0x884C0D2E,  //  00A7  GETMBR	R19	R6	K46
      0x7C480200,  //  00A8  CALL	R18	1
      0x5C4C0400,  //  00A9  MOVE	R19	R2
      0x5C500600,  //  00AA  MOVE	R20	R3
      0x7C2C1200,  //  00AB  CALL	R11	9
      0x58300017,  //  00AC  LDCONST	R12	K23
      0x7C240600,  //  00AD  CALL	R9	3
      0x8824012F,  //  00AE  GETMBR	R9	R0	K47
      0x8C241330,  //  00AF  GETMET	R9	R9	K48
      0x5C2C0C00,  //  00B0  MOVE	R11	R6
      0x7C240400,  //  00B1  CALL	R9	2
      0x5C141200,  //  00B2  MOVE	R5	R9
      0x5C240A00,  //  00B3  MOVE	R9	R5
      0x74260003,  //  00B4  JMPT	R9	#00B9
      0x8C240122,  //  00B5  GETMET	R9	R0	K34
      0x5C2C0C00,  //  00B6  MOVE	R11	R6
      0x50300000,  //  00B7  LDBOOL	R12	0	0
      0x7C240600,  //  00B8  CALL	R9	3
      0x50240200,  //  00B9  LDBOOL	R9	1	0
      0xA8040001,  //  00BA  EXBLK	1	1
      0x80041200,  //  00BB  RET	1	R9
      0x700200BB,  //  00BC  JMP		#0179
      0xB8220200,  //  00BD  GETNGBL	R8	K1
      0x8C201102,  //  00BE  GETMET	R8	R8	K2
      0x8C28091F,  //  00BF  GETMET	R10	R4	K31
      0x58300031,  //  00C0  LDCONST	R12	K49
      0x88340D11,  //  00C1  GETMBR	R13	R6	K17
      0x88380D1E,  //  00C2  GETMBR	R14	R6	K30
      0x7C280800,  //  00C3  CALL	R10	4
      0x582C0017,  //  00C4  LDCONST	R11	K23
      0x7C200600,  //  00C5  CALL	R8	3
      0x8820010B,  //  00C6  GETMBR	R8	R0	K11
      0x8820110C,  //  00C7  GETMBR	R8	R8	K12
      0x8C201132,  //  00C8  GETMET	R8	R8	K50
      0x88280D11,  //  00C9  GETMBR	R10	R6	K17
      0x7C200400,  //  00CA  CALL	R8	2
      0x4C240000,  //  00CB  LDNIL	R9
      0x1C241009,  //  00CC  EQ	R9	R8	R9
      0x7826000A,  //  00CD  JMPF	R9	#00D9
      0xB8260200,  //  00CE  GETNGBL	R9	K1
      0x8C241302,  //  00CF  GETMET	R9	R9	K2
      0x602C0008,  //  00D0  GETGBL	R11	G8
      0x88300D11,  //  00D1  GETMBR	R12	R6	K17
      0x7C2C0200,  //  00D2  CALL	R11	1
      0x002E660B,  //  00D3  ADD	R11	K51	R11
      0x58300012,  //  00D4  LDCONST	R12	K18
      0x7C240600,  //  00D5  CALL	R9	3
      0x50240000,  //  00D6  LDBOOL	R9	0	0
      0xA8040001,  //  00D7  EXBLK	1	1
      0x80041200,  //  00D8  RET	1	R9
      0x780A0000,  //  00D9  JMPF	R2	#00DB
      0x90223002,  //  00DA  SETMBR	R8	K24	R2
      0x780E0000,  //  00DB  JMPF	R3	#00DD
      0x90223203,  //  00DC  SETMBR	R8	K25	R3
      0x90223400,  //  00DD  SETMBR	R8	K26	R0
      0x901A3608,  //  00DE  SETMBR	R6	K27	R8
      0x8C241134,  //  00DF  GETMET	R9	R8	K52
      0x882C0D1E,  //  00E0  GETMBR	R11	R6	K30
      0x50300200,  //  00E1  LDBOOL	R12	1	0
      0x7C240600,  //  00E2  CALL	R9	3
      0x74260013,  //  00E3  JMPT	R9	#00F8
      0xB8260200,  //  00E4  GETNGBL	R9	K1
      0x8C241302,  //  00E5  GETMET	R9	R9	K2
      0x602C0008,  //  00E6  GETGBL	R11	G8
      0x88300D1E,  //  00E7  GETMBR	R12	R6	K30
      0x7C2C0200,  //  00E8  CALL	R11	1
      0x002E6A0B,  //  00E9  ADD	R11	K53	R11
      0x002C1736,  //  00EA  ADD	R11	R11	K54
      0x60300008,  //  00EB  GETGBL	R12	G8
      0x88341137,  //  00EC  GETMBR	R13	R8	K55
      0x7C300200,  //  00ED  CALL	R12	1
      0x002C160C,  //  00EE  ADD	R11	R11	R12
      0x58300017,  //  00EF  LDCONST	R12	K23
      0x7C240600,  //  00F0  CALL	R9	3
      0x8C240138,  //  00F1  GETMET	R9	R0	K56
      0x5C2C0C00,  //  00F2  MOVE	R11	R6
      0x50300000,  //  00F3  LDBOOL	R12	0	0
      0x7C240600,  //  00F4  CALL	R9	3
      0x50240000,  //  00F5  LDBOOL	R9	0	0
      0xA8040001,  //  00F6  EXBLK	1	1
      0x80041200,  //  00F7  RET	1	R9
      0x8C240D39,  //  00F8  GETMET	R9	R6	K57
      0x7C240200,  //  00F9  CALL	R9	1
      0x5C281200,  //  00FA  MOVE	R10	R9
      0x742A0002,  //  00FB  JMPT	R10	#00FF
      0x50280000,  //  00FC  LDBOOL	R10	0	0
      0xA8040001,  //  00FD  EXBLK	1	1
      0x80041400,  //  00FE  RET	1	R10
      0x88280D3B,  //  00FF  GETMBR	R10	R6	K59
      0x0428153C,  //  0100  SUB	R10	R10	K60
      0x402A2A0A,  //  0101  CONNECT	R10	K21	R10
      0x882C0D3A,  //  0102  GETMBR	R11	R6	K58
      0x9428160A,  //  0103  GETIDX	R10	R11	R10
      0x901A740A,  //  0104  SETMBR	R6	K58	R10
      0x88280D3A,  //  0105  GETMBR	R10	R6	K58
      0x40281409,  //  0106  CONNECT	R10	R10	R9
      0xB82A0200,  //  0107  GETNGBL	R10	K1
      0x8C281502,  //  0108  GETMET	R10	R10	K2
      0x8C30091F,  //  0109  GETMET	R12	R4	K31
      0x5838003D,  //  010A  LDCONST	R14	K61
      0x883C0D3B,  //  010B  GETMBR	R15	R6	K59
      0x88400D3A,  //  010C  GETMBR	R16	R6	K58
      0x8C402104,  //  010D  GETMET	R16	R16	K4
      0x7C400200,  //  010E  CALL	R16	1
      0x7C300800,  //  010F  CALL	R12	4
      0x54360003,  //  0110  LDINT	R13	4
      0x7C280600,  //  0111  CALL	R10	3
      0x8C280D23,  //  0112  GETMET	R10	R6	K35
      0x7C280200,  //  0113  CALL	R10	1
      0xB82A0200,  //  0114  GETNGBL	R10	K1
      0x8C281502,  //  0115  GETMET	R10	R10	K2
      0x60300008,  //  0116  GETGBL	R12	G8
      0x88340D3F,  //  0117  GETMBR	R13	R6	K63
      0x7C300200,  //  0118  CALL	R12	1
      0x00327C0C,  //  0119  ADD	R12	K62	R12
      0x00301940,  //  011A  ADD	R12	R12	K64
      0x60340008,  //  011B  GETGBL	R13	G8
      0x88380D25,  //  011C  GETMBR	R14	R6	K37
      0x7C340200,  //  011D  CALL	R13	1
      0x0030180D,  //  011E  ADD	R12	R12	R13
      0x00301941,  //  011F  ADD	R12	R12	K65
      0x60340008,  //  0120  GETGBL	R13	G8
      0x88380D29,  //  0121  GETMBR	R14	R6	K41
      0x543EFFFE,  //  0122  LDINT	R15	65535
      0x2C381C0F,  //  0123  AND	R14	R14	R15
      0x7C340200,  //  0124  CALL	R13	1
      0x0030180D,  //  0125  ADD	R12	R12	R13
      0x58340017,  //  0126  LDCONST	R13	K23
      0x7C280600,  //  0127  CALL	R10	3
      0xB82A0200,  //  0128  GETNGBL	R10	K1
      0x8C281502,  //  0129  GETMET	R10	R10	K2
      0x8C30091F,  //  012A  GETMET	R12	R4	K31
      0x58380042,  //  012B  LDCONST	R14	K66
      0x883C1111,  //  012C  GETMBR	R15	R8	K17
      0x88400D3F,  //  012D  GETMBR	R16	R6	K63
      0x88440D25,  //  012E  GETMBR	R17	R6	K37
      0x88480D1E,  //  012F  GETMBR	R18	R6	K30
      0x884C0D29,  //  0130  GETMBR	R19	R6	K41
      0x60500008,  //  0131  GETGBL	R20	G8
      0x88540D2E,  //  0132  GETMBR	R21	R6	K46
      0x7C500200,  //  0133  CALL	R20	1
      0x88540D2B,  //  0134  GETMBR	R21	R6	K43
      0x78560001,  //  0135  JMPF	R21	#0138
      0x5854002C,  //  0136  LDCONST	R21	K44
      0x70020000,  //  0137  JMP		#0139
      0x5854002D,  //  0138  LDCONST	R21	K45
      0x5C580400,  //  0139  MOVE	R22	R2
      0x5C5C0600,  //  013A  MOVE	R23	R3
      0x7C301600,  //  013B  CALL	R12	11
      0x58340017,  //  013C  LDCONST	R13	K23
      0x7C280600,  //  013D  CALL	R10	3
      0x8828010B,  //  013E  GETMBR	R10	R0	K11
      0x8C281524,  //  013F  GETMET	R10	R10	K36
      0x5C300C00,  //  0140  MOVE	R12	R6
      0x7C280400,  //  0141  CALL	R10	2
      0x88280D3F,  //  0142  GETMBR	R10	R6	K63
      0x1C2C1515,  //  0143  EQ	R11	R10	K21
      0x782E0018,  //  0144  JMPF	R11	#015E
      0xB82E0200,  //  0145  GETNGBL	R11	K1
      0x8C2C1702,  //  0146  GETMET	R11	R11	K2
      0xB8360A00,  //  0147  GETNGBL	R13	K5
      0x8C341B0A,  //  0148  GETMET	R13	R13	K10
      0x5C3C0C00,  //  0149  MOVE	R15	R6
      0x7C340400,  //  014A  CALL	R13	2
      0x0036860D,  //  014B  ADD	R13	K67	R13
      0x58380017,  //  014C  LDCONST	R14	K23
      0x7C2C0600,  //  014D  CALL	R11	3
      0x882C0D25,  //  014E  GETMBR	R11	R6	K37
      0x5432000F,  //  014F  LDINT	R12	16
      0x1C2C160C,  //  0150  EQ	R11	R11	R12
      0x782E0009,  //  0151  JMPF	R11	#015C
      0x882C0144,  //  0152  GETMBR	R11	R0	K68
      0x8C2C1745,  //  0153  GETMET	R11	R11	K69
      0x5C340C00,  //  0154  MOVE	R13	R6
      0x7C2C0400,  //  0155  CALL	R11	2
      0x5C141600,  //  0156  MOVE	R5	R11
      0x78160003,  //  0157  JMPF	R5	#015C
      0x882C0144,  //  0158  GETMBR	R11	R0	K68
      0x8C2C1746,  //  0159  GETMET	R11	R11	K70
      0x5C340000,  //  015A  MOVE	R13	R0
      0x7C2C0400,  //  015B  CALL	R11	2
      0x50140200,  //  015C  LDBOOL	R5	1	0
      0x7002001A,  //  015D  JMP		#0179
      0x1C2C153C,  //  015E  EQ	R11	R10	K60
      0x782E0010,  //  015F  JMPF	R11	#0171
      0x882C0144,  //  0160  GETMBR	R11	R0	K68
      0x8C2C1730,  //  0161  GETMET	R11	R11	K48
      0x5C340C00,  //  0162  MOVE	R13	R6
      0x7C2C0400,  //  0163  CALL	R11	2
      0x5C141600,  //  0164  MOVE	R5	R11
      0x78160004,  //  0165  JMPF	R5	#016B
      0x882C0144,  //  0166  GETMBR	R11	R0	K68
      0x8C2C1746,  //  0167  GETMET	R11	R11	K70
      0x5C340000,  //  0168  MOVE	R13	R0
      0x7C2C0400,  //  0169  CALL	R11	2
      0x70020003,  //  016A  JMP		#016F
      0x8C2C0138,  //  016B  GETMET	R11	R0	K56
      0x5C340C00,  //  016C  MOVE	R13	R6
      0x50380200,  //  016D  LDBOOL	R14	1	0
      0x7C2C0600,  //  016E  CALL	R11	3
      0x50140200,  //  016F  LDBOOL	R5	1	0
      0x70020007,  //  0170  JMP		#0179
      0xB82E0200,  //  0171  GETNGBL	R11	K1
      0x8C2C1702,  //  0172  GETMET	R11	R11	K2
      0x60340008,  //  0173  GETGBL	R13	G8
      0x5C381400,  //  0174  MOVE	R14	R10
      0x7C340200,  //  0175  CALL	R13	1
      0x00368E0D,  //  0176  ADD	R13	K71	R13
      0x58380017,  //  0177  LDCONST	R14	K23
      0x7C2C0600,  //  0178  CALL	R11	3
      0xA8040001,  //  0179  EXBLK	1	1
      0x80040A00,  //  017A  RET	1	R5
      0xA8040001,  //  017B  EXBLK	1	1
      0x70020014,  //  017C  JMP		#0192
      0xAC180002,  //  017D  CATCH	R6	0	2
      0x70020011,  //  017E  JMP		#0191
      0xB8220200,  //  017F  GETNGBL	R8	K1
      0x8C201102,  //  0180  GETMET	R8	R8	K2
      0x60280008,  //  0181  GETGBL	R10	G8
      0x5C2C0C00,  //  0182  MOVE	R11	R6
      0x7C280200,  //  0183  CALL	R10	1
      0x002A900A,  //  0184  ADD	R10	K72	R10
      0x00281549,  //  0185  ADD	R10	R10	K73
      0x602C0008,  //  0186  GETGBL	R11	G8
      0x5C300E00,  //  0187  MOVE	R12	R7
      0x7C2C0200,  //  0188  CALL	R11	1
      0x0028140B,  //  0189  ADD	R10	R10	R11
      0x7C200400,  //  018A  CALL	R8	2
      0xA4229400,  //  018B  IMPORT	R8	K74
      0x8C24114B,  //  018C  GETMET	R9	R8	K75
      0x7C240200,  //  018D  CALL	R9	1
      0x50240000,  //  018E  LDBOOL	R9	0	0
      0x80041200,  //  018F  RET	1	R9
      0x70020000,  //  0190  JMP		#0192
      0xB0080000,  //  0191  RAISE	2	R0	R0
      0x80000000,  //  0192  RET	0
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
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(commissioning),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Commisioning_Context),
    /* K4   */  be_nested_str_weak(im),
    /* K5   */  be_nested_str_weak(IM),
    /* K6   */  be_nested_str_weak(control_message),
    /* K7   */  be_nested_str_weak(Control_Message),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
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
      0x5C100000,  //  000D  MOVE	R4	R0
      0x7C080400,  //  000E  CALL	R2	2
      0x90020C02,  //  000F  SETMBR	R0	K6	R2
      0x80000000,  //  0010  RET	0
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
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(im, 3), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_MessageHandler_init_closure) },
        { be_const_key_weak(send_encrypted_ack, -1), be_const_closure(Matter_MessageHandler_send_encrypted_ack_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_MessageHandler_every_second_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_MessageHandler_every_250ms_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(msg_received, -1), be_const_closure(Matter_MessageHandler_msg_received_closure) },
        { be_const_key_weak(control_message, 4), be_const_var(3) },
        { be_const_key_weak(commissioning, -1), be_const_var(1) },
        { be_const_key_weak(send_simple_ack, 1), be_const_closure(Matter_MessageHandler_send_simple_ack_closure) },
        { be_const_key_weak(send_response_frame, 0), be_const_closure(Matter_MessageHandler_send_response_frame_closure) },
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
