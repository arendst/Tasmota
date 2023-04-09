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
    ( &(const bvalue[133]) {     /* constants */
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
    /* K29  */  be_nested_str_weak(find_fabric_by_destination_id),
    /* K30  */  be_nested_str_weak(destinationId),
    /* K31  */  be_nested_str_weak(initiatorRandom),
    /* K32  */  be_nested_str_weak(_fabric),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K34  */  be_nested_str_weak(_source_node_id),
    /* K35  */  be_nested_str_weak(source_node_id),
    /* K36  */  be_nested_str_weak(set_mode_CASE),
    /* K37  */  be_nested_str_weak(remove_session),
    /* K38  */  be_nested_str_weak(__future_initiator_session_id),
    /* K39  */  be_nested_str_weak(initiator_session_id),
    /* K40  */  be_nested_str_weak(__future_local_session_id),
    /* K41  */  be_nested_str_weak(gen_local_session_id),
    /* K42  */  be_nested_str_weak(future_local_session_id),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K44  */  be_nested_str_weak(remote_ip),
    /* K45  */  be_nested_str_weak(remote_port),
    /* K46  */  be_nested_str_weak(fromstring),
    /* K47  */  be_nested_str_weak(Sigma1_Resume),
    /* K48  */  be_nested_str_weak(HKDF_SHA256),
    /* K49  */  be_nested_str_weak(derive),
    /* K50  */  be_nested_str_weak(shared_secret),
    /* K51  */  be_nested_str_weak(NCASE_SigmaR1),
    /* K52  */  be_const_int(2147483647),
    /* K53  */  be_nested_str_weak(AES_CCM),
    /* K54  */  be_nested_str_weak(decrypt),
    /* K55  */  be_nested_str_weak(tag),
    /* K56  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K57  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s1rk_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K58  */  be_nested_str_weak(tohex),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20tag_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K60  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20Resume1MICPayload_X20_X3D_X20),
    /* K61  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20decrypted_tag_X20_X20_X20_X20_X20_X3D_X20),
    /* K62  */  be_nested_str_weak(resumption_id),
    /* K63  */  be_nested_str_weak(random),
    /* K64  */  be_nested_str_weak(Sigma2_Resume),
    /* K65  */  be_nested_str_weak(NCASE_SigmaR2),
    /* K66  */  be_nested_str_weak(Sigma2Resume),
    /* K67  */  be_nested_str_weak(responderSessionID),
    /* K68  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K69  */  be_nested_str_weak(SessionResumptionKeys),
    /* K70  */  be_nested_str_weak(rtc),
    /* K71  */  be_nested_str_weak(utc),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K76  */  be_nested_str_weak(tlv2raw),
    /* K77  */  be_nested_str_weak(__Msg1),
    /* K78  */  be_nested_str_weak(MTR_X3A_X20sigma2resume_raw_X3A_X20),
    /* K79  */  be_nested_str_weak(build_response),
    /* K80  */  be_nested_str_weak(encode_frame),
    /* K81  */  be_nested_str_weak(responder),
    /* K82  */  be_nested_str_weak(send_response),
    /* K83  */  be_nested_str_weak(message_counter),
    /* K84  */  be_nested_str_weak(set_keys),
    /* K85  */  be_nested_str_weak(MTR_X3A_X20fabric_X3D),
    /* K86  */  be_nested_str_weak(MTR_X3A_X20no_private_key_X3D),
    /* K87  */  be_nested_str_weak(no_private_key),
    /* K88  */  be_nested_str_weak(MTR_X3A_X20noc_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K89  */  be_nested_str_weak(noc),
    /* K90  */  be_nested_str_weak(get_icac),
    /* K91  */  be_nested_str_weak(MTR_X3A_X20icac_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K92  */  be_nested_str_weak(MTR_X3A_X20root_ca_cert_X20_X20_X3D),
    /* K93  */  be_nested_str_weak(root_ca_certificate),
    /* K94  */  be_nested_str_weak(ResponderEph_priv),
    /* K95  */  be_nested_str_weak(ResponderEph_pub),
    /* K96  */  be_nested_str_weak(EC_P256),
    /* K97  */  be_nested_str_weak(public_key),
    /* K98  */  be_nested_str_weak(MTR_X3A_X20ResponderEph_priv_X20_X20_X3D),
    /* K99  */  be_nested_str_weak(MTR_X3A_X20ResponderEph_pub_X20_X20_X3D),
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
    /* K110 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20resumptionid_X20_X20_X3D_X20),
    /* K111 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20MSG1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K112 */  be_nested_str_weak(SHA256),
    /* K113 */  be_nested_str_weak(update),
    /* K114 */  be_nested_str_weak(out),
    /* K115 */  be_nested_str_weak(MTR_X3A_X20TranscriptHash_X20_X3D),
    /* K116 */  be_nested_str_weak(S2K_Info),
    /* K117 */  be_nested_str_weak(get_ipk_group_key),
    /* K118 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20SharedSecret_X20_X20_X3D_X20),
    /* K119 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K120 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K121 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Raw_X20_X20_X20_X3D_X20),
    /* K122 */  be_nested_str_weak(TBEData2_Nonce),
    /* K123 */  be_nested_str_weak(encrypt),
    /* K124 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Enc_X20_X20_X20_X3D_X20),
    /* K125 */  be_nested_str_weak(Sigma2),
    /* K126 */  be_nested_str_weak(responderRandom),
    /* K127 */  be_nested_str_weak(responderSessionId),
    /* K128 */  be_nested_str_weak(responderEphPubKey),
    /* K129 */  be_nested_str_weak(encrypted2),
    /* K130 */  be_nested_str_weak(MTR_X3A_X20sigma2_X3A_X20),
    /* K131 */  be_nested_str_weak(__Msg2),
    /* K132 */  be_nested_str_weak(MTR_X3A_X20sigma2_raw_X3A_X20),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[698]) {  /* code */
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
      0x78160006,  //  0045  JMPF	R5	#004D
      0x881C011A,  //  0046  GETMBR	R7	R0	K26
      0x881C0F1B,  //  0047  GETMBR	R7	R7	K27
      0x8C1C0F1C,  //  0048  GETMET	R7	R7	K28
      0x88240916,  //  0049  GETMBR	R9	R4	K22
      0x7C1C0400,  //  004A  CALL	R7	2
      0x5C180E00,  //  004B  MOVE	R6	R7
      0x70020004,  //  004C  JMP		#0052
      0x8C1C011D,  //  004D  GETMET	R7	R0	K29
      0x8824091E,  //  004E  GETMBR	R9	R4	K30
      0x8828091F,  //  004F  GETMBR	R10	R4	K31
      0x7C1C0600,  //  0050  CALL	R7	3
      0x901A4007,  //  0051  SETMBR	R6	K32	R7
      0x4C1C0000,  //  0052  LDNIL	R7
      0x1C1C0C07,  //  0053  EQ	R7	R6	R7
      0x741E0003,  //  0054  JMPT	R7	#0059
      0x881C0D20,  //  0055  GETMBR	R7	R6	K32
      0x4C200000,  //  0056  LDNIL	R8
      0x1C1C0E08,  //  0057  EQ	R7	R7	R8
      0x781E000D,  //  0058  JMPF	R7	#0067
      0xB81E0C00,  //  0059  GETNGBL	R7	K6
      0x8C1C0F07,  //  005A  GETMET	R7	R7	K7
      0x58240021,  //  005B  LDCONST	R9	K33
      0x58280009,  //  005C  LDCONST	R10	K9
      0x7C1C0600,  //  005D  CALL	R7	3
      0x8C1C010A,  //  005E  GETMET	R7	R0	K10
      0x5C240200,  //  005F  MOVE	R9	R1
      0x5828000B,  //  0060  LDCONST	R10	K11
      0x582C0004,  //  0061  LDCONST	R11	K4
      0x5830000B,  //  0062  LDCONST	R12	K11
      0x50340000,  //  0063  LDBOOL	R13	0	0
      0x7C1C0C00,  //  0064  CALL	R7	6
      0x50200000,  //  0065  LDBOOL	R8	0	0
      0x80041000,  //  0066  RET	1	R8
      0x881C0323,  //  0067  GETMBR	R7	R1	K35
      0x901A4407,  //  0068  SETMBR	R6	K34	R7
      0x8C1C0D24,  //  0069  GETMET	R7	R6	K36
      0x7C1C0200,  //  006A  CALL	R7	1
      0x881C0319,  //  006B  GETMBR	R7	R1	K25
      0x201C0E06,  //  006C  NE	R7	R7	R6
      0x781E0004,  //  006D  JMPF	R7	#0073
      0x881C011A,  //  006E  GETMBR	R7	R0	K26
      0x881C0F1B,  //  006F  GETMBR	R7	R7	K27
      0x8C1C0F25,  //  0070  GETMET	R7	R7	K37
      0x88240319,  //  0071  GETMBR	R9	R1	K25
      0x7C1C0400,  //  0072  CALL	R7	2
      0x90063206,  //  0073  SETMBR	R1	K25	R6
      0x881C0927,  //  0074  GETMBR	R7	R4	K39
      0x901A4C07,  //  0075  SETMBR	R6	K38	R7
      0x881C011A,  //  0076  GETMBR	R7	R0	K26
      0x881C0F1B,  //  0077  GETMBR	R7	R7	K27
      0x8C1C0F29,  //  0078  GETMET	R7	R7	K41
      0x7C1C0200,  //  0079  CALL	R7	1
      0x901A5007,  //  007A  SETMBR	R6	K40	R7
      0x881C0D28,  //  007B  GETMBR	R7	R6	K40
      0x90025407,  //  007C  SETMBR	R0	K42	R7
      0xB81E0C00,  //  007D  GETNGBL	R7	K6
      0x8C1C0F07,  //  007E  GETMET	R7	R7	K7
      0x8C240711,  //  007F  GETMET	R9	R3	K17
      0x582C002B,  //  0080  LDCONST	R11	K43
      0x8830012A,  //  0081  GETMBR	R12	R0	K42
      0x8834032C,  //  0082  GETMBR	R13	R1	K44
      0x8838032D,  //  0083  GETMBR	R14	R1	K45
      0x7C240A00,  //  0084  CALL	R9	5
      0x58280009,  //  0085  LDCONST	R10	K9
      0x7C1C0600,  //  0086  CALL	R7	3
      0x781600E2,  //  0087  JMPF	R5	#016B
      0x881C091F,  //  0088  GETMBR	R7	R4	K31
      0x88200916,  //  0089  GETMBR	R8	R4	K22
      0x001C0E08,  //  008A  ADD	R7	R7	R8
      0x60200015,  //  008B  GETGBL	R8	G21
      0x7C200000,  //  008C  CALL	R8	0
      0x8C20112E,  //  008D  GETMET	R8	R8	K46
      0x5828002F,  //  008E  LDCONST	R10	K47
      0x7C200400,  //  008F  CALL	R8	2
      0x8C240530,  //  0090  GETMET	R9	R2	K48
      0x7C240200,  //  0091  CALL	R9	1
      0x8C241331,  //  0092  GETMET	R9	R9	K49
      0x882C0D32,  //  0093  GETMBR	R11	R6	K50
      0x5C300E00,  //  0094  MOVE	R12	R7
      0x5C341000,  //  0095  MOVE	R13	R8
      0x543A000F,  //  0096  LDINT	R14	16
      0x7C240A00,  //  0097  CALL	R9	5
      0x60280015,  //  0098  GETGBL	R10	G21
      0x7C280000,  //  0099  CALL	R10	0
      0x8C28152E,  //  009A  GETMET	R10	R10	K46
      0x58300033,  //  009B  LDCONST	R12	K51
      0x7C280400,  //  009C  CALL	R10	2
      0x542DFFEE,  //  009D  LDINT	R11	-17
      0x402E080B,  //  009E  CONNECT	R11	K4	R11
      0x88300917,  //  009F  GETMBR	R12	R4	K23
      0x942C180B,  //  00A0  GETIDX	R11	R12	R11
      0x5435FFEF,  //  00A1  LDINT	R13	-16
      0x40341B34,  //  00A2  CONNECT	R13	R13	K52
      0x88380917,  //  00A3  GETMBR	R14	R4	K23
      0x94301C0D,  //  00A4  GETIDX	R12	R14	R13
      0x8C3C0535,  //  00A5  GETMET	R15	R2	K53
      0x5C441200,  //  00A6  MOVE	R17	R9
      0x5C481400,  //  00A7  MOVE	R18	R10
      0x604C0015,  //  00A8  GETGBL	R19	G21
      0x7C4C0000,  //  00A9  CALL	R19	0
      0x6050000C,  //  00AA  GETGBL	R20	G12
      0x5C541600,  //  00AB  MOVE	R21	R11
      0x7C500200,  //  00AC  CALL	R20	1
      0x5456000F,  //  00AD  LDINT	R21	16
      0x7C3C0C00,  //  00AE  CALL	R15	6
      0x5C341E00,  //  00AF  MOVE	R13	R15
      0x8C3C1B36,  //  00B0  GETMET	R15	R13	K54
      0x5C441600,  //  00B1  MOVE	R17	R11
      0x7C3C0400,  //  00B2  CALL	R15	2
      0x5C381E00,  //  00B3  MOVE	R14	R15
      0x8C3C1B37,  //  00B4  GETMET	R15	R13	K55
      0x7C3C0200,  //  00B5  CALL	R15	1
      0xB8420C00,  //  00B6  GETNGBL	R16	K6
      0x8C402107,  //  00B7  GETMET	R16	R16	K7
      0x58480038,  //  00B8  LDCONST	R18	K56
      0x544E0003,  //  00B9  LDINT	R19	4
      0x7C400600,  //  00BA  CALL	R16	3
      0xB8420C00,  //  00BB  GETNGBL	R16	K6
      0x8C402107,  //  00BC  GETMET	R16	R16	K7
      0x8C48133A,  //  00BD  GETMET	R18	R9	K58
      0x7C480200,  //  00BE  CALL	R18	1
      0x004A7212,  //  00BF  ADD	R18	K57	R18
      0x544E0003,  //  00C0  LDINT	R19	4
      0x7C400600,  //  00C1  CALL	R16	3
      0xB8420C00,  //  00C2  GETNGBL	R16	K6
      0x8C402107,  //  00C3  GETMET	R16	R16	K7
      0x8C48193A,  //  00C4  GETMET	R18	R12	K58
      0x7C480200,  //  00C5  CALL	R18	1
      0x004A7612,  //  00C6  ADD	R18	K59	R18
      0x544E0003,  //  00C7  LDINT	R19	4
      0x7C400600,  //  00C8  CALL	R16	3
      0xB8420C00,  //  00C9  GETNGBL	R16	K6
      0x8C402107,  //  00CA  GETMET	R16	R16	K7
      0x8C481D3A,  //  00CB  GETMET	R18	R14	K58
      0x7C480200,  //  00CC  CALL	R18	1
      0x004A7812,  //  00CD  ADD	R18	K60	R18
      0x544E0003,  //  00CE  LDINT	R19	4
      0x7C400600,  //  00CF  CALL	R16	3
      0xB8420C00,  //  00D0  GETNGBL	R16	K6
      0x8C402107,  //  00D1  GETMET	R16	R16	K7
      0x8C481F3A,  //  00D2  GETMET	R18	R15	K58
      0x7C480200,  //  00D3  CALL	R18	1
      0x004A7A12,  //  00D4  ADD	R18	K61	R18
      0x544E0003,  //  00D5  LDINT	R19	4
      0x7C400600,  //  00D6  CALL	R16	3
      0xB8420C00,  //  00D7  GETNGBL	R16	K6
      0x8C402107,  //  00D8  GETMET	R16	R16	K7
      0x58480038,  //  00D9  LDCONST	R18	K56
      0x544E0003,  //  00DA  LDINT	R19	4
      0x7C400600,  //  00DB  CALL	R16	3
      0x1C40180F,  //  00DC  EQ	R16	R12	R15
      0x7842008B,  //  00DD  JMPF	R16	#016A
      0x8C40053F,  //  00DE  GETMET	R16	R2	K63
      0x544A000F,  //  00DF  LDINT	R18	16
      0x7C400400,  //  00E0  CALL	R16	2
      0x901A7C10,  //  00E1  SETMBR	R6	K62	R16
      0x60400015,  //  00E2  GETGBL	R16	G21
      0x7C400000,  //  00E3  CALL	R16	0
      0x8C40212E,  //  00E4  GETMET	R16	R16	K46
      0x58480040,  //  00E5  LDCONST	R18	K64
      0x7C400400,  //  00E6  CALL	R16	2
      0x88440D3E,  //  00E7  GETMBR	R17	R6	K62
      0x00402011,  //  00E8  ADD	R16	R16	R17
      0x8844091F,  //  00E9  GETMBR	R17	R4	K31
      0x88480916,  //  00EA  GETMBR	R18	R4	K22
      0x00442212,  //  00EB  ADD	R17	R17	R18
      0x8C480530,  //  00EC  GETMET	R18	R2	K48
      0x7C480200,  //  00ED  CALL	R18	1
      0x8C482531,  //  00EE  GETMET	R18	R18	K49
      0x88500D32,  //  00EF  GETMBR	R20	R6	K50
      0x5C542200,  //  00F0  MOVE	R21	R17
      0x5C582000,  //  00F1  MOVE	R22	R16
      0x545E000F,  //  00F2  LDINT	R23	16
      0x7C480A00,  //  00F3  CALL	R18	5
      0x8C4C0535,  //  00F4  GETMET	R19	R2	K53
      0x5C542400,  //  00F5  MOVE	R21	R18
      0x60580015,  //  00F6  GETGBL	R22	G21
      0x7C580000,  //  00F7  CALL	R22	0
      0x8C582D2E,  //  00F8  GETMET	R22	R22	K46
      0x58600041,  //  00F9  LDCONST	R24	K65
      0x7C580400,  //  00FA  CALL	R22	2
      0x605C0015,  //  00FB  GETGBL	R23	G21
      0x7C5C0000,  //  00FC  CALL	R23	0
      0x58600004,  //  00FD  LDCONST	R24	K4
      0x5466000F,  //  00FE  LDINT	R25	16
      0x7C4C0C00,  //  00FF  CALL	R19	6
      0x8C502737,  //  0100  GETMET	R20	R19	K55
      0x7C500200,  //  0101  CALL	R20	1
      0xB8561800,  //  0102  GETNGBL	R21	K12
      0x8C542B42,  //  0103  GETMET	R21	R21	K66
      0x7C540200,  //  0104  CALL	R21	1
      0x88580D3E,  //  0105  GETMBR	R22	R6	K62
      0x90562C16,  //  0106  SETMBR	R21	K22	R22
      0x88580D28,  //  0107  GETMBR	R22	R6	K40
      0x90568616,  //  0108  SETMBR	R21	K67	R22
      0x90568814,  //  0109  SETMBR	R21	K68	R20
      0x8C580530,  //  010A  GETMET	R22	R2	K48
      0x7C580200,  //  010B  CALL	R22	1
      0x8C582D31,  //  010C  GETMET	R22	R22	K49
      0x88600D32,  //  010D  GETMBR	R24	R6	K50
      0x8864091F,  //  010E  GETMBR	R25	R4	K31
      0x88680916,  //  010F  GETMBR	R26	R4	K22
      0x0064321A,  //  0110  ADD	R25	R25	R26
      0x60680015,  //  0111  GETGBL	R26	G21
      0x7C680000,  //  0112  CALL	R26	0
      0x8C68352E,  //  0113  GETMET	R26	R26	K46
      0x58700045,  //  0114  LDCONST	R28	K69
      0x7C680400,  //  0115  CALL	R26	2
      0x546E002F,  //  0116  LDINT	R27	48
      0x7C580A00,  //  0117  CALL	R22	5
      0x545E000E,  //  0118  LDINT	R23	15
      0x405E0817,  //  0119  CONNECT	R23	K4	R23
      0x945C2C17,  //  011A  GETIDX	R23	R22	R23
      0x5462000F,  //  011B  LDINT	R24	16
      0x5466001E,  //  011C  LDINT	R25	31
      0x40603019,  //  011D  CONNECT	R24	R24	R25
      0x94602C18,  //  011E  GETIDX	R24	R22	R24
      0x5466001F,  //  011F  LDINT	R25	32
      0x546A002E,  //  0120  LDINT	R26	47
      0x4064321A,  //  0121  CONNECT	R25	R25	R26
      0x94642C19,  //  0122  GETIDX	R25	R22	R25
      0xB86A0C00,  //  0123  GETNGBL	R26	K6
      0x8C683546,  //  0124  GETMET	R26	R26	K70
      0x7C680200,  //  0125  CALL	R26	1
      0x94683547,  //  0126  GETIDX	R26	R26	K71
      0xB86E0C00,  //  0127  GETNGBL	R27	K6
      0x8C6C3707,  //  0128  GETMET	R27	R27	K7
      0x58740048,  //  0129  LDCONST	R29	K72
      0x547A0003,  //  012A  LDINT	R30	4
      0x7C6C0600,  //  012B  CALL	R27	3
      0xB86E0C00,  //  012C  GETNGBL	R27	K6
      0x8C6C3707,  //  012D  GETMET	R27	R27	K7
      0x8C742F3A,  //  012E  GETMET	R29	R23	K58
      0x7C740200,  //  012F  CALL	R29	1
      0x0076921D,  //  0130  ADD	R29	K73	R29
      0x547A0003,  //  0131  LDINT	R30	4
      0x7C6C0600,  //  0132  CALL	R27	3
      0xB86E0C00,  //  0133  GETNGBL	R27	K6
      0x8C6C3707,  //  0134  GETMET	R27	R27	K7
      0x8C74313A,  //  0135  GETMET	R29	R24	K58
      0x7C740200,  //  0136  CALL	R29	1
      0x0076941D,  //  0137  ADD	R29	K74	R29
      0x547A0003,  //  0138  LDINT	R30	4
      0x7C6C0600,  //  0139  CALL	R27	3
      0xB86E0C00,  //  013A  GETNGBL	R27	K6
      0x8C6C3707,  //  013B  GETMET	R27	R27	K7
      0x8C74333A,  //  013C  GETMET	R29	R25	K58
      0x7C740200,  //  013D  CALL	R29	1
      0x0076961D,  //  013E  ADD	R29	K75	R29
      0x547A0003,  //  013F  LDINT	R30	4
      0x7C6C0600,  //  0140  CALL	R27	3
      0xB86E0C00,  //  0141  GETNGBL	R27	K6
      0x8C6C3707,  //  0142  GETMET	R27	R27	K7
      0x58740048,  //  0143  LDCONST	R29	K72
      0x547A0003,  //  0144  LDINT	R30	4
      0x7C6C0600,  //  0145  CALL	R27	3
      0x8C6C2B4C,  //  0146  GETMET	R27	R21	K76
      0x7C6C0200,  //  0147  CALL	R27	1
      0x4C700000,  //  0148  LDNIL	R28
      0x901A9A1C,  //  0149  SETMBR	R6	K77	R28
      0xB8720C00,  //  014A  GETNGBL	R28	K6
      0x8C703907,  //  014B  GETMET	R28	R28	K7
      0x8C78373A,  //  014C  GETMET	R30	R27	K58
      0x7C780200,  //  014D  CALL	R30	1
      0x007A9C1E,  //  014E  ADD	R30	K78	R30
      0x547E0003,  //  014F  LDINT	R31	4
      0x7C700600,  //  0150  CALL	R28	3
      0x8C70034F,  //  0151  GETMET	R28	R1	K79
      0x547A0032,  //  0152  LDINT	R30	51
      0x507C0200,  //  0153  LDBOOL	R31	1	0
      0x7C700600,  //  0154  CALL	R28	3
      0x8C743950,  //  0155  GETMET	R29	R28	K80
      0x5C7C3600,  //  0156  MOVE	R31	R27
      0x7C740400,  //  0157  CALL	R29	2
      0x88780151,  //  0158  GETMBR	R30	R0	K81
      0x8C783D52,  //  0159  GETMET	R30	R30	K82
      0x5C803A00,  //  015A  MOVE	R32	R29
      0x8884032C,  //  015B  GETMBR	R33	R1	K44
      0x8888032D,  //  015C  GETMBR	R34	R1	K45
      0x888C3953,  //  015D  GETMBR	R35	R28	K83
      0x88900319,  //  015E  GETMBR	R36	R1	K25
      0x88904903,  //  015F  GETMBR	R36	R36	K3
      0x7C780C00,  //  0160  CALL	R30	6
      0x8C780D54,  //  0161  GETMET	R30	R6	K84
      0x5C802E00,  //  0162  MOVE	R32	R23
      0x5C843000,  //  0163  MOVE	R33	R24
      0x5C883200,  //  0164  MOVE	R34	R25
      0x5C8C3400,  //  0165  MOVE	R35	R26
      0x7C780A00,  //  0166  CALL	R30	5
      0x50780200,  //  0167  LDBOOL	R30	1	0
      0x80043C00,  //  0168  RET	1	R30
      0x70020000,  //  0169  JMP		#016B
      0x50140000,  //  016A  LDBOOL	R5	0	0
      0x5C1C0A00,  //  016B  MOVE	R7	R5
      0x741E014A,  //  016C  JMPT	R7	#02B8
      0xB81E0C00,  //  016D  GETNGBL	R7	K6
      0x8C1C0F07,  //  016E  GETMET	R7	R7	K7
      0xB8261800,  //  016F  GETNGBL	R9	K12
      0x8C241313,  //  0170  GETMET	R9	R9	K19
      0x882C0D20,  //  0171  GETMBR	R11	R6	K32
      0x7C240400,  //  0172  CALL	R9	2
      0x0026AA09,  //  0173  ADD	R9	K85	R9
      0x542A0003,  //  0174  LDINT	R10	4
      0x7C1C0600,  //  0175  CALL	R7	3
      0xB81E0C00,  //  0176  GETNGBL	R7	K6
      0x8C1C0F07,  //  0177  GETMET	R7	R7	K7
      0x88240D20,  //  0178  GETMBR	R9	R6	K32
      0x88241357,  //  0179  GETMBR	R9	R9	K87
      0x8C24133A,  //  017A  GETMET	R9	R9	K58
      0x7C240200,  //  017B  CALL	R9	1
      0x0026AC09,  //  017C  ADD	R9	K86	R9
      0x542A0003,  //  017D  LDINT	R10	4
      0x7C1C0600,  //  017E  CALL	R7	3
      0xB81E0C00,  //  017F  GETNGBL	R7	K6
      0x8C1C0F07,  //  0180  GETMET	R7	R7	K7
      0x88240D20,  //  0181  GETMBR	R9	R6	K32
      0x88241359,  //  0182  GETMBR	R9	R9	K89
      0x8C24133A,  //  0183  GETMET	R9	R9	K58
      0x7C240200,  //  0184  CALL	R9	1
      0x0026B009,  //  0185  ADD	R9	K88	R9
      0x542A0003,  //  0186  LDINT	R10	4
      0x7C1C0600,  //  0187  CALL	R7	3
      0x881C0D20,  //  0188  GETMBR	R7	R6	K32
      0x8C1C0F5A,  //  0189  GETMET	R7	R7	K90
      0x7C1C0200,  //  018A  CALL	R7	1
      0x781E0009,  //  018B  JMPF	R7	#0196
      0xB81E0C00,  //  018C  GETNGBL	R7	K6
      0x8C1C0F07,  //  018D  GETMET	R7	R7	K7
      0x88240D20,  //  018E  GETMBR	R9	R6	K32
      0x8C24135A,  //  018F  GETMET	R9	R9	K90
      0x7C240200,  //  0190  CALL	R9	1
      0x8C24133A,  //  0191  GETMET	R9	R9	K58
      0x7C240200,  //  0192  CALL	R9	1
      0x0026B609,  //  0193  ADD	R9	K91	R9
      0x542A0003,  //  0194  LDINT	R10	4
      0x7C1C0600,  //  0195  CALL	R7	3
      0xB81E0C00,  //  0196  GETNGBL	R7	K6
      0x8C1C0F07,  //  0197  GETMET	R7	R7	K7
      0x88240D20,  //  0198  GETMBR	R9	R6	K32
      0x8824135D,  //  0199  GETMBR	R9	R9	K93
      0x8C24133A,  //  019A  GETMET	R9	R9	K58
      0x7C240200,  //  019B  CALL	R9	1
      0x0026B809,  //  019C  ADD	R9	K92	R9
      0x542A0003,  //  019D  LDINT	R10	4
      0x7C1C0600,  //  019E  CALL	R7	3
      0x8C1C053F,  //  019F  GETMET	R7	R2	K63
      0x5426000F,  //  01A0  LDINT	R9	16
      0x7C1C0400,  //  01A1  CALL	R7	2
      0x901A7C07,  //  01A2  SETMBR	R6	K62	R7
      0x8C1C053F,  //  01A3  GETMET	R7	R2	K63
      0x5426001F,  //  01A4  LDINT	R9	32
      0x7C1C0400,  //  01A5  CALL	R7	2
      0x9002BC07,  //  01A6  SETMBR	R0	K94	R7
      0x8C1C0560,  //  01A7  GETMET	R7	R2	K96
      0x7C1C0200,  //  01A8  CALL	R7	1
      0x8C1C0F61,  //  01A9  GETMET	R7	R7	K97
      0x8824015E,  //  01AA  GETMBR	R9	R0	K94
      0x7C1C0400,  //  01AB  CALL	R7	2
      0x9002BE07,  //  01AC  SETMBR	R0	K95	R7
      0xB81E0C00,  //  01AD  GETNGBL	R7	K6
      0x8C1C0F07,  //  01AE  GETMET	R7	R7	K7
      0x8824015E,  //  01AF  GETMBR	R9	R0	K94
      0x8C24133A,  //  01B0  GETMET	R9	R9	K58
      0x7C240200,  //  01B1  CALL	R9	1
      0x0026C409,  //  01B2  ADD	R9	K98	R9
      0x542A0003,  //  01B3  LDINT	R10	4
      0x7C1C0600,  //  01B4  CALL	R7	3
      0xB81E0C00,  //  01B5  GETNGBL	R7	K6
      0x8C1C0F07,  //  01B6  GETMET	R7	R7	K7
      0x8824015F,  //  01B7  GETMBR	R9	R0	K95
      0x8C24133A,  //  01B8  GETMET	R9	R9	K58
      0x7C240200,  //  01B9  CALL	R9	1
      0x0026C609,  //  01BA  ADD	R9	K99	R9
      0x542A0003,  //  01BB  LDINT	R10	4
      0x7C1C0600,  //  01BC  CALL	R7	3
      0x8C1C053F,  //  01BD  GETMET	R7	R2	K63
      0x5426001F,  //  01BE  LDINT	R9	32
      0x7C1C0400,  //  01BF  CALL	R7	2
      0x8C200560,  //  01C0  GETMET	R8	R2	K96
      0x7C200200,  //  01C1  CALL	R8	1
      0x8C201164,  //  01C2  GETMET	R8	R8	K100
      0x8828015E,  //  01C3  GETMBR	R10	R0	K94
      0x882C0915,  //  01C4  GETMBR	R11	R4	K21
      0x7C200600,  //  01C5  CALL	R8	3
      0x901A6408,  //  01C6  SETMBR	R6	K50	R8
      0xB8221800,  //  01C7  GETNGBL	R8	K12
      0x88201165,  //  01C8  GETMBR	R8	R8	K101
      0x8C201166,  //  01C9  GETMET	R8	R8	K102
      0x7C200200,  //  01CA  CALL	R8	1
      0x8C241167,  //  01CB  GETMET	R9	R8	K103
      0x582C000B,  //  01CC  LDCONST	R11	K11
      0xB8321800,  //  01CD  GETNGBL	R12	K12
      0x88301965,  //  01CE  GETMBR	R12	R12	K101
      0x88301968,  //  01CF  GETMBR	R12	R12	K104
      0x8C340D69,  //  01D0  GETMET	R13	R6	K105
      0x7C340200,  //  01D1  CALL	R13	1
      0x7C240800,  //  01D2  CALL	R9	4
      0x8C241167,  //  01D3  GETMET	R9	R8	K103
      0x582C0009,  //  01D4  LDCONST	R11	K9
      0xB8321800,  //  01D5  GETNGBL	R12	K12
      0x88301965,  //  01D6  GETMBR	R12	R12	K101
      0x88301968,  //  01D7  GETMBR	R12	R12	K104
      0x8C340D5A,  //  01D8  GETMET	R13	R6	K90
      0x7C340200,  //  01D9  CALL	R13	1
      0x7C240800,  //  01DA  CALL	R9	4
      0x8C241167,  //  01DB  GETMET	R9	R8	K103
      0x582C006A,  //  01DC  LDCONST	R11	K106
      0xB8321800,  //  01DD  GETNGBL	R12	K12
      0x88301965,  //  01DE  GETMBR	R12	R12	K101
      0x88301968,  //  01DF  GETMBR	R12	R12	K104
      0x8834015F,  //  01E0  GETMBR	R13	R0	K95
      0x7C240800,  //  01E1  CALL	R9	4
      0x8C241167,  //  01E2  GETMET	R9	R8	K103
      0x542E0003,  //  01E3  LDINT	R11	4
      0xB8321800,  //  01E4  GETNGBL	R12	K12
      0x88301965,  //  01E5  GETMBR	R12	R12	K101
      0x88301968,  //  01E6  GETMBR	R12	R12	K104
      0x88340915,  //  01E7  GETMBR	R13	R4	K21
      0x7C240800,  //  01E8  CALL	R9	4
      0x8C240560,  //  01E9  GETMET	R9	R2	K96
      0x7C240200,  //  01EA  CALL	R9	1
      0x8C24136B,  //  01EB  GETMET	R9	R9	K107
      0x8C2C0D6C,  //  01EC  GETMET	R11	R6	K108
      0x7C2C0200,  //  01ED  CALL	R11	1
      0x8C30114C,  //  01EE  GETMET	R12	R8	K76
      0x7C300200,  //  01EF  CALL	R12	1
      0x7C240600,  //  01F0  CALL	R9	3
      0xB82A1800,  //  01F1  GETNGBL	R10	K12
      0x88281565,  //  01F2  GETMBR	R10	R10	K101
      0x8C281566,  //  01F3  GETMET	R10	R10	K102
      0x7C280200,  //  01F4  CALL	R10	1
      0x8C2C1567,  //  01F5  GETMET	R11	R10	K103
      0x5834000B,  //  01F6  LDCONST	R13	K11
      0xB83A1800,  //  01F7  GETNGBL	R14	K12
      0x88381D65,  //  01F8  GETMBR	R14	R14	K101
      0x88381D68,  //  01F9  GETMBR	R14	R14	K104
      0x8C3C0D69,  //  01FA  GETMET	R15	R6	K105
      0x7C3C0200,  //  01FB  CALL	R15	1
      0x7C2C0800,  //  01FC  CALL	R11	4
      0x8C2C1567,  //  01FD  GETMET	R11	R10	K103
      0x58340009,  //  01FE  LDCONST	R13	K9
      0xB83A1800,  //  01FF  GETNGBL	R14	K12
      0x88381D65,  //  0200  GETMBR	R14	R14	K101
      0x88381D68,  //  0201  GETMBR	R14	R14	K104
      0x8C3C0D5A,  //  0202  GETMET	R15	R6	K90
      0x7C3C0200,  //  0203  CALL	R15	1
      0x7C2C0800,  //  0204  CALL	R11	4
      0x8C2C1567,  //  0205  GETMET	R11	R10	K103
      0x5834006A,  //  0206  LDCONST	R13	K106
      0xB83A1800,  //  0207  GETNGBL	R14	K12
      0x88381D65,  //  0208  GETMBR	R14	R14	K101
      0x88381D68,  //  0209  GETMBR	R14	R14	K104
      0x5C3C1200,  //  020A  MOVE	R15	R9
      0x7C2C0800,  //  020B  CALL	R11	4
      0x8C2C1567,  //  020C  GETMET	R11	R10	K103
      0x54360003,  //  020D  LDINT	R13	4
      0xB83A1800,  //  020E  GETNGBL	R14	K12
      0x88381D65,  //  020F  GETMBR	R14	R14	K101
      0x88381D68,  //  0210  GETMBR	R14	R14	K104
      0x883C0D3E,  //  0211  GETMBR	R15	R6	K62
      0x7C2C0800,  //  0212  CALL	R11	4
      0xB82E0C00,  //  0213  GETNGBL	R11	K6
      0x8C2C1707,  //  0214  GETMET	R11	R11	K7
      0x58340038,  //  0215  LDCONST	R13	K56
      0x543A0003,  //  0216  LDINT	R14	4
      0x7C2C0600,  //  0217  CALL	R11	3
      0x882C096D,  //  0218  GETMBR	R11	R4	K109
      0x901A9A0B,  //  0219  SETMBR	R6	K77	R11
      0xB82E0C00,  //  021A  GETNGBL	R11	K6
      0x8C2C1707,  //  021B  GETMET	R11	R11	K7
      0x88340D3E,  //  021C  GETMBR	R13	R6	K62
      0x8C341B3A,  //  021D  GETMET	R13	R13	K58
      0x7C340200,  //  021E  CALL	R13	1
      0x0036DC0D,  //  021F  ADD	R13	K110	R13
      0x543A0003,  //  0220  LDINT	R14	4
      0x7C2C0600,  //  0221  CALL	R11	3
      0xB82E0C00,  //  0222  GETNGBL	R11	K6
      0x8C2C1707,  //  0223  GETMET	R11	R11	K7
      0x88340D4D,  //  0224  GETMBR	R13	R6	K77
      0x8C341B3A,  //  0225  GETMET	R13	R13	K58
      0x7C340200,  //  0226  CALL	R13	1
      0x0036DE0D,  //  0227  ADD	R13	K111	R13
      0x543A0003,  //  0228  LDINT	R14	4
      0x7C2C0600,  //  0229  CALL	R11	3
      0x8C2C0570,  //  022A  GETMET	R11	R2	K112
      0x7C2C0200,  //  022B  CALL	R11	1
      0x8C2C1771,  //  022C  GETMET	R11	R11	K113
      0x88340D4D,  //  022D  GETMBR	R13	R6	K77
      0x7C2C0400,  //  022E  CALL	R11	2
      0x8C2C1772,  //  022F  GETMET	R11	R11	K114
      0x7C2C0200,  //  0230  CALL	R11	1
      0xB8320C00,  //  0231  GETNGBL	R12	K6
      0x8C301907,  //  0232  GETMET	R12	R12	K7
      0x8C38173A,  //  0233  GETMET	R14	R11	K58
      0x7C380200,  //  0234  CALL	R14	1
      0x003AE60E,  //  0235  ADD	R14	K115	R14
      0x543E0003,  //  0236  LDINT	R15	4
      0x7C300600,  //  0237  CALL	R12	3
      0x60300015,  //  0238  GETGBL	R12	G21
      0x7C300000,  //  0239  CALL	R12	0
      0x8C30192E,  //  023A  GETMET	R12	R12	K46
      0x88380174,  //  023B  GETMBR	R14	R0	K116
      0x7C300400,  //  023C  CALL	R12	2
      0x8C340D75,  //  023D  GETMET	R13	R6	K117
      0x7C340200,  //  023E  CALL	R13	1
      0x00341A07,  //  023F  ADD	R13	R13	R7
      0x8838015F,  //  0240  GETMBR	R14	R0	K95
      0x00341A0E,  //  0241  ADD	R13	R13	R14
      0x00341A0B,  //  0242  ADD	R13	R13	R11
      0x8C380530,  //  0243  GETMET	R14	R2	K48
      0x7C380200,  //  0244  CALL	R14	1
      0x8C381D31,  //  0245  GETMET	R14	R14	K49
      0x88400D32,  //  0246  GETMBR	R16	R6	K50
      0x5C441A00,  //  0247  MOVE	R17	R13
      0x5C481800,  //  0248  MOVE	R18	R12
      0x544E000F,  //  0249  LDINT	R19	16
      0x7C380A00,  //  024A  CALL	R14	5
      0xB83E0C00,  //  024B  GETNGBL	R15	K6
      0x8C3C1F07,  //  024C  GETMET	R15	R15	K7
      0x88440D32,  //  024D  GETMBR	R17	R6	K50
      0x8C44233A,  //  024E  GETMET	R17	R17	K58
      0x7C440200,  //  024F  CALL	R17	1
      0x0046EC11,  //  0250  ADD	R17	K118	R17
      0x544A0003,  //  0251  LDINT	R18	4
      0x7C3C0600,  //  0252  CALL	R15	3
      0xB83E0C00,  //  0253  GETNGBL	R15	K6
      0x8C3C1F07,  //  0254  GETMET	R15	R15	K7
      0x8C441B3A,  //  0255  GETMET	R17	R13	K58
      0x7C440200,  //  0256  CALL	R17	1
      0x0046EE11,  //  0257  ADD	R17	K119	R17
      0x544A0003,  //  0258  LDINT	R18	4
      0x7C3C0600,  //  0259  CALL	R15	3
      0xB83E0C00,  //  025A  GETNGBL	R15	K6
      0x8C3C1F07,  //  025B  GETMET	R15	R15	K7
      0x8C441D3A,  //  025C  GETMET	R17	R14	K58
      0x7C440200,  //  025D  CALL	R17	1
      0x0046F011,  //  025E  ADD	R17	K120	R17
      0x544A0003,  //  025F  LDINT	R18	4
      0x7C3C0600,  //  0260  CALL	R15	3
      0x8C3C154C,  //  0261  GETMET	R15	R10	K76
      0x7C3C0200,  //  0262  CALL	R15	1
      0xB8420C00,  //  0263  GETNGBL	R16	K6
      0x8C402107,  //  0264  GETMET	R16	R16	K7
      0x8C481F3A,  //  0265  GETMET	R18	R15	K58
      0x7C480200,  //  0266  CALL	R18	1
      0x004AF212,  //  0267  ADD	R18	K121	R18
      0x544E0003,  //  0268  LDINT	R19	4
      0x7C400600,  //  0269  CALL	R16	3
      0x8C400535,  //  026A  GETMET	R16	R2	K53
      0x5C481C00,  //  026B  MOVE	R18	R14
      0x604C0015,  //  026C  GETGBL	R19	G21
      0x7C4C0000,  //  026D  CALL	R19	0
      0x8C4C272E,  //  026E  GETMET	R19	R19	K46
      0x8854017A,  //  026F  GETMBR	R21	R0	K122
      0x7C4C0400,  //  0270  CALL	R19	2
      0x60500015,  //  0271  GETGBL	R20	G21
      0x7C500000,  //  0272  CALL	R20	0
      0x6054000C,  //  0273  GETGBL	R21	G12
      0x5C581E00,  //  0274  MOVE	R22	R15
      0x7C540200,  //  0275  CALL	R21	1
      0x545A000F,  //  0276  LDINT	R22	16
      0x7C400C00,  //  0277  CALL	R16	6
      0x8C44217B,  //  0278  GETMET	R17	R16	K123
      0x5C4C1E00,  //  0279  MOVE	R19	R15
      0x7C440400,  //  027A  CALL	R17	2
      0x8C482137,  //  027B  GETMET	R18	R16	K55
      0x7C480200,  //  027C  CALL	R18	1
      0x00442212,  //  027D  ADD	R17	R17	R18
      0xB84A0C00,  //  027E  GETNGBL	R18	K6
      0x8C482507,  //  027F  GETMET	R18	R18	K7
      0x8C50233A,  //  0280  GETMET	R20	R17	K58
      0x7C500200,  //  0281  CALL	R20	1
      0x0052F814,  //  0282  ADD	R20	K124	R20
      0x54560003,  //  0283  LDINT	R21	4
      0x7C480600,  //  0284  CALL	R18	3
      0xB84A0C00,  //  0285  GETNGBL	R18	K6
      0x8C482507,  //  0286  GETMET	R18	R18	K7
      0x58500038,  //  0287  LDCONST	R20	K56
      0x54560003,  //  0288  LDINT	R21	4
      0x7C480600,  //  0289  CALL	R18	3
      0xB84A1800,  //  028A  GETNGBL	R18	K12
      0x8C48257D,  //  028B  GETMET	R18	R18	K125
      0x7C480200,  //  028C  CALL	R18	1
      0x904AFC07,  //  028D  SETMBR	R18	K126	R7
      0x884C012A,  //  028E  GETMBR	R19	R0	K42
      0x904AFE13,  //  028F  SETMBR	R18	K127	R19
      0x884C015F,  //  0290  GETMBR	R19	R0	K95
      0x904B0013,  //  0291  SETMBR	R18	K128	R19
      0x904B0211,  //  0292  SETMBR	R18	K129	R17
      0xB84E0C00,  //  0293  GETNGBL	R19	K6
      0x8C4C2707,  //  0294  GETMET	R19	R19	K7
      0xB8561800,  //  0295  GETNGBL	R21	K12
      0x8C542B13,  //  0296  GETMET	R21	R21	K19
      0x5C5C2400,  //  0297  MOVE	R23	R18
      0x7C540400,  //  0298  CALL	R21	2
      0x00570415,  //  0299  ADD	R21	K130	R21
      0x545A0003,  //  029A  LDINT	R22	4
      0x7C4C0600,  //  029B  CALL	R19	3
      0x8C4C254C,  //  029C  GETMET	R19	R18	K76
      0x7C4C0200,  //  029D  CALL	R19	1
      0x901B0613,  //  029E  SETMBR	R6	K131	R19
      0xB8520C00,  //  029F  GETNGBL	R20	K6
      0x8C502907,  //  02A0  GETMET	R20	R20	K7
      0x8C58273A,  //  02A1  GETMET	R22	R19	K58
      0x7C580200,  //  02A2  CALL	R22	1
      0x005B0816,  //  02A3  ADD	R22	K132	R22
      0x545E0003,  //  02A4  LDINT	R23	4
      0x7C500600,  //  02A5  CALL	R20	3
      0x8C50034F,  //  02A6  GETMET	R20	R1	K79
      0x545A0030,  //  02A7  LDINT	R22	49
      0x505C0200,  //  02A8  LDBOOL	R23	1	0
      0x7C500600,  //  02A9  CALL	R20	3
      0x8C542950,  //  02AA  GETMET	R21	R20	K80
      0x5C5C2600,  //  02AB  MOVE	R23	R19
      0x7C540400,  //  02AC  CALL	R21	2
      0x88580151,  //  02AD  GETMBR	R22	R0	K81
      0x8C582D52,  //  02AE  GETMET	R22	R22	K82
      0x5C602A00,  //  02AF  MOVE	R24	R21
      0x8864032C,  //  02B0  GETMBR	R25	R1	K44
      0x8868032D,  //  02B1  GETMBR	R26	R1	K45
      0x886C2953,  //  02B2  GETMBR	R27	R20	K83
      0x88700319,  //  02B3  GETMBR	R28	R1	K25
      0x88703903,  //  02B4  GETMBR	R28	R28	K3
      0x7C580C00,  //  02B5  CALL	R22	6
      0x50580200,  //  02B6  LDBOOL	R22	1	0
      0x80042C00,  //  02B7  RET	1	R22
      0x501C0200,  //  02B8  LDBOOL	R7	1	0
      0x80040E00,  //  02B9  RET	1	R7
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
