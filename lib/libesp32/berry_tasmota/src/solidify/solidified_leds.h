/* Solidification of leds.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Leds_segment;
extern const bclass be_class_Leds;
extern const bclass be_class_Leds_matrix;
// compact class 'Leds_segment' ktab size: 16, total: 34 (saved 144 bytes)
static const bvalue be_ktab_class_Leds_segment[16] = {
  /* K0   */  be_nested_str(offset),
  /* K1   */  be_nested_str(bri),
  /* K2   */  be_nested_str(strip),
  /* K3   */  be_nested_str(call_native),
  /* K4   */  be_nested_str(to_gamma),
  /* K5   */  be_nested_str(leds),
  /* K6   */  be_nested_str(dirty),
  /* K7   */  be_nested_str(can_show),
  /* K8   */  be_nested_str(set_pixel_color),
  /* K9   */  be_nested_str(is_dirty),
  /* K10  */  be_nested_str(clear_to),
  /* K11  */  be_const_int(0),
  /* K12  */  be_nested_str(show),
  /* K13  */  be_nested_str(get_pixel_color),
  /* K14  */  be_nested_str(offseta),
  /* K15  */  be_nested_str(pixel_size),
};


extern const bclass be_class_Leds_segment;

/********************************************************************
** Solidified function: pixel_offset
********************************************************************/
be_local_closure(class_Leds_segment_pixel_offset,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
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
be_local_closure(class_Leds_segment_clear_to,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_clear_to,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x880C0102,  //  0004  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0005  GETMET	R3	R3	K3
      0x54160008,  //  0006  LDINT	R5	9
      0x88180102,  //  0007  GETMBR	R6	R0	K2
      0x8C180D04,  //  0008  GETMET	R6	R6	K4
      0x5C200200,  //  0009  MOVE	R8	R1
      0x5C240400,  //  000A  MOVE	R9	R2
      0x7C180600,  //  000B  CALL	R6	3
      0x881C0100,  //  000C  GETMBR	R7	R0	K0
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
be_local_closure(class_Leds_segment_pixel_count,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_pixel_count,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(class_Leds_segment_pixels_buffer,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
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
be_local_closure(class_Leds_segment_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x8C040306,  //  0001  GETMET	R1	R1	K6
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_show
********************************************************************/
be_local_closure(class_Leds_segment_can_show,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_can_show,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x8C040307,  //  0001  GETMET	R1	R1	K7
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(class_Leds_segment_set_pixel_color,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_set_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120000,  //  0002  JMPF	R4	#0004
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x8C100908,  //  0005  GETMET	R4	R4	K8
      0x88180100,  //  0006  GETMBR	R6	R0	K0
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
be_local_closure(class_Leds_segment_is_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_is_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x8C040309,  //  0001  GETMET	R1	R1	K9
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(class_Leds_segment_clear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_clear,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C04010A,  //  0000  GETMET	R1	R0	K10
      0x580C000B,  //  0001  LDCONST	R3	K11
      0x7C040400,  //  0002  CALL	R1	2
      0x8C04010C,  //  0003  GETMET	R1	R0	K12
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(class_Leds_segment_begin,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
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
be_local_closure(class_Leds_segment_get_pixel_color,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_get_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C08050D,  //  0001  GETMET	R2	R2	K13
      0x8810010E,  //  0002  GETMBR	R4	R0	K14
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
be_local_closure(class_Leds_segment_pixel_size,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_pixel_size,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x8C04030F,  //  0001  GETMET	R1	R1	K15
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Leds_segment_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x90020401,  //  0000  SETMBR	R0	K2	R1
      0x60100009,  //  0001  GETGBL	R4	G9
      0x5C140400,  //  0002  MOVE	R5	R2
      0x7C100200,  //  0003  CALL	R4	1
      0x90020004,  //  0004  SETMBR	R0	K0	R4
      0x60100009,  //  0005  GETGBL	R4	G9
      0x5C140600,  //  0006  MOVE	R5	R3
      0x7C100200,  //  0007  CALL	R4	1
      0x90020A04,  //  0008  SETMBR	R0	K5	R4
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(class_Leds_segment_show,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_show,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0007,  //  0003  JMPT	R2	#000C
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x1C08050B,  //  0005  EQ	R2	R2	K11
      0x780A0007,  //  0006  JMPF	R2	#000F
      0x88080105,  //  0007  GETMBR	R2	R0	K5
      0x880C0102,  //  0008  GETMBR	R3	R0	K2
      0x880C0705,  //  0009  GETMBR	R3	R3	K5
      0x1C080403,  //  000A  EQ	R2	R2	R3
      0x780A0002,  //  000B  JMPF	R2	#000F
      0x88080102,  //  000C  GETMBR	R2	R0	K2
      0x8C08050C,  //  000D  GETMET	R2	R2	K12
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
        { be_const_key(pixel_offset, 9), be_const_closure(class_Leds_segment_pixel_offset_closure) },
        { be_const_key(clear_to, -1), be_const_closure(class_Leds_segment_clear_to_closure) },
        { be_const_key(show, -1), be_const_closure(class_Leds_segment_show_closure) },
        { be_const_key(pixels_buffer, 10), be_const_closure(class_Leds_segment_pixels_buffer_closure) },
        { be_const_key(offset, -1), be_const_var(1) },
        { be_const_key(dirty, -1), be_const_closure(class_Leds_segment_dirty_closure) },
        { be_const_key(can_show, -1), be_const_closure(class_Leds_segment_can_show_closure) },
        { be_const_key(set_pixel_color, 6), be_const_closure(class_Leds_segment_set_pixel_color_closure) },
        { be_const_key(get_pixel_color, -1), be_const_closure(class_Leds_segment_get_pixel_color_closure) },
        { be_const_key(pixel_count, -1), be_const_closure(class_Leds_segment_pixel_count_closure) },
        { be_const_key(strip, 7), be_const_var(0) },
        { be_const_key(leds, -1), be_const_var(2) },
        { be_const_key(begin, -1), be_const_closure(class_Leds_segment_begin_closure) },
        { be_const_key(is_dirty, 8), be_const_closure(class_Leds_segment_is_dirty_closure) },
        { be_const_key(pixel_size, -1), be_const_closure(class_Leds_segment_pixel_size_closure) },
        { be_const_key(init, -1), be_const_closure(class_Leds_segment_init_closure) },
        { be_const_key(clear, 2), be_const_closure(class_Leds_segment_clear_closure) },
    })),
    (bstring*) &be_const_str_Leds_segment
);
// compact class 'Leds_matrix' ktab size: 24, total: 62 (saved 304 bytes)
static const bvalue be_ktab_class_Leds_matrix[24] = {
  /* K0   */  be_nested_str(strip),
  /* K1   */  be_nested_str(bri),
  /* K2   */  be_nested_str(call_native),
  /* K3   */  be_nested_str(to_gamma),
  /* K4   */  be_nested_str(offset),
  /* K5   */  be_nested_str(w),
  /* K6   */  be_nested_str(h),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str(leds),
  /* K9   */  be_nested_str(show),
  /* K10  */  be_nested_str(pix_buffer),
  /* K11  */  be_nested_str(pixels_buffer),
  /* K12  */  be_nested_str(pix_size),
  /* K13  */  be_nested_str(alternate),
  /* K14  */  be_nested_str(pixel_size),
  /* K15  */  be_nested_str(set_pixel_color),
  /* K16  */  be_nested_str(dirty),
  /* K17  */  be_const_int(1),
  /* K18  */  be_nested_str(is_dirty),
  /* K19  */  be_nested_str(can_show),
  /* K20  */  be_nested_str(setbytes),
  /* K21  */  be_nested_str(get_pixel_color),
  /* K22  */  be_nested_str(offseta),
  /* K23  */  be_nested_str(clear_to),
};


extern const bclass be_class_Leds_matrix;

/********************************************************************
** Solidified function: clear_to
********************************************************************/
be_local_closure(class_Leds_matrix_clear_to,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_clear_to,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0001,  //  0002  JMPF	R3	#0005
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x88080701,  //  0004  GETMBR	R2	R3	K1
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x8C0C0702,  //  0006  GETMET	R3	R3	K2
      0x54160008,  //  0007  LDINT	R5	9
      0x88180100,  //  0008  GETMBR	R6	R0	K0
      0x8C180D03,  //  0009  GETMET	R6	R6	K3
      0x5C200200,  //  000A  MOVE	R8	R1
      0x5C240400,  //  000B  MOVE	R9	R2
      0x7C180600,  //  000C  CALL	R6	3
      0x881C0104,  //  000D  GETMBR	R7	R0	K4
      0x88200105,  //  000E  GETMBR	R8	R0	K5
      0x88240106,  //  000F  GETMBR	R9	R0	K6
      0x08201009,  //  0010  MUL	R8	R8	R9
      0x7C0C0A00,  //  0011  CALL	R3	5
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(class_Leds_matrix_show,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_show,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x740A0009,  //  0003  JMPT	R2	#000E
      0x88080104,  //  0004  GETMBR	R2	R0	K4
      0x1C080507,  //  0005  EQ	R2	R2	K7
      0x780A000E,  //  0006  JMPF	R2	#0016
      0x88080105,  //  0007  GETMBR	R2	R0	K5
      0x880C0106,  //  0008  GETMBR	R3	R0	K6
      0x08080403,  //  0009  MUL	R2	R2	R3
      0x880C0100,  //  000A  GETMBR	R3	R0	K0
      0x880C0708,  //  000B  GETMBR	R3	R3	K8
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0007,  //  000D  JMPF	R2	#0016
      0x88080100,  //  000E  GETMBR	R2	R0	K0
      0x8C080509,  //  000F  GETMET	R2	R2	K9
      0x7C080200,  //  0010  CALL	R2	1
      0x88080100,  //  0011  GETMBR	R2	R0	K0
      0x8C08050B,  //  0012  GETMET	R2	R2	K11
      0x8810010A,  //  0013  GETMBR	R4	R0	K10
      0x7C080400,  //  0014  CALL	R2	2
      0x90021402,  //  0015  SETMBR	R0	K10	R2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_size
********************************************************************/
be_local_closure(class_Leds_matrix_pixel_size,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_pixel_size,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Leds_matrix_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020804,  //  0001  SETMBR	R0	K4	R4
      0x90020C03,  //  0002  SETMBR	R0	K6	R3
      0x90020A02,  //  0003  SETMBR	R0	K5	R2
      0x50140000,  //  0004  LDBOOL	R5	0	0
      0x90021A05,  //  0005  SETMBR	R0	K13	R5
      0x88140100,  //  0006  GETMBR	R5	R0	K0
      0x8C140B0B,  //  0007  GETMET	R5	R5	K11
      0x7C140200,  //  0008  CALL	R5	1
      0x90021405,  //  0009  SETMBR	R0	K10	R5
      0x88140100,  //  000A  GETMBR	R5	R0	K0
      0x8C140B0E,  //  000B  GETMET	R5	R5	K14
      0x7C140200,  //  000C  CALL	R5	1
      0x90021805,  //  000D  SETMBR	R0	K12	R5
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(class_Leds_matrix_set_pixel_color,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_set_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120001,  //  0002  JMPF	R4	#0005
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x880C0901,  //  0004  GETMBR	R3	R4	K1
      0x88100100,  //  0005  GETMBR	R4	R0	K0
      0x8C10090F,  //  0006  GETMET	R4	R4	K15
      0x88180104,  //  0007  GETMBR	R6	R0	K4
      0x00180206,  //  0008  ADD	R6	R1	R6
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x5C200600,  //  000A  MOVE	R8	R3
      0x7C100800,  //  000B  CALL	R4	4
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(class_Leds_matrix_pixels_buffer,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_pixels_buffer,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C04030B,  //  0001  GETMET	R1	R1	K11
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dirty
********************************************************************/
be_local_closure(class_Leds_matrix_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040310,  //  0001  GETMET	R1	R1	K16
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_matrix_pixel_color
********************************************************************/
be_local_closure(class_Leds_matrix_set_matrix_pixel_color,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_set_matrix_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160001,  //  0002  JMPF	R5	#0005
      0x88140100,  //  0003  GETMBR	R5	R0	K0
      0x88100B01,  //  0004  GETMBR	R4	R5	K1
      0x8814010D,  //  0005  GETMBR	R5	R0	K13
      0x7816000F,  //  0006  JMPF	R5	#0017
      0x2C140511,  //  0007  AND	R5	R2	K17
      0x7816000D,  //  0008  JMPF	R5	#0017
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0x8C140B0F,  //  000A  GETMET	R5	R5	K15
      0x881C0105,  //  000B  GETMBR	R7	R0	K5
      0x081C0207,  //  000C  MUL	R7	R1	R7
      0x88200106,  //  000D  GETMBR	R8	R0	K6
      0x001C0E08,  //  000E  ADD	R7	R7	R8
      0x041C0E02,  //  000F  SUB	R7	R7	R2
      0x041C0F11,  //  0010  SUB	R7	R7	K17
      0x88200104,  //  0011  GETMBR	R8	R0	K4
      0x001C0E08,  //  0012  ADD	R7	R7	R8
      0x5C200600,  //  0013  MOVE	R8	R3
      0x5C240800,  //  0014  MOVE	R9	R4
      0x7C140800,  //  0015  CALL	R5	4
      0x70020009,  //  0016  JMP		#0021
      0x88140100,  //  0017  GETMBR	R5	R0	K0
      0x8C140B0F,  //  0018  GETMET	R5	R5	K15
      0x881C0105,  //  0019  GETMBR	R7	R0	K5
      0x081C0207,  //  001A  MUL	R7	R1	R7
      0x001C0E02,  //  001B  ADD	R7	R7	R2
      0x88200104,  //  001C  GETMBR	R8	R0	K4
      0x001C0E08,  //  001D  ADD	R7	R7	R8
      0x5C200600,  //  001E  MOVE	R8	R3
      0x5C240800,  //  001F  MOVE	R9	R4
      0x7C140800,  //  0020  CALL	R5	4
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_dirty
********************************************************************/
be_local_closure(class_Leds_matrix_is_dirty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_is_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040312,  //  0001  GETMET	R1	R1	K18
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_alternate
********************************************************************/
be_local_closure(class_Leds_matrix_set_alternate,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_set_alternate,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90021A01,  //  0000  SETMBR	R0	K13	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(class_Leds_matrix_begin,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
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
be_local_closure(class_Leds_matrix_pixel_count,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_pixel_count,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x88080106,  //  0001  GETMBR	R2	R0	K6
      0x08040202,  //  0002  MUL	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_alternate
********************************************************************/
be_local_closure(class_Leds_matrix_get_alternate,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_get_alternate,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_offset
********************************************************************/
be_local_closure(class_Leds_matrix_pixel_offset,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_pixel_offset,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_show
********************************************************************/
be_local_closure(class_Leds_matrix_can_show,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_can_show,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040313,  //  0001  GETMET	R1	R1	K19
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bytes
********************************************************************/
be_local_closure(class_Leds_matrix_set_bytes,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_set_bytes,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88140106,  //  0000  GETMBR	R5	R0	K6
      0x8818010C,  //  0001  GETMBR	R6	R0	K12
      0x08140A06,  //  0002  MUL	R5	R5	R6
      0x24180805,  //  0003  GT	R6	R4	R5
      0x781A0000,  //  0004  JMPF	R6	#0006
      0x5C100A00,  //  0005  MOVE	R4	R5
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x00180C01,  //  0007  ADD	R6	R6	R1
      0x08180C05,  //  0008  MUL	R6	R6	R5
      0x881C010A,  //  0009  GETMBR	R7	R0	K10
      0x8C1C0F14,  //  000A  GETMET	R7	R7	K20
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
be_local_closure(class_Leds_matrix_get_pixel_color,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_get_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080515,  //  0001  GETMET	R2	R2	K21
      0x88100116,  //  0002  GETMBR	R4	R0	K22
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
be_local_closure(class_Leds_matrix_clear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_clear,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040117,  //  0000  GETMET	R1	R0	K23
      0x580C0007,  //  0001  LDCONST	R3	K7
      0x7C040400,  //  0002  CALL	R1	2
      0x8C040109,  //  0003  GETMET	R1	R0	K9
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
        { be_const_key(init, -1), be_const_closure(class_Leds_matrix_init_closure) },
        { be_const_key(show, -1), be_const_closure(class_Leds_matrix_show_closure) },
        { be_const_key(alternate, -1), be_const_var(4) },
        { be_const_key(get_pixel_color, 14), be_const_closure(class_Leds_matrix_get_pixel_color_closure) },
        { be_const_key(is_dirty, -1), be_const_closure(class_Leds_matrix_is_dirty_closure) },
        { be_const_key(clear_to, 0), be_const_closure(class_Leds_matrix_clear_to_closure) },
        { be_const_key(set_pixel_color, -1), be_const_closure(class_Leds_matrix_set_pixel_color_closure) },
        { be_const_key(pixels_buffer, -1), be_const_closure(class_Leds_matrix_pixels_buffer_closure) },
        { be_const_key(dirty, -1), be_const_closure(class_Leds_matrix_dirty_closure) },
        { be_const_key(can_show, 21), be_const_closure(class_Leds_matrix_can_show_closure) },
        { be_const_key(pixel_size, 4), be_const_closure(class_Leds_matrix_pixel_size_closure) },
        { be_const_key(w, -1), be_const_var(3) },
        { be_const_key(set_matrix_pixel_color, 9), be_const_closure(class_Leds_matrix_set_matrix_pixel_color_closure) },
        { be_const_key(offset, -1), be_const_var(1) },
        { be_const_key(strip, -1), be_const_var(0) },
        { be_const_key(begin, -1), be_const_closure(class_Leds_matrix_begin_closure) },
        { be_const_key(h, -1), be_const_var(2) },
        { be_const_key(pix_size, -1), be_const_var(6) },
        { be_const_key(pixel_count, -1), be_const_closure(class_Leds_matrix_pixel_count_closure) },
        { be_const_key(get_alternate, -1), be_const_closure(class_Leds_matrix_get_alternate_closure) },
        { be_const_key(pixel_offset, -1), be_const_closure(class_Leds_matrix_pixel_offset_closure) },
        { be_const_key(set_alternate, -1), be_const_closure(class_Leds_matrix_set_alternate_closure) },
        { be_const_key(set_bytes, -1), be_const_closure(class_Leds_matrix_set_bytes_closure) },
        { be_const_key(pix_buffer, 3), be_const_var(5) },
        { be_const_key(clear, -1), be_const_closure(class_Leds_matrix_clear_closure) },
    })),
    (bstring*) &be_const_str_Leds_matrix
);
// compact class 'Leds' ktab size: 39, total: 74 (saved 280 bytes)
static const bvalue be_ktab_class_Leds[39] = {
  /* K0   */  be_nested_str(leds),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str(value_error),
  /* K3   */  be_nested_str(out_X20of_X20range),
  /* K4   */  be_const_class(be_class_Leds_segment),
  /* K5   */  be_nested_str(gamma),
  /* K6   */  be_const_class(be_class_Leds),
  /* K7   */  be_nested_str(Leds),
  /* K8   */  be_nested_str(create_matrix),
  /* K9   */  be_nested_str(call_native),
  /* K10  */  be_nested_str(pixel_size),
  /* K11  */  be_nested_str(pixel_count),
  /* K12  */  be_nested_str(_change_buffer),
  /* K13  */  be_nested_str(animate),
  /* K14  */  be_const_int(2),
  /* K15  */  be_nested_str(bri),
  /* K16  */  be_nested_str(to_gamma),
  /* K17  */  be_nested_str(clear_to),
  /* K18  */  be_nested_str(show),
  /* K19  */  be_nested_str(apply_bri_gamma),
  /* K20  */  be_const_int(1),
  /* K21  */  be_const_class(be_class_Leds_matrix),
  /* K22  */  be_const_int(3),
  /* K23  */  be_nested_str(gpio),
  /* K24  */  be_nested_str(pin),
  /* K25  */  be_nested_str(WS2812),
  /* K26  */  be_nested_str(ctor),
  /* K27  */  be_nested_str(light),
  /* K28  */  be_nested_str(get),
  /* K29  */  be_nested_str(global),
  /* K30  */  be_nested_str(contains),
  /* K31  */  be_nested_str(_lhw),
  /* K32  */  be_nested_str(find),
  /* K33  */  be_nested_str(number_X20of_X20leds_X20do_X20not_X20match_X20with_X20previous_X20instanciation_X20_X25s_X20vs_X20_X25s),
  /* K34  */  be_nested_str(_p),
  /* K35  */  be_nested_str(begin),
  /* K36  */  be_nested_str(internal_error),
  /* K37  */  be_nested_str(couldn_X27t_X20not_X20initialize_X20noepixelbus),
  /* K38  */  be_nested_str(WS2812_GRB),
};


extern const bclass be_class_Leds;

/********************************************************************
** Solidified function: create_segment
********************************************************************/
be_local_closure(class_Leds_create_segment,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
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
** Solidified function: get_gamma
********************************************************************/
be_local_closure(class_Leds_get_gamma,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_get_gamma,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: matrix
********************************************************************/
be_local_closure(class_Leds_matrix,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_matrix,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x58100006,  //  0000  LDCONST	R4	K6
      0xB8160E00,  //  0001  GETNGBL	R5	K7
      0x08180001,  //  0002  MUL	R6	R0	R1
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x5C200600,  //  0004  MOVE	R8	R3
      0x7C140600,  //  0005  CALL	R5	3
      0x8C180B08,  //  0006  GETMET	R6	R5	K8
      0x5C200000,  //  0007  MOVE	R8	R0
      0x5C240200,  //  0008  MOVE	R9	R1
      0x58280001,  //  0009  LDCONST	R10	K1
      0x7C180800,  //  000A  CALL	R6	4
      0x80040C00,  //  000B  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixels_buffer
********************************************************************/
be_local_closure(class_Leds_pixels_buffer,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_pixels_buffer,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C080109,  //  0000  GETMET	R2	R0	K9
      0x54120005,  //  0001  LDINT	R4	6
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0203,  //  0004  EQ	R3	R1	R3
      0x780E0009,  //  0005  JMPF	R3	#0010
      0x600C0015,  //  0006  GETGBL	R3	G21
      0x5C100400,  //  0007  MOVE	R4	R2
      0x8C14010A,  //  0008  GETMET	R5	R0	K10
      0x7C140200,  //  0009  CALL	R5	1
      0x8C18010B,  //  000A  GETMET	R6	R0	K11
      0x7C180200,  //  000B  CALL	R6	1
      0x08140A06,  //  000C  MUL	R5	R5	R6
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80040600,  //  000E  RET	1	R3
      0x70020003,  //  000F  JMP		#0014
      0x8C0C030C,  //  0010  GETMET	R3	R1	K12
      0x5C140400,  //  0011  MOVE	R5	R2
      0x7C0C0400,  //  0012  CALL	R3	2
      0x80040200,  //  0013  RET	1	R1
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_animate
********************************************************************/
be_local_closure(class_Leds_get_animate,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_get_animate,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: show
********************************************************************/
be_local_closure(class_Leds_show,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_show,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040109,  //  0000  GETMET	R1	R0	K9
      0x580C000E,  //  0001  LDCONST	R3	K14
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel_color
********************************************************************/
be_local_closure(class_Leds_set_pixel_color,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_set_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120000,  //  0002  JMPF	R4	#0004
      0x880C010F,  //  0003  GETMBR	R3	R0	K15
      0x8C100109,  //  0004  GETMET	R4	R0	K9
      0x541A0009,  //  0005  LDINT	R6	10
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x8C200110,  //  0007  GETMET	R8	R0	K16
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
** Solidified function: is_dirty
********************************************************************/
be_local_closure(class_Leds_is_dirty,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_is_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040109,  //  0000  GETMET	R1	R0	K9
      0x540E0003,  //  0001  LDINT	R3	4
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear
********************************************************************/
be_local_closure(class_Leds_clear,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_clear,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040111,  //  0000  GETMET	R1	R0	K17
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x8C040112,  //  0003  GETMET	R1	R0	K18
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_size
********************************************************************/
be_local_closure(class_Leds_pixel_size,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_pixel_size,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040109,  //  0000  GETMET	R1	R0	K9
      0x540E0006,  //  0001  LDINT	R3	7
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_count
********************************************************************/
be_local_closure(class_Leds_pixel_count,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_pixel_count,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040109,  //  0000  GETMET	R1	R0	K9
      0x540E0007,  //  0001  LDINT	R3	8
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_gamma
********************************************************************/
be_local_closure(class_Leds_to_gamma,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_to_gamma,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x8808010F,  //  0003  GETMBR	R2	R0	K15
      0x8C0C0113,  //  0004  GETMET	R3	R0	K19
      0x5C140200,  //  0005  MOVE	R5	R1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x881C0105,  //  0007  GETMBR	R7	R0	K5
      0x7C0C0800,  //  0008  CALL	R3	4
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(class_Leds_begin,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_begin,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040109,  //  0000  GETMET	R1	R0	K9
      0x580C0014,  //  0001  LDCONST	R3	K20
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_matrix
********************************************************************/
be_local_closure(class_Leds_create_matrix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
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
      0x580C0001,  //  000F  LDCONST	R3	K1
      0x08100202,  //  0010  MUL	R4	R1	R2
      0x00100803,  //  0011  ADD	R4	R4	R3
      0x88140100,  //  0012  GETMBR	R5	R0	K0
      0x24100805,  //  0013  GT	R4	R4	R5
      0x74120005,  //  0014  JMPT	R4	#001B
      0x14100501,  //  0015  LT	R4	R2	K1
      0x74120003,  //  0016  JMPT	R4	#001B
      0x14100301,  //  0017  LT	R4	R1	K1
      0x74120001,  //  0018  JMPT	R4	#001B
      0x14100701,  //  0019  LT	R4	R3	K1
      0x78120000,  //  001A  JMPF	R4	#001C
      0xB0060503,  //  001B  RAISE	1	K2	K3
      0x58100015,  //  001C  LDCONST	R4	K21
      0xB4000015,  //  001D  CLASS	K21
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
** Solidified function: dirty
********************************************************************/
be_local_closure(class_Leds_dirty,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_dirty,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040109,  //  0000  GETMET	R1	R0	K9
      0x540E0004,  //  0001  LDINT	R3	5
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bri
********************************************************************/
be_local_closure(class_Leds_set_bri,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_set_bri,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x14080301,  //  0000  LT	R2	R1	K1
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040001,  //  0002  LDCONST	R1	K1
      0x540A00FE,  //  0003  LDINT	R2	255
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540600FE,  //  0006  LDINT	R1	255
      0x90021E01,  //  0007  SETMBR	R0	K15	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_show
********************************************************************/
be_local_closure(class_Leds_can_show,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_can_show,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040109,  //  0000  GETMET	R1	R0	K9
      0x580C0016,  //  0001  LDCONST	R3	K22
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_animate
********************************************************************/
be_local_closure(class_Leds_set_animate,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_set_animate,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90021A01,  //  0000  SETMBR	R0	K13	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bri
********************************************************************/
be_local_closure(class_Leds_get_bri,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_get_bri,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010F,  //  0000  GETMBR	R1	R0	K15
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clear_to
********************************************************************/
be_local_closure(class_Leds_clear_to,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_clear_to,
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140405,  //  0001  EQ	R5	R2	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x8808010F,  //  0003  GETMBR	R2	R0	K15
      0x4C140000,  //  0004  LDNIL	R5
      0x20140605,  //  0005  NE	R5	R3	R5
      0x7816000C,  //  0006  JMPF	R5	#0014
      0x4C140000,  //  0007  LDNIL	R5
      0x20140805,  //  0008  NE	R5	R4	R5
      0x78160009,  //  0009  JMPF	R5	#0014
      0x8C140109,  //  000A  GETMET	R5	R0	K9
      0x541E0008,  //  000B  LDINT	R7	9
      0x8C200110,  //  000C  GETMET	R8	R0	K16
      0x5C280200,  //  000D  MOVE	R10	R1
      0x5C2C0400,  //  000E  MOVE	R11	R2
      0x7C200600,  //  000F  CALL	R8	3
      0x5C240600,  //  0010  MOVE	R9	R3
      0x5C280800,  //  0011  MOVE	R10	R4
      0x7C140A00,  //  0012  CALL	R5	5
      0x70020006,  //  0013  JMP		#001B
      0x8C140109,  //  0014  GETMET	R5	R0	K9
      0x541E0008,  //  0015  LDINT	R7	9
      0x8C200110,  //  0016  GETMET	R8	R0	K16
      0x5C280200,  //  0017  MOVE	R10	R1
      0x5C2C0400,  //  0018  MOVE	R11	R2
      0x7C200600,  //  0019  CALL	R8	3
      0x7C140600,  //  001A  CALL	R5	3
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pixel_offset
********************************************************************/
be_local_closure(class_Leds_pixel_offset,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_pixel_offset,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80060200,  //  0000  RET	1	K1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pixel_color
********************************************************************/
be_local_closure(class_Leds_get_pixel_color,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_get_pixel_color,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C080109,  //  0000  GETMET	R2	R0	K9
      0x5412000A,  //  0001  LDINT	R4	11
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C080600,  //  0003  CALL	R2	3
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Leds_init,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[90]) {  /* code */
      0xA4162E00,  //  0000  IMPORT	R5	K23
      0x50180200,  //  0001  LDBOOL	R6	1	0
      0x90020A06,  //  0002  SETMBR	R0	K5	R6
      0x4C180000,  //  0003  LDNIL	R6
      0x1C180206,  //  0004  EQ	R6	R1	R6
      0x741A0008,  //  0005  JMPT	R6	#000F
      0x4C180000,  //  0006  LDNIL	R6
      0x1C180406,  //  0007  EQ	R6	R2	R6
      0x741A0005,  //  0008  JMPT	R6	#000F
      0x8C180B18,  //  0009  GETMET	R6	R5	K24
      0x88200B19,  //  000A  GETMBR	R8	R5	K25
      0x58240001,  //  000B  LDCONST	R9	K1
      0x7C180600,  //  000C  CALL	R6	3
      0x1C180406,  //  000D  EQ	R6	R2	R6
      0x781A000A,  //  000E  JMPF	R6	#001A
      0x8C18011A,  //  000F  GETMET	R6	R0	K26
      0x7C180200,  //  0010  CALL	R6	1
      0x8C18010B,  //  0011  GETMET	R6	R0	K11
      0x7C180200,  //  0012  CALL	R6	1
      0x90020006,  //  0013  SETMBR	R0	K0	R6
      0xA41A3600,  //  0014  IMPORT	R6	K27
      0x8C1C0D1C,  //  0015  GETMET	R7	R6	K28
      0x7C1C0200,  //  0016  CALL	R7	1
      0x941C0F0F,  //  0017  GETIDX	R7	R7	K15
      0x90021E07,  //  0018  SETMBR	R0	K15	R7
      0x70020039,  //  0019  JMP		#0054
      0x60180009,  //  001A  GETGBL	R6	G9
      0x5C1C0200,  //  001B  MOVE	R7	R1
      0x7C180200,  //  001C  CALL	R6	1
      0x5C040C00,  //  001D  MOVE	R1	R6
      0x90020001,  //  001E  SETMBR	R0	K0	R1
      0x541A007E,  //  001F  LDINT	R6	127
      0x90021E06,  //  0020  SETMBR	R0	K15	R6
      0xB81A3A00,  //  0021  GETNGBL	R6	K29
      0x8C180D1E,  //  0022  GETMET	R6	R6	K30
      0x5820001F,  //  0023  LDCONST	R8	K31
      0x7C180400,  //  0024  CALL	R6	2
      0x741A0003,  //  0025  JMPT	R6	#002A
      0xB81A3A00,  //  0026  GETNGBL	R6	K29
      0x601C0013,  //  0027  GETGBL	R7	G19
      0x7C1C0000,  //  0028  CALL	R7	0
      0x901A3E07,  //  0029  SETMBR	R6	K31	R7
      0xB81A3A00,  //  002A  GETNGBL	R6	K29
      0x88180D1F,  //  002B  GETMBR	R6	R6	K31
      0x8C180D20,  //  002C  GETMET	R6	R6	K32
      0x5C200200,  //  002D  MOVE	R8	R1
      0x7C180400,  //  002E  CALL	R6	2
      0x4C1C0000,  //  002F  LDNIL	R7
      0x20180C07,  //  0030  NE	R6	R6	R7
      0x781A0016,  //  0031  JMPF	R6	#0049
      0xB81A3A00,  //  0032  GETNGBL	R6	K29
      0x88180D1F,  //  0033  GETMBR	R6	R6	K31
      0x8C180D20,  //  0034  GETMET	R6	R6	K32
      0x5C200200,  //  0035  MOVE	R8	R1
      0x7C180400,  //  0036  CALL	R6	2
      0x881C0100,  //  0037  GETMBR	R7	R0	K0
      0x88200D00,  //  0038  GETMBR	R8	R6	K0
      0x201C0E08,  //  0039  NE	R7	R7	R8
      0x781E0005,  //  003A  JMPF	R7	#0041
      0x601C0018,  //  003B  GETGBL	R7	G24
      0x58200021,  //  003C  LDCONST	R8	K33
      0x88240100,  //  003D  GETMBR	R9	R0	K0
      0x88280D00,  //  003E  GETMBR	R10	R6	K0
      0x7C1C0600,  //  003F  CALL	R7	3
      0xB0060407,  //  0040  RAISE	1	K2	R7
      0x881C0D22,  //  0041  GETMBR	R7	R6	K34
      0x90024407,  //  0042  SETMBR	R0	K34	R7
      0x881C0D0D,  //  0043  GETMBR	R7	R6	K13
      0x90021A07,  //  0044  SETMBR	R0	K13	R7
      0xB81E3A00,  //  0045  GETNGBL	R7	K29
      0x881C0F1F,  //  0046  GETMBR	R7	R7	K31
      0x981C0200,  //  0047  SETIDX	R7	R1	R0
      0x7002000A,  //  0048  JMP		#0054
      0x8C18011A,  //  0049  GETMET	R6	R0	K26
      0x5C200200,  //  004A  MOVE	R8	R1
      0x5C240400,  //  004B  MOVE	R9	R2
      0x5C280600,  //  004C  MOVE	R10	R3
      0x5C2C0800,  //  004D  MOVE	R11	R4
      0x7C180A00,  //  004E  CALL	R6	5
      0xB81A3A00,  //  004F  GETNGBL	R6	K29
      0x88180D1F,  //  0050  GETMBR	R6	R6	K31
      0x98180200,  //  0051  SETIDX	R6	R1	R0
      0x8C180123,  //  0052  GETMET	R6	R0	K35
      0x7C180200,  //  0053  CALL	R6	1
      0x88180122,  //  0054  GETMBR	R6	R0	K34
      0x4C1C0000,  //  0055  LDNIL	R7
      0x1C180C07,  //  0056  EQ	R6	R6	R7
      0x781A0000,  //  0057  JMPF	R6	#0059
      0xB0064925,  //  0058  RAISE	1	K36	K37
      0x80000000,  //  0059  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ctor
********************************************************************/
be_local_closure(class_Leds_ctor,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_ctor,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140405,  //  0001  EQ	R5	R2	R5
      0x78160003,  //  0002  JMPF	R5	#0007
      0x8C140109,  //  0003  GETMET	R5	R0	K9
      0x581C0001,  //  0004  LDCONST	R7	K1
      0x7C140400,  //  0005  CALL	R5	2
      0x7002000A,  //  0006  JMP		#0012
      0x4C140000,  //  0007  LDNIL	R5
      0x1C140605,  //  0008  EQ	R5	R3	R5
      0x78160000,  //  0009  JMPF	R5	#000B
      0x880C0126,  //  000A  GETMBR	R3	R0	K38
      0x8C140109,  //  000B  GETMET	R5	R0	K9
      0x581C0001,  //  000C  LDCONST	R7	K1
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
** Solidified function: set_gamma
********************************************************************/
be_local_closure(class_Leds_set_gamma,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_set_gamma,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020A02,  //  0003  SETMBR	R0	K5	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds
********************************************************************/
extern const bclass be_class_Leds_ntv;
be_local_class(Leds,
    4,
    &be_class_Leds_ntv,
    be_nested_map(29,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(pixels_buffer, -1), be_const_closure(class_Leds_pixels_buffer_closure) },
        { be_const_key(get_gamma, 26), be_const_closure(class_Leds_get_gamma_closure) },
        { be_const_key(clear, -1), be_const_closure(class_Leds_clear_closure) },
        { be_const_key(matrix, 13), be_const_static_closure(class_Leds_matrix_closure) },
        { be_const_key(init, 23), be_const_closure(class_Leds_init_closure) },
        { be_const_key(get_animate, -1), be_const_closure(class_Leds_get_animate_closure) },
        { be_const_key(get_pixel_color, 12), be_const_closure(class_Leds_get_pixel_color_closure) },
        { be_const_key(set_pixel_color, -1), be_const_closure(class_Leds_set_pixel_color_closure) },
        { be_const_key(animate, -1), be_const_var(3) },
        { be_const_key(is_dirty, -1), be_const_closure(class_Leds_is_dirty_closure) },
        { be_const_key(create_segment, 4), be_const_closure(class_Leds_create_segment_closure) },
        { be_const_key(pixel_offset, -1), be_const_closure(class_Leds_pixel_offset_closure) },
        { be_const_key(clear_to, -1), be_const_closure(class_Leds_clear_to_closure) },
        { be_const_key(begin, 0), be_const_closure(class_Leds_begin_closure) },
        { be_const_key(set_animate, -1), be_const_closure(class_Leds_set_animate_closure) },
        { be_const_key(can_show, 24), be_const_closure(class_Leds_can_show_closure) },
        { be_const_key(create_matrix, -1), be_const_closure(class_Leds_create_matrix_closure) },
        { be_const_key(dirty, -1), be_const_closure(class_Leds_dirty_closure) },
        { be_const_key(set_bri, -1), be_const_closure(class_Leds_set_bri_closure) },
        { be_const_key(leds, 15), be_const_var(1) },
        { be_const_key(gamma, -1), be_const_var(0) },
        { be_const_key(bri, 14), be_const_var(2) },
        { be_const_key(get_bri, 11), be_const_closure(class_Leds_get_bri_closure) },
        { be_const_key(to_gamma, 2), be_const_closure(class_Leds_to_gamma_closure) },
        { be_const_key(pixel_count, -1), be_const_closure(class_Leds_pixel_count_closure) },
        { be_const_key(show, 6), be_const_closure(class_Leds_show_closure) },
        { be_const_key(pixel_size, -1), be_const_closure(class_Leds_pixel_size_closure) },
        { be_const_key(ctor, -1), be_const_closure(class_Leds_ctor_closure) },
        { be_const_key(set_gamma, -1), be_const_closure(class_Leds_set_gamma_closure) },
    })),
    (bstring*) &be_const_str_Leds
);
/********************************************************************/
/* End of solidification */
