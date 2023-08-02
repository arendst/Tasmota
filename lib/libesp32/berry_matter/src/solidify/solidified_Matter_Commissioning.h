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
    /* K2   */  be_nested_str_weak(loglevel),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20SEARCHING_X3A_X20destinationId_X3D),
    /* K5   */  be_nested_str_weak(tohex),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(sessions),
    /* K8   */  be_nested_str_weak(fabrics),
    /* K9   */  be_nested_str_weak(noc),
    /* K10  */  be_nested_str_weak(fabric_id),
    /* K11  */  be_nested_str_weak(device_id),
    /* K12  */  be_nested_str_weak(get_ca_pub),
    /* K13  */  be_nested_str_weak(get_ipk_group_key),
    /* K14  */  be_nested_str_weak(HMAC_SHA256),
    /* K15  */  be_nested_str_weak(update),
    /* K16  */  be_nested_str_weak(out),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20candidateDestinationId_X3D),
    /* K18  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(find_fabric_by_destination_id),
    &be_const_str_solidified,
    ( &(const binstruction[73]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x541A0003,  //  0003  LDINT	R6	4
      0x7C100400,  //  0004  CALL	R4	2
      0x78120006,  //  0005  JMPF	R4	#000D
      0xB8120200,  //  0006  GETNGBL	R4	K1
      0x8C100903,  //  0007  GETMET	R4	R4	K3
      0x8C180305,  //  0008  GETMET	R6	R1	K5
      0x7C180200,  //  0009  CALL	R6	1
      0x001A0806,  //  000A  ADD	R6	K4	R6
      0x541E0003,  //  000B  LDINT	R7	4
      0x7C100600,  //  000C  CALL	R4	3
      0x60100010,  //  000D  GETGBL	R4	G16
      0x88140106,  //  000E  GETMBR	R5	R0	K6
      0x88140B07,  //  000F  GETMBR	R5	R5	K7
      0x88140B08,  //  0010  GETMBR	R5	R5	K8
      0x7C100200,  //  0011  CALL	R4	1
      0xA8020030,  //  0012  EXBLK	0	#0044
      0x5C140800,  //  0013  MOVE	R5	R4
      0x7C140000,  //  0014  CALL	R5	0
      0x88180B09,  //  0015  GETMBR	R6	R5	K9
      0x4C1C0000,  //  0016  LDNIL	R7
      0x1C180C07,  //  0017  EQ	R6	R6	R7
      0x741A0007,  //  0018  JMPT	R6	#0021
      0x88180B0A,  //  0019  GETMBR	R6	R5	K10
      0x4C1C0000,  //  001A  LDNIL	R7
      0x1C180C07,  //  001B  EQ	R6	R6	R7
      0x741A0003,  //  001C  JMPT	R6	#0021
      0x88180B0B,  //  001D  GETMBR	R6	R5	K11
      0x4C1C0000,  //  001E  LDNIL	R7
      0x1C180C07,  //  001F  EQ	R6	R6	R7
      0x781A0000,  //  0020  JMPF	R6	#0022
      0x7001FFF0,  //  0021  JMP		#0013
      0x8C180B0C,  //  0022  GETMET	R6	R5	K12
      0x7C180200,  //  0023  CALL	R6	1
      0x00180406,  //  0024  ADD	R6	R2	R6
      0x881C0B0A,  //  0025  GETMBR	R7	R5	K10
      0x00180C07,  //  0026  ADD	R6	R6	R7
      0x881C0B0B,  //  0027  GETMBR	R7	R5	K11
      0x00180C07,  //  0028  ADD	R6	R6	R7
      0x8C1C0B0D,  //  0029  GETMET	R7	R5	K13
      0x7C1C0200,  //  002A  CALL	R7	1
      0x8C20070E,  //  002B  GETMET	R8	R3	K14
      0x5C280E00,  //  002C  MOVE	R10	R7
      0x7C200400,  //  002D  CALL	R8	2
      0x8C24110F,  //  002E  GETMET	R9	R8	K15
      0x5C2C0C00,  //  002F  MOVE	R11	R6
      0x7C240400,  //  0030  CALL	R9	2
      0x8C241110,  //  0031  GETMET	R9	R8	K16
      0x7C240200,  //  0032  CALL	R9	1
      0xB82A0200,  //  0033  GETNGBL	R10	K1
      0x8C281502,  //  0034  GETMET	R10	R10	K2
      0x54320003,  //  0035  LDINT	R12	4
      0x7C280400,  //  0036  CALL	R10	2
      0x782A0006,  //  0037  JMPF	R10	#003F
      0xB82A0200,  //  0038  GETNGBL	R10	K1
      0x8C281503,  //  0039  GETMET	R10	R10	K3
      0x8C301305,  //  003A  GETMET	R12	R9	K5
      0x7C300200,  //  003B  CALL	R12	1
      0x0032220C,  //  003C  ADD	R12	K17	R12
      0x54360003,  //  003D  LDINT	R13	4
      0x7C280600,  //  003E  CALL	R10	3
      0x1C281201,  //  003F  EQ	R10	R9	R1
      0x782A0001,  //  0040  JMPF	R10	#0043
      0xA8040001,  //  0041  EXBLK	1	1
      0x80040A00,  //  0042  RET	1	R5
      0x7001FFCE,  //  0043  JMP		#0013
      0x58100012,  //  0044  LDCONST	R4	K18
      0xAC100200,  //  0045  CATCH	R4	1	0
      0xB0080000,  //  0046  RAISE	2	R0	R0
      0x4C100000,  //  0047  LDNIL	R4
      0x80040800,  //  0048  RET	1	R4
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
    12,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20add_session_X20local_session_id_X3D_X25i_X20initiator_session_id_X3D_X25i),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(sessions),
    /* K5   */  be_nested_str_weak(create_session),
    /* K6   */  be_nested_str_weak(set_keys),
    }),
    be_str_weak(add_session),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xB81A0000,  //  0000  GETNGBL	R6	K0
      0x8C180D01,  //  0001  GETMET	R6	R6	K1
      0x60200018,  //  0002  GETGBL	R8	G24
      0x58240002,  //  0003  LDCONST	R9	K2
      0x5C280200,  //  0004  MOVE	R10	R1
      0x5C2C0400,  //  0005  MOVE	R11	R2
      0x7C200600,  //  0006  CALL	R8	3
      0x54260003,  //  0007  LDINT	R9	4
      0x7C180600,  //  0008  CALL	R6	3
      0x88180103,  //  0009  GETMBR	R6	R0	K3
      0x88180D04,  //  000A  GETMBR	R6	R6	K4
      0x8C180D05,  //  000B  GETMET	R6	R6	K5
      0x5C200200,  //  000C  MOVE	R8	R1
      0x5C240400,  //  000D  MOVE	R9	R2
      0x7C180600,  //  000E  CALL	R6	3
      0x8C1C0D06,  //  000F  GETMET	R7	R6	K6
      0x5C240600,  //  0010  MOVE	R9	R3
      0x5C280800,  //  0011  MOVE	R10	R4
      0x5C2C0A00,  //  0012  MOVE	R11	R5
      0x7C1C0800,  //  0013  CALL	R7	4
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_PBKDFParamRequest
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_PBKDFParamRequest,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[48]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20invalid_X20PBKDFParamRequest_X20message),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K11  */  be_const_int(3),
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
    /* K30  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K31  */  be_nested_str_weak(remote_ip),
    /* K32  */  be_nested_str_weak(remote_port),
    /* K33  */  be_nested_str_weak(PBKDFParamResponse),
    /* K34  */  be_nested_str_weak(initiatorRandom),
    /* K35  */  be_nested_str_weak(responderRandom),
    /* K36  */  be_nested_str_weak(random),
    /* K37  */  be_nested_str_weak(responderSessionId),
    /* K38  */  be_nested_str_weak(pbkdf_parameters_salt),
    /* K39  */  be_nested_str_weak(commissioning_salt),
    /* K40  */  be_nested_str_weak(pbkdf_parameters_iterations),
    /* K41  */  be_nested_str_weak(commissioning_iterations),
    /* K42  */  be_nested_str_weak(tlv2raw),
    /* K43  */  be_nested_str_weak(__Msg2),
    /* K44  */  be_nested_str_weak(build_response),
    /* K45  */  be_nested_str_weak(encode_frame),
    /* K46  */  be_nested_str_weak(responder),
    /* K47  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[118]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x88100302,  //  0002  GETMBR	R4	R1	K2
      0x5416001F,  //  0003  LDINT	R5	32
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
      0x581C000B,  //  0014  LDCONST	R7	K11
      0x7C100600,  //  0015  CALL	R4	3
      0x8C10010C,  //  0016  GETMET	R4	R0	K12
      0x5C180200,  //  0017  MOVE	R6	R1
      0x581C000D,  //  0018  LDCONST	R7	K13
      0x58200004,  //  0019  LDCONST	R8	K4
      0x58240009,  //  001A  LDCONST	R9	K9
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
      0x88140301,  //  0026  GETMBR	R5	R1	K1
      0x8C140B13,  //  0027  GETMET	R5	R5	K19
      0x7C140200,  //  0028  CALL	R5	1
      0x88140312,  //  0029  GETMBR	R5	R1	K18
      0x40140B15,  //  002A  CONNECT	R5	R5	K21
      0x88180311,  //  002B  GETMBR	R6	R1	K17
      0x94140C05,  //  002C  GETIDX	R5	R6	R5
      0x900E2805,  //  002D  SETMBR	R3	K20	R5
      0x88140916,  //  002E  GETMBR	R5	R4	K22
      0x20140B04,  //  002F  NE	R5	R5	K4
      0x78160012,  //  0030  JMPF	R5	#0044
      0xB8160C00,  //  0031  GETNGBL	R5	K6
      0x8C140B07,  //  0032  GETMET	R5	R5	K7
      0x581C0017,  //  0033  LDCONST	R7	K23
      0x58200009,  //  0034  LDCONST	R8	K9
      0x7C140600,  //  0035  CALL	R5	3
      0xB8160C00,  //  0036  GETNGBL	R5	K6
      0x8C140B07,  //  0037  GETMET	R5	R5	K7
      0x581C000A,  //  0038  LDCONST	R7	K10
      0x5820000B,  //  0039  LDCONST	R8	K11
      0x7C140600,  //  003A  CALL	R5	3
      0x8C14010C,  //  003B  GETMET	R5	R0	K12
      0x5C1C0200,  //  003C  MOVE	R7	R1
      0x5820000D,  //  003D  LDCONST	R8	K13
      0x58240004,  //  003E  LDCONST	R9	K4
      0x58280009,  //  003F  LDCONST	R10	K9
      0x502C0000,  //  0040  LDBOOL	R11	0	0
      0x7C140C00,  //  0041  CALL	R5	6
      0x50140000,  //  0042  LDBOOL	R5	0	0
      0x80040A00,  //  0043  RET	1	R5
      0x88140919,  //  0044  GETMBR	R5	R4	K25
      0x900E3005,  //  0045  SETMBR	R3	K24	R5
      0x8814011B,  //  0046  GETMBR	R5	R0	K27
      0x88140B1C,  //  0047  GETMBR	R5	R5	K28
      0x8C140B1D,  //  0048  GETMET	R5	R5	K29
      0x7C140200,  //  0049  CALL	R5	1
      0x900E3405,  //  004A  SETMBR	R3	K26	R5
      0xB8160C00,  //  004B  GETNGBL	R5	K6
      0x8C140B07,  //  004C  GETMET	R5	R5	K7
      0x601C0018,  //  004D  GETGBL	R7	G24
      0x5820001E,  //  004E  LDCONST	R8	K30
      0x8824071A,  //  004F  GETMBR	R9	R3	K26
      0x8828031F,  //  0050  GETMBR	R10	R1	K31
      0x882C0320,  //  0051  GETMBR	R11	R1	K32
      0x7C1C0800,  //  0052  CALL	R7	4
      0x5820000B,  //  0053  LDCONST	R8	K11
      0x7C140600,  //  0054  CALL	R5	3
      0xB8161C00,  //  0055  GETNGBL	R5	K14
      0x8C140B21,  //  0056  GETMET	R5	R5	K33
      0x7C140200,  //  0057  CALL	R5	1
      0x88180922,  //  0058  GETMBR	R6	R4	K34
      0x90164406,  //  0059  SETMBR	R5	K34	R6
      0x8C180524,  //  005A  GETMET	R6	R2	K36
      0x5422001F,  //  005B  LDINT	R8	32
      0x7C180400,  //  005C  CALL	R6	2
      0x90164606,  //  005D  SETMBR	R5	K35	R6
      0x8818071A,  //  005E  GETMBR	R6	R3	K26
      0x90164A06,  //  005F  SETMBR	R5	K37	R6
      0x8818011B,  //  0060  GETMBR	R6	R0	K27
      0x88180D27,  //  0061  GETMBR	R6	R6	K39
      0x90164C06,  //  0062  SETMBR	R5	K38	R6
      0x8818011B,  //  0063  GETMBR	R6	R0	K27
      0x88180D29,  //  0064  GETMBR	R6	R6	K41
      0x90165006,  //  0065  SETMBR	R5	K40	R6
      0x8C180B2A,  //  0066  GETMET	R6	R5	K42
      0x7C180200,  //  0067  CALL	R6	1
      0x900E5606,  //  0068  SETMBR	R3	K43	R6
      0x8C1C032C,  //  0069  GETMET	R7	R1	K44
      0x54260020,  //  006A  LDINT	R9	33
      0x50280200,  //  006B  LDBOOL	R10	1	0
      0x7C1C0600,  //  006C  CALL	R7	3
      0x8C200F2D,  //  006D  GETMET	R8	R7	K45
      0x5C280C00,  //  006E  MOVE	R10	R6
      0x7C200400,  //  006F  CALL	R8	2
      0x8824012E,  //  0070  GETMBR	R9	R0	K46
      0x8C24132F,  //  0071  GETMET	R9	R9	K47
      0x5C2C0E00,  //  0072  MOVE	R11	R7
      0x7C240400,  //  0073  CALL	R9	2
      0x50240200,  //  0074  LDBOOL	R9	1	0
      0x80041200,  //  0075  RET	1	R9
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
    21,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[52]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake1_X20message),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K11  */  be_nested_str_weak(send_status_report),
    /* K12  */  be_const_int(1),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(matter),
    /* K15  */  be_nested_str_weak(Pake1),
    /* K16  */  be_nested_str_weak(parse),
    /* K17  */  be_nested_str_weak(raw),
    /* K18  */  be_nested_str_weak(app_payload_idx),
    /* K19  */  be_nested_str_weak(pA),
    /* K20  */  be_nested_str_weak(SPAKE2P_Matter),
    /* K21  */  be_nested_str_weak(device),
    /* K22  */  be_nested_str_weak(commissioning_w0),
    /* K23  */  be_nested_str_weak(commissioning_L),
    /* K24  */  be_nested_str_weak(random),
    /* K25  */  be_nested_str_weak(compute_pB),
    /* K26  */  be_nested_str_weak(compute_ZV_verifier),
    /* K27  */  be_nested_str_weak(SHA256),
    /* K28  */  be_nested_str_weak(update),
    /* K29  */  be_nested_str_weak(fromstring),
    /* K30  */  be_nested_str_weak(Matter_Context_Prefix),
    /* K31  */  be_nested_str_weak(__Msg1),
    /* K32  */  be_nested_str_weak(__Msg2),
    /* K33  */  be_nested_str_weak(out),
    /* K34  */  be_nested_str_weak(set_context),
    /* K35  */  be_nested_str_weak(compute_TT_hash),
    /* K36  */  be_nested_str_weak(Pake2),
    /* K37  */  be_nested_str_weak(pB),
    /* K38  */  be_nested_str_weak(cB),
    /* K39  */  be_nested_str_weak(tlv2raw),
    /* K40  */  be_nested_str_weak(__spake_cA),
    /* K41  */  be_nested_str_weak(cA),
    /* K42  */  be_nested_str_weak(__spake_Ke),
    /* K43  */  be_nested_str_weak(Ke),
    /* K44  */  be_nested_str_weak(build_response),
    /* K45  */  be_nested_str_weak(encode_frame),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20New_X20Commissioning_X20_X28PASE_X20id_X3D_X25i_X29_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K47  */  be_nested_str_weak(__future_local_session_id),
    /* K48  */  be_nested_str_weak(_ip),
    /* K49  */  be_nested_str_weak(_port),
    /* K50  */  be_nested_str_weak(responder),
    /* K51  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
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
      0x8C180514,  //  0027  GETMET	R6	R2	K20
      0x88200115,  //  0028  GETMBR	R8	R0	K21
      0x88201116,  //  0029  GETMBR	R8	R8	K22
      0x4C240000,  //  002A  LDNIL	R9
      0x88280115,  //  002B  GETMBR	R10	R0	K21
      0x88281517,  //  002C  GETMBR	R10	R10	K23
      0x7C180800,  //  002D  CALL	R6	4
      0x8C1C0518,  //  002E  GETMET	R7	R2	K24
      0x5426001F,  //  002F  LDINT	R9	32
      0x7C1C0400,  //  0030  CALL	R7	2
      0x8C200D19,  //  0031  GETMET	R8	R6	K25
      0x5C280E00,  //  0032  MOVE	R10	R7
      0x7C200400,  //  0033  CALL	R8	2
      0x8C200D1A,  //  0034  GETMET	R8	R6	K26
      0x5C280A00,  //  0035  MOVE	R10	R5
      0x7C200400,  //  0036  CALL	R8	2
      0x8C20051B,  //  0037  GETMET	R8	R2	K27
      0x7C200200,  //  0038  CALL	R8	1
      0x8C24111C,  //  0039  GETMET	R9	R8	K28
      0x602C0015,  //  003A  GETGBL	R11	G21
      0x7C2C0000,  //  003B  CALL	R11	0
      0x8C2C171D,  //  003C  GETMET	R11	R11	K29
      0x8834011E,  //  003D  GETMBR	R13	R0	K30
      0x7C2C0400,  //  003E  CALL	R11	2
      0x7C240400,  //  003F  CALL	R9	2
      0x8C24111C,  //  0040  GETMET	R9	R8	K28
      0x882C071F,  //  0041  GETMBR	R11	R3	K31
      0x7C240400,  //  0042  CALL	R9	2
      0x8C24111C,  //  0043  GETMET	R9	R8	K28
      0x882C0720,  //  0044  GETMBR	R11	R3	K32
      0x7C240400,  //  0045  CALL	R9	2
      0x8C241121,  //  0046  GETMET	R9	R8	K33
      0x7C240200,  //  0047  CALL	R9	1
      0x901A2605,  //  0048  SETMBR	R6	K19	R5
      0x8C280D22,  //  0049  GETMET	R10	R6	K34
      0x5C301200,  //  004A  MOVE	R12	R9
      0x7C280400,  //  004B  CALL	R10	2
      0x8C280D23,  //  004C  GETMET	R10	R6	K35
      0x50300200,  //  004D  LDBOOL	R12	1	0
      0x7C280400,  //  004E  CALL	R10	2
      0xB82A1C00,  //  004F  GETNGBL	R10	K14
      0x8C281524,  //  0050  GETMET	R10	R10	K36
      0x7C280200,  //  0051  CALL	R10	1
      0x882C0D25,  //  0052  GETMBR	R11	R6	K37
      0x902A4A0B,  //  0053  SETMBR	R10	K37	R11
      0x882C0D26,  //  0054  GETMBR	R11	R6	K38
      0x902A4C0B,  //  0055  SETMBR	R10	K38	R11
      0x8C2C1527,  //  0056  GETMET	R11	R10	K39
      0x7C2C0200,  //  0057  CALL	R11	1
      0x88300D29,  //  0058  GETMBR	R12	R6	K41
      0x900E500C,  //  0059  SETMBR	R3	K40	R12
      0x88300D2B,  //  005A  GETMBR	R12	R6	K43
      0x900E540C,  //  005B  SETMBR	R3	K42	R12
      0x8C30032C,  //  005C  GETMET	R12	R1	K44
      0x543A0022,  //  005D  LDINT	R14	35
      0x503C0200,  //  005E  LDBOOL	R15	1	0
      0x7C300600,  //  005F  CALL	R12	3
      0x8C34192D,  //  0060  GETMET	R13	R12	K45
      0x5C3C1600,  //  0061  MOVE	R15	R11
      0x7C340400,  //  0062  CALL	R13	2
      0xB83A0C00,  //  0063  GETNGBL	R14	K6
      0x8C381D07,  //  0064  GETMET	R14	R14	K7
      0x60400018,  //  0065  GETGBL	R16	G24
      0x5844002E,  //  0066  LDCONST	R17	K46
      0x8848072F,  //  0067  GETMBR	R18	R3	K47
      0x884C0730,  //  0068  GETMBR	R19	R3	K48
      0x88500731,  //  0069  GETMBR	R20	R3	K49
      0x7C400800,  //  006A  CALL	R16	4
      0x5844000D,  //  006B  LDCONST	R17	K13
      0x7C380600,  //  006C  CALL	R14	3
      0x88380132,  //  006D  GETMBR	R14	R0	K50
      0x8C381D33,  //  006E  GETMET	R14	R14	K51
      0x5C401800,  //  006F  MOVE	R16	R12
      0x7C380400,  //  0070  CALL	R14	2
      0x50380200,  //  0071  LDBOOL	R14	1	0
      0x80041C00,  //  0072  RET	1	R14
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
    /* K22  */  be_nested_str_weak(rtc_utc),
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
    ( &(const binstruction[105]) {  /* code */
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
      0x8C1C0517,  //  0040  GETMET	R7	R2	K23
      0x7C1C0200,  //  0041  CALL	R7	1
      0x8C1C0F18,  //  0042  GETMET	R7	R7	K24
      0x88240719,  //  0043  GETMBR	R9	R3	K25
      0x60280015,  //  0044  GETGBL	R10	G21
      0x7C280000,  //  0045  CALL	R10	0
      0x602C0015,  //  0046  GETGBL	R11	G21
      0x7C2C0000,  //  0047  CALL	R11	0
      0x8C2C171A,  //  0048  GETMET	R11	R11	K26
      0x8834011B,  //  0049  GETMBR	R13	R0	K27
      0x7C2C0400,  //  004A  CALL	R11	2
      0x5432002F,  //  004B  LDINT	R12	48
      0x7C1C0A00,  //  004C  CALL	R7	5
      0x5422000E,  //  004D  LDINT	R8	15
      0x40220808,  //  004E  CONNECT	R8	K4	R8
      0x94200E08,  //  004F  GETIDX	R8	R7	R8
      0x5426000F,  //  0050  LDINT	R9	16
      0x542A001E,  //  0051  LDINT	R10	31
      0x4024120A,  //  0052  CONNECT	R9	R9	R10
      0x94240E09,  //  0053  GETIDX	R9	R7	R9
      0x542A001F,  //  0054  LDINT	R10	32
      0x542E002E,  //  0055  LDINT	R11	47
      0x4028140B,  //  0056  CONNECT	R10	R10	R11
      0x94280E0A,  //  0057  GETIDX	R10	R7	R10
      0x8C2C010B,  //  0058  GETMET	R11	R0	K11
      0x5C340200,  //  0059  MOVE	R13	R1
      0x58380004,  //  005A  LDCONST	R14	K4
      0x583C0004,  //  005B  LDCONST	R15	K4
      0x58400004,  //  005C  LDCONST	R16	K4
      0x50440000,  //  005D  LDBOOL	R17	0	0
      0x7C2C0C00,  //  005E  CALL	R11	6
      0x8C2C011C,  //  005F  GETMET	R11	R0	K28
      0x8834071D,  //  0060  GETMBR	R13	R3	K29
      0x8838071E,  //  0061  GETMBR	R14	R3	K30
      0x5C3C1000,  //  0062  MOVE	R15	R8
      0x5C401200,  //  0063  MOVE	R16	R9
      0x5C441400,  //  0064  MOVE	R17	R10
      0x5C480C00,  //  0065  MOVE	R18	R6
      0x7C2C0E00,  //  0066  CALL	R11	7
      0x502C0200,  //  0067  LDBOOL	R11	1	0
      0x80041600,  //  0068  RET	1	R11
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
    ( &(const bvalue[66]) {     /* constants */
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
    /* K57  */  be_nested_str_weak(rtc_utc),
    /* K58  */  be_nested_str_weak(close),
    /* K59  */  be_nested_str_weak(set_keys),
    /* K60  */  be_nested_str_weak(_breadcrumb),
    /* K61  */  be_nested_str_weak(counter_snd_next),
    /* K62  */  be_nested_str_weak(set_persist),
    /* K63  */  be_nested_str_weak(set_no_expiration),
    /* K64  */  be_nested_str_weak(persist_to_fabric),
    /* K65  */  be_nested_str_weak(save),
    }),
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[286]) {  /* code */
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
      0x8C740109,  //  0101  GETMET	R29	R0	K9
      0x5C7C0200,  //  0102  MOVE	R31	R1
      0x58800003,  //  0103  LDCONST	R32	K3
      0x58840003,  //  0104  LDCONST	R33	K3
      0x58880003,  //  0105  LDCONST	R34	K3
      0x508C0200,  //  0106  LDBOOL	R35	1	0
      0x7C740C00,  //  0107  CALL	R29	6
      0x8C74073A,  //  0108  GETMET	R29	R3	K58
      0x7C740200,  //  0109  CALL	R29	1
      0x8C74073B,  //  010A  GETMET	R29	R3	K59
      0x5C7C3200,  //  010B  MOVE	R31	R25
      0x5C803400,  //  010C  MOVE	R32	R26
      0x5C843600,  //  010D  MOVE	R33	R27
      0x5C883800,  //  010E  MOVE	R34	R28
      0x7C740A00,  //  010F  CALL	R29	5
      0x900E7903,  //  0110  SETMBR	R3	K60	K3
      0x8C74073D,  //  0111  GETMET	R29	R3	K61
      0x7C740200,  //  0112  CALL	R29	1
      0x8C74073E,  //  0113  GETMET	R29	R3	K62
      0x507C0200,  //  0114  LDBOOL	R31	1	0
      0x7C740400,  //  0115  CALL	R29	2
      0x8C74073F,  //  0116  GETMET	R29	R3	K63
      0x7C740200,  //  0117  CALL	R29	1
      0x8C740740,  //  0118  GETMET	R29	R3	K64
      0x7C740200,  //  0119  CALL	R29	1
      0x8C740741,  //  011A  GETMET	R29	R3	K65
      0x7C740200,  //  011B  CALL	R29	1
      0x50740200,  //  011C  LDBOOL	R29	1	0
      0x80043A00,  //  011D  RET	1	R29
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming
********************************************************************/
be_local_closure(Matter_Commisioning_Context_process_incoming,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
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
    /* K13  */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28secure_X20channel_X29_X20_X2502X),
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[90]) {  /* code */
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
      0x7002003F,  //  0017  JMP		#0058
      0x88080302,  //  0018  GETMBR	R2	R1	K2
      0x540E001F,  //  0019  LDINT	R3	32
      0x1C080403,  //  001A  EQ	R2	R2	R3
      0x780A0004,  //  001B  JMPF	R2	#0021
      0x8C080107,  //  001C  GETMET	R2	R0	K7
      0x5C100200,  //  001D  MOVE	R4	R1
      0x7C080400,  //  001E  CALL	R2	2
      0x80040400,  //  001F  RET	1	R2
      0x70020036,  //  0020  JMP		#0058
      0x88080302,  //  0021  GETMBR	R2	R1	K2
      0x540E0021,  //  0022  LDINT	R3	34
      0x1C080403,  //  0023  EQ	R2	R2	R3
      0x780A0004,  //  0024  JMPF	R2	#002A
      0x8C080108,  //  0025  GETMET	R2	R0	K8
      0x5C100200,  //  0026  MOVE	R4	R1
      0x7C080400,  //  0027  CALL	R2	2
      0x80040400,  //  0028  RET	1	R2
      0x7002002D,  //  0029  JMP		#0058
      0x88080302,  //  002A  GETMBR	R2	R1	K2
      0x540E0023,  //  002B  LDINT	R3	36
      0x1C080403,  //  002C  EQ	R2	R2	R3
      0x780A0004,  //  002D  JMPF	R2	#0033
      0x8C080109,  //  002E  GETMET	R2	R0	K9
      0x5C100200,  //  002F  MOVE	R4	R1
      0x7C080400,  //  0030  CALL	R2	2
      0x80040400,  //  0031  RET	1	R2
      0x70020024,  //  0032  JMP		#0058
      0x88080302,  //  0033  GETMBR	R2	R1	K2
      0x540E002F,  //  0034  LDINT	R3	48
      0x1C080403,  //  0035  EQ	R2	R2	R3
      0x780A0004,  //  0036  JMPF	R2	#003C
      0x8C08010A,  //  0037  GETMET	R2	R0	K10
      0x5C100200,  //  0038  MOVE	R4	R1
      0x7C080400,  //  0039  CALL	R2	2
      0x80040400,  //  003A  RET	1	R2
      0x7002001B,  //  003B  JMP		#0058
      0x88080302,  //  003C  GETMBR	R2	R1	K2
      0x540E0031,  //  003D  LDINT	R3	50
      0x1C080403,  //  003E  EQ	R2	R2	R3
      0x780A0004,  //  003F  JMPF	R2	#0045
      0x8C08010B,  //  0040  GETMET	R2	R0	K11
      0x5C100200,  //  0041  MOVE	R4	R1
      0x7C080400,  //  0042  CALL	R2	2
      0x80040400,  //  0043  RET	1	R2
      0x70020012,  //  0044  JMP		#0058
      0x88080302,  //  0045  GETMBR	R2	R1	K2
      0x540E003F,  //  0046  LDINT	R3	64
      0x1C080403,  //  0047  EQ	R2	R2	R3
      0x780A0004,  //  0048  JMPF	R2	#004E
      0x8C08010C,  //  0049  GETMET	R2	R0	K12
      0x5C100200,  //  004A  MOVE	R4	R1
      0x7C080400,  //  004B  CALL	R2	2
      0x80040400,  //  004C  RET	1	R2
      0x70020009,  //  004D  JMP		#0058
      0xB80A0600,  //  004E  GETNGBL	R2	K3
      0x8C080504,  //  004F  GETMET	R2	R2	K4
      0x60100018,  //  0050  GETGBL	R4	G24
      0x5814000D,  //  0051  LDCONST	R5	K13
      0x88180302,  //  0052  GETMBR	R6	R1	K2
      0x7C100400,  //  0053  CALL	R4	2
      0x58140006,  //  0054  LDCONST	R5	K6
      0x7C080600,  //  0055  CALL	R2	3
      0x50080000,  //  0056  LDBOOL	R2	0	0
      0x80040400,  //  0057  RET	1	R2
      0x50080000,  //  0058  LDBOOL	R2	0	0
      0x80040400,  //  0059  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Sigma1
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Sigma1,   /* name */
  be_nested_proto(
    36,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[103]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(send_status_report),
    /* K11  */  be_const_int(1),
    /* K12  */  be_const_int(2),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(Sigma1),
    /* K15  */  be_nested_str_weak(parse),
    /* K16  */  be_nested_str_weak(raw),
    /* K17  */  be_nested_str_weak(app_payload_idx),
    /* K18  */  be_nested_str_weak(__initiator_pub),
    /* K19  */  be_nested_str_weak(initiatorEphPubKey),
    /* K20  */  be_nested_str_weak(resumptionID),
    /* K21  */  be_nested_str_weak(initiatorResumeMIC),
    /* K22  */  be_nested_str_weak(device),
    /* K23  */  be_nested_str_weak(sessions),
    /* K24  */  be_nested_str_weak(find_session_by_resumption_id),
    /* K25  */  be_nested_str_weak(_fabric),
    /* K26  */  be_nested_str_weak(initiatorRandom),
    /* K27  */  be_nested_str_weak(fromstring),
    /* K28  */  be_nested_str_weak(Sigma1_Resume),
    /* K29  */  be_nested_str_weak(HKDF_SHA256),
    /* K30  */  be_nested_str_weak(derive),
    /* K31  */  be_nested_str_weak(shared_secret),
    /* K32  */  be_nested_str_weak(NCASE_SigmaS1),
    /* K33  */  be_const_int(2147483647),
    /* K34  */  be_nested_str_weak(AES_CCM),
    /* K35  */  be_nested_str_weak(decrypt),
    /* K36  */  be_nested_str_weak(tag),
    /* K37  */  be_nested_str_weak(_source_node_id),
    /* K38  */  be_nested_str_weak(source_node_id),
    /* K39  */  be_nested_str_weak(set_mode_CASE),
    /* K40  */  be_nested_str_weak(__future_initiator_session_id),
    /* K41  */  be_nested_str_weak(initiator_session_id),
    /* K42  */  be_nested_str_weak(__future_local_session_id),
    /* K43  */  be_nested_str_weak(gen_local_session_id),
    /* K44  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K45  */  be_nested_str_weak(remote_ip),
    /* K46  */  be_nested_str_weak(remote_port),
    /* K47  */  be_nested_str_weak(resumption_id),
    /* K48  */  be_nested_str_weak(random),
    /* K49  */  be_nested_str_weak(Sigma2_Resume),
    /* K50  */  be_nested_str_weak(NCASE_SigmaS2),
    /* K51  */  be_nested_str_weak(Sigma2Resume),
    /* K52  */  be_nested_str_weak(responderSessionID),
    /* K53  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K54  */  be_nested_str_weak(SessionResumptionKeys),
    /* K55  */  be_nested_str_weak(rtc_utc),
    /* K56  */  be_nested_str_weak(tlv2raw),
    /* K57  */  be_nested_str_weak(__Msg1),
    /* K58  */  be_nested_str_weak(build_response),
    /* K59  */  be_nested_str_weak(encode_frame),
    /* K60  */  be_nested_str_weak(responder),
    /* K61  */  be_nested_str_weak(send_response_frame),
    /* K62  */  be_nested_str_weak(close),
    /* K63  */  be_nested_str_weak(set_keys),
    /* K64  */  be_nested_str_weak(_breadcrumb),
    /* K65  */  be_nested_str_weak(counter_snd_next),
    /* K66  */  be_nested_str_weak(set_persist),
    /* K67  */  be_nested_str_weak(set_no_expiration),
    /* K68  */  be_nested_str_weak(persist_to_fabric),
    /* K69  */  be_nested_str_weak(save),
    /* K70  */  be_nested_str_weak(find_fabric_by_destination_id),
    /* K71  */  be_nested_str_weak(destinationId),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K73  */  be_nested_str_weak(__responder_priv),
    /* K74  */  be_nested_str_weak(__responder_pub),
    /* K75  */  be_nested_str_weak(EC_P256),
    /* K76  */  be_nested_str_weak(public_key),
    /* K77  */  be_nested_str_weak(shared_key),
    /* K78  */  be_nested_str_weak(TLV),
    /* K79  */  be_nested_str_weak(Matter_TLV_struct),
    /* K80  */  be_nested_str_weak(add_TLV),
    /* K81  */  be_nested_str_weak(B2),
    /* K82  */  be_nested_str_weak(get_noc),
    /* K83  */  be_nested_str_weak(get_icac),
    /* K84  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K85  */  be_nested_str_weak(get_pk),
    /* K86  */  be_nested_str_weak(Msg1),
    /* K87  */  be_nested_str_weak(SHA256),
    /* K88  */  be_nested_str_weak(update),
    /* K89  */  be_nested_str_weak(out),
    /* K90  */  be_nested_str_weak(S2K_Info),
    /* K91  */  be_nested_str_weak(get_ipk_group_key),
    /* K92  */  be_nested_str_weak(TBEData2_Nonce),
    /* K93  */  be_nested_str_weak(encrypt),
    /* K94  */  be_nested_str_weak(Sigma2),
    /* K95  */  be_nested_str_weak(responderRandom),
    /* K96  */  be_nested_str_weak(responderSessionId),
    /* K97  */  be_nested_str_weak(responderEphPubKey),
    /* K98  */  be_nested_str_weak(encrypted2),
    /* K99  */  be_nested_str_weak(__Msg2),
    /* K100 */  be_nested_str_weak(MTR_X3A_X20New_X20Connection_X20_X28CASE_X20id_X3D_X25i_X29_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K101 */  be_nested_str_weak(_ip),
    /* K102 */  be_nested_str_weak(_port),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[481]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x88100302,  //  0002  GETMBR	R4	R1	K2
      0x5416002F,  //  0003  LDINT	R5	48
      0x20100805,  //  0004  NE	R4	R4	R5
      0x74120005,  //  0005  JMPT	R4	#000C
      0x88100303,  //  0006  GETMBR	R4	R1	K3
      0x20100904,  //  0007  NE	R4	R4	K4
      0x74120002,  //  0008  JMPT	R4	#000C
      0x88100305,  //  0009  GETMBR	R4	R1	K5
      0x20100904,  //  000A  NE	R4	R4	K4
      0x7812000D,  //  000B  JMPF	R4	#001A
      0xB8120C00,  //  000C  GETNGBL	R4	K6
      0x8C100907,  //  000D  GETMET	R4	R4	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x581C0009,  //  000F  LDCONST	R7	K9
      0x7C100600,  //  0010  CALL	R4	3
      0x8C10010A,  //  0011  GETMET	R4	R0	K10
      0x5C180200,  //  0012  MOVE	R6	R1
      0x581C000B,  //  0013  LDCONST	R7	K11
      0x58200004,  //  0014  LDCONST	R8	K4
      0x5824000C,  //  0015  LDCONST	R9	K12
      0x50280000,  //  0016  LDBOOL	R10	0	0
      0x7C100C00,  //  0017  CALL	R4	6
      0x50100000,  //  0018  LDBOOL	R4	0	0
      0x80040800,  //  0019  RET	1	R4
      0xB8121A00,  //  001A  GETNGBL	R4	K13
      0x8C10090E,  //  001B  GETMET	R4	R4	K14
      0x7C100200,  //  001C  CALL	R4	1
      0x8C10090F,  //  001D  GETMET	R4	R4	K15
      0x88180310,  //  001E  GETMBR	R6	R1	K16
      0x881C0311,  //  001F  GETMBR	R7	R1	K17
      0x7C100600,  //  0020  CALL	R4	3
      0x88140913,  //  0021  GETMBR	R5	R4	K19
      0x900E2405,  //  0022  SETMBR	R3	K18	R5
      0x88140914,  //  0023  GETMBR	R5	R4	K20
      0x4C180000,  //  0024  LDNIL	R6
      0x20140A06,  //  0025  NE	R5	R5	R6
      0x78160003,  //  0026  JMPF	R5	#002B
      0x88140915,  //  0027  GETMBR	R5	R4	K21
      0x4C180000,  //  0028  LDNIL	R6
      0x20140A06,  //  0029  NE	R5	R5	R6
      0x74160000,  //  002A  JMPT	R5	#002C
      0x50140001,  //  002B  LDBOOL	R5	0	1
      0x50140200,  //  002C  LDBOOL	R5	1	0
      0x50140000,  //  002D  LDBOOL	R5	0	0
      0x4C180000,  //  002E  LDNIL	R6
      0x7816000D,  //  002F  JMPF	R5	#003E
      0x881C0116,  //  0030  GETMBR	R7	R0	K22
      0x881C0F17,  //  0031  GETMBR	R7	R7	K23
      0x8C1C0F18,  //  0032  GETMET	R7	R7	K24
      0x88240914,  //  0033  GETMBR	R9	R4	K20
      0x7C1C0400,  //  0034  CALL	R7	2
      0x5C180E00,  //  0035  MOVE	R6	R7
      0x4C1C0000,  //  0036  LDNIL	R7
      0x1C1C0C07,  //  0037  EQ	R7	R6	R7
      0x741E0003,  //  0038  JMPT	R7	#003D
      0x881C0D19,  //  0039  GETMBR	R7	R6	K25
      0x4C200000,  //  003A  LDNIL	R8
      0x1C1C0E08,  //  003B  EQ	R7	R7	R8
      0x781E0000,  //  003C  JMPF	R7	#003E
      0x50140000,  //  003D  LDBOOL	R5	0	0
      0x781600B5,  //  003E  JMPF	R5	#00F5
      0x881C091A,  //  003F  GETMBR	R7	R4	K26
      0x88200914,  //  0040  GETMBR	R8	R4	K20
      0x001C0E08,  //  0041  ADD	R7	R7	R8
      0x60200015,  //  0042  GETGBL	R8	G21
      0x7C200000,  //  0043  CALL	R8	0
      0x8C20111B,  //  0044  GETMET	R8	R8	K27
      0x5828001C,  //  0045  LDCONST	R10	K28
      0x7C200400,  //  0046  CALL	R8	2
      0x8C24051D,  //  0047  GETMET	R9	R2	K29
      0x7C240200,  //  0048  CALL	R9	1
      0x8C24131E,  //  0049  GETMET	R9	R9	K30
      0x882C0D1F,  //  004A  GETMBR	R11	R6	K31
      0x5C300E00,  //  004B  MOVE	R12	R7
      0x5C341000,  //  004C  MOVE	R13	R8
      0x543A000F,  //  004D  LDINT	R14	16
      0x7C240A00,  //  004E  CALL	R9	5
      0x60280015,  //  004F  GETGBL	R10	G21
      0x7C280000,  //  0050  CALL	R10	0
      0x8C28151B,  //  0051  GETMET	R10	R10	K27
      0x58300020,  //  0052  LDCONST	R12	K32
      0x7C280400,  //  0053  CALL	R10	2
      0x542DFFEE,  //  0054  LDINT	R11	-17
      0x402E080B,  //  0055  CONNECT	R11	K4	R11
      0x88300915,  //  0056  GETMBR	R12	R4	K21
      0x942C180B,  //  0057  GETIDX	R11	R12	R11
      0x5435FFEF,  //  0058  LDINT	R13	-16
      0x40341B21,  //  0059  CONNECT	R13	R13	K33
      0x88380915,  //  005A  GETMBR	R14	R4	K21
      0x94301C0D,  //  005B  GETIDX	R12	R14	R13
      0x8C3C0522,  //  005C  GETMET	R15	R2	K34
      0x5C441200,  //  005D  MOVE	R17	R9
      0x5C481400,  //  005E  MOVE	R18	R10
      0x604C0015,  //  005F  GETGBL	R19	G21
      0x7C4C0000,  //  0060  CALL	R19	0
      0x6050000C,  //  0061  GETGBL	R20	G12
      0x5C541600,  //  0062  MOVE	R21	R11
      0x7C500200,  //  0063  CALL	R20	1
      0x5456000F,  //  0064  LDINT	R21	16
      0x7C3C0C00,  //  0065  CALL	R15	6
      0x5C341E00,  //  0066  MOVE	R13	R15
      0x8C3C1B23,  //  0067  GETMET	R15	R13	K35
      0x5C441600,  //  0068  MOVE	R17	R11
      0x7C3C0400,  //  0069  CALL	R15	2
      0x5C381E00,  //  006A  MOVE	R14	R15
      0x8C3C1B24,  //  006B  GETMET	R15	R13	K36
      0x7C3C0200,  //  006C  CALL	R15	1
      0x1C40180F,  //  006D  EQ	R16	R12	R15
      0x78420084,  //  006E  JMPF	R16	#00F4
      0x88400D19,  //  006F  GETMBR	R16	R6	K25
      0x900E3210,  //  0070  SETMBR	R3	K25	R16
      0x88400326,  //  0071  GETMBR	R16	R1	K38
      0x900E4A10,  //  0072  SETMBR	R3	K37	R16
      0x8C400727,  //  0073  GETMET	R16	R3	K39
      0x7C400200,  //  0074  CALL	R16	1
      0x88400929,  //  0075  GETMBR	R16	R4	K41
      0x900E5010,  //  0076  SETMBR	R3	K40	R16
      0x88400116,  //  0077  GETMBR	R16	R0	K22
      0x88402117,  //  0078  GETMBR	R16	R16	K23
      0x8C40212B,  //  0079  GETMET	R16	R16	K43
      0x7C400200,  //  007A  CALL	R16	1
      0x900E5410,  //  007B  SETMBR	R3	K42	R16
      0xB8420C00,  //  007C  GETNGBL	R16	K6
      0x8C402107,  //  007D  GETMET	R16	R16	K7
      0x60480018,  //  007E  GETGBL	R18	G24
      0x584C002C,  //  007F  LDCONST	R19	K44
      0x8850072A,  //  0080  GETMBR	R20	R3	K42
      0x8854032D,  //  0081  GETMBR	R21	R1	K45
      0x8858032E,  //  0082  GETMBR	R22	R1	K46
      0x7C480800,  //  0083  CALL	R18	4
      0x584C0009,  //  0084  LDCONST	R19	K9
      0x7C400600,  //  0085  CALL	R16	3
      0x88400D1F,  //  0086  GETMBR	R16	R6	K31
      0x900E3E10,  //  0087  SETMBR	R3	K31	R16
      0x8C400530,  //  0088  GETMET	R16	R2	K48
      0x544A000F,  //  0089  LDINT	R18	16
      0x7C400400,  //  008A  CALL	R16	2
      0x900E5E10,  //  008B  SETMBR	R3	K47	R16
      0x60400015,  //  008C  GETGBL	R16	G21
      0x7C400000,  //  008D  CALL	R16	0
      0x8C40211B,  //  008E  GETMET	R16	R16	K27
      0x58480031,  //  008F  LDCONST	R18	K49
      0x7C400400,  //  0090  CALL	R16	2
      0x8844091A,  //  0091  GETMBR	R17	R4	K26
      0x8848072F,  //  0092  GETMBR	R18	R3	K47
      0x00442212,  //  0093  ADD	R17	R17	R18
      0x8C48051D,  //  0094  GETMET	R18	R2	K29
      0x7C480200,  //  0095  CALL	R18	1
      0x8C48251E,  //  0096  GETMET	R18	R18	K30
      0x8850071F,  //  0097  GETMBR	R20	R3	K31
      0x5C542200,  //  0098  MOVE	R21	R17
      0x5C582000,  //  0099  MOVE	R22	R16
      0x545E000F,  //  009A  LDINT	R23	16
      0x7C480A00,  //  009B  CALL	R18	5
      0x8C4C0522,  //  009C  GETMET	R19	R2	K34
      0x5C542400,  //  009D  MOVE	R21	R18
      0x60580015,  //  009E  GETGBL	R22	G21
      0x7C580000,  //  009F  CALL	R22	0
      0x8C582D1B,  //  00A0  GETMET	R22	R22	K27
      0x58600032,  //  00A1  LDCONST	R24	K50
      0x7C580400,  //  00A2  CALL	R22	2
      0x605C0015,  //  00A3  GETGBL	R23	G21
      0x7C5C0000,  //  00A4  CALL	R23	0
      0x58600004,  //  00A5  LDCONST	R24	K4
      0x5466000F,  //  00A6  LDINT	R25	16
      0x7C4C0C00,  //  00A7  CALL	R19	6
      0x8C502724,  //  00A8  GETMET	R20	R19	K36
      0x7C500200,  //  00A9  CALL	R20	1
      0xB8561A00,  //  00AA  GETNGBL	R21	K13
      0x8C542B33,  //  00AB  GETMET	R21	R21	K51
      0x7C540200,  //  00AC  CALL	R21	1
      0x8858072F,  //  00AD  GETMBR	R22	R3	K47
      0x90562816,  //  00AE  SETMBR	R21	K20	R22
      0x8858072A,  //  00AF  GETMBR	R22	R3	K42
      0x90566816,  //  00B0  SETMBR	R21	K52	R22
      0x90566A14,  //  00B1  SETMBR	R21	K53	R20
      0x8C58051D,  //  00B2  GETMET	R22	R2	K29
      0x7C580200,  //  00B3  CALL	R22	1
      0x8C582D1E,  //  00B4  GETMET	R22	R22	K30
      0x8860071F,  //  00B5  GETMBR	R24	R3	K31
      0x8864091A,  //  00B6  GETMBR	R25	R4	K26
      0x8868072F,  //  00B7  GETMBR	R26	R3	K47
      0x0064321A,  //  00B8  ADD	R25	R25	R26
      0x60680015,  //  00B9  GETGBL	R26	G21
      0x7C680000,  //  00BA  CALL	R26	0
      0x8C68351B,  //  00BB  GETMET	R26	R26	K27
      0x58700036,  //  00BC  LDCONST	R28	K54
      0x7C680400,  //  00BD  CALL	R26	2
      0x546E002F,  //  00BE  LDINT	R27	48
      0x7C580A00,  //  00BF  CALL	R22	5
      0x545E000E,  //  00C0  LDINT	R23	15
      0x405E0817,  //  00C1  CONNECT	R23	K4	R23
      0x945C2C17,  //  00C2  GETIDX	R23	R22	R23
      0x5462000F,  //  00C3  LDINT	R24	16
      0x5466001E,  //  00C4  LDINT	R25	31
      0x40603019,  //  00C5  CONNECT	R24	R24	R25
      0x94602C18,  //  00C6  GETIDX	R24	R22	R24
      0x5466001F,  //  00C7  LDINT	R25	32
      0x546A002E,  //  00C8  LDINT	R26	47
      0x4064321A,  //  00C9  CONNECT	R25	R25	R26
      0x94642C19,  //  00CA  GETIDX	R25	R22	R25
      0xB86A0C00,  //  00CB  GETNGBL	R26	K6
      0x8C683537,  //  00CC  GETMET	R26	R26	K55
      0x7C680200,  //  00CD  CALL	R26	1
      0x8C6C2B38,  //  00CE  GETMET	R27	R21	K56
      0x7C6C0200,  //  00CF  CALL	R27	1
      0x4C700000,  //  00D0  LDNIL	R28
      0x900E721C,  //  00D1  SETMBR	R3	K57	R28
      0x8C70033A,  //  00D2  GETMET	R28	R1	K58
      0x547A0032,  //  00D3  LDINT	R30	51
      0x507C0200,  //  00D4  LDBOOL	R31	1	0
      0x7C700600,  //  00D5  CALL	R28	3
      0x8C74393B,  //  00D6  GETMET	R29	R28	K59
      0x5C7C3600,  //  00D7  MOVE	R31	R27
      0x7C740400,  //  00D8  CALL	R29	2
      0x8878013C,  //  00D9  GETMBR	R30	R0	K60
      0x8C783D3D,  //  00DA  GETMET	R30	R30	K61
      0x5C803800,  //  00DB  MOVE	R32	R28
      0x7C780400,  //  00DC  CALL	R30	2
      0x8C78073E,  //  00DD  GETMET	R30	R3	K62
      0x7C780200,  //  00DE  CALL	R30	1
      0x8C78073F,  //  00DF  GETMET	R30	R3	K63
      0x5C802E00,  //  00E0  MOVE	R32	R23
      0x5C843000,  //  00E1  MOVE	R33	R24
      0x5C883200,  //  00E2  MOVE	R34	R25
      0x5C8C3400,  //  00E3  MOVE	R35	R26
      0x7C780A00,  //  00E4  CALL	R30	5
      0x900E8104,  //  00E5  SETMBR	R3	K64	K4
      0x8C780741,  //  00E6  GETMET	R30	R3	K65
      0x7C780200,  //  00E7  CALL	R30	1
      0x8C780742,  //  00E8  GETMET	R30	R3	K66
      0x50800200,  //  00E9  LDBOOL	R32	1	0
      0x7C780400,  //  00EA  CALL	R30	2
      0x8C780743,  //  00EB  GETMET	R30	R3	K67
      0x7C780200,  //  00EC  CALL	R30	1
      0x8C780744,  //  00ED  GETMET	R30	R3	K68
      0x7C780200,  //  00EE  CALL	R30	1
      0x8C780745,  //  00EF  GETMET	R30	R3	K69
      0x7C780200,  //  00F0  CALL	R30	1
      0x50780200,  //  00F1  LDBOOL	R30	1	0
      0x80043C00,  //  00F2  RET	1	R30
      0x70020000,  //  00F3  JMP		#00F5
      0x50140000,  //  00F4  LDBOOL	R5	0	0
      0x5C1C0A00,  //  00F5  MOVE	R7	R5
      0x741E00E7,  //  00F6  JMPT	R7	#01DF
      0x8C1C0146,  //  00F7  GETMET	R7	R0	K70
      0x88240947,  //  00F8  GETMBR	R9	R4	K71
      0x8828091A,  //  00F9  GETMBR	R10	R4	K26
      0x7C1C0600,  //  00FA  CALL	R7	3
      0x900E3207,  //  00FB  SETMBR	R3	K25	R7
      0x4C200000,  //  00FC  LDNIL	R8
      0x1C200608,  //  00FD  EQ	R8	R3	R8
      0x74220003,  //  00FE  JMPT	R8	#0103
      0x88200719,  //  00FF  GETMBR	R8	R3	K25
      0x4C240000,  //  0100  LDNIL	R9
      0x1C201009,  //  0101  EQ	R8	R8	R9
      0x7822000D,  //  0102  JMPF	R8	#0111
      0xB8220C00,  //  0103  GETNGBL	R8	K6
      0x8C201107,  //  0104  GETMET	R8	R8	K7
      0x58280048,  //  0105  LDCONST	R10	K72
      0x582C0009,  //  0106  LDCONST	R11	K9
      0x7C200600,  //  0107  CALL	R8	3
      0x8C20010A,  //  0108  GETMET	R8	R0	K10
      0x5C280200,  //  0109  MOVE	R10	R1
      0x582C000B,  //  010A  LDCONST	R11	K11
      0x58300004,  //  010B  LDCONST	R12	K4
      0x5834000B,  //  010C  LDCONST	R13	K11
      0x50380000,  //  010D  LDBOOL	R14	0	0
      0x7C200C00,  //  010E  CALL	R8	6
      0x50200000,  //  010F  LDBOOL	R8	0	0
      0x80041000,  //  0110  RET	1	R8
      0x88200326,  //  0111  GETMBR	R8	R1	K38
      0x900E4A08,  //  0112  SETMBR	R3	K37	R8
      0x8C200727,  //  0113  GETMET	R8	R3	K39
      0x7C200200,  //  0114  CALL	R8	1
      0x88200929,  //  0115  GETMBR	R8	R4	K41
      0x900E5008,  //  0116  SETMBR	R3	K40	R8
      0x88200116,  //  0117  GETMBR	R8	R0	K22
      0x88201117,  //  0118  GETMBR	R8	R8	K23
      0x8C20112B,  //  0119  GETMET	R8	R8	K43
      0x7C200200,  //  011A  CALL	R8	1
      0x900E5408,  //  011B  SETMBR	R3	K42	R8
      0xB8220C00,  //  011C  GETNGBL	R8	K6
      0x8C201107,  //  011D  GETMET	R8	R8	K7
      0x60280018,  //  011E  GETGBL	R10	G24
      0x582C002C,  //  011F  LDCONST	R11	K44
      0x8830072A,  //  0120  GETMBR	R12	R3	K42
      0x8834032D,  //  0121  GETMBR	R13	R1	K45
      0x8838032E,  //  0122  GETMBR	R14	R1	K46
      0x7C280800,  //  0123  CALL	R10	4
      0x582C0009,  //  0124  LDCONST	R11	K9
      0x7C200600,  //  0125  CALL	R8	3
      0x8C200530,  //  0126  GETMET	R8	R2	K48
      0x542A000F,  //  0127  LDINT	R10	16
      0x7C200400,  //  0128  CALL	R8	2
      0x900E5E08,  //  0129  SETMBR	R3	K47	R8
      0x8C200530,  //  012A  GETMET	R8	R2	K48
      0x542A001F,  //  012B  LDINT	R10	32
      0x7C200400,  //  012C  CALL	R8	2
      0x900E9208,  //  012D  SETMBR	R3	K73	R8
      0x8C20054B,  //  012E  GETMET	R8	R2	K75
      0x7C200200,  //  012F  CALL	R8	1
      0x8C20114C,  //  0130  GETMET	R8	R8	K76
      0x88280749,  //  0131  GETMBR	R10	R3	K73
      0x7C200400,  //  0132  CALL	R8	2
      0x900E9408,  //  0133  SETMBR	R3	K74	R8
      0x8C200530,  //  0134  GETMET	R8	R2	K48
      0x542A001F,  //  0135  LDINT	R10	32
      0x7C200400,  //  0136  CALL	R8	2
      0x8C24054B,  //  0137  GETMET	R9	R2	K75
      0x7C240200,  //  0138  CALL	R9	1
      0x8C24134D,  //  0139  GETMET	R9	R9	K77
      0x882C0749,  //  013A  GETMBR	R11	R3	K73
      0x88300913,  //  013B  GETMBR	R12	R4	K19
      0x7C240600,  //  013C  CALL	R9	3
      0x900E3E09,  //  013D  SETMBR	R3	K31	R9
      0xB8261A00,  //  013E  GETNGBL	R9	K13
      0x8824134E,  //  013F  GETMBR	R9	R9	K78
      0x8C24134F,  //  0140  GETMET	R9	R9	K79
      0x7C240200,  //  0141  CALL	R9	1
      0x8C281350,  //  0142  GETMET	R10	R9	K80
      0x5830000B,  //  0143  LDCONST	R12	K11
      0xB8361A00,  //  0144  GETNGBL	R13	K13
      0x88341B4E,  //  0145  GETMBR	R13	R13	K78
      0x88341B51,  //  0146  GETMBR	R13	R13	K81
      0x8C380F52,  //  0147  GETMET	R14	R7	K82
      0x7C380200,  //  0148  CALL	R14	1
      0x7C280800,  //  0149  CALL	R10	4
      0x8C281350,  //  014A  GETMET	R10	R9	K80
      0x5830000C,  //  014B  LDCONST	R12	K12
      0xB8361A00,  //  014C  GETNGBL	R13	K13
      0x88341B4E,  //  014D  GETMBR	R13	R13	K78
      0x88341B51,  //  014E  GETMBR	R13	R13	K81
      0x8C380F53,  //  014F  GETMET	R14	R7	K83
      0x7C380200,  //  0150  CALL	R14	1
      0x7C280800,  //  0151  CALL	R10	4
      0x8C281350,  //  0152  GETMET	R10	R9	K80
      0x58300009,  //  0153  LDCONST	R12	K9
      0xB8361A00,  //  0154  GETNGBL	R13	K13
      0x88341B4E,  //  0155  GETMBR	R13	R13	K78
      0x88341B51,  //  0156  GETMBR	R13	R13	K81
      0x8838074A,  //  0157  GETMBR	R14	R3	K74
      0x7C280800,  //  0158  CALL	R10	4
      0x8C281350,  //  0159  GETMET	R10	R9	K80
      0x54320003,  //  015A  LDINT	R12	4
      0xB8361A00,  //  015B  GETNGBL	R13	K13
      0x88341B4E,  //  015C  GETMBR	R13	R13	K78
      0x88341B51,  //  015D  GETMBR	R13	R13	K81
      0x88380913,  //  015E  GETMBR	R14	R4	K19
      0x7C280800,  //  015F  CALL	R10	4
      0x8C28054B,  //  0160  GETMET	R10	R2	K75
      0x7C280200,  //  0161  CALL	R10	1
      0x8C281554,  //  0162  GETMET	R10	R10	K84
      0x8C300F55,  //  0163  GETMET	R12	R7	K85
      0x7C300200,  //  0164  CALL	R12	1
      0x8C341338,  //  0165  GETMET	R13	R9	K56
      0x7C340200,  //  0166  CALL	R13	1
      0x7C280600,  //  0167  CALL	R10	3
      0xB82E1A00,  //  0168  GETNGBL	R11	K13
      0x882C174E,  //  0169  GETMBR	R11	R11	K78
      0x8C2C174F,  //  016A  GETMET	R11	R11	K79
      0x7C2C0200,  //  016B  CALL	R11	1
      0x8C301750,  //  016C  GETMET	R12	R11	K80
      0x5838000B,  //  016D  LDCONST	R14	K11
      0xB83E1A00,  //  016E  GETNGBL	R15	K13
      0x883C1F4E,  //  016F  GETMBR	R15	R15	K78
      0x883C1F51,  //  0170  GETMBR	R15	R15	K81
      0x8C400F52,  //  0171  GETMET	R16	R7	K82
      0x7C400200,  //  0172  CALL	R16	1
      0x7C300800,  //  0173  CALL	R12	4
      0x8C301750,  //  0174  GETMET	R12	R11	K80
      0x5838000C,  //  0175  LDCONST	R14	K12
      0xB83E1A00,  //  0176  GETNGBL	R15	K13
      0x883C1F4E,  //  0177  GETMBR	R15	R15	K78
      0x883C1F51,  //  0178  GETMBR	R15	R15	K81
      0x8C400F53,  //  0179  GETMET	R16	R7	K83
      0x7C400200,  //  017A  CALL	R16	1
      0x7C300800,  //  017B  CALL	R12	4
      0x8C301750,  //  017C  GETMET	R12	R11	K80
      0x58380009,  //  017D  LDCONST	R14	K9
      0xB83E1A00,  //  017E  GETNGBL	R15	K13
      0x883C1F4E,  //  017F  GETMBR	R15	R15	K78
      0x883C1F51,  //  0180  GETMBR	R15	R15	K81
      0x5C401400,  //  0181  MOVE	R16	R10
      0x7C300800,  //  0182  CALL	R12	4
      0x8C301750,  //  0183  GETMET	R12	R11	K80
      0x543A0003,  //  0184  LDINT	R14	4
      0xB83E1A00,  //  0185  GETNGBL	R15	K13
      0x883C1F4E,  //  0186  GETMBR	R15	R15	K78
      0x883C1F51,  //  0187  GETMBR	R15	R15	K81
      0x8840072F,  //  0188  GETMBR	R16	R3	K47
      0x7C300800,  //  0189  CALL	R12	4
      0x88300956,  //  018A  GETMBR	R12	R4	K86
      0x900E720C,  //  018B  SETMBR	R3	K57	R12
      0x8C300557,  //  018C  GETMET	R12	R2	K87
      0x7C300200,  //  018D  CALL	R12	1
      0x8C301958,  //  018E  GETMET	R12	R12	K88
      0x88380739,  //  018F  GETMBR	R14	R3	K57
      0x7C300400,  //  0190  CALL	R12	2
      0x8C301959,  //  0191  GETMET	R12	R12	K89
      0x7C300200,  //  0192  CALL	R12	1
      0x60340015,  //  0193  GETGBL	R13	G21
      0x7C340000,  //  0194  CALL	R13	0
      0x8C341B1B,  //  0195  GETMET	R13	R13	K27
      0x883C015A,  //  0196  GETMBR	R15	R0	K90
      0x7C340400,  //  0197  CALL	R13	2
      0x8C380F5B,  //  0198  GETMET	R14	R7	K91
      0x7C380200,  //  0199  CALL	R14	1
      0x00381C08,  //  019A  ADD	R14	R14	R8
      0x883C074A,  //  019B  GETMBR	R15	R3	K74
      0x00381C0F,  //  019C  ADD	R14	R14	R15
      0x00381C0C,  //  019D  ADD	R14	R14	R12
      0x8C3C051D,  //  019E  GETMET	R15	R2	K29
      0x7C3C0200,  //  019F  CALL	R15	1
      0x8C3C1F1E,  //  01A0  GETMET	R15	R15	K30
      0x8844071F,  //  01A1  GETMBR	R17	R3	K31
      0x5C481C00,  //  01A2  MOVE	R18	R14
      0x5C4C1A00,  //  01A3  MOVE	R19	R13
      0x5452000F,  //  01A4  LDINT	R20	16
      0x7C3C0A00,  //  01A5  CALL	R15	5
      0x8C401738,  //  01A6  GETMET	R16	R11	K56
      0x7C400200,  //  01A7  CALL	R16	1
      0x8C440522,  //  01A8  GETMET	R17	R2	K34
      0x5C4C1E00,  //  01A9  MOVE	R19	R15
      0x60500015,  //  01AA  GETGBL	R20	G21
      0x7C500000,  //  01AB  CALL	R20	0
      0x8C50291B,  //  01AC  GETMET	R20	R20	K27
      0x8858015C,  //  01AD  GETMBR	R22	R0	K92
      0x7C500400,  //  01AE  CALL	R20	2
      0x60540015,  //  01AF  GETGBL	R21	G21
      0x7C540000,  //  01B0  CALL	R21	0
      0x6058000C,  //  01B1  GETGBL	R22	G12
      0x5C5C2000,  //  01B2  MOVE	R23	R16
      0x7C580200,  //  01B3  CALL	R22	1
      0x545E000F,  //  01B4  LDINT	R23	16
      0x7C440C00,  //  01B5  CALL	R17	6
      0x8C48235D,  //  01B6  GETMET	R18	R17	K93
      0x5C502000,  //  01B7  MOVE	R20	R16
      0x7C480400,  //  01B8  CALL	R18	2
      0x8C4C2324,  //  01B9  GETMET	R19	R17	K36
      0x7C4C0200,  //  01BA  CALL	R19	1
      0x00482413,  //  01BB  ADD	R18	R18	R19
      0xB84E1A00,  //  01BC  GETNGBL	R19	K13
      0x8C4C275E,  //  01BD  GETMET	R19	R19	K94
      0x7C4C0200,  //  01BE  CALL	R19	1
      0x904EBE08,  //  01BF  SETMBR	R19	K95	R8
      0x8850072A,  //  01C0  GETMBR	R20	R3	K42
      0x904EC014,  //  01C1  SETMBR	R19	K96	R20
      0x8850074A,  //  01C2  GETMBR	R20	R3	K74
      0x904EC214,  //  01C3  SETMBR	R19	K97	R20
      0x904EC412,  //  01C4  SETMBR	R19	K98	R18
      0x8C502738,  //  01C5  GETMET	R20	R19	K56
      0x7C500200,  //  01C6  CALL	R20	1
      0x900EC614,  //  01C7  SETMBR	R3	K99	R20
      0x8C54033A,  //  01C8  GETMET	R21	R1	K58
      0x545E0030,  //  01C9  LDINT	R23	49
      0x50600200,  //  01CA  LDBOOL	R24	1	0
      0x7C540600,  //  01CB  CALL	R21	3
      0x8C582B3B,  //  01CC  GETMET	R22	R21	K59
      0x5C602800,  //  01CD  MOVE	R24	R20
      0x7C580400,  //  01CE  CALL	R22	2
      0xB85E0C00,  //  01CF  GETNGBL	R23	K6
      0x8C5C2F07,  //  01D0  GETMET	R23	R23	K7
      0x60640018,  //  01D1  GETGBL	R25	G24
      0x58680064,  //  01D2  LDCONST	R26	K100
      0x886C072A,  //  01D3  GETMBR	R27	R3	K42
      0x88700765,  //  01D4  GETMBR	R28	R3	K101
      0x88740766,  //  01D5  GETMBR	R29	R3	K102
      0x7C640800,  //  01D6  CALL	R25	4
      0x5868000C,  //  01D7  LDCONST	R26	K12
      0x7C5C0600,  //  01D8  CALL	R23	3
      0x885C013C,  //  01D9  GETMBR	R23	R0	K60
      0x8C5C2F3D,  //  01DA  GETMET	R23	R23	K61
      0x5C642A00,  //  01DB  MOVE	R25	R21
      0x7C5C0400,  //  01DC  CALL	R23	2
      0x505C0200,  //  01DD  LDBOOL	R23	1	0
      0x80042E00,  //  01DE  RET	1	R23
      0x501C0200,  //  01DF  LDBOOL	R7	1	0
      0x80040E00,  //  01E0  RET	1	R7
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
