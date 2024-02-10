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
    ( &(const bvalue[94]) {     /* constants */
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
    /* K57  */  be_nested_str_weak(is_marked_for_deletion),
    /* K58  */  be_nested_str_weak(B2),
    /* K59  */  be_nested_str_weak(get_noc),
    /* K60  */  be_nested_str_weak(get_icac),
    /* K61  */  be_nested_str_weak(AGGREGATOR_ENDPOINT),
    /* K62  */  be_nested_str_weak(get_fabric_index),
    /* K63  */  be_nested_str_weak(stop_iteration),
    /* K64  */  be_nested_str_weak(parse),
    /* K65  */  be_nested_str_weak(get_ca),
    /* K66  */  be_nested_str_weak(findsubval),
    /* K67  */  be_nested_str_weak(get_admin_vendor),
    /* K68  */  be_nested_str_weak(get_fabric_id_as_int64),
    /* K69  */  be_nested_str_weak(get_device_id_as_int64),
    /* K70  */  be_nested_str_weak(get_fabric_label),
    /* K71  */  be_nested_str_weak(Fabric),
    /* K72  */  be_nested_str_weak(_MAX_CASE),
    /* K73  */  be_nested_str_weak(count_active_fabrics),
    /* K74  */  be_nested_str_weak(_fabric),
    /* K75  */  be_nested_str_weak(is_commissioning_open),
    /* K76  */  be_nested_str_weak(is_root_commissioning_open),
    /* K77  */  be_nested_str_weak(commissioning_admin_fabric),
    /* K78  */  be_nested_str_weak(Tasmota),
    /* K79  */  be_nested_str_weak(vendorid),
    /* K80  */  be_nested_str_weak(DeviceName),
    /* K81  */  be_nested_str_weak(FriendlyName),
    /* K82  */  be_nested_str_weak(FriendlyName1),
    /* K83  */  be_nested_str_weak(XX),
    /* K84  */  be_nested_str_weak(Status_X202),
    /* K85  */  be_nested_str_weak(StatusFWR),
    /* K86  */  be_nested_str_weak(Hardware),
    /* K87  */  be_nested_str_weak(Version),
    /* K88  */  be_nested_str_weak(_X28),
    /* K89  */  be_nested_str_weak(locale),
    /* K90  */  be_nested_str_weak(create_TLV),
    /* K91  */  be_nested_str_weak(get_active_endpoints),
    /* K92  */  be_nested_str_weak(disable_bridge_mode),
    /* K93  */  be_nested_str_weak(read_attribute),
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
      0x70020366,  //  0038  JMP		#03A0
      0x54220031,  //  0039  LDINT	R8	50
      0x1C200C08,  //  003A  EQ	R8	R6	R8
      0x78220000,  //  003B  JMPF	R8	#003D
      0x70020362,  //  003C  JMP		#03A0
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
      0x70020282,  //  011C  JMP		#03A0
      0x54220033,  //  011D  LDINT	R8	52
      0x1C200C08,  //  011E  EQ	R8	R6	R8
      0x78220000,  //  011F  JMPF	R8	#0121
      0x7002027E,  //  0120  JMP		#03A0
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
      0x7002024F,  //  014F  JMP		#03A0
      0x5422003D,  //  0150  LDINT	R8	62
      0x1C200C08,  //  0151  EQ	R8	R6	R8
      0x782200B6,  //  0152  JMPF	R8	#020A
      0x8C200133,  //  0153  GETMET	R8	R0	K51
      0x5C280400,  //  0154  MOVE	R10	R2
      0x7C200400,  //  0155  CALL	R8	2
      0x1C200F05,  //  0156  EQ	R8	R7	K5
      0x78220037,  //  0157  JMPF	R8	#0190
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
      0xA8020020,  //  0169  EXBLK	0	#018B
      0x5C2C1400,  //  016A  MOVE	R11	R10
      0x7C2C0000,  //  016B  CALL	R11	0
      0x8C301739,  //  016C  GETMET	R12	R11	K57
      0x7C300200,  //  016D  CALL	R12	1
      0x78320000,  //  016E  JMPF	R12	#0170
      0x7001FFF9,  //  016F  JMP		#016A
      0x4C300000,  //  0170  LDNIL	R12
      0x1C30160C,  //  0171  EQ	R12	R11	R12
      0x78320000,  //  0172  JMPF	R12	#0174
      0x7001FFF5,  //  0173  JMP		#016A
      0x8C301115,  //  0174  GETMET	R12	R8	K21
      0x4C380000,  //  0175  LDNIL	R14
      0x7C300400,  //  0176  CALL	R12	2
      0x8C34190B,  //  0177  GETMET	R13	R12	K11
      0x583C0009,  //  0178  LDCONST	R15	K9
      0x88400B3A,  //  0179  GETMBR	R16	R5	K58
      0x8C44173B,  //  017A  GETMET	R17	R11	K59
      0x7C440200,  //  017B  CALL	R17	1
      0x7C340800,  //  017C  CALL	R13	4
      0x8C34190B,  //  017D  GETMET	R13	R12	K11
      0x583C000D,  //  017E  LDCONST	R15	K13
      0x88400B3A,  //  017F  GETMBR	R16	R5	K58
      0x8C44173C,  //  0180  GETMET	R17	R11	K60
      0x7C440200,  //  0181  CALL	R17	1
      0x7C340800,  //  0182  CALL	R13	4
      0x8C34190B,  //  0183  GETMET	R13	R12	K11
      0xB83E0200,  //  0184  GETNGBL	R15	K1
      0x883C1F3D,  //  0185  GETMBR	R15	R15	K61
      0x88400B0C,  //  0186  GETMBR	R16	R5	K12
      0x8C44173E,  //  0187  GETMET	R17	R11	K62
      0x7C440200,  //  0188  CALL	R17	1
      0x7C340800,  //  0189  CALL	R13	4
      0x7001FFDE,  //  018A  JMP		#016A
      0x5828003F,  //  018B  LDCONST	R10	K63
      0xAC280200,  //  018C  CATCH	R10	1	0
      0xB0080000,  //  018D  RAISE	2	R0	R0
      0x80041000,  //  018E  RET	1	R8
      0x70020078,  //  018F  JMP		#0209
      0x1C200F09,  //  0190  EQ	R8	R7	K9
      0x7822004D,  //  0191  JMPF	R8	#01E0
      0x8C200B11,  //  0192  GETMET	R8	R5	K17
      0x7C200200,  //  0193  CALL	R8	1
      0x88240534,  //  0194  GETMBR	R9	R2	K52
      0x78260005,  //  0195  JMPF	R9	#019C
      0x60240012,  //  0196  GETGBL	R9	G18
      0x7C240000,  //  0197  CALL	R9	0
      0x8C280335,  //  0198  GETMET	R10	R1	K53
      0x7C280200,  //  0199  CALL	R10	1
      0x4028120A,  //  019A  CONNECT	R10	R9	R10
      0x70020003,  //  019B  JMP		#01A0
      0x88240136,  //  019C  GETMBR	R9	R0	K54
      0x88241337,  //  019D  GETMBR	R9	R9	K55
      0x8C241338,  //  019E  GETMET	R9	R9	K56
      0x7C240200,  //  019F  CALL	R9	1
      0x60280010,  //  01A0  GETGBL	R10	G16
      0x5C2C1200,  //  01A1  MOVE	R11	R9
      0x7C280200,  //  01A2  CALL	R10	1
      0xA8020036,  //  01A3  EXBLK	0	#01DB
      0x5C2C1400,  //  01A4  MOVE	R11	R10
      0x7C2C0000,  //  01A5  CALL	R11	0
      0x4C300000,  //  01A6  LDNIL	R12
      0x1C30160C,  //  01A7  EQ	R12	R11	R12
      0x78320000,  //  01A8  JMPF	R12	#01AA
      0x7001FFF9,  //  01A9  JMP		#01A4
      0x8C301739,  //  01AA  GETMET	R12	R11	K57
      0x7C300200,  //  01AB  CALL	R12	1
      0x78320000,  //  01AC  JMPF	R12	#01AE
      0x7001FFF5,  //  01AD  JMP		#01A4
      0x8C300B40,  //  01AE  GETMET	R12	R5	K64
      0x8C381741,  //  01AF  GETMET	R14	R11	K65
      0x7C380200,  //  01B0  CALL	R14	1
      0x7C300400,  //  01B1  CALL	R12	2
      0x8C341115,  //  01B2  GETMET	R13	R8	K21
      0x4C3C0000,  //  01B3  LDNIL	R15
      0x7C340400,  //  01B4  CALL	R13	2
      0x8C381B0B,  //  01B5  GETMET	R14	R13	K11
      0x58400009,  //  01B6  LDCONST	R16	K9
      0x88440B3A,  //  01B7  GETMBR	R17	R5	K58
      0x8C481942,  //  01B8  GETMET	R18	R12	K66
      0x54520008,  //  01B9  LDINT	R20	9
      0x7C480400,  //  01BA  CALL	R18	2
      0x7C380800,  //  01BB  CALL	R14	4
      0x8C381B0B,  //  01BC  GETMET	R14	R13	K11
      0x5840000D,  //  01BD  LDCONST	R16	K13
      0x88440B0C,  //  01BE  GETMBR	R17	R5	K12
      0x8C481743,  //  01BF  GETMET	R18	R11	K67
      0x7C480200,  //  01C0  CALL	R18	1
      0x7C380800,  //  01C1  CALL	R14	4
      0x8C381B0B,  //  01C2  GETMET	R14	R13	K11
      0x5840000F,  //  01C3  LDCONST	R16	K15
      0x88440B07,  //  01C4  GETMBR	R17	R5	K7
      0x8C481744,  //  01C5  GETMET	R18	R11	K68
      0x7C480200,  //  01C6  CALL	R18	1
      0x7C380800,  //  01C7  CALL	R14	4
      0x8C381B0B,  //  01C8  GETMET	R14	R13	K11
      0x54420003,  //  01C9  LDINT	R16	4
      0x88440B07,  //  01CA  GETMBR	R17	R5	K7
      0x8C481745,  //  01CB  GETMET	R18	R11	K69
      0x7C480200,  //  01CC  CALL	R18	1
      0x7C380800,  //  01CD  CALL	R14	4
      0x8C381B0B,  //  01CE  GETMET	R14	R13	K11
      0x54420004,  //  01CF  LDINT	R16	5
      0x88440B16,  //  01D0  GETMBR	R17	R5	K22
      0x8C481746,  //  01D1  GETMET	R18	R11	K70
      0x7C480200,  //  01D2  CALL	R18	1
      0x7C380800,  //  01D3  CALL	R14	4
      0x8C381B0B,  //  01D4  GETMET	R14	R13	K11
      0x544200FD,  //  01D5  LDINT	R16	254
      0x88440B0C,  //  01D6  GETMBR	R17	R5	K12
      0x8C48173E,  //  01D7  GETMET	R18	R11	K62
      0x7C480200,  //  01D8  CALL	R18	1
      0x7C380800,  //  01D9  CALL	R14	4
      0x7001FFC8,  //  01DA  JMP		#01A4
      0x5828003F,  //  01DB  LDCONST	R10	K63
      0xAC280200,  //  01DC  CATCH	R10	1	0
      0xB0080000,  //  01DD  RAISE	2	R0	R0
      0x80041000,  //  01DE  RET	1	R8
      0x70020028,  //  01DF  JMP		#0209
      0x1C200F0D,  //  01E0  EQ	R8	R7	K13
      0x78220007,  //  01E1  JMPF	R8	#01EA
      0x8C200706,  //  01E2  GETMET	R8	R3	K6
      0x88280B0E,  //  01E3  GETMBR	R10	R5	K14
      0xB82E0200,  //  01E4  GETNGBL	R11	K1
      0x882C1747,  //  01E5  GETMBR	R11	R11	K71
      0x882C1748,  //  01E6  GETMBR	R11	R11	K72
      0x7C200600,  //  01E7  CALL	R8	3
      0x80041000,  //  01E8  RET	1	R8
      0x7002001E,  //  01E9  JMP		#0209
      0x1C200F0F,  //  01EA  EQ	R8	R7	K15
      0x78220009,  //  01EB  JMPF	R8	#01F6
      0x88200136,  //  01EC  GETMBR	R8	R0	K54
      0x88201137,  //  01ED  GETMBR	R8	R8	K55
      0x8C201149,  //  01EE  GETMET	R8	R8	K73
      0x7C200200,  //  01EF  CALL	R8	1
      0x8C240706,  //  01F0  GETMET	R9	R3	K6
      0x882C0B0E,  //  01F1  GETMBR	R11	R5	K14
      0x5C301000,  //  01F2  MOVE	R12	R8
      0x7C240600,  //  01F3  CALL	R9	3
      0x80041200,  //  01F4  RET	1	R9
      0x70020012,  //  01F5  JMP		#0209
      0x54220003,  //  01F6  LDINT	R8	4
      0x1C200E08,  //  01F7  EQ	R8	R7	R8
      0x78220000,  //  01F8  JMPF	R8	#01FA
      0x7002000E,  //  01F9  JMP		#0209
      0x54220004,  //  01FA  LDINT	R8	5
      0x1C200E08,  //  01FB  EQ	R8	R7	R8
      0x7822000B,  //  01FC  JMPF	R8	#0209
      0x8820034A,  //  01FD  GETMBR	R8	R1	K74
      0x8C20113E,  //  01FE  GETMET	R8	R8	K62
      0x7C200200,  //  01FF  CALL	R8	1
      0x4C240000,  //  0200  LDNIL	R9
      0x1C241009,  //  0201  EQ	R9	R8	R9
      0x78260000,  //  0202  JMPF	R9	#0204
      0x58200005,  //  0203  LDCONST	R8	K5
      0x8C240706,  //  0204  GETMET	R9	R3	K6
      0x882C0B0E,  //  0205  GETMBR	R11	R5	K14
      0x5C301000,  //  0206  MOVE	R12	R8
      0x7C240600,  //  0207  CALL	R9	3
      0x80041200,  //  0208  RET	1	R9
      0x70020195,  //  0209  JMP		#03A0
      0x5422003B,  //  020A  LDINT	R8	60
      0x1C200C08,  //  020B  EQ	R8	R6	R8
      0x7822003C,  //  020C  JMPF	R8	#024A
      0x1C200F05,  //  020D  EQ	R8	R7	K5
      0x78220012,  //  020E  JMPF	R8	#0222
      0x88200136,  //  020F  GETMBR	R8	R0	K54
      0x8C20114B,  //  0210  GETMET	R8	R8	K75
      0x7C200200,  //  0211  CALL	R8	1
      0x88240136,  //  0212  GETMBR	R9	R0	K54
      0x8C24134C,  //  0213  GETMET	R9	R9	K76
      0x7C240200,  //  0214  CALL	R9	1
      0x78220004,  //  0215  JMPF	R8	#021B
      0x78260001,  //  0216  JMPF	R9	#0219
      0x5828000D,  //  0217  LDCONST	R10	K13
      0x70020000,  //  0218  JMP		#021A
      0x58280009,  //  0219  LDCONST	R10	K9
      0x70020000,  //  021A  JMP		#021C
      0x58280005,  //  021B  LDCONST	R10	K5
      0x8C2C0706,  //  021C  GETMET	R11	R3	K6
      0x88340B0E,  //  021D  GETMBR	R13	R5	K14
      0x5C381400,  //  021E  MOVE	R14	R10
      0x7C2C0600,  //  021F  CALL	R11	3
      0x80041600,  //  0220  RET	1	R11
      0x70020026,  //  0221  JMP		#0249
      0x1C200F09,  //  0222  EQ	R8	R7	K9
      0x78220011,  //  0223  JMPF	R8	#0236
      0x88200136,  //  0224  GETMBR	R8	R0	K54
      0x8820114D,  //  0225  GETMBR	R8	R8	K77
      0x4C240000,  //  0226  LDNIL	R9
      0x20241009,  //  0227  NE	R9	R8	R9
      0x78260006,  //  0228  JMPF	R9	#0230
      0x8C240706,  //  0229  GETMET	R9	R3	K6
      0x882C0B0C,  //  022A  GETMBR	R11	R5	K12
      0x8C30113E,  //  022B  GETMET	R12	R8	K62
      0x7C300200,  //  022C  CALL	R12	1
      0x7C240600,  //  022D  CALL	R9	3
      0x80041200,  //  022E  RET	1	R9
      0x70020004,  //  022F  JMP		#0235
      0x8C240706,  //  0230  GETMET	R9	R3	K6
      0x882C0B18,  //  0231  GETMBR	R11	R5	K24
      0x4C300000,  //  0232  LDNIL	R12
      0x7C240600,  //  0233  CALL	R9	3
      0x80041200,  //  0234  RET	1	R9
      0x70020012,  //  0235  JMP		#0249
      0x1C200F0D,  //  0236  EQ	R8	R7	K13
      0x78220010,  //  0237  JMPF	R8	#0249
      0x88200136,  //  0238  GETMBR	R8	R0	K54
      0x8820114D,  //  0239  GETMBR	R8	R8	K77
      0x4C240000,  //  023A  LDNIL	R9
      0x20241009,  //  023B  NE	R9	R8	R9
      0x78260006,  //  023C  JMPF	R9	#0244
      0x8C240706,  //  023D  GETMET	R9	R3	K6
      0x882C0B0C,  //  023E  GETMBR	R11	R5	K12
      0x8C301143,  //  023F  GETMET	R12	R8	K67
      0x7C300200,  //  0240  CALL	R12	1
      0x7C240600,  //  0241  CALL	R9	3
      0x80041200,  //  0242  RET	1	R9
      0x70020004,  //  0243  JMP		#0249
      0x8C240706,  //  0244  GETMET	R9	R3	K6
      0x882C0B18,  //  0245  GETMBR	R11	R5	K24
      0x4C300000,  //  0246  LDNIL	R12
      0x7C240600,  //  0247  CALL	R9	3
      0x80041200,  //  0248  RET	1	R9
      0x70020155,  //  0249  JMP		#03A0
      0x54220027,  //  024A  LDINT	R8	40
      0x1C200C08,  //  024B  EQ	R8	R6	R8
      0x782200BA,  //  024C  JMPF	R8	#0308
      0x8C200133,  //  024D  GETMET	R8	R0	K51
      0x5C280400,  //  024E  MOVE	R10	R2
      0x7C200400,  //  024F  CALL	R8	2
      0x1C200F05,  //  0250  EQ	R8	R7	K5
      0x78220005,  //  0251  JMPF	R8	#0258
      0x8C200706,  //  0252  GETMET	R8	R3	K6
      0x88280B0C,  //  0253  GETMBR	R10	R5	K12
      0x582C0009,  //  0254  LDCONST	R11	K9
      0x7C200600,  //  0255  CALL	R8	3
      0x80041000,  //  0256  RET	1	R8
      0x700200AE,  //  0257  JMP		#0307
      0x1C200F09,  //  0258  EQ	R8	R7	K9
      0x78220005,  //  0259  JMPF	R8	#0260
      0x8C200706,  //  025A  GETMET	R8	R3	K6
      0x88280B16,  //  025B  GETMBR	R10	R5	K22
      0x582C004E,  //  025C  LDCONST	R11	K78
      0x7C200600,  //  025D  CALL	R8	3
      0x80041000,  //  025E  RET	1	R8
      0x700200A6,  //  025F  JMP		#0307
      0x1C200F0D,  //  0260  EQ	R8	R7	K13
      0x78220006,  //  0261  JMPF	R8	#0269
      0x8C200706,  //  0262  GETMET	R8	R3	K6
      0x88280B0C,  //  0263  GETMBR	R10	R5	K12
      0x882C0136,  //  0264  GETMBR	R11	R0	K54
      0x882C174F,  //  0265  GETMBR	R11	R11	K79
      0x7C200600,  //  0266  CALL	R8	3
      0x80041000,  //  0267  RET	1	R8
      0x7002009D,  //  0268  JMP		#0307
      0x1C200F0F,  //  0269  EQ	R8	R7	K15
      0x7822000A,  //  026A  JMPF	R8	#0276
      0x8C200706,  //  026B  GETMET	R8	R3	K6
      0x88280B16,  //  026C  GETMBR	R10	R5	K22
      0xB82E2400,  //  026D  GETNGBL	R11	K18
      0x8C2C1726,  //  026E  GETMET	R11	R11	K38
      0x58340050,  //  026F  LDCONST	R13	K80
      0x50380200,  //  0270  LDBOOL	R14	1	0
      0x7C2C0600,  //  0271  CALL	R11	3
      0x942C1750,  //  0272  GETIDX	R11	R11	K80
      0x7C200600,  //  0273  CALL	R8	3
      0x80041000,  //  0274  RET	1	R8
      0x70020090,  //  0275  JMP		#0307
      0x54220003,  //  0276  LDINT	R8	4
      0x1C200E08,  //  0277  EQ	R8	R7	R8
      0x78220005,  //  0278  JMPF	R8	#027F
      0x8C200706,  //  0279  GETMET	R8	R3	K6
      0x88280B0C,  //  027A  GETMBR	R10	R5	K12
      0x542E7FFF,  //  027B  LDINT	R11	32768
      0x7C200600,  //  027C  CALL	R8	3
      0x80041000,  //  027D  RET	1	R8
      0x70020087,  //  027E  JMP		#0307
      0x54220004,  //  027F  LDINT	R8	5
      0x1C200E08,  //  0280  EQ	R8	R7	R8
      0x7822000A,  //  0281  JMPF	R8	#028D
      0x8C200706,  //  0282  GETMET	R8	R3	K6
      0x88280B16,  //  0283  GETMBR	R10	R5	K22
      0xB82E2400,  //  0284  GETNGBL	R11	K18
      0x8C2C1726,  //  0285  GETMET	R11	R11	K38
      0x58340051,  //  0286  LDCONST	R13	K81
      0x50380200,  //  0287  LDBOOL	R14	1	0
      0x7C2C0600,  //  0288  CALL	R11	3
      0x942C1752,  //  0289  GETIDX	R11	R11	K82
      0x7C200600,  //  028A  CALL	R8	3
      0x80041000,  //  028B  RET	1	R8
      0x70020079,  //  028C  JMP		#0307
      0x54220005,  //  028D  LDINT	R8	6
      0x1C200E08,  //  028E  EQ	R8	R7	R8
      0x78220005,  //  028F  JMPF	R8	#0296
      0x8C200706,  //  0290  GETMET	R8	R3	K6
      0x88280B16,  //  0291  GETMBR	R10	R5	K22
      0x582C0053,  //  0292  LDCONST	R11	K83
      0x7C200600,  //  0293  CALL	R8	3
      0x80041000,  //  0294  RET	1	R8
      0x70020070,  //  0295  JMP		#0307
      0x54220006,  //  0296  LDINT	R8	7
      0x1C200E08,  //  0297  EQ	R8	R7	R8
      0x78220005,  //  0298  JMPF	R8	#029F
      0x8C200706,  //  0299  GETMET	R8	R3	K6
      0x88280B0C,  //  029A  GETMBR	R10	R5	K12
      0x582C0005,  //  029B  LDCONST	R11	K5
      0x7C200600,  //  029C  CALL	R8	3
      0x80041000,  //  029D  RET	1	R8
      0x70020067,  //  029E  JMP		#0307
      0x54220007,  //  029F  LDINT	R8	8
      0x1C200E08,  //  02A0  EQ	R8	R7	R8
      0x7822000B,  //  02A1  JMPF	R8	#02AE
      0x8C200706,  //  02A2  GETMET	R8	R3	K6
      0x88280B16,  //  02A3  GETMBR	R10	R5	K22
      0xB82E2400,  //  02A4  GETNGBL	R11	K18
      0x8C2C1726,  //  02A5  GETMET	R11	R11	K38
      0x58340054,  //  02A6  LDCONST	R13	K84
      0x50380200,  //  02A7  LDBOOL	R14	1	0
      0x7C2C0600,  //  02A8  CALL	R11	3
      0x942C1755,  //  02A9  GETIDX	R11	R11	K85
      0x942C1756,  //  02AA  GETIDX	R11	R11	K86
      0x7C200600,  //  02AB  CALL	R8	3
      0x80041000,  //  02AC  RET	1	R8
      0x70020058,  //  02AD  JMP		#0307
      0x54220008,  //  02AE  LDINT	R8	9
      0x1C200E08,  //  02AF  EQ	R8	R7	R8
      0x78220005,  //  02B0  JMPF	R8	#02B7
      0x8C200706,  //  02B1  GETMET	R8	R3	K6
      0x88280B0C,  //  02B2  GETMBR	R10	R5	K12
      0x582C0009,  //  02B3  LDCONST	R11	K9
      0x7C200600,  //  02B4  CALL	R8	3
      0x80041000,  //  02B5  RET	1	R8
      0x7002004F,  //  02B6  JMP		#0307
      0x54220009,  //  02B7  LDINT	R8	10
      0x1C200E08,  //  02B8  EQ	R8	R7	R8
      0x78220015,  //  02B9  JMPF	R8	#02D0
      0xB8222400,  //  02BA  GETNGBL	R8	K18
      0x8C201126,  //  02BB  GETMET	R8	R8	K38
      0x58280054,  //  02BC  LDCONST	R10	K84
      0x502C0200,  //  02BD  LDBOOL	R11	1	0
      0x7C200600,  //  02BE  CALL	R8	3
      0x94201155,  //  02BF  GETIDX	R8	R8	K85
      0x94201157,  //  02C0  GETIDX	R8	R8	K87
      0x8C24091B,  //  02C1  GETMET	R9	R4	K27
      0x5C2C1000,  //  02C2  MOVE	R11	R8
      0x58300058,  //  02C3  LDCONST	R12	K88
      0x7C240600,  //  02C4  CALL	R9	3
      0x24281305,  //  02C5  GT	R10	R9	K5
      0x782A0002,  //  02C6  JMPF	R10	#02CA
      0x04281309,  //  02C7  SUB	R10	R9	K9
      0x402A0A0A,  //  02C8  CONNECT	R10	K5	R10
      0x9420100A,  //  02C9  GETIDX	R8	R8	R10
      0x8C280706,  //  02CA  GETMET	R10	R3	K6
      0x88300B16,  //  02CB  GETMBR	R12	R5	K22
      0x5C341000,  //  02CC  MOVE	R13	R8
      0x7C280600,  //  02CD  CALL	R10	3
      0x80041400,  //  02CE  RET	1	R10
      0x70020036,  //  02CF  JMP		#0307
      0x5422000E,  //  02D0  LDINT	R8	15
      0x1C200E08,  //  02D1  EQ	R8	R7	R8
      0x7822000B,  //  02D2  JMPF	R8	#02DF
      0x8C200706,  //  02D3  GETMET	R8	R3	K6
      0x88280B16,  //  02D4  GETMBR	R10	R5	K22
      0xB82E2400,  //  02D5  GETNGBL	R11	K18
      0x8C2C1725,  //  02D6  GETMET	R11	R11	K37
      0x7C2C0200,  //  02D7  CALL	R11	1
      0x8C2C171B,  //  02D8  GETMET	R11	R11	K27
      0x5834001C,  //  02D9  LDCONST	R13	K28
      0x5838001D,  //  02DA  LDCONST	R14	K29
      0x7C2C0600,  //  02DB  CALL	R11	3
      0x7C200600,  //  02DC  CALL	R8	3
      0x80041000,  //  02DD  RET	1	R8
      0x70020027,  //  02DE  JMP		#0307
      0x54220010,  //  02DF  LDINT	R8	17
      0x1C200E08,  //  02E0  EQ	R8	R7	R8
      0x78220005,  //  02E1  JMPF	R8	#02E8
      0x8C200706,  //  02E2  GETMET	R8	R3	K6
      0x88280B10,  //  02E3  GETMBR	R10	R5	K16
      0x582C0009,  //  02E4  LDCONST	R11	K9
      0x7C200600,  //  02E5  CALL	R8	3
      0x80041000,  //  02E6  RET	1	R8
      0x7002001E,  //  02E7  JMP		#0307
      0x54220011,  //  02E8  LDINT	R8	18
      0x1C200E08,  //  02E9  EQ	R8	R7	R8
      0x7822000B,  //  02EA  JMPF	R8	#02F7
      0x8C200706,  //  02EB  GETMET	R8	R3	K6
      0x88280B16,  //  02EC  GETMBR	R10	R5	K22
      0xB82E2400,  //  02ED  GETNGBL	R11	K18
      0x8C2C1725,  //  02EE  GETMET	R11	R11	K37
      0x7C2C0200,  //  02EF  CALL	R11	1
      0x8C2C171B,  //  02F0  GETMET	R11	R11	K27
      0x5834001C,  //  02F1  LDCONST	R13	K28
      0x5838001D,  //  02F2  LDCONST	R14	K29
      0x7C2C0600,  //  02F3  CALL	R11	3
      0x7C200600,  //  02F4  CALL	R8	3
      0x80041000,  //  02F5  RET	1	R8
      0x7002000F,  //  02F6  JMP		#0307
      0x54220012,  //  02F7  LDINT	R8	19
      0x1C200E08,  //  02F8  EQ	R8	R7	R8
      0x7822000C,  //  02F9  JMPF	R8	#0307
      0x8C200B0A,  //  02FA  GETMET	R8	R5	K10
      0x7C200200,  //  02FB  CALL	R8	1
      0x8C24110B,  //  02FC  GETMET	R9	R8	K11
      0x582C0005,  //  02FD  LDCONST	R11	K5
      0x88300B0C,  //  02FE  GETMBR	R12	R5	K12
      0x5834000F,  //  02FF  LDCONST	R13	K15
      0x7C240800,  //  0300  CALL	R9	4
      0x8C24110B,  //  0301  GETMET	R9	R8	K11
      0x582C0009,  //  0302  LDCONST	R11	K9
      0x88300B0C,  //  0303  GETMBR	R12	R5	K12
      0x5834000F,  //  0304  LDCONST	R13	K15
      0x7C240800,  //  0305  CALL	R9	4
      0x80041000,  //  0306  RET	1	R8
      0x70020097,  //  0307  JMP		#03A0
      0x5422003E,  //  0308  LDINT	R8	63
      0x1C200C08,  //  0309  EQ	R8	R6	R8
      0x78220000,  //  030A  JMPF	R8	#030C
      0x70020093,  //  030B  JMP		#03A0
      0x54220029,  //  030C  LDINT	R8	42
      0x1C200C08,  //  030D  EQ	R8	R6	R8
      0x7822001D,  //  030E  JMPF	R8	#032D
      0x1C200F05,  //  030F  EQ	R8	R7	K5
      0x78220003,  //  0310  JMPF	R8	#0315
      0x8C200B11,  //  0311  GETMET	R8	R5	K17
      0x7C200200,  //  0312  CALL	R8	1
      0x80041000,  //  0313  RET	1	R8
      0x70020016,  //  0314  JMP		#032C
      0x1C200F09,  //  0315  EQ	R8	R7	K9
      0x78220005,  //  0316  JMPF	R8	#031D
      0x8C200706,  //  0317  GETMET	R8	R3	K6
      0x88280B10,  //  0318  GETMBR	R10	R5	K16
      0x582C0005,  //  0319  LDCONST	R11	K5
      0x7C200600,  //  031A  CALL	R8	3
      0x80041000,  //  031B  RET	1	R8
      0x7002000E,  //  031C  JMP		#032C
      0x1C200F0D,  //  031D  EQ	R8	R7	K13
      0x78220005,  //  031E  JMPF	R8	#0325
      0x8C200706,  //  031F  GETMET	R8	R3	K6
      0x88280B0E,  //  0320  GETMBR	R10	R5	K14
      0x582C0009,  //  0321  LDCONST	R11	K9
      0x7C200600,  //  0322  CALL	R8	3
      0x80041000,  //  0323  RET	1	R8
      0x70020006,  //  0324  JMP		#032C
      0x1C200F0F,  //  0325  EQ	R8	R7	K15
      0x78220004,  //  0326  JMPF	R8	#032C
      0x8C200706,  //  0327  GETMET	R8	R3	K6
      0x88280B18,  //  0328  GETMBR	R10	R5	K24
      0x4C2C0000,  //  0329  LDNIL	R11
      0x7C200600,  //  032A  CALL	R8	3
      0x80041000,  //  032B  RET	1	R8
      0x70020072,  //  032C  JMP		#03A0
      0x5422002A,  //  032D  LDINT	R8	43
      0x1C200C08,  //  032E  EQ	R8	R6	R8
      0x78220016,  //  032F  JMPF	R8	#0347
      0x1C200F05,  //  0330  EQ	R8	R7	K5
      0x78220007,  //  0331  JMPF	R8	#033A
      0x8C200706,  //  0332  GETMET	R8	R3	K6
      0x88280B16,  //  0333  GETMBR	R10	R5	K22
      0xB82E2400,  //  0334  GETNGBL	R11	K18
      0x8C2C1759,  //  0335  GETMET	R11	R11	K89
      0x7C2C0200,  //  0336  CALL	R11	1
      0x7C200600,  //  0337  CALL	R8	3
      0x80041000,  //  0338  RET	1	R8
      0x7002000B,  //  0339  JMP		#0346
      0x1C200F09,  //  033A  EQ	R8	R7	K9
      0x78220009,  //  033B  JMPF	R8	#0346
      0x8C200B11,  //  033C  GETMET	R8	R5	K17
      0x7C200200,  //  033D  CALL	R8	1
      0x8C24110B,  //  033E  GETMET	R9	R8	K11
      0x4C2C0000,  //  033F  LDNIL	R11
      0x88300B16,  //  0340  GETMBR	R12	R5	K22
      0xB8362400,  //  0341  GETNGBL	R13	K18
      0x8C341B59,  //  0342  GETMET	R13	R13	K89
      0x7C340200,  //  0343  CALL	R13	1
      0x7C240800,  //  0344  CALL	R9	4
      0x80041000,  //  0345  RET	1	R8
      0x70020058,  //  0346  JMP		#03A0
      0x5422002B,  //  0347  LDINT	R8	44
      0x1C200C08,  //  0348  EQ	R8	R6	R8
      0x7822001C,  //  0349  JMPF	R8	#0367
      0x1C200F05,  //  034A  EQ	R8	R7	K5
      0x78220005,  //  034B  JMPF	R8	#0352
      0x8C200706,  //  034C  GETMET	R8	R3	K6
      0x88280B0E,  //  034D  GETMBR	R10	R5	K14
      0x582C0009,  //  034E  LDCONST	R11	K9
      0x7C200600,  //  034F  CALL	R8	3
      0x80041000,  //  0350  RET	1	R8
      0x70020013,  //  0351  JMP		#0366
      0x1C200F09,  //  0352  EQ	R8	R7	K9
      0x78220005,  //  0353  JMPF	R8	#035A
      0x8C200706,  //  0354  GETMET	R8	R3	K6
      0x88280B0E,  //  0355  GETMBR	R10	R5	K14
      0x542E0003,  //  0356  LDINT	R11	4
      0x7C200600,  //  0357  CALL	R8	3
      0x80041000,  //  0358  RET	1	R8
      0x7002000B,  //  0359  JMP		#0366
      0x1C200F0D,  //  035A  EQ	R8	R7	K13
      0x78220009,  //  035B  JMPF	R8	#0366
      0x8C200B11,  //  035C  GETMET	R8	R5	K17
      0x7C200200,  //  035D  CALL	R8	1
      0x8C24110B,  //  035E  GETMET	R9	R8	K11
      0x4C2C0000,  //  035F  LDNIL	R11
      0x8C300B5A,  //  0360  GETMET	R12	R5	K90
      0x88380B0E,  //  0361  GETMBR	R14	R5	K14
      0x543E0003,  //  0362  LDINT	R15	4
      0x7C300600,  //  0363  CALL	R12	3
      0x7C240600,  //  0364  CALL	R9	3
      0x80041000,  //  0365  RET	1	R8
      0x70020038,  //  0366  JMP		#03A0
      0x54220030,  //  0367  LDINT	R8	49
      0x1C200C08,  //  0368  EQ	R8	R6	R8
      0x78220007,  //  0369  JMPF	R8	#0372
      0x1C200F0F,  //  036A  EQ	R8	R7	K15
      0x78220004,  //  036B  JMPF	R8	#0371
      0x8C200706,  //  036C  GETMET	R8	R3	K6
      0x88280B0E,  //  036D  GETMBR	R10	R5	K14
      0x542E001D,  //  036E  LDINT	R11	30
      0x7C200600,  //  036F  CALL	R8	3
      0x80041000,  //  0370  RET	1	R8
      0x7002002D,  //  0371  JMP		#03A0
      0x5422001C,  //  0372  LDINT	R8	29
      0x1C200C08,  //  0373  EQ	R8	R6	R8
      0x7822002A,  //  0374  JMPF	R8	#03A0
      0x1C200F0D,  //  0375  EQ	R8	R7	K13
      0x78220008,  //  0376  JMPF	R8	#0380
      0x8C200B11,  //  0377  GETMET	R8	R5	K17
      0x7C200200,  //  0378  CALL	R8	1
      0x8C24110B,  //  0379  GETMET	R9	R8	K11
      0x4C2C0000,  //  037A  LDNIL	R11
      0x88300B0C,  //  037B  GETMBR	R12	R5	K12
      0x5436001E,  //  037C  LDINT	R13	31
      0x7C240800,  //  037D  CALL	R9	4
      0x80041000,  //  037E  RET	1	R8
      0x7002001F,  //  037F  JMP		#03A0
      0x1C200F0F,  //  0380  EQ	R8	R7	K15
      0x7822001D,  //  0381  JMPF	R8	#03A0
      0x8C200B11,  //  0382  GETMET	R8	R5	K17
      0x7C200200,  //  0383  CALL	R8	1
      0x88240136,  //  0384  GETMBR	R9	R0	K54
      0x8C24135B,  //  0385  GETMET	R9	R9	K91
      0x502C0200,  //  0386  LDBOOL	R11	1	0
      0x7C240400,  //  0387  CALL	R9	2
      0x88280136,  //  0388  GETMBR	R10	R0	K54
      0x8828155C,  //  0389  GETMBR	R10	R10	K92
      0x602C0010,  //  038A  GETGBL	R11	G16
      0x5C301200,  //  038B  MOVE	R12	R9
      0x7C2C0200,  //  038C  CALL	R11	1
      0xA802000D,  //  038D  EXBLK	0	#039C
      0x5C301600,  //  038E  MOVE	R12	R11
      0x7C300000,  //  038F  CALL	R12	0
      0x5C341400,  //  0390  MOVE	R13	R10
      0x78360003,  //  0391  JMPF	R13	#0396
      0xB8360200,  //  0392  GETNGBL	R13	K1
      0x88341B3D,  //  0393  GETMBR	R13	R13	K61
      0x2034180D,  //  0394  NE	R13	R12	R13
      0x78360004,  //  0395  JMPF	R13	#039B
      0x8C34110B,  //  0396  GETMET	R13	R8	K11
      0x4C3C0000,  //  0397  LDNIL	R15
      0x88400B0C,  //  0398  GETMBR	R16	R5	K12
      0x5C441800,  //  0399  MOVE	R17	R12
      0x7C340800,  //  039A  CALL	R13	4
      0x7001FFF1,  //  039B  JMP		#038E
      0x582C003F,  //  039C  LDCONST	R11	K63
      0xAC2C0200,  //  039D  CATCH	R11	1	0
      0xB0080000,  //  039E  RAISE	2	R0	R0
      0x80041000,  //  039F  RET	1	R8
      0x60200003,  //  03A0  GETGBL	R8	G3
      0x5C240000,  //  03A1  MOVE	R9	R0
      0x7C200200,  //  03A2  CALL	R8	1
      0x8C20115D,  //  03A3  GETMET	R8	R8	K93
      0x5C280200,  //  03A4  MOVE	R10	R1
      0x5C2C0400,  //  03A5  MOVE	R11	R2
      0x5C300600,  //  03A6  MOVE	R12	R3
      0x7C200800,  //  03A7  CALL	R8	4
      0x80041000,  //  03A8  RET	1	R8
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
    31,                          /* nstack */
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
    /* K78  */  be_nested_str_weak(assign_fabric_index),
    /* K79  */  be_nested_str_weak(get_fabric_index),
    /* K80  */  be_nested_str_weak(set_fabric_label),
    /* K81  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Update_X20fabric_X20_X27_X25s_X27_X20label_X3D_X27_X25s_X27),
    /* K82  */  be_nested_str_weak(get_fabric_id),
    /* K83  */  be_nested_str_weak(tohex),
    /* K84  */  be_nested_str_weak(get_fabric),
    /* K85  */  be_nested_str_weak(fabric_index_X3A),
    /* K86  */  be_nested_str_weak(active_fabrics),
    /* K87  */  be_nested_str_weak(mark_for_deletion),
    /* K88  */  be_nested_str_weak(set_timer),
    /* K89  */  be_nested_str_weak(stop_iteration),
    /* K90  */  be_nested_str_weak(MTR_X3A_X20RemoveFabric_X20fabric_X28),
    /* K91  */  be_nested_str_weak(_X29_X20not_X20found),
    /* K92  */  be_nested_str_weak(INVALID_ACTION),
    /* K93  */  be_nested_str_weak(MTR_X3A_X20OpenCommissioningWindow_X28timeout_X3D_X25i_X2C_X20passcode_X3D_X25s_X2C_X20discriminator_X3D_X25i_X2C_X20iterations_X3D_X25i_X2C_X20salt_X3D_X25s_X29),
    /* K94  */  be_nested_str_weak(INVALID_DATA_TYPE),
    /* K95  */  be_nested_str_weak(MTR_X3A_X20wrong_X20size_X20for_X20PAKE_X20parameters),
    /* K96  */  be_nested_str_weak(CONSTRAINT_ERROR),
    /* K97  */  be_nested_str_weak(start_basic_commissioning),
    /* K98  */  be_nested_str_weak(MTR_X3A_X20OpenBasicCommissioningWindow_X20commissioning_timeout_X3D),
    /* K99  */  be_nested_str_weak(start_root_basic_commissioning),
    /* K100 */  be_nested_str_weak(stop_basic_commissioning),
    /* K101 */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[745]) {  /* code */
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
      0x70020281,  //  0064  JMP		#02E7
      0x5422003D,  //  0065  LDINT	R8	62
      0x1C200C08,  //  0066  EQ	R8	R6	R8
      0x782201E2,  //  0067  JMPF	R8	#024B
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
      0x700201C1,  //  0087  JMP		#024A
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
      0x70020182,  //  00C6  JMP		#024A
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
      0x70020142,  //  0106  JMP		#024A
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
      0x70020133,  //  0115  JMP		#024A
      0x54220005,  //  0116  LDINT	R8	6
      0x1C200E08,  //  0117  EQ	R8	R7	R8
      0x782200BB,  //  0118  JMPF	R8	#01D5
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
      0x604C0015,  //  0196  GETGBL	R19	G21
      0x7C4C0000,  //  0197  CALL	R19	0
      0x8C4C2741,  //  0198  GETMET	R19	R19	K65
      0x58540042,  //  0199  LDCONST	R21	K66
      0x7C4C0400,  //  019A  CALL	R19	2
      0x8C500943,  //  019B  GETMET	R20	R4	K67
      0x7C500200,  //  019C  CALL	R20	1
      0x8C542144,  //  019D  GETMET	R21	R16	K68
      0x7C540200,  //  019E  CALL	R21	1
      0x8C542B45,  //  019F  GETMET	R21	R21	K69
      0x7C540200,  //  01A0  CALL	R21	1
      0x8C582946,  //  01A1  GETMET	R22	R20	K70
      0x5C602400,  //  01A2  MOVE	R24	R18
      0x5C642A00,  //  01A3  MOVE	R25	R21
      0x5C682600,  //  01A4  MOVE	R26	R19
      0x546E0007,  //  01A5  LDINT	R27	8
      0x7C580A00,  //  01A6  CALL	R22	5
      0x885C0312,  //  01A7  GETMBR	R23	R1	K18
      0x785E0001,  //  01A8  JMPF	R23	#01AB
      0x885C0312,  //  01A9  GETMBR	R23	R1	K18
      0x70020001,  //  01AA  JMP		#01AD
      0x885C0116,  //  01AB  GETMBR	R23	R0	K22
      0x885C2F47,  //  01AC  GETMBR	R23	R23	K71
      0x8C601B48,  //  01AD  GETMET	R24	R13	K72
      0x5C682000,  //  01AE  MOVE	R26	R16
      0x5C6C2200,  //  01AF  MOVE	R27	R17
      0x5C702C00,  //  01B0  MOVE	R28	R22
      0x5C742E00,  //  01B1  MOVE	R29	R23
      0x7C600A00,  //  01B2  CALL	R24	5
      0x8C601B49,  //  01B3  GETMET	R24	R13	K73
      0x7C600200,  //  01B4  CALL	R24	1
      0x88600116,  //  01B5  GETMBR	R24	R0	K22
      0x8C60314A,  //  01B6  GETMET	R24	R24	K74
      0x5C681A00,  //  01B7  MOVE	R26	R13
      0x7C600400,  //  01B8  CALL	R24	2
      0x8C60034B,  //  01B9  GETMET	R24	R1	K75
      0x7C600200,  //  01BA  CALL	R24	1
      0x78620002,  //  01BB  JMPF	R24	#01BF
      0x8C60034C,  //  01BC  GETMET	R24	R1	K76
      0x546A003B,  //  01BD  LDINT	R26	60
      0x7C600400,  //  01BE  CALL	R24	2
      0x8C601B4D,  //  01BF  GETMET	R24	R13	K77
      0x7C600200,  //  01C0  CALL	R24	1
      0x8C601B4E,  //  01C1  GETMET	R24	R13	K78
      0x7C600200,  //  01C2  CALL	R24	1
      0x8C600B09,  //  01C3  GETMET	R24	R5	K9
      0x7C600200,  //  01C4  CALL	R24	1
      0x8C64310A,  //  01C5  GETMET	R25	R24	K10
      0x586C0005,  //  01C6  LDCONST	R27	K5
      0x88700B0B,  //  01C7  GETMBR	R28	R5	K11
      0xB8760200,  //  01C8  GETNGBL	R29	K1
      0x88743B2B,  //  01C9  GETMBR	R29	R29	K43
      0x7C640800,  //  01CA  CALL	R25	4
      0x8C64310A,  //  01CB  GETMET	R25	R24	K10
      0x586C0007,  //  01CC  LDCONST	R27	K7
      0x88700B0B,  //  01CD  GETMBR	R28	R5	K11
      0x8C741B4F,  //  01CE  GETMET	R29	R13	K79
      0x7C740200,  //  01CF  CALL	R29	1
      0x7C640800,  //  01D0  CALL	R25	4
      0x54660007,  //  01D1  LDINT	R25	8
      0x900E0819,  //  01D2  SETMBR	R3	K4	R25
      0x80043000,  //  01D3  RET	1	R24
      0x70020074,  //  01D4  JMP		#024A
      0x54220008,  //  01D5  LDINT	R8	9
      0x1C200E08,  //  01D6  EQ	R8	R7	R8
      0x7822002C,  //  01D7  JMPF	R8	#0205
      0x8C200506,  //  01D8  GETMET	R8	R2	K6
      0x58280005,  //  01D9  LDCONST	R10	K5
      0x7C200400,  //  01DA  CALL	R8	2
      0x8C240350,  //  01DB  GETMET	R9	R1	K80
      0x5C2C1000,  //  01DC  MOVE	R11	R8
      0x7C240400,  //  01DD  CALL	R9	2
      0xB8264400,  //  01DE  GETNGBL	R9	K34
      0x8C24132C,  //  01DF  GETMET	R9	R9	K44
      0x602C0018,  //  01E0  GETGBL	R11	G24
      0x58300051,  //  01E1  LDCONST	R12	K81
      0x88340312,  //  01E2  GETMBR	R13	R1	K18
      0x8C341B52,  //  01E3  GETMET	R13	R13	K82
      0x7C340200,  //  01E4  CALL	R13	1
      0x8C341B44,  //  01E5  GETMET	R13	R13	K68
      0x7C340200,  //  01E6  CALL	R13	1
      0x8C341B45,  //  01E7  GETMET	R13	R13	K69
      0x7C340200,  //  01E8  CALL	R13	1
      0x8C341B53,  //  01E9  GETMET	R13	R13	K83
      0x7C340200,  //  01EA  CALL	R13	1
      0x60380008,  //  01EB  GETGBL	R14	G8
      0x5C3C1000,  //  01EC  MOVE	R15	R8
      0x7C380200,  //  01ED  CALL	R14	1
      0x7C2C0600,  //  01EE  CALL	R11	3
      0x58300010,  //  01EF  LDCONST	R12	K16
      0x7C240600,  //  01F0  CALL	R9	3
      0x8C240B09,  //  01F1  GETMET	R9	R5	K9
      0x7C240200,  //  01F2  CALL	R9	1
      0x8C28130A,  //  01F3  GETMET	R10	R9	K10
      0x58300005,  //  01F4  LDCONST	R12	K5
      0x88340B0B,  //  01F5  GETMBR	R13	R5	K11
      0xB83A0200,  //  01F6  GETNGBL	R14	K1
      0x88381D2B,  //  01F7  GETMBR	R14	R14	K43
      0x7C280800,  //  01F8  CALL	R10	4
      0x8C28130A,  //  01F9  GETMET	R10	R9	K10
      0x58300007,  //  01FA  LDCONST	R12	K7
      0x88340B0B,  //  01FB  GETMBR	R13	R5	K11
      0x8C380354,  //  01FC  GETMET	R14	R1	K84
      0x7C380200,  //  01FD  CALL	R14	1
      0x8C381D4F,  //  01FE  GETMET	R14	R14	K79
      0x7C380200,  //  01FF  CALL	R14	1
      0x7C280800,  //  0200  CALL	R10	4
      0x542A0007,  //  0201  LDINT	R10	8
      0x900E080A,  //  0202  SETMBR	R3	K4	R10
      0x80041200,  //  0203  RET	1	R9
      0x70020044,  //  0204  JMP		#024A
      0x54220009,  //  0205  LDINT	R8	10
      0x1C200E08,  //  0206  EQ	R8	R7	R8
      0x78220041,  //  0207  JMPF	R8	#024A
      0x8C200506,  //  0208  GETMET	R8	R2	K6
      0x58280005,  //  0209  LDCONST	R10	K5
      0x7C200400,  //  020A  CALL	R8	2
      0x60240008,  //  020B  GETGBL	R9	G8
      0x5C281000,  //  020C  MOVE	R10	R8
      0x7C240200,  //  020D  CALL	R9	1
      0x0026AA09,  //  020E  ADD	R9	K85	R9
      0x900E5809,  //  020F  SETMBR	R3	K44	R9
      0x60240010,  //  0210  GETGBL	R9	G16
      0x88280116,  //  0211  GETMBR	R10	R0	K22
      0x88281530,  //  0212  GETMBR	R10	R10	K48
      0x8C281556,  //  0213  GETMET	R10	R10	K86
      0x7C280200,  //  0214  CALL	R10	1
      0x7C240200,  //  0215  CALL	R9	1
      0xA8020020,  //  0216  EXBLK	0	#0238
      0x5C281200,  //  0217  MOVE	R10	R9
      0x7C280000,  //  0218  CALL	R10	0
      0x8C2C154F,  //  0219  GETMET	R11	R10	K79
      0x7C2C0200,  //  021A  CALL	R11	1
      0x1C2C1608,  //  021B  EQ	R11	R11	R8
      0x782E0018,  //  021C  JMPF	R11	#0236
      0x8C2C1557,  //  021D  GETMET	R11	R10	K87
      0x7C2C0200,  //  021E  CALL	R11	1
      0xB82E4400,  //  021F  GETNGBL	R11	K34
      0x8C2C1758,  //  0220  GETMET	R11	R11	K88
      0x543607CF,  //  0221  LDINT	R13	2000
      0x84380000,  //  0222  CLOSURE	R14	P0
      0x7C2C0600,  //  0223  CALL	R11	3
      0x8C2C0B09,  //  0224  GETMET	R11	R5	K9
      0x7C2C0200,  //  0225  CALL	R11	1
      0x8C30170A,  //  0226  GETMET	R12	R11	K10
      0x58380005,  //  0227  LDCONST	R14	K5
      0x883C0B0B,  //  0228  GETMBR	R15	R5	K11
      0xB8420200,  //  0229  GETNGBL	R16	K1
      0x8840212B,  //  022A  GETMBR	R16	R16	K43
      0x7C300800,  //  022B  CALL	R12	4
      0x8C30170A,  //  022C  GETMET	R12	R11	K10
      0x58380007,  //  022D  LDCONST	R14	K7
      0x883C0B0B,  //  022E  GETMBR	R15	R5	K11
      0x5C401000,  //  022F  MOVE	R16	R8
      0x7C300800,  //  0230  CALL	R12	4
      0x54320007,  //  0231  LDINT	R12	8
      0x900E080C,  //  0232  SETMBR	R3	K4	R12
      0xA0000000,  //  0233  CLOSE	R0
      0xA8040001,  //  0234  EXBLK	1	1
      0x80041600,  //  0235  RET	1	R11
      0xA0240000,  //  0236  CLOSE	R9
      0x7001FFDE,  //  0237  JMP		#0217
      0x58240059,  //  0238  LDCONST	R9	K89
      0xAC240200,  //  0239  CATCH	R9	1	0
      0xB0080000,  //  023A  RAISE	2	R0	R0
      0xB8264400,  //  023B  GETNGBL	R9	K34
      0x8C24132C,  //  023C  GETMET	R9	R9	K44
      0x602C0008,  //  023D  GETGBL	R11	G8
      0x5C301000,  //  023E  MOVE	R12	R8
      0x7C2C0200,  //  023F  CALL	R11	1
      0x002EB40B,  //  0240  ADD	R11	K90	R11
      0x002C175B,  //  0241  ADD	R11	R11	K91
      0x5830000E,  //  0242  LDCONST	R12	K14
      0x7C240600,  //  0243  CALL	R9	3
      0xB8260200,  //  0244  GETNGBL	R9	K1
      0x8824135C,  //  0245  GETMBR	R9	R9	K92
      0x900E3409,  //  0246  SETMBR	R3	K26	R9
      0x4C240000,  //  0247  LDNIL	R9
      0xA0000000,  //  0248  CLOSE	R0
      0x80041200,  //  0249  RET	1	R9
      0x7002009B,  //  024A  JMP		#02E7
      0x5422003B,  //  024B  LDINT	R8	60
      0x1C200C08,  //  024C  EQ	R8	R6	R8
      0x78220085,  //  024D  JMPF	R8	#02D4
      0x1C200F05,  //  024E  EQ	R8	R7	K5
      0x78220065,  //  024F  JMPF	R8	#02B6
      0x8C200506,  //  0250  GETMET	R8	R2	K6
      0x58280005,  //  0251  LDCONST	R10	K5
      0x7C200400,  //  0252  CALL	R8	2
      0x8C240506,  //  0253  GETMET	R9	R2	K6
      0x582C0007,  //  0254  LDCONST	R11	K7
      0x7C240400,  //  0255  CALL	R9	2
      0x8C280506,  //  0256  GETMET	R10	R2	K6
      0x5830000E,  //  0257  LDCONST	R12	K14
      0x7C280400,  //  0258  CALL	R10	2
      0x8C2C0506,  //  0259  GETMET	R11	R2	K6
      0x58340010,  //  025A  LDCONST	R13	K16
      0x7C2C0400,  //  025B  CALL	R11	2
      0x8C300506,  //  025C  GETMET	R12	R2	K6
      0x543A0003,  //  025D  LDINT	R14	4
      0x7C300400,  //  025E  CALL	R12	2
      0xB8364400,  //  025F  GETNGBL	R13	K34
      0x8C341B2C,  //  0260  GETMET	R13	R13	K44
      0x603C0018,  //  0261  GETGBL	R15	G24
      0x5840005D,  //  0262  LDCONST	R16	K93
      0x5C441000,  //  0263  MOVE	R17	R8
      0x8C481353,  //  0264  GETMET	R18	R9	K83
      0x7C480200,  //  0265  CALL	R18	1
      0x5C4C1400,  //  0266  MOVE	R19	R10
      0x5C501600,  //  0267  MOVE	R20	R11
      0x8C541953,  //  0268  GETMET	R21	R12	K83
      0x7C540200,  //  0269  CALL	R21	1
      0x7C3C0C00,  //  026A  CALL	R15	6
      0x54420003,  //  026B  LDINT	R16	4
      0x7C340600,  //  026C  CALL	R13	3
      0x4C340000,  //  026D  LDNIL	R13
      0x1C34100D,  //  026E  EQ	R13	R8	R13
      0x7436000B,  //  026F  JMPT	R13	#027C
      0x4C340000,  //  0270  LDNIL	R13
      0x1C34120D,  //  0271  EQ	R13	R9	R13
      0x74360008,  //  0272  JMPT	R13	#027C
      0x4C340000,  //  0273  LDNIL	R13
      0x1C34140D,  //  0274  EQ	R13	R10	R13
      0x74360005,  //  0275  JMPT	R13	#027C
      0x4C340000,  //  0276  LDNIL	R13
      0x1C34160D,  //  0277  EQ	R13	R11	R13
      0x74360002,  //  0278  JMPT	R13	#027C
      0x4C340000,  //  0279  LDNIL	R13
      0x1C34180D,  //  027A  EQ	R13	R12	R13
      0x78360005,  //  027B  JMPF	R13	#0282
      0xB8360200,  //  027C  GETNGBL	R13	K1
      0x88341B5E,  //  027D  GETMBR	R13	R13	K94
      0x900E340D,  //  027E  SETMBR	R3	K26	R13
      0x4C340000,  //  027F  LDNIL	R13
      0xA0000000,  //  0280  CLOSE	R0
      0x80041A00,  //  0281  RET	1	R13
      0x6034000C,  //  0282  GETGBL	R13	G12
      0x5C381200,  //  0283  MOVE	R14	R9
      0x7C340200,  //  0284  CALL	R13	1
      0x543A001F,  //  0285  LDINT	R14	32
      0x543E0040,  //  0286  LDINT	R15	65
      0x00381C0F,  //  0287  ADD	R14	R14	R15
      0x20341A0E,  //  0288  NE	R13	R13	R14
      0x7436000B,  //  0289  JMPT	R13	#0296
      0x6034000C,  //  028A  GETGBL	R13	G12
      0x5C381800,  //  028B  MOVE	R14	R12
      0x7C340200,  //  028C  CALL	R13	1
      0x543A000F,  //  028D  LDINT	R14	16
      0x14341A0E,  //  028E  LT	R13	R13	R14
      0x74360005,  //  028F  JMPT	R13	#0296
      0x6034000C,  //  0290  GETGBL	R13	G12
      0x5C381800,  //  0291  MOVE	R14	R12
      0x7C340200,  //  0292  CALL	R13	1
      0x543A001F,  //  0293  LDINT	R14	32
      0x24341A0E,  //  0294  GT	R13	R13	R14
      0x7836000A,  //  0295  JMPF	R13	#02A1
      0xB8364400,  //  0296  GETNGBL	R13	K34
      0x8C341B2C,  //  0297  GETMET	R13	R13	K44
      0x583C005F,  //  0298  LDCONST	R15	K95
      0x5840000E,  //  0299  LDCONST	R16	K14
      0x7C340600,  //  029A  CALL	R13	3
      0xB8360200,  //  029B  GETNGBL	R13	K1
      0x88341B60,  //  029C  GETMBR	R13	R13	K96
      0x900E340D,  //  029D  SETMBR	R3	K26	R13
      0x4C340000,  //  029E  LDNIL	R13
      0xA0000000,  //  029F  CLOSE	R0
      0x80041A00,  //  02A0  RET	1	R13
      0x5436001E,  //  02A1  LDINT	R13	31
      0x40360A0D,  //  02A2  CONNECT	R13	K5	R13
      0x9434120D,  //  02A3  GETIDX	R13	R9	R13
      0x543A001F,  //  02A4  LDINT	R14	32
      0x40381D40,  //  02A5  CONNECT	R14	R14	K64
      0x9438120E,  //  02A6  GETIDX	R14	R9	R14
      0x883C0116,  //  02A7  GETMBR	R15	R0	K22
      0x8C3C1F61,  //  02A8  GETMET	R15	R15	K97
      0x5C441000,  //  02A9  MOVE	R17	R8
      0x5C481600,  //  02AA  MOVE	R18	R11
      0x5C4C1400,  //  02AB  MOVE	R19	R10
      0x5C501800,  //  02AC  MOVE	R20	R12
      0x5C541A00,  //  02AD  MOVE	R21	R13
      0x5C581C00,  //  02AE  MOVE	R22	R14
      0x8C5C0354,  //  02AF  GETMET	R23	R1	K84
      0x7C5C0200,  //  02B0  CALL	R23	1
      0x7C3C1000,  //  02B1  CALL	R15	8
      0x503C0200,  //  02B2  LDBOOL	R15	1	0
      0xA0000000,  //  02B3  CLOSE	R0
      0x80041E00,  //  02B4  RET	1	R15
      0x7002001C,  //  02B5  JMP		#02D3
      0x1C200F07,  //  02B6  EQ	R8	R7	K7
      0x78220012,  //  02B7  JMPF	R8	#02CB
      0x8C200506,  //  02B8  GETMET	R8	R2	K6
      0x58280005,  //  02B9  LDCONST	R10	K5
      0x7C200400,  //  02BA  CALL	R8	2
      0xB8264400,  //  02BB  GETNGBL	R9	K34
      0x8C24132C,  //  02BC  GETMET	R9	R9	K44
      0x602C0008,  //  02BD  GETGBL	R11	G8
      0x5C301000,  //  02BE  MOVE	R12	R8
      0x7C2C0200,  //  02BF  CALL	R11	1
      0x002EC40B,  //  02C0  ADD	R11	K98	R11
      0x58300010,  //  02C1  LDCONST	R12	K16
      0x7C240600,  //  02C2  CALL	R9	3
      0x88240116,  //  02C3  GETMBR	R9	R0	K22
      0x8C241363,  //  02C4  GETMET	R9	R9	K99
      0x5C2C1000,  //  02C5  MOVE	R11	R8
      0x7C240400,  //  02C6  CALL	R9	2
      0x50240200,  //  02C7  LDBOOL	R9	1	0
      0xA0000000,  //  02C8  CLOSE	R0
      0x80041200,  //  02C9  RET	1	R9
      0x70020007,  //  02CA  JMP		#02D3
      0x1C200F0E,  //  02CB  EQ	R8	R7	K14
      0x78220005,  //  02CC  JMPF	R8	#02D3
      0x88200116,  //  02CD  GETMBR	R8	R0	K22
      0x8C201164,  //  02CE  GETMET	R8	R8	K100
      0x7C200200,  //  02CF  CALL	R8	1
      0x50200200,  //  02D0  LDBOOL	R8	1	0
      0xA0000000,  //  02D1  CLOSE	R0
      0x80041000,  //  02D2  RET	1	R8
      0x70020012,  //  02D3  JMP		#02E7
      0x54220029,  //  02D4  LDINT	R8	42
      0x1C200C08,  //  02D5  EQ	R8	R6	R8
      0x78220005,  //  02D6  JMPF	R8	#02DD
      0x1C200F05,  //  02D7  EQ	R8	R7	K5
      0x78220002,  //  02D8  JMPF	R8	#02DC
      0x50200200,  //  02D9  LDBOOL	R8	1	0
      0xA0000000,  //  02DA  CLOSE	R0
      0x80041000,  //  02DB  RET	1	R8
      0x70020009,  //  02DC  JMP		#02E7
      0x60200003,  //  02DD  GETGBL	R8	G3
      0x5C240000,  //  02DE  MOVE	R9	R0
      0x7C200200,  //  02DF  CALL	R8	1
      0x8C201165,  //  02E0  GETMET	R8	R8	K101
      0x5C280200,  //  02E1  MOVE	R10	R1
      0x5C2C0400,  //  02E2  MOVE	R11	R2
      0x5C300600,  //  02E3  MOVE	R12	R3
      0x7C200800,  //  02E4  CALL	R8	4
      0xA0000000,  //  02E5  CLOSE	R0
      0x80041000,  //  02E6  RET	1	R8
      0xA0000000,  //  02E7  CLOSE	R0
      0x80000000,  //  02E8  RET	0
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
        be_const_map( *     be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(56, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(7),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(29, 11), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(44, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(31, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(60, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(63, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(62, 13), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(49, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(4),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(50, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(51, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(8),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(52, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(43, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(40, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(21,
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
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(48, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(11,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
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
