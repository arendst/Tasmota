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
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(x_flag_r),
    /* K1   */  be_nested_str_weak(build_standalone_ack),
    /* K2   */  be_nested_str_weak(encode_frame),
    /* K3   */  be_nested_str_weak(encrypt),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X2A_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X25s),
    /* K7   */  be_nested_str_weak(session),
    /* K8   */  be_nested_str_weak(local_session_id),
    /* K9   */  be_nested_str_weak(ack_message_counter),
    /* K10  */  be_nested_str_weak(message_counter),
    /* K11  */  be_nested_str_weak(_X7Breliable_X7D),
    /* K12  */  be_nested_str_weak(),
    /* K13  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(send_encrypted_ack),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x880C0300,  //  0000  GETMBR	R3	R1	K0
      0x780E0018,  //  0001  JMPF	R3	#001B
      0x8C0C0301,  //  0002  GETMET	R3	R1	K1
      0x5C140400,  //  0003  MOVE	R5	R2
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C100702,  //  0005  GETMET	R4	R3	K2
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100703,  //  0007  GETMET	R4	R3	K3
      0x7C100200,  //  0008  CALL	R4	1
      0xB8120800,  //  0009  GETNGBL	R4	K4
      0x8C100905,  //  000A  GETMET	R4	R4	K5
      0x60180018,  //  000B  GETGBL	R6	G24
      0x581C0006,  //  000C  LDCONST	R7	K6
      0x88200707,  //  000D  GETMBR	R8	R3	K7
      0x88201108,  //  000E  GETMBR	R8	R8	K8
      0x88240709,  //  000F  GETMBR	R9	R3	K9
      0x8828070A,  //  0010  GETMBR	R10	R3	K10
      0x780A0001,  //  0011  JMPF	R2	#0014
      0x582C000B,  //  0012  LDCONST	R11	K11
      0x70020000,  //  0013  JMP		#0015
      0x582C000C,  //  0014  LDCONST	R11	K12
      0x7C180A00,  //  0015  CALL	R6	5
      0x541E0003,  //  0016  LDINT	R7	4
      0x7C100600,  //  0017  CALL	R4	3
      0x8C10010D,  //  0018  GETMET	R4	R0	K13
      0x5C180600,  //  0019  MOVE	R6	R3
      0x7C100400,  //  001A  CALL	R4	2
      0x80000000,  //  001B  RET	0
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
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(x_flag_r),
    /* K1   */  be_nested_str_weak(build_standalone_ack),
    /* K2   */  be_nested_str_weak(encode_frame),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X25s),
    /* K6   */  be_nested_str_weak(session),
    /* K7   */  be_nested_str_weak(local_session_id),
    /* K8   */  be_nested_str_weak(ack_message_counter),
    /* K9   */  be_nested_str_weak(message_counter),
    /* K10  */  be_nested_str_weak(_X7Breliable_X7D),
    /* K11  */  be_nested_str_weak(),
    /* K12  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(send_simple_ack),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x880C0300,  //  0000  GETMBR	R3	R1	K0
      0x780E0016,  //  0001  JMPF	R3	#0019
      0x8C0C0301,  //  0002  GETMET	R3	R1	K1
      0x5C140400,  //  0003  MOVE	R5	R2
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C100702,  //  0005  GETMET	R4	R3	K2
      0x7C100200,  //  0006  CALL	R4	1
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x8C100904,  //  0008  GETMET	R4	R4	K4
      0x60180018,  //  0009  GETGBL	R6	G24
      0x581C0005,  //  000A  LDCONST	R7	K5
      0x88200706,  //  000B  GETMBR	R8	R3	K6
      0x88201107,  //  000C  GETMBR	R8	R8	K7
      0x88240708,  //  000D  GETMBR	R9	R3	K8
      0x88280709,  //  000E  GETMBR	R10	R3	K9
      0x780A0001,  //  000F  JMPF	R2	#0012
      0x582C000A,  //  0010  LDCONST	R11	K10
      0x70020000,  //  0011  JMP		#0013
      0x582C000B,  //  0012  LDCONST	R11	K11
      0x7C180A00,  //  0013  CALL	R6	5
      0x541E0003,  //  0014  LDINT	R7	4
      0x7C100600,  //  0015  CALL	R4	3
      0x8C10010C,  //  0016  GETMET	R4	R0	K12
      0x5C180600,  //  0017  MOVE	R6	R3
      0x7C100400,  //  0018  CALL	R4	2
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(Matter_MessageHandler_msg_received,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[66]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Frame),
    /* K2   */  be_nested_str_weak(decode_header),
    /* K3   */  be_nested_str_weak(sec_p),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(sessions),
    /* K6   */  be_nested_str_weak(find_session_source_id_unsecure),
    /* K7   */  be_nested_str_weak(source_node_id),
    /* K8   */  be_nested_str_weak(control_message),
    /* K9   */  be_nested_str_weak(process_incoming_control_message),
    /* K10  */  be_nested_str_weak(local_session_id),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(sec_sesstype),
    /* K13  */  be_nested_str_weak(_ip),
    /* K14  */  be_nested_str_weak(_port),
    /* K15  */  be_nested_str_weak(_message_handler),
    /* K16  */  be_nested_str_weak(session),
    /* K17  */  be_nested_str_weak(_counter_insecure_rcv),
    /* K18  */  be_nested_str_weak(validate),
    /* K19  */  be_nested_str_weak(message_counter),
    /* K20  */  be_nested_str_weak(tasmota),
    /* K21  */  be_nested_str_weak(log),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K23  */  be_nested_str_weak(val),
    /* K24  */  be_nested_str_weak(send_simple_ack),
    /* K25  */  be_nested_str_weak(decode_payload),
    /* K26  */  be_nested_str_weak(received_ack),
    /* K27  */  be_nested_str_weak(opcode),
    /* K28  */  be_nested_str_weak(get_opcode_name),
    /* K29  */  be_nested_str_weak(0x_X2502X),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20rid_X3D_X25i_X20exch_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K31  */  be_nested_str_weak(exchange_id),
    /* K32  */  be_const_int(3),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K34  */  be_nested_str_weak(x_flag_r),
    /* K35  */  be_nested_str_weak(_X7Breliable_X7D_X20),
    /* K36  */  be_nested_str_weak(),
    /* K37  */  be_nested_str_weak(ack_message_counter),
    /* K38  */  be_nested_str_weak(commissioning),
    /* K39  */  be_nested_str_weak(process_incoming),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K41  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
    /* K43  */  be_nested_str_weak(counter_rcv_validate),
    /* K44  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K45  */  be_nested_str_weak(_X20counter_X3D),
    /* K46  */  be_nested_str_weak(counter_rcv),
    /* K47  */  be_nested_str_weak(send_encrypted_ack),
    /* K48  */  be_nested_str_weak(decrypt),
    /* K49  */  be_nested_str_weak(raw),
    /* K50  */  be_nested_str_weak(payload_idx),
    /* K51  */  be_const_int(1),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20_X3E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K53  */  be_nested_str_weak(protocol_id),
    /* K54  */  be_nested_str_weak(_X20opcode_X3D),
    /* K55  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K56  */  be_nested_str_weak(im),
    /* K57  */  be_nested_str_weak(process_incoming_ack),
    /* K58  */  be_nested_str_weak(send_enqueued),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K60  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K61  */  be_nested_str_weak(_X3B),
    /* K62  */  be_const_int(2),
    /* K63  */  be_nested_str_weak(_debug_present),
    /* K64  */  be_nested_str_weak(debug),
    /* K65  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[323]) {  /* code */
      0x50100000,  //  0000  LDBOOL	R4	0	0
      0xA8020126,  //  0001  EXBLK	0	#0129
      0xB8160000,  //  0002  GETNGBL	R5	K0
      0x8C140B01,  //  0003  GETMET	R5	R5	K1
      0x5C1C0000,  //  0004  MOVE	R7	R0
      0x5C200200,  //  0005  MOVE	R8	R1
      0x5C240400,  //  0006  MOVE	R9	R2
      0x5C280600,  //  0007  MOVE	R10	R3
      0x7C140A00,  //  0008  CALL	R5	5
      0x8C180B02,  //  0009  GETMET	R6	R5	K2
      0x7C180200,  //  000A  CALL	R6	1
      0x5C1C0C00,  //  000B  MOVE	R7	R6
      0x741E0002,  //  000C  JMPT	R7	#0010
      0x501C0000,  //  000D  LDBOOL	R7	0	0
      0xA8040001,  //  000E  EXBLK	1	1
      0x80040E00,  //  000F  RET	1	R7
      0x881C0B03,  //  0010  GETMBR	R7	R5	K3
      0x781E000C,  //  0011  JMPF	R7	#001F
      0x881C0104,  //  0012  GETMBR	R7	R0	K4
      0x881C0F05,  //  0013  GETMBR	R7	R7	K5
      0x8C1C0F06,  //  0014  GETMET	R7	R7	K6
      0x88240B07,  //  0015  GETMBR	R9	R5	K7
      0x542A0059,  //  0016  LDINT	R10	90
      0x7C1C0600,  //  0017  CALL	R7	3
      0x88200108,  //  0018  GETMBR	R8	R0	K8
      0x8C201109,  //  0019  GETMET	R8	R8	K9
      0x5C280A00,  //  001A  MOVE	R10	R5
      0x7C200400,  //  001B  CALL	R8	2
      0xA8040001,  //  001C  EXBLK	1	1
      0x80041000,  //  001D  RET	1	R8
      0x70020105,  //  001E  JMP		#0125
      0x881C0B0A,  //  001F  GETMBR	R7	R5	K10
      0x1C1C0F0B,  //  0020  EQ	R7	R7	K11
      0x781E0070,  //  0021  JMPF	R7	#0093
      0x881C0B0C,  //  0022  GETMBR	R7	R5	K12
      0x1C1C0F0B,  //  0023  EQ	R7	R7	K11
      0x781E006D,  //  0024  JMPF	R7	#0093
      0x881C0104,  //  0025  GETMBR	R7	R0	K4
      0x881C0F05,  //  0026  GETMBR	R7	R7	K5
      0x8C1C0F06,  //  0027  GETMET	R7	R7	K6
      0x88240B07,  //  0028  GETMBR	R9	R5	K7
      0x542A0059,  //  0029  LDINT	R10	90
      0x7C1C0600,  //  002A  CALL	R7	3
      0x780A0000,  //  002B  JMPF	R2	#002D
      0x901E1A02,  //  002C  SETMBR	R7	K13	R2
      0x780E0000,  //  002D  JMPF	R3	#002F
      0x901E1C03,  //  002E  SETMBR	R7	K14	R3
      0x901E1E00,  //  002F  SETMBR	R7	K15	R0
      0x90162007,  //  0030  SETMBR	R5	K16	R7
      0x88200F11,  //  0031  GETMBR	R8	R7	K17
      0x8C201112,  //  0032  GETMET	R8	R8	K18
      0x88280B13,  //  0033  GETMBR	R10	R5	K19
      0x502C0000,  //  0034  LDBOOL	R11	0	0
      0x7C200600,  //  0035  CALL	R8	3
      0x74220011,  //  0036  JMPT	R8	#0049
      0xB8222800,  //  0037  GETNGBL	R8	K20
      0x8C201115,  //  0038  GETMET	R8	R8	K21
      0x60280018,  //  0039  GETGBL	R10	G24
      0x582C0016,  //  003A  LDCONST	R11	K22
      0x88300B13,  //  003B  GETMBR	R12	R5	K19
      0x88340F11,  //  003C  GETMBR	R13	R7	K17
      0x8C341B17,  //  003D  GETMET	R13	R13	K23
      0x7C340200,  //  003E  CALL	R13	1
      0x7C280600,  //  003F  CALL	R10	3
      0x542E0003,  //  0040  LDINT	R11	4
      0x7C200600,  //  0041  CALL	R8	3
      0x8C200118,  //  0042  GETMET	R8	R0	K24
      0x5C280A00,  //  0043  MOVE	R10	R5
      0x502C0000,  //  0044  LDBOOL	R11	0	0
      0x7C200600,  //  0045  CALL	R8	3
      0x50200000,  //  0046  LDBOOL	R8	0	0
      0xA8040001,  //  0047  EXBLK	1	1
      0x80041000,  //  0048  RET	1	R8
      0x8C200B19,  //  0049  GETMET	R8	R5	K25
      0x7C200200,  //  004A  CALL	R8	1
      0x74220002,  //  004B  JMPT	R8	#004F
      0x50200000,  //  004C  LDBOOL	R8	0	0
      0xA8040001,  //  004D  EXBLK	1	1
      0x80041000,  //  004E  RET	1	R8
      0x88200104,  //  004F  GETMBR	R8	R0	K4
      0x8C20111A,  //  0050  GETMET	R8	R8	K26
      0x5C280A00,  //  0051  MOVE	R10	R5
      0x7C200400,  //  0052  CALL	R8	2
      0x88200B1B,  //  0053  GETMBR	R8	R5	K27
      0x5426000F,  //  0054  LDINT	R9	16
      0x20201009,  //  0055  NE	R8	R8	R9
      0x78220018,  //  0056  JMPF	R8	#0070
      0xB8220000,  //  0057  GETNGBL	R8	K0
      0x8C20111C,  //  0058  GETMET	R8	R8	K28
      0x88280B1B,  //  0059  GETMBR	R10	R5	K27
      0x7C200400,  //  005A  CALL	R8	2
      0x5C241000,  //  005B  MOVE	R9	R8
      0x74260004,  //  005C  JMPT	R9	#0062
      0x60240018,  //  005D  GETGBL	R9	G24
      0x5828001D,  //  005E  LDCONST	R10	K29
      0x882C0B1B,  //  005F  GETMBR	R11	R5	K27
      0x7C240400,  //  0060  CALL	R9	2
      0x5C201200,  //  0061  MOVE	R8	R9
      0xB8262800,  //  0062  GETNGBL	R9	K20
      0x8C241315,  //  0063  GETMET	R9	R9	K21
      0x602C0018,  //  0064  GETGBL	R11	G24
      0x5830001E,  //  0065  LDCONST	R12	K30
      0x88340F0A,  //  0066  GETMBR	R13	R7	K10
      0x5C381000,  //  0067  MOVE	R14	R8
      0x883C0B13,  //  0068  GETMBR	R15	R5	K19
      0x88400B1F,  //  0069  GETMBR	R16	R5	K31
      0x5C440400,  //  006A  MOVE	R17	R2
      0x5C480600,  //  006B  MOVE	R18	R3
      0x7C2C0E00,  //  006C  CALL	R11	7
      0x58300020,  //  006D  LDCONST	R12	K32
      0x7C240600,  //  006E  CALL	R9	3
      0x70020013,  //  006F  JMP		#0084
      0xB8222800,  //  0070  GETNGBL	R8	K20
      0x8C201115,  //  0071  GETMET	R8	R8	K21
      0x60280018,  //  0072  GETGBL	R10	G24
      0x582C0021,  //  0073  LDCONST	R11	K33
      0x88300F0A,  //  0074  GETMBR	R12	R7	K10
      0x88340B13,  //  0075  GETMBR	R13	R5	K19
      0x88380B22,  //  0076  GETMBR	R14	R5	K34
      0x783A0001,  //  0077  JMPF	R14	#007A
      0x58380023,  //  0078  LDCONST	R14	K35
      0x70020000,  //  0079  JMP		#007B
      0x58380024,  //  007A  LDCONST	R14	K36
      0x883C0B1F,  //  007B  GETMBR	R15	R5	K31
      0x60400008,  //  007C  GETGBL	R16	G8
      0x88440B25,  //  007D  GETMBR	R17	R5	K37
      0x7C400200,  //  007E  CALL	R16	1
      0x5C440400,  //  007F  MOVE	R17	R2
      0x5C480600,  //  0080  MOVE	R18	R3
      0x7C281000,  //  0081  CALL	R10	8
      0x542E0003,  //  0082  LDINT	R11	4
      0x7C200600,  //  0083  CALL	R8	3
      0x88200126,  //  0084  GETMBR	R8	R0	K38
      0x8C201127,  //  0085  GETMET	R8	R8	K39
      0x5C280A00,  //  0086  MOVE	R10	R5
      0x7C200400,  //  0087  CALL	R8	2
      0x5C101000,  //  0088  MOVE	R4	R8
      0x5C200800,  //  0089  MOVE	R8	R4
      0x74220003,  //  008A  JMPT	R8	#008F
      0x8C200118,  //  008B  GETMET	R8	R0	K24
      0x5C280A00,  //  008C  MOVE	R10	R5
      0x502C0000,  //  008D  LDBOOL	R11	0	0
      0x7C200600,  //  008E  CALL	R8	3
      0x50200200,  //  008F  LDBOOL	R8	1	0
      0xA8040001,  //  0090  EXBLK	1	1
      0x80041000,  //  0091  RET	1	R8
      0x70020091,  //  0092  JMP		#0125
      0xB81E2800,  //  0093  GETNGBL	R7	K20
      0x8C1C0F15,  //  0094  GETMET	R7	R7	K21
      0x60240018,  //  0095  GETGBL	R9	G24
      0x58280028,  //  0096  LDCONST	R10	K40
      0x882C0B0A,  //  0097  GETMBR	R11	R5	K10
      0x88300B13,  //  0098  GETMBR	R12	R5	K19
      0x7C240600,  //  0099  CALL	R9	3
      0x542A0003,  //  009A  LDINT	R10	4
      0x7C1C0600,  //  009B  CALL	R7	3
      0x881C0104,  //  009C  GETMBR	R7	R0	K4
      0x881C0F05,  //  009D  GETMBR	R7	R7	K5
      0x8C1C0F29,  //  009E  GETMET	R7	R7	K41
      0x88240B0A,  //  009F  GETMBR	R9	R5	K10
      0x7C1C0400,  //  00A0  CALL	R7	2
      0x4C200000,  //  00A1  LDNIL	R8
      0x1C200E08,  //  00A2  EQ	R8	R7	R8
      0x7822000A,  //  00A3  JMPF	R8	#00AF
      0xB8222800,  //  00A4  GETNGBL	R8	K20
      0x8C201115,  //  00A5  GETMET	R8	R8	K21
      0x60280008,  //  00A6  GETGBL	R10	G8
      0x882C0B0A,  //  00A7  GETMBR	R11	R5	K10
      0x7C280200,  //  00A8  CALL	R10	1
      0x002A540A,  //  00A9  ADD	R10	K42	R10
      0x582C0020,  //  00AA  LDCONST	R11	K32
      0x7C200600,  //  00AB  CALL	R8	3
      0x50200000,  //  00AC  LDBOOL	R8	0	0
      0xA8040001,  //  00AD  EXBLK	1	1
      0x80041000,  //  00AE  RET	1	R8
      0x780A0000,  //  00AF  JMPF	R2	#00B1
      0x901E1A02,  //  00B0  SETMBR	R7	K13	R2
      0x780E0000,  //  00B1  JMPF	R3	#00B3
      0x901E1C03,  //  00B2  SETMBR	R7	K14	R3
      0x901E1E00,  //  00B3  SETMBR	R7	K15	R0
      0x90162007,  //  00B4  SETMBR	R5	K16	R7
      0x8C200F2B,  //  00B5  GETMET	R8	R7	K43
      0x88280B13,  //  00B6  GETMBR	R10	R5	K19
      0x502C0200,  //  00B7  LDBOOL	R11	1	0
      0x7C200600,  //  00B8  CALL	R8	3
      0x74220013,  //  00B9  JMPT	R8	#00CE
      0xB8222800,  //  00BA  GETNGBL	R8	K20
      0x8C201115,  //  00BB  GETMET	R8	R8	K21
      0x60280008,  //  00BC  GETGBL	R10	G8
      0x882C0B13,  //  00BD  GETMBR	R11	R5	K19
      0x7C280200,  //  00BE  CALL	R10	1
      0x002A580A,  //  00BF  ADD	R10	K44	R10
      0x0028152D,  //  00C0  ADD	R10	R10	K45
      0x602C0008,  //  00C1  GETGBL	R11	G8
      0x88300F2E,  //  00C2  GETMBR	R12	R7	K46
      0x7C2C0200,  //  00C3  CALL	R11	1
      0x0028140B,  //  00C4  ADD	R10	R10	R11
      0x542E0003,  //  00C5  LDINT	R11	4
      0x7C200600,  //  00C6  CALL	R8	3
      0x8C20012F,  //  00C7  GETMET	R8	R0	K47
      0x5C280A00,  //  00C8  MOVE	R10	R5
      0x502C0000,  //  00C9  LDBOOL	R11	0	0
      0x7C200600,  //  00CA  CALL	R8	3
      0x50200000,  //  00CB  LDBOOL	R8	0	0
      0xA8040001,  //  00CC  EXBLK	1	1
      0x80041000,  //  00CD  RET	1	R8
      0x8C200B30,  //  00CE  GETMET	R8	R5	K48
      0x7C200200,  //  00CF  CALL	R8	1
      0x5C241000,  //  00D0  MOVE	R9	R8
      0x74260002,  //  00D1  JMPT	R9	#00D5
      0x50240000,  //  00D2  LDBOOL	R9	0	0
      0xA8040001,  //  00D3  EXBLK	1	1
      0x80041200,  //  00D4  RET	1	R9
      0x88240B32,  //  00D5  GETMBR	R9	R5	K50
      0x04241333,  //  00D6  SUB	R9	R9	K51
      0x40261609,  //  00D7  CONNECT	R9	K11	R9
      0x88280B31,  //  00D8  GETMBR	R10	R5	K49
      0x94241409,  //  00D9  GETIDX	R9	R10	R9
      0x90166209,  //  00DA  SETMBR	R5	K49	R9
      0x88240B31,  //  00DB  GETMBR	R9	R5	K49
      0x40241208,  //  00DC  CONNECT	R9	R9	R8
      0x8C240B19,  //  00DD  GETMET	R9	R5	K25
      0x7C240200,  //  00DE  CALL	R9	1
      0xB8262800,  //  00DF  GETNGBL	R9	K20
      0x8C241315,  //  00E0  GETMET	R9	R9	K21
      0x602C0008,  //  00E1  GETGBL	R11	G8
      0x88300B35,  //  00E2  GETMBR	R12	R5	K53
      0x7C2C0200,  //  00E3  CALL	R11	1
      0x002E680B,  //  00E4  ADD	R11	K52	R11
      0x002C1736,  //  00E5  ADD	R11	R11	K54
      0x60300008,  //  00E6  GETGBL	R12	G8
      0x88340B1B,  //  00E7  GETMBR	R13	R5	K27
      0x7C300200,  //  00E8  CALL	R12	1
      0x002C160C,  //  00E9  ADD	R11	R11	R12
      0x002C1737,  //  00EA  ADD	R11	R11	K55
      0x60300008,  //  00EB  GETGBL	R12	G8
      0x88340B1F,  //  00EC  GETMBR	R13	R5	K31
      0x543AFFFE,  //  00ED  LDINT	R14	65535
      0x2C341A0E,  //  00EE  AND	R13	R13	R14
      0x7C300200,  //  00EF  CALL	R12	1
      0x002C160C,  //  00F0  ADD	R11	R11	R12
      0x54320003,  //  00F1  LDINT	R12	4
      0x7C240600,  //  00F2  CALL	R9	3
      0x88240104,  //  00F3  GETMBR	R9	R0	K4
      0x8C24131A,  //  00F4  GETMET	R9	R9	K26
      0x5C2C0A00,  //  00F5  MOVE	R11	R5
      0x7C240400,  //  00F6  CALL	R9	2
      0x88240B35,  //  00F7  GETMBR	R9	R5	K53
      0x1C28130B,  //  00F8  EQ	R10	R9	K11
      0x782A000F,  //  00F9  JMPF	R10	#010A
      0x88280B1B,  //  00FA  GETMBR	R10	R5	K27
      0x542E000F,  //  00FB  LDINT	R11	16
      0x1C28140B,  //  00FC  EQ	R10	R10	R11
      0x782A0009,  //  00FD  JMPF	R10	#0108
      0x88280138,  //  00FE  GETMBR	R10	R0	K56
      0x8C281539,  //  00FF  GETMET	R10	R10	K57
      0x5C300A00,  //  0100  MOVE	R12	R5
      0x7C280400,  //  0101  CALL	R10	2
      0x5C101400,  //  0102  MOVE	R4	R10
      0x78120003,  //  0103  JMPF	R4	#0108
      0x88280138,  //  0104  GETMBR	R10	R0	K56
      0x8C28153A,  //  0105  GETMET	R10	R10	K58
      0x5C300000,  //  0106  MOVE	R12	R0
      0x7C280400,  //  0107  CALL	R10	2
      0x50100200,  //  0108  LDBOOL	R4	1	0
      0x7002001A,  //  0109  JMP		#0125
      0x1C281333,  //  010A  EQ	R10	R9	K51
      0x782A0010,  //  010B  JMPF	R10	#011D
      0x88280138,  //  010C  GETMBR	R10	R0	K56
      0x8C281527,  //  010D  GETMET	R10	R10	K39
      0x5C300A00,  //  010E  MOVE	R12	R5
      0x7C280400,  //  010F  CALL	R10	2
      0x5C101400,  //  0110  MOVE	R4	R10
      0x78120004,  //  0111  JMPF	R4	#0117
      0x88280138,  //  0112  GETMBR	R10	R0	K56
      0x8C28153A,  //  0113  GETMET	R10	R10	K58
      0x5C300000,  //  0114  MOVE	R12	R0
      0x7C280400,  //  0115  CALL	R10	2
      0x70020003,  //  0116  JMP		#011B
      0x8C28012F,  //  0117  GETMET	R10	R0	K47
      0x5C300A00,  //  0118  MOVE	R12	R5
      0x50340200,  //  0119  LDBOOL	R13	1	0
      0x7C280600,  //  011A  CALL	R10	3
      0x50100200,  //  011B  LDBOOL	R4	1	0
      0x70020007,  //  011C  JMP		#0125
      0xB82A2800,  //  011D  GETNGBL	R10	K20
      0x8C281515,  //  011E  GETMET	R10	R10	K21
      0x60300008,  //  011F  GETGBL	R12	G8
      0x5C341200,  //  0120  MOVE	R13	R9
      0x7C300200,  //  0121  CALL	R12	1
      0x0032760C,  //  0122  ADD	R12	K59	R12
      0x58340020,  //  0123  LDCONST	R13	K32
      0x7C280600,  //  0124  CALL	R10	3
      0xA8040001,  //  0125  EXBLK	1	1
      0x80040800,  //  0126  RET	1	R4
      0xA8040001,  //  0127  EXBLK	1	1
      0x70020018,  //  0128  JMP		#0142
      0xAC140002,  //  0129  CATCH	R5	0	2
      0x70020015,  //  012A  JMP		#0141
      0xB81E2800,  //  012B  GETNGBL	R7	K20
      0x8C1C0F15,  //  012C  GETMET	R7	R7	K21
      0x60240008,  //  012D  GETGBL	R9	G8
      0x5C280A00,  //  012E  MOVE	R10	R5
      0x7C240200,  //  012F  CALL	R9	1
      0x00267809,  //  0130  ADD	R9	K60	R9
      0x0024133D,  //  0131  ADD	R9	R9	K61
      0x60280008,  //  0132  GETGBL	R10	G8
      0x5C2C0C00,  //  0133  MOVE	R11	R6
      0x7C280200,  //  0134  CALL	R10	1
      0x0024120A,  //  0135  ADD	R9	R9	R10
      0x5828003E,  //  0136  LDCONST	R10	K62
      0x7C1C0600,  //  0137  CALL	R7	3
      0xB81E2800,  //  0138  GETNGBL	R7	K20
      0x881C0F3F,  //  0139  GETMBR	R7	R7	K63
      0x781E0002,  //  013A  JMPF	R7	#013E
      0xA41E8000,  //  013B  IMPORT	R7	K64
      0x8C200F41,  //  013C  GETMET	R8	R7	K65
      0x7C200200,  //  013D  CALL	R8	1
      0x501C0000,  //  013E  LDBOOL	R7	0	0
      0x80040E00,  //  013F  RET	1	R7
      0x70020000,  //  0140  JMP		#0142
      0xB0080000,  //  0141  RAISE	2	R0	R0
      0x80000000,  //  0142  RET	0
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
