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
    ( &(const bvalue[75]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Frame),
    /* K3   */  be_nested_str_weak(decode_header),
    /* K4   */  be_nested_str_weak(sec_p),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20CONTROL_X20MESSAGE_X3D),
    /* K8   */  be_nested_str_weak(inspect),
    /* K9   */  be_nested_str_weak(device),
    /* K10  */  be_nested_str_weak(sessions),
    /* K11  */  be_nested_str_weak(find_session_source_id_unsecure),
    /* K12  */  be_nested_str_weak(source_node_id),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20find_X20session_X20by_X20source_node_id_X20_X3D_X20),
    /* K14  */  be_nested_str_weak(_X20session_id_X20_X3D_X20),
    /* K15  */  be_nested_str_weak(local_session_id),
    /* K16  */  be_const_int(2),
    /* K17  */  be_nested_str_weak(control_message),
    /* K18  */  be_nested_str_weak(process_incoming_control_message),
    /* K19  */  be_const_int(0),
    /* K20  */  be_nested_str_weak(sec_sesstype),
    /* K21  */  be_const_int(3),
    /* K22  */  be_nested_str_weak(_ip),
    /* K23  */  be_nested_str_weak(_port),
    /* K24  */  be_nested_str_weak(_message_handler),
    /* K25  */  be_nested_str_weak(session),
    /* K26  */  be_nested_str_weak(_counter_insecure_rcv),
    /* K27  */  be_nested_str_weak(validate),
    /* K28  */  be_nested_str_weak(message_counter),
    /* K29  */  be_nested_str_weak(format),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K31  */  be_nested_str_weak(val),
    /* K32  */  be_nested_str_weak(send_simple_ack),
    /* K33  */  be_nested_str_weak(decode_payload),
    /* K34  */  be_nested_str_weak(received_ack),
    /* K35  */  be_nested_str_weak(opcode),
    /* K36  */  be_nested_str_weak(get_opcode_name),
    /* K37  */  be_nested_str_weak(0x_X2502X),
    /* K38  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20rid_X3D_X25i_X20exch_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K39  */  be_nested_str_weak(exchange_id),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K41  */  be_nested_str_weak(x_flag_r),
    /* K42  */  be_nested_str_weak(_X7Breliable_X7D_X20),
    /* K43  */  be_nested_str_weak(),
    /* K44  */  be_nested_str_weak(ack_message_counter),
    /* K45  */  be_nested_str_weak(commissioning),
    /* K46  */  be_nested_str_weak(process_incoming),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K48  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K49  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
    /* K50  */  be_nested_str_weak(counter_rcv_validate),
    /* K51  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K52  */  be_nested_str_weak(_X20counter_X3D),
    /* K53  */  be_nested_str_weak(counter_rcv),
    /* K54  */  be_nested_str_weak(send_encrypted_ack),
    /* K55  */  be_nested_str_weak(decrypt),
    /* K56  */  be_nested_str_weak(raw),
    /* K57  */  be_nested_str_weak(payload_idx),
    /* K58  */  be_const_int(1),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20idx_X3D_X25i_X20clear_X3D_X25s),
    /* K60  */  be_nested_str_weak(tohex),
    /* K61  */  be_nested_str_weak(MTR_X3A_X20_X3E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K62  */  be_nested_str_weak(protocol_id),
    /* K63  */  be_nested_str_weak(_X20opcode_X3D),
    /* K64  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K65  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20_X5B_X2502X_X2F_X2502X_X5D_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20PROTOCOL_ID_SECURE_CHANNEL_X20),
    /* K67  */  be_nested_str_weak(im),
    /* K68  */  be_nested_str_weak(process_incoming_ack),
    /* K69  */  be_nested_str_weak(send_enqueued),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K71  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K72  */  be_nested_str_weak(_X3B),
    /* K73  */  be_nested_str_weak(debug),
    /* K74  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[396]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x50140000,  //  0001  LDBOOL	R5	0	0
      0xA8020172,  //  0002  EXBLK	0	#0176
      0xB81A0200,  //  0003  GETNGBL	R6	K1
      0x8C180D02,  //  0004  GETMET	R6	R6	K2
      0x5C200000,  //  0005  MOVE	R8	R0
      0x5C240200,  //  0006  MOVE	R9	R1
      0x5C280400,  //  0007  MOVE	R10	R2
      0x5C2C0600,  //  0008  MOVE	R11	R3
      0x7C180A00,  //  0009  CALL	R6	5
      0x8C1C0D03,  //  000A  GETMET	R7	R6	K3
      0x7C1C0200,  //  000B  CALL	R7	1
      0x5C200E00,  //  000C  MOVE	R8	R7
      0x74220002,  //  000D  JMPT	R8	#0011
      0x50200000,  //  000E  LDBOOL	R8	0	0
      0xA8040001,  //  000F  EXBLK	1	1
      0x80041000,  //  0010  RET	1	R8
      0x88200D04,  //  0011  GETMBR	R8	R6	K4
      0x78220021,  //  0012  JMPF	R8	#0035
      0xB8220A00,  //  0013  GETNGBL	R8	K5
      0x8C201106,  //  0014  GETMET	R8	R8	K6
      0xB82A0200,  //  0015  GETNGBL	R10	K1
      0x8C281508,  //  0016  GETMET	R10	R10	K8
      0x5C300C00,  //  0017  MOVE	R12	R6
      0x7C280400,  //  0018  CALL	R10	2
      0x002A0E0A,  //  0019  ADD	R10	K7	R10
      0x7C200400,  //  001A  CALL	R8	2
      0x88200109,  //  001B  GETMBR	R8	R0	K9
      0x8820110A,  //  001C  GETMBR	R8	R8	K10
      0x8C20110B,  //  001D  GETMET	R8	R8	K11
      0x88280D0C,  //  001E  GETMBR	R10	R6	K12
      0x542E0059,  //  001F  LDINT	R11	90
      0x7C200600,  //  0020  CALL	R8	3
      0xB8260A00,  //  0021  GETNGBL	R9	K5
      0x8C241306,  //  0022  GETMET	R9	R9	K6
      0x602C0008,  //  0023  GETGBL	R11	G8
      0x88300D0C,  //  0024  GETMBR	R12	R6	K12
      0x7C2C0200,  //  0025  CALL	R11	1
      0x002E1A0B,  //  0026  ADD	R11	K13	R11
      0x002C170E,  //  0027  ADD	R11	R11	K14
      0x60300008,  //  0028  GETGBL	R12	G8
      0x8834110F,  //  0029  GETMBR	R13	R8	K15
      0x7C300200,  //  002A  CALL	R12	1
      0x002C160C,  //  002B  ADD	R11	R11	R12
      0x58300010,  //  002C  LDCONST	R12	K16
      0x7C240600,  //  002D  CALL	R9	3
      0x88240111,  //  002E  GETMBR	R9	R0	K17
      0x8C241312,  //  002F  GETMET	R9	R9	K18
      0x5C2C0C00,  //  0030  MOVE	R11	R6
      0x7C240400,  //  0031  CALL	R9	2
      0xA8040001,  //  0032  EXBLK	1	1
      0x80041200,  //  0033  RET	1	R9
      0x7002013C,  //  0034  JMP		#0172
      0x88200D0F,  //  0035  GETMBR	R8	R6	K15
      0x1C201113,  //  0036  EQ	R8	R8	K19
      0x7822007D,  //  0037  JMPF	R8	#00B6
      0x88200D14,  //  0038  GETMBR	R8	R6	K20
      0x1C201113,  //  0039  EQ	R8	R8	K19
      0x7822007A,  //  003A  JMPF	R8	#00B6
      0x88200109,  //  003B  GETMBR	R8	R0	K9
      0x8820110A,  //  003C  GETMBR	R8	R8	K10
      0x8C20110B,  //  003D  GETMET	R8	R8	K11
      0x88280D0C,  //  003E  GETMBR	R10	R6	K12
      0x542E0059,  //  003F  LDINT	R11	90
      0x7C200600,  //  0040  CALL	R8	3
      0xB8260A00,  //  0041  GETNGBL	R9	K5
      0x8C241306,  //  0042  GETMET	R9	R9	K6
      0x602C0008,  //  0043  GETGBL	R11	G8
      0x88300D0C,  //  0044  GETMBR	R12	R6	K12
      0x7C2C0200,  //  0045  CALL	R11	1
      0x002E1A0B,  //  0046  ADD	R11	K13	R11
      0x002C170E,  //  0047  ADD	R11	R11	K14
      0x60300008,  //  0048  GETGBL	R12	G8
      0x8834110F,  //  0049  GETMBR	R13	R8	K15
      0x7C300200,  //  004A  CALL	R12	1
      0x002C160C,  //  004B  ADD	R11	R11	R12
      0x58300015,  //  004C  LDCONST	R12	K21
      0x7C240600,  //  004D  CALL	R9	3
      0x780A0000,  //  004E  JMPF	R2	#0050
      0x90222C02,  //  004F  SETMBR	R8	K22	R2
      0x780E0000,  //  0050  JMPF	R3	#0052
      0x90222E03,  //  0051  SETMBR	R8	K23	R3
      0x90223000,  //  0052  SETMBR	R8	K24	R0
      0x901A3208,  //  0053  SETMBR	R6	K25	R8
      0x8824111A,  //  0054  GETMBR	R9	R8	K26
      0x8C24131B,  //  0055  GETMET	R9	R9	K27
      0x882C0D1C,  //  0056  GETMBR	R11	R6	K28
      0x50300000,  //  0057  LDBOOL	R12	0	0
      0x7C240600,  //  0058  CALL	R9	3
      0x74260011,  //  0059  JMPT	R9	#006C
      0xB8260A00,  //  005A  GETNGBL	R9	K5
      0x8C241306,  //  005B  GETMET	R9	R9	K6
      0x8C2C091D,  //  005C  GETMET	R11	R4	K29
      0x5834001E,  //  005D  LDCONST	R13	K30
      0x88380D1C,  //  005E  GETMBR	R14	R6	K28
      0x883C111A,  //  005F  GETMBR	R15	R8	K26
      0x8C3C1F1F,  //  0060  GETMET	R15	R15	K31
      0x7C3C0200,  //  0061  CALL	R15	1
      0x7C2C0800,  //  0062  CALL	R11	4
      0x58300015,  //  0063  LDCONST	R12	K21
      0x7C240600,  //  0064  CALL	R9	3
      0x8C240120,  //  0065  GETMET	R9	R0	K32
      0x5C2C0C00,  //  0066  MOVE	R11	R6
      0x50300000,  //  0067  LDBOOL	R12	0	0
      0x7C240600,  //  0068  CALL	R9	3
      0x50240000,  //  0069  LDBOOL	R9	0	0
      0xA8040001,  //  006A  EXBLK	1	1
      0x80041200,  //  006B  RET	1	R9
      0x8C240D21,  //  006C  GETMET	R9	R6	K33
      0x7C240200,  //  006D  CALL	R9	1
      0x74260002,  //  006E  JMPT	R9	#0072
      0x50240000,  //  006F  LDBOOL	R9	0	0
      0xA8040001,  //  0070  EXBLK	1	1
      0x80041200,  //  0071  RET	1	R9
      0x88240109,  //  0072  GETMBR	R9	R0	K9
      0x8C241322,  //  0073  GETMET	R9	R9	K34
      0x5C2C0C00,  //  0074  MOVE	R11	R6
      0x7C240400,  //  0075  CALL	R9	2
      0x88240D23,  //  0076  GETMBR	R9	R6	K35
      0x542A000F,  //  0077  LDINT	R10	16
      0x2024120A,  //  0078  NE	R9	R9	R10
      0x78260018,  //  0079  JMPF	R9	#0093
      0xB8260200,  //  007A  GETNGBL	R9	K1
      0x8C241324,  //  007B  GETMET	R9	R9	K36
      0x882C0D23,  //  007C  GETMBR	R11	R6	K35
      0x7C240400,  //  007D  CALL	R9	2
      0x5C281200,  //  007E  MOVE	R10	R9
      0x742A0004,  //  007F  JMPT	R10	#0085
      0x8C28091D,  //  0080  GETMET	R10	R4	K29
      0x58300025,  //  0081  LDCONST	R12	K37
      0x88340D23,  //  0082  GETMBR	R13	R6	K35
      0x7C280600,  //  0083  CALL	R10	3
      0x5C241400,  //  0084  MOVE	R9	R10
      0xB82A0A00,  //  0085  GETNGBL	R10	K5
      0x8C281506,  //  0086  GETMET	R10	R10	K6
      0x8C30091D,  //  0087  GETMET	R12	R4	K29
      0x58380026,  //  0088  LDCONST	R14	K38
      0x883C110F,  //  0089  GETMBR	R15	R8	K15
      0x5C401200,  //  008A  MOVE	R16	R9
      0x88440D1C,  //  008B  GETMBR	R17	R6	K28
      0x88480D27,  //  008C  GETMBR	R18	R6	K39
      0x5C4C0400,  //  008D  MOVE	R19	R2
      0x5C500600,  //  008E  MOVE	R20	R3
      0x7C301000,  //  008F  CALL	R12	8
      0x58340010,  //  0090  LDCONST	R13	K16
      0x7C280600,  //  0091  CALL	R10	3
      0x70020013,  //  0092  JMP		#00A7
      0xB8260A00,  //  0093  GETNGBL	R9	K5
      0x8C241306,  //  0094  GETMET	R9	R9	K6
      0x8C2C091D,  //  0095  GETMET	R11	R4	K29
      0x58340028,  //  0096  LDCONST	R13	K40
      0x8838110F,  //  0097  GETMBR	R14	R8	K15
      0x883C0D1C,  //  0098  GETMBR	R15	R6	K28
      0x88400D29,  //  0099  GETMBR	R16	R6	K41
      0x78420001,  //  009A  JMPF	R16	#009D
      0x5840002A,  //  009B  LDCONST	R16	K42
      0x70020000,  //  009C  JMP		#009E
      0x5840002B,  //  009D  LDCONST	R16	K43
      0x88440D27,  //  009E  GETMBR	R17	R6	K39
      0x60480008,  //  009F  GETGBL	R18	G8
      0x884C0D2C,  //  00A0  GETMBR	R19	R6	K44
      0x7C480200,  //  00A1  CALL	R18	1
      0x5C4C0400,  //  00A2  MOVE	R19	R2
      0x5C500600,  //  00A3  MOVE	R20	R3
      0x7C2C1200,  //  00A4  CALL	R11	9
      0x58300015,  //  00A5  LDCONST	R12	K21
      0x7C240600,  //  00A6  CALL	R9	3
      0x8824012D,  //  00A7  GETMBR	R9	R0	K45
      0x8C24132E,  //  00A8  GETMET	R9	R9	K46
      0x5C2C0C00,  //  00A9  MOVE	R11	R6
      0x7C240400,  //  00AA  CALL	R9	2
      0x5C141200,  //  00AB  MOVE	R5	R9
      0x5C240A00,  //  00AC  MOVE	R9	R5
      0x74260003,  //  00AD  JMPT	R9	#00B2
      0x8C240120,  //  00AE  GETMET	R9	R0	K32
      0x5C2C0C00,  //  00AF  MOVE	R11	R6
      0x50300000,  //  00B0  LDBOOL	R12	0	0
      0x7C240600,  //  00B1  CALL	R9	3
      0x50240200,  //  00B2  LDBOOL	R9	1	0
      0xA8040001,  //  00B3  EXBLK	1	1
      0x80041200,  //  00B4  RET	1	R9
      0x700200BB,  //  00B5  JMP		#0172
      0xB8220A00,  //  00B6  GETNGBL	R8	K5
      0x8C201106,  //  00B7  GETMET	R8	R8	K6
      0x8C28091D,  //  00B8  GETMET	R10	R4	K29
      0x5830002F,  //  00B9  LDCONST	R12	K47
      0x88340D0F,  //  00BA  GETMBR	R13	R6	K15
      0x88380D1C,  //  00BB  GETMBR	R14	R6	K28
      0x7C280800,  //  00BC  CALL	R10	4
      0x582C0015,  //  00BD  LDCONST	R11	K21
      0x7C200600,  //  00BE  CALL	R8	3
      0x88200109,  //  00BF  GETMBR	R8	R0	K9
      0x8820110A,  //  00C0  GETMBR	R8	R8	K10
      0x8C201130,  //  00C1  GETMET	R8	R8	K48
      0x88280D0F,  //  00C2  GETMBR	R10	R6	K15
      0x7C200400,  //  00C3  CALL	R8	2
      0x4C240000,  //  00C4  LDNIL	R9
      0x1C241009,  //  00C5  EQ	R9	R8	R9
      0x7826000A,  //  00C6  JMPF	R9	#00D2
      0xB8260A00,  //  00C7  GETNGBL	R9	K5
      0x8C241306,  //  00C8  GETMET	R9	R9	K6
      0x602C0008,  //  00C9  GETGBL	R11	G8
      0x88300D0F,  //  00CA  GETMBR	R12	R6	K15
      0x7C2C0200,  //  00CB  CALL	R11	1
      0x002E620B,  //  00CC  ADD	R11	K49	R11
      0x58300010,  //  00CD  LDCONST	R12	K16
      0x7C240600,  //  00CE  CALL	R9	3
      0x50240000,  //  00CF  LDBOOL	R9	0	0
      0xA8040001,  //  00D0  EXBLK	1	1
      0x80041200,  //  00D1  RET	1	R9
      0x780A0000,  //  00D2  JMPF	R2	#00D4
      0x90222C02,  //  00D3  SETMBR	R8	K22	R2
      0x780E0000,  //  00D4  JMPF	R3	#00D6
      0x90222E03,  //  00D5  SETMBR	R8	K23	R3
      0x90223000,  //  00D6  SETMBR	R8	K24	R0
      0x901A3208,  //  00D7  SETMBR	R6	K25	R8
      0x8C241132,  //  00D8  GETMET	R9	R8	K50
      0x882C0D1C,  //  00D9  GETMBR	R11	R6	K28
      0x50300200,  //  00DA  LDBOOL	R12	1	0
      0x7C240600,  //  00DB  CALL	R9	3
      0x74260013,  //  00DC  JMPT	R9	#00F1
      0xB8260A00,  //  00DD  GETNGBL	R9	K5
      0x8C241306,  //  00DE  GETMET	R9	R9	K6
      0x602C0008,  //  00DF  GETGBL	R11	G8
      0x88300D1C,  //  00E0  GETMBR	R12	R6	K28
      0x7C2C0200,  //  00E1  CALL	R11	1
      0x002E660B,  //  00E2  ADD	R11	K51	R11
      0x002C1734,  //  00E3  ADD	R11	R11	K52
      0x60300008,  //  00E4  GETGBL	R12	G8
      0x88341135,  //  00E5  GETMBR	R13	R8	K53
      0x7C300200,  //  00E6  CALL	R12	1
      0x002C160C,  //  00E7  ADD	R11	R11	R12
      0x58300015,  //  00E8  LDCONST	R12	K21
      0x7C240600,  //  00E9  CALL	R9	3
      0x8C240136,  //  00EA  GETMET	R9	R0	K54
      0x5C2C0C00,  //  00EB  MOVE	R11	R6
      0x50300000,  //  00EC  LDBOOL	R12	0	0
      0x7C240600,  //  00ED  CALL	R9	3
      0x50240000,  //  00EE  LDBOOL	R9	0	0
      0xA8040001,  //  00EF  EXBLK	1	1
      0x80041200,  //  00F0  RET	1	R9
      0x8C240D37,  //  00F1  GETMET	R9	R6	K55
      0x7C240200,  //  00F2  CALL	R9	1
      0x5C281200,  //  00F3  MOVE	R10	R9
      0x742A0002,  //  00F4  JMPT	R10	#00F8
      0x50280000,  //  00F5  LDBOOL	R10	0	0
      0xA8040001,  //  00F6  EXBLK	1	1
      0x80041400,  //  00F7  RET	1	R10
      0x88280D39,  //  00F8  GETMBR	R10	R6	K57
      0x0428153A,  //  00F9  SUB	R10	R10	K58
      0x402A260A,  //  00FA  CONNECT	R10	K19	R10
      0x882C0D38,  //  00FB  GETMBR	R11	R6	K56
      0x9428160A,  //  00FC  GETIDX	R10	R11	R10
      0x901A700A,  //  00FD  SETMBR	R6	K56	R10
      0x88280D38,  //  00FE  GETMBR	R10	R6	K56
      0x40281409,  //  00FF  CONNECT	R10	R10	R9
      0xB82A0A00,  //  0100  GETNGBL	R10	K5
      0x8C281506,  //  0101  GETMET	R10	R10	K6
      0x8C30091D,  //  0102  GETMET	R12	R4	K29
      0x5838003B,  //  0103  LDCONST	R14	K59
      0x883C0D39,  //  0104  GETMBR	R15	R6	K57
      0x88400D38,  //  0105  GETMBR	R16	R6	K56
      0x8C40213C,  //  0106  GETMET	R16	R16	K60
      0x7C400200,  //  0107  CALL	R16	1
      0x7C300800,  //  0108  CALL	R12	4
      0x54360003,  //  0109  LDINT	R13	4
      0x7C280600,  //  010A  CALL	R10	3
      0x8C280D21,  //  010B  GETMET	R10	R6	K33
      0x7C280200,  //  010C  CALL	R10	1
      0xB82A0A00,  //  010D  GETNGBL	R10	K5
      0x8C281506,  //  010E  GETMET	R10	R10	K6
      0x60300008,  //  010F  GETGBL	R12	G8
      0x88340D3E,  //  0110  GETMBR	R13	R6	K62
      0x7C300200,  //  0111  CALL	R12	1
      0x00327A0C,  //  0112  ADD	R12	K61	R12
      0x0030193F,  //  0113  ADD	R12	R12	K63
      0x60340008,  //  0114  GETGBL	R13	G8
      0x88380D23,  //  0115  GETMBR	R14	R6	K35
      0x7C340200,  //  0116  CALL	R13	1
      0x0030180D,  //  0117  ADD	R12	R12	R13
      0x00301940,  //  0118  ADD	R12	R12	K64
      0x60340008,  //  0119  GETGBL	R13	G8
      0x88380D27,  //  011A  GETMBR	R14	R6	K39
      0x543EFFFE,  //  011B  LDINT	R15	65535
      0x2C381C0F,  //  011C  AND	R14	R14	R15
      0x7C340200,  //  011D  CALL	R13	1
      0x0030180D,  //  011E  ADD	R12	R12	R13
      0x58340015,  //  011F  LDCONST	R13	K21
      0x7C280600,  //  0120  CALL	R10	3
      0xB82A0A00,  //  0121  GETNGBL	R10	K5
      0x8C281506,  //  0122  GETMET	R10	R10	K6
      0x8C30091D,  //  0123  GETMET	R12	R4	K29
      0x58380041,  //  0124  LDCONST	R14	K65
      0x883C110F,  //  0125  GETMBR	R15	R8	K15
      0x88400D3E,  //  0126  GETMBR	R16	R6	K62
      0x88440D23,  //  0127  GETMBR	R17	R6	K35
      0x88480D1C,  //  0128  GETMBR	R18	R6	K28
      0x884C0D27,  //  0129  GETMBR	R19	R6	K39
      0x60500008,  //  012A  GETGBL	R20	G8
      0x88540D2C,  //  012B  GETMBR	R21	R6	K44
      0x7C500200,  //  012C  CALL	R20	1
      0x88540D29,  //  012D  GETMBR	R21	R6	K41
      0x78560001,  //  012E  JMPF	R21	#0131
      0x5854002A,  //  012F  LDCONST	R21	K42
      0x70020000,  //  0130  JMP		#0132
      0x5854002B,  //  0131  LDCONST	R21	K43
      0x5C580400,  //  0132  MOVE	R22	R2
      0x5C5C0600,  //  0133  MOVE	R23	R3
      0x7C301600,  //  0134  CALL	R12	11
      0x58340015,  //  0135  LDCONST	R13	K21
      0x7C280600,  //  0136  CALL	R10	3
      0x88280109,  //  0137  GETMBR	R10	R0	K9
      0x8C281522,  //  0138  GETMET	R10	R10	K34
      0x5C300C00,  //  0139  MOVE	R12	R6
      0x7C280400,  //  013A  CALL	R10	2
      0x88280D3E,  //  013B  GETMBR	R10	R6	K62
      0x1C2C1513,  //  013C  EQ	R11	R10	K19
      0x782E0018,  //  013D  JMPF	R11	#0157
      0xB82E0A00,  //  013E  GETNGBL	R11	K5
      0x8C2C1706,  //  013F  GETMET	R11	R11	K6
      0xB8360200,  //  0140  GETNGBL	R13	K1
      0x8C341B08,  //  0141  GETMET	R13	R13	K8
      0x5C3C0C00,  //  0142  MOVE	R15	R6
      0x7C340400,  //  0143  CALL	R13	2
      0x0036840D,  //  0144  ADD	R13	K66	R13
      0x58380015,  //  0145  LDCONST	R14	K21
      0x7C2C0600,  //  0146  CALL	R11	3
      0x882C0D23,  //  0147  GETMBR	R11	R6	K35
      0x5432000F,  //  0148  LDINT	R12	16
      0x1C2C160C,  //  0149  EQ	R11	R11	R12
      0x782E0009,  //  014A  JMPF	R11	#0155
      0x882C0143,  //  014B  GETMBR	R11	R0	K67
      0x8C2C1744,  //  014C  GETMET	R11	R11	K68
      0x5C340C00,  //  014D  MOVE	R13	R6
      0x7C2C0400,  //  014E  CALL	R11	2
      0x5C141600,  //  014F  MOVE	R5	R11
      0x78160003,  //  0150  JMPF	R5	#0155
      0x882C0143,  //  0151  GETMBR	R11	R0	K67
      0x8C2C1745,  //  0152  GETMET	R11	R11	K69
      0x5C340000,  //  0153  MOVE	R13	R0
      0x7C2C0400,  //  0154  CALL	R11	2
      0x50140200,  //  0155  LDBOOL	R5	1	0
      0x7002001A,  //  0156  JMP		#0172
      0x1C2C153A,  //  0157  EQ	R11	R10	K58
      0x782E0010,  //  0158  JMPF	R11	#016A
      0x882C0143,  //  0159  GETMBR	R11	R0	K67
      0x8C2C172E,  //  015A  GETMET	R11	R11	K46
      0x5C340C00,  //  015B  MOVE	R13	R6
      0x7C2C0400,  //  015C  CALL	R11	2
      0x5C141600,  //  015D  MOVE	R5	R11
      0x78160004,  //  015E  JMPF	R5	#0164
      0x882C0143,  //  015F  GETMBR	R11	R0	K67
      0x8C2C1745,  //  0160  GETMET	R11	R11	K69
      0x5C340000,  //  0161  MOVE	R13	R0
      0x7C2C0400,  //  0162  CALL	R11	2
      0x70020003,  //  0163  JMP		#0168
      0x8C2C0136,  //  0164  GETMET	R11	R0	K54
      0x5C340C00,  //  0165  MOVE	R13	R6
      0x50380200,  //  0166  LDBOOL	R14	1	0
      0x7C2C0600,  //  0167  CALL	R11	3
      0x50140200,  //  0168  LDBOOL	R5	1	0
      0x70020007,  //  0169  JMP		#0172
      0xB82E0A00,  //  016A  GETNGBL	R11	K5
      0x8C2C1706,  //  016B  GETMET	R11	R11	K6
      0x60340008,  //  016C  GETGBL	R13	G8
      0x5C381400,  //  016D  MOVE	R14	R10
      0x7C340200,  //  016E  CALL	R13	1
      0x00368C0D,  //  016F  ADD	R13	K70	R13
      0x58380015,  //  0170  LDCONST	R14	K21
      0x7C2C0600,  //  0171  CALL	R11	3
      0xA8040001,  //  0172  EXBLK	1	1
      0x80040A00,  //  0173  RET	1	R5
      0xA8040001,  //  0174  EXBLK	1	1
      0x70020014,  //  0175  JMP		#018B
      0xAC180002,  //  0176  CATCH	R6	0	2
      0x70020011,  //  0177  JMP		#018A
      0xB8220A00,  //  0178  GETNGBL	R8	K5
      0x8C201106,  //  0179  GETMET	R8	R8	K6
      0x60280008,  //  017A  GETGBL	R10	G8
      0x5C2C0C00,  //  017B  MOVE	R11	R6
      0x7C280200,  //  017C  CALL	R10	1
      0x002A8E0A,  //  017D  ADD	R10	K71	R10
      0x00281548,  //  017E  ADD	R10	R10	K72
      0x602C0008,  //  017F  GETGBL	R11	G8
      0x5C300E00,  //  0180  MOVE	R12	R7
      0x7C2C0200,  //  0181  CALL	R11	1
      0x0028140B,  //  0182  ADD	R10	R10	R11
      0x7C200400,  //  0183  CALL	R8	2
      0xA4229200,  //  0184  IMPORT	R8	K73
      0x8C24114A,  //  0185  GETMET	R9	R8	K74
      0x7C240200,  //  0186  CALL	R9	1
      0x50240000,  //  0187  LDBOOL	R9	0	0
      0x80041200,  //  0188  RET	1	R9
      0x70020000,  //  0189  JMP		#018B
      0xB0080000,  //  018A  RAISE	2	R0	R0
      0x80000000,  //  018B  RET	0
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
