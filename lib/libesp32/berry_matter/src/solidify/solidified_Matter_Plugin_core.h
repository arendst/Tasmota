/* Solidification of Matter_Plugin_core.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_core;

/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_core_invoke_request,   /* name */
  be_nested_proto(
    29,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[73]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(session),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(findsubval),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(breadcrumb),
    /* K10  */  be_nested_str_weak(Matter_TLV_struct),
    /* K11  */  be_nested_str_weak(add_TLV),
    /* K12  */  be_nested_str_weak(U1),
    /* K13  */  be_nested_str_weak(UTF1),
    /* K14  */  be_nested_str_weak(),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(XX),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(set_no_expiration),
    /* K19  */  be_nested_str_weak(device),
    /* K20  */  be_nested_str_weak(start_commissioning_complete_deferred),
    /* K21  */  be_nested_str_weak(status),
    /* K22  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K23  */  be_nested_str_weak(B2),
    /* K24  */  be_nested_str_weak(DAC_Cert_FFF1_8000),
    /* K25  */  be_nested_str_weak(PAI_Cert_FFF1),
    /* K26  */  be_nested_str_weak(CD_FFF1_8000),
    /* K27  */  be_nested_str_weak(B1),
    /* K28  */  be_nested_str_weak(U4),
    /* K29  */  be_nested_str_weak(tasmota),
    /* K30  */  be_nested_str_weak(rtc),
    /* K31  */  be_nested_str_weak(utc),
    /* K32  */  be_nested_str_weak(encode),
    /* K33  */  be_nested_str_weak(get_ac),
    /* K34  */  be_nested_str_weak(log),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20attestation_tbs_X3D),
    /* K36  */  be_nested_str_weak(tohex),
    /* K37  */  be_nested_str_weak(EC_P256),
    /* K38  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K39  */  be_nested_str_weak(DAC_Priv_FFF1_8000),
    /* K40  */  be_nested_str_weak(gen_CSR),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20nocsr_tbs_X3D),
    /* K42  */  be_nested_str_weak(set_ca),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20received_X20ca_root_X3D),
    /* K44  */  be_nested_str_weak(SUCCESS),
    /* K45  */  be_nested_str_weak(get_ca),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20AdNOC_X20without_X20CA),
    /* K47  */  be_nested_str_weak(set_noc),
    /* K48  */  be_nested_str_weak(set_ipk_epoch_key),
    /* K49  */  be_nested_str_weak(admin_subject),
    /* K50  */  be_nested_str_weak(admin_vendor),
    /* K51  */  be_nested_str_weak(parse),
    /* K52  */  be_nested_str_weak(findsub),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20no_X20fabricid_X20nor_X20deviceid_X20in_X20NOC_X20certificate),
    /* K54  */  be_nested_str_weak(int),
    /* K55  */  be_nested_str_weak(int64),
    /* K56  */  be_nested_str_weak(tobytes),
    /* K57  */  be_const_int(2147483647),
    /* K58  */  be_nested_str_weak(fromstring),
    /* K59  */  be_nested_str_weak(CompressedFabric),
    /* K60  */  be_nested_str_weak(HKDF_SHA256),
    /* K61  */  be_nested_str_weak(copy),
    /* K62  */  be_nested_str_weak(reverse),
    /* K63  */  be_nested_str_weak(derive),
    /* K64  */  be_nested_str_weak(set_fabric_device),
    /* K65  */  be_nested_str_weak(start_operational_dicovery_deferred),
    /* K66  */  be_nested_str_weak(set_fabric_label),
    /* K67  */  be_nested_str_weak(sessions),
    /* K68  */  be_nested_str_weak(sessions_active),
    /* K69  */  be_nested_str_weak(MTR_X3A_X20removing_X20fabric_X20),
    /* K70  */  be_nested_str_weak(fabric),
    /* K71  */  be_nested_str_weak(remove_session),
    /* K72  */  be_nested_str_weak(save),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[495]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x88200305,  //  0005  GETMBR	R8	R1	K5
      0x5426002F,  //  0006  LDINT	R9	48
      0x1C240C09,  //  0007  EQ	R9	R6	R9
      0x78260050,  //  0008  JMPF	R9	#005A
      0x1C240F06,  //  0009  EQ	R9	R7	K6
      0x78260017,  //  000A  JMPF	R9	#0023
      0x8C240507,  //  000B  GETMET	R9	R2	K7
      0x582C0006,  //  000C  LDCONST	R11	K6
      0x54320383,  //  000D  LDINT	R12	900
      0x7C240600,  //  000E  CALL	R9	3
      0x8C280507,  //  000F  GETMET	R10	R2	K7
      0x58300008,  //  0010  LDCONST	R12	K8
      0x58340006,  //  0011  LDCONST	R13	K6
      0x7C280600,  //  0012  CALL	R10	3
      0x9022120A,  //  0013  SETMBR	R8	K9	R10
      0x8C2C0B0A,  //  0014  GETMET	R11	R5	K10
      0x7C2C0200,  //  0015  CALL	R11	1
      0x8C30170B,  //  0016  GETMET	R12	R11	K11
      0x58380006,  //  0017  LDCONST	R14	K6
      0x883C0B0C,  //  0018  GETMBR	R15	R5	K12
      0x58400006,  //  0019  LDCONST	R16	K6
      0x7C300800,  //  001A  CALL	R12	4
      0x8C30170B,  //  001B  GETMET	R12	R11	K11
      0x58380008,  //  001C  LDCONST	R14	K8
      0x883C0B0D,  //  001D  GETMBR	R15	R5	K13
      0x5840000E,  //  001E  LDCONST	R16	K14
      0x7C300800,  //  001F  CALL	R12	4
      0x900E0908,  //  0020  SETMBR	R3	K4	K8
      0x80041600,  //  0021  RET	1	R11
      0x70020035,  //  0022  JMP		#0059
      0x1C240F0F,  //  0023  EQ	R9	R7	K15
      0x7826001A,  //  0024  JMPF	R9	#0040
      0x8C240507,  //  0025  GETMET	R9	R2	K7
      0x582C0006,  //  0026  LDCONST	R11	K6
      0x7C240400,  //  0027  CALL	R9	2
      0x8C280507,  //  0028  GETMET	R10	R2	K7
      0x58300008,  //  0029  LDCONST	R12	K8
      0x58340010,  //  002A  LDCONST	R13	K16
      0x7C280600,  //  002B  CALL	R10	3
      0x8C2C0507,  //  002C  GETMET	R11	R2	K7
      0x5834000F,  //  002D  LDCONST	R13	K15
      0x58380006,  //  002E  LDCONST	R14	K6
      0x7C2C0600,  //  002F  CALL	R11	3
      0x9022120B,  //  0030  SETMBR	R8	K9	R11
      0x8C300B0A,  //  0031  GETMET	R12	R5	K10
      0x7C300200,  //  0032  CALL	R12	1
      0x8C34190B,  //  0033  GETMET	R13	R12	K11
      0x583C0006,  //  0034  LDCONST	R15	K6
      0x88400B0C,  //  0035  GETMBR	R16	R5	K12
      0x58440006,  //  0036  LDCONST	R17	K6
      0x7C340800,  //  0037  CALL	R13	4
      0x8C34190B,  //  0038  GETMET	R13	R12	K11
      0x583C0008,  //  0039  LDCONST	R15	K8
      0x88400B0D,  //  003A  GETMBR	R16	R5	K13
      0x5844000E,  //  003B  LDCONST	R17	K14
      0x7C340800,  //  003C  CALL	R13	4
      0x900E0911,  //  003D  SETMBR	R3	K4	K17
      0x80041800,  //  003E  RET	1	R12
      0x70020018,  //  003F  JMP		#0059
      0x54260003,  //  0040  LDINT	R9	4
      0x1C240E09,  //  0041  EQ	R9	R7	R9
      0x78260015,  //  0042  JMPF	R9	#0059
      0x90221306,  //  0043  SETMBR	R8	K9	K6
      0x8C241112,  //  0044  GETMET	R9	R8	K18
      0x7C240200,  //  0045  CALL	R9	1
      0x8C240B0A,  //  0046  GETMET	R9	R5	K10
      0x7C240200,  //  0047  CALL	R9	1
      0x8C28130B,  //  0048  GETMET	R10	R9	K11
      0x58300006,  //  0049  LDCONST	R12	K6
      0x88340B0C,  //  004A  GETMBR	R13	R5	K12
      0x58380006,  //  004B  LDCONST	R14	K6
      0x7C280800,  //  004C  CALL	R10	4
      0x8C28130B,  //  004D  GETMET	R10	R9	K11
      0x58300008,  //  004E  LDCONST	R12	K8
      0x88340B0D,  //  004F  GETMBR	R13	R5	K13
      0x5838000E,  //  0050  LDCONST	R14	K14
      0x7C280800,  //  0051  CALL	R10	4
      0x542A0004,  //  0052  LDINT	R10	5
      0x900E080A,  //  0053  SETMBR	R3	K4	R10
      0x88280113,  //  0054  GETMBR	R10	R0	K19
      0x8C281514,  //  0055  GETMET	R10	R10	K20
      0x5C301000,  //  0056  MOVE	R12	R8
      0x7C280400,  //  0057  CALL	R10	2
      0x80041200,  //  0058  RET	1	R9
      0x70020193,  //  0059  JMP		#01EE
      0x5426003D,  //  005A  LDINT	R9	62
      0x1C240C09,  //  005B  EQ	R9	R6	R9
      0x78260190,  //  005C  JMPF	R9	#01EE
      0x1C240F0F,  //  005D  EQ	R9	R7	K15
      0x7826001D,  //  005E  JMPF	R9	#007D
      0x8C240507,  //  005F  GETMET	R9	R2	K7
      0x582C0006,  //  0060  LDCONST	R11	K6
      0x7C240400,  //  0061  CALL	R9	2
      0x20281308,  //  0062  NE	R10	R9	K8
      0x782A0006,  //  0063  JMPF	R10	#006B
      0x2028130F,  //  0064  NE	R10	R9	K15
      0x782A0004,  //  0065  JMPF	R10	#006B
      0xB82A0200,  //  0066  GETNGBL	R10	K1
      0x88281516,  //  0067  GETMBR	R10	R10	K22
      0x900E2A0A,  //  0068  SETMBR	R3	K21	R10
      0x4C280000,  //  0069  LDNIL	R10
      0x80041400,  //  006A  RET	1	R10
      0x8C280B0A,  //  006B  GETMET	R10	R5	K10
      0x7C280200,  //  006C  CALL	R10	1
      0x8C2C150B,  //  006D  GETMET	R11	R10	K11
      0x58340006,  //  006E  LDCONST	R13	K6
      0x88380B17,  //  006F  GETMBR	R14	R5	K23
      0x1C3C1308,  //  0070  EQ	R15	R9	K8
      0x783E0003,  //  0071  JMPF	R15	#0076
      0xB83E0200,  //  0072  GETNGBL	R15	K1
      0x8C3C1F18,  //  0073  GETMET	R15	R15	K24
      0x7C3C0200,  //  0074  CALL	R15	1
      0x70020002,  //  0075  JMP		#0079
      0xB83E0200,  //  0076  GETNGBL	R15	K1
      0x8C3C1F19,  //  0077  GETMET	R15	R15	K25
      0x7C3C0200,  //  0078  CALL	R15	1
      0x7C2C0800,  //  0079  CALL	R11	4
      0x900E0911,  //  007A  SETMBR	R3	K4	K17
      0x80041400,  //  007B  RET	1	R10
      0x70020170,  //  007C  JMP		#01EE
      0x1C240F06,  //  007D  EQ	R9	R7	K6
      0x78260044,  //  007E  JMPF	R9	#00C4
      0x8C240507,  //  007F  GETMET	R9	R2	K7
      0x582C0006,  //  0080  LDCONST	R11	K6
      0x7C240400,  //  0081  CALL	R9	2
      0x6028000C,  //  0082  GETGBL	R10	G12
      0x5C2C1200,  //  0083  MOVE	R11	R9
      0x7C280200,  //  0084  CALL	R10	1
      0x542E001F,  //  0085  LDINT	R11	32
      0x2028140B,  //  0086  NE	R10	R10	R11
      0x782A0001,  //  0087  JMPF	R10	#008A
      0x4C280000,  //  0088  LDNIL	R10
      0x80041400,  //  0089  RET	1	R10
      0x900E0908,  //  008A  SETMBR	R3	K4	K8
      0x8C280B0A,  //  008B  GETMET	R10	R5	K10
      0x7C280200,  //  008C  CALL	R10	1
      0x8C2C150B,  //  008D  GETMET	R11	R10	K11
      0x58340008,  //  008E  LDCONST	R13	K8
      0x88380B17,  //  008F  GETMBR	R14	R5	K23
      0xB83E0200,  //  0090  GETNGBL	R15	K1
      0x8C3C1F1A,  //  0091  GETMET	R15	R15	K26
      0x7C3C0200,  //  0092  CALL	R15	1
      0x7C2C0800,  //  0093  CALL	R11	4
      0x8C2C150B,  //  0094  GETMET	R11	R10	K11
      0x5834000F,  //  0095  LDCONST	R13	K15
      0x88380B1B,  //  0096  GETMBR	R14	R5	K27
      0x5C3C1200,  //  0097  MOVE	R15	R9
      0x7C2C0800,  //  0098  CALL	R11	4
      0x8C2C150B,  //  0099  GETMET	R11	R10	K11
      0x58340011,  //  009A  LDCONST	R13	K17
      0x88380B1C,  //  009B  GETMBR	R14	R5	K28
      0xB83E3A00,  //  009C  GETNGBL	R15	K29
      0x8C3C1F1E,  //  009D  GETMET	R15	R15	K30
      0x7C3C0200,  //  009E  CALL	R15	1
      0x943C1F1F,  //  009F  GETIDX	R15	R15	K31
      0x7C2C0800,  //  00A0  CALL	R11	4
      0x8C2C1520,  //  00A1  GETMET	R11	R10	K32
      0x7C2C0200,  //  00A2  CALL	R11	1
      0x8C301121,  //  00A3  GETMET	R12	R8	K33
      0x7C300200,  //  00A4  CALL	R12	1
      0x0034160C,  //  00A5  ADD	R13	R11	R12
      0xB83A3A00,  //  00A6  GETNGBL	R14	K29
      0x8C381D22,  //  00A7  GETMET	R14	R14	K34
      0x8C401B24,  //  00A8  GETMET	R16	R13	K36
      0x7C400200,  //  00A9  CALL	R16	1
      0x00424610,  //  00AA  ADD	R16	K35	R16
      0x58440011,  //  00AB  LDCONST	R17	K17
      0x7C380600,  //  00AC  CALL	R14	3
      0x8C380925,  //  00AD  GETMET	R14	R4	K37
      0x7C380200,  //  00AE  CALL	R14	1
      0x8C381D26,  //  00AF  GETMET	R14	R14	K38
      0xB8420200,  //  00B0  GETNGBL	R16	K1
      0x8C402127,  //  00B1  GETMET	R16	R16	K39
      0x7C400200,  //  00B2  CALL	R16	1
      0x5C441A00,  //  00B3  MOVE	R17	R13
      0x7C380600,  //  00B4  CALL	R14	3
      0x8C3C0B0A,  //  00B5  GETMET	R15	R5	K10
      0x7C3C0200,  //  00B6  CALL	R15	1
      0x8C401F0B,  //  00B7  GETMET	R16	R15	K11
      0x58480006,  //  00B8  LDCONST	R18	K6
      0x884C0B17,  //  00B9  GETMBR	R19	R5	K23
      0x5C501600,  //  00BA  MOVE	R20	R11
      0x7C400800,  //  00BB  CALL	R16	4
      0x8C401F0B,  //  00BC  GETMET	R16	R15	K11
      0x58480008,  //  00BD  LDCONST	R18	K8
      0x884C0B1B,  //  00BE  GETMBR	R19	R5	K27
      0x5C501C00,  //  00BF  MOVE	R20	R14
      0x7C400800,  //  00C0  CALL	R16	4
      0x900E0908,  //  00C1  SETMBR	R3	K4	K8
      0x80041E00,  //  00C2  RET	1	R15
      0x70020129,  //  00C3  JMP		#01EE
      0x54260003,  //  00C4  LDINT	R9	4
      0x1C240E09,  //  00C5  EQ	R9	R7	R9
      0x78260040,  //  00C6  JMPF	R9	#0108
      0x8C240507,  //  00C7  GETMET	R9	R2	K7
      0x582C0006,  //  00C8  LDCONST	R11	K6
      0x7C240400,  //  00C9  CALL	R9	2
      0x6028000C,  //  00CA  GETGBL	R10	G12
      0x5C2C1200,  //  00CB  MOVE	R11	R9
      0x7C280200,  //  00CC  CALL	R10	1
      0x542E001F,  //  00CD  LDINT	R11	32
      0x2028140B,  //  00CE  NE	R10	R10	R11
      0x782A0001,  //  00CF  JMPF	R10	#00D2
      0x4C280000,  //  00D0  LDNIL	R10
      0x80041400,  //  00D1  RET	1	R10
      0x8C280507,  //  00D2  GETMET	R10	R2	K7
      0x58300008,  //  00D3  LDCONST	R12	K8
      0x50340000,  //  00D4  LDBOOL	R13	0	0
      0x7C280600,  //  00D5  CALL	R10	3
      0x8C2C1128,  //  00D6  GETMET	R11	R8	K40
      0x7C2C0200,  //  00D7  CALL	R11	1
      0x8C300B0A,  //  00D8  GETMET	R12	R5	K10
      0x7C300200,  //  00D9  CALL	R12	1
      0x8C34190B,  //  00DA  GETMET	R13	R12	K11
      0x583C0008,  //  00DB  LDCONST	R15	K8
      0x88400B17,  //  00DC  GETMBR	R16	R5	K23
      0x5C441600,  //  00DD  MOVE	R17	R11
      0x7C340800,  //  00DE  CALL	R13	4
      0x8C34190B,  //  00DF  GETMET	R13	R12	K11
      0x583C000F,  //  00E0  LDCONST	R15	K15
      0x88400B1B,  //  00E1  GETMBR	R16	R5	K27
      0x5C441200,  //  00E2  MOVE	R17	R9
      0x7C340800,  //  00E3  CALL	R13	4
      0x8C341920,  //  00E4  GETMET	R13	R12	K32
      0x7C340200,  //  00E5  CALL	R13	1
      0x8C381121,  //  00E6  GETMET	R14	R8	K33
      0x7C380200,  //  00E7  CALL	R14	1
      0x00381A0E,  //  00E8  ADD	R14	R13	R14
      0xB83E3A00,  //  00E9  GETNGBL	R15	K29
      0x8C3C1F22,  //  00EA  GETMET	R15	R15	K34
      0x8C441D24,  //  00EB  GETMET	R17	R14	K36
      0x7C440200,  //  00EC  CALL	R17	1
      0x00465211,  //  00ED  ADD	R17	K41	R17
      0x58480011,  //  00EE  LDCONST	R18	K17
      0x7C3C0600,  //  00EF  CALL	R15	3
      0x8C3C0925,  //  00F0  GETMET	R15	R4	K37
      0x7C3C0200,  //  00F1  CALL	R15	1
      0x8C3C1F26,  //  00F2  GETMET	R15	R15	K38
      0xB8460200,  //  00F3  GETNGBL	R17	K1
      0x8C442327,  //  00F4  GETMET	R17	R17	K39
      0x7C440200,  //  00F5  CALL	R17	1
      0x5C481C00,  //  00F6  MOVE	R18	R14
      0x7C3C0600,  //  00F7  CALL	R15	3
      0x8C400B0A,  //  00F8  GETMET	R16	R5	K10
      0x7C400200,  //  00F9  CALL	R16	1
      0x8C44210B,  //  00FA  GETMET	R17	R16	K11
      0x584C0006,  //  00FB  LDCONST	R19	K6
      0x88500B17,  //  00FC  GETMBR	R20	R5	K23
      0x5C541A00,  //  00FD  MOVE	R21	R13
      0x7C440800,  //  00FE  CALL	R17	4
      0x8C44210B,  //  00FF  GETMET	R17	R16	K11
      0x584C0008,  //  0100  LDCONST	R19	K8
      0x88500B1B,  //  0101  GETMBR	R20	R5	K27
      0x5C541E00,  //  0102  MOVE	R21	R15
      0x7C440800,  //  0103  CALL	R17	4
      0x54460004,  //  0104  LDINT	R17	5
      0x900E0811,  //  0105  SETMBR	R3	K4	R17
      0x80042000,  //  0106  RET	1	R16
      0x700200E5,  //  0107  JMP		#01EE
      0x5426000A,  //  0108  LDINT	R9	11
      0x1C240E09,  //  0109  EQ	R9	R7	R9
      0x78260012,  //  010A  JMPF	R9	#011E
      0x8C240507,  //  010B  GETMET	R9	R2	K7
      0x582C0006,  //  010C  LDCONST	R11	K6
      0x7C240400,  //  010D  CALL	R9	2
      0x8C28112A,  //  010E  GETMET	R10	R8	K42
      0x5C301200,  //  010F  MOVE	R12	R9
      0x7C280400,  //  0110  CALL	R10	2
      0xB82A3A00,  //  0111  GETNGBL	R10	K29
      0x8C281522,  //  0112  GETMET	R10	R10	K34
      0x8C301324,  //  0113  GETMET	R12	R9	K36
      0x7C300200,  //  0114  CALL	R12	1
      0x0032560C,  //  0115  ADD	R12	K43	R12
      0x58340011,  //  0116  LDCONST	R13	K17
      0x7C280600,  //  0117  CALL	R10	3
      0xB82A0200,  //  0118  GETNGBL	R10	K1
      0x8828152C,  //  0119  GETMBR	R10	R10	K44
      0x900E2A0A,  //  011A  SETMBR	R3	K21	R10
      0x4C280000,  //  011B  LDNIL	R10
      0x80041400,  //  011C  RET	1	R10
      0x700200CF,  //  011D  JMP		#01EE
      0x54260005,  //  011E  LDINT	R9	6
      0x1C240E09,  //  011F  EQ	R9	R7	R9
      0x78260091,  //  0120  JMPF	R9	#01B3
      0x8C240507,  //  0121  GETMET	R9	R2	K7
      0x582C0006,  //  0122  LDCONST	R11	K6
      0x7C240400,  //  0123  CALL	R9	2
      0x8C280507,  //  0124  GETMET	R10	R2	K7
      0x58300008,  //  0125  LDCONST	R12	K8
      0x7C280400,  //  0126  CALL	R10	2
      0x8C2C0507,  //  0127  GETMET	R11	R2	K7
      0x5834000F,  //  0128  LDCONST	R13	K15
      0x7C2C0400,  //  0129  CALL	R11	2
      0x8C300507,  //  012A  GETMET	R12	R2	K7
      0x58380011,  //  012B  LDCONST	R14	K17
      0x7C300400,  //  012C  CALL	R12	2
      0x8C340507,  //  012D  GETMET	R13	R2	K7
      0x543E0003,  //  012E  LDINT	R15	4
      0x7C340400,  //  012F  CALL	R13	2
      0x8C38112D,  //  0130  GETMET	R14	R8	K45
      0x7C380200,  //  0131  CALL	R14	1
      0x4C3C0000,  //  0132  LDNIL	R15
      0x1C381C0F,  //  0133  EQ	R14	R14	R15
      0x783A0006,  //  0134  JMPF	R14	#013C
      0xB83A3A00,  //  0135  GETNGBL	R14	K29
      0x8C381D22,  //  0136  GETMET	R14	R14	K34
      0x5840002E,  //  0137  LDCONST	R16	K46
      0x5844000F,  //  0138  LDCONST	R17	K15
      0x7C380600,  //  0139  CALL	R14	3
      0x4C380000,  //  013A  LDNIL	R14
      0x80041C00,  //  013B  RET	1	R14
      0x8C38112F,  //  013C  GETMET	R14	R8	K47
      0x5C401200,  //  013D  MOVE	R16	R9
      0x5C441400,  //  013E  MOVE	R17	R10
      0x7C380600,  //  013F  CALL	R14	3
      0x8C381130,  //  0140  GETMET	R14	R8	K48
      0x5C401600,  //  0141  MOVE	R16	R11
      0x7C380400,  //  0142  CALL	R14	2
      0x9022620C,  //  0143  SETMBR	R8	K49	R12
      0x9022640D,  //  0144  SETMBR	R8	K50	R13
      0xB83A0200,  //  0145  GETNGBL	R14	K1
      0x88381D02,  //  0146  GETMBR	R14	R14	K2
      0x8C381D33,  //  0147  GETMET	R14	R14	K51
      0x5C401200,  //  0148  MOVE	R16	R9
      0x7C380400,  //  0149  CALL	R14	2
      0x8C3C1D34,  //  014A  GETMET	R15	R14	K52
      0x54460005,  //  014B  LDINT	R17	6
      0x7C3C0400,  //  014C  CALL	R15	2
      0x8C401F07,  //  014D  GETMET	R16	R15	K7
      0x544A0014,  //  014E  LDINT	R18	21
      0x7C400400,  //  014F  CALL	R16	2
      0x8C441F07,  //  0150  GETMET	R17	R15	K7
      0x544E0010,  //  0151  LDINT	R19	17
      0x7C440400,  //  0152  CALL	R17	2
      0x5C482000,  //  0153  MOVE	R18	R16
      0x784A0001,  //  0154  JMPF	R18	#0157
      0x5C482200,  //  0155  MOVE	R18	R17
      0x744A0006,  //  0156  JMPT	R18	#015E
      0xB84A3A00,  //  0157  GETNGBL	R18	K29
      0x8C482522,  //  0158  GETMET	R18	R18	K34
      0x58500035,  //  0159  LDCONST	R20	K53
      0x5854000F,  //  015A  LDCONST	R21	K15
      0x7C480600,  //  015B  CALL	R18	3
      0x50480000,  //  015C  LDBOOL	R18	0	0
      0x80042400,  //  015D  RET	1	R18
      0x60480004,  //  015E  GETGBL	R18	G4
      0x5C4C2000,  //  015F  MOVE	R19	R16
      0x7C480200,  //  0160  CALL	R18	1
      0x1C482536,  //  0161  EQ	R18	R18	K54
      0x784A0006,  //  0162  JMPF	R18	#016A
      0xB84A6E00,  //  0163  GETNGBL	R18	K55
      0x5C4C2000,  //  0164  MOVE	R19	R16
      0x7C480200,  //  0165  CALL	R18	1
      0x8C482538,  //  0166  GETMET	R18	R18	K56
      0x7C480200,  //  0167  CALL	R18	1
      0x5C402400,  //  0168  MOVE	R16	R18
      0x70020002,  //  0169  JMP		#016D
      0x8C482138,  //  016A  GETMET	R18	R16	K56
      0x7C480200,  //  016B  CALL	R18	1
      0x5C402400,  //  016C  MOVE	R16	R18
      0x60480004,  //  016D  GETGBL	R18	G4
      0x5C4C2200,  //  016E  MOVE	R19	R17
      0x7C480200,  //  016F  CALL	R18	1
      0x1C482536,  //  0170  EQ	R18	R18	K54
      0x784A0006,  //  0171  JMPF	R18	#0179
      0xB84A6E00,  //  0172  GETNGBL	R18	K55
      0x5C4C2200,  //  0173  MOVE	R19	R17
      0x7C480200,  //  0174  CALL	R18	1
      0x8C482538,  //  0175  GETMET	R18	R18	K56
      0x7C480200,  //  0176  CALL	R18	1
      0x5C442400,  //  0177  MOVE	R17	R18
      0x70020002,  //  0178  JMP		#017C
      0x8C482338,  //  0179  GETMET	R18	R17	K56
      0x7C480200,  //  017A  CALL	R18	1
      0x5C442400,  //  017B  MOVE	R17	R18
      0xB84A0200,  //  017C  GETNGBL	R18	K1
      0x88482502,  //  017D  GETMBR	R18	R18	K2
      0x8C482533,  //  017E  GETMET	R18	R18	K51
      0x8C50112D,  //  017F  GETMET	R20	R8	K45
      0x7C500200,  //  0180  CALL	R20	1
      0x7C480400,  //  0181  CALL	R18	2
      0x8C482507,  //  0182  GETMET	R18	R18	K7
      0x54520008,  //  0183  LDINT	R20	9
      0x7C480400,  //  0184  CALL	R18	2
      0x404E1139,  //  0185  CONNECT	R19	K8	K57
      0x94482413,  //  0186  GETIDX	R18	R18	R19
      0x60500015,  //  0187  GETGBL	R20	G21
      0x7C500000,  //  0188  CALL	R20	0
      0x8C50293A,  //  0189  GETMET	R20	R20	K58
      0x5858003B,  //  018A  LDCONST	R22	K59
      0x7C500400,  //  018B  CALL	R20	2
      0x5C4C2800,  //  018C  MOVE	R19	R20
      0x8C50093C,  //  018D  GETMET	R20	R4	K60
      0x7C500200,  //  018E  CALL	R20	1
      0x8C54213D,  //  018F  GETMET	R21	R16	K61
      0x7C540200,  //  0190  CALL	R21	1
      0x8C542B3E,  //  0191  GETMET	R21	R21	K62
      0x7C540200,  //  0192  CALL	R21	1
      0x8C58293F,  //  0193  GETMET	R22	R20	K63
      0x5C602400,  //  0194  MOVE	R24	R18
      0x5C642A00,  //  0195  MOVE	R25	R21
      0x5C682600,  //  0196  MOVE	R26	R19
      0x546E0007,  //  0197  LDINT	R27	8
      0x7C580A00,  //  0198  CALL	R22	5
      0x8C5C1140,  //  0199  GETMET	R23	R8	K64
      0x5C642000,  //  019A  MOVE	R25	R16
      0x5C682200,  //  019B  MOVE	R26	R17
      0x5C6C2C00,  //  019C  MOVE	R27	R22
      0x7C5C0800,  //  019D  CALL	R23	4
      0x885C0113,  //  019E  GETMBR	R23	R0	K19
      0x8C5C2F41,  //  019F  GETMET	R23	R23	K65
      0x5C641000,  //  01A0  MOVE	R25	R8
      0x7C5C0400,  //  01A1  CALL	R23	2
      0x8C5C0B0A,  //  01A2  GETMET	R23	R5	K10
      0x7C5C0200,  //  01A3  CALL	R23	1
      0x8C602F0B,  //  01A4  GETMET	R24	R23	K11
      0x58680006,  //  01A5  LDCONST	R26	K6
      0x886C0B0C,  //  01A6  GETMBR	R27	R5	K12
      0xB8720200,  //  01A7  GETNGBL	R28	K1
      0x8870392C,  //  01A8  GETMBR	R28	R28	K44
      0x7C600800,  //  01A9  CALL	R24	4
      0x8C602F0B,  //  01AA  GETMET	R24	R23	K11
      0x58680008,  //  01AB  LDCONST	R26	K8
      0x886C0B0C,  //  01AC  GETMBR	R27	R5	K12
      0x58700008,  //  01AD  LDCONST	R28	K8
      0x7C600800,  //  01AE  CALL	R24	4
      0x54620007,  //  01AF  LDINT	R24	8
      0x900E0818,  //  01B0  SETMBR	R3	K4	R24
      0x80042E00,  //  01B1  RET	1	R23
      0x7002003A,  //  01B2  JMP		#01EE
      0x54260008,  //  01B3  LDINT	R9	9
      0x1C240E09,  //  01B4  EQ	R9	R7	R9
      0x7826000B,  //  01B5  JMPF	R9	#01C2
      0x8C240507,  //  01B6  GETMET	R9	R2	K7
      0x582C0006,  //  01B7  LDCONST	R11	K6
      0x7C240400,  //  01B8  CALL	R9	2
      0x8C281142,  //  01B9  GETMET	R10	R8	K66
      0x5C301200,  //  01BA  MOVE	R12	R9
      0x7C280400,  //  01BB  CALL	R10	2
      0xB82A0200,  //  01BC  GETNGBL	R10	K1
      0x8828152C,  //  01BD  GETMBR	R10	R10	K44
      0x900E2A0A,  //  01BE  SETMBR	R3	K21	R10
      0x4C280000,  //  01BF  LDNIL	R10
      0x80041400,  //  01C0  RET	1	R10
      0x7002002B,  //  01C1  JMP		#01EE
      0x54260009,  //  01C2  LDINT	R9	10
      0x1C240E09,  //  01C3  EQ	R9	R7	R9
      0x78260028,  //  01C4  JMPF	R9	#01EE
      0x8C240507,  //  01C5  GETMET	R9	R2	K7
      0x582C0006,  //  01C6  LDCONST	R11	K6
      0x7C240400,  //  01C7  CALL	R9	2
      0x88280113,  //  01C8  GETMBR	R10	R0	K19
      0x88281543,  //  01C9  GETMBR	R10	R10	K67
      0x8C281544,  //  01CA  GETMET	R10	R10	K68
      0x7C280200,  //  01CB  CALL	R10	1
      0x282C1308,  //  01CC  GE	R11	R9	K8
      0x782E001A,  //  01CD  JMPF	R11	#01E9
      0x602C000C,  //  01CE  GETGBL	R11	G12
      0x5C301400,  //  01CF  MOVE	R12	R10
      0x7C2C0200,  //  01D0  CALL	R11	1
      0x182C120B,  //  01D1  LE	R11	R9	R11
      0x782E0015,  //  01D2  JMPF	R11	#01E9
      0x042C1308,  //  01D3  SUB	R11	R9	K8
      0x942C140B,  //  01D4  GETIDX	R11	R10	R11
      0xB8323A00,  //  01D5  GETNGBL	R12	K29
      0x8C301922,  //  01D6  GETMET	R12	R12	K34
      0x88381146,  //  01D7  GETMBR	R14	R8	K70
      0x8C381D3D,  //  01D8  GETMET	R14	R14	K61
      0x7C380200,  //  01D9  CALL	R14	1
      0x8C381D3E,  //  01DA  GETMET	R14	R14	K62
      0x7C380200,  //  01DB  CALL	R14	1
      0x8C381D24,  //  01DC  GETMET	R14	R14	K36
      0x7C380200,  //  01DD  CALL	R14	1
      0x003A8A0E,  //  01DE  ADD	R14	K69	R14
      0x7C300400,  //  01DF  CALL	R12	2
      0x88300113,  //  01E0  GETMBR	R12	R0	K19
      0x88301943,  //  01E1  GETMBR	R12	R12	K67
      0x8C301947,  //  01E2  GETMET	R12	R12	K71
      0x7C300200,  //  01E3  CALL	R12	1
      0x88300113,  //  01E4  GETMBR	R12	R0	K19
      0x88301943,  //  01E5  GETMBR	R12	R12	K67
      0x8C301948,  //  01E6  GETMET	R12	R12	K72
      0x7C300200,  //  01E7  CALL	R12	1
      0x7001FFFF,  //  01E8  JMP		#01E9
      0xB82E0200,  //  01E9  GETNGBL	R11	K1
      0x882C172C,  //  01EA  GETMBR	R11	R11	K44
      0x900E2A0B,  //  01EB  SETMBR	R3	K21	R11
      0x4C2C0000,  //  01EC  LDNIL	R11
      0x80041600,  //  01ED  RET	1	R11
      0x80000000,  //  01EE  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_core_read_attribute,   /* name */
  be_nested_proto(
    24,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[82]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U8),
    /* K8   */  be_nested_str_weak(session),
    /* K9   */  be_nested_str_weak(breadcrumb),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(Matter_TLV_struct),
    /* K12  */  be_nested_str_weak(add_TLV),
    /* K13  */  be_nested_str_weak(U2),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(U1),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(BOOL),
    /* K18  */  be_nested_str_weak(Matter_TLV_array),
    /* K19  */  be_nested_str_weak(tasmota),
    /* K20  */  be_nested_str_weak(eth),
    /* K21  */  be_nested_str_weak(up),
    /* K22  */  be_nested_str_weak(add_struct),
    /* K23  */  be_nested_str_weak(UTF1),
    /* K24  */  be_nested_str_weak(ethernet),
    /* K25  */  be_nested_str_weak(NULL),
    /* K26  */  be_nested_str_weak(fromhex),
    /* K27  */  be_nested_str_weak(replace),
    /* K28  */  be_nested_str_weak(find),
    /* K29  */  be_nested_str_weak(mac),
    /* K30  */  be_nested_str_weak(),
    /* K31  */  be_nested_str_weak(_X3A),
    /* K32  */  be_nested_str_weak(B1),
    /* K33  */  be_nested_str_weak(add_array),
    /* K34  */  be_nested_str_weak(get_ip_bytes),
    /* K35  */  be_nested_str_weak(ip),
    /* K36  */  be_nested_str_weak(ip6local),
    /* K37  */  be_nested_str_weak(ip6),
    /* K38  */  be_nested_str_weak(wifi),
    /* K39  */  be_nested_str_weak(cmd),
    /* K40  */  be_nested_str_weak(Status_X201),
    /* K41  */  be_nested_str_weak(StatusPRM),
    /* K42  */  be_nested_str_weak(BootCount),
    /* K43  */  be_nested_str_weak(U4),
    /* K44  */  be_nested_str_weak(Status_X2011),
    /* K45  */  be_nested_str_weak(StatusSTS),
    /* K46  */  be_nested_str_weak(UptimeSec),
    /* K47  */  be_nested_str_weak(int64),
    /* K48  */  be_nested_str_weak(rtc),
    /* K49  */  be_nested_str_weak(utc),
    /* K50  */  be_const_int(1000000),
    /* K51  */  be_nested_str_weak(local),
    /* K52  */  be_nested_str_weak(device),
    /* K53  */  be_nested_str_weak(sessions),
    /* K54  */  be_nested_str_weak(sessions_active),
    /* K55  */  be_nested_str_weak(B2),
    /* K56  */  be_nested_str_weak(noc),
    /* K57  */  be_nested_str_weak(icac),
    /* K58  */  be_nested_str_weak(stop_iteration),
    /* K59  */  be_nested_str_weak(parse),
    /* K60  */  be_nested_str_weak(get_ca),
    /* K61  */  be_nested_str_weak(findsubval),
    /* K62  */  be_nested_str_weak(admin_vendor),
    /* K63  */  be_nested_str_weak(fabric),
    /* K64  */  be_nested_str_weak(deviceid),
    /* K65  */  be_nested_str_weak(fabric_label),
    /* K66  */  be_nested_str_weak(Tasmota),
    /* K67  */  be_nested_str_weak(vendorid),
    /* K68  */  be_nested_str_weak(DeviceName),
    /* K69  */  be_nested_str_weak(FriendlyName),
    /* K70  */  be_nested_str_weak(FriendlyName1),
    /* K71  */  be_nested_str_weak(XX),
    /* K72  */  be_nested_str_weak(Status_X202),
    /* K73  */  be_nested_str_weak(StatusFWR),
    /* K74  */  be_nested_str_weak(Hardware),
    /* K75  */  be_nested_str_weak(Version),
    /* K76  */  be_nested_str_weak(locale),
    /* K77  */  be_nested_str_weak(Matter_TLV_list),
    /* K78  */  be_nested_str_weak(get_cluster_list),
    /* K79  */  be_nested_str_weak(get_active_endpoints),
    /* K80  */  be_nested_str_weak(status),
    /* K81  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[736]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E002F,  //  0005  LDINT	R7	48
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0031,  //  0007  JMPF	R7	#003A
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E0006,  //  0009  JMPF	R7	#0011
      0x8C1C0906,  //  000A  GETMET	R7	R4	K6
      0x88240907,  //  000B  GETMBR	R9	R4	K7
      0x88280308,  //  000C  GETMBR	R10	R1	K8
      0x88281509,  //  000D  GETMBR	R10	R10	K9
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x70020027,  //  0010  JMP		#0039
      0x1C1C0D0A,  //  0011  EQ	R7	R6	K10
      0x781E000D,  //  0012  JMPF	R7	#0021
      0x8C1C090B,  //  0013  GETMET	R7	R4	K11
      0x7C1C0200,  //  0014  CALL	R7	1
      0x8C200F0C,  //  0015  GETMET	R8	R7	K12
      0x58280005,  //  0016  LDCONST	R10	K5
      0x882C090D,  //  0017  GETMBR	R11	R4	K13
      0x5432003B,  //  0018  LDINT	R12	60
      0x7C200800,  //  0019  CALL	R8	4
      0x8C200F0C,  //  001A  GETMET	R8	R7	K12
      0x5828000A,  //  001B  LDCONST	R10	K10
      0x882C090D,  //  001C  GETMBR	R11	R4	K13
      0x54320383,  //  001D  LDINT	R12	900
      0x7C200800,  //  001E  CALL	R8	4
      0x80040E00,  //  001F  RET	1	R7
      0x70020017,  //  0020  JMP		#0039
      0x1C1C0D0E,  //  0021  EQ	R7	R6	K14
      0x781E0005,  //  0022  JMPF	R7	#0029
      0x8C1C0906,  //  0023  GETMET	R7	R4	K6
      0x8824090F,  //  0024  GETMBR	R9	R4	K15
      0x5828000E,  //  0025  LDCONST	R10	K14
      0x7C1C0600,  //  0026  CALL	R7	3
      0x80040E00,  //  0027  RET	1	R7
      0x7002000F,  //  0028  JMP		#0039
      0x1C1C0D10,  //  0029  EQ	R7	R6	K16
      0x781E0005,  //  002A  JMPF	R7	#0031
      0x8C1C0906,  //  002B  GETMET	R7	R4	K6
      0x8824090F,  //  002C  GETMBR	R9	R4	K15
      0x5828000E,  //  002D  LDCONST	R10	K14
      0x7C1C0600,  //  002E  CALL	R7	3
      0x80040E00,  //  002F  RET	1	R7
      0x70020007,  //  0030  JMP		#0039
      0x541E0003,  //  0031  LDINT	R7	4
      0x1C1C0C07,  //  0032  EQ	R7	R6	R7
      0x781E0004,  //  0033  JMPF	R7	#0039
      0x8C1C0906,  //  0034  GETMET	R7	R4	K6
      0x88240911,  //  0035  GETMBR	R9	R4	K17
      0x50280000,  //  0036  LDBOOL	R10	0	0
      0x7C1C0600,  //  0037  CALL	R7	3
      0x80040E00,  //  0038  RET	1	R7
      0x700202A4,  //  0039  JMP		#02DF
      0x541E0031,  //  003A  LDINT	R7	50
      0x1C1C0A07,  //  003B  EQ	R7	R5	R7
      0x781E0000,  //  003C  JMPF	R7	#003E
      0x700202A0,  //  003D  JMP		#02DF
      0x541E0032,  //  003E  LDINT	R7	51
      0x1C1C0A07,  //  003F  EQ	R7	R5	R7
      0x781E00DA,  //  0040  JMPF	R7	#011C
      0x1C1C0D05,  //  0041  EQ	R7	R6	K5
      0x781E00B5,  //  0042  JMPF	R7	#00F9
      0x8C1C0912,  //  0043  GETMET	R7	R4	K18
      0x7C1C0200,  //  0044  CALL	R7	1
      0xB8222600,  //  0045  GETNGBL	R8	K19
      0x8C201114,  //  0046  GETMET	R8	R8	K20
      0x7C200200,  //  0047  CALL	R8	1
      0x94241115,  //  0048  GETIDX	R9	R8	K21
      0x78260053,  //  0049  JMPF	R9	#009E
      0x8C240F16,  //  004A  GETMET	R9	R7	K22
      0x4C2C0000,  //  004B  LDNIL	R11
      0x7C240400,  //  004C  CALL	R9	2
      0x8C28130C,  //  004D  GETMET	R10	R9	K12
      0x58300005,  //  004E  LDCONST	R12	K5
      0x88340917,  //  004F  GETMBR	R13	R4	K23
      0x58380018,  //  0050  LDCONST	R14	K24
      0x7C280800,  //  0051  CALL	R10	4
      0x8C28130C,  //  0052  GETMET	R10	R9	K12
      0x5830000A,  //  0053  LDCONST	R12	K10
      0x88340911,  //  0054  GETMBR	R13	R4	K17
      0x5838000A,  //  0055  LDCONST	R14	K10
      0x7C280800,  //  0056  CALL	R10	4
      0x8C28130C,  //  0057  GETMET	R10	R9	K12
      0x5830000E,  //  0058  LDCONST	R12	K14
      0x88340911,  //  0059  GETMBR	R13	R4	K17
      0x5838000A,  //  005A  LDCONST	R14	K10
      0x7C280800,  //  005B  CALL	R10	4
      0x8C28130C,  //  005C  GETMET	R10	R9	K12
      0x58300010,  //  005D  LDCONST	R12	K16
      0x88340919,  //  005E  GETMBR	R13	R4	K25
      0x4C380000,  //  005F  LDNIL	R14
      0x7C280800,  //  0060  CALL	R10	4
      0x60280015,  //  0061  GETGBL	R10	G21
      0x7C280000,  //  0062  CALL	R10	0
      0x8C28151A,  //  0063  GETMET	R10	R10	K26
      0x8C30071B,  //  0064  GETMET	R12	R3	K27
      0x8C38111C,  //  0065  GETMET	R14	R8	K28
      0x5840001D,  //  0066  LDCONST	R16	K29
      0x5844001E,  //  0067  LDCONST	R17	K30
      0x7C380600,  //  0068  CALL	R14	3
      0x583C001F,  //  0069  LDCONST	R15	K31
      0x5840001E,  //  006A  LDCONST	R16	K30
      0x7C300800,  //  006B  CALL	R12	4
      0x7C280400,  //  006C  CALL	R10	2
      0x8C2C130C,  //  006D  GETMET	R11	R9	K12
      0x54360003,  //  006E  LDINT	R13	4
      0x88380920,  //  006F  GETMBR	R14	R4	K32
      0x5C3C1400,  //  0070  MOVE	R15	R10
      0x7C2C0800,  //  0071  CALL	R11	4
      0x8C2C1321,  //  0072  GETMET	R11	R9	K33
      0x54360004,  //  0073  LDINT	R13	5
      0x7C2C0400,  //  0074  CALL	R11	2
      0x8C30170C,  //  0075  GETMET	R12	R11	K12
      0x4C380000,  //  0076  LDNIL	R14
      0x883C0920,  //  0077  GETMBR	R15	R4	K32
      0xB8420200,  //  0078  GETNGBL	R16	K1
      0x8C402122,  //  0079  GETMET	R16	R16	K34
      0x8C48111C,  //  007A  GETMET	R18	R8	K28
      0x58500023,  //  007B  LDCONST	R20	K35
      0x5854001E,  //  007C  LDCONST	R21	K30
      0x7C480600,  //  007D  CALL	R18	3
      0x7C400400,  //  007E  CALL	R16	2
      0x7C300800,  //  007F  CALL	R12	4
      0x8C301321,  //  0080  GETMET	R12	R9	K33
      0x543A0005,  //  0081  LDINT	R14	6
      0x7C300400,  //  0082  CALL	R12	2
      0x8C34190C,  //  0083  GETMET	R13	R12	K12
      0x4C3C0000,  //  0084  LDNIL	R15
      0x88400920,  //  0085  GETMBR	R16	R4	K32
      0xB8460200,  //  0086  GETNGBL	R17	K1
      0x8C442322,  //  0087  GETMET	R17	R17	K34
      0x8C4C111C,  //  0088  GETMET	R19	R8	K28
      0x58540024,  //  0089  LDCONST	R21	K36
      0x5858001E,  //  008A  LDCONST	R22	K30
      0x7C4C0600,  //  008B  CALL	R19	3
      0x7C440400,  //  008C  CALL	R17	2
      0x7C340800,  //  008D  CALL	R13	4
      0x8C34190C,  //  008E  GETMET	R13	R12	K12
      0x4C3C0000,  //  008F  LDNIL	R15
      0x88400920,  //  0090  GETMBR	R16	R4	K32
      0xB8460200,  //  0091  GETNGBL	R17	K1
      0x8C442322,  //  0092  GETMET	R17	R17	K34
      0x8C4C111C,  //  0093  GETMET	R19	R8	K28
      0x58540025,  //  0094  LDCONST	R21	K37
      0x5858001E,  //  0095  LDCONST	R22	K30
      0x7C4C0600,  //  0096  CALL	R19	3
      0x7C440400,  //  0097  CALL	R17	2
      0x7C340800,  //  0098  CALL	R13	4
      0x8C34130C,  //  0099  GETMET	R13	R9	K12
      0x543E0006,  //  009A  LDINT	R15	7
      0x8840090F,  //  009B  GETMBR	R16	R4	K15
      0x5844000E,  //  009C  LDCONST	R17	K14
      0x7C340800,  //  009D  CALL	R13	4
      0xB8262600,  //  009E  GETNGBL	R9	K19
      0x8C241326,  //  009F  GETMET	R9	R9	K38
      0x7C240200,  //  00A0  CALL	R9	1
      0x94281315,  //  00A1  GETIDX	R10	R9	K21
      0x782A0053,  //  00A2  JMPF	R10	#00F7
      0x8C280F16,  //  00A3  GETMET	R10	R7	K22
      0x4C300000,  //  00A4  LDNIL	R12
      0x7C280400,  //  00A5  CALL	R10	2
      0x8C2C150C,  //  00A6  GETMET	R11	R10	K12
      0x58340005,  //  00A7  LDCONST	R13	K5
      0x88380917,  //  00A8  GETMBR	R14	R4	K23
      0x583C0026,  //  00A9  LDCONST	R15	K38
      0x7C2C0800,  //  00AA  CALL	R11	4
      0x8C2C150C,  //  00AB  GETMET	R11	R10	K12
      0x5834000A,  //  00AC  LDCONST	R13	K10
      0x88380911,  //  00AD  GETMBR	R14	R4	K17
      0x583C000A,  //  00AE  LDCONST	R15	K10
      0x7C2C0800,  //  00AF  CALL	R11	4
      0x8C2C150C,  //  00B0  GETMET	R11	R10	K12
      0x5834000E,  //  00B1  LDCONST	R13	K14
      0x88380911,  //  00B2  GETMBR	R14	R4	K17
      0x583C000A,  //  00B3  LDCONST	R15	K10
      0x7C2C0800,  //  00B4  CALL	R11	4
      0x8C2C150C,  //  00B5  GETMET	R11	R10	K12
      0x58340010,  //  00B6  LDCONST	R13	K16
      0x88380919,  //  00B7  GETMBR	R14	R4	K25
      0x4C3C0000,  //  00B8  LDNIL	R15
      0x7C2C0800,  //  00B9  CALL	R11	4
      0x602C0015,  //  00BA  GETGBL	R11	G21
      0x7C2C0000,  //  00BB  CALL	R11	0
      0x8C2C171A,  //  00BC  GETMET	R11	R11	K26
      0x8C34071B,  //  00BD  GETMET	R13	R3	K27
      0x8C3C131C,  //  00BE  GETMET	R15	R9	K28
      0x5844001D,  //  00BF  LDCONST	R17	K29
      0x5848001E,  //  00C0  LDCONST	R18	K30
      0x7C3C0600,  //  00C1  CALL	R15	3
      0x5840001F,  //  00C2  LDCONST	R16	K31
      0x5844001E,  //  00C3  LDCONST	R17	K30
      0x7C340800,  //  00C4  CALL	R13	4
      0x7C2C0400,  //  00C5  CALL	R11	2
      0x8C30150C,  //  00C6  GETMET	R12	R10	K12
      0x543A0003,  //  00C7  LDINT	R14	4
      0x883C0920,  //  00C8  GETMBR	R15	R4	K32
      0x5C401600,  //  00C9  MOVE	R16	R11
      0x7C300800,  //  00CA  CALL	R12	4
      0x8C301521,  //  00CB  GETMET	R12	R10	K33
      0x543A0004,  //  00CC  LDINT	R14	5
      0x7C300400,  //  00CD  CALL	R12	2
      0x8C34190C,  //  00CE  GETMET	R13	R12	K12
      0x4C3C0000,  //  00CF  LDNIL	R15
      0x88400920,  //  00D0  GETMBR	R16	R4	K32
      0xB8460200,  //  00D1  GETNGBL	R17	K1
      0x8C442322,  //  00D2  GETMET	R17	R17	K34
      0x8C4C131C,  //  00D3  GETMET	R19	R9	K28
      0x58540023,  //  00D4  LDCONST	R21	K35
      0x5858001E,  //  00D5  LDCONST	R22	K30
      0x7C4C0600,  //  00D6  CALL	R19	3
      0x7C440400,  //  00D7  CALL	R17	2
      0x7C340800,  //  00D8  CALL	R13	4
      0x8C341521,  //  00D9  GETMET	R13	R10	K33
      0x543E0005,  //  00DA  LDINT	R15	6
      0x7C340400,  //  00DB  CALL	R13	2
      0x8C381B0C,  //  00DC  GETMET	R14	R13	K12
      0x4C400000,  //  00DD  LDNIL	R16
      0x88440920,  //  00DE  GETMBR	R17	R4	K32
      0xB84A0200,  //  00DF  GETNGBL	R18	K1
      0x8C482522,  //  00E0  GETMET	R18	R18	K34
      0x8C50131C,  //  00E1  GETMET	R20	R9	K28
      0x58580024,  //  00E2  LDCONST	R22	K36
      0x585C001E,  //  00E3  LDCONST	R23	K30
      0x7C500600,  //  00E4  CALL	R20	3
      0x7C480400,  //  00E5  CALL	R18	2
      0x7C380800,  //  00E6  CALL	R14	4
      0x8C381B0C,  //  00E7  GETMET	R14	R13	K12
      0x4C400000,  //  00E8  LDNIL	R16
      0x88440920,  //  00E9  GETMBR	R17	R4	K32
      0xB84A0200,  //  00EA  GETNGBL	R18	K1
      0x8C482522,  //  00EB  GETMET	R18	R18	K34
      0x8C50131C,  //  00EC  GETMET	R20	R9	K28
      0x58580025,  //  00ED  LDCONST	R22	K37
      0x585C001E,  //  00EE  LDCONST	R23	K30
      0x7C500600,  //  00EF  CALL	R20	3
      0x7C480400,  //  00F0  CALL	R18	2
      0x7C380800,  //  00F1  CALL	R14	4
      0x8C38150C,  //  00F2  GETMET	R14	R10	K12
      0x54420006,  //  00F3  LDINT	R16	7
      0x8844090F,  //  00F4  GETMBR	R17	R4	K15
      0x5848000A,  //  00F5  LDCONST	R18	K10
      0x7C380800,  //  00F6  CALL	R14	4
      0x80040E00,  //  00F7  RET	1	R7
      0x70020021,  //  00F8  JMP		#011B
      0x1C1C0D0A,  //  00F9  EQ	R7	R6	K10
      0x781E000A,  //  00FA  JMPF	R7	#0106
      0x8C1C0906,  //  00FB  GETMET	R7	R4	K6
      0x8824090D,  //  00FC  GETMBR	R9	R4	K13
      0xB82A2600,  //  00FD  GETNGBL	R10	K19
      0x8C281527,  //  00FE  GETMET	R10	R10	K39
      0x58300028,  //  00FF  LDCONST	R12	K40
      0x7C280400,  //  0100  CALL	R10	2
      0x94281529,  //  0101  GETIDX	R10	R10	K41
      0x9428152A,  //  0102  GETIDX	R10	R10	K42
      0x7C1C0600,  //  0103  CALL	R7	3
      0x80040E00,  //  0104  RET	1	R7
      0x70020014,  //  0105  JMP		#011B
      0x1C1C0D0E,  //  0106  EQ	R7	R6	K14
      0x781E000A,  //  0107  JMPF	R7	#0113
      0x8C1C0906,  //  0108  GETMET	R7	R4	K6
      0x8824092B,  //  0109  GETMBR	R9	R4	K43
      0xB82A2600,  //  010A  GETNGBL	R10	K19
      0x8C281527,  //  010B  GETMET	R10	R10	K39
      0x5830002C,  //  010C  LDCONST	R12	K44
      0x7C280400,  //  010D  CALL	R10	2
      0x9428152D,  //  010E  GETIDX	R10	R10	K45
      0x9428152E,  //  010F  GETIDX	R10	R10	K46
      0x7C1C0600,  //  0110  CALL	R7	3
      0x80040E00,  //  0111  RET	1	R7
      0x70020007,  //  0112  JMP		#011B
      0x541E0007,  //  0113  LDINT	R7	8
      0x1C1C0C07,  //  0114  EQ	R7	R6	R7
      0x781E0004,  //  0115  JMPF	R7	#011B
      0x8C1C0906,  //  0116  GETMET	R7	R4	K6
      0x88240911,  //  0117  GETMBR	R9	R4	K17
      0x50280000,  //  0118  LDBOOL	R10	0	0
      0x7C1C0600,  //  0119  CALL	R7	3
      0x80040E00,  //  011A  RET	1	R7
      0x700201C2,  //  011B  JMP		#02DF
      0x541E0033,  //  011C  LDINT	R7	52
      0x1C1C0A07,  //  011D  EQ	R7	R5	R7
      0x781E0000,  //  011E  JMPF	R7	#0120
      0x700201BE,  //  011F  JMP		#02DF
      0x541E0037,  //  0120  LDINT	R7	56
      0x1C1C0A07,  //  0121  EQ	R7	R5	R7
      0x781E002C,  //  0122  JMPF	R7	#0150
      0x1C1C0D05,  //  0123  EQ	R7	R6	K5
      0x781E000F,  //  0124  JMPF	R7	#0135
      0xB81E5E00,  //  0125  GETNGBL	R7	K47
      0xB8222600,  //  0126  GETNGBL	R8	K19
      0x8C201130,  //  0127  GETMET	R8	R8	K48
      0x7C200200,  //  0128  CALL	R8	1
      0x94201131,  //  0129  GETIDX	R8	R8	K49
      0x7C1C0200,  //  012A  CALL	R7	1
      0xB8225E00,  //  012B  GETNGBL	R8	K47
      0x58240032,  //  012C  LDCONST	R9	K50
      0x7C200200,  //  012D  CALL	R8	1
      0x081C0E08,  //  012E  MUL	R7	R7	R8
      0x8C200906,  //  012F  GETMET	R8	R4	K6
      0x88280907,  //  0130  GETMBR	R10	R4	K7
      0x5C2C0E00,  //  0131  MOVE	R11	R7
      0x7C200600,  //  0132  CALL	R8	3
      0x80041000,  //  0133  RET	1	R8
      0x70020019,  //  0134  JMP		#014F
      0x1C1C0D0A,  //  0135  EQ	R7	R6	K10
      0x781E0005,  //  0136  JMPF	R7	#013D
      0x8C1C0906,  //  0137  GETMET	R7	R4	K6
      0x8824090F,  //  0138  GETMBR	R9	R4	K15
      0x58280010,  //  0139  LDCONST	R10	K16
      0x7C1C0600,  //  013A  CALL	R7	3
      0x80040E00,  //  013B  RET	1	R7
      0x70020011,  //  013C  JMP		#014F
      0x541E0006,  //  013D  LDINT	R7	7
      0x1C1C0C07,  //  013E  EQ	R7	R6	R7
      0x781E000E,  //  013F  JMPF	R7	#014F
      0xB81E5E00,  //  0140  GETNGBL	R7	K47
      0xB8222600,  //  0141  GETNGBL	R8	K19
      0x8C201130,  //  0142  GETMET	R8	R8	K48
      0x7C200200,  //  0143  CALL	R8	1
      0x94201133,  //  0144  GETIDX	R8	R8	K51
      0x7C1C0200,  //  0145  CALL	R7	1
      0xB8225E00,  //  0146  GETNGBL	R8	K47
      0x58240032,  //  0147  LDCONST	R9	K50
      0x7C200200,  //  0148  CALL	R8	1
      0x081C0E08,  //  0149  MUL	R7	R7	R8
      0x8C200906,  //  014A  GETMET	R8	R4	K6
      0x88280907,  //  014B  GETMBR	R10	R4	K7
      0x5C2C0E00,  //  014C  MOVE	R11	R7
      0x7C200600,  //  014D  CALL	R8	3
      0x80041000,  //  014E  RET	1	R8
      0x7002018E,  //  014F  JMP		#02DF
      0x541E003D,  //  0150  LDINT	R7	62
      0x1C1C0A07,  //  0151  EQ	R7	R5	R7
      0x781E0082,  //  0152  JMPF	R7	#01D6
      0x1C1C0D05,  //  0153  EQ	R7	R6	K5
      0x781E001D,  //  0154  JMPF	R7	#0173
      0x8C1C0912,  //  0155  GETMET	R7	R4	K18
      0x7C1C0200,  //  0156  CALL	R7	1
      0x60200010,  //  0157  GETGBL	R8	G16
      0x88240134,  //  0158  GETMBR	R9	R0	K52
      0x88241335,  //  0159  GETMBR	R9	R9	K53
      0x8C241336,  //  015A  GETMET	R9	R9	K54
      0x7C240200,  //  015B  CALL	R9	1
      0x7C200200,  //  015C  CALL	R8	1
      0xA802000F,  //  015D  EXBLK	0	#016E
      0x5C241000,  //  015E  MOVE	R9	R8
      0x7C240000,  //  015F  CALL	R9	0
      0x8C280F16,  //  0160  GETMET	R10	R7	K22
      0x4C300000,  //  0161  LDNIL	R12
      0x7C280400,  //  0162  CALL	R10	2
      0x8C2C150C,  //  0163  GETMET	R11	R10	K12
      0x5834000A,  //  0164  LDCONST	R13	K10
      0x88380937,  //  0165  GETMBR	R14	R4	K55
      0x883C1338,  //  0166  GETMBR	R15	R9	K56
      0x7C2C0800,  //  0167  CALL	R11	4
      0x8C2C150C,  //  0168  GETMET	R11	R10	K12
      0x5834000E,  //  0169  LDCONST	R13	K14
      0x88380937,  //  016A  GETMBR	R14	R4	K55
      0x883C1339,  //  016B  GETMBR	R15	R9	K57
      0x7C2C0800,  //  016C  CALL	R11	4
      0x7001FFEF,  //  016D  JMP		#015E
      0x5820003A,  //  016E  LDCONST	R8	K58
      0xAC200200,  //  016F  CATCH	R8	1	0
      0xB0080000,  //  0170  RAISE	2	R0	R0
      0x80040E00,  //  0171  RET	1	R7
      0x70020061,  //  0172  JMP		#01D5
      0x1C1C0D0A,  //  0173  EQ	R7	R6	K10
      0x781E0032,  //  0174  JMPF	R7	#01A8
      0x8C1C0912,  //  0175  GETMET	R7	R4	K18
      0x7C1C0200,  //  0176  CALL	R7	1
      0x60200010,  //  0177  GETGBL	R8	G16
      0x88240134,  //  0178  GETMBR	R9	R0	K52
      0x88241335,  //  0179  GETMBR	R9	R9	K53
      0x8C241336,  //  017A  GETMET	R9	R9	K54
      0x7C240200,  //  017B  CALL	R9	1
      0x7C200200,  //  017C  CALL	R8	1
      0xA8020024,  //  017D  EXBLK	0	#01A3
      0x5C241000,  //  017E  MOVE	R9	R8
      0x7C240000,  //  017F  CALL	R9	0
      0x8C28093B,  //  0180  GETMET	R10	R4	K59
      0x8C30133C,  //  0181  GETMET	R12	R9	K60
      0x7C300200,  //  0182  CALL	R12	1
      0x7C280400,  //  0183  CALL	R10	2
      0x8C2C0F16,  //  0184  GETMET	R11	R7	K22
      0x4C340000,  //  0185  LDNIL	R13
      0x7C2C0400,  //  0186  CALL	R11	2
      0x8C30170C,  //  0187  GETMET	R12	R11	K12
      0x5838000A,  //  0188  LDCONST	R14	K10
      0x883C0937,  //  0189  GETMBR	R15	R4	K55
      0x8C40153D,  //  018A  GETMET	R16	R10	K61
      0x544A0008,  //  018B  LDINT	R18	9
      0x7C400400,  //  018C  CALL	R16	2
      0x7C300800,  //  018D  CALL	R12	4
      0x8C30170C,  //  018E  GETMET	R12	R11	K12
      0x5838000E,  //  018F  LDCONST	R14	K14
      0x883C090D,  //  0190  GETMBR	R15	R4	K13
      0x8840133E,  //  0191  GETMBR	R16	R9	K62
      0x7C300800,  //  0192  CALL	R12	4
      0x8C30170C,  //  0193  GETMET	R12	R11	K12
      0x58380010,  //  0194  LDCONST	R14	K16
      0x883C0907,  //  0195  GETMBR	R15	R4	K7
      0x8840133F,  //  0196  GETMBR	R16	R9	K63
      0x7C300800,  //  0197  CALL	R12	4
      0x8C30170C,  //  0198  GETMET	R12	R11	K12
      0x543A0003,  //  0199  LDINT	R14	4
      0x883C0907,  //  019A  GETMBR	R15	R4	K7
      0x88401340,  //  019B  GETMBR	R16	R9	K64
      0x7C300800,  //  019C  CALL	R12	4
      0x8C30170C,  //  019D  GETMET	R12	R11	K12
      0x543A0004,  //  019E  LDINT	R14	5
      0x883C0917,  //  019F  GETMBR	R15	R4	K23
      0x88401341,  //  01A0  GETMBR	R16	R9	K65
      0x7C300800,  //  01A1  CALL	R12	4
      0x7001FFDA,  //  01A2  JMP		#017E
      0x5820003A,  //  01A3  LDCONST	R8	K58
      0xAC200200,  //  01A4  CATCH	R8	1	0
      0xB0080000,  //  01A5  RAISE	2	R0	R0
      0x80040E00,  //  01A6  RET	1	R7
      0x7002002C,  //  01A7  JMP		#01D5
      0x1C1C0D0E,  //  01A8  EQ	R7	R6	K14
      0x781E0005,  //  01A9  JMPF	R7	#01B0
      0x8C1C0906,  //  01AA  GETMET	R7	R4	K6
      0x8824090F,  //  01AB  GETMBR	R9	R4	K15
      0x542A0004,  //  01AC  LDINT	R10	5
      0x7C1C0600,  //  01AD  CALL	R7	3
      0x80040E00,  //  01AE  RET	1	R7
      0x70020024,  //  01AF  JMP		#01D5
      0x1C1C0D10,  //  01B0  EQ	R7	R6	K16
      0x781E000B,  //  01B1  JMPF	R7	#01BE
      0x881C0134,  //  01B2  GETMBR	R7	R0	K52
      0x881C0F35,  //  01B3  GETMBR	R7	R7	K53
      0x8C1C0F36,  //  01B4  GETMET	R7	R7	K54
      0x7C1C0200,  //  01B5  CALL	R7	1
      0x8C200906,  //  01B6  GETMET	R8	R4	K6
      0x8828090F,  //  01B7  GETMBR	R10	R4	K15
      0x602C000C,  //  01B8  GETGBL	R11	G12
      0x5C300E00,  //  01B9  MOVE	R12	R7
      0x7C2C0200,  //  01BA  CALL	R11	1
      0x7C200600,  //  01BB  CALL	R8	3
      0x80041000,  //  01BC  RET	1	R8
      0x70020016,  //  01BD  JMP		#01D5
      0x541E0003,  //  01BE  LDINT	R7	4
      0x1C1C0C07,  //  01BF  EQ	R7	R6	R7
      0x781E0000,  //  01C0  JMPF	R7	#01C2
      0x70020012,  //  01C1  JMP		#01D5
      0x541E0004,  //  01C2  LDINT	R7	5
      0x1C1C0C07,  //  01C3  EQ	R7	R6	R7
      0x781E000F,  //  01C4  JMPF	R7	#01D5
      0x881C0134,  //  01C5  GETMBR	R7	R0	K52
      0x881C0F35,  //  01C6  GETMBR	R7	R7	K53
      0x8C1C0F36,  //  01C7  GETMET	R7	R7	K54
      0x7C1C0200,  //  01C8  CALL	R7	1
      0x8C200F1C,  //  01C9  GETMET	R8	R7	K28
      0x88280308,  //  01CA  GETMBR	R10	R1	K8
      0x7C200400,  //  01CB  CALL	R8	2
      0x4C240000,  //  01CC  LDNIL	R9
      0x1C241009,  //  01CD  EQ	R9	R8	R9
      0x78260000,  //  01CE  JMPF	R9	#01D0
      0x58200005,  //  01CF  LDCONST	R8	K5
      0x8C240906,  //  01D0  GETMET	R9	R4	K6
      0x882C090F,  //  01D1  GETMBR	R11	R4	K15
      0x5C301000,  //  01D2  MOVE	R12	R8
      0x7C240600,  //  01D3  CALL	R9	3
      0x80041200,  //  01D4  RET	1	R9
      0x70020108,  //  01D5  JMP		#02DF
      0x541E003B,  //  01D6  LDINT	R7	60
      0x1C1C0A07,  //  01D7  EQ	R7	R5	R7
      0x781E0000,  //  01D8  JMPF	R7	#01DA
      0x70020104,  //  01D9  JMP		#02DF
      0x541E0027,  //  01DA  LDINT	R7	40
      0x1C1C0A07,  //  01DB  EQ	R7	R5	R7
      0x781E0071,  //  01DC  JMPF	R7	#024F
      0x1C1C0D05,  //  01DD  EQ	R7	R6	K5
      0x781E0005,  //  01DE  JMPF	R7	#01E5
      0x8C1C0906,  //  01DF  GETMET	R7	R4	K6
      0x8824090D,  //  01E0  GETMBR	R9	R4	K13
      0x58280005,  //  01E1  LDCONST	R10	K5
      0x7C1C0600,  //  01E2  CALL	R7	3
      0x80040E00,  //  01E3  RET	1	R7
      0x70020068,  //  01E4  JMP		#024E
      0x1C1C0D0A,  //  01E5  EQ	R7	R6	K10
      0x781E0005,  //  01E6  JMPF	R7	#01ED
      0x8C1C0906,  //  01E7  GETMET	R7	R4	K6
      0x88240917,  //  01E8  GETMBR	R9	R4	K23
      0x58280042,  //  01E9  LDCONST	R10	K66
      0x7C1C0600,  //  01EA  CALL	R7	3
      0x80040E00,  //  01EB  RET	1	R7
      0x70020060,  //  01EC  JMP		#024E
      0x1C1C0D0E,  //  01ED  EQ	R7	R6	K14
      0x781E0006,  //  01EE  JMPF	R7	#01F6
      0x8C1C0906,  //  01EF  GETMET	R7	R4	K6
      0x8824090D,  //  01F0  GETMBR	R9	R4	K13
      0x88280134,  //  01F1  GETMBR	R10	R0	K52
      0x88281543,  //  01F2  GETMBR	R10	R10	K67
      0x7C1C0600,  //  01F3  CALL	R7	3
      0x80040E00,  //  01F4  RET	1	R7
      0x70020057,  //  01F5  JMP		#024E
      0x1C1C0D10,  //  01F6  EQ	R7	R6	K16
      0x781E0009,  //  01F7  JMPF	R7	#0202
      0x8C1C0906,  //  01F8  GETMET	R7	R4	K6
      0x88240917,  //  01F9  GETMBR	R9	R4	K23
      0xB82A2600,  //  01FA  GETNGBL	R10	K19
      0x8C281527,  //  01FB  GETMET	R10	R10	K39
      0x58300044,  //  01FC  LDCONST	R12	K68
      0x7C280400,  //  01FD  CALL	R10	2
      0x94281544,  //  01FE  GETIDX	R10	R10	K68
      0x7C1C0600,  //  01FF  CALL	R7	3
      0x80040E00,  //  0200  RET	1	R7
      0x7002004B,  //  0201  JMP		#024E
      0x541E0003,  //  0202  LDINT	R7	4
      0x1C1C0C07,  //  0203  EQ	R7	R6	R7
      0x781E0005,  //  0204  JMPF	R7	#020B
      0x8C1C0906,  //  0205  GETMET	R7	R4	K6
      0x8824090D,  //  0206  GETMBR	R9	R4	K13
      0x542A7FFF,  //  0207  LDINT	R10	32768
      0x7C1C0600,  //  0208  CALL	R7	3
      0x80040E00,  //  0209  RET	1	R7
      0x70020042,  //  020A  JMP		#024E
      0x541E0004,  //  020B  LDINT	R7	5
      0x1C1C0C07,  //  020C  EQ	R7	R6	R7
      0x781E0009,  //  020D  JMPF	R7	#0218
      0x8C1C0906,  //  020E  GETMET	R7	R4	K6
      0x88240917,  //  020F  GETMBR	R9	R4	K23
      0xB82A2600,  //  0210  GETNGBL	R10	K19
      0x8C281527,  //  0211  GETMET	R10	R10	K39
      0x58300045,  //  0212  LDCONST	R12	K69
      0x7C280400,  //  0213  CALL	R10	2
      0x94281546,  //  0214  GETIDX	R10	R10	K70
      0x7C1C0600,  //  0215  CALL	R7	3
      0x80040E00,  //  0216  RET	1	R7
      0x70020035,  //  0217  JMP		#024E
      0x541E0005,  //  0218  LDINT	R7	6
      0x1C1C0C07,  //  0219  EQ	R7	R6	R7
      0x781E0005,  //  021A  JMPF	R7	#0221
      0x8C1C0906,  //  021B  GETMET	R7	R4	K6
      0x88240917,  //  021C  GETMBR	R9	R4	K23
      0x58280047,  //  021D  LDCONST	R10	K71
      0x7C1C0600,  //  021E  CALL	R7	3
      0x80040E00,  //  021F  RET	1	R7
      0x7002002C,  //  0220  JMP		#024E
      0x541E0006,  //  0221  LDINT	R7	7
      0x1C1C0C07,  //  0222  EQ	R7	R6	R7
      0x781E0005,  //  0223  JMPF	R7	#022A
      0x8C1C0906,  //  0224  GETMET	R7	R4	K6
      0x8824090D,  //  0225  GETMBR	R9	R4	K13
      0x58280005,  //  0226  LDCONST	R10	K5
      0x7C1C0600,  //  0227  CALL	R7	3
      0x80040E00,  //  0228  RET	1	R7
      0x70020023,  //  0229  JMP		#024E
      0x541E0007,  //  022A  LDINT	R7	8
      0x1C1C0C07,  //  022B  EQ	R7	R6	R7
      0x781E000A,  //  022C  JMPF	R7	#0238
      0x8C1C0906,  //  022D  GETMET	R7	R4	K6
      0x88240917,  //  022E  GETMBR	R9	R4	K23
      0xB82A2600,  //  022F  GETNGBL	R10	K19
      0x8C281527,  //  0230  GETMET	R10	R10	K39
      0x58300048,  //  0231  LDCONST	R12	K72
      0x7C280400,  //  0232  CALL	R10	2
      0x94281549,  //  0233  GETIDX	R10	R10	K73
      0x9428154A,  //  0234  GETIDX	R10	R10	K74
      0x7C1C0600,  //  0235  CALL	R7	3
      0x80040E00,  //  0236  RET	1	R7
      0x70020015,  //  0237  JMP		#024E
      0x541E0008,  //  0238  LDINT	R7	9
      0x1C1C0C07,  //  0239  EQ	R7	R6	R7
      0x781E0005,  //  023A  JMPF	R7	#0241
      0x8C1C0906,  //  023B  GETMET	R7	R4	K6
      0x8824090D,  //  023C  GETMBR	R9	R4	K13
      0x58280005,  //  023D  LDCONST	R10	K5
      0x7C1C0600,  //  023E  CALL	R7	3
      0x80040E00,  //  023F  RET	1	R7
      0x7002000C,  //  0240  JMP		#024E
      0x541E0009,  //  0241  LDINT	R7	10
      0x1C1C0C07,  //  0242  EQ	R7	R6	R7
      0x781E0009,  //  0243  JMPF	R7	#024E
      0x8C1C0906,  //  0244  GETMET	R7	R4	K6
      0x88240917,  //  0245  GETMBR	R9	R4	K23
      0xB82A2600,  //  0246  GETNGBL	R10	K19
      0x8C281527,  //  0247  GETMET	R10	R10	K39
      0x58300048,  //  0248  LDCONST	R12	K72
      0x7C280400,  //  0249  CALL	R10	2
      0x94281549,  //  024A  GETIDX	R10	R10	K73
      0x9428154B,  //  024B  GETIDX	R10	R10	K75
      0x7C1C0600,  //  024C  CALL	R7	3
      0x80040E00,  //  024D  RET	1	R7
      0x7002008F,  //  024E  JMP		#02DF
      0x541E003E,  //  024F  LDINT	R7	63
      0x1C1C0A07,  //  0250  EQ	R7	R5	R7
      0x781E0000,  //  0251  JMPF	R7	#0253
      0x7002008B,  //  0252  JMP		#02DF
      0x541E002A,  //  0253  LDINT	R7	43
      0x1C1C0A07,  //  0254  EQ	R7	R5	R7
      0x781E0016,  //  0255  JMPF	R7	#026D
      0x1C1C0D05,  //  0256  EQ	R7	R6	K5
      0x781E0007,  //  0257  JMPF	R7	#0260
      0x8C1C0906,  //  0258  GETMET	R7	R4	K6
      0x88240917,  //  0259  GETMBR	R9	R4	K23
      0xB82A2600,  //  025A  GETNGBL	R10	K19
      0x8C28154C,  //  025B  GETMET	R10	R10	K76
      0x7C280200,  //  025C  CALL	R10	1
      0x7C1C0600,  //  025D  CALL	R7	3
      0x80040E00,  //  025E  RET	1	R7
      0x7002000B,  //  025F  JMP		#026C
      0x1C1C0D0A,  //  0260  EQ	R7	R6	K10
      0x781E0009,  //  0261  JMPF	R7	#026C
      0x8C1C094D,  //  0262  GETMET	R7	R4	K77
      0x7C1C0200,  //  0263  CALL	R7	1
      0x8C200F0C,  //  0264  GETMET	R8	R7	K12
      0x4C280000,  //  0265  LDNIL	R10
      0x882C0917,  //  0266  GETMBR	R11	R4	K23
      0xB8322600,  //  0267  GETNGBL	R12	K19
      0x8C30194C,  //  0268  GETMET	R12	R12	K76
      0x7C300200,  //  0269  CALL	R12	1
      0x7C200800,  //  026A  CALL	R8	4
      0x80040E00,  //  026B  RET	1	R7
      0x70020071,  //  026C  JMP		#02DF
      0x541E002B,  //  026D  LDINT	R7	44
      0x1C1C0A07,  //  026E  EQ	R7	R5	R7
      0x781E001C,  //  026F  JMPF	R7	#028D
      0x1C1C0D05,  //  0270  EQ	R7	R6	K5
      0x781E0005,  //  0271  JMPF	R7	#0278
      0x8C1C0906,  //  0272  GETMET	R7	R4	K6
      0x8824090F,  //  0273  GETMBR	R9	R4	K15
      0x5828000A,  //  0274  LDCONST	R10	K10
      0x7C1C0600,  //  0275  CALL	R7	3
      0x80040E00,  //  0276  RET	1	R7
      0x70020013,  //  0277  JMP		#028C
      0x1C1C0D0A,  //  0278  EQ	R7	R6	K10
      0x781E0005,  //  0279  JMPF	R7	#0280
      0x8C1C0906,  //  027A  GETMET	R7	R4	K6
      0x8824090F,  //  027B  GETMBR	R9	R4	K15
      0x542A0003,  //  027C  LDINT	R10	4
      0x7C1C0600,  //  027D  CALL	R7	3
      0x80040E00,  //  027E  RET	1	R7
      0x7002000B,  //  027F  JMP		#028C
      0x1C1C0D0E,  //  0280  EQ	R7	R6	K14
      0x781E0009,  //  0281  JMPF	R7	#028C
      0x8C1C094D,  //  0282  GETMET	R7	R4	K77
      0x7C1C0200,  //  0283  CALL	R7	1
      0x8C200F0C,  //  0284  GETMET	R8	R7	K12
      0x4C280000,  //  0285  LDNIL	R10
      0x8C2C0906,  //  0286  GETMET	R11	R4	K6
      0x8834090F,  //  0287  GETMBR	R13	R4	K15
      0x543A0003,  //  0288  LDINT	R14	4
      0x7C2C0600,  //  0289  CALL	R11	3
      0x7C200600,  //  028A  CALL	R8	3
      0x80040E00,  //  028B  RET	1	R7
      0x70020051,  //  028C  JMP		#02DF
      0x541E0030,  //  028D  LDINT	R7	49
      0x1C1C0A07,  //  028E  EQ	R7	R5	R7
      0x781E0010,  //  028F  JMPF	R7	#02A1
      0x1C1C0D10,  //  0290  EQ	R7	R6	K16
      0x781E0005,  //  0291  JMPF	R7	#0298
      0x8C1C0906,  //  0292  GETMET	R7	R4	K6
      0x8824090F,  //  0293  GETMBR	R9	R4	K15
      0x542A001D,  //  0294  LDINT	R10	30
      0x7C1C0600,  //  0295  CALL	R7	3
      0x80040E00,  //  0296  RET	1	R7
      0x70020007,  //  0297  JMP		#02A0
      0x541EFFFB,  //  0298  LDINT	R7	65532
      0x1C1C0C07,  //  0299  EQ	R7	R6	R7
      0x781E0004,  //  029A  JMPF	R7	#02A0
      0x8C1C0906,  //  029B  GETMET	R7	R4	K6
      0x8824092B,  //  029C  GETMBR	R9	R4	K43
      0x58280005,  //  029D  LDCONST	R10	K5
      0x7C1C0600,  //  029E  CALL	R7	3
      0x80040E00,  //  029F  RET	1	R7
      0x7002003D,  //  02A0  JMP		#02DF
      0x541E001C,  //  02A1  LDINT	R7	29
      0x1C1C0A07,  //  02A2  EQ	R7	R5	R7
      0x781E0037,  //  02A3  JMPF	R7	#02DC
      0x1C1C0D05,  //  02A4  EQ	R7	R6	K5
      0x781E0000,  //  02A5  JMPF	R7	#02A7
      0x70020033,  //  02A6  JMP		#02DB
      0x1C1C0D0A,  //  02A7  EQ	R7	R6	K10
      0x781E0013,  //  02A8  JMPF	R7	#02BD
      0x8C1C0912,  //  02A9  GETMET	R7	R4	K18
      0x7C1C0200,  //  02AA  CALL	R7	1
      0x60200010,  //  02AB  GETGBL	R8	G16
      0x8C24014E,  //  02AC  GETMET	R9	R0	K78
      0x7C240200,  //  02AD  CALL	R9	1
      0x7C200200,  //  02AE  CALL	R8	1
      0xA8020007,  //  02AF  EXBLK	0	#02B8
      0x5C241000,  //  02B0  MOVE	R9	R8
      0x7C240000,  //  02B1  CALL	R9	0
      0x8C280F0C,  //  02B2  GETMET	R10	R7	K12
      0x4C300000,  //  02B3  LDNIL	R12
      0x8834092B,  //  02B4  GETMBR	R13	R4	K43
      0x5C381200,  //  02B5  MOVE	R14	R9
      0x7C280800,  //  02B6  CALL	R10	4
      0x7001FFF7,  //  02B7  JMP		#02B0
      0x5820003A,  //  02B8  LDCONST	R8	K58
      0xAC200200,  //  02B9  CATCH	R8	1	0
      0xB0080000,  //  02BA  RAISE	2	R0	R0
      0x80040E00,  //  02BB  RET	1	R7
      0x7002001D,  //  02BC  JMP		#02DB
      0x1C1C0D0E,  //  02BD  EQ	R7	R6	K14
      0x781E0003,  //  02BE  JMPF	R7	#02C3
      0x8C1C0912,  //  02BF  GETMET	R7	R4	K18
      0x7C1C0200,  //  02C0  CALL	R7	1
      0x80040E00,  //  02C1  RET	1	R7
      0x70020017,  //  02C2  JMP		#02DB
      0x1C1C0D10,  //  02C3  EQ	R7	R6	K16
      0x781E0015,  //  02C4  JMPF	R7	#02DB
      0x881C0134,  //  02C5  GETMBR	R7	R0	K52
      0x8C1C0F4F,  //  02C6  GETMET	R7	R7	K79
      0x50240200,  //  02C7  LDBOOL	R9	1	0
      0x7C1C0400,  //  02C8  CALL	R7	2
      0x8C200912,  //  02C9  GETMET	R8	R4	K18
      0x7C200200,  //  02CA  CALL	R8	1
      0x60240010,  //  02CB  GETGBL	R9	G16
      0x5C280E00,  //  02CC  MOVE	R10	R7
      0x7C240200,  //  02CD  CALL	R9	1
      0xA8020007,  //  02CE  EXBLK	0	#02D7
      0x5C281200,  //  02CF  MOVE	R10	R9
      0x7C280000,  //  02D0  CALL	R10	0
      0x8C2C110C,  //  02D1  GETMET	R11	R8	K12
      0x4C340000,  //  02D2  LDNIL	R13
      0x8838090D,  //  02D3  GETMBR	R14	R4	K13
      0x5C3C1400,  //  02D4  MOVE	R15	R10
      0x7C2C0800,  //  02D5  CALL	R11	4
      0x7001FFF7,  //  02D6  JMP		#02CF
      0x5824003A,  //  02D7  LDCONST	R9	K58
      0xAC240200,  //  02D8  CATCH	R9	1	0
      0xB0080000,  //  02D9  RAISE	2	R0	R0
      0x80041000,  //  02DA  RET	1	R8
      0x70020002,  //  02DB  JMP		#02DF
      0xB81E0200,  //  02DC  GETNGBL	R7	K1
      0x881C0F51,  //  02DD  GETMBR	R7	R7	K81
      0x900AA007,  //  02DE  SETMBR	R2	K80	R7
      0x80000000,  //  02DF  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_core_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(endpoints),
    /* K2   */  be_nested_str_weak(ENDPOINTS),
    /* K3   */  be_nested_str_weak(clusters),
    /* K4   */  be_nested_str_weak(CLUSTERS),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x90020202,  //  0007  SETMBR	R0	K1	R2
      0x88080104,  //  0008  GETMBR	R2	R0	K4
      0x90020602,  //  0009  SETMBR	R0	K3	R2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_core
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_core,
    0,
    &be_class_Matter_Plugin,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ENDPOINTS, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_core_read_attribute_closure) },
        { be_const_key_weak(invoke_request, 1), be_const_closure(Matter_Plugin_core_invoke_request_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_core_init_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(52, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(40, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(6),
        be_const_int(7),
        be_const_int(8),
        be_const_int(9),
    }))    ) } )) },
        { be_const_key_int(43, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
    }))    ) } )) },
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
    }))    ) } )) },
        { be_const_key_int(56, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(7),
    }))    ) } )) },
        { be_const_key_int(44, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
    }))    ) } )) },
        { be_const_key_int(62, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
    }))    ) } )) },
        { be_const_key_int(50, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(60, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(48, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
    }))    ) } )) },
        { be_const_key_int(49, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(65532),
    }))    ) } )) },
        { be_const_key_int(63, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(51, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(8),
    }))    ) } )) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_core)
);
/*******************************************************************/

void be_load_Matter_Plugin_core_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_core);
    be_setglobal(vm, "Matter_Plugin_core");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
