/* Solidification of Matter_MessageHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_MessageHandler;

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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(commissioning),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Commisioning_Context),
    /* K4   */  be_nested_str_weak(im),
    /* K5   */  be_nested_str_weak(IM),
    /* K6   */  be_nested_str_weak(control_message),
    /* K7   */  be_nested_str_weak(Control_Message),
    /* K8   */  be_nested_str_weak(_n_bytes),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
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
      0x60080015,  //  0010  GETGBL	R2	G21
      0x540E000F,  //  0011  LDINT	R3	16
      0x7C080200,  //  0012  CALL	R2	1
      0x90021002,  //  0013  SETMBR	R0	K8	R2
      0x80000000,  //  0014  RET	0
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(x_flag_r),
    /* K1   */  be_nested_str_weak(build_standalone_ack),
    /* K2   */  be_nested_str_weak(encode_frame),
    /* K3   */  be_nested_str_weak(encrypt),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(loglevel),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X2A_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X25s),
    /* K8   */  be_nested_str_weak(session),
    /* K9   */  be_nested_str_weak(local_session_id),
    /* K10  */  be_nested_str_weak(ack_message_counter),
    /* K11  */  be_nested_str_weak(message_counter),
    /* K12  */  be_nested_str_weak(_X7Breliable_X7D),
    /* K13  */  be_nested_str_weak(),
    /* K14  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(send_encrypted_ack),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x880C0300,  //  0000  GETMBR	R3	R1	K0
      0x780E001D,  //  0001  JMPF	R3	#0020
      0x8C0C0301,  //  0002  GETMET	R3	R1	K1
      0x5C140400,  //  0003  MOVE	R5	R2
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C100702,  //  0005  GETMET	R4	R3	K2
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100703,  //  0007  GETMET	R4	R3	K3
      0x7C100200,  //  0008  CALL	R4	1
      0xB8120800,  //  0009  GETNGBL	R4	K4
      0x8C100905,  //  000A  GETMET	R4	R4	K5
      0x541A0003,  //  000B  LDINT	R6	4
      0x7C100400,  //  000C  CALL	R4	2
      0x7812000E,  //  000D  JMPF	R4	#001D
      0xB8120800,  //  000E  GETNGBL	R4	K4
      0x8C100906,  //  000F  GETMET	R4	R4	K6
      0x60180018,  //  0010  GETGBL	R6	G24
      0x581C0007,  //  0011  LDCONST	R7	K7
      0x88200708,  //  0012  GETMBR	R8	R3	K8
      0x88201109,  //  0013  GETMBR	R8	R8	K9
      0x8824070A,  //  0014  GETMBR	R9	R3	K10
      0x8828070B,  //  0015  GETMBR	R10	R3	K11
      0x780A0001,  //  0016  JMPF	R2	#0019
      0x582C000C,  //  0017  LDCONST	R11	K12
      0x70020000,  //  0018  JMP		#001A
      0x582C000D,  //  0019  LDCONST	R11	K13
      0x7C180A00,  //  001A  CALL	R6	5
      0x541E0003,  //  001B  LDINT	R7	4
      0x7C100600,  //  001C  CALL	R4	3
      0x8C10010E,  //  001D  GETMET	R4	R0	K14
      0x5C180600,  //  001E  MOVE	R6	R3
      0x7C100400,  //  001F  CALL	R4	2
      0x80000000,  //  0020  RET	0
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
    ( &(const bvalue[70]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(msg_received),
    /* K4   */  be_nested_str_weak(Frame),
    /* K5   */  be_nested_str_weak(decode_header),
    /* K6   */  be_nested_str_weak(sec_p),
    /* K7   */  be_nested_str_weak(device),
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
    /* K24  */  be_nested_str_weak(loglevel),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K26  */  be_nested_str_weak(val),
    /* K27  */  be_nested_str_weak(send_simple_ack),
    /* K28  */  be_nested_str_weak(decode_payload),
    /* K29  */  be_nested_str_weak(received_ack),
    /* K30  */  be_nested_str_weak(opcode),
    /* K31  */  be_nested_str_weak(get_opcode_name),
    /* K32  */  be_nested_str_weak(0x_X2502X),
    /* K33  */  be_const_int(3),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20rid_X3D_X25i_X20exch_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K35  */  be_nested_str_weak(exchange_id),
    /* K36  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K37  */  be_nested_str_weak(x_flag_r),
    /* K38  */  be_nested_str_weak(_X7Breliable_X7D_X20),
    /* K39  */  be_nested_str_weak(),
    /* K40  */  be_nested_str_weak(ack_message_counter),
    /* K41  */  be_nested_str_weak(commissioning),
    /* K42  */  be_nested_str_weak(process_incoming),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K44  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
    /* K46  */  be_nested_str_weak(counter_rcv_validate),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K48  */  be_nested_str_weak(_X20counter_X3D),
    /* K49  */  be_nested_str_weak(counter_rcv),
    /* K50  */  be_nested_str_weak(send_encrypted_ack),
    /* K51  */  be_nested_str_weak(decrypt),
    /* K52  */  be_nested_str_weak(msg_received_header_frame_decrypted),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20_X3E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K54  */  be_nested_str_weak(protocol_id),
    /* K55  */  be_nested_str_weak(_X20opcode_X3D),
    /* K56  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K57  */  be_nested_str_weak(im),
    /* K58  */  be_nested_str_weak(process_incoming_ack),
    /* K59  */  be_nested_str_weak(send_enqueued),
    /* K60  */  be_const_int(1),
    /* K61  */  be_nested_str_weak(process_IM_start),
    /* K62  */  be_nested_str_weak(process_IM_end),
    /* K63  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K64  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K65  */  be_nested_str_weak(_X3B),
    /* K66  */  be_const_int(2),
    /* K67  */  be_nested_str_weak(_debug_present),
    /* K68  */  be_nested_str_weak(debug),
    /* K69  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[365]) {  /* code */
      0x50100000,  //  0000  LDBOOL	R4	0	0
      0xB8160000,  //  0001  GETNGBL	R5	K0
      0x88140B01,  //  0002  GETMBR	R5	R5	K1
      0x8C140B02,  //  0003  GETMET	R5	R5	K2
      0x581C0003,  //  0004  LDCONST	R7	K3
      0x7C140400,  //  0005  CALL	R5	2
      0xA802014B,  //  0006  EXBLK	0	#0153
      0xB8160000,  //  0007  GETNGBL	R5	K0
      0x8C140B04,  //  0008  GETMET	R5	R5	K4
      0x5C1C0000,  //  0009  MOVE	R7	R0
      0x5C200200,  //  000A  MOVE	R8	R1
      0x5C240400,  //  000B  MOVE	R9	R2
      0x5C280600,  //  000C  MOVE	R10	R3
      0x7C140A00,  //  000D  CALL	R5	5
      0x8C180B05,  //  000E  GETMET	R6	R5	K5
      0x7C180200,  //  000F  CALL	R6	1
      0x5C1C0C00,  //  0010  MOVE	R7	R6
      0x741E0002,  //  0011  JMPT	R7	#0015
      0x501C0000,  //  0012  LDBOOL	R7	0	0
      0xA8040001,  //  0013  EXBLK	1	1
      0x80040E00,  //  0014  RET	1	R7
      0x881C0B06,  //  0015  GETMBR	R7	R5	K6
      0x781E000C,  //  0016  JMPF	R7	#0024
      0x881C0107,  //  0017  GETMBR	R7	R0	K7
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
      0x7002012A,  //  0023  JMP		#014F
      0x881C0B0D,  //  0024  GETMBR	R7	R5	K13
      0x1C1C0F0E,  //  0025  EQ	R7	R7	K14
      0x781E007F,  //  0026  JMPF	R7	#00A7
      0x881C0B0F,  //  0027  GETMBR	R7	R5	K15
      0x1C1C0F0E,  //  0028  EQ	R7	R7	K14
      0x781E007C,  //  0029  JMPF	R7	#00A7
      0x881C0107,  //  002A  GETMBR	R7	R0	K7
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
      0x74220016,  //  003B  JMPT	R8	#0053
      0xB8222E00,  //  003C  GETNGBL	R8	K23
      0x8C201118,  //  003D  GETMET	R8	R8	K24
      0x542A0003,  //  003E  LDINT	R10	4
      0x7C200400,  //  003F  CALL	R8	2
      0x7822000A,  //  0040  JMPF	R8	#004C
      0xB8222E00,  //  0041  GETNGBL	R8	K23
      0x8C201102,  //  0042  GETMET	R8	R8	K2
      0x60280018,  //  0043  GETGBL	R10	G24
      0x582C0019,  //  0044  LDCONST	R11	K25
      0x88300B16,  //  0045  GETMBR	R12	R5	K22
      0x88340F14,  //  0046  GETMBR	R13	R7	K20
      0x8C341B1A,  //  0047  GETMET	R13	R13	K26
      0x7C340200,  //  0048  CALL	R13	1
      0x7C280600,  //  0049  CALL	R10	3
      0x542E0003,  //  004A  LDINT	R11	4
      0x7C200600,  //  004B  CALL	R8	3
      0x8C20011B,  //  004C  GETMET	R8	R0	K27
      0x5C280A00,  //  004D  MOVE	R10	R5
      0x502C0000,  //  004E  LDBOOL	R11	0	0
      0x7C200600,  //  004F  CALL	R8	3
      0x50200000,  //  0050  LDBOOL	R8	0	0
      0xA8040001,  //  0051  EXBLK	1	1
      0x80041000,  //  0052  RET	1	R8
      0x8C200B1C,  //  0053  GETMET	R8	R5	K28
      0x7C200200,  //  0054  CALL	R8	1
      0x74220002,  //  0055  JMPT	R8	#0059
      0x50200000,  //  0056  LDBOOL	R8	0	0
      0xA8040001,  //  0057  EXBLK	1	1
      0x80041000,  //  0058  RET	1	R8
      0x88200107,  //  0059  GETMBR	R8	R0	K7
      0x8C20111D,  //  005A  GETMET	R8	R8	K29
      0x5C280A00,  //  005B  MOVE	R10	R5
      0x7C200400,  //  005C  CALL	R8	2
      0x88200B1E,  //  005D  GETMBR	R8	R5	K30
      0x5426000F,  //  005E  LDINT	R9	16
      0x20201009,  //  005F  NE	R8	R8	R9
      0x7822001D,  //  0060  JMPF	R8	#007F
      0xB8220000,  //  0061  GETNGBL	R8	K0
      0x8C20111F,  //  0062  GETMET	R8	R8	K31
      0x88280B1E,  //  0063  GETMBR	R10	R5	K30
      0x7C200400,  //  0064  CALL	R8	2
      0x5C241000,  //  0065  MOVE	R9	R8
      0x74260004,  //  0066  JMPT	R9	#006C
      0x60240018,  //  0067  GETGBL	R9	G24
      0x58280020,  //  0068  LDCONST	R10	K32
      0x882C0B1E,  //  0069  GETMBR	R11	R5	K30
      0x7C240400,  //  006A  CALL	R9	2
      0x5C201200,  //  006B  MOVE	R8	R9
      0xB8262E00,  //  006C  GETNGBL	R9	K23
      0x8C241318,  //  006D  GETMET	R9	R9	K24
      0x582C0021,  //  006E  LDCONST	R11	K33
      0x7C240400,  //  006F  CALL	R9	2
      0x7826000C,  //  0070  JMPF	R9	#007E
      0xB8262E00,  //  0071  GETNGBL	R9	K23
      0x8C241302,  //  0072  GETMET	R9	R9	K2
      0x602C0018,  //  0073  GETGBL	R11	G24
      0x58300022,  //  0074  LDCONST	R12	K34
      0x88340F0D,  //  0075  GETMBR	R13	R7	K13
      0x5C381000,  //  0076  MOVE	R14	R8
      0x883C0B16,  //  0077  GETMBR	R15	R5	K22
      0x88400B23,  //  0078  GETMBR	R16	R5	K35
      0x5C440400,  //  0079  MOVE	R17	R2
      0x5C480600,  //  007A  MOVE	R18	R3
      0x7C2C0E00,  //  007B  CALL	R11	7
      0x58300021,  //  007C  LDCONST	R12	K33
      0x7C240600,  //  007D  CALL	R9	3
      0x70020018,  //  007E  JMP		#0098
      0xB8222E00,  //  007F  GETNGBL	R8	K23
      0x8C201118,  //  0080  GETMET	R8	R8	K24
      0x542A0003,  //  0081  LDINT	R10	4
      0x7C200400,  //  0082  CALL	R8	2
      0x78220013,  //  0083  JMPF	R8	#0098
      0xB8222E00,  //  0084  GETNGBL	R8	K23
      0x8C201102,  //  0085  GETMET	R8	R8	K2
      0x60280018,  //  0086  GETGBL	R10	G24
      0x582C0024,  //  0087  LDCONST	R11	K36
      0x88300F0D,  //  0088  GETMBR	R12	R7	K13
      0x88340B16,  //  0089  GETMBR	R13	R5	K22
      0x88380B25,  //  008A  GETMBR	R14	R5	K37
      0x783A0001,  //  008B  JMPF	R14	#008E
      0x58380026,  //  008C  LDCONST	R14	K38
      0x70020000,  //  008D  JMP		#008F
      0x58380027,  //  008E  LDCONST	R14	K39
      0x883C0B23,  //  008F  GETMBR	R15	R5	K35
      0x60400008,  //  0090  GETGBL	R16	G8
      0x88440B28,  //  0091  GETMBR	R17	R5	K40
      0x7C400200,  //  0092  CALL	R16	1
      0x5C440400,  //  0093  MOVE	R17	R2
      0x5C480600,  //  0094  MOVE	R18	R3
      0x7C281000,  //  0095  CALL	R10	8
      0x542E0003,  //  0096  LDINT	R11	4
      0x7C200600,  //  0097  CALL	R8	3
      0x88200129,  //  0098  GETMBR	R8	R0	K41
      0x8C20112A,  //  0099  GETMET	R8	R8	K42
      0x5C280A00,  //  009A  MOVE	R10	R5
      0x7C200400,  //  009B  CALL	R8	2
      0x5C101000,  //  009C  MOVE	R4	R8
      0x5C200800,  //  009D  MOVE	R8	R4
      0x74220003,  //  009E  JMPT	R8	#00A3
      0x8C20011B,  //  009F  GETMET	R8	R0	K27
      0x5C280A00,  //  00A0  MOVE	R10	R5
      0x502C0000,  //  00A1  LDBOOL	R11	0	0
      0x7C200600,  //  00A2  CALL	R8	3
      0x50200200,  //  00A3  LDBOOL	R8	1	0
      0xA8040001,  //  00A4  EXBLK	1	1
      0x80041000,  //  00A5  RET	1	R8
      0x700200A7,  //  00A6  JMP		#014F
      0xB81E2E00,  //  00A7  GETNGBL	R7	K23
      0x8C1C0F18,  //  00A8  GETMET	R7	R7	K24
      0x54260003,  //  00A9  LDINT	R9	4
      0x7C1C0400,  //  00AA  CALL	R7	2
      0x781E0008,  //  00AB  JMPF	R7	#00B5
      0xB81E2E00,  //  00AC  GETNGBL	R7	K23
      0x8C1C0F02,  //  00AD  GETMET	R7	R7	K2
      0x60240018,  //  00AE  GETGBL	R9	G24
      0x5828002B,  //  00AF  LDCONST	R10	K43
      0x882C0B0D,  //  00B0  GETMBR	R11	R5	K13
      0x88300B16,  //  00B1  GETMBR	R12	R5	K22
      0x7C240600,  //  00B2  CALL	R9	3
      0x542A0003,  //  00B3  LDINT	R10	4
      0x7C1C0600,  //  00B4  CALL	R7	3
      0x881C0107,  //  00B5  GETMBR	R7	R0	K7
      0x881C0F08,  //  00B6  GETMBR	R7	R7	K8
      0x8C1C0F2C,  //  00B7  GETMET	R7	R7	K44
      0x88240B0D,  //  00B8  GETMBR	R9	R5	K13
      0x7C1C0400,  //  00B9  CALL	R7	2
      0x4C200000,  //  00BA  LDNIL	R8
      0x1C200E08,  //  00BB  EQ	R8	R7	R8
      0x7822000A,  //  00BC  JMPF	R8	#00C8
      0xB8222E00,  //  00BD  GETNGBL	R8	K23
      0x8C201102,  //  00BE  GETMET	R8	R8	K2
      0x60280008,  //  00BF  GETGBL	R10	G8
      0x882C0B0D,  //  00C0  GETMBR	R11	R5	K13
      0x7C280200,  //  00C1  CALL	R10	1
      0x002A5A0A,  //  00C2  ADD	R10	K45	R10
      0x582C0021,  //  00C3  LDCONST	R11	K33
      0x7C200600,  //  00C4  CALL	R8	3
      0x50200000,  //  00C5  LDBOOL	R8	0	0
      0xA8040001,  //  00C6  EXBLK	1	1
      0x80041000,  //  00C7  RET	1	R8
      0x780A0000,  //  00C8  JMPF	R2	#00CA
      0x901E2002,  //  00C9  SETMBR	R7	K16	R2
      0x780E0000,  //  00CA  JMPF	R3	#00CC
      0x901E2203,  //  00CB  SETMBR	R7	K17	R3
      0x901E2400,  //  00CC  SETMBR	R7	K18	R0
      0x90162607,  //  00CD  SETMBR	R5	K19	R7
      0x8C200F2E,  //  00CE  GETMET	R8	R7	K46
      0x88280B16,  //  00CF  GETMBR	R10	R5	K22
      0x502C0200,  //  00D0  LDBOOL	R11	1	0
      0x7C200600,  //  00D1  CALL	R8	3
      0x74220018,  //  00D2  JMPT	R8	#00EC
      0xB8222E00,  //  00D3  GETNGBL	R8	K23
      0x8C201118,  //  00D4  GETMET	R8	R8	K24
      0x58280021,  //  00D5  LDCONST	R10	K33
      0x7C200400,  //  00D6  CALL	R8	2
      0x7822000C,  //  00D7  JMPF	R8	#00E5
      0xB8222E00,  //  00D8  GETNGBL	R8	K23
      0x8C201102,  //  00D9  GETMET	R8	R8	K2
      0x60280008,  //  00DA  GETGBL	R10	G8
      0x882C0B16,  //  00DB  GETMBR	R11	R5	K22
      0x7C280200,  //  00DC  CALL	R10	1
      0x002A5E0A,  //  00DD  ADD	R10	K47	R10
      0x00281530,  //  00DE  ADD	R10	R10	K48
      0x602C0008,  //  00DF  GETGBL	R11	G8
      0x88300F31,  //  00E0  GETMBR	R12	R7	K49
      0x7C2C0200,  //  00E1  CALL	R11	1
      0x0028140B,  //  00E2  ADD	R10	R10	R11
      0x582C0021,  //  00E3  LDCONST	R11	K33
      0x7C200600,  //  00E4  CALL	R8	3
      0x8C200132,  //  00E5  GETMET	R8	R0	K50
      0x5C280A00,  //  00E6  MOVE	R10	R5
      0x502C0000,  //  00E7  LDBOOL	R11	0	0
      0x7C200600,  //  00E8  CALL	R8	3
      0x50200000,  //  00E9  LDBOOL	R8	0	0
      0xA8040001,  //  00EA  EXBLK	1	1
      0x80041000,  //  00EB  RET	1	R8
      0x8C200B33,  //  00EC  GETMET	R8	R5	K51
      0x7C200200,  //  00ED  CALL	R8	1
      0xB8260000,  //  00EE  GETNGBL	R9	K0
      0x88241301,  //  00EF  GETMBR	R9	R9	K1
      0x8C241302,  //  00F0  GETMET	R9	R9	K2
      0x582C0034,  //  00F1  LDCONST	R11	K52
      0x7C240400,  //  00F2  CALL	R9	2
      0x5C241000,  //  00F3  MOVE	R9	R8
      0x74260002,  //  00F4  JMPT	R9	#00F8
      0x50240000,  //  00F5  LDBOOL	R9	0	0
      0xA8040001,  //  00F6  EXBLK	1	1
      0x80041200,  //  00F7  RET	1	R9
      0x8C240B1C,  //  00F8  GETMET	R9	R5	K28
      0x7C240200,  //  00F9  CALL	R9	1
      0xB8262E00,  //  00FA  GETNGBL	R9	K23
      0x8C241318,  //  00FB  GETMET	R9	R9	K24
      0x542E0003,  //  00FC  LDINT	R11	4
      0x7C240400,  //  00FD  CALL	R9	2
      0x78260013,  //  00FE  JMPF	R9	#0113
      0xB8262E00,  //  00FF  GETNGBL	R9	K23
      0x8C241302,  //  0100  GETMET	R9	R9	K2
      0x602C0008,  //  0101  GETGBL	R11	G8
      0x88300B36,  //  0102  GETMBR	R12	R5	K54
      0x7C2C0200,  //  0103  CALL	R11	1
      0x002E6A0B,  //  0104  ADD	R11	K53	R11
      0x002C1737,  //  0105  ADD	R11	R11	K55
      0x60300008,  //  0106  GETGBL	R12	G8
      0x88340B1E,  //  0107  GETMBR	R13	R5	K30
      0x7C300200,  //  0108  CALL	R12	1
      0x002C160C,  //  0109  ADD	R11	R11	R12
      0x002C1738,  //  010A  ADD	R11	R11	K56
      0x60300008,  //  010B  GETGBL	R12	G8
      0x88340B23,  //  010C  GETMBR	R13	R5	K35
      0x543AFFFE,  //  010D  LDINT	R14	65535
      0x2C341A0E,  //  010E  AND	R13	R13	R14
      0x7C300200,  //  010F  CALL	R12	1
      0x002C160C,  //  0110  ADD	R11	R11	R12
      0x54320003,  //  0111  LDINT	R12	4
      0x7C240600,  //  0112  CALL	R9	3
      0x88240107,  //  0113  GETMBR	R9	R0	K7
      0x8C24131D,  //  0114  GETMET	R9	R9	K29
      0x5C2C0A00,  //  0115  MOVE	R11	R5
      0x7C240400,  //  0116  CALL	R9	2
      0x88240B36,  //  0117  GETMBR	R9	R5	K54
      0x1C28130E,  //  0118  EQ	R10	R9	K14
      0x782A000F,  //  0119  JMPF	R10	#012A
      0x88280B1E,  //  011A  GETMBR	R10	R5	K30
      0x542E000F,  //  011B  LDINT	R11	16
      0x1C28140B,  //  011C  EQ	R10	R10	R11
      0x782A0009,  //  011D  JMPF	R10	#0128
      0x88280139,  //  011E  GETMBR	R10	R0	K57
      0x8C28153A,  //  011F  GETMET	R10	R10	K58
      0x5C300A00,  //  0120  MOVE	R12	R5
      0x7C280400,  //  0121  CALL	R10	2
      0x5C101400,  //  0122  MOVE	R4	R10
      0x78120003,  //  0123  JMPF	R4	#0128
      0x88280139,  //  0124  GETMBR	R10	R0	K57
      0x8C28153B,  //  0125  GETMET	R10	R10	K59
      0x5C300000,  //  0126  MOVE	R12	R0
      0x7C280400,  //  0127  CALL	R10	2
      0x50100200,  //  0128  LDBOOL	R4	1	0
      0x70020024,  //  0129  JMP		#014F
      0x1C28133C,  //  012A  EQ	R10	R9	K60
      0x782A001A,  //  012B  JMPF	R10	#0147
      0xB82A0000,  //  012C  GETNGBL	R10	K0
      0x88281501,  //  012D  GETMBR	R10	R10	K1
      0x8C281502,  //  012E  GETMET	R10	R10	K2
      0x5830003D,  //  012F  LDCONST	R12	K61
      0x7C280400,  //  0130  CALL	R10	2
      0x88280139,  //  0131  GETMBR	R10	R0	K57
      0x8C28152A,  //  0132  GETMET	R10	R10	K42
      0x5C300A00,  //  0133  MOVE	R12	R5
      0x7C280400,  //  0134  CALL	R10	2
      0x5C101400,  //  0135  MOVE	R4	R10
      0xB82A0000,  //  0136  GETNGBL	R10	K0
      0x88281501,  //  0137  GETMBR	R10	R10	K1
      0x8C281502,  //  0138  GETMET	R10	R10	K2
      0x5830003E,  //  0139  LDCONST	R12	K62
      0x7C280400,  //  013A  CALL	R10	2
      0x78120004,  //  013B  JMPF	R4	#0141
      0x88280139,  //  013C  GETMBR	R10	R0	K57
      0x8C28153B,  //  013D  GETMET	R10	R10	K59
      0x5C300000,  //  013E  MOVE	R12	R0
      0x7C280400,  //  013F  CALL	R10	2
      0x70020003,  //  0140  JMP		#0145
      0x8C280132,  //  0141  GETMET	R10	R0	K50
      0x5C300A00,  //  0142  MOVE	R12	R5
      0x50340200,  //  0143  LDBOOL	R13	1	0
      0x7C280600,  //  0144  CALL	R10	3
      0x50100200,  //  0145  LDBOOL	R4	1	0
      0x70020007,  //  0146  JMP		#014F
      0xB82A2E00,  //  0147  GETNGBL	R10	K23
      0x8C281502,  //  0148  GETMET	R10	R10	K2
      0x60300008,  //  0149  GETGBL	R12	G8
      0x5C341200,  //  014A  MOVE	R13	R9
      0x7C300200,  //  014B  CALL	R12	1
      0x00327E0C,  //  014C  ADD	R12	K63	R12
      0x58340021,  //  014D  LDCONST	R13	K33
      0x7C280600,  //  014E  CALL	R10	3
      0xA8040001,  //  014F  EXBLK	1	1
      0x80040800,  //  0150  RET	1	R4
      0xA8040001,  //  0151  EXBLK	1	1
      0x70020018,  //  0152  JMP		#016C
      0xAC140002,  //  0153  CATCH	R5	0	2
      0x70020015,  //  0154  JMP		#016B
      0xB81E2E00,  //  0155  GETNGBL	R7	K23
      0x8C1C0F02,  //  0156  GETMET	R7	R7	K2
      0x60240008,  //  0157  GETGBL	R9	G8
      0x5C280A00,  //  0158  MOVE	R10	R5
      0x7C240200,  //  0159  CALL	R9	1
      0x00268009,  //  015A  ADD	R9	K64	R9
      0x00241341,  //  015B  ADD	R9	R9	K65
      0x60280008,  //  015C  GETGBL	R10	G8
      0x5C2C0C00,  //  015D  MOVE	R11	R6
      0x7C280200,  //  015E  CALL	R10	1
      0x0024120A,  //  015F  ADD	R9	R9	R10
      0x58280042,  //  0160  LDCONST	R10	K66
      0x7C1C0600,  //  0161  CALL	R7	3
      0xB81E2E00,  //  0162  GETNGBL	R7	K23
      0x881C0F43,  //  0163  GETMBR	R7	R7	K67
      0x781E0002,  //  0164  JMPF	R7	#0168
      0xA41E8800,  //  0165  IMPORT	R7	K68
      0x8C200F45,  //  0166  GETMET	R8	R7	K69
      0x7C200200,  //  0167  CALL	R8	1
      0x501C0000,  //  0168  LDBOOL	R7	0	0
      0x80040E00,  //  0169  RET	1	R7
      0x70020000,  //  016A  JMP		#016C
      0xB0080000,  //  016B  RAISE	2	R0	R0
      0x80000000,  //  016C  RET	0
    })
  )
);
/*******************************************************************/


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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(send_response_frame),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(msg_send),
    }),
    be_str_weak(send_response_frame),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x7C080400,  //  0004  CALL	R2	2
      0x88080104,  //  0005  GETMBR	R2	R0	K4
      0x8C080505,  //  0006  GETMET	R2	R2	K5
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x80000000,  //  0009  RET	0
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(x_flag_r),
    /* K1   */  be_nested_str_weak(build_standalone_ack),
    /* K2   */  be_nested_str_weak(encode_frame),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(loglevel),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X25s),
    /* K7   */  be_nested_str_weak(session),
    /* K8   */  be_nested_str_weak(local_session_id),
    /* K9   */  be_nested_str_weak(ack_message_counter),
    /* K10  */  be_nested_str_weak(message_counter),
    /* K11  */  be_nested_str_weak(_X7Breliable_X7D),
    /* K12  */  be_nested_str_weak(),
    /* K13  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(send_simple_ack),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x880C0300,  //  0000  GETMBR	R3	R1	K0
      0x780E001B,  //  0001  JMPF	R3	#001E
      0x8C0C0301,  //  0002  GETMET	R3	R1	K1
      0x5C140400,  //  0003  MOVE	R5	R2
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C100702,  //  0005  GETMET	R4	R3	K2
      0x7C100200,  //  0006  CALL	R4	1
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x8C100904,  //  0008  GETMET	R4	R4	K4
      0x541A0003,  //  0009  LDINT	R6	4
      0x7C100400,  //  000A  CALL	R4	2
      0x7812000E,  //  000B  JMPF	R4	#001B
      0xB8120600,  //  000C  GETNGBL	R4	K3
      0x8C100905,  //  000D  GETMET	R4	R4	K5
      0x60180018,  //  000E  GETGBL	R6	G24
      0x581C0006,  //  000F  LDCONST	R7	K6
      0x88200707,  //  0010  GETMBR	R8	R3	K7
      0x88201108,  //  0011  GETMBR	R8	R8	K8
      0x88240709,  //  0012  GETMBR	R9	R3	K9
      0x8828070A,  //  0013  GETMBR	R10	R3	K10
      0x780A0001,  //  0014  JMPF	R2	#0017
      0x582C000B,  //  0015  LDCONST	R11	K11
      0x70020000,  //  0016  JMP		#0018
      0x582C000C,  //  0017  LDCONST	R11	K12
      0x7C180A00,  //  0018  CALL	R6	5
      0x541E0003,  //  0019  LDINT	R7	4
      0x7C100600,  //  001A  CALL	R4	3
      0x8C10010D,  //  001B  GETMET	R4	R0	K13
      0x5C180600,  //  001C  MOVE	R6	R3
      0x7C100400,  //  001D  CALL	R4	2
      0x80000000,  //  001E  RET	0
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
** Solidified class: Matter_MessageHandler
********************************************************************/
be_local_class(Matter_MessageHandler,
    5,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(commissioning, -1), be_const_var(1) },
        { be_const_key_weak(_n_bytes, 7), be_const_var(4) },
        { be_const_key_weak(send_encrypted_ack, -1), be_const_closure(Matter_MessageHandler_send_encrypted_ack_closure) },
        { be_const_key_weak(init, 5), be_const_closure(Matter_MessageHandler_init_closure) },
        { be_const_key_weak(msg_received, -1), be_const_closure(Matter_MessageHandler_msg_received_closure) },
        { be_const_key_weak(im, 11), be_const_var(2) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_MessageHandler_every_second_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_MessageHandler_every_250ms_closure) },
        { be_const_key_weak(send_simple_ack, 1), be_const_closure(Matter_MessageHandler_send_simple_ack_closure) },
        { be_const_key_weak(send_response_frame, 6), be_const_closure(Matter_MessageHandler_send_response_frame_closure) },
        { be_const_key_weak(control_message, -1), be_const_var(3) },
        { be_const_key_weak(device, -1), be_const_var(0) },
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
