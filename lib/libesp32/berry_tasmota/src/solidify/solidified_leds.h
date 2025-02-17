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
// compact class 'Leds_matrix' ktab size: 28, total: 72 (saved 352 bytes)
static const bvalue be_ktab_class_Leds_matrix[28] = {
  /* K0   */  be_nested_str(strip),
  /* K1   */  be_nested_str(is_dirty),
  /* K2   */  be_nested_str(dirty),
  /* K3   */  be_nested_str(bri),
  /* K4   */  be_nested_str(alternate),
  /* K5   */  be_const_int(1),
  /* K6   */  be_nested_str(set_pixel_color),
  /* K7   */  be_nested_str(w),
  /* K8   */  be_nested_str(h),
  /* K9   */  be_nested_str(offset),
  /* K10  */  be_nested_str(can_show),
  /* K11  */  be_nested_str(pix_size),
  /* K12  */  be_nested_str(pix_buffer),
  /* K13  */  be_nested_str(setbytes),
  /* K14  */  be_const_int(0),
  /* K15  */  be_nested_str(leds),
  /* K16  */  be_nested_str(show),
  /* K17  */  be_nested_str(pixels_buffer),
  /* K18  */  be_nested_str(get_pixel_color),
  /* K19  */  be_nested_str(offseta),
  /* K20  */  be_nested_str(call_native),
  /* K21  */  be_nested_str(to_gamma),
  /* K22  */  be_nested_str(pixel_size),
  /* K23  */  be_nested_str(clear_to),
  /* K24  */  be_const_int(3),
  /* K25  */  be_const_int(2),
  /* K26  */  be_nested_str(reverse),
  /* K27  */  be_const_int(2147483647),
};


extern const bclass be_class_Leds_matrix;

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
      0x8C040302,  //  0001  GETMET	R1	R1	K2
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
      0x88100B03,  //  0004  GETMBR	R4	R5	K3
      0x88140104,  //  0005  GETMBR	R5	R0	K4
      0x7816000F,  //  0006  JMPF	R5	#0017
      0x2C140505,  //  0007  AND	R5	R2	K5
      0x7816000D,  //  0008  JMPF	R5	#0017
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0x8C140B06,  //  000A  GETMET	R5	R5	K6
      0x881C0107,  //  000B  GETMBR	R7	R0	K7
      0x081C0207,  //  000C  MUL	R7	R1	R7
      0x88200108,  //  000D  GETMBR	R8	R0	K8
      0x001C0E08,  //  000E  ADD	R7	R7	R8
      0x041C0E02,  //  000F  SUB	R7	R7	R2
      0x041C0F05,  //  0010  SUB	R7	R7	K5
      0x88200109,  //  0011  GETMBR	R8	R0	K9
      0x001C0E08,  //  0012  ADD	R7	R7	R8
      0x5C200600,  //  0013  MOVE	R8	R3
      0x5C240800,  //  0014  MOVE	R9	R4
      0x7C140800,  //  0015  CALL	R5	4
      0x70020009,  //  0016  JMP		#0021
      0x88140100,  //  0017  GETMBR	R5	R0	K0
      0x8C140B06,  //  0018  GETMET	R5	R5	K6
      0x881C0107,  //  0019  GETMBR	R7	R0	K7
      0x081C0207,  //  001A  MUL	R7	R1	R7
      0x001C0E02,  //  001B  ADD	R7	R7	R2
      0x88200109,  //  001C  GETMBR	R8	R0	K9
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
      0x8C04030A,  //  0001  GETMET	R1	R1	K10
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
      0x88140108,  //  0000  GETMBR	R5	R0	K8
      0x8818010B,  //  0001  GETMBR	R6	R0	K11
      0x08140A06,  //  0002  MUL	R5	R5	R6
      0x24180805,  //  0003  GT	R6	R4	R5
      0x781A0000,  //  0004  JMPF	R6	#0006
      0x5C100A00,  //  0005  MOVE	R4	R5
      0x88180109,  //  0006  GETMBR	R6	R0	K9
      0x00180C01,  //  0007  ADD	R6	R6	R1
      0x08180C05,  //  0008  MUL	R6	R6	R5
      0x881C010C,  //  0009  GETMBR	R7	R0	K12
      0x8C1C0F0D,  //  000A  GETMET	R7	R7	K13
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
      0x88080109,  //  0004  GETMBR	R2	R0	K9
      0x1C08050E,  //  0005  EQ	R2	R2	K14
      0x780A000E,  //  0006  JMPF	R2	#0016
      0x88080107,  //  0007  GETMBR	R2	R0	K7
      0x880C0108,  //  0008  GETMBR	R3	R0	K8
      0x08080403,  //  0009  MUL	R2	R2	R3
      0x880C0100,  //  000A  GETMBR	R3	R0	K0
      0x880C070F,  //  000B  GETMBR	R3	R3	K15
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0007,  //  000D  JMPF	R2	#0016
      0x88080100,  //  000E  GETMBR	R2	R0	K0
      0x8C080510,  //  000F  GETMET	R2	R2	K16
      0x7C080200,  //  0010  CALL	R2	1
      0x88080100,  //  0011  GETMBR	R2	R0	K0
      0x8C080511,  //  0012  GETMET	R2	R2	K17
      0x8810010C,  //  0013  GETMBR	R4	R0	K12
      0x7C080400,  //  0014  CALL	R2	2
      0x90021802,  //  0015  SETMBR	R0	K12	R2
      0x80000000,  //  0016  RET	0
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
      0x88040104,  //  0000  GETMBR	R1	R0	K4
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
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x80040200,  //  0001  RET	1	R1
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
      0x8C080512,  //  0001  GETMET	R2	R2	K18
      0x88100113,  //  0002  GETMBR	R4	R0	K19
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
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


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
      0x88080703,  //  0004  GETMBR	R2	R3	K3
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x8C0C0714,  //  0006  GETMET	R3	R3	K20
      0x54160008,  //  0007  LDINT	R5	9
      0x88180100,  //  0008  GETMBR	R6	R0	K0
      0x8C180D15,  //  0009  GETMET	R6	R6	K21
      0x5C200200,  //  000A  MOVE	R8	R1
      0x5C240400,  //  000B  MOVE	R9	R2
      0x7C180600,  //  000C  CALL	R6	3
      0x881C0109,  //  000D  GETMBR	R7	R0	K9
      0x88200107,  //  000E  GETMBR	R8	R0	K7
      0x88240108,  //  000F  GETMBR	R9	R0	K8
      0x08201009,  //  0010  MUL	R8	R8	R9
      0x7C0C0A00,  //  0011  CALL	R3	5
      0x80000000,  //  0012  RET	0
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
      0x90020801,  //  0000  SETMBR	R0	K4	R1
      0x80000000,  //  0001  RET	0
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
      0x90021204,  //  0001  SETMBR	R0	K9	R4
      0x90021003,  //  0002  SETMBR	R0	K8	R3
      0x90020E02,  //  0003  SETMBR	R0	K7	R2
      0x50140000,  //  0004  LDBOOL	R5	0	0
      0x90020805,  //  0005  SETMBR	R0	K4	R5
      0x88140100,  //  0006  GETMBR	R5	R0	K0
      0x8C140B11,  //  0007  GETMET	R5	R5	K17
      0x7C140200,  //  0008  CALL	R5	1
      0x90021805,  //  0009  SETMBR	R0	K12	R5
      0x88140100,  //  000A  GETMBR	R5	R0	K0
      0x8C140B16,  //  000B  GETMET	R5	R5	K22
      0x7C140200,  //  000C  CALL	R5	1
      0x90021605,  //  000D  SETMBR	R0	K11	R5
      0x80000000,  //  000E  RET	0
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
      0x580C000E,  //  0001  LDCONST	R3	K14
      0x7C040400,  //  0002  CALL	R1	2
      0x8C040110,  //  0003  GETMET	R1	R0	K16
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scroll
********************************************************************/
be_local_closure(class_Leds_matrix_scroll,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_matrix,     /* shared constants */
    &be_const_str_scroll,
    &be_const_str_solidified,
    ( &(const binstruction[258]) {  /* code */
      0x8810010C,  //  0000  GETMBR	R4	R0	K12
      0x88140108,  //  0001  GETMBR	R5	R0	K8
      0x88180107,  //  0002  GETMBR	R6	R0	K7
      0x08180D18,  //  0003  MUL	R6	R6	K24
      0x4C1C0000,  //  0004  LDNIL	R7
      0x10200319,  //  0005  MOD	R8	R1	K25
      0x1C20110E,  //  0006  EQ	R8	R8	K14
      0x7822006B,  //  0007  JMPF	R8	#0074
      0x1C20030E,  //  0008  EQ	R8	R1	K14
      0x78220037,  //  0009  JMPF	R8	#0042
      0x8C20050D,  //  000A  GETMET	R8	R2	K13
      0x5828000E,  //  000B  LDCONST	R10	K14
      0x042C0D05,  //  000C  SUB	R11	R6	K5
      0x402E1C0B,  //  000D  CONNECT	R11	K14	R11
      0x942C080B,  //  000E  GETIDX	R11	R4	R11
      0x8C2C171A,  //  000F  GETMET	R11	R11	K26
      0x5834000E,  //  0010  LDCONST	R13	K14
      0x4C380000,  //  0011  LDNIL	R14
      0x583C0018,  //  0012  LDCONST	R15	K24
      0x7C2C0800,  //  0013  CALL	R11	4
      0x7C200600,  //  0014  CALL	R8	3
      0x5820000E,  //  0015  LDCONST	R8	K14
      0x04240B05,  //  0016  SUB	R9	R5	K5
      0x14241009,  //  0017  LT	R9	R8	R9
      0x78260016,  //  0018  JMPF	R9	#0030
      0x581C000E,  //  0019  LDCONST	R7	K14
      0x08241006,  //  001A  MUL	R9	R8	R6
      0x00281119,  //  001B  ADD	R10	R8	K25
      0x08281406,  //  001C  MUL	R10	R10	R6
      0x04281518,  //  001D  SUB	R10	R10	K24
      0x142C0E06,  //  001E  LT	R11	R7	R6
      0x782E000D,  //  001F  JMPF	R11	#002E
      0x002C0E09,  //  0020  ADD	R11	R7	R9
      0x04301407,  //  0021  SUB	R12	R10	R7
      0x9434080C,  //  0022  GETIDX	R13	R4	R12
      0x9810160D,  //  0023  SETIDX	R4	R11	R13
      0x00341705,  //  0024  ADD	R13	R11	K5
      0x00381905,  //  0025  ADD	R14	R12	K5
      0x9438080E,  //  0026  GETIDX	R14	R4	R14
      0x98101A0E,  //  0027  SETIDX	R4	R13	R14
      0x00341719,  //  0028  ADD	R13	R11	K25
      0x00381919,  //  0029  ADD	R14	R12	K25
      0x9438080E,  //  002A  GETIDX	R14	R4	R14
      0x98101A0E,  //  002B  SETIDX	R4	R13	R14
      0x001C0F18,  //  002C  ADD	R7	R7	K24
      0x7001FFEF,  //  002D  JMP		#001E
      0x00201105,  //  002E  ADD	R8	R8	K5
      0x7001FFE5,  //  002F  JMP		#0016
      0x780E0001,  //  0030  JMPF	R3	#0033
      0x5C240600,  //  0031  MOVE	R9	R3
      0x70020000,  //  0032  JMP		#0034
      0x5C240400,  //  0033  MOVE	R9	R2
      0x10280B19,  //  0034  MOD	R10	R5	K25
      0x1C281505,  //  0035  EQ	R10	R10	K5
      0x782A0004,  //  0036  JMPF	R10	#003C
      0x8C28131A,  //  0037  GETMET	R10	R9	K26
      0x5830000E,  //  0038  LDCONST	R12	K14
      0x4C340000,  //  0039  LDNIL	R13
      0x58380018,  //  003A  LDCONST	R14	K24
      0x7C280800,  //  003B  CALL	R10	4
      0x8C28090D,  //  003C  GETMET	R10	R4	K13
      0x04300B05,  //  003D  SUB	R12	R5	K5
      0x08301806,  //  003E  MUL	R12	R12	R6
      0x5C341200,  //  003F  MOVE	R13	R9
      0x7C280600,  //  0040  CALL	R10	3
      0x70020030,  //  0041  JMP		#0073
      0x8C20050D,  //  0042  GETMET	R8	R2	K13
      0x5828000E,  //  0043  LDCONST	R10	K14
      0x602C000C,  //  0044  GETGBL	R11	G12
      0x5C300800,  //  0045  MOVE	R12	R4
      0x7C2C0200,  //  0046  CALL	R11	1
      0x042C1606,  //  0047  SUB	R11	R11	R6
      0x402C171B,  //  0048  CONNECT	R11	R11	K27
      0x942C080B,  //  0049  GETIDX	R11	R4	R11
      0x8C2C171A,  //  004A  GETMET	R11	R11	K26
      0x5834000E,  //  004B  LDCONST	R13	K14
      0x4C380000,  //  004C  LDNIL	R14
      0x583C0018,  //  004D  LDCONST	R15	K24
      0x7C2C0800,  //  004E  CALL	R11	4
      0x7C200600,  //  004F  CALL	R8	3
      0x04200B05,  //  0050  SUB	R8	R5	K5
      0x2424110E,  //  0051  GT	R9	R8	K14
      0x7826000F,  //  0052  JMPF	R9	#0063
      0x8C24090D,  //  0053  GETMET	R9	R4	K13
      0x082C1006,  //  0054  MUL	R11	R8	R6
      0x04301105,  //  0055  SUB	R12	R8	K5
      0x08301806,  //  0056  MUL	R12	R12	R6
      0x08341006,  //  0057  MUL	R13	R8	R6
      0x04341B05,  //  0058  SUB	R13	R13	K5
      0x4030180D,  //  0059  CONNECT	R12	R12	R13
      0x9430080C,  //  005A  GETIDX	R12	R4	R12
      0x8C30191A,  //  005B  GETMET	R12	R12	K26
      0x5838000E,  //  005C  LDCONST	R14	K14
      0x4C3C0000,  //  005D  LDNIL	R15
      0x58400018,  //  005E  LDCONST	R16	K24
      0x7C300800,  //  005F  CALL	R12	4
      0x7C240600,  //  0060  CALL	R9	3
      0x04201105,  //  0061  SUB	R8	R8	K5
      0x7001FFED,  //  0062  JMP		#0051
      0x780E0001,  //  0063  JMPF	R3	#0066
      0x5C240600,  //  0064  MOVE	R9	R3
      0x70020000,  //  0065  JMP		#0067
      0x5C240400,  //  0066  MOVE	R9	R2
      0x10280B19,  //  0067  MOD	R10	R5	K25
      0x1C281505,  //  0068  EQ	R10	R10	K5
      0x782A0004,  //  0069  JMPF	R10	#006F
      0x8C28131A,  //  006A  GETMET	R10	R9	K26
      0x5830000E,  //  006B  LDCONST	R12	K14
      0x4C340000,  //  006C  LDNIL	R13
      0x58380018,  //  006D  LDCONST	R14	K24
      0x7C280800,  //  006E  CALL	R10	4
      0x8C28090D,  //  006F  GETMET	R10	R4	K13
      0x5830000E,  //  0070  LDCONST	R12	K14
      0x5C341200,  //  0071  MOVE	R13	R9
      0x7C280600,  //  0072  CALL	R10	3
      0x7002008C,  //  0073  JMP		#0101
      0x5820000E,  //  0074  LDCONST	R8	K14
      0x58240018,  //  0075  LDCONST	R9	K24
      0x1C280318,  //  0076  EQ	R10	R1	K24
      0x782A0001,  //  0077  JMPF	R10	#007A
      0x5429FFFE,  //  0078  LDINT	R10	-1
      0x0824120A,  //  0079  MUL	R9	R9	R10
      0x14281005,  //  007A  LT	R10	R8	R5
      0x782A0084,  //  007B  JMPF	R10	#0101
      0x08281006,  //  007C  MUL	R10	R8	R6
      0x5C1C1400,  //  007D  MOVE	R7	R10
      0x2428130E,  //  007E  GT	R10	R9	K14
      0x782A003E,  //  007F  JMPF	R10	#00BF
      0x00280E06,  //  0080  ADD	R10	R7	R6
      0x04281409,  //  0081  SUB	R10	R10	R9
      0x082C1118,  //  0082  MUL	R11	R8	K24
      0x94300807,  //  0083  GETIDX	R12	R4	R7
      0x9808160C,  //  0084  SETIDX	R2	R11	R12
      0x082C1118,  //  0085  MUL	R11	R8	K24
      0x002C1705,  //  0086  ADD	R11	R11	K5
      0x00300F05,  //  0087  ADD	R12	R7	K5
      0x9430080C,  //  0088  GETIDX	R12	R4	R12
      0x9808160C,  //  0089  SETIDX	R2	R11	R12
      0x082C1118,  //  008A  MUL	R11	R8	K24
      0x002C1719,  //  008B  ADD	R11	R11	K25
      0x00300F19,  //  008C  ADD	R12	R7	K25
      0x9430080C,  //  008D  GETIDX	R12	R4	R12
      0x9808160C,  //  008E  SETIDX	R2	R11	R12
      0x142C0E0A,  //  008F  LT	R11	R7	R10
      0x782E000E,  //  0090  JMPF	R11	#00A0
      0x002C0F18,  //  0091  ADD	R11	R7	K24
      0x942C080B,  //  0092  GETIDX	R11	R4	R11
      0x98100E0B,  //  0093  SETIDX	R4	R7	R11
      0x002C0F05,  //  0094  ADD	R11	R7	K5
      0x54320003,  //  0095  LDINT	R12	4
      0x00300E0C,  //  0096  ADD	R12	R7	R12
      0x9430080C,  //  0097  GETIDX	R12	R4	R12
      0x9810160C,  //  0098  SETIDX	R4	R11	R12
      0x002C0F19,  //  0099  ADD	R11	R7	K25
      0x54320004,  //  009A  LDINT	R12	5
      0x00300E0C,  //  009B  ADD	R12	R7	R12
      0x9430080C,  //  009C  GETIDX	R12	R4	R12
      0x9810160C,  //  009D  SETIDX	R4	R11	R12
      0x001C0E09,  //  009E  ADD	R7	R7	R9
      0x7001FFEE,  //  009F  JMP		#008F
      0x4C2C0000,  //  00A0  LDNIL	R11
      0x1C2C060B,  //  00A1  EQ	R11	R3	R11
      0x782E000D,  //  00A2  JMPF	R11	#00B1
      0x082C1118,  //  00A3  MUL	R11	R8	K24
      0x942C040B,  //  00A4  GETIDX	R11	R2	R11
      0x9810140B,  //  00A5  SETIDX	R4	R10	R11
      0x002C1505,  //  00A6  ADD	R11	R10	K5
      0x08301118,  //  00A7  MUL	R12	R8	K24
      0x00301905,  //  00A8  ADD	R12	R12	K5
      0x9430040C,  //  00A9  GETIDX	R12	R2	R12
      0x9810160C,  //  00AA  SETIDX	R4	R11	R12
      0x002C1519,  //  00AB  ADD	R11	R10	K25
      0x08301118,  //  00AC  MUL	R12	R8	K24
      0x00301919,  //  00AD  ADD	R12	R12	K25
      0x9430040C,  //  00AE  GETIDX	R12	R2	R12
      0x9810160C,  //  00AF  SETIDX	R4	R11	R12
      0x7002000C,  //  00B0  JMP		#00BE
      0x082C1118,  //  00B1  MUL	R11	R8	K24
      0x942C060B,  //  00B2  GETIDX	R11	R3	R11
      0x9810140B,  //  00B3  SETIDX	R4	R10	R11
      0x002C1505,  //  00B4  ADD	R11	R10	K5
      0x08301118,  //  00B5  MUL	R12	R8	K24
      0x00301905,  //  00B6  ADD	R12	R12	K5
      0x9430060C,  //  00B7  GETIDX	R12	R3	R12
      0x9810160C,  //  00B8  SETIDX	R4	R11	R12
      0x002C1519,  //  00B9  ADD	R11	R10	K25
      0x08301118,  //  00BA  MUL	R12	R8	K24
      0x00301919,  //  00BB  ADD	R12	R12	K25
      0x9430060C,  //  00BC  GETIDX	R12	R3	R12
      0x9810160C,  //  00BD  SETIDX	R4	R11	R12
      0x7002003D,  //  00BE  JMP		#00FD
      0x5C280E00,  //  00BF  MOVE	R10	R7
      0x002C0E06,  //  00C0  ADD	R11	R7	R6
      0x002C1609,  //  00C1  ADD	R11	R11	R9
      0x5C1C1600,  //  00C2  MOVE	R7	R11
      0x082C1118,  //  00C3  MUL	R11	R8	K24
      0x94300807,  //  00C4  GETIDX	R12	R4	R7
      0x9808160C,  //  00C5  SETIDX	R2	R11	R12
      0x082C1118,  //  00C6  MUL	R11	R8	K24
      0x002C1705,  //  00C7  ADD	R11	R11	K5
      0x00300F05,  //  00C8  ADD	R12	R7	K5
      0x9430080C,  //  00C9  GETIDX	R12	R4	R12
      0x9808160C,  //  00CA  SETIDX	R2	R11	R12
      0x082C1118,  //  00CB  MUL	R11	R8	K24
      0x002C1719,  //  00CC  ADD	R11	R11	K25
      0x00300F19,  //  00CD  ADD	R12	R7	K25
      0x9430080C,  //  00CE  GETIDX	R12	R4	R12
      0x9808160C,  //  00CF  SETIDX	R2	R11	R12
      0x242C0E0A,  //  00D0  GT	R11	R7	R10
      0x782E000C,  //  00D1  JMPF	R11	#00DF
      0x042C0F18,  //  00D2  SUB	R11	R7	K24
      0x942C080B,  //  00D3  GETIDX	R11	R4	R11
      0x98100E0B,  //  00D4  SETIDX	R4	R7	R11
      0x002C0F05,  //  00D5  ADD	R11	R7	K5
      0x04300F19,  //  00D6  SUB	R12	R7	K25
      0x9430080C,  //  00D7  GETIDX	R12	R4	R12
      0x9810160C,  //  00D8  SETIDX	R4	R11	R12
      0x002C0F19,  //  00D9  ADD	R11	R7	K25
      0x04300F05,  //  00DA  SUB	R12	R7	K5
      0x9430080C,  //  00DB  GETIDX	R12	R4	R12
      0x9810160C,  //  00DC  SETIDX	R4	R11	R12
      0x001C0E09,  //  00DD  ADD	R7	R7	R9
      0x7001FFF0,  //  00DE  JMP		#00D0
      0x4C2C0000,  //  00DF  LDNIL	R11
      0x1C2C060B,  //  00E0  EQ	R11	R3	R11
      0x782E000D,  //  00E1  JMPF	R11	#00F0
      0x082C1118,  //  00E2  MUL	R11	R8	K24
      0x942C040B,  //  00E3  GETIDX	R11	R2	R11
      0x9810140B,  //  00E4  SETIDX	R4	R10	R11
      0x002C1505,  //  00E5  ADD	R11	R10	K5
      0x08301118,  //  00E6  MUL	R12	R8	K24
      0x00301905,  //  00E7  ADD	R12	R12	K5
      0x9430040C,  //  00E8  GETIDX	R12	R2	R12
      0x9810160C,  //  00E9  SETIDX	R4	R11	R12
      0x002C1519,  //  00EA  ADD	R11	R10	K25
      0x08301118,  //  00EB  MUL	R12	R8	K24
      0x00301919,  //  00EC  ADD	R12	R12	K25
      0x9430040C,  //  00ED  GETIDX	R12	R2	R12
      0x9810160C,  //  00EE  SETIDX	R4	R11	R12
      0x7002000C,  //  00EF  JMP		#00FD
      0x082C1118,  //  00F0  MUL	R11	R8	K24
      0x942C060B,  //  00F1  GETIDX	R11	R3	R11
      0x9810140B,  //  00F2  SETIDX	R4	R10	R11
      0x002C1505,  //  00F3  ADD	R11	R10	K5
      0x08301118,  //  00F4  MUL	R12	R8	K24
      0x00301905,  //  00F5  ADD	R12	R12	K5
      0x9430060C,  //  00F6  GETIDX	R12	R3	R12
      0x9810160C,  //  00F7  SETIDX	R4	R11	R12
      0x002C1519,  //  00F8  ADD	R11	R10	K25
      0x08301118,  //  00F9  MUL	R12	R8	K24
      0x00301919,  //  00FA  ADD	R12	R12	K25
      0x9430060C,  //  00FB  GETIDX	R12	R3	R12
      0x9810160C,  //  00FC  SETIDX	R4	R11	R12
      0x5429FFFE,  //  00FD  LDINT	R10	-1
      0x0824120A,  //  00FE  MUL	R9	R9	R10
      0x00201105,  //  00FF  ADD	R8	R8	K5
      0x7001FF78,  //  0100  JMP		#007A
      0x80000000,  //  0101  RET	0
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
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x88080108,  //  0001  GETMBR	R2	R0	K8
      0x08040202,  //  0002  MUL	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
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
      0x880C0903,  //  0004  GETMBR	R3	R4	K3
      0x88100100,  //  0005  GETMBR	R4	R0	K0
      0x8C100906,  //  0006  GETMET	R4	R4	K6
      0x88180109,  //  0007  GETMBR	R6	R0	K9
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
      0x8C040311,  //  0001  GETMET	R1	R1	K17
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
    7,
    NULL,
    be_nested_map(26,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(is_dirty, -1), be_const_closure(class_Leds_matrix_is_dirty_closure) },
        { be_const_key(pixels_buffer, -1), be_const_closure(class_Leds_matrix_pixels_buffer_closure) },
        { be_const_key(set_matrix_pixel_color, 10), be_const_closure(class_Leds_matrix_set_matrix_pixel_color_closure) },
        { be_const_key(can_show, -1), be_const_closure(class_Leds_matrix_can_show_closure) },
        { be_const_key(offset, -1), be_const_var(1) },
        { be_const_key(set_bytes, 15), be_const_closure(class_Leds_matrix_set_bytes_closure) },
        { be_const_key(show, -1), be_const_closure(class_Leds_matrix_show_closure) },
        { be_const_key(alternate, -1), be_const_var(4) },
        { be_const_key(set_pixel_color, -1), be_const_closure(class_Leds_matrix_set_pixel_color_closure) },
        { be_const_key(pixel_count, -1), be_const_closure(class_Leds_matrix_pixel_count_closure) },
        { be_const_key(w, -1), be_const_var(3) },
        { be_const_key(dirty, 24), be_const_closure(class_Leds_matrix_dirty_closure) },
        { be_const_key(get_alternate, 13), be_const_closure(class_Leds_matrix_get_alternate_closure) },
        { be_const_key(get_pixel_color, -1), be_const_closure(class_Leds_matrix_get_pixel_color_closure) },
        { be_const_key(pix_size, -1), be_const_var(6) },
        { be_const_key(strip, -1), be_const_var(0) },
        { be_const_key(clear_to, 21), be_const_closure(class_Leds_matrix_clear_to_closure) },
        { be_const_key(init, -1), be_const_closure(class_Leds_matrix_init_closure) },
        { be_const_key(set_alternate, 8), be_const_closure(class_Leds_matrix_set_alternate_closure) },
        { be_const_key(pix_buffer, 17), be_const_var(5) },
        { be_const_key(clear, -1), be_const_closure(class_Leds_matrix_clear_closure) },
        { be_const_key(begin, -1), be_const_closure(class_Leds_matrix_begin_closure) },
        { be_const_key(scroll, -1), be_const_closure(class_Leds_matrix_scroll_closure) },
        { be_const_key(h, 9), be_const_var(2) },
        { be_const_key(pixel_offset, -1), be_const_closure(class_Leds_matrix_pixel_offset_closure) },
        { be_const_key(pixel_size, 1), be_const_closure(class_Leds_matrix_pixel_size_closure) },
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
    7,                          /* nstack */
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
    ( &(const binstruction[27]) {  /* code */
      0x8C080109,  //  0000  GETMET	R2	R0	K9
      0x54120005,  //  0001  LDINT	R4	6
      0x7C080400,  //  0002  CALL	R2	2
      0x8C0C010A,  //  0003  GETMET	R3	R0	K10
      0x7C0C0200,  //  0004  CALL	R3	1
      0x8C10010B,  //  0005  GETMET	R4	R0	K11
      0x7C100200,  //  0006  CALL	R4	1
      0x080C0604,  //  0007  MUL	R3	R3	R4
      0x4C100000,  //  0008  LDNIL	R4
      0x1C100204,  //  0009  EQ	R4	R1	R4
      0x74120004,  //  000A  JMPT	R4	#0010
      0x6010000C,  //  000B  GETGBL	R4	G12
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C100200,  //  000D  CALL	R4	1
      0x20100803,  //  000E  NE	R4	R4	R3
      0x78120005,  //  000F  JMPF	R4	#0016
      0x60100015,  //  0010  GETGBL	R4	G21
      0x5C140400,  //  0011  MOVE	R5	R2
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C100400,  //  0013  CALL	R4	2
      0x80040800,  //  0014  RET	1	R4
      0x70020003,  //  0015  JMP		#001A
      0x8C10030C,  //  0016  GETMET	R4	R1	K12
      0x5C180400,  //  0017  MOVE	R6	R2
      0x7C100400,  //  0018  CALL	R4	2
      0x80040200,  //  0019  RET	1	R1
      0x80000000,  //  001A  RET	0
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
