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
    ( &(const bvalue[84]) {     /* constants */
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
    /* K53  */  be_nested_str_weak(sessions_active),
    /* K54  */  be_nested_str_weak(B2),
    /* K55  */  be_nested_str_weak(get_noc),
    /* K56  */  be_nested_str_weak(get_icac),
    /* K57  */  be_nested_str_weak(stop_iteration),
    /* K58  */  be_nested_str_weak(parse),
    /* K59  */  be_nested_str_weak(get_ca),
    /* K60  */  be_nested_str_weak(findsubval),
    /* K61  */  be_nested_str_weak(get_admin_vendor),
    /* K62  */  be_nested_str_weak(get_fabric_compressed),
    /* K63  */  be_nested_str_weak(get_device_id),
    /* K64  */  be_nested_str_weak(get_fabric_label),
    /* K65  */  be_nested_str_weak(Fabric),
    /* K66  */  be_nested_str_weak(_MAX_CASE),
    /* K67  */  be_nested_str_weak(Tasmota),
    /* K68  */  be_nested_str_weak(vendorid),
    /* K69  */  be_nested_str_weak(DeviceName),
    /* K70  */  be_nested_str_weak(FriendlyName),
    /* K71  */  be_nested_str_weak(FriendlyName1),
    /* K72  */  be_nested_str_weak(XX),
    /* K73  */  be_nested_str_weak(Status_X202),
    /* K74  */  be_nested_str_weak(StatusFWR),
    /* K75  */  be_nested_str_weak(Hardware),
    /* K76  */  be_nested_str_weak(Version),
    /* K77  */  be_nested_str_weak(locale),
    /* K78  */  be_nested_str_weak(TYPES),
    /* K79  */  be_nested_str_weak(keys),
    /* K80  */  be_nested_str_weak(get_cluster_list),
    /* K81  */  be_nested_str_weak(get_active_endpoints),
    /* K82  */  be_nested_str_weak(status),
    /* K83  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[842]) {  /* code */
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
      0x7002030F,  //  0038  JMP		#0349
      0x541E0031,  //  0039  LDINT	R7	50
      0x1C1C0A07,  //  003A  EQ	R7	R5	R7
      0x781E0000,  //  003B  JMPF	R7	#003D
      0x7002030B,  //  003C  JMP		#0349
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
      0x7002022B,  //  011C  JMP		#0349
      0x541E0033,  //  011D  LDINT	R7	52
      0x1C1C0A07,  //  011E  EQ	R7	R5	R7
      0x781E0000,  //  011F  JMPF	R7	#0121
      0x70020227,  //  0120  JMP		#0349
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
      0x700201F7,  //  0150  JMP		#0349
      0x541E003D,  //  0151  LDINT	R7	62
      0x1C1C0A07,  //  0152  EQ	R7	R5	R7
      0x781E008A,  //  0153  JMPF	R7	#01DF
      0x1C1C0D05,  //  0154  EQ	R7	R6	K5
      0x781E001F,  //  0155  JMPF	R7	#0176
      0x8C1C0911,  //  0156  GETMET	R7	R4	K17
      0x7C1C0200,  //  0157  CALL	R7	1
      0x60200010,  //  0158  GETGBL	R8	G16
      0x88240133,  //  0159  GETMBR	R9	R0	K51
      0x88241334,  //  015A  GETMBR	R9	R9	K52
      0x8C241335,  //  015B  GETMET	R9	R9	K53
      0x7C240200,  //  015C  CALL	R9	1
      0x7C200200,  //  015D  CALL	R8	1
      0xA8020011,  //  015E  EXBLK	0	#0171
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
      0x7001FFED,  //  0170  JMP		#015F
      0x58200039,  //  0171  LDCONST	R8	K57
      0xAC200200,  //  0172  CATCH	R8	1	0
      0xB0080000,  //  0173  RAISE	2	R0	R0
      0x80040E00,  //  0174  RET	1	R7
      0x70020067,  //  0175  JMP		#01DE
      0x1C1C0D09,  //  0176  EQ	R7	R6	K9
      0x781E0036,  //  0177  JMPF	R7	#01AF
      0x8C1C0911,  //  0178  GETMET	R7	R4	K17
      0x7C1C0200,  //  0179  CALL	R7	1
      0x60200010,  //  017A  GETGBL	R8	G16
      0x88240133,  //  017B  GETMBR	R9	R0	K51
      0x88241334,  //  017C  GETMBR	R9	R9	K52
      0x8C241335,  //  017D  GETMET	R9	R9	K53
      0x7C240200,  //  017E  CALL	R9	1
      0x7C200200,  //  017F  CALL	R8	1
      0xA8020028,  //  0180  EXBLK	0	#01AA
      0x5C241000,  //  0181  MOVE	R9	R8
      0x7C240000,  //  0182  CALL	R9	0
      0x8C28093A,  //  0183  GETMET	R10	R4	K58
      0x8C30133B,  //  0184  GETMET	R12	R9	K59
      0x7C300200,  //  0185  CALL	R12	1
      0x7C280400,  //  0186  CALL	R10	2
      0x8C2C0F15,  //  0187  GETMET	R11	R7	K21
      0x4C340000,  //  0188  LDNIL	R13
      0x7C2C0400,  //  0189  CALL	R11	2
      0x8C30170B,  //  018A  GETMET	R12	R11	K11
      0x58380009,  //  018B  LDCONST	R14	K9
      0x883C0936,  //  018C  GETMBR	R15	R4	K54
      0x8C40153C,  //  018D  GETMET	R16	R10	K60
      0x544A0008,  //  018E  LDINT	R18	9
      0x7C400400,  //  018F  CALL	R16	2
      0x7C300800,  //  0190  CALL	R12	4
      0x8C30170B,  //  0191  GETMET	R12	R11	K11
      0x5838000D,  //  0192  LDCONST	R14	K13
      0x883C090C,  //  0193  GETMBR	R15	R4	K12
      0x8C40133D,  //  0194  GETMET	R16	R9	K61
      0x7C400200,  //  0195  CALL	R16	1
      0x7C300800,  //  0196  CALL	R12	4
      0x8C30170B,  //  0197  GETMET	R12	R11	K11
      0x5838000F,  //  0198  LDCONST	R14	K15
      0x883C0907,  //  0199  GETMBR	R15	R4	K7
      0x8C40133E,  //  019A  GETMET	R16	R9	K62
      0x7C400200,  //  019B  CALL	R16	1
      0x7C300800,  //  019C  CALL	R12	4
      0x8C30170B,  //  019D  GETMET	R12	R11	K11
      0x543A0003,  //  019E  LDINT	R14	4
      0x883C0907,  //  019F  GETMBR	R15	R4	K7
      0x8C40133F,  //  01A0  GETMET	R16	R9	K63
      0x7C400200,  //  01A1  CALL	R16	1
      0x7C300800,  //  01A2  CALL	R12	4
      0x8C30170B,  //  01A3  GETMET	R12	R11	K11
      0x543A0004,  //  01A4  LDINT	R14	5
      0x883C0916,  //  01A5  GETMBR	R15	R4	K22
      0x8C401340,  //  01A6  GETMET	R16	R9	K64
      0x7C400200,  //  01A7  CALL	R16	1
      0x7C300800,  //  01A8  CALL	R12	4
      0x7001FFD6,  //  01A9  JMP		#0181
      0x58200039,  //  01AA  LDCONST	R8	K57
      0xAC200200,  //  01AB  CATCH	R8	1	0
      0xB0080000,  //  01AC  RAISE	2	R0	R0
      0x80040E00,  //  01AD  RET	1	R7
      0x7002002E,  //  01AE  JMP		#01DE
      0x1C1C0D0D,  //  01AF  EQ	R7	R6	K13
      0x781E0007,  //  01B0  JMPF	R7	#01B9
      0x8C1C0906,  //  01B1  GETMET	R7	R4	K6
      0x8824090E,  //  01B2  GETMBR	R9	R4	K14
      0xB82A0200,  //  01B3  GETNGBL	R10	K1
      0x88281541,  //  01B4  GETMBR	R10	R10	K65
      0x88281542,  //  01B5  GETMBR	R10	R10	K66
      0x7C1C0600,  //  01B6  CALL	R7	3
      0x80040E00,  //  01B7  RET	1	R7
      0x70020024,  //  01B8  JMP		#01DE
      0x1C1C0D0F,  //  01B9  EQ	R7	R6	K15
      0x781E000B,  //  01BA  JMPF	R7	#01C7
      0x881C0133,  //  01BB  GETMBR	R7	R0	K51
      0x881C0F34,  //  01BC  GETMBR	R7	R7	K52
      0x8C1C0F35,  //  01BD  GETMET	R7	R7	K53
      0x7C1C0200,  //  01BE  CALL	R7	1
      0x8C200906,  //  01BF  GETMET	R8	R4	K6
      0x8828090E,  //  01C0  GETMBR	R10	R4	K14
      0x602C000C,  //  01C1  GETGBL	R11	G12
      0x5C300E00,  //  01C2  MOVE	R12	R7
      0x7C2C0200,  //  01C3  CALL	R11	1
      0x7C200600,  //  01C4  CALL	R8	3
      0x80041000,  //  01C5  RET	1	R8
      0x70020016,  //  01C6  JMP		#01DE
      0x541E0003,  //  01C7  LDINT	R7	4
      0x1C1C0C07,  //  01C8  EQ	R7	R6	R7
      0x781E0000,  //  01C9  JMPF	R7	#01CB
      0x70020012,  //  01CA  JMP		#01DE
      0x541E0004,  //  01CB  LDINT	R7	5
      0x1C1C0C07,  //  01CC  EQ	R7	R6	R7
      0x781E000F,  //  01CD  JMPF	R7	#01DE
      0x881C0133,  //  01CE  GETMBR	R7	R0	K51
      0x881C0F34,  //  01CF  GETMBR	R7	R7	K52
      0x8C1C0F35,  //  01D0  GETMET	R7	R7	K53
      0x7C1C0200,  //  01D1  CALL	R7	1
      0x8C200F1B,  //  01D2  GETMET	R8	R7	K27
      0x5C280200,  //  01D3  MOVE	R10	R1
      0x7C200400,  //  01D4  CALL	R8	2
      0x4C240000,  //  01D5  LDNIL	R9
      0x1C241009,  //  01D6  EQ	R9	R8	R9
      0x78260000,  //  01D7  JMPF	R9	#01D9
      0x5421FFFE,  //  01D8  LDINT	R8	-1
      0x8C240906,  //  01D9  GETMET	R9	R4	K6
      0x882C090E,  //  01DA  GETMBR	R11	R4	K14
      0x00301109,  //  01DB  ADD	R12	R8	K9
      0x7C240600,  //  01DC  CALL	R9	3
      0x80041200,  //  01DD  RET	1	R9
      0x70020169,  //  01DE  JMP		#0349
      0x541E003B,  //  01DF  LDINT	R7	60
      0x1C1C0A07,  //  01E0  EQ	R7	R5	R7
      0x781E0000,  //  01E1  JMPF	R7	#01E3
      0x70020165,  //  01E2  JMP		#0349
      0x541E0027,  //  01E3  LDINT	R7	40
      0x1C1C0A07,  //  01E4  EQ	R7	R5	R7
      0x781E0095,  //  01E5  JMPF	R7	#027C
      0x1C1C0D05,  //  01E6  EQ	R7	R6	K5
      0x781E0005,  //  01E7  JMPF	R7	#01EE
      0x8C1C0906,  //  01E8  GETMET	R7	R4	K6
      0x8824090C,  //  01E9  GETMBR	R9	R4	K12
      0x58280005,  //  01EA  LDCONST	R10	K5
      0x7C1C0600,  //  01EB  CALL	R7	3
      0x80040E00,  //  01EC  RET	1	R7
      0x7002008C,  //  01ED  JMP		#027B
      0x1C1C0D09,  //  01EE  EQ	R7	R6	K9
      0x781E0005,  //  01EF  JMPF	R7	#01F6
      0x8C1C0906,  //  01F0  GETMET	R7	R4	K6
      0x88240916,  //  01F1  GETMBR	R9	R4	K22
      0x58280043,  //  01F2  LDCONST	R10	K67
      0x7C1C0600,  //  01F3  CALL	R7	3
      0x80040E00,  //  01F4  RET	1	R7
      0x70020084,  //  01F5  JMP		#027B
      0x1C1C0D0D,  //  01F6  EQ	R7	R6	K13
      0x781E0006,  //  01F7  JMPF	R7	#01FF
      0x8C1C0906,  //  01F8  GETMET	R7	R4	K6
      0x8824090C,  //  01F9  GETMBR	R9	R4	K12
      0x88280133,  //  01FA  GETMBR	R10	R0	K51
      0x88281544,  //  01FB  GETMBR	R10	R10	K68
      0x7C1C0600,  //  01FC  CALL	R7	3
      0x80040E00,  //  01FD  RET	1	R7
      0x7002007B,  //  01FE  JMP		#027B
      0x1C1C0D0F,  //  01FF  EQ	R7	R6	K15
      0x781E000A,  //  0200  JMPF	R7	#020C
      0x8C1C0906,  //  0201  GETMET	R7	R4	K6
      0x88240916,  //  0202  GETMBR	R9	R4	K22
      0xB82A2400,  //  0203  GETNGBL	R10	K18
      0x8C281526,  //  0204  GETMET	R10	R10	K38
      0x58300045,  //  0205  LDCONST	R12	K69
      0x50340200,  //  0206  LDBOOL	R13	1	0
      0x7C280600,  //  0207  CALL	R10	3
      0x94281545,  //  0208  GETIDX	R10	R10	K69
      0x7C1C0600,  //  0209  CALL	R7	3
      0x80040E00,  //  020A  RET	1	R7
      0x7002006E,  //  020B  JMP		#027B
      0x541E0003,  //  020C  LDINT	R7	4
      0x1C1C0C07,  //  020D  EQ	R7	R6	R7
      0x781E0005,  //  020E  JMPF	R7	#0215
      0x8C1C0906,  //  020F  GETMET	R7	R4	K6
      0x8824090C,  //  0210  GETMBR	R9	R4	K12
      0x542A7FFF,  //  0211  LDINT	R10	32768
      0x7C1C0600,  //  0212  CALL	R7	3
      0x80040E00,  //  0213  RET	1	R7
      0x70020065,  //  0214  JMP		#027B
      0x541E0004,  //  0215  LDINT	R7	5
      0x1C1C0C07,  //  0216  EQ	R7	R6	R7
      0x781E000A,  //  0217  JMPF	R7	#0223
      0x8C1C0906,  //  0218  GETMET	R7	R4	K6
      0x88240916,  //  0219  GETMBR	R9	R4	K22
      0xB82A2400,  //  021A  GETNGBL	R10	K18
      0x8C281526,  //  021B  GETMET	R10	R10	K38
      0x58300046,  //  021C  LDCONST	R12	K70
      0x50340200,  //  021D  LDBOOL	R13	1	0
      0x7C280600,  //  021E  CALL	R10	3
      0x94281547,  //  021F  GETIDX	R10	R10	K71
      0x7C1C0600,  //  0220  CALL	R7	3
      0x80040E00,  //  0221  RET	1	R7
      0x70020057,  //  0222  JMP		#027B
      0x541E0005,  //  0223  LDINT	R7	6
      0x1C1C0C07,  //  0224  EQ	R7	R6	R7
      0x781E0005,  //  0225  JMPF	R7	#022C
      0x8C1C0906,  //  0226  GETMET	R7	R4	K6
      0x88240916,  //  0227  GETMBR	R9	R4	K22
      0x58280048,  //  0228  LDCONST	R10	K72
      0x7C1C0600,  //  0229  CALL	R7	3
      0x80040E00,  //  022A  RET	1	R7
      0x7002004E,  //  022B  JMP		#027B
      0x541E0006,  //  022C  LDINT	R7	7
      0x1C1C0C07,  //  022D  EQ	R7	R6	R7
      0x781E0005,  //  022E  JMPF	R7	#0235
      0x8C1C0906,  //  022F  GETMET	R7	R4	K6
      0x8824090C,  //  0230  GETMBR	R9	R4	K12
      0x58280005,  //  0231  LDCONST	R10	K5
      0x7C1C0600,  //  0232  CALL	R7	3
      0x80040E00,  //  0233  RET	1	R7
      0x70020045,  //  0234  JMP		#027B
      0x541E0007,  //  0235  LDINT	R7	8
      0x1C1C0C07,  //  0236  EQ	R7	R6	R7
      0x781E000B,  //  0237  JMPF	R7	#0244
      0x8C1C0906,  //  0238  GETMET	R7	R4	K6
      0x88240916,  //  0239  GETMBR	R9	R4	K22
      0xB82A2400,  //  023A  GETNGBL	R10	K18
      0x8C281526,  //  023B  GETMET	R10	R10	K38
      0x58300049,  //  023C  LDCONST	R12	K73
      0x50340200,  //  023D  LDBOOL	R13	1	0
      0x7C280600,  //  023E  CALL	R10	3
      0x9428154A,  //  023F  GETIDX	R10	R10	K74
      0x9428154B,  //  0240  GETIDX	R10	R10	K75
      0x7C1C0600,  //  0241  CALL	R7	3
      0x80040E00,  //  0242  RET	1	R7
      0x70020036,  //  0243  JMP		#027B
      0x541E0008,  //  0244  LDINT	R7	9
      0x1C1C0C07,  //  0245  EQ	R7	R6	R7
      0x781E0005,  //  0246  JMPF	R7	#024D
      0x8C1C0906,  //  0247  GETMET	R7	R4	K6
      0x8824090C,  //  0248  GETMBR	R9	R4	K12
      0x58280009,  //  0249  LDCONST	R10	K9
      0x7C1C0600,  //  024A  CALL	R7	3
      0x80040E00,  //  024B  RET	1	R7
      0x7002002D,  //  024C  JMP		#027B
      0x541E0009,  //  024D  LDINT	R7	10
      0x1C1C0C07,  //  024E  EQ	R7	R6	R7
      0x781E000B,  //  024F  JMPF	R7	#025C
      0x8C1C0906,  //  0250  GETMET	R7	R4	K6
      0x88240916,  //  0251  GETMBR	R9	R4	K22
      0xB82A2400,  //  0252  GETNGBL	R10	K18
      0x8C281526,  //  0253  GETMET	R10	R10	K38
      0x58300049,  //  0254  LDCONST	R12	K73
      0x50340200,  //  0255  LDBOOL	R13	1	0
      0x7C280600,  //  0256  CALL	R10	3
      0x9428154A,  //  0257  GETIDX	R10	R10	K74
      0x9428154C,  //  0258  GETIDX	R10	R10	K76
      0x7C1C0600,  //  0259  CALL	R7	3
      0x80040E00,  //  025A  RET	1	R7
      0x7002001E,  //  025B  JMP		#027B
      0x541E0011,  //  025C  LDINT	R7	18
      0x1C1C0C07,  //  025D  EQ	R7	R6	R7
      0x781E000B,  //  025E  JMPF	R7	#026B
      0x8C1C0906,  //  025F  GETMET	R7	R4	K6
      0x88240916,  //  0260  GETMBR	R9	R4	K22
      0xB82A2400,  //  0261  GETNGBL	R10	K18
      0x8C281525,  //  0262  GETMET	R10	R10	K37
      0x7C280200,  //  0263  CALL	R10	1
      0x8C28151B,  //  0264  GETMET	R10	R10	K27
      0x5830001C,  //  0265  LDCONST	R12	K28
      0x5834001D,  //  0266  LDCONST	R13	K29
      0x7C280600,  //  0267  CALL	R10	3
      0x7C1C0600,  //  0268  CALL	R7	3
      0x80040E00,  //  0269  RET	1	R7
      0x7002000F,  //  026A  JMP		#027B
      0x541E0012,  //  026B  LDINT	R7	19
      0x1C1C0C07,  //  026C  EQ	R7	R6	R7
      0x781E000C,  //  026D  JMPF	R7	#027B
      0x8C1C090A,  //  026E  GETMET	R7	R4	K10
      0x7C1C0200,  //  026F  CALL	R7	1
      0x8C200F0B,  //  0270  GETMET	R8	R7	K11
      0x58280005,  //  0271  LDCONST	R10	K5
      0x882C090C,  //  0272  GETMBR	R11	R4	K12
      0x5830000F,  //  0273  LDCONST	R12	K15
      0x7C200800,  //  0274  CALL	R8	4
      0x8C200F0B,  //  0275  GETMET	R8	R7	K11
      0x58280009,  //  0276  LDCONST	R10	K9
      0x882C090C,  //  0277  GETMBR	R11	R4	K12
      0x5830000F,  //  0278  LDCONST	R12	K15
      0x7C200800,  //  0279  CALL	R8	4
      0x80040E00,  //  027A  RET	1	R7
      0x700200CC,  //  027B  JMP		#0349
      0x541E003E,  //  027C  LDINT	R7	63
      0x1C1C0A07,  //  027D  EQ	R7	R5	R7
      0x781E0000,  //  027E  JMPF	R7	#0280
      0x700200C8,  //  027F  JMP		#0349
      0x541E0029,  //  0280  LDINT	R7	42
      0x1C1C0A07,  //  0281  EQ	R7	R5	R7
      0x781E001D,  //  0282  JMPF	R7	#02A1
      0x1C1C0D05,  //  0283  EQ	R7	R6	K5
      0x781E0003,  //  0284  JMPF	R7	#0289
      0x8C1C0911,  //  0285  GETMET	R7	R4	K17
      0x7C1C0200,  //  0286  CALL	R7	1
      0x80040E00,  //  0287  RET	1	R7
      0x70020016,  //  0288  JMP		#02A0
      0x1C1C0D09,  //  0289  EQ	R7	R6	K9
      0x781E0005,  //  028A  JMPF	R7	#0291
      0x8C1C0906,  //  028B  GETMET	R7	R4	K6
      0x88240910,  //  028C  GETMBR	R9	R4	K16
      0x58280005,  //  028D  LDCONST	R10	K5
      0x7C1C0600,  //  028E  CALL	R7	3
      0x80040E00,  //  028F  RET	1	R7
      0x7002000E,  //  0290  JMP		#02A0
      0x1C1C0D0D,  //  0291  EQ	R7	R6	K13
      0x781E0005,  //  0292  JMPF	R7	#0299
      0x8C1C0906,  //  0293  GETMET	R7	R4	K6
      0x8824090E,  //  0294  GETMBR	R9	R4	K14
      0x58280009,  //  0295  LDCONST	R10	K9
      0x7C1C0600,  //  0296  CALL	R7	3
      0x80040E00,  //  0297  RET	1	R7
      0x70020006,  //  0298  JMP		#02A0
      0x1C1C0D0F,  //  0299  EQ	R7	R6	K15
      0x781E0004,  //  029A  JMPF	R7	#02A0
      0x8C1C0906,  //  029B  GETMET	R7	R4	K6
      0x88240918,  //  029C  GETMBR	R9	R4	K24
      0x4C280000,  //  029D  LDNIL	R10
      0x7C1C0600,  //  029E  CALL	R7	3
      0x80040E00,  //  029F  RET	1	R7
      0x700200A7,  //  02A0  JMP		#0349
      0x541E002A,  //  02A1  LDINT	R7	43
      0x1C1C0A07,  //  02A2  EQ	R7	R5	R7
      0x781E0016,  //  02A3  JMPF	R7	#02BB
      0x1C1C0D05,  //  02A4  EQ	R7	R6	K5
      0x781E0007,  //  02A5  JMPF	R7	#02AE
      0x8C1C0906,  //  02A6  GETMET	R7	R4	K6
      0x88240916,  //  02A7  GETMBR	R9	R4	K22
      0xB82A2400,  //  02A8  GETNGBL	R10	K18
      0x8C28154D,  //  02A9  GETMET	R10	R10	K77
      0x7C280200,  //  02AA  CALL	R10	1
      0x7C1C0600,  //  02AB  CALL	R7	3
      0x80040E00,  //  02AC  RET	1	R7
      0x7002000B,  //  02AD  JMP		#02BA
      0x1C1C0D09,  //  02AE  EQ	R7	R6	K9
      0x781E0009,  //  02AF  JMPF	R7	#02BA
      0x8C1C0911,  //  02B0  GETMET	R7	R4	K17
      0x7C1C0200,  //  02B1  CALL	R7	1
      0x8C200F0B,  //  02B2  GETMET	R8	R7	K11
      0x4C280000,  //  02B3  LDNIL	R10
      0x882C0916,  //  02B4  GETMBR	R11	R4	K22
      0xB8322400,  //  02B5  GETNGBL	R12	K18
      0x8C30194D,  //  02B6  GETMET	R12	R12	K77
      0x7C300200,  //  02B7  CALL	R12	1
      0x7C200800,  //  02B8  CALL	R8	4
      0x80040E00,  //  02B9  RET	1	R7
      0x7002008D,  //  02BA  JMP		#0349
      0x541E002B,  //  02BB  LDINT	R7	44
      0x1C1C0A07,  //  02BC  EQ	R7	R5	R7
      0x781E001C,  //  02BD  JMPF	R7	#02DB
      0x1C1C0D05,  //  02BE  EQ	R7	R6	K5
      0x781E0005,  //  02BF  JMPF	R7	#02C6
      0x8C1C0906,  //  02C0  GETMET	R7	R4	K6
      0x8824090E,  //  02C1  GETMBR	R9	R4	K14
      0x58280009,  //  02C2  LDCONST	R10	K9
      0x7C1C0600,  //  02C3  CALL	R7	3
      0x80040E00,  //  02C4  RET	1	R7
      0x70020013,  //  02C5  JMP		#02DA
      0x1C1C0D09,  //  02C6  EQ	R7	R6	K9
      0x781E0005,  //  02C7  JMPF	R7	#02CE
      0x8C1C0906,  //  02C8  GETMET	R7	R4	K6
      0x8824090E,  //  02C9  GETMBR	R9	R4	K14
      0x542A0003,  //  02CA  LDINT	R10	4
      0x7C1C0600,  //  02CB  CALL	R7	3
      0x80040E00,  //  02CC  RET	1	R7
      0x7002000B,  //  02CD  JMP		#02DA
      0x1C1C0D0D,  //  02CE  EQ	R7	R6	K13
      0x781E0009,  //  02CF  JMPF	R7	#02DA
      0x8C1C0911,  //  02D0  GETMET	R7	R4	K17
      0x7C1C0200,  //  02D1  CALL	R7	1
      0x8C200F0B,  //  02D2  GETMET	R8	R7	K11
      0x4C280000,  //  02D3  LDNIL	R10
      0x8C2C0906,  //  02D4  GETMET	R11	R4	K6
      0x8834090E,  //  02D5  GETMBR	R13	R4	K14
      0x543A0003,  //  02D6  LDINT	R14	4
      0x7C2C0600,  //  02D7  CALL	R11	3
      0x7C200600,  //  02D8  CALL	R8	3
      0x80040E00,  //  02D9  RET	1	R7
      0x7002006D,  //  02DA  JMP		#0349
      0x541E0030,  //  02DB  LDINT	R7	49
      0x1C1C0A07,  //  02DC  EQ	R7	R5	R7
      0x781E0010,  //  02DD  JMPF	R7	#02EF
      0x1C1C0D0F,  //  02DE  EQ	R7	R6	K15
      0x781E0005,  //  02DF  JMPF	R7	#02E6
      0x8C1C0906,  //  02E0  GETMET	R7	R4	K6
      0x8824090E,  //  02E1  GETMBR	R9	R4	K14
      0x542A001D,  //  02E2  LDINT	R10	30
      0x7C1C0600,  //  02E3  CALL	R7	3
      0x80040E00,  //  02E4  RET	1	R7
      0x70020007,  //  02E5  JMP		#02EE
      0x541EFFFB,  //  02E6  LDINT	R7	65532
      0x1C1C0C07,  //  02E7  EQ	R7	R6	R7
      0x781E0004,  //  02E8  JMPF	R7	#02EE
      0x8C1C0906,  //  02E9  GETMET	R7	R4	K6
      0x8824092A,  //  02EA  GETMBR	R9	R4	K42
      0x58280005,  //  02EB  LDCONST	R10	K5
      0x7C1C0600,  //  02EC  CALL	R7	3
      0x80040E00,  //  02ED  RET	1	R7
      0x70020059,  //  02EE  JMP		#0349
      0x541E001C,  //  02EF  LDINT	R7	29
      0x1C1C0A07,  //  02F0  EQ	R7	R5	R7
      0x781E0053,  //  02F1  JMPF	R7	#0346
      0x1C1C0D05,  //  02F2  EQ	R7	R6	K5
      0x781E001C,  //  02F3  JMPF	R7	#0311
      0x8C1C0911,  //  02F4  GETMET	R7	R4	K17
      0x7C1C0200,  //  02F5  CALL	R7	1
      0x60200010,  //  02F6  GETGBL	R8	G16
      0x8824014E,  //  02F7  GETMBR	R9	R0	K78
      0x8C24134F,  //  02F8  GETMET	R9	R9	K79
      0x7C240200,  //  02F9  CALL	R9	1
      0x7C200200,  //  02FA  CALL	R8	1
      0xA802000F,  //  02FB  EXBLK	0	#030C
      0x5C241000,  //  02FC  MOVE	R9	R8
      0x7C240000,  //  02FD  CALL	R9	0
      0x8C280F15,  //  02FE  GETMET	R10	R7	K21
      0x7C280200,  //  02FF  CALL	R10	1
      0x8C2C150B,  //  0300  GETMET	R11	R10	K11
      0x58340005,  //  0301  LDCONST	R13	K5
      0x8838090C,  //  0302  GETMBR	R14	R4	K12
      0x5C3C1200,  //  0303  MOVE	R15	R9
      0x7C2C0800,  //  0304  CALL	R11	4
      0x8C2C150B,  //  0305  GETMET	R11	R10	K11
      0x58340009,  //  0306  LDCONST	R13	K9
      0x8838090C,  //  0307  GETMBR	R14	R4	K12
      0x883C014E,  //  0308  GETMBR	R15	R0	K78
      0x943C1E09,  //  0309  GETIDX	R15	R15	R9
      0x7C2C0800,  //  030A  CALL	R11	4
      0x7001FFEF,  //  030B  JMP		#02FC
      0x58200039,  //  030C  LDCONST	R8	K57
      0xAC200200,  //  030D  CATCH	R8	1	0
      0xB0080000,  //  030E  RAISE	2	R0	R0
      0x80040E00,  //  030F  RET	1	R7
      0x70020033,  //  0310  JMP		#0345
      0x1C1C0D09,  //  0311  EQ	R7	R6	K9
      0x781E0013,  //  0312  JMPF	R7	#0327
      0x8C1C0911,  //  0313  GETMET	R7	R4	K17
      0x7C1C0200,  //  0314  CALL	R7	1
      0x60200010,  //  0315  GETGBL	R8	G16
      0x8C240150,  //  0316  GETMET	R9	R0	K80
      0x7C240200,  //  0317  CALL	R9	1
      0x7C200200,  //  0318  CALL	R8	1
      0xA8020007,  //  0319  EXBLK	0	#0322
      0x5C241000,  //  031A  MOVE	R9	R8
      0x7C240000,  //  031B  CALL	R9	0
      0x8C280F0B,  //  031C  GETMET	R10	R7	K11
      0x4C300000,  //  031D  LDNIL	R12
      0x8834092A,  //  031E  GETMBR	R13	R4	K42
      0x5C381200,  //  031F  MOVE	R14	R9
      0x7C280800,  //  0320  CALL	R10	4
      0x7001FFF7,  //  0321  JMP		#031A
      0x58200039,  //  0322  LDCONST	R8	K57
      0xAC200200,  //  0323  CATCH	R8	1	0
      0xB0080000,  //  0324  RAISE	2	R0	R0
      0x80040E00,  //  0325  RET	1	R7
      0x7002001D,  //  0326  JMP		#0345
      0x1C1C0D0D,  //  0327  EQ	R7	R6	K13
      0x781E0003,  //  0328  JMPF	R7	#032D
      0x8C1C0911,  //  0329  GETMET	R7	R4	K17
      0x7C1C0200,  //  032A  CALL	R7	1
      0x80040E00,  //  032B  RET	1	R7
      0x70020017,  //  032C  JMP		#0345
      0x1C1C0D0F,  //  032D  EQ	R7	R6	K15
      0x781E0015,  //  032E  JMPF	R7	#0345
      0x881C0133,  //  032F  GETMBR	R7	R0	K51
      0x8C1C0F51,  //  0330  GETMET	R7	R7	K81
      0x50240200,  //  0331  LDBOOL	R9	1	0
      0x7C1C0400,  //  0332  CALL	R7	2
      0x8C200911,  //  0333  GETMET	R8	R4	K17
      0x7C200200,  //  0334  CALL	R8	1
      0x60240010,  //  0335  GETGBL	R9	G16
      0x5C280E00,  //  0336  MOVE	R10	R7
      0x7C240200,  //  0337  CALL	R9	1
      0xA8020007,  //  0338  EXBLK	0	#0341
      0x5C281200,  //  0339  MOVE	R10	R9
      0x7C280000,  //  033A  CALL	R10	0
      0x8C2C110B,  //  033B  GETMET	R11	R8	K11
      0x4C340000,  //  033C  LDNIL	R13
      0x8838090C,  //  033D  GETMBR	R14	R4	K12
      0x5C3C1400,  //  033E  MOVE	R15	R10
      0x7C2C0800,  //  033F  CALL	R11	4
      0x7001FFF7,  //  0340  JMP		#0339
      0x58240039,  //  0341  LDCONST	R9	K57
      0xAC240200,  //  0342  CATCH	R9	1	0
      0xB0080000,  //  0343  RAISE	2	R0	R0
      0x80041000,  //  0344  RET	1	R8
      0x70020002,  //  0345  JMP		#0349
      0xB81E0200,  //  0346  GETNGBL	R7	K1
      0x881C0F53,  //  0347  GETMBR	R7	R7	K83
      0x900AA407,  //  0348  SETMBR	R2	K82	R7
      0x80000000,  //  0349  RET	0
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
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Root_invoke_request,   /* name */
  be_nested_proto(
    28,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[79]) {     /* constants */
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
    /* K17  */  be_nested_str_weak(fabric_completed),
    /* K18  */  be_nested_str_weak(set_no_expiration),
    /* K19  */  be_nested_str_weak(save),
    /* K20  */  be_nested_str_weak(device),
    /* K21  */  be_nested_str_weak(start_commissioning_complete_deferred),
    /* K22  */  be_nested_str_weak(status),
    /* K23  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K24  */  be_nested_str_weak(B2),
    /* K25  */  be_nested_str_weak(DAC_Cert_FFF1_8000),
    /* K26  */  be_nested_str_weak(PAI_Cert_FFF1),
    /* K27  */  be_nested_str_weak(CD_FFF1_8000),
    /* K28  */  be_nested_str_weak(B1),
    /* K29  */  be_nested_str_weak(U4),
    /* K30  */  be_nested_str_weak(tasmota),
    /* K31  */  be_nested_str_weak(rtc),
    /* K32  */  be_nested_str_weak(utc),
    /* K33  */  be_nested_str_weak(encode),
    /* K34  */  be_nested_str_weak(get_ac),
    /* K35  */  be_nested_str_weak(log),
    /* K36  */  be_nested_str_weak(MTR_X3A_X20attestation_tbs_X3D),
    /* K37  */  be_nested_str_weak(tohex),
    /* K38  */  be_nested_str_weak(EC_P256),
    /* K39  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K40  */  be_nested_str_weak(DAC_Priv_FFF1_8000),
    /* K41  */  be_nested_str_weak(gen_CSR),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20nocsr_tbs_X3D),
    /* K43  */  be_nested_str_weak(set_ca),
    /* K44  */  be_nested_str_weak(MTR_X3A_X20received_X20ca_root_X3D),
    /* K45  */  be_nested_str_weak(SUCCESS),
    /* K46  */  be_nested_str_weak(get_ca),
    /* K47  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20AdNOC_X20without_X20CA),
    /* K48  */  be_nested_str_weak(set_noc),
    /* K49  */  be_nested_str_weak(set_ipk_epoch_key),
    /* K50  */  be_nested_str_weak(set_admin_subject_vendor),
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
    /* K65  */  be_nested_str_weak(persist_to_fabric),
    /* K66  */  be_nested_str_weak(fabric_candidate),
    /* K67  */  be_nested_str_weak(start_operational_discovery_deferred),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D_X2D),
    /* K69  */  be_nested_str_weak(MTR_X3A_X20fabric_X3D),
    /* K70  */  be_nested_str_weak(inspect),
    /* K71  */  be_nested_str_weak(_fabric),
    /* K72  */  be_nested_str_weak(set_fabric_label),
    /* K73  */  be_nested_str_weak(sessions),
    /* K74  */  be_nested_str_weak(sessions_active),
    /* K75  */  be_nested_str_weak(MTR_X3A_X20removing_X20fabric_X20),
    /* K76  */  be_nested_str_weak(get_fabric_id),
    /* K77  */  be_nested_str_weak(remove_session),
    /* K78  */  be_nested_str_weak(save_fabrics),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[532]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x5422002F,  //  0005  LDINT	R8	48
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220054,  //  0007  JMPF	R8	#005D
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
      0x70020039,  //  0021  JMP		#005C
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
      0x7002001C,  //  003E  JMP		#005C
      0x54220003,  //  003F  LDINT	R8	4
      0x1C200E08,  //  0040  EQ	R8	R7	R8
      0x78220019,  //  0041  JMPF	R8	#005C
      0x90061105,  //  0042  SETMBR	R1	K8	K5
      0x8C200311,  //  0043  GETMET	R8	R1	K17
      0x7C200200,  //  0044  CALL	R8	1
      0x8C200312,  //  0045  GETMET	R8	R1	K18
      0x7C200200,  //  0046  CALL	R8	1
      0x8C200313,  //  0047  GETMET	R8	R1	K19
      0x7C200200,  //  0048  CALL	R8	1
      0x8C200B09,  //  0049  GETMET	R8	R5	K9
      0x7C200200,  //  004A  CALL	R8	1
      0x8C24110A,  //  004B  GETMET	R9	R8	K10
      0x582C0005,  //  004C  LDCONST	R11	K5
      0x88300B0B,  //  004D  GETMBR	R12	R5	K11
      0x58340005,  //  004E  LDCONST	R13	K5
      0x7C240800,  //  004F  CALL	R9	4
      0x8C24110A,  //  0050  GETMET	R9	R8	K10
      0x582C0007,  //  0051  LDCONST	R11	K7
      0x88300B0C,  //  0052  GETMBR	R12	R5	K12
      0x5834000D,  //  0053  LDCONST	R13	K13
      0x7C240800,  //  0054  CALL	R9	4
      0x54260004,  //  0055  LDINT	R9	5
      0x900E0809,  //  0056  SETMBR	R3	K4	R9
      0x88240114,  //  0057  GETMBR	R9	R0	K20
      0x8C241315,  //  0058  GETMET	R9	R9	K21
      0x5C2C0200,  //  0059  MOVE	R11	R1
      0x7C240400,  //  005A  CALL	R9	2
      0x80041000,  //  005B  RET	1	R8
      0x700201B5,  //  005C  JMP		#0213
      0x5422003D,  //  005D  LDINT	R8	62
      0x1C200C08,  //  005E  EQ	R8	R6	R8
      0x782201AB,  //  005F  JMPF	R8	#020C
      0x1C200F0E,  //  0060  EQ	R8	R7	K14
      0x7822001D,  //  0061  JMPF	R8	#0080
      0x8C200506,  //  0062  GETMET	R8	R2	K6
      0x58280005,  //  0063  LDCONST	R10	K5
      0x7C200400,  //  0064  CALL	R8	2
      0x20241107,  //  0065  NE	R9	R8	K7
      0x78260006,  //  0066  JMPF	R9	#006E
      0x2024110E,  //  0067  NE	R9	R8	K14
      0x78260004,  //  0068  JMPF	R9	#006E
      0xB8260200,  //  0069  GETNGBL	R9	K1
      0x88241317,  //  006A  GETMBR	R9	R9	K23
      0x900E2C09,  //  006B  SETMBR	R3	K22	R9
      0x4C240000,  //  006C  LDNIL	R9
      0x80041200,  //  006D  RET	1	R9
      0x8C240B09,  //  006E  GETMET	R9	R5	K9
      0x7C240200,  //  006F  CALL	R9	1
      0x8C28130A,  //  0070  GETMET	R10	R9	K10
      0x58300005,  //  0071  LDCONST	R12	K5
      0x88340B18,  //  0072  GETMBR	R13	R5	K24
      0x1C381107,  //  0073  EQ	R14	R8	K7
      0x783A0003,  //  0074  JMPF	R14	#0079
      0xB83A0200,  //  0075  GETNGBL	R14	K1
      0x8C381D19,  //  0076  GETMET	R14	R14	K25
      0x7C380200,  //  0077  CALL	R14	1
      0x70020002,  //  0078  JMP		#007C
      0xB83A0200,  //  0079  GETNGBL	R14	K1
      0x8C381D1A,  //  007A  GETMET	R14	R14	K26
      0x7C380200,  //  007B  CALL	R14	1
      0x7C280800,  //  007C  CALL	R10	4
      0x900E0910,  //  007D  SETMBR	R3	K4	K16
      0x80041200,  //  007E  RET	1	R9
      0x7002018A,  //  007F  JMP		#020B
      0x1C200F05,  //  0080  EQ	R8	R7	K5
      0x78220044,  //  0081  JMPF	R8	#00C7
      0x8C200506,  //  0082  GETMET	R8	R2	K6
      0x58280005,  //  0083  LDCONST	R10	K5
      0x7C200400,  //  0084  CALL	R8	2
      0x6024000C,  //  0085  GETGBL	R9	G12
      0x5C281000,  //  0086  MOVE	R10	R8
      0x7C240200,  //  0087  CALL	R9	1
      0x542A001F,  //  0088  LDINT	R10	32
      0x2024120A,  //  0089  NE	R9	R9	R10
      0x78260001,  //  008A  JMPF	R9	#008D
      0x4C240000,  //  008B  LDNIL	R9
      0x80041200,  //  008C  RET	1	R9
      0x900E0907,  //  008D  SETMBR	R3	K4	K7
      0x8C240B09,  //  008E  GETMET	R9	R5	K9
      0x7C240200,  //  008F  CALL	R9	1
      0x8C28130A,  //  0090  GETMET	R10	R9	K10
      0x58300007,  //  0091  LDCONST	R12	K7
      0x88340B18,  //  0092  GETMBR	R13	R5	K24
      0xB83A0200,  //  0093  GETNGBL	R14	K1
      0x8C381D1B,  //  0094  GETMET	R14	R14	K27
      0x7C380200,  //  0095  CALL	R14	1
      0x7C280800,  //  0096  CALL	R10	4
      0x8C28130A,  //  0097  GETMET	R10	R9	K10
      0x5830000E,  //  0098  LDCONST	R12	K14
      0x88340B1C,  //  0099  GETMBR	R13	R5	K28
      0x5C381000,  //  009A  MOVE	R14	R8
      0x7C280800,  //  009B  CALL	R10	4
      0x8C28130A,  //  009C  GETMET	R10	R9	K10
      0x58300010,  //  009D  LDCONST	R12	K16
      0x88340B1D,  //  009E  GETMBR	R13	R5	K29
      0xB83A3C00,  //  009F  GETNGBL	R14	K30
      0x8C381D1F,  //  00A0  GETMET	R14	R14	K31
      0x7C380200,  //  00A1  CALL	R14	1
      0x94381D20,  //  00A2  GETIDX	R14	R14	K32
      0x7C280800,  //  00A3  CALL	R10	4
      0x8C281321,  //  00A4  GETMET	R10	R9	K33
      0x7C280200,  //  00A5  CALL	R10	1
      0x8C2C0322,  //  00A6  GETMET	R11	R1	K34
      0x7C2C0200,  //  00A7  CALL	R11	1
      0x0030140B,  //  00A8  ADD	R12	R10	R11
      0xB8363C00,  //  00A9  GETNGBL	R13	K30
      0x8C341B23,  //  00AA  GETMET	R13	R13	K35
      0x8C3C1925,  //  00AB  GETMET	R15	R12	K37
      0x7C3C0200,  //  00AC  CALL	R15	1
      0x003E480F,  //  00AD  ADD	R15	K36	R15
      0x58400010,  //  00AE  LDCONST	R16	K16
      0x7C340600,  //  00AF  CALL	R13	3
      0x8C340926,  //  00B0  GETMET	R13	R4	K38
      0x7C340200,  //  00B1  CALL	R13	1
      0x8C341B27,  //  00B2  GETMET	R13	R13	K39
      0xB83E0200,  //  00B3  GETNGBL	R15	K1
      0x8C3C1F28,  //  00B4  GETMET	R15	R15	K40
      0x7C3C0200,  //  00B5  CALL	R15	1
      0x5C401800,  //  00B6  MOVE	R16	R12
      0x7C340600,  //  00B7  CALL	R13	3
      0x8C380B09,  //  00B8  GETMET	R14	R5	K9
      0x7C380200,  //  00B9  CALL	R14	1
      0x8C3C1D0A,  //  00BA  GETMET	R15	R14	K10
      0x58440005,  //  00BB  LDCONST	R17	K5
      0x88480B18,  //  00BC  GETMBR	R18	R5	K24
      0x5C4C1400,  //  00BD  MOVE	R19	R10
      0x7C3C0800,  //  00BE  CALL	R15	4
      0x8C3C1D0A,  //  00BF  GETMET	R15	R14	K10
      0x58440007,  //  00C0  LDCONST	R17	K7
      0x88480B1C,  //  00C1  GETMBR	R18	R5	K28
      0x5C4C1A00,  //  00C2  MOVE	R19	R13
      0x7C3C0800,  //  00C3  CALL	R15	4
      0x900E0907,  //  00C4  SETMBR	R3	K4	K7
      0x80041C00,  //  00C5  RET	1	R14
      0x70020143,  //  00C6  JMP		#020B
      0x54220003,  //  00C7  LDINT	R8	4
      0x1C200E08,  //  00C8  EQ	R8	R7	R8
      0x78220040,  //  00C9  JMPF	R8	#010B
      0x8C200506,  //  00CA  GETMET	R8	R2	K6
      0x58280005,  //  00CB  LDCONST	R10	K5
      0x7C200400,  //  00CC  CALL	R8	2
      0x6024000C,  //  00CD  GETGBL	R9	G12
      0x5C281000,  //  00CE  MOVE	R10	R8
      0x7C240200,  //  00CF  CALL	R9	1
      0x542A001F,  //  00D0  LDINT	R10	32
      0x2024120A,  //  00D1  NE	R9	R9	R10
      0x78260001,  //  00D2  JMPF	R9	#00D5
      0x4C240000,  //  00D3  LDNIL	R9
      0x80041200,  //  00D4  RET	1	R9
      0x8C240506,  //  00D5  GETMET	R9	R2	K6
      0x582C0007,  //  00D6  LDCONST	R11	K7
      0x50300000,  //  00D7  LDBOOL	R12	0	0
      0x7C240600,  //  00D8  CALL	R9	3
      0x8C280329,  //  00D9  GETMET	R10	R1	K41
      0x7C280200,  //  00DA  CALL	R10	1
      0x8C2C0B09,  //  00DB  GETMET	R11	R5	K9
      0x7C2C0200,  //  00DC  CALL	R11	1
      0x8C30170A,  //  00DD  GETMET	R12	R11	K10
      0x58380007,  //  00DE  LDCONST	R14	K7
      0x883C0B18,  //  00DF  GETMBR	R15	R5	K24
      0x5C401400,  //  00E0  MOVE	R16	R10
      0x7C300800,  //  00E1  CALL	R12	4
      0x8C30170A,  //  00E2  GETMET	R12	R11	K10
      0x5838000E,  //  00E3  LDCONST	R14	K14
      0x883C0B1C,  //  00E4  GETMBR	R15	R5	K28
      0x5C401000,  //  00E5  MOVE	R16	R8
      0x7C300800,  //  00E6  CALL	R12	4
      0x8C301721,  //  00E7  GETMET	R12	R11	K33
      0x7C300200,  //  00E8  CALL	R12	1
      0x8C340322,  //  00E9  GETMET	R13	R1	K34
      0x7C340200,  //  00EA  CALL	R13	1
      0x0034180D,  //  00EB  ADD	R13	R12	R13
      0xB83A3C00,  //  00EC  GETNGBL	R14	K30
      0x8C381D23,  //  00ED  GETMET	R14	R14	K35
      0x8C401B25,  //  00EE  GETMET	R16	R13	K37
      0x7C400200,  //  00EF  CALL	R16	1
      0x00425410,  //  00F0  ADD	R16	K42	R16
      0x58440010,  //  00F1  LDCONST	R17	K16
      0x7C380600,  //  00F2  CALL	R14	3
      0x8C380926,  //  00F3  GETMET	R14	R4	K38
      0x7C380200,  //  00F4  CALL	R14	1
      0x8C381D27,  //  00F5  GETMET	R14	R14	K39
      0xB8420200,  //  00F6  GETNGBL	R16	K1
      0x8C402128,  //  00F7  GETMET	R16	R16	K40
      0x7C400200,  //  00F8  CALL	R16	1
      0x5C441A00,  //  00F9  MOVE	R17	R13
      0x7C380600,  //  00FA  CALL	R14	3
      0x8C3C0B09,  //  00FB  GETMET	R15	R5	K9
      0x7C3C0200,  //  00FC  CALL	R15	1
      0x8C401F0A,  //  00FD  GETMET	R16	R15	K10
      0x58480005,  //  00FE  LDCONST	R18	K5
      0x884C0B18,  //  00FF  GETMBR	R19	R5	K24
      0x5C501800,  //  0100  MOVE	R20	R12
      0x7C400800,  //  0101  CALL	R16	4
      0x8C401F0A,  //  0102  GETMET	R16	R15	K10
      0x58480007,  //  0103  LDCONST	R18	K7
      0x884C0B1C,  //  0104  GETMBR	R19	R5	K28
      0x5C501C00,  //  0105  MOVE	R20	R14
      0x7C400800,  //  0106  CALL	R16	4
      0x54420004,  //  0107  LDINT	R16	5
      0x900E0810,  //  0108  SETMBR	R3	K4	R16
      0x80041E00,  //  0109  RET	1	R15
      0x700200FF,  //  010A  JMP		#020B
      0x5422000A,  //  010B  LDINT	R8	11
      0x1C200E08,  //  010C  EQ	R8	R7	R8
      0x78220012,  //  010D  JMPF	R8	#0121
      0x8C200506,  //  010E  GETMET	R8	R2	K6
      0x58280005,  //  010F  LDCONST	R10	K5
      0x7C200400,  //  0110  CALL	R8	2
      0x8C24032B,  //  0111  GETMET	R9	R1	K43
      0x5C2C1000,  //  0112  MOVE	R11	R8
      0x7C240400,  //  0113  CALL	R9	2
      0xB8263C00,  //  0114  GETNGBL	R9	K30
      0x8C241323,  //  0115  GETMET	R9	R9	K35
      0x8C2C1125,  //  0116  GETMET	R11	R8	K37
      0x7C2C0200,  //  0117  CALL	R11	1
      0x002E580B,  //  0118  ADD	R11	K44	R11
      0x58300010,  //  0119  LDCONST	R12	K16
      0x7C240600,  //  011A  CALL	R9	3
      0xB8260200,  //  011B  GETNGBL	R9	K1
      0x8824132D,  //  011C  GETMBR	R9	R9	K45
      0x900E2C09,  //  011D  SETMBR	R3	K22	R9
      0x4C240000,  //  011E  LDNIL	R9
      0x80041200,  //  011F  RET	1	R9
      0x700200E9,  //  0120  JMP		#020B
      0x54220005,  //  0121  LDINT	R8	6
      0x1C200E08,  //  0122  EQ	R8	R7	R8
      0x782200AA,  //  0123  JMPF	R8	#01CF
      0x8C200506,  //  0124  GETMET	R8	R2	K6
      0x58280005,  //  0125  LDCONST	R10	K5
      0x7C200400,  //  0126  CALL	R8	2
      0x8C240506,  //  0127  GETMET	R9	R2	K6
      0x582C0007,  //  0128  LDCONST	R11	K7
      0x7C240400,  //  0129  CALL	R9	2
      0x8C280506,  //  012A  GETMET	R10	R2	K6
      0x5830000E,  //  012B  LDCONST	R12	K14
      0x7C280400,  //  012C  CALL	R10	2
      0x8C2C0506,  //  012D  GETMET	R11	R2	K6
      0x58340010,  //  012E  LDCONST	R13	K16
      0x7C2C0400,  //  012F  CALL	R11	2
      0x8C300506,  //  0130  GETMET	R12	R2	K6
      0x543A0003,  //  0131  LDINT	R14	4
      0x7C300400,  //  0132  CALL	R12	2
      0x8C34032E,  //  0133  GETMET	R13	R1	K46
      0x7C340200,  //  0134  CALL	R13	1
      0x4C380000,  //  0135  LDNIL	R14
      0x1C341A0E,  //  0136  EQ	R13	R13	R14
      0x78360006,  //  0137  JMPF	R13	#013F
      0xB8363C00,  //  0138  GETNGBL	R13	K30
      0x8C341B23,  //  0139  GETMET	R13	R13	K35
      0x583C002F,  //  013A  LDCONST	R15	K47
      0x5840000E,  //  013B  LDCONST	R16	K14
      0x7C340600,  //  013C  CALL	R13	3
      0x4C340000,  //  013D  LDNIL	R13
      0x80041A00,  //  013E  RET	1	R13
      0x8C340330,  //  013F  GETMET	R13	R1	K48
      0x5C3C1000,  //  0140  MOVE	R15	R8
      0x5C401200,  //  0141  MOVE	R16	R9
      0x7C340600,  //  0142  CALL	R13	3
      0x8C340331,  //  0143  GETMET	R13	R1	K49
      0x5C3C1400,  //  0144  MOVE	R15	R10
      0x7C340400,  //  0145  CALL	R13	2
      0x8C340332,  //  0146  GETMET	R13	R1	K50
      0x5C3C1600,  //  0147  MOVE	R15	R11
      0x5C401800,  //  0148  MOVE	R16	R12
      0x7C340600,  //  0149  CALL	R13	3
      0xB8360200,  //  014A  GETNGBL	R13	K1
      0x88341B02,  //  014B  GETMBR	R13	R13	K2
      0x8C341B33,  //  014C  GETMET	R13	R13	K51
      0x5C3C1000,  //  014D  MOVE	R15	R8
      0x7C340400,  //  014E  CALL	R13	2
      0x8C381B34,  //  014F  GETMET	R14	R13	K52
      0x54420005,  //  0150  LDINT	R16	6
      0x7C380400,  //  0151  CALL	R14	2
      0x8C3C1D06,  //  0152  GETMET	R15	R14	K6
      0x54460014,  //  0153  LDINT	R17	21
      0x7C3C0400,  //  0154  CALL	R15	2
      0x8C401D06,  //  0155  GETMET	R16	R14	K6
      0x544A0010,  //  0156  LDINT	R18	17
      0x7C400400,  //  0157  CALL	R16	2
      0x5C441E00,  //  0158  MOVE	R17	R15
      0x78460001,  //  0159  JMPF	R17	#015C
      0x5C442000,  //  015A  MOVE	R17	R16
      0x74460006,  //  015B  JMPT	R17	#0163
      0xB8463C00,  //  015C  GETNGBL	R17	K30
      0x8C442323,  //  015D  GETMET	R17	R17	K35
      0x584C0035,  //  015E  LDCONST	R19	K53
      0x5850000E,  //  015F  LDCONST	R20	K14
      0x7C440600,  //  0160  CALL	R17	3
      0x50440000,  //  0161  LDBOOL	R17	0	0
      0x80042200,  //  0162  RET	1	R17
      0x60440004,  //  0163  GETGBL	R17	G4
      0x5C481E00,  //  0164  MOVE	R18	R15
      0x7C440200,  //  0165  CALL	R17	1
      0x1C442336,  //  0166  EQ	R17	R17	K54
      0x78460006,  //  0167  JMPF	R17	#016F
      0xB8466E00,  //  0168  GETNGBL	R17	K55
      0x5C481E00,  //  0169  MOVE	R18	R15
      0x7C440200,  //  016A  CALL	R17	1
      0x8C442338,  //  016B  GETMET	R17	R17	K56
      0x7C440200,  //  016C  CALL	R17	1
      0x5C3C2200,  //  016D  MOVE	R15	R17
      0x70020002,  //  016E  JMP		#0172
      0x8C441F38,  //  016F  GETMET	R17	R15	K56
      0x7C440200,  //  0170  CALL	R17	1
      0x5C3C2200,  //  0171  MOVE	R15	R17
      0x60440004,  //  0172  GETGBL	R17	G4
      0x5C482000,  //  0173  MOVE	R18	R16
      0x7C440200,  //  0174  CALL	R17	1
      0x1C442336,  //  0175  EQ	R17	R17	K54
      0x78460006,  //  0176  JMPF	R17	#017E
      0xB8466E00,  //  0177  GETNGBL	R17	K55
      0x5C482000,  //  0178  MOVE	R18	R16
      0x7C440200,  //  0179  CALL	R17	1
      0x8C442338,  //  017A  GETMET	R17	R17	K56
      0x7C440200,  //  017B  CALL	R17	1
      0x5C402200,  //  017C  MOVE	R16	R17
      0x70020002,  //  017D  JMP		#0181
      0x8C442138,  //  017E  GETMET	R17	R16	K56
      0x7C440200,  //  017F  CALL	R17	1
      0x5C402200,  //  0180  MOVE	R16	R17
      0xB8460200,  //  0181  GETNGBL	R17	K1
      0x88442302,  //  0182  GETMBR	R17	R17	K2
      0x8C442333,  //  0183  GETMET	R17	R17	K51
      0x8C4C032E,  //  0184  GETMET	R19	R1	K46
      0x7C4C0200,  //  0185  CALL	R19	1
      0x7C440400,  //  0186  CALL	R17	2
      0x8C442306,  //  0187  GETMET	R17	R17	K6
      0x544E0008,  //  0188  LDINT	R19	9
      0x7C440400,  //  0189  CALL	R17	2
      0x404A0F39,  //  018A  CONNECT	R18	K7	K57
      0x94442212,  //  018B  GETIDX	R17	R17	R18
      0x604C0015,  //  018C  GETGBL	R19	G21
      0x7C4C0000,  //  018D  CALL	R19	0
      0x8C4C273A,  //  018E  GETMET	R19	R19	K58
      0x5854003B,  //  018F  LDCONST	R21	K59
      0x7C4C0400,  //  0190  CALL	R19	2
      0x5C482600,  //  0191  MOVE	R18	R19
      0x8C4C093C,  //  0192  GETMET	R19	R4	K60
      0x7C4C0200,  //  0193  CALL	R19	1
      0x8C501F3D,  //  0194  GETMET	R20	R15	K61
      0x7C500200,  //  0195  CALL	R20	1
      0x8C50293E,  //  0196  GETMET	R20	R20	K62
      0x7C500200,  //  0197  CALL	R20	1
      0x8C54273F,  //  0198  GETMET	R21	R19	K63
      0x5C5C2200,  //  0199  MOVE	R23	R17
      0x5C602800,  //  019A  MOVE	R24	R20
      0x5C642400,  //  019B  MOVE	R25	R18
      0x546A0007,  //  019C  LDINT	R26	8
      0x7C540A00,  //  019D  CALL	R21	5
      0x8C580340,  //  019E  GETMET	R22	R1	K64
      0x5C601E00,  //  019F  MOVE	R24	R15
      0x5C642000,  //  01A0  MOVE	R25	R16
      0x5C682A00,  //  01A1  MOVE	R26	R21
      0x7C580800,  //  01A2  CALL	R22	4
      0x8C580341,  //  01A3  GETMET	R22	R1	K65
      0x7C580200,  //  01A4  CALL	R22	1
      0x8C580342,  //  01A5  GETMET	R22	R1	K66
      0x7C580200,  //  01A6  CALL	R22	1
      0x88580114,  //  01A7  GETMBR	R22	R0	K20
      0x8C582D43,  //  01A8  GETMET	R22	R22	K67
      0x5C600200,  //  01A9  MOVE	R24	R1
      0x7C580400,  //  01AA  CALL	R22	2
      0xB85A3C00,  //  01AB  GETNGBL	R22	K30
      0x8C582D23,  //  01AC  GETMET	R22	R22	K35
      0x58600044,  //  01AD  LDCONST	R24	K68
      0x58640010,  //  01AE  LDCONST	R25	K16
      0x7C580600,  //  01AF  CALL	R22	3
      0xB85A3C00,  //  01B0  GETNGBL	R22	K30
      0x8C582D23,  //  01B1  GETMET	R22	R22	K35
      0xB8620200,  //  01B2  GETNGBL	R24	K1
      0x8C603146,  //  01B3  GETMET	R24	R24	K70
      0x88680347,  //  01B4  GETMBR	R26	R1	K71
      0x7C600400,  //  01B5  CALL	R24	2
      0x00628A18,  //  01B6  ADD	R24	K69	R24
      0x58640010,  //  01B7  LDCONST	R25	K16
      0x7C580600,  //  01B8  CALL	R22	3
      0xB85A3C00,  //  01B9  GETNGBL	R22	K30
      0x8C582D23,  //  01BA  GETMET	R22	R22	K35
      0x58600044,  //  01BB  LDCONST	R24	K68
      0x58640010,  //  01BC  LDCONST	R25	K16
      0x7C580600,  //  01BD  CALL	R22	3
      0x8C580B09,  //  01BE  GETMET	R22	R5	K9
      0x7C580200,  //  01BF  CALL	R22	1
      0x8C5C2D0A,  //  01C0  GETMET	R23	R22	K10
      0x58640005,  //  01C1  LDCONST	R25	K5
      0x88680B0B,  //  01C2  GETMBR	R26	R5	K11
      0xB86E0200,  //  01C3  GETNGBL	R27	K1
      0x886C372D,  //  01C4  GETMBR	R27	R27	K45
      0x7C5C0800,  //  01C5  CALL	R23	4
      0x8C5C2D0A,  //  01C6  GETMET	R23	R22	K10
      0x58640007,  //  01C7  LDCONST	R25	K7
      0x88680B0B,  //  01C8  GETMBR	R26	R5	K11
      0x586C0007,  //  01C9  LDCONST	R27	K7
      0x7C5C0800,  //  01CA  CALL	R23	4
      0x545E0007,  //  01CB  LDINT	R23	8
      0x900E0817,  //  01CC  SETMBR	R3	K4	R23
      0x80042C00,  //  01CD  RET	1	R22
      0x7002003B,  //  01CE  JMP		#020B
      0x54220008,  //  01CF  LDINT	R8	9
      0x1C200E08,  //  01D0  EQ	R8	R7	R8
      0x7822000B,  //  01D1  JMPF	R8	#01DE
      0x8C200506,  //  01D2  GETMET	R8	R2	K6
      0x58280005,  //  01D3  LDCONST	R10	K5
      0x7C200400,  //  01D4  CALL	R8	2
      0x8C240348,  //  01D5  GETMET	R9	R1	K72
      0x5C2C1000,  //  01D6  MOVE	R11	R8
      0x7C240400,  //  01D7  CALL	R9	2
      0xB8260200,  //  01D8  GETNGBL	R9	K1
      0x8824132D,  //  01D9  GETMBR	R9	R9	K45
      0x900E2C09,  //  01DA  SETMBR	R3	K22	R9
      0x4C240000,  //  01DB  LDNIL	R9
      0x80041200,  //  01DC  RET	1	R9
      0x7002002C,  //  01DD  JMP		#020B
      0x54220009,  //  01DE  LDINT	R8	10
      0x1C200E08,  //  01DF  EQ	R8	R7	R8
      0x78220029,  //  01E0  JMPF	R8	#020B
      0x8C200506,  //  01E1  GETMET	R8	R2	K6
      0x58280005,  //  01E2  LDCONST	R10	K5
      0x7C200400,  //  01E3  CALL	R8	2
      0x88240114,  //  01E4  GETMBR	R9	R0	K20
      0x88241349,  //  01E5  GETMBR	R9	R9	K73
      0x8C24134A,  //  01E6  GETMET	R9	R9	K74
      0x7C240200,  //  01E7  CALL	R9	1
      0x28281107,  //  01E8  GE	R10	R8	K7
      0x782A001B,  //  01E9  JMPF	R10	#0206
      0x6028000C,  //  01EA  GETGBL	R10	G12
      0x5C2C1200,  //  01EB  MOVE	R11	R9
      0x7C280200,  //  01EC  CALL	R10	1
      0x1828100A,  //  01ED  LE	R10	R8	R10
      0x782A0016,  //  01EE  JMPF	R10	#0206
      0x04281107,  //  01EF  SUB	R10	R8	K7
      0x9428120A,  //  01F0  GETIDX	R10	R9	R10
      0xB82E3C00,  //  01F1  GETNGBL	R11	K30
      0x8C2C1723,  //  01F2  GETMET	R11	R11	K35
      0x8C34034C,  //  01F3  GETMET	R13	R1	K76
      0x7C340200,  //  01F4  CALL	R13	1
      0x8C341B3D,  //  01F5  GETMET	R13	R13	K61
      0x7C340200,  //  01F6  CALL	R13	1
      0x8C341B3E,  //  01F7  GETMET	R13	R13	K62
      0x7C340200,  //  01F8  CALL	R13	1
      0x8C341B25,  //  01F9  GETMET	R13	R13	K37
      0x7C340200,  //  01FA  CALL	R13	1
      0x0036960D,  //  01FB  ADD	R13	K75	R13
      0x7C2C0400,  //  01FC  CALL	R11	2
      0x882C0114,  //  01FD  GETMBR	R11	R0	K20
      0x882C1749,  //  01FE  GETMBR	R11	R11	K73
      0x8C2C174D,  //  01FF  GETMET	R11	R11	K77
      0x7C2C0200,  //  0200  CALL	R11	1
      0x882C0114,  //  0201  GETMBR	R11	R0	K20
      0x882C1749,  //  0202  GETMBR	R11	R11	K73
      0x8C2C174E,  //  0203  GETMET	R11	R11	K78
      0x7C2C0200,  //  0204  CALL	R11	1
      0x7001FFFF,  //  0205  JMP		#0206
      0xB82A0200,  //  0206  GETNGBL	R10	K1
      0x8828152D,  //  0207  GETMBR	R10	R10	K45
      0x900E2C0A,  //  0208  SETMBR	R3	K22	R10
      0x4C280000,  //  0209  LDNIL	R10
      0x80041400,  //  020A  RET	1	R10
      0x70020006,  //  020B  JMP		#0213
      0x54220029,  //  020C  LDINT	R8	42
      0x1C200C08,  //  020D  EQ	R8	R6	R8
      0x78220003,  //  020E  JMPF	R8	#0213
      0x1C200F05,  //  020F  EQ	R8	R7	K5
      0x78220001,  //  0210  JMPF	R8	#0213
      0x50200200,  //  0211  LDBOOL	R8	1	0
      0x80041000,  //  0212  RET	1	R8
      0x80000000,  //  0213  RET	0
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
        { be_const_key_weak(read_attribute, 1), be_const_closure(Matter_Plugin_Root_read_attribute_closure) },
        { be_const_key_weak(invoke_request, 2), be_const_closure(Matter_Plugin_Root_invoke_request_closure) },
        { be_const_key_weak(CLUSTERS, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
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
        be_const_list( *     be_nested_list(12,
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
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(22, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(write_attribute, -1), be_const_closure(Matter_Plugin_Root_write_attribute_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Root_init_closure) },
        { be_const_key_weak(ENDPOINTS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
    }))    ) } )) },
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
