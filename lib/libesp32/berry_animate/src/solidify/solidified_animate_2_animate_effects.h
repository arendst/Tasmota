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
/********************************************************************/
/* End of solidification */
