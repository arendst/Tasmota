/* Solidification of leds.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Leds;

extern const bclass be_class_Leds_segment;

/********************************************************************
** Solidified function: pixel_offset
********************************************************************/
be_local_closure(Leds_segment_pixel_offset,   /* name */
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
    /* K0   */  be_nested_str(offset),
    }),
    &be_const_str_pixel_offset,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear_to
********************************************************************/
be_local_closure(Leds_segment_clear_to,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(bri),
    /* K1   */  be_nested_str(strip),
    /* K2   */  be_nested_str(call_native),
    /* K3   */  be_nested_str(to_gamma),
    /* K4   */  be_nested_str(offset),
    /* K5   */  be_nested_str(leds),
    }),
    &be_const_str_clear_to,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x880C0101,  //  0004  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0005  GETMET	R3	R3	K2
      0x54160008,  //  0006  LDINT	R5	9
      0x88180101,  //  0007  GETMBR	R6	R0	K1
      0x8C180D03,  //  0008  GETMET	R6	R6	K3
      0x5C200200,  //  0009  MOVE	R8	R1
      0x5C240400,  //  000A  MOVE	R9	R2
      0x7C180600,  //  000B  CALL	R6	3
      0x881C0104,  //  000C  GETMBR	R7	R0	K4
      0x88200105,  //  000D  GETMBR	R8	R0	K5
      0x7C0C0A00,  //  000E  CALL	R3	5
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_count
********************************************************************/
be_local_closure(Leds_segment_pixel_count,   /* name */
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
    /* K0   */  be_nested_str(leds),
    }),
    &be_const_str_pixel_count,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(Leds_segment_pixels_buffer,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    &be_const_str_pixels_buffer,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dirty
********************************************************************/
be_local_closure(Leds_segment_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(dirty),
    }),
    &be_const_str_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_show
********************************************************************/
be_local_closure(Leds_segment_can_show,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(can_show),
    }),
    &be_const_str_can_show,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(Leds_segment_set_pixel_color,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(bri),
    /* K1   */  be_nested_str(strip),
    /* K2   */  be_nested_str(set_pixel_color),
    /* K3   */  be_nested_str(offset),
    }),
    &be_const_str_set_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120000,  //  0002  JMPF	R4	#0004
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x88100101,  //  0004  GETMBR	R4	R0	K1
      0x8C100902,  //  0005  GETMET	R4	R4	K2
      0x88180103,  //  0006  GETMBR	R6	R0	K3
      0x00180206,  //  0007  ADD	R6	R1	R6
      0x5C1C0400,  //  0008  MOVE	R7	R2
      0x5C200600,  //  0009  MOVE	R8	R3
      0x7C100800,  //  000A  CALL	R4	4
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_dirty
********************************************************************/
be_local_closure(Leds_segment_is_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(is_dirty),
    }),
    &be_const_str_is_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(Leds_segment_clear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(clear_to),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(show),
    }),
    &be_const_str_clear,
    &be_const_str_solidified,
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
** Solidified function: begin
********************************************************************/
be_local_closure(Leds_segment_begin,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    &be_const_str_begin,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pixel_color
********************************************************************/
be_local_closure(Leds_segment_get_pixel_color,   /* name */
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
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(get_pixel_color),
    /* K2   */  be_nested_str(offseta),
    }),
    &be_const_str_get_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x00100204,  //  0003  ADD	R4	R1	R4
      0x7C080400,  //  0004  CALL	R2	2
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_size
********************************************************************/
be_local_closure(Leds_segment_pixel_size,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(pixel_size),
    }),
    &be_const_str_pixel_size,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Leds_segment_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(offset),
    /* K2   */  be_nested_str(leds),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60100009,  //  0001  GETGBL	R4	G9
      0x5C140400,  //  0002  MOVE	R5	R2
      0x7C100200,  //  0003  CALL	R4	1
      0x90020204,  //  0004  SETMBR	R0	K1	R4
      0x60100009,  //  0005  GETGBL	R4	G9
      0x5C140600,  //  0006  MOVE	R5	R3
      0x7C100200,  //  0007  CALL	R4	1
      0x90020404,  //  0008  SETMBR	R0	K2	R4
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(Leds_segment_show,   /* name */
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
    /* K0   */  be_nested_str(offset),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(leds),
    /* K3   */  be_nested_str(strip),
    /* K4   */  be_nested_str(show),
    }),
    &be_const_str_show,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0007,  //  0003  JMPT	R2	#000C
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x1C080501,  //  0005  EQ	R2	R2	K1
      0x780A0007,  //  0006  JMPF	R2	#000F
      0x88080102,  //  0007  GETMBR	R2	R0	K2
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x880C0702,  //  0009  GETMBR	R3	R3	K2
      0x1C080403,  //  000A  EQ	R2	R2	R3
      0x780A0002,  //  000B  JMPF	R2	#000F
      0x88080103,  //  000C  GETMBR	R2	R0	K3
      0x8C080504,  //  000D  GETMET	R2	R2	K4
      0x7C080200,  //  000E  CALL	R2	1
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds_segment
********************************************************************/
be_local_class(Leds_segment,
    3,
    NULL,
    be_nested_map(17,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(pixel_offset, 9), be_const_closure(Leds_segment_pixel_offset_closure) },
        { be_const_key(clear_to, -1), be_const_closure(Leds_segment_clear_to_closure) },
        { be_const_key(show, -1), be_const_closure(Leds_segment_show_closure) },
        { be_const_key(pixels_buffer, 10), be_const_closure(Leds_segment_pixels_buffer_closure) },
        { be_const_key(offset, -1), be_const_var(1) },
        { be_const_key(dirty, -1), be_const_closure(Leds_segment_dirty_closure) },
        { be_const_key(can_show, -1), be_const_closure(Leds_segment_can_show_closure) },
        { be_const_key(set_pixel_color, 6), be_const_closure(Leds_segment_set_pixel_color_closure) },
        { be_const_key(get_pixel_color, -1), be_const_closure(Leds_segment_get_pixel_color_closure) },
        { be_const_key(pixel_count, -1), be_const_closure(Leds_segment_pixel_count_closure) },
        { be_const_key(strip, 7), be_const_var(0) },
        { be_const_key(leds, -1), be_const_var(2) },
        { be_const_key(begin, -1), be_const_closure(Leds_segment_begin_closure) },
        { be_const_key(is_dirty, 8), be_const_closure(Leds_segment_is_dirty_closure) },
        { be_const_key(pixel_size, -1), be_const_closure(Leds_segment_pixel_size_closure) },
        { be_const_key(init, -1), be_const_closure(Leds_segment_init_closure) },
        { be_const_key(clear, 2), be_const_closure(Leds_segment_clear_closure) },
    })),
    (bstring*) &be_const_str_Leds_segment
);

/********************************************************************
** Solidified function: create_segment
********************************************************************/
be_local_closure(Leds_create_segment,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(leds),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(value_error),
    /* K3   */  be_nested_str(out_X20of_X20range),
    /* K4   */  be_const_class(be_class_Leds_segment),
    }),
    &be_const_str_create_segment,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x600C0009,  //  0000  GETGBL	R3	G9
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x60100009,  //  0003  GETGBL	R4	G9
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C100200,  //  0005  CALL	R4	1
      0x000C0604,  //  0006  ADD	R3	R3	R4
      0x88100100,  //  0007  GETMBR	R4	R0	K0
      0x240C0604,  //  0008  GT	R3	R3	R4
      0x740E0003,  //  0009  JMPT	R3	#000E
      0x140C0301,  //  000A  LT	R3	R1	K1
      0x740E0001,  //  000B  JMPT	R3	#000E
      0x140C0501,  //  000C  LT	R3	R2	K1
      0x780E0000,  //  000D  JMPF	R3	#000F
      0xB0060503,  //  000E  RAISE	1	K2	K3
      0x580C0004,  //  000F  LDCONST	R3	K4
      0xB4000004,  //  0010  CLASS	K4
      0x5C100600,  //  0011  MOVE	R4	R3
      0x5C140000,  //  0012  MOVE	R5	R0
      0x5C180200,  //  0013  MOVE	R6	R1
      0x5C1C0400,  //  0014  MOVE	R7	R2
      0x7C100600,  //  0015  CALL	R4	3
      0x80040800,  //  0016  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bri
********************************************************************/
be_local_closure(Leds_set_bri,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(bri),
    }),
    &be_const_str_set_bri,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x14080300,  //  0000  LT	R2	R1	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040000,  //  0002  LDCONST	R1	K0
      0x540A00FE,  //  0003  LDINT	R2	255
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540600FE,  //  0006  LDINT	R1	255
      0x90020201,  //  0007  SETMBR	R0	K1	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(Leds_begin,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    /* K1   */  be_const_int(1),
    }),
    &be_const_str_begin,
    &be_const_str_solidified,
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
** Solidified function: clear
********************************************************************/
be_local_closure(Leds_clear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(clear_to),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(show),
    }),
    &be_const_str_clear,
    &be_const_str_solidified,
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
be_local_closure(Leds_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(gamma),
    /* K1   */  be_nested_str(leds),
    /* K2   */  be_nested_str(bri),
    /* K3   */  be_nested_str(valuer_error),
    /* K4   */  be_nested_str(no_X20GPIO_X20specified_X20for_X20neopixelbus),
    /* K5   */  be_nested_str(ctor),
    /* K6   */  be_nested_str(_p),
    /* K7   */  be_nested_str(internal_error),
    /* K8   */  be_nested_str(couldn_X27t_X20not_X20initialize_X20noepixelbus),
    /* K9   */  be_nested_str(begin),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x50140200,  //  0000  LDBOOL	R5	1	0
      0x90020005,  //  0001  SETMBR	R0	K0	R5
      0x60140009,  //  0002  GETGBL	R5	G9
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x90020205,  //  0005  SETMBR	R0	K1	R5
      0x5416007E,  //  0006  LDINT	R5	127
      0x90020405,  //  0007  SETMBR	R0	K2	R5
      0x4C140000,  //  0008  LDNIL	R5
      0x1C140405,  //  0009  EQ	R5	R2	R5
      0x78160000,  //  000A  JMPF	R5	#000C
      0xB0060704,  //  000B  RAISE	1	K3	K4
      0x8C140105,  //  000C  GETMET	R5	R0	K5
      0x881C0101,  //  000D  GETMBR	R7	R0	K1
      0x5C200400,  //  000E  MOVE	R8	R2
      0x5C240600,  //  000F  MOVE	R9	R3
      0x5C280800,  //  0010  MOVE	R10	R4
      0x7C140A00,  //  0011  CALL	R5	5
      0x88140106,  //  0012  GETMBR	R5	R0	K6
      0x4C180000,  //  0013  LDNIL	R6
      0x1C140A06,  //  0014  EQ	R5	R5	R6
      0x78160000,  //  0015  JMPF	R5	#0017
      0xB0060F08,  //  0016  RAISE	1	K7	K8
      0x8C140109,  //  0017  GETMET	R5	R0	K9
      0x7C140200,  //  0018  CALL	R5	1
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_gamma
********************************************************************/
be_local_closure(Leds_to_gamma,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(bri),
    /* K1   */  be_nested_str(apply_bri_gamma),
    /* K2   */  be_nested_str(gamma),
    }),
    &be_const_str_to_gamma,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x8C0C0101,  //  0004  GETMET	R3	R0	K1
      0x5C140200,  //  0005  MOVE	R5	R1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x881C0102,  //  0007  GETMBR	R7	R0	K2
      0x7C0C0800,  //  0008  CALL	R3	4
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(Leds_show,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    /* K1   */  be_const_int(2),
    }),
    &be_const_str_show,
    &be_const_str_solidified,
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
** Solidified function: pixel_count
********************************************************************/
be_local_closure(Leds_pixel_count,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    }),
    &be_const_str_pixel_count,
    &be_const_str_solidified,
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
** Solidified function: get_bri
********************************************************************/
be_local_closure(Leds_get_bri,   /* name */
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
    /* K0   */  be_nested_str(bri),
    }),
    &be_const_str_get_bri,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_gamma
********************************************************************/
be_local_closure(Leds_set_gamma,   /* name */
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
    /* K0   */  be_nested_str(gamma),
    }),
    &be_const_str_set_gamma,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pixel_color
********************************************************************/
be_local_closure(Leds_get_pixel_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    }),
    &be_const_str_get_pixel_color,
    &be_const_str_solidified,
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
** Solidified function: dirty
********************************************************************/
be_local_closure(Leds_dirty,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    }),
    &be_const_str_dirty,
    &be_const_str_solidified,
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
** Solidified function: matrix
********************************************************************/
be_local_closure(Leds_matrix,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_class(be_class_Leds),
    /* K1   */  be_nested_str(Leds),
    /* K2   */  be_nested_str(create_matrix),
    /* K3   */  be_const_int(0),
    }),
    &be_const_str_matrix,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x58100000,  //  0000  LDCONST	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x08180001,  //  0002  MUL	R6	R0	R1
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x5C200600,  //  0004  MOVE	R8	R3
      0x7C140600,  //  0005  CALL	R5	3
      0x8C180B02,  //  0006  GETMET	R6	R5	K2
      0x5C200000,  //  0007  MOVE	R8	R0
      0x5C240200,  //  0008  MOVE	R9	R1
      0x58280003,  //  0009  LDCONST	R10	K3
      0x7C180800,  //  000A  CALL	R6	4
      0x80040C00,  //  000B  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_offset
********************************************************************/
be_local_closure(Leds_pixel_offset,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_const_int(0),
    }),
    &be_const_str_pixel_offset,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80060000,  //  0000  RET	1	K0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear_to
********************************************************************/
be_local_closure(Leds_clear_to,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(bri),
    /* K1   */  be_nested_str(call_native),
    /* K2   */  be_nested_str(to_gamma),
    }),
    &be_const_str_clear_to,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x8C0C0101,  //  0004  GETMET	R3	R0	K1
      0x54160008,  //  0005  LDINT	R5	9
      0x8C180102,  //  0006  GETMET	R6	R0	K2
      0x5C200200,  //  0007  MOVE	R8	R1
      0x5C240400,  //  0008  MOVE	R9	R2
      0x7C180600,  //  0009  CALL	R6	3
      0x7C0C0600,  //  000A  CALL	R3	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(Leds_set_pixel_color,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(bri),
    /* K1   */  be_nested_str(call_native),
    /* K2   */  be_nested_str(to_gamma),
    }),
    &be_const_str_set_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120000,  //  0002  JMPF	R4	#0004
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x8C100101,  //  0004  GETMET	R4	R0	K1
      0x541A0009,  //  0005  LDINT	R6	10
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x8C200102,  //  0007  GETMET	R8	R0	K2
      0x5C280400,  //  0008  MOVE	R10	R2
      0x5C2C0600,  //  0009  MOVE	R11	R3
      0x7C200600,  //  000A  CALL	R8	3
      0x7C100800,  //  000B  CALL	R4	4
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_size
********************************************************************/
be_local_closure(Leds_pixel_size,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    }),
    &be_const_str_pixel_size,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0006,  //  0001  LDINT	R3	7
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


extern const bclass be_class_Leds_matrix;

/********************************************************************
** Solidified function: clear_to
********************************************************************/
be_local_closure(Leds_matrix_clear_to,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(bri),
    /* K1   */  be_nested_str(strip),
    /* K2   */  be_nested_str(call_native),
    /* K3   */  be_nested_str(to_gamma),
    /* K4   */  be_nested_str(offset),
    /* K5   */  be_nested_str(w),
    /* K6   */  be_nested_str(h),
    }),
    &be_const_str_clear_to,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x880C0101,  //  0004  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0005  GETMET	R3	R3	K2
      0x54160008,  //  0006  LDINT	R5	9
      0x88180101,  //  0007  GETMBR	R6	R0	K1
      0x8C180D03,  //  0008  GETMET	R6	R6	K3
      0x5C200200,  //  0009  MOVE	R8	R1
      0x5C240400,  //  000A  MOVE	R9	R2
      0x7C180600,  //  000B  CALL	R6	3
      0x881C0104,  //  000C  GETMBR	R7	R0	K4
      0x88200105,  //  000D  GETMBR	R8	R0	K5
      0x88240106,  //  000E  GETMBR	R9	R0	K6
      0x08201009,  //  000F  MUL	R8	R8	R9
      0x7C0C0A00,  //  0010  CALL	R3	5
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(Leds_matrix_show,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(offset),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(w),
    /* K3   */  be_nested_str(h),
    /* K4   */  be_nested_str(strip),
    /* K5   */  be_nested_str(leds),
    /* K6   */  be_nested_str(show),
    /* K7   */  be_nested_str(pix_buffer),
    /* K8   */  be_nested_str(pixels_buffer),
    }),
    &be_const_str_show,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0009,  //  0003  JMPT	R2	#000E
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x1C080501,  //  0005  EQ	R2	R2	K1
      0x780A000E,  //  0006  JMPF	R2	#0016
      0x88080102,  //  0007  GETMBR	R2	R0	K2
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x08080403,  //  0009  MUL	R2	R2	R3
      0x880C0104,  //  000A  GETMBR	R3	R0	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0007,  //  000D  JMPF	R2	#0016
      0x88080104,  //  000E  GETMBR	R2	R0	K4
      0x8C080506,  //  000F  GETMET	R2	R2	K6
      0x7C080200,  //  0010  CALL	R2	1
      0x88080104,  //  0011  GETMBR	R2	R0	K4
      0x8C080508,  //  0012  GETMET	R2	R2	K8
      0x88100107,  //  0013  GETMBR	R4	R0	K7
      0x7C080400,  //  0014  CALL	R2	2
      0x90020E02,  //  0015  SETMBR	R0	K7	R2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_size
********************************************************************/
be_local_closure(Leds_matrix_pixel_size,   /* name */
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
    /* K0   */  be_nested_str(pix_size),
    }),
    &be_const_str_pixel_size,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Leds_matrix_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(offset),
    /* K2   */  be_nested_str(h),
    /* K3   */  be_nested_str(w),
    /* K4   */  be_nested_str(alternate),
    /* K5   */  be_nested_str(pix_buffer),
    /* K6   */  be_nested_str(pixels_buffer),
    /* K7   */  be_nested_str(pix_size),
    /* K8   */  be_nested_str(pixel_size),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020204,  //  0001  SETMBR	R0	K1	R4
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x90020602,  //  0003  SETMBR	R0	K3	R2
      0x50140000,  //  0004  LDBOOL	R5	0	0
      0x90020805,  //  0005  SETMBR	R0	K4	R5
      0x88140100,  //  0006  GETMBR	R5	R0	K0
      0x8C140B06,  //  0007  GETMET	R5	R5	K6
      0x7C140200,  //  0008  CALL	R5	1
      0x90020A05,  //  0009  SETMBR	R0	K5	R5
      0x88140100,  //  000A  GETMBR	R5	R0	K0
      0x8C140B08,  //  000B  GETMET	R5	R5	K8
      0x7C140200,  //  000C  CALL	R5	1
      0x90020E05,  //  000D  SETMBR	R0	K7	R5
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(Leds_matrix_set_pixel_color,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(bri),
    /* K1   */  be_nested_str(strip),
    /* K2   */  be_nested_str(set_pixel_color),
    /* K3   */  be_nested_str(offset),
    }),
    &be_const_str_set_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120000,  //  0002  JMPF	R4	#0004
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x88100101,  //  0004  GETMBR	R4	R0	K1
      0x8C100902,  //  0005  GETMET	R4	R4	K2
      0x88180103,  //  0006  GETMBR	R6	R0	K3
      0x00180206,  //  0007  ADD	R6	R1	R6
      0x5C1C0400,  //  0008  MOVE	R7	R2
      0x5C200600,  //  0009  MOVE	R8	R3
      0x7C100800,  //  000A  CALL	R4	4
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(Leds_matrix_pixels_buffer,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(pixels_buffer),
    }),
    &be_const_str_pixels_buffer,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dirty
********************************************************************/
be_local_closure(Leds_matrix_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(dirty),
    }),
    &be_const_str_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_matrix_pixel_color
********************************************************************/
be_local_closure(Leds_matrix_set_matrix_pixel_color,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(bri),
    /* K1   */  be_nested_str(alternate),
    /* K2   */  be_const_int(2),
    /* K3   */  be_nested_str(strip),
    /* K4   */  be_nested_str(set_pixel_color),
    /* K5   */  be_nested_str(w),
    /* K6   */  be_nested_str(h),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str(offset),
    }),
    &be_const_str_set_matrix_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x88140101,  //  0004  GETMBR	R5	R0	K1
      0x7816000F,  //  0005  JMPF	R5	#0016
      0x10140302,  //  0006  MOD	R5	R1	K2
      0x7816000D,  //  0007  JMPF	R5	#0016
      0x88140103,  //  0008  GETMBR	R5	R0	K3
      0x8C140B04,  //  0009  GETMET	R5	R5	K4
      0x881C0105,  //  000A  GETMBR	R7	R0	K5
      0x081C0207,  //  000B  MUL	R7	R1	R7
      0x88200106,  //  000C  GETMBR	R8	R0	K6
      0x001C0E08,  //  000D  ADD	R7	R7	R8
      0x041C0E02,  //  000E  SUB	R7	R7	R2
      0x041C0F07,  //  000F  SUB	R7	R7	K7
      0x88200108,  //  0010  GETMBR	R8	R0	K8
      0x001C0E08,  //  0011  ADD	R7	R7	R8
      0x5C200600,  //  0012  MOVE	R8	R3
      0x5C240800,  //  0013  MOVE	R9	R4
      0x7C140800,  //  0014  CALL	R5	4
      0x70020009,  //  0015  JMP		#0020
      0x88140103,  //  0016  GETMBR	R5	R0	K3
      0x8C140B04,  //  0017  GETMET	R5	R5	K4
      0x881C0105,  //  0018  GETMBR	R7	R0	K5
      0x081C0207,  //  0019  MUL	R7	R1	R7
      0x001C0E02,  //  001A  ADD	R7	R7	R2
      0x88200108,  //  001B  GETMBR	R8	R0	K8
      0x001C0E08,  //  001C  ADD	R7	R7	R8
      0x5C200600,  //  001D  MOVE	R8	R3
      0x5C240800,  //  001E  MOVE	R9	R4
      0x7C140800,  //  001F  CALL	R5	4
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_dirty
********************************************************************/
be_local_closure(Leds_matrix_is_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(is_dirty),
    }),
    &be_const_str_is_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_alternate
********************************************************************/
be_local_closure(Leds_matrix_set_alternate,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(alternate),
    }),
    &be_const_str_set_alternate,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(Leds_matrix_begin,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    &be_const_str_begin,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_count
********************************************************************/
be_local_closure(Leds_matrix_pixel_count,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(w),
    /* K1   */  be_nested_str(h),
    }),
    &be_const_str_pixel_count,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x08040202,  //  0002  MUL	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_alternate
********************************************************************/
be_local_closure(Leds_matrix_get_alternate,   /* name */
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
    /* K0   */  be_nested_str(alternate),
    }),
    &be_const_str_get_alternate,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_offset
********************************************************************/
be_local_closure(Leds_matrix_pixel_offset,   /* name */
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
    /* K0   */  be_nested_str(offset),
    }),
    &be_const_str_pixel_offset,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_show
********************************************************************/
be_local_closure(Leds_matrix_can_show,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(can_show),
    }),
    &be_const_str_can_show,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bytes
********************************************************************/
be_local_closure(Leds_matrix_set_bytes,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(h),
    /* K1   */  be_nested_str(pix_size),
    /* K2   */  be_nested_str(offset),
    /* K3   */  be_nested_str(pix_buffer),
    /* K4   */  be_nested_str(setbytes),
    }),
    &be_const_str_set_bytes,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x88180101,  //  0001  GETMBR	R6	R0	K1
      0x08140A06,  //  0002  MUL	R5	R5	R6
      0x24180805,  //  0003  GT	R6	R4	R5
      0x781A0000,  //  0004  JMPF	R6	#0006
      0x5C100A00,  //  0005  MOVE	R4	R5
      0x88180102,  //  0006  GETMBR	R6	R0	K2
      0x00180C01,  //  0007  ADD	R6	R6	R1
      0x08180C05,  //  0008  MUL	R6	R6	R5
      0x881C0103,  //  0009  GETMBR	R7	R0	K3
      0x8C1C0F04,  //  000A  GETMET	R7	R7	K4
      0x5C240C00,  //  000B  MOVE	R9	R6
      0x5C280400,  //  000C  MOVE	R10	R2
      0x5C2C0600,  //  000D  MOVE	R11	R3
      0x5C300800,  //  000E  MOVE	R12	R4
      0x7C1C0A00,  //  000F  CALL	R7	5
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pixel_color
********************************************************************/
be_local_closure(Leds_matrix_get_pixel_color,   /* name */
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
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(get_pixel_color),
    /* K2   */  be_nested_str(offseta),
    }),
    &be_const_str_get_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x00100204,  //  0003  ADD	R4	R1	R4
      0x7C080400,  //  0004  CALL	R2	2
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(Leds_matrix_clear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(clear_to),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(show),
    }),
    &be_const_str_clear,
    &be_const_str_solidified,
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
** Solidified class: Leds_matrix
********************************************************************/
be_local_class(Leds_matrix,
    7,
    NULL,
    be_nested_map(25,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(Leds_matrix_init_closure) },
        { be_const_key(show, -1), be_const_closure(Leds_matrix_show_closure) },
        { be_const_key(alternate, -1), be_const_var(4) },
        { be_const_key(get_pixel_color, 14), be_const_closure(Leds_matrix_get_pixel_color_closure) },
        { be_const_key(is_dirty, -1), be_const_closure(Leds_matrix_is_dirty_closure) },
        { be_const_key(clear_to, 0), be_const_closure(Leds_matrix_clear_to_closure) },
        { be_const_key(set_pixel_color, -1), be_const_closure(Leds_matrix_set_pixel_color_closure) },
        { be_const_key(pixels_buffer, -1), be_const_closure(Leds_matrix_pixels_buffer_closure) },
        { be_const_key(dirty, -1), be_const_closure(Leds_matrix_dirty_closure) },
        { be_const_key(can_show, 21), be_const_closure(Leds_matrix_can_show_closure) },
        { be_const_key(pixel_size, 4), be_const_closure(Leds_matrix_pixel_size_closure) },
        { be_const_key(w, -1), be_const_var(3) },
        { be_const_key(set_matrix_pixel_color, 9), be_const_closure(Leds_matrix_set_matrix_pixel_color_closure) },
        { be_const_key(offset, -1), be_const_var(1) },
        { be_const_key(strip, -1), be_const_var(0) },
        { be_const_key(begin, -1), be_const_closure(Leds_matrix_begin_closure) },
        { be_const_key(h, -1), be_const_var(2) },
        { be_const_key(pix_size, -1), be_const_var(6) },
        { be_const_key(pixel_count, -1), be_const_closure(Leds_matrix_pixel_count_closure) },
        { be_const_key(get_alternate, -1), be_const_closure(Leds_matrix_get_alternate_closure) },
        { be_const_key(pixel_offset, -1), be_const_closure(Leds_matrix_pixel_offset_closure) },
        { be_const_key(set_alternate, -1), be_const_closure(Leds_matrix_set_alternate_closure) },
        { be_const_key(set_bytes, -1), be_const_closure(Leds_matrix_set_bytes_closure) },
        { be_const_key(pix_buffer, 3), be_const_var(5) },
        { be_const_key(clear, -1), be_const_closure(Leds_matrix_clear_closure) },
    })),
    (bstring*) &be_const_str_Leds_matrix
);

/********************************************************************
** Solidified function: create_matrix
********************************************************************/
be_local_closure(Leds_create_matrix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(leds),
    /* K2   */  be_nested_str(value_error),
    /* K3   */  be_nested_str(out_X20of_X20range),
    /* K4   */  be_const_class(be_class_Leds_matrix),
    }),
    &be_const_str_create_matrix,
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x60100009,  //  0000  GETGBL	R4	G9
      0x5C140600,  //  0001  MOVE	R5	R3
      0x7C100200,  //  0002  CALL	R4	1
      0x5C0C0800,  //  0003  MOVE	R3	R4
      0x60100009,  //  0004  GETGBL	R4	G9
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C100200,  //  0006  CALL	R4	1
      0x5C040800,  //  0007  MOVE	R1	R4
      0x60100009,  //  0008  GETGBL	R4	G9
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C100200,  //  000A  CALL	R4	1
      0x5C080800,  //  000B  MOVE	R2	R4
      0x4C100000,  //  000C  LDNIL	R4
      0x1C100604,  //  000D  EQ	R4	R3	R4
      0x78120000,  //  000E  JMPF	R4	#0010
      0x580C0000,  //  000F  LDCONST	R3	K0
      0x08100202,  //  0010  MUL	R4	R1	R2
      0x00100803,  //  0011  ADD	R4	R4	R3
      0x88140101,  //  0012  GETMBR	R5	R0	K1
      0x24100805,  //  0013  GT	R4	R4	R5
      0x74120005,  //  0014  JMPT	R4	#001B
      0x14100500,  //  0015  LT	R4	R2	K0
      0x74120003,  //  0016  JMPT	R4	#001B
      0x14100300,  //  0017  LT	R4	R1	K0
      0x74120001,  //  0018  JMPT	R4	#001B
      0x14100700,  //  0019  LT	R4	R3	K0
      0x78120000,  //  001A  JMPF	R4	#001C
      0xB0060503,  //  001B  RAISE	1	K2	K3
      0x58100004,  //  001C  LDCONST	R4	K4
      0xB4000004,  //  001D  CLASS	K4
      0x5C140800,  //  001E  MOVE	R5	R4
      0x5C180000,  //  001F  MOVE	R6	R0
      0x5C1C0200,  //  0020  MOVE	R7	R1
      0x5C200400,  //  0021  MOVE	R8	R2
      0x5C240600,  //  0022  MOVE	R9	R3
      0x7C140800,  //  0023  CALL	R5	4
      0x80040A00,  //  0024  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_gamma
********************************************************************/
be_local_closure(Leds_get_gamma,   /* name */
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
    /* K0   */  be_nested_str(gamma),
    }),
    &be_const_str_get_gamma,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_dirty
********************************************************************/
be_local_closure(Leds_is_dirty,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    }),
    &be_const_str_is_dirty,
    &be_const_str_solidified,
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
** Solidified function: can_show
********************************************************************/
be_local_closure(Leds_can_show,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    /* K1   */  be_const_int(3),
    }),
    &be_const_str_can_show,
    &be_const_str_solidified,
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
** Solidified function: assign_rmt
********************************************************************/
be_local_closure(Leds_assign_rmt,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_const_class(be_class_Leds),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(value_error),
    /* K3   */  be_nested_str(invalid_X20GPIO_X20number),
    /* K4   */  be_nested_str(global),
    /* K5   */  be_nested_str(contains),
    /* K6   */  be_nested_str(_rmt),
    /* K7   */  be_nested_str(gpio),
    /* K8   */  be_nested_str(MAX_RMT),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str(push),
    /* K11  */  be_nested_str(stop_iteration),
    /* K12  */  be_nested_str(pin_used),
    /* K13  */  be_nested_str(WS2812),
    /* K14  */  be_nested_str(pin),
    /* K15  */  be_nested_str(internal_error),
    /* K16  */  be_nested_str(no_X20more_X20RMT_X20channel_X20available),
    }),
    &be_const_str_assign_rmt,
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080009,  //  0001  GETGBL	R2	G9
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x5C000400,  //  0004  MOVE	R0	R2
      0x14080101,  //  0005  LT	R2	R0	K1
      0x780A0000,  //  0006  JMPF	R2	#0008
      0xB0060503,  //  0007  RAISE	1	K2	K3
      0xA40A0800,  //  0008  IMPORT	R2	K4
      0x4C0C0000,  //  0009  LDNIL	R3
      0x8C100505,  //  000A  GETMET	R4	R2	K5
      0x58180006,  //  000B  LDCONST	R6	K6
      0x7C100400,  //  000C  CALL	R4	2
      0x74120021,  //  000D  JMPT	R4	#0030
      0x60100012,  //  000E  GETGBL	R4	G18
      0x7C100000,  //  000F  CALL	R4	0
      0x5C0C0800,  //  0010  MOVE	R3	R4
      0x900A0C03,  //  0011  SETMBR	R2	K6	R3
      0x60100010,  //  0012  GETGBL	R4	G16
      0xB8160E00,  //  0013  GETNGBL	R5	K7
      0x88140B08,  //  0014  GETMBR	R5	R5	K8
      0x04140B09,  //  0015  SUB	R5	R5	K9
      0x40160205,  //  0016  CONNECT	R5	K1	R5
      0x7C100200,  //  0017  CALL	R4	1
      0xA8020005,  //  0018  EXBLK	0	#001F
      0x5C140800,  //  0019  MOVE	R5	R4
      0x7C140000,  //  001A  CALL	R5	0
      0x8C18070A,  //  001B  GETMET	R6	R3	K10
      0x5421FFFE,  //  001C  LDINT	R8	-1
      0x7C180400,  //  001D  CALL	R6	2
      0x7001FFF9,  //  001E  JMP		#0019
      0x5810000B,  //  001F  LDCONST	R4	K11
      0xAC100200,  //  0020  CATCH	R4	1	0
      0xB0080000,  //  0021  RAISE	2	R0	R0
      0xB8120E00,  //  0022  GETNGBL	R4	K7
      0x8C10090C,  //  0023  GETMET	R4	R4	K12
      0xB81A0E00,  //  0024  GETNGBL	R6	K7
      0x88180D0D,  //  0025  GETMBR	R6	R6	K13
      0x581C0001,  //  0026  LDCONST	R7	K1
      0x7C100600,  //  0027  CALL	R4	3
      0x78120006,  //  0028  JMPF	R4	#0030
      0xB8120E00,  //  0029  GETNGBL	R4	K7
      0x8C10090E,  //  002A  GETMET	R4	R4	K14
      0xB81A0E00,  //  002B  GETNGBL	R6	K7
      0x88180D0D,  //  002C  GETMBR	R6	R6	K13
      0x581C0001,  //  002D  LDCONST	R7	K1
      0x7C100600,  //  002E  CALL	R4	3
      0x980E0204,  //  002F  SETIDX	R3	K1	R4
      0x880C0506,  //  0030  GETMBR	R3	R2	K6
      0x58100001,  //  0031  LDCONST	R4	K1
      0x5415FFFE,  //  0032  LDINT	R5	-1
      0xB81A0E00,  //  0033  GETNGBL	R6	K7
      0x88180D08,  //  0034  GETMBR	R6	R6	K8
      0x14180806,  //  0035  LT	R6	R4	R6
      0x781A000A,  //  0036  JMPF	R6	#0042
      0x94180604,  //  0037  GETIDX	R6	R3	R4
      0x1C1C0C00,  //  0038  EQ	R7	R6	R0
      0x781E0000,  //  0039  JMPF	R7	#003B
      0x80040800,  //  003A  RET	1	R4
      0x141C0D01,  //  003B  LT	R7	R6	K1
      0x781E0002,  //  003C  JMPF	R7	#0040
      0x141C0B01,  //  003D  LT	R7	R5	K1
      0x781E0000,  //  003E  JMPF	R7	#0040
      0x5C140800,  //  003F  MOVE	R5	R4
      0x00100909,  //  0040  ADD	R4	R4	K9
      0x7001FFF0,  //  0041  JMP		#0033
      0x28180B01,  //  0042  GE	R6	R5	K1
      0x781A0001,  //  0043  JMPF	R6	#0046
      0x980C0A00,  //  0044  SETIDX	R3	R5	R0
      0x80040A00,  //  0045  RET	1	R5
      0xB0061F10,  //  0046  RAISE	1	K15	K16
      0x80000000,  //  0047  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ctor
********************************************************************/
be_local_closure(Leds_ctor,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(WS2812_GRB),
    /* K1   */  be_nested_str(assign_rmt),
    /* K2   */  be_nested_str(call_native),
    /* K3   */  be_const_int(0),
    }),
    &be_const_str_ctor,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140605,  //  0001  EQ	R5	R3	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x4C140000,  //  0004  LDNIL	R5
      0x1C140805,  //  0005  EQ	R5	R4	R5
      0x78160003,  //  0006  JMPF	R5	#000B
      0x8C140101,  //  0007  GETMET	R5	R0	K1
      0x5C1C0400,  //  0008  MOVE	R7	R2
      0x7C140400,  //  0009  CALL	R5	2
      0x5C100A00,  //  000A  MOVE	R4	R5
      0x8C140102,  //  000B  GETMET	R5	R0	K2
      0x581C0003,  //  000C  LDCONST	R7	K3
      0x5C200200,  //  000D  MOVE	R8	R1
      0x5C240400,  //  000E  MOVE	R9	R2
      0x5C280600,  //  000F  MOVE	R10	R3
      0x5C2C0800,  //  0010  MOVE	R11	R4
      0x7C140C00,  //  0011  CALL	R5	6
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(Leds_pixels_buffer,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    /* K1   */  be_nested_str(pixel_size),
    /* K2   */  be_nested_str(pixel_count),
    /* K3   */  be_nested_str(_change_buffer),
    }),
    &be_const_str_pixels_buffer,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x54120005,  //  0001  LDINT	R4	6
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0203,  //  0004  EQ	R3	R1	R3
      0x780E0009,  //  0005  JMPF	R3	#0010
      0x600C0015,  //  0006  GETGBL	R3	G21
      0x5C100400,  //  0007  MOVE	R4	R2
      0x8C140101,  //  0008  GETMET	R5	R0	K1
      0x7C140200,  //  0009  CALL	R5	1
      0x8C180102,  //  000A  GETMET	R6	R0	K2
      0x7C180200,  //  000B  CALL	R6	1
      0x08140A06,  //  000C  MUL	R5	R5	R6
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80040600,  //  000E  RET	1	R3
      0x70020003,  //  000F  JMP		#0014
      0x8C0C0303,  //  0010  GETMET	R3	R1	K3
      0x5C140400,  //  0011  MOVE	R5	R2
      0x7C0C0400,  //  0012  CALL	R3	2
      0x80040200,  //  0013  RET	1	R1
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds
********************************************************************/
extern const bclass be_class_Leds_ntv;
be_local_class(Leds,
    3,
    &be_class_Leds_ntv,
    be_nested_map(27,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(leds, -1), be_const_var(1) },
        { be_const_key(create_segment, 25), be_const_closure(Leds_create_segment_closure) },
        { be_const_key(clear, -1), be_const_closure(Leds_clear_closure) },
        { be_const_key(begin, -1), be_const_closure(Leds_begin_closure) },
        { be_const_key(ctor, 7), be_const_closure(Leds_ctor_closure) },
        { be_const_key(assign_rmt, 12), be_const_static_closure(Leds_assign_rmt_closure) },
        { be_const_key(to_gamma, 8), be_const_closure(Leds_to_gamma_closure) },
        { be_const_key(dirty, -1), be_const_closure(Leds_dirty_closure) },
        { be_const_key(matrix, -1), be_const_static_closure(Leds_matrix_closure) },
        { be_const_key(pixel_offset, 2), be_const_closure(Leds_pixel_offset_closure) },
        { be_const_key(set_gamma, 4), be_const_closure(Leds_set_gamma_closure) },
        { be_const_key(get_pixel_color, -1), be_const_closure(Leds_get_pixel_color_closure) },
        { be_const_key(pixel_size, -1), be_const_closure(Leds_pixel_size_closure) },
        { be_const_key(create_matrix, -1), be_const_closure(Leds_create_matrix_closure) },
        { be_const_key(set_bri, 9), be_const_closure(Leds_set_bri_closure) },
        { be_const_key(clear_to, -1), be_const_closure(Leds_clear_to_closure) },
        { be_const_key(set_pixel_color, -1), be_const_closure(Leds_set_pixel_color_closure) },
        { be_const_key(gamma, -1), be_const_var(0) },
        { be_const_key(pixel_count, 17), be_const_closure(Leds_pixel_count_closure) },
        { be_const_key(get_bri, 13), be_const_closure(Leds_get_bri_closure) },
        { be_const_key(get_gamma, -1), be_const_closure(Leds_get_gamma_closure) },
        { be_const_key(bri, -1), be_const_var(2) },
        { be_const_key(is_dirty, -1), be_const_closure(Leds_is_dirty_closure) },
        { be_const_key(can_show, -1), be_const_closure(Leds_can_show_closure) },
        { be_const_key(init, 5), be_const_closure(Leds_init_closure) },
        { be_const_key(show, -1), be_const_closure(Leds_show_closure) },
        { be_const_key(pixels_buffer, -1), be_const_closure(Leds_pixels_buffer_closure) },
    })),
    (bstring*) &be_const_str_Leds
);
/*******************************************************************/

void be_load_Leds_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds);
    be_setglobal(vm, "Leds");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
