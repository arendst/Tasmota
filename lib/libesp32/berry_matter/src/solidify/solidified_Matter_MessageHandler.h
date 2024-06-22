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
    /* K34  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20_X3Ercv_X20Ack_X20_X20_X20_X28_X256i_X29_X20rid_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25s_X20_X25sfrom_X20_X5B_X25s_X5D_X3A_X25i),
    /* K36  */  be_nested_str_weak(x_flag_r),
    /* K37  */  be_nested_str_weak(_X7Breliable_X7D_X20),
    /* K38  */  be_nested_str_weak(),
    /* K39  */  be_nested_str_weak(exchange_id),
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
    ( &(const binstruction[354]) {  /* code */
      0x50100000,  //  0000  LDBOOL	R4	0	0
      0xB8160000,  //  0001  GETNGBL	R5	K0
      0x88140B01,  //  0002  GETMBR	R5	R5	K1
      0x8C140B02,  //  0003  GETMET	R5	R5	K2
      0x581C0003,  //  0004  LDCONST	R7	K3
      0x7C140400,  //  0005  CALL	R5	2
      0xA8020141,  //  0006  EXBLK	0	#0149
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
      0x70020120,  //  0023  JMP		#0145
      0x881C0B0D,  //  0024  GETMBR	R7	R5	K13
      0x1C1C0F0E,  //  0025  EQ	R7	R7	K14
      0x781E007A,  //  0026  JMPF	R7	#00A2
      0x881C0B0F,  //  0027  GETMBR	R7	R5	K15
      0x1C1C0F0E,  //  0028  EQ	R7	R7	K14
      0x781E0077,  //  0029  JMPF	R7	#00A2
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
      0x74220015,  //  003B  JMPT	R8	#0052
      0xB8222E00,  //  003C  GETNGBL	R8	K23
      0x8C201118,  //  003D  GETMET	R8	R8	K24
      0x542A0003,  //  003E  LDINT	R10	4
      0x7C200400,  //  003F  CALL	R8	2
      0x78220009,  //  0040  JMPF	R8	#004B
      0xB8220400,  //  0041  GETNGBL	R8	K2
      0x60240018,  //  0042  GETGBL	R9	G24
      0x58280019,  //  0043  LDCONST	R10	K25
      0x882C0B16,  //  0044  GETMBR	R11	R5	K22
      0x88300F14,  //  0045  GETMBR	R12	R7	K20
      0x8C30191A,  //  0046  GETMET	R12	R12	K26
      0x7C300200,  //  0047  CALL	R12	1
      0x7C240600,  //  0048  CALL	R9	3
      0x542A0003,  //  0049  LDINT	R10	4
      0x7C200400,  //  004A  CALL	R8	2
      0x8C20011B,  //  004B  GETMET	R8	R0	K27
      0x5C280A00,  //  004C  MOVE	R10	R5
      0x502C0000,  //  004D  LDBOOL	R11	0	0
      0x7C200600,  //  004E  CALL	R8	3
      0x50200000,  //  004F  LDBOOL	R8	0	0
      0xA8040001,  //  0050  EXBLK	1	1
      0x80041000,  //  0051  RET	1	R8
      0x8C200B1C,  //  0052  GETMET	R8	R5	K28
      0x7C200200,  //  0053  CALL	R8	1
      0x74220002,  //  0054  JMPT	R8	#0058
      0x50200000,  //  0055  LDBOOL	R8	0	0
      0xA8040001,  //  0056  EXBLK	1	1
      0x80041000,  //  0057  RET	1	R8
      0x88200107,  //  0058  GETMBR	R8	R0	K7
      0x8C20111D,  //  0059  GETMET	R8	R8	K29
      0x5C280A00,  //  005A  MOVE	R10	R5
      0x7C200400,  //  005B  CALL	R8	2
      0x88200B1E,  //  005C  GETMBR	R8	R5	K30
      0x5426000F,  //  005D  LDINT	R9	16
      0x20201009,  //  005E  NE	R8	R8	R9
      0x7822001A,  //  005F  JMPF	R8	#007B
      0xB8220000,  //  0060  GETNGBL	R8	K0
      0x8C20111F,  //  0061  GETMET	R8	R8	K31
      0x88280B1E,  //  0062  GETMBR	R10	R5	K30
      0x7C200400,  //  0063  CALL	R8	2
      0x5C241000,  //  0064  MOVE	R9	R8
      0x74260004,  //  0065  JMPT	R9	#006B
      0x60240018,  //  0066  GETGBL	R9	G24
      0x58280020,  //  0067  LDCONST	R10	K32
      0x882C0B1E,  //  0068  GETMBR	R11	R5	K30
      0x7C240400,  //  0069  CALL	R9	2
      0x5C201200,  //  006A  MOVE	R8	R9
      0xB8262E00,  //  006B  GETNGBL	R9	K23
      0x8C241318,  //  006C  GETMET	R9	R9	K24
      0x582C0021,  //  006D  LDCONST	R11	K33
      0x7C240400,  //  006E  CALL	R9	2
      0x78260009,  //  006F  JMPF	R9	#007A
      0xB8260400,  //  0070  GETNGBL	R9	K2
      0x60280018,  //  0071  GETGBL	R10	G24
      0x582C0022,  //  0072  LDCONST	R11	K34
      0x88300F0D,  //  0073  GETMBR	R12	R7	K13
      0x5C341000,  //  0074  MOVE	R13	R8
      0x5C380400,  //  0075  MOVE	R14	R2
      0x5C3C0600,  //  0076  MOVE	R15	R3
      0x7C280A00,  //  0077  CALL	R10	5
      0x582C0021,  //  0078  LDCONST	R11	K33
      0x7C240400,  //  0079  CALL	R9	2
      0x70020017,  //  007A  JMP		#0093
      0xB8222E00,  //  007B  GETNGBL	R8	K23
      0x8C201118,  //  007C  GETMET	R8	R8	K24
      0x542A0003,  //  007D  LDINT	R10	4
      0x7C200400,  //  007E  CALL	R8	2
      0x78220012,  //  007F  JMPF	R8	#0093
      0xB8220400,  //  0080  GETNGBL	R8	K2
      0x60240018,  //  0081  GETGBL	R9	G24
      0x58280023,  //  0082  LDCONST	R10	K35
      0x882C0F0D,  //  0083  GETMBR	R11	R7	K13
      0x88300B16,  //  0084  GETMBR	R12	R5	K22
      0x88340B24,  //  0085  GETMBR	R13	R5	K36
      0x78360001,  //  0086  JMPF	R13	#0089
      0x58340025,  //  0087  LDCONST	R13	K37
      0x70020000,  //  0088  JMP		#008A
      0x58340026,  //  0089  LDCONST	R13	K38
      0x88380B27,  //  008A  GETMBR	R14	R5	K39
      0x603C0008,  //  008B  GETGBL	R15	G8
      0x88400B28,  //  008C  GETMBR	R16	R5	K40
      0x7C3C0200,  //  008D  CALL	R15	1
      0x5C400400,  //  008E  MOVE	R16	R2
      0x5C440600,  //  008F  MOVE	R17	R3
      0x7C241000,  //  0090  CALL	R9	8
      0x542A0003,  //  0091  LDINT	R10	4
      0x7C200400,  //  0092  CALL	R8	2
      0x88200129,  //  0093  GETMBR	R8	R0	K41
      0x8C20112A,  //  0094  GETMET	R8	R8	K42
      0x5C280A00,  //  0095  MOVE	R10	R5
      0x7C200400,  //  0096  CALL	R8	2
      0x5C101000,  //  0097  MOVE	R4	R8
      0x5C200800,  //  0098  MOVE	R8	R4
      0x74220003,  //  0099  JMPT	R8	#009E
      0x8C20011B,  //  009A  GETMET	R8	R0	K27
      0x5C280A00,  //  009B  MOVE	R10	R5
      0x502C0000,  //  009C  LDBOOL	R11	0	0
      0x7C200600,  //  009D  CALL	R8	3
      0x50200200,  //  009E  LDBOOL	R8	1	0
      0xA8040001,  //  009F  EXBLK	1	1
      0x80041000,  //  00A0  RET	1	R8
      0x700200A2,  //  00A1  JMP		#0145
      0xB81E2E00,  //  00A2  GETNGBL	R7	K23
      0x8C1C0F18,  //  00A3  GETMET	R7	R7	K24
      0x54260003,  //  00A4  LDINT	R9	4
      0x7C1C0400,  //  00A5  CALL	R7	2
      0x781E0007,  //  00A6  JMPF	R7	#00AF
      0xB81E0400,  //  00A7  GETNGBL	R7	K2
      0x60200018,  //  00A8  GETGBL	R8	G24
      0x5824002B,  //  00A9  LDCONST	R9	K43
      0x88280B0D,  //  00AA  GETMBR	R10	R5	K13
      0x882C0B16,  //  00AB  GETMBR	R11	R5	K22
      0x7C200600,  //  00AC  CALL	R8	3
      0x54260003,  //  00AD  LDINT	R9	4
      0x7C1C0400,  //  00AE  CALL	R7	2
      0x881C0107,  //  00AF  GETMBR	R7	R0	K7
      0x881C0F08,  //  00B0  GETMBR	R7	R7	K8
      0x8C1C0F2C,  //  00B1  GETMET	R7	R7	K44
      0x88240B0D,  //  00B2  GETMBR	R9	R5	K13
      0x7C1C0400,  //  00B3  CALL	R7	2
      0x4C200000,  //  00B4  LDNIL	R8
      0x1C200E08,  //  00B5  EQ	R8	R7	R8
      0x78220009,  //  00B6  JMPF	R8	#00C1
      0xB8220400,  //  00B7  GETNGBL	R8	K2
      0x60240008,  //  00B8  GETGBL	R9	G8
      0x88280B0D,  //  00B9  GETMBR	R10	R5	K13
      0x7C240200,  //  00BA  CALL	R9	1
      0x00265A09,  //  00BB  ADD	R9	K45	R9
      0x58280021,  //  00BC  LDCONST	R10	K33
      0x7C200400,  //  00BD  CALL	R8	2
      0x50200000,  //  00BE  LDBOOL	R8	0	0
      0xA8040001,  //  00BF  EXBLK	1	1
      0x80041000,  //  00C0  RET	1	R8
      0x780A0000,  //  00C1  JMPF	R2	#00C3
      0x901E2002,  //  00C2  SETMBR	R7	K16	R2
      0x780E0000,  //  00C3  JMPF	R3	#00C5
      0x901E2203,  //  00C4  SETMBR	R7	K17	R3
      0x901E2400,  //  00C5  SETMBR	R7	K18	R0
      0x90162607,  //  00C6  SETMBR	R5	K19	R7
      0x8C200F2E,  //  00C7  GETMET	R8	R7	K46
      0x88280B16,  //  00C8  GETMBR	R10	R5	K22
      0x502C0200,  //  00C9  LDBOOL	R11	1	0
      0x7C200600,  //  00CA  CALL	R8	3
      0x74220017,  //  00CB  JMPT	R8	#00E4
      0xB8222E00,  //  00CC  GETNGBL	R8	K23
      0x8C201118,  //  00CD  GETMET	R8	R8	K24
      0x58280021,  //  00CE  LDCONST	R10	K33
      0x7C200400,  //  00CF  CALL	R8	2
      0x7822000B,  //  00D0  JMPF	R8	#00DD
      0xB8220400,  //  00D1  GETNGBL	R8	K2
      0x60240008,  //  00D2  GETGBL	R9	G8
      0x88280B16,  //  00D3  GETMBR	R10	R5	K22
      0x7C240200,  //  00D4  CALL	R9	1
      0x00265E09,  //  00D5  ADD	R9	K47	R9
      0x00241330,  //  00D6  ADD	R9	R9	K48
      0x60280008,  //  00D7  GETGBL	R10	G8
      0x882C0F31,  //  00D8  GETMBR	R11	R7	K49
      0x7C280200,  //  00D9  CALL	R10	1
      0x0024120A,  //  00DA  ADD	R9	R9	R10
      0x58280021,  //  00DB  LDCONST	R10	K33
      0x7C200400,  //  00DC  CALL	R8	2
      0x8C200132,  //  00DD  GETMET	R8	R0	K50
      0x5C280A00,  //  00DE  MOVE	R10	R5
      0x502C0000,  //  00DF  LDBOOL	R11	0	0
      0x7C200600,  //  00E0  CALL	R8	3
      0x50200000,  //  00E1  LDBOOL	R8	0	0
      0xA8040001,  //  00E2  EXBLK	1	1
      0x80041000,  //  00E3  RET	1	R8
      0x8C200B33,  //  00E4  GETMET	R8	R5	K51
      0x7C200200,  //  00E5  CALL	R8	1
      0xB8260000,  //  00E6  GETNGBL	R9	K0
      0x88241301,  //  00E7  GETMBR	R9	R9	K1
      0x8C241302,  //  00E8  GETMET	R9	R9	K2
      0x582C0034,  //  00E9  LDCONST	R11	K52
      0x7C240400,  //  00EA  CALL	R9	2
      0x5C241000,  //  00EB  MOVE	R9	R8
      0x74260002,  //  00EC  JMPT	R9	#00F0
      0x50240000,  //  00ED  LDBOOL	R9	0	0
      0xA8040001,  //  00EE  EXBLK	1	1
      0x80041200,  //  00EF  RET	1	R9
      0x8C240B1C,  //  00F0  GETMET	R9	R5	K28
      0x7C240200,  //  00F1  CALL	R9	1
      0xB8262E00,  //  00F2  GETNGBL	R9	K23
      0x8C241318,  //  00F3  GETMET	R9	R9	K24
      0x542E0003,  //  00F4  LDINT	R11	4
      0x7C240400,  //  00F5  CALL	R9	2
      0x78260012,  //  00F6  JMPF	R9	#010A
      0xB8260400,  //  00F7  GETNGBL	R9	K2
      0x60280008,  //  00F8  GETGBL	R10	G8
      0x882C0B36,  //  00F9  GETMBR	R11	R5	K54
      0x7C280200,  //  00FA  CALL	R10	1
      0x002A6A0A,  //  00FB  ADD	R10	K53	R10
      0x00281537,  //  00FC  ADD	R10	R10	K55
      0x602C0008,  //  00FD  GETGBL	R11	G8
      0x88300B1E,  //  00FE  GETMBR	R12	R5	K30
      0x7C2C0200,  //  00FF  CALL	R11	1
      0x0028140B,  //  0100  ADD	R10	R10	R11
      0x00281538,  //  0101  ADD	R10	R10	K56
      0x602C0008,  //  0102  GETGBL	R11	G8
      0x88300B27,  //  0103  GETMBR	R12	R5	K39
      0x5436FFFE,  //  0104  LDINT	R13	65535
      0x2C30180D,  //  0105  AND	R12	R12	R13
      0x7C2C0200,  //  0106  CALL	R11	1
      0x0028140B,  //  0107  ADD	R10	R10	R11
      0x542E0003,  //  0108  LDINT	R11	4
      0x7C240400,  //  0109  CALL	R9	2
      0x88240107,  //  010A  GETMBR	R9	R0	K7
      0x8C24131D,  //  010B  GETMET	R9	R9	K29
      0x5C2C0A00,  //  010C  MOVE	R11	R5
      0x7C240400,  //  010D  CALL	R9	2
      0x88240B36,  //  010E  GETMBR	R9	R5	K54
      0x1C28130E,  //  010F  EQ	R10	R9	K14
      0x782A000F,  //  0110  JMPF	R10	#0121
      0x88280B1E,  //  0111  GETMBR	R10	R5	K30
      0x542E000F,  //  0112  LDINT	R11	16
      0x1C28140B,  //  0113  EQ	R10	R10	R11
      0x782A0009,  //  0114  JMPF	R10	#011F
      0x88280139,  //  0115  GETMBR	R10	R0	K57
      0x8C28153A,  //  0116  GETMET	R10	R10	K58
      0x5C300A00,  //  0117  MOVE	R12	R5
      0x7C280400,  //  0118  CALL	R10	2
      0x5C101400,  //  0119  MOVE	R4	R10
      0x78120003,  //  011A  JMPF	R4	#011F
      0x88280139,  //  011B  GETMBR	R10	R0	K57
      0x8C28153B,  //  011C  GETMET	R10	R10	K59
      0x5C300000,  //  011D  MOVE	R12	R0
      0x7C280400,  //  011E  CALL	R10	2
      0x50100200,  //  011F  LDBOOL	R4	1	0
      0x70020023,  //  0120  JMP		#0145
      0x1C28133C,  //  0121  EQ	R10	R9	K60
      0x782A001A,  //  0122  JMPF	R10	#013E
      0xB82A0000,  //  0123  GETNGBL	R10	K0
      0x88281501,  //  0124  GETMBR	R10	R10	K1
      0x8C281502,  //  0125  GETMET	R10	R10	K2
      0x5830003D,  //  0126  LDCONST	R12	K61
      0x7C280400,  //  0127  CALL	R10	2
      0x88280139,  //  0128  GETMBR	R10	R0	K57
      0x8C28152A,  //  0129  GETMET	R10	R10	K42
      0x5C300A00,  //  012A  MOVE	R12	R5
      0x7C280400,  //  012B  CALL	R10	2
      0x5C101400,  //  012C  MOVE	R4	R10
      0xB82A0000,  //  012D  GETNGBL	R10	K0
      0x88281501,  //  012E  GETMBR	R10	R10	K1
      0x8C281502,  //  012F  GETMET	R10	R10	K2
      0x5830003E,  //  0130  LDCONST	R12	K62
      0x7C280400,  //  0131  CALL	R10	2
      0x78120004,  //  0132  JMPF	R4	#0138
      0x88280139,  //  0133  GETMBR	R10	R0	K57
      0x8C28153B,  //  0134  GETMET	R10	R10	K59
      0x5C300000,  //  0135  MOVE	R12	R0
      0x7C280400,  //  0136  CALL	R10	2
      0x70020003,  //  0137  JMP		#013C
      0x8C280132,  //  0138  GETMET	R10	R0	K50
      0x5C300A00,  //  0139  MOVE	R12	R5
      0x50340200,  //  013A  LDBOOL	R13	1	0
      0x7C280600,  //  013B  CALL	R10	3
      0x50100200,  //  013C  LDBOOL	R4	1	0
      0x70020006,  //  013D  JMP		#0145
      0xB82A0400,  //  013E  GETNGBL	R10	K2
      0x602C0008,  //  013F  GETGBL	R11	G8
      0x5C301200,  //  0140  MOVE	R12	R9
      0x7C2C0200,  //  0141  CALL	R11	1
      0x002E7E0B,  //  0142  ADD	R11	K63	R11
      0x58300021,  //  0143  LDCONST	R12	K33
      0x7C280400,  //  0144  CALL	R10	2
      0xA8040001,  //  0145  EXBLK	1	1
      0x80040800,  //  0146  RET	1	R4
      0xA8040001,  //  0147  EXBLK	1	1
      0x70020017,  //  0148  JMP		#0161
      0xAC140002,  //  0149  CATCH	R5	0	2
      0x70020014,  //  014A  JMP		#0160
      0xB81E0400,  //  014B  GETNGBL	R7	K2
      0x60200008,  //  014C  GETGBL	R8	G8
      0x5C240A00,  //  014D  MOVE	R9	R5
      0x7C200200,  //  014E  CALL	R8	1
      0x00228008,  //  014F  ADD	R8	K64	R8
      0x00201141,  //  0150  ADD	R8	R8	K65
      0x60240008,  //  0151  GETGBL	R9	G8
      0x5C280C00,  //  0152  MOVE	R10	R6
      0x7C240200,  //  0153  CALL	R9	1
      0x00201009,  //  0154  ADD	R8	R8	R9
      0x58240042,  //  0155  LDCONST	R9	K66
      0x7C1C0400,  //  0156  CALL	R7	2
      0xB81E2E00,  //  0157  GETNGBL	R7	K23
      0x881C0F43,  //  0158  GETMBR	R7	R7	K67
      0x781E0002,  //  0159  JMPF	R7	#015D
      0xA41E8800,  //  015A  IMPORT	R7	K68
      0x8C200F45,  //  015B  GETMET	R8	R7	K69
      0x7C200200,  //  015C  CALL	R8	1
      0x501C0000,  //  015D  LDBOOL	R7	0	0
      0x80040E00,  //  015E  RET	1	R7
      0x70020000,  //  015F  JMP		#0161
      0xB0080000,  //  0160  RAISE	2	R0	R0
      0x80000000,  //  0161  RET	0
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
** Solidified function: every_250ms
********************************************************************/
extern const bclass be_class_Matter_MessageHandler;
be_local_closure(class_Matter_MessageHandler_every_250ms,   /* name */
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
        { be_const_key_weak(every_250ms, -1), be_const_closure(class_Matter_MessageHandler_every_250ms_closure) },
        { be_const_key_weak(send_simple_ack, 1), be_const_closure(class_Matter_MessageHandler_send_simple_ack_closure) },
        { be_const_key_weak(send_response_frame, 6), be_const_closure(class_Matter_MessageHandler_send_response_frame_closure) },
        { be_const_key_weak(control_message, -1), be_const_var(3) },
        { be_const_key_weak(device, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_MessageHandler)
);
/********************************************************************/
/* End of solidification */
