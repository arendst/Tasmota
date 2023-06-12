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
    ( &(const bvalue[16]) {     /* constants */
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
    /* K15  */  be_nested_str_weak(send_response_frame),
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
      0x54220003,  //  0017  LDINT	R8	4
      0x7C140600,  //  0018  CALL	R5	3
      0x8C14010F,  //  0019  GETMET	R5	R0	K15
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
    ( &(const bvalue[15]) {     /* constants */
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
    /* K14  */  be_nested_str_weak(send_response_frame),
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
      0x54220003,  //  0015  LDINT	R8	4
      0x7C140600,  //  0016  CALL	R5	3
      0x8C14010E,  //  0017  GETMET	R5	R0	K14
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
    21,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[68]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Frame),
    /* K3   */  be_nested_str_weak(decode_header),
    /* K4   */  be_nested_str_weak(sec_p),
    /* K5   */  be_nested_str_weak(device),
    /* K6   */  be_nested_str_weak(sessions),
    /* K7   */  be_nested_str_weak(find_session_source_id_unsecure),
    /* K8   */  be_nested_str_weak(source_node_id),
    /* K9   */  be_nested_str_weak(control_message),
    /* K10  */  be_nested_str_weak(process_incoming_control_message),
    /* K11  */  be_nested_str_weak(local_session_id),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str_weak(sec_sesstype),
    /* K14  */  be_nested_str_weak(_ip),
    /* K15  */  be_nested_str_weak(_port),
    /* K16  */  be_nested_str_weak(_message_handler),
    /* K17  */  be_nested_str_weak(session),
    /* K18  */  be_nested_str_weak(_counter_insecure_rcv),
    /* K19  */  be_nested_str_weak(validate),
    /* K20  */  be_nested_str_weak(message_counter),
    /* K21  */  be_nested_str_weak(tasmota),
    /* K22  */  be_nested_str_weak(log),
    /* K23  */  be_nested_str_weak(format),
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
    /* K61  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K62  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K63  */  be_nested_str_weak(_X3B),
    /* K64  */  be_const_int(2),
    /* K65  */  be_nested_str_weak(_debug_present),
    /* K66  */  be_nested_str_weak(debug),
    /* K67  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[324]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x50140000,  //  0001  LDBOOL	R5	0	0
      0xA8020126,  //  0002  EXBLK	0	#012A
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
      0x7822000C,  //  0012  JMPF	R8	#0020
      0x88200105,  //  0013  GETMBR	R8	R0	K5
      0x88201106,  //  0014  GETMBR	R8	R8	K6
      0x8C201107,  //  0015  GETMET	R8	R8	K7
      0x88280D08,  //  0016  GETMBR	R10	R6	K8
      0x542E0059,  //  0017  LDINT	R11	90
      0x7C200600,  //  0018  CALL	R8	3
      0x88240109,  //  0019  GETMBR	R9	R0	K9
      0x8C24130A,  //  001A  GETMET	R9	R9	K10
      0x5C2C0C00,  //  001B  MOVE	R11	R6
      0x7C240400,  //  001C  CALL	R9	2
      0xA8040001,  //  001D  EXBLK	1	1
      0x80041200,  //  001E  RET	1	R9
      0x70020105,  //  001F  JMP		#0126
      0x88200D0B,  //  0020  GETMBR	R8	R6	K11
      0x1C20110C,  //  0021  EQ	R8	R8	K12
      0x78220070,  //  0022  JMPF	R8	#0094
      0x88200D0D,  //  0023  GETMBR	R8	R6	K13
      0x1C20110C,  //  0024  EQ	R8	R8	K12
      0x7822006D,  //  0025  JMPF	R8	#0094
      0x88200105,  //  0026  GETMBR	R8	R0	K5
      0x88201106,  //  0027  GETMBR	R8	R8	K6
      0x8C201107,  //  0028  GETMET	R8	R8	K7
      0x88280D08,  //  0029  GETMBR	R10	R6	K8
      0x542E0059,  //  002A  LDINT	R11	90
      0x7C200600,  //  002B  CALL	R8	3
      0x780A0000,  //  002C  JMPF	R2	#002E
      0x90221C02,  //  002D  SETMBR	R8	K14	R2
      0x780E0000,  //  002E  JMPF	R3	#0030
      0x90221E03,  //  002F  SETMBR	R8	K15	R3
      0x90222000,  //  0030  SETMBR	R8	K16	R0
      0x901A2208,  //  0031  SETMBR	R6	K17	R8
      0x88241112,  //  0032  GETMBR	R9	R8	K18
      0x8C241313,  //  0033  GETMET	R9	R9	K19
      0x882C0D14,  //  0034  GETMBR	R11	R6	K20
      0x50300000,  //  0035  LDBOOL	R12	0	0
      0x7C240600,  //  0036  CALL	R9	3
      0x74260011,  //  0037  JMPT	R9	#004A
      0xB8262A00,  //  0038  GETNGBL	R9	K21
      0x8C241316,  //  0039  GETMET	R9	R9	K22
      0x8C2C0917,  //  003A  GETMET	R11	R4	K23
      0x58340018,  //  003B  LDCONST	R13	K24
      0x88380D14,  //  003C  GETMBR	R14	R6	K20
      0x883C1112,  //  003D  GETMBR	R15	R8	K18
      0x8C3C1F19,  //  003E  GETMET	R15	R15	K25
      0x7C3C0200,  //  003F  CALL	R15	1
      0x7C2C0800,  //  0040  CALL	R11	4
      0x54320003,  //  0041  LDINT	R12	4
      0x7C240600,  //  0042  CALL	R9	3
      0x8C24011A,  //  0043  GETMET	R9	R0	K26
      0x5C2C0C00,  //  0044  MOVE	R11	R6
      0x50300000,  //  0045  LDBOOL	R12	0	0
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
      0x88240105,  //  0050  GETMBR	R9	R0	K5
      0x8C24131C,  //  0051  GETMET	R9	R9	K28
      0x5C2C0C00,  //  0052  MOVE	R11	R6
      0x7C240400,  //  0053  CALL	R9	2
      0x88240D1D,  //  0054  GETMBR	R9	R6	K29
      0x542A000F,  //  0055  LDINT	R10	16
      0x2024120A,  //  0056  NE	R9	R9	R10
      0x78260018,  //  0057  JMPF	R9	#0071
      0xB8260200,  //  0058  GETNGBL	R9	K1
      0x8C24131E,  //  0059  GETMET	R9	R9	K30
      0x882C0D1D,  //  005A  GETMBR	R11	R6	K29
      0x7C240400,  //  005B  CALL	R9	2
      0x5C281200,  //  005C  MOVE	R10	R9
      0x742A0004,  //  005D  JMPT	R10	#0063
      0x8C280917,  //  005E  GETMET	R10	R4	K23
      0x5830001F,  //  005F  LDCONST	R12	K31
      0x88340D1D,  //  0060  GETMBR	R13	R6	K29
      0x7C280600,  //  0061  CALL	R10	3
      0x5C241400,  //  0062  MOVE	R9	R10
      0xB82A2A00,  //  0063  GETNGBL	R10	K21
      0x8C281516,  //  0064  GETMET	R10	R10	K22
      0x8C300917,  //  0065  GETMET	R12	R4	K23
      0x58380020,  //  0066  LDCONST	R14	K32
      0x883C110B,  //  0067  GETMBR	R15	R8	K11
      0x5C401200,  //  0068  MOVE	R16	R9
      0x88440D14,  //  0069  GETMBR	R17	R6	K20
      0x88480D21,  //  006A  GETMBR	R18	R6	K33
      0x5C4C0400,  //  006B  MOVE	R19	R2
      0x5C500600,  //  006C  MOVE	R20	R3
      0x7C301000,  //  006D  CALL	R12	8
      0x58340022,  //  006E  LDCONST	R13	K34
      0x7C280600,  //  006F  CALL	R10	3
      0x70020013,  //  0070  JMP		#0085
      0xB8262A00,  //  0071  GETNGBL	R9	K21
      0x8C241316,  //  0072  GETMET	R9	R9	K22
      0x8C2C0917,  //  0073  GETMET	R11	R4	K23
      0x58340023,  //  0074  LDCONST	R13	K35
      0x8838110B,  //  0075  GETMBR	R14	R8	K11
      0x883C0D14,  //  0076  GETMBR	R15	R6	K20
      0x88400D24,  //  0077  GETMBR	R16	R6	K36
      0x78420001,  //  0078  JMPF	R16	#007B
      0x58400025,  //  0079  LDCONST	R16	K37
      0x70020000,  //  007A  JMP		#007C
      0x58400026,  //  007B  LDCONST	R16	K38
      0x88440D21,  //  007C  GETMBR	R17	R6	K33
      0x60480008,  //  007D  GETGBL	R18	G8
      0x884C0D27,  //  007E  GETMBR	R19	R6	K39
      0x7C480200,  //  007F  CALL	R18	1
      0x5C4C0400,  //  0080  MOVE	R19	R2
      0x5C500600,  //  0081  MOVE	R20	R3
      0x7C2C1200,  //  0082  CALL	R11	9
      0x54320003,  //  0083  LDINT	R12	4
      0x7C240600,  //  0084  CALL	R9	3
      0x88240128,  //  0085  GETMBR	R9	R0	K40
      0x8C241329,  //  0086  GETMET	R9	R9	K41
      0x5C2C0C00,  //  0087  MOVE	R11	R6
      0x7C240400,  //  0088  CALL	R9	2
      0x5C141200,  //  0089  MOVE	R5	R9
      0x5C240A00,  //  008A  MOVE	R9	R5
      0x74260003,  //  008B  JMPT	R9	#0090
      0x8C24011A,  //  008C  GETMET	R9	R0	K26
      0x5C2C0C00,  //  008D  MOVE	R11	R6
      0x50300000,  //  008E  LDBOOL	R12	0	0
      0x7C240600,  //  008F  CALL	R9	3
      0x50240200,  //  0090  LDBOOL	R9	1	0
      0xA8040001,  //  0091  EXBLK	1	1
      0x80041200,  //  0092  RET	1	R9
      0x70020091,  //  0093  JMP		#0126
      0xB8222A00,  //  0094  GETNGBL	R8	K21
      0x8C201116,  //  0095  GETMET	R8	R8	K22
      0x8C280917,  //  0096  GETMET	R10	R4	K23
      0x5830002A,  //  0097  LDCONST	R12	K42
      0x88340D0B,  //  0098  GETMBR	R13	R6	K11
      0x88380D14,  //  0099  GETMBR	R14	R6	K20
      0x7C280800,  //  009A  CALL	R10	4
      0x542E0003,  //  009B  LDINT	R11	4
      0x7C200600,  //  009C  CALL	R8	3
      0x88200105,  //  009D  GETMBR	R8	R0	K5
      0x88201106,  //  009E  GETMBR	R8	R8	K6
      0x8C20112B,  //  009F  GETMET	R8	R8	K43
      0x88280D0B,  //  00A0  GETMBR	R10	R6	K11
      0x7C200400,  //  00A1  CALL	R8	2
      0x4C240000,  //  00A2  LDNIL	R9
      0x1C241009,  //  00A3  EQ	R9	R8	R9
      0x7826000A,  //  00A4  JMPF	R9	#00B0
      0xB8262A00,  //  00A5  GETNGBL	R9	K21
      0x8C241316,  //  00A6  GETMET	R9	R9	K22
      0x602C0008,  //  00A7  GETGBL	R11	G8
      0x88300D0B,  //  00A8  GETMBR	R12	R6	K11
      0x7C2C0200,  //  00A9  CALL	R11	1
      0x002E580B,  //  00AA  ADD	R11	K44	R11
      0x58300022,  //  00AB  LDCONST	R12	K34
      0x7C240600,  //  00AC  CALL	R9	3
      0x50240000,  //  00AD  LDBOOL	R9	0	0
      0xA8040001,  //  00AE  EXBLK	1	1
      0x80041200,  //  00AF  RET	1	R9
      0x780A0000,  //  00B0  JMPF	R2	#00B2
      0x90221C02,  //  00B1  SETMBR	R8	K14	R2
      0x780E0000,  //  00B2  JMPF	R3	#00B4
      0x90221E03,  //  00B3  SETMBR	R8	K15	R3
      0x90222000,  //  00B4  SETMBR	R8	K16	R0
      0x901A2208,  //  00B5  SETMBR	R6	K17	R8
      0x8C24112D,  //  00B6  GETMET	R9	R8	K45
      0x882C0D14,  //  00B7  GETMBR	R11	R6	K20
      0x50300200,  //  00B8  LDBOOL	R12	1	0
      0x7C240600,  //  00B9  CALL	R9	3
      0x74260013,  //  00BA  JMPT	R9	#00CF
      0xB8262A00,  //  00BB  GETNGBL	R9	K21
      0x8C241316,  //  00BC  GETMET	R9	R9	K22
      0x602C0008,  //  00BD  GETGBL	R11	G8
      0x88300D14,  //  00BE  GETMBR	R12	R6	K20
      0x7C2C0200,  //  00BF  CALL	R11	1
      0x002E5C0B,  //  00C0  ADD	R11	K46	R11
      0x002C172F,  //  00C1  ADD	R11	R11	K47
      0x60300008,  //  00C2  GETGBL	R12	G8
      0x88341130,  //  00C3  GETMBR	R13	R8	K48
      0x7C300200,  //  00C4  CALL	R12	1
      0x002C160C,  //  00C5  ADD	R11	R11	R12
      0x54320003,  //  00C6  LDINT	R12	4
      0x7C240600,  //  00C7  CALL	R9	3
      0x8C240131,  //  00C8  GETMET	R9	R0	K49
      0x5C2C0C00,  //  00C9  MOVE	R11	R6
      0x50300000,  //  00CA  LDBOOL	R12	0	0
      0x7C240600,  //  00CB  CALL	R9	3
      0x50240000,  //  00CC  LDBOOL	R9	0	0
      0xA8040001,  //  00CD  EXBLK	1	1
      0x80041200,  //  00CE  RET	1	R9
      0x8C240D32,  //  00CF  GETMET	R9	R6	K50
      0x7C240200,  //  00D0  CALL	R9	1
      0x5C281200,  //  00D1  MOVE	R10	R9
      0x742A0002,  //  00D2  JMPT	R10	#00D6
      0x50280000,  //  00D3  LDBOOL	R10	0	0
      0xA8040001,  //  00D4  EXBLK	1	1
      0x80041400,  //  00D5  RET	1	R10
      0x88280D34,  //  00D6  GETMBR	R10	R6	K52
      0x04281535,  //  00D7  SUB	R10	R10	K53
      0x402A180A,  //  00D8  CONNECT	R10	K12	R10
      0x882C0D33,  //  00D9  GETMBR	R11	R6	K51
      0x9428160A,  //  00DA  GETIDX	R10	R11	R10
      0x901A660A,  //  00DB  SETMBR	R6	K51	R10
      0x88280D33,  //  00DC  GETMBR	R10	R6	K51
      0x40281409,  //  00DD  CONNECT	R10	R10	R9
      0x8C280D1B,  //  00DE  GETMET	R10	R6	K27
      0x7C280200,  //  00DF  CALL	R10	1
      0xB82A2A00,  //  00E0  GETNGBL	R10	K21
      0x8C281516,  //  00E1  GETMET	R10	R10	K22
      0x60300008,  //  00E2  GETGBL	R12	G8
      0x88340D37,  //  00E3  GETMBR	R13	R6	K55
      0x7C300200,  //  00E4  CALL	R12	1
      0x00326C0C,  //  00E5  ADD	R12	K54	R12
      0x00301938,  //  00E6  ADD	R12	R12	K56
      0x60340008,  //  00E7  GETGBL	R13	G8
      0x88380D1D,  //  00E8  GETMBR	R14	R6	K29
      0x7C340200,  //  00E9  CALL	R13	1
      0x0030180D,  //  00EA  ADD	R12	R12	R13
      0x00301939,  //  00EB  ADD	R12	R12	K57
      0x60340008,  //  00EC  GETGBL	R13	G8
      0x88380D21,  //  00ED  GETMBR	R14	R6	K33
      0x543EFFFE,  //  00EE  LDINT	R15	65535
      0x2C381C0F,  //  00EF  AND	R14	R14	R15
      0x7C340200,  //  00F0  CALL	R13	1
      0x0030180D,  //  00F1  ADD	R12	R12	R13
      0x54360003,  //  00F2  LDINT	R13	4
      0x7C280600,  //  00F3  CALL	R10	3
      0x88280105,  //  00F4  GETMBR	R10	R0	K5
      0x8C28151C,  //  00F5  GETMET	R10	R10	K28
      0x5C300C00,  //  00F6  MOVE	R12	R6
      0x7C280400,  //  00F7  CALL	R10	2
      0x88280D37,  //  00F8  GETMBR	R10	R6	K55
      0x1C2C150C,  //  00F9  EQ	R11	R10	K12
      0x782E000F,  //  00FA  JMPF	R11	#010B
      0x882C0D1D,  //  00FB  GETMBR	R11	R6	K29
      0x5432000F,  //  00FC  LDINT	R12	16
      0x1C2C160C,  //  00FD  EQ	R11	R11	R12
      0x782E0009,  //  00FE  JMPF	R11	#0109
      0x882C013A,  //  00FF  GETMBR	R11	R0	K58
      0x8C2C173B,  //  0100  GETMET	R11	R11	K59
      0x5C340C00,  //  0101  MOVE	R13	R6
      0x7C2C0400,  //  0102  CALL	R11	2
      0x5C141600,  //  0103  MOVE	R5	R11
      0x78160003,  //  0104  JMPF	R5	#0109
      0x882C013A,  //  0105  GETMBR	R11	R0	K58
      0x8C2C173C,  //  0106  GETMET	R11	R11	K60
      0x5C340000,  //  0107  MOVE	R13	R0
      0x7C2C0400,  //  0108  CALL	R11	2
      0x50140200,  //  0109  LDBOOL	R5	1	0
      0x7002001A,  //  010A  JMP		#0126
      0x1C2C1535,  //  010B  EQ	R11	R10	K53
      0x782E0010,  //  010C  JMPF	R11	#011E
      0x882C013A,  //  010D  GETMBR	R11	R0	K58
      0x8C2C1729,  //  010E  GETMET	R11	R11	K41
      0x5C340C00,  //  010F  MOVE	R13	R6
      0x7C2C0400,  //  0110  CALL	R11	2
      0x5C141600,  //  0111  MOVE	R5	R11
      0x78160004,  //  0112  JMPF	R5	#0118
      0x882C013A,  //  0113  GETMBR	R11	R0	K58
      0x8C2C173C,  //  0114  GETMET	R11	R11	K60
      0x5C340000,  //  0115  MOVE	R13	R0
      0x7C2C0400,  //  0116  CALL	R11	2
      0x70020003,  //  0117  JMP		#011C
      0x8C2C0131,  //  0118  GETMET	R11	R0	K49
      0x5C340C00,  //  0119  MOVE	R13	R6
      0x50380200,  //  011A  LDBOOL	R14	1	0
      0x7C2C0600,  //  011B  CALL	R11	3
      0x50140200,  //  011C  LDBOOL	R5	1	0
      0x70020007,  //  011D  JMP		#0126
      0xB82E2A00,  //  011E  GETNGBL	R11	K21
      0x8C2C1716,  //  011F  GETMET	R11	R11	K22
      0x60340008,  //  0120  GETGBL	R13	G8
      0x5C381400,  //  0121  MOVE	R14	R10
      0x7C340200,  //  0122  CALL	R13	1
      0x00367A0D,  //  0123  ADD	R13	K61	R13
      0x58380022,  //  0124  LDCONST	R14	K34
      0x7C2C0600,  //  0125  CALL	R11	3
      0xA8040001,  //  0126  EXBLK	1	1
      0x80040A00,  //  0127  RET	1	R5
      0xA8040001,  //  0128  EXBLK	1	1
      0x70020018,  //  0129  JMP		#0143
      0xAC180002,  //  012A  CATCH	R6	0	2
      0x70020015,  //  012B  JMP		#0142
      0xB8222A00,  //  012C  GETNGBL	R8	K21
      0x8C201116,  //  012D  GETMET	R8	R8	K22
      0x60280008,  //  012E  GETGBL	R10	G8
      0x5C2C0C00,  //  012F  MOVE	R11	R6
      0x7C280200,  //  0130  CALL	R10	1
      0x002A7C0A,  //  0131  ADD	R10	K62	R10
      0x0028153F,  //  0132  ADD	R10	R10	K63
      0x602C0008,  //  0133  GETGBL	R11	G8
      0x5C300E00,  //  0134  MOVE	R12	R7
      0x7C2C0200,  //  0135  CALL	R11	1
      0x0028140B,  //  0136  ADD	R10	R10	R11
      0x582C0040,  //  0137  LDCONST	R11	K64
      0x7C200600,  //  0138  CALL	R8	3
      0xB8222A00,  //  0139  GETNGBL	R8	K21
      0x88201141,  //  013A  GETMBR	R8	R8	K65
      0x78220002,  //  013B  JMPF	R8	#013F
      0xA4228400,  //  013C  IMPORT	R8	K66
      0x8C241143,  //  013D  GETMET	R9	R8	K67
      0x7C240200,  //  013E  CALL	R9	1
      0x50200000,  //  013F  LDBOOL	R8	0	0
      0x80041000,  //  0140  RET	1	R8
      0x70020000,  //  0141  JMP		#0143
      0xB0080000,  //  0142  RAISE	2	R0	R0
      0x80000000,  //  0143  RET	0
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
