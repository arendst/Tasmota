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
    ( &(const bvalue[100]) {     /* constants */
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
    /* K17  */  be_nested_str_weak(_fabric),
    /* K18  */  be_nested_str_weak(fabric_completed),
    /* K19  */  be_nested_str_weak(set_no_expiration),
    /* K20  */  be_nested_str_weak(save),
    /* K21  */  be_nested_str_weak(device),
    /* K22  */  be_nested_str_weak(start_commissioning_complete_deferred),
    /* K23  */  be_nested_str_weak(context_error),
    /* K24  */  be_nested_str_weak(CommissioningComplete_X3A_X20no_X20fabric_X20attached),
    /* K25  */  be_nested_str_weak(status),
    /* K26  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K27  */  be_nested_str_weak(B2),
    /* K28  */  be_nested_str_weak(DAC_Cert_FFF1_8000),
    /* K29  */  be_nested_str_weak(PAI_Cert_FFF1),
    /* K30  */  be_nested_str_weak(CD_FFF1_8000),
    /* K31  */  be_nested_str_weak(B1),
    /* K32  */  be_nested_str_weak(U4),
    /* K33  */  be_nested_str_weak(tasmota),
    /* K34  */  be_nested_str_weak(rtc),
    /* K35  */  be_nested_str_weak(utc),
    /* K36  */  be_nested_str_weak(tlv2raw),
    /* K37  */  be_nested_str_weak(get_ac),
    /* K38  */  be_nested_str_weak(EC_P256),
    /* K39  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K40  */  be_nested_str_weak(DAC_Priv_FFF1_8000),
    /* K41  */  be_nested_str_weak(gen_CSR),
    /* K42  */  be_nested_str_weak(set_temp_ca),
    /* K43  */  be_nested_str_weak(SUCCESS),
    /* K44  */  be_nested_str_weak(log),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20AddNoc_X20Args_X3D),
    /* K46  */  be_nested_str_weak(get_temp_ca),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20AdNOC_X20without_X20CA),
    /* K48  */  be_nested_str_weak(sessions),
    /* K49  */  be_nested_str_weak(create_fabric),
    /* K50  */  be_nested_str_weak(set_ca),
    /* K51  */  be_nested_str_weak(set_noc_icac),
    /* K52  */  be_nested_str_weak(set_ipk_epoch_key),
    /* K53  */  be_nested_str_weak(set_admin_subject_vendor),
    /* K54  */  be_nested_str_weak(set_pk),
    /* K55  */  be_nested_str_weak(get_pk),
    /* K56  */  be_nested_str_weak(parse),
    /* K57  */  be_nested_str_weak(findsub),
    /* K58  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20no_X20fabricid_X20nor_X20deviceid_X20in_X20NOC_X20certificate),
    /* K59  */  be_nested_str_weak(int),
    /* K60  */  be_nested_str_weak(int64),
    /* K61  */  be_nested_str_weak(fromu32),
    /* K62  */  be_nested_str_weak(tobytes),
    /* K63  */  be_nested_str_weak(get_temp_ca_pub),
    /* K64  */  be_const_int(2147483647),
    /* K65  */  be_nested_str_weak(fromstring),
    /* K66  */  be_nested_str_weak(CompressedFabric),
    /* K67  */  be_nested_str_weak(HKDF_SHA256),
    /* K68  */  be_nested_str_weak(copy),
    /* K69  */  be_nested_str_weak(reverse),
    /* K70  */  be_nested_str_weak(derive),
    /* K71  */  be_nested_str_weak(commissioning_admin_fabric),
    /* K72  */  be_nested_str_weak(set_fabric_device),
    /* K73  */  be_nested_str_weak(fabric_candidate),
    /* K74  */  be_nested_str_weak(start_operational_discovery_deferred),
    /* K75  */  be_nested_str_weak(is_PASE),
    /* K76  */  be_nested_str_weak(set_expire_in_seconds),
    /* K77  */  be_nested_str_weak(log_new_fabric),
    /* K78  */  be_nested_str_weak(set_fabric_label),
    /* K79  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Update_X20fabric_X20_X27_X25s_X27_X20label_X3D_X27_X25s_X27),
    /* K80  */  be_nested_str_weak(get_fabric_id),
    /* K81  */  be_nested_str_weak(tohex),
    /* K82  */  be_nested_str_weak(fabric_index_X3A),
    /* K83  */  be_nested_str_weak(active_fabrics),
    /* K84  */  be_nested_str_weak(get_fabric_index),
    /* K85  */  be_nested_str_weak(set_timer),
    /* K86  */  be_nested_str_weak(stop_iteration),
    /* K87  */  be_nested_str_weak(MTR_X3A_X20RemoveFabric_X20fabric_X28),
    /* K88  */  be_nested_str_weak(_X29_X20not_X20found),
    /* K89  */  be_nested_str_weak(INVALID_ACTION),
    /* K90  */  be_nested_str_weak(MTR_X3A_X20OpenCommissioningWindow_X28timeout_X3D_X25i_X2C_X20passcode_X3D_X25s_X2C_X20discriminator_X3D_X25i_X2C_X20iterations_X3D_X25i_X2C_X20salt_X3D_X25s_X29),
    /* K91  */  be_nested_str_weak(INVALID_DATA_TYPE),
    /* K92  */  be_nested_str_weak(MTR_X3A_X20wrong_X20size_X20for_X20PAKE_X20parameters),
    /* K93  */  be_nested_str_weak(CONSTRAINT_ERROR),
    /* K94  */  be_nested_str_weak(start_basic_commissioning),
    /* K95  */  be_nested_str_weak(get_fabric),
    /* K96  */  be_nested_str_weak(MTR_X3A_X20OpenBasicCommissioningWindow_X20commissioning_timeout_X3D),
    /* K97  */  be_nested_str_weak(start_root_basic_commissioning),
    /* K98  */  be_nested_str_weak(stop_basic_commissioning),
    /* K99  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[708]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x5422002F,  //  0005  LDINT	R8	48
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220059,  //  0007  JMPF	R8	#0062
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
      0x7002003E,  //  0021  JMP		#0061
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
      0x70020021,  //  003E  JMP		#0061
      0x54220003,  //  003F  LDINT	R8	4
      0x1C200E08,  //  0040  EQ	R8	R7	R8
      0x7822001E,  //  0041  JMPF	R8	#0061
      0x88200311,  //  0042  GETMBR	R8	R1	K17
      0x7822001B,  //  0043  JMPF	R8	#0060
      0x90061105,  //  0044  SETMBR	R1	K8	K5
      0x88200311,  //  0045  GETMBR	R8	R1	K17
      0x8C201112,  //  0046  GETMET	R8	R8	K18
      0x7C200200,  //  0047  CALL	R8	1
      0x8C200313,  //  0048  GETMET	R8	R1	K19
      0x7C200200,  //  0049  CALL	R8	1
      0x8C200314,  //  004A  GETMET	R8	R1	K20
      0x7C200200,  //  004B  CALL	R8	1
      0x8C200B09,  //  004C  GETMET	R8	R5	K9
      0x7C200200,  //  004D  CALL	R8	1
      0x8C24110A,  //  004E  GETMET	R9	R8	K10
      0x582C0005,  //  004F  LDCONST	R11	K5
      0x88300B0B,  //  0050  GETMBR	R12	R5	K11
      0x58340005,  //  0051  LDCONST	R13	K5
      0x7C240800,  //  0052  CALL	R9	4
      0x8C24110A,  //  0053  GETMET	R9	R8	K10
      0x582C0007,  //  0054  LDCONST	R11	K7
      0x88300B0C,  //  0055  GETMBR	R12	R5	K12
      0x5834000D,  //  0056  LDCONST	R13	K13
      0x7C240800,  //  0057  CALL	R9	4
      0x54260004,  //  0058  LDINT	R9	5
      0x900E0809,  //  0059  SETMBR	R3	K4	R9
      0x88240115,  //  005A  GETMBR	R9	R0	K21
      0x8C241316,  //  005B  GETMET	R9	R9	K22
      0x5C2C0200,  //  005C  MOVE	R11	R1
      0x7C240400,  //  005D  CALL	R9	2
      0x80041000,  //  005E  RET	1	R8
      0x70020000,  //  005F  JMP		#0061
      0xB0062F18,  //  0060  RAISE	1	K23	K24
      0x7002025F,  //  0061  JMP		#02C2
      0x5422003D,  //  0062  LDINT	R8	62
      0x1C200C08,  //  0063  EQ	R8	R6	R8
      0x782201C0,  //  0064  JMPF	R8	#0226
      0x1C200F0E,  //  0065  EQ	R8	R7	K14
      0x7822001D,  //  0066  JMPF	R8	#0085
      0x8C200506,  //  0067  GETMET	R8	R2	K6
      0x58280005,  //  0068  LDCONST	R10	K5
      0x7C200400,  //  0069  CALL	R8	2
      0x20241107,  //  006A  NE	R9	R8	K7
      0x78260006,  //  006B  JMPF	R9	#0073
      0x2024110E,  //  006C  NE	R9	R8	K14
      0x78260004,  //  006D  JMPF	R9	#0073
      0xB8260200,  //  006E  GETNGBL	R9	K1
      0x8824131A,  //  006F  GETMBR	R9	R9	K26
      0x900E3209,  //  0070  SETMBR	R3	K25	R9
      0x4C240000,  //  0071  LDNIL	R9
      0x80041200,  //  0072  RET	1	R9
      0x8C240B09,  //  0073  GETMET	R9	R5	K9
      0x7C240200,  //  0074  CALL	R9	1
      0x8C28130A,  //  0075  GETMET	R10	R9	K10
      0x58300005,  //  0076  LDCONST	R12	K5
      0x88340B1B,  //  0077  GETMBR	R13	R5	K27
      0x1C381107,  //  0078  EQ	R14	R8	K7
      0x783A0003,  //  0079  JMPF	R14	#007E
      0xB83A0200,  //  007A  GETNGBL	R14	K1
      0x8C381D1C,  //  007B  GETMET	R14	R14	K28
      0x7C380200,  //  007C  CALL	R14	1
      0x70020002,  //  007D  JMP		#0081
      0xB83A0200,  //  007E  GETNGBL	R14	K1
      0x8C381D1D,  //  007F  GETMET	R14	R14	K29
      0x7C380200,  //  0080  CALL	R14	1
      0x7C280800,  //  0081  CALL	R10	4
      0x900E0910,  //  0082  SETMBR	R3	K4	K16
      0x80041200,  //  0083  RET	1	R9
      0x7002019F,  //  0084  JMP		#0225
      0x1C200F05,  //  0085  EQ	R8	R7	K5
      0x7822003D,  //  0086  JMPF	R8	#00C5
      0x8C200506,  //  0087  GETMET	R8	R2	K6
      0x58280005,  //  0088  LDCONST	R10	K5
      0x7C200400,  //  0089  CALL	R8	2
      0x6024000C,  //  008A  GETGBL	R9	G12
      0x5C281000,  //  008B  MOVE	R10	R8
      0x7C240200,  //  008C  CALL	R9	1
      0x542A001F,  //  008D  LDINT	R10	32
      0x2024120A,  //  008E  NE	R9	R9	R10
      0x78260001,  //  008F  JMPF	R9	#0092
      0x4C240000,  //  0090  LDNIL	R9
      0x80041200,  //  0091  RET	1	R9
      0x900E0907,  //  0092  SETMBR	R3	K4	K7
      0x8C240B09,  //  0093  GETMET	R9	R5	K9
      0x7C240200,  //  0094  CALL	R9	1
      0x8C28130A,  //  0095  GETMET	R10	R9	K10
      0x58300007,  //  0096  LDCONST	R12	K7
      0x88340B1B,  //  0097  GETMBR	R13	R5	K27
      0xB83A0200,  //  0098  GETNGBL	R14	K1
      0x8C381D1E,  //  0099  GETMET	R14	R14	K30
      0x7C380200,  //  009A  CALL	R14	1
      0x7C280800,  //  009B  CALL	R10	4
      0x8C28130A,  //  009C  GETMET	R10	R9	K10
      0x5830000E,  //  009D  LDCONST	R12	K14
      0x88340B1F,  //  009E  GETMBR	R13	R5	K31
      0x5C381000,  //  009F  MOVE	R14	R8
      0x7C280800,  //  00A0  CALL	R10	4
      0x8C28130A,  //  00A1  GETMET	R10	R9	K10
      0x58300010,  //  00A2  LDCONST	R12	K16
      0x88340B20,  //  00A3  GETMBR	R13	R5	K32
      0xB83A4200,  //  00A4  GETNGBL	R14	K33
      0x8C381D22,  //  00A5  GETMET	R14	R14	K34
      0x7C380200,  //  00A6  CALL	R14	1
      0x94381D23,  //  00A7  GETIDX	R14	R14	K35
      0x7C280800,  //  00A8  CALL	R10	4
      0x8C281324,  //  00A9  GETMET	R10	R9	K36
      0x7C280200,  //  00AA  CALL	R10	1
      0x8C2C0325,  //  00AB  GETMET	R11	R1	K37
      0x7C2C0200,  //  00AC  CALL	R11	1
      0x0030140B,  //  00AD  ADD	R12	R10	R11
      0x8C340926,  //  00AE  GETMET	R13	R4	K38
      0x7C340200,  //  00AF  CALL	R13	1
      0x8C341B27,  //  00B0  GETMET	R13	R13	K39
      0xB83E0200,  //  00B1  GETNGBL	R15	K1
      0x8C3C1F28,  //  00B2  GETMET	R15	R15	K40
      0x7C3C0200,  //  00B3  CALL	R15	1
      0x5C401800,  //  00B4  MOVE	R16	R12
      0x7C340600,  //  00B5  CALL	R13	3
      0x8C380B09,  //  00B6  GETMET	R14	R5	K9
      0x7C380200,  //  00B7  CALL	R14	1
      0x8C3C1D0A,  //  00B8  GETMET	R15	R14	K10
      0x58440005,  //  00B9  LDCONST	R17	K5
      0x88480B1B,  //  00BA  GETMBR	R18	R5	K27
      0x5C4C1400,  //  00BB  MOVE	R19	R10
      0x7C3C0800,  //  00BC  CALL	R15	4
      0x8C3C1D0A,  //  00BD  GETMET	R15	R14	K10
      0x58440007,  //  00BE  LDCONST	R17	K7
      0x88480B1F,  //  00BF  GETMBR	R18	R5	K31
      0x5C4C1A00,  //  00C0  MOVE	R19	R13
      0x7C3C0800,  //  00C1  CALL	R15	4
      0x900E0907,  //  00C2  SETMBR	R3	K4	K7
      0x80041C00,  //  00C3  RET	1	R14
      0x7002015F,  //  00C4  JMP		#0225
      0x54220003,  //  00C5  LDINT	R8	4
      0x1C200E08,  //  00C6  EQ	R8	R7	R8
      0x78220039,  //  00C7  JMPF	R8	#0102
      0x8C200506,  //  00C8  GETMET	R8	R2	K6
      0x58280005,  //  00C9  LDCONST	R10	K5
      0x7C200400,  //  00CA  CALL	R8	2
      0x6024000C,  //  00CB  GETGBL	R9	G12
      0x5C281000,  //  00CC  MOVE	R10	R8
      0x7C240200,  //  00CD  CALL	R9	1
      0x542A001F,  //  00CE  LDINT	R10	32
      0x2024120A,  //  00CF  NE	R9	R9	R10
      0x78260001,  //  00D0  JMPF	R9	#00D3
      0x4C240000,  //  00D1  LDNIL	R9
      0x80041200,  //  00D2  RET	1	R9
      0x8C240506,  //  00D3  GETMET	R9	R2	K6
      0x582C0007,  //  00D4  LDCONST	R11	K7
      0x50300000,  //  00D5  LDBOOL	R12	0	0
      0x7C240600,  //  00D6  CALL	R9	3
      0x8C280329,  //  00D7  GETMET	R10	R1	K41
      0x7C280200,  //  00D8  CALL	R10	1
      0x8C2C0B09,  //  00D9  GETMET	R11	R5	K9
      0x7C2C0200,  //  00DA  CALL	R11	1
      0x8C30170A,  //  00DB  GETMET	R12	R11	K10
      0x58380007,  //  00DC  LDCONST	R14	K7
      0x883C0B1B,  //  00DD  GETMBR	R15	R5	K27
      0x5C401400,  //  00DE  MOVE	R16	R10
      0x7C300800,  //  00DF  CALL	R12	4
      0x8C30170A,  //  00E0  GETMET	R12	R11	K10
      0x5838000E,  //  00E1  LDCONST	R14	K14
      0x883C0B1F,  //  00E2  GETMBR	R15	R5	K31
      0x5C401000,  //  00E3  MOVE	R16	R8
      0x7C300800,  //  00E4  CALL	R12	4
      0x8C301724,  //  00E5  GETMET	R12	R11	K36
      0x7C300200,  //  00E6  CALL	R12	1
      0x8C340325,  //  00E7  GETMET	R13	R1	K37
      0x7C340200,  //  00E8  CALL	R13	1
      0x0034180D,  //  00E9  ADD	R13	R12	R13
      0x8C380926,  //  00EA  GETMET	R14	R4	K38
      0x7C380200,  //  00EB  CALL	R14	1
      0x8C381D27,  //  00EC  GETMET	R14	R14	K39
      0xB8420200,  //  00ED  GETNGBL	R16	K1
      0x8C402128,  //  00EE  GETMET	R16	R16	K40
      0x7C400200,  //  00EF  CALL	R16	1
      0x5C441A00,  //  00F0  MOVE	R17	R13
      0x7C380600,  //  00F1  CALL	R14	3
      0x8C3C0B09,  //  00F2  GETMET	R15	R5	K9
      0x7C3C0200,  //  00F3  CALL	R15	1
      0x8C401F0A,  //  00F4  GETMET	R16	R15	K10
      0x58480005,  //  00F5  LDCONST	R18	K5
      0x884C0B1B,  //  00F6  GETMBR	R19	R5	K27
      0x5C501800,  //  00F7  MOVE	R20	R12
      0x7C400800,  //  00F8  CALL	R16	4
      0x8C401F0A,  //  00F9  GETMET	R16	R15	K10
      0x58480007,  //  00FA  LDCONST	R18	K7
      0x884C0B1F,  //  00FB  GETMBR	R19	R5	K31
      0x5C501C00,  //  00FC  MOVE	R20	R14
      0x7C400800,  //  00FD  CALL	R16	4
      0x54420004,  //  00FE  LDINT	R16	5
      0x900E0810,  //  00FF  SETMBR	R3	K4	R16
      0x80041E00,  //  0100  RET	1	R15
      0x70020122,  //  0101  JMP		#0225
      0x5422000A,  //  0102  LDINT	R8	11
      0x1C200E08,  //  0103  EQ	R8	R7	R8
      0x7822000B,  //  0104  JMPF	R8	#0111
      0x8C200506,  //  0105  GETMET	R8	R2	K6
      0x58280005,  //  0106  LDCONST	R10	K5
      0x7C200400,  //  0107  CALL	R8	2
      0x8C24032A,  //  0108  GETMET	R9	R1	K42
      0x5C2C1000,  //  0109  MOVE	R11	R8
      0x7C240400,  //  010A  CALL	R9	2
      0xB8260200,  //  010B  GETNGBL	R9	K1
      0x8824132B,  //  010C  GETMBR	R9	R9	K43
      0x900E3209,  //  010D  SETMBR	R3	K25	R9
      0x4C240000,  //  010E  LDNIL	R9
      0x80041200,  //  010F  RET	1	R9
      0x70020113,  //  0110  JMP		#0225
      0x54220005,  //  0111  LDINT	R8	6
      0x1C200E08,  //  0112  EQ	R8	R7	R8
      0x782200B9,  //  0113  JMPF	R8	#01CE
      0xB8224200,  //  0114  GETNGBL	R8	K33
      0x8C20112C,  //  0115  GETMET	R8	R8	K44
      0x60280008,  //  0116  GETGBL	R10	G8
      0x5C2C0400,  //  0117  MOVE	R11	R2
      0x7C280200,  //  0118  CALL	R10	1
      0x002A5A0A,  //  0119  ADD	R10	K45	R10
      0x542E0003,  //  011A  LDINT	R11	4
      0x7C200600,  //  011B  CALL	R8	3
      0x8C200506,  //  011C  GETMET	R8	R2	K6
      0x58280005,  //  011D  LDCONST	R10	K5
      0x7C200400,  //  011E  CALL	R8	2
      0x8C240506,  //  011F  GETMET	R9	R2	K6
      0x582C0007,  //  0120  LDCONST	R11	K7
      0x7C240400,  //  0121  CALL	R9	2
      0x6028000C,  //  0122  GETGBL	R10	G12
      0x5C2C1200,  //  0123  MOVE	R11	R9
      0x7C280200,  //  0124  CALL	R10	1
      0x1C281505,  //  0125  EQ	R10	R10	K5
      0x782A0000,  //  0126  JMPF	R10	#0128
      0x4C240000,  //  0127  LDNIL	R9
      0x8C280506,  //  0128  GETMET	R10	R2	K6
      0x5830000E,  //  0129  LDCONST	R12	K14
      0x7C280400,  //  012A  CALL	R10	2
      0x8C2C0506,  //  012B  GETMET	R11	R2	K6
      0x58340010,  //  012C  LDCONST	R13	K16
      0x7C2C0400,  //  012D  CALL	R11	2
      0x8C300506,  //  012E  GETMET	R12	R2	K6
      0x543A0003,  //  012F  LDINT	R14	4
      0x7C300400,  //  0130  CALL	R12	2
      0x8C34032E,  //  0131  GETMET	R13	R1	K46
      0x7C340200,  //  0132  CALL	R13	1
      0x4C380000,  //  0133  LDNIL	R14
      0x1C341A0E,  //  0134  EQ	R13	R13	R14
      0x78360006,  //  0135  JMPF	R13	#013D
      0xB8364200,  //  0136  GETNGBL	R13	K33
      0x8C341B2C,  //  0137  GETMET	R13	R13	K44
      0x583C002F,  //  0138  LDCONST	R15	K47
      0x5840000E,  //  0139  LDCONST	R16	K14
      0x7C340600,  //  013A  CALL	R13	3
      0x4C340000,  //  013B  LDNIL	R13
      0x80041A00,  //  013C  RET	1	R13
      0x88340115,  //  013D  GETMBR	R13	R0	K21
      0x88341B30,  //  013E  GETMBR	R13	R13	K48
      0x8C341B31,  //  013F  GETMET	R13	R13	K49
      0x7C340200,  //  0140  CALL	R13	1
      0x8C381B32,  //  0141  GETMET	R14	R13	K50
      0x8C40032E,  //  0142  GETMET	R16	R1	K46
      0x7C400200,  //  0143  CALL	R16	1
      0x7C380400,  //  0144  CALL	R14	2
      0x8C381B33,  //  0145  GETMET	R14	R13	K51
      0x5C401000,  //  0146  MOVE	R16	R8
      0x5C441200,  //  0147  MOVE	R17	R9
      0x7C380600,  //  0148  CALL	R14	3
      0x8C381B34,  //  0149  GETMET	R14	R13	K52
      0x5C401400,  //  014A  MOVE	R16	R10
      0x7C380400,  //  014B  CALL	R14	2
      0x8C381B35,  //  014C  GETMET	R14	R13	K53
      0x5C401600,  //  014D  MOVE	R16	R11
      0x5C441800,  //  014E  MOVE	R17	R12
      0x7C380600,  //  014F  CALL	R14	3
      0x8C381B36,  //  0150  GETMET	R14	R13	K54
      0x8C400337,  //  0151  GETMET	R16	R1	K55
      0x7C400200,  //  0152  CALL	R16	1
      0x7C380400,  //  0153  CALL	R14	2
      0xB83A0200,  //  0154  GETNGBL	R14	K1
      0x88381D02,  //  0155  GETMBR	R14	R14	K2
      0x8C381D38,  //  0156  GETMET	R14	R14	K56
      0x5C401000,  //  0157  MOVE	R16	R8
      0x7C380400,  //  0158  CALL	R14	2
      0x8C3C1D39,  //  0159  GETMET	R15	R14	K57
      0x54460005,  //  015A  LDINT	R17	6
      0x7C3C0400,  //  015B  CALL	R15	2
      0x8C401F06,  //  015C  GETMET	R16	R15	K6
      0x544A0014,  //  015D  LDINT	R18	21
      0x7C400400,  //  015E  CALL	R16	2
      0x8C441F06,  //  015F  GETMET	R17	R15	K6
      0x544E0010,  //  0160  LDINT	R19	17
      0x7C440400,  //  0161  CALL	R17	2
      0x5C482000,  //  0162  MOVE	R18	R16
      0x784A0001,  //  0163  JMPF	R18	#0166
      0x5C482200,  //  0164  MOVE	R18	R17
      0x744A0006,  //  0165  JMPT	R18	#016D
      0xB84A4200,  //  0166  GETNGBL	R18	K33
      0x8C48252C,  //  0167  GETMET	R18	R18	K44
      0x5850003A,  //  0168  LDCONST	R20	K58
      0x5854000E,  //  0169  LDCONST	R21	K14
      0x7C480600,  //  016A  CALL	R18	3
      0x50480000,  //  016B  LDBOOL	R18	0	0
      0x80042400,  //  016C  RET	1	R18
      0x60480004,  //  016D  GETGBL	R18	G4
      0x5C4C2000,  //  016E  MOVE	R19	R16
      0x7C480200,  //  016F  CALL	R18	1
      0x1C48253B,  //  0170  EQ	R18	R18	K59
      0x784A0007,  //  0171  JMPF	R18	#017A
      0xB84A7800,  //  0172  GETNGBL	R18	K60
      0x8C48253D,  //  0173  GETMET	R18	R18	K61
      0x5C502000,  //  0174  MOVE	R20	R16
      0x7C480400,  //  0175  CALL	R18	2
      0x8C48253E,  //  0176  GETMET	R18	R18	K62
      0x7C480200,  //  0177  CALL	R18	1
      0x5C402400,  //  0178  MOVE	R16	R18
      0x70020002,  //  0179  JMP		#017D
      0x8C48213E,  //  017A  GETMET	R18	R16	K62
      0x7C480200,  //  017B  CALL	R18	1
      0x5C402400,  //  017C  MOVE	R16	R18
      0x60480004,  //  017D  GETGBL	R18	G4
      0x5C4C2200,  //  017E  MOVE	R19	R17
      0x7C480200,  //  017F  CALL	R18	1
      0x1C48253B,  //  0180  EQ	R18	R18	K59
      0x784A0007,  //  0181  JMPF	R18	#018A
      0xB84A7800,  //  0182  GETNGBL	R18	K60
      0x8C48253D,  //  0183  GETMET	R18	R18	K61
      0x5C502200,  //  0184  MOVE	R20	R17
      0x7C480400,  //  0185  CALL	R18	2
      0x8C48253E,  //  0186  GETMET	R18	R18	K62
      0x7C480200,  //  0187  CALL	R18	1
      0x5C442400,  //  0188  MOVE	R17	R18
      0x70020002,  //  0189  JMP		#018D
      0x8C48233E,  //  018A  GETMET	R18	R17	K62
      0x7C480200,  //  018B  CALL	R18	1
      0x5C442400,  //  018C  MOVE	R17	R18
      0x8C48033F,  //  018D  GETMET	R18	R1	K63
      0x7C480200,  //  018E  CALL	R18	1
      0x404E0F40,  //  018F  CONNECT	R19	K7	K64
      0x94482413,  //  0190  GETIDX	R18	R18	R19
      0x60500015,  //  0191  GETGBL	R20	G21
      0x7C500000,  //  0192  CALL	R20	0
      0x8C502941,  //  0193  GETMET	R20	R20	K65
      0x58580042,  //  0194  LDCONST	R22	K66
      0x7C500400,  //  0195  CALL	R20	2
      0x5C4C2800,  //  0196  MOVE	R19	R20
      0x8C500943,  //  0197  GETMET	R20	R4	K67
      0x7C500200,  //  0198  CALL	R20	1
      0x8C542144,  //  0199  GETMET	R21	R16	K68
      0x7C540200,  //  019A  CALL	R21	1
      0x8C542B45,  //  019B  GETMET	R21	R21	K69
      0x7C540200,  //  019C  CALL	R21	1
      0x8C582946,  //  019D  GETMET	R22	R20	K70
      0x5C602400,  //  019E  MOVE	R24	R18
      0x5C642A00,  //  019F  MOVE	R25	R21
      0x5C682600,  //  01A0  MOVE	R26	R19
      0x546E0007,  //  01A1  LDINT	R27	8
      0x7C580A00,  //  01A2  CALL	R22	5
      0x885C0311,  //  01A3  GETMBR	R23	R1	K17
      0x785E0001,  //  01A4  JMPF	R23	#01A7
      0x885C0311,  //  01A5  GETMBR	R23	R1	K17
      0x70020001,  //  01A6  JMP		#01A9
      0x885C0115,  //  01A7  GETMBR	R23	R0	K21
      0x885C2F47,  //  01A8  GETMBR	R23	R23	K71
      0x8C601B48,  //  01A9  GETMET	R24	R13	K72
      0x5C682000,  //  01AA  MOVE	R26	R16
      0x5C6C2200,  //  01AB  MOVE	R27	R17
      0x5C702C00,  //  01AC  MOVE	R28	R22
      0x5C742E00,  //  01AD  MOVE	R29	R23
      0x7C600A00,  //  01AE  CALL	R24	5
      0x8C601B49,  //  01AF  GETMET	R24	R13	K73
      0x7C600200,  //  01B0  CALL	R24	1
      0x88600115,  //  01B1  GETMBR	R24	R0	K21
      0x8C60314A,  //  01B2  GETMET	R24	R24	K74
      0x5C681A00,  //  01B3  MOVE	R26	R13
      0x7C600400,  //  01B4  CALL	R24	2
      0x8C60034B,  //  01B5  GETMET	R24	R1	K75
      0x7C600200,  //  01B6  CALL	R24	1
      0x78620002,  //  01B7  JMPF	R24	#01BB
      0x8C60034C,  //  01B8  GETMET	R24	R1	K76
      0x546A003B,  //  01B9  LDINT	R26	60
      0x7C600400,  //  01BA  CALL	R24	2
      0x8C601B4D,  //  01BB  GETMET	R24	R13	K77
      0x7C600200,  //  01BC  CALL	R24	1
      0x8C600B09,  //  01BD  GETMET	R24	R5	K9
      0x7C600200,  //  01BE  CALL	R24	1
      0x8C64310A,  //  01BF  GETMET	R25	R24	K10
      0x586C0005,  //  01C0  LDCONST	R27	K5
      0x88700B0B,  //  01C1  GETMBR	R28	R5	K11
      0xB8760200,  //  01C2  GETNGBL	R29	K1
      0x88743B2B,  //  01C3  GETMBR	R29	R29	K43
      0x7C640800,  //  01C4  CALL	R25	4
      0x8C64310A,  //  01C5  GETMET	R25	R24	K10
      0x586C0007,  //  01C6  LDCONST	R27	K7
      0x88700B0B,  //  01C7  GETMBR	R28	R5	K11
      0x58740007,  //  01C8  LDCONST	R29	K7
      0x7C640800,  //  01C9  CALL	R25	4
      0x54660007,  //  01CA  LDINT	R25	8
      0x900E0819,  //  01CB  SETMBR	R3	K4	R25
      0x80043000,  //  01CC  RET	1	R24
      0x70020056,  //  01CD  JMP		#0225
      0x54220008,  //  01CE  LDINT	R8	9
      0x1C200E08,  //  01CF  EQ	R8	R7	R8
      0x7822001E,  //  01D0  JMPF	R8	#01F0
      0x8C200506,  //  01D1  GETMET	R8	R2	K6
      0x58280005,  //  01D2  LDCONST	R10	K5
      0x7C200400,  //  01D3  CALL	R8	2
      0x8C24034E,  //  01D4  GETMET	R9	R1	K78
      0x5C2C1000,  //  01D5  MOVE	R11	R8
      0x7C240400,  //  01D6  CALL	R9	2
      0xB8264200,  //  01D7  GETNGBL	R9	K33
      0x8C24132C,  //  01D8  GETMET	R9	R9	K44
      0x602C0018,  //  01D9  GETGBL	R11	G24
      0x5830004F,  //  01DA  LDCONST	R12	K79
      0x88340311,  //  01DB  GETMBR	R13	R1	K17
      0x8C341B50,  //  01DC  GETMET	R13	R13	K80
      0x7C340200,  //  01DD  CALL	R13	1
      0x8C341B44,  //  01DE  GETMET	R13	R13	K68
      0x7C340200,  //  01DF  CALL	R13	1
      0x8C341B45,  //  01E0  GETMET	R13	R13	K69
      0x7C340200,  //  01E1  CALL	R13	1
      0x8C341B51,  //  01E2  GETMET	R13	R13	K81
      0x7C340200,  //  01E3  CALL	R13	1
      0x60380008,  //  01E4  GETGBL	R14	G8
      0x5C3C1000,  //  01E5  MOVE	R15	R8
      0x7C380200,  //  01E6  CALL	R14	1
      0x7C2C0600,  //  01E7  CALL	R11	3
      0x58300010,  //  01E8  LDCONST	R12	K16
      0x7C240600,  //  01E9  CALL	R9	3
      0xB8260200,  //  01EA  GETNGBL	R9	K1
      0x8824132B,  //  01EB  GETMBR	R9	R9	K43
      0x900E3209,  //  01EC  SETMBR	R3	K25	R9
      0x4C240000,  //  01ED  LDNIL	R9
      0x80041200,  //  01EE  RET	1	R9
      0x70020034,  //  01EF  JMP		#0225
      0x54220009,  //  01F0  LDINT	R8	10
      0x1C200E08,  //  01F1  EQ	R8	R7	R8
      0x78220031,  //  01F2  JMPF	R8	#0225
      0x8C200506,  //  01F3  GETMET	R8	R2	K6
      0x58280005,  //  01F4  LDCONST	R10	K5
      0x7C200400,  //  01F5  CALL	R8	2
      0x60240008,  //  01F6  GETGBL	R9	G8
      0x5C281000,  //  01F7  MOVE	R10	R8
      0x7C240200,  //  01F8  CALL	R9	1
      0x0026A409,  //  01F9  ADD	R9	K82	R9
      0x900E5809,  //  01FA  SETMBR	R3	K44	R9
      0x60240010,  //  01FB  GETGBL	R9	G16
      0x88280115,  //  01FC  GETMBR	R10	R0	K21
      0x88281530,  //  01FD  GETMBR	R10	R10	K48
      0x8C281553,  //  01FE  GETMET	R10	R10	K83
      0x7C280200,  //  01FF  CALL	R10	1
      0x7C240200,  //  0200  CALL	R9	1
      0xA8020010,  //  0201  EXBLK	0	#0213
      0x5C281200,  //  0202  MOVE	R10	R9
      0x7C280000,  //  0203  CALL	R10	0
      0x8C2C1554,  //  0204  GETMET	R11	R10	K84
      0x7C2C0200,  //  0205  CALL	R11	1
      0x1C2C1608,  //  0206  EQ	R11	R11	R8
      0x782E0008,  //  0207  JMPF	R11	#0211
      0xB82E4200,  //  0208  GETNGBL	R11	K33
      0x8C2C1755,  //  0209  GETMET	R11	R11	K85
      0x543607CF,  //  020A  LDINT	R13	2000
      0x84380000,  //  020B  CLOSURE	R14	P0
      0x7C2C0600,  //  020C  CALL	R11	3
      0x502C0200,  //  020D  LDBOOL	R11	1	0
      0xA0000000,  //  020E  CLOSE	R0
      0xA8040001,  //  020F  EXBLK	1	1
      0x80041600,  //  0210  RET	1	R11
      0xA0240000,  //  0211  CLOSE	R9
      0x7001FFEE,  //  0212  JMP		#0202
      0x58240056,  //  0213  LDCONST	R9	K86
      0xAC240200,  //  0214  CATCH	R9	1	0
      0xB0080000,  //  0215  RAISE	2	R0	R0
      0xB8264200,  //  0216  GETNGBL	R9	K33
      0x8C24132C,  //  0217  GETMET	R9	R9	K44
      0x602C0008,  //  0218  GETGBL	R11	G8
      0x5C301000,  //  0219  MOVE	R12	R8
      0x7C2C0200,  //  021A  CALL	R11	1
      0x002EAE0B,  //  021B  ADD	R11	K87	R11
      0x002C1758,  //  021C  ADD	R11	R11	K88
      0x5830000E,  //  021D  LDCONST	R12	K14
      0x7C240600,  //  021E  CALL	R9	3
      0xB8260200,  //  021F  GETNGBL	R9	K1
      0x88241359,  //  0220  GETMBR	R9	R9	K89
      0x900E3209,  //  0221  SETMBR	R3	K25	R9
      0x4C240000,  //  0222  LDNIL	R9
      0xA0000000,  //  0223  CLOSE	R0
      0x80041200,  //  0224  RET	1	R9
      0x7002009B,  //  0225  JMP		#02C2
      0x5422003B,  //  0226  LDINT	R8	60
      0x1C200C08,  //  0227  EQ	R8	R6	R8
      0x78220085,  //  0228  JMPF	R8	#02AF
      0x1C200F05,  //  0229  EQ	R8	R7	K5
      0x78220065,  //  022A  JMPF	R8	#0291
      0x8C200506,  //  022B  GETMET	R8	R2	K6
      0x58280005,  //  022C  LDCONST	R10	K5
      0x7C200400,  //  022D  CALL	R8	2
      0x8C240506,  //  022E  GETMET	R9	R2	K6
      0x582C0007,  //  022F  LDCONST	R11	K7
      0x7C240400,  //  0230  CALL	R9	2
      0x8C280506,  //  0231  GETMET	R10	R2	K6
      0x5830000E,  //  0232  LDCONST	R12	K14
      0x7C280400,  //  0233  CALL	R10	2
      0x8C2C0506,  //  0234  GETMET	R11	R2	K6
      0x58340010,  //  0235  LDCONST	R13	K16
      0x7C2C0400,  //  0236  CALL	R11	2
      0x8C300506,  //  0237  GETMET	R12	R2	K6
      0x543A0003,  //  0238  LDINT	R14	4
      0x7C300400,  //  0239  CALL	R12	2
      0xB8364200,  //  023A  GETNGBL	R13	K33
      0x8C341B2C,  //  023B  GETMET	R13	R13	K44
      0x603C0018,  //  023C  GETGBL	R15	G24
      0x5840005A,  //  023D  LDCONST	R16	K90
      0x5C441000,  //  023E  MOVE	R17	R8
      0x8C481351,  //  023F  GETMET	R18	R9	K81
      0x7C480200,  //  0240  CALL	R18	1
      0x5C4C1400,  //  0241  MOVE	R19	R10
      0x5C501600,  //  0242  MOVE	R20	R11
      0x8C541951,  //  0243  GETMET	R21	R12	K81
      0x7C540200,  //  0244  CALL	R21	1
      0x7C3C0C00,  //  0245  CALL	R15	6
      0x54420003,  //  0246  LDINT	R16	4
      0x7C340600,  //  0247  CALL	R13	3
      0x4C340000,  //  0248  LDNIL	R13
      0x1C34100D,  //  0249  EQ	R13	R8	R13
      0x7436000B,  //  024A  JMPT	R13	#0257
      0x4C340000,  //  024B  LDNIL	R13
      0x1C34120D,  //  024C  EQ	R13	R9	R13
      0x74360008,  //  024D  JMPT	R13	#0257
      0x4C340000,  //  024E  LDNIL	R13
      0x1C34140D,  //  024F  EQ	R13	R10	R13
      0x74360005,  //  0250  JMPT	R13	#0257
      0x4C340000,  //  0251  LDNIL	R13
      0x1C34160D,  //  0252  EQ	R13	R11	R13
      0x74360002,  //  0253  JMPT	R13	#0257
      0x4C340000,  //  0254  LDNIL	R13
      0x1C34180D,  //  0255  EQ	R13	R12	R13
      0x78360005,  //  0256  JMPF	R13	#025D
      0xB8360200,  //  0257  GETNGBL	R13	K1
      0x88341B5B,  //  0258  GETMBR	R13	R13	K91
      0x900E320D,  //  0259  SETMBR	R3	K25	R13
      0x4C340000,  //  025A  LDNIL	R13
      0xA0000000,  //  025B  CLOSE	R0
      0x80041A00,  //  025C  RET	1	R13
      0x6034000C,  //  025D  GETGBL	R13	G12
      0x5C381200,  //  025E  MOVE	R14	R9
      0x7C340200,  //  025F  CALL	R13	1
      0x543A001F,  //  0260  LDINT	R14	32
      0x543E0040,  //  0261  LDINT	R15	65
      0x00381C0F,  //  0262  ADD	R14	R14	R15
      0x20341A0E,  //  0263  NE	R13	R13	R14
      0x7436000B,  //  0264  JMPT	R13	#0271
      0x6034000C,  //  0265  GETGBL	R13	G12
      0x5C381800,  //  0266  MOVE	R14	R12
      0x7C340200,  //  0267  CALL	R13	1
      0x543A000F,  //  0268  LDINT	R14	16
      0x14341A0E,  //  0269  LT	R13	R13	R14
      0x74360005,  //  026A  JMPT	R13	#0271
      0x6034000C,  //  026B  GETGBL	R13	G12
      0x5C381800,  //  026C  MOVE	R14	R12
      0x7C340200,  //  026D  CALL	R13	1
      0x543A001F,  //  026E  LDINT	R14	32
      0x24341A0E,  //  026F  GT	R13	R13	R14
      0x7836000A,  //  0270  JMPF	R13	#027C
      0xB8364200,  //  0271  GETNGBL	R13	K33
      0x8C341B2C,  //  0272  GETMET	R13	R13	K44
      0x583C005C,  //  0273  LDCONST	R15	K92
      0x5840000E,  //  0274  LDCONST	R16	K14
      0x7C340600,  //  0275  CALL	R13	3
      0xB8360200,  //  0276  GETNGBL	R13	K1
      0x88341B5D,  //  0277  GETMBR	R13	R13	K93
      0x900E320D,  //  0278  SETMBR	R3	K25	R13
      0x4C340000,  //  0279  LDNIL	R13
      0xA0000000,  //  027A  CLOSE	R0
      0x80041A00,  //  027B  RET	1	R13
      0x5436001E,  //  027C  LDINT	R13	31
      0x40360A0D,  //  027D  CONNECT	R13	K5	R13
      0x9434120D,  //  027E  GETIDX	R13	R9	R13
      0x543A001F,  //  027F  LDINT	R14	32
      0x40381D40,  //  0280  CONNECT	R14	R14	K64
      0x9438120E,  //  0281  GETIDX	R14	R9	R14
      0x883C0115,  //  0282  GETMBR	R15	R0	K21
      0x8C3C1F5E,  //  0283  GETMET	R15	R15	K94
      0x5C441000,  //  0284  MOVE	R17	R8
      0x5C481600,  //  0285  MOVE	R18	R11
      0x5C4C1400,  //  0286  MOVE	R19	R10
      0x5C501800,  //  0287  MOVE	R20	R12
      0x5C541A00,  //  0288  MOVE	R21	R13
      0x5C581C00,  //  0289  MOVE	R22	R14
      0x8C5C035F,  //  028A  GETMET	R23	R1	K95
      0x7C5C0200,  //  028B  CALL	R23	1
      0x7C3C1000,  //  028C  CALL	R15	8
      0x503C0200,  //  028D  LDBOOL	R15	1	0
      0xA0000000,  //  028E  CLOSE	R0
      0x80041E00,  //  028F  RET	1	R15
      0x7002001C,  //  0290  JMP		#02AE
      0x1C200F07,  //  0291  EQ	R8	R7	K7
      0x78220012,  //  0292  JMPF	R8	#02A6
      0x8C200506,  //  0293  GETMET	R8	R2	K6
      0x58280005,  //  0294  LDCONST	R10	K5
      0x7C200400,  //  0295  CALL	R8	2
      0xB8264200,  //  0296  GETNGBL	R9	K33
      0x8C24132C,  //  0297  GETMET	R9	R9	K44
      0x602C0008,  //  0298  GETGBL	R11	G8
      0x5C301000,  //  0299  MOVE	R12	R8
      0x7C2C0200,  //  029A  CALL	R11	1
      0x002EC00B,  //  029B  ADD	R11	K96	R11
      0x58300010,  //  029C  LDCONST	R12	K16
      0x7C240600,  //  029D  CALL	R9	3
      0x88240115,  //  029E  GETMBR	R9	R0	K21
      0x8C241361,  //  029F  GETMET	R9	R9	K97
      0x5C2C1000,  //  02A0  MOVE	R11	R8
      0x7C240400,  //  02A1  CALL	R9	2
      0x50240200,  //  02A2  LDBOOL	R9	1	0
      0xA0000000,  //  02A3  CLOSE	R0
      0x80041200,  //  02A4  RET	1	R9
      0x70020007,  //  02A5  JMP		#02AE
      0x1C200F0E,  //  02A6  EQ	R8	R7	K14
      0x78220005,  //  02A7  JMPF	R8	#02AE
      0x88200115,  //  02A8  GETMBR	R8	R0	K21
      0x8C201162,  //  02A9  GETMET	R8	R8	K98
      0x7C200200,  //  02AA  CALL	R8	1
      0x50200200,  //  02AB  LDBOOL	R8	1	0
      0xA0000000,  //  02AC  CLOSE	R0
      0x80041000,  //  02AD  RET	1	R8
      0x70020012,  //  02AE  JMP		#02C2
      0x54220029,  //  02AF  LDINT	R8	42
      0x1C200C08,  //  02B0  EQ	R8	R6	R8
      0x78220005,  //  02B1  JMPF	R8	#02B8
      0x1C200F05,  //  02B2  EQ	R8	R7	K5
      0x78220002,  //  02B3  JMPF	R8	#02B7
      0x50200200,  //  02B4  LDBOOL	R8	1	0
      0xA0000000,  //  02B5  CLOSE	R0
      0x80041000,  //  02B6  RET	1	R8
      0x70020009,  //  02B7  JMP		#02C2
      0x60200003,  //  02B8  GETGBL	R8	G3
      0x5C240000,  //  02B9  MOVE	R9	R0
      0x7C200200,  //  02BA  CALL	R8	1
      0x8C201163,  //  02BB  GETMET	R8	R8	K99
      0x5C280200,  //  02BC  MOVE	R10	R1
      0x5C2C0400,  //  02BD  MOVE	R11	R2
      0x5C300600,  //  02BE  MOVE	R12	R3
      0x7C200800,  //  02BF  CALL	R8	4
      0xA0000000,  //  02C0  CLOSE	R0
      0x80041000,  //  02C1  RET	1	R8
      0xA0000000,  //  02C2  CLOSE	R0
      0x80000000,  //  02C3  RET	0
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
    ( &(const bvalue[87]) {     /* constants */
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
    /* K86  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[888]) {  /* code */
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
      0x7002033D,  //  0038  JMP		#0377
      0x541E0031,  //  0039  LDINT	R7	50
      0x1C1C0A07,  //  003A  EQ	R7	R5	R7
      0x781E0000,  //  003B  JMPF	R7	#003D
      0x70020339,  //  003C  JMP		#0377
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
      0x70020259,  //  011C  JMP		#0377
      0x541E0033,  //  011D  LDINT	R7	52
      0x1C1C0A07,  //  011E  EQ	R7	R5	R7
      0x781E0000,  //  011F  JMPF	R7	#0121
      0x70020255,  //  0120  JMP		#0377
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
      0x70020225,  //  0150  JMP		#0377
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
      0x70020191,  //  01E4  JMP		#0377
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
      0x70020151,  //  0224  JMP		#0377
      0x541E0027,  //  0225  LDINT	R7	40
      0x1C1C0A07,  //  0226  EQ	R7	R5	R7
      0x781E00AE,  //  0227  JMPF	R7	#02D7
      0x1C1C0D05,  //  0228  EQ	R7	R6	K5
      0x781E0005,  //  0229  JMPF	R7	#0230
      0x8C1C0906,  //  022A  GETMET	R7	R4	K6
      0x8824090C,  //  022B  GETMBR	R9	R4	K12
      0x58280009,  //  022C  LDCONST	R10	K9
      0x7C1C0600,  //  022D  CALL	R7	3
      0x80040E00,  //  022E  RET	1	R7
      0x700200A5,  //  022F  JMP		#02D6
      0x1C1C0D09,  //  0230  EQ	R7	R6	K9
      0x781E0005,  //  0231  JMPF	R7	#0238
      0x8C1C0906,  //  0232  GETMET	R7	R4	K6
      0x88240916,  //  0233  GETMBR	R9	R4	K22
      0x58280049,  //  0234  LDCONST	R10	K73
      0x7C1C0600,  //  0235  CALL	R7	3
      0x80040E00,  //  0236  RET	1	R7
      0x7002009D,  //  0237  JMP		#02D6
      0x1C1C0D0D,  //  0238  EQ	R7	R6	K13
      0x781E0006,  //  0239  JMPF	R7	#0241
      0x8C1C0906,  //  023A  GETMET	R7	R4	K6
      0x8824090C,  //  023B  GETMBR	R9	R4	K12
      0x88280133,  //  023C  GETMBR	R10	R0	K51
      0x8828154A,  //  023D  GETMBR	R10	R10	K74
      0x7C1C0600,  //  023E  CALL	R7	3
      0x80040E00,  //  023F  RET	1	R7
      0x70020094,  //  0240  JMP		#02D6
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
      0x70020087,  //  024D  JMP		#02D6
      0x541E0003,  //  024E  LDINT	R7	4
      0x1C1C0C07,  //  024F  EQ	R7	R6	R7
      0x781E0005,  //  0250  JMPF	R7	#0257
      0x8C1C0906,  //  0251  GETMET	R7	R4	K6
      0x8824090C,  //  0252  GETMBR	R9	R4	K12
      0x542A7FFF,  //  0253  LDINT	R10	32768
      0x7C1C0600,  //  0254  CALL	R7	3
      0x80040E00,  //  0255  RET	1	R7
      0x7002007E,  //  0256  JMP		#02D6
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
      0x70020070,  //  0264  JMP		#02D6
      0x541E0005,  //  0265  LDINT	R7	6
      0x1C1C0C07,  //  0266  EQ	R7	R6	R7
      0x781E0005,  //  0267  JMPF	R7	#026E
      0x8C1C0906,  //  0268  GETMET	R7	R4	K6
      0x88240916,  //  0269  GETMBR	R9	R4	K22
      0x5828004E,  //  026A  LDCONST	R10	K78
      0x7C1C0600,  //  026B  CALL	R7	3
      0x80040E00,  //  026C  RET	1	R7
      0x70020067,  //  026D  JMP		#02D6
      0x541E0006,  //  026E  LDINT	R7	7
      0x1C1C0C07,  //  026F  EQ	R7	R6	R7
      0x781E0005,  //  0270  JMPF	R7	#0277
      0x8C1C0906,  //  0271  GETMET	R7	R4	K6
      0x8824090C,  //  0272  GETMBR	R9	R4	K12
      0x58280005,  //  0273  LDCONST	R10	K5
      0x7C1C0600,  //  0274  CALL	R7	3
      0x80040E00,  //  0275  RET	1	R7
      0x7002005E,  //  0276  JMP		#02D6
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
      0x7002004F,  //  0285  JMP		#02D6
      0x541E0008,  //  0286  LDINT	R7	9
      0x1C1C0C07,  //  0287  EQ	R7	R6	R7
      0x781E0005,  //  0288  JMPF	R7	#028F
      0x8C1C0906,  //  0289  GETMET	R7	R4	K6
      0x8824090C,  //  028A  GETMBR	R9	R4	K12
      0x58280009,  //  028B  LDCONST	R10	K9
      0x7C1C0600,  //  028C  CALL	R7	3
      0x80040E00,  //  028D  RET	1	R7
      0x70020046,  //  028E  JMP		#02D6
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
      0x7002002D,  //  02A7  JMP		#02D6
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
      0x7002001E,  //  02B6  JMP		#02D6
      0x541E0011,  //  02B7  LDINT	R7	18
      0x1C1C0C07,  //  02B8  EQ	R7	R6	R7
      0x781E000B,  //  02B9  JMPF	R7	#02C6
      0x8C1C0906,  //  02BA  GETMET	R7	R4	K6
      0x88240916,  //  02BB  GETMBR	R9	R4	K22
      0xB82A2400,  //  02BC  GETNGBL	R10	K18
      0x8C281525,  //  02BD  GETMET	R10	R10	K37
      0x7C280200,  //  02BE  CALL	R10	1
      0x8C28151B,  //  02BF  GETMET	R10	R10	K27
      0x5830001C,  //  02C0  LDCONST	R12	K28
      0x5834001D,  //  02C1  LDCONST	R13	K29
      0x7C280600,  //  02C2  CALL	R10	3
      0x7C1C0600,  //  02C3  CALL	R7	3
      0x80040E00,  //  02C4  RET	1	R7
      0x7002000F,  //  02C5  JMP		#02D6
      0x541E0012,  //  02C6  LDINT	R7	19
      0x1C1C0C07,  //  02C7  EQ	R7	R6	R7
      0x781E000C,  //  02C8  JMPF	R7	#02D6
      0x8C1C090A,  //  02C9  GETMET	R7	R4	K10
      0x7C1C0200,  //  02CA  CALL	R7	1
      0x8C200F0B,  //  02CB  GETMET	R8	R7	K11
      0x58280005,  //  02CC  LDCONST	R10	K5
      0x882C090C,  //  02CD  GETMBR	R11	R4	K12
      0x5830000F,  //  02CE  LDCONST	R12	K15
      0x7C200800,  //  02CF  CALL	R8	4
      0x8C200F0B,  //  02D0  GETMET	R8	R7	K11
      0x58280009,  //  02D1  LDCONST	R10	K9
      0x882C090C,  //  02D2  GETMBR	R11	R4	K12
      0x5830000F,  //  02D3  LDCONST	R12	K15
      0x7C200800,  //  02D4  CALL	R8	4
      0x80040E00,  //  02D5  RET	1	R7
      0x7002009F,  //  02D6  JMP		#0377
      0x541E003E,  //  02D7  LDINT	R7	63
      0x1C1C0A07,  //  02D8  EQ	R7	R5	R7
      0x781E0000,  //  02D9  JMPF	R7	#02DB
      0x7002009B,  //  02DA  JMP		#0377
      0x541E0029,  //  02DB  LDINT	R7	42
      0x1C1C0A07,  //  02DC  EQ	R7	R5	R7
      0x781E001D,  //  02DD  JMPF	R7	#02FC
      0x1C1C0D05,  //  02DE  EQ	R7	R6	K5
      0x781E0003,  //  02DF  JMPF	R7	#02E4
      0x8C1C0911,  //  02E0  GETMET	R7	R4	K17
      0x7C1C0200,  //  02E1  CALL	R7	1
      0x80040E00,  //  02E2  RET	1	R7
      0x70020016,  //  02E3  JMP		#02FB
      0x1C1C0D09,  //  02E4  EQ	R7	R6	K9
      0x781E0005,  //  02E5  JMPF	R7	#02EC
      0x8C1C0906,  //  02E6  GETMET	R7	R4	K6
      0x88240910,  //  02E7  GETMBR	R9	R4	K16
      0x58280005,  //  02E8  LDCONST	R10	K5
      0x7C1C0600,  //  02E9  CALL	R7	3
      0x80040E00,  //  02EA  RET	1	R7
      0x7002000E,  //  02EB  JMP		#02FB
      0x1C1C0D0D,  //  02EC  EQ	R7	R6	K13
      0x781E0005,  //  02ED  JMPF	R7	#02F4
      0x8C1C0906,  //  02EE  GETMET	R7	R4	K6
      0x8824090E,  //  02EF  GETMBR	R9	R4	K14
      0x58280009,  //  02F0  LDCONST	R10	K9
      0x7C1C0600,  //  02F1  CALL	R7	3
      0x80040E00,  //  02F2  RET	1	R7
      0x70020006,  //  02F3  JMP		#02FB
      0x1C1C0D0F,  //  02F4  EQ	R7	R6	K15
      0x781E0004,  //  02F5  JMPF	R7	#02FB
      0x8C1C0906,  //  02F6  GETMET	R7	R4	K6
      0x88240918,  //  02F7  GETMBR	R9	R4	K24
      0x4C280000,  //  02F8  LDNIL	R10
      0x7C1C0600,  //  02F9  CALL	R7	3
      0x80040E00,  //  02FA  RET	1	R7
      0x7002007A,  //  02FB  JMP		#0377
      0x541E002A,  //  02FC  LDINT	R7	43
      0x1C1C0A07,  //  02FD  EQ	R7	R5	R7
      0x781E0016,  //  02FE  JMPF	R7	#0316
      0x1C1C0D05,  //  02FF  EQ	R7	R6	K5
      0x781E0007,  //  0300  JMPF	R7	#0309
      0x8C1C0906,  //  0301  GETMET	R7	R4	K6
      0x88240916,  //  0302  GETMBR	R9	R4	K22
      0xB82A2400,  //  0303  GETNGBL	R10	K18
      0x8C281554,  //  0304  GETMET	R10	R10	K84
      0x7C280200,  //  0305  CALL	R10	1
      0x7C1C0600,  //  0306  CALL	R7	3
      0x80040E00,  //  0307  RET	1	R7
      0x7002000B,  //  0308  JMP		#0315
      0x1C1C0D09,  //  0309  EQ	R7	R6	K9
      0x781E0009,  //  030A  JMPF	R7	#0315
      0x8C1C0911,  //  030B  GETMET	R7	R4	K17
      0x7C1C0200,  //  030C  CALL	R7	1
      0x8C200F0B,  //  030D  GETMET	R8	R7	K11
      0x4C280000,  //  030E  LDNIL	R10
      0x882C0916,  //  030F  GETMBR	R11	R4	K22
      0xB8322400,  //  0310  GETNGBL	R12	K18
      0x8C301954,  //  0311  GETMET	R12	R12	K84
      0x7C300200,  //  0312  CALL	R12	1
      0x7C200800,  //  0313  CALL	R8	4
      0x80040E00,  //  0314  RET	1	R7
      0x70020060,  //  0315  JMP		#0377
      0x541E002B,  //  0316  LDINT	R7	44
      0x1C1C0A07,  //  0317  EQ	R7	R5	R7
      0x781E001C,  //  0318  JMPF	R7	#0336
      0x1C1C0D05,  //  0319  EQ	R7	R6	K5
      0x781E0005,  //  031A  JMPF	R7	#0321
      0x8C1C0906,  //  031B  GETMET	R7	R4	K6
      0x8824090E,  //  031C  GETMBR	R9	R4	K14
      0x58280009,  //  031D  LDCONST	R10	K9
      0x7C1C0600,  //  031E  CALL	R7	3
      0x80040E00,  //  031F  RET	1	R7
      0x70020013,  //  0320  JMP		#0335
      0x1C1C0D09,  //  0321  EQ	R7	R6	K9
      0x781E0005,  //  0322  JMPF	R7	#0329
      0x8C1C0906,  //  0323  GETMET	R7	R4	K6
      0x8824090E,  //  0324  GETMBR	R9	R4	K14
      0x542A0003,  //  0325  LDINT	R10	4
      0x7C1C0600,  //  0326  CALL	R7	3
      0x80040E00,  //  0327  RET	1	R7
      0x7002000B,  //  0328  JMP		#0335
      0x1C1C0D0D,  //  0329  EQ	R7	R6	K13
      0x781E0009,  //  032A  JMPF	R7	#0335
      0x8C1C0911,  //  032B  GETMET	R7	R4	K17
      0x7C1C0200,  //  032C  CALL	R7	1
      0x8C200F0B,  //  032D  GETMET	R8	R7	K11
      0x4C280000,  //  032E  LDNIL	R10
      0x8C2C0906,  //  032F  GETMET	R11	R4	K6
      0x8834090E,  //  0330  GETMBR	R13	R4	K14
      0x543A0003,  //  0331  LDINT	R14	4
      0x7C2C0600,  //  0332  CALL	R11	3
      0x7C200600,  //  0333  CALL	R8	3
      0x80040E00,  //  0334  RET	1	R7
      0x70020040,  //  0335  JMP		#0377
      0x541E0030,  //  0336  LDINT	R7	49
      0x1C1C0A07,  //  0337  EQ	R7	R5	R7
      0x781E0010,  //  0338  JMPF	R7	#034A
      0x1C1C0D0F,  //  0339  EQ	R7	R6	K15
      0x781E0005,  //  033A  JMPF	R7	#0341
      0x8C1C0906,  //  033B  GETMET	R7	R4	K6
      0x8824090E,  //  033C  GETMBR	R9	R4	K14
      0x542A001D,  //  033D  LDINT	R10	30
      0x7C1C0600,  //  033E  CALL	R7	3
      0x80040E00,  //  033F  RET	1	R7
      0x70020007,  //  0340  JMP		#0349
      0x541EFFFB,  //  0341  LDINT	R7	65532
      0x1C1C0C07,  //  0342  EQ	R7	R6	R7
      0x781E0004,  //  0343  JMPF	R7	#0349
      0x8C1C0906,  //  0344  GETMET	R7	R4	K6
      0x8824092A,  //  0345  GETMBR	R9	R4	K42
      0x542A0003,  //  0346  LDINT	R10	4
      0x7C1C0600,  //  0347  CALL	R7	3
      0x80040E00,  //  0348  RET	1	R7
      0x7002002C,  //  0349  JMP		#0377
      0x541E001C,  //  034A  LDINT	R7	29
      0x1C1C0A07,  //  034B  EQ	R7	R5	R7
      0x781E0021,  //  034C  JMPF	R7	#036F
      0x1C1C0D0F,  //  034D  EQ	R7	R6	K15
      0x781E0016,  //  034E  JMPF	R7	#0366
      0x8C1C0911,  //  034F  GETMET	R7	R4	K17
      0x7C1C0200,  //  0350  CALL	R7	1
      0x88200133,  //  0351  GETMBR	R8	R0	K51
      0x8C201155,  //  0352  GETMET	R8	R8	K85
      0x50280200,  //  0353  LDBOOL	R10	1	0
      0x7C200400,  //  0354  CALL	R8	2
      0x60240010,  //  0355  GETGBL	R9	G16
      0x5C281000,  //  0356  MOVE	R10	R8
      0x7C240200,  //  0357  CALL	R9	1
      0xA8020007,  //  0358  EXBLK	0	#0361
      0x5C281200,  //  0359  MOVE	R10	R9
      0x7C280000,  //  035A  CALL	R10	0
      0x8C2C0F0B,  //  035B  GETMET	R11	R7	K11
      0x4C340000,  //  035C  LDNIL	R13
      0x8838090C,  //  035D  GETMBR	R14	R4	K12
      0x5C3C1400,  //  035E  MOVE	R15	R10
      0x7C2C0800,  //  035F  CALL	R11	4
      0x7001FFF7,  //  0360  JMP		#0359
      0x5824003A,  //  0361  LDCONST	R9	K58
      0xAC240200,  //  0362  CATCH	R9	1	0
      0xB0080000,  //  0363  RAISE	2	R0	R0
      0x80040E00,  //  0364  RET	1	R7
      0x70020007,  //  0365  JMP		#036E
      0x601C0003,  //  0366  GETGBL	R7	G3
      0x5C200000,  //  0367  MOVE	R8	R0
      0x7C1C0200,  //  0368  CALL	R7	1
      0x8C1C0F56,  //  0369  GETMET	R7	R7	K86
      0x5C240200,  //  036A  MOVE	R9	R1
      0x5C280400,  //  036B  MOVE	R10	R2
      0x7C1C0600,  //  036C  CALL	R7	3
      0x80040E00,  //  036D  RET	1	R7
      0x70020007,  //  036E  JMP		#0377
      0x601C0003,  //  036F  GETGBL	R7	G3
      0x5C200000,  //  0370  MOVE	R8	R0
      0x7C1C0200,  //  0371  CALL	R7	1
      0x8C1C0F56,  //  0372  GETMET	R7	R7	K86
      0x5C240200,  //  0373  MOVE	R9	R1
      0x5C280400,  //  0374  MOVE	R10	R2
      0x7C1C0600,  //  0375  CALL	R7	3
      0x80040E00,  //  0376  RET	1	R7
      0x80000000,  //  0377  RET	0
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
        be_const_list( *     be_nested_list(14,
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
