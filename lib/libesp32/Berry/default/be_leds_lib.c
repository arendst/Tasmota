/********************************************************************
 * Berry class `Leds`
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WS2812

/********************************************************************
** Solidified function: pixel_count
********************************************************************/
be_local_closure(Leds_matrix_pixel_count,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified function: set_alternate
********************************************************************/
be_local_closure(Leds_matrix_set_alternate,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
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
** Solidified function: pixel_size
********************************************************************/
be_local_closure(Leds_matrix_pixel_size,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(Leds_matrix_set_pixel_color,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(set_pixel_color),
    /* K2   */  be_nested_str(offset),
    }),
    &be_const_str_set_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0x88180102,  //  0002  GETMBR	R6	R0	K2
      0x00180206,  //  0003  ADD	R6	R1	R6
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x5C200600,  //  0005  MOVE	R8	R3
      0x7C100800,  //  0006  CALL	R4	4
      0x80000000,  //  0007  RET	0
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
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(alternate),
    /* K1   */  be_const_int(2),
    /* K2   */  be_nested_str(strip),
    /* K3   */  be_nested_str(set_pixel_color),
    /* K4   */  be_nested_str(w),
    /* K5   */  be_nested_str(h),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str(offset),
    }),
    &be_const_str_set_matrix_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x7816000F,  //  0001  JMPF	R5	#0012
      0x10140301,  //  0002  MOD	R5	R1	K1
      0x7816000D,  //  0003  JMPF	R5	#0012
      0x88140102,  //  0004  GETMBR	R5	R0	K2
      0x8C140B03,  //  0005  GETMET	R5	R5	K3
      0x881C0104,  //  0006  GETMBR	R7	R0	K4
      0x081C0207,  //  0007  MUL	R7	R1	R7
      0x88200105,  //  0008  GETMBR	R8	R0	K5
      0x001C0E08,  //  0009  ADD	R7	R7	R8
      0x041C0E02,  //  000A  SUB	R7	R7	R2
      0x041C0F06,  //  000B  SUB	R7	R7	K6
      0x88200107,  //  000C  GETMBR	R8	R0	K7
      0x001C0E08,  //  000D  ADD	R7	R7	R8
      0x5C200600,  //  000E  MOVE	R8	R3
      0x5C240800,  //  000F  MOVE	R9	R4
      0x7C140800,  //  0010  CALL	R5	4
      0x70020009,  //  0011  JMP		#001C
      0x88140102,  //  0012  GETMBR	R5	R0	K2
      0x8C140B03,  //  0013  GETMET	R5	R5	K3
      0x881C0104,  //  0014  GETMBR	R7	R0	K4
      0x081C0207,  //  0015  MUL	R7	R1	R7
      0x001C0E02,  //  0016  ADD	R7	R7	R2
      0x88200107,  //  0017  GETMBR	R8	R0	K7
      0x001C0E08,  //  0018  ADD	R7	R7	R8
      0x5C200600,  //  0019  MOVE	R8	R3
      0x5C240800,  //  001A  MOVE	R9	R4
      0x7C140800,  //  001B  CALL	R5	4
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(Leds_matrix_show,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(offset),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(w),
    /* K3   */  be_nested_str(h),
    /* K4   */  be_nested_str(strip),
    /* K5   */  be_nested_str(leds),
    /* K6   */  be_nested_str(show),
    }),
    &be_const_str_show,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0009,  //  0003  JMPT	R2	#000E
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x1C080501,  //  0005  EQ	R2	R2	K1
      0x780A0009,  //  0006  JMPF	R2	#0011
      0x88080102,  //  0007  GETMBR	R2	R0	K2
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x08080403,  //  0009  MUL	R2	R2	R3
      0x880C0104,  //  000A  GETMBR	R3	R0	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0002,  //  000D  JMPF	R2	#0011
      0x88080104,  //  000E  GETMBR	R2	R0	K4
      0x8C080506,  //  000F  GETMET	R2	R2	K6
      0x7C080200,  //  0010  CALL	R2	1
      0x80000000,  //  0011  RET	0
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
    0,                          /* varg */
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
** Solidified function: clear_to
********************************************************************/
be_local_closure(Leds_matrix_clear_to,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(w),
    /* K2   */  be_nested_str(h),
    /* K3   */  be_nested_str(strip),
    /* K4   */  be_nested_str(set_pixel_color),
    /* K5   */  be_nested_str(offset),
    /* K6   */  be_const_int(1),
    }),
    &be_const_str_clear_to,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x08100805,  //  0003  MUL	R4	R4	R5
      0x14100604,  //  0004  LT	R4	R3	R4
      0x78120008,  //  0005  JMPF	R4	#000F
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x88180105,  //  0008  GETMBR	R6	R0	K5
      0x00180606,  //  0009  ADD	R6	R3	R6
      0x5C1C0200,  //  000A  MOVE	R7	R1
      0x5C200400,  //  000B  MOVE	R8	R2
      0x7C100800,  //  000C  CALL	R4	4
      0x000C0706,  //  000D  ADD	R3	R3	K6
      0x7001FFF1,  //  000E  JMP		#0001
      0x80000000,  //  000F  RET	0
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
    0,                          /* varg */
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
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(Leds_matrix_pixels_buffer,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified function: init
********************************************************************/
be_local_closure(Leds_matrix_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(offset),
    /* K2   */  be_nested_str(h),
    /* K3   */  be_nested_str(w),
    /* K4   */  be_nested_str(alternate),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020204,  //  0001  SETMBR	R0	K1	R4
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x90020602,  //  0003  SETMBR	R0	K3	R2
      0x50140000,  //  0004  LDBOOL	R5	0	0
      0x90020805,  //  0005  SETMBR	R0	K4	R5
      0x80000000,  //  0006  RET	0
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
    0,                          /* varg */
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
** Solidified function: get_pixel_color
********************************************************************/
be_local_closure(Leds_matrix_get_pixel_color,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
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
** Solidified function: get_alternate
********************************************************************/
be_local_closure(Leds_matrix_get_alternate,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified function: begin
********************************************************************/
be_local_closure(Leds_matrix_begin,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified function: can_show
********************************************************************/
be_local_closure(Leds_matrix_can_show,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified class: Leds_matrix
********************************************************************/
be_local_class(Leds_matrix,
    5,
    NULL,
    be_nested_map(21,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(pixel_count, -1), be_const_closure(Leds_matrix_pixel_count_closure) },
        { be_const_key(h, 6), be_const_var(2) },
        { be_const_key(set_alternate, 7), be_const_closure(Leds_matrix_set_alternate_closure) },
        { be_const_key(pixel_size, 16), be_const_closure(Leds_matrix_pixel_size_closure) },
        { be_const_key(set_pixel_color, 19), be_const_closure(Leds_matrix_set_pixel_color_closure) },
        { be_const_key(set_matrix_pixel_color, 10), be_const_closure(Leds_matrix_set_matrix_pixel_color_closure) },
        { be_const_key(show, -1), be_const_closure(Leds_matrix_show_closure) },
        { be_const_key(alternate, -1), be_const_var(4) },
        { be_const_key(strip, -1), be_const_var(0) },
        { be_const_key(clear_to, -1), be_const_closure(Leds_matrix_clear_to_closure) },
        { be_const_key(w, 15), be_const_var(3) },
        { be_const_key(pixels_buffer, -1), be_const_closure(Leds_matrix_pixels_buffer_closure) },
        { be_const_key(init, -1), be_const_closure(Leds_matrix_init_closure) },
        { be_const_key(dirty, -1), be_const_closure(Leds_matrix_dirty_closure) },
        { be_const_key(get_pixel_color, -1), be_const_closure(Leds_matrix_get_pixel_color_closure) },
        { be_const_key(get_alternate, 17), be_const_closure(Leds_matrix_get_alternate_closure) },
        { be_const_key(offset, 8), be_const_var(1) },
        { be_const_key(clear, -1), be_const_closure(Leds_matrix_clear_closure) },
        { be_const_key(begin, -1), be_const_closure(Leds_matrix_begin_closure) },
        { be_const_key(is_dirty, -1), be_const_closure(Leds_matrix_is_dirty_closure) },
        { be_const_key(can_show, -1), be_const_closure(Leds_matrix_can_show_closure) },
    })),
    be_str_literal("Leds_matrix")
);

/********************************************************************
** Solidified function: create_matrix
********************************************************************/
be_local_closure(Leds_create_matrix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
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
** Solidified function: begin
********************************************************************/
be_local_closure(Leds_begin,   /* name */
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
** Solidified function: to_gamma
********************************************************************/
be_local_closure(Leds_to_gamma,   /* name */
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
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(scale_uint),
    /* K2   */  be_const_int(0),
    /* K3   */  be_const_int(16711680),
    /* K4   */  be_nested_str(gamma),
    /* K5   */  be_nested_str(light),
    /* K6   */  be_nested_str(gamma8),
    }),
    &be_const_str_to_gamma,
    &be_const_str_solidified,
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
be_local_closure(Leds_pixel_count,   /* name */
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
** Solidified function: matrix
********************************************************************/
be_local_closure(Leds_matrix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(Leds),
    /* K1   */  be_nested_str(create_matrix),
    /* K2   */  be_const_int(0),
    }),
    &be_const_str_matrix,
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x08140001,  //  0001  MUL	R5	R0	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x5C1C0600,  //  0003  MOVE	R7	R3
      0x7C100600,  //  0004  CALL	R4	3
      0x8C140901,  //  0005  GETMET	R5	R4	K1
      0x5C1C0000,  //  0006  MOVE	R7	R0
      0x5C200200,  //  0007  MOVE	R8	R1
      0x58240002,  //  0008  LDCONST	R9	K2
      0x7C140800,  //  0009  CALL	R5	4
      0x80040A00,  //  000A  RET	1	R5
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
    0,                          /* varg */
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


/********************************************************************
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(Leds_pixels_buffer,   /* name */
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
    /* K0   */  be_nested_str(call_native),
    }),
    &be_const_str_pixels_buffer,
    &be_const_str_solidified,
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
** Solidified function: get_pixel_color
********************************************************************/
be_local_closure(Leds_get_pixel_color,   /* name */
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
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(Leds_set_pixel_color,   /* name */
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
    /* K0   */  be_nested_str(call_native),
    /* K1   */  be_nested_str(to_gamma),
    }),
    &be_const_str_set_pixel_color,
    &be_const_str_solidified,
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
** Solidified function: is_dirty
********************************************************************/
be_local_closure(Leds_is_dirty,   /* name */
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
** Solidified function: init
********************************************************************/
be_local_closure(Leds_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(gamma),
    /* K1   */  be_nested_str(leds),
    /* K2   */  be_nested_str(pin),
    /* K3   */  be_nested_str(WS2812),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str(valuer_error),
    /* K6   */  be_nested_str(no_X20GPIO_X20specified_X20for_X20neopixelbus),
    /* K7   */  be_nested_str(ctor),
    /* K8   */  be_nested_str(_p),
    /* K9   */  be_nested_str(internal_error),
    /* K10  */  be_nested_str(couldn_X27t_X20not_X20initialize_X20noepixelbus),
    /* K11  */  be_nested_str(begin),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x50140200,  //  0000  LDBOOL	R5	1	0
      0x90020005,  //  0001  SETMBR	R0	K0	R5
      0x60140009,  //  0002  GETGBL	R5	G9
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x90020205,  //  0005  SETMBR	R0	K1	R5
      0x4C140000,  //  0006  LDNIL	R5
      0x1C140405,  //  0007  EQ	R5	R2	R5
      0x78160008,  //  0008  JMPF	R5	#0012
      0x8C140502,  //  0009  GETMET	R5	R2	K2
      0x881C0503,  //  000A  GETMBR	R7	R2	K3
      0x7C140400,  //  000B  CALL	R5	2
      0x28140B04,  //  000C  GE	R5	R5	K4
      0x78160003,  //  000D  JMPF	R5	#0012
      0x8C140502,  //  000E  GETMET	R5	R2	K2
      0x881C0503,  //  000F  GETMBR	R7	R2	K3
      0x7C140400,  //  0010  CALL	R5	2
      0x5C080A00,  //  0011  MOVE	R2	R5
      0x4C140000,  //  0012  LDNIL	R5
      0x1C140405,  //  0013  EQ	R5	R2	R5
      0x78160000,  //  0014  JMPF	R5	#0016
      0xB0060B06,  //  0015  RAISE	1	K5	K6
      0x8C140107,  //  0016  GETMET	R5	R0	K7
      0x881C0101,  //  0017  GETMBR	R7	R0	K1
      0x5C200400,  //  0018  MOVE	R8	R2
      0x5C240600,  //  0019  MOVE	R9	R3
      0x5C280800,  //  001A  MOVE	R10	R4
      0x7C140A00,  //  001B  CALL	R5	5
      0x88140108,  //  001C  GETMBR	R5	R0	K8
      0x4C180000,  //  001D  LDNIL	R6
      0x1C140A06,  //  001E  EQ	R5	R5	R6
      0x78160000,  //  001F  JMPF	R5	#0021
      0xB006130A,  //  0020  RAISE	1	K9	K10
      0x8C14010B,  //  0021  GETMET	R5	R0	K11
      0x7C140200,  //  0022  CALL	R5	1
      0x80000000,  //  0023  RET	0
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
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(call_native),
    /* K1   */  be_nested_str(to_gamma),
    }),
    &be_const_str_clear_to,
    &be_const_str_solidified,
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
** Solidified function: can_show
********************************************************************/
be_local_closure(Leds_can_show,   /* name */
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
** Solidified function: clear
********************************************************************/
be_local_closure(Leds_clear,   /* name */
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
** Solidified function: show
********************************************************************/
be_local_closure(Leds_show,   /* name */
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
** Solidified function: ctor
********************************************************************/
be_local_closure(Leds_ctor,   /* name */
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
    /* K0   */  be_nested_str(call_native),
    /* K1   */  be_const_int(0),
    }),
    &be_const_str_ctor,
    &be_const_str_solidified,
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
** Solidified function: dirty
********************************************************************/
be_local_closure(Leds_dirty,   /* name */
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
** Solidified function: get_pixel_color
********************************************************************/
be_local_closure(Leds_segment_get_pixel_color,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
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
** Solidified function: clear_to
********************************************************************/
be_local_closure(Leds_segment_clear_to,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(leds),
    /* K2   */  be_nested_str(strip),
    /* K3   */  be_nested_str(set_pixel_color),
    /* K4   */  be_nested_str(offset),
    /* K5   */  be_const_int(1),
    }),
    &be_const_str_clear_to,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x14100604,  //  0002  LT	R4	R3	R4
      0x78120008,  //  0003  JMPF	R4	#000D
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x00180606,  //  0007  ADD	R6	R3	R6
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x5C200400,  //  0009  MOVE	R8	R2
      0x7C100800,  //  000A  CALL	R4	4
      0x000C0705,  //  000B  ADD	R3	R3	K5
      0x7001FFF3,  //  000C  JMP		#0001
      0x80000000,  //  000D  RET	0
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
    0,                          /* varg */
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
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(strip),
    /* K1   */  be_nested_str(set_pixel_color),
    /* K2   */  be_nested_str(offset),
    }),
    &be_const_str_set_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0x88180102,  //  0002  GETMBR	R6	R0	K2
      0x00180206,  //  0003  ADD	R6	R1	R6
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x5C200600,  //  0005  MOVE	R8	R3
      0x7C100800,  //  0006  CALL	R4	4
      0x80000000,  //  0007  RET	0
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
    0,                          /* varg */
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
    0,                          /* varg */
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
be_local_closure(Leds_segment_pixel_count,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified function: init
********************************************************************/
be_local_closure(Leds_segment_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
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
** Solidified function: pixel_size
********************************************************************/
be_local_closure(Leds_segment_pixel_size,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified function: dirty
********************************************************************/
be_local_closure(Leds_segment_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified function: show
********************************************************************/
be_local_closure(Leds_segment_show,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
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
** Solidified function: is_dirty
********************************************************************/
be_local_closure(Leds_segment_is_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(Leds_segment_pixels_buffer,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
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
** Solidified class: Leds_segment
********************************************************************/
be_local_class(Leds_segment,
    3,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(get_pixel_color, -1), be_const_closure(Leds_segment_get_pixel_color_closure) },
        { be_const_key(strip, -1), be_const_var(0) },
        { be_const_key(clear_to, 5), be_const_closure(Leds_segment_clear_to_closure) },
        { be_const_key(can_show, 13), be_const_closure(Leds_segment_can_show_closure) },
        { be_const_key(set_pixel_color, -1), be_const_closure(Leds_segment_set_pixel_color_closure) },
        { be_const_key(clear, -1), be_const_closure(Leds_segment_clear_closure) },
        { be_const_key(is_dirty, -1), be_const_closure(Leds_segment_is_dirty_closure) },
        { be_const_key(pixel_count, -1), be_const_closure(Leds_segment_pixel_count_closure) },
        { be_const_key(leds, -1), be_const_var(2) },
        { be_const_key(pixel_size, -1), be_const_closure(Leds_segment_pixel_size_closure) },
        { be_const_key(offset, -1), be_const_var(1) },
        { be_const_key(dirty, 8), be_const_closure(Leds_segment_dirty_closure) },
        { be_const_key(show, -1), be_const_closure(Leds_segment_show_closure) },
        { be_const_key(init, -1), be_const_closure(Leds_segment_init_closure) },
        { be_const_key(begin, 6), be_const_closure(Leds_segment_begin_closure) },
        { be_const_key(pixels_buffer, -1), be_const_closure(Leds_segment_pixels_buffer_closure) },
    })),
    be_str_literal("Leds_segment")
);

/********************************************************************
** Solidified function: create_segment
********************************************************************/
be_local_closure(Leds_create_segment,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
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
** Solidified class: Leds
********************************************************************/
extern const bclass be_class_Leds_ntv;
be_local_class(Leds,
    2,
    &be_class_Leds_ntv,
    be_nested_map(20,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(pixel_count, -1), be_const_closure(Leds_pixel_count_closure) },
        { be_const_key(dirty, 6), be_const_closure(Leds_dirty_closure) },
        { be_const_key(to_gamma, -1), be_const_closure(Leds_to_gamma_closure) },
        { be_const_key(create_matrix, 1), be_const_closure(Leds_create_matrix_closure) },
        { be_const_key(matrix, -1), be_const_static_closure(Leds_matrix_closure) },
        { be_const_key(pixel_size, -1), be_const_closure(Leds_pixel_size_closure) },
        { be_const_key(ctor, 0), be_const_closure(Leds_ctor_closure) },
        { be_const_key(pixels_buffer, 13), be_const_closure(Leds_pixels_buffer_closure) },
        { be_const_key(get_pixel_color, -1), be_const_closure(Leds_get_pixel_color_closure) },
        { be_const_key(show, -1), be_const_closure(Leds_show_closure) },
        { be_const_key(begin, 17), be_const_closure(Leds_begin_closure) },
        { be_const_key(leds, -1), be_const_var(1) },
        { be_const_key(clear, -1), be_const_closure(Leds_clear_closure) },
        { be_const_key(can_show, -1), be_const_closure(Leds_can_show_closure) },
        { be_const_key(gamma, 12), be_const_var(0) },
        { be_const_key(init, 11), be_const_closure(Leds_init_closure) },
        { be_const_key(set_pixel_color, 9), be_const_closure(Leds_set_pixel_color_closure) },
        { be_const_key(clear_to, 18), be_const_closure(Leds_clear_to_closure) },
        { be_const_key(is_dirty, -1), be_const_closure(Leds_is_dirty_closure) },
        { be_const_key(create_segment, -1), be_const_closure(Leds_create_segment_closure) },
    })),
    be_str_literal("Leds")
);
/*******************************************************************/

void be_load_Leds_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds);
    be_setglobal(vm, "Leds");
    be_pop(vm, 1);
}

#endif // USE_WS2812
