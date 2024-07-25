/* Solidification of Matter_MessageHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_MessageHandler' ktab size: 79, total: 110 (saved 248 bytes)
static const bvalue be_ktab_class_Matter_MessageHandler[79] = {
  /* K0   */  be_nested_str_weak(device),
  /* K1   */  be_nested_str_weak(commissioning),
  /* K2   */  be_nested_str_weak(matter),
  /* K3   */  be_nested_str_weak(Commisioning_Context),
  /* K4   */  be_nested_str_weak(im),
  /* K5   */  be_nested_str_weak(IM),
  /* K6   */  be_nested_str_weak(control_message),
  /* K7   */  be_nested_str_weak(Control_Message),
  /* K8   */  be_nested_str_weak(_n_bytes),
  /* K9   */  be_nested_str_weak(x_flag_r),
  /* K10  */  be_nested_str_weak(build_standalone_ack),
  /* K11  */  be_nested_str_weak(encode_frame),
  /* K12  */  be_nested_str_weak(encrypt),
  /* K13  */  be_nested_str_weak(tasmota),
  /* K14  */  be_nested_str_weak(loglevel),
  /* K15  */  be_nested_str_weak(log),
  /* K16  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X2A_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X25s),
  /* K17  */  be_nested_str_weak(session),
  /* K18  */  be_nested_str_weak(local_session_id),
  /* K19  */  be_nested_str_weak(ack_message_counter),
  /* K20  */  be_nested_str_weak(message_counter),
  /* K21  */  be_nested_str_weak(_X7Breliable_X7D),
  /* K22  */  be_nested_str_weak(),
  /* K23  */  be_nested_str_weak(send_response_frame),
  /* K24  */  be_nested_str_weak(Frame),
  /* K25  */  be_nested_str_weak(decode_header),
  /* K26  */  be_nested_str_weak(sec_p),
  /* K27  */  be_nested_str_weak(sessions),
  /* K28  */  be_nested_str_weak(find_session_source_id_unsecure),
  /* K29  */  be_nested_str_weak(source_node_id),
  /* K30  */  be_nested_str_weak(process_incoming_control_message),
  /* K31  */  be_const_int(0),
  /* K32  */  be_nested_str_weak(sec_sesstype),
  /* K33  */  be_nested_str_weak(_ip),
  /* K34  */  be_nested_str_weak(_port),
  /* K35  */  be_nested_str_weak(_message_handler),
  /* K36  */  be_nested_str_weak(_counter_insecure_rcv),
  /* K37  */  be_nested_str_weak(validate),
  /* K38  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
  /* K39  */  be_nested_str_weak(val),
  /* K40  */  be_nested_str_weak(send_simple_ack),
  /* K41  */  be_nested_str_weak(decode_payload),
  /* K42  */  be_nested_str_weak(received_ack),
  /* K43  */  be_nested_str_weak(opcode),
  /* K44  */  be_nested_str_weak(get_opcode_name),
  /* K45  */  be_nested_str_weak(0x_X2502X),
  /* K46  */  be_const_int(3),
  /* K47  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20from_X20_X5B_X25s_X5D_X3A_X25i),
  /* K48  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
  /* K49  */  be_nested_str_weak(_X7Breliable_X7D_X20),
  /* K50  */  be_nested_str_weak(exchange_id),
  /* K51  */  be_nested_str_weak(process_incoming),
  /* K52  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
  /* K53  */  be_nested_str_weak(get_session_by_local_session_id),
  /* K54  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
  /* K55  */  be_nested_str_weak(counter_rcv_validate),
  /* K56  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20encrypted_X20message_X20_X3D_X20),
  /* K57  */  be_nested_str_weak(_X20counter_X3D),
  /* K58  */  be_nested_str_weak(counter_rcv),
  /* K59  */  be_nested_str_weak(send_encrypted_ack),
  /* K60  */  be_nested_str_weak(decrypt),
  /* K61  */  be_nested_str_weak(MTR_X3A_X20_X3E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Decrypted_X20message_X3A_X20protocol_id_X3A),
  /* K62  */  be_nested_str_weak(protocol_id),
  /* K63  */  be_nested_str_weak(_X20opcode_X3D),
  /* K64  */  be_nested_str_weak(_X20exchange_id_X3D),
  /* K65  */  be_nested_str_weak(process_incoming_ack),
  /* K66  */  be_nested_str_weak(send_enqueued),
  /* K67  */  be_const_int(1),
  /* K68  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
  /* K69  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
  /* K70  */  be_nested_str_weak(_X3B),
  /* K71  */  be_const_int(2),
  /* K72  */  be_nested_str_weak(_debug_present),
  /* K73  */  be_nested_str_weak(debug),
  /* K74  */  be_nested_str_weak(traceback),
  /* K75  */  be_nested_str_weak(msg_send),
  /* K76  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X25s),
  /* K77  */  be_nested_str_weak(every_50ms),
  /* K78  */  be_nested_str_weak(every_second),
};


extern const bclass be_class_Matter_MessageHandler;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_MessageHandler_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_MessageHandler,     /* shared constants */
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
be_local_closure(class_Matter_MessageHandler_send_encrypted_ack,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_MessageHandler,     /* shared constants */
    be_str_weak(send_encrypted_ack),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x880C0309,  //  0000  GETMBR	R3	R1	K9
      0x780E001C,  //  0001  JMPF	R3	#001F
      0x8C0C030A,  //  0002  GETMET	R3	R1	K10
      0x5C140400,  //  0003  MOVE	R5	R2
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C10070B,  //  0005  GETMET	R4	R3	K11
      0x7C100200,  //  0006  CALL	R4	1
      0x8C10070C,  //  0007  GETMET	R4	R3	K12
      0x7C100200,  //  0008  CALL	R4	1
      0xB8121A00,  //  0009  GETNGBL	R4	K13
      0x8C10090E,  //  000A  GETMET	R4	R4	K14
      0x541A0003,  //  000B  LDINT	R6	4
      0x7C100400,  //  000C  CALL	R4	2
      0x7812000D,  //  000D  JMPF	R4	#001C
      0xB8121E00,  //  000E  GETNGBL	R4	K15
      0x60140018,  //  000F  GETGBL	R5	G24
      0x58180010,  //  0010  LDCONST	R6	K16
      0x881C0711,  //  0011  GETMBR	R7	R3	K17
      0x881C0F12,  //  0012  GETMBR	R7	R7	K18
      0x88200713,  //  0013  GETMBR	R8	R3	K19
      0x88240714,  //  0014  GETMBR	R9	R3	K20
      0x780A0001,  //  0015  JMPF	R2	#0018
      0x58280015,  //  0016  LDCONST	R10	K21
      0x70020000,  //  0017  JMP		#0019
      0x58280016,  //  0018  LDCONST	R10	K22
      0x7C140A00,  //  0019  CALL	R5	5
      0x541A0003,  //  001A  LDINT	R6	4
      0x7C100400,  //  001B  CALL	R4	2
      0x8C100117,  //  001C  GETMET	R4	R0	K23
      0x5C180600,  //  001D  MOVE	R6	R3
      0x7C100400,  //  001E  CALL	R4	2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(class_Matter_MessageHandler_msg_received,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_MessageHandler,     /* shared constants */
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[334]) {  /* code */
      0x50100000,  //  0000  LDBOOL	R4	0	0
      0xA8020132,  //  0001  EXBLK	0	#0135
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B18,  //  0003  GETMET	R5	R5	K24
      0x5C1C0000,  //  0004  MOVE	R7	R0
      0x5C200200,  //  0005  MOVE	R8	R1
      0x5C240400,  //  0006  MOVE	R9	R2
      0x5C280600,  //  0007  MOVE	R10	R3
      0x7C140A00,  //  0008  CALL	R5	5
      0x8C180B19,  //  0009  GETMET	R6	R5	K25
      0x7C180200,  //  000A  CALL	R6	1
      0x5C1C0C00,  //  000B  MOVE	R7	R6
      0x741E0002,  //  000C  JMPT	R7	#0010
      0x501C0000,  //  000D  LDBOOL	R7	0	0
      0xA8040001,  //  000E  EXBLK	1	1
      0x80040E00,  //  000F  RET	1	R7
      0x881C0B1A,  //  0010  GETMBR	R7	R5	K26
      0x781E000C,  //  0011  JMPF	R7	#001F
      0x881C0100,  //  0012  GETMBR	R7	R0	K0
      0x881C0F1B,  //  0013  GETMBR	R7	R7	K27
      0x8C1C0F1C,  //  0014  GETMET	R7	R7	K28
      0x88240B1D,  //  0015  GETMBR	R9	R5	K29
      0x542A0059,  //  0016  LDINT	R10	90
      0x7C1C0600,  //  0017  CALL	R7	3
      0x88200106,  //  0018  GETMBR	R8	R0	K6
      0x8C20111E,  //  0019  GETMET	R8	R8	K30
      0x5C280A00,  //  001A  MOVE	R10	R5
      0x7C200400,  //  001B  CALL	R8	2
      0xA8040001,  //  001C  EXBLK	1	1
      0x80041000,  //  001D  RET	1	R8
      0x70020111,  //  001E  JMP		#0131
      0x881C0B12,  //  001F  GETMBR	R7	R5	K18
      0x1C1C0F1F,  //  0020  EQ	R7	R7	K31
      0x781E007A,  //  0021  JMPF	R7	#009D
      0x881C0B20,  //  0022  GETMBR	R7	R5	K32
      0x1C1C0F1F,  //  0023  EQ	R7	R7	K31
      0x781E0077,  //  0024  JMPF	R7	#009D
      0x881C0100,  //  0025  GETMBR	R7	R0	K0
      0x881C0F1B,  //  0026  GETMBR	R7	R7	K27
      0x8C1C0F1C,  //  0027  GETMET	R7	R7	K28
      0x88240B1D,  //  0028  GETMBR	R9	R5	K29
      0x542A0059,  //  0029  LDINT	R10	90
      0x7C1C0600,  //  002A  CALL	R7	3
      0x780A0000,  //  002B  JMPF	R2	#002D
      0x901E4202,  //  002C  SETMBR	R7	K33	R2
      0x780E0000,  //  002D  JMPF	R3	#002F
      0x901E4403,  //  002E  SETMBR	R7	K34	R3
      0x901E4600,  //  002F  SETMBR	R7	K35	R0
      0x90162207,  //  0030  SETMBR	R5	K17	R7
      0x88200F24,  //  0031  GETMBR	R8	R7	K36
      0x8C201125,  //  0032  GETMET	R8	R8	K37
      0x88280B14,  //  0033  GETMBR	R10	R5	K20
      0x502C0000,  //  0034  LDBOOL	R11	0	0
      0x7C200600,  //  0035  CALL	R8	3
      0x74220015,  //  0036  JMPT	R8	#004D
      0xB8221A00,  //  0037  GETNGBL	R8	K13
      0x8C20110E,  //  0038  GETMET	R8	R8	K14
      0x542A0003,  //  0039  LDINT	R10	4
      0x7C200400,  //  003A  CALL	R8	2
      0x78220009,  //  003B  JMPF	R8	#0046
      0xB8221E00,  //  003C  GETNGBL	R8	K15
      0x60240018,  //  003D  GETGBL	R9	G24
      0x58280026,  //  003E  LDCONST	R10	K38
      0x882C0B14,  //  003F  GETMBR	R11	R5	K20
      0x88300F24,  //  0040  GETMBR	R12	R7	K36
      0x8C301927,  //  0041  GETMET	R12	R12	K39
      0x7C300200,  //  0042  CALL	R12	1
      0x7C240600,  //  0043  CALL	R9	3
      0x542A0003,  //  0044  LDINT	R10	4
      0x7C200400,  //  0045  CALL	R8	2
      0x8C200128,  //  0046  GETMET	R8	R0	K40
      0x5C280A00,  //  0047  MOVE	R10	R5
      0x502C0000,  //  0048  LDBOOL	R11	0	0
      0x7C200600,  //  0049  CALL	R8	3
      0x50200000,  //  004A  LDBOOL	R8	0	0
      0xA8040001,  //  004B  EXBLK	1	1
      0x80041000,  //  004C  RET	1	R8
      0x8C200B29,  //  004D  GETMET	R8	R5	K41
      0x7C200200,  //  004E  CALL	R8	1
      0x74220002,  //  004F  JMPT	R8	#0053
      0x50200000,  //  0050  LDBOOL	R8	0	0
      0xA8040001,  //  0051  EXBLK	1	1
      0x80041000,  //  0052  RET	1	R8
      0x88200100,  //  0053  GETMBR	R8	R0	K0
      0x8C20112A,  //  0054  GETMET	R8	R8	K42
      0x5C280A00,  //  0055  MOVE	R10	R5
      0x7C200400,  //  0056  CALL	R8	2
      0x88200B2B,  //  0057  GETMBR	R8	R5	K43
      0x5426000F,  //  0058  LDINT	R9	16
      0x20201009,  //  0059  NE	R8	R8	R9
      0x7822001A,  //  005A  JMPF	R8	#0076
      0xB8220400,  //  005B  GETNGBL	R8	K2
      0x8C20112C,  //  005C  GETMET	R8	R8	K44
      0x88280B2B,  //  005D  GETMBR	R10	R5	K43
      0x7C200400,  //  005E  CALL	R8	2
      0x5C241000,  //  005F  MOVE	R9	R8
      0x74260004,  //  0060  JMPT	R9	#0066
      0x60240018,  //  0061  GETGBL	R9	G24
      0x5828002D,  //  0062  LDCONST	R10	K45
      0x882C0B2B,  //  0063  GETMBR	R11	R5	K43
      0x7C240400,  //  0064  CALL	R9	2
      0x5C201200,  //  0065  MOVE	R8	R9
      0xB8261A00,  //  0066  GETNGBL	R9	K13
      0x8C24130E,  //  0067  GETMET	R9	R9	K14
      0x582C002E,  //  0068  LDCONST	R11	K46
      0x7C240400,  //  0069  CALL	R9	2
      0x78260009,  //  006A  JMPF	R9	#0075
      0xB8261E00,  //  006B  GETNGBL	R9	K15
      0x60280018,  //  006C  GETGBL	R10	G24
      0x582C002F,  //  006D  LDCONST	R11	K47
      0x88300F12,  //  006E  GETMBR	R12	R7	K18
      0x5C341000,  //  006F  MOVE	R13	R8
      0x5C380400,  //  0070  MOVE	R14	R2
      0x5C3C0600,  //  0071  MOVE	R15	R3
      0x7C280A00,  //  0072  CALL	R10	5
      0x582C002E,  //  0073  LDCONST	R11	K46
      0x7C240400,  //  0074  CALL	R9	2
      0x70020017,  //  0075  JMP		#008E
      0xB8221A00,  //  0076  GETNGBL	R8	K13
      0x8C20110E,  //  0077  GETMET	R8	R8	K14
      0x542A0003,  //  0078  LDINT	R10	4
      0x7C200400,  //  0079  CALL	R8	2
      0x78220012,  //  007A  JMPF	R8	#008E
      0xB8221E00,  //  007B  GETNGBL	R8	K15
      0x60240018,  //  007C  GETGBL	R9	G24
      0x58280030,  //  007D  LDCONST	R10	K48
      0x882C0F12,  //  007E  GETMBR	R11	R7	K18
      0x88300B14,  //  007F  GETMBR	R12	R5	K20
      0x88340B09,  //  0080  GETMBR	R13	R5	K9
      0x78360001,  //  0081  JMPF	R13	#0084
      0x58340031,  //  0082  LDCONST	R13	K49
      0x70020000,  //  0083  JMP		#0085
      0x58340016,  //  0084  LDCONST	R13	K22
      0x88380B32,  //  0085  GETMBR	R14	R5	K50
      0x603C0008,  //  0086  GETGBL	R15	G8
      0x88400B13,  //  0087  GETMBR	R16	R5	K19
      0x7C3C0200,  //  0088  CALL	R15	1
      0x5C400400,  //  0089  MOVE	R16	R2
      0x5C440600,  //  008A  MOVE	R17	R3
      0x7C241000,  //  008B  CALL	R9	8
      0x542A0003,  //  008C  LDINT	R10	4
      0x7C200400,  //  008D  CALL	R8	2
      0x88200101,  //  008E  GETMBR	R8	R0	K1
      0x8C201133,  //  008F  GETMET	R8	R8	K51
      0x5C280A00,  //  0090  MOVE	R10	R5
      0x7C200400,  //  0091  CALL	R8	2
      0x5C101000,  //  0092  MOVE	R4	R8
      0x5C200800,  //  0093  MOVE	R8	R4
      0x74220003,  //  0094  JMPT	R8	#0099
      0x8C200128,  //  0095  GETMET	R8	R0	K40
      0x5C280A00,  //  0096  MOVE	R10	R5
      0x502C0000,  //  0097  LDBOOL	R11	0	0
      0x7C200600,  //  0098  CALL	R8	3
      0x50200200,  //  0099  LDBOOL	R8	1	0
      0xA8040001,  //  009A  EXBLK	1	1
      0x80041000,  //  009B  RET	1	R8
      0x70020093,  //  009C  JMP		#0131
      0xB81E1A00,  //  009D  GETNGBL	R7	K13
      0x8C1C0F0E,  //  009E  GETMET	R7	R7	K14
      0x54260003,  //  009F  LDINT	R9	4
      0x7C1C0400,  //  00A0  CALL	R7	2
      0x781E0007,  //  00A1  JMPF	R7	#00AA
      0xB81E1E00,  //  00A2  GETNGBL	R7	K15
      0x60200018,  //  00A3  GETGBL	R8	G24
      0x58240034,  //  00A4  LDCONST	R9	K52
      0x88280B12,  //  00A5  GETMBR	R10	R5	K18
      0x882C0B14,  //  00A6  GETMBR	R11	R5	K20
      0x7C200600,  //  00A7  CALL	R8	3
      0x54260003,  //  00A8  LDINT	R9	4
      0x7C1C0400,  //  00A9  CALL	R7	2
      0x881C0100,  //  00AA  GETMBR	R7	R0	K0
      0x881C0F1B,  //  00AB  GETMBR	R7	R7	K27
      0x8C1C0F35,  //  00AC  GETMET	R7	R7	K53
      0x88240B12,  //  00AD  GETMBR	R9	R5	K18
      0x7C1C0400,  //  00AE  CALL	R7	2
      0x4C200000,  //  00AF  LDNIL	R8
      0x1C200E08,  //  00B0  EQ	R8	R7	R8
      0x78220009,  //  00B1  JMPF	R8	#00BC
      0xB8221E00,  //  00B2  GETNGBL	R8	K15
      0x60240008,  //  00B3  GETGBL	R9	G8
      0x88280B12,  //  00B4  GETMBR	R10	R5	K18
      0x7C240200,  //  00B5  CALL	R9	1
      0x00266C09,  //  00B6  ADD	R9	K54	R9
      0x5828002E,  //  00B7  LDCONST	R10	K46
      0x7C200400,  //  00B8  CALL	R8	2
      0x50200000,  //  00B9  LDBOOL	R8	0	0
      0xA8040001,  //  00BA  EXBLK	1	1
      0x80041000,  //  00BB  RET	1	R8
      0x780A0000,  //  00BC  JMPF	R2	#00BE
      0x901E4202,  //  00BD  SETMBR	R7	K33	R2
      0x780E0000,  //  00BE  JMPF	R3	#00C0
      0x901E4403,  //  00BF  SETMBR	R7	K34	R3
      0x901E4600,  //  00C0  SETMBR	R7	K35	R0
      0x90162207,  //  00C1  SETMBR	R5	K17	R7
      0x8C200F37,  //  00C2  GETMET	R8	R7	K55
      0x88280B14,  //  00C3  GETMBR	R10	R5	K20
      0x502C0200,  //  00C4  LDBOOL	R11	1	0
      0x7C200600,  //  00C5  CALL	R8	3
      0x74220017,  //  00C6  JMPT	R8	#00DF
      0xB8221A00,  //  00C7  GETNGBL	R8	K13
      0x8C20110E,  //  00C8  GETMET	R8	R8	K14
      0x5828002E,  //  00C9  LDCONST	R10	K46
      0x7C200400,  //  00CA  CALL	R8	2
      0x7822000B,  //  00CB  JMPF	R8	#00D8
      0xB8221E00,  //  00CC  GETNGBL	R8	K15
      0x60240008,  //  00CD  GETGBL	R9	G8
      0x88280B14,  //  00CE  GETMBR	R10	R5	K20
      0x7C240200,  //  00CF  CALL	R9	1
      0x00267009,  //  00D0  ADD	R9	K56	R9
      0x00241339,  //  00D1  ADD	R9	R9	K57
      0x60280008,  //  00D2  GETGBL	R10	G8
      0x882C0F3A,  //  00D3  GETMBR	R11	R7	K58
      0x7C280200,  //  00D4  CALL	R10	1
      0x0024120A,  //  00D5  ADD	R9	R9	R10
      0x5828002E,  //  00D6  LDCONST	R10	K46
      0x7C200400,  //  00D7  CALL	R8	2
      0x8C20013B,  //  00D8  GETMET	R8	R0	K59
      0x5C280A00,  //  00D9  MOVE	R10	R5
      0x502C0000,  //  00DA  LDBOOL	R11	0	0
      0x7C200600,  //  00DB  CALL	R8	3
      0x50200000,  //  00DC  LDBOOL	R8	0	0
      0xA8040001,  //  00DD  EXBLK	1	1
      0x80041000,  //  00DE  RET	1	R8
      0x8C200B3C,  //  00DF  GETMET	R8	R5	K60
      0x7C200200,  //  00E0  CALL	R8	1
      0x5C241000,  //  00E1  MOVE	R9	R8
      0x74260002,  //  00E2  JMPT	R9	#00E6
      0x50240000,  //  00E3  LDBOOL	R9	0	0
      0xA8040001,  //  00E4  EXBLK	1	1
      0x80041200,  //  00E5  RET	1	R9
      0x8C240B29,  //  00E6  GETMET	R9	R5	K41
      0x7C240200,  //  00E7  CALL	R9	1
      0xB8261A00,  //  00E8  GETNGBL	R9	K13
      0x8C24130E,  //  00E9  GETMET	R9	R9	K14
      0x542E0003,  //  00EA  LDINT	R11	4
      0x7C240400,  //  00EB  CALL	R9	2
      0x78260012,  //  00EC  JMPF	R9	#0100
      0xB8261E00,  //  00ED  GETNGBL	R9	K15
      0x60280008,  //  00EE  GETGBL	R10	G8
      0x882C0B3E,  //  00EF  GETMBR	R11	R5	K62
      0x7C280200,  //  00F0  CALL	R10	1
      0x002A7A0A,  //  00F1  ADD	R10	K61	R10
      0x0028153F,  //  00F2  ADD	R10	R10	K63
      0x602C0008,  //  00F3  GETGBL	R11	G8
      0x88300B2B,  //  00F4  GETMBR	R12	R5	K43
      0x7C2C0200,  //  00F5  CALL	R11	1
      0x0028140B,  //  00F6  ADD	R10	R10	R11
      0x00281540,  //  00F7  ADD	R10	R10	K64
      0x602C0008,  //  00F8  GETGBL	R11	G8
      0x88300B32,  //  00F9  GETMBR	R12	R5	K50
      0x5436FFFE,  //  00FA  LDINT	R13	65535
      0x2C30180D,  //  00FB  AND	R12	R12	R13
      0x7C2C0200,  //  00FC  CALL	R11	1
      0x0028140B,  //  00FD  ADD	R10	R10	R11
      0x542E0003,  //  00FE  LDINT	R11	4
      0x7C240400,  //  00FF  CALL	R9	2
      0x88240100,  //  0100  GETMBR	R9	R0	K0
      0x8C24132A,  //  0101  GETMET	R9	R9	K42
      0x5C2C0A00,  //  0102  MOVE	R11	R5
      0x7C240400,  //  0103  CALL	R9	2
      0x88240B3E,  //  0104  GETMBR	R9	R5	K62
      0x1C28131F,  //  0105  EQ	R10	R9	K31
      0x782A000F,  //  0106  JMPF	R10	#0117
      0x88280B2B,  //  0107  GETMBR	R10	R5	K43
      0x542E000F,  //  0108  LDINT	R11	16
      0x1C28140B,  //  0109  EQ	R10	R10	R11
      0x782A0009,  //  010A  JMPF	R10	#0115
      0x88280104,  //  010B  GETMBR	R10	R0	K4
      0x8C281541,  //  010C  GETMET	R10	R10	K65
      0x5C300A00,  //  010D  MOVE	R12	R5
      0x7C280400,  //  010E  CALL	R10	2
      0x5C101400,  //  010F  MOVE	R4	R10
      0x78120003,  //  0110  JMPF	R4	#0115
      0x88280104,  //  0111  GETMBR	R10	R0	K4
      0x8C281542,  //  0112  GETMET	R10	R10	K66
      0x5C300000,  //  0113  MOVE	R12	R0
      0x7C280400,  //  0114  CALL	R10	2
      0x50100200,  //  0115  LDBOOL	R4	1	0
      0x70020019,  //  0116  JMP		#0131
      0x1C281343,  //  0117  EQ	R10	R9	K67
      0x782A0010,  //  0118  JMPF	R10	#012A
      0x88280104,  //  0119  GETMBR	R10	R0	K4
      0x8C281533,  //  011A  GETMET	R10	R10	K51
      0x5C300A00,  //  011B  MOVE	R12	R5
      0x7C280400,  //  011C  CALL	R10	2
      0x5C101400,  //  011D  MOVE	R4	R10
      0x78120004,  //  011E  JMPF	R4	#0124
      0x88280104,  //  011F  GETMBR	R10	R0	K4
      0x8C281542,  //  0120  GETMET	R10	R10	K66
      0x5C300000,  //  0121  MOVE	R12	R0
      0x7C280400,  //  0122  CALL	R10	2
      0x70020003,  //  0123  JMP		#0128
      0x8C28013B,  //  0124  GETMET	R10	R0	K59
      0x5C300A00,  //  0125  MOVE	R12	R5
      0x50340200,  //  0126  LDBOOL	R13	1	0
      0x7C280600,  //  0127  CALL	R10	3
      0x50100200,  //  0128  LDBOOL	R4	1	0
      0x70020006,  //  0129  JMP		#0131
      0xB82A1E00,  //  012A  GETNGBL	R10	K15
      0x602C0008,  //  012B  GETGBL	R11	G8
      0x5C301200,  //  012C  MOVE	R12	R9
      0x7C2C0200,  //  012D  CALL	R11	1
      0x002E880B,  //  012E  ADD	R11	K68	R11
      0x5830002E,  //  012F  LDCONST	R12	K46
      0x7C280400,  //  0130  CALL	R10	2
      0xA8040001,  //  0131  EXBLK	1	1
      0x80040800,  //  0132  RET	1	R4
      0xA8040001,  //  0133  EXBLK	1	1
      0x70020017,  //  0134  JMP		#014D
      0xAC140002,  //  0135  CATCH	R5	0	2
      0x70020014,  //  0136  JMP		#014C
      0xB81E1E00,  //  0137  GETNGBL	R7	K15
      0x60200008,  //  0138  GETGBL	R8	G8
      0x5C240A00,  //  0139  MOVE	R9	R5
      0x7C200200,  //  013A  CALL	R8	1
      0x00228A08,  //  013B  ADD	R8	K69	R8
      0x00201146,  //  013C  ADD	R8	R8	K70
      0x60240008,  //  013D  GETGBL	R9	G8
      0x5C280C00,  //  013E  MOVE	R10	R6
      0x7C240200,  //  013F  CALL	R9	1
      0x00201009,  //  0140  ADD	R8	R8	R9
      0x58240047,  //  0141  LDCONST	R9	K71
      0x7C1C0400,  //  0142  CALL	R7	2
      0xB81E1A00,  //  0143  GETNGBL	R7	K13
      0x881C0F48,  //  0144  GETMBR	R7	R7	K72
      0x781E0002,  //  0145  JMPF	R7	#0149
      0xA41E9200,  //  0146  IMPORT	R7	K73
      0x8C200F4A,  //  0147  GETMET	R8	R7	K74
      0x7C200200,  //  0148  CALL	R8	1
      0x501C0000,  //  0149  LDBOOL	R7	0	0
      0x80040E00,  //  014A  RET	1	R7
      0x70020000,  //  014B  JMP		#014D
      0xB0080000,  //  014C  RAISE	2	R0	R0
      0x80000000,  //  014D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_response_frame
********************************************************************/
be_local_closure(class_Matter_MessageHandler_send_response_frame,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_MessageHandler,     /* shared constants */
    be_str_weak(send_response_frame),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C08054B,  //  0001  GETMET	R2	R2	K75
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_simple_ack
********************************************************************/
be_local_closure(class_Matter_MessageHandler_send_simple_ack,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_MessageHandler,     /* shared constants */
    be_str_weak(send_simple_ack),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x880C0309,  //  0000  GETMBR	R3	R1	K9
      0x780E001C,  //  0001  JMPF	R3	#001F
      0x8C0C030A,  //  0002  GETMET	R3	R1	K10
      0x5C140400,  //  0003  MOVE	R5	R2
      0x7C0C0400,  //  0004  CALL	R3	2
      0x88100312,  //  0005  GETMBR	R4	R1	K18
      0x900E2404,  //  0006  SETMBR	R3	K18	R4
      0x8C10070B,  //  0007  GETMET	R4	R3	K11
      0x7C100200,  //  0008  CALL	R4	1
      0xB8121A00,  //  0009  GETNGBL	R4	K13
      0x8C10090E,  //  000A  GETMET	R4	R4	K14
      0x541A0003,  //  000B  LDINT	R6	4
      0x7C100400,  //  000C  CALL	R4	2
      0x7812000D,  //  000D  JMPF	R4	#001C
      0xB8121E00,  //  000E  GETNGBL	R4	K15
      0x60140018,  //  000F  GETGBL	R5	G24
      0x5818004C,  //  0010  LDCONST	R6	K76
      0x881C0711,  //  0011  GETMBR	R7	R3	K17
      0x881C0F12,  //  0012  GETMBR	R7	R7	K18
      0x88200713,  //  0013  GETMBR	R8	R3	K19
      0x88240714,  //  0014  GETMBR	R9	R3	K20
      0x780A0001,  //  0015  JMPF	R2	#0018
      0x58280015,  //  0016  LDCONST	R10	K21
      0x70020000,  //  0017  JMP		#0019
      0x58280016,  //  0018  LDCONST	R10	K22
      0x7C140A00,  //  0019  CALL	R5	5
      0x541A0003,  //  001A  LDINT	R6	4
      0x7C100400,  //  001B  CALL	R4	2
      0x8C100117,  //  001C  GETMET	R4	R0	K23
      0x5C180600,  //  001D  MOVE	R6	R3
      0x7C100400,  //  001E  CALL	R4	2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(class_Matter_MessageHandler_every_50ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_MessageHandler,     /* shared constants */
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x8C04034D,  //  0001  GETMET	R1	R1	K77
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_Matter_MessageHandler_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_MessageHandler,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x8C04034E,  //  0001  GETMET	R1	R1	K78
      0x7C040200,  //  0002  CALL	R1	1
      0x88040104,  //  0003  GETMBR	R1	R0	K4
      0x8C04034E,  //  0004  GETMET	R1	R1	K78
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
        { be_const_key_weak(send_encrypted_ack, -1), be_const_closure(class_Matter_MessageHandler_send_encrypted_ack_closure) },
        { be_const_key_weak(init, 5), be_const_closure(class_Matter_MessageHandler_init_closure) },
        { be_const_key_weak(msg_received, -1), be_const_closure(class_Matter_MessageHandler_msg_received_closure) },
        { be_const_key_weak(im, 11), be_const_var(2) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_MessageHandler_every_second_closure) },
        { be_const_key_weak(every_50ms, -1), be_const_closure(class_Matter_MessageHandler_every_50ms_closure) },
        { be_const_key_weak(send_simple_ack, 1), be_const_closure(class_Matter_MessageHandler_send_simple_ack_closure) },
        { be_const_key_weak(send_response_frame, 6), be_const_closure(class_Matter_MessageHandler_send_response_frame_closure) },
        { be_const_key_weak(control_message, -1), be_const_var(3) },
        { be_const_key_weak(device, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_MessageHandler)
);
/********************************************************************/
/* End of solidification */
