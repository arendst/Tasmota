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
      0x50140000,  //  001E  LDBOOL	R5	0	0
      0x80040A00,  //  001F  RET	1	R5
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
      0x50180000,  //  0043  LDBOOL	R6	0	0
      0x80040C00,  //  0044  RET	1	R6
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
      0x50100000,  //  001D  LDBOOL	R4	0	0
      0x80040800,  //  001E  RET	1	R4
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
    19,                          /* nstack */
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
      0x50100000,  //  001D  LDBOOL	R4	0	0
      0x80040800,  //  001E  RET	1	R4
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
      0x50180000,  //  003B  LDBOOL	R6	0	0
      0x80040C00,  //  003C  RET	1	R6
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
      0x8C2C011C,  //  0060  GETMET	R11	R0	K28
      0x8834071D,  //  0061  GETMBR	R13	R3	K29
      0x8838071E,  //  0062  GETMBR	R14	R3	K30
      0x5C3C1000,  //  0063  MOVE	R15	R8
      0x5C401200,  //  0064  MOVE	R16	R9
      0x5C441400,  //  0065  MOVE	R17	R10
      0x5C480C00,  //  0066  MOVE	R18	R6
      0x7C2C0E00,  //  0067  CALL	R11	7
      0x502C0200,  //  0068  LDBOOL	R11	1	0
      0x80041600,  //  0069  RET	1	R11
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
    /* K36  */  be_nested_str_weak(findsubval),
    /* K37  */  be_const_int(3),
    /* K38  */  be_nested_str_weak(MTR_X3A_X20initiatorNOCTLV_X20_X3D_X20),
    /* K39  */  be_nested_str_weak(findsub),
    /* K40  */  be_nested_str_weak(int),
    /* K41  */  be_nested_str_weak(peer_node_id),
    /* K42  */  be_nested_str_weak(int64),
    /* K43  */  be_nested_str_weak(fromu32),
    /* K44  */  be_nested_str_weak(tobytes),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20initiatorFabricId_X3D),
    /* K46  */  be_nested_str_weak(Matter_TLV_struct),
    /* K47  */  be_nested_str_weak(add_TLV),
    /* K48  */  be_nested_str_weak(B1),
    /* K49  */  be_nested_str_weak(__initiator_pub),
    /* K50  */  be_nested_str_weak(__responder_pub),
    /* K51  */  be_nested_str_weak(tlv2raw),
    /* K52  */  be_nested_str_weak(EC_P256),
    /* K53  */  be_nested_str_weak(ecdsa_verify_sha256),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20sigma3_tbs_X20does_X20not_X20have_X20a_X20valid_X20signature),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X20Invalid_X20signature_X2C_X20trying_X20anyways),
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
    ( &(const binstruction[303]) {  /* code */
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
      0x500C0000,  //  0017  LDBOOL	R3	0	0
      0x80040600,  //  0018  RET	1	R3
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
      0x50340000,  //  006A  LDBOOL	R13	0	0
      0x80041A00,  //  006B  RET	1	R13
      0xB8361800,  //  006C  GETNGBL	R13	K12
      0x88341B23,  //  006D  GETMBR	R13	R13	K35
      0x8C341B0E,  //  006E  GETMET	R13	R13	K14
      0x5C3C1600,  //  006F  MOVE	R15	R11
      0x7C340400,  //  0070  CALL	R13	2
      0x8C381B24,  //  0071  GETMET	R14	R13	K36
      0x5840000A,  //  0072  LDCONST	R16	K10
      0x7C380400,  //  0073  CALL	R14	2
      0x8C3C1B24,  //  0074  GETMET	R15	R13	K36
      0x58440008,  //  0075  LDCONST	R17	K8
      0x7C3C0400,  //  0076  CALL	R15	2
      0x8C401B24,  //  0077  GETMET	R16	R13	K36
      0x58480025,  //  0078  LDCONST	R18	K37
      0x7C400400,  //  0079  CALL	R16	2
      0xB8461800,  //  007A  GETNGBL	R17	K12
      0x88442323,  //  007B  GETMBR	R17	R17	K35
      0x8C44230E,  //  007C  GETMET	R17	R17	K14
      0x5C4C1C00,  //  007D  MOVE	R19	R14
      0x7C440400,  //  007E  CALL	R17	2
      0xB84A0A00,  //  007F  GETNGBL	R18	K5
      0x8C482506,  //  0080  GETMET	R18	R18	K6
      0x60500008,  //  0081  GETGBL	R20	G8
      0x5C542200,  //  0082  MOVE	R21	R17
      0x7C500200,  //  0083  CALL	R20	1
      0x00524C14,  //  0084  ADD	R20	K38	R20
      0x58540025,  //  0085  LDCONST	R21	K37
      0x7C480600,  //  0086  CALL	R18	3
      0x8C482324,  //  0087  GETMET	R18	R17	K36
      0x54520008,  //  0088  LDINT	R20	9
      0x7C480400,  //  0089  CALL	R18	2
      0x8C4C2327,  //  008A  GETMET	R19	R17	K39
      0x54560005,  //  008B  LDINT	R21	6
      0x7C4C0400,  //  008C  CALL	R19	2
      0x8C502724,  //  008D  GETMET	R20	R19	K36
      0x545A0010,  //  008E  LDINT	R22	17
      0x7C500400,  //  008F  CALL	R20	2
      0x60540004,  //  0090  GETGBL	R21	G4
      0x5C582800,  //  0091  MOVE	R22	R20
      0x7C540200,  //  0092  CALL	R21	1
      0x1C542B28,  //  0093  EQ	R21	R21	K40
      0x78560007,  //  0094  JMPF	R21	#009D
      0xB8565400,  //  0095  GETNGBL	R21	K42
      0x8C542B2B,  //  0096  GETMET	R21	R21	K43
      0x5C5C2800,  //  0097  MOVE	R23	R20
      0x7C540400,  //  0098  CALL	R21	2
      0x8C542B2C,  //  0099  GETMET	R21	R21	K44
      0x7C540200,  //  009A  CALL	R21	1
      0x900E5215,  //  009B  SETMBR	R3	K41	R21
      0x70020002,  //  009C  JMP		#00A0
      0x8C54292C,  //  009D  GETMET	R21	R20	K44
      0x7C540200,  //  009E  CALL	R21	1
      0x900E5215,  //  009F  SETMBR	R3	K41	R21
      0xB8560A00,  //  00A0  GETNGBL	R21	K5
      0x8C542B06,  //  00A1  GETMET	R21	R21	K6
      0x605C0008,  //  00A2  GETGBL	R23	G8
      0x88600729,  //  00A3  GETMBR	R24	R3	K41
      0x7C5C0200,  //  00A4  CALL	R23	1
      0x005E5A17,  //  00A5  ADD	R23	K45	R23
      0x58600025,  //  00A6  LDCONST	R24	K37
      0x7C540600,  //  00A7  CALL	R21	3
      0xB8561800,  //  00A8  GETNGBL	R21	K12
      0x88542B23,  //  00A9  GETMBR	R21	R21	K35
      0x8C542B2E,  //  00AA  GETMET	R21	R21	K46
      0x7C540200,  //  00AB  CALL	R21	1
      0x8C582B2F,  //  00AC  GETMET	R22	R21	K47
      0x5860000A,  //  00AD  LDCONST	R24	K10
      0xB8661800,  //  00AE  GETNGBL	R25	K12
      0x88643323,  //  00AF  GETMBR	R25	R25	K35
      0x88643330,  //  00B0  GETMBR	R25	R25	K48
      0x5C681C00,  //  00B1  MOVE	R26	R14
      0x7C580800,  //  00B2  CALL	R22	4
      0x8C582B2F,  //  00B3  GETMET	R22	R21	K47
      0x58600008,  //  00B4  LDCONST	R24	K8
      0xB8661800,  //  00B5  GETNGBL	R25	K12
      0x88643323,  //  00B6  GETMBR	R25	R25	K35
      0x88643330,  //  00B7  GETMBR	R25	R25	K48
      0x5C681E00,  //  00B8  MOVE	R26	R15
      0x7C580800,  //  00B9  CALL	R22	4
      0x8C582B2F,  //  00BA  GETMET	R22	R21	K47
      0x58600025,  //  00BB  LDCONST	R24	K37
      0xB8661800,  //  00BC  GETNGBL	R25	K12
      0x88643323,  //  00BD  GETMBR	R25	R25	K35
      0x88643330,  //  00BE  GETMBR	R25	R25	K48
      0x88680731,  //  00BF  GETMBR	R26	R3	K49
      0x7C580800,  //  00C0  CALL	R22	4
      0x8C582B2F,  //  00C1  GETMET	R22	R21	K47
      0x54620003,  //  00C2  LDINT	R24	4
      0xB8661800,  //  00C3  GETNGBL	R25	K12
      0x88643323,  //  00C4  GETMBR	R25	R25	K35
      0x88643330,  //  00C5  GETMBR	R25	R25	K48
      0x88680732,  //  00C6  GETMBR	R26	R3	K50
      0x7C580800,  //  00C7  CALL	R22	4
      0x8C582B33,  //  00C8  GETMET	R22	R21	K51
      0x7C580200,  //  00C9  CALL	R22	1
      0x8C5C0534,  //  00CA  GETMET	R23	R2	K52
      0x7C5C0200,  //  00CB  CALL	R23	1
      0x8C5C2F35,  //  00CC  GETMET	R23	R23	K53
      0x5C642400,  //  00CD  MOVE	R25	R18
      0x5C682C00,  //  00CE  MOVE	R26	R22
      0x5C6C2000,  //  00CF  MOVE	R27	R16
      0x7C5C0800,  //  00D0  CALL	R23	4
      0x5C602E00,  //  00D1  MOVE	R24	R23
      0x7462000A,  //  00D2  JMPT	R24	#00DE
      0xB8620A00,  //  00D3  GETNGBL	R24	K5
      0x8C603106,  //  00D4  GETMET	R24	R24	K6
      0x58680036,  //  00D5  LDCONST	R26	K54
      0x586C0008,  //  00D6  LDCONST	R27	K8
      0x7C600600,  //  00D7  CALL	R24	3
      0xB8620A00,  //  00D8  GETNGBL	R24	K5
      0x8C603106,  //  00D9  GETMET	R24	R24	K6
      0x58680037,  //  00DA  LDCONST	R26	K55
      0x586C0008,  //  00DB  LDCONST	R27	K8
      0x7C600600,  //  00DC  CALL	R24	3
      0x70020004,  //  00DD  JMP		#00E3
      0xB8620A00,  //  00DE  GETNGBL	R24	K5
      0x8C603106,  //  00DF  GETMET	R24	R24	K6
      0x58680038,  //  00E0  LDCONST	R26	K56
      0x586C0025,  //  00E1  LDCONST	R27	K37
      0x7C600600,  //  00E2  CALL	R24	3
      0x8C600511,  //  00E3  GETMET	R24	R2	K17
      0x7C600200,  //  00E4  CALL	R24	1
      0x8C603112,  //  00E5  GETMET	R24	R24	K18
      0x88680713,  //  00E6  GETMBR	R26	R3	K19
      0x7C600400,  //  00E7  CALL	R24	2
      0x8C603112,  //  00E8  GETMET	R24	R24	K18
      0x88680714,  //  00E9  GETMBR	R26	R3	K20
      0x7C600400,  //  00EA  CALL	R24	2
      0x8C603112,  //  00EB  GETMET	R24	R24	K18
      0x88680939,  //  00EC  GETMBR	R26	R4	K57
      0x7C600400,  //  00ED  CALL	R24	2
      0x8C603115,  //  00EE  GETMET	R24	R24	K21
      0x7C600200,  //  00EF  CALL	R24	1
      0x5C143000,  //  00F0  MOVE	R5	R24
      0x4C600000,  //  00F1  LDNIL	R24
      0x900E2618,  //  00F2  SETMBR	R3	K19	R24
      0x4C600000,  //  00F3  LDNIL	R24
      0x900E2818,  //  00F4  SETMBR	R3	K20	R24
      0x8C600518,  //  00F5  GETMET	R24	R2	K24
      0x7C600200,  //  00F6  CALL	R24	1
      0x8C603119,  //  00F7  GETMET	R24	R24	K25
      0x8868071A,  //  00F8  GETMBR	R26	R3	K26
      0x8C6C071B,  //  00F9  GETMET	R27	R3	K27
      0x7C6C0200,  //  00FA  CALL	R27	1
      0x006C3605,  //  00FB  ADD	R27	R27	R5
      0x60700015,  //  00FC  GETGBL	R28	G21
      0x7C700000,  //  00FD  CALL	R28	0
      0x8C703916,  //  00FE  GETMET	R28	R28	K22
      0x8878013A,  //  00FF  GETMBR	R30	R0	K58
      0x7C700400,  //  0100  CALL	R28	2
      0x5476002F,  //  0101  LDINT	R29	48
      0x7C600A00,  //  0102  CALL	R24	5
      0x5466000E,  //  0103  LDINT	R25	15
      0x40660619,  //  0104  CONNECT	R25	K3	R25
      0x94643019,  //  0105  GETIDX	R25	R24	R25
      0x546A000F,  //  0106  LDINT	R26	16
      0x546E001E,  //  0107  LDINT	R27	31
      0x4068341B,  //  0108  CONNECT	R26	R26	R27
      0x9468301A,  //  0109  GETIDX	R26	R24	R26
      0x546E001F,  //  010A  LDINT	R27	32
      0x5472002E,  //  010B  LDINT	R28	47
      0x406C361C,  //  010C  CONNECT	R27	R27	R28
      0x946C301B,  //  010D  GETIDX	R27	R24	R27
      0xB8720A00,  //  010E  GETNGBL	R28	K5
      0x8C70393B,  //  010F  GETMET	R28	R28	K59
      0x7C700200,  //  0110  CALL	R28	1
      0x9470393C,  //  0111  GETIDX	R28	R28	K60
      0x8C740109,  //  0112  GETMET	R29	R0	K9
      0x5C7C0200,  //  0113  MOVE	R31	R1
      0x58800003,  //  0114  LDCONST	R32	K3
      0x58840003,  //  0115  LDCONST	R33	K3
      0x58880003,  //  0116  LDCONST	R34	K3
      0x508C0200,  //  0117  LDBOOL	R35	1	0
      0x7C740C00,  //  0118  CALL	R29	6
      0x8C74073D,  //  0119  GETMET	R29	R3	K61
      0x7C740200,  //  011A  CALL	R29	1
      0x8C74073E,  //  011B  GETMET	R29	R3	K62
      0x5C7C3200,  //  011C  MOVE	R31	R25
      0x5C803400,  //  011D  MOVE	R32	R26
      0x5C843600,  //  011E  MOVE	R33	R27
      0x5C883800,  //  011F  MOVE	R34	R28
      0x7C740A00,  //  0120  CALL	R29	5
      0x900E7F03,  //  0121  SETMBR	R3	K63	K3
      0x8C740740,  //  0122  GETMET	R29	R3	K64
      0x7C740200,  //  0123  CALL	R29	1
      0x8C740741,  //  0124  GETMET	R29	R3	K65
      0x507C0200,  //  0125  LDBOOL	R31	1	0
      0x7C740400,  //  0126  CALL	R29	2
      0x8C740742,  //  0127  GETMET	R29	R3	K66
      0x7C740200,  //  0128  CALL	R29	1
      0x8C740743,  //  0129  GETMET	R29	R3	K67
      0x7C740200,  //  012A  CALL	R29	1
      0x8C740744,  //  012B  GETMET	R29	R3	K68
      0x7C740200,  //  012C  CALL	R29	1
      0x50740200,  //  012D  LDBOOL	R29	1	0
      0x80043A00,  //  012E  RET	1	R29
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
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(is_commissioning_open),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20commissioning_X20not_X20open),
    /* K6   */  be_const_int(2),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20received_X20message_X20),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(inspect),
    /* K10  */  be_nested_str_weak(parse_PBKDFParamRequest),
    /* K11  */  be_nested_str_weak(parse_Pake1),
    /* K12  */  be_nested_str_weak(parse_Pake3),
    /* K13  */  be_nested_str_weak(parse_Sigma1),
    /* K14  */  be_nested_str_weak(parse_Sigma3),
    /* K15  */  be_nested_str_weak(parse_StatusReport),
    /* K16  */  be_nested_str_weak(string),
    /* K17  */  be_nested_str_weak(format),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28secure_X20channel_X29_X20_X2502X),
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[100]) {  /* code */
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
      0xB80A0600,  //  0013  GETNGBL	R2	K3
      0x8C080504,  //  0014  GETMET	R2	R2	K4
      0xB8121000,  //  0015  GETNGBL	R4	K8
      0x8C100909,  //  0016  GETMET	R4	R4	K9
      0x5C180200,  //  0017  MOVE	R6	R1
      0x7C100400,  //  0018  CALL	R4	2
      0x00120E04,  //  0019  ADD	R4	K7	R4
      0x54160003,  //  001A  LDINT	R5	4
      0x7C080600,  //  001B  CALL	R2	3
      0x88080302,  //  001C  GETMBR	R2	R1	K2
      0x540E000F,  //  001D  LDINT	R3	16
      0x1C080403,  //  001E  EQ	R2	R2	R3
      0x780A0000,  //  001F  JMPF	R2	#0021
      0x70020040,  //  0020  JMP		#0062
      0x88080302,  //  0021  GETMBR	R2	R1	K2
      0x540E001F,  //  0022  LDINT	R3	32
      0x1C080403,  //  0023  EQ	R2	R2	R3
      0x780A0004,  //  0024  JMPF	R2	#002A
      0x8C08010A,  //  0025  GETMET	R2	R0	K10
      0x5C100200,  //  0026  MOVE	R4	R1
      0x7C080400,  //  0027  CALL	R2	2
      0x80040400,  //  0028  RET	1	R2
      0x70020037,  //  0029  JMP		#0062
      0x88080302,  //  002A  GETMBR	R2	R1	K2
      0x540E0021,  //  002B  LDINT	R3	34
      0x1C080403,  //  002C  EQ	R2	R2	R3
      0x780A0004,  //  002D  JMPF	R2	#0033
      0x8C08010B,  //  002E  GETMET	R2	R0	K11
      0x5C100200,  //  002F  MOVE	R4	R1
      0x7C080400,  //  0030  CALL	R2	2
      0x80040400,  //  0031  RET	1	R2
      0x7002002E,  //  0032  JMP		#0062
      0x88080302,  //  0033  GETMBR	R2	R1	K2
      0x540E0023,  //  0034  LDINT	R3	36
      0x1C080403,  //  0035  EQ	R2	R2	R3
      0x780A0004,  //  0036  JMPF	R2	#003C
      0x8C08010C,  //  0037  GETMET	R2	R0	K12
      0x5C100200,  //  0038  MOVE	R4	R1
      0x7C080400,  //  0039  CALL	R2	2
      0x80040400,  //  003A  RET	1	R2
      0x70020025,  //  003B  JMP		#0062
      0x88080302,  //  003C  GETMBR	R2	R1	K2
      0x540E002F,  //  003D  LDINT	R3	48
      0x1C080403,  //  003E  EQ	R2	R2	R3
      0x780A0004,  //  003F  JMPF	R2	#0045
      0x8C08010D,  //  0040  GETMET	R2	R0	K13
      0x5C100200,  //  0041  MOVE	R4	R1
      0x7C080400,  //  0042  CALL	R2	2
      0x80040400,  //  0043  RET	1	R2
      0x7002001C,  //  0044  JMP		#0062
      0x88080302,  //  0045  GETMBR	R2	R1	K2
      0x540E0031,  //  0046  LDINT	R3	50
      0x1C080403,  //  0047  EQ	R2	R2	R3
      0x780A0004,  //  0048  JMPF	R2	#004E
      0x8C08010E,  //  0049  GETMET	R2	R0	K14
      0x5C100200,  //  004A  MOVE	R4	R1
      0x7C080400,  //  004B  CALL	R2	2
      0x80040400,  //  004C  RET	1	R2
      0x70020013,  //  004D  JMP		#0062
      0x88080302,  //  004E  GETMBR	R2	R1	K2
      0x540E003F,  //  004F  LDINT	R3	64
      0x1C080403,  //  0050  EQ	R2	R2	R3
      0x780A0004,  //  0051  JMPF	R2	#0057
      0x8C08010F,  //  0052  GETMET	R2	R0	K15
      0x5C100200,  //  0053  MOVE	R4	R1
      0x7C080400,  //  0054  CALL	R2	2
      0x80040400,  //  0055  RET	1	R2
      0x7002000A,  //  0056  JMP		#0062
      0xA40A2000,  //  0057  IMPORT	R2	K16
      0xB80E0600,  //  0058  GETNGBL	R3	K3
      0x8C0C0704,  //  0059  GETMET	R3	R3	K4
      0x8C140511,  //  005A  GETMET	R5	R2	K17
      0x581C0012,  //  005B  LDCONST	R7	K18
      0x88200302,  //  005C  GETMBR	R8	R1	K2
      0x7C140600,  //  005D  CALL	R5	3
      0x58180006,  //  005E  LDCONST	R6	K6
      0x7C0C0600,  //  005F  CALL	R3	3
      0x500C0000,  //  0060  LDBOOL	R3	0	0
      0x80040600,  //  0061  RET	1	R3
      0x50080000,  //  0062  LDBOOL	R2	0	0
      0x80040400,  //  0063  RET	1	R2
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
    ( &(const bvalue[130]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(format),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20sigma1_X3D_X25s),
    /* K20  */  be_nested_str_weak(inspect),
    /* K21  */  be_nested_str_weak(__initiator_pub),
    /* K22  */  be_nested_str_weak(initiatorEphPubKey),
    /* K23  */  be_nested_str_weak(resumptionID),
    /* K24  */  be_nested_str_weak(initiatorResumeMIC),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20is_resumption_X3D_X25i),
    /* K26  */  be_nested_str_weak(device),
    /* K27  */  be_nested_str_weak(sessions),
    /* K28  */  be_nested_str_weak(find_session_by_resumption_id),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20session_resumption_X20found_X20session_X3D_X25s_X20session_resumption_X3D_X25s),
    /* K30  */  be_nested_str_weak(_fabric),
    /* K31  */  be_nested_str_weak(initiatorRandom),
    /* K32  */  be_nested_str_weak(fromstring),
    /* K33  */  be_nested_str_weak(Sigma1_Resume),
    /* K34  */  be_nested_str_weak(HKDF_SHA256),
    /* K35  */  be_nested_str_weak(derive),
    /* K36  */  be_nested_str_weak(shared_secret),
    /* K37  */  be_nested_str_weak(NCASE_SigmaS1),
    /* K38  */  be_const_int(2147483647),
    /* K39  */  be_nested_str_weak(AES_CCM),
    /* K40  */  be_nested_str_weak(decrypt),
    /* K41  */  be_nested_str_weak(tag),
    /* K42  */  be_nested_str_weak(_source_node_id),
    /* K43  */  be_nested_str_weak(source_node_id),
    /* K44  */  be_nested_str_weak(set_mode_CASE),
    /* K45  */  be_nested_str_weak(__future_initiator_session_id),
    /* K46  */  be_nested_str_weak(initiator_session_id),
    /* K47  */  be_nested_str_weak(__future_local_session_id),
    /* K48  */  be_nested_str_weak(gen_local_session_id),
    /* K49  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K50  */  be_nested_str_weak(remote_ip),
    /* K51  */  be_nested_str_weak(remote_port),
    /* K52  */  be_nested_str_weak(resumption_id),
    /* K53  */  be_nested_str_weak(random),
    /* K54  */  be_nested_str_weak(Sigma2_Resume),
    /* K55  */  be_nested_str_weak(NCASE_SigmaS2),
    /* K56  */  be_nested_str_weak(Sigma2Resume),
    /* K57  */  be_nested_str_weak(responderSessionID),
    /* K58  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K59  */  be_nested_str_weak(SessionResumptionKeys),
    /* K60  */  be_nested_str_weak(rtc),
    /* K61  */  be_nested_str_weak(utc),
    /* K62  */  be_nested_str_weak(tlv2raw),
    /* K63  */  be_nested_str_weak(__Msg1),
    /* K64  */  be_nested_str_weak(build_response),
    /* K65  */  be_nested_str_weak(encode_frame),
    /* K66  */  be_nested_str_weak(responder),
    /* K67  */  be_nested_str_weak(send_response_frame),
    /* K68  */  be_nested_str_weak(close),
    /* K69  */  be_nested_str_weak(set_keys),
    /* K70  */  be_nested_str_weak(_breadcrumb),
    /* K71  */  be_nested_str_weak(counter_snd_next),
    /* K72  */  be_nested_str_weak(set_persist),
    /* K73  */  be_nested_str_weak(set_no_expiration),
    /* K74  */  be_nested_str_weak(persist_to_fabric),
    /* K75  */  be_nested_str_weak(save),
    /* K76  */  be_nested_str_weak(find_fabric_by_destination_id),
    /* K77  */  be_nested_str_weak(destinationId),
    /* K78  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K79  */  be_nested_str_weak(MTR_X3A_X20fabric_X3D),
    /* K80  */  be_nested_str_weak(MTR_X3A_X20no_private_key_X3D),
    /* K81  */  be_nested_str_weak(get_pk),
    /* K82  */  be_nested_str_weak(tohex),
    /* K83  */  be_nested_str_weak(MTR_X3A_X20noc_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K84  */  be_nested_str_weak(get_noc),
    /* K85  */  be_nested_str_weak(get_icac),
    /* K86  */  be_nested_str_weak(MTR_X3A_X20icac_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K87  */  be_nested_str_weak(MTR_X3A_X20root_ca_cert_X20_X20_X3D),
    /* K88  */  be_nested_str_weak(get_ca),
    /* K89  */  be_nested_str_weak(__responder_priv),
    /* K90  */  be_nested_str_weak(__responder_pub),
    /* K91  */  be_nested_str_weak(EC_P256),
    /* K92  */  be_nested_str_weak(public_key),
    /* K93  */  be_nested_str_weak(MTR_X3A_X20ResponderEph_priv_X20_X20_X3D),
    /* K94  */  be_nested_str_weak(MTR_X3A_X20ResponderEph_pub_X20_X20_X3D),
    /* K95  */  be_nested_str_weak(shared_key),
    /* K96  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20shared_secret_X20_X20_X3D_X20),
    /* K97  */  be_nested_str_weak(TLV),
    /* K98  */  be_nested_str_weak(Matter_TLV_struct),
    /* K99  */  be_nested_str_weak(add_TLV),
    /* K100 */  be_nested_str_weak(B2),
    /* K101 */  be_const_int(3),
    /* K102 */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K103 */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K104 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20fabric_X2Eget_pk_X20_X20_X3D_X20),
    /* K105 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20sigma2_tbsdata_X20_X20_X3D_X20),
    /* K106 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBSData2Signature_X20_X20_X3D_X20),
    /* K107 */  be_nested_str_weak(Msg1),
    /* K108 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20resumptionid_X20_X20_X3D_X20),
    /* K109 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20MSG1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K110 */  be_nested_str_weak(SHA256),
    /* K111 */  be_nested_str_weak(update),
    /* K112 */  be_nested_str_weak(out),
    /* K113 */  be_nested_str_weak(MTR_X3A_X20TranscriptHash_X20_X3D),
    /* K114 */  be_nested_str_weak(S2K_Info),
    /* K115 */  be_nested_str_weak(get_ipk_group_key),
    /* K116 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20SharedSecret_X20_X20_X3D_X20),
    /* K117 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K118 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K119 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Raw_X20_X20_X20_X3D_X20),
    /* K120 */  be_nested_str_weak(TBEData2_Nonce),
    /* K121 */  be_nested_str_weak(encrypt),
    /* K122 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Enc_X20_X20_X20_X3D_X20),
    /* K123 */  be_nested_str_weak(Sigma2),
    /* K124 */  be_nested_str_weak(responderRandom),
    /* K125 */  be_nested_str_weak(responderSessionId),
    /* K126 */  be_nested_str_weak(responderEphPubKey),
    /* K127 */  be_nested_str_weak(encrypted2),
    /* K128 */  be_nested_str_weak(MTR_X3A_X20sigma2_X3A_X20),
    /* K129 */  be_nested_str_weak(__Msg2),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[689]) {  /* code */
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
      0x50140000,  //  0019  LDBOOL	R5	0	0
      0x80040A00,  //  001A  RET	1	R5
      0xB8161A00,  //  001B  GETNGBL	R5	K13
      0x8C140B0E,  //  001C  GETMET	R5	R5	K14
      0x7C140200,  //  001D  CALL	R5	1
      0x8C140B0F,  //  001E  GETMET	R5	R5	K15
      0x881C0310,  //  001F  GETMBR	R7	R1	K16
      0x88200311,  //  0020  GETMBR	R8	R1	K17
      0x7C140600,  //  0021  CALL	R5	3
      0xB81A0E00,  //  0022  GETNGBL	R6	K7
      0x8C180D08,  //  0023  GETMET	R6	R6	K8
      0x8C200712,  //  0024  GETMET	R8	R3	K18
      0x58280013,  //  0025  LDCONST	R10	K19
      0xB82E1A00,  //  0026  GETNGBL	R11	K13
      0x8C2C1714,  //  0027  GETMET	R11	R11	K20
      0x5C340A00,  //  0028  MOVE	R13	R5
      0x7C2C0400,  //  0029  CALL	R11	2
      0x7C200600,  //  002A  CALL	R8	3
      0x54260003,  //  002B  LDINT	R9	4
      0x7C180600,  //  002C  CALL	R6	3
      0x88180B16,  //  002D  GETMBR	R6	R5	K22
      0x90122A06,  //  002E  SETMBR	R4	K21	R6
      0x88180B17,  //  002F  GETMBR	R6	R5	K23
      0x4C1C0000,  //  0030  LDNIL	R7
      0x20180C07,  //  0031  NE	R6	R6	R7
      0x781A0003,  //  0032  JMPF	R6	#0037
      0x88180B18,  //  0033  GETMBR	R6	R5	K24
      0x4C1C0000,  //  0034  LDNIL	R7
      0x20180C07,  //  0035  NE	R6	R6	R7
      0x741A0000,  //  0036  JMPT	R6	#0038
      0x50180001,  //  0037  LDBOOL	R6	0	1
      0x50180200,  //  0038  LDBOOL	R6	1	0
      0xB81E0E00,  //  0039  GETNGBL	R7	K7
      0x8C1C0F08,  //  003A  GETMET	R7	R7	K8
      0x8C240712,  //  003B  GETMET	R9	R3	K18
      0x582C0019,  //  003C  LDCONST	R11	K25
      0x781A0001,  //  003D  JMPF	R6	#0040
      0x5830000C,  //  003E  LDCONST	R12	K12
      0x70020000,  //  003F  JMP		#0041
      0x58300005,  //  0040  LDCONST	R12	K5
      0x7C240600,  //  0041  CALL	R9	3
      0x542A0003,  //  0042  LDINT	R10	4
      0x7C1C0600,  //  0043  CALL	R7	3
      0x50180000,  //  0044  LDBOOL	R6	0	0
      0x4C1C0000,  //  0045  LDNIL	R7
      0x781A001C,  //  0046  JMPF	R6	#0064
      0x8820011A,  //  0047  GETMBR	R8	R0	K26
      0x8820111B,  //  0048  GETMBR	R8	R8	K27
      0x8C20111C,  //  0049  GETMET	R8	R8	K28
      0x88280B17,  //  004A  GETMBR	R10	R5	K23
      0x7C200400,  //  004B  CALL	R8	2
      0x5C1C1000,  //  004C  MOVE	R7	R8
      0xB8220E00,  //  004D  GETNGBL	R8	K7
      0x8C201108,  //  004E  GETMET	R8	R8	K8
      0x8C280712,  //  004F  GETMET	R10	R3	K18
      0x5830001D,  //  0050  LDCONST	R12	K29
      0xB8361A00,  //  0051  GETNGBL	R13	K13
      0x8C341B14,  //  0052  GETMET	R13	R13	K20
      0x5C3C0800,  //  0053  MOVE	R15	R4
      0x7C340400,  //  0054  CALL	R13	2
      0xB83A1A00,  //  0055  GETNGBL	R14	K13
      0x8C381D14,  //  0056  GETMET	R14	R14	K20
      0x5C400E00,  //  0057  MOVE	R16	R7
      0x7C380400,  //  0058  CALL	R14	2
      0x7C280800,  //  0059  CALL	R10	4
      0x542E0003,  //  005A  LDINT	R11	4
      0x7C200600,  //  005B  CALL	R8	3
      0x4C200000,  //  005C  LDNIL	R8
      0x1C200E08,  //  005D  EQ	R8	R7	R8
      0x74220003,  //  005E  JMPT	R8	#0063
      0x88200F1E,  //  005F  GETMBR	R8	R7	K30
      0x4C240000,  //  0060  LDNIL	R9
      0x1C201009,  //  0061  EQ	R8	R8	R9
      0x78220000,  //  0062  JMPF	R8	#0064
      0x50180000,  //  0063  LDBOOL	R6	0	0
      0x781A00B6,  //  0064  JMPF	R6	#011C
      0x88200B1F,  //  0065  GETMBR	R8	R5	K31
      0x88240B17,  //  0066  GETMBR	R9	R5	K23
      0x00201009,  //  0067  ADD	R8	R8	R9
      0x60240015,  //  0068  GETGBL	R9	G21
      0x7C240000,  //  0069  CALL	R9	0
      0x8C241320,  //  006A  GETMET	R9	R9	K32
      0x582C0021,  //  006B  LDCONST	R11	K33
      0x7C240400,  //  006C  CALL	R9	2
      0x8C280522,  //  006D  GETMET	R10	R2	K34
      0x7C280200,  //  006E  CALL	R10	1
      0x8C281523,  //  006F  GETMET	R10	R10	K35
      0x88300F24,  //  0070  GETMBR	R12	R7	K36
      0x5C341000,  //  0071  MOVE	R13	R8
      0x5C381200,  //  0072  MOVE	R14	R9
      0x543E000F,  //  0073  LDINT	R15	16
      0x7C280A00,  //  0074  CALL	R10	5
      0x602C0015,  //  0075  GETGBL	R11	G21
      0x7C2C0000,  //  0076  CALL	R11	0
      0x8C2C1720,  //  0077  GETMET	R11	R11	K32
      0x58340025,  //  0078  LDCONST	R13	K37
      0x7C2C0400,  //  0079  CALL	R11	2
      0x5431FFEE,  //  007A  LDINT	R12	-17
      0x40320A0C,  //  007B  CONNECT	R12	K5	R12
      0x88340B18,  //  007C  GETMBR	R13	R5	K24
      0x94301A0C,  //  007D  GETIDX	R12	R13	R12
      0x5439FFEF,  //  007E  LDINT	R14	-16
      0x40381D26,  //  007F  CONNECT	R14	R14	K38
      0x883C0B18,  //  0080  GETMBR	R15	R5	K24
      0x94341E0E,  //  0081  GETIDX	R13	R15	R14
      0x8C400527,  //  0082  GETMET	R16	R2	K39
      0x5C481400,  //  0083  MOVE	R18	R10
      0x5C4C1600,  //  0084  MOVE	R19	R11
      0x60500015,  //  0085  GETGBL	R20	G21
      0x7C500000,  //  0086  CALL	R20	0
      0x6054000C,  //  0087  GETGBL	R21	G12
      0x5C581800,  //  0088  MOVE	R22	R12
      0x7C540200,  //  0089  CALL	R21	1
      0x545A000F,  //  008A  LDINT	R22	16
      0x7C400C00,  //  008B  CALL	R16	6
      0x5C382000,  //  008C  MOVE	R14	R16
      0x8C401D28,  //  008D  GETMET	R16	R14	K40
      0x5C481800,  //  008E  MOVE	R18	R12
      0x7C400400,  //  008F  CALL	R16	2
      0x5C3C2000,  //  0090  MOVE	R15	R16
      0x8C401D29,  //  0091  GETMET	R16	R14	K41
      0x7C400200,  //  0092  CALL	R16	1
      0x1C441A10,  //  0093  EQ	R17	R13	R16
      0x78460085,  //  0094  JMPF	R17	#011B
      0x88440F1E,  //  0095  GETMBR	R17	R7	K30
      0x90123C11,  //  0096  SETMBR	R4	K30	R17
      0x8844032B,  //  0097  GETMBR	R17	R1	K43
      0x90125411,  //  0098  SETMBR	R4	K42	R17
      0x8C44092C,  //  0099  GETMET	R17	R4	K44
      0x7C440200,  //  009A  CALL	R17	1
      0x88440B2E,  //  009B  GETMBR	R17	R5	K46
      0x90125A11,  //  009C  SETMBR	R4	K45	R17
      0x8844011A,  //  009D  GETMBR	R17	R0	K26
      0x8844231B,  //  009E  GETMBR	R17	R17	K27
      0x8C442330,  //  009F  GETMET	R17	R17	K48
      0x7C440200,  //  00A0  CALL	R17	1
      0x90125E11,  //  00A1  SETMBR	R4	K47	R17
      0xB8460E00,  //  00A2  GETNGBL	R17	K7
      0x8C442308,  //  00A3  GETMET	R17	R17	K8
      0x8C4C0712,  //  00A4  GETMET	R19	R3	K18
      0x58540031,  //  00A5  LDCONST	R21	K49
      0x8858092F,  //  00A6  GETMBR	R22	R4	K47
      0x885C0332,  //  00A7  GETMBR	R23	R1	K50
      0x88600333,  //  00A8  GETMBR	R24	R1	K51
      0x7C4C0A00,  //  00A9  CALL	R19	5
      0x5850000A,  //  00AA  LDCONST	R20	K10
      0x7C440600,  //  00AB  CALL	R17	3
      0x88440F24,  //  00AC  GETMBR	R17	R7	K36
      0x90124811,  //  00AD  SETMBR	R4	K36	R17
      0x8C440535,  //  00AE  GETMET	R17	R2	K53
      0x544E000F,  //  00AF  LDINT	R19	16
      0x7C440400,  //  00B0  CALL	R17	2
      0x90126811,  //  00B1  SETMBR	R4	K52	R17
      0x60440015,  //  00B2  GETGBL	R17	G21
      0x7C440000,  //  00B3  CALL	R17	0
      0x8C442320,  //  00B4  GETMET	R17	R17	K32
      0x584C0036,  //  00B5  LDCONST	R19	K54
      0x7C440400,  //  00B6  CALL	R17	2
      0x88480B1F,  //  00B7  GETMBR	R18	R5	K31
      0x884C0934,  //  00B8  GETMBR	R19	R4	K52
      0x00482413,  //  00B9  ADD	R18	R18	R19
      0x8C4C0522,  //  00BA  GETMET	R19	R2	K34
      0x7C4C0200,  //  00BB  CALL	R19	1
      0x8C4C2723,  //  00BC  GETMET	R19	R19	K35
      0x88540924,  //  00BD  GETMBR	R21	R4	K36
      0x5C582400,  //  00BE  MOVE	R22	R18
      0x5C5C2200,  //  00BF  MOVE	R23	R17
      0x5462000F,  //  00C0  LDINT	R24	16
      0x7C4C0A00,  //  00C1  CALL	R19	5
      0x8C500527,  //  00C2  GETMET	R20	R2	K39
      0x5C582600,  //  00C3  MOVE	R22	R19
      0x605C0015,  //  00C4  GETGBL	R23	G21
      0x7C5C0000,  //  00C5  CALL	R23	0
      0x8C5C2F20,  //  00C6  GETMET	R23	R23	K32
      0x58640037,  //  00C7  LDCONST	R25	K55
      0x7C5C0400,  //  00C8  CALL	R23	2
      0x60600015,  //  00C9  GETGBL	R24	G21
      0x7C600000,  //  00CA  CALL	R24	0
      0x58640005,  //  00CB  LDCONST	R25	K5
      0x546A000F,  //  00CC  LDINT	R26	16
      0x7C500C00,  //  00CD  CALL	R20	6
      0x8C542929,  //  00CE  GETMET	R21	R20	K41
      0x7C540200,  //  00CF  CALL	R21	1
      0xB85A1A00,  //  00D0  GETNGBL	R22	K13
      0x8C582D38,  //  00D1  GETMET	R22	R22	K56
      0x7C580200,  //  00D2  CALL	R22	1
      0x885C0934,  //  00D3  GETMBR	R23	R4	K52
      0x905A2E17,  //  00D4  SETMBR	R22	K23	R23
      0x885C092F,  //  00D5  GETMBR	R23	R4	K47
      0x905A7217,  //  00D6  SETMBR	R22	K57	R23
      0x905A7415,  //  00D7  SETMBR	R22	K58	R21
      0x8C5C0522,  //  00D8  GETMET	R23	R2	K34
      0x7C5C0200,  //  00D9  CALL	R23	1
      0x8C5C2F23,  //  00DA  GETMET	R23	R23	K35
      0x88640924,  //  00DB  GETMBR	R25	R4	K36
      0x88680B1F,  //  00DC  GETMBR	R26	R5	K31
      0x886C0934,  //  00DD  GETMBR	R27	R4	K52
      0x0068341B,  //  00DE  ADD	R26	R26	R27
      0x606C0015,  //  00DF  GETGBL	R27	G21
      0x7C6C0000,  //  00E0  CALL	R27	0
      0x8C6C3720,  //  00E1  GETMET	R27	R27	K32
      0x5874003B,  //  00E2  LDCONST	R29	K59
      0x7C6C0400,  //  00E3  CALL	R27	2
      0x5472002F,  //  00E4  LDINT	R28	48
      0x7C5C0A00,  //  00E5  CALL	R23	5
      0x5462000E,  //  00E6  LDINT	R24	15
      0x40620A18,  //  00E7  CONNECT	R24	K5	R24
      0x94602E18,  //  00E8  GETIDX	R24	R23	R24
      0x5466000F,  //  00E9  LDINT	R25	16
      0x546A001E,  //  00EA  LDINT	R26	31
      0x4064321A,  //  00EB  CONNECT	R25	R25	R26
      0x94642E19,  //  00EC  GETIDX	R25	R23	R25
      0x546A001F,  //  00ED  LDINT	R26	32
      0x546E002E,  //  00EE  LDINT	R27	47
      0x4068341B,  //  00EF  CONNECT	R26	R26	R27
      0x94682E1A,  //  00F0  GETIDX	R26	R23	R26
      0xB86E0E00,  //  00F1  GETNGBL	R27	K7
      0x8C6C373C,  //  00F2  GETMET	R27	R27	K60
      0x7C6C0200,  //  00F3  CALL	R27	1
      0x946C373D,  //  00F4  GETIDX	R27	R27	K61
      0x8C702D3E,  //  00F5  GETMET	R28	R22	K62
      0x7C700200,  //  00F6  CALL	R28	1
      0x4C740000,  //  00F7  LDNIL	R29
      0x90127E1D,  //  00F8  SETMBR	R4	K63	R29
      0x8C740340,  //  00F9  GETMET	R29	R1	K64
      0x547E0032,  //  00FA  LDINT	R31	51
      0x50800200,  //  00FB  LDBOOL	R32	1	0
      0x7C740600,  //  00FC  CALL	R29	3
      0x8C783B41,  //  00FD  GETMET	R30	R29	K65
      0x5C803800,  //  00FE  MOVE	R32	R28
      0x7C780400,  //  00FF  CALL	R30	2
      0x887C0142,  //  0100  GETMBR	R31	R0	K66
      0x8C7C3F43,  //  0101  GETMET	R31	R31	K67
      0x5C843A00,  //  0102  MOVE	R33	R29
      0x7C7C0400,  //  0103  CALL	R31	2
      0x8C7C0944,  //  0104  GETMET	R31	R4	K68
      0x7C7C0200,  //  0105  CALL	R31	1
      0x8C7C0945,  //  0106  GETMET	R31	R4	K69
      0x5C843000,  //  0107  MOVE	R33	R24
      0x5C883200,  //  0108  MOVE	R34	R25
      0x5C8C3400,  //  0109  MOVE	R35	R26
      0x5C903600,  //  010A  MOVE	R36	R27
      0x7C7C0A00,  //  010B  CALL	R31	5
      0x90128D05,  //  010C  SETMBR	R4	K70	K5
      0x8C7C0947,  //  010D  GETMET	R31	R4	K71
      0x7C7C0200,  //  010E  CALL	R31	1
      0x8C7C0948,  //  010F  GETMET	R31	R4	K72
      0x50840200,  //  0110  LDBOOL	R33	1	0
      0x7C7C0400,  //  0111  CALL	R31	2
      0x8C7C0949,  //  0112  GETMET	R31	R4	K73
      0x7C7C0200,  //  0113  CALL	R31	1
      0x8C7C094A,  //  0114  GETMET	R31	R4	K74
      0x7C7C0200,  //  0115  CALL	R31	1
      0x8C7C094B,  //  0116  GETMET	R31	R4	K75
      0x7C7C0200,  //  0117  CALL	R31	1
      0x507C0200,  //  0118  LDBOOL	R31	1	0
      0x80043E00,  //  0119  RET	1	R31
      0x70020000,  //  011A  JMP		#011C
      0x50180000,  //  011B  LDBOOL	R6	0	0
      0x5C200C00,  //  011C  MOVE	R8	R6
      0x74220190,  //  011D  JMPT	R8	#02AF
      0x8C20014C,  //  011E  GETMET	R8	R0	K76
      0x88280B4D,  //  011F  GETMBR	R10	R5	K77
      0x882C0B1F,  //  0120  GETMBR	R11	R5	K31
      0x7C200600,  //  0121  CALL	R8	3
      0x90123C08,  //  0122  SETMBR	R4	K30	R8
      0x4C240000,  //  0123  LDNIL	R9
      0x1C240809,  //  0124  EQ	R9	R4	R9
      0x74260003,  //  0125  JMPT	R9	#012A
      0x8824091E,  //  0126  GETMBR	R9	R4	K30
      0x4C280000,  //  0127  LDNIL	R10
      0x1C24120A,  //  0128  EQ	R9	R9	R10
      0x7826000D,  //  0129  JMPF	R9	#0138
      0xB8260E00,  //  012A  GETNGBL	R9	K7
      0x8C241308,  //  012B  GETMET	R9	R9	K8
      0x582C004E,  //  012C  LDCONST	R11	K78
      0x5830000A,  //  012D  LDCONST	R12	K10
      0x7C240600,  //  012E  CALL	R9	3
      0x8C24010B,  //  012F  GETMET	R9	R0	K11
      0x5C2C0200,  //  0130  MOVE	R11	R1
      0x5830000C,  //  0131  LDCONST	R12	K12
      0x58340005,  //  0132  LDCONST	R13	K5
      0x5838000C,  //  0133  LDCONST	R14	K12
      0x503C0000,  //  0134  LDBOOL	R15	0	0
      0x7C240C00,  //  0135  CALL	R9	6
      0x50240000,  //  0136  LDBOOL	R9	0	0
      0x80041200,  //  0137  RET	1	R9
      0x8824032B,  //  0138  GETMBR	R9	R1	K43
      0x90125409,  //  0139  SETMBR	R4	K42	R9
      0x8C24092C,  //  013A  GETMET	R9	R4	K44
      0x7C240200,  //  013B  CALL	R9	1
      0x88240B2E,  //  013C  GETMBR	R9	R5	K46
      0x90125A09,  //  013D  SETMBR	R4	K45	R9
      0x8824011A,  //  013E  GETMBR	R9	R0	K26
      0x8824131B,  //  013F  GETMBR	R9	R9	K27
      0x8C241330,  //  0140  GETMET	R9	R9	K48
      0x7C240200,  //  0141  CALL	R9	1
      0x90125E09,  //  0142  SETMBR	R4	K47	R9
      0xB8260E00,  //  0143  GETNGBL	R9	K7
      0x8C241308,  //  0144  GETMET	R9	R9	K8
      0x8C2C0712,  //  0145  GETMET	R11	R3	K18
      0x58340031,  //  0146  LDCONST	R13	K49
      0x8838092F,  //  0147  GETMBR	R14	R4	K47
      0x883C0332,  //  0148  GETMBR	R15	R1	K50
      0x88400333,  //  0149  GETMBR	R16	R1	K51
      0x7C2C0A00,  //  014A  CALL	R11	5
      0x5830000A,  //  014B  LDCONST	R12	K10
      0x7C240600,  //  014C  CALL	R9	3
      0xB8260E00,  //  014D  GETNGBL	R9	K7
      0x8C241308,  //  014E  GETMET	R9	R9	K8
      0xB82E1A00,  //  014F  GETNGBL	R11	K13
      0x8C2C1714,  //  0150  GETMET	R11	R11	K20
      0x8834091E,  //  0151  GETMBR	R13	R4	K30
      0x7C2C0400,  //  0152  CALL	R11	2
      0x002E9E0B,  //  0153  ADD	R11	K79	R11
      0x54320003,  //  0154  LDINT	R12	4
      0x7C240600,  //  0155  CALL	R9	3
      0xB8260E00,  //  0156  GETNGBL	R9	K7
      0x8C241308,  //  0157  GETMET	R9	R9	K8
      0x8C2C0951,  //  0158  GETMET	R11	R4	K81
      0x7C2C0200,  //  0159  CALL	R11	1
      0x8C2C1752,  //  015A  GETMET	R11	R11	K82
      0x7C2C0200,  //  015B  CALL	R11	1
      0x002EA00B,  //  015C  ADD	R11	K80	R11
      0x54320003,  //  015D  LDINT	R12	4
      0x7C240600,  //  015E  CALL	R9	3
      0xB8260E00,  //  015F  GETNGBL	R9	K7
      0x8C241308,  //  0160  GETMET	R9	R9	K8
      0x8C2C0954,  //  0161  GETMET	R11	R4	K84
      0x7C2C0200,  //  0162  CALL	R11	1
      0x8C2C1752,  //  0163  GETMET	R11	R11	K82
      0x7C2C0200,  //  0164  CALL	R11	1
      0x002EA60B,  //  0165  ADD	R11	K83	R11
      0x54320003,  //  0166  LDINT	R12	4
      0x7C240600,  //  0167  CALL	R9	3
      0x8C241155,  //  0168  GETMET	R9	R8	K85
      0x7C240200,  //  0169  CALL	R9	1
      0x78260008,  //  016A  JMPF	R9	#0174
      0xB8260E00,  //  016B  GETNGBL	R9	K7
      0x8C241308,  //  016C  GETMET	R9	R9	K8
      0x8C2C1155,  //  016D  GETMET	R11	R8	K85
      0x7C2C0200,  //  016E  CALL	R11	1
      0x8C2C1752,  //  016F  GETMET	R11	R11	K82
      0x7C2C0200,  //  0170  CALL	R11	1
      0x002EAC0B,  //  0171  ADD	R11	K86	R11
      0x54320003,  //  0172  LDINT	R12	4
      0x7C240600,  //  0173  CALL	R9	3
      0xB8260E00,  //  0174  GETNGBL	R9	K7
      0x8C241308,  //  0175  GETMET	R9	R9	K8
      0x8C2C1158,  //  0176  GETMET	R11	R8	K88
      0x7C2C0200,  //  0177  CALL	R11	1
      0x8C2C1752,  //  0178  GETMET	R11	R11	K82
      0x7C2C0200,  //  0179  CALL	R11	1
      0x002EAE0B,  //  017A  ADD	R11	K87	R11
      0x54320003,  //  017B  LDINT	R12	4
      0x7C240600,  //  017C  CALL	R9	3
      0x8C240535,  //  017D  GETMET	R9	R2	K53
      0x542E000F,  //  017E  LDINT	R11	16
      0x7C240400,  //  017F  CALL	R9	2
      0x90126809,  //  0180  SETMBR	R4	K52	R9
      0x8C240535,  //  0181  GETMET	R9	R2	K53
      0x542E001F,  //  0182  LDINT	R11	32
      0x7C240400,  //  0183  CALL	R9	2
      0x9012B209,  //  0184  SETMBR	R4	K89	R9
      0x8C24055B,  //  0185  GETMET	R9	R2	K91
      0x7C240200,  //  0186  CALL	R9	1
      0x8C24135C,  //  0187  GETMET	R9	R9	K92
      0x882C0959,  //  0188  GETMBR	R11	R4	K89
      0x7C240400,  //  0189  CALL	R9	2
      0x9012B409,  //  018A  SETMBR	R4	K90	R9
      0xB8260E00,  //  018B  GETNGBL	R9	K7
      0x8C241308,  //  018C  GETMET	R9	R9	K8
      0x882C0959,  //  018D  GETMBR	R11	R4	K89
      0x8C2C1752,  //  018E  GETMET	R11	R11	K82
      0x7C2C0200,  //  018F  CALL	R11	1
      0x002EBA0B,  //  0190  ADD	R11	K93	R11
      0x54320003,  //  0191  LDINT	R12	4
      0x7C240600,  //  0192  CALL	R9	3
      0xB8260E00,  //  0193  GETNGBL	R9	K7
      0x8C241308,  //  0194  GETMET	R9	R9	K8
      0x882C095A,  //  0195  GETMBR	R11	R4	K90
      0x8C2C1752,  //  0196  GETMET	R11	R11	K82
      0x7C2C0200,  //  0197  CALL	R11	1
      0x002EBC0B,  //  0198  ADD	R11	K94	R11
      0x54320003,  //  0199  LDINT	R12	4
      0x7C240600,  //  019A  CALL	R9	3
      0x8C240535,  //  019B  GETMET	R9	R2	K53
      0x542E001F,  //  019C  LDINT	R11	32
      0x7C240400,  //  019D  CALL	R9	2
      0x8C28055B,  //  019E  GETMET	R10	R2	K91
      0x7C280200,  //  019F  CALL	R10	1
      0x8C28155F,  //  01A0  GETMET	R10	R10	K95
      0x88300959,  //  01A1  GETMBR	R12	R4	K89
      0x88340B16,  //  01A2  GETMBR	R13	R5	K22
      0x7C280600,  //  01A3  CALL	R10	3
      0x9012480A,  //  01A4  SETMBR	R4	K36	R10
      0xB82A0E00,  //  01A5  GETNGBL	R10	K7
      0x8C281508,  //  01A6  GETMET	R10	R10	K8
      0x88300924,  //  01A7  GETMBR	R12	R4	K36
      0x8C301952,  //  01A8  GETMET	R12	R12	K82
      0x7C300200,  //  01A9  CALL	R12	1
      0x0032C00C,  //  01AA  ADD	R12	K96	R12
      0x54360003,  //  01AB  LDINT	R13	4
      0x7C280600,  //  01AC  CALL	R10	3
      0xB82A1A00,  //  01AD  GETNGBL	R10	K13
      0x88281561,  //  01AE  GETMBR	R10	R10	K97
      0x8C281562,  //  01AF  GETMET	R10	R10	K98
      0x7C280200,  //  01B0  CALL	R10	1
      0x8C2C1563,  //  01B1  GETMET	R11	R10	K99
      0x5834000C,  //  01B2  LDCONST	R13	K12
      0xB83A1A00,  //  01B3  GETNGBL	R14	K13
      0x88381D61,  //  01B4  GETMBR	R14	R14	K97
      0x88381D64,  //  01B5  GETMBR	R14	R14	K100
      0x8C3C1154,  //  01B6  GETMET	R15	R8	K84
      0x7C3C0200,  //  01B7  CALL	R15	1
      0x7C2C0800,  //  01B8  CALL	R11	4
      0x8C2C1563,  //  01B9  GETMET	R11	R10	K99
      0x5834000A,  //  01BA  LDCONST	R13	K10
      0xB83A1A00,  //  01BB  GETNGBL	R14	K13
      0x88381D61,  //  01BC  GETMBR	R14	R14	K97
      0x88381D64,  //  01BD  GETMBR	R14	R14	K100
      0x8C3C1155,  //  01BE  GETMET	R15	R8	K85
      0x7C3C0200,  //  01BF  CALL	R15	1
      0x7C2C0800,  //  01C0  CALL	R11	4
      0x8C2C1563,  //  01C1  GETMET	R11	R10	K99
      0x58340065,  //  01C2  LDCONST	R13	K101
      0xB83A1A00,  //  01C3  GETNGBL	R14	K13
      0x88381D61,  //  01C4  GETMBR	R14	R14	K97
      0x88381D64,  //  01C5  GETMBR	R14	R14	K100
      0x883C095A,  //  01C6  GETMBR	R15	R4	K90
      0x7C2C0800,  //  01C7  CALL	R11	4
      0x8C2C1563,  //  01C8  GETMET	R11	R10	K99
      0x54360003,  //  01C9  LDINT	R13	4
      0xB83A1A00,  //  01CA  GETNGBL	R14	K13
      0x88381D61,  //  01CB  GETMBR	R14	R14	K97
      0x88381D64,  //  01CC  GETMBR	R14	R14	K100
      0x883C0B16,  //  01CD  GETMBR	R15	R5	K22
      0x7C2C0800,  //  01CE  CALL	R11	4
      0x8C2C055B,  //  01CF  GETMET	R11	R2	K91
      0x7C2C0200,  //  01D0  CALL	R11	1
      0x8C2C1766,  //  01D1  GETMET	R11	R11	K102
      0x8C341151,  //  01D2  GETMET	R13	R8	K81
      0x7C340200,  //  01D3  CALL	R13	1
      0x8C38153E,  //  01D4  GETMET	R14	R10	K62
      0x7C380200,  //  01D5  CALL	R14	1
      0x7C2C0600,  //  01D6  CALL	R11	3
      0xB8320E00,  //  01D7  GETNGBL	R12	K7
      0x8C301908,  //  01D8  GETMET	R12	R12	K8
      0x58380067,  //  01D9  LDCONST	R14	K103
      0x543E0003,  //  01DA  LDINT	R15	4
      0x7C300600,  //  01DB  CALL	R12	3
      0xB8320E00,  //  01DC  GETNGBL	R12	K7
      0x8C301908,  //  01DD  GETMET	R12	R12	K8
      0x60380008,  //  01DE  GETGBL	R14	G8
      0x8C3C1151,  //  01DF  GETMET	R15	R8	K81
      0x7C3C0200,  //  01E0  CALL	R15	1
      0x7C380200,  //  01E1  CALL	R14	1
      0x003AD00E,  //  01E2  ADD	R14	K104	R14
      0x543E0003,  //  01E3  LDINT	R15	4
      0x7C300600,  //  01E4  CALL	R12	3
      0xB8320E00,  //  01E5  GETNGBL	R12	K7
      0x8C301908,  //  01E6  GETMET	R12	R12	K8
      0x60380008,  //  01E7  GETGBL	R14	G8
      0x5C3C1400,  //  01E8  MOVE	R15	R10
      0x7C380200,  //  01E9  CALL	R14	1
      0x003AD20E,  //  01EA  ADD	R14	K105	R14
      0x543E0003,  //  01EB  LDINT	R15	4
      0x7C300600,  //  01EC  CALL	R12	3
      0xB8320E00,  //  01ED  GETNGBL	R12	K7
      0x8C301908,  //  01EE  GETMET	R12	R12	K8
      0x8C381752,  //  01EF  GETMET	R14	R11	K82
      0x7C380200,  //  01F0  CALL	R14	1
      0x003AD40E,  //  01F1  ADD	R14	K106	R14
      0x543E0003,  //  01F2  LDINT	R15	4
      0x7C300600,  //  01F3  CALL	R12	3
      0xB8321A00,  //  01F4  GETNGBL	R12	K13
      0x88301961,  //  01F5  GETMBR	R12	R12	K97
      0x8C301962,  //  01F6  GETMET	R12	R12	K98
      0x7C300200,  //  01F7  CALL	R12	1
      0x8C341963,  //  01F8  GETMET	R13	R12	K99
      0x583C000C,  //  01F9  LDCONST	R15	K12
      0xB8421A00,  //  01FA  GETNGBL	R16	K13
      0x88402161,  //  01FB  GETMBR	R16	R16	K97
      0x88402164,  //  01FC  GETMBR	R16	R16	K100
      0x8C441154,  //  01FD  GETMET	R17	R8	K84
      0x7C440200,  //  01FE  CALL	R17	1
      0x7C340800,  //  01FF  CALL	R13	4
      0x8C341963,  //  0200  GETMET	R13	R12	K99
      0x583C000A,  //  0201  LDCONST	R15	K10
      0xB8421A00,  //  0202  GETNGBL	R16	K13
      0x88402161,  //  0203  GETMBR	R16	R16	K97
      0x88402164,  //  0204  GETMBR	R16	R16	K100
      0x8C441155,  //  0205  GETMET	R17	R8	K85
      0x7C440200,  //  0206  CALL	R17	1
      0x7C340800,  //  0207  CALL	R13	4
      0x8C341963,  //  0208  GETMET	R13	R12	K99
      0x583C0065,  //  0209  LDCONST	R15	K101
      0xB8421A00,  //  020A  GETNGBL	R16	K13
      0x88402161,  //  020B  GETMBR	R16	R16	K97
      0x88402164,  //  020C  GETMBR	R16	R16	K100
      0x5C441600,  //  020D  MOVE	R17	R11
      0x7C340800,  //  020E  CALL	R13	4
      0x8C341963,  //  020F  GETMET	R13	R12	K99
      0x543E0003,  //  0210  LDINT	R15	4
      0xB8421A00,  //  0211  GETNGBL	R16	K13
      0x88402161,  //  0212  GETMBR	R16	R16	K97
      0x88402164,  //  0213  GETMBR	R16	R16	K100
      0x88440934,  //  0214  GETMBR	R17	R4	K52
      0x7C340800,  //  0215  CALL	R13	4
      0xB8360E00,  //  0216  GETNGBL	R13	K7
      0x8C341B08,  //  0217  GETMET	R13	R13	K8
      0x583C0067,  //  0218  LDCONST	R15	K103
      0x54420003,  //  0219  LDINT	R16	4
      0x7C340600,  //  021A  CALL	R13	3
      0x88340B6B,  //  021B  GETMBR	R13	R5	K107
      0x90127E0D,  //  021C  SETMBR	R4	K63	R13
      0xB8360E00,  //  021D  GETNGBL	R13	K7
      0x8C341B08,  //  021E  GETMET	R13	R13	K8
      0x883C0934,  //  021F  GETMBR	R15	R4	K52
      0x8C3C1F52,  //  0220  GETMET	R15	R15	K82
      0x7C3C0200,  //  0221  CALL	R15	1
      0x003ED80F,  //  0222  ADD	R15	K108	R15
      0x54420003,  //  0223  LDINT	R16	4
      0x7C340600,  //  0224  CALL	R13	3
      0xB8360E00,  //  0225  GETNGBL	R13	K7
      0x8C341B08,  //  0226  GETMET	R13	R13	K8
      0x883C093F,  //  0227  GETMBR	R15	R4	K63
      0x8C3C1F52,  //  0228  GETMET	R15	R15	K82
      0x7C3C0200,  //  0229  CALL	R15	1
      0x003EDA0F,  //  022A  ADD	R15	K109	R15
      0x54420003,  //  022B  LDINT	R16	4
      0x7C340600,  //  022C  CALL	R13	3
      0x8C34056E,  //  022D  GETMET	R13	R2	K110
      0x7C340200,  //  022E  CALL	R13	1
      0x8C341B6F,  //  022F  GETMET	R13	R13	K111
      0x883C093F,  //  0230  GETMBR	R15	R4	K63
      0x7C340400,  //  0231  CALL	R13	2
      0x8C341B70,  //  0232  GETMET	R13	R13	K112
      0x7C340200,  //  0233  CALL	R13	1
      0xB83A0E00,  //  0234  GETNGBL	R14	K7
      0x8C381D08,  //  0235  GETMET	R14	R14	K8
      0x8C401B52,  //  0236  GETMET	R16	R13	K82
      0x7C400200,  //  0237  CALL	R16	1
      0x0042E210,  //  0238  ADD	R16	K113	R16
      0x54460003,  //  0239  LDINT	R17	4
      0x7C380600,  //  023A  CALL	R14	3
      0x60380015,  //  023B  GETGBL	R14	G21
      0x7C380000,  //  023C  CALL	R14	0
      0x8C381D20,  //  023D  GETMET	R14	R14	K32
      0x88400172,  //  023E  GETMBR	R16	R0	K114
      0x7C380400,  //  023F  CALL	R14	2
      0x8C3C1173,  //  0240  GETMET	R15	R8	K115
      0x7C3C0200,  //  0241  CALL	R15	1
      0x003C1E09,  //  0242  ADD	R15	R15	R9
      0x8840095A,  //  0243  GETMBR	R16	R4	K90
      0x003C1E10,  //  0244  ADD	R15	R15	R16
      0x003C1E0D,  //  0245  ADD	R15	R15	R13
      0x8C400522,  //  0246  GETMET	R16	R2	K34
      0x7C400200,  //  0247  CALL	R16	1
      0x8C402123,  //  0248  GETMET	R16	R16	K35
      0x88480924,  //  0249  GETMBR	R18	R4	K36
      0x5C4C1E00,  //  024A  MOVE	R19	R15
      0x5C501C00,  //  024B  MOVE	R20	R14
      0x5456000F,  //  024C  LDINT	R21	16
      0x7C400A00,  //  024D  CALL	R16	5
      0xB8460E00,  //  024E  GETNGBL	R17	K7
      0x8C442308,  //  024F  GETMET	R17	R17	K8
      0x884C0924,  //  0250  GETMBR	R19	R4	K36
      0x8C4C2752,  //  0251  GETMET	R19	R19	K82
      0x7C4C0200,  //  0252  CALL	R19	1
      0x004EE813,  //  0253  ADD	R19	K116	R19
      0x54520003,  //  0254  LDINT	R20	4
      0x7C440600,  //  0255  CALL	R17	3
      0xB8460E00,  //  0256  GETNGBL	R17	K7
      0x8C442308,  //  0257  GETMET	R17	R17	K8
      0x8C4C1F52,  //  0258  GETMET	R19	R15	K82
      0x7C4C0200,  //  0259  CALL	R19	1
      0x004EEA13,  //  025A  ADD	R19	K117	R19
      0x54520003,  //  025B  LDINT	R20	4
      0x7C440600,  //  025C  CALL	R17	3
      0xB8460E00,  //  025D  GETNGBL	R17	K7
      0x8C442308,  //  025E  GETMET	R17	R17	K8
      0x8C4C2152,  //  025F  GETMET	R19	R16	K82
      0x7C4C0200,  //  0260  CALL	R19	1
      0x004EEC13,  //  0261  ADD	R19	K118	R19
      0x54520003,  //  0262  LDINT	R20	4
      0x7C440600,  //  0263  CALL	R17	3
      0x8C44193E,  //  0264  GETMET	R17	R12	K62
      0x7C440200,  //  0265  CALL	R17	1
      0xB84A0E00,  //  0266  GETNGBL	R18	K7
      0x8C482508,  //  0267  GETMET	R18	R18	K8
      0x8C502352,  //  0268  GETMET	R20	R17	K82
      0x7C500200,  //  0269  CALL	R20	1
      0x0052EE14,  //  026A  ADD	R20	K119	R20
      0x54560003,  //  026B  LDINT	R21	4
      0x7C480600,  //  026C  CALL	R18	3
      0x8C480527,  //  026D  GETMET	R18	R2	K39
      0x5C502000,  //  026E  MOVE	R20	R16
      0x60540015,  //  026F  GETGBL	R21	G21
      0x7C540000,  //  0270  CALL	R21	0
      0x8C542B20,  //  0271  GETMET	R21	R21	K32
      0x885C0178,  //  0272  GETMBR	R23	R0	K120
      0x7C540400,  //  0273  CALL	R21	2
      0x60580015,  //  0274  GETGBL	R22	G21
      0x7C580000,  //  0275  CALL	R22	0
      0x605C000C,  //  0276  GETGBL	R23	G12
      0x5C602200,  //  0277  MOVE	R24	R17
      0x7C5C0200,  //  0278  CALL	R23	1
      0x5462000F,  //  0279  LDINT	R24	16
      0x7C480C00,  //  027A  CALL	R18	6
      0x8C4C2579,  //  027B  GETMET	R19	R18	K121
      0x5C542200,  //  027C  MOVE	R21	R17
      0x7C4C0400,  //  027D  CALL	R19	2
      0x8C502529,  //  027E  GETMET	R20	R18	K41
      0x7C500200,  //  027F  CALL	R20	1
      0x004C2614,  //  0280  ADD	R19	R19	R20
      0xB8520E00,  //  0281  GETNGBL	R20	K7
      0x8C502908,  //  0282  GETMET	R20	R20	K8
      0x8C582752,  //  0283  GETMET	R22	R19	K82
      0x7C580200,  //  0284  CALL	R22	1
      0x005AF416,  //  0285  ADD	R22	K122	R22
      0x545E0003,  //  0286  LDINT	R23	4
      0x7C500600,  //  0287  CALL	R20	3
      0xB8520E00,  //  0288  GETNGBL	R20	K7
      0x8C502908,  //  0289  GETMET	R20	R20	K8
      0x58580067,  //  028A  LDCONST	R22	K103
      0x545E0003,  //  028B  LDINT	R23	4
      0x7C500600,  //  028C  CALL	R20	3
      0xB8521A00,  //  028D  GETNGBL	R20	K13
      0x8C50297B,  //  028E  GETMET	R20	R20	K123
      0x7C500200,  //  028F  CALL	R20	1
      0x9052F809,  //  0290  SETMBR	R20	K124	R9
      0x8854092F,  //  0291  GETMBR	R21	R4	K47
      0x9052FA15,  //  0292  SETMBR	R20	K125	R21
      0x8854095A,  //  0293  GETMBR	R21	R4	K90
      0x9052FC15,  //  0294  SETMBR	R20	K126	R21
      0x9052FE13,  //  0295  SETMBR	R20	K127	R19
      0xB8560E00,  //  0296  GETNGBL	R21	K7
      0x8C542B08,  //  0297  GETMET	R21	R21	K8
      0xB85E1A00,  //  0298  GETNGBL	R23	K13
      0x8C5C2F14,  //  0299  GETMET	R23	R23	K20
      0x5C642800,  //  029A  MOVE	R25	R20
      0x7C5C0400,  //  029B  CALL	R23	2
      0x005F0017,  //  029C  ADD	R23	K128	R23
      0x54620003,  //  029D  LDINT	R24	4
      0x7C540600,  //  029E  CALL	R21	3
      0x8C54293E,  //  029F  GETMET	R21	R20	K62
      0x7C540200,  //  02A0  CALL	R21	1
      0x90130215,  //  02A1  SETMBR	R4	K129	R21
      0x8C580340,  //  02A2  GETMET	R22	R1	K64
      0x54620030,  //  02A3  LDINT	R24	49
      0x50640200,  //  02A4  LDBOOL	R25	1	0
      0x7C580600,  //  02A5  CALL	R22	3
      0x8C5C2D41,  //  02A6  GETMET	R23	R22	K65
      0x5C642A00,  //  02A7  MOVE	R25	R21
      0x7C5C0400,  //  02A8  CALL	R23	2
      0x88600142,  //  02A9  GETMBR	R24	R0	K66
      0x8C603143,  //  02AA  GETMET	R24	R24	K67
      0x5C682C00,  //  02AB  MOVE	R26	R22
      0x7C600400,  //  02AC  CALL	R24	2
      0x50600200,  //  02AD  LDBOOL	R24	1	0
      0x80043000,  //  02AE  RET	1	R24
      0x50200200,  //  02AF  LDBOOL	R8	1	0
      0x80041000,  //  02B0  RET	1	R8
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
