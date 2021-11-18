/********************************************************************
 * Berry class `Leds`
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WS2812

/********************************************************************
** Solidified function: get_pixel_color
********************************************************************/
be_local_closure(get_pixel_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    }),
    (be_nested_const_str("get_pixel_color", 337490048, 15)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5412000A,  //  0001  LDINT	R4	11
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C080600,  //  0003  CALL	R2	3
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(pixels_buffer,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    }),
    (be_nested_const_str("pixels_buffer", 1229555807, 13)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0005,  //  0001  LDINT	R3	6
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(clear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("clear_to", -766965166, 8),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_string("show", -1454906820, 4),
    }),
    (be_nested_const_str("clear", 1550717474, 5)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x8C040102,  //  0003  GETMET	R1	R0	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_string("gamma", -802614262, 5),
    /* K1   */  be_nested_string("pin", 1866532500, 3),
    /* K2   */  be_nested_string("WS2812", -755226078, 6),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_string("valuer_error", -1727020191, 12),
    /* K5   */  be_nested_string("no GPIO specified for neopixelbus", 42078528, 33),
    /* K6   */  be_nested_string("ctor", 375399343, 4),
    /* K7   */  be_nested_string("_p", 1594591802, 2),
    /* K8   */  be_nested_string("internal_error", -1775809127, 14),
    /* K9   */  be_nested_string("couldn't not initialize noepixelbus", -1758476484, 35),
    /* K10  */  be_nested_string("begin", 1748273790, 5),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[31]) {  /* code */
      0x50100200,  //  0000  LDBOOL	R4	1	0
      0x90020004,  //  0001  SETMBR	R0	K0	R4
      0x4C100000,  //  0002  LDNIL	R4
      0x1C100404,  //  0003  EQ	R4	R2	R4
      0x78120008,  //  0004  JMPF	R4	#000E
      0x8C100501,  //  0005  GETMET	R4	R2	K1
      0x88180502,  //  0006  GETMBR	R6	R2	K2
      0x7C100400,  //  0007  CALL	R4	2
      0x28100903,  //  0008  GE	R4	R4	K3
      0x78120003,  //  0009  JMPF	R4	#000E
      0x8C100501,  //  000A  GETMET	R4	R2	K1
      0x88180502,  //  000B  GETMBR	R6	R2	K2
      0x7C100400,  //  000C  CALL	R4	2
      0x5C080800,  //  000D  MOVE	R2	R4
      0x4C100000,  //  000E  LDNIL	R4
      0x1C100404,  //  000F  EQ	R4	R2	R4
      0x78120000,  //  0010  JMPF	R4	#0012
      0xB0060905,  //  0011  RAISE	1	K4	K5
      0x8C100106,  //  0012  GETMET	R4	R0	K6
      0x5C180200,  //  0013  MOVE	R6	R1
      0x5C1C0400,  //  0014  MOVE	R7	R2
      0x5C200600,  //  0015  MOVE	R8	R3
      0x7C100800,  //  0016  CALL	R4	4
      0x88100107,  //  0017  GETMBR	R4	R0	K7
      0x4C140000,  //  0018  LDNIL	R5
      0x1C100805,  //  0019  EQ	R4	R4	R5
      0x78120000,  //  001A  JMPF	R4	#001C
      0xB0061109,  //  001B  RAISE	1	K8	K9
      0x8C10010A,  //  001C  GETMET	R4	R0	K10
      0x7C100200,  //  001D  CALL	R4	1
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(set_pixel_color,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    /* K1   */  be_nested_string("to_gamma", 1597139862, 8),
    }),
    (be_nested_const_str("set_pixel_color", 1275248356, 15)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 9]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x541A0009,  //  0001  LDINT	R6	10
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x8C200101,  //  0003  GETMET	R8	R0	K1
      0x5C280400,  //  0004  MOVE	R10	R2
      0x5C2C0600,  //  0005  MOVE	R11	R3
      0x7C200600,  //  0006  CALL	R8	3
      0x7C100800,  //  0007  CALL	R4	4
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(begin,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    /* K1   */  be_const_int(1),
    }),
    (be_nested_const_str("begin", 1748273790, 5)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_gamma
********************************************************************/
be_local_closure(to_gamma,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_string("tasmota", 424643812, 7),
    /* K1   */  be_nested_string("scale_uint", -1204156202, 10),
    /* K2   */  be_const_int(0),
    /* K3   */  be_const_int(16711680),
    /* K4   */  be_nested_string("gamma", -802614262, 5),
    /* K5   */  be_nested_string("light", -493019601, 5),
    /* K6   */  be_nested_string("gamma8", -492123466, 6),
    }),
    (be_nested_const_str("to_gamma", 1597139862, 8)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[67]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0403,  //  0001  NE	R3	R2	R3
      0x780E0001,  //  0002  JMPF	R3	#0005
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x70020000,  //  0004  JMP		#0006
      0x540E0063,  //  0005  LDINT	R3	100
      0x5C080600,  //  0006  MOVE	R2	R3
      0xB80E0000,  //  0007  GETNGBL	R3	K0
      0x8C0C0701,  //  0008  GETMET	R3	R3	K1
      0x5C140400,  //  0009  MOVE	R5	R2
      0x58180002,  //  000A  LDCONST	R6	K2
      0x541E0063,  //  000B  LDINT	R7	100
      0x58200002,  //  000C  LDCONST	R8	K2
      0x2C240303,  //  000D  AND	R9	R1	K3
      0x542A000F,  //  000E  LDINT	R10	16
      0x3C24120A,  //  000F  SHR	R9	R9	R10
      0x7C0C0C00,  //  0010  CALL	R3	6
      0xB8120000,  //  0011  GETNGBL	R4	K0
      0x8C100901,  //  0012  GETMET	R4	R4	K1
      0x5C180400,  //  0013  MOVE	R6	R2
      0x581C0002,  //  0014  LDCONST	R7	K2
      0x54220063,  //  0015  LDINT	R8	100
      0x58240002,  //  0016  LDCONST	R9	K2
      0x542AFEFF,  //  0017  LDINT	R10	65280
      0x2C28020A,  //  0018  AND	R10	R1	R10
      0x542E0007,  //  0019  LDINT	R11	8
      0x3C28140B,  //  001A  SHR	R10	R10	R11
      0x7C100C00,  //  001B  CALL	R4	6
      0xB8160000,  //  001C  GETNGBL	R5	K0
      0x8C140B01,  //  001D  GETMET	R5	R5	K1
      0x5C1C0400,  //  001E  MOVE	R7	R2
      0x58200002,  //  001F  LDCONST	R8	K2
      0x54260063,  //  0020  LDINT	R9	100
      0x58280002,  //  0021  LDCONST	R10	K2
      0x542E00FE,  //  0022  LDINT	R11	255
      0x2C2C020B,  //  0023  AND	R11	R1	R11
      0x7C140C00,  //  0024  CALL	R5	6
      0x88180104,  //  0025  GETMBR	R6	R0	K4
      0x781A0013,  //  0026  JMPF	R6	#003B
      0xB81A0A00,  //  0027  GETNGBL	R6	K5
      0x8C180D06,  //  0028  GETMET	R6	R6	K6
      0x5C200600,  //  0029  MOVE	R8	R3
      0x7C180400,  //  002A  CALL	R6	2
      0x541E000F,  //  002B  LDINT	R7	16
      0x38180C07,  //  002C  SHL	R6	R6	R7
      0xB81E0A00,  //  002D  GETNGBL	R7	K5
      0x8C1C0F06,  //  002E  GETMET	R7	R7	K6
      0x5C240800,  //  002F  MOVE	R9	R4
      0x7C1C0400,  //  0030  CALL	R7	2
      0x54220007,  //  0031  LDINT	R8	8
      0x381C0E08,  //  0032  SHL	R7	R7	R8
      0x30180C07,  //  0033  OR	R6	R6	R7
      0xB81E0A00,  //  0034  GETNGBL	R7	K5
      0x8C1C0F06,  //  0035  GETMET	R7	R7	K6
      0x5C240A00,  //  0036  MOVE	R9	R5
      0x7C1C0400,  //  0037  CALL	R7	2
      0x30180C07,  //  0038  OR	R6	R6	R7
      0x80040C00,  //  0039  RET	1	R6
      0x70020006,  //  003A  JMP		#0042
      0x541A000F,  //  003B  LDINT	R6	16
      0x38180606,  //  003C  SHL	R6	R3	R6
      0x541E0007,  //  003D  LDINT	R7	8
      0x381C0807,  //  003E  SHL	R7	R4	R7
      0x30180C07,  //  003F  OR	R6	R6	R7
      0x30180C05,  //  0040  OR	R6	R6	R5
      0x80040C00,  //  0041  RET	1	R6
      0x80000000,  //  0042  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_count
********************************************************************/
be_local_closure(pixel_count,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    }),
    (be_nested_const_str("pixel_count", -1855836553, 11)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0007,  //  0001  LDINT	R3	8
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_show
********************************************************************/
be_local_closure(can_show,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    /* K1   */  be_const_int(3),
    }),
    (be_nested_const_str("can_show", 960091187, 8)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_size
********************************************************************/
be_local_closure(pixel_size,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    }),
    (be_nested_const_str("pixel_size", -2085831511, 10)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0006,  //  0001  LDINT	R3	7
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dirty
********************************************************************/
be_local_closure(dirty,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    }),
    (be_nested_const_str("dirty", -1627386213, 5)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0004,  //  0001  LDINT	R3	5
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(show,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    /* K1   */  be_const_int(2),
    }),
    (be_nested_const_str("show", -1454906820, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear_to
********************************************************************/
be_local_closure(clear_to,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    /* K1   */  be_nested_string("to_gamma", 1597139862, 8),
    }),
    (be_nested_const_str("clear_to", -766965166, 8)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 8]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x54160008,  //  0001  LDINT	R5	9
      0x8C180101,  //  0002  GETMET	R6	R0	K1
      0x5C200200,  //  0003  MOVE	R8	R1
      0x5C240400,  //  0004  MOVE	R9	R2
      0x7C180600,  //  0005  CALL	R6	3
      0x7C0C0600,  //  0006  CALL	R3	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_dirty
********************************************************************/
be_local_closure(is_dirty,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    }),
    (be_nested_const_str("is_dirty", 418034110, 8)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0003,  //  0001  LDINT	R3	4
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ctor
********************************************************************/
be_local_closure(ctor,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("call_native", 1389147405, 11),
    /* K1   */  be_const_int(0),
    }),
    (be_nested_const_str("ctor", 375399343, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[16]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120005,  //  0002  JMPF	R4	#0009
      0x8C100100,  //  0003  GETMET	R4	R0	K0
      0x58180001,  //  0004  LDCONST	R6	K1
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x5C200400,  //  0006  MOVE	R8	R2
      0x7C100800,  //  0007  CALL	R4	4
      0x70020005,  //  0008  JMP		#000F
      0x8C100100,  //  0009  GETMET	R4	R0	K0
      0x58180001,  //  000A  LDCONST	R6	K1
      0x5C1C0200,  //  000B  MOVE	R7	R1
      0x5C200400,  //  000C  MOVE	R8	R2
      0x5C240600,  //  000D  MOVE	R9	R3
      0x7C100A00,  //  000E  CALL	R4	5
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds
********************************************************************/
extern const bclass be_class_Leds_ntv;
be_local_class(Leds,
    1,
    &be_class_Leds_ntv,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("get_pixel_color", 337490048, 15, -1), be_const_closure(get_pixel_color_closure) },
        { be_nested_key("pixels_buffer", 1229555807, 13, -1), be_const_closure(pixels_buffer_closure) },
        { be_nested_key("clear", 1550717474, 5, 13), be_const_closure(clear_closure) },
        { be_nested_key("init", 380752755, 4, 8), be_const_closure(init_closure) },
        { be_nested_key("set_pixel_color", 1275248356, 15, -1), be_const_closure(set_pixel_color_closure) },
        { be_nested_key("begin", 1748273790, 5, -1), be_const_closure(begin_closure) },
        { be_nested_key("to_gamma", 1597139862, 8, -1), be_const_closure(to_gamma_closure) },
        { be_nested_key("pixel_count", -1855836553, 11, -1), be_const_closure(pixel_count_closure) },
        { be_nested_key("can_show", 960091187, 8, -1), be_const_closure(can_show_closure) },
        { be_nested_key("pixel_size", -2085831511, 10, -1), be_const_closure(pixel_size_closure) },
        { be_nested_key("gamma", -802614262, 5, -1), be_const_var(0) },
        { be_nested_key("dirty", -1627386213, 5, -1), be_const_closure(dirty_closure) },
        { be_nested_key("show", -1454906820, 4, -1), be_const_closure(show_closure) },
        { be_nested_key("clear_to", -766965166, 8, -1), be_const_closure(clear_to_closure) },
        { be_nested_key("is_dirty", 418034110, 8, 5), be_const_closure(is_dirty_closure) },
        { be_nested_key("ctor", 375399343, 4, 1), be_const_closure(ctor_closure) },
    })),
    (be_nested_const_str("Leds", -1585722021, 4))
);
/*******************************************************************/

void be_load_Leds_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds);
    be_setglobal(vm, "Leds");
    be_pop(vm, 1);
}

#endif // USE_WS2812
