/* Solidification of Matter_MessageHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_MessageHandler' ktab size: 62, total: 76 (saved 112 bytes)
static const bvalue be_ktab_class_Matter_MessageHandler[62] = {
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
  /* K13  */  be_nested_str_weak(send_response_frame),
  /* K14  */  be_nested_str_weak(Frame),
  /* K15  */  be_nested_str_weak(decode_header),
  /* K16  */  be_nested_str_weak(sec_p),
  /* K17  */  be_nested_str_weak(sessions),
  /* K18  */  be_nested_str_weak(find_session_source_id_unsecure),
  /* K19  */  be_nested_str_weak(source_node_id),
  /* K20  */  be_nested_str_weak(process_incoming_control_message),
  /* K21  */  be_nested_str_weak(local_session_id),
  /* K22  */  be_const_int(0),
  /* K23  */  be_nested_str_weak(sec_sesstype),
  /* K24  */  be_nested_str_weak(_ip),
  /* K25  */  be_nested_str_weak(_port),
  /* K26  */  be_nested_str_weak(_message_handler),
  /* K27  */  be_nested_str_weak(session),
  /* K28  */  be_nested_str_weak(_counter_insecure_rcv),
  /* K29  */  be_nested_str_weak(validate),
  /* K30  */  be_nested_str_weak(message_counter),
  /* K31  */  be_nested_str_weak(send_simple_ack),
  /* K32  */  be_nested_str_weak(decode_payload),
  /* K33  */  be_nested_str_weak(received_ack),
  /* K34  */  be_nested_str_weak(opcode),
  /* K35  */  be_nested_str_weak(get_opcode_name),
  /* K36  */  be_nested_str_weak(0x_X2502X),
  /* K37  */  be_nested_str_weak(tasmota),
  /* K38  */  be_nested_str_weak(loglevel),
  /* K39  */  be_const_int(3),
  /* K40  */  be_nested_str_weak(log),
  /* K41  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X28_X256i_X29_X20_X25s_X20from_X20_X5B_X25s_X5D_X3A_X25i),
  /* K42  */  be_nested_str_weak(process_incoming),
  /* K43  */  be_nested_str_weak(get_session_by_local_session_id),
  /* K44  */  be_nested_str_weak(MTR_X3A_X20unknown_X20local_session_id_X3D),
  /* K45  */  be_nested_str_weak(counter_rcv_validate),
  /* K46  */  be_nested_str_weak(send_encrypted_ack),
  /* K47  */  be_nested_str_weak(decrypt),
  /* K48  */  be_nested_str_weak(protocol_id),
  /* K49  */  be_nested_str_weak(process_incoming_ack),
  /* K50  */  be_nested_str_weak(send_enqueued),
  /* K51  */  be_const_int(1),
  /* K52  */  be_nested_str_weak(MTR_X3A_X20ignoring_X20unhandled_X20protocol_id_X3A),
  /* K53  */  be_nested_str_weak(MTR_X3A_X20MessageHandler_X3A_X3Amsg_received_X20exception_X3A_X20),
  /* K54  */  be_nested_str_weak(_X3B),
  /* K55  */  be_const_int(2),
  /* K56  */  be_nested_str_weak(_debug_present),
  /* K57  */  be_nested_str_weak(debug),
  /* K58  */  be_nested_str_weak(traceback),
  /* K59  */  be_nested_str_weak(msg_send),
  /* K60  */  be_nested_str_weak(every_50ms),
  /* K61  */  be_nested_str_weak(every_second),
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
    7,                          /* nstack */
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
    ( &(const binstruction[13]) {  /* code */
      0x880C0309,  //  0000  GETMBR	R3	R1	K9
      0x780E0009,  //  0001  JMPF	R3	#000C
      0x8C0C030A,  //  0002  GETMET	R3	R1	K10
      0x5C140400,  //  0003  MOVE	R5	R2
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C10070B,  //  0005  GETMET	R4	R3	K11
      0x7C100200,  //  0006  CALL	R4	1
      0x8C10070C,  //  0007  GETMET	R4	R3	K12
      0x7C100200,  //  0008  CALL	R4	1
      0x8C10010D,  //  0009  GETMET	R4	R0	K13
      0x5C180600,  //  000A  MOVE	R6	R3
      0x7C100400,  //  000B  CALL	R4	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(class_Matter_MessageHandler_msg_received,   /* name */
  be_nested_proto(
    16,                          /* nstack */
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
    ( &(const binstruction[241]) {  /* code */
      0x50100000,  //  0000  LDBOOL	R4	0	0
      0xA80200D5,  //  0001  EXBLK	0	#00D8
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B0E,  //  0003  GETMET	R5	R5	K14
      0x5C1C0000,  //  0004  MOVE	R7	R0
      0x5C200200,  //  0005  MOVE	R8	R1
      0x5C240400,  //  0006  MOVE	R9	R2
      0x5C280600,  //  0007  MOVE	R10	R3
      0x7C140A00,  //  0008  CALL	R5	5
      0x8C180B0F,  //  0009  GETMET	R6	R5	K15
      0x7C180200,  //  000A  CALL	R6	1
      0x5C1C0C00,  //  000B  MOVE	R7	R6
      0x741E0002,  //  000C  JMPT	R7	#0010
      0x501C0000,  //  000D  LDBOOL	R7	0	0
      0xA8040001,  //  000E  EXBLK	1	1
      0x80040E00,  //  000F  RET	1	R7
      0x881C0B10,  //  0010  GETMBR	R7	R5	K16
      0x781E000C,  //  0011  JMPF	R7	#001F
      0x881C0100,  //  0012  GETMBR	R7	R0	K0
      0x881C0F11,  //  0013  GETMBR	R7	R7	K17
      0x8C1C0F12,  //  0014  GETMET	R7	R7	K18
      0x88240B13,  //  0015  GETMBR	R9	R5	K19
      0x542A0059,  //  0016  LDINT	R10	90
      0x7C1C0600,  //  0017  CALL	R7	3
      0x88200106,  //  0018  GETMBR	R8	R0	K6
      0x8C201114,  //  0019  GETMET	R8	R8	K20
      0x5C280A00,  //  001A  MOVE	R10	R5
      0x7C200400,  //  001B  CALL	R8	2
      0xA8040001,  //  001C  EXBLK	1	1
      0x80041000,  //  001D  RET	1	R8
      0x700200B4,  //  001E  JMP		#00D4
      0x881C0B15,  //  001F  GETMBR	R7	R5	K21
      0x1C1C0F16,  //  0020  EQ	R7	R7	K22
      0x781E0053,  //  0021  JMPF	R7	#0076
      0x881C0B17,  //  0022  GETMBR	R7	R5	K23
      0x1C1C0F16,  //  0023  EQ	R7	R7	K22
      0x781E0050,  //  0024  JMPF	R7	#0076
      0x881C0100,  //  0025  GETMBR	R7	R0	K0
      0x881C0F11,  //  0026  GETMBR	R7	R7	K17
      0x8C1C0F12,  //  0027  GETMET	R7	R7	K18
      0x88240B13,  //  0028  GETMBR	R9	R5	K19
      0x542A0059,  //  0029  LDINT	R10	90
      0x7C1C0600,  //  002A  CALL	R7	3
      0x780A0000,  //  002B  JMPF	R2	#002D
      0x901E3002,  //  002C  SETMBR	R7	K24	R2
      0x780E0000,  //  002D  JMPF	R3	#002F
      0x901E3203,  //  002E  SETMBR	R7	K25	R3
      0x901E3400,  //  002F  SETMBR	R7	K26	R0
      0x90163607,  //  0030  SETMBR	R5	K27	R7
      0x88200F1C,  //  0031  GETMBR	R8	R7	K28
      0x8C20111D,  //  0032  GETMET	R8	R8	K29
      0x88280B1E,  //  0033  GETMBR	R10	R5	K30
      0x502C0000,  //  0034  LDBOOL	R11	0	0
      0x7C200600,  //  0035  CALL	R8	3
      0x74220006,  //  0036  JMPT	R8	#003E
      0x8C20011F,  //  0037  GETMET	R8	R0	K31
      0x5C280A00,  //  0038  MOVE	R10	R5
      0x502C0000,  //  0039  LDBOOL	R11	0	0
      0x7C200600,  //  003A  CALL	R8	3
      0x50200000,  //  003B  LDBOOL	R8	0	0
      0xA8040001,  //  003C  EXBLK	1	1
      0x80041000,  //  003D  RET	1	R8
      0x8C200B20,  //  003E  GETMET	R8	R5	K32
      0x7C200200,  //  003F  CALL	R8	1
      0x74220002,  //  0040  JMPT	R8	#0044
      0x50200000,  //  0041  LDBOOL	R8	0	0
      0xA8040001,  //  0042  EXBLK	1	1
      0x80041000,  //  0043  RET	1	R8
      0x88200100,  //  0044  GETMBR	R8	R0	K0
      0x8C201121,  //  0045  GETMET	R8	R8	K33
      0x5C280A00,  //  0046  MOVE	R10	R5
      0x7C200400,  //  0047  CALL	R8	2
      0x88200B22,  //  0048  GETMBR	R8	R5	K34
      0x5426000F,  //  0049  LDINT	R9	16
      0x20201009,  //  004A  NE	R8	R8	R9
      0x7822001A,  //  004B  JMPF	R8	#0067
      0xB8220400,  //  004C  GETNGBL	R8	K2
      0x8C201123,  //  004D  GETMET	R8	R8	K35
      0x88280B22,  //  004E  GETMBR	R10	R5	K34
      0x7C200400,  //  004F  CALL	R8	2
      0x5C241000,  //  0050  MOVE	R9	R8
      0x74260004,  //  0051  JMPT	R9	#0057
      0x60240018,  //  0052  GETGBL	R9	G24
      0x58280024,  //  0053  LDCONST	R10	K36
      0x882C0B22,  //  0054  GETMBR	R11	R5	K34
      0x7C240400,  //  0055  CALL	R9	2
      0x5C201200,  //  0056  MOVE	R8	R9
      0xB8264A00,  //  0057  GETNGBL	R9	K37
      0x8C241326,  //  0058  GETMET	R9	R9	K38
      0x582C0027,  //  0059  LDCONST	R11	K39
      0x7C240400,  //  005A  CALL	R9	2
      0x78260009,  //  005B  JMPF	R9	#0066
      0xB8265000,  //  005C  GETNGBL	R9	K40
      0x60280018,  //  005D  GETGBL	R10	G24
      0x582C0029,  //  005E  LDCONST	R11	K41
      0x88300F15,  //  005F  GETMBR	R12	R7	K21
      0x5C341000,  //  0060  MOVE	R13	R8
      0x5C380400,  //  0061  MOVE	R14	R2
      0x5C3C0600,  //  0062  MOVE	R15	R3
      0x7C280A00,  //  0063  CALL	R10	5
      0x582C0027,  //  0064  LDCONST	R11	K39
      0x7C240400,  //  0065  CALL	R9	2
      0x7001FFFF,  //  0066  JMP		#0067
      0x88200101,  //  0067  GETMBR	R8	R0	K1
      0x8C20112A,  //  0068  GETMET	R8	R8	K42
      0x5C280A00,  //  0069  MOVE	R10	R5
      0x7C200400,  //  006A  CALL	R8	2
      0x5C101000,  //  006B  MOVE	R4	R8
      0x5C200800,  //  006C  MOVE	R8	R4
      0x74220003,  //  006D  JMPT	R8	#0072
      0x8C20011F,  //  006E  GETMET	R8	R0	K31
      0x5C280A00,  //  006F  MOVE	R10	R5
      0x502C0000,  //  0070  LDBOOL	R11	0	0
      0x7C200600,  //  0071  CALL	R8	3
      0x50200200,  //  0072  LDBOOL	R8	1	0
      0xA8040001,  //  0073  EXBLK	1	1
      0x80041000,  //  0074  RET	1	R8
      0x7002005D,  //  0075  JMP		#00D4
      0x881C0100,  //  0076  GETMBR	R7	R0	K0
      0x881C0F11,  //  0077  GETMBR	R7	R7	K17
      0x8C1C0F2B,  //  0078  GETMET	R7	R7	K43
      0x88240B15,  //  0079  GETMBR	R9	R5	K21
      0x7C1C0400,  //  007A  CALL	R7	2
      0x4C200000,  //  007B  LDNIL	R8
      0x1C200E08,  //  007C  EQ	R8	R7	R8
      0x78220009,  //  007D  JMPF	R8	#0088
      0xB8225000,  //  007E  GETNGBL	R8	K40
      0x60240008,  //  007F  GETGBL	R9	G8
      0x88280B15,  //  0080  GETMBR	R10	R5	K21
      0x7C240200,  //  0081  CALL	R9	1
      0x00265809,  //  0082  ADD	R9	K44	R9
      0x58280027,  //  0083  LDCONST	R10	K39
      0x7C200400,  //  0084  CALL	R8	2
      0x50200000,  //  0085  LDBOOL	R8	0	0
      0xA8040001,  //  0086  EXBLK	1	1
      0x80041000,  //  0087  RET	1	R8
      0x780A0000,  //  0088  JMPF	R2	#008A
      0x901E3002,  //  0089  SETMBR	R7	K24	R2
      0x780E0000,  //  008A  JMPF	R3	#008C
      0x901E3203,  //  008B  SETMBR	R7	K25	R3
      0x901E3400,  //  008C  SETMBR	R7	K26	R0
      0x90163607,  //  008D  SETMBR	R5	K27	R7
      0x8C200F2D,  //  008E  GETMET	R8	R7	K45
      0x88280B1E,  //  008F  GETMBR	R10	R5	K30
      0x502C0200,  //  0090  LDBOOL	R11	1	0
      0x7C200600,  //  0091  CALL	R8	3
      0x74220006,  //  0092  JMPT	R8	#009A
      0x8C20012E,  //  0093  GETMET	R8	R0	K46
      0x5C280A00,  //  0094  MOVE	R10	R5
      0x502C0000,  //  0095  LDBOOL	R11	0	0
      0x7C200600,  //  0096  CALL	R8	3
      0x50200000,  //  0097  LDBOOL	R8	0	0
      0xA8040001,  //  0098  EXBLK	1	1
      0x80041000,  //  0099  RET	1	R8
      0x8C200B2F,  //  009A  GETMET	R8	R5	K47
      0x7C200200,  //  009B  CALL	R8	1
      0x5C241000,  //  009C  MOVE	R9	R8
      0x74260002,  //  009D  JMPT	R9	#00A1
      0x50240000,  //  009E  LDBOOL	R9	0	0
      0xA8040001,  //  009F  EXBLK	1	1
      0x80041200,  //  00A0  RET	1	R9
      0x8C240B20,  //  00A1  GETMET	R9	R5	K32
      0x7C240200,  //  00A2  CALL	R9	1
      0x88240100,  //  00A3  GETMBR	R9	R0	K0
      0x8C241321,  //  00A4  GETMET	R9	R9	K33
      0x5C2C0A00,  //  00A5  MOVE	R11	R5
      0x7C240400,  //  00A6  CALL	R9	2
      0x88240B30,  //  00A7  GETMBR	R9	R5	K48
      0x1C281316,  //  00A8  EQ	R10	R9	K22
      0x782A000F,  //  00A9  JMPF	R10	#00BA
      0x88280B22,  //  00AA  GETMBR	R10	R5	K34
      0x542E000F,  //  00AB  LDINT	R11	16
      0x1C28140B,  //  00AC  EQ	R10	R10	R11
      0x782A0009,  //  00AD  JMPF	R10	#00B8
      0x88280104,  //  00AE  GETMBR	R10	R0	K4
      0x8C281531,  //  00AF  GETMET	R10	R10	K49
      0x5C300A00,  //  00B0  MOVE	R12	R5
      0x7C280400,  //  00B1  CALL	R10	2
      0x5C101400,  //  00B2  MOVE	R4	R10
      0x78120003,  //  00B3  JMPF	R4	#00B8
      0x88280104,  //  00B4  GETMBR	R10	R0	K4
      0x8C281532,  //  00B5  GETMET	R10	R10	K50
      0x5C300000,  //  00B6  MOVE	R12	R0
      0x7C280400,  //  00B7  CALL	R10	2
      0x50100200,  //  00B8  LDBOOL	R4	1	0
      0x70020019,  //  00B9  JMP		#00D4
      0x1C281333,  //  00BA  EQ	R10	R9	K51
      0x782A0010,  //  00BB  JMPF	R10	#00CD
      0x88280104,  //  00BC  GETMBR	R10	R0	K4
      0x8C28152A,  //  00BD  GETMET	R10	R10	K42
      0x5C300A00,  //  00BE  MOVE	R12	R5
      0x7C280400,  //  00BF  CALL	R10	2
      0x5C101400,  //  00C0  MOVE	R4	R10
      0x78120004,  //  00C1  JMPF	R4	#00C7
      0x88280104,  //  00C2  GETMBR	R10	R0	K4
      0x8C281532,  //  00C3  GETMET	R10	R10	K50
      0x5C300000,  //  00C4  MOVE	R12	R0
      0x7C280400,  //  00C5  CALL	R10	2
      0x70020003,  //  00C6  JMP		#00CB
      0x8C28012E,  //  00C7  GETMET	R10	R0	K46
      0x5C300A00,  //  00C8  MOVE	R12	R5
      0x50340200,  //  00C9  LDBOOL	R13	1	0
      0x7C280600,  //  00CA  CALL	R10	3
      0x50100200,  //  00CB  LDBOOL	R4	1	0
      0x70020006,  //  00CC  JMP		#00D4
      0xB82A5000,  //  00CD  GETNGBL	R10	K40
      0x602C0008,  //  00CE  GETGBL	R11	G8
      0x5C301200,  //  00CF  MOVE	R12	R9
      0x7C2C0200,  //  00D0  CALL	R11	1
      0x002E680B,  //  00D1  ADD	R11	K52	R11
      0x58300027,  //  00D2  LDCONST	R12	K39
      0x7C280400,  //  00D3  CALL	R10	2
      0xA8040001,  //  00D4  EXBLK	1	1
      0x80040800,  //  00D5  RET	1	R4
      0xA8040001,  //  00D6  EXBLK	1	1
      0x70020017,  //  00D7  JMP		#00F0
      0xAC140002,  //  00D8  CATCH	R5	0	2
      0x70020014,  //  00D9  JMP		#00EF
      0xB81E5000,  //  00DA  GETNGBL	R7	K40
      0x60200008,  //  00DB  GETGBL	R8	G8
      0x5C240A00,  //  00DC  MOVE	R9	R5
      0x7C200200,  //  00DD  CALL	R8	1
      0x00226A08,  //  00DE  ADD	R8	K53	R8
      0x00201136,  //  00DF  ADD	R8	R8	K54
      0x60240008,  //  00E0  GETGBL	R9	G8
      0x5C280C00,  //  00E1  MOVE	R10	R6
      0x7C240200,  //  00E2  CALL	R9	1
      0x00201009,  //  00E3  ADD	R8	R8	R9
      0x58240037,  //  00E4  LDCONST	R9	K55
      0x7C1C0400,  //  00E5  CALL	R7	2
      0xB81E4A00,  //  00E6  GETNGBL	R7	K37
      0x881C0F38,  //  00E7  GETMBR	R7	R7	K56
      0x781E0002,  //  00E8  JMPF	R7	#00EC
      0xA41E7200,  //  00E9  IMPORT	R7	K57
      0x8C200F3A,  //  00EA  GETMET	R8	R7	K58
      0x7C200200,  //  00EB  CALL	R8	1
      0x501C0000,  //  00EC  LDBOOL	R7	0	0
      0x80040E00,  //  00ED  RET	1	R7
      0x70020000,  //  00EE  JMP		#00F0
      0xB0080000,  //  00EF  RAISE	2	R0	R0
      0x80000000,  //  00F0  RET	0
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
      0x8C08053B,  //  0001  GETMET	R2	R2	K59
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
    7,                          /* nstack */
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
    ( &(const binstruction[13]) {  /* code */
      0x880C0309,  //  0000  GETMBR	R3	R1	K9
      0x780E0009,  //  0001  JMPF	R3	#000C
      0x8C0C030A,  //  0002  GETMET	R3	R1	K10
      0x5C140400,  //  0003  MOVE	R5	R2
      0x7C0C0400,  //  0004  CALL	R3	2
      0x88100315,  //  0005  GETMBR	R4	R1	K21
      0x900E2A04,  //  0006  SETMBR	R3	K21	R4
      0x8C10070B,  //  0007  GETMET	R4	R3	K11
      0x7C100200,  //  0008  CALL	R4	1
      0x8C10010D,  //  0009  GETMET	R4	R0	K13
      0x5C180600,  //  000A  MOVE	R6	R3
      0x7C100400,  //  000B  CALL	R4	2
      0x80000000,  //  000C  RET	0
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
      0x8C04033C,  //  0001  GETMET	R1	R1	K60
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
      0x8C04033D,  //  0001  GETMET	R1	R1	K61
      0x7C040200,  //  0002  CALL	R1	1
      0x88040104,  //  0003  GETMBR	R1	R0	K4
      0x8C04033D,  //  0004  GETMET	R1	R1	K61
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
