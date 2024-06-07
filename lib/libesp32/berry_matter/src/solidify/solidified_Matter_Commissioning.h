/* Solidification of Matter_Commissioning.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Commisioning_Context;

/********************************************************************
** Solidified function: parse_StatusReport
********************************************************************/
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_parse_StatusReport,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20),
    /* K3   */  be_nested_str_weak(raw),
    /* K4   */  be_nested_str_weak(app_payload_idx),
    /* K5   */  be_const_int(2147483647),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_const_int(3),
    }),
    be_str_weak(parse_StatusReport),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x88100304,  //  0002  GETMBR	R4	R1	K4
      0x40100905,  //  0003  CONNECT	R4	R4	K5
      0x88140303,  //  0004  GETMBR	R5	R1	K3
      0x94100A04,  //  0005  GETIDX	R4	R5	R4
      0x8C100906,  //  0006  GETMET	R4	R4	K6
      0x7C100200,  //  0007  CALL	R4	1
      0x00120404,  //  0008  ADD	R4	K2	R4
      0x58140007,  //  0009  LDCONST	R5	K7
      0x7C0C0400,  //  000A  CALL	R3	2
      0x500C0000,  //  000B  LDBOOL	R3	0	0
      0x80040600,  //  000C  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_fabric_by_destination_id
********************************************************************/
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_find_fabric_by_destination_id,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
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
    ( &(const binstruction[71]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x541A0003,  //  0003  LDINT	R6	4
      0x7C100400,  //  0004  CALL	R4	2
      0x78120005,  //  0005  JMPF	R4	#000C
      0xB8120600,  //  0006  GETNGBL	R4	K3
      0x8C140305,  //  0007  GETMET	R5	R1	K5
      0x7C140200,  //  0008  CALL	R5	1
      0x00160805,  //  0009  ADD	R5	K4	R5
      0x541A0003,  //  000A  LDINT	R6	4
      0x7C100400,  //  000B  CALL	R4	2
      0x60100010,  //  000C  GETGBL	R4	G16
      0x88140106,  //  000D  GETMBR	R5	R0	K6
      0x88140B07,  //  000E  GETMBR	R5	R5	K7
      0x88140B08,  //  000F  GETMBR	R5	R5	K8
      0x7C100200,  //  0010  CALL	R4	1
      0xA802002F,  //  0011  EXBLK	0	#0042
      0x5C140800,  //  0012  MOVE	R5	R4
      0x7C140000,  //  0013  CALL	R5	0
      0x88180B09,  //  0014  GETMBR	R6	R5	K9
      0x4C1C0000,  //  0015  LDNIL	R7
      0x1C180C07,  //  0016  EQ	R6	R6	R7
      0x741A0007,  //  0017  JMPT	R6	#0020
      0x88180B0A,  //  0018  GETMBR	R6	R5	K10
      0x4C1C0000,  //  0019  LDNIL	R7
      0x1C180C07,  //  001A  EQ	R6	R6	R7
      0x741A0003,  //  001B  JMPT	R6	#0020
      0x88180B0B,  //  001C  GETMBR	R6	R5	K11
      0x4C1C0000,  //  001D  LDNIL	R7
      0x1C180C07,  //  001E  EQ	R6	R6	R7
      0x781A0000,  //  001F  JMPF	R6	#0021
      0x7001FFF0,  //  0020  JMP		#0012
      0x8C180B0C,  //  0021  GETMET	R6	R5	K12
      0x7C180200,  //  0022  CALL	R6	1
      0x00180406,  //  0023  ADD	R6	R2	R6
      0x881C0B0A,  //  0024  GETMBR	R7	R5	K10
      0x00180C07,  //  0025  ADD	R6	R6	R7
      0x881C0B0B,  //  0026  GETMBR	R7	R5	K11
      0x00180C07,  //  0027  ADD	R6	R6	R7
      0x8C1C0B0D,  //  0028  GETMET	R7	R5	K13
      0x7C1C0200,  //  0029  CALL	R7	1
      0x8C20070E,  //  002A  GETMET	R8	R3	K14
      0x5C280E00,  //  002B  MOVE	R10	R7
      0x7C200400,  //  002C  CALL	R8	2
      0x8C24110F,  //  002D  GETMET	R9	R8	K15
      0x5C2C0C00,  //  002E  MOVE	R11	R6
      0x7C240400,  //  002F  CALL	R9	2
      0x8C241110,  //  0030  GETMET	R9	R8	K16
      0x7C240200,  //  0031  CALL	R9	1
      0xB82A0200,  //  0032  GETNGBL	R10	K1
      0x8C281502,  //  0033  GETMET	R10	R10	K2
      0x54320003,  //  0034  LDINT	R12	4
      0x7C280400,  //  0035  CALL	R10	2
      0x782A0005,  //  0036  JMPF	R10	#003D
      0xB82A0600,  //  0037  GETNGBL	R10	K3
      0x8C2C1305,  //  0038  GETMET	R11	R9	K5
      0x7C2C0200,  //  0039  CALL	R11	1
      0x002E220B,  //  003A  ADD	R11	K17	R11
      0x54320003,  //  003B  LDINT	R12	4
      0x7C280400,  //  003C  CALL	R10	2
      0x1C281201,  //  003D  EQ	R10	R9	R1
      0x782A0001,  //  003E  JMPF	R10	#0041
      0xA8040001,  //  003F  EXBLK	1	1
      0x80040A00,  //  0040  RET	1	R5
      0x7001FFCF,  //  0041  JMP		#0012
      0x58100012,  //  0042  LDCONST	R4	K18
      0xAC100200,  //  0043  CATCH	R4	1	0
      0xB0080000,  //  0044  RAISE	2	R0	R0
      0x4C100000,  //  0045  LDNIL	R4
      0x80040800,  //  0046  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_every_second,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
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
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_add_session,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(log),
    /* K1   */  be_nested_str_weak(MTR_X3A_X20add_session_X20local_session_id_X3D_X25i_X20initiator_session_id_X3D_X25i),
    /* K2   */  be_nested_str_weak(device),
    /* K3   */  be_nested_str_weak(sessions),
    /* K4   */  be_nested_str_weak(create_session),
    /* K5   */  be_nested_str_weak(set_keys),
    }),
    be_str_weak(add_session),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB81A0000,  //  0000  GETNGBL	R6	K0
      0x601C0018,  //  0001  GETGBL	R7	G24
      0x58200001,  //  0002  LDCONST	R8	K1
      0x5C240200,  //  0003  MOVE	R9	R1
      0x5C280400,  //  0004  MOVE	R10	R2
      0x7C1C0600,  //  0005  CALL	R7	3
      0x54220003,  //  0006  LDINT	R8	4
      0x7C180400,  //  0007  CALL	R6	2
      0x88180102,  //  0008  GETMBR	R6	R0	K2
      0x88180D03,  //  0009  GETMBR	R6	R6	K3
      0x8C180D04,  //  000A  GETMET	R6	R6	K4
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240400,  //  000C  MOVE	R9	R2
      0x7C180600,  //  000D  CALL	R6	3
      0x8C1C0D05,  //  000E  GETMET	R7	R6	K5
      0x5C240600,  //  000F  MOVE	R9	R3
      0x5C280800,  //  0010  MOVE	R10	R4
      0x5C2C0A00,  //  0011  MOVE	R11	R5
      0x7C1C0800,  //  0012  CALL	R7	4
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_PBKDFParamRequest
********************************************************************/
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_parse_PBKDFParamRequest,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
    1,                          /* has constants */
    ( &(const bvalue[47]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20invalid_X20PBKDFParamRequest_X20message),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(send_status_report),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(PBKDFParamRequest),
    /* K15  */  be_nested_str_weak(parse),
    /* K16  */  be_nested_str_weak(raw),
    /* K17  */  be_nested_str_weak(app_payload_idx),
    /* K18  */  be_nested_str_weak(set_mode_PASE),
    /* K19  */  be_nested_str_weak(__Msg1),
    /* K20  */  be_const_int(2147483647),
    /* K21  */  be_nested_str_weak(passcodeId),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20non_X2Dzero_X20passcode_X20id),
    /* K23  */  be_nested_str_weak(__future_initiator_session_id),
    /* K24  */  be_nested_str_weak(initiator_session_id),
    /* K25  */  be_nested_str_weak(__future_local_session_id),
    /* K26  */  be_nested_str_weak(device),
    /* K27  */  be_nested_str_weak(sessions),
    /* K28  */  be_nested_str_weak(gen_local_session_id),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K30  */  be_nested_str_weak(remote_ip),
    /* K31  */  be_nested_str_weak(remote_port),
    /* K32  */  be_nested_str_weak(PBKDFParamResponse),
    /* K33  */  be_nested_str_weak(initiatorRandom),
    /* K34  */  be_nested_str_weak(responderRandom),
    /* K35  */  be_nested_str_weak(random),
    /* K36  */  be_nested_str_weak(responderSessionId),
    /* K37  */  be_nested_str_weak(pbkdf_parameters_salt),
    /* K38  */  be_nested_str_weak(commissioning_salt),
    /* K39  */  be_nested_str_weak(pbkdf_parameters_iterations),
    /* K40  */  be_nested_str_weak(commissioning_iterations),
    /* K41  */  be_nested_str_weak(tlv2raw),
    /* K42  */  be_nested_str_weak(__Msg2),
    /* K43  */  be_nested_str_weak(build_response),
    /* K44  */  be_nested_str_weak(encode_frame),
    /* K45  */  be_nested_str_weak(responder),
    /* K46  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[113]) {  /* code */
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
      0x78120010,  //  000B  JMPF	R4	#001D
      0xB8120C00,  //  000C  GETNGBL	R4	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0xB8120C00,  //  0010  GETNGBL	R4	K6
      0x58140009,  //  0011  LDCONST	R5	K9
      0x5818000A,  //  0012  LDCONST	R6	K10
      0x7C100400,  //  0013  CALL	R4	2
      0x8C10010B,  //  0014  GETMET	R4	R0	K11
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C000C,  //  0016  LDCONST	R7	K12
      0x58200004,  //  0017  LDCONST	R8	K4
      0x58240008,  //  0018  LDCONST	R9	K8
      0x50280000,  //  0019  LDBOOL	R10	0	0
      0x7C100C00,  //  001A  CALL	R4	6
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x80040800,  //  001C  RET	1	R4
      0xB8121A00,  //  001D  GETNGBL	R4	K13
      0x8C10090E,  //  001E  GETMET	R4	R4	K14
      0x7C100200,  //  001F  CALL	R4	1
      0x8C10090F,  //  0020  GETMET	R4	R4	K15
      0x88180310,  //  0021  GETMBR	R6	R1	K16
      0x881C0311,  //  0022  GETMBR	R7	R1	K17
      0x7C100600,  //  0023  CALL	R4	3
      0x88140301,  //  0024  GETMBR	R5	R1	K1
      0x8C140B12,  //  0025  GETMET	R5	R5	K18
      0x7C140200,  //  0026  CALL	R5	1
      0x88140311,  //  0027  GETMBR	R5	R1	K17
      0x40140B14,  //  0028  CONNECT	R5	R5	K20
      0x88180310,  //  0029  GETMBR	R6	R1	K16
      0x94140C05,  //  002A  GETIDX	R5	R6	R5
      0x900E2605,  //  002B  SETMBR	R3	K19	R5
      0x88140915,  //  002C  GETMBR	R5	R4	K21
      0x20140B04,  //  002D  NE	R5	R5	K4
      0x78160010,  //  002E  JMPF	R5	#0040
      0xB8160C00,  //  002F  GETNGBL	R5	K6
      0x58180016,  //  0030  LDCONST	R6	K22
      0x581C0008,  //  0031  LDCONST	R7	K8
      0x7C140400,  //  0032  CALL	R5	2
      0xB8160C00,  //  0033  GETNGBL	R5	K6
      0x58180009,  //  0034  LDCONST	R6	K9
      0x581C000A,  //  0035  LDCONST	R7	K10
      0x7C140400,  //  0036  CALL	R5	2
      0x8C14010B,  //  0037  GETMET	R5	R0	K11
      0x5C1C0200,  //  0038  MOVE	R7	R1
      0x5820000C,  //  0039  LDCONST	R8	K12
      0x58240004,  //  003A  LDCONST	R9	K4
      0x58280008,  //  003B  LDCONST	R10	K8
      0x502C0000,  //  003C  LDBOOL	R11	0	0
      0x7C140C00,  //  003D  CALL	R5	6
      0x50140000,  //  003E  LDBOOL	R5	0	0
      0x80040A00,  //  003F  RET	1	R5
      0x88140918,  //  0040  GETMBR	R5	R4	K24
      0x900E2E05,  //  0041  SETMBR	R3	K23	R5
      0x8814011A,  //  0042  GETMBR	R5	R0	K26
      0x88140B1B,  //  0043  GETMBR	R5	R5	K27
      0x8C140B1C,  //  0044  GETMET	R5	R5	K28
      0x7C140200,  //  0045  CALL	R5	1
      0x900E3205,  //  0046  SETMBR	R3	K25	R5
      0xB8160C00,  //  0047  GETNGBL	R5	K6
      0x60180018,  //  0048  GETGBL	R6	G24
      0x581C001D,  //  0049  LDCONST	R7	K29
      0x88200719,  //  004A  GETMBR	R8	R3	K25
      0x8824031E,  //  004B  GETMBR	R9	R1	K30
      0x8828031F,  //  004C  GETMBR	R10	R1	K31
      0x7C180800,  //  004D  CALL	R6	4
      0x581C000A,  //  004E  LDCONST	R7	K10
      0x7C140400,  //  004F  CALL	R5	2
      0xB8161A00,  //  0050  GETNGBL	R5	K13
      0x8C140B20,  //  0051  GETMET	R5	R5	K32
      0x7C140200,  //  0052  CALL	R5	1
      0x88180921,  //  0053  GETMBR	R6	R4	K33
      0x90164206,  //  0054  SETMBR	R5	K33	R6
      0x8C180523,  //  0055  GETMET	R6	R2	K35
      0x5422001F,  //  0056  LDINT	R8	32
      0x7C180400,  //  0057  CALL	R6	2
      0x90164406,  //  0058  SETMBR	R5	K34	R6
      0x88180719,  //  0059  GETMBR	R6	R3	K25
      0x90164806,  //  005A  SETMBR	R5	K36	R6
      0x8818011A,  //  005B  GETMBR	R6	R0	K26
      0x88180D26,  //  005C  GETMBR	R6	R6	K38
      0x90164A06,  //  005D  SETMBR	R5	K37	R6
      0x8818011A,  //  005E  GETMBR	R6	R0	K26
      0x88180D28,  //  005F  GETMBR	R6	R6	K40
      0x90164E06,  //  0060  SETMBR	R5	K39	R6
      0x8C180B29,  //  0061  GETMET	R6	R5	K41
      0x7C180200,  //  0062  CALL	R6	1
      0x900E5406,  //  0063  SETMBR	R3	K42	R6
      0x8C1C032B,  //  0064  GETMET	R7	R1	K43
      0x54260020,  //  0065  LDINT	R9	33
      0x50280200,  //  0066  LDBOOL	R10	1	0
      0x7C1C0600,  //  0067  CALL	R7	3
      0x8C200F2C,  //  0068  GETMET	R8	R7	K44
      0x5C280C00,  //  0069  MOVE	R10	R6
      0x7C200400,  //  006A  CALL	R8	2
      0x8824012D,  //  006B  GETMBR	R9	R0	K45
      0x8C24132E,  //  006C  GETMET	R9	R9	K46
      0x5C2C0E00,  //  006D  MOVE	R11	R7
      0x7C240400,  //  006E  CALL	R9	2
      0x50240200,  //  006F  LDBOOL	R9	1	0
      0x80041200,  //  0070  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_status_report
********************************************************************/
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_send_status_report,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
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
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_parse_Pake1,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
    1,                          /* has constants */
    ( &(const bvalue[51]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake1_X20message),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K10  */  be_nested_str_weak(send_status_report),
    /* K11  */  be_const_int(1),
    /* K12  */  be_const_int(2),
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
    /* K45  */  be_nested_str_weak(MTR_X3A_X20New_X20Commissioning_X20_X28PASE_X20id_X3D_X25i_X29_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K46  */  be_nested_str_weak(__future_local_session_id),
    /* K47  */  be_nested_str_weak(_ip),
    /* K48  */  be_nested_str_weak(_port),
    /* K49  */  be_nested_str_weak(responder),
    /* K50  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[112]) {  /* code */
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
      0x78120010,  //  000B  JMPF	R4	#001D
      0xB8120C00,  //  000C  GETNGBL	R4	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0xB8120C00,  //  0010  GETNGBL	R4	K6
      0x58140009,  //  0011  LDCONST	R5	K9
      0x58180008,  //  0012  LDCONST	R6	K8
      0x7C100400,  //  0013  CALL	R4	2
      0x8C10010A,  //  0014  GETMET	R4	R0	K10
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C000B,  //  0016  LDCONST	R7	K11
      0x58200004,  //  0017  LDCONST	R8	K4
      0x5824000C,  //  0018  LDCONST	R9	K12
      0x50280000,  //  0019  LDBOOL	R10	0	0
      0x7C100C00,  //  001A  CALL	R4	6
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x80040800,  //  001C  RET	1	R4
      0xB8121A00,  //  001D  GETNGBL	R4	K13
      0x8C10090E,  //  001E  GETMET	R4	R4	K14
      0x7C100200,  //  001F  CALL	R4	1
      0x8C10090F,  //  0020  GETMET	R4	R4	K15
      0x88180310,  //  0021  GETMBR	R6	R1	K16
      0x881C0311,  //  0022  GETMBR	R7	R1	K17
      0x7C100600,  //  0023  CALL	R4	3
      0x88140912,  //  0024  GETMBR	R5	R4	K18
      0x8C180513,  //  0025  GETMET	R6	R2	K19
      0x88200114,  //  0026  GETMBR	R8	R0	K20
      0x88201115,  //  0027  GETMBR	R8	R8	K21
      0x4C240000,  //  0028  LDNIL	R9
      0x88280114,  //  0029  GETMBR	R10	R0	K20
      0x88281516,  //  002A  GETMBR	R10	R10	K22
      0x7C180800,  //  002B  CALL	R6	4
      0x8C1C0517,  //  002C  GETMET	R7	R2	K23
      0x5426001F,  //  002D  LDINT	R9	32
      0x7C1C0400,  //  002E  CALL	R7	2
      0x8C200D18,  //  002F  GETMET	R8	R6	K24
      0x5C280E00,  //  0030  MOVE	R10	R7
      0x7C200400,  //  0031  CALL	R8	2
      0x8C200D19,  //  0032  GETMET	R8	R6	K25
      0x5C280A00,  //  0033  MOVE	R10	R5
      0x7C200400,  //  0034  CALL	R8	2
      0x8C20051A,  //  0035  GETMET	R8	R2	K26
      0x7C200200,  //  0036  CALL	R8	1
      0x8C24111B,  //  0037  GETMET	R9	R8	K27
      0x602C0015,  //  0038  GETGBL	R11	G21
      0x7C2C0000,  //  0039  CALL	R11	0
      0x8C2C171C,  //  003A  GETMET	R11	R11	K28
      0x8834011D,  //  003B  GETMBR	R13	R0	K29
      0x7C2C0400,  //  003C  CALL	R11	2
      0x7C240400,  //  003D  CALL	R9	2
      0x8C24111B,  //  003E  GETMET	R9	R8	K27
      0x882C071E,  //  003F  GETMBR	R11	R3	K30
      0x7C240400,  //  0040  CALL	R9	2
      0x8C24111B,  //  0041  GETMET	R9	R8	K27
      0x882C071F,  //  0042  GETMBR	R11	R3	K31
      0x7C240400,  //  0043  CALL	R9	2
      0x8C241120,  //  0044  GETMET	R9	R8	K32
      0x7C240200,  //  0045  CALL	R9	1
      0x901A2405,  //  0046  SETMBR	R6	K18	R5
      0x8C280D21,  //  0047  GETMET	R10	R6	K33
      0x5C301200,  //  0048  MOVE	R12	R9
      0x7C280400,  //  0049  CALL	R10	2
      0x8C280D22,  //  004A  GETMET	R10	R6	K34
      0x50300200,  //  004B  LDBOOL	R12	1	0
      0x7C280400,  //  004C  CALL	R10	2
      0xB82A1A00,  //  004D  GETNGBL	R10	K13
      0x8C281523,  //  004E  GETMET	R10	R10	K35
      0x7C280200,  //  004F  CALL	R10	1
      0x882C0D24,  //  0050  GETMBR	R11	R6	K36
      0x902A480B,  //  0051  SETMBR	R10	K36	R11
      0x882C0D25,  //  0052  GETMBR	R11	R6	K37
      0x902A4A0B,  //  0053  SETMBR	R10	K37	R11
      0x8C2C1526,  //  0054  GETMET	R11	R10	K38
      0x7C2C0200,  //  0055  CALL	R11	1
      0x88300D28,  //  0056  GETMBR	R12	R6	K40
      0x900E4E0C,  //  0057  SETMBR	R3	K39	R12
      0x88300D2A,  //  0058  GETMBR	R12	R6	K42
      0x900E520C,  //  0059  SETMBR	R3	K41	R12
      0x8C30032B,  //  005A  GETMET	R12	R1	K43
      0x543A0022,  //  005B  LDINT	R14	35
      0x503C0200,  //  005C  LDBOOL	R15	1	0
      0x7C300600,  //  005D  CALL	R12	3
      0x8C34192C,  //  005E  GETMET	R13	R12	K44
      0x5C3C1600,  //  005F  MOVE	R15	R11
      0x7C340400,  //  0060  CALL	R13	2
      0xB83A0C00,  //  0061  GETNGBL	R14	K6
      0x603C0018,  //  0062  GETGBL	R15	G24
      0x5840002D,  //  0063  LDCONST	R16	K45
      0x8844072E,  //  0064  GETMBR	R17	R3	K46
      0x8848072F,  //  0065  GETMBR	R18	R3	K47
      0x884C0730,  //  0066  GETMBR	R19	R3	K48
      0x7C3C0800,  //  0067  CALL	R15	4
      0x5840000C,  //  0068  LDCONST	R16	K12
      0x7C380400,  //  0069  CALL	R14	2
      0x88380131,  //  006A  GETMBR	R14	R0	K49
      0x8C381D32,  //  006B  GETMET	R14	R14	K50
      0x5C401800,  //  006C  MOVE	R16	R12
      0x7C380400,  //  006D  CALL	R14	2
      0x50380200,  //  006E  LDBOOL	R14	1	0
      0x80041C00,  //  006F  RET	1	R14
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
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
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_parse_Pake3,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
    1,                          /* has constants */
    ( &(const bvalue[31]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake3_X20message),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K10  */  be_nested_str_weak(send_status_report),
    /* K11  */  be_const_int(1),
    /* K12  */  be_const_int(2),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(Pake3),
    /* K15  */  be_nested_str_weak(parse),
    /* K16  */  be_nested_str_weak(raw),
    /* K17  */  be_nested_str_weak(app_payload_idx),
    /* K18  */  be_nested_str_weak(cA),
    /* K19  */  be_nested_str_weak(__spake_cA),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20invalid_X20cA_X20received),
    /* K21  */  be_nested_str_weak(tasmota),
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
    ( &(const binstruction[101]) {  /* code */
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
      0x78120010,  //  000B  JMPF	R4	#001D
      0xB8120C00,  //  000C  GETNGBL	R4	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0xB8120C00,  //  0010  GETNGBL	R4	K6
      0x58140009,  //  0011  LDCONST	R5	K9
      0x58180008,  //  0012  LDCONST	R6	K8
      0x7C100400,  //  0013  CALL	R4	2
      0x8C10010A,  //  0014  GETMET	R4	R0	K10
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C000B,  //  0016  LDCONST	R7	K11
      0x58200004,  //  0017  LDCONST	R8	K4
      0x5824000C,  //  0018  LDCONST	R9	K12
      0x50280000,  //  0019  LDBOOL	R10	0	0
      0x7C100C00,  //  001A  CALL	R4	6
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x80040800,  //  001C  RET	1	R4
      0xB8121A00,  //  001D  GETNGBL	R4	K13
      0x8C10090E,  //  001E  GETMET	R4	R4	K14
      0x7C100200,  //  001F  CALL	R4	1
      0x8C10090F,  //  0020  GETMET	R4	R4	K15
      0x88180310,  //  0021  GETMBR	R6	R1	K16
      0x881C0311,  //  0022  GETMBR	R7	R1	K17
      0x7C100600,  //  0023  CALL	R4	3
      0x88140912,  //  0024  GETMBR	R5	R4	K18
      0x88180713,  //  0025  GETMBR	R6	R3	K19
      0x20180A06,  //  0026  NE	R6	R5	R6
      0x781A0010,  //  0027  JMPF	R6	#0039
      0xB81A0C00,  //  0028  GETNGBL	R6	K6
      0x581C0014,  //  0029  LDCONST	R7	K20
      0x58200008,  //  002A  LDCONST	R8	K8
      0x7C180400,  //  002B  CALL	R6	2
      0xB81A0C00,  //  002C  GETNGBL	R6	K6
      0x581C0009,  //  002D  LDCONST	R7	K9
      0x58200008,  //  002E  LDCONST	R8	K8
      0x7C180400,  //  002F  CALL	R6	2
      0x8C18010A,  //  0030  GETMET	R6	R0	K10
      0x5C200200,  //  0031  MOVE	R8	R1
      0x5824000B,  //  0032  LDCONST	R9	K11
      0x58280004,  //  0033  LDCONST	R10	K4
      0x582C000C,  //  0034  LDCONST	R11	K12
      0x50300000,  //  0035  LDBOOL	R12	0	0
      0x7C180C00,  //  0036  CALL	R6	6
      0x50180000,  //  0037  LDBOOL	R6	0	0
      0x80040C00,  //  0038  RET	1	R6
      0xB81A2A00,  //  0039  GETNGBL	R6	K21
      0x8C180D16,  //  003A  GETMET	R6	R6	K22
      0x7C180200,  //  003B  CALL	R6	1
      0x8C1C0517,  //  003C  GETMET	R7	R2	K23
      0x7C1C0200,  //  003D  CALL	R7	1
      0x8C1C0F18,  //  003E  GETMET	R7	R7	K24
      0x88240719,  //  003F  GETMBR	R9	R3	K25
      0x60280015,  //  0040  GETGBL	R10	G21
      0x7C280000,  //  0041  CALL	R10	0
      0x602C0015,  //  0042  GETGBL	R11	G21
      0x7C2C0000,  //  0043  CALL	R11	0
      0x8C2C171A,  //  0044  GETMET	R11	R11	K26
      0x8834011B,  //  0045  GETMBR	R13	R0	K27
      0x7C2C0400,  //  0046  CALL	R11	2
      0x5432002F,  //  0047  LDINT	R12	48
      0x7C1C0A00,  //  0048  CALL	R7	5
      0x5422000E,  //  0049  LDINT	R8	15
      0x40220808,  //  004A  CONNECT	R8	K4	R8
      0x94200E08,  //  004B  GETIDX	R8	R7	R8
      0x5426000F,  //  004C  LDINT	R9	16
      0x542A001E,  //  004D  LDINT	R10	31
      0x4024120A,  //  004E  CONNECT	R9	R9	R10
      0x94240E09,  //  004F  GETIDX	R9	R7	R9
      0x542A001F,  //  0050  LDINT	R10	32
      0x542E002E,  //  0051  LDINT	R11	47
      0x4028140B,  //  0052  CONNECT	R10	R10	R11
      0x94280E0A,  //  0053  GETIDX	R10	R7	R10
      0x8C2C010A,  //  0054  GETMET	R11	R0	K10
      0x5C340200,  //  0055  MOVE	R13	R1
      0x58380004,  //  0056  LDCONST	R14	K4
      0x583C0004,  //  0057  LDCONST	R15	K4
      0x58400004,  //  0058  LDCONST	R16	K4
      0x50440000,  //  0059  LDBOOL	R17	0	0
      0x7C2C0C00,  //  005A  CALL	R11	6
      0x8C2C011C,  //  005B  GETMET	R11	R0	K28
      0x8834071D,  //  005C  GETMBR	R13	R3	K29
      0x8838071E,  //  005D  GETMBR	R14	R3	K30
      0x5C3C1000,  //  005E  MOVE	R15	R8
      0x5C401200,  //  005F  MOVE	R16	R9
      0x5C441400,  //  0060  MOVE	R17	R10
      0x5C480C00,  //  0061  MOVE	R18	R6
      0x7C2C0E00,  //  0062  CALL	R11	7
      0x502C0200,  //  0063  LDBOOL	R11	1	0
      0x80041600,  //  0064  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Sigma3
********************************************************************/
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_parse_Sigma3,   /* name */
  be_nested_proto(
    36,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
    1,                          /* has constants */
    ( &(const bvalue[66]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(send_status_report),
    /* K9   */  be_const_int(1),
    /* K10  */  be_const_int(2),
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
    /* K37  */  be_nested_str_weak(findsub),
    /* K38  */  be_nested_str_weak(int),
    /* K39  */  be_nested_str_weak(peer_node_id),
    /* K40  */  be_nested_str_weak(int64),
    /* K41  */  be_nested_str_weak(fromu32),
    /* K42  */  be_nested_str_weak(tobytes),
    /* K43  */  be_nested_str_weak(Matter_TLV_struct),
    /* K44  */  be_nested_str_weak(add_TLV),
    /* K45  */  be_nested_str_weak(B1),
    /* K46  */  be_nested_str_weak(__initiator_pub),
    /* K47  */  be_nested_str_weak(__responder_pub),
    /* K48  */  be_nested_str_weak(tlv2raw),
    /* K49  */  be_nested_str_weak(EC_P256),
    /* K50  */  be_nested_str_weak(ecdsa_verify_sha256),
    /* K51  */  be_nested_str_weak(MTR_X3A_X20sigma3_tbs_X20does_X20not_X20have_X20a_X20valid_X20signature),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X20Invalid_X20signature_X2C_X20trying_X20anyways),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20Sigma3_X20verified_X2C_X20computing_X20new_X20keys),
    /* K54  */  be_nested_str_weak(Msg3),
    /* K55  */  be_nested_str_weak(SEKeys_Info),
    /* K56  */  be_nested_str_weak(tasmota),
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
    ( &(const binstruction[280]) {  /* code */
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
      0x780E000C,  //  000A  JMPF	R3	#0018
      0xB80E0A00,  //  000B  GETNGBL	R3	K5
      0x58100006,  //  000C  LDCONST	R4	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C0C0108,  //  000F  GETMET	R3	R0	K8
      0x5C140200,  //  0010  MOVE	R5	R1
      0x58180009,  //  0011  LDCONST	R6	K9
      0x581C0003,  //  0012  LDCONST	R7	K3
      0x5820000A,  //  0013  LDCONST	R8	K10
      0x50240000,  //  0014  LDBOOL	R9	0	0
      0x7C0C0C00,  //  0015  CALL	R3	6
      0x500C0000,  //  0016  LDBOOL	R3	0	0
      0x80040600,  //  0017  RET	1	R3
      0x880C030B,  //  0018  GETMBR	R3	R1	K11
      0xB8121800,  //  0019  GETNGBL	R4	K12
      0x8C10090D,  //  001A  GETMET	R4	R4	K13
      0x7C100200,  //  001B  CALL	R4	1
      0x8C10090E,  //  001C  GETMET	R4	R4	K14
      0x8818030F,  //  001D  GETMBR	R6	R1	K15
      0x881C0310,  //  001E  GETMBR	R7	R1	K16
      0x7C100600,  //  001F  CALL	R4	3
      0x8C140511,  //  0020  GETMET	R5	R2	K17
      0x7C140200,  //  0021  CALL	R5	1
      0x8C140B12,  //  0022  GETMET	R5	R5	K18
      0x881C0713,  //  0023  GETMBR	R7	R3	K19
      0x7C140400,  //  0024  CALL	R5	2
      0x8C140B12,  //  0025  GETMET	R5	R5	K18
      0x881C0714,  //  0026  GETMBR	R7	R3	K20
      0x7C140400,  //  0027  CALL	R5	2
      0x8C140B15,  //  0028  GETMET	R5	R5	K21
      0x7C140200,  //  0029  CALL	R5	1
      0x60180015,  //  002A  GETGBL	R6	G21
      0x7C180000,  //  002B  CALL	R6	0
      0x8C180D16,  //  002C  GETMET	R6	R6	K22
      0x88200117,  //  002D  GETMBR	R8	R0	K23
      0x7C180400,  //  002E  CALL	R6	2
      0x8C1C0518,  //  002F  GETMET	R7	R2	K24
      0x7C1C0200,  //  0030  CALL	R7	1
      0x8C1C0F19,  //  0031  GETMET	R7	R7	K25
      0x8824071A,  //  0032  GETMBR	R9	R3	K26
      0x8C28071B,  //  0033  GETMET	R10	R3	K27
      0x7C280200,  //  0034  CALL	R10	1
      0x00281405,  //  0035  ADD	R10	R10	R5
      0x5C2C0C00,  //  0036  MOVE	R11	R6
      0x5432000F,  //  0037  LDINT	R12	16
      0x7C1C0A00,  //  0038  CALL	R7	5
      0x5421FFEE,  //  0039  LDINT	R8	-17
      0x40220608,  //  003A  CONNECT	R8	K3	R8
      0x8824091C,  //  003B  GETMBR	R9	R4	K28
      0x94201208,  //  003C  GETIDX	R8	R9	R8
      0x5429FFEF,  //  003D  LDINT	R10	-16
      0x4028151D,  //  003E  CONNECT	R10	R10	K29
      0x882C091C,  //  003F  GETMBR	R11	R4	K28
      0x9424160A,  //  0040  GETIDX	R9	R11	R10
      0x8C30051E,  //  0041  GETMET	R12	R2	K30
      0x5C380E00,  //  0042  MOVE	R14	R7
      0x603C0015,  //  0043  GETGBL	R15	G21
      0x7C3C0000,  //  0044  CALL	R15	0
      0x8C3C1F16,  //  0045  GETMET	R15	R15	K22
      0x8844011F,  //  0046  GETMBR	R17	R0	K31
      0x7C3C0400,  //  0047  CALL	R15	2
      0x60400015,  //  0048  GETGBL	R16	G21
      0x7C400000,  //  0049  CALL	R16	0
      0x6044000C,  //  004A  GETGBL	R17	G12
      0x5C481000,  //  004B  MOVE	R18	R8
      0x7C440200,  //  004C  CALL	R17	1
      0x544A000F,  //  004D  LDINT	R18	16
      0x7C300C00,  //  004E  CALL	R12	6
      0x5C281800,  //  004F  MOVE	R10	R12
      0x8C301520,  //  0050  GETMET	R12	R10	K32
      0x5C381000,  //  0051  MOVE	R14	R8
      0x7C300400,  //  0052  CALL	R12	2
      0x5C2C1800,  //  0053  MOVE	R11	R12
      0x8C301521,  //  0054  GETMET	R12	R10	K33
      0x7C300200,  //  0055  CALL	R12	1
      0x20341809,  //  0056  NE	R13	R12	R9
      0x78360010,  //  0057  JMPF	R13	#0069
      0xB8360A00,  //  0058  GETNGBL	R13	K5
      0x58380022,  //  0059  LDCONST	R14	K34
      0x583C0007,  //  005A  LDCONST	R15	K7
      0x7C340400,  //  005B  CALL	R13	2
      0xB8360A00,  //  005C  GETNGBL	R13	K5
      0x58380006,  //  005D  LDCONST	R14	K6
      0x583C0007,  //  005E  LDCONST	R15	K7
      0x7C340400,  //  005F  CALL	R13	2
      0x8C340108,  //  0060  GETMET	R13	R0	K8
      0x5C3C0200,  //  0061  MOVE	R15	R1
      0x58400009,  //  0062  LDCONST	R16	K9
      0x58440003,  //  0063  LDCONST	R17	K3
      0x5848000A,  //  0064  LDCONST	R18	K10
      0x504C0000,  //  0065  LDBOOL	R19	0	0
      0x7C340C00,  //  0066  CALL	R13	6
      0x50340000,  //  0067  LDBOOL	R13	0	0
      0x80041A00,  //  0068  RET	1	R13
      0xB8361800,  //  0069  GETNGBL	R13	K12
      0x88341B23,  //  006A  GETMBR	R13	R13	K35
      0x8C341B0E,  //  006B  GETMET	R13	R13	K14
      0x5C3C1600,  //  006C  MOVE	R15	R11
      0x7C340400,  //  006D  CALL	R13	2
      0x8C381B24,  //  006E  GETMET	R14	R13	K36
      0x58400009,  //  006F  LDCONST	R16	K9
      0x7C380400,  //  0070  CALL	R14	2
      0x8C3C1B24,  //  0071  GETMET	R15	R13	K36
      0x5844000A,  //  0072  LDCONST	R17	K10
      0x7C3C0400,  //  0073  CALL	R15	2
      0x8C401B24,  //  0074  GETMET	R16	R13	K36
      0x58480007,  //  0075  LDCONST	R18	K7
      0x7C400400,  //  0076  CALL	R16	2
      0xB8461800,  //  0077  GETNGBL	R17	K12
      0x88442323,  //  0078  GETMBR	R17	R17	K35
      0x8C44230E,  //  0079  GETMET	R17	R17	K14
      0x5C4C1C00,  //  007A  MOVE	R19	R14
      0x7C440400,  //  007B  CALL	R17	2
      0x8C482324,  //  007C  GETMET	R18	R17	K36
      0x54520008,  //  007D  LDINT	R20	9
      0x7C480400,  //  007E  CALL	R18	2
      0x8C4C2325,  //  007F  GETMET	R19	R17	K37
      0x54560005,  //  0080  LDINT	R21	6
      0x7C4C0400,  //  0081  CALL	R19	2
      0x8C502724,  //  0082  GETMET	R20	R19	K36
      0x545A0010,  //  0083  LDINT	R22	17
      0x7C500400,  //  0084  CALL	R20	2
      0x60540004,  //  0085  GETGBL	R21	G4
      0x5C582800,  //  0086  MOVE	R22	R20
      0x7C540200,  //  0087  CALL	R21	1
      0x1C542B26,  //  0088  EQ	R21	R21	K38
      0x78560007,  //  0089  JMPF	R21	#0092
      0xB8565000,  //  008A  GETNGBL	R21	K40
      0x8C542B29,  //  008B  GETMET	R21	R21	K41
      0x5C5C2800,  //  008C  MOVE	R23	R20
      0x7C540400,  //  008D  CALL	R21	2
      0x8C542B2A,  //  008E  GETMET	R21	R21	K42
      0x7C540200,  //  008F  CALL	R21	1
      0x900E4E15,  //  0090  SETMBR	R3	K39	R21
      0x70020002,  //  0091  JMP		#0095
      0x8C54292A,  //  0092  GETMET	R21	R20	K42
      0x7C540200,  //  0093  CALL	R21	1
      0x900E4E15,  //  0094  SETMBR	R3	K39	R21
      0xB8561800,  //  0095  GETNGBL	R21	K12
      0x88542B23,  //  0096  GETMBR	R21	R21	K35
      0x8C542B2B,  //  0097  GETMET	R21	R21	K43
      0x7C540200,  //  0098  CALL	R21	1
      0x8C582B2C,  //  0099  GETMET	R22	R21	K44
      0x58600009,  //  009A  LDCONST	R24	K9
      0xB8661800,  //  009B  GETNGBL	R25	K12
      0x88643323,  //  009C  GETMBR	R25	R25	K35
      0x8864332D,  //  009D  GETMBR	R25	R25	K45
      0x5C681C00,  //  009E  MOVE	R26	R14
      0x7C580800,  //  009F  CALL	R22	4
      0x8C582B2C,  //  00A0  GETMET	R22	R21	K44
      0x5860000A,  //  00A1  LDCONST	R24	K10
      0xB8661800,  //  00A2  GETNGBL	R25	K12
      0x88643323,  //  00A3  GETMBR	R25	R25	K35
      0x8864332D,  //  00A4  GETMBR	R25	R25	K45
      0x5C681E00,  //  00A5  MOVE	R26	R15
      0x7C580800,  //  00A6  CALL	R22	4
      0x8C582B2C,  //  00A7  GETMET	R22	R21	K44
      0x58600007,  //  00A8  LDCONST	R24	K7
      0xB8661800,  //  00A9  GETNGBL	R25	K12
      0x88643323,  //  00AA  GETMBR	R25	R25	K35
      0x8864332D,  //  00AB  GETMBR	R25	R25	K45
      0x8868072E,  //  00AC  GETMBR	R26	R3	K46
      0x7C580800,  //  00AD  CALL	R22	4
      0x8C582B2C,  //  00AE  GETMET	R22	R21	K44
      0x54620003,  //  00AF  LDINT	R24	4
      0xB8661800,  //  00B0  GETNGBL	R25	K12
      0x88643323,  //  00B1  GETMBR	R25	R25	K35
      0x8864332D,  //  00B2  GETMBR	R25	R25	K45
      0x8868072F,  //  00B3  GETMBR	R26	R3	K47
      0x7C580800,  //  00B4  CALL	R22	4
      0x8C582B30,  //  00B5  GETMET	R22	R21	K48
      0x7C580200,  //  00B6  CALL	R22	1
      0x8C5C0531,  //  00B7  GETMET	R23	R2	K49
      0x7C5C0200,  //  00B8  CALL	R23	1
      0x8C5C2F32,  //  00B9  GETMET	R23	R23	K50
      0x5C642400,  //  00BA  MOVE	R25	R18
      0x5C682C00,  //  00BB  MOVE	R26	R22
      0x5C6C2000,  //  00BC  MOVE	R27	R16
      0x7C5C0800,  //  00BD  CALL	R23	4
      0x5C602E00,  //  00BE  MOVE	R24	R23
      0x74620008,  //  00BF  JMPT	R24	#00C9
      0xB8620A00,  //  00C0  GETNGBL	R24	K5
      0x58640033,  //  00C1  LDCONST	R25	K51
      0x5868000A,  //  00C2  LDCONST	R26	K10
      0x7C600400,  //  00C3  CALL	R24	2
      0xB8620A00,  //  00C4  GETNGBL	R24	K5
      0x58640034,  //  00C5  LDCONST	R25	K52
      0x5868000A,  //  00C6  LDCONST	R26	K10
      0x7C600400,  //  00C7  CALL	R24	2
      0x70020003,  //  00C8  JMP		#00CD
      0xB8620A00,  //  00C9  GETNGBL	R24	K5
      0x58640035,  //  00CA  LDCONST	R25	K53
      0x58680007,  //  00CB  LDCONST	R26	K7
      0x7C600400,  //  00CC  CALL	R24	2
      0x8C600511,  //  00CD  GETMET	R24	R2	K17
      0x7C600200,  //  00CE  CALL	R24	1
      0x8C603112,  //  00CF  GETMET	R24	R24	K18
      0x88680713,  //  00D0  GETMBR	R26	R3	K19
      0x7C600400,  //  00D1  CALL	R24	2
      0x8C603112,  //  00D2  GETMET	R24	R24	K18
      0x88680714,  //  00D3  GETMBR	R26	R3	K20
      0x7C600400,  //  00D4  CALL	R24	2
      0x8C603112,  //  00D5  GETMET	R24	R24	K18
      0x88680936,  //  00D6  GETMBR	R26	R4	K54
      0x7C600400,  //  00D7  CALL	R24	2
      0x8C603115,  //  00D8  GETMET	R24	R24	K21
      0x7C600200,  //  00D9  CALL	R24	1
      0x5C143000,  //  00DA  MOVE	R5	R24
      0x4C600000,  //  00DB  LDNIL	R24
      0x900E2618,  //  00DC  SETMBR	R3	K19	R24
      0x4C600000,  //  00DD  LDNIL	R24
      0x900E2818,  //  00DE  SETMBR	R3	K20	R24
      0x8C600518,  //  00DF  GETMET	R24	R2	K24
      0x7C600200,  //  00E0  CALL	R24	1
      0x8C603119,  //  00E1  GETMET	R24	R24	K25
      0x8868071A,  //  00E2  GETMBR	R26	R3	K26
      0x8C6C071B,  //  00E3  GETMET	R27	R3	K27
      0x7C6C0200,  //  00E4  CALL	R27	1
      0x006C3605,  //  00E5  ADD	R27	R27	R5
      0x60700015,  //  00E6  GETGBL	R28	G21
      0x7C700000,  //  00E7  CALL	R28	0
      0x8C703916,  //  00E8  GETMET	R28	R28	K22
      0x88780137,  //  00E9  GETMBR	R30	R0	K55
      0x7C700400,  //  00EA  CALL	R28	2
      0x5476002F,  //  00EB  LDINT	R29	48
      0x7C600A00,  //  00EC  CALL	R24	5
      0x5466000E,  //  00ED  LDINT	R25	15
      0x40660619,  //  00EE  CONNECT	R25	K3	R25
      0x94643019,  //  00EF  GETIDX	R25	R24	R25
      0x546A000F,  //  00F0  LDINT	R26	16
      0x546E001E,  //  00F1  LDINT	R27	31
      0x4068341B,  //  00F2  CONNECT	R26	R26	R27
      0x9468301A,  //  00F3  GETIDX	R26	R24	R26
      0x546E001F,  //  00F4  LDINT	R27	32
      0x5472002E,  //  00F5  LDINT	R28	47
      0x406C361C,  //  00F6  CONNECT	R27	R27	R28
      0x946C301B,  //  00F7  GETIDX	R27	R24	R27
      0xB8727000,  //  00F8  GETNGBL	R28	K56
      0x8C703939,  //  00F9  GETMET	R28	R28	K57
      0x7C700200,  //  00FA  CALL	R28	1
      0x8C740108,  //  00FB  GETMET	R29	R0	K8
      0x5C7C0200,  //  00FC  MOVE	R31	R1
      0x58800003,  //  00FD  LDCONST	R32	K3
      0x58840003,  //  00FE  LDCONST	R33	K3
      0x58880003,  //  00FF  LDCONST	R34	K3
      0x508C0200,  //  0100  LDBOOL	R35	1	0
      0x7C740C00,  //  0101  CALL	R29	6
      0x8C74073A,  //  0102  GETMET	R29	R3	K58
      0x7C740200,  //  0103  CALL	R29	1
      0x8C74073B,  //  0104  GETMET	R29	R3	K59
      0x5C7C3200,  //  0105  MOVE	R31	R25
      0x5C803400,  //  0106  MOVE	R32	R26
      0x5C843600,  //  0107  MOVE	R33	R27
      0x5C883800,  //  0108  MOVE	R34	R28
      0x7C740A00,  //  0109  CALL	R29	5
      0x900E7903,  //  010A  SETMBR	R3	K60	K3
      0x8C74073D,  //  010B  GETMET	R29	R3	K61
      0x7C740200,  //  010C  CALL	R29	1
      0x8C74073E,  //  010D  GETMET	R29	R3	K62
      0x507C0200,  //  010E  LDBOOL	R31	1	0
      0x7C740400,  //  010F  CALL	R29	2
      0x8C74073F,  //  0110  GETMET	R29	R3	K63
      0x7C740200,  //  0111  CALL	R29	1
      0x8C740740,  //  0112  GETMET	R29	R3	K64
      0x7C740200,  //  0113  CALL	R29	1
      0x8C740741,  //  0114  GETMET	R29	R3	K65
      0x7C740200,  //  0115  CALL	R29	1
      0x50740200,  //  0116  LDBOOL	R29	1	0
      0x80043A00,  //  0117  RET	1	R29
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming
********************************************************************/
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_process_incoming,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(is_commissioning_open),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20commissioning_X20not_X20open),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(parse_PBKDFParamRequest),
    /* K7   */  be_nested_str_weak(parse_Pake1),
    /* K8   */  be_nested_str_weak(parse_Pake3),
    /* K9   */  be_nested_str_weak(parse_Sigma1),
    /* K10  */  be_nested_str_weak(parse_Sigma3),
    /* K11  */  be_nested_str_weak(parse_StatusReport),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28secure_X20channel_X29_X20_X2502X),
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[88]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A000D,  //  0003  JMPT	R2	#0012
      0x88080302,  //  0004  GETMBR	R2	R1	K2
      0x540E001F,  //  0005  LDINT	R3	32
      0x28080403,  //  0006  GE	R2	R2	R3
      0x780A0009,  //  0007  JMPF	R2	#0012
      0x88080302,  //  0008  GETMBR	R2	R1	K2
      0x540E0023,  //  0009  LDINT	R3	36
      0x18080403,  //  000A  LE	R2	R2	R3
      0x780A0005,  //  000B  JMPF	R2	#0012
      0xB80A0600,  //  000C  GETNGBL	R2	K3
      0x580C0004,  //  000D  LDCONST	R3	K4
      0x58100005,  //  000E  LDCONST	R4	K5
      0x7C080400,  //  000F  CALL	R2	2
      0x50080000,  //  0010  LDBOOL	R2	0	0
      0x80040400,  //  0011  RET	1	R2
      0x88080302,  //  0012  GETMBR	R2	R1	K2
      0x540E000F,  //  0013  LDINT	R3	16
      0x1C080403,  //  0014  EQ	R2	R2	R3
      0x780A0000,  //  0015  JMPF	R2	#0017
      0x7002003E,  //  0016  JMP		#0056
      0x88080302,  //  0017  GETMBR	R2	R1	K2
      0x540E001F,  //  0018  LDINT	R3	32
      0x1C080403,  //  0019  EQ	R2	R2	R3
      0x780A0004,  //  001A  JMPF	R2	#0020
      0x8C080106,  //  001B  GETMET	R2	R0	K6
      0x5C100200,  //  001C  MOVE	R4	R1
      0x7C080400,  //  001D  CALL	R2	2
      0x80040400,  //  001E  RET	1	R2
      0x70020035,  //  001F  JMP		#0056
      0x88080302,  //  0020  GETMBR	R2	R1	K2
      0x540E0021,  //  0021  LDINT	R3	34
      0x1C080403,  //  0022  EQ	R2	R2	R3
      0x780A0004,  //  0023  JMPF	R2	#0029
      0x8C080107,  //  0024  GETMET	R2	R0	K7
      0x5C100200,  //  0025  MOVE	R4	R1
      0x7C080400,  //  0026  CALL	R2	2
      0x80040400,  //  0027  RET	1	R2
      0x7002002C,  //  0028  JMP		#0056
      0x88080302,  //  0029  GETMBR	R2	R1	K2
      0x540E0023,  //  002A  LDINT	R3	36
      0x1C080403,  //  002B  EQ	R2	R2	R3
      0x780A0004,  //  002C  JMPF	R2	#0032
      0x8C080108,  //  002D  GETMET	R2	R0	K8
      0x5C100200,  //  002E  MOVE	R4	R1
      0x7C080400,  //  002F  CALL	R2	2
      0x80040400,  //  0030  RET	1	R2
      0x70020023,  //  0031  JMP		#0056
      0x88080302,  //  0032  GETMBR	R2	R1	K2
      0x540E002F,  //  0033  LDINT	R3	48
      0x1C080403,  //  0034  EQ	R2	R2	R3
      0x780A0004,  //  0035  JMPF	R2	#003B
      0x8C080109,  //  0036  GETMET	R2	R0	K9
      0x5C100200,  //  0037  MOVE	R4	R1
      0x7C080400,  //  0038  CALL	R2	2
      0x80040400,  //  0039  RET	1	R2
      0x7002001A,  //  003A  JMP		#0056
      0x88080302,  //  003B  GETMBR	R2	R1	K2
      0x540E0031,  //  003C  LDINT	R3	50
      0x1C080403,  //  003D  EQ	R2	R2	R3
      0x780A0004,  //  003E  JMPF	R2	#0044
      0x8C08010A,  //  003F  GETMET	R2	R0	K10
      0x5C100200,  //  0040  MOVE	R4	R1
      0x7C080400,  //  0041  CALL	R2	2
      0x80040400,  //  0042  RET	1	R2
      0x70020011,  //  0043  JMP		#0056
      0x88080302,  //  0044  GETMBR	R2	R1	K2
      0x540E003F,  //  0045  LDINT	R3	64
      0x1C080403,  //  0046  EQ	R2	R2	R3
      0x780A0004,  //  0047  JMPF	R2	#004D
      0x8C08010B,  //  0048  GETMET	R2	R0	K11
      0x5C100200,  //  0049  MOVE	R4	R1
      0x7C080400,  //  004A  CALL	R2	2
      0x80040400,  //  004B  RET	1	R2
      0x70020008,  //  004C  JMP		#0056
      0xB80A0600,  //  004D  GETNGBL	R2	K3
      0x600C0018,  //  004E  GETGBL	R3	G24
      0x5810000C,  //  004F  LDCONST	R4	K12
      0x88140302,  //  0050  GETMBR	R5	R1	K2
      0x7C0C0400,  //  0051  CALL	R3	2
      0x58100005,  //  0052  LDCONST	R4	K5
      0x7C080400,  //  0053  CALL	R2	2
      0x50080000,  //  0054  LDBOOL	R2	0	0
      0x80040400,  //  0055  RET	1	R2
      0x50080000,  //  0056  LDBOOL	R2	0	0
      0x80040400,  //  0057  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Sigma1
********************************************************************/
extern const bclass be_class_Matter_Commisioning_Context;
be_local_closure(class_Matter_Commisioning_Context_parse_Sigma1,   /* name */
  be_nested_proto(
    36,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Commisioning_Context, 
    1,                          /* has constants */
    ( &(const bvalue[103]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(send_status_report),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(matter),
    /* K13  */  be_nested_str_weak(Sigma1),
    /* K14  */  be_nested_str_weak(parse),
    /* K15  */  be_nested_str_weak(raw),
    /* K16  */  be_nested_str_weak(app_payload_idx),
    /* K17  */  be_nested_str_weak(__initiator_pub),
    /* K18  */  be_nested_str_weak(initiatorEphPubKey),
    /* K19  */  be_nested_str_weak(resumptionID),
    /* K20  */  be_nested_str_weak(initiatorResumeMIC),
    /* K21  */  be_nested_str_weak(device),
    /* K22  */  be_nested_str_weak(sessions),
    /* K23  */  be_nested_str_weak(find_session_by_resumption_id),
    /* K24  */  be_nested_str_weak(_fabric),
    /* K25  */  be_nested_str_weak(initiatorRandom),
    /* K26  */  be_nested_str_weak(fromstring),
    /* K27  */  be_nested_str_weak(Sigma1_Resume),
    /* K28  */  be_nested_str_weak(HKDF_SHA256),
    /* K29  */  be_nested_str_weak(derive),
    /* K30  */  be_nested_str_weak(shared_secret),
    /* K31  */  be_nested_str_weak(NCASE_SigmaS1),
    /* K32  */  be_const_int(2147483647),
    /* K33  */  be_nested_str_weak(AES_CCM),
    /* K34  */  be_nested_str_weak(decrypt),
    /* K35  */  be_nested_str_weak(tag),
    /* K36  */  be_nested_str_weak(_source_node_id),
    /* K37  */  be_nested_str_weak(source_node_id),
    /* K38  */  be_nested_str_weak(set_mode_CASE),
    /* K39  */  be_nested_str_weak(__future_initiator_session_id),
    /* K40  */  be_nested_str_weak(initiator_session_id),
    /* K41  */  be_nested_str_weak(__future_local_session_id),
    /* K42  */  be_nested_str_weak(gen_local_session_id),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K44  */  be_nested_str_weak(remote_ip),
    /* K45  */  be_nested_str_weak(remote_port),
    /* K46  */  be_nested_str_weak(resumption_id),
    /* K47  */  be_nested_str_weak(random),
    /* K48  */  be_nested_str_weak(Sigma2_Resume),
    /* K49  */  be_nested_str_weak(NCASE_SigmaS2),
    /* K50  */  be_nested_str_weak(Sigma2Resume),
    /* K51  */  be_nested_str_weak(responderSessionID),
    /* K52  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K53  */  be_nested_str_weak(SessionResumptionKeys),
    /* K54  */  be_nested_str_weak(tasmota),
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
    ( &(const binstruction[476]) {  /* code */
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
      0x7812000C,  //  000B  JMPF	R4	#0019
      0xB8120C00,  //  000C  GETNGBL	R4	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0x8C100109,  //  0010  GETMET	R4	R0	K9
      0x5C180200,  //  0011  MOVE	R6	R1
      0x581C000A,  //  0012  LDCONST	R7	K10
      0x58200004,  //  0013  LDCONST	R8	K4
      0x5824000B,  //  0014  LDCONST	R9	K11
      0x50280000,  //  0015  LDBOOL	R10	0	0
      0x7C100C00,  //  0016  CALL	R4	6
      0x50100000,  //  0017  LDBOOL	R4	0	0
      0x80040800,  //  0018  RET	1	R4
      0xB8121800,  //  0019  GETNGBL	R4	K12
      0x8C10090D,  //  001A  GETMET	R4	R4	K13
      0x7C100200,  //  001B  CALL	R4	1
      0x8C10090E,  //  001C  GETMET	R4	R4	K14
      0x8818030F,  //  001D  GETMBR	R6	R1	K15
      0x881C0310,  //  001E  GETMBR	R7	R1	K16
      0x7C100600,  //  001F  CALL	R4	3
      0x88140912,  //  0020  GETMBR	R5	R4	K18
      0x900E2205,  //  0021  SETMBR	R3	K17	R5
      0x88140913,  //  0022  GETMBR	R5	R4	K19
      0x4C180000,  //  0023  LDNIL	R6
      0x20140A06,  //  0024  NE	R5	R5	R6
      0x78160003,  //  0025  JMPF	R5	#002A
      0x88140914,  //  0026  GETMBR	R5	R4	K20
      0x4C180000,  //  0027  LDNIL	R6
      0x20140A06,  //  0028  NE	R5	R5	R6
      0x74160000,  //  0029  JMPT	R5	#002B
      0x50140001,  //  002A  LDBOOL	R5	0	1
      0x50140200,  //  002B  LDBOOL	R5	1	0
      0x50140000,  //  002C  LDBOOL	R5	0	0
      0x4C180000,  //  002D  LDNIL	R6
      0x7816000D,  //  002E  JMPF	R5	#003D
      0x881C0115,  //  002F  GETMBR	R7	R0	K21
      0x881C0F16,  //  0030  GETMBR	R7	R7	K22
      0x8C1C0F17,  //  0031  GETMET	R7	R7	K23
      0x88240913,  //  0032  GETMBR	R9	R4	K19
      0x7C1C0400,  //  0033  CALL	R7	2
      0x5C180E00,  //  0034  MOVE	R6	R7
      0x4C1C0000,  //  0035  LDNIL	R7
      0x1C1C0C07,  //  0036  EQ	R7	R6	R7
      0x741E0003,  //  0037  JMPT	R7	#003C
      0x881C0D18,  //  0038  GETMBR	R7	R6	K24
      0x4C200000,  //  0039  LDNIL	R8
      0x1C1C0E08,  //  003A  EQ	R7	R7	R8
      0x781E0000,  //  003B  JMPF	R7	#003D
      0x50140000,  //  003C  LDBOOL	R5	0	0
      0x781600B4,  //  003D  JMPF	R5	#00F3
      0x881C0919,  //  003E  GETMBR	R7	R4	K25
      0x88200913,  //  003F  GETMBR	R8	R4	K19
      0x001C0E08,  //  0040  ADD	R7	R7	R8
      0x60200015,  //  0041  GETGBL	R8	G21
      0x7C200000,  //  0042  CALL	R8	0
      0x8C20111A,  //  0043  GETMET	R8	R8	K26
      0x5828001B,  //  0044  LDCONST	R10	K27
      0x7C200400,  //  0045  CALL	R8	2
      0x8C24051C,  //  0046  GETMET	R9	R2	K28
      0x7C240200,  //  0047  CALL	R9	1
      0x8C24131D,  //  0048  GETMET	R9	R9	K29
      0x882C0D1E,  //  0049  GETMBR	R11	R6	K30
      0x5C300E00,  //  004A  MOVE	R12	R7
      0x5C341000,  //  004B  MOVE	R13	R8
      0x543A000F,  //  004C  LDINT	R14	16
      0x7C240A00,  //  004D  CALL	R9	5
      0x60280015,  //  004E  GETGBL	R10	G21
      0x7C280000,  //  004F  CALL	R10	0
      0x8C28151A,  //  0050  GETMET	R10	R10	K26
      0x5830001F,  //  0051  LDCONST	R12	K31
      0x7C280400,  //  0052  CALL	R10	2
      0x542DFFEE,  //  0053  LDINT	R11	-17
      0x402E080B,  //  0054  CONNECT	R11	K4	R11
      0x88300914,  //  0055  GETMBR	R12	R4	K20
      0x942C180B,  //  0056  GETIDX	R11	R12	R11
      0x5435FFEF,  //  0057  LDINT	R13	-16
      0x40341B20,  //  0058  CONNECT	R13	R13	K32
      0x88380914,  //  0059  GETMBR	R14	R4	K20
      0x94301C0D,  //  005A  GETIDX	R12	R14	R13
      0x8C3C0521,  //  005B  GETMET	R15	R2	K33
      0x5C441200,  //  005C  MOVE	R17	R9
      0x5C481400,  //  005D  MOVE	R18	R10
      0x604C0015,  //  005E  GETGBL	R19	G21
      0x7C4C0000,  //  005F  CALL	R19	0
      0x6050000C,  //  0060  GETGBL	R20	G12
      0x5C541600,  //  0061  MOVE	R21	R11
      0x7C500200,  //  0062  CALL	R20	1
      0x5456000F,  //  0063  LDINT	R21	16
      0x7C3C0C00,  //  0064  CALL	R15	6
      0x5C341E00,  //  0065  MOVE	R13	R15
      0x8C3C1B22,  //  0066  GETMET	R15	R13	K34
      0x5C441600,  //  0067  MOVE	R17	R11
      0x7C3C0400,  //  0068  CALL	R15	2
      0x5C381E00,  //  0069  MOVE	R14	R15
      0x8C3C1B23,  //  006A  GETMET	R15	R13	K35
      0x7C3C0200,  //  006B  CALL	R15	1
      0x1C40180F,  //  006C  EQ	R16	R12	R15
      0x78420083,  //  006D  JMPF	R16	#00F2
      0x88400D18,  //  006E  GETMBR	R16	R6	K24
      0x900E3010,  //  006F  SETMBR	R3	K24	R16
      0x88400325,  //  0070  GETMBR	R16	R1	K37
      0x900E4810,  //  0071  SETMBR	R3	K36	R16
      0x8C400726,  //  0072  GETMET	R16	R3	K38
      0x7C400200,  //  0073  CALL	R16	1
      0x88400928,  //  0074  GETMBR	R16	R4	K40
      0x900E4E10,  //  0075  SETMBR	R3	K39	R16
      0x88400115,  //  0076  GETMBR	R16	R0	K21
      0x88402116,  //  0077  GETMBR	R16	R16	K22
      0x8C40212A,  //  0078  GETMET	R16	R16	K42
      0x7C400200,  //  0079  CALL	R16	1
      0x900E5210,  //  007A  SETMBR	R3	K41	R16
      0xB8420C00,  //  007B  GETNGBL	R16	K6
      0x60440018,  //  007C  GETGBL	R17	G24
      0x5848002B,  //  007D  LDCONST	R18	K43
      0x884C0729,  //  007E  GETMBR	R19	R3	K41
      0x8850032C,  //  007F  GETMBR	R20	R1	K44
      0x8854032D,  //  0080  GETMBR	R21	R1	K45
      0x7C440800,  //  0081  CALL	R17	4
      0x58480008,  //  0082  LDCONST	R18	K8
      0x7C400400,  //  0083  CALL	R16	2
      0x88400D1E,  //  0084  GETMBR	R16	R6	K30
      0x900E3C10,  //  0085  SETMBR	R3	K30	R16
      0x8C40052F,  //  0086  GETMET	R16	R2	K47
      0x544A000F,  //  0087  LDINT	R18	16
      0x7C400400,  //  0088  CALL	R16	2
      0x900E5C10,  //  0089  SETMBR	R3	K46	R16
      0x60400015,  //  008A  GETGBL	R16	G21
      0x7C400000,  //  008B  CALL	R16	0
      0x8C40211A,  //  008C  GETMET	R16	R16	K26
      0x58480030,  //  008D  LDCONST	R18	K48
      0x7C400400,  //  008E  CALL	R16	2
      0x88440919,  //  008F  GETMBR	R17	R4	K25
      0x8848072E,  //  0090  GETMBR	R18	R3	K46
      0x00442212,  //  0091  ADD	R17	R17	R18
      0x8C48051C,  //  0092  GETMET	R18	R2	K28
      0x7C480200,  //  0093  CALL	R18	1
      0x8C48251D,  //  0094  GETMET	R18	R18	K29
      0x8850071E,  //  0095  GETMBR	R20	R3	K30
      0x5C542200,  //  0096  MOVE	R21	R17
      0x5C582000,  //  0097  MOVE	R22	R16
      0x545E000F,  //  0098  LDINT	R23	16
      0x7C480A00,  //  0099  CALL	R18	5
      0x8C4C0521,  //  009A  GETMET	R19	R2	K33
      0x5C542400,  //  009B  MOVE	R21	R18
      0x60580015,  //  009C  GETGBL	R22	G21
      0x7C580000,  //  009D  CALL	R22	0
      0x8C582D1A,  //  009E  GETMET	R22	R22	K26
      0x58600031,  //  009F  LDCONST	R24	K49
      0x7C580400,  //  00A0  CALL	R22	2
      0x605C0015,  //  00A1  GETGBL	R23	G21
      0x7C5C0000,  //  00A2  CALL	R23	0
      0x58600004,  //  00A3  LDCONST	R24	K4
      0x5466000F,  //  00A4  LDINT	R25	16
      0x7C4C0C00,  //  00A5  CALL	R19	6
      0x8C502723,  //  00A6  GETMET	R20	R19	K35
      0x7C500200,  //  00A7  CALL	R20	1
      0xB8561800,  //  00A8  GETNGBL	R21	K12
      0x8C542B32,  //  00A9  GETMET	R21	R21	K50
      0x7C540200,  //  00AA  CALL	R21	1
      0x8858072E,  //  00AB  GETMBR	R22	R3	K46
      0x90562616,  //  00AC  SETMBR	R21	K19	R22
      0x88580729,  //  00AD  GETMBR	R22	R3	K41
      0x90566616,  //  00AE  SETMBR	R21	K51	R22
      0x90566814,  //  00AF  SETMBR	R21	K52	R20
      0x8C58051C,  //  00B0  GETMET	R22	R2	K28
      0x7C580200,  //  00B1  CALL	R22	1
      0x8C582D1D,  //  00B2  GETMET	R22	R22	K29
      0x8860071E,  //  00B3  GETMBR	R24	R3	K30
      0x88640919,  //  00B4  GETMBR	R25	R4	K25
      0x8868072E,  //  00B5  GETMBR	R26	R3	K46
      0x0064321A,  //  00B6  ADD	R25	R25	R26
      0x60680015,  //  00B7  GETGBL	R26	G21
      0x7C680000,  //  00B8  CALL	R26	0
      0x8C68351A,  //  00B9  GETMET	R26	R26	K26
      0x58700035,  //  00BA  LDCONST	R28	K53
      0x7C680400,  //  00BB  CALL	R26	2
      0x546E002F,  //  00BC  LDINT	R27	48
      0x7C580A00,  //  00BD  CALL	R22	5
      0x545E000E,  //  00BE  LDINT	R23	15
      0x405E0817,  //  00BF  CONNECT	R23	K4	R23
      0x945C2C17,  //  00C0  GETIDX	R23	R22	R23
      0x5462000F,  //  00C1  LDINT	R24	16
      0x5466001E,  //  00C2  LDINT	R25	31
      0x40603019,  //  00C3  CONNECT	R24	R24	R25
      0x94602C18,  //  00C4  GETIDX	R24	R22	R24
      0x5466001F,  //  00C5  LDINT	R25	32
      0x546A002E,  //  00C6  LDINT	R26	47
      0x4064321A,  //  00C7  CONNECT	R25	R25	R26
      0x94642C19,  //  00C8  GETIDX	R25	R22	R25
      0xB86A6C00,  //  00C9  GETNGBL	R26	K54
      0x8C683537,  //  00CA  GETMET	R26	R26	K55
      0x7C680200,  //  00CB  CALL	R26	1
      0x8C6C2B38,  //  00CC  GETMET	R27	R21	K56
      0x7C6C0200,  //  00CD  CALL	R27	1
      0x4C700000,  //  00CE  LDNIL	R28
      0x900E721C,  //  00CF  SETMBR	R3	K57	R28
      0x8C70033A,  //  00D0  GETMET	R28	R1	K58
      0x547A0032,  //  00D1  LDINT	R30	51
      0x507C0200,  //  00D2  LDBOOL	R31	1	0
      0x7C700600,  //  00D3  CALL	R28	3
      0x8C74393B,  //  00D4  GETMET	R29	R28	K59
      0x5C7C3600,  //  00D5  MOVE	R31	R27
      0x7C740400,  //  00D6  CALL	R29	2
      0x8878013C,  //  00D7  GETMBR	R30	R0	K60
      0x8C783D3D,  //  00D8  GETMET	R30	R30	K61
      0x5C803800,  //  00D9  MOVE	R32	R28
      0x7C780400,  //  00DA  CALL	R30	2
      0x8C78073E,  //  00DB  GETMET	R30	R3	K62
      0x7C780200,  //  00DC  CALL	R30	1
      0x8C78073F,  //  00DD  GETMET	R30	R3	K63
      0x5C802E00,  //  00DE  MOVE	R32	R23
      0x5C843000,  //  00DF  MOVE	R33	R24
      0x5C883200,  //  00E0  MOVE	R34	R25
      0x5C8C3400,  //  00E1  MOVE	R35	R26
      0x7C780A00,  //  00E2  CALL	R30	5
      0x900E8104,  //  00E3  SETMBR	R3	K64	K4
      0x8C780741,  //  00E4  GETMET	R30	R3	K65
      0x7C780200,  //  00E5  CALL	R30	1
      0x8C780742,  //  00E6  GETMET	R30	R3	K66
      0x50800200,  //  00E7  LDBOOL	R32	1	0
      0x7C780400,  //  00E8  CALL	R30	2
      0x8C780743,  //  00E9  GETMET	R30	R3	K67
      0x7C780200,  //  00EA  CALL	R30	1
      0x8C780744,  //  00EB  GETMET	R30	R3	K68
      0x7C780200,  //  00EC  CALL	R30	1
      0x8C780745,  //  00ED  GETMET	R30	R3	K69
      0x7C780200,  //  00EE  CALL	R30	1
      0x50780200,  //  00EF  LDBOOL	R30	1	0
      0x80043C00,  //  00F0  RET	1	R30
      0x70020000,  //  00F1  JMP		#00F3
      0x50140000,  //  00F2  LDBOOL	R5	0	0
      0x5C1C0A00,  //  00F3  MOVE	R7	R5
      0x741E00E4,  //  00F4  JMPT	R7	#01DA
      0x8C1C0146,  //  00F5  GETMET	R7	R0	K70
      0x88240947,  //  00F6  GETMBR	R9	R4	K71
      0x88280919,  //  00F7  GETMBR	R10	R4	K25
      0x7C1C0600,  //  00F8  CALL	R7	3
      0x900E3007,  //  00F9  SETMBR	R3	K24	R7
      0x4C200000,  //  00FA  LDNIL	R8
      0x1C200608,  //  00FB  EQ	R8	R3	R8
      0x74220003,  //  00FC  JMPT	R8	#0101
      0x88200718,  //  00FD  GETMBR	R8	R3	K24
      0x4C240000,  //  00FE  LDNIL	R9
      0x1C201009,  //  00FF  EQ	R8	R8	R9
      0x7822000C,  //  0100  JMPF	R8	#010E
      0xB8220C00,  //  0101  GETNGBL	R8	K6
      0x58240048,  //  0102  LDCONST	R9	K72
      0x58280008,  //  0103  LDCONST	R10	K8
      0x7C200400,  //  0104  CALL	R8	2
      0x8C200109,  //  0105  GETMET	R8	R0	K9
      0x5C280200,  //  0106  MOVE	R10	R1
      0x582C000A,  //  0107  LDCONST	R11	K10
      0x58300004,  //  0108  LDCONST	R12	K4
      0x5834000A,  //  0109  LDCONST	R13	K10
      0x50380000,  //  010A  LDBOOL	R14	0	0
      0x7C200C00,  //  010B  CALL	R8	6
      0x50200000,  //  010C  LDBOOL	R8	0	0
      0x80041000,  //  010D  RET	1	R8
      0x88200325,  //  010E  GETMBR	R8	R1	K37
      0x900E4808,  //  010F  SETMBR	R3	K36	R8
      0x8C200726,  //  0110  GETMET	R8	R3	K38
      0x7C200200,  //  0111  CALL	R8	1
      0x88200928,  //  0112  GETMBR	R8	R4	K40
      0x900E4E08,  //  0113  SETMBR	R3	K39	R8
      0x88200115,  //  0114  GETMBR	R8	R0	K21
      0x88201116,  //  0115  GETMBR	R8	R8	K22
      0x8C20112A,  //  0116  GETMET	R8	R8	K42
      0x7C200200,  //  0117  CALL	R8	1
      0x900E5208,  //  0118  SETMBR	R3	K41	R8
      0xB8220C00,  //  0119  GETNGBL	R8	K6
      0x60240018,  //  011A  GETGBL	R9	G24
      0x5828002B,  //  011B  LDCONST	R10	K43
      0x882C0729,  //  011C  GETMBR	R11	R3	K41
      0x8830032C,  //  011D  GETMBR	R12	R1	K44
      0x8834032D,  //  011E  GETMBR	R13	R1	K45
      0x7C240800,  //  011F  CALL	R9	4
      0x58280008,  //  0120  LDCONST	R10	K8
      0x7C200400,  //  0121  CALL	R8	2
      0x8C20052F,  //  0122  GETMET	R8	R2	K47
      0x542A000F,  //  0123  LDINT	R10	16
      0x7C200400,  //  0124  CALL	R8	2
      0x900E5C08,  //  0125  SETMBR	R3	K46	R8
      0x8C20052F,  //  0126  GETMET	R8	R2	K47
      0x542A001F,  //  0127  LDINT	R10	32
      0x7C200400,  //  0128  CALL	R8	2
      0x900E9208,  //  0129  SETMBR	R3	K73	R8
      0x8C20054B,  //  012A  GETMET	R8	R2	K75
      0x7C200200,  //  012B  CALL	R8	1
      0x8C20114C,  //  012C  GETMET	R8	R8	K76
      0x88280749,  //  012D  GETMBR	R10	R3	K73
      0x7C200400,  //  012E  CALL	R8	2
      0x900E9408,  //  012F  SETMBR	R3	K74	R8
      0x8C20052F,  //  0130  GETMET	R8	R2	K47
      0x542A001F,  //  0131  LDINT	R10	32
      0x7C200400,  //  0132  CALL	R8	2
      0x8C24054B,  //  0133  GETMET	R9	R2	K75
      0x7C240200,  //  0134  CALL	R9	1
      0x8C24134D,  //  0135  GETMET	R9	R9	K77
      0x882C0749,  //  0136  GETMBR	R11	R3	K73
      0x88300912,  //  0137  GETMBR	R12	R4	K18
      0x7C240600,  //  0138  CALL	R9	3
      0x900E3C09,  //  0139  SETMBR	R3	K30	R9
      0xB8261800,  //  013A  GETNGBL	R9	K12
      0x8824134E,  //  013B  GETMBR	R9	R9	K78
      0x8C24134F,  //  013C  GETMET	R9	R9	K79
      0x7C240200,  //  013D  CALL	R9	1
      0x8C281350,  //  013E  GETMET	R10	R9	K80
      0x5830000A,  //  013F  LDCONST	R12	K10
      0xB8361800,  //  0140  GETNGBL	R13	K12
      0x88341B4E,  //  0141  GETMBR	R13	R13	K78
      0x88341B51,  //  0142  GETMBR	R13	R13	K81
      0x8C380F52,  //  0143  GETMET	R14	R7	K82
      0x7C380200,  //  0144  CALL	R14	1
      0x7C280800,  //  0145  CALL	R10	4
      0x8C281350,  //  0146  GETMET	R10	R9	K80
      0x5830000B,  //  0147  LDCONST	R12	K11
      0xB8361800,  //  0148  GETNGBL	R13	K12
      0x88341B4E,  //  0149  GETMBR	R13	R13	K78
      0x88341B51,  //  014A  GETMBR	R13	R13	K81
      0x8C380F53,  //  014B  GETMET	R14	R7	K83
      0x7C380200,  //  014C  CALL	R14	1
      0x7C280800,  //  014D  CALL	R10	4
      0x8C281350,  //  014E  GETMET	R10	R9	K80
      0x58300008,  //  014F  LDCONST	R12	K8
      0xB8361800,  //  0150  GETNGBL	R13	K12
      0x88341B4E,  //  0151  GETMBR	R13	R13	K78
      0x88341B51,  //  0152  GETMBR	R13	R13	K81
      0x8838074A,  //  0153  GETMBR	R14	R3	K74
      0x7C280800,  //  0154  CALL	R10	4
      0x8C281350,  //  0155  GETMET	R10	R9	K80
      0x54320003,  //  0156  LDINT	R12	4
      0xB8361800,  //  0157  GETNGBL	R13	K12
      0x88341B4E,  //  0158  GETMBR	R13	R13	K78
      0x88341B51,  //  0159  GETMBR	R13	R13	K81
      0x88380912,  //  015A  GETMBR	R14	R4	K18
      0x7C280800,  //  015B  CALL	R10	4
      0x8C28054B,  //  015C  GETMET	R10	R2	K75
      0x7C280200,  //  015D  CALL	R10	1
      0x8C281554,  //  015E  GETMET	R10	R10	K84
      0x8C300F55,  //  015F  GETMET	R12	R7	K85
      0x7C300200,  //  0160  CALL	R12	1
      0x8C341338,  //  0161  GETMET	R13	R9	K56
      0x7C340200,  //  0162  CALL	R13	1
      0x7C280600,  //  0163  CALL	R10	3
      0xB82E1800,  //  0164  GETNGBL	R11	K12
      0x882C174E,  //  0165  GETMBR	R11	R11	K78
      0x8C2C174F,  //  0166  GETMET	R11	R11	K79
      0x7C2C0200,  //  0167  CALL	R11	1
      0x8C301750,  //  0168  GETMET	R12	R11	K80
      0x5838000A,  //  0169  LDCONST	R14	K10
      0xB83E1800,  //  016A  GETNGBL	R15	K12
      0x883C1F4E,  //  016B  GETMBR	R15	R15	K78
      0x883C1F51,  //  016C  GETMBR	R15	R15	K81
      0x8C400F52,  //  016D  GETMET	R16	R7	K82
      0x7C400200,  //  016E  CALL	R16	1
      0x7C300800,  //  016F  CALL	R12	4
      0x8C301750,  //  0170  GETMET	R12	R11	K80
      0x5838000B,  //  0171  LDCONST	R14	K11
      0xB83E1800,  //  0172  GETNGBL	R15	K12
      0x883C1F4E,  //  0173  GETMBR	R15	R15	K78
      0x883C1F51,  //  0174  GETMBR	R15	R15	K81
      0x8C400F53,  //  0175  GETMET	R16	R7	K83
      0x7C400200,  //  0176  CALL	R16	1
      0x7C300800,  //  0177  CALL	R12	4
      0x8C301750,  //  0178  GETMET	R12	R11	K80
      0x58380008,  //  0179  LDCONST	R14	K8
      0xB83E1800,  //  017A  GETNGBL	R15	K12
      0x883C1F4E,  //  017B  GETMBR	R15	R15	K78
      0x883C1F51,  //  017C  GETMBR	R15	R15	K81
      0x5C401400,  //  017D  MOVE	R16	R10
      0x7C300800,  //  017E  CALL	R12	4
      0x8C301750,  //  017F  GETMET	R12	R11	K80
      0x543A0003,  //  0180  LDINT	R14	4
      0xB83E1800,  //  0181  GETNGBL	R15	K12
      0x883C1F4E,  //  0182  GETMBR	R15	R15	K78
      0x883C1F51,  //  0183  GETMBR	R15	R15	K81
      0x8840072E,  //  0184  GETMBR	R16	R3	K46
      0x7C300800,  //  0185  CALL	R12	4
      0x88300956,  //  0186  GETMBR	R12	R4	K86
      0x900E720C,  //  0187  SETMBR	R3	K57	R12
      0x8C300557,  //  0188  GETMET	R12	R2	K87
      0x7C300200,  //  0189  CALL	R12	1
      0x8C301958,  //  018A  GETMET	R12	R12	K88
      0x88380739,  //  018B  GETMBR	R14	R3	K57
      0x7C300400,  //  018C  CALL	R12	2
      0x8C301959,  //  018D  GETMET	R12	R12	K89
      0x7C300200,  //  018E  CALL	R12	1
      0x60340015,  //  018F  GETGBL	R13	G21
      0x7C340000,  //  0190  CALL	R13	0
      0x8C341B1A,  //  0191  GETMET	R13	R13	K26
      0x883C015A,  //  0192  GETMBR	R15	R0	K90
      0x7C340400,  //  0193  CALL	R13	2
      0x8C380F5B,  //  0194  GETMET	R14	R7	K91
      0x7C380200,  //  0195  CALL	R14	1
      0x00381C08,  //  0196  ADD	R14	R14	R8
      0x883C074A,  //  0197  GETMBR	R15	R3	K74
      0x00381C0F,  //  0198  ADD	R14	R14	R15
      0x00381C0C,  //  0199  ADD	R14	R14	R12
      0x8C3C051C,  //  019A  GETMET	R15	R2	K28
      0x7C3C0200,  //  019B  CALL	R15	1
      0x8C3C1F1D,  //  019C  GETMET	R15	R15	K29
      0x8844071E,  //  019D  GETMBR	R17	R3	K30
      0x5C481C00,  //  019E  MOVE	R18	R14
      0x5C4C1A00,  //  019F  MOVE	R19	R13
      0x5452000F,  //  01A0  LDINT	R20	16
      0x7C3C0A00,  //  01A1  CALL	R15	5
      0x8C401738,  //  01A2  GETMET	R16	R11	K56
      0x7C400200,  //  01A3  CALL	R16	1
      0x8C440521,  //  01A4  GETMET	R17	R2	K33
      0x5C4C1E00,  //  01A5  MOVE	R19	R15
      0x60500015,  //  01A6  GETGBL	R20	G21
      0x7C500000,  //  01A7  CALL	R20	0
      0x8C50291A,  //  01A8  GETMET	R20	R20	K26
      0x8858015C,  //  01A9  GETMBR	R22	R0	K92
      0x7C500400,  //  01AA  CALL	R20	2
      0x60540015,  //  01AB  GETGBL	R21	G21
      0x7C540000,  //  01AC  CALL	R21	0
      0x6058000C,  //  01AD  GETGBL	R22	G12
      0x5C5C2000,  //  01AE  MOVE	R23	R16
      0x7C580200,  //  01AF  CALL	R22	1
      0x545E000F,  //  01B0  LDINT	R23	16
      0x7C440C00,  //  01B1  CALL	R17	6
      0x8C48235D,  //  01B2  GETMET	R18	R17	K93
      0x5C502000,  //  01B3  MOVE	R20	R16
      0x7C480400,  //  01B4  CALL	R18	2
      0x8C4C2323,  //  01B5  GETMET	R19	R17	K35
      0x7C4C0200,  //  01B6  CALL	R19	1
      0x00482413,  //  01B7  ADD	R18	R18	R19
      0xB84E1800,  //  01B8  GETNGBL	R19	K12
      0x8C4C275E,  //  01B9  GETMET	R19	R19	K94
      0x7C4C0200,  //  01BA  CALL	R19	1
      0x904EBE08,  //  01BB  SETMBR	R19	K95	R8
      0x88500729,  //  01BC  GETMBR	R20	R3	K41
      0x904EC014,  //  01BD  SETMBR	R19	K96	R20
      0x8850074A,  //  01BE  GETMBR	R20	R3	K74
      0x904EC214,  //  01BF  SETMBR	R19	K97	R20
      0x904EC412,  //  01C0  SETMBR	R19	K98	R18
      0x8C502738,  //  01C1  GETMET	R20	R19	K56
      0x7C500200,  //  01C2  CALL	R20	1
      0x900EC614,  //  01C3  SETMBR	R3	K99	R20
      0x8C54033A,  //  01C4  GETMET	R21	R1	K58
      0x545E0030,  //  01C5  LDINT	R23	49
      0x50600200,  //  01C6  LDBOOL	R24	1	0
      0x7C540600,  //  01C7  CALL	R21	3
      0x8C582B3B,  //  01C8  GETMET	R22	R21	K59
      0x5C602800,  //  01C9  MOVE	R24	R20
      0x7C580400,  //  01CA  CALL	R22	2
      0xB85E0C00,  //  01CB  GETNGBL	R23	K6
      0x60600018,  //  01CC  GETGBL	R24	G24
      0x58640064,  //  01CD  LDCONST	R25	K100
      0x88680729,  //  01CE  GETMBR	R26	R3	K41
      0x886C0765,  //  01CF  GETMBR	R27	R3	K101
      0x88700766,  //  01D0  GETMBR	R28	R3	K102
      0x7C600800,  //  01D1  CALL	R24	4
      0x5864000B,  //  01D2  LDCONST	R25	K11
      0x7C5C0400,  //  01D3  CALL	R23	2
      0x885C013C,  //  01D4  GETMBR	R23	R0	K60
      0x8C5C2F3D,  //  01D5  GETMET	R23	R23	K61
      0x5C642A00,  //  01D6  MOVE	R25	R21
      0x7C5C0400,  //  01D7  CALL	R23	2
      0x505C0200,  //  01D8  LDBOOL	R23	1	0
      0x80042E00,  //  01D9  RET	1	R23
      0x501C0200,  //  01DA  LDBOOL	R7	1	0
      0x80040E00,  //  01DB  RET	1	R7
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
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_Commisioning_Context_every_second_closure) },
        { be_const_key_weak(parse_StatusReport, -1), be_const_closure(class_Matter_Commisioning_Context_parse_StatusReport_closure) },
        { be_const_key_weak(S2K_Info, -1), be_nested_str_weak(Sigma2) },
        { be_const_key_weak(find_fabric_by_destination_id, -1), be_const_closure(class_Matter_Commisioning_Context_find_fabric_by_destination_id_closure) },
        { be_const_key_weak(process_incoming, 18), be_const_closure(class_Matter_Commisioning_Context_process_incoming_closure) },
        { be_const_key_weak(Matter_Context_Prefix, 0), be_nested_str_weak(CHIP_X20PAKE_X20V1_X20Commissioning) },
        { be_const_key_weak(SEKeys_Info, -1), be_nested_str_weak(SessionKeys) },
        { be_const_key_weak(TBEData3_Nonce, 4), be_nested_str_weak(NCASE_Sigma3N) },
        { be_const_key_weak(parse_PBKDFParamRequest, -1), be_const_closure(class_Matter_Commisioning_Context_parse_PBKDFParamRequest_closure) },
        { be_const_key_weak(send_status_report, -1), be_const_closure(class_Matter_Commisioning_Context_send_status_report_closure) },
        { be_const_key_weak(parse_Sigma3, -1), be_const_closure(class_Matter_Commisioning_Context_parse_Sigma3_closure) },
        { be_const_key_weak(device, -1), be_const_var(1) },
        { be_const_key_weak(TBEData2_Nonce, -1), be_nested_str_weak(NCASE_Sigma2N) },
        { be_const_key_weak(responder, 16), be_const_var(0) },
        { be_const_key_weak(parse_Pake3, -1), be_const_closure(class_Matter_Commisioning_Context_parse_Pake3_closure) },
        { be_const_key_weak(init, 14), be_const_closure(class_Matter_Commisioning_Context_init_closure) },
        { be_const_key_weak(parse_Pake1, -1), be_const_closure(class_Matter_Commisioning_Context_parse_Pake1_closure) },
        { be_const_key_weak(S3K_Info, 10), be_nested_str_weak(Sigma3) },
        { be_const_key_weak(add_session, -1), be_const_closure(class_Matter_Commisioning_Context_add_session_closure) },
        { be_const_key_weak(parse_Sigma1, -1), be_const_closure(class_Matter_Commisioning_Context_parse_Sigma1_closure) },
    })),
    be_str_weak(Matter_Commisioning_Context)
);
/********************************************************************/
/* End of solidification */
