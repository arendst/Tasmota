/* Solidification of Matter_Plugin_Root.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Root;

/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Root_invoke_request,   /* name */
  be_nested_proto(
    30,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 10),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(device),
        /* K1   */  be_nested_str_weak(remove_fabric),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x88000100,  //  0001  GETMBR	R0	R0	K0
          0x8C000101,  //  0002  GETMET	R0	R0	K1
          0x68080001,  //  0003  GETUPV	R2	U1
          0x7C000400,  //  0004  CALL	R0	2
          0x80000000,  //  0005  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[102]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(_breadcrumb),
    /* K9   */  be_nested_str_weak(Matter_TLV_struct),
    /* K10  */  be_nested_str_weak(add_TLV),
    /* K11  */  be_nested_str_weak(U1),
    /* K12  */  be_nested_str_weak(UTF1),
    /* K13  */  be_nested_str_weak(),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(XX),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(send_ack_now),
    /* K18  */  be_nested_str_weak(msg),
    /* K19  */  be_nested_str_weak(_fabric),
    /* K20  */  be_nested_str_weak(fabric_completed),
    /* K21  */  be_nested_str_weak(set_no_expiration),
    /* K22  */  be_nested_str_weak(save),
    /* K23  */  be_nested_str_weak(device),
    /* K24  */  be_nested_str_weak(start_commissioning_complete_deferred),
    /* K25  */  be_nested_str_weak(context_error),
    /* K26  */  be_nested_str_weak(CommissioningComplete_X3A_X20no_X20fabric_X20attached),
    /* K27  */  be_nested_str_weak(status),
    /* K28  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K29  */  be_nested_str_weak(B2),
    /* K30  */  be_nested_str_weak(DAC_Cert_FFF1_8000),
    /* K31  */  be_nested_str_weak(PAI_Cert_FFF1),
    /* K32  */  be_nested_str_weak(CD_FFF1_8000),
    /* K33  */  be_nested_str_weak(B1),
    /* K34  */  be_nested_str_weak(U4),
    /* K35  */  be_nested_str_weak(tasmota),
    /* K36  */  be_nested_str_weak(rtc),
    /* K37  */  be_nested_str_weak(utc),
    /* K38  */  be_nested_str_weak(tlv2raw),
    /* K39  */  be_nested_str_weak(get_ac),
    /* K40  */  be_nested_str_weak(EC_P256),
    /* K41  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K42  */  be_nested_str_weak(DAC_Priv_FFF1_8000),
    /* K43  */  be_nested_str_weak(gen_CSR),
    /* K44  */  be_nested_str_weak(set_temp_ca),
    /* K45  */  be_nested_str_weak(SUCCESS),
    /* K46  */  be_nested_str_weak(log),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20AddNoc_X20Args_X3D),
    /* K48  */  be_nested_str_weak(get_temp_ca),
    /* K49  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20AdNOC_X20without_X20CA),
    /* K50  */  be_nested_str_weak(sessions),
    /* K51  */  be_nested_str_weak(create_fabric),
    /* K52  */  be_nested_str_weak(set_ca),
    /* K53  */  be_nested_str_weak(set_noc_icac),
    /* K54  */  be_nested_str_weak(set_ipk_epoch_key),
    /* K55  */  be_nested_str_weak(set_admin_subject_vendor),
    /* K56  */  be_nested_str_weak(set_pk),
    /* K57  */  be_nested_str_weak(get_pk),
    /* K58  */  be_nested_str_weak(parse),
    /* K59  */  be_nested_str_weak(findsub),
    /* K60  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20no_X20fabricid_X20nor_X20deviceid_X20in_X20NOC_X20certificate),
    /* K61  */  be_nested_str_weak(int),
    /* K62  */  be_nested_str_weak(int64),
    /* K63  */  be_nested_str_weak(fromu32),
    /* K64  */  be_nested_str_weak(tobytes),
    /* K65  */  be_nested_str_weak(get_temp_ca_pub),
    /* K66  */  be_const_int(2147483647),
    /* K67  */  be_nested_str_weak(fromstring),
    /* K68  */  be_nested_str_weak(CompressedFabric),
    /* K69  */  be_nested_str_weak(HKDF_SHA256),
    /* K70  */  be_nested_str_weak(copy),
    /* K71  */  be_nested_str_weak(reverse),
    /* K72  */  be_nested_str_weak(derive),
    /* K73  */  be_nested_str_weak(commissioning_admin_fabric),
    /* K74  */  be_nested_str_weak(set_fabric_device),
    /* K75  */  be_nested_str_weak(fabric_candidate),
    /* K76  */  be_nested_str_weak(start_operational_discovery_deferred),
    /* K77  */  be_nested_str_weak(is_PASE),
    /* K78  */  be_nested_str_weak(set_expire_in_seconds),
    /* K79  */  be_nested_str_weak(log_new_fabric),
    /* K80  */  be_nested_str_weak(set_fabric_label),
    /* K81  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Update_X20fabric_X20_X27_X25s_X27_X20label_X3D_X27_X25s_X27),
    /* K82  */  be_nested_str_weak(get_fabric_id),
    /* K83  */  be_nested_str_weak(tohex),
    /* K84  */  be_nested_str_weak(fabric_index_X3A),
    /* K85  */  be_nested_str_weak(active_fabrics),
    /* K86  */  be_nested_str_weak(get_fabric_index),
    /* K87  */  be_nested_str_weak(set_timer),
    /* K88  */  be_nested_str_weak(stop_iteration),
    /* K89  */  be_nested_str_weak(MTR_X3A_X20RemoveFabric_X20fabric_X28),
    /* K90  */  be_nested_str_weak(_X29_X20not_X20found),
    /* K91  */  be_nested_str_weak(INVALID_ACTION),
    /* K92  */  be_nested_str_weak(MTR_X3A_X20OpenCommissioningWindow_X28timeout_X3D_X25i_X2C_X20passcode_X3D_X25s_X2C_X20discriminator_X3D_X25i_X2C_X20iterations_X3D_X25i_X2C_X20salt_X3D_X25s_X29),
    /* K93  */  be_nested_str_weak(INVALID_DATA_TYPE),
    /* K94  */  be_nested_str_weak(MTR_X3A_X20wrong_X20size_X20for_X20PAKE_X20parameters),
    /* K95  */  be_nested_str_weak(CONSTRAINT_ERROR),
    /* K96  */  be_nested_str_weak(start_basic_commissioning),
    /* K97  */  be_nested_str_weak(get_fabric),
    /* K98  */  be_nested_str_weak(MTR_X3A_X20OpenBasicCommissioningWindow_X20commissioning_timeout_X3D),
    /* K99  */  be_nested_str_weak(start_root_basic_commissioning),
    /* K100 */  be_nested_str_weak(stop_basic_commissioning),
    /* K101 */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[714]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x5422002F,  //  0005  LDINT	R8	48
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x7822005C,  //  0007  JMPF	R8	#0065
      0x1C200F05,  //  0008  EQ	R8	R7	K5
      0x78220017,  //  0009  JMPF	R8	#0022
      0x8C200506,  //  000A  GETMET	R8	R2	K6
      0x58280005,  //  000B  LDCONST	R10	K5
      0x542E0383,  //  000C  LDINT	R11	900
      0x7C200600,  //  000D  CALL	R8	3
      0x8C240506,  //  000E  GETMET	R9	R2	K6
      0x582C0007,  //  000F  LDCONST	R11	K7
      0x58300005,  //  0010  LDCONST	R12	K5
      0x7C240600,  //  0011  CALL	R9	3
      0x90061009,  //  0012  SETMBR	R1	K8	R9
      0x8C280B09,  //  0013  GETMET	R10	R5	K9
      0x7C280200,  //  0014  CALL	R10	1
      0x8C2C150A,  //  0015  GETMET	R11	R10	K10
      0x58340005,  //  0016  LDCONST	R13	K5
      0x88380B0B,  //  0017  GETMBR	R14	R5	K11
      0x583C0005,  //  0018  LDCONST	R15	K5
      0x7C2C0800,  //  0019  CALL	R11	4
      0x8C2C150A,  //  001A  GETMET	R11	R10	K10
      0x58340007,  //  001B  LDCONST	R13	K7
      0x88380B0C,  //  001C  GETMBR	R14	R5	K12
      0x583C000D,  //  001D  LDCONST	R15	K13
      0x7C2C0800,  //  001E  CALL	R11	4
      0x900E0907,  //  001F  SETMBR	R3	K4	K7
      0x80041400,  //  0020  RET	1	R10
      0x70020041,  //  0021  JMP		#0064
      0x1C200F0E,  //  0022  EQ	R8	R7	K14
      0x7822001A,  //  0023  JMPF	R8	#003F
      0x8C200506,  //  0024  GETMET	R8	R2	K6
      0x58280005,  //  0025  LDCONST	R10	K5
      0x7C200400,  //  0026  CALL	R8	2
      0x8C240506,  //  0027  GETMET	R9	R2	K6
      0x582C0007,  //  0028  LDCONST	R11	K7
      0x5830000F,  //  0029  LDCONST	R12	K15
      0x7C240600,  //  002A  CALL	R9	3
      0x8C280506,  //  002B  GETMET	R10	R2	K6
      0x5830000E,  //  002C  LDCONST	R12	K14
      0x58340005,  //  002D  LDCONST	R13	K5
      0x7C280600,  //  002E  CALL	R10	3
      0x9006100A,  //  002F  SETMBR	R1	K8	R10
      0x8C2C0B09,  //  0030  GETMET	R11	R5	K9
      0x7C2C0200,  //  0031  CALL	R11	1
      0x8C30170A,  //  0032  GETMET	R12	R11	K10
      0x58380005,  //  0033  LDCONST	R14	K5
      0x883C0B0B,  //  0034  GETMBR	R15	R5	K11
      0x58400005,  //  0035  LDCONST	R16	K5
      0x7C300800,  //  0036  CALL	R12	4
      0x8C30170A,  //  0037  GETMET	R12	R11	K10
      0x58380007,  //  0038  LDCONST	R14	K7
      0x883C0B0C,  //  0039  GETMBR	R15	R5	K12
      0x5840000D,  //  003A  LDCONST	R16	K13
      0x7C300800,  //  003B  CALL	R12	4
      0x900E0910,  //  003C  SETMBR	R3	K4	K16
      0x80041600,  //  003D  RET	1	R11
      0x70020024,  //  003E  JMP		#0064
      0x54220003,  //  003F  LDINT	R8	4
      0x1C200E08,  //  0040  EQ	R8	R7	R8
      0x78220021,  //  0041  JMPF	R8	#0064
      0x8C200111,  //  0042  GETMET	R8	R0	K17
      0x88280712,  //  0043  GETMBR	R10	R3	K18
      0x7C200400,  //  0044  CALL	R8	2
      0x88200313,  //  0045  GETMBR	R8	R1	K19
      0x7822001B,  //  0046  JMPF	R8	#0063
      0x90061105,  //  0047  SETMBR	R1	K8	K5
      0x88200313,  //  0048  GETMBR	R8	R1	K19
      0x8C201114,  //  0049  GETMET	R8	R8	K20
      0x7C200200,  //  004A  CALL	R8	1
      0x8C200315,  //  004B  GETMET	R8	R1	K21
      0x7C200200,  //  004C  CALL	R8	1
      0x8C200316,  //  004D  GETMET	R8	R1	K22
      0x7C200200,  //  004E  CALL	R8	1
      0x8C200B09,  //  004F  GETMET	R8	R5	K9
      0x7C200200,  //  0050  CALL	R8	1
      0x8C24110A,  //  0051  GETMET	R9	R8	K10
      0x582C0005,  //  0052  LDCONST	R11	K5
      0x88300B0B,  //  0053  GETMBR	R12	R5	K11
      0x58340005,  //  0054  LDCONST	R13	K5
      0x7C240800,  //  0055  CALL	R9	4
      0x8C24110A,  //  0056  GETMET	R9	R8	K10
      0x582C0007,  //  0057  LDCONST	R11	K7
      0x88300B0C,  //  0058  GETMBR	R12	R5	K12
      0x5834000D,  //  0059  LDCONST	R13	K13
      0x7C240800,  //  005A  CALL	R9	4
      0x54260004,  //  005B  LDINT	R9	5
      0x900E0809,  //  005C  SETMBR	R3	K4	R9
      0x88240117,  //  005D  GETMBR	R9	R0	K23
      0x8C241318,  //  005E  GETMET	R9	R9	K24
      0x5C2C0200,  //  005F  MOVE	R11	R1
      0x7C240400,  //  0060  CALL	R9	2
      0x80041000,  //  0061  RET	1	R8
      0x70020000,  //  0062  JMP		#0064
      0xB006331A,  //  0063  RAISE	1	K25	K26
      0x70020262,  //  0064  JMP		#02C8
      0x5422003D,  //  0065  LDINT	R8	62
      0x1C200C08,  //  0066  EQ	R8	R6	R8
      0x782201C3,  //  0067  JMPF	R8	#022C
      0x1C200F0E,  //  0068  EQ	R8	R7	K14
      0x7822001D,  //  0069  JMPF	R8	#0088
      0x8C200506,  //  006A  GETMET	R8	R2	K6
      0x58280005,  //  006B  LDCONST	R10	K5
      0x7C200400,  //  006C  CALL	R8	2
      0x20241107,  //  006D  NE	R9	R8	K7
      0x78260006,  //  006E  JMPF	R9	#0076
      0x2024110E,  //  006F  NE	R9	R8	K14
      0x78260004,  //  0070  JMPF	R9	#0076
      0xB8260200,  //  0071  GETNGBL	R9	K1
      0x8824131C,  //  0072  GETMBR	R9	R9	K28
      0x900E3609,  //  0073  SETMBR	R3	K27	R9
      0x4C240000,  //  0074  LDNIL	R9
      0x80041200,  //  0075  RET	1	R9
      0x8C240B09,  //  0076  GETMET	R9	R5	K9
      0x7C240200,  //  0077  CALL	R9	1
      0x8C28130A,  //  0078  GETMET	R10	R9	K10
      0x58300005,  //  0079  LDCONST	R12	K5
      0x88340B1D,  //  007A  GETMBR	R13	R5	K29
      0x1C381107,  //  007B  EQ	R14	R8	K7
      0x783A0003,  //  007C  JMPF	R14	#0081
      0xB83A0200,  //  007D  GETNGBL	R14	K1
      0x8C381D1E,  //  007E  GETMET	R14	R14	K30
      0x7C380200,  //  007F  CALL	R14	1
      0x70020002,  //  0080  JMP		#0084
      0xB83A0200,  //  0081  GETNGBL	R14	K1
      0x8C381D1F,  //  0082  GETMET	R14	R14	K31
      0x7C380200,  //  0083  CALL	R14	1
      0x7C280800,  //  0084  CALL	R10	4
      0x900E0910,  //  0085  SETMBR	R3	K4	K16
      0x80041200,  //  0086  RET	1	R9
      0x700201A2,  //  0087  JMP		#022B
      0x1C200F05,  //  0088  EQ	R8	R7	K5
      0x7822003D,  //  0089  JMPF	R8	#00C8
      0x8C200506,  //  008A  GETMET	R8	R2	K6
      0x58280005,  //  008B  LDCONST	R10	K5
      0x7C200400,  //  008C  CALL	R8	2
      0x6024000C,  //  008D  GETGBL	R9	G12
      0x5C281000,  //  008E  MOVE	R10	R8
      0x7C240200,  //  008F  CALL	R9	1
      0x542A001F,  //  0090  LDINT	R10	32
      0x2024120A,  //  0091  NE	R9	R9	R10
      0x78260001,  //  0092  JMPF	R9	#0095
      0x4C240000,  //  0093  LDNIL	R9
      0x80041200,  //  0094  RET	1	R9
      0x900E0907,  //  0095  SETMBR	R3	K4	K7
      0x8C240B09,  //  0096  GETMET	R9	R5	K9
      0x7C240200,  //  0097  CALL	R9	1
      0x8C28130A,  //  0098  GETMET	R10	R9	K10
      0x58300007,  //  0099  LDCONST	R12	K7
      0x88340B1D,  //  009A  GETMBR	R13	R5	K29
      0xB83A0200,  //  009B  GETNGBL	R14	K1
      0x8C381D20,  //  009C  GETMET	R14	R14	K32
      0x7C380200,  //  009D  CALL	R14	1
      0x7C280800,  //  009E  CALL	R10	4
      0x8C28130A,  //  009F  GETMET	R10	R9	K10
      0x5830000E,  //  00A0  LDCONST	R12	K14
      0x88340B21,  //  00A1  GETMBR	R13	R5	K33
      0x5C381000,  //  00A2  MOVE	R14	R8
      0x7C280800,  //  00A3  CALL	R10	4
      0x8C28130A,  //  00A4  GETMET	R10	R9	K10
      0x58300010,  //  00A5  LDCONST	R12	K16
      0x88340B22,  //  00A6  GETMBR	R13	R5	K34
      0xB83A4600,  //  00A7  GETNGBL	R14	K35
      0x8C381D24,  //  00A8  GETMET	R14	R14	K36
      0x7C380200,  //  00A9  CALL	R14	1
      0x94381D25,  //  00AA  GETIDX	R14	R14	K37
      0x7C280800,  //  00AB  CALL	R10	4
      0x8C281326,  //  00AC  GETMET	R10	R9	K38
      0x7C280200,  //  00AD  CALL	R10	1
      0x8C2C0327,  //  00AE  GETMET	R11	R1	K39
      0x7C2C0200,  //  00AF  CALL	R11	1
      0x0030140B,  //  00B0  ADD	R12	R10	R11
      0x8C340928,  //  00B1  GETMET	R13	R4	K40
      0x7C340200,  //  00B2  CALL	R13	1
      0x8C341B29,  //  00B3  GETMET	R13	R13	K41
      0xB83E0200,  //  00B4  GETNGBL	R15	K1
      0x8C3C1F2A,  //  00B5  GETMET	R15	R15	K42
      0x7C3C0200,  //  00B6  CALL	R15	1
      0x5C401800,  //  00B7  MOVE	R16	R12
      0x7C340600,  //  00B8  CALL	R13	3
      0x8C380B09,  //  00B9  GETMET	R14	R5	K9
      0x7C380200,  //  00BA  CALL	R14	1
      0x8C3C1D0A,  //  00BB  GETMET	R15	R14	K10
      0x58440005,  //  00BC  LDCONST	R17	K5
      0x88480B1D,  //  00BD  GETMBR	R18	R5	K29
      0x5C4C1400,  //  00BE  MOVE	R19	R10
      0x7C3C0800,  //  00BF  CALL	R15	4
      0x8C3C1D0A,  //  00C0  GETMET	R15	R14	K10
      0x58440007,  //  00C1  LDCONST	R17	K7
      0x88480B21,  //  00C2  GETMBR	R18	R5	K33
      0x5C4C1A00,  //  00C3  MOVE	R19	R13
      0x7C3C0800,  //  00C4  CALL	R15	4
      0x900E0907,  //  00C5  SETMBR	R3	K4	K7
      0x80041C00,  //  00C6  RET	1	R14
      0x70020162,  //  00C7  JMP		#022B
      0x54220003,  //  00C8  LDINT	R8	4
      0x1C200E08,  //  00C9  EQ	R8	R7	R8
      0x7822003C,  //  00CA  JMPF	R8	#0108
      0x8C200111,  //  00CB  GETMET	R8	R0	K17
      0x88280712,  //  00CC  GETMBR	R10	R3	K18
      0x7C200400,  //  00CD  CALL	R8	2
      0x8C200506,  //  00CE  GETMET	R8	R2	K6
      0x58280005,  //  00CF  LDCONST	R10	K5
      0x7C200400,  //  00D0  CALL	R8	2
      0x6024000C,  //  00D1  GETGBL	R9	G12
      0x5C281000,  //  00D2  MOVE	R10	R8
      0x7C240200,  //  00D3  CALL	R9	1
      0x542A001F,  //  00D4  LDINT	R10	32
      0x2024120A,  //  00D5  NE	R9	R9	R10
      0x78260001,  //  00D6  JMPF	R9	#00D9
      0x4C240000,  //  00D7  LDNIL	R9
      0x80041200,  //  00D8  RET	1	R9
      0x8C240506,  //  00D9  GETMET	R9	R2	K6
      0x582C0007,  //  00DA  LDCONST	R11	K7
      0x50300000,  //  00DB  LDBOOL	R12	0	0
      0x7C240600,  //  00DC  CALL	R9	3
      0x8C28032B,  //  00DD  GETMET	R10	R1	K43
      0x7C280200,  //  00DE  CALL	R10	1
      0x8C2C0B09,  //  00DF  GETMET	R11	R5	K9
      0x7C2C0200,  //  00E0  CALL	R11	1
      0x8C30170A,  //  00E1  GETMET	R12	R11	K10
      0x58380007,  //  00E2  LDCONST	R14	K7
      0x883C0B1D,  //  00E3  GETMBR	R15	R5	K29
      0x5C401400,  //  00E4  MOVE	R16	R10
      0x7C300800,  //  00E5  CALL	R12	4
      0x8C30170A,  //  00E6  GETMET	R12	R11	K10
      0x5838000E,  //  00E7  LDCONST	R14	K14
      0x883C0B21,  //  00E8  GETMBR	R15	R5	K33
      0x5C401000,  //  00E9  MOVE	R16	R8
      0x7C300800,  //  00EA  CALL	R12	4
      0x8C301726,  //  00EB  GETMET	R12	R11	K38
      0x7C300200,  //  00EC  CALL	R12	1
      0x8C340327,  //  00ED  GETMET	R13	R1	K39
      0x7C340200,  //  00EE  CALL	R13	1
      0x0034180D,  //  00EF  ADD	R13	R12	R13
      0x8C380928,  //  00F0  GETMET	R14	R4	K40
      0x7C380200,  //  00F1  CALL	R14	1
      0x8C381D29,  //  00F2  GETMET	R14	R14	K41
      0xB8420200,  //  00F3  GETNGBL	R16	K1
      0x8C40212A,  //  00F4  GETMET	R16	R16	K42
      0x7C400200,  //  00F5  CALL	R16	1
      0x5C441A00,  //  00F6  MOVE	R17	R13
      0x7C380600,  //  00F7  CALL	R14	3
      0x8C3C0B09,  //  00F8  GETMET	R15	R5	K9
      0x7C3C0200,  //  00F9  CALL	R15	1
      0x8C401F0A,  //  00FA  GETMET	R16	R15	K10
      0x58480005,  //  00FB  LDCONST	R18	K5
      0x884C0B1D,  //  00FC  GETMBR	R19	R5	K29
      0x5C501800,  //  00FD  MOVE	R20	R12
      0x7C400800,  //  00FE  CALL	R16	4
      0x8C401F0A,  //  00FF  GETMET	R16	R15	K10
      0x58480007,  //  0100  LDCONST	R18	K7
      0x884C0B21,  //  0101  GETMBR	R19	R5	K33
      0x5C501C00,  //  0102  MOVE	R20	R14
      0x7C400800,  //  0103  CALL	R16	4
      0x54420004,  //  0104  LDINT	R16	5
      0x900E0810,  //  0105  SETMBR	R3	K4	R16
      0x80041E00,  //  0106  RET	1	R15
      0x70020122,  //  0107  JMP		#022B
      0x5422000A,  //  0108  LDINT	R8	11
      0x1C200E08,  //  0109  EQ	R8	R7	R8
      0x7822000B,  //  010A  JMPF	R8	#0117
      0x8C200506,  //  010B  GETMET	R8	R2	K6
      0x58280005,  //  010C  LDCONST	R10	K5
      0x7C200400,  //  010D  CALL	R8	2
      0x8C24032C,  //  010E  GETMET	R9	R1	K44
      0x5C2C1000,  //  010F  MOVE	R11	R8
      0x7C240400,  //  0110  CALL	R9	2
      0xB8260200,  //  0111  GETNGBL	R9	K1
      0x8824132D,  //  0112  GETMBR	R9	R9	K45
      0x900E3609,  //  0113  SETMBR	R3	K27	R9
      0x4C240000,  //  0114  LDNIL	R9
      0x80041200,  //  0115  RET	1	R9
      0x70020113,  //  0116  JMP		#022B
      0x54220005,  //  0117  LDINT	R8	6
      0x1C200E08,  //  0118  EQ	R8	R7	R8
      0x782200B9,  //  0119  JMPF	R8	#01D4
      0xB8224600,  //  011A  GETNGBL	R8	K35
      0x8C20112E,  //  011B  GETMET	R8	R8	K46
      0x60280008,  //  011C  GETGBL	R10	G8
      0x5C2C0400,  //  011D  MOVE	R11	R2
      0x7C280200,  //  011E  CALL	R10	1
      0x002A5E0A,  //  011F  ADD	R10	K47	R10
      0x542E0003,  //  0120  LDINT	R11	4
      0x7C200600,  //  0121  CALL	R8	3
      0x8C200506,  //  0122  GETMET	R8	R2	K6
      0x58280005,  //  0123  LDCONST	R10	K5
      0x7C200400,  //  0124  CALL	R8	2
      0x8C240506,  //  0125  GETMET	R9	R2	K6
      0x582C0007,  //  0126  LDCONST	R11	K7
      0x7C240400,  //  0127  CALL	R9	2
      0x6028000C,  //  0128  GETGBL	R10	G12
      0x5C2C1200,  //  0129  MOVE	R11	R9
      0x7C280200,  //  012A  CALL	R10	1
      0x1C281505,  //  012B  EQ	R10	R10	K5
      0x782A0000,  //  012C  JMPF	R10	#012E
      0x4C240000,  //  012D  LDNIL	R9
      0x8C280506,  //  012E  GETMET	R10	R2	K6
      0x5830000E,  //  012F  LDCONST	R12	K14
      0x7C280400,  //  0130  CALL	R10	2
      0x8C2C0506,  //  0131  GETMET	R11	R2	K6
      0x58340010,  //  0132  LDCONST	R13	K16
      0x7C2C0400,  //  0133  CALL	R11	2
      0x8C300506,  //  0134  GETMET	R12	R2	K6
      0x543A0003,  //  0135  LDINT	R14	4
      0x7C300400,  //  0136  CALL	R12	2
      0x8C340330,  //  0137  GETMET	R13	R1	K48
      0x7C340200,  //  0138  CALL	R13	1
      0x4C380000,  //  0139  LDNIL	R14
      0x1C341A0E,  //  013A  EQ	R13	R13	R14
      0x78360006,  //  013B  JMPF	R13	#0143
      0xB8364600,  //  013C  GETNGBL	R13	K35
      0x8C341B2E,  //  013D  GETMET	R13	R13	K46
      0x583C0031,  //  013E  LDCONST	R15	K49
      0x5840000E,  //  013F  LDCONST	R16	K14
      0x7C340600,  //  0140  CALL	R13	3
      0x4C340000,  //  0141  LDNIL	R13
      0x80041A00,  //  0142  RET	1	R13
      0x88340117,  //  0143  GETMBR	R13	R0	K23
      0x88341B32,  //  0144  GETMBR	R13	R13	K50
      0x8C341B33,  //  0145  GETMET	R13	R13	K51
      0x7C340200,  //  0146  CALL	R13	1
      0x8C381B34,  //  0147  GETMET	R14	R13	K52
      0x8C400330,  //  0148  GETMET	R16	R1	K48
      0x7C400200,  //  0149  CALL	R16	1
      0x7C380400,  //  014A  CALL	R14	2
      0x8C381B35,  //  014B  GETMET	R14	R13	K53
      0x5C401000,  //  014C  MOVE	R16	R8
      0x5C441200,  //  014D  MOVE	R17	R9
      0x7C380600,  //  014E  CALL	R14	3
      0x8C381B36,  //  014F  GETMET	R14	R13	K54
      0x5C401400,  //  0150  MOVE	R16	R10
      0x7C380400,  //  0151  CALL	R14	2
      0x8C381B37,  //  0152  GETMET	R14	R13	K55
      0x5C401600,  //  0153  MOVE	R16	R11
      0x5C441800,  //  0154  MOVE	R17	R12
      0x7C380600,  //  0155  CALL	R14	3
      0x8C381B38,  //  0156  GETMET	R14	R13	K56
      0x8C400339,  //  0157  GETMET	R16	R1	K57
      0x7C400200,  //  0158  CALL	R16	1
      0x7C380400,  //  0159  CALL	R14	2
      0xB83A0200,  //  015A  GETNGBL	R14	K1
      0x88381D02,  //  015B  GETMBR	R14	R14	K2
      0x8C381D3A,  //  015C  GETMET	R14	R14	K58
      0x5C401000,  //  015D  MOVE	R16	R8
      0x7C380400,  //  015E  CALL	R14	2
      0x8C3C1D3B,  //  015F  GETMET	R15	R14	K59
      0x54460005,  //  0160  LDINT	R17	6
      0x7C3C0400,  //  0161  CALL	R15	2
      0x8C401F06,  //  0162  GETMET	R16	R15	K6
      0x544A0014,  //  0163  LDINT	R18	21
      0x7C400400,  //  0164  CALL	R16	2
      0x8C441F06,  //  0165  GETMET	R17	R15	K6
      0x544E0010,  //  0166  LDINT	R19	17
      0x7C440400,  //  0167  CALL	R17	2
      0x5C482000,  //  0168  MOVE	R18	R16
      0x784A0001,  //  0169  JMPF	R18	#016C
      0x5C482200,  //  016A  MOVE	R18	R17
      0x744A0006,  //  016B  JMPT	R18	#0173
      0xB84A4600,  //  016C  GETNGBL	R18	K35
      0x8C48252E,  //  016D  GETMET	R18	R18	K46
      0x5850003C,  //  016E  LDCONST	R20	K60
      0x5854000E,  //  016F  LDCONST	R21	K14
      0x7C480600,  //  0170  CALL	R18	3
      0x50480000,  //  0171  LDBOOL	R18	0	0
      0x80042400,  //  0172  RET	1	R18
      0x60480004,  //  0173  GETGBL	R18	G4
      0x5C4C2000,  //  0174  MOVE	R19	R16
      0x7C480200,  //  0175  CALL	R18	1
      0x1C48253D,  //  0176  EQ	R18	R18	K61
      0x784A0007,  //  0177  JMPF	R18	#0180
      0xB84A7C00,  //  0178  GETNGBL	R18	K62
      0x8C48253F,  //  0179  GETMET	R18	R18	K63
      0x5C502000,  //  017A  MOVE	R20	R16
      0x7C480400,  //  017B  CALL	R18	2
      0x8C482540,  //  017C  GETMET	R18	R18	K64
      0x7C480200,  //  017D  CALL	R18	1
      0x5C402400,  //  017E  MOVE	R16	R18
      0x70020002,  //  017F  JMP		#0183
      0x8C482140,  //  0180  GETMET	R18	R16	K64
      0x7C480200,  //  0181  CALL	R18	1
      0x5C402400,  //  0182  MOVE	R16	R18
      0x60480004,  //  0183  GETGBL	R18	G4
      0x5C4C2200,  //  0184  MOVE	R19	R17
      0x7C480200,  //  0185  CALL	R18	1
      0x1C48253D,  //  0186  EQ	R18	R18	K61
      0x784A0007,  //  0187  JMPF	R18	#0190
      0xB84A7C00,  //  0188  GETNGBL	R18	K62
      0x8C48253F,  //  0189  GETMET	R18	R18	K63
      0x5C502200,  //  018A  MOVE	R20	R17
      0x7C480400,  //  018B  CALL	R18	2
      0x8C482540,  //  018C  GETMET	R18	R18	K64
      0x7C480200,  //  018D  CALL	R18	1
      0x5C442400,  //  018E  MOVE	R17	R18
      0x70020002,  //  018F  JMP		#0193
      0x8C482340,  //  0190  GETMET	R18	R17	K64
      0x7C480200,  //  0191  CALL	R18	1
      0x5C442400,  //  0192  MOVE	R17	R18
      0x8C480341,  //  0193  GETMET	R18	R1	K65
      0x7C480200,  //  0194  CALL	R18	1
      0x404E0F42,  //  0195  CONNECT	R19	K7	K66
      0x94482413,  //  0196  GETIDX	R18	R18	R19
      0x60500015,  //  0197  GETGBL	R20	G21
      0x7C500000,  //  0198  CALL	R20	0
      0x8C502943,  //  0199  GETMET	R20	R20	K67
      0x58580044,  //  019A  LDCONST	R22	K68
      0x7C500400,  //  019B  CALL	R20	2
      0x5C4C2800,  //  019C  MOVE	R19	R20
      0x8C500945,  //  019D  GETMET	R20	R4	K69
      0x7C500200,  //  019E  CALL	R20	1
      0x8C542146,  //  019F  GETMET	R21	R16	K70
      0x7C540200,  //  01A0  CALL	R21	1
      0x8C542B47,  //  01A1  GETMET	R21	R21	K71
      0x7C540200,  //  01A2  CALL	R21	1
      0x8C582948,  //  01A3  GETMET	R22	R20	K72
      0x5C602400,  //  01A4  MOVE	R24	R18
      0x5C642A00,  //  01A5  MOVE	R25	R21
      0x5C682600,  //  01A6  MOVE	R26	R19
      0x546E0007,  //  01A7  LDINT	R27	8
      0x7C580A00,  //  01A8  CALL	R22	5
      0x885C0313,  //  01A9  GETMBR	R23	R1	K19
      0x785E0001,  //  01AA  JMPF	R23	#01AD
      0x885C0313,  //  01AB  GETMBR	R23	R1	K19
      0x70020001,  //  01AC  JMP		#01AF
      0x885C0117,  //  01AD  GETMBR	R23	R0	K23
      0x885C2F49,  //  01AE  GETMBR	R23	R23	K73
      0x8C601B4A,  //  01AF  GETMET	R24	R13	K74
      0x5C682000,  //  01B0  MOVE	R26	R16
      0x5C6C2200,  //  01B1  MOVE	R27	R17
      0x5C702C00,  //  01B2  MOVE	R28	R22
      0x5C742E00,  //  01B3  MOVE	R29	R23
      0x7C600A00,  //  01B4  CALL	R24	5
      0x8C601B4B,  //  01B5  GETMET	R24	R13	K75
      0x7C600200,  //  01B6  CALL	R24	1
      0x88600117,  //  01B7  GETMBR	R24	R0	K23
      0x8C60314C,  //  01B8  GETMET	R24	R24	K76
      0x5C681A00,  //  01B9  MOVE	R26	R13
      0x7C600400,  //  01BA  CALL	R24	2
      0x8C60034D,  //  01BB  GETMET	R24	R1	K77
      0x7C600200,  //  01BC  CALL	R24	1
      0x78620002,  //  01BD  JMPF	R24	#01C1
      0x8C60034E,  //  01BE  GETMET	R24	R1	K78
      0x546A003B,  //  01BF  LDINT	R26	60
      0x7C600400,  //  01C0  CALL	R24	2
      0x8C601B4F,  //  01C1  GETMET	R24	R13	K79
      0x7C600200,  //  01C2  CALL	R24	1
      0x8C600B09,  //  01C3  GETMET	R24	R5	K9
      0x7C600200,  //  01C4  CALL	R24	1
      0x8C64310A,  //  01C5  GETMET	R25	R24	K10
      0x586C0005,  //  01C6  LDCONST	R27	K5
      0x88700B0B,  //  01C7  GETMBR	R28	R5	K11
      0xB8760200,  //  01C8  GETNGBL	R29	K1
      0x88743B2D,  //  01C9  GETMBR	R29	R29	K45
      0x7C640800,  //  01CA  CALL	R25	4
      0x8C64310A,  //  01CB  GETMET	R25	R24	K10
      0x586C0007,  //  01CC  LDCONST	R27	K7
      0x88700B0B,  //  01CD  GETMBR	R28	R5	K11
      0x58740007,  //  01CE  LDCONST	R29	K7
      0x7C640800,  //  01CF  CALL	R25	4
      0x54660007,  //  01D0  LDINT	R25	8
      0x900E0819,  //  01D1  SETMBR	R3	K4	R25
      0x80043000,  //  01D2  RET	1	R24
      0x70020056,  //  01D3  JMP		#022B
      0x54220008,  //  01D4  LDINT	R8	9
      0x1C200E08,  //  01D5  EQ	R8	R7	R8
      0x7822001E,  //  01D6  JMPF	R8	#01F6
      0x8C200506,  //  01D7  GETMET	R8	R2	K6
      0x58280005,  //  01D8  LDCONST	R10	K5
      0x7C200400,  //  01D9  CALL	R8	2
      0x8C240350,  //  01DA  GETMET	R9	R1	K80
      0x5C2C1000,  //  01DB  MOVE	R11	R8
      0x7C240400,  //  01DC  CALL	R9	2
      0xB8264600,  //  01DD  GETNGBL	R9	K35
      0x8C24132E,  //  01DE  GETMET	R9	R9	K46
      0x602C0018,  //  01DF  GETGBL	R11	G24
      0x58300051,  //  01E0  LDCONST	R12	K81
      0x88340313,  //  01E1  GETMBR	R13	R1	K19
      0x8C341B52,  //  01E2  GETMET	R13	R13	K82
      0x7C340200,  //  01E3  CALL	R13	1
      0x8C341B46,  //  01E4  GETMET	R13	R13	K70
      0x7C340200,  //  01E5  CALL	R13	1
      0x8C341B47,  //  01E6  GETMET	R13	R13	K71
      0x7C340200,  //  01E7  CALL	R13	1
      0x8C341B53,  //  01E8  GETMET	R13	R13	K83
      0x7C340200,  //  01E9  CALL	R13	1
      0x60380008,  //  01EA  GETGBL	R14	G8
      0x5C3C1000,  //  01EB  MOVE	R15	R8
      0x7C380200,  //  01EC  CALL	R14	1
      0x7C2C0600,  //  01ED  CALL	R11	3
      0x58300010,  //  01EE  LDCONST	R12	K16
      0x7C240600,  //  01EF  CALL	R9	3
      0xB8260200,  //  01F0  GETNGBL	R9	K1
      0x8824132D,  //  01F1  GETMBR	R9	R9	K45
      0x900E3609,  //  01F2  SETMBR	R3	K27	R9
      0x4C240000,  //  01F3  LDNIL	R9
      0x80041200,  //  01F4  RET	1	R9
      0x70020034,  //  01F5  JMP		#022B
      0x54220009,  //  01F6  LDINT	R8	10
      0x1C200E08,  //  01F7  EQ	R8	R7	R8
      0x78220031,  //  01F8  JMPF	R8	#022B
      0x8C200506,  //  01F9  GETMET	R8	R2	K6
      0x58280005,  //  01FA  LDCONST	R10	K5
      0x7C200400,  //  01FB  CALL	R8	2
      0x60240008,  //  01FC  GETGBL	R9	G8
      0x5C281000,  //  01FD  MOVE	R10	R8
      0x7C240200,  //  01FE  CALL	R9	1
      0x0026A809,  //  01FF  ADD	R9	K84	R9
      0x900E5C09,  //  0200  SETMBR	R3	K46	R9
      0x60240010,  //  0201  GETGBL	R9	G16
      0x88280117,  //  0202  GETMBR	R10	R0	K23
      0x88281532,  //  0203  GETMBR	R10	R10	K50
      0x8C281555,  //  0204  GETMET	R10	R10	K85
      0x7C280200,  //  0205  CALL	R10	1
      0x7C240200,  //  0206  CALL	R9	1
      0xA8020010,  //  0207  EXBLK	0	#0219
      0x5C281200,  //  0208  MOVE	R10	R9
      0x7C280000,  //  0209  CALL	R10	0
      0x8C2C1556,  //  020A  GETMET	R11	R10	K86
      0x7C2C0200,  //  020B  CALL	R11	1
      0x1C2C1608,  //  020C  EQ	R11	R11	R8
      0x782E0008,  //  020D  JMPF	R11	#0217
      0xB82E4600,  //  020E  GETNGBL	R11	K35
      0x8C2C1757,  //  020F  GETMET	R11	R11	K87
      0x543607CF,  //  0210  LDINT	R13	2000
      0x84380000,  //  0211  CLOSURE	R14	P0
      0x7C2C0600,  //  0212  CALL	R11	3
      0x502C0200,  //  0213  LDBOOL	R11	1	0
      0xA0000000,  //  0214  CLOSE	R0
      0xA8040001,  //  0215  EXBLK	1	1
      0x80041600,  //  0216  RET	1	R11
      0xA0240000,  //  0217  CLOSE	R9
      0x7001FFEE,  //  0218  JMP		#0208
      0x58240058,  //  0219  LDCONST	R9	K88
      0xAC240200,  //  021A  CATCH	R9	1	0
      0xB0080000,  //  021B  RAISE	2	R0	R0
      0xB8264600,  //  021C  GETNGBL	R9	K35
      0x8C24132E,  //  021D  GETMET	R9	R9	K46
      0x602C0008,  //  021E  GETGBL	R11	G8
      0x5C301000,  //  021F  MOVE	R12	R8
      0x7C2C0200,  //  0220  CALL	R11	1
      0x002EB20B,  //  0221  ADD	R11	K89	R11
      0x002C175A,  //  0222  ADD	R11	R11	K90
      0x5830000E,  //  0223  LDCONST	R12	K14
      0x7C240600,  //  0224  CALL	R9	3
      0xB8260200,  //  0225  GETNGBL	R9	K1
      0x8824135B,  //  0226  GETMBR	R9	R9	K91
      0x900E3609,  //  0227  SETMBR	R3	K27	R9
      0x4C240000,  //  0228  LDNIL	R9
      0xA0000000,  //  0229  CLOSE	R0
      0x80041200,  //  022A  RET	1	R9
      0x7002009B,  //  022B  JMP		#02C8
      0x5422003B,  //  022C  LDINT	R8	60
      0x1C200C08,  //  022D  EQ	R8	R6	R8
      0x78220085,  //  022E  JMPF	R8	#02B5
      0x1C200F05,  //  022F  EQ	R8	R7	K5
      0x78220065,  //  0230  JMPF	R8	#0297
      0x8C200506,  //  0231  GETMET	R8	R2	K6
      0x58280005,  //  0232  LDCONST	R10	K5
      0x7C200400,  //  0233  CALL	R8	2
      0x8C240506,  //  0234  GETMET	R9	R2	K6
      0x582C0007,  //  0235  LDCONST	R11	K7
      0x7C240400,  //  0236  CALL	R9	2
      0x8C280506,  //  0237  GETMET	R10	R2	K6
      0x5830000E,  //  0238  LDCONST	R12	K14
      0x7C280400,  //  0239  CALL	R10	2
      0x8C2C0506,  //  023A  GETMET	R11	R2	K6
      0x58340010,  //  023B  LDCONST	R13	K16
      0x7C2C0400,  //  023C  CALL	R11	2
      0x8C300506,  //  023D  GETMET	R12	R2	K6
      0x543A0003,  //  023E  LDINT	R14	4
      0x7C300400,  //  023F  CALL	R12	2
      0xB8364600,  //  0240  GETNGBL	R13	K35
      0x8C341B2E,  //  0241  GETMET	R13	R13	K46
      0x603C0018,  //  0242  GETGBL	R15	G24
      0x5840005C,  //  0243  LDCONST	R16	K92
      0x5C441000,  //  0244  MOVE	R17	R8
      0x8C481353,  //  0245  GETMET	R18	R9	K83
      0x7C480200,  //  0246  CALL	R18	1
      0x5C4C1400,  //  0247  MOVE	R19	R10
      0x5C501600,  //  0248  MOVE	R20	R11
      0x8C541953,  //  0249  GETMET	R21	R12	K83
      0x7C540200,  //  024A  CALL	R21	1
      0x7C3C0C00,  //  024B  CALL	R15	6
      0x54420003,  //  024C  LDINT	R16	4
      0x7C340600,  //  024D  CALL	R13	3
      0x4C340000,  //  024E  LDNIL	R13
      0x1C34100D,  //  024F  EQ	R13	R8	R13
      0x7436000B,  //  0250  JMPT	R13	#025D
      0x4C340000,  //  0251  LDNIL	R13
      0x1C34120D,  //  0252  EQ	R13	R9	R13
      0x74360008,  //  0253  JMPT	R13	#025D
      0x4C340000,  //  0254  LDNIL	R13
      0x1C34140D,  //  0255  EQ	R13	R10	R13
      0x74360005,  //  0256  JMPT	R13	#025D
      0x4C340000,  //  0257  LDNIL	R13
      0x1C34160D,  //  0258  EQ	R13	R11	R13
      0x74360002,  //  0259  JMPT	R13	#025D
      0x4C340000,  //  025A  LDNIL	R13
      0x1C34180D,  //  025B  EQ	R13	R12	R13
      0x78360005,  //  025C  JMPF	R13	#0263
      0xB8360200,  //  025D  GETNGBL	R13	K1
      0x88341B5D,  //  025E  GETMBR	R13	R13	K93
      0x900E360D,  //  025F  SETMBR	R3	K27	R13
      0x4C340000,  //  0260  LDNIL	R13
      0xA0000000,  //  0261  CLOSE	R0
      0x80041A00,  //  0262  RET	1	R13
      0x6034000C,  //  0263  GETGBL	R13	G12
      0x5C381200,  //  0264  MOVE	R14	R9
      0x7C340200,  //  0265  CALL	R13	1
      0x543A001F,  //  0266  LDINT	R14	32
      0x543E0040,  //  0267  LDINT	R15	65
      0x00381C0F,  //  0268  ADD	R14	R14	R15
      0x20341A0E,  //  0269  NE	R13	R13	R14
      0x7436000B,  //  026A  JMPT	R13	#0277
      0x6034000C,  //  026B  GETGBL	R13	G12
      0x5C381800,  //  026C  MOVE	R14	R12
      0x7C340200,  //  026D  CALL	R13	1
      0x543A000F,  //  026E  LDINT	R14	16
      0x14341A0E,  //  026F  LT	R13	R13	R14
      0x74360005,  //  0270  JMPT	R13	#0277
      0x6034000C,  //  0271  GETGBL	R13	G12
      0x5C381800,  //  0272  MOVE	R14	R12
      0x7C340200,  //  0273  CALL	R13	1
      0x543A001F,  //  0274  LDINT	R14	32
      0x24341A0E,  //  0275  GT	R13	R13	R14
      0x7836000A,  //  0276  JMPF	R13	#0282
      0xB8364600,  //  0277  GETNGBL	R13	K35
      0x8C341B2E,  //  0278  GETMET	R13	R13	K46
      0x583C005E,  //  0279  LDCONST	R15	K94
      0x5840000E,  //  027A  LDCONST	R16	K14
      0x7C340600,  //  027B  CALL	R13	3
      0xB8360200,  //  027C  GETNGBL	R13	K1
      0x88341B5F,  //  027D  GETMBR	R13	R13	K95
      0x900E360D,  //  027E  SETMBR	R3	K27	R13
      0x4C340000,  //  027F  LDNIL	R13
      0xA0000000,  //  0280  CLOSE	R0
      0x80041A00,  //  0281  RET	1	R13
      0x5436001E,  //  0282  LDINT	R13	31
      0x40360A0D,  //  0283  CONNECT	R13	K5	R13
      0x9434120D,  //  0284  GETIDX	R13	R9	R13
      0x543A001F,  //  0285  LDINT	R14	32
      0x40381D42,  //  0286  CONNECT	R14	R14	K66
      0x9438120E,  //  0287  GETIDX	R14	R9	R14
      0x883C0117,  //  0288  GETMBR	R15	R0	K23
      0x8C3C1F60,  //  0289  GETMET	R15	R15	K96
      0x5C441000,  //  028A  MOVE	R17	R8
      0x5C481600,  //  028B  MOVE	R18	R11
      0x5C4C1400,  //  028C  MOVE	R19	R10
      0x5C501800,  //  028D  MOVE	R20	R12
      0x5C541A00,  //  028E  MOVE	R21	R13
      0x5C581C00,  //  028F  MOVE	R22	R14
      0x8C5C0361,  //  0290  GETMET	R23	R1	K97
      0x7C5C0200,  //  0291  CALL	R23	1
      0x7C3C1000,  //  0292  CALL	R15	8
      0x503C0200,  //  0293  LDBOOL	R15	1	0
      0xA0000000,  //  0294  CLOSE	R0
      0x80041E00,  //  0295  RET	1	R15
      0x7002001C,  //  0296  JMP		#02B4
      0x1C200F07,  //  0297  EQ	R8	R7	K7
      0x78220012,  //  0298  JMPF	R8	#02AC
      0x8C200506,  //  0299  GETMET	R8	R2	K6
      0x58280005,  //  029A  LDCONST	R10	K5
      0x7C200400,  //  029B  CALL	R8	2
      0xB8264600,  //  029C  GETNGBL	R9	K35
      0x8C24132E,  //  029D  GETMET	R9	R9	K46
      0x602C0008,  //  029E  GETGBL	R11	G8
      0x5C301000,  //  029F  MOVE	R12	R8
      0x7C2C0200,  //  02A0  CALL	R11	1
      0x002EC40B,  //  02A1  ADD	R11	K98	R11
      0x58300010,  //  02A2  LDCONST	R12	K16
      0x7C240600,  //  02A3  CALL	R9	3
      0x88240117,  //  02A4  GETMBR	R9	R0	K23
      0x8C241363,  //  02A5  GETMET	R9	R9	K99
      0x5C2C1000,  //  02A6  MOVE	R11	R8
      0x7C240400,  //  02A7  CALL	R9	2
      0x50240200,  //  02A8  LDBOOL	R9	1	0
      0xA0000000,  //  02A9  CLOSE	R0
      0x80041200,  //  02AA  RET	1	R9
      0x70020007,  //  02AB  JMP		#02B4
      0x1C200F0E,  //  02AC  EQ	R8	R7	K14
      0x78220005,  //  02AD  JMPF	R8	#02B4
      0x88200117,  //  02AE  GETMBR	R8	R0	K23
      0x8C201164,  //  02AF  GETMET	R8	R8	K100
      0x7C200200,  //  02B0  CALL	R8	1
      0x50200200,  //  02B1  LDBOOL	R8	1	0
      0xA0000000,  //  02B2  CLOSE	R0
      0x80041000,  //  02B3  RET	1	R8
      0x70020012,  //  02B4  JMP		#02C8
      0x54220029,  //  02B5  LDINT	R8	42
      0x1C200C08,  //  02B6  EQ	R8	R6	R8
      0x78220005,  //  02B7  JMPF	R8	#02BE
      0x1C200F05,  //  02B8  EQ	R8	R7	K5
      0x78220002,  //  02B9  JMPF	R8	#02BD
      0x50200200,  //  02BA  LDBOOL	R8	1	0
      0xA0000000,  //  02BB  CLOSE	R0
      0x80041000,  //  02BC  RET	1	R8
      0x70020009,  //  02BD  JMP		#02C8
      0x60200003,  //  02BE  GETGBL	R8	G3
      0x5C240000,  //  02BF  MOVE	R9	R0
      0x7C200200,  //  02C0  CALL	R8	1
      0x8C201165,  //  02C1  GETMET	R8	R8	K101
      0x5C280200,  //  02C2  MOVE	R10	R1
      0x5C2C0400,  //  02C3  MOVE	R11	R2
      0x5C300600,  //  02C4  MOVE	R12	R3
      0x7C200800,  //  02C5  CALL	R8	4
      0xA0000000,  //  02C6  CLOSE	R0
      0x80041000,  //  02C7  RET	1	R8
      0xA0000000,  //  02C8  CLOSE	R0
      0x80000000,  //  02C9  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Root_read_attribute,   /* name */
  be_nested_proto(
    24,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[88]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U8),
    /* K8   */  be_nested_str_weak(_breadcrumb),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(Matter_TLV_struct),
    /* K11  */  be_nested_str_weak(add_TLV),
    /* K12  */  be_nested_str_weak(U2),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(U1),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(BOOL),
    /* K17  */  be_nested_str_weak(Matter_TLV_array),
    /* K18  */  be_nested_str_weak(tasmota),
    /* K19  */  be_nested_str_weak(eth),
    /* K20  */  be_nested_str_weak(up),
    /* K21  */  be_nested_str_weak(add_struct),
    /* K22  */  be_nested_str_weak(UTF1),
    /* K23  */  be_nested_str_weak(ethernet),
    /* K24  */  be_nested_str_weak(NULL),
    /* K25  */  be_nested_str_weak(fromhex),
    /* K26  */  be_nested_str_weak(replace),
    /* K27  */  be_nested_str_weak(find),
    /* K28  */  be_nested_str_weak(mac),
    /* K29  */  be_nested_str_weak(),
    /* K30  */  be_nested_str_weak(_X3A),
    /* K31  */  be_nested_str_weak(B1),
    /* K32  */  be_nested_str_weak(add_array),
    /* K33  */  be_nested_str_weak(get_ip_bytes),
    /* K34  */  be_nested_str_weak(ip),
    /* K35  */  be_nested_str_weak(ip6local),
    /* K36  */  be_nested_str_weak(ip6),
    /* K37  */  be_nested_str_weak(wifi),
    /* K38  */  be_nested_str_weak(cmd),
    /* K39  */  be_nested_str_weak(Status_X201),
    /* K40  */  be_nested_str_weak(StatusPRM),
    /* K41  */  be_nested_str_weak(BootCount),
    /* K42  */  be_nested_str_weak(U4),
    /* K43  */  be_nested_str_weak(Status_X2011),
    /* K44  */  be_nested_str_weak(StatusSTS),
    /* K45  */  be_nested_str_weak(UptimeSec),
    /* K46  */  be_nested_str_weak(int64),
    /* K47  */  be_nested_str_weak(rtc),
    /* K48  */  be_nested_str_weak(utc),
    /* K49  */  be_const_int(1000000),
    /* K50  */  be_nested_str_weak(local),
    /* K51  */  be_nested_str_weak(device),
    /* K52  */  be_nested_str_weak(sessions),
    /* K53  */  be_nested_str_weak(active_fabrics),
    /* K54  */  be_nested_str_weak(B2),
    /* K55  */  be_nested_str_weak(get_noc),
    /* K56  */  be_nested_str_weak(get_icac),
    /* K57  */  be_nested_str_weak(get_fabric_index),
    /* K58  */  be_nested_str_weak(stop_iteration),
    /* K59  */  be_nested_str_weak(parse),
    /* K60  */  be_nested_str_weak(get_ca),
    /* K61  */  be_nested_str_weak(findsubval),
    /* K62  */  be_nested_str_weak(get_admin_vendor),
    /* K63  */  be_nested_str_weak(get_fabric_id),
    /* K64  */  be_nested_str_weak(get_device_id),
    /* K65  */  be_nested_str_weak(get_fabric_label),
    /* K66  */  be_nested_str_weak(Fabric),
    /* K67  */  be_nested_str_weak(_MAX_CASE),
    /* K68  */  be_nested_str_weak(count_active_fabrics),
    /* K69  */  be_nested_str_weak(_fabric),
    /* K70  */  be_nested_str_weak(is_commissioning_open),
    /* K71  */  be_nested_str_weak(is_root_commissioning_open),
    /* K72  */  be_nested_str_weak(commissioning_admin_fabric),
    /* K73  */  be_nested_str_weak(Tasmota),
    /* K74  */  be_nested_str_weak(vendorid),
    /* K75  */  be_nested_str_weak(DeviceName),
    /* K76  */  be_nested_str_weak(FriendlyName),
    /* K77  */  be_nested_str_weak(FriendlyName1),
    /* K78  */  be_nested_str_weak(XX),
    /* K79  */  be_nested_str_weak(Status_X202),
    /* K80  */  be_nested_str_weak(StatusFWR),
    /* K81  */  be_nested_str_weak(Hardware),
    /* K82  */  be_nested_str_weak(Version),
    /* K83  */  be_nested_str_weak(_X28),
    /* K84  */  be_nested_str_weak(locale),
    /* K85  */  be_nested_str_weak(get_active_endpoints),
    /* K86  */  be_nested_str_weak(disable_bridge_mode),
    /* K87  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[904]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E002F,  //  0005  LDINT	R7	48
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0030,  //  0007  JMPF	R7	#0039
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E0005,  //  0009  JMPF	R7	#0010
      0x8C1C0906,  //  000A  GETMET	R7	R4	K6
      0x88240907,  //  000B  GETMBR	R9	R4	K7
      0x88280308,  //  000C  GETMBR	R10	R1	K8
      0x7C1C0600,  //  000D  CALL	R7	3
      0x80040E00,  //  000E  RET	1	R7
      0x70020027,  //  000F  JMP		#0038
      0x1C1C0D09,  //  0010  EQ	R7	R6	K9
      0x781E000D,  //  0011  JMPF	R7	#0020
      0x8C1C090A,  //  0012  GETMET	R7	R4	K10
      0x7C1C0200,  //  0013  CALL	R7	1
      0x8C200F0B,  //  0014  GETMET	R8	R7	K11
      0x58280005,  //  0015  LDCONST	R10	K5
      0x882C090C,  //  0016  GETMBR	R11	R4	K12
      0x5432003B,  //  0017  LDINT	R12	60
      0x7C200800,  //  0018  CALL	R8	4
      0x8C200F0B,  //  0019  GETMET	R8	R7	K11
      0x58280009,  //  001A  LDCONST	R10	K9
      0x882C090C,  //  001B  GETMBR	R11	R4	K12
      0x54320383,  //  001C  LDINT	R12	900
      0x7C200800,  //  001D  CALL	R8	4
      0x80040E00,  //  001E  RET	1	R7
      0x70020017,  //  001F  JMP		#0038
      0x1C1C0D0D,  //  0020  EQ	R7	R6	K13
      0x781E0005,  //  0021  JMPF	R7	#0028
      0x8C1C0906,  //  0022  GETMET	R7	R4	K6
      0x8824090E,  //  0023  GETMBR	R9	R4	K14
      0x5828000D,  //  0024  LDCONST	R10	K13
      0x7C1C0600,  //  0025  CALL	R7	3
      0x80040E00,  //  0026  RET	1	R7
      0x7002000F,  //  0027  JMP		#0038
      0x1C1C0D0F,  //  0028  EQ	R7	R6	K15
      0x781E0005,  //  0029  JMPF	R7	#0030
      0x8C1C0906,  //  002A  GETMET	R7	R4	K6
      0x8824090E,  //  002B  GETMBR	R9	R4	K14
      0x5828000D,  //  002C  LDCONST	R10	K13
      0x7C1C0600,  //  002D  CALL	R7	3
      0x80040E00,  //  002E  RET	1	R7
      0x70020007,  //  002F  JMP		#0038
      0x541E0003,  //  0030  LDINT	R7	4
      0x1C1C0C07,  //  0031  EQ	R7	R6	R7
      0x781E0004,  //  0032  JMPF	R7	#0038
      0x8C1C0906,  //  0033  GETMET	R7	R4	K6
      0x88240910,  //  0034  GETMBR	R9	R4	K16
      0x50280000,  //  0035  LDBOOL	R10	0	0
      0x7C1C0600,  //  0036  CALL	R7	3
      0x80040E00,  //  0037  RET	1	R7
      0x7002034D,  //  0038  JMP		#0387
      0x541E0031,  //  0039  LDINT	R7	50
      0x1C1C0A07,  //  003A  EQ	R7	R5	R7
      0x781E0000,  //  003B  JMPF	R7	#003D
      0x70020349,  //  003C  JMP		#0387
      0x541E0032,  //  003D  LDINT	R7	51
      0x1C1C0A07,  //  003E  EQ	R7	R5	R7
      0x781E00DC,  //  003F  JMPF	R7	#011D
      0x1C1C0D05,  //  0040  EQ	R7	R6	K5
      0x781E00B5,  //  0041  JMPF	R7	#00F8
      0x8C1C0911,  //  0042  GETMET	R7	R4	K17
      0x7C1C0200,  //  0043  CALL	R7	1
      0xB8222400,  //  0044  GETNGBL	R8	K18
      0x8C201113,  //  0045  GETMET	R8	R8	K19
      0x7C200200,  //  0046  CALL	R8	1
      0x94241114,  //  0047  GETIDX	R9	R8	K20
      0x78260053,  //  0048  JMPF	R9	#009D
      0x8C240F15,  //  0049  GETMET	R9	R7	K21
      0x4C2C0000,  //  004A  LDNIL	R11
      0x7C240400,  //  004B  CALL	R9	2
      0x8C28130B,  //  004C  GETMET	R10	R9	K11
      0x58300005,  //  004D  LDCONST	R12	K5
      0x88340916,  //  004E  GETMBR	R13	R4	K22
      0x58380017,  //  004F  LDCONST	R14	K23
      0x7C280800,  //  0050  CALL	R10	4
      0x8C28130B,  //  0051  GETMET	R10	R9	K11
      0x58300009,  //  0052  LDCONST	R12	K9
      0x88340910,  //  0053  GETMBR	R13	R4	K16
      0x58380009,  //  0054  LDCONST	R14	K9
      0x7C280800,  //  0055  CALL	R10	4
      0x8C28130B,  //  0056  GETMET	R10	R9	K11
      0x5830000D,  //  0057  LDCONST	R12	K13
      0x88340910,  //  0058  GETMBR	R13	R4	K16
      0x58380009,  //  0059  LDCONST	R14	K9
      0x7C280800,  //  005A  CALL	R10	4
      0x8C28130B,  //  005B  GETMET	R10	R9	K11
      0x5830000F,  //  005C  LDCONST	R12	K15
      0x88340918,  //  005D  GETMBR	R13	R4	K24
      0x4C380000,  //  005E  LDNIL	R14
      0x7C280800,  //  005F  CALL	R10	4
      0x60280015,  //  0060  GETGBL	R10	G21
      0x7C280000,  //  0061  CALL	R10	0
      0x8C281519,  //  0062  GETMET	R10	R10	K25
      0x8C30071A,  //  0063  GETMET	R12	R3	K26
      0x8C38111B,  //  0064  GETMET	R14	R8	K27
      0x5840001C,  //  0065  LDCONST	R16	K28
      0x5844001D,  //  0066  LDCONST	R17	K29
      0x7C380600,  //  0067  CALL	R14	3
      0x583C001E,  //  0068  LDCONST	R15	K30
      0x5840001D,  //  0069  LDCONST	R16	K29
      0x7C300800,  //  006A  CALL	R12	4
      0x7C280400,  //  006B  CALL	R10	2
      0x8C2C130B,  //  006C  GETMET	R11	R9	K11
      0x54360003,  //  006D  LDINT	R13	4
      0x8838091F,  //  006E  GETMBR	R14	R4	K31
      0x5C3C1400,  //  006F  MOVE	R15	R10
      0x7C2C0800,  //  0070  CALL	R11	4
      0x8C2C1320,  //  0071  GETMET	R11	R9	K32
      0x54360004,  //  0072  LDINT	R13	5
      0x7C2C0400,  //  0073  CALL	R11	2
      0x8C30170B,  //  0074  GETMET	R12	R11	K11
      0x4C380000,  //  0075  LDNIL	R14
      0x883C091F,  //  0076  GETMBR	R15	R4	K31
      0xB8420200,  //  0077  GETNGBL	R16	K1
      0x8C402121,  //  0078  GETMET	R16	R16	K33
      0x8C48111B,  //  0079  GETMET	R18	R8	K27
      0x58500022,  //  007A  LDCONST	R20	K34
      0x5854001D,  //  007B  LDCONST	R21	K29
      0x7C480600,  //  007C  CALL	R18	3
      0x7C400400,  //  007D  CALL	R16	2
      0x7C300800,  //  007E  CALL	R12	4
      0x8C301320,  //  007F  GETMET	R12	R9	K32
      0x543A0005,  //  0080  LDINT	R14	6
      0x7C300400,  //  0081  CALL	R12	2
      0x8C34190B,  //  0082  GETMET	R13	R12	K11
      0x4C3C0000,  //  0083  LDNIL	R15
      0x8840091F,  //  0084  GETMBR	R16	R4	K31
      0xB8460200,  //  0085  GETNGBL	R17	K1
      0x8C442321,  //  0086  GETMET	R17	R17	K33
      0x8C4C111B,  //  0087  GETMET	R19	R8	K27
      0x58540023,  //  0088  LDCONST	R21	K35
      0x5858001D,  //  0089  LDCONST	R22	K29
      0x7C4C0600,  //  008A  CALL	R19	3
      0x7C440400,  //  008B  CALL	R17	2
      0x7C340800,  //  008C  CALL	R13	4
      0x8C34190B,  //  008D  GETMET	R13	R12	K11
      0x4C3C0000,  //  008E  LDNIL	R15
      0x8840091F,  //  008F  GETMBR	R16	R4	K31
      0xB8460200,  //  0090  GETNGBL	R17	K1
      0x8C442321,  //  0091  GETMET	R17	R17	K33
      0x8C4C111B,  //  0092  GETMET	R19	R8	K27
      0x58540024,  //  0093  LDCONST	R21	K36
      0x5858001D,  //  0094  LDCONST	R22	K29
      0x7C4C0600,  //  0095  CALL	R19	3
      0x7C440400,  //  0096  CALL	R17	2
      0x7C340800,  //  0097  CALL	R13	4
      0x8C34130B,  //  0098  GETMET	R13	R9	K11
      0x543E0006,  //  0099  LDINT	R15	7
      0x8840090E,  //  009A  GETMBR	R16	R4	K14
      0x5844000D,  //  009B  LDCONST	R17	K13
      0x7C340800,  //  009C  CALL	R13	4
      0xB8262400,  //  009D  GETNGBL	R9	K18
      0x8C241325,  //  009E  GETMET	R9	R9	K37
      0x7C240200,  //  009F  CALL	R9	1
      0x94281314,  //  00A0  GETIDX	R10	R9	K20
      0x782A0053,  //  00A1  JMPF	R10	#00F6
      0x8C280F15,  //  00A2  GETMET	R10	R7	K21
      0x4C300000,  //  00A3  LDNIL	R12
      0x7C280400,  //  00A4  CALL	R10	2
      0x8C2C150B,  //  00A5  GETMET	R11	R10	K11
      0x58340005,  //  00A6  LDCONST	R13	K5
      0x88380916,  //  00A7  GETMBR	R14	R4	K22
      0x583C0025,  //  00A8  LDCONST	R15	K37
      0x7C2C0800,  //  00A9  CALL	R11	4
      0x8C2C150B,  //  00AA  GETMET	R11	R10	K11
      0x58340009,  //  00AB  LDCONST	R13	K9
      0x88380910,  //  00AC  GETMBR	R14	R4	K16
      0x583C0009,  //  00AD  LDCONST	R15	K9
      0x7C2C0800,  //  00AE  CALL	R11	4
      0x8C2C150B,  //  00AF  GETMET	R11	R10	K11
      0x5834000D,  //  00B0  LDCONST	R13	K13
      0x88380910,  //  00B1  GETMBR	R14	R4	K16
      0x583C0009,  //  00B2  LDCONST	R15	K9
      0x7C2C0800,  //  00B3  CALL	R11	4
      0x8C2C150B,  //  00B4  GETMET	R11	R10	K11
      0x5834000F,  //  00B5  LDCONST	R13	K15
      0x88380918,  //  00B6  GETMBR	R14	R4	K24
      0x4C3C0000,  //  00B7  LDNIL	R15
      0x7C2C0800,  //  00B8  CALL	R11	4
      0x602C0015,  //  00B9  GETGBL	R11	G21
      0x7C2C0000,  //  00BA  CALL	R11	0
      0x8C2C1719,  //  00BB  GETMET	R11	R11	K25
      0x8C34071A,  //  00BC  GETMET	R13	R3	K26
      0x8C3C131B,  //  00BD  GETMET	R15	R9	K27
      0x5844001C,  //  00BE  LDCONST	R17	K28
      0x5848001D,  //  00BF  LDCONST	R18	K29
      0x7C3C0600,  //  00C0  CALL	R15	3
      0x5840001E,  //  00C1  LDCONST	R16	K30
      0x5844001D,  //  00C2  LDCONST	R17	K29
      0x7C340800,  //  00C3  CALL	R13	4
      0x7C2C0400,  //  00C4  CALL	R11	2
      0x8C30150B,  //  00C5  GETMET	R12	R10	K11
      0x543A0003,  //  00C6  LDINT	R14	4
      0x883C091F,  //  00C7  GETMBR	R15	R4	K31
      0x5C401600,  //  00C8  MOVE	R16	R11
      0x7C300800,  //  00C9  CALL	R12	4
      0x8C301520,  //  00CA  GETMET	R12	R10	K32
      0x543A0004,  //  00CB  LDINT	R14	5
      0x7C300400,  //  00CC  CALL	R12	2
      0x8C34190B,  //  00CD  GETMET	R13	R12	K11
      0x4C3C0000,  //  00CE  LDNIL	R15
      0x8840091F,  //  00CF  GETMBR	R16	R4	K31
      0xB8460200,  //  00D0  GETNGBL	R17	K1
      0x8C442321,  //  00D1  GETMET	R17	R17	K33
      0x8C4C131B,  //  00D2  GETMET	R19	R9	K27
      0x58540022,  //  00D3  LDCONST	R21	K34
      0x5858001D,  //  00D4  LDCONST	R22	K29
      0x7C4C0600,  //  00D5  CALL	R19	3
      0x7C440400,  //  00D6  CALL	R17	2
      0x7C340800,  //  00D7  CALL	R13	4
      0x8C341520,  //  00D8  GETMET	R13	R10	K32
      0x543E0005,  //  00D9  LDINT	R15	6
      0x7C340400,  //  00DA  CALL	R13	2
      0x8C381B0B,  //  00DB  GETMET	R14	R13	K11
      0x4C400000,  //  00DC  LDNIL	R16
      0x8844091F,  //  00DD  GETMBR	R17	R4	K31
      0xB84A0200,  //  00DE  GETNGBL	R18	K1
      0x8C482521,  //  00DF  GETMET	R18	R18	K33
      0x8C50131B,  //  00E0  GETMET	R20	R9	K27
      0x58580023,  //  00E1  LDCONST	R22	K35
      0x585C001D,  //  00E2  LDCONST	R23	K29
      0x7C500600,  //  00E3  CALL	R20	3
      0x7C480400,  //  00E4  CALL	R18	2
      0x7C380800,  //  00E5  CALL	R14	4
      0x8C381B0B,  //  00E6  GETMET	R14	R13	K11
      0x4C400000,  //  00E7  LDNIL	R16
      0x8844091F,  //  00E8  GETMBR	R17	R4	K31
      0xB84A0200,  //  00E9  GETNGBL	R18	K1
      0x8C482521,  //  00EA  GETMET	R18	R18	K33
      0x8C50131B,  //  00EB  GETMET	R20	R9	K27
      0x58580024,  //  00EC  LDCONST	R22	K36
      0x585C001D,  //  00ED  LDCONST	R23	K29
      0x7C500600,  //  00EE  CALL	R20	3
      0x7C480400,  //  00EF  CALL	R18	2
      0x7C380800,  //  00F0  CALL	R14	4
      0x8C38150B,  //  00F1  GETMET	R14	R10	K11
      0x54420006,  //  00F2  LDINT	R16	7
      0x8844090E,  //  00F3  GETMBR	R17	R4	K14
      0x58480009,  //  00F4  LDCONST	R18	K9
      0x7C380800,  //  00F5  CALL	R14	4
      0x80040E00,  //  00F6  RET	1	R7
      0x70020023,  //  00F7  JMP		#011C
      0x1C1C0D09,  //  00F8  EQ	R7	R6	K9
      0x781E000B,  //  00F9  JMPF	R7	#0106
      0x8C1C0906,  //  00FA  GETMET	R7	R4	K6
      0x8824090C,  //  00FB  GETMBR	R9	R4	K12
      0xB82A2400,  //  00FC  GETNGBL	R10	K18
      0x8C281526,  //  00FD  GETMET	R10	R10	K38
      0x58300027,  //  00FE  LDCONST	R12	K39
      0x50340200,  //  00FF  LDBOOL	R13	1	0
      0x7C280600,  //  0100  CALL	R10	3
      0x94281528,  //  0101  GETIDX	R10	R10	K40
      0x94281529,  //  0102  GETIDX	R10	R10	K41
      0x7C1C0600,  //  0103  CALL	R7	3
      0x80040E00,  //  0104  RET	1	R7
      0x70020015,  //  0105  JMP		#011C
      0x1C1C0D0D,  //  0106  EQ	R7	R6	K13
      0x781E000B,  //  0107  JMPF	R7	#0114
      0x8C1C0906,  //  0108  GETMET	R7	R4	K6
      0x8824092A,  //  0109  GETMBR	R9	R4	K42
      0xB82A2400,  //  010A  GETNGBL	R10	K18
      0x8C281526,  //  010B  GETMET	R10	R10	K38
      0x5830002B,  //  010C  LDCONST	R12	K43
      0x50340200,  //  010D  LDBOOL	R13	1	0
      0x7C280600,  //  010E  CALL	R10	3
      0x9428152C,  //  010F  GETIDX	R10	R10	K44
      0x9428152D,  //  0110  GETIDX	R10	R10	K45
      0x7C1C0600,  //  0111  CALL	R7	3
      0x80040E00,  //  0112  RET	1	R7
      0x70020007,  //  0113  JMP		#011C
      0x541E0007,  //  0114  LDINT	R7	8
      0x1C1C0C07,  //  0115  EQ	R7	R6	R7
      0x781E0004,  //  0116  JMPF	R7	#011C
      0x8C1C0906,  //  0117  GETMET	R7	R4	K6
      0x88240910,  //  0118  GETMBR	R9	R4	K16
      0x50280000,  //  0119  LDBOOL	R10	0	0
      0x7C1C0600,  //  011A  CALL	R7	3
      0x80040E00,  //  011B  RET	1	R7
      0x70020269,  //  011C  JMP		#0387
      0x541E0033,  //  011D  LDINT	R7	52
      0x1C1C0A07,  //  011E  EQ	R7	R5	R7
      0x781E0000,  //  011F  JMPF	R7	#0121
      0x70020265,  //  0120  JMP		#0387
      0x541E0037,  //  0121  LDINT	R7	56
      0x1C1C0A07,  //  0122  EQ	R7	R5	R7
      0x781E002C,  //  0123  JMPF	R7	#0151
      0x1C1C0D05,  //  0124  EQ	R7	R6	K5
      0x781E000F,  //  0125  JMPF	R7	#0136
      0xB81E5C00,  //  0126  GETNGBL	R7	K46
      0xB8222400,  //  0127  GETNGBL	R8	K18
      0x8C20112F,  //  0128  GETMET	R8	R8	K47
      0x7C200200,  //  0129  CALL	R8	1
      0x94201130,  //  012A  GETIDX	R8	R8	K48
      0x7C1C0200,  //  012B  CALL	R7	1
      0xB8225C00,  //  012C  GETNGBL	R8	K46
      0x58240031,  //  012D  LDCONST	R9	K49
      0x7C200200,  //  012E  CALL	R8	1
      0x081C0E08,  //  012F  MUL	R7	R7	R8
      0x8C200906,  //  0130  GETMET	R8	R4	K6
      0x88280907,  //  0131  GETMBR	R10	R4	K7
      0x5C2C0E00,  //  0132  MOVE	R11	R7
      0x7C200600,  //  0133  CALL	R8	3
      0x80041000,  //  0134  RET	1	R8
      0x70020019,  //  0135  JMP		#0150
      0x1C1C0D09,  //  0136  EQ	R7	R6	K9
      0x781E0005,  //  0137  JMPF	R7	#013E
      0x8C1C0906,  //  0138  GETMET	R7	R4	K6
      0x8824090E,  //  0139  GETMBR	R9	R4	K14
      0x5828000F,  //  013A  LDCONST	R10	K15
      0x7C1C0600,  //  013B  CALL	R7	3
      0x80040E00,  //  013C  RET	1	R7
      0x70020011,  //  013D  JMP		#0150
      0x541E0006,  //  013E  LDINT	R7	7
      0x1C1C0C07,  //  013F  EQ	R7	R6	R7
      0x781E000E,  //  0140  JMPF	R7	#0150
      0xB81E5C00,  //  0141  GETNGBL	R7	K46
      0xB8222400,  //  0142  GETNGBL	R8	K18
      0x8C20112F,  //  0143  GETMET	R8	R8	K47
      0x7C200200,  //  0144  CALL	R8	1
      0x94201132,  //  0145  GETIDX	R8	R8	K50
      0x7C1C0200,  //  0146  CALL	R7	1
      0xB8225C00,  //  0147  GETNGBL	R8	K46
      0x58240031,  //  0148  LDCONST	R9	K49
      0x7C200200,  //  0149  CALL	R8	1
      0x081C0E08,  //  014A  MUL	R7	R7	R8
      0x8C200906,  //  014B  GETMET	R8	R4	K6
      0x88280907,  //  014C  GETMBR	R10	R4	K7
      0x5C2C0E00,  //  014D  MOVE	R11	R7
      0x7C200600,  //  014E  CALL	R8	3
      0x80041000,  //  014F  RET	1	R8
      0x70020235,  //  0150  JMP		#0387
      0x541E003D,  //  0151  LDINT	R7	62
      0x1C1C0A07,  //  0152  EQ	R7	R5	R7
      0x781E0090,  //  0153  JMPF	R7	#01E5
      0x1C1C0D05,  //  0154  EQ	R7	R6	K5
      0x781E0025,  //  0155  JMPF	R7	#017C
      0x8C1C0911,  //  0156  GETMET	R7	R4	K17
      0x7C1C0200,  //  0157  CALL	R7	1
      0x60200010,  //  0158  GETGBL	R8	G16
      0x88240133,  //  0159  GETMBR	R9	R0	K51
      0x88241334,  //  015A  GETMBR	R9	R9	K52
      0x8C241335,  //  015B  GETMET	R9	R9	K53
      0x7C240200,  //  015C  CALL	R9	1
      0x7C200200,  //  015D  CALL	R8	1
      0xA8020017,  //  015E  EXBLK	0	#0177
      0x5C241000,  //  015F  MOVE	R9	R8
      0x7C240000,  //  0160  CALL	R9	0
      0x8C280F15,  //  0161  GETMET	R10	R7	K21
      0x4C300000,  //  0162  LDNIL	R12
      0x7C280400,  //  0163  CALL	R10	2
      0x8C2C150B,  //  0164  GETMET	R11	R10	K11
      0x58340009,  //  0165  LDCONST	R13	K9
      0x88380936,  //  0166  GETMBR	R14	R4	K54
      0x8C3C1337,  //  0167  GETMET	R15	R9	K55
      0x7C3C0200,  //  0168  CALL	R15	1
      0x7C2C0800,  //  0169  CALL	R11	4
      0x8C2C150B,  //  016A  GETMET	R11	R10	K11
      0x5834000D,  //  016B  LDCONST	R13	K13
      0x88380936,  //  016C  GETMBR	R14	R4	K54
      0x8C3C1338,  //  016D  GETMET	R15	R9	K56
      0x7C3C0200,  //  016E  CALL	R15	1
      0x7C2C0800,  //  016F  CALL	R11	4
      0x8C2C150B,  //  0170  GETMET	R11	R10	K11
      0x543600FD,  //  0171  LDINT	R13	254
      0x8838090C,  //  0172  GETMBR	R14	R4	K12
      0x8C3C1339,  //  0173  GETMET	R15	R9	K57
      0x7C3C0200,  //  0174  CALL	R15	1
      0x7C2C0800,  //  0175  CALL	R11	4
      0x7001FFE7,  //  0176  JMP		#015F
      0x5820003A,  //  0177  LDCONST	R8	K58
      0xAC200200,  //  0178  CATCH	R8	1	0
      0xB0080000,  //  0179  RAISE	2	R0	R0
      0x80040E00,  //  017A  RET	1	R7
      0x70020067,  //  017B  JMP		#01E4
      0x1C1C0D09,  //  017C  EQ	R7	R6	K9
      0x781E003C,  //  017D  JMPF	R7	#01BB
      0x8C1C0911,  //  017E  GETMET	R7	R4	K17
      0x7C1C0200,  //  017F  CALL	R7	1
      0x60200010,  //  0180  GETGBL	R8	G16
      0x88240133,  //  0181  GETMBR	R9	R0	K51
      0x88241334,  //  0182  GETMBR	R9	R9	K52
      0x8C241335,  //  0183  GETMET	R9	R9	K53
      0x7C240200,  //  0184  CALL	R9	1
      0x7C200200,  //  0185  CALL	R8	1
      0xA802002E,  //  0186  EXBLK	0	#01B6
      0x5C241000,  //  0187  MOVE	R9	R8
      0x7C240000,  //  0188  CALL	R9	0
      0x8C28093B,  //  0189  GETMET	R10	R4	K59
      0x8C30133C,  //  018A  GETMET	R12	R9	K60
      0x7C300200,  //  018B  CALL	R12	1
      0x7C280400,  //  018C  CALL	R10	2
      0x8C2C0F15,  //  018D  GETMET	R11	R7	K21
      0x4C340000,  //  018E  LDNIL	R13
      0x7C2C0400,  //  018F  CALL	R11	2
      0x8C30170B,  //  0190  GETMET	R12	R11	K11
      0x58380009,  //  0191  LDCONST	R14	K9
      0x883C0936,  //  0192  GETMBR	R15	R4	K54
      0x8C40153D,  //  0193  GETMET	R16	R10	K61
      0x544A0008,  //  0194  LDINT	R18	9
      0x7C400400,  //  0195  CALL	R16	2
      0x7C300800,  //  0196  CALL	R12	4
      0x8C30170B,  //  0197  GETMET	R12	R11	K11
      0x5838000D,  //  0198  LDCONST	R14	K13
      0x883C090C,  //  0199  GETMBR	R15	R4	K12
      0x8C40133E,  //  019A  GETMET	R16	R9	K62
      0x7C400200,  //  019B  CALL	R16	1
      0x7C300800,  //  019C  CALL	R12	4
      0x8C30170B,  //  019D  GETMET	R12	R11	K11
      0x5838000F,  //  019E  LDCONST	R14	K15
      0x883C0907,  //  019F  GETMBR	R15	R4	K7
      0x8C40133F,  //  01A0  GETMET	R16	R9	K63
      0x7C400200,  //  01A1  CALL	R16	1
      0x7C300800,  //  01A2  CALL	R12	4
      0x8C30170B,  //  01A3  GETMET	R12	R11	K11
      0x543A0003,  //  01A4  LDINT	R14	4
      0x883C0907,  //  01A5  GETMBR	R15	R4	K7
      0x8C401340,  //  01A6  GETMET	R16	R9	K64
      0x7C400200,  //  01A7  CALL	R16	1
      0x7C300800,  //  01A8  CALL	R12	4
      0x8C30170B,  //  01A9  GETMET	R12	R11	K11
      0x543A0004,  //  01AA  LDINT	R14	5
      0x883C0916,  //  01AB  GETMBR	R15	R4	K22
      0x8C401341,  //  01AC  GETMET	R16	R9	K65
      0x7C400200,  //  01AD  CALL	R16	1
      0x7C300800,  //  01AE  CALL	R12	4
      0x8C30170B,  //  01AF  GETMET	R12	R11	K11
      0x543A00FD,  //  01B0  LDINT	R14	254
      0x883C090C,  //  01B1  GETMBR	R15	R4	K12
      0x8C401339,  //  01B2  GETMET	R16	R9	K57
      0x7C400200,  //  01B3  CALL	R16	1
      0x7C300800,  //  01B4  CALL	R12	4
      0x7001FFD0,  //  01B5  JMP		#0187
      0x5820003A,  //  01B6  LDCONST	R8	K58
      0xAC200200,  //  01B7  CATCH	R8	1	0
      0xB0080000,  //  01B8  RAISE	2	R0	R0
      0x80040E00,  //  01B9  RET	1	R7
      0x70020028,  //  01BA  JMP		#01E4
      0x1C1C0D0D,  //  01BB  EQ	R7	R6	K13
      0x781E0007,  //  01BC  JMPF	R7	#01C5
      0x8C1C0906,  //  01BD  GETMET	R7	R4	K6
      0x8824090E,  //  01BE  GETMBR	R9	R4	K14
      0xB82A0200,  //  01BF  GETNGBL	R10	K1
      0x88281542,  //  01C0  GETMBR	R10	R10	K66
      0x88281543,  //  01C1  GETMBR	R10	R10	K67
      0x7C1C0600,  //  01C2  CALL	R7	3
      0x80040E00,  //  01C3  RET	1	R7
      0x7002001E,  //  01C4  JMP		#01E4
      0x1C1C0D0F,  //  01C5  EQ	R7	R6	K15
      0x781E0009,  //  01C6  JMPF	R7	#01D1
      0x881C0133,  //  01C7  GETMBR	R7	R0	K51
      0x881C0F34,  //  01C8  GETMBR	R7	R7	K52
      0x8C1C0F44,  //  01C9  GETMET	R7	R7	K68
      0x7C1C0200,  //  01CA  CALL	R7	1
      0x8C200906,  //  01CB  GETMET	R8	R4	K6
      0x8828090E,  //  01CC  GETMBR	R10	R4	K14
      0x5C2C0E00,  //  01CD  MOVE	R11	R7
      0x7C200600,  //  01CE  CALL	R8	3
      0x80041000,  //  01CF  RET	1	R8
      0x70020012,  //  01D0  JMP		#01E4
      0x541E0003,  //  01D1  LDINT	R7	4
      0x1C1C0C07,  //  01D2  EQ	R7	R6	R7
      0x781E0000,  //  01D3  JMPF	R7	#01D5
      0x7002000E,  //  01D4  JMP		#01E4
      0x541E0004,  //  01D5  LDINT	R7	5
      0x1C1C0C07,  //  01D6  EQ	R7	R6	R7
      0x781E000B,  //  01D7  JMPF	R7	#01E4
      0x881C0345,  //  01D8  GETMBR	R7	R1	K69
      0x8C1C0F39,  //  01D9  GETMET	R7	R7	K57
      0x7C1C0200,  //  01DA  CALL	R7	1
      0x4C200000,  //  01DB  LDNIL	R8
      0x1C200E08,  //  01DC  EQ	R8	R7	R8
      0x78220000,  //  01DD  JMPF	R8	#01DF
      0x581C0005,  //  01DE  LDCONST	R7	K5
      0x8C200906,  //  01DF  GETMET	R8	R4	K6
      0x8828090E,  //  01E0  GETMBR	R10	R4	K14
      0x5C2C0E00,  //  01E1  MOVE	R11	R7
      0x7C200600,  //  01E2  CALL	R8	3
      0x80041000,  //  01E3  RET	1	R8
      0x700201A1,  //  01E4  JMP		#0387
      0x541E003B,  //  01E5  LDINT	R7	60
      0x1C1C0A07,  //  01E6  EQ	R7	R5	R7
      0x781E003C,  //  01E7  JMPF	R7	#0225
      0x1C1C0D05,  //  01E8  EQ	R7	R6	K5
      0x781E0012,  //  01E9  JMPF	R7	#01FD
      0x881C0133,  //  01EA  GETMBR	R7	R0	K51
      0x8C1C0F46,  //  01EB  GETMET	R7	R7	K70
      0x7C1C0200,  //  01EC  CALL	R7	1
      0x88200133,  //  01ED  GETMBR	R8	R0	K51
      0x8C201147,  //  01EE  GETMET	R8	R8	K71
      0x7C200200,  //  01EF  CALL	R8	1
      0x781E0004,  //  01F0  JMPF	R7	#01F6
      0x78220001,  //  01F1  JMPF	R8	#01F4
      0x5824000D,  //  01F2  LDCONST	R9	K13
      0x70020000,  //  01F3  JMP		#01F5
      0x58240009,  //  01F4  LDCONST	R9	K9
      0x70020000,  //  01F5  JMP		#01F7
      0x58240005,  //  01F6  LDCONST	R9	K5
      0x8C280906,  //  01F7  GETMET	R10	R4	K6
      0x8830090E,  //  01F8  GETMBR	R12	R4	K14
      0x5C341200,  //  01F9  MOVE	R13	R9
      0x7C280600,  //  01FA  CALL	R10	3
      0x80041400,  //  01FB  RET	1	R10
      0x70020026,  //  01FC  JMP		#0224
      0x1C1C0D09,  //  01FD  EQ	R7	R6	K9
      0x781E0011,  //  01FE  JMPF	R7	#0211
      0x881C0133,  //  01FF  GETMBR	R7	R0	K51
      0x881C0F48,  //  0200  GETMBR	R7	R7	K72
      0x4C200000,  //  0201  LDNIL	R8
      0x20200E08,  //  0202  NE	R8	R7	R8
      0x78220006,  //  0203  JMPF	R8	#020B
      0x8C200906,  //  0204  GETMET	R8	R4	K6
      0x8828090C,  //  0205  GETMBR	R10	R4	K12
      0x8C2C0F39,  //  0206  GETMET	R11	R7	K57
      0x7C2C0200,  //  0207  CALL	R11	1
      0x7C200600,  //  0208  CALL	R8	3
      0x80041000,  //  0209  RET	1	R8
      0x70020004,  //  020A  JMP		#0210
      0x8C200906,  //  020B  GETMET	R8	R4	K6
      0x88280918,  //  020C  GETMBR	R10	R4	K24
      0x4C2C0000,  //  020D  LDNIL	R11
      0x7C200600,  //  020E  CALL	R8	3
      0x80041000,  //  020F  RET	1	R8
      0x70020012,  //  0210  JMP		#0224
      0x1C1C0D0D,  //  0211  EQ	R7	R6	K13
      0x781E0010,  //  0212  JMPF	R7	#0224
      0x881C0133,  //  0213  GETMBR	R7	R0	K51
      0x881C0F48,  //  0214  GETMBR	R7	R7	K72
      0x4C200000,  //  0215  LDNIL	R8
      0x20200E08,  //  0216  NE	R8	R7	R8
      0x78220006,  //  0217  JMPF	R8	#021F
      0x8C200906,  //  0218  GETMET	R8	R4	K6
      0x8828090C,  //  0219  GETMBR	R10	R4	K12
      0x8C2C0F3E,  //  021A  GETMET	R11	R7	K62
      0x7C2C0200,  //  021B  CALL	R11	1
      0x7C200600,  //  021C  CALL	R8	3
      0x80041000,  //  021D  RET	1	R8
      0x70020004,  //  021E  JMP		#0224
      0x8C200906,  //  021F  GETMET	R8	R4	K6
      0x88280918,  //  0220  GETMBR	R10	R4	K24
      0x4C2C0000,  //  0221  LDNIL	R11
      0x7C200600,  //  0222  CALL	R8	3
      0x80041000,  //  0223  RET	1	R8
      0x70020161,  //  0224  JMP		#0387
      0x541E0027,  //  0225  LDINT	R7	40
      0x1C1C0A07,  //  0226  EQ	R7	R5	R7
      0x781E00B7,  //  0227  JMPF	R7	#02E0
      0x1C1C0D05,  //  0228  EQ	R7	R6	K5
      0x781E0005,  //  0229  JMPF	R7	#0230
      0x8C1C0906,  //  022A  GETMET	R7	R4	K6
      0x8824090C,  //  022B  GETMBR	R9	R4	K12
      0x58280009,  //  022C  LDCONST	R10	K9
      0x7C1C0600,  //  022D  CALL	R7	3
      0x80040E00,  //  022E  RET	1	R7
      0x700200AE,  //  022F  JMP		#02DF
      0x1C1C0D09,  //  0230  EQ	R7	R6	K9
      0x781E0005,  //  0231  JMPF	R7	#0238
      0x8C1C0906,  //  0232  GETMET	R7	R4	K6
      0x88240916,  //  0233  GETMBR	R9	R4	K22
      0x58280049,  //  0234  LDCONST	R10	K73
      0x7C1C0600,  //  0235  CALL	R7	3
      0x80040E00,  //  0236  RET	1	R7
      0x700200A6,  //  0237  JMP		#02DF
      0x1C1C0D0D,  //  0238  EQ	R7	R6	K13
      0x781E0006,  //  0239  JMPF	R7	#0241
      0x8C1C0906,  //  023A  GETMET	R7	R4	K6
      0x8824090C,  //  023B  GETMBR	R9	R4	K12
      0x88280133,  //  023C  GETMBR	R10	R0	K51
      0x8828154A,  //  023D  GETMBR	R10	R10	K74
      0x7C1C0600,  //  023E  CALL	R7	3
      0x80040E00,  //  023F  RET	1	R7
      0x7002009D,  //  0240  JMP		#02DF
      0x1C1C0D0F,  //  0241  EQ	R7	R6	K15
      0x781E000A,  //  0242  JMPF	R7	#024E
      0x8C1C0906,  //  0243  GETMET	R7	R4	K6
      0x88240916,  //  0244  GETMBR	R9	R4	K22
      0xB82A2400,  //  0245  GETNGBL	R10	K18
      0x8C281526,  //  0246  GETMET	R10	R10	K38
      0x5830004B,  //  0247  LDCONST	R12	K75
      0x50340200,  //  0248  LDBOOL	R13	1	0
      0x7C280600,  //  0249  CALL	R10	3
      0x9428154B,  //  024A  GETIDX	R10	R10	K75
      0x7C1C0600,  //  024B  CALL	R7	3
      0x80040E00,  //  024C  RET	1	R7
      0x70020090,  //  024D  JMP		#02DF
      0x541E0003,  //  024E  LDINT	R7	4
      0x1C1C0C07,  //  024F  EQ	R7	R6	R7
      0x781E0005,  //  0250  JMPF	R7	#0257
      0x8C1C0906,  //  0251  GETMET	R7	R4	K6
      0x8824090C,  //  0252  GETMBR	R9	R4	K12
      0x542A7FFF,  //  0253  LDINT	R10	32768
      0x7C1C0600,  //  0254  CALL	R7	3
      0x80040E00,  //  0255  RET	1	R7
      0x70020087,  //  0256  JMP		#02DF
      0x541E0004,  //  0257  LDINT	R7	5
      0x1C1C0C07,  //  0258  EQ	R7	R6	R7
      0x781E000A,  //  0259  JMPF	R7	#0265
      0x8C1C0906,  //  025A  GETMET	R7	R4	K6
      0x88240916,  //  025B  GETMBR	R9	R4	K22
      0xB82A2400,  //  025C  GETNGBL	R10	K18
      0x8C281526,  //  025D  GETMET	R10	R10	K38
      0x5830004C,  //  025E  LDCONST	R12	K76
      0x50340200,  //  025F  LDBOOL	R13	1	0
      0x7C280600,  //  0260  CALL	R10	3
      0x9428154D,  //  0261  GETIDX	R10	R10	K77
      0x7C1C0600,  //  0262  CALL	R7	3
      0x80040E00,  //  0263  RET	1	R7
      0x70020079,  //  0264  JMP		#02DF
      0x541E0005,  //  0265  LDINT	R7	6
      0x1C1C0C07,  //  0266  EQ	R7	R6	R7
      0x781E0005,  //  0267  JMPF	R7	#026E
      0x8C1C0906,  //  0268  GETMET	R7	R4	K6
      0x88240916,  //  0269  GETMBR	R9	R4	K22
      0x5828004E,  //  026A  LDCONST	R10	K78
      0x7C1C0600,  //  026B  CALL	R7	3
      0x80040E00,  //  026C  RET	1	R7
      0x70020070,  //  026D  JMP		#02DF
      0x541E0006,  //  026E  LDINT	R7	7
      0x1C1C0C07,  //  026F  EQ	R7	R6	R7
      0x781E0005,  //  0270  JMPF	R7	#0277
      0x8C1C0906,  //  0271  GETMET	R7	R4	K6
      0x8824090C,  //  0272  GETMBR	R9	R4	K12
      0x58280005,  //  0273  LDCONST	R10	K5
      0x7C1C0600,  //  0274  CALL	R7	3
      0x80040E00,  //  0275  RET	1	R7
      0x70020067,  //  0276  JMP		#02DF
      0x541E0007,  //  0277  LDINT	R7	8
      0x1C1C0C07,  //  0278  EQ	R7	R6	R7
      0x781E000B,  //  0279  JMPF	R7	#0286
      0x8C1C0906,  //  027A  GETMET	R7	R4	K6
      0x88240916,  //  027B  GETMBR	R9	R4	K22
      0xB82A2400,  //  027C  GETNGBL	R10	K18
      0x8C281526,  //  027D  GETMET	R10	R10	K38
      0x5830004F,  //  027E  LDCONST	R12	K79
      0x50340200,  //  027F  LDBOOL	R13	1	0
      0x7C280600,  //  0280  CALL	R10	3
      0x94281550,  //  0281  GETIDX	R10	R10	K80
      0x94281551,  //  0282  GETIDX	R10	R10	K81
      0x7C1C0600,  //  0283  CALL	R7	3
      0x80040E00,  //  0284  RET	1	R7
      0x70020058,  //  0285  JMP		#02DF
      0x541E0008,  //  0286  LDINT	R7	9
      0x1C1C0C07,  //  0287  EQ	R7	R6	R7
      0x781E0005,  //  0288  JMPF	R7	#028F
      0x8C1C0906,  //  0289  GETMET	R7	R4	K6
      0x8824090C,  //  028A  GETMBR	R9	R4	K12
      0x58280009,  //  028B  LDCONST	R10	K9
      0x7C1C0600,  //  028C  CALL	R7	3
      0x80040E00,  //  028D  RET	1	R7
      0x7002004F,  //  028E  JMP		#02DF
      0x541E0009,  //  028F  LDINT	R7	10
      0x1C1C0C07,  //  0290  EQ	R7	R6	R7
      0x781E0015,  //  0291  JMPF	R7	#02A8
      0xB81E2400,  //  0292  GETNGBL	R7	K18
      0x8C1C0F26,  //  0293  GETMET	R7	R7	K38
      0x5824004F,  //  0294  LDCONST	R9	K79
      0x50280200,  //  0295  LDBOOL	R10	1	0
      0x7C1C0600,  //  0296  CALL	R7	3
      0x941C0F50,  //  0297  GETIDX	R7	R7	K80
      0x941C0F52,  //  0298  GETIDX	R7	R7	K82
      0x8C20071B,  //  0299  GETMET	R8	R3	K27
      0x5C280E00,  //  029A  MOVE	R10	R7
      0x582C0053,  //  029B  LDCONST	R11	K83
      0x7C200600,  //  029C  CALL	R8	3
      0x24241105,  //  029D  GT	R9	R8	K5
      0x78260002,  //  029E  JMPF	R9	#02A2
      0x04241109,  //  029F  SUB	R9	R8	K9
      0x40260A09,  //  02A0  CONNECT	R9	K5	R9
      0x941C0E09,  //  02A1  GETIDX	R7	R7	R9
      0x8C240906,  //  02A2  GETMET	R9	R4	K6
      0x882C0916,  //  02A3  GETMBR	R11	R4	K22
      0x5C300E00,  //  02A4  MOVE	R12	R7
      0x7C240600,  //  02A5  CALL	R9	3
      0x80041200,  //  02A6  RET	1	R9
      0x70020036,  //  02A7  JMP		#02DF
      0x541E000E,  //  02A8  LDINT	R7	15
      0x1C1C0C07,  //  02A9  EQ	R7	R6	R7
      0x781E000B,  //  02AA  JMPF	R7	#02B7
      0x8C1C0906,  //  02AB  GETMET	R7	R4	K6
      0x88240916,  //  02AC  GETMBR	R9	R4	K22
      0xB82A2400,  //  02AD  GETNGBL	R10	K18
      0x8C281525,  //  02AE  GETMET	R10	R10	K37
      0x7C280200,  //  02AF  CALL	R10	1
      0x8C28151B,  //  02B0  GETMET	R10	R10	K27
      0x5830001C,  //  02B1  LDCONST	R12	K28
      0x5834001D,  //  02B2  LDCONST	R13	K29
      0x7C280600,  //  02B3  CALL	R10	3
      0x7C1C0600,  //  02B4  CALL	R7	3
      0x80040E00,  //  02B5  RET	1	R7
      0x70020027,  //  02B6  JMP		#02DF
      0x541E0010,  //  02B7  LDINT	R7	17
      0x1C1C0C07,  //  02B8  EQ	R7	R6	R7
      0x781E0005,  //  02B9  JMPF	R7	#02C0
      0x8C1C0906,  //  02BA  GETMET	R7	R4	K6
      0x88240910,  //  02BB  GETMBR	R9	R4	K16
      0x58280009,  //  02BC  LDCONST	R10	K9
      0x7C1C0600,  //  02BD  CALL	R7	3
      0x80040E00,  //  02BE  RET	1	R7
      0x7002001E,  //  02BF  JMP		#02DF
      0x541E0011,  //  02C0  LDINT	R7	18
      0x1C1C0C07,  //  02C1  EQ	R7	R6	R7
      0x781E000B,  //  02C2  JMPF	R7	#02CF
      0x8C1C0906,  //  02C3  GETMET	R7	R4	K6
      0x88240916,  //  02C4  GETMBR	R9	R4	K22
      0xB82A2400,  //  02C5  GETNGBL	R10	K18
      0x8C281525,  //  02C6  GETMET	R10	R10	K37
      0x7C280200,  //  02C7  CALL	R10	1
      0x8C28151B,  //  02C8  GETMET	R10	R10	K27
      0x5830001C,  //  02C9  LDCONST	R12	K28
      0x5834001D,  //  02CA  LDCONST	R13	K29
      0x7C280600,  //  02CB  CALL	R10	3
      0x7C1C0600,  //  02CC  CALL	R7	3
      0x80040E00,  //  02CD  RET	1	R7
      0x7002000F,  //  02CE  JMP		#02DF
      0x541E0012,  //  02CF  LDINT	R7	19
      0x1C1C0C07,  //  02D0  EQ	R7	R6	R7
      0x781E000C,  //  02D1  JMPF	R7	#02DF
      0x8C1C090A,  //  02D2  GETMET	R7	R4	K10
      0x7C1C0200,  //  02D3  CALL	R7	1
      0x8C200F0B,  //  02D4  GETMET	R8	R7	K11
      0x58280005,  //  02D5  LDCONST	R10	K5
      0x882C090C,  //  02D6  GETMBR	R11	R4	K12
      0x5830000F,  //  02D7  LDCONST	R12	K15
      0x7C200800,  //  02D8  CALL	R8	4
      0x8C200F0B,  //  02D9  GETMET	R8	R7	K11
      0x58280009,  //  02DA  LDCONST	R10	K9
      0x882C090C,  //  02DB  GETMBR	R11	R4	K12
      0x5830000F,  //  02DC  LDCONST	R12	K15
      0x7C200800,  //  02DD  CALL	R8	4
      0x80040E00,  //  02DE  RET	1	R7
      0x700200A6,  //  02DF  JMP		#0387
      0x541E003E,  //  02E0  LDINT	R7	63
      0x1C1C0A07,  //  02E1  EQ	R7	R5	R7
      0x781E0000,  //  02E2  JMPF	R7	#02E4
      0x700200A2,  //  02E3  JMP		#0387
      0x541E0029,  //  02E4  LDINT	R7	42
      0x1C1C0A07,  //  02E5  EQ	R7	R5	R7
      0x781E001D,  //  02E6  JMPF	R7	#0305
      0x1C1C0D05,  //  02E7  EQ	R7	R6	K5
      0x781E0003,  //  02E8  JMPF	R7	#02ED
      0x8C1C0911,  //  02E9  GETMET	R7	R4	K17
      0x7C1C0200,  //  02EA  CALL	R7	1
      0x80040E00,  //  02EB  RET	1	R7
      0x70020016,  //  02EC  JMP		#0304
      0x1C1C0D09,  //  02ED  EQ	R7	R6	K9
      0x781E0005,  //  02EE  JMPF	R7	#02F5
      0x8C1C0906,  //  02EF  GETMET	R7	R4	K6
      0x88240910,  //  02F0  GETMBR	R9	R4	K16
      0x58280005,  //  02F1  LDCONST	R10	K5
      0x7C1C0600,  //  02F2  CALL	R7	3
      0x80040E00,  //  02F3  RET	1	R7
      0x7002000E,  //  02F4  JMP		#0304
      0x1C1C0D0D,  //  02F5  EQ	R7	R6	K13
      0x781E0005,  //  02F6  JMPF	R7	#02FD
      0x8C1C0906,  //  02F7  GETMET	R7	R4	K6
      0x8824090E,  //  02F8  GETMBR	R9	R4	K14
      0x58280009,  //  02F9  LDCONST	R10	K9
      0x7C1C0600,  //  02FA  CALL	R7	3
      0x80040E00,  //  02FB  RET	1	R7
      0x70020006,  //  02FC  JMP		#0304
      0x1C1C0D0F,  //  02FD  EQ	R7	R6	K15
      0x781E0004,  //  02FE  JMPF	R7	#0304
      0x8C1C0906,  //  02FF  GETMET	R7	R4	K6
      0x88240918,  //  0300  GETMBR	R9	R4	K24
      0x4C280000,  //  0301  LDNIL	R10
      0x7C1C0600,  //  0302  CALL	R7	3
      0x80040E00,  //  0303  RET	1	R7
      0x70020081,  //  0304  JMP		#0387
      0x541E002A,  //  0305  LDINT	R7	43
      0x1C1C0A07,  //  0306  EQ	R7	R5	R7
      0x781E0016,  //  0307  JMPF	R7	#031F
      0x1C1C0D05,  //  0308  EQ	R7	R6	K5
      0x781E0007,  //  0309  JMPF	R7	#0312
      0x8C1C0906,  //  030A  GETMET	R7	R4	K6
      0x88240916,  //  030B  GETMBR	R9	R4	K22
      0xB82A2400,  //  030C  GETNGBL	R10	K18
      0x8C281554,  //  030D  GETMET	R10	R10	K84
      0x7C280200,  //  030E  CALL	R10	1
      0x7C1C0600,  //  030F  CALL	R7	3
      0x80040E00,  //  0310  RET	1	R7
      0x7002000B,  //  0311  JMP		#031E
      0x1C1C0D09,  //  0312  EQ	R7	R6	K9
      0x781E0009,  //  0313  JMPF	R7	#031E
      0x8C1C0911,  //  0314  GETMET	R7	R4	K17
      0x7C1C0200,  //  0315  CALL	R7	1
      0x8C200F0B,  //  0316  GETMET	R8	R7	K11
      0x4C280000,  //  0317  LDNIL	R10
      0x882C0916,  //  0318  GETMBR	R11	R4	K22
      0xB8322400,  //  0319  GETNGBL	R12	K18
      0x8C301954,  //  031A  GETMET	R12	R12	K84
      0x7C300200,  //  031B  CALL	R12	1
      0x7C200800,  //  031C  CALL	R8	4
      0x80040E00,  //  031D  RET	1	R7
      0x70020067,  //  031E  JMP		#0387
      0x541E002B,  //  031F  LDINT	R7	44
      0x1C1C0A07,  //  0320  EQ	R7	R5	R7
      0x781E001C,  //  0321  JMPF	R7	#033F
      0x1C1C0D05,  //  0322  EQ	R7	R6	K5
      0x781E0005,  //  0323  JMPF	R7	#032A
      0x8C1C0906,  //  0324  GETMET	R7	R4	K6
      0x8824090E,  //  0325  GETMBR	R9	R4	K14
      0x58280009,  //  0326  LDCONST	R10	K9
      0x7C1C0600,  //  0327  CALL	R7	3
      0x80040E00,  //  0328  RET	1	R7
      0x70020013,  //  0329  JMP		#033E
      0x1C1C0D09,  //  032A  EQ	R7	R6	K9
      0x781E0005,  //  032B  JMPF	R7	#0332
      0x8C1C0906,  //  032C  GETMET	R7	R4	K6
      0x8824090E,  //  032D  GETMBR	R9	R4	K14
      0x542A0003,  //  032E  LDINT	R10	4
      0x7C1C0600,  //  032F  CALL	R7	3
      0x80040E00,  //  0330  RET	1	R7
      0x7002000B,  //  0331  JMP		#033E
      0x1C1C0D0D,  //  0332  EQ	R7	R6	K13
      0x781E0009,  //  0333  JMPF	R7	#033E
      0x8C1C0911,  //  0334  GETMET	R7	R4	K17
      0x7C1C0200,  //  0335  CALL	R7	1
      0x8C200F0B,  //  0336  GETMET	R8	R7	K11
      0x4C280000,  //  0337  LDNIL	R10
      0x8C2C0906,  //  0338  GETMET	R11	R4	K6
      0x8834090E,  //  0339  GETMBR	R13	R4	K14
      0x543A0003,  //  033A  LDINT	R14	4
      0x7C2C0600,  //  033B  CALL	R11	3
      0x7C200600,  //  033C  CALL	R8	3
      0x80040E00,  //  033D  RET	1	R7
      0x70020047,  //  033E  JMP		#0387
      0x541E0030,  //  033F  LDINT	R7	49
      0x1C1C0A07,  //  0340  EQ	R7	R5	R7
      0x781E0010,  //  0341  JMPF	R7	#0353
      0x1C1C0D0F,  //  0342  EQ	R7	R6	K15
      0x781E0005,  //  0343  JMPF	R7	#034A
      0x8C1C0906,  //  0344  GETMET	R7	R4	K6
      0x8824090E,  //  0345  GETMBR	R9	R4	K14
      0x542A001D,  //  0346  LDINT	R10	30
      0x7C1C0600,  //  0347  CALL	R7	3
      0x80040E00,  //  0348  RET	1	R7
      0x70020007,  //  0349  JMP		#0352
      0x541EFFFB,  //  034A  LDINT	R7	65532
      0x1C1C0C07,  //  034B  EQ	R7	R6	R7
      0x781E0004,  //  034C  JMPF	R7	#0352
      0x8C1C0906,  //  034D  GETMET	R7	R4	K6
      0x8824092A,  //  034E  GETMBR	R9	R4	K42
      0x542A0003,  //  034F  LDINT	R10	4
      0x7C1C0600,  //  0350  CALL	R7	3
      0x80040E00,  //  0351  RET	1	R7
      0x70020033,  //  0352  JMP		#0387
      0x541E001C,  //  0353  LDINT	R7	29
      0x1C1C0A07,  //  0354  EQ	R7	R5	R7
      0x781E0028,  //  0355  JMPF	R7	#037F
      0x1C1C0D0F,  //  0356  EQ	R7	R6	K15
      0x781E001D,  //  0357  JMPF	R7	#0376
      0x8C1C0911,  //  0358  GETMET	R7	R4	K17
      0x7C1C0200,  //  0359  CALL	R7	1
      0x88200133,  //  035A  GETMBR	R8	R0	K51
      0x8C201155,  //  035B  GETMET	R8	R8	K85
      0x50280200,  //  035C  LDBOOL	R10	1	0
      0x7C200400,  //  035D  CALL	R8	2
      0x88240133,  //  035E  GETMBR	R9	R0	K51
      0x88241356,  //  035F  GETMBR	R9	R9	K86
      0x60280010,  //  0360  GETGBL	R10	G16
      0x5C2C1000,  //  0361  MOVE	R11	R8
      0x7C280200,  //  0362  CALL	R10	1
      0xA802000C,  //  0363  EXBLK	0	#0371
      0x5C2C1400,  //  0364  MOVE	R11	R10
      0x7C2C0000,  //  0365  CALL	R11	0
      0x5C301200,  //  0366  MOVE	R12	R9
      0x78320002,  //  0367  JMPF	R12	#036B
      0x5432FEFF,  //  0368  LDINT	R12	65280
      0x1430160C,  //  0369  LT	R12	R11	R12
      0x78320004,  //  036A  JMPF	R12	#0370
      0x8C300F0B,  //  036B  GETMET	R12	R7	K11
      0x4C380000,  //  036C  LDNIL	R14
      0x883C090C,  //  036D  GETMBR	R15	R4	K12
      0x5C401600,  //  036E  MOVE	R16	R11
      0x7C300800,  //  036F  CALL	R12	4
      0x7001FFF2,  //  0370  JMP		#0364
      0x5828003A,  //  0371  LDCONST	R10	K58
      0xAC280200,  //  0372  CATCH	R10	1	0
      0xB0080000,  //  0373  RAISE	2	R0	R0
      0x80040E00,  //  0374  RET	1	R7
      0x70020007,  //  0375  JMP		#037E
      0x601C0003,  //  0376  GETGBL	R7	G3
      0x5C200000,  //  0377  MOVE	R8	R0
      0x7C1C0200,  //  0378  CALL	R7	1
      0x8C1C0F57,  //  0379  GETMET	R7	R7	K87
      0x5C240200,  //  037A  MOVE	R9	R1
      0x5C280400,  //  037B  MOVE	R10	R2
      0x7C1C0600,  //  037C  CALL	R7	3
      0x80040E00,  //  037D  RET	1	R7
      0x70020007,  //  037E  JMP		#0387
      0x601C0003,  //  037F  GETGBL	R7	G3
      0x5C200000,  //  0380  MOVE	R8	R0
      0x7C1C0200,  //  0381  CALL	R7	1
      0x8C1C0F57,  //  0382  GETMET	R7	R7	K87
      0x5C240200,  //  0383  MOVE	R9	R1
      0x5C280400,  //  0384  MOVE	R10	R2
      0x7C1C0600,  //  0385  CALL	R7	3
      0x80040E00,  //  0386  RET	1	R7
      0x80000000,  //  0387  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Root_write_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(int),
    /* K6   */  be_nested_str_weak(int64),
    /* K7   */  be_nested_str_weak(_breadcrumb),
    /* K8   */  be_nested_str_weak(attribute_updated),
    /* K9   */  be_nested_str_weak(status),
    /* K10  */  be_nested_str_weak(CONSTRAINT_ERROR),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(INVALID_ACTION),
    }),
    be_str_weak(write_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[100]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E002F,  //  0004  LDINT	R7	48
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0019,  //  0006  JMPF	R7	#0021
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E0016,  //  0008  JMPF	R7	#0020
      0x601C0004,  //  0009  GETGBL	R7	G4
      0x5C200600,  //  000A  MOVE	R8	R3
      0x7C1C0200,  //  000B  CALL	R7	1
      0x1C1C0F05,  //  000C  EQ	R7	R7	K5
      0x741E0004,  //  000D  JMPT	R7	#0013
      0x601C000F,  //  000E  GETGBL	R7	G15
      0x5C200600,  //  000F  MOVE	R8	R3
      0xB8260C00,  //  0010  GETNGBL	R9	K6
      0x7C1C0400,  //  0011  CALL	R7	2
      0x781E0007,  //  0012  JMPF	R7	#001B
      0x90060E03,  //  0013  SETMBR	R1	K7	R3
      0x8C1C0108,  //  0014  GETMET	R7	R0	K8
      0x88240502,  //  0015  GETMBR	R9	R2	K2
      0x88280503,  //  0016  GETMBR	R10	R2	K3
      0x7C1C0600,  //  0017  CALL	R7	3
      0x501C0200,  //  0018  LDBOOL	R7	1	0
      0x80040E00,  //  0019  RET	1	R7
      0x70020004,  //  001A  JMP		#0020
      0xB81E0000,  //  001B  GETNGBL	R7	K0
      0x881C0F0A,  //  001C  GETMBR	R7	R7	K10
      0x900A1207,  //  001D  SETMBR	R2	K9	R7
      0x501C0000,  //  001E  LDBOOL	R7	0	0
      0x80040E00,  //  001F  RET	1	R7
      0x70020041,  //  0020  JMP		#0063
      0x541E001E,  //  0021  LDINT	R7	31
      0x1C1C0A07,  //  0022  EQ	R7	R5	R7
      0x781E0004,  //  0023  JMPF	R7	#0029
      0x1C1C0D04,  //  0024  EQ	R7	R6	K4
      0x781E0001,  //  0025  JMPF	R7	#0028
      0x501C0200,  //  0026  LDBOOL	R7	1	0
      0x80040E00,  //  0027  RET	1	R7
      0x70020039,  //  0028  JMP		#0063
      0x541E0027,  //  0029  LDINT	R7	40
      0x1C1C0A07,  //  002A  EQ	R7	R5	R7
      0x781E000B,  //  002B  JMPF	R7	#0038
      0x541E0004,  //  002C  LDINT	R7	5
      0x1C1C0C07,  //  002D  EQ	R7	R6	R7
      0x781E0002,  //  002E  JMPF	R7	#0032
      0x501C0200,  //  002F  LDBOOL	R7	1	0
      0x80040E00,  //  0030  RET	1	R7
      0x70020004,  //  0031  JMP		#0037
      0x541E0005,  //  0032  LDINT	R7	6
      0x1C1C0C07,  //  0033  EQ	R7	R6	R7
      0x781E0001,  //  0034  JMPF	R7	#0037
      0x501C0200,  //  0035  LDBOOL	R7	1	0
      0x80040E00,  //  0036  RET	1	R7
      0x7002002A,  //  0037  JMP		#0063
      0x541E0029,  //  0038  LDINT	R7	42
      0x1C1C0A07,  //  0039  EQ	R7	R5	R7
      0x781E0004,  //  003A  JMPF	R7	#0040
      0x1C1C0D04,  //  003B  EQ	R7	R6	K4
      0x781E0001,  //  003C  JMPF	R7	#003F
      0x501C0200,  //  003D  LDBOOL	R7	1	0
      0x80040E00,  //  003E  RET	1	R7
      0x70020022,  //  003F  JMP		#0063
      0x541E002A,  //  0040  LDINT	R7	43
      0x1C1C0A07,  //  0041  EQ	R7	R5	R7
      0x781E0007,  //  0042  JMPF	R7	#004B
      0x1C1C0D04,  //  0043  EQ	R7	R6	K4
      0x781E0004,  //  0044  JMPF	R7	#004A
      0xB81E0000,  //  0045  GETNGBL	R7	K0
      0x881C0F0A,  //  0046  GETMBR	R7	R7	K10
      0x900A1207,  //  0047  SETMBR	R2	K9	R7
      0x501C0000,  //  0048  LDBOOL	R7	0	0
      0x80040E00,  //  0049  RET	1	R7
      0x70020017,  //  004A  JMP		#0063
      0x541E002B,  //  004B  LDINT	R7	44
      0x1C1C0A07,  //  004C  EQ	R7	R5	R7
      0x781E0009,  //  004D  JMPF	R7	#0058
      0x1C1C0D04,  //  004E  EQ	R7	R6	K4
      0x781E0002,  //  004F  JMPF	R7	#0053
      0x501C0200,  //  0050  LDBOOL	R7	1	0
      0x80040E00,  //  0051  RET	1	R7
      0x70020003,  //  0052  JMP		#0057
      0x1C1C0D0B,  //  0053  EQ	R7	R6	K11
      0x781E0001,  //  0054  JMPF	R7	#0057
      0x501C0200,  //  0055  LDBOOL	R7	1	0
      0x80040E00,  //  0056  RET	1	R7
      0x7002000A,  //  0057  JMP		#0063
      0x541E0030,  //  0058  LDINT	R7	49
      0x1C1C0A07,  //  0059  EQ	R7	R5	R7
      0x781E0007,  //  005A  JMPF	R7	#0063
      0x541E0003,  //  005B  LDINT	R7	4
      0x1C1C0C07,  //  005C  EQ	R7	R6	R7
      0x781E0004,  //  005D  JMPF	R7	#0063
      0xB81E0000,  //  005E  GETNGBL	R7	K0
      0x881C0F0C,  //  005F  GETMBR	R7	R7	K12
      0x900A1207,  //  0060  SETMBR	R2	K9	R7
      0x501C0000,  //  0061  LDBOOL	R7	0	0
      0x80040E00,  //  0062  RET	1	R7
      0x80000000,  //  0063  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Root
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Root,
    0,
    &be_class_Matter_Plugin,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(invoke_request, 2), be_const_closure(Matter_Plugin_Root_invoke_request_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Root_X20node) },
        { be_const_key_weak(CLUSTERS, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(52, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(40, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(15,
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
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
        be_const_int(19),
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
        { be_const_key_int(63, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(56, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(7),
    }))    ) } )) },
        { be_const_key_int(44, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
    }))    ) } )) },
        { be_const_key_int(43, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
    }))    ) } )) },
        { be_const_key_int(31, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
    }))    ) } )) },
        { be_const_key_int(60, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
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
        { be_const_key_int(49, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(4),
        be_const_int(65532),
    }))    ) } )) },
        { be_const_key_int(50, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_weak(TYPES, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(22, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(write_attribute, -1), be_const_closure(Matter_Plugin_Root_write_attribute_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(root) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Root_read_attribute_closure) },
    })),
    be_str_weak(Matter_Plugin_Root)
);
/*******************************************************************/

void be_load_Matter_Plugin_Root_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Root);
    be_setglobal(vm, "Matter_Plugin_Root");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
