/* Solidification of Matter_Commissioning.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Commisioning_Context;

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
    ( &(const bvalue[88]) {     /* constants */
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
    /* K64  */  be_nested_str_weak(encode),
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
    /* K84  */  be_nested_str_weak(set_persist),
    /* K85  */  be_nested_str_weak(set_no_expiration),
    /* K86  */  be_nested_str_weak(persist_to_fabric),
    /* K87  */  be_nested_str_weak(save),
    }),
    be_str_weak(parse_Sigma3),
    &be_const_str_solidified,
    ( &(const binstruction[478]) {  /* code */
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
      0x50800200,  //  01D4  LDBOOL	R32	1	0
      0x7C780400,  //  01D5  CALL	R30	2
      0x8C780755,  //  01D6  GETMET	R30	R3	K85
      0x7C780200,  //  01D7  CALL	R30	1
      0x8C780756,  //  01D8  GETMET	R30	R3	K86
      0x7C780200,  //  01D9  CALL	R30	1
      0x8C780757,  //  01DA  GETMET	R30	R3	K87
      0x7C780200,  //  01DB  CALL	R30	1
      0x50780200,  //  01DC  LDBOOL	R30	1	0
      0x80043C00,  //  01DD  RET	1	R30
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
    ( &(const bvalue[42]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(MTR_X3A_X20received_X20cA_X3D),
    /* K19  */  be_nested_str_weak(tohex),
    /* K20  */  be_nested_str_weak(spake),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20invalid_X20cA_X20received),
    /* K22  */  be_nested_str_weak(created),
    /* K23  */  be_nested_str_weak(rtc),
    /* K24  */  be_nested_str_weak(utc),
    /* K25  */  be_nested_str_weak(HKDF_SHA256),
    /* K26  */  be_nested_str_weak(derive),
    /* K27  */  be_nested_str_weak(Ke),
    /* K28  */  be_nested_str_weak(fromstring),
    /* K29  */  be_nested_str_weak(SEKeys_Info),
    /* K30  */  be_nested_str_weak(I2RKey),
    /* K31  */  be_nested_str_weak(R2IKey),
    /* K32  */  be_nested_str_weak(AttestationChallenge),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20session_keys_X3D),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K36  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K38  */  be_nested_str_weak(responder),
    /* K39  */  be_nested_str_weak(add_session),
    /* K40  */  be_nested_str_weak(future_local_session_id),
    /* K41  */  be_nested_str_weak(future_initiator_session_id),
    }),
    be_str_weak(parse_Pake3),
    &be_const_str_solidified,
    ( &(const binstruction[161]) {  /* code */
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
      0xB8120A00,  //  0027  GETNGBL	R4	K5
      0x8C100906,  //  0028  GETMET	R4	R4	K6
      0x88180111,  //  0029  GETMBR	R6	R0	K17
      0x8C180D13,  //  002A  GETMET	R6	R6	K19
      0x7C180200,  //  002B  CALL	R6	1
      0x001A2406,  //  002C  ADD	R6	K18	R6
      0x541E0003,  //  002D  LDINT	R7	4
      0x7C100600,  //  002E  CALL	R4	3
      0x88100111,  //  002F  GETMBR	R4	R0	K17
      0x88140114,  //  0030  GETMBR	R5	R0	K20
      0x88140B11,  //  0031  GETMBR	R5	R5	K17
      0x20100805,  //  0032  NE	R4	R4	R5
      0x78120012,  //  0033  JMPF	R4	#0047
      0xB8120A00,  //  0034  GETNGBL	R4	K5
      0x8C100906,  //  0035  GETMET	R4	R4	K6
      0x58180015,  //  0036  LDCONST	R6	K21
      0x581C0008,  //  0037  LDCONST	R7	K8
      0x7C100600,  //  0038  CALL	R4	3
      0xB8120A00,  //  0039  GETNGBL	R4	K5
      0x8C100906,  //  003A  GETMET	R4	R4	K6
      0x58180009,  //  003B  LDCONST	R6	K9
      0x581C0008,  //  003C  LDCONST	R7	K8
      0x7C100600,  //  003D  CALL	R4	3
      0x8C10010A,  //  003E  GETMET	R4	R0	K10
      0x5C180200,  //  003F  MOVE	R6	R1
      0x581C000B,  //  0040  LDCONST	R7	K11
      0x58200003,  //  0041  LDCONST	R8	K3
      0x58240008,  //  0042  LDCONST	R9	K8
      0x50280000,  //  0043  LDBOOL	R10	0	0
      0x7C100C00,  //  0044  CALL	R4	6
      0x50140000,  //  0045  LDBOOL	R5	0	0
      0x80040A00,  //  0046  RET	1	R5
      0xB8120A00,  //  0047  GETNGBL	R4	K5
      0x8C100917,  //  0048  GETMET	R4	R4	K23
      0x7C100200,  //  0049  CALL	R4	1
      0x94100918,  //  004A  GETIDX	R4	R4	K24
      0x90022C04,  //  004B  SETMBR	R0	K22	R4
      0x8C100519,  //  004C  GETMET	R4	R2	K25
      0x7C100200,  //  004D  CALL	R4	1
      0x8C10091A,  //  004E  GETMET	R4	R4	K26
      0x8818011B,  //  004F  GETMBR	R6	R0	K27
      0x601C0015,  //  0050  GETGBL	R7	G21
      0x7C1C0000,  //  0051  CALL	R7	0
      0x60200015,  //  0052  GETGBL	R8	G21
      0x7C200000,  //  0053  CALL	R8	0
      0x8C20111C,  //  0054  GETMET	R8	R8	K28
      0x8828011D,  //  0055  GETMBR	R10	R0	K29
      0x7C200400,  //  0056  CALL	R8	2
      0x5426002F,  //  0057  LDINT	R9	48
      0x7C100A00,  //  0058  CALL	R4	5
      0x5416000E,  //  0059  LDINT	R5	15
      0x40160605,  //  005A  CONNECT	R5	K3	R5
      0x94140805,  //  005B  GETIDX	R5	R4	R5
      0x90023C05,  //  005C  SETMBR	R0	K30	R5
      0x5416000F,  //  005D  LDINT	R5	16
      0x541A001E,  //  005E  LDINT	R6	31
      0x40140A06,  //  005F  CONNECT	R5	R5	R6
      0x94140805,  //  0060  GETIDX	R5	R4	R5
      0x90023E05,  //  0061  SETMBR	R0	K31	R5
      0x5416001F,  //  0062  LDINT	R5	32
      0x541A002E,  //  0063  LDINT	R6	47
      0x40140A06,  //  0064  CONNECT	R5	R5	R6
      0x94140805,  //  0065  GETIDX	R5	R4	R5
      0x90024005,  //  0066  SETMBR	R0	K32	R5
      0xB8160A00,  //  0067  GETNGBL	R5	K5
      0x8C140B06,  //  0068  GETMET	R5	R5	K6
      0x581C0021,  //  0069  LDCONST	R7	K33
      0x54220003,  //  006A  LDINT	R8	4
      0x7C140600,  //  006B  CALL	R5	3
      0xB8160A00,  //  006C  GETNGBL	R5	K5
      0x8C140B06,  //  006D  GETMET	R5	R5	K6
      0x8C1C0913,  //  006E  GETMET	R7	R4	K19
      0x7C1C0200,  //  006F  CALL	R7	1
      0x001E4407,  //  0070  ADD	R7	K34	R7
      0x54220003,  //  0071  LDINT	R8	4
      0x7C140600,  //  0072  CALL	R5	3
      0xB8160A00,  //  0073  GETNGBL	R5	K5
      0x8C140B06,  //  0074  GETMET	R5	R5	K6
      0x881C011E,  //  0075  GETMBR	R7	R0	K30
      0x8C1C0F13,  //  0076  GETMET	R7	R7	K19
      0x7C1C0200,  //  0077  CALL	R7	1
      0x001E4607,  //  0078  ADD	R7	K35	R7
      0x54220003,  //  0079  LDINT	R8	4
      0x7C140600,  //  007A  CALL	R5	3
      0xB8160A00,  //  007B  GETNGBL	R5	K5
      0x8C140B06,  //  007C  GETMET	R5	R5	K6
      0x881C011F,  //  007D  GETMBR	R7	R0	K31
      0x8C1C0F13,  //  007E  GETMET	R7	R7	K19
      0x7C1C0200,  //  007F  CALL	R7	1
      0x001E4807,  //  0080  ADD	R7	K36	R7
      0x54220003,  //  0081  LDINT	R8	4
      0x7C140600,  //  0082  CALL	R5	3
      0xB8160A00,  //  0083  GETNGBL	R5	K5
      0x8C140B06,  //  0084  GETMET	R5	R5	K6
      0x881C0120,  //  0085  GETMBR	R7	R0	K32
      0x8C1C0F13,  //  0086  GETMET	R7	R7	K19
      0x7C1C0200,  //  0087  CALL	R7	1
      0x001E4A07,  //  0088  ADD	R7	K37	R7
      0x54220003,  //  0089  LDINT	R8	4
      0x7C140600,  //  008A  CALL	R5	3
      0xB8160A00,  //  008B  GETNGBL	R5	K5
      0x8C140B06,  //  008C  GETMET	R5	R5	K6
      0x581C0021,  //  008D  LDCONST	R7	K33
      0x54220003,  //  008E  LDINT	R8	4
      0x7C140600,  //  008F  CALL	R5	3
      0x8C14010A,  //  0090  GETMET	R5	R0	K10
      0x5C1C0200,  //  0091  MOVE	R7	R1
      0x58200003,  //  0092  LDCONST	R8	K3
      0x58240003,  //  0093  LDCONST	R9	K3
      0x58280003,  //  0094  LDCONST	R10	K3
      0x502C0000,  //  0095  LDBOOL	R11	0	0
      0x7C140C00,  //  0096  CALL	R5	6
      0x88180126,  //  0097  GETMBR	R6	R0	K38
      0x8C180D27,  //  0098  GETMET	R6	R6	K39
      0x88200128,  //  0099  GETMBR	R8	R0	K40
      0x88240129,  //  009A  GETMBR	R9	R0	K41
      0x8828011E,  //  009B  GETMBR	R10	R0	K30
      0x882C011F,  //  009C  GETMBR	R11	R0	K31
      0x88300120,  //  009D  GETMBR	R12	R0	K32
      0x88340116,  //  009E  GETMBR	R13	R0	K22
      0x7C180E00,  //  009F  CALL	R6	7
      0x80000000,  //  00A0  RET	0
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
    ( &(const bvalue[50]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20invalid_X20PBKDFParamRequest_X20message),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K10  */  be_nested_str_weak(send_status_report),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(matter),
    /* K13  */  be_nested_str_weak(PBKDFParamRequest),
    /* K14  */  be_nested_str_weak(parse),
    /* K15  */  be_nested_str_weak(raw),
    /* K16  */  be_nested_str_weak(app_payload_idx),
    /* K17  */  be_nested_str_weak(session),
    /* K18  */  be_nested_str_weak(set_mode_PASE),
    /* K19  */  be_const_int(2147483647),
    /* K20  */  be_nested_str_weak(passcodeId),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20non_X2Dzero_X20passcode_X20id),
    /* K22  */  be_nested_str_weak(future_initiator_session_id),
    /* K23  */  be_nested_str_weak(initiator_session_id),
    /* K24  */  be_nested_str_weak(future_local_session_id),
    /* K25  */  be_nested_str_weak(device),
    /* K26  */  be_nested_str_weak(sessions),
    /* K27  */  be_nested_str_weak(gen_local_session_id),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20Loc_session_X3D),
    /* K29  */  be_nested_str_weak(PBKDFParamResponse),
    /* K30  */  be_nested_str_weak(initiatorRandom),
    /* K31  */  be_nested_str_weak(responderRandom),
    /* K32  */  be_nested_str_weak(random),
    /* K33  */  be_nested_str_weak(responderSessionId),
    /* K34  */  be_nested_str_weak(pbkdf_parameters_salt),
    /* K35  */  be_nested_str_weak(commissioning_salt),
    /* K36  */  be_nested_str_weak(pbkdf_parameters_iterations),
    /* K37  */  be_nested_str_weak(commissioning_iterations),
    /* K38  */  be_nested_str_weak(MTR_X3A_X20pbkdfparamresp_X3A_X20),
    /* K39  */  be_nested_str_weak(inspect),
    /* K40  */  be_nested_str_weak(encode),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20pbkdfparamresp_raw_X3A_X20),
    /* K42  */  be_nested_str_weak(tohex),
    /* K43  */  be_nested_str_weak(build_response),
    /* K44  */  be_nested_str_weak(encode_frame),
    /* K45  */  be_nested_str_weak(responder),
    /* K46  */  be_nested_str_weak(send_response),
    /* K47  */  be_nested_str_weak(remote_ip),
    /* K48  */  be_nested_str_weak(remote_port),
    /* K49  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(parse_PBKDFParamRequest),
    &be_const_str_solidified,
    ( &(const binstruction[134]) {  /* code */
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
      0x88100311,  //  0025  GETMBR	R4	R1	K17
      0x8C100912,  //  0026  GETMET	R4	R4	K18
      0x7C100200,  //  0027  CALL	R4	1
      0x88100310,  //  0028  GETMBR	R4	R1	K16
      0x40100913,  //  0029  CONNECT	R4	R4	K19
      0x8814030F,  //  002A  GETMBR	R5	R1	K15
      0x94100A04,  //  002B  GETIDX	R4	R5	R4
      0x90021A04,  //  002C  SETMBR	R0	K13	R4
      0x88100714,  //  002D  GETMBR	R4	R3	K20
      0x20100903,  //  002E  NE	R4	R4	K3
      0x78120012,  //  002F  JMPF	R4	#0043
      0xB8120A00,  //  0030  GETNGBL	R4	K5
      0x8C100906,  //  0031  GETMET	R4	R4	K6
      0x58180015,  //  0032  LDCONST	R6	K21
      0x581C0008,  //  0033  LDCONST	R7	K8
      0x7C100600,  //  0034  CALL	R4	3
      0xB8120A00,  //  0035  GETNGBL	R4	K5
      0x8C100906,  //  0036  GETMET	R4	R4	K6
      0x58180009,  //  0037  LDCONST	R6	K9
      0x581C0008,  //  0038  LDCONST	R7	K8
      0x7C100600,  //  0039  CALL	R4	3
      0x8C10010A,  //  003A  GETMET	R4	R0	K10
      0x5C180200,  //  003B  MOVE	R6	R1
      0x581C000B,  //  003C  LDCONST	R7	K11
      0x58200003,  //  003D  LDCONST	R8	K3
      0x58240008,  //  003E  LDCONST	R9	K8
      0x50280000,  //  003F  LDBOOL	R10	0	0
      0x7C100C00,  //  0040  CALL	R4	6
      0x50140000,  //  0041  LDBOOL	R5	0	0
      0x80040A00,  //  0042  RET	1	R5
      0x88100717,  //  0043  GETMBR	R4	R3	K23
      0x90022C04,  //  0044  SETMBR	R0	K22	R4
      0x88100119,  //  0045  GETMBR	R4	R0	K25
      0x8810091A,  //  0046  GETMBR	R4	R4	K26
      0x8C10091B,  //  0047  GETMET	R4	R4	K27
      0x7C100200,  //  0048  CALL	R4	1
      0x90023004,  //  0049  SETMBR	R0	K24	R4
      0xB8120A00,  //  004A  GETNGBL	R4	K5
      0x8C100906,  //  004B  GETMET	R4	R4	K6
      0x60180008,  //  004C  GETGBL	R6	G8
      0x881C0118,  //  004D  GETMBR	R7	R0	K24
      0x7C180200,  //  004E  CALL	R6	1
      0x001A3806,  //  004F  ADD	R6	K28	R6
      0x7C100400,  //  0050  CALL	R4	2
      0xB8121800,  //  0051  GETNGBL	R4	K12
      0x8C10091D,  //  0052  GETMET	R4	R4	K29
      0x7C100200,  //  0053  CALL	R4	1
      0x8814071E,  //  0054  GETMBR	R5	R3	K30
      0x90123C05,  //  0055  SETMBR	R4	K30	R5
      0x8C140520,  //  0056  GETMET	R5	R2	K32
      0x541E001F,  //  0057  LDINT	R7	32
      0x7C140400,  //  0058  CALL	R5	2
      0x90123E05,  //  0059  SETMBR	R4	K31	R5
      0x88140118,  //  005A  GETMBR	R5	R0	K24
      0x90124205,  //  005B  SETMBR	R4	K33	R5
      0x88140119,  //  005C  GETMBR	R5	R0	K25
      0x88140B23,  //  005D  GETMBR	R5	R5	K35
      0x90124405,  //  005E  SETMBR	R4	K34	R5
      0x88140119,  //  005F  GETMBR	R5	R0	K25
      0x88140B25,  //  0060  GETMBR	R5	R5	K37
      0x90124805,  //  0061  SETMBR	R4	K36	R5
      0xB8160A00,  //  0062  GETNGBL	R5	K5
      0x8C140B06,  //  0063  GETMET	R5	R5	K6
      0x601C0008,  //  0064  GETGBL	R7	G8
      0xB8221800,  //  0065  GETNGBL	R8	K12
      0x8C201127,  //  0066  GETMET	R8	R8	K39
      0x5C280800,  //  0067  MOVE	R10	R4
      0x7C200400,  //  0068  CALL	R8	2
      0x7C1C0200,  //  0069  CALL	R7	1
      0x001E4C07,  //  006A  ADD	R7	K38	R7
      0x54220003,  //  006B  LDINT	R8	4
      0x7C140600,  //  006C  CALL	R5	3
      0x8C140928,  //  006D  GETMET	R5	R4	K40
      0x7C140200,  //  006E  CALL	R5	1
      0xB81A0A00,  //  006F  GETNGBL	R6	K5
      0x8C180D06,  //  0070  GETMET	R6	R6	K6
      0x8C200B2A,  //  0071  GETMET	R8	R5	K42
      0x7C200200,  //  0072  CALL	R8	1
      0x00225208,  //  0073  ADD	R8	K41	R8
      0x54260003,  //  0074  LDINT	R9	4
      0x7C180600,  //  0075  CALL	R6	3
      0x90023A05,  //  0076  SETMBR	R0	K29	R5
      0x8C18032B,  //  0077  GETMET	R6	R1	K43
      0x54220020,  //  0078  LDINT	R8	33
      0x50240200,  //  0079  LDBOOL	R9	1	0
      0x7C180600,  //  007A  CALL	R6	3
      0x8C1C0D2C,  //  007B  GETMET	R7	R6	K44
      0x5C240A00,  //  007C  MOVE	R9	R5
      0x7C1C0400,  //  007D  CALL	R7	2
      0x8820012D,  //  007E  GETMBR	R8	R0	K45
      0x8C20112E,  //  007F  GETMET	R8	R8	K46
      0x5C280E00,  //  0080  MOVE	R10	R7
      0x882C032F,  //  0081  GETMBR	R11	R1	K47
      0x88300330,  //  0082  GETMBR	R12	R1	K48
      0x88340D31,  //  0083  GETMBR	R13	R6	K49
      0x7C200A00,  //  0084  CALL	R8	5
      0x80000000,  //  0085  RET	0
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
    ( &(const bvalue[117]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(opcode),
    /* K2   */  be_nested_str_weak(local_session_id),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(protocol_id),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20invalid_X20Sigma1_X20message),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28General_X20Code_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20INVALID_PARAMETER_X29),
    /* K10  */  be_nested_str_weak(send_status_report),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(matter),
    /* K13  */  be_nested_str_weak(Sigma1),
    /* K14  */  be_nested_str_weak(parse),
    /* K15  */  be_nested_str_weak(raw),
    /* K16  */  be_nested_str_weak(app_payload_idx),
    /* K17  */  be_nested_str_weak(initiatorEph_pub),
    /* K18  */  be_nested_str_weak(initiatorEphPubKey),
    /* K19  */  be_nested_str_weak(resumptionID),
    /* K20  */  be_nested_str_weak(initiatorResumeMIC),
    /* K21  */  be_nested_str_weak(session),
    /* K22  */  be_nested_str_weak(device),
    /* K23  */  be_nested_str_weak(sessions),
    /* K24  */  be_nested_str_weak(find_session_by_resumption_id),
    /* K25  */  be_nested_str_weak(find_fabric_by_destination_id),
    /* K26  */  be_nested_str_weak(destinationId),
    /* K27  */  be_nested_str_weak(initiatorRandom),
    /* K28  */  be_nested_str_weak(_fabric),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20StatusReport_X28GeneralCode_X3A_X20FAILURE_X2C_X20ProtocolId_X3A_X20SECURE_CHANNEL_X2C_X20ProtocolCode_X3A_X20NO_SHARED_TRUST_ROOTS_X29),
    /* K30  */  be_nested_str_weak(_source_node_id),
    /* K31  */  be_nested_str_weak(source_node_id),
    /* K32  */  be_nested_str_weak(set_mode_CASE),
    /* K33  */  be_nested_str_weak(remove_session),
    /* K34  */  be_nested_str_weak(__future_initiator_session_id),
    /* K35  */  be_nested_str_weak(initiator_session_id),
    /* K36  */  be_nested_str_weak(__future_local_session_id),
    /* K37  */  be_nested_str_weak(gen_local_session_id),
    /* K38  */  be_nested_str_weak(future_local_session_id),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20Loc_session_X3D),
    /* K40  */  be_nested_str_weak(fromstring),
    /* K41  */  be_nested_str_weak(Sigma1_Resume),
    /* K42  */  be_nested_str_weak(HKDF_SHA256),
    /* K43  */  be_nested_str_weak(derive),
    /* K44  */  be_nested_str_weak(shared_secret),
    /* K45  */  be_nested_str_weak(NCASE_SigmaR1),
    /* K46  */  be_const_int(2147483647),
    /* K47  */  be_nested_str_weak(AES_CCM),
    /* K48  */  be_nested_str_weak(decrypt),
    /* K49  */  be_nested_str_weak(tag),
    /* K50  */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K51  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s1rk_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K52  */  be_nested_str_weak(tohex),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20tag_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20Resume1MICPayload_X20_X3D_X20),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20decrypted_tag_X20_X20_X20_X20_X20_X3D_X20),
    /* K56  */  be_nested_str_weak(resumption_id),
    /* K57  */  be_nested_str_weak(random),
    /* K58  */  be_nested_str_weak(Sigma2_Resume),
    /* K59  */  be_nested_str_weak(NCASE_SigmaR2),
    /* K60  */  be_nested_str_weak(Sigma2Resume),
    /* K61  */  be_nested_str_weak(responderSessionID),
    /* K62  */  be_nested_str_weak(sigma2ResumeMIC),
    /* K63  */  be_nested_str_weak(SessionResumptionKeys),
    /* K64  */  be_nested_str_weak(rtc),
    /* K65  */  be_nested_str_weak(utc),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K67  */  be_nested_str_weak(MTR_X3A_X20I2RKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20R2IKey_X20_X20_X20_X20_X20_X20_X3D),
    /* K69  */  be_nested_str_weak(MTR_X3A_X20AC_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K70  */  be_nested_str_weak(encode),
    /* K71  */  be_nested_str_weak(__Msg1),
    /* K72  */  be_nested_str_weak(MTR_X3A_X20sigma2resume_raw_X3A_X20),
    /* K73  */  be_nested_str_weak(build_response),
    /* K74  */  be_nested_str_weak(encode_frame),
    /* K75  */  be_nested_str_weak(responder),
    /* K76  */  be_nested_str_weak(send_response),
    /* K77  */  be_nested_str_weak(remote_ip),
    /* K78  */  be_nested_str_weak(remote_port),
    /* K79  */  be_nested_str_weak(message_counter),
    /* K80  */  be_nested_str_weak(set_keys),
    /* K81  */  be_nested_str_weak(ResponderEph_priv),
    /* K82  */  be_nested_str_weak(ResponderEph_pub),
    /* K83  */  be_nested_str_weak(EC_P256),
    /* K84  */  be_nested_str_weak(public_key),
    /* K85  */  be_nested_str_weak(shared_key),
    /* K86  */  be_nested_str_weak(TLV),
    /* K87  */  be_nested_str_weak(Matter_TLV_struct),
    /* K88  */  be_nested_str_weak(add_TLV),
    /* K89  */  be_nested_str_weak(B2),
    /* K90  */  be_nested_str_weak(get_noc),
    /* K91  */  be_nested_str_weak(get_icac),
    /* K92  */  be_const_int(3),
    /* K93  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K94  */  be_nested_str_weak(get_pk),
    /* K95  */  be_nested_str_weak(Msg1),
    /* K96  */  be_nested_str_weak(MTR_X3A_X20_X2A_X20MSG1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K97  */  be_nested_str_weak(SHA256),
    /* K98  */  be_nested_str_weak(update),
    /* K99  */  be_nested_str_weak(out),
    /* K100 */  be_nested_str_weak(S2K_Info),
    /* K101 */  be_nested_str_weak(get_ipk_group_key),
    /* K102 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20SharedSecret_X20_X20_X3D_X20),
    /* K103 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_salt_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K104 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20s2k_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K105 */  be_nested_str_weak(TBEData2_Nonce),
    /* K106 */  be_nested_str_weak(encrypt),
    /* K107 */  be_nested_str_weak(MTR_X3A_X20_X2A_X20TBEData2Enc_X20_X20_X20_X3D_X20),
    /* K108 */  be_nested_str_weak(Sigma2),
    /* K109 */  be_nested_str_weak(responderRandom),
    /* K110 */  be_nested_str_weak(responderSessionId),
    /* K111 */  be_nested_str_weak(responderEphPubKey),
    /* K112 */  be_nested_str_weak(encrypted2),
    /* K113 */  be_nested_str_weak(MTR_X3A_X20sigma2_X3A_X20),
    /* K114 */  be_nested_str_weak(inspect),
    /* K115 */  be_nested_str_weak(__Msg2),
    /* K116 */  be_nested_str_weak(MTR_X3A_X20sigma2_raw_X3A_X20),
    }),
    be_str_weak(parse_Sigma1),
    &be_const_str_solidified,
    ( &(const binstruction[591]) {  /* code */
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
      0x88100712,  //  0025  GETMBR	R4	R3	K18
      0x90022204,  //  0026  SETMBR	R0	K17	R4
      0x88100713,  //  0027  GETMBR	R4	R3	K19
      0x4C140000,  //  0028  LDNIL	R5
      0x20100805,  //  0029  NE	R4	R4	R5
      0x78120003,  //  002A  JMPF	R4	#002F
      0x88100714,  //  002B  GETMBR	R4	R3	K20
      0x4C140000,  //  002C  LDNIL	R5
      0x20100805,  //  002D  NE	R4	R4	R5
      0x74120000,  //  002E  JMPT	R4	#0030
      0x50100001,  //  002F  LDBOOL	R4	0	1
      0x50100200,  //  0030  LDBOOL	R4	1	0
      0x88140315,  //  0031  GETMBR	R5	R1	K21
      0x78120006,  //  0032  JMPF	R4	#003A
      0x88180116,  //  0033  GETMBR	R6	R0	K22
      0x88180D17,  //  0034  GETMBR	R6	R6	K23
      0x8C180D18,  //  0035  GETMET	R6	R6	K24
      0x88200713,  //  0036  GETMBR	R8	R3	K19
      0x7C180400,  //  0037  CALL	R6	2
      0x5C140C00,  //  0038  MOVE	R5	R6
      0x70020004,  //  0039  JMP		#003F
      0x8C180119,  //  003A  GETMET	R6	R0	K25
      0x8820071A,  //  003B  GETMBR	R8	R3	K26
      0x8824071B,  //  003C  GETMBR	R9	R3	K27
      0x7C180600,  //  003D  CALL	R6	3
      0x90163806,  //  003E  SETMBR	R5	K28	R6
      0x4C180000,  //  003F  LDNIL	R6
      0x1C180A06,  //  0040  EQ	R6	R5	R6
      0x741A0003,  //  0041  JMPT	R6	#0046
      0x88180B1C,  //  0042  GETMBR	R6	R5	K28
      0x4C1C0000,  //  0043  LDNIL	R7
      0x1C180C07,  //  0044  EQ	R6	R6	R7
      0x781A000D,  //  0045  JMPF	R6	#0054
      0xB81A0A00,  //  0046  GETNGBL	R6	K5
      0x8C180D06,  //  0047  GETMET	R6	R6	K6
      0x5820001D,  //  0048  LDCONST	R8	K29
      0x58240008,  //  0049  LDCONST	R9	K8
      0x7C180600,  //  004A  CALL	R6	3
      0x8C18010A,  //  004B  GETMET	R6	R0	K10
      0x5C200200,  //  004C  MOVE	R8	R1
      0x5824000B,  //  004D  LDCONST	R9	K11
      0x58280003,  //  004E  LDCONST	R10	K3
      0x582C000B,  //  004F  LDCONST	R11	K11
      0x50300000,  //  0050  LDBOOL	R12	0	0
      0x7C180C00,  //  0051  CALL	R6	6
      0x501C0000,  //  0052  LDBOOL	R7	0	0
      0x80040E00,  //  0053  RET	1	R7
      0x8818031F,  //  0054  GETMBR	R6	R1	K31
      0x90163C06,  //  0055  SETMBR	R5	K30	R6
      0x8C180B20,  //  0056  GETMET	R6	R5	K32
      0x7C180200,  //  0057  CALL	R6	1
      0x88180315,  //  0058  GETMBR	R6	R1	K21
      0x20180C05,  //  0059  NE	R6	R6	R5
      0x781A0004,  //  005A  JMPF	R6	#0060
      0x88180116,  //  005B  GETMBR	R6	R0	K22
      0x88180D17,  //  005C  GETMBR	R6	R6	K23
      0x8C180D21,  //  005D  GETMET	R6	R6	K33
      0x88200315,  //  005E  GETMBR	R8	R1	K21
      0x7C180400,  //  005F  CALL	R6	2
      0x90062A05,  //  0060  SETMBR	R1	K21	R5
      0x88180723,  //  0061  GETMBR	R6	R3	K35
      0x90164406,  //  0062  SETMBR	R5	K34	R6
      0x88180116,  //  0063  GETMBR	R6	R0	K22
      0x88180D17,  //  0064  GETMBR	R6	R6	K23
      0x8C180D25,  //  0065  GETMET	R6	R6	K37
      0x7C180200,  //  0066  CALL	R6	1
      0x90164806,  //  0067  SETMBR	R5	K36	R6
      0x88180B24,  //  0068  GETMBR	R6	R5	K36
      0x90024C06,  //  0069  SETMBR	R0	K38	R6
      0xB81A0A00,  //  006A  GETNGBL	R6	K5
      0x8C180D06,  //  006B  GETMET	R6	R6	K6
      0x60200008,  //  006C  GETGBL	R8	G8
      0x88240126,  //  006D  GETMBR	R9	R0	K38
      0x7C200200,  //  006E  CALL	R8	1
      0x00224E08,  //  006F  ADD	R8	K39	R8
      0x7C180400,  //  0070  CALL	R6	2
      0x781200E1,  //  0071  JMPF	R4	#0154
      0x8818071B,  //  0072  GETMBR	R6	R3	K27
      0x881C0713,  //  0073  GETMBR	R7	R3	K19
      0x00180C07,  //  0074  ADD	R6	R6	R7
      0x601C0015,  //  0075  GETGBL	R7	G21
      0x7C1C0000,  //  0076  CALL	R7	0
      0x8C1C0F28,  //  0077  GETMET	R7	R7	K40
      0x58240029,  //  0078  LDCONST	R9	K41
      0x7C1C0400,  //  0079  CALL	R7	2
      0x8C20052A,  //  007A  GETMET	R8	R2	K42
      0x7C200200,  //  007B  CALL	R8	1
      0x8C20112B,  //  007C  GETMET	R8	R8	K43
      0x88280B2C,  //  007D  GETMBR	R10	R5	K44
      0x5C2C0C00,  //  007E  MOVE	R11	R6
      0x5C300E00,  //  007F  MOVE	R12	R7
      0x5436000F,  //  0080  LDINT	R13	16
      0x7C200A00,  //  0081  CALL	R8	5
      0x60240015,  //  0082  GETGBL	R9	G21
      0x7C240000,  //  0083  CALL	R9	0
      0x8C241328,  //  0084  GETMET	R9	R9	K40
      0x582C002D,  //  0085  LDCONST	R11	K45
      0x7C240400,  //  0086  CALL	R9	2
      0x5429FFEE,  //  0087  LDINT	R10	-17
      0x402A060A,  //  0088  CONNECT	R10	K3	R10
      0x882C0714,  //  0089  GETMBR	R11	R3	K20
      0x9428160A,  //  008A  GETIDX	R10	R11	R10
      0x5431FFEF,  //  008B  LDINT	R12	-16
      0x4030192E,  //  008C  CONNECT	R12	R12	K46
      0x88340714,  //  008D  GETMBR	R13	R3	K20
      0x942C1A0C,  //  008E  GETIDX	R11	R13	R12
      0x8C38052F,  //  008F  GETMET	R14	R2	K47
      0x5C401000,  //  0090  MOVE	R16	R8
      0x5C441200,  //  0091  MOVE	R17	R9
      0x60480015,  //  0092  GETGBL	R18	G21
      0x7C480000,  //  0093  CALL	R18	0
      0x604C000C,  //  0094  GETGBL	R19	G12
      0x5C501400,  //  0095  MOVE	R20	R10
      0x7C4C0200,  //  0096  CALL	R19	1
      0x5452000F,  //  0097  LDINT	R20	16
      0x7C380C00,  //  0098  CALL	R14	6
      0x5C301C00,  //  0099  MOVE	R12	R14
      0x8C381930,  //  009A  GETMET	R14	R12	K48
      0x5C401400,  //  009B  MOVE	R16	R10
      0x7C380400,  //  009C  CALL	R14	2
      0x5C341C00,  //  009D  MOVE	R13	R14
      0x8C381931,  //  009E  GETMET	R14	R12	K49
      0x7C380200,  //  009F  CALL	R14	1
      0xB83E0A00,  //  00A0  GETNGBL	R15	K5
      0x8C3C1F06,  //  00A1  GETMET	R15	R15	K6
      0x58440032,  //  00A2  LDCONST	R17	K50
      0x544A0003,  //  00A3  LDINT	R18	4
      0x7C3C0600,  //  00A4  CALL	R15	3
      0xB83E0A00,  //  00A5  GETNGBL	R15	K5
      0x8C3C1F06,  //  00A6  GETMET	R15	R15	K6
      0x8C441134,  //  00A7  GETMET	R17	R8	K52
      0x7C440200,  //  00A8  CALL	R17	1
      0x00466611,  //  00A9  ADD	R17	K51	R17
      0x544A0003,  //  00AA  LDINT	R18	4
      0x7C3C0600,  //  00AB  CALL	R15	3
      0xB83E0A00,  //  00AC  GETNGBL	R15	K5
      0x8C3C1F06,  //  00AD  GETMET	R15	R15	K6
      0x8C441734,  //  00AE  GETMET	R17	R11	K52
      0x7C440200,  //  00AF  CALL	R17	1
      0x00466A11,  //  00B0  ADD	R17	K53	R17
      0x544A0003,  //  00B1  LDINT	R18	4
      0x7C3C0600,  //  00B2  CALL	R15	3
      0xB83E0A00,  //  00B3  GETNGBL	R15	K5
      0x8C3C1F06,  //  00B4  GETMET	R15	R15	K6
      0x8C441B34,  //  00B5  GETMET	R17	R13	K52
      0x7C440200,  //  00B6  CALL	R17	1
      0x00466C11,  //  00B7  ADD	R17	K54	R17
      0x544A0003,  //  00B8  LDINT	R18	4
      0x7C3C0600,  //  00B9  CALL	R15	3
      0xB83E0A00,  //  00BA  GETNGBL	R15	K5
      0x8C3C1F06,  //  00BB  GETMET	R15	R15	K6
      0x8C441D34,  //  00BC  GETMET	R17	R14	K52
      0x7C440200,  //  00BD  CALL	R17	1
      0x00466E11,  //  00BE  ADD	R17	K55	R17
      0x544A0003,  //  00BF  LDINT	R18	4
      0x7C3C0600,  //  00C0  CALL	R15	3
      0xB83E0A00,  //  00C1  GETNGBL	R15	K5
      0x8C3C1F06,  //  00C2  GETMET	R15	R15	K6
      0x58440032,  //  00C3  LDCONST	R17	K50
      0x544A0003,  //  00C4  LDINT	R18	4
      0x7C3C0600,  //  00C5  CALL	R15	3
      0x1C3C160E,  //  00C6  EQ	R15	R11	R14
      0x783E0089,  //  00C7  JMPF	R15	#0152
      0x8C3C0539,  //  00C8  GETMET	R15	R2	K57
      0x5446000F,  //  00C9  LDINT	R17	16
      0x7C3C0400,  //  00CA  CALL	R15	2
      0x9016700F,  //  00CB  SETMBR	R5	K56	R15
      0x603C0015,  //  00CC  GETGBL	R15	G21
      0x7C3C0000,  //  00CD  CALL	R15	0
      0x8C3C1F28,  //  00CE  GETMET	R15	R15	K40
      0x5844003A,  //  00CF  LDCONST	R17	K58
      0x7C3C0400,  //  00D0  CALL	R15	2
      0x88400B38,  //  00D1  GETMBR	R16	R5	K56
      0x003C1E10,  //  00D2  ADD	R15	R15	R16
      0x8840071B,  //  00D3  GETMBR	R16	R3	K27
      0x88440713,  //  00D4  GETMBR	R17	R3	K19
      0x00402011,  //  00D5  ADD	R16	R16	R17
      0x8C44052A,  //  00D6  GETMET	R17	R2	K42
      0x7C440200,  //  00D7  CALL	R17	1
      0x8C44232B,  //  00D8  GETMET	R17	R17	K43
      0x884C0B2C,  //  00D9  GETMBR	R19	R5	K44
      0x5C502000,  //  00DA  MOVE	R20	R16
      0x5C541E00,  //  00DB  MOVE	R21	R15
      0x545A000F,  //  00DC  LDINT	R22	16
      0x7C440A00,  //  00DD  CALL	R17	5
      0x8C48052F,  //  00DE  GETMET	R18	R2	K47
      0x5C502200,  //  00DF  MOVE	R20	R17
      0x60540015,  //  00E0  GETGBL	R21	G21
      0x7C540000,  //  00E1  CALL	R21	0
      0x8C542B28,  //  00E2  GETMET	R21	R21	K40
      0x585C003B,  //  00E3  LDCONST	R23	K59
      0x7C540400,  //  00E4  CALL	R21	2
      0x60580015,  //  00E5  GETGBL	R22	G21
      0x7C580000,  //  00E6  CALL	R22	0
      0x585C0003,  //  00E7  LDCONST	R23	K3
      0x5462000F,  //  00E8  LDINT	R24	16
      0x7C480C00,  //  00E9  CALL	R18	6
      0x8C4C2531,  //  00EA  GETMET	R19	R18	K49
      0x7C4C0200,  //  00EB  CALL	R19	1
      0xB8521800,  //  00EC  GETNGBL	R20	K12
      0x8C50293C,  //  00ED  GETMET	R20	R20	K60
      0x7C500200,  //  00EE  CALL	R20	1
      0x88540B38,  //  00EF  GETMBR	R21	R5	K56
      0x90522615,  //  00F0  SETMBR	R20	K19	R21
      0x88540B24,  //  00F1  GETMBR	R21	R5	K36
      0x90527A15,  //  00F2  SETMBR	R20	K61	R21
      0x90527C13,  //  00F3  SETMBR	R20	K62	R19
      0x8C54052A,  //  00F4  GETMET	R21	R2	K42
      0x7C540200,  //  00F5  CALL	R21	1
      0x8C542B2B,  //  00F6  GETMET	R21	R21	K43
      0x885C0B2C,  //  00F7  GETMBR	R23	R5	K44
      0x8860071B,  //  00F8  GETMBR	R24	R3	K27
      0x88640713,  //  00F9  GETMBR	R25	R3	K19
      0x00603019,  //  00FA  ADD	R24	R24	R25
      0x60640015,  //  00FB  GETGBL	R25	G21
      0x7C640000,  //  00FC  CALL	R25	0
      0x8C643328,  //  00FD  GETMET	R25	R25	K40
      0x586C003F,  //  00FE  LDCONST	R27	K63
      0x7C640400,  //  00FF  CALL	R25	2
      0x546A002F,  //  0100  LDINT	R26	48
      0x7C540A00,  //  0101  CALL	R21	5
      0x545A000E,  //  0102  LDINT	R22	15
      0x405A0616,  //  0103  CONNECT	R22	K3	R22
      0x94582A16,  //  0104  GETIDX	R22	R21	R22
      0x545E000F,  //  0105  LDINT	R23	16
      0x5462001E,  //  0106  LDINT	R24	31
      0x405C2E18,  //  0107  CONNECT	R23	R23	R24
      0x945C2A17,  //  0108  GETIDX	R23	R21	R23
      0x5462001F,  //  0109  LDINT	R24	32
      0x5466002E,  //  010A  LDINT	R25	47
      0x40603019,  //  010B  CONNECT	R24	R24	R25
      0x94602A18,  //  010C  GETIDX	R24	R21	R24
      0xB8660A00,  //  010D  GETNGBL	R25	K5
      0x8C643340,  //  010E  GETMET	R25	R25	K64
      0x7C640200,  //  010F  CALL	R25	1
      0x94643341,  //  0110  GETIDX	R25	R25	K65
      0xB86A0A00,  //  0111  GETNGBL	R26	K5
      0x8C683506,  //  0112  GETMET	R26	R26	K6
      0x58700042,  //  0113  LDCONST	R28	K66
      0x54760003,  //  0114  LDINT	R29	4
      0x7C680600,  //  0115  CALL	R26	3
      0xB86A0A00,  //  0116  GETNGBL	R26	K5
      0x8C683506,  //  0117  GETMET	R26	R26	K6
      0x8C702D34,  //  0118  GETMET	R28	R22	K52
      0x7C700200,  //  0119  CALL	R28	1
      0x0072861C,  //  011A  ADD	R28	K67	R28
      0x54760003,  //  011B  LDINT	R29	4
      0x7C680600,  //  011C  CALL	R26	3
      0xB86A0A00,  //  011D  GETNGBL	R26	K5
      0x8C683506,  //  011E  GETMET	R26	R26	K6
      0x8C702F34,  //  011F  GETMET	R28	R23	K52
      0x7C700200,  //  0120  CALL	R28	1
      0x0072881C,  //  0121  ADD	R28	K68	R28
      0x54760003,  //  0122  LDINT	R29	4
      0x7C680600,  //  0123  CALL	R26	3
      0xB86A0A00,  //  0124  GETNGBL	R26	K5
      0x8C683506,  //  0125  GETMET	R26	R26	K6
      0x8C703134,  //  0126  GETMET	R28	R24	K52
      0x7C700200,  //  0127  CALL	R28	1
      0x00728A1C,  //  0128  ADD	R28	K69	R28
      0x54760003,  //  0129  LDINT	R29	4
      0x7C680600,  //  012A  CALL	R26	3
      0xB86A0A00,  //  012B  GETNGBL	R26	K5
      0x8C683506,  //  012C  GETMET	R26	R26	K6
      0x58700042,  //  012D  LDCONST	R28	K66
      0x54760003,  //  012E  LDINT	R29	4
      0x7C680600,  //  012F  CALL	R26	3
      0x8C682946,  //  0130  GETMET	R26	R20	K70
      0x7C680200,  //  0131  CALL	R26	1
      0x4C6C0000,  //  0132  LDNIL	R27
      0x90168E1B,  //  0133  SETMBR	R5	K71	R27
      0xB86E0A00,  //  0134  GETNGBL	R27	K5
      0x8C6C3706,  //  0135  GETMET	R27	R27	K6
      0x8C743534,  //  0136  GETMET	R29	R26	K52
      0x7C740200,  //  0137  CALL	R29	1
      0x0076901D,  //  0138  ADD	R29	K72	R29
      0x547A0003,  //  0139  LDINT	R30	4
      0x7C6C0600,  //  013A  CALL	R27	3
      0x8C6C0349,  //  013B  GETMET	R27	R1	K73
      0x54760032,  //  013C  LDINT	R29	51
      0x50780200,  //  013D  LDBOOL	R30	1	0
      0x7C6C0600,  //  013E  CALL	R27	3
      0x8C70374A,  //  013F  GETMET	R28	R27	K74
      0x5C783400,  //  0140  MOVE	R30	R26
      0x7C700400,  //  0141  CALL	R28	2
      0x8874014B,  //  0142  GETMBR	R29	R0	K75
      0x8C743B4C,  //  0143  GETMET	R29	R29	K76
      0x5C7C3800,  //  0144  MOVE	R31	R28
      0x8880034D,  //  0145  GETMBR	R32	R1	K77
      0x8884034E,  //  0146  GETMBR	R33	R1	K78
      0x8888374F,  //  0147  GETMBR	R34	R27	K79
      0x7C740A00,  //  0148  CALL	R29	5
      0x8C740B50,  //  0149  GETMET	R29	R5	K80
      0x5C7C2C00,  //  014A  MOVE	R31	R22
      0x5C802E00,  //  014B  MOVE	R32	R23
      0x5C843000,  //  014C  MOVE	R33	R24
      0x5C883200,  //  014D  MOVE	R34	R25
      0x7C740A00,  //  014E  CALL	R29	5
      0x50740200,  //  014F  LDBOOL	R29	1	0
      0x80043A00,  //  0150  RET	1	R29
      0x70020001,  //  0151  JMP		#0154
      0x4C3C0000,  //  0152  LDNIL	R15
      0x900E260F,  //  0153  SETMBR	R3	K19	R15
      0x88180713,  //  0154  GETMBR	R6	R3	K19
      0x4C1C0000,  //  0155  LDNIL	R7
      0x1C180C07,  //  0156  EQ	R6	R6	R7
      0x741A0003,  //  0157  JMPT	R6	#015C
      0x88180714,  //  0158  GETMBR	R6	R3	K20
      0x4C1C0000,  //  0159  LDNIL	R7
      0x1C180C07,  //  015A  EQ	R6	R6	R7
      0x781A00F0,  //  015B  JMPF	R6	#024D
      0x8C180539,  //  015C  GETMET	R6	R2	K57
      0x5422000F,  //  015D  LDINT	R8	16
      0x7C180400,  //  015E  CALL	R6	2
      0x90167006,  //  015F  SETMBR	R5	K56	R6
      0x8C180539,  //  0160  GETMET	R6	R2	K57
      0x5422001F,  //  0161  LDINT	R8	32
      0x7C180400,  //  0162  CALL	R6	2
      0x9002A206,  //  0163  SETMBR	R0	K81	R6
      0x8C180553,  //  0164  GETMET	R6	R2	K83
      0x7C180200,  //  0165  CALL	R6	1
      0x8C180D54,  //  0166  GETMET	R6	R6	K84
      0x88200151,  //  0167  GETMBR	R8	R0	K81
      0x7C180400,  //  0168  CALL	R6	2
      0x9002A406,  //  0169  SETMBR	R0	K82	R6
      0x8C180539,  //  016A  GETMET	R6	R2	K57
      0x5422001F,  //  016B  LDINT	R8	32
      0x7C180400,  //  016C  CALL	R6	2
      0x8C1C0553,  //  016D  GETMET	R7	R2	K83
      0x7C1C0200,  //  016E  CALL	R7	1
      0x8C1C0F55,  //  016F  GETMET	R7	R7	K85
      0x88240151,  //  0170  GETMBR	R9	R0	K81
      0x88280712,  //  0171  GETMBR	R10	R3	K18
      0x7C1C0600,  //  0172  CALL	R7	3
      0x90165807,  //  0173  SETMBR	R5	K44	R7
      0xB81E1800,  //  0174  GETNGBL	R7	K12
      0x881C0F56,  //  0175  GETMBR	R7	R7	K86
      0x8C1C0F57,  //  0176  GETMET	R7	R7	K87
      0x7C1C0200,  //  0177  CALL	R7	1
      0x8C200F58,  //  0178  GETMET	R8	R7	K88
      0x5828000B,  //  0179  LDCONST	R10	K11
      0xB82E1800,  //  017A  GETNGBL	R11	K12
      0x882C1756,  //  017B  GETMBR	R11	R11	K86
      0x882C1759,  //  017C  GETMBR	R11	R11	K89
      0x8C300B5A,  //  017D  GETMET	R12	R5	K90
      0x7C300200,  //  017E  CALL	R12	1
      0x7C200800,  //  017F  CALL	R8	4
      0x8C200F58,  //  0180  GETMET	R8	R7	K88
      0x58280008,  //  0181  LDCONST	R10	K8
      0xB82E1800,  //  0182  GETNGBL	R11	K12
      0x882C1756,  //  0183  GETMBR	R11	R11	K86
      0x882C1759,  //  0184  GETMBR	R11	R11	K89
      0x8C300B5B,  //  0185  GETMET	R12	R5	K91
      0x7C300200,  //  0186  CALL	R12	1
      0x7C200800,  //  0187  CALL	R8	4
      0x8C200F58,  //  0188  GETMET	R8	R7	K88
      0x5828005C,  //  0189  LDCONST	R10	K92
      0xB82E1800,  //  018A  GETNGBL	R11	K12
      0x882C1756,  //  018B  GETMBR	R11	R11	K86
      0x882C1759,  //  018C  GETMBR	R11	R11	K89
      0x88300152,  //  018D  GETMBR	R12	R0	K82
      0x7C200800,  //  018E  CALL	R8	4
      0x8C200F58,  //  018F  GETMET	R8	R7	K88
      0x542A0003,  //  0190  LDINT	R10	4
      0xB82E1800,  //  0191  GETNGBL	R11	K12
      0x882C1756,  //  0192  GETMBR	R11	R11	K86
      0x882C1759,  //  0193  GETMBR	R11	R11	K89
      0x88300712,  //  0194  GETMBR	R12	R3	K18
      0x7C200800,  //  0195  CALL	R8	4
      0x8C200553,  //  0196  GETMET	R8	R2	K83
      0x7C200200,  //  0197  CALL	R8	1
      0x8C20115D,  //  0198  GETMET	R8	R8	K93
      0x8C280B5E,  //  0199  GETMET	R10	R5	K94
      0x7C280200,  //  019A  CALL	R10	1
      0x8C2C0F46,  //  019B  GETMET	R11	R7	K70
      0x7C2C0200,  //  019C  CALL	R11	1
      0x7C200600,  //  019D  CALL	R8	3
      0xB8261800,  //  019E  GETNGBL	R9	K12
      0x88241356,  //  019F  GETMBR	R9	R9	K86
      0x8C241357,  //  01A0  GETMET	R9	R9	K87
      0x7C240200,  //  01A1  CALL	R9	1
      0x8C281358,  //  01A2  GETMET	R10	R9	K88
      0x5830000B,  //  01A3  LDCONST	R12	K11
      0xB8361800,  //  01A4  GETNGBL	R13	K12
      0x88341B56,  //  01A5  GETMBR	R13	R13	K86
      0x88341B59,  //  01A6  GETMBR	R13	R13	K89
      0x8C380B5A,  //  01A7  GETMET	R14	R5	K90
      0x7C380200,  //  01A8  CALL	R14	1
      0x7C280800,  //  01A9  CALL	R10	4
      0x8C281358,  //  01AA  GETMET	R10	R9	K88
      0x58300008,  //  01AB  LDCONST	R12	K8
      0xB8361800,  //  01AC  GETNGBL	R13	K12
      0x88341B56,  //  01AD  GETMBR	R13	R13	K86
      0x88341B59,  //  01AE  GETMBR	R13	R13	K89
      0x8C380B5B,  //  01AF  GETMET	R14	R5	K91
      0x7C380200,  //  01B0  CALL	R14	1
      0x7C280800,  //  01B1  CALL	R10	4
      0x8C281358,  //  01B2  GETMET	R10	R9	K88
      0x5830005C,  //  01B3  LDCONST	R12	K92
      0xB8361800,  //  01B4  GETNGBL	R13	K12
      0x88341B56,  //  01B5  GETMBR	R13	R13	K86
      0x88341B59,  //  01B6  GETMBR	R13	R13	K89
      0x5C381000,  //  01B7  MOVE	R14	R8
      0x7C280800,  //  01B8  CALL	R10	4
      0x8C281358,  //  01B9  GETMET	R10	R9	K88
      0x54320003,  //  01BA  LDINT	R12	4
      0xB8361800,  //  01BB  GETNGBL	R13	K12
      0x88341B56,  //  01BC  GETMBR	R13	R13	K86
      0x88341B59,  //  01BD  GETMBR	R13	R13	K89
      0x88380B38,  //  01BE  GETMBR	R14	R5	K56
      0x7C280800,  //  01BF  CALL	R10	4
      0xB82A0A00,  //  01C0  GETNGBL	R10	K5
      0x8C281506,  //  01C1  GETMET	R10	R10	K6
      0x58300032,  //  01C2  LDCONST	R12	K50
      0x54360003,  //  01C3  LDINT	R13	4
      0x7C280600,  //  01C4  CALL	R10	3
      0x8828075F,  //  01C5  GETMBR	R10	R3	K95
      0x90168E0A,  //  01C6  SETMBR	R5	K71	R10
      0xB82A0A00,  //  01C7  GETNGBL	R10	K5
      0x8C281506,  //  01C8  GETMET	R10	R10	K6
      0x88300B47,  //  01C9  GETMBR	R12	R5	K71
      0x8C301934,  //  01CA  GETMET	R12	R12	K52
      0x7C300200,  //  01CB  CALL	R12	1
      0x0032C00C,  //  01CC  ADD	R12	K96	R12
      0x54360003,  //  01CD  LDINT	R13	4
      0x7C280600,  //  01CE  CALL	R10	3
      0x8C280561,  //  01CF  GETMET	R10	R2	K97
      0x7C280200,  //  01D0  CALL	R10	1
      0x8C281562,  //  01D1  GETMET	R10	R10	K98
      0x88300B47,  //  01D2  GETMBR	R12	R5	K71
      0x7C280400,  //  01D3  CALL	R10	2
      0x8C281563,  //  01D4  GETMET	R10	R10	K99
      0x7C280200,  //  01D5  CALL	R10	1
      0x602C0015,  //  01D6  GETGBL	R11	G21
      0x7C2C0000,  //  01D7  CALL	R11	0
      0x8C2C1728,  //  01D8  GETMET	R11	R11	K40
      0x88340164,  //  01D9  GETMBR	R13	R0	K100
      0x7C2C0400,  //  01DA  CALL	R11	2
      0x8C300B65,  //  01DB  GETMET	R12	R5	K101
      0x7C300200,  //  01DC  CALL	R12	1
      0x00301806,  //  01DD  ADD	R12	R12	R6
      0x88340152,  //  01DE  GETMBR	R13	R0	K82
      0x0030180D,  //  01DF  ADD	R12	R12	R13
      0x0030180A,  //  01E0  ADD	R12	R12	R10
      0x8C34052A,  //  01E1  GETMET	R13	R2	K42
      0x7C340200,  //  01E2  CALL	R13	1
      0x8C341B2B,  //  01E3  GETMET	R13	R13	K43
      0x883C0B2C,  //  01E4  GETMBR	R15	R5	K44
      0x5C401800,  //  01E5  MOVE	R16	R12
      0x5C441600,  //  01E6  MOVE	R17	R11
      0x544A000F,  //  01E7  LDINT	R18	16
      0x7C340A00,  //  01E8  CALL	R13	5
      0xB83A0A00,  //  01E9  GETNGBL	R14	K5
      0x8C381D06,  //  01EA  GETMET	R14	R14	K6
      0x88400B2C,  //  01EB  GETMBR	R16	R5	K44
      0x8C402134,  //  01EC  GETMET	R16	R16	K52
      0x7C400200,  //  01ED  CALL	R16	1
      0x0042CC10,  //  01EE  ADD	R16	K102	R16
      0x54460003,  //  01EF  LDINT	R17	4
      0x7C380600,  //  01F0  CALL	R14	3
      0xB83A0A00,  //  01F1  GETNGBL	R14	K5
      0x8C381D06,  //  01F2  GETMET	R14	R14	K6
      0x8C401934,  //  01F3  GETMET	R16	R12	K52
      0x7C400200,  //  01F4  CALL	R16	1
      0x0042CE10,  //  01F5  ADD	R16	K103	R16
      0x54460003,  //  01F6  LDINT	R17	4
      0x7C380600,  //  01F7  CALL	R14	3
      0xB83A0A00,  //  01F8  GETNGBL	R14	K5
      0x8C381D06,  //  01F9  GETMET	R14	R14	K6
      0x8C401B34,  //  01FA  GETMET	R16	R13	K52
      0x7C400200,  //  01FB  CALL	R16	1
      0x0042D010,  //  01FC  ADD	R16	K104	R16
      0x54460003,  //  01FD  LDINT	R17	4
      0x7C380600,  //  01FE  CALL	R14	3
      0x8C381346,  //  01FF  GETMET	R14	R9	K70
      0x7C380200,  //  0200  CALL	R14	1
      0x8C3C052F,  //  0201  GETMET	R15	R2	K47
      0x5C441A00,  //  0202  MOVE	R17	R13
      0x60480015,  //  0203  GETGBL	R18	G21
      0x7C480000,  //  0204  CALL	R18	0
      0x8C482528,  //  0205  GETMET	R18	R18	K40
      0x88500169,  //  0206  GETMBR	R20	R0	K105
      0x7C480400,  //  0207  CALL	R18	2
      0x604C0015,  //  0208  GETGBL	R19	G21
      0x7C4C0000,  //  0209  CALL	R19	0
      0x6050000C,  //  020A  GETGBL	R20	G12
      0x5C541C00,  //  020B  MOVE	R21	R14
      0x7C500200,  //  020C  CALL	R20	1
      0x5456000F,  //  020D  LDINT	R21	16
      0x7C3C0C00,  //  020E  CALL	R15	6
      0x8C401F6A,  //  020F  GETMET	R16	R15	K106
      0x5C481C00,  //  0210  MOVE	R18	R14
      0x7C400400,  //  0211  CALL	R16	2
      0x8C441F31,  //  0212  GETMET	R17	R15	K49
      0x7C440200,  //  0213  CALL	R17	1
      0x00402011,  //  0214  ADD	R16	R16	R17
      0xB8460A00,  //  0215  GETNGBL	R17	K5
      0x8C442306,  //  0216  GETMET	R17	R17	K6
      0x8C4C2134,  //  0217  GETMET	R19	R16	K52
      0x7C4C0200,  //  0218  CALL	R19	1
      0x004ED613,  //  0219  ADD	R19	K107	R19
      0x54520003,  //  021A  LDINT	R20	4
      0x7C440600,  //  021B  CALL	R17	3
      0xB8460A00,  //  021C  GETNGBL	R17	K5
      0x8C442306,  //  021D  GETMET	R17	R17	K6
      0x584C0032,  //  021E  LDCONST	R19	K50
      0x54520003,  //  021F  LDINT	R20	4
      0x7C440600,  //  0220  CALL	R17	3
      0xB8461800,  //  0221  GETNGBL	R17	K12
      0x8C44236C,  //  0222  GETMET	R17	R17	K108
      0x7C440200,  //  0223  CALL	R17	1
      0x9046DA06,  //  0224  SETMBR	R17	K109	R6
      0x88480126,  //  0225  GETMBR	R18	R0	K38
      0x9046DC12,  //  0226  SETMBR	R17	K110	R18
      0x88480152,  //  0227  GETMBR	R18	R0	K82
      0x9046DE12,  //  0228  SETMBR	R17	K111	R18
      0x9046E010,  //  0229  SETMBR	R17	K112	R16
      0xB84A0A00,  //  022A  GETNGBL	R18	K5
      0x8C482506,  //  022B  GETMET	R18	R18	K6
      0xB8521800,  //  022C  GETNGBL	R20	K12
      0x8C502972,  //  022D  GETMET	R20	R20	K114
      0x5C582200,  //  022E  MOVE	R22	R17
      0x7C500400,  //  022F  CALL	R20	2
      0x0052E214,  //  0230  ADD	R20	K113	R20
      0x54560003,  //  0231  LDINT	R21	4
      0x7C480600,  //  0232  CALL	R18	3
      0x8C482346,  //  0233  GETMET	R18	R17	K70
      0x7C480200,  //  0234  CALL	R18	1
      0x9016E612,  //  0235  SETMBR	R5	K115	R18
      0xB84E0A00,  //  0236  GETNGBL	R19	K5
      0x8C4C2706,  //  0237  GETMET	R19	R19	K6
      0x8C542534,  //  0238  GETMET	R21	R18	K52
      0x7C540200,  //  0239  CALL	R21	1
      0x0056E815,  //  023A  ADD	R21	K116	R21
      0x545A0003,  //  023B  LDINT	R22	4
      0x7C4C0600,  //  023C  CALL	R19	3
      0x8C4C0349,  //  023D  GETMET	R19	R1	K73
      0x54560030,  //  023E  LDINT	R21	49
      0x50580200,  //  023F  LDBOOL	R22	1	0
      0x7C4C0600,  //  0240  CALL	R19	3
      0x8C50274A,  //  0241  GETMET	R20	R19	K74
      0x5C582400,  //  0242  MOVE	R22	R18
      0x7C500400,  //  0243  CALL	R20	2
      0x8854014B,  //  0244  GETMBR	R21	R0	K75
      0x8C542B4C,  //  0245  GETMET	R21	R21	K76
      0x5C5C2800,  //  0246  MOVE	R23	R20
      0x8860034D,  //  0247  GETMBR	R24	R1	K77
      0x8864034E,  //  0248  GETMBR	R25	R1	K78
      0x8868274F,  //  0249  GETMBR	R26	R19	K79
      0x7C540A00,  //  024A  CALL	R21	5
      0x50540200,  //  024B  LDBOOL	R21	1	0
      0x80042A00,  //  024C  RET	1	R21
      0x50180200,  //  024D  LDBOOL	R6	1	0
      0x80040C00,  //  024E  RET	1	R6
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
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20StatusReport_X20_X3D_X20),
    /* K4   */  be_nested_str_weak(raw),
    /* K5   */  be_nested_str_weak(app_payload_idx),
    /* K6   */  be_const_int(2147483647),
    /* K7   */  be_nested_str_weak(tohex),
    }),
    be_str_weak(parse_StatusReport),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
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
      0x7C0C0400,  //  000A  CALL	R3	2
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0x80040600,  //  000C  RET	1	R3
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
    ( &(const bvalue[83]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(MTR_X3A_X20received_X20pA_X3D),
    /* K19  */  be_nested_str_weak(tohex),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20spake_X3A_X20),
    /* K21  */  be_nested_str_weak(inspect),
    /* K22  */  be_nested_str_weak(spake),
    /* K23  */  be_nested_str_weak(SPAKE2P_Matter),
    /* K24  */  be_nested_str_weak(device),
    /* K25  */  be_nested_str_weak(commissioning_w0),
    /* K26  */  be_nested_str_weak(commissioning_L),
    /* K27  */  be_nested_str_weak(compute_pB),
    /* K28  */  be_nested_str_weak(y),
    /* K29  */  be_nested_str_weak(pB),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20y_X3D),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20pb_X3D),
    /* K32  */  be_nested_str_weak(compute_ZV_verifier),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20Z_X3D),
    /* K34  */  be_nested_str_weak(Z),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20V_X3D),
    /* K36  */  be_nested_str_weak(V),
    /* K37  */  be_nested_str_weak(SHA256),
    /* K38  */  be_nested_str_weak(update),
    /* K39  */  be_nested_str_weak(fromstring),
    /* K40  */  be_nested_str_weak(Matter_Context_Prefix),
    /* K41  */  be_nested_str_weak(PBKDFParamRequest),
    /* K42  */  be_nested_str_weak(PBKDFParamResponse),
    /* K43  */  be_nested_str_weak(out),
    /* K44  */  be_nested_str_weak(MTR_X3A_X20Context_X3D),
    /* K45  */  be_nested_str_weak(set_context),
    /* K46  */  be_nested_str_weak(compute_TT_hash),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D),
    /* K48  */  be_nested_str_weak(MTR_X3A_X20Context_X20_X3D_X20),
    /* K49  */  be_nested_str_weak(Context),
    /* K50  */  be_nested_str_weak(MTR_X3A_X20M_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K51  */  be_nested_str_weak(M),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20N_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K53  */  be_nested_str_weak(N),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20pA_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K55  */  be_nested_str_weak(MTR_X3A_X20pB_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K56  */  be_nested_str_weak(MTR_X3A_X20Z_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K57  */  be_nested_str_weak(MTR_X3A_X20V_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K58  */  be_nested_str_weak(MTR_X3A_X20w0_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K59  */  be_nested_str_weak(w0),
    /* K60  */  be_nested_str_weak(MTR_X3A_X20Kmain_X20_X20_X20_X3D),
    /* K61  */  be_nested_str_weak(Kmain),
    /* K62  */  be_nested_str_weak(MTR_X3A_X20KcA_X20_X20_X20_X20_X20_X3D),
    /* K63  */  be_nested_str_weak(KcA),
    /* K64  */  be_nested_str_weak(MTR_X3A_X20KcB_X20_X20_X20_X20_X20_X3D),
    /* K65  */  be_nested_str_weak(KcB),
    /* K66  */  be_nested_str_weak(MTR_X3A_X20K_shared_X3D),
    /* K67  */  be_nested_str_weak(K_shared),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20Ke_X20_X20_X20_X20_X20_X20_X3D),
    /* K69  */  be_nested_str_weak(Ke),
    /* K70  */  be_nested_str_weak(cB),
    /* K71  */  be_nested_str_weak(MTR_X3A_X20cB_X3D),
    /* K72  */  be_nested_str_weak(Pake2),
    /* K73  */  be_nested_str_weak(MTR_X3A_X20pake2_X3A_X20),
    /* K74  */  be_nested_str_weak(encode),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20pake2_raw_X3A_X20),
    /* K76  */  be_nested_str_weak(build_response),
    /* K77  */  be_nested_str_weak(encode_frame),
    /* K78  */  be_nested_str_weak(responder),
    /* K79  */  be_nested_str_weak(send_response),
    /* K80  */  be_nested_str_weak(remote_ip),
    /* K81  */  be_nested_str_weak(remote_port),
    /* K82  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(parse_Pake1),
    &be_const_str_solidified,
    ( &(const binstruction[325]) {  /* code */
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
      0xB8120A00,  //  0027  GETNGBL	R4	K5
      0x8C100906,  //  0028  GETMET	R4	R4	K6
      0x88180111,  //  0029  GETMBR	R6	R0	K17
      0x8C180D13,  //  002A  GETMET	R6	R6	K19
      0x7C180200,  //  002B  CALL	R6	1
      0x001A2406,  //  002C  ADD	R6	K18	R6
      0x541E0003,  //  002D  LDINT	R7	4
      0x7C100600,  //  002E  CALL	R4	3
      0xB8120A00,  //  002F  GETNGBL	R4	K5
      0x8C100906,  //  0030  GETMET	R4	R4	K6
      0xB81A1800,  //  0031  GETNGBL	R6	K12
      0x8C180D15,  //  0032  GETMET	R6	R6	K21
      0x88200116,  //  0033  GETMBR	R8	R0	K22
      0x7C180400,  //  0034  CALL	R6	2
      0x001A2806,  //  0035  ADD	R6	K20	R6
      0x541E0003,  //  0036  LDINT	R7	4
      0x7C100600,  //  0037  CALL	R4	3
      0x8C100517,  //  0038  GETMET	R4	R2	K23
      0x88180118,  //  0039  GETMBR	R6	R0	K24
      0x88180D19,  //  003A  GETMBR	R6	R6	K25
      0x4C1C0000,  //  003B  LDNIL	R7
      0x88200118,  //  003C  GETMBR	R8	R0	K24
      0x8820111A,  //  003D  GETMBR	R8	R8	K26
      0x7C100800,  //  003E  CALL	R4	4
      0x90022C04,  //  003F  SETMBR	R0	K22	R4
      0x88100116,  //  0040  GETMBR	R4	R0	K22
      0x8C10091B,  //  0041  GETMET	R4	R4	K27
      0x8818011C,  //  0042  GETMBR	R6	R0	K28
      0x7C100400,  //  0043  CALL	R4	2
      0x88100116,  //  0044  GETMBR	R4	R0	K22
      0x8810091D,  //  0045  GETMBR	R4	R4	K29
      0x90023A04,  //  0046  SETMBR	R0	K29	R4
      0xB8120A00,  //  0047  GETNGBL	R4	K5
      0x8C100906,  //  0048  GETMET	R4	R4	K6
      0x8818011C,  //  0049  GETMBR	R6	R0	K28
      0x8C180D13,  //  004A  GETMET	R6	R6	K19
      0x7C180200,  //  004B  CALL	R6	1
      0x001A3C06,  //  004C  ADD	R6	K30	R6
      0x541E0003,  //  004D  LDINT	R7	4
      0x7C100600,  //  004E  CALL	R4	3
      0xB8120A00,  //  004F  GETNGBL	R4	K5
      0x8C100906,  //  0050  GETMET	R4	R4	K6
      0x8818011D,  //  0051  GETMBR	R6	R0	K29
      0x8C180D13,  //  0052  GETMET	R6	R6	K19
      0x7C180200,  //  0053  CALL	R6	1
      0x001A3E06,  //  0054  ADD	R6	K31	R6
      0x541E0003,  //  0055  LDINT	R7	4
      0x7C100600,  //  0056  CALL	R4	3
      0x88100116,  //  0057  GETMBR	R4	R0	K22
      0x8C100920,  //  0058  GETMET	R4	R4	K32
      0x88180111,  //  0059  GETMBR	R6	R0	K17
      0x7C100400,  //  005A  CALL	R4	2
      0xB8120A00,  //  005B  GETNGBL	R4	K5
      0x8C100906,  //  005C  GETMET	R4	R4	K6
      0x88180116,  //  005D  GETMBR	R6	R0	K22
      0x88180D22,  //  005E  GETMBR	R6	R6	K34
      0x8C180D13,  //  005F  GETMET	R6	R6	K19
      0x7C180200,  //  0060  CALL	R6	1
      0x001A4206,  //  0061  ADD	R6	K33	R6
      0x541E0003,  //  0062  LDINT	R7	4
      0x7C100600,  //  0063  CALL	R4	3
      0xB8120A00,  //  0064  GETNGBL	R4	K5
      0x8C100906,  //  0065  GETMET	R4	R4	K6
      0x88180116,  //  0066  GETMBR	R6	R0	K22
      0x88180D24,  //  0067  GETMBR	R6	R6	K36
      0x8C180D13,  //  0068  GETMET	R6	R6	K19
      0x7C180200,  //  0069  CALL	R6	1
      0x001A4606,  //  006A  ADD	R6	K35	R6
      0x541E0003,  //  006B  LDINT	R7	4
      0x7C100600,  //  006C  CALL	R4	3
      0x8C100525,  //  006D  GETMET	R4	R2	K37
      0x7C100200,  //  006E  CALL	R4	1
      0x8C140926,  //  006F  GETMET	R5	R4	K38
      0x601C0015,  //  0070  GETGBL	R7	G21
      0x7C1C0000,  //  0071  CALL	R7	0
      0x8C1C0F27,  //  0072  GETMET	R7	R7	K39
      0x88240128,  //  0073  GETMBR	R9	R0	K40
      0x7C1C0400,  //  0074  CALL	R7	2
      0x7C140400,  //  0075  CALL	R5	2
      0x8C140926,  //  0076  GETMET	R5	R4	K38
      0x881C0129,  //  0077  GETMBR	R7	R0	K41
      0x7C140400,  //  0078  CALL	R5	2
      0x8C140926,  //  0079  GETMET	R5	R4	K38
      0x881C012A,  //  007A  GETMBR	R7	R0	K42
      0x7C140400,  //  007B  CALL	R5	2
      0x8C14092B,  //  007C  GETMET	R5	R4	K43
      0x7C140200,  //  007D  CALL	R5	1
      0xB81A0A00,  //  007E  GETNGBL	R6	K5
      0x8C180D06,  //  007F  GETMET	R6	R6	K6
      0x8C200B13,  //  0080  GETMET	R8	R5	K19
      0x7C200200,  //  0081  CALL	R8	1
      0x00225808,  //  0082  ADD	R8	K44	R8
      0x54260003,  //  0083  LDINT	R9	4
      0x7C180600,  //  0084  CALL	R6	3
      0x88180116,  //  0085  GETMBR	R6	R0	K22
      0x881C0111,  //  0086  GETMBR	R7	R0	K17
      0x901A2207,  //  0087  SETMBR	R6	K17	R7
      0x88180116,  //  0088  GETMBR	R6	R0	K22
      0x8C180D2D,  //  0089  GETMET	R6	R6	K45
      0x5C200A00,  //  008A  MOVE	R8	R5
      0x7C180400,  //  008B  CALL	R6	2
      0x88180116,  //  008C  GETMBR	R6	R0	K22
      0x8C180D2E,  //  008D  GETMET	R6	R6	K46
      0x50200200,  //  008E  LDBOOL	R8	1	0
      0x7C180400,  //  008F  CALL	R6	2
      0xB81A0A00,  //  0090  GETNGBL	R6	K5
      0x8C180D06,  //  0091  GETMET	R6	R6	K6
      0x5820002F,  //  0092  LDCONST	R8	K47
      0x54260003,  //  0093  LDINT	R9	4
      0x7C180600,  //  0094  CALL	R6	3
      0xB81A0A00,  //  0095  GETNGBL	R6	K5
      0x8C180D06,  //  0096  GETMET	R6	R6	K6
      0x88200116,  //  0097  GETMBR	R8	R0	K22
      0x88201131,  //  0098  GETMBR	R8	R8	K49
      0x8C201113,  //  0099  GETMET	R8	R8	K19
      0x7C200200,  //  009A  CALL	R8	1
      0x00226008,  //  009B  ADD	R8	K48	R8
      0x54260003,  //  009C  LDINT	R9	4
      0x7C180600,  //  009D  CALL	R6	3
      0xB81A0A00,  //  009E  GETNGBL	R6	K5
      0x8C180D06,  //  009F  GETMET	R6	R6	K6
      0x88200116,  //  00A0  GETMBR	R8	R0	K22
      0x88201133,  //  00A1  GETMBR	R8	R8	K51
      0x8C201113,  //  00A2  GETMET	R8	R8	K19
      0x7C200200,  //  00A3  CALL	R8	1
      0x00226408,  //  00A4  ADD	R8	K50	R8
      0x54260003,  //  00A5  LDINT	R9	4
      0x7C180600,  //  00A6  CALL	R6	3
      0xB81A0A00,  //  00A7  GETNGBL	R6	K5
      0x8C180D06,  //  00A8  GETMET	R6	R6	K6
      0x88200116,  //  00A9  GETMBR	R8	R0	K22
      0x88201135,  //  00AA  GETMBR	R8	R8	K53
      0x8C201113,  //  00AB  GETMET	R8	R8	K19
      0x7C200200,  //  00AC  CALL	R8	1
      0x00226808,  //  00AD  ADD	R8	K52	R8
      0x54260003,  //  00AE  LDINT	R9	4
      0x7C180600,  //  00AF  CALL	R6	3
      0xB81A0A00,  //  00B0  GETNGBL	R6	K5
      0x8C180D06,  //  00B1  GETMET	R6	R6	K6
      0x88200116,  //  00B2  GETMBR	R8	R0	K22
      0x88201111,  //  00B3  GETMBR	R8	R8	K17
      0x8C201113,  //  00B4  GETMET	R8	R8	K19
      0x7C200200,  //  00B5  CALL	R8	1
      0x00226C08,  //  00B6  ADD	R8	K54	R8
      0x54260003,  //  00B7  LDINT	R9	4
      0x7C180600,  //  00B8  CALL	R6	3
      0xB81A0A00,  //  00B9  GETNGBL	R6	K5
      0x8C180D06,  //  00BA  GETMET	R6	R6	K6
      0x88200116,  //  00BB  GETMBR	R8	R0	K22
      0x8820111D,  //  00BC  GETMBR	R8	R8	K29
      0x8C201113,  //  00BD  GETMET	R8	R8	K19
      0x7C200200,  //  00BE  CALL	R8	1
      0x00226E08,  //  00BF  ADD	R8	K55	R8
      0x54260003,  //  00C0  LDINT	R9	4
      0x7C180600,  //  00C1  CALL	R6	3
      0xB81A0A00,  //  00C2  GETNGBL	R6	K5
      0x8C180D06,  //  00C3  GETMET	R6	R6	K6
      0x88200116,  //  00C4  GETMBR	R8	R0	K22
      0x88201122,  //  00C5  GETMBR	R8	R8	K34
      0x8C201113,  //  00C6  GETMET	R8	R8	K19
      0x7C200200,  //  00C7  CALL	R8	1
      0x00227008,  //  00C8  ADD	R8	K56	R8
      0x54260003,  //  00C9  LDINT	R9	4
      0x7C180600,  //  00CA  CALL	R6	3
      0xB81A0A00,  //  00CB  GETNGBL	R6	K5
      0x8C180D06,  //  00CC  GETMET	R6	R6	K6
      0x88200116,  //  00CD  GETMBR	R8	R0	K22
      0x88201124,  //  00CE  GETMBR	R8	R8	K36
      0x8C201113,  //  00CF  GETMET	R8	R8	K19
      0x7C200200,  //  00D0  CALL	R8	1
      0x00227208,  //  00D1  ADD	R8	K57	R8
      0x54260003,  //  00D2  LDINT	R9	4
      0x7C180600,  //  00D3  CALL	R6	3
      0xB81A0A00,  //  00D4  GETNGBL	R6	K5
      0x8C180D06,  //  00D5  GETMET	R6	R6	K6
      0x88200116,  //  00D6  GETMBR	R8	R0	K22
      0x8820113B,  //  00D7  GETMBR	R8	R8	K59
      0x8C201113,  //  00D8  GETMET	R8	R8	K19
      0x7C200200,  //  00D9  CALL	R8	1
      0x00227408,  //  00DA  ADD	R8	K58	R8
      0x54260003,  //  00DB  LDINT	R9	4
      0x7C180600,  //  00DC  CALL	R6	3
      0xB81A0A00,  //  00DD  GETNGBL	R6	K5
      0x8C180D06,  //  00DE  GETMET	R6	R6	K6
      0x5820002F,  //  00DF  LDCONST	R8	K47
      0x54260003,  //  00E0  LDINT	R9	4
      0x7C180600,  //  00E1  CALL	R6	3
      0xB81A0A00,  //  00E2  GETNGBL	R6	K5
      0x8C180D06,  //  00E3  GETMET	R6	R6	K6
      0x88200116,  //  00E4  GETMBR	R8	R0	K22
      0x8820113D,  //  00E5  GETMBR	R8	R8	K61
      0x8C201113,  //  00E6  GETMET	R8	R8	K19
      0x7C200200,  //  00E7  CALL	R8	1
      0x00227808,  //  00E8  ADD	R8	K60	R8
      0x54260003,  //  00E9  LDINT	R9	4
      0x7C180600,  //  00EA  CALL	R6	3
      0xB81A0A00,  //  00EB  GETNGBL	R6	K5
      0x8C180D06,  //  00EC  GETMET	R6	R6	K6
      0x88200116,  //  00ED  GETMBR	R8	R0	K22
      0x8820113F,  //  00EE  GETMBR	R8	R8	K63
      0x8C201113,  //  00EF  GETMET	R8	R8	K19
      0x7C200200,  //  00F0  CALL	R8	1
      0x00227C08,  //  00F1  ADD	R8	K62	R8
      0x54260003,  //  00F2  LDINT	R9	4
      0x7C180600,  //  00F3  CALL	R6	3
      0xB81A0A00,  //  00F4  GETNGBL	R6	K5
      0x8C180D06,  //  00F5  GETMET	R6	R6	K6
      0x88200116,  //  00F6  GETMBR	R8	R0	K22
      0x88201141,  //  00F7  GETMBR	R8	R8	K65
      0x8C201113,  //  00F8  GETMET	R8	R8	K19
      0x7C200200,  //  00F9  CALL	R8	1
      0x00228008,  //  00FA  ADD	R8	K64	R8
      0x54260003,  //  00FB  LDINT	R9	4
      0x7C180600,  //  00FC  CALL	R6	3
      0xB81A0A00,  //  00FD  GETNGBL	R6	K5
      0x8C180D06,  //  00FE  GETMET	R6	R6	K6
      0x88200116,  //  00FF  GETMBR	R8	R0	K22
      0x88201143,  //  0100  GETMBR	R8	R8	K67
      0x8C201113,  //  0101  GETMET	R8	R8	K19
      0x7C200200,  //  0102  CALL	R8	1
      0x00228408,  //  0103  ADD	R8	K66	R8
      0x54260003,  //  0104  LDINT	R9	4
      0x7C180600,  //  0105  CALL	R6	3
      0xB81A0A00,  //  0106  GETNGBL	R6	K5
      0x8C180D06,  //  0107  GETMET	R6	R6	K6
      0x88200116,  //  0108  GETMBR	R8	R0	K22
      0x88201145,  //  0109  GETMBR	R8	R8	K69
      0x8C201113,  //  010A  GETMET	R8	R8	K19
      0x7C200200,  //  010B  CALL	R8	1
      0x00228808,  //  010C  ADD	R8	K68	R8
      0x54260003,  //  010D  LDINT	R9	4
      0x7C180600,  //  010E  CALL	R6	3
      0x88180116,  //  010F  GETMBR	R6	R0	K22
      0x88180D46,  //  0110  GETMBR	R6	R6	K70
      0x90028C06,  //  0111  SETMBR	R0	K70	R6
      0x88180116,  //  0112  GETMBR	R6	R0	K22
      0x88180D45,  //  0113  GETMBR	R6	R6	K69
      0x90028A06,  //  0114  SETMBR	R0	K69	R6
      0xB81A0A00,  //  0115  GETNGBL	R6	K5
      0x8C180D06,  //  0116  GETMET	R6	R6	K6
      0x88200146,  //  0117  GETMBR	R8	R0	K70
      0x8C201113,  //  0118  GETMET	R8	R8	K19
      0x7C200200,  //  0119  CALL	R8	1
      0x00228E08,  //  011A  ADD	R8	K71	R8
      0x54260003,  //  011B  LDINT	R9	4
      0x7C180600,  //  011C  CALL	R6	3
      0xB81A1800,  //  011D  GETNGBL	R6	K12
      0x8C180D48,  //  011E  GETMET	R6	R6	K72
      0x7C180200,  //  011F  CALL	R6	1
      0x881C011D,  //  0120  GETMBR	R7	R0	K29
      0x901A3A07,  //  0121  SETMBR	R6	K29	R7
      0x881C0146,  //  0122  GETMBR	R7	R0	K70
      0x901A8C07,  //  0123  SETMBR	R6	K70	R7
      0xB81E0A00,  //  0124  GETNGBL	R7	K5
      0x8C1C0F06,  //  0125  GETMET	R7	R7	K6
      0xB8261800,  //  0126  GETNGBL	R9	K12
      0x8C241315,  //  0127  GETMET	R9	R9	K21
      0x5C2C0C00,  //  0128  MOVE	R11	R6
      0x7C240400,  //  0129  CALL	R9	2
      0x00269209,  //  012A  ADD	R9	K73	R9
      0x542A0003,  //  012B  LDINT	R10	4
      0x7C1C0600,  //  012C  CALL	R7	3
      0x8C1C0D4A,  //  012D  GETMET	R7	R6	K74
      0x7C1C0200,  //  012E  CALL	R7	1
      0xB8220A00,  //  012F  GETNGBL	R8	K5
      0x8C201106,  //  0130  GETMET	R8	R8	K6
      0x8C280F13,  //  0131  GETMET	R10	R7	K19
      0x7C280200,  //  0132  CALL	R10	1
      0x002A960A,  //  0133  ADD	R10	K75	R10
      0x542E0003,  //  0134  LDINT	R11	4
      0x7C200600,  //  0135  CALL	R8	3
      0x8C20034C,  //  0136  GETMET	R8	R1	K76
      0x542A0022,  //  0137  LDINT	R10	35
      0x502C0200,  //  0138  LDBOOL	R11	1	0
      0x7C200600,  //  0139  CALL	R8	3
      0x8C24114D,  //  013A  GETMET	R9	R8	K77
      0x5C2C0E00,  //  013B  MOVE	R11	R7
      0x7C240400,  //  013C  CALL	R9	2
      0x8828014E,  //  013D  GETMBR	R10	R0	K78
      0x8C28154F,  //  013E  GETMET	R10	R10	K79
      0x5C301200,  //  013F  MOVE	R12	R9
      0x88340350,  //  0140  GETMBR	R13	R1	K80
      0x88380351,  //  0141  GETMBR	R14	R1	K81
      0x883C1152,  //  0142  GETMBR	R15	R8	K82
      0x7C280A00,  //  0143  CALL	R10	5
      0x80000000,  //  0144  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_status_report
********************************************************************/
be_local_closure(Matter_Commisioning_Context_send_status_report,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(build_response),
    /* K1   */  be_nested_str_weak(add),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str_weak(encode_frame),
    /* K4   */  be_nested_str_weak(responder),
    /* K5   */  be_nested_str_weak(send_response),
    /* K6   */  be_nested_str_weak(remote_ip),
    /* K7   */  be_nested_str_weak(remote_port),
    /* K8   */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(send_status_report),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
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
      0x88380D08,  //  001A  GETMBR	R14	R6	K8
      0x7C240A00,  //  001B  CALL	R9	5
      0x80000000,  //  001C  RET	0
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
    be_nested_map(37,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Matter_Commisioning_Context_init_closure) },
        { be_const_key_weak(S2K_Info, 5), be_nested_str_weak(Sigma2) },
        { be_const_key_weak(cB, 1), be_const_var(11) },
        { be_const_key_weak(parse_Sigma3, 23), be_const_closure(Matter_Commisioning_Context_parse_Sigma3_closure) },
        { be_const_key_weak(future_local_session_id, -1), be_const_var(4) },
        { be_const_key_weak(SEKeys_Info, -1), be_nested_str_weak(SessionKeys) },
        { be_const_key_weak(ResponderEph_priv, -1), be_const_var(13) },
        { be_const_key_weak(Ke, 18), be_const_var(12) },
        { be_const_key_weak(PBKDFParamResponse, 25), be_const_var(6) },
        { be_const_key_weak(process_incoming, 36), be_const_closure(Matter_Commisioning_Context_process_incoming_closure) },
        { be_const_key_weak(future_initiator_session_id, -1), be_const_var(3) },
        { be_const_key_weak(y, -1), be_const_var(7) },
        { be_const_key_weak(I2RKey, -1), be_const_var(17) },
        { be_const_key_weak(responder, 34), be_const_var(0) },
        { be_const_key_weak(pA, 33), be_const_var(8) },
        { be_const_key_weak(TBEData2_Nonce, -1), be_nested_str_weak(NCASE_Sigma2N) },
        { be_const_key_weak(find_fabric_by_destination_id, -1), be_const_closure(Matter_Commisioning_Context_find_fabric_by_destination_id_closure) },
        { be_const_key_weak(device, -1), be_const_var(1) },
        { be_const_key_weak(parse_Pake1, -1), be_const_closure(Matter_Commisioning_Context_parse_Pake1_closure) },
        { be_const_key_weak(parse_Pake3, -1), be_const_closure(Matter_Commisioning_Context_parse_Pake3_closure) },
        { be_const_key_weak(cA, 29), be_const_var(10) },
        { be_const_key_weak(ResponderEph_pub, 9), be_const_var(14) },
        { be_const_key_weak(parse_PBKDFParamRequest, 6), be_const_closure(Matter_Commisioning_Context_parse_PBKDFParamRequest_closure) },
        { be_const_key_weak(parse_StatusReport, -1), be_const_closure(Matter_Commisioning_Context_parse_StatusReport_closure) },
        { be_const_key_weak(Matter_Context_Prefix, -1), be_nested_str_weak(CHIP_X20PAKE_X20V1_X20Commissioning) },
        { be_const_key_weak(pB, 21), be_const_var(9) },
        { be_const_key_weak(TBEData3_Nonce, -1), be_nested_str_weak(NCASE_Sigma3N) },
        { be_const_key_weak(S3K_Info, 26), be_nested_str_weak(Sigma3) },
        { be_const_key_weak(parse_Sigma1, -1), be_const_closure(Matter_Commisioning_Context_parse_Sigma1_closure) },
        { be_const_key_weak(created, -1), be_const_var(16) },
        { be_const_key_weak(PBKDFParamRequest, -1), be_const_var(5) },
        { be_const_key_weak(every_second, 7), be_const_closure(Matter_Commisioning_Context_every_second_closure) },
        { be_const_key_weak(send_status_report, -1), be_const_closure(Matter_Commisioning_Context_send_status_report_closure) },
        { be_const_key_weak(AttestationChallenge, -1), be_const_var(19) },
        { be_const_key_weak(R2IKey, 35), be_const_var(18) },
        { be_const_key_weak(initiatorEph_pub, -1), be_const_var(15) },
        { be_const_key_weak(spake, -1), be_const_var(2) },
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
