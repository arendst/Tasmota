/* Solidification of Matter_MessageHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_MessageHandler;

/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_MessageHandler;
be_local_closure(class_Matter_MessageHandler_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_MessageHandler, 
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
extern const bclass be_class_Matter_MessageHandler;
be_local_closure(class_Matter_MessageHandler_send_encrypted_ack,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_MessageHandler, 
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
    ( &(const binstruction[32]) {  /* code */
      0x880C0300,  //  0000  GETMBR	R3	R1	K0
      0x780E001C,  //  0001  JMPF	R3	#001F
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
      0x7812000D,  //  000D  JMPF	R4	#001C
      0xB8120C00,  //  000E  GETNGBL	R4	K6
      0x60140018,  //  000F  GETGBL	R5	G24
      0x58180007,  //  0010  LDCONST	R6	K7
      0x881C0708,  //  0011  GETMBR	R7	R3	K8
      0x881C0F09,  //  0012  GETMBR	R7	R7	K9
      0x8820070A,  //  0013  GETMBR	R8	R3	K10
      0x8824070B,  //  0014  GETMBR	R9	R3	K11
      0x780A0001,  //  0015  JMPF	R2	#0018
      0x5828000C,  //  0016  LDCONST	R10	K12
      0x70020000,  //  0017  JMP		#0019
      0x5828000D,  //  0018  LDCONST	R10	K13
      0x7C140A00,  //  0019  CALL	R5	5
      0x541A0003,  //  001A  LDINT	R6	4
      0x7C100400,  //  001B  CALL	R4	2
      0x8C10010E,  //  001C  GETMET	R4	R0	K14
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
extern const bclass be_class_Matter_MessageHandler;
be_local_closure(class_Matter_MessageHandler_msg_received,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_MessageHandler, 
    1,                          /* has constants */
    ( &(const bvalue[65]) {     /* constants */
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
    /* K21  */  be_nested_str_weak(loglevel),
    /* K22  */  be_nested_str_weak(log),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20unencrypted_X20message_X20_X3D_X20_X25i_X20ref_X20_X3D_X20_X25i),
    /* K24  */  be_nested_str_weak(val),
    /* K25  */  be_nested_str_weak(send_simple_ack),
    /* K26  */  be_nested_str_weak(decode_payload),
    /* K27  */  be_nested_str_weak(received_ack),
    /* K28  */  be_nested_str_weak(opcode),
    /* K29  */  be_nested_str_weak(get_opcode_name),
    /* K30  */  be_nested_str_weak(0x_X2502X),
    /* K31  */  be_const_int(3),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K34  */  be_nested_str_weak(x_flag_r),
    /* K35  */  be_nested_str_weak(_X7Breliable_X7D_X20),
    /* K36  */  be_nested_str_weak(),
    /* K37  */  be_nested_str_weak(exchange_id),
    /* K38  */  be_nested_str_weak(ack_message_counter),
    /* K39  */  be_nested_str_weak(commissioning),
    /* K40  */  be_nested_str_weak(process_incoming),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20decode_X20header_X3A_X20local_session_id_X3D_X25i_X20message_counter_X3D_X25i),
    /* K42  */  be_nested_str_weak(get_session_by_local_session_id),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
    /* K44  */  be_nested_str_weak(counter_rcv_validate),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Duplicate_X20encrypted_X20message_X20_X3D_X20),
    /* K46  */  be_nested_str_weak(_X20counter_X3D),
    /* K47  */  be_nested_str_weak(counter_rcv),
    /* K48  */  be_nested_str_weak(send_encrypted_ack),
    /* K49  */  be_nested_str_weak(decrypt),
    /* K50  */  be_nested_str_weak(MTR_X3A_X20_X3E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Decrypted_X20message_X3A_X20protocol_id_X3A),
    /* K51  */  be_nested_str_weak(protocol_id),
    /* K52  */  be_nested_str_weak(_X20opcode_X3D),
    /* K53  */  be_nested_str_weak(_X20exchange_id_X3D),
    /* K54  */  be_nested_str_weak(im),
    /* K55  */  be_nested_str_weak(process_incoming_ack),
    /* K56  */  be_nested_str_weak(send_enqueued),
    /* K57  */  be_const_int(1),
    /* K58  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
    /* K60  */  be_nested_str_weak(_X3B),
    /* K61  */  be_const_int(2),
    /* K62  */  be_nested_str_weak(_debug_present),
    /* K63  */  be_nested_str_weak(debug),
    /* K64  */  be_nested_str_weak(traceback),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[334]) {  /* code */
      0x50100000,  //  0000  LDBOOL	R4	0	0
      0xA8020132,  //  0001  EXBLK	0	#0135
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
      0x70020111,  //  001E  JMP		#0131
      0x881C0B0A,  //  001F  GETMBR	R7	R5	K10
      0x1C1C0F0B,  //  0020  EQ	R7	R7	K11
      0x781E007A,  //  0021  JMPF	R7	#009D
      0x881C0B0C,  //  0022  GETMBR	R7	R5	K12
      0x1C1C0F0B,  //  0023  EQ	R7	R7	K11
      0x781E0077,  //  0024  JMPF	R7	#009D
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
      0x74220015,  //  0036  JMPT	R8	#004D
      0xB8222800,  //  0037  GETNGBL	R8	K20
      0x8C201115,  //  0038  GETMET	R8	R8	K21
      0x542A0003,  //  0039  LDINT	R10	4
      0x7C200400,  //  003A  CALL	R8	2
      0x78220009,  //  003B  JMPF	R8	#0046
      0xB8222C00,  //  003C  GETNGBL	R8	K22
      0x60240018,  //  003D  GETGBL	R9	G24
      0x58280017,  //  003E  LDCONST	R10	K23
      0x882C0B13,  //  003F  GETMBR	R11	R5	K19
      0x88300F11,  //  0040  GETMBR	R12	R7	K17
      0x8C301918,  //  0041  GETMET	R12	R12	K24
      0x7C300200,  //  0042  CALL	R12	1
      0x7C240600,  //  0043  CALL	R9	3
      0x542A0003,  //  0044  LDINT	R10	4
      0x7C200400,  //  0045  CALL	R8	2
      0x8C200119,  //  0046  GETMET	R8	R0	K25
      0x5C280A00,  //  0047  MOVE	R10	R5
      0x502C0000,  //  0048  LDBOOL	R11	0	0
      0x7C200600,  //  0049  CALL	R8	3
      0x50200000,  //  004A  LDBOOL	R8	0	0
      0xA8040001,  //  004B  EXBLK	1	1
      0x80041000,  //  004C  RET	1	R8
      0x8C200B1A,  //  004D  GETMET	R8	R5	K26
      0x7C200200,  //  004E  CALL	R8	1
      0x74220002,  //  004F  JMPT	R8	#0053
      0x50200000,  //  0050  LDBOOL	R8	0	0
      0xA8040001,  //  0051  EXBLK	1	1
      0x80041000,  //  0052  RET	1	R8
      0x88200104,  //  0053  GETMBR	R8	R0	K4
      0x8C20111B,  //  0054  GETMET	R8	R8	K27
      0x5C280A00,  //  0055  MOVE	R10	R5
      0x7C200400,  //  0056  CALL	R8	2
      0x88200B1C,  //  0057  GETMBR	R8	R5	K28
      0x5426000F,  //  0058  LDINT	R9	16
      0x20201009,  //  0059  NE	R8	R8	R9
      0x7822001A,  //  005A  JMPF	R8	#0076
      0xB8220000,  //  005B  GETNGBL	R8	K0
      0x8C20111D,  //  005C  GETMET	R8	R8	K29
      0x88280B1C,  //  005D  GETMBR	R10	R5	K28
      0x7C200400,  //  005E  CALL	R8	2
      0x5C241000,  //  005F  MOVE	R9	R8
      0x74260004,  //  0060  JMPT	R9	#0066
      0x60240018,  //  0061  GETGBL	R9	G24
      0x5828001E,  //  0062  LDCONST	R10	K30
      0x882C0B1C,  //  0063  GETMBR	R11	R5	K28
      0x7C240400,  //  0064  CALL	R9	2
      0x5C201200,  //  0065  MOVE	R8	R9
      0xB8262800,  //  0066  GETNGBL	R9	K20
      0x8C241315,  //  0067  GETMET	R9	R9	K21
      0x582C001F,  //  0068  LDCONST	R11	K31
      0x7C240400,  //  0069  CALL	R9	2
      0x78260009,  //  006A  JMPF	R9	#0075
      0xB8262C00,  //  006B  GETNGBL	R9	K22
      0x60280018,  //  006C  GETGBL	R10	G24
      0x582C0020,  //  006D  LDCONST	R11	K32
      0x88300F0A,  //  006E  GETMBR	R12	R7	K10
      0x5C341000,  //  006F  MOVE	R13	R8
      0x5C380400,  //  0070  MOVE	R14	R2
      0x5C3C0600,  //  0071  MOVE	R15	R3
      0x7C280A00,  //  0072  CALL	R10	5
      0x582C001F,  //  0073  LDCONST	R11	K31
      0x7C240400,  //  0074  CALL	R9	2
      0x70020017,  //  0075  JMP		#008E
      0xB8222800,  //  0076  GETNGBL	R8	K20
      0x8C201115,  //  0077  GETMET	R8	R8	K21
      0x542A0003,  //  0078  LDINT	R10	4
      0x7C200400,  //  0079  CALL	R8	2
      0x78220012,  //  007A  JMPF	R8	#008E
      0xB8222C00,  //  007B  GETNGBL	R8	K22
      0x60240018,  //  007C  GETGBL	R9	G24
      0x58280021,  //  007D  LDCONST	R10	K33
      0x882C0F0A,  //  007E  GETMBR	R11	R7	K10
      0x88300B13,  //  007F  GETMBR	R12	R5	K19
      0x88340B22,  //  0080  GETMBR	R13	R5	K34
      0x78360001,  //  0081  JMPF	R13	#0084
      0x58340023,  //  0082  LDCONST	R13	K35
      0x70020000,  //  0083  JMP		#0085
      0x58340024,  //  0084  LDCONST	R13	K36
      0x88380B25,  //  0085  GETMBR	R14	R5	K37
      0x603C0008,  //  0086  GETGBL	R15	G8
      0x88400B26,  //  0087  GETMBR	R16	R5	K38
      0x7C3C0200,  //  0088  CALL	R15	1
      0x5C400400,  //  0089  MOVE	R16	R2
      0x5C440600,  //  008A  MOVE	R17	R3
      0x7C241000,  //  008B  CALL	R9	8
      0x542A0003,  //  008C  LDINT	R10	4
      0x7C200400,  //  008D  CALL	R8	2
      0x88200127,  //  008E  GETMBR	R8	R0	K39
      0x8C201128,  //  008F  GETMET	R8	R8	K40
      0x5C280A00,  //  0090  MOVE	R10	R5
      0x7C200400,  //  0091  CALL	R8	2
      0x5C101000,  //  0092  MOVE	R4	R8
      0x5C200800,  //  0093  MOVE	R8	R4
      0x74220003,  //  0094  JMPT	R8	#0099
      0x8C200119,  //  0095  GETMET	R8	R0	K25
      0x5C280A00,  //  0096  MOVE	R10	R5
      0x502C0000,  //  0097  LDBOOL	R11	0	0
      0x7C200600,  //  0098  CALL	R8	3
      0x50200200,  //  0099  LDBOOL	R8	1	0
      0xA8040001,  //  009A  EXBLK	1	1
      0x80041000,  //  009B  RET	1	R8
      0x70020093,  //  009C  JMP		#0131
      0xB81E2800,  //  009D  GETNGBL	R7	K20
      0x8C1C0F15,  //  009E  GETMET	R7	R7	K21
      0x54260003,  //  009F  LDINT	R9	4
      0x7C1C0400,  //  00A0  CALL	R7	2
      0x781E0007,  //  00A1  JMPF	R7	#00AA
      0xB81E2C00,  //  00A2  GETNGBL	R7	K22
      0x60200018,  //  00A3  GETGBL	R8	G24
      0x58240029,  //  00A4  LDCONST	R9	K41
      0x88280B0A,  //  00A5  GETMBR	R10	R5	K10
      0x882C0B13,  //  00A6  GETMBR	R11	R5	K19
      0x7C200600,  //  00A7  CALL	R8	3
      0x54260003,  //  00A8  LDINT	R9	4
      0x7C1C0400,  //  00A9  CALL	R7	2
      0x881C0104,  //  00AA  GETMBR	R7	R0	K4
      0x881C0F05,  //  00AB  GETMBR	R7	R7	K5
      0x8C1C0F2A,  //  00AC  GETMET	R7	R7	K42
      0x88240B0A,  //  00AD  GETMBR	R9	R5	K10
      0x7C1C0400,  //  00AE  CALL	R7	2
      0x4C200000,  //  00AF  LDNIL	R8
      0x1C200E08,  //  00B0  EQ	R8	R7	R8
      0x78220009,  //  00B1  JMPF	R8	#00BC
      0xB8222C00,  //  00B2  GETNGBL	R8	K22
      0x60240008,  //  00B3  GETGBL	R9	G8
      0x88280B0A,  //  00B4  GETMBR	R10	R5	K10
      0x7C240200,  //  00B5  CALL	R9	1
      0x00265609,  //  00B6  ADD	R9	K43	R9
      0x5828001F,  //  00B7  LDCONST	R10	K31
      0x7C200400,  //  00B8  CALL	R8	2
      0x50200000,  //  00B9  LDBOOL	R8	0	0
      0xA8040001,  //  00BA  EXBLK	1	1
      0x80041000,  //  00BB  RET	1	R8
      0x780A0000,  //  00BC  JMPF	R2	#00BE
      0x901E1A02,  //  00BD  SETMBR	R7	K13	R2
      0x780E0000,  //  00BE  JMPF	R3	#00C0
      0x901E1C03,  //  00BF  SETMBR	R7	K14	R3
      0x901E1E00,  //  00C0  SETMBR	R7	K15	R0
      0x90162007,  //  00C1  SETMBR	R5	K16	R7
      0x8C200F2C,  //  00C2  GETMET	R8	R7	K44
      0x88280B13,  //  00C3  GETMBR	R10	R5	K19
      0x502C0200,  //  00C4  LDBOOL	R11	1	0
      0x7C200600,  //  00C5  CALL	R8	3
      0x74220017,  //  00C6  JMPT	R8	#00DF
      0xB8222800,  //  00C7  GETNGBL	R8	K20
      0x8C201115,  //  00C8  GETMET	R8	R8	K21
      0x5828001F,  //  00C9  LDCONST	R10	K31
      0x7C200400,  //  00CA  CALL	R8	2
      0x7822000B,  //  00CB  JMPF	R8	#00D8
      0xB8222C00,  //  00CC  GETNGBL	R8	K22
      0x60240008,  //  00CD  GETGBL	R9	G8
      0x88280B13,  //  00CE  GETMBR	R10	R5	K19
      0x7C240200,  //  00CF  CALL	R9	1
      0x00265A09,  //  00D0  ADD	R9	K45	R9
      0x0024132E,  //  00D1  ADD	R9	R9	K46
      0x60280008,  //  00D2  GETGBL	R10	G8
      0x882C0F2F,  //  00D3  GETMBR	R11	R7	K47
      0x7C280200,  //  00D4  CALL	R10	1
      0x0024120A,  //  00D5  ADD	R9	R9	R10
      0x5828001F,  //  00D6  LDCONST	R10	K31
      0x7C200400,  //  00D7  CALL	R8	2
      0x8C200130,  //  00D8  GETMET	R8	R0	K48
      0x5C280A00,  //  00D9  MOVE	R10	R5
      0x502C0000,  //  00DA  LDBOOL	R11	0	0
      0x7C200600,  //  00DB  CALL	R8	3
      0x50200000,  //  00DC  LDBOOL	R8	0	0
      0xA8040001,  //  00DD  EXBLK	1	1
      0x80041000,  //  00DE  RET	1	R8
      0x8C200B31,  //  00DF  GETMET	R8	R5	K49
      0x7C200200,  //  00E0  CALL	R8	1
      0x5C241000,  //  00E1  MOVE	R9	R8
      0x74260002,  //  00E2  JMPT	R9	#00E6
      0x50240000,  //  00E3  LDBOOL	R9	0	0
      0xA8040001,  //  00E4  EXBLK	1	1
      0x80041200,  //  00E5  RET	1	R9
      0x8C240B1A,  //  00E6  GETMET	R9	R5	K26
      0x7C240200,  //  00E7  CALL	R9	1
      0xB8262800,  //  00E8  GETNGBL	R9	K20
      0x8C241315,  //  00E9  GETMET	R9	R9	K21
      0x542E0003,  //  00EA  LDINT	R11	4
      0x7C240400,  //  00EB  CALL	R9	2
      0x78260012,  //  00EC  JMPF	R9	#0100
      0xB8262C00,  //  00ED  GETNGBL	R9	K22
      0x60280008,  //  00EE  GETGBL	R10	G8
      0x882C0B33,  //  00EF  GETMBR	R11	R5	K51
      0x7C280200,  //  00F0  CALL	R10	1
      0x002A640A,  //  00F1  ADD	R10	K50	R10
      0x00281534,  //  00F2  ADD	R10	R10	K52
      0x602C0008,  //  00F3  GETGBL	R11	G8
      0x88300B1C,  //  00F4  GETMBR	R12	R5	K28
      0x7C2C0200,  //  00F5  CALL	R11	1
      0x0028140B,  //  00F6  ADD	R10	R10	R11
      0x00281535,  //  00F7  ADD	R10	R10	K53
      0x602C0008,  //  00F8  GETGBL	R11	G8
      0x88300B25,  //  00F9  GETMBR	R12	R5	K37
      0x5436FFFE,  //  00FA  LDINT	R13	65535
      0x2C30180D,  //  00FB  AND	R12	R12	R13
      0x7C2C0200,  //  00FC  CALL	R11	1
      0x0028140B,  //  00FD  ADD	R10	R10	R11
      0x542E0003,  //  00FE  LDINT	R11	4
      0x7C240400,  //  00FF  CALL	R9	2
      0x88240104,  //  0100  GETMBR	R9	R0	K4
      0x8C24131B,  //  0101  GETMET	R9	R9	K27
      0x5C2C0A00,  //  0102  MOVE	R11	R5
      0x7C240400,  //  0103  CALL	R9	2
      0x88240B33,  //  0104  GETMBR	R9	R5	K51
      0x1C28130B,  //  0105  EQ	R10	R9	K11
      0x782A000F,  //  0106  JMPF	R10	#0117
      0x88280B1C,  //  0107  GETMBR	R10	R5	K28
      0x542E000F,  //  0108  LDINT	R11	16
      0x1C28140B,  //  0109  EQ	R10	R10	R11
      0x782A0009,  //  010A  JMPF	R10	#0115
      0x88280136,  //  010B  GETMBR	R10	R0	K54
      0x8C281537,  //  010C  GETMET	R10	R10	K55
      0x5C300A00,  //  010D  MOVE	R12	R5
      0x7C280400,  //  010E  CALL	R10	2
      0x5C101400,  //  010F  MOVE	R4	R10
      0x78120003,  //  0110  JMPF	R4	#0115
      0x88280136,  //  0111  GETMBR	R10	R0	K54
      0x8C281538,  //  0112  GETMET	R10	R10	K56
      0x5C300000,  //  0113  MOVE	R12	R0
      0x7C280400,  //  0114  CALL	R10	2
      0x50100200,  //  0115  LDBOOL	R4	1	0
      0x70020019,  //  0116  JMP		#0131
      0x1C281339,  //  0117  EQ	R10	R9	K57
      0x782A0010,  //  0118  JMPF	R10	#012A
      0x88280136,  //  0119  GETMBR	R10	R0	K54
      0x8C281528,  //  011A  GETMET	R10	R10	K40
      0x5C300A00,  //  011B  MOVE	R12	R5
      0x7C280400,  //  011C  CALL	R10	2
      0x5C101400,  //  011D  MOVE	R4	R10
      0x78120004,  //  011E  JMPF	R4	#0124
      0x88280136,  //  011F  GETMBR	R10	R0	K54
      0x8C281538,  //  0120  GETMET	R10	R10	K56
      0x5C300000,  //  0121  MOVE	R12	R0
      0x7C280400,  //  0122  CALL	R10	2
      0x70020003,  //  0123  JMP		#0128
      0x8C280130,  //  0124  GETMET	R10	R0	K48
      0x5C300A00,  //  0125  MOVE	R12	R5
      0x50340200,  //  0126  LDBOOL	R13	1	0
      0x7C280600,  //  0127  CALL	R10	3
      0x50100200,  //  0128  LDBOOL	R4	1	0
      0x70020006,  //  0129  JMP		#0131
      0xB82A2C00,  //  012A  GETNGBL	R10	K22
      0x602C0008,  //  012B  GETGBL	R11	G8
      0x5C301200,  //  012C  MOVE	R12	R9
      0x7C2C0200,  //  012D  CALL	R11	1
      0x002E740B,  //  012E  ADD	R11	K58	R11
      0x5830001F,  //  012F  LDCONST	R12	K31
      0x7C280400,  //  0130  CALL	R10	2
      0xA8040001,  //  0131  EXBLK	1	1
      0x80040800,  //  0132  RET	1	R4
      0xA8040001,  //  0133  EXBLK	1	1
      0x70020017,  //  0134  JMP		#014D
      0xAC140002,  //  0135  CATCH	R5	0	2
      0x70020014,  //  0136  JMP		#014C
      0xB81E2C00,  //  0137  GETNGBL	R7	K22
      0x60200008,  //  0138  GETGBL	R8	G8
      0x5C240A00,  //  0139  MOVE	R9	R5
      0x7C200200,  //  013A  CALL	R8	1
      0x00227608,  //  013B  ADD	R8	K59	R8
      0x0020113C,  //  013C  ADD	R8	R8	K60
      0x60240008,  //  013D  GETGBL	R9	G8
      0x5C280C00,  //  013E  MOVE	R10	R6
      0x7C240200,  //  013F  CALL	R9	1
      0x00201009,  //  0140  ADD	R8	R8	R9
      0x5824003D,  //  0141  LDCONST	R9	K61
      0x7C1C0400,  //  0142  CALL	R7	2
      0xB81E2800,  //  0143  GETNGBL	R7	K20
      0x881C0F3E,  //  0144  GETMBR	R7	R7	K62
      0x781E0002,  //  0145  JMPF	R7	#0149
      0xA41E7E00,  //  0146  IMPORT	R7	K63
      0x8C200F40,  //  0147  GETMET	R8	R7	K64
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
extern const bclass be_class_Matter_MessageHandler;
be_local_closure(class_Matter_MessageHandler_send_response_frame,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_MessageHandler, 
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
** Solidified function: send_simple_ack
********************************************************************/
extern const bclass be_class_Matter_MessageHandler;
be_local_closure(class_Matter_MessageHandler_send_simple_ack,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_MessageHandler, 
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(x_flag_r),
    /* K1   */  be_nested_str_weak(build_standalone_ack),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_nested_str_weak(encode_frame),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(loglevel),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i_X20_X25s),
    /* K8   */  be_nested_str_weak(session),
    /* K9   */  be_nested_str_weak(ack_message_counter),
    /* K10  */  be_nested_str_weak(message_counter),
    /* K11  */  be_nested_str_weak(_X7Breliable_X7D),
    /* K12  */  be_nested_str_weak(),
    /* K13  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(send_simple_ack),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x880C0300,  //  0000  GETMBR	R3	R1	K0
      0x780E001C,  //  0001  JMPF	R3	#001F
      0x8C0C0301,  //  0002  GETMET	R3	R1	K1
      0x5C140400,  //  0003  MOVE	R5	R2
      0x7C0C0400,  //  0004  CALL	R3	2
      0x88100302,  //  0005  GETMBR	R4	R1	K2
      0x900E0404,  //  0006  SETMBR	R3	K2	R4
      0x8C100703,  //  0007  GETMET	R4	R3	K3
      0x7C100200,  //  0008  CALL	R4	1
      0xB8120800,  //  0009  GETNGBL	R4	K4
      0x8C100905,  //  000A  GETMET	R4	R4	K5
      0x541A0003,  //  000B  LDINT	R6	4
      0x7C100400,  //  000C  CALL	R4	2
      0x7812000D,  //  000D  JMPF	R4	#001C
      0xB8120C00,  //  000E  GETNGBL	R4	K6
      0x60140018,  //  000F  GETGBL	R5	G24
      0x58180007,  //  0010  LDCONST	R6	K7
      0x881C0708,  //  0011  GETMBR	R7	R3	K8
      0x881C0F02,  //  0012  GETMBR	R7	R7	K2
      0x88200709,  //  0013  GETMBR	R8	R3	K9
      0x8824070A,  //  0014  GETMBR	R9	R3	K10
      0x780A0001,  //  0015  JMPF	R2	#0018
      0x5828000B,  //  0016  LDCONST	R10	K11
      0x70020000,  //  0017  JMP		#0019
      0x5828000C,  //  0018  LDCONST	R10	K12
      0x7C140A00,  //  0019  CALL	R5	5
      0x541A0003,  //  001A  LDINT	R6	4
      0x7C100400,  //  001B  CALL	R4	2
      0x8C10010D,  //  001C  GETMET	R4	R0	K13
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
extern const bclass be_class_Matter_MessageHandler;
be_local_closure(class_Matter_MessageHandler_every_50ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_MessageHandler, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(im),
    /* K1   */  be_nested_str_weak(every_50ms),
    }),
    be_str_weak(every_50ms),
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
extern const bclass be_class_Matter_MessageHandler;
be_local_closure(class_Matter_MessageHandler_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_MessageHandler, 
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
