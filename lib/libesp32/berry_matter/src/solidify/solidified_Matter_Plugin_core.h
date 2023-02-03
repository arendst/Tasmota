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
    14,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[43]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(create_TLV),
    /* K4   */  be_nested_str_weak(U8),
    /* K5   */  be_nested_str_weak(session),
    /* K6   */  be_nested_str_weak(breadcrumb),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(Matter_TLV_struct),
    /* K9   */  be_nested_str_weak(add_TLV),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(U1),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(BOOL),
    /* K15  */  be_nested_str_weak(Matter_TLV_list),
    /* K16  */  be_nested_str_weak(tasmota),
    /* K17  */  be_nested_str_weak(cmd),
    /* K18  */  be_nested_str_weak(Status_X201),
    /* K19  */  be_nested_str_weak(StatusPRM),
    /* K20  */  be_nested_str_weak(BootCount),
    /* K21  */  be_nested_str_weak(U4),
    /* K22  */  be_nested_str_weak(Status_X2011),
    /* K23  */  be_nested_str_weak(StatusSTS),
    /* K24  */  be_nested_str_weak(UptimeSec),
    /* K25  */  be_nested_str_weak(int64),
    /* K26  */  be_nested_str_weak(rtc),
    /* K27  */  be_nested_str_weak(utc),
    /* K28  */  be_const_int(1000000),
    /* K29  */  be_nested_str_weak(local),
    /* K30  */  be_nested_str_weak(UTF1),
    /* K31  */  be_nested_str_weak(Tasmota),
    /* K32  */  be_nested_str_weak(device),
    /* K33  */  be_nested_str_weak(vendorid),
    /* K34  */  be_nested_str_weak(DeviceName),
    /* K35  */  be_nested_str_weak(FriendlyName),
    /* K36  */  be_nested_str_weak(FriendlyName1),
    /* K37  */  be_nested_str_weak(XX),
    /* K38  */  be_nested_str_weak(Status_X202),
    /* K39  */  be_nested_str_weak(StatusFWR),
    /* K40  */  be_nested_str_weak(Hardware),
    /* K41  */  be_nested_str_weak(Version),
    /* K42  */  be_nested_str_weak(locale),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[391]) {  /* code */
      0xB8160000,  //  0000  GETNGBL	R5	K0
      0x88140B01,  //  0001  GETMBR	R5	R5	K1
      0x541A002F,  //  0002  LDINT	R6	48
      0x1C180606,  //  0003  EQ	R6	R3	R6
      0x781A0031,  //  0004  JMPF	R6	#0037
      0x1C180902,  //  0005  EQ	R6	R4	K2
      0x781A0006,  //  0006  JMPF	R6	#000E
      0x8C180B03,  //  0007  GETMET	R6	R5	K3
      0x88200B04,  //  0008  GETMBR	R8	R5	K4
      0x88240305,  //  0009  GETMBR	R9	R1	K5
      0x88241306,  //  000A  GETMBR	R9	R9	K6
      0x7C180600,  //  000B  CALL	R6	3
      0x80040C00,  //  000C  RET	1	R6
      0x70020027,  //  000D  JMP		#0036
      0x1C180907,  //  000E  EQ	R6	R4	K7
      0x781A000D,  //  000F  JMPF	R6	#001E
      0x8C180B08,  //  0010  GETMET	R6	R5	K8
      0x7C180200,  //  0011  CALL	R6	1
      0x8C1C0D09,  //  0012  GETMET	R7	R6	K9
      0x58240002,  //  0013  LDCONST	R9	K2
      0x88280B0A,  //  0014  GETMBR	R10	R5	K10
      0x542E003B,  //  0015  LDINT	R11	60
      0x7C1C0800,  //  0016  CALL	R7	4
      0x8C1C0D09,  //  0017  GETMET	R7	R6	K9
      0x58240007,  //  0018  LDCONST	R9	K7
      0x88280B0A,  //  0019  GETMBR	R10	R5	K10
      0x542E0383,  //  001A  LDINT	R11	900
      0x7C1C0800,  //  001B  CALL	R7	4
      0x80040C00,  //  001C  RET	1	R6
      0x70020017,  //  001D  JMP		#0036
      0x1C18090B,  //  001E  EQ	R6	R4	K11
      0x781A0005,  //  001F  JMPF	R6	#0026
      0x8C180B03,  //  0020  GETMET	R6	R5	K3
      0x88200B0C,  //  0021  GETMBR	R8	R5	K12
      0x5824000B,  //  0022  LDCONST	R9	K11
      0x7C180600,  //  0023  CALL	R6	3
      0x80040C00,  //  0024  RET	1	R6
      0x7002000F,  //  0025  JMP		#0036
      0x1C18090D,  //  0026  EQ	R6	R4	K13
      0x781A0005,  //  0027  JMPF	R6	#002E
      0x8C180B03,  //  0028  GETMET	R6	R5	K3
      0x88200B0C,  //  0029  GETMBR	R8	R5	K12
      0x5824000B,  //  002A  LDCONST	R9	K11
      0x7C180600,  //  002B  CALL	R6	3
      0x80040C00,  //  002C  RET	1	R6
      0x70020007,  //  002D  JMP		#0036
      0x541A0003,  //  002E  LDINT	R6	4
      0x1C180806,  //  002F  EQ	R6	R4	R6
      0x781A0004,  //  0030  JMPF	R6	#0036
      0x8C180B03,  //  0031  GETMET	R6	R5	K3
      0x88200B0E,  //  0032  GETMBR	R8	R5	K14
      0x50240000,  //  0033  LDBOOL	R9	0	0
      0x7C180600,  //  0034  CALL	R6	3
      0x80040C00,  //  0035  RET	1	R6
      0x7002014E,  //  0036  JMP		#0186
      0x541A0031,  //  0037  LDINT	R6	50
      0x1C180606,  //  0038  EQ	R6	R3	R6
      0x781A0000,  //  0039  JMPF	R6	#003B
      0x7002014A,  //  003A  JMP		#0186
      0x541A0032,  //  003B  LDINT	R6	51
      0x1C180606,  //  003C  EQ	R6	R3	R6
      0x781A0028,  //  003D  JMPF	R6	#0067
      0x1C180902,  //  003E  EQ	R6	R4	K2
      0x781A0003,  //  003F  JMPF	R6	#0044
      0x8C180B0F,  //  0040  GETMET	R6	R5	K15
      0x7C180200,  //  0041  CALL	R6	1
      0x80040C00,  //  0042  RET	1	R6
      0x70020021,  //  0043  JMP		#0066
      0x1C180907,  //  0044  EQ	R6	R4	K7
      0x781A000A,  //  0045  JMPF	R6	#0051
      0x8C180B03,  //  0046  GETMET	R6	R5	K3
      0x88200B0A,  //  0047  GETMBR	R8	R5	K10
      0xB8262000,  //  0048  GETNGBL	R9	K16
      0x8C241311,  //  0049  GETMET	R9	R9	K17
      0x582C0012,  //  004A  LDCONST	R11	K18
      0x7C240400,  //  004B  CALL	R9	2
      0x94241313,  //  004C  GETIDX	R9	R9	K19
      0x94241314,  //  004D  GETIDX	R9	R9	K20
      0x7C180600,  //  004E  CALL	R6	3
      0x80040C00,  //  004F  RET	1	R6
      0x70020014,  //  0050  JMP		#0066
      0x1C18090B,  //  0051  EQ	R6	R4	K11
      0x781A000A,  //  0052  JMPF	R6	#005E
      0x8C180B03,  //  0053  GETMET	R6	R5	K3
      0x88200B15,  //  0054  GETMBR	R8	R5	K21
      0xB8262000,  //  0055  GETNGBL	R9	K16
      0x8C241311,  //  0056  GETMET	R9	R9	K17
      0x582C0016,  //  0057  LDCONST	R11	K22
      0x7C240400,  //  0058  CALL	R9	2
      0x94241317,  //  0059  GETIDX	R9	R9	K23
      0x94241318,  //  005A  GETIDX	R9	R9	K24
      0x7C180600,  //  005B  CALL	R6	3
      0x80040C00,  //  005C  RET	1	R6
      0x70020007,  //  005D  JMP		#0066
      0x541A0007,  //  005E  LDINT	R6	8
      0x1C180806,  //  005F  EQ	R6	R4	R6
      0x781A0004,  //  0060  JMPF	R6	#0066
      0x8C180B03,  //  0061  GETMET	R6	R5	K3
      0x88200B0E,  //  0062  GETMBR	R8	R5	K14
      0x50240000,  //  0063  LDBOOL	R9	0	0
      0x7C180600,  //  0064  CALL	R6	3
      0x80040C00,  //  0065  RET	1	R6
      0x7002011E,  //  0066  JMP		#0186
      0x541A0033,  //  0067  LDINT	R6	52
      0x1C180606,  //  0068  EQ	R6	R3	R6
      0x781A0000,  //  0069  JMPF	R6	#006B
      0x7002011A,  //  006A  JMP		#0186
      0x541A0037,  //  006B  LDINT	R6	56
      0x1C180606,  //  006C  EQ	R6	R3	R6
      0x781A002C,  //  006D  JMPF	R6	#009B
      0x1C180902,  //  006E  EQ	R6	R4	K2
      0x781A000F,  //  006F  JMPF	R6	#0080
      0xB81A3200,  //  0070  GETNGBL	R6	K25
      0xB81E2000,  //  0071  GETNGBL	R7	K16
      0x8C1C0F1A,  //  0072  GETMET	R7	R7	K26
      0x7C1C0200,  //  0073  CALL	R7	1
      0x941C0F1B,  //  0074  GETIDX	R7	R7	K27
      0x7C180200,  //  0075  CALL	R6	1
      0xB81E3200,  //  0076  GETNGBL	R7	K25
      0x5820001C,  //  0077  LDCONST	R8	K28
      0x7C1C0200,  //  0078  CALL	R7	1
      0x08180C07,  //  0079  MUL	R6	R6	R7
      0x8C1C0B03,  //  007A  GETMET	R7	R5	K3
      0x88240B04,  //  007B  GETMBR	R9	R5	K4
      0x5C280C00,  //  007C  MOVE	R10	R6
      0x7C1C0600,  //  007D  CALL	R7	3
      0x80040E00,  //  007E  RET	1	R7
      0x70020019,  //  007F  JMP		#009A
      0x1C180907,  //  0080  EQ	R6	R4	K7
      0x781A0005,  //  0081  JMPF	R6	#0088
      0x8C180B03,  //  0082  GETMET	R6	R5	K3
      0x88200B0C,  //  0083  GETMBR	R8	R5	K12
      0x5824000D,  //  0084  LDCONST	R9	K13
      0x7C180600,  //  0085  CALL	R6	3
      0x80040C00,  //  0086  RET	1	R6
      0x70020011,  //  0087  JMP		#009A
      0x541A0006,  //  0088  LDINT	R6	7
      0x1C180806,  //  0089  EQ	R6	R4	R6
      0x781A000E,  //  008A  JMPF	R6	#009A
      0xB81A3200,  //  008B  GETNGBL	R6	K25
      0xB81E2000,  //  008C  GETNGBL	R7	K16
      0x8C1C0F1A,  //  008D  GETMET	R7	R7	K26
      0x7C1C0200,  //  008E  CALL	R7	1
      0x941C0F1D,  //  008F  GETIDX	R7	R7	K29
      0x7C180200,  //  0090  CALL	R6	1
      0xB81E3200,  //  0091  GETNGBL	R7	K25
      0x5820001C,  //  0092  LDCONST	R8	K28
      0x7C1C0200,  //  0093  CALL	R7	1
      0x08180C07,  //  0094  MUL	R6	R6	R7
      0x8C1C0B03,  //  0095  GETMET	R7	R5	K3
      0x88240B04,  //  0096  GETMBR	R9	R5	K4
      0x5C280C00,  //  0097  MOVE	R10	R6
      0x7C1C0600,  //  0098  CALL	R7	3
      0x80040E00,  //  0099  RET	1	R7
      0x700200EA,  //  009A  JMP		#0186
      0x541A003D,  //  009B  LDINT	R6	62
      0x1C180606,  //  009C  EQ	R6	R3	R6
      0x781A001D,  //  009D  JMPF	R6	#00BC
      0x1C180902,  //  009E  EQ	R6	R4	K2
      0x781A0000,  //  009F  JMPF	R6	#00A1
      0x70020019,  //  00A0  JMP		#00BB
      0x1C180907,  //  00A1  EQ	R6	R4	K7
      0x781A0000,  //  00A2  JMPF	R6	#00A4
      0x70020016,  //  00A3  JMP		#00BB
      0x1C18090B,  //  00A4  EQ	R6	R4	K11
      0x781A0005,  //  00A5  JMPF	R6	#00AC
      0x8C180B03,  //  00A6  GETMET	R6	R5	K3
      0x88200B0C,  //  00A7  GETMBR	R8	R5	K12
      0x54260004,  //  00A8  LDINT	R9	5
      0x7C180600,  //  00A9  CALL	R6	3
      0x80040C00,  //  00AA  RET	1	R6
      0x7002000E,  //  00AB  JMP		#00BB
      0x1C18090D,  //  00AC  EQ	R6	R4	K13
      0x781A0005,  //  00AD  JMPF	R6	#00B4
      0x8C180B03,  //  00AE  GETMET	R6	R5	K3
      0x88200B0C,  //  00AF  GETMBR	R8	R5	K12
      0x58240007,  //  00B0  LDCONST	R9	K7
      0x7C180600,  //  00B1  CALL	R6	3
      0x80040C00,  //  00B2  RET	1	R6
      0x70020006,  //  00B3  JMP		#00BB
      0x541A0003,  //  00B4  LDINT	R6	4
      0x1C180806,  //  00B5  EQ	R6	R4	R6
      0x781A0000,  //  00B6  JMPF	R6	#00B8
      0x70020002,  //  00B7  JMP		#00BB
      0x541A0004,  //  00B8  LDINT	R6	5
      0x1C180806,  //  00B9  EQ	R6	R4	R6
      0x7819FFFF,  //  00BA  JMPF	R6	#00BB
      0x700200C9,  //  00BB  JMP		#0186
      0x541A003B,  //  00BC  LDINT	R6	60
      0x1C180606,  //  00BD  EQ	R6	R3	R6
      0x781A0000,  //  00BE  JMPF	R6	#00C0
      0x700200C5,  //  00BF  JMP		#0186
      0x541A0027,  //  00C0  LDINT	R6	40
      0x1C180606,  //  00C1  EQ	R6	R3	R6
      0x781A0071,  //  00C2  JMPF	R6	#0135
      0x1C180902,  //  00C3  EQ	R6	R4	K2
      0x781A0005,  //  00C4  JMPF	R6	#00CB
      0x8C180B03,  //  00C5  GETMET	R6	R5	K3
      0x88200B0A,  //  00C6  GETMBR	R8	R5	K10
      0x58240002,  //  00C7  LDCONST	R9	K2
      0x7C180600,  //  00C8  CALL	R6	3
      0x80040C00,  //  00C9  RET	1	R6
      0x70020068,  //  00CA  JMP		#0134
      0x1C180907,  //  00CB  EQ	R6	R4	K7
      0x781A0005,  //  00CC  JMPF	R6	#00D3
      0x8C180B03,  //  00CD  GETMET	R6	R5	K3
      0x88200B1E,  //  00CE  GETMBR	R8	R5	K30
      0x5824001F,  //  00CF  LDCONST	R9	K31
      0x7C180600,  //  00D0  CALL	R6	3
      0x80040C00,  //  00D1  RET	1	R6
      0x70020060,  //  00D2  JMP		#0134
      0x1C18090B,  //  00D3  EQ	R6	R4	K11
      0x781A0006,  //  00D4  JMPF	R6	#00DC
      0x8C180B03,  //  00D5  GETMET	R6	R5	K3
      0x88200B0A,  //  00D6  GETMBR	R8	R5	K10
      0x88240120,  //  00D7  GETMBR	R9	R0	K32
      0x88241321,  //  00D8  GETMBR	R9	R9	K33
      0x7C180600,  //  00D9  CALL	R6	3
      0x80040C00,  //  00DA  RET	1	R6
      0x70020057,  //  00DB  JMP		#0134
      0x1C18090D,  //  00DC  EQ	R6	R4	K13
      0x781A0009,  //  00DD  JMPF	R6	#00E8
      0x8C180B03,  //  00DE  GETMET	R6	R5	K3
      0x88200B1E,  //  00DF  GETMBR	R8	R5	K30
      0xB8262000,  //  00E0  GETNGBL	R9	K16
      0x8C241311,  //  00E1  GETMET	R9	R9	K17
      0x582C0022,  //  00E2  LDCONST	R11	K34
      0x7C240400,  //  00E3  CALL	R9	2
      0x94241322,  //  00E4  GETIDX	R9	R9	K34
      0x7C180600,  //  00E5  CALL	R6	3
      0x80040C00,  //  00E6  RET	1	R6
      0x7002004B,  //  00E7  JMP		#0134
      0x541A0003,  //  00E8  LDINT	R6	4
      0x1C180806,  //  00E9  EQ	R6	R4	R6
      0x781A0005,  //  00EA  JMPF	R6	#00F1
      0x8C180B03,  //  00EB  GETMET	R6	R5	K3
      0x88200B0A,  //  00EC  GETMBR	R8	R5	K10
      0x54267FFF,  //  00ED  LDINT	R9	32768
      0x7C180600,  //  00EE  CALL	R6	3
      0x80040C00,  //  00EF  RET	1	R6
      0x70020042,  //  00F0  JMP		#0134
      0x541A0004,  //  00F1  LDINT	R6	5
      0x1C180806,  //  00F2  EQ	R6	R4	R6
      0x781A0009,  //  00F3  JMPF	R6	#00FE
      0x8C180B03,  //  00F4  GETMET	R6	R5	K3
      0x88200B1E,  //  00F5  GETMBR	R8	R5	K30
      0xB8262000,  //  00F6  GETNGBL	R9	K16
      0x8C241311,  //  00F7  GETMET	R9	R9	K17
      0x582C0023,  //  00F8  LDCONST	R11	K35
      0x7C240400,  //  00F9  CALL	R9	2
      0x94241324,  //  00FA  GETIDX	R9	R9	K36
      0x7C180600,  //  00FB  CALL	R6	3
      0x80040C00,  //  00FC  RET	1	R6
      0x70020035,  //  00FD  JMP		#0134
      0x541A0005,  //  00FE  LDINT	R6	6
      0x1C180806,  //  00FF  EQ	R6	R4	R6
      0x781A0005,  //  0100  JMPF	R6	#0107
      0x8C180B03,  //  0101  GETMET	R6	R5	K3
      0x88200B1E,  //  0102  GETMBR	R8	R5	K30
      0x58240025,  //  0103  LDCONST	R9	K37
      0x7C180600,  //  0104  CALL	R6	3
      0x80040C00,  //  0105  RET	1	R6
      0x7002002C,  //  0106  JMP		#0134
      0x541A0006,  //  0107  LDINT	R6	7
      0x1C180806,  //  0108  EQ	R6	R4	R6
      0x781A0005,  //  0109  JMPF	R6	#0110
      0x8C180B03,  //  010A  GETMET	R6	R5	K3
      0x88200B0A,  //  010B  GETMBR	R8	R5	K10
      0x58240002,  //  010C  LDCONST	R9	K2
      0x7C180600,  //  010D  CALL	R6	3
      0x80040C00,  //  010E  RET	1	R6
      0x70020023,  //  010F  JMP		#0134
      0x541A0007,  //  0110  LDINT	R6	8
      0x1C180806,  //  0111  EQ	R6	R4	R6
      0x781A000A,  //  0112  JMPF	R6	#011E
      0x8C180B03,  //  0113  GETMET	R6	R5	K3
      0x88200B1E,  //  0114  GETMBR	R8	R5	K30
      0xB8262000,  //  0115  GETNGBL	R9	K16
      0x8C241311,  //  0116  GETMET	R9	R9	K17
      0x582C0026,  //  0117  LDCONST	R11	K38
      0x7C240400,  //  0118  CALL	R9	2
      0x94241327,  //  0119  GETIDX	R9	R9	K39
      0x94241328,  //  011A  GETIDX	R9	R9	K40
      0x7C180600,  //  011B  CALL	R6	3
      0x80040C00,  //  011C  RET	1	R6
      0x70020015,  //  011D  JMP		#0134
      0x541A0008,  //  011E  LDINT	R6	9
      0x1C180806,  //  011F  EQ	R6	R4	R6
      0x781A0005,  //  0120  JMPF	R6	#0127
      0x8C180B03,  //  0121  GETMET	R6	R5	K3
      0x88200B0A,  //  0122  GETMBR	R8	R5	K10
      0x58240002,  //  0123  LDCONST	R9	K2
      0x7C180600,  //  0124  CALL	R6	3
      0x80040C00,  //  0125  RET	1	R6
      0x7002000C,  //  0126  JMP		#0134
      0x541A0009,  //  0127  LDINT	R6	10
      0x1C180806,  //  0128  EQ	R6	R4	R6
      0x781A0009,  //  0129  JMPF	R6	#0134
      0x8C180B03,  //  012A  GETMET	R6	R5	K3
      0x88200B1E,  //  012B  GETMBR	R8	R5	K30
      0xB8262000,  //  012C  GETNGBL	R9	K16
      0x8C241311,  //  012D  GETMET	R9	R9	K17
      0x582C0026,  //  012E  LDCONST	R11	K38
      0x7C240400,  //  012F  CALL	R9	2
      0x94241327,  //  0130  GETIDX	R9	R9	K39
      0x94241329,  //  0131  GETIDX	R9	R9	K41
      0x7C180600,  //  0132  CALL	R6	3
      0x80040C00,  //  0133  RET	1	R6
      0x70020050,  //  0134  JMP		#0186
      0x541A003E,  //  0135  LDINT	R6	63
      0x1C180606,  //  0136  EQ	R6	R3	R6
      0x781A0000,  //  0137  JMPF	R6	#0139
      0x7002004C,  //  0138  JMP		#0186
      0x541A002A,  //  0139  LDINT	R6	43
      0x1C180606,  //  013A  EQ	R6	R3	R6
      0x781A0016,  //  013B  JMPF	R6	#0153
      0x1C180902,  //  013C  EQ	R6	R4	K2
      0x781A0007,  //  013D  JMPF	R6	#0146
      0x8C180B03,  //  013E  GETMET	R6	R5	K3
      0x88200B1E,  //  013F  GETMBR	R8	R5	K30
      0xB8262000,  //  0140  GETNGBL	R9	K16
      0x8C24132A,  //  0141  GETMET	R9	R9	K42
      0x7C240200,  //  0142  CALL	R9	1
      0x7C180600,  //  0143  CALL	R6	3
      0x80040C00,  //  0144  RET	1	R6
      0x7002000B,  //  0145  JMP		#0152
      0x1C180907,  //  0146  EQ	R6	R4	K7
      0x781A0009,  //  0147  JMPF	R6	#0152
      0x8C180B0F,  //  0148  GETMET	R6	R5	K15
      0x7C180200,  //  0149  CALL	R6	1
      0x8C1C0D09,  //  014A  GETMET	R7	R6	K9
      0x4C240000,  //  014B  LDNIL	R9
      0x88280B1E,  //  014C  GETMBR	R10	R5	K30
      0xB82E2000,  //  014D  GETNGBL	R11	K16
      0x8C2C172A,  //  014E  GETMET	R11	R11	K42
      0x7C2C0200,  //  014F  CALL	R11	1
      0x7C1C0800,  //  0150  CALL	R7	4
      0x80040C00,  //  0151  RET	1	R6
      0x70020032,  //  0152  JMP		#0186
      0x541A002B,  //  0153  LDINT	R6	44
      0x1C180606,  //  0154  EQ	R6	R3	R6
      0x781A001C,  //  0155  JMPF	R6	#0173
      0x1C180902,  //  0156  EQ	R6	R4	K2
      0x781A0005,  //  0157  JMPF	R6	#015E
      0x8C180B03,  //  0158  GETMET	R6	R5	K3
      0x88200B0C,  //  0159  GETMBR	R8	R5	K12
      0x58240007,  //  015A  LDCONST	R9	K7
      0x7C180600,  //  015B  CALL	R6	3
      0x80040C00,  //  015C  RET	1	R6
      0x70020013,  //  015D  JMP		#0172
      0x1C180907,  //  015E  EQ	R6	R4	K7
      0x781A0005,  //  015F  JMPF	R6	#0166
      0x8C180B03,  //  0160  GETMET	R6	R5	K3
      0x88200B0C,  //  0161  GETMBR	R8	R5	K12
      0x54260003,  //  0162  LDINT	R9	4
      0x7C180600,  //  0163  CALL	R6	3
      0x80040C00,  //  0164  RET	1	R6
      0x7002000B,  //  0165  JMP		#0172
      0x1C18090B,  //  0166  EQ	R6	R4	K11
      0x781A0009,  //  0167  JMPF	R6	#0172
      0x8C180B0F,  //  0168  GETMET	R6	R5	K15
      0x7C180200,  //  0169  CALL	R6	1
      0x8C1C0D09,  //  016A  GETMET	R7	R6	K9
      0x4C240000,  //  016B  LDNIL	R9
      0x8C280B03,  //  016C  GETMET	R10	R5	K3
      0x88300B0C,  //  016D  GETMBR	R12	R5	K12
      0x54360003,  //  016E  LDINT	R13	4
      0x7C280600,  //  016F  CALL	R10	3
      0x7C1C0600,  //  0170  CALL	R7	3
      0x80040C00,  //  0171  RET	1	R6
      0x70020012,  //  0172  JMP		#0186
      0x541A0030,  //  0173  LDINT	R6	49
      0x1C180606,  //  0174  EQ	R6	R3	R6
      0x781A000F,  //  0175  JMPF	R6	#0186
      0x1C18090D,  //  0176  EQ	R6	R4	K13
      0x781A0005,  //  0177  JMPF	R6	#017E
      0x8C180B03,  //  0178  GETMET	R6	R5	K3
      0x88200B0C,  //  0179  GETMBR	R8	R5	K12
      0x5426001D,  //  017A  LDINT	R9	30
      0x7C180600,  //  017B  CALL	R6	3
      0x80040C00,  //  017C  RET	1	R6
      0x70020007,  //  017D  JMP		#0186
      0x541AFFFB,  //  017E  LDINT	R6	65532
      0x1C180806,  //  017F  EQ	R6	R4	R6
      0x781A0004,  //  0180  JMPF	R6	#0186
      0x8C180B03,  //  0181  GETMET	R6	R5	K3
      0x88200B15,  //  0182  GETMBR	R8	R5	K21
      0x58240002,  //  0183  LDCONST	R9	K2
      0x7C180600,  //  0184  CALL	R6	3
      0x80040C00,  //  0185  RET	1	R6
      0x80000000,  //  0186  RET	0
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
    ( &(const bvalue[66]) {     /* constants */
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
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[435]) {  /* code */
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
      0x70020157,  //  0059  JMP		#01B2
      0x5426003D,  //  005A  LDINT	R9	62
      0x1C240C09,  //  005B  EQ	R9	R6	R9
      0x78260154,  //  005C  JMPF	R9	#01B2
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
      0x70020134,  //  007C  JMP		#01B2
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
      0x700200ED,  //  00C3  JMP		#01B2
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
      0x700200A9,  //  0107  JMP		#01B2
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
      0x70020093,  //  011D  JMP		#01B2
      0x54260005,  //  011E  LDINT	R9	6
      0x1C240E09,  //  011F  EQ	R9	R7	R9
      0x78260090,  //  0120  JMPF	R9	#01B2
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
      0x80000000,  //  01B2  RET	0
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
