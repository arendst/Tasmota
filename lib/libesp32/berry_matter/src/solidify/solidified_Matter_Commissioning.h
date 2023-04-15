/* Solidification of Matter_Commissioning.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Commisioning_Context;

/********************************************************************
** Solidified function: parse_StatusReport
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_StatusReport,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20),
    /* K4   */  be_nested_str_weak(raw),
    /* K5   */  be_nested_str_weak(app_payload_idx),
    /* K6   */  be_const_int(2147483647),
    /* K7   */  be_nested_str_weak(tohex),
    /* K8   */  be_const_int(2),
    }),
    be_str_weak(parse_StatusReport),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x88140305,  //  0003  GETMBR	R5	R1	K5
      0x40140B06,  //  0004  CONNECT	R5	R5	K6
      0x88180304,  //  0005  GETMBR	R6	R1	K4
      0x94140C05,  //  0006  GETIDX	R5	R6	R5
      0x8C140B07,  //  0007  GETMET	R5	R5	K7
      0x7C140200,  //  0008  CALL	R5	1
      0x00160605,  //  0009  ADD	R5	K3	R5
      0x58180008,  //  000A  LDCONST	R6	K8
      0x7C0C0600,  //  000B  CALL	R3	3
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0x80040600,  //  000D  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_fabric_by_destination_id
********************************************************************/
be_local_closure(Matter_Commisioning_Context_find_fabric_by_destination_id,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20SEARCHING_X3A_X20destinationId_X3D),
    /* K4   */  be_nested_str_weak(tohex),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(sessions),
    /* K8   */  be_nested_str_weak(fabrics),
    /* K9   */  be_nested_str_weak(noc),
    /* K10  */  be_nested_str_weak(fabric_id),
    /* K11  */  be_nested_str_weak(device_id),
    /* K12  */  be_nested_str_weak(get_ca_pub),
    /* K13  */  be_nested_str_weak(get_ipk_group_key),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20destinationMessage_X3D),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20key_ipk_X3D),
    /* K16  */  be_nested_str_weak(HMAC_SHA256),
    /* K17  */  be_nested_str_weak(update),
    /* K18  */  be_nested_str_weak(out),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20candidateDestinationId_X3D),
    /* K20  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(find_fabric_by_destination_id),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x8C180304,  //  0003  GETMET	R6	R1	K4
      0x7C180200,  //  0004  CALL	R6	1
      0x001A0606,  //  0005  ADD	R6	K3	R6
      0x581C0005,  //  0006  LDCONST	R7	K5
      0x7C100600,  //  0007  CALL	R4	3
      0x60100010,  //  0008  GETGBL	R4	G16
      0x88140106,  //  0009  GETMBR	R5	R0	K6
      0x88140B07,  //  000A  GETMBR	R5	R5	K7
      0x88140B08,  //  000B  GETMBR	R5	R5	K8
      0x7C100200,  //  000C  CALL	R4	1
      0xA8020039,  //  000D  EXBLK	0	#0048
      0x5C140800,  //  000E  MOVE	R5	R4
      0x7C140000,  //  000F  CALL	R5	0
      0x88180B09,  //  0010  GETMBR	R6	R5	K9
      0x4C1C0000,  //  0011  LDNIL	R7
      0x1C180C07,  //  0012  EQ	R6	R6	R7
      0x741A0007,  //  0013  JMPT	R6	#001C
      0x88180B0A,  //  0014  GETMBR	R6	R5	K10
      0x4C1C0000,  //  0015  LDNIL	R7
      0x1C180C07,  //  0016  EQ	R6	R6	R7
      0x741A0003,  //  0017  JMPT	R6	#001C
      0x88180B0B,  //  0018  GETMBR	R6	R5	K11
      0x4C1C0000,  //  0019  LDNIL	R7
      0x1C180C07,  //  001A  EQ	R6	R6	R7
      0x781A0000,  //  001B  JMPF	R6	#001D
      0x7001FFF0,  //  001C  JMP		#000E
      0x8C180B0C,  //  001D  GETMET	R6	R5	K12
      0x7C180200,  //  001E  CALL	R6	1
      0x00180406,  //  001F  ADD	R6	R2	R6
      0x881C0B0A,  //  0020  GETMBR	R7	R5	K10
      0x00180C07,  //  0021  ADD	R6	R6	R7
      0x881C0B0B,  //  0022  GETMBR	R7	R5	K11
      0x00180C07,  //  0023  ADD	R6	R6	R7
      0x8C1C0B0D,  //  0024  GETMET	R7	R5	K13
      0x7C1C0200,  //  0025  CALL	R7	1
      0xB8220200,  //  0026  GETNGBL	R8	K1
      0x8C201102,  //  0027  GETMET	R8	R8	K2
      0x8C280D04,  //  0028  GETMET	R10	R6	K4
      0x7C280200,  //  0029  CALL	R10	1
      0x002A1C0A,  //  002A  ADD	R10	K14	R10
      0x582C0005,  //  002B  LDCONST	R11	K5
      0x7C200600,  //  002C  CALL	R8	3
      0xB8220200,  //  002D  GETNGBL	R8	K1
      0x8C201102,  //  002E  GETMET	R8	R8	K2
      0x8C280F04,  //  002F  GETMET	R10	R7	K4
      0x7C280200,  //  0030  CALL	R10	1
      0x002A1E0A,  //  0031  ADD	R10	K15	R10
      0x542E0003,  //  0032  LDINT	R11	4
      0x7C200600,  //  0033  CALL	R8	3
      0x8C200710,  //  0034  GETMET	R8	R3	K16
      0x5C280E00,  //  0035  MOVE	R10	R7
      0x7C200400,  //  0036  CALL	R8	2
      0x8C241111,  //  0037  GETMET	R9	R8	K17
      0x5C2C0C00,  //  0038  MOVE	R11	R6
      0x7C240400,  //  0039  CALL	R9	2
      0x8C241112,  //  003A  GETMET	R9	R8	K18
      0x7C240200,  //  003B  CALL	R9	1
      0xB82A0200,  //  003C  GETNGBL	R10	K1
      0x8C281502,  //  003D  GETMET	R10	R10	K2
      0x8C301304,  //  003E  GETMET	R12	R9	K4
      0x7C300200,  //  003F  CALL	R12	1
      0x0032260C,  //  0040  ADD	R12	K19	R12
      0x58340005,  //  0041  LDCONST	R13	K5
      0x7C280600,  //  0042  CALL	R10	3
      0x1C281201,  //  0043  EQ	R10	R9	R1
      0x782A0001,  //  0044  JMPF	R10	#0047
      0xA8040001,  //  0045  EXBLK	1	1
      0x80040A00,  //  0046  RET	1	R5
      0x7001FFC5,  //  0047  JMP		#000E
      0x58100014,  //  0048  LDCONST	R4	K20
      0xAC100200,  //  0049  CATCH	R4	1	0
      0xB0080000,  //  004A  RAISE	2	R0	R0
      0x4C100000,  //  004B  LDNIL	R4
      0x80040800,  //  004C  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Commisioning_Context_every_second,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_session
********************************************************************/
be_local_closure(Matter_Commisioning_Context_add_session,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20add_session_X20local_session_id_X3D_X25i_X20initiator_session_id_X3D_X25i),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(sessions),
    /* K8   */  be_nested_str_weak(create_session),
    /* K9   */  be_nested_str_weak(set_keys),
    }),
    be_str_weak(add_session),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA41A0000,  //  0000  IMPORT	R6	K0
      0xB81E0200,  //  0001  GETNGBL	R7	K1
      0x8C1C0F02,  //  0002  GETMET	R7	R7	K2
      0x8C240D03,  //  0003  GETMET	R9	R6	K3
      0x582C0004,  //  0004  LDCONST	R11	K4
      0x5C300200,  //  0005  MOVE	R12	R1
      0x5C340400,  //  0006  MOVE	R13	R2
      0x7C240800,  //  0007  CALL	R9	4
      0x58280005,  //  0008  LDCONST	R10	K5
      0x7C1C0600,  //  0009  CALL	R7	3
      0x881C0106,  //  000A  GETMBR	R7	R0	K6
      0x881C0F07,  //  000B  GETMBR	R7	R7	K7
      0x8C1C0F08,  //  000C  GETMET	R7	R7	K8
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280400,  //  000E  MOVE	R10	R2
      0x7C1C0600,  //  000F  CALL	R7	3
      0x8C200F09,  //  0010  GETMET	R8	R7	K9
      0x5C280600,  //  0011  MOVE	R10	R3
      0x5C2C0800,  //  0012  MOVE	R11	R4
      0x5C300A00,  //  0013  MOVE	R12	R5
      0x7C200800,  //  0014  CALL	R8	4
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_PBKDFParamRequest
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_PBKDFParamRequest,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[49]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(session),
    /* K3   */  be_nested_str_weak(opcode),
    /* K4   */  be_nested_str_weak(local_session_id),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(protocol_id),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20invalid_X20PBKDFParamRequest_X20message),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K12  */  be_nested_str_weak(send_status_report),
    /* K13  */  be_const_int(1),
    /* K14  */  be_nested_str_weak(matter),
    /* K15  */  be_nested_str_weak(PBKDFParamRequest),
    /* K16  */  be_nested_str_weak(parse),
    /* K17  */  be_nested_str_weak(raw),
    /* K18  */  be_nested_str_weak(app_payload_idx),
    /* K19  */  be_nested_str_weak(set_mode_PASE),
    /* K20  */  be_nested_str_weak(__Msg1),
    /* K21  */  be_const_int(2147483647),
    /* K22  */  be_nested_str_weak(passcodeId),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20non_X2Dzero_X20passcode_X20id),
    /* K24  */  be_nested_str_weak(__future_initiator_session_id),
    /* K25  */  be_nested_str_weak(initiator_session_id),
    /* K26  */  be_nested_str_weak(__future_local_session_id),
    /* K27  */  be_nested_str_weak(device),
    /* K28  */  be_nested_str_weak(sessions),
    /* K29  */  be_nested_str_weak(gen_local_session_id),
    /* K30  */  be_nested_str_weak(format),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K32  */  be_nested_str_weak(remote_ip),
    /* K33  */  be_nested_str_weak(remote_port),
    /* K34  */  be_nested_str_weak(PBKDFParamResponse),
    /* K35  */  be_nested_str_weak(initiatorRandom),
    /* K36  */  be_nested_str_weak(responderRandom),
    /* K37  */  be_nested_str_weak(random),
    /* K38  */  be_nested_str_weak(responderSessionId),
    /* K39  */  be_nested_str_weak(pbkdf_parameters_salt),
    /* K40  */  be_nested_str_weak(commissioning_salt),
    /* K41  */  be_nested_str_weak(pbkdf_parameters_iterations),
    /* K42  */  be_nested_str_weak(commissioning_iterations),
    /* K43  */  be_nested_str_weak(tlv2raw),
    /* K44  */  be_nested_str_weak(__Msg2),
    /* K45  */  be_nested_str_weak(build_response),
    /* K46  */  be_nested_str_weak(encode_frame),
    /* K47  */  be_nested_str_weak(responder),
    /* K48  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[119]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x88100302,  //  0002  GETMBR	R4	R1	K2
      0x88140303,  //  0003  GETMBR	R5	R1	K3
      0x541A001F,  //  0004  LDINT	R6	32
      0x20140A06,  //  0005  NE	R5	R5	R6
      0x74160005,  //  0006  JMPT	R5	#000D
      0x88140304,  //  0007  GETMBR	R5	R1	K4
      0x20140B05,  //  0008  NE	R5	R5	K5
      0x74160002,  //  0009  JMPT	R5	#000D
      0x88140306,  //  000A  GETMBR	R5	R1	K6
      0x20140B05,  //  000B  NE	R5	R5	K5
      0x78160012,  //  000C  JMPF	R5	#0020
      0xB8160E00,  //  000D  GETNGBL	R5	K7
      0x8C140B08,  //  000E  GETMET	R5	R5	K8
      0x581C0009,  //  000F  LDCONST	R7	K9
      0x5820000A,  //  0010  LDCONST	R8	K10
      0x7C140600,  //  0011  CALL	R5	3
      0xB8160E00,  //  0012  GETNGBL	R5	K7
      0x8C140B08,  //  0013  GETMET	R5	R5	K8
      0x581C000B,  //  0014  LDCONST	R7	K11
      0x5820000A,  //  0015  LDCONST	R8	K10
      0x7C140600,  //  0016  CALL	R5	3
      0x8C14010C,  //  0017  GETMET	R5	R0	K12
      0x5C1C0200,  //  0018  MOVE	R7	R1
      0x5820000D,  //  0019  LDCONST	R8	K13
      0x58240005,  //  001A  LDCONST	R9	K5
      0x5828000A,  //  001B  LDCONST	R10	K10
      0x502C0000,  //  001C  LDBOOL	R11	0	0
      0x7C140C00,  //  001D  CALL	R5	6
      0x50180000,  //  001E  LDBOOL	R6	0	0
      0x80040C00,  //  001F  RET	1	R6
      0xB8161C00,  //  0020  GETNGBL	R5	K14
      0x8C140B0F,  //  0021  GETMET	R5	R5	K15
      0x7C140200,  //  0022  CALL	R5	1
      0x8C140B10,  //  0023  GETMET	R5	R5	K16
      0x881C0311,  //  0024  GETMBR	R7	R1	K17
      0x88200312,  //  0025  GETMBR	R8	R1	K18
      0x7C140600,  //  0026  CALL	R5	3
      0x88180302,  //  0027  GETMBR	R6	R1	K2
      0x8C180D13,  //  0028  GETMET	R6	R6	K19
      0x7C180200,  //  0029  CALL	R6	1
      0x88180312,  //  002A  GETMBR	R6	R1	K18
      0x40180D15,  //  002B  CONNECT	R6	R6	K21
      0x881C0311,  //  002C  GETMBR	R7	R1	K17
      0x94180E06,  //  002D  GETIDX	R6	R7	R6
      0x90122806,  //  002E  SETMBR	R4	K20	R6
      0x88180B16,  //  002F  GETMBR	R6	R5	K22
      0x20180D05,  //  0030  NE	R6	R6	K5
      0x781A0012,  //  0031  JMPF	R6	#0045
      0xB81A0E00,  //  0032  GETNGBL	R6	K7
      0x8C180D08,  //  0033  GETMET	R6	R6	K8
      0x58200017,  //  0034  LDCONST	R8	K23
      0x5824000A,  //  0035  LDCONST	R9	K10
      0x7C180600,  //  0036  CALL	R6	3
      0xB81A0E00,  //  0037  GETNGBL	R6	K7
      0x8C180D08,  //  0038  GETMET	R6	R6	K8
      0x5820000B,  //  0039  LDCONST	R8	K11
      0x5824000A,  //  003A  LDCONST	R9	K10
      0x7C180600,  //  003B  CALL	R6	3
      0x8C18010C,  //  003C  GETMET	R6	R0	K12
      0x5C200200,  //  003D  MOVE	R8	R1
      0x5824000D,  //  003E  LDCONST	R9	K13
      0x58280005,  //  003F  LDCONST	R10	K5
      0x582C000A,  //  0040  LDCONST	R11	K10
      0x50300000,  //  0041  LDBOOL	R12	0	0
      0x7C180C00,  //  0042  CALL	R6	6
      0x501C0000,  //  0043  LDBOOL	R7	0	0
      0x80040E00,  //  0044  RET	1	R7
      0x88180B19,  //  0045  GETMBR	R6	R5	K25
      0x90123006,  //  0046  SETMBR	R4	K24	R6
      0x8818011B,  //  0047  GETMBR	R6	R0	K27
      0x88180D1C,  //  0048  GETMBR	R6	R6	K28
      0x8C180D1D,  //  0049  GETMET	R6	R6	K29
      0x7C180200,  //  004A  CALL	R6	1
      0x90123406,  //  004B  SETMBR	R4	K26	R6
      0xB81A0E00,  //  004C  GETNGBL	R6	K7
      0x8C180D08,  //  004D  GETMET	R6	R6	K8
      0x8C20071E,  //  004E  GETMET	R8	R3	K30
      0x5828001F,  //  004F  LDCONST	R10	K31
      0x882C091A,  //  0050  GETMBR	R11	R4	K26
      0x88300320,  //  0051  GETMBR	R12	R1	K32
      0x88340321,  //  0052  GETMBR	R13	R1	K33
      0x7C200A00,  //  0053  CALL	R8	5
      0x5824000A,  //  0054  LDCONST	R9	K10
      0x7C180600,  //  0055  CALL	R6	3
      0xB81A1C00,  //  0056  GETNGBL	R6	K14
      0x8C180D22,  //  0057  GETMET	R6	R6	K34
      0x7C180200,  //  0058  CALL	R6	1
      0x881C0B23,  //  0059  GETMBR	R7	R5	K35
      0x901A4607,  //  005A  SETMBR	R6	K35	R7
      0x8C1C0525,  //  005B  GETMET	R7	R2	K37
      0x5426001F,  //  005C  LDINT	R9	32
      0x7C1C0400,  //  005D  CALL	R7	2
      0x901A4807,  //  005E  SETMBR	R6	K36	R7
      0x881C091A,  //  005F  GETMBR	R7	R4	K26
      0x901A4C07,  //  0060  SETMBR	R6	K38	R7
      0x881C011B,  //  0061  GETMBR	R7	R0	K27
      0x881C0F28,  //  0062  GETMBR	R7	R7	K40
      0x901A4E07,  //  0063  SETMBR	R6	K39	R7
      0x881C011B,  //  0064  GETMBR	R7	R0	K27
      0x881C0F2A,  //  0065  GETMBR	R7	R7	K42
      0x901A5207,  //  0066  SETMBR	R6	K41	R7
      0x8C1C0D2B,  //  0067  GETMET	R7	R6	K43
      0x7C1C0200,  //  0068  CALL	R7	1
      0x90125807,  //  0069  SETMBR	R4	K44	R7
      0x8C20032D,  //  006A  GETMET	R8	R1	K45
      0x542A0020,  //  006B  LDINT	R10	33
      0x502C0200,  //  006C  LDBOOL	R11	1	0
      0x7C200600,  //  006D  CALL	R8	3
      0x8C24112E,  //  006E  GETMET	R9	R8	K46
      0x5C2C0E00,  //  006F  MOVE	R11	R7
      0x7C240400,  //  0070  CALL	R9	2
      0x8828012F,  //  0071  GETMBR	R10	R0	K47
      0x8C281530,  //  0072  GETMET	R10	R10	K48
      0x5C301000,  //  0073  MOVE	R12	R8
      0x7C280400,  //  0074  CALL	R10	2
      0x50280200,  //  0075  LDBOOL	R10	1	0
      0x80041400,  //  0076  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_status_report
********************************************************************/
be_local_closure(Matter_Commisioning_Context_send_status_report,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(build_response),
    /* K1   */  be_nested_str_weak(add),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str_weak(encode_frame),
    /* K4   */  be_nested_str_weak(responder),
    /* K5   */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(send_status_report),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8C180300,  //  0000  GETMET	R6	R1	K0
      0x5422003F,  //  0001  LDINT	R8	64
      0x5C240A00,  //  0002  MOVE	R9	R5
      0x7C180600,  //  0003  CALL	R6	3
      0x601C0015,  //  0004  GETGBL	R7	G21
      0x7C1C0000,  //  0005  CALL	R7	0
      0x8C200F01,  //  0006  GETMET	R8	R7	K1
      0x5C280400,  //  0007  MOVE	R10	R2
      0x582C0002,  //  0008  LDCONST	R11	K2
      0x7C200600,  //  0009  CALL	R8	3
      0x8C200F01,  //  000A  GETMET	R8	R7	K1
      0x5C280600,  //  000B  MOVE	R10	R3
      0x542E0003,  //  000C  LDINT	R11	4
      0x7C200600,  //  000D  CALL	R8	3
      0x8C200F01,  //  000E  GETMET	R8	R7	K1
      0x5C280800,  //  000F  MOVE	R10	R4
      0x542E0003,  //  0010  LDINT	R11	4
      0x7C200600,  //  0011  CALL	R8	3
      0x8C200D03,  //  0012  GETMET	R8	R6	K3
      0x5C280E00,  //  0013  MOVE	R10	R7
      0x7C200400,  //  0014  CALL	R8	2
      0x88240104,  //  0015  GETMBR	R9	R0	K4
      0x8C241305,  //  0016  GETMET	R9	R9	K5
      0x5C2C0C00,  //  0017  MOVE	R11	R6
      0x7C240400,  //  0018  CALL	R9	2
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Pake1
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Pake1,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[47]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake1_X20message),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K11  */  be_nested_str_weak(send_status_report),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(Pake1),
    /* K15  */  be_nested_str_weak(parse),
    /* K16  */  be_nested_str_weak(raw),
    /* K17  */  be_nested_str_weak(app_payload_idx),
    /* K18  */  be_nested_str_weak(pA),
    /* K19  */  be_nested_str_weak(SPAKE2P_Matter),
    /* K20  */  be_nested_str_weak(device),
    /* K21  */  be_nested_str_weak(commissioning_w0),
    /* K22  */  be_nested_str_weak(commissioning_L),
    /* K23  */  be_nested_str_weak(random),
    /* K24  */  be_nested_str_weak(compute_pB),
    /* K25  */  be_nested_str_weak(compute_ZV_verifier),
    /* K26  */  be_nested_str_weak(SHA256),
    /* K27  */  be_nested_str_weak(update),
    /* K28  */  be_nested_str_weak(fromstring),
    /* K29  */  be_nested_str_weak(Matter_Context_Prefix),
    /* K30  */  be_nested_str_weak(__Msg1),
    /* K31  */  be_nested_str_weak(__Msg2),
    /* K32  */  be_nested_str_weak(out),
    /* K33  */  be_nested_str_weak(set_context),
    /* K34  */  be_nested_str_weak(compute_TT_hash),
    /* K35  */  be_nested_str_weak(Pake2),
    /* K36  */  be_nested_str_weak(pB),
    /* K37  */  be_nested_str_weak(cB),
    /* K38  */  be_nested_str_weak(tlv2raw),
    /* K39  */  be_nested_str_weak(__spake_cA),
    /* K40  */  be_nested_str_weak(cA),
    /* K41  */  be_nested_str_weak(__spake_Ke),
    /* K42  */  be_nested_str_weak(Ke),
    /* K43  */  be_nested_str_weak(build_response),
    /* K44  */  be_nested_str_weak(encode_frame),
    /* K45  */  be_nested_str_weak(responder),
    /* K46  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x88100302,  //  0002  GETMBR	R4	R1	K2
      0x54160021,  //  0003  LDINT	R5	34
      0x20100805,  //  0004  NE	R4	R4	R5
      0x74120005,  //  0005  JMPT	R4	#000C
      0x88100303,  //  0006  GETMBR	R4	R1	K3
      0x20100904,  //  0007  NE	R4	R4	K4
      0x74120002,  //  0008  JMPT	R4	#000C
      0x88100305,  //  0009  GETMBR	R4	R1	K5
      0x20100904,  //  000A  NE	R4	R4	K4
      0x78120012,  //  000B  JMPF	R4	#001F
      0xB8120C00,  //  000C  GETNGBL	R4	K6
      0x8C100907,  //  000D  GETMET	R4	R4	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x581C0009,  //  000F  LDCONST	R7	K9
      0x7C100600,  //  0010  CALL	R4	3
      0xB8120C00,  //  0011  GETNGBL	R4	K6
      0x8C100907,  //  0012  GETMET	R4	R4	K7
      0x5818000A,  //  0013  LDCONST	R6	K10
      0x581C0009,  //  0014  LDCONST	R7	K9
      0x7C100600,  //  0015  CALL	R4	3
      0x8C10010B,  //  0016  GETMET	R4	R0	K11
      0x5C180200,  //  0017  MOVE	R6	R1
      0x581C000C,  //  0018  LDCONST	R7	K12
      0x58200004,  //  0019  LDCONST	R8	K4
      0x58240009,  //  001A  LDCONST	R9	K9
      0x50280000,  //  001B  LDBOOL	R10	0	0
      0x7C100C00,  //  001C  CALL	R4	6
      0x50140000,  //  001D  LDBOOL	R5	0	0
      0x80040A00,  //  001E  RET	1	R5
      0xB8121A00,  //  001F  GETNGBL	R4	K13
      0x8C10090E,  //  0020  GETMET	R4	R4	K14
      0x7C100200,  //  0021  CALL	R4	1
      0x8C10090F,  //  0022  GETMET	R4	R4	K15
      0x88180310,  //  0023  GETMBR	R6	R1	K16
      0x881C0311,  //  0024  GETMBR	R7	R1	K17
      0x7C100600,  //  0025  CALL	R4	3
      0x88140912,  //  0026  GETMBR	R5	R4	K18
      0x8C180513,  //  0027  GETMET	R6	R2	K19
      0x88200114,  //  0028  GETMBR	R8	R0	K20
      0x88201115,  //  0029  GETMBR	R8	R8	K21
      0x4C240000,  //  002A  LDNIL	R9
      0x88280114,  //  002B  GETMBR	R10	R0	K20
      0x88281516,  //  002C  GETMBR	R10	R10	K22
      0x7C180800,  //  002D  CALL	R6	4
      0x8C1C0517,  //  002E  GETMET	R7	R2	K23
      0x5426001F,  //  002F  LDINT	R9	32
      0x7C1C0400,  //  0030  CALL	R7	2
      0x8C200D18,  //  0031  GETMET	R8	R6	K24
      0x5C280E00,  //  0032  MOVE	R10	R7
      0x7C200400,  //  0033  CALL	R8	2
      0x8C200D19,  //  0034  GETMET	R8	R6	K25
      0x5C280A00,  //  0035  MOVE	R10	R5
      0x7C200400,  //  0036  CALL	R8	2
      0x8C20051A,  //  0037  GETMET	R8	R2	K26
      0x7C200200,  //  0038  CALL	R8	1
      0x8C24111B,  //  0039  GETMET	R9	R8	K27
      0x602C0015,  //  003A  GETGBL	R11	G21
      0x7C2C0000,  //  003B  CALL	R11	0
      0x8C2C171C,  //  003C  GETMET	R11	R11	K28
      0x8834011D,  //  003D  GETMBR	R13	R0	K29
      0x7C2C0400,  //  003E  CALL	R11	2
      0x7C240400,  //  003F  CALL	R9	2
      0x8C24111B,  //  0040  GETMET	R9	R8	K27
      0x882C071E,  //  0041  GETMBR	R11	R3	K30
      0x7C240400,  //  0042  CALL	R9	2
      0x8C24111B,  //  0043  GETMET	R9	R8	K27
      0x882C071F,  //  0044  GETMBR	R11	R3	K31
      0x7C240400,  //  0045  CALL	R9	2
      0x8C241120,  //  0046  GETMET	R9	R8	K32
      0x7C240200,  //  0047  CALL	R9	1
      0x901A2405,  //  0048  SETMBR	R6	K18	R5
      0x8C280D21,  //  0049  GETMET	R10	R6	K33
      0x5C301200,  //  004A  MOVE	R12	R9
      0x7C280400,  //  004B  CALL	R10	2
      0x8C280D22,  //  004C  GETMET	R10	R6	K34
      0x50300200,  //  004D  LDBOOL	R12	1	0
      0x7C280400,  //  004E  CALL	R10	2
      0xB82A1A00,  //  004F  GETNGBL	R10	K13
      0x8C281523,  //  0050  GETMET	R10	R10	K35
      0x7C280200,  //  0051  CALL	R10	1
      0x882C0D24,  //  0052  GETMBR	R11	R6	K36
      0x902A480B,  //  0053  SETMBR	R10	K36	R11
      0x882C0D25,  //  0054  GETMBR	R11	R6	K37
      0x902A4A0B,  //  0055  SETMBR	R10	K37	R11
      0x8C2C1526,  //  0056  GETMET	R11	R10	K38
      0x7C2C0200,  //  0057  CALL	R11	1
      0x88300D28,  //  0058  GETMBR	R12	R6	K40
      0x900E4E0C,  //  0059  SETMBR	R3	K39	R12
      0x88300D2A,  //  005A  GETMBR	R12	R6	K42
      0x900E520C,  //  005B  SETMBR	R3	K41	R12
      0x8C30032B,  //  005C  GETMET	R12	R1	K43
      0x543A0022,  //  005D  LDINT	R14	35
      0x503C0200,  //  005E  LDBOOL	R15	1	0
      0x7C300600,  //  005F  CALL	R12	3
      0x8C34192C,  //  0060  GETMET	R13	R12	K44
      0x5C3C1600,  //  0061  MOVE	R15	R11
      0x7C340400,  //  0062  CALL	R13	2
      0x8838012D,  //  0063  GETMBR	R14	R0	K45
      0x8C381D2E,  //  0064  GETMET	R14	R14	K46
      0x5C401800,  //  0065  MOVE	R16	R12
      0x7C380400,  //  0066  CALL	R14	2
      0x50380200,  //  0067  LDBOOL	R14	1	0
      0x80041C00,  //  0068  RET	1	R14
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Commisioning_Context_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(responder),
    /* K2   */  be_nested_str_weak(device),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x880C0302,  //  0002  GETMBR	R3	R1	K2
      0x90020403,  //  0003  SETMBR	R0	K2	R3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Pake3
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Pake3,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[31]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake3_X20message),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K11  */  be_nested_str_weak(send_status_report),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(Pake3),
    /* K15  */  be_nested_str_weak(parse),
    /* K16  */  be_nested_str_weak(raw),
    /* K17  */  be_nested_str_weak(app_payload_idx),
    /* K18  */  be_nested_str_weak(cA),
    /* K19  */  be_nested_str_weak(__spake_cA),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20invalid_X20cA_X20received),
    /* K21  */  be_nested_str_weak(rtc),
    /* K22  */  be_nested_str_weak(utc),
    /* K23  */  be_nested_str_weak(HKDF_SHA256),
    /* K24  */  be_nested_str_weak(derive),
    /* K25  */  be_nested_str_weak(__spake_Ke),
    /* K26  */  be_nested_str_weak(fromstring),
    /* K27  */  be_nested_str_weak(SEKeys_Info),
    /* K28  */  be_nested_str_weak(add_session),
    /* K29  */  be_nested_str_weak(__future_local_session_id),
    /* K30  */  be_nested_str_weak(__future_initiator_session_id),
    }),
    be_str_weak(parse_Pake3),
    &be_const_str_solidified,
    ( &(const binstruction[106]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x88100302,  //  0002  GETMBR	R4	R1	K2
      0x54160023,  //  0003  LDINT	R5	36
      0x20100805,  //  0004  NE	R4	R4	R5
      0x74120005,  //  0005  JMPT	R4	#000C
      0x88100303,  //  0006  GETMBR	R4	R1	K3
      0x20100904,  //  0007  NE	R4	R4	K4
      0x74120002,  //  0008  JMPT	R4	#000C
      0x88100305,  //  0009  GETMBR	R4	R1	K5
      0x20100904,  //  000A  NE	R4	R4	K4
      0x78120012,  //  000B  JMPF	R4	#001F
      0xB8120C00,  //  000C  GETNGBL	R4	K6
      0x8C100907,  //  000D  GETMET	R4	R4	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x581C0009,  //  000F  LDCONST	R7	K9
      0x7C100600,  //  0010  CALL	R4	3
      0xB8120C00,  //  0011  GETNGBL	R4	K6
      0x8C100907,  //  0012  GETMET	R4	R4	K7
      0x5818000A,  //  0013  LDCONST	R6	K10
      0x581C0009,  //  0014  LDCONST	R7	K9
      0x7C100600,  //  0015  CALL	R4	3
      0x8C10010B,  //  0016  GETMET	R4	R0	K11
      0x5C180200,  //  0017  MOVE	R6	R1
      0x581C000C,  //  0018  LDCONST	R7	K12
      0x58200004,  //  0019  LDCONST	R8	K4
      0x58240009,  //  001A  LDCONST	R9	K9
      0x50280000,  //  001B  LDBOOL	R10	0	0
      0x7C100C00,  //  001C  CALL	R4	6
      0x50140000,  //  001D  LDBOOL	R5	0	0
      0x80040A00,  //  001E  RET	1	R5
      0xB8121A00,  //  001F  GETNGBL	R4	K13
      0x8C10090E,  //  0020  GETMET	R4	R4	K14
      0x7C100200,  //  0021  CALL	R4	1
      0x8C10090F,  //  0022  GETMET	R4	R4	K15
      0x88180310,  //  0023  GETMBR	R6	R1	K16
      0x881C0311,  //  0024  GETMBR	R7	R1	K17
      0x7C100600,  //  0025  CALL	R4	3
      0x88140912,  //  0026  GETMBR	R5	R4	K18
      0x88180713,  //  0027  GETMBR	R6	R3	K19
      0x20180A06,  //  0028  NE	R6	R5	R6
      0x781A0012,  //  0029  JMPF	R6	#003D
      0xB81A0C00,  //  002A  GETNGBL	R6	K6
      0x8C180D07,  //  002B  GETMET	R6	R6	K7
      0x58200014,  //  002C  LDCONST	R8	K20
      0x58240009,  //  002D  LDCONST	R9	K9
      0x7C180600,  //  002E  CALL	R6	3
      0xB81A0C00,  //  002F  GETNGBL	R6	K6
      0x8C180D07,  //  0030  GETMET	R6	R6	K7
      0x5820000A,  //  0031  LDCONST	R8	K10
      0x58240009,  //  0032  LDCONST	R9	K9
      0x7C180600,  //  0033  CALL	R6	3
      0x8C18010B,  //  0034  GETMET	R6	R0	K11
      0x5C200200,  //  0035  MOVE	R8	R1
      0x5824000C,  //  0036  LDCONST	R9	K12
      0x58280004,  //  0037  LDCONST	R10	K4
      0x582C0009,  //  0038  LDCONST	R11	K9
      0x50300000,  //  0039  LDBOOL	R12	0	0
      0x7C180C00,  //  003A  CALL	R6	6
      0x501C0000,  //  003B  LDBOOL	R7	0	0
      0x80040E00,  //  003C  RET	1	R7
      0xB81A0C00,  //  003D  GETNGBL	R6	K6
      0x8C180D15,  //  003E  GETMET	R6	R6	K21
      0x7C180200,  //  003F  CALL	R6	1
      0x94180D16,  //  0040  GETIDX	R6	R6	K22
      0x8C1C0517,  //  0041  GETMET	R7	R2	K23
      0x7C1C0200,  //  0042  CALL	R7	1
      0x8C1C0F18,  //  0043  GETMET	R7	R7	K24
      0x88240719,  //  0044  GETMBR	R9	R3	K25
      0x60280015,  //  0045  GETGBL	R10	G21
      0x7C280000,  //  0046  CALL	R10	0
      0x602C0015,  //  0047  GETGBL	R11	G21
      0x7C2C0000,  //  0048  CALL	R11	0
      0x8C2C171A,  //  0049  GETMET	R11	R11	K26
      0x8834011B,  //  004A  GETMBR	R13	R0	K27
      0x7C2C0400,  //  004B  CALL	R11	2
      0x5432002F,  //  004C  LDINT	R12	48
      0x7C1C0A00,  //  004D  CALL	R7	5
      0x5422000E,  //  004E  LDINT	R8	15
      0x40220808,  //  004F  CONNECT	R8	K4	R8
      0x94200E08,  //  0050  GETIDX	R8	R7	R8
      0x5426000F,  //  0051  LDINT	R9	16
      0x542A001E,  //  0052  LDINT	R10	31
      0x4024120A,  //  0053  CONNECT	R9	R9	R10
      0x94240E09,  //  0054  GETIDX	R9	R7	R9
      0x542A001F,  //  0055  LDINT	R10	32
      0x542E002E,  //  0056  LDINT	R11	47
      0x4028140B,  //  0057  CONNECT	R10	R10	R11
      0x94280E0A,  //  0058  GETIDX	R10	R7	R10
      0x8C2C010B,  //  0059  GETMET	R11	R0	K11
      0x5C340200,  //  005A  MOVE	R13	R1
      0x58380004,  //  005B  LDCONST	R14	K4
      0x583C0004,  //  005C  LDCONST	R15	K4
      0x58400004,  //  005D  LDCONST	R16	K4
      0x50440000,  //  005E  LDBOOL	R17	0	0
      0x7C2C0C00,  //  005F  CALL	R11	6
      0x8C30011C,  //  0060  GETMET	R12	R0	K28
      0x8838071D,  //  0061  GETMBR	R14	R3	K29
      0x883C071E,  //  0062  GETMBR	R15	R3	K30
      0x5C401000,  //  0063  MOVE	R16	R8
      0x5C441200,  //  0064  MOVE	R17	R9
      0x5C481400,  //  0065  MOVE	R18	R10
      0x5C4C0C00,  //  0066  MOVE	R19	R6
      0x7C300E00,  //  0067  CALL	R12	7
      0x50300200,  //  0068  LDBOOL	R12	1	0
      0x80041800,  //  0069  RET	1	R12
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Sigma3
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Sigma3,   /* name */
  be_nested_proto(
    36,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[69]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(send_status_report),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(session),
    /* K12  */  be_nested_str_weak(matter),
    /* K13  */  be_nested_str_weak(Sigma3),
    /* K14  */  be_nested_str_weak(parse),
    /* K15  */  be_nested_str_weak(raw),
    /* K16  */  be_nested_str_weak(app_payload_idx),
    /* K17  */  be_nested_str_weak(SHA256),
    /* K18  */  be_nested_str_weak(update),
    /* K19  */  be_nested_str_weak(__Msg1),
    /* K20  */  be_nested_str_weak(__Msg2),
    /* K21  */  be_nested_str_weak(out),
    /* K22  */  be_nested_str_weak(fromstring),
    /* K23  */  be_nested_str_weak(S3K_Info),
    /* K24  */  be_nested_str_weak(HKDF_SHA256),
    /* K25  */  be_nested_str_weak(derive),
    /* K26  */  be_nested_str_weak(shared_secret),
    /* K27  */  be_nested_str_weak(get_ipk_group_key),
    /* K28  */  be_nested_str_weak(TBEData3Encrypted),
    /* K29  */  be_const_int(2147483647),
    /* K30  */  be_nested_str_weak(AES_CCM),
    /* K31  */  be_nested_str_weak(TBEData3_Nonce),
    /* K32  */  be_nested_str_weak(decrypt),
    /* K33  */  be_nested_str_weak(tag),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20Tag_X20don_X27t_X20match),
    /* K35  */  be_nested_str_weak(TLV),
    /* K36  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData3TLV_X20_X20_X20_X3D_X20),
    /* K37  */  be_nested_str_weak(findsubval),
    /* K38  */  be_const_int(3),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20initiatorNOCTLV_X20_X3D_X20),
    /* K40  */  be_nested_str_weak(findsub),
    /* K41  */  be_nested_str_weak(int),
    /* K42  */  be_nested_str_weak(peer_node_id),
    /* K43  */  be_nested_str_weak(int64),
    /* K44  */  be_nested_str_weak(fromu32),
    /* K45  */  be_nested_str_weak(tobytes),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20initiatorFabricId_X3D),
    /* K47  */  be_nested_str_weak(Matter_TLV_struct),
    /* K48  */  be_nested_str_weak(add_TLV),
    /* K49  */  be_nested_str_weak(B1),
    /* K50  */  be_nested_str_weak(__initiator_pub),
    /* K51  */  be_nested_str_weak(__responder_pub),
    /* K52  */  be_nested_str_weak(tlv2raw),
    /* K53  */  be_nested_str_weak(EC_P256),
    /* K54  */  be_nested_str_weak(ecdsa_verify_sha256),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20sigma3_tbs_X20does_X20not_X20have_X20a_X20valid_X20signature),
    /* K56  */  be_nested_str_weak(MTR_X3A_X20Sigma3_X20verified_X2C_X20computing_X20new_X20keys),
    /* K57  */  be_nested_str_weak(Msg3),
    /* K58  */  be_nested_str_weak(SEKeys_Info),
    /* K59  */  be_nested_str_weak(rtc),
    /* K60  */  be_nested_str_weak(utc),
    /* K61  */  be_nested_str_weak(close),
    /* K62  */  be_nested_str_weak(set_keys),
    /* K63  */  be_nested_str_weak(_breadcrumb),
    /* K64  */  be_nested_str_weak(counter_snd_next),
    /* K65  */  be_nested_str_weak(set_persist),
    /* K66  */  be_nested_str_weak(set_no_expiration),
    /* K67  */  be_nested_str_weak(persist_to_fabric),
    /* K68  */  be_nested_str_weak(save),
    }),
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[319]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x54120031,  //  0002  LDINT	R4	50
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x740E0005,  //  0004  JMPT	R3	#000B
      0x880C0302,  //  0005  GETMBR	R3	R1	K2
      0x200C0703,  //  0006  NE	R3	R3	K3
      0x740E0002,  //  0007  JMPT	R3	#000B
      0x880C0304,  //  0008  GETMBR	R3	R1	K4
      0x200C0703,  //  0009  NE	R3	R3	K3
      0x780E000D,  //  000A  JMPF	R3	#0019
      0xB80E0A00,  //  000B  GETNGBL	R3	K5
      0x8C0C0706,  //  000C  GETMET	R3	R3	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C0C0600,  //  000F  CALL	R3	3
      0x8C0C0109,  //  0010  GETMET	R3	R0	K9
      0x5C140200,  //  0011  MOVE	R5	R1
      0x5818000A,  //  0012  LDCONST	R6	K10
      0x581C0003,  //  0013  LDCONST	R7	K3
      0x58200008,  //  0014  LDCONST	R8	K8
      0x50240000,  //  0015  LDBOOL	R9	0	0
      0x7C0C0C00,  //  0016  CALL	R3	6
      0x50100000,  //  0017  LDBOOL	R4	0	0
      0x80040800,  //  0018  RET	1	R4
      0x880C030B,  //  0019  GETMBR	R3	R1	K11
      0xB8121800,  //  001A  GETNGBL	R4	K12
      0x8C10090D,  //  001B  GETMET	R4	R4	K13
      0x7C100200,  //  001C  CALL	R4	1
      0x8C10090E,  //  001D  GETMET	R4	R4	K14
      0x8818030F,  //  001E  GETMBR	R6	R1	K15
      0x881C0310,  //  001F  GETMBR	R7	R1	K16
      0x7C100600,  //  0020  CALL	R4	3
      0x8C140511,  //  0021  GETMET	R5	R2	K17
      0x7C140200,  //  0022  CALL	R5	1
      0x8C140B12,  //  0023  GETMET	R5	R5	K18
      0x881C0713,  //  0024  GETMBR	R7	R3	K19
      0x7C140400,  //  0025  CALL	R5	2
      0x8C140B12,  //  0026  GETMET	R5	R5	K18
      0x881C0714,  //  0027  GETMBR	R7	R3	K20
      0x7C140400,  //  0028  CALL	R5	2
      0x8C140B15,  //  0029  GETMET	R5	R5	K21
      0x7C140200,  //  002A  CALL	R5	1
      0x60180015,  //  002B  GETGBL	R6	G21
      0x7C180000,  //  002C  CALL	R6	0
      0x8C180D16,  //  002D  GETMET	R6	R6	K22
      0x88200117,  //  002E  GETMBR	R8	R0	K23
      0x7C180400,  //  002F  CALL	R6	2
      0x8C1C0518,  //  0030  GETMET	R7	R2	K24
      0x7C1C0200,  //  0031  CALL	R7	1
      0x8C1C0F19,  //  0032  GETMET	R7	R7	K25
      0x8824071A,  //  0033  GETMBR	R9	R3	K26
      0x8C28071B,  //  0034  GETMET	R10	R3	K27
      0x7C280200,  //  0035  CALL	R10	1
      0x00281405,  //  0036  ADD	R10	R10	R5
      0x5C2C0C00,  //  0037  MOVE	R11	R6
      0x5432000F,  //  0038  LDINT	R12	16
      0x7C1C0A00,  //  0039  CALL	R7	5
      0x5421FFEE,  //  003A  LDINT	R8	-17
      0x40220608,  //  003B  CONNECT	R8	K3	R8
      0x8824091C,  //  003C  GETMBR	R9	R4	K28
      0x94201208,  //  003D  GETIDX	R8	R9	R8
      0x5429FFEF,  //  003E  LDINT	R10	-16
      0x4028151D,  //  003F  CONNECT	R10	R10	K29
      0x882C091C,  //  0040  GETMBR	R11	R4	K28
      0x9424160A,  //  0041  GETIDX	R9	R11	R10
      0x8C30051E,  //  0042  GETMET	R12	R2	K30
      0x5C380E00,  //  0043  MOVE	R14	R7
      0x603C0015,  //  0044  GETGBL	R15	G21
      0x7C3C0000,  //  0045  CALL	R15	0
      0x8C3C1F16,  //  0046  GETMET	R15	R15	K22
      0x8844011F,  //  0047  GETMBR	R17	R0	K31
      0x7C3C0400,  //  0048  CALL	R15	2
      0x60400015,  //  0049  GETGBL	R16	G21
      0x7C400000,  //  004A  CALL	R16	0
      0x6044000C,  //  004B  GETGBL	R17	G12
      0x5C481000,  //  004C  MOVE	R18	R8
      0x7C440200,  //  004D  CALL	R17	1
      0x544A000F,  //  004E  LDINT	R18	16
      0x7C300C00,  //  004F  CALL	R12	6
      0x5C281800,  //  0050  MOVE	R10	R12
      0x8C301520,  //  0051  GETMET	R12	R10	K32
      0x5C381000,  //  0052  MOVE	R14	R8
      0x7C300400,  //  0053  CALL	R12	2
      0x5C2C1800,  //  0054  MOVE	R11	R12
      0x8C301521,  //  0055  GETMET	R12	R10	K33
      0x7C300200,  //  0056  CALL	R12	1
      0x20341809,  //  0057  NE	R13	R12	R9
      0x78360012,  //  0058  JMPF	R13	#006C
      0xB8360A00,  //  0059  GETNGBL	R13	K5
      0x8C341B06,  //  005A  GETMET	R13	R13	K6
      0x583C0022,  //  005B  LDCONST	R15	K34
      0x58400008,  //  005C  LDCONST	R16	K8
      0x7C340600,  //  005D  CALL	R13	3
      0xB8360A00,  //  005E  GETNGBL	R13	K5
      0x8C341B06,  //  005F  GETMET	R13	R13	K6
      0x583C0007,  //  0060  LDCONST	R15	K7
      0x58400008,  //  0061  LDCONST	R16	K8
      0x7C340600,  //  0062  CALL	R13	3
      0x8C340109,  //  0063  GETMET	R13	R0	K9
      0x5C3C0200,  //  0064  MOVE	R15	R1
      0x5840000A,  //  0065  LDCONST	R16	K10
      0x58440003,  //  0066  LDCONST	R17	K3
      0x58480008,  //  0067  LDCONST	R18	K8
      0x504C0000,  //  0068  LDBOOL	R19	0	0
      0x7C340C00,  //  0069  CALL	R13	6
      0x50380000,  //  006A  LDBOOL	R14	0	0
      0x80041C00,  //  006B  RET	1	R14
      0xB8361800,  //  006C  GETNGBL	R13	K12
      0x88341B23,  //  006D  GETMBR	R13	R13	K35
      0x8C341B0E,  //  006E  GETMET	R13	R13	K14
      0x5C3C1600,  //  006F  MOVE	R15	R11
      0x7C340400,  //  0070  CALL	R13	2
      0xB83A0A00,  //  0071  GETNGBL	R14	K5
      0x8C381D06,  //  0072  GETMET	R14	R14	K6
      0x60400008,  //  0073  GETGBL	R16	G8
      0x5C441A00,  //  0074  MOVE	R17	R13
      0x7C400200,  //  0075  CALL	R16	1
      0x00424810,  //  0076  ADD	R16	K36	R16
      0x54460003,  //  0077  LDINT	R17	4
      0x7C380600,  //  0078  CALL	R14	3
      0x8C381B25,  //  0079  GETMET	R14	R13	K37
      0x5840000A,  //  007A  LDCONST	R16	K10
      0x7C380400,  //  007B  CALL	R14	2
      0x8C3C1B25,  //  007C  GETMET	R15	R13	K37
      0x58440008,  //  007D  LDCONST	R17	K8
      0x7C3C0400,  //  007E  CALL	R15	2
      0x8C401B25,  //  007F  GETMET	R16	R13	K37
      0x58480026,  //  0080  LDCONST	R18	K38
      0x7C400400,  //  0081  CALL	R16	2
      0xB8461800,  //  0082  GETNGBL	R17	K12
      0x88442323,  //  0083  GETMBR	R17	R17	K35
      0x8C44230E,  //  0084  GETMET	R17	R17	K14
      0x5C4C1C00,  //  0085  MOVE	R19	R14
      0x7C440400,  //  0086  CALL	R17	2
      0xB84A0A00,  //  0087  GETNGBL	R18	K5
      0x8C482506,  //  0088  GETMET	R18	R18	K6
      0x60500008,  //  0089  GETGBL	R20	G8
      0x5C542200,  //  008A  MOVE	R21	R17
      0x7C500200,  //  008B  CALL	R20	1
      0x00524E14,  //  008C  ADD	R20	K39	R20
      0x58540026,  //  008D  LDCONST	R21	K38
      0x7C480600,  //  008E  CALL	R18	3
      0x8C482325,  //  008F  GETMET	R18	R17	K37
      0x54520008,  //  0090  LDINT	R20	9
      0x7C480400,  //  0091  CALL	R18	2
      0x8C4C2328,  //  0092  GETMET	R19	R17	K40
      0x54560005,  //  0093  LDINT	R21	6
      0x7C4C0400,  //  0094  CALL	R19	2
      0x8C502725,  //  0095  GETMET	R20	R19	K37
      0x545A0010,  //  0096  LDINT	R22	17
      0x7C500400,  //  0097  CALL	R20	2
      0x60540004,  //  0098  GETGBL	R21	G4
      0x5C582800,  //  0099  MOVE	R22	R20
      0x7C540200,  //  009A  CALL	R21	1
      0x1C542B29,  //  009B  EQ	R21	R21	K41
      0x78560007,  //  009C  JMPF	R21	#00A5
      0xB8565600,  //  009D  GETNGBL	R21	K43
      0x8C542B2C,  //  009E  GETMET	R21	R21	K44
      0x5C5C2800,  //  009F  MOVE	R23	R20
      0x7C540400,  //  00A0  CALL	R21	2
      0x8C542B2D,  //  00A1  GETMET	R21	R21	K45
      0x7C540200,  //  00A2  CALL	R21	1
      0x900E5415,  //  00A3  SETMBR	R3	K42	R21
      0x70020002,  //  00A4  JMP		#00A8
      0x8C54292D,  //  00A5  GETMET	R21	R20	K45
      0x7C540200,  //  00A6  CALL	R21	1
      0x900E5415,  //  00A7  SETMBR	R3	K42	R21
      0xB8560A00,  //  00A8  GETNGBL	R21	K5
      0x8C542B06,  //  00A9  GETMET	R21	R21	K6
      0x605C0008,  //  00AA  GETGBL	R23	G8
      0x8860072A,  //  00AB  GETMBR	R24	R3	K42
      0x7C5C0200,  //  00AC  CALL	R23	1
      0x005E5C17,  //  00AD  ADD	R23	K46	R23
      0x58600026,  //  00AE  LDCONST	R24	K38
      0x7C540600,  //  00AF  CALL	R21	3
      0xB8561800,  //  00B0  GETNGBL	R21	K12
      0x88542B23,  //  00B1  GETMBR	R21	R21	K35
      0x8C542B2F,  //  00B2  GETMET	R21	R21	K47
      0x7C540200,  //  00B3  CALL	R21	1
      0x8C582B30,  //  00B4  GETMET	R22	R21	K48
      0x5860000A,  //  00B5  LDCONST	R24	K10
      0xB8661800,  //  00B6  GETNGBL	R25	K12
      0x88643323,  //  00B7  GETMBR	R25	R25	K35
      0x88643331,  //  00B8  GETMBR	R25	R25	K49
      0x5C681C00,  //  00B9  MOVE	R26	R14
      0x7C580800,  //  00BA  CALL	R22	4
      0x8C582B30,  //  00BB  GETMET	R22	R21	K48
      0x58600008,  //  00BC  LDCONST	R24	K8
      0xB8661800,  //  00BD  GETNGBL	R25	K12
      0x88643323,  //  00BE  GETMBR	R25	R25	K35
      0x88643331,  //  00BF  GETMBR	R25	R25	K49
      0x5C681E00,  //  00C0  MOVE	R26	R15
      0x7C580800,  //  00C1  CALL	R22	4
      0x8C582B30,  //  00C2  GETMET	R22	R21	K48
      0x58600026,  //  00C3  LDCONST	R24	K38
      0xB8661800,  //  00C4  GETNGBL	R25	K12
      0x88643323,  //  00C5  GETMBR	R25	R25	K35
      0x88643331,  //  00C6  GETMBR	R25	R25	K49
      0x88680732,  //  00C7  GETMBR	R26	R3	K50
      0x7C580800,  //  00C8  CALL	R22	4
      0x8C582B30,  //  00C9  GETMET	R22	R21	K48
      0x54620003,  //  00CA  LDINT	R24	4
      0xB8661800,  //  00CB  GETNGBL	R25	K12
      0x88643323,  //  00CC  GETMBR	R25	R25	K35
      0x88643331,  //  00CD  GETMBR	R25	R25	K49
      0x88680733,  //  00CE  GETMBR	R26	R3	K51
      0x7C580800,  //  00CF  CALL	R22	4
      0x8C582B34,  //  00D0  GETMET	R22	R21	K52
      0x7C580200,  //  00D1  CALL	R22	1
      0x8C5C0535,  //  00D2  GETMET	R23	R2	K53
      0x7C5C0200,  //  00D3  CALL	R23	1
      0x8C5C2F36,  //  00D4  GETMET	R23	R23	K54
      0x5C642400,  //  00D5  MOVE	R25	R18
      0x5C682C00,  //  00D6  MOVE	R26	R22
      0x5C6C2000,  //  00D7  MOVE	R27	R16
      0x7C5C0800,  //  00D8  CALL	R23	4
      0x5C602E00,  //  00D9  MOVE	R24	R23
      0x74620012,  //  00DA  JMPT	R24	#00EE
      0xB8620A00,  //  00DB  GETNGBL	R24	K5
      0x8C603106,  //  00DC  GETMET	R24	R24	K6
      0x58680037,  //  00DD  LDCONST	R26	K55
      0x586C0008,  //  00DE  LDCONST	R27	K8
      0x7C600600,  //  00DF  CALL	R24	3
      0xB8620A00,  //  00E0  GETNGBL	R24	K5
      0x8C603106,  //  00E1  GETMET	R24	R24	K6
      0x58680007,  //  00E2  LDCONST	R26	K7
      0x586C0008,  //  00E3  LDCONST	R27	K8
      0x7C600600,  //  00E4  CALL	R24	3
      0x8C600109,  //  00E5  GETMET	R24	R0	K9
      0x5C680200,  //  00E6  MOVE	R26	R1
      0x586C000A,  //  00E7  LDCONST	R27	K10
      0x58700003,  //  00E8  LDCONST	R28	K3
      0x58740008,  //  00E9  LDCONST	R29	K8
      0x50780000,  //  00EA  LDBOOL	R30	0	0
      0x7C600C00,  //  00EB  CALL	R24	6
      0x50640000,  //  00EC  LDBOOL	R25	0	0
      0x80043200,  //  00ED  RET	1	R25
      0xB8620A00,  //  00EE  GETNGBL	R24	K5
      0x8C603106,  //  00EF  GETMET	R24	R24	K6
      0x58680038,  //  00F0  LDCONST	R26	K56
      0x586C0026,  //  00F1  LDCONST	R27	K38
      0x7C600600,  //  00F2  CALL	R24	3
      0x8C600511,  //  00F3  GETMET	R24	R2	K17
      0x7C600200,  //  00F4  CALL	R24	1
      0x8C603112,  //  00F5  GETMET	R24	R24	K18
      0x88680713,  //  00F6  GETMBR	R26	R3	K19
      0x7C600400,  //  00F7  CALL	R24	2
      0x8C603112,  //  00F8  GETMET	R24	R24	K18
      0x88680714,  //  00F9  GETMBR	R26	R3	K20
      0x7C600400,  //  00FA  CALL	R24	2
      0x8C603112,  //  00FB  GETMET	R24	R24	K18
      0x88680939,  //  00FC  GETMBR	R26	R4	K57
      0x7C600400,  //  00FD  CALL	R24	2
      0x8C603115,  //  00FE  GETMET	R24	R24	K21
      0x7C600200,  //  00FF  CALL	R24	1
      0x5C143000,  //  0100  MOVE	R5	R24
      0x4C600000,  //  0101  LDNIL	R24
      0x900E2618,  //  0102  SETMBR	R3	K19	R24
      0x4C600000,  //  0103  LDNIL	R24
      0x900E2818,  //  0104  SETMBR	R3	K20	R24
      0x8C600518,  //  0105  GETMET	R24	R2	K24
      0x7C600200,  //  0106  CALL	R24	1
      0x8C603119,  //  0107  GETMET	R24	R24	K25
      0x8868071A,  //  0108  GETMBR	R26	R3	K26
      0x8C6C071B,  //  0109  GETMET	R27	R3	K27
      0x7C6C0200,  //  010A  CALL	R27	1
      0x006C3605,  //  010B  ADD	R27	R27	R5
      0x60700015,  //  010C  GETGBL	R28	G21
      0x7C700000,  //  010D  CALL	R28	0
      0x8C703916,  //  010E  GETMET	R28	R28	K22
      0x8878013A,  //  010F  GETMBR	R30	R0	K58
      0x7C700400,  //  0110  CALL	R28	2
      0x5476002F,  //  0111  LDINT	R29	48
      0x7C600A00,  //  0112  CALL	R24	5
      0x5466000E,  //  0113  LDINT	R25	15
      0x40660619,  //  0114  CONNECT	R25	K3	R25
      0x94643019,  //  0115  GETIDX	R25	R24	R25
      0x546A000F,  //  0116  LDINT	R26	16
      0x546E001E,  //  0117  LDINT	R27	31
      0x4068341B,  //  0118  CONNECT	R26	R26	R27
      0x9468301A,  //  0119  GETIDX	R26	R24	R26
      0x546E001F,  //  011A  LDINT	R27	32
      0x5472002E,  //  011B  LDINT	R28	47
      0x406C361C,  //  011C  CONNECT	R27	R27	R28
      0x946C301B,  //  011D  GETIDX	R27	R24	R27
      0xB8720A00,  //  011E  GETNGBL	R28	K5
      0x8C70393B,  //  011F  GETMET	R28	R28	K59
      0x7C700200,  //  0120  CALL	R28	1
      0x9470393C,  //  0121  GETIDX	R28	R28	K60
      0x8C740109,  //  0122  GETMET	R29	R0	K9
      0x5C7C0200,  //  0123  MOVE	R31	R1
      0x58800003,  //  0124  LDCONST	R32	K3
      0x58840003,  //  0125  LDCONST	R33	K3
      0x58880003,  //  0126  LDCONST	R34	K3
      0x508C0200,  //  0127  LDBOOL	R35	1	0
      0x7C740C00,  //  0128  CALL	R29	6
      0x8C78073D,  //  0129  GETMET	R30	R3	K61
      0x7C780200,  //  012A  CALL	R30	1
      0x8C78073E,  //  012B  GETMET	R30	R3	K62
      0x5C803200,  //  012C  MOVE	R32	R25
      0x5C843400,  //  012D  MOVE	R33	R26
      0x5C883600,  //  012E  MOVE	R34	R27
      0x5C8C3800,  //  012F  MOVE	R35	R28
      0x7C780A00,  //  0130  CALL	R30	5
      0x900E7F03,  //  0131  SETMBR	R3	K63	K3
      0x8C780740,  //  0132  GETMET	R30	R3	K64
      0x7C780200,  //  0133  CALL	R30	1
      0x8C780741,  //  0134  GETMET	R30	R3	K65
      0x50800200,  //  0135  LDBOOL	R32	1	0
      0x7C780400,  //  0136  CALL	R30	2
      0x8C780742,  //  0137  GETMET	R30	R3	K66
      0x7C780200,  //  0138  CALL	R30	1
      0x8C780743,  //  0139  GETMET	R30	R3	K67
      0x7C780200,  //  013A  CALL	R30	1
      0x8C780744,  //  013B  GETMET	R30	R3	K68
      0x7C780200,  //  013C  CALL	R30	1
      0x50780200,  //  013D  LDBOOL	R30	1	0
      0x80043C00,  //  013E  RET	1	R30
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming
********************************************************************/
be_local_closure(Matter_Commisioning_Context_process_incoming,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(is_commissioning_open),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20commissioning_X20not_X20open),
    /* K6   */  be_const_int(2),
    /* K7   */  be_nested_str_weak(parse_PBKDFParamRequest),
    /* K8   */  be_nested_str_weak(parse_Pake1),
    /* K9   */  be_nested_str_weak(parse_Pake3),
    /* K10  */  be_nested_str_weak(parse_Sigma1),
    /* K11  */  be_nested_str_weak(parse_Sigma3),
    /* K12  */  be_nested_str_weak(parse_StatusReport),
    /* K13  */  be_nested_str_weak(string),
    /* K14  */  be_nested_str_weak(format),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28secure_X20channel_X29_X20_X2502X),
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A000E,  //  0003  JMPT	R2	#0013
      0x88080302,  //  0004  GETMBR	R2	R1	K2
      0x540E001F,  //  0005  LDINT	R3	32
      0x28080403,  //  0006  GE	R2	R2	R3
      0x780A000A,  //  0007  JMPF	R2	#0013
      0x88080302,  //  0008  GETMBR	R2	R1	K2
      0x540E0023,  //  0009  LDINT	R3	36
      0x18080403,  //  000A  LE	R2	R2	R3
      0x780A0006,  //  000B  JMPF	R2	#0013
      0xB80A0600,  //  000C  GETNGBL	R2	K3
      0x8C080504,  //  000D  GETMET	R2	R2	K4
      0x58100005,  //  000E  LDCONST	R4	K5
      0x58140006,  //  000F  LDCONST	R5	K6
      0x7C080600,  //  0010  CALL	R2	3
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x80040400,  //  0012  RET	1	R2
      0x88080302,  //  0013  GETMBR	R2	R1	K2
      0x540E000F,  //  0014  LDINT	R3	16
      0x1C080403,  //  0015  EQ	R2	R2	R3
      0x780A0000,  //  0016  JMPF	R2	#0018
      0x70020040,  //  0017  JMP		#0059
      0x88080302,  //  0018  GETMBR	R2	R1	K2
      0x540E001F,  //  0019  LDINT	R3	32
      0x1C080403,  //  001A  EQ	R2	R2	R3
      0x780A0004,  //  001B  JMPF	R2	#0021
      0x8C080107,  //  001C  GETMET	R2	R0	K7
      0x5C100200,  //  001D  MOVE	R4	R1
      0x7C080400,  //  001E  CALL	R2	2
      0x80040400,  //  001F  RET	1	R2
      0x70020037,  //  0020  JMP		#0059
      0x88080302,  //  0021  GETMBR	R2	R1	K2
      0x540E0021,  //  0022  LDINT	R3	34
      0x1C080403,  //  0023  EQ	R2	R2	R3
      0x780A0004,  //  0024  JMPF	R2	#002A
      0x8C080108,  //  0025  GETMET	R2	R0	K8
      0x5C100200,  //  0026  MOVE	R4	R1
      0x7C080400,  //  0027  CALL	R2	2
      0x80040400,  //  0028  RET	1	R2
      0x7002002E,  //  0029  JMP		#0059
      0x88080302,  //  002A  GETMBR	R2	R1	K2
      0x540E0023,  //  002B  LDINT	R3	36
      0x1C080403,  //  002C  EQ	R2	R2	R3
      0x780A0004,  //  002D  JMPF	R2	#0033
      0x8C080109,  //  002E  GETMET	R2	R0	K9
      0x5C100200,  //  002F  MOVE	R4	R1
      0x7C080400,  //  0030  CALL	R2	2
      0x80040400,  //  0031  RET	1	R2
      0x70020025,  //  0032  JMP		#0059
      0x88080302,  //  0033  GETMBR	R2	R1	K2
      0x540E002F,  //  0034  LDINT	R3	48
      0x1C080403,  //  0035  EQ	R2	R2	R3
      0x780A0004,  //  0036  JMPF	R2	#003C
      0x8C08010A,  //  0037  GETMET	R2	R0	K10
      0x5C100200,  //  0038  MOVE	R4	R1
      0x7C080400,  //  0039  CALL	R2	2
      0x80040400,  //  003A  RET	1	R2
      0x7002001C,  //  003B  JMP		#0059
      0x88080302,  //  003C  GETMBR	R2	R1	K2
      0x540E0031,  //  003D  LDINT	R3	50
      0x1C080403,  //  003E  EQ	R2	R2	R3
      0x780A0004,  //  003F  JMPF	R2	#0045
      0x8C08010B,  //  0040  GETMET	R2	R0	K11
      0x5C100200,  //  0041  MOVE	R4	R1
      0x7C080400,  //  0042  CALL	R2	2
      0x80040400,  //  0043  RET	1	R2
      0x70020013,  //  0044  JMP		#0059
      0x88080302,  //  0045  GETMBR	R2	R1	K2
      0x540E003F,  //  0046  LDINT	R3	64
      0x1C080403,  //  0047  EQ	R2	R2	R3
      0x780A0004,  //  0048  JMPF	R2	#004E
      0x8C08010C,  //  0049  GETMET	R2	R0	K12
      0x5C100200,  //  004A  MOVE	R4	R1
      0x7C080400,  //  004B  CALL	R2	2
      0x80040400,  //  004C  RET	1	R2
      0x7002000A,  //  004D  JMP		#0059
      0xA40A1A00,  //  004E  IMPORT	R2	K13
      0xB80E0600,  //  004F  GETNGBL	R3	K3
      0x8C0C0704,  //  0050  GETMET	R3	R3	K4
      0x8C14050E,  //  0051  GETMET	R5	R2	K14
      0x581C000F,  //  0052  LDCONST	R7	K15
      0x88200302,  //  0053  GETMBR	R8	R1	K2
      0x7C140600,  //  0054  CALL	R5	3
      0x58180006,  //  0055  LDCONST	R6	K6
      0x7C0C0600,  //  0056  CALL	R3	3
      0x500C0000,  //  0057  LDBOOL	R3	0	0
      0x80040600,  //  0058  RET	1	R3
      0x50080000,  //  0059  LDBOOL	R2	0	0
      0x80040400,  //  005A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Sigma1
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Sigma1,   /* name */
  be_nested_proto(
    37,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[123]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(session),
    /* K3   */  be_nested_str_weak(opcode),
    /* K4   */  be_nested_str_weak(local_session_id),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(protocol_id),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(send_status_report),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(Sigma1),
    /* K15  */  be_nested_str_weak(parse),
    /* K16  */  be_nested_str_weak(raw),
    /* K17  */  be_nested_str_weak(app_payload_idx),
    /* K18  */  be_nested_str_weak(__initiator_pub),
    /* K19  */  be_nested_str_weak(initiatorEphPubKey),
    /* K20  */  be_nested_str_weak(resumptionID),
    /* K21  */  be_nested_str_weak(initiatorResumeMIC),
    /* K22  */  be_nested_str_weak(format),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20is_resumption_X3D_X25i),
    /* K24  */  be_nested_str_weak(device),
    /* K25  */  be_nested_str_weak(sessions),
    /* K26  */  be_nested_str_weak(find_session_by_resumption_id),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20session_resumption_X20found_X20session_X3D_X25s_X20session_resumption_X3D_X25s),
    /* K28  */  be_nested_str_weak(inspect),
    /* K29  */  be_nested_str_weak(_fabric),
    /* K30  */  be_nested_str_weak(initiatorRandom),
    /* K31  */  be_nested_str_weak(fromstring),
    /* K32  */  be_nested_str_weak(Sigma1_Resume),
    /* K33  */  be_nested_str_weak(HKDF_SHA256),
    /* K34  */  be_nested_str_weak(derive),
    /* K35  */  be_nested_str_weak(shared_secret),
    /* K36  */  be_nested_str_weak(NCASE_SigmaS1),
    /* K37  */  be_const_int(2147483647),
    /* K38  */  be_nested_str_weak(AES_CCM),
    /* K39  */  be_nested_str_weak(decrypt),
    /* K40  */  be_nested_str_weak(tag),
    /* K41  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s1rk_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K43  */  be_nested_str_weak(tohex),
    /* K44  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20tag_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20Resume1MICPayload_X20_X3D_X20),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20decrypted_tag_X20_X20_X20_X20_X20_X3D_X20),
    /* K47  */  be_nested_str_weak(_source_node_id),
    /* K48  */  be_nested_str_weak(source_node_id),
    /* K49  */  be_nested_str_weak(set_mode_CASE),
    /* K50  */  be_nested_str_weak(__future_initiator_session_id),
    /* K51  */  be_nested_str_weak(initiator_session_id),
    /* K52  */  be_nested_str_weak(__future_local_session_id),
    /* K53  */  be_nested_str_weak(gen_local_session_id),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K55  */  be_nested_str_weak(remote_ip),
    /* K56  */  be_nested_str_weak(remote_port),
    /* K57  */  be_nested_str_weak(resumption_id),
    /* K58  */  be_nested_str_weak(random),
    /* K59  */  be_nested_str_weak(Sigma2_Resume),
    /* K60  */  be_nested_str_weak(NCASE_SigmaS2),
    /* K61  */  be_nested_str_weak(Sigma2Resume),
    /* K62  */  be_nested_str_weak(responderSessionID),
    /* K63  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K64  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2rk_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K65  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2rk_salt_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20new_resumption_id_X20_X3D_X20),
    /* K67  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20responderSessionID_X3D_X20),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20sigma2ResumeMIC_X20_X20_X20_X3D_X20),
    /* K69  */  be_nested_str_weak(SessionResumptionKeys),
    /* K70  */  be_nested_str_weak(rtc),
    /* K71  */  be_nested_str_weak(utc),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K76  */  be_nested_str_weak(tlv2raw),
    /* K77  */  be_nested_str_weak(__Msg1),
    /* K78  */  be_nested_str_weak(MTR_X3A_X20sigma2resume_X3A_X20),
    /* K79  */  be_nested_str_weak(MTR_X3A_X20sigma2resume_raw_X3A_X20),
    /* K80  */  be_nested_str_weak(build_response),
    /* K81  */  be_nested_str_weak(encode_frame),
    /* K82  */  be_nested_str_weak(responder),
    /* K83  */  be_nested_str_weak(send_response_frame),
    /* K84  */  be_nested_str_weak(close),
    /* K85  */  be_nested_str_weak(set_keys),
    /* K86  */  be_nested_str_weak(_breadcrumb),
    /* K87  */  be_nested_str_weak(counter_snd_next),
    /* K88  */  be_nested_str_weak(set_persist),
    /* K89  */  be_nested_str_weak(set_no_expiration),
    /* K90  */  be_nested_str_weak(persist_to_fabric),
    /* K91  */  be_nested_str_weak(save),
    /* K92  */  be_nested_str_weak(find_fabric_by_destination_id),
    /* K93  */  be_nested_str_weak(destinationId),
    /* K94  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K95  */  be_nested_str_weak(get_icac),
    /* K96  */  be_nested_str_weak(__responder_priv),
    /* K97  */  be_nested_str_weak(__responder_pub),
    /* K98  */  be_nested_str_weak(EC_P256),
    /* K99  */  be_nested_str_weak(public_key),
    /* K100 */  be_nested_str_weak(shared_key),
    /* K101 */  be_nested_str_weak(TLV),
    /* K102 */  be_nested_str_weak(Matter_TLV_struct),
    /* K103 */  be_nested_str_weak(add_TLV),
    /* K104 */  be_nested_str_weak(B2),
    /* K105 */  be_nested_str_weak(get_noc),
    /* K106 */  be_const_int(3),
    /* K107 */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K108 */  be_nested_str_weak(get_pk),
    /* K109 */  be_nested_str_weak(Msg1),
    /* K110 */  be_nested_str_weak(SHA256),
    /* K111 */  be_nested_str_weak(update),
    /* K112 */  be_nested_str_weak(out),
    /* K113 */  be_nested_str_weak(S2K_Info),
    /* K114 */  be_nested_str_weak(get_ipk_group_key),
    /* K115 */  be_nested_str_weak(TBEData2_Nonce),
    /* K116 */  be_nested_str_weak(encrypt),
    /* K117 */  be_nested_str_weak(Sigma2),
    /* K118 */  be_nested_str_weak(responderRandom),
    /* K119 */  be_nested_str_weak(responderSessionId),
    /* K120 */  be_nested_str_weak(responderEphPubKey),
    /* K121 */  be_nested_str_weak(encrypted2),
    /* K122 */  be_nested_str_weak(__Msg2),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[635]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x88100302,  //  0002  GETMBR	R4	R1	K2
      0x88140303,  //  0003  GETMBR	R5	R1	K3
      0x541A002F,  //  0004  LDINT	R6	48
      0x20140A06,  //  0005  NE	R5	R5	R6
      0x74160005,  //  0006  JMPT	R5	#000D
      0x88140304,  //  0007  GETMBR	R5	R1	K4
      0x20140B05,  //  0008  NE	R5	R5	K5
      0x74160002,  //  0009  JMPT	R5	#000D
      0x88140306,  //  000A  GETMBR	R5	R1	K6
      0x20140B05,  //  000B  NE	R5	R5	K5
      0x7816000D,  //  000C  JMPF	R5	#001B
      0xB8160E00,  //  000D  GETNGBL	R5	K7
      0x8C140B08,  //  000E  GETMET	R5	R5	K8
      0x581C0009,  //  000F  LDCONST	R7	K9
      0x5820000A,  //  0010  LDCONST	R8	K10
      0x7C140600,  //  0011  CALL	R5	3
      0x8C14010B,  //  0012  GETMET	R5	R0	K11
      0x5C1C0200,  //  0013  MOVE	R7	R1
      0x5820000C,  //  0014  LDCONST	R8	K12
      0x58240005,  //  0015  LDCONST	R9	K5
      0x5828000A,  //  0016  LDCONST	R10	K10
      0x502C0000,  //  0017  LDBOOL	R11	0	0
      0x7C140C00,  //  0018  CALL	R5	6
      0x50180000,  //  0019  LDBOOL	R6	0	0
      0x80040C00,  //  001A  RET	1	R6
      0xB8161A00,  //  001B  GETNGBL	R5	K13
      0x8C140B0E,  //  001C  GETMET	R5	R5	K14
      0x7C140200,  //  001D  CALL	R5	1
      0x8C140B0F,  //  001E  GETMET	R5	R5	K15
      0x881C0310,  //  001F  GETMBR	R7	R1	K16
      0x88200311,  //  0020  GETMBR	R8	R1	K17
      0x7C140600,  //  0021  CALL	R5	3
      0x88180B13,  //  0022  GETMBR	R6	R5	K19
      0x90122406,  //  0023  SETMBR	R4	K18	R6
      0x88180B14,  //  0024  GETMBR	R6	R5	K20
      0x4C1C0000,  //  0025  LDNIL	R7
      0x20180C07,  //  0026  NE	R6	R6	R7
      0x781A0003,  //  0027  JMPF	R6	#002C
      0x88180B15,  //  0028  GETMBR	R6	R5	K21
      0x4C1C0000,  //  0029  LDNIL	R7
      0x20180C07,  //  002A  NE	R6	R6	R7
      0x741A0000,  //  002B  JMPT	R6	#002D
      0x50180001,  //  002C  LDBOOL	R6	0	1
      0x50180200,  //  002D  LDBOOL	R6	1	0
      0xB81E0E00,  //  002E  GETNGBL	R7	K7
      0x8C1C0F08,  //  002F  GETMET	R7	R7	K8
      0x8C240716,  //  0030  GETMET	R9	R3	K22
      0x582C0017,  //  0031  LDCONST	R11	K23
      0x781A0001,  //  0032  JMPF	R6	#0035
      0x5830000C,  //  0033  LDCONST	R12	K12
      0x70020000,  //  0034  JMP		#0036
      0x58300005,  //  0035  LDCONST	R12	K5
      0x7C240600,  //  0036  CALL	R9	3
      0x542A0003,  //  0037  LDINT	R10	4
      0x7C1C0600,  //  0038  CALL	R7	3
      0x50180000,  //  0039  LDBOOL	R6	0	0
      0x4C1C0000,  //  003A  LDNIL	R7
      0x781A001C,  //  003B  JMPF	R6	#0059
      0x88200118,  //  003C  GETMBR	R8	R0	K24
      0x88201119,  //  003D  GETMBR	R8	R8	K25
      0x8C20111A,  //  003E  GETMET	R8	R8	K26
      0x88280B14,  //  003F  GETMBR	R10	R5	K20
      0x7C200400,  //  0040  CALL	R8	2
      0x5C1C1000,  //  0041  MOVE	R7	R8
      0xB8220E00,  //  0042  GETNGBL	R8	K7
      0x8C201108,  //  0043  GETMET	R8	R8	K8
      0x8C280716,  //  0044  GETMET	R10	R3	K22
      0x5830001B,  //  0045  LDCONST	R12	K27
      0xB8361A00,  //  0046  GETNGBL	R13	K13
      0x8C341B1C,  //  0047  GETMET	R13	R13	K28
      0x5C3C0800,  //  0048  MOVE	R15	R4
      0x7C340400,  //  0049  CALL	R13	2
      0xB83A1A00,  //  004A  GETNGBL	R14	K13
      0x8C381D1C,  //  004B  GETMET	R14	R14	K28
      0x5C400E00,  //  004C  MOVE	R16	R7
      0x7C380400,  //  004D  CALL	R14	2
      0x7C280800,  //  004E  CALL	R10	4
      0x542E0003,  //  004F  LDINT	R11	4
      0x7C200600,  //  0050  CALL	R8	3
      0x4C200000,  //  0051  LDNIL	R8
      0x1C200E08,  //  0052  EQ	R8	R7	R8
      0x74220003,  //  0053  JMPT	R8	#0058
      0x88200F1D,  //  0054  GETMBR	R8	R7	K29
      0x4C240000,  //  0055  LDNIL	R9
      0x1C201009,  //  0056  EQ	R8	R8	R9
      0x78220000,  //  0057  JMPF	R8	#0059
      0x50180000,  //  0058  LDBOOL	R6	0	0
      0x781A013A,  //  0059  JMPF	R6	#0195
      0x88200B1E,  //  005A  GETMBR	R8	R5	K30
      0x88240B14,  //  005B  GETMBR	R9	R5	K20
      0x00201009,  //  005C  ADD	R8	R8	R9
      0x60240015,  //  005D  GETGBL	R9	G21
      0x7C240000,  //  005E  CALL	R9	0
      0x8C24131F,  //  005F  GETMET	R9	R9	K31
      0x582C0020,  //  0060  LDCONST	R11	K32
      0x7C240400,  //  0061  CALL	R9	2
      0x8C280521,  //  0062  GETMET	R10	R2	K33
      0x7C280200,  //  0063  CALL	R10	1
      0x8C281522,  //  0064  GETMET	R10	R10	K34
      0x88300F23,  //  0065  GETMBR	R12	R7	K35
      0x5C341000,  //  0066  MOVE	R13	R8
      0x5C381200,  //  0067  MOVE	R14	R9
      0x543E000F,  //  0068  LDINT	R15	16
      0x7C280A00,  //  0069  CALL	R10	5
      0x602C0015,  //  006A  GETGBL	R11	G21
      0x7C2C0000,  //  006B  CALL	R11	0
      0x8C2C171F,  //  006C  GETMET	R11	R11	K31
      0x58340024,  //  006D  LDCONST	R13	K36
      0x7C2C0400,  //  006E  CALL	R11	2
      0x5431FFEE,  //  006F  LDINT	R12	-17
      0x40320A0C,  //  0070  CONNECT	R12	K5	R12
      0x88340B15,  //  0071  GETMBR	R13	R5	K21
      0x94301A0C,  //  0072  GETIDX	R12	R13	R12
      0x5439FFEF,  //  0073  LDINT	R14	-16
      0x40381D25,  //  0074  CONNECT	R14	R14	K37
      0x883C0B15,  //  0075  GETMBR	R15	R5	K21
      0x94341E0E,  //  0076  GETIDX	R13	R15	R14
      0x8C400526,  //  0077  GETMET	R16	R2	K38
      0x5C481400,  //  0078  MOVE	R18	R10
      0x5C4C1600,  //  0079  MOVE	R19	R11
      0x60500015,  //  007A  GETGBL	R20	G21
      0x7C500000,  //  007B  CALL	R20	0
      0x6054000C,  //  007C  GETGBL	R21	G12
      0x5C581800,  //  007D  MOVE	R22	R12
      0x7C540200,  //  007E  CALL	R21	1
      0x545A000F,  //  007F  LDINT	R22	16
      0x7C400C00,  //  0080  CALL	R16	6
      0x5C382000,  //  0081  MOVE	R14	R16
      0x8C401D27,  //  0082  GETMET	R16	R14	K39
      0x5C481800,  //  0083  MOVE	R18	R12
      0x7C400400,  //  0084  CALL	R16	2
      0x5C3C2000,  //  0085  MOVE	R15	R16
      0x8C401D28,  //  0086  GETMET	R16	R14	K40
      0x7C400200,  //  0087  CALL	R16	1
      0xB8460E00,  //  0088  GETNGBL	R17	K7
      0x8C442308,  //  0089  GETMET	R17	R17	K8
      0x584C0029,  //  008A  LDCONST	R19	K41
      0x54520003,  //  008B  LDINT	R20	4
      0x7C440600,  //  008C  CALL	R17	3
      0xB8460E00,  //  008D  GETNGBL	R17	K7
      0x8C442308,  //  008E  GETMET	R17	R17	K8
      0x8C4C152B,  //  008F  GETMET	R19	R10	K43
      0x7C4C0200,  //  0090  CALL	R19	1
      0x004E5413,  //  0091  ADD	R19	K42	R19
      0x54520003,  //  0092  LDINT	R20	4
      0x7C440600,  //  0093  CALL	R17	3
      0xB8460E00,  //  0094  GETNGBL	R17	K7
      0x8C442308,  //  0095  GETMET	R17	R17	K8
      0x8C4C1B2B,  //  0096  GETMET	R19	R13	K43
      0x7C4C0200,  //  0097  CALL	R19	1
      0x004E5813,  //  0098  ADD	R19	K44	R19
      0x54520003,  //  0099  LDINT	R20	4
      0x7C440600,  //  009A  CALL	R17	3
      0xB8460E00,  //  009B  GETNGBL	R17	K7
      0x8C442308,  //  009C  GETMET	R17	R17	K8
      0x8C4C1F2B,  //  009D  GETMET	R19	R15	K43
      0x7C4C0200,  //  009E  CALL	R19	1
      0x004E5A13,  //  009F  ADD	R19	K45	R19
      0x54520003,  //  00A0  LDINT	R20	4
      0x7C440600,  //  00A1  CALL	R17	3
      0xB8460E00,  //  00A2  GETNGBL	R17	K7
      0x8C442308,  //  00A3  GETMET	R17	R17	K8
      0x8C4C212B,  //  00A4  GETMET	R19	R16	K43
      0x7C4C0200,  //  00A5  CALL	R19	1
      0x004E5C13,  //  00A6  ADD	R19	K46	R19
      0x54520003,  //  00A7  LDINT	R20	4
      0x7C440600,  //  00A8  CALL	R17	3
      0xB8460E00,  //  00A9  GETNGBL	R17	K7
      0x8C442308,  //  00AA  GETMET	R17	R17	K8
      0x584C0029,  //  00AB  LDCONST	R19	K41
      0x54520003,  //  00AC  LDINT	R20	4
      0x7C440600,  //  00AD  CALL	R17	3
      0x1C441A10,  //  00AE  EQ	R17	R13	R16
      0x784600E3,  //  00AF  JMPF	R17	#0194
      0x88440F1D,  //  00B0  GETMBR	R17	R7	K29
      0x90123A11,  //  00B1  SETMBR	R4	K29	R17
      0x88440330,  //  00B2  GETMBR	R17	R1	K48
      0x90125E11,  //  00B3  SETMBR	R4	K47	R17
      0x8C440931,  //  00B4  GETMET	R17	R4	K49
      0x7C440200,  //  00B5  CALL	R17	1
      0x88440B33,  //  00B6  GETMBR	R17	R5	K51
      0x90126411,  //  00B7  SETMBR	R4	K50	R17
      0x88440118,  //  00B8  GETMBR	R17	R0	K24
      0x88442319,  //  00B9  GETMBR	R17	R17	K25
      0x8C442335,  //  00BA  GETMET	R17	R17	K53
      0x7C440200,  //  00BB  CALL	R17	1
      0x90126811,  //  00BC  SETMBR	R4	K52	R17
      0xB8460E00,  //  00BD  GETNGBL	R17	K7
      0x8C442308,  //  00BE  GETMET	R17	R17	K8
      0x8C4C0716,  //  00BF  GETMET	R19	R3	K22
      0x58540036,  //  00C0  LDCONST	R21	K54
      0x88580934,  //  00C1  GETMBR	R22	R4	K52
      0x885C0337,  //  00C2  GETMBR	R23	R1	K55
      0x88600338,  //  00C3  GETMBR	R24	R1	K56
      0x7C4C0A00,  //  00C4  CALL	R19	5
      0x5850000A,  //  00C5  LDCONST	R20	K10
      0x7C440600,  //  00C6  CALL	R17	3
      0x88440F23,  //  00C7  GETMBR	R17	R7	K35
      0x90124611,  //  00C8  SETMBR	R4	K35	R17
      0x8C44053A,  //  00C9  GETMET	R17	R2	K58
      0x544E000F,  //  00CA  LDINT	R19	16
      0x7C440400,  //  00CB  CALL	R17	2
      0x90127211,  //  00CC  SETMBR	R4	K57	R17
      0x60440015,  //  00CD  GETGBL	R17	G21
      0x7C440000,  //  00CE  CALL	R17	0
      0x8C44231F,  //  00CF  GETMET	R17	R17	K31
      0x584C003B,  //  00D0  LDCONST	R19	K59
      0x7C440400,  //  00D1  CALL	R17	2
      0x88480B1E,  //  00D2  GETMBR	R18	R5	K30
      0x884C0939,  //  00D3  GETMBR	R19	R4	K57
      0x00482413,  //  00D4  ADD	R18	R18	R19
      0x8C4C0521,  //  00D5  GETMET	R19	R2	K33
      0x7C4C0200,  //  00D6  CALL	R19	1
      0x8C4C2722,  //  00D7  GETMET	R19	R19	K34
      0x88540923,  //  00D8  GETMBR	R21	R4	K35
      0x5C582400,  //  00D9  MOVE	R22	R18
      0x5C5C2200,  //  00DA  MOVE	R23	R17
      0x5462000F,  //  00DB  LDINT	R24	16
      0x7C4C0A00,  //  00DC  CALL	R19	5
      0x8C500526,  //  00DD  GETMET	R20	R2	K38
      0x5C582600,  //  00DE  MOVE	R22	R19
      0x605C0015,  //  00DF  GETGBL	R23	G21
      0x7C5C0000,  //  00E0  CALL	R23	0
      0x8C5C2F1F,  //  00E1  GETMET	R23	R23	K31
      0x5864003C,  //  00E2  LDCONST	R25	K60
      0x7C5C0400,  //  00E3  CALL	R23	2
      0x60600015,  //  00E4  GETGBL	R24	G21
      0x7C600000,  //  00E5  CALL	R24	0
      0x58640005,  //  00E6  LDCONST	R25	K5
      0x546A000F,  //  00E7  LDINT	R26	16
      0x7C500C00,  //  00E8  CALL	R20	6
      0x8C542928,  //  00E9  GETMET	R21	R20	K40
      0x7C540200,  //  00EA  CALL	R21	1
      0xB85A1A00,  //  00EB  GETNGBL	R22	K13
      0x8C582D3D,  //  00EC  GETMET	R22	R22	K61
      0x7C580200,  //  00ED  CALL	R22	1
      0x885C0939,  //  00EE  GETMBR	R23	R4	K57
      0x905A2817,  //  00EF  SETMBR	R22	K20	R23
      0x885C0934,  //  00F0  GETMBR	R23	R4	K52
      0x905A7C17,  //  00F1  SETMBR	R22	K62	R23
      0x905A7E15,  //  00F2  SETMBR	R22	K63	R21
      0xB85E0E00,  //  00F3  GETNGBL	R23	K7
      0x8C5C2F08,  //  00F4  GETMET	R23	R23	K8
      0x58640029,  //  00F5  LDCONST	R25	K41
      0x546A0003,  //  00F6  LDINT	R26	4
      0x7C5C0600,  //  00F7  CALL	R23	3
      0xB85E0E00,  //  00F8  GETNGBL	R23	K7
      0x8C5C2F08,  //  00F9  GETMET	R23	R23	K8
      0x8C64272B,  //  00FA  GETMET	R25	R19	K43
      0x7C640200,  //  00FB  CALL	R25	1
      0x00668019,  //  00FC  ADD	R25	K64	R25
      0x546A0003,  //  00FD  LDINT	R26	4
      0x7C5C0600,  //  00FE  CALL	R23	3
      0xB85E0E00,  //  00FF  GETNGBL	R23	K7
      0x8C5C2F08,  //  0100  GETMET	R23	R23	K8
      0x8C64252B,  //  0101  GETMET	R25	R18	K43
      0x7C640200,  //  0102  CALL	R25	1
      0x00668219,  //  0103  ADD	R25	K65	R25
      0x546A0003,  //  0104  LDINT	R26	4
      0x7C5C0600,  //  0105  CALL	R23	3
      0xB85E0E00,  //  0106  GETNGBL	R23	K7
      0x8C5C2F08,  //  0107  GETMET	R23	R23	K8
      0x88640939,  //  0108  GETMBR	R25	R4	K57
      0x8C64332B,  //  0109  GETMET	R25	R25	K43
      0x7C640200,  //  010A  CALL	R25	1
      0x00668419,  //  010B  ADD	R25	K66	R25
      0x546A0003,  //  010C  LDINT	R26	4
      0x7C5C0600,  //  010D  CALL	R23	3
      0xB85E0E00,  //  010E  GETNGBL	R23	K7
      0x8C5C2F08,  //  010F  GETMET	R23	R23	K8
      0x60640008,  //  0110  GETGBL	R25	G8
      0x88680934,  //  0111  GETMBR	R26	R4	K52
      0x7C640200,  //  0112  CALL	R25	1
      0x00668619,  //  0113  ADD	R25	K67	R25
      0x546A0003,  //  0114  LDINT	R26	4
      0x7C5C0600,  //  0115  CALL	R23	3
      0xB85E0E00,  //  0116  GETNGBL	R23	K7
      0x8C5C2F08,  //  0117  GETMET	R23	R23	K8
      0x8C642B2B,  //  0118  GETMET	R25	R21	K43
      0x7C640200,  //  0119  CALL	R25	1
      0x00668819,  //  011A  ADD	R25	K68	R25
      0x546A0003,  //  011B  LDINT	R26	4
      0x7C5C0600,  //  011C  CALL	R23	3
      0xB85E0E00,  //  011D  GETNGBL	R23	K7
      0x8C5C2F08,  //  011E  GETMET	R23	R23	K8
      0x58640029,  //  011F  LDCONST	R25	K41
      0x546A0003,  //  0120  LDINT	R26	4
      0x7C5C0600,  //  0121  CALL	R23	3
      0x8C5C0521,  //  0122  GETMET	R23	R2	K33
      0x7C5C0200,  //  0123  CALL	R23	1
      0x8C5C2F22,  //  0124  GETMET	R23	R23	K34
      0x88640923,  //  0125  GETMBR	R25	R4	K35
      0x88680B1E,  //  0126  GETMBR	R26	R5	K30
      0x886C0939,  //  0127  GETMBR	R27	R4	K57
      0x0068341B,  //  0128  ADD	R26	R26	R27
      0x606C0015,  //  0129  GETGBL	R27	G21
      0x7C6C0000,  //  012A  CALL	R27	0
      0x8C6C371F,  //  012B  GETMET	R27	R27	K31
      0x58740045,  //  012C  LDCONST	R29	K69
      0x7C6C0400,  //  012D  CALL	R27	2
      0x5472002F,  //  012E  LDINT	R28	48
      0x7C5C0A00,  //  012F  CALL	R23	5
      0x5462000E,  //  0130  LDINT	R24	15
      0x40620A18,  //  0131  CONNECT	R24	K5	R24
      0x94602E18,  //  0132  GETIDX	R24	R23	R24
      0x5466000F,  //  0133  LDINT	R25	16
      0x546A001E,  //  0134  LDINT	R26	31
      0x4064321A,  //  0135  CONNECT	R25	R25	R26
      0x94642E19,  //  0136  GETIDX	R25	R23	R25
      0x546A001F,  //  0137  LDINT	R26	32
      0x546E002E,  //  0138  LDINT	R27	47
      0x4068341B,  //  0139  CONNECT	R26	R26	R27
      0x94682E1A,  //  013A  GETIDX	R26	R23	R26
      0xB86E0E00,  //  013B  GETNGBL	R27	K7
      0x8C6C3746,  //  013C  GETMET	R27	R27	K70
      0x7C6C0200,  //  013D  CALL	R27	1
      0x946C3747,  //  013E  GETIDX	R27	R27	K71
      0xB8720E00,  //  013F  GETNGBL	R28	K7
      0x8C703908,  //  0140  GETMET	R28	R28	K8
      0x58780048,  //  0141  LDCONST	R30	K72
      0x547E0003,  //  0142  LDINT	R31	4
      0x7C700600,  //  0143  CALL	R28	3
      0xB8720E00,  //  0144  GETNGBL	R28	K7
      0x8C703908,  //  0145  GETMET	R28	R28	K8
      0x8C78312B,  //  0146  GETMET	R30	R24	K43
      0x7C780200,  //  0147  CALL	R30	1
      0x007A921E,  //  0148  ADD	R30	K73	R30
      0x547E0003,  //  0149  LDINT	R31	4
      0x7C700600,  //  014A  CALL	R28	3
      0xB8720E00,  //  014B  GETNGBL	R28	K7
      0x8C703908,  //  014C  GETMET	R28	R28	K8
      0x8C78332B,  //  014D  GETMET	R30	R25	K43
      0x7C780200,  //  014E  CALL	R30	1
      0x007A941E,  //  014F  ADD	R30	K74	R30
      0x547E0003,  //  0150  LDINT	R31	4
      0x7C700600,  //  0151  CALL	R28	3
      0xB8720E00,  //  0152  GETNGBL	R28	K7
      0x8C703908,  //  0153  GETMET	R28	R28	K8
      0x8C78352B,  //  0154  GETMET	R30	R26	K43
      0x7C780200,  //  0155  CALL	R30	1
      0x007A961E,  //  0156  ADD	R30	K75	R30
      0x547E0003,  //  0157  LDINT	R31	4
      0x7C700600,  //  0158  CALL	R28	3
      0xB8720E00,  //  0159  GETNGBL	R28	K7
      0x8C703908,  //  015A  GETMET	R28	R28	K8
      0x58780048,  //  015B  LDCONST	R30	K72
      0x547E0003,  //  015C  LDINT	R31	4
      0x7C700600,  //  015D  CALL	R28	3
      0x8C702D4C,  //  015E  GETMET	R28	R22	K76
      0x7C700200,  //  015F  CALL	R28	1
      0x4C740000,  //  0160  LDNIL	R29
      0x90129A1D,  //  0161  SETMBR	R4	K77	R29
      0xB8760E00,  //  0162  GETNGBL	R29	K7
      0x8C743B08,  //  0163  GETMET	R29	R29	K8
      0xB87E1A00,  //  0164  GETNGBL	R31	K13
      0x8C7C3F1C,  //  0165  GETMET	R31	R31	K28
      0x5C842C00,  //  0166  MOVE	R33	R22
      0x7C7C0400,  //  0167  CALL	R31	2
      0x007E9C1F,  //  0168  ADD	R31	K78	R31
      0x54820003,  //  0169  LDINT	R32	4
      0x7C740600,  //  016A  CALL	R29	3
      0xB8760E00,  //  016B  GETNGBL	R29	K7
      0x8C743B08,  //  016C  GETMET	R29	R29	K8
      0x8C7C392B,  //  016D  GETMET	R31	R28	K43
      0x7C7C0200,  //  016E  CALL	R31	1
      0x007E9E1F,  //  016F  ADD	R31	K79	R31
      0x54820003,  //  0170  LDINT	R32	4
      0x7C740600,  //  0171  CALL	R29	3
      0x8C740350,  //  0172  GETMET	R29	R1	K80
      0x547E0032,  //  0173  LDINT	R31	51
      0x50800200,  //  0174  LDBOOL	R32	1	0
      0x7C740600,  //  0175  CALL	R29	3
      0x8C783B51,  //  0176  GETMET	R30	R29	K81
      0x5C803800,  //  0177  MOVE	R32	R28
      0x7C780400,  //  0178  CALL	R30	2
      0x887C0152,  //  0179  GETMBR	R31	R0	K82
      0x8C7C3F53,  //  017A  GETMET	R31	R31	K83
      0x5C843A00,  //  017B  MOVE	R33	R29
      0x7C7C0400,  //  017C  CALL	R31	2
      0x8C7C0954,  //  017D  GETMET	R31	R4	K84
      0x7C7C0200,  //  017E  CALL	R31	1
      0x8C7C0955,  //  017F  GETMET	R31	R4	K85
      0x5C843000,  //  0180  MOVE	R33	R24
      0x5C883200,  //  0181  MOVE	R34	R25
      0x5C8C3400,  //  0182  MOVE	R35	R26
      0x5C903600,  //  0183  MOVE	R36	R27
      0x7C7C0A00,  //  0184  CALL	R31	5
      0x9012AD05,  //  0185  SETMBR	R4	K86	K5
      0x8C7C0957,  //  0186  GETMET	R31	R4	K87
      0x7C7C0200,  //  0187  CALL	R31	1
      0x8C7C0958,  //  0188  GETMET	R31	R4	K88
      0x50840200,  //  0189  LDBOOL	R33	1	0
      0x7C7C0400,  //  018A  CALL	R31	2
      0x8C7C0959,  //  018B  GETMET	R31	R4	K89
      0x7C7C0200,  //  018C  CALL	R31	1
      0x8C7C095A,  //  018D  GETMET	R31	R4	K90
      0x7C7C0200,  //  018E  CALL	R31	1
      0x8C7C095B,  //  018F  GETMET	R31	R4	K91
      0x7C7C0200,  //  0190  CALL	R31	1
      0x507C0200,  //  0191  LDBOOL	R31	1	0
      0x80043E00,  //  0192  RET	1	R31
      0x70020000,  //  0193  JMP		#0195
      0x50180000,  //  0194  LDBOOL	R6	0	0
      0x5C200C00,  //  0195  MOVE	R8	R6
      0x742200E1,  //  0196  JMPT	R8	#0279
      0x8C20015C,  //  0197  GETMET	R8	R0	K92
      0x88280B5D,  //  0198  GETMBR	R10	R5	K93
      0x882C0B1E,  //  0199  GETMBR	R11	R5	K30
      0x7C200600,  //  019A  CALL	R8	3
      0x90123A08,  //  019B  SETMBR	R4	K29	R8
      0x4C240000,  //  019C  LDNIL	R9
      0x1C240809,  //  019D  EQ	R9	R4	R9
      0x74260003,  //  019E  JMPT	R9	#01A3
      0x8824091D,  //  019F  GETMBR	R9	R4	K29
      0x4C280000,  //  01A0  LDNIL	R10
      0x1C24120A,  //  01A1  EQ	R9	R9	R10
      0x7826000D,  //  01A2  JMPF	R9	#01B1
      0xB8260E00,  //  01A3  GETNGBL	R9	K7
      0x8C241308,  //  01A4  GETMET	R9	R9	K8
      0x582C005E,  //  01A5  LDCONST	R11	K94
      0x5830000A,  //  01A6  LDCONST	R12	K10
      0x7C240600,  //  01A7  CALL	R9	3
      0x8C24010B,  //  01A8  GETMET	R9	R0	K11
      0x5C2C0200,  //  01A9  MOVE	R11	R1
      0x5830000C,  //  01AA  LDCONST	R12	K12
      0x58340005,  //  01AB  LDCONST	R13	K5
      0x5838000C,  //  01AC  LDCONST	R14	K12
      0x503C0000,  //  01AD  LDBOOL	R15	0	0
      0x7C240C00,  //  01AE  CALL	R9	6
      0x50280000,  //  01AF  LDBOOL	R10	0	0
      0x80041400,  //  01B0  RET	1	R10
      0x88240330,  //  01B1  GETMBR	R9	R1	K48
      0x90125E09,  //  01B2  SETMBR	R4	K47	R9
      0x8C240931,  //  01B3  GETMET	R9	R4	K49
      0x7C240200,  //  01B4  CALL	R9	1
      0x88240B33,  //  01B5  GETMBR	R9	R5	K51
      0x90126409,  //  01B6  SETMBR	R4	K50	R9
      0x88240118,  //  01B7  GETMBR	R9	R0	K24
      0x88241319,  //  01B8  GETMBR	R9	R9	K25
      0x8C241335,  //  01B9  GETMET	R9	R9	K53
      0x7C240200,  //  01BA  CALL	R9	1
      0x90126809,  //  01BB  SETMBR	R4	K52	R9
      0xB8260E00,  //  01BC  GETNGBL	R9	K7
      0x8C241308,  //  01BD  GETMET	R9	R9	K8
      0x8C2C0716,  //  01BE  GETMET	R11	R3	K22
      0x58340036,  //  01BF  LDCONST	R13	K54
      0x88380934,  //  01C0  GETMBR	R14	R4	K52
      0x883C0337,  //  01C1  GETMBR	R15	R1	K55
      0x88400338,  //  01C2  GETMBR	R16	R1	K56
      0x7C2C0A00,  //  01C3  CALL	R11	5
      0x5830000A,  //  01C4  LDCONST	R12	K10
      0x7C240600,  //  01C5  CALL	R9	3
      0x8824091D,  //  01C6  GETMBR	R9	R4	K29
      0x8C24135F,  //  01C7  GETMET	R9	R9	K95
      0x7C240200,  //  01C8  CALL	R9	1
      0x7825FFFF,  //  01C9  JMPF	R9	#01CA
      0x8C24053A,  //  01CA  GETMET	R9	R2	K58
      0x542E000F,  //  01CB  LDINT	R11	16
      0x7C240400,  //  01CC  CALL	R9	2
      0x90127209,  //  01CD  SETMBR	R4	K57	R9
      0x8C24053A,  //  01CE  GETMET	R9	R2	K58
      0x542E001F,  //  01CF  LDINT	R11	32
      0x7C240400,  //  01D0  CALL	R9	2
      0x9012C009,  //  01D1  SETMBR	R4	K96	R9
      0x8C240562,  //  01D2  GETMET	R9	R2	K98
      0x7C240200,  //  01D3  CALL	R9	1
      0x8C241363,  //  01D4  GETMET	R9	R9	K99
      0x882C0960,  //  01D5  GETMBR	R11	R4	K96
      0x7C240400,  //  01D6  CALL	R9	2
      0x9012C209,  //  01D7  SETMBR	R4	K97	R9
      0x8C24053A,  //  01D8  GETMET	R9	R2	K58
      0x542E001F,  //  01D9  LDINT	R11	32
      0x7C240400,  //  01DA  CALL	R9	2
      0x8C280562,  //  01DB  GETMET	R10	R2	K98
      0x7C280200,  //  01DC  CALL	R10	1
      0x8C281564,  //  01DD  GETMET	R10	R10	K100
      0x88300960,  //  01DE  GETMBR	R12	R4	K96
      0x88340B13,  //  01DF  GETMBR	R13	R5	K19
      0x7C280600,  //  01E0  CALL	R10	3
      0x9012460A,  //  01E1  SETMBR	R4	K35	R10
      0xB82A1A00,  //  01E2  GETNGBL	R10	K13
      0x88281565,  //  01E3  GETMBR	R10	R10	K101
      0x8C281566,  //  01E4  GETMET	R10	R10	K102
      0x7C280200,  //  01E5  CALL	R10	1
      0x8C2C1567,  //  01E6  GETMET	R11	R10	K103
      0x5834000C,  //  01E7  LDCONST	R13	K12
      0xB83A1A00,  //  01E8  GETNGBL	R14	K13
      0x88381D65,  //  01E9  GETMBR	R14	R14	K101
      0x88381D68,  //  01EA  GETMBR	R14	R14	K104
      0x8C3C0969,  //  01EB  GETMET	R15	R4	K105
      0x7C3C0200,  //  01EC  CALL	R15	1
      0x7C2C0800,  //  01ED  CALL	R11	4
      0x8C2C1567,  //  01EE  GETMET	R11	R10	K103
      0x5834000A,  //  01EF  LDCONST	R13	K10
      0xB83A1A00,  //  01F0  GETNGBL	R14	K13
      0x88381D65,  //  01F1  GETMBR	R14	R14	K101
      0x88381D68,  //  01F2  GETMBR	R14	R14	K104
      0x8C3C095F,  //  01F3  GETMET	R15	R4	K95
      0x7C3C0200,  //  01F4  CALL	R15	1
      0x7C2C0800,  //  01F5  CALL	R11	4
      0x8C2C1567,  //  01F6  GETMET	R11	R10	K103
      0x5834006A,  //  01F7  LDCONST	R13	K106
      0xB83A1A00,  //  01F8  GETNGBL	R14	K13
      0x88381D65,  //  01F9  GETMBR	R14	R14	K101
      0x88381D68,  //  01FA  GETMBR	R14	R14	K104
      0x883C0961,  //  01FB  GETMBR	R15	R4	K97
      0x7C2C0800,  //  01FC  CALL	R11	4
      0x8C2C1567,  //  01FD  GETMET	R11	R10	K103
      0x54360003,  //  01FE  LDINT	R13	4
      0xB83A1A00,  //  01FF  GETNGBL	R14	K13
      0x88381D65,  //  0200  GETMBR	R14	R14	K101
      0x88381D68,  //  0201  GETMBR	R14	R14	K104
      0x883C0B13,  //  0202  GETMBR	R15	R5	K19
      0x7C2C0800,  //  0203  CALL	R11	4
      0x8C2C0562,  //  0204  GETMET	R11	R2	K98
      0x7C2C0200,  //  0205  CALL	R11	1
      0x8C2C176B,  //  0206  GETMET	R11	R11	K107
      0x8C34096C,  //  0207  GETMET	R13	R4	K108
      0x7C340200,  //  0208  CALL	R13	1
      0x8C38154C,  //  0209  GETMET	R14	R10	K76
      0x7C380200,  //  020A  CALL	R14	1
      0x7C2C0600,  //  020B  CALL	R11	3
      0xB8321A00,  //  020C  GETNGBL	R12	K13
      0x88301965,  //  020D  GETMBR	R12	R12	K101
      0x8C301966,  //  020E  GETMET	R12	R12	K102
      0x7C300200,  //  020F  CALL	R12	1
      0x8C341967,  //  0210  GETMET	R13	R12	K103
      0x583C000C,  //  0211  LDCONST	R15	K12
      0xB8421A00,  //  0212  GETNGBL	R16	K13
      0x88402165,  //  0213  GETMBR	R16	R16	K101
      0x88402168,  //  0214  GETMBR	R16	R16	K104
      0x8C440969,  //  0215  GETMET	R17	R4	K105
      0x7C440200,  //  0216  CALL	R17	1
      0x7C340800,  //  0217  CALL	R13	4
      0x8C341967,  //  0218  GETMET	R13	R12	K103
      0x583C000A,  //  0219  LDCONST	R15	K10
      0xB8421A00,  //  021A  GETNGBL	R16	K13
      0x88402165,  //  021B  GETMBR	R16	R16	K101
      0x88402168,  //  021C  GETMBR	R16	R16	K104
      0x8C44095F,  //  021D  GETMET	R17	R4	K95
      0x7C440200,  //  021E  CALL	R17	1
      0x7C340800,  //  021F  CALL	R13	4
      0x8C341967,  //  0220  GETMET	R13	R12	K103
      0x583C006A,  //  0221  LDCONST	R15	K106
      0xB8421A00,  //  0222  GETNGBL	R16	K13
      0x88402165,  //  0223  GETMBR	R16	R16	K101
      0x88402168,  //  0224  GETMBR	R16	R16	K104
      0x5C441600,  //  0225  MOVE	R17	R11
      0x7C340800,  //  0226  CALL	R13	4
      0x8C341967,  //  0227  GETMET	R13	R12	K103
      0x543E0003,  //  0228  LDINT	R15	4
      0xB8421A00,  //  0229  GETNGBL	R16	K13
      0x88402165,  //  022A  GETMBR	R16	R16	K101
      0x88402168,  //  022B  GETMBR	R16	R16	K104
      0x88440939,  //  022C  GETMBR	R17	R4	K57
      0x7C340800,  //  022D  CALL	R13	4
      0x88340B6D,  //  022E  GETMBR	R13	R5	K109
      0x90129A0D,  //  022F  SETMBR	R4	K77	R13
      0x8C34056E,  //  0230  GETMET	R13	R2	K110
      0x7C340200,  //  0231  CALL	R13	1
      0x8C341B6F,  //  0232  GETMET	R13	R13	K111
      0x883C094D,  //  0233  GETMBR	R15	R4	K77
      0x7C340400,  //  0234  CALL	R13	2
      0x8C341B70,  //  0235  GETMET	R13	R13	K112
      0x7C340200,  //  0236  CALL	R13	1
      0x60380015,  //  0237  GETGBL	R14	G21
      0x7C380000,  //  0238  CALL	R14	0
      0x8C381D1F,  //  0239  GETMET	R14	R14	K31
      0x88400171,  //  023A  GETMBR	R16	R0	K113
      0x7C380400,  //  023B  CALL	R14	2
      0x8C3C0972,  //  023C  GETMET	R15	R4	K114
      0x7C3C0200,  //  023D  CALL	R15	1
      0x003C1E09,  //  023E  ADD	R15	R15	R9
      0x88400961,  //  023F  GETMBR	R16	R4	K97
      0x003C1E10,  //  0240  ADD	R15	R15	R16
      0x003C1E0D,  //  0241  ADD	R15	R15	R13
      0x8C400521,  //  0242  GETMET	R16	R2	K33
      0x7C400200,  //  0243  CALL	R16	1
      0x8C402122,  //  0244  GETMET	R16	R16	K34
      0x88480923,  //  0245  GETMBR	R18	R4	K35
      0x5C4C1E00,  //  0246  MOVE	R19	R15
      0x5C501C00,  //  0247  MOVE	R20	R14
      0x5456000F,  //  0248  LDINT	R21	16
      0x7C400A00,  //  0249  CALL	R16	5
      0x8C44194C,  //  024A  GETMET	R17	R12	K76
      0x7C440200,  //  024B  CALL	R17	1
      0x8C480526,  //  024C  GETMET	R18	R2	K38
      0x5C502000,  //  024D  MOVE	R20	R16
      0x60540015,  //  024E  GETGBL	R21	G21
      0x7C540000,  //  024F  CALL	R21	0
      0x8C542B1F,  //  0250  GETMET	R21	R21	K31
      0x885C0173,  //  0251  GETMBR	R23	R0	K115
      0x7C540400,  //  0252  CALL	R21	2
      0x60580015,  //  0253  GETGBL	R22	G21
      0x7C580000,  //  0254  CALL	R22	0
      0x605C000C,  //  0255  GETGBL	R23	G12
      0x5C602200,  //  0256  MOVE	R24	R17
      0x7C5C0200,  //  0257  CALL	R23	1
      0x5462000F,  //  0258  LDINT	R24	16
      0x7C480C00,  //  0259  CALL	R18	6
      0x8C4C2574,  //  025A  GETMET	R19	R18	K116
      0x5C542200,  //  025B  MOVE	R21	R17
      0x7C4C0400,  //  025C  CALL	R19	2
      0x8C502528,  //  025D  GETMET	R20	R18	K40
      0x7C500200,  //  025E  CALL	R20	1
      0x004C2614,  //  025F  ADD	R19	R19	R20
      0xB8521A00,  //  0260  GETNGBL	R20	K13
      0x8C502975,  //  0261  GETMET	R20	R20	K117
      0x7C500200,  //  0262  CALL	R20	1
      0x9052EC09,  //  0263  SETMBR	R20	K118	R9
      0x88540934,  //  0264  GETMBR	R21	R4	K52
      0x9052EE15,  //  0265  SETMBR	R20	K119	R21
      0x88540961,  //  0266  GETMBR	R21	R4	K97
      0x9052F015,  //  0267  SETMBR	R20	K120	R21
      0x9052F213,  //  0268  SETMBR	R20	K121	R19
      0x8C54294C,  //  0269  GETMET	R21	R20	K76
      0x7C540200,  //  026A  CALL	R21	1
      0x9012F415,  //  026B  SETMBR	R4	K122	R21
      0x8C580350,  //  026C  GETMET	R22	R1	K80
      0x54620030,  //  026D  LDINT	R24	49
      0x50640200,  //  026E  LDBOOL	R25	1	0
      0x7C580600,  //  026F  CALL	R22	3
      0x8C5C2D51,  //  0270  GETMET	R23	R22	K81
      0x5C642A00,  //  0271  MOVE	R25	R21
      0x7C5C0400,  //  0272  CALL	R23	2
      0x88600152,  //  0273  GETMBR	R24	R0	K82
      0x8C603153,  //  0274  GETMET	R24	R24	K83
      0x5C682C00,  //  0275  MOVE	R26	R22
      0x7C600400,  //  0276  CALL	R24	2
      0x50600200,  //  0277  LDBOOL	R24	1	0
      0x80043000,  //  0278  RET	1	R24
      0x50200200,  //  0279  LDBOOL	R8	1	0
      0x80041000,  //  027A  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Commisioning_Context
********************************************************************/
be_local_class(Matter_Commisioning_Context,
    2,
    NULL,
    be_nested_map(20,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Commisioning_Context_every_second_closure) },
        { be_const_key_weak(parse_StatusReport, -1), be_const_closure(Matter_Commisioning_Context_parse_StatusReport_closure) },
        { be_const_key_weak(S2K_Info, -1), be_nested_str_weak(Sigma2) },
        { be_const_key_weak(find_fabric_by_destination_id, -1), be_const_closure(Matter_Commisioning_Context_find_fabric_by_destination_id_closure) },
        { be_const_key_weak(process_incoming, 18), be_const_closure(Matter_Commisioning_Context_process_incoming_closure) },
        { be_const_key_weak(Matter_Context_Prefix, 0), be_nested_str_weak(CHIP_X20PAKE_X20V1_X20Commissioning) },
        { be_const_key_weak(SEKeys_Info, -1), be_nested_str_weak(SessionKeys) },
        { be_const_key_weak(TBEData3_Nonce, 4), be_nested_str_weak(NCASE_Sigma3N) },
        { be_const_key_weak(parse_PBKDFParamRequest, -1), be_const_closure(Matter_Commisioning_Context_parse_PBKDFParamRequest_closure) },
        { be_const_key_weak(send_status_report, -1), be_const_closure(Matter_Commisioning_Context_send_status_report_closure) },
        { be_const_key_weak(parse_Sigma3, -1), be_const_closure(Matter_Commisioning_Context_parse_Sigma3_closure) },
        { be_const_key_weak(device, -1), be_const_var(1) },
        { be_const_key_weak(TBEData2_Nonce, -1), be_nested_str_weak(NCASE_Sigma2N) },
        { be_const_key_weak(responder, 16), be_const_var(0) },
        { be_const_key_weak(parse_Pake3, -1), be_const_closure(Matter_Commisioning_Context_parse_Pake3_closure) },
        { be_const_key_weak(init, 14), be_const_closure(Matter_Commisioning_Context_init_closure) },
        { be_const_key_weak(parse_Pake1, -1), be_const_closure(Matter_Commisioning_Context_parse_Pake1_closure) },
        { be_const_key_weak(S3K_Info, 10), be_nested_str_weak(Sigma3) },
        { be_const_key_weak(add_session, -1), be_const_closure(Matter_Commisioning_Context_add_session_closure) },
        { be_const_key_weak(parse_Sigma1, -1), be_const_closure(Matter_Commisioning_Context_parse_Sigma1_closure) },
    })),
    be_str_weak(Matter_Commisioning_Context)
);
/*******************************************************************/

void be_load_Matter_Commisioning_Context_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Commisioning_Context);
    be_setglobal(vm, "Matter_Commisioning_Context");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
