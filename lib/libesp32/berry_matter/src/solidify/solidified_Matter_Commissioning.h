/* Solidification of Matter_Commissioning.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Commisioning_Context;

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
    /* K19  */  be_nested_str_weak(created),
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
    ( &(const bvalue[16]) {     /* constants */
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
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
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
      0x70020022,  //  0024  JMP		#0048
      0x88080302,  //  0025  GETMBR	R2	R1	K2
      0x540E0021,  //  0026  LDINT	R3	34
      0x1C080403,  //  0027  EQ	R2	R2	R3
      0x780A0004,  //  0028  JMPF	R2	#002E
      0x8C08010C,  //  0029  GETMET	R2	R0	K12
      0x5C100200,  //  002A  MOVE	R4	R1
      0x7C080400,  //  002B  CALL	R2	2
      0x80040400,  //  002C  RET	1	R2
      0x70020019,  //  002D  JMP		#0048
      0x88080302,  //  002E  GETMBR	R2	R1	K2
      0x540E0023,  //  002F  LDINT	R3	36
      0x1C080403,  //  0030  EQ	R2	R2	R3
      0x780A0004,  //  0031  JMPF	R2	#0037
      0x8C08010D,  //  0032  GETMET	R2	R0	K13
      0x5C100200,  //  0033  MOVE	R4	R1
      0x7C080400,  //  0034  CALL	R2	2
      0x80040400,  //  0035  RET	1	R2
      0x70020010,  //  0036  JMP		#0048
      0x88080302,  //  0037  GETMBR	R2	R1	K2
      0x540E002F,  //  0038  LDINT	R3	48
      0x1C080403,  //  0039  EQ	R2	R2	R3
      0x780A0004,  //  003A  JMPF	R2	#0040
      0x8C08010E,  //  003B  GETMET	R2	R0	K14
      0x5C100200,  //  003C  MOVE	R4	R1
      0x7C080400,  //  003D  CALL	R2	2
      0x80040400,  //  003E  RET	1	R2
      0x70020007,  //  003F  JMP		#0048
      0x88080302,  //  0040  GETMBR	R2	R1	K2
      0x540E0031,  //  0041  LDINT	R3	50
      0x1C080403,  //  0042  EQ	R2	R2	R3
      0x780A0003,  //  0043  JMPF	R2	#0048
      0x8C08010F,  //  0044  GETMET	R2	R0	K15
      0x5C100200,  //  0045  MOVE	R4	R1
      0x7C080400,  //  0046  CALL	R2	2
      0x80040400,  //  0047  RET	1	R2
      0x50080000,  //  0048  LDBOOL	R2	0	0
      0x80040400,  //  0049  RET	1	R2
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
    ( &(const bvalue[115]) {     /* constants */
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
    /* K16  */  be_nested_str_weak(session),
    /* K17  */  be_nested_str_weak(device),
    /* K18  */  be_nested_str_weak(sessions),
    /* K19  */  be_nested_str_weak(find_session_by_resumption_id),
    /* K20  */  be_nested_str_weak(find_fabric_by_destination_id),
    /* K21  */  be_nested_str_weak(destinationId),
    /* K22  */  be_nested_str_weak(initiatorRandom),
    /* K23  */  be_nested_str_weak(_fabric),
    /* K24  */  be_nested_str_weak(valuer_error),
    /* K25  */  be_nested_str_weak(StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K26  */  be_nested_str_weak(source_node_id),
    /* K27  */  be_nested_str_weak(set_mode_CASE),
    /* K28  */  be_nested_str_weak(remove_session),
    /* K29  */  be_nested_str_weak(__future_initiator_session_id),
    /* K30  */  be_nested_str_weak(initiator_session_id),
    /* K31  */  be_nested_str_weak(__future_local_session_id),
    /* K32  */  be_nested_str_weak(gen_local_session_id),
    /* K33  */  be_nested_str_weak(future_local_session_id),
    /* K34  */  be_nested_str_weak(tasmota),
    /* K35  */  be_nested_str_weak(log),
    /* K36  */  be_nested_str_weak(MTR_X3A_X20Loc_session_X3D),
    /* K37  */  be_nested_str_weak(fromstring),
    /* K38  */  be_nested_str_weak(Sigma1_Resume),
    /* K39  */  be_nested_str_weak(HKDF_SHA256),
    /* K40  */  be_nested_str_weak(derive),
    /* K41  */  be_nested_str_weak(shared_secret),
    /* K42  */  be_nested_str_weak(NCASE_SigmaR1),
    /* K43  */  be_const_int(2147483647),
    /* K44  */  be_nested_str_weak(AES_CCM),
    /* K45  */  be_nested_str_weak(decrypt),
    /* K46  */  be_nested_str_weak(tag),
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
    /* K68  */  be_nested_str_weak(__Msg1),
    /* K69  */  be_nested_str_weak(MTR_X3A_X20sigma2resume_raw_X3A_X20),
    /* K70  */  be_nested_str_weak(build_response),
    /* K71  */  be_nested_str_weak(responder),
    /* K72  */  be_nested_str_weak(send_response),
    /* K73  */  be_nested_str_weak(remote_ip),
    /* K74  */  be_nested_str_weak(remote_port),
    /* K75  */  be_nested_str_weak(message_counter),
    /* K76  */  be_nested_str_weak(set_keys),
    /* K77  */  be_nested_str_weak(ResponderEph_priv),
    /* K78  */  be_nested_str_weak(ResponderEph_pub),
    /* K79  */  be_nested_str_weak(EC_P256),
    /* K80  */  be_nested_str_weak(public_key),
    /* K81  */  be_nested_str_weak(shared_key),
    /* K82  */  be_nested_str_weak(TLV),
    /* K83  */  be_nested_str_weak(Matter_TLV_struct),
    /* K84  */  be_nested_str_weak(add_TLV),
    /* K85  */  be_const_int(1),
    /* K86  */  be_nested_str_weak(B2),
    /* K87  */  be_nested_str_weak(get_noc),
    /* K88  */  be_const_int(2),
    /* K89  */  be_nested_str_weak(get_icac),
    /* K90  */  be_const_int(3),
    /* K91  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K92  */  be_nested_str_weak(get_pk),
    /* K93  */  be_nested_str_weak(Msg1),
    /* K94  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20MSG1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K95  */  be_nested_str_weak(SHA256),
    /* K96  */  be_nested_str_weak(update),
    /* K97  */  be_nested_str_weak(out),
    /* K98  */  be_nested_str_weak(S2K_Info),
    /* K99  */  be_nested_str_weak(get_ipk_group_key),
    /* K100 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20SharedSecret_X20_X20_X3D_X20),
    /* K101 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K102 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K103 */  be_nested_str_weak(TBEData2_Nonce),
    /* K104 */  be_nested_str_weak(encrypt),
    /* K105 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Enc_X20_X20_X20_X3D_X20),
    /* K106 */  be_nested_str_weak(Sigma2),
    /* K107 */  be_nested_str_weak(responderRandom),
    /* K108 */  be_nested_str_weak(responderSessionId),
    /* K109 */  be_nested_str_weak(responderEphPubKey),
    /* K110 */  be_nested_str_weak(encrypted2),
    /* K111 */  be_nested_str_weak(MTR_X3A_X20sigma2_X3A_X20),
    /* K112 */  be_nested_str_weak(inspect),
    /* K113 */  be_nested_str_weak(__Msg2),
    /* K114 */  be_nested_str_weak(MTR_X3A_X20sigma2_raw_X3A_X20),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[560]) {  /* code */
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
      0x88140310,  //  001F  GETMBR	R5	R1	K16
      0x78120006,  //  0020  JMPF	R4	#0028
      0x88180111,  //  0021  GETMBR	R6	R0	K17
      0x88180D12,  //  0022  GETMBR	R6	R6	K18
      0x8C180D13,  //  0023  GETMET	R6	R6	K19
      0x8820070E,  //  0024  GETMBR	R8	R3	K14
      0x7C180400,  //  0025  CALL	R6	2
      0x5C140C00,  //  0026  MOVE	R5	R6
      0x70020004,  //  0027  JMP		#002D
      0x8C180114,  //  0028  GETMET	R6	R0	K20
      0x88200715,  //  0029  GETMBR	R8	R3	K21
      0x88240716,  //  002A  GETMBR	R9	R3	K22
      0x7C180600,  //  002B  CALL	R6	3
      0x90162E06,  //  002C  SETMBR	R5	K23	R6
      0x4C180000,  //  002D  LDNIL	R6
      0x1C180A06,  //  002E  EQ	R6	R5	R6
      0x741A0003,  //  002F  JMPT	R6	#0034
      0x88180B17,  //  0030  GETMBR	R6	R5	K23
      0x4C1C0000,  //  0031  LDNIL	R7
      0x1C180C07,  //  0032  EQ	R6	R6	R7
      0x781A0000,  //  0033  JMPF	R6	#0035
      0xB0063119,  //  0034  RAISE	1	K24	K25
      0x8818031A,  //  0035  GETMBR	R6	R1	K26
      0x90163406,  //  0036  SETMBR	R5	K26	R6
      0x8C180B1B,  //  0037  GETMET	R6	R5	K27
      0x7C180200,  //  0038  CALL	R6	1
      0x88180310,  //  0039  GETMBR	R6	R1	K16
      0x20180C05,  //  003A  NE	R6	R6	R5
      0x781A0004,  //  003B  JMPF	R6	#0041
      0x88180111,  //  003C  GETMBR	R6	R0	K17
      0x88180D12,  //  003D  GETMBR	R6	R6	K18
      0x8C180D1C,  //  003E  GETMET	R6	R6	K28
      0x88200310,  //  003F  GETMBR	R8	R1	K16
      0x7C180400,  //  0040  CALL	R6	2
      0x90062005,  //  0041  SETMBR	R1	K16	R5
      0x8818071E,  //  0042  GETMBR	R6	R3	K30
      0x90163A06,  //  0043  SETMBR	R5	K29	R6
      0x88180111,  //  0044  GETMBR	R6	R0	K17
      0x88180D12,  //  0045  GETMBR	R6	R6	K18
      0x8C180D20,  //  0046  GETMET	R6	R6	K32
      0x7C180200,  //  0047  CALL	R6	1
      0x90163E06,  //  0048  SETMBR	R5	K31	R6
      0x88180B1F,  //  0049  GETMBR	R6	R5	K31
      0x90024206,  //  004A  SETMBR	R0	K33	R6
      0xB81A4400,  //  004B  GETNGBL	R6	K34
      0x8C180D23,  //  004C  GETMET	R6	R6	K35
      0x60200008,  //  004D  GETGBL	R8	G8
      0x88240121,  //  004E  GETMBR	R9	R0	K33
      0x7C200200,  //  004F  CALL	R8	1
      0x00224808,  //  0050  ADD	R8	K36	R8
      0x7C180400,  //  0051  CALL	R6	2
      0x781200E1,  //  0052  JMPF	R4	#0135
      0x88180716,  //  0053  GETMBR	R6	R3	K22
      0x881C070E,  //  0054  GETMBR	R7	R3	K14
      0x00180C07,  //  0055  ADD	R6	R6	R7
      0x601C0015,  //  0056  GETGBL	R7	G21
      0x7C1C0000,  //  0057  CALL	R7	0
      0x8C1C0F25,  //  0058  GETMET	R7	R7	K37
      0x58240026,  //  0059  LDCONST	R9	K38
      0x7C1C0400,  //  005A  CALL	R7	2
      0x8C200527,  //  005B  GETMET	R8	R2	K39
      0x7C200200,  //  005C  CALL	R8	1
      0x8C201128,  //  005D  GETMET	R8	R8	K40
      0x88280B29,  //  005E  GETMBR	R10	R5	K41
      0x5C2C0C00,  //  005F  MOVE	R11	R6
      0x5C300E00,  //  0060  MOVE	R12	R7
      0x5436000F,  //  0061  LDINT	R13	16
      0x7C200A00,  //  0062  CALL	R8	5
      0x60240015,  //  0063  GETGBL	R9	G21
      0x7C240000,  //  0064  CALL	R9	0
      0x8C241325,  //  0065  GETMET	R9	R9	K37
      0x582C002A,  //  0066  LDCONST	R11	K42
      0x7C240400,  //  0067  CALL	R9	2
      0x5429FFEE,  //  0068  LDINT	R10	-17
      0x402A060A,  //  0069  CONNECT	R10	K3	R10
      0x882C070F,  //  006A  GETMBR	R11	R3	K15
      0x9428160A,  //  006B  GETIDX	R10	R11	R10
      0x5431FFEF,  //  006C  LDINT	R12	-16
      0x4030192B,  //  006D  CONNECT	R12	R12	K43
      0x8834070F,  //  006E  GETMBR	R13	R3	K15
      0x942C1A0C,  //  006F  GETIDX	R11	R13	R12
      0x8C38052C,  //  0070  GETMET	R14	R2	K44
      0x5C401000,  //  0071  MOVE	R16	R8
      0x5C441200,  //  0072  MOVE	R17	R9
      0x60480015,  //  0073  GETGBL	R18	G21
      0x7C480000,  //  0074  CALL	R18	0
      0x604C000C,  //  0075  GETGBL	R19	G12
      0x5C501400,  //  0076  MOVE	R20	R10
      0x7C4C0200,  //  0077  CALL	R19	1
      0x5452000F,  //  0078  LDINT	R20	16
      0x7C380C00,  //  0079  CALL	R14	6
      0x5C301C00,  //  007A  MOVE	R12	R14
      0x8C38192D,  //  007B  GETMET	R14	R12	K45
      0x5C401400,  //  007C  MOVE	R16	R10
      0x7C380400,  //  007D  CALL	R14	2
      0x5C341C00,  //  007E  MOVE	R13	R14
      0x8C38192E,  //  007F  GETMET	R14	R12	K46
      0x7C380200,  //  0080  CALL	R14	1
      0xB83E4400,  //  0081  GETNGBL	R15	K34
      0x8C3C1F23,  //  0082  GETMET	R15	R15	K35
      0x5844002F,  //  0083  LDCONST	R17	K47
      0x544A0003,  //  0084  LDINT	R18	4
      0x7C3C0600,  //  0085  CALL	R15	3
      0xB83E4400,  //  0086  GETNGBL	R15	K34
      0x8C3C1F23,  //  0087  GETMET	R15	R15	K35
      0x8C441131,  //  0088  GETMET	R17	R8	K49
      0x7C440200,  //  0089  CALL	R17	1
      0x00466011,  //  008A  ADD	R17	K48	R17
      0x544A0003,  //  008B  LDINT	R18	4
      0x7C3C0600,  //  008C  CALL	R15	3
      0xB83E4400,  //  008D  GETNGBL	R15	K34
      0x8C3C1F23,  //  008E  GETMET	R15	R15	K35
      0x8C441731,  //  008F  GETMET	R17	R11	K49
      0x7C440200,  //  0090  CALL	R17	1
      0x00466411,  //  0091  ADD	R17	K50	R17
      0x544A0003,  //  0092  LDINT	R18	4
      0x7C3C0600,  //  0093  CALL	R15	3
      0xB83E4400,  //  0094  GETNGBL	R15	K34
      0x8C3C1F23,  //  0095  GETMET	R15	R15	K35
      0x8C441B31,  //  0096  GETMET	R17	R13	K49
      0x7C440200,  //  0097  CALL	R17	1
      0x00466611,  //  0098  ADD	R17	K51	R17
      0x544A0003,  //  0099  LDINT	R18	4
      0x7C3C0600,  //  009A  CALL	R15	3
      0xB83E4400,  //  009B  GETNGBL	R15	K34
      0x8C3C1F23,  //  009C  GETMET	R15	R15	K35
      0x8C441D31,  //  009D  GETMET	R17	R14	K49
      0x7C440200,  //  009E  CALL	R17	1
      0x00466811,  //  009F  ADD	R17	K52	R17
      0x544A0003,  //  00A0  LDINT	R18	4
      0x7C3C0600,  //  00A1  CALL	R15	3
      0xB83E4400,  //  00A2  GETNGBL	R15	K34
      0x8C3C1F23,  //  00A3  GETMET	R15	R15	K35
      0x5844002F,  //  00A4  LDCONST	R17	K47
      0x544A0003,  //  00A5  LDINT	R18	4
      0x7C3C0600,  //  00A6  CALL	R15	3
      0x1C3C160E,  //  00A7  EQ	R15	R11	R14
      0x783E0089,  //  00A8  JMPF	R15	#0133
      0x8C3C0536,  //  00A9  GETMET	R15	R2	K54
      0x5446000F,  //  00AA  LDINT	R17	16
      0x7C3C0400,  //  00AB  CALL	R15	2
      0x90166A0F,  //  00AC  SETMBR	R5	K53	R15
      0x603C0015,  //  00AD  GETGBL	R15	G21
      0x7C3C0000,  //  00AE  CALL	R15	0
      0x8C3C1F25,  //  00AF  GETMET	R15	R15	K37
      0x58440037,  //  00B0  LDCONST	R17	K55
      0x7C3C0400,  //  00B1  CALL	R15	2
      0x88400B35,  //  00B2  GETMBR	R16	R5	K53
      0x003C1E10,  //  00B3  ADD	R15	R15	R16
      0x88400716,  //  00B4  GETMBR	R16	R3	K22
      0x8844070E,  //  00B5  GETMBR	R17	R3	K14
      0x00402011,  //  00B6  ADD	R16	R16	R17
      0x8C440527,  //  00B7  GETMET	R17	R2	K39
      0x7C440200,  //  00B8  CALL	R17	1
      0x8C442328,  //  00B9  GETMET	R17	R17	K40
      0x884C0B29,  //  00BA  GETMBR	R19	R5	K41
      0x5C502000,  //  00BB  MOVE	R20	R16
      0x5C541E00,  //  00BC  MOVE	R21	R15
      0x545A000F,  //  00BD  LDINT	R22	16
      0x7C440A00,  //  00BE  CALL	R17	5
      0x8C48052C,  //  00BF  GETMET	R18	R2	K44
      0x5C502200,  //  00C0  MOVE	R20	R17
      0x60540015,  //  00C1  GETGBL	R21	G21
      0x7C540000,  //  00C2  CALL	R21	0
      0x8C542B25,  //  00C3  GETMET	R21	R21	K37
      0x585C0038,  //  00C4  LDCONST	R23	K56
      0x7C540400,  //  00C5  CALL	R21	2
      0x60580015,  //  00C6  GETGBL	R22	G21
      0x7C580000,  //  00C7  CALL	R22	0
      0x585C0003,  //  00C8  LDCONST	R23	K3
      0x5462000F,  //  00C9  LDINT	R24	16
      0x7C480C00,  //  00CA  CALL	R18	6
      0x8C4C252E,  //  00CB  GETMET	R19	R18	K46
      0x7C4C0200,  //  00CC  CALL	R19	1
      0xB8520E00,  //  00CD  GETNGBL	R20	K7
      0x8C502939,  //  00CE  GETMET	R20	R20	K57
      0x7C500200,  //  00CF  CALL	R20	1
      0x88540B35,  //  00D0  GETMBR	R21	R5	K53
      0x90521C15,  //  00D1  SETMBR	R20	K14	R21
      0x88540B1F,  //  00D2  GETMBR	R21	R5	K31
      0x90527415,  //  00D3  SETMBR	R20	K58	R21
      0x90527613,  //  00D4  SETMBR	R20	K59	R19
      0x8C540527,  //  00D5  GETMET	R21	R2	K39
      0x7C540200,  //  00D6  CALL	R21	1
      0x8C542B28,  //  00D7  GETMET	R21	R21	K40
      0x885C0B29,  //  00D8  GETMBR	R23	R5	K41
      0x88600716,  //  00D9  GETMBR	R24	R3	K22
      0x8864070E,  //  00DA  GETMBR	R25	R3	K14
      0x00603019,  //  00DB  ADD	R24	R24	R25
      0x60640015,  //  00DC  GETGBL	R25	G21
      0x7C640000,  //  00DD  CALL	R25	0
      0x8C643325,  //  00DE  GETMET	R25	R25	K37
      0x586C003C,  //  00DF  LDCONST	R27	K60
      0x7C640400,  //  00E0  CALL	R25	2
      0x546A002F,  //  00E1  LDINT	R26	48
      0x7C540A00,  //  00E2  CALL	R21	5
      0x545A000E,  //  00E3  LDINT	R22	15
      0x405A0616,  //  00E4  CONNECT	R22	K3	R22
      0x94582A16,  //  00E5  GETIDX	R22	R21	R22
      0x545E000F,  //  00E6  LDINT	R23	16
      0x5462001E,  //  00E7  LDINT	R24	31
      0x405C2E18,  //  00E8  CONNECT	R23	R23	R24
      0x945C2A17,  //  00E9  GETIDX	R23	R21	R23
      0x5462001F,  //  00EA  LDINT	R24	32
      0x5466002E,  //  00EB  LDINT	R25	47
      0x40603019,  //  00EC  CONNECT	R24	R24	R25
      0x94602A18,  //  00ED  GETIDX	R24	R21	R24
      0xB8664400,  //  00EE  GETNGBL	R25	K34
      0x8C64333D,  //  00EF  GETMET	R25	R25	K61
      0x7C640200,  //  00F0  CALL	R25	1
      0x9464333E,  //  00F1  GETIDX	R25	R25	K62
      0xB86A4400,  //  00F2  GETNGBL	R26	K34
      0x8C683523,  //  00F3  GETMET	R26	R26	K35
      0x5870003F,  //  00F4  LDCONST	R28	K63
      0x54760003,  //  00F5  LDINT	R29	4
      0x7C680600,  //  00F6  CALL	R26	3
      0xB86A4400,  //  00F7  GETNGBL	R26	K34
      0x8C683523,  //  00F8  GETMET	R26	R26	K35
      0x8C702D31,  //  00F9  GETMET	R28	R22	K49
      0x7C700200,  //  00FA  CALL	R28	1
      0x0072801C,  //  00FB  ADD	R28	K64	R28
      0x54760003,  //  00FC  LDINT	R29	4
      0x7C680600,  //  00FD  CALL	R26	3
      0xB86A4400,  //  00FE  GETNGBL	R26	K34
      0x8C683523,  //  00FF  GETMET	R26	R26	K35
      0x8C702F31,  //  0100  GETMET	R28	R23	K49
      0x7C700200,  //  0101  CALL	R28	1
      0x0072821C,  //  0102  ADD	R28	K65	R28
      0x54760003,  //  0103  LDINT	R29	4
      0x7C680600,  //  0104  CALL	R26	3
      0xB86A4400,  //  0105  GETNGBL	R26	K34
      0x8C683523,  //  0106  GETMET	R26	R26	K35
      0x8C703131,  //  0107  GETMET	R28	R24	K49
      0x7C700200,  //  0108  CALL	R28	1
      0x0072841C,  //  0109  ADD	R28	K66	R28
      0x54760003,  //  010A  LDINT	R29	4
      0x7C680600,  //  010B  CALL	R26	3
      0xB86A4400,  //  010C  GETNGBL	R26	K34
      0x8C683523,  //  010D  GETMET	R26	R26	K35
      0x5870003F,  //  010E  LDCONST	R28	K63
      0x54760003,  //  010F  LDINT	R29	4
      0x7C680600,  //  0110  CALL	R26	3
      0x8C682943,  //  0111  GETMET	R26	R20	K67
      0x7C680200,  //  0112  CALL	R26	1
      0x4C6C0000,  //  0113  LDNIL	R27
      0x9016881B,  //  0114  SETMBR	R5	K68	R27
      0xB86E4400,  //  0115  GETNGBL	R27	K34
      0x8C6C3723,  //  0116  GETMET	R27	R27	K35
      0x8C743531,  //  0117  GETMET	R29	R26	K49
      0x7C740200,  //  0118  CALL	R29	1
      0x00768A1D,  //  0119  ADD	R29	K69	R29
      0x547A0003,  //  011A  LDINT	R30	4
      0x7C6C0600,  //  011B  CALL	R27	3
      0x8C6C0346,  //  011C  GETMET	R27	R1	K70
      0x54760032,  //  011D  LDINT	R29	51
      0x50780200,  //  011E  LDBOOL	R30	1	0
      0x7C6C0600,  //  011F  CALL	R27	3
      0x8C703743,  //  0120  GETMET	R28	R27	K67
      0x5C783400,  //  0121  MOVE	R30	R26
      0x7C700400,  //  0122  CALL	R28	2
      0x88740147,  //  0123  GETMBR	R29	R0	K71
      0x8C743B48,  //  0124  GETMET	R29	R29	K72
      0x5C7C3800,  //  0125  MOVE	R31	R28
      0x88800349,  //  0126  GETMBR	R32	R1	K73
      0x8884034A,  //  0127  GETMBR	R33	R1	K74
      0x8888374B,  //  0128  GETMBR	R34	R27	K75
      0x7C740A00,  //  0129  CALL	R29	5
      0x8C740B4C,  //  012A  GETMET	R29	R5	K76
      0x5C7C2C00,  //  012B  MOVE	R31	R22
      0x5C802E00,  //  012C  MOVE	R32	R23
      0x5C843000,  //  012D  MOVE	R33	R24
      0x5C883200,  //  012E  MOVE	R34	R25
      0x7C740A00,  //  012F  CALL	R29	5
      0x50740200,  //  0130  LDBOOL	R29	1	0
      0x80043A00,  //  0131  RET	1	R29
      0x70020001,  //  0132  JMP		#0135
      0x4C3C0000,  //  0133  LDNIL	R15
      0x900E1C0F,  //  0134  SETMBR	R3	K14	R15
      0x8818070E,  //  0135  GETMBR	R6	R3	K14
      0x4C1C0000,  //  0136  LDNIL	R7
      0x1C180C07,  //  0137  EQ	R6	R6	R7
      0x741A0003,  //  0138  JMPT	R6	#013D
      0x8818070F,  //  0139  GETMBR	R6	R3	K15
      0x4C1C0000,  //  013A  LDNIL	R7
      0x1C180C07,  //  013B  EQ	R6	R6	R7
      0x781A00F0,  //  013C  JMPF	R6	#022E
      0x8C180536,  //  013D  GETMET	R6	R2	K54
      0x5422000F,  //  013E  LDINT	R8	16
      0x7C180400,  //  013F  CALL	R6	2
      0x90166A06,  //  0140  SETMBR	R5	K53	R6
      0x8C180536,  //  0141  GETMET	R6	R2	K54
      0x5422001F,  //  0142  LDINT	R8	32
      0x7C180400,  //  0143  CALL	R6	2
      0x90029A06,  //  0144  SETMBR	R0	K77	R6
      0x8C18054F,  //  0145  GETMET	R6	R2	K79
      0x7C180200,  //  0146  CALL	R6	1
      0x8C180D50,  //  0147  GETMET	R6	R6	K80
      0x8820014D,  //  0148  GETMBR	R8	R0	K77
      0x7C180400,  //  0149  CALL	R6	2
      0x90029C06,  //  014A  SETMBR	R0	K78	R6
      0x8C180536,  //  014B  GETMET	R6	R2	K54
      0x5422001F,  //  014C  LDINT	R8	32
      0x7C180400,  //  014D  CALL	R6	2
      0x8C1C054F,  //  014E  GETMET	R7	R2	K79
      0x7C1C0200,  //  014F  CALL	R7	1
      0x8C1C0F51,  //  0150  GETMET	R7	R7	K81
      0x8824014D,  //  0151  GETMBR	R9	R0	K77
      0x8828070D,  //  0152  GETMBR	R10	R3	K13
      0x7C1C0600,  //  0153  CALL	R7	3
      0x90165207,  //  0154  SETMBR	R5	K41	R7
      0xB81E0E00,  //  0155  GETNGBL	R7	K7
      0x881C0F52,  //  0156  GETMBR	R7	R7	K82
      0x8C1C0F53,  //  0157  GETMET	R7	R7	K83
      0x7C1C0200,  //  0158  CALL	R7	1
      0x8C200F54,  //  0159  GETMET	R8	R7	K84
      0x58280055,  //  015A  LDCONST	R10	K85
      0xB82E0E00,  //  015B  GETNGBL	R11	K7
      0x882C1752,  //  015C  GETMBR	R11	R11	K82
      0x882C1756,  //  015D  GETMBR	R11	R11	K86
      0x8C300B57,  //  015E  GETMET	R12	R5	K87
      0x7C300200,  //  015F  CALL	R12	1
      0x7C200800,  //  0160  CALL	R8	4
      0x8C200F54,  //  0161  GETMET	R8	R7	K84
      0x58280058,  //  0162  LDCONST	R10	K88
      0xB82E0E00,  //  0163  GETNGBL	R11	K7
      0x882C1752,  //  0164  GETMBR	R11	R11	K82
      0x882C1756,  //  0165  GETMBR	R11	R11	K86
      0x8C300B59,  //  0166  GETMET	R12	R5	K89
      0x7C300200,  //  0167  CALL	R12	1
      0x7C200800,  //  0168  CALL	R8	4
      0x8C200F54,  //  0169  GETMET	R8	R7	K84
      0x5828005A,  //  016A  LDCONST	R10	K90
      0xB82E0E00,  //  016B  GETNGBL	R11	K7
      0x882C1752,  //  016C  GETMBR	R11	R11	K82
      0x882C1756,  //  016D  GETMBR	R11	R11	K86
      0x8830014E,  //  016E  GETMBR	R12	R0	K78
      0x7C200800,  //  016F  CALL	R8	4
      0x8C200F54,  //  0170  GETMET	R8	R7	K84
      0x542A0003,  //  0171  LDINT	R10	4
      0xB82E0E00,  //  0172  GETNGBL	R11	K7
      0x882C1752,  //  0173  GETMBR	R11	R11	K82
      0x882C1756,  //  0174  GETMBR	R11	R11	K86
      0x8830070D,  //  0175  GETMBR	R12	R3	K13
      0x7C200800,  //  0176  CALL	R8	4
      0x8C20054F,  //  0177  GETMET	R8	R2	K79
      0x7C200200,  //  0178  CALL	R8	1
      0x8C20115B,  //  0179  GETMET	R8	R8	K91
      0x8C280B5C,  //  017A  GETMET	R10	R5	K92
      0x7C280200,  //  017B  CALL	R10	1
      0x8C2C0F43,  //  017C  GETMET	R11	R7	K67
      0x7C2C0200,  //  017D  CALL	R11	1
      0x7C200600,  //  017E  CALL	R8	3
      0xB8260E00,  //  017F  GETNGBL	R9	K7
      0x88241352,  //  0180  GETMBR	R9	R9	K82
      0x8C241353,  //  0181  GETMET	R9	R9	K83
      0x7C240200,  //  0182  CALL	R9	1
      0x8C281354,  //  0183  GETMET	R10	R9	K84
      0x58300055,  //  0184  LDCONST	R12	K85
      0xB8360E00,  //  0185  GETNGBL	R13	K7
      0x88341B52,  //  0186  GETMBR	R13	R13	K82
      0x88341B56,  //  0187  GETMBR	R13	R13	K86
      0x8C380B57,  //  0188  GETMET	R14	R5	K87
      0x7C380200,  //  0189  CALL	R14	1
      0x7C280800,  //  018A  CALL	R10	4
      0x8C281354,  //  018B  GETMET	R10	R9	K84
      0x58300058,  //  018C  LDCONST	R12	K88
      0xB8360E00,  //  018D  GETNGBL	R13	K7
      0x88341B52,  //  018E  GETMBR	R13	R13	K82
      0x88341B56,  //  018F  GETMBR	R13	R13	K86
      0x8C380B59,  //  0190  GETMET	R14	R5	K89
      0x7C380200,  //  0191  CALL	R14	1
      0x7C280800,  //  0192  CALL	R10	4
      0x8C281354,  //  0193  GETMET	R10	R9	K84
      0x5830005A,  //  0194  LDCONST	R12	K90
      0xB8360E00,  //  0195  GETNGBL	R13	K7
      0x88341B52,  //  0196  GETMBR	R13	R13	K82
      0x88341B56,  //  0197  GETMBR	R13	R13	K86
      0x5C381000,  //  0198  MOVE	R14	R8
      0x7C280800,  //  0199  CALL	R10	4
      0x8C281354,  //  019A  GETMET	R10	R9	K84
      0x54320003,  //  019B  LDINT	R12	4
      0xB8360E00,  //  019C  GETNGBL	R13	K7
      0x88341B52,  //  019D  GETMBR	R13	R13	K82
      0x88341B56,  //  019E  GETMBR	R13	R13	K86
      0x88380B35,  //  019F  GETMBR	R14	R5	K53
      0x7C280800,  //  01A0  CALL	R10	4
      0xB82A4400,  //  01A1  GETNGBL	R10	K34
      0x8C281523,  //  01A2  GETMET	R10	R10	K35
      0x5830002F,  //  01A3  LDCONST	R12	K47
      0x54360003,  //  01A4  LDINT	R13	4
      0x7C280600,  //  01A5  CALL	R10	3
      0x8828075D,  //  01A6  GETMBR	R10	R3	K93
      0x9016880A,  //  01A7  SETMBR	R5	K68	R10
      0xB82A4400,  //  01A8  GETNGBL	R10	K34
      0x8C281523,  //  01A9  GETMET	R10	R10	K35
      0x88300B44,  //  01AA  GETMBR	R12	R5	K68
      0x8C301931,  //  01AB  GETMET	R12	R12	K49
      0x7C300200,  //  01AC  CALL	R12	1
      0x0032BC0C,  //  01AD  ADD	R12	K94	R12
      0x54360003,  //  01AE  LDINT	R13	4
      0x7C280600,  //  01AF  CALL	R10	3
      0x8C28055F,  //  01B0  GETMET	R10	R2	K95
      0x7C280200,  //  01B1  CALL	R10	1
      0x8C281560,  //  01B2  GETMET	R10	R10	K96
      0x88300B44,  //  01B3  GETMBR	R12	R5	K68
      0x7C280400,  //  01B4  CALL	R10	2
      0x8C281561,  //  01B5  GETMET	R10	R10	K97
      0x7C280200,  //  01B6  CALL	R10	1
      0x602C0015,  //  01B7  GETGBL	R11	G21
      0x7C2C0000,  //  01B8  CALL	R11	0
      0x8C2C1725,  //  01B9  GETMET	R11	R11	K37
      0x88340162,  //  01BA  GETMBR	R13	R0	K98
      0x7C2C0400,  //  01BB  CALL	R11	2
      0x8C300B63,  //  01BC  GETMET	R12	R5	K99
      0x7C300200,  //  01BD  CALL	R12	1
      0x00301806,  //  01BE  ADD	R12	R12	R6
      0x8834014E,  //  01BF  GETMBR	R13	R0	K78
      0x0030180D,  //  01C0  ADD	R12	R12	R13
      0x0030180A,  //  01C1  ADD	R12	R12	R10
      0x8C340527,  //  01C2  GETMET	R13	R2	K39
      0x7C340200,  //  01C3  CALL	R13	1
      0x8C341B28,  //  01C4  GETMET	R13	R13	K40
      0x883C0B29,  //  01C5  GETMBR	R15	R5	K41
      0x5C401800,  //  01C6  MOVE	R16	R12
      0x5C441600,  //  01C7  MOVE	R17	R11
      0x544A000F,  //  01C8  LDINT	R18	16
      0x7C340A00,  //  01C9  CALL	R13	5
      0xB83A4400,  //  01CA  GETNGBL	R14	K34
      0x8C381D23,  //  01CB  GETMET	R14	R14	K35
      0x88400B29,  //  01CC  GETMBR	R16	R5	K41
      0x8C402131,  //  01CD  GETMET	R16	R16	K49
      0x7C400200,  //  01CE  CALL	R16	1
      0x0042C810,  //  01CF  ADD	R16	K100	R16
      0x54460003,  //  01D0  LDINT	R17	4
      0x7C380600,  //  01D1  CALL	R14	3
      0xB83A4400,  //  01D2  GETNGBL	R14	K34
      0x8C381D23,  //  01D3  GETMET	R14	R14	K35
      0x8C401931,  //  01D4  GETMET	R16	R12	K49
      0x7C400200,  //  01D5  CALL	R16	1
      0x0042CA10,  //  01D6  ADD	R16	K101	R16
      0x54460003,  //  01D7  LDINT	R17	4
      0x7C380600,  //  01D8  CALL	R14	3
      0xB83A4400,  //  01D9  GETNGBL	R14	K34
      0x8C381D23,  //  01DA  GETMET	R14	R14	K35
      0x8C401B31,  //  01DB  GETMET	R16	R13	K49
      0x7C400200,  //  01DC  CALL	R16	1
      0x0042CC10,  //  01DD  ADD	R16	K102	R16
      0x54460003,  //  01DE  LDINT	R17	4
      0x7C380600,  //  01DF  CALL	R14	3
      0x8C381343,  //  01E0  GETMET	R14	R9	K67
      0x7C380200,  //  01E1  CALL	R14	1
      0x8C3C052C,  //  01E2  GETMET	R15	R2	K44
      0x5C441A00,  //  01E3  MOVE	R17	R13
      0x60480015,  //  01E4  GETGBL	R18	G21
      0x7C480000,  //  01E5  CALL	R18	0
      0x8C482525,  //  01E6  GETMET	R18	R18	K37
      0x88500167,  //  01E7  GETMBR	R20	R0	K103
      0x7C480400,  //  01E8  CALL	R18	2
      0x604C0015,  //  01E9  GETGBL	R19	G21
      0x7C4C0000,  //  01EA  CALL	R19	0
      0x6050000C,  //  01EB  GETGBL	R20	G12
      0x5C541C00,  //  01EC  MOVE	R21	R14
      0x7C500200,  //  01ED  CALL	R20	1
      0x5456000F,  //  01EE  LDINT	R21	16
      0x7C3C0C00,  //  01EF  CALL	R15	6
      0x8C401F68,  //  01F0  GETMET	R16	R15	K104
      0x5C481C00,  //  01F1  MOVE	R18	R14
      0x7C400400,  //  01F2  CALL	R16	2
      0x8C441F2E,  //  01F3  GETMET	R17	R15	K46
      0x7C440200,  //  01F4  CALL	R17	1
      0x00402011,  //  01F5  ADD	R16	R16	R17
      0xB8464400,  //  01F6  GETNGBL	R17	K34
      0x8C442323,  //  01F7  GETMET	R17	R17	K35
      0x8C4C2131,  //  01F8  GETMET	R19	R16	K49
      0x7C4C0200,  //  01F9  CALL	R19	1
      0x004ED213,  //  01FA  ADD	R19	K105	R19
      0x54520003,  //  01FB  LDINT	R20	4
      0x7C440600,  //  01FC  CALL	R17	3
      0xB8464400,  //  01FD  GETNGBL	R17	K34
      0x8C442323,  //  01FE  GETMET	R17	R17	K35
      0x584C002F,  //  01FF  LDCONST	R19	K47
      0x54520003,  //  0200  LDINT	R20	4
      0x7C440600,  //  0201  CALL	R17	3
      0xB8460E00,  //  0202  GETNGBL	R17	K7
      0x8C44236A,  //  0203  GETMET	R17	R17	K106
      0x7C440200,  //  0204  CALL	R17	1
      0x9046D606,  //  0205  SETMBR	R17	K107	R6
      0x88480121,  //  0206  GETMBR	R18	R0	K33
      0x9046D812,  //  0207  SETMBR	R17	K108	R18
      0x8848014E,  //  0208  GETMBR	R18	R0	K78
      0x9046DA12,  //  0209  SETMBR	R17	K109	R18
      0x9046DC10,  //  020A  SETMBR	R17	K110	R16
      0xB84A4400,  //  020B  GETNGBL	R18	K34
      0x8C482523,  //  020C  GETMET	R18	R18	K35
      0xB8520E00,  //  020D  GETNGBL	R20	K7
      0x8C502970,  //  020E  GETMET	R20	R20	K112
      0x5C582200,  //  020F  MOVE	R22	R17
      0x7C500400,  //  0210  CALL	R20	2
      0x0052DE14,  //  0211  ADD	R20	K111	R20
      0x54560003,  //  0212  LDINT	R21	4
      0x7C480600,  //  0213  CALL	R18	3
      0x8C482343,  //  0214  GETMET	R18	R17	K67
      0x7C480200,  //  0215  CALL	R18	1
      0x9016E212,  //  0216  SETMBR	R5	K113	R18
      0xB84E4400,  //  0217  GETNGBL	R19	K34
      0x8C4C2723,  //  0218  GETMET	R19	R19	K35
      0x8C542531,  //  0219  GETMET	R21	R18	K49
      0x7C540200,  //  021A  CALL	R21	1
      0x0056E415,  //  021B  ADD	R21	K114	R21
      0x545A0003,  //  021C  LDINT	R22	4
      0x7C4C0600,  //  021D  CALL	R19	3
      0x8C4C0346,  //  021E  GETMET	R19	R1	K70
      0x54560030,  //  021F  LDINT	R21	49
      0x50580200,  //  0220  LDBOOL	R22	1	0
      0x7C4C0600,  //  0221  CALL	R19	3
      0x8C502743,  //  0222  GETMET	R20	R19	K67
      0x5C582400,  //  0223  MOVE	R22	R18
      0x7C500400,  //  0224  CALL	R20	2
      0x88540147,  //  0225  GETMBR	R21	R0	K71
      0x8C542B48,  //  0226  GETMET	R21	R21	K72
      0x5C5C2800,  //  0227  MOVE	R23	R20
      0x88600349,  //  0228  GETMBR	R24	R1	K73
      0x8864034A,  //  0229  GETMBR	R25	R1	K74
      0x8868274B,  //  022A  GETMBR	R26	R19	K75
      0x7C540A00,  //  022B  CALL	R21	5
      0x50540200,  //  022C  LDBOOL	R21	1	0
      0x80042A00,  //  022D  RET	1	R21
      0x50180200,  //  022E  LDBOOL	R6	1	0
      0x80040C00,  //  022F  RET	1	R6
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
    ( &(const bvalue[95]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(__Msg1),
    /* K19  */  be_nested_str_weak(__Msg2),
    /* K20  */  be_nested_str_weak(out),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20session_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20session_X2Eipk_epoch_key_X20),
    /* K23  */  be_nested_str_weak(get_ipk_epoch_key),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20session_X2Efabric_compressed_X20),
    /* K25  */  be_nested_str_weak(get_fabric_compressed),
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
    /* K93  */  be_nested_str_weak(persist_to_fabric),
    /* K94  */  be_nested_str_weak(save),
    }),
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[449]) {  /* code */
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
      0x8C240717,  //  002E  GETMET	R9	R3	K23
      0x7C240200,  //  002F  CALL	R9	1
      0x7C200200,  //  0030  CALL	R8	1
      0x00222C08,  //  0031  ADD	R8	K22	R8
      0x54260003,  //  0032  LDINT	R9	4
      0x7C180600,  //  0033  CALL	R6	3
      0xB81A1A00,  //  0034  GETNGBL	R6	K13
      0x8C180D0E,  //  0035  GETMET	R6	R6	K14
      0x60200008,  //  0036  GETGBL	R8	G8
      0x8C240719,  //  0037  GETMET	R9	R3	K25
      0x7C240200,  //  0038  CALL	R9	1
      0x7C200200,  //  0039  CALL	R8	1
      0x00223008,  //  003A  ADD	R8	K24	R8
      0x54260003,  //  003B  LDINT	R9	4
      0x7C180600,  //  003C  CALL	R6	3
      0xB81A1A00,  //  003D  GETNGBL	R6	K13
      0x8C180D0E,  //  003E  GETMET	R6	R6	K14
      0x8C20071B,  //  003F  GETMET	R8	R3	K27
      0x7C200200,  //  0040  CALL	R8	1
      0x8C20111C,  //  0041  GETMET	R8	R8	K28
      0x7C200200,  //  0042  CALL	R8	1
      0x00223408,  //  0043  ADD	R8	K26	R8
      0x54260003,  //  0044  LDINT	R9	4
      0x7C180600,  //  0045  CALL	R6	3
      0xB81A1A00,  //  0046  GETNGBL	R6	K13
      0x8C180D0E,  //  0047  GETMET	R6	R6	K14
      0x8C200B1C,  //  0048  GETMET	R8	R5	K28
      0x7C200200,  //  0049  CALL	R8	1
      0x00223A08,  //  004A  ADD	R8	K29	R8
      0x54260003,  //  004B  LDINT	R9	4
      0x7C180600,  //  004C  CALL	R6	3
      0x60180015,  //  004D  GETGBL	R6	G21
      0x7C180000,  //  004E  CALL	R6	0
      0x8C180D1E,  //  004F  GETMET	R6	R6	K30
      0x8820011F,  //  0050  GETMBR	R8	R0	K31
      0x7C180400,  //  0051  CALL	R6	2
      0x8C1C0520,  //  0052  GETMET	R7	R2	K32
      0x7C1C0200,  //  0053  CALL	R7	1
      0x8C1C0F21,  //  0054  GETMET	R7	R7	K33
      0x88240722,  //  0055  GETMBR	R9	R3	K34
      0x8C28071B,  //  0056  GETMET	R10	R3	K27
      0x7C280200,  //  0057  CALL	R10	1
      0x00281405,  //  0058  ADD	R10	R10	R5
      0x5C2C0C00,  //  0059  MOVE	R11	R6
      0x5432000F,  //  005A  LDINT	R12	16
      0x7C1C0A00,  //  005B  CALL	R7	5
      0xB8221A00,  //  005C  GETNGBL	R8	K13
      0x8C20110E,  //  005D  GETMET	R8	R8	K14
      0x5828000F,  //  005E  LDCONST	R10	K15
      0x542E0003,  //  005F  LDINT	R11	4
      0x7C200600,  //  0060  CALL	R8	3
      0xB8221A00,  //  0061  GETNGBL	R8	K13
      0x8C20110E,  //  0062  GETMET	R8	R8	K14
      0x8C28071B,  //  0063  GETMET	R10	R3	K27
      0x7C280200,  //  0064  CALL	R10	1
      0x00281405,  //  0065  ADD	R10	R10	R5
      0x8C28151C,  //  0066  GETMET	R10	R10	K28
      0x7C280200,  //  0067  CALL	R10	1
      0x002A460A,  //  0068  ADD	R10	K35	R10
      0x542E0003,  //  0069  LDINT	R11	4
      0x7C200600,  //  006A  CALL	R8	3
      0xB8221A00,  //  006B  GETNGBL	R8	K13
      0x8C20110E,  //  006C  GETMET	R8	R8	K14
      0x8C280F1C,  //  006D  GETMET	R10	R7	K28
      0x7C280200,  //  006E  CALL	R10	1
      0x002A480A,  //  006F  ADD	R10	K36	R10
      0x542E0003,  //  0070  LDINT	R11	4
      0x7C200600,  //  0071  CALL	R8	3
      0xB8221A00,  //  0072  GETNGBL	R8	K13
      0x8C20110E,  //  0073  GETMET	R8	R8	K14
      0x5828000F,  //  0074  LDCONST	R10	K15
      0x542E0003,  //  0075  LDINT	R11	4
      0x7C200600,  //  0076  CALL	R8	3
      0x5421FFEE,  //  0077  LDINT	R8	-17
      0x40220608,  //  0078  CONNECT	R8	K3	R8
      0x88240925,  //  0079  GETMBR	R9	R4	K37
      0x94201208,  //  007A  GETIDX	R8	R9	R8
      0x5429FFEF,  //  007B  LDINT	R10	-16
      0x40281526,  //  007C  CONNECT	R10	R10	K38
      0x882C0925,  //  007D  GETMBR	R11	R4	K37
      0x9424160A,  //  007E  GETIDX	R9	R11	R10
      0x8C300527,  //  007F  GETMET	R12	R2	K39
      0x5C380E00,  //  0080  MOVE	R14	R7
      0x603C0015,  //  0081  GETGBL	R15	G21
      0x7C3C0000,  //  0082  CALL	R15	0
      0x8C3C1F1E,  //  0083  GETMET	R15	R15	K30
      0x88440128,  //  0084  GETMBR	R17	R0	K40
      0x7C3C0400,  //  0085  CALL	R15	2
      0x60400015,  //  0086  GETGBL	R16	G21
      0x7C400000,  //  0087  CALL	R16	0
      0x6044000C,  //  0088  GETGBL	R17	G12
      0x5C481000,  //  0089  MOVE	R18	R8
      0x7C440200,  //  008A  CALL	R17	1
      0x544A000F,  //  008B  LDINT	R18	16
      0x7C300C00,  //  008C  CALL	R12	6
      0x5C281800,  //  008D  MOVE	R10	R12
      0x8C301529,  //  008E  GETMET	R12	R10	K41
      0x5C381000,  //  008F  MOVE	R14	R8
      0x7C300400,  //  0090  CALL	R12	2
      0x5C2C1800,  //  0091  MOVE	R11	R12
      0x8C30152A,  //  0092  GETMET	R12	R10	K42
      0x7C300200,  //  0093  CALL	R12	1
      0xB8361A00,  //  0094  GETNGBL	R13	K13
      0x8C341B0E,  //  0095  GETMET	R13	R13	K14
      0x8C3C171C,  //  0096  GETMET	R15	R11	K28
      0x7C3C0200,  //  0097  CALL	R15	1
      0x003E560F,  //  0098  ADD	R15	K43	R15
      0x54420003,  //  0099  LDINT	R16	4
      0x7C340600,  //  009A  CALL	R13	3
      0xB8361A00,  //  009B  GETNGBL	R13	K13
      0x8C341B0E,  //  009C  GETMET	R13	R13	K14
      0x8C3C191C,  //  009D  GETMET	R15	R12	K28
      0x7C3C0200,  //  009E  CALL	R15	1
      0x003E580F,  //  009F  ADD	R15	K44	R15
      0x54420003,  //  00A0  LDINT	R16	4
      0x7C340600,  //  00A1  CALL	R13	3
      0xB8361A00,  //  00A2  GETNGBL	R13	K13
      0x8C341B0E,  //  00A3  GETMET	R13	R13	K14
      0x8C3C131C,  //  00A4  GETMET	R15	R9	K28
      0x7C3C0200,  //  00A5  CALL	R15	1
      0x003E5A0F,  //  00A6  ADD	R15	K45	R15
      0x54420003,  //  00A7  LDINT	R16	4
      0x7C340600,  //  00A8  CALL	R13	3
      0xB8361A00,  //  00A9  GETNGBL	R13	K13
      0x8C341B0E,  //  00AA  GETMET	R13	R13	K14
      0x583C000F,  //  00AB  LDCONST	R15	K15
      0x54420003,  //  00AC  LDINT	R16	4
      0x7C340600,  //  00AD  CALL	R13	3
      0x20341809,  //  00AE  NE	R13	R12	R9
      0x78360000,  //  00AF  JMPF	R13	#00B1
      0xB0065D2F,  //  00B0  RAISE	1	K46	K47
      0xB8361000,  //  00B1  GETNGBL	R13	K8
      0x88341B30,  //  00B2  GETMBR	R13	R13	K48
      0x8C341B0A,  //  00B3  GETMET	R13	R13	K10
      0x5C3C1600,  //  00B4  MOVE	R15	R11
      0x7C340400,  //  00B5  CALL	R13	2
      0x8C381B31,  //  00B6  GETMET	R14	R13	K49
      0x58400032,  //  00B7  LDCONST	R16	K50
      0x7C380400,  //  00B8  CALL	R14	2
      0x8C3C1B31,  //  00B9  GETMET	R15	R13	K49
      0x58440033,  //  00BA  LDCONST	R17	K51
      0x7C3C0400,  //  00BB  CALL	R15	2
      0x8C401B31,  //  00BC  GETMET	R16	R13	K49
      0x58480034,  //  00BD  LDCONST	R18	K52
      0x7C400400,  //  00BE  CALL	R16	2
      0xB8461000,  //  00BF  GETNGBL	R17	K8
      0x88442330,  //  00C0  GETMBR	R17	R17	K48
      0x8C44230A,  //  00C1  GETMET	R17	R17	K10
      0x5C4C1C00,  //  00C2  MOVE	R19	R14
      0x7C440400,  //  00C3  CALL	R17	2
      0xB84A1A00,  //  00C4  GETNGBL	R18	K13
      0x8C48250E,  //  00C5  GETMET	R18	R18	K14
      0x60500008,  //  00C6  GETGBL	R20	G8
      0x5C542200,  //  00C7  MOVE	R21	R17
      0x7C500200,  //  00C8  CALL	R20	1
      0x00526A14,  //  00C9  ADD	R20	K53	R20
      0x58540034,  //  00CA  LDCONST	R21	K52
      0x7C480600,  //  00CB  CALL	R18	3
      0x8C482331,  //  00CC  GETMET	R18	R17	K49
      0x54520008,  //  00CD  LDINT	R20	9
      0x7C480400,  //  00CE  CALL	R18	2
      0x8C4C2336,  //  00CF  GETMET	R19	R17	K54
      0x54560005,  //  00D0  LDINT	R21	6
      0x7C4C0400,  //  00D1  CALL	R19	2
      0x8C502731,  //  00D2  GETMET	R20	R19	K49
      0x545A0010,  //  00D3  LDINT	R22	17
      0x7C500400,  //  00D4  CALL	R20	2
      0x60540004,  //  00D5  GETGBL	R21	G4
      0x5C582800,  //  00D6  MOVE	R22	R20
      0x7C540200,  //  00D7  CALL	R21	1
      0x1C542B37,  //  00D8  EQ	R21	R21	K55
      0x78560003,  //  00D9  JMPF	R21	#00DE
      0xB8567000,  //  00DA  GETNGBL	R21	K56
      0x5C582800,  //  00DB  MOVE	R22	R20
      0x7C540200,  //  00DC  CALL	R21	1
      0x5C502A00,  //  00DD  MOVE	R20	R21
      0x8C54293A,  //  00DE  GETMET	R21	R20	K58
      0x7C540200,  //  00DF  CALL	R21	1
      0x900E7215,  //  00E0  SETMBR	R3	K57	R21
      0xB8561A00,  //  00E1  GETNGBL	R21	K13
      0x8C542B0E,  //  00E2  GETMET	R21	R21	K14
      0x605C0008,  //  00E3  GETGBL	R23	G8
      0x88600739,  //  00E4  GETMBR	R24	R3	K57
      0x7C5C0200,  //  00E5  CALL	R23	1
      0x005E7617,  //  00E6  ADD	R23	K59	R23
      0x58600034,  //  00E7  LDCONST	R24	K52
      0x7C540600,  //  00E8  CALL	R21	3
      0xB8561000,  //  00E9  GETNGBL	R21	K8
      0x88542B30,  //  00EA  GETMBR	R21	R21	K48
      0x8C542B3C,  //  00EB  GETMET	R21	R21	K60
      0x7C540200,  //  00EC  CALL	R21	1
      0x8C582B3D,  //  00ED  GETMET	R22	R21	K61
      0x58600032,  //  00EE  LDCONST	R24	K50
      0xB8661000,  //  00EF  GETNGBL	R25	K8
      0x88643330,  //  00F0  GETMBR	R25	R25	K48
      0x8864333E,  //  00F1  GETMBR	R25	R25	K62
      0x5C681C00,  //  00F2  MOVE	R26	R14
      0x7C580800,  //  00F3  CALL	R22	4
      0x8C582B3D,  //  00F4  GETMET	R22	R21	K61
      0x58600033,  //  00F5  LDCONST	R24	K51
      0xB8661000,  //  00F6  GETNGBL	R25	K8
      0x88643330,  //  00F7  GETMBR	R25	R25	K48
      0x8864333E,  //  00F8  GETMBR	R25	R25	K62
      0x5C681E00,  //  00F9  MOVE	R26	R15
      0x7C580800,  //  00FA  CALL	R22	4
      0x8C582B3D,  //  00FB  GETMET	R22	R21	K61
      0x58600034,  //  00FC  LDCONST	R24	K52
      0xB8661000,  //  00FD  GETNGBL	R25	K8
      0x88643330,  //  00FE  GETMBR	R25	R25	K48
      0x8864333E,  //  00FF  GETMBR	R25	R25	K62
      0x8868013F,  //  0100  GETMBR	R26	R0	K63
      0x7C580800,  //  0101  CALL	R22	4
      0x8C582B3D,  //  0102  GETMET	R22	R21	K61
      0x54620003,  //  0103  LDINT	R24	4
      0xB8661000,  //  0104  GETNGBL	R25	K8
      0x88643330,  //  0105  GETMBR	R25	R25	K48
      0x8864333E,  //  0106  GETMBR	R25	R25	K62
      0x88680140,  //  0107  GETMBR	R26	R0	K64
      0x7C580800,  //  0108  CALL	R22	4
      0x8C582B41,  //  0109  GETMET	R22	R21	K65
      0x7C580200,  //  010A  CALL	R22	1
      0xB85E1A00,  //  010B  GETNGBL	R23	K13
      0x8C5C2F0E,  //  010C  GETMET	R23	R23	K14
      0x8C64251C,  //  010D  GETMET	R25	R18	K28
      0x7C640200,  //  010E  CALL	R25	1
      0x00668419,  //  010F  ADD	R25	K66	R25
      0x546A0003,  //  0110  LDINT	R26	4
      0x7C5C0600,  //  0111  CALL	R23	3
      0xB85E1A00,  //  0112  GETNGBL	R23	K13
      0x8C5C2F0E,  //  0113  GETMET	R23	R23	K14
      0x8C64211C,  //  0114  GETMET	R25	R16	K28
      0x7C640200,  //  0115  CALL	R25	1
      0x00668619,  //  0116  ADD	R25	K67	R25
      0x546A0003,  //  0117  LDINT	R26	4
      0x7C5C0600,  //  0118  CALL	R23	3
      0xB85E1A00,  //  0119  GETNGBL	R23	K13
      0x8C5C2F0E,  //  011A  GETMET	R23	R23	K14
      0x5864000F,  //  011B  LDCONST	R25	K15
      0x546A0003,  //  011C  LDINT	R26	4
      0x7C5C0600,  //  011D  CALL	R23	3
      0x8C5C0544,  //  011E  GETMET	R23	R2	K68
      0x7C5C0200,  //  011F  CALL	R23	1
      0x8C5C2F45,  //  0120  GETMET	R23	R23	K69
      0x5C642400,  //  0121  MOVE	R25	R18
      0x5C682C00,  //  0122  MOVE	R26	R22
      0x5C6C2000,  //  0123  MOVE	R27	R16
      0x7C5C0800,  //  0124  CALL	R23	4
      0x5C602E00,  //  0125  MOVE	R24	R23
      0x74620000,  //  0126  JMPT	R24	#0128
      0xB0065D46,  //  0127  RAISE	1	K46	K70
      0xB8621A00,  //  0128  GETNGBL	R24	K13
      0x8C60310E,  //  0129  GETMET	R24	R24	K14
      0x58680047,  //  012A  LDCONST	R26	K71
      0x586C0034,  //  012B  LDCONST	R27	K52
      0x7C600600,  //  012C  CALL	R24	3
      0x8C600510,  //  012D  GETMET	R24	R2	K16
      0x7C600200,  //  012E  CALL	R24	1
      0x8C603111,  //  012F  GETMET	R24	R24	K17
      0x88680712,  //  0130  GETMBR	R26	R3	K18
      0x7C600400,  //  0131  CALL	R24	2
      0x8C603111,  //  0132  GETMET	R24	R24	K17
      0x88680713,  //  0133  GETMBR	R26	R3	K19
      0x7C600400,  //  0134  CALL	R24	2
      0x8C603111,  //  0135  GETMET	R24	R24	K17
      0x88680948,  //  0136  GETMBR	R26	R4	K72
      0x7C600400,  //  0137  CALL	R24	2
      0x8C603114,  //  0138  GETMET	R24	R24	K20
      0x7C600200,  //  0139  CALL	R24	1
      0x5C143000,  //  013A  MOVE	R5	R24
      0x4C600000,  //  013B  LDNIL	R24
      0x900E2418,  //  013C  SETMBR	R3	K18	R24
      0x4C600000,  //  013D  LDNIL	R24
      0x900E2618,  //  013E  SETMBR	R3	K19	R24
      0xB8621A00,  //  013F  GETNGBL	R24	K13
      0x8C60310E,  //  0140  GETMET	R24	R24	K14
      0x58680049,  //  0141  LDCONST	R26	K73
      0x546E0003,  //  0142  LDINT	R27	4
      0x7C600600,  //  0143  CALL	R24	3
      0xB8621A00,  //  0144  GETNGBL	R24	K13
      0x8C60310E,  //  0145  GETMET	R24	R24	K14
      0x88680722,  //  0146  GETMBR	R26	R3	K34
      0x8C68351C,  //  0147  GETMET	R26	R26	K28
      0x7C680200,  //  0148  CALL	R26	1
      0x006A941A,  //  0149  ADD	R26	K74	R26
      0x546E0003,  //  014A  LDINT	R27	4
      0x7C600600,  //  014B  CALL	R24	3
      0xB8621A00,  //  014C  GETNGBL	R24	K13
      0x8C60310E,  //  014D  GETMET	R24	R24	K14
      0x8C68071B,  //  014E  GETMET	R26	R3	K27
      0x7C680200,  //  014F  CALL	R26	1
      0x00683405,  //  0150  ADD	R26	R26	R5
      0x8C68351C,  //  0151  GETMET	R26	R26	K28
      0x7C680200,  //  0152  CALL	R26	1
      0x006A961A,  //  0153  ADD	R26	K75	R26
      0x546E0003,  //  0154  LDINT	R27	4
      0x7C600600,  //  0155  CALL	R24	3
      0x8C600520,  //  0156  GETMET	R24	R2	K32
      0x7C600200,  //  0157  CALL	R24	1
      0x8C603121,  //  0158  GETMET	R24	R24	K33
      0x88680722,  //  0159  GETMBR	R26	R3	K34
      0x8C6C071B,  //  015A  GETMET	R27	R3	K27
      0x7C6C0200,  //  015B  CALL	R27	1
      0x006C3605,  //  015C  ADD	R27	R27	R5
      0x60700015,  //  015D  GETGBL	R28	G21
      0x7C700000,  //  015E  CALL	R28	0
      0x8C70391E,  //  015F  GETMET	R28	R28	K30
      0x8878014C,  //  0160  GETMBR	R30	R0	K76
      0x7C700400,  //  0161  CALL	R28	2
      0x5476002F,  //  0162  LDINT	R29	48
      0x7C600A00,  //  0163  CALL	R24	5
      0x5466000E,  //  0164  LDINT	R25	15
      0x40660619,  //  0165  CONNECT	R25	K3	R25
      0x94643019,  //  0166  GETIDX	R25	R24	R25
      0x546A000F,  //  0167  LDINT	R26	16
      0x546E001E,  //  0168  LDINT	R27	31
      0x4068341B,  //  0169  CONNECT	R26	R26	R27
      0x9468301A,  //  016A  GETIDX	R26	R24	R26
      0x546E001F,  //  016B  LDINT	R27	32
      0x5472002E,  //  016C  LDINT	R28	47
      0x406C361C,  //  016D  CONNECT	R27	R27	R28
      0x946C301B,  //  016E  GETIDX	R27	R24	R27
      0xB8721A00,  //  016F  GETNGBL	R28	K13
      0x8C70394D,  //  0170  GETMET	R28	R28	K77
      0x7C700200,  //  0171  CALL	R28	1
      0x9470394E,  //  0172  GETIDX	R28	R28	K78
      0xB8761A00,  //  0173  GETNGBL	R29	K13
      0x8C743B0E,  //  0174  GETMET	R29	R29	K14
      0x587C0049,  //  0175  LDCONST	R31	K73
      0x54820003,  //  0176  LDINT	R32	4
      0x7C740600,  //  0177  CALL	R29	3
      0xB8761A00,  //  0178  GETNGBL	R29	K13
      0x8C743B0E,  //  0179  GETMET	R29	R29	K14
      0x8C7C331C,  //  017A  GETMET	R31	R25	K28
      0x7C7C0200,  //  017B  CALL	R31	1
      0x007E9E1F,  //  017C  ADD	R31	K79	R31
      0x54820003,  //  017D  LDINT	R32	4
      0x7C740600,  //  017E  CALL	R29	3
      0xB8761A00,  //  017F  GETNGBL	R29	K13
      0x8C743B0E,  //  0180  GETMET	R29	R29	K14
      0x8C7C351C,  //  0181  GETMET	R31	R26	K28
      0x7C7C0200,  //  0182  CALL	R31	1
      0x007EA01F,  //  0183  ADD	R31	K80	R31
      0x54820003,  //  0184  LDINT	R32	4
      0x7C740600,  //  0185  CALL	R29	3
      0xB8761A00,  //  0186  GETNGBL	R29	K13
      0x8C743B0E,  //  0187  GETMET	R29	R29	K14
      0x8C7C371C,  //  0188  GETMET	R31	R27	K28
      0x7C7C0200,  //  0189  CALL	R31	1
      0x007EA21F,  //  018A  ADD	R31	K81	R31
      0x54820003,  //  018B  LDINT	R32	4
      0x7C740600,  //  018C  CALL	R29	3
      0xB8761A00,  //  018D  GETNGBL	R29	K13
      0x8C743B0E,  //  018E  GETMET	R29	R29	K14
      0x587C0049,  //  018F  LDCONST	R31	K73
      0x54820003,  //  0190  LDINT	R32	4
      0x7C740600,  //  0191  CALL	R29	3
      0x8C740352,  //  0192  GETMET	R29	R1	K82
      0x547E003F,  //  0193  LDINT	R31	64
      0x50800200,  //  0194  LDBOOL	R32	1	0
      0x7C740600,  //  0195  CALL	R29	3
      0x60780015,  //  0196  GETGBL	R30	G21
      0x7C780000,  //  0197  CALL	R30	0
      0x8C7C3D53,  //  0198  GETMET	R31	R30	K83
      0x58840003,  //  0199  LDCONST	R33	K3
      0x58880033,  //  019A  LDCONST	R34	K51
      0x7C7C0600,  //  019B  CALL	R31	3
      0x8C7C3D53,  //  019C  GETMET	R31	R30	K83
      0x58840003,  //  019D  LDCONST	R33	K3
      0x548A0003,  //  019E  LDINT	R34	4
      0x7C7C0600,  //  019F  CALL	R31	3
      0x8C7C3D53,  //  01A0  GETMET	R31	R30	K83
      0x58840003,  //  01A1  LDCONST	R33	K3
      0x548A0003,  //  01A2  LDINT	R34	4
      0x7C7C0600,  //  01A3  CALL	R31	3
      0x8C7C3B41,  //  01A4  GETMET	R31	R29	K65
      0x5C843C00,  //  01A5  MOVE	R33	R30
      0x7C7C0400,  //  01A6  CALL	R31	2
      0x88800154,  //  01A7  GETMBR	R32	R0	K84
      0x8C804155,  //  01A8  GETMET	R32	R32	K85
      0x5C883E00,  //  01A9  MOVE	R34	R31
      0x888C0356,  //  01AA  GETMBR	R35	R1	K86
      0x88900357,  //  01AB  GETMBR	R36	R1	K87
      0x88943B58,  //  01AC  GETMBR	R37	R29	K88
      0x7C800A00,  //  01AD  CALL	R32	5
      0x8C800759,  //  01AE  GETMET	R32	R3	K89
      0x7C800200,  //  01AF  CALL	R32	1
      0x8C80075A,  //  01B0  GETMET	R32	R3	K90
      0x5C883200,  //  01B1  MOVE	R34	R25
      0x5C8C3400,  //  01B2  MOVE	R35	R26
      0x5C903600,  //  01B3  MOVE	R36	R27
      0x5C943800,  //  01B4  MOVE	R37	R28
      0x7C800A00,  //  01B5  CALL	R32	5
      0x8C80075B,  //  01B6  GETMET	R32	R3	K91
      0x50880200,  //  01B7  LDBOOL	R34	1	0
      0x7C800400,  //  01B8  CALL	R32	2
      0x8C80075C,  //  01B9  GETMET	R32	R3	K92
      0x7C800200,  //  01BA  CALL	R32	1
      0x8C80075D,  //  01BB  GETMET	R32	R3	K93
      0x7C800200,  //  01BC  CALL	R32	1
      0x8C80075E,  //  01BD  GETMET	R32	R3	K94
      0x7C800200,  //  01BE  CALL	R32	1
      0x50800200,  //  01BF  LDBOOL	R32	1	0
      0x80044000,  //  01C0  RET	1	R32
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
    /* K13  */  be_nested_str_weak(set_mode_PASE),
    /* K14  */  be_const_int(2147483647),
    /* K15  */  be_nested_str_weak(passcodeId),
    /* K16  */  be_nested_str_weak(non_X2Dzero_X20passcode_X20id),
    /* K17  */  be_nested_str_weak(future_initiator_session_id),
    /* K18  */  be_nested_str_weak(initiator_session_id),
    /* K19  */  be_nested_str_weak(future_local_session_id),
    /* K20  */  be_nested_str_weak(device),
    /* K21  */  be_nested_str_weak(sessions),
    /* K22  */  be_nested_str_weak(gen_local_session_id),
    /* K23  */  be_nested_str_weak(tasmota),
    /* K24  */  be_nested_str_weak(log),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20Loc_session_X3D),
    /* K26  */  be_nested_str_weak(PBKDFParamResponse),
    /* K27  */  be_nested_str_weak(initiatorRandom),
    /* K28  */  be_nested_str_weak(responderRandom),
    /* K29  */  be_nested_str_weak(random),
    /* K30  */  be_nested_str_weak(responderSessionId),
    /* K31  */  be_nested_str_weak(pbkdf_parameters_salt),
    /* K32  */  be_nested_str_weak(salt),
    /* K33  */  be_nested_str_weak(pbkdf_parameters_iterations),
    /* K34  */  be_nested_str_weak(iterations),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20pbkdfparamresp_X3A_X20),
    /* K36  */  be_nested_str_weak(inspect),
    /* K37  */  be_nested_str_weak(encode),
    /* K38  */  be_nested_str_weak(MTR_X3A_X20pbkdfparamresp_raw_X3A_X20),
    /* K39  */  be_nested_str_weak(tohex),
    /* K40  */  be_nested_str_weak(build_response),
    /* K41  */  be_nested_str_weak(responder),
    /* K42  */  be_nested_str_weak(send_response),
    /* K43  */  be_nested_str_weak(remote_ip),
    /* K44  */  be_nested_str_weak(remote_port),
    /* K45  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[98]) {  /* code */
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
      0x7C100200,  //  0015  CALL	R4	1
      0x8810030B,  //  0016  GETMBR	R4	R1	K11
      0x4010090E,  //  0017  CONNECT	R4	R4	K14
      0x8814030A,  //  0018  GETMBR	R5	R1	K10
      0x94100A04,  //  0019  GETIDX	R4	R5	R4
      0x90021004,  //  001A  SETMBR	R0	K8	R4
      0x8810070F,  //  001B  GETMBR	R4	R3	K15
      0x20100903,  //  001C  NE	R4	R4	K3
      0x78120000,  //  001D  JMPF	R4	#001F
      0xB0060B10,  //  001E  RAISE	1	K5	K16
      0x88100712,  //  001F  GETMBR	R4	R3	K18
      0x90022204,  //  0020  SETMBR	R0	K17	R4
      0x88100114,  //  0021  GETMBR	R4	R0	K20
      0x88100915,  //  0022  GETMBR	R4	R4	K21
      0x8C100916,  //  0023  GETMET	R4	R4	K22
      0x7C100200,  //  0024  CALL	R4	1
      0x90022604,  //  0025  SETMBR	R0	K19	R4
      0xB8122E00,  //  0026  GETNGBL	R4	K23
      0x8C100918,  //  0027  GETMET	R4	R4	K24
      0x60180008,  //  0028  GETGBL	R6	G8
      0x881C0113,  //  0029  GETMBR	R7	R0	K19
      0x7C180200,  //  002A  CALL	R6	1
      0x001A3206,  //  002B  ADD	R6	K25	R6
      0x7C100400,  //  002C  CALL	R4	2
      0xB8120E00,  //  002D  GETNGBL	R4	K7
      0x8C10091A,  //  002E  GETMET	R4	R4	K26
      0x7C100200,  //  002F  CALL	R4	1
      0x8814071B,  //  0030  GETMBR	R5	R3	K27
      0x90123605,  //  0031  SETMBR	R4	K27	R5
      0x8C14051D,  //  0032  GETMET	R5	R2	K29
      0x541E001F,  //  0033  LDINT	R7	32
      0x7C140400,  //  0034  CALL	R5	2
      0x90123805,  //  0035  SETMBR	R4	K28	R5
      0x88140113,  //  0036  GETMBR	R5	R0	K19
      0x90123C05,  //  0037  SETMBR	R4	K30	R5
      0x88140114,  //  0038  GETMBR	R5	R0	K20
      0x88140B20,  //  0039  GETMBR	R5	R5	K32
      0x90123E05,  //  003A  SETMBR	R4	K31	R5
      0x88140114,  //  003B  GETMBR	R5	R0	K20
      0x88140B22,  //  003C  GETMBR	R5	R5	K34
      0x90124205,  //  003D  SETMBR	R4	K33	R5
      0xB8162E00,  //  003E  GETNGBL	R5	K23
      0x8C140B18,  //  003F  GETMET	R5	R5	K24
      0x601C0008,  //  0040  GETGBL	R7	G8
      0xB8220E00,  //  0041  GETNGBL	R8	K7
      0x8C201124,  //  0042  GETMET	R8	R8	K36
      0x5C280800,  //  0043  MOVE	R10	R4
      0x7C200400,  //  0044  CALL	R8	2
      0x7C1C0200,  //  0045  CALL	R7	1
      0x001E4607,  //  0046  ADD	R7	K35	R7
      0x54220003,  //  0047  LDINT	R8	4
      0x7C140600,  //  0048  CALL	R5	3
      0x8C140925,  //  0049  GETMET	R5	R4	K37
      0x7C140200,  //  004A  CALL	R5	1
      0xB81A2E00,  //  004B  GETNGBL	R6	K23
      0x8C180D18,  //  004C  GETMET	R6	R6	K24
      0x8C200B27,  //  004D  GETMET	R8	R5	K39
      0x7C200200,  //  004E  CALL	R8	1
      0x00224C08,  //  004F  ADD	R8	K38	R8
      0x54260003,  //  0050  LDINT	R9	4
      0x7C180600,  //  0051  CALL	R6	3
      0x90023405,  //  0052  SETMBR	R0	K26	R5
      0x8C180328,  //  0053  GETMET	R6	R1	K40
      0x54220020,  //  0054  LDINT	R8	33
      0x50240200,  //  0055  LDBOOL	R9	1	0
      0x7C180600,  //  0056  CALL	R6	3
      0x8C1C0D25,  //  0057  GETMET	R7	R6	K37
      0x5C240A00,  //  0058  MOVE	R9	R5
      0x7C1C0400,  //  0059  CALL	R7	2
      0x88200129,  //  005A  GETMBR	R8	R0	K41
      0x8C20112A,  //  005B  GETMET	R8	R8	K42
      0x5C280E00,  //  005C  MOVE	R10	R7
      0x882C032B,  //  005D  GETMBR	R11	R1	K43
      0x8830032C,  //  005E  GETMBR	R12	R1	K44
      0x88340D2D,  //  005F  GETMBR	R13	R6	K45
      0x7C200A00,  //  0060  CALL	R8	5
      0x80000000,  //  0061  RET	0
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
** Solidified class: Matter_Commisioning_Context
********************************************************************/
be_local_class(Matter_Commisioning_Context,
    20,
    NULL,
    be_nested_map(35,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(parse_Pake3, -1), be_const_closure(Matter_Commisioning_Context_parse_Pake3_closure) },
        { be_const_key_weak(I2RKey, 9), be_const_var(17) },
        { be_const_key_weak(AttestationChallenge, -1), be_const_var(19) },
        { be_const_key_weak(parse_Pake1, -1), be_const_closure(Matter_Commisioning_Context_parse_Pake1_closure) },
        { be_const_key_weak(PBKDFParamRequest, -1), be_const_var(5) },
        { be_const_key_weak(Matter_Context_Prefix, 14), be_nested_str_weak(CHIP_X20PAKE_X20V1_X20Commissioning) },
        { be_const_key_weak(device, 34), be_const_var(1) },
        { be_const_key_weak(S3K_Info, -1), be_nested_str_weak(Sigma3) },
        { be_const_key_weak(R2IKey, 21), be_const_var(18) },
        { be_const_key_weak(S2K_Info, 27), be_nested_str_weak(Sigma2) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Commisioning_Context_every_second_closure) },
        { be_const_key_weak(future_local_session_id, -1), be_const_var(4) },
        { be_const_key_weak(process_incoming, 29), be_const_closure(Matter_Commisioning_Context_process_incoming_closure) },
        { be_const_key_weak(pB, -1), be_const_var(9) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Commisioning_Context_init_closure) },
        { be_const_key_weak(ResponderEph_pub, -1), be_const_var(14) },
        { be_const_key_weak(cA, -1), be_const_var(10) },
        { be_const_key_weak(future_initiator_session_id, 16), be_const_var(3) },
        { be_const_key_weak(responder, -1), be_const_var(0) },
        { be_const_key_weak(parse_Sigma1, 24), be_const_closure(Matter_Commisioning_Context_parse_Sigma1_closure) },
        { be_const_key_weak(PBKDFParamResponse, -1), be_const_var(6) },
        { be_const_key_weak(Ke, 26), be_const_var(12) },
        { be_const_key_weak(parse_Sigma3, -1), be_const_closure(Matter_Commisioning_Context_parse_Sigma3_closure) },
        { be_const_key_weak(parse_PBKDFParamRequest, -1), be_const_closure(Matter_Commisioning_Context_parse_PBKDFParamRequest_closure) },
        { be_const_key_weak(created, -1), be_const_var(16) },
        { be_const_key_weak(spake, -1), be_const_var(2) },
        { be_const_key_weak(find_fabric_by_destination_id, -1), be_const_closure(Matter_Commisioning_Context_find_fabric_by_destination_id_closure) },
        { be_const_key_weak(TBEData3_Nonce, -1), be_nested_str_weak(NCASE_Sigma3N) },
        { be_const_key_weak(ResponderEph_priv, -1), be_const_var(13) },
        { be_const_key_weak(TBEData2_Nonce, -1), be_nested_str_weak(NCASE_Sigma2N) },
        { be_const_key_weak(cB, -1), be_const_var(11) },
        { be_const_key_weak(SEKeys_Info, -1), be_nested_str_weak(SessionKeys) },
        { be_const_key_weak(pA, 1), be_const_var(8) },
        { be_const_key_weak(initiatorEph_pub, 4), be_const_var(15) },
        { be_const_key_weak(y, -1), be_const_var(7) },
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
