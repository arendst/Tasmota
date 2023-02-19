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
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[47]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(protocol_error),
    /* K6   */  be_nested_str_weak(invalid_X20PBKDFParamRequest_X20message),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(PBKDFParamRequest),
    /* K9   */  be_nested_str_weak(parse),
    /* K10  */  be_nested_str_weak(raw),
    /* K11  */  be_nested_str_weak(app_payload_idx),
    /* K12  */  be_nested_str_weak(session),
    /* K13  */  be_nested_str_weak(set_mode),
    /* K14  */  be_nested_str_weak(Session),
    /* K15  */  be_nested_str_weak(__PASE),
    /* K16  */  be_const_int(2147483647),
    /* K17  */  be_nested_str_weak(passcodeId),
    /* K18  */  be_nested_str_weak(non_X2Dzero_X20passcode_X20id),
    /* K19  */  be_nested_str_weak(future_initiator_session_id),
    /* K20  */  be_nested_str_weak(initiator_session_id),
    /* K21  */  be_nested_str_weak(future_local_session_id),
    /* K22  */  be_nested_str_weak(device),
    /* K23  */  be_nested_str_weak(sessions),
    /* K24  */  be_nested_str_weak(gen_local_session_id),
    /* K25  */  be_nested_str_weak(PBKDFParamResponse),
    /* K26  */  be_nested_str_weak(initiatorRandom),
    /* K27  */  be_nested_str_weak(responderRandom),
    /* K28  */  be_nested_str_weak(random),
    /* K29  */  be_nested_str_weak(responderSessionId),
    /* K30  */  be_nested_str_weak(pbkdf_parameters_salt),
    /* K31  */  be_nested_str_weak(salt),
    /* K32  */  be_nested_str_weak(pbkdf_parameters_iterations),
    /* K33  */  be_nested_str_weak(iterations),
    /* K34  */  be_nested_str_weak(tasmota),
    /* K35  */  be_nested_str_weak(log),
    /* K36  */  be_nested_str_weak(MTR_X3A_X20pbkdfparamresp_X3A_X20),
    /* K37  */  be_nested_str_weak(inspect),
    /* K38  */  be_nested_str_weak(encode),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20pbkdfparamresp_raw_X3A_X20),
    /* K40  */  be_nested_str_weak(tohex),
    /* K41  */  be_nested_str_weak(build_response),
    /* K42  */  be_nested_str_weak(responder),
    /* K43  */  be_nested_str_weak(send_response),
    /* K44  */  be_nested_str_weak(remote_ip),
    /* K45  */  be_nested_str_weak(remote_port),
    /* K46  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x5412001F,  //  0002  LDINT	R4	32
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x740E0005,  //  0004  JMPT	R3	#000B
      0x880C0302,  //  0005  GETMBR	R3	R1	K2
      0x200C0703,  //  0006  NE	R3	R3	K3
      0x740E0002,  //  0007  JMPT	R3	#000B
      0x880C0304,  //  0008  GETMBR	R3	R1	K4
      0x200C0703,  //  0009  NE	R3	R3	K3
      0x780E0000,  //  000A  JMPF	R3	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0xB80E0E00,  //  000C  GETNGBL	R3	K7
      0x8C0C0708,  //  000D  GETMET	R3	R3	K8
      0x7C0C0200,  //  000E  CALL	R3	1
      0x8C0C0709,  //  000F  GETMET	R3	R3	K9
      0x8814030A,  //  0010  GETMBR	R5	R1	K10
      0x8818030B,  //  0011  GETMBR	R6	R1	K11
      0x7C0C0600,  //  0012  CALL	R3	3
      0x8810030C,  //  0013  GETMBR	R4	R1	K12
      0x8C10090D,  //  0014  GETMET	R4	R4	K13
      0xB81A0E00,  //  0015  GETNGBL	R6	K7
      0x88180D0E,  //  0016  GETMBR	R6	R6	K14
      0x88180D0F,  //  0017  GETMBR	R6	R6	K15
      0x7C100400,  //  0018  CALL	R4	2
      0x8810030B,  //  0019  GETMBR	R4	R1	K11
      0x40100910,  //  001A  CONNECT	R4	R4	K16
      0x8814030A,  //  001B  GETMBR	R5	R1	K10
      0x94100A04,  //  001C  GETIDX	R4	R5	R4
      0x90021004,  //  001D  SETMBR	R0	K8	R4
      0x88100711,  //  001E  GETMBR	R4	R3	K17
      0x20100903,  //  001F  NE	R4	R4	K3
      0x78120000,  //  0020  JMPF	R4	#0022
      0xB0060B12,  //  0021  RAISE	1	K5	K18
      0x88100714,  //  0022  GETMBR	R4	R3	K20
      0x90022604,  //  0023  SETMBR	R0	K19	R4
      0x88100116,  //  0024  GETMBR	R4	R0	K22
      0x88100917,  //  0025  GETMBR	R4	R4	K23
      0x8C100918,  //  0026  GETMET	R4	R4	K24
      0x7C100200,  //  0027  CALL	R4	1
      0x90022A04,  //  0028  SETMBR	R0	K21	R4
      0xB8120E00,  //  0029  GETNGBL	R4	K7
      0x8C100919,  //  002A  GETMET	R4	R4	K25
      0x7C100200,  //  002B  CALL	R4	1
      0x8814071A,  //  002C  GETMBR	R5	R3	K26
      0x90123405,  //  002D  SETMBR	R4	K26	R5
      0x8C14051C,  //  002E  GETMET	R5	R2	K28
      0x541E001F,  //  002F  LDINT	R7	32
      0x7C140400,  //  0030  CALL	R5	2
      0x90123605,  //  0031  SETMBR	R4	K27	R5
      0x88140115,  //  0032  GETMBR	R5	R0	K21
      0x90123A05,  //  0033  SETMBR	R4	K29	R5
      0x88140116,  //  0034  GETMBR	R5	R0	K22
      0x88140B1F,  //  0035  GETMBR	R5	R5	K31
      0x90123C05,  //  0036  SETMBR	R4	K30	R5
      0x88140116,  //  0037  GETMBR	R5	R0	K22
      0x88140B21,  //  0038  GETMBR	R5	R5	K33
      0x90124005,  //  0039  SETMBR	R4	K32	R5
      0xB8164400,  //  003A  GETNGBL	R5	K34
      0x8C140B23,  //  003B  GETMET	R5	R5	K35
      0x601C0008,  //  003C  GETGBL	R7	G8
      0xB8220E00,  //  003D  GETNGBL	R8	K7
      0x8C201125,  //  003E  GETMET	R8	R8	K37
      0x5C280800,  //  003F  MOVE	R10	R4
      0x7C200400,  //  0040  CALL	R8	2
      0x7C1C0200,  //  0041  CALL	R7	1
      0x001E4807,  //  0042  ADD	R7	K36	R7
      0x54220003,  //  0043  LDINT	R8	4
      0x7C140600,  //  0044  CALL	R5	3
      0x8C140926,  //  0045  GETMET	R5	R4	K38
      0x7C140200,  //  0046  CALL	R5	1
      0xB81A4400,  //  0047  GETNGBL	R6	K34
      0x8C180D23,  //  0048  GETMET	R6	R6	K35
      0x8C200B28,  //  0049  GETMET	R8	R5	K40
      0x7C200200,  //  004A  CALL	R8	1
      0x00224E08,  //  004B  ADD	R8	K39	R8
      0x54260003,  //  004C  LDINT	R9	4
      0x7C180600,  //  004D  CALL	R6	3
      0x90023205,  //  004E  SETMBR	R0	K25	R5
      0x8C180329,  //  004F  GETMET	R6	R1	K41
      0x54220020,  //  0050  LDINT	R8	33
      0x50240200,  //  0051  LDBOOL	R9	1	0
      0x7C180600,  //  0052  CALL	R6	3
      0x8C1C0D26,  //  0053  GETMET	R7	R6	K38
      0x5C240A00,  //  0054  MOVE	R9	R5
      0x7C1C0400,  //  0055  CALL	R7	2
      0x8820012A,  //  0056  GETMBR	R8	R0	K42
      0x8C20112B,  //  0057  GETMET	R8	R8	K43
      0x5C280E00,  //  0058  MOVE	R10	R7
      0x882C032C,  //  0059  GETMBR	R11	R1	K44
      0x8830032D,  //  005A  GETMBR	R12	R1	K45
      0x88340D2E,  //  005B  GETMBR	R13	R6	K46
      0x7C200A00,  //  005C  CALL	R8	5
      0x80000000,  //  005D  RET	0
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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(responder),
    /* K2   */  be_nested_str_weak(device),
    /* K3   */  be_nested_str_weak(y),
    /* K4   */  be_nested_str_weak(random),
    /* K5   */  be_nested_str_weak(window_open),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x880C0302,  //  0002  GETMBR	R3	R1	K2
      0x90020403,  //  0003  SETMBR	R0	K2	R3
      0x8C0C0504,  //  0004  GETMET	R3	R2	K4
      0x5416001F,  //  0005  LDINT	R5	32
      0x7C0C0400,  //  0006  CALL	R3	2
      0x90020603,  //  0007  SETMBR	R0	K3	R3
      0x500C0200,  //  0008  LDBOOL	R3	1	0
      0x90020A03,  //  0009  SETMBR	R0	K5	R3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Pake1
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Pake1,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[79]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(protocol_error),
    /* K6   */  be_nested_str_weak(invalid_X20Pake1_X20message),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(Pake1),
    /* K9   */  be_nested_str_weak(parse),
    /* K10  */  be_nested_str_weak(raw),
    /* K11  */  be_nested_str_weak(app_payload_idx),
    /* K12  */  be_nested_str_weak(pA),
    /* K13  */  be_nested_str_weak(tasmota),
    /* K14  */  be_nested_str_weak(log),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20received_X20pA_X3D),
    /* K16  */  be_nested_str_weak(tohex),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20spake_X3A_X20),
    /* K18  */  be_nested_str_weak(inspect),
    /* K19  */  be_nested_str_weak(spake),
    /* K20  */  be_nested_str_weak(SPAKE2P_Matter),
    /* K21  */  be_nested_str_weak(device),
    /* K22  */  be_nested_str_weak(w0),
    /* K23  */  be_nested_str_weak(w1),
    /* K24  */  be_nested_str_weak(L),
    /* K25  */  be_nested_str_weak(compute_pB),
    /* K26  */  be_nested_str_weak(y),
    /* K27  */  be_nested_str_weak(pB),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20y_X3D),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20pb_X3D),
    /* K30  */  be_nested_str_weak(compute_ZV_verifier),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20Z_X3D),
    /* K32  */  be_nested_str_weak(Z),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20V_X3D),
    /* K34  */  be_nested_str_weak(V),
    /* K35  */  be_nested_str_weak(SHA256),
    /* K36  */  be_nested_str_weak(update),
    /* K37  */  be_nested_str_weak(fromstring),
    /* K38  */  be_nested_str_weak(Matter_Context_Prefix),
    /* K39  */  be_nested_str_weak(PBKDFParamRequest),
    /* K40  */  be_nested_str_weak(PBKDFParamResponse),
    /* K41  */  be_nested_str_weak(out),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20Context_X3D),
    /* K43  */  be_nested_str_weak(set_context),
    /* K44  */  be_nested_str_weak(compute_TT_hash),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20Context_X20_X3D_X20),
    /* K47  */  be_nested_str_weak(Context),
    /* K48  */  be_nested_str_weak(MTR_X3A_X20M_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K49  */  be_nested_str_weak(M),
    /* K50  */  be_nested_str_weak(MTR_X3A_X20N_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K51  */  be_nested_str_weak(N),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20pA_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20pB_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20Z_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20V_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K56  */  be_nested_str_weak(MTR_X3A_X20w0_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K57  */  be_nested_str_weak(MTR_X3A_X20Kmain_X20_X20_X20_X3D),
    /* K58  */  be_nested_str_weak(Kmain),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20KcA_X20_X20_X20_X20_X20_X3D),
    /* K60  */  be_nested_str_weak(KcA),
    /* K61  */  be_nested_str_weak(MTR_X3A_X20KcB_X20_X20_X20_X20_X20_X3D),
    /* K62  */  be_nested_str_weak(KcB),
    /* K63  */  be_nested_str_weak(MTR_X3A_X20K_shared_X3D),
    /* K64  */  be_nested_str_weak(K_shared),
    /* K65  */  be_nested_str_weak(MTR_X3A_X20Ke_X20_X20_X20_X20_X20_X20_X3D),
    /* K66  */  be_nested_str_weak(Ke),
    /* K67  */  be_nested_str_weak(cB),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20cB_X3D),
    /* K69  */  be_nested_str_weak(Pake2),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20pake2_X3A_X20),
    /* K71  */  be_nested_str_weak(encode),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20pake2_raw_X3A_X20),
    /* K73  */  be_nested_str_weak(build_response),
    /* K74  */  be_nested_str_weak(responder),
    /* K75  */  be_nested_str_weak(send_response),
    /* K76  */  be_nested_str_weak(remote_ip),
    /* K77  */  be_nested_str_weak(remote_port),
    /* K78  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[308]) {  /* code */
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
      0x780E0000,  //  000A  JMPF	R3	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0xB80E0E00,  //  000C  GETNGBL	R3	K7
      0x8C0C0708,  //  000D  GETMET	R3	R3	K8
      0x7C0C0200,  //  000E  CALL	R3	1
      0x8C0C0709,  //  000F  GETMET	R3	R3	K9
      0x8814030A,  //  0010  GETMBR	R5	R1	K10
      0x8818030B,  //  0011  GETMBR	R6	R1	K11
      0x7C0C0600,  //  0012  CALL	R3	3
      0x8810070C,  //  0013  GETMBR	R4	R3	K12
      0x90021804,  //  0014  SETMBR	R0	K12	R4
      0xB8121A00,  //  0015  GETNGBL	R4	K13
      0x8C10090E,  //  0016  GETMET	R4	R4	K14
      0x8818010C,  //  0017  GETMBR	R6	R0	K12
      0x8C180D10,  //  0018  GETMET	R6	R6	K16
      0x7C180200,  //  0019  CALL	R6	1
      0x001A1E06,  //  001A  ADD	R6	K15	R6
      0x541E0003,  //  001B  LDINT	R7	4
      0x7C100600,  //  001C  CALL	R4	3
      0xB8121A00,  //  001D  GETNGBL	R4	K13
      0x8C10090E,  //  001E  GETMET	R4	R4	K14
      0xB81A0E00,  //  001F  GETNGBL	R6	K7
      0x8C180D12,  //  0020  GETMET	R6	R6	K18
      0x88200113,  //  0021  GETMBR	R8	R0	K19
      0x7C180400,  //  0022  CALL	R6	2
      0x001A2206,  //  0023  ADD	R6	K17	R6
      0x541E0003,  //  0024  LDINT	R7	4
      0x7C100600,  //  0025  CALL	R4	3
      0x8C100514,  //  0026  GETMET	R4	R2	K20
      0x88180115,  //  0027  GETMBR	R6	R0	K21
      0x88180D16,  //  0028  GETMBR	R6	R6	K22
      0x881C0115,  //  0029  GETMBR	R7	R0	K21
      0x881C0F17,  //  002A  GETMBR	R7	R7	K23
      0x88200115,  //  002B  GETMBR	R8	R0	K21
      0x88201118,  //  002C  GETMBR	R8	R8	K24
      0x7C100800,  //  002D  CALL	R4	4
      0x90022604,  //  002E  SETMBR	R0	K19	R4
      0x88100113,  //  002F  GETMBR	R4	R0	K19
      0x8C100919,  //  0030  GETMET	R4	R4	K25
      0x8818011A,  //  0031  GETMBR	R6	R0	K26
      0x7C100400,  //  0032  CALL	R4	2
      0x88100113,  //  0033  GETMBR	R4	R0	K19
      0x8810091B,  //  0034  GETMBR	R4	R4	K27
      0x90023604,  //  0035  SETMBR	R0	K27	R4
      0xB8121A00,  //  0036  GETNGBL	R4	K13
      0x8C10090E,  //  0037  GETMET	R4	R4	K14
      0x8818011A,  //  0038  GETMBR	R6	R0	K26
      0x8C180D10,  //  0039  GETMET	R6	R6	K16
      0x7C180200,  //  003A  CALL	R6	1
      0x001A3806,  //  003B  ADD	R6	K28	R6
      0x541E0003,  //  003C  LDINT	R7	4
      0x7C100600,  //  003D  CALL	R4	3
      0xB8121A00,  //  003E  GETNGBL	R4	K13
      0x8C10090E,  //  003F  GETMET	R4	R4	K14
      0x8818011B,  //  0040  GETMBR	R6	R0	K27
      0x8C180D10,  //  0041  GETMET	R6	R6	K16
      0x7C180200,  //  0042  CALL	R6	1
      0x001A3A06,  //  0043  ADD	R6	K29	R6
      0x541E0003,  //  0044  LDINT	R7	4
      0x7C100600,  //  0045  CALL	R4	3
      0x88100113,  //  0046  GETMBR	R4	R0	K19
      0x8C10091E,  //  0047  GETMET	R4	R4	K30
      0x8818010C,  //  0048  GETMBR	R6	R0	K12
      0x7C100400,  //  0049  CALL	R4	2
      0xB8121A00,  //  004A  GETNGBL	R4	K13
      0x8C10090E,  //  004B  GETMET	R4	R4	K14
      0x88180113,  //  004C  GETMBR	R6	R0	K19
      0x88180D20,  //  004D  GETMBR	R6	R6	K32
      0x8C180D10,  //  004E  GETMET	R6	R6	K16
      0x7C180200,  //  004F  CALL	R6	1
      0x001A3E06,  //  0050  ADD	R6	K31	R6
      0x541E0003,  //  0051  LDINT	R7	4
      0x7C100600,  //  0052  CALL	R4	3
      0xB8121A00,  //  0053  GETNGBL	R4	K13
      0x8C10090E,  //  0054  GETMET	R4	R4	K14
      0x88180113,  //  0055  GETMBR	R6	R0	K19
      0x88180D22,  //  0056  GETMBR	R6	R6	K34
      0x8C180D10,  //  0057  GETMET	R6	R6	K16
      0x7C180200,  //  0058  CALL	R6	1
      0x001A4206,  //  0059  ADD	R6	K33	R6
      0x541E0003,  //  005A  LDINT	R7	4
      0x7C100600,  //  005B  CALL	R4	3
      0x8C100523,  //  005C  GETMET	R4	R2	K35
      0x7C100200,  //  005D  CALL	R4	1
      0x8C140924,  //  005E  GETMET	R5	R4	K36
      0x601C0015,  //  005F  GETGBL	R7	G21
      0x7C1C0000,  //  0060  CALL	R7	0
      0x8C1C0F25,  //  0061  GETMET	R7	R7	K37
      0x88240126,  //  0062  GETMBR	R9	R0	K38
      0x7C1C0400,  //  0063  CALL	R7	2
      0x7C140400,  //  0064  CALL	R5	2
      0x8C140924,  //  0065  GETMET	R5	R4	K36
      0x881C0127,  //  0066  GETMBR	R7	R0	K39
      0x7C140400,  //  0067  CALL	R5	2
      0x8C140924,  //  0068  GETMET	R5	R4	K36
      0x881C0128,  //  0069  GETMBR	R7	R0	K40
      0x7C140400,  //  006A  CALL	R5	2
      0x8C140929,  //  006B  GETMET	R5	R4	K41
      0x7C140200,  //  006C  CALL	R5	1
      0xB81A1A00,  //  006D  GETNGBL	R6	K13
      0x8C180D0E,  //  006E  GETMET	R6	R6	K14
      0x8C200B10,  //  006F  GETMET	R8	R5	K16
      0x7C200200,  //  0070  CALL	R8	1
      0x00225408,  //  0071  ADD	R8	K42	R8
      0x54260003,  //  0072  LDINT	R9	4
      0x7C180600,  //  0073  CALL	R6	3
      0x88180113,  //  0074  GETMBR	R6	R0	K19
      0x881C010C,  //  0075  GETMBR	R7	R0	K12
      0x901A1807,  //  0076  SETMBR	R6	K12	R7
      0x88180113,  //  0077  GETMBR	R6	R0	K19
      0x8C180D2B,  //  0078  GETMET	R6	R6	K43
      0x5C200A00,  //  0079  MOVE	R8	R5
      0x7C180400,  //  007A  CALL	R6	2
      0x88180113,  //  007B  GETMBR	R6	R0	K19
      0x8C180D2C,  //  007C  GETMET	R6	R6	K44
      0x50200200,  //  007D  LDBOOL	R8	1	0
      0x7C180400,  //  007E  CALL	R6	2
      0xB81A1A00,  //  007F  GETNGBL	R6	K13
      0x8C180D0E,  //  0080  GETMET	R6	R6	K14
      0x5820002D,  //  0081  LDCONST	R8	K45
      0x54260003,  //  0082  LDINT	R9	4
      0x7C180600,  //  0083  CALL	R6	3
      0xB81A1A00,  //  0084  GETNGBL	R6	K13
      0x8C180D0E,  //  0085  GETMET	R6	R6	K14
      0x88200113,  //  0086  GETMBR	R8	R0	K19
      0x8820112F,  //  0087  GETMBR	R8	R8	K47
      0x8C201110,  //  0088  GETMET	R8	R8	K16
      0x7C200200,  //  0089  CALL	R8	1
      0x00225C08,  //  008A  ADD	R8	K46	R8
      0x54260003,  //  008B  LDINT	R9	4
      0x7C180600,  //  008C  CALL	R6	3
      0xB81A1A00,  //  008D  GETNGBL	R6	K13
      0x8C180D0E,  //  008E  GETMET	R6	R6	K14
      0x88200113,  //  008F  GETMBR	R8	R0	K19
      0x88201131,  //  0090  GETMBR	R8	R8	K49
      0x8C201110,  //  0091  GETMET	R8	R8	K16
      0x7C200200,  //  0092  CALL	R8	1
      0x00226008,  //  0093  ADD	R8	K48	R8
      0x54260003,  //  0094  LDINT	R9	4
      0x7C180600,  //  0095  CALL	R6	3
      0xB81A1A00,  //  0096  GETNGBL	R6	K13
      0x8C180D0E,  //  0097  GETMET	R6	R6	K14
      0x88200113,  //  0098  GETMBR	R8	R0	K19
      0x88201133,  //  0099  GETMBR	R8	R8	K51
      0x8C201110,  //  009A  GETMET	R8	R8	K16
      0x7C200200,  //  009B  CALL	R8	1
      0x00226408,  //  009C  ADD	R8	K50	R8
      0x54260003,  //  009D  LDINT	R9	4
      0x7C180600,  //  009E  CALL	R6	3
      0xB81A1A00,  //  009F  GETNGBL	R6	K13
      0x8C180D0E,  //  00A0  GETMET	R6	R6	K14
      0x88200113,  //  00A1  GETMBR	R8	R0	K19
      0x8820110C,  //  00A2  GETMBR	R8	R8	K12
      0x8C201110,  //  00A3  GETMET	R8	R8	K16
      0x7C200200,  //  00A4  CALL	R8	1
      0x00226808,  //  00A5  ADD	R8	K52	R8
      0x54260003,  //  00A6  LDINT	R9	4
      0x7C180600,  //  00A7  CALL	R6	3
      0xB81A1A00,  //  00A8  GETNGBL	R6	K13
      0x8C180D0E,  //  00A9  GETMET	R6	R6	K14
      0x88200113,  //  00AA  GETMBR	R8	R0	K19
      0x8820111B,  //  00AB  GETMBR	R8	R8	K27
      0x8C201110,  //  00AC  GETMET	R8	R8	K16
      0x7C200200,  //  00AD  CALL	R8	1
      0x00226A08,  //  00AE  ADD	R8	K53	R8
      0x54260003,  //  00AF  LDINT	R9	4
      0x7C180600,  //  00B0  CALL	R6	3
      0xB81A1A00,  //  00B1  GETNGBL	R6	K13
      0x8C180D0E,  //  00B2  GETMET	R6	R6	K14
      0x88200113,  //  00B3  GETMBR	R8	R0	K19
      0x88201120,  //  00B4  GETMBR	R8	R8	K32
      0x8C201110,  //  00B5  GETMET	R8	R8	K16
      0x7C200200,  //  00B6  CALL	R8	1
      0x00226C08,  //  00B7  ADD	R8	K54	R8
      0x54260003,  //  00B8  LDINT	R9	4
      0x7C180600,  //  00B9  CALL	R6	3
      0xB81A1A00,  //  00BA  GETNGBL	R6	K13
      0x8C180D0E,  //  00BB  GETMET	R6	R6	K14
      0x88200113,  //  00BC  GETMBR	R8	R0	K19
      0x88201122,  //  00BD  GETMBR	R8	R8	K34
      0x8C201110,  //  00BE  GETMET	R8	R8	K16
      0x7C200200,  //  00BF  CALL	R8	1
      0x00226E08,  //  00C0  ADD	R8	K55	R8
      0x54260003,  //  00C1  LDINT	R9	4
      0x7C180600,  //  00C2  CALL	R6	3
      0xB81A1A00,  //  00C3  GETNGBL	R6	K13
      0x8C180D0E,  //  00C4  GETMET	R6	R6	K14
      0x88200113,  //  00C5  GETMBR	R8	R0	K19
      0x88201116,  //  00C6  GETMBR	R8	R8	K22
      0x8C201110,  //  00C7  GETMET	R8	R8	K16
      0x7C200200,  //  00C8  CALL	R8	1
      0x00227008,  //  00C9  ADD	R8	K56	R8
      0x54260003,  //  00CA  LDINT	R9	4
      0x7C180600,  //  00CB  CALL	R6	3
      0xB81A1A00,  //  00CC  GETNGBL	R6	K13
      0x8C180D0E,  //  00CD  GETMET	R6	R6	K14
      0x5820002D,  //  00CE  LDCONST	R8	K45
      0x54260003,  //  00CF  LDINT	R9	4
      0x7C180600,  //  00D0  CALL	R6	3
      0xB81A1A00,  //  00D1  GETNGBL	R6	K13
      0x8C180D0E,  //  00D2  GETMET	R6	R6	K14
      0x88200113,  //  00D3  GETMBR	R8	R0	K19
      0x8820113A,  //  00D4  GETMBR	R8	R8	K58
      0x8C201110,  //  00D5  GETMET	R8	R8	K16
      0x7C200200,  //  00D6  CALL	R8	1
      0x00227208,  //  00D7  ADD	R8	K57	R8
      0x54260003,  //  00D8  LDINT	R9	4
      0x7C180600,  //  00D9  CALL	R6	3
      0xB81A1A00,  //  00DA  GETNGBL	R6	K13
      0x8C180D0E,  //  00DB  GETMET	R6	R6	K14
      0x88200113,  //  00DC  GETMBR	R8	R0	K19
      0x8820113C,  //  00DD  GETMBR	R8	R8	K60
      0x8C201110,  //  00DE  GETMET	R8	R8	K16
      0x7C200200,  //  00DF  CALL	R8	1
      0x00227608,  //  00E0  ADD	R8	K59	R8
      0x54260003,  //  00E1  LDINT	R9	4
      0x7C180600,  //  00E2  CALL	R6	3
      0xB81A1A00,  //  00E3  GETNGBL	R6	K13
      0x8C180D0E,  //  00E4  GETMET	R6	R6	K14
      0x88200113,  //  00E5  GETMBR	R8	R0	K19
      0x8820113E,  //  00E6  GETMBR	R8	R8	K62
      0x8C201110,  //  00E7  GETMET	R8	R8	K16
      0x7C200200,  //  00E8  CALL	R8	1
      0x00227A08,  //  00E9  ADD	R8	K61	R8
      0x54260003,  //  00EA  LDINT	R9	4
      0x7C180600,  //  00EB  CALL	R6	3
      0xB81A1A00,  //  00EC  GETNGBL	R6	K13
      0x8C180D0E,  //  00ED  GETMET	R6	R6	K14
      0x88200113,  //  00EE  GETMBR	R8	R0	K19
      0x88201140,  //  00EF  GETMBR	R8	R8	K64
      0x8C201110,  //  00F0  GETMET	R8	R8	K16
      0x7C200200,  //  00F1  CALL	R8	1
      0x00227E08,  //  00F2  ADD	R8	K63	R8
      0x54260003,  //  00F3  LDINT	R9	4
      0x7C180600,  //  00F4  CALL	R6	3
      0xB81A1A00,  //  00F5  GETNGBL	R6	K13
      0x8C180D0E,  //  00F6  GETMET	R6	R6	K14
      0x88200113,  //  00F7  GETMBR	R8	R0	K19
      0x88201142,  //  00F8  GETMBR	R8	R8	K66
      0x8C201110,  //  00F9  GETMET	R8	R8	K16
      0x7C200200,  //  00FA  CALL	R8	1
      0x00228208,  //  00FB  ADD	R8	K65	R8
      0x54260003,  //  00FC  LDINT	R9	4
      0x7C180600,  //  00FD  CALL	R6	3
      0x88180113,  //  00FE  GETMBR	R6	R0	K19
      0x88180D43,  //  00FF  GETMBR	R6	R6	K67
      0x90028606,  //  0100  SETMBR	R0	K67	R6
      0x88180113,  //  0101  GETMBR	R6	R0	K19
      0x88180D42,  //  0102  GETMBR	R6	R6	K66
      0x90028406,  //  0103  SETMBR	R0	K66	R6
      0xB81A1A00,  //  0104  GETNGBL	R6	K13
      0x8C180D0E,  //  0105  GETMET	R6	R6	K14
      0x88200143,  //  0106  GETMBR	R8	R0	K67
      0x8C201110,  //  0107  GETMET	R8	R8	K16
      0x7C200200,  //  0108  CALL	R8	1
      0x00228808,  //  0109  ADD	R8	K68	R8
      0x54260003,  //  010A  LDINT	R9	4
      0x7C180600,  //  010B  CALL	R6	3
      0xB81A0E00,  //  010C  GETNGBL	R6	K7
      0x8C180D45,  //  010D  GETMET	R6	R6	K69
      0x7C180200,  //  010E  CALL	R6	1
      0x881C011B,  //  010F  GETMBR	R7	R0	K27
      0x901A3607,  //  0110  SETMBR	R6	K27	R7
      0x881C0143,  //  0111  GETMBR	R7	R0	K67
      0x901A8607,  //  0112  SETMBR	R6	K67	R7
      0xB81E1A00,  //  0113  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0114  GETMET	R7	R7	K14
      0xB8260E00,  //  0115  GETNGBL	R9	K7
      0x8C241312,  //  0116  GETMET	R9	R9	K18
      0x5C2C0C00,  //  0117  MOVE	R11	R6
      0x7C240400,  //  0118  CALL	R9	2
      0x00268C09,  //  0119  ADD	R9	K70	R9
      0x542A0003,  //  011A  LDINT	R10	4
      0x7C1C0600,  //  011B  CALL	R7	3
      0x8C1C0D47,  //  011C  GETMET	R7	R6	K71
      0x7C1C0200,  //  011D  CALL	R7	1
      0xB8221A00,  //  011E  GETNGBL	R8	K13
      0x8C20110E,  //  011F  GETMET	R8	R8	K14
      0x8C280F10,  //  0120  GETMET	R10	R7	K16
      0x7C280200,  //  0121  CALL	R10	1
      0x002A900A,  //  0122  ADD	R10	K72	R10
      0x542E0003,  //  0123  LDINT	R11	4
      0x7C200600,  //  0124  CALL	R8	3
      0x8C200349,  //  0125  GETMET	R8	R1	K73
      0x542A0022,  //  0126  LDINT	R10	35
      0x502C0200,  //  0127  LDBOOL	R11	1	0
      0x7C200600,  //  0128  CALL	R8	3
      0x8C241147,  //  0129  GETMET	R9	R8	K71
      0x5C2C0E00,  //  012A  MOVE	R11	R7
      0x7C240400,  //  012B  CALL	R9	2
      0x8828014A,  //  012C  GETMBR	R10	R0	K74
      0x8C28154B,  //  012D  GETMET	R10	R10	K75
      0x5C301200,  //  012E  MOVE	R12	R9
      0x8834034C,  //  012F  GETMBR	R13	R1	K76
      0x8838034D,  //  0130  GETMBR	R14	R1	K77
      0x883C114E,  //  0131  GETMBR	R15	R8	K78
      0x7C280A00,  //  0132  CALL	R10	5
      0x80000000,  //  0133  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_session_by_destination_id
********************************************************************/
be_local_closure(Matter_Commisioning_Context_find_session_by_destination_id,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20SEARCHING_X3A_X20destinationId_X3D),
    /* K4   */  be_nested_str_weak(tohex),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(sessions),
    /* K8   */  be_nested_str_weak(noc),
    /* K9   */  be_nested_str_weak(fabric),
    /* K10  */  be_nested_str_weak(deviceid),
    /* K11  */  be_nested_str_weak(get_ca_pub),
    /* K12  */  be_nested_str_weak(get_fabric),
    /* K13  */  be_nested_str_weak(get_deviceid),
    /* K14  */  be_nested_str_weak(get_ipk_group_key),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20destinationMessage_X3D),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20key_ipk_X3D),
    /* K17  */  be_nested_str_weak(HMAC_SHA256),
    /* K18  */  be_nested_str_weak(update),
    /* K19  */  be_nested_str_weak(out),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20SIGMA1_X3A_X20candidateDestinationId_X3D),
    /* K21  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(find_session_by_destination_id),
    &be_const_str_solidified,
    ( &(const binstruction[79]) {  /* code */
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
      0x88140B07,  //  000B  GETMBR	R5	R5	K7
      0x7C100200,  //  000C  CALL	R4	1
      0xA802003B,  //  000D  EXBLK	0	#004A
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
      0x8C1C0B0C,  //  0020  GETMET	R7	R5	K12
      0x7C1C0200,  //  0021  CALL	R7	1
      0x00180C07,  //  0022  ADD	R6	R6	R7
      0x8C1C0B0D,  //  0023  GETMET	R7	R5	K13
      0x7C1C0200,  //  0024  CALL	R7	1
      0x00180C07,  //  0025  ADD	R6	R6	R7
      0x8C1C0B0E,  //  0026  GETMET	R7	R5	K14
      0x7C1C0200,  //  0027  CALL	R7	1
      0xB8220200,  //  0028  GETNGBL	R8	K1
      0x8C201102,  //  0029  GETMET	R8	R8	K2
      0x8C280D04,  //  002A  GETMET	R10	R6	K4
      0x7C280200,  //  002B  CALL	R10	1
      0x002A1E0A,  //  002C  ADD	R10	K15	R10
      0x582C0005,  //  002D  LDCONST	R11	K5
      0x7C200600,  //  002E  CALL	R8	3
      0xB8220200,  //  002F  GETNGBL	R8	K1
      0x8C201102,  //  0030  GETMET	R8	R8	K2
      0x8C280F04,  //  0031  GETMET	R10	R7	K4
      0x7C280200,  //  0032  CALL	R10	1
      0x002A200A,  //  0033  ADD	R10	K16	R10
      0x542E0003,  //  0034  LDINT	R11	4
      0x7C200600,  //  0035  CALL	R8	3
      0x8C200711,  //  0036  GETMET	R8	R3	K17
      0x5C280E00,  //  0037  MOVE	R10	R7
      0x7C200400,  //  0038  CALL	R8	2
      0x8C241112,  //  0039  GETMET	R9	R8	K18
      0x5C2C0C00,  //  003A  MOVE	R11	R6
      0x7C240400,  //  003B  CALL	R9	2
      0x8C241113,  //  003C  GETMET	R9	R8	K19
      0x7C240200,  //  003D  CALL	R9	1
      0xB82A0200,  //  003E  GETNGBL	R10	K1
      0x8C281502,  //  003F  GETMET	R10	R10	K2
      0x8C301304,  //  0040  GETMET	R12	R9	K4
      0x7C300200,  //  0041  CALL	R12	1
      0x0032280C,  //  0042  ADD	R12	K20	R12
      0x58340005,  //  0043  LDCONST	R13	K5
      0x7C280600,  //  0044  CALL	R10	3
      0x1C281201,  //  0045  EQ	R10	R9	R1
      0x782A0001,  //  0046  JMPF	R10	#0049
      0xA8040001,  //  0047  EXBLK	1	1
      0x80040A00,  //  0048  RET	1	R5
      0x7001FFC3,  //  0049  JMP		#000E
      0x58100015,  //  004A  LDCONST	R4	K21
      0xAC100200,  //  004B  CATCH	R4	1	0
      0xB0080000,  //  004C  RAISE	2	R0	R0
      0x4C100000,  //  004D  LDNIL	R4
      0x80040800,  //  004E  RET	1	R4
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
    38,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[94]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(protocol_error),
    /* K6   */  be_nested_str_weak(invalid_X20Pake1_X20message),
    /* K7   */  be_nested_str_weak(session),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(Sigma3),
    /* K10  */  be_nested_str_weak(parse),
    /* K11  */  be_nested_str_weak(raw),
    /* K12  */  be_nested_str_weak(app_payload_idx),
    /* K13  */  be_nested_str_weak(tasmota),
    /* K14  */  be_nested_str_weak(log),
    /* K15  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K16  */  be_nested_str_weak(SHA256),
    /* K17  */  be_nested_str_weak(update),
    /* K18  */  be_nested_str_weak(_Msg1),
    /* K19  */  be_nested_str_weak(_Msg2),
    /* K20  */  be_nested_str_weak(out),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20session_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20session_X2Eipk_epoch_key_X20),
    /* K23  */  be_nested_str_weak(ipk_epoch_key),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20session_X2Efabric_compressed_X20),
    /* K25  */  be_nested_str_weak(fabric_compressed),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20ipk_group_key_X20_X3D_X20),
    /* K27  */  be_nested_str_weak(get_ipk_group_key),
    /* K28  */  be_nested_str_weak(tohex),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TranscriptHash_X3D_X20),
    /* K30  */  be_nested_str_weak(fromstring),
    /* K31  */  be_nested_str_weak(S3K_Info),
    /* K32  */  be_nested_str_weak(HKDF_SHA256),
    /* K33  */  be_nested_str_weak(derive),
    /* K34  */  be_nested_str_weak(shared_secret),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s3k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K36  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s3k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K37  */  be_nested_str_weak(TBEData3Encrypted),
    /* K38  */  be_const_int(2147483647),
    /* K39  */  be_nested_str_weak(AES_CCM),
    /* K40  */  be_nested_str_weak(TBEData3_Nonce),
    /* K41  */  be_nested_str_weak(decrypt),
    /* K42  */  be_nested_str_weak(tag),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData3_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K44  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBETag3_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20tag_sent_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K46  */  be_nested_str_weak(value_error),
    /* K47  */  be_nested_str_weak(tag_X20do_X20not_X20match),
    /* K48  */  be_nested_str_weak(TLV),
    /* K49  */  be_nested_str_weak(findsubval),
    /* K50  */  be_const_int(1),
    /* K51  */  be_const_int(2),
    /* K52  */  be_const_int(3),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20initiatorNOCTLV_X20_X3D_X20),
    /* K54  */  be_nested_str_weak(findsub),
    /* K55  */  be_nested_str_weak(int),
    /* K56  */  be_nested_str_weak(int64),
    /* K57  */  be_nested_str_weak(peer_node_id),
    /* K58  */  be_nested_str_weak(tobytes),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20initiatorFabricId_X3D),
    /* K60  */  be_nested_str_weak(Matter_TLV_struct),
    /* K61  */  be_nested_str_weak(add_TLV),
    /* K62  */  be_nested_str_weak(B1),
    /* K63  */  be_nested_str_weak(initiatorEph_pub),
    /* K64  */  be_nested_str_weak(ResponderEph_pub),
    /* K65  */  be_nested_str_weak(encode),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20initiatorNOCPubKey_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K67  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20ec_signature_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K68  */  be_nested_str_weak(EC_P256),
    /* K69  */  be_nested_str_weak(ecdsa_verify_sha256),
    /* K70  */  be_nested_str_weak(sigma3_tbs_X20does_X20not_X20have_X20a_X20valid_X20signature),
    /* K71  */  be_nested_str_weak(MTR_X3A_X20Sigma3_X20verified_X2C_X20computing_X20new_X20keys),
    /* K72  */  be_nested_str_weak(Msg3),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K74  */  be_nested_str_weak(MTR_X3A_X20shared_secret_X20_X3D),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20ipk_X20_X2B_X20hash_X20_X20_X20_X20_X3D),
    /* K76  */  be_nested_str_weak(SEKeys_Info),
    /* K77  */  be_nested_str_weak(rtc),
    /* K78  */  be_nested_str_weak(utc),
    /* K79  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K80  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K81  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K82  */  be_nested_str_weak(build_response),
    /* K83  */  be_nested_str_weak(add),
    /* K84  */  be_nested_str_weak(responder),
    /* K85  */  be_nested_str_weak(send_response),
    /* K86  */  be_nested_str_weak(remote_ip),
    /* K87  */  be_nested_str_weak(remote_port),
    /* K88  */  be_nested_str_weak(message_counter),
    /* K89  */  be_nested_str_weak(close),
    /* K90  */  be_nested_str_weak(set_keys),
    /* K91  */  be_nested_str_weak(set_persist),
    /* K92  */  be_nested_str_weak(set_no_expiration),
    /* K93  */  be_nested_str_weak(save),
    }),
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[445]) {  /* code */
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
      0x780E0000,  //  000A  JMPF	R3	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0x880C0307,  //  000C  GETMBR	R3	R1	K7
      0xB8121000,  //  000D  GETNGBL	R4	K8
      0x8C100909,  //  000E  GETMET	R4	R4	K9
      0x7C100200,  //  000F  CALL	R4	1
      0x8C10090A,  //  0010  GETMET	R4	R4	K10
      0x8818030B,  //  0011  GETMBR	R6	R1	K11
      0x881C030C,  //  0012  GETMBR	R7	R1	K12
      0x7C100600,  //  0013  CALL	R4	3
      0xB8161A00,  //  0014  GETNGBL	R5	K13
      0x8C140B0E,  //  0015  GETMET	R5	R5	K14
      0x581C000F,  //  0016  LDCONST	R7	K15
      0x54220003,  //  0017  LDINT	R8	4
      0x7C140600,  //  0018  CALL	R5	3
      0x8C140510,  //  0019  GETMET	R5	R2	K16
      0x7C140200,  //  001A  CALL	R5	1
      0x8C140B11,  //  001B  GETMET	R5	R5	K17
      0x881C0712,  //  001C  GETMBR	R7	R3	K18
      0x7C140400,  //  001D  CALL	R5	2
      0x8C140B11,  //  001E  GETMET	R5	R5	K17
      0x881C0713,  //  001F  GETMBR	R7	R3	K19
      0x7C140400,  //  0020  CALL	R5	2
      0x8C140B14,  //  0021  GETMET	R5	R5	K20
      0x7C140200,  //  0022  CALL	R5	1
      0xB81A1A00,  //  0023  GETNGBL	R6	K13
      0x8C180D0E,  //  0024  GETMET	R6	R6	K14
      0x60200008,  //  0025  GETGBL	R8	G8
      0x5C240600,  //  0026  MOVE	R9	R3
      0x7C200200,  //  0027  CALL	R8	1
      0x00222A08,  //  0028  ADD	R8	K21	R8
      0x54260003,  //  0029  LDINT	R9	4
      0x7C180600,  //  002A  CALL	R6	3
      0xB81A1A00,  //  002B  GETNGBL	R6	K13
      0x8C180D0E,  //  002C  GETMET	R6	R6	K14
      0x60200008,  //  002D  GETGBL	R8	G8
      0x88240717,  //  002E  GETMBR	R9	R3	K23
      0x7C200200,  //  002F  CALL	R8	1
      0x00222C08,  //  0030  ADD	R8	K22	R8
      0x54260003,  //  0031  LDINT	R9	4
      0x7C180600,  //  0032  CALL	R6	3
      0xB81A1A00,  //  0033  GETNGBL	R6	K13
      0x8C180D0E,  //  0034  GETMET	R6	R6	K14
      0x60200008,  //  0035  GETGBL	R8	G8
      0x88240719,  //  0036  GETMBR	R9	R3	K25
      0x7C200200,  //  0037  CALL	R8	1
      0x00223008,  //  0038  ADD	R8	K24	R8
      0x54260003,  //  0039  LDINT	R9	4
      0x7C180600,  //  003A  CALL	R6	3
      0xB81A1A00,  //  003B  GETNGBL	R6	K13
      0x8C180D0E,  //  003C  GETMET	R6	R6	K14
      0x8C20071B,  //  003D  GETMET	R8	R3	K27
      0x7C200200,  //  003E  CALL	R8	1
      0x8C20111C,  //  003F  GETMET	R8	R8	K28
      0x7C200200,  //  0040  CALL	R8	1
      0x00223408,  //  0041  ADD	R8	K26	R8
      0x54260003,  //  0042  LDINT	R9	4
      0x7C180600,  //  0043  CALL	R6	3
      0xB81A1A00,  //  0044  GETNGBL	R6	K13
      0x8C180D0E,  //  0045  GETMET	R6	R6	K14
      0x8C200B1C,  //  0046  GETMET	R8	R5	K28
      0x7C200200,  //  0047  CALL	R8	1
      0x00223A08,  //  0048  ADD	R8	K29	R8
      0x54260003,  //  0049  LDINT	R9	4
      0x7C180600,  //  004A  CALL	R6	3
      0x60180015,  //  004B  GETGBL	R6	G21
      0x7C180000,  //  004C  CALL	R6	0
      0x8C180D1E,  //  004D  GETMET	R6	R6	K30
      0x8820011F,  //  004E  GETMBR	R8	R0	K31
      0x7C180400,  //  004F  CALL	R6	2
      0x8C1C0520,  //  0050  GETMET	R7	R2	K32
      0x7C1C0200,  //  0051  CALL	R7	1
      0x8C1C0F21,  //  0052  GETMET	R7	R7	K33
      0x88240722,  //  0053  GETMBR	R9	R3	K34
      0x8C28071B,  //  0054  GETMET	R10	R3	K27
      0x7C280200,  //  0055  CALL	R10	1
      0x00281405,  //  0056  ADD	R10	R10	R5
      0x5C2C0C00,  //  0057  MOVE	R11	R6
      0x5432000F,  //  0058  LDINT	R12	16
      0x7C1C0A00,  //  0059  CALL	R7	5
      0xB8221A00,  //  005A  GETNGBL	R8	K13
      0x8C20110E,  //  005B  GETMET	R8	R8	K14
      0x5828000F,  //  005C  LDCONST	R10	K15
      0x542E0003,  //  005D  LDINT	R11	4
      0x7C200600,  //  005E  CALL	R8	3
      0xB8221A00,  //  005F  GETNGBL	R8	K13
      0x8C20110E,  //  0060  GETMET	R8	R8	K14
      0x8C28071B,  //  0061  GETMET	R10	R3	K27
      0x7C280200,  //  0062  CALL	R10	1
      0x00281405,  //  0063  ADD	R10	R10	R5
      0x8C28151C,  //  0064  GETMET	R10	R10	K28
      0x7C280200,  //  0065  CALL	R10	1
      0x002A460A,  //  0066  ADD	R10	K35	R10
      0x542E0003,  //  0067  LDINT	R11	4
      0x7C200600,  //  0068  CALL	R8	3
      0xB8221A00,  //  0069  GETNGBL	R8	K13
      0x8C20110E,  //  006A  GETMET	R8	R8	K14
      0x8C280F1C,  //  006B  GETMET	R10	R7	K28
      0x7C280200,  //  006C  CALL	R10	1
      0x002A480A,  //  006D  ADD	R10	K36	R10
      0x542E0003,  //  006E  LDINT	R11	4
      0x7C200600,  //  006F  CALL	R8	3
      0xB8221A00,  //  0070  GETNGBL	R8	K13
      0x8C20110E,  //  0071  GETMET	R8	R8	K14
      0x5828000F,  //  0072  LDCONST	R10	K15
      0x542E0003,  //  0073  LDINT	R11	4
      0x7C200600,  //  0074  CALL	R8	3
      0x5421FFEE,  //  0075  LDINT	R8	-17
      0x40220608,  //  0076  CONNECT	R8	K3	R8
      0x88240925,  //  0077  GETMBR	R9	R4	K37
      0x94201208,  //  0078  GETIDX	R8	R9	R8
      0x5429FFEF,  //  0079  LDINT	R10	-16
      0x40281526,  //  007A  CONNECT	R10	R10	K38
      0x882C0925,  //  007B  GETMBR	R11	R4	K37
      0x9424160A,  //  007C  GETIDX	R9	R11	R10
      0x8C300527,  //  007D  GETMET	R12	R2	K39
      0x5C380E00,  //  007E  MOVE	R14	R7
      0x603C0015,  //  007F  GETGBL	R15	G21
      0x7C3C0000,  //  0080  CALL	R15	0
      0x8C3C1F1E,  //  0081  GETMET	R15	R15	K30
      0x88440128,  //  0082  GETMBR	R17	R0	K40
      0x7C3C0400,  //  0083  CALL	R15	2
      0x60400015,  //  0084  GETGBL	R16	G21
      0x7C400000,  //  0085  CALL	R16	0
      0x6044000C,  //  0086  GETGBL	R17	G12
      0x5C481000,  //  0087  MOVE	R18	R8
      0x7C440200,  //  0088  CALL	R17	1
      0x544A000F,  //  0089  LDINT	R18	16
      0x7C300C00,  //  008A  CALL	R12	6
      0x5C281800,  //  008B  MOVE	R10	R12
      0x8C301529,  //  008C  GETMET	R12	R10	K41
      0x5C381000,  //  008D  MOVE	R14	R8
      0x7C300400,  //  008E  CALL	R12	2
      0x5C2C1800,  //  008F  MOVE	R11	R12
      0x8C30152A,  //  0090  GETMET	R12	R10	K42
      0x7C300200,  //  0091  CALL	R12	1
      0xB8361A00,  //  0092  GETNGBL	R13	K13
      0x8C341B0E,  //  0093  GETMET	R13	R13	K14
      0x8C3C171C,  //  0094  GETMET	R15	R11	K28
      0x7C3C0200,  //  0095  CALL	R15	1
      0x003E560F,  //  0096  ADD	R15	K43	R15
      0x54420003,  //  0097  LDINT	R16	4
      0x7C340600,  //  0098  CALL	R13	3
      0xB8361A00,  //  0099  GETNGBL	R13	K13
      0x8C341B0E,  //  009A  GETMET	R13	R13	K14
      0x8C3C191C,  //  009B  GETMET	R15	R12	K28
      0x7C3C0200,  //  009C  CALL	R15	1
      0x003E580F,  //  009D  ADD	R15	K44	R15
      0x54420003,  //  009E  LDINT	R16	4
      0x7C340600,  //  009F  CALL	R13	3
      0xB8361A00,  //  00A0  GETNGBL	R13	K13
      0x8C341B0E,  //  00A1  GETMET	R13	R13	K14
      0x8C3C131C,  //  00A2  GETMET	R15	R9	K28
      0x7C3C0200,  //  00A3  CALL	R15	1
      0x003E5A0F,  //  00A4  ADD	R15	K45	R15
      0x54420003,  //  00A5  LDINT	R16	4
      0x7C340600,  //  00A6  CALL	R13	3
      0xB8361A00,  //  00A7  GETNGBL	R13	K13
      0x8C341B0E,  //  00A8  GETMET	R13	R13	K14
      0x583C000F,  //  00A9  LDCONST	R15	K15
      0x54420003,  //  00AA  LDINT	R16	4
      0x7C340600,  //  00AB  CALL	R13	3
      0x20341809,  //  00AC  NE	R13	R12	R9
      0x78360000,  //  00AD  JMPF	R13	#00AF
      0xB0065D2F,  //  00AE  RAISE	1	K46	K47
      0xB8361000,  //  00AF  GETNGBL	R13	K8
      0x88341B30,  //  00B0  GETMBR	R13	R13	K48
      0x8C341B0A,  //  00B1  GETMET	R13	R13	K10
      0x5C3C1600,  //  00B2  MOVE	R15	R11
      0x7C340400,  //  00B3  CALL	R13	2
      0x8C381B31,  //  00B4  GETMET	R14	R13	K49
      0x58400032,  //  00B5  LDCONST	R16	K50
      0x7C380400,  //  00B6  CALL	R14	2
      0x8C3C1B31,  //  00B7  GETMET	R15	R13	K49
      0x58440033,  //  00B8  LDCONST	R17	K51
      0x7C3C0400,  //  00B9  CALL	R15	2
      0x8C401B31,  //  00BA  GETMET	R16	R13	K49
      0x58480034,  //  00BB  LDCONST	R18	K52
      0x7C400400,  //  00BC  CALL	R16	2
      0xB8461000,  //  00BD  GETNGBL	R17	K8
      0x88442330,  //  00BE  GETMBR	R17	R17	K48
      0x8C44230A,  //  00BF  GETMET	R17	R17	K10
      0x5C4C1C00,  //  00C0  MOVE	R19	R14
      0x7C440400,  //  00C1  CALL	R17	2
      0xB84A1A00,  //  00C2  GETNGBL	R18	K13
      0x8C48250E,  //  00C3  GETMET	R18	R18	K14
      0x60500008,  //  00C4  GETGBL	R20	G8
      0x5C542200,  //  00C5  MOVE	R21	R17
      0x7C500200,  //  00C6  CALL	R20	1
      0x00526A14,  //  00C7  ADD	R20	K53	R20
      0x58540034,  //  00C8  LDCONST	R21	K52
      0x7C480600,  //  00C9  CALL	R18	3
      0x8C482331,  //  00CA  GETMET	R18	R17	K49
      0x54520008,  //  00CB  LDINT	R20	9
      0x7C480400,  //  00CC  CALL	R18	2
      0x8C4C2336,  //  00CD  GETMET	R19	R17	K54
      0x54560005,  //  00CE  LDINT	R21	6
      0x7C4C0400,  //  00CF  CALL	R19	2
      0x8C502731,  //  00D0  GETMET	R20	R19	K49
      0x545A0010,  //  00D1  LDINT	R22	17
      0x7C500400,  //  00D2  CALL	R20	2
      0x60540004,  //  00D3  GETGBL	R21	G4
      0x5C582800,  //  00D4  MOVE	R22	R20
      0x7C540200,  //  00D5  CALL	R21	1
      0x1C542B37,  //  00D6  EQ	R21	R21	K55
      0x78560003,  //  00D7  JMPF	R21	#00DC
      0xB8567000,  //  00D8  GETNGBL	R21	K56
      0x5C582800,  //  00D9  MOVE	R22	R20
      0x7C540200,  //  00DA  CALL	R21	1
      0x5C502A00,  //  00DB  MOVE	R20	R21
      0x8C54293A,  //  00DC  GETMET	R21	R20	K58
      0x7C540200,  //  00DD  CALL	R21	1
      0x900E7215,  //  00DE  SETMBR	R3	K57	R21
      0xB8561A00,  //  00DF  GETNGBL	R21	K13
      0x8C542B0E,  //  00E0  GETMET	R21	R21	K14
      0x605C0008,  //  00E1  GETGBL	R23	G8
      0x88600739,  //  00E2  GETMBR	R24	R3	K57
      0x7C5C0200,  //  00E3  CALL	R23	1
      0x005E7617,  //  00E4  ADD	R23	K59	R23
      0x58600034,  //  00E5  LDCONST	R24	K52
      0x7C540600,  //  00E6  CALL	R21	3
      0xB8561000,  //  00E7  GETNGBL	R21	K8
      0x88542B30,  //  00E8  GETMBR	R21	R21	K48
      0x8C542B3C,  //  00E9  GETMET	R21	R21	K60
      0x7C540200,  //  00EA  CALL	R21	1
      0x8C582B3D,  //  00EB  GETMET	R22	R21	K61
      0x58600032,  //  00EC  LDCONST	R24	K50
      0xB8661000,  //  00ED  GETNGBL	R25	K8
      0x88643330,  //  00EE  GETMBR	R25	R25	K48
      0x8864333E,  //  00EF  GETMBR	R25	R25	K62
      0x5C681C00,  //  00F0  MOVE	R26	R14
      0x7C580800,  //  00F1  CALL	R22	4
      0x8C582B3D,  //  00F2  GETMET	R22	R21	K61
      0x58600033,  //  00F3  LDCONST	R24	K51
      0xB8661000,  //  00F4  GETNGBL	R25	K8
      0x88643330,  //  00F5  GETMBR	R25	R25	K48
      0x8864333E,  //  00F6  GETMBR	R25	R25	K62
      0x5C681E00,  //  00F7  MOVE	R26	R15
      0x7C580800,  //  00F8  CALL	R22	4
      0x8C582B3D,  //  00F9  GETMET	R22	R21	K61
      0x58600034,  //  00FA  LDCONST	R24	K52
      0xB8661000,  //  00FB  GETNGBL	R25	K8
      0x88643330,  //  00FC  GETMBR	R25	R25	K48
      0x8864333E,  //  00FD  GETMBR	R25	R25	K62
      0x8868013F,  //  00FE  GETMBR	R26	R0	K63
      0x7C580800,  //  00FF  CALL	R22	4
      0x8C582B3D,  //  0100  GETMET	R22	R21	K61
      0x54620003,  //  0101  LDINT	R24	4
      0xB8661000,  //  0102  GETNGBL	R25	K8
      0x88643330,  //  0103  GETMBR	R25	R25	K48
      0x8864333E,  //  0104  GETMBR	R25	R25	K62
      0x88680140,  //  0105  GETMBR	R26	R0	K64
      0x7C580800,  //  0106  CALL	R22	4
      0x8C582B41,  //  0107  GETMET	R22	R21	K65
      0x7C580200,  //  0108  CALL	R22	1
      0xB85E1A00,  //  0109  GETNGBL	R23	K13
      0x8C5C2F0E,  //  010A  GETMET	R23	R23	K14
      0x8C64251C,  //  010B  GETMET	R25	R18	K28
      0x7C640200,  //  010C  CALL	R25	1
      0x00668419,  //  010D  ADD	R25	K66	R25
      0x546A0003,  //  010E  LDINT	R26	4
      0x7C5C0600,  //  010F  CALL	R23	3
      0xB85E1A00,  //  0110  GETNGBL	R23	K13
      0x8C5C2F0E,  //  0111  GETMET	R23	R23	K14
      0x8C64211C,  //  0112  GETMET	R25	R16	K28
      0x7C640200,  //  0113  CALL	R25	1
      0x00668619,  //  0114  ADD	R25	K67	R25
      0x546A0003,  //  0115  LDINT	R26	4
      0x7C5C0600,  //  0116  CALL	R23	3
      0xB85E1A00,  //  0117  GETNGBL	R23	K13
      0x8C5C2F0E,  //  0118  GETMET	R23	R23	K14
      0x5864000F,  //  0119  LDCONST	R25	K15
      0x546A0003,  //  011A  LDINT	R26	4
      0x7C5C0600,  //  011B  CALL	R23	3
      0x8C5C0544,  //  011C  GETMET	R23	R2	K68
      0x7C5C0200,  //  011D  CALL	R23	1
      0x8C5C2F45,  //  011E  GETMET	R23	R23	K69
      0x5C642400,  //  011F  MOVE	R25	R18
      0x5C682C00,  //  0120  MOVE	R26	R22
      0x5C6C2000,  //  0121  MOVE	R27	R16
      0x7C5C0800,  //  0122  CALL	R23	4
      0x5C602E00,  //  0123  MOVE	R24	R23
      0x74620000,  //  0124  JMPT	R24	#0126
      0xB0065D46,  //  0125  RAISE	1	K46	K70
      0xB8621A00,  //  0126  GETNGBL	R24	K13
      0x8C60310E,  //  0127  GETMET	R24	R24	K14
      0x58680047,  //  0128  LDCONST	R26	K71
      0x586C0034,  //  0129  LDCONST	R27	K52
      0x7C600600,  //  012A  CALL	R24	3
      0x8C600510,  //  012B  GETMET	R24	R2	K16
      0x7C600200,  //  012C  CALL	R24	1
      0x8C603111,  //  012D  GETMET	R24	R24	K17
      0x88680712,  //  012E  GETMBR	R26	R3	K18
      0x7C600400,  //  012F  CALL	R24	2
      0x8C603111,  //  0130  GETMET	R24	R24	K17
      0x88680713,  //  0131  GETMBR	R26	R3	K19
      0x7C600400,  //  0132  CALL	R24	2
      0x8C603111,  //  0133  GETMET	R24	R24	K17
      0x88680948,  //  0134  GETMBR	R26	R4	K72
      0x7C600400,  //  0135  CALL	R24	2
      0x8C603114,  //  0136  GETMET	R24	R24	K20
      0x7C600200,  //  0137  CALL	R24	1
      0x5C143000,  //  0138  MOVE	R5	R24
      0x4C600000,  //  0139  LDNIL	R24
      0x900E2418,  //  013A  SETMBR	R3	K18	R24
      0x4C600000,  //  013B  LDNIL	R24
      0x900E2618,  //  013C  SETMBR	R3	K19	R24
      0xB8621A00,  //  013D  GETNGBL	R24	K13
      0x8C60310E,  //  013E  GETMET	R24	R24	K14
      0x58680049,  //  013F  LDCONST	R26	K73
      0x546E0003,  //  0140  LDINT	R27	4
      0x7C600600,  //  0141  CALL	R24	3
      0xB8621A00,  //  0142  GETNGBL	R24	K13
      0x8C60310E,  //  0143  GETMET	R24	R24	K14
      0x88680722,  //  0144  GETMBR	R26	R3	K34
      0x8C68351C,  //  0145  GETMET	R26	R26	K28
      0x7C680200,  //  0146  CALL	R26	1
      0x006A941A,  //  0147  ADD	R26	K74	R26
      0x546E0003,  //  0148  LDINT	R27	4
      0x7C600600,  //  0149  CALL	R24	3
      0xB8621A00,  //  014A  GETNGBL	R24	K13
      0x8C60310E,  //  014B  GETMET	R24	R24	K14
      0x8C68071B,  //  014C  GETMET	R26	R3	K27
      0x7C680200,  //  014D  CALL	R26	1
      0x00683405,  //  014E  ADD	R26	R26	R5
      0x8C68351C,  //  014F  GETMET	R26	R26	K28
      0x7C680200,  //  0150  CALL	R26	1
      0x006A961A,  //  0151  ADD	R26	K75	R26
      0x546E0003,  //  0152  LDINT	R27	4
      0x7C600600,  //  0153  CALL	R24	3
      0x8C600520,  //  0154  GETMET	R24	R2	K32
      0x7C600200,  //  0155  CALL	R24	1
      0x8C603121,  //  0156  GETMET	R24	R24	K33
      0x88680722,  //  0157  GETMBR	R26	R3	K34
      0x8C6C071B,  //  0158  GETMET	R27	R3	K27
      0x7C6C0200,  //  0159  CALL	R27	1
      0x006C3605,  //  015A  ADD	R27	R27	R5
      0x60700015,  //  015B  GETGBL	R28	G21
      0x7C700000,  //  015C  CALL	R28	0
      0x8C70391E,  //  015D  GETMET	R28	R28	K30
      0x8878014C,  //  015E  GETMBR	R30	R0	K76
      0x7C700400,  //  015F  CALL	R28	2
      0x5476002F,  //  0160  LDINT	R29	48
      0x7C600A00,  //  0161  CALL	R24	5
      0x5466000E,  //  0162  LDINT	R25	15
      0x40660619,  //  0163  CONNECT	R25	K3	R25
      0x94643019,  //  0164  GETIDX	R25	R24	R25
      0x546A000F,  //  0165  LDINT	R26	16
      0x546E001E,  //  0166  LDINT	R27	31
      0x4068341B,  //  0167  CONNECT	R26	R26	R27
      0x9468301A,  //  0168  GETIDX	R26	R24	R26
      0x546E001F,  //  0169  LDINT	R27	32
      0x5472002E,  //  016A  LDINT	R28	47
      0x406C361C,  //  016B  CONNECT	R27	R27	R28
      0x946C301B,  //  016C  GETIDX	R27	R24	R27
      0xB8721A00,  //  016D  GETNGBL	R28	K13
      0x8C70394D,  //  016E  GETMET	R28	R28	K77
      0x7C700200,  //  016F  CALL	R28	1
      0x9470394E,  //  0170  GETIDX	R28	R28	K78
      0xB8761A00,  //  0171  GETNGBL	R29	K13
      0x8C743B0E,  //  0172  GETMET	R29	R29	K14
      0x587C0049,  //  0173  LDCONST	R31	K73
      0x54820003,  //  0174  LDINT	R32	4
      0x7C740600,  //  0175  CALL	R29	3
      0xB8761A00,  //  0176  GETNGBL	R29	K13
      0x8C743B0E,  //  0177  GETMET	R29	R29	K14
      0x8C7C331C,  //  0178  GETMET	R31	R25	K28
      0x7C7C0200,  //  0179  CALL	R31	1
      0x007E9E1F,  //  017A  ADD	R31	K79	R31
      0x54820003,  //  017B  LDINT	R32	4
      0x7C740600,  //  017C  CALL	R29	3
      0xB8761A00,  //  017D  GETNGBL	R29	K13
      0x8C743B0E,  //  017E  GETMET	R29	R29	K14
      0x8C7C351C,  //  017F  GETMET	R31	R26	K28
      0x7C7C0200,  //  0180  CALL	R31	1
      0x007EA01F,  //  0181  ADD	R31	K80	R31
      0x54820003,  //  0182  LDINT	R32	4
      0x7C740600,  //  0183  CALL	R29	3
      0xB8761A00,  //  0184  GETNGBL	R29	K13
      0x8C743B0E,  //  0185  GETMET	R29	R29	K14
      0x8C7C371C,  //  0186  GETMET	R31	R27	K28
      0x7C7C0200,  //  0187  CALL	R31	1
      0x007EA21F,  //  0188  ADD	R31	K81	R31
      0x54820003,  //  0189  LDINT	R32	4
      0x7C740600,  //  018A  CALL	R29	3
      0xB8761A00,  //  018B  GETNGBL	R29	K13
      0x8C743B0E,  //  018C  GETMET	R29	R29	K14
      0x587C0049,  //  018D  LDCONST	R31	K73
      0x54820003,  //  018E  LDINT	R32	4
      0x7C740600,  //  018F  CALL	R29	3
      0x8C740352,  //  0190  GETMET	R29	R1	K82
      0x547E003F,  //  0191  LDINT	R31	64
      0x50800200,  //  0192  LDBOOL	R32	1	0
      0x7C740600,  //  0193  CALL	R29	3
      0x60780015,  //  0194  GETGBL	R30	G21
      0x7C780000,  //  0195  CALL	R30	0
      0x8C7C3D53,  //  0196  GETMET	R31	R30	K83
      0x58840003,  //  0197  LDCONST	R33	K3
      0x58880033,  //  0198  LDCONST	R34	K51
      0x7C7C0600,  //  0199  CALL	R31	3
      0x8C7C3D53,  //  019A  GETMET	R31	R30	K83
      0x58840003,  //  019B  LDCONST	R33	K3
      0x548A0003,  //  019C  LDINT	R34	4
      0x7C7C0600,  //  019D  CALL	R31	3
      0x8C7C3D53,  //  019E  GETMET	R31	R30	K83
      0x58840003,  //  019F  LDCONST	R33	K3
      0x548A0003,  //  01A0  LDINT	R34	4
      0x7C7C0600,  //  01A1  CALL	R31	3
      0x8C7C3B41,  //  01A2  GETMET	R31	R29	K65
      0x5C843C00,  //  01A3  MOVE	R33	R30
      0x7C7C0400,  //  01A4  CALL	R31	2
      0x88800154,  //  01A5  GETMBR	R32	R0	K84
      0x8C804155,  //  01A6  GETMET	R32	R32	K85
      0x5C883E00,  //  01A7  MOVE	R34	R31
      0x888C0356,  //  01A8  GETMBR	R35	R1	K86
      0x88900357,  //  01A9  GETMBR	R36	R1	K87
      0x88943B58,  //  01AA  GETMBR	R37	R29	K88
      0x7C800A00,  //  01AB  CALL	R32	5
      0x8C800759,  //  01AC  GETMET	R32	R3	K89
      0x7C800200,  //  01AD  CALL	R32	1
      0x8C80075A,  //  01AE  GETMET	R32	R3	K90
      0x5C883200,  //  01AF  MOVE	R34	R25
      0x5C8C3400,  //  01B0  MOVE	R35	R26
      0x5C903600,  //  01B1  MOVE	R36	R27
      0x5C943800,  //  01B2  MOVE	R37	R28
      0x7C800A00,  //  01B3  CALL	R32	5
      0x8C80075B,  //  01B4  GETMET	R32	R3	K91
      0x50880200,  //  01B5  LDBOOL	R34	1	0
      0x7C800400,  //  01B6  CALL	R32	2
      0x8C80075C,  //  01B7  GETMET	R32	R3	K92
      0x7C800200,  //  01B8  CALL	R32	1
      0x8C80075D,  //  01B9  GETMET	R32	R3	K93
      0x7C800200,  //  01BA  CALL	R32	1
      0x50800200,  //  01BB  LDBOOL	R32	1	0
      0x80044000,  //  01BC  RET	1	R32
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Sigma1
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Sigma1,   /* name */
  be_nested_proto(
    35,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[118]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(protocol_error),
    /* K6   */  be_nested_str_weak(invalid_X20Pake1_X20message),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(Sigma1),
    /* K9   */  be_nested_str_weak(parse),
    /* K10  */  be_nested_str_weak(raw),
    /* K11  */  be_nested_str_weak(app_payload_idx),
    /* K12  */  be_nested_str_weak(initiatorEph_pub),
    /* K13  */  be_nested_str_weak(initiatorEphPubKey),
    /* K14  */  be_nested_str_weak(resumptionID),
    /* K15  */  be_nested_str_weak(initiatorResumeMIC),
    /* K16  */  be_nested_str_weak(device),
    /* K17  */  be_nested_str_weak(sessions),
    /* K18  */  be_nested_str_weak(find_session_by_resumption_id),
    /* K19  */  be_nested_str_weak(find_session_by_destination_id),
    /* K20  */  be_nested_str_weak(destinationId),
    /* K21  */  be_nested_str_weak(initiatorRandom),
    /* K22  */  be_nested_str_weak(valuer_error),
    /* K23  */  be_nested_str_weak(StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K24  */  be_nested_str_weak(source_node_id),
    /* K25  */  be_nested_str_weak(set_mode),
    /* K26  */  be_nested_str_weak(Session),
    /* K27  */  be_nested_str_weak(__CASE),
    /* K28  */  be_nested_str_weak(session),
    /* K29  */  be_nested_str_weak(remove_session),
    /* K30  */  be_nested_str_weak(_future_initiator_session_id),
    /* K31  */  be_nested_str_weak(initiator_session_id),
    /* K32  */  be_nested_str_weak(_future_local_session_id),
    /* K33  */  be_nested_str_weak(gen_local_session_id),
    /* K34  */  be_nested_str_weak(future_local_session_id),
    /* K35  */  be_nested_str_weak(shared_secret),
    /* K36  */  be_nested_str_weak(fromstring),
    /* K37  */  be_nested_str_weak(Sigma1_Resume),
    /* K38  */  be_nested_str_weak(HKDF_SHA256),
    /* K39  */  be_nested_str_weak(derive),
    /* K40  */  be_nested_str_weak(NCASE_SigmaR1),
    /* K41  */  be_const_int(2147483647),
    /* K42  */  be_nested_str_weak(AES_CCM),
    /* K43  */  be_nested_str_weak(decrypt),
    /* K44  */  be_nested_str_weak(tag),
    /* K45  */  be_nested_str_weak(tasmota),
    /* K46  */  be_nested_str_weak(log),
    /* K47  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K48  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s1rk_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K49  */  be_nested_str_weak(tohex),
    /* K50  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20tag_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K51  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20Resume1MICPayload_X20_X3D_X20),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20decrypted_tag_X20_X20_X20_X20_X20_X3D_X20),
    /* K53  */  be_nested_str_weak(resumption_id),
    /* K54  */  be_nested_str_weak(random),
    /* K55  */  be_nested_str_weak(Sigma2_Resume),
    /* K56  */  be_nested_str_weak(NCASE_SigmaR2),
    /* K57  */  be_nested_str_weak(Sigma2Resume),
    /* K58  */  be_nested_str_weak(responderSessionID),
    /* K59  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K60  */  be_nested_str_weak(SessionResumptionKeys),
    /* K61  */  be_nested_str_weak(rtc),
    /* K62  */  be_nested_str_weak(utc),
    /* K63  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K64  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K65  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K67  */  be_nested_str_weak(encode),
    /* K68  */  be_nested_str_weak(_Msg1),
    /* K69  */  be_nested_str_weak(MTR_X3A_X20sigma2resume_raw_X3A_X20),
    /* K70  */  be_nested_str_weak(build_response),
    /* K71  */  be_nested_str_weak(responder),
    /* K72  */  be_nested_str_weak(send_response),
    /* K73  */  be_nested_str_weak(remote_ip),
    /* K74  */  be_nested_str_weak(remote_port),
    /* K75  */  be_nested_str_weak(message_counter),
    /* K76  */  be_nested_str_weak(set_keys),
    /* K77  */  be_nested_str_weak(set_persist),
    /* K78  */  be_nested_str_weak(set_no_expiration),
    /* K79  */  be_nested_str_weak(save),
    /* K80  */  be_nested_str_weak(ResponderEph_priv),
    /* K81  */  be_nested_str_weak(ResponderEph_pub),
    /* K82  */  be_nested_str_weak(EC_P256),
    /* K83  */  be_nested_str_weak(public_key),
    /* K84  */  be_nested_str_weak(shared_key),
    /* K85  */  be_nested_str_weak(TLV),
    /* K86  */  be_nested_str_weak(Matter_TLV_struct),
    /* K87  */  be_nested_str_weak(add_TLV),
    /* K88  */  be_const_int(1),
    /* K89  */  be_nested_str_weak(B2),
    /* K90  */  be_nested_str_weak(get_noc),
    /* K91  */  be_const_int(2),
    /* K92  */  be_nested_str_weak(get_icac),
    /* K93  */  be_const_int(3),
    /* K94  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K95  */  be_nested_str_weak(get_pk),
    /* K96  */  be_nested_str_weak(Msg1),
    /* K97  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20MSG1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K98  */  be_nested_str_weak(SHA256),
    /* K99  */  be_nested_str_weak(update),
    /* K100 */  be_nested_str_weak(out),
    /* K101 */  be_nested_str_weak(S2K_Info),
    /* K102 */  be_nested_str_weak(get_ipk_group_key),
    /* K103 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20SharedSecret_X20_X20_X3D_X20),
    /* K104 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K105 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K106 */  be_nested_str_weak(TBEData2_Nonce),
    /* K107 */  be_nested_str_weak(encrypt),
    /* K108 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Enc_X20_X20_X20_X3D_X20),
    /* K109 */  be_nested_str_weak(Sigma2),
    /* K110 */  be_nested_str_weak(responderRandom),
    /* K111 */  be_nested_str_weak(responderSessionId),
    /* K112 */  be_nested_str_weak(responderEphPubKey),
    /* K113 */  be_nested_str_weak(encrypted2),
    /* K114 */  be_nested_str_weak(MTR_X3A_X20sigma2_X3A_X20),
    /* K115 */  be_nested_str_weak(inspect),
    /* K116 */  be_nested_str_weak(_Msg2),
    /* K117 */  be_nested_str_weak(MTR_X3A_X20sigma2_raw_X3A_X20),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[562]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x5412002F,  //  0002  LDINT	R4	48
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x740E0005,  //  0004  JMPT	R3	#000B
      0x880C0302,  //  0005  GETMBR	R3	R1	K2
      0x200C0703,  //  0006  NE	R3	R3	K3
      0x740E0002,  //  0007  JMPT	R3	#000B
      0x880C0304,  //  0008  GETMBR	R3	R1	K4
      0x200C0703,  //  0009  NE	R3	R3	K3
      0x780E0000,  //  000A  JMPF	R3	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0xB80E0E00,  //  000C  GETNGBL	R3	K7
      0x8C0C0708,  //  000D  GETMET	R3	R3	K8
      0x7C0C0200,  //  000E  CALL	R3	1
      0x8C0C0709,  //  000F  GETMET	R3	R3	K9
      0x8814030A,  //  0010  GETMBR	R5	R1	K10
      0x8818030B,  //  0011  GETMBR	R6	R1	K11
      0x7C0C0600,  //  0012  CALL	R3	3
      0x8810070D,  //  0013  GETMBR	R4	R3	K13
      0x90021804,  //  0014  SETMBR	R0	K12	R4
      0x8810070E,  //  0015  GETMBR	R4	R3	K14
      0x4C140000,  //  0016  LDNIL	R5
      0x20100805,  //  0017  NE	R4	R4	R5
      0x78120003,  //  0018  JMPF	R4	#001D
      0x8810070F,  //  0019  GETMBR	R4	R3	K15
      0x4C140000,  //  001A  LDNIL	R5
      0x20100805,  //  001B  NE	R4	R4	R5
      0x74120000,  //  001C  JMPT	R4	#001E
      0x50100001,  //  001D  LDBOOL	R4	0	1
      0x50100200,  //  001E  LDBOOL	R4	1	0
      0x4C140000,  //  001F  LDNIL	R5
      0x78120006,  //  0020  JMPF	R4	#0028
      0x88180110,  //  0021  GETMBR	R6	R0	K16
      0x88180D11,  //  0022  GETMBR	R6	R6	K17
      0x8C180D12,  //  0023  GETMET	R6	R6	K18
      0x8820070E,  //  0024  GETMBR	R8	R3	K14
      0x7C180400,  //  0025  CALL	R6	2
      0x5C140C00,  //  0026  MOVE	R5	R6
      0x70020004,  //  0027  JMP		#002D
      0x8C180113,  //  0028  GETMET	R6	R0	K19
      0x88200714,  //  0029  GETMBR	R8	R3	K20
      0x88240715,  //  002A  GETMBR	R9	R3	K21
      0x7C180600,  //  002B  CALL	R6	3
      0x5C140C00,  //  002C  MOVE	R5	R6
      0x4C180000,  //  002D  LDNIL	R6
      0x1C180A06,  //  002E  EQ	R6	R5	R6
      0x781A0000,  //  002F  JMPF	R6	#0031
      0xB0062D17,  //  0030  RAISE	1	K22	K23
      0x88180318,  //  0031  GETMBR	R6	R1	K24
      0x90163006,  //  0032  SETMBR	R5	K24	R6
      0x8C180B19,  //  0033  GETMET	R6	R5	K25
      0xB8220E00,  //  0034  GETNGBL	R8	K7
      0x8820111A,  //  0035  GETMBR	R8	R8	K26
      0x8820111B,  //  0036  GETMBR	R8	R8	K27
      0x7C180400,  //  0037  CALL	R6	2
      0x8818031C,  //  0038  GETMBR	R6	R1	K28
      0x781A0004,  //  0039  JMPF	R6	#003F
      0x88180110,  //  003A  GETMBR	R6	R0	K16
      0x88180D11,  //  003B  GETMBR	R6	R6	K17
      0x8C180D1D,  //  003C  GETMET	R6	R6	K29
      0x8820031C,  //  003D  GETMBR	R8	R1	K28
      0x7C180400,  //  003E  CALL	R6	2
      0x90063805,  //  003F  SETMBR	R1	K28	R5
      0x8818071F,  //  0040  GETMBR	R6	R3	K31
      0x90163C06,  //  0041  SETMBR	R5	K30	R6
      0x88180110,  //  0042  GETMBR	R6	R0	K16
      0x88180D11,  //  0043  GETMBR	R6	R6	K17
      0x8C180D21,  //  0044  GETMET	R6	R6	K33
      0x7C180200,  //  0045  CALL	R6	1
      0x90164006,  //  0046  SETMBR	R5	K32	R6
      0x88180B20,  //  0047  GETMBR	R6	R5	K32
      0x90024406,  //  0048  SETMBR	R0	K34	R6
      0x781200EC,  //  0049  JMPF	R4	#0137
      0x88180B23,  //  004A  GETMBR	R6	R5	K35
      0x4C1C0000,  //  004B  LDNIL	R7
      0x20180C07,  //  004C  NE	R6	R6	R7
      0x781A00E8,  //  004D  JMPF	R6	#0137
      0x88180715,  //  004E  GETMBR	R6	R3	K21
      0x881C070E,  //  004F  GETMBR	R7	R3	K14
      0x00180C07,  //  0050  ADD	R6	R6	R7
      0x601C0015,  //  0051  GETGBL	R7	G21
      0x7C1C0000,  //  0052  CALL	R7	0
      0x8C1C0F24,  //  0053  GETMET	R7	R7	K36
      0x58240025,  //  0054  LDCONST	R9	K37
      0x7C1C0400,  //  0055  CALL	R7	2
      0x8C200526,  //  0056  GETMET	R8	R2	K38
      0x7C200200,  //  0057  CALL	R8	1
      0x8C201127,  //  0058  GETMET	R8	R8	K39
      0x88280B23,  //  0059  GETMBR	R10	R5	K35
      0x5C2C0C00,  //  005A  MOVE	R11	R6
      0x5C300E00,  //  005B  MOVE	R12	R7
      0x5436000F,  //  005C  LDINT	R13	16
      0x7C200A00,  //  005D  CALL	R8	5
      0x60240015,  //  005E  GETGBL	R9	G21
      0x7C240000,  //  005F  CALL	R9	0
      0x8C241324,  //  0060  GETMET	R9	R9	K36
      0x582C0028,  //  0061  LDCONST	R11	K40
      0x7C240400,  //  0062  CALL	R9	2
      0x5429FFEE,  //  0063  LDINT	R10	-17
      0x402A060A,  //  0064  CONNECT	R10	K3	R10
      0x882C070F,  //  0065  GETMBR	R11	R3	K15
      0x9428160A,  //  0066  GETIDX	R10	R11	R10
      0x5431FFEF,  //  0067  LDINT	R12	-16
      0x40301929,  //  0068  CONNECT	R12	R12	K41
      0x8834070F,  //  0069  GETMBR	R13	R3	K15
      0x942C1A0C,  //  006A  GETIDX	R11	R13	R12
      0x8C38052A,  //  006B  GETMET	R14	R2	K42
      0x5C401000,  //  006C  MOVE	R16	R8
      0x5C441200,  //  006D  MOVE	R17	R9
      0x60480015,  //  006E  GETGBL	R18	G21
      0x7C480000,  //  006F  CALL	R18	0
      0x604C000C,  //  0070  GETGBL	R19	G12
      0x5C501400,  //  0071  MOVE	R20	R10
      0x7C4C0200,  //  0072  CALL	R19	1
      0x5452000F,  //  0073  LDINT	R20	16
      0x7C380C00,  //  0074  CALL	R14	6
      0x5C301C00,  //  0075  MOVE	R12	R14
      0x8C38192B,  //  0076  GETMET	R14	R12	K43
      0x5C401400,  //  0077  MOVE	R16	R10
      0x7C380400,  //  0078  CALL	R14	2
      0x5C341C00,  //  0079  MOVE	R13	R14
      0x8C38192C,  //  007A  GETMET	R14	R12	K44
      0x7C380200,  //  007B  CALL	R14	1
      0xB83E5A00,  //  007C  GETNGBL	R15	K45
      0x8C3C1F2E,  //  007D  GETMET	R15	R15	K46
      0x5844002F,  //  007E  LDCONST	R17	K47
      0x544A0003,  //  007F  LDINT	R18	4
      0x7C3C0600,  //  0080  CALL	R15	3
      0xB83E5A00,  //  0081  GETNGBL	R15	K45
      0x8C3C1F2E,  //  0082  GETMET	R15	R15	K46
      0x8C441131,  //  0083  GETMET	R17	R8	K49
      0x7C440200,  //  0084  CALL	R17	1
      0x00466011,  //  0085  ADD	R17	K48	R17
      0x544A0003,  //  0086  LDINT	R18	4
      0x7C3C0600,  //  0087  CALL	R15	3
      0xB83E5A00,  //  0088  GETNGBL	R15	K45
      0x8C3C1F2E,  //  0089  GETMET	R15	R15	K46
      0x8C441731,  //  008A  GETMET	R17	R11	K49
      0x7C440200,  //  008B  CALL	R17	1
      0x00466411,  //  008C  ADD	R17	K50	R17
      0x544A0003,  //  008D  LDINT	R18	4
      0x7C3C0600,  //  008E  CALL	R15	3
      0xB83E5A00,  //  008F  GETNGBL	R15	K45
      0x8C3C1F2E,  //  0090  GETMET	R15	R15	K46
      0x8C441B31,  //  0091  GETMET	R17	R13	K49
      0x7C440200,  //  0092  CALL	R17	1
      0x00466611,  //  0093  ADD	R17	K51	R17
      0x544A0003,  //  0094  LDINT	R18	4
      0x7C3C0600,  //  0095  CALL	R15	3
      0xB83E5A00,  //  0096  GETNGBL	R15	K45
      0x8C3C1F2E,  //  0097  GETMET	R15	R15	K46
      0x8C441D31,  //  0098  GETMET	R17	R14	K49
      0x7C440200,  //  0099  CALL	R17	1
      0x00466811,  //  009A  ADD	R17	K52	R17
      0x544A0003,  //  009B  LDINT	R18	4
      0x7C3C0600,  //  009C  CALL	R15	3
      0xB83E5A00,  //  009D  GETNGBL	R15	K45
      0x8C3C1F2E,  //  009E  GETMET	R15	R15	K46
      0x5844002F,  //  009F  LDCONST	R17	K47
      0x544A0003,  //  00A0  LDINT	R18	4
      0x7C3C0600,  //  00A1  CALL	R15	3
      0x1C3C160E,  //  00A2  EQ	R15	R11	R14
      0x783E0090,  //  00A3  JMPF	R15	#0135
      0x8C3C0536,  //  00A4  GETMET	R15	R2	K54
      0x5446000F,  //  00A5  LDINT	R17	16
      0x7C3C0400,  //  00A6  CALL	R15	2
      0x90166A0F,  //  00A7  SETMBR	R5	K53	R15
      0x603C0015,  //  00A8  GETGBL	R15	G21
      0x7C3C0000,  //  00A9  CALL	R15	0
      0x8C3C1F24,  //  00AA  GETMET	R15	R15	K36
      0x58440037,  //  00AB  LDCONST	R17	K55
      0x7C3C0400,  //  00AC  CALL	R15	2
      0x88400B35,  //  00AD  GETMBR	R16	R5	K53
      0x003C1E10,  //  00AE  ADD	R15	R15	R16
      0x88400715,  //  00AF  GETMBR	R16	R3	K21
      0x8844070E,  //  00B0  GETMBR	R17	R3	K14
      0x00402011,  //  00B1  ADD	R16	R16	R17
      0x8C440526,  //  00B2  GETMET	R17	R2	K38
      0x7C440200,  //  00B3  CALL	R17	1
      0x8C442327,  //  00B4  GETMET	R17	R17	K39
      0x884C0B23,  //  00B5  GETMBR	R19	R5	K35
      0x5C502000,  //  00B6  MOVE	R20	R16
      0x5C541E00,  //  00B7  MOVE	R21	R15
      0x545A000F,  //  00B8  LDINT	R22	16
      0x7C440A00,  //  00B9  CALL	R17	5
      0x8C48052A,  //  00BA  GETMET	R18	R2	K42
      0x5C502200,  //  00BB  MOVE	R20	R17
      0x60540015,  //  00BC  GETGBL	R21	G21
      0x7C540000,  //  00BD  CALL	R21	0
      0x8C542B24,  //  00BE  GETMET	R21	R21	K36
      0x585C0038,  //  00BF  LDCONST	R23	K56
      0x7C540400,  //  00C0  CALL	R21	2
      0x60580015,  //  00C1  GETGBL	R22	G21
      0x7C580000,  //  00C2  CALL	R22	0
      0x585C0003,  //  00C3  LDCONST	R23	K3
      0x5462000F,  //  00C4  LDINT	R24	16
      0x7C480C00,  //  00C5  CALL	R18	6
      0x8C4C252C,  //  00C6  GETMET	R19	R18	K44
      0x7C4C0200,  //  00C7  CALL	R19	1
      0xB8520E00,  //  00C8  GETNGBL	R20	K7
      0x8C502939,  //  00C9  GETMET	R20	R20	K57
      0x7C500200,  //  00CA  CALL	R20	1
      0x88540B35,  //  00CB  GETMBR	R21	R5	K53
      0x90521C15,  //  00CC  SETMBR	R20	K14	R21
      0x88540B20,  //  00CD  GETMBR	R21	R5	K32
      0x90527415,  //  00CE  SETMBR	R20	K58	R21
      0x90527613,  //  00CF  SETMBR	R20	K59	R19
      0x8C540526,  //  00D0  GETMET	R21	R2	K38
      0x7C540200,  //  00D1  CALL	R21	1
      0x8C542B27,  //  00D2  GETMET	R21	R21	K39
      0x885C0B23,  //  00D3  GETMBR	R23	R5	K35
      0x88600715,  //  00D4  GETMBR	R24	R3	K21
      0x8864070E,  //  00D5  GETMBR	R25	R3	K14
      0x00603019,  //  00D6  ADD	R24	R24	R25
      0x60640015,  //  00D7  GETGBL	R25	G21
      0x7C640000,  //  00D8  CALL	R25	0
      0x8C643324,  //  00D9  GETMET	R25	R25	K36
      0x586C003C,  //  00DA  LDCONST	R27	K60
      0x7C640400,  //  00DB  CALL	R25	2
      0x546A002F,  //  00DC  LDINT	R26	48
      0x7C540A00,  //  00DD  CALL	R21	5
      0x545A000E,  //  00DE  LDINT	R22	15
      0x405A0616,  //  00DF  CONNECT	R22	K3	R22
      0x94582A16,  //  00E0  GETIDX	R22	R21	R22
      0x545E000F,  //  00E1  LDINT	R23	16
      0x5462001E,  //  00E2  LDINT	R24	31
      0x405C2E18,  //  00E3  CONNECT	R23	R23	R24
      0x945C2A17,  //  00E4  GETIDX	R23	R21	R23
      0x5462001F,  //  00E5  LDINT	R24	32
      0x5466002E,  //  00E6  LDINT	R25	47
      0x40603019,  //  00E7  CONNECT	R24	R24	R25
      0x94602A18,  //  00E8  GETIDX	R24	R21	R24
      0xB8665A00,  //  00E9  GETNGBL	R25	K45
      0x8C64333D,  //  00EA  GETMET	R25	R25	K61
      0x7C640200,  //  00EB  CALL	R25	1
      0x9464333E,  //  00EC  GETIDX	R25	R25	K62
      0xB86A5A00,  //  00ED  GETNGBL	R26	K45
      0x8C68352E,  //  00EE  GETMET	R26	R26	K46
      0x5870003F,  //  00EF  LDCONST	R28	K63
      0x54760003,  //  00F0  LDINT	R29	4
      0x7C680600,  //  00F1  CALL	R26	3
      0xB86A5A00,  //  00F2  GETNGBL	R26	K45
      0x8C68352E,  //  00F3  GETMET	R26	R26	K46
      0x8C702D31,  //  00F4  GETMET	R28	R22	K49
      0x7C700200,  //  00F5  CALL	R28	1
      0x0072801C,  //  00F6  ADD	R28	K64	R28
      0x54760003,  //  00F7  LDINT	R29	4
      0x7C680600,  //  00F8  CALL	R26	3
      0xB86A5A00,  //  00F9  GETNGBL	R26	K45
      0x8C68352E,  //  00FA  GETMET	R26	R26	K46
      0x8C702F31,  //  00FB  GETMET	R28	R23	K49
      0x7C700200,  //  00FC  CALL	R28	1
      0x0072821C,  //  00FD  ADD	R28	K65	R28
      0x54760003,  //  00FE  LDINT	R29	4
      0x7C680600,  //  00FF  CALL	R26	3
      0xB86A5A00,  //  0100  GETNGBL	R26	K45
      0x8C68352E,  //  0101  GETMET	R26	R26	K46
      0x8C703131,  //  0102  GETMET	R28	R24	K49
      0x7C700200,  //  0103  CALL	R28	1
      0x0072841C,  //  0104  ADD	R28	K66	R28
      0x54760003,  //  0105  LDINT	R29	4
      0x7C680600,  //  0106  CALL	R26	3
      0xB86A5A00,  //  0107  GETNGBL	R26	K45
      0x8C68352E,  //  0108  GETMET	R26	R26	K46
      0x5870003F,  //  0109  LDCONST	R28	K63
      0x54760003,  //  010A  LDINT	R29	4
      0x7C680600,  //  010B  CALL	R26	3
      0x8C682943,  //  010C  GETMET	R26	R20	K67
      0x7C680200,  //  010D  CALL	R26	1
      0x4C6C0000,  //  010E  LDNIL	R27
      0x9016881B,  //  010F  SETMBR	R5	K68	R27
      0xB86E5A00,  //  0110  GETNGBL	R27	K45
      0x8C6C372E,  //  0111  GETMET	R27	R27	K46
      0x8C743531,  //  0112  GETMET	R29	R26	K49
      0x7C740200,  //  0113  CALL	R29	1
      0x00768A1D,  //  0114  ADD	R29	K69	R29
      0x547A0003,  //  0115  LDINT	R30	4
      0x7C6C0600,  //  0116  CALL	R27	3
      0x8C6C0346,  //  0117  GETMET	R27	R1	K70
      0x54760032,  //  0118  LDINT	R29	51
      0x50780200,  //  0119  LDBOOL	R30	1	0
      0x7C6C0600,  //  011A  CALL	R27	3
      0x8C703743,  //  011B  GETMET	R28	R27	K67
      0x5C783400,  //  011C  MOVE	R30	R26
      0x7C700400,  //  011D  CALL	R28	2
      0x88740147,  //  011E  GETMBR	R29	R0	K71
      0x8C743B48,  //  011F  GETMET	R29	R29	K72
      0x5C7C3800,  //  0120  MOVE	R31	R28
      0x88800349,  //  0121  GETMBR	R32	R1	K73
      0x8884034A,  //  0122  GETMBR	R33	R1	K74
      0x8888374B,  //  0123  GETMBR	R34	R27	K75
      0x7C740A00,  //  0124  CALL	R29	5
      0x8C740B4C,  //  0125  GETMET	R29	R5	K76
      0x5C7C2C00,  //  0126  MOVE	R31	R22
      0x5C802E00,  //  0127  MOVE	R32	R23
      0x5C843000,  //  0128  MOVE	R33	R24
      0x5C883200,  //  0129  MOVE	R34	R25
      0x7C740A00,  //  012A  CALL	R29	5
      0x8C740B4D,  //  012B  GETMET	R29	R5	K77
      0x507C0200,  //  012C  LDBOOL	R31	1	0
      0x7C740400,  //  012D  CALL	R29	2
      0x8C740B4E,  //  012E  GETMET	R29	R5	K78
      0x7C740200,  //  012F  CALL	R29	1
      0x8C740B4F,  //  0130  GETMET	R29	R5	K79
      0x7C740200,  //  0131  CALL	R29	1
      0x50740200,  //  0132  LDBOOL	R29	1	0
      0x80043A00,  //  0133  RET	1	R29
      0x70020001,  //  0134  JMP		#0137
      0x4C3C0000,  //  0135  LDNIL	R15
      0x900E1C0F,  //  0136  SETMBR	R3	K14	R15
      0x8818070E,  //  0137  GETMBR	R6	R3	K14
      0x4C1C0000,  //  0138  LDNIL	R7
      0x1C180C07,  //  0139  EQ	R6	R6	R7
      0x741A0003,  //  013A  JMPT	R6	#013F
      0x8818070F,  //  013B  GETMBR	R6	R3	K15
      0x4C1C0000,  //  013C  LDNIL	R7
      0x1C180C07,  //  013D  EQ	R6	R6	R7
      0x781A00F0,  //  013E  JMPF	R6	#0230
      0x8C180536,  //  013F  GETMET	R6	R2	K54
      0x5422000F,  //  0140  LDINT	R8	16
      0x7C180400,  //  0141  CALL	R6	2
      0x90166A06,  //  0142  SETMBR	R5	K53	R6
      0x8C180536,  //  0143  GETMET	R6	R2	K54
      0x5422001F,  //  0144  LDINT	R8	32
      0x7C180400,  //  0145  CALL	R6	2
      0x9002A006,  //  0146  SETMBR	R0	K80	R6
      0x8C180552,  //  0147  GETMET	R6	R2	K82
      0x7C180200,  //  0148  CALL	R6	1
      0x8C180D53,  //  0149  GETMET	R6	R6	K83
      0x88200150,  //  014A  GETMBR	R8	R0	K80
      0x7C180400,  //  014B  CALL	R6	2
      0x9002A206,  //  014C  SETMBR	R0	K81	R6
      0x8C180536,  //  014D  GETMET	R6	R2	K54
      0x5422001F,  //  014E  LDINT	R8	32
      0x7C180400,  //  014F  CALL	R6	2
      0x8C1C0552,  //  0150  GETMET	R7	R2	K82
      0x7C1C0200,  //  0151  CALL	R7	1
      0x8C1C0F54,  //  0152  GETMET	R7	R7	K84
      0x88240150,  //  0153  GETMBR	R9	R0	K80
      0x8828070D,  //  0154  GETMBR	R10	R3	K13
      0x7C1C0600,  //  0155  CALL	R7	3
      0x90164607,  //  0156  SETMBR	R5	K35	R7
      0xB81E0E00,  //  0157  GETNGBL	R7	K7
      0x881C0F55,  //  0158  GETMBR	R7	R7	K85
      0x8C1C0F56,  //  0159  GETMET	R7	R7	K86
      0x7C1C0200,  //  015A  CALL	R7	1
      0x8C200F57,  //  015B  GETMET	R8	R7	K87
      0x58280058,  //  015C  LDCONST	R10	K88
      0xB82E0E00,  //  015D  GETNGBL	R11	K7
      0x882C1755,  //  015E  GETMBR	R11	R11	K85
      0x882C1759,  //  015F  GETMBR	R11	R11	K89
      0x8C300B5A,  //  0160  GETMET	R12	R5	K90
      0x7C300200,  //  0161  CALL	R12	1
      0x7C200800,  //  0162  CALL	R8	4
      0x8C200F57,  //  0163  GETMET	R8	R7	K87
      0x5828005B,  //  0164  LDCONST	R10	K91
      0xB82E0E00,  //  0165  GETNGBL	R11	K7
      0x882C1755,  //  0166  GETMBR	R11	R11	K85
      0x882C1759,  //  0167  GETMBR	R11	R11	K89
      0x8C300B5C,  //  0168  GETMET	R12	R5	K92
      0x7C300200,  //  0169  CALL	R12	1
      0x7C200800,  //  016A  CALL	R8	4
      0x8C200F57,  //  016B  GETMET	R8	R7	K87
      0x5828005D,  //  016C  LDCONST	R10	K93
      0xB82E0E00,  //  016D  GETNGBL	R11	K7
      0x882C1755,  //  016E  GETMBR	R11	R11	K85
      0x882C1759,  //  016F  GETMBR	R11	R11	K89
      0x88300151,  //  0170  GETMBR	R12	R0	K81
      0x7C200800,  //  0171  CALL	R8	4
      0x8C200F57,  //  0172  GETMET	R8	R7	K87
      0x542A0003,  //  0173  LDINT	R10	4
      0xB82E0E00,  //  0174  GETNGBL	R11	K7
      0x882C1755,  //  0175  GETMBR	R11	R11	K85
      0x882C1759,  //  0176  GETMBR	R11	R11	K89
      0x8830070D,  //  0177  GETMBR	R12	R3	K13
      0x7C200800,  //  0178  CALL	R8	4
      0x8C200552,  //  0179  GETMET	R8	R2	K82
      0x7C200200,  //  017A  CALL	R8	1
      0x8C20115E,  //  017B  GETMET	R8	R8	K94
      0x8C280B5F,  //  017C  GETMET	R10	R5	K95
      0x7C280200,  //  017D  CALL	R10	1
      0x8C2C0F43,  //  017E  GETMET	R11	R7	K67
      0x7C2C0200,  //  017F  CALL	R11	1
      0x7C200600,  //  0180  CALL	R8	3
      0xB8260E00,  //  0181  GETNGBL	R9	K7
      0x88241355,  //  0182  GETMBR	R9	R9	K85
      0x8C241356,  //  0183  GETMET	R9	R9	K86
      0x7C240200,  //  0184  CALL	R9	1
      0x8C281357,  //  0185  GETMET	R10	R9	K87
      0x58300058,  //  0186  LDCONST	R12	K88
      0xB8360E00,  //  0187  GETNGBL	R13	K7
      0x88341B55,  //  0188  GETMBR	R13	R13	K85
      0x88341B59,  //  0189  GETMBR	R13	R13	K89
      0x8C380B5A,  //  018A  GETMET	R14	R5	K90
      0x7C380200,  //  018B  CALL	R14	1
      0x7C280800,  //  018C  CALL	R10	4
      0x8C281357,  //  018D  GETMET	R10	R9	K87
      0x5830005B,  //  018E  LDCONST	R12	K91
      0xB8360E00,  //  018F  GETNGBL	R13	K7
      0x88341B55,  //  0190  GETMBR	R13	R13	K85
      0x88341B59,  //  0191  GETMBR	R13	R13	K89
      0x8C380B5C,  //  0192  GETMET	R14	R5	K92
      0x7C380200,  //  0193  CALL	R14	1
      0x7C280800,  //  0194  CALL	R10	4
      0x8C281357,  //  0195  GETMET	R10	R9	K87
      0x5830005D,  //  0196  LDCONST	R12	K93
      0xB8360E00,  //  0197  GETNGBL	R13	K7
      0x88341B55,  //  0198  GETMBR	R13	R13	K85
      0x88341B59,  //  0199  GETMBR	R13	R13	K89
      0x5C381000,  //  019A  MOVE	R14	R8
      0x7C280800,  //  019B  CALL	R10	4
      0x8C281357,  //  019C  GETMET	R10	R9	K87
      0x54320003,  //  019D  LDINT	R12	4
      0xB8360E00,  //  019E  GETNGBL	R13	K7
      0x88341B55,  //  019F  GETMBR	R13	R13	K85
      0x88341B59,  //  01A0  GETMBR	R13	R13	K89
      0x88380B35,  //  01A1  GETMBR	R14	R5	K53
      0x7C280800,  //  01A2  CALL	R10	4
      0xB82A5A00,  //  01A3  GETNGBL	R10	K45
      0x8C28152E,  //  01A4  GETMET	R10	R10	K46
      0x5830002F,  //  01A5  LDCONST	R12	K47
      0x54360003,  //  01A6  LDINT	R13	4
      0x7C280600,  //  01A7  CALL	R10	3
      0x88280760,  //  01A8  GETMBR	R10	R3	K96
      0x9016880A,  //  01A9  SETMBR	R5	K68	R10
      0xB82A5A00,  //  01AA  GETNGBL	R10	K45
      0x8C28152E,  //  01AB  GETMET	R10	R10	K46
      0x88300B44,  //  01AC  GETMBR	R12	R5	K68
      0x8C301931,  //  01AD  GETMET	R12	R12	K49
      0x7C300200,  //  01AE  CALL	R12	1
      0x0032C20C,  //  01AF  ADD	R12	K97	R12
      0x54360003,  //  01B0  LDINT	R13	4
      0x7C280600,  //  01B1  CALL	R10	3
      0x8C280562,  //  01B2  GETMET	R10	R2	K98
      0x7C280200,  //  01B3  CALL	R10	1
      0x8C281563,  //  01B4  GETMET	R10	R10	K99
      0x88300B44,  //  01B5  GETMBR	R12	R5	K68
      0x7C280400,  //  01B6  CALL	R10	2
      0x8C281564,  //  01B7  GETMET	R10	R10	K100
      0x7C280200,  //  01B8  CALL	R10	1
      0x602C0015,  //  01B9  GETGBL	R11	G21
      0x7C2C0000,  //  01BA  CALL	R11	0
      0x8C2C1724,  //  01BB  GETMET	R11	R11	K36
      0x88340165,  //  01BC  GETMBR	R13	R0	K101
      0x7C2C0400,  //  01BD  CALL	R11	2
      0x8C300B66,  //  01BE  GETMET	R12	R5	K102
      0x7C300200,  //  01BF  CALL	R12	1
      0x00301806,  //  01C0  ADD	R12	R12	R6
      0x88340151,  //  01C1  GETMBR	R13	R0	K81
      0x0030180D,  //  01C2  ADD	R12	R12	R13
      0x0030180A,  //  01C3  ADD	R12	R12	R10
      0x8C340526,  //  01C4  GETMET	R13	R2	K38
      0x7C340200,  //  01C5  CALL	R13	1
      0x8C341B27,  //  01C6  GETMET	R13	R13	K39
      0x883C0B23,  //  01C7  GETMBR	R15	R5	K35
      0x5C401800,  //  01C8  MOVE	R16	R12
      0x5C441600,  //  01C9  MOVE	R17	R11
      0x544A000F,  //  01CA  LDINT	R18	16
      0x7C340A00,  //  01CB  CALL	R13	5
      0xB83A5A00,  //  01CC  GETNGBL	R14	K45
      0x8C381D2E,  //  01CD  GETMET	R14	R14	K46
      0x88400B23,  //  01CE  GETMBR	R16	R5	K35
      0x8C402131,  //  01CF  GETMET	R16	R16	K49
      0x7C400200,  //  01D0  CALL	R16	1
      0x0042CE10,  //  01D1  ADD	R16	K103	R16
      0x54460003,  //  01D2  LDINT	R17	4
      0x7C380600,  //  01D3  CALL	R14	3
      0xB83A5A00,  //  01D4  GETNGBL	R14	K45
      0x8C381D2E,  //  01D5  GETMET	R14	R14	K46
      0x8C401931,  //  01D6  GETMET	R16	R12	K49
      0x7C400200,  //  01D7  CALL	R16	1
      0x0042D010,  //  01D8  ADD	R16	K104	R16
      0x54460003,  //  01D9  LDINT	R17	4
      0x7C380600,  //  01DA  CALL	R14	3
      0xB83A5A00,  //  01DB  GETNGBL	R14	K45
      0x8C381D2E,  //  01DC  GETMET	R14	R14	K46
      0x8C401B31,  //  01DD  GETMET	R16	R13	K49
      0x7C400200,  //  01DE  CALL	R16	1
      0x0042D210,  //  01DF  ADD	R16	K105	R16
      0x54460003,  //  01E0  LDINT	R17	4
      0x7C380600,  //  01E1  CALL	R14	3
      0x8C381343,  //  01E2  GETMET	R14	R9	K67
      0x7C380200,  //  01E3  CALL	R14	1
      0x8C3C052A,  //  01E4  GETMET	R15	R2	K42
      0x5C441A00,  //  01E5  MOVE	R17	R13
      0x60480015,  //  01E6  GETGBL	R18	G21
      0x7C480000,  //  01E7  CALL	R18	0
      0x8C482524,  //  01E8  GETMET	R18	R18	K36
      0x8850016A,  //  01E9  GETMBR	R20	R0	K106
      0x7C480400,  //  01EA  CALL	R18	2
      0x604C0015,  //  01EB  GETGBL	R19	G21
      0x7C4C0000,  //  01EC  CALL	R19	0
      0x6050000C,  //  01ED  GETGBL	R20	G12
      0x5C541C00,  //  01EE  MOVE	R21	R14
      0x7C500200,  //  01EF  CALL	R20	1
      0x5456000F,  //  01F0  LDINT	R21	16
      0x7C3C0C00,  //  01F1  CALL	R15	6
      0x8C401F6B,  //  01F2  GETMET	R16	R15	K107
      0x5C481C00,  //  01F3  MOVE	R18	R14
      0x7C400400,  //  01F4  CALL	R16	2
      0x8C441F2C,  //  01F5  GETMET	R17	R15	K44
      0x7C440200,  //  01F6  CALL	R17	1
      0x00402011,  //  01F7  ADD	R16	R16	R17
      0xB8465A00,  //  01F8  GETNGBL	R17	K45
      0x8C44232E,  //  01F9  GETMET	R17	R17	K46
      0x8C4C2131,  //  01FA  GETMET	R19	R16	K49
      0x7C4C0200,  //  01FB  CALL	R19	1
      0x004ED813,  //  01FC  ADD	R19	K108	R19
      0x54520003,  //  01FD  LDINT	R20	4
      0x7C440600,  //  01FE  CALL	R17	3
      0xB8465A00,  //  01FF  GETNGBL	R17	K45
      0x8C44232E,  //  0200  GETMET	R17	R17	K46
      0x584C002F,  //  0201  LDCONST	R19	K47
      0x54520003,  //  0202  LDINT	R20	4
      0x7C440600,  //  0203  CALL	R17	3
      0xB8460E00,  //  0204  GETNGBL	R17	K7
      0x8C44236D,  //  0205  GETMET	R17	R17	K109
      0x7C440200,  //  0206  CALL	R17	1
      0x9046DC06,  //  0207  SETMBR	R17	K110	R6
      0x88480122,  //  0208  GETMBR	R18	R0	K34
      0x9046DE12,  //  0209  SETMBR	R17	K111	R18
      0x88480151,  //  020A  GETMBR	R18	R0	K81
      0x9046E012,  //  020B  SETMBR	R17	K112	R18
      0x9046E210,  //  020C  SETMBR	R17	K113	R16
      0xB84A5A00,  //  020D  GETNGBL	R18	K45
      0x8C48252E,  //  020E  GETMET	R18	R18	K46
      0xB8520E00,  //  020F  GETNGBL	R20	K7
      0x8C502973,  //  0210  GETMET	R20	R20	K115
      0x5C582200,  //  0211  MOVE	R22	R17
      0x7C500400,  //  0212  CALL	R20	2
      0x0052E414,  //  0213  ADD	R20	K114	R20
      0x54560003,  //  0214  LDINT	R21	4
      0x7C480600,  //  0215  CALL	R18	3
      0x8C482343,  //  0216  GETMET	R18	R17	K67
      0x7C480200,  //  0217  CALL	R18	1
      0x9016E812,  //  0218  SETMBR	R5	K116	R18
      0xB84E5A00,  //  0219  GETNGBL	R19	K45
      0x8C4C272E,  //  021A  GETMET	R19	R19	K46
      0x8C542531,  //  021B  GETMET	R21	R18	K49
      0x7C540200,  //  021C  CALL	R21	1
      0x0056EA15,  //  021D  ADD	R21	K117	R21
      0x545A0003,  //  021E  LDINT	R22	4
      0x7C4C0600,  //  021F  CALL	R19	3
      0x8C4C0346,  //  0220  GETMET	R19	R1	K70
      0x54560030,  //  0221  LDINT	R21	49
      0x50580200,  //  0222  LDBOOL	R22	1	0
      0x7C4C0600,  //  0223  CALL	R19	3
      0x8C502743,  //  0224  GETMET	R20	R19	K67
      0x5C582400,  //  0225  MOVE	R22	R18
      0x7C500400,  //  0226  CALL	R20	2
      0x88540147,  //  0227  GETMBR	R21	R0	K71
      0x8C542B48,  //  0228  GETMET	R21	R21	K72
      0x5C5C2800,  //  0229  MOVE	R23	R20
      0x88600349,  //  022A  GETMBR	R24	R1	K73
      0x8864034A,  //  022B  GETMBR	R25	R1	K74
      0x8868274B,  //  022C  GETMBR	R26	R19	K75
      0x7C540A00,  //  022D  CALL	R21	5
      0x50540200,  //  022E  LDBOOL	R21	1	0
      0x80042A00,  //  022F  RET	1	R21
      0x50180200,  //  0230  LDBOOL	R6	1	0
      0x80040C00,  //  0231  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Pake3
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Pake3,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[46]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(protocol_error),
    /* K6   */  be_nested_str_weak(invalid_X20Pake3_X20message),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(Pake3),
    /* K9   */  be_nested_str_weak(parse),
    /* K10  */  be_nested_str_weak(raw),
    /* K11  */  be_nested_str_weak(app_payload_idx),
    /* K12  */  be_nested_str_weak(cA),
    /* K13  */  be_nested_str_weak(tasmota),
    /* K14  */  be_nested_str_weak(log),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20received_X20cA_X3D),
    /* K16  */  be_nested_str_weak(tohex),
    /* K17  */  be_nested_str_weak(spake),
    /* K18  */  be_nested_str_weak(invalid_X20cA_X20received),
    /* K19  */  be_nested_str_weak(session_timestamp),
    /* K20  */  be_nested_str_weak(rtc),
    /* K21  */  be_nested_str_weak(utc),
    /* K22  */  be_nested_str_weak(HKDF_SHA256),
    /* K23  */  be_nested_str_weak(derive),
    /* K24  */  be_nested_str_weak(Ke),
    /* K25  */  be_nested_str_weak(fromstring),
    /* K26  */  be_nested_str_weak(SEKeys_Info),
    /* K27  */  be_nested_str_weak(I2RKey),
    /* K28  */  be_nested_str_weak(R2IKey),
    /* K29  */  be_nested_str_weak(AttestationChallenge),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20session_keys_X3D),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K35  */  be_nested_str_weak(build_response),
    /* K36  */  be_nested_str_weak(add),
    /* K37  */  be_const_int(2),
    /* K38  */  be_nested_str_weak(encode),
    /* K39  */  be_nested_str_weak(responder),
    /* K40  */  be_nested_str_weak(send_response),
    /* K41  */  be_nested_str_weak(remote_ip),
    /* K42  */  be_nested_str_weak(remote_port),
    /* K43  */  be_nested_str_weak(add_session),
    /* K44  */  be_nested_str_weak(future_local_session_id),
    /* K45  */  be_nested_str_weak(future_initiator_session_id),
    }),
    be_str_weak(parse_Pake3),
    &be_const_str_solidified,
    ( &(const binstruction[146]) {  /* code */
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
      0x780E0000,  //  000A  JMPF	R3	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0xB80E0E00,  //  000C  GETNGBL	R3	K7
      0x8C0C0708,  //  000D  GETMET	R3	R3	K8
      0x7C0C0200,  //  000E  CALL	R3	1
      0x8C0C0709,  //  000F  GETMET	R3	R3	K9
      0x8814030A,  //  0010  GETMBR	R5	R1	K10
      0x8818030B,  //  0011  GETMBR	R6	R1	K11
      0x7C0C0600,  //  0012  CALL	R3	3
      0x8810070C,  //  0013  GETMBR	R4	R3	K12
      0x90021804,  //  0014  SETMBR	R0	K12	R4
      0xB8121A00,  //  0015  GETNGBL	R4	K13
      0x8C10090E,  //  0016  GETMET	R4	R4	K14
      0x8818010C,  //  0017  GETMBR	R6	R0	K12
      0x8C180D10,  //  0018  GETMET	R6	R6	K16
      0x7C180200,  //  0019  CALL	R6	1
      0x001A1E06,  //  001A  ADD	R6	K15	R6
      0x541E0003,  //  001B  LDINT	R7	4
      0x7C100600,  //  001C  CALL	R4	3
      0x8810010C,  //  001D  GETMBR	R4	R0	K12
      0x88140111,  //  001E  GETMBR	R5	R0	K17
      0x88140B0C,  //  001F  GETMBR	R5	R5	K12
      0x20100805,  //  0020  NE	R4	R4	R5
      0x78120000,  //  0021  JMPF	R4	#0023
      0xB0060B12,  //  0022  RAISE	1	K5	K18
      0xB8121A00,  //  0023  GETNGBL	R4	K13
      0x8C100914,  //  0024  GETMET	R4	R4	K20
      0x7C100200,  //  0025  CALL	R4	1
      0x94100915,  //  0026  GETIDX	R4	R4	K21
      0x90022604,  //  0027  SETMBR	R0	K19	R4
      0x8C100516,  //  0028  GETMET	R4	R2	K22
      0x7C100200,  //  0029  CALL	R4	1
      0x8C100917,  //  002A  GETMET	R4	R4	K23
      0x88180118,  //  002B  GETMBR	R6	R0	K24
      0x601C0015,  //  002C  GETGBL	R7	G21
      0x7C1C0000,  //  002D  CALL	R7	0
      0x60200015,  //  002E  GETGBL	R8	G21
      0x7C200000,  //  002F  CALL	R8	0
      0x8C201119,  //  0030  GETMET	R8	R8	K25
      0x8828011A,  //  0031  GETMBR	R10	R0	K26
      0x7C200400,  //  0032  CALL	R8	2
      0x5426002F,  //  0033  LDINT	R9	48
      0x7C100A00,  //  0034  CALL	R4	5
      0x5416000E,  //  0035  LDINT	R5	15
      0x40160605,  //  0036  CONNECT	R5	K3	R5
      0x94140805,  //  0037  GETIDX	R5	R4	R5
      0x90023605,  //  0038  SETMBR	R0	K27	R5
      0x5416000F,  //  0039  LDINT	R5	16
      0x541A001E,  //  003A  LDINT	R6	31
      0x40140A06,  //  003B  CONNECT	R5	R5	R6
      0x94140805,  //  003C  GETIDX	R5	R4	R5
      0x90023805,  //  003D  SETMBR	R0	K28	R5
      0x5416001F,  //  003E  LDINT	R5	32
      0x541A002E,  //  003F  LDINT	R6	47
      0x40140A06,  //  0040  CONNECT	R5	R5	R6
      0x94140805,  //  0041  GETIDX	R5	R4	R5
      0x90023A05,  //  0042  SETMBR	R0	K29	R5
      0xB8161A00,  //  0043  GETNGBL	R5	K13
      0x8C140B0E,  //  0044  GETMET	R5	R5	K14
      0x581C001E,  //  0045  LDCONST	R7	K30
      0x54220003,  //  0046  LDINT	R8	4
      0x7C140600,  //  0047  CALL	R5	3
      0xB8161A00,  //  0048  GETNGBL	R5	K13
      0x8C140B0E,  //  0049  GETMET	R5	R5	K14
      0x8C1C0910,  //  004A  GETMET	R7	R4	K16
      0x7C1C0200,  //  004B  CALL	R7	1
      0x001E3E07,  //  004C  ADD	R7	K31	R7
      0x54220003,  //  004D  LDINT	R8	4
      0x7C140600,  //  004E  CALL	R5	3
      0xB8161A00,  //  004F  GETNGBL	R5	K13
      0x8C140B0E,  //  0050  GETMET	R5	R5	K14
      0x881C011B,  //  0051  GETMBR	R7	R0	K27
      0x8C1C0F10,  //  0052  GETMET	R7	R7	K16
      0x7C1C0200,  //  0053  CALL	R7	1
      0x001E4007,  //  0054  ADD	R7	K32	R7
      0x54220003,  //  0055  LDINT	R8	4
      0x7C140600,  //  0056  CALL	R5	3
      0xB8161A00,  //  0057  GETNGBL	R5	K13
      0x8C140B0E,  //  0058  GETMET	R5	R5	K14
      0x881C011C,  //  0059  GETMBR	R7	R0	K28
      0x8C1C0F10,  //  005A  GETMET	R7	R7	K16
      0x7C1C0200,  //  005B  CALL	R7	1
      0x001E4207,  //  005C  ADD	R7	K33	R7
      0x54220003,  //  005D  LDINT	R8	4
      0x7C140600,  //  005E  CALL	R5	3
      0xB8161A00,  //  005F  GETNGBL	R5	K13
      0x8C140B0E,  //  0060  GETMET	R5	R5	K14
      0x881C011D,  //  0061  GETMBR	R7	R0	K29
      0x8C1C0F10,  //  0062  GETMET	R7	R7	K16
      0x7C1C0200,  //  0063  CALL	R7	1
      0x001E4407,  //  0064  ADD	R7	K34	R7
      0x54220003,  //  0065  LDINT	R8	4
      0x7C140600,  //  0066  CALL	R5	3
      0xB8161A00,  //  0067  GETNGBL	R5	K13
      0x8C140B0E,  //  0068  GETMET	R5	R5	K14
      0x581C001E,  //  0069  LDCONST	R7	K30
      0x54220003,  //  006A  LDINT	R8	4
      0x7C140600,  //  006B  CALL	R5	3
      0x8C140323,  //  006C  GETMET	R5	R1	K35
      0x541E003F,  //  006D  LDINT	R7	64
      0x50200000,  //  006E  LDBOOL	R8	0	0
      0x7C140600,  //  006F  CALL	R5	3
      0x60180015,  //  0070  GETGBL	R6	G21
      0x7C180000,  //  0071  CALL	R6	0
      0x8C1C0D24,  //  0072  GETMET	R7	R6	K36
      0x58240003,  //  0073  LDCONST	R9	K3
      0x58280025,  //  0074  LDCONST	R10	K37
      0x7C1C0600,  //  0075  CALL	R7	3
      0x8C1C0D24,  //  0076  GETMET	R7	R6	K36
      0x58240003,  //  0077  LDCONST	R9	K3
      0x542A0003,  //  0078  LDINT	R10	4
      0x7C1C0600,  //  0079  CALL	R7	3
      0x8C1C0D24,  //  007A  GETMET	R7	R6	K36
      0x58240003,  //  007B  LDCONST	R9	K3
      0x542A0003,  //  007C  LDINT	R10	4
      0x7C1C0600,  //  007D  CALL	R7	3
      0x8C1C0B26,  //  007E  GETMET	R7	R5	K38
      0x5C240C00,  //  007F  MOVE	R9	R6
      0x7C1C0400,  //  0080  CALL	R7	2
      0x88200127,  //  0081  GETMBR	R8	R0	K39
      0x8C201128,  //  0082  GETMET	R8	R8	K40
      0x5C280E00,  //  0083  MOVE	R10	R7
      0x882C0329,  //  0084  GETMBR	R11	R1	K41
      0x8830032A,  //  0085  GETMBR	R12	R1	K42
      0x4C340000,  //  0086  LDNIL	R13
      0x7C200A00,  //  0087  CALL	R8	5
      0x88200127,  //  0088  GETMBR	R8	R0	K39
      0x8C20112B,  //  0089  GETMET	R8	R8	K43
      0x8828012C,  //  008A  GETMBR	R10	R0	K44
      0x882C012D,  //  008B  GETMBR	R11	R0	K45
      0x8830011B,  //  008C  GETMBR	R12	R0	K27
      0x8834011C,  //  008D  GETMBR	R13	R0	K28
      0x8838011D,  //  008E  GETMBR	R14	R0	K29
      0x883C0113,  //  008F  GETMBR	R15	R0	K19
      0x7C200E00,  //  0090  CALL	R8	7
      0x80000000,  //  0091  RET	0
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(window_open),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20commissioning_X20not_X20open),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20received_X20message_X20),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(inspect),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(opcode),
    /* K10  */  be_nested_str_weak(parse_PBKDFParamRequest),
    /* K11  */  be_nested_str_weak(parse_Pake1),
    /* K12  */  be_nested_str_weak(parse_Pake3),
    /* K13  */  be_nested_str_weak(parse_Sigma1),
    /* K14  */  be_nested_str_weak(parse_Sigma3),
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0006,  //  0001  JMPT	R2	#0009
      0xB80A0200,  //  0002  GETNGBL	R2	K1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x58100003,  //  0004  LDCONST	R4	K3
      0x58140004,  //  0005  LDCONST	R5	K4
      0x7C080600,  //  0006  CALL	R2	3
      0x50080000,  //  0007  LDBOOL	R2	0	0
      0x80040400,  //  0008  RET	1	R2
      0xB80A0200,  //  0009  GETNGBL	R2	K1
      0x8C080502,  //  000A  GETMET	R2	R2	K2
      0xB8120C00,  //  000B  GETNGBL	R4	K6
      0x8C100907,  //  000C  GETMET	R4	R4	K7
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0x00120A04,  //  000F  ADD	R4	K5	R4
      0x58140008,  //  0010  LDCONST	R5	K8
      0x7C080600,  //  0011  CALL	R2	3
      0x88080309,  //  0012  GETMBR	R2	R1	K9
      0x540E001F,  //  0013  LDINT	R3	32
      0x1C080403,  //  0014  EQ	R2	R2	R3
      0x780A0004,  //  0015  JMPF	R2	#001B
      0x8C08010A,  //  0016  GETMET	R2	R0	K10
      0x5C100200,  //  0017  MOVE	R4	R1
      0x7C080400,  //  0018  CALL	R2	2
      0x80040400,  //  0019  RET	1	R2
      0x70020022,  //  001A  JMP		#003E
      0x88080309,  //  001B  GETMBR	R2	R1	K9
      0x540E0021,  //  001C  LDINT	R3	34
      0x1C080403,  //  001D  EQ	R2	R2	R3
      0x780A0004,  //  001E  JMPF	R2	#0024
      0x8C08010B,  //  001F  GETMET	R2	R0	K11
      0x5C100200,  //  0020  MOVE	R4	R1
      0x7C080400,  //  0021  CALL	R2	2
      0x80040400,  //  0022  RET	1	R2
      0x70020019,  //  0023  JMP		#003E
      0x88080309,  //  0024  GETMBR	R2	R1	K9
      0x540E0023,  //  0025  LDINT	R3	36
      0x1C080403,  //  0026  EQ	R2	R2	R3
      0x780A0004,  //  0027  JMPF	R2	#002D
      0x8C08010C,  //  0028  GETMET	R2	R0	K12
      0x5C100200,  //  0029  MOVE	R4	R1
      0x7C080400,  //  002A  CALL	R2	2
      0x80040400,  //  002B  RET	1	R2
      0x70020010,  //  002C  JMP		#003E
      0x88080309,  //  002D  GETMBR	R2	R1	K9
      0x540E002F,  //  002E  LDINT	R3	48
      0x1C080403,  //  002F  EQ	R2	R2	R3
      0x780A0004,  //  0030  JMPF	R2	#0036
      0x8C08010D,  //  0031  GETMET	R2	R0	K13
      0x5C100200,  //  0032  MOVE	R4	R1
      0x7C080400,  //  0033  CALL	R2	2
      0x80040400,  //  0034  RET	1	R2
      0x70020007,  //  0035  JMP		#003E
      0x88080309,  //  0036  GETMBR	R2	R1	K9
      0x540E0031,  //  0037  LDINT	R3	50
      0x1C080403,  //  0038  EQ	R2	R2	R3
      0x780A0003,  //  0039  JMPF	R2	#003E
      0x8C08010E,  //  003A  GETMET	R2	R0	K14
      0x5C100200,  //  003B  MOVE	R4	R1
      0x7C080400,  //  003C  CALL	R2	2
      0x80040400,  //  003D  RET	1	R2
      0x50080000,  //  003E  LDBOOL	R2	0	0
      0x80040400,  //  003F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Commisioning_Context
********************************************************************/
be_local_class(Matter_Commisioning_Context,
    21,
    NULL,
    be_nested_map(36,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(parse_PBKDFParamRequest, -1), be_const_closure(Matter_Commisioning_Context_parse_PBKDFParamRequest_closure) },
        { be_const_key_weak(cA, 32), be_const_var(10) },
        { be_const_key_weak(PBKDFParamRequest, -1), be_const_var(5) },
        { be_const_key_weak(parse_Pake3, 20), be_const_closure(Matter_Commisioning_Context_parse_Pake3_closure) },
        { be_const_key_weak(cB, -1), be_const_var(11) },
        { be_const_key_weak(spake, -1), be_const_var(2) },
        { be_const_key_weak(find_session_by_destination_id, 2), be_const_closure(Matter_Commisioning_Context_find_session_by_destination_id_closure) },
        { be_const_key_weak(R2IKey, -1), be_const_var(18) },
        { be_const_key_weak(Ke, -1), be_const_var(12) },
        { be_const_key_weak(parse_Sigma1, -1), be_const_closure(Matter_Commisioning_Context_parse_Sigma1_closure) },
        { be_const_key_weak(SEKeys_Info, -1), be_nested_str_weak(SessionKeys) },
        { be_const_key_weak(I2RKey, -1), be_const_var(17) },
        { be_const_key_weak(ResponderEph_priv, -1), be_const_var(13) },
        { be_const_key_weak(parse_Pake1, -1), be_const_closure(Matter_Commisioning_Context_parse_Pake1_closure) },
        { be_const_key_weak(AttestationChallenge, -1), be_const_var(19) },
        { be_const_key_weak(init, 3), be_const_closure(Matter_Commisioning_Context_init_closure) },
        { be_const_key_weak(ResponderEph_pub, 6), be_const_var(14) },
        { be_const_key_weak(initiatorEph_pub, -1), be_const_var(15) },
        { be_const_key_weak(window_open, -1), be_const_var(20) },
        { be_const_key_weak(session_timestamp, 26), be_const_var(16) },
        { be_const_key_weak(future_local_session_id, 34), be_const_var(4) },
        { be_const_key_weak(Matter_Context_Prefix, 27), be_nested_str_weak(CHIP_X20PAKE_X20V1_X20Commissioning) },
        { be_const_key_weak(pA, 30), be_const_var(8) },
        { be_const_key_weak(pB, -1), be_const_var(9) },
        { be_const_key_weak(TBEData2_Nonce, -1), be_nested_str_weak(NCASE_Sigma2N) },
        { be_const_key_weak(future_initiator_session_id, -1), be_const_var(3) },
        { be_const_key_weak(PBKDFParamResponse, -1), be_const_var(6) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Commisioning_Context_every_second_closure) },
        { be_const_key_weak(y, 19), be_const_var(7) },
        { be_const_key_weak(parse_Sigma3, 8), be_const_closure(Matter_Commisioning_Context_parse_Sigma3_closure) },
        { be_const_key_weak(S2K_Info, -1), be_nested_str_weak(Sigma2) },
        { be_const_key_weak(TBEData3_Nonce, 9), be_nested_str_weak(NCASE_Sigma3N) },
        { be_const_key_weak(responder, -1), be_const_var(0) },
        { be_const_key_weak(S3K_Info, -1), be_nested_str_weak(Sigma3) },
        { be_const_key_weak(device, -1), be_const_var(1) },
        { be_const_key_weak(process_incoming, -1), be_const_closure(Matter_Commisioning_Context_process_incoming_closure) },
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
