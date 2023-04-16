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
    ( &(const bvalue[74]) {     /* constants */
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
    /* K16  */  be_nested_str_weak(control_message),
    /* K17  */  be_nested_str_weak(process_incoming_control_message),
    /* K18  */  be_const_int(0),
    /* K19  */  be_nested_str_weak(sec_sesstype),
    /* K20  */  be_nested_str_weak(_ip),
    /* K21  */  be_nested_str_weak(_port),
    /* K22  */  be_nested_str_weak(_message_handler),
    /* K23  */  be_nested_str_weak(session),
    /* K24  */  be_nested_str_weak(_counter_insecure_rcv),
    /* K25  */  be_nested_str_weak(validate),
    /* K26  */  be_nested_str_weak(message_counter),
    /* K27  */  be_nested_str_weak(format),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K29  */  be_nested_str_weak(val),
    /* K30  */  be_const_int(3),
    /* K31  */  be_nested_str_weak(send_simple_ack),
    /* K32  */  be_nested_str_weak(decode_payload),
    /* K33  */  be_nested_str_weak(received_ack),
    /* K34  */  be_nested_str_weak(opcode),
    /* K35  */  be_nested_str_weak(get_opcode_name),
    /* K36  */  be_nested_str_weak(0x_X2502X),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20rid_X3D_X25i_X20exch_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K38  */  be_nested_str_weak(exchange_id),
    /* K39  */  be_const_int(2),
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
    /* K66  */  be_nested_str_weak(im),
    /* K67  */  be_nested_str_weak(process_incoming_ack),
    /* K68  */  be_nested_str_weak(send_enqueued),
    /* K69  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K71  */  be_nested_str_weak(_X3B),
    /* K72  */  be_nested_str_weak(debug),
    /* K73  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[388]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x50140000,  //  0001  LDBOOL	R5	0	0
      0xA802016A,  //  0002  EXBLK	0	#016E
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
      0x78220022,  //  0012  JMPF	R8	#0036
      0xB8220A00,  //  0013  GETNGBL	R8	K5
      0x8C201106,  //  0014  GETMET	R8	R8	K6
      0xB82A0200,  //  0015  GETNGBL	R10	K1
      0x8C281508,  //  0016  GETMET	R10	R10	K8
      0x5C300C00,  //  0017  MOVE	R12	R6
      0x7C280400,  //  0018  CALL	R10	2
      0x002A0E0A,  //  0019  ADD	R10	K7	R10
      0x542E0003,  //  001A  LDINT	R11	4
      0x7C200600,  //  001B  CALL	R8	3
      0x88200109,  //  001C  GETMBR	R8	R0	K9
      0x8820110A,  //  001D  GETMBR	R8	R8	K10
      0x8C20110B,  //  001E  GETMET	R8	R8	K11
      0x88280D0C,  //  001F  GETMBR	R10	R6	K12
      0x542E0059,  //  0020  LDINT	R11	90
      0x7C200600,  //  0021  CALL	R8	3
      0xB8260A00,  //  0022  GETNGBL	R9	K5
      0x8C241306,  //  0023  GETMET	R9	R9	K6
      0x602C0008,  //  0024  GETGBL	R11	G8
      0x88300D0C,  //  0025  GETMBR	R12	R6	K12
      0x7C2C0200,  //  0026  CALL	R11	1
      0x002E1A0B,  //  0027  ADD	R11	K13	R11
      0x002C170E,  //  0028  ADD	R11	R11	K14
      0x60300008,  //  0029  GETGBL	R12	G8
      0x8834110F,  //  002A  GETMBR	R13	R8	K15
      0x7C300200,  //  002B  CALL	R12	1
      0x002C160C,  //  002C  ADD	R11	R11	R12
      0x54320003,  //  002D  LDINT	R12	4
      0x7C240600,  //  002E  CALL	R9	3
      0x88240110,  //  002F  GETMBR	R9	R0	K16
      0x8C241311,  //  0030  GETMET	R9	R9	K17
      0x5C2C0C00,  //  0031  MOVE	R11	R6
      0x7C240400,  //  0032  CALL	R9	2
      0xA8040001,  //  0033  EXBLK	1	1
      0x80041200,  //  0034  RET	1	R9
      0x70020133,  //  0035  JMP		#016A
      0x88200D0F,  //  0036  GETMBR	R8	R6	K15
      0x1C201112,  //  0037  EQ	R8	R8	K18
      0x7822007D,  //  0038  JMPF	R8	#00B7
      0x88200D13,  //  0039  GETMBR	R8	R6	K19
      0x1C201112,  //  003A  EQ	R8	R8	K18
      0x7822007A,  //  003B  JMPF	R8	#00B7
      0x88200109,  //  003C  GETMBR	R8	R0	K9
      0x8820110A,  //  003D  GETMBR	R8	R8	K10
      0x8C20110B,  //  003E  GETMET	R8	R8	K11
      0x88280D0C,  //  003F  GETMBR	R10	R6	K12
      0x542E0059,  //  0040  LDINT	R11	90
      0x7C200600,  //  0041  CALL	R8	3
      0xB8260A00,  //  0042  GETNGBL	R9	K5
      0x8C241306,  //  0043  GETMET	R9	R9	K6
      0x602C0008,  //  0044  GETGBL	R11	G8
      0x88300D0C,  //  0045  GETMBR	R12	R6	K12
      0x7C2C0200,  //  0046  CALL	R11	1
      0x002E1A0B,  //  0047  ADD	R11	K13	R11
      0x002C170E,  //  0048  ADD	R11	R11	K14
      0x60300008,  //  0049  GETGBL	R12	G8
      0x8834110F,  //  004A  GETMBR	R13	R8	K15
      0x7C300200,  //  004B  CALL	R12	1
      0x002C160C,  //  004C  ADD	R11	R11	R12
      0x54320003,  //  004D  LDINT	R12	4
      0x7C240600,  //  004E  CALL	R9	3
      0x780A0000,  //  004F  JMPF	R2	#0051
      0x90222802,  //  0050  SETMBR	R8	K20	R2
      0x780E0000,  //  0051  JMPF	R3	#0053
      0x90222A03,  //  0052  SETMBR	R8	K21	R3
      0x90222C00,  //  0053  SETMBR	R8	K22	R0
      0x901A2E08,  //  0054  SETMBR	R6	K23	R8
      0x88241118,  //  0055  GETMBR	R9	R8	K24
      0x8C241319,  //  0056  GETMET	R9	R9	K25
      0x882C0D1A,  //  0057  GETMBR	R11	R6	K26
      0x50300000,  //  0058  LDBOOL	R12	0	0
      0x7C240600,  //  0059  CALL	R9	3
      0x74260011,  //  005A  JMPT	R9	#006D
      0xB8260A00,  //  005B  GETNGBL	R9	K5
      0x8C241306,  //  005C  GETMET	R9	R9	K6
      0x8C2C091B,  //  005D  GETMET	R11	R4	K27
      0x5834001C,  //  005E  LDCONST	R13	K28
      0x88380D1A,  //  005F  GETMBR	R14	R6	K26
      0x883C1118,  //  0060  GETMBR	R15	R8	K24
      0x8C3C1F1D,  //  0061  GETMET	R15	R15	K29
      0x7C3C0200,  //  0062  CALL	R15	1
      0x7C2C0800,  //  0063  CALL	R11	4
      0x5830001E,  //  0064  LDCONST	R12	K30
      0x7C240600,  //  0065  CALL	R9	3
      0x8C24011F,  //  0066  GETMET	R9	R0	K31
      0x5C2C0C00,  //  0067  MOVE	R11	R6
      0x50300000,  //  0068  LDBOOL	R12	0	0
      0x7C240600,  //  0069  CALL	R9	3
      0x50240000,  //  006A  LDBOOL	R9	0	0
      0xA8040001,  //  006B  EXBLK	1	1
      0x80041200,  //  006C  RET	1	R9
      0x8C240D20,  //  006D  GETMET	R9	R6	K32
      0x7C240200,  //  006E  CALL	R9	1
      0x74260002,  //  006F  JMPT	R9	#0073
      0x50240000,  //  0070  LDBOOL	R9	0	0
      0xA8040001,  //  0071  EXBLK	1	1
      0x80041200,  //  0072  RET	1	R9
      0x88240109,  //  0073  GETMBR	R9	R0	K9
      0x8C241321,  //  0074  GETMET	R9	R9	K33
      0x5C2C0C00,  //  0075  MOVE	R11	R6
      0x7C240400,  //  0076  CALL	R9	2
      0x88240D22,  //  0077  GETMBR	R9	R6	K34
      0x542A000F,  //  0078  LDINT	R10	16
      0x2024120A,  //  0079  NE	R9	R9	R10
      0x78260018,  //  007A  JMPF	R9	#0094
      0xB8260200,  //  007B  GETNGBL	R9	K1
      0x8C241323,  //  007C  GETMET	R9	R9	K35
      0x882C0D22,  //  007D  GETMBR	R11	R6	K34
      0x7C240400,  //  007E  CALL	R9	2
      0x5C281200,  //  007F  MOVE	R10	R9
      0x742A0004,  //  0080  JMPT	R10	#0086
      0x8C28091B,  //  0081  GETMET	R10	R4	K27
      0x58300024,  //  0082  LDCONST	R12	K36
      0x88340D22,  //  0083  GETMBR	R13	R6	K34
      0x7C280600,  //  0084  CALL	R10	3
      0x5C241400,  //  0085  MOVE	R9	R10
      0xB82A0A00,  //  0086  GETNGBL	R10	K5
      0x8C281506,  //  0087  GETMET	R10	R10	K6
      0x8C30091B,  //  0088  GETMET	R12	R4	K27
      0x58380025,  //  0089  LDCONST	R14	K37
      0x883C110F,  //  008A  GETMBR	R15	R8	K15
      0x5C401200,  //  008B  MOVE	R16	R9
      0x88440D1A,  //  008C  GETMBR	R17	R6	K26
      0x88480D26,  //  008D  GETMBR	R18	R6	K38
      0x5C4C0400,  //  008E  MOVE	R19	R2
      0x5C500600,  //  008F  MOVE	R20	R3
      0x7C301000,  //  0090  CALL	R12	8
      0x58340027,  //  0091  LDCONST	R13	K39
      0x7C280600,  //  0092  CALL	R10	3
      0x70020013,  //  0093  JMP		#00A8
      0xB8260A00,  //  0094  GETNGBL	R9	K5
      0x8C241306,  //  0095  GETMET	R9	R9	K6
      0x8C2C091B,  //  0096  GETMET	R11	R4	K27
      0x58340028,  //  0097  LDCONST	R13	K40
      0x8838110F,  //  0098  GETMBR	R14	R8	K15
      0x883C0D1A,  //  0099  GETMBR	R15	R6	K26
      0x88400D29,  //  009A  GETMBR	R16	R6	K41
      0x78420001,  //  009B  JMPF	R16	#009E
      0x5840002A,  //  009C  LDCONST	R16	K42
      0x70020000,  //  009D  JMP		#009F
      0x5840002B,  //  009E  LDCONST	R16	K43
      0x88440D26,  //  009F  GETMBR	R17	R6	K38
      0x60480008,  //  00A0  GETGBL	R18	G8
      0x884C0D2C,  //  00A1  GETMBR	R19	R6	K44
      0x7C480200,  //  00A2  CALL	R18	1
      0x5C4C0400,  //  00A3  MOVE	R19	R2
      0x5C500600,  //  00A4  MOVE	R20	R3
      0x7C2C1200,  //  00A5  CALL	R11	9
      0x5830001E,  //  00A6  LDCONST	R12	K30
      0x7C240600,  //  00A7  CALL	R9	3
      0x8824012D,  //  00A8  GETMBR	R9	R0	K45
      0x8C24132E,  //  00A9  GETMET	R9	R9	K46
      0x5C2C0C00,  //  00AA  MOVE	R11	R6
      0x7C240400,  //  00AB  CALL	R9	2
      0x5C141200,  //  00AC  MOVE	R5	R9
      0x5C240A00,  //  00AD  MOVE	R9	R5
      0x74260003,  //  00AE  JMPT	R9	#00B3
      0x8C24011F,  //  00AF  GETMET	R9	R0	K31
      0x5C2C0C00,  //  00B0  MOVE	R11	R6
      0x50300000,  //  00B1  LDBOOL	R12	0	0
      0x7C240600,  //  00B2  CALL	R9	3
      0x50240200,  //  00B3  LDBOOL	R9	1	0
      0xA8040001,  //  00B4  EXBLK	1	1
      0x80041200,  //  00B5  RET	1	R9
      0x700200B2,  //  00B6  JMP		#016A
      0xB8220A00,  //  00B7  GETNGBL	R8	K5
      0x8C201106,  //  00B8  GETMET	R8	R8	K6
      0x8C28091B,  //  00B9  GETMET	R10	R4	K27
      0x5830002F,  //  00BA  LDCONST	R12	K47
      0x88340D0F,  //  00BB  GETMBR	R13	R6	K15
      0x88380D1A,  //  00BC  GETMBR	R14	R6	K26
      0x7C280800,  //  00BD  CALL	R10	4
      0x582C001E,  //  00BE  LDCONST	R11	K30
      0x7C200600,  //  00BF  CALL	R8	3
      0x88200109,  //  00C0  GETMBR	R8	R0	K9
      0x8820110A,  //  00C1  GETMBR	R8	R8	K10
      0x8C201130,  //  00C2  GETMET	R8	R8	K48
      0x88280D0F,  //  00C3  GETMBR	R10	R6	K15
      0x7C200400,  //  00C4  CALL	R8	2
      0x4C240000,  //  00C5  LDNIL	R9
      0x1C241009,  //  00C6  EQ	R9	R8	R9
      0x7826000A,  //  00C7  JMPF	R9	#00D3
      0xB8260A00,  //  00C8  GETNGBL	R9	K5
      0x8C241306,  //  00C9  GETMET	R9	R9	K6
      0x602C0008,  //  00CA  GETGBL	R11	G8
      0x88300D0F,  //  00CB  GETMBR	R12	R6	K15
      0x7C2C0200,  //  00CC  CALL	R11	1
      0x002E620B,  //  00CD  ADD	R11	K49	R11
      0x58300027,  //  00CE  LDCONST	R12	K39
      0x7C240600,  //  00CF  CALL	R9	3
      0x50240000,  //  00D0  LDBOOL	R9	0	0
      0xA8040001,  //  00D1  EXBLK	1	1
      0x80041200,  //  00D2  RET	1	R9
      0x780A0000,  //  00D3  JMPF	R2	#00D5
      0x90222802,  //  00D4  SETMBR	R8	K20	R2
      0x780E0000,  //  00D5  JMPF	R3	#00D7
      0x90222A03,  //  00D6  SETMBR	R8	K21	R3
      0x90222C00,  //  00D7  SETMBR	R8	K22	R0
      0x901A2E08,  //  00D8  SETMBR	R6	K23	R8
      0x8C241132,  //  00D9  GETMET	R9	R8	K50
      0x882C0D1A,  //  00DA  GETMBR	R11	R6	K26
      0x50300200,  //  00DB  LDBOOL	R12	1	0
      0x7C240600,  //  00DC  CALL	R9	3
      0x74260013,  //  00DD  JMPT	R9	#00F2
      0xB8260A00,  //  00DE  GETNGBL	R9	K5
      0x8C241306,  //  00DF  GETMET	R9	R9	K6
      0x602C0008,  //  00E0  GETGBL	R11	G8
      0x88300D1A,  //  00E1  GETMBR	R12	R6	K26
      0x7C2C0200,  //  00E2  CALL	R11	1
      0x002E660B,  //  00E3  ADD	R11	K51	R11
      0x002C1734,  //  00E4  ADD	R11	R11	K52
      0x60300008,  //  00E5  GETGBL	R12	G8
      0x88341135,  //  00E6  GETMBR	R13	R8	K53
      0x7C300200,  //  00E7  CALL	R12	1
      0x002C160C,  //  00E8  ADD	R11	R11	R12
      0x5830001E,  //  00E9  LDCONST	R12	K30
      0x7C240600,  //  00EA  CALL	R9	3
      0x8C240136,  //  00EB  GETMET	R9	R0	K54
      0x5C2C0C00,  //  00EC  MOVE	R11	R6
      0x50300000,  //  00ED  LDBOOL	R12	0	0
      0x7C240600,  //  00EE  CALL	R9	3
      0x50240000,  //  00EF  LDBOOL	R9	0	0
      0xA8040001,  //  00F0  EXBLK	1	1
      0x80041200,  //  00F1  RET	1	R9
      0x8C240D37,  //  00F2  GETMET	R9	R6	K55
      0x7C240200,  //  00F3  CALL	R9	1
      0x5C281200,  //  00F4  MOVE	R10	R9
      0x742A0002,  //  00F5  JMPT	R10	#00F9
      0x50280000,  //  00F6  LDBOOL	R10	0	0
      0xA8040001,  //  00F7  EXBLK	1	1
      0x80041400,  //  00F8  RET	1	R10
      0x88280D39,  //  00F9  GETMBR	R10	R6	K57
      0x0428153A,  //  00FA  SUB	R10	R10	K58
      0x402A240A,  //  00FB  CONNECT	R10	K18	R10
      0x882C0D38,  //  00FC  GETMBR	R11	R6	K56
      0x9428160A,  //  00FD  GETIDX	R10	R11	R10
      0x901A700A,  //  00FE  SETMBR	R6	K56	R10
      0x88280D38,  //  00FF  GETMBR	R10	R6	K56
      0x40281409,  //  0100  CONNECT	R10	R10	R9
      0xB82A0A00,  //  0101  GETNGBL	R10	K5
      0x8C281506,  //  0102  GETMET	R10	R10	K6
      0x8C30091B,  //  0103  GETMET	R12	R4	K27
      0x5838003B,  //  0104  LDCONST	R14	K59
      0x883C0D39,  //  0105  GETMBR	R15	R6	K57
      0x88400D38,  //  0106  GETMBR	R16	R6	K56
      0x8C40213C,  //  0107  GETMET	R16	R16	K60
      0x7C400200,  //  0108  CALL	R16	1
      0x7C300800,  //  0109  CALL	R12	4
      0x54360003,  //  010A  LDINT	R13	4
      0x7C280600,  //  010B  CALL	R10	3
      0x8C280D20,  //  010C  GETMET	R10	R6	K32
      0x7C280200,  //  010D  CALL	R10	1
      0xB82A0A00,  //  010E  GETNGBL	R10	K5
      0x8C281506,  //  010F  GETMET	R10	R10	K6
      0x60300008,  //  0110  GETGBL	R12	G8
      0x88340D3E,  //  0111  GETMBR	R13	R6	K62
      0x7C300200,  //  0112  CALL	R12	1
      0x00327A0C,  //  0113  ADD	R12	K61	R12
      0x0030193F,  //  0114  ADD	R12	R12	K63
      0x60340008,  //  0115  GETGBL	R13	G8
      0x88380D22,  //  0116  GETMBR	R14	R6	K34
      0x7C340200,  //  0117  CALL	R13	1
      0x0030180D,  //  0118  ADD	R12	R12	R13
      0x00301940,  //  0119  ADD	R12	R12	K64
      0x60340008,  //  011A  GETGBL	R13	G8
      0x88380D26,  //  011B  GETMBR	R14	R6	K38
      0x543EFFFE,  //  011C  LDINT	R15	65535
      0x2C381C0F,  //  011D  AND	R14	R14	R15
      0x7C340200,  //  011E  CALL	R13	1
      0x0030180D,  //  011F  ADD	R12	R12	R13
      0x5834001E,  //  0120  LDCONST	R13	K30
      0x7C280600,  //  0121  CALL	R10	3
      0xB82A0A00,  //  0122  GETNGBL	R10	K5
      0x8C281506,  //  0123  GETMET	R10	R10	K6
      0x8C30091B,  //  0124  GETMET	R12	R4	K27
      0x58380041,  //  0125  LDCONST	R14	K65
      0x883C110F,  //  0126  GETMBR	R15	R8	K15
      0x88400D3E,  //  0127  GETMBR	R16	R6	K62
      0x88440D22,  //  0128  GETMBR	R17	R6	K34
      0x88480D1A,  //  0129  GETMBR	R18	R6	K26
      0x884C0D26,  //  012A  GETMBR	R19	R6	K38
      0x60500008,  //  012B  GETGBL	R20	G8
      0x88540D2C,  //  012C  GETMBR	R21	R6	K44
      0x7C500200,  //  012D  CALL	R20	1
      0x88540D29,  //  012E  GETMBR	R21	R6	K41
      0x78560001,  //  012F  JMPF	R21	#0132
      0x5854002A,  //  0130  LDCONST	R21	K42
      0x70020000,  //  0131  JMP		#0133
      0x5854002B,  //  0132  LDCONST	R21	K43
      0x5C580400,  //  0133  MOVE	R22	R2
      0x5C5C0600,  //  0134  MOVE	R23	R3
      0x7C301600,  //  0135  CALL	R12	11
      0x5834001E,  //  0136  LDCONST	R13	K30
      0x7C280600,  //  0137  CALL	R10	3
      0x88280109,  //  0138  GETMBR	R10	R0	K9
      0x8C281521,  //  0139  GETMET	R10	R10	K33
      0x5C300C00,  //  013A  MOVE	R12	R6
      0x7C280400,  //  013B  CALL	R10	2
      0x88280D3E,  //  013C  GETMBR	R10	R6	K62
      0x1C2C1512,  //  013D  EQ	R11	R10	K18
      0x782E000F,  //  013E  JMPF	R11	#014F
      0x882C0D22,  //  013F  GETMBR	R11	R6	K34
      0x5432000F,  //  0140  LDINT	R12	16
      0x1C2C160C,  //  0141  EQ	R11	R11	R12
      0x782E0009,  //  0142  JMPF	R11	#014D
      0x882C0142,  //  0143  GETMBR	R11	R0	K66
      0x8C2C1743,  //  0144  GETMET	R11	R11	K67
      0x5C340C00,  //  0145  MOVE	R13	R6
      0x7C2C0400,  //  0146  CALL	R11	2
      0x5C141600,  //  0147  MOVE	R5	R11
      0x78160003,  //  0148  JMPF	R5	#014D
      0x882C0142,  //  0149  GETMBR	R11	R0	K66
      0x8C2C1744,  //  014A  GETMET	R11	R11	K68
      0x5C340000,  //  014B  MOVE	R13	R0
      0x7C2C0400,  //  014C  CALL	R11	2
      0x50140200,  //  014D  LDBOOL	R5	1	0
      0x7002001A,  //  014E  JMP		#016A
      0x1C2C153A,  //  014F  EQ	R11	R10	K58
      0x782E0010,  //  0150  JMPF	R11	#0162
      0x882C0142,  //  0151  GETMBR	R11	R0	K66
      0x8C2C172E,  //  0152  GETMET	R11	R11	K46
      0x5C340C00,  //  0153  MOVE	R13	R6
      0x7C2C0400,  //  0154  CALL	R11	2
      0x5C141600,  //  0155  MOVE	R5	R11
      0x78160004,  //  0156  JMPF	R5	#015C
      0x882C0142,  //  0157  GETMBR	R11	R0	K66
      0x8C2C1744,  //  0158  GETMET	R11	R11	K68
      0x5C340000,  //  0159  MOVE	R13	R0
      0x7C2C0400,  //  015A  CALL	R11	2
      0x70020003,  //  015B  JMP		#0160
      0x8C2C0136,  //  015C  GETMET	R11	R0	K54
      0x5C340C00,  //  015D  MOVE	R13	R6
      0x50380200,  //  015E  LDBOOL	R14	1	0
      0x7C2C0600,  //  015F  CALL	R11	3
      0x50140200,  //  0160  LDBOOL	R5	1	0
      0x70020007,  //  0161  JMP		#016A
      0xB82E0A00,  //  0162  GETNGBL	R11	K5
      0x8C2C1706,  //  0163  GETMET	R11	R11	K6
      0x60340008,  //  0164  GETGBL	R13	G8
      0x5C381400,  //  0165  MOVE	R14	R10
      0x7C340200,  //  0166  CALL	R13	1
      0x00368A0D,  //  0167  ADD	R13	K69	R13
      0x5838001E,  //  0168  LDCONST	R14	K30
      0x7C2C0600,  //  0169  CALL	R11	3
      0xA8040001,  //  016A  EXBLK	1	1
      0x80040A00,  //  016B  RET	1	R5
      0xA8040001,  //  016C  EXBLK	1	1
      0x70020014,  //  016D  JMP		#0183
      0xAC180002,  //  016E  CATCH	R6	0	2
      0x70020011,  //  016F  JMP		#0182
      0xB8220A00,  //  0170  GETNGBL	R8	K5
      0x8C201106,  //  0171  GETMET	R8	R8	K6
      0x60280008,  //  0172  GETGBL	R10	G8
      0x5C2C0C00,  //  0173  MOVE	R11	R6
      0x7C280200,  //  0174  CALL	R10	1
      0x002A8C0A,  //  0175  ADD	R10	K70	R10
      0x00281547,  //  0176  ADD	R10	R10	K71
      0x602C0008,  //  0177  GETGBL	R11	G8
      0x5C300E00,  //  0178  MOVE	R12	R7
      0x7C2C0200,  //  0179  CALL	R11	1
      0x0028140B,  //  017A  ADD	R10	R10	R11
      0x7C200400,  //  017B  CALL	R8	2
      0xA4229000,  //  017C  IMPORT	R8	K72
      0x8C241149,  //  017D  GETMET	R9	R8	K73
      0x7C240200,  //  017E  CALL	R9	1
      0x50240000,  //  017F  LDBOOL	R9	0	0
      0x80041200,  //  0180  RET	1	R9
      0x70020000,  //  0181  JMP		#0183
      0xB0080000,  //  0182  RAISE	2	R0	R0
      0x80000000,  //  0183  RET	0
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
