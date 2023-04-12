/* Solidification of Matter_Commissioning.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Commisioning_Context;

/********************************************************************
** Solidified function: parse_PBKDFParamRequest
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_PBKDFParamRequest,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[51]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20invalid_X20PBKDFParamRequest_X20message),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K11  */  be_nested_str_weak(send_status_report),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(PBKDFParamRequest),
    /* K15  */  be_nested_str_weak(parse),
    /* K16  */  be_nested_str_weak(raw),
    /* K17  */  be_nested_str_weak(app_payload_idx),
    /* K18  */  be_nested_str_weak(session),
    /* K19  */  be_nested_str_weak(set_mode_PASE),
    /* K20  */  be_const_int(2147483647),
    /* K21  */  be_nested_str_weak(passcodeId),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20non_X2Dzero_X20passcode_X20id),
    /* K23  */  be_nested_str_weak(future_initiator_session_id),
    /* K24  */  be_nested_str_weak(initiator_session_id),
    /* K25  */  be_nested_str_weak(future_local_session_id),
    /* K26  */  be_nested_str_weak(device),
    /* K27  */  be_nested_str_weak(sessions),
    /* K28  */  be_nested_str_weak(gen_local_session_id),
    /* K29  */  be_nested_str_weak(format),
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
    /* K42  */  be_nested_str_weak(MTR_X3A_X20pbkdfparamresp_X3A_X20),
    /* K43  */  be_nested_str_weak(inspect),
    /* K44  */  be_nested_str_weak(tlv2raw),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20pbkdfparamresp_raw_X3A_X20),
    /* K46  */  be_nested_str_weak(tohex),
    /* K47  */  be_nested_str_weak(build_response),
    /* K48  */  be_nested_str_weak(encode_frame),
    /* K49  */  be_nested_str_weak(responder),
    /* K50  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[136]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
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
      0x88140312,  //  0026  GETMBR	R5	R1	K18
      0x8C140B13,  //  0027  GETMET	R5	R5	K19
      0x7C140200,  //  0028  CALL	R5	1
      0x88140311,  //  0029  GETMBR	R5	R1	K17
      0x40140B14,  //  002A  CONNECT	R5	R5	K20
      0x88180310,  //  002B  GETMBR	R6	R1	K16
      0x94140C05,  //  002C  GETIDX	R5	R6	R5
      0x90021C05,  //  002D  SETMBR	R0	K14	R5
      0x88140915,  //  002E  GETMBR	R5	R4	K21
      0x20140B04,  //  002F  NE	R5	R5	K4
      0x78160012,  //  0030  JMPF	R5	#0044
      0xB8160C00,  //  0031  GETNGBL	R5	K6
      0x8C140B07,  //  0032  GETMET	R5	R5	K7
      0x581C0016,  //  0033  LDCONST	R7	K22
      0x58200009,  //  0034  LDCONST	R8	K9
      0x7C140600,  //  0035  CALL	R5	3
      0xB8160C00,  //  0036  GETNGBL	R5	K6
      0x8C140B07,  //  0037  GETMET	R5	R5	K7
      0x581C000A,  //  0038  LDCONST	R7	K10
      0x58200009,  //  0039  LDCONST	R8	K9
      0x7C140600,  //  003A  CALL	R5	3
      0x8C14010B,  //  003B  GETMET	R5	R0	K11
      0x5C1C0200,  //  003C  MOVE	R7	R1
      0x5820000C,  //  003D  LDCONST	R8	K12
      0x58240004,  //  003E  LDCONST	R9	K4
      0x58280009,  //  003F  LDCONST	R10	K9
      0x502C0000,  //  0040  LDBOOL	R11	0	0
      0x7C140C00,  //  0041  CALL	R5	6
      0x50180000,  //  0042  LDBOOL	R6	0	0
      0x80040C00,  //  0043  RET	1	R6
      0x88140918,  //  0044  GETMBR	R5	R4	K24
      0x90022E05,  //  0045  SETMBR	R0	K23	R5
      0x8814011A,  //  0046  GETMBR	R5	R0	K26
      0x88140B1B,  //  0047  GETMBR	R5	R5	K27
      0x8C140B1C,  //  0048  GETMET	R5	R5	K28
      0x7C140200,  //  0049  CALL	R5	1
      0x90023205,  //  004A  SETMBR	R0	K25	R5
      0xB8160C00,  //  004B  GETNGBL	R5	K6
      0x8C140B07,  //  004C  GETMET	R5	R5	K7
      0x8C1C071D,  //  004D  GETMET	R7	R3	K29
      0x5824001E,  //  004E  LDCONST	R9	K30
      0x88280119,  //  004F  GETMBR	R10	R0	K25
      0x882C031F,  //  0050  GETMBR	R11	R1	K31
      0x88300320,  //  0051  GETMBR	R12	R1	K32
      0x7C1C0A00,  //  0052  CALL	R7	5
      0x58200009,  //  0053  LDCONST	R8	K9
      0x7C140600,  //  0054  CALL	R5	3
      0xB8161A00,  //  0055  GETNGBL	R5	K13
      0x8C140B21,  //  0056  GETMET	R5	R5	K33
      0x7C140200,  //  0057  CALL	R5	1
      0x88180922,  //  0058  GETMBR	R6	R4	K34
      0x90164406,  //  0059  SETMBR	R5	K34	R6
      0x8C180524,  //  005A  GETMET	R6	R2	K36
      0x5422001F,  //  005B  LDINT	R8	32
      0x7C180400,  //  005C  CALL	R6	2
      0x90164606,  //  005D  SETMBR	R5	K35	R6
      0x88180119,  //  005E  GETMBR	R6	R0	K25
      0x90164A06,  //  005F  SETMBR	R5	K37	R6
      0x8818011A,  //  0060  GETMBR	R6	R0	K26
      0x88180D27,  //  0061  GETMBR	R6	R6	K39
      0x90164C06,  //  0062  SETMBR	R5	K38	R6
      0x8818011A,  //  0063  GETMBR	R6	R0	K26
      0x88180D29,  //  0064  GETMBR	R6	R6	K41
      0x90165006,  //  0065  SETMBR	R5	K40	R6
      0xB81A0C00,  //  0066  GETNGBL	R6	K6
      0x8C180D07,  //  0067  GETMET	R6	R6	K7
      0x60200008,  //  0068  GETGBL	R8	G8
      0xB8261A00,  //  0069  GETNGBL	R9	K13
      0x8C24132B,  //  006A  GETMET	R9	R9	K43
      0x5C2C0A00,  //  006B  MOVE	R11	R5
      0x7C240400,  //  006C  CALL	R9	2
      0x7C200200,  //  006D  CALL	R8	1
      0x00225408,  //  006E  ADD	R8	K42	R8
      0x54260003,  //  006F  LDINT	R9	4
      0x7C180600,  //  0070  CALL	R6	3
      0x8C180B2C,  //  0071  GETMET	R6	R5	K44
      0x7C180200,  //  0072  CALL	R6	1
      0xB81E0C00,  //  0073  GETNGBL	R7	K6
      0x8C1C0F07,  //  0074  GETMET	R7	R7	K7
      0x8C240D2E,  //  0075  GETMET	R9	R6	K46
      0x7C240200,  //  0076  CALL	R9	1
      0x00265A09,  //  0077  ADD	R9	K45	R9
      0x542A0003,  //  0078  LDINT	R10	4
      0x7C1C0600,  //  0079  CALL	R7	3
      0x90024206,  //  007A  SETMBR	R0	K33	R6
      0x8C1C032F,  //  007B  GETMET	R7	R1	K47
      0x54260020,  //  007C  LDINT	R9	33
      0x50280200,  //  007D  LDBOOL	R10	1	0
      0x7C1C0600,  //  007E  CALL	R7	3
      0x8C200F30,  //  007F  GETMET	R8	R7	K48
      0x5C280C00,  //  0080  MOVE	R10	R6
      0x7C200400,  //  0081  CALL	R8	2
      0x88240131,  //  0082  GETMBR	R9	R0	K49
      0x8C241332,  //  0083  GETMET	R9	R9	K50
      0x5C2C0E00,  //  0084  MOVE	R11	R7
      0x7C240400,  //  0085  CALL	R9	2
      0x50240200,  //  0086  LDBOOL	R9	1	0
      0x80041200,  //  0087  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Pake1
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Pake1,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[44]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake1_X20message),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K10  */  be_nested_str_weak(send_status_report),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(matter),
    /* K13  */  be_nested_str_weak(Pake1),
    /* K14  */  be_nested_str_weak(parse),
    /* K15  */  be_nested_str_weak(raw),
    /* K16  */  be_nested_str_weak(app_payload_idx),
    /* K17  */  be_nested_str_weak(pA),
    /* K18  */  be_nested_str_weak(spake),
    /* K19  */  be_nested_str_weak(SPAKE2P_Matter),
    /* K20  */  be_nested_str_weak(device),
    /* K21  */  be_nested_str_weak(commissioning_w0),
    /* K22  */  be_nested_str_weak(commissioning_L),
    /* K23  */  be_nested_str_weak(compute_pB),
    /* K24  */  be_nested_str_weak(y),
    /* K25  */  be_nested_str_weak(pB),
    /* K26  */  be_nested_str_weak(compute_ZV_verifier),
    /* K27  */  be_nested_str_weak(SHA256),
    /* K28  */  be_nested_str_weak(update),
    /* K29  */  be_nested_str_weak(fromstring),
    /* K30  */  be_nested_str_weak(Matter_Context_Prefix),
    /* K31  */  be_nested_str_weak(PBKDFParamRequest),
    /* K32  */  be_nested_str_weak(PBKDFParamResponse),
    /* K33  */  be_nested_str_weak(out),
    /* K34  */  be_nested_str_weak(set_context),
    /* K35  */  be_nested_str_weak(compute_TT_hash),
    /* K36  */  be_nested_str_weak(cB),
    /* K37  */  be_nested_str_weak(Ke),
    /* K38  */  be_nested_str_weak(Pake2),
    /* K39  */  be_nested_str_weak(tlv2raw),
    /* K40  */  be_nested_str_weak(build_response),
    /* K41  */  be_nested_str_weak(encode_frame),
    /* K42  */  be_nested_str_weak(responder),
    /* K43  */  be_nested_str_weak(send_response_frame),
    }),
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[114]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x54120021,  //  0002  LDINT	R4	34
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x740E0005,  //  0004  JMPT	R3	#000B
      0x880C0302,  //  0005  GETMBR	R3	R1	K2
      0x200C0703,  //  0006  NE	R3	R3	K3
      0x740E0002,  //  0007  JMPT	R3	#000B
      0x880C0304,  //  0008  GETMBR	R3	R1	K4
      0x200C0703,  //  0009  NE	R3	R3	K3
      0x780E0012,  //  000A  JMPF	R3	#001E
      0xB80E0A00,  //  000B  GETNGBL	R3	K5
      0x8C0C0706,  //  000C  GETMET	R3	R3	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB80E0A00,  //  0010  GETNGBL	R3	K5
      0x8C0C0706,  //  0011  GETMET	R3	R3	K6
      0x58140009,  //  0012  LDCONST	R5	K9
      0x58180008,  //  0013  LDCONST	R6	K8
      0x7C0C0600,  //  0014  CALL	R3	3
      0x8C0C010A,  //  0015  GETMET	R3	R0	K10
      0x5C140200,  //  0016  MOVE	R5	R1
      0x5818000B,  //  0017  LDCONST	R6	K11
      0x581C0003,  //  0018  LDCONST	R7	K3
      0x58200008,  //  0019  LDCONST	R8	K8
      0x50240000,  //  001A  LDBOOL	R9	0	0
      0x7C0C0C00,  //  001B  CALL	R3	6
      0x50100000,  //  001C  LDBOOL	R4	0	0
      0x80040800,  //  001D  RET	1	R4
      0xB80E1800,  //  001E  GETNGBL	R3	K12
      0x8C0C070D,  //  001F  GETMET	R3	R3	K13
      0x7C0C0200,  //  0020  CALL	R3	1
      0x8C0C070E,  //  0021  GETMET	R3	R3	K14
      0x8814030F,  //  0022  GETMBR	R5	R1	K15
      0x88180310,  //  0023  GETMBR	R6	R1	K16
      0x7C0C0600,  //  0024  CALL	R3	3
      0x88100711,  //  0025  GETMBR	R4	R3	K17
      0x90022204,  //  0026  SETMBR	R0	K17	R4
      0x8C100513,  //  0027  GETMET	R4	R2	K19
      0x88180114,  //  0028  GETMBR	R6	R0	K20
      0x88180D15,  //  0029  GETMBR	R6	R6	K21
      0x4C1C0000,  //  002A  LDNIL	R7
      0x88200114,  //  002B  GETMBR	R8	R0	K20
      0x88201116,  //  002C  GETMBR	R8	R8	K22
      0x7C100800,  //  002D  CALL	R4	4
      0x90022404,  //  002E  SETMBR	R0	K18	R4
      0x88100112,  //  002F  GETMBR	R4	R0	K18
      0x8C100917,  //  0030  GETMET	R4	R4	K23
      0x88180118,  //  0031  GETMBR	R6	R0	K24
      0x7C100400,  //  0032  CALL	R4	2
      0x88100112,  //  0033  GETMBR	R4	R0	K18
      0x88100919,  //  0034  GETMBR	R4	R4	K25
      0x90023204,  //  0035  SETMBR	R0	K25	R4
      0x88100112,  //  0036  GETMBR	R4	R0	K18
      0x8C10091A,  //  0037  GETMET	R4	R4	K26
      0x88180111,  //  0038  GETMBR	R6	R0	K17
      0x7C100400,  //  0039  CALL	R4	2
      0x8C10051B,  //  003A  GETMET	R4	R2	K27
      0x7C100200,  //  003B  CALL	R4	1
      0x8C14091C,  //  003C  GETMET	R5	R4	K28
      0x601C0015,  //  003D  GETGBL	R7	G21
      0x7C1C0000,  //  003E  CALL	R7	0
      0x8C1C0F1D,  //  003F  GETMET	R7	R7	K29
      0x8824011E,  //  0040  GETMBR	R9	R0	K30
      0x7C1C0400,  //  0041  CALL	R7	2
      0x7C140400,  //  0042  CALL	R5	2
      0x8C14091C,  //  0043  GETMET	R5	R4	K28
      0x881C011F,  //  0044  GETMBR	R7	R0	K31
      0x7C140400,  //  0045  CALL	R5	2
      0x8C14091C,  //  0046  GETMET	R5	R4	K28
      0x881C0120,  //  0047  GETMBR	R7	R0	K32
      0x7C140400,  //  0048  CALL	R5	2
      0x8C140921,  //  0049  GETMET	R5	R4	K33
      0x7C140200,  //  004A  CALL	R5	1
      0x88180112,  //  004B  GETMBR	R6	R0	K18
      0x881C0111,  //  004C  GETMBR	R7	R0	K17
      0x901A2207,  //  004D  SETMBR	R6	K17	R7
      0x88180112,  //  004E  GETMBR	R6	R0	K18
      0x8C180D22,  //  004F  GETMET	R6	R6	K34
      0x5C200A00,  //  0050  MOVE	R8	R5
      0x7C180400,  //  0051  CALL	R6	2
      0x88180112,  //  0052  GETMBR	R6	R0	K18
      0x8C180D23,  //  0053  GETMET	R6	R6	K35
      0x50200200,  //  0054  LDBOOL	R8	1	0
      0x7C180400,  //  0055  CALL	R6	2
      0x88180112,  //  0056  GETMBR	R6	R0	K18
      0x88180D24,  //  0057  GETMBR	R6	R6	K36
      0x90024806,  //  0058  SETMBR	R0	K36	R6
      0x88180112,  //  0059  GETMBR	R6	R0	K18
      0x88180D25,  //  005A  GETMBR	R6	R6	K37
      0x90024A06,  //  005B  SETMBR	R0	K37	R6
      0xB81A1800,  //  005C  GETNGBL	R6	K12
      0x8C180D26,  //  005D  GETMET	R6	R6	K38
      0x7C180200,  //  005E  CALL	R6	1
      0x881C0119,  //  005F  GETMBR	R7	R0	K25
      0x901A3207,  //  0060  SETMBR	R6	K25	R7
      0x881C0124,  //  0061  GETMBR	R7	R0	K36
      0x901A4807,  //  0062  SETMBR	R6	K36	R7
      0x8C1C0D27,  //  0063  GETMET	R7	R6	K39
      0x7C1C0200,  //  0064  CALL	R7	1
      0x8C200328,  //  0065  GETMET	R8	R1	K40
      0x542A0022,  //  0066  LDINT	R10	35
      0x502C0200,  //  0067  LDBOOL	R11	1	0
      0x7C200600,  //  0068  CALL	R8	3
      0x8C241129,  //  0069  GETMET	R9	R8	K41
      0x5C2C0E00,  //  006A  MOVE	R11	R7
      0x7C240400,  //  006B  CALL	R9	2
      0x8828012A,  //  006C  GETMBR	R10	R0	K42
      0x8C28152B,  //  006D  GETMET	R10	R10	K43
      0x5C301000,  //  006E  MOVE	R12	R8
      0x7C280400,  //  006F  CALL	R10	2
      0x50280200,  //  0070  LDBOOL	R10	1	0
      0x80041400,  //  0071  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Commisioning_Context_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(responder),
    /* K2   */  be_nested_str_weak(device),
    /* K3   */  be_nested_str_weak(y),
    /* K4   */  be_nested_str_weak(random),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x880C0302,  //  0002  GETMBR	R3	R1	K2
      0x90020403,  //  0003  SETMBR	R0	K2	R3
      0x8C0C0504,  //  0004  GETMET	R3	R2	K4
      0x5416001F,  //  0005  LDINT	R5	32
      0x7C0C0400,  //  0006  CALL	R3	2
      0x90020603,  //  0007  SETMBR	R0	K3	R3
      0x80000000,  //  0008  RET	0
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
    ( &(const bvalue[20]) {     /* constants */
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
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(parse_PBKDFParamRequest),
    /* K12  */  be_nested_str_weak(parse_Pake1),
    /* K13  */  be_nested_str_weak(parse_Pake3),
    /* K14  */  be_nested_str_weak(parse_Sigma1),
    /* K15  */  be_nested_str_weak(parse_Sigma3),
    /* K16  */  be_nested_str_weak(parse_StatusReport),
    /* K17  */  be_nested_str_weak(string),
    /* K18  */  be_nested_str_weak(format),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28secure_X20channel_X29_X20_X2502X),
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
      0x5814000A,  //  001A  LDCONST	R5	K10
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
      0x8C08010B,  //  0025  GETMET	R2	R0	K11
      0x5C100200,  //  0026  MOVE	R4	R1
      0x7C080400,  //  0027  CALL	R2	2
      0x80040400,  //  0028  RET	1	R2
      0x70020037,  //  0029  JMP		#0062
      0x88080302,  //  002A  GETMBR	R2	R1	K2
      0x540E0021,  //  002B  LDINT	R3	34
      0x1C080403,  //  002C  EQ	R2	R2	R3
      0x780A0004,  //  002D  JMPF	R2	#0033
      0x8C08010C,  //  002E  GETMET	R2	R0	K12
      0x5C100200,  //  002F  MOVE	R4	R1
      0x7C080400,  //  0030  CALL	R2	2
      0x80040400,  //  0031  RET	1	R2
      0x7002002E,  //  0032  JMP		#0062
      0x88080302,  //  0033  GETMBR	R2	R1	K2
      0x540E0023,  //  0034  LDINT	R3	36
      0x1C080403,  //  0035  EQ	R2	R2	R3
      0x780A0004,  //  0036  JMPF	R2	#003C
      0x8C08010D,  //  0037  GETMET	R2	R0	K13
      0x5C100200,  //  0038  MOVE	R4	R1
      0x7C080400,  //  0039  CALL	R2	2
      0x80040400,  //  003A  RET	1	R2
      0x70020025,  //  003B  JMP		#0062
      0x88080302,  //  003C  GETMBR	R2	R1	K2
      0x540E002F,  //  003D  LDINT	R3	48
      0x1C080403,  //  003E  EQ	R2	R2	R3
      0x780A0004,  //  003F  JMPF	R2	#0045
      0x8C08010E,  //  0040  GETMET	R2	R0	K14
      0x5C100200,  //  0041  MOVE	R4	R1
      0x7C080400,  //  0042  CALL	R2	2
      0x80040400,  //  0043  RET	1	R2
      0x7002001C,  //  0044  JMP		#0062
      0x88080302,  //  0045  GETMBR	R2	R1	K2
      0x540E0031,  //  0046  LDINT	R3	50
      0x1C080403,  //  0047  EQ	R2	R2	R3
      0x780A0004,  //  0048  JMPF	R2	#004E
      0x8C08010F,  //  0049  GETMET	R2	R0	K15
      0x5C100200,  //  004A  MOVE	R4	R1
      0x7C080400,  //  004B  CALL	R2	2
      0x80040400,  //  004C  RET	1	R2
      0x70020013,  //  004D  JMP		#0062
      0x88080302,  //  004E  GETMBR	R2	R1	K2
      0x540E003F,  //  004F  LDINT	R3	64
      0x1C080403,  //  0050  EQ	R2	R2	R3
      0x780A0004,  //  0051  JMPF	R2	#0057
      0x8C080110,  //  0052  GETMET	R2	R0	K16
      0x5C100200,  //  0053  MOVE	R4	R1
      0x7C080400,  //  0054  CALL	R2	2
      0x80040400,  //  0055  RET	1	R2
      0x7002000A,  //  0056  JMP		#0062
      0xA40A2200,  //  0057  IMPORT	R2	K17
      0xB80E0600,  //  0058  GETNGBL	R3	K3
      0x8C0C0704,  //  0059  GETMET	R3	R3	K4
      0x8C140512,  //  005A  GETMET	R5	R2	K18
      0x581C0013,  //  005B  LDCONST	R7	K19
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
    ( &(const bvalue[99]) {     /* constants */
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
    /* K17  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K18  */  be_nested_str_weak(SHA256),
    /* K19  */  be_nested_str_weak(update),
    /* K20  */  be_nested_str_weak(__Msg1),
    /* K21  */  be_nested_str_weak(__Msg2),
    /* K22  */  be_nested_str_weak(out),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20session_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X2Eipk_epoch_key_X3D),
    /* K25  */  be_nested_str_weak(get_ipk_epoch_key),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X2Efabric_compr_X20_X3D_X20),
    /* K27  */  be_nested_str_weak(get_fabric_compressed),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20ipk_group_key_X20_X3D_X20),
    /* K29  */  be_nested_str_weak(get_ipk_group_key),
    /* K30  */  be_nested_str_weak(tohex),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TranscriptHash_X3D_X20),
    /* K32  */  be_nested_str_weak(fromstring),
    /* K33  */  be_nested_str_weak(S3K_Info),
    /* K34  */  be_nested_str_weak(HKDF_SHA256),
    /* K35  */  be_nested_str_weak(derive),
    /* K36  */  be_nested_str_weak(shared_secret),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s3k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K38  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s3k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K39  */  be_nested_str_weak(TBEData3Encrypted),
    /* K40  */  be_const_int(2147483647),
    /* K41  */  be_nested_str_weak(AES_CCM),
    /* K42  */  be_nested_str_weak(TBEData3_Nonce),
    /* K43  */  be_nested_str_weak(decrypt),
    /* K44  */  be_nested_str_weak(tag),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData3_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBETag3_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20tag_sent_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K48  */  be_nested_str_weak(MTR_X3A_X20Tag_X20don_X27t_X20match),
    /* K49  */  be_nested_str_weak(TLV),
    /* K50  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData3TLV_X20_X20_X20_X3D_X20),
    /* K51  */  be_nested_str_weak(findsubval),
    /* K52  */  be_const_int(3),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20initiatorNOC_X20_X20_X3D_X20),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20initiatorICAC_X20_X3D_X20),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20ec_signature_X20_X20_X3D_X20),
    /* K56  */  be_nested_str_weak(MTR_X3A_X20initiatorNOCTLV_X20_X3D_X20),
    /* K57  */  be_nested_str_weak(findsub),
    /* K58  */  be_nested_str_weak(int),
    /* K59  */  be_nested_str_weak(int64),
    /* K60  */  be_nested_str_weak(peer_node_id),
    /* K61  */  be_nested_str_weak(tobytes),
    /* K62  */  be_nested_str_weak(MTR_X3A_X20initiatorFabricId_X3D),
    /* K63  */  be_nested_str_weak(Matter_TLV_struct),
    /* K64  */  be_nested_str_weak(add_TLV),
    /* K65  */  be_nested_str_weak(B1),
    /* K66  */  be_nested_str_weak(initiatorEph_pub),
    /* K67  */  be_nested_str_weak(ResponderEph_pub),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20sigma3_tbs_X20_X20_X20_X20_X3D_X20),
    /* K69  */  be_nested_str_weak(tlv2raw),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20sigma3_tbs_raw_X3D_X20),
    /* K71  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20initiatorNOCPubKey_X3D_X20),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20ec_signature_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K73  */  be_nested_str_weak(EC_P256),
    /* K74  */  be_nested_str_weak(ecdsa_verify_sha256),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20sigma3_tbs_X20does_X20not_X20have_X20a_X20valid_X20signature),
    /* K76  */  be_nested_str_weak(MTR_X3A_X20Sigma3_X20verified_X2C_X20computing_X20new_X20keys),
    /* K77  */  be_nested_str_weak(Msg3),
    /* K78  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20__Msg1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K79  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20__Msg2_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K80  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20__Msg3_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K81  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TranscriptHash_X20_X20_X20_X20_X3D_X20),
    /* K82  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K83  */  be_nested_str_weak(MTR_X3A_X20shared_secret_X20_X3D),
    /* K84  */  be_nested_str_weak(MTR_X3A_X20ipk_X20_X2B_X20hash_X20_X20_X20_X20_X3D),
    /* K85  */  be_nested_str_weak(SEKeys_Info),
    /* K86  */  be_nested_str_weak(rtc),
    /* K87  */  be_nested_str_weak(utc),
    /* K88  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K89  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K90  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K91  */  be_nested_str_weak(close),
    /* K92  */  be_nested_str_weak(set_keys),
    /* K93  */  be_nested_str_weak(_breadcrumb),
    /* K94  */  be_nested_str_weak(counter_snd_next),
    /* K95  */  be_nested_str_weak(set_persist),
    /* K96  */  be_nested_str_weak(set_no_expiration),
    /* K97  */  be_nested_str_weak(persist_to_fabric),
    /* K98  */  be_nested_str_weak(save),
    }),
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[558]) {  /* code */
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
      0xB8160A00,  //  0021  GETNGBL	R5	K5
      0x8C140B06,  //  0022  GETMET	R5	R5	K6
      0x581C0011,  //  0023  LDCONST	R7	K17
      0x54220003,  //  0024  LDINT	R8	4
      0x7C140600,  //  0025  CALL	R5	3
      0x8C140512,  //  0026  GETMET	R5	R2	K18
      0x7C140200,  //  0027  CALL	R5	1
      0x8C140B13,  //  0028  GETMET	R5	R5	K19
      0x881C0714,  //  0029  GETMBR	R7	R3	K20
      0x7C140400,  //  002A  CALL	R5	2
      0x8C140B13,  //  002B  GETMET	R5	R5	K19
      0x881C0715,  //  002C  GETMBR	R7	R3	K21
      0x7C140400,  //  002D  CALL	R5	2
      0x8C140B16,  //  002E  GETMET	R5	R5	K22
      0x7C140200,  //  002F  CALL	R5	1
      0xB81A0A00,  //  0030  GETNGBL	R6	K5
      0x8C180D06,  //  0031  GETMET	R6	R6	K6
      0x60200008,  //  0032  GETGBL	R8	G8
      0x5C240600,  //  0033  MOVE	R9	R3
      0x7C200200,  //  0034  CALL	R8	1
      0x00222E08,  //  0035  ADD	R8	K23	R8
      0x54260003,  //  0036  LDINT	R9	4
      0x7C180600,  //  0037  CALL	R6	3
      0xB81A0A00,  //  0038  GETNGBL	R6	K5
      0x8C180D06,  //  0039  GETMET	R6	R6	K6
      0x60200008,  //  003A  GETGBL	R8	G8
      0x8C240719,  //  003B  GETMET	R9	R3	K25
      0x7C240200,  //  003C  CALL	R9	1
      0x7C200200,  //  003D  CALL	R8	1
      0x00223008,  //  003E  ADD	R8	K24	R8
      0x54260003,  //  003F  LDINT	R9	4
      0x7C180600,  //  0040  CALL	R6	3
      0xB81A0A00,  //  0041  GETNGBL	R6	K5
      0x8C180D06,  //  0042  GETMET	R6	R6	K6
      0x60200008,  //  0043  GETGBL	R8	G8
      0x8C24071B,  //  0044  GETMET	R9	R3	K27
      0x7C240200,  //  0045  CALL	R9	1
      0x7C200200,  //  0046  CALL	R8	1
      0x00223408,  //  0047  ADD	R8	K26	R8
      0x54260003,  //  0048  LDINT	R9	4
      0x7C180600,  //  0049  CALL	R6	3
      0xB81A0A00,  //  004A  GETNGBL	R6	K5
      0x8C180D06,  //  004B  GETMET	R6	R6	K6
      0x8C20071D,  //  004C  GETMET	R8	R3	K29
      0x7C200200,  //  004D  CALL	R8	1
      0x8C20111E,  //  004E  GETMET	R8	R8	K30
      0x7C200200,  //  004F  CALL	R8	1
      0x00223808,  //  0050  ADD	R8	K28	R8
      0x54260003,  //  0051  LDINT	R9	4
      0x7C180600,  //  0052  CALL	R6	3
      0xB81A0A00,  //  0053  GETNGBL	R6	K5
      0x8C180D06,  //  0054  GETMET	R6	R6	K6
      0x8C200B1E,  //  0055  GETMET	R8	R5	K30
      0x7C200200,  //  0056  CALL	R8	1
      0x00223E08,  //  0057  ADD	R8	K31	R8
      0x54260003,  //  0058  LDINT	R9	4
      0x7C180600,  //  0059  CALL	R6	3
      0x60180015,  //  005A  GETGBL	R6	G21
      0x7C180000,  //  005B  CALL	R6	0
      0x8C180D20,  //  005C  GETMET	R6	R6	K32
      0x88200121,  //  005D  GETMBR	R8	R0	K33
      0x7C180400,  //  005E  CALL	R6	2
      0x8C1C0522,  //  005F  GETMET	R7	R2	K34
      0x7C1C0200,  //  0060  CALL	R7	1
      0x8C1C0F23,  //  0061  GETMET	R7	R7	K35
      0x88240724,  //  0062  GETMBR	R9	R3	K36
      0x8C28071D,  //  0063  GETMET	R10	R3	K29
      0x7C280200,  //  0064  CALL	R10	1
      0x00281405,  //  0065  ADD	R10	R10	R5
      0x5C2C0C00,  //  0066  MOVE	R11	R6
      0x5432000F,  //  0067  LDINT	R12	16
      0x7C1C0A00,  //  0068  CALL	R7	5
      0xB8220A00,  //  0069  GETNGBL	R8	K5
      0x8C201106,  //  006A  GETMET	R8	R8	K6
      0x58280011,  //  006B  LDCONST	R10	K17
      0x542E0003,  //  006C  LDINT	R11	4
      0x7C200600,  //  006D  CALL	R8	3
      0xB8220A00,  //  006E  GETNGBL	R8	K5
      0x8C201106,  //  006F  GETMET	R8	R8	K6
      0x8C28071D,  //  0070  GETMET	R10	R3	K29
      0x7C280200,  //  0071  CALL	R10	1
      0x00281405,  //  0072  ADD	R10	R10	R5
      0x8C28151E,  //  0073  GETMET	R10	R10	K30
      0x7C280200,  //  0074  CALL	R10	1
      0x002A4A0A,  //  0075  ADD	R10	K37	R10
      0x542E0003,  //  0076  LDINT	R11	4
      0x7C200600,  //  0077  CALL	R8	3
      0xB8220A00,  //  0078  GETNGBL	R8	K5
      0x8C201106,  //  0079  GETMET	R8	R8	K6
      0x8C280F1E,  //  007A  GETMET	R10	R7	K30
      0x7C280200,  //  007B  CALL	R10	1
      0x002A4C0A,  //  007C  ADD	R10	K38	R10
      0x542E0003,  //  007D  LDINT	R11	4
      0x7C200600,  //  007E  CALL	R8	3
      0xB8220A00,  //  007F  GETNGBL	R8	K5
      0x8C201106,  //  0080  GETMET	R8	R8	K6
      0x58280011,  //  0081  LDCONST	R10	K17
      0x542E0003,  //  0082  LDINT	R11	4
      0x7C200600,  //  0083  CALL	R8	3
      0x5421FFEE,  //  0084  LDINT	R8	-17
      0x40220608,  //  0085  CONNECT	R8	K3	R8
      0x88240927,  //  0086  GETMBR	R9	R4	K39
      0x94201208,  //  0087  GETIDX	R8	R9	R8
      0x5429FFEF,  //  0088  LDINT	R10	-16
      0x40281528,  //  0089  CONNECT	R10	R10	K40
      0x882C0927,  //  008A  GETMBR	R11	R4	K39
      0x9424160A,  //  008B  GETIDX	R9	R11	R10
      0x8C300529,  //  008C  GETMET	R12	R2	K41
      0x5C380E00,  //  008D  MOVE	R14	R7
      0x603C0015,  //  008E  GETGBL	R15	G21
      0x7C3C0000,  //  008F  CALL	R15	0
      0x8C3C1F20,  //  0090  GETMET	R15	R15	K32
      0x8844012A,  //  0091  GETMBR	R17	R0	K42
      0x7C3C0400,  //  0092  CALL	R15	2
      0x60400015,  //  0093  GETGBL	R16	G21
      0x7C400000,  //  0094  CALL	R16	0
      0x6044000C,  //  0095  GETGBL	R17	G12
      0x5C481000,  //  0096  MOVE	R18	R8
      0x7C440200,  //  0097  CALL	R17	1
      0x544A000F,  //  0098  LDINT	R18	16
      0x7C300C00,  //  0099  CALL	R12	6
      0x5C281800,  //  009A  MOVE	R10	R12
      0x8C30152B,  //  009B  GETMET	R12	R10	K43
      0x5C381000,  //  009C  MOVE	R14	R8
      0x7C300400,  //  009D  CALL	R12	2
      0x5C2C1800,  //  009E  MOVE	R11	R12
      0x8C30152C,  //  009F  GETMET	R12	R10	K44
      0x7C300200,  //  00A0  CALL	R12	1
      0xB8360A00,  //  00A1  GETNGBL	R13	K5
      0x8C341B06,  //  00A2  GETMET	R13	R13	K6
      0x8C3C171E,  //  00A3  GETMET	R15	R11	K30
      0x7C3C0200,  //  00A4  CALL	R15	1
      0x003E5A0F,  //  00A5  ADD	R15	K45	R15
      0x54420003,  //  00A6  LDINT	R16	4
      0x7C340600,  //  00A7  CALL	R13	3
      0xB8360A00,  //  00A8  GETNGBL	R13	K5
      0x8C341B06,  //  00A9  GETMET	R13	R13	K6
      0x8C3C191E,  //  00AA  GETMET	R15	R12	K30
      0x7C3C0200,  //  00AB  CALL	R15	1
      0x003E5C0F,  //  00AC  ADD	R15	K46	R15
      0x54420003,  //  00AD  LDINT	R16	4
      0x7C340600,  //  00AE  CALL	R13	3
      0xB8360A00,  //  00AF  GETNGBL	R13	K5
      0x8C341B06,  //  00B0  GETMET	R13	R13	K6
      0x8C3C131E,  //  00B1  GETMET	R15	R9	K30
      0x7C3C0200,  //  00B2  CALL	R15	1
      0x003E5E0F,  //  00B3  ADD	R15	K47	R15
      0x54420003,  //  00B4  LDINT	R16	4
      0x7C340600,  //  00B5  CALL	R13	3
      0xB8360A00,  //  00B6  GETNGBL	R13	K5
      0x8C341B06,  //  00B7  GETMET	R13	R13	K6
      0x583C0011,  //  00B8  LDCONST	R15	K17
      0x54420003,  //  00B9  LDINT	R16	4
      0x7C340600,  //  00BA  CALL	R13	3
      0x20341809,  //  00BB  NE	R13	R12	R9
      0x78360012,  //  00BC  JMPF	R13	#00D0
      0xB8360A00,  //  00BD  GETNGBL	R13	K5
      0x8C341B06,  //  00BE  GETMET	R13	R13	K6
      0x583C0030,  //  00BF  LDCONST	R15	K48
      0x58400008,  //  00C0  LDCONST	R16	K8
      0x7C340600,  //  00C1  CALL	R13	3
      0xB8360A00,  //  00C2  GETNGBL	R13	K5
      0x8C341B06,  //  00C3  GETMET	R13	R13	K6
      0x583C0007,  //  00C4  LDCONST	R15	K7
      0x58400008,  //  00C5  LDCONST	R16	K8
      0x7C340600,  //  00C6  CALL	R13	3
      0x8C340109,  //  00C7  GETMET	R13	R0	K9
      0x5C3C0200,  //  00C8  MOVE	R15	R1
      0x5840000A,  //  00C9  LDCONST	R16	K10
      0x58440003,  //  00CA  LDCONST	R17	K3
      0x58480008,  //  00CB  LDCONST	R18	K8
      0x504C0000,  //  00CC  LDBOOL	R19	0	0
      0x7C340C00,  //  00CD  CALL	R13	6
      0x50380000,  //  00CE  LDBOOL	R14	0	0
      0x80041C00,  //  00CF  RET	1	R14
      0xB8361800,  //  00D0  GETNGBL	R13	K12
      0x88341B31,  //  00D1  GETMBR	R13	R13	K49
      0x8C341B0E,  //  00D2  GETMET	R13	R13	K14
      0x5C3C1600,  //  00D3  MOVE	R15	R11
      0x7C340400,  //  00D4  CALL	R13	2
      0xB83A0A00,  //  00D5  GETNGBL	R14	K5
      0x8C381D06,  //  00D6  GETMET	R14	R14	K6
      0x60400008,  //  00D7  GETGBL	R16	G8
      0x5C441A00,  //  00D8  MOVE	R17	R13
      0x7C400200,  //  00D9  CALL	R16	1
      0x00426410,  //  00DA  ADD	R16	K50	R16
      0x54460003,  //  00DB  LDINT	R17	4
      0x7C380600,  //  00DC  CALL	R14	3
      0x8C381B33,  //  00DD  GETMET	R14	R13	K51
      0x5840000A,  //  00DE  LDCONST	R16	K10
      0x7C380400,  //  00DF  CALL	R14	2
      0x8C3C1B33,  //  00E0  GETMET	R15	R13	K51
      0x58440008,  //  00E1  LDCONST	R17	K8
      0x7C3C0400,  //  00E2  CALL	R15	2
      0x8C401B33,  //  00E3  GETMET	R16	R13	K51
      0x58480034,  //  00E4  LDCONST	R18	K52
      0x7C400400,  //  00E5  CALL	R16	2
      0xB8460A00,  //  00E6  GETNGBL	R17	K5
      0x8C442306,  //  00E7  GETMET	R17	R17	K6
      0x604C0008,  //  00E8  GETGBL	R19	G8
      0x5C501C00,  //  00E9  MOVE	R20	R14
      0x7C4C0200,  //  00EA  CALL	R19	1
      0x004E6A13,  //  00EB  ADD	R19	K53	R19
      0x54520003,  //  00EC  LDINT	R20	4
      0x7C440600,  //  00ED  CALL	R17	3
      0xB8460A00,  //  00EE  GETNGBL	R17	K5
      0x8C442306,  //  00EF  GETMET	R17	R17	K6
      0x604C0008,  //  00F0  GETGBL	R19	G8
      0x5C501E00,  //  00F1  MOVE	R20	R15
      0x7C4C0200,  //  00F2  CALL	R19	1
      0x004E6C13,  //  00F3  ADD	R19	K54	R19
      0x54520003,  //  00F4  LDINT	R20	4
      0x7C440600,  //  00F5  CALL	R17	3
      0xB8460A00,  //  00F6  GETNGBL	R17	K5
      0x8C442306,  //  00F7  GETMET	R17	R17	K6
      0x604C0008,  //  00F8  GETGBL	R19	G8
      0x5C502000,  //  00F9  MOVE	R20	R16
      0x7C4C0200,  //  00FA  CALL	R19	1
      0x004E6E13,  //  00FB  ADD	R19	K55	R19
      0x54520003,  //  00FC  LDINT	R20	4
      0x7C440600,  //  00FD  CALL	R17	3
      0xB8461800,  //  00FE  GETNGBL	R17	K12
      0x88442331,  //  00FF  GETMBR	R17	R17	K49
      0x8C44230E,  //  0100  GETMET	R17	R17	K14
      0x5C4C1C00,  //  0101  MOVE	R19	R14
      0x7C440400,  //  0102  CALL	R17	2
      0xB84A0A00,  //  0103  GETNGBL	R18	K5
      0x8C482506,  //  0104  GETMET	R18	R18	K6
      0x60500008,  //  0105  GETGBL	R20	G8
      0x5C542200,  //  0106  MOVE	R21	R17
      0x7C500200,  //  0107  CALL	R20	1
      0x00527014,  //  0108  ADD	R20	K56	R20
      0x58540034,  //  0109  LDCONST	R21	K52
      0x7C480600,  //  010A  CALL	R18	3
      0x8C482333,  //  010B  GETMET	R18	R17	K51
      0x54520008,  //  010C  LDINT	R20	9
      0x7C480400,  //  010D  CALL	R18	2
      0x8C4C2339,  //  010E  GETMET	R19	R17	K57
      0x54560005,  //  010F  LDINT	R21	6
      0x7C4C0400,  //  0110  CALL	R19	2
      0x8C502733,  //  0111  GETMET	R20	R19	K51
      0x545A0010,  //  0112  LDINT	R22	17
      0x7C500400,  //  0113  CALL	R20	2
      0x60540004,  //  0114  GETGBL	R21	G4
      0x5C582800,  //  0115  MOVE	R22	R20
      0x7C540200,  //  0116  CALL	R21	1
      0x1C542B3A,  //  0117  EQ	R21	R21	K58
      0x78560003,  //  0118  JMPF	R21	#011D
      0xB8567600,  //  0119  GETNGBL	R21	K59
      0x5C582800,  //  011A  MOVE	R22	R20
      0x7C540200,  //  011B  CALL	R21	1
      0x5C502A00,  //  011C  MOVE	R20	R21
      0x8C54293D,  //  011D  GETMET	R21	R20	K61
      0x7C540200,  //  011E  CALL	R21	1
      0x900E7815,  //  011F  SETMBR	R3	K60	R21
      0xB8560A00,  //  0120  GETNGBL	R21	K5
      0x8C542B06,  //  0121  GETMET	R21	R21	K6
      0x605C0008,  //  0122  GETGBL	R23	G8
      0x8860073C,  //  0123  GETMBR	R24	R3	K60
      0x7C5C0200,  //  0124  CALL	R23	1
      0x005E7C17,  //  0125  ADD	R23	K62	R23
      0x58600034,  //  0126  LDCONST	R24	K52
      0x7C540600,  //  0127  CALL	R21	3
      0xB8561800,  //  0128  GETNGBL	R21	K12
      0x88542B31,  //  0129  GETMBR	R21	R21	K49
      0x8C542B3F,  //  012A  GETMET	R21	R21	K63
      0x7C540200,  //  012B  CALL	R21	1
      0x8C582B40,  //  012C  GETMET	R22	R21	K64
      0x5860000A,  //  012D  LDCONST	R24	K10
      0xB8661800,  //  012E  GETNGBL	R25	K12
      0x88643331,  //  012F  GETMBR	R25	R25	K49
      0x88643341,  //  0130  GETMBR	R25	R25	K65
      0x5C681C00,  //  0131  MOVE	R26	R14
      0x7C580800,  //  0132  CALL	R22	4
      0x8C582B40,  //  0133  GETMET	R22	R21	K64
      0x58600008,  //  0134  LDCONST	R24	K8
      0xB8661800,  //  0135  GETNGBL	R25	K12
      0x88643331,  //  0136  GETMBR	R25	R25	K49
      0x88643341,  //  0137  GETMBR	R25	R25	K65
      0x5C681E00,  //  0138  MOVE	R26	R15
      0x7C580800,  //  0139  CALL	R22	4
      0x8C582B40,  //  013A  GETMET	R22	R21	K64
      0x58600034,  //  013B  LDCONST	R24	K52
      0xB8661800,  //  013C  GETNGBL	R25	K12
      0x88643331,  //  013D  GETMBR	R25	R25	K49
      0x88643341,  //  013E  GETMBR	R25	R25	K65
      0x88680142,  //  013F  GETMBR	R26	R0	K66
      0x7C580800,  //  0140  CALL	R22	4
      0x8C582B40,  //  0141  GETMET	R22	R21	K64
      0x54620003,  //  0142  LDINT	R24	4
      0xB8661800,  //  0143  GETNGBL	R25	K12
      0x88643331,  //  0144  GETMBR	R25	R25	K49
      0x88643341,  //  0145  GETMBR	R25	R25	K65
      0x88680143,  //  0146  GETMBR	R26	R0	K67
      0x7C580800,  //  0147  CALL	R22	4
      0xB85A0A00,  //  0148  GETNGBL	R22	K5
      0x8C582D06,  //  0149  GETMET	R22	R22	K6
      0x60600008,  //  014A  GETGBL	R24	G8
      0x5C642A00,  //  014B  MOVE	R25	R21
      0x7C600200,  //  014C  CALL	R24	1
      0x00628818,  //  014D  ADD	R24	K68	R24
      0x54660003,  //  014E  LDINT	R25	4
      0x7C580600,  //  014F  CALL	R22	3
      0x8C582B45,  //  0150  GETMET	R22	R21	K69
      0x7C580200,  //  0151  CALL	R22	1
      0xB85E0A00,  //  0152  GETNGBL	R23	K5
      0x8C5C2F06,  //  0153  GETMET	R23	R23	K6
      0x8C642D1E,  //  0154  GETMET	R25	R22	K30
      0x7C640200,  //  0155  CALL	R25	1
      0x00668C19,  //  0156  ADD	R25	K70	R25
      0x546A0003,  //  0157  LDINT	R26	4
      0x7C5C0600,  //  0158  CALL	R23	3
      0xB85E0A00,  //  0159  GETNGBL	R23	K5
      0x8C5C2F06,  //  015A  GETMET	R23	R23	K6
      0x8C64251E,  //  015B  GETMET	R25	R18	K30
      0x7C640200,  //  015C  CALL	R25	1
      0x00668E19,  //  015D  ADD	R25	K71	R25
      0x546A0003,  //  015E  LDINT	R26	4
      0x7C5C0600,  //  015F  CALL	R23	3
      0xB85E0A00,  //  0160  GETNGBL	R23	K5
      0x8C5C2F06,  //  0161  GETMET	R23	R23	K6
      0x8C64211E,  //  0162  GETMET	R25	R16	K30
      0x7C640200,  //  0163  CALL	R25	1
      0x00669019,  //  0164  ADD	R25	K72	R25
      0x546A0003,  //  0165  LDINT	R26	4
      0x7C5C0600,  //  0166  CALL	R23	3
      0xB85E0A00,  //  0167  GETNGBL	R23	K5
      0x8C5C2F06,  //  0168  GETMET	R23	R23	K6
      0x58640011,  //  0169  LDCONST	R25	K17
      0x546A0003,  //  016A  LDINT	R26	4
      0x7C5C0600,  //  016B  CALL	R23	3
      0x8C5C0549,  //  016C  GETMET	R23	R2	K73
      0x7C5C0200,  //  016D  CALL	R23	1
      0x8C5C2F4A,  //  016E  GETMET	R23	R23	K74
      0x5C642400,  //  016F  MOVE	R25	R18
      0x5C682C00,  //  0170  MOVE	R26	R22
      0x5C6C2000,  //  0171  MOVE	R27	R16
      0x7C5C0800,  //  0172  CALL	R23	4
      0x5C602E00,  //  0173  MOVE	R24	R23
      0x74620012,  //  0174  JMPT	R24	#0188
      0xB8620A00,  //  0175  GETNGBL	R24	K5
      0x8C603106,  //  0176  GETMET	R24	R24	K6
      0x5868004B,  //  0177  LDCONST	R26	K75
      0x586C0008,  //  0178  LDCONST	R27	K8
      0x7C600600,  //  0179  CALL	R24	3
      0xB8620A00,  //  017A  GETNGBL	R24	K5
      0x8C603106,  //  017B  GETMET	R24	R24	K6
      0x58680007,  //  017C  LDCONST	R26	K7
      0x586C0008,  //  017D  LDCONST	R27	K8
      0x7C600600,  //  017E  CALL	R24	3
      0x8C600109,  //  017F  GETMET	R24	R0	K9
      0x5C680200,  //  0180  MOVE	R26	R1
      0x586C000A,  //  0181  LDCONST	R27	K10
      0x58700003,  //  0182  LDCONST	R28	K3
      0x58740008,  //  0183  LDCONST	R29	K8
      0x50780000,  //  0184  LDBOOL	R30	0	0
      0x7C600C00,  //  0185  CALL	R24	6
      0x50640000,  //  0186  LDBOOL	R25	0	0
      0x80043200,  //  0187  RET	1	R25
      0xB8620A00,  //  0188  GETNGBL	R24	K5
      0x8C603106,  //  0189  GETMET	R24	R24	K6
      0x5868004C,  //  018A  LDCONST	R26	K76
      0x586C0034,  //  018B  LDCONST	R27	K52
      0x7C600600,  //  018C  CALL	R24	3
      0x8C600512,  //  018D  GETMET	R24	R2	K18
      0x7C600200,  //  018E  CALL	R24	1
      0x8C603113,  //  018F  GETMET	R24	R24	K19
      0x88680714,  //  0190  GETMBR	R26	R3	K20
      0x7C600400,  //  0191  CALL	R24	2
      0x8C603113,  //  0192  GETMET	R24	R24	K19
      0x88680715,  //  0193  GETMBR	R26	R3	K21
      0x7C600400,  //  0194  CALL	R24	2
      0x8C603113,  //  0195  GETMET	R24	R24	K19
      0x8868094D,  //  0196  GETMBR	R26	R4	K77
      0x7C600400,  //  0197  CALL	R24	2
      0x8C603116,  //  0198  GETMET	R24	R24	K22
      0x7C600200,  //  0199  CALL	R24	1
      0x5C143000,  //  019A  MOVE	R5	R24
      0xB8620A00,  //  019B  GETNGBL	R24	K5
      0x8C603106,  //  019C  GETMET	R24	R24	K6
      0x88680714,  //  019D  GETMBR	R26	R3	K20
      0x8C68351E,  //  019E  GETMET	R26	R26	K30
      0x7C680200,  //  019F  CALL	R26	1
      0x006A9C1A,  //  01A0  ADD	R26	K78	R26
      0x546E0003,  //  01A1  LDINT	R27	4
      0x7C600600,  //  01A2  CALL	R24	3
      0xB8620A00,  //  01A3  GETNGBL	R24	K5
      0x8C603106,  //  01A4  GETMET	R24	R24	K6
      0x88680715,  //  01A5  GETMBR	R26	R3	K21
      0x8C68351E,  //  01A6  GETMET	R26	R26	K30
      0x7C680200,  //  01A7  CALL	R26	1
      0x006A9E1A,  //  01A8  ADD	R26	K79	R26
      0x546E0003,  //  01A9  LDINT	R27	4
      0x7C600600,  //  01AA  CALL	R24	3
      0xB8620A00,  //  01AB  GETNGBL	R24	K5
      0x8C603106,  //  01AC  GETMET	R24	R24	K6
      0x8868094D,  //  01AD  GETMBR	R26	R4	K77
      0x8C68351E,  //  01AE  GETMET	R26	R26	K30
      0x7C680200,  //  01AF  CALL	R26	1
      0x006AA01A,  //  01B0  ADD	R26	K80	R26
      0x546E0003,  //  01B1  LDINT	R27	4
      0x7C600600,  //  01B2  CALL	R24	3
      0xB8620A00,  //  01B3  GETNGBL	R24	K5
      0x8C603106,  //  01B4  GETMET	R24	R24	K6
      0x8C680B1E,  //  01B5  GETMET	R26	R5	K30
      0x7C680200,  //  01B6  CALL	R26	1
      0x006AA21A,  //  01B7  ADD	R26	K81	R26
      0x546E0003,  //  01B8  LDINT	R27	4
      0x7C600600,  //  01B9  CALL	R24	3
      0x4C600000,  //  01BA  LDNIL	R24
      0x900E2818,  //  01BB  SETMBR	R3	K20	R24
      0x4C600000,  //  01BC  LDNIL	R24
      0x900E2A18,  //  01BD  SETMBR	R3	K21	R24
      0xB8620A00,  //  01BE  GETNGBL	R24	K5
      0x8C603106,  //  01BF  GETMET	R24	R24	K6
      0x58680052,  //  01C0  LDCONST	R26	K82
      0x546E0003,  //  01C1  LDINT	R27	4
      0x7C600600,  //  01C2  CALL	R24	3
      0xB8620A00,  //  01C3  GETNGBL	R24	K5
      0x8C603106,  //  01C4  GETMET	R24	R24	K6
      0x88680724,  //  01C5  GETMBR	R26	R3	K36
      0x8C68351E,  //  01C6  GETMET	R26	R26	K30
      0x7C680200,  //  01C7  CALL	R26	1
      0x006AA61A,  //  01C8  ADD	R26	K83	R26
      0x546E0003,  //  01C9  LDINT	R27	4
      0x7C600600,  //  01CA  CALL	R24	3
      0xB8620A00,  //  01CB  GETNGBL	R24	K5
      0x8C603106,  //  01CC  GETMET	R24	R24	K6
      0x8C68071D,  //  01CD  GETMET	R26	R3	K29
      0x7C680200,  //  01CE  CALL	R26	1
      0x00683405,  //  01CF  ADD	R26	R26	R5
      0x8C68351E,  //  01D0  GETMET	R26	R26	K30
      0x7C680200,  //  01D1  CALL	R26	1
      0x006AA81A,  //  01D2  ADD	R26	K84	R26
      0x546E0003,  //  01D3  LDINT	R27	4
      0x7C600600,  //  01D4  CALL	R24	3
      0x8C600522,  //  01D5  GETMET	R24	R2	K34
      0x7C600200,  //  01D6  CALL	R24	1
      0x8C603123,  //  01D7  GETMET	R24	R24	K35
      0x88680724,  //  01D8  GETMBR	R26	R3	K36
      0x8C6C071D,  //  01D9  GETMET	R27	R3	K29
      0x7C6C0200,  //  01DA  CALL	R27	1
      0x006C3605,  //  01DB  ADD	R27	R27	R5
      0x60700015,  //  01DC  GETGBL	R28	G21
      0x7C700000,  //  01DD  CALL	R28	0
      0x8C703920,  //  01DE  GETMET	R28	R28	K32
      0x88780155,  //  01DF  GETMBR	R30	R0	K85
      0x7C700400,  //  01E0  CALL	R28	2
      0x5476002F,  //  01E1  LDINT	R29	48
      0x7C600A00,  //  01E2  CALL	R24	5
      0x5466000E,  //  01E3  LDINT	R25	15
      0x40660619,  //  01E4  CONNECT	R25	K3	R25
      0x94643019,  //  01E5  GETIDX	R25	R24	R25
      0x546A000F,  //  01E6  LDINT	R26	16
      0x546E001E,  //  01E7  LDINT	R27	31
      0x4068341B,  //  01E8  CONNECT	R26	R26	R27
      0x9468301A,  //  01E9  GETIDX	R26	R24	R26
      0x546E001F,  //  01EA  LDINT	R27	32
      0x5472002E,  //  01EB  LDINT	R28	47
      0x406C361C,  //  01EC  CONNECT	R27	R27	R28
      0x946C301B,  //  01ED  GETIDX	R27	R24	R27
      0xB8720A00,  //  01EE  GETNGBL	R28	K5
      0x8C703956,  //  01EF  GETMET	R28	R28	K86
      0x7C700200,  //  01F0  CALL	R28	1
      0x94703957,  //  01F1  GETIDX	R28	R28	K87
      0xB8760A00,  //  01F2  GETNGBL	R29	K5
      0x8C743B06,  //  01F3  GETMET	R29	R29	K6
      0x587C0052,  //  01F4  LDCONST	R31	K82
      0x54820003,  //  01F5  LDINT	R32	4
      0x7C740600,  //  01F6  CALL	R29	3
      0xB8760A00,  //  01F7  GETNGBL	R29	K5
      0x8C743B06,  //  01F8  GETMET	R29	R29	K6
      0x8C7C331E,  //  01F9  GETMET	R31	R25	K30
      0x7C7C0200,  //  01FA  CALL	R31	1
      0x007EB01F,  //  01FB  ADD	R31	K88	R31
      0x54820003,  //  01FC  LDINT	R32	4
      0x7C740600,  //  01FD  CALL	R29	3
      0xB8760A00,  //  01FE  GETNGBL	R29	K5
      0x8C743B06,  //  01FF  GETMET	R29	R29	K6
      0x8C7C351E,  //  0200  GETMET	R31	R26	K30
      0x7C7C0200,  //  0201  CALL	R31	1
      0x007EB21F,  //  0202  ADD	R31	K89	R31
      0x54820003,  //  0203  LDINT	R32	4
      0x7C740600,  //  0204  CALL	R29	3
      0xB8760A00,  //  0205  GETNGBL	R29	K5
      0x8C743B06,  //  0206  GETMET	R29	R29	K6
      0x8C7C371E,  //  0207  GETMET	R31	R27	K30
      0x7C7C0200,  //  0208  CALL	R31	1
      0x007EB41F,  //  0209  ADD	R31	K90	R31
      0x54820003,  //  020A  LDINT	R32	4
      0x7C740600,  //  020B  CALL	R29	3
      0xB8760A00,  //  020C  GETNGBL	R29	K5
      0x8C743B06,  //  020D  GETMET	R29	R29	K6
      0x587C0052,  //  020E  LDCONST	R31	K82
      0x54820003,  //  020F  LDINT	R32	4
      0x7C740600,  //  0210  CALL	R29	3
      0x8C740109,  //  0211  GETMET	R29	R0	K9
      0x5C7C0200,  //  0212  MOVE	R31	R1
      0x58800003,  //  0213  LDCONST	R32	K3
      0x58840003,  //  0214  LDCONST	R33	K3
      0x58880003,  //  0215  LDCONST	R34	K3
      0x508C0200,  //  0216  LDBOOL	R35	1	0
      0x7C740C00,  //  0217  CALL	R29	6
      0x8C78075B,  //  0218  GETMET	R30	R3	K91
      0x7C780200,  //  0219  CALL	R30	1
      0x8C78075C,  //  021A  GETMET	R30	R3	K92
      0x5C803200,  //  021B  MOVE	R32	R25
      0x5C843400,  //  021C  MOVE	R33	R26
      0x5C883600,  //  021D  MOVE	R34	R27
      0x5C8C3800,  //  021E  MOVE	R35	R28
      0x7C780A00,  //  021F  CALL	R30	5
      0x900EBB03,  //  0220  SETMBR	R3	K93	K3
      0x8C78075E,  //  0221  GETMET	R30	R3	K94
      0x7C780200,  //  0222  CALL	R30	1
      0x8C78075F,  //  0223  GETMET	R30	R3	K95
      0x50800200,  //  0224  LDBOOL	R32	1	0
      0x7C780400,  //  0225  CALL	R30	2
      0x8C780760,  //  0226  GETMET	R30	R3	K96
      0x7C780200,  //  0227  CALL	R30	1
      0x8C780761,  //  0228  GETMET	R30	R3	K97
      0x7C780200,  //  0229  CALL	R30	1
      0x8C780762,  //  022A  GETMET	R30	R3	K98
      0x7C780200,  //  022B  CALL	R30	1
      0x50780200,  //  022C  LDBOOL	R30	1	0
      0x80043C00,  //  022D  RET	1	R30
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_session
********************************************************************/
be_local_closure(Matter_Commisioning_Context_add_session,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    7,                          /* argc */
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
    ( &(const binstruction[23]) {  /* code */
      0xA41E0000,  //  0000  IMPORT	R7	K0
      0xB8220200,  //  0001  GETNGBL	R8	K1
      0x8C201102,  //  0002  GETMET	R8	R8	K2
      0x8C280F03,  //  0003  GETMET	R10	R7	K3
      0x58300004,  //  0004  LDCONST	R12	K4
      0x5C340200,  //  0005  MOVE	R13	R1
      0x5C380400,  //  0006  MOVE	R14	R2
      0x7C280800,  //  0007  CALL	R10	4
      0x582C0005,  //  0008  LDCONST	R11	K5
      0x7C200600,  //  0009  CALL	R8	3
      0x88200106,  //  000A  GETMBR	R8	R0	K6
      0x88201107,  //  000B  GETMBR	R8	R8	K7
      0x8C201108,  //  000C  GETMET	R8	R8	K8
      0x5C280200,  //  000D  MOVE	R10	R1
      0x5C2C0400,  //  000E  MOVE	R11	R2
      0x7C200600,  //  000F  CALL	R8	3
      0x8C241109,  //  0010  GETMET	R9	R8	K9
      0x5C2C0600,  //  0011  MOVE	R11	R3
      0x5C300800,  //  0012  MOVE	R12	R4
      0x5C340A00,  //  0013  MOVE	R13	R5
      0x5C380C00,  //  0014  MOVE	R14	R6
      0x7C240A00,  //  0015  CALL	R9	5
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Pake3
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Pake3,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[34]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake3_X20message),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K10  */  be_nested_str_weak(send_status_report),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(matter),
    /* K13  */  be_nested_str_weak(Pake3),
    /* K14  */  be_nested_str_weak(parse),
    /* K15  */  be_nested_str_weak(raw),
    /* K16  */  be_nested_str_weak(app_payload_idx),
    /* K17  */  be_nested_str_weak(cA),
    /* K18  */  be_nested_str_weak(spake),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20invalid_X20cA_X20received),
    /* K20  */  be_nested_str_weak(created),
    /* K21  */  be_nested_str_weak(rtc),
    /* K22  */  be_nested_str_weak(utc),
    /* K23  */  be_nested_str_weak(HKDF_SHA256),
    /* K24  */  be_nested_str_weak(derive),
    /* K25  */  be_nested_str_weak(Ke),
    /* K26  */  be_nested_str_weak(fromstring),
    /* K27  */  be_nested_str_weak(SEKeys_Info),
    /* K28  */  be_nested_str_weak(I2RKey),
    /* K29  */  be_nested_str_weak(R2IKey),
    /* K30  */  be_nested_str_weak(AttestationChallenge),
    /* K31  */  be_nested_str_weak(add_session),
    /* K32  */  be_nested_str_weak(future_local_session_id),
    /* K33  */  be_nested_str_weak(future_initiator_session_id),
    }),
    be_str_weak(parse_Pake3),
    &be_const_str_solidified,
    ( &(const binstruction[112]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x54120023,  //  0002  LDINT	R4	36
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x740E0005,  //  0004  JMPT	R3	#000B
      0x880C0302,  //  0005  GETMBR	R3	R1	K2
      0x200C0703,  //  0006  NE	R3	R3	K3
      0x740E0002,  //  0007  JMPT	R3	#000B
      0x880C0304,  //  0008  GETMBR	R3	R1	K4
      0x200C0703,  //  0009  NE	R3	R3	K3
      0x780E0012,  //  000A  JMPF	R3	#001E
      0xB80E0A00,  //  000B  GETNGBL	R3	K5
      0x8C0C0706,  //  000C  GETMET	R3	R3	K6
      0x58140007,  //  000D  LDCONST	R5	K7
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB80E0A00,  //  0010  GETNGBL	R3	K5
      0x8C0C0706,  //  0011  GETMET	R3	R3	K6
      0x58140009,  //  0012  LDCONST	R5	K9
      0x58180008,  //  0013  LDCONST	R6	K8
      0x7C0C0600,  //  0014  CALL	R3	3
      0x8C0C010A,  //  0015  GETMET	R3	R0	K10
      0x5C140200,  //  0016  MOVE	R5	R1
      0x5818000B,  //  0017  LDCONST	R6	K11
      0x581C0003,  //  0018  LDCONST	R7	K3
      0x58200008,  //  0019  LDCONST	R8	K8
      0x50240000,  //  001A  LDBOOL	R9	0	0
      0x7C0C0C00,  //  001B  CALL	R3	6
      0x50100000,  //  001C  LDBOOL	R4	0	0
      0x80040800,  //  001D  RET	1	R4
      0xB80E1800,  //  001E  GETNGBL	R3	K12
      0x8C0C070D,  //  001F  GETMET	R3	R3	K13
      0x7C0C0200,  //  0020  CALL	R3	1
      0x8C0C070E,  //  0021  GETMET	R3	R3	K14
      0x8814030F,  //  0022  GETMBR	R5	R1	K15
      0x88180310,  //  0023  GETMBR	R6	R1	K16
      0x7C0C0600,  //  0024  CALL	R3	3
      0x88100711,  //  0025  GETMBR	R4	R3	K17
      0x90022204,  //  0026  SETMBR	R0	K17	R4
      0x88100111,  //  0027  GETMBR	R4	R0	K17
      0x88140112,  //  0028  GETMBR	R5	R0	K18
      0x88140B11,  //  0029  GETMBR	R5	R5	K17
      0x20100805,  //  002A  NE	R4	R4	R5
      0x78120012,  //  002B  JMPF	R4	#003F
      0xB8120A00,  //  002C  GETNGBL	R4	K5
      0x8C100906,  //  002D  GETMET	R4	R4	K6
      0x58180013,  //  002E  LDCONST	R6	K19
      0x581C0008,  //  002F  LDCONST	R7	K8
      0x7C100600,  //  0030  CALL	R4	3
      0xB8120A00,  //  0031  GETNGBL	R4	K5
      0x8C100906,  //  0032  GETMET	R4	R4	K6
      0x58180009,  //  0033  LDCONST	R6	K9
      0x581C0008,  //  0034  LDCONST	R7	K8
      0x7C100600,  //  0035  CALL	R4	3
      0x8C10010A,  //  0036  GETMET	R4	R0	K10
      0x5C180200,  //  0037  MOVE	R6	R1
      0x581C000B,  //  0038  LDCONST	R7	K11
      0x58200003,  //  0039  LDCONST	R8	K3
      0x58240008,  //  003A  LDCONST	R9	K8
      0x50280000,  //  003B  LDBOOL	R10	0	0
      0x7C100C00,  //  003C  CALL	R4	6
      0x50140000,  //  003D  LDBOOL	R5	0	0
      0x80040A00,  //  003E  RET	1	R5
      0xB8120A00,  //  003F  GETNGBL	R4	K5
      0x8C100915,  //  0040  GETMET	R4	R4	K21
      0x7C100200,  //  0041  CALL	R4	1
      0x94100916,  //  0042  GETIDX	R4	R4	K22
      0x90022804,  //  0043  SETMBR	R0	K20	R4
      0x8C100517,  //  0044  GETMET	R4	R2	K23
      0x7C100200,  //  0045  CALL	R4	1
      0x8C100918,  //  0046  GETMET	R4	R4	K24
      0x88180119,  //  0047  GETMBR	R6	R0	K25
      0x601C0015,  //  0048  GETGBL	R7	G21
      0x7C1C0000,  //  0049  CALL	R7	0
      0x60200015,  //  004A  GETGBL	R8	G21
      0x7C200000,  //  004B  CALL	R8	0
      0x8C20111A,  //  004C  GETMET	R8	R8	K26
      0x8828011B,  //  004D  GETMBR	R10	R0	K27
      0x7C200400,  //  004E  CALL	R8	2
      0x5426002F,  //  004F  LDINT	R9	48
      0x7C100A00,  //  0050  CALL	R4	5
      0x5416000E,  //  0051  LDINT	R5	15
      0x40160605,  //  0052  CONNECT	R5	K3	R5
      0x94140805,  //  0053  GETIDX	R5	R4	R5
      0x90023805,  //  0054  SETMBR	R0	K28	R5
      0x5416000F,  //  0055  LDINT	R5	16
      0x541A001E,  //  0056  LDINT	R6	31
      0x40140A06,  //  0057  CONNECT	R5	R5	R6
      0x94140805,  //  0058  GETIDX	R5	R4	R5
      0x90023A05,  //  0059  SETMBR	R0	K29	R5
      0x5416001F,  //  005A  LDINT	R5	32
      0x541A002E,  //  005B  LDINT	R6	47
      0x40140A06,  //  005C  CONNECT	R5	R5	R6
      0x94140805,  //  005D  GETIDX	R5	R4	R5
      0x90023C05,  //  005E  SETMBR	R0	K30	R5
      0x8C14010A,  //  005F  GETMET	R5	R0	K10
      0x5C1C0200,  //  0060  MOVE	R7	R1
      0x58200003,  //  0061  LDCONST	R8	K3
      0x58240003,  //  0062  LDCONST	R9	K3
      0x58280003,  //  0063  LDCONST	R10	K3
      0x502C0000,  //  0064  LDBOOL	R11	0	0
      0x7C140C00,  //  0065  CALL	R5	6
      0x8C18011F,  //  0066  GETMET	R6	R0	K31
      0x88200120,  //  0067  GETMBR	R8	R0	K32
      0x88240121,  //  0068  GETMBR	R9	R0	K33
      0x8828011C,  //  0069  GETMBR	R10	R0	K28
      0x882C011D,  //  006A  GETMBR	R11	R0	K29
      0x8830011E,  //  006B  GETMBR	R12	R0	K30
      0x88340114,  //  006C  GETMBR	R13	R0	K20
      0x7C180E00,  //  006D  CALL	R6	7
      0x50180200,  //  006E  LDBOOL	R6	1	0
      0x80040C00,  //  006F  RET	1	R6
    })
  )
);
/*******************************************************************/


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
    ( &(const bvalue[145]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(send_status_report),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(matter),
    /* K13  */  be_nested_str_weak(Sigma1),
    /* K14  */  be_nested_str_weak(parse),
    /* K15  */  be_nested_str_weak(raw),
    /* K16  */  be_nested_str_weak(app_payload_idx),
    /* K17  */  be_nested_str_weak(format),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20sigma1_X3D_X25s),
    /* K19  */  be_nested_str_weak(inspect),
    /* K20  */  be_nested_str_weak(initiatorEph_pub),
    /* K21  */  be_nested_str_weak(initiatorEphPubKey),
    /* K22  */  be_nested_str_weak(resumptionID),
    /* K23  */  be_nested_str_weak(initiatorResumeMIC),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20is_resumption_X3D_X25i),
    /* K25  */  be_nested_str_weak(session),
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
    /* K42  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s1rk_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K44  */  be_nested_str_weak(tohex),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20tag_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20Resume1MICPayload_X20_X3D_X20),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20decrypted_tag_X20_X20_X20_X20_X20_X3D_X20),
    /* K48  */  be_nested_str_weak(_source_node_id),
    /* K49  */  be_nested_str_weak(source_node_id),
    /* K50  */  be_nested_str_weak(set_mode_CASE),
    /* K51  */  be_nested_str_weak(__future_initiator_session_id),
    /* K52  */  be_nested_str_weak(initiator_session_id),
    /* K53  */  be_nested_str_weak(__future_local_session_id),
    /* K54  */  be_nested_str_weak(gen_local_session_id),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K56  */  be_nested_str_weak(remote_ip),
    /* K57  */  be_nested_str_weak(remote_port),
    /* K58  */  be_nested_str_weak(resumption_id),
    /* K59  */  be_nested_str_weak(random),
    /* K60  */  be_nested_str_weak(Sigma2_Resume),
    /* K61  */  be_nested_str_weak(NCASE_SigmaS2),
    /* K62  */  be_nested_str_weak(Sigma2Resume),
    /* K63  */  be_nested_str_weak(responderSessionID),
    /* K64  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K65  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2rk_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2rk_salt_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K67  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20new_resumption_id_X20_X3D_X20),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20responderSessionID_X3D_X20),
    /* K69  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20sigma2ResumeMIC_X20_X20_X20_X3D_X20),
    /* K70  */  be_nested_str_weak(SessionResumptionKeys),
    /* K71  */  be_nested_str_weak(rtc),
    /* K72  */  be_nested_str_weak(utc),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K76  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K77  */  be_nested_str_weak(tlv2raw),
    /* K78  */  be_nested_str_weak(__Msg1),
    /* K79  */  be_nested_str_weak(MTR_X3A_X20sigma2resume_X3A_X20),
    /* K80  */  be_nested_str_weak(MTR_X3A_X20sigma2resume_raw_X3A_X20),
    /* K81  */  be_nested_str_weak(build_response),
    /* K82  */  be_nested_str_weak(encode_frame),
    /* K83  */  be_nested_str_weak(responder),
    /* K84  */  be_nested_str_weak(send_response_frame),
    /* K85  */  be_nested_str_weak(close),
    /* K86  */  be_nested_str_weak(set_keys),
    /* K87  */  be_nested_str_weak(_breadcrumb),
    /* K88  */  be_nested_str_weak(counter_snd_next),
    /* K89  */  be_nested_str_weak(set_persist),
    /* K90  */  be_nested_str_weak(set_no_expiration),
    /* K91  */  be_nested_str_weak(persist_to_fabric),
    /* K92  */  be_nested_str_weak(save),
    /* K93  */  be_nested_str_weak(find_fabric_by_destination_id),
    /* K94  */  be_nested_str_weak(destinationId),
    /* K95  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K96  */  be_nested_str_weak(future_local_session_id),
    /* K97  */  be_nested_str_weak(MTR_X3A_X20fabric_X3D),
    /* K98  */  be_nested_str_weak(MTR_X3A_X20no_private_key_X3D),
    /* K99  */  be_nested_str_weak(no_private_key),
    /* K100 */  be_nested_str_weak(MTR_X3A_X20noc_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K101 */  be_nested_str_weak(noc),
    /* K102 */  be_nested_str_weak(get_icac),
    /* K103 */  be_nested_str_weak(MTR_X3A_X20icac_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K104 */  be_nested_str_weak(MTR_X3A_X20root_ca_cert_X20_X20_X3D),
    /* K105 */  be_nested_str_weak(root_ca_certificate),
    /* K106 */  be_nested_str_weak(ResponderEph_priv),
    /* K107 */  be_nested_str_weak(ResponderEph_pub),
    /* K108 */  be_nested_str_weak(EC_P256),
    /* K109 */  be_nested_str_weak(public_key),
    /* K110 */  be_nested_str_weak(MTR_X3A_X20ResponderEph_priv_X20_X20_X3D),
    /* K111 */  be_nested_str_weak(MTR_X3A_X20ResponderEph_pub_X20_X20_X3D),
    /* K112 */  be_nested_str_weak(shared_key),
    /* K113 */  be_nested_str_weak(TLV),
    /* K114 */  be_nested_str_weak(Matter_TLV_struct),
    /* K115 */  be_nested_str_weak(add_TLV),
    /* K116 */  be_nested_str_weak(B2),
    /* K117 */  be_nested_str_weak(get_noc),
    /* K118 */  be_const_int(3),
    /* K119 */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K120 */  be_nested_str_weak(get_pk),
    /* K121 */  be_nested_str_weak(Msg1),
    /* K122 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20resumptionid_X20_X20_X3D_X20),
    /* K123 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20MSG1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K124 */  be_nested_str_weak(SHA256),
    /* K125 */  be_nested_str_weak(update),
    /* K126 */  be_nested_str_weak(out),
    /* K127 */  be_nested_str_weak(MTR_X3A_X20TranscriptHash_X20_X3D),
    /* K128 */  be_nested_str_weak(S2K_Info),
    /* K129 */  be_nested_str_weak(get_ipk_group_key),
    /* K130 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20SharedSecret_X20_X20_X3D_X20),
    /* K131 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K132 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K133 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Raw_X20_X20_X20_X3D_X20),
    /* K134 */  be_nested_str_weak(TBEData2_Nonce),
    /* K135 */  be_nested_str_weak(encrypt),
    /* K136 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Enc_X20_X20_X20_X3D_X20),
    /* K137 */  be_nested_str_weak(Sigma2),
    /* K138 */  be_nested_str_weak(responderRandom),
    /* K139 */  be_nested_str_weak(responderSessionId),
    /* K140 */  be_nested_str_weak(responderEphPubKey),
    /* K141 */  be_nested_str_weak(encrypted2),
    /* K142 */  be_nested_str_weak(MTR_X3A_X20sigma2_X3A_X20),
    /* K143 */  be_nested_str_weak(__Msg2),
    /* K144 */  be_nested_str_weak(MTR_X3A_X20sigma2_raw_X3A_X20),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[795]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
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
      0x58240009,  //  0015  LDCONST	R9	K9
      0x50280000,  //  0016  LDBOOL	R10	0	0
      0x7C100C00,  //  0017  CALL	R4	6
      0x50140000,  //  0018  LDBOOL	R5	0	0
      0x80040A00,  //  0019  RET	1	R5
      0xB8121800,  //  001A  GETNGBL	R4	K12
      0x8C10090D,  //  001B  GETMET	R4	R4	K13
      0x7C100200,  //  001C  CALL	R4	1
      0x8C10090E,  //  001D  GETMET	R4	R4	K14
      0x8818030F,  //  001E  GETMBR	R6	R1	K15
      0x881C0310,  //  001F  GETMBR	R7	R1	K16
      0x7C100600,  //  0020  CALL	R4	3
      0xB8160C00,  //  0021  GETNGBL	R5	K6
      0x8C140B07,  //  0022  GETMET	R5	R5	K7
      0x8C1C0711,  //  0023  GETMET	R7	R3	K17
      0x58240012,  //  0024  LDCONST	R9	K18
      0xB82A1800,  //  0025  GETNGBL	R10	K12
      0x8C281513,  //  0026  GETMET	R10	R10	K19
      0x5C300800,  //  0027  MOVE	R12	R4
      0x7C280400,  //  0028  CALL	R10	2
      0x7C1C0600,  //  0029  CALL	R7	3
      0x54220003,  //  002A  LDINT	R8	4
      0x7C140600,  //  002B  CALL	R5	3
      0x88140915,  //  002C  GETMBR	R5	R4	K21
      0x90022805,  //  002D  SETMBR	R0	K20	R5
      0x88140916,  //  002E  GETMBR	R5	R4	K22
      0x4C180000,  //  002F  LDNIL	R6
      0x20140A06,  //  0030  NE	R5	R5	R6
      0x78160003,  //  0031  JMPF	R5	#0036
      0x88140917,  //  0032  GETMBR	R5	R4	K23
      0x4C180000,  //  0033  LDNIL	R6
      0x20140A06,  //  0034  NE	R5	R5	R6
      0x74160000,  //  0035  JMPT	R5	#0037
      0x50140001,  //  0036  LDBOOL	R5	0	1
      0x50140200,  //  0037  LDBOOL	R5	1	0
      0xB81A0C00,  //  0038  GETNGBL	R6	K6
      0x8C180D07,  //  0039  GETMET	R6	R6	K7
      0x8C200711,  //  003A  GETMET	R8	R3	K17
      0x58280018,  //  003B  LDCONST	R10	K24
      0x78160001,  //  003C  JMPF	R5	#003F
      0x582C000B,  //  003D  LDCONST	R11	K11
      0x70020000,  //  003E  JMP		#0040
      0x582C0004,  //  003F  LDCONST	R11	K4
      0x7C200600,  //  0040  CALL	R8	3
      0x54260003,  //  0041  LDINT	R9	4
      0x7C180600,  //  0042  CALL	R6	3
      0x50140000,  //  0043  LDBOOL	R5	0	0
      0x88180319,  //  0044  GETMBR	R6	R1	K25
      0x4C1C0000,  //  0045  LDNIL	R7
      0x7816001C,  //  0046  JMPF	R5	#0064
      0x8820011A,  //  0047  GETMBR	R8	R0	K26
      0x8820111B,  //  0048  GETMBR	R8	R8	K27
      0x8C20111C,  //  0049  GETMET	R8	R8	K28
      0x88280916,  //  004A  GETMBR	R10	R4	K22
      0x7C200400,  //  004B  CALL	R8	2
      0x5C1C1000,  //  004C  MOVE	R7	R8
      0xB8220C00,  //  004D  GETNGBL	R8	K6
      0x8C201107,  //  004E  GETMET	R8	R8	K7
      0x8C280711,  //  004F  GETMET	R10	R3	K17
      0x5830001D,  //  0050  LDCONST	R12	K29
      0xB8361800,  //  0051  GETNGBL	R13	K12
      0x8C341B13,  //  0052  GETMET	R13	R13	K19
      0x5C3C0C00,  //  0053  MOVE	R15	R6
      0x7C340400,  //  0054  CALL	R13	2
      0xB83A1800,  //  0055  GETNGBL	R14	K12
      0x8C381D13,  //  0056  GETMET	R14	R14	K19
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
      0x50140000,  //  0063  LDBOOL	R5	0	0
      0x7816013A,  //  0064  JMPF	R5	#01A0
      0x8820091F,  //  0065  GETMBR	R8	R4	K31
      0x88240916,  //  0066  GETMBR	R9	R4	K22
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
      0x4032080C,  //  007B  CONNECT	R12	K4	R12
      0x88340917,  //  007C  GETMBR	R13	R4	K23
      0x94301A0C,  //  007D  GETIDX	R12	R13	R12
      0x5439FFEF,  //  007E  LDINT	R14	-16
      0x40381D26,  //  007F  CONNECT	R14	R14	K38
      0x883C0917,  //  0080  GETMBR	R15	R4	K23
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
      0xB8460C00,  //  0093  GETNGBL	R17	K6
      0x8C442307,  //  0094  GETMET	R17	R17	K7
      0x584C002A,  //  0095  LDCONST	R19	K42
      0x54520003,  //  0096  LDINT	R20	4
      0x7C440600,  //  0097  CALL	R17	3
      0xB8460C00,  //  0098  GETNGBL	R17	K6
      0x8C442307,  //  0099  GETMET	R17	R17	K7
      0x8C4C152C,  //  009A  GETMET	R19	R10	K44
      0x7C4C0200,  //  009B  CALL	R19	1
      0x004E5613,  //  009C  ADD	R19	K43	R19
      0x54520003,  //  009D  LDINT	R20	4
      0x7C440600,  //  009E  CALL	R17	3
      0xB8460C00,  //  009F  GETNGBL	R17	K6
      0x8C442307,  //  00A0  GETMET	R17	R17	K7
      0x8C4C1B2C,  //  00A1  GETMET	R19	R13	K44
      0x7C4C0200,  //  00A2  CALL	R19	1
      0x004E5A13,  //  00A3  ADD	R19	K45	R19
      0x54520003,  //  00A4  LDINT	R20	4
      0x7C440600,  //  00A5  CALL	R17	3
      0xB8460C00,  //  00A6  GETNGBL	R17	K6
      0x8C442307,  //  00A7  GETMET	R17	R17	K7
      0x8C4C1F2C,  //  00A8  GETMET	R19	R15	K44
      0x7C4C0200,  //  00A9  CALL	R19	1
      0x004E5C13,  //  00AA  ADD	R19	K46	R19
      0x54520003,  //  00AB  LDINT	R20	4
      0x7C440600,  //  00AC  CALL	R17	3
      0xB8460C00,  //  00AD  GETNGBL	R17	K6
      0x8C442307,  //  00AE  GETMET	R17	R17	K7
      0x8C4C212C,  //  00AF  GETMET	R19	R16	K44
      0x7C4C0200,  //  00B0  CALL	R19	1
      0x004E5E13,  //  00B1  ADD	R19	K47	R19
      0x54520003,  //  00B2  LDINT	R20	4
      0x7C440600,  //  00B3  CALL	R17	3
      0xB8460C00,  //  00B4  GETNGBL	R17	K6
      0x8C442307,  //  00B5  GETMET	R17	R17	K7
      0x584C002A,  //  00B6  LDCONST	R19	K42
      0x54520003,  //  00B7  LDINT	R20	4
      0x7C440600,  //  00B8  CALL	R17	3
      0x1C441A10,  //  00B9  EQ	R17	R13	R16
      0x784600E3,  //  00BA  JMPF	R17	#019F
      0x88440F1E,  //  00BB  GETMBR	R17	R7	K30
      0x901A3C11,  //  00BC  SETMBR	R6	K30	R17
      0x88440331,  //  00BD  GETMBR	R17	R1	K49
      0x901A6011,  //  00BE  SETMBR	R6	K48	R17
      0x8C440D32,  //  00BF  GETMET	R17	R6	K50
      0x7C440200,  //  00C0  CALL	R17	1
      0x88440934,  //  00C1  GETMBR	R17	R4	K52
      0x901A6611,  //  00C2  SETMBR	R6	K51	R17
      0x8844011A,  //  00C3  GETMBR	R17	R0	K26
      0x8844231B,  //  00C4  GETMBR	R17	R17	K27
      0x8C442336,  //  00C5  GETMET	R17	R17	K54
      0x7C440200,  //  00C6  CALL	R17	1
      0x901A6A11,  //  00C7  SETMBR	R6	K53	R17
      0xB8460C00,  //  00C8  GETNGBL	R17	K6
      0x8C442307,  //  00C9  GETMET	R17	R17	K7
      0x8C4C0711,  //  00CA  GETMET	R19	R3	K17
      0x58540037,  //  00CB  LDCONST	R21	K55
      0x88580D35,  //  00CC  GETMBR	R22	R6	K53
      0x885C0338,  //  00CD  GETMBR	R23	R1	K56
      0x88600339,  //  00CE  GETMBR	R24	R1	K57
      0x7C4C0A00,  //  00CF  CALL	R19	5
      0x58500009,  //  00D0  LDCONST	R20	K9
      0x7C440600,  //  00D1  CALL	R17	3
      0x88440F24,  //  00D2  GETMBR	R17	R7	K36
      0x901A4811,  //  00D3  SETMBR	R6	K36	R17
      0x8C44053B,  //  00D4  GETMET	R17	R2	K59
      0x544E000F,  //  00D5  LDINT	R19	16
      0x7C440400,  //  00D6  CALL	R17	2
      0x901A7411,  //  00D7  SETMBR	R6	K58	R17
      0x60440015,  //  00D8  GETGBL	R17	G21
      0x7C440000,  //  00D9  CALL	R17	0
      0x8C442320,  //  00DA  GETMET	R17	R17	K32
      0x584C003C,  //  00DB  LDCONST	R19	K60
      0x7C440400,  //  00DC  CALL	R17	2
      0x8848091F,  //  00DD  GETMBR	R18	R4	K31
      0x884C0D3A,  //  00DE  GETMBR	R19	R6	K58
      0x00482413,  //  00DF  ADD	R18	R18	R19
      0x8C4C0522,  //  00E0  GETMET	R19	R2	K34
      0x7C4C0200,  //  00E1  CALL	R19	1
      0x8C4C2723,  //  00E2  GETMET	R19	R19	K35
      0x88540D24,  //  00E3  GETMBR	R21	R6	K36
      0x5C582400,  //  00E4  MOVE	R22	R18
      0x5C5C2200,  //  00E5  MOVE	R23	R17
      0x5462000F,  //  00E6  LDINT	R24	16
      0x7C4C0A00,  //  00E7  CALL	R19	5
      0x8C500527,  //  00E8  GETMET	R20	R2	K39
      0x5C582600,  //  00E9  MOVE	R22	R19
      0x605C0015,  //  00EA  GETGBL	R23	G21
      0x7C5C0000,  //  00EB  CALL	R23	0
      0x8C5C2F20,  //  00EC  GETMET	R23	R23	K32
      0x5864003D,  //  00ED  LDCONST	R25	K61
      0x7C5C0400,  //  00EE  CALL	R23	2
      0x60600015,  //  00EF  GETGBL	R24	G21
      0x7C600000,  //  00F0  CALL	R24	0
      0x58640004,  //  00F1  LDCONST	R25	K4
      0x546A000F,  //  00F2  LDINT	R26	16
      0x7C500C00,  //  00F3  CALL	R20	6
      0x8C542929,  //  00F4  GETMET	R21	R20	K41
      0x7C540200,  //  00F5  CALL	R21	1
      0xB85A1800,  //  00F6  GETNGBL	R22	K12
      0x8C582D3E,  //  00F7  GETMET	R22	R22	K62
      0x7C580200,  //  00F8  CALL	R22	1
      0x885C0D3A,  //  00F9  GETMBR	R23	R6	K58
      0x905A2C17,  //  00FA  SETMBR	R22	K22	R23
      0x885C0D35,  //  00FB  GETMBR	R23	R6	K53
      0x905A7E17,  //  00FC  SETMBR	R22	K63	R23
      0x905A8015,  //  00FD  SETMBR	R22	K64	R21
      0xB85E0C00,  //  00FE  GETNGBL	R23	K6
      0x8C5C2F07,  //  00FF  GETMET	R23	R23	K7
      0x5864002A,  //  0100  LDCONST	R25	K42
      0x546A0003,  //  0101  LDINT	R26	4
      0x7C5C0600,  //  0102  CALL	R23	3
      0xB85E0C00,  //  0103  GETNGBL	R23	K6
      0x8C5C2F07,  //  0104  GETMET	R23	R23	K7
      0x8C64272C,  //  0105  GETMET	R25	R19	K44
      0x7C640200,  //  0106  CALL	R25	1
      0x00668219,  //  0107  ADD	R25	K65	R25
      0x546A0003,  //  0108  LDINT	R26	4
      0x7C5C0600,  //  0109  CALL	R23	3
      0xB85E0C00,  //  010A  GETNGBL	R23	K6
      0x8C5C2F07,  //  010B  GETMET	R23	R23	K7
      0x8C64252C,  //  010C  GETMET	R25	R18	K44
      0x7C640200,  //  010D  CALL	R25	1
      0x00668419,  //  010E  ADD	R25	K66	R25
      0x546A0003,  //  010F  LDINT	R26	4
      0x7C5C0600,  //  0110  CALL	R23	3
      0xB85E0C00,  //  0111  GETNGBL	R23	K6
      0x8C5C2F07,  //  0112  GETMET	R23	R23	K7
      0x88640D3A,  //  0113  GETMBR	R25	R6	K58
      0x8C64332C,  //  0114  GETMET	R25	R25	K44
      0x7C640200,  //  0115  CALL	R25	1
      0x00668619,  //  0116  ADD	R25	K67	R25
      0x546A0003,  //  0117  LDINT	R26	4
      0x7C5C0600,  //  0118  CALL	R23	3
      0xB85E0C00,  //  0119  GETNGBL	R23	K6
      0x8C5C2F07,  //  011A  GETMET	R23	R23	K7
      0x60640008,  //  011B  GETGBL	R25	G8
      0x88680D35,  //  011C  GETMBR	R26	R6	K53
      0x7C640200,  //  011D  CALL	R25	1
      0x00668819,  //  011E  ADD	R25	K68	R25
      0x546A0003,  //  011F  LDINT	R26	4
      0x7C5C0600,  //  0120  CALL	R23	3
      0xB85E0C00,  //  0121  GETNGBL	R23	K6
      0x8C5C2F07,  //  0122  GETMET	R23	R23	K7
      0x8C642B2C,  //  0123  GETMET	R25	R21	K44
      0x7C640200,  //  0124  CALL	R25	1
      0x00668A19,  //  0125  ADD	R25	K69	R25
      0x546A0003,  //  0126  LDINT	R26	4
      0x7C5C0600,  //  0127  CALL	R23	3
      0xB85E0C00,  //  0128  GETNGBL	R23	K6
      0x8C5C2F07,  //  0129  GETMET	R23	R23	K7
      0x5864002A,  //  012A  LDCONST	R25	K42
      0x546A0003,  //  012B  LDINT	R26	4
      0x7C5C0600,  //  012C  CALL	R23	3
      0x8C5C0522,  //  012D  GETMET	R23	R2	K34
      0x7C5C0200,  //  012E  CALL	R23	1
      0x8C5C2F23,  //  012F  GETMET	R23	R23	K35
      0x88640D24,  //  0130  GETMBR	R25	R6	K36
      0x8868091F,  //  0131  GETMBR	R26	R4	K31
      0x886C0D3A,  //  0132  GETMBR	R27	R6	K58
      0x0068341B,  //  0133  ADD	R26	R26	R27
      0x606C0015,  //  0134  GETGBL	R27	G21
      0x7C6C0000,  //  0135  CALL	R27	0
      0x8C6C3720,  //  0136  GETMET	R27	R27	K32
      0x58740046,  //  0137  LDCONST	R29	K70
      0x7C6C0400,  //  0138  CALL	R27	2
      0x5472002F,  //  0139  LDINT	R28	48
      0x7C5C0A00,  //  013A  CALL	R23	5
      0x5462000E,  //  013B  LDINT	R24	15
      0x40620818,  //  013C  CONNECT	R24	K4	R24
      0x94602E18,  //  013D  GETIDX	R24	R23	R24
      0x5466000F,  //  013E  LDINT	R25	16
      0x546A001E,  //  013F  LDINT	R26	31
      0x4064321A,  //  0140  CONNECT	R25	R25	R26
      0x94642E19,  //  0141  GETIDX	R25	R23	R25
      0x546A001F,  //  0142  LDINT	R26	32
      0x546E002E,  //  0143  LDINT	R27	47
      0x4068341B,  //  0144  CONNECT	R26	R26	R27
      0x94682E1A,  //  0145  GETIDX	R26	R23	R26
      0xB86E0C00,  //  0146  GETNGBL	R27	K6
      0x8C6C3747,  //  0147  GETMET	R27	R27	K71
      0x7C6C0200,  //  0148  CALL	R27	1
      0x946C3748,  //  0149  GETIDX	R27	R27	K72
      0xB8720C00,  //  014A  GETNGBL	R28	K6
      0x8C703907,  //  014B  GETMET	R28	R28	K7
      0x58780049,  //  014C  LDCONST	R30	K73
      0x547E0003,  //  014D  LDINT	R31	4
      0x7C700600,  //  014E  CALL	R28	3
      0xB8720C00,  //  014F  GETNGBL	R28	K6
      0x8C703907,  //  0150  GETMET	R28	R28	K7
      0x8C78312C,  //  0151  GETMET	R30	R24	K44
      0x7C780200,  //  0152  CALL	R30	1
      0x007A941E,  //  0153  ADD	R30	K74	R30
      0x547E0003,  //  0154  LDINT	R31	4
      0x7C700600,  //  0155  CALL	R28	3
      0xB8720C00,  //  0156  GETNGBL	R28	K6
      0x8C703907,  //  0157  GETMET	R28	R28	K7
      0x8C78332C,  //  0158  GETMET	R30	R25	K44
      0x7C780200,  //  0159  CALL	R30	1
      0x007A961E,  //  015A  ADD	R30	K75	R30
      0x547E0003,  //  015B  LDINT	R31	4
      0x7C700600,  //  015C  CALL	R28	3
      0xB8720C00,  //  015D  GETNGBL	R28	K6
      0x8C703907,  //  015E  GETMET	R28	R28	K7
      0x8C78352C,  //  015F  GETMET	R30	R26	K44
      0x7C780200,  //  0160  CALL	R30	1
      0x007A981E,  //  0161  ADD	R30	K76	R30
      0x547E0003,  //  0162  LDINT	R31	4
      0x7C700600,  //  0163  CALL	R28	3
      0xB8720C00,  //  0164  GETNGBL	R28	K6
      0x8C703907,  //  0165  GETMET	R28	R28	K7
      0x58780049,  //  0166  LDCONST	R30	K73
      0x547E0003,  //  0167  LDINT	R31	4
      0x7C700600,  //  0168  CALL	R28	3
      0x8C702D4D,  //  0169  GETMET	R28	R22	K77
      0x7C700200,  //  016A  CALL	R28	1
      0x4C740000,  //  016B  LDNIL	R29
      0x901A9C1D,  //  016C  SETMBR	R6	K78	R29
      0xB8760C00,  //  016D  GETNGBL	R29	K6
      0x8C743B07,  //  016E  GETMET	R29	R29	K7
      0xB87E1800,  //  016F  GETNGBL	R31	K12
      0x8C7C3F13,  //  0170  GETMET	R31	R31	K19
      0x5C842C00,  //  0171  MOVE	R33	R22
      0x7C7C0400,  //  0172  CALL	R31	2
      0x007E9E1F,  //  0173  ADD	R31	K79	R31
      0x54820003,  //  0174  LDINT	R32	4
      0x7C740600,  //  0175  CALL	R29	3
      0xB8760C00,  //  0176  GETNGBL	R29	K6
      0x8C743B07,  //  0177  GETMET	R29	R29	K7
      0x8C7C392C,  //  0178  GETMET	R31	R28	K44
      0x7C7C0200,  //  0179  CALL	R31	1
      0x007EA01F,  //  017A  ADD	R31	K80	R31
      0x54820003,  //  017B  LDINT	R32	4
      0x7C740600,  //  017C  CALL	R29	3
      0x8C740351,  //  017D  GETMET	R29	R1	K81
      0x547E0032,  //  017E  LDINT	R31	51
      0x50800200,  //  017F  LDBOOL	R32	1	0
      0x7C740600,  //  0180  CALL	R29	3
      0x8C783B52,  //  0181  GETMET	R30	R29	K82
      0x5C803800,  //  0182  MOVE	R32	R28
      0x7C780400,  //  0183  CALL	R30	2
      0x887C0153,  //  0184  GETMBR	R31	R0	K83
      0x8C7C3F54,  //  0185  GETMET	R31	R31	K84
      0x5C843A00,  //  0186  MOVE	R33	R29
      0x7C7C0400,  //  0187  CALL	R31	2
      0x8C7C0D55,  //  0188  GETMET	R31	R6	K85
      0x7C7C0200,  //  0189  CALL	R31	1
      0x8C7C0D56,  //  018A  GETMET	R31	R6	K86
      0x5C843000,  //  018B  MOVE	R33	R24
      0x5C883200,  //  018C  MOVE	R34	R25
      0x5C8C3400,  //  018D  MOVE	R35	R26
      0x5C903600,  //  018E  MOVE	R36	R27
      0x7C7C0A00,  //  018F  CALL	R31	5
      0x901AAF04,  //  0190  SETMBR	R6	K87	K4
      0x8C7C0D58,  //  0191  GETMET	R31	R6	K88
      0x7C7C0200,  //  0192  CALL	R31	1
      0x8C7C0D59,  //  0193  GETMET	R31	R6	K89
      0x50840200,  //  0194  LDBOOL	R33	1	0
      0x7C7C0400,  //  0195  CALL	R31	2
      0x8C7C0D5A,  //  0196  GETMET	R31	R6	K90
      0x7C7C0200,  //  0197  CALL	R31	1
      0x8C7C0D5B,  //  0198  GETMET	R31	R6	K91
      0x7C7C0200,  //  0199  CALL	R31	1
      0x8C7C0D5C,  //  019A  GETMET	R31	R6	K92
      0x7C7C0200,  //  019B  CALL	R31	1
      0x507C0200,  //  019C  LDBOOL	R31	1	0
      0x80043E00,  //  019D  RET	1	R31
      0x70020000,  //  019E  JMP		#01A0
      0x50140000,  //  019F  LDBOOL	R5	0	0
      0x5C200A00,  //  01A0  MOVE	R8	R5
      0x74220176,  //  01A1  JMPT	R8	#0319
      0x8C20015D,  //  01A2  GETMET	R8	R0	K93
      0x8828095E,  //  01A3  GETMBR	R10	R4	K94
      0x882C091F,  //  01A4  GETMBR	R11	R4	K31
      0x7C200600,  //  01A5  CALL	R8	3
      0x901A3C08,  //  01A6  SETMBR	R6	K30	R8
      0x4C240000,  //  01A7  LDNIL	R9
      0x1C240C09,  //  01A8  EQ	R9	R6	R9
      0x74260003,  //  01A9  JMPT	R9	#01AE
      0x88240D1E,  //  01AA  GETMBR	R9	R6	K30
      0x4C280000,  //  01AB  LDNIL	R10
      0x1C24120A,  //  01AC  EQ	R9	R9	R10
      0x7826000D,  //  01AD  JMPF	R9	#01BC
      0xB8260C00,  //  01AE  GETNGBL	R9	K6
      0x8C241307,  //  01AF  GETMET	R9	R9	K7
      0x582C005F,  //  01B0  LDCONST	R11	K95
      0x58300009,  //  01B1  LDCONST	R12	K9
      0x7C240600,  //  01B2  CALL	R9	3
      0x8C24010A,  //  01B3  GETMET	R9	R0	K10
      0x5C2C0200,  //  01B4  MOVE	R11	R1
      0x5830000B,  //  01B5  LDCONST	R12	K11
      0x58340004,  //  01B6  LDCONST	R13	K4
      0x5838000B,  //  01B7  LDCONST	R14	K11
      0x503C0000,  //  01B8  LDBOOL	R15	0	0
      0x7C240C00,  //  01B9  CALL	R9	6
      0x50280000,  //  01BA  LDBOOL	R10	0	0
      0x80041400,  //  01BB  RET	1	R10
      0x88240331,  //  01BC  GETMBR	R9	R1	K49
      0x901A6009,  //  01BD  SETMBR	R6	K48	R9
      0x8C240D32,  //  01BE  GETMET	R9	R6	K50
      0x7C240200,  //  01BF  CALL	R9	1
      0x88240934,  //  01C0  GETMBR	R9	R4	K52
      0x901A6609,  //  01C1  SETMBR	R6	K51	R9
      0x8824011A,  //  01C2  GETMBR	R9	R0	K26
      0x8824131B,  //  01C3  GETMBR	R9	R9	K27
      0x8C241336,  //  01C4  GETMET	R9	R9	K54
      0x7C240200,  //  01C5  CALL	R9	1
      0x901A6A09,  //  01C6  SETMBR	R6	K53	R9
      0x88240D35,  //  01C7  GETMBR	R9	R6	K53
      0x9002C009,  //  01C8  SETMBR	R0	K96	R9
      0xB8260C00,  //  01C9  GETNGBL	R9	K6
      0x8C241307,  //  01CA  GETMET	R9	R9	K7
      0x8C2C0711,  //  01CB  GETMET	R11	R3	K17
      0x58340037,  //  01CC  LDCONST	R13	K55
      0x88380160,  //  01CD  GETMBR	R14	R0	K96
      0x883C0338,  //  01CE  GETMBR	R15	R1	K56
      0x88400339,  //  01CF  GETMBR	R16	R1	K57
      0x7C2C0A00,  //  01D0  CALL	R11	5
      0x58300009,  //  01D1  LDCONST	R12	K9
      0x7C240600,  //  01D2  CALL	R9	3
      0xB8260C00,  //  01D3  GETNGBL	R9	K6
      0x8C241307,  //  01D4  GETMET	R9	R9	K7
      0xB82E1800,  //  01D5  GETNGBL	R11	K12
      0x8C2C1713,  //  01D6  GETMET	R11	R11	K19
      0x88340D1E,  //  01D7  GETMBR	R13	R6	K30
      0x7C2C0400,  //  01D8  CALL	R11	2
      0x002EC20B,  //  01D9  ADD	R11	K97	R11
      0x54320003,  //  01DA  LDINT	R12	4
      0x7C240600,  //  01DB  CALL	R9	3
      0xB8260C00,  //  01DC  GETNGBL	R9	K6
      0x8C241307,  //  01DD  GETMET	R9	R9	K7
      0x882C0D1E,  //  01DE  GETMBR	R11	R6	K30
      0x882C1763,  //  01DF  GETMBR	R11	R11	K99
      0x8C2C172C,  //  01E0  GETMET	R11	R11	K44
      0x7C2C0200,  //  01E1  CALL	R11	1
      0x002EC40B,  //  01E2  ADD	R11	K98	R11
      0x54320003,  //  01E3  LDINT	R12	4
      0x7C240600,  //  01E4  CALL	R9	3
      0xB8260C00,  //  01E5  GETNGBL	R9	K6
      0x8C241307,  //  01E6  GETMET	R9	R9	K7
      0x882C0D1E,  //  01E7  GETMBR	R11	R6	K30
      0x882C1765,  //  01E8  GETMBR	R11	R11	K101
      0x8C2C172C,  //  01E9  GETMET	R11	R11	K44
      0x7C2C0200,  //  01EA  CALL	R11	1
      0x002EC80B,  //  01EB  ADD	R11	K100	R11
      0x54320003,  //  01EC  LDINT	R12	4
      0x7C240600,  //  01ED  CALL	R9	3
      0x88240D1E,  //  01EE  GETMBR	R9	R6	K30
      0x8C241366,  //  01EF  GETMET	R9	R9	K102
      0x7C240200,  //  01F0  CALL	R9	1
      0x78260009,  //  01F1  JMPF	R9	#01FC
      0xB8260C00,  //  01F2  GETNGBL	R9	K6
      0x8C241307,  //  01F3  GETMET	R9	R9	K7
      0x882C0D1E,  //  01F4  GETMBR	R11	R6	K30
      0x8C2C1766,  //  01F5  GETMET	R11	R11	K102
      0x7C2C0200,  //  01F6  CALL	R11	1
      0x8C2C172C,  //  01F7  GETMET	R11	R11	K44
      0x7C2C0200,  //  01F8  CALL	R11	1
      0x002ECE0B,  //  01F9  ADD	R11	K103	R11
      0x54320003,  //  01FA  LDINT	R12	4
      0x7C240600,  //  01FB  CALL	R9	3
      0xB8260C00,  //  01FC  GETNGBL	R9	K6
      0x8C241307,  //  01FD  GETMET	R9	R9	K7
      0x882C0D1E,  //  01FE  GETMBR	R11	R6	K30
      0x882C1769,  //  01FF  GETMBR	R11	R11	K105
      0x8C2C172C,  //  0200  GETMET	R11	R11	K44
      0x7C2C0200,  //  0201  CALL	R11	1
      0x002ED00B,  //  0202  ADD	R11	K104	R11
      0x54320003,  //  0203  LDINT	R12	4
      0x7C240600,  //  0204  CALL	R9	3
      0x8C24053B,  //  0205  GETMET	R9	R2	K59
      0x542E000F,  //  0206  LDINT	R11	16
      0x7C240400,  //  0207  CALL	R9	2
      0x901A7409,  //  0208  SETMBR	R6	K58	R9
      0x8C24053B,  //  0209  GETMET	R9	R2	K59
      0x542E001F,  //  020A  LDINT	R11	32
      0x7C240400,  //  020B  CALL	R9	2
      0x9002D409,  //  020C  SETMBR	R0	K106	R9
      0x8C24056C,  //  020D  GETMET	R9	R2	K108
      0x7C240200,  //  020E  CALL	R9	1
      0x8C24136D,  //  020F  GETMET	R9	R9	K109
      0x882C016A,  //  0210  GETMBR	R11	R0	K106
      0x7C240400,  //  0211  CALL	R9	2
      0x9002D609,  //  0212  SETMBR	R0	K107	R9
      0xB8260C00,  //  0213  GETNGBL	R9	K6
      0x8C241307,  //  0214  GETMET	R9	R9	K7
      0x882C016A,  //  0215  GETMBR	R11	R0	K106
      0x8C2C172C,  //  0216  GETMET	R11	R11	K44
      0x7C2C0200,  //  0217  CALL	R11	1
      0x002EDC0B,  //  0218  ADD	R11	K110	R11
      0x54320003,  //  0219  LDINT	R12	4
      0x7C240600,  //  021A  CALL	R9	3
      0xB8260C00,  //  021B  GETNGBL	R9	K6
      0x8C241307,  //  021C  GETMET	R9	R9	K7
      0x882C016B,  //  021D  GETMBR	R11	R0	K107
      0x8C2C172C,  //  021E  GETMET	R11	R11	K44
      0x7C2C0200,  //  021F  CALL	R11	1
      0x002EDE0B,  //  0220  ADD	R11	K111	R11
      0x54320003,  //  0221  LDINT	R12	4
      0x7C240600,  //  0222  CALL	R9	3
      0x8C24053B,  //  0223  GETMET	R9	R2	K59
      0x542E001F,  //  0224  LDINT	R11	32
      0x7C240400,  //  0225  CALL	R9	2
      0x8C28056C,  //  0226  GETMET	R10	R2	K108
      0x7C280200,  //  0227  CALL	R10	1
      0x8C281570,  //  0228  GETMET	R10	R10	K112
      0x8830016A,  //  0229  GETMBR	R12	R0	K106
      0x88340915,  //  022A  GETMBR	R13	R4	K21
      0x7C280600,  //  022B  CALL	R10	3
      0x901A480A,  //  022C  SETMBR	R6	K36	R10
      0xB82A1800,  //  022D  GETNGBL	R10	K12
      0x88281571,  //  022E  GETMBR	R10	R10	K113
      0x8C281572,  //  022F  GETMET	R10	R10	K114
      0x7C280200,  //  0230  CALL	R10	1
      0x8C2C1573,  //  0231  GETMET	R11	R10	K115
      0x5834000B,  //  0232  LDCONST	R13	K11
      0xB83A1800,  //  0233  GETNGBL	R14	K12
      0x88381D71,  //  0234  GETMBR	R14	R14	K113
      0x88381D74,  //  0235  GETMBR	R14	R14	K116
      0x8C3C0D75,  //  0236  GETMET	R15	R6	K117
      0x7C3C0200,  //  0237  CALL	R15	1
      0x7C2C0800,  //  0238  CALL	R11	4
      0x8C2C1573,  //  0239  GETMET	R11	R10	K115
      0x58340009,  //  023A  LDCONST	R13	K9
      0xB83A1800,  //  023B  GETNGBL	R14	K12
      0x88381D71,  //  023C  GETMBR	R14	R14	K113
      0x88381D74,  //  023D  GETMBR	R14	R14	K116
      0x8C3C0D66,  //  023E  GETMET	R15	R6	K102
      0x7C3C0200,  //  023F  CALL	R15	1
      0x7C2C0800,  //  0240  CALL	R11	4
      0x8C2C1573,  //  0241  GETMET	R11	R10	K115
      0x58340076,  //  0242  LDCONST	R13	K118
      0xB83A1800,  //  0243  GETNGBL	R14	K12
      0x88381D71,  //  0244  GETMBR	R14	R14	K113
      0x88381D74,  //  0245  GETMBR	R14	R14	K116
      0x883C016B,  //  0246  GETMBR	R15	R0	K107
      0x7C2C0800,  //  0247  CALL	R11	4
      0x8C2C1573,  //  0248  GETMET	R11	R10	K115
      0x54360003,  //  0249  LDINT	R13	4
      0xB83A1800,  //  024A  GETNGBL	R14	K12
      0x88381D71,  //  024B  GETMBR	R14	R14	K113
      0x88381D74,  //  024C  GETMBR	R14	R14	K116
      0x883C0915,  //  024D  GETMBR	R15	R4	K21
      0x7C2C0800,  //  024E  CALL	R11	4
      0x8C2C056C,  //  024F  GETMET	R11	R2	K108
      0x7C2C0200,  //  0250  CALL	R11	1
      0x8C2C1777,  //  0251  GETMET	R11	R11	K119
      0x8C340D78,  //  0252  GETMET	R13	R6	K120
      0x7C340200,  //  0253  CALL	R13	1
      0x8C38154D,  //  0254  GETMET	R14	R10	K77
      0x7C380200,  //  0255  CALL	R14	1
      0x7C2C0600,  //  0256  CALL	R11	3
      0xB8321800,  //  0257  GETNGBL	R12	K12
      0x88301971,  //  0258  GETMBR	R12	R12	K113
      0x8C301972,  //  0259  GETMET	R12	R12	K114
      0x7C300200,  //  025A  CALL	R12	1
      0x8C341973,  //  025B  GETMET	R13	R12	K115
      0x583C000B,  //  025C  LDCONST	R15	K11
      0xB8421800,  //  025D  GETNGBL	R16	K12
      0x88402171,  //  025E  GETMBR	R16	R16	K113
      0x88402174,  //  025F  GETMBR	R16	R16	K116
      0x8C440D75,  //  0260  GETMET	R17	R6	K117
      0x7C440200,  //  0261  CALL	R17	1
      0x7C340800,  //  0262  CALL	R13	4
      0x8C341973,  //  0263  GETMET	R13	R12	K115
      0x583C0009,  //  0264  LDCONST	R15	K9
      0xB8421800,  //  0265  GETNGBL	R16	K12
      0x88402171,  //  0266  GETMBR	R16	R16	K113
      0x88402174,  //  0267  GETMBR	R16	R16	K116
      0x8C440D66,  //  0268  GETMET	R17	R6	K102
      0x7C440200,  //  0269  CALL	R17	1
      0x7C340800,  //  026A  CALL	R13	4
      0x8C341973,  //  026B  GETMET	R13	R12	K115
      0x583C0076,  //  026C  LDCONST	R15	K118
      0xB8421800,  //  026D  GETNGBL	R16	K12
      0x88402171,  //  026E  GETMBR	R16	R16	K113
      0x88402174,  //  026F  GETMBR	R16	R16	K116
      0x5C441600,  //  0270  MOVE	R17	R11
      0x7C340800,  //  0271  CALL	R13	4
      0x8C341973,  //  0272  GETMET	R13	R12	K115
      0x543E0003,  //  0273  LDINT	R15	4
      0xB8421800,  //  0274  GETNGBL	R16	K12
      0x88402171,  //  0275  GETMBR	R16	R16	K113
      0x88402174,  //  0276  GETMBR	R16	R16	K116
      0x88440D3A,  //  0277  GETMBR	R17	R6	K58
      0x7C340800,  //  0278  CALL	R13	4
      0xB8360C00,  //  0279  GETNGBL	R13	K6
      0x8C341B07,  //  027A  GETMET	R13	R13	K7
      0x583C002A,  //  027B  LDCONST	R15	K42
      0x54420003,  //  027C  LDINT	R16	4
      0x7C340600,  //  027D  CALL	R13	3
      0x88340979,  //  027E  GETMBR	R13	R4	K121
      0x901A9C0D,  //  027F  SETMBR	R6	K78	R13
      0xB8360C00,  //  0280  GETNGBL	R13	K6
      0x8C341B07,  //  0281  GETMET	R13	R13	K7
      0x883C0D3A,  //  0282  GETMBR	R15	R6	K58
      0x8C3C1F2C,  //  0283  GETMET	R15	R15	K44
      0x7C3C0200,  //  0284  CALL	R15	1
      0x003EF40F,  //  0285  ADD	R15	K122	R15
      0x54420003,  //  0286  LDINT	R16	4
      0x7C340600,  //  0287  CALL	R13	3
      0xB8360C00,  //  0288  GETNGBL	R13	K6
      0x8C341B07,  //  0289  GETMET	R13	R13	K7
      0x883C0D4E,  //  028A  GETMBR	R15	R6	K78
      0x8C3C1F2C,  //  028B  GETMET	R15	R15	K44
      0x7C3C0200,  //  028C  CALL	R15	1
      0x003EF60F,  //  028D  ADD	R15	K123	R15
      0x54420003,  //  028E  LDINT	R16	4
      0x7C340600,  //  028F  CALL	R13	3
      0x8C34057C,  //  0290  GETMET	R13	R2	K124
      0x7C340200,  //  0291  CALL	R13	1
      0x8C341B7D,  //  0292  GETMET	R13	R13	K125
      0x883C0D4E,  //  0293  GETMBR	R15	R6	K78
      0x7C340400,  //  0294  CALL	R13	2
      0x8C341B7E,  //  0295  GETMET	R13	R13	K126
      0x7C340200,  //  0296  CALL	R13	1
      0xB83A0C00,  //  0297  GETNGBL	R14	K6
      0x8C381D07,  //  0298  GETMET	R14	R14	K7
      0x8C401B2C,  //  0299  GETMET	R16	R13	K44
      0x7C400200,  //  029A  CALL	R16	1
      0x0042FE10,  //  029B  ADD	R16	K127	R16
      0x54460003,  //  029C  LDINT	R17	4
      0x7C380600,  //  029D  CALL	R14	3
      0x60380015,  //  029E  GETGBL	R14	G21
      0x7C380000,  //  029F  CALL	R14	0
      0x8C381D20,  //  02A0  GETMET	R14	R14	K32
      0x88400180,  //  02A1  GETMBR	R16	R0	K128
      0x7C380400,  //  02A2  CALL	R14	2
      0x8C3C0D81,  //  02A3  GETMET	R15	R6	K129
      0x7C3C0200,  //  02A4  CALL	R15	1
      0x003C1E09,  //  02A5  ADD	R15	R15	R9
      0x8840016B,  //  02A6  GETMBR	R16	R0	K107
      0x003C1E10,  //  02A7  ADD	R15	R15	R16
      0x003C1E0D,  //  02A8  ADD	R15	R15	R13
      0x8C400522,  //  02A9  GETMET	R16	R2	K34
      0x7C400200,  //  02AA  CALL	R16	1
      0x8C402123,  //  02AB  GETMET	R16	R16	K35
      0x88480D24,  //  02AC  GETMBR	R18	R6	K36
      0x5C4C1E00,  //  02AD  MOVE	R19	R15
      0x5C501C00,  //  02AE  MOVE	R20	R14
      0x5456000F,  //  02AF  LDINT	R21	16
      0x7C400A00,  //  02B0  CALL	R16	5
      0xB8460C00,  //  02B1  GETNGBL	R17	K6
      0x8C442307,  //  02B2  GETMET	R17	R17	K7
      0x884C0D24,  //  02B3  GETMBR	R19	R6	K36
      0x8C4C272C,  //  02B4  GETMET	R19	R19	K44
      0x7C4C0200,  //  02B5  CALL	R19	1
      0x004F0413,  //  02B6  ADD	R19	K130	R19
      0x54520003,  //  02B7  LDINT	R20	4
      0x7C440600,  //  02B8  CALL	R17	3
      0xB8460C00,  //  02B9  GETNGBL	R17	K6
      0x8C442307,  //  02BA  GETMET	R17	R17	K7
      0x8C4C1F2C,  //  02BB  GETMET	R19	R15	K44
      0x7C4C0200,  //  02BC  CALL	R19	1
      0x004F0613,  //  02BD  ADD	R19	K131	R19
      0x54520003,  //  02BE  LDINT	R20	4
      0x7C440600,  //  02BF  CALL	R17	3
      0xB8460C00,  //  02C0  GETNGBL	R17	K6
      0x8C442307,  //  02C1  GETMET	R17	R17	K7
      0x8C4C212C,  //  02C2  GETMET	R19	R16	K44
      0x7C4C0200,  //  02C3  CALL	R19	1
      0x004F0813,  //  02C4  ADD	R19	K132	R19
      0x54520003,  //  02C5  LDINT	R20	4
      0x7C440600,  //  02C6  CALL	R17	3
      0x8C44194D,  //  02C7  GETMET	R17	R12	K77
      0x7C440200,  //  02C8  CALL	R17	1
      0xB84A0C00,  //  02C9  GETNGBL	R18	K6
      0x8C482507,  //  02CA  GETMET	R18	R18	K7
      0x8C50232C,  //  02CB  GETMET	R20	R17	K44
      0x7C500200,  //  02CC  CALL	R20	1
      0x00530A14,  //  02CD  ADD	R20	K133	R20
      0x54560003,  //  02CE  LDINT	R21	4
      0x7C480600,  //  02CF  CALL	R18	3
      0x8C480527,  //  02D0  GETMET	R18	R2	K39
      0x5C502000,  //  02D1  MOVE	R20	R16
      0x60540015,  //  02D2  GETGBL	R21	G21
      0x7C540000,  //  02D3  CALL	R21	0
      0x8C542B20,  //  02D4  GETMET	R21	R21	K32
      0x885C0186,  //  02D5  GETMBR	R23	R0	K134
      0x7C540400,  //  02D6  CALL	R21	2
      0x60580015,  //  02D7  GETGBL	R22	G21
      0x7C580000,  //  02D8  CALL	R22	0
      0x605C000C,  //  02D9  GETGBL	R23	G12
      0x5C602200,  //  02DA  MOVE	R24	R17
      0x7C5C0200,  //  02DB  CALL	R23	1
      0x5462000F,  //  02DC  LDINT	R24	16
      0x7C480C00,  //  02DD  CALL	R18	6
      0x8C4C2587,  //  02DE  GETMET	R19	R18	K135
      0x5C542200,  //  02DF  MOVE	R21	R17
      0x7C4C0400,  //  02E0  CALL	R19	2
      0x8C502529,  //  02E1  GETMET	R20	R18	K41
      0x7C500200,  //  02E2  CALL	R20	1
      0x004C2614,  //  02E3  ADD	R19	R19	R20
      0xB8520C00,  //  02E4  GETNGBL	R20	K6
      0x8C502907,  //  02E5  GETMET	R20	R20	K7
      0x8C58272C,  //  02E6  GETMET	R22	R19	K44
      0x7C580200,  //  02E7  CALL	R22	1
      0x005B1016,  //  02E8  ADD	R22	K136	R22
      0x545E0003,  //  02E9  LDINT	R23	4
      0x7C500600,  //  02EA  CALL	R20	3
      0xB8520C00,  //  02EB  GETNGBL	R20	K6
      0x8C502907,  //  02EC  GETMET	R20	R20	K7
      0x5858002A,  //  02ED  LDCONST	R22	K42
      0x545E0003,  //  02EE  LDINT	R23	4
      0x7C500600,  //  02EF  CALL	R20	3
      0xB8521800,  //  02F0  GETNGBL	R20	K12
      0x8C502989,  //  02F1  GETMET	R20	R20	K137
      0x7C500200,  //  02F2  CALL	R20	1
      0x90531409,  //  02F3  SETMBR	R20	K138	R9
      0x88540160,  //  02F4  GETMBR	R21	R0	K96
      0x90531615,  //  02F5  SETMBR	R20	K139	R21
      0x8854016B,  //  02F6  GETMBR	R21	R0	K107
      0x90531815,  //  02F7  SETMBR	R20	K140	R21
      0x90531A13,  //  02F8  SETMBR	R20	K141	R19
      0xB8560C00,  //  02F9  GETNGBL	R21	K6
      0x8C542B07,  //  02FA  GETMET	R21	R21	K7
      0xB85E1800,  //  02FB  GETNGBL	R23	K12
      0x8C5C2F13,  //  02FC  GETMET	R23	R23	K19
      0x5C642800,  //  02FD  MOVE	R25	R20
      0x7C5C0400,  //  02FE  CALL	R23	2
      0x005F1C17,  //  02FF  ADD	R23	K142	R23
      0x54620003,  //  0300  LDINT	R24	4
      0x7C540600,  //  0301  CALL	R21	3
      0x8C54294D,  //  0302  GETMET	R21	R20	K77
      0x7C540200,  //  0303  CALL	R21	1
      0x901B1E15,  //  0304  SETMBR	R6	K143	R21
      0xB85A0C00,  //  0305  GETNGBL	R22	K6
      0x8C582D07,  //  0306  GETMET	R22	R22	K7
      0x8C602B2C,  //  0307  GETMET	R24	R21	K44
      0x7C600200,  //  0308  CALL	R24	1
      0x00632018,  //  0309  ADD	R24	K144	R24
      0x54660003,  //  030A  LDINT	R25	4
      0x7C580600,  //  030B  CALL	R22	3
      0x8C580351,  //  030C  GETMET	R22	R1	K81
      0x54620030,  //  030D  LDINT	R24	49
      0x50640200,  //  030E  LDBOOL	R25	1	0
      0x7C580600,  //  030F  CALL	R22	3
      0x8C5C2D52,  //  0310  GETMET	R23	R22	K82
      0x5C642A00,  //  0311  MOVE	R25	R21
      0x7C5C0400,  //  0312  CALL	R23	2
      0x88600153,  //  0313  GETMBR	R24	R0	K83
      0x8C603154,  //  0314  GETMET	R24	R24	K84
      0x5C682C00,  //  0315  MOVE	R26	R22
      0x7C600400,  //  0316  CALL	R24	2
      0x50600200,  //  0317  LDBOOL	R24	1	0
      0x80043000,  //  0318  RET	1	R24
      0x50200200,  //  0319  LDBOOL	R8	1	0
      0x80041000,  //  031A  RET	1	R8
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
** Solidified class: Matter_Commisioning_Context
********************************************************************/
be_local_class(Matter_Commisioning_Context,
    20,
    NULL,
    be_nested_map(38,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(send_status_report, -1), be_const_closure(Matter_Commisioning_Context_send_status_report_closure) },
        { be_const_key_weak(cB, -1), be_const_var(11) },
        { be_const_key_weak(TBEData3_Nonce, 37), be_nested_str_weak(NCASE_Sigma3N) },
        { be_const_key_weak(find_fabric_by_destination_id, -1), be_const_closure(Matter_Commisioning_Context_find_fabric_by_destination_id_closure) },
        { be_const_key_weak(parse_Sigma1, 3), be_const_closure(Matter_Commisioning_Context_parse_Sigma1_closure) },
        { be_const_key_weak(responder, -1), be_const_var(0) },
        { be_const_key_weak(parse_PBKDFParamRequest, -1), be_const_closure(Matter_Commisioning_Context_parse_PBKDFParamRequest_closure) },
        { be_const_key_weak(parse_Pake1, -1), be_const_closure(Matter_Commisioning_Context_parse_Pake1_closure) },
        { be_const_key_weak(process_incoming, -1), be_const_closure(Matter_Commisioning_Context_process_incoming_closure) },
        { be_const_key_weak(future_initiator_session_id, 2), be_const_var(3) },
        { be_const_key_weak(cA, -1), be_const_var(10) },
        { be_const_key_weak(parse_Sigma3, -1), be_const_closure(Matter_Commisioning_Context_parse_Sigma3_closure) },
        { be_const_key_weak(ResponderEph_priv, -1), be_const_var(13) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Commisioning_Context_init_closure) },
        { be_const_key_weak(parse_StatusReport, -1), be_const_closure(Matter_Commisioning_Context_parse_StatusReport_closure) },
        { be_const_key_weak(device, 8), be_const_var(1) },
        { be_const_key_weak(PBKDFParamResponse, -1), be_const_var(6) },
        { be_const_key_weak(future_local_session_id, 18), be_const_var(4) },
        { be_const_key_weak(pB, 11), be_const_var(9) },
        { be_const_key_weak(add_session, -1), be_const_closure(Matter_Commisioning_Context_add_session_closure) },
        { be_const_key_weak(Ke, -1), be_const_var(12) },
        { be_const_key_weak(SEKeys_Info, -1), be_nested_str_weak(SessionKeys) },
        { be_const_key_weak(S2K_Info, -1), be_nested_str_weak(Sigma2) },
        { be_const_key_weak(created, 26), be_const_var(16) },
        { be_const_key_weak(AttestationChallenge, 21), be_const_var(19) },
        { be_const_key_weak(R2IKey, 20), be_const_var(18) },
        { be_const_key_weak(parse_Pake3, -1), be_const_closure(Matter_Commisioning_Context_parse_Pake3_closure) },
        { be_const_key_weak(S3K_Info, -1), be_nested_str_weak(Sigma3) },
        { be_const_key_weak(pA, 16), be_const_var(8) },
        { be_const_key_weak(Matter_Context_Prefix, 14), be_nested_str_weak(CHIP_X20PAKE_X20V1_X20Commissioning) },
        { be_const_key_weak(PBKDFParamRequest, -1), be_const_var(5) },
        { be_const_key_weak(every_second, 10), be_const_closure(Matter_Commisioning_Context_every_second_closure) },
        { be_const_key_weak(TBEData2_Nonce, -1), be_nested_str_weak(NCASE_Sigma2N) },
        { be_const_key_weak(initiatorEph_pub, 4), be_const_var(15) },
        { be_const_key_weak(y, -1), be_const_var(7) },
        { be_const_key_weak(spake, 0), be_const_var(2) },
        { be_const_key_weak(ResponderEph_pub, 1), be_const_var(14) },
        { be_const_key_weak(I2RKey, -1), be_const_var(17) },
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
