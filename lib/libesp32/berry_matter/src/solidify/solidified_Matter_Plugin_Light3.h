/* Solidification of Matter_Plugin_Light3.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light3;

/********************************************************************
** Solidified function: onoff_changed
********************************************************************/
be_local_closure(Matter_Plugin_Light3_onoff_changed,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(attribute_updated),
    /* K1   */  be_const_int(0),
    }),
    be_str_weak(onoff_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x54120005,  //  0002  LDINT	R4	6
      0x58140001,  //  0003  LDCONST	R5	K1
      0x7C040800,  //  0004  CALL	R1	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light3_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_hue),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(shadow_bri),
    /* K4   */  be_nested_str_weak(shadow_sat),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x90020302,  //  0007  SETMBR	R0	K1	K2
      0x90020702,  //  0008  SETMBR	R0	K3	K2
      0x90020902,  //  0009  SETMBR	R0	K4	K2
      0x500C0000,  //  000A  LDBOOL	R3	0	0
      0x90020A03,  //  000B  SETMBR	R0	K5	R3
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light3_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(3),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(U2),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(U1),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(BOOL),
    /* K13  */  be_nested_str_weak(shadow_onoff),
    /* K14  */  be_nested_str_weak(shadow_bri),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(shadow_hue),
    /* K17  */  be_nested_str_weak(shadow_sat),
    /* K18  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[280]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x1C1C0B05,  //  0005  EQ	R7	R5	K5
      0x781E0021,  //  0006  JMPF	R7	#0029
      0x1C1C0D06,  //  0007  EQ	R7	R6	K6
      0x781E0005,  //  0008  JMPF	R7	#000F
      0x8C1C0907,  //  0009  GETMET	R7	R4	K7
      0x88240908,  //  000A  GETMBR	R9	R4	K8
      0x58280006,  //  000B  LDCONST	R10	K6
      0x7C1C0600,  //  000C  CALL	R7	3
      0x80040E00,  //  000D  RET	1	R7
      0x70020018,  //  000E  JMP		#0028
      0x1C1C0D09,  //  000F  EQ	R7	R6	K9
      0x781E0005,  //  0010  JMPF	R7	#0017
      0x8C1C0907,  //  0011  GETMET	R7	R4	K7
      0x8824090A,  //  0012  GETMBR	R9	R4	K10
      0x58280006,  //  0013  LDCONST	R10	K6
      0x7C1C0600,  //  0014  CALL	R7	3
      0x80040E00,  //  0015  RET	1	R7
      0x70020010,  //  0016  JMP		#0028
      0x541EFFFB,  //  0017  LDINT	R7	65532
      0x1C1C0C07,  //  0018  EQ	R7	R6	R7
      0x781E0005,  //  0019  JMPF	R7	#0020
      0x8C1C0907,  //  001A  GETMET	R7	R4	K7
      0x8824090B,  //  001B  GETMBR	R9	R4	K11
      0x58280006,  //  001C  LDCONST	R10	K6
      0x7C1C0600,  //  001D  CALL	R7	3
      0x80040E00,  //  001E  RET	1	R7
      0x70020007,  //  001F  JMP		#0028
      0x541EFFFC,  //  0020  LDINT	R7	65533
      0x1C1C0C07,  //  0021  EQ	R7	R6	R7
      0x781E0004,  //  0022  JMPF	R7	#0028
      0x8C1C0907,  //  0023  GETMET	R7	R4	K7
      0x8824090B,  //  0024  GETMBR	R9	R4	K11
      0x542A0003,  //  0025  LDINT	R10	4
      0x7C1C0600,  //  0026  CALL	R7	3
      0x80040E00,  //  0027  RET	1	R7
      0x700200ED,  //  0028  JMP		#0117
      0x541E0003,  //  0029  LDINT	R7	4
      0x1C1C0A07,  //  002A  EQ	R7	R5	R7
      0x781E0016,  //  002B  JMPF	R7	#0043
      0x1C1C0D06,  //  002C  EQ	R7	R6	K6
      0x781E0002,  //  002D  JMPF	R7	#0031
      0x4C1C0000,  //  002E  LDNIL	R7
      0x80040E00,  //  002F  RET	1	R7
      0x70020010,  //  0030  JMP		#0042
      0x541EFFFB,  //  0031  LDINT	R7	65532
      0x1C1C0C07,  //  0032  EQ	R7	R6	R7
      0x781E0005,  //  0033  JMPF	R7	#003A
      0x8C1C0907,  //  0034  GETMET	R7	R4	K7
      0x8824090B,  //  0035  GETMBR	R9	R4	K11
      0x58280006,  //  0036  LDCONST	R10	K6
      0x7C1C0600,  //  0037  CALL	R7	3
      0x80040E00,  //  0038  RET	1	R7
      0x70020007,  //  0039  JMP		#0042
      0x541EFFFC,  //  003A  LDINT	R7	65533
      0x1C1C0C07,  //  003B  EQ	R7	R6	R7
      0x781E0004,  //  003C  JMPF	R7	#0042
      0x8C1C0907,  //  003D  GETMET	R7	R4	K7
      0x8824090B,  //  003E  GETMBR	R9	R4	K11
      0x542A0003,  //  003F  LDINT	R10	4
      0x7C1C0600,  //  0040  CALL	R7	3
      0x80040E00,  //  0041  RET	1	R7
      0x700200D3,  //  0042  JMP		#0117
      0x541E0004,  //  0043  LDINT	R7	5
      0x1C1C0A07,  //  0044  EQ	R7	R5	R7
      0x781E0011,  //  0045  JMPF	R7	#0058
      0x541EFFFB,  //  0046  LDINT	R7	65532
      0x1C1C0C07,  //  0047  EQ	R7	R6	R7
      0x781E0005,  //  0048  JMPF	R7	#004F
      0x8C1C0907,  //  0049  GETMET	R7	R4	K7
      0x8824090B,  //  004A  GETMBR	R9	R4	K11
      0x58280006,  //  004B  LDCONST	R10	K6
      0x7C1C0600,  //  004C  CALL	R7	3
      0x80040E00,  //  004D  RET	1	R7
      0x70020007,  //  004E  JMP		#0057
      0x541EFFFC,  //  004F  LDINT	R7	65533
      0x1C1C0C07,  //  0050  EQ	R7	R6	R7
      0x781E0004,  //  0051  JMPF	R7	#0057
      0x8C1C0907,  //  0052  GETMET	R7	R4	K7
      0x8824090B,  //  0053  GETMBR	R9	R4	K11
      0x542A0003,  //  0054  LDINT	R10	4
      0x7C1C0600,  //  0055  CALL	R7	3
      0x80040E00,  //  0056  RET	1	R7
      0x700200BE,  //  0057  JMP		#0117
      0x541E0005,  //  0058  LDINT	R7	6
      0x1C1C0A07,  //  0059  EQ	R7	R5	R7
      0x781E0019,  //  005A  JMPF	R7	#0075
      0x1C1C0D06,  //  005B  EQ	R7	R6	K6
      0x781E0005,  //  005C  JMPF	R7	#0063
      0x8C1C0907,  //  005D  GETMET	R7	R4	K7
      0x8824090C,  //  005E  GETMBR	R9	R4	K12
      0x8828010D,  //  005F  GETMBR	R10	R0	K13
      0x7C1C0600,  //  0060  CALL	R7	3
      0x80040E00,  //  0061  RET	1	R7
      0x70020010,  //  0062  JMP		#0074
      0x541EFFFB,  //  0063  LDINT	R7	65532
      0x1C1C0C07,  //  0064  EQ	R7	R6	R7
      0x781E0005,  //  0065  JMPF	R7	#006C
      0x8C1C0907,  //  0066  GETMET	R7	R4	K7
      0x8824090B,  //  0067  GETMBR	R9	R4	K11
      0x58280006,  //  0068  LDCONST	R10	K6
      0x7C1C0600,  //  0069  CALL	R7	3
      0x80040E00,  //  006A  RET	1	R7
      0x70020007,  //  006B  JMP		#0074
      0x541EFFFC,  //  006C  LDINT	R7	65533
      0x1C1C0C07,  //  006D  EQ	R7	R6	R7
      0x781E0004,  //  006E  JMPF	R7	#0074
      0x8C1C0907,  //  006F  GETMET	R7	R4	K7
      0x8824090B,  //  0070  GETMBR	R9	R4	K11
      0x542A0003,  //  0071  LDINT	R10	4
      0x7C1C0600,  //  0072  CALL	R7	3
      0x80040E00,  //  0073  RET	1	R7
      0x700200A1,  //  0074  JMP		#0117
      0x541E0007,  //  0075  LDINT	R7	8
      0x1C1C0A07,  //  0076  EQ	R7	R5	R7
      0x781E003B,  //  0077  JMPF	R7	#00B4
      0x1C1C0D06,  //  0078  EQ	R7	R6	K6
      0x781E0005,  //  0079  JMPF	R7	#0080
      0x8C1C0907,  //  007A  GETMET	R7	R4	K7
      0x8824090A,  //  007B  GETMBR	R9	R4	K10
      0x8828010E,  //  007C  GETMBR	R10	R0	K14
      0x7C1C0600,  //  007D  CALL	R7	3
      0x80040E00,  //  007E  RET	1	R7
      0x70020032,  //  007F  JMP		#00B3
      0x1C1C0D0F,  //  0080  EQ	R7	R6	K15
      0x781E0005,  //  0081  JMPF	R7	#0088
      0x8C1C0907,  //  0082  GETMET	R7	R4	K7
      0x8824090A,  //  0083  GETMBR	R9	R4	K10
      0x58280006,  //  0084  LDCONST	R10	K6
      0x7C1C0600,  //  0085  CALL	R7	3
      0x80040E00,  //  0086  RET	1	R7
      0x7002002A,  //  0087  JMP		#00B3
      0x1C1C0D05,  //  0088  EQ	R7	R6	K5
      0x781E0005,  //  0089  JMPF	R7	#0090
      0x8C1C0907,  //  008A  GETMET	R7	R4	K7
      0x8824090A,  //  008B  GETMBR	R9	R4	K10
      0x542A00FD,  //  008C  LDINT	R10	254
      0x7C1C0600,  //  008D  CALL	R7	3
      0x80040E00,  //  008E  RET	1	R7
      0x70020022,  //  008F  JMP		#00B3
      0x541E000E,  //  0090  LDINT	R7	15
      0x1C1C0C07,  //  0091  EQ	R7	R6	R7
      0x781E0005,  //  0092  JMPF	R7	#0099
      0x8C1C0907,  //  0093  GETMET	R7	R4	K7
      0x8824090A,  //  0094  GETMBR	R9	R4	K10
      0x58280006,  //  0095  LDCONST	R10	K6
      0x7C1C0600,  //  0096  CALL	R7	3
      0x80040E00,  //  0097  RET	1	R7
      0x70020019,  //  0098  JMP		#00B3
      0x541E0010,  //  0099  LDINT	R7	17
      0x1C1C0C07,  //  009A  EQ	R7	R6	R7
      0x781E0005,  //  009B  JMPF	R7	#00A2
      0x8C1C0907,  //  009C  GETMET	R7	R4	K7
      0x8824090A,  //  009D  GETMBR	R9	R4	K10
      0x8828010E,  //  009E  GETMBR	R10	R0	K14
      0x7C1C0600,  //  009F  CALL	R7	3
      0x80040E00,  //  00A0  RET	1	R7
      0x70020010,  //  00A1  JMP		#00B3
      0x541EFFFB,  //  00A2  LDINT	R7	65532
      0x1C1C0C07,  //  00A3  EQ	R7	R6	R7
      0x781E0005,  //  00A4  JMPF	R7	#00AB
      0x8C1C0907,  //  00A5  GETMET	R7	R4	K7
      0x8824090B,  //  00A6  GETMBR	R9	R4	K11
      0x58280009,  //  00A7  LDCONST	R10	K9
      0x7C1C0600,  //  00A8  CALL	R7	3
      0x80040E00,  //  00A9  RET	1	R7
      0x70020007,  //  00AA  JMP		#00B3
      0x541EFFFC,  //  00AB  LDINT	R7	65533
      0x1C1C0C07,  //  00AC  EQ	R7	R6	R7
      0x781E0004,  //  00AD  JMPF	R7	#00B3
      0x8C1C0907,  //  00AE  GETMET	R7	R4	K7
      0x8824090B,  //  00AF  GETMBR	R9	R4	K11
      0x542A0004,  //  00B0  LDINT	R10	5
      0x7C1C0600,  //  00B1  CALL	R7	3
      0x80040E00,  //  00B2  RET	1	R7
      0x70020062,  //  00B3  JMP		#0117
      0x541E02FF,  //  00B4  LDINT	R7	768
      0x1C1C0A07,  //  00B5  EQ	R7	R5	R7
      0x781E0057,  //  00B6  JMPF	R7	#010F
      0x1C1C0D06,  //  00B7  EQ	R7	R6	K6
      0x781E0005,  //  00B8  JMPF	R7	#00BF
      0x8C1C0907,  //  00B9  GETMET	R7	R4	K7
      0x8824090A,  //  00BA  GETMBR	R9	R4	K10
      0x88280110,  //  00BB  GETMBR	R10	R0	K16
      0x7C1C0600,  //  00BC  CALL	R7	3
      0x80040E00,  //  00BD  RET	1	R7
      0x7002004E,  //  00BE  JMP		#010E
      0x1C1C0D09,  //  00BF  EQ	R7	R6	K9
      0x781E0005,  //  00C0  JMPF	R7	#00C7
      0x8C1C0907,  //  00C1  GETMET	R7	R4	K7
      0x8824090A,  //  00C2  GETMBR	R9	R4	K10
      0x88280111,  //  00C3  GETMBR	R10	R0	K17
      0x7C1C0600,  //  00C4  CALL	R7	3
      0x80040E00,  //  00C5  RET	1	R7
      0x70020046,  //  00C6  JMP		#010E
      0x541E0006,  //  00C7  LDINT	R7	7
      0x1C1C0C07,  //  00C8  EQ	R7	R6	R7
      0x781E0005,  //  00C9  JMPF	R7	#00D0
      0x8C1C0907,  //  00CA  GETMET	R7	R4	K7
      0x8824090A,  //  00CB  GETMBR	R9	R4	K10
      0x58280006,  //  00CC  LDCONST	R10	K6
      0x7C1C0600,  //  00CD  CALL	R7	3
      0x80040E00,  //  00CE  RET	1	R7
      0x7002003D,  //  00CF  JMP		#010E
      0x541E0007,  //  00D0  LDINT	R7	8
      0x1C1C0C07,  //  00D1  EQ	R7	R6	R7
      0x781E0005,  //  00D2  JMPF	R7	#00D9
      0x8C1C0907,  //  00D3  GETMET	R7	R4	K7
      0x8824090A,  //  00D4  GETMBR	R9	R4	K10
      0x58280006,  //  00D5  LDCONST	R10	K6
      0x7C1C0600,  //  00D6  CALL	R7	3
      0x80040E00,  //  00D7  RET	1	R7
      0x70020034,  //  00D8  JMP		#010E
      0x541E000E,  //  00D9  LDINT	R7	15
      0x1C1C0C07,  //  00DA  EQ	R7	R6	R7
      0x781E0005,  //  00DB  JMPF	R7	#00E2
      0x8C1C0907,  //  00DC  GETMET	R7	R4	K7
      0x8824090A,  //  00DD  GETMBR	R9	R4	K10
      0x58280006,  //  00DE  LDCONST	R10	K6
      0x7C1C0600,  //  00DF  CALL	R7	3
      0x80040E00,  //  00E0  RET	1	R7
      0x7002002B,  //  00E1  JMP		#010E
      0x541E4000,  //  00E2  LDINT	R7	16385
      0x1C1C0C07,  //  00E3  EQ	R7	R6	R7
      0x781E0005,  //  00E4  JMPF	R7	#00EB
      0x8C1C0907,  //  00E5  GETMET	R7	R4	K7
      0x8824090A,  //  00E6  GETMBR	R9	R4	K10
      0x58280006,  //  00E7  LDCONST	R10	K6
      0x7C1C0600,  //  00E8  CALL	R7	3
      0x80040E00,  //  00E9  RET	1	R7
      0x70020022,  //  00EA  JMP		#010E
      0x541E4009,  //  00EB  LDINT	R7	16394
      0x1C1C0C07,  //  00EC  EQ	R7	R6	R7
      0x781E0005,  //  00ED  JMPF	R7	#00F4
      0x8C1C0907,  //  00EE  GETMET	R7	R4	K7
      0x8824090A,  //  00EF  GETMBR	R9	R4	K10
      0x58280006,  //  00F0  LDCONST	R10	K6
      0x7C1C0600,  //  00F1  CALL	R7	3
      0x80040E00,  //  00F2  RET	1	R7
      0x70020019,  //  00F3  JMP		#010E
      0x541E000F,  //  00F4  LDINT	R7	16
      0x1C1C0C07,  //  00F5  EQ	R7	R6	R7
      0x781E0005,  //  00F6  JMPF	R7	#00FD
      0x8C1C0907,  //  00F7  GETMET	R7	R4	K7
      0x8824090A,  //  00F8  GETMBR	R9	R4	K10
      0x58280006,  //  00F9  LDCONST	R10	K6
      0x7C1C0600,  //  00FA  CALL	R7	3
      0x80040E00,  //  00FB  RET	1	R7
      0x70020010,  //  00FC  JMP		#010E
      0x541EFFFB,  //  00FD  LDINT	R7	65532
      0x1C1C0C07,  //  00FE  EQ	R7	R6	R7
      0x781E0005,  //  00FF  JMPF	R7	#0106
      0x8C1C0907,  //  0100  GETMET	R7	R4	K7
      0x8824090B,  //  0101  GETMBR	R9	R4	K11
      0x58280009,  //  0102  LDCONST	R10	K9
      0x7C1C0600,  //  0103  CALL	R7	3
      0x80040E00,  //  0104  RET	1	R7
      0x70020007,  //  0105  JMP		#010E
      0x541EFFFC,  //  0106  LDINT	R7	65533
      0x1C1C0C07,  //  0107  EQ	R7	R6	R7
      0x781E0004,  //  0108  JMPF	R7	#010E
      0x8C1C0907,  //  0109  GETMET	R7	R4	K7
      0x8824090B,  //  010A  GETMBR	R9	R4	K11
      0x542A0004,  //  010B  LDINT	R10	5
      0x7C1C0600,  //  010C  CALL	R7	3
      0x80040E00,  //  010D  RET	1	R7
      0x70020007,  //  010E  JMP		#0117
      0x601C0003,  //  010F  GETGBL	R7	G3
      0x5C200000,  //  0110  MOVE	R8	R0
      0x7C1C0200,  //  0111  CALL	R7	1
      0x8C1C0F12,  //  0112  GETMET	R7	R7	K18
      0x5C240200,  //  0113  MOVE	R9	R1
      0x5C280400,  //  0114  MOVE	R10	R2
      0x7C1C0600,  //  0115  CALL	R7	3
      0x80040E00,  //  0116  RET	1	R7
      0x80000000,  //  0117  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light3_update_shadow,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(bri),
    /* K4   */  be_nested_str_weak(hue),
    /* K5   */  be_nested_str_weak(sat),
    /* K6   */  be_nested_str_weak(power),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(scale_uint),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(shadow_bri),
    /* K11  */  be_nested_str_weak(shadow_hue),
    /* K12  */  be_nested_str_weak(shadow_sat),
    /* K13  */  be_nested_str_weak(shadow_onoff),
    /* K14  */  be_nested_str_weak(attribute_updated),
    /* K15  */  be_const_int(1),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[98]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x58140003,  //  0004  LDCONST	R5	K3
      0x4C180000,  //  0005  LDNIL	R6
      0x7C0C0600,  //  0006  CALL	R3	3
      0x8C100502,  //  0007  GETMET	R4	R2	K2
      0x58180004,  //  0008  LDCONST	R6	K4
      0x4C1C0000,  //  0009  LDNIL	R7
      0x7C100600,  //  000A  CALL	R4	3
      0x8C140502,  //  000B  GETMET	R5	R2	K2
      0x581C0005,  //  000C  LDCONST	R7	K5
      0x4C200000,  //  000D  LDNIL	R8
      0x7C140600,  //  000E  CALL	R5	3
      0x8C180502,  //  000F  GETMET	R6	R2	K2
      0x58200006,  //  0010  LDCONST	R8	K6
      0x4C240000,  //  0011  LDNIL	R9
      0x7C180600,  //  0012  CALL	R6	3
      0x4C1C0000,  //  0013  LDNIL	R7
      0x201C0607,  //  0014  NE	R7	R3	R7
      0x781E0009,  //  0015  JMPF	R7	#0020
      0xB81E0E00,  //  0016  GETNGBL	R7	K7
      0x8C1C0F08,  //  0017  GETMET	R7	R7	K8
      0x5C240600,  //  0018  MOVE	R9	R3
      0x58280009,  //  0019  LDCONST	R10	K9
      0x542E00FE,  //  001A  LDINT	R11	255
      0x58300009,  //  001B  LDCONST	R12	K9
      0x543600FD,  //  001C  LDINT	R13	254
      0x7C1C0C00,  //  001D  CALL	R7	6
      0x5C0C0E00,  //  001E  MOVE	R3	R7
      0x70020000,  //  001F  JMP		#0021
      0x880C010A,  //  0020  GETMBR	R3	R0	K10
      0x4C1C0000,  //  0021  LDNIL	R7
      0x201C0807,  //  0022  NE	R7	R4	R7
      0x781E0009,  //  0023  JMPF	R7	#002E
      0xB81E0E00,  //  0024  GETNGBL	R7	K7
      0x8C1C0F08,  //  0025  GETMET	R7	R7	K8
      0x5C240800,  //  0026  MOVE	R9	R4
      0x58280009,  //  0027  LDCONST	R10	K9
      0x542E0167,  //  0028  LDINT	R11	360
      0x58300009,  //  0029  LDCONST	R12	K9
      0x543600FD,  //  002A  LDINT	R13	254
      0x7C1C0C00,  //  002B  CALL	R7	6
      0x5C100E00,  //  002C  MOVE	R4	R7
      0x70020000,  //  002D  JMP		#002F
      0x880C010B,  //  002E  GETMBR	R3	R0	K11
      0x4C1C0000,  //  002F  LDNIL	R7
      0x201C0A07,  //  0030  NE	R7	R5	R7
      0x781E0009,  //  0031  JMPF	R7	#003C
      0xB81E0E00,  //  0032  GETNGBL	R7	K7
      0x8C1C0F08,  //  0033  GETMET	R7	R7	K8
      0x5C240A00,  //  0034  MOVE	R9	R5
      0x58280009,  //  0035  LDCONST	R10	K9
      0x542E00FE,  //  0036  LDINT	R11	255
      0x58300009,  //  0037  LDCONST	R12	K9
      0x543600FD,  //  0038  LDINT	R13	254
      0x7C1C0C00,  //  0039  CALL	R7	6
      0x5C140E00,  //  003A  MOVE	R5	R7
      0x70020000,  //  003B  JMP		#003D
      0x880C010C,  //  003C  GETMBR	R3	R0	K12
      0x881C010D,  //  003D  GETMBR	R7	R0	K13
      0x201C0C07,  //  003E  NE	R7	R6	R7
      0x781E0005,  //  003F  JMPF	R7	#0046
      0x8C1C010E,  //  0040  GETMET	R7	R0	K14
      0x4C240000,  //  0041  LDNIL	R9
      0x542A0005,  //  0042  LDINT	R10	6
      0x582C0009,  //  0043  LDCONST	R11	K9
      0x7C1C0800,  //  0044  CALL	R7	4
      0x90021A06,  //  0045  SETMBR	R0	K13	R6
      0x881C010A,  //  0046  GETMBR	R7	R0	K10
      0x201C0607,  //  0047  NE	R7	R3	R7
      0x781E0005,  //  0048  JMPF	R7	#004F
      0x8C1C010E,  //  0049  GETMET	R7	R0	K14
      0x4C240000,  //  004A  LDNIL	R9
      0x542A0007,  //  004B  LDINT	R10	8
      0x582C0009,  //  004C  LDCONST	R11	K9
      0x7C1C0800,  //  004D  CALL	R7	4
      0x90021403,  //  004E  SETMBR	R0	K10	R3
      0x881C010B,  //  004F  GETMBR	R7	R0	K11
      0x201C0807,  //  0050  NE	R7	R4	R7
      0x781E0005,  //  0051  JMPF	R7	#0058
      0x8C1C010E,  //  0052  GETMET	R7	R0	K14
      0x4C240000,  //  0053  LDNIL	R9
      0x542A02FF,  //  0054  LDINT	R10	768
      0x582C0009,  //  0055  LDCONST	R11	K9
      0x7C1C0800,  //  0056  CALL	R7	4
      0x90021604,  //  0057  SETMBR	R0	K11	R4
      0x881C010C,  //  0058  GETMBR	R7	R0	K12
      0x201C0A07,  //  0059  NE	R7	R5	R7
      0x781E0005,  //  005A  JMPF	R7	#0061
      0x8C1C010E,  //  005B  GETMET	R7	R0	K14
      0x4C240000,  //  005C  LDNIL	R9
      0x542A02FF,  //  005D  LDINT	R10	768
      0x582C000F,  //  005E  LDCONST	R11	K15
      0x7C1C0800,  //  005F  CALL	R7	4
      0x90021805,  //  0060  SETMBR	R0	K12	R5
      0x80000000,  //  0061  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Plugin_Light3_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(every_second),
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
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light3_invoke_request,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_const_int(3),
    /* K6   */  be_const_int(0),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(Matter_TLV_struct),
    /* K9   */  be_nested_str_weak(add_TLV),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_nested_str_weak(set),
    /* K12  */  be_nested_str_weak(power),
    /* K13  */  be_nested_str_weak(update_shadow),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(shadow_onoff),
    /* K16  */  be_nested_str_weak(findsubval),
    /* K17  */  be_nested_str_weak(tasmota),
    /* K18  */  be_nested_str_weak(scale_uint),
    /* K19  */  be_nested_str_weak(bri),
    /* K20  */  be_nested_str_weak(hue),
    /* K21  */  be_nested_str_weak(sat),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[285]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x1C200D05,  //  0005  EQ	R8	R6	K5
      0x78220016,  //  0006  JMPF	R8	#001E
      0x1C200F06,  //  0007  EQ	R8	R7	K6
      0x78220002,  //  0008  JMPF	R8	#000C
      0x50200200,  //  0009  LDBOOL	R8	1	0
      0x80041000,  //  000A  RET	1	R8
      0x70020010,  //  000B  JMP		#001D
      0x1C200F07,  //  000C  EQ	R8	R7	K7
      0x78220009,  //  000D  JMPF	R8	#0018
      0x8C200B08,  //  000E  GETMET	R8	R5	K8
      0x7C200200,  //  000F  CALL	R8	1
      0x8C241109,  //  0010  GETMET	R9	R8	K9
      0x582C0006,  //  0011  LDCONST	R11	K6
      0x88300B0A,  //  0012  GETMBR	R12	R5	K10
      0x58340006,  //  0013  LDCONST	R13	K6
      0x7C240800,  //  0014  CALL	R9	4
      0x900E0906,  //  0015  SETMBR	R3	K4	K6
      0x80041000,  //  0016  RET	1	R8
      0x70020004,  //  0017  JMP		#001D
      0x5422003F,  //  0018  LDINT	R8	64
      0x1C200E08,  //  0019  EQ	R8	R7	R8
      0x78220001,  //  001A  JMPF	R8	#001D
      0x50200200,  //  001B  LDBOOL	R8	1	0
      0x80041000,  //  001C  RET	1	R8
      0x700200FD,  //  001D  JMP		#011C
      0x54220003,  //  001E  LDINT	R8	4
      0x1C200C08,  //  001F  EQ	R8	R6	R8
      0x78220002,  //  0020  JMPF	R8	#0024
      0x50200200,  //  0021  LDBOOL	R8	1	0
      0x80041000,  //  0022  RET	1	R8
      0x700200F7,  //  0023  JMP		#011C
      0x54220004,  //  0024  LDINT	R8	5
      0x1C200C08,  //  0025  EQ	R8	R6	R8
      0x78220002,  //  0026  JMPF	R8	#002A
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x700200F1,  //  0029  JMP		#011C
      0x54220005,  //  002A  LDINT	R8	6
      0x1C200C08,  //  002B  EQ	R8	R6	R8
      0x78220029,  //  002C  JMPF	R8	#0057
      0x1C200F06,  //  002D  EQ	R8	R7	K6
      0x7822000A,  //  002E  JMPF	R8	#003A
      0x8C20090B,  //  002F  GETMET	R8	R4	K11
      0x60280013,  //  0030  GETGBL	R10	G19
      0x7C280000,  //  0031  CALL	R10	0
      0x502C0000,  //  0032  LDBOOL	R11	0	0
      0x982A180B,  //  0033  SETIDX	R10	K12	R11
      0x7C200400,  //  0034  CALL	R8	2
      0x8C20010D,  //  0035  GETMET	R8	R0	K13
      0x7C200200,  //  0036  CALL	R8	1
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x80041000,  //  0038  RET	1	R8
      0x7002001B,  //  0039  JMP		#0056
      0x1C200F07,  //  003A  EQ	R8	R7	K7
      0x7822000A,  //  003B  JMPF	R8	#0047
      0x8C20090B,  //  003C  GETMET	R8	R4	K11
      0x60280013,  //  003D  GETGBL	R10	G19
      0x7C280000,  //  003E  CALL	R10	0
      0x502C0200,  //  003F  LDBOOL	R11	1	0
      0x982A180B,  //  0040  SETIDX	R10	K12	R11
      0x7C200400,  //  0041  CALL	R8	2
      0x8C20010D,  //  0042  GETMET	R8	R0	K13
      0x7C200200,  //  0043  CALL	R8	1
      0x50200200,  //  0044  LDBOOL	R8	1	0
      0x80041000,  //  0045  RET	1	R8
      0x7002000E,  //  0046  JMP		#0056
      0x1C200F0E,  //  0047  EQ	R8	R7	K14
      0x7822000C,  //  0048  JMPF	R8	#0056
      0x8C20090B,  //  0049  GETMET	R8	R4	K11
      0x60280013,  //  004A  GETGBL	R10	G19
      0x7C280000,  //  004B  CALL	R10	0
      0x882C010F,  //  004C  GETMBR	R11	R0	K15
      0x782E0000,  //  004D  JMPF	R11	#004F
      0x502C0001,  //  004E  LDBOOL	R11	0	1
      0x502C0200,  //  004F  LDBOOL	R11	1	0
      0x982A180B,  //  0050  SETIDX	R10	K12	R11
      0x7C200400,  //  0051  CALL	R8	2
      0x8C20010D,  //  0052  GETMET	R8	R0	K13
      0x7C200200,  //  0053  CALL	R8	1
      0x50200200,  //  0054  LDBOOL	R8	1	0
      0x80041000,  //  0055  RET	1	R8
      0x700200C4,  //  0056  JMP		#011C
      0x54220007,  //  0057  LDINT	R8	8
      0x1C200C08,  //  0058  EQ	R8	R6	R8
      0x78220051,  //  0059  JMPF	R8	#00AC
      0x1C200F06,  //  005A  EQ	R8	R7	K6
      0x78220014,  //  005B  JMPF	R8	#0071
      0x8C200510,  //  005C  GETMET	R8	R2	K16
      0x58280006,  //  005D  LDCONST	R10	K6
      0x7C200400,  //  005E  CALL	R8	2
      0xB8262200,  //  005F  GETNGBL	R9	K17
      0x8C241312,  //  0060  GETMET	R9	R9	K18
      0x5C2C1000,  //  0061  MOVE	R11	R8
      0x58300006,  //  0062  LDCONST	R12	K6
      0x543600FD,  //  0063  LDINT	R13	254
      0x58380006,  //  0064  LDCONST	R14	K6
      0x543E0167,  //  0065  LDINT	R15	360
      0x7C240C00,  //  0066  CALL	R9	6
      0x8C28090B,  //  0067  GETMET	R10	R4	K11
      0x60300013,  //  0068  GETGBL	R12	G19
      0x7C300000,  //  0069  CALL	R12	0
      0x98322609,  //  006A  SETIDX	R12	K19	R9
      0x7C280400,  //  006B  CALL	R10	2
      0x8C28010D,  //  006C  GETMET	R10	R0	K13
      0x7C280200,  //  006D  CALL	R10	1
      0x50280200,  //  006E  LDBOOL	R10	1	0
      0x80041400,  //  006F  RET	1	R10
      0x70020039,  //  0070  JMP		#00AB
      0x1C200F07,  //  0071  EQ	R8	R7	K7
      0x78220002,  //  0072  JMPF	R8	#0076
      0x50200200,  //  0073  LDBOOL	R8	1	0
      0x80041000,  //  0074  RET	1	R8
      0x70020034,  //  0075  JMP		#00AB
      0x1C200F0E,  //  0076  EQ	R8	R7	K14
      0x78220002,  //  0077  JMPF	R8	#007B
      0x50200200,  //  0078  LDBOOL	R8	1	0
      0x80041000,  //  0079  RET	1	R8
      0x7002002F,  //  007A  JMP		#00AB
      0x1C200F05,  //  007B  EQ	R8	R7	K5
      0x78220002,  //  007C  JMPF	R8	#0080
      0x50200200,  //  007D  LDBOOL	R8	1	0
      0x80041000,  //  007E  RET	1	R8
      0x7002002A,  //  007F  JMP		#00AB
      0x54220003,  //  0080  LDINT	R8	4
      0x1C200E08,  //  0081  EQ	R8	R7	R8
      0x78220016,  //  0082  JMPF	R8	#009A
      0x8C200510,  //  0083  GETMET	R8	R2	K16
      0x58280006,  //  0084  LDCONST	R10	K6
      0x7C200400,  //  0085  CALL	R8	2
      0xB8262200,  //  0086  GETNGBL	R9	K17
      0x8C241312,  //  0087  GETMET	R9	R9	K18
      0x5C2C1000,  //  0088  MOVE	R11	R8
      0x58300006,  //  0089  LDCONST	R12	K6
      0x543600FD,  //  008A  LDINT	R13	254
      0x58380006,  //  008B  LDCONST	R14	K6
      0x543E0167,  //  008C  LDINT	R15	360
      0x7C240C00,  //  008D  CALL	R9	6
      0x24281306,  //  008E  GT	R10	R9	K6
      0x8C2C090B,  //  008F  GETMET	R11	R4	K11
      0x60340013,  //  0090  GETGBL	R13	G19
      0x7C340000,  //  0091  CALL	R13	0
      0x98362609,  //  0092  SETIDX	R13	K19	R9
      0x9836180A,  //  0093  SETIDX	R13	K12	R10
      0x7C2C0400,  //  0094  CALL	R11	2
      0x8C2C010D,  //  0095  GETMET	R11	R0	K13
      0x7C2C0200,  //  0096  CALL	R11	1
      0x502C0200,  //  0097  LDBOOL	R11	1	0
      0x80041600,  //  0098  RET	1	R11
      0x70020010,  //  0099  JMP		#00AB
      0x54220004,  //  009A  LDINT	R8	5
      0x1C200E08,  //  009B  EQ	R8	R7	R8
      0x78220002,  //  009C  JMPF	R8	#00A0
      0x50200200,  //  009D  LDBOOL	R8	1	0
      0x80041000,  //  009E  RET	1	R8
      0x7002000A,  //  009F  JMP		#00AB
      0x54220005,  //  00A0  LDINT	R8	6
      0x1C200E08,  //  00A1  EQ	R8	R7	R8
      0x78220002,  //  00A2  JMPF	R8	#00A6
      0x50200200,  //  00A3  LDBOOL	R8	1	0
      0x80041000,  //  00A4  RET	1	R8
      0x70020004,  //  00A5  JMP		#00AB
      0x54220006,  //  00A6  LDINT	R8	7
      0x1C200E08,  //  00A7  EQ	R8	R7	R8
      0x78220001,  //  00A8  JMPF	R8	#00AB
      0x50200200,  //  00A9  LDBOOL	R8	1	0
      0x80041000,  //  00AA  RET	1	R8
      0x7002006F,  //  00AB  JMP		#011C
      0x542202FF,  //  00AC  LDINT	R8	768
      0x1C200C08,  //  00AD  EQ	R8	R6	R8
      0x7822006C,  //  00AE  JMPF	R8	#011C
      0x1C200F06,  //  00AF  EQ	R8	R7	K6
      0x78220014,  //  00B0  JMPF	R8	#00C6
      0x8C200510,  //  00B1  GETMET	R8	R2	K16
      0x58280006,  //  00B2  LDCONST	R10	K6
      0x7C200400,  //  00B3  CALL	R8	2
      0xB8262200,  //  00B4  GETNGBL	R9	K17
      0x8C241312,  //  00B5  GETMET	R9	R9	K18
      0x5C2C1000,  //  00B6  MOVE	R11	R8
      0x58300006,  //  00B7  LDCONST	R12	K6
      0x543600FD,  //  00B8  LDINT	R13	254
      0x58380006,  //  00B9  LDCONST	R14	K6
      0x543E0167,  //  00BA  LDINT	R15	360
      0x7C240C00,  //  00BB  CALL	R9	6
      0x8C28090B,  //  00BC  GETMET	R10	R4	K11
      0x60300013,  //  00BD  GETGBL	R12	G19
      0x7C300000,  //  00BE  CALL	R12	0
      0x98322809,  //  00BF  SETIDX	R12	K20	R9
      0x7C280400,  //  00C0  CALL	R10	2
      0x8C28010D,  //  00C1  GETMET	R10	R0	K13
      0x7C280200,  //  00C2  CALL	R10	1
      0x50280200,  //  00C3  LDBOOL	R10	1	0
      0x80041400,  //  00C4  RET	1	R10
      0x70020055,  //  00C5  JMP		#011C
      0x1C200F07,  //  00C6  EQ	R8	R7	K7
      0x78220002,  //  00C7  JMPF	R8	#00CB
      0x50200200,  //  00C8  LDBOOL	R8	1	0
      0x80041000,  //  00C9  RET	1	R8
      0x70020050,  //  00CA  JMP		#011C
      0x1C200F0E,  //  00CB  EQ	R8	R7	K14
      0x78220002,  //  00CC  JMPF	R8	#00D0
      0x50200200,  //  00CD  LDBOOL	R8	1	0
      0x80041000,  //  00CE  RET	1	R8
      0x7002004B,  //  00CF  JMP		#011C
      0x1C200F05,  //  00D0  EQ	R8	R7	K5
      0x78220014,  //  00D1  JMPF	R8	#00E7
      0x8C200510,  //  00D2  GETMET	R8	R2	K16
      0x58280006,  //  00D3  LDCONST	R10	K6
      0x7C200400,  //  00D4  CALL	R8	2
      0xB8262200,  //  00D5  GETNGBL	R9	K17
      0x8C241312,  //  00D6  GETMET	R9	R9	K18
      0x5C2C1000,  //  00D7  MOVE	R11	R8
      0x58300006,  //  00D8  LDCONST	R12	K6
      0x543600FD,  //  00D9  LDINT	R13	254
      0x58380006,  //  00DA  LDCONST	R14	K6
      0x543E00FE,  //  00DB  LDINT	R15	255
      0x7C240C00,  //  00DC  CALL	R9	6
      0x8C28090B,  //  00DD  GETMET	R10	R4	K11
      0x60300013,  //  00DE  GETGBL	R12	G19
      0x7C300000,  //  00DF  CALL	R12	0
      0x98322A09,  //  00E0  SETIDX	R12	K21	R9
      0x7C280400,  //  00E1  CALL	R10	2
      0x8C28010D,  //  00E2  GETMET	R10	R0	K13
      0x7C280200,  //  00E3  CALL	R10	1
      0x50280200,  //  00E4  LDBOOL	R10	1	0
      0x80041400,  //  00E5  RET	1	R10
      0x70020034,  //  00E6  JMP		#011C
      0x54220003,  //  00E7  LDINT	R8	4
      0x1C200E08,  //  00E8  EQ	R8	R7	R8
      0x78220002,  //  00E9  JMPF	R8	#00ED
      0x50200200,  //  00EA  LDBOOL	R8	1	0
      0x80041000,  //  00EB  RET	1	R8
      0x7002002E,  //  00EC  JMP		#011C
      0x54220004,  //  00ED  LDINT	R8	5
      0x1C200E08,  //  00EE  EQ	R8	R7	R8
      0x78220002,  //  00EF  JMPF	R8	#00F3
      0x50200200,  //  00F0  LDBOOL	R8	1	0
      0x80041000,  //  00F1  RET	1	R8
      0x70020028,  //  00F2  JMP		#011C
      0x54220005,  //  00F3  LDINT	R8	6
      0x1C200E08,  //  00F4  EQ	R8	R7	R8
      0x78220020,  //  00F5  JMPF	R8	#0117
      0x8C200510,  //  00F6  GETMET	R8	R2	K16
      0x58280006,  //  00F7  LDCONST	R10	K6
      0x7C200400,  //  00F8  CALL	R8	2
      0xB8262200,  //  00F9  GETNGBL	R9	K17
      0x8C241312,  //  00FA  GETMET	R9	R9	K18
      0x5C2C1000,  //  00FB  MOVE	R11	R8
      0x58300006,  //  00FC  LDCONST	R12	K6
      0x543600FD,  //  00FD  LDINT	R13	254
      0x58380006,  //  00FE  LDCONST	R14	K6
      0x543E0167,  //  00FF  LDINT	R15	360
      0x7C240C00,  //  0100  CALL	R9	6
      0x8C280510,  //  0101  GETMET	R10	R2	K16
      0x58300007,  //  0102  LDCONST	R12	K7
      0x7C280400,  //  0103  CALL	R10	2
      0xB82E2200,  //  0104  GETNGBL	R11	K17
      0x8C2C1712,  //  0105  GETMET	R11	R11	K18
      0x5C341400,  //  0106  MOVE	R13	R10
      0x58380006,  //  0107  LDCONST	R14	K6
      0x543E00FD,  //  0108  LDINT	R15	254
      0x58400006,  //  0109  LDCONST	R16	K6
      0x544600FE,  //  010A  LDINT	R17	255
      0x7C2C0C00,  //  010B  CALL	R11	6
      0x8C30090B,  //  010C  GETMET	R12	R4	K11
      0x60380013,  //  010D  GETGBL	R14	G19
      0x7C380000,  //  010E  CALL	R14	0
      0x983A2809,  //  010F  SETIDX	R14	K20	R9
      0x983A2A0B,  //  0110  SETIDX	R14	K21	R11
      0x7C300400,  //  0111  CALL	R12	2
      0x8C30010D,  //  0112  GETMET	R12	R0	K13
      0x7C300200,  //  0113  CALL	R12	1
      0x50300200,  //  0114  LDBOOL	R12	1	0
      0x80041800,  //  0115  RET	1	R12
      0x70020004,  //  0116  JMP		#011C
      0x54220046,  //  0117  LDINT	R8	71
      0x1C200E08,  //  0118  EQ	R8	R7	R8
      0x78220001,  //  0119  JMPF	R8	#011C
      0x50200200,  //  011A  LDBOOL	R8	1	0
      0x80041000,  //  011B  RET	1	R8
      0x80000000,  //  011C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light3
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Light3,
    4,
    &be_class_Matter_Plugin,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(269, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(shadow_bri, 7), be_const_var(1) },
        { be_const_key_weak(onoff_changed, 0), be_const_closure(Matter_Plugin_Light3_onoff_changed_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light3_init_closure) },
        { be_const_key_weak(shadow_hue, 8), be_const_var(0) },
        { be_const_key_weak(shadow_sat, -1), be_const_var(2) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light3_read_attribute_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Light3_update_shadow_closure) },
        { be_const_key_weak(shadow_onoff, -1), be_const_var(3) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Plugin_Light3_every_second_closure) },
        { be_const_key_weak(CLUSTERS, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(6, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(768, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(7),
        be_const_int(8),
        be_const_int(15),
        be_const_int(16385),
        be_const_int(16394),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(8, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(2),
        be_const_int(3),
        be_const_int(15),
        be_const_int(17),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light3_invoke_request_closure) },
    })),
    be_str_weak(Matter_Plugin_Light3)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light3_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light3);
    be_setglobal(vm, "Matter_Plugin_Light3");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
