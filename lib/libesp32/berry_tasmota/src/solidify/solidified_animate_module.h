/* Solidification of animate_module.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Animate_palette;

/********************************************************************
** Solidified function: to_css_gradient
********************************************************************/
be_local_closure(Animate_palette_to_css_gradient,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(parse_palette),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(background_X3Alinear_X2Dgradient_X28to_X20right),
    /* K3   */  be_nested_str_weak(palette),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_nested_str_weak(_X2C_X23_X2502X_X2502X_X2502X_X20_X25_X2E1f_X25_X25),
    /* K6   */  be_const_real_hex(0x41200000),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(_X29_X3B),
    }),
    be_str_weak(to_css_gradient),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x541203E7,  //  0002  LDINT	R4	1000
      0x7C040600,  //  0003  CALL	R1	3
      0x58080002,  //  0004  LDCONST	R2	K2
      0x580C0001,  //  0005  LDCONST	R3	K1
      0x6010000C,  //  0006  GETGBL	R4	G12
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C100200,  //  0008  CALL	R4	1
      0x14100604,  //  0009  LT	R4	R3	R4
      0x7812001C,  //  000A  JMPF	R4	#0028
      0x94100203,  //  000B  GETIDX	R4	R1	R3
      0x88140103,  //  000C  GETMBR	R5	R0	K3
      0x8C140B04,  //  000D  GETMET	R5	R5	K4
      0x541E0003,  //  000E  LDINT	R7	4
      0x081C0607,  //  000F  MUL	R7	R3	R7
      0x54220003,  //  0010  LDINT	R8	4
      0x7C140600,  //  0011  CALL	R5	3
      0x541A0007,  //  0012  LDINT	R6	8
      0x3C180A06,  //  0013  SHR	R6	R5	R6
      0x541E00FE,  //  0014  LDINT	R7	255
      0x2C180C07,  //  0015  AND	R6	R6	R7
      0x541E000F,  //  0016  LDINT	R7	16
      0x3C1C0A07,  //  0017  SHR	R7	R5	R7
      0x542200FE,  //  0018  LDINT	R8	255
      0x2C1C0E08,  //  0019  AND	R7	R7	R8
      0x54220017,  //  001A  LDINT	R8	24
      0x3C200A08,  //  001B  SHR	R8	R5	R8
      0x542600FE,  //  001C  LDINT	R9	255
      0x2C201009,  //  001D  AND	R8	R8	R9
      0x60240018,  //  001E  GETGBL	R9	G24
      0x58280005,  //  001F  LDCONST	R10	K5
      0x5C2C0C00,  //  0020  MOVE	R11	R6
      0x5C300E00,  //  0021  MOVE	R12	R7
      0x5C341000,  //  0022  MOVE	R13	R8
      0x0C380906,  //  0023  DIV	R14	R4	K6
      0x7C240A00,  //  0024  CALL	R9	5
      0x00080409,  //  0025  ADD	R2	R2	R9
      0x000C0707,  //  0026  ADD	R3	R3	K7
      0x7001FFDD,  //  0027  JMP		#0006
      0x00080508,  //  0028  ADD	R2	R2	K8
      0x80040400,  //  0029  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_palette
********************************************************************/
be_local_closure(Animate_palette_parse_palette,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(slots),
    /* K1   */  be_nested_str_weak(resize),
    /* K2   */  be_nested_str_weak(palette),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_const_int(0),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(scale_int),
    }),
    be_str_weak(parse_palette),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0x600C0012,  //  0000  GETGBL	R3	G18
      0x7C0C0000,  //  0001  CALL	R3	0
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x8C140701,  //  0003  GETMET	R5	R3	K1
      0x5C1C0800,  //  0004  MOVE	R7	R4
      0x7C140400,  //  0005  CALL	R5	2
      0x88140102,  //  0006  GETMBR	R5	R0	K2
      0x8C140B03,  //  0007  GETMET	R5	R5	K3
      0x581C0004,  //  0008  LDCONST	R7	K4
      0x58200005,  //  0009  LDCONST	R8	K5
      0x7C140600,  //  000A  CALL	R5	3
      0x20140B04,  //  000B  NE	R5	R5	K4
      0x78160024,  //  000C  JMPF	R5	#0032
      0x58140004,  //  000D  LDCONST	R5	K4
      0x58180004,  //  000E  LDCONST	R6	K4
      0x041C0905,  //  000F  SUB	R7	R4	K5
      0x141C0C07,  //  0010  LT	R7	R6	R7
      0x781E0008,  //  0011  JMPF	R7	#001B
      0x881C0102,  //  0012  GETMBR	R7	R0	K2
      0x8C1C0F03,  //  0013  GETMET	R7	R7	K3
      0x54260003,  //  0014  LDINT	R9	4
      0x08240C09,  //  0015  MUL	R9	R6	R9
      0x58280005,  //  0016  LDCONST	R10	K5
      0x7C1C0600,  //  0017  CALL	R7	3
      0x00140A07,  //  0018  ADD	R5	R5	R7
      0x00180D05,  //  0019  ADD	R6	R6	K5
      0x7001FFF3,  //  001A  JMP		#000F
      0x581C0004,  //  001B  LDCONST	R7	K4
      0x58180004,  //  001C  LDCONST	R6	K4
      0x14200C04,  //  001D  LT	R8	R6	R4
      0x78220011,  //  001E  JMPF	R8	#0031
      0xB8220C00,  //  001F  GETNGBL	R8	K6
      0x8C201107,  //  0020  GETMET	R8	R8	K7
      0x5C280E00,  //  0021  MOVE	R10	R7
      0x582C0004,  //  0022  LDCONST	R11	K4
      0x5C300A00,  //  0023  MOVE	R12	R5
      0x5C340200,  //  0024  MOVE	R13	R1
      0x5C380400,  //  0025  MOVE	R14	R2
      0x7C200C00,  //  0026  CALL	R8	6
      0x980C0C08,  //  0027  SETIDX	R3	R6	R8
      0x88200102,  //  0028  GETMBR	R8	R0	K2
      0x8C201103,  //  0029  GETMET	R8	R8	K3
      0x542A0003,  //  002A  LDINT	R10	4
      0x08280C0A,  //  002B  MUL	R10	R6	R10
      0x582C0005,  //  002C  LDCONST	R11	K5
      0x7C200600,  //  002D  CALL	R8	3
      0x001C0E08,  //  002E  ADD	R7	R7	R8
      0x00180D05,  //  002F  ADD	R6	R6	K5
      0x7001FFEB,  //  0030  JMP		#001D
      0x70020013,  //  0031  JMP		#0046
      0x58140004,  //  0032  LDCONST	R5	K4
      0x14180A04,  //  0033  LT	R6	R5	R4
      0x781A0010,  //  0034  JMPF	R6	#0046
      0x88180102,  //  0035  GETMBR	R6	R0	K2
      0x8C180D03,  //  0036  GETMET	R6	R6	K3
      0x54220003,  //  0037  LDINT	R8	4
      0x08200A08,  //  0038  MUL	R8	R5	R8
      0x58240005,  //  0039  LDCONST	R9	K5
      0x7C180600,  //  003A  CALL	R6	3
      0xB81E0C00,  //  003B  GETNGBL	R7	K6
      0x8C1C0F07,  //  003C  GETMET	R7	R7	K7
      0x5C240C00,  //  003D  MOVE	R9	R6
      0x58280004,  //  003E  LDCONST	R10	K4
      0x542E00FE,  //  003F  LDINT	R11	255
      0x5C300200,  //  0040  MOVE	R12	R1
      0x5C340400,  //  0041  MOVE	R13	R2
      0x7C1C0C00,  //  0042  CALL	R7	6
      0x980C0A07,  //  0043  SETIDX	R3	R5	R7
      0x00140B05,  //  0044  ADD	R5	R5	K5
      0x7001FFEC,  //  0045  JMP		#0033
      0x80040600,  //  0046  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_palette_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(running),
    /* K1   */  be_nested_str_weak(palette),
    /* K2   */  be_nested_str_weak(bri),
    /* K3   */  be_nested_str_weak(slots),
    /* K4   */  be_nested_str_weak(set_duration),
    /* K5   */  be_nested_str_weak(color),
    /* K6   */  be_nested_str_weak(light_state),
    /* K7   */  be_nested_str_weak(RGB),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x500C0000,  //  0000  LDBOOL	R3	0	0
      0x90020003,  //  0001  SETMBR	R0	K0	R3
      0x90020201,  //  0002  SETMBR	R0	K1	R1
      0x540E0063,  //  0003  LDINT	R3	100
      0x90020403,  //  0004  SETMBR	R0	K2	R3
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x54120003,  //  0008  LDINT	R4	4
      0x0C0C0604,  //  0009  DIV	R3	R3	R4
      0x90020603,  //  000A  SETMBR	R0	K3	R3
      0x4C0C0000,  //  000B  LDNIL	R3
      0x200C0403,  //  000C  NE	R3	R2	R3
      0x780E0005,  //  000D  JMPF	R3	#0014
      0x8C0C0104,  //  000E  GETMET	R3	R0	K4
      0x60140009,  //  000F  GETGBL	R5	G9
      0x541A03E7,  //  0010  LDINT	R6	1000
      0x08180406,  //  0011  MUL	R6	R2	R6
      0x7C140200,  //  0012  CALL	R5	1
      0x7C0C0400,  //  0013  CALL	R3	2
      0xB80E0C00,  //  0014  GETNGBL	R3	K6
      0xB8120C00,  //  0015  GETNGBL	R4	K6
      0x88100907,  //  0016  GETMBR	R4	R4	K7
      0x7C0C0200,  //  0017  CALL	R3	1
      0x90020A03,  //  0018  SETMBR	R0	K5	R3
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_cb
********************************************************************/
be_local_closure(Animate_palette_set_cb,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animate_obj),
    /* K1   */  be_nested_str_weak(animate_mth),
    }),
    be_str_weak(set_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Animate_palette_stop,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(origin),
    /* K1   */  be_nested_str_weak(running),
    }),
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x50040000,  //  0002  LDBOOL	R1	0	0
      0x90020201,  //  0003  SETMBR	R0	K1	R1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animate
********************************************************************/
be_local_closure(Animate_palette_animate,   /* name */
  be_nested_proto(
    26,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(duration_ms),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(origin),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(scale_uint),
    /* K6   */  be_nested_str_weak(slots),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(slots_arr),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(palette),
    /* K11  */  be_nested_str_weak(get),
    /* K12  */  be_nested_str_weak(color),
    /* K13  */  be_nested_str_weak(set_rgb),
    /* K14  */  be_nested_str_weak(bri),
    /* K15  */  be_nested_str_weak(set_bri),
    /* K16  */  be_nested_str_weak(r),
    /* K17  */  be_nested_str_weak(g),
    /* K18  */  be_nested_str_weak(b),
    /* K19  */  be_nested_str_weak(animate_obj),
    /* K20  */  be_nested_str_weak(animate_mth),
    }),
    be_str_weak(animate),
    &be_const_str_solidified,
    ( &(const binstruction[193]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0000,  //  0003  JMPF	R2	#0005
      0x80000400,  //  0004  RET	0
      0x4C080000,  //  0005  LDNIL	R2
      0x1C080202,  //  0006  EQ	R2	R1	R2
      0x780A0003,  //  0007  JMPF	R2	#000C
      0xB80A0200,  //  0008  GETNGBL	R2	K1
      0x8C080502,  //  0009  GETMET	R2	R2	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x5C040400,  //  000B  MOVE	R1	R2
      0x88080103,  //  000C  GETMBR	R2	R0	K3
      0x04080202,  //  000D  SUB	R2	R1	R2
      0x140C0504,  //  000E  LT	R3	R2	K4
      0x780E0001,  //  000F  JMPF	R3	#0012
      0x58080004,  //  0010  LDCONST	R2	K4
      0x88040103,  //  0011  GETMBR	R1	R0	K3
      0x880C0100,  //  0012  GETMBR	R3	R0	K0
      0xB8120200,  //  0013  GETNGBL	R4	K1
      0x88100905,  //  0014  GETMBR	R4	R4	K5
      0x28140403,  //  0015  GE	R5	R2	R3
      0x78160005,  //  0016  JMPF	R5	#001D
      0x0C180403,  //  0017  DIV	R6	R2	R3
      0x08180C03,  //  0018  MUL	R6	R6	R3
      0x88140103,  //  0019  GETMBR	R5	R0	K3
      0x00140A06,  //  001A  ADD	R5	R5	R6
      0x90020605,  //  001B  SETMBR	R0	K3	R5
      0x10080403,  //  001C  MOD	R2	R2	R3
      0x88140106,  //  001D  GETMBR	R5	R0	K6
      0x04180B07,  //  001E  SUB	R6	R5	K7
      0x241C0D04,  //  001F  GT	R7	R6	K4
      0x781E0006,  //  0020  JMPF	R7	#0028
      0x881C0108,  //  0021  GETMBR	R7	R0	K8
      0x941C0E06,  //  0022  GETIDX	R7	R7	R6
      0x281C0407,  //  0023  GE	R7	R2	R7
      0x781E0000,  //  0024  JMPF	R7	#0026
      0x70020001,  //  0025  JMP		#0028
      0x04180D09,  //  0026  SUB	R6	R6	K9
      0x7001FFF6,  //  0027  JMP		#001F
      0x881C010A,  //  0028  GETMBR	R7	R0	K10
      0x8C1C0F0B,  //  0029  GETMET	R7	R7	K11
      0x54260003,  //  002A  LDINT	R9	4
      0x08240C09,  //  002B  MUL	R9	R6	R9
      0x542A0003,  //  002C  LDINT	R10	4
      0x7C1C0600,  //  002D  CALL	R7	3
      0x8820010A,  //  002E  GETMBR	R8	R0	K10
      0x8C20110B,  //  002F  GETMET	R8	R8	K11
      0x00280D09,  //  0030  ADD	R10	R6	K9
      0x542E0003,  //  0031  LDINT	R11	4
      0x0828140B,  //  0032  MUL	R10	R10	R11
      0x542E0003,  //  0033  LDINT	R11	4
      0x7C200600,  //  0034  CALL	R8	3
      0x88240108,  //  0035  GETMBR	R9	R0	K8
      0x94241206,  //  0036  GETIDX	R9	R9	R6
      0x00280D09,  //  0037  ADD	R10	R6	K9
      0x882C0108,  //  0038  GETMBR	R11	R0	K8
      0x9428160A,  //  0039  GETIDX	R10	R11	R10
      0x5C300800,  //  003A  MOVE	R12	R4
      0x5C340400,  //  003B  MOVE	R13	R2
      0x5C381200,  //  003C  MOVE	R14	R9
      0x5C3C1400,  //  003D  MOVE	R15	R10
      0x54420007,  //  003E  LDINT	R16	8
      0x3C400E10,  //  003F  SHR	R16	R7	R16
      0x544600FE,  //  0040  LDINT	R17	255
      0x2C402011,  //  0041  AND	R16	R16	R17
      0x54460007,  //  0042  LDINT	R17	8
      0x3C441011,  //  0043  SHR	R17	R8	R17
      0x544A00FE,  //  0044  LDINT	R18	255
      0x2C442212,  //  0045  AND	R17	R17	R18
      0x7C300A00,  //  0046  CALL	R12	5
      0x5C2C1800,  //  0047  MOVE	R11	R12
      0x5C300800,  //  0048  MOVE	R12	R4
      0x5C340400,  //  0049  MOVE	R13	R2
      0x5C381200,  //  004A  MOVE	R14	R9
      0x5C3C1400,  //  004B  MOVE	R15	R10
      0x5442000F,  //  004C  LDINT	R16	16
      0x3C400E10,  //  004D  SHR	R16	R7	R16
      0x544600FE,  //  004E  LDINT	R17	255
      0x2C402011,  //  004F  AND	R16	R16	R17
      0x5446000F,  //  0050  LDINT	R17	16
      0x3C441011,  //  0051  SHR	R17	R8	R17
      0x544A00FE,  //  0052  LDINT	R18	255
      0x2C442212,  //  0053  AND	R17	R17	R18
      0x7C300A00,  //  0054  CALL	R12	5
      0x5C340800,  //  0055  MOVE	R13	R4
      0x5C380400,  //  0056  MOVE	R14	R2
      0x5C3C1200,  //  0057  MOVE	R15	R9
      0x5C401400,  //  0058  MOVE	R16	R10
      0x54460017,  //  0059  LDINT	R17	24
      0x3C440E11,  //  005A  SHR	R17	R7	R17
      0x544A00FE,  //  005B  LDINT	R18	255
      0x2C442212,  //  005C  AND	R17	R17	R18
      0x544A0017,  //  005D  LDINT	R18	24
      0x3C481012,  //  005E  SHR	R18	R8	R18
      0x544E00FE,  //  005F  LDINT	R19	255
      0x2C482413,  //  0060  AND	R18	R18	R19
      0x7C340A00,  //  0061  CALL	R13	5
      0x8838010C,  //  0062  GETMBR	R14	R0	K12
      0x8C3C1D0D,  //  0063  GETMET	R15	R14	K13
      0x54460007,  //  0064  LDINT	R17	8
      0x3C440E11,  //  0065  SHR	R17	R7	R17
      0x544A00FE,  //  0066  LDINT	R18	255
      0x2C442212,  //  0067  AND	R17	R17	R18
      0x544A000F,  //  0068  LDINT	R18	16
      0x3C480E12,  //  0069  SHR	R18	R7	R18
      0x544E00FE,  //  006A  LDINT	R19	255
      0x2C482413,  //  006B  AND	R18	R18	R19
      0x544E0017,  //  006C  LDINT	R19	24
      0x3C4C0E13,  //  006D  SHR	R19	R7	R19
      0x545200FE,  //  006E  LDINT	R20	255
      0x2C4C2614,  //  006F  AND	R19	R19	R20
      0x7C3C0800,  //  0070  CALL	R15	4
      0x883C1D0E,  //  0071  GETMBR	R15	R14	K14
      0x8C401D0D,  //  0072  GETMET	R16	R14	K13
      0x544A0007,  //  0073  LDINT	R18	8
      0x3C481012,  //  0074  SHR	R18	R8	R18
      0x544E00FE,  //  0075  LDINT	R19	255
      0x2C482413,  //  0076  AND	R18	R18	R19
      0x544E000F,  //  0077  LDINT	R19	16
      0x3C4C1013,  //  0078  SHR	R19	R8	R19
      0x545200FE,  //  0079  LDINT	R20	255
      0x2C4C2614,  //  007A  AND	R19	R19	R20
      0x54520017,  //  007B  LDINT	R20	24
      0x3C501014,  //  007C  SHR	R20	R8	R20
      0x545600FE,  //  007D  LDINT	R21	255
      0x2C502815,  //  007E  AND	R20	R20	R21
      0x7C400800,  //  007F  CALL	R16	4
      0x88401D0E,  //  0080  GETMBR	R16	R14	K14
      0x5C440800,  //  0081  MOVE	R17	R4
      0x5C480400,  //  0082  MOVE	R18	R2
      0x5C4C1200,  //  0083  MOVE	R19	R9
      0x5C501400,  //  0084  MOVE	R20	R10
      0x5C541E00,  //  0085  MOVE	R21	R15
      0x5C582000,  //  0086  MOVE	R22	R16
      0x7C440A00,  //  0087  CALL	R17	5
      0x8C481D0D,  //  0088  GETMET	R18	R14	K13
      0x5C501600,  //  0089  MOVE	R20	R11
      0x5C541800,  //  008A  MOVE	R21	R12
      0x5C581A00,  //  008B  MOVE	R22	R13
      0x7C480800,  //  008C  CALL	R18	4
      0x8C481D0F,  //  008D  GETMET	R18	R14	K15
      0x5C502200,  //  008E  MOVE	R20	R17
      0x7C480400,  //  008F  CALL	R18	2
      0x882C1D10,  //  0090  GETMBR	R11	R14	K16
      0x88301D11,  //  0091  GETMBR	R12	R14	K17
      0x88341D12,  //  0092  GETMBR	R13	R14	K18
      0x8848010E,  //  0093  GETMBR	R18	R0	K14
      0x544E0063,  //  0094  LDINT	R19	100
      0x204C2413,  //  0095  NE	R19	R18	R19
      0x784E001A,  //  0096  JMPF	R19	#00B2
      0xB84E0200,  //  0097  GETNGBL	R19	K1
      0x8C4C2705,  //  0098  GETMET	R19	R19	K5
      0x5C541600,  //  0099  MOVE	R21	R11
      0x58580004,  //  009A  LDCONST	R22	K4
      0x545E0063,  //  009B  LDINT	R23	100
      0x58600004,  //  009C  LDCONST	R24	K4
      0x5C642400,  //  009D  MOVE	R25	R18
      0x7C4C0C00,  //  009E  CALL	R19	6
      0x5C2C2600,  //  009F  MOVE	R11	R19
      0xB84E0200,  //  00A0  GETNGBL	R19	K1
      0x8C4C2705,  //  00A1  GETMET	R19	R19	K5
      0x5C541800,  //  00A2  MOVE	R21	R12
      0x58580004,  //  00A3  LDCONST	R22	K4
      0x545E0063,  //  00A4  LDINT	R23	100
      0x58600004,  //  00A5  LDCONST	R24	K4
      0x5C642400,  //  00A6  MOVE	R25	R18
      0x7C4C0C00,  //  00A7  CALL	R19	6
      0x5C302600,  //  00A8  MOVE	R12	R19
      0xB84E0200,  //  00A9  GETNGBL	R19	K1
      0x8C4C2705,  //  00AA  GETMET	R19	R19	K5
      0x5C541A00,  //  00AB  MOVE	R21	R13
      0x58580004,  //  00AC  LDCONST	R22	K4
      0x545E0063,  //  00AD  LDINT	R23	100
      0x58600004,  //  00AE  LDCONST	R24	K4
      0x5C642400,  //  00AF  MOVE	R25	R18
      0x7C4C0C00,  //  00B0  CALL	R19	6
      0x5C342600,  //  00B1  MOVE	R13	R19
      0x544E000F,  //  00B2  LDINT	R19	16
      0x384C1613,  //  00B3  SHL	R19	R11	R19
      0x54520007,  //  00B4  LDINT	R20	8
      0x38501814,  //  00B5  SHL	R20	R12	R20
      0x304C2614,  //  00B6  OR	R19	R19	R20
      0x304C260D,  //  00B7  OR	R19	R19	R13
      0x88500113,  //  00B8  GETMBR	R20	R0	K19
      0x88540114,  //  00B9  GETMBR	R21	R0	K20
      0x78520004,  //  00BA  JMPF	R20	#00C0
      0x78560003,  //  00BB  JMPF	R21	#00C0
      0x5C582A00,  //  00BC  MOVE	R22	R21
      0x5C5C2800,  //  00BD  MOVE	R23	R20
      0x5C602600,  //  00BE  MOVE	R24	R19
      0x7C580400,  //  00BF  CALL	R22	2
      0x80042600,  //  00C0  RET	1	R19
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_range
********************************************************************/
be_local_closure(Animate_palette_set_range,   /* name */
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
    /* K0   */  be_nested_str_weak(value_error),
    /* K1   */  be_nested_str_weak(min_X20must_X20be_X20lower_X20than_X20mex),
    /* K2   */  be_nested_str_weak(range_min),
    /* K3   */  be_nested_str_weak(range_max),
    /* K4   */  be_nested_str_weak(slots_arr),
    /* K5   */  be_nested_str_weak(parse_palette),
    }),
    be_str_weak(set_range),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x280C0202,  //  0000  GE	R3	R1	R2
      0x780E0000,  //  0001  JMPF	R3	#0003
      0xB0060101,  //  0002  RAISE	1	K0	K1
      0x90020401,  //  0003  SETMBR	R0	K2	R1
      0x90020602,  //  0004  SETMBR	R0	K3	R2
      0x8C0C0105,  //  0005  GETMET	R3	R0	K5
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x7C0C0600,  //  0008  CALL	R3	3
      0x90020803,  //  0009  SETMBR	R0	K4	R3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bri
********************************************************************/
be_local_closure(Animate_palette_set_bri,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(bri),
    }),
    be_str_weak(set_bri),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_running
********************************************************************/
be_local_closure(Animate_palette_is_running,   /* name */
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
    /* K0   */  be_nested_str_weak(running),
    }),
    be_str_weak(is_running),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040017,  //  0000  GETGBL	R1	G23
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_duration
********************************************************************/
be_local_closure(Animate_palette_set_duration,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(value_error),
    /* K2   */  be_nested_str_weak(duration_ms_X20must_X20be_X20positive),
    /* K3   */  be_nested_str_weak(duration_ms),
    /* K4   */  be_nested_str_weak(slots_arr),
    /* K5   */  be_nested_str_weak(parse_palette),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(set_duration),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x18080300,  //  0004  LE	R2	R1	K0
      0x780A0000,  //  0005  JMPF	R2	#0007
      0xB0060302,  //  0006  RAISE	1	K1	K2
      0x90020601,  //  0007  SETMBR	R0	K3	R1
      0x8C080105,  //  0008  GETMET	R2	R0	K5
      0x58100000,  //  0009  LDCONST	R4	K0
      0x04140306,  //  000A  SUB	R5	R1	K6
      0x7C080600,  //  000B  CALL	R2	3
      0x90020802,  //  000C  SETMBR	R0	K4	R2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_value
********************************************************************/
be_local_closure(Animate_palette_set_value,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(range_min),
    /* K1   */  be_nested_str_weak(range_max),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(scale_int),
    /* K4   */  be_nested_str_weak(slots),
    /* K5   */  be_const_int(2),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(slots_arr),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(palette),
    /* K10  */  be_nested_str_weak(get),
    /* K11  */  be_nested_str_weak(animate_obj),
    /* K12  */  be_nested_str_weak(animate_mth),
    }),
    be_str_weak(set_value),
    &be_const_str_solidified,
    ( &(const binstruction[96]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x740A0003,  //  0003  JMPT	R2	#0008
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C080403,  //  0006  EQ	R2	R2	R3
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x4C080000,  //  0008  LDNIL	R2
      0x80040400,  //  0009  RET	1	R2
      0xB80A0400,  //  000A  GETNGBL	R2	K2
      0x88080503,  //  000B  GETMBR	R2	R2	K3
      0x880C0104,  //  000C  GETMBR	R3	R0	K4
      0x04100705,  //  000D  SUB	R4	R3	K5
      0x24140906,  //  000E  GT	R5	R4	K6
      0x78160006,  //  000F  JMPF	R5	#0017
      0x88140107,  //  0010  GETMBR	R5	R0	K7
      0x94140A04,  //  0011  GETIDX	R5	R5	R4
      0x28140205,  //  0012  GE	R5	R1	R5
      0x78160000,  //  0013  JMPF	R5	#0015
      0x70020001,  //  0014  JMP		#0017
      0x04100908,  //  0015  SUB	R4	R4	K8
      0x7001FFF6,  //  0016  JMP		#000E
      0x88140109,  //  0017  GETMBR	R5	R0	K9
      0x8C140B0A,  //  0018  GETMET	R5	R5	K10
      0x541E0003,  //  0019  LDINT	R7	4
      0x081C0807,  //  001A  MUL	R7	R4	R7
      0x54220003,  //  001B  LDINT	R8	4
      0x7C140600,  //  001C  CALL	R5	3
      0x88180109,  //  001D  GETMBR	R6	R0	K9
      0x8C180D0A,  //  001E  GETMET	R6	R6	K10
      0x00200908,  //  001F  ADD	R8	R4	K8
      0x54260003,  //  0020  LDINT	R9	4
      0x08201009,  //  0021  MUL	R8	R8	R9
      0x54260003,  //  0022  LDINT	R9	4
      0x7C180600,  //  0023  CALL	R6	3
      0x881C0107,  //  0024  GETMBR	R7	R0	K7
      0x941C0E04,  //  0025  GETIDX	R7	R7	R4
      0x00200908,  //  0026  ADD	R8	R4	K8
      0x88240107,  //  0027  GETMBR	R9	R0	K7
      0x94201208,  //  0028  GETIDX	R8	R9	R8
      0x5C280400,  //  0029  MOVE	R10	R2
      0x5C2C0200,  //  002A  MOVE	R11	R1
      0x5C300E00,  //  002B  MOVE	R12	R7
      0x5C341000,  //  002C  MOVE	R13	R8
      0x543A0007,  //  002D  LDINT	R14	8
      0x3C380A0E,  //  002E  SHR	R14	R5	R14
      0x543E00FE,  //  002F  LDINT	R15	255
      0x2C381C0F,  //  0030  AND	R14	R14	R15
      0x543E0007,  //  0031  LDINT	R15	8
      0x3C3C0C0F,  //  0032  SHR	R15	R6	R15
      0x544200FE,  //  0033  LDINT	R16	255
      0x2C3C1E10,  //  0034  AND	R15	R15	R16
      0x7C280A00,  //  0035  CALL	R10	5
      0x5C241400,  //  0036  MOVE	R9	R10
      0x5C280400,  //  0037  MOVE	R10	R2
      0x5C2C0200,  //  0038  MOVE	R11	R1
      0x5C300E00,  //  0039  MOVE	R12	R7
      0x5C341000,  //  003A  MOVE	R13	R8
      0x543A000F,  //  003B  LDINT	R14	16
      0x3C380A0E,  //  003C  SHR	R14	R5	R14
      0x543E00FE,  //  003D  LDINT	R15	255
      0x2C381C0F,  //  003E  AND	R14	R14	R15
      0x543E000F,  //  003F  LDINT	R15	16
      0x3C3C0C0F,  //  0040  SHR	R15	R6	R15
      0x544200FE,  //  0041  LDINT	R16	255
      0x2C3C1E10,  //  0042  AND	R15	R15	R16
      0x7C280A00,  //  0043  CALL	R10	5
      0x5C2C0400,  //  0044  MOVE	R11	R2
      0x5C300200,  //  0045  MOVE	R12	R1
      0x5C340E00,  //  0046  MOVE	R13	R7
      0x5C381000,  //  0047  MOVE	R14	R8
      0x543E0017,  //  0048  LDINT	R15	24
      0x3C3C0A0F,  //  0049  SHR	R15	R5	R15
      0x544200FE,  //  004A  LDINT	R16	255
      0x2C3C1E10,  //  004B  AND	R15	R15	R16
      0x54420017,  //  004C  LDINT	R16	24
      0x3C400C10,  //  004D  SHR	R16	R6	R16
      0x544600FE,  //  004E  LDINT	R17	255
      0x2C402011,  //  004F  AND	R16	R16	R17
      0x7C2C0A00,  //  0050  CALL	R11	5
      0x5432000F,  //  0051  LDINT	R12	16
      0x3830120C,  //  0052  SHL	R12	R9	R12
      0x54360007,  //  0053  LDINT	R13	8
      0x3834140D,  //  0054  SHL	R13	R10	R13
      0x3030180D,  //  0055  OR	R12	R12	R13
      0x3030180B,  //  0056  OR	R12	R12	R11
      0x8834010B,  //  0057  GETMBR	R13	R0	K11
      0x8838010C,  //  0058  GETMBR	R14	R0	K12
      0x78360004,  //  0059  JMPF	R13	#005F
      0x783A0003,  //  005A  JMPF	R14	#005F
      0x5C3C1C00,  //  005B  MOVE	R15	R14
      0x5C401A00,  //  005C  MOVE	R16	R13
      0x5C441800,  //  005D  MOVE	R17	R12
      0x7C3C0400,  //  005E  CALL	R15	2
      0x80041800,  //  005F  RET	1	R12
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(Animate_palette_start,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(duration_ms),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(origin),
    /* K4   */  be_nested_str_weak(running),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0000,  //  0003  JMPF	R2	#0005
      0x80000400,  //  0004  RET	0
      0x4C080000,  //  0005  LDNIL	R2
      0x1C080202,  //  0006  EQ	R2	R1	R2
      0x780A0003,  //  0007  JMPF	R2	#000C
      0xB80A0200,  //  0008  GETNGBL	R2	K1
      0x8C080502,  //  0009  GETMET	R2	R2	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x5C040400,  //  000B  MOVE	R1	R2
      0x90020601,  //  000C  SETMBR	R0	K3	R1
      0x50080200,  //  000D  LDBOOL	R2	1	0
      0x90020802,  //  000E  SETMBR	R0	K4	R2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_palette
********************************************************************/
be_local_class(Animate_palette,
    12,
    NULL,
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(start, -1), be_const_closure(Animate_palette_start_closure) },
        { be_const_key_weak(to_css_gradient, -1), be_const_closure(Animate_palette_to_css_gradient_closure) },
        { be_const_key_weak(range_min, -1), be_const_var(6) },
        { be_const_key_weak(init, -1), be_const_closure(Animate_palette_init_closure) },
        { be_const_key_weak(set_value, -1), be_const_closure(Animate_palette_set_value_closure) },
        { be_const_key_weak(stop, -1), be_const_closure(Animate_palette_stop_closure) },
        { be_const_key_weak(slots, 15), be_const_var(2) },
        { be_const_key_weak(origin, -1), be_const_var(5) },
        { be_const_key_weak(animate, 19), be_const_closure(Animate_palette_animate_closure) },
        { be_const_key_weak(set_range, 2), be_const_closure(Animate_palette_set_range_closure) },
        { be_const_key_weak(slots_arr, -1), be_const_var(1) },
        { be_const_key_weak(set_bri, -1), be_const_closure(Animate_palette_set_bri_closure) },
        { be_const_key_weak(bri, 17), be_const_var(10) },
        { be_const_key_weak(is_running, -1), be_const_closure(Animate_palette_is_running_closure) },
        { be_const_key_weak(animate_mth, -1), be_const_var(9) },
        { be_const_key_weak(duration_ms, -1), be_const_var(4) },
        { be_const_key_weak(color, -1), be_const_var(11) },
        { be_const_key_weak(set_duration, 22), be_const_closure(Animate_palette_set_duration_closure) },
        { be_const_key_weak(animate_obj, -1), be_const_var(8) },
        { be_const_key_weak(palette, -1), be_const_var(0) },
        { be_const_key_weak(parse_palette, 14), be_const_closure(Animate_palette_parse_palette_closure) },
        { be_const_key_weak(set_cb, 4), be_const_closure(Animate_palette_set_cb_closure) },
        { be_const_key_weak(running, -1), be_const_var(3) },
        { be_const_key_weak(range_max, 0), be_const_var(7) },
    })),
    be_str_weak(Animate_palette)
);

extern const bclass be_class_Animate_rotate;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_rotate_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(set_cb),
    /* K2   */  be_nested_str_weak(code),
    /* K3   */  be_nested_str_weak(push),
    /* K4   */  be_nested_str_weak(animate),
    /* K5   */  be_nested_str_weak(ins_ramp),
    /* K6   */  be_nested_str_weak(ins_goto),
    /* K7   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x60180003,  //  0000  GETGBL	R6	G3
      0x5C1C0000,  //  0001  MOVE	R7	R0
      0x7C180200,  //  0002  CALL	R6	1
      0x8C180D00,  //  0003  GETMET	R6	R6	K0
      0x7C180200,  //  0004  CALL	R6	1
      0x8C180101,  //  0005  GETMET	R6	R0	K1
      0x5C200200,  //  0006  MOVE	R8	R1
      0x5C240400,  //  0007  MOVE	R9	R2
      0x7C180600,  //  0008  CALL	R6	3
      0x88180102,  //  0009  GETMBR	R6	R0	K2
      0x8C180D03,  //  000A  GETMET	R6	R6	K3
      0xB8220800,  //  000B  GETNGBL	R8	K4
      0x8C201105,  //  000C  GETMET	R8	R8	K5
      0x5C280600,  //  000D  MOVE	R10	R3
      0x5C2C0800,  //  000E  MOVE	R11	R4
      0x5C300A00,  //  000F  MOVE	R12	R5
      0x7C200800,  //  0010  CALL	R8	4
      0x7C180400,  //  0011  CALL	R6	2
      0x88180102,  //  0012  GETMBR	R6	R0	K2
      0x8C180D03,  //  0013  GETMET	R6	R6	K3
      0xB8220800,  //  0014  GETNGBL	R8	K4
      0x8C201106,  //  0015  GETMET	R8	R8	K6
      0x58280007,  //  0016  LDCONST	R10	K7
      0x582C0007,  //  0017  LDCONST	R11	K7
      0x58300007,  //  0018  LDCONST	R12	K7
      0x7C200800,  //  0019  CALL	R8	4
      0x7C180400,  //  001A  CALL	R6	2
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_rotate
********************************************************************/
extern const bclass be_class_Animate_engine;
be_local_class(Animate_rotate,
    0,
    &be_class_Animate_engine,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Animate_rotate_init_closure) },
    })),
    be_str_weak(Animate_rotate)
);

extern const bclass be_class_Animate_ins_goto;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_ins_goto_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(pc_rel),
    /* K1   */  be_nested_str_weak(pc_abs),
    /* K2   */  be_nested_str_weak(duration),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_ins_goto
********************************************************************/
be_local_class(Animate_ins_goto,
    3,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pc_rel, -1), be_const_var(0) },
        { be_const_key_weak(duration, -1), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(Animate_ins_goto_init_closure) },
        { be_const_key_weak(pc_abs, 2), be_const_var(1) },
    })),
    be_str_weak(Animate_ins_goto)
);

extern const bclass be_class_Animate_ins_ramp;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_ins_ramp_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(a),
    /* K1   */  be_nested_str_weak(b),
    /* K2   */  be_nested_str_weak(duration),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_ins_ramp
********************************************************************/
be_local_class(Animate_ins_ramp,
    3,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(a, -1), be_const_var(0) },
        { be_const_key_weak(b, 2), be_const_var(1) },
        { be_const_key_weak(duration, -1), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(Animate_ins_ramp_init_closure) },
    })),
    be_str_weak(Animate_ins_ramp)
);

extern const bclass be_class_Animate_engine;

/********************************************************************
** Solidified function: animate
********************************************************************/
be_local_closure(Animate_engine_animate,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(running),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(ins_time),
    /* K4   */  be_nested_str_weak(pc),
    /* K5   */  be_nested_str_weak(code),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(internal_error),
    /* K8   */  be_nested_str_weak(Animate_X20pc_X20is_X20out_X20of_X20range),
    /* K9   */  be_nested_str_weak(animate),
    /* K10  */  be_nested_str_weak(ins_ramp),
    /* K11  */  be_nested_str_weak(closure),
    /* K12  */  be_nested_str_weak(animate_obj),
    /* K13  */  be_nested_str_weak(animate_mth),
    /* K14  */  be_nested_str_weak(duration),
    /* K15  */  be_nested_str_weak(value),
    /* K16  */  be_nested_str_weak(scale_uint),
    /* K17  */  be_nested_str_weak(a),
    /* K18  */  be_nested_str_weak(b),
    /* K19  */  be_const_int(1),
    /* K20  */  be_nested_str_weak(ins_goto),
    /* K21  */  be_nested_str_weak(pc_rel),
    /* K22  */  be_nested_str_weak(pc_abs),
    /* K23  */  be_nested_str_weak(unknown_X20instruction),
    }),
    be_str_weak(animate),
    &be_const_str_solidified,
    ( &(const binstruction[113]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0000,  //  0001  JMPT	R2	#0003
      0x80000400,  //  0002  RET	0
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0003,  //  0005  JMPF	R2	#000A
      0xB80A0200,  //  0006  GETNGBL	R2	K1
      0x8C080502,  //  0007  GETMET	R2	R2	K2
      0x7C080200,  //  0008  CALL	R2	1
      0x5C040400,  //  0009  MOVE	R1	R2
      0x50080200,  //  000A  LDBOOL	R2	1	0
      0x780A0062,  //  000B  JMPF	R2	#006F
      0x88080103,  //  000C  GETMBR	R2	R0	K3
      0x04080202,  //  000D  SUB	R2	R1	R2
      0x880C0104,  //  000E  GETMBR	R3	R0	K4
      0x6010000C,  //  000F  GETGBL	R4	G12
      0x88140105,  //  0010  GETMBR	R5	R0	K5
      0x7C100200,  //  0011  CALL	R4	1
      0x280C0604,  //  0012  GE	R3	R3	R4
      0x780E0002,  //  0013  JMPF	R3	#0017
      0x500C0000,  //  0014  LDBOOL	R3	0	0
      0x90020003,  //  0015  SETMBR	R0	K0	R3
      0x70020057,  //  0016  JMP		#006F
      0x880C0104,  //  0017  GETMBR	R3	R0	K4
      0x140C0706,  //  0018  LT	R3	R3	K6
      0x780E0000,  //  0019  JMPF	R3	#001B
      0xB0060F08,  //  001A  RAISE	1	K7	K8
      0x880C0105,  //  001B  GETMBR	R3	R0	K5
      0x88100104,  //  001C  GETMBR	R4	R0	K4
      0x940C0604,  //  001D  GETIDX	R3	R3	R4
      0x6010000F,  //  001E  GETGBL	R4	G15
      0x5C140600,  //  001F  MOVE	R5	R3
      0xB81A1200,  //  0020  GETNGBL	R6	K9
      0x88180D0A,  //  0021  GETMBR	R6	R6	K10
      0x7C100400,  //  0022  CALL	R4	2
      0x7812002E,  //  0023  JMPF	R4	#0053
      0x8810010B,  //  0024  GETMBR	R4	R0	K11
      0x8814010C,  //  0025  GETMBR	R5	R0	K12
      0x8818010D,  //  0026  GETMBR	R6	R0	K13
      0x881C070E,  //  0027  GETMBR	R7	R3	K14
      0x141C0407,  //  0028  LT	R7	R2	R7
      0x781E0014,  //  0029  JMPF	R7	#003F
      0xB81E0200,  //  002A  GETNGBL	R7	K1
      0x8C1C0F10,  //  002B  GETMET	R7	R7	K16
      0x5C240400,  //  002C  MOVE	R9	R2
      0x58280006,  //  002D  LDCONST	R10	K6
      0x882C070E,  //  002E  GETMBR	R11	R3	K14
      0x88300711,  //  002F  GETMBR	R12	R3	K17
      0x88340712,  //  0030  GETMBR	R13	R3	K18
      0x7C1C0C00,  //  0031  CALL	R7	6
      0x90021E07,  //  0032  SETMBR	R0	K15	R7
      0x78160004,  //  0033  JMPF	R5	#0039
      0x781A0003,  //  0034  JMPF	R6	#0039
      0x5C1C0C00,  //  0035  MOVE	R7	R6
      0x5C200A00,  //  0036  MOVE	R8	R5
      0x8824010F,  //  0037  GETMBR	R9	R0	K15
      0x7C1C0400,  //  0038  CALL	R7	2
      0x78120002,  //  0039  JMPF	R4	#003D
      0x5C1C0800,  //  003A  MOVE	R7	R4
      0x8820010F,  //  003B  GETMBR	R8	R0	K15
      0x7C1C0200,  //  003C  CALL	R7	1
      0x70020030,  //  003D  JMP		#006F
      0x70020012,  //  003E  JMP		#0052
      0x881C0712,  //  003F  GETMBR	R7	R3	K18
      0x90021E07,  //  0040  SETMBR	R0	K15	R7
      0x78160004,  //  0041  JMPF	R5	#0047
      0x781A0003,  //  0042  JMPF	R6	#0047
      0x5C1C0C00,  //  0043  MOVE	R7	R6
      0x5C200A00,  //  0044  MOVE	R8	R5
      0x8824010F,  //  0045  GETMBR	R9	R0	K15
      0x7C1C0400,  //  0046  CALL	R7	2
      0x78120002,  //  0047  JMPF	R4	#004B
      0x5C1C0800,  //  0048  MOVE	R7	R4
      0x8820010F,  //  0049  GETMBR	R8	R0	K15
      0x7C1C0200,  //  004A  CALL	R7	1
      0x881C0104,  //  004B  GETMBR	R7	R0	K4
      0x001C0F13,  //  004C  ADD	R7	R7	K19
      0x90020807,  //  004D  SETMBR	R0	K4	R7
      0x881C070E,  //  004E  GETMBR	R7	R3	K14
      0x041C0407,  //  004F  SUB	R7	R2	R7
      0x041C0207,  //  0050  SUB	R7	R1	R7
      0x90020607,  //  0051  SETMBR	R0	K3	R7
      0x7002001A,  //  0052  JMP		#006E
      0x6010000F,  //  0053  GETGBL	R4	G15
      0x5C140600,  //  0054  MOVE	R5	R3
      0xB81A1200,  //  0055  GETNGBL	R6	K9
      0x88180D14,  //  0056  GETMBR	R6	R6	K20
      0x7C100400,  //  0057  CALL	R4	2
      0x78120013,  //  0058  JMPF	R4	#006D
      0x8810070E,  //  0059  GETMBR	R4	R3	K14
      0x14100404,  //  005A  LT	R4	R2	R4
      0x78120001,  //  005B  JMPF	R4	#005E
      0x70020011,  //  005C  JMP		#006F
      0x7002000D,  //  005D  JMP		#006C
      0x88100715,  //  005E  GETMBR	R4	R3	K21
      0x20100906,  //  005F  NE	R4	R4	K6
      0x78120004,  //  0060  JMPF	R4	#0066
      0x88100104,  //  0061  GETMBR	R4	R0	K4
      0x88140715,  //  0062  GETMBR	R5	R3	K21
      0x00100805,  //  0063  ADD	R4	R4	R5
      0x90020804,  //  0064  SETMBR	R0	K4	R4
      0x70020001,  //  0065  JMP		#0068
      0x88100716,  //  0066  GETMBR	R4	R3	K22
      0x90020804,  //  0067  SETMBR	R0	K4	R4
      0x8810070E,  //  0068  GETMBR	R4	R3	K14
      0x04100404,  //  0069  SUB	R4	R2	R4
      0x04100204,  //  006A  SUB	R4	R1	R4
      0x90020604,  //  006B  SETMBR	R0	K3	R4
      0x70020000,  //  006C  JMP		#006E
      0xB0060F17,  //  006D  RAISE	1	K7	K23
      0x7001FF9A,  //  006E  JMP		#000A
      0x8808010F,  //  006F  GETMBR	R2	R0	K15
      0x80040400,  //  0070  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_engine_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(code),
    /* K1   */  be_nested_str_weak(pc),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(ins_time),
    /* K4   */  be_nested_str_weak(running),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x90020302,  //  0003  SETMBR	R0	K1	K2
      0x90020702,  //  0004  SETMBR	R0	K3	K2
      0x50040000,  //  0005  LDBOOL	R1	0	0
      0x90020801,  //  0006  SETMBR	R0	K4	R1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Animate_engine_stop,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(running),
    }),
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_running
********************************************************************/
be_local_closure(Animate_engine_is_running,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(running),
    }),
    be_str_weak(is_running),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_cb
********************************************************************/
be_local_closure(Animate_engine_set_cb,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animate_obj),
    /* K1   */  be_nested_str_weak(animate_mth),
    }),
    be_str_weak(set_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(Animate_engine_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(millis),
    /* K2   */  be_nested_str_weak(value),
    /* K3   */  be_nested_str_weak(ins_time),
    /* K4   */  be_nested_str_weak(running),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0203,  //  0001  EQ	R3	R1	R3
      0x780E0003,  //  0002  JMPF	R3	#0007
      0xB80E0000,  //  0003  GETNGBL	R3	K0
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x5C040600,  //  0006  MOVE	R1	R3
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0000,  //  0009  JMPF	R3	#000B
      0x90020402,  //  000A  SETMBR	R0	K2	R2
      0x90020601,  //  000B  SETMBR	R0	K3	R1
      0x500C0200,  //  000C  LDBOOL	R3	1	0
      0x90020803,  //  000D  SETMBR	R0	K4	R3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_engine
********************************************************************/
be_local_class(Animate_engine,
    8,
    NULL,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(animate, -1), be_const_closure(Animate_engine_animate_closure) },
        { be_const_key_weak(ins_time, -1), be_const_var(5) },
        { be_const_key_weak(running, 10), be_const_var(6) },
        { be_const_key_weak(stop, 7), be_const_closure(Animate_engine_stop_closure) },
        { be_const_key_weak(closure, -1), be_const_var(1) },
        { be_const_key_weak(init, 1), be_const_closure(Animate_engine_init_closure) },
        { be_const_key_weak(animate_mth, -1), be_const_var(3) },
        { be_const_key_weak(set_cb, -1), be_const_closure(Animate_engine_set_cb_closure) },
        { be_const_key_weak(code, -1), be_const_var(0) },
        { be_const_key_weak(is_running, -1), be_const_closure(Animate_engine_is_running_closure) },
        { be_const_key_weak(animate_obj, 11), be_const_var(2) },
        { be_const_key_weak(pc, -1), be_const_var(4) },
        { be_const_key_weak(value, 6), be_const_var(7) },
        { be_const_key_weak(start, -1), be_const_closure(Animate_engine_start_closure) },
    })),
    be_str_weak(Animate_engine)
);

extern const bclass be_class_Animate_back_forth;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_back_forth_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(set_cb),
    /* K2   */  be_nested_str_weak(code),
    /* K3   */  be_nested_str_weak(push),
    /* K4   */  be_nested_str_weak(animate),
    /* K5   */  be_nested_str_weak(ins_ramp),
    /* K6   */  be_nested_str_weak(ins_goto),
    /* K7   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x60180003,  //  0000  GETGBL	R6	G3
      0x5C1C0000,  //  0001  MOVE	R7	R0
      0x7C180200,  //  0002  CALL	R6	1
      0x8C180D00,  //  0003  GETMET	R6	R6	K0
      0x7C180200,  //  0004  CALL	R6	1
      0x8C180101,  //  0005  GETMET	R6	R0	K1
      0x5C200200,  //  0006  MOVE	R8	R1
      0x5C240400,  //  0007  MOVE	R9	R2
      0x7C180600,  //  0008  CALL	R6	3
      0x88180102,  //  0009  GETMBR	R6	R0	K2
      0x8C180D03,  //  000A  GETMET	R6	R6	K3
      0xB8220800,  //  000B  GETNGBL	R8	K4
      0x8C201105,  //  000C  GETMET	R8	R8	K5
      0x5C280600,  //  000D  MOVE	R10	R3
      0x5C2C0800,  //  000E  MOVE	R11	R4
      0x5C300A00,  //  000F  MOVE	R12	R5
      0x7C200800,  //  0010  CALL	R8	4
      0x7C180400,  //  0011  CALL	R6	2
      0x88180102,  //  0012  GETMBR	R6	R0	K2
      0x8C180D03,  //  0013  GETMET	R6	R6	K3
      0xB8220800,  //  0014  GETNGBL	R8	K4
      0x8C201105,  //  0015  GETMET	R8	R8	K5
      0x5C280800,  //  0016  MOVE	R10	R4
      0x5C2C0600,  //  0017  MOVE	R11	R3
      0x5C300A00,  //  0018  MOVE	R12	R5
      0x7C200800,  //  0019  CALL	R8	4
      0x7C180400,  //  001A  CALL	R6	2
      0x88180102,  //  001B  GETMBR	R6	R0	K2
      0x8C180D03,  //  001C  GETMET	R6	R6	K3
      0xB8220800,  //  001D  GETNGBL	R8	K4
      0x8C201106,  //  001E  GETMET	R8	R8	K6
      0x58280007,  //  001F  LDCONST	R10	K7
      0x582C0007,  //  0020  LDCONST	R11	K7
      0x58300007,  //  0021  LDCONST	R12	K7
      0x7C200800,  //  0022  CALL	R8	4
      0x7C180400,  //  0023  CALL	R6	2
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_back_forth
********************************************************************/
extern const bclass be_class_Animate_engine;
be_local_class(Animate_back_forth,
    0,
    &be_class_Animate_engine,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Animate_back_forth_init_closure) },
    })),
    be_str_weak(Animate_back_forth)
);

extern const bclass be_class_Animate_from_to;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_from_to_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(set_cb),
    /* K2   */  be_nested_str_weak(code),
    /* K3   */  be_nested_str_weak(push),
    /* K4   */  be_nested_str_weak(animate),
    /* K5   */  be_nested_str_weak(ins_ramp),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60180003,  //  0000  GETGBL	R6	G3
      0x5C1C0000,  //  0001  MOVE	R7	R0
      0x7C180200,  //  0002  CALL	R6	1
      0x8C180D00,  //  0003  GETMET	R6	R6	K0
      0x7C180200,  //  0004  CALL	R6	1
      0x8C180101,  //  0005  GETMET	R6	R0	K1
      0x5C200200,  //  0006  MOVE	R8	R1
      0x5C240400,  //  0007  MOVE	R9	R2
      0x7C180600,  //  0008  CALL	R6	3
      0x88180102,  //  0009  GETMBR	R6	R0	K2
      0x8C180D03,  //  000A  GETMET	R6	R6	K3
      0xB8220800,  //  000B  GETNGBL	R8	K4
      0x8C201105,  //  000C  GETMET	R8	R8	K5
      0x5C280600,  //  000D  MOVE	R10	R3
      0x5C2C0800,  //  000E  MOVE	R11	R4
      0x5C300A00,  //  000F  MOVE	R12	R5
      0x7C200800,  //  0010  CALL	R8	4
      0x7C180400,  //  0011  CALL	R6	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_from_to
********************************************************************/
extern const bclass be_class_Animate_engine;
be_local_class(Animate_from_to,
    0,
    &be_class_Animate_engine,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Animate_from_to_init_closure) },
    })),
    be_str_weak(Animate_from_to)
);

/********************************************************************
** Solidified module: animate
********************************************************************/
be_local_module(animate,
    "animate",
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ins_ramp, -1), be_const_class(be_class_Animate_ins_ramp) },
        { be_const_key_weak(from_to, -1), be_const_class(be_class_Animate_from_to) },
        { be_const_key_weak(palette, 3), be_const_class(be_class_Animate_palette) },
        { be_const_key_weak(engine, 5), be_const_class(be_class_Animate_engine) },
        { be_const_key_weak(back_forth, -1), be_const_class(be_class_Animate_back_forth) },
        { be_const_key_weak(rotate, -1), be_const_class(be_class_Animate_rotate) },
        { be_const_key_weak(ins_goto, -1), be_const_class(be_class_Animate_ins_goto) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(animate);
/********************************************************************/
/********************************************************************/
/* End of solidification */
