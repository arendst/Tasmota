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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(send_response_frame),
    /* K4   */  be_nested_str_weak(msg_send),
    }),
    be_str_weak(send_response_frame),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x7C080400,  //  0004  CALL	R2	2
      0x88080100,  //  0005  GETMBR	R2	R0	K0
      0x8C080504,  //  0006  GETMET	R2	R2	K4
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x80000000,  //  0009  RET	0
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
    ( &(const bvalue[69]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(msg_received),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(Frame),
    /* K6   */  be_nested_str_weak(decode_header),
    /* K7   */  be_nested_str_weak(sec_p),
    /* K8   */  be_nested_str_weak(sessions),
    /* K9   */  be_nested_str_weak(find_session_source_id_unsecure),
    /* K10  */  be_nested_str_weak(source_node_id),
    /* K11  */  be_nested_str_weak(control_message),
    /* K12  */  be_nested_str_weak(process_incoming_control_message),
    /* K13  */  be_nested_str_weak(local_session_id),
    /* K14  */  be_const_int(0),
    /* K15  */  be_nested_str_weak(sec_sesstype),
    /* K16  */  be_nested_str_weak(_ip),
    /* K17  */  be_nested_str_weak(_port),
    /* K18  */  be_nested_str_weak(_message_handler),
    /* K19  */  be_nested_str_weak(session),
    /* K20  */  be_nested_str_weak(_counter_insecure_rcv),
    /* K21  */  be_nested_str_weak(validate),
    /* K22  */  be_nested_str_weak(message_counter),
    /* K23  */  be_nested_str_weak(tasmota),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K25  */  be_nested_str_weak(val),
    /* K26  */  be_nested_str_weak(send_simple_ack),
    /* K27  */  be_nested_str_weak(decode_payload),
    /* K28  */  be_nested_str_weak(received_ack),
    /* K29  */  be_nested_str_weak(opcode),
    /* K30  */  be_nested_str_weak(get_opcode_name),
    /* K31  */  be_nested_str_weak(0x_X2502X),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20rid_X3D_X25i_X20exch_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K33  */  be_nested_str_weak(exchange_id),
    /* K34  */  be_const_int(3),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K36  */  be_nested_str_weak(x_flag_r),
    /* K37  */  be_nested_str_weak(_X7Breliable_X7D_X20),
    /* K38  */  be_nested_str_weak(),
    /* K39  */  be_nested_str_weak(ack_message_counter),
    /* K40  */  be_nested_str_weak(commissioning),
    /* K41  */  be_nested_str_weak(process_incoming),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K43  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K44  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
    /* K45  */  be_nested_str_weak(counter_rcv_validate),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K47  */  be_nested_str_weak(_X20counter_X3D),
    /* K48  */  be_nested_str_weak(counter_rcv),
    /* K49  */  be_nested_str_weak(send_encrypted_ack),
    /* K50  */  be_nested_str_weak(decrypt),
    /* K51  */  be_nested_str_weak(raw),
    /* K52  */  be_nested_str_weak(payload_idx),
    /* K53  */  be_const_int(1),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20_X3E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K55  */  be_nested_str_weak(protocol_id),
    /* K56  */  be_nested_str_weak(_X20opcode_X3D),
    /* K57  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K58  */  be_nested_str_weak(im),
    /* K59  */  be_nested_str_weak(process_incoming_ack),
    /* K60  */  be_nested_str_weak(send_enqueued),
    /* K61  */  be_nested_str_weak(process_IM_end),
    /* K62  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K63  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K64  */  be_nested_str_weak(_X3B),
    /* K65  */  be_const_int(2),
    /* K66  */  be_nested_str_weak(_debug_present),
    /* K67  */  be_nested_str_weak(debug),
    /* K68  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[333]) {  /* code */
      0x50100000,  //  0000  LDBOOL	R4	0	0
      0x88140100,  //  0001  GETMBR	R5	R0	K0
      0x88140B01,  //  0002  GETMBR	R5	R5	K1
      0x8C140B02,  //  0003  GETMET	R5	R5	K2
      0x581C0003,  //  0004  LDCONST	R7	K3
      0x7C140400,  //  0005  CALL	R5	2
      0xA802012B,  //  0006  EXBLK	0	#0133
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x5C1C0000,  //  0009  MOVE	R7	R0
      0x5C200200,  //  000A  MOVE	R8	R1
      0x5C240400,  //  000B  MOVE	R9	R2
      0x5C280600,  //  000C  MOVE	R10	R3
      0x7C140A00,  //  000D  CALL	R5	5
      0x8C180B06,  //  000E  GETMET	R6	R5	K6
      0x7C180200,  //  000F  CALL	R6	1
      0x5C1C0C00,  //  0010  MOVE	R7	R6
      0x741E0002,  //  0011  JMPT	R7	#0015
      0x501C0000,  //  0012  LDBOOL	R7	0	0
      0xA8040001,  //  0013  EXBLK	1	1
      0x80040E00,  //  0014  RET	1	R7
      0x881C0B07,  //  0015  GETMBR	R7	R5	K7
      0x781E000C,  //  0016  JMPF	R7	#0024
      0x881C0100,  //  0017  GETMBR	R7	R0	K0
      0x881C0F08,  //  0018  GETMBR	R7	R7	K8
      0x8C1C0F09,  //  0019  GETMET	R7	R7	K9
      0x88240B0A,  //  001A  GETMBR	R9	R5	K10
      0x542A0059,  //  001B  LDINT	R10	90
      0x7C1C0600,  //  001C  CALL	R7	3
      0x8820010B,  //  001D  GETMBR	R8	R0	K11
      0x8C20110C,  //  001E  GETMET	R8	R8	K12
      0x5C280A00,  //  001F  MOVE	R10	R5
      0x7C200400,  //  0020  CALL	R8	2
      0xA8040001,  //  0021  EXBLK	1	1
      0x80041000,  //  0022  RET	1	R8
      0x7002010A,  //  0023  JMP		#012F
      0x881C0B0D,  //  0024  GETMBR	R7	R5	K13
      0x1C1C0F0E,  //  0025  EQ	R7	R7	K14
      0x781E0070,  //  0026  JMPF	R7	#0098
      0x881C0B0F,  //  0027  GETMBR	R7	R5	K15
      0x1C1C0F0E,  //  0028  EQ	R7	R7	K14
      0x781E006D,  //  0029  JMPF	R7	#0098
      0x881C0100,  //  002A  GETMBR	R7	R0	K0
      0x881C0F08,  //  002B  GETMBR	R7	R7	K8
      0x8C1C0F09,  //  002C  GETMET	R7	R7	K9
      0x88240B0A,  //  002D  GETMBR	R9	R5	K10
      0x542A0059,  //  002E  LDINT	R10	90
      0x7C1C0600,  //  002F  CALL	R7	3
      0x780A0000,  //  0030  JMPF	R2	#0032
      0x901E2002,  //  0031  SETMBR	R7	K16	R2
      0x780E0000,  //  0032  JMPF	R3	#0034
      0x901E2203,  //  0033  SETMBR	R7	K17	R3
      0x901E2400,  //  0034  SETMBR	R7	K18	R0
      0x90162607,  //  0035  SETMBR	R5	K19	R7
      0x88200F14,  //  0036  GETMBR	R8	R7	K20
      0x8C201115,  //  0037  GETMET	R8	R8	K21
      0x88280B16,  //  0038  GETMBR	R10	R5	K22
      0x502C0000,  //  0039  LDBOOL	R11	0	0
      0x7C200600,  //  003A  CALL	R8	3
      0x74220011,  //  003B  JMPT	R8	#004E
      0xB8222E00,  //  003C  GETNGBL	R8	K23
      0x8C201102,  //  003D  GETMET	R8	R8	K2
      0x60280018,  //  003E  GETGBL	R10	G24
      0x582C0018,  //  003F  LDCONST	R11	K24
      0x88300B16,  //  0040  GETMBR	R12	R5	K22
      0x88340F14,  //  0041  GETMBR	R13	R7	K20
      0x8C341B19,  //  0042  GETMET	R13	R13	K25
      0x7C340200,  //  0043  CALL	R13	1
      0x7C280600,  //  0044  CALL	R10	3
      0x542E0003,  //  0045  LDINT	R11	4
      0x7C200600,  //  0046  CALL	R8	3
      0x8C20011A,  //  0047  GETMET	R8	R0	K26
      0x5C280A00,  //  0048  MOVE	R10	R5
      0x502C0000,  //  0049  LDBOOL	R11	0	0
      0x7C200600,  //  004A  CALL	R8	3
      0x50200000,  //  004B  LDBOOL	R8	0	0
      0xA8040001,  //  004C  EXBLK	1	1
      0x80041000,  //  004D  RET	1	R8
      0x8C200B1B,  //  004E  GETMET	R8	R5	K27
      0x7C200200,  //  004F  CALL	R8	1
      0x74220002,  //  0050  JMPT	R8	#0054
      0x50200000,  //  0051  LDBOOL	R8	0	0
      0xA8040001,  //  0052  EXBLK	1	1
      0x80041000,  //  0053  RET	1	R8
      0x88200100,  //  0054  GETMBR	R8	R0	K0
      0x8C20111C,  //  0055  GETMET	R8	R8	K28
      0x5C280A00,  //  0056  MOVE	R10	R5
      0x7C200400,  //  0057  CALL	R8	2
      0x88200B1D,  //  0058  GETMBR	R8	R5	K29
      0x5426000F,  //  0059  LDINT	R9	16
      0x20201009,  //  005A  NE	R8	R8	R9
      0x78220018,  //  005B  JMPF	R8	#0075
      0xB8220800,  //  005C  GETNGBL	R8	K4
      0x8C20111E,  //  005D  GETMET	R8	R8	K30
      0x88280B1D,  //  005E  GETMBR	R10	R5	K29
      0x7C200400,  //  005F  CALL	R8	2
      0x5C241000,  //  0060  MOVE	R9	R8
      0x74260004,  //  0061  JMPT	R9	#0067
      0x60240018,  //  0062  GETGBL	R9	G24
      0x5828001F,  //  0063  LDCONST	R10	K31
      0x882C0B1D,  //  0064  GETMBR	R11	R5	K29
      0x7C240400,  //  0065  CALL	R9	2
      0x5C201200,  //  0066  MOVE	R8	R9
      0xB8262E00,  //  0067  GETNGBL	R9	K23
      0x8C241302,  //  0068  GETMET	R9	R9	K2
      0x602C0018,  //  0069  GETGBL	R11	G24
      0x58300020,  //  006A  LDCONST	R12	K32
      0x88340F0D,  //  006B  GETMBR	R13	R7	K13
      0x5C381000,  //  006C  MOVE	R14	R8
      0x883C0B16,  //  006D  GETMBR	R15	R5	K22
      0x88400B21,  //  006E  GETMBR	R16	R5	K33
      0x5C440400,  //  006F  MOVE	R17	R2
      0x5C480600,  //  0070  MOVE	R18	R3
      0x7C2C0E00,  //  0071  CALL	R11	7
      0x58300022,  //  0072  LDCONST	R12	K34
      0x7C240600,  //  0073  CALL	R9	3
      0x70020013,  //  0074  JMP		#0089
      0xB8222E00,  //  0075  GETNGBL	R8	K23
      0x8C201102,  //  0076  GETMET	R8	R8	K2
      0x60280018,  //  0077  GETGBL	R10	G24
      0x582C0023,  //  0078  LDCONST	R11	K35
      0x88300F0D,  //  0079  GETMBR	R12	R7	K13
      0x88340B16,  //  007A  GETMBR	R13	R5	K22
      0x88380B24,  //  007B  GETMBR	R14	R5	K36
      0x783A0001,  //  007C  JMPF	R14	#007F
      0x58380025,  //  007D  LDCONST	R14	K37
      0x70020000,  //  007E  JMP		#0080
      0x58380026,  //  007F  LDCONST	R14	K38
      0x883C0B21,  //  0080  GETMBR	R15	R5	K33
      0x60400008,  //  0081  GETGBL	R16	G8
      0x88440B27,  //  0082  GETMBR	R17	R5	K39
      0x7C400200,  //  0083  CALL	R16	1
      0x5C440400,  //  0084  MOVE	R17	R2
      0x5C480600,  //  0085  MOVE	R18	R3
      0x7C281000,  //  0086  CALL	R10	8
      0x542E0003,  //  0087  LDINT	R11	4
      0x7C200600,  //  0088  CALL	R8	3
      0x88200128,  //  0089  GETMBR	R8	R0	K40
      0x8C201129,  //  008A  GETMET	R8	R8	K41
      0x5C280A00,  //  008B  MOVE	R10	R5
      0x7C200400,  //  008C  CALL	R8	2
      0x5C101000,  //  008D  MOVE	R4	R8
      0x5C200800,  //  008E  MOVE	R8	R4
      0x74220003,  //  008F  JMPT	R8	#0094
      0x8C20011A,  //  0090  GETMET	R8	R0	K26
      0x5C280A00,  //  0091  MOVE	R10	R5
      0x502C0000,  //  0092  LDBOOL	R11	0	0
      0x7C200600,  //  0093  CALL	R8	3
      0x50200200,  //  0094  LDBOOL	R8	1	0
      0xA8040001,  //  0095  EXBLK	1	1
      0x80041000,  //  0096  RET	1	R8
      0x70020096,  //  0097  JMP		#012F
      0xB81E2E00,  //  0098  GETNGBL	R7	K23
      0x8C1C0F02,  //  0099  GETMET	R7	R7	K2
      0x60240018,  //  009A  GETGBL	R9	G24
      0x5828002A,  //  009B  LDCONST	R10	K42
      0x882C0B0D,  //  009C  GETMBR	R11	R5	K13
      0x88300B16,  //  009D  GETMBR	R12	R5	K22
      0x7C240600,  //  009E  CALL	R9	3
      0x542A0003,  //  009F  LDINT	R10	4
      0x7C1C0600,  //  00A0  CALL	R7	3
      0x881C0100,  //  00A1  GETMBR	R7	R0	K0
      0x881C0F08,  //  00A2  GETMBR	R7	R7	K8
      0x8C1C0F2B,  //  00A3  GETMET	R7	R7	K43
      0x88240B0D,  //  00A4  GETMBR	R9	R5	K13
      0x7C1C0400,  //  00A5  CALL	R7	2
      0x4C200000,  //  00A6  LDNIL	R8
      0x1C200E08,  //  00A7  EQ	R8	R7	R8
      0x7822000A,  //  00A8  JMPF	R8	#00B4
      0xB8222E00,  //  00A9  GETNGBL	R8	K23
      0x8C201102,  //  00AA  GETMET	R8	R8	K2
      0x60280008,  //  00AB  GETGBL	R10	G8
      0x882C0B0D,  //  00AC  GETMBR	R11	R5	K13
      0x7C280200,  //  00AD  CALL	R10	1
      0x002A580A,  //  00AE  ADD	R10	K44	R10
      0x582C0022,  //  00AF  LDCONST	R11	K34
      0x7C200600,  //  00B0  CALL	R8	3
      0x50200000,  //  00B1  LDBOOL	R8	0	0
      0xA8040001,  //  00B2  EXBLK	1	1
      0x80041000,  //  00B3  RET	1	R8
      0x780A0000,  //  00B4  JMPF	R2	#00B6
      0x901E2002,  //  00B5  SETMBR	R7	K16	R2
      0x780E0000,  //  00B6  JMPF	R3	#00B8
      0x901E2203,  //  00B7  SETMBR	R7	K17	R3
      0x901E2400,  //  00B8  SETMBR	R7	K18	R0
      0x90162607,  //  00B9  SETMBR	R5	K19	R7
      0x8C200F2D,  //  00BA  GETMET	R8	R7	K45
      0x88280B16,  //  00BB  GETMBR	R10	R5	K22
      0x502C0200,  //  00BC  LDBOOL	R11	1	0
      0x7C200600,  //  00BD  CALL	R8	3
      0x74220013,  //  00BE  JMPT	R8	#00D3
      0xB8222E00,  //  00BF  GETNGBL	R8	K23
      0x8C201102,  //  00C0  GETMET	R8	R8	K2
      0x60280008,  //  00C1  GETGBL	R10	G8
      0x882C0B16,  //  00C2  GETMBR	R11	R5	K22
      0x7C280200,  //  00C3  CALL	R10	1
      0x002A5C0A,  //  00C4  ADD	R10	K46	R10
      0x0028152F,  //  00C5  ADD	R10	R10	K47
      0x602C0008,  //  00C6  GETGBL	R11	G8
      0x88300F30,  //  00C7  GETMBR	R12	R7	K48
      0x7C2C0200,  //  00C8  CALL	R11	1
      0x0028140B,  //  00C9  ADD	R10	R10	R11
      0x582C0022,  //  00CA  LDCONST	R11	K34
      0x7C200600,  //  00CB  CALL	R8	3
      0x8C200131,  //  00CC  GETMET	R8	R0	K49
      0x5C280A00,  //  00CD  MOVE	R10	R5
      0x502C0000,  //  00CE  LDBOOL	R11	0	0
      0x7C200600,  //  00CF  CALL	R8	3
      0x50200000,  //  00D0  LDBOOL	R8	0	0
      0xA8040001,  //  00D1  EXBLK	1	1
      0x80041000,  //  00D2  RET	1	R8
      0x8C200B32,  //  00D3  GETMET	R8	R5	K50
      0x7C200200,  //  00D4  CALL	R8	1
      0x5C241000,  //  00D5  MOVE	R9	R8
      0x74260002,  //  00D6  JMPT	R9	#00DA
      0x50240000,  //  00D7  LDBOOL	R9	0	0
      0xA8040001,  //  00D8  EXBLK	1	1
      0x80041200,  //  00D9  RET	1	R9
      0x88240B34,  //  00DA  GETMBR	R9	R5	K52
      0x04241335,  //  00DB  SUB	R9	R9	K53
      0x40261C09,  //  00DC  CONNECT	R9	K14	R9
      0x88280B33,  //  00DD  GETMBR	R10	R5	K51
      0x94241409,  //  00DE  GETIDX	R9	R10	R9
      0x90166609,  //  00DF  SETMBR	R5	K51	R9
      0x88240B33,  //  00E0  GETMBR	R9	R5	K51
      0x40241208,  //  00E1  CONNECT	R9	R9	R8
      0x8C240B1B,  //  00E2  GETMET	R9	R5	K27
      0x7C240200,  //  00E3  CALL	R9	1
      0xB8262E00,  //  00E4  GETNGBL	R9	K23
      0x8C241302,  //  00E5  GETMET	R9	R9	K2
      0x602C0008,  //  00E6  GETGBL	R11	G8
      0x88300B37,  //  00E7  GETMBR	R12	R5	K55
      0x7C2C0200,  //  00E8  CALL	R11	1
      0x002E6C0B,  //  00E9  ADD	R11	K54	R11
      0x002C1738,  //  00EA  ADD	R11	R11	K56
      0x60300008,  //  00EB  GETGBL	R12	G8
      0x88340B1D,  //  00EC  GETMBR	R13	R5	K29
      0x7C300200,  //  00ED  CALL	R12	1
      0x002C160C,  //  00EE  ADD	R11	R11	R12
      0x002C1739,  //  00EF  ADD	R11	R11	K57
      0x60300008,  //  00F0  GETGBL	R12	G8
      0x88340B21,  //  00F1  GETMBR	R13	R5	K33
      0x543AFFFE,  //  00F2  LDINT	R14	65535
      0x2C341A0E,  //  00F3  AND	R13	R13	R14
      0x7C300200,  //  00F4  CALL	R12	1
      0x002C160C,  //  00F5  ADD	R11	R11	R12
      0x54320003,  //  00F6  LDINT	R12	4
      0x7C240600,  //  00F7  CALL	R9	3
      0x88240100,  //  00F8  GETMBR	R9	R0	K0
      0x8C24131C,  //  00F9  GETMET	R9	R9	K28
      0x5C2C0A00,  //  00FA  MOVE	R11	R5
      0x7C240400,  //  00FB  CALL	R9	2
      0x88240B37,  //  00FC  GETMBR	R9	R5	K55
      0x1C28130E,  //  00FD  EQ	R10	R9	K14
      0x782A000F,  //  00FE  JMPF	R10	#010F
      0x88280B1D,  //  00FF  GETMBR	R10	R5	K29
      0x542E000F,  //  0100  LDINT	R11	16
      0x1C28140B,  //  0101  EQ	R10	R10	R11
      0x782A0009,  //  0102  JMPF	R10	#010D
      0x8828013A,  //  0103  GETMBR	R10	R0	K58
      0x8C28153B,  //  0104  GETMET	R10	R10	K59
      0x5C300A00,  //  0105  MOVE	R12	R5
      0x7C280400,  //  0106  CALL	R10	2
      0x5C101400,  //  0107  MOVE	R4	R10
      0x78120003,  //  0108  JMPF	R4	#010D
      0x8828013A,  //  0109  GETMBR	R10	R0	K58
      0x8C28153C,  //  010A  GETMET	R10	R10	K60
      0x5C300000,  //  010B  MOVE	R12	R0
      0x7C280400,  //  010C  CALL	R10	2
      0x50100200,  //  010D  LDBOOL	R4	1	0
      0x7002001F,  //  010E  JMP		#012F
      0x1C281335,  //  010F  EQ	R10	R9	K53
      0x782A0015,  //  0110  JMPF	R10	#0127
      0x8828013A,  //  0111  GETMBR	R10	R0	K58
      0x8C281529,  //  0112  GETMET	R10	R10	K41
      0x5C300A00,  //  0113  MOVE	R12	R5
      0x7C280400,  //  0114  CALL	R10	2
      0x5C101400,  //  0115  MOVE	R4	R10
      0x88280100,  //  0116  GETMBR	R10	R0	K0
      0x88281501,  //  0117  GETMBR	R10	R10	K1
      0x8C281502,  //  0118  GETMET	R10	R10	K2
      0x5830003D,  //  0119  LDCONST	R12	K61
      0x7C280400,  //  011A  CALL	R10	2
      0x78120004,  //  011B  JMPF	R4	#0121
      0x8828013A,  //  011C  GETMBR	R10	R0	K58
      0x8C28153C,  //  011D  GETMET	R10	R10	K60
      0x5C300000,  //  011E  MOVE	R12	R0
      0x7C280400,  //  011F  CALL	R10	2
      0x70020003,  //  0120  JMP		#0125
      0x8C280131,  //  0121  GETMET	R10	R0	K49
      0x5C300A00,  //  0122  MOVE	R12	R5
      0x50340200,  //  0123  LDBOOL	R13	1	0
      0x7C280600,  //  0124  CALL	R10	3
      0x50100200,  //  0125  LDBOOL	R4	1	0
      0x70020007,  //  0126  JMP		#012F
      0xB82A2E00,  //  0127  GETNGBL	R10	K23
      0x8C281502,  //  0128  GETMET	R10	R10	K2
      0x60300008,  //  0129  GETGBL	R12	G8
      0x5C341200,  //  012A  MOVE	R13	R9
      0x7C300200,  //  012B  CALL	R12	1
      0x00327C0C,  //  012C  ADD	R12	K62	R12
      0x58340022,  //  012D  LDCONST	R13	K34
      0x7C280600,  //  012E  CALL	R10	3
      0xA8040001,  //  012F  EXBLK	1	1
      0x80040800,  //  0130  RET	1	R4
      0xA8040001,  //  0131  EXBLK	1	1
      0x70020018,  //  0132  JMP		#014C
      0xAC140002,  //  0133  CATCH	R5	0	2
      0x70020015,  //  0134  JMP		#014B
      0xB81E2E00,  //  0135  GETNGBL	R7	K23
      0x8C1C0F02,  //  0136  GETMET	R7	R7	K2
      0x60240008,  //  0137  GETGBL	R9	G8
      0x5C280A00,  //  0138  MOVE	R10	R5
      0x7C240200,  //  0139  CALL	R9	1
      0x00267E09,  //  013A  ADD	R9	K63	R9
      0x00241340,  //  013B  ADD	R9	R9	K64
      0x60280008,  //  013C  GETGBL	R10	G8
      0x5C2C0C00,  //  013D  MOVE	R11	R6
      0x7C280200,  //  013E  CALL	R10	1
      0x0024120A,  //  013F  ADD	R9	R9	R10
      0x58280041,  //  0140  LDCONST	R10	K65
      0x7C1C0600,  //  0141  CALL	R7	3
      0xB81E2E00,  //  0142  GETNGBL	R7	K23
      0x881C0F42,  //  0143  GETMBR	R7	R7	K66
      0x781E0002,  //  0144  JMPF	R7	#0148
      0xA41E8600,  //  0145  IMPORT	R7	K67
      0x8C200F44,  //  0146  GETMET	R8	R7	K68
      0x7C200200,  //  0147  CALL	R8	1
      0x501C0000,  //  0148  LDBOOL	R7	0	0
      0x80040E00,  //  0149  RET	1	R7
      0x70020000,  //  014A  JMP		#014C
      0xB0080000,  //  014B  RAISE	2	R0	R0
      0x80000000,  //  014C  RET	0
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
