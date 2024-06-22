/* Solidification of Matter_TLV.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_TLV_item;

/********************************************************************
** Solidified function: reset
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_reset,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(parent),
    /* K1   */  be_nested_str_weak(next_idx),
    /* K2   */  be_nested_str_weak(tag_vendor),
    /* K3   */  be_nested_str_weak(tag_profile),
    /* K4   */  be_nested_str_weak(tag_number),
    /* K5   */  be_nested_str_weak(tag_sub),
    /* K6   */  be_nested_str_weak(typ),
    /* K7   */  be_nested_str_weak(val),
    }),
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
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_set_or_nil,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(reset),
    /* K1   */  be_nested_str_weak(typ),
    /* K2   */  be_nested_str_weak(val),
    }),
    be_str_weak(set_or_nil),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
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
      0x90020201,  //  000C  SETMBR	R0	K1	R1
      0x90020402,  //  000D  SETMBR	R0	K2	R2
      0x80040000,  //  000E  RET	1	R0
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: encode_len
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_encode_len,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[32]) {     /* constants */
    /* K0   */  be_nested_str_weak(TLV),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(typ),
    /* K3   */  be_nested_str_weak(RAW),
    /* K4   */  be_nested_str_weak(val),
    /* K5   */  be_nested_str_weak(BFALSE),
    /* K6   */  be_nested_str_weak(BTRUE),
    /* K7   */  be_nested_str_weak(I2),
    /* K8   */  be_nested_str_weak(I4),
    /* K9   */  be_nested_str_weak(I1),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(U1),
    /* K13  */  be_nested_str_weak(B1),
    /* K14  */  be_nested_str_weak(B8),
    /* K15  */  be_nested_str_weak(B2),
    /* K16  */  be_nested_str_weak(B4),
    /* K17  */  be_nested_str_weak(UTF1),
    /* K18  */  be_nested_str_weak(UTF8),
    /* K19  */  be_nested_str_weak(UTF2),
    /* K20  */  be_nested_str_weak(UTF4),
    /* K21  */  be_nested_str_weak(_encode_tag_len),
    /* K22  */  be_const_int(1),
    /* K23  */  be_const_int(2),
    /* K24  */  be_nested_str_weak(I8),
    /* K25  */  be_nested_str_weak(U8),
    /* K26  */  be_nested_str_weak(FLOAT),
    /* K27  */  be_nested_str_weak(DOUBLE),
    /* K28  */  be_nested_str_weak(value_error),
    /* K29  */  be_nested_str_weak(Unsupported_X20type_X20TLV_X2EDOUBLE),
    /* K30  */  be_nested_str_weak(NULL),
    /* K31  */  be_nested_str_weak(unsupported_X20type_X20),
    }),
    be_str_weak(encode_len),
    &be_const_str_solidified,
    ( &(const binstruction[258]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x58080001,  //  0001  LDCONST	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x88100303,  //  0003  GETMBR	R4	R1	K3
      0x1C0C0604,  //  0004  EQ	R3	R3	R4
      0x780E0003,  //  0005  JMPF	R3	#000A
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x88100104,  //  0007  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0008  CALL	R3	1
      0x80040600,  //  0009  RET	1	R3
      0x880C0102,  //  000A  GETMBR	R3	R0	K2
      0x88100305,  //  000B  GETMBR	R4	R1	K5
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x740E0003,  //  000D  JMPT	R3	#0012
      0x880C0102,  //  000E  GETMBR	R3	R0	K2
      0x88100306,  //  000F  GETMBR	R4	R1	K6
      0x1C0C0604,  //  0010  EQ	R3	R3	R4
      0x780E0008,  //  0011  JMPF	R3	#001B
      0x600C0017,  //  0012  GETGBL	R3	G23
      0x88100104,  //  0013  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0014  CALL	R3	1
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x880C0306,  //  0016  GETMBR	R3	R1	K6
      0x70020000,  //  0017  JMP		#0019
      0x880C0305,  //  0018  GETMBR	R3	R1	K5
      0x90020403,  //  0019  SETMBR	R0	K2	R3
      0x70020070,  //  001A  JMP		#008C
      0x880C0102,  //  001B  GETMBR	R3	R0	K2
      0x88100307,  //  001C  GETMBR	R4	R1	K7
      0x280C0604,  //  001D  GE	R3	R3	R4
      0x780E0018,  //  001E  JMPF	R3	#0038
      0x880C0102,  //  001F  GETMBR	R3	R0	K2
      0x88100308,  //  0020  GETMBR	R4	R1	K8
      0x180C0604,  //  0021  LE	R3	R3	R4
      0x780E0014,  //  0022  JMPF	R3	#0038
      0x600C0009,  //  0023  GETGBL	R3	G9
      0x88100104,  //  0024  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0025  CALL	R3	1
      0x5412007E,  //  0026  LDINT	R4	127
      0x18100604,  //  0027  LE	R4	R3	R4
      0x78120005,  //  0028  JMPF	R4	#002F
      0x5411FF7F,  //  0029  LDINT	R4	-128
      0x28100604,  //  002A  GE	R4	R3	R4
      0x78120002,  //  002B  JMPF	R4	#002F
      0x88100309,  //  002C  GETMBR	R4	R1	K9
      0x90020404,  //  002D  SETMBR	R0	K2	R4
      0x70020007,  //  002E  JMP		#0037
      0x54127FFE,  //  002F  LDINT	R4	32767
      0x18100604,  //  0030  LE	R4	R3	R4
      0x78120004,  //  0031  JMPF	R4	#0037
      0x54117FFF,  //  0032  LDINT	R4	-32768
      0x28100604,  //  0033  GE	R4	R3	R4
      0x78120001,  //  0034  JMPF	R4	#0037
      0x88100307,  //  0035  GETMBR	R4	R1	K7
      0x90020404,  //  0036  SETMBR	R0	K2	R4
      0x70020053,  //  0037  JMP		#008C
      0x880C0102,  //  0038  GETMBR	R3	R0	K2
      0x8810030A,  //  0039  GETMBR	R4	R1	K10
      0x280C0604,  //  003A  GE	R3	R3	R4
      0x780E0016,  //  003B  JMPF	R3	#0053
      0x880C0102,  //  003C  GETMBR	R3	R0	K2
      0x8810030B,  //  003D  GETMBR	R4	R1	K11
      0x180C0604,  //  003E  LE	R3	R3	R4
      0x780E0012,  //  003F  JMPF	R3	#0053
      0x600C0009,  //  0040  GETGBL	R3	G9
      0x88100104,  //  0041  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0042  CALL	R3	1
      0x541200FE,  //  0043  LDINT	R4	255
      0x18100604,  //  0044  LE	R4	R3	R4
      0x78120004,  //  0045  JMPF	R4	#004B
      0x28100701,  //  0046  GE	R4	R3	K1
      0x78120002,  //  0047  JMPF	R4	#004B
      0x8810030C,  //  0048  GETMBR	R4	R1	K12
      0x90020404,  //  0049  SETMBR	R0	K2	R4
      0x70020006,  //  004A  JMP		#0052
      0x5412FFFE,  //  004B  LDINT	R4	65535
      0x18100604,  //  004C  LE	R4	R3	R4
      0x78120003,  //  004D  JMPF	R4	#0052
      0x28100701,  //  004E  GE	R4	R3	K1
      0x78120001,  //  004F  JMPF	R4	#0052
      0x8810030A,  //  0050  GETMBR	R4	R1	K10
      0x90020404,  //  0051  SETMBR	R0	K2	R4
      0x70020038,  //  0052  JMP		#008C
      0x880C0102,  //  0053  GETMBR	R3	R0	K2
      0x8810030D,  //  0054  GETMBR	R4	R1	K13
      0x280C0604,  //  0055  GE	R3	R3	R4
      0x780E0018,  //  0056  JMPF	R3	#0070
      0x880C0102,  //  0057  GETMBR	R3	R0	K2
      0x8810030E,  //  0058  GETMBR	R4	R1	K14
      0x180C0604,  //  0059  LE	R3	R3	R4
      0x780E0014,  //  005A  JMPF	R3	#0070
      0x600C000C,  //  005B  GETGBL	R3	G12
      0x88100104,  //  005C  GETMBR	R4	R0	K4
      0x7C0C0200,  //  005D  CALL	R3	1
      0x541200FE,  //  005E  LDINT	R4	255
      0x180C0604,  //  005F  LE	R3	R3	R4
      0x780E0002,  //  0060  JMPF	R3	#0064
      0x880C030D,  //  0061  GETMBR	R3	R1	K13
      0x90020403,  //  0062  SETMBR	R0	K2	R3
      0x7002000A,  //  0063  JMP		#006F
      0x600C000C,  //  0064  GETGBL	R3	G12
      0x88100104,  //  0065  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0066  CALL	R3	1
      0x5412FFFE,  //  0067  LDINT	R4	65535
      0x180C0604,  //  0068  LE	R3	R3	R4
      0x780E0002,  //  0069  JMPF	R3	#006D
      0x880C030F,  //  006A  GETMBR	R3	R1	K15
      0x90020403,  //  006B  SETMBR	R0	K2	R3
      0x70020001,  //  006C  JMP		#006F
      0x880C0310,  //  006D  GETMBR	R3	R1	K16
      0x90020403,  //  006E  SETMBR	R0	K2	R3
      0x7002001B,  //  006F  JMP		#008C
      0x880C0102,  //  0070  GETMBR	R3	R0	K2
      0x88100311,  //  0071  GETMBR	R4	R1	K17
      0x280C0604,  //  0072  GE	R3	R3	R4
      0x780E0017,  //  0073  JMPF	R3	#008C
      0x880C0102,  //  0074  GETMBR	R3	R0	K2
      0x88100312,  //  0075  GETMBR	R4	R1	K18
      0x180C0604,  //  0076  LE	R3	R3	R4
      0x780E0013,  //  0077  JMPF	R3	#008C
      0x600C000C,  //  0078  GETGBL	R3	G12
      0x88100104,  //  0079  GETMBR	R4	R0	K4
      0x7C0C0200,  //  007A  CALL	R3	1
      0x541200FE,  //  007B  LDINT	R4	255
      0x180C0604,  //  007C  LE	R3	R3	R4
      0x780E0002,  //  007D  JMPF	R3	#0081
      0x880C0311,  //  007E  GETMBR	R3	R1	K17
      0x90020403,  //  007F  SETMBR	R0	K2	R3
      0x7002000A,  //  0080  JMP		#008C
      0x600C000C,  //  0081  GETGBL	R3	G12
      0x88100104,  //  0082  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0083  CALL	R3	1
      0x5412FFFE,  //  0084  LDINT	R4	65535
      0x180C0604,  //  0085  LE	R3	R3	R4
      0x780E0002,  //  0086  JMPF	R3	#008A
      0x880C0313,  //  0087  GETMBR	R3	R1	K19
      0x90020403,  //  0088  SETMBR	R0	K2	R3
      0x70020001,  //  0089  JMP		#008C
      0x880C0314,  //  008A  GETMBR	R3	R1	K20
      0x90020403,  //  008B  SETMBR	R0	K2	R3
      0x8C0C0115,  //  008C  GETMET	R3	R0	K21
      0x7C0C0200,  //  008D  CALL	R3	1
      0x00080403,  //  008E  ADD	R2	R2	R3
      0x880C0102,  //  008F  GETMBR	R3	R0	K2
      0x88100309,  //  0090  GETMBR	R4	R1	K9
      0x1C0C0604,  //  0091  EQ	R3	R3	R4
      0x740E0003,  //  0092  JMPT	R3	#0097
      0x880C0102,  //  0093  GETMBR	R3	R0	K2
      0x8810030C,  //  0094  GETMBR	R4	R1	K12
      0x1C0C0604,  //  0095  EQ	R3	R3	R4
      0x780E0001,  //  0096  JMPF	R3	#0099
      0x00080516,  //  0097  ADD	R2	R2	K22
      0x70020067,  //  0098  JMP		#0101
      0x880C0102,  //  0099  GETMBR	R3	R0	K2
      0x88100307,  //  009A  GETMBR	R4	R1	K7
      0x1C0C0604,  //  009B  EQ	R3	R3	R4
      0x740E0003,  //  009C  JMPT	R3	#00A1
      0x880C0102,  //  009D  GETMBR	R3	R0	K2
      0x8810030A,  //  009E  GETMBR	R4	R1	K10
      0x1C0C0604,  //  009F  EQ	R3	R3	R4
      0x780E0001,  //  00A0  JMPF	R3	#00A3
      0x00080517,  //  00A1  ADD	R2	R2	K23
      0x7002005D,  //  00A2  JMP		#0101
      0x880C0102,  //  00A3  GETMBR	R3	R0	K2
      0x88100308,  //  00A4  GETMBR	R4	R1	K8
      0x1C0C0604,  //  00A5  EQ	R3	R3	R4
      0x740E0003,  //  00A6  JMPT	R3	#00AB
      0x880C0102,  //  00A7  GETMBR	R3	R0	K2
      0x8810030B,  //  00A8  GETMBR	R4	R1	K11
      0x1C0C0604,  //  00A9  EQ	R3	R3	R4
      0x780E0002,  //  00AA  JMPF	R3	#00AE
      0x540E0003,  //  00AB  LDINT	R3	4
      0x00080403,  //  00AC  ADD	R2	R2	R3
      0x70020052,  //  00AD  JMP		#0101
      0x880C0102,  //  00AE  GETMBR	R3	R0	K2
      0x88100318,  //  00AF  GETMBR	R4	R1	K24
      0x1C0C0604,  //  00B0  EQ	R3	R3	R4
      0x740E0003,  //  00B1  JMPT	R3	#00B6
      0x880C0102,  //  00B2  GETMBR	R3	R0	K2
      0x88100319,  //  00B3  GETMBR	R4	R1	K25
      0x1C0C0604,  //  00B4  EQ	R3	R3	R4
      0x780E0002,  //  00B5  JMPF	R3	#00B9
      0x540E0007,  //  00B6  LDINT	R3	8
      0x00080403,  //  00B7  ADD	R2	R2	R3
      0x70020047,  //  00B8  JMP		#0101
      0x880C0102,  //  00B9  GETMBR	R3	R0	K2
      0x88100305,  //  00BA  GETMBR	R4	R1	K5
      0x1C0C0604,  //  00BB  EQ	R3	R3	R4
      0x740E0003,  //  00BC  JMPT	R3	#00C1
      0x880C0102,  //  00BD  GETMBR	R3	R0	K2
      0x88100306,  //  00BE  GETMBR	R4	R1	K6
      0x1C0C0604,  //  00BF  EQ	R3	R3	R4
      0x780E0000,  //  00C0  JMPF	R3	#00C2
      0x7002003E,  //  00C1  JMP		#0101
      0x880C0102,  //  00C2  GETMBR	R3	R0	K2
      0x8810031A,  //  00C3  GETMBR	R4	R1	K26
      0x1C0C0604,  //  00C4  EQ	R3	R3	R4
      0x780E0002,  //  00C5  JMPF	R3	#00C9
      0x540E0003,  //  00C6  LDINT	R3	4
      0x00080403,  //  00C7  ADD	R2	R2	R3
      0x70020037,  //  00C8  JMP		#0101
      0x880C0102,  //  00C9  GETMBR	R3	R0	K2
      0x8810031B,  //  00CA  GETMBR	R4	R1	K27
      0x1C0C0604,  //  00CB  EQ	R3	R3	R4
      0x780E0001,  //  00CC  JMPF	R3	#00CF
      0xB006391D,  //  00CD  RAISE	1	K28	K29
      0x70020031,  //  00CE  JMP		#0101
      0x880C0102,  //  00CF  GETMBR	R3	R0	K2
      0x88100311,  //  00D0  GETMBR	R4	R1	K17
      0x1C0C0604,  //  00D1  EQ	R3	R3	R4
      0x780E0005,  //  00D2  JMPF	R3	#00D9
      0x600C000C,  //  00D3  GETGBL	R3	G12
      0x88100104,  //  00D4  GETMBR	R4	R0	K4
      0x7C0C0200,  //  00D5  CALL	R3	1
      0x000E2C03,  //  00D6  ADD	R3	K22	R3
      0x00080403,  //  00D7  ADD	R2	R2	R3
      0x70020027,  //  00D8  JMP		#0101
      0x880C0102,  //  00D9  GETMBR	R3	R0	K2
      0x88100313,  //  00DA  GETMBR	R4	R1	K19
      0x1C0C0604,  //  00DB  EQ	R3	R3	R4
      0x780E0005,  //  00DC  JMPF	R3	#00E3
      0x600C000C,  //  00DD  GETGBL	R3	G12
      0x88100104,  //  00DE  GETMBR	R4	R0	K4
      0x7C0C0200,  //  00DF  CALL	R3	1
      0x000E2E03,  //  00E0  ADD	R3	K23	R3
      0x00080403,  //  00E1  ADD	R2	R2	R3
      0x7002001D,  //  00E2  JMP		#0101
      0x880C0102,  //  00E3  GETMBR	R3	R0	K2
      0x8810030D,  //  00E4  GETMBR	R4	R1	K13
      0x1C0C0604,  //  00E5  EQ	R3	R3	R4
      0x780E0005,  //  00E6  JMPF	R3	#00ED
      0x600C000C,  //  00E7  GETGBL	R3	G12
      0x88100104,  //  00E8  GETMBR	R4	R0	K4
      0x7C0C0200,  //  00E9  CALL	R3	1
      0x000E2C03,  //  00EA  ADD	R3	K22	R3
      0x00080403,  //  00EB  ADD	R2	R2	R3
      0x70020013,  //  00EC  JMP		#0101
      0x880C0102,  //  00ED  GETMBR	R3	R0	K2
      0x8810030F,  //  00EE  GETMBR	R4	R1	K15
      0x1C0C0604,  //  00EF  EQ	R3	R3	R4
      0x780E0005,  //  00F0  JMPF	R3	#00F7
      0x600C000C,  //  00F1  GETGBL	R3	G12
      0x88100104,  //  00F2  GETMBR	R4	R0	K4
      0x7C0C0200,  //  00F3  CALL	R3	1
      0x000E2E03,  //  00F4  ADD	R3	K23	R3
      0x00080403,  //  00F5  ADD	R2	R2	R3
      0x70020009,  //  00F6  JMP		#0101
      0x880C0102,  //  00F7  GETMBR	R3	R0	K2
      0x8810031E,  //  00F8  GETMBR	R4	R1	K30
      0x1C0C0604,  //  00F9  EQ	R3	R3	R4
      0x780E0000,  //  00FA  JMPF	R3	#00FC
      0x70020004,  //  00FB  JMP		#0101
      0x600C0008,  //  00FC  GETGBL	R3	G8
      0x88100102,  //  00FD  GETMBR	R4	R0	K2
      0x7C0C0200,  //  00FE  CALL	R3	1
      0x000E3E03,  //  00FF  ADD	R3	K31	R3
      0xB0063803,  //  0100  RAISE	1	K28	R3
      0x80040400,  //  0101  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_set,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(reset),
    /* K1   */  be_nested_str_weak(typ),
    /* K2   */  be_nested_str_weak(val),
    }),
    be_str_weak(set),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x7C0C0200,  //  0001  CALL	R3	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x200C0403,  //  0003  NE	R3	R2	R3
      0x740E0002,  //  0004  JMPT	R3	#0008
      0x540E0013,  //  0005  LDINT	R3	20
      0x1C0C0203,  //  0006  EQ	R3	R1	R3
      0x780E0002,  //  0007  JMPF	R3	#000B
      0x90020201,  //  0008  SETMBR	R0	K1	R1
      0x90020402,  //  0009  SETMBR	R0	K2	R2
      0x80040000,  //  000A  RET	1	R0
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_TLV
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_to_TLV,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    0,                          /* has constants */
    NULL,                       /* no const */
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
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[34]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(tag_profile),
    /* K2   */  be_nested_str_weak(Matter_X3A_X3A),
    /* K3   */  be_nested_str_weak(tag_number),
    /* K4   */  be_nested_str_weak(0x_X2508X_X20),
    /* K5   */  be_nested_str_weak(tag_vendor),
    /* K6   */  be_nested_str_weak(0x_X2504X_X3A_X3A),
    /* K7   */  be_nested_str_weak(0x_X2504X_X3A),
    /* K8   */  be_nested_str_weak(tag_sub),
    /* K9   */  be_nested_str_weak(_X25i_X20),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(_X3D_X20),
    /* K12  */  be_nested_str_weak(val),
    /* K13  */  be_nested_str_weak(int),
    /* K14  */  be_nested_str_weak(_X25i),
    /* K15  */  be_nested_str_weak(typ),
    /* K16  */  be_nested_str_weak(TLV),
    /* K17  */  be_nested_str_weak(U1),
    /* K18  */  be_nested_str_weak(U8),
    /* K19  */  be_nested_str_weak(U),
    /* K20  */  be_nested_str_weak(bool),
    /* K21  */  be_nested_str_weak(true),
    /* K22  */  be_nested_str_weak(false),
    /* K23  */  be_nested_str_weak(null),
    /* K24  */  be_nested_str_weak(real),
    /* K25  */  be_nested_str_weak(_X25g),
    /* K26  */  be_nested_str_weak(string),
    /* K27  */  be_nested_str_weak(_X22_X25s_X22),
    /* K28  */  be_nested_str_weak(int64),
    /* K29  */  be_nested_str_weak(tostring),
    /* K30  */  be_nested_str_weak(instance),
    /* K31  */  be_nested_str_weak(_X25s),
    /* K32  */  be_nested_str_weak(tohex),
    /* K33  */  be_nested_str_weak(_X20),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[167]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0xA802009C,  //  0001  EXBLK	0	#009F
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x200C0203,  //  0003  NE	R3	R1	R3
      0x780E0038,  //  0004  JMPF	R3	#003E
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x5411FFFE,  //  0006  LDINT	R4	-1
      0x1C0C0604,  //  0007  EQ	R3	R3	R4
      0x780E000A,  //  0008  JMPF	R3	#0014
      0x00080502,  //  0009  ADD	R2	R2	K2
      0x880C0103,  //  000A  GETMBR	R3	R0	K3
      0x4C100000,  //  000B  LDNIL	R4
      0x200C0604,  //  000C  NE	R3	R3	R4
      0x780E0004,  //  000D  JMPF	R3	#0013
      0x600C0018,  //  000E  GETGBL	R3	G24
      0x58100004,  //  000F  LDCONST	R4	K4
      0x88140103,  //  0010  GETMBR	R5	R0	K3
      0x7C0C0400,  //  0011  CALL	R3	2
      0x00080403,  //  0012  ADD	R2	R2	R3
      0x70020023,  //  0013  JMP		#0038
      0x880C0105,  //  0014  GETMBR	R3	R0	K5
      0x4C100000,  //  0015  LDNIL	R4
      0x200C0604,  //  0016  NE	R3	R3	R4
      0x780E0004,  //  0017  JMPF	R3	#001D
      0x600C0018,  //  0018  GETGBL	R3	G24
      0x58100006,  //  0019  LDCONST	R4	K6
      0x88140105,  //  001A  GETMBR	R5	R0	K5
      0x7C0C0400,  //  001B  CALL	R3	2
      0x00080403,  //  001C  ADD	R2	R2	R3
      0x880C0101,  //  001D  GETMBR	R3	R0	K1
      0x4C100000,  //  001E  LDNIL	R4
      0x200C0604,  //  001F  NE	R3	R3	R4
      0x780E0004,  //  0020  JMPF	R3	#0026
      0x600C0018,  //  0021  GETGBL	R3	G24
      0x58100007,  //  0022  LDCONST	R4	K7
      0x88140101,  //  0023  GETMBR	R5	R0	K1
      0x7C0C0400,  //  0024  CALL	R3	2
      0x00080403,  //  0025  ADD	R2	R2	R3
      0x880C0103,  //  0026  GETMBR	R3	R0	K3
      0x4C100000,  //  0027  LDNIL	R4
      0x200C0604,  //  0028  NE	R3	R3	R4
      0x780E0004,  //  0029  JMPF	R3	#002F
      0x600C0018,  //  002A  GETGBL	R3	G24
      0x58100004,  //  002B  LDCONST	R4	K4
      0x88140103,  //  002C  GETMBR	R5	R0	K3
      0x7C0C0400,  //  002D  CALL	R3	2
      0x00080403,  //  002E  ADD	R2	R2	R3
      0x880C0108,  //  002F  GETMBR	R3	R0	K8
      0x4C100000,  //  0030  LDNIL	R4
      0x200C0604,  //  0031  NE	R3	R3	R4
      0x780E0004,  //  0032  JMPF	R3	#0038
      0x600C0018,  //  0033  GETGBL	R3	G24
      0x58100009,  //  0034  LDCONST	R4	K9
      0x88140108,  //  0035  GETMBR	R5	R0	K8
      0x7C0C0400,  //  0036  CALL	R3	2
      0x00080403,  //  0037  ADD	R2	R2	R3
      0x600C000C,  //  0038  GETGBL	R3	G12
      0x5C100400,  //  0039  MOVE	R4	R2
      0x7C0C0200,  //  003A  CALL	R3	1
      0x240C070A,  //  003B  GT	R3	R3	K10
      0x780E0000,  //  003C  JMPF	R3	#003E
      0x0008050B,  //  003D  ADD	R2	R2	K11
      0x600C0004,  //  003E  GETGBL	R3	G4
      0x8810010C,  //  003F  GETMBR	R4	R0	K12
      0x7C0C0200,  //  0040  CALL	R3	1
      0x1C0C070D,  //  0041  EQ	R3	R3	K13
      0x780E0010,  //  0042  JMPF	R3	#0054
      0x600C0018,  //  0043  GETGBL	R3	G24
      0x5810000E,  //  0044  LDCONST	R4	K14
      0x8814010C,  //  0045  GETMBR	R5	R0	K12
      0x7C0C0400,  //  0046  CALL	R3	2
      0x00080403,  //  0047  ADD	R2	R2	R3
      0x880C010F,  //  0048  GETMBR	R3	R0	K15
      0x88100110,  //  0049  GETMBR	R4	R0	K16
      0x88100911,  //  004A  GETMBR	R4	R4	K17
      0x280C0604,  //  004B  GE	R3	R3	R4
      0x780E0005,  //  004C  JMPF	R3	#0053
      0x880C010F,  //  004D  GETMBR	R3	R0	K15
      0x88100110,  //  004E  GETMBR	R4	R0	K16
      0x88100912,  //  004F  GETMBR	R4	R4	K18
      0x180C0604,  //  0050  LE	R3	R3	R4
      0x780E0000,  //  0051  JMPF	R3	#0053
      0x00080513,  //  0052  ADD	R2	R2	K19
      0x70020048,  //  0053  JMP		#009D
      0x600C0004,  //  0054  GETGBL	R3	G4
      0x8810010C,  //  0055  GETMBR	R4	R0	K12
      0x7C0C0200,  //  0056  CALL	R3	1
      0x1C0C0714,  //  0057  EQ	R3	R3	K20
      0x780E0006,  //  0058  JMPF	R3	#0060
      0x880C010C,  //  0059  GETMBR	R3	R0	K12
      0x780E0001,  //  005A  JMPF	R3	#005D
      0x580C0015,  //  005B  LDCONST	R3	K21
      0x70020000,  //  005C  JMP		#005E
      0x580C0016,  //  005D  LDCONST	R3	K22
      0x00080403,  //  005E  ADD	R2	R2	R3
      0x7002003C,  //  005F  JMP		#009D
      0x880C010C,  //  0060  GETMBR	R3	R0	K12
      0x4C100000,  //  0061  LDNIL	R4
      0x1C0C0604,  //  0062  EQ	R3	R3	R4
      0x780E0001,  //  0063  JMPF	R3	#0066
      0x00080517,  //  0064  ADD	R2	R2	K23
      0x70020036,  //  0065  JMP		#009D
      0x600C0004,  //  0066  GETGBL	R3	G4
      0x8810010C,  //  0067  GETMBR	R4	R0	K12
      0x7C0C0200,  //  0068  CALL	R3	1
      0x1C0C0718,  //  0069  EQ	R3	R3	K24
      0x780E0005,  //  006A  JMPF	R3	#0071
      0x600C0018,  //  006B  GETGBL	R3	G24
      0x58100019,  //  006C  LDCONST	R4	K25
      0x8814010C,  //  006D  GETMBR	R5	R0	K12
      0x7C0C0400,  //  006E  CALL	R3	2
      0x00080403,  //  006F  ADD	R2	R2	R3
      0x7002002B,  //  0070  JMP		#009D
      0x600C0004,  //  0071  GETGBL	R3	G4
      0x8810010C,  //  0072  GETMBR	R4	R0	K12
      0x7C0C0200,  //  0073  CALL	R3	1
      0x1C0C071A,  //  0074  EQ	R3	R3	K26
      0x780E0005,  //  0075  JMPF	R3	#007C
      0x600C0018,  //  0076  GETGBL	R3	G24
      0x5810001B,  //  0077  LDCONST	R4	K27
      0x8814010C,  //  0078  GETMBR	R5	R0	K12
      0x7C0C0400,  //  0079  CALL	R3	2
      0x00080403,  //  007A  ADD	R2	R2	R3
      0x70020020,  //  007B  JMP		#009D
      0x600C000F,  //  007C  GETGBL	R3	G15
      0x8810010C,  //  007D  GETMBR	R4	R0	K12
      0xB8163800,  //  007E  GETNGBL	R5	K28
      0x7C0C0400,  //  007F  CALL	R3	2
      0x780E000F,  //  0080  JMPF	R3	#0091
      0x880C010C,  //  0081  GETMBR	R3	R0	K12
      0x8C0C071D,  //  0082  GETMET	R3	R3	K29
      0x7C0C0200,  //  0083  CALL	R3	1
      0x00080403,  //  0084  ADD	R2	R2	R3
      0x880C010F,  //  0085  GETMBR	R3	R0	K15
      0x88100110,  //  0086  GETMBR	R4	R0	K16
      0x88100911,  //  0087  GETMBR	R4	R4	K17
      0x280C0604,  //  0088  GE	R3	R3	R4
      0x780E0005,  //  0089  JMPF	R3	#0090
      0x880C010F,  //  008A  GETMBR	R3	R0	K15
      0x88100110,  //  008B  GETMBR	R4	R0	K16
      0x88100912,  //  008C  GETMBR	R4	R4	K18
      0x180C0604,  //  008D  LE	R3	R3	R4
      0x780E0000,  //  008E  JMPF	R3	#0090
      0x00080513,  //  008F  ADD	R2	R2	K19
      0x7002000B,  //  0090  JMP		#009D
      0x600C0004,  //  0091  GETGBL	R3	G4
      0x8810010C,  //  0092  GETMBR	R4	R0	K12
      0x7C0C0200,  //  0093  CALL	R3	1
      0x1C0C071E,  //  0094  EQ	R3	R3	K30
      0x780E0006,  //  0095  JMPF	R3	#009D
      0x600C0018,  //  0096  GETGBL	R3	G24
      0x5810001F,  //  0097  LDCONST	R4	K31
      0x8814010C,  //  0098  GETMBR	R5	R0	K12
      0x8C140B20,  //  0099  GETMET	R5	R5	K32
      0x7C140200,  //  009A  CALL	R5	1
      0x7C0C0400,  //  009B  CALL	R3	2
      0x00080403,  //  009C  ADD	R2	R2	R3
      0xA8040001,  //  009D  EXBLK	1	1
      0x70020006,  //  009E  JMP		#00A6
      0xAC0C0002,  //  009F  CATCH	R3	0	2
      0x70020003,  //  00A0  JMP		#00A5
      0x00140721,  //  00A1  ADD	R5	R3	K33
      0x00140A04,  //  00A2  ADD	R5	R5	R4
      0x80040A00,  //  00A3  RET	1	R5
      0x70020000,  //  00A4  JMP		#00A6
      0xB0080000,  //  00A5  RAISE	2	R0	R0
      0x80040400,  //  00A6  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_str_val
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_to_str_val,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(val),
    /* K1   */  be_nested_str_weak(int),
    /* K2   */  be_nested_str_weak(typ),
    /* K3   */  be_nested_str_weak(TLV),
    /* K4   */  be_nested_str_weak(U1),
    /* K5   */  be_nested_str_weak(U8),
    /* K6   */  be_nested_str_weak(U),
    /* K7   */  be_nested_str_weak(bool),
    /* K8   */  be_nested_str_weak(true),
    /* K9   */  be_nested_str_weak(false),
    /* K10  */  be_nested_str_weak(null),
    /* K11  */  be_nested_str_weak(real),
    /* K12  */  be_nested_str_weak(string),
    /* K13  */  be_nested_str_weak(int64),
    /* K14  */  be_nested_str_weak(tostring),
    /* K15  */  be_nested_str_weak(instance),
    }),
    be_str_weak(to_str_val),
    &be_const_str_solidified,
    ( &(const binstruction[98]) {  /* code */
      0x60040004,  //  0000  GETGBL	R1	G4
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040301,  //  0003  EQ	R1	R1	K1
      0x78060014,  //  0004  JMPF	R1	#001A
      0x88040102,  //  0005  GETMBR	R1	R0	K2
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x88080504,  //  0007  GETMBR	R2	R2	K4
      0x28040202,  //  0008  GE	R1	R1	R2
      0x7806000A,  //  0009  JMPF	R1	#0015
      0x88040102,  //  000A  GETMBR	R1	R0	K2
      0x88080103,  //  000B  GETMBR	R2	R0	K3
      0x88080505,  //  000C  GETMBR	R2	R2	K5
      0x18040202,  //  000D  LE	R1	R1	R2
      0x78060005,  //  000E  JMPF	R1	#0015
      0x60040008,  //  000F  GETGBL	R1	G8
      0x88080100,  //  0010  GETMBR	R2	R0	K0
      0x7C040200,  //  0011  CALL	R1	1
      0x00040306,  //  0012  ADD	R1	R1	K6
      0x80040200,  //  0013  RET	1	R1
      0x70020003,  //  0014  JMP		#0019
      0x60040008,  //  0015  GETGBL	R1	G8
      0x88080100,  //  0016  GETMBR	R2	R0	K0
      0x7C040200,  //  0017  CALL	R1	1
      0x80040200,  //  0018  RET	1	R1
      0x70020046,  //  0019  JMP		#0061
      0x60040004,  //  001A  GETGBL	R1	G4
      0x88080100,  //  001B  GETMBR	R2	R0	K0
      0x7C040200,  //  001C  CALL	R1	1
      0x1C040307,  //  001D  EQ	R1	R1	K7
      0x78060006,  //  001E  JMPF	R1	#0026
      0x88040100,  //  001F  GETMBR	R1	R0	K0
      0x78060001,  //  0020  JMPF	R1	#0023
      0x58040008,  //  0021  LDCONST	R1	K8
      0x70020000,  //  0022  JMP		#0024
      0x58040009,  //  0023  LDCONST	R1	K9
      0x80040200,  //  0024  RET	1	R1
      0x7002003A,  //  0025  JMP		#0061
      0x88040100,  //  0026  GETMBR	R1	R0	K0
      0x4C080000,  //  0027  LDNIL	R2
      0x1C040202,  //  0028  EQ	R1	R1	R2
      0x78060001,  //  0029  JMPF	R1	#002C
      0x80061400,  //  002A  RET	1	K10
      0x70020034,  //  002B  JMP		#0061
      0x60040004,  //  002C  GETGBL	R1	G4
      0x88080100,  //  002D  GETMBR	R2	R0	K0
      0x7C040200,  //  002E  CALL	R1	1
      0x1C04030B,  //  002F  EQ	R1	R1	K11
      0x78060004,  //  0030  JMPF	R1	#0036
      0x60040008,  //  0031  GETGBL	R1	G8
      0x88080100,  //  0032  GETMBR	R2	R0	K0
      0x7C040200,  //  0033  CALL	R1	1
      0x80040200,  //  0034  RET	1	R1
      0x7002002A,  //  0035  JMP		#0061
      0x60040004,  //  0036  GETGBL	R1	G4
      0x88080100,  //  0037  GETMBR	R2	R0	K0
      0x7C040200,  //  0038  CALL	R1	1
      0x1C04030C,  //  0039  EQ	R1	R1	K12
      0x78060002,  //  003A  JMPF	R1	#003E
      0x88040100,  //  003B  GETMBR	R1	R0	K0
      0x80040200,  //  003C  RET	1	R1
      0x70020022,  //  003D  JMP		#0061
      0x6004000F,  //  003E  GETGBL	R1	G15
      0x88080100,  //  003F  GETMBR	R2	R0	K0
      0xB80E1A00,  //  0040  GETNGBL	R3	K13
      0x7C040400,  //  0041  CALL	R1	2
      0x78060014,  //  0042  JMPF	R1	#0058
      0x88040102,  //  0043  GETMBR	R1	R0	K2
      0x88080103,  //  0044  GETMBR	R2	R0	K3
      0x88080504,  //  0045  GETMBR	R2	R2	K4
      0x28040202,  //  0046  GE	R1	R1	R2
      0x7806000A,  //  0047  JMPF	R1	#0053
      0x88040102,  //  0048  GETMBR	R1	R0	K2
      0x88080103,  //  0049  GETMBR	R2	R0	K3
      0x88080505,  //  004A  GETMBR	R2	R2	K5
      0x18040202,  //  004B  LE	R1	R1	R2
      0x78060005,  //  004C  JMPF	R1	#0053
      0x88040100,  //  004D  GETMBR	R1	R0	K0
      0x8C04030E,  //  004E  GETMET	R1	R1	K14
      0x7C040200,  //  004F  CALL	R1	1
      0x00040306,  //  0050  ADD	R1	R1	K6
      0x80040200,  //  0051  RET	1	R1
      0x70020003,  //  0052  JMP		#0057
      0x88040100,  //  0053  GETMBR	R1	R0	K0
      0x8C04030E,  //  0054  GETMET	R1	R1	K14
      0x7C040200,  //  0055  CALL	R1	1
      0x80040200,  //  0056  RET	1	R1
      0x70020008,  //  0057  JMP		#0061
      0x60040004,  //  0058  GETGBL	R1	G4
      0x88080100,  //  0059  GETMBR	R2	R0	K0
      0x7C040200,  //  005A  CALL	R1	1
      0x1C04030F,  //  005B  EQ	R1	R1	K15
      0x78060003,  //  005C  JMPF	R1	#0061
      0x8C04010E,  //  005D  GETMET	R1	R0	K14
      0x500C0200,  //  005E  LDBOOL	R3	1	0
      0x7C040400,  //  005F  CALL	R1	2
      0x80040200,  //  0060  RET	1	R1
      0x80000000,  //  0061  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fulltag
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_set_fulltag,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tag_vendor),
    /* K1   */  be_nested_str_weak(tag_profile),
    /* K2   */  be_nested_str_weak(tag_number),
    /* K3   */  be_nested_str_weak(tag_sub),
    }),
    be_str_weak(set_fulltag),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60100009,  //  0000  GETGBL	R4	G9
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C100200,  //  0002  CALL	R4	1
      0x90020004,  //  0003  SETMBR	R0	K0	R4
      0x60100009,  //  0004  GETGBL	R4	G9
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C100200,  //  0006  CALL	R4	1
      0x90020204,  //  0007  SETMBR	R0	K1	R4
      0x60100009,  //  0008  GETGBL	R4	G9
      0x5C140600,  //  0009  MOVE	R5	R3
      0x7C100200,  //  000A  CALL	R4	1
      0x90020404,  //  000B  SETMBR	R0	K2	R4
      0x4C100000,  //  000C  LDNIL	R4
      0x90020604,  //  000D  SETMBR	R0	K3	R4
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_parse,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(typ),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(_len),
    /* K3   */  be_nested_str_weak(val),
    /* K4   */  be_nested_str_weak(int64),
    /* K5   */  be_nested_str_weak(frombytes),
    /* K6   */  be_nested_str_weak(BFALSE),
    /* K7   */  be_nested_str_weak(BTRUE),
    /* K8   */  be_nested_str_weak(U8),
    /* K9   */  be_nested_str_weak(I8),
    /* K10  */  be_nested_str_weak(geti),
    /* K11  */  be_nested_str_weak(get),
    /* K12  */  be_nested_str_weak(FLOAT),
    /* K13  */  be_nested_str_weak(getfloat),
    /* K14  */  be_const_int(1),
    /* K15  */  be_nested_str_weak(UTF8),
    /* K16  */  be_nested_str_weak(asstring),
    /* K17  */  be_nested_str_weak(NULL),
    /* K18  */  be_nested_str_weak(EOC),
    /* K19  */  be_nested_str_weak(log),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20unexpected_X20eoc),
    /* K21  */  be_const_int(3),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20unexpected_X20type_X3A_X20),
    /* K23  */  be_nested_str_weak(next_idx),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[101]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x88140902,  //  0002  GETMBR	R5	R4	K2
      0x94140A03,  //  0003  GETIDX	R5	R5	R3
      0x541A0007,  //  0004  LDINT	R6	8
      0x1C180A06,  //  0005  EQ	R6	R5	R6
      0x781A0008,  //  0006  JMPF	R6	#0010
      0xB81A0800,  //  0007  GETNGBL	R6	K4
      0x8C180D05,  //  0008  GETMET	R6	R6	K5
      0x5C200200,  //  0009  MOVE	R8	R1
      0x5C240400,  //  000A  MOVE	R9	R2
      0x7C180600,  //  000B  CALL	R6	3
      0x90020606,  //  000C  SETMBR	R0	K3	R6
      0x541A0007,  //  000D  LDINT	R6	8
      0x00080406,  //  000E  ADD	R2	R2	R6
      0x70020052,  //  000F  JMP		#0063
      0x88180906,  //  0010  GETMBR	R6	R4	K6
      0x1C180606,  //  0011  EQ	R6	R3	R6
      0x741A0002,  //  0012  JMPT	R6	#0016
      0x88180907,  //  0013  GETMBR	R6	R4	K7
      0x1C180606,  //  0014  EQ	R6	R3	R6
      0x781A0003,  //  0015  JMPF	R6	#001A
      0x88180907,  //  0016  GETMBR	R6	R4	K7
      0x1C180606,  //  0017  EQ	R6	R3	R6
      0x90020606,  //  0018  SETMBR	R0	K3	R6
      0x70020048,  //  0019  JMP		#0063
      0x88180908,  //  001A  GETMBR	R6	R4	K8
      0x14180606,  //  001B  LT	R6	R3	R6
      0x781A000E,  //  001C  JMPF	R6	#002C
      0x88180909,  //  001D  GETMBR	R6	R4	K9
      0x18180606,  //  001E  LE	R6	R3	R6
      0x781A0004,  //  001F  JMPF	R6	#0025
      0x8C18030A,  //  0020  GETMET	R6	R1	K10
      0x5C200400,  //  0021  MOVE	R8	R2
      0x5C240A00,  //  0022  MOVE	R9	R5
      0x7C180600,  //  0023  CALL	R6	3
      0x70020003,  //  0024  JMP		#0029
      0x8C18030B,  //  0025  GETMET	R6	R1	K11
      0x5C200400,  //  0026  MOVE	R8	R2
      0x5C240A00,  //  0027  MOVE	R9	R5
      0x7C180600,  //  0028  CALL	R6	3
      0x90020606,  //  0029  SETMBR	R0	K3	R6
      0x00080405,  //  002A  ADD	R2	R2	R5
      0x70020036,  //  002B  JMP		#0063
      0x8818090C,  //  002C  GETMBR	R6	R4	K12
      0x1C180606,  //  002D  EQ	R6	R3	R6
      0x781A0006,  //  002E  JMPF	R6	#0036
      0x8C18030D,  //  002F  GETMET	R6	R1	K13
      0x5C200400,  //  0030  MOVE	R8	R2
      0x7C180400,  //  0031  CALL	R6	2
      0x90020606,  //  0032  SETMBR	R0	K3	R6
      0x541A0003,  //  0033  LDINT	R6	4
      0x00080406,  //  0034  ADD	R2	R2	R6
      0x7002002C,  //  0035  JMP		#0063
      0x5419FFF7,  //  0036  LDINT	R6	-8
      0x28180A06,  //  0037  GE	R6	R5	R6
      0x781A0016,  //  0038  JMPF	R6	#0050
      0x5419FFFE,  //  0039  LDINT	R6	-1
      0x18180A06,  //  003A  LE	R6	R5	R6
      0x781A0013,  //  003B  JMPF	R6	#0050
      0x8C18030B,  //  003C  GETMET	R6	R1	K11
      0x5C200400,  //  003D  MOVE	R8	R2
      0x44240A00,  //  003E  NEG	R9	R5
      0x7C180600,  //  003F  CALL	R6	3
      0x441C0A00,  //  0040  NEG	R7	R5
      0x00080407,  //  0041  ADD	R2	R2	R7
      0x001C0406,  //  0042  ADD	R7	R2	R6
      0x041C0F0E,  //  0043  SUB	R7	R7	K14
      0x401C0407,  //  0044  CONNECT	R7	R2	R7
      0x941C0207,  //  0045  GETIDX	R7	R1	R7
      0x90020607,  //  0046  SETMBR	R0	K3	R7
      0x00080406,  //  0047  ADD	R2	R2	R6
      0x881C090F,  //  0048  GETMBR	R7	R4	K15
      0x181C0607,  //  0049  LE	R7	R3	R7
      0x781E0003,  //  004A  JMPF	R7	#004F
      0x881C0103,  //  004B  GETMBR	R7	R0	K3
      0x8C1C0F10,  //  004C  GETMET	R7	R7	K16
      0x7C1C0200,  //  004D  CALL	R7	1
      0x90020607,  //  004E  SETMBR	R0	K3	R7
      0x70020012,  //  004F  JMP		#0063
      0x88180911,  //  0050  GETMBR	R6	R4	K17
      0x1C180606,  //  0051  EQ	R6	R3	R6
      0x781A0000,  //  0052  JMPF	R6	#0054
      0x7002000E,  //  0053  JMP		#0063
      0x88180912,  //  0054  GETMBR	R6	R4	K18
      0x1C180606,  //  0055  EQ	R6	R3	R6
      0x781A0004,  //  0056  JMPF	R6	#005C
      0xB81A2600,  //  0057  GETNGBL	R6	K19
      0x581C0014,  //  0058  LDCONST	R7	K20
      0x58200015,  //  0059  LDCONST	R8	K21
      0x7C180400,  //  005A  CALL	R6	2
      0x70020006,  //  005B  JMP		#0063
      0xB81A2600,  //  005C  GETNGBL	R6	K19
      0x601C0008,  //  005D  GETGBL	R7	G8
      0x5C200600,  //  005E  MOVE	R8	R3
      0x7C1C0200,  //  005F  CALL	R7	1
      0x001E2C07,  //  0060  ADD	R7	K22	R7
      0x58200015,  //  0061  LDCONST	R8	K21
      0x7C180400,  //  0062  CALL	R6	2
      0x90022E02,  //  0063  SETMBR	R0	K23	R2
      0x80040400,  //  0064  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_commonprofile
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_set_commonprofile,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_fulltag),
    }),
    be_str_weak(set_commonprofile),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
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
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item__encode_tag,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(tag_number),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(tag_vendor),
    /* K3   */  be_nested_str_weak(add),
    /* K4   */  be_nested_str_weak(typ),
    /* K5   */  be_const_int(1),
    /* K6   */  be_const_int(2),
    /* K7   */  be_nested_str_weak(tag_profile),
    /* K8   */  be_nested_str_weak(tag_sub),
    }),
    be_str_weak(_encode_tag),
    &be_const_str_solidified,
    ( &(const binstruction[133]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x70020000,  //  0005  JMP		#0007
      0x58080001,  //  0006  LDCONST	R2	K1
      0x540EFFFF,  //  0007  LDINT	R3	65536
      0x280C0403,  //  0008  GE	R3	R2	R3
      0x740E0002,  //  0009  JMPT	R3	#000D
      0x140C0501,  //  000A  LT	R3	R2	K1
      0x740E0000,  //  000B  JMPT	R3	#000D
      0x500C0001,  //  000C  LDBOOL	R3	0	1
      0x500C0200,  //  000D  LDBOOL	R3	1	0
      0x58100001,  //  000E  LDCONST	R4	K1
      0x88140102,  //  000F  GETMBR	R5	R0	K2
      0x4C180000,  //  0010  LDNIL	R6
      0x20140A06,  //  0011  NE	R5	R5	R6
      0x78160026,  //  0012  JMPF	R5	#003A
      0x780E0012,  //  0013  JMPF	R3	#0027
      0x8C140303,  //  0014  GETMET	R5	R1	K3
      0x541E00DF,  //  0015  LDINT	R7	224
      0x88200104,  //  0016  GETMBR	R8	R0	K4
      0x001C0E08,  //  0017  ADD	R7	R7	R8
      0x58200005,  //  0018  LDCONST	R8	K5
      0x7C140600,  //  0019  CALL	R5	3
      0x8C140303,  //  001A  GETMET	R5	R1	K3
      0x881C0102,  //  001B  GETMBR	R7	R0	K2
      0x58200006,  //  001C  LDCONST	R8	K6
      0x7C140600,  //  001D  CALL	R5	3
      0x8C140303,  //  001E  GETMET	R5	R1	K3
      0x881C0107,  //  001F  GETMBR	R7	R0	K7
      0x58200006,  //  0020  LDCONST	R8	K6
      0x7C140600,  //  0021  CALL	R5	3
      0x8C140303,  //  0022  GETMET	R5	R1	K3
      0x881C0100,  //  0023  GETMBR	R7	R0	K0
      0x54220003,  //  0024  LDINT	R8	4
      0x7C140600,  //  0025  CALL	R5	3
      0x70020011,  //  0026  JMP		#0039
      0x8C140303,  //  0027  GETMET	R5	R1	K3
      0x541E00BF,  //  0028  LDINT	R7	192
      0x88200104,  //  0029  GETMBR	R8	R0	K4
      0x001C0E08,  //  002A  ADD	R7	R7	R8
      0x58200005,  //  002B  LDCONST	R8	K5
      0x7C140600,  //  002C  CALL	R5	3
      0x8C140303,  //  002D  GETMET	R5	R1	K3
      0x881C0102,  //  002E  GETMBR	R7	R0	K2
      0x58200006,  //  002F  LDCONST	R8	K6
      0x7C140600,  //  0030  CALL	R5	3
      0x8C140303,  //  0031  GETMET	R5	R1	K3
      0x881C0107,  //  0032  GETMBR	R7	R0	K7
      0x58200006,  //  0033  LDCONST	R8	K6
      0x7C140600,  //  0034  CALL	R5	3
      0x8C140303,  //  0035  GETMET	R5	R1	K3
      0x881C0100,  //  0036  GETMBR	R7	R0	K0
      0x58200006,  //  0037  LDCONST	R8	K6
      0x7C140600,  //  0038  CALL	R5	3
      0x70020049,  //  0039  JMP		#0084
      0x88140107,  //  003A  GETMBR	R5	R0	K7
      0x5419FFFE,  //  003B  LDINT	R6	-1
      0x1C140A06,  //  003C  EQ	R5	R5	R6
      0x78160016,  //  003D  JMPF	R5	#0055
      0x780E000A,  //  003E  JMPF	R3	#004A
      0x8C140303,  //  003F  GETMET	R5	R1	K3
      0x541E005F,  //  0040  LDINT	R7	96
      0x88200104,  //  0041  GETMBR	R8	R0	K4
      0x001C0E08,  //  0042  ADD	R7	R7	R8
      0x58200005,  //  0043  LDCONST	R8	K5
      0x7C140600,  //  0044  CALL	R5	3
      0x8C140303,  //  0045  GETMET	R5	R1	K3
      0x881C0100,  //  0046  GETMBR	R7	R0	K0
      0x54220003,  //  0047  LDINT	R8	4
      0x7C140600,  //  0048  CALL	R5	3
      0x70020009,  //  0049  JMP		#0054
      0x8C140303,  //  004A  GETMET	R5	R1	K3
      0x541E003F,  //  004B  LDINT	R7	64
      0x88200104,  //  004C  GETMBR	R8	R0	K4
      0x001C0E08,  //  004D  ADD	R7	R7	R8
      0x58200005,  //  004E  LDCONST	R8	K5
      0x7C140600,  //  004F  CALL	R5	3
      0x8C140303,  //  0050  GETMET	R5	R1	K3
      0x881C0100,  //  0051  GETMBR	R7	R0	K0
      0x58200006,  //  0052  LDCONST	R8	K6
      0x7C140600,  //  0053  CALL	R5	3
      0x7002002E,  //  0054  JMP		#0084
      0x88140107,  //  0055  GETMBR	R5	R0	K7
      0x4C180000,  //  0056  LDNIL	R6
      0x20140A06,  //  0057  NE	R5	R5	R6
      0x78160016,  //  0058  JMPF	R5	#0070
      0x780E000A,  //  0059  JMPF	R3	#0065
      0x8C140303,  //  005A  GETMET	R5	R1	K3
      0x541E009F,  //  005B  LDINT	R7	160
      0x88200104,  //  005C  GETMBR	R8	R0	K4
      0x001C0E08,  //  005D  ADD	R7	R7	R8
      0x58200005,  //  005E  LDCONST	R8	K5
      0x7C140600,  //  005F  CALL	R5	3
      0x8C140303,  //  0060  GETMET	R5	R1	K3
      0x881C0100,  //  0061  GETMBR	R7	R0	K0
      0x54220003,  //  0062  LDINT	R8	4
      0x7C140600,  //  0063  CALL	R5	3
      0x70020009,  //  0064  JMP		#006F
      0x8C140303,  //  0065  GETMET	R5	R1	K3
      0x541E007F,  //  0066  LDINT	R7	128
      0x88200104,  //  0067  GETMBR	R8	R0	K4
      0x001C0E08,  //  0068  ADD	R7	R7	R8
      0x58200005,  //  0069  LDCONST	R8	K5
      0x7C140600,  //  006A  CALL	R5	3
      0x8C140303,  //  006B  GETMET	R5	R1	K3
      0x881C0100,  //  006C  GETMBR	R7	R0	K0
      0x58200006,  //  006D  LDCONST	R8	K6
      0x7C140600,  //  006E  CALL	R5	3
      0x70020013,  //  006F  JMP		#0084
      0x88140108,  //  0070  GETMBR	R5	R0	K8
      0x4C180000,  //  0071  LDNIL	R6
      0x20140A06,  //  0072  NE	R5	R5	R6
      0x7816000A,  //  0073  JMPF	R5	#007F
      0x8C140303,  //  0074  GETMET	R5	R1	K3
      0x541E001F,  //  0075  LDINT	R7	32
      0x88200104,  //  0076  GETMBR	R8	R0	K4
      0x001C0E08,  //  0077  ADD	R7	R7	R8
      0x58200005,  //  0078  LDCONST	R8	K5
      0x7C140600,  //  0079  CALL	R5	3
      0x8C140303,  //  007A  GETMET	R5	R1	K3
      0x881C0108,  //  007B  GETMBR	R7	R0	K8
      0x58200005,  //  007C  LDCONST	R8	K5
      0x7C140600,  //  007D  CALL	R5	3
      0x70020004,  //  007E  JMP		#0084
      0x8C140303,  //  007F  GETMET	R5	R1	K3
      0x881C0104,  //  0080  GETMBR	R7	R0	K4
      0x001E0207,  //  0081  ADD	R7	K1	R7
      0x58200005,  //  0082  LDCONST	R8	K5
      0x7C140600,  //  0083  CALL	R5	3
      0x80000000,  //  0084  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_sort,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_TLV_item),
    /* K1   */  be_const_int(1),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(_cmp_gt),
    /* K4   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(sort),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x040C0701,  //  0005  SUB	R3	R3	K1
      0x400E0203,  //  0006  CONNECT	R3	K1	R3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020013,  //  0008  EXBLK	0	#001D
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x94100003,  //  000B  GETIDX	R4	R0	R3
      0x5C140600,  //  000C  MOVE	R5	R3
      0x24180B02,  //  000D  GT	R6	R5	K2
      0x781A000B,  //  000E  JMPF	R6	#001B
      0x04180B01,  //  000F  SUB	R6	R5	K1
      0x94180006,  //  0010  GETIDX	R6	R0	R6
      0x8C180D03,  //  0011  GETMET	R6	R6	K3
      0x5C200800,  //  0012  MOVE	R8	R4
      0x7C180400,  //  0013  CALL	R6	2
      0x24180D02,  //  0014  GT	R6	R6	K2
      0x781A0004,  //  0015  JMPF	R6	#001B
      0x04180B01,  //  0016  SUB	R6	R5	K1
      0x94180006,  //  0017  GETIDX	R6	R0	R6
      0x98000A06,  //  0018  SETIDX	R0	R5	R6
      0x04140B01,  //  0019  SUB	R5	R5	K1
      0x7001FFF1,  //  001A  JMP		#000D
      0x98000A04,  //  001B  SETIDX	R0	R5	R4
      0x7001FFEB,  //  001C  JMP		#0009
      0x58080004,  //  001D  LDCONST	R2	K4
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
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(parent),
    }),
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
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item__cmp_gt,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(tag_vendor),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(tag_profile),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(tag_number),
    /* K5   */  be_nested_str_weak(tag_sub),
    }),
    be_str_weak(_cmp_gt),
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x20080403,  //  0002  NE	R2	R2	R3
      0x780A0012,  //  0003  JMPF	R2	#0017
      0x88080300,  //  0004  GETMBR	R2	R1	K0
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C080403,  //  0006  EQ	R2	R2	R3
      0x780A0000,  //  0007  JMPF	R2	#0009
      0x80060200,  //  0008  RET	1	K1
      0x88080100,  //  0009  GETMBR	R2	R0	K0
      0x880C0300,  //  000A  GETMBR	R3	R1	K0
      0x24080403,  //  000B  GT	R2	R2	R3
      0x780A0000,  //  000C  JMPF	R2	#000E
      0x80060200,  //  000D  RET	1	K1
      0x88080100,  //  000E  GETMBR	R2	R0	K0
      0x880C0300,  //  000F  GETMBR	R3	R1	K0
      0x1C080403,  //  0010  EQ	R2	R2	R3
      0x780A0004,  //  0011  JMPF	R2	#0017
      0x88080102,  //  0012  GETMBR	R2	R0	K2
      0x880C0302,  //  0013  GETMBR	R3	R1	K2
      0x24080403,  //  0014  GT	R2	R2	R3
      0x780A0000,  //  0015  JMPF	R2	#0017
      0x80060200,  //  0016  RET	1	K1
      0x88080102,  //  0017  GETMBR	R2	R0	K2
      0x540DFFFE,  //  0018  LDINT	R3	-1
      0x1C080403,  //  0019  EQ	R2	R2	R3
      0x780A0005,  //  001A  JMPF	R2	#0021
      0x88080302,  //  001B  GETMBR	R2	R1	K2
      0x4C0C0000,  //  001C  LDNIL	R3
      0x1C080403,  //  001D  EQ	R2	R2	R3
      0x780A0000,  //  001E  JMPF	R2	#0020
      0x80060200,  //  001F  RET	1	K1
      0x70020008,  //  0020  JMP		#002A
      0x88080102,  //  0021  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0022  LDNIL	R3
      0x1C080403,  //  0023  EQ	R2	R2	R3
      0x780A0004,  //  0024  JMPF	R2	#002A
      0x88080302,  //  0025  GETMBR	R2	R1	K2
      0x540DFFFE,  //  0026  LDINT	R3	-1
      0x1C080403,  //  0027  EQ	R2	R2	R3
      0x780A0000,  //  0028  JMPF	R2	#002A
      0x80060600,  //  0029  RET	1	K3
      0x88080104,  //  002A  GETMBR	R2	R0	K4
      0x4C0C0000,  //  002B  LDNIL	R3
      0x20080403,  //  002C  NE	R2	R2	R3
      0x780A000A,  //  002D  JMPF	R2	#0039
      0x88080304,  //  002E  GETMBR	R2	R1	K4
      0x4C0C0000,  //  002F  LDNIL	R3
      0x1C080403,  //  0030  EQ	R2	R2	R3
      0x780A0000,  //  0031  JMPF	R2	#0033
      0x80060200,  //  0032  RET	1	K1
      0x88080104,  //  0033  GETMBR	R2	R0	K4
      0x880C0304,  //  0034  GETMBR	R3	R1	K4
      0x24080403,  //  0035  GT	R2	R2	R3
      0x780A0000,  //  0036  JMPF	R2	#0038
      0x80060200,  //  0037  RET	1	K1
      0x80060600,  //  0038  RET	1	K3
      0x88080105,  //  0039  GETMBR	R2	R0	K5
      0x4C0C0000,  //  003A  LDNIL	R3
      0x20080403,  //  003B  NE	R2	R2	R3
      0x780A0009,  //  003C  JMPF	R2	#0047
      0x88080305,  //  003D  GETMBR	R2	R1	K5
      0x4C0C0000,  //  003E  LDNIL	R3
      0x1C080403,  //  003F  EQ	R2	R2	R3
      0x780A0000,  //  0040  JMPF	R2	#0042
      0x80060200,  //  0041  RET	1	K1
      0x88080105,  //  0042  GETMBR	R2	R0	K5
      0x880C0305,  //  0043  GETMBR	R3	R1	K5
      0x24080403,  //  0044  GT	R2	R2	R3
      0x780A0000,  //  0045  JMPF	R2	#0047
      0x80060200,  //  0046  RET	1	K1
      0x80060600,  //  0047  RET	1	K3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tlv2raw
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_tlv2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[44]) {     /* constants */
    /* K0   */  be_nested_str_weak(TLV),
    /* K1   */  be_nested_str_weak(typ),
    /* K2   */  be_nested_str_weak(RAW),
    /* K3   */  be_nested_str_weak(val),
    /* K4   */  be_nested_str_weak(I8),
    /* K5   */  be_nested_str_weak(U8),
    /* K6   */  be_nested_str_weak(int),
    /* K7   */  be_nested_str_weak(I4),
    /* K8   */  be_nested_str_weak(U4),
    /* K9   */  be_nested_str_weak(BFALSE),
    /* K10  */  be_nested_str_weak(BTRUE),
    /* K11  */  be_nested_str_weak(I2),
    /* K12  */  be_nested_str_weak(I1),
    /* K13  */  be_nested_str_weak(U2),
    /* K14  */  be_const_int(0),
    /* K15  */  be_nested_str_weak(U1),
    /* K16  */  be_nested_str_weak(B1),
    /* K17  */  be_nested_str_weak(B8),
    /* K18  */  be_nested_str_weak(B2),
    /* K19  */  be_nested_str_weak(B4),
    /* K20  */  be_nested_str_weak(UTF1),
    /* K21  */  be_nested_str_weak(UTF8),
    /* K22  */  be_nested_str_weak(UTF2),
    /* K23  */  be_nested_str_weak(UTF4),
    /* K24  */  be_nested_str_weak(_encode_tag),
    /* K25  */  be_nested_str_weak(add),
    /* K26  */  be_const_int(1),
    /* K27  */  be_const_int(2),
    /* K28  */  be_nested_str_weak(copy),
    /* K29  */  be_nested_str_weak(resize),
    /* K30  */  be_nested_str_weak(int64),
    /* K31  */  be_nested_str_weak(tobytes),
    /* K32  */  be_nested_str_weak(fromu32),
    /* K33  */  be_nested_str_weak(FLOAT),
    /* K34  */  be_nested_str_weak(setfloat),
    /* K35  */  be_nested_str_weak(DOUBLE),
    /* K36  */  be_nested_str_weak(value_error),
    /* K37  */  be_nested_str_weak(Unsupported_X20type_X20TLV_X2EDOUBLE),
    /* K38  */  be_nested_str_weak(string_X20too_X20big),
    /* K39  */  be_nested_str_weak(fromstring),
    /* K40  */  be_nested_str_weak(frostring),
    /* K41  */  be_nested_str_weak(bytes_X20too_X20big),
    /* K42  */  be_nested_str_weak(NULL),
    /* K43  */  be_nested_str_weak(unsupported_X20type_X20),
    }),
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[404]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x780E0002,  //  0003  JMPF	R3	#0007
      0x600C0015,  //  0004  GETGBL	R3	G21
      0x7C0C0000,  //  0005  CALL	R3	0
      0x5C040600,  //  0006  MOVE	R1	R3
      0x880C0101,  //  0007  GETMBR	R3	R0	K1
      0x88100502,  //  0008  GETMBR	R4	R2	K2
      0x1C0C0604,  //  0009  EQ	R3	R3	R4
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x880C0103,  //  000B  GETMBR	R3	R0	K3
      0x400C0203,  //  000C  CONNECT	R3	R1	R3
      0x80040200,  //  000D  RET	1	R1
      0x880C0101,  //  000E  GETMBR	R3	R0	K1
      0x88100504,  //  000F  GETMBR	R4	R2	K4
      0x1C0C0604,  //  0010  EQ	R3	R3	R4
      0x740E0003,  //  0011  JMPT	R3	#0016
      0x880C0101,  //  0012  GETMBR	R3	R0	K1
      0x88100505,  //  0013  GETMBR	R4	R2	K5
      0x1C0C0604,  //  0014  EQ	R3	R3	R4
      0x780E000D,  //  0015  JMPF	R3	#0024
      0x600C0004,  //  0016  GETGBL	R3	G4
      0x88100103,  //  0017  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0018  CALL	R3	1
      0x1C0C0706,  //  0019  EQ	R3	R3	K6
      0x780E0008,  //  001A  JMPF	R3	#0024
      0x880C0101,  //  001B  GETMBR	R3	R0	K1
      0x88100504,  //  001C  GETMBR	R4	R2	K4
      0x1C0C0604,  //  001D  EQ	R3	R3	R4
      0x780E0002,  //  001E  JMPF	R3	#0022
      0x880C0507,  //  001F  GETMBR	R3	R2	K7
      0x90020203,  //  0020  SETMBR	R0	K1	R3
      0x70020001,  //  0021  JMP		#0024
      0x880C0508,  //  0022  GETMBR	R3	R2	K8
      0x90020203,  //  0023  SETMBR	R0	K1	R3
      0x880C0101,  //  0024  GETMBR	R3	R0	K1
      0x88100509,  //  0025  GETMBR	R4	R2	K9
      0x1C0C0604,  //  0026  EQ	R3	R3	R4
      0x740E0003,  //  0027  JMPT	R3	#002C
      0x880C0101,  //  0028  GETMBR	R3	R0	K1
      0x8810050A,  //  0029  GETMBR	R4	R2	K10
      0x1C0C0604,  //  002A  EQ	R3	R3	R4
      0x780E0008,  //  002B  JMPF	R3	#0035
      0x600C0017,  //  002C  GETGBL	R3	G23
      0x88100103,  //  002D  GETMBR	R4	R0	K3
      0x7C0C0200,  //  002E  CALL	R3	1
      0x780E0001,  //  002F  JMPF	R3	#0032
      0x880C050A,  //  0030  GETMBR	R3	R2	K10
      0x70020000,  //  0031  JMP		#0033
      0x880C0509,  //  0032  GETMBR	R3	R2	K9
      0x90020203,  //  0033  SETMBR	R0	K1	R3
      0x70020070,  //  0034  JMP		#00A6
      0x880C0101,  //  0035  GETMBR	R3	R0	K1
      0x8810050B,  //  0036  GETMBR	R4	R2	K11
      0x280C0604,  //  0037  GE	R3	R3	R4
      0x780E0018,  //  0038  JMPF	R3	#0052
      0x880C0101,  //  0039  GETMBR	R3	R0	K1
      0x88100507,  //  003A  GETMBR	R4	R2	K7
      0x180C0604,  //  003B  LE	R3	R3	R4
      0x780E0014,  //  003C  JMPF	R3	#0052
      0x600C0009,  //  003D  GETGBL	R3	G9
      0x88100103,  //  003E  GETMBR	R4	R0	K3
      0x7C0C0200,  //  003F  CALL	R3	1
      0x5412007E,  //  0040  LDINT	R4	127
      0x18100604,  //  0041  LE	R4	R3	R4
      0x78120005,  //  0042  JMPF	R4	#0049
      0x5411FF7F,  //  0043  LDINT	R4	-128
      0x28100604,  //  0044  GE	R4	R3	R4
      0x78120002,  //  0045  JMPF	R4	#0049
      0x8810050C,  //  0046  GETMBR	R4	R2	K12
      0x90020204,  //  0047  SETMBR	R0	K1	R4
      0x70020007,  //  0048  JMP		#0051
      0x54127FFE,  //  0049  LDINT	R4	32767
      0x18100604,  //  004A  LE	R4	R3	R4
      0x78120004,  //  004B  JMPF	R4	#0051
      0x54117FFF,  //  004C  LDINT	R4	-32768
      0x28100604,  //  004D  GE	R4	R3	R4
      0x78120001,  //  004E  JMPF	R4	#0051
      0x8810050B,  //  004F  GETMBR	R4	R2	K11
      0x90020204,  //  0050  SETMBR	R0	K1	R4
      0x70020053,  //  0051  JMP		#00A6
      0x880C0101,  //  0052  GETMBR	R3	R0	K1
      0x8810050D,  //  0053  GETMBR	R4	R2	K13
      0x280C0604,  //  0054  GE	R3	R3	R4
      0x780E0016,  //  0055  JMPF	R3	#006D
      0x880C0101,  //  0056  GETMBR	R3	R0	K1
      0x88100508,  //  0057  GETMBR	R4	R2	K8
      0x180C0604,  //  0058  LE	R3	R3	R4
      0x780E0012,  //  0059  JMPF	R3	#006D
      0x600C0009,  //  005A  GETGBL	R3	G9
      0x88100103,  //  005B  GETMBR	R4	R0	K3
      0x7C0C0200,  //  005C  CALL	R3	1
      0x541200FE,  //  005D  LDINT	R4	255
      0x18100604,  //  005E  LE	R4	R3	R4
      0x78120004,  //  005F  JMPF	R4	#0065
      0x2810070E,  //  0060  GE	R4	R3	K14
      0x78120002,  //  0061  JMPF	R4	#0065
      0x8810050F,  //  0062  GETMBR	R4	R2	K15
      0x90020204,  //  0063  SETMBR	R0	K1	R4
      0x70020006,  //  0064  JMP		#006C
      0x5412FFFE,  //  0065  LDINT	R4	65535
      0x18100604,  //  0066  LE	R4	R3	R4
      0x78120003,  //  0067  JMPF	R4	#006C
      0x2810070E,  //  0068  GE	R4	R3	K14
      0x78120001,  //  0069  JMPF	R4	#006C
      0x8810050D,  //  006A  GETMBR	R4	R2	K13
      0x90020204,  //  006B  SETMBR	R0	K1	R4
      0x70020038,  //  006C  JMP		#00A6
      0x880C0101,  //  006D  GETMBR	R3	R0	K1
      0x88100510,  //  006E  GETMBR	R4	R2	K16
      0x280C0604,  //  006F  GE	R3	R3	R4
      0x780E0018,  //  0070  JMPF	R3	#008A
      0x880C0101,  //  0071  GETMBR	R3	R0	K1
      0x88100511,  //  0072  GETMBR	R4	R2	K17
      0x180C0604,  //  0073  LE	R3	R3	R4
      0x780E0014,  //  0074  JMPF	R3	#008A
      0x600C000C,  //  0075  GETGBL	R3	G12
      0x88100103,  //  0076  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0077  CALL	R3	1
      0x541200FE,  //  0078  LDINT	R4	255
      0x180C0604,  //  0079  LE	R3	R3	R4
      0x780E0002,  //  007A  JMPF	R3	#007E
      0x880C0510,  //  007B  GETMBR	R3	R2	K16
      0x90020203,  //  007C  SETMBR	R0	K1	R3
      0x7002000A,  //  007D  JMP		#0089
      0x600C000C,  //  007E  GETGBL	R3	G12
      0x88100103,  //  007F  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0080  CALL	R3	1
      0x5412FFFE,  //  0081  LDINT	R4	65535
      0x180C0604,  //  0082  LE	R3	R3	R4
      0x780E0002,  //  0083  JMPF	R3	#0087
      0x880C0512,  //  0084  GETMBR	R3	R2	K18
      0x90020203,  //  0085  SETMBR	R0	K1	R3
      0x70020001,  //  0086  JMP		#0089
      0x880C0513,  //  0087  GETMBR	R3	R2	K19
      0x90020203,  //  0088  SETMBR	R0	K1	R3
      0x7002001B,  //  0089  JMP		#00A6
      0x880C0101,  //  008A  GETMBR	R3	R0	K1
      0x88100514,  //  008B  GETMBR	R4	R2	K20
      0x280C0604,  //  008C  GE	R3	R3	R4
      0x780E0017,  //  008D  JMPF	R3	#00A6
      0x880C0101,  //  008E  GETMBR	R3	R0	K1
      0x88100515,  //  008F  GETMBR	R4	R2	K21
      0x180C0604,  //  0090  LE	R3	R3	R4
      0x780E0013,  //  0091  JMPF	R3	#00A6
      0x600C000C,  //  0092  GETGBL	R3	G12
      0x88100103,  //  0093  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0094  CALL	R3	1
      0x541200FE,  //  0095  LDINT	R4	255
      0x180C0604,  //  0096  LE	R3	R3	R4
      0x780E0002,  //  0097  JMPF	R3	#009B
      0x880C0514,  //  0098  GETMBR	R3	R2	K20
      0x90020203,  //  0099  SETMBR	R0	K1	R3
      0x7002000A,  //  009A  JMP		#00A6
      0x600C000C,  //  009B  GETGBL	R3	G12
      0x88100103,  //  009C  GETMBR	R4	R0	K3
      0x7C0C0200,  //  009D  CALL	R3	1
      0x5412FFFE,  //  009E  LDINT	R4	65535
      0x180C0604,  //  009F  LE	R3	R3	R4
      0x780E0002,  //  00A0  JMPF	R3	#00A4
      0x880C0516,  //  00A1  GETMBR	R3	R2	K22
      0x90020203,  //  00A2  SETMBR	R0	K1	R3
      0x70020001,  //  00A3  JMP		#00A6
      0x880C0517,  //  00A4  GETMBR	R3	R2	K23
      0x90020203,  //  00A5  SETMBR	R0	K1	R3
      0x8C0C0118,  //  00A6  GETMET	R3	R0	K24
      0x5C140200,  //  00A7  MOVE	R5	R1
      0x7C0C0400,  //  00A8  CALL	R3	2
      0x880C0101,  //  00A9  GETMBR	R3	R0	K1
      0x8810050C,  //  00AA  GETMBR	R4	R2	K12
      0x1C0C0604,  //  00AB  EQ	R3	R3	R4
      0x740E0003,  //  00AC  JMPT	R3	#00B1
      0x880C0101,  //  00AD  GETMBR	R3	R0	K1
      0x8810050F,  //  00AE  GETMBR	R4	R2	K15
      0x1C0C0604,  //  00AF  EQ	R3	R3	R4
      0x780E0006,  //  00B0  JMPF	R3	#00B8
      0x8C0C0319,  //  00B1  GETMET	R3	R1	K25
      0x60140009,  //  00B2  GETGBL	R5	G9
      0x88180103,  //  00B3  GETMBR	R6	R0	K3
      0x7C140200,  //  00B4  CALL	R5	1
      0x5818001A,  //  00B5  LDCONST	R6	K26
      0x7C0C0600,  //  00B6  CALL	R3	3
      0x700200DA,  //  00B7  JMP		#0193
      0x880C0101,  //  00B8  GETMBR	R3	R0	K1
      0x8810050B,  //  00B9  GETMBR	R4	R2	K11
      0x1C0C0604,  //  00BA  EQ	R3	R3	R4
      0x740E0003,  //  00BB  JMPT	R3	#00C0
      0x880C0101,  //  00BC  GETMBR	R3	R0	K1
      0x8810050D,  //  00BD  GETMBR	R4	R2	K13
      0x1C0C0604,  //  00BE  EQ	R3	R3	R4
      0x780E0006,  //  00BF  JMPF	R3	#00C7
      0x8C0C0319,  //  00C0  GETMET	R3	R1	K25
      0x60140009,  //  00C1  GETGBL	R5	G9
      0x88180103,  //  00C2  GETMBR	R6	R0	K3
      0x7C140200,  //  00C3  CALL	R5	1
      0x5818001B,  //  00C4  LDCONST	R6	K27
      0x7C0C0600,  //  00C5  CALL	R3	3
      0x700200CB,  //  00C6  JMP		#0193
      0x880C0101,  //  00C7  GETMBR	R3	R0	K1
      0x88100507,  //  00C8  GETMBR	R4	R2	K7
      0x1C0C0604,  //  00C9  EQ	R3	R3	R4
      0x740E0003,  //  00CA  JMPT	R3	#00CF
      0x880C0101,  //  00CB  GETMBR	R3	R0	K1
      0x88100508,  //  00CC  GETMBR	R4	R2	K8
      0x1C0C0604,  //  00CD  EQ	R3	R3	R4
      0x780E0006,  //  00CE  JMPF	R3	#00D6
      0x8C0C0319,  //  00CF  GETMET	R3	R1	K25
      0x60140009,  //  00D0  GETGBL	R5	G9
      0x88180103,  //  00D1  GETMBR	R6	R0	K3
      0x7C140200,  //  00D2  CALL	R5	1
      0x541A0003,  //  00D3  LDINT	R6	4
      0x7C0C0600,  //  00D4  CALL	R3	3
      0x700200BC,  //  00D5  JMP		#0193
      0x880C0101,  //  00D6  GETMBR	R3	R0	K1
      0x88100504,  //  00D7  GETMBR	R4	R2	K4
      0x1C0C0604,  //  00D8  EQ	R3	R3	R4
      0x740E0003,  //  00D9  JMPT	R3	#00DE
      0x880C0101,  //  00DA  GETMBR	R3	R0	K1
      0x88100505,  //  00DB  GETMBR	R4	R2	K5
      0x1C0C0604,  //  00DC  EQ	R3	R3	R4
      0x780E002D,  //  00DD  JMPF	R3	#010C
      0x880C0103,  //  00DE  GETMBR	R3	R0	K3
      0x6010000F,  //  00DF  GETGBL	R4	G15
      0x5C140600,  //  00E0  MOVE	R5	R3
      0x60180015,  //  00E1  GETGBL	R6	G21
      0x7C100400,  //  00E2  CALL	R4	2
      0x78120006,  //  00E3  JMPF	R4	#00EB
      0x8C10071C,  //  00E4  GETMET	R4	R3	K28
      0x7C100200,  //  00E5  CALL	R4	1
      0x8C10091D,  //  00E6  GETMET	R4	R4	K29
      0x541A0007,  //  00E7  LDINT	R6	8
      0x7C100400,  //  00E8  CALL	R4	2
      0x5C0C0800,  //  00E9  MOVE	R3	R4
      0x7002001E,  //  00EA  JMP		#010A
      0x6010000F,  //  00EB  GETGBL	R4	G15
      0x5C140600,  //  00EC  MOVE	R5	R3
      0xB81A3C00,  //  00ED  GETNGBL	R6	K30
      0x7C100400,  //  00EE  CALL	R4	2
      0x78120003,  //  00EF  JMPF	R4	#00F4
      0x8C10071F,  //  00F0  GETMET	R4	R3	K31
      0x7C100200,  //  00F1  CALL	R4	1
      0x5C0C0800,  //  00F2  MOVE	R3	R4
      0x70020015,  //  00F3  JMP		#010A
      0x88100101,  //  00F4  GETMBR	R4	R0	K1
      0x88140504,  //  00F5  GETMBR	R5	R2	K4
      0x1C100805,  //  00F6  EQ	R4	R4	R5
      0x78120008,  //  00F7  JMPF	R4	#0101
      0xB8123C00,  //  00F8  GETNGBL	R4	K30
      0x60140009,  //  00F9  GETGBL	R5	G9
      0x5C180600,  //  00FA  MOVE	R6	R3
      0x7C140200,  //  00FB  CALL	R5	1
      0x7C100200,  //  00FC  CALL	R4	1
      0x8C10091F,  //  00FD  GETMET	R4	R4	K31
      0x7C100200,  //  00FE  CALL	R4	1
      0x5C0C0800,  //  00FF  MOVE	R3	R4
      0x70020008,  //  0100  JMP		#010A
      0xB8123C00,  //  0101  GETNGBL	R4	K30
      0x8C100920,  //  0102  GETMET	R4	R4	K32
      0x60180009,  //  0103  GETGBL	R6	G9
      0x5C1C0600,  //  0104  MOVE	R7	R3
      0x7C180200,  //  0105  CALL	R6	1
      0x7C100400,  //  0106  CALL	R4	2
      0x8C10091F,  //  0107  GETMET	R4	R4	K31
      0x7C100200,  //  0108  CALL	R4	1
      0x5C0C0800,  //  0109  MOVE	R3	R4
      0x40100203,  //  010A  CONNECT	R4	R1	R3
      0x70020086,  //  010B  JMP		#0193
      0x880C0101,  //  010C  GETMBR	R3	R0	K1
      0x88100509,  //  010D  GETMBR	R4	R2	K9
      0x1C0C0604,  //  010E  EQ	R3	R3	R4
      0x740E0003,  //  010F  JMPT	R3	#0114
      0x880C0101,  //  0110  GETMBR	R3	R0	K1
      0x8810050A,  //  0111  GETMBR	R4	R2	K10
      0x1C0C0604,  //  0112  EQ	R3	R3	R4
      0x780E0000,  //  0113  JMPF	R3	#0115
      0x7002007D,  //  0114  JMP		#0193
      0x880C0101,  //  0115  GETMBR	R3	R0	K1
      0x88100521,  //  0116  GETMBR	R4	R2	K33
      0x1C0C0604,  //  0117  EQ	R3	R3	R4
      0x780E000D,  //  0118  JMPF	R3	#0127
      0x600C000C,  //  0119  GETGBL	R3	G12
      0x5C100200,  //  011A  MOVE	R4	R1
      0x7C0C0200,  //  011B  CALL	R3	1
      0x8C100319,  //  011C  GETMET	R4	R1	K25
      0x5818000E,  //  011D  LDCONST	R6	K14
      0x541E0003,  //  011E  LDINT	R7	4
      0x7C100600,  //  011F  CALL	R4	3
      0x8C100322,  //  0120  GETMET	R4	R1	K34
      0x5C180600,  //  0121  MOVE	R6	R3
      0x601C000A,  //  0122  GETGBL	R7	G10
      0x88200103,  //  0123  GETMBR	R8	R0	K3
      0x7C1C0200,  //  0124  CALL	R7	1
      0x7C100600,  //  0125  CALL	R4	3
      0x7002006B,  //  0126  JMP		#0193
      0x880C0101,  //  0127  GETMBR	R3	R0	K1
      0x88100523,  //  0128  GETMBR	R4	R2	K35
      0x1C0C0604,  //  0129  EQ	R3	R3	R4
      0x780E0001,  //  012A  JMPF	R3	#012D
      0xB0064925,  //  012B  RAISE	1	K36	K37
      0x70020065,  //  012C  JMP		#0193
      0x880C0101,  //  012D  GETMBR	R3	R0	K1
      0x88100514,  //  012E  GETMBR	R4	R2	K20
      0x1C0C0604,  //  012F  EQ	R3	R3	R4
      0x780E0015,  //  0130  JMPF	R3	#0147
      0x600C000C,  //  0131  GETGBL	R3	G12
      0x88100103,  //  0132  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0133  CALL	R3	1
      0x541200FE,  //  0134  LDINT	R4	255
      0x240C0604,  //  0135  GT	R3	R3	R4
      0x780E0000,  //  0136  JMPF	R3	#0138
      0xB0064926,  //  0137  RAISE	1	K36	K38
      0x8C0C0319,  //  0138  GETMET	R3	R1	K25
      0x6014000C,  //  0139  GETGBL	R5	G12
      0x88180103,  //  013A  GETMBR	R6	R0	K3
      0x7C140200,  //  013B  CALL	R5	1
      0x5818001A,  //  013C  LDCONST	R6	K26
      0x7C0C0600,  //  013D  CALL	R3	3
      0x600C0015,  //  013E  GETGBL	R3	G21
      0x7C0C0000,  //  013F  CALL	R3	0
      0x8C0C0727,  //  0140  GETMET	R3	R3	K39
      0x60140008,  //  0141  GETGBL	R5	G8
      0x88180103,  //  0142  GETMBR	R6	R0	K3
      0x7C140200,  //  0143  CALL	R5	1
      0x7C0C0400,  //  0144  CALL	R3	2
      0x400C0203,  //  0145  CONNECT	R3	R1	R3
      0x7002004B,  //  0146  JMP		#0193
      0x880C0101,  //  0147  GETMBR	R3	R0	K1
      0x88100516,  //  0148  GETMBR	R4	R2	K22
      0x1C0C0604,  //  0149  EQ	R3	R3	R4
      0x780E0015,  //  014A  JMPF	R3	#0161
      0x600C000C,  //  014B  GETGBL	R3	G12
      0x88100103,  //  014C  GETMBR	R4	R0	K3
      0x7C0C0200,  //  014D  CALL	R3	1
      0x5412FFFE,  //  014E  LDINT	R4	65535
      0x240C0604,  //  014F  GT	R3	R3	R4
      0x780E0000,  //  0150  JMPF	R3	#0152
      0xB0064926,  //  0151  RAISE	1	K36	K38
      0x8C0C0319,  //  0152  GETMET	R3	R1	K25
      0x6014000C,  //  0153  GETGBL	R5	G12
      0x88180103,  //  0154  GETMBR	R6	R0	K3
      0x7C140200,  //  0155  CALL	R5	1
      0x5818001B,  //  0156  LDCONST	R6	K27
      0x7C0C0600,  //  0157  CALL	R3	3
      0x600C0015,  //  0158  GETGBL	R3	G21
      0x7C0C0000,  //  0159  CALL	R3	0
      0x8C0C0728,  //  015A  GETMET	R3	R3	K40
      0x60140008,  //  015B  GETGBL	R5	G8
      0x88180103,  //  015C  GETMBR	R6	R0	K3
      0x7C140200,  //  015D  CALL	R5	1
      0x7C0C0400,  //  015E  CALL	R3	2
      0x400C0203,  //  015F  CONNECT	R3	R1	R3
      0x70020031,  //  0160  JMP		#0193
      0x880C0101,  //  0161  GETMBR	R3	R0	K1
      0x88100510,  //  0162  GETMBR	R4	R2	K16
      0x1C0C0604,  //  0163  EQ	R3	R3	R4
      0x780E000F,  //  0164  JMPF	R3	#0175
      0x600C000C,  //  0165  GETGBL	R3	G12
      0x88100103,  //  0166  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0167  CALL	R3	1
      0x541200FE,  //  0168  LDINT	R4	255
      0x240C0604,  //  0169  GT	R3	R3	R4
      0x780E0000,  //  016A  JMPF	R3	#016C
      0xB0064929,  //  016B  RAISE	1	K36	K41
      0x8C0C0319,  //  016C  GETMET	R3	R1	K25
      0x6014000C,  //  016D  GETGBL	R5	G12
      0x88180103,  //  016E  GETMBR	R6	R0	K3
      0x7C140200,  //  016F  CALL	R5	1
      0x5818001A,  //  0170  LDCONST	R6	K26
      0x7C0C0600,  //  0171  CALL	R3	3
      0x880C0103,  //  0172  GETMBR	R3	R0	K3
      0x400C0203,  //  0173  CONNECT	R3	R1	R3
      0x7002001D,  //  0174  JMP		#0193
      0x880C0101,  //  0175  GETMBR	R3	R0	K1
      0x88100512,  //  0176  GETMBR	R4	R2	K18
      0x1C0C0604,  //  0177  EQ	R3	R3	R4
      0x780E000F,  //  0178  JMPF	R3	#0189
      0x600C000C,  //  0179  GETGBL	R3	G12
      0x88100103,  //  017A  GETMBR	R4	R0	K3
      0x7C0C0200,  //  017B  CALL	R3	1
      0x5412FFFE,  //  017C  LDINT	R4	65535
      0x240C0604,  //  017D  GT	R3	R3	R4
      0x780E0000,  //  017E  JMPF	R3	#0180
      0xB0064929,  //  017F  RAISE	1	K36	K41
      0x8C0C0319,  //  0180  GETMET	R3	R1	K25
      0x6014000C,  //  0181  GETGBL	R5	G12
      0x88180103,  //  0182  GETMBR	R6	R0	K3
      0x7C140200,  //  0183  CALL	R5	1
      0x5818001B,  //  0184  LDCONST	R6	K27
      0x7C0C0600,  //  0185  CALL	R3	3
      0x880C0103,  //  0186  GETMBR	R3	R0	K3
      0x400C0203,  //  0187  CONNECT	R3	R1	R3
      0x70020009,  //  0188  JMP		#0193
      0x880C0101,  //  0189  GETMBR	R3	R0	K1
      0x8810052A,  //  018A  GETMBR	R4	R2	K42
      0x1C0C0604,  //  018B  EQ	R3	R3	R4
      0x780E0000,  //  018C  JMPF	R3	#018E
      0x70020004,  //  018D  JMP		#0193
      0x600C0008,  //  018E  GETGBL	R3	G8
      0x88100101,  //  018F  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0190  CALL	R3	1
      0x000E5603,  //  0191  ADD	R3	K43	R3
      0xB0064803,  //  0192  RAISE	1	K36	R3
      0x80040200,  //  0193  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _encode_tag_len
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item__encode_tag_len,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(tag_number),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(tag_vendor),
    /* K3   */  be_nested_str_weak(tag_profile),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(tag_sub),
    /* K6   */  be_const_int(2),
    /* K7   */  be_const_int(1),
    }),
    be_str_weak(_encode_tag_len),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x70020000,  //  0005  JMP		#0007
      0x58040001,  //  0006  LDCONST	R1	K1
      0x540AFFFF,  //  0007  LDINT	R2	65536
      0x28080202,  //  0008  GE	R2	R1	R2
      0x740A0002,  //  0009  JMPT	R2	#000D
      0x14080301,  //  000A  LT	R2	R1	K1
      0x740A0000,  //  000B  JMPT	R2	#000D
      0x50080001,  //  000C  LDBOOL	R2	0	1
      0x50080200,  //  000D  LDBOOL	R2	1	0
      0x580C0001,  //  000E  LDCONST	R3	K1
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
      0x80060800,  //  0022  RET	1	K4
      0x70020010,  //  0023  JMP		#0035
      0x88100103,  //  0024  GETMBR	R4	R0	K3
      0x4C140000,  //  0025  LDNIL	R5
      0x20100805,  //  0026  NE	R4	R4	R5
      0x78120005,  //  0027  JMPF	R4	#002E
      0x780A0002,  //  0028  JMPF	R2	#002C
      0x54120004,  //  0029  LDINT	R4	5
      0x80040800,  //  002A  RET	1	R4
      0x70020000,  //  002B  JMP		#002D
      0x80060800,  //  002C  RET	1	K4
      0x70020006,  //  002D  JMP		#0035
      0x88100105,  //  002E  GETMBR	R4	R0	K5
      0x4C140000,  //  002F  LDNIL	R5
      0x20100805,  //  0030  NE	R4	R4	R5
      0x78120001,  //  0031  JMPF	R4	#0034
      0x80060C00,  //  0032  RET	1	K6
      0x70020000,  //  0033  JMP		#0035
      0x80060E00,  //  0034  RET	1	K7
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_contextspecific
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_set_contextspecific,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_fulltag),
    /* K1   */  be_nested_str_weak(tag_sub),
    }),
    be_str_weak(set_contextspecific),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x60080009,  //  0002  GETGBL	R2	G9
      0x5C0C0200,  //  0003  MOVE	R3	R1
      0x7C080200,  //  0004  CALL	R2	1
      0x90020202,  //  0005  SETMBR	R0	K1	R2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_parent
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_set_parent,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(parent),
    }),
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
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_create_TLV,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_TLV_item),
    /* K1   */  be_nested_str_weak(typ),
    /* K2   */  be_nested_str_weak(val),
    }),
    be_str_weak(create_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x200C0203,  //  0002  NE	R3	R1	R3
      0x740E0002,  //  0003  JMPT	R3	#0007
      0x540E0013,  //  0004  LDINT	R3	20
      0x1C0C0003,  //  0005  EQ	R3	R0	R3
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x5C0C0400,  //  0007  MOVE	R3	R2
      0x7C0C0000,  //  0008  CALL	R3	0
      0x900E0200,  //  0009  SETMBR	R3	K1	R0
      0x900E0401,  //  000A  SETMBR	R3	K2	R1
      0x80040600,  //  000B  RET	1	R3
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_anonymoustag
********************************************************************/
extern const bclass be_class_Matter_TLV_item;
be_local_closure(class_Matter_TLV_item_set_anonymoustag,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_item, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(set_fulltag),
    }),
    be_str_weak(set_anonymoustag),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
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

extern const bclass be_class_Matter_TLV_list;

/********************************************************************
** Solidified function: findsubval
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_findsubval,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(findsub),
    /* K1   */  be_nested_str_weak(val),
    }),
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_tlv2raw,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(_encode_tag),
    /* K1   */  be_nested_str_weak(val),
    /* K2   */  be_nested_str_weak(is_struct),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(sort),
    /* K5   */  be_nested_str_weak(tlv2raw),
    /* K6   */  be_nested_str_weak(stop_iteration),
    /* K7   */  be_nested_str_weak(add),
    /* K8   */  be_nested_str_weak(TLV),
    /* K9   */  be_nested_str_weak(EOC),
    /* K10  */  be_const_int(1),
    }),
    be_str_weak(tlv2raw),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0002,  //  0002  JMPF	R2	#0006
      0x60080015,  //  0003  GETGBL	R2	G21
      0x7C080000,  //  0004  CALL	R2	0
      0x5C040400,  //  0005  MOVE	R1	R2
      0x8C080100,  //  0006  GETMET	R2	R0	K0
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C080400,  //  0008  CALL	R2	2
      0x88080101,  //  0009  GETMBR	R2	R0	K1
      0x880C0102,  //  000A  GETMBR	R3	R0	K2
      0x780E0005,  //  000B  JMPF	R3	#0012
      0x8C0C0503,  //  000C  GETMET	R3	R2	K3
      0x7C0C0200,  //  000D  CALL	R3	1
      0x5C080600,  //  000E  MOVE	R2	R3
      0x8C0C0104,  //  000F  GETMET	R3	R0	K4
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
      0x8C140905,  //  001F  GETMET	R5	R4	K5
      0x5C1C0200,  //  0020  MOVE	R7	R1
      0x7C140400,  //  0021  CALL	R5	2
      0x7001FFF2,  //  0022  JMP		#0016
      0x580C0006,  //  0023  LDCONST	R3	K6
      0xAC0C0200,  //  0024  CATCH	R3	1	0
      0xB0080000,  //  0025  RAISE	2	R0	R0
      0x8C0C0307,  //  0026  GETMET	R3	R1	K7
      0x88140108,  //  0027  GETMBR	R5	R0	K8
      0x88140B09,  //  0028  GETMBR	R5	R5	K9
      0x5818000A,  //  0029  LDCONST	R6	K10
      0x7C0C0600,  //  002A  CALL	R3	3
      0x80040200,  //  002B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_str_val
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_to_str_val,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(tostring),
    }),
    be_str_weak(to_str_val),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tostring_inner),
    /* K1   */  be_nested_str_weak(_X5B_X5B),
    /* K2   */  be_nested_str_weak(_X5D_X5D),
    }),
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
** Solidified function: size
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_size,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(val),
    }),
    be_str_weak(size),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setitem
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_setitem,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(val),
    }),
    be_str_weak(setitem),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_struct
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_add_struct,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(TLV),
    /* K1   */  be_nested_str_weak(Matter_TLV_struct),
    /* K2   */  be_nested_str_weak(tag_sub),
    /* K3   */  be_nested_str_weak(val),
    /* K4   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_struct),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x900A0401,  //  0004  SETMBR	R2	K2	R1
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x8C0C0704,  //  0006  GETMET	R3	R3	K4
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_add_list,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(TLV),
    /* K1   */  be_nested_str_weak(Matter_TLV_list),
    /* K2   */  be_nested_str_weak(tag_sub),
    /* K3   */  be_nested_str_weak(val),
    /* K4   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_list),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x900A0401,  //  0004  SETMBR	R2	K2	R1
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x8C0C0704,  //  0006  GETMET	R3	R3	K4
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_parse,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(TLV),
    /* K1   */  be_nested_str_weak(EOC),
    /* K2   */  be_nested_str_weak(parse),
    /* K3   */  be_nested_str_weak(next_idx),
    /* K4   */  be_nested_str_weak(val),
    /* K5   */  be_nested_str_weak(push),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x940C0202,  //  0000  GETIDX	R3	R1	R2
      0x88100100,  //  0001  GETMBR	R4	R0	K0
      0x88100901,  //  0002  GETMBR	R4	R4	K1
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x780E000B,  //  0004  JMPF	R3	#0011
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x8C0C0702,  //  0006  GETMET	R3	R3	K2
      0x5C140200,  //  0007  MOVE	R5	R1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x5C1C0000,  //  0009  MOVE	R7	R0
      0x7C0C0800,  //  000A  CALL	R3	4
      0x88080703,  //  000B  GETMBR	R2	R3	K3
      0x88100104,  //  000C  GETMBR	R4	R0	K4
      0x8C100905,  //  000D  GETMET	R4	R4	K5
      0x5C180600,  //  000E  MOVE	R6	R3
      0x7C100400,  //  000F  CALL	R4	2
      0x7001FFEE,  //  0010  JMP		#0000
      0x00080506,  //  0011  ADD	R2	R2	K6
      0x90020602,  //  0012  SETMBR	R0	K3	R2
      0x80040400,  //  0013  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(typ),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(LIST),
    /* K4   */  be_nested_str_weak(val),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x88080503,  //  0007  GETMBR	R2	R2	K3
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x60080012,  //  0009  GETGBL	R2	G18
      0x7C080000,  //  000A  CALL	R2	0
      0x90020802,  //  000B  SETMBR	R0	K4	R2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: item
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_item,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(val),
    }),
    be_str_weak(item),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x94080401,  //  0001  GETIDX	R2	R2	R1
      0x80040400,  //  0002  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: getsubval
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_getsubval,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(getsub),
    /* K1   */  be_nested_str_weak(val),
    }),
    be_str_weak(getsubval),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_getsub,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(findsub),
    /* K1   */  be_nested_str_weak(value_error),
    /* K2   */  be_nested_str_weak(sub_X20not_X20found),
    }),
    be_str_weak(getsub),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0xB0060302,  //  0006  RAISE	1	K1	K2
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_obj
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_add_obj,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(val),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(to_TLV),
    /* K3   */  be_nested_str_weak(tag_sub),
    }),
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
      0x880C0100,  //  0008  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0009  GETMET	R3	R3	K1
      0x5C140400,  //  000A  MOVE	R5	R2
      0x7C0C0400,  //  000B  CALL	R3	2
      0x70020006,  //  000C  JMP		#0014
      0x8C0C0502,  //  000D  GETMET	R3	R2	K2
      0x7C0C0200,  //  000E  CALL	R3	1
      0x900E0601,  //  000F  SETMBR	R3	K3	R1
      0x88100100,  //  0010  GETMBR	R4	R0	K0
      0x8C100901,  //  0011  GETMET	R4	R4	K1
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_add_TLV,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(NULL),
    /* K3   */  be_nested_str_weak(Matter_TLV_item),
    /* K4   */  be_nested_str_weak(tag_sub),
    /* K5   */  be_nested_str_weak(typ),
    /* K6   */  be_nested_str_weak(val),
    /* K7   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_TLV),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x20100604,  //  0001  NE	R4	R3	R4
      0x74120004,  //  0002  JMPT	R4	#0008
      0xB8120000,  //  0003  GETNGBL	R4	K0
      0x88100901,  //  0004  GETMBR	R4	R4	K1
      0x88100902,  //  0005  GETMBR	R4	R4	K2
      0x1C100404,  //  0006  EQ	R4	R2	R4
      0x7812000A,  //  0007  JMPF	R4	#0013
      0x88100101,  //  0008  GETMBR	R4	R0	K1
      0x8C100903,  //  0009  GETMET	R4	R4	K3
      0x5C180000,  //  000A  MOVE	R6	R0
      0x7C100400,  //  000B  CALL	R4	2
      0x90120801,  //  000C  SETMBR	R4	K4	R1
      0x90120A02,  //  000D  SETMBR	R4	K5	R2
      0x90120C03,  //  000E  SETMBR	R4	K6	R3
      0x88140106,  //  000F  GETMBR	R5	R0	K6
      0x8C140B07,  //  0010  GETMET	R5	R5	K7
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x7C140400,  //  0012  CALL	R5	2
      0x80040000,  //  0013  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_array
********************************************************************/
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_add_array,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(TLV),
    /* K1   */  be_nested_str_weak(Matter_TLV_array),
    /* K2   */  be_nested_str_weak(tag_sub),
    /* K3   */  be_nested_str_weak(val),
    /* K4   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_array),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0x900A0401,  //  0004  SETMBR	R2	K2	R1
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x8C0C0704,  //  0006  GETMET	R3	R3	K4
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_findsub,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(val),
    /* K1   */  be_nested_str_weak(tag_sub),
    /* K2   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(findsub),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x600C0010,  //  0000  GETGBL	R3	G16
      0x88100100,  //  0001  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0002  CALL	R3	1
      0xA8020007,  //  0003  EXBLK	0	#000C
      0x5C100600,  //  0004  MOVE	R4	R3
      0x7C100000,  //  0005  CALL	R4	0
      0x88140901,  //  0006  GETMBR	R5	R4	K1
      0x1C140A01,  //  0007  EQ	R5	R5	R1
      0x78160001,  //  0008  JMPF	R5	#000B
      0xA8040001,  //  0009  EXBLK	1	1
      0x80040800,  //  000A  RET	1	R4
      0x7001FFF7,  //  000B  JMP		#0004
      0x580C0002,  //  000C  LDCONST	R3	K2
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_tostring_inner,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(tag_profile),
    /* K2   */  be_nested_str_weak(Matter_X3A_X3A),
    /* K3   */  be_nested_str_weak(tag_number),
    /* K4   */  be_nested_str_weak(0x_X2508X_X20),
    /* K5   */  be_nested_str_weak(tag_vendor),
    /* K6   */  be_nested_str_weak(0x_X2504X_X3A_X3A),
    /* K7   */  be_nested_str_weak(0x_X2504X_X3A),
    /* K8   */  be_nested_str_weak(tag_sub),
    /* K9   */  be_nested_str_weak(_X25i_X20),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(_X3D_X20),
    /* K12  */  be_nested_str_weak(val),
    /* K13  */  be_nested_str_weak(copy),
    /* K14  */  be_nested_str_weak(sort),
    /* K15  */  be_nested_str_weak(concat),
    /* K16  */  be_nested_str_weak(_X2C_X20),
    /* K17  */  be_nested_str_weak(_X20),
    }),
    be_str_weak(tostring_inner),
    &be_const_str_solidified,
    ( &(const binstruction[85]) {  /* code */
      0x58140000,  //  0000  LDCONST	R5	K0
      0xA802004A,  //  0001  EXBLK	0	#004D
      0x50180200,  //  0002  LDBOOL	R6	1	0
      0x20180806,  //  0003  NE	R6	R4	R6
      0x781A0038,  //  0004  JMPF	R6	#003E
      0x88180101,  //  0005  GETMBR	R6	R0	K1
      0x541DFFFE,  //  0006  LDINT	R7	-1
      0x1C180C07,  //  0007  EQ	R6	R6	R7
      0x781A000A,  //  0008  JMPF	R6	#0014
      0x00140B02,  //  0009  ADD	R5	R5	K2
      0x88180103,  //  000A  GETMBR	R6	R0	K3
      0x4C1C0000,  //  000B  LDNIL	R7
      0x20180C07,  //  000C  NE	R6	R6	R7
      0x781A0004,  //  000D  JMPF	R6	#0013
      0x60180018,  //  000E  GETGBL	R6	G24
      0x581C0004,  //  000F  LDCONST	R7	K4
      0x88200103,  //  0010  GETMBR	R8	R0	K3
      0x7C180400,  //  0011  CALL	R6	2
      0x00140A06,  //  0012  ADD	R5	R5	R6
      0x70020023,  //  0013  JMP		#0038
      0x88180105,  //  0014  GETMBR	R6	R0	K5
      0x4C1C0000,  //  0015  LDNIL	R7
      0x20180C07,  //  0016  NE	R6	R6	R7
      0x781A0004,  //  0017  JMPF	R6	#001D
      0x60180018,  //  0018  GETGBL	R6	G24
      0x581C0006,  //  0019  LDCONST	R7	K6
      0x88200105,  //  001A  GETMBR	R8	R0	K5
      0x7C180400,  //  001B  CALL	R6	2
      0x00140A06,  //  001C  ADD	R5	R5	R6
      0x88180101,  //  001D  GETMBR	R6	R0	K1
      0x4C1C0000,  //  001E  LDNIL	R7
      0x20180C07,  //  001F  NE	R6	R6	R7
      0x781A0004,  //  0020  JMPF	R6	#0026
      0x60180018,  //  0021  GETGBL	R6	G24
      0x581C0007,  //  0022  LDCONST	R7	K7
      0x88200101,  //  0023  GETMBR	R8	R0	K1
      0x7C180400,  //  0024  CALL	R6	2
      0x00140A06,  //  0025  ADD	R5	R5	R6
      0x88180103,  //  0026  GETMBR	R6	R0	K3
      0x4C1C0000,  //  0027  LDNIL	R7
      0x20180C07,  //  0028  NE	R6	R6	R7
      0x781A0004,  //  0029  JMPF	R6	#002F
      0x60180018,  //  002A  GETGBL	R6	G24
      0x581C0004,  //  002B  LDCONST	R7	K4
      0x88200103,  //  002C  GETMBR	R8	R0	K3
      0x7C180400,  //  002D  CALL	R6	2
      0x00140A06,  //  002E  ADD	R5	R5	R6
      0x88180108,  //  002F  GETMBR	R6	R0	K8
      0x4C1C0000,  //  0030  LDNIL	R7
      0x20180C07,  //  0031  NE	R6	R6	R7
      0x781A0004,  //  0032  JMPF	R6	#0038
      0x60180018,  //  0033  GETGBL	R6	G24
      0x581C0009,  //  0034  LDCONST	R7	K9
      0x88200108,  //  0035  GETMBR	R8	R0	K8
      0x7C180400,  //  0036  CALL	R6	2
      0x00140A06,  //  0037  ADD	R5	R5	R6
      0x6018000C,  //  0038  GETGBL	R6	G12
      0x5C1C0A00,  //  0039  MOVE	R7	R5
      0x7C180200,  //  003A  CALL	R6	1
      0x24180D0A,  //  003B  GT	R6	R6	K10
      0x781A0000,  //  003C  JMPF	R6	#003E
      0x00140B0B,  //  003D  ADD	R5	R5	K11
      0x00140A02,  //  003E  ADD	R5	R5	R2
      0x8818010C,  //  003F  GETMBR	R6	R0	K12
      0x8C180D0D,  //  0040  GETMET	R6	R6	K13
      0x7C180200,  //  0041  CALL	R6	1
      0x78060002,  //  0042  JMPF	R1	#0046
      0x8C1C010E,  //  0043  GETMET	R7	R0	K14
      0x5C240C00,  //  0044  MOVE	R9	R6
      0x7C1C0400,  //  0045  CALL	R7	2
      0x8C1C0D0F,  //  0046  GETMET	R7	R6	K15
      0x58240010,  //  0047  LDCONST	R9	K16
      0x7C1C0400,  //  0048  CALL	R7	2
      0x00140A07,  //  0049  ADD	R5	R5	R7
      0x00140A03,  //  004A  ADD	R5	R5	R3
      0xA8040001,  //  004B  EXBLK	1	1
      0x70020006,  //  004C  JMP		#0054
      0xAC180002,  //  004D  CATCH	R6	0	2
      0x70020003,  //  004E  JMP		#0053
      0x00200D11,  //  004F  ADD	R8	R6	K17
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_push,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(val),
    /* K1   */  be_nested_str_weak(push),
    }),
    be_str_weak(push),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_findsubtyp,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(findsub),
    /* K1   */  be_nested_str_weak(typ),
    }),
    be_str_weak(findsubtyp),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x880C0501,  //  0006  GETMBR	R3	R2	K1
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
extern const bclass be_class_Matter_TLV_list;
be_local_closure(class_Matter_TLV_list_encode_len,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_list, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(_encode_tag_len),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(val),
    /* K3   */  be_nested_str_weak(encode_len),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(encode_len),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58080001,  //  0002  LDCONST	R2	K1
      0x600C000C,  //  0003  GETGBL	R3	G12
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x140C0403,  //  0006  LT	R3	R2	R3
      0x780E0006,  //  0007  JMPF	R3	#000F
      0x880C0102,  //  0008  GETMBR	R3	R0	K2
      0x940C0602,  //  0009  GETIDX	R3	R3	R2
      0x8C0C0703,  //  000A  GETMET	R3	R3	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x00040203,  //  000C  ADD	R1	R1	R3
      0x00080504,  //  000D  ADD	R2	R2	K4
      0x7001FFF3,  //  000E  JMP		#0003
      0x00040304,  //  000F  ADD	R1	R1	K4
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
extern const bclass be_class_Matter_TLV_struct;
be_local_closure(class_Matter_TLV_struct_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_struct, 
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
extern const bclass be_class_Matter_TLV_struct;
be_local_closure(class_Matter_TLV_struct_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_struct, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(typ),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(STRUCT),
    /* K4   */  be_nested_str_weak(val),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x88080503,  //  0007  GETMBR	R2	R2	K3
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x60080012,  //  0009  GETGBL	R2	G18
      0x7C080000,  //  000A  CALL	R2	0
      0x90020802,  //  000B  SETMBR	R0	K4	R2
      0x80000000,  //  000C  RET	0
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

extern const bclass be_class_Matter_TLV_array;

/********************************************************************
** Solidified function: tostring
********************************************************************/
extern const bclass be_class_Matter_TLV_array;
be_local_closure(class_Matter_TLV_array_tostring,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_array, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tostring_inner),
    /* K1   */  be_nested_str_weak(_X5B),
    /* K2   */  be_nested_str_weak(_X5D),
    }),
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
extern const bclass be_class_Matter_TLV_array;
be_local_closure(class_Matter_TLV_array_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_array, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(typ),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(ARRAY),
    /* K4   */  be_nested_str_weak(val),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x88080503,  //  0007  GETMBR	R2	R2	K3
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x60080012,  //  0009  GETGBL	R2	G18
      0x7C080000,  //  000A  CALL	R2	0
      0x90020802,  //  000B  SETMBR	R0	K4	R2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
extern const bclass be_class_Matter_TLV_array;
be_local_closure(class_Matter_TLV_array_parse,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV_array, 
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(TLV),
    /* K1   */  be_nested_str_weak(EOC),
    /* K2   */  be_nested_str_weak(parse),
    /* K3   */  be_nested_str_weak(next_idx),
    /* K4   */  be_nested_str_weak(tag_vendor),
    /* K5   */  be_nested_str_weak(tag_profile),
    /* K6   */  be_nested_str_weak(tag_number),
    /* K7   */  be_nested_str_weak(tag_sub),
    /* K8   */  be_nested_str_weak(val),
    /* K9   */  be_nested_str_weak(push),
    /* K10  */  be_const_int(1),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x940C0202,  //  0000  GETIDX	R3	R1	R2
      0x88100100,  //  0001  GETMBR	R4	R0	K0
      0x88100901,  //  0002  GETMBR	R4	R4	K1
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x780E0013,  //  0004  JMPF	R3	#0019
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x8C0C0702,  //  0006  GETMET	R3	R3	K2
      0x5C140200,  //  0007  MOVE	R5	R1
      0x5C180400,  //  0008  MOVE	R6	R2
      0x5C1C0000,  //  0009  MOVE	R7	R0
      0x7C0C0800,  //  000A  CALL	R3	4
      0x88080703,  //  000B  GETMBR	R2	R3	K3
      0x4C100000,  //  000C  LDNIL	R4
      0x900E0804,  //  000D  SETMBR	R3	K4	R4
      0x4C100000,  //  000E  LDNIL	R4
      0x900E0A04,  //  000F  SETMBR	R3	K5	R4
      0x4C100000,  //  0010  LDNIL	R4
      0x900E0C04,  //  0011  SETMBR	R3	K6	R4
      0x4C100000,  //  0012  LDNIL	R4
      0x900E0E04,  //  0013  SETMBR	R3	K7	R4
      0x88100108,  //  0014  GETMBR	R4	R0	K8
      0x8C100909,  //  0015  GETMET	R4	R4	K9
      0x5C180600,  //  0016  MOVE	R6	R3
      0x7C100400,  //  0017  CALL	R4	2
      0x7001FFE6,  //  0018  JMP		#0000
      0x0008050A,  //  0019  ADD	R2	R2	K10
      0x90020602,  //  001A  SETMBR	R0	K3	R2
      0x80040400,  //  001B  RET	1	R2
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

/********************************************************************
** Solidified function: create_TLV
********************************************************************/
extern const bclass be_class_Matter_TLV;
be_local_closure(class_Matter_TLV_create_TLV,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_TLV),
    /* K1   */  be_nested_str_weak(Matter_TLV_item),
    /* K2   */  be_nested_str_weak(create_TLV),
    }),
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
extern const bclass be_class_Matter_TLV;
be_local_closure(class_Matter_TLV_parse,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_TLV, 
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_TLV),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(EOC),
    /* K4   */  be_nested_str_weak(invalid_X20TLV_X20type_X20),
    /* K5   */  be_nested_str_weak(TLV_error),
    /* K6   */  be_nested_str_weak(STRUCT),
    /* K7   */  be_nested_str_weak(Matter_TLV_struct),
    /* K8   */  be_nested_str_weak(ARRAY),
    /* K9   */  be_nested_str_weak(Matter_TLV_array),
    /* K10  */  be_nested_str_weak(LIST),
    /* K11  */  be_nested_str_weak(Matter_TLV_list),
    /* K12  */  be_nested_str_weak(Matter_TLV_item),
    /* K13  */  be_nested_str_weak(typ),
    /* K14  */  be_nested_str_weak(tag_vendor),
    /* K15  */  be_nested_str_weak(get),
    /* K16  */  be_const_int(2),
    /* K17  */  be_nested_str_weak(tag_profile),
    /* K18  */  be_nested_str_weak(tag_sub),
    /* K19  */  be_nested_str_weak(tag_number),
    /* K20  */  be_nested_str_weak(parse),
    }),
    be_str_weak(parse),
    &be_const_str_solidified,
    ( &(const binstruction[118]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x5C100600,  //  0001  MOVE	R4	R3
      0x4C140000,  //  0002  LDNIL	R5
      0x1C140205,  //  0003  EQ	R5	R1	R5
      0x78160000,  //  0004  JMPF	R5	#0006
      0x58040001,  //  0005  LDCONST	R1	K1
      0x94140001,  //  0006  GETIDX	R5	R0	R1
      0x541A001E,  //  0007  LDINT	R6	31
      0x2C140A06,  //  0008  AND	R5	R5	R6
      0x94180001,  //  0009  GETIDX	R6	R0	R1
      0x541E00DF,  //  000A  LDINT	R7	224
      0x2C180C07,  //  000B  AND	R6	R6	R7
      0x00040302,  //  000C  ADD	R1	R1	K2
      0x881C0903,  //  000D  GETMBR	R7	R4	K3
      0x241C0A07,  //  000E  GT	R7	R5	R7
      0x781E0004,  //  000F  JMPF	R7	#0015
      0x601C0008,  //  0010  GETGBL	R7	G8
      0x5C200A00,  //  0011  MOVE	R8	R5
      0x7C1C0200,  //  0012  CALL	R7	1
      0x001E0807,  //  0013  ADD	R7	K4	R7
      0xB0060A07,  //  0014  RAISE	1	K5	R7
      0x4C1C0000,  //  0015  LDNIL	R7
      0x88200906,  //  0016  GETMBR	R8	R4	K6
      0x1C200A08,  //  0017  EQ	R8	R5	R8
      0x78220004,  //  0018  JMPF	R8	#001E
      0x8C200707,  //  0019  GETMET	R8	R3	K7
      0x5C280400,  //  001A  MOVE	R10	R2
      0x7C200400,  //  001B  CALL	R8	2
      0x5C1C1000,  //  001C  MOVE	R7	R8
      0x70020013,  //  001D  JMP		#0032
      0x88200908,  //  001E  GETMBR	R8	R4	K8
      0x1C200A08,  //  001F  EQ	R8	R5	R8
      0x78220004,  //  0020  JMPF	R8	#0026
      0x8C200709,  //  0021  GETMET	R8	R3	K9
      0x5C280400,  //  0022  MOVE	R10	R2
      0x7C200400,  //  0023  CALL	R8	2
      0x5C1C1000,  //  0024  MOVE	R7	R8
      0x7002000B,  //  0025  JMP		#0032
      0x8820090A,  //  0026  GETMBR	R8	R4	K10
      0x1C200A08,  //  0027  EQ	R8	R5	R8
      0x78220004,  //  0028  JMPF	R8	#002E
      0x8C20070B,  //  0029  GETMET	R8	R3	K11
      0x5C280400,  //  002A  MOVE	R10	R2
      0x7C200400,  //  002B  CALL	R8	2
      0x5C1C1000,  //  002C  MOVE	R7	R8
      0x70020003,  //  002D  JMP		#0032
      0x8C20070C,  //  002E  GETMET	R8	R3	K12
      0x5C280400,  //  002F  MOVE	R10	R2
      0x7C200400,  //  0030  CALL	R8	2
      0x5C1C1000,  //  0031  MOVE	R7	R8
      0x901E1A05,  //  0032  SETMBR	R7	K13	R5
      0x542200BF,  //  0033  LDINT	R8	192
      0x1C200C08,  //  0034  EQ	R8	R6	R8
      0x74220002,  //  0035  JMPT	R8	#0039
      0x542200DF,  //  0036  LDINT	R8	224
      0x1C200C08,  //  0037  EQ	R8	R6	R8
      0x7822000B,  //  0038  JMPF	R8	#0045
      0x8C20010F,  //  0039  GETMET	R8	R0	K15
      0x5C280200,  //  003A  MOVE	R10	R1
      0x582C0010,  //  003B  LDCONST	R11	K16
      0x7C200600,  //  003C  CALL	R8	3
      0x901E1C08,  //  003D  SETMBR	R7	K14	R8
      0x8C20010F,  //  003E  GETMET	R8	R0	K15
      0x00280310,  //  003F  ADD	R10	R1	K16
      0x582C0010,  //  0040  LDCONST	R11	K16
      0x7C200600,  //  0041  CALL	R8	3
      0x901E2208,  //  0042  SETMBR	R7	K17	R8
      0x54220003,  //  0043  LDINT	R8	4
      0x00040208,  //  0044  ADD	R1	R1	R8
      0x5422003F,  //  0045  LDINT	R8	64
      0x1C200C08,  //  0046  EQ	R8	R6	R8
      0x74220002,  //  0047  JMPT	R8	#004B
      0x5422005F,  //  0048  LDINT	R8	96
      0x1C200C08,  //  0049  EQ	R8	R6	R8
      0x78220003,  //  004A  JMPF	R8	#004F
      0x4C200000,  //  004B  LDNIL	R8
      0x901E1C08,  //  004C  SETMBR	R7	K14	R8
      0x5421FFFE,  //  004D  LDINT	R8	-1
      0x901E2208,  //  004E  SETMBR	R7	K17	R8
      0x1C200D01,  //  004F  EQ	R8	R6	K1
      0x78220000,  //  0050  JMPF	R8	#0052
      0x7002001D,  //  0051  JMP		#0070
      0x5422001F,  //  0052  LDINT	R8	32
      0x1C200C08,  //  0053  EQ	R8	R6	R8
      0x78220003,  //  0054  JMPF	R8	#0059
      0x94200001,  //  0055  GETIDX	R8	R0	R1
      0x901E2408,  //  0056  SETMBR	R7	K18	R8
      0x00040302,  //  0057  ADD	R1	R1	K2
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
      0x8C20010F,  //  0062  GETMET	R8	R0	K15
      0x5C280200,  //  0063  MOVE	R10	R1
      0x582C0010,  //  0064  LDCONST	R11	K16
      0x7C200600,  //  0065  CALL	R8	3
      0x901E2608,  //  0066  SETMBR	R7	K19	R8
      0x00040310,  //  0067  ADD	R1	R1	K16
      0x70020006,  //  0068  JMP		#0070
      0x8C20010F,  //  0069  GETMET	R8	R0	K15
      0x5C280200,  //  006A  MOVE	R10	R1
      0x542E0003,  //  006B  LDINT	R11	4
      0x7C200600,  //  006C  CALL	R8	3
      0x901E2608,  //  006D  SETMBR	R7	K19	R8
      0x54220003,  //  006E  LDINT	R8	4
      0x00040208,  //  006F  ADD	R1	R1	R8
      0x8C200F14,  //  0070  GETMET	R8	R7	K20
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
    be_nested_map(35,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(BFALSE, -1), be_const_int(8) },
        { be_const_key_weak(create_TLV, -1), be_const_static_closure(class_Matter_TLV_create_TLV_closure) },
        { be_const_key_weak(Matter_TLV_list, -1), be_const_class(be_class_Matter_TLV_list) },
        { be_const_key_weak(I4, 33), be_const_int(2) },
        { be_const_key_weak(U2, -1), be_const_int(5) },
        { be_const_key_weak(UTF4, -1), be_const_int(14) },
        { be_const_key_weak(DOUBLE, 3), be_const_int(11) },
        { be_const_key_weak(U4, 28), be_const_int(6) },
        { be_const_key_weak(UTF2, 14), be_const_int(13) },
        { be_const_key_weak(Matter_TLV_struct, 2), be_const_class(be_class_Matter_TLV_struct) },
        { be_const_key_weak(BOOL, -1), be_const_int(8) },
        { be_const_key_weak(RAW, -1), be_const_int(255) },
        { be_const_key_weak(parse, -1), be_const_static_closure(class_Matter_TLV_parse_closure) },
        { be_const_key_weak(U8, -1), be_const_int(7) },
        { be_const_key_weak(LIST, -1), be_const_int(23) },
        { be_const_key_weak(I2, 27), be_const_int(1) },
        { be_const_key_weak(B2, -1), be_const_int(17) },
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
        { be_const_key_weak(EOC, -1), be_const_int(24) },
        { be_const_key_weak(Matter_TLV_array, -1), be_const_class(be_class_Matter_TLV_array) },
        { be_const_key_weak(U1, 32), be_const_int(4) },
        { be_const_key_weak(STRUCT, -1), be_const_int(21) },
        { be_const_key_weak(Matter_TLV_item, 19), be_const_class(be_class_Matter_TLV_item) },
        { be_const_key_weak(_type, 21), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_weak(I8, 26), be_const_int(3) },
        { be_const_key_weak(B8, 11), be_const_int(19) },
        { be_const_key_weak(FLOAT, -1), be_const_int(10) },
        { be_const_key_weak(ARRAY, -1), be_const_int(22) },
        { be_const_key_weak(B4, -1), be_const_int(18) },
        { be_const_key_weak(B1, -1), be_const_int(16) },
        { be_const_key_weak(UTF1, -1), be_const_int(12) },
        { be_const_key_weak(I1, -1), be_const_int(0) },
        { be_const_key_weak(BTRUE, -1), be_const_int(9) },
        { be_const_key_weak(NULL, -1), be_const_int(20) },
        { be_const_key_weak(UTF8, -1), be_const_int(15) },
    })),
    be_str_weak(Matter_TLV)
);
/********************************************************************/
/* End of solidification */
