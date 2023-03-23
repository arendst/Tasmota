/* Solidification of Matter_Plugin_Root.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Root;

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
    ( &(const bvalue[90]) {     /* constants */
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
    /* K83  */  be_nested_str_weak(locale),
    /* K84  */  be_nested_str_weak(TYPES),
    /* K85  */  be_nested_str_weak(keys),
    /* K86  */  be_nested_str_weak(get_cluster_list),
    /* K87  */  be_nested_str_weak(get_active_endpoints),
    /* K88  */  be_nested_str_weak(status),
    /* K89  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[918]) {  /* code */
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
      0x7002035B,  //  0038  JMP		#0395
      0x541E0031,  //  0039  LDINT	R7	50
      0x1C1C0A07,  //  003A  EQ	R7	R5	R7
      0x781E0000,  //  003B  JMPF	R7	#003D
      0x70020357,  //  003C  JMP		#0395
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
      0x70020277,  //  011C  JMP		#0395
      0x541E0033,  //  011D  LDINT	R7	52
      0x1C1C0A07,  //  011E  EQ	R7	R5	R7
      0x781E0000,  //  011F  JMPF	R7	#0121
      0x70020273,  //  0120  JMP		#0395
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
      0x70020243,  //  0150  JMP		#0395
      0x541E003D,  //  0151  LDINT	R7	62
      0x1C1C0A07,  //  0152  EQ	R7	R5	R7
      0x781E008B,  //  0153  JMPF	R7	#01E0
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
      0x70020062,  //  017B  JMP		#01DF
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
      0x70020023,  //  01BA  JMP		#01DF
      0x1C1C0D0D,  //  01BB  EQ	R7	R6	K13
      0x781E0007,  //  01BC  JMPF	R7	#01C5
      0x8C1C0906,  //  01BD  GETMET	R7	R4	K6
      0x8824090E,  //  01BE  GETMBR	R9	R4	K14
      0xB82A0200,  //  01BF  GETNGBL	R10	K1
      0x88281542,  //  01C0  GETMBR	R10	R10	K66
      0x88281543,  //  01C1  GETMBR	R10	R10	K67
      0x7C1C0600,  //  01C2  CALL	R7	3
      0x80040E00,  //  01C3  RET	1	R7
      0x70020019,  //  01C4  JMP		#01DF
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
      0x7002000D,  //  01D0  JMP		#01DF
      0x541E0003,  //  01D1  LDINT	R7	4
      0x1C1C0C07,  //  01D2  EQ	R7	R6	R7
      0x781E0000,  //  01D3  JMPF	R7	#01D5
      0x70020009,  //  01D4  JMP		#01DF
      0x541E0004,  //  01D5  LDINT	R7	5
      0x1C1C0C07,  //  01D6  EQ	R7	R6	R7
      0x781E0006,  //  01D7  JMPF	R7	#01DF
      0x8C1C0906,  //  01D8  GETMET	R7	R4	K6
      0x8824090E,  //  01D9  GETMBR	R9	R4	K14
      0x88280345,  //  01DA  GETMBR	R10	R1	K69
      0x8C281539,  //  01DB  GETMET	R10	R10	K57
      0x7C280200,  //  01DC  CALL	R10	1
      0x7C1C0600,  //  01DD  CALL	R7	3
      0x80040E00,  //  01DE  RET	1	R7
      0x700201B4,  //  01DF  JMP		#0395
      0x541E003B,  //  01E0  LDINT	R7	60
      0x1C1C0A07,  //  01E1  EQ	R7	R5	R7
      0x781E003C,  //  01E2  JMPF	R7	#0220
      0x1C1C0D05,  //  01E3  EQ	R7	R6	K5
      0x781E0012,  //  01E4  JMPF	R7	#01F8
      0x881C0133,  //  01E5  GETMBR	R7	R0	K51
      0x8C1C0F46,  //  01E6  GETMET	R7	R7	K70
      0x7C1C0200,  //  01E7  CALL	R7	1
      0x88200133,  //  01E8  GETMBR	R8	R0	K51
      0x8C201147,  //  01E9  GETMET	R8	R8	K71
      0x7C200200,  //  01EA  CALL	R8	1
      0x781E0004,  //  01EB  JMPF	R7	#01F1
      0x78220001,  //  01EC  JMPF	R8	#01EF
      0x5824000D,  //  01ED  LDCONST	R9	K13
      0x70020000,  //  01EE  JMP		#01F0
      0x58240009,  //  01EF  LDCONST	R9	K9
      0x70020000,  //  01F0  JMP		#01F2
      0x58240005,  //  01F1  LDCONST	R9	K5
      0x8C280906,  //  01F2  GETMET	R10	R4	K6
      0x8830090E,  //  01F3  GETMBR	R12	R4	K14
      0x5C341200,  //  01F4  MOVE	R13	R9
      0x7C280600,  //  01F5  CALL	R10	3
      0x80041400,  //  01F6  RET	1	R10
      0x70020026,  //  01F7  JMP		#021F
      0x1C1C0D09,  //  01F8  EQ	R7	R6	K9
      0x781E0011,  //  01F9  JMPF	R7	#020C
      0x881C0133,  //  01FA  GETMBR	R7	R0	K51
      0x881C0F48,  //  01FB  GETMBR	R7	R7	K72
      0x4C200000,  //  01FC  LDNIL	R8
      0x20200E08,  //  01FD  NE	R8	R7	R8
      0x78220006,  //  01FE  JMPF	R8	#0206
      0x8C200906,  //  01FF  GETMET	R8	R4	K6
      0x8828090C,  //  0200  GETMBR	R10	R4	K12
      0x8C2C0F39,  //  0201  GETMET	R11	R7	K57
      0x7C2C0200,  //  0202  CALL	R11	1
      0x7C200600,  //  0203  CALL	R8	3
      0x80041000,  //  0204  RET	1	R8
      0x70020004,  //  0205  JMP		#020B
      0x8C200906,  //  0206  GETMET	R8	R4	K6
      0x88280918,  //  0207  GETMBR	R10	R4	K24
      0x4C2C0000,  //  0208  LDNIL	R11
      0x7C200600,  //  0209  CALL	R8	3
      0x80041000,  //  020A  RET	1	R8
      0x70020012,  //  020B  JMP		#021F
      0x1C1C0D0D,  //  020C  EQ	R7	R6	K13
      0x781E0010,  //  020D  JMPF	R7	#021F
      0x881C0133,  //  020E  GETMBR	R7	R0	K51
      0x881C0F48,  //  020F  GETMBR	R7	R7	K72
      0x4C200000,  //  0210  LDNIL	R8
      0x20200E08,  //  0211  NE	R8	R7	R8
      0x78220006,  //  0212  JMPF	R8	#021A
      0x8C200906,  //  0213  GETMET	R8	R4	K6
      0x8828090C,  //  0214  GETMBR	R10	R4	K12
      0x8C2C0F3E,  //  0215  GETMET	R11	R7	K62
      0x7C2C0200,  //  0216  CALL	R11	1
      0x7C200600,  //  0217  CALL	R8	3
      0x80041000,  //  0218  RET	1	R8
      0x70020004,  //  0219  JMP		#021F
      0x8C200906,  //  021A  GETMET	R8	R4	K6
      0x88280918,  //  021B  GETMBR	R10	R4	K24
      0x4C2C0000,  //  021C  LDNIL	R11
      0x7C200600,  //  021D  CALL	R8	3
      0x80041000,  //  021E  RET	1	R8
      0x70020174,  //  021F  JMP		#0395
      0x541E0027,  //  0220  LDINT	R7	40
      0x1C1C0A07,  //  0221  EQ	R7	R5	R7
      0x781E00A4,  //  0222  JMPF	R7	#02C8
      0x1C1C0D05,  //  0223  EQ	R7	R6	K5
      0x781E0005,  //  0224  JMPF	R7	#022B
      0x8C1C0906,  //  0225  GETMET	R7	R4	K6
      0x8824090C,  //  0226  GETMBR	R9	R4	K12
      0x58280005,  //  0227  LDCONST	R10	K5
      0x7C1C0600,  //  0228  CALL	R7	3
      0x80040E00,  //  0229  RET	1	R7
      0x7002009B,  //  022A  JMP		#02C7
      0x1C1C0D09,  //  022B  EQ	R7	R6	K9
      0x781E0005,  //  022C  JMPF	R7	#0233
      0x8C1C0906,  //  022D  GETMET	R7	R4	K6
      0x88240916,  //  022E  GETMBR	R9	R4	K22
      0x58280049,  //  022F  LDCONST	R10	K73
      0x7C1C0600,  //  0230  CALL	R7	3
      0x80040E00,  //  0231  RET	1	R7
      0x70020093,  //  0232  JMP		#02C7
      0x1C1C0D0D,  //  0233  EQ	R7	R6	K13
      0x781E0006,  //  0234  JMPF	R7	#023C
      0x8C1C0906,  //  0235  GETMET	R7	R4	K6
      0x8824090C,  //  0236  GETMBR	R9	R4	K12
      0x88280133,  //  0237  GETMBR	R10	R0	K51
      0x8828154A,  //  0238  GETMBR	R10	R10	K74
      0x7C1C0600,  //  0239  CALL	R7	3
      0x80040E00,  //  023A  RET	1	R7
      0x7002008A,  //  023B  JMP		#02C7
      0x1C1C0D0F,  //  023C  EQ	R7	R6	K15
      0x781E000A,  //  023D  JMPF	R7	#0249
      0x8C1C0906,  //  023E  GETMET	R7	R4	K6
      0x88240916,  //  023F  GETMBR	R9	R4	K22
      0xB82A2400,  //  0240  GETNGBL	R10	K18
      0x8C281526,  //  0241  GETMET	R10	R10	K38
      0x5830004B,  //  0242  LDCONST	R12	K75
      0x50340200,  //  0243  LDBOOL	R13	1	0
      0x7C280600,  //  0244  CALL	R10	3
      0x9428154B,  //  0245  GETIDX	R10	R10	K75
      0x7C1C0600,  //  0246  CALL	R7	3
      0x80040E00,  //  0247  RET	1	R7
      0x7002007D,  //  0248  JMP		#02C7
      0x541E0003,  //  0249  LDINT	R7	4
      0x1C1C0C07,  //  024A  EQ	R7	R6	R7
      0x781E0005,  //  024B  JMPF	R7	#0252
      0x8C1C0906,  //  024C  GETMET	R7	R4	K6
      0x8824090C,  //  024D  GETMBR	R9	R4	K12
      0x542A7FFF,  //  024E  LDINT	R10	32768
      0x7C1C0600,  //  024F  CALL	R7	3
      0x80040E00,  //  0250  RET	1	R7
      0x70020074,  //  0251  JMP		#02C7
      0x541E0004,  //  0252  LDINT	R7	5
      0x1C1C0C07,  //  0253  EQ	R7	R6	R7
      0x781E000A,  //  0254  JMPF	R7	#0260
      0x8C1C0906,  //  0255  GETMET	R7	R4	K6
      0x88240916,  //  0256  GETMBR	R9	R4	K22
      0xB82A2400,  //  0257  GETNGBL	R10	K18
      0x8C281526,  //  0258  GETMET	R10	R10	K38
      0x5830004C,  //  0259  LDCONST	R12	K76
      0x50340200,  //  025A  LDBOOL	R13	1	0
      0x7C280600,  //  025B  CALL	R10	3
      0x9428154D,  //  025C  GETIDX	R10	R10	K77
      0x7C1C0600,  //  025D  CALL	R7	3
      0x80040E00,  //  025E  RET	1	R7
      0x70020066,  //  025F  JMP		#02C7
      0x541E0005,  //  0260  LDINT	R7	6
      0x1C1C0C07,  //  0261  EQ	R7	R6	R7
      0x781E0005,  //  0262  JMPF	R7	#0269
      0x8C1C0906,  //  0263  GETMET	R7	R4	K6
      0x88240916,  //  0264  GETMBR	R9	R4	K22
      0x5828004E,  //  0265  LDCONST	R10	K78
      0x7C1C0600,  //  0266  CALL	R7	3
      0x80040E00,  //  0267  RET	1	R7
      0x7002005D,  //  0268  JMP		#02C7
      0x541E0006,  //  0269  LDINT	R7	7
      0x1C1C0C07,  //  026A  EQ	R7	R6	R7
      0x781E0005,  //  026B  JMPF	R7	#0272
      0x8C1C0906,  //  026C  GETMET	R7	R4	K6
      0x8824090C,  //  026D  GETMBR	R9	R4	K12
      0x58280005,  //  026E  LDCONST	R10	K5
      0x7C1C0600,  //  026F  CALL	R7	3
      0x80040E00,  //  0270  RET	1	R7
      0x70020054,  //  0271  JMP		#02C7
      0x541E0007,  //  0272  LDINT	R7	8
      0x1C1C0C07,  //  0273  EQ	R7	R6	R7
      0x781E000B,  //  0274  JMPF	R7	#0281
      0x8C1C0906,  //  0275  GETMET	R7	R4	K6
      0x88240916,  //  0276  GETMBR	R9	R4	K22
      0xB82A2400,  //  0277  GETNGBL	R10	K18
      0x8C281526,  //  0278  GETMET	R10	R10	K38
      0x5830004F,  //  0279  LDCONST	R12	K79
      0x50340200,  //  027A  LDBOOL	R13	1	0
      0x7C280600,  //  027B  CALL	R10	3
      0x94281550,  //  027C  GETIDX	R10	R10	K80
      0x94281551,  //  027D  GETIDX	R10	R10	K81
      0x7C1C0600,  //  027E  CALL	R7	3
      0x80040E00,  //  027F  RET	1	R7
      0x70020045,  //  0280  JMP		#02C7
      0x541E0008,  //  0281  LDINT	R7	9
      0x1C1C0C07,  //  0282  EQ	R7	R6	R7
      0x781E0005,  //  0283  JMPF	R7	#028A
      0x8C1C0906,  //  0284  GETMET	R7	R4	K6
      0x8824090C,  //  0285  GETMBR	R9	R4	K12
      0x58280009,  //  0286  LDCONST	R10	K9
      0x7C1C0600,  //  0287  CALL	R7	3
      0x80040E00,  //  0288  RET	1	R7
      0x7002003C,  //  0289  JMP		#02C7
      0x541E0009,  //  028A  LDINT	R7	10
      0x1C1C0C07,  //  028B  EQ	R7	R6	R7
      0x781E000B,  //  028C  JMPF	R7	#0299
      0x8C1C0906,  //  028D  GETMET	R7	R4	K6
      0x88240916,  //  028E  GETMBR	R9	R4	K22
      0xB82A2400,  //  028F  GETNGBL	R10	K18
      0x8C281526,  //  0290  GETMET	R10	R10	K38
      0x5830004F,  //  0291  LDCONST	R12	K79
      0x50340200,  //  0292  LDBOOL	R13	1	0
      0x7C280600,  //  0293  CALL	R10	3
      0x94281550,  //  0294  GETIDX	R10	R10	K80
      0x94281552,  //  0295  GETIDX	R10	R10	K82
      0x7C1C0600,  //  0296  CALL	R7	3
      0x80040E00,  //  0297  RET	1	R7
      0x7002002D,  //  0298  JMP		#02C7
      0x541E000E,  //  0299  LDINT	R7	15
      0x1C1C0C07,  //  029A  EQ	R7	R6	R7
      0x781E000B,  //  029B  JMPF	R7	#02A8
      0x8C1C0906,  //  029C  GETMET	R7	R4	K6
      0x88240916,  //  029D  GETMBR	R9	R4	K22
      0xB82A2400,  //  029E  GETNGBL	R10	K18
      0x8C281525,  //  029F  GETMET	R10	R10	K37
      0x7C280200,  //  02A0  CALL	R10	1
      0x8C28151B,  //  02A1  GETMET	R10	R10	K27
      0x5830001C,  //  02A2  LDCONST	R12	K28
      0x5834001D,  //  02A3  LDCONST	R13	K29
      0x7C280600,  //  02A4  CALL	R10	3
      0x7C1C0600,  //  02A5  CALL	R7	3
      0x80040E00,  //  02A6  RET	1	R7
      0x7002001E,  //  02A7  JMP		#02C7
      0x541E0011,  //  02A8  LDINT	R7	18
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
      0x7002000F,  //  02B6  JMP		#02C7
      0x541E0012,  //  02B7  LDINT	R7	19
      0x1C1C0C07,  //  02B8  EQ	R7	R6	R7
      0x781E000C,  //  02B9  JMPF	R7	#02C7
      0x8C1C090A,  //  02BA  GETMET	R7	R4	K10
      0x7C1C0200,  //  02BB  CALL	R7	1
      0x8C200F0B,  //  02BC  GETMET	R8	R7	K11
      0x58280005,  //  02BD  LDCONST	R10	K5
      0x882C090C,  //  02BE  GETMBR	R11	R4	K12
      0x5830000F,  //  02BF  LDCONST	R12	K15
      0x7C200800,  //  02C0  CALL	R8	4
      0x8C200F0B,  //  02C1  GETMET	R8	R7	K11
      0x58280009,  //  02C2  LDCONST	R10	K9
      0x882C090C,  //  02C3  GETMBR	R11	R4	K12
      0x5830000F,  //  02C4  LDCONST	R12	K15
      0x7C200800,  //  02C5  CALL	R8	4
      0x80040E00,  //  02C6  RET	1	R7
      0x700200CC,  //  02C7  JMP		#0395
      0x541E003E,  //  02C8  LDINT	R7	63
      0x1C1C0A07,  //  02C9  EQ	R7	R5	R7
      0x781E0000,  //  02CA  JMPF	R7	#02CC
      0x700200C8,  //  02CB  JMP		#0395
      0x541E0029,  //  02CC  LDINT	R7	42
      0x1C1C0A07,  //  02CD  EQ	R7	R5	R7
      0x781E001D,  //  02CE  JMPF	R7	#02ED
      0x1C1C0D05,  //  02CF  EQ	R7	R6	K5
      0x781E0003,  //  02D0  JMPF	R7	#02D5
      0x8C1C0911,  //  02D1  GETMET	R7	R4	K17
      0x7C1C0200,  //  02D2  CALL	R7	1
      0x80040E00,  //  02D3  RET	1	R7
      0x70020016,  //  02D4  JMP		#02EC
      0x1C1C0D09,  //  02D5  EQ	R7	R6	K9
      0x781E0005,  //  02D6  JMPF	R7	#02DD
      0x8C1C0906,  //  02D7  GETMET	R7	R4	K6
      0x88240910,  //  02D8  GETMBR	R9	R4	K16
      0x58280005,  //  02D9  LDCONST	R10	K5
      0x7C1C0600,  //  02DA  CALL	R7	3
      0x80040E00,  //  02DB  RET	1	R7
      0x7002000E,  //  02DC  JMP		#02EC
      0x1C1C0D0D,  //  02DD  EQ	R7	R6	K13
      0x781E0005,  //  02DE  JMPF	R7	#02E5
      0x8C1C0906,  //  02DF  GETMET	R7	R4	K6
      0x8824090E,  //  02E0  GETMBR	R9	R4	K14
      0x58280009,  //  02E1  LDCONST	R10	K9
      0x7C1C0600,  //  02E2  CALL	R7	3
      0x80040E00,  //  02E3  RET	1	R7
      0x70020006,  //  02E4  JMP		#02EC
      0x1C1C0D0F,  //  02E5  EQ	R7	R6	K15
      0x781E0004,  //  02E6  JMPF	R7	#02EC
      0x8C1C0906,  //  02E7  GETMET	R7	R4	K6
      0x88240918,  //  02E8  GETMBR	R9	R4	K24
      0x4C280000,  //  02E9  LDNIL	R10
      0x7C1C0600,  //  02EA  CALL	R7	3
      0x80040E00,  //  02EB  RET	1	R7
      0x700200A7,  //  02EC  JMP		#0395
      0x541E002A,  //  02ED  LDINT	R7	43
      0x1C1C0A07,  //  02EE  EQ	R7	R5	R7
      0x781E0016,  //  02EF  JMPF	R7	#0307
      0x1C1C0D05,  //  02F0  EQ	R7	R6	K5
      0x781E0007,  //  02F1  JMPF	R7	#02FA
      0x8C1C0906,  //  02F2  GETMET	R7	R4	K6
      0x88240916,  //  02F3  GETMBR	R9	R4	K22
      0xB82A2400,  //  02F4  GETNGBL	R10	K18
      0x8C281553,  //  02F5  GETMET	R10	R10	K83
      0x7C280200,  //  02F6  CALL	R10	1
      0x7C1C0600,  //  02F7  CALL	R7	3
      0x80040E00,  //  02F8  RET	1	R7
      0x7002000B,  //  02F9  JMP		#0306
      0x1C1C0D09,  //  02FA  EQ	R7	R6	K9
      0x781E0009,  //  02FB  JMPF	R7	#0306
      0x8C1C0911,  //  02FC  GETMET	R7	R4	K17
      0x7C1C0200,  //  02FD  CALL	R7	1
      0x8C200F0B,  //  02FE  GETMET	R8	R7	K11
      0x4C280000,  //  02FF  LDNIL	R10
      0x882C0916,  //  0300  GETMBR	R11	R4	K22
      0xB8322400,  //  0301  GETNGBL	R12	K18
      0x8C301953,  //  0302  GETMET	R12	R12	K83
      0x7C300200,  //  0303  CALL	R12	1
      0x7C200800,  //  0304  CALL	R8	4
      0x80040E00,  //  0305  RET	1	R7
      0x7002008D,  //  0306  JMP		#0395
      0x541E002B,  //  0307  LDINT	R7	44
      0x1C1C0A07,  //  0308  EQ	R7	R5	R7
      0x781E001C,  //  0309  JMPF	R7	#0327
      0x1C1C0D05,  //  030A  EQ	R7	R6	K5
      0x781E0005,  //  030B  JMPF	R7	#0312
      0x8C1C0906,  //  030C  GETMET	R7	R4	K6
      0x8824090E,  //  030D  GETMBR	R9	R4	K14
      0x58280009,  //  030E  LDCONST	R10	K9
      0x7C1C0600,  //  030F  CALL	R7	3
      0x80040E00,  //  0310  RET	1	R7
      0x70020013,  //  0311  JMP		#0326
      0x1C1C0D09,  //  0312  EQ	R7	R6	K9
      0x781E0005,  //  0313  JMPF	R7	#031A
      0x8C1C0906,  //  0314  GETMET	R7	R4	K6
      0x8824090E,  //  0315  GETMBR	R9	R4	K14
      0x542A0003,  //  0316  LDINT	R10	4
      0x7C1C0600,  //  0317  CALL	R7	3
      0x80040E00,  //  0318  RET	1	R7
      0x7002000B,  //  0319  JMP		#0326
      0x1C1C0D0D,  //  031A  EQ	R7	R6	K13
      0x781E0009,  //  031B  JMPF	R7	#0326
      0x8C1C0911,  //  031C  GETMET	R7	R4	K17
      0x7C1C0200,  //  031D  CALL	R7	1
      0x8C200F0B,  //  031E  GETMET	R8	R7	K11
      0x4C280000,  //  031F  LDNIL	R10
      0x8C2C0906,  //  0320  GETMET	R11	R4	K6
      0x8834090E,  //  0321  GETMBR	R13	R4	K14
      0x543A0003,  //  0322  LDINT	R14	4
      0x7C2C0600,  //  0323  CALL	R11	3
      0x7C200600,  //  0324  CALL	R8	3
      0x80040E00,  //  0325  RET	1	R7
      0x7002006D,  //  0326  JMP		#0395
      0x541E0030,  //  0327  LDINT	R7	49
      0x1C1C0A07,  //  0328  EQ	R7	R5	R7
      0x781E0010,  //  0329  JMPF	R7	#033B
      0x1C1C0D0F,  //  032A  EQ	R7	R6	K15
      0x781E0005,  //  032B  JMPF	R7	#0332
      0x8C1C0906,  //  032C  GETMET	R7	R4	K6
      0x8824090E,  //  032D  GETMBR	R9	R4	K14
      0x542A001D,  //  032E  LDINT	R10	30
      0x7C1C0600,  //  032F  CALL	R7	3
      0x80040E00,  //  0330  RET	1	R7
      0x70020007,  //  0331  JMP		#033A
      0x541EFFFB,  //  0332  LDINT	R7	65532
      0x1C1C0C07,  //  0333  EQ	R7	R6	R7
      0x781E0004,  //  0334  JMPF	R7	#033A
      0x8C1C0906,  //  0335  GETMET	R7	R4	K6
      0x8824092A,  //  0336  GETMBR	R9	R4	K42
      0x542A0003,  //  0337  LDINT	R10	4
      0x7C1C0600,  //  0338  CALL	R7	3
      0x80040E00,  //  0339  RET	1	R7
      0x70020059,  //  033A  JMP		#0395
      0x541E001C,  //  033B  LDINT	R7	29
      0x1C1C0A07,  //  033C  EQ	R7	R5	R7
      0x781E0053,  //  033D  JMPF	R7	#0392
      0x1C1C0D05,  //  033E  EQ	R7	R6	K5
      0x781E001C,  //  033F  JMPF	R7	#035D
      0x8C1C0911,  //  0340  GETMET	R7	R4	K17
      0x7C1C0200,  //  0341  CALL	R7	1
      0x60200010,  //  0342  GETGBL	R8	G16
      0x88240154,  //  0343  GETMBR	R9	R0	K84
      0x8C241355,  //  0344  GETMET	R9	R9	K85
      0x7C240200,  //  0345  CALL	R9	1
      0x7C200200,  //  0346  CALL	R8	1
      0xA802000F,  //  0347  EXBLK	0	#0358
      0x5C241000,  //  0348  MOVE	R9	R8
      0x7C240000,  //  0349  CALL	R9	0
      0x8C280F15,  //  034A  GETMET	R10	R7	K21
      0x7C280200,  //  034B  CALL	R10	1
      0x8C2C150B,  //  034C  GETMET	R11	R10	K11
      0x58340005,  //  034D  LDCONST	R13	K5
      0x8838090C,  //  034E  GETMBR	R14	R4	K12
      0x5C3C1200,  //  034F  MOVE	R15	R9
      0x7C2C0800,  //  0350  CALL	R11	4
      0x8C2C150B,  //  0351  GETMET	R11	R10	K11
      0x58340009,  //  0352  LDCONST	R13	K9
      0x8838090C,  //  0353  GETMBR	R14	R4	K12
      0x883C0154,  //  0354  GETMBR	R15	R0	K84
      0x943C1E09,  //  0355  GETIDX	R15	R15	R9
      0x7C2C0800,  //  0356  CALL	R11	4
      0x7001FFEF,  //  0357  JMP		#0348
      0x5820003A,  //  0358  LDCONST	R8	K58
      0xAC200200,  //  0359  CATCH	R8	1	0
      0xB0080000,  //  035A  RAISE	2	R0	R0
      0x80040E00,  //  035B  RET	1	R7
      0x70020033,  //  035C  JMP		#0391
      0x1C1C0D09,  //  035D  EQ	R7	R6	K9
      0x781E0013,  //  035E  JMPF	R7	#0373
      0x8C1C0911,  //  035F  GETMET	R7	R4	K17
      0x7C1C0200,  //  0360  CALL	R7	1
      0x60200010,  //  0361  GETGBL	R8	G16
      0x8C240156,  //  0362  GETMET	R9	R0	K86
      0x7C240200,  //  0363  CALL	R9	1
      0x7C200200,  //  0364  CALL	R8	1
      0xA8020007,  //  0365  EXBLK	0	#036E
      0x5C241000,  //  0366  MOVE	R9	R8
      0x7C240000,  //  0367  CALL	R9	0
      0x8C280F0B,  //  0368  GETMET	R10	R7	K11
      0x4C300000,  //  0369  LDNIL	R12
      0x8834092A,  //  036A  GETMBR	R13	R4	K42
      0x5C381200,  //  036B  MOVE	R14	R9
      0x7C280800,  //  036C  CALL	R10	4
      0x7001FFF7,  //  036D  JMP		#0366
      0x5820003A,  //  036E  LDCONST	R8	K58
      0xAC200200,  //  036F  CATCH	R8	1	0
      0xB0080000,  //  0370  RAISE	2	R0	R0
      0x80040E00,  //  0371  RET	1	R7
      0x7002001D,  //  0372  JMP		#0391
      0x1C1C0D0D,  //  0373  EQ	R7	R6	K13
      0x781E0003,  //  0374  JMPF	R7	#0379
      0x8C1C0911,  //  0375  GETMET	R7	R4	K17
      0x7C1C0200,  //  0376  CALL	R7	1
      0x80040E00,  //  0377  RET	1	R7
      0x70020017,  //  0378  JMP		#0391
      0x1C1C0D0F,  //  0379  EQ	R7	R6	K15
      0x781E0015,  //  037A  JMPF	R7	#0391
      0x881C0133,  //  037B  GETMBR	R7	R0	K51
      0x8C1C0F57,  //  037C  GETMET	R7	R7	K87
      0x50240200,  //  037D  LDBOOL	R9	1	0
      0x7C1C0400,  //  037E  CALL	R7	2
      0x8C200911,  //  037F  GETMET	R8	R4	K17
      0x7C200200,  //  0380  CALL	R8	1
      0x60240010,  //  0381  GETGBL	R9	G16
      0x5C280E00,  //  0382  MOVE	R10	R7
      0x7C240200,  //  0383  CALL	R9	1
      0xA8020007,  //  0384  EXBLK	0	#038D
      0x5C281200,  //  0385  MOVE	R10	R9
      0x7C280000,  //  0386  CALL	R10	0
      0x8C2C110B,  //  0387  GETMET	R11	R8	K11
      0x4C340000,  //  0388  LDNIL	R13
      0x8838090C,  //  0389  GETMBR	R14	R4	K12
      0x5C3C1400,  //  038A  MOVE	R15	R10
      0x7C2C0800,  //  038B  CALL	R11	4
      0x7001FFF7,  //  038C  JMP		#0385
      0x5824003A,  //  038D  LDCONST	R9	K58
      0xAC240200,  //  038E  CATCH	R9	1	0
      0xB0080000,  //  038F  RAISE	2	R0	R0
      0x80041000,  //  0390  RET	1	R8
      0x70020002,  //  0391  JMP		#0395
      0xB81E0200,  //  0392  GETNGBL	R7	K1
      0x881C0F59,  //  0393  GETMBR	R7	R7	K89
      0x900AB007,  //  0394  SETMBR	R2	K88	R7
      0x80000000,  //  0395  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Root_write_attribute,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(int),
    /* K7   */  be_nested_str_weak(int64),
    /* K8   */  be_nested_str_weak(_breadcrumb),
    /* K9   */  be_nested_str_weak(attribute_updated),
    /* K10  */  be_nested_str_weak(endpoint),
    /* K11  */  be_nested_str_weak(status),
    /* K12  */  be_nested_str_weak(CONSTRAINT_ERROR),
    /* K13  */  be_const_int(1),
    /* K14  */  be_nested_str_weak(INVALID_ACTION),
    }),
    be_str_weak(write_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[102]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x881C0504,  //  0004  GETMBR	R7	R2	K4
      0x5422002F,  //  0005  LDINT	R8	48
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x7822001A,  //  0007  JMPF	R8	#0023
      0x1C200F05,  //  0008  EQ	R8	R7	K5
      0x78220017,  //  0009  JMPF	R8	#0022
      0x60200004,  //  000A  GETGBL	R8	G4
      0x5C240600,  //  000B  MOVE	R9	R3
      0x7C200200,  //  000C  CALL	R8	1
      0x1C201106,  //  000D  EQ	R8	R8	K6
      0x74220004,  //  000E  JMPT	R8	#0014
      0x6020000F,  //  000F  GETGBL	R8	G15
      0x5C240600,  //  0010  MOVE	R9	R3
      0xB82A0E00,  //  0011  GETNGBL	R10	K7
      0x7C200400,  //  0012  CALL	R8	2
      0x78220008,  //  0013  JMPF	R8	#001D
      0x90061003,  //  0014  SETMBR	R1	K8	R3
      0x8C200109,  //  0015  GETMET	R8	R0	K9
      0x8828050A,  //  0016  GETMBR	R10	R2	K10
      0x882C0503,  //  0017  GETMBR	R11	R2	K3
      0x88300504,  //  0018  GETMBR	R12	R2	K4
      0x7C200800,  //  0019  CALL	R8	4
      0x50200200,  //  001A  LDBOOL	R8	1	0
      0x80041000,  //  001B  RET	1	R8
      0x70020004,  //  001C  JMP		#0022
      0xB8220200,  //  001D  GETNGBL	R8	K1
      0x8820110C,  //  001E  GETMBR	R8	R8	K12
      0x900A1608,  //  001F  SETMBR	R2	K11	R8
      0x50200000,  //  0020  LDBOOL	R8	0	0
      0x80041000,  //  0021  RET	1	R8
      0x70020041,  //  0022  JMP		#0065
      0x5422001E,  //  0023  LDINT	R8	31
      0x1C200C08,  //  0024  EQ	R8	R6	R8
      0x78220004,  //  0025  JMPF	R8	#002B
      0x1C200F05,  //  0026  EQ	R8	R7	K5
      0x78220001,  //  0027  JMPF	R8	#002A
      0x50200200,  //  0028  LDBOOL	R8	1	0
      0x80041000,  //  0029  RET	1	R8
      0x70020039,  //  002A  JMP		#0065
      0x54220027,  //  002B  LDINT	R8	40
      0x1C200C08,  //  002C  EQ	R8	R6	R8
      0x7822000B,  //  002D  JMPF	R8	#003A
      0x54220004,  //  002E  LDINT	R8	5
      0x1C200E08,  //  002F  EQ	R8	R7	R8
      0x78220002,  //  0030  JMPF	R8	#0034
      0x50200200,  //  0031  LDBOOL	R8	1	0
      0x80041000,  //  0032  RET	1	R8
      0x70020004,  //  0033  JMP		#0039
      0x54220005,  //  0034  LDINT	R8	6
      0x1C200E08,  //  0035  EQ	R8	R7	R8
      0x78220001,  //  0036  JMPF	R8	#0039
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x80041000,  //  0038  RET	1	R8
      0x7002002A,  //  0039  JMP		#0065
      0x54220029,  //  003A  LDINT	R8	42
      0x1C200C08,  //  003B  EQ	R8	R6	R8
      0x78220004,  //  003C  JMPF	R8	#0042
      0x1C200F05,  //  003D  EQ	R8	R7	K5
      0x78220001,  //  003E  JMPF	R8	#0041
      0x50200200,  //  003F  LDBOOL	R8	1	0
      0x80041000,  //  0040  RET	1	R8
      0x70020022,  //  0041  JMP		#0065
      0x5422002A,  //  0042  LDINT	R8	43
      0x1C200C08,  //  0043  EQ	R8	R6	R8
      0x78220007,  //  0044  JMPF	R8	#004D
      0x1C200F05,  //  0045  EQ	R8	R7	K5
      0x78220004,  //  0046  JMPF	R8	#004C
      0xB8220200,  //  0047  GETNGBL	R8	K1
      0x8820110C,  //  0048  GETMBR	R8	R8	K12
      0x900A1608,  //  0049  SETMBR	R2	K11	R8
      0x50200000,  //  004A  LDBOOL	R8	0	0
      0x80041000,  //  004B  RET	1	R8
      0x70020017,  //  004C  JMP		#0065
      0x5422002B,  //  004D  LDINT	R8	44
      0x1C200C08,  //  004E  EQ	R8	R6	R8
      0x78220009,  //  004F  JMPF	R8	#005A
      0x1C200F05,  //  0050  EQ	R8	R7	K5
      0x78220002,  //  0051  JMPF	R8	#0055
      0x50200200,  //  0052  LDBOOL	R8	1	0
      0x80041000,  //  0053  RET	1	R8
      0x70020003,  //  0054  JMP		#0059
      0x1C200F0D,  //  0055  EQ	R8	R7	K13
      0x78220001,  //  0056  JMPF	R8	#0059
      0x50200200,  //  0057  LDBOOL	R8	1	0
      0x80041000,  //  0058  RET	1	R8
      0x7002000A,  //  0059  JMP		#0065
      0x54220030,  //  005A  LDINT	R8	49
      0x1C200C08,  //  005B  EQ	R8	R6	R8
      0x78220007,  //  005C  JMPF	R8	#0065
      0x54220003,  //  005D  LDINT	R8	4
      0x1C200E08,  //  005E  EQ	R8	R7	R8
      0x78220004,  //  005F  JMPF	R8	#0065
      0xB8220200,  //  0060  GETNGBL	R8	K1
      0x8820110E,  //  0061  GETMBR	R8	R8	K14
      0x900A1608,  //  0062  SETMBR	R2	K11	R8
      0x50200000,  //  0063  LDBOOL	R8	0	0
      0x80041000,  //  0064  RET	1	R8
      0x80000000,  //  0065  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Root_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(clusters),
    /* K2   */  be_nested_str_weak(CLUSTERS),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0x90020203,  //  0008  SETMBR	R0	K1	R3
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Root_invoke_request,   /* name */
  be_nested_proto(
    29,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[94]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(TLV),
    /* K4   */  be_nested_str_weak(cluster),
    /* K5   */  be_nested_str_weak(command),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(findsubval),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(_breadcrumb),
    /* K10  */  be_nested_str_weak(Matter_TLV_struct),
    /* K11  */  be_nested_str_weak(add_TLV),
    /* K12  */  be_nested_str_weak(U1),
    /* K13  */  be_nested_str_weak(UTF1),
    /* K14  */  be_nested_str_weak(),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(XX),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(fabric_completed),
    /* K19  */  be_nested_str_weak(set_no_expiration),
    /* K20  */  be_nested_str_weak(save),
    /* K21  */  be_nested_str_weak(device),
    /* K22  */  be_nested_str_weak(start_commissioning_complete_deferred),
    /* K23  */  be_nested_str_weak(status),
    /* K24  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K25  */  be_nested_str_weak(B2),
    /* K26  */  be_nested_str_weak(DAC_Cert_FFF1_8000),
    /* K27  */  be_nested_str_weak(PAI_Cert_FFF1),
    /* K28  */  be_nested_str_weak(CD_FFF1_8000),
    /* K29  */  be_nested_str_weak(B1),
    /* K30  */  be_nested_str_weak(U4),
    /* K31  */  be_nested_str_weak(tasmota),
    /* K32  */  be_nested_str_weak(rtc),
    /* K33  */  be_nested_str_weak(utc),
    /* K34  */  be_nested_str_weak(encode),
    /* K35  */  be_nested_str_weak(get_ac),
    /* K36  */  be_nested_str_weak(log),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20attestation_tbs_X3D),
    /* K38  */  be_nested_str_weak(tohex),
    /* K39  */  be_nested_str_weak(EC_P256),
    /* K40  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K41  */  be_nested_str_weak(DAC_Priv_FFF1_8000),
    /* K42  */  be_nested_str_weak(gen_CSR),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20nocsr_tbs_X3D),
    /* K44  */  be_nested_str_weak(set_ca),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20received_X20ca_root_X3D),
    /* K46  */  be_nested_str_weak(SUCCESS),
    /* K47  */  be_nested_str_weak(get_ca),
    /* K48  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20AdNOC_X20without_X20CA),
    /* K49  */  be_nested_str_weak(set_noc),
    /* K50  */  be_nested_str_weak(set_ipk_epoch_key),
    /* K51  */  be_nested_str_weak(set_admin_subject_vendor),
    /* K52  */  be_nested_str_weak(parse),
    /* K53  */  be_nested_str_weak(findsub),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20no_X20fabricid_X20nor_X20deviceid_X20in_X20NOC_X20certificate),
    /* K55  */  be_nested_str_weak(int),
    /* K56  */  be_nested_str_weak(int64),
    /* K57  */  be_nested_str_weak(fromu32),
    /* K58  */  be_nested_str_weak(tobytes),
    /* K59  */  be_const_int(2147483647),
    /* K60  */  be_nested_str_weak(fromstring),
    /* K61  */  be_nested_str_weak(CompressedFabric),
    /* K62  */  be_nested_str_weak(HKDF_SHA256),
    /* K63  */  be_nested_str_weak(copy),
    /* K64  */  be_nested_str_weak(reverse),
    /* K65  */  be_nested_str_weak(derive),
    /* K66  */  be_nested_str_weak(set_fabric_device),
    /* K67  */  be_nested_str_weak(persist_to_fabric),
    /* K68  */  be_nested_str_weak(fabric_candidate),
    /* K69  */  be_nested_str_weak(start_operational_discovery_deferred),
    /* K70  */  be_nested_str_weak(MTR_X3A_X20_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D),
    /* K71  */  be_nested_str_weak(MTR_X3A_X20fabric_X3D),
    /* K72  */  be_nested_str_weak(inspect),
    /* K73  */  be_nested_str_weak(_fabric),
    /* K74  */  be_nested_str_weak(set_fabric_label),
    /* K75  */  be_nested_str_weak(sessions),
    /* K76  */  be_nested_str_weak(active_fabrics),
    /* K77  */  be_nested_str_weak(get_fabric_index),
    /* K78  */  be_nested_str_weak(MTR_X3A_X20removing_X20fabric_X20),
    /* K79  */  be_nested_str_weak(get_fabric_id),
    /* K80  */  be_nested_str_weak(remove_fabric),
    /* K81  */  be_nested_str_weak(stop_iteration),
    /* K82  */  be_nested_str_weak(MTR_X3A_X20RemoveFabric_X20fabric_X28),
    /* K83  */  be_nested_str_weak(_X29_X20not_X20found),
    /* K84  */  be_nested_str_weak(format),
    /* K85  */  be_nested_str_weak(MTR_X3A_X20OpenCommissioningWindow_X28timeout_X3D_X25i_X2C_X20passcode_X3D_X25s_X2C_X20discriminator_X3D_X25i_X2C_X20iterations_X3D_X25i_X2C_X20salt_X3D_X25s_X29),
    /* K86  */  be_nested_str_weak(INVALID_DATA_TYPE),
    /* K87  */  be_nested_str_weak(MTR_X3A_X20wrong_X20size_X20for_X20PAKE_X20parameters),
    /* K88  */  be_nested_str_weak(CONSTRAINT_ERROR),
    /* K89  */  be_nested_str_weak(start_basic_commissioning),
    /* K90  */  be_nested_str_weak(get_fabric),
    /* K91  */  be_nested_str_weak(MTR_X3A_X20OpenBasicCommissioningWindow_X20commissioning_timeout_X3D),
    /* K92  */  be_nested_str_weak(start_root_basic_commissioning),
    /* K93  */  be_nested_str_weak(stop_basic_commissioning),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[684]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xA4160200,  //  0001  IMPORT	R5	K1
      0xB81A0400,  //  0002  GETNGBL	R6	K2
      0x88180D03,  //  0003  GETMBR	R6	R6	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x88200705,  //  0005  GETMBR	R8	R3	K5
      0x5426002F,  //  0006  LDINT	R9	48
      0x1C240E09,  //  0007  EQ	R9	R7	R9
      0x78260054,  //  0008  JMPF	R9	#005E
      0x1C241106,  //  0009  EQ	R9	R8	K6
      0x78260017,  //  000A  JMPF	R9	#0023
      0x8C240507,  //  000B  GETMET	R9	R2	K7
      0x582C0006,  //  000C  LDCONST	R11	K6
      0x54320383,  //  000D  LDINT	R12	900
      0x7C240600,  //  000E  CALL	R9	3
      0x8C280507,  //  000F  GETMET	R10	R2	K7
      0x58300008,  //  0010  LDCONST	R12	K8
      0x58340006,  //  0011  LDCONST	R13	K6
      0x7C280600,  //  0012  CALL	R10	3
      0x9006120A,  //  0013  SETMBR	R1	K9	R10
      0x8C2C0D0A,  //  0014  GETMET	R11	R6	K10
      0x7C2C0200,  //  0015  CALL	R11	1
      0x8C30170B,  //  0016  GETMET	R12	R11	K11
      0x58380006,  //  0017  LDCONST	R14	K6
      0x883C0D0C,  //  0018  GETMBR	R15	R6	K12
      0x58400006,  //  0019  LDCONST	R16	K6
      0x7C300800,  //  001A  CALL	R12	4
      0x8C30170B,  //  001B  GETMET	R12	R11	K11
      0x58380008,  //  001C  LDCONST	R14	K8
      0x883C0D0D,  //  001D  GETMBR	R15	R6	K13
      0x5840000E,  //  001E  LDCONST	R16	K14
      0x7C300800,  //  001F  CALL	R12	4
      0x900E0B08,  //  0020  SETMBR	R3	K5	K8
      0x80041600,  //  0021  RET	1	R11
      0x70020039,  //  0022  JMP		#005D
      0x1C24110F,  //  0023  EQ	R9	R8	K15
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
      0x9006120B,  //  0030  SETMBR	R1	K9	R11
      0x8C300D0A,  //  0031  GETMET	R12	R6	K10
      0x7C300200,  //  0032  CALL	R12	1
      0x8C34190B,  //  0033  GETMET	R13	R12	K11
      0x583C0006,  //  0034  LDCONST	R15	K6
      0x88400D0C,  //  0035  GETMBR	R16	R6	K12
      0x58440006,  //  0036  LDCONST	R17	K6
      0x7C340800,  //  0037  CALL	R13	4
      0x8C34190B,  //  0038  GETMET	R13	R12	K11
      0x583C0008,  //  0039  LDCONST	R15	K8
      0x88400D0D,  //  003A  GETMBR	R16	R6	K13
      0x5844000E,  //  003B  LDCONST	R17	K14
      0x7C340800,  //  003C  CALL	R13	4
      0x900E0B11,  //  003D  SETMBR	R3	K5	K17
      0x80041800,  //  003E  RET	1	R12
      0x7002001C,  //  003F  JMP		#005D
      0x54260003,  //  0040  LDINT	R9	4
      0x1C241009,  //  0041  EQ	R9	R8	R9
      0x78260019,  //  0042  JMPF	R9	#005D
      0x90061306,  //  0043  SETMBR	R1	K9	K6
      0x8C240312,  //  0044  GETMET	R9	R1	K18
      0x7C240200,  //  0045  CALL	R9	1
      0x8C240313,  //  0046  GETMET	R9	R1	K19
      0x7C240200,  //  0047  CALL	R9	1
      0x8C240314,  //  0048  GETMET	R9	R1	K20
      0x7C240200,  //  0049  CALL	R9	1
      0x8C240D0A,  //  004A  GETMET	R9	R6	K10
      0x7C240200,  //  004B  CALL	R9	1
      0x8C28130B,  //  004C  GETMET	R10	R9	K11
      0x58300006,  //  004D  LDCONST	R12	K6
      0x88340D0C,  //  004E  GETMBR	R13	R6	K12
      0x58380006,  //  004F  LDCONST	R14	K6
      0x7C280800,  //  0050  CALL	R10	4
      0x8C28130B,  //  0051  GETMET	R10	R9	K11
      0x58300008,  //  0052  LDCONST	R12	K8
      0x88340D0D,  //  0053  GETMBR	R13	R6	K13
      0x5838000E,  //  0054  LDCONST	R14	K14
      0x7C280800,  //  0055  CALL	R10	4
      0x542A0004,  //  0056  LDINT	R10	5
      0x900E0A0A,  //  0057  SETMBR	R3	K5	R10
      0x88280115,  //  0058  GETMBR	R10	R0	K21
      0x8C281516,  //  0059  GETMET	R10	R10	K22
      0x5C300200,  //  005A  MOVE	R12	R1
      0x7C280400,  //  005B  CALL	R10	2
      0x80041200,  //  005C  RET	1	R9
      0x7002024C,  //  005D  JMP		#02AB
      0x5426003D,  //  005E  LDINT	R9	62
      0x1C240E09,  //  005F  EQ	R9	R7	R9
      0x782601BF,  //  0060  JMPF	R9	#0221
      0x1C24110F,  //  0061  EQ	R9	R8	K15
      0x7826001D,  //  0062  JMPF	R9	#0081
      0x8C240507,  //  0063  GETMET	R9	R2	K7
      0x582C0006,  //  0064  LDCONST	R11	K6
      0x7C240400,  //  0065  CALL	R9	2
      0x20281308,  //  0066  NE	R10	R9	K8
      0x782A0006,  //  0067  JMPF	R10	#006F
      0x2028130F,  //  0068  NE	R10	R9	K15
      0x782A0004,  //  0069  JMPF	R10	#006F
      0xB82A0400,  //  006A  GETNGBL	R10	K2
      0x88281518,  //  006B  GETMBR	R10	R10	K24
      0x900E2E0A,  //  006C  SETMBR	R3	K23	R10
      0x4C280000,  //  006D  LDNIL	R10
      0x80041400,  //  006E  RET	1	R10
      0x8C280D0A,  //  006F  GETMET	R10	R6	K10
      0x7C280200,  //  0070  CALL	R10	1
      0x8C2C150B,  //  0071  GETMET	R11	R10	K11
      0x58340006,  //  0072  LDCONST	R13	K6
      0x88380D19,  //  0073  GETMBR	R14	R6	K25
      0x1C3C1308,  //  0074  EQ	R15	R9	K8
      0x783E0003,  //  0075  JMPF	R15	#007A
      0xB83E0400,  //  0076  GETNGBL	R15	K2
      0x8C3C1F1A,  //  0077  GETMET	R15	R15	K26
      0x7C3C0200,  //  0078  CALL	R15	1
      0x70020002,  //  0079  JMP		#007D
      0xB83E0400,  //  007A  GETNGBL	R15	K2
      0x8C3C1F1B,  //  007B  GETMET	R15	R15	K27
      0x7C3C0200,  //  007C  CALL	R15	1
      0x7C2C0800,  //  007D  CALL	R11	4
      0x900E0B11,  //  007E  SETMBR	R3	K5	K17
      0x80041400,  //  007F  RET	1	R10
      0x7002019E,  //  0080  JMP		#0220
      0x1C241106,  //  0081  EQ	R9	R8	K6
      0x78260044,  //  0082  JMPF	R9	#00C8
      0x8C240507,  //  0083  GETMET	R9	R2	K7
      0x582C0006,  //  0084  LDCONST	R11	K6
      0x7C240400,  //  0085  CALL	R9	2
      0x6028000C,  //  0086  GETGBL	R10	G12
      0x5C2C1200,  //  0087  MOVE	R11	R9
      0x7C280200,  //  0088  CALL	R10	1
      0x542E001F,  //  0089  LDINT	R11	32
      0x2028140B,  //  008A  NE	R10	R10	R11
      0x782A0001,  //  008B  JMPF	R10	#008E
      0x4C280000,  //  008C  LDNIL	R10
      0x80041400,  //  008D  RET	1	R10
      0x900E0B08,  //  008E  SETMBR	R3	K5	K8
      0x8C280D0A,  //  008F  GETMET	R10	R6	K10
      0x7C280200,  //  0090  CALL	R10	1
      0x8C2C150B,  //  0091  GETMET	R11	R10	K11
      0x58340008,  //  0092  LDCONST	R13	K8
      0x88380D19,  //  0093  GETMBR	R14	R6	K25
      0xB83E0400,  //  0094  GETNGBL	R15	K2
      0x8C3C1F1C,  //  0095  GETMET	R15	R15	K28
      0x7C3C0200,  //  0096  CALL	R15	1
      0x7C2C0800,  //  0097  CALL	R11	4
      0x8C2C150B,  //  0098  GETMET	R11	R10	K11
      0x5834000F,  //  0099  LDCONST	R13	K15
      0x88380D1D,  //  009A  GETMBR	R14	R6	K29
      0x5C3C1200,  //  009B  MOVE	R15	R9
      0x7C2C0800,  //  009C  CALL	R11	4
      0x8C2C150B,  //  009D  GETMET	R11	R10	K11
      0x58340011,  //  009E  LDCONST	R13	K17
      0x88380D1E,  //  009F  GETMBR	R14	R6	K30
      0xB83E3E00,  //  00A0  GETNGBL	R15	K31
      0x8C3C1F20,  //  00A1  GETMET	R15	R15	K32
      0x7C3C0200,  //  00A2  CALL	R15	1
      0x943C1F21,  //  00A3  GETIDX	R15	R15	K33
      0x7C2C0800,  //  00A4  CALL	R11	4
      0x8C2C1522,  //  00A5  GETMET	R11	R10	K34
      0x7C2C0200,  //  00A6  CALL	R11	1
      0x8C300323,  //  00A7  GETMET	R12	R1	K35
      0x7C300200,  //  00A8  CALL	R12	1
      0x0034160C,  //  00A9  ADD	R13	R11	R12
      0xB83A3E00,  //  00AA  GETNGBL	R14	K31
      0x8C381D24,  //  00AB  GETMET	R14	R14	K36
      0x8C401B26,  //  00AC  GETMET	R16	R13	K38
      0x7C400200,  //  00AD  CALL	R16	1
      0x00424A10,  //  00AE  ADD	R16	K37	R16
      0x58440011,  //  00AF  LDCONST	R17	K17
      0x7C380600,  //  00B0  CALL	R14	3
      0x8C380927,  //  00B1  GETMET	R14	R4	K39
      0x7C380200,  //  00B2  CALL	R14	1
      0x8C381D28,  //  00B3  GETMET	R14	R14	K40
      0xB8420400,  //  00B4  GETNGBL	R16	K2
      0x8C402129,  //  00B5  GETMET	R16	R16	K41
      0x7C400200,  //  00B6  CALL	R16	1
      0x5C441A00,  //  00B7  MOVE	R17	R13
      0x7C380600,  //  00B8  CALL	R14	3
      0x8C3C0D0A,  //  00B9  GETMET	R15	R6	K10
      0x7C3C0200,  //  00BA  CALL	R15	1
      0x8C401F0B,  //  00BB  GETMET	R16	R15	K11
      0x58480006,  //  00BC  LDCONST	R18	K6
      0x884C0D19,  //  00BD  GETMBR	R19	R6	K25
      0x5C501600,  //  00BE  MOVE	R20	R11
      0x7C400800,  //  00BF  CALL	R16	4
      0x8C401F0B,  //  00C0  GETMET	R16	R15	K11
      0x58480008,  //  00C1  LDCONST	R18	K8
      0x884C0D1D,  //  00C2  GETMBR	R19	R6	K29
      0x5C501C00,  //  00C3  MOVE	R20	R14
      0x7C400800,  //  00C4  CALL	R16	4
      0x900E0B08,  //  00C5  SETMBR	R3	K5	K8
      0x80041E00,  //  00C6  RET	1	R15
      0x70020157,  //  00C7  JMP		#0220
      0x54260003,  //  00C8  LDINT	R9	4
      0x1C241009,  //  00C9  EQ	R9	R8	R9
      0x78260040,  //  00CA  JMPF	R9	#010C
      0x8C240507,  //  00CB  GETMET	R9	R2	K7
      0x582C0006,  //  00CC  LDCONST	R11	K6
      0x7C240400,  //  00CD  CALL	R9	2
      0x6028000C,  //  00CE  GETGBL	R10	G12
      0x5C2C1200,  //  00CF  MOVE	R11	R9
      0x7C280200,  //  00D0  CALL	R10	1
      0x542E001F,  //  00D1  LDINT	R11	32
      0x2028140B,  //  00D2  NE	R10	R10	R11
      0x782A0001,  //  00D3  JMPF	R10	#00D6
      0x4C280000,  //  00D4  LDNIL	R10
      0x80041400,  //  00D5  RET	1	R10
      0x8C280507,  //  00D6  GETMET	R10	R2	K7
      0x58300008,  //  00D7  LDCONST	R12	K8
      0x50340000,  //  00D8  LDBOOL	R13	0	0
      0x7C280600,  //  00D9  CALL	R10	3
      0x8C2C032A,  //  00DA  GETMET	R11	R1	K42
      0x7C2C0200,  //  00DB  CALL	R11	1
      0x8C300D0A,  //  00DC  GETMET	R12	R6	K10
      0x7C300200,  //  00DD  CALL	R12	1
      0x8C34190B,  //  00DE  GETMET	R13	R12	K11
      0x583C0008,  //  00DF  LDCONST	R15	K8
      0x88400D19,  //  00E0  GETMBR	R16	R6	K25
      0x5C441600,  //  00E1  MOVE	R17	R11
      0x7C340800,  //  00E2  CALL	R13	4
      0x8C34190B,  //  00E3  GETMET	R13	R12	K11
      0x583C000F,  //  00E4  LDCONST	R15	K15
      0x88400D1D,  //  00E5  GETMBR	R16	R6	K29
      0x5C441200,  //  00E6  MOVE	R17	R9
      0x7C340800,  //  00E7  CALL	R13	4
      0x8C341922,  //  00E8  GETMET	R13	R12	K34
      0x7C340200,  //  00E9  CALL	R13	1
      0x8C380323,  //  00EA  GETMET	R14	R1	K35
      0x7C380200,  //  00EB  CALL	R14	1
      0x00381A0E,  //  00EC  ADD	R14	R13	R14
      0xB83E3E00,  //  00ED  GETNGBL	R15	K31
      0x8C3C1F24,  //  00EE  GETMET	R15	R15	K36
      0x8C441D26,  //  00EF  GETMET	R17	R14	K38
      0x7C440200,  //  00F0  CALL	R17	1
      0x00465611,  //  00F1  ADD	R17	K43	R17
      0x58480011,  //  00F2  LDCONST	R18	K17
      0x7C3C0600,  //  00F3  CALL	R15	3
      0x8C3C0927,  //  00F4  GETMET	R15	R4	K39
      0x7C3C0200,  //  00F5  CALL	R15	1
      0x8C3C1F28,  //  00F6  GETMET	R15	R15	K40
      0xB8460400,  //  00F7  GETNGBL	R17	K2
      0x8C442329,  //  00F8  GETMET	R17	R17	K41
      0x7C440200,  //  00F9  CALL	R17	1
      0x5C481C00,  //  00FA  MOVE	R18	R14
      0x7C3C0600,  //  00FB  CALL	R15	3
      0x8C400D0A,  //  00FC  GETMET	R16	R6	K10
      0x7C400200,  //  00FD  CALL	R16	1
      0x8C44210B,  //  00FE  GETMET	R17	R16	K11
      0x584C0006,  //  00FF  LDCONST	R19	K6
      0x88500D19,  //  0100  GETMBR	R20	R6	K25
      0x5C541A00,  //  0101  MOVE	R21	R13
      0x7C440800,  //  0102  CALL	R17	4
      0x8C44210B,  //  0103  GETMET	R17	R16	K11
      0x584C0008,  //  0104  LDCONST	R19	K8
      0x88500D1D,  //  0105  GETMBR	R20	R6	K29
      0x5C541E00,  //  0106  MOVE	R21	R15
      0x7C440800,  //  0107  CALL	R17	4
      0x54460004,  //  0108  LDINT	R17	5
      0x900E0A11,  //  0109  SETMBR	R3	K5	R17
      0x80042000,  //  010A  RET	1	R16
      0x70020113,  //  010B  JMP		#0220
      0x5426000A,  //  010C  LDINT	R9	11
      0x1C241009,  //  010D  EQ	R9	R8	R9
      0x78260012,  //  010E  JMPF	R9	#0122
      0x8C240507,  //  010F  GETMET	R9	R2	K7
      0x582C0006,  //  0110  LDCONST	R11	K6
      0x7C240400,  //  0111  CALL	R9	2
      0x8C28032C,  //  0112  GETMET	R10	R1	K44
      0x5C301200,  //  0113  MOVE	R12	R9
      0x7C280400,  //  0114  CALL	R10	2
      0xB82A3E00,  //  0115  GETNGBL	R10	K31
      0x8C281524,  //  0116  GETMET	R10	R10	K36
      0x8C301326,  //  0117  GETMET	R12	R9	K38
      0x7C300200,  //  0118  CALL	R12	1
      0x00325A0C,  //  0119  ADD	R12	K45	R12
      0x58340011,  //  011A  LDCONST	R13	K17
      0x7C280600,  //  011B  CALL	R10	3
      0xB82A0400,  //  011C  GETNGBL	R10	K2
      0x8828152E,  //  011D  GETMBR	R10	R10	K46
      0x900E2E0A,  //  011E  SETMBR	R3	K23	R10
      0x4C280000,  //  011F  LDNIL	R10
      0x80041400,  //  0120  RET	1	R10
      0x700200FD,  //  0121  JMP		#0220
      0x54260005,  //  0122  LDINT	R9	6
      0x1C241009,  //  0123  EQ	R9	R8	R9
      0x782600B2,  //  0124  JMPF	R9	#01D8
      0x8C240507,  //  0125  GETMET	R9	R2	K7
      0x582C0006,  //  0126  LDCONST	R11	K6
      0x7C240400,  //  0127  CALL	R9	2
      0x8C280507,  //  0128  GETMET	R10	R2	K7
      0x58300008,  //  0129  LDCONST	R12	K8
      0x7C280400,  //  012A  CALL	R10	2
      0x602C000C,  //  012B  GETGBL	R11	G12
      0x5C301400,  //  012C  MOVE	R12	R10
      0x7C2C0200,  //  012D  CALL	R11	1
      0x1C2C1706,  //  012E  EQ	R11	R11	K6
      0x782E0000,  //  012F  JMPF	R11	#0131
      0x4C280000,  //  0130  LDNIL	R10
      0x8C2C0507,  //  0131  GETMET	R11	R2	K7
      0x5834000F,  //  0132  LDCONST	R13	K15
      0x7C2C0400,  //  0133  CALL	R11	2
      0x8C300507,  //  0134  GETMET	R12	R2	K7
      0x58380011,  //  0135  LDCONST	R14	K17
      0x7C300400,  //  0136  CALL	R12	2
      0x8C340507,  //  0137  GETMET	R13	R2	K7
      0x543E0003,  //  0138  LDINT	R15	4
      0x7C340400,  //  0139  CALL	R13	2
      0x8C38032F,  //  013A  GETMET	R14	R1	K47
      0x7C380200,  //  013B  CALL	R14	1
      0x4C3C0000,  //  013C  LDNIL	R15
      0x1C381C0F,  //  013D  EQ	R14	R14	R15
      0x783A0006,  //  013E  JMPF	R14	#0146
      0xB83A3E00,  //  013F  GETNGBL	R14	K31
      0x8C381D24,  //  0140  GETMET	R14	R14	K36
      0x58400030,  //  0141  LDCONST	R16	K48
      0x5844000F,  //  0142  LDCONST	R17	K15
      0x7C380600,  //  0143  CALL	R14	3
      0x4C380000,  //  0144  LDNIL	R14
      0x80041C00,  //  0145  RET	1	R14
      0x8C380331,  //  0146  GETMET	R14	R1	K49
      0x5C401200,  //  0147  MOVE	R16	R9
      0x5C441400,  //  0148  MOVE	R17	R10
      0x7C380600,  //  0149  CALL	R14	3
      0x8C380332,  //  014A  GETMET	R14	R1	K50
      0x5C401600,  //  014B  MOVE	R16	R11
      0x7C380400,  //  014C  CALL	R14	2
      0x8C380333,  //  014D  GETMET	R14	R1	K51
      0x5C401800,  //  014E  MOVE	R16	R12
      0x5C441A00,  //  014F  MOVE	R17	R13
      0x7C380600,  //  0150  CALL	R14	3
      0xB83A0400,  //  0151  GETNGBL	R14	K2
      0x88381D03,  //  0152  GETMBR	R14	R14	K3
      0x8C381D34,  //  0153  GETMET	R14	R14	K52
      0x5C401200,  //  0154  MOVE	R16	R9
      0x7C380400,  //  0155  CALL	R14	2
      0x8C3C1D35,  //  0156  GETMET	R15	R14	K53
      0x54460005,  //  0157  LDINT	R17	6
      0x7C3C0400,  //  0158  CALL	R15	2
      0x8C401F07,  //  0159  GETMET	R16	R15	K7
      0x544A0014,  //  015A  LDINT	R18	21
      0x7C400400,  //  015B  CALL	R16	2
      0x8C441F07,  //  015C  GETMET	R17	R15	K7
      0x544E0010,  //  015D  LDINT	R19	17
      0x7C440400,  //  015E  CALL	R17	2
      0x5C482000,  //  015F  MOVE	R18	R16
      0x784A0001,  //  0160  JMPF	R18	#0163
      0x5C482200,  //  0161  MOVE	R18	R17
      0x744A0006,  //  0162  JMPT	R18	#016A
      0xB84A3E00,  //  0163  GETNGBL	R18	K31
      0x8C482524,  //  0164  GETMET	R18	R18	K36
      0x58500036,  //  0165  LDCONST	R20	K54
      0x5854000F,  //  0166  LDCONST	R21	K15
      0x7C480600,  //  0167  CALL	R18	3
      0x50480000,  //  0168  LDBOOL	R18	0	0
      0x80042400,  //  0169  RET	1	R18
      0x60480004,  //  016A  GETGBL	R18	G4
      0x5C4C2000,  //  016B  MOVE	R19	R16
      0x7C480200,  //  016C  CALL	R18	1
      0x1C482537,  //  016D  EQ	R18	R18	K55
      0x784A0007,  //  016E  JMPF	R18	#0177
      0xB84A7000,  //  016F  GETNGBL	R18	K56
      0x8C482539,  //  0170  GETMET	R18	R18	K57
      0x5C502000,  //  0171  MOVE	R20	R16
      0x7C480400,  //  0172  CALL	R18	2
      0x8C48253A,  //  0173  GETMET	R18	R18	K58
      0x7C480200,  //  0174  CALL	R18	1
      0x5C402400,  //  0175  MOVE	R16	R18
      0x70020002,  //  0176  JMP		#017A
      0x8C48213A,  //  0177  GETMET	R18	R16	K58
      0x7C480200,  //  0178  CALL	R18	1
      0x5C402400,  //  0179  MOVE	R16	R18
      0x60480004,  //  017A  GETGBL	R18	G4
      0x5C4C2200,  //  017B  MOVE	R19	R17
      0x7C480200,  //  017C  CALL	R18	1
      0x1C482537,  //  017D  EQ	R18	R18	K55
      0x784A0007,  //  017E  JMPF	R18	#0187
      0xB84A7000,  //  017F  GETNGBL	R18	K56
      0x8C482539,  //  0180  GETMET	R18	R18	K57
      0x5C502200,  //  0181  MOVE	R20	R17
      0x7C480400,  //  0182  CALL	R18	2
      0x8C48253A,  //  0183  GETMET	R18	R18	K58
      0x7C480200,  //  0184  CALL	R18	1
      0x5C442400,  //  0185  MOVE	R17	R18
      0x70020002,  //  0186  JMP		#018A
      0x8C48233A,  //  0187  GETMET	R18	R17	K58
      0x7C480200,  //  0188  CALL	R18	1
      0x5C442400,  //  0189  MOVE	R17	R18
      0xB84A0400,  //  018A  GETNGBL	R18	K2
      0x88482503,  //  018B  GETMBR	R18	R18	K3
      0x8C482534,  //  018C  GETMET	R18	R18	K52
      0x8C50032F,  //  018D  GETMET	R20	R1	K47
      0x7C500200,  //  018E  CALL	R20	1
      0x7C480400,  //  018F  CALL	R18	2
      0x8C482507,  //  0190  GETMET	R18	R18	K7
      0x54520008,  //  0191  LDINT	R20	9
      0x7C480400,  //  0192  CALL	R18	2
      0x404E113B,  //  0193  CONNECT	R19	K8	K59
      0x94482413,  //  0194  GETIDX	R18	R18	R19
      0x60500015,  //  0195  GETGBL	R20	G21
      0x7C500000,  //  0196  CALL	R20	0
      0x8C50293C,  //  0197  GETMET	R20	R20	K60
      0x5858003D,  //  0198  LDCONST	R22	K61
      0x7C500400,  //  0199  CALL	R20	2
      0x5C4C2800,  //  019A  MOVE	R19	R20
      0x8C50093E,  //  019B  GETMET	R20	R4	K62
      0x7C500200,  //  019C  CALL	R20	1
      0x8C54213F,  //  019D  GETMET	R21	R16	K63
      0x7C540200,  //  019E  CALL	R21	1
      0x8C542B40,  //  019F  GETMET	R21	R21	K64
      0x7C540200,  //  01A0  CALL	R21	1
      0x8C582941,  //  01A1  GETMET	R22	R20	K65
      0x5C602400,  //  01A2  MOVE	R24	R18
      0x5C642A00,  //  01A3  MOVE	R25	R21
      0x5C682600,  //  01A4  MOVE	R26	R19
      0x546E0007,  //  01A5  LDINT	R27	8
      0x7C580A00,  //  01A6  CALL	R22	5
      0x8C5C0342,  //  01A7  GETMET	R23	R1	K66
      0x5C642000,  //  01A8  MOVE	R25	R16
      0x5C682200,  //  01A9  MOVE	R26	R17
      0x5C6C2C00,  //  01AA  MOVE	R27	R22
      0x7C5C0800,  //  01AB  CALL	R23	4
      0x8C5C0343,  //  01AC  GETMET	R23	R1	K67
      0x7C5C0200,  //  01AD  CALL	R23	1
      0x8C5C0344,  //  01AE  GETMET	R23	R1	K68
      0x7C5C0200,  //  01AF  CALL	R23	1
      0x885C0115,  //  01B0  GETMBR	R23	R0	K21
      0x8C5C2F45,  //  01B1  GETMET	R23	R23	K69
      0x5C640200,  //  01B2  MOVE	R25	R1
      0x7C5C0400,  //  01B3  CALL	R23	2
      0xB85E3E00,  //  01B4  GETNGBL	R23	K31
      0x8C5C2F24,  //  01B5  GETMET	R23	R23	K36
      0x58640046,  //  01B6  LDCONST	R25	K70
      0x58680011,  //  01B7  LDCONST	R26	K17
      0x7C5C0600,  //  01B8  CALL	R23	3
      0xB85E3E00,  //  01B9  GETNGBL	R23	K31
      0x8C5C2F24,  //  01BA  GETMET	R23	R23	K36
      0xB8660400,  //  01BB  GETNGBL	R25	K2
      0x8C643348,  //  01BC  GETMET	R25	R25	K72
      0x886C0349,  //  01BD  GETMBR	R27	R1	K73
      0x7C640400,  //  01BE  CALL	R25	2
      0x00668E19,  //  01BF  ADD	R25	K71	R25
      0x58680011,  //  01C0  LDCONST	R26	K17
      0x7C5C0600,  //  01C1  CALL	R23	3
      0xB85E3E00,  //  01C2  GETNGBL	R23	K31
      0x8C5C2F24,  //  01C3  GETMET	R23	R23	K36
      0x58640046,  //  01C4  LDCONST	R25	K70
      0x58680011,  //  01C5  LDCONST	R26	K17
      0x7C5C0600,  //  01C6  CALL	R23	3
      0x8C5C0D0A,  //  01C7  GETMET	R23	R6	K10
      0x7C5C0200,  //  01C8  CALL	R23	1
      0x8C602F0B,  //  01C9  GETMET	R24	R23	K11
      0x58680006,  //  01CA  LDCONST	R26	K6
      0x886C0D0C,  //  01CB  GETMBR	R27	R6	K12
      0xB8720400,  //  01CC  GETNGBL	R28	K2
      0x8870392E,  //  01CD  GETMBR	R28	R28	K46
      0x7C600800,  //  01CE  CALL	R24	4
      0x8C602F0B,  //  01CF  GETMET	R24	R23	K11
      0x58680008,  //  01D0  LDCONST	R26	K8
      0x886C0D0C,  //  01D1  GETMBR	R27	R6	K12
      0x58700008,  //  01D2  LDCONST	R28	K8
      0x7C600800,  //  01D3  CALL	R24	4
      0x54620007,  //  01D4  LDINT	R24	8
      0x900E0A18,  //  01D5  SETMBR	R3	K5	R24
      0x80042E00,  //  01D6  RET	1	R23
      0x70020047,  //  01D7  JMP		#0220
      0x54260008,  //  01D8  LDINT	R9	9
      0x1C241009,  //  01D9  EQ	R9	R8	R9
      0x7826000B,  //  01DA  JMPF	R9	#01E7
      0x8C240507,  //  01DB  GETMET	R9	R2	K7
      0x582C0006,  //  01DC  LDCONST	R11	K6
      0x7C240400,  //  01DD  CALL	R9	2
      0x8C28034A,  //  01DE  GETMET	R10	R1	K74
      0x5C301200,  //  01DF  MOVE	R12	R9
      0x7C280400,  //  01E0  CALL	R10	2
      0xB82A0400,  //  01E1  GETNGBL	R10	K2
      0x8828152E,  //  01E2  GETMBR	R10	R10	K46
      0x900E2E0A,  //  01E3  SETMBR	R3	K23	R10
      0x4C280000,  //  01E4  LDNIL	R10
      0x80041400,  //  01E5  RET	1	R10
      0x70020038,  //  01E6  JMP		#0220
      0x54260009,  //  01E7  LDINT	R9	10
      0x1C241009,  //  01E8  EQ	R9	R8	R9
      0x78260035,  //  01E9  JMPF	R9	#0220
      0x8C240507,  //  01EA  GETMET	R9	R2	K7
      0x582C0006,  //  01EB  LDCONST	R11	K6
      0x7C240400,  //  01EC  CALL	R9	2
      0x60280010,  //  01ED  GETGBL	R10	G16
      0x882C0115,  //  01EE  GETMBR	R11	R0	K21
      0x882C174B,  //  01EF  GETMBR	R11	R11	K75
      0x8C2C174C,  //  01F0  GETMET	R11	R11	K76
      0x7C2C0200,  //  01F1  CALL	R11	1
      0x7C280200,  //  01F2  CALL	R10	1
      0xA802001A,  //  01F3  EXBLK	0	#020F
      0x5C2C1400,  //  01F4  MOVE	R11	R10
      0x7C2C0000,  //  01F5  CALL	R11	0
      0x8C30174D,  //  01F6  GETMET	R12	R11	K77
      0x7C300200,  //  01F7  CALL	R12	1
      0x1C301809,  //  01F8  EQ	R12	R12	R9
      0x78320011,  //  01F9  JMPF	R12	#020C
      0xB8323E00,  //  01FA  GETNGBL	R12	K31
      0x8C301924,  //  01FB  GETMET	R12	R12	K36
      0x8C38174F,  //  01FC  GETMET	R14	R11	K79
      0x7C380200,  //  01FD  CALL	R14	1
      0x8C381D3F,  //  01FE  GETMET	R14	R14	K63
      0x7C380200,  //  01FF  CALL	R14	1
      0x8C381D40,  //  0200  GETMET	R14	R14	K64
      0x7C380200,  //  0201  CALL	R14	1
      0x8C381D26,  //  0202  GETMET	R14	R14	K38
      0x7C380200,  //  0203  CALL	R14	1
      0x003A9C0E,  //  0204  ADD	R14	K78	R14
      0x583C000F,  //  0205  LDCONST	R15	K15
      0x7C300600,  //  0206  CALL	R12	3
      0x88300115,  //  0207  GETMBR	R12	R0	K21
      0x8C301950,  //  0208  GETMET	R12	R12	K80
      0x5C381600,  //  0209  MOVE	R14	R11
      0x7C300400,  //  020A  CALL	R12	2
      0x70020000,  //  020B  JMP		#020D
      0x7001FFE6,  //  020C  JMP		#01F4
      0xA8040001,  //  020D  EXBLK	1	1
      0x70020002,  //  020E  JMP		#0212
      0x58280051,  //  020F  LDCONST	R10	K81
      0xAC280200,  //  0210  CATCH	R10	1	0
      0xB0080000,  //  0211  RAISE	2	R0	R0
      0xB82A3E00,  //  0212  GETNGBL	R10	K31
      0x8C281524,  //  0213  GETMET	R10	R10	K36
      0x60300008,  //  0214  GETGBL	R12	G8
      0x5C341200,  //  0215  MOVE	R13	R9
      0x7C300200,  //  0216  CALL	R12	1
      0x0032A40C,  //  0217  ADD	R12	K82	R12
      0x00301953,  //  0218  ADD	R12	R12	K83
      0x5834000F,  //  0219  LDCONST	R13	K15
      0x7C280600,  //  021A  CALL	R10	3
      0xB82A0400,  //  021B  GETNGBL	R10	K2
      0x8828152E,  //  021C  GETMBR	R10	R10	K46
      0x900E2E0A,  //  021D  SETMBR	R3	K23	R10
      0x4C280000,  //  021E  LDNIL	R10
      0x80041400,  //  021F  RET	1	R10
      0x70020089,  //  0220  JMP		#02AB
      0x5426003B,  //  0221  LDINT	R9	60
      0x1C240E09,  //  0222  EQ	R9	R7	R9
      0x7826007F,  //  0223  JMPF	R9	#02A4
      0x1C241106,  //  0224  EQ	R9	R8	K6
      0x78260061,  //  0225  JMPF	R9	#0288
      0x8C240507,  //  0226  GETMET	R9	R2	K7
      0x582C0006,  //  0227  LDCONST	R11	K6
      0x7C240400,  //  0228  CALL	R9	2
      0x8C280507,  //  0229  GETMET	R10	R2	K7
      0x58300008,  //  022A  LDCONST	R12	K8
      0x7C280400,  //  022B  CALL	R10	2
      0x8C2C0507,  //  022C  GETMET	R11	R2	K7
      0x5834000F,  //  022D  LDCONST	R13	K15
      0x7C2C0400,  //  022E  CALL	R11	2
      0x8C300507,  //  022F  GETMET	R12	R2	K7
      0x58380011,  //  0230  LDCONST	R14	K17
      0x7C300400,  //  0231  CALL	R12	2
      0x8C340507,  //  0232  GETMET	R13	R2	K7
      0x543E0003,  //  0233  LDINT	R15	4
      0x7C340400,  //  0234  CALL	R13	2
      0xB83A3E00,  //  0235  GETNGBL	R14	K31
      0x8C381D24,  //  0236  GETMET	R14	R14	K36
      0x8C400B54,  //  0237  GETMET	R16	R5	K84
      0x58480055,  //  0238  LDCONST	R18	K85
      0x5C4C1200,  //  0239  MOVE	R19	R9
      0x8C501526,  //  023A  GETMET	R20	R10	K38
      0x7C500200,  //  023B  CALL	R20	1
      0x5C541600,  //  023C  MOVE	R21	R11
      0x5C581800,  //  023D  MOVE	R22	R12
      0x8C5C1B26,  //  023E  GETMET	R23	R13	K38
      0x7C5C0200,  //  023F  CALL	R23	1
      0x7C400E00,  //  0240  CALL	R16	7
      0x5844000F,  //  0241  LDCONST	R17	K15
      0x7C380600,  //  0242  CALL	R14	3
      0x4C380000,  //  0243  LDNIL	R14
      0x1C38120E,  //  0244  EQ	R14	R9	R14
      0x743A000B,  //  0245  JMPT	R14	#0252
      0x4C380000,  //  0246  LDNIL	R14
      0x1C38140E,  //  0247  EQ	R14	R10	R14
      0x743A0008,  //  0248  JMPT	R14	#0252
      0x4C380000,  //  0249  LDNIL	R14
      0x1C38160E,  //  024A  EQ	R14	R11	R14
      0x743A0005,  //  024B  JMPT	R14	#0252
      0x4C380000,  //  024C  LDNIL	R14
      0x1C38180E,  //  024D  EQ	R14	R12	R14
      0x743A0002,  //  024E  JMPT	R14	#0252
      0x4C380000,  //  024F  LDNIL	R14
      0x1C381A0E,  //  0250  EQ	R14	R13	R14
      0x783A0004,  //  0251  JMPF	R14	#0257
      0xB83A0400,  //  0252  GETNGBL	R14	K2
      0x88381D56,  //  0253  GETMBR	R14	R14	K86
      0x900E2E0E,  //  0254  SETMBR	R3	K23	R14
      0x4C380000,  //  0255  LDNIL	R14
      0x80041C00,  //  0256  RET	1	R14
      0x6038000C,  //  0257  GETGBL	R14	G12
      0x5C3C1400,  //  0258  MOVE	R15	R10
      0x7C380200,  //  0259  CALL	R14	1
      0x543E001F,  //  025A  LDINT	R15	32
      0x54420040,  //  025B  LDINT	R16	65
      0x003C1E10,  //  025C  ADD	R15	R15	R16
      0x20381C0F,  //  025D  NE	R14	R14	R15
      0x743A000B,  //  025E  JMPT	R14	#026B
      0x6038000C,  //  025F  GETGBL	R14	G12
      0x5C3C1A00,  //  0260  MOVE	R15	R13
      0x7C380200,  //  0261  CALL	R14	1
      0x543E000F,  //  0262  LDINT	R15	16
      0x14381C0F,  //  0263  LT	R14	R14	R15
      0x743A0005,  //  0264  JMPT	R14	#026B
      0x6038000C,  //  0265  GETGBL	R14	G12
      0x5C3C1A00,  //  0266  MOVE	R15	R13
      0x7C380200,  //  0267  CALL	R14	1
      0x543E001F,  //  0268  LDINT	R15	32
      0x24381C0F,  //  0269  GT	R14	R14	R15
      0x783A0008,  //  026A  JMPF	R14	#0274
      0xB83A3E00,  //  026B  GETNGBL	R14	K31
      0x8C381D24,  //  026C  GETMET	R14	R14	K36
      0x58400057,  //  026D  LDCONST	R16	K87
      0x7C380400,  //  026E  CALL	R14	2
      0xB83A0400,  //  026F  GETNGBL	R14	K2
      0x88381D58,  //  0270  GETMBR	R14	R14	K88
      0x900E2E0E,  //  0271  SETMBR	R3	K23	R14
      0x4C380000,  //  0272  LDNIL	R14
      0x80041C00,  //  0273  RET	1	R14
      0x543A001E,  //  0274  LDINT	R14	31
      0x403A0C0E,  //  0275  CONNECT	R14	K6	R14
      0x9438140E,  //  0276  GETIDX	R14	R10	R14
      0x543E001F,  //  0277  LDINT	R15	32
      0x403C1F3B,  //  0278  CONNECT	R15	R15	K59
      0x943C140F,  //  0279  GETIDX	R15	R10	R15
      0x88400115,  //  027A  GETMBR	R16	R0	K21
      0x8C402159,  //  027B  GETMET	R16	R16	K89
      0x5C481200,  //  027C  MOVE	R18	R9
      0x5C4C1800,  //  027D  MOVE	R19	R12
      0x5C501600,  //  027E  MOVE	R20	R11
      0x5C541A00,  //  027F  MOVE	R21	R13
      0x5C581C00,  //  0280  MOVE	R22	R14
      0x5C5C1E00,  //  0281  MOVE	R23	R15
      0x8C60035A,  //  0282  GETMET	R24	R1	K90
      0x7C600200,  //  0283  CALL	R24	1
      0x7C401000,  //  0284  CALL	R16	8
      0x50400200,  //  0285  LDBOOL	R16	1	0
      0x80042000,  //  0286  RET	1	R16
      0x7002001A,  //  0287  JMP		#02A3
      0x1C241108,  //  0288  EQ	R9	R8	K8
      0x78260011,  //  0289  JMPF	R9	#029C
      0x8C240507,  //  028A  GETMET	R9	R2	K7
      0x582C0006,  //  028B  LDCONST	R11	K6
      0x7C240400,  //  028C  CALL	R9	2
      0xB82A3E00,  //  028D  GETNGBL	R10	K31
      0x8C281524,  //  028E  GETMET	R10	R10	K36
      0x60300008,  //  028F  GETGBL	R12	G8
      0x5C341200,  //  0290  MOVE	R13	R9
      0x7C300200,  //  0291  CALL	R12	1
      0x0032B60C,  //  0292  ADD	R12	K91	R12
      0x5834000F,  //  0293  LDCONST	R13	K15
      0x7C280600,  //  0294  CALL	R10	3
      0x88280115,  //  0295  GETMBR	R10	R0	K21
      0x8C28155C,  //  0296  GETMET	R10	R10	K92
      0x5C301200,  //  0297  MOVE	R12	R9
      0x7C280400,  //  0298  CALL	R10	2
      0x50280200,  //  0299  LDBOOL	R10	1	0
      0x80041400,  //  029A  RET	1	R10
      0x70020006,  //  029B  JMP		#02A3
      0x1C24110F,  //  029C  EQ	R9	R8	K15
      0x78260004,  //  029D  JMPF	R9	#02A3
      0x88240115,  //  029E  GETMBR	R9	R0	K21
      0x8C24135D,  //  029F  GETMET	R9	R9	K93
      0x7C240200,  //  02A0  CALL	R9	1
      0x50240200,  //  02A1  LDBOOL	R9	1	0
      0x80041200,  //  02A2  RET	1	R9
      0x70020006,  //  02A3  JMP		#02AB
      0x54260029,  //  02A4  LDINT	R9	42
      0x1C240E09,  //  02A5  EQ	R9	R7	R9
      0x78260003,  //  02A6  JMPF	R9	#02AB
      0x1C241106,  //  02A7  EQ	R9	R8	K6
      0x78260001,  //  02A8  JMPF	R9	#02AB
      0x50240200,  //  02A9  LDBOOL	R9	1	0
      0x80041200,  //  02AA  RET	1	R9
      0x80000000,  //  02AB  RET	0
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
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Root_read_attribute_closure) },
        { be_const_key_weak(write_attribute, -1), be_const_closure(Matter_Plugin_Root_write_attribute_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(22, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Root_init_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(56, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(7),
    }))    ) } )) },
        { be_const_key_int(29, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
    }))    ) } )) },
        { be_const_key_int(44, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
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
        { be_const_key_int(43, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
    }))    ) } )) },
        { be_const_key_int(48, 11), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
    }))    ) } )) },
        { be_const_key_int(63, 13), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(50, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(52, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
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
        { be_const_key_int(49, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(4),
        be_const_int(65532),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Root_invoke_request_closure) },
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
