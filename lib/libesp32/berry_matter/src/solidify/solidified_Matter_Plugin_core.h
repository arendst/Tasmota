/* Solidification of Matter_Plugin_core.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_core;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_core_read_attribute,   /* name */
  be_nested_proto(
    24,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[76]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(create_TLV),
    /* K5   */  be_nested_str_weak(U8),
    /* K6   */  be_nested_str_weak(session),
    /* K7   */  be_nested_str_weak(breadcrumb),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(Matter_TLV_struct),
    /* K10  */  be_nested_str_weak(add_TLV),
    /* K11  */  be_nested_str_weak(U2),
    /* K12  */  be_const_int(2),
    /* K13  */  be_nested_str_weak(U1),
    /* K14  */  be_const_int(3),
    /* K15  */  be_nested_str_weak(BOOL),
    /* K16  */  be_nested_str_weak(Matter_TLV_array),
    /* K17  */  be_nested_str_weak(tasmota),
    /* K18  */  be_nested_str_weak(eth),
    /* K19  */  be_nested_str_weak(up),
    /* K20  */  be_nested_str_weak(add_struct),
    /* K21  */  be_nested_str_weak(UTF1),
    /* K22  */  be_nested_str_weak(ethernet),
    /* K23  */  be_nested_str_weak(NULL),
    /* K24  */  be_nested_str_weak(fromhex),
    /* K25  */  be_nested_str_weak(replace),
    /* K26  */  be_nested_str_weak(find),
    /* K27  */  be_nested_str_weak(mac),
    /* K28  */  be_nested_str_weak(),
    /* K29  */  be_nested_str_weak(_X3A),
    /* K30  */  be_nested_str_weak(B1),
    /* K31  */  be_nested_str_weak(add_array),
    /* K32  */  be_nested_str_weak(get_ip_bytes),
    /* K33  */  be_nested_str_weak(ip),
    /* K34  */  be_nested_str_weak(ip6local),
    /* K35  */  be_nested_str_weak(ip6),
    /* K36  */  be_nested_str_weak(wifi),
    /* K37  */  be_nested_str_weak(cmd),
    /* K38  */  be_nested_str_weak(Status_X201),
    /* K39  */  be_nested_str_weak(StatusPRM),
    /* K40  */  be_nested_str_weak(BootCount),
    /* K41  */  be_nested_str_weak(U4),
    /* K42  */  be_nested_str_weak(Status_X2011),
    /* K43  */  be_nested_str_weak(StatusSTS),
    /* K44  */  be_nested_str_weak(UptimeSec),
    /* K45  */  be_nested_str_weak(int64),
    /* K46  */  be_nested_str_weak(rtc),
    /* K47  */  be_nested_str_weak(utc),
    /* K48  */  be_const_int(1000000),
    /* K49  */  be_nested_str_weak(local),
    /* K50  */  be_nested_str_weak(device),
    /* K51  */  be_nested_str_weak(sessions),
    /* K52  */  be_nested_str_weak(sessions_active),
    /* K53  */  be_nested_str_weak(B2),
    /* K54  */  be_nested_str_weak(noc),
    /* K55  */  be_nested_str_weak(icac),
    /* K56  */  be_nested_str_weak(stop_iteration),
    /* K57  */  be_nested_str_weak(parse),
    /* K58  */  be_nested_str_weak(get_ca),
    /* K59  */  be_nested_str_weak(findsubval),
    /* K60  */  be_nested_str_weak(admin_vendor),
    /* K61  */  be_nested_str_weak(fabric),
    /* K62  */  be_nested_str_weak(deviceid),
    /* K63  */  be_nested_str_weak(fabric_label),
    /* K64  */  be_nested_str_weak(Tasmota),
    /* K65  */  be_nested_str_weak(vendorid),
    /* K66  */  be_nested_str_weak(DeviceName),
    /* K67  */  be_nested_str_weak(FriendlyName),
    /* K68  */  be_nested_str_weak(FriendlyName1),
    /* K69  */  be_nested_str_weak(XX),
    /* K70  */  be_nested_str_weak(Status_X202),
    /* K71  */  be_nested_str_weak(StatusFWR),
    /* K72  */  be_nested_str_weak(Hardware),
    /* K73  */  be_nested_str_weak(Version),
    /* K74  */  be_nested_str_weak(locale),
    /* K75  */  be_nested_str_weak(Matter_TLV_list),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[649]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0xB81A0200,  //  0001  GETNGBL	R6	K1
      0x88180D02,  //  0002  GETMBR	R6	R6	K2
      0x541E002F,  //  0003  LDINT	R7	48
      0x1C1C0607,  //  0004  EQ	R7	R3	R7
      0x781E0031,  //  0005  JMPF	R7	#0038
      0x1C1C0903,  //  0006  EQ	R7	R4	K3
      0x781E0006,  //  0007  JMPF	R7	#000F
      0x8C1C0D04,  //  0008  GETMET	R7	R6	K4
      0x88240D05,  //  0009  GETMBR	R9	R6	K5
      0x88280306,  //  000A  GETMBR	R10	R1	K6
      0x88281507,  //  000B  GETMBR	R10	R10	K7
      0x7C1C0600,  //  000C  CALL	R7	3
      0x80040E00,  //  000D  RET	1	R7
      0x70020027,  //  000E  JMP		#0037
      0x1C1C0908,  //  000F  EQ	R7	R4	K8
      0x781E000D,  //  0010  JMPF	R7	#001F
      0x8C1C0D09,  //  0011  GETMET	R7	R6	K9
      0x7C1C0200,  //  0012  CALL	R7	1
      0x8C200F0A,  //  0013  GETMET	R8	R7	K10
      0x58280003,  //  0014  LDCONST	R10	K3
      0x882C0D0B,  //  0015  GETMBR	R11	R6	K11
      0x5432003B,  //  0016  LDINT	R12	60
      0x7C200800,  //  0017  CALL	R8	4
      0x8C200F0A,  //  0018  GETMET	R8	R7	K10
      0x58280008,  //  0019  LDCONST	R10	K8
      0x882C0D0B,  //  001A  GETMBR	R11	R6	K11
      0x54320383,  //  001B  LDINT	R12	900
      0x7C200800,  //  001C  CALL	R8	4
      0x80040E00,  //  001D  RET	1	R7
      0x70020017,  //  001E  JMP		#0037
      0x1C1C090C,  //  001F  EQ	R7	R4	K12
      0x781E0005,  //  0020  JMPF	R7	#0027
      0x8C1C0D04,  //  0021  GETMET	R7	R6	K4
      0x88240D0D,  //  0022  GETMBR	R9	R6	K13
      0x5828000C,  //  0023  LDCONST	R10	K12
      0x7C1C0600,  //  0024  CALL	R7	3
      0x80040E00,  //  0025  RET	1	R7
      0x7002000F,  //  0026  JMP		#0037
      0x1C1C090E,  //  0027  EQ	R7	R4	K14
      0x781E0005,  //  0028  JMPF	R7	#002F
      0x8C1C0D04,  //  0029  GETMET	R7	R6	K4
      0x88240D0D,  //  002A  GETMBR	R9	R6	K13
      0x5828000C,  //  002B  LDCONST	R10	K12
      0x7C1C0600,  //  002C  CALL	R7	3
      0x80040E00,  //  002D  RET	1	R7
      0x70020007,  //  002E  JMP		#0037
      0x541E0003,  //  002F  LDINT	R7	4
      0x1C1C0807,  //  0030  EQ	R7	R4	R7
      0x781E0004,  //  0031  JMPF	R7	#0037
      0x8C1C0D04,  //  0032  GETMET	R7	R6	K4
      0x88240D0F,  //  0033  GETMBR	R9	R6	K15
      0x50280000,  //  0034  LDBOOL	R10	0	0
      0x7C1C0600,  //  0035  CALL	R7	3
      0x80040E00,  //  0036  RET	1	R7
      0x7002024F,  //  0037  JMP		#0288
      0x541E0031,  //  0038  LDINT	R7	50
      0x1C1C0607,  //  0039  EQ	R7	R3	R7
      0x781E0000,  //  003A  JMPF	R7	#003C
      0x7002024B,  //  003B  JMP		#0288
      0x541E0032,  //  003C  LDINT	R7	51
      0x1C1C0607,  //  003D  EQ	R7	R3	R7
      0x781E00DA,  //  003E  JMPF	R7	#011A
      0x1C1C0903,  //  003F  EQ	R7	R4	K3
      0x781E00B5,  //  0040  JMPF	R7	#00F7
      0x8C1C0D10,  //  0041  GETMET	R7	R6	K16
      0x7C1C0200,  //  0042  CALL	R7	1
      0xB8222200,  //  0043  GETNGBL	R8	K17
      0x8C201112,  //  0044  GETMET	R8	R8	K18
      0x7C200200,  //  0045  CALL	R8	1
      0x94241113,  //  0046  GETIDX	R9	R8	K19
      0x78260053,  //  0047  JMPF	R9	#009C
      0x8C240F14,  //  0048  GETMET	R9	R7	K20
      0x4C2C0000,  //  0049  LDNIL	R11
      0x7C240400,  //  004A  CALL	R9	2
      0x8C28130A,  //  004B  GETMET	R10	R9	K10
      0x58300003,  //  004C  LDCONST	R12	K3
      0x88340D15,  //  004D  GETMBR	R13	R6	K21
      0x58380016,  //  004E  LDCONST	R14	K22
      0x7C280800,  //  004F  CALL	R10	4
      0x8C28130A,  //  0050  GETMET	R10	R9	K10
      0x58300008,  //  0051  LDCONST	R12	K8
      0x88340D0F,  //  0052  GETMBR	R13	R6	K15
      0x58380008,  //  0053  LDCONST	R14	K8
      0x7C280800,  //  0054  CALL	R10	4
      0x8C28130A,  //  0055  GETMET	R10	R9	K10
      0x5830000C,  //  0056  LDCONST	R12	K12
      0x88340D0F,  //  0057  GETMBR	R13	R6	K15
      0x58380008,  //  0058  LDCONST	R14	K8
      0x7C280800,  //  0059  CALL	R10	4
      0x8C28130A,  //  005A  GETMET	R10	R9	K10
      0x5830000E,  //  005B  LDCONST	R12	K14
      0x88340D17,  //  005C  GETMBR	R13	R6	K23
      0x4C380000,  //  005D  LDNIL	R14
      0x7C280800,  //  005E  CALL	R10	4
      0x60280015,  //  005F  GETGBL	R10	G21
      0x7C280000,  //  0060  CALL	R10	0
      0x8C281518,  //  0061  GETMET	R10	R10	K24
      0x8C300B19,  //  0062  GETMET	R12	R5	K25
      0x8C38111A,  //  0063  GETMET	R14	R8	K26
      0x5840001B,  //  0064  LDCONST	R16	K27
      0x5844001C,  //  0065  LDCONST	R17	K28
      0x7C380600,  //  0066  CALL	R14	3
      0x583C001D,  //  0067  LDCONST	R15	K29
      0x5840001C,  //  0068  LDCONST	R16	K28
      0x7C300800,  //  0069  CALL	R12	4
      0x7C280400,  //  006A  CALL	R10	2
      0x8C2C130A,  //  006B  GETMET	R11	R9	K10
      0x54360003,  //  006C  LDINT	R13	4
      0x88380D1E,  //  006D  GETMBR	R14	R6	K30
      0x5C3C1400,  //  006E  MOVE	R15	R10
      0x7C2C0800,  //  006F  CALL	R11	4
      0x8C2C131F,  //  0070  GETMET	R11	R9	K31
      0x54360004,  //  0071  LDINT	R13	5
      0x7C2C0400,  //  0072  CALL	R11	2
      0x8C30170A,  //  0073  GETMET	R12	R11	K10
      0x4C380000,  //  0074  LDNIL	R14
      0x883C0D1E,  //  0075  GETMBR	R15	R6	K30
      0xB8420200,  //  0076  GETNGBL	R16	K1
      0x8C402120,  //  0077  GETMET	R16	R16	K32
      0x8C48111A,  //  0078  GETMET	R18	R8	K26
      0x58500021,  //  0079  LDCONST	R20	K33
      0x5854001C,  //  007A  LDCONST	R21	K28
      0x7C480600,  //  007B  CALL	R18	3
      0x7C400400,  //  007C  CALL	R16	2
      0x7C300800,  //  007D  CALL	R12	4
      0x8C30131F,  //  007E  GETMET	R12	R9	K31
      0x543A0005,  //  007F  LDINT	R14	6
      0x7C300400,  //  0080  CALL	R12	2
      0x8C34190A,  //  0081  GETMET	R13	R12	K10
      0x4C3C0000,  //  0082  LDNIL	R15
      0x88400D1E,  //  0083  GETMBR	R16	R6	K30
      0xB8460200,  //  0084  GETNGBL	R17	K1
      0x8C442320,  //  0085  GETMET	R17	R17	K32
      0x8C4C111A,  //  0086  GETMET	R19	R8	K26
      0x58540022,  //  0087  LDCONST	R21	K34
      0x5858001C,  //  0088  LDCONST	R22	K28
      0x7C4C0600,  //  0089  CALL	R19	3
      0x7C440400,  //  008A  CALL	R17	2
      0x7C340800,  //  008B  CALL	R13	4
      0x8C34190A,  //  008C  GETMET	R13	R12	K10
      0x4C3C0000,  //  008D  LDNIL	R15
      0x88400D1E,  //  008E  GETMBR	R16	R6	K30
      0xB8460200,  //  008F  GETNGBL	R17	K1
      0x8C442320,  //  0090  GETMET	R17	R17	K32
      0x8C4C111A,  //  0091  GETMET	R19	R8	K26
      0x58540023,  //  0092  LDCONST	R21	K35
      0x5858001C,  //  0093  LDCONST	R22	K28
      0x7C4C0600,  //  0094  CALL	R19	3
      0x7C440400,  //  0095  CALL	R17	2
      0x7C340800,  //  0096  CALL	R13	4
      0x8C34130A,  //  0097  GETMET	R13	R9	K10
      0x543E0006,  //  0098  LDINT	R15	7
      0x88400D0D,  //  0099  GETMBR	R16	R6	K13
      0x5844000C,  //  009A  LDCONST	R17	K12
      0x7C340800,  //  009B  CALL	R13	4
      0xB8262200,  //  009C  GETNGBL	R9	K17
      0x8C241324,  //  009D  GETMET	R9	R9	K36
      0x7C240200,  //  009E  CALL	R9	1
      0x94281313,  //  009F  GETIDX	R10	R9	K19
      0x782A0053,  //  00A0  JMPF	R10	#00F5
      0x8C280F14,  //  00A1  GETMET	R10	R7	K20
      0x4C300000,  //  00A2  LDNIL	R12
      0x7C280400,  //  00A3  CALL	R10	2
      0x8C2C150A,  //  00A4  GETMET	R11	R10	K10
      0x58340003,  //  00A5  LDCONST	R13	K3
      0x88380D15,  //  00A6  GETMBR	R14	R6	K21
      0x583C0024,  //  00A7  LDCONST	R15	K36
      0x7C2C0800,  //  00A8  CALL	R11	4
      0x8C2C150A,  //  00A9  GETMET	R11	R10	K10
      0x58340008,  //  00AA  LDCONST	R13	K8
      0x88380D0F,  //  00AB  GETMBR	R14	R6	K15
      0x583C0008,  //  00AC  LDCONST	R15	K8
      0x7C2C0800,  //  00AD  CALL	R11	4
      0x8C2C150A,  //  00AE  GETMET	R11	R10	K10
      0x5834000C,  //  00AF  LDCONST	R13	K12
      0x88380D0F,  //  00B0  GETMBR	R14	R6	K15
      0x583C0008,  //  00B1  LDCONST	R15	K8
      0x7C2C0800,  //  00B2  CALL	R11	4
      0x8C2C150A,  //  00B3  GETMET	R11	R10	K10
      0x5834000E,  //  00B4  LDCONST	R13	K14
      0x88380D17,  //  00B5  GETMBR	R14	R6	K23
      0x4C3C0000,  //  00B6  LDNIL	R15
      0x7C2C0800,  //  00B7  CALL	R11	4
      0x602C0015,  //  00B8  GETGBL	R11	G21
      0x7C2C0000,  //  00B9  CALL	R11	0
      0x8C2C1718,  //  00BA  GETMET	R11	R11	K24
      0x8C340B19,  //  00BB  GETMET	R13	R5	K25
      0x8C3C131A,  //  00BC  GETMET	R15	R9	K26
      0x5844001B,  //  00BD  LDCONST	R17	K27
      0x5848001C,  //  00BE  LDCONST	R18	K28
      0x7C3C0600,  //  00BF  CALL	R15	3
      0x5840001D,  //  00C0  LDCONST	R16	K29
      0x5844001C,  //  00C1  LDCONST	R17	K28
      0x7C340800,  //  00C2  CALL	R13	4
      0x7C2C0400,  //  00C3  CALL	R11	2
      0x8C30150A,  //  00C4  GETMET	R12	R10	K10
      0x543A0003,  //  00C5  LDINT	R14	4
      0x883C0D1E,  //  00C6  GETMBR	R15	R6	K30
      0x5C401600,  //  00C7  MOVE	R16	R11
      0x7C300800,  //  00C8  CALL	R12	4
      0x8C30151F,  //  00C9  GETMET	R12	R10	K31
      0x543A0004,  //  00CA  LDINT	R14	5
      0x7C300400,  //  00CB  CALL	R12	2
      0x8C34190A,  //  00CC  GETMET	R13	R12	K10
      0x4C3C0000,  //  00CD  LDNIL	R15
      0x88400D1E,  //  00CE  GETMBR	R16	R6	K30
      0xB8460200,  //  00CF  GETNGBL	R17	K1
      0x8C442320,  //  00D0  GETMET	R17	R17	K32
      0x8C4C131A,  //  00D1  GETMET	R19	R9	K26
      0x58540021,  //  00D2  LDCONST	R21	K33
      0x5858001C,  //  00D3  LDCONST	R22	K28
      0x7C4C0600,  //  00D4  CALL	R19	3
      0x7C440400,  //  00D5  CALL	R17	2
      0x7C340800,  //  00D6  CALL	R13	4
      0x8C34151F,  //  00D7  GETMET	R13	R10	K31
      0x543E0005,  //  00D8  LDINT	R15	6
      0x7C340400,  //  00D9  CALL	R13	2
      0x8C381B0A,  //  00DA  GETMET	R14	R13	K10
      0x4C400000,  //  00DB  LDNIL	R16
      0x88440D1E,  //  00DC  GETMBR	R17	R6	K30
      0xB84A0200,  //  00DD  GETNGBL	R18	K1
      0x8C482520,  //  00DE  GETMET	R18	R18	K32
      0x8C50131A,  //  00DF  GETMET	R20	R9	K26
      0x58580022,  //  00E0  LDCONST	R22	K34
      0x585C001C,  //  00E1  LDCONST	R23	K28
      0x7C500600,  //  00E2  CALL	R20	3
      0x7C480400,  //  00E3  CALL	R18	2
      0x7C380800,  //  00E4  CALL	R14	4
      0x8C381B0A,  //  00E5  GETMET	R14	R13	K10
      0x4C400000,  //  00E6  LDNIL	R16
      0x88440D1E,  //  00E7  GETMBR	R17	R6	K30
      0xB84A0200,  //  00E8  GETNGBL	R18	K1
      0x8C482520,  //  00E9  GETMET	R18	R18	K32
      0x8C50131A,  //  00EA  GETMET	R20	R9	K26
      0x58580023,  //  00EB  LDCONST	R22	K35
      0x585C001C,  //  00EC  LDCONST	R23	K28
      0x7C500600,  //  00ED  CALL	R20	3
      0x7C480400,  //  00EE  CALL	R18	2
      0x7C380800,  //  00EF  CALL	R14	4
      0x8C38150A,  //  00F0  GETMET	R14	R10	K10
      0x54420006,  //  00F1  LDINT	R16	7
      0x88440D0D,  //  00F2  GETMBR	R17	R6	K13
      0x58480008,  //  00F3  LDCONST	R18	K8
      0x7C380800,  //  00F4  CALL	R14	4
      0x80040E00,  //  00F5  RET	1	R7
      0x70020021,  //  00F6  JMP		#0119
      0x1C1C0908,  //  00F7  EQ	R7	R4	K8
      0x781E000A,  //  00F8  JMPF	R7	#0104
      0x8C1C0D04,  //  00F9  GETMET	R7	R6	K4
      0x88240D0B,  //  00FA  GETMBR	R9	R6	K11
      0xB82A2200,  //  00FB  GETNGBL	R10	K17
      0x8C281525,  //  00FC  GETMET	R10	R10	K37
      0x58300026,  //  00FD  LDCONST	R12	K38
      0x7C280400,  //  00FE  CALL	R10	2
      0x94281527,  //  00FF  GETIDX	R10	R10	K39
      0x94281528,  //  0100  GETIDX	R10	R10	K40
      0x7C1C0600,  //  0101  CALL	R7	3
      0x80040E00,  //  0102  RET	1	R7
      0x70020014,  //  0103  JMP		#0119
      0x1C1C090C,  //  0104  EQ	R7	R4	K12
      0x781E000A,  //  0105  JMPF	R7	#0111
      0x8C1C0D04,  //  0106  GETMET	R7	R6	K4
      0x88240D29,  //  0107  GETMBR	R9	R6	K41
      0xB82A2200,  //  0108  GETNGBL	R10	K17
      0x8C281525,  //  0109  GETMET	R10	R10	K37
      0x5830002A,  //  010A  LDCONST	R12	K42
      0x7C280400,  //  010B  CALL	R10	2
      0x9428152B,  //  010C  GETIDX	R10	R10	K43
      0x9428152C,  //  010D  GETIDX	R10	R10	K44
      0x7C1C0600,  //  010E  CALL	R7	3
      0x80040E00,  //  010F  RET	1	R7
      0x70020007,  //  0110  JMP		#0119
      0x541E0007,  //  0111  LDINT	R7	8
      0x1C1C0807,  //  0112  EQ	R7	R4	R7
      0x781E0004,  //  0113  JMPF	R7	#0119
      0x8C1C0D04,  //  0114  GETMET	R7	R6	K4
      0x88240D0F,  //  0115  GETMBR	R9	R6	K15
      0x50280000,  //  0116  LDBOOL	R10	0	0
      0x7C1C0600,  //  0117  CALL	R7	3
      0x80040E00,  //  0118  RET	1	R7
      0x7002016D,  //  0119  JMP		#0288
      0x541E0033,  //  011A  LDINT	R7	52
      0x1C1C0607,  //  011B  EQ	R7	R3	R7
      0x781E0000,  //  011C  JMPF	R7	#011E
      0x70020169,  //  011D  JMP		#0288
      0x541E0037,  //  011E  LDINT	R7	56
      0x1C1C0607,  //  011F  EQ	R7	R3	R7
      0x781E002C,  //  0120  JMPF	R7	#014E
      0x1C1C0903,  //  0121  EQ	R7	R4	K3
      0x781E000F,  //  0122  JMPF	R7	#0133
      0xB81E5A00,  //  0123  GETNGBL	R7	K45
      0xB8222200,  //  0124  GETNGBL	R8	K17
      0x8C20112E,  //  0125  GETMET	R8	R8	K46
      0x7C200200,  //  0126  CALL	R8	1
      0x9420112F,  //  0127  GETIDX	R8	R8	K47
      0x7C1C0200,  //  0128  CALL	R7	1
      0xB8225A00,  //  0129  GETNGBL	R8	K45
      0x58240030,  //  012A  LDCONST	R9	K48
      0x7C200200,  //  012B  CALL	R8	1
      0x081C0E08,  //  012C  MUL	R7	R7	R8
      0x8C200D04,  //  012D  GETMET	R8	R6	K4
      0x88280D05,  //  012E  GETMBR	R10	R6	K5
      0x5C2C0E00,  //  012F  MOVE	R11	R7
      0x7C200600,  //  0130  CALL	R8	3
      0x80041000,  //  0131  RET	1	R8
      0x70020019,  //  0132  JMP		#014D
      0x1C1C0908,  //  0133  EQ	R7	R4	K8
      0x781E0005,  //  0134  JMPF	R7	#013B
      0x8C1C0D04,  //  0135  GETMET	R7	R6	K4
      0x88240D0D,  //  0136  GETMBR	R9	R6	K13
      0x5828000E,  //  0137  LDCONST	R10	K14
      0x7C1C0600,  //  0138  CALL	R7	3
      0x80040E00,  //  0139  RET	1	R7
      0x70020011,  //  013A  JMP		#014D
      0x541E0006,  //  013B  LDINT	R7	7
      0x1C1C0807,  //  013C  EQ	R7	R4	R7
      0x781E000E,  //  013D  JMPF	R7	#014D
      0xB81E5A00,  //  013E  GETNGBL	R7	K45
      0xB8222200,  //  013F  GETNGBL	R8	K17
      0x8C20112E,  //  0140  GETMET	R8	R8	K46
      0x7C200200,  //  0141  CALL	R8	1
      0x94201131,  //  0142  GETIDX	R8	R8	K49
      0x7C1C0200,  //  0143  CALL	R7	1
      0xB8225A00,  //  0144  GETNGBL	R8	K45
      0x58240030,  //  0145  LDCONST	R9	K48
      0x7C200200,  //  0146  CALL	R8	1
      0x081C0E08,  //  0147  MUL	R7	R7	R8
      0x8C200D04,  //  0148  GETMET	R8	R6	K4
      0x88280D05,  //  0149  GETMBR	R10	R6	K5
      0x5C2C0E00,  //  014A  MOVE	R11	R7
      0x7C200600,  //  014B  CALL	R8	3
      0x80041000,  //  014C  RET	1	R8
      0x70020139,  //  014D  JMP		#0288
      0x541E003D,  //  014E  LDINT	R7	62
      0x1C1C0607,  //  014F  EQ	R7	R3	R7
      0x781E006C,  //  0150  JMPF	R7	#01BE
      0x1C1C0903,  //  0151  EQ	R7	R4	K3
      0x781E001D,  //  0152  JMPF	R7	#0171
      0x8C1C0D10,  //  0153  GETMET	R7	R6	K16
      0x7C1C0200,  //  0154  CALL	R7	1
      0x60200010,  //  0155  GETGBL	R8	G16
      0x88240132,  //  0156  GETMBR	R9	R0	K50
      0x88241333,  //  0157  GETMBR	R9	R9	K51
      0x8C241334,  //  0158  GETMET	R9	R9	K52
      0x7C240200,  //  0159  CALL	R9	1
      0x7C200200,  //  015A  CALL	R8	1
      0xA802000F,  //  015B  EXBLK	0	#016C
      0x5C241000,  //  015C  MOVE	R9	R8
      0x7C240000,  //  015D  CALL	R9	0
      0x8C280F14,  //  015E  GETMET	R10	R7	K20
      0x4C300000,  //  015F  LDNIL	R12
      0x7C280400,  //  0160  CALL	R10	2
      0x8C2C150A,  //  0161  GETMET	R11	R10	K10
      0x58340008,  //  0162  LDCONST	R13	K8
      0x88380D35,  //  0163  GETMBR	R14	R6	K53
      0x883C1336,  //  0164  GETMBR	R15	R9	K54
      0x7C2C0800,  //  0165  CALL	R11	4
      0x8C2C150A,  //  0166  GETMET	R11	R10	K10
      0x5834000C,  //  0167  LDCONST	R13	K12
      0x88380D35,  //  0168  GETMBR	R14	R6	K53
      0x883C1337,  //  0169  GETMBR	R15	R9	K55
      0x7C2C0800,  //  016A  CALL	R11	4
      0x7001FFEF,  //  016B  JMP		#015C
      0x58200038,  //  016C  LDCONST	R8	K56
      0xAC200200,  //  016D  CATCH	R8	1	0
      0xB0080000,  //  016E  RAISE	2	R0	R0
      0x80040E00,  //  016F  RET	1	R7
      0x7002004B,  //  0170  JMP		#01BD
      0x1C1C0908,  //  0171  EQ	R7	R4	K8
      0x781E0032,  //  0172  JMPF	R7	#01A6
      0x8C1C0D10,  //  0173  GETMET	R7	R6	K16
      0x7C1C0200,  //  0174  CALL	R7	1
      0x60200010,  //  0175  GETGBL	R8	G16
      0x88240132,  //  0176  GETMBR	R9	R0	K50
      0x88241333,  //  0177  GETMBR	R9	R9	K51
      0x8C241334,  //  0178  GETMET	R9	R9	K52
      0x7C240200,  //  0179  CALL	R9	1
      0x7C200200,  //  017A  CALL	R8	1
      0xA8020024,  //  017B  EXBLK	0	#01A1
      0x5C241000,  //  017C  MOVE	R9	R8
      0x7C240000,  //  017D  CALL	R9	0
      0x8C280D39,  //  017E  GETMET	R10	R6	K57
      0x8C30133A,  //  017F  GETMET	R12	R9	K58
      0x7C300200,  //  0180  CALL	R12	1
      0x7C280400,  //  0181  CALL	R10	2
      0x8C2C0F14,  //  0182  GETMET	R11	R7	K20
      0x4C340000,  //  0183  LDNIL	R13
      0x7C2C0400,  //  0184  CALL	R11	2
      0x8C30170A,  //  0185  GETMET	R12	R11	K10
      0x58380008,  //  0186  LDCONST	R14	K8
      0x883C0D35,  //  0187  GETMBR	R15	R6	K53
      0x8C40153B,  //  0188  GETMET	R16	R10	K59
      0x544A0008,  //  0189  LDINT	R18	9
      0x7C400400,  //  018A  CALL	R16	2
      0x7C300800,  //  018B  CALL	R12	4
      0x8C30170A,  //  018C  GETMET	R12	R11	K10
      0x5838000C,  //  018D  LDCONST	R14	K12
      0x883C0D0B,  //  018E  GETMBR	R15	R6	K11
      0x8840133C,  //  018F  GETMBR	R16	R9	K60
      0x7C300800,  //  0190  CALL	R12	4
      0x8C30170A,  //  0191  GETMET	R12	R11	K10
      0x5838000E,  //  0192  LDCONST	R14	K14
      0x883C0D05,  //  0193  GETMBR	R15	R6	K5
      0x8840133D,  //  0194  GETMBR	R16	R9	K61
      0x7C300800,  //  0195  CALL	R12	4
      0x8C30170A,  //  0196  GETMET	R12	R11	K10
      0x543A0003,  //  0197  LDINT	R14	4
      0x883C0D05,  //  0198  GETMBR	R15	R6	K5
      0x8840133E,  //  0199  GETMBR	R16	R9	K62
      0x7C300800,  //  019A  CALL	R12	4
      0x8C30170A,  //  019B  GETMET	R12	R11	K10
      0x543A0004,  //  019C  LDINT	R14	5
      0x883C0D15,  //  019D  GETMBR	R15	R6	K21
      0x8840133F,  //  019E  GETMBR	R16	R9	K63
      0x7C300800,  //  019F  CALL	R12	4
      0x7001FFDA,  //  01A0  JMP		#017C
      0x58200038,  //  01A1  LDCONST	R8	K56
      0xAC200200,  //  01A2  CATCH	R8	1	0
      0xB0080000,  //  01A3  RAISE	2	R0	R0
      0x80040E00,  //  01A4  RET	1	R7
      0x70020016,  //  01A5  JMP		#01BD
      0x1C1C090C,  //  01A6  EQ	R7	R4	K12
      0x781E0005,  //  01A7  JMPF	R7	#01AE
      0x8C1C0D04,  //  01A8  GETMET	R7	R6	K4
      0x88240D0D,  //  01A9  GETMBR	R9	R6	K13
      0x542A0004,  //  01AA  LDINT	R10	5
      0x7C1C0600,  //  01AB  CALL	R7	3
      0x80040E00,  //  01AC  RET	1	R7
      0x7002000E,  //  01AD  JMP		#01BD
      0x1C1C090E,  //  01AE  EQ	R7	R4	K14
      0x781E0005,  //  01AF  JMPF	R7	#01B6
      0x8C1C0D04,  //  01B0  GETMET	R7	R6	K4
      0x88240D0D,  //  01B1  GETMBR	R9	R6	K13
      0x58280008,  //  01B2  LDCONST	R10	K8
      0x7C1C0600,  //  01B3  CALL	R7	3
      0x80040E00,  //  01B4  RET	1	R7
      0x70020006,  //  01B5  JMP		#01BD
      0x541E0003,  //  01B6  LDINT	R7	4
      0x1C1C0807,  //  01B7  EQ	R7	R4	R7
      0x781E0000,  //  01B8  JMPF	R7	#01BA
      0x70020002,  //  01B9  JMP		#01BD
      0x541E0004,  //  01BA  LDINT	R7	5
      0x1C1C0807,  //  01BB  EQ	R7	R4	R7
      0x781DFFFF,  //  01BC  JMPF	R7	#01BD
      0x700200C9,  //  01BD  JMP		#0288
      0x541E003B,  //  01BE  LDINT	R7	60
      0x1C1C0607,  //  01BF  EQ	R7	R3	R7
      0x781E0000,  //  01C0  JMPF	R7	#01C2
      0x700200C5,  //  01C1  JMP		#0288
      0x541E0027,  //  01C2  LDINT	R7	40
      0x1C1C0607,  //  01C3  EQ	R7	R3	R7
      0x781E0071,  //  01C4  JMPF	R7	#0237
      0x1C1C0903,  //  01C5  EQ	R7	R4	K3
      0x781E0005,  //  01C6  JMPF	R7	#01CD
      0x8C1C0D04,  //  01C7  GETMET	R7	R6	K4
      0x88240D0B,  //  01C8  GETMBR	R9	R6	K11
      0x58280003,  //  01C9  LDCONST	R10	K3
      0x7C1C0600,  //  01CA  CALL	R7	3
      0x80040E00,  //  01CB  RET	1	R7
      0x70020068,  //  01CC  JMP		#0236
      0x1C1C0908,  //  01CD  EQ	R7	R4	K8
      0x781E0005,  //  01CE  JMPF	R7	#01D5
      0x8C1C0D04,  //  01CF  GETMET	R7	R6	K4
      0x88240D15,  //  01D0  GETMBR	R9	R6	K21
      0x58280040,  //  01D1  LDCONST	R10	K64
      0x7C1C0600,  //  01D2  CALL	R7	3
      0x80040E00,  //  01D3  RET	1	R7
      0x70020060,  //  01D4  JMP		#0236
      0x1C1C090C,  //  01D5  EQ	R7	R4	K12
      0x781E0006,  //  01D6  JMPF	R7	#01DE
      0x8C1C0D04,  //  01D7  GETMET	R7	R6	K4
      0x88240D0B,  //  01D8  GETMBR	R9	R6	K11
      0x88280132,  //  01D9  GETMBR	R10	R0	K50
      0x88281541,  //  01DA  GETMBR	R10	R10	K65
      0x7C1C0600,  //  01DB  CALL	R7	3
      0x80040E00,  //  01DC  RET	1	R7
      0x70020057,  //  01DD  JMP		#0236
      0x1C1C090E,  //  01DE  EQ	R7	R4	K14
      0x781E0009,  //  01DF  JMPF	R7	#01EA
      0x8C1C0D04,  //  01E0  GETMET	R7	R6	K4
      0x88240D15,  //  01E1  GETMBR	R9	R6	K21
      0xB82A2200,  //  01E2  GETNGBL	R10	K17
      0x8C281525,  //  01E3  GETMET	R10	R10	K37
      0x58300042,  //  01E4  LDCONST	R12	K66
      0x7C280400,  //  01E5  CALL	R10	2
      0x94281542,  //  01E6  GETIDX	R10	R10	K66
      0x7C1C0600,  //  01E7  CALL	R7	3
      0x80040E00,  //  01E8  RET	1	R7
      0x7002004B,  //  01E9  JMP		#0236
      0x541E0003,  //  01EA  LDINT	R7	4
      0x1C1C0807,  //  01EB  EQ	R7	R4	R7
      0x781E0005,  //  01EC  JMPF	R7	#01F3
      0x8C1C0D04,  //  01ED  GETMET	R7	R6	K4
      0x88240D0B,  //  01EE  GETMBR	R9	R6	K11
      0x542A7FFF,  //  01EF  LDINT	R10	32768
      0x7C1C0600,  //  01F0  CALL	R7	3
      0x80040E00,  //  01F1  RET	1	R7
      0x70020042,  //  01F2  JMP		#0236
      0x541E0004,  //  01F3  LDINT	R7	5
      0x1C1C0807,  //  01F4  EQ	R7	R4	R7
      0x781E0009,  //  01F5  JMPF	R7	#0200
      0x8C1C0D04,  //  01F6  GETMET	R7	R6	K4
      0x88240D15,  //  01F7  GETMBR	R9	R6	K21
      0xB82A2200,  //  01F8  GETNGBL	R10	K17
      0x8C281525,  //  01F9  GETMET	R10	R10	K37
      0x58300043,  //  01FA  LDCONST	R12	K67
      0x7C280400,  //  01FB  CALL	R10	2
      0x94281544,  //  01FC  GETIDX	R10	R10	K68
      0x7C1C0600,  //  01FD  CALL	R7	3
      0x80040E00,  //  01FE  RET	1	R7
      0x70020035,  //  01FF  JMP		#0236
      0x541E0005,  //  0200  LDINT	R7	6
      0x1C1C0807,  //  0201  EQ	R7	R4	R7
      0x781E0005,  //  0202  JMPF	R7	#0209
      0x8C1C0D04,  //  0203  GETMET	R7	R6	K4
      0x88240D15,  //  0204  GETMBR	R9	R6	K21
      0x58280045,  //  0205  LDCONST	R10	K69
      0x7C1C0600,  //  0206  CALL	R7	3
      0x80040E00,  //  0207  RET	1	R7
      0x7002002C,  //  0208  JMP		#0236
      0x541E0006,  //  0209  LDINT	R7	7
      0x1C1C0807,  //  020A  EQ	R7	R4	R7
      0x781E0005,  //  020B  JMPF	R7	#0212
      0x8C1C0D04,  //  020C  GETMET	R7	R6	K4
      0x88240D0B,  //  020D  GETMBR	R9	R6	K11
      0x58280003,  //  020E  LDCONST	R10	K3
      0x7C1C0600,  //  020F  CALL	R7	3
      0x80040E00,  //  0210  RET	1	R7
      0x70020023,  //  0211  JMP		#0236
      0x541E0007,  //  0212  LDINT	R7	8
      0x1C1C0807,  //  0213  EQ	R7	R4	R7
      0x781E000A,  //  0214  JMPF	R7	#0220
      0x8C1C0D04,  //  0215  GETMET	R7	R6	K4
      0x88240D15,  //  0216  GETMBR	R9	R6	K21
      0xB82A2200,  //  0217  GETNGBL	R10	K17
      0x8C281525,  //  0218  GETMET	R10	R10	K37
      0x58300046,  //  0219  LDCONST	R12	K70
      0x7C280400,  //  021A  CALL	R10	2
      0x94281547,  //  021B  GETIDX	R10	R10	K71
      0x94281548,  //  021C  GETIDX	R10	R10	K72
      0x7C1C0600,  //  021D  CALL	R7	3
      0x80040E00,  //  021E  RET	1	R7
      0x70020015,  //  021F  JMP		#0236
      0x541E0008,  //  0220  LDINT	R7	9
      0x1C1C0807,  //  0221  EQ	R7	R4	R7
      0x781E0005,  //  0222  JMPF	R7	#0229
      0x8C1C0D04,  //  0223  GETMET	R7	R6	K4
      0x88240D0B,  //  0224  GETMBR	R9	R6	K11
      0x58280003,  //  0225  LDCONST	R10	K3
      0x7C1C0600,  //  0226  CALL	R7	3
      0x80040E00,  //  0227  RET	1	R7
      0x7002000C,  //  0228  JMP		#0236
      0x541E0009,  //  0229  LDINT	R7	10
      0x1C1C0807,  //  022A  EQ	R7	R4	R7
      0x781E0009,  //  022B  JMPF	R7	#0236
      0x8C1C0D04,  //  022C  GETMET	R7	R6	K4
      0x88240D15,  //  022D  GETMBR	R9	R6	K21
      0xB82A2200,  //  022E  GETNGBL	R10	K17
      0x8C281525,  //  022F  GETMET	R10	R10	K37
      0x58300046,  //  0230  LDCONST	R12	K70
      0x7C280400,  //  0231  CALL	R10	2
      0x94281547,  //  0232  GETIDX	R10	R10	K71
      0x94281549,  //  0233  GETIDX	R10	R10	K73
      0x7C1C0600,  //  0234  CALL	R7	3
      0x80040E00,  //  0235  RET	1	R7
      0x70020050,  //  0236  JMP		#0288
      0x541E003E,  //  0237  LDINT	R7	63
      0x1C1C0607,  //  0238  EQ	R7	R3	R7
      0x781E0000,  //  0239  JMPF	R7	#023B
      0x7002004C,  //  023A  JMP		#0288
      0x541E002A,  //  023B  LDINT	R7	43
      0x1C1C0607,  //  023C  EQ	R7	R3	R7
      0x781E0016,  //  023D  JMPF	R7	#0255
      0x1C1C0903,  //  023E  EQ	R7	R4	K3
      0x781E0007,  //  023F  JMPF	R7	#0248
      0x8C1C0D04,  //  0240  GETMET	R7	R6	K4
      0x88240D15,  //  0241  GETMBR	R9	R6	K21
      0xB82A2200,  //  0242  GETNGBL	R10	K17
      0x8C28154A,  //  0243  GETMET	R10	R10	K74
      0x7C280200,  //  0244  CALL	R10	1
      0x7C1C0600,  //  0245  CALL	R7	3
      0x80040E00,  //  0246  RET	1	R7
      0x7002000B,  //  0247  JMP		#0254
      0x1C1C0908,  //  0248  EQ	R7	R4	K8
      0x781E0009,  //  0249  JMPF	R7	#0254
      0x8C1C0D4B,  //  024A  GETMET	R7	R6	K75
      0x7C1C0200,  //  024B  CALL	R7	1
      0x8C200F0A,  //  024C  GETMET	R8	R7	K10
      0x4C280000,  //  024D  LDNIL	R10
      0x882C0D15,  //  024E  GETMBR	R11	R6	K21
      0xB8322200,  //  024F  GETNGBL	R12	K17
      0x8C30194A,  //  0250  GETMET	R12	R12	K74
      0x7C300200,  //  0251  CALL	R12	1
      0x7C200800,  //  0252  CALL	R8	4
      0x80040E00,  //  0253  RET	1	R7
      0x70020032,  //  0254  JMP		#0288
      0x541E002B,  //  0255  LDINT	R7	44
      0x1C1C0607,  //  0256  EQ	R7	R3	R7
      0x781E001C,  //  0257  JMPF	R7	#0275
      0x1C1C0903,  //  0258  EQ	R7	R4	K3
      0x781E0005,  //  0259  JMPF	R7	#0260
      0x8C1C0D04,  //  025A  GETMET	R7	R6	K4
      0x88240D0D,  //  025B  GETMBR	R9	R6	K13
      0x58280008,  //  025C  LDCONST	R10	K8
      0x7C1C0600,  //  025D  CALL	R7	3
      0x80040E00,  //  025E  RET	1	R7
      0x70020013,  //  025F  JMP		#0274
      0x1C1C0908,  //  0260  EQ	R7	R4	K8
      0x781E0005,  //  0261  JMPF	R7	#0268
      0x8C1C0D04,  //  0262  GETMET	R7	R6	K4
      0x88240D0D,  //  0263  GETMBR	R9	R6	K13
      0x542A0003,  //  0264  LDINT	R10	4
      0x7C1C0600,  //  0265  CALL	R7	3
      0x80040E00,  //  0266  RET	1	R7
      0x7002000B,  //  0267  JMP		#0274
      0x1C1C090C,  //  0268  EQ	R7	R4	K12
      0x781E0009,  //  0269  JMPF	R7	#0274
      0x8C1C0D4B,  //  026A  GETMET	R7	R6	K75
      0x7C1C0200,  //  026B  CALL	R7	1
      0x8C200F0A,  //  026C  GETMET	R8	R7	K10
      0x4C280000,  //  026D  LDNIL	R10
      0x8C2C0D04,  //  026E  GETMET	R11	R6	K4
      0x88340D0D,  //  026F  GETMBR	R13	R6	K13
      0x543A0003,  //  0270  LDINT	R14	4
      0x7C2C0600,  //  0271  CALL	R11	3
      0x7C200600,  //  0272  CALL	R8	3
      0x80040E00,  //  0273  RET	1	R7
      0x70020012,  //  0274  JMP		#0288
      0x541E0030,  //  0275  LDINT	R7	49
      0x1C1C0607,  //  0276  EQ	R7	R3	R7
      0x781E000F,  //  0277  JMPF	R7	#0288
      0x1C1C090E,  //  0278  EQ	R7	R4	K14
      0x781E0005,  //  0279  JMPF	R7	#0280
      0x8C1C0D04,  //  027A  GETMET	R7	R6	K4
      0x88240D0D,  //  027B  GETMBR	R9	R6	K13
      0x542A001D,  //  027C  LDINT	R10	30
      0x7C1C0600,  //  027D  CALL	R7	3
      0x80040E00,  //  027E  RET	1	R7
      0x70020007,  //  027F  JMP		#0288
      0x541EFFFB,  //  0280  LDINT	R7	65532
      0x1C1C0807,  //  0281  EQ	R7	R4	R7
      0x781E0004,  //  0282  JMPF	R7	#0288
      0x8C1C0D04,  //  0283  GETMET	R7	R6	K4
      0x88240D29,  //  0284  GETMBR	R9	R6	K41
      0x58280003,  //  0285  LDCONST	R10	K3
      0x7C1C0600,  //  0286  CALL	R7	3
      0x80040E00,  //  0287  RET	1	R7
      0x80000000,  //  0288  RET	0
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(endpoints),
    /* K2   */  be_const_int(0),
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
      0x60080012,  //  0006  GETGBL	R2	G18
      0x7C080000,  //  0007  CALL	R2	0
      0x400C0502,  //  0008  CONNECT	R3	R2	K2
      0x90020202,  //  0009  SETMBR	R0	K1	R2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


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
** Solidified class: Matter_Plugin_core
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_core,
    0,
    &be_class_Matter_Plugin,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, 1), be_const_closure(Matter_Plugin_core_read_attribute_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_core_init_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_core_invoke_request_closure) },
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
