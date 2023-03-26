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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_hue),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x90020302,  //  0007  SETMBR	R0	K1	K2
      0x80000000,  //  0008  RET	0
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
    ( &(const bvalue[17]) {     /* constants */
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
    /* K14  */  be_nested_str_weak(shadow_hue),
    /* K15  */  be_nested_str_weak(shadow_sat),
    /* K16  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[255]) {  /* code */
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
      0x700200D4,  //  0028  JMP		#00FE
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
      0x700200BA,  //  0042  JMP		#00FE
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
      0x700200A5,  //  0057  JMP		#00FE
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
      0x70020088,  //  0074  JMP		#00FE
      0x541E0007,  //  0075  LDINT	R7	8
      0x1C1C0A07,  //  0076  EQ	R7	R5	R7
      0x781E002B,  //  0077  JMPF	R7	#00A4
      0x1C1C0D06,  //  0078  EQ	R7	R6	K6
      0x781E0005,  //  0079  JMPF	R7	#0080
      0x8C1C0907,  //  007A  GETMET	R7	R4	K7
      0x8824090A,  //  007B  GETMBR	R9	R4	K10
      0x542A0087,  //  007C  LDINT	R10	136
      0x7C1C0600,  //  007D  CALL	R7	3
      0x80040E00,  //  007E  RET	1	R7
      0x70020022,  //  007F  JMP		#00A3
      0x541E000E,  //  0080  LDINT	R7	15
      0x1C1C0C07,  //  0081  EQ	R7	R6	R7
      0x781E0005,  //  0082  JMPF	R7	#0089
      0x8C1C0907,  //  0083  GETMET	R7	R4	K7
      0x8824090A,  //  0084  GETMBR	R9	R4	K10
      0x58280006,  //  0085  LDCONST	R10	K6
      0x7C1C0600,  //  0086  CALL	R7	3
      0x80040E00,  //  0087  RET	1	R7
      0x70020019,  //  0088  JMP		#00A3
      0x541E0010,  //  0089  LDINT	R7	17
      0x1C1C0C07,  //  008A  EQ	R7	R6	R7
      0x781E0005,  //  008B  JMPF	R7	#0092
      0x8C1C0907,  //  008C  GETMET	R7	R4	K7
      0x8824090A,  //  008D  GETMBR	R9	R4	K10
      0x58280009,  //  008E  LDCONST	R10	K9
      0x7C1C0600,  //  008F  CALL	R7	3
      0x80040E00,  //  0090  RET	1	R7
      0x70020010,  //  0091  JMP		#00A3
      0x541EFFFB,  //  0092  LDINT	R7	65532
      0x1C1C0C07,  //  0093  EQ	R7	R6	R7
      0x781E0005,  //  0094  JMPF	R7	#009B
      0x8C1C0907,  //  0095  GETMET	R7	R4	K7
      0x8824090B,  //  0096  GETMBR	R9	R4	K11
      0x58280009,  //  0097  LDCONST	R10	K9
      0x7C1C0600,  //  0098  CALL	R7	3
      0x80040E00,  //  0099  RET	1	R7
      0x70020007,  //  009A  JMP		#00A3
      0x541EFFFC,  //  009B  LDINT	R7	65533
      0x1C1C0C07,  //  009C  EQ	R7	R6	R7
      0x781E0004,  //  009D  JMPF	R7	#00A3
      0x8C1C0907,  //  009E  GETMET	R7	R4	K7
      0x8824090B,  //  009F  GETMBR	R9	R4	K11
      0x542A0004,  //  00A0  LDINT	R10	5
      0x7C1C0600,  //  00A1  CALL	R7	3
      0x80040E00,  //  00A2  RET	1	R7
      0x70020059,  //  00A3  JMP		#00FE
      0x541E02FF,  //  00A4  LDINT	R7	768
      0x1C1C0A07,  //  00A5  EQ	R7	R5	R7
      0x781E004E,  //  00A6  JMPF	R7	#00F6
      0x1C1C0D06,  //  00A7  EQ	R7	R6	K6
      0x781E0005,  //  00A8  JMPF	R7	#00AF
      0x8C1C0907,  //  00A9  GETMET	R7	R4	K7
      0x8824090A,  //  00AA  GETMBR	R9	R4	K10
      0x8828010E,  //  00AB  GETMBR	R10	R0	K14
      0x7C1C0600,  //  00AC  CALL	R7	3
      0x80040E00,  //  00AD  RET	1	R7
      0x70020045,  //  00AE  JMP		#00F5
      0x1C1C0D09,  //  00AF  EQ	R7	R6	K9
      0x781E0005,  //  00B0  JMPF	R7	#00B7
      0x8C1C0907,  //  00B1  GETMET	R7	R4	K7
      0x8824090A,  //  00B2  GETMBR	R9	R4	K10
      0x8828010F,  //  00B3  GETMBR	R10	R0	K15
      0x7C1C0600,  //  00B4  CALL	R7	3
      0x80040E00,  //  00B5  RET	1	R7
      0x7002003D,  //  00B6  JMP		#00F5
      0x541E0006,  //  00B7  LDINT	R7	7
      0x1C1C0C07,  //  00B8  EQ	R7	R6	R7
      0x781E0005,  //  00B9  JMPF	R7	#00C0
      0x8C1C0907,  //  00BA  GETMET	R7	R4	K7
      0x8824090A,  //  00BB  GETMBR	R9	R4	K10
      0x58280006,  //  00BC  LDCONST	R10	K6
      0x7C1C0600,  //  00BD  CALL	R7	3
      0x80040E00,  //  00BE  RET	1	R7
      0x70020034,  //  00BF  JMP		#00F5
      0x541E0007,  //  00C0  LDINT	R7	8
      0x1C1C0C07,  //  00C1  EQ	R7	R6	R7
      0x781E0005,  //  00C2  JMPF	R7	#00C9
      0x8C1C0907,  //  00C3  GETMET	R7	R4	K7
      0x8824090A,  //  00C4  GETMBR	R9	R4	K10
      0x58280006,  //  00C5  LDCONST	R10	K6
      0x7C1C0600,  //  00C6  CALL	R7	3
      0x80040E00,  //  00C7  RET	1	R7
      0x7002002B,  //  00C8  JMP		#00F5
      0x541E000E,  //  00C9  LDINT	R7	15
      0x1C1C0C07,  //  00CA  EQ	R7	R6	R7
      0x781E0005,  //  00CB  JMPF	R7	#00D2
      0x8C1C0907,  //  00CC  GETMET	R7	R4	K7
      0x8824090A,  //  00CD  GETMBR	R9	R4	K10
      0x58280006,  //  00CE  LDCONST	R10	K6
      0x7C1C0600,  //  00CF  CALL	R7	3
      0x80040E00,  //  00D0  RET	1	R7
      0x70020022,  //  00D1  JMP		#00F5
      0x541E4000,  //  00D2  LDINT	R7	16385
      0x1C1C0C07,  //  00D3  EQ	R7	R6	R7
      0x781E0005,  //  00D4  JMPF	R7	#00DB
      0x8C1C0907,  //  00D5  GETMET	R7	R4	K7
      0x8824090A,  //  00D6  GETMBR	R9	R4	K10
      0x58280006,  //  00D7  LDCONST	R10	K6
      0x7C1C0600,  //  00D8  CALL	R7	3
      0x80040E00,  //  00D9  RET	1	R7
      0x70020019,  //  00DA  JMP		#00F5
      0x541E4009,  //  00DB  LDINT	R7	16394
      0x1C1C0C07,  //  00DC  EQ	R7	R6	R7
      0x781E0005,  //  00DD  JMPF	R7	#00E4
      0x8C1C0907,  //  00DE  GETMET	R7	R4	K7
      0x8824090A,  //  00DF  GETMBR	R9	R4	K10
      0x58280006,  //  00E0  LDCONST	R10	K6
      0x7C1C0600,  //  00E1  CALL	R7	3
      0x80040E00,  //  00E2  RET	1	R7
      0x70020010,  //  00E3  JMP		#00F5
      0x541EFFFB,  //  00E4  LDINT	R7	65532
      0x1C1C0C07,  //  00E5  EQ	R7	R6	R7
      0x781E0005,  //  00E6  JMPF	R7	#00ED
      0x8C1C0907,  //  00E7  GETMET	R7	R4	K7
      0x8824090B,  //  00E8  GETMBR	R9	R4	K11
      0x58280009,  //  00E9  LDCONST	R10	K9
      0x7C1C0600,  //  00EA  CALL	R7	3
      0x80040E00,  //  00EB  RET	1	R7
      0x70020007,  //  00EC  JMP		#00F5
      0x541EFFFC,  //  00ED  LDINT	R7	65533
      0x1C1C0C07,  //  00EE  EQ	R7	R6	R7
      0x781E0004,  //  00EF  JMPF	R7	#00F5
      0x8C1C0907,  //  00F0  GETMET	R7	R4	K7
      0x8824090B,  //  00F1  GETMBR	R9	R4	K11
      0x542A0004,  //  00F2  LDINT	R10	5
      0x7C1C0600,  //  00F3  CALL	R7	3
      0x80040E00,  //  00F4  RET	1	R7
      0x70020007,  //  00F5  JMP		#00FE
      0x601C0003,  //  00F6  GETGBL	R7	G3
      0x5C200000,  //  00F7  MOVE	R8	R0
      0x7C1C0200,  //  00F8  CALL	R7	1
      0x8C1C0F10,  //  00F9  GETMET	R7	R7	K16
      0x5C240200,  //  00FA  MOVE	R9	R1
      0x5C280400,  //  00FB  MOVE	R10	R2
      0x7C1C0600,  //  00FC  CALL	R7	3
      0x80040E00,  //  00FD  RET	1	R7
      0x80000000,  //  00FE  RET	0
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
    /* K7   */  be_nested_str_weak(shadow_bri),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(scale_uint),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(shadow_hue),
    /* K12  */  be_nested_str_weak(shadow_sat),
    /* K13  */  be_nested_str_weak(shadow_onoff),
    /* K14  */  be_nested_str_weak(attribute_updated),
    /* K15  */  be_const_int(1),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[92]) {  /* code */
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
      0x781E0008,  //  0015  JMPF	R7	#001F
      0xB81E1000,  //  0016  GETNGBL	R7	K8
      0x8C1C0F09,  //  0017  GETMET	R7	R7	K9
      0x5C240600,  //  0018  MOVE	R9	R3
      0x5828000A,  //  0019  LDCONST	R10	K10
      0x542E00FE,  //  001A  LDINT	R11	255
      0x5830000A,  //  001B  LDCONST	R12	K10
      0x543600FD,  //  001C  LDINT	R13	254
      0x7C1C0C00,  //  001D  CALL	R7	6
      0x90020E07,  //  001E  SETMBR	R0	K7	R7
      0x4C1C0000,  //  001F  LDNIL	R7
      0x201C0807,  //  0020  NE	R7	R4	R7
      0x781E0008,  //  0021  JMPF	R7	#002B
      0xB81E1000,  //  0022  GETNGBL	R7	K8
      0x8C1C0F09,  //  0023  GETMET	R7	R7	K9
      0x5C240800,  //  0024  MOVE	R9	R4
      0x5828000A,  //  0025  LDCONST	R10	K10
      0x542E0167,  //  0026  LDINT	R11	360
      0x5830000A,  //  0027  LDCONST	R12	K10
      0x543600FD,  //  0028  LDINT	R13	254
      0x7C1C0C00,  //  0029  CALL	R7	6
      0x90021607,  //  002A  SETMBR	R0	K11	R7
      0x4C1C0000,  //  002B  LDNIL	R7
      0x201C0A07,  //  002C  NE	R7	R5	R7
      0x781E0008,  //  002D  JMPF	R7	#0037
      0xB81E1000,  //  002E  GETNGBL	R7	K8
      0x8C1C0F09,  //  002F  GETMET	R7	R7	K9
      0x5C240A00,  //  0030  MOVE	R9	R5
      0x5828000A,  //  0031  LDCONST	R10	K10
      0x542E00FE,  //  0032  LDINT	R11	255
      0x5830000A,  //  0033  LDCONST	R12	K10
      0x543600FD,  //  0034  LDINT	R13	254
      0x7C1C0C00,  //  0035  CALL	R7	6
      0x90021807,  //  0036  SETMBR	R0	K12	R7
      0x881C010D,  //  0037  GETMBR	R7	R0	K13
      0x201C0C07,  //  0038  NE	R7	R6	R7
      0x781E0005,  //  0039  JMPF	R7	#0040
      0x8C1C010E,  //  003A  GETMET	R7	R0	K14
      0x4C240000,  //  003B  LDNIL	R9
      0x542A0005,  //  003C  LDINT	R10	6
      0x582C000A,  //  003D  LDCONST	R11	K10
      0x7C1C0800,  //  003E  CALL	R7	4
      0x90021A06,  //  003F  SETMBR	R0	K13	R6
      0x881C0107,  //  0040  GETMBR	R7	R0	K7
      0x201C0607,  //  0041  NE	R7	R3	R7
      0x781E0005,  //  0042  JMPF	R7	#0049
      0x8C1C010E,  //  0043  GETMET	R7	R0	K14
      0x4C240000,  //  0044  LDNIL	R9
      0x542A0007,  //  0045  LDINT	R10	8
      0x582C000A,  //  0046  LDCONST	R11	K10
      0x7C1C0800,  //  0047  CALL	R7	4
      0x90020E03,  //  0048  SETMBR	R0	K7	R3
      0x881C010B,  //  0049  GETMBR	R7	R0	K11
      0x201C0807,  //  004A  NE	R7	R4	R7
      0x781E0005,  //  004B  JMPF	R7	#0052
      0x8C1C010E,  //  004C  GETMET	R7	R0	K14
      0x4C240000,  //  004D  LDNIL	R9
      0x542A02FF,  //  004E  LDINT	R10	768
      0x582C000A,  //  004F  LDCONST	R11	K10
      0x7C1C0800,  //  0050  CALL	R7	4
      0x90021604,  //  0051  SETMBR	R0	K11	R4
      0x881C010C,  //  0052  GETMBR	R7	R0	K12
      0x201C0A07,  //  0053  NE	R7	R5	R7
      0x781E0005,  //  0054  JMPF	R7	#005B
      0x8C1C010E,  //  0055  GETMET	R7	R0	K14
      0x4C240000,  //  0056  LDNIL	R9
      0x542A02FF,  //  0057  LDINT	R10	768
      0x582C000F,  //  0058  LDCONST	R11	K15
      0x7C1C0800,  //  0059  CALL	R7	4
      0x90021805,  //  005A  SETMBR	R0	K12	R5
      0x80000000,  //  005B  RET	0
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
    13,                          /* nstack */
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
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_const_int(3),
    /* K5   */  be_const_int(0),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(Matter_TLV_struct),
    /* K8   */  be_nested_str_weak(add_TLV),
    /* K9   */  be_nested_str_weak(U2),
    /* K10  */  be_nested_str_weak(set_onoff),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(get_onoff),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[119]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x1C1C0B04,  //  0004  EQ	R7	R5	K4
      0x781E0016,  //  0005  JMPF	R7	#001D
      0x1C1C0D05,  //  0006  EQ	R7	R6	K5
      0x781E0002,  //  0007  JMPF	R7	#000B
      0x501C0200,  //  0008  LDBOOL	R7	1	0
      0x80040E00,  //  0009  RET	1	R7
      0x70020010,  //  000A  JMP		#001C
      0x1C1C0D06,  //  000B  EQ	R7	R6	K6
      0x781E0009,  //  000C  JMPF	R7	#0017
      0x8C1C0907,  //  000D  GETMET	R7	R4	K7
      0x7C1C0200,  //  000E  CALL	R7	1
      0x8C200F08,  //  000F  GETMET	R8	R7	K8
      0x58280005,  //  0010  LDCONST	R10	K5
      0x882C0909,  //  0011  GETMBR	R11	R4	K9
      0x58300005,  //  0012  LDCONST	R12	K5
      0x7C200800,  //  0013  CALL	R8	4
      0x900E0705,  //  0014  SETMBR	R3	K3	K5
      0x80040E00,  //  0015  RET	1	R7
      0x70020004,  //  0016  JMP		#001C
      0x541E003F,  //  0017  LDINT	R7	64
      0x1C1C0C07,  //  0018  EQ	R7	R6	R7
      0x781E0001,  //  0019  JMPF	R7	#001C
      0x501C0200,  //  001A  LDBOOL	R7	1	0
      0x80040E00,  //  001B  RET	1	R7
      0x70020058,  //  001C  JMP		#0076
      0x541E0003,  //  001D  LDINT	R7	4
      0x1C1C0A07,  //  001E  EQ	R7	R5	R7
      0x781E0002,  //  001F  JMPF	R7	#0023
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x80040E00,  //  0021  RET	1	R7
      0x70020052,  //  0022  JMP		#0076
      0x541E0004,  //  0023  LDINT	R7	5
      0x1C1C0A07,  //  0024  EQ	R7	R5	R7
      0x781E0002,  //  0025  JMPF	R7	#0029
      0x501C0200,  //  0026  LDBOOL	R7	1	0
      0x80040E00,  //  0027  RET	1	R7
      0x7002004C,  //  0028  JMP		#0076
      0x541E0005,  //  0029  LDINT	R7	6
      0x1C1C0A07,  //  002A  EQ	R7	R5	R7
      0x781E001B,  //  002B  JMPF	R7	#0048
      0x1C1C0D05,  //  002C  EQ	R7	R6	K5
      0x781E0005,  //  002D  JMPF	R7	#0034
      0x8C1C010A,  //  002E  GETMET	R7	R0	K10
      0x50240000,  //  002F  LDBOOL	R9	0	0
      0x7C1C0400,  //  0030  CALL	R7	2
      0x501C0200,  //  0031  LDBOOL	R7	1	0
      0x80040E00,  //  0032  RET	1	R7
      0x70020012,  //  0033  JMP		#0047
      0x1C1C0D06,  //  0034  EQ	R7	R6	K6
      0x781E0005,  //  0035  JMPF	R7	#003C
      0x8C1C010A,  //  0036  GETMET	R7	R0	K10
      0x50240200,  //  0037  LDBOOL	R9	1	0
      0x7C1C0400,  //  0038  CALL	R7	2
      0x501C0200,  //  0039  LDBOOL	R7	1	0
      0x80040E00,  //  003A  RET	1	R7
      0x7002000A,  //  003B  JMP		#0047
      0x1C1C0D0B,  //  003C  EQ	R7	R6	K11
      0x781E0008,  //  003D  JMPF	R7	#0047
      0x8C1C010A,  //  003E  GETMET	R7	R0	K10
      0x8C24010C,  //  003F  GETMET	R9	R0	K12
      0x7C240200,  //  0040  CALL	R9	1
      0x78260000,  //  0041  JMPF	R9	#0043
      0x50240001,  //  0042  LDBOOL	R9	0	1
      0x50240200,  //  0043  LDBOOL	R9	1	0
      0x7C1C0400,  //  0044  CALL	R7	2
      0x501C0200,  //  0045  LDBOOL	R7	1	0
      0x80040E00,  //  0046  RET	1	R7
      0x7002002D,  //  0047  JMP		#0076
      0x541E0007,  //  0048  LDINT	R7	8
      0x1C1C0A07,  //  0049  EQ	R7	R5	R7
      0x781E002A,  //  004A  JMPF	R7	#0076
      0x1C1C0D05,  //  004B  EQ	R7	R6	K5
      0x781E0002,  //  004C  JMPF	R7	#0050
      0x501C0200,  //  004D  LDBOOL	R7	1	0
      0x80040E00,  //  004E  RET	1	R7
      0x70020025,  //  004F  JMP		#0076
      0x1C1C0D06,  //  0050  EQ	R7	R6	K6
      0x781E0002,  //  0051  JMPF	R7	#0055
      0x501C0200,  //  0052  LDBOOL	R7	1	0
      0x80040E00,  //  0053  RET	1	R7
      0x70020020,  //  0054  JMP		#0076
      0x1C1C0D0B,  //  0055  EQ	R7	R6	K11
      0x781E0002,  //  0056  JMPF	R7	#005A
      0x501C0200,  //  0057  LDBOOL	R7	1	0
      0x80040E00,  //  0058  RET	1	R7
      0x7002001B,  //  0059  JMP		#0076
      0x1C1C0D04,  //  005A  EQ	R7	R6	K4
      0x781E0002,  //  005B  JMPF	R7	#005F
      0x501C0200,  //  005C  LDBOOL	R7	1	0
      0x80040E00,  //  005D  RET	1	R7
      0x70020016,  //  005E  JMP		#0076
      0x541E0003,  //  005F  LDINT	R7	4
      0x1C1C0C07,  //  0060  EQ	R7	R6	R7
      0x781E0002,  //  0061  JMPF	R7	#0065
      0x501C0200,  //  0062  LDBOOL	R7	1	0
      0x80040E00,  //  0063  RET	1	R7
      0x70020010,  //  0064  JMP		#0076
      0x541E0004,  //  0065  LDINT	R7	5
      0x1C1C0C07,  //  0066  EQ	R7	R6	R7
      0x781E0002,  //  0067  JMPF	R7	#006B
      0x501C0200,  //  0068  LDBOOL	R7	1	0
      0x80040E00,  //  0069  RET	1	R7
      0x7002000A,  //  006A  JMP		#0076
      0x541E0005,  //  006B  LDINT	R7	6
      0x1C1C0C07,  //  006C  EQ	R7	R6	R7
      0x781E0002,  //  006D  JMPF	R7	#0071
      0x501C0200,  //  006E  LDBOOL	R7	1	0
      0x80040E00,  //  006F  RET	1	R7
      0x70020004,  //  0070  JMP		#0076
      0x541E0006,  //  0071  LDINT	R7	7
      0x1C1C0C07,  //  0072  EQ	R7	R6	R7
      0x781E0001,  //  0073  JMPF	R7	#0076
      0x501C0200,  //  0074  LDBOOL	R7	1	0
      0x80040E00,  //  0075  RET	1	R7
      0x80000000,  //  0076  RET	0
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
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
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
