/* Solidification of Matter_Plugin_1_Root.h */
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
    25,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[92]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(set),
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
    /* K47  */  be_nested_str_weak(rtc_utc),
    /* K48  */  be_const_int(1000000),
    /* K49  */  be_nested_str_weak(rtc),
    /* K50  */  be_nested_str_weak(local),
    /* K51  */  be_nested_str_weak(ack_request),
    /* K52  */  be_nested_str_weak(fabric_filtered),
    /* K53  */  be_nested_str_weak(get_fabric),
    /* K54  */  be_nested_str_weak(device),
    /* K55  */  be_nested_str_weak(sessions),
    /* K56  */  be_nested_str_weak(active_fabrics),
    /* K57  */  be_nested_str_weak(B2),
    /* K58  */  be_nested_str_weak(get_noc),
    /* K59  */  be_nested_str_weak(get_icac),
    /* K60  */  be_nested_str_weak(get_fabric_index),
    /* K61  */  be_nested_str_weak(stop_iteration),
    /* K62  */  be_nested_str_weak(parse),
    /* K63  */  be_nested_str_weak(get_ca),
    /* K64  */  be_nested_str_weak(findsubval),
    /* K65  */  be_nested_str_weak(get_admin_vendor),
    /* K66  */  be_nested_str_weak(get_fabric_id),
    /* K67  */  be_nested_str_weak(get_device_id),
    /* K68  */  be_nested_str_weak(get_fabric_label),
    /* K69  */  be_nested_str_weak(Fabric),
    /* K70  */  be_nested_str_weak(_MAX_CASE),
    /* K71  */  be_nested_str_weak(count_active_fabrics),
    /* K72  */  be_nested_str_weak(_fabric),
    /* K73  */  be_nested_str_weak(is_commissioning_open),
    /* K74  */  be_nested_str_weak(is_root_commissioning_open),
    /* K75  */  be_nested_str_weak(commissioning_admin_fabric),
    /* K76  */  be_nested_str_weak(Tasmota),
    /* K77  */  be_nested_str_weak(vendorid),
    /* K78  */  be_nested_str_weak(DeviceName),
    /* K79  */  be_nested_str_weak(FriendlyName),
    /* K80  */  be_nested_str_weak(FriendlyName1),
    /* K81  */  be_nested_str_weak(XX),
    /* K82  */  be_nested_str_weak(Status_X202),
    /* K83  */  be_nested_str_weak(StatusFWR),
    /* K84  */  be_nested_str_weak(Hardware),
    /* K85  */  be_nested_str_weak(Version),
    /* K86  */  be_nested_str_weak(_X28),
    /* K87  */  be_nested_str_weak(locale),
    /* K88  */  be_nested_str_weak(create_TLV),
    /* K89  */  be_nested_str_weak(get_active_endpoints),
    /* K90  */  be_nested_str_weak(disable_bridge_mode),
    /* K91  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[937]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x881C0504,  //  0004  GETMBR	R7	R2	K4
      0x5422002F,  //  0005  LDINT	R8	48
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220030,  //  0007  JMPF	R8	#0039
      0x1C200F05,  //  0008  EQ	R8	R7	K5
      0x78220005,  //  0009  JMPF	R8	#0010
      0x8C200706,  //  000A  GETMET	R8	R3	K6
      0x88280B07,  //  000B  GETMBR	R10	R5	K7
      0x882C0308,  //  000C  GETMBR	R11	R1	K8
      0x7C200600,  //  000D  CALL	R8	3
      0x80041000,  //  000E  RET	1	R8
      0x70020027,  //  000F  JMP		#0038
      0x1C200F09,  //  0010  EQ	R8	R7	K9
      0x7822000D,  //  0011  JMPF	R8	#0020
      0x8C200B0A,  //  0012  GETMET	R8	R5	K10
      0x7C200200,  //  0013  CALL	R8	1
      0x8C24110B,  //  0014  GETMET	R9	R8	K11
      0x582C0005,  //  0015  LDCONST	R11	K5
      0x88300B0C,  //  0016  GETMBR	R12	R5	K12
      0x5436003B,  //  0017  LDINT	R13	60
      0x7C240800,  //  0018  CALL	R9	4
      0x8C24110B,  //  0019  GETMET	R9	R8	K11
      0x582C0009,  //  001A  LDCONST	R11	K9
      0x88300B0C,  //  001B  GETMBR	R12	R5	K12
      0x54360383,  //  001C  LDINT	R13	900
      0x7C240800,  //  001D  CALL	R9	4
      0x80041000,  //  001E  RET	1	R8
      0x70020017,  //  001F  JMP		#0038
      0x1C200F0D,  //  0020  EQ	R8	R7	K13
      0x78220005,  //  0021  JMPF	R8	#0028
      0x8C200706,  //  0022  GETMET	R8	R3	K6
      0x88280B0E,  //  0023  GETMBR	R10	R5	K14
      0x582C000D,  //  0024  LDCONST	R11	K13
      0x7C200600,  //  0025  CALL	R8	3
      0x80041000,  //  0026  RET	1	R8
      0x7002000F,  //  0027  JMP		#0038
      0x1C200F0F,  //  0028  EQ	R8	R7	K15
      0x78220005,  //  0029  JMPF	R8	#0030
      0x8C200706,  //  002A  GETMET	R8	R3	K6
      0x88280B0E,  //  002B  GETMBR	R10	R5	K14
      0x582C000D,  //  002C  LDCONST	R11	K13
      0x7C200600,  //  002D  CALL	R8	3
      0x80041000,  //  002E  RET	1	R8
      0x70020007,  //  002F  JMP		#0038
      0x54220003,  //  0030  LDINT	R8	4
      0x1C200E08,  //  0031  EQ	R8	R7	R8
      0x78220004,  //  0032  JMPF	R8	#0038
      0x8C200706,  //  0033  GETMET	R8	R3	K6
      0x88280B10,  //  0034  GETMBR	R10	R5	K16
      0x502C0000,  //  0035  LDBOOL	R11	0	0
      0x7C200600,  //  0036  CALL	R8	3
      0x80041000,  //  0037  RET	1	R8
      0x7002036E,  //  0038  JMP		#03A8
      0x54220031,  //  0039  LDINT	R8	50
      0x1C200C08,  //  003A  EQ	R8	R6	R8
      0x78220000,  //  003B  JMPF	R8	#003D
      0x7002036A,  //  003C  JMP		#03A8
      0x54220032,  //  003D  LDINT	R8	51
      0x1C200C08,  //  003E  EQ	R8	R6	R8
      0x782200DC,  //  003F  JMPF	R8	#011D
      0x1C200F05,  //  0040  EQ	R8	R7	K5
      0x782200B5,  //  0041  JMPF	R8	#00F8
      0x8C200B11,  //  0042  GETMET	R8	R5	K17
      0x7C200200,  //  0043  CALL	R8	1
      0xB8262400,  //  0044  GETNGBL	R9	K18
      0x8C241313,  //  0045  GETMET	R9	R9	K19
      0x7C240200,  //  0046  CALL	R9	1
      0x94281314,  //  0047  GETIDX	R10	R9	K20
      0x782A0053,  //  0048  JMPF	R10	#009D
      0x8C281115,  //  0049  GETMET	R10	R8	K21
      0x4C300000,  //  004A  LDNIL	R12
      0x7C280400,  //  004B  CALL	R10	2
      0x8C2C150B,  //  004C  GETMET	R11	R10	K11
      0x58340005,  //  004D  LDCONST	R13	K5
      0x88380B16,  //  004E  GETMBR	R14	R5	K22
      0x583C0017,  //  004F  LDCONST	R15	K23
      0x7C2C0800,  //  0050  CALL	R11	4
      0x8C2C150B,  //  0051  GETMET	R11	R10	K11
      0x58340009,  //  0052  LDCONST	R13	K9
      0x88380B10,  //  0053  GETMBR	R14	R5	K16
      0x583C0009,  //  0054  LDCONST	R15	K9
      0x7C2C0800,  //  0055  CALL	R11	4
      0x8C2C150B,  //  0056  GETMET	R11	R10	K11
      0x5834000D,  //  0057  LDCONST	R13	K13
      0x88380B10,  //  0058  GETMBR	R14	R5	K16
      0x583C0009,  //  0059  LDCONST	R15	K9
      0x7C2C0800,  //  005A  CALL	R11	4
      0x8C2C150B,  //  005B  GETMET	R11	R10	K11
      0x5834000F,  //  005C  LDCONST	R13	K15
      0x88380B18,  //  005D  GETMBR	R14	R5	K24
      0x4C3C0000,  //  005E  LDNIL	R15
      0x7C2C0800,  //  005F  CALL	R11	4
      0x602C0015,  //  0060  GETGBL	R11	G21
      0x7C2C0000,  //  0061  CALL	R11	0
      0x8C2C1719,  //  0062  GETMET	R11	R11	K25
      0x8C34091A,  //  0063  GETMET	R13	R4	K26
      0x8C3C131B,  //  0064  GETMET	R15	R9	K27
      0x5844001C,  //  0065  LDCONST	R17	K28
      0x5848001D,  //  0066  LDCONST	R18	K29
      0x7C3C0600,  //  0067  CALL	R15	3
      0x5840001E,  //  0068  LDCONST	R16	K30
      0x5844001D,  //  0069  LDCONST	R17	K29
      0x7C340800,  //  006A  CALL	R13	4
      0x7C2C0400,  //  006B  CALL	R11	2
      0x8C30150B,  //  006C  GETMET	R12	R10	K11
      0x543A0003,  //  006D  LDINT	R14	4
      0x883C0B1F,  //  006E  GETMBR	R15	R5	K31
      0x5C401600,  //  006F  MOVE	R16	R11
      0x7C300800,  //  0070  CALL	R12	4
      0x8C301520,  //  0071  GETMET	R12	R10	K32
      0x543A0004,  //  0072  LDINT	R14	5
      0x7C300400,  //  0073  CALL	R12	2
      0x8C34190B,  //  0074  GETMET	R13	R12	K11
      0x4C3C0000,  //  0075  LDNIL	R15
      0x88400B1F,  //  0076  GETMBR	R16	R5	K31
      0xB8460200,  //  0077  GETNGBL	R17	K1
      0x8C442321,  //  0078  GETMET	R17	R17	K33
      0x8C4C131B,  //  0079  GETMET	R19	R9	K27
      0x58540022,  //  007A  LDCONST	R21	K34
      0x5858001D,  //  007B  LDCONST	R22	K29
      0x7C4C0600,  //  007C  CALL	R19	3
      0x7C440400,  //  007D  CALL	R17	2
      0x7C340800,  //  007E  CALL	R13	4
      0x8C341520,  //  007F  GETMET	R13	R10	K32
      0x543E0005,  //  0080  LDINT	R15	6
      0x7C340400,  //  0081  CALL	R13	2
      0x8C381B0B,  //  0082  GETMET	R14	R13	K11
      0x4C400000,  //  0083  LDNIL	R16
      0x88440B1F,  //  0084  GETMBR	R17	R5	K31
      0xB84A0200,  //  0085  GETNGBL	R18	K1
      0x8C482521,  //  0086  GETMET	R18	R18	K33
      0x8C50131B,  //  0087  GETMET	R20	R9	K27
      0x58580023,  //  0088  LDCONST	R22	K35
      0x585C001D,  //  0089  LDCONST	R23	K29
      0x7C500600,  //  008A  CALL	R20	3
      0x7C480400,  //  008B  CALL	R18	2
      0x7C380800,  //  008C  CALL	R14	4
      0x8C381B0B,  //  008D  GETMET	R14	R13	K11
      0x4C400000,  //  008E  LDNIL	R16
      0x88440B1F,  //  008F  GETMBR	R17	R5	K31
      0xB84A0200,  //  0090  GETNGBL	R18	K1
      0x8C482521,  //  0091  GETMET	R18	R18	K33
      0x8C50131B,  //  0092  GETMET	R20	R9	K27
      0x58580024,  //  0093  LDCONST	R22	K36
      0x585C001D,  //  0094  LDCONST	R23	K29
      0x7C500600,  //  0095  CALL	R20	3
      0x7C480400,  //  0096  CALL	R18	2
      0x7C380800,  //  0097  CALL	R14	4
      0x8C38150B,  //  0098  GETMET	R14	R10	K11
      0x54420006,  //  0099  LDINT	R16	7
      0x88440B0E,  //  009A  GETMBR	R17	R5	K14
      0x5848000D,  //  009B  LDCONST	R18	K13
      0x7C380800,  //  009C  CALL	R14	4
      0xB82A2400,  //  009D  GETNGBL	R10	K18
      0x8C281525,  //  009E  GETMET	R10	R10	K37
      0x7C280200,  //  009F  CALL	R10	1
      0x942C1514,  //  00A0  GETIDX	R11	R10	K20
      0x782E0053,  //  00A1  JMPF	R11	#00F6
      0x8C2C1115,  //  00A2  GETMET	R11	R8	K21
      0x4C340000,  //  00A3  LDNIL	R13
      0x7C2C0400,  //  00A4  CALL	R11	2
      0x8C30170B,  //  00A5  GETMET	R12	R11	K11
      0x58380005,  //  00A6  LDCONST	R14	K5
      0x883C0B16,  //  00A7  GETMBR	R15	R5	K22
      0x58400025,  //  00A8  LDCONST	R16	K37
      0x7C300800,  //  00A9  CALL	R12	4
      0x8C30170B,  //  00AA  GETMET	R12	R11	K11
      0x58380009,  //  00AB  LDCONST	R14	K9
      0x883C0B10,  //  00AC  GETMBR	R15	R5	K16
      0x58400009,  //  00AD  LDCONST	R16	K9
      0x7C300800,  //  00AE  CALL	R12	4
      0x8C30170B,  //  00AF  GETMET	R12	R11	K11
      0x5838000D,  //  00B0  LDCONST	R14	K13
      0x883C0B10,  //  00B1  GETMBR	R15	R5	K16
      0x58400009,  //  00B2  LDCONST	R16	K9
      0x7C300800,  //  00B3  CALL	R12	4
      0x8C30170B,  //  00B4  GETMET	R12	R11	K11
      0x5838000F,  //  00B5  LDCONST	R14	K15
      0x883C0B18,  //  00B6  GETMBR	R15	R5	K24
      0x4C400000,  //  00B7  LDNIL	R16
      0x7C300800,  //  00B8  CALL	R12	4
      0x60300015,  //  00B9  GETGBL	R12	G21
      0x7C300000,  //  00BA  CALL	R12	0
      0x8C301919,  //  00BB  GETMET	R12	R12	K25
      0x8C38091A,  //  00BC  GETMET	R14	R4	K26
      0x8C40151B,  //  00BD  GETMET	R16	R10	K27
      0x5848001C,  //  00BE  LDCONST	R18	K28
      0x584C001D,  //  00BF  LDCONST	R19	K29
      0x7C400600,  //  00C0  CALL	R16	3
      0x5844001E,  //  00C1  LDCONST	R17	K30
      0x5848001D,  //  00C2  LDCONST	R18	K29
      0x7C380800,  //  00C3  CALL	R14	4
      0x7C300400,  //  00C4  CALL	R12	2
      0x8C34170B,  //  00C5  GETMET	R13	R11	K11
      0x543E0003,  //  00C6  LDINT	R15	4
      0x88400B1F,  //  00C7  GETMBR	R16	R5	K31
      0x5C441800,  //  00C8  MOVE	R17	R12
      0x7C340800,  //  00C9  CALL	R13	4
      0x8C341720,  //  00CA  GETMET	R13	R11	K32
      0x543E0004,  //  00CB  LDINT	R15	5
      0x7C340400,  //  00CC  CALL	R13	2
      0x8C381B0B,  //  00CD  GETMET	R14	R13	K11
      0x4C400000,  //  00CE  LDNIL	R16
      0x88440B1F,  //  00CF  GETMBR	R17	R5	K31
      0xB84A0200,  //  00D0  GETNGBL	R18	K1
      0x8C482521,  //  00D1  GETMET	R18	R18	K33
      0x8C50151B,  //  00D2  GETMET	R20	R10	K27
      0x58580022,  //  00D3  LDCONST	R22	K34
      0x585C001D,  //  00D4  LDCONST	R23	K29
      0x7C500600,  //  00D5  CALL	R20	3
      0x7C480400,  //  00D6  CALL	R18	2
      0x7C380800,  //  00D7  CALL	R14	4
      0x8C381720,  //  00D8  GETMET	R14	R11	K32
      0x54420005,  //  00D9  LDINT	R16	6
      0x7C380400,  //  00DA  CALL	R14	2
      0x8C3C1D0B,  //  00DB  GETMET	R15	R14	K11
      0x4C440000,  //  00DC  LDNIL	R17
      0x88480B1F,  //  00DD  GETMBR	R18	R5	K31
      0xB84E0200,  //  00DE  GETNGBL	R19	K1
      0x8C4C2721,  //  00DF  GETMET	R19	R19	K33
      0x8C54151B,  //  00E0  GETMET	R21	R10	K27
      0x585C0023,  //  00E1  LDCONST	R23	K35
      0x5860001D,  //  00E2  LDCONST	R24	K29
      0x7C540600,  //  00E3  CALL	R21	3
      0x7C4C0400,  //  00E4  CALL	R19	2
      0x7C3C0800,  //  00E5  CALL	R15	4
      0x8C3C1D0B,  //  00E6  GETMET	R15	R14	K11
      0x4C440000,  //  00E7  LDNIL	R17
      0x88480B1F,  //  00E8  GETMBR	R18	R5	K31
      0xB84E0200,  //  00E9  GETNGBL	R19	K1
      0x8C4C2721,  //  00EA  GETMET	R19	R19	K33
      0x8C54151B,  //  00EB  GETMET	R21	R10	K27
      0x585C0024,  //  00EC  LDCONST	R23	K36
      0x5860001D,  //  00ED  LDCONST	R24	K29
      0x7C540600,  //  00EE  CALL	R21	3
      0x7C4C0400,  //  00EF  CALL	R19	2
      0x7C3C0800,  //  00F0  CALL	R15	4
      0x8C3C170B,  //  00F1  GETMET	R15	R11	K11
      0x54460006,  //  00F2  LDINT	R17	7
      0x88480B0E,  //  00F3  GETMBR	R18	R5	K14
      0x584C0009,  //  00F4  LDCONST	R19	K9
      0x7C3C0800,  //  00F5  CALL	R15	4
      0x80041000,  //  00F6  RET	1	R8
      0x70020023,  //  00F7  JMP		#011C
      0x1C200F09,  //  00F8  EQ	R8	R7	K9
      0x7822000B,  //  00F9  JMPF	R8	#0106
      0x8C200706,  //  00FA  GETMET	R8	R3	K6
      0x88280B0C,  //  00FB  GETMBR	R10	R5	K12
      0xB82E2400,  //  00FC  GETNGBL	R11	K18
      0x8C2C1726,  //  00FD  GETMET	R11	R11	K38
      0x58340027,  //  00FE  LDCONST	R13	K39
      0x50380200,  //  00FF  LDBOOL	R14	1	0
      0x7C2C0600,  //  0100  CALL	R11	3
      0x942C1728,  //  0101  GETIDX	R11	R11	K40
      0x942C1729,  //  0102  GETIDX	R11	R11	K41
      0x7C200600,  //  0103  CALL	R8	3
      0x80041000,  //  0104  RET	1	R8
      0x70020015,  //  0105  JMP		#011C
      0x1C200F0D,  //  0106  EQ	R8	R7	K13
      0x7822000B,  //  0107  JMPF	R8	#0114
      0x8C200706,  //  0108  GETMET	R8	R3	K6
      0x88280B2A,  //  0109  GETMBR	R10	R5	K42
      0xB82E2400,  //  010A  GETNGBL	R11	K18
      0x8C2C1726,  //  010B  GETMET	R11	R11	K38
      0x5834002B,  //  010C  LDCONST	R13	K43
      0x50380200,  //  010D  LDBOOL	R14	1	0
      0x7C2C0600,  //  010E  CALL	R11	3
      0x942C172C,  //  010F  GETIDX	R11	R11	K44
      0x942C172D,  //  0110  GETIDX	R11	R11	K45
      0x7C200600,  //  0111  CALL	R8	3
      0x80041000,  //  0112  RET	1	R8
      0x70020007,  //  0113  JMP		#011C
      0x54220007,  //  0114  LDINT	R8	8
      0x1C200E08,  //  0115  EQ	R8	R7	R8
      0x78220004,  //  0116  JMPF	R8	#011C
      0x8C200706,  //  0117  GETMET	R8	R3	K6
      0x88280B10,  //  0118  GETMBR	R10	R5	K16
      0x502C0000,  //  0119  LDBOOL	R11	0	0
      0x7C200600,  //  011A  CALL	R8	3
      0x80041000,  //  011B  RET	1	R8
      0x7002028A,  //  011C  JMP		#03A8
      0x54220033,  //  011D  LDINT	R8	52
      0x1C200C08,  //  011E  EQ	R8	R6	R8
      0x78220000,  //  011F  JMPF	R8	#0121
      0x70020286,  //  0120  JMP		#03A8
      0x54220037,  //  0121  LDINT	R8	56
      0x1C200C08,  //  0122  EQ	R8	R6	R8
      0x7822002B,  //  0123  JMPF	R8	#0150
      0x1C200F05,  //  0124  EQ	R8	R7	K5
      0x7822000E,  //  0125  JMPF	R8	#0135
      0xB8225C00,  //  0126  GETNGBL	R8	K46
      0xB8262400,  //  0127  GETNGBL	R9	K18
      0x8C24132F,  //  0128  GETMET	R9	R9	K47
      0x7C240200,  //  0129  CALL	R9	1
      0x7C200200,  //  012A  CALL	R8	1
      0xB8265C00,  //  012B  GETNGBL	R9	K46
      0x58280030,  //  012C  LDCONST	R10	K48
      0x7C240200,  //  012D  CALL	R9	1
      0x08201009,  //  012E  MUL	R8	R8	R9
      0x8C240706,  //  012F  GETMET	R9	R3	K6
      0x882C0B07,  //  0130  GETMBR	R11	R5	K7
      0x5C301000,  //  0131  MOVE	R12	R8
      0x7C240600,  //  0132  CALL	R9	3
      0x80041200,  //  0133  RET	1	R9
      0x70020019,  //  0134  JMP		#014F
      0x1C200F09,  //  0135  EQ	R8	R7	K9
      0x78220005,  //  0136  JMPF	R8	#013D
      0x8C200706,  //  0137  GETMET	R8	R3	K6
      0x88280B0E,  //  0138  GETMBR	R10	R5	K14
      0x582C000F,  //  0139  LDCONST	R11	K15
      0x7C200600,  //  013A  CALL	R8	3
      0x80041000,  //  013B  RET	1	R8
      0x70020011,  //  013C  JMP		#014F
      0x54220006,  //  013D  LDINT	R8	7
      0x1C200E08,  //  013E  EQ	R8	R7	R8
      0x7822000E,  //  013F  JMPF	R8	#014F
      0xB8225C00,  //  0140  GETNGBL	R8	K46
      0xB8262400,  //  0141  GETNGBL	R9	K18
      0x8C241331,  //  0142  GETMET	R9	R9	K49
      0x7C240200,  //  0143  CALL	R9	1
      0x94241332,  //  0144  GETIDX	R9	R9	K50
      0x7C200200,  //  0145  CALL	R8	1
      0xB8265C00,  //  0146  GETNGBL	R9	K46
      0x58280030,  //  0147  LDCONST	R10	K48
      0x7C240200,  //  0148  CALL	R9	1
      0x08201009,  //  0149  MUL	R8	R8	R9
      0x8C240706,  //  014A  GETMET	R9	R3	K6
      0x882C0B07,  //  014B  GETMBR	R11	R5	K7
      0x5C301000,  //  014C  MOVE	R12	R8
      0x7C240600,  //  014D  CALL	R9	3
      0x80041200,  //  014E  RET	1	R9
      0x70020257,  //  014F  JMP		#03A8
      0x5422003D,  //  0150  LDINT	R8	62
      0x1C200C08,  //  0151  EQ	R8	R6	R8
      0x782200AD,  //  0152  JMPF	R8	#0201
      0x8C200133,  //  0153  GETMET	R8	R0	K51
      0x5C280400,  //  0154  MOVE	R10	R2
      0x7C200400,  //  0155  CALL	R8	2
      0x1C200F05,  //  0156  EQ	R8	R7	K5
      0x78220032,  //  0157  JMPF	R8	#018B
      0x8C200B11,  //  0158  GETMET	R8	R5	K17
      0x7C200200,  //  0159  CALL	R8	1
      0x88240534,  //  015A  GETMBR	R9	R2	K52
      0x78260005,  //  015B  JMPF	R9	#0162
      0x60240012,  //  015C  GETGBL	R9	G18
      0x7C240000,  //  015D  CALL	R9	0
      0x8C280335,  //  015E  GETMET	R10	R1	K53
      0x7C280200,  //  015F  CALL	R10	1
      0x4028120A,  //  0160  CONNECT	R10	R9	R10
      0x70020003,  //  0161  JMP		#0166
      0x88240136,  //  0162  GETMBR	R9	R0	K54
      0x88241337,  //  0163  GETMBR	R9	R9	K55
      0x8C241338,  //  0164  GETMET	R9	R9	K56
      0x7C240200,  //  0165  CALL	R9	1
      0x60280010,  //  0166  GETGBL	R10	G16
      0x5C2C1200,  //  0167  MOVE	R11	R9
      0x7C280200,  //  0168  CALL	R10	1
      0xA802001B,  //  0169  EXBLK	0	#0186
      0x5C2C1400,  //  016A  MOVE	R11	R10
      0x7C2C0000,  //  016B  CALL	R11	0
      0x4C300000,  //  016C  LDNIL	R12
      0x1C30160C,  //  016D  EQ	R12	R11	R12
      0x78320000,  //  016E  JMPF	R12	#0170
      0x7001FFF9,  //  016F  JMP		#016A
      0x8C301115,  //  0170  GETMET	R12	R8	K21
      0x4C380000,  //  0171  LDNIL	R14
      0x7C300400,  //  0172  CALL	R12	2
      0x8C34190B,  //  0173  GETMET	R13	R12	K11
      0x583C0009,  //  0174  LDCONST	R15	K9
      0x88400B39,  //  0175  GETMBR	R16	R5	K57
      0x8C44173A,  //  0176  GETMET	R17	R11	K58
      0x7C440200,  //  0177  CALL	R17	1
      0x7C340800,  //  0178  CALL	R13	4
      0x8C34190B,  //  0179  GETMET	R13	R12	K11
      0x583C000D,  //  017A  LDCONST	R15	K13
      0x88400B39,  //  017B  GETMBR	R16	R5	K57
      0x8C44173B,  //  017C  GETMET	R17	R11	K59
      0x7C440200,  //  017D  CALL	R17	1
      0x7C340800,  //  017E  CALL	R13	4
      0x8C34190B,  //  017F  GETMET	R13	R12	K11
      0x543E00FD,  //  0180  LDINT	R15	254
      0x88400B0C,  //  0181  GETMBR	R16	R5	K12
      0x8C44173C,  //  0182  GETMET	R17	R11	K60
      0x7C440200,  //  0183  CALL	R17	1
      0x7C340800,  //  0184  CALL	R13	4
      0x7001FFE3,  //  0185  JMP		#016A
      0x5828003D,  //  0186  LDCONST	R10	K61
      0xAC280200,  //  0187  CATCH	R10	1	0
      0xB0080000,  //  0188  RAISE	2	R0	R0
      0x80041000,  //  0189  RET	1	R8
      0x70020074,  //  018A  JMP		#0200
      0x1C200F09,  //  018B  EQ	R8	R7	K9
      0x78220049,  //  018C  JMPF	R8	#01D7
      0x8C200B11,  //  018D  GETMET	R8	R5	K17
      0x7C200200,  //  018E  CALL	R8	1
      0x88240534,  //  018F  GETMBR	R9	R2	K52
      0x78260005,  //  0190  JMPF	R9	#0197
      0x60240012,  //  0191  GETGBL	R9	G18
      0x7C240000,  //  0192  CALL	R9	0
      0x8C280335,  //  0193  GETMET	R10	R1	K53
      0x7C280200,  //  0194  CALL	R10	1
      0x4028120A,  //  0195  CONNECT	R10	R9	R10
      0x70020003,  //  0196  JMP		#019B
      0x88240136,  //  0197  GETMBR	R9	R0	K54
      0x88241337,  //  0198  GETMBR	R9	R9	K55
      0x8C241338,  //  0199  GETMET	R9	R9	K56
      0x7C240200,  //  019A  CALL	R9	1
      0x60280010,  //  019B  GETGBL	R10	G16
      0x5C2C1200,  //  019C  MOVE	R11	R9
      0x7C280200,  //  019D  CALL	R10	1
      0xA8020032,  //  019E  EXBLK	0	#01D2
      0x5C2C1400,  //  019F  MOVE	R11	R10
      0x7C2C0000,  //  01A0  CALL	R11	0
      0x4C300000,  //  01A1  LDNIL	R12
      0x1C30160C,  //  01A2  EQ	R12	R11	R12
      0x78320000,  //  01A3  JMPF	R12	#01A5
      0x7001FFF9,  //  01A4  JMP		#019F
      0x8C300B3E,  //  01A5  GETMET	R12	R5	K62
      0x8C38173F,  //  01A6  GETMET	R14	R11	K63
      0x7C380200,  //  01A7  CALL	R14	1
      0x7C300400,  //  01A8  CALL	R12	2
      0x8C341115,  //  01A9  GETMET	R13	R8	K21
      0x4C3C0000,  //  01AA  LDNIL	R15
      0x7C340400,  //  01AB  CALL	R13	2
      0x8C381B0B,  //  01AC  GETMET	R14	R13	K11
      0x58400009,  //  01AD  LDCONST	R16	K9
      0x88440B39,  //  01AE  GETMBR	R17	R5	K57
      0x8C481940,  //  01AF  GETMET	R18	R12	K64
      0x54520008,  //  01B0  LDINT	R20	9
      0x7C480400,  //  01B1  CALL	R18	2
      0x7C380800,  //  01B2  CALL	R14	4
      0x8C381B0B,  //  01B3  GETMET	R14	R13	K11
      0x5840000D,  //  01B4  LDCONST	R16	K13
      0x88440B0C,  //  01B5  GETMBR	R17	R5	K12
      0x8C481741,  //  01B6  GETMET	R18	R11	K65
      0x7C480200,  //  01B7  CALL	R18	1
      0x7C380800,  //  01B8  CALL	R14	4
      0x8C381B0B,  //  01B9  GETMET	R14	R13	K11
      0x5840000F,  //  01BA  LDCONST	R16	K15
      0x88440B07,  //  01BB  GETMBR	R17	R5	K7
      0x8C481742,  //  01BC  GETMET	R18	R11	K66
      0x7C480200,  //  01BD  CALL	R18	1
      0x7C380800,  //  01BE  CALL	R14	4
      0x8C381B0B,  //  01BF  GETMET	R14	R13	K11
      0x54420003,  //  01C0  LDINT	R16	4
      0x88440B07,  //  01C1  GETMBR	R17	R5	K7
      0x8C481743,  //  01C2  GETMET	R18	R11	K67
      0x7C480200,  //  01C3  CALL	R18	1
      0x7C380800,  //  01C4  CALL	R14	4
      0x8C381B0B,  //  01C5  GETMET	R14	R13	K11
      0x54420004,  //  01C6  LDINT	R16	5
      0x88440B16,  //  01C7  GETMBR	R17	R5	K22
      0x8C481744,  //  01C8  GETMET	R18	R11	K68
      0x7C480200,  //  01C9  CALL	R18	1
      0x7C380800,  //  01CA  CALL	R14	4
      0x8C381B0B,  //  01CB  GETMET	R14	R13	K11
      0x544200FD,  //  01CC  LDINT	R16	254
      0x88440B0C,  //  01CD  GETMBR	R17	R5	K12
      0x8C48173C,  //  01CE  GETMET	R18	R11	K60
      0x7C480200,  //  01CF  CALL	R18	1
      0x7C380800,  //  01D0  CALL	R14	4
      0x7001FFCC,  //  01D1  JMP		#019F
      0x5828003D,  //  01D2  LDCONST	R10	K61
      0xAC280200,  //  01D3  CATCH	R10	1	0
      0xB0080000,  //  01D4  RAISE	2	R0	R0
      0x80041000,  //  01D5  RET	1	R8
      0x70020028,  //  01D6  JMP		#0200
      0x1C200F0D,  //  01D7  EQ	R8	R7	K13
      0x78220007,  //  01D8  JMPF	R8	#01E1
      0x8C200706,  //  01D9  GETMET	R8	R3	K6
      0x88280B0E,  //  01DA  GETMBR	R10	R5	K14
      0xB82E0200,  //  01DB  GETNGBL	R11	K1
      0x882C1745,  //  01DC  GETMBR	R11	R11	K69
      0x882C1746,  //  01DD  GETMBR	R11	R11	K70
      0x7C200600,  //  01DE  CALL	R8	3
      0x80041000,  //  01DF  RET	1	R8
      0x7002001E,  //  01E0  JMP		#0200
      0x1C200F0F,  //  01E1  EQ	R8	R7	K15
      0x78220009,  //  01E2  JMPF	R8	#01ED
      0x88200136,  //  01E3  GETMBR	R8	R0	K54
      0x88201137,  //  01E4  GETMBR	R8	R8	K55
      0x8C201147,  //  01E5  GETMET	R8	R8	K71
      0x7C200200,  //  01E6  CALL	R8	1
      0x8C240706,  //  01E7  GETMET	R9	R3	K6
      0x882C0B0E,  //  01E8  GETMBR	R11	R5	K14
      0x5C301000,  //  01E9  MOVE	R12	R8
      0x7C240600,  //  01EA  CALL	R9	3
      0x80041200,  //  01EB  RET	1	R9
      0x70020012,  //  01EC  JMP		#0200
      0x54220003,  //  01ED  LDINT	R8	4
      0x1C200E08,  //  01EE  EQ	R8	R7	R8
      0x78220000,  //  01EF  JMPF	R8	#01F1
      0x7002000E,  //  01F0  JMP		#0200
      0x54220004,  //  01F1  LDINT	R8	5
      0x1C200E08,  //  01F2  EQ	R8	R7	R8
      0x7822000B,  //  01F3  JMPF	R8	#0200
      0x88200348,  //  01F4  GETMBR	R8	R1	K72
      0x8C20113C,  //  01F5  GETMET	R8	R8	K60
      0x7C200200,  //  01F6  CALL	R8	1
      0x4C240000,  //  01F7  LDNIL	R9
      0x1C241009,  //  01F8  EQ	R9	R8	R9
      0x78260000,  //  01F9  JMPF	R9	#01FB
      0x58200005,  //  01FA  LDCONST	R8	K5
      0x8C240706,  //  01FB  GETMET	R9	R3	K6
      0x882C0B0E,  //  01FC  GETMBR	R11	R5	K14
      0x5C301000,  //  01FD  MOVE	R12	R8
      0x7C240600,  //  01FE  CALL	R9	3
      0x80041200,  //  01FF  RET	1	R9
      0x700201A6,  //  0200  JMP		#03A8
      0x5422003B,  //  0201  LDINT	R8	60
      0x1C200C08,  //  0202  EQ	R8	R6	R8
      0x7822003C,  //  0203  JMPF	R8	#0241
      0x1C200F05,  //  0204  EQ	R8	R7	K5
      0x78220012,  //  0205  JMPF	R8	#0219
      0x88200136,  //  0206  GETMBR	R8	R0	K54
      0x8C201149,  //  0207  GETMET	R8	R8	K73
      0x7C200200,  //  0208  CALL	R8	1
      0x88240136,  //  0209  GETMBR	R9	R0	K54
      0x8C24134A,  //  020A  GETMET	R9	R9	K74
      0x7C240200,  //  020B  CALL	R9	1
      0x78220004,  //  020C  JMPF	R8	#0212
      0x78260001,  //  020D  JMPF	R9	#0210
      0x5828000D,  //  020E  LDCONST	R10	K13
      0x70020000,  //  020F  JMP		#0211
      0x58280009,  //  0210  LDCONST	R10	K9
      0x70020000,  //  0211  JMP		#0213
      0x58280005,  //  0212  LDCONST	R10	K5
      0x8C2C0706,  //  0213  GETMET	R11	R3	K6
      0x88340B0E,  //  0214  GETMBR	R13	R5	K14
      0x5C381400,  //  0215  MOVE	R14	R10
      0x7C2C0600,  //  0216  CALL	R11	3
      0x80041600,  //  0217  RET	1	R11
      0x70020026,  //  0218  JMP		#0240
      0x1C200F09,  //  0219  EQ	R8	R7	K9
      0x78220011,  //  021A  JMPF	R8	#022D
      0x88200136,  //  021B  GETMBR	R8	R0	K54
      0x8820114B,  //  021C  GETMBR	R8	R8	K75
      0x4C240000,  //  021D  LDNIL	R9
      0x20241009,  //  021E  NE	R9	R8	R9
      0x78260006,  //  021F  JMPF	R9	#0227
      0x8C240706,  //  0220  GETMET	R9	R3	K6
      0x882C0B0C,  //  0221  GETMBR	R11	R5	K12
      0x8C30113C,  //  0222  GETMET	R12	R8	K60
      0x7C300200,  //  0223  CALL	R12	1
      0x7C240600,  //  0224  CALL	R9	3
      0x80041200,  //  0225  RET	1	R9
      0x70020004,  //  0226  JMP		#022C
      0x8C240706,  //  0227  GETMET	R9	R3	K6
      0x882C0B18,  //  0228  GETMBR	R11	R5	K24
      0x4C300000,  //  0229  LDNIL	R12
      0x7C240600,  //  022A  CALL	R9	3
      0x80041200,  //  022B  RET	1	R9
      0x70020012,  //  022C  JMP		#0240
      0x1C200F0D,  //  022D  EQ	R8	R7	K13
      0x78220010,  //  022E  JMPF	R8	#0240
      0x88200136,  //  022F  GETMBR	R8	R0	K54
      0x8820114B,  //  0230  GETMBR	R8	R8	K75
      0x4C240000,  //  0231  LDNIL	R9
      0x20241009,  //  0232  NE	R9	R8	R9
      0x78260006,  //  0233  JMPF	R9	#023B
      0x8C240706,  //  0234  GETMET	R9	R3	K6
      0x882C0B0C,  //  0235  GETMBR	R11	R5	K12
      0x8C301141,  //  0236  GETMET	R12	R8	K65
      0x7C300200,  //  0237  CALL	R12	1
      0x7C240600,  //  0238  CALL	R9	3
      0x80041200,  //  0239  RET	1	R9
      0x70020004,  //  023A  JMP		#0240
      0x8C240706,  //  023B  GETMET	R9	R3	K6
      0x882C0B18,  //  023C  GETMBR	R11	R5	K24
      0x4C300000,  //  023D  LDNIL	R12
      0x7C240600,  //  023E  CALL	R9	3
      0x80041200,  //  023F  RET	1	R9
      0x70020166,  //  0240  JMP		#03A8
      0x54220027,  //  0241  LDINT	R8	40
      0x1C200C08,  //  0242  EQ	R8	R6	R8
      0x782200BA,  //  0243  JMPF	R8	#02FF
      0x8C200133,  //  0244  GETMET	R8	R0	K51
      0x5C280400,  //  0245  MOVE	R10	R2
      0x7C200400,  //  0246  CALL	R8	2
      0x1C200F05,  //  0247  EQ	R8	R7	K5
      0x78220005,  //  0248  JMPF	R8	#024F
      0x8C200706,  //  0249  GETMET	R8	R3	K6
      0x88280B0C,  //  024A  GETMBR	R10	R5	K12
      0x582C0009,  //  024B  LDCONST	R11	K9
      0x7C200600,  //  024C  CALL	R8	3
      0x80041000,  //  024D  RET	1	R8
      0x700200AE,  //  024E  JMP		#02FE
      0x1C200F09,  //  024F  EQ	R8	R7	K9
      0x78220005,  //  0250  JMPF	R8	#0257
      0x8C200706,  //  0251  GETMET	R8	R3	K6
      0x88280B16,  //  0252  GETMBR	R10	R5	K22
      0x582C004C,  //  0253  LDCONST	R11	K76
      0x7C200600,  //  0254  CALL	R8	3
      0x80041000,  //  0255  RET	1	R8
      0x700200A6,  //  0256  JMP		#02FE
      0x1C200F0D,  //  0257  EQ	R8	R7	K13
      0x78220006,  //  0258  JMPF	R8	#0260
      0x8C200706,  //  0259  GETMET	R8	R3	K6
      0x88280B0C,  //  025A  GETMBR	R10	R5	K12
      0x882C0136,  //  025B  GETMBR	R11	R0	K54
      0x882C174D,  //  025C  GETMBR	R11	R11	K77
      0x7C200600,  //  025D  CALL	R8	3
      0x80041000,  //  025E  RET	1	R8
      0x7002009D,  //  025F  JMP		#02FE
      0x1C200F0F,  //  0260  EQ	R8	R7	K15
      0x7822000A,  //  0261  JMPF	R8	#026D
      0x8C200706,  //  0262  GETMET	R8	R3	K6
      0x88280B16,  //  0263  GETMBR	R10	R5	K22
      0xB82E2400,  //  0264  GETNGBL	R11	K18
      0x8C2C1726,  //  0265  GETMET	R11	R11	K38
      0x5834004E,  //  0266  LDCONST	R13	K78
      0x50380200,  //  0267  LDBOOL	R14	1	0
      0x7C2C0600,  //  0268  CALL	R11	3
      0x942C174E,  //  0269  GETIDX	R11	R11	K78
      0x7C200600,  //  026A  CALL	R8	3
      0x80041000,  //  026B  RET	1	R8
      0x70020090,  //  026C  JMP		#02FE
      0x54220003,  //  026D  LDINT	R8	4
      0x1C200E08,  //  026E  EQ	R8	R7	R8
      0x78220005,  //  026F  JMPF	R8	#0276
      0x8C200706,  //  0270  GETMET	R8	R3	K6
      0x88280B0C,  //  0271  GETMBR	R10	R5	K12
      0x542E7FFF,  //  0272  LDINT	R11	32768
      0x7C200600,  //  0273  CALL	R8	3
      0x80041000,  //  0274  RET	1	R8
      0x70020087,  //  0275  JMP		#02FE
      0x54220004,  //  0276  LDINT	R8	5
      0x1C200E08,  //  0277  EQ	R8	R7	R8
      0x7822000A,  //  0278  JMPF	R8	#0284
      0x8C200706,  //  0279  GETMET	R8	R3	K6
      0x88280B16,  //  027A  GETMBR	R10	R5	K22
      0xB82E2400,  //  027B  GETNGBL	R11	K18
      0x8C2C1726,  //  027C  GETMET	R11	R11	K38
      0x5834004F,  //  027D  LDCONST	R13	K79
      0x50380200,  //  027E  LDBOOL	R14	1	0
      0x7C2C0600,  //  027F  CALL	R11	3
      0x942C1750,  //  0280  GETIDX	R11	R11	K80
      0x7C200600,  //  0281  CALL	R8	3
      0x80041000,  //  0282  RET	1	R8
      0x70020079,  //  0283  JMP		#02FE
      0x54220005,  //  0284  LDINT	R8	6
      0x1C200E08,  //  0285  EQ	R8	R7	R8
      0x78220005,  //  0286  JMPF	R8	#028D
      0x8C200706,  //  0287  GETMET	R8	R3	K6
      0x88280B16,  //  0288  GETMBR	R10	R5	K22
      0x582C0051,  //  0289  LDCONST	R11	K81
      0x7C200600,  //  028A  CALL	R8	3
      0x80041000,  //  028B  RET	1	R8
      0x70020070,  //  028C  JMP		#02FE
      0x54220006,  //  028D  LDINT	R8	7
      0x1C200E08,  //  028E  EQ	R8	R7	R8
      0x78220005,  //  028F  JMPF	R8	#0296
      0x8C200706,  //  0290  GETMET	R8	R3	K6
      0x88280B0C,  //  0291  GETMBR	R10	R5	K12
      0x582C0005,  //  0292  LDCONST	R11	K5
      0x7C200600,  //  0293  CALL	R8	3
      0x80041000,  //  0294  RET	1	R8
      0x70020067,  //  0295  JMP		#02FE
      0x54220007,  //  0296  LDINT	R8	8
      0x1C200E08,  //  0297  EQ	R8	R7	R8
      0x7822000B,  //  0298  JMPF	R8	#02A5
      0x8C200706,  //  0299  GETMET	R8	R3	K6
      0x88280B16,  //  029A  GETMBR	R10	R5	K22
      0xB82E2400,  //  029B  GETNGBL	R11	K18
      0x8C2C1726,  //  029C  GETMET	R11	R11	K38
      0x58340052,  //  029D  LDCONST	R13	K82
      0x50380200,  //  029E  LDBOOL	R14	1	0
      0x7C2C0600,  //  029F  CALL	R11	3
      0x942C1753,  //  02A0  GETIDX	R11	R11	K83
      0x942C1754,  //  02A1  GETIDX	R11	R11	K84
      0x7C200600,  //  02A2  CALL	R8	3
      0x80041000,  //  02A3  RET	1	R8
      0x70020058,  //  02A4  JMP		#02FE
      0x54220008,  //  02A5  LDINT	R8	9
      0x1C200E08,  //  02A6  EQ	R8	R7	R8
      0x78220005,  //  02A7  JMPF	R8	#02AE
      0x8C200706,  //  02A8  GETMET	R8	R3	K6
      0x88280B0C,  //  02A9  GETMBR	R10	R5	K12
      0x582C0009,  //  02AA  LDCONST	R11	K9
      0x7C200600,  //  02AB  CALL	R8	3
      0x80041000,  //  02AC  RET	1	R8
      0x7002004F,  //  02AD  JMP		#02FE
      0x54220009,  //  02AE  LDINT	R8	10
      0x1C200E08,  //  02AF  EQ	R8	R7	R8
      0x78220015,  //  02B0  JMPF	R8	#02C7
      0xB8222400,  //  02B1  GETNGBL	R8	K18
      0x8C201126,  //  02B2  GETMET	R8	R8	K38
      0x58280052,  //  02B3  LDCONST	R10	K82
      0x502C0200,  //  02B4  LDBOOL	R11	1	0
      0x7C200600,  //  02B5  CALL	R8	3
      0x94201153,  //  02B6  GETIDX	R8	R8	K83
      0x94201155,  //  02B7  GETIDX	R8	R8	K85
      0x8C24091B,  //  02B8  GETMET	R9	R4	K27
      0x5C2C1000,  //  02B9  MOVE	R11	R8
      0x58300056,  //  02BA  LDCONST	R12	K86
      0x7C240600,  //  02BB  CALL	R9	3
      0x24281305,  //  02BC  GT	R10	R9	K5
      0x782A0002,  //  02BD  JMPF	R10	#02C1
      0x04281309,  //  02BE  SUB	R10	R9	K9
      0x402A0A0A,  //  02BF  CONNECT	R10	K5	R10
      0x9420100A,  //  02C0  GETIDX	R8	R8	R10
      0x8C280706,  //  02C1  GETMET	R10	R3	K6
      0x88300B16,  //  02C2  GETMBR	R12	R5	K22
      0x5C341000,  //  02C3  MOVE	R13	R8
      0x7C280600,  //  02C4  CALL	R10	3
      0x80041400,  //  02C5  RET	1	R10
      0x70020036,  //  02C6  JMP		#02FE
      0x5422000E,  //  02C7  LDINT	R8	15
      0x1C200E08,  //  02C8  EQ	R8	R7	R8
      0x7822000B,  //  02C9  JMPF	R8	#02D6
      0x8C200706,  //  02CA  GETMET	R8	R3	K6
      0x88280B16,  //  02CB  GETMBR	R10	R5	K22
      0xB82E2400,  //  02CC  GETNGBL	R11	K18
      0x8C2C1725,  //  02CD  GETMET	R11	R11	K37
      0x7C2C0200,  //  02CE  CALL	R11	1
      0x8C2C171B,  //  02CF  GETMET	R11	R11	K27
      0x5834001C,  //  02D0  LDCONST	R13	K28
      0x5838001D,  //  02D1  LDCONST	R14	K29
      0x7C2C0600,  //  02D2  CALL	R11	3
      0x7C200600,  //  02D3  CALL	R8	3
      0x80041000,  //  02D4  RET	1	R8
      0x70020027,  //  02D5  JMP		#02FE
      0x54220010,  //  02D6  LDINT	R8	17
      0x1C200E08,  //  02D7  EQ	R8	R7	R8
      0x78220005,  //  02D8  JMPF	R8	#02DF
      0x8C200706,  //  02D9  GETMET	R8	R3	K6
      0x88280B10,  //  02DA  GETMBR	R10	R5	K16
      0x582C0009,  //  02DB  LDCONST	R11	K9
      0x7C200600,  //  02DC  CALL	R8	3
      0x80041000,  //  02DD  RET	1	R8
      0x7002001E,  //  02DE  JMP		#02FE
      0x54220011,  //  02DF  LDINT	R8	18
      0x1C200E08,  //  02E0  EQ	R8	R7	R8
      0x7822000B,  //  02E1  JMPF	R8	#02EE
      0x8C200706,  //  02E2  GETMET	R8	R3	K6
      0x88280B16,  //  02E3  GETMBR	R10	R5	K22
      0xB82E2400,  //  02E4  GETNGBL	R11	K18
      0x8C2C1725,  //  02E5  GETMET	R11	R11	K37
      0x7C2C0200,  //  02E6  CALL	R11	1
      0x8C2C171B,  //  02E7  GETMET	R11	R11	K27
      0x5834001C,  //  02E8  LDCONST	R13	K28
      0x5838001D,  //  02E9  LDCONST	R14	K29
      0x7C2C0600,  //  02EA  CALL	R11	3
      0x7C200600,  //  02EB  CALL	R8	3
      0x80041000,  //  02EC  RET	1	R8
      0x7002000F,  //  02ED  JMP		#02FE
      0x54220012,  //  02EE  LDINT	R8	19
      0x1C200E08,  //  02EF  EQ	R8	R7	R8
      0x7822000C,  //  02F0  JMPF	R8	#02FE
      0x8C200B0A,  //  02F1  GETMET	R8	R5	K10
      0x7C200200,  //  02F2  CALL	R8	1
      0x8C24110B,  //  02F3  GETMET	R9	R8	K11
      0x582C0005,  //  02F4  LDCONST	R11	K5
      0x88300B0C,  //  02F5  GETMBR	R12	R5	K12
      0x5834000F,  //  02F6  LDCONST	R13	K15
      0x7C240800,  //  02F7  CALL	R9	4
      0x8C24110B,  //  02F8  GETMET	R9	R8	K11
      0x582C0009,  //  02F9  LDCONST	R11	K9
      0x88300B0C,  //  02FA  GETMBR	R12	R5	K12
      0x5834000F,  //  02FB  LDCONST	R13	K15
      0x7C240800,  //  02FC  CALL	R9	4
      0x80041000,  //  02FD  RET	1	R8
      0x700200A8,  //  02FE  JMP		#03A8
      0x5422003E,  //  02FF  LDINT	R8	63
      0x1C200C08,  //  0300  EQ	R8	R6	R8
      0x78220000,  //  0301  JMPF	R8	#0303
      0x700200A4,  //  0302  JMP		#03A8
      0x54220029,  //  0303  LDINT	R8	42
      0x1C200C08,  //  0304  EQ	R8	R6	R8
      0x7822001D,  //  0305  JMPF	R8	#0324
      0x1C200F05,  //  0306  EQ	R8	R7	K5
      0x78220003,  //  0307  JMPF	R8	#030C
      0x8C200B11,  //  0308  GETMET	R8	R5	K17
      0x7C200200,  //  0309  CALL	R8	1
      0x80041000,  //  030A  RET	1	R8
      0x70020016,  //  030B  JMP		#0323
      0x1C200F09,  //  030C  EQ	R8	R7	K9
      0x78220005,  //  030D  JMPF	R8	#0314
      0x8C200706,  //  030E  GETMET	R8	R3	K6
      0x88280B10,  //  030F  GETMBR	R10	R5	K16
      0x582C0005,  //  0310  LDCONST	R11	K5
      0x7C200600,  //  0311  CALL	R8	3
      0x80041000,  //  0312  RET	1	R8
      0x7002000E,  //  0313  JMP		#0323
      0x1C200F0D,  //  0314  EQ	R8	R7	K13
      0x78220005,  //  0315  JMPF	R8	#031C
      0x8C200706,  //  0316  GETMET	R8	R3	K6
      0x88280B0E,  //  0317  GETMBR	R10	R5	K14
      0x582C0009,  //  0318  LDCONST	R11	K9
      0x7C200600,  //  0319  CALL	R8	3
      0x80041000,  //  031A  RET	1	R8
      0x70020006,  //  031B  JMP		#0323
      0x1C200F0F,  //  031C  EQ	R8	R7	K15
      0x78220004,  //  031D  JMPF	R8	#0323
      0x8C200706,  //  031E  GETMET	R8	R3	K6
      0x88280B18,  //  031F  GETMBR	R10	R5	K24
      0x4C2C0000,  //  0320  LDNIL	R11
      0x7C200600,  //  0321  CALL	R8	3
      0x80041000,  //  0322  RET	1	R8
      0x70020083,  //  0323  JMP		#03A8
      0x5422002A,  //  0324  LDINT	R8	43
      0x1C200C08,  //  0325  EQ	R8	R6	R8
      0x78220016,  //  0326  JMPF	R8	#033E
      0x1C200F05,  //  0327  EQ	R8	R7	K5
      0x78220007,  //  0328  JMPF	R8	#0331
      0x8C200706,  //  0329  GETMET	R8	R3	K6
      0x88280B16,  //  032A  GETMBR	R10	R5	K22
      0xB82E2400,  //  032B  GETNGBL	R11	K18
      0x8C2C1757,  //  032C  GETMET	R11	R11	K87
      0x7C2C0200,  //  032D  CALL	R11	1
      0x7C200600,  //  032E  CALL	R8	3
      0x80041000,  //  032F  RET	1	R8
      0x7002000B,  //  0330  JMP		#033D
      0x1C200F09,  //  0331  EQ	R8	R7	K9
      0x78220009,  //  0332  JMPF	R8	#033D
      0x8C200B11,  //  0333  GETMET	R8	R5	K17
      0x7C200200,  //  0334  CALL	R8	1
      0x8C24110B,  //  0335  GETMET	R9	R8	K11
      0x4C2C0000,  //  0336  LDNIL	R11
      0x88300B16,  //  0337  GETMBR	R12	R5	K22
      0xB8362400,  //  0338  GETNGBL	R13	K18
      0x8C341B57,  //  0339  GETMET	R13	R13	K87
      0x7C340200,  //  033A  CALL	R13	1
      0x7C240800,  //  033B  CALL	R9	4
      0x80041000,  //  033C  RET	1	R8
      0x70020069,  //  033D  JMP		#03A8
      0x5422002B,  //  033E  LDINT	R8	44
      0x1C200C08,  //  033F  EQ	R8	R6	R8
      0x7822001C,  //  0340  JMPF	R8	#035E
      0x1C200F05,  //  0341  EQ	R8	R7	K5
      0x78220005,  //  0342  JMPF	R8	#0349
      0x8C200706,  //  0343  GETMET	R8	R3	K6
      0x88280B0E,  //  0344  GETMBR	R10	R5	K14
      0x582C0009,  //  0345  LDCONST	R11	K9
      0x7C200600,  //  0346  CALL	R8	3
      0x80041000,  //  0347  RET	1	R8
      0x70020013,  //  0348  JMP		#035D
      0x1C200F09,  //  0349  EQ	R8	R7	K9
      0x78220005,  //  034A  JMPF	R8	#0351
      0x8C200706,  //  034B  GETMET	R8	R3	K6
      0x88280B0E,  //  034C  GETMBR	R10	R5	K14
      0x542E0003,  //  034D  LDINT	R11	4
      0x7C200600,  //  034E  CALL	R8	3
      0x80041000,  //  034F  RET	1	R8
      0x7002000B,  //  0350  JMP		#035D
      0x1C200F0D,  //  0351  EQ	R8	R7	K13
      0x78220009,  //  0352  JMPF	R8	#035D
      0x8C200B11,  //  0353  GETMET	R8	R5	K17
      0x7C200200,  //  0354  CALL	R8	1
      0x8C24110B,  //  0355  GETMET	R9	R8	K11
      0x4C2C0000,  //  0356  LDNIL	R11
      0x8C300B58,  //  0357  GETMET	R12	R5	K88
      0x88380B0E,  //  0358  GETMBR	R14	R5	K14
      0x543E0003,  //  0359  LDINT	R15	4
      0x7C300600,  //  035A  CALL	R12	3
      0x7C240600,  //  035B  CALL	R9	3
      0x80041000,  //  035C  RET	1	R8
      0x70020049,  //  035D  JMP		#03A8
      0x54220030,  //  035E  LDINT	R8	49
      0x1C200C08,  //  035F  EQ	R8	R6	R8
      0x78220010,  //  0360  JMPF	R8	#0372
      0x1C200F0F,  //  0361  EQ	R8	R7	K15
      0x78220005,  //  0362  JMPF	R8	#0369
      0x8C200706,  //  0363  GETMET	R8	R3	K6
      0x88280B0E,  //  0364  GETMBR	R10	R5	K14
      0x542E001D,  //  0365  LDINT	R11	30
      0x7C200600,  //  0366  CALL	R8	3
      0x80041000,  //  0367  RET	1	R8
      0x70020007,  //  0368  JMP		#0371
      0x5422FFFB,  //  0369  LDINT	R8	65532
      0x1C200E08,  //  036A  EQ	R8	R7	R8
      0x78220004,  //  036B  JMPF	R8	#0371
      0x8C200706,  //  036C  GETMET	R8	R3	K6
      0x88280B2A,  //  036D  GETMBR	R10	R5	K42
      0x542E0003,  //  036E  LDINT	R11	4
      0x7C200600,  //  036F  CALL	R8	3
      0x80041000,  //  0370  RET	1	R8
      0x70020035,  //  0371  JMP		#03A8
      0x5422001C,  //  0372  LDINT	R8	29
      0x1C200C08,  //  0373  EQ	R8	R6	R8
      0x78220029,  //  0374  JMPF	R8	#039F
      0x1C200F0F,  //  0375  EQ	R8	R7	K15
      0x7822001D,  //  0376  JMPF	R8	#0395
      0x8C200B11,  //  0377  GETMET	R8	R5	K17
      0x7C200200,  //  0378  CALL	R8	1
      0x88240136,  //  0379  GETMBR	R9	R0	K54
      0x8C241359,  //  037A  GETMET	R9	R9	K89
      0x502C0200,  //  037B  LDBOOL	R11	1	0
      0x7C240400,  //  037C  CALL	R9	2
      0x88280136,  //  037D  GETMBR	R10	R0	K54
      0x8828155A,  //  037E  GETMBR	R10	R10	K90
      0x602C0010,  //  037F  GETGBL	R11	G16
      0x5C301200,  //  0380  MOVE	R12	R9
      0x7C2C0200,  //  0381  CALL	R11	1
      0xA802000C,  //  0382  EXBLK	0	#0390
      0x5C301600,  //  0383  MOVE	R12	R11
      0x7C300000,  //  0384  CALL	R12	0
      0x5C341400,  //  0385  MOVE	R13	R10
      0x78360002,  //  0386  JMPF	R13	#038A
      0x5436FEFF,  //  0387  LDINT	R13	65280
      0x1434180D,  //  0388  LT	R13	R12	R13
      0x78360004,  //  0389  JMPF	R13	#038F
      0x8C34110B,  //  038A  GETMET	R13	R8	K11
      0x4C3C0000,  //  038B  LDNIL	R15
      0x88400B0C,  //  038C  GETMBR	R16	R5	K12
      0x5C441800,  //  038D  MOVE	R17	R12
      0x7C340800,  //  038E  CALL	R13	4
      0x7001FFF2,  //  038F  JMP		#0383
      0x582C003D,  //  0390  LDCONST	R11	K61
      0xAC2C0200,  //  0391  CATCH	R11	1	0
      0xB0080000,  //  0392  RAISE	2	R0	R0
      0x80041000,  //  0393  RET	1	R8
      0x70020008,  //  0394  JMP		#039E
      0x60200003,  //  0395  GETGBL	R8	G3
      0x5C240000,  //  0396  MOVE	R9	R0
      0x7C200200,  //  0397  CALL	R8	1
      0x8C20115B,  //  0398  GETMET	R8	R8	K91
      0x5C280200,  //  0399  MOVE	R10	R1
      0x5C2C0400,  //  039A  MOVE	R11	R2
      0x5C300600,  //  039B  MOVE	R12	R3
      0x7C200800,  //  039C  CALL	R8	4
      0x80041000,  //  039D  RET	1	R8
      0x70020008,  //  039E  JMP		#03A8
      0x60200003,  //  039F  GETGBL	R8	G3
      0x5C240000,  //  03A0  MOVE	R9	R0
      0x7C200200,  //  03A1  CALL	R8	1
      0x8C20115B,  //  03A2  GETMET	R8	R8	K91
      0x5C280200,  //  03A3  MOVE	R10	R1
      0x5C2C0400,  //  03A4  MOVE	R11	R2
      0x5C300600,  //  03A5  MOVE	R12	R3
      0x7C200800,  //  03A6  CALL	R8	4
      0x80041000,  //  03A7  RET	1	R8
      0x80000000,  //  03A8  RET	0
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
    /* K17  */  be_nested_str_weak(ack_request),
    /* K18  */  be_nested_str_weak(_fabric),
    /* K19  */  be_nested_str_weak(fabric_completed),
    /* K20  */  be_nested_str_weak(set_no_expiration),
    /* K21  */  be_nested_str_weak(save),
    /* K22  */  be_nested_str_weak(device),
    /* K23  */  be_nested_str_weak(start_commissioning_complete_deferred),
    /* K24  */  be_nested_str_weak(context_error),
    /* K25  */  be_nested_str_weak(CommissioningComplete_X3A_X20no_X20fabric_X20attached),
    /* K26  */  be_nested_str_weak(status),
    /* K27  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K28  */  be_nested_str_weak(B2),
    /* K29  */  be_nested_str_weak(DAC_Cert_FFF1_8000),
    /* K30  */  be_nested_str_weak(PAI_Cert_FFF1),
    /* K31  */  be_nested_str_weak(CD_FFF1_8000),
    /* K32  */  be_nested_str_weak(B1),
    /* K33  */  be_nested_str_weak(U4),
    /* K34  */  be_nested_str_weak(tasmota),
    /* K35  */  be_nested_str_weak(rtc_utc),
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
    ( &(const binstruction[713]) {  /* code */
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
      0x5C280600,  //  0043  MOVE	R10	R3
      0x7C200400,  //  0044  CALL	R8	2
      0x88200312,  //  0045  GETMBR	R8	R1	K18
      0x7822001B,  //  0046  JMPF	R8	#0063
      0x90061105,  //  0047  SETMBR	R1	K8	K5
      0x88200312,  //  0048  GETMBR	R8	R1	K18
      0x8C201113,  //  0049  GETMET	R8	R8	K19
      0x7C200200,  //  004A  CALL	R8	1
      0x8C200314,  //  004B  GETMET	R8	R1	K20
      0x7C200200,  //  004C  CALL	R8	1
      0x8C200315,  //  004D  GETMET	R8	R1	K21
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
      0x88240116,  //  005D  GETMBR	R9	R0	K22
      0x8C241317,  //  005E  GETMET	R9	R9	K23
      0x5C2C0200,  //  005F  MOVE	R11	R1
      0x7C240400,  //  0060  CALL	R9	2
      0x80041000,  //  0061  RET	1	R8
      0x70020000,  //  0062  JMP		#0064
      0xB0063119,  //  0063  RAISE	1	K24	K25
      0x70020261,  //  0064  JMP		#02C7
      0x5422003D,  //  0065  LDINT	R8	62
      0x1C200C08,  //  0066  EQ	R8	R6	R8
      0x782201C2,  //  0067  JMPF	R8	#022B
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
      0x8824131B,  //  0072  GETMBR	R9	R9	K27
      0x900E3409,  //  0073  SETMBR	R3	K26	R9
      0x4C240000,  //  0074  LDNIL	R9
      0x80041200,  //  0075  RET	1	R9
      0x8C240B09,  //  0076  GETMET	R9	R5	K9
      0x7C240200,  //  0077  CALL	R9	1
      0x8C28130A,  //  0078  GETMET	R10	R9	K10
      0x58300005,  //  0079  LDCONST	R12	K5
      0x88340B1C,  //  007A  GETMBR	R13	R5	K28
      0x1C381107,  //  007B  EQ	R14	R8	K7
      0x783A0003,  //  007C  JMPF	R14	#0081
      0xB83A0200,  //  007D  GETNGBL	R14	K1
      0x8C381D1D,  //  007E  GETMET	R14	R14	K29
      0x7C380200,  //  007F  CALL	R14	1
      0x70020002,  //  0080  JMP		#0084
      0xB83A0200,  //  0081  GETNGBL	R14	K1
      0x8C381D1E,  //  0082  GETMET	R14	R14	K30
      0x7C380200,  //  0083  CALL	R14	1
      0x7C280800,  //  0084  CALL	R10	4
      0x900E0910,  //  0085  SETMBR	R3	K4	K16
      0x80041200,  //  0086  RET	1	R9
      0x700201A1,  //  0087  JMP		#022A
      0x1C200F05,  //  0088  EQ	R8	R7	K5
      0x7822003C,  //  0089  JMPF	R8	#00C7
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
      0x88340B1C,  //  009A  GETMBR	R13	R5	K28
      0xB83A0200,  //  009B  GETNGBL	R14	K1
      0x8C381D1F,  //  009C  GETMET	R14	R14	K31
      0x7C380200,  //  009D  CALL	R14	1
      0x7C280800,  //  009E  CALL	R10	4
      0x8C28130A,  //  009F  GETMET	R10	R9	K10
      0x5830000E,  //  00A0  LDCONST	R12	K14
      0x88340B20,  //  00A1  GETMBR	R13	R5	K32
      0x5C381000,  //  00A2  MOVE	R14	R8
      0x7C280800,  //  00A3  CALL	R10	4
      0x8C28130A,  //  00A4  GETMET	R10	R9	K10
      0x58300010,  //  00A5  LDCONST	R12	K16
      0x88340B21,  //  00A6  GETMBR	R13	R5	K33
      0xB83A4400,  //  00A7  GETNGBL	R14	K34
      0x8C381D23,  //  00A8  GETMET	R14	R14	K35
      0x7C380200,  //  00A9  CALL	R14	1
      0x7C280800,  //  00AA  CALL	R10	4
      0x8C281324,  //  00AB  GETMET	R10	R9	K36
      0x7C280200,  //  00AC  CALL	R10	1
      0x8C2C0325,  //  00AD  GETMET	R11	R1	K37
      0x7C2C0200,  //  00AE  CALL	R11	1
      0x0030140B,  //  00AF  ADD	R12	R10	R11
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
      0x88480B1C,  //  00BC  GETMBR	R18	R5	K28
      0x5C4C1400,  //  00BD  MOVE	R19	R10
      0x7C3C0800,  //  00BE  CALL	R15	4
      0x8C3C1D0A,  //  00BF  GETMET	R15	R14	K10
      0x58440007,  //  00C0  LDCONST	R17	K7
      0x88480B20,  //  00C1  GETMBR	R18	R5	K32
      0x5C4C1A00,  //  00C2  MOVE	R19	R13
      0x7C3C0800,  //  00C3  CALL	R15	4
      0x900E0907,  //  00C4  SETMBR	R3	K4	K7
      0x80041C00,  //  00C5  RET	1	R14
      0x70020162,  //  00C6  JMP		#022A
      0x54220003,  //  00C7  LDINT	R8	4
      0x1C200E08,  //  00C8  EQ	R8	R7	R8
      0x7822003C,  //  00C9  JMPF	R8	#0107
      0x8C200111,  //  00CA  GETMET	R8	R0	K17
      0x5C280600,  //  00CB  MOVE	R10	R3
      0x7C200400,  //  00CC  CALL	R8	2
      0x8C200506,  //  00CD  GETMET	R8	R2	K6
      0x58280005,  //  00CE  LDCONST	R10	K5
      0x7C200400,  //  00CF  CALL	R8	2
      0x6024000C,  //  00D0  GETGBL	R9	G12
      0x5C281000,  //  00D1  MOVE	R10	R8
      0x7C240200,  //  00D2  CALL	R9	1
      0x542A001F,  //  00D3  LDINT	R10	32
      0x2024120A,  //  00D4  NE	R9	R9	R10
      0x78260001,  //  00D5  JMPF	R9	#00D8
      0x4C240000,  //  00D6  LDNIL	R9
      0x80041200,  //  00D7  RET	1	R9
      0x8C240506,  //  00D8  GETMET	R9	R2	K6
      0x582C0007,  //  00D9  LDCONST	R11	K7
      0x50300000,  //  00DA  LDBOOL	R12	0	0
      0x7C240600,  //  00DB  CALL	R9	3
      0x8C280329,  //  00DC  GETMET	R10	R1	K41
      0x7C280200,  //  00DD  CALL	R10	1
      0x8C2C0B09,  //  00DE  GETMET	R11	R5	K9
      0x7C2C0200,  //  00DF  CALL	R11	1
      0x8C30170A,  //  00E0  GETMET	R12	R11	K10
      0x58380007,  //  00E1  LDCONST	R14	K7
      0x883C0B1C,  //  00E2  GETMBR	R15	R5	K28
      0x5C401400,  //  00E3  MOVE	R16	R10
      0x7C300800,  //  00E4  CALL	R12	4
      0x8C30170A,  //  00E5  GETMET	R12	R11	K10
      0x5838000E,  //  00E6  LDCONST	R14	K14
      0x883C0B20,  //  00E7  GETMBR	R15	R5	K32
      0x5C401000,  //  00E8  MOVE	R16	R8
      0x7C300800,  //  00E9  CALL	R12	4
      0x8C301724,  //  00EA  GETMET	R12	R11	K36
      0x7C300200,  //  00EB  CALL	R12	1
      0x8C340325,  //  00EC  GETMET	R13	R1	K37
      0x7C340200,  //  00ED  CALL	R13	1
      0x0034180D,  //  00EE  ADD	R13	R12	R13
      0x8C380926,  //  00EF  GETMET	R14	R4	K38
      0x7C380200,  //  00F0  CALL	R14	1
      0x8C381D27,  //  00F1  GETMET	R14	R14	K39
      0xB8420200,  //  00F2  GETNGBL	R16	K1
      0x8C402128,  //  00F3  GETMET	R16	R16	K40
      0x7C400200,  //  00F4  CALL	R16	1
      0x5C441A00,  //  00F5  MOVE	R17	R13
      0x7C380600,  //  00F6  CALL	R14	3
      0x8C3C0B09,  //  00F7  GETMET	R15	R5	K9
      0x7C3C0200,  //  00F8  CALL	R15	1
      0x8C401F0A,  //  00F9  GETMET	R16	R15	K10
      0x58480005,  //  00FA  LDCONST	R18	K5
      0x884C0B1C,  //  00FB  GETMBR	R19	R5	K28
      0x5C501800,  //  00FC  MOVE	R20	R12
      0x7C400800,  //  00FD  CALL	R16	4
      0x8C401F0A,  //  00FE  GETMET	R16	R15	K10
      0x58480007,  //  00FF  LDCONST	R18	K7
      0x884C0B20,  //  0100  GETMBR	R19	R5	K32
      0x5C501C00,  //  0101  MOVE	R20	R14
      0x7C400800,  //  0102  CALL	R16	4
      0x54420004,  //  0103  LDINT	R16	5
      0x900E0810,  //  0104  SETMBR	R3	K4	R16
      0x80041E00,  //  0105  RET	1	R15
      0x70020122,  //  0106  JMP		#022A
      0x5422000A,  //  0107  LDINT	R8	11
      0x1C200E08,  //  0108  EQ	R8	R7	R8
      0x7822000B,  //  0109  JMPF	R8	#0116
      0x8C200506,  //  010A  GETMET	R8	R2	K6
      0x58280005,  //  010B  LDCONST	R10	K5
      0x7C200400,  //  010C  CALL	R8	2
      0x8C24032A,  //  010D  GETMET	R9	R1	K42
      0x5C2C1000,  //  010E  MOVE	R11	R8
      0x7C240400,  //  010F  CALL	R9	2
      0xB8260200,  //  0110  GETNGBL	R9	K1
      0x8824132B,  //  0111  GETMBR	R9	R9	K43
      0x900E3409,  //  0112  SETMBR	R3	K26	R9
      0x4C240000,  //  0113  LDNIL	R9
      0x80041200,  //  0114  RET	1	R9
      0x70020113,  //  0115  JMP		#022A
      0x54220005,  //  0116  LDINT	R8	6
      0x1C200E08,  //  0117  EQ	R8	R7	R8
      0x782200B9,  //  0118  JMPF	R8	#01D3
      0xB8224400,  //  0119  GETNGBL	R8	K34
      0x8C20112C,  //  011A  GETMET	R8	R8	K44
      0x60280008,  //  011B  GETGBL	R10	G8
      0x5C2C0400,  //  011C  MOVE	R11	R2
      0x7C280200,  //  011D  CALL	R10	1
      0x002A5A0A,  //  011E  ADD	R10	K45	R10
      0x542E0003,  //  011F  LDINT	R11	4
      0x7C200600,  //  0120  CALL	R8	3
      0x8C200506,  //  0121  GETMET	R8	R2	K6
      0x58280005,  //  0122  LDCONST	R10	K5
      0x7C200400,  //  0123  CALL	R8	2
      0x8C240506,  //  0124  GETMET	R9	R2	K6
      0x582C0007,  //  0125  LDCONST	R11	K7
      0x7C240400,  //  0126  CALL	R9	2
      0x6028000C,  //  0127  GETGBL	R10	G12
      0x5C2C1200,  //  0128  MOVE	R11	R9
      0x7C280200,  //  0129  CALL	R10	1
      0x1C281505,  //  012A  EQ	R10	R10	K5
      0x782A0000,  //  012B  JMPF	R10	#012D
      0x4C240000,  //  012C  LDNIL	R9
      0x8C280506,  //  012D  GETMET	R10	R2	K6
      0x5830000E,  //  012E  LDCONST	R12	K14
      0x7C280400,  //  012F  CALL	R10	2
      0x8C2C0506,  //  0130  GETMET	R11	R2	K6
      0x58340010,  //  0131  LDCONST	R13	K16
      0x7C2C0400,  //  0132  CALL	R11	2
      0x8C300506,  //  0133  GETMET	R12	R2	K6
      0x543A0003,  //  0134  LDINT	R14	4
      0x7C300400,  //  0135  CALL	R12	2
      0x8C34032E,  //  0136  GETMET	R13	R1	K46
      0x7C340200,  //  0137  CALL	R13	1
      0x4C380000,  //  0138  LDNIL	R14
      0x1C341A0E,  //  0139  EQ	R13	R13	R14
      0x78360006,  //  013A  JMPF	R13	#0142
      0xB8364400,  //  013B  GETNGBL	R13	K34
      0x8C341B2C,  //  013C  GETMET	R13	R13	K44
      0x583C002F,  //  013D  LDCONST	R15	K47
      0x5840000E,  //  013E  LDCONST	R16	K14
      0x7C340600,  //  013F  CALL	R13	3
      0x4C340000,  //  0140  LDNIL	R13
      0x80041A00,  //  0141  RET	1	R13
      0x88340116,  //  0142  GETMBR	R13	R0	K22
      0x88341B30,  //  0143  GETMBR	R13	R13	K48
      0x8C341B31,  //  0144  GETMET	R13	R13	K49
      0x7C340200,  //  0145  CALL	R13	1
      0x8C381B32,  //  0146  GETMET	R14	R13	K50
      0x8C40032E,  //  0147  GETMET	R16	R1	K46
      0x7C400200,  //  0148  CALL	R16	1
      0x7C380400,  //  0149  CALL	R14	2
      0x8C381B33,  //  014A  GETMET	R14	R13	K51
      0x5C401000,  //  014B  MOVE	R16	R8
      0x5C441200,  //  014C  MOVE	R17	R9
      0x7C380600,  //  014D  CALL	R14	3
      0x8C381B34,  //  014E  GETMET	R14	R13	K52
      0x5C401400,  //  014F  MOVE	R16	R10
      0x7C380400,  //  0150  CALL	R14	2
      0x8C381B35,  //  0151  GETMET	R14	R13	K53
      0x5C401600,  //  0152  MOVE	R16	R11
      0x5C441800,  //  0153  MOVE	R17	R12
      0x7C380600,  //  0154  CALL	R14	3
      0x8C381B36,  //  0155  GETMET	R14	R13	K54
      0x8C400337,  //  0156  GETMET	R16	R1	K55
      0x7C400200,  //  0157  CALL	R16	1
      0x7C380400,  //  0158  CALL	R14	2
      0xB83A0200,  //  0159  GETNGBL	R14	K1
      0x88381D02,  //  015A  GETMBR	R14	R14	K2
      0x8C381D38,  //  015B  GETMET	R14	R14	K56
      0x5C401000,  //  015C  MOVE	R16	R8
      0x7C380400,  //  015D  CALL	R14	2
      0x8C3C1D39,  //  015E  GETMET	R15	R14	K57
      0x54460005,  //  015F  LDINT	R17	6
      0x7C3C0400,  //  0160  CALL	R15	2
      0x8C401F06,  //  0161  GETMET	R16	R15	K6
      0x544A0014,  //  0162  LDINT	R18	21
      0x7C400400,  //  0163  CALL	R16	2
      0x8C441F06,  //  0164  GETMET	R17	R15	K6
      0x544E0010,  //  0165  LDINT	R19	17
      0x7C440400,  //  0166  CALL	R17	2
      0x5C482000,  //  0167  MOVE	R18	R16
      0x784A0001,  //  0168  JMPF	R18	#016B
      0x5C482200,  //  0169  MOVE	R18	R17
      0x744A0006,  //  016A  JMPT	R18	#0172
      0xB84A4400,  //  016B  GETNGBL	R18	K34
      0x8C48252C,  //  016C  GETMET	R18	R18	K44
      0x5850003A,  //  016D  LDCONST	R20	K58
      0x5854000E,  //  016E  LDCONST	R21	K14
      0x7C480600,  //  016F  CALL	R18	3
      0x50480000,  //  0170  LDBOOL	R18	0	0
      0x80042400,  //  0171  RET	1	R18
      0x60480004,  //  0172  GETGBL	R18	G4
      0x5C4C2000,  //  0173  MOVE	R19	R16
      0x7C480200,  //  0174  CALL	R18	1
      0x1C48253B,  //  0175  EQ	R18	R18	K59
      0x784A0007,  //  0176  JMPF	R18	#017F
      0xB84A7800,  //  0177  GETNGBL	R18	K60
      0x8C48253D,  //  0178  GETMET	R18	R18	K61
      0x5C502000,  //  0179  MOVE	R20	R16
      0x7C480400,  //  017A  CALL	R18	2
      0x8C48253E,  //  017B  GETMET	R18	R18	K62
      0x7C480200,  //  017C  CALL	R18	1
      0x5C402400,  //  017D  MOVE	R16	R18
      0x70020002,  //  017E  JMP		#0182
      0x8C48213E,  //  017F  GETMET	R18	R16	K62
      0x7C480200,  //  0180  CALL	R18	1
      0x5C402400,  //  0181  MOVE	R16	R18
      0x60480004,  //  0182  GETGBL	R18	G4
      0x5C4C2200,  //  0183  MOVE	R19	R17
      0x7C480200,  //  0184  CALL	R18	1
      0x1C48253B,  //  0185  EQ	R18	R18	K59
      0x784A0007,  //  0186  JMPF	R18	#018F
      0xB84A7800,  //  0187  GETNGBL	R18	K60
      0x8C48253D,  //  0188  GETMET	R18	R18	K61
      0x5C502200,  //  0189  MOVE	R20	R17
      0x7C480400,  //  018A  CALL	R18	2
      0x8C48253E,  //  018B  GETMET	R18	R18	K62
      0x7C480200,  //  018C  CALL	R18	1
      0x5C442400,  //  018D  MOVE	R17	R18
      0x70020002,  //  018E  JMP		#0192
      0x8C48233E,  //  018F  GETMET	R18	R17	K62
      0x7C480200,  //  0190  CALL	R18	1
      0x5C442400,  //  0191  MOVE	R17	R18
      0x8C48033F,  //  0192  GETMET	R18	R1	K63
      0x7C480200,  //  0193  CALL	R18	1
      0x404E0F40,  //  0194  CONNECT	R19	K7	K64
      0x94482413,  //  0195  GETIDX	R18	R18	R19
      0x60500015,  //  0196  GETGBL	R20	G21
      0x7C500000,  //  0197  CALL	R20	0
      0x8C502941,  //  0198  GETMET	R20	R20	K65
      0x58580042,  //  0199  LDCONST	R22	K66
      0x7C500400,  //  019A  CALL	R20	2
      0x5C4C2800,  //  019B  MOVE	R19	R20
      0x8C500943,  //  019C  GETMET	R20	R4	K67
      0x7C500200,  //  019D  CALL	R20	1
      0x8C542144,  //  019E  GETMET	R21	R16	K68
      0x7C540200,  //  019F  CALL	R21	1
      0x8C542B45,  //  01A0  GETMET	R21	R21	K69
      0x7C540200,  //  01A1  CALL	R21	1
      0x8C582946,  //  01A2  GETMET	R22	R20	K70
      0x5C602400,  //  01A3  MOVE	R24	R18
      0x5C642A00,  //  01A4  MOVE	R25	R21
      0x5C682600,  //  01A5  MOVE	R26	R19
      0x546E0007,  //  01A6  LDINT	R27	8
      0x7C580A00,  //  01A7  CALL	R22	5
      0x885C0312,  //  01A8  GETMBR	R23	R1	K18
      0x785E0001,  //  01A9  JMPF	R23	#01AC
      0x885C0312,  //  01AA  GETMBR	R23	R1	K18
      0x70020001,  //  01AB  JMP		#01AE
      0x885C0116,  //  01AC  GETMBR	R23	R0	K22
      0x885C2F47,  //  01AD  GETMBR	R23	R23	K71
      0x8C601B48,  //  01AE  GETMET	R24	R13	K72
      0x5C682000,  //  01AF  MOVE	R26	R16
      0x5C6C2200,  //  01B0  MOVE	R27	R17
      0x5C702C00,  //  01B1  MOVE	R28	R22
      0x5C742E00,  //  01B2  MOVE	R29	R23
      0x7C600A00,  //  01B3  CALL	R24	5
      0x8C601B49,  //  01B4  GETMET	R24	R13	K73
      0x7C600200,  //  01B5  CALL	R24	1
      0x88600116,  //  01B6  GETMBR	R24	R0	K22
      0x8C60314A,  //  01B7  GETMET	R24	R24	K74
      0x5C681A00,  //  01B8  MOVE	R26	R13
      0x7C600400,  //  01B9  CALL	R24	2
      0x8C60034B,  //  01BA  GETMET	R24	R1	K75
      0x7C600200,  //  01BB  CALL	R24	1
      0x78620002,  //  01BC  JMPF	R24	#01C0
      0x8C60034C,  //  01BD  GETMET	R24	R1	K76
      0x546A003B,  //  01BE  LDINT	R26	60
      0x7C600400,  //  01BF  CALL	R24	2
      0x8C601B4D,  //  01C0  GETMET	R24	R13	K77
      0x7C600200,  //  01C1  CALL	R24	1
      0x8C600B09,  //  01C2  GETMET	R24	R5	K9
      0x7C600200,  //  01C3  CALL	R24	1
      0x8C64310A,  //  01C4  GETMET	R25	R24	K10
      0x586C0005,  //  01C5  LDCONST	R27	K5
      0x88700B0B,  //  01C6  GETMBR	R28	R5	K11
      0xB8760200,  //  01C7  GETNGBL	R29	K1
      0x88743B2B,  //  01C8  GETMBR	R29	R29	K43
      0x7C640800,  //  01C9  CALL	R25	4
      0x8C64310A,  //  01CA  GETMET	R25	R24	K10
      0x586C0007,  //  01CB  LDCONST	R27	K7
      0x88700B0B,  //  01CC  GETMBR	R28	R5	K11
      0x58740007,  //  01CD  LDCONST	R29	K7
      0x7C640800,  //  01CE  CALL	R25	4
      0x54660007,  //  01CF  LDINT	R25	8
      0x900E0819,  //  01D0  SETMBR	R3	K4	R25
      0x80043000,  //  01D1  RET	1	R24
      0x70020056,  //  01D2  JMP		#022A
      0x54220008,  //  01D3  LDINT	R8	9
      0x1C200E08,  //  01D4  EQ	R8	R7	R8
      0x7822001E,  //  01D5  JMPF	R8	#01F5
      0x8C200506,  //  01D6  GETMET	R8	R2	K6
      0x58280005,  //  01D7  LDCONST	R10	K5
      0x7C200400,  //  01D8  CALL	R8	2
      0x8C24034E,  //  01D9  GETMET	R9	R1	K78
      0x5C2C1000,  //  01DA  MOVE	R11	R8
      0x7C240400,  //  01DB  CALL	R9	2
      0xB8264400,  //  01DC  GETNGBL	R9	K34
      0x8C24132C,  //  01DD  GETMET	R9	R9	K44
      0x602C0018,  //  01DE  GETGBL	R11	G24
      0x5830004F,  //  01DF  LDCONST	R12	K79
      0x88340312,  //  01E0  GETMBR	R13	R1	K18
      0x8C341B50,  //  01E1  GETMET	R13	R13	K80
      0x7C340200,  //  01E2  CALL	R13	1
      0x8C341B44,  //  01E3  GETMET	R13	R13	K68
      0x7C340200,  //  01E4  CALL	R13	1
      0x8C341B45,  //  01E5  GETMET	R13	R13	K69
      0x7C340200,  //  01E6  CALL	R13	1
      0x8C341B51,  //  01E7  GETMET	R13	R13	K81
      0x7C340200,  //  01E8  CALL	R13	1
      0x60380008,  //  01E9  GETGBL	R14	G8
      0x5C3C1000,  //  01EA  MOVE	R15	R8
      0x7C380200,  //  01EB  CALL	R14	1
      0x7C2C0600,  //  01EC  CALL	R11	3
      0x58300010,  //  01ED  LDCONST	R12	K16
      0x7C240600,  //  01EE  CALL	R9	3
      0xB8260200,  //  01EF  GETNGBL	R9	K1
      0x8824132B,  //  01F0  GETMBR	R9	R9	K43
      0x900E3409,  //  01F1  SETMBR	R3	K26	R9
      0x4C240000,  //  01F2  LDNIL	R9
      0x80041200,  //  01F3  RET	1	R9
      0x70020034,  //  01F4  JMP		#022A
      0x54220009,  //  01F5  LDINT	R8	10
      0x1C200E08,  //  01F6  EQ	R8	R7	R8
      0x78220031,  //  01F7  JMPF	R8	#022A
      0x8C200506,  //  01F8  GETMET	R8	R2	K6
      0x58280005,  //  01F9  LDCONST	R10	K5
      0x7C200400,  //  01FA  CALL	R8	2
      0x60240008,  //  01FB  GETGBL	R9	G8
      0x5C281000,  //  01FC  MOVE	R10	R8
      0x7C240200,  //  01FD  CALL	R9	1
      0x0026A409,  //  01FE  ADD	R9	K82	R9
      0x900E5809,  //  01FF  SETMBR	R3	K44	R9
      0x60240010,  //  0200  GETGBL	R9	G16
      0x88280116,  //  0201  GETMBR	R10	R0	K22
      0x88281530,  //  0202  GETMBR	R10	R10	K48
      0x8C281553,  //  0203  GETMET	R10	R10	K83
      0x7C280200,  //  0204  CALL	R10	1
      0x7C240200,  //  0205  CALL	R9	1
      0xA8020010,  //  0206  EXBLK	0	#0218
      0x5C281200,  //  0207  MOVE	R10	R9
      0x7C280000,  //  0208  CALL	R10	0
      0x8C2C1554,  //  0209  GETMET	R11	R10	K84
      0x7C2C0200,  //  020A  CALL	R11	1
      0x1C2C1608,  //  020B  EQ	R11	R11	R8
      0x782E0008,  //  020C  JMPF	R11	#0216
      0xB82E4400,  //  020D  GETNGBL	R11	K34
      0x8C2C1755,  //  020E  GETMET	R11	R11	K85
      0x543607CF,  //  020F  LDINT	R13	2000
      0x84380000,  //  0210  CLOSURE	R14	P0
      0x7C2C0600,  //  0211  CALL	R11	3
      0x502C0200,  //  0212  LDBOOL	R11	1	0
      0xA0000000,  //  0213  CLOSE	R0
      0xA8040001,  //  0214  EXBLK	1	1
      0x80041600,  //  0215  RET	1	R11
      0xA0240000,  //  0216  CLOSE	R9
      0x7001FFEE,  //  0217  JMP		#0207
      0x58240056,  //  0218  LDCONST	R9	K86
      0xAC240200,  //  0219  CATCH	R9	1	0
      0xB0080000,  //  021A  RAISE	2	R0	R0
      0xB8264400,  //  021B  GETNGBL	R9	K34
      0x8C24132C,  //  021C  GETMET	R9	R9	K44
      0x602C0008,  //  021D  GETGBL	R11	G8
      0x5C301000,  //  021E  MOVE	R12	R8
      0x7C2C0200,  //  021F  CALL	R11	1
      0x002EAE0B,  //  0220  ADD	R11	K87	R11
      0x002C1758,  //  0221  ADD	R11	R11	K88
      0x5830000E,  //  0222  LDCONST	R12	K14
      0x7C240600,  //  0223  CALL	R9	3
      0xB8260200,  //  0224  GETNGBL	R9	K1
      0x88241359,  //  0225  GETMBR	R9	R9	K89
      0x900E3409,  //  0226  SETMBR	R3	K26	R9
      0x4C240000,  //  0227  LDNIL	R9
      0xA0000000,  //  0228  CLOSE	R0
      0x80041200,  //  0229  RET	1	R9
      0x7002009B,  //  022A  JMP		#02C7
      0x5422003B,  //  022B  LDINT	R8	60
      0x1C200C08,  //  022C  EQ	R8	R6	R8
      0x78220085,  //  022D  JMPF	R8	#02B4
      0x1C200F05,  //  022E  EQ	R8	R7	K5
      0x78220065,  //  022F  JMPF	R8	#0296
      0x8C200506,  //  0230  GETMET	R8	R2	K6
      0x58280005,  //  0231  LDCONST	R10	K5
      0x7C200400,  //  0232  CALL	R8	2
      0x8C240506,  //  0233  GETMET	R9	R2	K6
      0x582C0007,  //  0234  LDCONST	R11	K7
      0x7C240400,  //  0235  CALL	R9	2
      0x8C280506,  //  0236  GETMET	R10	R2	K6
      0x5830000E,  //  0237  LDCONST	R12	K14
      0x7C280400,  //  0238  CALL	R10	2
      0x8C2C0506,  //  0239  GETMET	R11	R2	K6
      0x58340010,  //  023A  LDCONST	R13	K16
      0x7C2C0400,  //  023B  CALL	R11	2
      0x8C300506,  //  023C  GETMET	R12	R2	K6
      0x543A0003,  //  023D  LDINT	R14	4
      0x7C300400,  //  023E  CALL	R12	2
      0xB8364400,  //  023F  GETNGBL	R13	K34
      0x8C341B2C,  //  0240  GETMET	R13	R13	K44
      0x603C0018,  //  0241  GETGBL	R15	G24
      0x5840005A,  //  0242  LDCONST	R16	K90
      0x5C441000,  //  0243  MOVE	R17	R8
      0x8C481351,  //  0244  GETMET	R18	R9	K81
      0x7C480200,  //  0245  CALL	R18	1
      0x5C4C1400,  //  0246  MOVE	R19	R10
      0x5C501600,  //  0247  MOVE	R20	R11
      0x8C541951,  //  0248  GETMET	R21	R12	K81
      0x7C540200,  //  0249  CALL	R21	1
      0x7C3C0C00,  //  024A  CALL	R15	6
      0x54420003,  //  024B  LDINT	R16	4
      0x7C340600,  //  024C  CALL	R13	3
      0x4C340000,  //  024D  LDNIL	R13
      0x1C34100D,  //  024E  EQ	R13	R8	R13
      0x7436000B,  //  024F  JMPT	R13	#025C
      0x4C340000,  //  0250  LDNIL	R13
      0x1C34120D,  //  0251  EQ	R13	R9	R13
      0x74360008,  //  0252  JMPT	R13	#025C
      0x4C340000,  //  0253  LDNIL	R13
      0x1C34140D,  //  0254  EQ	R13	R10	R13
      0x74360005,  //  0255  JMPT	R13	#025C
      0x4C340000,  //  0256  LDNIL	R13
      0x1C34160D,  //  0257  EQ	R13	R11	R13
      0x74360002,  //  0258  JMPT	R13	#025C
      0x4C340000,  //  0259  LDNIL	R13
      0x1C34180D,  //  025A  EQ	R13	R12	R13
      0x78360005,  //  025B  JMPF	R13	#0262
      0xB8360200,  //  025C  GETNGBL	R13	K1
      0x88341B5B,  //  025D  GETMBR	R13	R13	K91
      0x900E340D,  //  025E  SETMBR	R3	K26	R13
      0x4C340000,  //  025F  LDNIL	R13
      0xA0000000,  //  0260  CLOSE	R0
      0x80041A00,  //  0261  RET	1	R13
      0x6034000C,  //  0262  GETGBL	R13	G12
      0x5C381200,  //  0263  MOVE	R14	R9
      0x7C340200,  //  0264  CALL	R13	1
      0x543A001F,  //  0265  LDINT	R14	32
      0x543E0040,  //  0266  LDINT	R15	65
      0x00381C0F,  //  0267  ADD	R14	R14	R15
      0x20341A0E,  //  0268  NE	R13	R13	R14
      0x7436000B,  //  0269  JMPT	R13	#0276
      0x6034000C,  //  026A  GETGBL	R13	G12
      0x5C381800,  //  026B  MOVE	R14	R12
      0x7C340200,  //  026C  CALL	R13	1
      0x543A000F,  //  026D  LDINT	R14	16
      0x14341A0E,  //  026E  LT	R13	R13	R14
      0x74360005,  //  026F  JMPT	R13	#0276
      0x6034000C,  //  0270  GETGBL	R13	G12
      0x5C381800,  //  0271  MOVE	R14	R12
      0x7C340200,  //  0272  CALL	R13	1
      0x543A001F,  //  0273  LDINT	R14	32
      0x24341A0E,  //  0274  GT	R13	R13	R14
      0x7836000A,  //  0275  JMPF	R13	#0281
      0xB8364400,  //  0276  GETNGBL	R13	K34
      0x8C341B2C,  //  0277  GETMET	R13	R13	K44
      0x583C005C,  //  0278  LDCONST	R15	K92
      0x5840000E,  //  0279  LDCONST	R16	K14
      0x7C340600,  //  027A  CALL	R13	3
      0xB8360200,  //  027B  GETNGBL	R13	K1
      0x88341B5D,  //  027C  GETMBR	R13	R13	K93
      0x900E340D,  //  027D  SETMBR	R3	K26	R13
      0x4C340000,  //  027E  LDNIL	R13
      0xA0000000,  //  027F  CLOSE	R0
      0x80041A00,  //  0280  RET	1	R13
      0x5436001E,  //  0281  LDINT	R13	31
      0x40360A0D,  //  0282  CONNECT	R13	K5	R13
      0x9434120D,  //  0283  GETIDX	R13	R9	R13
      0x543A001F,  //  0284  LDINT	R14	32
      0x40381D40,  //  0285  CONNECT	R14	R14	K64
      0x9438120E,  //  0286  GETIDX	R14	R9	R14
      0x883C0116,  //  0287  GETMBR	R15	R0	K22
      0x8C3C1F5E,  //  0288  GETMET	R15	R15	K94
      0x5C441000,  //  0289  MOVE	R17	R8
      0x5C481600,  //  028A  MOVE	R18	R11
      0x5C4C1400,  //  028B  MOVE	R19	R10
      0x5C501800,  //  028C  MOVE	R20	R12
      0x5C541A00,  //  028D  MOVE	R21	R13
      0x5C581C00,  //  028E  MOVE	R22	R14
      0x8C5C035F,  //  028F  GETMET	R23	R1	K95
      0x7C5C0200,  //  0290  CALL	R23	1
      0x7C3C1000,  //  0291  CALL	R15	8
      0x503C0200,  //  0292  LDBOOL	R15	1	0
      0xA0000000,  //  0293  CLOSE	R0
      0x80041E00,  //  0294  RET	1	R15
      0x7002001C,  //  0295  JMP		#02B3
      0x1C200F07,  //  0296  EQ	R8	R7	K7
      0x78220012,  //  0297  JMPF	R8	#02AB
      0x8C200506,  //  0298  GETMET	R8	R2	K6
      0x58280005,  //  0299  LDCONST	R10	K5
      0x7C200400,  //  029A  CALL	R8	2
      0xB8264400,  //  029B  GETNGBL	R9	K34
      0x8C24132C,  //  029C  GETMET	R9	R9	K44
      0x602C0008,  //  029D  GETGBL	R11	G8
      0x5C301000,  //  029E  MOVE	R12	R8
      0x7C2C0200,  //  029F  CALL	R11	1
      0x002EC00B,  //  02A0  ADD	R11	K96	R11
      0x58300010,  //  02A1  LDCONST	R12	K16
      0x7C240600,  //  02A2  CALL	R9	3
      0x88240116,  //  02A3  GETMBR	R9	R0	K22
      0x8C241361,  //  02A4  GETMET	R9	R9	K97
      0x5C2C1000,  //  02A5  MOVE	R11	R8
      0x7C240400,  //  02A6  CALL	R9	2
      0x50240200,  //  02A7  LDBOOL	R9	1	0
      0xA0000000,  //  02A8  CLOSE	R0
      0x80041200,  //  02A9  RET	1	R9
      0x70020007,  //  02AA  JMP		#02B3
      0x1C200F0E,  //  02AB  EQ	R8	R7	K14
      0x78220005,  //  02AC  JMPF	R8	#02B3
      0x88200116,  //  02AD  GETMBR	R8	R0	K22
      0x8C201162,  //  02AE  GETMET	R8	R8	K98
      0x7C200200,  //  02AF  CALL	R8	1
      0x50200200,  //  02B0  LDBOOL	R8	1	0
      0xA0000000,  //  02B1  CLOSE	R0
      0x80041000,  //  02B2  RET	1	R8
      0x70020012,  //  02B3  JMP		#02C7
      0x54220029,  //  02B4  LDINT	R8	42
      0x1C200C08,  //  02B5  EQ	R8	R6	R8
      0x78220005,  //  02B6  JMPF	R8	#02BD
      0x1C200F05,  //  02B7  EQ	R8	R7	K5
      0x78220002,  //  02B8  JMPF	R8	#02BC
      0x50200200,  //  02B9  LDBOOL	R8	1	0
      0xA0000000,  //  02BA  CLOSE	R0
      0x80041000,  //  02BB  RET	1	R8
      0x70020009,  //  02BC  JMP		#02C7
      0x60200003,  //  02BD  GETGBL	R8	G3
      0x5C240000,  //  02BE  MOVE	R9	R0
      0x7C200200,  //  02BF  CALL	R8	1
      0x8C201163,  //  02C0  GETMET	R8	R8	K99
      0x5C280200,  //  02C1  MOVE	R10	R1
      0x5C2C0400,  //  02C2  MOVE	R11	R2
      0x5C300600,  //  02C3  MOVE	R12	R3
      0x7C200800,  //  02C4  CALL	R8	4
      0xA0000000,  //  02C5  CLOSE	R0
      0x80041000,  //  02C6  RET	1	R8
      0xA0000000,  //  02C7  CLOSE	R0
      0x80000000,  //  02C8  RET	0
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
        { be_const_key_weak(invoke_request, 6), be_const_closure(Matter_Plugin_Root_invoke_request_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(root) },
        { be_const_key_weak(TYPES, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(22, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(write_attribute, -1), be_const_closure(Matter_Plugin_Root_write_attribute_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Root_X20node) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(60, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(48, 9), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
    }))    ) } )) },
        { be_const_key_int(49, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(4),
        be_const_int(65532),
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
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(63, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(56, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(7),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(17),
    }))    ) } )) },
        { be_const_key_int(43, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
    }))    ) } )) },
        { be_const_key_int(44, 8), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
    }))    ) } )) },
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
