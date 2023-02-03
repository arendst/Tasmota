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
    4,                          /* argc */
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
    /* K38  */  be_const_int(3),
    /* K39  */  be_nested_str_weak(encode),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20pbkdfparamresp_raw_X3A_X20),
    /* K41  */  be_nested_str_weak(tohex),
    /* K42  */  be_nested_str_weak(build_response),
    /* K43  */  be_nested_str_weak(responder),
    /* K44  */  be_nested_str_weak(send_response),
    /* K45  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x88140301,  //  0001  GETMBR	R5	R1	K1
      0x541A001F,  //  0002  LDINT	R6	32
      0x20140A06,  //  0003  NE	R5	R5	R6
      0x74160005,  //  0004  JMPT	R5	#000B
      0x88140302,  //  0005  GETMBR	R5	R1	K2
      0x20140B03,  //  0006  NE	R5	R5	K3
      0x74160002,  //  0007  JMPT	R5	#000B
      0x88140304,  //  0008  GETMBR	R5	R1	K4
      0x20140B03,  //  0009  NE	R5	R5	K3
      0x78160000,  //  000A  JMPF	R5	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0xB8160E00,  //  000C  GETNGBL	R5	K7
      0x8C140B08,  //  000D  GETMET	R5	R5	K8
      0x7C140200,  //  000E  CALL	R5	1
      0x8C140B09,  //  000F  GETMET	R5	R5	K9
      0x881C030A,  //  0010  GETMBR	R7	R1	K10
      0x8820030B,  //  0011  GETMBR	R8	R1	K11
      0x7C140600,  //  0012  CALL	R5	3
      0x8818030C,  //  0013  GETMBR	R6	R1	K12
      0x8C180D0D,  //  0014  GETMET	R6	R6	K13
      0xB8220E00,  //  0015  GETNGBL	R8	K7
      0x8820110E,  //  0016  GETMBR	R8	R8	K14
      0x8820110F,  //  0017  GETMBR	R8	R8	K15
      0x7C180400,  //  0018  CALL	R6	2
      0x8818030B,  //  0019  GETMBR	R6	R1	K11
      0x40180D10,  //  001A  CONNECT	R6	R6	K16
      0x881C030A,  //  001B  GETMBR	R7	R1	K10
      0x94180E06,  //  001C  GETIDX	R6	R7	R6
      0x90021006,  //  001D  SETMBR	R0	K8	R6
      0x88180B11,  //  001E  GETMBR	R6	R5	K17
      0x20180D03,  //  001F  NE	R6	R6	K3
      0x781A0000,  //  0020  JMPF	R6	#0022
      0xB0060B12,  //  0021  RAISE	1	K5	K18
      0x88180B14,  //  0022  GETMBR	R6	R5	K20
      0x90022606,  //  0023  SETMBR	R0	K19	R6
      0x88180116,  //  0024  GETMBR	R6	R0	K22
      0x88180D17,  //  0025  GETMBR	R6	R6	K23
      0x8C180D18,  //  0026  GETMET	R6	R6	K24
      0x7C180200,  //  0027  CALL	R6	1
      0x90022A06,  //  0028  SETMBR	R0	K21	R6
      0xB81A0E00,  //  0029  GETNGBL	R6	K7
      0x8C180D19,  //  002A  GETMET	R6	R6	K25
      0x7C180200,  //  002B  CALL	R6	1
      0x881C0B1A,  //  002C  GETMBR	R7	R5	K26
      0x901A3407,  //  002D  SETMBR	R6	K26	R7
      0x8C1C091C,  //  002E  GETMET	R7	R4	K28
      0x5426001F,  //  002F  LDINT	R9	32
      0x7C1C0400,  //  0030  CALL	R7	2
      0x901A3607,  //  0031  SETMBR	R6	K27	R7
      0x881C0115,  //  0032  GETMBR	R7	R0	K21
      0x901A3A07,  //  0033  SETMBR	R6	K29	R7
      0x881C0116,  //  0034  GETMBR	R7	R0	K22
      0x881C0F1F,  //  0035  GETMBR	R7	R7	K31
      0x901A3C07,  //  0036  SETMBR	R6	K30	R7
      0x881C0116,  //  0037  GETMBR	R7	R0	K22
      0x881C0F21,  //  0038  GETMBR	R7	R7	K33
      0x901A4007,  //  0039  SETMBR	R6	K32	R7
      0xB81E4400,  //  003A  GETNGBL	R7	K34
      0x8C1C0F23,  //  003B  GETMET	R7	R7	K35
      0x60240008,  //  003C  GETGBL	R9	G8
      0xB82A0E00,  //  003D  GETNGBL	R10	K7
      0x8C281525,  //  003E  GETMET	R10	R10	K37
      0x5C300C00,  //  003F  MOVE	R12	R6
      0x7C280400,  //  0040  CALL	R10	2
      0x7C240200,  //  0041  CALL	R9	1
      0x00264809,  //  0042  ADD	R9	K36	R9
      0x58280026,  //  0043  LDCONST	R10	K38
      0x7C1C0600,  //  0044  CALL	R7	3
      0x8C1C0D27,  //  0045  GETMET	R7	R6	K39
      0x7C1C0200,  //  0046  CALL	R7	1
      0xB8224400,  //  0047  GETNGBL	R8	K34
      0x8C201123,  //  0048  GETMET	R8	R8	K35
      0x8C280F29,  //  0049  GETMET	R10	R7	K41
      0x7C280200,  //  004A  CALL	R10	1
      0x002A500A,  //  004B  ADD	R10	K40	R10
      0x582C0026,  //  004C  LDCONST	R11	K38
      0x7C200600,  //  004D  CALL	R8	3
      0x90023207,  //  004E  SETMBR	R0	K25	R7
      0x8C20032A,  //  004F  GETMET	R8	R1	K42
      0x542A0020,  //  0050  LDINT	R10	33
      0x502C0200,  //  0051  LDBOOL	R11	1	0
      0x7C200600,  //  0052  CALL	R8	3
      0x8C241127,  //  0053  GETMET	R9	R8	K39
      0x5C2C0E00,  //  0054  MOVE	R11	R7
      0x7C240400,  //  0055  CALL	R9	2
      0x8828012B,  //  0056  GETMBR	R10	R0	K43
      0x8C28152C,  //  0057  GETMET	R10	R10	K44
      0x5C301200,  //  0058  MOVE	R12	R9
      0x5C340400,  //  0059  MOVE	R13	R2
      0x5C380600,  //  005A  MOVE	R14	R3
      0x883C112D,  //  005B  GETMBR	R15	R8	K45
      0x7C280A00,  //  005C  CALL	R10	5
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
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[82]) {     /* constants */
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
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20spake_X3A_X20),
    /* K19  */  be_nested_str_weak(inspect),
    /* K20  */  be_nested_str_weak(spake),
    /* K21  */  be_nested_str_weak(SPAKE2P_Matter),
    /* K22  */  be_nested_str_weak(device),
    /* K23  */  be_nested_str_weak(w0),
    /* K24  */  be_nested_str_weak(w1),
    /* K25  */  be_nested_str_weak(L),
    /* K26  */  be_nested_str_weak(compute_pB),
    /* K27  */  be_nested_str_weak(y),
    /* K28  */  be_nested_str_weak(pB),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20y_X3D),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20pb_X3D),
    /* K31  */  be_nested_str_weak(compute_ZV_verifier),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20Z_X3D),
    /* K33  */  be_nested_str_weak(Z),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20V_X3D),
    /* K35  */  be_nested_str_weak(V),
    /* K36  */  be_nested_str_weak(SHA256),
    /* K37  */  be_nested_str_weak(update),
    /* K38  */  be_nested_str_weak(fromstring),
    /* K39  */  be_nested_str_weak(Matter_Context_Prefix),
    /* K40  */  be_nested_str_weak(PBKDFParamRequest),
    /* K41  */  be_nested_str_weak(PBKDFParamResponse),
    /* K42  */  be_nested_str_weak(out),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20Context_X3D),
    /* K44  */  be_nested_str_weak(set_context),
    /* K45  */  be_nested_str_weak(compute_TT_hash),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20Context_X20_X3D_X20),
    /* K48  */  be_nested_str_weak(Context),
    /* K49  */  be_nested_str_weak(MTR_X3A_X20A_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K50  */  be_nested_str_weak(A),
    /* K51  */  be_nested_str_weak(MTR_X3A_X20B_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K52  */  be_nested_str_weak(B),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20M_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K54  */  be_nested_str_weak(M),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20N_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K56  */  be_nested_str_weak(N),
    /* K57  */  be_nested_str_weak(MTR_X3A_X20pA_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K58  */  be_nested_str_weak(MTR_X3A_X20pB_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K59  */  be_nested_str_weak(MTR_X3A_X20Z_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K60  */  be_nested_str_weak(MTR_X3A_X20V_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K61  */  be_nested_str_weak(MTR_X3A_X20w0_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K62  */  be_nested_str_weak(MTR_X3A_X20Kmain_X20_X20_X20_X3D),
    /* K63  */  be_nested_str_weak(Kmain),
    /* K64  */  be_nested_str_weak(MTR_X3A_X20KcA_X20_X20_X20_X20_X20_X3D),
    /* K65  */  be_nested_str_weak(KcA),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20KcB_X20_X20_X20_X20_X20_X3D),
    /* K67  */  be_nested_str_weak(KcB),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20K_shared_X3D),
    /* K69  */  be_nested_str_weak(K_shared),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20Ke_X20_X20_X20_X20_X20_X20_X3D),
    /* K71  */  be_nested_str_weak(Ke),
    /* K72  */  be_nested_str_weak(cB),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20cB_X3D),
    /* K74  */  be_nested_str_weak(Pake2),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20pake2_X3A_X20),
    /* K76  */  be_nested_str_weak(encode),
    /* K77  */  be_nested_str_weak(MTR_X3A_X20pake2_raw_X3A_X20),
    /* K78  */  be_nested_str_weak(build_response),
    /* K79  */  be_nested_str_weak(responder),
    /* K80  */  be_nested_str_weak(send_response),
    /* K81  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[326]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x88140301,  //  0001  GETMBR	R5	R1	K1
      0x541A0021,  //  0002  LDINT	R6	34
      0x20140A06,  //  0003  NE	R5	R5	R6
      0x74160005,  //  0004  JMPT	R5	#000B
      0x88140302,  //  0005  GETMBR	R5	R1	K2
      0x20140B03,  //  0006  NE	R5	R5	K3
      0x74160002,  //  0007  JMPT	R5	#000B
      0x88140304,  //  0008  GETMBR	R5	R1	K4
      0x20140B03,  //  0009  NE	R5	R5	K3
      0x78160000,  //  000A  JMPF	R5	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0xB8160E00,  //  000C  GETNGBL	R5	K7
      0x8C140B08,  //  000D  GETMET	R5	R5	K8
      0x7C140200,  //  000E  CALL	R5	1
      0x8C140B09,  //  000F  GETMET	R5	R5	K9
      0x881C030A,  //  0010  GETMBR	R7	R1	K10
      0x8820030B,  //  0011  GETMBR	R8	R1	K11
      0x7C140600,  //  0012  CALL	R5	3
      0x88180B0C,  //  0013  GETMBR	R6	R5	K12
      0x90021806,  //  0014  SETMBR	R0	K12	R6
      0xB81A1A00,  //  0015  GETNGBL	R6	K13
      0x8C180D0E,  //  0016  GETMET	R6	R6	K14
      0x8820010C,  //  0017  GETMBR	R8	R0	K12
      0x8C201110,  //  0018  GETMET	R8	R8	K16
      0x7C200200,  //  0019  CALL	R8	1
      0x00221E08,  //  001A  ADD	R8	K15	R8
      0x58240011,  //  001B  LDCONST	R9	K17
      0x7C180600,  //  001C  CALL	R6	3
      0xB81A1A00,  //  001D  GETNGBL	R6	K13
      0x8C180D0E,  //  001E  GETMET	R6	R6	K14
      0xB8220E00,  //  001F  GETNGBL	R8	K7
      0x8C201113,  //  0020  GETMET	R8	R8	K19
      0x88280114,  //  0021  GETMBR	R10	R0	K20
      0x7C200400,  //  0022  CALL	R8	2
      0x00222408,  //  0023  ADD	R8	K18	R8
      0x58240011,  //  0024  LDCONST	R9	K17
      0x7C180600,  //  0025  CALL	R6	3
      0x8C180915,  //  0026  GETMET	R6	R4	K21
      0x88200116,  //  0027  GETMBR	R8	R0	K22
      0x88201117,  //  0028  GETMBR	R8	R8	K23
      0x88240116,  //  0029  GETMBR	R9	R0	K22
      0x88241318,  //  002A  GETMBR	R9	R9	K24
      0x88280116,  //  002B  GETMBR	R10	R0	K22
      0x88281519,  //  002C  GETMBR	R10	R10	K25
      0x7C180800,  //  002D  CALL	R6	4
      0x90022806,  //  002E  SETMBR	R0	K20	R6
      0x88180114,  //  002F  GETMBR	R6	R0	K20
      0x8C180D1A,  //  0030  GETMET	R6	R6	K26
      0x8820011B,  //  0031  GETMBR	R8	R0	K27
      0x7C180400,  //  0032  CALL	R6	2
      0x88180114,  //  0033  GETMBR	R6	R0	K20
      0x88180D1C,  //  0034  GETMBR	R6	R6	K28
      0x90023806,  //  0035  SETMBR	R0	K28	R6
      0xB81A1A00,  //  0036  GETNGBL	R6	K13
      0x8C180D0E,  //  0037  GETMET	R6	R6	K14
      0x8820011B,  //  0038  GETMBR	R8	R0	K27
      0x8C201110,  //  0039  GETMET	R8	R8	K16
      0x7C200200,  //  003A  CALL	R8	1
      0x00223A08,  //  003B  ADD	R8	K29	R8
      0x58240011,  //  003C  LDCONST	R9	K17
      0x7C180600,  //  003D  CALL	R6	3
      0xB81A1A00,  //  003E  GETNGBL	R6	K13
      0x8C180D0E,  //  003F  GETMET	R6	R6	K14
      0x8820011C,  //  0040  GETMBR	R8	R0	K28
      0x8C201110,  //  0041  GETMET	R8	R8	K16
      0x7C200200,  //  0042  CALL	R8	1
      0x00223C08,  //  0043  ADD	R8	K30	R8
      0x58240011,  //  0044  LDCONST	R9	K17
      0x7C180600,  //  0045  CALL	R6	3
      0x88180114,  //  0046  GETMBR	R6	R0	K20
      0x8C180D1F,  //  0047  GETMET	R6	R6	K31
      0x8820010C,  //  0048  GETMBR	R8	R0	K12
      0x7C180400,  //  0049  CALL	R6	2
      0xB81A1A00,  //  004A  GETNGBL	R6	K13
      0x8C180D0E,  //  004B  GETMET	R6	R6	K14
      0x88200114,  //  004C  GETMBR	R8	R0	K20
      0x88201121,  //  004D  GETMBR	R8	R8	K33
      0x8C201110,  //  004E  GETMET	R8	R8	K16
      0x7C200200,  //  004F  CALL	R8	1
      0x00224008,  //  0050  ADD	R8	K32	R8
      0x58240011,  //  0051  LDCONST	R9	K17
      0x7C180600,  //  0052  CALL	R6	3
      0xB81A1A00,  //  0053  GETNGBL	R6	K13
      0x8C180D0E,  //  0054  GETMET	R6	R6	K14
      0x88200114,  //  0055  GETMBR	R8	R0	K20
      0x88201123,  //  0056  GETMBR	R8	R8	K35
      0x8C201110,  //  0057  GETMET	R8	R8	K16
      0x7C200200,  //  0058  CALL	R8	1
      0x00224408,  //  0059  ADD	R8	K34	R8
      0x58240011,  //  005A  LDCONST	R9	K17
      0x7C180600,  //  005B  CALL	R6	3
      0x8C180924,  //  005C  GETMET	R6	R4	K36
      0x7C180200,  //  005D  CALL	R6	1
      0x8C1C0D25,  //  005E  GETMET	R7	R6	K37
      0x60240015,  //  005F  GETGBL	R9	G21
      0x7C240000,  //  0060  CALL	R9	0
      0x8C241326,  //  0061  GETMET	R9	R9	K38
      0x882C0127,  //  0062  GETMBR	R11	R0	K39
      0x7C240400,  //  0063  CALL	R9	2
      0x7C1C0400,  //  0064  CALL	R7	2
      0x8C1C0D25,  //  0065  GETMET	R7	R6	K37
      0x88240128,  //  0066  GETMBR	R9	R0	K40
      0x7C1C0400,  //  0067  CALL	R7	2
      0x8C1C0D25,  //  0068  GETMET	R7	R6	K37
      0x88240129,  //  0069  GETMBR	R9	R0	K41
      0x7C1C0400,  //  006A  CALL	R7	2
      0x8C1C0D2A,  //  006B  GETMET	R7	R6	K42
      0x7C1C0200,  //  006C  CALL	R7	1
      0xB8221A00,  //  006D  GETNGBL	R8	K13
      0x8C20110E,  //  006E  GETMET	R8	R8	K14
      0x8C280F10,  //  006F  GETMET	R10	R7	K16
      0x7C280200,  //  0070  CALL	R10	1
      0x002A560A,  //  0071  ADD	R10	K43	R10
      0x582C0011,  //  0072  LDCONST	R11	K17
      0x7C200600,  //  0073  CALL	R8	3
      0x88200114,  //  0074  GETMBR	R8	R0	K20
      0x8824010C,  //  0075  GETMBR	R9	R0	K12
      0x90221809,  //  0076  SETMBR	R8	K12	R9
      0x88200114,  //  0077  GETMBR	R8	R0	K20
      0x8C20112C,  //  0078  GETMET	R8	R8	K44
      0x5C280E00,  //  0079  MOVE	R10	R7
      0x7C200400,  //  007A  CALL	R8	2
      0x88200114,  //  007B  GETMBR	R8	R0	K20
      0x8C20112D,  //  007C  GETMET	R8	R8	K45
      0x50280200,  //  007D  LDBOOL	R10	1	0
      0x7C200400,  //  007E  CALL	R8	2
      0xB8221A00,  //  007F  GETNGBL	R8	K13
      0x8C20110E,  //  0080  GETMET	R8	R8	K14
      0x5828002E,  //  0081  LDCONST	R10	K46
      0x582C0011,  //  0082  LDCONST	R11	K17
      0x7C200600,  //  0083  CALL	R8	3
      0xB8221A00,  //  0084  GETNGBL	R8	K13
      0x8C20110E,  //  0085  GETMET	R8	R8	K14
      0x88280114,  //  0086  GETMBR	R10	R0	K20
      0x88281530,  //  0087  GETMBR	R10	R10	K48
      0x8C281510,  //  0088  GETMET	R10	R10	K16
      0x7C280200,  //  0089  CALL	R10	1
      0x002A5E0A,  //  008A  ADD	R10	K47	R10
      0x582C0011,  //  008B  LDCONST	R11	K17
      0x7C200600,  //  008C  CALL	R8	3
      0xB8221A00,  //  008D  GETNGBL	R8	K13
      0x8C20110E,  //  008E  GETMET	R8	R8	K14
      0x88280114,  //  008F  GETMBR	R10	R0	K20
      0x88281532,  //  0090  GETMBR	R10	R10	K50
      0x8C281510,  //  0091  GETMET	R10	R10	K16
      0x7C280200,  //  0092  CALL	R10	1
      0x002A620A,  //  0093  ADD	R10	K49	R10
      0x582C0011,  //  0094  LDCONST	R11	K17
      0x7C200600,  //  0095  CALL	R8	3
      0xB8221A00,  //  0096  GETNGBL	R8	K13
      0x8C20110E,  //  0097  GETMET	R8	R8	K14
      0x88280114,  //  0098  GETMBR	R10	R0	K20
      0x88281534,  //  0099  GETMBR	R10	R10	K52
      0x8C281510,  //  009A  GETMET	R10	R10	K16
      0x7C280200,  //  009B  CALL	R10	1
      0x002A660A,  //  009C  ADD	R10	K51	R10
      0x582C0011,  //  009D  LDCONST	R11	K17
      0x7C200600,  //  009E  CALL	R8	3
      0xB8221A00,  //  009F  GETNGBL	R8	K13
      0x8C20110E,  //  00A0  GETMET	R8	R8	K14
      0x88280114,  //  00A1  GETMBR	R10	R0	K20
      0x88281536,  //  00A2  GETMBR	R10	R10	K54
      0x8C281510,  //  00A3  GETMET	R10	R10	K16
      0x7C280200,  //  00A4  CALL	R10	1
      0x002A6A0A,  //  00A5  ADD	R10	K53	R10
      0x582C0011,  //  00A6  LDCONST	R11	K17
      0x7C200600,  //  00A7  CALL	R8	3
      0xB8221A00,  //  00A8  GETNGBL	R8	K13
      0x8C20110E,  //  00A9  GETMET	R8	R8	K14
      0x88280114,  //  00AA  GETMBR	R10	R0	K20
      0x88281538,  //  00AB  GETMBR	R10	R10	K56
      0x8C281510,  //  00AC  GETMET	R10	R10	K16
      0x7C280200,  //  00AD  CALL	R10	1
      0x002A6E0A,  //  00AE  ADD	R10	K55	R10
      0x582C0011,  //  00AF  LDCONST	R11	K17
      0x7C200600,  //  00B0  CALL	R8	3
      0xB8221A00,  //  00B1  GETNGBL	R8	K13
      0x8C20110E,  //  00B2  GETMET	R8	R8	K14
      0x88280114,  //  00B3  GETMBR	R10	R0	K20
      0x8828150C,  //  00B4  GETMBR	R10	R10	K12
      0x8C281510,  //  00B5  GETMET	R10	R10	K16
      0x7C280200,  //  00B6  CALL	R10	1
      0x002A720A,  //  00B7  ADD	R10	K57	R10
      0x582C0011,  //  00B8  LDCONST	R11	K17
      0x7C200600,  //  00B9  CALL	R8	3
      0xB8221A00,  //  00BA  GETNGBL	R8	K13
      0x8C20110E,  //  00BB  GETMET	R8	R8	K14
      0x88280114,  //  00BC  GETMBR	R10	R0	K20
      0x8828151C,  //  00BD  GETMBR	R10	R10	K28
      0x8C281510,  //  00BE  GETMET	R10	R10	K16
      0x7C280200,  //  00BF  CALL	R10	1
      0x002A740A,  //  00C0  ADD	R10	K58	R10
      0x582C0011,  //  00C1  LDCONST	R11	K17
      0x7C200600,  //  00C2  CALL	R8	3
      0xB8221A00,  //  00C3  GETNGBL	R8	K13
      0x8C20110E,  //  00C4  GETMET	R8	R8	K14
      0x88280114,  //  00C5  GETMBR	R10	R0	K20
      0x88281521,  //  00C6  GETMBR	R10	R10	K33
      0x8C281510,  //  00C7  GETMET	R10	R10	K16
      0x7C280200,  //  00C8  CALL	R10	1
      0x002A760A,  //  00C9  ADD	R10	K59	R10
      0x582C0011,  //  00CA  LDCONST	R11	K17
      0x7C200600,  //  00CB  CALL	R8	3
      0xB8221A00,  //  00CC  GETNGBL	R8	K13
      0x8C20110E,  //  00CD  GETMET	R8	R8	K14
      0x88280114,  //  00CE  GETMBR	R10	R0	K20
      0x88281523,  //  00CF  GETMBR	R10	R10	K35
      0x8C281510,  //  00D0  GETMET	R10	R10	K16
      0x7C280200,  //  00D1  CALL	R10	1
      0x002A780A,  //  00D2  ADD	R10	K60	R10
      0x582C0011,  //  00D3  LDCONST	R11	K17
      0x7C200600,  //  00D4  CALL	R8	3
      0xB8221A00,  //  00D5  GETNGBL	R8	K13
      0x8C20110E,  //  00D6  GETMET	R8	R8	K14
      0x88280114,  //  00D7  GETMBR	R10	R0	K20
      0x88281517,  //  00D8  GETMBR	R10	R10	K23
      0x8C281510,  //  00D9  GETMET	R10	R10	K16
      0x7C280200,  //  00DA  CALL	R10	1
      0x002A7A0A,  //  00DB  ADD	R10	K61	R10
      0x582C0011,  //  00DC  LDCONST	R11	K17
      0x7C200600,  //  00DD  CALL	R8	3
      0xB8221A00,  //  00DE  GETNGBL	R8	K13
      0x8C20110E,  //  00DF  GETMET	R8	R8	K14
      0x5828002E,  //  00E0  LDCONST	R10	K46
      0x582C0011,  //  00E1  LDCONST	R11	K17
      0x7C200600,  //  00E2  CALL	R8	3
      0xB8221A00,  //  00E3  GETNGBL	R8	K13
      0x8C20110E,  //  00E4  GETMET	R8	R8	K14
      0x88280114,  //  00E5  GETMBR	R10	R0	K20
      0x8828153F,  //  00E6  GETMBR	R10	R10	K63
      0x8C281510,  //  00E7  GETMET	R10	R10	K16
      0x7C280200,  //  00E8  CALL	R10	1
      0x002A7C0A,  //  00E9  ADD	R10	K62	R10
      0x582C0011,  //  00EA  LDCONST	R11	K17
      0x7C200600,  //  00EB  CALL	R8	3
      0xB8221A00,  //  00EC  GETNGBL	R8	K13
      0x8C20110E,  //  00ED  GETMET	R8	R8	K14
      0x88280114,  //  00EE  GETMBR	R10	R0	K20
      0x88281541,  //  00EF  GETMBR	R10	R10	K65
      0x8C281510,  //  00F0  GETMET	R10	R10	K16
      0x7C280200,  //  00F1  CALL	R10	1
      0x002A800A,  //  00F2  ADD	R10	K64	R10
      0x582C0011,  //  00F3  LDCONST	R11	K17
      0x7C200600,  //  00F4  CALL	R8	3
      0xB8221A00,  //  00F5  GETNGBL	R8	K13
      0x8C20110E,  //  00F6  GETMET	R8	R8	K14
      0x88280114,  //  00F7  GETMBR	R10	R0	K20
      0x88281543,  //  00F8  GETMBR	R10	R10	K67
      0x8C281510,  //  00F9  GETMET	R10	R10	K16
      0x7C280200,  //  00FA  CALL	R10	1
      0x002A840A,  //  00FB  ADD	R10	K66	R10
      0x582C0011,  //  00FC  LDCONST	R11	K17
      0x7C200600,  //  00FD  CALL	R8	3
      0xB8221A00,  //  00FE  GETNGBL	R8	K13
      0x8C20110E,  //  00FF  GETMET	R8	R8	K14
      0x88280114,  //  0100  GETMBR	R10	R0	K20
      0x88281545,  //  0101  GETMBR	R10	R10	K69
      0x8C281510,  //  0102  GETMET	R10	R10	K16
      0x7C280200,  //  0103  CALL	R10	1
      0x002A880A,  //  0104  ADD	R10	K68	R10
      0x582C0011,  //  0105  LDCONST	R11	K17
      0x7C200600,  //  0106  CALL	R8	3
      0xB8221A00,  //  0107  GETNGBL	R8	K13
      0x8C20110E,  //  0108  GETMET	R8	R8	K14
      0x88280114,  //  0109  GETMBR	R10	R0	K20
      0x88281547,  //  010A  GETMBR	R10	R10	K71
      0x8C281510,  //  010B  GETMET	R10	R10	K16
      0x7C280200,  //  010C  CALL	R10	1
      0x002A8C0A,  //  010D  ADD	R10	K70	R10
      0x582C0011,  //  010E  LDCONST	R11	K17
      0x7C200600,  //  010F  CALL	R8	3
      0x88200114,  //  0110  GETMBR	R8	R0	K20
      0x88201148,  //  0111  GETMBR	R8	R8	K72
      0x90029008,  //  0112  SETMBR	R0	K72	R8
      0x88200114,  //  0113  GETMBR	R8	R0	K20
      0x88201147,  //  0114  GETMBR	R8	R8	K71
      0x90028E08,  //  0115  SETMBR	R0	K71	R8
      0xB8221A00,  //  0116  GETNGBL	R8	K13
      0x8C20110E,  //  0117  GETMET	R8	R8	K14
      0x88280148,  //  0118  GETMBR	R10	R0	K72
      0x8C281510,  //  0119  GETMET	R10	R10	K16
      0x7C280200,  //  011A  CALL	R10	1
      0x002A920A,  //  011B  ADD	R10	K73	R10
      0x582C0011,  //  011C  LDCONST	R11	K17
      0x7C200600,  //  011D  CALL	R8	3
      0xB8220E00,  //  011E  GETNGBL	R8	K7
      0x8C20114A,  //  011F  GETMET	R8	R8	K74
      0x7C200200,  //  0120  CALL	R8	1
      0x8824011C,  //  0121  GETMBR	R9	R0	K28
      0x90223809,  //  0122  SETMBR	R8	K28	R9
      0x88240148,  //  0123  GETMBR	R9	R0	K72
      0x90229009,  //  0124  SETMBR	R8	K72	R9
      0xB8261A00,  //  0125  GETNGBL	R9	K13
      0x8C24130E,  //  0126  GETMET	R9	R9	K14
      0xB82E0E00,  //  0127  GETNGBL	R11	K7
      0x8C2C1713,  //  0128  GETMET	R11	R11	K19
      0x5C341000,  //  0129  MOVE	R13	R8
      0x7C2C0400,  //  012A  CALL	R11	2
      0x002E960B,  //  012B  ADD	R11	K75	R11
      0x58300011,  //  012C  LDCONST	R12	K17
      0x7C240600,  //  012D  CALL	R9	3
      0x8C24114C,  //  012E  GETMET	R9	R8	K76
      0x7C240200,  //  012F  CALL	R9	1
      0xB82A1A00,  //  0130  GETNGBL	R10	K13
      0x8C28150E,  //  0131  GETMET	R10	R10	K14
      0x8C301310,  //  0132  GETMET	R12	R9	K16
      0x7C300200,  //  0133  CALL	R12	1
      0x00329A0C,  //  0134  ADD	R12	K77	R12
      0x58340011,  //  0135  LDCONST	R13	K17
      0x7C280600,  //  0136  CALL	R10	3
      0x8C28034E,  //  0137  GETMET	R10	R1	K78
      0x54320022,  //  0138  LDINT	R12	35
      0x50340200,  //  0139  LDBOOL	R13	1	0
      0x7C280600,  //  013A  CALL	R10	3
      0x8C2C154C,  //  013B  GETMET	R11	R10	K76
      0x5C341200,  //  013C  MOVE	R13	R9
      0x7C2C0400,  //  013D  CALL	R11	2
      0x8830014F,  //  013E  GETMBR	R12	R0	K79
      0x8C301950,  //  013F  GETMET	R12	R12	K80
      0x5C381600,  //  0140  MOVE	R14	R11
      0x5C3C0400,  //  0141  MOVE	R15	R2
      0x5C400600,  //  0142  MOVE	R16	R3
      0x88441551,  //  0143  GETMBR	R17	R10	K81
      0x7C300A00,  //  0144  CALL	R12	5
      0x80000000,  //  0145  RET	0
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
      0x582C0005,  //  0034  LDCONST	R11	K5
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
    40,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[92]) {     /* constants */
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
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(SHA256),
    /* K18  */  be_nested_str_weak(update),
    /* K19  */  be_nested_str_weak(_Msg1),
    /* K20  */  be_nested_str_weak(_Msg2),
    /* K21  */  be_nested_str_weak(out),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20session_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20session_X2Eipk_epoch_key_X20),
    /* K24  */  be_nested_str_weak(ipk_epoch_key),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20session_X2Efabric_compressed_X20),
    /* K26  */  be_nested_str_weak(fabric_compressed),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20ipk_group_key_X20_X3D_X20),
    /* K28  */  be_nested_str_weak(get_ipk_group_key),
    /* K29  */  be_nested_str_weak(tohex),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TranscriptHash_X3D_X20),
    /* K31  */  be_nested_str_weak(fromstring),
    /* K32  */  be_nested_str_weak(S3K_Info),
    /* K33  */  be_nested_str_weak(HKDF_SHA256),
    /* K34  */  be_nested_str_weak(derive),
    /* K35  */  be_nested_str_weak(shared_secret),
    /* K36  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s3k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s3k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K38  */  be_nested_str_weak(TBEData3Encrypted),
    /* K39  */  be_const_int(2147483647),
    /* K40  */  be_nested_str_weak(AES_CCM),
    /* K41  */  be_nested_str_weak(TBEData3_Nonce),
    /* K42  */  be_nested_str_weak(decrypt),
    /* K43  */  be_nested_str_weak(tag),
    /* K44  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData3_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBETag3_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20tag_sent_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K47  */  be_nested_str_weak(value_error),
    /* K48  */  be_nested_str_weak(tag_X20do_X20not_X20match),
    /* K49  */  be_nested_str_weak(TLV),
    /* K50  */  be_nested_str_weak(findsubval),
    /* K51  */  be_const_int(1),
    /* K52  */  be_const_int(2),
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
    /* K86  */  be_nested_str_weak(message_counter),
    /* K87  */  be_nested_str_weak(close),
    /* K88  */  be_nested_str_weak(set_keys),
    /* K89  */  be_nested_str_weak(set_persist),
    /* K90  */  be_nested_str_weak(set_no_expiration),
    /* K91  */  be_nested_str_weak(save),
    }),
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[445]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x88140301,  //  0001  GETMBR	R5	R1	K1
      0x541A0031,  //  0002  LDINT	R6	50
      0x20140A06,  //  0003  NE	R5	R5	R6
      0x74160005,  //  0004  JMPT	R5	#000B
      0x88140302,  //  0005  GETMBR	R5	R1	K2
      0x20140B03,  //  0006  NE	R5	R5	K3
      0x74160002,  //  0007  JMPT	R5	#000B
      0x88140304,  //  0008  GETMBR	R5	R1	K4
      0x20140B03,  //  0009  NE	R5	R5	K3
      0x78160000,  //  000A  JMPF	R5	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0x88140307,  //  000C  GETMBR	R5	R1	K7
      0xB81A1000,  //  000D  GETNGBL	R6	K8
      0x8C180D09,  //  000E  GETMET	R6	R6	K9
      0x7C180200,  //  000F  CALL	R6	1
      0x8C180D0A,  //  0010  GETMET	R6	R6	K10
      0x8820030B,  //  0011  GETMBR	R8	R1	K11
      0x8824030C,  //  0012  GETMBR	R9	R1	K12
      0x7C180600,  //  0013  CALL	R6	3
      0xB81E1A00,  //  0014  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0015  GETMET	R7	R7	K14
      0x5824000F,  //  0016  LDCONST	R9	K15
      0x58280010,  //  0017  LDCONST	R10	K16
      0x7C1C0600,  //  0018  CALL	R7	3
      0x8C1C0911,  //  0019  GETMET	R7	R4	K17
      0x7C1C0200,  //  001A  CALL	R7	1
      0x8C1C0F12,  //  001B  GETMET	R7	R7	K18
      0x88240B13,  //  001C  GETMBR	R9	R5	K19
      0x7C1C0400,  //  001D  CALL	R7	2
      0x8C1C0F12,  //  001E  GETMET	R7	R7	K18
      0x88240B14,  //  001F  GETMBR	R9	R5	K20
      0x7C1C0400,  //  0020  CALL	R7	2
      0x8C1C0F15,  //  0021  GETMET	R7	R7	K21
      0x7C1C0200,  //  0022  CALL	R7	1
      0xB8221A00,  //  0023  GETNGBL	R8	K13
      0x8C20110E,  //  0024  GETMET	R8	R8	K14
      0x60280008,  //  0025  GETGBL	R10	G8
      0x5C2C0A00,  //  0026  MOVE	R11	R5
      0x7C280200,  //  0027  CALL	R10	1
      0x002A2C0A,  //  0028  ADD	R10	K22	R10
      0x582C0010,  //  0029  LDCONST	R11	K16
      0x7C200600,  //  002A  CALL	R8	3
      0xB8221A00,  //  002B  GETNGBL	R8	K13
      0x8C20110E,  //  002C  GETMET	R8	R8	K14
      0x60280008,  //  002D  GETGBL	R10	G8
      0x882C0B18,  //  002E  GETMBR	R11	R5	K24
      0x7C280200,  //  002F  CALL	R10	1
      0x002A2E0A,  //  0030  ADD	R10	K23	R10
      0x582C0010,  //  0031  LDCONST	R11	K16
      0x7C200600,  //  0032  CALL	R8	3
      0xB8221A00,  //  0033  GETNGBL	R8	K13
      0x8C20110E,  //  0034  GETMET	R8	R8	K14
      0x60280008,  //  0035  GETGBL	R10	G8
      0x882C0B1A,  //  0036  GETMBR	R11	R5	K26
      0x7C280200,  //  0037  CALL	R10	1
      0x002A320A,  //  0038  ADD	R10	K25	R10
      0x582C0010,  //  0039  LDCONST	R11	K16
      0x7C200600,  //  003A  CALL	R8	3
      0xB8221A00,  //  003B  GETNGBL	R8	K13
      0x8C20110E,  //  003C  GETMET	R8	R8	K14
      0x8C280B1C,  //  003D  GETMET	R10	R5	K28
      0x7C280200,  //  003E  CALL	R10	1
      0x8C28151D,  //  003F  GETMET	R10	R10	K29
      0x7C280200,  //  0040  CALL	R10	1
      0x002A360A,  //  0041  ADD	R10	K27	R10
      0x582C0010,  //  0042  LDCONST	R11	K16
      0x7C200600,  //  0043  CALL	R8	3
      0xB8221A00,  //  0044  GETNGBL	R8	K13
      0x8C20110E,  //  0045  GETMET	R8	R8	K14
      0x8C280F1D,  //  0046  GETMET	R10	R7	K29
      0x7C280200,  //  0047  CALL	R10	1
      0x002A3C0A,  //  0048  ADD	R10	K30	R10
      0x582C0010,  //  0049  LDCONST	R11	K16
      0x7C200600,  //  004A  CALL	R8	3
      0x60200015,  //  004B  GETGBL	R8	G21
      0x7C200000,  //  004C  CALL	R8	0
      0x8C20111F,  //  004D  GETMET	R8	R8	K31
      0x88280120,  //  004E  GETMBR	R10	R0	K32
      0x7C200400,  //  004F  CALL	R8	2
      0x8C240921,  //  0050  GETMET	R9	R4	K33
      0x7C240200,  //  0051  CALL	R9	1
      0x8C241322,  //  0052  GETMET	R9	R9	K34
      0x882C0B23,  //  0053  GETMBR	R11	R5	K35
      0x8C300B1C,  //  0054  GETMET	R12	R5	K28
      0x7C300200,  //  0055  CALL	R12	1
      0x00301807,  //  0056  ADD	R12	R12	R7
      0x5C341000,  //  0057  MOVE	R13	R8
      0x543A000F,  //  0058  LDINT	R14	16
      0x7C240A00,  //  0059  CALL	R9	5
      0xB82A1A00,  //  005A  GETNGBL	R10	K13
      0x8C28150E,  //  005B  GETMET	R10	R10	K14
      0x5830000F,  //  005C  LDCONST	R12	K15
      0x58340010,  //  005D  LDCONST	R13	K16
      0x7C280600,  //  005E  CALL	R10	3
      0xB82A1A00,  //  005F  GETNGBL	R10	K13
      0x8C28150E,  //  0060  GETMET	R10	R10	K14
      0x8C300B1C,  //  0061  GETMET	R12	R5	K28
      0x7C300200,  //  0062  CALL	R12	1
      0x00301807,  //  0063  ADD	R12	R12	R7
      0x8C30191D,  //  0064  GETMET	R12	R12	K29
      0x7C300200,  //  0065  CALL	R12	1
      0x0032480C,  //  0066  ADD	R12	K36	R12
      0x58340010,  //  0067  LDCONST	R13	K16
      0x7C280600,  //  0068  CALL	R10	3
      0xB82A1A00,  //  0069  GETNGBL	R10	K13
      0x8C28150E,  //  006A  GETMET	R10	R10	K14
      0x8C30131D,  //  006B  GETMET	R12	R9	K29
      0x7C300200,  //  006C  CALL	R12	1
      0x00324A0C,  //  006D  ADD	R12	K37	R12
      0x58340010,  //  006E  LDCONST	R13	K16
      0x7C280600,  //  006F  CALL	R10	3
      0xB82A1A00,  //  0070  GETNGBL	R10	K13
      0x8C28150E,  //  0071  GETMET	R10	R10	K14
      0x5830000F,  //  0072  LDCONST	R12	K15
      0x58340010,  //  0073  LDCONST	R13	K16
      0x7C280600,  //  0074  CALL	R10	3
      0x5429FFEE,  //  0075  LDINT	R10	-17
      0x402A060A,  //  0076  CONNECT	R10	K3	R10
      0x882C0D26,  //  0077  GETMBR	R11	R6	K38
      0x9428160A,  //  0078  GETIDX	R10	R11	R10
      0x5431FFEF,  //  0079  LDINT	R12	-16
      0x40301927,  //  007A  CONNECT	R12	R12	K39
      0x88340D26,  //  007B  GETMBR	R13	R6	K38
      0x942C1A0C,  //  007C  GETIDX	R11	R13	R12
      0x8C380928,  //  007D  GETMET	R14	R4	K40
      0x5C401200,  //  007E  MOVE	R16	R9
      0x60440015,  //  007F  GETGBL	R17	G21
      0x7C440000,  //  0080  CALL	R17	0
      0x8C44231F,  //  0081  GETMET	R17	R17	K31
      0x884C0129,  //  0082  GETMBR	R19	R0	K41
      0x7C440400,  //  0083  CALL	R17	2
      0x60480015,  //  0084  GETGBL	R18	G21
      0x7C480000,  //  0085  CALL	R18	0
      0x604C000C,  //  0086  GETGBL	R19	G12
      0x5C501400,  //  0087  MOVE	R20	R10
      0x7C4C0200,  //  0088  CALL	R19	1
      0x5452000F,  //  0089  LDINT	R20	16
      0x7C380C00,  //  008A  CALL	R14	6
      0x5C301C00,  //  008B  MOVE	R12	R14
      0x8C38192A,  //  008C  GETMET	R14	R12	K42
      0x5C401400,  //  008D  MOVE	R16	R10
      0x7C380400,  //  008E  CALL	R14	2
      0x5C341C00,  //  008F  MOVE	R13	R14
      0x8C38192B,  //  0090  GETMET	R14	R12	K43
      0x7C380200,  //  0091  CALL	R14	1
      0xB83E1A00,  //  0092  GETNGBL	R15	K13
      0x8C3C1F0E,  //  0093  GETMET	R15	R15	K14
      0x8C441B1D,  //  0094  GETMET	R17	R13	K29
      0x7C440200,  //  0095  CALL	R17	1
      0x00465811,  //  0096  ADD	R17	K44	R17
      0x58480010,  //  0097  LDCONST	R18	K16
      0x7C3C0600,  //  0098  CALL	R15	3
      0xB83E1A00,  //  0099  GETNGBL	R15	K13
      0x8C3C1F0E,  //  009A  GETMET	R15	R15	K14
      0x8C441D1D,  //  009B  GETMET	R17	R14	K29
      0x7C440200,  //  009C  CALL	R17	1
      0x00465A11,  //  009D  ADD	R17	K45	R17
      0x58480010,  //  009E  LDCONST	R18	K16
      0x7C3C0600,  //  009F  CALL	R15	3
      0xB83E1A00,  //  00A0  GETNGBL	R15	K13
      0x8C3C1F0E,  //  00A1  GETMET	R15	R15	K14
      0x8C44171D,  //  00A2  GETMET	R17	R11	K29
      0x7C440200,  //  00A3  CALL	R17	1
      0x00465C11,  //  00A4  ADD	R17	K46	R17
      0x58480010,  //  00A5  LDCONST	R18	K16
      0x7C3C0600,  //  00A6  CALL	R15	3
      0xB83E1A00,  //  00A7  GETNGBL	R15	K13
      0x8C3C1F0E,  //  00A8  GETMET	R15	R15	K14
      0x5844000F,  //  00A9  LDCONST	R17	K15
      0x58480010,  //  00AA  LDCONST	R18	K16
      0x7C3C0600,  //  00AB  CALL	R15	3
      0x203C1C0B,  //  00AC  NE	R15	R14	R11
      0x783E0000,  //  00AD  JMPF	R15	#00AF
      0xB0065F30,  //  00AE  RAISE	1	K47	K48
      0xB83E1000,  //  00AF  GETNGBL	R15	K8
      0x883C1F31,  //  00B0  GETMBR	R15	R15	K49
      0x8C3C1F0A,  //  00B1  GETMET	R15	R15	K10
      0x5C441A00,  //  00B2  MOVE	R17	R13
      0x7C3C0400,  //  00B3  CALL	R15	2
      0x8C401F32,  //  00B4  GETMET	R16	R15	K50
      0x58480033,  //  00B5  LDCONST	R18	K51
      0x7C400400,  //  00B6  CALL	R16	2
      0x8C441F32,  //  00B7  GETMET	R17	R15	K50
      0x584C0034,  //  00B8  LDCONST	R19	K52
      0x7C440400,  //  00B9  CALL	R17	2
      0x8C481F32,  //  00BA  GETMET	R18	R15	K50
      0x58500010,  //  00BB  LDCONST	R20	K16
      0x7C480400,  //  00BC  CALL	R18	2
      0xB84E1000,  //  00BD  GETNGBL	R19	K8
      0x884C2731,  //  00BE  GETMBR	R19	R19	K49
      0x8C4C270A,  //  00BF  GETMET	R19	R19	K10
      0x5C542000,  //  00C0  MOVE	R21	R16
      0x7C4C0400,  //  00C1  CALL	R19	2
      0xB8521A00,  //  00C2  GETNGBL	R20	K13
      0x8C50290E,  //  00C3  GETMET	R20	R20	K14
      0x60580008,  //  00C4  GETGBL	R22	G8
      0x5C5C2600,  //  00C5  MOVE	R23	R19
      0x7C580200,  //  00C6  CALL	R22	1
      0x005A6A16,  //  00C7  ADD	R22	K53	R22
      0x585C0010,  //  00C8  LDCONST	R23	K16
      0x7C500600,  //  00C9  CALL	R20	3
      0x8C502732,  //  00CA  GETMET	R20	R19	K50
      0x545A0008,  //  00CB  LDINT	R22	9
      0x7C500400,  //  00CC  CALL	R20	2
      0x8C542736,  //  00CD  GETMET	R21	R19	K54
      0x545E0005,  //  00CE  LDINT	R23	6
      0x7C540400,  //  00CF  CALL	R21	2
      0x8C582B32,  //  00D0  GETMET	R22	R21	K50
      0x54620010,  //  00D1  LDINT	R24	17
      0x7C580400,  //  00D2  CALL	R22	2
      0x605C0004,  //  00D3  GETGBL	R23	G4
      0x5C602C00,  //  00D4  MOVE	R24	R22
      0x7C5C0200,  //  00D5  CALL	R23	1
      0x1C5C2F37,  //  00D6  EQ	R23	R23	K55
      0x785E0003,  //  00D7  JMPF	R23	#00DC
      0xB85E7000,  //  00D8  GETNGBL	R23	K56
      0x5C602C00,  //  00D9  MOVE	R24	R22
      0x7C5C0200,  //  00DA  CALL	R23	1
      0x5C582E00,  //  00DB  MOVE	R22	R23
      0x8C5C2D3A,  //  00DC  GETMET	R23	R22	K58
      0x7C5C0200,  //  00DD  CALL	R23	1
      0x90167217,  //  00DE  SETMBR	R5	K57	R23
      0xB85E1A00,  //  00DF  GETNGBL	R23	K13
      0x8C5C2F0E,  //  00E0  GETMET	R23	R23	K14
      0x60640008,  //  00E1  GETGBL	R25	G8
      0x88680B39,  //  00E2  GETMBR	R26	R5	K57
      0x7C640200,  //  00E3  CALL	R25	1
      0x00667619,  //  00E4  ADD	R25	K59	R25
      0x58680010,  //  00E5  LDCONST	R26	K16
      0x7C5C0600,  //  00E6  CALL	R23	3
      0xB85E1000,  //  00E7  GETNGBL	R23	K8
      0x885C2F31,  //  00E8  GETMBR	R23	R23	K49
      0x8C5C2F3C,  //  00E9  GETMET	R23	R23	K60
      0x7C5C0200,  //  00EA  CALL	R23	1
      0x8C602F3D,  //  00EB  GETMET	R24	R23	K61
      0x58680033,  //  00EC  LDCONST	R26	K51
      0xB86E1000,  //  00ED  GETNGBL	R27	K8
      0x886C3731,  //  00EE  GETMBR	R27	R27	K49
      0x886C373E,  //  00EF  GETMBR	R27	R27	K62
      0x5C702000,  //  00F0  MOVE	R28	R16
      0x7C600800,  //  00F1  CALL	R24	4
      0x8C602F3D,  //  00F2  GETMET	R24	R23	K61
      0x58680034,  //  00F3  LDCONST	R26	K52
      0xB86E1000,  //  00F4  GETNGBL	R27	K8
      0x886C3731,  //  00F5  GETMBR	R27	R27	K49
      0x886C373E,  //  00F6  GETMBR	R27	R27	K62
      0x5C702200,  //  00F7  MOVE	R28	R17
      0x7C600800,  //  00F8  CALL	R24	4
      0x8C602F3D,  //  00F9  GETMET	R24	R23	K61
      0x58680010,  //  00FA  LDCONST	R26	K16
      0xB86E1000,  //  00FB  GETNGBL	R27	K8
      0x886C3731,  //  00FC  GETMBR	R27	R27	K49
      0x886C373E,  //  00FD  GETMBR	R27	R27	K62
      0x8870013F,  //  00FE  GETMBR	R28	R0	K63
      0x7C600800,  //  00FF  CALL	R24	4
      0x8C602F3D,  //  0100  GETMET	R24	R23	K61
      0x546A0003,  //  0101  LDINT	R26	4
      0xB86E1000,  //  0102  GETNGBL	R27	K8
      0x886C3731,  //  0103  GETMBR	R27	R27	K49
      0x886C373E,  //  0104  GETMBR	R27	R27	K62
      0x88700140,  //  0105  GETMBR	R28	R0	K64
      0x7C600800,  //  0106  CALL	R24	4
      0x8C602F41,  //  0107  GETMET	R24	R23	K65
      0x7C600200,  //  0108  CALL	R24	1
      0xB8661A00,  //  0109  GETNGBL	R25	K13
      0x8C64330E,  //  010A  GETMET	R25	R25	K14
      0x8C6C291D,  //  010B  GETMET	R27	R20	K29
      0x7C6C0200,  //  010C  CALL	R27	1
      0x006E841B,  //  010D  ADD	R27	K66	R27
      0x58700010,  //  010E  LDCONST	R28	K16
      0x7C640600,  //  010F  CALL	R25	3
      0xB8661A00,  //  0110  GETNGBL	R25	K13
      0x8C64330E,  //  0111  GETMET	R25	R25	K14
      0x8C6C251D,  //  0112  GETMET	R27	R18	K29
      0x7C6C0200,  //  0113  CALL	R27	1
      0x006E861B,  //  0114  ADD	R27	K67	R27
      0x58700010,  //  0115  LDCONST	R28	K16
      0x7C640600,  //  0116  CALL	R25	3
      0xB8661A00,  //  0117  GETNGBL	R25	K13
      0x8C64330E,  //  0118  GETMET	R25	R25	K14
      0x586C000F,  //  0119  LDCONST	R27	K15
      0x58700010,  //  011A  LDCONST	R28	K16
      0x7C640600,  //  011B  CALL	R25	3
      0x8C640944,  //  011C  GETMET	R25	R4	K68
      0x7C640200,  //  011D  CALL	R25	1
      0x8C643345,  //  011E  GETMET	R25	R25	K69
      0x5C6C2800,  //  011F  MOVE	R27	R20
      0x5C703000,  //  0120  MOVE	R28	R24
      0x5C742400,  //  0121  MOVE	R29	R18
      0x7C640800,  //  0122  CALL	R25	4
      0x5C683200,  //  0123  MOVE	R26	R25
      0x746A0000,  //  0124  JMPT	R26	#0126
      0xB0065F46,  //  0125  RAISE	1	K47	K70
      0xB86A1A00,  //  0126  GETNGBL	R26	K13
      0x8C68350E,  //  0127  GETMET	R26	R26	K14
      0x58700047,  //  0128  LDCONST	R28	K71
      0x58740010,  //  0129  LDCONST	R29	K16
      0x7C680600,  //  012A  CALL	R26	3
      0x8C680911,  //  012B  GETMET	R26	R4	K17
      0x7C680200,  //  012C  CALL	R26	1
      0x8C683512,  //  012D  GETMET	R26	R26	K18
      0x88700B13,  //  012E  GETMBR	R28	R5	K19
      0x7C680400,  //  012F  CALL	R26	2
      0x8C683512,  //  0130  GETMET	R26	R26	K18
      0x88700B14,  //  0131  GETMBR	R28	R5	K20
      0x7C680400,  //  0132  CALL	R26	2
      0x8C683512,  //  0133  GETMET	R26	R26	K18
      0x88700D48,  //  0134  GETMBR	R28	R6	K72
      0x7C680400,  //  0135  CALL	R26	2
      0x8C683515,  //  0136  GETMET	R26	R26	K21
      0x7C680200,  //  0137  CALL	R26	1
      0x5C1C3400,  //  0138  MOVE	R7	R26
      0x4C680000,  //  0139  LDNIL	R26
      0x9016261A,  //  013A  SETMBR	R5	K19	R26
      0x4C680000,  //  013B  LDNIL	R26
      0x9016281A,  //  013C  SETMBR	R5	K20	R26
      0xB86A1A00,  //  013D  GETNGBL	R26	K13
      0x8C68350E,  //  013E  GETMET	R26	R26	K14
      0x58700049,  //  013F  LDCONST	R28	K73
      0x58740010,  //  0140  LDCONST	R29	K16
      0x7C680600,  //  0141  CALL	R26	3
      0xB86A1A00,  //  0142  GETNGBL	R26	K13
      0x8C68350E,  //  0143  GETMET	R26	R26	K14
      0x88700B23,  //  0144  GETMBR	R28	R5	K35
      0x8C70391D,  //  0145  GETMET	R28	R28	K29
      0x7C700200,  //  0146  CALL	R28	1
      0x0072941C,  //  0147  ADD	R28	K74	R28
      0x58740010,  //  0148  LDCONST	R29	K16
      0x7C680600,  //  0149  CALL	R26	3
      0xB86A1A00,  //  014A  GETNGBL	R26	K13
      0x8C68350E,  //  014B  GETMET	R26	R26	K14
      0x8C700B1C,  //  014C  GETMET	R28	R5	K28
      0x7C700200,  //  014D  CALL	R28	1
      0x00703807,  //  014E  ADD	R28	R28	R7
      0x8C70391D,  //  014F  GETMET	R28	R28	K29
      0x7C700200,  //  0150  CALL	R28	1
      0x0072961C,  //  0151  ADD	R28	K75	R28
      0x58740010,  //  0152  LDCONST	R29	K16
      0x7C680600,  //  0153  CALL	R26	3
      0x8C680921,  //  0154  GETMET	R26	R4	K33
      0x7C680200,  //  0155  CALL	R26	1
      0x8C683522,  //  0156  GETMET	R26	R26	K34
      0x88700B23,  //  0157  GETMBR	R28	R5	K35
      0x8C740B1C,  //  0158  GETMET	R29	R5	K28
      0x7C740200,  //  0159  CALL	R29	1
      0x00743A07,  //  015A  ADD	R29	R29	R7
      0x60780015,  //  015B  GETGBL	R30	G21
      0x7C780000,  //  015C  CALL	R30	0
      0x8C783D1F,  //  015D  GETMET	R30	R30	K31
      0x8880014C,  //  015E  GETMBR	R32	R0	K76
      0x7C780400,  //  015F  CALL	R30	2
      0x547E002F,  //  0160  LDINT	R31	48
      0x7C680A00,  //  0161  CALL	R26	5
      0x546E000E,  //  0162  LDINT	R27	15
      0x406E061B,  //  0163  CONNECT	R27	K3	R27
      0x946C341B,  //  0164  GETIDX	R27	R26	R27
      0x5472000F,  //  0165  LDINT	R28	16
      0x5476001E,  //  0166  LDINT	R29	31
      0x4070381D,  //  0167  CONNECT	R28	R28	R29
      0x9470341C,  //  0168  GETIDX	R28	R26	R28
      0x5476001F,  //  0169  LDINT	R29	32
      0x547A002E,  //  016A  LDINT	R30	47
      0x40743A1E,  //  016B  CONNECT	R29	R29	R30
      0x9474341D,  //  016C  GETIDX	R29	R26	R29
      0xB87A1A00,  //  016D  GETNGBL	R30	K13
      0x8C783D4D,  //  016E  GETMET	R30	R30	K77
      0x7C780200,  //  016F  CALL	R30	1
      0x94783D4E,  //  0170  GETIDX	R30	R30	K78
      0xB87E1A00,  //  0171  GETNGBL	R31	K13
      0x8C7C3F0E,  //  0172  GETMET	R31	R31	K14
      0x58840049,  //  0173  LDCONST	R33	K73
      0x58880010,  //  0174  LDCONST	R34	K16
      0x7C7C0600,  //  0175  CALL	R31	3
      0xB87E1A00,  //  0176  GETNGBL	R31	K13
      0x8C7C3F0E,  //  0177  GETMET	R31	R31	K14
      0x8C84371D,  //  0178  GETMET	R33	R27	K29
      0x7C840200,  //  0179  CALL	R33	1
      0x00869E21,  //  017A  ADD	R33	K79	R33
      0x58880010,  //  017B  LDCONST	R34	K16
      0x7C7C0600,  //  017C  CALL	R31	3
      0xB87E1A00,  //  017D  GETNGBL	R31	K13
      0x8C7C3F0E,  //  017E  GETMET	R31	R31	K14
      0x8C84391D,  //  017F  GETMET	R33	R28	K29
      0x7C840200,  //  0180  CALL	R33	1
      0x0086A021,  //  0181  ADD	R33	K80	R33
      0x58880010,  //  0182  LDCONST	R34	K16
      0x7C7C0600,  //  0183  CALL	R31	3
      0xB87E1A00,  //  0184  GETNGBL	R31	K13
      0x8C7C3F0E,  //  0185  GETMET	R31	R31	K14
      0x8C843B1D,  //  0186  GETMET	R33	R29	K29
      0x7C840200,  //  0187  CALL	R33	1
      0x0086A221,  //  0188  ADD	R33	K81	R33
      0x58880010,  //  0189  LDCONST	R34	K16
      0x7C7C0600,  //  018A  CALL	R31	3
      0xB87E1A00,  //  018B  GETNGBL	R31	K13
      0x8C7C3F0E,  //  018C  GETMET	R31	R31	K14
      0x58840049,  //  018D  LDCONST	R33	K73
      0x58880010,  //  018E  LDCONST	R34	K16
      0x7C7C0600,  //  018F  CALL	R31	3
      0x8C7C0352,  //  0190  GETMET	R31	R1	K82
      0x5486003F,  //  0191  LDINT	R33	64
      0x50880200,  //  0192  LDBOOL	R34	1	0
      0x7C7C0600,  //  0193  CALL	R31	3
      0x60800015,  //  0194  GETGBL	R32	G21
      0x7C800000,  //  0195  CALL	R32	0
      0x8C844153,  //  0196  GETMET	R33	R32	K83
      0x588C0003,  //  0197  LDCONST	R35	K3
      0x58900034,  //  0198  LDCONST	R36	K52
      0x7C840600,  //  0199  CALL	R33	3
      0x8C844153,  //  019A  GETMET	R33	R32	K83
      0x588C0003,  //  019B  LDCONST	R35	K3
      0x54920003,  //  019C  LDINT	R36	4
      0x7C840600,  //  019D  CALL	R33	3
      0x8C844153,  //  019E  GETMET	R33	R32	K83
      0x588C0003,  //  019F  LDCONST	R35	K3
      0x54920003,  //  01A0  LDINT	R36	4
      0x7C840600,  //  01A1  CALL	R33	3
      0x8C843F41,  //  01A2  GETMET	R33	R31	K65
      0x5C8C4000,  //  01A3  MOVE	R35	R32
      0x7C840400,  //  01A4  CALL	R33	2
      0x88880154,  //  01A5  GETMBR	R34	R0	K84
      0x8C884555,  //  01A6  GETMET	R34	R34	K85
      0x5C904200,  //  01A7  MOVE	R36	R33
      0x5C940400,  //  01A8  MOVE	R37	R2
      0x5C980600,  //  01A9  MOVE	R38	R3
      0x889C3F56,  //  01AA  GETMBR	R39	R31	K86
      0x7C880A00,  //  01AB  CALL	R34	5
      0x8C880B57,  //  01AC  GETMET	R34	R5	K87
      0x7C880200,  //  01AD  CALL	R34	1
      0x8C880B58,  //  01AE  GETMET	R34	R5	K88
      0x5C903600,  //  01AF  MOVE	R36	R27
      0x5C943800,  //  01B0  MOVE	R37	R28
      0x5C983A00,  //  01B1  MOVE	R38	R29
      0x5C9C3C00,  //  01B2  MOVE	R39	R30
      0x7C880A00,  //  01B3  CALL	R34	5
      0x8C880B59,  //  01B4  GETMET	R34	R5	K89
      0x50900200,  //  01B5  LDBOOL	R36	1	0
      0x7C880400,  //  01B6  CALL	R34	2
      0x8C880B5A,  //  01B7  GETMET	R34	R5	K90
      0x7C880200,  //  01B8  CALL	R34	1
      0x8C880B5B,  //  01B9  GETMET	R34	R5	K91
      0x7C880200,  //  01BA  CALL	R34	1
      0x50880200,  //  01BB  LDBOOL	R34	1	0
      0x80044400,  //  01BC  RET	1	R34
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
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[116]) {     /* constants */
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
    /* K14  */  be_nested_str_weak(find_session_by_destination_id),
    /* K15  */  be_nested_str_weak(destinationId),
    /* K16  */  be_nested_str_weak(initiatorRandom),
    /* K17  */  be_nested_str_weak(valuer_error),
    /* K18  */  be_nested_str_weak(StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K19  */  be_nested_str_weak(source_node_id),
    /* K20  */  be_nested_str_weak(set_mode),
    /* K21  */  be_nested_str_weak(Session),
    /* K22  */  be_nested_str_weak(__CASE),
    /* K23  */  be_nested_str_weak(session),
    /* K24  */  be_nested_str_weak(device),
    /* K25  */  be_nested_str_weak(sessions),
    /* K26  */  be_nested_str_weak(remove_session),
    /* K27  */  be_nested_str_weak(_future_initiator_session_id),
    /* K28  */  be_nested_str_weak(initiator_session_id),
    /* K29  */  be_nested_str_weak(_future_local_session_id),
    /* K30  */  be_nested_str_weak(gen_local_session_id),
    /* K31  */  be_nested_str_weak(future_local_session_id),
    /* K32  */  be_nested_str_weak(resumptionID),
    /* K33  */  be_nested_str_weak(initiatorResumeMIC),
    /* K34  */  be_nested_str_weak(shared_secret),
    /* K35  */  be_nested_str_weak(fromstring),
    /* K36  */  be_nested_str_weak(Sigma1_Resume),
    /* K37  */  be_nested_str_weak(HKDF_SHA256),
    /* K38  */  be_nested_str_weak(derive),
    /* K39  */  be_nested_str_weak(NCASE_SigmaR1),
    /* K40  */  be_const_int(2147483647),
    /* K41  */  be_nested_str_weak(AES_CCM),
    /* K42  */  be_nested_str_weak(decrypt),
    /* K43  */  be_nested_str_weak(tag),
    /* K44  */  be_nested_str_weak(tasmota),
    /* K45  */  be_nested_str_weak(log),
    /* K46  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K47  */  be_const_int(3),
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
    /* K73  */  be_nested_str_weak(message_counter),
    /* K74  */  be_nested_str_weak(close),
    /* K75  */  be_nested_str_weak(set_keys),
    /* K76  */  be_nested_str_weak(set_persist),
    /* K77  */  be_nested_str_weak(set_no_expiration),
    /* K78  */  be_nested_str_weak(save),
    /* K79  */  be_nested_str_weak(ResponderEph_priv),
    /* K80  */  be_nested_str_weak(ResponderEph_pub),
    /* K81  */  be_nested_str_weak(EC_P256),
    /* K82  */  be_nested_str_weak(public_key),
    /* K83  */  be_nested_str_weak(shared_key),
    /* K84  */  be_nested_str_weak(TLV),
    /* K85  */  be_nested_str_weak(Matter_TLV_struct),
    /* K86  */  be_nested_str_weak(add_TLV),
    /* K87  */  be_const_int(1),
    /* K88  */  be_nested_str_weak(B2),
    /* K89  */  be_nested_str_weak(get_noc),
    /* K90  */  be_const_int(2),
    /* K91  */  be_nested_str_weak(get_icac),
    /* K92  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K93  */  be_nested_str_weak(get_pk),
    /* K94  */  be_nested_str_weak(Msg1),
    /* K95  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20MSG1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K96  */  be_nested_str_weak(SHA256),
    /* K97  */  be_nested_str_weak(update),
    /* K98  */  be_nested_str_weak(out),
    /* K99  */  be_nested_str_weak(S2K_Info),
    /* K100 */  be_nested_str_weak(get_ipk_group_key),
    /* K101 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20SharedSecret_X20_X20_X3D_X20),
    /* K102 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K103 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K104 */  be_nested_str_weak(TBEData2_Nonce),
    /* K105 */  be_nested_str_weak(encrypt),
    /* K106 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Enc_X20_X20_X20_X3D_X20),
    /* K107 */  be_nested_str_weak(Sigma2),
    /* K108 */  be_nested_str_weak(responderRandom),
    /* K109 */  be_nested_str_weak(responderSessionId),
    /* K110 */  be_nested_str_weak(responderEphPubKey),
    /* K111 */  be_nested_str_weak(encrypted2),
    /* K112 */  be_nested_str_weak(MTR_X3A_X20sigma2_X3A_X20),
    /* K113 */  be_nested_str_weak(inspect),
    /* K114 */  be_nested_str_weak(_Msg2),
    /* K115 */  be_nested_str_weak(MTR_X3A_X20sigma2_raw_X3A_X20),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[551]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x88140301,  //  0001  GETMBR	R5	R1	K1
      0x541A002F,  //  0002  LDINT	R6	48
      0x20140A06,  //  0003  NE	R5	R5	R6
      0x74160005,  //  0004  JMPT	R5	#000B
      0x88140302,  //  0005  GETMBR	R5	R1	K2
      0x20140B03,  //  0006  NE	R5	R5	K3
      0x74160002,  //  0007  JMPT	R5	#000B
      0x88140304,  //  0008  GETMBR	R5	R1	K4
      0x20140B03,  //  0009  NE	R5	R5	K3
      0x78160000,  //  000A  JMPF	R5	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0xB8160E00,  //  000C  GETNGBL	R5	K7
      0x8C140B08,  //  000D  GETMET	R5	R5	K8
      0x7C140200,  //  000E  CALL	R5	1
      0x8C140B09,  //  000F  GETMET	R5	R5	K9
      0x881C030A,  //  0010  GETMBR	R7	R1	K10
      0x8820030B,  //  0011  GETMBR	R8	R1	K11
      0x7C140600,  //  0012  CALL	R5	3
      0x88180B0D,  //  0013  GETMBR	R6	R5	K13
      0x90021806,  //  0014  SETMBR	R0	K12	R6
      0x8C18010E,  //  0015  GETMET	R6	R0	K14
      0x88200B0F,  //  0016  GETMBR	R8	R5	K15
      0x88240B10,  //  0017  GETMBR	R9	R5	K16
      0x7C180600,  //  0018  CALL	R6	3
      0x4C1C0000,  //  0019  LDNIL	R7
      0x1C1C0C07,  //  001A  EQ	R7	R6	R7
      0x781E0000,  //  001B  JMPF	R7	#001D
      0xB0062312,  //  001C  RAISE	1	K17	K18
      0x881C0313,  //  001D  GETMBR	R7	R1	K19
      0x901A2607,  //  001E  SETMBR	R6	K19	R7
      0x8C1C0D14,  //  001F  GETMET	R7	R6	K20
      0xB8260E00,  //  0020  GETNGBL	R9	K7
      0x88241315,  //  0021  GETMBR	R9	R9	K21
      0x88241316,  //  0022  GETMBR	R9	R9	K22
      0x7C1C0400,  //  0023  CALL	R7	2
      0x881C0317,  //  0024  GETMBR	R7	R1	K23
      0x781E0004,  //  0025  JMPF	R7	#002B
      0x881C0118,  //  0026  GETMBR	R7	R0	K24
      0x881C0F19,  //  0027  GETMBR	R7	R7	K25
      0x8C1C0F1A,  //  0028  GETMET	R7	R7	K26
      0x88240317,  //  0029  GETMBR	R9	R1	K23
      0x7C1C0400,  //  002A  CALL	R7	2
      0x90062E06,  //  002B  SETMBR	R1	K23	R6
      0x881C0B1C,  //  002C  GETMBR	R7	R5	K28
      0x901A3607,  //  002D  SETMBR	R6	K27	R7
      0x881C0118,  //  002E  GETMBR	R7	R0	K24
      0x881C0F19,  //  002F  GETMBR	R7	R7	K25
      0x8C1C0F1E,  //  0030  GETMET	R7	R7	K30
      0x7C1C0200,  //  0031  CALL	R7	1
      0x901A3A07,  //  0032  SETMBR	R6	K29	R7
      0x881C0D1D,  //  0033  GETMBR	R7	R6	K29
      0x90023E07,  //  0034  SETMBR	R0	K31	R7
      0x881C0B20,  //  0035  GETMBR	R7	R5	K32
      0x4C200000,  //  0036  LDNIL	R8
      0x201C0E08,  //  0037  NE	R7	R7	R8
      0x781E00F2,  //  0038  JMPF	R7	#012C
      0x881C0B21,  //  0039  GETMBR	R7	R5	K33
      0x4C200000,  //  003A  LDNIL	R8
      0x201C0E08,  //  003B  NE	R7	R7	R8
      0x781E00EE,  //  003C  JMPF	R7	#012C
      0x881C0D22,  //  003D  GETMBR	R7	R6	K34
      0x4C200000,  //  003E  LDNIL	R8
      0x201C0E08,  //  003F  NE	R7	R7	R8
      0x781E00EA,  //  0040  JMPF	R7	#012C
      0x881C0B10,  //  0041  GETMBR	R7	R5	K16
      0x88200B20,  //  0042  GETMBR	R8	R5	K32
      0x001C0E08,  //  0043  ADD	R7	R7	R8
      0x60200015,  //  0044  GETGBL	R8	G21
      0x7C200000,  //  0045  CALL	R8	0
      0x8C201123,  //  0046  GETMET	R8	R8	K35
      0x58280024,  //  0047  LDCONST	R10	K36
      0x7C200400,  //  0048  CALL	R8	2
      0x8C240925,  //  0049  GETMET	R9	R4	K37
      0x7C240200,  //  004A  CALL	R9	1
      0x8C241326,  //  004B  GETMET	R9	R9	K38
      0x882C0D22,  //  004C  GETMBR	R11	R6	K34
      0x5C300E00,  //  004D  MOVE	R12	R7
      0x5C341000,  //  004E  MOVE	R13	R8
      0x543A000F,  //  004F  LDINT	R14	16
      0x7C240A00,  //  0050  CALL	R9	5
      0x60280015,  //  0051  GETGBL	R10	G21
      0x7C280000,  //  0052  CALL	R10	0
      0x8C281523,  //  0053  GETMET	R10	R10	K35
      0x58300027,  //  0054  LDCONST	R12	K39
      0x7C280400,  //  0055  CALL	R10	2
      0x542DFFEE,  //  0056  LDINT	R11	-17
      0x402E060B,  //  0057  CONNECT	R11	K3	R11
      0x88300B21,  //  0058  GETMBR	R12	R5	K33
      0x942C180B,  //  0059  GETIDX	R11	R12	R11
      0x5435FFEF,  //  005A  LDINT	R13	-16
      0x40341B28,  //  005B  CONNECT	R13	R13	K40
      0x88380B21,  //  005C  GETMBR	R14	R5	K33
      0x94301C0D,  //  005D  GETIDX	R12	R14	R13
      0x8C3C0929,  //  005E  GETMET	R15	R4	K41
      0x5C441200,  //  005F  MOVE	R17	R9
      0x5C481400,  //  0060  MOVE	R18	R10
      0x604C0015,  //  0061  GETGBL	R19	G21
      0x7C4C0000,  //  0062  CALL	R19	0
      0x6050000C,  //  0063  GETGBL	R20	G12
      0x5C541600,  //  0064  MOVE	R21	R11
      0x7C500200,  //  0065  CALL	R20	1
      0x5456000F,  //  0066  LDINT	R21	16
      0x7C3C0C00,  //  0067  CALL	R15	6
      0x5C341E00,  //  0068  MOVE	R13	R15
      0x8C3C1B2A,  //  0069  GETMET	R15	R13	K42
      0x5C441600,  //  006A  MOVE	R17	R11
      0x7C3C0400,  //  006B  CALL	R15	2
      0x5C381E00,  //  006C  MOVE	R14	R15
      0x8C3C1B2B,  //  006D  GETMET	R15	R13	K43
      0x7C3C0200,  //  006E  CALL	R15	1
      0xB8425800,  //  006F  GETNGBL	R16	K44
      0x8C40212D,  //  0070  GETMET	R16	R16	K45
      0x5848002E,  //  0071  LDCONST	R18	K46
      0x584C002F,  //  0072  LDCONST	R19	K47
      0x7C400600,  //  0073  CALL	R16	3
      0xB8425800,  //  0074  GETNGBL	R16	K44
      0x8C40212D,  //  0075  GETMET	R16	R16	K45
      0x8C481331,  //  0076  GETMET	R18	R9	K49
      0x7C480200,  //  0077  CALL	R18	1
      0x004A6012,  //  0078  ADD	R18	K48	R18
      0x584C002F,  //  0079  LDCONST	R19	K47
      0x7C400600,  //  007A  CALL	R16	3
      0xB8425800,  //  007B  GETNGBL	R16	K44
      0x8C40212D,  //  007C  GETMET	R16	R16	K45
      0x8C481931,  //  007D  GETMET	R18	R12	K49
      0x7C480200,  //  007E  CALL	R18	1
      0x004A6412,  //  007F  ADD	R18	K50	R18
      0x584C002F,  //  0080  LDCONST	R19	K47
      0x7C400600,  //  0081  CALL	R16	3
      0xB8425800,  //  0082  GETNGBL	R16	K44
      0x8C40212D,  //  0083  GETMET	R16	R16	K45
      0x8C481D31,  //  0084  GETMET	R18	R14	K49
      0x7C480200,  //  0085  CALL	R18	1
      0x004A6612,  //  0086  ADD	R18	K51	R18
      0x584C002F,  //  0087  LDCONST	R19	K47
      0x7C400600,  //  0088  CALL	R16	3
      0xB8425800,  //  0089  GETNGBL	R16	K44
      0x8C40212D,  //  008A  GETMET	R16	R16	K45
      0x8C481F31,  //  008B  GETMET	R18	R15	K49
      0x7C480200,  //  008C  CALL	R18	1
      0x004A6812,  //  008D  ADD	R18	K52	R18
      0x584C002F,  //  008E  LDCONST	R19	K47
      0x7C400600,  //  008F  CALL	R16	3
      0xB8425800,  //  0090  GETNGBL	R16	K44
      0x8C40212D,  //  0091  GETMET	R16	R16	K45
      0x5848002E,  //  0092  LDCONST	R18	K46
      0x584C002F,  //  0093  LDCONST	R19	K47
      0x7C400600,  //  0094  CALL	R16	3
      0x1C40180F,  //  0095  EQ	R16	R12	R15
      0x78420092,  //  0096  JMPF	R16	#012A
      0x8C400936,  //  0097  GETMET	R16	R4	K54
      0x544A000F,  //  0098  LDINT	R18	16
      0x7C400400,  //  0099  CALL	R16	2
      0x901A6A10,  //  009A  SETMBR	R6	K53	R16
      0x60400015,  //  009B  GETGBL	R16	G21
      0x7C400000,  //  009C  CALL	R16	0
      0x8C402123,  //  009D  GETMET	R16	R16	K35
      0x58480037,  //  009E  LDCONST	R18	K55
      0x7C400400,  //  009F  CALL	R16	2
      0x88440D35,  //  00A0  GETMBR	R17	R6	K53
      0x00402011,  //  00A1  ADD	R16	R16	R17
      0x88440B10,  //  00A2  GETMBR	R17	R5	K16
      0x88480B20,  //  00A3  GETMBR	R18	R5	K32
      0x00442212,  //  00A4  ADD	R17	R17	R18
      0x8C480925,  //  00A5  GETMET	R18	R4	K37
      0x7C480200,  //  00A6  CALL	R18	1
      0x8C482526,  //  00A7  GETMET	R18	R18	K38
      0x88500D22,  //  00A8  GETMBR	R20	R6	K34
      0x5C542200,  //  00A9  MOVE	R21	R17
      0x5C582000,  //  00AA  MOVE	R22	R16
      0x545E000F,  //  00AB  LDINT	R23	16
      0x7C480A00,  //  00AC  CALL	R18	5
      0x8C4C0929,  //  00AD  GETMET	R19	R4	K41
      0x5C542400,  //  00AE  MOVE	R21	R18
      0x60580015,  //  00AF  GETGBL	R22	G21
      0x7C580000,  //  00B0  CALL	R22	0
      0x8C582D23,  //  00B1  GETMET	R22	R22	K35
      0x58600038,  //  00B2  LDCONST	R24	K56
      0x7C580400,  //  00B3  CALL	R22	2
      0x605C0015,  //  00B4  GETGBL	R23	G21
      0x7C5C0000,  //  00B5  CALL	R23	0
      0x58600003,  //  00B6  LDCONST	R24	K3
      0x5466000F,  //  00B7  LDINT	R25	16
      0x7C4C0C00,  //  00B8  CALL	R19	6
      0x8C50272B,  //  00B9  GETMET	R20	R19	K43
      0x7C500200,  //  00BA  CALL	R20	1
      0xB8560E00,  //  00BB  GETNGBL	R21	K7
      0x8C542B39,  //  00BC  GETMET	R21	R21	K57
      0x7C540200,  //  00BD  CALL	R21	1
      0x88580D35,  //  00BE  GETMBR	R22	R6	K53
      0x90564016,  //  00BF  SETMBR	R21	K32	R22
      0x88580D1D,  //  00C0  GETMBR	R22	R6	K29
      0x90567416,  //  00C1  SETMBR	R21	K58	R22
      0x90567614,  //  00C2  SETMBR	R21	K59	R20
      0x8C580925,  //  00C3  GETMET	R22	R4	K37
      0x7C580200,  //  00C4  CALL	R22	1
      0x8C582D26,  //  00C5  GETMET	R22	R22	K38
      0x88600D22,  //  00C6  GETMBR	R24	R6	K34
      0x88640B10,  //  00C7  GETMBR	R25	R5	K16
      0x88680B20,  //  00C8  GETMBR	R26	R5	K32
      0x0064321A,  //  00C9  ADD	R25	R25	R26
      0x60680015,  //  00CA  GETGBL	R26	G21
      0x7C680000,  //  00CB  CALL	R26	0
      0x8C683523,  //  00CC  GETMET	R26	R26	K35
      0x5870003C,  //  00CD  LDCONST	R28	K60
      0x7C680400,  //  00CE  CALL	R26	2
      0x546E002F,  //  00CF  LDINT	R27	48
      0x7C580A00,  //  00D0  CALL	R22	5
      0x545E000E,  //  00D1  LDINT	R23	15
      0x405E0617,  //  00D2  CONNECT	R23	K3	R23
      0x945C2C17,  //  00D3  GETIDX	R23	R22	R23
      0x5462000F,  //  00D4  LDINT	R24	16
      0x5466001E,  //  00D5  LDINT	R25	31
      0x40603019,  //  00D6  CONNECT	R24	R24	R25
      0x94602C18,  //  00D7  GETIDX	R24	R22	R24
      0x5466001F,  //  00D8  LDINT	R25	32
      0x546A002E,  //  00D9  LDINT	R26	47
      0x4064321A,  //  00DA  CONNECT	R25	R25	R26
      0x94642C19,  //  00DB  GETIDX	R25	R22	R25
      0xB86A5800,  //  00DC  GETNGBL	R26	K44
      0x8C68353D,  //  00DD  GETMET	R26	R26	K61
      0x7C680200,  //  00DE  CALL	R26	1
      0x9468353E,  //  00DF  GETIDX	R26	R26	K62
      0xB86E5800,  //  00E0  GETNGBL	R27	K44
      0x8C6C372D,  //  00E1  GETMET	R27	R27	K45
      0x5874003F,  //  00E2  LDCONST	R29	K63
      0x5878002F,  //  00E3  LDCONST	R30	K47
      0x7C6C0600,  //  00E4  CALL	R27	3
      0xB86E5800,  //  00E5  GETNGBL	R27	K44
      0x8C6C372D,  //  00E6  GETMET	R27	R27	K45
      0x8C742F31,  //  00E7  GETMET	R29	R23	K49
      0x7C740200,  //  00E8  CALL	R29	1
      0x0076801D,  //  00E9  ADD	R29	K64	R29
      0x5878002F,  //  00EA  LDCONST	R30	K47
      0x7C6C0600,  //  00EB  CALL	R27	3
      0xB86E5800,  //  00EC  GETNGBL	R27	K44
      0x8C6C372D,  //  00ED  GETMET	R27	R27	K45
      0x8C743131,  //  00EE  GETMET	R29	R24	K49
      0x7C740200,  //  00EF  CALL	R29	1
      0x0076821D,  //  00F0  ADD	R29	K65	R29
      0x5878002F,  //  00F1  LDCONST	R30	K47
      0x7C6C0600,  //  00F2  CALL	R27	3
      0xB86E5800,  //  00F3  GETNGBL	R27	K44
      0x8C6C372D,  //  00F4  GETMET	R27	R27	K45
      0x8C743331,  //  00F5  GETMET	R29	R25	K49
      0x7C740200,  //  00F6  CALL	R29	1
      0x0076841D,  //  00F7  ADD	R29	K66	R29
      0x5878002F,  //  00F8  LDCONST	R30	K47
      0x7C6C0600,  //  00F9  CALL	R27	3
      0xB86E5800,  //  00FA  GETNGBL	R27	K44
      0x8C6C372D,  //  00FB  GETMET	R27	R27	K45
      0x5874003F,  //  00FC  LDCONST	R29	K63
      0x5878002F,  //  00FD  LDCONST	R30	K47
      0x7C6C0600,  //  00FE  CALL	R27	3
      0x8C6C2B43,  //  00FF  GETMET	R27	R21	K67
      0x7C6C0200,  //  0100  CALL	R27	1
      0x4C700000,  //  0101  LDNIL	R28
      0x901A881C,  //  0102  SETMBR	R6	K68	R28
      0xB8725800,  //  0103  GETNGBL	R28	K44
      0x8C70392D,  //  0104  GETMET	R28	R28	K45
      0x8C783731,  //  0105  GETMET	R30	R27	K49
      0x7C780200,  //  0106  CALL	R30	1
      0x007A8A1E,  //  0107  ADD	R30	K69	R30
      0x587C002F,  //  0108  LDCONST	R31	K47
      0x7C700600,  //  0109  CALL	R28	3
      0x8C700346,  //  010A  GETMET	R28	R1	K70
      0x547A0032,  //  010B  LDINT	R30	51
      0x507C0200,  //  010C  LDBOOL	R31	1	0
      0x7C700600,  //  010D  CALL	R28	3
      0x8C743943,  //  010E  GETMET	R29	R28	K67
      0x5C7C3600,  //  010F  MOVE	R31	R27
      0x7C740400,  //  0110  CALL	R29	2
      0x88780147,  //  0111  GETMBR	R30	R0	K71
      0x8C783D48,  //  0112  GETMET	R30	R30	K72
      0x5C803A00,  //  0113  MOVE	R32	R29
      0x5C840400,  //  0114  MOVE	R33	R2
      0x5C880600,  //  0115  MOVE	R34	R3
      0x888C3949,  //  0116  GETMBR	R35	R28	K73
      0x7C780A00,  //  0117  CALL	R30	5
      0x8C780D4A,  //  0118  GETMET	R30	R6	K74
      0x7C780200,  //  0119  CALL	R30	1
      0x8C780D4B,  //  011A  GETMET	R30	R6	K75
      0x5C802E00,  //  011B  MOVE	R32	R23
      0x5C843000,  //  011C  MOVE	R33	R24
      0x5C883200,  //  011D  MOVE	R34	R25
      0x5C8C3400,  //  011E  MOVE	R35	R26
      0x7C780A00,  //  011F  CALL	R30	5
      0x8C780D4C,  //  0120  GETMET	R30	R6	K76
      0x50800200,  //  0121  LDBOOL	R32	1	0
      0x7C780400,  //  0122  CALL	R30	2
      0x8C780D4D,  //  0123  GETMET	R30	R6	K77
      0x7C780200,  //  0124  CALL	R30	1
      0x8C780D4E,  //  0125  GETMET	R30	R6	K78
      0x7C780200,  //  0126  CALL	R30	1
      0x50780200,  //  0127  LDBOOL	R30	1	0
      0x80043C00,  //  0128  RET	1	R30
      0x70020001,  //  0129  JMP		#012C
      0x4C400000,  //  012A  LDNIL	R16
      0x90164010,  //  012B  SETMBR	R5	K32	R16
      0x881C0B20,  //  012C  GETMBR	R7	R5	K32
      0x4C200000,  //  012D  LDNIL	R8
      0x1C1C0E08,  //  012E  EQ	R7	R7	R8
      0x741E0003,  //  012F  JMPT	R7	#0134
      0x881C0B21,  //  0130  GETMBR	R7	R5	K33
      0x4C200000,  //  0131  LDNIL	R8
      0x1C1C0E08,  //  0132  EQ	R7	R7	R8
      0x781E00F0,  //  0133  JMPF	R7	#0225
      0x8C1C0936,  //  0134  GETMET	R7	R4	K54
      0x5426000F,  //  0135  LDINT	R9	16
      0x7C1C0400,  //  0136  CALL	R7	2
      0x901A6A07,  //  0137  SETMBR	R6	K53	R7
      0x8C1C0936,  //  0138  GETMET	R7	R4	K54
      0x5426001F,  //  0139  LDINT	R9	32
      0x7C1C0400,  //  013A  CALL	R7	2
      0x90029E07,  //  013B  SETMBR	R0	K79	R7
      0x8C1C0951,  //  013C  GETMET	R7	R4	K81
      0x7C1C0200,  //  013D  CALL	R7	1
      0x8C1C0F52,  //  013E  GETMET	R7	R7	K82
      0x8824014F,  //  013F  GETMBR	R9	R0	K79
      0x7C1C0400,  //  0140  CALL	R7	2
      0x9002A007,  //  0141  SETMBR	R0	K80	R7
      0x8C1C0936,  //  0142  GETMET	R7	R4	K54
      0x5426001F,  //  0143  LDINT	R9	32
      0x7C1C0400,  //  0144  CALL	R7	2
      0x8C200951,  //  0145  GETMET	R8	R4	K81
      0x7C200200,  //  0146  CALL	R8	1
      0x8C201153,  //  0147  GETMET	R8	R8	K83
      0x8828014F,  //  0148  GETMBR	R10	R0	K79
      0x882C0B0D,  //  0149  GETMBR	R11	R5	K13
      0x7C200600,  //  014A  CALL	R8	3
      0x901A4408,  //  014B  SETMBR	R6	K34	R8
      0xB8220E00,  //  014C  GETNGBL	R8	K7
      0x88201154,  //  014D  GETMBR	R8	R8	K84
      0x8C201155,  //  014E  GETMET	R8	R8	K85
      0x7C200200,  //  014F  CALL	R8	1
      0x8C241156,  //  0150  GETMET	R9	R8	K86
      0x582C0057,  //  0151  LDCONST	R11	K87
      0xB8320E00,  //  0152  GETNGBL	R12	K7
      0x88301954,  //  0153  GETMBR	R12	R12	K84
      0x88301958,  //  0154  GETMBR	R12	R12	K88
      0x8C340D59,  //  0155  GETMET	R13	R6	K89
      0x7C340200,  //  0156  CALL	R13	1
      0x7C240800,  //  0157  CALL	R9	4
      0x8C241156,  //  0158  GETMET	R9	R8	K86
      0x582C005A,  //  0159  LDCONST	R11	K90
      0xB8320E00,  //  015A  GETNGBL	R12	K7
      0x88301954,  //  015B  GETMBR	R12	R12	K84
      0x88301958,  //  015C  GETMBR	R12	R12	K88
      0x8C340D5B,  //  015D  GETMET	R13	R6	K91
      0x7C340200,  //  015E  CALL	R13	1
      0x7C240800,  //  015F  CALL	R9	4
      0x8C241156,  //  0160  GETMET	R9	R8	K86
      0x582C002F,  //  0161  LDCONST	R11	K47
      0xB8320E00,  //  0162  GETNGBL	R12	K7
      0x88301954,  //  0163  GETMBR	R12	R12	K84
      0x88301958,  //  0164  GETMBR	R12	R12	K88
      0x88340150,  //  0165  GETMBR	R13	R0	K80
      0x7C240800,  //  0166  CALL	R9	4
      0x8C241156,  //  0167  GETMET	R9	R8	K86
      0x542E0003,  //  0168  LDINT	R11	4
      0xB8320E00,  //  0169  GETNGBL	R12	K7
      0x88301954,  //  016A  GETMBR	R12	R12	K84
      0x88301958,  //  016B  GETMBR	R12	R12	K88
      0x88340B0D,  //  016C  GETMBR	R13	R5	K13
      0x7C240800,  //  016D  CALL	R9	4
      0x8C240951,  //  016E  GETMET	R9	R4	K81
      0x7C240200,  //  016F  CALL	R9	1
      0x8C24135C,  //  0170  GETMET	R9	R9	K92
      0x8C2C0D5D,  //  0171  GETMET	R11	R6	K93
      0x7C2C0200,  //  0172  CALL	R11	1
      0x8C301143,  //  0173  GETMET	R12	R8	K67
      0x7C300200,  //  0174  CALL	R12	1
      0x7C240600,  //  0175  CALL	R9	3
      0xB82A0E00,  //  0176  GETNGBL	R10	K7
      0x88281554,  //  0177  GETMBR	R10	R10	K84
      0x8C281555,  //  0178  GETMET	R10	R10	K85
      0x7C280200,  //  0179  CALL	R10	1
      0x8C2C1556,  //  017A  GETMET	R11	R10	K86
      0x58340057,  //  017B  LDCONST	R13	K87
      0xB83A0E00,  //  017C  GETNGBL	R14	K7
      0x88381D54,  //  017D  GETMBR	R14	R14	K84
      0x88381D58,  //  017E  GETMBR	R14	R14	K88
      0x8C3C0D59,  //  017F  GETMET	R15	R6	K89
      0x7C3C0200,  //  0180  CALL	R15	1
      0x7C2C0800,  //  0181  CALL	R11	4
      0x8C2C1556,  //  0182  GETMET	R11	R10	K86
      0x5834005A,  //  0183  LDCONST	R13	K90
      0xB83A0E00,  //  0184  GETNGBL	R14	K7
      0x88381D54,  //  0185  GETMBR	R14	R14	K84
      0x88381D58,  //  0186  GETMBR	R14	R14	K88
      0x8C3C0D5B,  //  0187  GETMET	R15	R6	K91
      0x7C3C0200,  //  0188  CALL	R15	1
      0x7C2C0800,  //  0189  CALL	R11	4
      0x8C2C1556,  //  018A  GETMET	R11	R10	K86
      0x5834002F,  //  018B  LDCONST	R13	K47
      0xB83A0E00,  //  018C  GETNGBL	R14	K7
      0x88381D54,  //  018D  GETMBR	R14	R14	K84
      0x88381D58,  //  018E  GETMBR	R14	R14	K88
      0x5C3C1200,  //  018F  MOVE	R15	R9
      0x7C2C0800,  //  0190  CALL	R11	4
      0x8C2C1556,  //  0191  GETMET	R11	R10	K86
      0x54360003,  //  0192  LDINT	R13	4
      0xB83A0E00,  //  0193  GETNGBL	R14	K7
      0x88381D54,  //  0194  GETMBR	R14	R14	K84
      0x88381D58,  //  0195  GETMBR	R14	R14	K88
      0x883C0D35,  //  0196  GETMBR	R15	R6	K53
      0x7C2C0800,  //  0197  CALL	R11	4
      0xB82E5800,  //  0198  GETNGBL	R11	K44
      0x8C2C172D,  //  0199  GETMET	R11	R11	K45
      0x5834002E,  //  019A  LDCONST	R13	K46
      0x5838002F,  //  019B  LDCONST	R14	K47
      0x7C2C0600,  //  019C  CALL	R11	3
      0x882C0B5E,  //  019D  GETMBR	R11	R5	K94
      0x901A880B,  //  019E  SETMBR	R6	K68	R11
      0xB82E5800,  //  019F  GETNGBL	R11	K44
      0x8C2C172D,  //  01A0  GETMET	R11	R11	K45
      0x88340D44,  //  01A1  GETMBR	R13	R6	K68
      0x8C341B31,  //  01A2  GETMET	R13	R13	K49
      0x7C340200,  //  01A3  CALL	R13	1
      0x0036BE0D,  //  01A4  ADD	R13	K95	R13
      0x5838002F,  //  01A5  LDCONST	R14	K47
      0x7C2C0600,  //  01A6  CALL	R11	3
      0x8C2C0960,  //  01A7  GETMET	R11	R4	K96
      0x7C2C0200,  //  01A8  CALL	R11	1
      0x8C2C1761,  //  01A9  GETMET	R11	R11	K97
      0x88340D44,  //  01AA  GETMBR	R13	R6	K68
      0x7C2C0400,  //  01AB  CALL	R11	2
      0x8C2C1762,  //  01AC  GETMET	R11	R11	K98
      0x7C2C0200,  //  01AD  CALL	R11	1
      0x60300015,  //  01AE  GETGBL	R12	G21
      0x7C300000,  //  01AF  CALL	R12	0
      0x8C301923,  //  01B0  GETMET	R12	R12	K35
      0x88380163,  //  01B1  GETMBR	R14	R0	K99
      0x7C300400,  //  01B2  CALL	R12	2
      0x8C340D64,  //  01B3  GETMET	R13	R6	K100
      0x7C340200,  //  01B4  CALL	R13	1
      0x00341A07,  //  01B5  ADD	R13	R13	R7
      0x88380150,  //  01B6  GETMBR	R14	R0	K80
      0x00341A0E,  //  01B7  ADD	R13	R13	R14
      0x00341A0B,  //  01B8  ADD	R13	R13	R11
      0x8C380925,  //  01B9  GETMET	R14	R4	K37
      0x7C380200,  //  01BA  CALL	R14	1
      0x8C381D26,  //  01BB  GETMET	R14	R14	K38
      0x88400D22,  //  01BC  GETMBR	R16	R6	K34
      0x5C441A00,  //  01BD  MOVE	R17	R13
      0x5C481800,  //  01BE  MOVE	R18	R12
      0x544E000F,  //  01BF  LDINT	R19	16
      0x7C380A00,  //  01C0  CALL	R14	5
      0xB83E5800,  //  01C1  GETNGBL	R15	K44
      0x8C3C1F2D,  //  01C2  GETMET	R15	R15	K45
      0x88440D22,  //  01C3  GETMBR	R17	R6	K34
      0x8C442331,  //  01C4  GETMET	R17	R17	K49
      0x7C440200,  //  01C5  CALL	R17	1
      0x0046CA11,  //  01C6  ADD	R17	K101	R17
      0x5848002F,  //  01C7  LDCONST	R18	K47
      0x7C3C0600,  //  01C8  CALL	R15	3
      0xB83E5800,  //  01C9  GETNGBL	R15	K44
      0x8C3C1F2D,  //  01CA  GETMET	R15	R15	K45
      0x8C441B31,  //  01CB  GETMET	R17	R13	K49
      0x7C440200,  //  01CC  CALL	R17	1
      0x0046CC11,  //  01CD  ADD	R17	K102	R17
      0x5848002F,  //  01CE  LDCONST	R18	K47
      0x7C3C0600,  //  01CF  CALL	R15	3
      0xB83E5800,  //  01D0  GETNGBL	R15	K44
      0x8C3C1F2D,  //  01D1  GETMET	R15	R15	K45
      0x8C441D31,  //  01D2  GETMET	R17	R14	K49
      0x7C440200,  //  01D3  CALL	R17	1
      0x0046CE11,  //  01D4  ADD	R17	K103	R17
      0x5848002F,  //  01D5  LDCONST	R18	K47
      0x7C3C0600,  //  01D6  CALL	R15	3
      0x8C3C1543,  //  01D7  GETMET	R15	R10	K67
      0x7C3C0200,  //  01D8  CALL	R15	1
      0x8C400929,  //  01D9  GETMET	R16	R4	K41
      0x5C481C00,  //  01DA  MOVE	R18	R14
      0x604C0015,  //  01DB  GETGBL	R19	G21
      0x7C4C0000,  //  01DC  CALL	R19	0
      0x8C4C2723,  //  01DD  GETMET	R19	R19	K35
      0x88540168,  //  01DE  GETMBR	R21	R0	K104
      0x7C4C0400,  //  01DF  CALL	R19	2
      0x60500015,  //  01E0  GETGBL	R20	G21
      0x7C500000,  //  01E1  CALL	R20	0
      0x6054000C,  //  01E2  GETGBL	R21	G12
      0x5C581E00,  //  01E3  MOVE	R22	R15
      0x7C540200,  //  01E4  CALL	R21	1
      0x545A000F,  //  01E5  LDINT	R22	16
      0x7C400C00,  //  01E6  CALL	R16	6
      0x8C442169,  //  01E7  GETMET	R17	R16	K105
      0x5C4C1E00,  //  01E8  MOVE	R19	R15
      0x7C440400,  //  01E9  CALL	R17	2
      0x8C48212B,  //  01EA  GETMET	R18	R16	K43
      0x7C480200,  //  01EB  CALL	R18	1
      0x00442212,  //  01EC  ADD	R17	R17	R18
      0xB84A5800,  //  01ED  GETNGBL	R18	K44
      0x8C48252D,  //  01EE  GETMET	R18	R18	K45
      0x8C502331,  //  01EF  GETMET	R20	R17	K49
      0x7C500200,  //  01F0  CALL	R20	1
      0x0052D414,  //  01F1  ADD	R20	K106	R20
      0x5854002F,  //  01F2  LDCONST	R21	K47
      0x7C480600,  //  01F3  CALL	R18	3
      0xB84A5800,  //  01F4  GETNGBL	R18	K44
      0x8C48252D,  //  01F5  GETMET	R18	R18	K45
      0x5850002E,  //  01F6  LDCONST	R20	K46
      0x5854002F,  //  01F7  LDCONST	R21	K47
      0x7C480600,  //  01F8  CALL	R18	3
      0xB84A0E00,  //  01F9  GETNGBL	R18	K7
      0x8C48256B,  //  01FA  GETMET	R18	R18	K107
      0x7C480200,  //  01FB  CALL	R18	1
      0x904AD807,  //  01FC  SETMBR	R18	K108	R7
      0x884C011F,  //  01FD  GETMBR	R19	R0	K31
      0x904ADA13,  //  01FE  SETMBR	R18	K109	R19
      0x884C0150,  //  01FF  GETMBR	R19	R0	K80
      0x904ADC13,  //  0200  SETMBR	R18	K110	R19
      0x904ADE11,  //  0201  SETMBR	R18	K111	R17
      0xB84E5800,  //  0202  GETNGBL	R19	K44
      0x8C4C272D,  //  0203  GETMET	R19	R19	K45
      0xB8560E00,  //  0204  GETNGBL	R21	K7
      0x8C542B71,  //  0205  GETMET	R21	R21	K113
      0x5C5C2400,  //  0206  MOVE	R23	R18
      0x7C540400,  //  0207  CALL	R21	2
      0x0056E015,  //  0208  ADD	R21	K112	R21
      0x5858002F,  //  0209  LDCONST	R22	K47
      0x7C4C0600,  //  020A  CALL	R19	3
      0x8C4C2543,  //  020B  GETMET	R19	R18	K67
      0x7C4C0200,  //  020C  CALL	R19	1
      0x901AE413,  //  020D  SETMBR	R6	K114	R19
      0xB8525800,  //  020E  GETNGBL	R20	K44
      0x8C50292D,  //  020F  GETMET	R20	R20	K45
      0x8C582731,  //  0210  GETMET	R22	R19	K49
      0x7C580200,  //  0211  CALL	R22	1
      0x005AE616,  //  0212  ADD	R22	K115	R22
      0x585C002F,  //  0213  LDCONST	R23	K47
      0x7C500600,  //  0214  CALL	R20	3
      0x8C500346,  //  0215  GETMET	R20	R1	K70
      0x545A0030,  //  0216  LDINT	R22	49
      0x505C0200,  //  0217  LDBOOL	R23	1	0
      0x7C500600,  //  0218  CALL	R20	3
      0x8C542943,  //  0219  GETMET	R21	R20	K67
      0x5C5C2600,  //  021A  MOVE	R23	R19
      0x7C540400,  //  021B  CALL	R21	2
      0x88580147,  //  021C  GETMBR	R22	R0	K71
      0x8C582D48,  //  021D  GETMET	R22	R22	K72
      0x5C602A00,  //  021E  MOVE	R24	R21
      0x5C640400,  //  021F  MOVE	R25	R2
      0x5C680600,  //  0220  MOVE	R26	R3
      0x886C2949,  //  0221  GETMBR	R27	R20	K73
      0x7C580A00,  //  0222  CALL	R22	5
      0x50580200,  //  0223  LDBOOL	R22	1	0
      0x80042C00,  //  0224  RET	1	R22
      0x501C0200,  //  0225  LDBOOL	R7	1	0
      0x80040E00,  //  0226  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_Pake3
********************************************************************/
be_local_closure(Matter_Commisioning_Context_parse_Pake3,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[45]) {     /* constants */
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
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(spake),
    /* K19  */  be_nested_str_weak(invalid_X20cA_X20received),
    /* K20  */  be_nested_str_weak(session_timestamp),
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
    /* K31  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20session_keys_X3D),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K36  */  be_nested_str_weak(build_response),
    /* K37  */  be_nested_str_weak(add),
    /* K38  */  be_const_int(2),
    /* K39  */  be_nested_str_weak(encode),
    /* K40  */  be_nested_str_weak(responder),
    /* K41  */  be_nested_str_weak(send_response),
    /* K42  */  be_nested_str_weak(add_session),
    /* K43  */  be_nested_str_weak(future_local_session_id),
    /* K44  */  be_nested_str_weak(future_initiator_session_id),
    }),
    be_str_weak(parse_Pake3),
    &be_const_str_solidified,
    ( &(const binstruction[146]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x88140301,  //  0001  GETMBR	R5	R1	K1
      0x541A0023,  //  0002  LDINT	R6	36
      0x20140A06,  //  0003  NE	R5	R5	R6
      0x74160005,  //  0004  JMPT	R5	#000B
      0x88140302,  //  0005  GETMBR	R5	R1	K2
      0x20140B03,  //  0006  NE	R5	R5	K3
      0x74160002,  //  0007  JMPT	R5	#000B
      0x88140304,  //  0008  GETMBR	R5	R1	K4
      0x20140B03,  //  0009  NE	R5	R5	K3
      0x78160000,  //  000A  JMPF	R5	#000C
      0xB0060B06,  //  000B  RAISE	1	K5	K6
      0xB8160E00,  //  000C  GETNGBL	R5	K7
      0x8C140B08,  //  000D  GETMET	R5	R5	K8
      0x7C140200,  //  000E  CALL	R5	1
      0x8C140B09,  //  000F  GETMET	R5	R5	K9
      0x881C030A,  //  0010  GETMBR	R7	R1	K10
      0x8820030B,  //  0011  GETMBR	R8	R1	K11
      0x7C140600,  //  0012  CALL	R5	3
      0x88180B0C,  //  0013  GETMBR	R6	R5	K12
      0x90021806,  //  0014  SETMBR	R0	K12	R6
      0xB81A1A00,  //  0015  GETNGBL	R6	K13
      0x8C180D0E,  //  0016  GETMET	R6	R6	K14
      0x8820010C,  //  0017  GETMBR	R8	R0	K12
      0x8C201110,  //  0018  GETMET	R8	R8	K16
      0x7C200200,  //  0019  CALL	R8	1
      0x00221E08,  //  001A  ADD	R8	K15	R8
      0x58240011,  //  001B  LDCONST	R9	K17
      0x7C180600,  //  001C  CALL	R6	3
      0x8818010C,  //  001D  GETMBR	R6	R0	K12
      0x881C0112,  //  001E  GETMBR	R7	R0	K18
      0x881C0F0C,  //  001F  GETMBR	R7	R7	K12
      0x20180C07,  //  0020  NE	R6	R6	R7
      0x781A0000,  //  0021  JMPF	R6	#0023
      0xB0060B13,  //  0022  RAISE	1	K5	K19
      0xB81A1A00,  //  0023  GETNGBL	R6	K13
      0x8C180D15,  //  0024  GETMET	R6	R6	K21
      0x7C180200,  //  0025  CALL	R6	1
      0x94180D16,  //  0026  GETIDX	R6	R6	K22
      0x90022806,  //  0027  SETMBR	R0	K20	R6
      0x8C180917,  //  0028  GETMET	R6	R4	K23
      0x7C180200,  //  0029  CALL	R6	1
      0x8C180D18,  //  002A  GETMET	R6	R6	K24
      0x88200119,  //  002B  GETMBR	R8	R0	K25
      0x60240015,  //  002C  GETGBL	R9	G21
      0x7C240000,  //  002D  CALL	R9	0
      0x60280015,  //  002E  GETGBL	R10	G21
      0x7C280000,  //  002F  CALL	R10	0
      0x8C28151A,  //  0030  GETMET	R10	R10	K26
      0x8830011B,  //  0031  GETMBR	R12	R0	K27
      0x7C280400,  //  0032  CALL	R10	2
      0x542E002F,  //  0033  LDINT	R11	48
      0x7C180A00,  //  0034  CALL	R6	5
      0x541E000E,  //  0035  LDINT	R7	15
      0x401E0607,  //  0036  CONNECT	R7	K3	R7
      0x941C0C07,  //  0037  GETIDX	R7	R6	R7
      0x90023807,  //  0038  SETMBR	R0	K28	R7
      0x541E000F,  //  0039  LDINT	R7	16
      0x5422001E,  //  003A  LDINT	R8	31
      0x401C0E08,  //  003B  CONNECT	R7	R7	R8
      0x941C0C07,  //  003C  GETIDX	R7	R6	R7
      0x90023A07,  //  003D  SETMBR	R0	K29	R7
      0x541E001F,  //  003E  LDINT	R7	32
      0x5422002E,  //  003F  LDINT	R8	47
      0x401C0E08,  //  0040  CONNECT	R7	R7	R8
      0x941C0C07,  //  0041  GETIDX	R7	R6	R7
      0x90023C07,  //  0042  SETMBR	R0	K30	R7
      0xB81E1A00,  //  0043  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0044  GETMET	R7	R7	K14
      0x5824001F,  //  0045  LDCONST	R9	K31
      0x58280011,  //  0046  LDCONST	R10	K17
      0x7C1C0600,  //  0047  CALL	R7	3
      0xB81E1A00,  //  0048  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0049  GETMET	R7	R7	K14
      0x8C240D10,  //  004A  GETMET	R9	R6	K16
      0x7C240200,  //  004B  CALL	R9	1
      0x00264009,  //  004C  ADD	R9	K32	R9
      0x58280011,  //  004D  LDCONST	R10	K17
      0x7C1C0600,  //  004E  CALL	R7	3
      0xB81E1A00,  //  004F  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0050  GETMET	R7	R7	K14
      0x8824011C,  //  0051  GETMBR	R9	R0	K28
      0x8C241310,  //  0052  GETMET	R9	R9	K16
      0x7C240200,  //  0053  CALL	R9	1
      0x00264209,  //  0054  ADD	R9	K33	R9
      0x58280011,  //  0055  LDCONST	R10	K17
      0x7C1C0600,  //  0056  CALL	R7	3
      0xB81E1A00,  //  0057  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0058  GETMET	R7	R7	K14
      0x8824011D,  //  0059  GETMBR	R9	R0	K29
      0x8C241310,  //  005A  GETMET	R9	R9	K16
      0x7C240200,  //  005B  CALL	R9	1
      0x00264409,  //  005C  ADD	R9	K34	R9
      0x58280011,  //  005D  LDCONST	R10	K17
      0x7C1C0600,  //  005E  CALL	R7	3
      0xB81E1A00,  //  005F  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0060  GETMET	R7	R7	K14
      0x8824011E,  //  0061  GETMBR	R9	R0	K30
      0x8C241310,  //  0062  GETMET	R9	R9	K16
      0x7C240200,  //  0063  CALL	R9	1
      0x00264609,  //  0064  ADD	R9	K35	R9
      0x58280011,  //  0065  LDCONST	R10	K17
      0x7C1C0600,  //  0066  CALL	R7	3
      0xB81E1A00,  //  0067  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0068  GETMET	R7	R7	K14
      0x5824001F,  //  0069  LDCONST	R9	K31
      0x58280011,  //  006A  LDCONST	R10	K17
      0x7C1C0600,  //  006B  CALL	R7	3
      0x8C1C0324,  //  006C  GETMET	R7	R1	K36
      0x5426003F,  //  006D  LDINT	R9	64
      0x50280000,  //  006E  LDBOOL	R10	0	0
      0x7C1C0600,  //  006F  CALL	R7	3
      0x60200015,  //  0070  GETGBL	R8	G21
      0x7C200000,  //  0071  CALL	R8	0
      0x8C241125,  //  0072  GETMET	R9	R8	K37
      0x582C0003,  //  0073  LDCONST	R11	K3
      0x58300026,  //  0074  LDCONST	R12	K38
      0x7C240600,  //  0075  CALL	R9	3
      0x8C241125,  //  0076  GETMET	R9	R8	K37
      0x582C0003,  //  0077  LDCONST	R11	K3
      0x54320003,  //  0078  LDINT	R12	4
      0x7C240600,  //  0079  CALL	R9	3
      0x8C241125,  //  007A  GETMET	R9	R8	K37
      0x582C0003,  //  007B  LDCONST	R11	K3
      0x54320003,  //  007C  LDINT	R12	4
      0x7C240600,  //  007D  CALL	R9	3
      0x8C240F27,  //  007E  GETMET	R9	R7	K39
      0x5C2C1000,  //  007F  MOVE	R11	R8
      0x7C240400,  //  0080  CALL	R9	2
      0x88280128,  //  0081  GETMBR	R10	R0	K40
      0x8C281529,  //  0082  GETMET	R10	R10	K41
      0x5C301200,  //  0083  MOVE	R12	R9
      0x5C340400,  //  0084  MOVE	R13	R2
      0x5C380600,  //  0085  MOVE	R14	R3
      0x4C3C0000,  //  0086  LDNIL	R15
      0x7C280A00,  //  0087  CALL	R10	5
      0x88280128,  //  0088  GETMBR	R10	R0	K40
      0x8C28152A,  //  0089  GETMET	R10	R10	K42
      0x8830012B,  //  008A  GETMBR	R12	R0	K43
      0x8834012C,  //  008B  GETMBR	R13	R0	K44
      0x8838011C,  //  008C  GETMBR	R14	R0	K28
      0x883C011D,  //  008D  GETMBR	R15	R0	K29
      0x8840011E,  //  008E  GETMBR	R16	R0	K30
      0x88440114,  //  008F  GETMBR	R17	R0	K20
      0x7C280E00,  //  0090  CALL	R10	7
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
    9,                          /* nstack */
    4,                          /* argc */
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
    ( &(const binstruction[74]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x74120006,  //  0001  JMPT	R4	#0009
      0xB8120200,  //  0002  GETNGBL	R4	K1
      0x8C100902,  //  0003  GETMET	R4	R4	K2
      0x58180003,  //  0004  LDCONST	R6	K3
      0x581C0004,  //  0005  LDCONST	R7	K4
      0x7C100600,  //  0006  CALL	R4	3
      0x50100000,  //  0007  LDBOOL	R4	0	0
      0x80040800,  //  0008  RET	1	R4
      0xB8120200,  //  0009  GETNGBL	R4	K1
      0x8C100902,  //  000A  GETMET	R4	R4	K2
      0xB81A0C00,  //  000B  GETNGBL	R6	K6
      0x8C180D07,  //  000C  GETMET	R6	R6	K7
      0x5C200200,  //  000D  MOVE	R8	R1
      0x7C180400,  //  000E  CALL	R6	2
      0x001A0A06,  //  000F  ADD	R6	K5	R6
      0x581C0008,  //  0010  LDCONST	R7	K8
      0x7C100600,  //  0011  CALL	R4	3
      0x88100309,  //  0012  GETMBR	R4	R1	K9
      0x5416001F,  //  0013  LDINT	R5	32
      0x1C100805,  //  0014  EQ	R4	R4	R5
      0x78120006,  //  0015  JMPF	R4	#001D
      0x8C10010A,  //  0016  GETMET	R4	R0	K10
      0x5C180200,  //  0017  MOVE	R6	R1
      0x5C1C0400,  //  0018  MOVE	R7	R2
      0x5C200600,  //  0019  MOVE	R8	R3
      0x7C100800,  //  001A  CALL	R4	4
      0x80040800,  //  001B  RET	1	R4
      0x7002002A,  //  001C  JMP		#0048
      0x88100309,  //  001D  GETMBR	R4	R1	K9
      0x54160021,  //  001E  LDINT	R5	34
      0x1C100805,  //  001F  EQ	R4	R4	R5
      0x78120006,  //  0020  JMPF	R4	#0028
      0x8C10010B,  //  0021  GETMET	R4	R0	K11
      0x5C180200,  //  0022  MOVE	R6	R1
      0x5C1C0400,  //  0023  MOVE	R7	R2
      0x5C200600,  //  0024  MOVE	R8	R3
      0x7C100800,  //  0025  CALL	R4	4
      0x80040800,  //  0026  RET	1	R4
      0x7002001F,  //  0027  JMP		#0048
      0x88100309,  //  0028  GETMBR	R4	R1	K9
      0x54160023,  //  0029  LDINT	R5	36
      0x1C100805,  //  002A  EQ	R4	R4	R5
      0x78120006,  //  002B  JMPF	R4	#0033
      0x8C10010C,  //  002C  GETMET	R4	R0	K12
      0x5C180200,  //  002D  MOVE	R6	R1
      0x5C1C0400,  //  002E  MOVE	R7	R2
      0x5C200600,  //  002F  MOVE	R8	R3
      0x7C100800,  //  0030  CALL	R4	4
      0x80040800,  //  0031  RET	1	R4
      0x70020014,  //  0032  JMP		#0048
      0x88100309,  //  0033  GETMBR	R4	R1	K9
      0x5416002F,  //  0034  LDINT	R5	48
      0x1C100805,  //  0035  EQ	R4	R4	R5
      0x78120006,  //  0036  JMPF	R4	#003E
      0x8C10010D,  //  0037  GETMET	R4	R0	K13
      0x5C180200,  //  0038  MOVE	R6	R1
      0x5C1C0400,  //  0039  MOVE	R7	R2
      0x5C200600,  //  003A  MOVE	R8	R3
      0x7C100800,  //  003B  CALL	R4	4
      0x80040800,  //  003C  RET	1	R4
      0x70020009,  //  003D  JMP		#0048
      0x88100309,  //  003E  GETMBR	R4	R1	K9
      0x54160031,  //  003F  LDINT	R5	50
      0x1C100805,  //  0040  EQ	R4	R4	R5
      0x78120005,  //  0041  JMPF	R4	#0048
      0x8C10010E,  //  0042  GETMET	R4	R0	K14
      0x5C180200,  //  0043  MOVE	R6	R1
      0x5C1C0400,  //  0044  MOVE	R7	R2
      0x5C200600,  //  0045  MOVE	R8	R3
      0x7C100800,  //  0046  CALL	R4	4
      0x80040800,  //  0047  RET	1	R4
      0x50100000,  //  0048  LDBOOL	R4	0	0
      0x80040800,  //  0049  RET	1	R4
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
