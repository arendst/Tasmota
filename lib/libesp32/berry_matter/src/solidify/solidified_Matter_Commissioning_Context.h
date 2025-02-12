/* Solidification of Matter_Commissioning_Context.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Commisioning_Context' ktab size: 185, total: 362 (saved 1416 bytes)
static const bvalue be_ktab_class_Matter_Commisioning_Context[185] = {
  /* K0   */  be_nested_str_weak(session),
  /* K1   */  be_nested_str_weak(raw),
  /* K2   */  be_nested_str_weak(app_payload_idx),
  /* K3   */  be_const_int(2147483647),
  /* K4   */  be_nested_str_weak(tohex),
  /* K5   */  be_nested_str_weak(log),
  /* K6   */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20_X25s),
  /* K7   */  be_nested_str_weak(0000000000000000),
  /* K8   */  be_const_int(3),
  /* K9   */  be_nested_str_weak(crypto),
  /* K10  */  be_nested_str_weak(tasmota),
  /* K11  */  be_nested_str_weak(loglevel),
  /* K12  */  be_nested_str_weak(MTR_X3A_X20SEARCHING_X3A_X20destinationId_X3D),
  /* K13  */  be_nested_str_weak(device),
  /* K14  */  be_nested_str_weak(sessions),
  /* K15  */  be_nested_str_weak(fabrics),
  /* K16  */  be_nested_str_weak(noc),
  /* K17  */  be_nested_str_weak(fabric_id),
  /* K18  */  be_nested_str_weak(device_id),
  /* K19  */  be_nested_str_weak(get_ca_pub),
  /* K20  */  be_nested_str_weak(get_ipk_group_key),
  /* K21  */  be_nested_str_weak(HMAC_SHA256),
  /* K22  */  be_nested_str_weak(update),
  /* K23  */  be_nested_str_weak(out),
  /* K24  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20candidateDestinationId_X3D),
  /* K25  */  be_nested_str_weak(stop_iteration),
  /* K26  */  be_nested_str_weak(MTR_X3A_X20add_session_X20local_session_id_X3D_X25i_X20initiator_session_id_X3D_X25i),
  /* K27  */  be_nested_str_weak(create_session),
  /* K28  */  be_nested_str_weak(set_keys),
  /* K29  */  be_nested_str_weak(opcode),
  /* K30  */  be_nested_str_weak(local_session_id),
  /* K31  */  be_const_int(0),
  /* K32  */  be_nested_str_weak(protocol_id),
  /* K33  */  be_nested_str_weak(MTR_X3A_X20invalid_X20PBKDFParamRequest_X20message),
  /* K34  */  be_const_int(2),
  /* K35  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
  /* K36  */  be_nested_str_weak(send_status_report),
  /* K37  */  be_const_int(1),
  /* K38  */  be_nested_str_weak(matter),
  /* K39  */  be_nested_str_weak(PBKDFParamRequest),
  /* K40  */  be_nested_str_weak(parse),
  /* K41  */  be_nested_str_weak(set_mode_PASE),
  /* K42  */  be_nested_str_weak(__Msg1),
  /* K43  */  be_nested_str_weak(passcodeId),
  /* K44  */  be_nested_str_weak(MTR_X3A_X20non_X2Dzero_X20passcode_X20id),
  /* K45  */  be_nested_str_weak(__future_initiator_session_id),
  /* K46  */  be_nested_str_weak(initiator_session_id),
  /* K47  */  be_nested_str_weak(__future_local_session_id),
  /* K48  */  be_nested_str_weak(gen_local_session_id),
  /* K49  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
  /* K50  */  be_nested_str_weak(remote_ip),
  /* K51  */  be_nested_str_weak(remote_port),
  /* K52  */  be_nested_str_weak(PBKDFParamResponse),
  /* K53  */  be_nested_str_weak(initiatorRandom),
  /* K54  */  be_nested_str_weak(responderRandom),
  /* K55  */  be_nested_str_weak(random),
  /* K56  */  be_nested_str_weak(responderSessionId),
  /* K57  */  be_nested_str_weak(pbkdf_parameters_salt),
  /* K58  */  be_nested_str_weak(commissioning),
  /* K59  */  be_nested_str_weak(commissioning_salt),
  /* K60  */  be_nested_str_weak(pbkdf_parameters_iterations),
  /* K61  */  be_nested_str_weak(commissioning_iterations),
  /* K62  */  be_nested_str_weak(tlv2raw),
  /* K63  */  be_nested_str_weak(__Msg2),
  /* K64  */  be_nested_str_weak(build_response),
  /* K65  */  be_nested_str_weak(encode_frame),
  /* K66  */  be_nested_str_weak(responder),
  /* K67  */  be_nested_str_weak(send_response_frame),
  /* K68  */  be_nested_str_weak(add),
  /* K69  */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake1_X20message),
  /* K70  */  be_nested_str_weak(Pake1),
  /* K71  */  be_nested_str_weak(pA),
  /* K72  */  be_nested_str_weak(SPAKE2P_Matter),
  /* K73  */  be_nested_str_weak(commissioning_w0),
  /* K74  */  be_nested_str_weak(commissioning_L),
  /* K75  */  be_nested_str_weak(compute_pB),
  /* K76  */  be_nested_str_weak(compute_ZV_verifier),
  /* K77  */  be_nested_str_weak(SHA256),
  /* K78  */  be_nested_str_weak(fromstring),
  /* K79  */  be_nested_str_weak(Matter_Context_Prefix),
  /* K80  */  be_nested_str_weak(set_context),
  /* K81  */  be_nested_str_weak(compute_TT_hash),
  /* K82  */  be_nested_str_weak(Pake2),
  /* K83  */  be_nested_str_weak(pB),
  /* K84  */  be_nested_str_weak(cB),
  /* K85  */  be_nested_str_weak(__spake_cA),
  /* K86  */  be_nested_str_weak(cA),
  /* K87  */  be_nested_str_weak(__spake_Ke),
  /* K88  */  be_nested_str_weak(Ke),
  /* K89  */  be_nested_str_weak(MTR_X3A_X20New_X20Commissioning_X20_X28PASE_X20id_X3D_X25i_X29_X20from_X20_X5B_X25s_X5D_X3A_X25i),
  /* K90  */  be_nested_str_weak(_ip),
  /* K91  */  be_nested_str_weak(_port),
  /* K92  */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake3_X20message),
  /* K93  */  be_nested_str_weak(Pake3),
  /* K94  */  be_nested_str_weak(MTR_X3A_X20invalid_X20cA_X20received),
  /* K95  */  be_nested_str_weak(rtc_utc),
  /* K96  */  be_nested_str_weak(HKDF_SHA256),
  /* K97  */  be_nested_str_weak(derive),
  /* K98  */  be_nested_str_weak(SEKeys_Info),
  /* K99  */  be_nested_str_weak(add_session),
  /* K100 */  be_nested_str_weak(Sigma3),
  /* K101 */  be_nested_str_weak(S3K_Info),
  /* K102 */  be_nested_str_weak(shared_secret),
  /* K103 */  be_nested_str_weak(TBEData3Encrypted),
  /* K104 */  be_nested_str_weak(AES_CCM),
  /* K105 */  be_nested_str_weak(TBEData3_Nonce),
  /* K106 */  be_nested_str_weak(decrypt),
  /* K107 */  be_nested_str_weak(tag),
  /* K108 */  be_nested_str_weak(MTR_X3A_X20Tag_X20don_X27t_X20match),
  /* K109 */  be_nested_str_weak(TLV),
  /* K110 */  be_nested_str_weak(findsubval),
  /* K111 */  be_nested_str_weak(findsub),
  /* K112 */  be_nested_str_weak(int),
  /* K113 */  be_nested_str_weak(peer_node_id),
  /* K114 */  be_nested_str_weak(int64),
  /* K115 */  be_nested_str_weak(fromu32),
  /* K116 */  be_nested_str_weak(tobytes),
  /* K117 */  be_nested_str_weak(Matter_TLV_struct),
  /* K118 */  be_nested_str_weak(add_TLV),
  /* K119 */  be_nested_str_weak(B1),
  /* K120 */  be_nested_str_weak(__initiator_pub),
  /* K121 */  be_nested_str_weak(__responder_pub),
  /* K122 */  be_nested_str_weak(EC_P256),
  /* K123 */  be_nested_str_weak(ecdsa_verify_sha256),
  /* K124 */  be_nested_str_weak(MTR_X3A_X20sigma3_tbs_X20does_X20not_X20have_X20a_X20valid_X20signature),
  /* K125 */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X20Invalid_X20signature_X2C_X20trying_X20anyways),
  /* K126 */  be_nested_str_weak(MTR_X3A_X20Sigma3_X20verified_X2C_X20computing_X20new_X20keys),
  /* K127 */  be_nested_str_weak(Msg3),
  /* K128 */  be_nested_str_weak(close),
  /* K129 */  be_nested_str_weak(_breadcrumb),
  /* K130 */  be_nested_str_weak(counter_snd_next),
  /* K131 */  be_nested_str_weak(set_persist),
  /* K132 */  be_nested_str_weak(set_no_expiration),
  /* K133 */  be_nested_str_weak(persist_to_fabric),
  /* K134 */  be_nested_str_weak(save),
  /* K135 */  be_nested_str_weak(is_commissioning_open),
  /* K136 */  be_nested_str_weak(MTR_X3A_X20commissioning_X20not_X20open),
  /* K137 */  be_nested_str_weak(parse_PBKDFParamRequest),
  /* K138 */  be_nested_str_weak(parse_Pake1),
  /* K139 */  be_nested_str_weak(parse_Pake3),
  /* K140 */  be_nested_str_weak(parse_Sigma1),
  /* K141 */  be_nested_str_weak(parse_Sigma3),
  /* K142 */  be_nested_str_weak(parse_StatusReport),
  /* K143 */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28secure_X20channel_X29_X20_X2502X),
  /* K144 */  be_nested_str_weak(Sigma1),
  /* K145 */  be_nested_str_weak(initiatorEphPubKey),
  /* K146 */  be_nested_str_weak(resumptionID),
  /* K147 */  be_nested_str_weak(initiatorResumeMIC),
  /* K148 */  be_nested_str_weak(find_session_by_resumption_id),
  /* K149 */  be_nested_str_weak(_fabric),
  /* K150 */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Sigma1_X20Resumption_X20FAILED_X2C_X20session_X20not_X20found_X20resumption_id_X3D_X25s),
  /* K151 */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Sigma1_X20Resumption),
  /* K152 */  be_nested_str_weak(Sigma1_Resume),
  /* K153 */  be_nested_str_weak(NCASE_SigmaS1),
  /* K154 */  be_nested_str_weak(_source_node_id),
  /* K155 */  be_nested_str_weak(source_node_id),
  /* K156 */  be_nested_str_weak(set_mode_CASE),
  /* K157 */  be_nested_str_weak(resumption_id),
  /* K158 */  be_nested_str_weak(Sigma2_Resume),
  /* K159 */  be_nested_str_weak(NCASE_SigmaS2),
  /* K160 */  be_nested_str_weak(Sigma2Resume),
  /* K161 */  be_nested_str_weak(sigma2ResumeMIC),
  /* K162 */  be_nested_str_weak(responderSessionID),
  /* K163 */  be_nested_str_weak(SessionResumptionKeys),
  /* K164 */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Sigma1_X20Resumption_X20SUCCEEDED),
  /* K165 */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Sigma1_X20Resumption_X20FAILED_X2C_X20switching_X20to_X20CASE),
  /* K166 */  be_nested_str_weak(find_fabric_by_destination_id),
  /* K167 */  be_nested_str_weak(destinationId),
  /* K168 */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
  /* K169 */  be_nested_str_weak(__responder_priv),
  /* K170 */  be_nested_str_weak(public_key),
  /* K171 */  be_nested_str_weak(shared_key),
  /* K172 */  be_nested_str_weak(B2),
  /* K173 */  be_nested_str_weak(get_noc),
  /* K174 */  be_nested_str_weak(get_icac),
  /* K175 */  be_nested_str_weak(ecdsa_sign_sha256),
  /* K176 */  be_nested_str_weak(get_pk),
  /* K177 */  be_nested_str_weak(Msg1),
  /* K178 */  be_nested_str_weak(S2K_Info),
  /* K179 */  be_nested_str_weak(TBEData2_Nonce),
  /* K180 */  be_nested_str_weak(encrypt),
  /* K181 */  be_nested_str_weak(Sigma2),
  /* K182 */  be_nested_str_weak(responderEphPubKey),
  /* K183 */  be_nested_str_weak(encrypted2),
  /* K184 */  be_nested_str_weak(MTR_X3A_X20New_X20Connection_X20_X28CASE_X20id_X3D_X25i_X29_X20from_X20_X5B_X25s_X5D_X3A_X25i),
};


extern const bclass be_class_Matter_Commisioning_Context;

/********************************************************************
** Solidified function: parse_StatusReport
********************************************************************/
be_local_closure(class_Matter_Commisioning_Context_parse_StatusReport,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(parse_StatusReport),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x880C0302,  //  0001  GETMBR	R3	R1	K2
      0x400C0703,  //  0002  CONNECT	R3	R3	K3
      0x88100301,  //  0003  GETMBR	R4	R1	K1
      0x940C0803,  //  0004  GETIDX	R3	R4	R3
      0x8C0C0704,  //  0005  GETMET	R3	R3	K4
      0x7C0C0200,  //  0006  CALL	R3	1
      0xB8120A00,  //  0007  GETNGBL	R4	K5
      0x60140018,  //  0008  GETGBL	R5	G24
      0x58180006,  //  0009  LDCONST	R6	K6
      0x5C1C0600,  //  000A  MOVE	R7	R3
      0x7C140400,  //  000B  CALL	R5	2
      0x20180707,  //  000C  NE	R6	R3	K7
      0x781A0001,  //  000D  JMPF	R6	#0010
      0x58180008,  //  000E  LDCONST	R6	K8
      0x70020000,  //  000F  JMP		#0011
      0x541A0003,  //  0010  LDINT	R6	4
      0x7C100400,  //  0011  CALL	R4	2
      0x50100000,  //  0012  LDBOOL	R4	0	0
      0x80040800,  //  0013  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_fabric_by_destination_id
********************************************************************/
be_local_closure(class_Matter_Commisioning_Context_find_fabric_by_destination_id,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(find_fabric_by_destination_id),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0xA40E1200,  //  0000  IMPORT	R3	K9
      0xB8121400,  //  0001  GETNGBL	R4	K10
      0x8C10090B,  //  0002  GETMET	R4	R4	K11
      0x541A0003,  //  0003  LDINT	R6	4
      0x7C100400,  //  0004  CALL	R4	2
      0x78120005,  //  0005  JMPF	R4	#000C
      0xB8120A00,  //  0006  GETNGBL	R4	K5
      0x8C140304,  //  0007  GETMET	R5	R1	K4
      0x7C140200,  //  0008  CALL	R5	1
      0x00161805,  //  0009  ADD	R5	K12	R5
      0x541A0003,  //  000A  LDINT	R6	4
      0x7C100400,  //  000B  CALL	R4	2
      0x60100010,  //  000C  GETGBL	R4	G16
      0x8814010D,  //  000D  GETMBR	R5	R0	K13
      0x88140B0E,  //  000E  GETMBR	R5	R5	K14
      0x88140B0F,  //  000F  GETMBR	R5	R5	K15
      0x7C100200,  //  0010  CALL	R4	1
      0xA802002F,  //  0011  EXBLK	0	#0042
      0x5C140800,  //  0012  MOVE	R5	R4
      0x7C140000,  //  0013  CALL	R5	0
      0x88180B10,  //  0014  GETMBR	R6	R5	K16
      0x4C1C0000,  //  0015  LDNIL	R7
      0x1C180C07,  //  0016  EQ	R6	R6	R7
      0x741A0007,  //  0017  JMPT	R6	#0020
      0x88180B11,  //  0018  GETMBR	R6	R5	K17
      0x4C1C0000,  //  0019  LDNIL	R7
      0x1C180C07,  //  001A  EQ	R6	R6	R7
      0x741A0003,  //  001B  JMPT	R6	#0020
      0x88180B12,  //  001C  GETMBR	R6	R5	K18
      0x4C1C0000,  //  001D  LDNIL	R7
      0x1C180C07,  //  001E  EQ	R6	R6	R7
      0x781A0000,  //  001F  JMPF	R6	#0021
      0x7001FFF0,  //  0020  JMP		#0012
      0x8C180B13,  //  0021  GETMET	R6	R5	K19
      0x7C180200,  //  0022  CALL	R6	1
      0x00180406,  //  0023  ADD	R6	R2	R6
      0x881C0B11,  //  0024  GETMBR	R7	R5	K17
      0x00180C07,  //  0025  ADD	R6	R6	R7
      0x881C0B12,  //  0026  GETMBR	R7	R5	K18
      0x00180C07,  //  0027  ADD	R6	R6	R7
      0x8C1C0B14,  //  0028  GETMET	R7	R5	K20
      0x7C1C0200,  //  0029  CALL	R7	1
      0x8C200715,  //  002A  GETMET	R8	R3	K21
      0x5C280E00,  //  002B  MOVE	R10	R7
      0x7C200400,  //  002C  CALL	R8	2
      0x8C241116,  //  002D  GETMET	R9	R8	K22
      0x5C2C0C00,  //  002E  MOVE	R11	R6
      0x7C240400,  //  002F  CALL	R9	2
      0x8C241117,  //  0030  GETMET	R9	R8	K23
      0x7C240200,  //  0031  CALL	R9	1
      0xB82A1400,  //  0032  GETNGBL	R10	K10
      0x8C28150B,  //  0033  GETMET	R10	R10	K11
      0x54320003,  //  0034  LDINT	R12	4
      0x7C280400,  //  0035  CALL	R10	2
      0x782A0005,  //  0036  JMPF	R10	#003D
      0xB82A0A00,  //  0037  GETNGBL	R10	K5
      0x8C2C1304,  //  0038  GETMET	R11	R9	K4
      0x7C2C0200,  //  0039  CALL	R11	1
      0x002E300B,  //  003A  ADD	R11	K24	R11
      0x54320003,  //  003B  LDINT	R12	4
      0x7C280400,  //  003C  CALL	R10	2
      0x1C281201,  //  003D  EQ	R10	R9	R1
      0x782A0001,  //  003E  JMPF	R10	#0041
      0xA8040001,  //  003F  EXBLK	1	1
      0x80040A00,  //  0040  RET	1	R5
      0x7001FFCF,  //  0041  JMP		#0012
      0x58100019,  //  0042  LDCONST	R4	K25
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
be_local_closure(class_Matter_Commisioning_Context_every_second,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
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
be_local_closure(class_Matter_Commisioning_Context_add_session,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(add_session),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB81A0A00,  //  0000  GETNGBL	R6	K5
      0x601C0018,  //  0001  GETGBL	R7	G24
      0x5820001A,  //  0002  LDCONST	R8	K26
      0x5C240200,  //  0003  MOVE	R9	R1
      0x5C280400,  //  0004  MOVE	R10	R2
      0x7C1C0600,  //  0005  CALL	R7	3
      0x54220003,  //  0006  LDINT	R8	4
      0x7C180400,  //  0007  CALL	R6	2
      0x8818010D,  //  0008  GETMBR	R6	R0	K13
      0x88180D0E,  //  0009  GETMBR	R6	R6	K14
      0x8C180D1B,  //  000A  GETMET	R6	R6	K27
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240400,  //  000C  MOVE	R9	R2
      0x7C180600,  //  000D  CALL	R6	3
      0x8C1C0D1C,  //  000E  GETMET	R7	R6	K28
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
be_local_closure(class_Matter_Commisioning_Context_parse_PBKDFParamRequest,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
      0xA40A1200,  //  0000  IMPORT	R2	K9
      0x880C0300,  //  0001  GETMBR	R3	R1	K0
      0x8810031D,  //  0002  GETMBR	R4	R1	K29
      0x5416001F,  //  0003  LDINT	R5	32
      0x20100805,  //  0004  NE	R4	R4	R5
      0x74120005,  //  0005  JMPT	R4	#000C
      0x8810031E,  //  0006  GETMBR	R4	R1	K30
      0x2010091F,  //  0007  NE	R4	R4	K31
      0x74120002,  //  0008  JMPT	R4	#000C
      0x88100320,  //  0009  GETMBR	R4	R1	K32
      0x2010091F,  //  000A  NE	R4	R4	K31
      0x78120010,  //  000B  JMPF	R4	#001D
      0xB8120A00,  //  000C  GETNGBL	R4	K5
      0x58140021,  //  000D  LDCONST	R5	K33
      0x58180022,  //  000E  LDCONST	R6	K34
      0x7C100400,  //  000F  CALL	R4	2
      0xB8120A00,  //  0010  GETNGBL	R4	K5
      0x58140023,  //  0011  LDCONST	R5	K35
      0x58180008,  //  0012  LDCONST	R6	K8
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100124,  //  0014  GETMET	R4	R0	K36
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C0025,  //  0016  LDCONST	R7	K37
      0x5820001F,  //  0017  LDCONST	R8	K31
      0x58240022,  //  0018  LDCONST	R9	K34
      0x50280000,  //  0019  LDBOOL	R10	0	0
      0x7C100C00,  //  001A  CALL	R4	6
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x80040800,  //  001C  RET	1	R4
      0xB8124C00,  //  001D  GETNGBL	R4	K38
      0x8C100927,  //  001E  GETMET	R4	R4	K39
      0x7C100200,  //  001F  CALL	R4	1
      0x8C100928,  //  0020  GETMET	R4	R4	K40
      0x88180301,  //  0021  GETMBR	R6	R1	K1
      0x881C0302,  //  0022  GETMBR	R7	R1	K2
      0x7C100600,  //  0023  CALL	R4	3
      0x88140300,  //  0024  GETMBR	R5	R1	K0
      0x8C140B29,  //  0025  GETMET	R5	R5	K41
      0x7C140200,  //  0026  CALL	R5	1
      0x88140302,  //  0027  GETMBR	R5	R1	K2
      0x40140B03,  //  0028  CONNECT	R5	R5	K3
      0x88180301,  //  0029  GETMBR	R6	R1	K1
      0x94140C05,  //  002A  GETIDX	R5	R6	R5
      0x900E5405,  //  002B  SETMBR	R3	K42	R5
      0x8814092B,  //  002C  GETMBR	R5	R4	K43
      0x20140B1F,  //  002D  NE	R5	R5	K31
      0x78160010,  //  002E  JMPF	R5	#0040
      0xB8160A00,  //  002F  GETNGBL	R5	K5
      0x5818002C,  //  0030  LDCONST	R6	K44
      0x581C0022,  //  0031  LDCONST	R7	K34
      0x7C140400,  //  0032  CALL	R5	2
      0xB8160A00,  //  0033  GETNGBL	R5	K5
      0x58180023,  //  0034  LDCONST	R6	K35
      0x581C0008,  //  0035  LDCONST	R7	K8
      0x7C140400,  //  0036  CALL	R5	2
      0x8C140124,  //  0037  GETMET	R5	R0	K36
      0x5C1C0200,  //  0038  MOVE	R7	R1
      0x58200025,  //  0039  LDCONST	R8	K37
      0x5824001F,  //  003A  LDCONST	R9	K31
      0x58280022,  //  003B  LDCONST	R10	K34
      0x502C0000,  //  003C  LDBOOL	R11	0	0
      0x7C140C00,  //  003D  CALL	R5	6
      0x50140000,  //  003E  LDBOOL	R5	0	0
      0x80040A00,  //  003F  RET	1	R5
      0x8814092E,  //  0040  GETMBR	R5	R4	K46
      0x900E5A05,  //  0041  SETMBR	R3	K45	R5
      0x8814010D,  //  0042  GETMBR	R5	R0	K13
      0x88140B0E,  //  0043  GETMBR	R5	R5	K14
      0x8C140B30,  //  0044  GETMET	R5	R5	K48
      0x7C140200,  //  0045  CALL	R5	1
      0x900E5E05,  //  0046  SETMBR	R3	K47	R5
      0xB8160A00,  //  0047  GETNGBL	R5	K5
      0x60180018,  //  0048  GETGBL	R6	G24
      0x581C0031,  //  0049  LDCONST	R7	K49
      0x8820072F,  //  004A  GETMBR	R8	R3	K47
      0x88240332,  //  004B  GETMBR	R9	R1	K50
      0x88280333,  //  004C  GETMBR	R10	R1	K51
      0x7C180800,  //  004D  CALL	R6	4
      0x581C0008,  //  004E  LDCONST	R7	K8
      0x7C140400,  //  004F  CALL	R5	2
      0xB8164C00,  //  0050  GETNGBL	R5	K38
      0x8C140B34,  //  0051  GETMET	R5	R5	K52
      0x7C140200,  //  0052  CALL	R5	1
      0x88180935,  //  0053  GETMBR	R6	R4	K53
      0x90166A06,  //  0054  SETMBR	R5	K53	R6
      0x8C180537,  //  0055  GETMET	R6	R2	K55
      0x5422001F,  //  0056  LDINT	R8	32
      0x7C180400,  //  0057  CALL	R6	2
      0x90166C06,  //  0058  SETMBR	R5	K54	R6
      0x8818072F,  //  0059  GETMBR	R6	R3	K47
      0x90167006,  //  005A  SETMBR	R5	K56	R6
      0x8818010D,  //  005B  GETMBR	R6	R0	K13
      0x88180D3A,  //  005C  GETMBR	R6	R6	K58
      0x88180D3B,  //  005D  GETMBR	R6	R6	K59
      0x90167206,  //  005E  SETMBR	R5	K57	R6
      0x8818010D,  //  005F  GETMBR	R6	R0	K13
      0x88180D3A,  //  0060  GETMBR	R6	R6	K58
      0x88180D3D,  //  0061  GETMBR	R6	R6	K61
      0x90167806,  //  0062  SETMBR	R5	K60	R6
      0x8C180B3E,  //  0063  GETMET	R6	R5	K62
      0x7C180200,  //  0064  CALL	R6	1
      0x900E7E06,  //  0065  SETMBR	R3	K63	R6
      0x8C1C0340,  //  0066  GETMET	R7	R1	K64
      0x54260020,  //  0067  LDINT	R9	33
      0x50280200,  //  0068  LDBOOL	R10	1	0
      0x7C1C0600,  //  0069  CALL	R7	3
      0x8C200F41,  //  006A  GETMET	R8	R7	K65
      0x5C280C00,  //  006B  MOVE	R10	R6
      0x7C200400,  //  006C  CALL	R8	2
      0x88240142,  //  006D  GETMBR	R9	R0	K66
      0x8C241343,  //  006E  GETMET	R9	R9	K67
      0x5C2C0E00,  //  006F  MOVE	R11	R7
      0x7C240400,  //  0070  CALL	R9	2
      0x50240200,  //  0071  LDBOOL	R9	1	0
      0x80041200,  //  0072  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_status_report
********************************************************************/
be_local_closure(class_Matter_Commisioning_Context_send_status_report,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(send_status_report),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8C180340,  //  0000  GETMET	R6	R1	K64
      0x5422003F,  //  0001  LDINT	R8	64
      0x5C240A00,  //  0002  MOVE	R9	R5
      0x7C180600,  //  0003  CALL	R6	3
      0x601C0015,  //  0004  GETGBL	R7	G21
      0x7C1C0000,  //  0005  CALL	R7	0
      0x8C200F44,  //  0006  GETMET	R8	R7	K68
      0x5C280400,  //  0007  MOVE	R10	R2
      0x582C0022,  //  0008  LDCONST	R11	K34
      0x7C200600,  //  0009  CALL	R8	3
      0x8C200F44,  //  000A  GETMET	R8	R7	K68
      0x5C280600,  //  000B  MOVE	R10	R3
      0x542E0003,  //  000C  LDINT	R11	4
      0x7C200600,  //  000D  CALL	R8	3
      0x8C200F44,  //  000E  GETMET	R8	R7	K68
      0x5C280800,  //  000F  MOVE	R10	R4
      0x542E0003,  //  0010  LDINT	R11	4
      0x7C200600,  //  0011  CALL	R8	3
      0x8C200D41,  //  0012  GETMET	R8	R6	K65
      0x5C280E00,  //  0013  MOVE	R10	R7
      0x7C200400,  //  0014  CALL	R8	2
      0x88240142,  //  0015  GETMBR	R9	R0	K66
      0x8C241343,  //  0016  GETMET	R9	R9	K67
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
be_local_closure(class_Matter_Commisioning_Context_parse_Pake1,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[114]) {  /* code */
      0xA40A1200,  //  0000  IMPORT	R2	K9
      0x880C0300,  //  0001  GETMBR	R3	R1	K0
      0x8810031D,  //  0002  GETMBR	R4	R1	K29
      0x54160021,  //  0003  LDINT	R5	34
      0x20100805,  //  0004  NE	R4	R4	R5
      0x74120005,  //  0005  JMPT	R4	#000C
      0x8810031E,  //  0006  GETMBR	R4	R1	K30
      0x2010091F,  //  0007  NE	R4	R4	K31
      0x74120002,  //  0008  JMPT	R4	#000C
      0x88100320,  //  0009  GETMBR	R4	R1	K32
      0x2010091F,  //  000A  NE	R4	R4	K31
      0x78120010,  //  000B  JMPF	R4	#001D
      0xB8120A00,  //  000C  GETNGBL	R4	K5
      0x58140045,  //  000D  LDCONST	R5	K69
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0xB8120A00,  //  0010  GETNGBL	R4	K5
      0x58140023,  //  0011  LDCONST	R5	K35
      0x58180008,  //  0012  LDCONST	R6	K8
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100124,  //  0014  GETMET	R4	R0	K36
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C0025,  //  0016  LDCONST	R7	K37
      0x5820001F,  //  0017  LDCONST	R8	K31
      0x58240022,  //  0018  LDCONST	R9	K34
      0x50280000,  //  0019  LDBOOL	R10	0	0
      0x7C100C00,  //  001A  CALL	R4	6
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x80040800,  //  001C  RET	1	R4
      0xB8124C00,  //  001D  GETNGBL	R4	K38
      0x8C100946,  //  001E  GETMET	R4	R4	K70
      0x7C100200,  //  001F  CALL	R4	1
      0x8C100928,  //  0020  GETMET	R4	R4	K40
      0x88180301,  //  0021  GETMBR	R6	R1	K1
      0x881C0302,  //  0022  GETMBR	R7	R1	K2
      0x7C100600,  //  0023  CALL	R4	3
      0x88140947,  //  0024  GETMBR	R5	R4	K71
      0x8C180548,  //  0025  GETMET	R6	R2	K72
      0x8820010D,  //  0026  GETMBR	R8	R0	K13
      0x8820113A,  //  0027  GETMBR	R8	R8	K58
      0x88201149,  //  0028  GETMBR	R8	R8	K73
      0x4C240000,  //  0029  LDNIL	R9
      0x8828010D,  //  002A  GETMBR	R10	R0	K13
      0x8828153A,  //  002B  GETMBR	R10	R10	K58
      0x8828154A,  //  002C  GETMBR	R10	R10	K74
      0x7C180800,  //  002D  CALL	R6	4
      0x8C1C0537,  //  002E  GETMET	R7	R2	K55
      0x5426001F,  //  002F  LDINT	R9	32
      0x7C1C0400,  //  0030  CALL	R7	2
      0x8C200D4B,  //  0031  GETMET	R8	R6	K75
      0x5C280E00,  //  0032  MOVE	R10	R7
      0x7C200400,  //  0033  CALL	R8	2
      0x8C200D4C,  //  0034  GETMET	R8	R6	K76
      0x5C280A00,  //  0035  MOVE	R10	R5
      0x7C200400,  //  0036  CALL	R8	2
      0x8C20054D,  //  0037  GETMET	R8	R2	K77
      0x7C200200,  //  0038  CALL	R8	1
      0x8C241116,  //  0039  GETMET	R9	R8	K22
      0x602C0015,  //  003A  GETGBL	R11	G21
      0x7C2C0000,  //  003B  CALL	R11	0
      0x8C2C174E,  //  003C  GETMET	R11	R11	K78
      0x8834014F,  //  003D  GETMBR	R13	R0	K79
      0x7C2C0400,  //  003E  CALL	R11	2
      0x7C240400,  //  003F  CALL	R9	2
      0x8C241116,  //  0040  GETMET	R9	R8	K22
      0x882C072A,  //  0041  GETMBR	R11	R3	K42
      0x7C240400,  //  0042  CALL	R9	2
      0x8C241116,  //  0043  GETMET	R9	R8	K22
      0x882C073F,  //  0044  GETMBR	R11	R3	K63
      0x7C240400,  //  0045  CALL	R9	2
      0x8C241117,  //  0046  GETMET	R9	R8	K23
      0x7C240200,  //  0047  CALL	R9	1
      0x901A8E05,  //  0048  SETMBR	R6	K71	R5
      0x8C280D50,  //  0049  GETMET	R10	R6	K80
      0x5C301200,  //  004A  MOVE	R12	R9
      0x7C280400,  //  004B  CALL	R10	2
      0x8C280D51,  //  004C  GETMET	R10	R6	K81
      0x50300200,  //  004D  LDBOOL	R12	1	0
      0x7C280400,  //  004E  CALL	R10	2
      0xB82A4C00,  //  004F  GETNGBL	R10	K38
      0x8C281552,  //  0050  GETMET	R10	R10	K82
      0x7C280200,  //  0051  CALL	R10	1
      0x882C0D53,  //  0052  GETMBR	R11	R6	K83
      0x902AA60B,  //  0053  SETMBR	R10	K83	R11
      0x882C0D54,  //  0054  GETMBR	R11	R6	K84
      0x902AA80B,  //  0055  SETMBR	R10	K84	R11
      0x8C2C153E,  //  0056  GETMET	R11	R10	K62
      0x7C2C0200,  //  0057  CALL	R11	1
      0x88300D56,  //  0058  GETMBR	R12	R6	K86
      0x900EAA0C,  //  0059  SETMBR	R3	K85	R12
      0x88300D58,  //  005A  GETMBR	R12	R6	K88
      0x900EAE0C,  //  005B  SETMBR	R3	K87	R12
      0x8C300340,  //  005C  GETMET	R12	R1	K64
      0x543A0022,  //  005D  LDINT	R14	35
      0x503C0200,  //  005E  LDBOOL	R15	1	0
      0x7C300600,  //  005F  CALL	R12	3
      0x8C341941,  //  0060  GETMET	R13	R12	K65
      0x5C3C1600,  //  0061  MOVE	R15	R11
      0x7C340400,  //  0062  CALL	R13	2
      0xB83A0A00,  //  0063  GETNGBL	R14	K5
      0x603C0018,  //  0064  GETGBL	R15	G24
      0x58400059,  //  0065  LDCONST	R16	K89
      0x8844072F,  //  0066  GETMBR	R17	R3	K47
      0x8848075A,  //  0067  GETMBR	R18	R3	K90
      0x884C075B,  //  0068  GETMBR	R19	R3	K91
      0x7C3C0800,  //  0069  CALL	R15	4
      0x58400022,  //  006A  LDCONST	R16	K34
      0x7C380400,  //  006B  CALL	R14	2
      0x88380142,  //  006C  GETMBR	R14	R0	K66
      0x8C381D43,  //  006D  GETMET	R14	R14	K67
      0x5C401800,  //  006E  MOVE	R16	R12
      0x7C380400,  //  006F  CALL	R14	2
      0x50380200,  //  0070  LDBOOL	R14	1	0
      0x80041C00,  //  0071  RET	1	R14
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Commisioning_Context_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA40A1200,  //  0000  IMPORT	R2	K9
      0x90028401,  //  0001  SETMBR	R0	K66	R1
      0x880C030D,  //  0002  GETMBR	R3	R1	K13
      0x90021A03,  //  0003  SETMBR	R0	K13	R3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Pake3
********************************************************************/
be_local_closure(class_Matter_Commisioning_Context_parse_Pake3,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(parse_Pake3),
    &be_const_str_solidified,
    ( &(const binstruction[101]) {  /* code */
      0xA40A1200,  //  0000  IMPORT	R2	K9
      0x880C0300,  //  0001  GETMBR	R3	R1	K0
      0x8810031D,  //  0002  GETMBR	R4	R1	K29
      0x54160023,  //  0003  LDINT	R5	36
      0x20100805,  //  0004  NE	R4	R4	R5
      0x74120005,  //  0005  JMPT	R4	#000C
      0x8810031E,  //  0006  GETMBR	R4	R1	K30
      0x2010091F,  //  0007  NE	R4	R4	K31
      0x74120002,  //  0008  JMPT	R4	#000C
      0x88100320,  //  0009  GETMBR	R4	R1	K32
      0x2010091F,  //  000A  NE	R4	R4	K31
      0x78120010,  //  000B  JMPF	R4	#001D
      0xB8120A00,  //  000C  GETNGBL	R4	K5
      0x5814005C,  //  000D  LDCONST	R5	K92
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0xB8120A00,  //  0010  GETNGBL	R4	K5
      0x58140023,  //  0011  LDCONST	R5	K35
      0x58180008,  //  0012  LDCONST	R6	K8
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100124,  //  0014  GETMET	R4	R0	K36
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C0025,  //  0016  LDCONST	R7	K37
      0x5820001F,  //  0017  LDCONST	R8	K31
      0x58240022,  //  0018  LDCONST	R9	K34
      0x50280000,  //  0019  LDBOOL	R10	0	0
      0x7C100C00,  //  001A  CALL	R4	6
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x80040800,  //  001C  RET	1	R4
      0xB8124C00,  //  001D  GETNGBL	R4	K38
      0x8C10095D,  //  001E  GETMET	R4	R4	K93
      0x7C100200,  //  001F  CALL	R4	1
      0x8C100928,  //  0020  GETMET	R4	R4	K40
      0x88180301,  //  0021  GETMBR	R6	R1	K1
      0x881C0302,  //  0022  GETMBR	R7	R1	K2
      0x7C100600,  //  0023  CALL	R4	3
      0x88140956,  //  0024  GETMBR	R5	R4	K86
      0x88180755,  //  0025  GETMBR	R6	R3	K85
      0x20180A06,  //  0026  NE	R6	R5	R6
      0x781A0010,  //  0027  JMPF	R6	#0039
      0xB81A0A00,  //  0028  GETNGBL	R6	K5
      0x581C005E,  //  0029  LDCONST	R7	K94
      0x58200008,  //  002A  LDCONST	R8	K8
      0x7C180400,  //  002B  CALL	R6	2
      0xB81A0A00,  //  002C  GETNGBL	R6	K5
      0x581C0023,  //  002D  LDCONST	R7	K35
      0x58200008,  //  002E  LDCONST	R8	K8
      0x7C180400,  //  002F  CALL	R6	2
      0x8C180124,  //  0030  GETMET	R6	R0	K36
      0x5C200200,  //  0031  MOVE	R8	R1
      0x58240025,  //  0032  LDCONST	R9	K37
      0x5828001F,  //  0033  LDCONST	R10	K31
      0x582C0022,  //  0034  LDCONST	R11	K34
      0x50300000,  //  0035  LDBOOL	R12	0	0
      0x7C180C00,  //  0036  CALL	R6	6
      0x50180000,  //  0037  LDBOOL	R6	0	0
      0x80040C00,  //  0038  RET	1	R6
      0xB81A1400,  //  0039  GETNGBL	R6	K10
      0x8C180D5F,  //  003A  GETMET	R6	R6	K95
      0x7C180200,  //  003B  CALL	R6	1
      0x8C1C0560,  //  003C  GETMET	R7	R2	K96
      0x7C1C0200,  //  003D  CALL	R7	1
      0x8C1C0F61,  //  003E  GETMET	R7	R7	K97
      0x88240757,  //  003F  GETMBR	R9	R3	K87
      0x60280015,  //  0040  GETGBL	R10	G21
      0x7C280000,  //  0041  CALL	R10	0
      0x602C0015,  //  0042  GETGBL	R11	G21
      0x7C2C0000,  //  0043  CALL	R11	0
      0x8C2C174E,  //  0044  GETMET	R11	R11	K78
      0x88340162,  //  0045  GETMBR	R13	R0	K98
      0x7C2C0400,  //  0046  CALL	R11	2
      0x5432002F,  //  0047  LDINT	R12	48
      0x7C1C0A00,  //  0048  CALL	R7	5
      0x5422000E,  //  0049  LDINT	R8	15
      0x40223E08,  //  004A  CONNECT	R8	K31	R8
      0x94200E08,  //  004B  GETIDX	R8	R7	R8
      0x5426000F,  //  004C  LDINT	R9	16
      0x542A001E,  //  004D  LDINT	R10	31
      0x4024120A,  //  004E  CONNECT	R9	R9	R10
      0x94240E09,  //  004F  GETIDX	R9	R7	R9
      0x542A001F,  //  0050  LDINT	R10	32
      0x542E002E,  //  0051  LDINT	R11	47
      0x4028140B,  //  0052  CONNECT	R10	R10	R11
      0x94280E0A,  //  0053  GETIDX	R10	R7	R10
      0x8C2C0124,  //  0054  GETMET	R11	R0	K36
      0x5C340200,  //  0055  MOVE	R13	R1
      0x5838001F,  //  0056  LDCONST	R14	K31
      0x583C001F,  //  0057  LDCONST	R15	K31
      0x5840001F,  //  0058  LDCONST	R16	K31
      0x50440000,  //  0059  LDBOOL	R17	0	0
      0x7C2C0C00,  //  005A  CALL	R11	6
      0x8C2C0163,  //  005B  GETMET	R11	R0	K99
      0x8834072F,  //  005C  GETMBR	R13	R3	K47
      0x8838072D,  //  005D  GETMBR	R14	R3	K45
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
be_local_closure(class_Matter_Commisioning_Context_parse_Sigma3,   /* name */
  be_nested_proto(
    36,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[278]) {  /* code */
      0xA40A1200,  //  0000  IMPORT	R2	K9
      0x880C031D,  //  0001  GETMBR	R3	R1	K29
      0x54120031,  //  0002  LDINT	R4	50
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x740E0005,  //  0004  JMPT	R3	#000B
      0x880C031E,  //  0005  GETMBR	R3	R1	K30
      0x200C071F,  //  0006  NE	R3	R3	K31
      0x740E0002,  //  0007  JMPT	R3	#000B
      0x880C0320,  //  0008  GETMBR	R3	R1	K32
      0x200C071F,  //  0009  NE	R3	R3	K31
      0x780E000C,  //  000A  JMPF	R3	#0018
      0xB80E0A00,  //  000B  GETNGBL	R3	K5
      0x58100023,  //  000C  LDCONST	R4	K35
      0x58140008,  //  000D  LDCONST	R5	K8
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C0C0124,  //  000F  GETMET	R3	R0	K36
      0x5C140200,  //  0010  MOVE	R5	R1
      0x58180025,  //  0011  LDCONST	R6	K37
      0x581C001F,  //  0012  LDCONST	R7	K31
      0x58200022,  //  0013  LDCONST	R8	K34
      0x50240000,  //  0014  LDBOOL	R9	0	0
      0x7C0C0C00,  //  0015  CALL	R3	6
      0x500C0000,  //  0016  LDBOOL	R3	0	0
      0x80040600,  //  0017  RET	1	R3
      0x880C0300,  //  0018  GETMBR	R3	R1	K0
      0xB8124C00,  //  0019  GETNGBL	R4	K38
      0x8C100964,  //  001A  GETMET	R4	R4	K100
      0x7C100200,  //  001B  CALL	R4	1
      0x8C100928,  //  001C  GETMET	R4	R4	K40
      0x88180301,  //  001D  GETMBR	R6	R1	K1
      0x881C0302,  //  001E  GETMBR	R7	R1	K2
      0x7C100600,  //  001F  CALL	R4	3
      0x8C14054D,  //  0020  GETMET	R5	R2	K77
      0x7C140200,  //  0021  CALL	R5	1
      0x8C140B16,  //  0022  GETMET	R5	R5	K22
      0x881C072A,  //  0023  GETMBR	R7	R3	K42
      0x7C140400,  //  0024  CALL	R5	2
      0x8C140B16,  //  0025  GETMET	R5	R5	K22
      0x881C073F,  //  0026  GETMBR	R7	R3	K63
      0x7C140400,  //  0027  CALL	R5	2
      0x8C140B17,  //  0028  GETMET	R5	R5	K23
      0x7C140200,  //  0029  CALL	R5	1
      0x60180015,  //  002A  GETGBL	R6	G21
      0x7C180000,  //  002B  CALL	R6	0
      0x8C180D4E,  //  002C  GETMET	R6	R6	K78
      0x88200165,  //  002D  GETMBR	R8	R0	K101
      0x7C180400,  //  002E  CALL	R6	2
      0x8C1C0560,  //  002F  GETMET	R7	R2	K96
      0x7C1C0200,  //  0030  CALL	R7	1
      0x8C1C0F61,  //  0031  GETMET	R7	R7	K97
      0x88240766,  //  0032  GETMBR	R9	R3	K102
      0x8C280714,  //  0033  GETMET	R10	R3	K20
      0x7C280200,  //  0034  CALL	R10	1
      0x00281405,  //  0035  ADD	R10	R10	R5
      0x5C2C0C00,  //  0036  MOVE	R11	R6
      0x5432000F,  //  0037  LDINT	R12	16
      0x7C1C0A00,  //  0038  CALL	R7	5
      0x5421FFEE,  //  0039  LDINT	R8	-17
      0x40223E08,  //  003A  CONNECT	R8	K31	R8
      0x88240967,  //  003B  GETMBR	R9	R4	K103
      0x94201208,  //  003C  GETIDX	R8	R9	R8
      0x5425FFEF,  //  003D  LDINT	R9	-16
      0x40241303,  //  003E  CONNECT	R9	R9	K3
      0x88280967,  //  003F  GETMBR	R10	R4	K103
      0x94241409,  //  0040  GETIDX	R9	R10	R9
      0x8C280568,  //  0041  GETMET	R10	R2	K104
      0x5C300E00,  //  0042  MOVE	R12	R7
      0x60340015,  //  0043  GETGBL	R13	G21
      0x7C340000,  //  0044  CALL	R13	0
      0x8C341B4E,  //  0045  GETMET	R13	R13	K78
      0x883C0169,  //  0046  GETMBR	R15	R0	K105
      0x7C340400,  //  0047  CALL	R13	2
      0x60380015,  //  0048  GETGBL	R14	G21
      0x7C380000,  //  0049  CALL	R14	0
      0x603C000C,  //  004A  GETGBL	R15	G12
      0x5C401000,  //  004B  MOVE	R16	R8
      0x7C3C0200,  //  004C  CALL	R15	1
      0x5442000F,  //  004D  LDINT	R16	16
      0x7C280C00,  //  004E  CALL	R10	6
      0x8C2C156A,  //  004F  GETMET	R11	R10	K106
      0x5C341000,  //  0050  MOVE	R13	R8
      0x7C2C0400,  //  0051  CALL	R11	2
      0x8C30156B,  //  0052  GETMET	R12	R10	K107
      0x7C300200,  //  0053  CALL	R12	1
      0x20341809,  //  0054  NE	R13	R12	R9
      0x78360010,  //  0055  JMPF	R13	#0067
      0xB8360A00,  //  0056  GETNGBL	R13	K5
      0x5838006C,  //  0057  LDCONST	R14	K108
      0x583C0008,  //  0058  LDCONST	R15	K8
      0x7C340400,  //  0059  CALL	R13	2
      0xB8360A00,  //  005A  GETNGBL	R13	K5
      0x58380023,  //  005B  LDCONST	R14	K35
      0x583C0008,  //  005C  LDCONST	R15	K8
      0x7C340400,  //  005D  CALL	R13	2
      0x8C340124,  //  005E  GETMET	R13	R0	K36
      0x5C3C0200,  //  005F  MOVE	R15	R1
      0x58400025,  //  0060  LDCONST	R16	K37
      0x5844001F,  //  0061  LDCONST	R17	K31
      0x58480022,  //  0062  LDCONST	R18	K34
      0x504C0000,  //  0063  LDBOOL	R19	0	0
      0x7C340C00,  //  0064  CALL	R13	6
      0x50340000,  //  0065  LDBOOL	R13	0	0
      0x80041A00,  //  0066  RET	1	R13
      0xB8364C00,  //  0067  GETNGBL	R13	K38
      0x88341B6D,  //  0068  GETMBR	R13	R13	K109
      0x8C341B28,  //  0069  GETMET	R13	R13	K40
      0x5C3C1600,  //  006A  MOVE	R15	R11
      0x7C340400,  //  006B  CALL	R13	2
      0x8C381B6E,  //  006C  GETMET	R14	R13	K110
      0x58400025,  //  006D  LDCONST	R16	K37
      0x7C380400,  //  006E  CALL	R14	2
      0x8C3C1B6E,  //  006F  GETMET	R15	R13	K110
      0x58440022,  //  0070  LDCONST	R17	K34
      0x7C3C0400,  //  0071  CALL	R15	2
      0x8C401B6E,  //  0072  GETMET	R16	R13	K110
      0x58480008,  //  0073  LDCONST	R18	K8
      0x7C400400,  //  0074  CALL	R16	2
      0xB8464C00,  //  0075  GETNGBL	R17	K38
      0x8844236D,  //  0076  GETMBR	R17	R17	K109
      0x8C442328,  //  0077  GETMET	R17	R17	K40
      0x5C4C1C00,  //  0078  MOVE	R19	R14
      0x7C440400,  //  0079  CALL	R17	2
      0x8C48236E,  //  007A  GETMET	R18	R17	K110
      0x54520008,  //  007B  LDINT	R20	9
      0x7C480400,  //  007C  CALL	R18	2
      0x8C4C236F,  //  007D  GETMET	R19	R17	K111
      0x54560005,  //  007E  LDINT	R21	6
      0x7C4C0400,  //  007F  CALL	R19	2
      0x8C50276E,  //  0080  GETMET	R20	R19	K110
      0x545A0010,  //  0081  LDINT	R22	17
      0x7C500400,  //  0082  CALL	R20	2
      0x60540004,  //  0083  GETGBL	R21	G4
      0x5C582800,  //  0084  MOVE	R22	R20
      0x7C540200,  //  0085  CALL	R21	1
      0x1C542B70,  //  0086  EQ	R21	R21	K112
      0x78560007,  //  0087  JMPF	R21	#0090
      0xB856E400,  //  0088  GETNGBL	R21	K114
      0x8C542B73,  //  0089  GETMET	R21	R21	K115
      0x5C5C2800,  //  008A  MOVE	R23	R20
      0x7C540400,  //  008B  CALL	R21	2
      0x8C542B74,  //  008C  GETMET	R21	R21	K116
      0x7C540200,  //  008D  CALL	R21	1
      0x900EE215,  //  008E  SETMBR	R3	K113	R21
      0x70020002,  //  008F  JMP		#0093
      0x8C542974,  //  0090  GETMET	R21	R20	K116
      0x7C540200,  //  0091  CALL	R21	1
      0x900EE215,  //  0092  SETMBR	R3	K113	R21
      0xB8564C00,  //  0093  GETNGBL	R21	K38
      0x88542B6D,  //  0094  GETMBR	R21	R21	K109
      0x8C542B75,  //  0095  GETMET	R21	R21	K117
      0x7C540200,  //  0096  CALL	R21	1
      0x8C582B76,  //  0097  GETMET	R22	R21	K118
      0x58600025,  //  0098  LDCONST	R24	K37
      0xB8664C00,  //  0099  GETNGBL	R25	K38
      0x8864336D,  //  009A  GETMBR	R25	R25	K109
      0x88643377,  //  009B  GETMBR	R25	R25	K119
      0x5C681C00,  //  009C  MOVE	R26	R14
      0x7C580800,  //  009D  CALL	R22	4
      0x8C582B76,  //  009E  GETMET	R22	R21	K118
      0x58600022,  //  009F  LDCONST	R24	K34
      0xB8664C00,  //  00A0  GETNGBL	R25	K38
      0x8864336D,  //  00A1  GETMBR	R25	R25	K109
      0x88643377,  //  00A2  GETMBR	R25	R25	K119
      0x5C681E00,  //  00A3  MOVE	R26	R15
      0x7C580800,  //  00A4  CALL	R22	4
      0x8C582B76,  //  00A5  GETMET	R22	R21	K118
      0x58600008,  //  00A6  LDCONST	R24	K8
      0xB8664C00,  //  00A7  GETNGBL	R25	K38
      0x8864336D,  //  00A8  GETMBR	R25	R25	K109
      0x88643377,  //  00A9  GETMBR	R25	R25	K119
      0x88680778,  //  00AA  GETMBR	R26	R3	K120
      0x7C580800,  //  00AB  CALL	R22	4
      0x8C582B76,  //  00AC  GETMET	R22	R21	K118
      0x54620003,  //  00AD  LDINT	R24	4
      0xB8664C00,  //  00AE  GETNGBL	R25	K38
      0x8864336D,  //  00AF  GETMBR	R25	R25	K109
      0x88643377,  //  00B0  GETMBR	R25	R25	K119
      0x88680779,  //  00B1  GETMBR	R26	R3	K121
      0x7C580800,  //  00B2  CALL	R22	4
      0x8C582B3E,  //  00B3  GETMET	R22	R21	K62
      0x7C580200,  //  00B4  CALL	R22	1
      0x8C5C057A,  //  00B5  GETMET	R23	R2	K122
      0x7C5C0200,  //  00B6  CALL	R23	1
      0x8C5C2F7B,  //  00B7  GETMET	R23	R23	K123
      0x5C642400,  //  00B8  MOVE	R25	R18
      0x5C682C00,  //  00B9  MOVE	R26	R22
      0x5C6C2000,  //  00BA  MOVE	R27	R16
      0x7C5C0800,  //  00BB  CALL	R23	4
      0x5C602E00,  //  00BC  MOVE	R24	R23
      0x74620008,  //  00BD  JMPT	R24	#00C7
      0xB8620A00,  //  00BE  GETNGBL	R24	K5
      0x5864007C,  //  00BF  LDCONST	R25	K124
      0x58680022,  //  00C0  LDCONST	R26	K34
      0x7C600400,  //  00C1  CALL	R24	2
      0xB8620A00,  //  00C2  GETNGBL	R24	K5
      0x5864007D,  //  00C3  LDCONST	R25	K125
      0x58680022,  //  00C4  LDCONST	R26	K34
      0x7C600400,  //  00C5  CALL	R24	2
      0x70020003,  //  00C6  JMP		#00CB
      0xB8620A00,  //  00C7  GETNGBL	R24	K5
      0x5864007E,  //  00C8  LDCONST	R25	K126
      0x58680008,  //  00C9  LDCONST	R26	K8
      0x7C600400,  //  00CA  CALL	R24	2
      0x8C60054D,  //  00CB  GETMET	R24	R2	K77
      0x7C600200,  //  00CC  CALL	R24	1
      0x8C603116,  //  00CD  GETMET	R24	R24	K22
      0x8868072A,  //  00CE  GETMBR	R26	R3	K42
      0x7C600400,  //  00CF  CALL	R24	2
      0x8C603116,  //  00D0  GETMET	R24	R24	K22
      0x8868073F,  //  00D1  GETMBR	R26	R3	K63
      0x7C600400,  //  00D2  CALL	R24	2
      0x8C603116,  //  00D3  GETMET	R24	R24	K22
      0x8868097F,  //  00D4  GETMBR	R26	R4	K127
      0x7C600400,  //  00D5  CALL	R24	2
      0x8C603117,  //  00D6  GETMET	R24	R24	K23
      0x7C600200,  //  00D7  CALL	R24	1
      0x5C143000,  //  00D8  MOVE	R5	R24
      0x4C600000,  //  00D9  LDNIL	R24
      0x900E5418,  //  00DA  SETMBR	R3	K42	R24
      0x4C600000,  //  00DB  LDNIL	R24
      0x900E7E18,  //  00DC  SETMBR	R3	K63	R24
      0x8C600560,  //  00DD  GETMET	R24	R2	K96
      0x7C600200,  //  00DE  CALL	R24	1
      0x8C603161,  //  00DF  GETMET	R24	R24	K97
      0x88680766,  //  00E0  GETMBR	R26	R3	K102
      0x8C6C0714,  //  00E1  GETMET	R27	R3	K20
      0x7C6C0200,  //  00E2  CALL	R27	1
      0x006C3605,  //  00E3  ADD	R27	R27	R5
      0x60700015,  //  00E4  GETGBL	R28	G21
      0x7C700000,  //  00E5  CALL	R28	0
      0x8C70394E,  //  00E6  GETMET	R28	R28	K78
      0x88780162,  //  00E7  GETMBR	R30	R0	K98
      0x7C700400,  //  00E8  CALL	R28	2
      0x5476002F,  //  00E9  LDINT	R29	48
      0x7C600A00,  //  00EA  CALL	R24	5
      0x5466000E,  //  00EB  LDINT	R25	15
      0x40663E19,  //  00EC  CONNECT	R25	K31	R25
      0x94643019,  //  00ED  GETIDX	R25	R24	R25
      0x546A000F,  //  00EE  LDINT	R26	16
      0x546E001E,  //  00EF  LDINT	R27	31
      0x4068341B,  //  00F0  CONNECT	R26	R26	R27
      0x9468301A,  //  00F1  GETIDX	R26	R24	R26
      0x546E001F,  //  00F2  LDINT	R27	32
      0x5472002E,  //  00F3  LDINT	R28	47
      0x406C361C,  //  00F4  CONNECT	R27	R27	R28
      0x946C301B,  //  00F5  GETIDX	R27	R24	R27
      0xB8721400,  //  00F6  GETNGBL	R28	K10
      0x8C70395F,  //  00F7  GETMET	R28	R28	K95
      0x7C700200,  //  00F8  CALL	R28	1
      0x8C740124,  //  00F9  GETMET	R29	R0	K36
      0x5C7C0200,  //  00FA  MOVE	R31	R1
      0x5880001F,  //  00FB  LDCONST	R32	K31
      0x5884001F,  //  00FC  LDCONST	R33	K31
      0x5888001F,  //  00FD  LDCONST	R34	K31
      0x508C0200,  //  00FE  LDBOOL	R35	1	0
      0x7C740C00,  //  00FF  CALL	R29	6
      0x8C740780,  //  0100  GETMET	R29	R3	K128
      0x7C740200,  //  0101  CALL	R29	1
      0x8C74071C,  //  0102  GETMET	R29	R3	K28
      0x5C7C3200,  //  0103  MOVE	R31	R25
      0x5C803400,  //  0104  MOVE	R32	R26
      0x5C843600,  //  0105  MOVE	R33	R27
      0x5C883800,  //  0106  MOVE	R34	R28
      0x7C740A00,  //  0107  CALL	R29	5
      0x900F031F,  //  0108  SETMBR	R3	K129	K31
      0x8C740782,  //  0109  GETMET	R29	R3	K130
      0x7C740200,  //  010A  CALL	R29	1
      0x8C740783,  //  010B  GETMET	R29	R3	K131
      0x507C0200,  //  010C  LDBOOL	R31	1	0
      0x7C740400,  //  010D  CALL	R29	2
      0x8C740784,  //  010E  GETMET	R29	R3	K132
      0x7C740200,  //  010F  CALL	R29	1
      0x8C740785,  //  0110  GETMET	R29	R3	K133
      0x7C740200,  //  0111  CALL	R29	1
      0x8C740786,  //  0112  GETMET	R29	R3	K134
      0x7C740200,  //  0113  CALL	R29	1
      0x50740200,  //  0114  LDBOOL	R29	1	0
      0x80043A00,  //  0115  RET	1	R29
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming
********************************************************************/
be_local_closure(class_Matter_Commisioning_Context_process_incoming,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[89]) {  /* code */
      0x8808010D,  //  0000  GETMBR	R2	R0	K13
      0x8808053A,  //  0001  GETMBR	R2	R2	K58
      0x8C080587,  //  0002  GETMET	R2	R2	K135
      0x7C080200,  //  0003  CALL	R2	1
      0x740A000D,  //  0004  JMPT	R2	#0013
      0x8808031D,  //  0005  GETMBR	R2	R1	K29
      0x540E001F,  //  0006  LDINT	R3	32
      0x28080403,  //  0007  GE	R2	R2	R3
      0x780A0009,  //  0008  JMPF	R2	#0013
      0x8808031D,  //  0009  GETMBR	R2	R1	K29
      0x540E0023,  //  000A  LDINT	R3	36
      0x18080403,  //  000B  LE	R2	R2	R3
      0x780A0005,  //  000C  JMPF	R2	#0013
      0xB80A0A00,  //  000D  GETNGBL	R2	K5
      0x580C0088,  //  000E  LDCONST	R3	K136
      0x58100022,  //  000F  LDCONST	R4	K34
      0x7C080400,  //  0010  CALL	R2	2
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x80040400,  //  0012  RET	1	R2
      0x8808031D,  //  0013  GETMBR	R2	R1	K29
      0x540E000F,  //  0014  LDINT	R3	16
      0x1C080403,  //  0015  EQ	R2	R2	R3
      0x780A0000,  //  0016  JMPF	R2	#0018
      0x7002003E,  //  0017  JMP		#0057
      0x8808031D,  //  0018  GETMBR	R2	R1	K29
      0x540E001F,  //  0019  LDINT	R3	32
      0x1C080403,  //  001A  EQ	R2	R2	R3
      0x780A0004,  //  001B  JMPF	R2	#0021
      0x8C080189,  //  001C  GETMET	R2	R0	K137
      0x5C100200,  //  001D  MOVE	R4	R1
      0x7C080400,  //  001E  CALL	R2	2
      0x80040400,  //  001F  RET	1	R2
      0x70020035,  //  0020  JMP		#0057
      0x8808031D,  //  0021  GETMBR	R2	R1	K29
      0x540E0021,  //  0022  LDINT	R3	34
      0x1C080403,  //  0023  EQ	R2	R2	R3
      0x780A0004,  //  0024  JMPF	R2	#002A
      0x8C08018A,  //  0025  GETMET	R2	R0	K138
      0x5C100200,  //  0026  MOVE	R4	R1
      0x7C080400,  //  0027  CALL	R2	2
      0x80040400,  //  0028  RET	1	R2
      0x7002002C,  //  0029  JMP		#0057
      0x8808031D,  //  002A  GETMBR	R2	R1	K29
      0x540E0023,  //  002B  LDINT	R3	36
      0x1C080403,  //  002C  EQ	R2	R2	R3
      0x780A0004,  //  002D  JMPF	R2	#0033
      0x8C08018B,  //  002E  GETMET	R2	R0	K139
      0x5C100200,  //  002F  MOVE	R4	R1
      0x7C080400,  //  0030  CALL	R2	2
      0x80040400,  //  0031  RET	1	R2
      0x70020023,  //  0032  JMP		#0057
      0x8808031D,  //  0033  GETMBR	R2	R1	K29
      0x540E002F,  //  0034  LDINT	R3	48
      0x1C080403,  //  0035  EQ	R2	R2	R3
      0x780A0004,  //  0036  JMPF	R2	#003C
      0x8C08018C,  //  0037  GETMET	R2	R0	K140
      0x5C100200,  //  0038  MOVE	R4	R1
      0x7C080400,  //  0039  CALL	R2	2
      0x80040400,  //  003A  RET	1	R2
      0x7002001A,  //  003B  JMP		#0057
      0x8808031D,  //  003C  GETMBR	R2	R1	K29
      0x540E0031,  //  003D  LDINT	R3	50
      0x1C080403,  //  003E  EQ	R2	R2	R3
      0x780A0004,  //  003F  JMPF	R2	#0045
      0x8C08018D,  //  0040  GETMET	R2	R0	K141
      0x5C100200,  //  0041  MOVE	R4	R1
      0x7C080400,  //  0042  CALL	R2	2
      0x80040400,  //  0043  RET	1	R2
      0x70020011,  //  0044  JMP		#0057
      0x8808031D,  //  0045  GETMBR	R2	R1	K29
      0x540E003F,  //  0046  LDINT	R3	64
      0x1C080403,  //  0047  EQ	R2	R2	R3
      0x780A0004,  //  0048  JMPF	R2	#004E
      0x8C08018E,  //  0049  GETMET	R2	R0	K142
      0x5C100200,  //  004A  MOVE	R4	R1
      0x7C080400,  //  004B  CALL	R2	2
      0x80040400,  //  004C  RET	1	R2
      0x70020008,  //  004D  JMP		#0057
      0xB80A0A00,  //  004E  GETNGBL	R2	K5
      0x600C0018,  //  004F  GETGBL	R3	G24
      0x5810008F,  //  0050  LDCONST	R4	K143
      0x8814031D,  //  0051  GETMBR	R5	R1	K29
      0x7C0C0400,  //  0052  CALL	R3	2
      0x58100022,  //  0053  LDCONST	R4	K34
      0x7C080400,  //  0054  CALL	R2	2
      0x50080000,  //  0055  LDBOOL	R2	0	0
      0x80040400,  //  0056  RET	1	R2
      0x50080000,  //  0057  LDBOOL	R2	0	0
      0x80040400,  //  0058  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Sigma1
********************************************************************/
be_local_closure(class_Matter_Commisioning_Context_parse_Sigma1,   /* name */
  be_nested_proto(
    37,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commisioning_Context,     /* shared constants */
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[495]) {  /* code */
      0xA40A1200,  //  0000  IMPORT	R2	K9
      0x880C0300,  //  0001  GETMBR	R3	R1	K0
      0x8810031D,  //  0002  GETMBR	R4	R1	K29
      0x5416002F,  //  0003  LDINT	R5	48
      0x20100805,  //  0004  NE	R4	R4	R5
      0x74120005,  //  0005  JMPT	R4	#000C
      0x8810031E,  //  0006  GETMBR	R4	R1	K30
      0x2010091F,  //  0007  NE	R4	R4	K31
      0x74120002,  //  0008  JMPT	R4	#000C
      0x88100320,  //  0009  GETMBR	R4	R1	K32
      0x2010091F,  //  000A  NE	R4	R4	K31
      0x7812000C,  //  000B  JMPF	R4	#0019
      0xB8120A00,  //  000C  GETNGBL	R4	K5
      0x58140023,  //  000D  LDCONST	R5	K35
      0x58180008,  //  000E  LDCONST	R6	K8
      0x7C100400,  //  000F  CALL	R4	2
      0x8C100124,  //  0010  GETMET	R4	R0	K36
      0x5C180200,  //  0011  MOVE	R6	R1
      0x581C0025,  //  0012  LDCONST	R7	K37
      0x5820001F,  //  0013  LDCONST	R8	K31
      0x58240022,  //  0014  LDCONST	R9	K34
      0x50280000,  //  0015  LDBOOL	R10	0	0
      0x7C100C00,  //  0016  CALL	R4	6
      0x50100000,  //  0017  LDBOOL	R4	0	0
      0x80040800,  //  0018  RET	1	R4
      0xB8124C00,  //  0019  GETNGBL	R4	K38
      0x8C100990,  //  001A  GETMET	R4	R4	K144
      0x7C100200,  //  001B  CALL	R4	1
      0x8C100928,  //  001C  GETMET	R4	R4	K40
      0x88180301,  //  001D  GETMBR	R6	R1	K1
      0x881C0302,  //  001E  GETMBR	R7	R1	K2
      0x7C100600,  //  001F  CALL	R4	3
      0x88140991,  //  0020  GETMBR	R5	R4	K145
      0x900EF005,  //  0021  SETMBR	R3	K120	R5
      0x88140992,  //  0022  GETMBR	R5	R4	K146
      0x4C180000,  //  0023  LDNIL	R6
      0x20140A06,  //  0024  NE	R5	R5	R6
      0x78160003,  //  0025  JMPF	R5	#002A
      0x88140993,  //  0026  GETMBR	R5	R4	K147
      0x4C180000,  //  0027  LDNIL	R6
      0x20140A06,  //  0028  NE	R5	R5	R6
      0x74160000,  //  0029  JMPT	R5	#002B
      0x50140001,  //  002A  LDBOOL	R5	0	1
      0x50140200,  //  002B  LDBOOL	R5	1	0
      0x4C180000,  //  002C  LDNIL	R6
      0x78160014,  //  002D  JMPF	R5	#0043
      0x881C010D,  //  002E  GETMBR	R7	R0	K13
      0x881C0F0E,  //  002F  GETMBR	R7	R7	K14
      0x8C1C0F94,  //  0030  GETMET	R7	R7	K148
      0x88240992,  //  0031  GETMBR	R9	R4	K146
      0x7C1C0400,  //  0032  CALL	R7	2
      0x5C180E00,  //  0033  MOVE	R6	R7
      0x4C1C0000,  //  0034  LDNIL	R7
      0x1C1C0C07,  //  0035  EQ	R7	R6	R7
      0x741E0003,  //  0036  JMPT	R7	#003B
      0x881C0D95,  //  0037  GETMBR	R7	R6	K149
      0x4C200000,  //  0038  LDNIL	R8
      0x1C1C0E08,  //  0039  EQ	R7	R7	R8
      0x781E0007,  //  003A  JMPF	R7	#0043
      0xB81E0A00,  //  003B  GETNGBL	R7	K5
      0x60200018,  //  003C  GETGBL	R8	G24
      0x58240096,  //  003D  LDCONST	R9	K150
      0x88280992,  //  003E  GETMBR	R10	R4	K146
      0x7C200400,  //  003F  CALL	R8	2
      0x58240008,  //  0040  LDCONST	R9	K8
      0x7C1C0400,  //  0041  CALL	R7	2
      0x50140000,  //  0042  LDBOOL	R5	0	0
      0x781600C1,  //  0043  JMPF	R5	#0106
      0xB81E0A00,  //  0044  GETNGBL	R7	K5
      0x58200097,  //  0045  LDCONST	R8	K151
      0x58240008,  //  0046  LDCONST	R9	K8
      0x7C1C0400,  //  0047  CALL	R7	2
      0x881C0935,  //  0048  GETMBR	R7	R4	K53
      0x88200992,  //  0049  GETMBR	R8	R4	K146
      0x001C0E08,  //  004A  ADD	R7	R7	R8
      0x60200015,  //  004B  GETGBL	R8	G21
      0x7C200000,  //  004C  CALL	R8	0
      0x8C20114E,  //  004D  GETMET	R8	R8	K78
      0x58280098,  //  004E  LDCONST	R10	K152
      0x7C200400,  //  004F  CALL	R8	2
      0x8C240560,  //  0050  GETMET	R9	R2	K96
      0x7C240200,  //  0051  CALL	R9	1
      0x8C241361,  //  0052  GETMET	R9	R9	K97
      0x882C0D66,  //  0053  GETMBR	R11	R6	K102
      0x5C300E00,  //  0054  MOVE	R12	R7
      0x5C341000,  //  0055  MOVE	R13	R8
      0x543A000F,  //  0056  LDINT	R14	16
      0x7C240A00,  //  0057  CALL	R9	5
      0x60280015,  //  0058  GETGBL	R10	G21
      0x7C280000,  //  0059  CALL	R10	0
      0x8C28154E,  //  005A  GETMET	R10	R10	K78
      0x58300099,  //  005B  LDCONST	R12	K153
      0x7C280400,  //  005C  CALL	R10	2
      0x542DFFEE,  //  005D  LDINT	R11	-17
      0x402E3E0B,  //  005E  CONNECT	R11	K31	R11
      0x88300993,  //  005F  GETMBR	R12	R4	K147
      0x942C180B,  //  0060  GETIDX	R11	R12	R11
      0x5431FFEF,  //  0061  LDINT	R12	-16
      0x40301903,  //  0062  CONNECT	R12	R12	K3
      0x88340993,  //  0063  GETMBR	R13	R4	K147
      0x94301A0C,  //  0064  GETIDX	R12	R13	R12
      0x8C340568,  //  0065  GETMET	R13	R2	K104
      0x5C3C1200,  //  0066  MOVE	R15	R9
      0x5C401400,  //  0067  MOVE	R16	R10
      0x60440015,  //  0068  GETGBL	R17	G21
      0x7C440000,  //  0069  CALL	R17	0
      0x6048000C,  //  006A  GETGBL	R18	G12
      0x5C4C1600,  //  006B  MOVE	R19	R11
      0x7C480200,  //  006C  CALL	R18	1
      0x544E000F,  //  006D  LDINT	R19	16
      0x7C340C00,  //  006E  CALL	R13	6
      0x8C381B6A,  //  006F  GETMET	R14	R13	K106
      0x5C401600,  //  0070  MOVE	R16	R11
      0x7C380400,  //  0071  CALL	R14	2
      0x8C3C1B6B,  //  0072  GETMET	R15	R13	K107
      0x7C3C0200,  //  0073  CALL	R15	1
      0x1C40180F,  //  0074  EQ	R16	R12	R15
      0x7842008A,  //  0075  JMPF	R16	#0101
      0x88400D95,  //  0076  GETMBR	R16	R6	K149
      0x900F2A10,  //  0077  SETMBR	R3	K149	R16
      0x8840039B,  //  0078  GETMBR	R16	R1	K155
      0x900F3410,  //  0079  SETMBR	R3	K154	R16
      0x8C40079C,  //  007A  GETMET	R16	R3	K156
      0x7C400200,  //  007B  CALL	R16	1
      0x8840092E,  //  007C  GETMBR	R16	R4	K46
      0x900E5A10,  //  007D  SETMBR	R3	K45	R16
      0x8840010D,  //  007E  GETMBR	R16	R0	K13
      0x8840210E,  //  007F  GETMBR	R16	R16	K14
      0x8C402130,  //  0080  GETMET	R16	R16	K48
      0x7C400200,  //  0081  CALL	R16	1
      0x900E5E10,  //  0082  SETMBR	R3	K47	R16
      0xB8420A00,  //  0083  GETNGBL	R16	K5
      0x60440018,  //  0084  GETGBL	R17	G24
      0x58480031,  //  0085  LDCONST	R18	K49
      0x884C072F,  //  0086  GETMBR	R19	R3	K47
      0x88500332,  //  0087  GETMBR	R20	R1	K50
      0x88540333,  //  0088  GETMBR	R21	R1	K51
      0x7C440800,  //  0089  CALL	R17	4
      0x58480008,  //  008A  LDCONST	R18	K8
      0x7C400400,  //  008B  CALL	R16	2
      0x88400D66,  //  008C  GETMBR	R16	R6	K102
      0x900ECC10,  //  008D  SETMBR	R3	K102	R16
      0x8C400537,  //  008E  GETMET	R16	R2	K55
      0x544A000F,  //  008F  LDINT	R18	16
      0x7C400400,  //  0090  CALL	R16	2
      0x900F3A10,  //  0091  SETMBR	R3	K157	R16
      0x60400015,  //  0092  GETGBL	R16	G21
      0x7C400000,  //  0093  CALL	R16	0
      0x8C40214E,  //  0094  GETMET	R16	R16	K78
      0x5848009E,  //  0095  LDCONST	R18	K158
      0x7C400400,  //  0096  CALL	R16	2
      0x88440935,  //  0097  GETMBR	R17	R4	K53
      0x8848079D,  //  0098  GETMBR	R18	R3	K157
      0x00442212,  //  0099  ADD	R17	R17	R18
      0x8C480560,  //  009A  GETMET	R18	R2	K96
      0x7C480200,  //  009B  CALL	R18	1
      0x8C482561,  //  009C  GETMET	R18	R18	K97
      0x88500766,  //  009D  GETMBR	R20	R3	K102
      0x5C542200,  //  009E  MOVE	R21	R17
      0x5C582000,  //  009F  MOVE	R22	R16
      0x545E000F,  //  00A0  LDINT	R23	16
      0x7C480A00,  //  00A1  CALL	R18	5
      0x8C4C0568,  //  00A2  GETMET	R19	R2	K104
      0x5C542400,  //  00A3  MOVE	R21	R18
      0x60580015,  //  00A4  GETGBL	R22	G21
      0x7C580000,  //  00A5  CALL	R22	0
      0x8C582D4E,  //  00A6  GETMET	R22	R22	K78
      0x5860009F,  //  00A7  LDCONST	R24	K159
      0x7C580400,  //  00A8  CALL	R22	2
      0x605C0015,  //  00A9  GETGBL	R23	G21
      0x7C5C0000,  //  00AA  CALL	R23	0
      0x5860001F,  //  00AB  LDCONST	R24	K31
      0x5466000F,  //  00AC  LDINT	R25	16
      0x7C4C0C00,  //  00AD  CALL	R19	6
      0x8C50276B,  //  00AE  GETMET	R20	R19	K107
      0x7C500200,  //  00AF  CALL	R20	1
      0xB8564C00,  //  00B0  GETNGBL	R21	K38
      0x8C542BA0,  //  00B1  GETMET	R21	R21	K160
      0x7C540200,  //  00B2  CALL	R21	1
      0x8858079D,  //  00B3  GETMBR	R22	R3	K157
      0x90572416,  //  00B4  SETMBR	R21	K146	R22
      0x90574214,  //  00B5  SETMBR	R21	K161	R20
      0x8858072F,  //  00B6  GETMBR	R22	R3	K47
      0x90574416,  //  00B7  SETMBR	R21	K162	R22
      0x88580935,  //  00B8  GETMBR	R22	R4	K53
      0x885C0D9D,  //  00B9  GETMBR	R23	R6	K157
      0x00582C17,  //  00BA  ADD	R22	R22	R23
      0x8C5C0560,  //  00BB  GETMET	R23	R2	K96
      0x7C5C0200,  //  00BC  CALL	R23	1
      0x8C5C2F61,  //  00BD  GETMET	R23	R23	K97
      0x88640766,  //  00BE  GETMBR	R25	R3	K102
      0x5C682C00,  //  00BF  MOVE	R26	R22
      0x606C0015,  //  00C0  GETGBL	R27	G21
      0x7C6C0000,  //  00C1  CALL	R27	0
      0x8C6C374E,  //  00C2  GETMET	R27	R27	K78
      0x587400A3,  //  00C3  LDCONST	R29	K163
      0x7C6C0400,  //  00C4  CALL	R27	2
      0x5472002F,  //  00C5  LDINT	R28	48
      0x7C5C0A00,  //  00C6  CALL	R23	5
      0x5462000E,  //  00C7  LDINT	R24	15
      0x40623E18,  //  00C8  CONNECT	R24	K31	R24
      0x94602E18,  //  00C9  GETIDX	R24	R23	R24
      0x5466000F,  //  00CA  LDINT	R25	16
      0x546A001E,  //  00CB  LDINT	R26	31
      0x4064321A,  //  00CC  CONNECT	R25	R25	R26
      0x94642E19,  //  00CD  GETIDX	R25	R23	R25
      0x546A001F,  //  00CE  LDINT	R26	32
      0x546E002E,  //  00CF  LDINT	R27	47
      0x4068341B,  //  00D0  CONNECT	R26	R26	R27
      0x94682E1A,  //  00D1  GETIDX	R26	R23	R26
      0xB86E1400,  //  00D2  GETNGBL	R27	K10
      0x8C6C375F,  //  00D3  GETMET	R27	R27	K95
      0x7C6C0200,  //  00D4  CALL	R27	1
      0x8C702B3E,  //  00D5  GETMET	R28	R21	K62
      0x7C700200,  //  00D6  CALL	R28	1
      0x4C740000,  //  00D7  LDNIL	R29
      0x900E541D,  //  00D8  SETMBR	R3	K42	R29
      0x8C740340,  //  00D9  GETMET	R29	R1	K64
      0x547E0032,  //  00DA  LDINT	R31	51
      0x50800200,  //  00DB  LDBOOL	R32	1	0
      0x7C740600,  //  00DC  CALL	R29	3
      0x8C783B41,  //  00DD  GETMET	R30	R29	K65
      0x5C803800,  //  00DE  MOVE	R32	R28
      0x7C780400,  //  00DF  CALL	R30	2
      0x887C0142,  //  00E0  GETMBR	R31	R0	K66
      0x8C7C3F43,  //  00E1  GETMET	R31	R31	K67
      0x5C843A00,  //  00E2  MOVE	R33	R29
      0x7C7C0400,  //  00E3  CALL	R31	2
      0x8C7C0780,  //  00E4  GETMET	R31	R3	K128
      0x7C7C0200,  //  00E5  CALL	R31	1
      0x8C7C071C,  //  00E6  GETMET	R31	R3	K28
      0x5C843000,  //  00E7  MOVE	R33	R24
      0x5C883200,  //  00E8  MOVE	R34	R25
      0x5C8C3400,  //  00E9  MOVE	R35	R26
      0x5C903600,  //  00EA  MOVE	R36	R27
      0x7C7C0A00,  //  00EB  CALL	R31	5
      0x887C0D71,  //  00EC  GETMBR	R31	R6	K113
      0x900EE21F,  //  00ED  SETMBR	R3	K113	R31
      0x900F031F,  //  00EE  SETMBR	R3	K129	K31
      0x8C7C0782,  //  00EF  GETMET	R31	R3	K130
      0x7C7C0200,  //  00F0  CALL	R31	1
      0x8C7C0783,  //  00F1  GETMET	R31	R3	K131
      0x50840200,  //  00F2  LDBOOL	R33	1	0
      0x7C7C0400,  //  00F3  CALL	R31	2
      0x8C7C0784,  //  00F4  GETMET	R31	R3	K132
      0x7C7C0200,  //  00F5  CALL	R31	1
      0x8C7C0785,  //  00F6  GETMET	R31	R3	K133
      0x7C7C0200,  //  00F7  CALL	R31	1
      0x8C7C0786,  //  00F8  GETMET	R31	R3	K134
      0x7C7C0200,  //  00F9  CALL	R31	1
      0xB87E0A00,  //  00FA  GETNGBL	R31	K5
      0x588000A4,  //  00FB  LDCONST	R32	K164
      0x58840008,  //  00FC  LDCONST	R33	K8
      0x7C7C0400,  //  00FD  CALL	R31	2
      0x507C0200,  //  00FE  LDBOOL	R31	1	0
      0x80043E00,  //  00FF  RET	1	R31
      0x70020004,  //  0100  JMP		#0106
      0xB8420A00,  //  0101  GETNGBL	R16	K5
      0x584400A5,  //  0102  LDCONST	R17	K165
      0x58480008,  //  0103  LDCONST	R18	K8
      0x7C400400,  //  0104  CALL	R16	2
      0x50140000,  //  0105  LDBOOL	R5	0	0
      0x5C1C0A00,  //  0106  MOVE	R7	R5
      0x741E00E4,  //  0107  JMPT	R7	#01ED
      0x8C1C01A6,  //  0108  GETMET	R7	R0	K166
      0x882409A7,  //  0109  GETMBR	R9	R4	K167
      0x88280935,  //  010A  GETMBR	R10	R4	K53
      0x7C1C0600,  //  010B  CALL	R7	3
      0x900F2A07,  //  010C  SETMBR	R3	K149	R7
      0x4C200000,  //  010D  LDNIL	R8
      0x1C200608,  //  010E  EQ	R8	R3	R8
      0x74220003,  //  010F  JMPT	R8	#0114
      0x88200795,  //  0110  GETMBR	R8	R3	K149
      0x4C240000,  //  0111  LDNIL	R9
      0x1C201009,  //  0112  EQ	R8	R8	R9
      0x7822000C,  //  0113  JMPF	R8	#0121
      0xB8220A00,  //  0114  GETNGBL	R8	K5
      0x582400A8,  //  0115  LDCONST	R9	K168
      0x58280008,  //  0116  LDCONST	R10	K8
      0x7C200400,  //  0117  CALL	R8	2
      0x8C200124,  //  0118  GETMET	R8	R0	K36
      0x5C280200,  //  0119  MOVE	R10	R1
      0x582C0025,  //  011A  LDCONST	R11	K37
      0x5830001F,  //  011B  LDCONST	R12	K31
      0x58340025,  //  011C  LDCONST	R13	K37
      0x50380000,  //  011D  LDBOOL	R14	0	0
      0x7C200C00,  //  011E  CALL	R8	6
      0x50200000,  //  011F  LDBOOL	R8	0	0
      0x80041000,  //  0120  RET	1	R8
      0x8820039B,  //  0121  GETMBR	R8	R1	K155
      0x900F3408,  //  0122  SETMBR	R3	K154	R8
      0x8C20079C,  //  0123  GETMET	R8	R3	K156
      0x7C200200,  //  0124  CALL	R8	1
      0x8820092E,  //  0125  GETMBR	R8	R4	K46
      0x900E5A08,  //  0126  SETMBR	R3	K45	R8
      0x8820010D,  //  0127  GETMBR	R8	R0	K13
      0x8820110E,  //  0128  GETMBR	R8	R8	K14
      0x8C201130,  //  0129  GETMET	R8	R8	K48
      0x7C200200,  //  012A  CALL	R8	1
      0x900E5E08,  //  012B  SETMBR	R3	K47	R8
      0xB8220A00,  //  012C  GETNGBL	R8	K5
      0x60240018,  //  012D  GETGBL	R9	G24
      0x58280031,  //  012E  LDCONST	R10	K49
      0x882C072F,  //  012F  GETMBR	R11	R3	K47
      0x88300332,  //  0130  GETMBR	R12	R1	K50
      0x88340333,  //  0131  GETMBR	R13	R1	K51
      0x7C240800,  //  0132  CALL	R9	4
      0x58280008,  //  0133  LDCONST	R10	K8
      0x7C200400,  //  0134  CALL	R8	2
      0x8C200537,  //  0135  GETMET	R8	R2	K55
      0x542A000F,  //  0136  LDINT	R10	16
      0x7C200400,  //  0137  CALL	R8	2
      0x900F3A08,  //  0138  SETMBR	R3	K157	R8
      0x8C200537,  //  0139  GETMET	R8	R2	K55
      0x542A001F,  //  013A  LDINT	R10	32
      0x7C200400,  //  013B  CALL	R8	2
      0x900F5208,  //  013C  SETMBR	R3	K169	R8
      0x8C20057A,  //  013D  GETMET	R8	R2	K122
      0x7C200200,  //  013E  CALL	R8	1
      0x8C2011AA,  //  013F  GETMET	R8	R8	K170
      0x882807A9,  //  0140  GETMBR	R10	R3	K169
      0x7C200400,  //  0141  CALL	R8	2
      0x900EF208,  //  0142  SETMBR	R3	K121	R8
      0x8C200537,  //  0143  GETMET	R8	R2	K55
      0x542A001F,  //  0144  LDINT	R10	32
      0x7C200400,  //  0145  CALL	R8	2
      0x8C24057A,  //  0146  GETMET	R9	R2	K122
      0x7C240200,  //  0147  CALL	R9	1
      0x8C2413AB,  //  0148  GETMET	R9	R9	K171
      0x882C07A9,  //  0149  GETMBR	R11	R3	K169
      0x88300991,  //  014A  GETMBR	R12	R4	K145
      0x7C240600,  //  014B  CALL	R9	3
      0x900ECC09,  //  014C  SETMBR	R3	K102	R9
      0xB8264C00,  //  014D  GETNGBL	R9	K38
      0x8824136D,  //  014E  GETMBR	R9	R9	K109
      0x8C241375,  //  014F  GETMET	R9	R9	K117
      0x7C240200,  //  0150  CALL	R9	1
      0x8C281376,  //  0151  GETMET	R10	R9	K118
      0x58300025,  //  0152  LDCONST	R12	K37
      0xB8364C00,  //  0153  GETNGBL	R13	K38
      0x88341B6D,  //  0154  GETMBR	R13	R13	K109
      0x88341BAC,  //  0155  GETMBR	R13	R13	K172
      0x8C380FAD,  //  0156  GETMET	R14	R7	K173
      0x7C380200,  //  0157  CALL	R14	1
      0x7C280800,  //  0158  CALL	R10	4
      0x8C281376,  //  0159  GETMET	R10	R9	K118
      0x58300022,  //  015A  LDCONST	R12	K34
      0xB8364C00,  //  015B  GETNGBL	R13	K38
      0x88341B6D,  //  015C  GETMBR	R13	R13	K109
      0x88341BAC,  //  015D  GETMBR	R13	R13	K172
      0x8C380FAE,  //  015E  GETMET	R14	R7	K174
      0x7C380200,  //  015F  CALL	R14	1
      0x7C280800,  //  0160  CALL	R10	4
      0x8C281376,  //  0161  GETMET	R10	R9	K118
      0x58300008,  //  0162  LDCONST	R12	K8
      0xB8364C00,  //  0163  GETNGBL	R13	K38
      0x88341B6D,  //  0164  GETMBR	R13	R13	K109
      0x88341BAC,  //  0165  GETMBR	R13	R13	K172
      0x88380779,  //  0166  GETMBR	R14	R3	K121
      0x7C280800,  //  0167  CALL	R10	4
      0x8C281376,  //  0168  GETMET	R10	R9	K118
      0x54320003,  //  0169  LDINT	R12	4
      0xB8364C00,  //  016A  GETNGBL	R13	K38
      0x88341B6D,  //  016B  GETMBR	R13	R13	K109
      0x88341BAC,  //  016C  GETMBR	R13	R13	K172
      0x88380991,  //  016D  GETMBR	R14	R4	K145
      0x7C280800,  //  016E  CALL	R10	4
      0x8C28057A,  //  016F  GETMET	R10	R2	K122
      0x7C280200,  //  0170  CALL	R10	1
      0x8C2815AF,  //  0171  GETMET	R10	R10	K175
      0x8C300FB0,  //  0172  GETMET	R12	R7	K176
      0x7C300200,  //  0173  CALL	R12	1
      0x8C34133E,  //  0174  GETMET	R13	R9	K62
      0x7C340200,  //  0175  CALL	R13	1
      0x7C280600,  //  0176  CALL	R10	3
      0xB82E4C00,  //  0177  GETNGBL	R11	K38
      0x882C176D,  //  0178  GETMBR	R11	R11	K109
      0x8C2C1775,  //  0179  GETMET	R11	R11	K117
      0x7C2C0200,  //  017A  CALL	R11	1
      0x8C301776,  //  017B  GETMET	R12	R11	K118
      0x58380025,  //  017C  LDCONST	R14	K37
      0xB83E4C00,  //  017D  GETNGBL	R15	K38
      0x883C1F6D,  //  017E  GETMBR	R15	R15	K109
      0x883C1FAC,  //  017F  GETMBR	R15	R15	K172
      0x8C400FAD,  //  0180  GETMET	R16	R7	K173
      0x7C400200,  //  0181  CALL	R16	1
      0x7C300800,  //  0182  CALL	R12	4
      0x8C301776,  //  0183  GETMET	R12	R11	K118
      0x58380022,  //  0184  LDCONST	R14	K34
      0xB83E4C00,  //  0185  GETNGBL	R15	K38
      0x883C1F6D,  //  0186  GETMBR	R15	R15	K109
      0x883C1FAC,  //  0187  GETMBR	R15	R15	K172
      0x8C400FAE,  //  0188  GETMET	R16	R7	K174
      0x7C400200,  //  0189  CALL	R16	1
      0x7C300800,  //  018A  CALL	R12	4
      0x8C301776,  //  018B  GETMET	R12	R11	K118
      0x58380008,  //  018C  LDCONST	R14	K8
      0xB83E4C00,  //  018D  GETNGBL	R15	K38
      0x883C1F6D,  //  018E  GETMBR	R15	R15	K109
      0x883C1FAC,  //  018F  GETMBR	R15	R15	K172
      0x5C401400,  //  0190  MOVE	R16	R10
      0x7C300800,  //  0191  CALL	R12	4
      0x8C301776,  //  0192  GETMET	R12	R11	K118
      0x543A0003,  //  0193  LDINT	R14	4
      0xB83E4C00,  //  0194  GETNGBL	R15	K38
      0x883C1F6D,  //  0195  GETMBR	R15	R15	K109
      0x883C1FAC,  //  0196  GETMBR	R15	R15	K172
      0x8840079D,  //  0197  GETMBR	R16	R3	K157
      0x7C300800,  //  0198  CALL	R12	4
      0x883009B1,  //  0199  GETMBR	R12	R4	K177
      0x900E540C,  //  019A  SETMBR	R3	K42	R12
      0x8C30054D,  //  019B  GETMET	R12	R2	K77
      0x7C300200,  //  019C  CALL	R12	1
      0x8C301916,  //  019D  GETMET	R12	R12	K22
      0x8838072A,  //  019E  GETMBR	R14	R3	K42
      0x7C300400,  //  019F  CALL	R12	2
      0x8C301917,  //  01A0  GETMET	R12	R12	K23
      0x7C300200,  //  01A1  CALL	R12	1
      0x60340015,  //  01A2  GETGBL	R13	G21
      0x7C340000,  //  01A3  CALL	R13	0
      0x8C341B4E,  //  01A4  GETMET	R13	R13	K78
      0x883C01B2,  //  01A5  GETMBR	R15	R0	K178
      0x7C340400,  //  01A6  CALL	R13	2
      0x8C380F14,  //  01A7  GETMET	R14	R7	K20
      0x7C380200,  //  01A8  CALL	R14	1
      0x00381C08,  //  01A9  ADD	R14	R14	R8
      0x883C0779,  //  01AA  GETMBR	R15	R3	K121
      0x00381C0F,  //  01AB  ADD	R14	R14	R15
      0x00381C0C,  //  01AC  ADD	R14	R14	R12
      0x8C3C0560,  //  01AD  GETMET	R15	R2	K96
      0x7C3C0200,  //  01AE  CALL	R15	1
      0x8C3C1F61,  //  01AF  GETMET	R15	R15	K97
      0x88440766,  //  01B0  GETMBR	R17	R3	K102
      0x5C481C00,  //  01B1  MOVE	R18	R14
      0x5C4C1A00,  //  01B2  MOVE	R19	R13
      0x5452000F,  //  01B3  LDINT	R20	16
      0x7C3C0A00,  //  01B4  CALL	R15	5
      0x8C40173E,  //  01B5  GETMET	R16	R11	K62
      0x7C400200,  //  01B6  CALL	R16	1
      0x8C440568,  //  01B7  GETMET	R17	R2	K104
      0x5C4C1E00,  //  01B8  MOVE	R19	R15
      0x60500015,  //  01B9  GETGBL	R20	G21
      0x7C500000,  //  01BA  CALL	R20	0
      0x8C50294E,  //  01BB  GETMET	R20	R20	K78
      0x885801B3,  //  01BC  GETMBR	R22	R0	K179
      0x7C500400,  //  01BD  CALL	R20	2
      0x60540015,  //  01BE  GETGBL	R21	G21
      0x7C540000,  //  01BF  CALL	R21	0
      0x6058000C,  //  01C0  GETGBL	R22	G12
      0x5C5C2000,  //  01C1  MOVE	R23	R16
      0x7C580200,  //  01C2  CALL	R22	1
      0x545E000F,  //  01C3  LDINT	R23	16
      0x7C440C00,  //  01C4  CALL	R17	6
      0x8C4823B4,  //  01C5  GETMET	R18	R17	K180
      0x5C502000,  //  01C6  MOVE	R20	R16
      0x7C480400,  //  01C7  CALL	R18	2
      0x8C4C236B,  //  01C8  GETMET	R19	R17	K107
      0x7C4C0200,  //  01C9  CALL	R19	1
      0x00482413,  //  01CA  ADD	R18	R18	R19
      0xB84E4C00,  //  01CB  GETNGBL	R19	K38
      0x8C4C27B5,  //  01CC  GETMET	R19	R19	K181
      0x7C4C0200,  //  01CD  CALL	R19	1
      0x904E6C08,  //  01CE  SETMBR	R19	K54	R8
      0x8850072F,  //  01CF  GETMBR	R20	R3	K47
      0x904E7014,  //  01D0  SETMBR	R19	K56	R20
      0x88500779,  //  01D1  GETMBR	R20	R3	K121
      0x904F6C14,  //  01D2  SETMBR	R19	K182	R20
      0x904F6E12,  //  01D3  SETMBR	R19	K183	R18
      0x8C50273E,  //  01D4  GETMET	R20	R19	K62
      0x7C500200,  //  01D5  CALL	R20	1
      0x900E7E14,  //  01D6  SETMBR	R3	K63	R20
      0x8C540340,  //  01D7  GETMET	R21	R1	K64
      0x545E0030,  //  01D8  LDINT	R23	49
      0x50600200,  //  01D9  LDBOOL	R24	1	0
      0x7C540600,  //  01DA  CALL	R21	3
      0x8C582B41,  //  01DB  GETMET	R22	R21	K65
      0x5C602800,  //  01DC  MOVE	R24	R20
      0x7C580400,  //  01DD  CALL	R22	2
      0xB85E0A00,  //  01DE  GETNGBL	R23	K5
      0x60600018,  //  01DF  GETGBL	R24	G24
      0x586400B8,  //  01E0  LDCONST	R25	K184
      0x8868072F,  //  01E1  GETMBR	R26	R3	K47
      0x886C075A,  //  01E2  GETMBR	R27	R3	K90
      0x8870075B,  //  01E3  GETMBR	R28	R3	K91
      0x7C600800,  //  01E4  CALL	R24	4
      0x58640022,  //  01E5  LDCONST	R25	K34
      0x7C5C0400,  //  01E6  CALL	R23	2
      0x885C0142,  //  01E7  GETMBR	R23	R0	K66
      0x8C5C2F43,  //  01E8  GETMET	R23	R23	K67
      0x5C642A00,  //  01E9  MOVE	R25	R21
      0x7C5C0400,  //  01EA  CALL	R23	2
      0x505C0200,  //  01EB  LDBOOL	R23	1	0
      0x80042E00,  //  01EC  RET	1	R23
      0x501C0200,  //  01ED  LDBOOL	R7	1	0
      0x80040E00,  //  01EE  RET	1	R7
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
