/* Solidification of Matter_TLV.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Matter_TLV_item;
// compact class 'Matter_TLV_item' ktab size: 66, total: 149 (saved 664 bytes)
static const bvalue be_ktab_class_Matter_TLV_item[66] = {
  /* K0   */  be_nested_str_weak(parent),
  /* K1   */  be_nested_str_weak(next_idx),
  /* K2   */  be_nested_str_weak(tag_vendor),
  /* K3   */  be_nested_str_weak(tag_profile),
  /* K4   */  be_nested_str_weak(tag_number),
  /* K5   */  be_nested_str_weak(tag_sub),
  /* K6   */  be_nested_str_weak(typ),
  /* K7   */  be_nested_str_weak(val),
  /* K8   */  be_nested_str_weak(reset),
  /* K9   */  be_nested_str_weak(TLV),
  /* K10  */  be_const_int(0),
  /* K11  */  be_const_int(1),
  /* K12  */  be_const_int(2),
  /* K13  */  be_nested_str_weak(_encode_tag_len),
  /* K14  */  be_const_int(3),
  /* K15  */  be_nested_str_weak(value_error),
  /* K16  */  be_nested_str_weak(Unsupported_X20type_X200x0B_X20_X23_X2DTLV_X2EDOUBLE_X2D_X23),
  /* K17  */  be_nested_str_weak(unsupported_X20type_X20),
  /* K18  */  be_nested_str_weak(),
  /* K19  */  be_nested_str_weak(Matter_X3A_X3A),
  /* K20  */  be_nested_str_weak(0x_X2508X_X20),
  /* K21  */  be_nested_str_weak(0x_X2504X_X3A_X3A),
  /* K22  */  be_nested_str_weak(0x_X2504X_X3A),
  /* K23  */  be_nested_str_weak(_X25i_X20),
  /* K24  */  be_nested_str_weak(_X3D_X20),
  /* K25  */  be_nested_str_weak(int),
  /* K26  */  be_nested_str_weak(_X25i),
  /* K27  */  be_nested_str_weak(U),
  /* K28  */  be_nested_str_weak(bool),
  /* K29  */  be_nested_str_weak(true),
  /* K30  */  be_nested_str_weak(false),
  /* K31  */  be_nested_str_weak(null),
  /* K32  */  be_nested_str_weak(real),
  /* K33  */  be_nested_str_weak(_X25g),
  /* K34  */  be_nested_str_weak(string),
  /* K35  */  be_nested_str_weak(_X22_X25s_X22),
  /* K36  */  be_nested_str_weak(int64),
  /* K37  */  be_nested_str_weak(tostring),
  /* K38  */  be_nested_str_weak(instance),
  /* K39  */  be_nested_str_weak(_X25s),
  /* K40  */  be_nested_str_weak(tohex),
  /* K41  */  be_nested_str_weak(_X20),
  /* K42  */  be_nested_str_weak(_len),
  /* K43  */  be_nested_str_weak(frombytes),
  /* K44  */  be_nested_str_weak(geti),
  /* K45  */  be_nested_str_weak(get),
  /* K46  */  be_nested_str_weak(getfloat),
  /* K47  */  be_nested_str_weak(asstring),
  /* K48  */  be_nested_str_weak(log),
  /* K49  */  be_nested_str_weak(MTR_X3A_X20unexpected_X20eoc),
  /* K50  */  be_nested_str_weak(MTR_X3A_X20unexpected_X20type_X3A_X20),
  /* K51  */  be_nested_str_weak(set_fulltag),
  /* K52  */  be_nested_str_weak(add),
  /* K53  */  be_const_class(be_class_Matter_TLV_item),
  /* K54  */  be_nested_str_weak(_cmp_gt),
  /* K55  */  be_nested_str_weak(stop_iteration),
  /* K56  */  be_nested_str_weak(_encode_tag),
  /* K57  */  be_nested_str_weak(copy),
  /* K58  */  be_nested_str_weak(resize),
  /* K59  */  be_nested_str_weak(tobytes),
  /* K60  */  be_nested_str_weak(fromu32),
  /* K61  */  be_nested_str_weak(setfloat),
  /* K62  */  be_nested_str_weak(string_X20too_X20big),
  /* K63  */  be_nested_str_weak(fromstring),
  /* K64  */  be_nested_str_weak(frostring),
  /* K65  */  be_nested_str_weak(bytes_X20too_X20big),
};


extern const bclass be_class_Matter_TLV_item;

/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(class_Matter_TLV_item_reset,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x90020202,  //  0002  SETMBR	R0	K1	R2
      0x90020402,  //  0003  SETMBR	R0	K2	R2
      0x90020602,  //  0004  SETMBR	R0	K3	R2
      0x90020802,  //  0005  SETMBR	R0	K4	R2
      0x90020A02,  //  0006  SETMBR	R0	K5	R2
      0x90020C02,  //  0007  SETMBR	R0	K6	R2
      0x90020E02,  //  0008  SETMBR	R0	K7	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_or_nil
********************************************************************/
be_local_closure(class_Matter_TLV_item_set_or_nil,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(set_or_nil),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C0C0108,  //  0000  GETMET	R3	R0	K8
      0x7C0C0200,  //  0001  CALL	R3	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0403,  //  0003  EQ	R3	R2	R3
      0x780E0000,  //  0004  JMPF	R3	#0006
      0x54060013,  //  0005  LDINT	R1	20
      0x4C0C0000,  //  0006  LDNIL	R3
      0x200C0403,  //  0007  NE	R3	R2	R3
      0x740E0002,  //  0008  JMPT	R3	#000C
      0x540E0013,  //  0009  LDINT	R3	20
      0x1C0C0203,  //  000A  EQ	R3	R1	R3
      0x780E0002,  //  000B  JMPF	R3	#000F
      0x90020C01,  //  000C  SETMBR	R0	K6	R1
      0x90020E02,  //  000D  SETMBR	R0	K7	R2
      0x80040000,  //  000E  RET	1	R0
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: encode_len
********************************************************************/
be_local_closure(class_Matter_TLV_item_encode_len,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(encode_len),
    &be_const_str_solidified,
    ( &(const binstruction[250]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x5808000A,  //  0001  LDCONST	R2	K10
      0x880C0106,  //  0002  GETMBR	R3	R0	K6
      0x541200FE,  //  0003  LDINT	R4	255
      0x1C0C0604,  //  0004  EQ	R3	R3	R4
      0x780E0003,  //  0005  JMPF	R3	#000A
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x88100107,  //  0007  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0008  CALL	R3	1
      0x80040600,  //  0009  RET	1	R3
      0x880C0106,  //  000A  GETMBR	R3	R0	K6
      0x54120007,  //  000B  LDINT	R4	8
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x740E0003,  //  000D  JMPT	R3	#0012
      0x880C0106,  //  000E  GETMBR	R3	R0	K6
      0x54120008,  //  000F  LDINT	R4	9
      0x1C0C0604,  //  0010  EQ	R3	R3	R4
      0x780E0008,  //  0011  JMPF	R3	#001B
      0x600C0017,  //  0012  GETGBL	R3	G23
      0x88100107,  //  0013  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0014  CALL	R3	1
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x540E0008,  //  0016  LDINT	R3	9
      0x70020000,  //  0017  JMP		#0019
      0x540E0007,  //  0018  LDINT	R3	8
      0x90020C03,  //  0019  SETMBR	R0	K6	R3
      0x7002006C,  //  001A  JMP		#0088
      0x880C0106,  //  001B  GETMBR	R3	R0	K6
      0x280C070B,  //  001C  GE	R3	R3	K11
      0x780E0015,  //  001D  JMPF	R3	#0034
      0x880C0106,  //  001E  GETMBR	R3	R0	K6
      0x180C070C,  //  001F  LE	R3	R3	K12
      0x780E0012,  //  0020  JMPF	R3	#0034
      0x600C0009,  //  0021  GETGBL	R3	G9
      0x88100107,  //  0022  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0023  CALL	R3	1
      0x5412007E,  //  0024  LDINT	R4	127
      0x18100604,  //  0025  LE	R4	R3	R4
      0x78120004,  //  0026  JMPF	R4	#002C
      0x5411FF7F,  //  0027  LDINT	R4	-128
      0x28100604,  //  0028  GE	R4	R3	R4
      0x78120001,  //  0029  JMPF	R4	#002C
      0x90020D0A,  //  002A  SETMBR	R0	K6	K10
      0x70020006,  //  002B  JMP		#0033
      0x54127FFE,  //  002C  LDINT	R4	32767
      0x18100604,  //  002D  LE	R4	R3	R4
      0x78120003,  //  002E  JMPF	R4	#0033
      0x54117FFF,  //  002F  LDINT	R4	-32768
      0x28100604,  //  0030  GE	R4	R3	R4
      0x78120000,  //  0031  JMPF	R4	#0033
      0x90020D0B,  //  0032  SETMBR	R0	K6	K11
      0x70020053,  //  0033  JMP		#0088
      0x880C0106,  //  0034  GETMBR	R3	R0	K6
      0x54120004,  //  0035  LDINT	R4	5
      0x280C0604,  //  0036  GE	R3	R3	R4
      0x780E0016,  //  0037  JMPF	R3	#004F
      0x880C0106,  //  0038  GETMBR	R3	R0	K6
      0x54120005,  //  0039  LDINT	R4	6
      0x180C0604,  //  003A  LE	R3	R3	R4
      0x780E0012,  //  003B  JMPF	R3	#004F
      0x600C0009,  //  003C  GETGBL	R3	G9
      0x88100107,  //  003D  GETMBR	R4	R0	K7
      0x7C0C0200,  //  003E  CALL	R3	1
      0x541200FE,  //  003F  LDINT	R4	255
      0x18100604,  //  0040  LE	R4	R3	R4
      0x78120004,  //  0041  JMPF	R4	#0047
      0x2810070A,  //  0042  GE	R4	R3	K10
      0x78120002,  //  0043  JMPF	R4	#0047
      0x54120003,  //  0044  LDINT	R4	4
      0x90020C04,  //  0045  SETMBR	R0	K6	R4
      0x70020006,  //  0046  JMP		#004E
      0x5412FFFE,  //  0047  LDINT	R4	65535
      0x18100604,  //  0048  LE	R4	R3	R4
      0x78120003,  //  0049  JMPF	R4	#004E
      0x2810070A,  //  004A  GE	R4	R3	K10
      0x78120001,  //  004B  JMPF	R4	#004E
      0x54120004,  //  004C  LDINT	R4	5
      0x90020C04,  //  004D  SETMBR	R0	K6	R4
      0x70020038,  //  004E  JMP		#0088
      0x880C0106,  //  004F  GETMBR	R3	R0	K6
      0x5412000F,  //  0050  LDINT	R4	16
      0x280C0604,  //  0051  GE	R3	R3	R4
      0x780E0018,  //  0052  JMPF	R3	#006C
      0x880C0106,  //  0053  GETMBR	R3	R0	K6
      0x54120012,  //  0054  LDINT	R4	19
      0x180C0604,  //  0055  LE	R3	R3	R4
      0x780E0014,  //  0056  JMPF	R3	#006C
      0x600C000C,  //  0057  GETGBL	R3	G12
      0x88100107,  //  0058  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0059  CALL	R3	1
      0x541200FE,  //  005A  LDINT	R4	255
      0x180C0604,  //  005B  LE	R3	R3	R4
      0x780E0002,  //  005C  JMPF	R3	#0060
      0x540E000F,  //  005D  LDINT	R3	16
      0x90020C03,  //  005E  SETMBR	R0	K6	R3
      0x7002000A,  //  005F  JMP		#006B
      0x600C000C,  //  0060  GETGBL	R3	G12
      0x88100107,  //  0061  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0062  CALL	R3	1
      0x5412FFFE,  //  0063  LDINT	R4	65535
      0x180C0604,  //  0064  LE	R3	R3	R4
      0x780E0002,  //  0065  JMPF	R3	#0069
      0x540E0010,  //  0066  LDINT	R3	17
      0x90020C03,  //  0067  SETMBR	R0	K6	R3
      0x70020001,  //  0068  JMP		#006B
      0x540E0011,  //  0069  LDINT	R3	18
      0x90020C03,  //  006A  SETMBR	R0	K6	R3
      0x7002001B,  //  006B  JMP		#0088
      0x880C0106,  //  006C  GETMBR	R3	R0	K6
      0x5412000B,  //  006D  LDINT	R4	12
      0x280C0604,  //  006E  GE	R3	R3	R4
      0x780E0017,  //  006F  JMPF	R3	#0088
      0x880C0106,  //  0070  GETMBR	R3	R0	K6
      0x5412000E,  //  0071  LDINT	R4	15
      0x180C0604,  //  0072  LE	R3	R3	R4
      0x780E0013,  //  0073  JMPF	R3	#0088
      0x600C000C,  //  0074  GETGBL	R3	G12
      0x88100107,  //  0075  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0076  CALL	R3	1
      0x541200FE,  //  0077  LDINT	R4	255
      0x180C0604,  //  0078  LE	R3	R3	R4
      0x780E0002,  //  0079  JMPF	R3	#007D
      0x540E000B,  //  007A  LDINT	R3	12
      0x90020C03,  //  007B  SETMBR	R0	K6	R3
      0x7002000A,  //  007C  JMP		#0088
      0x600C000C,  //  007D  GETGBL	R3	G12
      0x88100107,  //  007E  GETMBR	R4	R0	K7
      0x7C0C0200,  //  007F  CALL	R3	1
      0x5412FFFE,  //  0080  LDINT	R4	65535
      0x180C0604,  //  0081  LE	R3	R3	R4
      0x780E0002,  //  0082  JMPF	R3	#0086
      0x540E000C,  //  0083  LDINT	R3	13
      0x90020C03,  //  0084  SETMBR	R0	K6	R3
      0x70020001,  //  0085  JMP		#0088
      0x540E000D,  //  0086  LDINT	R3	14
      0x90020C03,  //  0087  SETMBR	R0	K6	R3
      0x8C0C010D,  //  0088  GETMET	R3	R0	K13
      0x7C0C0200,  //  0089  CALL	R3	1
      0x00080403,  //  008A  ADD	R2	R2	R3
      0x880C0106,  //  008B  GETMBR	R3	R0	K6
      0x1C0C070A,  //  008C  EQ	R3	R3	K10
      0x740E0003,  //  008D  JMPT	R3	#0092
      0x880C0106,  //  008E  GETMBR	R3	R0	K6
      0x54120003,  //  008F  LDINT	R4	4
      0x1C0C0604,  //  0090  EQ	R3	R3	R4
      0x780E0001,  //  0091  JMPF	R3	#0094
      0x0008050B,  //  0092  ADD	R2	R2	K11
      0x70020064,  //  0093  JMP		#00F9
      0x880C0106,  //  0094  GETMBR	R3	R0	K6
      0x1C0C070B,  //  0095  EQ	R3	R3	K11
      0x740E0003,  //  0096  JMPT	R3	#009B
      0x880C0106,  //  0097  GETMBR	R3	R0	K6
      0x54120004,  //  0098  LDINT	R4	5
      0x1C0C0604,  //  0099  EQ	R3	R3	R4
      0x780E0001,  //  009A  JMPF	R3	#009D
      0x0008050C,  //  009B  ADD	R2	R2	K12
      0x7002005B,  //  009C  JMP		#00F9
      0x880C0106,  //  009D  GETMBR	R3	R0	K6
      0x1C0C070C,  //  009E  EQ	R3	R3	K12
      0x740E0003,  //  009F  JMPT	R3	#00A4
      0x880C0106,  //  00A0  GETMBR	R3	R0	K6
      0x54120005,  //  00A1  LDINT	R4	6
      0x1C0C0604,  //  00A2  EQ	R3	R3	R4
      0x780E0002,  //  00A3  JMPF	R3	#00A7
      0x540E0003,  //  00A4  LDINT	R3	4
      0x00080403,  //  00A5  ADD	R2	R2	R3
      0x70020051,  //  00A6  JMP		#00F9
      0x880C0106,  //  00A7  GETMBR	R3	R0	K6
      0x1C0C070E,  //  00A8  EQ	R3	R3	K14
      0x740E0003,  //  00A9  JMPT	R3	#00AE
      0x880C0106,  //  00AA  GETMBR	R3	R0	K6
      0x54120006,  //  00AB  LDINT	R4	7
      0x1C0C0604,  //  00AC  EQ	R3	R3	R4
      0x780E0002,  //  00AD  JMPF	R3	#00B1
      0x540E0007,  //  00AE  LDINT	R3	8
      0x00080403,  //  00AF  ADD	R2	R2	R3
      0x70020047,  //  00B0  JMP		#00F9
      0x880C0106,  //  00B1  GETMBR	R3	R0	K6
      0x54120007,  //  00B2  LDINT	R4	8
      0x1C0C0604,  //  00B3  EQ	R3	R3	R4
      0x740E0003,  //  00B4  JMPT	R3	#00B9
      0x880C0106,  //  00B5  GETMBR	R3	R0	K6
      0x54120008,  //  00B6  LDINT	R4	9
      0x1C0C0604,  //  00B7  EQ	R3	R3	R4
      0x780E0000,  //  00B8  JMPF	R3	#00BA
      0x7002003E,  //  00B9  JMP		#00F9
      0x880C0106,  //  00BA  GETMBR	R3	R0	K6
      0x54120009,  //  00BB  LDINT	R4	10
      0x1C0C0604,  //  00BC  EQ	R3	R3	R4
      0x780E0002,  //  00BD  JMPF	R3	#00C1
      0x540E0003,  //  00BE  LDINT	R3	4
      0x00080403,  //  00BF  ADD	R2	R2	R3
      0x70020037,  //  00C0  JMP		#00F9
      0x880C0106,  //  00C1  GETMBR	R3	R0	K6
      0x5412000A,  //  00C2  LDINT	R4	11
      0x1C0C0604,  //  00C3  EQ	R3	R3	R4
      0x780E0001,  //  00C4  JMPF	R3	#00C7
      0xB0061F10,  //  00C5  RAISE	1	K15	K16
      0x70020031,  //  00C6  JMP		#00F9
      0x880C0106,  //  00C7  GETMBR	R3	R0	K6
      0x5412000B,  //  00C8  LDINT	R4	12
      0x1C0C0604,  //  00C9  EQ	R3	R3	R4
      0x780E0005,  //  00CA  JMPF	R3	#00D1
      0x600C000C,  //  00CB  GETGBL	R3	G12
      0x88100107,  //  00CC  GETMBR	R4	R0	K7
      0x7C0C0200,  //  00CD  CALL	R3	1
      0x000E1603,  //  00CE  ADD	R3	K11	R3
      0x00080403,  //  00CF  ADD	R2	R2	R3
      0x70020027,  //  00D0  JMP		#00F9
      0x880C0106,  //  00D1  GETMBR	R3	R0	K6
      0x5412000C,  //  00D2  LDINT	R4	13
      0x1C0C0604,  //  00D3  EQ	R3	R3	R4
      0x780E0005,  //  00D4  JMPF	R3	#00DB
      0x600C000C,  //  00D5  GETGBL	R3	G12
      0x88100107,  //  00D6  GETMBR	R4	R0	K7
      0x7C0C0200,  //  00D7  CALL	R3	1
      0x000E1803,  //  00D8  ADD	R3	K12	R3
      0x00080403,  //  00D9  ADD	R2	R2	R3
      0x7002001D,  //  00DA  JMP		#00F9
      0x880C0106,  //  00DB  GETMBR	R3	R0	K6
      0x5412000F,  //  00DC  LDINT	R4	16
      0x1C0C0604,  //  00DD  EQ	R3	R3	R4
      0x780E0005,  //  00DE  JMPF	R3	#00E5
      0x600C000C,  //  00DF  GETGBL	R3	G12
      0x88100107,  //  00E0  GETMBR	R4	R0	K7
      0x7C0C0200,  //  00E1  CALL	R3	1
      0x000E1603,  //  00E2  ADD	R3	K11	R3
      0x00080403,  //  00E3  ADD	R2	R2	R3
      0x70020013,  //  00E4  JMP		#00F9
      0x880C0106,  //  00E5  GETMBR	R3	R0	K6
      0x54120010,  //  00E6  LDINT	R4	17
      0x1C0C0604,  //  00E7  EQ	R3	R3	R4
      0x780E0005,  //  00E8  JMPF	R3	#00EF
      0x600C000C,  //  00E9  GETGBL	R3	G12
      0x88100107,  //  00EA  GETMBR	R4	R0	K7
      0x7C0C0200,  //  00EB  CALL	R3	1
      0x000E1803,  //  00EC  ADD	R3	K12	R3
      0x00080403,  //  00ED  ADD	R2	R2	R3
      0x70020009,  //  00EE  JMP		#00F9
      0x880C0106,  //  00EF  GETMBR	R3	R0	K6
      0x54120013,  //  00F0  LDINT	R4	20
      0x1C0C0604,  //  00F1  EQ	R3	R3	R4
      0x780E0000,  //  00F2  JMPF	R3	#00F4
      0x70020004,  //  00F3  JMP		#00F9
      0x600C0008,  //  00F4  GETGBL	R3	G8
      0x88100106,  //  00F5  GETMBR	R4	R0	K6
      0x7C0C0200,  //  00F6  CALL	R3	1
      0x000E2203,  //  00F7  ADD	R3	K17	R3
      0xB0061E03,  //  00F8  RAISE	1	K15	R3
      0x80040400,  //  00F9  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set
********************************************************************/
be_local_closure(class_Matter_TLV_item_set,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(set),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C0C0108,  //  0000  GETMET	R3	R0	K8
      0x7C0C0200,  //  0001  CALL	R3	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x200C0403,  //  0003  NE	R3	R2	R3
      0x740E0002,  //  0004  JMPT	R3	#0008
      0x540E0013,  //  0005  LDINT	R3	20
      0x1C0C0203,  //  0006  EQ	R3	R1	R3
      0x780E0002,  //  0007  JMPF	R3	#000B
      0x90020C01,  //  0008  SETMBR	R0	K6	R1
      0x90020E02,  //  0009  SETMBR	R0	K7	R2
      0x80040000,  //  000A  RET	1	R0
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
be_local_closure(class_Matter_TLV_item_to_TLV,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(to_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80040000,  //  0000  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Matter_TLV_item_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[163]) {  /* code */
      0x58080012,  //  0000  LDCONST	R2	K18
      0xA8020098,  //  0001  EXBLK	0	#009B
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x200C0203,  //  0003  NE	R3	R1	R3
      0x780E0038,  //  0004  JMPF	R3	#003E
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x5411FFFE,  //  0006  LDINT	R4	-1
      0x1C0C0604,  //  0007  EQ	R3	R3	R4
      0x780E000A,  //  0008  JMPF	R3	#0014
      0x00080513,  //  0009  ADD	R2	R2	K19
      0x880C0104,  //  000A  GETMBR	R3	R0	K4
      0x4C100000,  //  000B  LDNIL	R4
      0x200C0604,  //  000C  NE	R3	R3	R4
      0x780E0004,  //  000D  JMPF	R3	#0013
      0x600C0018,  //  000E  GETGBL	R3	G24
      0x58100014,  //  000F  LDCONST	R4	K20
      0x88140104,  //  0010  GETMBR	R5	R0	K4
      0x7C0C0400,  //  0011  CALL	R3	2
      0x00080403,  //  0012  ADD	R2	R2	R3
      0x70020023,  //  0013  JMP		#0038
      0x880C0102,  //  0014  GETMBR	R3	R0	K2
      0x4C100000,  //  0015  LDNIL	R4
      0x200C0604,  //  0016  NE	R3	R3	R4
      0x780E0004,  //  0017  JMPF	R3	#001D
      0x600C0018,  //  0018  GETGBL	R3	G24
      0x58100015,  //  0019  LDCONST	R4	K21
      0x88140102,  //  001A  GETMBR	R5	R0	K2
      0x7C0C0400,  //  001B  CALL	R3	2
      0x00080403,  //  001C  ADD	R2	R2	R3
      0x880C0103,  //  001D  GETMBR	R3	R0	K3
      0x4C100000,  //  001E  LDNIL	R4
      0x200C0604,  //  001F  NE	R3	R3	R4
      0x780E0004,  //  0020  JMPF	R3	#0026
      0x600C0018,  //  0021  GETGBL	R3	G24
      0x58100016,  //  0022  LDCONST	R4	K22
      0x88140103,  //  0023  GETMBR	R5	R0	K3
      0x7C0C0400,  //  0024  CALL	R3	2
      0x00080403,  //  0025  ADD	R2	R2	R3
      0x880C0104,  //  0026  GETMBR	R3	R0	K4
      0x4C100000,  //  0027  LDNIL	R4
      0x200C0604,  //  0028  NE	R3	R3	R4
      0x780E0004,  //  0029  JMPF	R3	#002F
      0x600C0018,  //  002A  GETGBL	R3	G24
      0x58100014,  //  002B  LDCONST	R4	K20
      0x88140104,  //  002C  GETMBR	R5	R0	K4
      0x7C0C0400,  //  002D  CALL	R3	2
      0x00080403,  //  002E  ADD	R2	R2	R3
      0x880C0105,  //  002F  GETMBR	R3	R0	K5
      0x4C100000,  //  0030  LDNIL	R4
      0x200C0604,  //  0031  NE	R3	R3	R4
      0x780E0004,  //  0032  JMPF	R3	#0038
      0x600C0018,  //  0033  GETGBL	R3	G24
      0x58100017,  //  0034  LDCONST	R4	K23
      0x88140105,  //  0035  GETMBR	R5	R0	K5
      0x7C0C0400,  //  0036  CALL	R3	2
      0x00080403,  //  0037  ADD	R2	R2	R3
      0x600C000C,  //  0038  GETGBL	R3	G12
      0x5C100400,  //  0039  MOVE	R4	R2
      0x7C0C0200,  //  003A  CALL	R3	1
      0x240C070A,  //  003B  GT	R3	R3	K10
      0x780E0000,  //  003C  JMPF	R3	#003E
      0x00080518,  //  003D  ADD	R2	R2	K24
      0x600C0004,  //  003E  GETGBL	R3	G4
      0x88100107,  //  003F  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0040  CALL	R3	1
      0x1C0C0719,  //  0041  EQ	R3	R3	K25
      0x780E000E,  //  0042  JMPF	R3	#0052
      0x600C0018,  //  0043  GETGBL	R3	G24
      0x5810001A,  //  0044  LDCONST	R4	K26
      0x88140107,  //  0045  GETMBR	R5	R0	K7
      0x7C0C0400,  //  0046  CALL	R3	2
      0x00080403,  //  0047  ADD	R2	R2	R3
      0x880C0106,  //  0048  GETMBR	R3	R0	K6
      0x54120003,  //  0049  LDINT	R4	4
      0x280C0604,  //  004A  GE	R3	R3	R4
      0x780E0004,  //  004B  JMPF	R3	#0051
      0x880C0106,  //  004C  GETMBR	R3	R0	K6
      0x54120006,  //  004D  LDINT	R4	7
      0x180C0604,  //  004E  LE	R3	R3	R4
      0x780E0000,  //  004F  JMPF	R3	#0051
      0x0008051B,  //  0050  ADD	R2	R2	K27
      0x70020046,  //  0051  JMP		#0099
      0x600C0004,  //  0052  GETGBL	R3	G4
      0x88100107,  //  0053  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0054  CALL	R3	1
      0x1C0C071C,  //  0055  EQ	R3	R3	K28
      0x780E0006,  //  0056  JMPF	R3	#005E
      0x880C0107,  //  0057  GETMBR	R3	R0	K7
      0x780E0001,  //  0058  JMPF	R3	#005B
      0x580C001D,  //  0059  LDCONST	R3	K29
      0x70020000,  //  005A  JMP		#005C
      0x580C001E,  //  005B  LDCONST	R3	K30
      0x00080403,  //  005C  ADD	R2	R2	R3
      0x7002003A,  //  005D  JMP		#0099
      0x880C0107,  //  005E  GETMBR	R3	R0	K7
      0x4C100000,  //  005F  LDNIL	R4
      0x1C0C0604,  //  0060  EQ	R3	R3	R4
      0x780E0001,  //  0061  JMPF	R3	#0064
      0x0008051F,  //  0062  ADD	R2	R2	K31
      0x70020034,  //  0063  JMP		#0099
      0x600C0004,  //  0064  GETGBL	R3	G4
      0x88100107,  //  0065  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0066  CALL	R3	1
      0x1C0C0720,  //  0067  EQ	R3	R3	K32
      0x780E0005,  //  0068  JMPF	R3	#006F
      0x600C0018,  //  0069  GETGBL	R3	G24
      0x58100021,  //  006A  LDCONST	R4	K33
      0x88140107,  //  006B  GETMBR	R5	R0	K7
      0x7C0C0400,  //  006C  CALL	R3	2
      0x00080403,  //  006D  ADD	R2	R2	R3
      0x70020029,  //  006E  JMP		#0099
      0x600C0004,  //  006F  GETGBL	R3	G4
      0x88100107,  //  0070  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0071  CALL	R3	1
      0x1C0C0722,  //  0072  EQ	R3	R3	K34
      0x780E0005,  //  0073  JMPF	R3	#007A
      0x600C0018,  //  0074  GETGBL	R3	G24
      0x58100023,  //  0075  LDCONST	R4	K35
      0x88140107,  //  0076  GETMBR	R5	R0	K7
      0x7C0C0400,  //  0077  CALL	R3	2
      0x00080403,  //  0078  ADD	R2	R2	R3
      0x7002001E,  //  0079  JMP		#0099
      0x600C000F,  //  007A  GETGBL	R3	G15
      0x88100107,  //  007B  GETMBR	R4	R0	K7
      0xB8164800,  //  007C  GETNGBL	R5	K36
      0x7C0C0400,  //  007D  CALL	R3	2
      0x780E000D,  //  007E  JMPF	R3	#008D
      0x880C0107,  //  007F  GETMBR	R3	R0	K7
      0x8C0C0725,  //  0080  GETMET	R3	R3	K37
      0x7C0C0200,  //  0081  CALL	R3	1
      0x00080403,  //  0082  ADD	R2	R2	R3
      0x880C0106,  //  0083  GETMBR	R3	R0	K6
      0x54120003,  //  0084  LDINT	R4	4
      0x280C0604,  //  0085  GE	R3	R3	R4
      0x780E0004,  //  0086  JMPF	R3	#008C
      0x880C0106,  //  0087  GETMBR	R3	R0	K6
      0x54120006,  //  0088  LDINT	R4	7
      0x180C0604,  //  0089  LE	R3	R3	R4
      0x780E0000,  //  008A  JMPF	R3	#008C
      0x0008051B,  //  008B  ADD	R2	R2	K27
      0x7002000B,  //  008C  JMP		#0099
      0x600C0004,  //  008D  GETGBL	R3	G4
      0x88100107,  //  008E  GETMBR	R4	R0	K7
      0x7C0C0200,  //  008F  CALL	R3	1
      0x1C0C0726,  //  0090  EQ	R3	R3	K38
      0x780E0006,  //  0091  JMPF	R3	#0099
      0x600C0018,  //  0092  GETGBL	R3	G24
      0x58100027,  //  0093  LDCONST	R4	K39
      0x88140107,  //  0094  GETMBR	R5	R0	K7
      0x8C140B28,  //  0095  GETMET	R5	R5	K40
      0x7C140200,  //  0096  CALL	R5	1
      0x7C0C0400,  //  0097  CALL	R3	2
      0x00080403,  //  0098  ADD	R2	R2	R3
      0xA8040001,  //  0099  EXBLK	1	1
      0x70020006,  //  009A  JMP		#00A2
      0xAC0C0002,  //  009B  CATCH	R3	0	2
      0x70020003,  //  009C  JMP		#00A1
      0x00140729,  //  009D  ADD	R5	R3	K41
      0x00140A04,  //  009E  ADD	R5	R5	R4
      0x80040A00,  //  009F  RET	1	R5
      0x70020000,  //  00A0  JMP		#00A2
      0xB0080000,  //  00A1  RAISE	2	R0	R0
      0x80040400,  //  00A2  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_str_val
********************************************************************/
be_local_closure(class_Matter_TLV_item_to_str_val,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(to_str_val),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0x60040004,  //  0000  GETGBL	R1	G4
      0x88080107,  //  0001  GETMBR	R2	R0	K7
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040319,  //  0003  EQ	R1	R1	K25
      0x78060012,  //  0004  JMPF	R1	#0018
      0x88040106,  //  0005  GETMBR	R1	R0	K6
      0x540A0003,  //  0006  LDINT	R2	4
      0x28040202,  //  0007  GE	R1	R1	R2
      0x78060009,  //  0008  JMPF	R1	#0013
      0x88040106,  //  0009  GETMBR	R1	R0	K6
      0x540A0006,  //  000A  LDINT	R2	7
      0x18040202,  //  000B  LE	R1	R1	R2
      0x78060005,  //  000C  JMPF	R1	#0013
      0x60040008,  //  000D  GETGBL	R1	G8
      0x88080107,  //  000E  GETMBR	R2	R0	K7
      0x7C040200,  //  000F  CALL	R1	1
      0x0004031B,  //  0010  ADD	R1	R1	K27
      0x80040200,  //  0011  RET	1	R1
      0x70020003,  //  0012  JMP		#0017
      0x60040008,  //  0013  GETGBL	R1	G8
      0x88080107,  //  0014  GETMBR	R2	R0	K7
      0x7C040200,  //  0015  CALL	R1	1
      0x80040200,  //  0016  RET	1	R1
      0x70020044,  //  0017  JMP		#005D
      0x60040004,  //  0018  GETGBL	R1	G4
      0x88080107,  //  0019  GETMBR	R2	R0	K7
      0x7C040200,  //  001A  CALL	R1	1
      0x1C04031C,  //  001B  EQ	R1	R1	K28
      0x78060006,  //  001C  JMPF	R1	#0024
      0x88040107,  //  001D  GETMBR	R1	R0	K7
      0x78060001,  //  001E  JMPF	R1	#0021
      0x5804001D,  //  001F  LDCONST	R1	K29
      0x70020000,  //  0020  JMP		#0022
      0x5804001E,  //  0021  LDCONST	R1	K30
      0x80040200,  //  0022  RET	1	R1
      0x70020038,  //  0023  JMP		#005D
      0x88040107,  //  0024  GETMBR	R1	R0	K7
      0x4C080000,  //  0025  LDNIL	R2
      0x1C040202,  //  0026  EQ	R1	R1	R2
      0x78060001,  //  0027  JMPF	R1	#002A
      0x80063E00,  //  0028  RET	1	K31
      0x70020032,  //  0029  JMP		#005D
      0x60040004,  //  002A  GETGBL	R1	G4
      0x88080107,  //  002B  GETMBR	R2	R0	K7
      0x7C040200,  //  002C  CALL	R1	1
      0x1C040320,  //  002D  EQ	R1	R1	K32
      0x78060004,  //  002E  JMPF	R1	#0034
      0x60040008,  //  002F  GETGBL	R1	G8
      0x88080107,  //  0030  GETMBR	R2	R0	K7
      0x7C040200,  //  0031  CALL	R1	1
      0x80040200,  //  0032  RET	1	R1
      0x70020028,  //  0033  JMP		#005D
      0x60040004,  //  0034  GETGBL	R1	G4
      0x88080107,  //  0035  GETMBR	R2	R0	K7
      0x7C040200,  //  0036  CALL	R1	1
      0x1C040322,  //  0037  EQ	R1	R1	K34
      0x78060002,  //  0038  JMPF	R1	#003C
      0x88040107,  //  0039  GETMBR	R1	R0	K7
      0x80040200,  //  003A  RET	1	R1
      0x70020020,  //  003B  JMP		#005D
      0x6004000F,  //  003C  GETGBL	R1	G15
      0x88080107,  //  003D  GETMBR	R2	R0	K7
      0xB80E4800,  //  003E  GETNGBL	R3	K36
      0x7C040400,  //  003F  CALL	R1	2
      0x78060012,  //  0040  JMPF	R1	#0054
      0x88040106,  //  0041  GETMBR	R1	R0	K6
      0x540A0003,  //  0042  LDINT	R2	4
      0x28040202,  //  0043  GE	R1	R1	R2
      0x78060009,  //  0044  JMPF	R1	#004F
      0x88040106,  //  0045  GETMBR	R1	R0	K6
      0x540A0006,  //  0046  LDINT	R2	7
      0x18040202,  //  0047  LE	R1	R1	R2
      0x78060005,  //  0048  JMPF	R1	#004F
      0x88040107,  //  0049  GETMBR	R1	R0	K7
      0x8C040325,  //  004A  GETMET	R1	R1	K37
      0x7C040200,  //  004B  CALL	R1	1
      0x0004031B,  //  004C  ADD	R1	R1	K27
      0x80040200,  //  004D  RET	1	R1
      0x70020003,  //  004E  JMP		#0053
      0x88040107,  //  004F  GETMBR	R1	R0	K7
      0x8C040325,  //  0050  GETMET	R1	R1	K37
      0x7C040200,  //  0051  CALL	R1	1
      0x80040200,  //  0052  RET	1	R1
      0x70020008,  //  0053  JMP		#005D
      0x60040004,  //  0054  GETGBL	R1	G4
      0x88080107,  //  0055  GETMBR	R2	R0	K7
      0x7C040200,  //  0056  CALL	R1	1
      0x1C040326,  //  0057  EQ	R1	R1	K38
      0x78060003,  //  0058  JMPF	R1	#005D
      0x8C040125,  //  0059  GETMET	R1	R0	K37
      0x500C0200,  //  005A  LDBOOL	R3	1	0
      0x7C040400,  //  005B  CALL	R1	2
      0x80040200,  //  005C  RET	1	R1
      0x80000000,  //  005D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fulltag
********************************************************************/
be_local_closure(class_Matter_TLV_item_set_fulltag,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(set_fulltag),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60100009,  //  0000  GETGBL	R4	G9
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C100200,  //  0002  CALL	R4	1
      0x90020404,  //  0003  SETMBR	R0	K2	R4
      0x60100009,  //  0004  GETGBL	R4	G9
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C100200,  //  0006  CALL	R4	1
      0x90020604,  //  0007  SETMBR	R0	K3	R4
      0x60100009,  //  0008  GETGBL	R4	G9
      0x5C140600,  //  0009  MOVE	R5	R3
      0x7C100200,  //  000A  CALL	R4	1
      0x90020804,  //  000B  SETMBR	R0	K4	R4
      0x4C100000,  //  000C  LDNIL	R4
      0x90020A04,  //  000D  SETMBR	R0	K5	R4
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Matter_TLV_item_parse,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[100]) {  /* code */
      0x880C0106,  //  0000  GETMBR	R3	R0	K6
      0x88100109,  //  0001  GETMBR	R4	R0	K9
      0x8814092A,  //  0002  GETMBR	R5	R4	K42
      0x94140A03,  //  0003  GETIDX	R5	R5	R3
      0x541A0007,  //  0004  LDINT	R6	8
      0x1C180A06,  //  0005  EQ	R6	R5	R6
      0x781A0008,  //  0006  JMPF	R6	#0010
      0xB81A4800,  //  0007  GETNGBL	R6	K36
      0x8C180D2B,  //  0008  GETMET	R6	R6	K43
      0x5C200200,  //  0009  MOVE	R8	R1
      0x5C240400,  //  000A  MOVE	R9	R2
      0x7C180600,  //  000B  CALL	R6	3
      0x90020E06,  //  000C  SETMBR	R0	K7	R6
      0x541A0007,  //  000D  LDINT	R6	8
      0x00080406,  //  000E  ADD	R2	R2	R6
      0x70020051,  //  000F  JMP		#0062
      0x541A0007,  //  0010  LDINT	R6	8
      0x1C180606,  //  0011  EQ	R6	R3	R6
      0x741A0002,  //  0012  JMPT	R6	#0016
      0x541A0008,  //  0013  LDINT	R6	9
      0x1C180606,  //  0014  EQ	R6	R3	R6
      0x781A0003,  //  0015  JMPF	R6	#001A
      0x541A0008,  //  0016  LDINT	R6	9
      0x1C180606,  //  0017  EQ	R6	R3	R6
      0x90020E06,  //  0018  SETMBR	R0	K7	R6
      0x70020047,  //  0019  JMP		#0062
      0x541A0006,  //  001A  LDINT	R6	7
      0x14180606,  //  001B  LT	R6	R3	R6
      0x781A000D,  //  001C  JMPF	R6	#002B
      0x1818070E,  //  001D  LE	R6	R3	K14
      0x781A0004,  //  001E  JMPF	R6	#0024
      0x8C18032C,  //  001F  GETMET	R6	R1	K44
      0x5C200400,  //  0020  MOVE	R8	R2
      0x5C240A00,  //  0021  MOVE	R9	R5
      0x7C180600,  //  0022  CALL	R6	3
      0x70020003,  //  0023  JMP		#0028
      0x8C18032D,  //  0024  GETMET	R6	R1	K45
      0x5C200400,  //  0025  MOVE	R8	R2
      0x5C240A00,  //  0026  MOVE	R9	R5
      0x7C180600,  //  0027  CALL	R6	3
      0x90020E06,  //  0028  SETMBR	R0	K7	R6
      0x00080405,  //  0029  ADD	R2	R2	R5
      0x70020036,  //  002A  JMP		#0062
      0x541A0009,  //  002B  LDINT	R6	10
      0x1C180606,  //  002C  EQ	R6	R3	R6
      0x781A0006,  //  002D  JMPF	R6	#0035
      0x8C18032E,  //  002E  GETMET	R6	R1	K46
      0x5C200400,  //  002F  MOVE	R8	R2
      0x7C180400,  //  0030  CALL	R6	2
      0x90020E06,  //  0031  SETMBR	R0	K7	R6
      0x541A0003,  //  0032  LDINT	R6	4
      0x00080406,  //  0033  ADD	R2	R2	R6
      0x7002002C,  //  0034  JMP		#0062
      0x5419FFF7,  //  0035  LDINT	R6	-8
      0x28180A06,  //  0036  GE	R6	R5	R6
      0x781A0016,  //  0037  JMPF	R6	#004F
      0x5419FFFE,  //  0038  LDINT	R6	-1
      0x18180A06,  //  0039  LE	R6	R5	R6
      0x781A0013,  //  003A  JMPF	R6	#004F
      0x8C18032D,  //  003B  GETMET	R6	R1	K45
      0x5C200400,  //  003C  MOVE	R8	R2
      0x44240A00,  //  003D  NEG	R9	R5
      0x7C180600,  //  003E  CALL	R6	3
      0x441C0A00,  //  003F  NEG	R7	R5
      0x00080407,  //  0040  ADD	R2	R2	R7
      0x001C0406,  //  0041  ADD	R7	R2	R6
      0x041C0F0B,  //  0042  SUB	R7	R7	K11
      0x401C0407,  //  0043  CONNECT	R7	R2	R7
      0x941C0207,  //  0044  GETIDX	R7	R1	R7
      0x90020E07,  //  0045  SETMBR	R0	K7	R7
      0x00080406,  //  0046  ADD	R2	R2	R6
      0x541E000E,  //  0047  LDINT	R7	15
      0x181C0607,  //  0048  LE	R7	R3	R7
      0x781E0003,  //  0049  JMPF	R7	#004E
      0x881C0107,  //  004A  GETMBR	R7	R0	K7
      0x8C1C0F2F,  //  004B  GETMET	R7	R7	K47
      0x7C1C0200,  //  004C  CALL	R7	1
      0x90020E07,  //  004D  SETMBR	R0	K7	R7
      0x70020012,  //  004E  JMP		#0062
      0x541A0013,  //  004F  LDINT	R6	20
      0x1C180606,  //  0050  EQ	R6	R3	R6
      0x781A0000,  //  0051  JMPF	R6	#0053
      0x7002000E,  //  0052  JMP		#0062
      0x541A0017,  //  0053  LDINT	R6	24
      0x1C180606,  //  0054  EQ	R6	R3	R6
      0x781A0004,  //  0055  JMPF	R6	#005B
      0xB81A6000,  //  0056  GETNGBL	R6	K48
      0x581C0031,  //  0057  LDCONST	R7	K49
      0x5820000E,  //  0058  LDCONST	R8	K14
      0x7C180400,  //  0059  CALL	R6	2
      0x70020006,  //  005A  JMP		#0062
      0xB81A6000,  //  005B  GETNGBL	R6	K48
      0x601C0008,  //  005C  GETGBL	R7	G8
      0x5C200600,  //  005D  MOVE	R8	R3
      0x7C1C0200,  //  005E  CALL	R7	1
      0x001E6407,  //  005F  ADD	R7	K50	R7
      0x5820000E,  //  0060  LDCONST	R8	K14
      0x7C180400,  //  0061  CALL	R6	2
      0x90020202,  //  0062  SETMBR	R0	K1	R2
      0x80040400,  //  0063  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_commonprofile
********************************************************************/
be_local_closure(class_Matter_TLV_item_set_commonprofile,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(set_commonprofile),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040133,  //  0000  GETMET	R1	R0	K51
      0x4C0C0000,  //  0001  LDNIL	R3
      0x5411FFFE,  //  0002  LDINT	R4	-1
      0x4C140000,  //  0003  LDNIL	R5
      0x7C040800,  //  0004  CALL	R1	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _encode_tag
********************************************************************/
be_local_closure(class_Matter_TLV_item__encode_tag,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(_encode_tag),
    &be_const_str_solidified,
    ( &(const binstruction[133]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x88080104,  //  0004  GETMBR	R2	R0	K4
      0x70020000,  //  0005  JMP		#0007
      0x5808000A,  //  0006  LDCONST	R2	K10
      0x540EFFFF,  //  0007  LDINT	R3	65536
      0x280C0403,  //  0008  GE	R3	R2	R3
      0x740E0002,  //  0009  JMPT	R3	#000D
      0x140C050A,  //  000A  LT	R3	R2	K10
      0x740E0000,  //  000B  JMPT	R3	#000D
      0x500C0001,  //  000C  LDBOOL	R3	0	1
      0x500C0200,  //  000D  LDBOOL	R3	1	0
      0x5810000A,  //  000E  LDCONST	R4	K10
      0x88140102,  //  000F  GETMBR	R5	R0	K2
      0x4C180000,  //  0010  LDNIL	R6
      0x20140A06,  //  0011  NE	R5	R5	R6
      0x78160026,  //  0012  JMPF	R5	#003A
      0x780E0012,  //  0013  JMPF	R3	#0027
      0x8C140334,  //  0014  GETMET	R5	R1	K52
      0x541E00DF,  //  0015  LDINT	R7	224
      0x88200106,  //  0016  GETMBR	R8	R0	K6
      0x001C0E08,  //  0017  ADD	R7	R7	R8
      0x5820000B,  //  0018  LDCONST	R8	K11
      0x7C140600,  //  0019  CALL	R5	3
      0x8C140334,  //  001A  GETMET	R5	R1	K52
      0x881C0102,  //  001B  GETMBR	R7	R0	K2
      0x5820000C,  //  001C  LDCONST	R8	K12
      0x7C140600,  //  001D  CALL	R5	3
      0x8C140334,  //  001E  GETMET	R5	R1	K52
      0x881C0103,  //  001F  GETMBR	R7	R0	K3
      0x5820000C,  //  0020  LDCONST	R8	K12
      0x7C140600,  //  0021  CALL	R5	3
      0x8C140334,  //  0022  GETMET	R5	R1	K52
      0x881C0104,  //  0023  GETMBR	R7	R0	K4
      0x54220003,  //  0024  LDINT	R8	4
      0x7C140600,  //  0025  CALL	R5	3
      0x70020011,  //  0026  JMP		#0039
      0x8C140334,  //  0027  GETMET	R5	R1	K52
      0x541E00BF,  //  0028  LDINT	R7	192
      0x88200106,  //  0029  GETMBR	R8	R0	K6
      0x001C0E08,  //  002A  ADD	R7	R7	R8
      0x5820000B,  //  002B  LDCONST	R8	K11
      0x7C140600,  //  002C  CALL	R5	3
      0x8C140334,  //  002D  GETMET	R5	R1	K52
      0x881C0102,  //  002E  GETMBR	R7	R0	K2
      0x5820000C,  //  002F  LDCONST	R8	K12
      0x7C140600,  //  0030  CALL	R5	3
      0x8C140334,  //  0031  GETMET	R5	R1	K52
      0x881C0103,  //  0032  GETMBR	R7	R0	K3
      0x5820000C,  //  0033  LDCONST	R8	K12
      0x7C140600,  //  0034  CALL	R5	3
      0x8C140334,  //  0035  GETMET	R5	R1	K52
      0x881C0104,  //  0036  GETMBR	R7	R0	K4
      0x5820000C,  //  0037  LDCONST	R8	K12
      0x7C140600,  //  0038  CALL	R5	3
      0x70020049,  //  0039  JMP		#0084
      0x88140103,  //  003A  GETMBR	R5	R0	K3
      0x5419FFFE,  //  003B  LDINT	R6	-1
      0x1C140A06,  //  003C  EQ	R5	R5	R6
      0x78160016,  //  003D  JMPF	R5	#0055
      0x780E000A,  //  003E  JMPF	R3	#004A
      0x8C140334,  //  003F  GETMET	R5	R1	K52
      0x541E005F,  //  0040  LDINT	R7	96
      0x88200106,  //  0041  GETMBR	R8	R0	K6
      0x001C0E08,  //  0042  ADD	R7	R7	R8
      0x5820000B,  //  0043  LDCONST	R8	K11
      0x7C140600,  //  0044  CALL	R5	3
      0x8C140334,  //  0045  GETMET	R5	R1	K52
      0x881C0104,  //  0046  GETMBR	R7	R0	K4
      0x54220003,  //  0047  LDINT	R8	4
      0x7C140600,  //  0048  CALL	R5	3
      0x70020009,  //  0049  JMP		#0054
      0x8C140334,  //  004A  GETMET	R5	R1	K52
      0x541E003F,  //  004B  LDINT	R7	64
      0x88200106,  //  004C  GETMBR	R8	R0	K6
      0x001C0E08,  //  004D  ADD	R7	R7	R8
      0x5820000B,  //  004E  LDCONST	R8	K11
      0x7C140600,  //  004F  CALL	R5	3
      0x8C140334,  //  0050  GETMET	R5	R1	K52
      0x881C0104,  //  0051  GETMBR	R7	R0	K4
      0x5820000C,  //  0052  LDCONST	R8	K12
      0x7C140600,  //  0053  CALL	R5	3
      0x7002002E,  //  0054  JMP		#0084
      0x88140103,  //  0055  GETMBR	R5	R0	K3
      0x4C180000,  //  0056  LDNIL	R6
      0x20140A06,  //  0057  NE	R5	R5	R6
      0x78160016,  //  0058  JMPF	R5	#0070
      0x780E000A,  //  0059  JMPF	R3	#0065
      0x8C140334,  //  005A  GETMET	R5	R1	K52
      0x541E009F,  //  005B  LDINT	R7	160
      0x88200106,  //  005C  GETMBR	R8	R0	K6
      0x001C0E08,  //  005D  ADD	R7	R7	R8
      0x5820000B,  //  005E  LDCONST	R8	K11
      0x7C140600,  //  005F  CALL	R5	3
      0x8C140334,  //  0060  GETMET	R5	R1	K52
      0x881C0104,  //  0061  GETMBR	R7	R0	K4
      0x54220003,  //  0062  LDINT	R8	4
      0x7C140600,  //  0063  CALL	R5	3
      0x70020009,  //  0064  JMP		#006F
      0x8C140334,  //  0065  GETMET	R5	R1	K52
      0x541E007F,  //  0066  LDINT	R7	128
      0x88200106,  //  0067  GETMBR	R8	R0	K6
      0x001C0E08,  //  0068  ADD	R7	R7	R8
      0x5820000B,  //  0069  LDCONST	R8	K11
      0x7C140600,  //  006A  CALL	R5	3
      0x8C140334,  //  006B  GETMET	R5	R1	K52
      0x881C0104,  //  006C  GETMBR	R7	R0	K4
      0x5820000C,  //  006D  LDCONST	R8	K12
      0x7C140600,  //  006E  CALL	R5	3
      0x70020013,  //  006F  JMP		#0084
      0x88140105,  //  0070  GETMBR	R5	R0	K5
      0x4C180000,  //  0071  LDNIL	R6
      0x20140A06,  //  0072  NE	R5	R5	R6
      0x7816000A,  //  0073  JMPF	R5	#007F
      0x8C140334,  //  0074  GETMET	R5	R1	K52
      0x541E001F,  //  0075  LDINT	R7	32
      0x88200106,  //  0076  GETMBR	R8	R0	K6
      0x001C0E08,  //  0077  ADD	R7	R7	R8
      0x5820000B,  //  0078  LDCONST	R8	K11
      0x7C140600,  //  0079  CALL	R5	3
      0x8C140334,  //  007A  GETMET	R5	R1	K52
      0x881C0105,  //  007B  GETMBR	R7	R0	K5
      0x5820000B,  //  007C  LDCONST	R8	K11
      0x7C140600,  //  007D  CALL	R5	3
      0x70020004,  //  007E  JMP		#0084
      0x8C140334,  //  007F  GETMET	R5	R1	K52
      0x881C0106,  //  0080  GETMBR	R7	R0	K6
      0x001E1407,  //  0081  ADD	R7	K10	R7
      0x5820000B,  //  0082  LDCONST	R8	K11
      0x7C140600,  //  0083  CALL	R5	3
      0x80000000,  //  0084  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort
********************************************************************/
be_local_closure(class_Matter_TLV_item_sort,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(sort),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x58040035,  //  0000  LDCONST	R1	K53
      0x60080010,  //  0001  GETGBL	R2	G16
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x040C070B,  //  0005  SUB	R3	R3	K11
      0x400E1603,  //  0006  CONNECT	R3	K11	R3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020013,  //  0008  EXBLK	0	#001D
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x94100003,  //  000B  GETIDX	R4	R0	R3
      0x5C140600,  //  000C  MOVE	R5	R3
      0x24180B0A,  //  000D  GT	R6	R5	K10
      0x781A000B,  //  000E  JMPF	R6	#001B
      0x04180B0B,  //  000F  SUB	R6	R5	K11
      0x94180006,  //  0010  GETIDX	R6	R0	R6
      0x8C180D36,  //  0011  GETMET	R6	R6	K54
      0x5C200800,  //  0012  MOVE	R8	R4
      0x7C180400,  //  0013  CALL	R6	2
      0x24180D0A,  //  0014  GT	R6	R6	K10
      0x781A0004,  //  0015  JMPF	R6	#001B
      0x04180B0B,  //  0016  SUB	R6	R5	K11
      0x94180006,  //  0017  GETIDX	R6	R0	R6
      0x98000A06,  //  0018  SETIDX	R0	R5	R6
      0x04140B0B,  //  0019  SUB	R5	R5	K11
      0x7001FFF1,  //  001A  JMP		#000D
      0x98000A04,  //  001B  SETIDX	R0	R5	R4
      0x7001FFEB,  //  001C  JMP		#0009
      0x58080037,  //  001D  LDCONST	R2	K55
      0xAC080200,  //  001E  CATCH	R2	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0x80040000,  //  0020  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_TLV_item_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _cmp_gt
********************************************************************/
be_local_closure(class_Matter_TLV_item__cmp_gt,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(_cmp_gt),
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0012,  //  0003  JMPF	R2	#0017
      0x88080302,  //  0004  GETMBR	R2	R1	K2
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C080403,  //  0006  EQ	R2	R2	R3
      0x780A0000,  //  0007  JMPF	R2	#0009
      0x80061600,  //  0008  RET	1	K11
      0x88080102,  //  0009  GETMBR	R2	R0	K2
      0x880C0302,  //  000A  GETMBR	R3	R1	K2
      0x24080403,  //  000B  GT	R2	R2	R3
      0x780A0000,  //  000C  JMPF	R2	#000E
      0x80061600,  //  000D  RET	1	K11
      0x88080102,  //  000E  GETMBR	R2	R0	K2
      0x880C0302,  //  000F  GETMBR	R3	R1	K2
      0x1C080403,  //  0010  EQ	R2	R2	R3
      0x780A0004,  //  0011  JMPF	R2	#0017
      0x88080103,  //  0012  GETMBR	R2	R0	K3
      0x880C0303,  //  0013  GETMBR	R3	R1	K3
      0x24080403,  //  0014  GT	R2	R2	R3
      0x780A0000,  //  0015  JMPF	R2	#0017
      0x80061600,  //  0016  RET	1	K11
      0x88080103,  //  0017  GETMBR	R2	R0	K3
      0x540DFFFE,  //  0018  LDINT	R3	-1
      0x1C080403,  //  0019  EQ	R2	R2	R3
      0x780A0005,  //  001A  JMPF	R2	#0021
      0x88080303,  //  001B  GETMBR	R2	R1	K3
      0x4C0C0000,  //  001C  LDNIL	R3
      0x1C080403,  //  001D  EQ	R2	R2	R3
      0x780A0000,  //  001E  JMPF	R2	#0020
      0x80061600,  //  001F  RET	1	K11
      0x70020008,  //  0020  JMP		#002A
      0x88080103,  //  0021  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0022  LDNIL	R3
      0x1C080403,  //  0023  EQ	R2	R2	R3
      0x780A0004,  //  0024  JMPF	R2	#002A
      0x88080303,  //  0025  GETMBR	R2	R1	K3
      0x540DFFFE,  //  0026  LDINT	R3	-1
      0x1C080403,  //  0027  EQ	R2	R2	R3
      0x780A0000,  //  0028  JMPF	R2	#002A
      0x80061400,  //  0029  RET	1	K10
      0x88080104,  //  002A  GETMBR	R2	R0	K4
      0x4C0C0000,  //  002B  LDNIL	R3
      0x20080403,  //  002C  NE	R2	R2	R3
      0x780A000A,  //  002D  JMPF	R2	#0039
      0x88080304,  //  002E  GETMBR	R2	R1	K4
      0x4C0C0000,  //  002F  LDNIL	R3
      0x1C080403,  //  0030  EQ	R2	R2	R3
      0x780A0000,  //  0031  JMPF	R2	#0033
      0x80061600,  //  0032  RET	1	K11
      0x88080104,  //  0033  GETMBR	R2	R0	K4
      0x880C0304,  //  0034  GETMBR	R3	R1	K4
      0x24080403,  //  0035  GT	R2	R2	R3
      0x780A0000,  //  0036  JMPF	R2	#0038
      0x80061600,  //  0037  RET	1	K11
      0x80061400,  //  0038  RET	1	K10
      0x88080105,  //  0039  GETMBR	R2	R0	K5
      0x4C0C0000,  //  003A  LDNIL	R3
      0x20080403,  //  003B  NE	R2	R2	R3
      0x780A0009,  //  003C  JMPF	R2	#0047
      0x88080305,  //  003D  GETMBR	R2	R1	K5
      0x4C0C0000,  //  003E  LDNIL	R3
      0x1C080403,  //  003F  EQ	R2	R2	R3
      0x780A0000,  //  0040  JMPF	R2	#0042
      0x80061600,  //  0041  RET	1	K11
      0x88080105,  //  0042  GETMBR	R2	R0	K5
      0x880C0305,  //  0043  GETMBR	R3	R1	K5
      0x24080403,  //  0044  GT	R2	R2	R3
      0x780A0000,  //  0045  JMPF	R2	#0047
      0x80061600,  //  0046  RET	1	K11
      0x80061400,  //  0047  RET	1	K10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(class_Matter_TLV_item_tlv2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[392]) {  /* code */
      0x88080109,  //  0000  GETMBR	R2	R0	K9
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x780E0002,  //  0003  JMPF	R3	#0007
      0x600C0015,  //  0004  GETGBL	R3	G21
      0x7C0C0000,  //  0005  CALL	R3	0
      0x5C040600,  //  0006  MOVE	R1	R3
      0x880C0106,  //  0007  GETMBR	R3	R0	K6
      0x541200FE,  //  0008  LDINT	R4	255
      0x1C0C0604,  //  0009  EQ	R3	R3	R4
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x880C0107,  //  000B  GETMBR	R3	R0	K7
      0x400C0203,  //  000C  CONNECT	R3	R1	R3
      0x80040200,  //  000D  RET	1	R1
      0x880C0106,  //  000E  GETMBR	R3	R0	K6
      0x1C0C070E,  //  000F  EQ	R3	R3	K14
      0x740E0003,  //  0010  JMPT	R3	#0015
      0x880C0106,  //  0011  GETMBR	R3	R0	K6
      0x54120006,  //  0012  LDINT	R4	7
      0x1C0C0604,  //  0013  EQ	R3	R3	R4
      0x780E000B,  //  0014  JMPF	R3	#0021
      0x600C0004,  //  0015  GETGBL	R3	G4
      0x88100107,  //  0016  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0017  CALL	R3	1
      0x1C0C0719,  //  0018  EQ	R3	R3	K25
      0x780E0006,  //  0019  JMPF	R3	#0021
      0x880C0106,  //  001A  GETMBR	R3	R0	K6
      0x1C0C070E,  //  001B  EQ	R3	R3	K14
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x90020D0C,  //  001D  SETMBR	R0	K6	K12
      0x70020001,  //  001E  JMP		#0021
      0x540E0005,  //  001F  LDINT	R3	6
      0x90020C03,  //  0020  SETMBR	R0	K6	R3
      0x880C0106,  //  0021  GETMBR	R3	R0	K6
      0x54120007,  //  0022  LDINT	R4	8
      0x1C0C0604,  //  0023  EQ	R3	R3	R4
      0x740E0003,  //  0024  JMPT	R3	#0029
      0x880C0106,  //  0025  GETMBR	R3	R0	K6
      0x54120008,  //  0026  LDINT	R4	9
      0x1C0C0604,  //  0027  EQ	R3	R3	R4
      0x780E0008,  //  0028  JMPF	R3	#0032
      0x600C0017,  //  0029  GETGBL	R3	G23
      0x88100107,  //  002A  GETMBR	R4	R0	K7
      0x7C0C0200,  //  002B  CALL	R3	1
      0x780E0001,  //  002C  JMPF	R3	#002F
      0x540E0008,  //  002D  LDINT	R3	9
      0x70020000,  //  002E  JMP		#0030
      0x540E0007,  //  002F  LDINT	R3	8
      0x90020C03,  //  0030  SETMBR	R0	K6	R3
      0x7002006C,  //  0031  JMP		#009F
      0x880C0106,  //  0032  GETMBR	R3	R0	K6
      0x280C070B,  //  0033  GE	R3	R3	K11
      0x780E0015,  //  0034  JMPF	R3	#004B
      0x880C0106,  //  0035  GETMBR	R3	R0	K6
      0x180C070C,  //  0036  LE	R3	R3	K12
      0x780E0012,  //  0037  JMPF	R3	#004B
      0x600C0009,  //  0038  GETGBL	R3	G9
      0x88100107,  //  0039  GETMBR	R4	R0	K7
      0x7C0C0200,  //  003A  CALL	R3	1
      0x5412007E,  //  003B  LDINT	R4	127
      0x18100604,  //  003C  LE	R4	R3	R4
      0x78120004,  //  003D  JMPF	R4	#0043
      0x5411FF7F,  //  003E  LDINT	R4	-128
      0x28100604,  //  003F  GE	R4	R3	R4
      0x78120001,  //  0040  JMPF	R4	#0043
      0x90020D0A,  //  0041  SETMBR	R0	K6	K10
      0x70020006,  //  0042  JMP		#004A
      0x54127FFE,  //  0043  LDINT	R4	32767
      0x18100604,  //  0044  LE	R4	R3	R4
      0x78120003,  //  0045  JMPF	R4	#004A
      0x54117FFF,  //  0046  LDINT	R4	-32768
      0x28100604,  //  0047  GE	R4	R3	R4
      0x78120000,  //  0048  JMPF	R4	#004A
      0x90020D0B,  //  0049  SETMBR	R0	K6	K11
      0x70020053,  //  004A  JMP		#009F
      0x880C0106,  //  004B  GETMBR	R3	R0	K6
      0x54120004,  //  004C  LDINT	R4	5
      0x280C0604,  //  004D  GE	R3	R3	R4
      0x780E0016,  //  004E  JMPF	R3	#0066
      0x880C0106,  //  004F  GETMBR	R3	R0	K6
      0x54120005,  //  0050  LDINT	R4	6
      0x180C0604,  //  0051  LE	R3	R3	R4
      0x780E0012,  //  0052  JMPF	R3	#0066
      0x600C0009,  //  0053  GETGBL	R3	G9
      0x88100107,  //  0054  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0055  CALL	R3	1
      0x541200FE,  //  0056  LDINT	R4	255
      0x18100604,  //  0057  LE	R4	R3	R4
      0x78120004,  //  0058  JMPF	R4	#005E
      0x2810070A,  //  0059  GE	R4	R3	K10
      0x78120002,  //  005A  JMPF	R4	#005E
      0x54120003,  //  005B  LDINT	R4	4
      0x90020C04,  //  005C  SETMBR	R0	K6	R4
      0x70020006,  //  005D  JMP		#0065
      0x5412FFFE,  //  005E  LDINT	R4	65535
      0x18100604,  //  005F  LE	R4	R3	R4
      0x78120003,  //  0060  JMPF	R4	#0065
      0x2810070A,  //  0061  GE	R4	R3	K10
      0x78120001,  //  0062  JMPF	R4	#0065
      0x54120004,  //  0063  LDINT	R4	5
      0x90020C04,  //  0064  SETMBR	R0	K6	R4
      0x70020038,  //  0065  JMP		#009F
      0x880C0106,  //  0066  GETMBR	R3	R0	K6
      0x5412000F,  //  0067  LDINT	R4	16
      0x280C0604,  //  0068  GE	R3	R3	R4
      0x780E0018,  //  0069  JMPF	R3	#0083
      0x880C0106,  //  006A  GETMBR	R3	R0	K6
      0x54120012,  //  006B  LDINT	R4	19
      0x180C0604,  //  006C  LE	R3	R3	R4
      0x780E0014,  //  006D  JMPF	R3	#0083
      0x600C000C,  //  006E  GETGBL	R3	G12
      0x88100107,  //  006F  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0070  CALL	R3	1
      0x541200FE,  //  0071  LDINT	R4	255
      0x180C0604,  //  0072  LE	R3	R3	R4
      0x780E0002,  //  0073  JMPF	R3	#0077
      0x540E000F,  //  0074  LDINT	R3	16
      0x90020C03,  //  0075  SETMBR	R0	K6	R3
      0x7002000A,  //  0076  JMP		#0082
      0x600C000C,  //  0077  GETGBL	R3	G12
      0x88100107,  //  0078  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0079  CALL	R3	1
      0x5412FFFE,  //  007A  LDINT	R4	65535
      0x180C0604,  //  007B  LE	R3	R3	R4
      0x780E0002,  //  007C  JMPF	R3	#0080
      0x540E0010,  //  007D  LDINT	R3	17
      0x90020C03,  //  007E  SETMBR	R0	K6	R3
      0x70020001,  //  007F  JMP		#0082
      0x540E0011,  //  0080  LDINT	R3	18
      0x90020C03,  //  0081  SETMBR	R0	K6	R3
      0x7002001B,  //  0082  JMP		#009F
      0x880C0106,  //  0083  GETMBR	R3	R0	K6
      0x5412000B,  //  0084  LDINT	R4	12
      0x280C0604,  //  0085  GE	R3	R3	R4
      0x780E0017,  //  0086  JMPF	R3	#009F
      0x880C0106,  //  0087  GETMBR	R3	R0	K6
      0x5412000E,  //  0088  LDINT	R4	15
      0x180C0604,  //  0089  LE	R3	R3	R4
      0x780E0013,  //  008A  JMPF	R3	#009F
      0x600C000C,  //  008B  GETGBL	R3	G12
      0x88100107,  //  008C  GETMBR	R4	R0	K7
      0x7C0C0200,  //  008D  CALL	R3	1
      0x541200FE,  //  008E  LDINT	R4	255
      0x180C0604,  //  008F  LE	R3	R3	R4
      0x780E0002,  //  0090  JMPF	R3	#0094
      0x540E000B,  //  0091  LDINT	R3	12
      0x90020C03,  //  0092  SETMBR	R0	K6	R3
      0x7002000A,  //  0093  JMP		#009F
      0x600C000C,  //  0094  GETGBL	R3	G12
      0x88100107,  //  0095  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0096  CALL	R3	1
      0x5412FFFE,  //  0097  LDINT	R4	65535
      0x180C0604,  //  0098  LE	R3	R3	R4
      0x780E0002,  //  0099  JMPF	R3	#009D
      0x540E000C,  //  009A  LDINT	R3	13
      0x90020C03,  //  009B  SETMBR	R0	K6	R3
      0x70020001,  //  009C  JMP		#009F
      0x540E000D,  //  009D  LDINT	R3	14
      0x90020C03,  //  009E  SETMBR	R0	K6	R3
      0x8C0C0138,  //  009F  GETMET	R3	R0	K56
      0x5C140200,  //  00A0  MOVE	R5	R1
      0x7C0C0400,  //  00A1  CALL	R3	2
      0x880C0106,  //  00A2  GETMBR	R3	R0	K6
      0x1C0C070A,  //  00A3  EQ	R3	R3	K10
      0x740E0003,  //  00A4  JMPT	R3	#00A9
      0x880C0106,  //  00A5  GETMBR	R3	R0	K6
      0x54120003,  //  00A6  LDINT	R4	4
      0x1C0C0604,  //  00A7  EQ	R3	R3	R4
      0x780E0006,  //  00A8  JMPF	R3	#00B0
      0x8C0C0334,  //  00A9  GETMET	R3	R1	K52
      0x60140009,  //  00AA  GETGBL	R5	G9
      0x88180107,  //  00AB  GETMBR	R6	R0	K7
      0x7C140200,  //  00AC  CALL	R5	1
      0x5818000B,  //  00AD  LDCONST	R6	K11
      0x7C0C0600,  //  00AE  CALL	R3	3
      0x700200D6,  //  00AF  JMP		#0187
      0x880C0106,  //  00B0  GETMBR	R3	R0	K6
      0x1C0C070B,  //  00B1  EQ	R3	R3	K11
      0x740E0003,  //  00B2  JMPT	R3	#00B7
      0x880C0106,  //  00B3  GETMBR	R3	R0	K6
      0x54120004,  //  00B4  LDINT	R4	5
      0x1C0C0604,  //  00B5  EQ	R3	R3	R4
      0x780E0006,  //  00B6  JMPF	R3	#00BE
      0x8C0C0334,  //  00B7  GETMET	R3	R1	K52
      0x60140009,  //  00B8  GETGBL	R5	G9
      0x88180107,  //  00B9  GETMBR	R6	R0	K7
      0x7C140200,  //  00BA  CALL	R5	1
      0x5818000C,  //  00BB  LDCONST	R6	K12
      0x7C0C0600,  //  00BC  CALL	R3	3
      0x700200C8,  //  00BD  JMP		#0187
      0x880C0106,  //  00BE  GETMBR	R3	R0	K6
      0x1C0C070C,  //  00BF  EQ	R3	R3	K12
      0x740E0003,  //  00C0  JMPT	R3	#00C5
      0x880C0106,  //  00C1  GETMBR	R3	R0	K6
      0x54120005,  //  00C2  LDINT	R4	6
      0x1C0C0604,  //  00C3  EQ	R3	R3	R4
      0x780E0006,  //  00C4  JMPF	R3	#00CC
      0x8C0C0334,  //  00C5  GETMET	R3	R1	K52
      0x60140009,  //  00C6  GETGBL	R5	G9
      0x88180107,  //  00C7  GETMBR	R6	R0	K7
      0x7C140200,  //  00C8  CALL	R5	1
      0x541A0003,  //  00C9  LDINT	R6	4
      0x7C0C0600,  //  00CA  CALL	R3	3
      0x700200BA,  //  00CB  JMP		#0187
      0x880C0106,  //  00CC  GETMBR	R3	R0	K6
      0x1C0C070E,  //  00CD  EQ	R3	R3	K14
      0x740E0003,  //  00CE  JMPT	R3	#00D3
      0x880C0106,  //  00CF  GETMBR	R3	R0	K6
      0x54120006,  //  00D0  LDINT	R4	7
      0x1C0C0604,  //  00D1  EQ	R3	R3	R4
      0x780E002C,  //  00D2  JMPF	R3	#0100
      0x880C0107,  //  00D3  GETMBR	R3	R0	K7
      0x6010000F,  //  00D4  GETGBL	R4	G15
      0x5C140600,  //  00D5  MOVE	R5	R3
      0x60180015,  //  00D6  GETGBL	R6	G21
      0x7C100400,  //  00D7  CALL	R4	2
      0x78120006,  //  00D8  JMPF	R4	#00E0
      0x8C100739,  //  00D9  GETMET	R4	R3	K57
      0x7C100200,  //  00DA  CALL	R4	1
      0x8C10093A,  //  00DB  GETMET	R4	R4	K58
      0x541A0007,  //  00DC  LDINT	R6	8
      0x7C100400,  //  00DD  CALL	R4	2
      0x5C0C0800,  //  00DE  MOVE	R3	R4
      0x7002001D,  //  00DF  JMP		#00FE
      0x6010000F,  //  00E0  GETGBL	R4	G15
      0x5C140600,  //  00E1  MOVE	R5	R3
      0xB81A4800,  //  00E2  GETNGBL	R6	K36
      0x7C100400,  //  00E3  CALL	R4	2
      0x78120003,  //  00E4  JMPF	R4	#00E9
      0x8C10073B,  //  00E5  GETMET	R4	R3	K59
      0x7C100200,  //  00E6  CALL	R4	1
      0x5C0C0800,  //  00E7  MOVE	R3	R4
      0x70020014,  //  00E8  JMP		#00FE
      0x88100106,  //  00E9  GETMBR	R4	R0	K6
      0x1C10090E,  //  00EA  EQ	R4	R4	K14
      0x78120008,  //  00EB  JMPF	R4	#00F5
      0xB8124800,  //  00EC  GETNGBL	R4	K36
      0x60140009,  //  00ED  GETGBL	R5	G9
      0x5C180600,  //  00EE  MOVE	R6	R3
      0x7C140200,  //  00EF  CALL	R5	1
      0x7C100200,  //  00F0  CALL	R4	1
      0x8C10093B,  //  00F1  GETMET	R4	R4	K59
      0x7C100200,  //  00F2  CALL	R4	1
      0x5C0C0800,  //  00F3  MOVE	R3	R4
      0x70020008,  //  00F4  JMP		#00FE
      0xB8124800,  //  00F5  GETNGBL	R4	K36
      0x8C10093C,  //  00F6  GETMET	R4	R4	K60
      0x60180009,  //  00F7  GETGBL	R6	G9
      0x5C1C0600,  //  00F8  MOVE	R7	R3
      0x7C180200,  //  00F9  CALL	R6	1
      0x7C100400,  //  00FA  CALL	R4	2
      0x8C10093B,  //  00FB  GETMET	R4	R4	K59
      0x7C100200,  //  00FC  CALL	R4	1
      0x5C0C0800,  //  00FD  MOVE	R3	R4
      0x40100203,  //  00FE  CONNECT	R4	R1	R3
      0x70020086,  //  00FF  JMP		#0187
      0x880C0106,  //  0100  GETMBR	R3	R0	K6
      0x54120007,  //  0101  LDINT	R4	8
      0x1C0C0604,  //  0102  EQ	R3	R3	R4
      0x740E0003,  //  0103  JMPT	R3	#0108
      0x880C0106,  //  0104  GETMBR	R3	R0	K6
      0x54120008,  //  0105  LDINT	R4	9
      0x1C0C0604,  //  0106  EQ	R3	R3	R4
      0x780E0000,  //  0107  JMPF	R3	#0109
      0x7002007D,  //  0108  JMP		#0187
      0x880C0106,  //  0109  GETMBR	R3	R0	K6
      0x54120009,  //  010A  LDINT	R4	10
      0x1C0C0604,  //  010B  EQ	R3	R3	R4
      0x780E000D,  //  010C  JMPF	R3	#011B
      0x600C000C,  //  010D  GETGBL	R3	G12
      0x5C100200,  //  010E  MOVE	R4	R1
      0x7C0C0200,  //  010F  CALL	R3	1
      0x8C100334,  //  0110  GETMET	R4	R1	K52
      0x5818000A,  //  0111  LDCONST	R6	K10
      0x541E0003,  //  0112  LDINT	R7	4
      0x7C100600,  //  0113  CALL	R4	3
      0x8C10033D,  //  0114  GETMET	R4	R1	K61
      0x5C180600,  //  0115  MOVE	R6	R3
      0x601C000A,  //  0116  GETGBL	R7	G10
      0x88200107,  //  0117  GETMBR	R8	R0	K7
      0x7C1C0200,  //  0118  CALL	R7	1
      0x7C100600,  //  0119  CALL	R4	3
      0x7002006B,  //  011A  JMP		#0187
      0x880C0106,  //  011B  GETMBR	R3	R0	K6
      0x5412000A,  //  011C  LDINT	R4	11
      0x1C0C0604,  //  011D  EQ	R3	R3	R4
      0x780E0001,  //  011E  JMPF	R3	#0121
      0xB0061F10,  //  011F  RAISE	1	K15	K16
      0x70020065,  //  0120  JMP		#0187
      0x880C0106,  //  0121  GETMBR	R3	R0	K6
      0x5412000B,  //  0122  LDINT	R4	12
      0x1C0C0604,  //  0123  EQ	R3	R3	R4
      0x780E0015,  //  0124  JMPF	R3	#013B
      0x600C000C,  //  0125  GETGBL	R3	G12
      0x88100107,  //  0126  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0127  CALL	R3	1
      0x541200FE,  //  0128  LDINT	R4	255
      0x240C0604,  //  0129  GT	R3	R3	R4
      0x780E0000,  //  012A  JMPF	R3	#012C
      0xB0061F3E,  //  012B  RAISE	1	K15	K62
      0x8C0C0334,  //  012C  GETMET	R3	R1	K52
      0x6014000C,  //  012D  GETGBL	R5	G12
      0x88180107,  //  012E  GETMBR	R6	R0	K7
      0x7C140200,  //  012F  CALL	R5	1
      0x5818000B,  //  0130  LDCONST	R6	K11
      0x7C0C0600,  //  0131  CALL	R3	3
      0x600C0015,  //  0132  GETGBL	R3	G21
      0x7C0C0000,  //  0133  CALL	R3	0
      0x8C0C073F,  //  0134  GETMET	R3	R3	K63
      0x60140008,  //  0135  GETGBL	R5	G8
      0x88180107,  //  0136  GETMBR	R6	R0	K7
      0x7C140200,  //  0137  CALL	R5	1
      0x7C0C0400,  //  0138  CALL	R3	2
      0x400C0203,  //  0139  CONNECT	R3	R1	R3
      0x7002004B,  //  013A  JMP		#0187
      0x880C0106,  //  013B  GETMBR	R3	R0	K6
      0x5412000C,  //  013C  LDINT	R4	13
      0x1C0C0604,  //  013D  EQ	R3	R3	R4
      0x780E0015,  //  013E  JMPF	R3	#0155
      0x600C000C,  //  013F  GETGBL	R3	G12
      0x88100107,  //  0140  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0141  CALL	R3	1
      0x5412FFFE,  //  0142  LDINT	R4	65535
      0x240C0604,  //  0143  GT	R3	R3	R4
      0x780E0000,  //  0144  JMPF	R3	#0146
      0xB0061F3E,  //  0145  RAISE	1	K15	K62
      0x8C0C0334,  //  0146  GETMET	R3	R1	K52
      0x6014000C,  //  0147  GETGBL	R5	G12
      0x88180107,  //  0148  GETMBR	R6	R0	K7
      0x7C140200,  //  0149  CALL	R5	1
      0x5818000C,  //  014A  LDCONST	R6	K12
      0x7C0C0600,  //  014B  CALL	R3	3
      0x600C0015,  //  014C  GETGBL	R3	G21
      0x7C0C0000,  //  014D  CALL	R3	0
      0x8C0C0740,  //  014E  GETMET	R3	R3	K64
      0x60140008,  //  014F  GETGBL	R5	G8
      0x88180107,  //  0150  GETMBR	R6	R0	K7
      0x7C140200,  //  0151  CALL	R5	1
      0x7C0C0400,  //  0152  CALL	R3	2
      0x400C0203,  //  0153  CONNECT	R3	R1	R3
      0x70020031,  //  0154  JMP		#0187
      0x880C0106,  //  0155  GETMBR	R3	R0	K6
      0x5412000F,  //  0156  LDINT	R4	16
      0x1C0C0604,  //  0157  EQ	R3	R3	R4
      0x780E000F,  //  0158  JMPF	R3	#0169
      0x600C000C,  //  0159  GETGBL	R3	G12
      0x88100107,  //  015A  GETMBR	R4	R0	K7
      0x7C0C0200,  //  015B  CALL	R3	1
      0x541200FE,  //  015C  LDINT	R4	255
      0x240C0604,  //  015D  GT	R3	R3	R4
      0x780E0000,  //  015E  JMPF	R3	#0160
      0xB0061F41,  //  015F  RAISE	1	K15	K65
      0x8C0C0334,  //  0160  GETMET	R3	R1	K52
      0x6014000C,  //  0161  GETGBL	R5	G12
      0x88180107,  //  0162  GETMBR	R6	R0	K7
      0x7C140200,  //  0163  CALL	R5	1
      0x5818000B,  //  0164  LDCONST	R6	K11
      0x7C0C0600,  //  0165  CALL	R3	3
      0x880C0107,  //  0166  GETMBR	R3	R0	K7
      0x400C0203,  //  0167  CONNECT	R3	R1	R3
      0x7002001D,  //  0168  JMP		#0187
      0x880C0106,  //  0169  GETMBR	R3	R0	K6
      0x54120010,  //  016A  LDINT	R4	17
      0x1C0C0604,  //  016B  EQ	R3	R3	R4
      0x780E000F,  //  016C  JMPF	R3	#017D
      0x600C000C,  //  016D  GETGBL	R3	G12
      0x88100107,  //  016E  GETMBR	R4	R0	K7
      0x7C0C0200,  //  016F  CALL	R3	1
      0x5412FFFE,  //  0170  LDINT	R4	65535
      0x240C0604,  //  0171  GT	R3	R3	R4
      0x780E0000,  //  0172  JMPF	R3	#0174
      0xB0061F41,  //  0173  RAISE	1	K15	K65
      0x8C0C0334,  //  0174  GETMET	R3	R1	K52
      0x6014000C,  //  0175  GETGBL	R5	G12
      0x88180107,  //  0176  GETMBR	R6	R0	K7
      0x7C140200,  //  0177  CALL	R5	1
      0x5818000C,  //  0178  LDCONST	R6	K12
      0x7C0C0600,  //  0179  CALL	R3	3
      0x880C0107,  //  017A  GETMBR	R3	R0	K7
      0x400C0203,  //  017B  CONNECT	R3	R1	R3
      0x70020009,  //  017C  JMP		#0187
      0x880C0106,  //  017D  GETMBR	R3	R0	K6
      0x54120013,  //  017E  LDINT	R4	20
      0x1C0C0604,  //  017F  EQ	R3	R3	R4
      0x780E0000,  //  0180  JMPF	R3	#0182
      0x70020004,  //  0181  JMP		#0187
      0x600C0008,  //  0182  GETGBL	R3	G8
      0x88100106,  //  0183  GETMBR	R4	R0	K6
      0x7C0C0200,  //  0184  CALL	R3	1
      0x000E2203,  //  0185  ADD	R3	K17	R3
      0xB0061E03,  //  0186  RAISE	1	K15	R3
      0x80040200,  //  0187  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _encode_tag_len
********************************************************************/
be_local_closure(class_Matter_TLV_item__encode_tag_len,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(_encode_tag_len),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x88040104,  //  0004  GETMBR	R1	R0	K4
      0x70020000,  //  0005  JMP		#0007
      0x5804000A,  //  0006  LDCONST	R1	K10
      0x540AFFFF,  //  0007  LDINT	R2	65536
      0x28080202,  //  0008  GE	R2	R1	R2
      0x740A0002,  //  0009  JMPT	R2	#000D
      0x1408030A,  //  000A  LT	R2	R1	K10
      0x740A0000,  //  000B  JMPT	R2	#000D
      0x50080001,  //  000C  LDBOOL	R2	0	1
      0x50080200,  //  000D  LDBOOL	R2	1	0
      0x580C000A,  //  000E  LDCONST	R3	K10
      0x88100102,  //  000F  GETMBR	R4	R0	K2
      0x4C140000,  //  0010  LDNIL	R5
      0x20100805,  //  0011  NE	R4	R4	R5
      0x78120006,  //  0012  JMPF	R4	#001A
      0x780A0002,  //  0013  JMPF	R2	#0017
      0x54120008,  //  0014  LDINT	R4	9
      0x80040800,  //  0015  RET	1	R4
      0x70020001,  //  0016  JMP		#0019
      0x54120006,  //  0017  LDINT	R4	7
      0x80040800,  //  0018  RET	1	R4
      0x7002001A,  //  0019  JMP		#0035
      0x88100103,  //  001A  GETMBR	R4	R0	K3
      0x5415FFFE,  //  001B  LDINT	R5	-1
      0x1C100805,  //  001C  EQ	R4	R4	R5
      0x78120005,  //  001D  JMPF	R4	#0024
      0x780A0002,  //  001E  JMPF	R2	#0022
      0x54120004,  //  001F  LDINT	R4	5
      0x80040800,  //  0020  RET	1	R4
      0x70020000,  //  0021  JMP		#0023
      0x80061C00,  //  0022  RET	1	K14
      0x70020010,  //  0023  JMP		#0035
      0x88100103,  //  0024  GETMBR	R4	R0	K3
      0x4C140000,  //  0025  LDNIL	R5
      0x20100805,  //  0026  NE	R4	R4	R5
      0x78120005,  //  0027  JMPF	R4	#002E
      0x780A0002,  //  0028  JMPF	R2	#002C
      0x54120004,  //  0029  LDINT	R4	5
      0x80040800,  //  002A  RET	1	R4
      0x70020000,  //  002B  JMP		#002D
      0x80061C00,  //  002C  RET	1	K14
      0x70020006,  //  002D  JMP		#0035
      0x88100105,  //  002E  GETMBR	R4	R0	K5
      0x4C140000,  //  002F  LDNIL	R5
      0x20100805,  //  0030  NE	R4	R4	R5
      0x78120001,  //  0031  JMPF	R4	#0034
      0x80061800,  //  0032  RET	1	K12
      0x70020000,  //  0033  JMP		#0035
      0x80061600,  //  0034  RET	1	K11
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_contextspecific
********************************************************************/
be_local_closure(class_Matter_TLV_item_set_contextspecific,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(set_contextspecific),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C080133,  //  0000  GETMET	R2	R0	K51
      0x7C080200,  //  0001  CALL	R2	1
      0x60080009,  //  0002  GETGBL	R2	G9
      0x5C0C0200,  //  0003  MOVE	R3	R1
      0x7C080200,  //  0004  CALL	R2	1
      0x90020A02,  //  0005  SETMBR	R0	K5	R2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_parent
********************************************************************/
be_local_closure(class_Matter_TLV_item_set_parent,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(set_parent),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_TLV
********************************************************************/
be_local_closure(class_Matter_TLV_item_create_TLV,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(create_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x58080035,  //  0000  LDCONST	R2	K53
      0x4C0C0000,  //  0001  LDNIL	R3
      0x200C0203,  //  0002  NE	R3	R1	R3
      0x740E0002,  //  0003  JMPT	R3	#0007
      0x540E0013,  //  0004  LDINT	R3	20
      0x1C0C0003,  //  0005  EQ	R3	R0	R3
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x5C0C0400,  //  0007  MOVE	R3	R2
      0x7C0C0000,  //  0008  CALL	R3	0
      0x900E0C00,  //  0009  SETMBR	R3	K6	R0
      0x900E0E01,  //  000A  SETMBR	R3	K7	R1
      0x80040600,  //  000B  RET	1	R3
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_anonymoustag
********************************************************************/
be_local_closure(class_Matter_TLV_item_set_anonymoustag,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_item,     /* shared constants */
    be_str_weak(set_anonymoustag),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040133,  //  0000  GETMET	R1	R0	K51
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_TLV_item
********************************************************************/
be_local_class(Matter_TLV_item,
    8,
    NULL,
    be_nested_map(32,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(reset, -1), be_const_closure(class_Matter_TLV_item_reset_closure) },
        { be_const_key_weak(set_or_nil, -1), be_const_closure(class_Matter_TLV_item_set_or_nil_closure) },
        { be_const_key_weak(set_anonymoustag, 30), be_const_closure(class_Matter_TLV_item_set_anonymoustag_closure) },
        { be_const_key_weak(set, -1), be_const_closure(class_Matter_TLV_item_set_closure) },
        { be_const_key_weak(create_TLV, -1), be_const_static_closure(class_Matter_TLV_item_create_TLV_closure) },
        { be_const_key_weak(tostring, 22), be_const_closure(class_Matter_TLV_item_tostring_closure) },
        { be_const_key_weak(tag_sub, -1), be_const_var(5) },
        { be_const_key_weak(is_array, -1), be_const_bool(0) },
        { be_const_key_weak(to_str_val, 4), be_const_closure(class_Matter_TLV_item_to_str_val_closure) },
        { be_const_key_weak(parent, -1), be_const_var(0) },
        { be_const_key_weak(is_list, -1), be_const_bool(0) },
        { be_const_key_weak(is_struct, -1), be_const_bool(0) },
        { be_const_key_weak(parse, -1), be_const_closure(class_Matter_TLV_item_parse_closure) },
        { be_const_key_weak(set_parent, -1), be_const_closure(class_Matter_TLV_item_set_parent_closure) },
        { be_const_key_weak(set_commonprofile, -1), be_const_closure(class_Matter_TLV_item_set_commonprofile_closure) },
        { be_const_key_weak(_encode_tag, -1), be_const_closure(class_Matter_TLV_item__encode_tag_closure) },
        { be_const_key_weak(TLV, -1), be_const_class(be_class_Matter_TLV) },
        { be_const_key_weak(sort, 25), be_const_static_closure(class_Matter_TLV_item_sort_closure) },
        { be_const_key_weak(tag_vendor, -1), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_TLV_item_init_closure) },
        { be_const_key_weak(_cmp_gt, -1), be_const_closure(class_Matter_TLV_item__cmp_gt_closure) },
        { be_const_key_weak(tag_number, -1), be_const_var(4) },
        { be_const_key_weak(tlv2raw, -1), be_const_closure(class_Matter_TLV_item_tlv2raw_closure) },
        { be_const_key_weak(_encode_tag_len, -1), be_const_closure(class_Matter_TLV_item__encode_tag_len_closure) },
        { be_const_key_weak(next_idx, -1), be_const_var(1) },
        { be_const_key_weak(tag_profile, 16), be_const_var(3) },
        { be_const_key_weak(typ, -1), be_const_var(6) },
        { be_const_key_weak(set_contextspecific, -1), be_const_closure(class_Matter_TLV_item_set_contextspecific_closure) },
        { be_const_key_weak(val, 13), be_const_var(7) },
        { be_const_key_weak(to_TLV, 9), be_const_closure(class_Matter_TLV_item_to_TLV_closure) },
        { be_const_key_weak(set_fulltag, -1), be_const_closure(class_Matter_TLV_item_set_fulltag_closure) },
        { be_const_key_weak(encode_len, 2), be_const_closure(class_Matter_TLV_item_encode_len_closure) },
    })),
    be_str_weak(Matter_TLV_item)
);
// compact class 'Matter_TLV_list' ktab size: 45, total: 87 (saved 336 bytes)
static const bvalue be_ktab_class_Matter_TLV_list[45] = {
  /* K0   */  be_nested_str_weak(findsub),
  /* K1   */  be_nested_str_weak(val),
  /* K2   */  be_nested_str_weak(_encode_tag),
  /* K3   */  be_nested_str_weak(is_struct),
  /* K4   */  be_nested_str_weak(copy),
  /* K5   */  be_nested_str_weak(sort),
  /* K6   */  be_nested_str_weak(tlv2raw),
  /* K7   */  be_nested_str_weak(stop_iteration),
  /* K8   */  be_nested_str_weak(add),
  /* K9   */  be_const_int(1),
  /* K10  */  be_nested_str_weak(tostring),
  /* K11  */  be_nested_str_weak(tostring_inner),
  /* K12  */  be_nested_str_weak(_X5B_X5B),
  /* K13  */  be_nested_str_weak(_X5D_X5D),
  /* K14  */  be_nested_str_weak(TLV),
  /* K15  */  be_nested_str_weak(Matter_TLV_struct),
  /* K16  */  be_nested_str_weak(tag_sub),
  /* K17  */  be_nested_str_weak(push),
  /* K18  */  be_nested_str_weak(Matter_TLV_list),
  /* K19  */  be_nested_str_weak(parse),
  /* K20  */  be_nested_str_weak(next_idx),
  /* K21  */  be_nested_str_weak(init),
  /* K22  */  be_nested_str_weak(typ),
  /* K23  */  be_nested_str_weak(getsub),
  /* K24  */  be_nested_str_weak(value_error),
  /* K25  */  be_nested_str_weak(sub_X20not_X20found),
  /* K26  */  be_nested_str_weak(to_TLV),
  /* K27  */  be_nested_str_weak(Matter_TLV_item),
  /* K28  */  be_nested_str_weak(Matter_TLV_array),
  /* K29  */  be_nested_str_weak(),
  /* K30  */  be_nested_str_weak(tag_profile),
  /* K31  */  be_nested_str_weak(Matter_X3A_X3A),
  /* K32  */  be_nested_str_weak(tag_number),
  /* K33  */  be_nested_str_weak(0x_X2508X_X20),
  /* K34  */  be_nested_str_weak(tag_vendor),
  /* K35  */  be_nested_str_weak(0x_X2504X_X3A_X3A),
  /* K36  */  be_nested_str_weak(0x_X2504X_X3A),
  /* K37  */  be_nested_str_weak(_X25i_X20),
  /* K38  */  be_const_int(0),
  /* K39  */  be_nested_str_weak(_X3D_X20),
  /* K40  */  be_nested_str_weak(concat),
  /* K41  */  be_nested_str_weak(_X2C_X20),
  /* K42  */  be_nested_str_weak(_X20),
  /* K43  */  be_nested_str_weak(_encode_tag_len),
  /* K44  */  be_nested_str_weak(encode_len),
};


extern const bclass be_class_Matter_TLV_list;

/********************************************************************
** Solidified function: findsubval
********************************************************************/
be_local_closure(class_Matter_TLV_list_findsubval,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(findsubval),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x4C100000,  //  0003  LDNIL	R4
      0x20100604,  //  0004  NE	R4	R3	R4
      0x78120001,  //  0005  JMPF	R4	#0008
      0x88100701,  //  0006  GETMBR	R4	R3	K1
      0x80040800,  //  0007  RET	1	R4
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
be_local_closure(class_Matter_TLV_list_tlv2raw,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0002,  //  0002  JMPF	R2	#0006
      0x60080015,  //  0003  GETGBL	R2	G21
      0x7C080000,  //  0004  CALL	R2	0
      0x5C040400,  //  0005  MOVE	R1	R2
      0x8C080102,  //  0006  GETMET	R2	R0	K2
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x88080101,  //  0009  GETMBR	R2	R0	K1
      0x880C0103,  //  000A  GETMBR	R3	R0	K3
      0x780E0005,  //  000B  JMPF	R3	#0012
      0x8C0C0504,  //  000C  GETMET	R3	R2	K4
      0x7C0C0200,  //  000D  CALL	R3	1
      0x5C080600,  //  000E  MOVE	R2	R3
      0x8C0C0105,  //  000F  GETMET	R3	R0	K5
      0x5C140400,  //  0010  MOVE	R5	R2
      0x7C0C0400,  //  0011  CALL	R3	2
      0x600C0010,  //  0012  GETGBL	R3	G16
      0x5C100400,  //  0013  MOVE	R4	R2
      0x7C0C0200,  //  0014  CALL	R3	1
      0xA802000C,  //  0015  EXBLK	0	#0023
      0x5C100600,  //  0016  MOVE	R4	R3
      0x7C100000,  //  0017  CALL	R4	0
      0x6014000F,  //  0018  GETGBL	R5	G15
      0x5C180800,  //  0019  MOVE	R6	R4
      0x601C0015,  //  001A  GETGBL	R7	G21
      0x7C140400,  //  001B  CALL	R5	2
      0x78160001,  //  001C  JMPF	R5	#001F
      0x40140204,  //  001D  CONNECT	R5	R1	R4
      0x70020002,  //  001E  JMP		#0022
      0x8C140906,  //  001F  GETMET	R5	R4	K6
      0x5C1C0200,  //  0020  MOVE	R7	R1
      0x7C140400,  //  0021  CALL	R5	2
      0x7001FFF2,  //  0022  JMP		#0016
      0x580C0007,  //  0023  LDCONST	R3	K7
      0xAC0C0200,  //  0024  CATCH	R3	1	0
      0xB0080000,  //  0025  RAISE	2	R0	R0
      0x8C0C0308,  //  0026  GETMET	R3	R1	K8
      0x54160017,  //  0027  LDINT	R5	24
      0x58180009,  //  0028  LDCONST	R6	K9
      0x7C0C0600,  //  0029  CALL	R3	3
      0x80040200,  //  002A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_str_val
********************************************************************/
be_local_closure(class_Matter_TLV_list_to_str_val,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(to_str_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C04010A,  //  0000  GETMET	R1	R0	K10
      0x500C0200,  //  0001  LDBOOL	R3	1	0
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Matter_TLV_list_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C08010B,  //  0000  GETMET	R2	R0	K11
      0x50100000,  //  0001  LDBOOL	R4	0	0
      0x5814000C,  //  0002  LDCONST	R5	K12
      0x5818000D,  //  0003  LDCONST	R6	K13
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x7C080A00,  //  0005  CALL	R2	5
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: size
********************************************************************/
be_local_closure(class_Matter_TLV_list_size,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(size),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setitem
********************************************************************/
be_local_closure(class_Matter_TLV_list_setitem,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(setitem),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x880C0101,  //  0000  GETMBR	R3	R0	K1
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_struct
********************************************************************/
be_local_closure(class_Matter_TLV_list_add_struct,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(add_struct),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8808010E,  //  0000  GETMBR	R2	R0	K14
      0x8C08050F,  //  0001  GETMET	R2	R2	K15
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x900A2001,  //  0004  SETMBR	R2	K16	R1
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x8C0C0711,  //  0006  GETMET	R3	R3	K17
      0x5C140400,  //  0007  MOVE	R5	R2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x80040400,  //  0009  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_list
********************************************************************/
be_local_closure(class_Matter_TLV_list_add_list,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(add_list),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8808010E,  //  0000  GETMBR	R2	R0	K14
      0x8C080512,  //  0001  GETMET	R2	R2	K18
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x900A2001,  //  0004  SETMBR	R2	K16	R1
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x8C0C0711,  //  0006  GETMET	R3	R3	K17
      0x5C140400,  //  0007  MOVE	R5	R2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x80040400,  //  0009  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Matter_TLV_list_parse,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x940C0202,  //  0000  GETIDX	R3	R1	R2
      0x54120017,  //  0001  LDINT	R4	24
      0x200C0604,  //  0002  NE	R3	R3	R4
      0x780E000B,  //  0003  JMPF	R3	#0010
      0x880C010E,  //  0004  GETMBR	R3	R0	K14
      0x8C0C0713,  //  0005  GETMET	R3	R3	K19
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x5C1C0000,  //  0008  MOVE	R7	R0
      0x7C0C0800,  //  0009  CALL	R3	4
      0x88080714,  //  000A  GETMBR	R2	R3	K20
      0x88100101,  //  000B  GETMBR	R4	R0	K1
      0x8C100911,  //  000C  GETMET	R4	R4	K17
      0x5C180600,  //  000D  MOVE	R6	R3
      0x7C100400,  //  000E  CALL	R4	2
      0x7001FFEF,  //  000F  JMP		#0000
      0x00080509,  //  0010  ADD	R2	R2	K9
      0x90022802,  //  0011  SETMBR	R0	K20	R2
      0x80040400,  //  0012  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_TLV_list_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080515,  //  0003  GETMET	R2	R2	K21
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x540A0016,  //  0006  LDINT	R2	23
      0x90022C02,  //  0007  SETMBR	R0	K22	R2
      0x60080012,  //  0008  GETGBL	R2	G18
      0x7C080000,  //  0009  CALL	R2	0
      0x90020202,  //  000A  SETMBR	R0	K1	R2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: item
********************************************************************/
be_local_closure(class_Matter_TLV_list_item,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(item),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x94080401,  //  0001  GETIDX	R2	R2	R1
      0x80040400,  //  0002  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: getsubval
********************************************************************/
be_local_closure(class_Matter_TLV_list_getsubval,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(getsubval),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C080117,  //  0000  GETMET	R2	R0	K23
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x88080501,  //  0003  GETMBR	R2	R2	K1
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: getsub
********************************************************************/
be_local_closure(class_Matter_TLV_list_getsub,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(getsub),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0xB0063119,  //  0006  RAISE	1	K24	K25
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_obj
********************************************************************/
be_local_closure(class_Matter_TLV_list_add_obj,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(add_obj),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0403,  //  0001  NE	R3	R2	R3
      0x780E0010,  //  0002  JMPF	R3	#0014
      0x600C000F,  //  0003  GETGBL	R3	G15
      0x5C100400,  //  0004  MOVE	R4	R2
      0x60140015,  //  0005  GETGBL	R5	G21
      0x7C0C0400,  //  0006  CALL	R3	2
      0x780E0004,  //  0007  JMPF	R3	#000D
      0x880C0101,  //  0008  GETMBR	R3	R0	K1
      0x8C0C0711,  //  0009  GETMET	R3	R3	K17
      0x5C140400,  //  000A  MOVE	R5	R2
      0x7C0C0400,  //  000B  CALL	R3	2
      0x70020006,  //  000C  JMP		#0014
      0x8C0C051A,  //  000D  GETMET	R3	R2	K26
      0x7C0C0200,  //  000E  CALL	R3	1
      0x900E2001,  //  000F  SETMBR	R3	K16	R1
      0x88100101,  //  0010  GETMBR	R4	R0	K1
      0x8C100911,  //  0011  GETMET	R4	R4	K17
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C100400,  //  0013  CALL	R4	2
      0x80040000,  //  0014  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_TLV
********************************************************************/
be_local_closure(class_Matter_TLV_list_add_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(add_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x20100604,  //  0001  NE	R4	R3	R4
      0x74120002,  //  0002  JMPT	R4	#0006
      0x54120013,  //  0003  LDINT	R4	20
      0x1C100404,  //  0004  EQ	R4	R2	R4
      0x7812000A,  //  0005  JMPF	R4	#0011
      0x8810010E,  //  0006  GETMBR	R4	R0	K14
      0x8C10091B,  //  0007  GETMET	R4	R4	K27
      0x5C180000,  //  0008  MOVE	R6	R0
      0x7C100400,  //  0009  CALL	R4	2
      0x90122001,  //  000A  SETMBR	R4	K16	R1
      0x90122C02,  //  000B  SETMBR	R4	K22	R2
      0x90120203,  //  000C  SETMBR	R4	K1	R3
      0x88140101,  //  000D  GETMBR	R5	R0	K1
      0x8C140B11,  //  000E  GETMET	R5	R5	K17
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x7C140400,  //  0010  CALL	R5	2
      0x80040000,  //  0011  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_array
********************************************************************/
be_local_closure(class_Matter_TLV_list_add_array,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(add_array),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8808010E,  //  0000  GETMBR	R2	R0	K14
      0x8C08051C,  //  0001  GETMET	R2	R2	K28
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x900A2001,  //  0004  SETMBR	R2	K16	R1
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x8C0C0711,  //  0006  GETMET	R3	R3	K17
      0x5C140400,  //  0007  MOVE	R5	R2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x80040400,  //  0009  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: findsub
********************************************************************/
be_local_closure(class_Matter_TLV_list_findsub,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(findsub),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x600C0010,  //  0000  GETGBL	R3	G16
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0xA8020007,  //  0003  EXBLK	0	#000C
      0x5C100600,  //  0004  MOVE	R4	R3
      0x7C100000,  //  0005  CALL	R4	0
      0x88140910,  //  0006  GETMBR	R5	R4	K16
      0x1C140A01,  //  0007  EQ	R5	R5	R1
      0x78160001,  //  0008  JMPF	R5	#000B
      0xA8040001,  //  0009  EXBLK	1	1
      0x80040800,  //  000A  RET	1	R4
      0x7001FFF7,  //  000B  JMP		#0004
      0x580C0007,  //  000C  LDCONST	R3	K7
      0xAC0C0200,  //  000D  CATCH	R3	1	0
      0xB0080000,  //  000E  RAISE	2	R0	R0
      0x80040400,  //  000F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring_inner
********************************************************************/
be_local_closure(class_Matter_TLV_list_tostring_inner,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(tostring_inner),
    &be_const_str_solidified,
    ( &(const binstruction[85]) {  /* code */
      0x5814001D,  //  0000  LDCONST	R5	K29
      0xA802004A,  //  0001  EXBLK	0	#004D
      0x50180200,  //  0002  LDBOOL	R6	1	0
      0x20180806,  //  0003  NE	R6	R4	R6
      0x781A0038,  //  0004  JMPF	R6	#003E
      0x8818011E,  //  0005  GETMBR	R6	R0	K30
      0x541DFFFE,  //  0006  LDINT	R7	-1
      0x1C180C07,  //  0007  EQ	R6	R6	R7
      0x781A000A,  //  0008  JMPF	R6	#0014
      0x00140B1F,  //  0009  ADD	R5	R5	K31
      0x88180120,  //  000A  GETMBR	R6	R0	K32
      0x4C1C0000,  //  000B  LDNIL	R7
      0x20180C07,  //  000C  NE	R6	R6	R7
      0x781A0004,  //  000D  JMPF	R6	#0013
      0x60180018,  //  000E  GETGBL	R6	G24
      0x581C0021,  //  000F  LDCONST	R7	K33
      0x88200120,  //  0010  GETMBR	R8	R0	K32
      0x7C180400,  //  0011  CALL	R6	2
      0x00140A06,  //  0012  ADD	R5	R5	R6
      0x70020023,  //  0013  JMP		#0038
      0x88180122,  //  0014  GETMBR	R6	R0	K34
      0x4C1C0000,  //  0015  LDNIL	R7
      0x20180C07,  //  0016  NE	R6	R6	R7
      0x781A0004,  //  0017  JMPF	R6	#001D
      0x60180018,  //  0018  GETGBL	R6	G24
      0x581C0023,  //  0019  LDCONST	R7	K35
      0x88200122,  //  001A  GETMBR	R8	R0	K34
      0x7C180400,  //  001B  CALL	R6	2
      0x00140A06,  //  001C  ADD	R5	R5	R6
      0x8818011E,  //  001D  GETMBR	R6	R0	K30
      0x4C1C0000,  //  001E  LDNIL	R7
      0x20180C07,  //  001F  NE	R6	R6	R7
      0x781A0004,  //  0020  JMPF	R6	#0026
      0x60180018,  //  0021  GETGBL	R6	G24
      0x581C0024,  //  0022  LDCONST	R7	K36
      0x8820011E,  //  0023  GETMBR	R8	R0	K30
      0x7C180400,  //  0024  CALL	R6	2
      0x00140A06,  //  0025  ADD	R5	R5	R6
      0x88180120,  //  0026  GETMBR	R6	R0	K32
      0x4C1C0000,  //  0027  LDNIL	R7
      0x20180C07,  //  0028  NE	R6	R6	R7
      0x781A0004,  //  0029  JMPF	R6	#002F
      0x60180018,  //  002A  GETGBL	R6	G24
      0x581C0021,  //  002B  LDCONST	R7	K33
      0x88200120,  //  002C  GETMBR	R8	R0	K32
      0x7C180400,  //  002D  CALL	R6	2
      0x00140A06,  //  002E  ADD	R5	R5	R6
      0x88180110,  //  002F  GETMBR	R6	R0	K16
      0x4C1C0000,  //  0030  LDNIL	R7
      0x20180C07,  //  0031  NE	R6	R6	R7
      0x781A0004,  //  0032  JMPF	R6	#0038
      0x60180018,  //  0033  GETGBL	R6	G24
      0x581C0025,  //  0034  LDCONST	R7	K37
      0x88200110,  //  0035  GETMBR	R8	R0	K16
      0x7C180400,  //  0036  CALL	R6	2
      0x00140A06,  //  0037  ADD	R5	R5	R6
      0x6018000C,  //  0038  GETGBL	R6	G12
      0x5C1C0A00,  //  0039  MOVE	R7	R5
      0x7C180200,  //  003A  CALL	R6	1
      0x24180D26,  //  003B  GT	R6	R6	K38
      0x781A0000,  //  003C  JMPF	R6	#003E
      0x00140B27,  //  003D  ADD	R5	R5	K39
      0x00140A02,  //  003E  ADD	R5	R5	R2
      0x88180101,  //  003F  GETMBR	R6	R0	K1
      0x8C180D04,  //  0040  GETMET	R6	R6	K4
      0x7C180200,  //  0041  CALL	R6	1
      0x78060002,  //  0042  JMPF	R1	#0046
      0x8C1C0105,  //  0043  GETMET	R7	R0	K5
      0x5C240C00,  //  0044  MOVE	R9	R6
      0x7C1C0400,  //  0045  CALL	R7	2
      0x8C1C0D28,  //  0046  GETMET	R7	R6	K40
      0x58240029,  //  0047  LDCONST	R9	K41
      0x7C1C0400,  //  0048  CALL	R7	2
      0x00140A07,  //  0049  ADD	R5	R5	R7
      0x00140A03,  //  004A  ADD	R5	R5	R3
      0xA8040001,  //  004B  EXBLK	1	1
      0x70020006,  //  004C  JMP		#0054
      0xAC180002,  //  004D  CATCH	R6	0	2
      0x70020003,  //  004E  JMP		#0053
      0x00200D2A,  //  004F  ADD	R8	R6	K42
      0x00201007,  //  0050  ADD	R8	R8	R7
      0x80041000,  //  0051  RET	1	R8
      0x70020000,  //  0052  JMP		#0054
      0xB0080000,  //  0053  RAISE	2	R0	R0
      0x80040A00,  //  0054  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push
********************************************************************/
be_local_closure(class_Matter_TLV_list_push,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(push),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x8C080511,  //  0001  GETMET	R2	R2	K17
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: findsubtyp
********************************************************************/
be_local_closure(class_Matter_TLV_list_findsubtyp,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(findsubtyp),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x880C0516,  //  0006  GETMBR	R3	R2	K22
      0x80040600,  //  0007  RET	1	R3
      0x4C0C0000,  //  0008  LDNIL	R3
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: encode_len
********************************************************************/
be_local_closure(class_Matter_TLV_list_encode_len,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_list,     /* shared constants */
    be_str_weak(encode_len),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C04012B,  //  0000  GETMET	R1	R0	K43
      0x7C040200,  //  0001  CALL	R1	1
      0x58080026,  //  0002  LDCONST	R2	K38
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x88100101,  //  0004  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x140C0403,  //  0006  LT	R3	R2	R3
      0x780E0006,  //  0007  JMPF	R3	#000F
      0x880C0101,  //  0008  GETMBR	R3	R0	K1
      0x940C0602,  //  0009  GETIDX	R3	R3	R2
      0x8C0C072C,  //  000A  GETMET	R3	R3	K44
      0x7C0C0200,  //  000B  CALL	R3	1
      0x00040203,  //  000C  ADD	R1	R1	R3
      0x00080509,  //  000D  ADD	R2	R2	K9
      0x7001FFF3,  //  000E  JMP		#0003
      0x00040309,  //  000F  ADD	R1	R1	K9
      0x80040200,  //  0010  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_TLV_list
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_class(Matter_TLV_list,
    0,
    &be_class_Matter_TLV_item,
    be_nested_map(22,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(encode_len, 7), be_const_closure(class_Matter_TLV_list_encode_len_closure) },
        { be_const_key_weak(tlv2raw, -1), be_const_closure(class_Matter_TLV_list_tlv2raw_closure) },
        { be_const_key_weak(to_str_val, 14), be_const_closure(class_Matter_TLV_list_to_str_val_closure) },
        { be_const_key_weak(findsubval, 5), be_const_closure(class_Matter_TLV_list_findsubval_closure) },
        { be_const_key_weak(size, -1), be_const_closure(class_Matter_TLV_list_size_closure) },
        { be_const_key_weak(findsubtyp, 21), be_const_closure(class_Matter_TLV_list_findsubtyp_closure) },
        { be_const_key_weak(add_struct, 19), be_const_closure(class_Matter_TLV_list_add_struct_closure) },
        { be_const_key_weak(push, -1), be_const_closure(class_Matter_TLV_list_push_closure) },
        { be_const_key_weak(parse, -1), be_const_closure(class_Matter_TLV_list_parse_closure) },
        { be_const_key_weak(init, 0), be_const_closure(class_Matter_TLV_list_init_closure) },
        { be_const_key_weak(item, -1), be_const_closure(class_Matter_TLV_list_item_closure) },
        { be_const_key_weak(tostring_inner, -1), be_const_closure(class_Matter_TLV_list_tostring_inner_closure) },
        { be_const_key_weak(getsubval, 20), be_const_closure(class_Matter_TLV_list_getsubval_closure) },
        { be_const_key_weak(getsub, -1), be_const_closure(class_Matter_TLV_list_getsub_closure) },
        { be_const_key_weak(findsub, -1), be_const_closure(class_Matter_TLV_list_findsub_closure) },
        { be_const_key_weak(add_list, 17), be_const_closure(class_Matter_TLV_list_add_list_closure) },
        { be_const_key_weak(add_array, -1), be_const_closure(class_Matter_TLV_list_add_array_closure) },
        { be_const_key_weak(add_TLV, -1), be_const_closure(class_Matter_TLV_list_add_TLV_closure) },
        { be_const_key_weak(setitem, 11), be_const_closure(class_Matter_TLV_list_setitem_closure) },
        { be_const_key_weak(add_obj, -1), be_const_closure(class_Matter_TLV_list_add_obj_closure) },
        { be_const_key_weak(is_list, -1), be_const_bool(1) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_Matter_TLV_list_tostring_closure) },
    })),
    be_str_weak(Matter_TLV_list)
);

extern const bclass be_class_Matter_TLV_struct;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Matter_TLV_struct_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tostring_inner),
    /* K1   */  be_nested_str_weak(_X7B),
    /* K2   */  be_nested_str_weak(_X7D),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x50100200,  //  0001  LDBOOL	R4	1	0
      0x58140001,  //  0002  LDCONST	R5	K1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x7C080A00,  //  0005  CALL	R2	5
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_TLV_struct_init,   /* name */
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
    /* K1   */  be_nested_str_weak(typ),
    /* K2   */  be_nested_str_weak(val),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x540A0014,  //  0006  LDINT	R2	21
      0x90020202,  //  0007  SETMBR	R0	K1	R2
      0x60080012,  //  0008  GETGBL	R2	G18
      0x7C080000,  //  0009  CALL	R2	0
      0x90020402,  //  000A  SETMBR	R0	K2	R2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_TLV_struct
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_class(Matter_TLV_struct,
    0,
    &be_class_Matter_TLV_list,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_TLV_struct_init_closure) },
        { be_const_key_weak(tostring, -1), be_const_closure(class_Matter_TLV_struct_tostring_closure) },
        { be_const_key_weak(is_list, -1), be_const_bool(0) },
        { be_const_key_weak(is_struct, 0), be_const_bool(1) },
    })),
    be_str_weak(Matter_TLV_struct)
);
// compact class 'Matter_TLV_array' ktab size: 15, total: 16 (saved 8 bytes)
static const bvalue be_ktab_class_Matter_TLV_array[15] = {
  /* K0   */  be_nested_str_weak(tostring_inner),
  /* K1   */  be_nested_str_weak(_X5B),
  /* K2   */  be_nested_str_weak(_X5D),
  /* K3   */  be_nested_str_weak(init),
  /* K4   */  be_nested_str_weak(typ),
  /* K5   */  be_nested_str_weak(val),
  /* K6   */  be_nested_str_weak(TLV),
  /* K7   */  be_nested_str_weak(parse),
  /* K8   */  be_nested_str_weak(next_idx),
  /* K9   */  be_nested_str_weak(tag_vendor),
  /* K10  */  be_nested_str_weak(tag_profile),
  /* K11  */  be_nested_str_weak(tag_number),
  /* K12  */  be_nested_str_weak(tag_sub),
  /* K13  */  be_nested_str_weak(push),
  /* K14  */  be_const_int(1),
};


extern const bclass be_class_Matter_TLV_array;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Matter_TLV_array_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_array,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x50100000,  //  0001  LDBOOL	R4	0	0
      0x58140001,  //  0002  LDCONST	R5	K1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x7C080A00,  //  0005  CALL	R2	5
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_TLV_array_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_array,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080503,  //  0003  GETMET	R2	R2	K3
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x540A0015,  //  0006  LDINT	R2	22
      0x90020802,  //  0007  SETMBR	R0	K4	R2
      0x60080012,  //  0008  GETGBL	R2	G18
      0x7C080000,  //  0009  CALL	R2	0
      0x90020A02,  //  000A  SETMBR	R0	K5	R2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Matter_TLV_array_parse,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV_array,     /* shared constants */
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x940C0202,  //  0000  GETIDX	R3	R1	R2
      0x54120017,  //  0001  LDINT	R4	24
      0x200C0604,  //  0002  NE	R3	R3	R4
      0x780E0013,  //  0003  JMPF	R3	#0018
      0x880C0106,  //  0004  GETMBR	R3	R0	K6
      0x8C0C0707,  //  0005  GETMET	R3	R3	K7
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x5C1C0000,  //  0008  MOVE	R7	R0
      0x7C0C0800,  //  0009  CALL	R3	4
      0x88080708,  //  000A  GETMBR	R2	R3	K8
      0x4C100000,  //  000B  LDNIL	R4
      0x900E1204,  //  000C  SETMBR	R3	K9	R4
      0x4C100000,  //  000D  LDNIL	R4
      0x900E1404,  //  000E  SETMBR	R3	K10	R4
      0x4C100000,  //  000F  LDNIL	R4
      0x900E1604,  //  0010  SETMBR	R3	K11	R4
      0x4C100000,  //  0011  LDNIL	R4
      0x900E1804,  //  0012  SETMBR	R3	K12	R4
      0x88100105,  //  0013  GETMBR	R4	R0	K5
      0x8C10090D,  //  0014  GETMET	R4	R4	K13
      0x5C180600,  //  0015  MOVE	R6	R3
      0x7C100400,  //  0016  CALL	R4	2
      0x7001FFE7,  //  0017  JMP		#0000
      0x0008050E,  //  0018  ADD	R2	R2	K14
      0x90021002,  //  0019  SETMBR	R0	K8	R2
      0x80040400,  //  001A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_TLV_array
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_class(Matter_TLV_array,
    0,
    &be_class_Matter_TLV_list,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, 1), be_const_closure(class_Matter_TLV_array_tostring_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_TLV_array_init_closure) },
        { be_const_key_weak(parse, 3), be_const_closure(class_Matter_TLV_array_parse_closure) },
        { be_const_key_weak(is_list, -1), be_const_bool(0) },
        { be_const_key_weak(is_array, -1), be_const_bool(1) },
    })),
    be_str_weak(Matter_TLV_array)
);
extern const bclass be_class_Matter_TLV;
// compact class 'Matter_TLV' ktab size: 18, total: 20 (saved 16 bytes)
static const bvalue be_ktab_class_Matter_TLV[18] = {
  /* K0   */  be_const_class(be_class_Matter_TLV),
  /* K1   */  be_nested_str_weak(Matter_TLV_item),
  /* K2   */  be_nested_str_weak(create_TLV),
  /* K3   */  be_const_int(0),
  /* K4   */  be_const_int(1),
  /* K5   */  be_nested_str_weak(invalid_X20TLV_X20type_X20),
  /* K6   */  be_nested_str_weak(TLV_error),
  /* K7   */  be_nested_str_weak(Matter_TLV_struct),
  /* K8   */  be_nested_str_weak(Matter_TLV_array),
  /* K9   */  be_nested_str_weak(Matter_TLV_list),
  /* K10  */  be_nested_str_weak(typ),
  /* K11  */  be_nested_str_weak(tag_vendor),
  /* K12  */  be_nested_str_weak(get),
  /* K13  */  be_const_int(2),
  /* K14  */  be_nested_str_weak(tag_profile),
  /* K15  */  be_nested_str_weak(tag_sub),
  /* K16  */  be_nested_str_weak(tag_number),
  /* K17  */  be_nested_str_weak(parse),
};


extern const bclass be_class_Matter_TLV;

/********************************************************************
** Solidified function: create_TLV
********************************************************************/
be_local_closure(class_Matter_TLV_create_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV,     /* shared constants */
    be_str_weak(create_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x880C0501,  //  0001  GETMBR	R3	R2	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x5C140000,  //  0003  MOVE	R5	R0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C0C0600,  //  0005  CALL	R3	3
      0x80040600,  //  0006  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(class_Matter_TLV_parse,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_TLV,     /* shared constants */
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[118]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x4C140000,  //  0002  LDNIL	R5
      0x1C140205,  //  0003  EQ	R5	R1	R5
      0x78160000,  //  0004  JMPF	R5	#0006
      0x58040003,  //  0005  LDCONST	R1	K3
      0x94140001,  //  0006  GETIDX	R5	R0	R1
      0x541A001E,  //  0007  LDINT	R6	31
      0x2C140A06,  //  0008  AND	R5	R5	R6
      0x94180001,  //  0009  GETIDX	R6	R0	R1
      0x541E00DF,  //  000A  LDINT	R7	224
      0x2C180C07,  //  000B  AND	R6	R6	R7
      0x00040304,  //  000C  ADD	R1	R1	K4
      0x541E0017,  //  000D  LDINT	R7	24
      0x241C0A07,  //  000E  GT	R7	R5	R7
      0x781E0004,  //  000F  JMPF	R7	#0015
      0x601C0008,  //  0010  GETGBL	R7	G8
      0x5C200A00,  //  0011  MOVE	R8	R5
      0x7C1C0200,  //  0012  CALL	R7	1
      0x001E0A07,  //  0013  ADD	R7	K5	R7
      0xB0060C07,  //  0014  RAISE	1	K6	R7
      0x4C1C0000,  //  0015  LDNIL	R7
      0x54220014,  //  0016  LDINT	R8	21
      0x1C200A08,  //  0017  EQ	R8	R5	R8
      0x78220004,  //  0018  JMPF	R8	#001E
      0x8C200707,  //  0019  GETMET	R8	R3	K7
      0x5C280400,  //  001A  MOVE	R10	R2
      0x7C200400,  //  001B  CALL	R8	2
      0x5C1C1000,  //  001C  MOVE	R7	R8
      0x70020013,  //  001D  JMP		#0032
      0x54220015,  //  001E  LDINT	R8	22
      0x1C200A08,  //  001F  EQ	R8	R5	R8
      0x78220004,  //  0020  JMPF	R8	#0026
      0x8C200708,  //  0021  GETMET	R8	R3	K8
      0x5C280400,  //  0022  MOVE	R10	R2
      0x7C200400,  //  0023  CALL	R8	2
      0x5C1C1000,  //  0024  MOVE	R7	R8
      0x7002000B,  //  0025  JMP		#0032
      0x54220016,  //  0026  LDINT	R8	23
      0x1C200A08,  //  0027  EQ	R8	R5	R8
      0x78220004,  //  0028  JMPF	R8	#002E
      0x8C200709,  //  0029  GETMET	R8	R3	K9
      0x5C280400,  //  002A  MOVE	R10	R2
      0x7C200400,  //  002B  CALL	R8	2
      0x5C1C1000,  //  002C  MOVE	R7	R8
      0x70020003,  //  002D  JMP		#0032
      0x8C200701,  //  002E  GETMET	R8	R3	K1
      0x5C280400,  //  002F  MOVE	R10	R2
      0x7C200400,  //  0030  CALL	R8	2
      0x5C1C1000,  //  0031  MOVE	R7	R8
      0x901E1405,  //  0032  SETMBR	R7	K10	R5
      0x542200BF,  //  0033  LDINT	R8	192
      0x1C200C08,  //  0034  EQ	R8	R6	R8
      0x74220002,  //  0035  JMPT	R8	#0039
      0x542200DF,  //  0036  LDINT	R8	224
      0x1C200C08,  //  0037  EQ	R8	R6	R8
      0x7822000B,  //  0038  JMPF	R8	#0045
      0x8C20010C,  //  0039  GETMET	R8	R0	K12
      0x5C280200,  //  003A  MOVE	R10	R1
      0x582C000D,  //  003B  LDCONST	R11	K13
      0x7C200600,  //  003C  CALL	R8	3
      0x901E1608,  //  003D  SETMBR	R7	K11	R8
      0x8C20010C,  //  003E  GETMET	R8	R0	K12
      0x0028030D,  //  003F  ADD	R10	R1	K13
      0x582C000D,  //  0040  LDCONST	R11	K13
      0x7C200600,  //  0041  CALL	R8	3
      0x901E1C08,  //  0042  SETMBR	R7	K14	R8
      0x54220003,  //  0043  LDINT	R8	4
      0x00040208,  //  0044  ADD	R1	R1	R8
      0x5422003F,  //  0045  LDINT	R8	64
      0x1C200C08,  //  0046  EQ	R8	R6	R8
      0x74220002,  //  0047  JMPT	R8	#004B
      0x5422005F,  //  0048  LDINT	R8	96
      0x1C200C08,  //  0049  EQ	R8	R6	R8
      0x78220003,  //  004A  JMPF	R8	#004F
      0x4C200000,  //  004B  LDNIL	R8
      0x901E1608,  //  004C  SETMBR	R7	K11	R8
      0x5421FFFE,  //  004D  LDINT	R8	-1
      0x901E1C08,  //  004E  SETMBR	R7	K14	R8
      0x1C200D03,  //  004F  EQ	R8	R6	K3
      0x78220000,  //  0050  JMPF	R8	#0052
      0x7002001D,  //  0051  JMP		#0070
      0x5422001F,  //  0052  LDINT	R8	32
      0x1C200C08,  //  0053  EQ	R8	R6	R8
      0x78220003,  //  0054  JMPF	R8	#0059
      0x94200001,  //  0055  GETIDX	R8	R0	R1
      0x901E1E08,  //  0056  SETMBR	R7	K15	R8
      0x00040304,  //  0057  ADD	R1	R1	K4
      0x70020016,  //  0058  JMP		#0070
      0x542200BF,  //  0059  LDINT	R8	192
      0x1C200C08,  //  005A  EQ	R8	R6	R8
      0x74220005,  //  005B  JMPT	R8	#0062
      0x5422007F,  //  005C  LDINT	R8	128
      0x1C200C08,  //  005D  EQ	R8	R6	R8
      0x74220002,  //  005E  JMPT	R8	#0062
      0x5422003F,  //  005F  LDINT	R8	64
      0x1C200C08,  //  0060  EQ	R8	R6	R8
      0x78220006,  //  0061  JMPF	R8	#0069
      0x8C20010C,  //  0062  GETMET	R8	R0	K12
      0x5C280200,  //  0063  MOVE	R10	R1
      0x582C000D,  //  0064  LDCONST	R11	K13
      0x7C200600,  //  0065  CALL	R8	3
      0x901E2008,  //  0066  SETMBR	R7	K16	R8
      0x0004030D,  //  0067  ADD	R1	R1	K13
      0x70020006,  //  0068  JMP		#0070
      0x8C20010C,  //  0069  GETMET	R8	R0	K12
      0x5C280200,  //  006A  MOVE	R10	R1
      0x542E0003,  //  006B  LDINT	R11	4
      0x7C200600,  //  006C  CALL	R8	3
      0x901E2008,  //  006D  SETMBR	R7	K16	R8
      0x54220003,  //  006E  LDINT	R8	4
      0x00040208,  //  006F  ADD	R1	R1	R8
      0x8C200F11,  //  0070  GETMET	R8	R7	K17
      0x5C280000,  //  0071  MOVE	R10	R0
      0x5C2C0200,  //  0072  MOVE	R11	R1
      0x7C200600,  //  0073  CALL	R8	3
      0x5C041000,  //  0074  MOVE	R1	R8
      0x80040E00,  //  0075  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_TLV
********************************************************************/
be_local_class(Matter_TLV,
    0,
    NULL,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(create_TLV, -1), be_const_static_closure(class_Matter_TLV_create_TLV_closure) },
        { be_const_key_weak(Matter_TLV_item, -1), be_const_class(be_class_Matter_TLV_item) },
        { be_const_key_weak(Matter_TLV_list, -1), be_const_class(be_class_Matter_TLV_list) },
        { be_const_key_weak(Matter_TLV_array, -1), be_const_class(be_class_Matter_TLV_array) },
        { be_const_key_weak(parse, -1), be_const_static_closure(class_Matter_TLV_parse_closure) },
        { be_const_key_weak(_len, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(25,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(1),
        be_const_int(2),
        be_const_int(4),
        be_const_int(8),
        be_const_int(1),
        be_const_int(2),
        be_const_int(4),
        be_const_int(8),
        be_const_int(0),
        be_const_int(0),
        be_const_int(4),
        be_const_int(8),
        be_const_int(-1),
        be_const_int(-2),
        be_const_int(-4),
        be_const_int(-8),
        be_const_int(-1),
        be_const_int(-2),
        be_const_int(-4),
        be_const_int(-8),
        be_const_int(0),
        be_const_int(-99),
        be_const_int(-99),
        be_const_int(-99),
        be_const_int(0),
    }))    ) } )) },
        { be_const_key_weak(_type, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(25,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(i1),
        be_nested_str_weak(i2),
        be_nested_str_weak(i4),
        be_nested_str_weak(i8),
        be_nested_str_weak(u1),
        be_nested_str_weak(u2),
        be_nested_str_weak(u4),
        be_nested_str_weak(u8),
        be_nested_str_weak(bool),
        be_nested_str_weak(bool),
        be_nested_str_weak(float),
        be_nested_str_weak(double),
        be_nested_str_weak(UTF1),
        be_nested_str_weak(UTF2),
        be_nested_str_weak(UTF4),
        be_nested_str_weak(UTF8),
        be_nested_str_weak(b1),
        be_nested_str_weak(b2),
        be_nested_str_weak(b4),
        be_nested_str_weak(b8),
        be_nested_str_weak(null),
        be_nested_str_weak(struct),
        be_nested_str_weak(array),
        be_nested_str_weak(list),
        be_nested_str_weak(end),
    }))    ) } )) },
        { be_const_key_weak(Matter_TLV_struct, -1), be_const_class(be_class_Matter_TLV_struct) },
    })),
    be_str_weak(Matter_TLV)
);
/********************************************************************/
/* End of solidification */
