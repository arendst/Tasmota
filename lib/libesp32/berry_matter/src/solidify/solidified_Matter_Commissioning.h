/* Solidification of Matter_Commissioning.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Commisioning_Context' ktab size: 190, total: 359 (saved 1352 bytes)
static const bvalue be_ktab_class_Matter_Commisioning_Context[190] = {
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
  /* K36  */  be_const_int(3),
  /* K37  */  be_nested_str_weak(send_status_report),
  /* K38  */  be_const_int(1),
  /* K39  */  be_nested_str_weak(matter),
  /* K40  */  be_nested_str_weak(PBKDFParamRequest),
  /* K41  */  be_nested_str_weak(parse),
  /* K42  */  be_nested_str_weak(set_mode_PASE),
  /* K43  */  be_nested_str_weak(__Msg1),
  /* K44  */  be_nested_str_weak(passcodeId),
  /* K45  */  be_nested_str_weak(MTR_X3A_X20non_X2Dzero_X20passcode_X20id),
  /* K46  */  be_nested_str_weak(__future_initiator_session_id),
  /* K47  */  be_nested_str_weak(initiator_session_id),
  /* K48  */  be_nested_str_weak(__future_local_session_id),
  /* K49  */  be_nested_str_weak(gen_local_session_id),
  /* K50  */  be_nested_str_weak(MTR_X3A_X20_X2BSession_X20_X20_X20_X28_X256i_X29_X20from_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
  /* K51  */  be_nested_str_weak(remote_ip),
  /* K52  */  be_nested_str_weak(remote_port),
  /* K53  */  be_nested_str_weak(PBKDFParamResponse),
  /* K54  */  be_nested_str_weak(initiatorRandom),
  /* K55  */  be_nested_str_weak(responderRandom),
  /* K56  */  be_nested_str_weak(random),
  /* K57  */  be_nested_str_weak(responderSessionId),
  /* K58  */  be_nested_str_weak(pbkdf_parameters_salt),
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
  /* K69  */  be_const_int(2),
  /* K70  */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake1_X20message),
  /* K71  */  be_nested_str_weak(Pake1),
  /* K72  */  be_nested_str_weak(pA),
  /* K73  */  be_nested_str_weak(SPAKE2P_Matter),
  /* K74  */  be_nested_str_weak(commissioning_w0),
  /* K75  */  be_nested_str_weak(commissioning_L),
  /* K76  */  be_nested_str_weak(compute_pB),
  /* K77  */  be_nested_str_weak(compute_ZV_verifier),
  /* K78  */  be_nested_str_weak(SHA256),
  /* K79  */  be_nested_str_weak(fromstring),
  /* K80  */  be_nested_str_weak(Matter_Context_Prefix),
  /* K81  */  be_nested_str_weak(set_context),
  /* K82  */  be_nested_str_weak(compute_TT_hash),
  /* K83  */  be_nested_str_weak(Pake2),
  /* K84  */  be_nested_str_weak(pB),
  /* K85  */  be_nested_str_weak(cB),
  /* K86  */  be_nested_str_weak(__spake_cA),
  /* K87  */  be_nested_str_weak(cA),
  /* K88  */  be_nested_str_weak(__spake_Ke),
  /* K89  */  be_nested_str_weak(Ke),
  /* K90  */  be_nested_str_weak(MTR_X3A_X20New_X20Commissioning_X20_X28PASE_X20id_X3D_X25i_X29_X20from_X20_X5B_X25s_X5D_X3A_X25i),
  /* K91  */  be_nested_str_weak(_ip),
  /* K92  */  be_nested_str_weak(_port),
  /* K93  */  be_nested_str_weak(MTR_X3A_X20invalid_X20Pake3_X20message),
  /* K94  */  be_nested_str_weak(Pake3),
  /* K95  */  be_nested_str_weak(MTR_X3A_X20invalid_X20cA_X20received),
  /* K96  */  be_nested_str_weak(rtc_utc),
  /* K97  */  be_nested_str_weak(HKDF_SHA256),
  /* K98  */  be_nested_str_weak(derive),
  /* K99  */  be_nested_str_weak(SEKeys_Info),
  /* K100 */  be_nested_str_weak(add_session),
  /* K101 */  be_const_int(3),
  /* K102 */  be_const_int(1),
  /* K103 */  be_const_int(2),
  /* K104 */  be_nested_str_weak(Sigma3),
  /* K105 */  be_nested_str_weak(S3K_Info),
  /* K106 */  be_nested_str_weak(shared_secret),
  /* K107 */  be_nested_str_weak(TBEData3Encrypted),
  /* K108 */  be_const_int(2147483647),
  /* K109 */  be_nested_str_weak(AES_CCM),
  /* K110 */  be_nested_str_weak(TBEData3_Nonce),
  /* K111 */  be_nested_str_weak(decrypt),
  /* K112 */  be_nested_str_weak(tag),
  /* K113 */  be_nested_str_weak(MTR_X3A_X20Tag_X20don_X27t_X20match),
  /* K114 */  be_nested_str_weak(TLV),
  /* K115 */  be_nested_str_weak(findsubval),
  /* K116 */  be_nested_str_weak(findsub),
  /* K117 */  be_nested_str_weak(int),
  /* K118 */  be_nested_str_weak(peer_node_id),
  /* K119 */  be_nested_str_weak(int64),
  /* K120 */  be_nested_str_weak(fromu32),
  /* K121 */  be_nested_str_weak(tobytes),
  /* K122 */  be_nested_str_weak(Matter_TLV_struct),
  /* K123 */  be_nested_str_weak(add_TLV),
  /* K124 */  be_nested_str_weak(B1),
  /* K125 */  be_nested_str_weak(__initiator_pub),
  /* K126 */  be_nested_str_weak(__responder_pub),
  /* K127 */  be_nested_str_weak(EC_P256),
  /* K128 */  be_nested_str_weak(ecdsa_verify_sha256),
  /* K129 */  be_nested_str_weak(MTR_X3A_X20sigma3_tbs_X20does_X20not_X20have_X20a_X20valid_X20signature),
  /* K130 */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X20Invalid_X20signature_X2C_X20trying_X20anyways),
  /* K131 */  be_nested_str_weak(MTR_X3A_X20Sigma3_X20verified_X2C_X20computing_X20new_X20keys),
  /* K132 */  be_nested_str_weak(Msg3),
  /* K133 */  be_nested_str_weak(close),
  /* K134 */  be_nested_str_weak(_breadcrumb),
  /* K135 */  be_nested_str_weak(counter_snd_next),
  /* K136 */  be_nested_str_weak(set_persist),
  /* K137 */  be_nested_str_weak(set_no_expiration),
  /* K138 */  be_nested_str_weak(persist_to_fabric),
  /* K139 */  be_nested_str_weak(save),
  /* K140 */  be_nested_str_weak(is_commissioning_open),
  /* K141 */  be_nested_str_weak(MTR_X3A_X20commissioning_X20not_X20open),
  /* K142 */  be_nested_str_weak(parse_PBKDFParamRequest),
  /* K143 */  be_nested_str_weak(parse_Pake1),
  /* K144 */  be_nested_str_weak(parse_Pake3),
  /* K145 */  be_nested_str_weak(parse_Sigma1),
  /* K146 */  be_nested_str_weak(parse_Sigma3),
  /* K147 */  be_nested_str_weak(parse_StatusReport),
  /* K148 */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28secure_X20channel_X29_X20_X2502X),
  /* K149 */  be_nested_str_weak(Sigma1),
  /* K150 */  be_nested_str_weak(initiatorEphPubKey),
  /* K151 */  be_nested_str_weak(resumptionID),
  /* K152 */  be_nested_str_weak(initiatorResumeMIC),
  /* K153 */  be_nested_str_weak(find_session_by_resumption_id),
  /* K154 */  be_nested_str_weak(_fabric),
  /* K155 */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Sigma1_X20Resumption_X20FAILED_X2C_X20session_X20not_X20found_X20resumption_id_X3D_X25s),
  /* K156 */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Sigma1_X20Resumption),
  /* K157 */  be_nested_str_weak(Sigma1_Resume),
  /* K158 */  be_nested_str_weak(NCASE_SigmaS1),
  /* K159 */  be_nested_str_weak(_source_node_id),
  /* K160 */  be_nested_str_weak(source_node_id),
  /* K161 */  be_nested_str_weak(set_mode_CASE),
  /* K162 */  be_nested_str_weak(resumption_id),
  /* K163 */  be_nested_str_weak(Sigma2_Resume),
  /* K164 */  be_nested_str_weak(NCASE_SigmaS2),
  /* K165 */  be_nested_str_weak(Sigma2Resume),
  /* K166 */  be_nested_str_weak(sigma2ResumeMIC),
  /* K167 */  be_nested_str_weak(responderSessionID),
  /* K168 */  be_nested_str_weak(SessionResumptionKeys),
  /* K169 */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Sigma1_X20Resumption_X20SUCCEEDED),
  /* K170 */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Sigma1_X20Resumption_X20FAILED_X2C_X20switching_X20to_X20CASE),
  /* K171 */  be_nested_str_weak(find_fabric_by_destination_id),
  /* K172 */  be_nested_str_weak(destinationId),
  /* K173 */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
  /* K174 */  be_nested_str_weak(__responder_priv),
  /* K175 */  be_nested_str_weak(public_key),
  /* K176 */  be_nested_str_weak(shared_key),
  /* K177 */  be_nested_str_weak(B2),
  /* K178 */  be_nested_str_weak(get_noc),
  /* K179 */  be_nested_str_weak(get_icac),
  /* K180 */  be_nested_str_weak(ecdsa_sign_sha256),
  /* K181 */  be_nested_str_weak(get_pk),
  /* K182 */  be_nested_str_weak(Msg1),
  /* K183 */  be_nested_str_weak(S2K_Info),
  /* K184 */  be_nested_str_weak(TBEData2_Nonce),
  /* K185 */  be_nested_str_weak(encrypt),
  /* K186 */  be_nested_str_weak(Sigma2),
  /* K187 */  be_nested_str_weak(responderEphPubKey),
  /* K188 */  be_nested_str_weak(encrypted2),
  /* K189 */  be_nested_str_weak(MTR_X3A_X20New_X20Connection_X20_X28CASE_X20id_X3D_X25i_X29_X20from_X20_X5B_X25s_X5D_X3A_X25i),
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
    ( &(const binstruction[113]) {  /* code */
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
      0x58180024,  //  0012  LDCONST	R6	K36
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100125,  //  0014  GETMET	R4	R0	K37
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C0026,  //  0016  LDCONST	R7	K38
      0x5820001F,  //  0017  LDCONST	R8	K31
      0x58240022,  //  0018  LDCONST	R9	K34
      0x50280000,  //  0019  LDBOOL	R10	0	0
      0x7C100C00,  //  001A  CALL	R4	6
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x80040800,  //  001C  RET	1	R4
      0xB8124E00,  //  001D  GETNGBL	R4	K39
      0x8C100928,  //  001E  GETMET	R4	R4	K40
      0x7C100200,  //  001F  CALL	R4	1
      0x8C100929,  //  0020  GETMET	R4	R4	K41
      0x88180301,  //  0021  GETMBR	R6	R1	K1
      0x881C0302,  //  0022  GETMBR	R7	R1	K2
      0x7C100600,  //  0023  CALL	R4	3
      0x88140300,  //  0024  GETMBR	R5	R1	K0
      0x8C140B2A,  //  0025  GETMET	R5	R5	K42
      0x7C140200,  //  0026  CALL	R5	1
      0x88140302,  //  0027  GETMBR	R5	R1	K2
      0x40140B03,  //  0028  CONNECT	R5	R5	K3
      0x88180301,  //  0029  GETMBR	R6	R1	K1
      0x94140C05,  //  002A  GETIDX	R5	R6	R5
      0x900E5605,  //  002B  SETMBR	R3	K43	R5
      0x8814092C,  //  002C  GETMBR	R5	R4	K44
      0x20140B1F,  //  002D  NE	R5	R5	K31
      0x78160010,  //  002E  JMPF	R5	#0040
      0xB8160A00,  //  002F  GETNGBL	R5	K5
      0x5818002D,  //  0030  LDCONST	R6	K45
      0x581C0022,  //  0031  LDCONST	R7	K34
      0x7C140400,  //  0032  CALL	R5	2
      0xB8160A00,  //  0033  GETNGBL	R5	K5
      0x58180023,  //  0034  LDCONST	R6	K35
      0x581C0024,  //  0035  LDCONST	R7	K36
      0x7C140400,  //  0036  CALL	R5	2
      0x8C140125,  //  0037  GETMET	R5	R0	K37
      0x5C1C0200,  //  0038  MOVE	R7	R1
      0x58200026,  //  0039  LDCONST	R8	K38
      0x5824001F,  //  003A  LDCONST	R9	K31
      0x58280022,  //  003B  LDCONST	R10	K34
      0x502C0000,  //  003C  LDBOOL	R11	0	0
      0x7C140C00,  //  003D  CALL	R5	6
      0x50140000,  //  003E  LDBOOL	R5	0	0
      0x80040A00,  //  003F  RET	1	R5
      0x8814092F,  //  0040  GETMBR	R5	R4	K47
      0x900E5C05,  //  0041  SETMBR	R3	K46	R5
      0x8814010D,  //  0042  GETMBR	R5	R0	K13
      0x88140B0E,  //  0043  GETMBR	R5	R5	K14
      0x8C140B31,  //  0044  GETMET	R5	R5	K49
      0x7C140200,  //  0045  CALL	R5	1
      0x900E6005,  //  0046  SETMBR	R3	K48	R5
      0xB8160A00,  //  0047  GETNGBL	R5	K5
      0x60180018,  //  0048  GETGBL	R6	G24
      0x581C0032,  //  0049  LDCONST	R7	K50
      0x88200730,  //  004A  GETMBR	R8	R3	K48
      0x88240333,  //  004B  GETMBR	R9	R1	K51
      0x88280334,  //  004C  GETMBR	R10	R1	K52
      0x7C180800,  //  004D  CALL	R6	4
      0x581C0024,  //  004E  LDCONST	R7	K36
      0x7C140400,  //  004F  CALL	R5	2
      0xB8164E00,  //  0050  GETNGBL	R5	K39
      0x8C140B35,  //  0051  GETMET	R5	R5	K53
      0x7C140200,  //  0052  CALL	R5	1
      0x88180936,  //  0053  GETMBR	R6	R4	K54
      0x90166C06,  //  0054  SETMBR	R5	K54	R6
      0x8C180538,  //  0055  GETMET	R6	R2	K56
      0x5422001F,  //  0056  LDINT	R8	32
      0x7C180400,  //  0057  CALL	R6	2
      0x90166E06,  //  0058  SETMBR	R5	K55	R6
      0x88180730,  //  0059  GETMBR	R6	R3	K48
      0x90167206,  //  005A  SETMBR	R5	K57	R6
      0x8818010D,  //  005B  GETMBR	R6	R0	K13
      0x88180D3B,  //  005C  GETMBR	R6	R6	K59
      0x90167406,  //  005D  SETMBR	R5	K58	R6
      0x8818010D,  //  005E  GETMBR	R6	R0	K13
      0x88180D3D,  //  005F  GETMBR	R6	R6	K61
      0x90167806,  //  0060  SETMBR	R5	K60	R6
      0x8C180B3E,  //  0061  GETMET	R6	R5	K62
      0x7C180200,  //  0062  CALL	R6	1
      0x900E7E06,  //  0063  SETMBR	R3	K63	R6
      0x8C1C0340,  //  0064  GETMET	R7	R1	K64
      0x54260020,  //  0065  LDINT	R9	33
      0x50280200,  //  0066  LDBOOL	R10	1	0
      0x7C1C0600,  //  0067  CALL	R7	3
      0x8C200F41,  //  0068  GETMET	R8	R7	K65
      0x5C280C00,  //  0069  MOVE	R10	R6
      0x7C200400,  //  006A  CALL	R8	2
      0x88240142,  //  006B  GETMBR	R9	R0	K66
      0x8C241343,  //  006C  GETMET	R9	R9	K67
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
      0x582C0045,  //  0008  LDCONST	R11	K69
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
    ( &(const binstruction[112]) {  /* code */
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
      0x58140046,  //  000D  LDCONST	R5	K70
      0x58180024,  //  000E  LDCONST	R6	K36
      0x7C100400,  //  000F  CALL	R4	2
      0xB8120A00,  //  0010  GETNGBL	R4	K5
      0x58140023,  //  0011  LDCONST	R5	K35
      0x58180024,  //  0012  LDCONST	R6	K36
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100125,  //  0014  GETMET	R4	R0	K37
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C0026,  //  0016  LDCONST	R7	K38
      0x5820001F,  //  0017  LDCONST	R8	K31
      0x58240045,  //  0018  LDCONST	R9	K69
      0x50280000,  //  0019  LDBOOL	R10	0	0
      0x7C100C00,  //  001A  CALL	R4	6
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x80040800,  //  001C  RET	1	R4
      0xB8124E00,  //  001D  GETNGBL	R4	K39
      0x8C100947,  //  001E  GETMET	R4	R4	K71
      0x7C100200,  //  001F  CALL	R4	1
      0x8C100929,  //  0020  GETMET	R4	R4	K41
      0x88180301,  //  0021  GETMBR	R6	R1	K1
      0x881C0302,  //  0022  GETMBR	R7	R1	K2
      0x7C100600,  //  0023  CALL	R4	3
      0x88140948,  //  0024  GETMBR	R5	R4	K72
      0x8C180549,  //  0025  GETMET	R6	R2	K73
      0x8820010D,  //  0026  GETMBR	R8	R0	K13
      0x8820114A,  //  0027  GETMBR	R8	R8	K74
      0x4C240000,  //  0028  LDNIL	R9
      0x8828010D,  //  0029  GETMBR	R10	R0	K13
      0x8828154B,  //  002A  GETMBR	R10	R10	K75
      0x7C180800,  //  002B  CALL	R6	4
      0x8C1C0538,  //  002C  GETMET	R7	R2	K56
      0x5426001F,  //  002D  LDINT	R9	32
      0x7C1C0400,  //  002E  CALL	R7	2
      0x8C200D4C,  //  002F  GETMET	R8	R6	K76
      0x5C280E00,  //  0030  MOVE	R10	R7
      0x7C200400,  //  0031  CALL	R8	2
      0x8C200D4D,  //  0032  GETMET	R8	R6	K77
      0x5C280A00,  //  0033  MOVE	R10	R5
      0x7C200400,  //  0034  CALL	R8	2
      0x8C20054E,  //  0035  GETMET	R8	R2	K78
      0x7C200200,  //  0036  CALL	R8	1
      0x8C241116,  //  0037  GETMET	R9	R8	K22
      0x602C0015,  //  0038  GETGBL	R11	G21
      0x7C2C0000,  //  0039  CALL	R11	0
      0x8C2C174F,  //  003A  GETMET	R11	R11	K79
      0x88340150,  //  003B  GETMBR	R13	R0	K80
      0x7C2C0400,  //  003C  CALL	R11	2
      0x7C240400,  //  003D  CALL	R9	2
      0x8C241116,  //  003E  GETMET	R9	R8	K22
      0x882C072B,  //  003F  GETMBR	R11	R3	K43
      0x7C240400,  //  0040  CALL	R9	2
      0x8C241116,  //  0041  GETMET	R9	R8	K22
      0x882C073F,  //  0042  GETMBR	R11	R3	K63
      0x7C240400,  //  0043  CALL	R9	2
      0x8C241117,  //  0044  GETMET	R9	R8	K23
      0x7C240200,  //  0045  CALL	R9	1
      0x901A9005,  //  0046  SETMBR	R6	K72	R5
      0x8C280D51,  //  0047  GETMET	R10	R6	K81
      0x5C301200,  //  0048  MOVE	R12	R9
      0x7C280400,  //  0049  CALL	R10	2
      0x8C280D52,  //  004A  GETMET	R10	R6	K82
      0x50300200,  //  004B  LDBOOL	R12	1	0
      0x7C280400,  //  004C  CALL	R10	2
      0xB82A4E00,  //  004D  GETNGBL	R10	K39
      0x8C281553,  //  004E  GETMET	R10	R10	K83
      0x7C280200,  //  004F  CALL	R10	1
      0x882C0D54,  //  0050  GETMBR	R11	R6	K84
      0x902AA80B,  //  0051  SETMBR	R10	K84	R11
      0x882C0D55,  //  0052  GETMBR	R11	R6	K85
      0x902AAA0B,  //  0053  SETMBR	R10	K85	R11
      0x8C2C153E,  //  0054  GETMET	R11	R10	K62
      0x7C2C0200,  //  0055  CALL	R11	1
      0x88300D57,  //  0056  GETMBR	R12	R6	K87
      0x900EAC0C,  //  0057  SETMBR	R3	K86	R12
      0x88300D59,  //  0058  GETMBR	R12	R6	K89
      0x900EB00C,  //  0059  SETMBR	R3	K88	R12
      0x8C300340,  //  005A  GETMET	R12	R1	K64
      0x543A0022,  //  005B  LDINT	R14	35
      0x503C0200,  //  005C  LDBOOL	R15	1	0
      0x7C300600,  //  005D  CALL	R12	3
      0x8C341941,  //  005E  GETMET	R13	R12	K65
      0x5C3C1600,  //  005F  MOVE	R15	R11
      0x7C340400,  //  0060  CALL	R13	2
      0xB83A0A00,  //  0061  GETNGBL	R14	K5
      0x603C0018,  //  0062  GETGBL	R15	G24
      0x5840005A,  //  0063  LDCONST	R16	K90
      0x88440730,  //  0064  GETMBR	R17	R3	K48
      0x8848075B,  //  0065  GETMBR	R18	R3	K91
      0x884C075C,  //  0066  GETMBR	R19	R3	K92
      0x7C3C0800,  //  0067  CALL	R15	4
      0x58400045,  //  0068  LDCONST	R16	K69
      0x7C380400,  //  0069  CALL	R14	2
      0x88380142,  //  006A  GETMBR	R14	R0	K66
      0x8C381D43,  //  006B  GETMET	R14	R14	K67
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
      0x5814005D,  //  000D  LDCONST	R5	K93
      0x58180024,  //  000E  LDCONST	R6	K36
      0x7C100400,  //  000F  CALL	R4	2
      0xB8120A00,  //  0010  GETNGBL	R4	K5
      0x58140023,  //  0011  LDCONST	R5	K35
      0x58180024,  //  0012  LDCONST	R6	K36
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100125,  //  0014  GETMET	R4	R0	K37
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C0026,  //  0016  LDCONST	R7	K38
      0x5820001F,  //  0017  LDCONST	R8	K31
      0x58240045,  //  0018  LDCONST	R9	K69
      0x50280000,  //  0019  LDBOOL	R10	0	0
      0x7C100C00,  //  001A  CALL	R4	6
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x80040800,  //  001C  RET	1	R4
      0xB8124E00,  //  001D  GETNGBL	R4	K39
      0x8C10095E,  //  001E  GETMET	R4	R4	K94
      0x7C100200,  //  001F  CALL	R4	1
      0x8C100929,  //  0020  GETMET	R4	R4	K41
      0x88180301,  //  0021  GETMBR	R6	R1	K1
      0x881C0302,  //  0022  GETMBR	R7	R1	K2
      0x7C100600,  //  0023  CALL	R4	3
      0x88140957,  //  0024  GETMBR	R5	R4	K87
      0x88180756,  //  0025  GETMBR	R6	R3	K86
      0x20180A06,  //  0026  NE	R6	R5	R6
      0x781A0010,  //  0027  JMPF	R6	#0039
      0xB81A0A00,  //  0028  GETNGBL	R6	K5
      0x581C005F,  //  0029  LDCONST	R7	K95
      0x58200024,  //  002A  LDCONST	R8	K36
      0x7C180400,  //  002B  CALL	R6	2
      0xB81A0A00,  //  002C  GETNGBL	R6	K5
      0x581C0023,  //  002D  LDCONST	R7	K35
      0x58200024,  //  002E  LDCONST	R8	K36
      0x7C180400,  //  002F  CALL	R6	2
      0x8C180125,  //  0030  GETMET	R6	R0	K37
      0x5C200200,  //  0031  MOVE	R8	R1
      0x58240026,  //  0032  LDCONST	R9	K38
      0x5828001F,  //  0033  LDCONST	R10	K31
      0x582C0045,  //  0034  LDCONST	R11	K69
      0x50300000,  //  0035  LDBOOL	R12	0	0
      0x7C180C00,  //  0036  CALL	R6	6
      0x50180000,  //  0037  LDBOOL	R6	0	0
      0x80040C00,  //  0038  RET	1	R6
      0xB81A1400,  //  0039  GETNGBL	R6	K10
      0x8C180D60,  //  003A  GETMET	R6	R6	K96
      0x7C180200,  //  003B  CALL	R6	1
      0x8C1C0561,  //  003C  GETMET	R7	R2	K97
      0x7C1C0200,  //  003D  CALL	R7	1
      0x8C1C0F62,  //  003E  GETMET	R7	R7	K98
      0x88240758,  //  003F  GETMBR	R9	R3	K88
      0x60280015,  //  0040  GETGBL	R10	G21
      0x7C280000,  //  0041  CALL	R10	0
      0x602C0015,  //  0042  GETGBL	R11	G21
      0x7C2C0000,  //  0043  CALL	R11	0
      0x8C2C174F,  //  0044  GETMET	R11	R11	K79
      0x88340163,  //  0045  GETMBR	R13	R0	K99
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
      0x8C2C0125,  //  0054  GETMET	R11	R0	K37
      0x5C340200,  //  0055  MOVE	R13	R1
      0x5838001F,  //  0056  LDCONST	R14	K31
      0x583C001F,  //  0057  LDCONST	R15	K31
      0x5840001F,  //  0058  LDCONST	R16	K31
      0x50440000,  //  0059  LDBOOL	R17	0	0
      0x7C2C0C00,  //  005A  CALL	R11	6
      0x8C2C0164,  //  005B  GETMET	R11	R0	K100
      0x88340730,  //  005C  GETMBR	R13	R3	K48
      0x8838072E,  //  005D  GETMBR	R14	R3	K46
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
    ( &(const binstruction[280]) {  /* code */
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
      0x58140065,  //  000D  LDCONST	R5	K101
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C0C0125,  //  000F  GETMET	R3	R0	K37
      0x5C140200,  //  0010  MOVE	R5	R1
      0x58180066,  //  0011  LDCONST	R6	K102
      0x581C001F,  //  0012  LDCONST	R7	K31
      0x58200067,  //  0013  LDCONST	R8	K103
      0x50240000,  //  0014  LDBOOL	R9	0	0
      0x7C0C0C00,  //  0015  CALL	R3	6
      0x500C0000,  //  0016  LDBOOL	R3	0	0
      0x80040600,  //  0017  RET	1	R3
      0x880C0300,  //  0018  GETMBR	R3	R1	K0
      0xB8124E00,  //  0019  GETNGBL	R4	K39
      0x8C100968,  //  001A  GETMET	R4	R4	K104
      0x7C100200,  //  001B  CALL	R4	1
      0x8C100929,  //  001C  GETMET	R4	R4	K41
      0x88180301,  //  001D  GETMBR	R6	R1	K1
      0x881C0302,  //  001E  GETMBR	R7	R1	K2
      0x7C100600,  //  001F  CALL	R4	3
      0x8C14054E,  //  0020  GETMET	R5	R2	K78
      0x7C140200,  //  0021  CALL	R5	1
      0x8C140B16,  //  0022  GETMET	R5	R5	K22
      0x881C072B,  //  0023  GETMBR	R7	R3	K43
      0x7C140400,  //  0024  CALL	R5	2
      0x8C140B16,  //  0025  GETMET	R5	R5	K22
      0x881C073F,  //  0026  GETMBR	R7	R3	K63
      0x7C140400,  //  0027  CALL	R5	2
      0x8C140B17,  //  0028  GETMET	R5	R5	K23
      0x7C140200,  //  0029  CALL	R5	1
      0x60180015,  //  002A  GETGBL	R6	G21
      0x7C180000,  //  002B  CALL	R6	0
      0x8C180D4F,  //  002C  GETMET	R6	R6	K79
      0x88200169,  //  002D  GETMBR	R8	R0	K105
      0x7C180400,  //  002E  CALL	R6	2
      0x8C1C0561,  //  002F  GETMET	R7	R2	K97
      0x7C1C0200,  //  0030  CALL	R7	1
      0x8C1C0F62,  //  0031  GETMET	R7	R7	K98
      0x8824076A,  //  0032  GETMBR	R9	R3	K106
      0x8C280714,  //  0033  GETMET	R10	R3	K20
      0x7C280200,  //  0034  CALL	R10	1
      0x00281405,  //  0035  ADD	R10	R10	R5
      0x5C2C0C00,  //  0036  MOVE	R11	R6
      0x5432000F,  //  0037  LDINT	R12	16
      0x7C1C0A00,  //  0038  CALL	R7	5
      0x5421FFEE,  //  0039  LDINT	R8	-17
      0x40223E08,  //  003A  CONNECT	R8	K31	R8
      0x8824096B,  //  003B  GETMBR	R9	R4	K107
      0x94201208,  //  003C  GETIDX	R8	R9	R8
      0x5429FFEF,  //  003D  LDINT	R10	-16
      0x4028156C,  //  003E  CONNECT	R10	R10	K108
      0x882C096B,  //  003F  GETMBR	R11	R4	K107
      0x9424160A,  //  0040  GETIDX	R9	R11	R10
      0x8C30056D,  //  0041  GETMET	R12	R2	K109
      0x5C380E00,  //  0042  MOVE	R14	R7
      0x603C0015,  //  0043  GETGBL	R15	G21
      0x7C3C0000,  //  0044  CALL	R15	0
      0x8C3C1F4F,  //  0045  GETMET	R15	R15	K79
      0x8844016E,  //  0046  GETMBR	R17	R0	K110
      0x7C3C0400,  //  0047  CALL	R15	2
      0x60400015,  //  0048  GETGBL	R16	G21
      0x7C400000,  //  0049  CALL	R16	0
      0x6044000C,  //  004A  GETGBL	R17	G12
      0x5C481000,  //  004B  MOVE	R18	R8
      0x7C440200,  //  004C  CALL	R17	1
      0x544A000F,  //  004D  LDINT	R18	16
      0x7C300C00,  //  004E  CALL	R12	6
      0x5C281800,  //  004F  MOVE	R10	R12
      0x8C30156F,  //  0050  GETMET	R12	R10	K111
      0x5C381000,  //  0051  MOVE	R14	R8
      0x7C300400,  //  0052  CALL	R12	2
      0x5C2C1800,  //  0053  MOVE	R11	R12
      0x8C301570,  //  0054  GETMET	R12	R10	K112
      0x7C300200,  //  0055  CALL	R12	1
      0x20341809,  //  0056  NE	R13	R12	R9
      0x78360010,  //  0057  JMPF	R13	#0069
      0xB8360A00,  //  0058  GETNGBL	R13	K5
      0x58380071,  //  0059  LDCONST	R14	K113
      0x583C0065,  //  005A  LDCONST	R15	K101
      0x7C340400,  //  005B  CALL	R13	2
      0xB8360A00,  //  005C  GETNGBL	R13	K5
      0x58380023,  //  005D  LDCONST	R14	K35
      0x583C0065,  //  005E  LDCONST	R15	K101
      0x7C340400,  //  005F  CALL	R13	2
      0x8C340125,  //  0060  GETMET	R13	R0	K37
      0x5C3C0200,  //  0061  MOVE	R15	R1
      0x58400066,  //  0062  LDCONST	R16	K102
      0x5844001F,  //  0063  LDCONST	R17	K31
      0x58480067,  //  0064  LDCONST	R18	K103
      0x504C0000,  //  0065  LDBOOL	R19	0	0
      0x7C340C00,  //  0066  CALL	R13	6
      0x50340000,  //  0067  LDBOOL	R13	0	0
      0x80041A00,  //  0068  RET	1	R13
      0xB8364E00,  //  0069  GETNGBL	R13	K39
      0x88341B72,  //  006A  GETMBR	R13	R13	K114
      0x8C341B29,  //  006B  GETMET	R13	R13	K41
      0x5C3C1600,  //  006C  MOVE	R15	R11
      0x7C340400,  //  006D  CALL	R13	2
      0x8C381B73,  //  006E  GETMET	R14	R13	K115
      0x58400066,  //  006F  LDCONST	R16	K102
      0x7C380400,  //  0070  CALL	R14	2
      0x8C3C1B73,  //  0071  GETMET	R15	R13	K115
      0x58440067,  //  0072  LDCONST	R17	K103
      0x7C3C0400,  //  0073  CALL	R15	2
      0x8C401B73,  //  0074  GETMET	R16	R13	K115
      0x58480065,  //  0075  LDCONST	R18	K101
      0x7C400400,  //  0076  CALL	R16	2
      0xB8464E00,  //  0077  GETNGBL	R17	K39
      0x88442372,  //  0078  GETMBR	R17	R17	K114
      0x8C442329,  //  0079  GETMET	R17	R17	K41
      0x5C4C1C00,  //  007A  MOVE	R19	R14
      0x7C440400,  //  007B  CALL	R17	2
      0x8C482373,  //  007C  GETMET	R18	R17	K115
      0x54520008,  //  007D  LDINT	R20	9
      0x7C480400,  //  007E  CALL	R18	2
      0x8C4C2374,  //  007F  GETMET	R19	R17	K116
      0x54560005,  //  0080  LDINT	R21	6
      0x7C4C0400,  //  0081  CALL	R19	2
      0x8C502773,  //  0082  GETMET	R20	R19	K115
      0x545A0010,  //  0083  LDINT	R22	17
      0x7C500400,  //  0084  CALL	R20	2
      0x60540004,  //  0085  GETGBL	R21	G4
      0x5C582800,  //  0086  MOVE	R22	R20
      0x7C540200,  //  0087  CALL	R21	1
      0x1C542B75,  //  0088  EQ	R21	R21	K117
      0x78560007,  //  0089  JMPF	R21	#0092
      0xB856EE00,  //  008A  GETNGBL	R21	K119
      0x8C542B78,  //  008B  GETMET	R21	R21	K120
      0x5C5C2800,  //  008C  MOVE	R23	R20
      0x7C540400,  //  008D  CALL	R21	2
      0x8C542B79,  //  008E  GETMET	R21	R21	K121
      0x7C540200,  //  008F  CALL	R21	1
      0x900EEC15,  //  0090  SETMBR	R3	K118	R21
      0x70020002,  //  0091  JMP		#0095
      0x8C542979,  //  0092  GETMET	R21	R20	K121
      0x7C540200,  //  0093  CALL	R21	1
      0x900EEC15,  //  0094  SETMBR	R3	K118	R21
      0xB8564E00,  //  0095  GETNGBL	R21	K39
      0x88542B72,  //  0096  GETMBR	R21	R21	K114
      0x8C542B7A,  //  0097  GETMET	R21	R21	K122
      0x7C540200,  //  0098  CALL	R21	1
      0x8C582B7B,  //  0099  GETMET	R22	R21	K123
      0x58600066,  //  009A  LDCONST	R24	K102
      0xB8664E00,  //  009B  GETNGBL	R25	K39
      0x88643372,  //  009C  GETMBR	R25	R25	K114
      0x8864337C,  //  009D  GETMBR	R25	R25	K124
      0x5C681C00,  //  009E  MOVE	R26	R14
      0x7C580800,  //  009F  CALL	R22	4
      0x8C582B7B,  //  00A0  GETMET	R22	R21	K123
      0x58600067,  //  00A1  LDCONST	R24	K103
      0xB8664E00,  //  00A2  GETNGBL	R25	K39
      0x88643372,  //  00A3  GETMBR	R25	R25	K114
      0x8864337C,  //  00A4  GETMBR	R25	R25	K124
      0x5C681E00,  //  00A5  MOVE	R26	R15
      0x7C580800,  //  00A6  CALL	R22	4
      0x8C582B7B,  //  00A7  GETMET	R22	R21	K123
      0x58600065,  //  00A8  LDCONST	R24	K101
      0xB8664E00,  //  00A9  GETNGBL	R25	K39
      0x88643372,  //  00AA  GETMBR	R25	R25	K114
      0x8864337C,  //  00AB  GETMBR	R25	R25	K124
      0x8868077D,  //  00AC  GETMBR	R26	R3	K125
      0x7C580800,  //  00AD  CALL	R22	4
      0x8C582B7B,  //  00AE  GETMET	R22	R21	K123
      0x54620003,  //  00AF  LDINT	R24	4
      0xB8664E00,  //  00B0  GETNGBL	R25	K39
      0x88643372,  //  00B1  GETMBR	R25	R25	K114
      0x8864337C,  //  00B2  GETMBR	R25	R25	K124
      0x8868077E,  //  00B3  GETMBR	R26	R3	K126
      0x7C580800,  //  00B4  CALL	R22	4
      0x8C582B3E,  //  00B5  GETMET	R22	R21	K62
      0x7C580200,  //  00B6  CALL	R22	1
      0x8C5C057F,  //  00B7  GETMET	R23	R2	K127
      0x7C5C0200,  //  00B8  CALL	R23	1
      0x8C5C2F80,  //  00B9  GETMET	R23	R23	K128
      0x5C642400,  //  00BA  MOVE	R25	R18
      0x5C682C00,  //  00BB  MOVE	R26	R22
      0x5C6C2000,  //  00BC  MOVE	R27	R16
      0x7C5C0800,  //  00BD  CALL	R23	4
      0x5C602E00,  //  00BE  MOVE	R24	R23
      0x74620008,  //  00BF  JMPT	R24	#00C9
      0xB8620A00,  //  00C0  GETNGBL	R24	K5
      0x58640081,  //  00C1  LDCONST	R25	K129
      0x58680067,  //  00C2  LDCONST	R26	K103
      0x7C600400,  //  00C3  CALL	R24	2
      0xB8620A00,  //  00C4  GETNGBL	R24	K5
      0x58640082,  //  00C5  LDCONST	R25	K130
      0x58680067,  //  00C6  LDCONST	R26	K103
      0x7C600400,  //  00C7  CALL	R24	2
      0x70020003,  //  00C8  JMP		#00CD
      0xB8620A00,  //  00C9  GETNGBL	R24	K5
      0x58640083,  //  00CA  LDCONST	R25	K131
      0x58680065,  //  00CB  LDCONST	R26	K101
      0x7C600400,  //  00CC  CALL	R24	2
      0x8C60054E,  //  00CD  GETMET	R24	R2	K78
      0x7C600200,  //  00CE  CALL	R24	1
      0x8C603116,  //  00CF  GETMET	R24	R24	K22
      0x8868072B,  //  00D0  GETMBR	R26	R3	K43
      0x7C600400,  //  00D1  CALL	R24	2
      0x8C603116,  //  00D2  GETMET	R24	R24	K22
      0x8868073F,  //  00D3  GETMBR	R26	R3	K63
      0x7C600400,  //  00D4  CALL	R24	2
      0x8C603116,  //  00D5  GETMET	R24	R24	K22
      0x88680984,  //  00D6  GETMBR	R26	R4	K132
      0x7C600400,  //  00D7  CALL	R24	2
      0x8C603117,  //  00D8  GETMET	R24	R24	K23
      0x7C600200,  //  00D9  CALL	R24	1
      0x5C143000,  //  00DA  MOVE	R5	R24
      0x4C600000,  //  00DB  LDNIL	R24
      0x900E5618,  //  00DC  SETMBR	R3	K43	R24
      0x4C600000,  //  00DD  LDNIL	R24
      0x900E7E18,  //  00DE  SETMBR	R3	K63	R24
      0x8C600561,  //  00DF  GETMET	R24	R2	K97
      0x7C600200,  //  00E0  CALL	R24	1
      0x8C603162,  //  00E1  GETMET	R24	R24	K98
      0x8868076A,  //  00E2  GETMBR	R26	R3	K106
      0x8C6C0714,  //  00E3  GETMET	R27	R3	K20
      0x7C6C0200,  //  00E4  CALL	R27	1
      0x006C3605,  //  00E5  ADD	R27	R27	R5
      0x60700015,  //  00E6  GETGBL	R28	G21
      0x7C700000,  //  00E7  CALL	R28	0
      0x8C70394F,  //  00E8  GETMET	R28	R28	K79
      0x88780163,  //  00E9  GETMBR	R30	R0	K99
      0x7C700400,  //  00EA  CALL	R28	2
      0x5476002F,  //  00EB  LDINT	R29	48
      0x7C600A00,  //  00EC  CALL	R24	5
      0x5466000E,  //  00ED  LDINT	R25	15
      0x40663E19,  //  00EE  CONNECT	R25	K31	R25
      0x94643019,  //  00EF  GETIDX	R25	R24	R25
      0x546A000F,  //  00F0  LDINT	R26	16
      0x546E001E,  //  00F1  LDINT	R27	31
      0x4068341B,  //  00F2  CONNECT	R26	R26	R27
      0x9468301A,  //  00F3  GETIDX	R26	R24	R26
      0x546E001F,  //  00F4  LDINT	R27	32
      0x5472002E,  //  00F5  LDINT	R28	47
      0x406C361C,  //  00F6  CONNECT	R27	R27	R28
      0x946C301B,  //  00F7  GETIDX	R27	R24	R27
      0xB8721400,  //  00F8  GETNGBL	R28	K10
      0x8C703960,  //  00F9  GETMET	R28	R28	K96
      0x7C700200,  //  00FA  CALL	R28	1
      0x8C740125,  //  00FB  GETMET	R29	R0	K37
      0x5C7C0200,  //  00FC  MOVE	R31	R1
      0x5880001F,  //  00FD  LDCONST	R32	K31
      0x5884001F,  //  00FE  LDCONST	R33	K31
      0x5888001F,  //  00FF  LDCONST	R34	K31
      0x508C0200,  //  0100  LDBOOL	R35	1	0
      0x7C740C00,  //  0101  CALL	R29	6
      0x8C740785,  //  0102  GETMET	R29	R3	K133
      0x7C740200,  //  0103  CALL	R29	1
      0x8C74071C,  //  0104  GETMET	R29	R3	K28
      0x5C7C3200,  //  0105  MOVE	R31	R25
      0x5C803400,  //  0106  MOVE	R32	R26
      0x5C843600,  //  0107  MOVE	R33	R27
      0x5C883800,  //  0108  MOVE	R34	R28
      0x7C740A00,  //  0109  CALL	R29	5
      0x900F0D1F,  //  010A  SETMBR	R3	K134	K31
      0x8C740787,  //  010B  GETMET	R29	R3	K135
      0x7C740200,  //  010C  CALL	R29	1
      0x8C740788,  //  010D  GETMET	R29	R3	K136
      0x507C0200,  //  010E  LDBOOL	R31	1	0
      0x7C740400,  //  010F  CALL	R29	2
      0x8C740789,  //  0110  GETMET	R29	R3	K137
      0x7C740200,  //  0111  CALL	R29	1
      0x8C74078A,  //  0112  GETMET	R29	R3	K138
      0x7C740200,  //  0113  CALL	R29	1
      0x8C74078B,  //  0114  GETMET	R29	R3	K139
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
    ( &(const binstruction[88]) {  /* code */
      0x8808010D,  //  0000  GETMBR	R2	R0	K13
      0x8C08058C,  //  0001  GETMET	R2	R2	K140
      0x7C080200,  //  0002  CALL	R2	1
      0x740A000D,  //  0003  JMPT	R2	#0012
      0x8808031D,  //  0004  GETMBR	R2	R1	K29
      0x540E001F,  //  0005  LDINT	R3	32
      0x28080403,  //  0006  GE	R2	R2	R3
      0x780A0009,  //  0007  JMPF	R2	#0012
      0x8808031D,  //  0008  GETMBR	R2	R1	K29
      0x540E0023,  //  0009  LDINT	R3	36
      0x18080403,  //  000A  LE	R2	R2	R3
      0x780A0005,  //  000B  JMPF	R2	#0012
      0xB80A0A00,  //  000C  GETNGBL	R2	K5
      0x580C008D,  //  000D  LDCONST	R3	K141
      0x58100022,  //  000E  LDCONST	R4	K34
      0x7C080400,  //  000F  CALL	R2	2
      0x50080000,  //  0010  LDBOOL	R2	0	0
      0x80040400,  //  0011  RET	1	R2
      0x8808031D,  //  0012  GETMBR	R2	R1	K29
      0x540E000F,  //  0013  LDINT	R3	16
      0x1C080403,  //  0014  EQ	R2	R2	R3
      0x780A0000,  //  0015  JMPF	R2	#0017
      0x7002003E,  //  0016  JMP		#0056
      0x8808031D,  //  0017  GETMBR	R2	R1	K29
      0x540E001F,  //  0018  LDINT	R3	32
      0x1C080403,  //  0019  EQ	R2	R2	R3
      0x780A0004,  //  001A  JMPF	R2	#0020
      0x8C08018E,  //  001B  GETMET	R2	R0	K142
      0x5C100200,  //  001C  MOVE	R4	R1
      0x7C080400,  //  001D  CALL	R2	2
      0x80040400,  //  001E  RET	1	R2
      0x70020035,  //  001F  JMP		#0056
      0x8808031D,  //  0020  GETMBR	R2	R1	K29
      0x540E0021,  //  0021  LDINT	R3	34
      0x1C080403,  //  0022  EQ	R2	R2	R3
      0x780A0004,  //  0023  JMPF	R2	#0029
      0x8C08018F,  //  0024  GETMET	R2	R0	K143
      0x5C100200,  //  0025  MOVE	R4	R1
      0x7C080400,  //  0026  CALL	R2	2
      0x80040400,  //  0027  RET	1	R2
      0x7002002C,  //  0028  JMP		#0056
      0x8808031D,  //  0029  GETMBR	R2	R1	K29
      0x540E0023,  //  002A  LDINT	R3	36
      0x1C080403,  //  002B  EQ	R2	R2	R3
      0x780A0004,  //  002C  JMPF	R2	#0032
      0x8C080190,  //  002D  GETMET	R2	R0	K144
      0x5C100200,  //  002E  MOVE	R4	R1
      0x7C080400,  //  002F  CALL	R2	2
      0x80040400,  //  0030  RET	1	R2
      0x70020023,  //  0031  JMP		#0056
      0x8808031D,  //  0032  GETMBR	R2	R1	K29
      0x540E002F,  //  0033  LDINT	R3	48
      0x1C080403,  //  0034  EQ	R2	R2	R3
      0x780A0004,  //  0035  JMPF	R2	#003B
      0x8C080191,  //  0036  GETMET	R2	R0	K145
      0x5C100200,  //  0037  MOVE	R4	R1
      0x7C080400,  //  0038  CALL	R2	2
      0x80040400,  //  0039  RET	1	R2
      0x7002001A,  //  003A  JMP		#0056
      0x8808031D,  //  003B  GETMBR	R2	R1	K29
      0x540E0031,  //  003C  LDINT	R3	50
      0x1C080403,  //  003D  EQ	R2	R2	R3
      0x780A0004,  //  003E  JMPF	R2	#0044
      0x8C080192,  //  003F  GETMET	R2	R0	K146
      0x5C100200,  //  0040  MOVE	R4	R1
      0x7C080400,  //  0041  CALL	R2	2
      0x80040400,  //  0042  RET	1	R2
      0x70020011,  //  0043  JMP		#0056
      0x8808031D,  //  0044  GETMBR	R2	R1	K29
      0x540E003F,  //  0045  LDINT	R3	64
      0x1C080403,  //  0046  EQ	R2	R2	R3
      0x780A0004,  //  0047  JMPF	R2	#004D
      0x8C080193,  //  0048  GETMET	R2	R0	K147
      0x5C100200,  //  0049  MOVE	R4	R1
      0x7C080400,  //  004A  CALL	R2	2
      0x80040400,  //  004B  RET	1	R2
      0x70020008,  //  004C  JMP		#0056
      0xB80A0A00,  //  004D  GETNGBL	R2	K5
      0x600C0018,  //  004E  GETGBL	R3	G24
      0x58100094,  //  004F  LDCONST	R4	K148
      0x8814031D,  //  0050  GETMBR	R5	R1	K29
      0x7C0C0400,  //  0051  CALL	R3	2
      0x58100022,  //  0052  LDCONST	R4	K34
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
    ( &(const binstruction[497]) {  /* code */
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
      0x58180024,  //  000E  LDCONST	R6	K36
      0x7C100400,  //  000F  CALL	R4	2
      0x8C100125,  //  0010  GETMET	R4	R0	K37
      0x5C180200,  //  0011  MOVE	R6	R1
      0x581C0026,  //  0012  LDCONST	R7	K38
      0x5820001F,  //  0013  LDCONST	R8	K31
      0x58240045,  //  0014  LDCONST	R9	K69
      0x50280000,  //  0015  LDBOOL	R10	0	0
      0x7C100C00,  //  0016  CALL	R4	6
      0x50100000,  //  0017  LDBOOL	R4	0	0
      0x80040800,  //  0018  RET	1	R4
      0xB8124E00,  //  0019  GETNGBL	R4	K39
      0x8C100995,  //  001A  GETMET	R4	R4	K149
      0x7C100200,  //  001B  CALL	R4	1
      0x8C100929,  //  001C  GETMET	R4	R4	K41
      0x88180301,  //  001D  GETMBR	R6	R1	K1
      0x881C0302,  //  001E  GETMBR	R7	R1	K2
      0x7C100600,  //  001F  CALL	R4	3
      0x88140996,  //  0020  GETMBR	R5	R4	K150
      0x900EFA05,  //  0021  SETMBR	R3	K125	R5
      0x88140997,  //  0022  GETMBR	R5	R4	K151
      0x4C180000,  //  0023  LDNIL	R6
      0x20140A06,  //  0024  NE	R5	R5	R6
      0x78160003,  //  0025  JMPF	R5	#002A
      0x88140998,  //  0026  GETMBR	R5	R4	K152
      0x4C180000,  //  0027  LDNIL	R6
      0x20140A06,  //  0028  NE	R5	R5	R6
      0x74160000,  //  0029  JMPT	R5	#002B
      0x50140001,  //  002A  LDBOOL	R5	0	1
      0x50140200,  //  002B  LDBOOL	R5	1	0
      0x4C180000,  //  002C  LDNIL	R6
      0x78160014,  //  002D  JMPF	R5	#0043
      0x881C010D,  //  002E  GETMBR	R7	R0	K13
      0x881C0F0E,  //  002F  GETMBR	R7	R7	K14
      0x8C1C0F99,  //  0030  GETMET	R7	R7	K153
      0x88240997,  //  0031  GETMBR	R9	R4	K151
      0x7C1C0400,  //  0032  CALL	R7	2
      0x5C180E00,  //  0033  MOVE	R6	R7
      0x4C1C0000,  //  0034  LDNIL	R7
      0x1C1C0C07,  //  0035  EQ	R7	R6	R7
      0x741E0003,  //  0036  JMPT	R7	#003B
      0x881C0D9A,  //  0037  GETMBR	R7	R6	K154
      0x4C200000,  //  0038  LDNIL	R8
      0x1C1C0E08,  //  0039  EQ	R7	R7	R8
      0x781E0007,  //  003A  JMPF	R7	#0043
      0xB81E0A00,  //  003B  GETNGBL	R7	K5
      0x60200018,  //  003C  GETGBL	R8	G24
      0x5824009B,  //  003D  LDCONST	R9	K155
      0x88280997,  //  003E  GETMBR	R10	R4	K151
      0x7C200400,  //  003F  CALL	R8	2
      0x58240024,  //  0040  LDCONST	R9	K36
      0x7C1C0400,  //  0041  CALL	R7	2
      0x50140000,  //  0042  LDBOOL	R5	0	0
      0x781600C3,  //  0043  JMPF	R5	#0108
      0xB81E0A00,  //  0044  GETNGBL	R7	K5
      0x5820009C,  //  0045  LDCONST	R8	K156
      0x58240024,  //  0046  LDCONST	R9	K36
      0x7C1C0400,  //  0047  CALL	R7	2
      0x881C0936,  //  0048  GETMBR	R7	R4	K54
      0x88200997,  //  0049  GETMBR	R8	R4	K151
      0x001C0E08,  //  004A  ADD	R7	R7	R8
      0x60200015,  //  004B  GETGBL	R8	G21
      0x7C200000,  //  004C  CALL	R8	0
      0x8C20114F,  //  004D  GETMET	R8	R8	K79
      0x5828009D,  //  004E  LDCONST	R10	K157
      0x7C200400,  //  004F  CALL	R8	2
      0x8C240561,  //  0050  GETMET	R9	R2	K97
      0x7C240200,  //  0051  CALL	R9	1
      0x8C241362,  //  0052  GETMET	R9	R9	K98
      0x882C0D6A,  //  0053  GETMBR	R11	R6	K106
      0x5C300E00,  //  0054  MOVE	R12	R7
      0x5C341000,  //  0055  MOVE	R13	R8
      0x543A000F,  //  0056  LDINT	R14	16
      0x7C240A00,  //  0057  CALL	R9	5
      0x60280015,  //  0058  GETGBL	R10	G21
      0x7C280000,  //  0059  CALL	R10	0
      0x8C28154F,  //  005A  GETMET	R10	R10	K79
      0x5830009E,  //  005B  LDCONST	R12	K158
      0x7C280400,  //  005C  CALL	R10	2
      0x542DFFEE,  //  005D  LDINT	R11	-17
      0x402E3E0B,  //  005E  CONNECT	R11	K31	R11
      0x88300998,  //  005F  GETMBR	R12	R4	K152
      0x942C180B,  //  0060  GETIDX	R11	R12	R11
      0x5435FFEF,  //  0061  LDINT	R13	-16
      0x40341B03,  //  0062  CONNECT	R13	R13	K3
      0x88380998,  //  0063  GETMBR	R14	R4	K152
      0x94301C0D,  //  0064  GETIDX	R12	R14	R13
      0x8C3C056D,  //  0065  GETMET	R15	R2	K109
      0x5C441200,  //  0066  MOVE	R17	R9
      0x5C481400,  //  0067  MOVE	R18	R10
      0x604C0015,  //  0068  GETGBL	R19	G21
      0x7C4C0000,  //  0069  CALL	R19	0
      0x6050000C,  //  006A  GETGBL	R20	G12
      0x5C541600,  //  006B  MOVE	R21	R11
      0x7C500200,  //  006C  CALL	R20	1
      0x5456000F,  //  006D  LDINT	R21	16
      0x7C3C0C00,  //  006E  CALL	R15	6
      0x5C341E00,  //  006F  MOVE	R13	R15
      0x8C3C1B6F,  //  0070  GETMET	R15	R13	K111
      0x5C441600,  //  0071  MOVE	R17	R11
      0x7C3C0400,  //  0072  CALL	R15	2
      0x5C381E00,  //  0073  MOVE	R14	R15
      0x8C3C1B70,  //  0074  GETMET	R15	R13	K112
      0x7C3C0200,  //  0075  CALL	R15	1
      0x1C40180F,  //  0076  EQ	R16	R12	R15
      0x7842008A,  //  0077  JMPF	R16	#0103
      0x88400D9A,  //  0078  GETMBR	R16	R6	K154
      0x900F3410,  //  0079  SETMBR	R3	K154	R16
      0x884003A0,  //  007A  GETMBR	R16	R1	K160
      0x900F3E10,  //  007B  SETMBR	R3	K159	R16
      0x8C4007A1,  //  007C  GETMET	R16	R3	K161
      0x7C400200,  //  007D  CALL	R16	1
      0x8840092F,  //  007E  GETMBR	R16	R4	K47
      0x900E5C10,  //  007F  SETMBR	R3	K46	R16
      0x8840010D,  //  0080  GETMBR	R16	R0	K13
      0x8840210E,  //  0081  GETMBR	R16	R16	K14
      0x8C402131,  //  0082  GETMET	R16	R16	K49
      0x7C400200,  //  0083  CALL	R16	1
      0x900E6010,  //  0084  SETMBR	R3	K48	R16
      0xB8420A00,  //  0085  GETNGBL	R16	K5
      0x60440018,  //  0086  GETGBL	R17	G24
      0x58480032,  //  0087  LDCONST	R18	K50
      0x884C0730,  //  0088  GETMBR	R19	R3	K48
      0x88500333,  //  0089  GETMBR	R20	R1	K51
      0x88540334,  //  008A  GETMBR	R21	R1	K52
      0x7C440800,  //  008B  CALL	R17	4
      0x58480024,  //  008C  LDCONST	R18	K36
      0x7C400400,  //  008D  CALL	R16	2
      0x88400D6A,  //  008E  GETMBR	R16	R6	K106
      0x900ED410,  //  008F  SETMBR	R3	K106	R16
      0x8C400538,  //  0090  GETMET	R16	R2	K56
      0x544A000F,  //  0091  LDINT	R18	16
      0x7C400400,  //  0092  CALL	R16	2
      0x900F4410,  //  0093  SETMBR	R3	K162	R16
      0x60400015,  //  0094  GETGBL	R16	G21
      0x7C400000,  //  0095  CALL	R16	0
      0x8C40214F,  //  0096  GETMET	R16	R16	K79
      0x584800A3,  //  0097  LDCONST	R18	K163
      0x7C400400,  //  0098  CALL	R16	2
      0x88440936,  //  0099  GETMBR	R17	R4	K54
      0x884807A2,  //  009A  GETMBR	R18	R3	K162
      0x00442212,  //  009B  ADD	R17	R17	R18
      0x8C480561,  //  009C  GETMET	R18	R2	K97
      0x7C480200,  //  009D  CALL	R18	1
      0x8C482562,  //  009E  GETMET	R18	R18	K98
      0x8850076A,  //  009F  GETMBR	R20	R3	K106
      0x5C542200,  //  00A0  MOVE	R21	R17
      0x5C582000,  //  00A1  MOVE	R22	R16
      0x545E000F,  //  00A2  LDINT	R23	16
      0x7C480A00,  //  00A3  CALL	R18	5
      0x8C4C056D,  //  00A4  GETMET	R19	R2	K109
      0x5C542400,  //  00A5  MOVE	R21	R18
      0x60580015,  //  00A6  GETGBL	R22	G21
      0x7C580000,  //  00A7  CALL	R22	0
      0x8C582D4F,  //  00A8  GETMET	R22	R22	K79
      0x586000A4,  //  00A9  LDCONST	R24	K164
      0x7C580400,  //  00AA  CALL	R22	2
      0x605C0015,  //  00AB  GETGBL	R23	G21
      0x7C5C0000,  //  00AC  CALL	R23	0
      0x5860001F,  //  00AD  LDCONST	R24	K31
      0x5466000F,  //  00AE  LDINT	R25	16
      0x7C4C0C00,  //  00AF  CALL	R19	6
      0x8C502770,  //  00B0  GETMET	R20	R19	K112
      0x7C500200,  //  00B1  CALL	R20	1
      0xB8564E00,  //  00B2  GETNGBL	R21	K39
      0x8C542BA5,  //  00B3  GETMET	R21	R21	K165
      0x7C540200,  //  00B4  CALL	R21	1
      0x885807A2,  //  00B5  GETMBR	R22	R3	K162
      0x90572E16,  //  00B6  SETMBR	R21	K151	R22
      0x90574C14,  //  00B7  SETMBR	R21	K166	R20
      0x88580730,  //  00B8  GETMBR	R22	R3	K48
      0x90574E16,  //  00B9  SETMBR	R21	K167	R22
      0x88580936,  //  00BA  GETMBR	R22	R4	K54
      0x885C0DA2,  //  00BB  GETMBR	R23	R6	K162
      0x00582C17,  //  00BC  ADD	R22	R22	R23
      0x8C5C0561,  //  00BD  GETMET	R23	R2	K97
      0x7C5C0200,  //  00BE  CALL	R23	1
      0x8C5C2F62,  //  00BF  GETMET	R23	R23	K98
      0x8864076A,  //  00C0  GETMBR	R25	R3	K106
      0x5C682C00,  //  00C1  MOVE	R26	R22
      0x606C0015,  //  00C2  GETGBL	R27	G21
      0x7C6C0000,  //  00C3  CALL	R27	0
      0x8C6C374F,  //  00C4  GETMET	R27	R27	K79
      0x587400A8,  //  00C5  LDCONST	R29	K168
      0x7C6C0400,  //  00C6  CALL	R27	2
      0x5472002F,  //  00C7  LDINT	R28	48
      0x7C5C0A00,  //  00C8  CALL	R23	5
      0x5462000E,  //  00C9  LDINT	R24	15
      0x40623E18,  //  00CA  CONNECT	R24	K31	R24
      0x94602E18,  //  00CB  GETIDX	R24	R23	R24
      0x5466000F,  //  00CC  LDINT	R25	16
      0x546A001E,  //  00CD  LDINT	R26	31
      0x4064321A,  //  00CE  CONNECT	R25	R25	R26
      0x94642E19,  //  00CF  GETIDX	R25	R23	R25
      0x546A001F,  //  00D0  LDINT	R26	32
      0x546E002E,  //  00D1  LDINT	R27	47
      0x4068341B,  //  00D2  CONNECT	R26	R26	R27
      0x94682E1A,  //  00D3  GETIDX	R26	R23	R26
      0xB86E1400,  //  00D4  GETNGBL	R27	K10
      0x8C6C3760,  //  00D5  GETMET	R27	R27	K96
      0x7C6C0200,  //  00D6  CALL	R27	1
      0x8C702B3E,  //  00D7  GETMET	R28	R21	K62
      0x7C700200,  //  00D8  CALL	R28	1
      0x4C740000,  //  00D9  LDNIL	R29
      0x900E561D,  //  00DA  SETMBR	R3	K43	R29
      0x8C740340,  //  00DB  GETMET	R29	R1	K64
      0x547E0032,  //  00DC  LDINT	R31	51
      0x50800200,  //  00DD  LDBOOL	R32	1	0
      0x7C740600,  //  00DE  CALL	R29	3
      0x8C783B41,  //  00DF  GETMET	R30	R29	K65
      0x5C803800,  //  00E0  MOVE	R32	R28
      0x7C780400,  //  00E1  CALL	R30	2
      0x887C0142,  //  00E2  GETMBR	R31	R0	K66
      0x8C7C3F43,  //  00E3  GETMET	R31	R31	K67
      0x5C843A00,  //  00E4  MOVE	R33	R29
      0x7C7C0400,  //  00E5  CALL	R31	2
      0x8C7C0785,  //  00E6  GETMET	R31	R3	K133
      0x7C7C0200,  //  00E7  CALL	R31	1
      0x8C7C071C,  //  00E8  GETMET	R31	R3	K28
      0x5C843000,  //  00E9  MOVE	R33	R24
      0x5C883200,  //  00EA  MOVE	R34	R25
      0x5C8C3400,  //  00EB  MOVE	R35	R26
      0x5C903600,  //  00EC  MOVE	R36	R27
      0x7C7C0A00,  //  00ED  CALL	R31	5
      0x887C0D76,  //  00EE  GETMBR	R31	R6	K118
      0x900EEC1F,  //  00EF  SETMBR	R3	K118	R31
      0x900F0D1F,  //  00F0  SETMBR	R3	K134	K31
      0x8C7C0787,  //  00F1  GETMET	R31	R3	K135
      0x7C7C0200,  //  00F2  CALL	R31	1
      0x8C7C0788,  //  00F3  GETMET	R31	R3	K136
      0x50840200,  //  00F4  LDBOOL	R33	1	0
      0x7C7C0400,  //  00F5  CALL	R31	2
      0x8C7C0789,  //  00F6  GETMET	R31	R3	K137
      0x7C7C0200,  //  00F7  CALL	R31	1
      0x8C7C078A,  //  00F8  GETMET	R31	R3	K138
      0x7C7C0200,  //  00F9  CALL	R31	1
      0x8C7C078B,  //  00FA  GETMET	R31	R3	K139
      0x7C7C0200,  //  00FB  CALL	R31	1
      0xB87E0A00,  //  00FC  GETNGBL	R31	K5
      0x588000A9,  //  00FD  LDCONST	R32	K169
      0x58840024,  //  00FE  LDCONST	R33	K36
      0x7C7C0400,  //  00FF  CALL	R31	2
      0x507C0200,  //  0100  LDBOOL	R31	1	0
      0x80043E00,  //  0101  RET	1	R31
      0x70020004,  //  0102  JMP		#0108
      0xB8420A00,  //  0103  GETNGBL	R16	K5
      0x584400AA,  //  0104  LDCONST	R17	K170
      0x58480024,  //  0105  LDCONST	R18	K36
      0x7C400400,  //  0106  CALL	R16	2
      0x50140000,  //  0107  LDBOOL	R5	0	0
      0x5C1C0A00,  //  0108  MOVE	R7	R5
      0x741E00E4,  //  0109  JMPT	R7	#01EF
      0x8C1C01AB,  //  010A  GETMET	R7	R0	K171
      0x882409AC,  //  010B  GETMBR	R9	R4	K172
      0x88280936,  //  010C  GETMBR	R10	R4	K54
      0x7C1C0600,  //  010D  CALL	R7	3
      0x900F3407,  //  010E  SETMBR	R3	K154	R7
      0x4C200000,  //  010F  LDNIL	R8
      0x1C200608,  //  0110  EQ	R8	R3	R8
      0x74220003,  //  0111  JMPT	R8	#0116
      0x8820079A,  //  0112  GETMBR	R8	R3	K154
      0x4C240000,  //  0113  LDNIL	R9
      0x1C201009,  //  0114  EQ	R8	R8	R9
      0x7822000C,  //  0115  JMPF	R8	#0123
      0xB8220A00,  //  0116  GETNGBL	R8	K5
      0x582400AD,  //  0117  LDCONST	R9	K173
      0x58280024,  //  0118  LDCONST	R10	K36
      0x7C200400,  //  0119  CALL	R8	2
      0x8C200125,  //  011A  GETMET	R8	R0	K37
      0x5C280200,  //  011B  MOVE	R10	R1
      0x582C0026,  //  011C  LDCONST	R11	K38
      0x5830001F,  //  011D  LDCONST	R12	K31
      0x58340026,  //  011E  LDCONST	R13	K38
      0x50380000,  //  011F  LDBOOL	R14	0	0
      0x7C200C00,  //  0120  CALL	R8	6
      0x50200000,  //  0121  LDBOOL	R8	0	0
      0x80041000,  //  0122  RET	1	R8
      0x882003A0,  //  0123  GETMBR	R8	R1	K160
      0x900F3E08,  //  0124  SETMBR	R3	K159	R8
      0x8C2007A1,  //  0125  GETMET	R8	R3	K161
      0x7C200200,  //  0126  CALL	R8	1
      0x8820092F,  //  0127  GETMBR	R8	R4	K47
      0x900E5C08,  //  0128  SETMBR	R3	K46	R8
      0x8820010D,  //  0129  GETMBR	R8	R0	K13
      0x8820110E,  //  012A  GETMBR	R8	R8	K14
      0x8C201131,  //  012B  GETMET	R8	R8	K49
      0x7C200200,  //  012C  CALL	R8	1
      0x900E6008,  //  012D  SETMBR	R3	K48	R8
      0xB8220A00,  //  012E  GETNGBL	R8	K5
      0x60240018,  //  012F  GETGBL	R9	G24
      0x58280032,  //  0130  LDCONST	R10	K50
      0x882C0730,  //  0131  GETMBR	R11	R3	K48
      0x88300333,  //  0132  GETMBR	R12	R1	K51
      0x88340334,  //  0133  GETMBR	R13	R1	K52
      0x7C240800,  //  0134  CALL	R9	4
      0x58280024,  //  0135  LDCONST	R10	K36
      0x7C200400,  //  0136  CALL	R8	2
      0x8C200538,  //  0137  GETMET	R8	R2	K56
      0x542A000F,  //  0138  LDINT	R10	16
      0x7C200400,  //  0139  CALL	R8	2
      0x900F4408,  //  013A  SETMBR	R3	K162	R8
      0x8C200538,  //  013B  GETMET	R8	R2	K56
      0x542A001F,  //  013C  LDINT	R10	32
      0x7C200400,  //  013D  CALL	R8	2
      0x900F5C08,  //  013E  SETMBR	R3	K174	R8
      0x8C20057F,  //  013F  GETMET	R8	R2	K127
      0x7C200200,  //  0140  CALL	R8	1
      0x8C2011AF,  //  0141  GETMET	R8	R8	K175
      0x882807AE,  //  0142  GETMBR	R10	R3	K174
      0x7C200400,  //  0143  CALL	R8	2
      0x900EFC08,  //  0144  SETMBR	R3	K126	R8
      0x8C200538,  //  0145  GETMET	R8	R2	K56
      0x542A001F,  //  0146  LDINT	R10	32
      0x7C200400,  //  0147  CALL	R8	2
      0x8C24057F,  //  0148  GETMET	R9	R2	K127
      0x7C240200,  //  0149  CALL	R9	1
      0x8C2413B0,  //  014A  GETMET	R9	R9	K176
      0x882C07AE,  //  014B  GETMBR	R11	R3	K174
      0x88300996,  //  014C  GETMBR	R12	R4	K150
      0x7C240600,  //  014D  CALL	R9	3
      0x900ED409,  //  014E  SETMBR	R3	K106	R9
      0xB8264E00,  //  014F  GETNGBL	R9	K39
      0x88241372,  //  0150  GETMBR	R9	R9	K114
      0x8C24137A,  //  0151  GETMET	R9	R9	K122
      0x7C240200,  //  0152  CALL	R9	1
      0x8C28137B,  //  0153  GETMET	R10	R9	K123
      0x58300026,  //  0154  LDCONST	R12	K38
      0xB8364E00,  //  0155  GETNGBL	R13	K39
      0x88341B72,  //  0156  GETMBR	R13	R13	K114
      0x88341BB1,  //  0157  GETMBR	R13	R13	K177
      0x8C380FB2,  //  0158  GETMET	R14	R7	K178
      0x7C380200,  //  0159  CALL	R14	1
      0x7C280800,  //  015A  CALL	R10	4
      0x8C28137B,  //  015B  GETMET	R10	R9	K123
      0x58300045,  //  015C  LDCONST	R12	K69
      0xB8364E00,  //  015D  GETNGBL	R13	K39
      0x88341B72,  //  015E  GETMBR	R13	R13	K114
      0x88341BB1,  //  015F  GETMBR	R13	R13	K177
      0x8C380FB3,  //  0160  GETMET	R14	R7	K179
      0x7C380200,  //  0161  CALL	R14	1
      0x7C280800,  //  0162  CALL	R10	4
      0x8C28137B,  //  0163  GETMET	R10	R9	K123
      0x58300024,  //  0164  LDCONST	R12	K36
      0xB8364E00,  //  0165  GETNGBL	R13	K39
      0x88341B72,  //  0166  GETMBR	R13	R13	K114
      0x88341BB1,  //  0167  GETMBR	R13	R13	K177
      0x8838077E,  //  0168  GETMBR	R14	R3	K126
      0x7C280800,  //  0169  CALL	R10	4
      0x8C28137B,  //  016A  GETMET	R10	R9	K123
      0x54320003,  //  016B  LDINT	R12	4
      0xB8364E00,  //  016C  GETNGBL	R13	K39
      0x88341B72,  //  016D  GETMBR	R13	R13	K114
      0x88341BB1,  //  016E  GETMBR	R13	R13	K177
      0x88380996,  //  016F  GETMBR	R14	R4	K150
      0x7C280800,  //  0170  CALL	R10	4
      0x8C28057F,  //  0171  GETMET	R10	R2	K127
      0x7C280200,  //  0172  CALL	R10	1
      0x8C2815B4,  //  0173  GETMET	R10	R10	K180
      0x8C300FB5,  //  0174  GETMET	R12	R7	K181
      0x7C300200,  //  0175  CALL	R12	1
      0x8C34133E,  //  0176  GETMET	R13	R9	K62
      0x7C340200,  //  0177  CALL	R13	1
      0x7C280600,  //  0178  CALL	R10	3
      0xB82E4E00,  //  0179  GETNGBL	R11	K39
      0x882C1772,  //  017A  GETMBR	R11	R11	K114
      0x8C2C177A,  //  017B  GETMET	R11	R11	K122
      0x7C2C0200,  //  017C  CALL	R11	1
      0x8C30177B,  //  017D  GETMET	R12	R11	K123
      0x58380026,  //  017E  LDCONST	R14	K38
      0xB83E4E00,  //  017F  GETNGBL	R15	K39
      0x883C1F72,  //  0180  GETMBR	R15	R15	K114
      0x883C1FB1,  //  0181  GETMBR	R15	R15	K177
      0x8C400FB2,  //  0182  GETMET	R16	R7	K178
      0x7C400200,  //  0183  CALL	R16	1
      0x7C300800,  //  0184  CALL	R12	4
      0x8C30177B,  //  0185  GETMET	R12	R11	K123
      0x58380045,  //  0186  LDCONST	R14	K69
      0xB83E4E00,  //  0187  GETNGBL	R15	K39
      0x883C1F72,  //  0188  GETMBR	R15	R15	K114
      0x883C1FB1,  //  0189  GETMBR	R15	R15	K177
      0x8C400FB3,  //  018A  GETMET	R16	R7	K179
      0x7C400200,  //  018B  CALL	R16	1
      0x7C300800,  //  018C  CALL	R12	4
      0x8C30177B,  //  018D  GETMET	R12	R11	K123
      0x58380024,  //  018E  LDCONST	R14	K36
      0xB83E4E00,  //  018F  GETNGBL	R15	K39
      0x883C1F72,  //  0190  GETMBR	R15	R15	K114
      0x883C1FB1,  //  0191  GETMBR	R15	R15	K177
      0x5C401400,  //  0192  MOVE	R16	R10
      0x7C300800,  //  0193  CALL	R12	4
      0x8C30177B,  //  0194  GETMET	R12	R11	K123
      0x543A0003,  //  0195  LDINT	R14	4
      0xB83E4E00,  //  0196  GETNGBL	R15	K39
      0x883C1F72,  //  0197  GETMBR	R15	R15	K114
      0x883C1FB1,  //  0198  GETMBR	R15	R15	K177
      0x884007A2,  //  0199  GETMBR	R16	R3	K162
      0x7C300800,  //  019A  CALL	R12	4
      0x883009B6,  //  019B  GETMBR	R12	R4	K182
      0x900E560C,  //  019C  SETMBR	R3	K43	R12
      0x8C30054E,  //  019D  GETMET	R12	R2	K78
      0x7C300200,  //  019E  CALL	R12	1
      0x8C301916,  //  019F  GETMET	R12	R12	K22
      0x8838072B,  //  01A0  GETMBR	R14	R3	K43
      0x7C300400,  //  01A1  CALL	R12	2
      0x8C301917,  //  01A2  GETMET	R12	R12	K23
      0x7C300200,  //  01A3  CALL	R12	1
      0x60340015,  //  01A4  GETGBL	R13	G21
      0x7C340000,  //  01A5  CALL	R13	0
      0x8C341B4F,  //  01A6  GETMET	R13	R13	K79
      0x883C01B7,  //  01A7  GETMBR	R15	R0	K183
      0x7C340400,  //  01A8  CALL	R13	2
      0x8C380F14,  //  01A9  GETMET	R14	R7	K20
      0x7C380200,  //  01AA  CALL	R14	1
      0x00381C08,  //  01AB  ADD	R14	R14	R8
      0x883C077E,  //  01AC  GETMBR	R15	R3	K126
      0x00381C0F,  //  01AD  ADD	R14	R14	R15
      0x00381C0C,  //  01AE  ADD	R14	R14	R12
      0x8C3C0561,  //  01AF  GETMET	R15	R2	K97
      0x7C3C0200,  //  01B0  CALL	R15	1
      0x8C3C1F62,  //  01B1  GETMET	R15	R15	K98
      0x8844076A,  //  01B2  GETMBR	R17	R3	K106
      0x5C481C00,  //  01B3  MOVE	R18	R14
      0x5C4C1A00,  //  01B4  MOVE	R19	R13
      0x5452000F,  //  01B5  LDINT	R20	16
      0x7C3C0A00,  //  01B6  CALL	R15	5
      0x8C40173E,  //  01B7  GETMET	R16	R11	K62
      0x7C400200,  //  01B8  CALL	R16	1
      0x8C44056D,  //  01B9  GETMET	R17	R2	K109
      0x5C4C1E00,  //  01BA  MOVE	R19	R15
      0x60500015,  //  01BB  GETGBL	R20	G21
      0x7C500000,  //  01BC  CALL	R20	0
      0x8C50294F,  //  01BD  GETMET	R20	R20	K79
      0x885801B8,  //  01BE  GETMBR	R22	R0	K184
      0x7C500400,  //  01BF  CALL	R20	2
      0x60540015,  //  01C0  GETGBL	R21	G21
      0x7C540000,  //  01C1  CALL	R21	0
      0x6058000C,  //  01C2  GETGBL	R22	G12
      0x5C5C2000,  //  01C3  MOVE	R23	R16
      0x7C580200,  //  01C4  CALL	R22	1
      0x545E000F,  //  01C5  LDINT	R23	16
      0x7C440C00,  //  01C6  CALL	R17	6
      0x8C4823B9,  //  01C7  GETMET	R18	R17	K185
      0x5C502000,  //  01C8  MOVE	R20	R16
      0x7C480400,  //  01C9  CALL	R18	2
      0x8C4C2370,  //  01CA  GETMET	R19	R17	K112
      0x7C4C0200,  //  01CB  CALL	R19	1
      0x00482413,  //  01CC  ADD	R18	R18	R19
      0xB84E4E00,  //  01CD  GETNGBL	R19	K39
      0x8C4C27BA,  //  01CE  GETMET	R19	R19	K186
      0x7C4C0200,  //  01CF  CALL	R19	1
      0x904E6E08,  //  01D0  SETMBR	R19	K55	R8
      0x88500730,  //  01D1  GETMBR	R20	R3	K48
      0x904E7214,  //  01D2  SETMBR	R19	K57	R20
      0x8850077E,  //  01D3  GETMBR	R20	R3	K126
      0x904F7614,  //  01D4  SETMBR	R19	K187	R20
      0x904F7812,  //  01D5  SETMBR	R19	K188	R18
      0x8C50273E,  //  01D6  GETMET	R20	R19	K62
      0x7C500200,  //  01D7  CALL	R20	1
      0x900E7E14,  //  01D8  SETMBR	R3	K63	R20
      0x8C540340,  //  01D9  GETMET	R21	R1	K64
      0x545E0030,  //  01DA  LDINT	R23	49
      0x50600200,  //  01DB  LDBOOL	R24	1	0
      0x7C540600,  //  01DC  CALL	R21	3
      0x8C582B41,  //  01DD  GETMET	R22	R21	K65
      0x5C602800,  //  01DE  MOVE	R24	R20
      0x7C580400,  //  01DF  CALL	R22	2
      0xB85E0A00,  //  01E0  GETNGBL	R23	K5
      0x60600018,  //  01E1  GETGBL	R24	G24
      0x586400BD,  //  01E2  LDCONST	R25	K189
      0x88680730,  //  01E3  GETMBR	R26	R3	K48
      0x886C075B,  //  01E4  GETMBR	R27	R3	K91
      0x8870075C,  //  01E5  GETMBR	R28	R3	K92
      0x7C600800,  //  01E6  CALL	R24	4
      0x58640045,  //  01E7  LDCONST	R25	K69
      0x7C5C0400,  //  01E8  CALL	R23	2
      0x885C0142,  //  01E9  GETMBR	R23	R0	K66
      0x8C5C2F43,  //  01EA  GETMET	R23	R23	K67
      0x5C642A00,  //  01EB  MOVE	R25	R21
      0x7C5C0400,  //  01EC  CALL	R23	2
      0x505C0200,  //  01ED  LDBOOL	R23	1	0
      0x80042E00,  //  01EE  RET	1	R23
      0x501C0200,  //  01EF  LDBOOL	R7	1	0
      0x80040E00,  //  01F0  RET	1	R7
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
