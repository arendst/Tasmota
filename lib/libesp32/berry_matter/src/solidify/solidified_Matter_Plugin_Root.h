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
    ( &(const bvalue[82]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U8),
    /* K8   */  be_nested_str_weak(__breadcrumb),
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
    /* K55  */  be_nested_str_weak(noc),
    /* K56  */  be_nested_str_weak(icac),
    /* K57  */  be_nested_str_weak(stop_iteration),
    /* K58  */  be_nested_str_weak(parse),
    /* K59  */  be_nested_str_weak(get_ca),
    /* K60  */  be_nested_str_weak(findsubval),
    /* K61  */  be_nested_str_weak(admin_vendor),
    /* K62  */  be_nested_str_weak(fabric),
    /* K63  */  be_nested_str_weak(deviceid),
    /* K64  */  be_nested_str_weak(fabric_label),
    /* K65  */  be_nested_str_weak(Tasmota),
    /* K66  */  be_nested_str_weak(vendorid),
    /* K67  */  be_nested_str_weak(DeviceName),
    /* K68  */  be_nested_str_weak(FriendlyName),
    /* K69  */  be_nested_str_weak(FriendlyName1),
    /* K70  */  be_nested_str_weak(XX),
    /* K71  */  be_nested_str_weak(Status_X202),
    /* K72  */  be_nested_str_weak(StatusFWR),
    /* K73  */  be_nested_str_weak(Hardware),
    /* K74  */  be_nested_str_weak(Version),
    /* K75  */  be_nested_str_weak(locale),
    /* K76  */  be_nested_str_weak(TYPES),
    /* K77  */  be_nested_str_weak(keys),
    /* K78  */  be_nested_str_weak(get_cluster_list),
    /* K79  */  be_nested_str_weak(get_active_endpoints),
    /* K80  */  be_nested_str_weak(status),
    /* K81  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[828]) {  /* code */
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
      0x70020301,  //  0038  JMP		#033B
      0x541E0031,  //  0039  LDINT	R7	50
      0x1C1C0A07,  //  003A  EQ	R7	R5	R7
      0x781E0000,  //  003B  JMPF	R7	#003D
      0x700202FD,  //  003C  JMP		#033B
      0x541E0032,  //  003D  LDINT	R7	51
      0x1C1C0A07,  //  003E  EQ	R7	R5	R7
      0x781E00DA,  //  003F  JMPF	R7	#011B
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
      0x70020021,  //  00F7  JMP		#011A
      0x1C1C0D09,  //  00F8  EQ	R7	R6	K9
      0x781E000A,  //  00F9  JMPF	R7	#0105
      0x8C1C0906,  //  00FA  GETMET	R7	R4	K6
      0x8824090C,  //  00FB  GETMBR	R9	R4	K12
      0xB82A2400,  //  00FC  GETNGBL	R10	K18
      0x8C281526,  //  00FD  GETMET	R10	R10	K38
      0x58300027,  //  00FE  LDCONST	R12	K39
      0x7C280400,  //  00FF  CALL	R10	2
      0x94281528,  //  0100  GETIDX	R10	R10	K40
      0x94281529,  //  0101  GETIDX	R10	R10	K41
      0x7C1C0600,  //  0102  CALL	R7	3
      0x80040E00,  //  0103  RET	1	R7
      0x70020014,  //  0104  JMP		#011A
      0x1C1C0D0D,  //  0105  EQ	R7	R6	K13
      0x781E000A,  //  0106  JMPF	R7	#0112
      0x8C1C0906,  //  0107  GETMET	R7	R4	K6
      0x8824092A,  //  0108  GETMBR	R9	R4	K42
      0xB82A2400,  //  0109  GETNGBL	R10	K18
      0x8C281526,  //  010A  GETMET	R10	R10	K38
      0x5830002B,  //  010B  LDCONST	R12	K43
      0x7C280400,  //  010C  CALL	R10	2
      0x9428152C,  //  010D  GETIDX	R10	R10	K44
      0x9428152D,  //  010E  GETIDX	R10	R10	K45
      0x7C1C0600,  //  010F  CALL	R7	3
      0x80040E00,  //  0110  RET	1	R7
      0x70020007,  //  0111  JMP		#011A
      0x541E0007,  //  0112  LDINT	R7	8
      0x1C1C0C07,  //  0113  EQ	R7	R6	R7
      0x781E0004,  //  0114  JMPF	R7	#011A
      0x8C1C0906,  //  0115  GETMET	R7	R4	K6
      0x88240910,  //  0116  GETMBR	R9	R4	K16
      0x50280000,  //  0117  LDBOOL	R10	0	0
      0x7C1C0600,  //  0118  CALL	R7	3
      0x80040E00,  //  0119  RET	1	R7
      0x7002021F,  //  011A  JMP		#033B
      0x541E0033,  //  011B  LDINT	R7	52
      0x1C1C0A07,  //  011C  EQ	R7	R5	R7
      0x781E0000,  //  011D  JMPF	R7	#011F
      0x7002021B,  //  011E  JMP		#033B
      0x541E0037,  //  011F  LDINT	R7	56
      0x1C1C0A07,  //  0120  EQ	R7	R5	R7
      0x781E002C,  //  0121  JMPF	R7	#014F
      0x1C1C0D05,  //  0122  EQ	R7	R6	K5
      0x781E000F,  //  0123  JMPF	R7	#0134
      0xB81E5C00,  //  0124  GETNGBL	R7	K46
      0xB8222400,  //  0125  GETNGBL	R8	K18
      0x8C20112F,  //  0126  GETMET	R8	R8	K47
      0x7C200200,  //  0127  CALL	R8	1
      0x94201130,  //  0128  GETIDX	R8	R8	K48
      0x7C1C0200,  //  0129  CALL	R7	1
      0xB8225C00,  //  012A  GETNGBL	R8	K46
      0x58240031,  //  012B  LDCONST	R9	K49
      0x7C200200,  //  012C  CALL	R8	1
      0x081C0E08,  //  012D  MUL	R7	R7	R8
      0x8C200906,  //  012E  GETMET	R8	R4	K6
      0x88280907,  //  012F  GETMBR	R10	R4	K7
      0x5C2C0E00,  //  0130  MOVE	R11	R7
      0x7C200600,  //  0131  CALL	R8	3
      0x80041000,  //  0132  RET	1	R8
      0x70020019,  //  0133  JMP		#014E
      0x1C1C0D09,  //  0134  EQ	R7	R6	K9
      0x781E0005,  //  0135  JMPF	R7	#013C
      0x8C1C0906,  //  0136  GETMET	R7	R4	K6
      0x8824090E,  //  0137  GETMBR	R9	R4	K14
      0x5828000F,  //  0138  LDCONST	R10	K15
      0x7C1C0600,  //  0139  CALL	R7	3
      0x80040E00,  //  013A  RET	1	R7
      0x70020011,  //  013B  JMP		#014E
      0x541E0006,  //  013C  LDINT	R7	7
      0x1C1C0C07,  //  013D  EQ	R7	R6	R7
      0x781E000E,  //  013E  JMPF	R7	#014E
      0xB81E5C00,  //  013F  GETNGBL	R7	K46
      0xB8222400,  //  0140  GETNGBL	R8	K18
      0x8C20112F,  //  0141  GETMET	R8	R8	K47
      0x7C200200,  //  0142  CALL	R8	1
      0x94201132,  //  0143  GETIDX	R8	R8	K50
      0x7C1C0200,  //  0144  CALL	R7	1
      0xB8225C00,  //  0145  GETNGBL	R8	K46
      0x58240031,  //  0146  LDCONST	R9	K49
      0x7C200200,  //  0147  CALL	R8	1
      0x081C0E08,  //  0148  MUL	R7	R7	R8
      0x8C200906,  //  0149  GETMET	R8	R4	K6
      0x88280907,  //  014A  GETMBR	R10	R4	K7
      0x5C2C0E00,  //  014B  MOVE	R11	R7
      0x7C200600,  //  014C  CALL	R8	3
      0x80041000,  //  014D  RET	1	R8
      0x700201EB,  //  014E  JMP		#033B
      0x541E003D,  //  014F  LDINT	R7	62
      0x1C1C0A07,  //  0150  EQ	R7	R5	R7
      0x781E0082,  //  0151  JMPF	R7	#01D5
      0x1C1C0D05,  //  0152  EQ	R7	R6	K5
      0x781E001D,  //  0153  JMPF	R7	#0172
      0x8C1C0911,  //  0154  GETMET	R7	R4	K17
      0x7C1C0200,  //  0155  CALL	R7	1
      0x60200010,  //  0156  GETGBL	R8	G16
      0x88240133,  //  0157  GETMBR	R9	R0	K51
      0x88241334,  //  0158  GETMBR	R9	R9	K52
      0x8C241335,  //  0159  GETMET	R9	R9	K53
      0x7C240200,  //  015A  CALL	R9	1
      0x7C200200,  //  015B  CALL	R8	1
      0xA802000F,  //  015C  EXBLK	0	#016D
      0x5C241000,  //  015D  MOVE	R9	R8
      0x7C240000,  //  015E  CALL	R9	0
      0x8C280F15,  //  015F  GETMET	R10	R7	K21
      0x4C300000,  //  0160  LDNIL	R12
      0x7C280400,  //  0161  CALL	R10	2
      0x8C2C150B,  //  0162  GETMET	R11	R10	K11
      0x58340009,  //  0163  LDCONST	R13	K9
      0x88380936,  //  0164  GETMBR	R14	R4	K54
      0x883C1337,  //  0165  GETMBR	R15	R9	K55
      0x7C2C0800,  //  0166  CALL	R11	4
      0x8C2C150B,  //  0167  GETMET	R11	R10	K11
      0x5834000D,  //  0168  LDCONST	R13	K13
      0x88380936,  //  0169  GETMBR	R14	R4	K54
      0x883C1338,  //  016A  GETMBR	R15	R9	K56
      0x7C2C0800,  //  016B  CALL	R11	4
      0x7001FFEF,  //  016C  JMP		#015D
      0x58200039,  //  016D  LDCONST	R8	K57
      0xAC200200,  //  016E  CATCH	R8	1	0
      0xB0080000,  //  016F  RAISE	2	R0	R0
      0x80040E00,  //  0170  RET	1	R7
      0x70020061,  //  0171  JMP		#01D4
      0x1C1C0D09,  //  0172  EQ	R7	R6	K9
      0x781E0032,  //  0173  JMPF	R7	#01A7
      0x8C1C0911,  //  0174  GETMET	R7	R4	K17
      0x7C1C0200,  //  0175  CALL	R7	1
      0x60200010,  //  0176  GETGBL	R8	G16
      0x88240133,  //  0177  GETMBR	R9	R0	K51
      0x88241334,  //  0178  GETMBR	R9	R9	K52
      0x8C241335,  //  0179  GETMET	R9	R9	K53
      0x7C240200,  //  017A  CALL	R9	1
      0x7C200200,  //  017B  CALL	R8	1
      0xA8020024,  //  017C  EXBLK	0	#01A2
      0x5C241000,  //  017D  MOVE	R9	R8
      0x7C240000,  //  017E  CALL	R9	0
      0x8C28093A,  //  017F  GETMET	R10	R4	K58
      0x8C30133B,  //  0180  GETMET	R12	R9	K59
      0x7C300200,  //  0181  CALL	R12	1
      0x7C280400,  //  0182  CALL	R10	2
      0x8C2C0F15,  //  0183  GETMET	R11	R7	K21
      0x4C340000,  //  0184  LDNIL	R13
      0x7C2C0400,  //  0185  CALL	R11	2
      0x8C30170B,  //  0186  GETMET	R12	R11	K11
      0x58380009,  //  0187  LDCONST	R14	K9
      0x883C0936,  //  0188  GETMBR	R15	R4	K54
      0x8C40153C,  //  0189  GETMET	R16	R10	K60
      0x544A0008,  //  018A  LDINT	R18	9
      0x7C400400,  //  018B  CALL	R16	2
      0x7C300800,  //  018C  CALL	R12	4
      0x8C30170B,  //  018D  GETMET	R12	R11	K11
      0x5838000D,  //  018E  LDCONST	R14	K13
      0x883C090C,  //  018F  GETMBR	R15	R4	K12
      0x8840133D,  //  0190  GETMBR	R16	R9	K61
      0x7C300800,  //  0191  CALL	R12	4
      0x8C30170B,  //  0192  GETMET	R12	R11	K11
      0x5838000F,  //  0193  LDCONST	R14	K15
      0x883C0907,  //  0194  GETMBR	R15	R4	K7
      0x8840133E,  //  0195  GETMBR	R16	R9	K62
      0x7C300800,  //  0196  CALL	R12	4
      0x8C30170B,  //  0197  GETMET	R12	R11	K11
      0x543A0003,  //  0198  LDINT	R14	4
      0x883C0907,  //  0199  GETMBR	R15	R4	K7
      0x8840133F,  //  019A  GETMBR	R16	R9	K63
      0x7C300800,  //  019B  CALL	R12	4
      0x8C30170B,  //  019C  GETMET	R12	R11	K11
      0x543A0004,  //  019D  LDINT	R14	5
      0x883C0916,  //  019E  GETMBR	R15	R4	K22
      0x88401340,  //  019F  GETMBR	R16	R9	K64
      0x7C300800,  //  01A0  CALL	R12	4
      0x7001FFDA,  //  01A1  JMP		#017D
      0x58200039,  //  01A2  LDCONST	R8	K57
      0xAC200200,  //  01A3  CATCH	R8	1	0
      0xB0080000,  //  01A4  RAISE	2	R0	R0
      0x80040E00,  //  01A5  RET	1	R7
      0x7002002C,  //  01A6  JMP		#01D4
      0x1C1C0D0D,  //  01A7  EQ	R7	R6	K13
      0x781E0005,  //  01A8  JMPF	R7	#01AF
      0x8C1C0906,  //  01A9  GETMET	R7	R4	K6
      0x8824090E,  //  01AA  GETMBR	R9	R4	K14
      0x542A0004,  //  01AB  LDINT	R10	5
      0x7C1C0600,  //  01AC  CALL	R7	3
      0x80040E00,  //  01AD  RET	1	R7
      0x70020024,  //  01AE  JMP		#01D4
      0x1C1C0D0F,  //  01AF  EQ	R7	R6	K15
      0x781E000B,  //  01B0  JMPF	R7	#01BD
      0x881C0133,  //  01B1  GETMBR	R7	R0	K51
      0x881C0F34,  //  01B2  GETMBR	R7	R7	K52
      0x8C1C0F35,  //  01B3  GETMET	R7	R7	K53
      0x7C1C0200,  //  01B4  CALL	R7	1
      0x8C200906,  //  01B5  GETMET	R8	R4	K6
      0x8828090E,  //  01B6  GETMBR	R10	R4	K14
      0x602C000C,  //  01B7  GETGBL	R11	G12
      0x5C300E00,  //  01B8  MOVE	R12	R7
      0x7C2C0200,  //  01B9  CALL	R11	1
      0x7C200600,  //  01BA  CALL	R8	3
      0x80041000,  //  01BB  RET	1	R8
      0x70020016,  //  01BC  JMP		#01D4
      0x541E0003,  //  01BD  LDINT	R7	4
      0x1C1C0C07,  //  01BE  EQ	R7	R6	R7
      0x781E0000,  //  01BF  JMPF	R7	#01C1
      0x70020012,  //  01C0  JMP		#01D4
      0x541E0004,  //  01C1  LDINT	R7	5
      0x1C1C0C07,  //  01C2  EQ	R7	R6	R7
      0x781E000F,  //  01C3  JMPF	R7	#01D4
      0x881C0133,  //  01C4  GETMBR	R7	R0	K51
      0x881C0F34,  //  01C5  GETMBR	R7	R7	K52
      0x8C1C0F35,  //  01C6  GETMET	R7	R7	K53
      0x7C1C0200,  //  01C7  CALL	R7	1
      0x8C200F1B,  //  01C8  GETMET	R8	R7	K27
      0x5C280200,  //  01C9  MOVE	R10	R1
      0x7C200400,  //  01CA  CALL	R8	2
      0x4C240000,  //  01CB  LDNIL	R9
      0x1C241009,  //  01CC  EQ	R9	R8	R9
      0x78260000,  //  01CD  JMPF	R9	#01CF
      0x5421FFFE,  //  01CE  LDINT	R8	-1
      0x8C240906,  //  01CF  GETMET	R9	R4	K6
      0x882C090E,  //  01D0  GETMBR	R11	R4	K14
      0x00301109,  //  01D1  ADD	R12	R8	K9
      0x7C240600,  //  01D2  CALL	R9	3
      0x80041200,  //  01D3  RET	1	R9
      0x70020165,  //  01D4  JMP		#033B
      0x541E003B,  //  01D5  LDINT	R7	60
      0x1C1C0A07,  //  01D6  EQ	R7	R5	R7
      0x781E0000,  //  01D7  JMPF	R7	#01D9
      0x70020161,  //  01D8  JMP		#033B
      0x541E0027,  //  01D9  LDINT	R7	40
      0x1C1C0A07,  //  01DA  EQ	R7	R5	R7
      0x781E0091,  //  01DB  JMPF	R7	#026E
      0x1C1C0D05,  //  01DC  EQ	R7	R6	K5
      0x781E0005,  //  01DD  JMPF	R7	#01E4
      0x8C1C0906,  //  01DE  GETMET	R7	R4	K6
      0x8824090C,  //  01DF  GETMBR	R9	R4	K12
      0x58280005,  //  01E0  LDCONST	R10	K5
      0x7C1C0600,  //  01E1  CALL	R7	3
      0x80040E00,  //  01E2  RET	1	R7
      0x70020088,  //  01E3  JMP		#026D
      0x1C1C0D09,  //  01E4  EQ	R7	R6	K9
      0x781E0005,  //  01E5  JMPF	R7	#01EC
      0x8C1C0906,  //  01E6  GETMET	R7	R4	K6
      0x88240916,  //  01E7  GETMBR	R9	R4	K22
      0x58280041,  //  01E8  LDCONST	R10	K65
      0x7C1C0600,  //  01E9  CALL	R7	3
      0x80040E00,  //  01EA  RET	1	R7
      0x70020080,  //  01EB  JMP		#026D
      0x1C1C0D0D,  //  01EC  EQ	R7	R6	K13
      0x781E0006,  //  01ED  JMPF	R7	#01F5
      0x8C1C0906,  //  01EE  GETMET	R7	R4	K6
      0x8824090C,  //  01EF  GETMBR	R9	R4	K12
      0x88280133,  //  01F0  GETMBR	R10	R0	K51
      0x88281542,  //  01F1  GETMBR	R10	R10	K66
      0x7C1C0600,  //  01F2  CALL	R7	3
      0x80040E00,  //  01F3  RET	1	R7
      0x70020077,  //  01F4  JMP		#026D
      0x1C1C0D0F,  //  01F5  EQ	R7	R6	K15
      0x781E0009,  //  01F6  JMPF	R7	#0201
      0x8C1C0906,  //  01F7  GETMET	R7	R4	K6
      0x88240916,  //  01F8  GETMBR	R9	R4	K22
      0xB82A2400,  //  01F9  GETNGBL	R10	K18
      0x8C281526,  //  01FA  GETMET	R10	R10	K38
      0x58300043,  //  01FB  LDCONST	R12	K67
      0x7C280400,  //  01FC  CALL	R10	2
      0x94281543,  //  01FD  GETIDX	R10	R10	K67
      0x7C1C0600,  //  01FE  CALL	R7	3
      0x80040E00,  //  01FF  RET	1	R7
      0x7002006B,  //  0200  JMP		#026D
      0x541E0003,  //  0201  LDINT	R7	4
      0x1C1C0C07,  //  0202  EQ	R7	R6	R7
      0x781E0005,  //  0203  JMPF	R7	#020A
      0x8C1C0906,  //  0204  GETMET	R7	R4	K6
      0x8824090C,  //  0205  GETMBR	R9	R4	K12
      0x542A7FFF,  //  0206  LDINT	R10	32768
      0x7C1C0600,  //  0207  CALL	R7	3
      0x80040E00,  //  0208  RET	1	R7
      0x70020062,  //  0209  JMP		#026D
      0x541E0004,  //  020A  LDINT	R7	5
      0x1C1C0C07,  //  020B  EQ	R7	R6	R7
      0x781E0009,  //  020C  JMPF	R7	#0217
      0x8C1C0906,  //  020D  GETMET	R7	R4	K6
      0x88240916,  //  020E  GETMBR	R9	R4	K22
      0xB82A2400,  //  020F  GETNGBL	R10	K18
      0x8C281526,  //  0210  GETMET	R10	R10	K38
      0x58300044,  //  0211  LDCONST	R12	K68
      0x7C280400,  //  0212  CALL	R10	2
      0x94281545,  //  0213  GETIDX	R10	R10	K69
      0x7C1C0600,  //  0214  CALL	R7	3
      0x80040E00,  //  0215  RET	1	R7
      0x70020055,  //  0216  JMP		#026D
      0x541E0005,  //  0217  LDINT	R7	6
      0x1C1C0C07,  //  0218  EQ	R7	R6	R7
      0x781E0005,  //  0219  JMPF	R7	#0220
      0x8C1C0906,  //  021A  GETMET	R7	R4	K6
      0x88240916,  //  021B  GETMBR	R9	R4	K22
      0x58280046,  //  021C  LDCONST	R10	K70
      0x7C1C0600,  //  021D  CALL	R7	3
      0x80040E00,  //  021E  RET	1	R7
      0x7002004C,  //  021F  JMP		#026D
      0x541E0006,  //  0220  LDINT	R7	7
      0x1C1C0C07,  //  0221  EQ	R7	R6	R7
      0x781E0005,  //  0222  JMPF	R7	#0229
      0x8C1C0906,  //  0223  GETMET	R7	R4	K6
      0x8824090C,  //  0224  GETMBR	R9	R4	K12
      0x58280005,  //  0225  LDCONST	R10	K5
      0x7C1C0600,  //  0226  CALL	R7	3
      0x80040E00,  //  0227  RET	1	R7
      0x70020043,  //  0228  JMP		#026D
      0x541E0007,  //  0229  LDINT	R7	8
      0x1C1C0C07,  //  022A  EQ	R7	R6	R7
      0x781E000A,  //  022B  JMPF	R7	#0237
      0x8C1C0906,  //  022C  GETMET	R7	R4	K6
      0x88240916,  //  022D  GETMBR	R9	R4	K22
      0xB82A2400,  //  022E  GETNGBL	R10	K18
      0x8C281526,  //  022F  GETMET	R10	R10	K38
      0x58300047,  //  0230  LDCONST	R12	K71
      0x7C280400,  //  0231  CALL	R10	2
      0x94281548,  //  0232  GETIDX	R10	R10	K72
      0x94281549,  //  0233  GETIDX	R10	R10	K73
      0x7C1C0600,  //  0234  CALL	R7	3
      0x80040E00,  //  0235  RET	1	R7
      0x70020035,  //  0236  JMP		#026D
      0x541E0008,  //  0237  LDINT	R7	9
      0x1C1C0C07,  //  0238  EQ	R7	R6	R7
      0x781E0005,  //  0239  JMPF	R7	#0240
      0x8C1C0906,  //  023A  GETMET	R7	R4	K6
      0x8824090C,  //  023B  GETMBR	R9	R4	K12
      0x58280009,  //  023C  LDCONST	R10	K9
      0x7C1C0600,  //  023D  CALL	R7	3
      0x80040E00,  //  023E  RET	1	R7
      0x7002002C,  //  023F  JMP		#026D
      0x541E0009,  //  0240  LDINT	R7	10
      0x1C1C0C07,  //  0241  EQ	R7	R6	R7
      0x781E000A,  //  0242  JMPF	R7	#024E
      0x8C1C0906,  //  0243  GETMET	R7	R4	K6
      0x88240916,  //  0244  GETMBR	R9	R4	K22
      0xB82A2400,  //  0245  GETNGBL	R10	K18
      0x8C281526,  //  0246  GETMET	R10	R10	K38
      0x58300047,  //  0247  LDCONST	R12	K71
      0x7C280400,  //  0248  CALL	R10	2
      0x94281548,  //  0249  GETIDX	R10	R10	K72
      0x9428154A,  //  024A  GETIDX	R10	R10	K74
      0x7C1C0600,  //  024B  CALL	R7	3
      0x80040E00,  //  024C  RET	1	R7
      0x7002001E,  //  024D  JMP		#026D
      0x541E0011,  //  024E  LDINT	R7	18
      0x1C1C0C07,  //  024F  EQ	R7	R6	R7
      0x781E000B,  //  0250  JMPF	R7	#025D
      0x8C1C0906,  //  0251  GETMET	R7	R4	K6
      0x88240916,  //  0252  GETMBR	R9	R4	K22
      0xB82A2400,  //  0253  GETNGBL	R10	K18
      0x8C281525,  //  0254  GETMET	R10	R10	K37
      0x7C280200,  //  0255  CALL	R10	1
      0x8C28151B,  //  0256  GETMET	R10	R10	K27
      0x5830001C,  //  0257  LDCONST	R12	K28
      0x5834001D,  //  0258  LDCONST	R13	K29
      0x7C280600,  //  0259  CALL	R10	3
      0x7C1C0600,  //  025A  CALL	R7	3
      0x80040E00,  //  025B  RET	1	R7
      0x7002000F,  //  025C  JMP		#026D
      0x541E0012,  //  025D  LDINT	R7	19
      0x1C1C0C07,  //  025E  EQ	R7	R6	R7
      0x781E000C,  //  025F  JMPF	R7	#026D
      0x8C1C090A,  //  0260  GETMET	R7	R4	K10
      0x7C1C0200,  //  0261  CALL	R7	1
      0x8C200F0B,  //  0262  GETMET	R8	R7	K11
      0x58280005,  //  0263  LDCONST	R10	K5
      0x882C090C,  //  0264  GETMBR	R11	R4	K12
      0x5830000F,  //  0265  LDCONST	R12	K15
      0x7C200800,  //  0266  CALL	R8	4
      0x8C200F0B,  //  0267  GETMET	R8	R7	K11
      0x58280009,  //  0268  LDCONST	R10	K9
      0x882C090C,  //  0269  GETMBR	R11	R4	K12
      0x5830000F,  //  026A  LDCONST	R12	K15
      0x7C200800,  //  026B  CALL	R8	4
      0x80040E00,  //  026C  RET	1	R7
      0x700200CC,  //  026D  JMP		#033B
      0x541E003E,  //  026E  LDINT	R7	63
      0x1C1C0A07,  //  026F  EQ	R7	R5	R7
      0x781E0000,  //  0270  JMPF	R7	#0272
      0x700200C8,  //  0271  JMP		#033B
      0x541E0029,  //  0272  LDINT	R7	42
      0x1C1C0A07,  //  0273  EQ	R7	R5	R7
      0x781E001D,  //  0274  JMPF	R7	#0293
      0x1C1C0D05,  //  0275  EQ	R7	R6	K5
      0x781E0003,  //  0276  JMPF	R7	#027B
      0x8C1C0911,  //  0277  GETMET	R7	R4	K17
      0x7C1C0200,  //  0278  CALL	R7	1
      0x80040E00,  //  0279  RET	1	R7
      0x70020016,  //  027A  JMP		#0292
      0x1C1C0D09,  //  027B  EQ	R7	R6	K9
      0x781E0005,  //  027C  JMPF	R7	#0283
      0x8C1C0906,  //  027D  GETMET	R7	R4	K6
      0x88240910,  //  027E  GETMBR	R9	R4	K16
      0x58280005,  //  027F  LDCONST	R10	K5
      0x7C1C0600,  //  0280  CALL	R7	3
      0x80040E00,  //  0281  RET	1	R7
      0x7002000E,  //  0282  JMP		#0292
      0x1C1C0D0D,  //  0283  EQ	R7	R6	K13
      0x781E0005,  //  0284  JMPF	R7	#028B
      0x8C1C0906,  //  0285  GETMET	R7	R4	K6
      0x8824090E,  //  0286  GETMBR	R9	R4	K14
      0x58280009,  //  0287  LDCONST	R10	K9
      0x7C1C0600,  //  0288  CALL	R7	3
      0x80040E00,  //  0289  RET	1	R7
      0x70020006,  //  028A  JMP		#0292
      0x1C1C0D0F,  //  028B  EQ	R7	R6	K15
      0x781E0004,  //  028C  JMPF	R7	#0292
      0x8C1C0906,  //  028D  GETMET	R7	R4	K6
      0x88240918,  //  028E  GETMBR	R9	R4	K24
      0x4C280000,  //  028F  LDNIL	R10
      0x7C1C0600,  //  0290  CALL	R7	3
      0x80040E00,  //  0291  RET	1	R7
      0x700200A7,  //  0292  JMP		#033B
      0x541E002A,  //  0293  LDINT	R7	43
      0x1C1C0A07,  //  0294  EQ	R7	R5	R7
      0x781E0016,  //  0295  JMPF	R7	#02AD
      0x1C1C0D05,  //  0296  EQ	R7	R6	K5
      0x781E0007,  //  0297  JMPF	R7	#02A0
      0x8C1C0906,  //  0298  GETMET	R7	R4	K6
      0x88240916,  //  0299  GETMBR	R9	R4	K22
      0xB82A2400,  //  029A  GETNGBL	R10	K18
      0x8C28154B,  //  029B  GETMET	R10	R10	K75
      0x7C280200,  //  029C  CALL	R10	1
      0x7C1C0600,  //  029D  CALL	R7	3
      0x80040E00,  //  029E  RET	1	R7
      0x7002000B,  //  029F  JMP		#02AC
      0x1C1C0D09,  //  02A0  EQ	R7	R6	K9
      0x781E0009,  //  02A1  JMPF	R7	#02AC
      0x8C1C0911,  //  02A2  GETMET	R7	R4	K17
      0x7C1C0200,  //  02A3  CALL	R7	1
      0x8C200F0B,  //  02A4  GETMET	R8	R7	K11
      0x4C280000,  //  02A5  LDNIL	R10
      0x882C0916,  //  02A6  GETMBR	R11	R4	K22
      0xB8322400,  //  02A7  GETNGBL	R12	K18
      0x8C30194B,  //  02A8  GETMET	R12	R12	K75
      0x7C300200,  //  02A9  CALL	R12	1
      0x7C200800,  //  02AA  CALL	R8	4
      0x80040E00,  //  02AB  RET	1	R7
      0x7002008D,  //  02AC  JMP		#033B
      0x541E002B,  //  02AD  LDINT	R7	44
      0x1C1C0A07,  //  02AE  EQ	R7	R5	R7
      0x781E001C,  //  02AF  JMPF	R7	#02CD
      0x1C1C0D05,  //  02B0  EQ	R7	R6	K5
      0x781E0005,  //  02B1  JMPF	R7	#02B8
      0x8C1C0906,  //  02B2  GETMET	R7	R4	K6
      0x8824090E,  //  02B3  GETMBR	R9	R4	K14
      0x58280009,  //  02B4  LDCONST	R10	K9
      0x7C1C0600,  //  02B5  CALL	R7	3
      0x80040E00,  //  02B6  RET	1	R7
      0x70020013,  //  02B7  JMP		#02CC
      0x1C1C0D09,  //  02B8  EQ	R7	R6	K9
      0x781E0005,  //  02B9  JMPF	R7	#02C0
      0x8C1C0906,  //  02BA  GETMET	R7	R4	K6
      0x8824090E,  //  02BB  GETMBR	R9	R4	K14
      0x542A0003,  //  02BC  LDINT	R10	4
      0x7C1C0600,  //  02BD  CALL	R7	3
      0x80040E00,  //  02BE  RET	1	R7
      0x7002000B,  //  02BF  JMP		#02CC
      0x1C1C0D0D,  //  02C0  EQ	R7	R6	K13
      0x781E0009,  //  02C1  JMPF	R7	#02CC
      0x8C1C0911,  //  02C2  GETMET	R7	R4	K17
      0x7C1C0200,  //  02C3  CALL	R7	1
      0x8C200F0B,  //  02C4  GETMET	R8	R7	K11
      0x4C280000,  //  02C5  LDNIL	R10
      0x8C2C0906,  //  02C6  GETMET	R11	R4	K6
      0x8834090E,  //  02C7  GETMBR	R13	R4	K14
      0x543A0003,  //  02C8  LDINT	R14	4
      0x7C2C0600,  //  02C9  CALL	R11	3
      0x7C200600,  //  02CA  CALL	R8	3
      0x80040E00,  //  02CB  RET	1	R7
      0x7002006D,  //  02CC  JMP		#033B
      0x541E0030,  //  02CD  LDINT	R7	49
      0x1C1C0A07,  //  02CE  EQ	R7	R5	R7
      0x781E0010,  //  02CF  JMPF	R7	#02E1
      0x1C1C0D0F,  //  02D0  EQ	R7	R6	K15
      0x781E0005,  //  02D1  JMPF	R7	#02D8
      0x8C1C0906,  //  02D2  GETMET	R7	R4	K6
      0x8824090E,  //  02D3  GETMBR	R9	R4	K14
      0x542A001D,  //  02D4  LDINT	R10	30
      0x7C1C0600,  //  02D5  CALL	R7	3
      0x80040E00,  //  02D6  RET	1	R7
      0x70020007,  //  02D7  JMP		#02E0
      0x541EFFFB,  //  02D8  LDINT	R7	65532
      0x1C1C0C07,  //  02D9  EQ	R7	R6	R7
      0x781E0004,  //  02DA  JMPF	R7	#02E0
      0x8C1C0906,  //  02DB  GETMET	R7	R4	K6
      0x8824092A,  //  02DC  GETMBR	R9	R4	K42
      0x58280005,  //  02DD  LDCONST	R10	K5
      0x7C1C0600,  //  02DE  CALL	R7	3
      0x80040E00,  //  02DF  RET	1	R7
      0x70020059,  //  02E0  JMP		#033B
      0x541E001C,  //  02E1  LDINT	R7	29
      0x1C1C0A07,  //  02E2  EQ	R7	R5	R7
      0x781E0053,  //  02E3  JMPF	R7	#0338
      0x1C1C0D05,  //  02E4  EQ	R7	R6	K5
      0x781E001C,  //  02E5  JMPF	R7	#0303
      0x8C1C0911,  //  02E6  GETMET	R7	R4	K17
      0x7C1C0200,  //  02E7  CALL	R7	1
      0x60200010,  //  02E8  GETGBL	R8	G16
      0x8824014C,  //  02E9  GETMBR	R9	R0	K76
      0x8C24134D,  //  02EA  GETMET	R9	R9	K77
      0x7C240200,  //  02EB  CALL	R9	1
      0x7C200200,  //  02EC  CALL	R8	1
      0xA802000F,  //  02ED  EXBLK	0	#02FE
      0x5C241000,  //  02EE  MOVE	R9	R8
      0x7C240000,  //  02EF  CALL	R9	0
      0x8C280F15,  //  02F0  GETMET	R10	R7	K21
      0x7C280200,  //  02F1  CALL	R10	1
      0x8C2C150B,  //  02F2  GETMET	R11	R10	K11
      0x58340005,  //  02F3  LDCONST	R13	K5
      0x8838090C,  //  02F4  GETMBR	R14	R4	K12
      0x5C3C1200,  //  02F5  MOVE	R15	R9
      0x7C2C0800,  //  02F6  CALL	R11	4
      0x8C2C150B,  //  02F7  GETMET	R11	R10	K11
      0x58340009,  //  02F8  LDCONST	R13	K9
      0x8838090C,  //  02F9  GETMBR	R14	R4	K12
      0x883C014C,  //  02FA  GETMBR	R15	R0	K76
      0x943C1E09,  //  02FB  GETIDX	R15	R15	R9
      0x7C2C0800,  //  02FC  CALL	R11	4
      0x7001FFEF,  //  02FD  JMP		#02EE
      0x58200039,  //  02FE  LDCONST	R8	K57
      0xAC200200,  //  02FF  CATCH	R8	1	0
      0xB0080000,  //  0300  RAISE	2	R0	R0
      0x80040E00,  //  0301  RET	1	R7
      0x70020033,  //  0302  JMP		#0337
      0x1C1C0D09,  //  0303  EQ	R7	R6	K9
      0x781E0013,  //  0304  JMPF	R7	#0319
      0x8C1C0911,  //  0305  GETMET	R7	R4	K17
      0x7C1C0200,  //  0306  CALL	R7	1
      0x60200010,  //  0307  GETGBL	R8	G16
      0x8C24014E,  //  0308  GETMET	R9	R0	K78
      0x7C240200,  //  0309  CALL	R9	1
      0x7C200200,  //  030A  CALL	R8	1
      0xA8020007,  //  030B  EXBLK	0	#0314
      0x5C241000,  //  030C  MOVE	R9	R8
      0x7C240000,  //  030D  CALL	R9	0
      0x8C280F0B,  //  030E  GETMET	R10	R7	K11
      0x4C300000,  //  030F  LDNIL	R12
      0x8834092A,  //  0310  GETMBR	R13	R4	K42
      0x5C381200,  //  0311  MOVE	R14	R9
      0x7C280800,  //  0312  CALL	R10	4
      0x7001FFF7,  //  0313  JMP		#030C
      0x58200039,  //  0314  LDCONST	R8	K57
      0xAC200200,  //  0315  CATCH	R8	1	0
      0xB0080000,  //  0316  RAISE	2	R0	R0
      0x80040E00,  //  0317  RET	1	R7
      0x7002001D,  //  0318  JMP		#0337
      0x1C1C0D0D,  //  0319  EQ	R7	R6	K13
      0x781E0003,  //  031A  JMPF	R7	#031F
      0x8C1C0911,  //  031B  GETMET	R7	R4	K17
      0x7C1C0200,  //  031C  CALL	R7	1
      0x80040E00,  //  031D  RET	1	R7
      0x70020017,  //  031E  JMP		#0337
      0x1C1C0D0F,  //  031F  EQ	R7	R6	K15
      0x781E0015,  //  0320  JMPF	R7	#0337
      0x881C0133,  //  0321  GETMBR	R7	R0	K51
      0x8C1C0F4F,  //  0322  GETMET	R7	R7	K79
      0x50240200,  //  0323  LDBOOL	R9	1	0
      0x7C1C0400,  //  0324  CALL	R7	2
      0x8C200911,  //  0325  GETMET	R8	R4	K17
      0x7C200200,  //  0326  CALL	R8	1
      0x60240010,  //  0327  GETGBL	R9	G16
      0x5C280E00,  //  0328  MOVE	R10	R7
      0x7C240200,  //  0329  CALL	R9	1
      0xA8020007,  //  032A  EXBLK	0	#0333
      0x5C281200,  //  032B  MOVE	R10	R9
      0x7C280000,  //  032C  CALL	R10	0
      0x8C2C110B,  //  032D  GETMET	R11	R8	K11
      0x4C340000,  //  032E  LDNIL	R13
      0x8838090C,  //  032F  GETMBR	R14	R4	K12
      0x5C3C1400,  //  0330  MOVE	R15	R10
      0x7C2C0800,  //  0331  CALL	R11	4
      0x7001FFF7,  //  0332  JMP		#032B
      0x58240039,  //  0333  LDCONST	R9	K57
      0xAC240200,  //  0334  CATCH	R9	1	0
      0xB0080000,  //  0335  RAISE	2	R0	R0
      0x80041000,  //  0336  RET	1	R8
      0x70020002,  //  0337  JMP		#033B
      0xB81E0200,  //  0338  GETNGBL	R7	K1
      0x881C0F51,  //  0339  GETMBR	R7	R7	K81
      0x900AA007,  //  033A  SETMBR	R2	K80	R7
      0x80000000,  //  033B  RET	0
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
    /* K8   */  be_nested_str_weak(__breadcrumb),
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
    ( &(const bvalue[72]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(__breadcrumb),
    /* K9   */  be_nested_str_weak(Matter_TLV_struct),
    /* K10  */  be_nested_str_weak(add_TLV),
    /* K11  */  be_nested_str_weak(U1),
    /* K12  */  be_nested_str_weak(UTF1),
    /* K13  */  be_nested_str_weak(),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(XX),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(set_no_expiration),
    /* K18  */  be_nested_str_weak(device),
    /* K19  */  be_nested_str_weak(start_commissioning_complete_deferred),
    /* K20  */  be_nested_str_weak(status),
    /* K21  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K22  */  be_nested_str_weak(B2),
    /* K23  */  be_nested_str_weak(DAC_Cert_FFF1_8000),
    /* K24  */  be_nested_str_weak(PAI_Cert_FFF1),
    /* K25  */  be_nested_str_weak(CD_FFF1_8000),
    /* K26  */  be_nested_str_weak(B1),
    /* K27  */  be_nested_str_weak(U4),
    /* K28  */  be_nested_str_weak(tasmota),
    /* K29  */  be_nested_str_weak(rtc),
    /* K30  */  be_nested_str_weak(utc),
    /* K31  */  be_nested_str_weak(encode),
    /* K32  */  be_nested_str_weak(get_ac),
    /* K33  */  be_nested_str_weak(log),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20attestation_tbs_X3D),
    /* K35  */  be_nested_str_weak(tohex),
    /* K36  */  be_nested_str_weak(EC_P256),
    /* K37  */  be_nested_str_weak(ecdsa_sign_sha256),
    /* K38  */  be_nested_str_weak(DAC_Priv_FFF1_8000),
    /* K39  */  be_nested_str_weak(gen_CSR),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20nocsr_tbs_X3D),
    /* K41  */  be_nested_str_weak(set_ca),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20received_X20ca_root_X3D),
    /* K43  */  be_nested_str_weak(SUCCESS),
    /* K44  */  be_nested_str_weak(get_ca),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20AdNOC_X20without_X20CA),
    /* K46  */  be_nested_str_weak(set_noc),
    /* K47  */  be_nested_str_weak(set_ipk_epoch_key),
    /* K48  */  be_nested_str_weak(admin_subject),
    /* K49  */  be_nested_str_weak(admin_vendor),
    /* K50  */  be_nested_str_weak(parse),
    /* K51  */  be_nested_str_weak(findsub),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20Error_X3A_X20no_X20fabricid_X20nor_X20deviceid_X20in_X20NOC_X20certificate),
    /* K53  */  be_nested_str_weak(int),
    /* K54  */  be_nested_str_weak(int64),
    /* K55  */  be_nested_str_weak(tobytes),
    /* K56  */  be_const_int(2147483647),
    /* K57  */  be_nested_str_weak(fromstring),
    /* K58  */  be_nested_str_weak(CompressedFabric),
    /* K59  */  be_nested_str_weak(HKDF_SHA256),
    /* K60  */  be_nested_str_weak(copy),
    /* K61  */  be_nested_str_weak(reverse),
    /* K62  */  be_nested_str_weak(derive),
    /* K63  */  be_nested_str_weak(set_fabric_device),
    /* K64  */  be_nested_str_weak(start_operational_dicovery_deferred),
    /* K65  */  be_nested_str_weak(set_fabric_label),
    /* K66  */  be_nested_str_weak(sessions),
    /* K67  */  be_nested_str_weak(sessions_active),
    /* K68  */  be_nested_str_weak(MTR_X3A_X20removing_X20fabric_X20),
    /* K69  */  be_nested_str_weak(fabric),
    /* K70  */  be_nested_str_weak(remove_session),
    /* K71  */  be_nested_str_weak(save),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[502]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x5422002F,  //  0005  LDINT	R8	48
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220050,  //  0007  JMPF	R8	#0059
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
      0x70020035,  //  0021  JMP		#0058
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
      0x70020018,  //  003E  JMP		#0058
      0x54220003,  //  003F  LDINT	R8	4
      0x1C200E08,  //  0040  EQ	R8	R7	R8
      0x78220015,  //  0041  JMPF	R8	#0058
      0x90061105,  //  0042  SETMBR	R1	K8	K5
      0x8C200311,  //  0043  GETMET	R8	R1	K17
      0x7C200200,  //  0044  CALL	R8	1
      0x8C200B09,  //  0045  GETMET	R8	R5	K9
      0x7C200200,  //  0046  CALL	R8	1
      0x8C24110A,  //  0047  GETMET	R9	R8	K10
      0x582C0005,  //  0048  LDCONST	R11	K5
      0x88300B0B,  //  0049  GETMBR	R12	R5	K11
      0x58340005,  //  004A  LDCONST	R13	K5
      0x7C240800,  //  004B  CALL	R9	4
      0x8C24110A,  //  004C  GETMET	R9	R8	K10
      0x582C0007,  //  004D  LDCONST	R11	K7
      0x88300B0C,  //  004E  GETMBR	R12	R5	K12
      0x5834000D,  //  004F  LDCONST	R13	K13
      0x7C240800,  //  0050  CALL	R9	4
      0x54260004,  //  0051  LDINT	R9	5
      0x900E0809,  //  0052  SETMBR	R3	K4	R9
      0x88240112,  //  0053  GETMBR	R9	R0	K18
      0x8C241313,  //  0054  GETMET	R9	R9	K19
      0x5C2C0200,  //  0055  MOVE	R11	R1
      0x7C240400,  //  0056  CALL	R9	2
      0x80041000,  //  0057  RET	1	R8
      0x7002019B,  //  0058  JMP		#01F5
      0x5422003D,  //  0059  LDINT	R8	62
      0x1C200C08,  //  005A  EQ	R8	R6	R8
      0x78220191,  //  005B  JMPF	R8	#01EE
      0x1C200F0E,  //  005C  EQ	R8	R7	K14
      0x7822001D,  //  005D  JMPF	R8	#007C
      0x8C200506,  //  005E  GETMET	R8	R2	K6
      0x58280005,  //  005F  LDCONST	R10	K5
      0x7C200400,  //  0060  CALL	R8	2
      0x20241107,  //  0061  NE	R9	R8	K7
      0x78260006,  //  0062  JMPF	R9	#006A
      0x2024110E,  //  0063  NE	R9	R8	K14
      0x78260004,  //  0064  JMPF	R9	#006A
      0xB8260200,  //  0065  GETNGBL	R9	K1
      0x88241315,  //  0066  GETMBR	R9	R9	K21
      0x900E2809,  //  0067  SETMBR	R3	K20	R9
      0x4C240000,  //  0068  LDNIL	R9
      0x80041200,  //  0069  RET	1	R9
      0x8C240B09,  //  006A  GETMET	R9	R5	K9
      0x7C240200,  //  006B  CALL	R9	1
      0x8C28130A,  //  006C  GETMET	R10	R9	K10
      0x58300005,  //  006D  LDCONST	R12	K5
      0x88340B16,  //  006E  GETMBR	R13	R5	K22
      0x1C381107,  //  006F  EQ	R14	R8	K7
      0x783A0003,  //  0070  JMPF	R14	#0075
      0xB83A0200,  //  0071  GETNGBL	R14	K1
      0x8C381D17,  //  0072  GETMET	R14	R14	K23
      0x7C380200,  //  0073  CALL	R14	1
      0x70020002,  //  0074  JMP		#0078
      0xB83A0200,  //  0075  GETNGBL	R14	K1
      0x8C381D18,  //  0076  GETMET	R14	R14	K24
      0x7C380200,  //  0077  CALL	R14	1
      0x7C280800,  //  0078  CALL	R10	4
      0x900E0910,  //  0079  SETMBR	R3	K4	K16
      0x80041200,  //  007A  RET	1	R9
      0x70020170,  //  007B  JMP		#01ED
      0x1C200F05,  //  007C  EQ	R8	R7	K5
      0x78220044,  //  007D  JMPF	R8	#00C3
      0x8C200506,  //  007E  GETMET	R8	R2	K6
      0x58280005,  //  007F  LDCONST	R10	K5
      0x7C200400,  //  0080  CALL	R8	2
      0x6024000C,  //  0081  GETGBL	R9	G12
      0x5C281000,  //  0082  MOVE	R10	R8
      0x7C240200,  //  0083  CALL	R9	1
      0x542A001F,  //  0084  LDINT	R10	32
      0x2024120A,  //  0085  NE	R9	R9	R10
      0x78260001,  //  0086  JMPF	R9	#0089
      0x4C240000,  //  0087  LDNIL	R9
      0x80041200,  //  0088  RET	1	R9
      0x900E0907,  //  0089  SETMBR	R3	K4	K7
      0x8C240B09,  //  008A  GETMET	R9	R5	K9
      0x7C240200,  //  008B  CALL	R9	1
      0x8C28130A,  //  008C  GETMET	R10	R9	K10
      0x58300007,  //  008D  LDCONST	R12	K7
      0x88340B16,  //  008E  GETMBR	R13	R5	K22
      0xB83A0200,  //  008F  GETNGBL	R14	K1
      0x8C381D19,  //  0090  GETMET	R14	R14	K25
      0x7C380200,  //  0091  CALL	R14	1
      0x7C280800,  //  0092  CALL	R10	4
      0x8C28130A,  //  0093  GETMET	R10	R9	K10
      0x5830000E,  //  0094  LDCONST	R12	K14
      0x88340B1A,  //  0095  GETMBR	R13	R5	K26
      0x5C381000,  //  0096  MOVE	R14	R8
      0x7C280800,  //  0097  CALL	R10	4
      0x8C28130A,  //  0098  GETMET	R10	R9	K10
      0x58300010,  //  0099  LDCONST	R12	K16
      0x88340B1B,  //  009A  GETMBR	R13	R5	K27
      0xB83A3800,  //  009B  GETNGBL	R14	K28
      0x8C381D1D,  //  009C  GETMET	R14	R14	K29
      0x7C380200,  //  009D  CALL	R14	1
      0x94381D1E,  //  009E  GETIDX	R14	R14	K30
      0x7C280800,  //  009F  CALL	R10	4
      0x8C28131F,  //  00A0  GETMET	R10	R9	K31
      0x7C280200,  //  00A1  CALL	R10	1
      0x8C2C0320,  //  00A2  GETMET	R11	R1	K32
      0x7C2C0200,  //  00A3  CALL	R11	1
      0x0030140B,  //  00A4  ADD	R12	R10	R11
      0xB8363800,  //  00A5  GETNGBL	R13	K28
      0x8C341B21,  //  00A6  GETMET	R13	R13	K33
      0x8C3C1923,  //  00A7  GETMET	R15	R12	K35
      0x7C3C0200,  //  00A8  CALL	R15	1
      0x003E440F,  //  00A9  ADD	R15	K34	R15
      0x58400010,  //  00AA  LDCONST	R16	K16
      0x7C340600,  //  00AB  CALL	R13	3
      0x8C340924,  //  00AC  GETMET	R13	R4	K36
      0x7C340200,  //  00AD  CALL	R13	1
      0x8C341B25,  //  00AE  GETMET	R13	R13	K37
      0xB83E0200,  //  00AF  GETNGBL	R15	K1
      0x8C3C1F26,  //  00B0  GETMET	R15	R15	K38
      0x7C3C0200,  //  00B1  CALL	R15	1
      0x5C401800,  //  00B2  MOVE	R16	R12
      0x7C340600,  //  00B3  CALL	R13	3
      0x8C380B09,  //  00B4  GETMET	R14	R5	K9
      0x7C380200,  //  00B5  CALL	R14	1
      0x8C3C1D0A,  //  00B6  GETMET	R15	R14	K10
      0x58440005,  //  00B7  LDCONST	R17	K5
      0x88480B16,  //  00B8  GETMBR	R18	R5	K22
      0x5C4C1400,  //  00B9  MOVE	R19	R10
      0x7C3C0800,  //  00BA  CALL	R15	4
      0x8C3C1D0A,  //  00BB  GETMET	R15	R14	K10
      0x58440007,  //  00BC  LDCONST	R17	K7
      0x88480B1A,  //  00BD  GETMBR	R18	R5	K26
      0x5C4C1A00,  //  00BE  MOVE	R19	R13
      0x7C3C0800,  //  00BF  CALL	R15	4
      0x900E0907,  //  00C0  SETMBR	R3	K4	K7
      0x80041C00,  //  00C1  RET	1	R14
      0x70020129,  //  00C2  JMP		#01ED
      0x54220003,  //  00C3  LDINT	R8	4
      0x1C200E08,  //  00C4  EQ	R8	R7	R8
      0x78220040,  //  00C5  JMPF	R8	#0107
      0x8C200506,  //  00C6  GETMET	R8	R2	K6
      0x58280005,  //  00C7  LDCONST	R10	K5
      0x7C200400,  //  00C8  CALL	R8	2
      0x6024000C,  //  00C9  GETGBL	R9	G12
      0x5C281000,  //  00CA  MOVE	R10	R8
      0x7C240200,  //  00CB  CALL	R9	1
      0x542A001F,  //  00CC  LDINT	R10	32
      0x2024120A,  //  00CD  NE	R9	R9	R10
      0x78260001,  //  00CE  JMPF	R9	#00D1
      0x4C240000,  //  00CF  LDNIL	R9
      0x80041200,  //  00D0  RET	1	R9
      0x8C240506,  //  00D1  GETMET	R9	R2	K6
      0x582C0007,  //  00D2  LDCONST	R11	K7
      0x50300000,  //  00D3  LDBOOL	R12	0	0
      0x7C240600,  //  00D4  CALL	R9	3
      0x8C280327,  //  00D5  GETMET	R10	R1	K39
      0x7C280200,  //  00D6  CALL	R10	1
      0x8C2C0B09,  //  00D7  GETMET	R11	R5	K9
      0x7C2C0200,  //  00D8  CALL	R11	1
      0x8C30170A,  //  00D9  GETMET	R12	R11	K10
      0x58380007,  //  00DA  LDCONST	R14	K7
      0x883C0B16,  //  00DB  GETMBR	R15	R5	K22
      0x5C401400,  //  00DC  MOVE	R16	R10
      0x7C300800,  //  00DD  CALL	R12	4
      0x8C30170A,  //  00DE  GETMET	R12	R11	K10
      0x5838000E,  //  00DF  LDCONST	R14	K14
      0x883C0B1A,  //  00E0  GETMBR	R15	R5	K26
      0x5C401000,  //  00E1  MOVE	R16	R8
      0x7C300800,  //  00E2  CALL	R12	4
      0x8C30171F,  //  00E3  GETMET	R12	R11	K31
      0x7C300200,  //  00E4  CALL	R12	1
      0x8C340320,  //  00E5  GETMET	R13	R1	K32
      0x7C340200,  //  00E6  CALL	R13	1
      0x0034180D,  //  00E7  ADD	R13	R12	R13
      0xB83A3800,  //  00E8  GETNGBL	R14	K28
      0x8C381D21,  //  00E9  GETMET	R14	R14	K33
      0x8C401B23,  //  00EA  GETMET	R16	R13	K35
      0x7C400200,  //  00EB  CALL	R16	1
      0x00425010,  //  00EC  ADD	R16	K40	R16
      0x58440010,  //  00ED  LDCONST	R17	K16
      0x7C380600,  //  00EE  CALL	R14	3
      0x8C380924,  //  00EF  GETMET	R14	R4	K36
      0x7C380200,  //  00F0  CALL	R14	1
      0x8C381D25,  //  00F1  GETMET	R14	R14	K37
      0xB8420200,  //  00F2  GETNGBL	R16	K1
      0x8C402126,  //  00F3  GETMET	R16	R16	K38
      0x7C400200,  //  00F4  CALL	R16	1
      0x5C441A00,  //  00F5  MOVE	R17	R13
      0x7C380600,  //  00F6  CALL	R14	3
      0x8C3C0B09,  //  00F7  GETMET	R15	R5	K9
      0x7C3C0200,  //  00F8  CALL	R15	1
      0x8C401F0A,  //  00F9  GETMET	R16	R15	K10
      0x58480005,  //  00FA  LDCONST	R18	K5
      0x884C0B16,  //  00FB  GETMBR	R19	R5	K22
      0x5C501800,  //  00FC  MOVE	R20	R12
      0x7C400800,  //  00FD  CALL	R16	4
      0x8C401F0A,  //  00FE  GETMET	R16	R15	K10
      0x58480007,  //  00FF  LDCONST	R18	K7
      0x884C0B1A,  //  0100  GETMBR	R19	R5	K26
      0x5C501C00,  //  0101  MOVE	R20	R14
      0x7C400800,  //  0102  CALL	R16	4
      0x54420004,  //  0103  LDINT	R16	5
      0x900E0810,  //  0104  SETMBR	R3	K4	R16
      0x80041E00,  //  0105  RET	1	R15
      0x700200E5,  //  0106  JMP		#01ED
      0x5422000A,  //  0107  LDINT	R8	11
      0x1C200E08,  //  0108  EQ	R8	R7	R8
      0x78220012,  //  0109  JMPF	R8	#011D
      0x8C200506,  //  010A  GETMET	R8	R2	K6
      0x58280005,  //  010B  LDCONST	R10	K5
      0x7C200400,  //  010C  CALL	R8	2
      0x8C240329,  //  010D  GETMET	R9	R1	K41
      0x5C2C1000,  //  010E  MOVE	R11	R8
      0x7C240400,  //  010F  CALL	R9	2
      0xB8263800,  //  0110  GETNGBL	R9	K28
      0x8C241321,  //  0111  GETMET	R9	R9	K33
      0x8C2C1123,  //  0112  GETMET	R11	R8	K35
      0x7C2C0200,  //  0113  CALL	R11	1
      0x002E540B,  //  0114  ADD	R11	K42	R11
      0x58300010,  //  0115  LDCONST	R12	K16
      0x7C240600,  //  0116  CALL	R9	3
      0xB8260200,  //  0117  GETNGBL	R9	K1
      0x8824132B,  //  0118  GETMBR	R9	R9	K43
      0x900E2809,  //  0119  SETMBR	R3	K20	R9
      0x4C240000,  //  011A  LDNIL	R9
      0x80041200,  //  011B  RET	1	R9
      0x700200CF,  //  011C  JMP		#01ED
      0x54220005,  //  011D  LDINT	R8	6
      0x1C200E08,  //  011E  EQ	R8	R7	R8
      0x78220091,  //  011F  JMPF	R8	#01B2
      0x8C200506,  //  0120  GETMET	R8	R2	K6
      0x58280005,  //  0121  LDCONST	R10	K5
      0x7C200400,  //  0122  CALL	R8	2
      0x8C240506,  //  0123  GETMET	R9	R2	K6
      0x582C0007,  //  0124  LDCONST	R11	K7
      0x7C240400,  //  0125  CALL	R9	2
      0x8C280506,  //  0126  GETMET	R10	R2	K6
      0x5830000E,  //  0127  LDCONST	R12	K14
      0x7C280400,  //  0128  CALL	R10	2
      0x8C2C0506,  //  0129  GETMET	R11	R2	K6
      0x58340010,  //  012A  LDCONST	R13	K16
      0x7C2C0400,  //  012B  CALL	R11	2
      0x8C300506,  //  012C  GETMET	R12	R2	K6
      0x543A0003,  //  012D  LDINT	R14	4
      0x7C300400,  //  012E  CALL	R12	2
      0x8C34032C,  //  012F  GETMET	R13	R1	K44
      0x7C340200,  //  0130  CALL	R13	1
      0x4C380000,  //  0131  LDNIL	R14
      0x1C341A0E,  //  0132  EQ	R13	R13	R14
      0x78360006,  //  0133  JMPF	R13	#013B
      0xB8363800,  //  0134  GETNGBL	R13	K28
      0x8C341B21,  //  0135  GETMET	R13	R13	K33
      0x583C002D,  //  0136  LDCONST	R15	K45
      0x5840000E,  //  0137  LDCONST	R16	K14
      0x7C340600,  //  0138  CALL	R13	3
      0x4C340000,  //  0139  LDNIL	R13
      0x80041A00,  //  013A  RET	1	R13
      0x8C34032E,  //  013B  GETMET	R13	R1	K46
      0x5C3C1000,  //  013C  MOVE	R15	R8
      0x5C401200,  //  013D  MOVE	R16	R9
      0x7C340600,  //  013E  CALL	R13	3
      0x8C34032F,  //  013F  GETMET	R13	R1	K47
      0x5C3C1400,  //  0140  MOVE	R15	R10
      0x7C340400,  //  0141  CALL	R13	2
      0x9006600B,  //  0142  SETMBR	R1	K48	R11
      0x9006620C,  //  0143  SETMBR	R1	K49	R12
      0xB8360200,  //  0144  GETNGBL	R13	K1
      0x88341B02,  //  0145  GETMBR	R13	R13	K2
      0x8C341B32,  //  0146  GETMET	R13	R13	K50
      0x5C3C1000,  //  0147  MOVE	R15	R8
      0x7C340400,  //  0148  CALL	R13	2
      0x8C381B33,  //  0149  GETMET	R14	R13	K51
      0x54420005,  //  014A  LDINT	R16	6
      0x7C380400,  //  014B  CALL	R14	2
      0x8C3C1D06,  //  014C  GETMET	R15	R14	K6
      0x54460014,  //  014D  LDINT	R17	21
      0x7C3C0400,  //  014E  CALL	R15	2
      0x8C401D06,  //  014F  GETMET	R16	R14	K6
      0x544A0010,  //  0150  LDINT	R18	17
      0x7C400400,  //  0151  CALL	R16	2
      0x5C441E00,  //  0152  MOVE	R17	R15
      0x78460001,  //  0153  JMPF	R17	#0156
      0x5C442000,  //  0154  MOVE	R17	R16
      0x74460006,  //  0155  JMPT	R17	#015D
      0xB8463800,  //  0156  GETNGBL	R17	K28
      0x8C442321,  //  0157  GETMET	R17	R17	K33
      0x584C0034,  //  0158  LDCONST	R19	K52
      0x5850000E,  //  0159  LDCONST	R20	K14
      0x7C440600,  //  015A  CALL	R17	3
      0x50440000,  //  015B  LDBOOL	R17	0	0
      0x80042200,  //  015C  RET	1	R17
      0x60440004,  //  015D  GETGBL	R17	G4
      0x5C481E00,  //  015E  MOVE	R18	R15
      0x7C440200,  //  015F  CALL	R17	1
      0x1C442335,  //  0160  EQ	R17	R17	K53
      0x78460006,  //  0161  JMPF	R17	#0169
      0xB8466C00,  //  0162  GETNGBL	R17	K54
      0x5C481E00,  //  0163  MOVE	R18	R15
      0x7C440200,  //  0164  CALL	R17	1
      0x8C442337,  //  0165  GETMET	R17	R17	K55
      0x7C440200,  //  0166  CALL	R17	1
      0x5C3C2200,  //  0167  MOVE	R15	R17
      0x70020002,  //  0168  JMP		#016C
      0x8C441F37,  //  0169  GETMET	R17	R15	K55
      0x7C440200,  //  016A  CALL	R17	1
      0x5C3C2200,  //  016B  MOVE	R15	R17
      0x60440004,  //  016C  GETGBL	R17	G4
      0x5C482000,  //  016D  MOVE	R18	R16
      0x7C440200,  //  016E  CALL	R17	1
      0x1C442335,  //  016F  EQ	R17	R17	K53
      0x78460006,  //  0170  JMPF	R17	#0178
      0xB8466C00,  //  0171  GETNGBL	R17	K54
      0x5C482000,  //  0172  MOVE	R18	R16
      0x7C440200,  //  0173  CALL	R17	1
      0x8C442337,  //  0174  GETMET	R17	R17	K55
      0x7C440200,  //  0175  CALL	R17	1
      0x5C402200,  //  0176  MOVE	R16	R17
      0x70020002,  //  0177  JMP		#017B
      0x8C442137,  //  0178  GETMET	R17	R16	K55
      0x7C440200,  //  0179  CALL	R17	1
      0x5C402200,  //  017A  MOVE	R16	R17
      0xB8460200,  //  017B  GETNGBL	R17	K1
      0x88442302,  //  017C  GETMBR	R17	R17	K2
      0x8C442332,  //  017D  GETMET	R17	R17	K50
      0x8C4C032C,  //  017E  GETMET	R19	R1	K44
      0x7C4C0200,  //  017F  CALL	R19	1
      0x7C440400,  //  0180  CALL	R17	2
      0x8C442306,  //  0181  GETMET	R17	R17	K6
      0x544E0008,  //  0182  LDINT	R19	9
      0x7C440400,  //  0183  CALL	R17	2
      0x404A0F38,  //  0184  CONNECT	R18	K7	K56
      0x94442212,  //  0185  GETIDX	R17	R17	R18
      0x604C0015,  //  0186  GETGBL	R19	G21
      0x7C4C0000,  //  0187  CALL	R19	0
      0x8C4C2739,  //  0188  GETMET	R19	R19	K57
      0x5854003A,  //  0189  LDCONST	R21	K58
      0x7C4C0400,  //  018A  CALL	R19	2
      0x5C482600,  //  018B  MOVE	R18	R19
      0x8C4C093B,  //  018C  GETMET	R19	R4	K59
      0x7C4C0200,  //  018D  CALL	R19	1
      0x8C501F3C,  //  018E  GETMET	R20	R15	K60
      0x7C500200,  //  018F  CALL	R20	1
      0x8C50293D,  //  0190  GETMET	R20	R20	K61
      0x7C500200,  //  0191  CALL	R20	1
      0x8C54273E,  //  0192  GETMET	R21	R19	K62
      0x5C5C2200,  //  0193  MOVE	R23	R17
      0x5C602800,  //  0194  MOVE	R24	R20
      0x5C642400,  //  0195  MOVE	R25	R18
      0x546A0007,  //  0196  LDINT	R26	8
      0x7C540A00,  //  0197  CALL	R21	5
      0x8C58033F,  //  0198  GETMET	R22	R1	K63
      0x5C601E00,  //  0199  MOVE	R24	R15
      0x5C642000,  //  019A  MOVE	R25	R16
      0x5C682A00,  //  019B  MOVE	R26	R21
      0x7C580800,  //  019C  CALL	R22	4
      0x88580112,  //  019D  GETMBR	R22	R0	K18
      0x8C582D40,  //  019E  GETMET	R22	R22	K64
      0x5C600200,  //  019F  MOVE	R24	R1
      0x7C580400,  //  01A0  CALL	R22	2
      0x8C580B09,  //  01A1  GETMET	R22	R5	K9
      0x7C580200,  //  01A2  CALL	R22	1
      0x8C5C2D0A,  //  01A3  GETMET	R23	R22	K10
      0x58640005,  //  01A4  LDCONST	R25	K5
      0x88680B0B,  //  01A5  GETMBR	R26	R5	K11
      0xB86E0200,  //  01A6  GETNGBL	R27	K1
      0x886C372B,  //  01A7  GETMBR	R27	R27	K43
      0x7C5C0800,  //  01A8  CALL	R23	4
      0x8C5C2D0A,  //  01A9  GETMET	R23	R22	K10
      0x58640007,  //  01AA  LDCONST	R25	K7
      0x88680B0B,  //  01AB  GETMBR	R26	R5	K11
      0x586C0007,  //  01AC  LDCONST	R27	K7
      0x7C5C0800,  //  01AD  CALL	R23	4
      0x545E0007,  //  01AE  LDINT	R23	8
      0x900E0817,  //  01AF  SETMBR	R3	K4	R23
      0x80042C00,  //  01B0  RET	1	R22
      0x7002003A,  //  01B1  JMP		#01ED
      0x54220008,  //  01B2  LDINT	R8	9
      0x1C200E08,  //  01B3  EQ	R8	R7	R8
      0x7822000B,  //  01B4  JMPF	R8	#01C1
      0x8C200506,  //  01B5  GETMET	R8	R2	K6
      0x58280005,  //  01B6  LDCONST	R10	K5
      0x7C200400,  //  01B7  CALL	R8	2
      0x8C240341,  //  01B8  GETMET	R9	R1	K65
      0x5C2C1000,  //  01B9  MOVE	R11	R8
      0x7C240400,  //  01BA  CALL	R9	2
      0xB8260200,  //  01BB  GETNGBL	R9	K1
      0x8824132B,  //  01BC  GETMBR	R9	R9	K43
      0x900E2809,  //  01BD  SETMBR	R3	K20	R9
      0x4C240000,  //  01BE  LDNIL	R9
      0x80041200,  //  01BF  RET	1	R9
      0x7002002B,  //  01C0  JMP		#01ED
      0x54220009,  //  01C1  LDINT	R8	10
      0x1C200E08,  //  01C2  EQ	R8	R7	R8
      0x78220028,  //  01C3  JMPF	R8	#01ED
      0x8C200506,  //  01C4  GETMET	R8	R2	K6
      0x58280005,  //  01C5  LDCONST	R10	K5
      0x7C200400,  //  01C6  CALL	R8	2
      0x88240112,  //  01C7  GETMBR	R9	R0	K18
      0x88241342,  //  01C8  GETMBR	R9	R9	K66
      0x8C241343,  //  01C9  GETMET	R9	R9	K67
      0x7C240200,  //  01CA  CALL	R9	1
      0x28281107,  //  01CB  GE	R10	R8	K7
      0x782A001A,  //  01CC  JMPF	R10	#01E8
      0x6028000C,  //  01CD  GETGBL	R10	G12
      0x5C2C1200,  //  01CE  MOVE	R11	R9
      0x7C280200,  //  01CF  CALL	R10	1
      0x1828100A,  //  01D0  LE	R10	R8	R10
      0x782A0015,  //  01D1  JMPF	R10	#01E8
      0x04281107,  //  01D2  SUB	R10	R8	K7
      0x9428120A,  //  01D3  GETIDX	R10	R9	R10
      0xB82E3800,  //  01D4  GETNGBL	R11	K28
      0x8C2C1721,  //  01D5  GETMET	R11	R11	K33
      0x88340345,  //  01D6  GETMBR	R13	R1	K69
      0x8C341B3C,  //  01D7  GETMET	R13	R13	K60
      0x7C340200,  //  01D8  CALL	R13	1
      0x8C341B3D,  //  01D9  GETMET	R13	R13	K61
      0x7C340200,  //  01DA  CALL	R13	1
      0x8C341B23,  //  01DB  GETMET	R13	R13	K35
      0x7C340200,  //  01DC  CALL	R13	1
      0x0036880D,  //  01DD  ADD	R13	K68	R13
      0x7C2C0400,  //  01DE  CALL	R11	2
      0x882C0112,  //  01DF  GETMBR	R11	R0	K18
      0x882C1742,  //  01E0  GETMBR	R11	R11	K66
      0x8C2C1746,  //  01E1  GETMET	R11	R11	K70
      0x7C2C0200,  //  01E2  CALL	R11	1
      0x882C0112,  //  01E3  GETMBR	R11	R0	K18
      0x882C1742,  //  01E4  GETMBR	R11	R11	K66
      0x8C2C1747,  //  01E5  GETMET	R11	R11	K71
      0x7C2C0200,  //  01E6  CALL	R11	1
      0x7001FFFF,  //  01E7  JMP		#01E8
      0xB82A0200,  //  01E8  GETNGBL	R10	K1
      0x8828152B,  //  01E9  GETMBR	R10	R10	K43
      0x900E280A,  //  01EA  SETMBR	R3	K20	R10
      0x4C280000,  //  01EB  LDNIL	R10
      0x80041400,  //  01EC  RET	1	R10
      0x70020006,  //  01ED  JMP		#01F5
      0x54220029,  //  01EE  LDINT	R8	42
      0x1C200C08,  //  01EF  EQ	R8	R6	R8
      0x78220003,  //  01F0  JMPF	R8	#01F5
      0x1C200F05,  //  01F1  EQ	R8	R7	K5
      0x78220001,  //  01F2  JMPF	R8	#01F5
      0x50200200,  //  01F3  LDBOOL	R8	1	0
      0x80041000,  //  01F4  RET	1	R8
      0x80000000,  //  01F5  RET	0
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
