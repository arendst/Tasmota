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
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[52]) {     /* constants */
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
    /* K50  */  be_nested_str_weak(send_response),
    /* K51  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[140]) {  /* code */
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
      0x5C2C1000,  //  0084  MOVE	R11	R8
      0x8830031F,  //  0085  GETMBR	R12	R1	K31
      0x88340320,  //  0086  GETMBR	R13	R1	K32
      0x88380F33,  //  0087  GETMBR	R14	R7	K51
      0x883C0312,  //  0088  GETMBR	R15	R1	K18
      0x883C1F03,  //  0089  GETMBR	R15	R15	K3
      0x7C240C00,  //  008A  CALL	R9	6
      0x80000000,  //  008B  RET	0
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
    ( &(const bvalue[48]) {     /* constants */
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
    /* K43  */  be_nested_str_weak(send_response),
    /* K44  */  be_nested_str_weak(remote_ip),
    /* K45  */  be_nested_str_weak(remote_port),
    /* K46  */  be_nested_str_weak(message_counter),
    /* K47  */  be_nested_str_weak(session),
    }),
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[118]) {  /* code */
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
      0x5C301200,  //  006E  MOVE	R12	R9
      0x8834032C,  //  006F  GETMBR	R13	R1	K44
      0x8838032D,  //  0070  GETMBR	R14	R1	K45
      0x883C112E,  //  0071  GETMBR	R15	R8	K46
      0x8840032F,  //  0072  GETMBR	R16	R1	K47
      0x88402102,  //  0073  GETMBR	R16	R16	K2
      0x7C280C00,  //  0074  CALL	R10	6
      0x80000000,  //  0075  RET	0
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
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
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
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
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
      0x540E001F,  //  001D  LDINT	R3	32
      0x1C080403,  //  001E  EQ	R2	R2	R3
      0x780A0004,  //  001F  JMPF	R2	#0025
      0x8C08010B,  //  0020  GETMET	R2	R0	K11
      0x5C100200,  //  0021  MOVE	R4	R1
      0x7C080400,  //  0022  CALL	R2	2
      0x80040400,  //  0023  RET	1	R2
      0x7002002B,  //  0024  JMP		#0051
      0x88080302,  //  0025  GETMBR	R2	R1	K2
      0x540E0021,  //  0026  LDINT	R3	34
      0x1C080403,  //  0027  EQ	R2	R2	R3
      0x780A0004,  //  0028  JMPF	R2	#002E
      0x8C08010C,  //  0029  GETMET	R2	R0	K12
      0x5C100200,  //  002A  MOVE	R4	R1
      0x7C080400,  //  002B  CALL	R2	2
      0x80040400,  //  002C  RET	1	R2
      0x70020022,  //  002D  JMP		#0051
      0x88080302,  //  002E  GETMBR	R2	R1	K2
      0x540E0023,  //  002F  LDINT	R3	36
      0x1C080403,  //  0030  EQ	R2	R2	R3
      0x780A0004,  //  0031  JMPF	R2	#0037
      0x8C08010D,  //  0032  GETMET	R2	R0	K13
      0x5C100200,  //  0033  MOVE	R4	R1
      0x7C080400,  //  0034  CALL	R2	2
      0x80040400,  //  0035  RET	1	R2
      0x70020019,  //  0036  JMP		#0051
      0x88080302,  //  0037  GETMBR	R2	R1	K2
      0x540E002F,  //  0038  LDINT	R3	48
      0x1C080403,  //  0039  EQ	R2	R2	R3
      0x780A0004,  //  003A  JMPF	R2	#0040
      0x8C08010E,  //  003B  GETMET	R2	R0	K14
      0x5C100200,  //  003C  MOVE	R4	R1
      0x7C080400,  //  003D  CALL	R2	2
      0x80040400,  //  003E  RET	1	R2
      0x70020010,  //  003F  JMP		#0051
      0x88080302,  //  0040  GETMBR	R2	R1	K2
      0x540E0031,  //  0041  LDINT	R3	50
      0x1C080403,  //  0042  EQ	R2	R2	R3
      0x780A0004,  //  0043  JMPF	R2	#0049
      0x8C08010F,  //  0044  GETMET	R2	R0	K15
      0x5C100200,  //  0045  MOVE	R4	R1
      0x7C080400,  //  0046  CALL	R2	2
      0x80040400,  //  0047  RET	1	R2
      0x70020007,  //  0048  JMP		#0051
      0x88080302,  //  0049  GETMBR	R2	R1	K2
      0x540E003F,  //  004A  LDINT	R3	64
      0x1C080403,  //  004B  EQ	R2	R2	R3
      0x780A0003,  //  004C  JMPF	R2	#0051
      0x8C080110,  //  004D  GETMET	R2	R0	K16
      0x5C100200,  //  004E  MOVE	R4	R1
      0x7C080400,  //  004F  CALL	R2	2
      0x80040400,  //  0050  RET	1	R2
      0x50080000,  //  0051  LDBOOL	R2	0	0
      0x80040400,  //  0052  RET	1	R2
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
    ( &(const bvalue[89]) {     /* constants */
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
    /* K24  */  be_nested_str_weak(MTR_X3A_X20session_X2Eipk_epoch_key_X20),
    /* K25  */  be_nested_str_weak(get_ipk_epoch_key),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20session_X2Efabric_compressed_X20),
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
    /* K50  */  be_nested_str_weak(findsubval),
    /* K51  */  be_const_int(3),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20initiatorNOCTLV_X20_X3D_X20),
    /* K53  */  be_nested_str_weak(findsub),
    /* K54  */  be_nested_str_weak(int),
    /* K55  */  be_nested_str_weak(int64),
    /* K56  */  be_nested_str_weak(peer_node_id),
    /* K57  */  be_nested_str_weak(tobytes),
    /* K58  */  be_nested_str_weak(MTR_X3A_X20initiatorFabricId_X3D),
    /* K59  */  be_nested_str_weak(Matter_TLV_struct),
    /* K60  */  be_nested_str_weak(add_TLV),
    /* K61  */  be_nested_str_weak(B1),
    /* K62  */  be_nested_str_weak(initiatorEph_pub),
    /* K63  */  be_nested_str_weak(ResponderEph_pub),
    /* K64  */  be_nested_str_weak(tlv2raw),
    /* K65  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20initiatorNOCPubKey_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20ec_signature_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K67  */  be_nested_str_weak(EC_P256),
    /* K68  */  be_nested_str_weak(ecdsa_verify_sha256),
    /* K69  */  be_nested_str_weak(MTR_X3A_X20sigma3_tbs_X20does_X20not_X20have_X20a_X20valid_X20signature),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20Sigma3_X20verified_X2C_X20computing_X20new_X20keys),
    /* K71  */  be_nested_str_weak(Msg3),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20shared_secret_X20_X3D),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20ipk_X20_X2B_X20hash_X20_X20_X20_X20_X3D),
    /* K75  */  be_nested_str_weak(SEKeys_Info),
    /* K76  */  be_nested_str_weak(rtc),
    /* K77  */  be_nested_str_weak(utc),
    /* K78  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K79  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K80  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K81  */  be_nested_str_weak(close),
    /* K82  */  be_nested_str_weak(set_keys),
    /* K83  */  be_nested_str_weak(_breadcrumb),
    /* K84  */  be_nested_str_weak(counter_snd_next),
    /* K85  */  be_nested_str_weak(set_persist),
    /* K86  */  be_nested_str_weak(set_no_expiration),
    /* K87  */  be_nested_str_weak(persist_to_fabric),
    /* K88  */  be_nested_str_weak(save),
    }),
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[480]) {  /* code */
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
      0x8C381B32,  //  00D5  GETMET	R14	R13	K50
      0x5840000A,  //  00D6  LDCONST	R16	K10
      0x7C380400,  //  00D7  CALL	R14	2
      0x8C3C1B32,  //  00D8  GETMET	R15	R13	K50
      0x58440008,  //  00D9  LDCONST	R17	K8
      0x7C3C0400,  //  00DA  CALL	R15	2
      0x8C401B32,  //  00DB  GETMET	R16	R13	K50
      0x58480033,  //  00DC  LDCONST	R18	K51
      0x7C400400,  //  00DD  CALL	R16	2
      0xB8461800,  //  00DE  GETNGBL	R17	K12
      0x88442331,  //  00DF  GETMBR	R17	R17	K49
      0x8C44230E,  //  00E0  GETMET	R17	R17	K14
      0x5C4C1C00,  //  00E1  MOVE	R19	R14
      0x7C440400,  //  00E2  CALL	R17	2
      0xB84A0A00,  //  00E3  GETNGBL	R18	K5
      0x8C482506,  //  00E4  GETMET	R18	R18	K6
      0x60500008,  //  00E5  GETGBL	R20	G8
      0x5C542200,  //  00E6  MOVE	R21	R17
      0x7C500200,  //  00E7  CALL	R20	1
      0x00526814,  //  00E8  ADD	R20	K52	R20
      0x58540033,  //  00E9  LDCONST	R21	K51
      0x7C480600,  //  00EA  CALL	R18	3
      0x8C482332,  //  00EB  GETMET	R18	R17	K50
      0x54520008,  //  00EC  LDINT	R20	9
      0x7C480400,  //  00ED  CALL	R18	2
      0x8C4C2335,  //  00EE  GETMET	R19	R17	K53
      0x54560005,  //  00EF  LDINT	R21	6
      0x7C4C0400,  //  00F0  CALL	R19	2
      0x8C502732,  //  00F1  GETMET	R20	R19	K50
      0x545A0010,  //  00F2  LDINT	R22	17
      0x7C500400,  //  00F3  CALL	R20	2
      0x60540004,  //  00F4  GETGBL	R21	G4
      0x5C582800,  //  00F5  MOVE	R22	R20
      0x7C540200,  //  00F6  CALL	R21	1
      0x1C542B36,  //  00F7  EQ	R21	R21	K54
      0x78560003,  //  00F8  JMPF	R21	#00FD
      0xB8566E00,  //  00F9  GETNGBL	R21	K55
      0x5C582800,  //  00FA  MOVE	R22	R20
      0x7C540200,  //  00FB  CALL	R21	1
      0x5C502A00,  //  00FC  MOVE	R20	R21
      0x8C542939,  //  00FD  GETMET	R21	R20	K57
      0x7C540200,  //  00FE  CALL	R21	1
      0x900E7015,  //  00FF  SETMBR	R3	K56	R21
      0xB8560A00,  //  0100  GETNGBL	R21	K5
      0x8C542B06,  //  0101  GETMET	R21	R21	K6
      0x605C0008,  //  0102  GETGBL	R23	G8
      0x88600738,  //  0103  GETMBR	R24	R3	K56
      0x7C5C0200,  //  0104  CALL	R23	1
      0x005E7417,  //  0105  ADD	R23	K58	R23
      0x58600033,  //  0106  LDCONST	R24	K51
      0x7C540600,  //  0107  CALL	R21	3
      0xB8561800,  //  0108  GETNGBL	R21	K12
      0x88542B31,  //  0109  GETMBR	R21	R21	K49
      0x8C542B3B,  //  010A  GETMET	R21	R21	K59
      0x7C540200,  //  010B  CALL	R21	1
      0x8C582B3C,  //  010C  GETMET	R22	R21	K60
      0x5860000A,  //  010D  LDCONST	R24	K10
      0xB8661800,  //  010E  GETNGBL	R25	K12
      0x88643331,  //  010F  GETMBR	R25	R25	K49
      0x8864333D,  //  0110  GETMBR	R25	R25	K61
      0x5C681C00,  //  0111  MOVE	R26	R14
      0x7C580800,  //  0112  CALL	R22	4
      0x8C582B3C,  //  0113  GETMET	R22	R21	K60
      0x58600008,  //  0114  LDCONST	R24	K8
      0xB8661800,  //  0115  GETNGBL	R25	K12
      0x88643331,  //  0116  GETMBR	R25	R25	K49
      0x8864333D,  //  0117  GETMBR	R25	R25	K61
      0x5C681E00,  //  0118  MOVE	R26	R15
      0x7C580800,  //  0119  CALL	R22	4
      0x8C582B3C,  //  011A  GETMET	R22	R21	K60
      0x58600033,  //  011B  LDCONST	R24	K51
      0xB8661800,  //  011C  GETNGBL	R25	K12
      0x88643331,  //  011D  GETMBR	R25	R25	K49
      0x8864333D,  //  011E  GETMBR	R25	R25	K61
      0x8868013E,  //  011F  GETMBR	R26	R0	K62
      0x7C580800,  //  0120  CALL	R22	4
      0x8C582B3C,  //  0121  GETMET	R22	R21	K60
      0x54620003,  //  0122  LDINT	R24	4
      0xB8661800,  //  0123  GETNGBL	R25	K12
      0x88643331,  //  0124  GETMBR	R25	R25	K49
      0x8864333D,  //  0125  GETMBR	R25	R25	K61
      0x8868013F,  //  0126  GETMBR	R26	R0	K63
      0x7C580800,  //  0127  CALL	R22	4
      0x8C582B40,  //  0128  GETMET	R22	R21	K64
      0x7C580200,  //  0129  CALL	R22	1
      0xB85E0A00,  //  012A  GETNGBL	R23	K5
      0x8C5C2F06,  //  012B  GETMET	R23	R23	K6
      0x8C64251E,  //  012C  GETMET	R25	R18	K30
      0x7C640200,  //  012D  CALL	R25	1
      0x00668219,  //  012E  ADD	R25	K65	R25
      0x546A0003,  //  012F  LDINT	R26	4
      0x7C5C0600,  //  0130  CALL	R23	3
      0xB85E0A00,  //  0131  GETNGBL	R23	K5
      0x8C5C2F06,  //  0132  GETMET	R23	R23	K6
      0x8C64211E,  //  0133  GETMET	R25	R16	K30
      0x7C640200,  //  0134  CALL	R25	1
      0x00668419,  //  0135  ADD	R25	K66	R25
      0x546A0003,  //  0136  LDINT	R26	4
      0x7C5C0600,  //  0137  CALL	R23	3
      0xB85E0A00,  //  0138  GETNGBL	R23	K5
      0x8C5C2F06,  //  0139  GETMET	R23	R23	K6
      0x58640011,  //  013A  LDCONST	R25	K17
      0x546A0003,  //  013B  LDINT	R26	4
      0x7C5C0600,  //  013C  CALL	R23	3
      0x8C5C0543,  //  013D  GETMET	R23	R2	K67
      0x7C5C0200,  //  013E  CALL	R23	1
      0x8C5C2F44,  //  013F  GETMET	R23	R23	K68
      0x5C642400,  //  0140  MOVE	R25	R18
      0x5C682C00,  //  0141  MOVE	R26	R22
      0x5C6C2000,  //  0142  MOVE	R27	R16
      0x7C5C0800,  //  0143  CALL	R23	4
      0x5C602E00,  //  0144  MOVE	R24	R23
      0x74620012,  //  0145  JMPT	R24	#0159
      0xB8620A00,  //  0146  GETNGBL	R24	K5
      0x8C603106,  //  0147  GETMET	R24	R24	K6
      0x58680045,  //  0148  LDCONST	R26	K69
      0x586C0008,  //  0149  LDCONST	R27	K8
      0x7C600600,  //  014A  CALL	R24	3
      0xB8620A00,  //  014B  GETNGBL	R24	K5
      0x8C603106,  //  014C  GETMET	R24	R24	K6
      0x58680007,  //  014D  LDCONST	R26	K7
      0x586C0008,  //  014E  LDCONST	R27	K8
      0x7C600600,  //  014F  CALL	R24	3
      0x8C600109,  //  0150  GETMET	R24	R0	K9
      0x5C680200,  //  0151  MOVE	R26	R1
      0x586C000A,  //  0152  LDCONST	R27	K10
      0x58700003,  //  0153  LDCONST	R28	K3
      0x58740008,  //  0154  LDCONST	R29	K8
      0x50780000,  //  0155  LDBOOL	R30	0	0
      0x7C600C00,  //  0156  CALL	R24	6
      0x50640000,  //  0157  LDBOOL	R25	0	0
      0x80043200,  //  0158  RET	1	R25
      0xB8620A00,  //  0159  GETNGBL	R24	K5
      0x8C603106,  //  015A  GETMET	R24	R24	K6
      0x58680046,  //  015B  LDCONST	R26	K70
      0x586C0033,  //  015C  LDCONST	R27	K51
      0x7C600600,  //  015D  CALL	R24	3
      0x8C600512,  //  015E  GETMET	R24	R2	K18
      0x7C600200,  //  015F  CALL	R24	1
      0x8C603113,  //  0160  GETMET	R24	R24	K19
      0x88680714,  //  0161  GETMBR	R26	R3	K20
      0x7C600400,  //  0162  CALL	R24	2
      0x8C603113,  //  0163  GETMET	R24	R24	K19
      0x88680715,  //  0164  GETMBR	R26	R3	K21
      0x7C600400,  //  0165  CALL	R24	2
      0x8C603113,  //  0166  GETMET	R24	R24	K19
      0x88680947,  //  0167  GETMBR	R26	R4	K71
      0x7C600400,  //  0168  CALL	R24	2
      0x8C603116,  //  0169  GETMET	R24	R24	K22
      0x7C600200,  //  016A  CALL	R24	1
      0x5C143000,  //  016B  MOVE	R5	R24
      0x4C600000,  //  016C  LDNIL	R24
      0x900E2818,  //  016D  SETMBR	R3	K20	R24
      0x4C600000,  //  016E  LDNIL	R24
      0x900E2A18,  //  016F  SETMBR	R3	K21	R24
      0xB8620A00,  //  0170  GETNGBL	R24	K5
      0x8C603106,  //  0171  GETMET	R24	R24	K6
      0x58680048,  //  0172  LDCONST	R26	K72
      0x546E0003,  //  0173  LDINT	R27	4
      0x7C600600,  //  0174  CALL	R24	3
      0xB8620A00,  //  0175  GETNGBL	R24	K5
      0x8C603106,  //  0176  GETMET	R24	R24	K6
      0x88680724,  //  0177  GETMBR	R26	R3	K36
      0x8C68351E,  //  0178  GETMET	R26	R26	K30
      0x7C680200,  //  0179  CALL	R26	1
      0x006A921A,  //  017A  ADD	R26	K73	R26
      0x546E0003,  //  017B  LDINT	R27	4
      0x7C600600,  //  017C  CALL	R24	3
      0xB8620A00,  //  017D  GETNGBL	R24	K5
      0x8C603106,  //  017E  GETMET	R24	R24	K6
      0x8C68071D,  //  017F  GETMET	R26	R3	K29
      0x7C680200,  //  0180  CALL	R26	1
      0x00683405,  //  0181  ADD	R26	R26	R5
      0x8C68351E,  //  0182  GETMET	R26	R26	K30
      0x7C680200,  //  0183  CALL	R26	1
      0x006A941A,  //  0184  ADD	R26	K74	R26
      0x546E0003,  //  0185  LDINT	R27	4
      0x7C600600,  //  0186  CALL	R24	3
      0x8C600522,  //  0187  GETMET	R24	R2	K34
      0x7C600200,  //  0188  CALL	R24	1
      0x8C603123,  //  0189  GETMET	R24	R24	K35
      0x88680724,  //  018A  GETMBR	R26	R3	K36
      0x8C6C071D,  //  018B  GETMET	R27	R3	K29
      0x7C6C0200,  //  018C  CALL	R27	1
      0x006C3605,  //  018D  ADD	R27	R27	R5
      0x60700015,  //  018E  GETGBL	R28	G21
      0x7C700000,  //  018F  CALL	R28	0
      0x8C703920,  //  0190  GETMET	R28	R28	K32
      0x8878014B,  //  0191  GETMBR	R30	R0	K75
      0x7C700400,  //  0192  CALL	R28	2
      0x5476002F,  //  0193  LDINT	R29	48
      0x7C600A00,  //  0194  CALL	R24	5
      0x5466000E,  //  0195  LDINT	R25	15
      0x40660619,  //  0196  CONNECT	R25	K3	R25
      0x94643019,  //  0197  GETIDX	R25	R24	R25
      0x546A000F,  //  0198  LDINT	R26	16
      0x546E001E,  //  0199  LDINT	R27	31
      0x4068341B,  //  019A  CONNECT	R26	R26	R27
      0x9468301A,  //  019B  GETIDX	R26	R24	R26
      0x546E001F,  //  019C  LDINT	R27	32
      0x5472002E,  //  019D  LDINT	R28	47
      0x406C361C,  //  019E  CONNECT	R27	R27	R28
      0x946C301B,  //  019F  GETIDX	R27	R24	R27
      0xB8720A00,  //  01A0  GETNGBL	R28	K5
      0x8C70394C,  //  01A1  GETMET	R28	R28	K76
      0x7C700200,  //  01A2  CALL	R28	1
      0x9470394D,  //  01A3  GETIDX	R28	R28	K77
      0xB8760A00,  //  01A4  GETNGBL	R29	K5
      0x8C743B06,  //  01A5  GETMET	R29	R29	K6
      0x587C0048,  //  01A6  LDCONST	R31	K72
      0x54820003,  //  01A7  LDINT	R32	4
      0x7C740600,  //  01A8  CALL	R29	3
      0xB8760A00,  //  01A9  GETNGBL	R29	K5
      0x8C743B06,  //  01AA  GETMET	R29	R29	K6
      0x8C7C331E,  //  01AB  GETMET	R31	R25	K30
      0x7C7C0200,  //  01AC  CALL	R31	1
      0x007E9C1F,  //  01AD  ADD	R31	K78	R31
      0x54820003,  //  01AE  LDINT	R32	4
      0x7C740600,  //  01AF  CALL	R29	3
      0xB8760A00,  //  01B0  GETNGBL	R29	K5
      0x8C743B06,  //  01B1  GETMET	R29	R29	K6
      0x8C7C351E,  //  01B2  GETMET	R31	R26	K30
      0x7C7C0200,  //  01B3  CALL	R31	1
      0x007E9E1F,  //  01B4  ADD	R31	K79	R31
      0x54820003,  //  01B5  LDINT	R32	4
      0x7C740600,  //  01B6  CALL	R29	3
      0xB8760A00,  //  01B7  GETNGBL	R29	K5
      0x8C743B06,  //  01B8  GETMET	R29	R29	K6
      0x8C7C371E,  //  01B9  GETMET	R31	R27	K30
      0x7C7C0200,  //  01BA  CALL	R31	1
      0x007EA01F,  //  01BB  ADD	R31	K80	R31
      0x54820003,  //  01BC  LDINT	R32	4
      0x7C740600,  //  01BD  CALL	R29	3
      0xB8760A00,  //  01BE  GETNGBL	R29	K5
      0x8C743B06,  //  01BF  GETMET	R29	R29	K6
      0x587C0048,  //  01C0  LDCONST	R31	K72
      0x54820003,  //  01C1  LDINT	R32	4
      0x7C740600,  //  01C2  CALL	R29	3
      0x8C740109,  //  01C3  GETMET	R29	R0	K9
      0x5C7C0200,  //  01C4  MOVE	R31	R1
      0x58800003,  //  01C5  LDCONST	R32	K3
      0x58840003,  //  01C6  LDCONST	R33	K3
      0x58880003,  //  01C7  LDCONST	R34	K3
      0x508C0200,  //  01C8  LDBOOL	R35	1	0
      0x7C740C00,  //  01C9  CALL	R29	6
      0x8C780751,  //  01CA  GETMET	R30	R3	K81
      0x7C780200,  //  01CB  CALL	R30	1
      0x8C780752,  //  01CC  GETMET	R30	R3	K82
      0x5C803200,  //  01CD  MOVE	R32	R25
      0x5C843400,  //  01CE  MOVE	R33	R26
      0x5C883600,  //  01CF  MOVE	R34	R27
      0x5C8C3800,  //  01D0  MOVE	R35	R28
      0x7C780A00,  //  01D1  CALL	R30	5
      0x900EA703,  //  01D2  SETMBR	R3	K83	K3
      0x8C780754,  //  01D3  GETMET	R30	R3	K84
      0x7C780200,  //  01D4  CALL	R30	1
      0x8C780755,  //  01D5  GETMET	R30	R3	K85
      0x50800200,  //  01D6  LDBOOL	R32	1	0
      0x7C780400,  //  01D7  CALL	R30	2
      0x8C780756,  //  01D8  GETMET	R30	R3	K86
      0x7C780200,  //  01D9  CALL	R30	1
      0x8C780757,  //  01DA  GETMET	R30	R3	K87
      0x7C780200,  //  01DB  CALL	R30	1
      0x8C780758,  //  01DC  GETMET	R30	R3	K88
      0x7C780200,  //  01DD  CALL	R30	1
      0x50780200,  //  01DE  LDBOOL	R30	1	0
      0x80043C00,  //  01DF  RET	1	R30
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
    ( &(const binstruction[111]) {  /* code */
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
      0x80000000,  //  006E  RET	0
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
    /* K3   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X20_X3D_X20),
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
      0x500C0200,  //  000C  LDBOOL	R3	1	0
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
    ( &(const bvalue[127]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(opcode),
    /* K3   */  be_nested_str_weak(local_session_id),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(protocol_id),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Sigma1_X20message),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K11  */  be_nested_str_weak(send_status_report),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(matter),
    /* K14  */  be_nested_str_weak(Sigma1),
    /* K15  */  be_nested_str_weak(parse),
    /* K16  */  be_nested_str_weak(raw),
    /* K17  */  be_nested_str_weak(app_payload_idx),
    /* K18  */  be_nested_str_weak(initiatorEph_pub),
    /* K19  */  be_nested_str_weak(initiatorEphPubKey),
    /* K20  */  be_nested_str_weak(resumptionID),
    /* K21  */  be_nested_str_weak(initiatorResumeMIC),
    /* K22  */  be_nested_str_weak(session),
    /* K23  */  be_nested_str_weak(device),
    /* K24  */  be_nested_str_weak(sessions),
    /* K25  */  be_nested_str_weak(find_session_by_resumption_id),
    /* K26  */  be_nested_str_weak(find_fabric_by_destination_id),
    /* K27  */  be_nested_str_weak(destinationId),
    /* K28  */  be_nested_str_weak(initiatorRandom),
    /* K29  */  be_nested_str_weak(_fabric),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K31  */  be_nested_str_weak(_source_node_id),
    /* K32  */  be_nested_str_weak(source_node_id),
    /* K33  */  be_nested_str_weak(set_mode_CASE),
    /* K34  */  be_nested_str_weak(remove_session),
    /* K35  */  be_nested_str_weak(__future_initiator_session_id),
    /* K36  */  be_nested_str_weak(initiator_session_id),
    /* K37  */  be_nested_str_weak(__future_local_session_id),
    /* K38  */  be_nested_str_weak(gen_local_session_id),
    /* K39  */  be_nested_str_weak(future_local_session_id),
    /* K40  */  be_nested_str_weak(format),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K42  */  be_nested_str_weak(remote_ip),
    /* K43  */  be_nested_str_weak(remote_port),
    /* K44  */  be_nested_str_weak(fromstring),
    /* K45  */  be_nested_str_weak(Sigma1_Resume),
    /* K46  */  be_nested_str_weak(HKDF_SHA256),
    /* K47  */  be_nested_str_weak(derive),
    /* K48  */  be_nested_str_weak(shared_secret),
    /* K49  */  be_nested_str_weak(NCASE_SigmaR1),
    /* K50  */  be_const_int(2147483647),
    /* K51  */  be_nested_str_weak(AES_CCM),
    /* K52  */  be_nested_str_weak(decrypt),
    /* K53  */  be_nested_str_weak(tag),
    /* K54  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s1rk_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K56  */  be_nested_str_weak(tohex),
    /* K57  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20tag_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K58  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20Resume1MICPayload_X20_X3D_X20),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20decrypted_tag_X20_X20_X20_X20_X20_X3D_X20),
    /* K60  */  be_nested_str_weak(resumption_id),
    /* K61  */  be_nested_str_weak(random),
    /* K62  */  be_nested_str_weak(Sigma2_Resume),
    /* K63  */  be_nested_str_weak(NCASE_SigmaR2),
    /* K64  */  be_nested_str_weak(Sigma2Resume),
    /* K65  */  be_nested_str_weak(responderSessionID),
    /* K66  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K67  */  be_nested_str_weak(SessionResumptionKeys),
    /* K68  */  be_nested_str_weak(rtc),
    /* K69  */  be_nested_str_weak(utc),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K71  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K74  */  be_nested_str_weak(tlv2raw),
    /* K75  */  be_nested_str_weak(__Msg1),
    /* K76  */  be_nested_str_weak(MTR_X3A_X20sigma2resume_raw_X3A_X20),
    /* K77  */  be_nested_str_weak(build_response),
    /* K78  */  be_nested_str_weak(encode_frame),
    /* K79  */  be_nested_str_weak(responder),
    /* K80  */  be_nested_str_weak(send_response),
    /* K81  */  be_nested_str_weak(message_counter),
    /* K82  */  be_nested_str_weak(set_keys),
    /* K83  */  be_nested_str_weak(MTR_X3A_X20fabric_X3D),
    /* K84  */  be_nested_str_weak(inspect),
    /* K85  */  be_nested_str_weak(MTR_X3A_X20no_private_key_X3D),
    /* K86  */  be_nested_str_weak(no_private_key),
    /* K87  */  be_nested_str_weak(MTR_X3A_X20noc_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K88  */  be_nested_str_weak(noc),
    /* K89  */  be_nested_str_weak(MTR_X3A_X20root_ca_cert_X20_X20_X3D),
    /* K90  */  be_nested_str_weak(root_ca_certificate),
    /* K91  */  be_nested_str_weak(ResponderEph_priv),
    /* K92  */  be_nested_str_weak(ResponderEph_pub),
    /* K93  */  be_nested_str_weak(EC_P256),
    /* K94  */  be_nested_str_weak(public_key),
    /* K95  */  be_nested_str_weak(shared_key),
    /* K96  */  be_nested_str_weak(TLV),
    /* K97  */  be_nested_str_weak(Matter_TLV_struct),
    /* K98  */  be_nested_str_weak(add_TLV),
    /* K99  */  be_nested_str_weak(B2),
    /* K100 */  be_nested_str_weak(get_noc),
    /* K101 */  be_nested_str_weak(get_icac),
    /* K102 */  be_const_int(3),
    /* K103 */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K104 */  be_nested_str_weak(get_pk),
    /* K105 */  be_nested_str_weak(Msg1),
    /* K106 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20MSG1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K107 */  be_nested_str_weak(SHA256),
    /* K108 */  be_nested_str_weak(update),
    /* K109 */  be_nested_str_weak(out),
    /* K110 */  be_nested_str_weak(MTR_X3A_X20TranscriptHash_X20_X3D),
    /* K111 */  be_nested_str_weak(S2K_Info),
    /* K112 */  be_nested_str_weak(get_ipk_group_key),
    /* K113 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20SharedSecret_X20_X20_X3D_X20),
    /* K114 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K115 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K116 */  be_nested_str_weak(TBEData2_Nonce),
    /* K117 */  be_nested_str_weak(encrypt),
    /* K118 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Enc_X20_X20_X20_X3D_X20),
    /* K119 */  be_nested_str_weak(Sigma2),
    /* K120 */  be_nested_str_weak(responderRandom),
    /* K121 */  be_nested_str_weak(responderSessionId),
    /* K122 */  be_nested_str_weak(responderEphPubKey),
    /* K123 */  be_nested_str_weak(encrypted2),
    /* K124 */  be_nested_str_weak(MTR_X3A_X20sigma2_X3A_X20),
    /* K125 */  be_nested_str_weak(__Msg2),
    /* K126 */  be_nested_str_weak(MTR_X3A_X20sigma2_raw_X3A_X20),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[642]) {  /* code */
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
      0x88140913,  //  0026  GETMBR	R5	R4	K19
      0x90022405,  //  0027  SETMBR	R0	K18	R5
      0x88140914,  //  0028  GETMBR	R5	R4	K20
      0x4C180000,  //  0029  LDNIL	R6
      0x20140A06,  //  002A  NE	R5	R5	R6
      0x78160003,  //  002B  JMPF	R5	#0030
      0x88140915,  //  002C  GETMBR	R5	R4	K21
      0x4C180000,  //  002D  LDNIL	R6
      0x20140A06,  //  002E  NE	R5	R5	R6
      0x74160000,  //  002F  JMPT	R5	#0031
      0x50140001,  //  0030  LDBOOL	R5	0	1
      0x50140200,  //  0031  LDBOOL	R5	1	0
      0x88180316,  //  0032  GETMBR	R6	R1	K22
      0x78160006,  //  0033  JMPF	R5	#003B
      0x881C0117,  //  0034  GETMBR	R7	R0	K23
      0x881C0F18,  //  0035  GETMBR	R7	R7	K24
      0x8C1C0F19,  //  0036  GETMET	R7	R7	K25
      0x88240914,  //  0037  GETMBR	R9	R4	K20
      0x7C1C0400,  //  0038  CALL	R7	2
      0x5C180E00,  //  0039  MOVE	R6	R7
      0x70020004,  //  003A  JMP		#0040
      0x8C1C011A,  //  003B  GETMET	R7	R0	K26
      0x8824091B,  //  003C  GETMBR	R9	R4	K27
      0x8828091C,  //  003D  GETMBR	R10	R4	K28
      0x7C1C0600,  //  003E  CALL	R7	3
      0x901A3A07,  //  003F  SETMBR	R6	K29	R7
      0x4C1C0000,  //  0040  LDNIL	R7
      0x1C1C0C07,  //  0041  EQ	R7	R6	R7
      0x741E0003,  //  0042  JMPT	R7	#0047
      0x881C0D1D,  //  0043  GETMBR	R7	R6	K29
      0x4C200000,  //  0044  LDNIL	R8
      0x1C1C0E08,  //  0045  EQ	R7	R7	R8
      0x781E000D,  //  0046  JMPF	R7	#0055
      0xB81E0C00,  //  0047  GETNGBL	R7	K6
      0x8C1C0F07,  //  0048  GETMET	R7	R7	K7
      0x5824001E,  //  0049  LDCONST	R9	K30
      0x58280009,  //  004A  LDCONST	R10	K9
      0x7C1C0600,  //  004B  CALL	R7	3
      0x8C1C010B,  //  004C  GETMET	R7	R0	K11
      0x5C240200,  //  004D  MOVE	R9	R1
      0x5828000C,  //  004E  LDCONST	R10	K12
      0x582C0004,  //  004F  LDCONST	R11	K4
      0x5830000C,  //  0050  LDCONST	R12	K12
      0x50340000,  //  0051  LDBOOL	R13	0	0
      0x7C1C0C00,  //  0052  CALL	R7	6
      0x50200000,  //  0053  LDBOOL	R8	0	0
      0x80041000,  //  0054  RET	1	R8
      0x881C0320,  //  0055  GETMBR	R7	R1	K32
      0x901A3E07,  //  0056  SETMBR	R6	K31	R7
      0x8C1C0D21,  //  0057  GETMET	R7	R6	K33
      0x7C1C0200,  //  0058  CALL	R7	1
      0x881C0316,  //  0059  GETMBR	R7	R1	K22
      0x201C0E06,  //  005A  NE	R7	R7	R6
      0x781E0004,  //  005B  JMPF	R7	#0061
      0x881C0117,  //  005C  GETMBR	R7	R0	K23
      0x881C0F18,  //  005D  GETMBR	R7	R7	K24
      0x8C1C0F22,  //  005E  GETMET	R7	R7	K34
      0x88240316,  //  005F  GETMBR	R9	R1	K22
      0x7C1C0400,  //  0060  CALL	R7	2
      0x90062C06,  //  0061  SETMBR	R1	K22	R6
      0x881C0924,  //  0062  GETMBR	R7	R4	K36
      0x901A4607,  //  0063  SETMBR	R6	K35	R7
      0x881C0117,  //  0064  GETMBR	R7	R0	K23
      0x881C0F18,  //  0065  GETMBR	R7	R7	K24
      0x8C1C0F26,  //  0066  GETMET	R7	R7	K38
      0x7C1C0200,  //  0067  CALL	R7	1
      0x901A4A07,  //  0068  SETMBR	R6	K37	R7
      0x881C0D25,  //  0069  GETMBR	R7	R6	K37
      0x90024E07,  //  006A  SETMBR	R0	K39	R7
      0xB81E0C00,  //  006B  GETNGBL	R7	K6
      0x8C1C0F07,  //  006C  GETMET	R7	R7	K7
      0x8C240728,  //  006D  GETMET	R9	R3	K40
      0x582C0029,  //  006E  LDCONST	R11	K41
      0x88300127,  //  006F  GETMBR	R12	R0	K39
      0x8834032A,  //  0070  GETMBR	R13	R1	K42
      0x8838032B,  //  0071  GETMBR	R14	R1	K43
      0x7C240A00,  //  0072  CALL	R9	5
      0x58280009,  //  0073  LDCONST	R10	K9
      0x7C1C0600,  //  0074  CALL	R7	3
      0x781600E3,  //  0075  JMPF	R5	#015A
      0x881C091C,  //  0076  GETMBR	R7	R4	K28
      0x88200914,  //  0077  GETMBR	R8	R4	K20
      0x001C0E08,  //  0078  ADD	R7	R7	R8
      0x60200015,  //  0079  GETGBL	R8	G21
      0x7C200000,  //  007A  CALL	R8	0
      0x8C20112C,  //  007B  GETMET	R8	R8	K44
      0x5828002D,  //  007C  LDCONST	R10	K45
      0x7C200400,  //  007D  CALL	R8	2
      0x8C24052E,  //  007E  GETMET	R9	R2	K46
      0x7C240200,  //  007F  CALL	R9	1
      0x8C24132F,  //  0080  GETMET	R9	R9	K47
      0x882C0D30,  //  0081  GETMBR	R11	R6	K48
      0x5C300E00,  //  0082  MOVE	R12	R7
      0x5C341000,  //  0083  MOVE	R13	R8
      0x543A000F,  //  0084  LDINT	R14	16
      0x7C240A00,  //  0085  CALL	R9	5
      0x60280015,  //  0086  GETGBL	R10	G21
      0x7C280000,  //  0087  CALL	R10	0
      0x8C28152C,  //  0088  GETMET	R10	R10	K44
      0x58300031,  //  0089  LDCONST	R12	K49
      0x7C280400,  //  008A  CALL	R10	2
      0x542DFFEE,  //  008B  LDINT	R11	-17
      0x402E080B,  //  008C  CONNECT	R11	K4	R11
      0x88300915,  //  008D  GETMBR	R12	R4	K21
      0x942C180B,  //  008E  GETIDX	R11	R12	R11
      0x5435FFEF,  //  008F  LDINT	R13	-16
      0x40341B32,  //  0090  CONNECT	R13	R13	K50
      0x88380915,  //  0091  GETMBR	R14	R4	K21
      0x94301C0D,  //  0092  GETIDX	R12	R14	R13
      0x8C3C0533,  //  0093  GETMET	R15	R2	K51
      0x5C441200,  //  0094  MOVE	R17	R9
      0x5C481400,  //  0095  MOVE	R18	R10
      0x604C0015,  //  0096  GETGBL	R19	G21
      0x7C4C0000,  //  0097  CALL	R19	0
      0x6050000C,  //  0098  GETGBL	R20	G12
      0x5C541600,  //  0099  MOVE	R21	R11
      0x7C500200,  //  009A  CALL	R20	1
      0x5456000F,  //  009B  LDINT	R21	16
      0x7C3C0C00,  //  009C  CALL	R15	6
      0x5C341E00,  //  009D  MOVE	R13	R15
      0x8C3C1B34,  //  009E  GETMET	R15	R13	K52
      0x5C441600,  //  009F  MOVE	R17	R11
      0x7C3C0400,  //  00A0  CALL	R15	2
      0x5C381E00,  //  00A1  MOVE	R14	R15
      0x8C3C1B35,  //  00A2  GETMET	R15	R13	K53
      0x7C3C0200,  //  00A3  CALL	R15	1
      0xB8420C00,  //  00A4  GETNGBL	R16	K6
      0x8C402107,  //  00A5  GETMET	R16	R16	K7
      0x58480036,  //  00A6  LDCONST	R18	K54
      0x544E0003,  //  00A7  LDINT	R19	4
      0x7C400600,  //  00A8  CALL	R16	3
      0xB8420C00,  //  00A9  GETNGBL	R16	K6
      0x8C402107,  //  00AA  GETMET	R16	R16	K7
      0x8C481338,  //  00AB  GETMET	R18	R9	K56
      0x7C480200,  //  00AC  CALL	R18	1
      0x004A6E12,  //  00AD  ADD	R18	K55	R18
      0x544E0003,  //  00AE  LDINT	R19	4
      0x7C400600,  //  00AF  CALL	R16	3
      0xB8420C00,  //  00B0  GETNGBL	R16	K6
      0x8C402107,  //  00B1  GETMET	R16	R16	K7
      0x8C481938,  //  00B2  GETMET	R18	R12	K56
      0x7C480200,  //  00B3  CALL	R18	1
      0x004A7212,  //  00B4  ADD	R18	K57	R18
      0x544E0003,  //  00B5  LDINT	R19	4
      0x7C400600,  //  00B6  CALL	R16	3
      0xB8420C00,  //  00B7  GETNGBL	R16	K6
      0x8C402107,  //  00B8  GETMET	R16	R16	K7
      0x8C481D38,  //  00B9  GETMET	R18	R14	K56
      0x7C480200,  //  00BA  CALL	R18	1
      0x004A7412,  //  00BB  ADD	R18	K58	R18
      0x544E0003,  //  00BC  LDINT	R19	4
      0x7C400600,  //  00BD  CALL	R16	3
      0xB8420C00,  //  00BE  GETNGBL	R16	K6
      0x8C402107,  //  00BF  GETMET	R16	R16	K7
      0x8C481F38,  //  00C0  GETMET	R18	R15	K56
      0x7C480200,  //  00C1  CALL	R18	1
      0x004A7612,  //  00C2  ADD	R18	K59	R18
      0x544E0003,  //  00C3  LDINT	R19	4
      0x7C400600,  //  00C4  CALL	R16	3
      0xB8420C00,  //  00C5  GETNGBL	R16	K6
      0x8C402107,  //  00C6  GETMET	R16	R16	K7
      0x58480036,  //  00C7  LDCONST	R18	K54
      0x544E0003,  //  00C8  LDINT	R19	4
      0x7C400600,  //  00C9  CALL	R16	3
      0x1C40180F,  //  00CA  EQ	R16	R12	R15
      0x7842008B,  //  00CB  JMPF	R16	#0158
      0x8C40053D,  //  00CC  GETMET	R16	R2	K61
      0x544A000F,  //  00CD  LDINT	R18	16
      0x7C400400,  //  00CE  CALL	R16	2
      0x901A7810,  //  00CF  SETMBR	R6	K60	R16
      0x60400015,  //  00D0  GETGBL	R16	G21
      0x7C400000,  //  00D1  CALL	R16	0
      0x8C40212C,  //  00D2  GETMET	R16	R16	K44
      0x5848003E,  //  00D3  LDCONST	R18	K62
      0x7C400400,  //  00D4  CALL	R16	2
      0x88440D3C,  //  00D5  GETMBR	R17	R6	K60
      0x00402011,  //  00D6  ADD	R16	R16	R17
      0x8844091C,  //  00D7  GETMBR	R17	R4	K28
      0x88480914,  //  00D8  GETMBR	R18	R4	K20
      0x00442212,  //  00D9  ADD	R17	R17	R18
      0x8C48052E,  //  00DA  GETMET	R18	R2	K46
      0x7C480200,  //  00DB  CALL	R18	1
      0x8C48252F,  //  00DC  GETMET	R18	R18	K47
      0x88500D30,  //  00DD  GETMBR	R20	R6	K48
      0x5C542200,  //  00DE  MOVE	R21	R17
      0x5C582000,  //  00DF  MOVE	R22	R16
      0x545E000F,  //  00E0  LDINT	R23	16
      0x7C480A00,  //  00E1  CALL	R18	5
      0x8C4C0533,  //  00E2  GETMET	R19	R2	K51
      0x5C542400,  //  00E3  MOVE	R21	R18
      0x60580015,  //  00E4  GETGBL	R22	G21
      0x7C580000,  //  00E5  CALL	R22	0
      0x8C582D2C,  //  00E6  GETMET	R22	R22	K44
      0x5860003F,  //  00E7  LDCONST	R24	K63
      0x7C580400,  //  00E8  CALL	R22	2
      0x605C0015,  //  00E9  GETGBL	R23	G21
      0x7C5C0000,  //  00EA  CALL	R23	0
      0x58600004,  //  00EB  LDCONST	R24	K4
      0x5466000F,  //  00EC  LDINT	R25	16
      0x7C4C0C00,  //  00ED  CALL	R19	6
      0x8C502735,  //  00EE  GETMET	R20	R19	K53
      0x7C500200,  //  00EF  CALL	R20	1
      0xB8561A00,  //  00F0  GETNGBL	R21	K13
      0x8C542B40,  //  00F1  GETMET	R21	R21	K64
      0x7C540200,  //  00F2  CALL	R21	1
      0x88580D3C,  //  00F3  GETMBR	R22	R6	K60
      0x90562816,  //  00F4  SETMBR	R21	K20	R22
      0x88580D25,  //  00F5  GETMBR	R22	R6	K37
      0x90568216,  //  00F6  SETMBR	R21	K65	R22
      0x90568414,  //  00F7  SETMBR	R21	K66	R20
      0x8C58052E,  //  00F8  GETMET	R22	R2	K46
      0x7C580200,  //  00F9  CALL	R22	1
      0x8C582D2F,  //  00FA  GETMET	R22	R22	K47
      0x88600D30,  //  00FB  GETMBR	R24	R6	K48
      0x8864091C,  //  00FC  GETMBR	R25	R4	K28
      0x88680914,  //  00FD  GETMBR	R26	R4	K20
      0x0064321A,  //  00FE  ADD	R25	R25	R26
      0x60680015,  //  00FF  GETGBL	R26	G21
      0x7C680000,  //  0100  CALL	R26	0
      0x8C68352C,  //  0101  GETMET	R26	R26	K44
      0x58700043,  //  0102  LDCONST	R28	K67
      0x7C680400,  //  0103  CALL	R26	2
      0x546E002F,  //  0104  LDINT	R27	48
      0x7C580A00,  //  0105  CALL	R22	5
      0x545E000E,  //  0106  LDINT	R23	15
      0x405E0817,  //  0107  CONNECT	R23	K4	R23
      0x945C2C17,  //  0108  GETIDX	R23	R22	R23
      0x5462000F,  //  0109  LDINT	R24	16
      0x5466001E,  //  010A  LDINT	R25	31
      0x40603019,  //  010B  CONNECT	R24	R24	R25
      0x94602C18,  //  010C  GETIDX	R24	R22	R24
      0x5466001F,  //  010D  LDINT	R25	32
      0x546A002E,  //  010E  LDINT	R26	47
      0x4064321A,  //  010F  CONNECT	R25	R25	R26
      0x94642C19,  //  0110  GETIDX	R25	R22	R25
      0xB86A0C00,  //  0111  GETNGBL	R26	K6
      0x8C683544,  //  0112  GETMET	R26	R26	K68
      0x7C680200,  //  0113  CALL	R26	1
      0x94683545,  //  0114  GETIDX	R26	R26	K69
      0xB86E0C00,  //  0115  GETNGBL	R27	K6
      0x8C6C3707,  //  0116  GETMET	R27	R27	K7
      0x58740046,  //  0117  LDCONST	R29	K70
      0x547A0003,  //  0118  LDINT	R30	4
      0x7C6C0600,  //  0119  CALL	R27	3
      0xB86E0C00,  //  011A  GETNGBL	R27	K6
      0x8C6C3707,  //  011B  GETMET	R27	R27	K7
      0x8C742F38,  //  011C  GETMET	R29	R23	K56
      0x7C740200,  //  011D  CALL	R29	1
      0x00768E1D,  //  011E  ADD	R29	K71	R29
      0x547A0003,  //  011F  LDINT	R30	4
      0x7C6C0600,  //  0120  CALL	R27	3
      0xB86E0C00,  //  0121  GETNGBL	R27	K6
      0x8C6C3707,  //  0122  GETMET	R27	R27	K7
      0x8C743138,  //  0123  GETMET	R29	R24	K56
      0x7C740200,  //  0124  CALL	R29	1
      0x0076901D,  //  0125  ADD	R29	K72	R29
      0x547A0003,  //  0126  LDINT	R30	4
      0x7C6C0600,  //  0127  CALL	R27	3
      0xB86E0C00,  //  0128  GETNGBL	R27	K6
      0x8C6C3707,  //  0129  GETMET	R27	R27	K7
      0x8C743338,  //  012A  GETMET	R29	R25	K56
      0x7C740200,  //  012B  CALL	R29	1
      0x0076921D,  //  012C  ADD	R29	K73	R29
      0x547A0003,  //  012D  LDINT	R30	4
      0x7C6C0600,  //  012E  CALL	R27	3
      0xB86E0C00,  //  012F  GETNGBL	R27	K6
      0x8C6C3707,  //  0130  GETMET	R27	R27	K7
      0x58740046,  //  0131  LDCONST	R29	K70
      0x547A0003,  //  0132  LDINT	R30	4
      0x7C6C0600,  //  0133  CALL	R27	3
      0x8C6C2B4A,  //  0134  GETMET	R27	R21	K74
      0x7C6C0200,  //  0135  CALL	R27	1
      0x4C700000,  //  0136  LDNIL	R28
      0x901A961C,  //  0137  SETMBR	R6	K75	R28
      0xB8720C00,  //  0138  GETNGBL	R28	K6
      0x8C703907,  //  0139  GETMET	R28	R28	K7
      0x8C783738,  //  013A  GETMET	R30	R27	K56
      0x7C780200,  //  013B  CALL	R30	1
      0x007A981E,  //  013C  ADD	R30	K76	R30
      0x547E0003,  //  013D  LDINT	R31	4
      0x7C700600,  //  013E  CALL	R28	3
      0x8C70034D,  //  013F  GETMET	R28	R1	K77
      0x547A0032,  //  0140  LDINT	R30	51
      0x507C0200,  //  0141  LDBOOL	R31	1	0
      0x7C700600,  //  0142  CALL	R28	3
      0x8C74394E,  //  0143  GETMET	R29	R28	K78
      0x5C7C3600,  //  0144  MOVE	R31	R27
      0x7C740400,  //  0145  CALL	R29	2
      0x8878014F,  //  0146  GETMBR	R30	R0	K79
      0x8C783D50,  //  0147  GETMET	R30	R30	K80
      0x5C803A00,  //  0148  MOVE	R32	R29
      0x8884032A,  //  0149  GETMBR	R33	R1	K42
      0x8888032B,  //  014A  GETMBR	R34	R1	K43
      0x888C3951,  //  014B  GETMBR	R35	R28	K81
      0x88900316,  //  014C  GETMBR	R36	R1	K22
      0x88904903,  //  014D  GETMBR	R36	R36	K3
      0x7C780C00,  //  014E  CALL	R30	6
      0x8C780D52,  //  014F  GETMET	R30	R6	K82
      0x5C802E00,  //  0150  MOVE	R32	R23
      0x5C843000,  //  0151  MOVE	R33	R24
      0x5C883200,  //  0152  MOVE	R34	R25
      0x5C8C3400,  //  0153  MOVE	R35	R26
      0x7C780A00,  //  0154  CALL	R30	5
      0x50780200,  //  0155  LDBOOL	R30	1	0
      0x80043C00,  //  0156  RET	1	R30
      0x70020001,  //  0157  JMP		#015A
      0x4C400000,  //  0158  LDNIL	R16
      0x90122810,  //  0159  SETMBR	R4	K20	R16
      0x881C0914,  //  015A  GETMBR	R7	R4	K20
      0x4C200000,  //  015B  LDNIL	R8
      0x1C1C0E08,  //  015C  EQ	R7	R7	R8
      0x741E0003,  //  015D  JMPT	R7	#0162
      0x881C0915,  //  015E  GETMBR	R7	R4	K21
      0x4C200000,  //  015F  LDNIL	R8
      0x1C1C0E08,  //  0160  EQ	R7	R7	R8
      0x781E011D,  //  0161  JMPF	R7	#0280
      0xB81E0C00,  //  0162  GETNGBL	R7	K6
      0x8C1C0F07,  //  0163  GETMET	R7	R7	K7
      0xB8261A00,  //  0164  GETNGBL	R9	K13
      0x8C241354,  //  0165  GETMET	R9	R9	K84
      0x882C0D1D,  //  0166  GETMBR	R11	R6	K29
      0x7C240400,  //  0167  CALL	R9	2
      0x0026A609,  //  0168  ADD	R9	K83	R9
      0x542A0003,  //  0169  LDINT	R10	4
      0x7C1C0600,  //  016A  CALL	R7	3
      0xB81E0C00,  //  016B  GETNGBL	R7	K6
      0x8C1C0F07,  //  016C  GETMET	R7	R7	K7
      0x88240D1D,  //  016D  GETMBR	R9	R6	K29
      0x88241356,  //  016E  GETMBR	R9	R9	K86
      0x8C241338,  //  016F  GETMET	R9	R9	K56
      0x7C240200,  //  0170  CALL	R9	1
      0x0026AA09,  //  0171  ADD	R9	K85	R9
      0x542A0003,  //  0172  LDINT	R10	4
      0x7C1C0600,  //  0173  CALL	R7	3
      0xB81E0C00,  //  0174  GETNGBL	R7	K6
      0x8C1C0F07,  //  0175  GETMET	R7	R7	K7
      0x88240D1D,  //  0176  GETMBR	R9	R6	K29
      0x88241358,  //  0177  GETMBR	R9	R9	K88
      0x8C241338,  //  0178  GETMET	R9	R9	K56
      0x7C240200,  //  0179  CALL	R9	1
      0x0026AE09,  //  017A  ADD	R9	K87	R9
      0x542A0003,  //  017B  LDINT	R10	4
      0x7C1C0600,  //  017C  CALL	R7	3
      0xB81E0C00,  //  017D  GETNGBL	R7	K6
      0x8C1C0F07,  //  017E  GETMET	R7	R7	K7
      0x88240D1D,  //  017F  GETMBR	R9	R6	K29
      0x8824135A,  //  0180  GETMBR	R9	R9	K90
      0x8C241338,  //  0181  GETMET	R9	R9	K56
      0x7C240200,  //  0182  CALL	R9	1
      0x0026B209,  //  0183  ADD	R9	K89	R9
      0x542A0003,  //  0184  LDINT	R10	4
      0x7C1C0600,  //  0185  CALL	R7	3
      0x8C1C053D,  //  0186  GETMET	R7	R2	K61
      0x5426000F,  //  0187  LDINT	R9	16
      0x7C1C0400,  //  0188  CALL	R7	2
      0x901A7807,  //  0189  SETMBR	R6	K60	R7
      0x8C1C053D,  //  018A  GETMET	R7	R2	K61
      0x5426001F,  //  018B  LDINT	R9	32
      0x7C1C0400,  //  018C  CALL	R7	2
      0x9002B607,  //  018D  SETMBR	R0	K91	R7
      0x8C1C055D,  //  018E  GETMET	R7	R2	K93
      0x7C1C0200,  //  018F  CALL	R7	1
      0x8C1C0F5E,  //  0190  GETMET	R7	R7	K94
      0x8824015B,  //  0191  GETMBR	R9	R0	K91
      0x7C1C0400,  //  0192  CALL	R7	2
      0x9002B807,  //  0193  SETMBR	R0	K92	R7
      0x8C1C053D,  //  0194  GETMET	R7	R2	K61
      0x5426001F,  //  0195  LDINT	R9	32
      0x7C1C0400,  //  0196  CALL	R7	2
      0x8C20055D,  //  0197  GETMET	R8	R2	K93
      0x7C200200,  //  0198  CALL	R8	1
      0x8C20115F,  //  0199  GETMET	R8	R8	K95
      0x8828015B,  //  019A  GETMBR	R10	R0	K91
      0x882C0913,  //  019B  GETMBR	R11	R4	K19
      0x7C200600,  //  019C  CALL	R8	3
      0x901A6008,  //  019D  SETMBR	R6	K48	R8
      0xB8221A00,  //  019E  GETNGBL	R8	K13
      0x88201160,  //  019F  GETMBR	R8	R8	K96
      0x8C201161,  //  01A0  GETMET	R8	R8	K97
      0x7C200200,  //  01A1  CALL	R8	1
      0x8C241162,  //  01A2  GETMET	R9	R8	K98
      0x582C000C,  //  01A3  LDCONST	R11	K12
      0xB8321A00,  //  01A4  GETNGBL	R12	K13
      0x88301960,  //  01A5  GETMBR	R12	R12	K96
      0x88301963,  //  01A6  GETMBR	R12	R12	K99
      0x8C340D64,  //  01A7  GETMET	R13	R6	K100
      0x7C340200,  //  01A8  CALL	R13	1
      0x7C240800,  //  01A9  CALL	R9	4
      0x8C241162,  //  01AA  GETMET	R9	R8	K98
      0x582C0009,  //  01AB  LDCONST	R11	K9
      0xB8321A00,  //  01AC  GETNGBL	R12	K13
      0x88301960,  //  01AD  GETMBR	R12	R12	K96
      0x88301963,  //  01AE  GETMBR	R12	R12	K99
      0x8C340D65,  //  01AF  GETMET	R13	R6	K101
      0x7C340200,  //  01B0  CALL	R13	1
      0x7C240800,  //  01B1  CALL	R9	4
      0x8C241162,  //  01B2  GETMET	R9	R8	K98
      0x582C0066,  //  01B3  LDCONST	R11	K102
      0xB8321A00,  //  01B4  GETNGBL	R12	K13
      0x88301960,  //  01B5  GETMBR	R12	R12	K96
      0x88301963,  //  01B6  GETMBR	R12	R12	K99
      0x8834015C,  //  01B7  GETMBR	R13	R0	K92
      0x7C240800,  //  01B8  CALL	R9	4
      0x8C241162,  //  01B9  GETMET	R9	R8	K98
      0x542E0003,  //  01BA  LDINT	R11	4
      0xB8321A00,  //  01BB  GETNGBL	R12	K13
      0x88301960,  //  01BC  GETMBR	R12	R12	K96
      0x88301963,  //  01BD  GETMBR	R12	R12	K99
      0x88340913,  //  01BE  GETMBR	R13	R4	K19
      0x7C240800,  //  01BF  CALL	R9	4
      0x8C24055D,  //  01C0  GETMET	R9	R2	K93
      0x7C240200,  //  01C1  CALL	R9	1
      0x8C241367,  //  01C2  GETMET	R9	R9	K103
      0x8C2C0D68,  //  01C3  GETMET	R11	R6	K104
      0x7C2C0200,  //  01C4  CALL	R11	1
      0x8C30114A,  //  01C5  GETMET	R12	R8	K74
      0x7C300200,  //  01C6  CALL	R12	1
      0x7C240600,  //  01C7  CALL	R9	3
      0xB82A1A00,  //  01C8  GETNGBL	R10	K13
      0x88281560,  //  01C9  GETMBR	R10	R10	K96
      0x8C281561,  //  01CA  GETMET	R10	R10	K97
      0x7C280200,  //  01CB  CALL	R10	1
      0x8C2C1562,  //  01CC  GETMET	R11	R10	K98
      0x5834000C,  //  01CD  LDCONST	R13	K12
      0xB83A1A00,  //  01CE  GETNGBL	R14	K13
      0x88381D60,  //  01CF  GETMBR	R14	R14	K96
      0x88381D63,  //  01D0  GETMBR	R14	R14	K99
      0x8C3C0D64,  //  01D1  GETMET	R15	R6	K100
      0x7C3C0200,  //  01D2  CALL	R15	1
      0x7C2C0800,  //  01D3  CALL	R11	4
      0x8C2C1562,  //  01D4  GETMET	R11	R10	K98
      0x58340009,  //  01D5  LDCONST	R13	K9
      0xB83A1A00,  //  01D6  GETNGBL	R14	K13
      0x88381D60,  //  01D7  GETMBR	R14	R14	K96
      0x88381D63,  //  01D8  GETMBR	R14	R14	K99
      0x8C3C0D65,  //  01D9  GETMET	R15	R6	K101
      0x7C3C0200,  //  01DA  CALL	R15	1
      0x7C2C0800,  //  01DB  CALL	R11	4
      0x8C2C1562,  //  01DC  GETMET	R11	R10	K98
      0x58340066,  //  01DD  LDCONST	R13	K102
      0xB83A1A00,  //  01DE  GETNGBL	R14	K13
      0x88381D60,  //  01DF  GETMBR	R14	R14	K96
      0x88381D63,  //  01E0  GETMBR	R14	R14	K99
      0x5C3C1200,  //  01E1  MOVE	R15	R9
      0x7C2C0800,  //  01E2  CALL	R11	4
      0x8C2C1562,  //  01E3  GETMET	R11	R10	K98
      0x54360003,  //  01E4  LDINT	R13	4
      0xB83A1A00,  //  01E5  GETNGBL	R14	K13
      0x88381D60,  //  01E6  GETMBR	R14	R14	K96
      0x88381D63,  //  01E7  GETMBR	R14	R14	K99
      0x883C0D3C,  //  01E8  GETMBR	R15	R6	K60
      0x7C2C0800,  //  01E9  CALL	R11	4
      0xB82E0C00,  //  01EA  GETNGBL	R11	K6
      0x8C2C1707,  //  01EB  GETMET	R11	R11	K7
      0x58340036,  //  01EC  LDCONST	R13	K54
      0x543A0003,  //  01ED  LDINT	R14	4
      0x7C2C0600,  //  01EE  CALL	R11	3
      0x882C0969,  //  01EF  GETMBR	R11	R4	K105
      0x901A960B,  //  01F0  SETMBR	R6	K75	R11
      0xB82E0C00,  //  01F1  GETNGBL	R11	K6
      0x8C2C1707,  //  01F2  GETMET	R11	R11	K7
      0x88340D4B,  //  01F3  GETMBR	R13	R6	K75
      0x8C341B38,  //  01F4  GETMET	R13	R13	K56
      0x7C340200,  //  01F5  CALL	R13	1
      0x0036D40D,  //  01F6  ADD	R13	K106	R13
      0x543A0003,  //  01F7  LDINT	R14	4
      0x7C2C0600,  //  01F8  CALL	R11	3
      0x8C2C056B,  //  01F9  GETMET	R11	R2	K107
      0x7C2C0200,  //  01FA  CALL	R11	1
      0x8C2C176C,  //  01FB  GETMET	R11	R11	K108
      0x88340D4B,  //  01FC  GETMBR	R13	R6	K75
      0x7C2C0400,  //  01FD  CALL	R11	2
      0x8C2C176D,  //  01FE  GETMET	R11	R11	K109
      0x7C2C0200,  //  01FF  CALL	R11	1
      0xB8320C00,  //  0200  GETNGBL	R12	K6
      0x8C301907,  //  0201  GETMET	R12	R12	K7
      0x8C381738,  //  0202  GETMET	R14	R11	K56
      0x7C380200,  //  0203  CALL	R14	1
      0x003ADC0E,  //  0204  ADD	R14	K110	R14
      0x543E0003,  //  0205  LDINT	R15	4
      0x7C300600,  //  0206  CALL	R12	3
      0x60300015,  //  0207  GETGBL	R12	G21
      0x7C300000,  //  0208  CALL	R12	0
      0x8C30192C,  //  0209  GETMET	R12	R12	K44
      0x8838016F,  //  020A  GETMBR	R14	R0	K111
      0x7C300400,  //  020B  CALL	R12	2
      0x8C340D70,  //  020C  GETMET	R13	R6	K112
      0x7C340200,  //  020D  CALL	R13	1
      0x00341A07,  //  020E  ADD	R13	R13	R7
      0x8838015C,  //  020F  GETMBR	R14	R0	K92
      0x00341A0E,  //  0210  ADD	R13	R13	R14
      0x00341A0B,  //  0211  ADD	R13	R13	R11
      0x8C38052E,  //  0212  GETMET	R14	R2	K46
      0x7C380200,  //  0213  CALL	R14	1
      0x8C381D2F,  //  0214  GETMET	R14	R14	K47
      0x88400D30,  //  0215  GETMBR	R16	R6	K48
      0x5C441A00,  //  0216  MOVE	R17	R13
      0x5C481800,  //  0217  MOVE	R18	R12
      0x544E000F,  //  0218  LDINT	R19	16
      0x7C380A00,  //  0219  CALL	R14	5
      0xB83E0C00,  //  021A  GETNGBL	R15	K6
      0x8C3C1F07,  //  021B  GETMET	R15	R15	K7
      0x88440D30,  //  021C  GETMBR	R17	R6	K48
      0x8C442338,  //  021D  GETMET	R17	R17	K56
      0x7C440200,  //  021E  CALL	R17	1
      0x0046E211,  //  021F  ADD	R17	K113	R17
      0x544A0003,  //  0220  LDINT	R18	4
      0x7C3C0600,  //  0221  CALL	R15	3
      0xB83E0C00,  //  0222  GETNGBL	R15	K6
      0x8C3C1F07,  //  0223  GETMET	R15	R15	K7
      0x8C441B38,  //  0224  GETMET	R17	R13	K56
      0x7C440200,  //  0225  CALL	R17	1
      0x0046E411,  //  0226  ADD	R17	K114	R17
      0x544A0003,  //  0227  LDINT	R18	4
      0x7C3C0600,  //  0228  CALL	R15	3
      0xB83E0C00,  //  0229  GETNGBL	R15	K6
      0x8C3C1F07,  //  022A  GETMET	R15	R15	K7
      0x8C441D38,  //  022B  GETMET	R17	R14	K56
      0x7C440200,  //  022C  CALL	R17	1
      0x0046E611,  //  022D  ADD	R17	K115	R17
      0x544A0003,  //  022E  LDINT	R18	4
      0x7C3C0600,  //  022F  CALL	R15	3
      0x8C3C154A,  //  0230  GETMET	R15	R10	K74
      0x7C3C0200,  //  0231  CALL	R15	1
      0x8C400533,  //  0232  GETMET	R16	R2	K51
      0x5C481C00,  //  0233  MOVE	R18	R14
      0x604C0015,  //  0234  GETGBL	R19	G21
      0x7C4C0000,  //  0235  CALL	R19	0
      0x8C4C272C,  //  0236  GETMET	R19	R19	K44
      0x88540174,  //  0237  GETMBR	R21	R0	K116
      0x7C4C0400,  //  0238  CALL	R19	2
      0x60500015,  //  0239  GETGBL	R20	G21
      0x7C500000,  //  023A  CALL	R20	0
      0x6054000C,  //  023B  GETGBL	R21	G12
      0x5C581E00,  //  023C  MOVE	R22	R15
      0x7C540200,  //  023D  CALL	R21	1
      0x545A000F,  //  023E  LDINT	R22	16
      0x7C400C00,  //  023F  CALL	R16	6
      0x8C442175,  //  0240  GETMET	R17	R16	K117
      0x5C4C1E00,  //  0241  MOVE	R19	R15
      0x7C440400,  //  0242  CALL	R17	2
      0x8C482135,  //  0243  GETMET	R18	R16	K53
      0x7C480200,  //  0244  CALL	R18	1
      0x00442212,  //  0245  ADD	R17	R17	R18
      0xB84A0C00,  //  0246  GETNGBL	R18	K6
      0x8C482507,  //  0247  GETMET	R18	R18	K7
      0x8C502338,  //  0248  GETMET	R20	R17	K56
      0x7C500200,  //  0249  CALL	R20	1
      0x0052EC14,  //  024A  ADD	R20	K118	R20
      0x54560003,  //  024B  LDINT	R21	4
      0x7C480600,  //  024C  CALL	R18	3
      0xB84A0C00,  //  024D  GETNGBL	R18	K6
      0x8C482507,  //  024E  GETMET	R18	R18	K7
      0x58500036,  //  024F  LDCONST	R20	K54
      0x54560003,  //  0250  LDINT	R21	4
      0x7C480600,  //  0251  CALL	R18	3
      0xB84A1A00,  //  0252  GETNGBL	R18	K13
      0x8C482577,  //  0253  GETMET	R18	R18	K119
      0x7C480200,  //  0254  CALL	R18	1
      0x904AF007,  //  0255  SETMBR	R18	K120	R7
      0x884C0127,  //  0256  GETMBR	R19	R0	K39
      0x904AF213,  //  0257  SETMBR	R18	K121	R19
      0x884C015C,  //  0258  GETMBR	R19	R0	K92
      0x904AF413,  //  0259  SETMBR	R18	K122	R19
      0x904AF611,  //  025A  SETMBR	R18	K123	R17
      0xB84E0C00,  //  025B  GETNGBL	R19	K6
      0x8C4C2707,  //  025C  GETMET	R19	R19	K7
      0xB8561A00,  //  025D  GETNGBL	R21	K13
      0x8C542B54,  //  025E  GETMET	R21	R21	K84
      0x5C5C2400,  //  025F  MOVE	R23	R18
      0x7C540400,  //  0260  CALL	R21	2
      0x0056F815,  //  0261  ADD	R21	K124	R21
      0x545A0003,  //  0262  LDINT	R22	4
      0x7C4C0600,  //  0263  CALL	R19	3
      0x8C4C254A,  //  0264  GETMET	R19	R18	K74
      0x7C4C0200,  //  0265  CALL	R19	1
      0x901AFA13,  //  0266  SETMBR	R6	K125	R19
      0xB8520C00,  //  0267  GETNGBL	R20	K6
      0x8C502907,  //  0268  GETMET	R20	R20	K7
      0x8C582738,  //  0269  GETMET	R22	R19	K56
      0x7C580200,  //  026A  CALL	R22	1
      0x005AFC16,  //  026B  ADD	R22	K126	R22
      0x545E0003,  //  026C  LDINT	R23	4
      0x7C500600,  //  026D  CALL	R20	3
      0x8C50034D,  //  026E  GETMET	R20	R1	K77
      0x545A0030,  //  026F  LDINT	R22	49
      0x505C0200,  //  0270  LDBOOL	R23	1	0
      0x7C500600,  //  0271  CALL	R20	3
      0x8C54294E,  //  0272  GETMET	R21	R20	K78
      0x5C5C2600,  //  0273  MOVE	R23	R19
      0x7C540400,  //  0274  CALL	R21	2
      0x8858014F,  //  0275  GETMBR	R22	R0	K79
      0x8C582D50,  //  0276  GETMET	R22	R22	K80
      0x5C602A00,  //  0277  MOVE	R24	R21
      0x8864032A,  //  0278  GETMBR	R25	R1	K42
      0x8868032B,  //  0279  GETMBR	R26	R1	K43
      0x886C2951,  //  027A  GETMBR	R27	R20	K81
      0x88700316,  //  027B  GETMBR	R28	R1	K22
      0x88703903,  //  027C  GETMBR	R28	R28	K3
      0x7C580C00,  //  027D  CALL	R22	6
      0x50580200,  //  027E  LDBOOL	R22	1	0
      0x80042C00,  //  027F  RET	1	R22
      0x501C0200,  //  0280  LDBOOL	R7	1	0
      0x80040E00,  //  0281  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_status_report
********************************************************************/
be_local_closure(Matter_Commisioning_Context_send_status_report,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(build_response),
    /* K1   */  be_nested_str_weak(add),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str_weak(encode_frame),
    /* K4   */  be_nested_str_weak(responder),
    /* K5   */  be_nested_str_weak(send_response),
    /* K6   */  be_nested_str_weak(remote_ip),
    /* K7   */  be_nested_str_weak(remote_port),
    /* K8   */  be_nested_str_weak(message_counter),
    /* K9   */  be_nested_str_weak(session),
    /* K10  */  be_nested_str_weak(local_session_id),
    }),
    be_str_weak(send_status_report),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
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
      0x5C2C1000,  //  0017  MOVE	R11	R8
      0x88300306,  //  0018  GETMBR	R12	R1	K6
      0x88340307,  //  0019  GETMBR	R13	R1	K7
      0x78160001,  //  001A  JMPF	R5	#001D
      0x88380D08,  //  001B  GETMBR	R14	R6	K8
      0x70020000,  //  001C  JMP		#001E
      0x4C380000,  //  001D  LDNIL	R14
      0x883C0309,  //  001E  GETMBR	R15	R1	K9
      0x883C1F0A,  //  001F  GETMBR	R15	R15	K10
      0x7C240C00,  //  0020  CALL	R9	6
      0x80000000,  //  0021  RET	0
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
