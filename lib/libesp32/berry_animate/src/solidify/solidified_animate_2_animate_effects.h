/* Solidification of animate_2_animate_effects.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Animate_painter;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Animate_painter_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(_cur_anim),
    /* K2   */  be_nested_str_weak(add_painter),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0002,  //  0004  JMPF	R2	#0008
      0x8C080302,  //  0005  GETMET	R2	R1	K2
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: paint
********************************************************************/
be_local_closure(class_Animate_painter_paint,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(paint),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_painter
********************************************************************/
be_local_class(Animate_painter,
    0,
    NULL,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(paint, -1), be_const_closure(class_Animate_painter_paint_closure) },
        { be_const_key_weak(init, 0), be_const_closure(class_Animate_painter_init_closure) },
    })),
    be_str_weak(Animate_painter)
);
// compact class 'Animate_pulse' ktab size: 15, total: 28 (saved 104 bytes)
static const bvalue be_ktab_class_Animate_pulse[15] = {
  /* K0   */  be_nested_str_weak(pulse_size),
  /* K1   */  be_nested_str_weak(slew_size),
  /* K2   */  be_nested_str_weak(back_color),
  /* K3   */  be_nested_str_weak(pos),
  /* K4   */  be_nested_str_weak(color),
  /* K5   */  be_nested_str_weak(init),
  /* K6   */  be_const_int(16777215),
  /* K7   */  be_const_int(1),
  /* K8   */  be_const_int(0),
  /* K9   */  be_const_int(-16777216),
  /* K10  */  be_nested_str_weak(fill_pixels),
  /* K11  */  be_nested_str_weak(pixel_size),
  /* K12  */  be_nested_str_weak(blend),
  /* K13  */  be_nested_str_weak(tasmota),
  /* K14  */  be_nested_str_weak(scale_int),
};


extern const bclass be_class_Animate_pulse;

/********************************************************************
** Solidified function: set_pulse_size
********************************************************************/
be_local_closure(class_Animate_pulse_set_pulse_size,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_pulse,     /* shared constants */
    be_str_weak(set_pulse_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_slew_size
********************************************************************/
be_local_closure(class_Animate_pulse_set_slew_size,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_pulse,     /* shared constants */
    be_str_weak(set_slew_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020201,  //  0000  SETMBR	R0	K1	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_back_color
********************************************************************/
be_local_closure(class_Animate_pulse_set_back_color,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_pulse,     /* shared constants */
    be_str_weak(set_back_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020401,  //  0000  SETMBR	R0	K2	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pos
********************************************************************/
be_local_closure(class_Animate_pulse_set_pos,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_pulse,     /* shared constants */
    be_str_weak(set_pos),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020601,  //  0000  SETMBR	R0	K3	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_color
********************************************************************/
be_local_closure(class_Animate_pulse_set_color,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_pulse,     /* shared constants */
    be_str_weak(set_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020801,  //  0000  SETMBR	R0	K4	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Animate_pulse_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_pulse,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100905,  //  0003  GETMET	R4	R4	K5
      0x7C100200,  //  0004  CALL	R4	1
      0x4C100000,  //  0005  LDNIL	R4
      0x1C100204,  //  0006  EQ	R4	R1	R4
      0x78120000,  //  0007  JMPF	R4	#0009
      0x58040006,  //  0008  LDCONST	R1	K6
      0x4C100000,  //  0009  LDNIL	R4
      0x1C100404,  //  000A  EQ	R4	R2	R4
      0x78120000,  //  000B  JMPF	R4	#000D
      0x58080007,  //  000C  LDCONST	R2	K7
      0x4C100000,  //  000D  LDNIL	R4
      0x1C100604,  //  000E  EQ	R4	R3	R4
      0x78120000,  //  000F  JMPF	R4	#0011
      0x580C0008,  //  0010  LDCONST	R3	K8
      0x90020801,  //  0011  SETMBR	R0	K4	R1
      0x90020509,  //  0012  SETMBR	R0	K2	K9
      0x14100508,  //  0013  LT	R4	R2	K8
      0x78120000,  //  0014  JMPF	R4	#0016
      0x58080008,  //  0015  LDCONST	R2	K8
      0x90020002,  //  0016  SETMBR	R0	K0	R2
      0x14100708,  //  0017  LT	R4	R3	K8
      0x78120000,  //  0018  JMPF	R4	#001A
      0x580C0008,  //  0019  LDCONST	R3	K8
      0x90020203,  //  001A  SETMBR	R0	K1	R3
      0x90020708,  //  001B  SETMBR	R0	K3	K8
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: paint
********************************************************************/
be_local_closure(class_Animate_pulse_paint,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_pulse,     /* shared constants */
    be_str_weak(paint),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x200C0509,  //  0001  NE	R3	R2	K9
      0x780E0002,  //  0002  JMPF	R3	#0006
      0x8C0C030A,  //  0003  GETMET	R3	R1	K10
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0x88100101,  //  0007  GETMBR	R4	R0	K1
      0x88140100,  //  0008  GETMBR	R5	R0	K0
      0x88180104,  //  0009  GETMBR	R6	R0	K4
      0x881C030B,  //  000A  GETMBR	R7	R1	K11
      0x4C200000,  //  000B  LDNIL	R8
      0x4C240000,  //  000C  LDNIL	R9
      0x5C200600,  //  000D  MOVE	R8	R3
      0x00280605,  //  000E  ADD	R10	R3	R5
      0x5C241400,  //  000F  MOVE	R9	R10
      0x14281108,  //  0010  LT	R10	R8	K8
      0x782A0000,  //  0011  JMPF	R10	#0013
      0x58200008,  //  0012  LDCONST	R8	K8
      0x28281207,  //  0013  GE	R10	R9	R7
      0x782A0000,  //  0014  JMPF	R10	#0016
      0x5C240E00,  //  0015  MOVE	R9	R7
      0x5C281000,  //  0016  MOVE	R10	R8
      0x142C1409,  //  0017  LT	R11	R10	R9
      0x782E0002,  //  0018  JMPF	R11	#001C
      0x98041406,  //  0019  SETIDX	R1	R10	R6
      0x00281507,  //  001A  ADD	R10	R10	K7
      0x7001FFFA,  //  001B  JMP		#0017
      0x242C0908,  //  001C  GT	R11	R4	K8
      0x782E003A,  //  001D  JMPF	R11	#0059
      0x042C0604,  //  001E  SUB	R11	R3	R4
      0x5C201600,  //  001F  MOVE	R8	R11
      0x5C240600,  //  0020  MOVE	R9	R3
      0x142C1108,  //  0021  LT	R11	R8	K8
      0x782E0000,  //  0022  JMPF	R11	#0024
      0x58200008,  //  0023  LDCONST	R8	K8
      0x282C1207,  //  0024  GE	R11	R9	R7
      0x782E0000,  //  0025  JMPF	R11	#0027
      0x5C240E00,  //  0026  MOVE	R9	R7
      0x5C281000,  //  0027  MOVE	R10	R8
      0x142C1409,  //  0028  LT	R11	R10	R9
      0x782E000F,  //  0029  JMPF	R11	#003A
      0x8C2C030C,  //  002A  GETMET	R11	R1	K12
      0x5C340400,  //  002B  MOVE	R13	R2
      0x5C380C00,  //  002C  MOVE	R14	R6
      0xB83E1A00,  //  002D  GETNGBL	R15	K13
      0x8C3C1F0E,  //  002E  GETMET	R15	R15	K14
      0x5C441400,  //  002F  MOVE	R17	R10
      0x04480604,  //  0030  SUB	R18	R3	R4
      0x04482507,  //  0031  SUB	R18	R18	K7
      0x5C4C0600,  //  0032  MOVE	R19	R3
      0x545200FE,  //  0033  LDINT	R20	255
      0x58540008,  //  0034  LDCONST	R21	K8
      0x7C3C0C00,  //  0035  CALL	R15	6
      0x7C2C0800,  //  0036  CALL	R11	4
      0x9804140B,  //  0037  SETIDX	R1	R10	R11
      0x00281507,  //  0038  ADD	R10	R10	K7
      0x7001FFED,  //  0039  JMP		#0028
      0x002C0605,  //  003A  ADD	R11	R3	R5
      0x5C201600,  //  003B  MOVE	R8	R11
      0x002C0605,  //  003C  ADD	R11	R3	R5
      0x002C1604,  //  003D  ADD	R11	R11	R4
      0x5C241600,  //  003E  MOVE	R9	R11
      0x142C1108,  //  003F  LT	R11	R8	K8
      0x782E0000,  //  0040  JMPF	R11	#0042
      0x58200008,  //  0041  LDCONST	R8	K8
      0x282C1207,  //  0042  GE	R11	R9	R7
      0x782E0000,  //  0043  JMPF	R11	#0045
      0x5C240E00,  //  0044  MOVE	R9	R7
      0x5C281000,  //  0045  MOVE	R10	R8
      0x142C1409,  //  0046  LT	R11	R10	R9
      0x782E0010,  //  0047  JMPF	R11	#0059
      0x8C2C030C,  //  0048  GETMET	R11	R1	K12
      0x5C340400,  //  0049  MOVE	R13	R2
      0x5C380C00,  //  004A  MOVE	R14	R6
      0xB83E1A00,  //  004B  GETNGBL	R15	K13
      0x8C3C1F0E,  //  004C  GETMET	R15	R15	K14
      0x5C441400,  //  004D  MOVE	R17	R10
      0x00480605,  //  004E  ADD	R18	R3	R5
      0x04482507,  //  004F  SUB	R18	R18	K7
      0x004C0605,  //  0050  ADD	R19	R3	R5
      0x004C2604,  //  0051  ADD	R19	R19	R4
      0x58500008,  //  0052  LDCONST	R20	K8
      0x545600FE,  //  0053  LDINT	R21	255
      0x7C3C0C00,  //  0054  CALL	R15	6
      0x7C2C0800,  //  0055  CALL	R11	4
      0x9804140B,  //  0056  SETIDX	R1	R10	R11
      0x00281507,  //  0057  ADD	R10	R10	K7
      0x7001FFEC,  //  0058  JMP		#0046
      0x502C0200,  //  0059  LDBOOL	R11	1	0
      0x80041600,  //  005A  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_pulse
********************************************************************/
extern const bclass be_class_Animate_painter;
be_local_class(Animate_pulse,
    5,
    &be_class_Animate_painter,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(paint, -1), be_const_closure(class_Animate_pulse_paint_closure) },
        { be_const_key_weak(set_slew_size, -1), be_const_closure(class_Animate_pulse_set_slew_size_closure) },
        { be_const_key_weak(pulse_size, -1), be_const_var(4) },
        { be_const_key_weak(set_back_color, 8), be_const_closure(class_Animate_pulse_set_back_color_closure) },
        { be_const_key_weak(color, -1), be_const_var(0) },
        { be_const_key_weak(back_color, -1), be_const_var(1) },
        { be_const_key_weak(set_pos, -1), be_const_closure(class_Animate_pulse_set_pos_closure) },
        { be_const_key_weak(set_color, -1), be_const_closure(class_Animate_pulse_set_color_closure) },
        { be_const_key_weak(init, 7), be_const_closure(class_Animate_pulse_init_closure) },
        { be_const_key_weak(pos, -1), be_const_var(2) },
        { be_const_key_weak(slew_size, 5), be_const_var(3) },
        { be_const_key_weak(set_pulse_size, 0), be_const_closure(class_Animate_pulse_set_pulse_size_closure) },
    })),
    be_str_weak(Animate_pulse)
);
// compact class 'Animate_crenel' ktab size: 19, total: 34 (saved 120 bytes)
static const bvalue be_ktab_class_Animate_crenel[19] = {
  /* K0   */  be_nested_str_weak(pulse_size),
  /* K1   */  be_nested_str_weak(back_color),
  /* K2   */  be_const_int(-16777216),
  /* K3   */  be_nested_str_weak(fill_pixels),
  /* K4   */  be_nested_str_weak(pos),
  /* K5   */  be_nested_str_weak(low_size),
  /* K6   */  be_nested_str_weak(color),
  /* K7   */  be_nested_str_weak(pixel_size),
  /* K8   */  be_nested_str_weak(nb_pulse),
  /* K9   */  be_const_int(0),
  /* K10  */  be_const_int(1),
  /* K11  */  be_nested_str_weak(int),
  /* K12  */  be_nested_str_weak(instance),
  /* K13  */  be_nested_str_weak(get_color),
  /* K14  */  be_nested_str_weak(tasmota),
  /* K15  */  be_nested_str_weak(scale_int),
  /* K16  */  be_nested_str_weak(init),
  /* K17  */  be_const_int(16777215),
  /* K18  */  be_const_int(3),
};


extern const bclass be_class_Animate_crenel;

/********************************************************************
** Solidified function: set_pulse_size
********************************************************************/
be_local_closure(class_Animate_crenel_set_pulse_size,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_crenel,     /* shared constants */
    be_str_weak(set_pulse_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: paint
********************************************************************/
be_local_closure(class_Animate_crenel_paint,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_crenel,     /* shared constants */
    be_str_weak(paint),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x200C0502,  //  0001  NE	R3	R2	K2
      0x780E0002,  //  0002  JMPF	R3	#0006
      0x8C0C0303,  //  0003  GETMET	R3	R1	K3
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x880C0104,  //  0006  GETMBR	R3	R0	K4
      0x88100100,  //  0007  GETMBR	R4	R0	K0
      0x88140105,  //  0008  GETMBR	R5	R0	K5
      0x88180106,  //  0009  GETMBR	R6	R0	K6
      0x881C0307,  //  000A  GETMBR	R7	R1	K7
      0x60200009,  //  000B  GETGBL	R8	G9
      0x00240805,  //  000C  ADD	R9	R4	R5
      0x7C200200,  //  000D  CALL	R8	1
      0x88240108,  //  000E  GETMBR	R9	R0	K8
      0x18281109,  //  000F  LE	R10	R8	K9
      0x782A0000,  //  0010  JMPF	R10	#0012
      0x5820000A,  //  0011  LDCONST	R8	K10
      0x1C281309,  //  0012  EQ	R10	R9	K9
      0x782A0000,  //  0013  JMPF	R10	#0015
      0x80001400,  //  0014  RET	0
      0x14281309,  //  0015  LT	R10	R9	K9
      0x782A0006,  //  0016  JMPF	R10	#001E
      0x00280604,  //  0017  ADD	R10	R3	R4
      0x0428150A,  //  0018  SUB	R10	R10	K10
      0x10281408,  //  0019  MOD	R10	R10	R8
      0x04281404,  //  001A  SUB	R10	R10	R4
      0x0028150A,  //  001B  ADD	R10	R10	K10
      0x5C0C1400,  //  001C  MOVE	R3	R10
      0x70020007,  //  001D  JMP		#0026
      0x44281000,  //  001E  NEG	R10	R8
      0x1428060A,  //  001F  LT	R10	R3	R10
      0x782A0004,  //  0020  JMPF	R10	#0026
      0x20281309,  //  0021  NE	R10	R9	K9
      0x782A0002,  //  0022  JMPF	R10	#0026
      0x000C0608,  //  0023  ADD	R3	R3	R8
      0x0424130A,  //  0024  SUB	R9	R9	K10
      0x7001FFF7,  //  0025  JMP		#001E
      0x14280607,  //  0026  LT	R10	R3	R7
      0x782A0028,  //  0027  JMPF	R10	#0051
      0x20281309,  //  0028  NE	R10	R9	K9
      0x782A0026,  //  0029  JMPF	R10	#0051
      0x58280009,  //  002A  LDCONST	R10	K9
      0x142C0709,  //  002B  LT	R11	R3	K9
      0x782E0001,  //  002C  JMPF	R11	#002F
      0x442C0600,  //  002D  NEG	R11	R3
      0x5C281600,  //  002E  MOVE	R10	R11
      0x602C0004,  //  002F  GETGBL	R11	G4
      0x88300106,  //  0030  GETMBR	R12	R0	K6
      0x7C2C0200,  //  0031  CALL	R11	1
      0x1C2C170B,  //  0032  EQ	R11	R11	K11
      0x782E0009,  //  0033  JMPF	R11	#003E
      0x142C1404,  //  0034  LT	R11	R10	R4
      0x782E0006,  //  0035  JMPF	R11	#003D
      0x002C060A,  //  0036  ADD	R11	R3	R10
      0x142C1607,  //  0037  LT	R11	R11	R7
      0x782E0003,  //  0038  JMPF	R11	#003D
      0x002C060A,  //  0039  ADD	R11	R3	R10
      0x98041606,  //  003A  SETIDX	R1	R11	R6
      0x0028150A,  //  003B  ADD	R10	R10	K10
      0x7001FFF6,  //  003C  JMP		#0034
      0x7002000F,  //  003D  JMP		#004E
      0x602C0004,  //  003E  GETGBL	R11	G4
      0x88300106,  //  003F  GETMBR	R12	R0	K6
      0x7C2C0200,  //  0040  CALL	R11	1
      0x1C2C170C,  //  0041  EQ	R11	R11	K12
      0x782E000A,  //  0042  JMPF	R11	#004E
      0x882C0106,  //  0043  GETMBR	R11	R0	K6
      0x8C2C170D,  //  0044  GETMET	R11	R11	K13
      0xB8361C00,  //  0045  GETNGBL	R13	K14
      0x8C341B0F,  //  0046  GETMET	R13	R13	K15
      0x5C3C1400,  //  0047  MOVE	R15	R10
      0x58400009,  //  0048  LDCONST	R16	K9
      0x0444090A,  //  0049  SUB	R17	R4	K10
      0x58480009,  //  004A  LDCONST	R18	K9
      0x544E00FE,  //  004B  LDINT	R19	255
      0x7C340C00,  //  004C  CALL	R13	6
      0x7C2C0400,  //  004D  CALL	R11	2
      0x000C0608,  //  004E  ADD	R3	R3	R8
      0x0424130A,  //  004F  SUB	R9	R9	K10
      0x7001FFD4,  //  0050  JMP		#0026
      0x50280200,  //  0051  LDBOOL	R10	1	0
      0x80041400,  //  0052  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_back_color
********************************************************************/
be_local_closure(class_Animate_crenel_set_back_color,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_crenel,     /* shared constants */
    be_str_weak(set_back_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020201,  //  0000  SETMBR	R0	K1	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pos
********************************************************************/
be_local_closure(class_Animate_crenel_set_pos,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_crenel,     /* shared constants */
    be_str_weak(set_pos),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020801,  //  0000  SETMBR	R0	K4	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_color
********************************************************************/
be_local_closure(class_Animate_crenel_set_color,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_crenel,     /* shared constants */
    be_str_weak(set_color),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020C01,  //  0000  SETMBR	R0	K6	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_low_size
********************************************************************/
be_local_closure(class_Animate_crenel_set_low_size,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_crenel,     /* shared constants */
    be_str_weak(set_low_size),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020A01,  //  0000  SETMBR	R0	K5	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Animate_crenel_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_crenel,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x60140003,  //  0000  GETGBL	R5	G3
      0x5C180000,  //  0001  MOVE	R6	R0
      0x7C140200,  //  0002  CALL	R5	1
      0x8C140B10,  //  0003  GETMET	R5	R5	K16
      0x7C140200,  //  0004  CALL	R5	1
      0x4C140000,  //  0005  LDNIL	R5
      0x1C140205,  //  0006  EQ	R5	R1	R5
      0x78160000,  //  0007  JMPF	R5	#0009
      0x58040011,  //  0008  LDCONST	R1	K17
      0x4C140000,  //  0009  LDNIL	R5
      0x1C140405,  //  000A  EQ	R5	R2	R5
      0x78160000,  //  000B  JMPF	R5	#000D
      0x5808000A,  //  000C  LDCONST	R2	K10
      0x4C140000,  //  000D  LDNIL	R5
      0x1C140605,  //  000E  EQ	R5	R3	R5
      0x78160000,  //  000F  JMPF	R5	#0011
      0x580C0012,  //  0010  LDCONST	R3	K18
      0x4C140000,  //  0011  LDNIL	R5
      0x1C140805,  //  0012  EQ	R5	R4	R5
      0x78160000,  //  0013  JMPF	R5	#0015
      0x5411FFFE,  //  0014  LDINT	R4	-1
      0x90020C01,  //  0015  SETMBR	R0	K6	R1
      0x90020302,  //  0016  SETMBR	R0	K1	K2
      0x14140509,  //  0017  LT	R5	R2	K9
      0x78160000,  //  0018  JMPF	R5	#001A
      0x58080009,  //  0019  LDCONST	R2	K9
      0x90020002,  //  001A  SETMBR	R0	K0	R2
      0x14140709,  //  001B  LT	R5	R3	K9
      0x78160000,  //  001C  JMPF	R5	#001E
      0x580C0009,  //  001D  LDCONST	R3	K9
      0x90020A03,  //  001E  SETMBR	R0	K5	R3
      0x90021004,  //  001F  SETMBR	R0	K8	R4
      0x90020909,  //  0020  SETMBR	R0	K4	K9
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_nb_pulse
********************************************************************/
be_local_closure(class_Animate_crenel_set_nb_pulse,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Animate_crenel,     /* shared constants */
    be_str_weak(set_nb_pulse),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90021001,  //  0000  SETMBR	R0	K8	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_crenel
********************************************************************/
extern const bclass be_class_Animate_painter;
be_local_class(Animate_crenel,
    6,
    &be_class_Animate_painter,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(low_size, -1), be_const_var(4) },
        { be_const_key_weak(paint, -1), be_const_closure(class_Animate_crenel_paint_closure) },
        { be_const_key_weak(color, -1), be_const_var(0) },
        { be_const_key_weak(set_pulse_size, 11), be_const_closure(class_Animate_crenel_set_pulse_size_closure) },
        { be_const_key_weak(pulse_size, -1), be_const_var(3) },
        { be_const_key_weak(set_back_color, 10), be_const_closure(class_Animate_crenel_set_back_color_closure) },
        { be_const_key_weak(set_pos, -1), be_const_closure(class_Animate_crenel_set_pos_closure) },
        { be_const_key_weak(back_color, -1), be_const_var(1) },
        { be_const_key_weak(set_low_size, 7), be_const_closure(class_Animate_crenel_set_low_size_closure) },
        { be_const_key_weak(pos, -1), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(class_Animate_crenel_init_closure) },
        { be_const_key_weak(set_color, 1), be_const_closure(class_Animate_crenel_set_color_closure) },
        { be_const_key_weak(set_nb_pulse, -1), be_const_closure(class_Animate_crenel_set_nb_pulse_closure) },
        { be_const_key_weak(nb_pulse, 0), be_const_var(5) },
    })),
    be_str_weak(Animate_crenel)
);
/********************************************************************/
/* End of solidification */
