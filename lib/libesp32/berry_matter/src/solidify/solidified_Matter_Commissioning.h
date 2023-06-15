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
    /* K8   */  be_const_int(3),
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
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20SEARCHING_X3A_X20destinationId_X3D),
    /* K4   */  be_nested_str_weak(tohex),
    /* K5   */  be_nested_str_weak(device),
    /* K6   */  be_nested_str_weak(sessions),
    /* K7   */  be_nested_str_weak(fabrics),
    /* K8   */  be_nested_str_weak(noc),
    /* K9   */  be_nested_str_weak(fabric_id),
    /* K10  */  be_nested_str_weak(device_id),
    /* K11  */  be_nested_str_weak(get_ca_pub),
    /* K12  */  be_nested_str_weak(get_ipk_group_key),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20destinationMessage_X3D),
    /* K14  */  be_nested_str_weak(HMAC_SHA256),
    /* K15  */  be_nested_str_weak(update),
    /* K16  */  be_nested_str_weak(out),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20candidateDestinationId_X3D),
    /* K18  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(find_fabric_by_destination_id),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x8C180304,  //  0003  GETMET	R6	R1	K4
      0x7C180200,  //  0004  CALL	R6	1
      0x001A0606,  //  0005  ADD	R6	K3	R6
      0x541E0003,  //  0006  LDINT	R7	4
      0x7C100600,  //  0007  CALL	R4	3
      0x60100010,  //  0008  GETGBL	R4	G16
      0x88140105,  //  0009  GETMBR	R5	R0	K5
      0x88140B06,  //  000A  GETMBR	R5	R5	K6
      0x88140B07,  //  000B  GETMBR	R5	R5	K7
      0x7C100200,  //  000C  CALL	R4	1
      0xA8020032,  //  000D  EXBLK	0	#0041
      0x5C140800,  //  000E  MOVE	R5	R4
      0x7C140000,  //  000F  CALL	R5	0
      0x88180B08,  //  0010  GETMBR	R6	R5	K8
      0x4C1C0000,  //  0011  LDNIL	R7
      0x1C180C07,  //  0012  EQ	R6	R6	R7
      0x741A0007,  //  0013  JMPT	R6	#001C
      0x88180B09,  //  0014  GETMBR	R6	R5	K9
      0x4C1C0000,  //  0015  LDNIL	R7
      0x1C180C07,  //  0016  EQ	R6	R6	R7
      0x741A0003,  //  0017  JMPT	R6	#001C
      0x88180B0A,  //  0018  GETMBR	R6	R5	K10
      0x4C1C0000,  //  0019  LDNIL	R7
      0x1C180C07,  //  001A  EQ	R6	R6	R7
      0x781A0000,  //  001B  JMPF	R6	#001D
      0x7001FFF0,  //  001C  JMP		#000E
      0x8C180B0B,  //  001D  GETMET	R6	R5	K11
      0x7C180200,  //  001E  CALL	R6	1
      0x00180406,  //  001F  ADD	R6	R2	R6
      0x881C0B09,  //  0020  GETMBR	R7	R5	K9
      0x00180C07,  //  0021  ADD	R6	R6	R7
      0x881C0B0A,  //  0022  GETMBR	R7	R5	K10
      0x00180C07,  //  0023  ADD	R6	R6	R7
      0x8C1C0B0C,  //  0024  GETMET	R7	R5	K12
      0x7C1C0200,  //  0025  CALL	R7	1
      0xB8220200,  //  0026  GETNGBL	R8	K1
      0x8C201102,  //  0027  GETMET	R8	R8	K2
      0x8C280D04,  //  0028  GETMET	R10	R6	K4
      0x7C280200,  //  0029  CALL	R10	1
      0x002A1A0A,  //  002A  ADD	R10	K13	R10
      0x542E0003,  //  002B  LDINT	R11	4
      0x7C200600,  //  002C  CALL	R8	3
      0x8C20070E,  //  002D  GETMET	R8	R3	K14
      0x5C280E00,  //  002E  MOVE	R10	R7
      0x7C200400,  //  002F  CALL	R8	2
      0x8C24110F,  //  0030  GETMET	R9	R8	K15
      0x5C2C0C00,  //  0031  MOVE	R11	R6
      0x7C240400,  //  0032  CALL	R9	2
      0x8C241110,  //  0033  GETMET	R9	R8	K16
      0x7C240200,  //  0034  CALL	R9	1
      0xB82A0200,  //  0035  GETNGBL	R10	K1
      0x8C281502,  //  0036  GETMET	R10	R10	K2
      0x8C301304,  //  0037  GETMET	R12	R9	K4
      0x7C300200,  //  0038  CALL	R12	1
      0x0032220C,  //  0039  ADD	R12	K17	R12
      0x54360003,  //  003A  LDINT	R13	4
      0x7C280600,  //  003B  CALL	R10	3
      0x1C281201,  //  003C  EQ	R10	R9	R1
      0x782A0001,  //  003D  JMPF	R10	#0040
      0xA8040001,  //  003E  EXBLK	1	1
      0x80040A00,  //  003F  RET	1	R5
      0x7001FFCC,  //  0040  JMP		#000E
      0x58100012,  //  0041  LDCONST	R4	K18
      0xAC100200,  //  0042  CATCH	R4	1	0
      0xB0080000,  //  0043  RAISE	2	R0	R0
      0x4C100000,  //  0044  LDNIL	R4
      0x80040800,  //  0045  RET	1	R4
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20add_session_X20local_session_id_X3D_X25i_X20initiator_session_id_X3D_X25i),
    /* K5   */  be_nested_str_weak(device),
    /* K6   */  be_nested_str_weak(sessions),
    /* K7   */  be_nested_str_weak(create_session),
    /* K8   */  be_nested_str_weak(set_keys),
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
      0x542A0003,  //  0008  LDINT	R10	4
      0x7C1C0600,  //  0009  CALL	R7	3
      0x881C0105,  //  000A  GETMBR	R7	R0	K5
      0x881C0F06,  //  000B  GETMBR	R7	R7	K6
      0x8C1C0F07,  //  000C  GETMET	R7	R7	K7
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280400,  //  000E  MOVE	R10	R2
      0x7C1C0600,  //  000F  CALL	R7	3
      0x8C200F08,  //  0010  GETMET	R8	R7	K8
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
    ( &(const bvalue[50]) {     /* constants */
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
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(send_status_report),
    /* K14  */  be_const_int(1),
    /* K15  */  be_nested_str_weak(matter),
    /* K16  */  be_nested_str_weak(PBKDFParamRequest),
    /* K17  */  be_nested_str_weak(parse),
    /* K18  */  be_nested_str_weak(raw),
    /* K19  */  be_nested_str_weak(app_payload_idx),
    /* K20  */  be_nested_str_weak(set_mode_PASE),
    /* K21  */  be_nested_str_weak(__Msg1),
    /* K22  */  be_const_int(2147483647),
    /* K23  */  be_nested_str_weak(passcodeId),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20non_X2Dzero_X20passcode_X20id),
    /* K25  */  be_nested_str_weak(__future_initiator_session_id),
    /* K26  */  be_nested_str_weak(initiator_session_id),
    /* K27  */  be_nested_str_weak(__future_local_session_id),
    /* K28  */  be_nested_str_weak(device),
    /* K29  */  be_nested_str_weak(sessions),
    /* K30  */  be_nested_str_weak(gen_local_session_id),
    /* K31  */  be_nested_str_weak(format),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20New_Session_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K33  */  be_nested_str_weak(remote_ip),
    /* K34  */  be_nested_str_weak(remote_port),
    /* K35  */  be_nested_str_weak(PBKDFParamResponse),
    /* K36  */  be_nested_str_weak(initiatorRandom),
    /* K37  */  be_nested_str_weak(responderRandom),
    /* K38  */  be_nested_str_weak(random),
    /* K39  */  be_nested_str_weak(responderSessionId),
    /* K40  */  be_nested_str_weak(pbkdf_parameters_salt),
    /* K41  */  be_nested_str_weak(commissioning_salt),
    /* K42  */  be_nested_str_weak(pbkdf_parameters_iterations),
    /* K43  */  be_nested_str_weak(commissioning_iterations),
    /* K44  */  be_nested_str_weak(tlv2raw),
    /* K45  */  be_nested_str_weak(__Msg2),
    /* K46  */  be_nested_str_weak(build_response),
    /* K47  */  be_nested_str_weak(encode_frame),
    /* K48  */  be_nested_str_weak(responder),
    /* K49  */  be_nested_str_weak(send_response_frame),
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
      0x5820000C,  //  0015  LDCONST	R8	K12
      0x7C140600,  //  0016  CALL	R5	3
      0x8C14010D,  //  0017  GETMET	R5	R0	K13
      0x5C1C0200,  //  0018  MOVE	R7	R1
      0x5820000E,  //  0019  LDCONST	R8	K14
      0x58240005,  //  001A  LDCONST	R9	K5
      0x5828000A,  //  001B  LDCONST	R10	K10
      0x502C0000,  //  001C  LDBOOL	R11	0	0
      0x7C140C00,  //  001D  CALL	R5	6
      0x50140000,  //  001E  LDBOOL	R5	0	0
      0x80040A00,  //  001F  RET	1	R5
      0xB8161E00,  //  0020  GETNGBL	R5	K15
      0x8C140B10,  //  0021  GETMET	R5	R5	K16
      0x7C140200,  //  0022  CALL	R5	1
      0x8C140B11,  //  0023  GETMET	R5	R5	K17
      0x881C0312,  //  0024  GETMBR	R7	R1	K18
      0x88200313,  //  0025  GETMBR	R8	R1	K19
      0x7C140600,  //  0026  CALL	R5	3
      0x88180302,  //  0027  GETMBR	R6	R1	K2
      0x8C180D14,  //  0028  GETMET	R6	R6	K20
      0x7C180200,  //  0029  CALL	R6	1
      0x88180313,  //  002A  GETMBR	R6	R1	K19
      0x40180D16,  //  002B  CONNECT	R6	R6	K22
      0x881C0312,  //  002C  GETMBR	R7	R1	K18
      0x94180E06,  //  002D  GETIDX	R6	R7	R6
      0x90122A06,  //  002E  SETMBR	R4	K21	R6
      0x88180B17,  //  002F  GETMBR	R6	R5	K23
      0x20180D05,  //  0030  NE	R6	R6	K5
      0x781A0012,  //  0031  JMPF	R6	#0045
      0xB81A0E00,  //  0032  GETNGBL	R6	K7
      0x8C180D08,  //  0033  GETMET	R6	R6	K8
      0x58200018,  //  0034  LDCONST	R8	K24
      0x5824000A,  //  0035  LDCONST	R9	K10
      0x7C180600,  //  0036  CALL	R6	3
      0xB81A0E00,  //  0037  GETNGBL	R6	K7
      0x8C180D08,  //  0038  GETMET	R6	R6	K8
      0x5820000B,  //  0039  LDCONST	R8	K11
      0x5824000C,  //  003A  LDCONST	R9	K12
      0x7C180600,  //  003B  CALL	R6	3
      0x8C18010D,  //  003C  GETMET	R6	R0	K13
      0x5C200200,  //  003D  MOVE	R8	R1
      0x5824000E,  //  003E  LDCONST	R9	K14
      0x58280005,  //  003F  LDCONST	R10	K5
      0x582C000A,  //  0040  LDCONST	R11	K10
      0x50300000,  //  0041  LDBOOL	R12	0	0
      0x7C180C00,  //  0042  CALL	R6	6
      0x50180000,  //  0043  LDBOOL	R6	0	0
      0x80040C00,  //  0044  RET	1	R6
      0x88180B1A,  //  0045  GETMBR	R6	R5	K26
      0x90123206,  //  0046  SETMBR	R4	K25	R6
      0x8818011C,  //  0047  GETMBR	R6	R0	K28
      0x88180D1D,  //  0048  GETMBR	R6	R6	K29
      0x8C180D1E,  //  0049  GETMET	R6	R6	K30
      0x7C180200,  //  004A  CALL	R6	1
      0x90123606,  //  004B  SETMBR	R4	K27	R6
      0xB81A0E00,  //  004C  GETNGBL	R6	K7
      0x8C180D08,  //  004D  GETMET	R6	R6	K8
      0x8C20071F,  //  004E  GETMET	R8	R3	K31
      0x58280020,  //  004F  LDCONST	R10	K32
      0x882C091B,  //  0050  GETMBR	R11	R4	K27
      0x88300321,  //  0051  GETMBR	R12	R1	K33
      0x88340322,  //  0052  GETMBR	R13	R1	K34
      0x7C200A00,  //  0053  CALL	R8	5
      0x5824000C,  //  0054  LDCONST	R9	K12
      0x7C180600,  //  0055  CALL	R6	3
      0xB81A1E00,  //  0056  GETNGBL	R6	K15
      0x8C180D23,  //  0057  GETMET	R6	R6	K35
      0x7C180200,  //  0058  CALL	R6	1
      0x881C0B24,  //  0059  GETMBR	R7	R5	K36
      0x901A4807,  //  005A  SETMBR	R6	K36	R7
      0x8C1C0526,  //  005B  GETMET	R7	R2	K38
      0x5426001F,  //  005C  LDINT	R9	32
      0x7C1C0400,  //  005D  CALL	R7	2
      0x901A4A07,  //  005E  SETMBR	R6	K37	R7
      0x881C091B,  //  005F  GETMBR	R7	R4	K27
      0x901A4E07,  //  0060  SETMBR	R6	K39	R7
      0x881C011C,  //  0061  GETMBR	R7	R0	K28
      0x881C0F29,  //  0062  GETMBR	R7	R7	K41
      0x901A5007,  //  0063  SETMBR	R6	K40	R7
      0x881C011C,  //  0064  GETMBR	R7	R0	K28
      0x881C0F2B,  //  0065  GETMBR	R7	R7	K43
      0x901A5407,  //  0066  SETMBR	R6	K42	R7
      0x8C1C0D2C,  //  0067  GETMET	R7	R6	K44
      0x7C1C0200,  //  0068  CALL	R7	1
      0x90125A07,  //  0069  SETMBR	R4	K45	R7
      0x8C20032E,  //  006A  GETMET	R8	R1	K46
      0x542A0020,  //  006B  LDINT	R10	33
      0x502C0200,  //  006C  LDBOOL	R11	1	0
      0x7C200600,  //  006D  CALL	R8	3
      0x8C24112F,  //  006E  GETMET	R9	R8	K47
      0x5C2C0E00,  //  006F  MOVE	R11	R7
      0x7C240400,  //  0070  CALL	R9	2
      0x88280130,  //  0071  GETMBR	R10	R0	K48
      0x8C281531,  //  0072  GETMET	R10	R10	K49
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
    23,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[54]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(session),
    /* K3   */  be_nested_str_weak(opcode),
    /* K4   */  be_nested_str_weak(local_session_id),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(protocol_id),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake1_X20message),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K12  */  be_nested_str_weak(send_status_report),
    /* K13  */  be_const_int(1),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(matter),
    /* K16  */  be_nested_str_weak(Pake1),
    /* K17  */  be_nested_str_weak(parse),
    /* K18  */  be_nested_str_weak(raw),
    /* K19  */  be_nested_str_weak(app_payload_idx),
    /* K20  */  be_nested_str_weak(pA),
    /* K21  */  be_nested_str_weak(SPAKE2P_Matter),
    /* K22  */  be_nested_str_weak(device),
    /* K23  */  be_nested_str_weak(commissioning_w0),
    /* K24  */  be_nested_str_weak(commissioning_L),
    /* K25  */  be_nested_str_weak(random),
    /* K26  */  be_nested_str_weak(compute_pB),
    /* K27  */  be_nested_str_weak(compute_ZV_verifier),
    /* K28  */  be_nested_str_weak(SHA256),
    /* K29  */  be_nested_str_weak(update),
    /* K30  */  be_nested_str_weak(fromstring),
    /* K31  */  be_nested_str_weak(Matter_Context_Prefix),
    /* K32  */  be_nested_str_weak(__Msg1),
    /* K33  */  be_nested_str_weak(__Msg2),
    /* K34  */  be_nested_str_weak(out),
    /* K35  */  be_nested_str_weak(set_context),
    /* K36  */  be_nested_str_weak(compute_TT_hash),
    /* K37  */  be_nested_str_weak(Pake2),
    /* K38  */  be_nested_str_weak(pB),
    /* K39  */  be_nested_str_weak(cB),
    /* K40  */  be_nested_str_weak(tlv2raw),
    /* K41  */  be_nested_str_weak(__spake_cA),
    /* K42  */  be_nested_str_weak(cA),
    /* K43  */  be_nested_str_weak(__spake_Ke),
    /* K44  */  be_nested_str_weak(Ke),
    /* K45  */  be_nested_str_weak(build_response),
    /* K46  */  be_nested_str_weak(encode_frame),
    /* K47  */  be_nested_str_weak(format),
    /* K48  */  be_nested_str_weak(MTR_X3A_X20New_X20Commissioning_X20_X28PASE_X20id_X3D_X25i_X29_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K49  */  be_nested_str_weak(__future_local_session_id),
    /* K50  */  be_nested_str_weak(_ip),
    /* K51  */  be_nested_str_weak(_port),
    /* K52  */  be_nested_str_weak(responder),
    /* K53  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x88100302,  //  0002  GETMBR	R4	R1	K2
      0x88140303,  //  0003  GETMBR	R5	R1	K3
      0x541A0021,  //  0004  LDINT	R6	34
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
      0x5828000E,  //  001B  LDCONST	R10	K14
      0x502C0000,  //  001C  LDBOOL	R11	0	0
      0x7C140C00,  //  001D  CALL	R5	6
      0x50140000,  //  001E  LDBOOL	R5	0	0
      0x80040A00,  //  001F  RET	1	R5
      0xB8161E00,  //  0020  GETNGBL	R5	K15
      0x8C140B10,  //  0021  GETMET	R5	R5	K16
      0x7C140200,  //  0022  CALL	R5	1
      0x8C140B11,  //  0023  GETMET	R5	R5	K17
      0x881C0312,  //  0024  GETMBR	R7	R1	K18
      0x88200313,  //  0025  GETMBR	R8	R1	K19
      0x7C140600,  //  0026  CALL	R5	3
      0x88180B14,  //  0027  GETMBR	R6	R5	K20
      0x8C1C0515,  //  0028  GETMET	R7	R2	K21
      0x88240116,  //  0029  GETMBR	R9	R0	K22
      0x88241317,  //  002A  GETMBR	R9	R9	K23
      0x4C280000,  //  002B  LDNIL	R10
      0x882C0116,  //  002C  GETMBR	R11	R0	K22
      0x882C1718,  //  002D  GETMBR	R11	R11	K24
      0x7C1C0800,  //  002E  CALL	R7	4
      0x8C200519,  //  002F  GETMET	R8	R2	K25
      0x542A001F,  //  0030  LDINT	R10	32
      0x7C200400,  //  0031  CALL	R8	2
      0x8C240F1A,  //  0032  GETMET	R9	R7	K26
      0x5C2C1000,  //  0033  MOVE	R11	R8
      0x7C240400,  //  0034  CALL	R9	2
      0x8C240F1B,  //  0035  GETMET	R9	R7	K27
      0x5C2C0C00,  //  0036  MOVE	R11	R6
      0x7C240400,  //  0037  CALL	R9	2
      0x8C24051C,  //  0038  GETMET	R9	R2	K28
      0x7C240200,  //  0039  CALL	R9	1
      0x8C28131D,  //  003A  GETMET	R10	R9	K29
      0x60300015,  //  003B  GETGBL	R12	G21
      0x7C300000,  //  003C  CALL	R12	0
      0x8C30191E,  //  003D  GETMET	R12	R12	K30
      0x8838011F,  //  003E  GETMBR	R14	R0	K31
      0x7C300400,  //  003F  CALL	R12	2
      0x7C280400,  //  0040  CALL	R10	2
      0x8C28131D,  //  0041  GETMET	R10	R9	K29
      0x88300920,  //  0042  GETMBR	R12	R4	K32
      0x7C280400,  //  0043  CALL	R10	2
      0x8C28131D,  //  0044  GETMET	R10	R9	K29
      0x88300921,  //  0045  GETMBR	R12	R4	K33
      0x7C280400,  //  0046  CALL	R10	2
      0x8C281322,  //  0047  GETMET	R10	R9	K34
      0x7C280200,  //  0048  CALL	R10	1
      0x901E2806,  //  0049  SETMBR	R7	K20	R6
      0x8C2C0F23,  //  004A  GETMET	R11	R7	K35
      0x5C341400,  //  004B  MOVE	R13	R10
      0x7C2C0400,  //  004C  CALL	R11	2
      0x8C2C0F24,  //  004D  GETMET	R11	R7	K36
      0x50340200,  //  004E  LDBOOL	R13	1	0
      0x7C2C0400,  //  004F  CALL	R11	2
      0xB82E1E00,  //  0050  GETNGBL	R11	K15
      0x8C2C1725,  //  0051  GETMET	R11	R11	K37
      0x7C2C0200,  //  0052  CALL	R11	1
      0x88300F26,  //  0053  GETMBR	R12	R7	K38
      0x902E4C0C,  //  0054  SETMBR	R11	K38	R12
      0x88300F27,  //  0055  GETMBR	R12	R7	K39
      0x902E4E0C,  //  0056  SETMBR	R11	K39	R12
      0x8C301728,  //  0057  GETMET	R12	R11	K40
      0x7C300200,  //  0058  CALL	R12	1
      0x88340F2A,  //  0059  GETMBR	R13	R7	K42
      0x9012520D,  //  005A  SETMBR	R4	K41	R13
      0x88340F2C,  //  005B  GETMBR	R13	R7	K44
      0x9012560D,  //  005C  SETMBR	R4	K43	R13
      0x8C34032D,  //  005D  GETMET	R13	R1	K45
      0x543E0022,  //  005E  LDINT	R15	35
      0x50400200,  //  005F  LDBOOL	R16	1	0
      0x7C340600,  //  0060  CALL	R13	3
      0x8C381B2E,  //  0061  GETMET	R14	R13	K46
      0x5C401800,  //  0062  MOVE	R16	R12
      0x7C380400,  //  0063  CALL	R14	2
      0xB83E0E00,  //  0064  GETNGBL	R15	K7
      0x8C3C1F08,  //  0065  GETMET	R15	R15	K8
      0x8C44072F,  //  0066  GETMET	R17	R3	K47
      0x584C0030,  //  0067  LDCONST	R19	K48
      0x88500931,  //  0068  GETMBR	R20	R4	K49
      0x88540932,  //  0069  GETMBR	R21	R4	K50
      0x88580933,  //  006A  GETMBR	R22	R4	K51
      0x7C440A00,  //  006B  CALL	R17	5
      0x7C3C0400,  //  006C  CALL	R15	2
      0x883C0134,  //  006D  GETMBR	R15	R0	K52
      0x8C3C1F35,  //  006E  GETMET	R15	R15	K53
      0x5C441A00,  //  006F  MOVE	R17	R13
      0x7C3C0400,  //  0070  CALL	R15	2
      0x503C0200,  //  0071  LDBOOL	R15	1	0
      0x80041E00,  //  0072  RET	1	R15
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
    ( &(const bvalue[32]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake3_X20message),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K11  */  be_nested_str_weak(send_status_report),
    /* K12  */  be_const_int(1),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(matter),
    /* K15  */  be_nested_str_weak(Pake3),
    /* K16  */  be_nested_str_weak(parse),
    /* K17  */  be_nested_str_weak(raw),
    /* K18  */  be_nested_str_weak(app_payload_idx),
    /* K19  */  be_nested_str_weak(cA),
    /* K20  */  be_nested_str_weak(__spake_cA),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20invalid_X20cA_X20received),
    /* K22  */  be_nested_str_weak(rtc),
    /* K23  */  be_nested_str_weak(utc),
    /* K24  */  be_nested_str_weak(HKDF_SHA256),
    /* K25  */  be_nested_str_weak(derive),
    /* K26  */  be_nested_str_weak(__spake_Ke),
    /* K27  */  be_nested_str_weak(fromstring),
    /* K28  */  be_nested_str_weak(SEKeys_Info),
    /* K29  */  be_nested_str_weak(add_session),
    /* K30  */  be_nested_str_weak(__future_local_session_id),
    /* K31  */  be_nested_str_weak(__future_initiator_session_id),
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
      0x5824000D,  //  001A  LDCONST	R9	K13
      0x50280000,  //  001B  LDBOOL	R10	0	0
      0x7C100C00,  //  001C  CALL	R4	6
      0x50100000,  //  001D  LDBOOL	R4	0	0
      0x80040800,  //  001E  RET	1	R4
      0xB8121C00,  //  001F  GETNGBL	R4	K14
      0x8C10090F,  //  0020  GETMET	R4	R4	K15
      0x7C100200,  //  0021  CALL	R4	1
      0x8C100910,  //  0022  GETMET	R4	R4	K16
      0x88180311,  //  0023  GETMBR	R6	R1	K17
      0x881C0312,  //  0024  GETMBR	R7	R1	K18
      0x7C100600,  //  0025  CALL	R4	3
      0x88140913,  //  0026  GETMBR	R5	R4	K19
      0x88180714,  //  0027  GETMBR	R6	R3	K20
      0x20180A06,  //  0028  NE	R6	R5	R6
      0x781A0012,  //  0029  JMPF	R6	#003D
      0xB81A0C00,  //  002A  GETNGBL	R6	K6
      0x8C180D07,  //  002B  GETMET	R6	R6	K7
      0x58200015,  //  002C  LDCONST	R8	K21
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
      0x582C000D,  //  0038  LDCONST	R11	K13
      0x50300000,  //  0039  LDBOOL	R12	0	0
      0x7C180C00,  //  003A  CALL	R6	6
      0x50180000,  //  003B  LDBOOL	R6	0	0
      0x80040C00,  //  003C  RET	1	R6
      0xB81A0C00,  //  003D  GETNGBL	R6	K6
      0x8C180D16,  //  003E  GETMET	R6	R6	K22
      0x7C180200,  //  003F  CALL	R6	1
      0x94180D17,  //  0040  GETIDX	R6	R6	K23
      0x8C1C0518,  //  0041  GETMET	R7	R2	K24
      0x7C1C0200,  //  0042  CALL	R7	1
      0x8C1C0F19,  //  0043  GETMET	R7	R7	K25
      0x8824071A,  //  0044  GETMBR	R9	R3	K26
      0x60280015,  //  0045  GETGBL	R10	G21
      0x7C280000,  //  0046  CALL	R10	0
      0x602C0015,  //  0047  GETGBL	R11	G21
      0x7C2C0000,  //  0048  CALL	R11	0
      0x8C2C171B,  //  0049  GETMET	R11	R11	K27
      0x8834011C,  //  004A  GETMBR	R13	R0	K28
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
      0x8C2C011D,  //  0060  GETMET	R11	R0	K29
      0x8834071E,  //  0061  GETMBR	R13	R3	K30
      0x8838071F,  //  0062  GETMBR	R14	R3	K31
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
    ( &(const bvalue[67]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(send_status_report),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(session),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(Sigma3),
    /* K15  */  be_nested_str_weak(parse),
    /* K16  */  be_nested_str_weak(raw),
    /* K17  */  be_nested_str_weak(app_payload_idx),
    /* K18  */  be_nested_str_weak(SHA256),
    /* K19  */  be_nested_str_weak(update),
    /* K20  */  be_nested_str_weak(__Msg1),
    /* K21  */  be_nested_str_weak(__Msg2),
    /* K22  */  be_nested_str_weak(out),
    /* K23  */  be_nested_str_weak(fromstring),
    /* K24  */  be_nested_str_weak(S3K_Info),
    /* K25  */  be_nested_str_weak(HKDF_SHA256),
    /* K26  */  be_nested_str_weak(derive),
    /* K27  */  be_nested_str_weak(shared_secret),
    /* K28  */  be_nested_str_weak(get_ipk_group_key),
    /* K29  */  be_nested_str_weak(TBEData3Encrypted),
    /* K30  */  be_const_int(2147483647),
    /* K31  */  be_nested_str_weak(AES_CCM),
    /* K32  */  be_nested_str_weak(TBEData3_Nonce),
    /* K33  */  be_nested_str_weak(decrypt),
    /* K34  */  be_nested_str_weak(tag),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20Tag_X20don_X27t_X20match),
    /* K36  */  be_nested_str_weak(TLV),
    /* K37  */  be_nested_str_weak(findsubval),
    /* K38  */  be_nested_str_weak(findsub),
    /* K39  */  be_nested_str_weak(int),
    /* K40  */  be_nested_str_weak(peer_node_id),
    /* K41  */  be_nested_str_weak(int64),
    /* K42  */  be_nested_str_weak(fromu32),
    /* K43  */  be_nested_str_weak(tobytes),
    /* K44  */  be_nested_str_weak(Matter_TLV_struct),
    /* K45  */  be_nested_str_weak(add_TLV),
    /* K46  */  be_nested_str_weak(B1),
    /* K47  */  be_nested_str_weak(__initiator_pub),
    /* K48  */  be_nested_str_weak(__responder_pub),
    /* K49  */  be_nested_str_weak(tlv2raw),
    /* K50  */  be_nested_str_weak(EC_P256),
    /* K51  */  be_nested_str_weak(ecdsa_verify_sha256),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20sigma3_tbs_X20does_X20not_X20have_X20a_X20valid_X20signature),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X20Invalid_X20signature_X2C_X20trying_X20anyways),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20Sigma3_X20verified_X2C_X20computing_X20new_X20keys),
    /* K55  */  be_nested_str_weak(Msg3),
    /* K56  */  be_nested_str_weak(SEKeys_Info),
    /* K57  */  be_nested_str_weak(rtc),
    /* K58  */  be_nested_str_weak(utc),
    /* K59  */  be_nested_str_weak(close),
    /* K60  */  be_nested_str_weak(set_keys),
    /* K61  */  be_nested_str_weak(_breadcrumb),
    /* K62  */  be_nested_str_weak(counter_snd_next),
    /* K63  */  be_nested_str_weak(set_persist),
    /* K64  */  be_nested_str_weak(set_no_expiration),
    /* K65  */  be_nested_str_weak(persist_to_fabric),
    /* K66  */  be_nested_str_weak(save),
    }),
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[287]) {  /* code */
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
      0x5820000B,  //  0014  LDCONST	R8	K11
      0x50240000,  //  0015  LDBOOL	R9	0	0
      0x7C0C0C00,  //  0016  CALL	R3	6
      0x500C0000,  //  0017  LDBOOL	R3	0	0
      0x80040600,  //  0018  RET	1	R3
      0x880C030C,  //  0019  GETMBR	R3	R1	K12
      0xB8121A00,  //  001A  GETNGBL	R4	K13
      0x8C10090E,  //  001B  GETMET	R4	R4	K14
      0x7C100200,  //  001C  CALL	R4	1
      0x8C10090F,  //  001D  GETMET	R4	R4	K15
      0x88180310,  //  001E  GETMBR	R6	R1	K16
      0x881C0311,  //  001F  GETMBR	R7	R1	K17
      0x7C100600,  //  0020  CALL	R4	3
      0x8C140512,  //  0021  GETMET	R5	R2	K18
      0x7C140200,  //  0022  CALL	R5	1
      0x8C140B13,  //  0023  GETMET	R5	R5	K19
      0x881C0714,  //  0024  GETMBR	R7	R3	K20
      0x7C140400,  //  0025  CALL	R5	2
      0x8C140B13,  //  0026  GETMET	R5	R5	K19
      0x881C0715,  //  0027  GETMBR	R7	R3	K21
      0x7C140400,  //  0028  CALL	R5	2
      0x8C140B16,  //  0029  GETMET	R5	R5	K22
      0x7C140200,  //  002A  CALL	R5	1
      0x60180015,  //  002B  GETGBL	R6	G21
      0x7C180000,  //  002C  CALL	R6	0
      0x8C180D17,  //  002D  GETMET	R6	R6	K23
      0x88200118,  //  002E  GETMBR	R8	R0	K24
      0x7C180400,  //  002F  CALL	R6	2
      0x8C1C0519,  //  0030  GETMET	R7	R2	K25
      0x7C1C0200,  //  0031  CALL	R7	1
      0x8C1C0F1A,  //  0032  GETMET	R7	R7	K26
      0x8824071B,  //  0033  GETMBR	R9	R3	K27
      0x8C28071C,  //  0034  GETMET	R10	R3	K28
      0x7C280200,  //  0035  CALL	R10	1
      0x00281405,  //  0036  ADD	R10	R10	R5
      0x5C2C0C00,  //  0037  MOVE	R11	R6
      0x5432000F,  //  0038  LDINT	R12	16
      0x7C1C0A00,  //  0039  CALL	R7	5
      0x5421FFEE,  //  003A  LDINT	R8	-17
      0x40220608,  //  003B  CONNECT	R8	K3	R8
      0x8824091D,  //  003C  GETMBR	R9	R4	K29
      0x94201208,  //  003D  GETIDX	R8	R9	R8
      0x5429FFEF,  //  003E  LDINT	R10	-16
      0x4028151E,  //  003F  CONNECT	R10	R10	K30
      0x882C091D,  //  0040  GETMBR	R11	R4	K29
      0x9424160A,  //  0041  GETIDX	R9	R11	R10
      0x8C30051F,  //  0042  GETMET	R12	R2	K31
      0x5C380E00,  //  0043  MOVE	R14	R7
      0x603C0015,  //  0044  GETGBL	R15	G21
      0x7C3C0000,  //  0045  CALL	R15	0
      0x8C3C1F17,  //  0046  GETMET	R15	R15	K23
      0x88440120,  //  0047  GETMBR	R17	R0	K32
      0x7C3C0400,  //  0048  CALL	R15	2
      0x60400015,  //  0049  GETGBL	R16	G21
      0x7C400000,  //  004A  CALL	R16	0
      0x6044000C,  //  004B  GETGBL	R17	G12
      0x5C481000,  //  004C  MOVE	R18	R8
      0x7C440200,  //  004D  CALL	R17	1
      0x544A000F,  //  004E  LDINT	R18	16
      0x7C300C00,  //  004F  CALL	R12	6
      0x5C281800,  //  0050  MOVE	R10	R12
      0x8C301521,  //  0051  GETMET	R12	R10	K33
      0x5C381000,  //  0052  MOVE	R14	R8
      0x7C300400,  //  0053  CALL	R12	2
      0x5C2C1800,  //  0054  MOVE	R11	R12
      0x8C301522,  //  0055  GETMET	R12	R10	K34
      0x7C300200,  //  0056  CALL	R12	1
      0x20341809,  //  0057  NE	R13	R12	R9
      0x78360012,  //  0058  JMPF	R13	#006C
      0xB8360A00,  //  0059  GETNGBL	R13	K5
      0x8C341B06,  //  005A  GETMET	R13	R13	K6
      0x583C0023,  //  005B  LDCONST	R15	K35
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
      0x5848000B,  //  0067  LDCONST	R18	K11
      0x504C0000,  //  0068  LDBOOL	R19	0	0
      0x7C340C00,  //  0069  CALL	R13	6
      0x50340000,  //  006A  LDBOOL	R13	0	0
      0x80041A00,  //  006B  RET	1	R13
      0xB8361A00,  //  006C  GETNGBL	R13	K13
      0x88341B24,  //  006D  GETMBR	R13	R13	K36
      0x8C341B0F,  //  006E  GETMET	R13	R13	K15
      0x5C3C1600,  //  006F  MOVE	R15	R11
      0x7C340400,  //  0070  CALL	R13	2
      0x8C381B25,  //  0071  GETMET	R14	R13	K37
      0x5840000A,  //  0072  LDCONST	R16	K10
      0x7C380400,  //  0073  CALL	R14	2
      0x8C3C1B25,  //  0074  GETMET	R15	R13	K37
      0x5844000B,  //  0075  LDCONST	R17	K11
      0x7C3C0400,  //  0076  CALL	R15	2
      0x8C401B25,  //  0077  GETMET	R16	R13	K37
      0x58480008,  //  0078  LDCONST	R18	K8
      0x7C400400,  //  0079  CALL	R16	2
      0xB8461A00,  //  007A  GETNGBL	R17	K13
      0x88442324,  //  007B  GETMBR	R17	R17	K36
      0x8C44230F,  //  007C  GETMET	R17	R17	K15
      0x5C4C1C00,  //  007D  MOVE	R19	R14
      0x7C440400,  //  007E  CALL	R17	2
      0x8C482325,  //  007F  GETMET	R18	R17	K37
      0x54520008,  //  0080  LDINT	R20	9
      0x7C480400,  //  0081  CALL	R18	2
      0x8C4C2326,  //  0082  GETMET	R19	R17	K38
      0x54560005,  //  0083  LDINT	R21	6
      0x7C4C0400,  //  0084  CALL	R19	2
      0x8C502725,  //  0085  GETMET	R20	R19	K37
      0x545A0010,  //  0086  LDINT	R22	17
      0x7C500400,  //  0087  CALL	R20	2
      0x60540004,  //  0088  GETGBL	R21	G4
      0x5C582800,  //  0089  MOVE	R22	R20
      0x7C540200,  //  008A  CALL	R21	1
      0x1C542B27,  //  008B  EQ	R21	R21	K39
      0x78560007,  //  008C  JMPF	R21	#0095
      0xB8565200,  //  008D  GETNGBL	R21	K41
      0x8C542B2A,  //  008E  GETMET	R21	R21	K42
      0x5C5C2800,  //  008F  MOVE	R23	R20
      0x7C540400,  //  0090  CALL	R21	2
      0x8C542B2B,  //  0091  GETMET	R21	R21	K43
      0x7C540200,  //  0092  CALL	R21	1
      0x900E5015,  //  0093  SETMBR	R3	K40	R21
      0x70020002,  //  0094  JMP		#0098
      0x8C54292B,  //  0095  GETMET	R21	R20	K43
      0x7C540200,  //  0096  CALL	R21	1
      0x900E5015,  //  0097  SETMBR	R3	K40	R21
      0xB8561A00,  //  0098  GETNGBL	R21	K13
      0x88542B24,  //  0099  GETMBR	R21	R21	K36
      0x8C542B2C,  //  009A  GETMET	R21	R21	K44
      0x7C540200,  //  009B  CALL	R21	1
      0x8C582B2D,  //  009C  GETMET	R22	R21	K45
      0x5860000A,  //  009D  LDCONST	R24	K10
      0xB8661A00,  //  009E  GETNGBL	R25	K13
      0x88643324,  //  009F  GETMBR	R25	R25	K36
      0x8864332E,  //  00A0  GETMBR	R25	R25	K46
      0x5C681C00,  //  00A1  MOVE	R26	R14
      0x7C580800,  //  00A2  CALL	R22	4
      0x8C582B2D,  //  00A3  GETMET	R22	R21	K45
      0x5860000B,  //  00A4  LDCONST	R24	K11
      0xB8661A00,  //  00A5  GETNGBL	R25	K13
      0x88643324,  //  00A6  GETMBR	R25	R25	K36
      0x8864332E,  //  00A7  GETMBR	R25	R25	K46
      0x5C681E00,  //  00A8  MOVE	R26	R15
      0x7C580800,  //  00A9  CALL	R22	4
      0x8C582B2D,  //  00AA  GETMET	R22	R21	K45
      0x58600008,  //  00AB  LDCONST	R24	K8
      0xB8661A00,  //  00AC  GETNGBL	R25	K13
      0x88643324,  //  00AD  GETMBR	R25	R25	K36
      0x8864332E,  //  00AE  GETMBR	R25	R25	K46
      0x8868072F,  //  00AF  GETMBR	R26	R3	K47
      0x7C580800,  //  00B0  CALL	R22	4
      0x8C582B2D,  //  00B1  GETMET	R22	R21	K45
      0x54620003,  //  00B2  LDINT	R24	4
      0xB8661A00,  //  00B3  GETNGBL	R25	K13
      0x88643324,  //  00B4  GETMBR	R25	R25	K36
      0x8864332E,  //  00B5  GETMBR	R25	R25	K46
      0x88680730,  //  00B6  GETMBR	R26	R3	K48
      0x7C580800,  //  00B7  CALL	R22	4
      0x8C582B31,  //  00B8  GETMET	R22	R21	K49
      0x7C580200,  //  00B9  CALL	R22	1
      0x8C5C0532,  //  00BA  GETMET	R23	R2	K50
      0x7C5C0200,  //  00BB  CALL	R23	1
      0x8C5C2F33,  //  00BC  GETMET	R23	R23	K51
      0x5C642400,  //  00BD  MOVE	R25	R18
      0x5C682C00,  //  00BE  MOVE	R26	R22
      0x5C6C2000,  //  00BF  MOVE	R27	R16
      0x7C5C0800,  //  00C0  CALL	R23	4
      0x5C602E00,  //  00C1  MOVE	R24	R23
      0x7462000A,  //  00C2  JMPT	R24	#00CE
      0xB8620A00,  //  00C3  GETNGBL	R24	K5
      0x8C603106,  //  00C4  GETMET	R24	R24	K6
      0x58680034,  //  00C5  LDCONST	R26	K52
      0x586C000B,  //  00C6  LDCONST	R27	K11
      0x7C600600,  //  00C7  CALL	R24	3
      0xB8620A00,  //  00C8  GETNGBL	R24	K5
      0x8C603106,  //  00C9  GETMET	R24	R24	K6
      0x58680035,  //  00CA  LDCONST	R26	K53
      0x586C000B,  //  00CB  LDCONST	R27	K11
      0x7C600600,  //  00CC  CALL	R24	3
      0x70020004,  //  00CD  JMP		#00D3
      0xB8620A00,  //  00CE  GETNGBL	R24	K5
      0x8C603106,  //  00CF  GETMET	R24	R24	K6
      0x58680036,  //  00D0  LDCONST	R26	K54
      0x586C0008,  //  00D1  LDCONST	R27	K8
      0x7C600600,  //  00D2  CALL	R24	3
      0x8C600512,  //  00D3  GETMET	R24	R2	K18
      0x7C600200,  //  00D4  CALL	R24	1
      0x8C603113,  //  00D5  GETMET	R24	R24	K19
      0x88680714,  //  00D6  GETMBR	R26	R3	K20
      0x7C600400,  //  00D7  CALL	R24	2
      0x8C603113,  //  00D8  GETMET	R24	R24	K19
      0x88680715,  //  00D9  GETMBR	R26	R3	K21
      0x7C600400,  //  00DA  CALL	R24	2
      0x8C603113,  //  00DB  GETMET	R24	R24	K19
      0x88680937,  //  00DC  GETMBR	R26	R4	K55
      0x7C600400,  //  00DD  CALL	R24	2
      0x8C603116,  //  00DE  GETMET	R24	R24	K22
      0x7C600200,  //  00DF  CALL	R24	1
      0x5C143000,  //  00E0  MOVE	R5	R24
      0x4C600000,  //  00E1  LDNIL	R24
      0x900E2818,  //  00E2  SETMBR	R3	K20	R24
      0x4C600000,  //  00E3  LDNIL	R24
      0x900E2A18,  //  00E4  SETMBR	R3	K21	R24
      0x8C600519,  //  00E5  GETMET	R24	R2	K25
      0x7C600200,  //  00E6  CALL	R24	1
      0x8C60311A,  //  00E7  GETMET	R24	R24	K26
      0x8868071B,  //  00E8  GETMBR	R26	R3	K27
      0x8C6C071C,  //  00E9  GETMET	R27	R3	K28
      0x7C6C0200,  //  00EA  CALL	R27	1
      0x006C3605,  //  00EB  ADD	R27	R27	R5
      0x60700015,  //  00EC  GETGBL	R28	G21
      0x7C700000,  //  00ED  CALL	R28	0
      0x8C703917,  //  00EE  GETMET	R28	R28	K23
      0x88780138,  //  00EF  GETMBR	R30	R0	K56
      0x7C700400,  //  00F0  CALL	R28	2
      0x5476002F,  //  00F1  LDINT	R29	48
      0x7C600A00,  //  00F2  CALL	R24	5
      0x5466000E,  //  00F3  LDINT	R25	15
      0x40660619,  //  00F4  CONNECT	R25	K3	R25
      0x94643019,  //  00F5  GETIDX	R25	R24	R25
      0x546A000F,  //  00F6  LDINT	R26	16
      0x546E001E,  //  00F7  LDINT	R27	31
      0x4068341B,  //  00F8  CONNECT	R26	R26	R27
      0x9468301A,  //  00F9  GETIDX	R26	R24	R26
      0x546E001F,  //  00FA  LDINT	R27	32
      0x5472002E,  //  00FB  LDINT	R28	47
      0x406C361C,  //  00FC  CONNECT	R27	R27	R28
      0x946C301B,  //  00FD  GETIDX	R27	R24	R27
      0xB8720A00,  //  00FE  GETNGBL	R28	K5
      0x8C703939,  //  00FF  GETMET	R28	R28	K57
      0x7C700200,  //  0100  CALL	R28	1
      0x9470393A,  //  0101  GETIDX	R28	R28	K58
      0x8C740109,  //  0102  GETMET	R29	R0	K9
      0x5C7C0200,  //  0103  MOVE	R31	R1
      0x58800003,  //  0104  LDCONST	R32	K3
      0x58840003,  //  0105  LDCONST	R33	K3
      0x58880003,  //  0106  LDCONST	R34	K3
      0x508C0200,  //  0107  LDBOOL	R35	1	0
      0x7C740C00,  //  0108  CALL	R29	6
      0x8C74073B,  //  0109  GETMET	R29	R3	K59
      0x7C740200,  //  010A  CALL	R29	1
      0x8C74073C,  //  010B  GETMET	R29	R3	K60
      0x5C7C3200,  //  010C  MOVE	R31	R25
      0x5C803400,  //  010D  MOVE	R32	R26
      0x5C843600,  //  010E  MOVE	R33	R27
      0x5C883800,  //  010F  MOVE	R34	R28
      0x7C740A00,  //  0110  CALL	R29	5
      0x900E7B03,  //  0111  SETMBR	R3	K61	K3
      0x8C74073E,  //  0112  GETMET	R29	R3	K62
      0x7C740200,  //  0113  CALL	R29	1
      0x8C74073F,  //  0114  GETMET	R29	R3	K63
      0x507C0200,  //  0115  LDBOOL	R31	1	0
      0x7C740400,  //  0116  CALL	R29	2
      0x8C740740,  //  0117  GETMET	R29	R3	K64
      0x7C740200,  //  0118  CALL	R29	1
      0x8C740741,  //  0119  GETMET	R29	R3	K65
      0x7C740200,  //  011A  CALL	R29	1
      0x8C740742,  //  011B  GETMET	R29	R3	K66
      0x7C740200,  //  011C  CALL	R29	1
      0x50740200,  //  011D  LDBOOL	R29	1	0
      0x80043A00,  //  011E  RET	1	R29
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
    ( &(const bvalue[106]) {     /* constants */
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
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(send_status_report),
    /* K12  */  be_const_int(1),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(matter),
    /* K15  */  be_nested_str_weak(Sigma1),
    /* K16  */  be_nested_str_weak(parse),
    /* K17  */  be_nested_str_weak(raw),
    /* K18  */  be_nested_str_weak(app_payload_idx),
    /* K19  */  be_nested_str_weak(__initiator_pub),
    /* K20  */  be_nested_str_weak(initiatorEphPubKey),
    /* K21  */  be_nested_str_weak(resumptionID),
    /* K22  */  be_nested_str_weak(initiatorResumeMIC),
    /* K23  */  be_nested_str_weak(device),
    /* K24  */  be_nested_str_weak(sessions),
    /* K25  */  be_nested_str_weak(find_session_by_resumption_id),
    /* K26  */  be_nested_str_weak(_fabric),
    /* K27  */  be_nested_str_weak(initiatorRandom),
    /* K28  */  be_nested_str_weak(fromstring),
    /* K29  */  be_nested_str_weak(Sigma1_Resume),
    /* K30  */  be_nested_str_weak(HKDF_SHA256),
    /* K31  */  be_nested_str_weak(derive),
    /* K32  */  be_nested_str_weak(shared_secret),
    /* K33  */  be_nested_str_weak(NCASE_SigmaS1),
    /* K34  */  be_const_int(2147483647),
    /* K35  */  be_nested_str_weak(AES_CCM),
    /* K36  */  be_nested_str_weak(decrypt),
    /* K37  */  be_nested_str_weak(tag),
    /* K38  */  be_nested_str_weak(_source_node_id),
    /* K39  */  be_nested_str_weak(source_node_id),
    /* K40  */  be_nested_str_weak(set_mode_CASE),
    /* K41  */  be_nested_str_weak(__future_initiator_session_id),
    /* K42  */  be_nested_str_weak(initiator_session_id),
    /* K43  */  be_nested_str_weak(__future_local_session_id),
    /* K44  */  be_nested_str_weak(gen_local_session_id),
    /* K45  */  be_nested_str_weak(format),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20New_Session_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K47  */  be_nested_str_weak(remote_ip),
    /* K48  */  be_nested_str_weak(remote_port),
    /* K49  */  be_nested_str_weak(resumption_id),
    /* K50  */  be_nested_str_weak(random),
    /* K51  */  be_nested_str_weak(Sigma2_Resume),
    /* K52  */  be_nested_str_weak(NCASE_SigmaS2),
    /* K53  */  be_nested_str_weak(Sigma2Resume),
    /* K54  */  be_nested_str_weak(responderSessionID),
    /* K55  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K56  */  be_nested_str_weak(SessionResumptionKeys),
    /* K57  */  be_nested_str_weak(rtc),
    /* K58  */  be_nested_str_weak(utc),
    /* K59  */  be_nested_str_weak(tlv2raw),
    /* K60  */  be_nested_str_weak(__Msg1),
    /* K61  */  be_nested_str_weak(build_response),
    /* K62  */  be_nested_str_weak(encode_frame),
    /* K63  */  be_nested_str_weak(responder),
    /* K64  */  be_nested_str_weak(send_response_frame),
    /* K65  */  be_nested_str_weak(close),
    /* K66  */  be_nested_str_weak(set_keys),
    /* K67  */  be_nested_str_weak(_breadcrumb),
    /* K68  */  be_nested_str_weak(counter_snd_next),
    /* K69  */  be_nested_str_weak(set_persist),
    /* K70  */  be_nested_str_weak(set_no_expiration),
    /* K71  */  be_nested_str_weak(persist_to_fabric),
    /* K72  */  be_nested_str_weak(save),
    /* K73  */  be_nested_str_weak(find_fabric_by_destination_id),
    /* K74  */  be_nested_str_weak(destinationId),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K76  */  be_nested_str_weak(__responder_priv),
    /* K77  */  be_nested_str_weak(__responder_pub),
    /* K78  */  be_nested_str_weak(EC_P256),
    /* K79  */  be_nested_str_weak(public_key),
    /* K80  */  be_nested_str_weak(shared_key),
    /* K81  */  be_nested_str_weak(TLV),
    /* K82  */  be_nested_str_weak(Matter_TLV_struct),
    /* K83  */  be_nested_str_weak(add_TLV),
    /* K84  */  be_nested_str_weak(B2),
    /* K85  */  be_nested_str_weak(get_noc),
    /* K86  */  be_nested_str_weak(get_icac),
    /* K87  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K88  */  be_nested_str_weak(get_pk),
    /* K89  */  be_nested_str_weak(Msg1),
    /* K90  */  be_nested_str_weak(SHA256),
    /* K91  */  be_nested_str_weak(update),
    /* K92  */  be_nested_str_weak(out),
    /* K93  */  be_nested_str_weak(S2K_Info),
    /* K94  */  be_nested_str_weak(get_ipk_group_key),
    /* K95  */  be_nested_str_weak(TBEData2_Nonce),
    /* K96  */  be_nested_str_weak(encrypt),
    /* K97  */  be_nested_str_weak(Sigma2),
    /* K98  */  be_nested_str_weak(responderRandom),
    /* K99  */  be_nested_str_weak(responderSessionId),
    /* K100 */  be_nested_str_weak(responderEphPubKey),
    /* K101 */  be_nested_str_weak(encrypted2),
    /* K102 */  be_nested_str_weak(__Msg2),
    /* K103 */  be_nested_str_weak(MTR_X3A_X20New_X20Connection_X20_X28CASE_X20id_X3D_X25i_X29_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K104 */  be_nested_str_weak(_ip),
    /* K105 */  be_nested_str_weak(_port),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[482]) {  /* code */
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
      0x5828000D,  //  0016  LDCONST	R10	K13
      0x502C0000,  //  0017  LDBOOL	R11	0	0
      0x7C140C00,  //  0018  CALL	R5	6
      0x50140000,  //  0019  LDBOOL	R5	0	0
      0x80040A00,  //  001A  RET	1	R5
      0xB8161C00,  //  001B  GETNGBL	R5	K14
      0x8C140B0F,  //  001C  GETMET	R5	R5	K15
      0x7C140200,  //  001D  CALL	R5	1
      0x8C140B10,  //  001E  GETMET	R5	R5	K16
      0x881C0311,  //  001F  GETMBR	R7	R1	K17
      0x88200312,  //  0020  GETMBR	R8	R1	K18
      0x7C140600,  //  0021  CALL	R5	3
      0x88180B14,  //  0022  GETMBR	R6	R5	K20
      0x90122606,  //  0023  SETMBR	R4	K19	R6
      0x88180B15,  //  0024  GETMBR	R6	R5	K21
      0x4C1C0000,  //  0025  LDNIL	R7
      0x20180C07,  //  0026  NE	R6	R6	R7
      0x781A0003,  //  0027  JMPF	R6	#002C
      0x88180B16,  //  0028  GETMBR	R6	R5	K22
      0x4C1C0000,  //  0029  LDNIL	R7
      0x20180C07,  //  002A  NE	R6	R6	R7
      0x741A0000,  //  002B  JMPT	R6	#002D
      0x50180001,  //  002C  LDBOOL	R6	0	1
      0x50180200,  //  002D  LDBOOL	R6	1	0
      0x50180000,  //  002E  LDBOOL	R6	0	0
      0x4C1C0000,  //  002F  LDNIL	R7
      0x781A000D,  //  0030  JMPF	R6	#003F
      0x88200117,  //  0031  GETMBR	R8	R0	K23
      0x88201118,  //  0032  GETMBR	R8	R8	K24
      0x8C201119,  //  0033  GETMET	R8	R8	K25
      0x88280B15,  //  0034  GETMBR	R10	R5	K21
      0x7C200400,  //  0035  CALL	R8	2
      0x5C1C1000,  //  0036  MOVE	R7	R8
      0x4C200000,  //  0037  LDNIL	R8
      0x1C200E08,  //  0038  EQ	R8	R7	R8
      0x74220003,  //  0039  JMPT	R8	#003E
      0x88200F1A,  //  003A  GETMBR	R8	R7	K26
      0x4C240000,  //  003B  LDNIL	R9
      0x1C201009,  //  003C  EQ	R8	R8	R9
      0x78220000,  //  003D  JMPF	R8	#003F
      0x50180000,  //  003E  LDBOOL	R6	0	0
      0x781A00B6,  //  003F  JMPF	R6	#00F7
      0x88200B1B,  //  0040  GETMBR	R8	R5	K27
      0x88240B15,  //  0041  GETMBR	R9	R5	K21
      0x00201009,  //  0042  ADD	R8	R8	R9
      0x60240015,  //  0043  GETGBL	R9	G21
      0x7C240000,  //  0044  CALL	R9	0
      0x8C24131C,  //  0045  GETMET	R9	R9	K28
      0x582C001D,  //  0046  LDCONST	R11	K29
      0x7C240400,  //  0047  CALL	R9	2
      0x8C28051E,  //  0048  GETMET	R10	R2	K30
      0x7C280200,  //  0049  CALL	R10	1
      0x8C28151F,  //  004A  GETMET	R10	R10	K31
      0x88300F20,  //  004B  GETMBR	R12	R7	K32
      0x5C341000,  //  004C  MOVE	R13	R8
      0x5C381200,  //  004D  MOVE	R14	R9
      0x543E000F,  //  004E  LDINT	R15	16
      0x7C280A00,  //  004F  CALL	R10	5
      0x602C0015,  //  0050  GETGBL	R11	G21
      0x7C2C0000,  //  0051  CALL	R11	0
      0x8C2C171C,  //  0052  GETMET	R11	R11	K28
      0x58340021,  //  0053  LDCONST	R13	K33
      0x7C2C0400,  //  0054  CALL	R11	2
      0x5431FFEE,  //  0055  LDINT	R12	-17
      0x40320A0C,  //  0056  CONNECT	R12	K5	R12
      0x88340B16,  //  0057  GETMBR	R13	R5	K22
      0x94301A0C,  //  0058  GETIDX	R12	R13	R12
      0x5439FFEF,  //  0059  LDINT	R14	-16
      0x40381D22,  //  005A  CONNECT	R14	R14	K34
      0x883C0B16,  //  005B  GETMBR	R15	R5	K22
      0x94341E0E,  //  005C  GETIDX	R13	R15	R14
      0x8C400523,  //  005D  GETMET	R16	R2	K35
      0x5C481400,  //  005E  MOVE	R18	R10
      0x5C4C1600,  //  005F  MOVE	R19	R11
      0x60500015,  //  0060  GETGBL	R20	G21
      0x7C500000,  //  0061  CALL	R20	0
      0x6054000C,  //  0062  GETGBL	R21	G12
      0x5C581800,  //  0063  MOVE	R22	R12
      0x7C540200,  //  0064  CALL	R21	1
      0x545A000F,  //  0065  LDINT	R22	16
      0x7C400C00,  //  0066  CALL	R16	6
      0x5C382000,  //  0067  MOVE	R14	R16
      0x8C401D24,  //  0068  GETMET	R16	R14	K36
      0x5C481800,  //  0069  MOVE	R18	R12
      0x7C400400,  //  006A  CALL	R16	2
      0x5C3C2000,  //  006B  MOVE	R15	R16
      0x8C401D25,  //  006C  GETMET	R16	R14	K37
      0x7C400200,  //  006D  CALL	R16	1
      0x1C441A10,  //  006E  EQ	R17	R13	R16
      0x78460085,  //  006F  JMPF	R17	#00F6
      0x88440F1A,  //  0070  GETMBR	R17	R7	K26
      0x90123411,  //  0071  SETMBR	R4	K26	R17
      0x88440327,  //  0072  GETMBR	R17	R1	K39
      0x90124C11,  //  0073  SETMBR	R4	K38	R17
      0x8C440928,  //  0074  GETMET	R17	R4	K40
      0x7C440200,  //  0075  CALL	R17	1
      0x88440B2A,  //  0076  GETMBR	R17	R5	K42
      0x90125211,  //  0077  SETMBR	R4	K41	R17
      0x88440117,  //  0078  GETMBR	R17	R0	K23
      0x88442318,  //  0079  GETMBR	R17	R17	K24
      0x8C44232C,  //  007A  GETMET	R17	R17	K44
      0x7C440200,  //  007B  CALL	R17	1
      0x90125611,  //  007C  SETMBR	R4	K43	R17
      0xB8460E00,  //  007D  GETNGBL	R17	K7
      0x8C442308,  //  007E  GETMET	R17	R17	K8
      0x8C4C072D,  //  007F  GETMET	R19	R3	K45
      0x5854002E,  //  0080  LDCONST	R21	K46
      0x8858092B,  //  0081  GETMBR	R22	R4	K43
      0x885C032F,  //  0082  GETMBR	R23	R1	K47
      0x88600330,  //  0083  GETMBR	R24	R1	K48
      0x7C4C0A00,  //  0084  CALL	R19	5
      0x5850000A,  //  0085  LDCONST	R20	K10
      0x7C440600,  //  0086  CALL	R17	3
      0x88440F20,  //  0087  GETMBR	R17	R7	K32
      0x90124011,  //  0088  SETMBR	R4	K32	R17
      0x8C440532,  //  0089  GETMET	R17	R2	K50
      0x544E000F,  //  008A  LDINT	R19	16
      0x7C440400,  //  008B  CALL	R17	2
      0x90126211,  //  008C  SETMBR	R4	K49	R17
      0x60440015,  //  008D  GETGBL	R17	G21
      0x7C440000,  //  008E  CALL	R17	0
      0x8C44231C,  //  008F  GETMET	R17	R17	K28
      0x584C0033,  //  0090  LDCONST	R19	K51
      0x7C440400,  //  0091  CALL	R17	2
      0x88480B1B,  //  0092  GETMBR	R18	R5	K27
      0x884C0931,  //  0093  GETMBR	R19	R4	K49
      0x00482413,  //  0094  ADD	R18	R18	R19
      0x8C4C051E,  //  0095  GETMET	R19	R2	K30
      0x7C4C0200,  //  0096  CALL	R19	1
      0x8C4C271F,  //  0097  GETMET	R19	R19	K31
      0x88540920,  //  0098  GETMBR	R21	R4	K32
      0x5C582400,  //  0099  MOVE	R22	R18
      0x5C5C2200,  //  009A  MOVE	R23	R17
      0x5462000F,  //  009B  LDINT	R24	16
      0x7C4C0A00,  //  009C  CALL	R19	5
      0x8C500523,  //  009D  GETMET	R20	R2	K35
      0x5C582600,  //  009E  MOVE	R22	R19
      0x605C0015,  //  009F  GETGBL	R23	G21
      0x7C5C0000,  //  00A0  CALL	R23	0
      0x8C5C2F1C,  //  00A1  GETMET	R23	R23	K28
      0x58640034,  //  00A2  LDCONST	R25	K52
      0x7C5C0400,  //  00A3  CALL	R23	2
      0x60600015,  //  00A4  GETGBL	R24	G21
      0x7C600000,  //  00A5  CALL	R24	0
      0x58640005,  //  00A6  LDCONST	R25	K5
      0x546A000F,  //  00A7  LDINT	R26	16
      0x7C500C00,  //  00A8  CALL	R20	6
      0x8C542925,  //  00A9  GETMET	R21	R20	K37
      0x7C540200,  //  00AA  CALL	R21	1
      0xB85A1C00,  //  00AB  GETNGBL	R22	K14
      0x8C582D35,  //  00AC  GETMET	R22	R22	K53
      0x7C580200,  //  00AD  CALL	R22	1
      0x885C0931,  //  00AE  GETMBR	R23	R4	K49
      0x905A2A17,  //  00AF  SETMBR	R22	K21	R23
      0x885C092B,  //  00B0  GETMBR	R23	R4	K43
      0x905A6C17,  //  00B1  SETMBR	R22	K54	R23
      0x905A6E15,  //  00B2  SETMBR	R22	K55	R21
      0x8C5C051E,  //  00B3  GETMET	R23	R2	K30
      0x7C5C0200,  //  00B4  CALL	R23	1
      0x8C5C2F1F,  //  00B5  GETMET	R23	R23	K31
      0x88640920,  //  00B6  GETMBR	R25	R4	K32
      0x88680B1B,  //  00B7  GETMBR	R26	R5	K27
      0x886C0931,  //  00B8  GETMBR	R27	R4	K49
      0x0068341B,  //  00B9  ADD	R26	R26	R27
      0x606C0015,  //  00BA  GETGBL	R27	G21
      0x7C6C0000,  //  00BB  CALL	R27	0
      0x8C6C371C,  //  00BC  GETMET	R27	R27	K28
      0x58740038,  //  00BD  LDCONST	R29	K56
      0x7C6C0400,  //  00BE  CALL	R27	2
      0x5472002F,  //  00BF  LDINT	R28	48
      0x7C5C0A00,  //  00C0  CALL	R23	5
      0x5462000E,  //  00C1  LDINT	R24	15
      0x40620A18,  //  00C2  CONNECT	R24	K5	R24
      0x94602E18,  //  00C3  GETIDX	R24	R23	R24
      0x5466000F,  //  00C4  LDINT	R25	16
      0x546A001E,  //  00C5  LDINT	R26	31
      0x4064321A,  //  00C6  CONNECT	R25	R25	R26
      0x94642E19,  //  00C7  GETIDX	R25	R23	R25
      0x546A001F,  //  00C8  LDINT	R26	32
      0x546E002E,  //  00C9  LDINT	R27	47
      0x4068341B,  //  00CA  CONNECT	R26	R26	R27
      0x94682E1A,  //  00CB  GETIDX	R26	R23	R26
      0xB86E0E00,  //  00CC  GETNGBL	R27	K7
      0x8C6C3739,  //  00CD  GETMET	R27	R27	K57
      0x7C6C0200,  //  00CE  CALL	R27	1
      0x946C373A,  //  00CF  GETIDX	R27	R27	K58
      0x8C702D3B,  //  00D0  GETMET	R28	R22	K59
      0x7C700200,  //  00D1  CALL	R28	1
      0x4C740000,  //  00D2  LDNIL	R29
      0x9012781D,  //  00D3  SETMBR	R4	K60	R29
      0x8C74033D,  //  00D4  GETMET	R29	R1	K61
      0x547E0032,  //  00D5  LDINT	R31	51
      0x50800200,  //  00D6  LDBOOL	R32	1	0
      0x7C740600,  //  00D7  CALL	R29	3
      0x8C783B3E,  //  00D8  GETMET	R30	R29	K62
      0x5C803800,  //  00D9  MOVE	R32	R28
      0x7C780400,  //  00DA  CALL	R30	2
      0x887C013F,  //  00DB  GETMBR	R31	R0	K63
      0x8C7C3F40,  //  00DC  GETMET	R31	R31	K64
      0x5C843A00,  //  00DD  MOVE	R33	R29
      0x7C7C0400,  //  00DE  CALL	R31	2
      0x8C7C0941,  //  00DF  GETMET	R31	R4	K65
      0x7C7C0200,  //  00E0  CALL	R31	1
      0x8C7C0942,  //  00E1  GETMET	R31	R4	K66
      0x5C843000,  //  00E2  MOVE	R33	R24
      0x5C883200,  //  00E3  MOVE	R34	R25
      0x5C8C3400,  //  00E4  MOVE	R35	R26
      0x5C903600,  //  00E5  MOVE	R36	R27
      0x7C7C0A00,  //  00E6  CALL	R31	5
      0x90128705,  //  00E7  SETMBR	R4	K67	K5
      0x8C7C0944,  //  00E8  GETMET	R31	R4	K68
      0x7C7C0200,  //  00E9  CALL	R31	1
      0x8C7C0945,  //  00EA  GETMET	R31	R4	K69
      0x50840200,  //  00EB  LDBOOL	R33	1	0
      0x7C7C0400,  //  00EC  CALL	R31	2
      0x8C7C0946,  //  00ED  GETMET	R31	R4	K70
      0x7C7C0200,  //  00EE  CALL	R31	1
      0x8C7C0947,  //  00EF  GETMET	R31	R4	K71
      0x7C7C0200,  //  00F0  CALL	R31	1
      0x8C7C0948,  //  00F1  GETMET	R31	R4	K72
      0x7C7C0200,  //  00F2  CALL	R31	1
      0x507C0200,  //  00F3  LDBOOL	R31	1	0
      0x80043E00,  //  00F4  RET	1	R31
      0x70020000,  //  00F5  JMP		#00F7
      0x50180000,  //  00F6  LDBOOL	R6	0	0
      0x5C200C00,  //  00F7  MOVE	R8	R6
      0x742200E6,  //  00F8  JMPT	R8	#01E0
      0x8C200149,  //  00F9  GETMET	R8	R0	K73
      0x88280B4A,  //  00FA  GETMBR	R10	R5	K74
      0x882C0B1B,  //  00FB  GETMBR	R11	R5	K27
      0x7C200600,  //  00FC  CALL	R8	3
      0x90123408,  //  00FD  SETMBR	R4	K26	R8
      0x4C240000,  //  00FE  LDNIL	R9
      0x1C240809,  //  00FF  EQ	R9	R4	R9
      0x74260003,  //  0100  JMPT	R9	#0105
      0x8824091A,  //  0101  GETMBR	R9	R4	K26
      0x4C280000,  //  0102  LDNIL	R10
      0x1C24120A,  //  0103  EQ	R9	R9	R10
      0x7826000D,  //  0104  JMPF	R9	#0113
      0xB8260E00,  //  0105  GETNGBL	R9	K7
      0x8C241308,  //  0106  GETMET	R9	R9	K8
      0x582C004B,  //  0107  LDCONST	R11	K75
      0x5830000A,  //  0108  LDCONST	R12	K10
      0x7C240600,  //  0109  CALL	R9	3
      0x8C24010B,  //  010A  GETMET	R9	R0	K11
      0x5C2C0200,  //  010B  MOVE	R11	R1
      0x5830000C,  //  010C  LDCONST	R12	K12
      0x58340005,  //  010D  LDCONST	R13	K5
      0x5838000C,  //  010E  LDCONST	R14	K12
      0x503C0000,  //  010F  LDBOOL	R15	0	0
      0x7C240C00,  //  0110  CALL	R9	6
      0x50240000,  //  0111  LDBOOL	R9	0	0
      0x80041200,  //  0112  RET	1	R9
      0x88240327,  //  0113  GETMBR	R9	R1	K39
      0x90124C09,  //  0114  SETMBR	R4	K38	R9
      0x8C240928,  //  0115  GETMET	R9	R4	K40
      0x7C240200,  //  0116  CALL	R9	1
      0x88240B2A,  //  0117  GETMBR	R9	R5	K42
      0x90125209,  //  0118  SETMBR	R4	K41	R9
      0x88240117,  //  0119  GETMBR	R9	R0	K23
      0x88241318,  //  011A  GETMBR	R9	R9	K24
      0x8C24132C,  //  011B  GETMET	R9	R9	K44
      0x7C240200,  //  011C  CALL	R9	1
      0x90125609,  //  011D  SETMBR	R4	K43	R9
      0xB8260E00,  //  011E  GETNGBL	R9	K7
      0x8C241308,  //  011F  GETMET	R9	R9	K8
      0x8C2C072D,  //  0120  GETMET	R11	R3	K45
      0x5834002E,  //  0121  LDCONST	R13	K46
      0x8838092B,  //  0122  GETMBR	R14	R4	K43
      0x883C032F,  //  0123  GETMBR	R15	R1	K47
      0x88400330,  //  0124  GETMBR	R16	R1	K48
      0x7C2C0A00,  //  0125  CALL	R11	5
      0x5830000A,  //  0126  LDCONST	R12	K10
      0x7C240600,  //  0127  CALL	R9	3
      0x8C240532,  //  0128  GETMET	R9	R2	K50
      0x542E000F,  //  0129  LDINT	R11	16
      0x7C240400,  //  012A  CALL	R9	2
      0x90126209,  //  012B  SETMBR	R4	K49	R9
      0x8C240532,  //  012C  GETMET	R9	R2	K50
      0x542E001F,  //  012D  LDINT	R11	32
      0x7C240400,  //  012E  CALL	R9	2
      0x90129809,  //  012F  SETMBR	R4	K76	R9
      0x8C24054E,  //  0130  GETMET	R9	R2	K78
      0x7C240200,  //  0131  CALL	R9	1
      0x8C24134F,  //  0132  GETMET	R9	R9	K79
      0x882C094C,  //  0133  GETMBR	R11	R4	K76
      0x7C240400,  //  0134  CALL	R9	2
      0x90129A09,  //  0135  SETMBR	R4	K77	R9
      0x8C240532,  //  0136  GETMET	R9	R2	K50
      0x542E001F,  //  0137  LDINT	R11	32
      0x7C240400,  //  0138  CALL	R9	2
      0x8C28054E,  //  0139  GETMET	R10	R2	K78
      0x7C280200,  //  013A  CALL	R10	1
      0x8C281550,  //  013B  GETMET	R10	R10	K80
      0x8830094C,  //  013C  GETMBR	R12	R4	K76
      0x88340B14,  //  013D  GETMBR	R13	R5	K20
      0x7C280600,  //  013E  CALL	R10	3
      0x9012400A,  //  013F  SETMBR	R4	K32	R10
      0xB82A1C00,  //  0140  GETNGBL	R10	K14
      0x88281551,  //  0141  GETMBR	R10	R10	K81
      0x8C281552,  //  0142  GETMET	R10	R10	K82
      0x7C280200,  //  0143  CALL	R10	1
      0x8C2C1553,  //  0144  GETMET	R11	R10	K83
      0x5834000C,  //  0145  LDCONST	R13	K12
      0xB83A1C00,  //  0146  GETNGBL	R14	K14
      0x88381D51,  //  0147  GETMBR	R14	R14	K81
      0x88381D54,  //  0148  GETMBR	R14	R14	K84
      0x8C3C1155,  //  0149  GETMET	R15	R8	K85
      0x7C3C0200,  //  014A  CALL	R15	1
      0x7C2C0800,  //  014B  CALL	R11	4
      0x8C2C1553,  //  014C  GETMET	R11	R10	K83
      0x5834000D,  //  014D  LDCONST	R13	K13
      0xB83A1C00,  //  014E  GETNGBL	R14	K14
      0x88381D51,  //  014F  GETMBR	R14	R14	K81
      0x88381D54,  //  0150  GETMBR	R14	R14	K84
      0x8C3C1156,  //  0151  GETMET	R15	R8	K86
      0x7C3C0200,  //  0152  CALL	R15	1
      0x7C2C0800,  //  0153  CALL	R11	4
      0x8C2C1553,  //  0154  GETMET	R11	R10	K83
      0x5834000A,  //  0155  LDCONST	R13	K10
      0xB83A1C00,  //  0156  GETNGBL	R14	K14
      0x88381D51,  //  0157  GETMBR	R14	R14	K81
      0x88381D54,  //  0158  GETMBR	R14	R14	K84
      0x883C094D,  //  0159  GETMBR	R15	R4	K77
      0x7C2C0800,  //  015A  CALL	R11	4
      0x8C2C1553,  //  015B  GETMET	R11	R10	K83
      0x54360003,  //  015C  LDINT	R13	4
      0xB83A1C00,  //  015D  GETNGBL	R14	K14
      0x88381D51,  //  015E  GETMBR	R14	R14	K81
      0x88381D54,  //  015F  GETMBR	R14	R14	K84
      0x883C0B14,  //  0160  GETMBR	R15	R5	K20
      0x7C2C0800,  //  0161  CALL	R11	4
      0x8C2C054E,  //  0162  GETMET	R11	R2	K78
      0x7C2C0200,  //  0163  CALL	R11	1
      0x8C2C1757,  //  0164  GETMET	R11	R11	K87
      0x8C341158,  //  0165  GETMET	R13	R8	K88
      0x7C340200,  //  0166  CALL	R13	1
      0x8C38153B,  //  0167  GETMET	R14	R10	K59
      0x7C380200,  //  0168  CALL	R14	1
      0x7C2C0600,  //  0169  CALL	R11	3
      0xB8321C00,  //  016A  GETNGBL	R12	K14
      0x88301951,  //  016B  GETMBR	R12	R12	K81
      0x8C301952,  //  016C  GETMET	R12	R12	K82
      0x7C300200,  //  016D  CALL	R12	1
      0x8C341953,  //  016E  GETMET	R13	R12	K83
      0x583C000C,  //  016F  LDCONST	R15	K12
      0xB8421C00,  //  0170  GETNGBL	R16	K14
      0x88402151,  //  0171  GETMBR	R16	R16	K81
      0x88402154,  //  0172  GETMBR	R16	R16	K84
      0x8C441155,  //  0173  GETMET	R17	R8	K85
      0x7C440200,  //  0174  CALL	R17	1
      0x7C340800,  //  0175  CALL	R13	4
      0x8C341953,  //  0176  GETMET	R13	R12	K83
      0x583C000D,  //  0177  LDCONST	R15	K13
      0xB8421C00,  //  0178  GETNGBL	R16	K14
      0x88402151,  //  0179  GETMBR	R16	R16	K81
      0x88402154,  //  017A  GETMBR	R16	R16	K84
      0x8C441156,  //  017B  GETMET	R17	R8	K86
      0x7C440200,  //  017C  CALL	R17	1
      0x7C340800,  //  017D  CALL	R13	4
      0x8C341953,  //  017E  GETMET	R13	R12	K83
      0x583C000A,  //  017F  LDCONST	R15	K10
      0xB8421C00,  //  0180  GETNGBL	R16	K14
      0x88402151,  //  0181  GETMBR	R16	R16	K81
      0x88402154,  //  0182  GETMBR	R16	R16	K84
      0x5C441600,  //  0183  MOVE	R17	R11
      0x7C340800,  //  0184  CALL	R13	4
      0x8C341953,  //  0185  GETMET	R13	R12	K83
      0x543E0003,  //  0186  LDINT	R15	4
      0xB8421C00,  //  0187  GETNGBL	R16	K14
      0x88402151,  //  0188  GETMBR	R16	R16	K81
      0x88402154,  //  0189  GETMBR	R16	R16	K84
      0x88440931,  //  018A  GETMBR	R17	R4	K49
      0x7C340800,  //  018B  CALL	R13	4
      0x88340B59,  //  018C  GETMBR	R13	R5	K89
      0x9012780D,  //  018D  SETMBR	R4	K60	R13
      0x8C34055A,  //  018E  GETMET	R13	R2	K90
      0x7C340200,  //  018F  CALL	R13	1
      0x8C341B5B,  //  0190  GETMET	R13	R13	K91
      0x883C093C,  //  0191  GETMBR	R15	R4	K60
      0x7C340400,  //  0192  CALL	R13	2
      0x8C341B5C,  //  0193  GETMET	R13	R13	K92
      0x7C340200,  //  0194  CALL	R13	1
      0x60380015,  //  0195  GETGBL	R14	G21
      0x7C380000,  //  0196  CALL	R14	0
      0x8C381D1C,  //  0197  GETMET	R14	R14	K28
      0x8840015D,  //  0198  GETMBR	R16	R0	K93
      0x7C380400,  //  0199  CALL	R14	2
      0x8C3C115E,  //  019A  GETMET	R15	R8	K94
      0x7C3C0200,  //  019B  CALL	R15	1
      0x003C1E09,  //  019C  ADD	R15	R15	R9
      0x8840094D,  //  019D  GETMBR	R16	R4	K77
      0x003C1E10,  //  019E  ADD	R15	R15	R16
      0x003C1E0D,  //  019F  ADD	R15	R15	R13
      0x8C40051E,  //  01A0  GETMET	R16	R2	K30
      0x7C400200,  //  01A1  CALL	R16	1
      0x8C40211F,  //  01A2  GETMET	R16	R16	K31
      0x88480920,  //  01A3  GETMBR	R18	R4	K32
      0x5C4C1E00,  //  01A4  MOVE	R19	R15
      0x5C501C00,  //  01A5  MOVE	R20	R14
      0x5456000F,  //  01A6  LDINT	R21	16
      0x7C400A00,  //  01A7  CALL	R16	5
      0x8C44193B,  //  01A8  GETMET	R17	R12	K59
      0x7C440200,  //  01A9  CALL	R17	1
      0x8C480523,  //  01AA  GETMET	R18	R2	K35
      0x5C502000,  //  01AB  MOVE	R20	R16
      0x60540015,  //  01AC  GETGBL	R21	G21
      0x7C540000,  //  01AD  CALL	R21	0
      0x8C542B1C,  //  01AE  GETMET	R21	R21	K28
      0x885C015F,  //  01AF  GETMBR	R23	R0	K95
      0x7C540400,  //  01B0  CALL	R21	2
      0x60580015,  //  01B1  GETGBL	R22	G21
      0x7C580000,  //  01B2  CALL	R22	0
      0x605C000C,  //  01B3  GETGBL	R23	G12
      0x5C602200,  //  01B4  MOVE	R24	R17
      0x7C5C0200,  //  01B5  CALL	R23	1
      0x5462000F,  //  01B6  LDINT	R24	16
      0x7C480C00,  //  01B7  CALL	R18	6
      0x8C4C2560,  //  01B8  GETMET	R19	R18	K96
      0x5C542200,  //  01B9  MOVE	R21	R17
      0x7C4C0400,  //  01BA  CALL	R19	2
      0x8C502525,  //  01BB  GETMET	R20	R18	K37
      0x7C500200,  //  01BC  CALL	R20	1
      0x004C2614,  //  01BD  ADD	R19	R19	R20
      0xB8521C00,  //  01BE  GETNGBL	R20	K14
      0x8C502961,  //  01BF  GETMET	R20	R20	K97
      0x7C500200,  //  01C0  CALL	R20	1
      0x9052C409,  //  01C1  SETMBR	R20	K98	R9
      0x8854092B,  //  01C2  GETMBR	R21	R4	K43
      0x9052C615,  //  01C3  SETMBR	R20	K99	R21
      0x8854094D,  //  01C4  GETMBR	R21	R4	K77
      0x9052C815,  //  01C5  SETMBR	R20	K100	R21
      0x9052CA13,  //  01C6  SETMBR	R20	K101	R19
      0x8C54293B,  //  01C7  GETMET	R21	R20	K59
      0x7C540200,  //  01C8  CALL	R21	1
      0x9012CC15,  //  01C9  SETMBR	R4	K102	R21
      0x8C58033D,  //  01CA  GETMET	R22	R1	K61
      0x54620030,  //  01CB  LDINT	R24	49
      0x50640200,  //  01CC  LDBOOL	R25	1	0
      0x7C580600,  //  01CD  CALL	R22	3
      0x8C5C2D3E,  //  01CE  GETMET	R23	R22	K62
      0x5C642A00,  //  01CF  MOVE	R25	R21
      0x7C5C0400,  //  01D0  CALL	R23	2
      0xB8620E00,  //  01D1  GETNGBL	R24	K7
      0x8C603108,  //  01D2  GETMET	R24	R24	K8
      0x8C68072D,  //  01D3  GETMET	R26	R3	K45
      0x58700067,  //  01D4  LDCONST	R28	K103
      0x8874092B,  //  01D5  GETMBR	R29	R4	K43
      0x88780968,  //  01D6  GETMBR	R30	R4	K104
      0x887C0969,  //  01D7  GETMBR	R31	R4	K105
      0x7C680A00,  //  01D8  CALL	R26	5
      0x7C600400,  //  01D9  CALL	R24	2
      0x8860013F,  //  01DA  GETMBR	R24	R0	K63
      0x8C603140,  //  01DB  GETMET	R24	R24	K64
      0x5C682C00,  //  01DC  MOVE	R26	R22
      0x7C600400,  //  01DD  CALL	R24	2
      0x50600200,  //  01DE  LDBOOL	R24	1	0
      0x80043000,  //  01DF  RET	1	R24
      0x50200200,  //  01E0  LDBOOL	R8	1	0
      0x80041000,  //  01E1  RET	1	R8
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
