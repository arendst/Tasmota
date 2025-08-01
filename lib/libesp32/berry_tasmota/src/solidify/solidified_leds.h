/* Solidification of leds.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Leds_segment;
// compact class 'Leds_segment' ktab size: 19, total: 50 (saved 248 bytes)
static const bvalue be_ktab_class_Leds_segment[19] = {
  /* K0   */  be_nested_str(strip),
  /* K1   */  be_nested_str(get_pixel_color),
  /* K2   */  be_nested_str(offset),
  /* K3   */  be_nested_str(can_show_wait),
  /* K4   */  be_nested_str(bri),
  /* K5   */  be_nested_str(set_pixel_color),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str(animate),
  /* K8   */  be_nested_str(leds),
  /* K9   */  be_nested_str(show),
  /* K10  */  be_nested_str(gamma),
  /* K11  */  be_nested_str(apply_bri_gamma),
  /* K12  */  be_nested_str(pixel_size),
  /* K13  */  be_nested_str(can_show),
  /* K14  */  be_nested_str(is_dirty),
  /* K15  */  be_nested_str(call_native),
  /* K16  */  be_nested_str(to_gamma),
  /* K17  */  be_nested_str(dirty),
  /* K18  */  be_nested_str(clear_to),
};


extern const bclass be_class_Leds_segment;

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
** Solidified function: can_show_wait
********************************************************************/
be_local_closure(class_Leds_segment_can_show_wait,   /* name */
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
    &be_const_str_can_show_wait,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040303,  //  0001  GETMET	R1	R1	K3
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
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
      0x880C0104,  //  0003  GETMBR	R3	R0	K4
      0x88100100,  //  0004  GETMBR	R4	R0	K0
      0x8C100905,  //  0005  GETMET	R4	R4	K5
      0x88180102,  //  0006  GETMBR	R6	R0	K2
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
** Solidified function: set_bri
********************************************************************/
be_local_closure(class_Leds_segment_set_bri,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_set_bri,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x14080306,  //  0000  LT	R2	R1	K6
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040006,  //  0002  LDCONST	R1	K6
      0x540A00FE,  //  0003  LDINT	R2	255
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540600FE,  //  0006  LDINT	R1	255
      0x90020801,  //  0007  SETMBR	R0	K4	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_animate
********************************************************************/
be_local_closure(class_Leds_segment_get_animate,   /* name */
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
    &be_const_str_get_animate,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x80040200,  //  0001  RET	1	R1
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
      0x88080102,  //  0004  GETMBR	R2	R0	K2
      0x1C080506,  //  0005  EQ	R2	R2	K6
      0x780A0007,  //  0006  JMPF	R2	#000F
      0x88080108,  //  0007  GETMBR	R2	R0	K8
      0x880C0100,  //  0008  GETMBR	R3	R0	K0
      0x880C0708,  //  0009  GETMBR	R3	R3	K8
      0x1C080403,  //  000A  EQ	R2	R2	R3
      0x780A0002,  //  000B  JMPF	R2	#000F
      0x88080100,  //  000C  GETMBR	R2	R0	K0
      0x8C080509,  //  000D  GETMET	R2	R2	K9
      0x7C080200,  //  000E  CALL	R2	1
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_gamma
********************************************************************/
be_local_closure(class_Leds_segment_set_gamma,   /* name */
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
    &be_const_str_set_gamma,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90021402,  //  0003  SETMBR	R0	K10	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_gamma
********************************************************************/
be_local_closure(class_Leds_segment_to_gamma,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_to_gamma,
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x88080104,  //  0003  GETMBR	R2	R0	K4
      0x880C0100,  //  0004  GETMBR	R3	R0	K0
      0x8C0C070B,  //  0005  GETMET	R3	R3	K11
      0x5C140200,  //  0006  MOVE	R5	R1
      0x5C180400,  //  0007  MOVE	R6	R2
      0x881C010A,  //  0008  GETMBR	R7	R0	K10
      0x7C0C0800,  //  0009  CALL	R3	4
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


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
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bri
********************************************************************/
be_local_closure(class_Leds_segment_get_bri,   /* name */
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
    &be_const_str_get_bri,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_gamma
********************************************************************/
be_local_closure(class_Leds_segment_get_gamma,   /* name */
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
    &be_const_str_get_gamma,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010A,  //  0000  GETMBR	R1	R0	K10
      0x80040200,  //  0001  RET	1	R1
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
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C04030C,  //  0001  GETMET	R1	R1	K12
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
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
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C04030D,  //  0001  GETMET	R1	R1	K13
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
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
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C04030E,  //  0001  GETMET	R1	R1	K14
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
    ( &(const binstruction[16]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60100009,  //  0001  GETGBL	R4	G9
      0x5C140400,  //  0002  MOVE	R5	R2
      0x7C100200,  //  0003  CALL	R4	1
      0x90020404,  //  0004  SETMBR	R0	K2	R4
      0x60100009,  //  0005  GETGBL	R4	G9
      0x5C140600,  //  0006  MOVE	R5	R3
      0x7C100200,  //  0007  CALL	R4	1
      0x90021004,  //  0008  SETMBR	R0	K8	R4
      0x88100304,  //  0009  GETMBR	R4	R1	K4
      0x90020804,  //  000A  SETMBR	R0	K4	R4
      0x8810030A,  //  000B  GETMBR	R4	R1	K10
      0x90021404,  //  000C  SETMBR	R0	K10	R4
      0x4C100000,  //  000D  LDNIL	R4
      0x90020E04,  //  000E  SETMBR	R0	K7	R4
      0x80000000,  //  000F  RET	0
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
    ( &(const binstruction[15]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x88080104,  //  0003  GETMBR	R2	R0	K4
      0x880C0100,  //  0004  GETMBR	R3	R0	K0
      0x8C0C070F,  //  0005  GETMET	R3	R3	K15
      0x54160008,  //  0006  LDINT	R5	9
      0x8C180110,  //  0007  GETMET	R6	R0	K16
      0x5C200200,  //  0008  MOVE	R8	R1
      0x5C240400,  //  0009  MOVE	R9	R2
      0x7C180600,  //  000A  CALL	R6	3
      0x881C0102,  //  000B  GETMBR	R7	R0	K2
      0x88200108,  //  000C  GETMBR	R8	R0	K8
      0x7C0C0A00,  //  000D  CALL	R3	5
      0x80000000,  //  000E  RET	0
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
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040311,  //  0001  GETMET	R1	R1	K17
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
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
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x80040200,  //  0001  RET	1	R1
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
      0x8C040112,  //  0000  GETMET	R1	R0	K18
      0x580C0006,  //  0001  LDCONST	R3	K6
      0x7C040400,  //  0002  CALL	R1	2
      0x8C040109,  //  0003  GETMET	R1	R0	K9
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_animate
********************************************************************/
be_local_closure(class_Leds_segment_set_animate,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds_segment,     /* shared constants */
    &be_const_str_set_animate,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020E01,  //  0000  SETMBR	R0	K7	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds_segment
********************************************************************/
be_local_class(Leds_segment,
    6,
    NULL,
    be_nested_map(28,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(get_pixel_color, 21), be_const_closure(class_Leds_segment_get_pixel_color_closure) },
        { be_const_key(can_show_wait, -1), be_const_closure(class_Leds_segment_can_show_wait_closure) },
        { be_const_key(gamma, 20), be_const_var(4) },
        { be_const_key(leds, -1), be_const_var(2) },
        { be_const_key(set_pixel_color, -1), be_const_closure(class_Leds_segment_set_pixel_color_closure) },
        { be_const_key(set_animate, 15), be_const_closure(class_Leds_segment_set_animate_closure) },
        { be_const_key(to_gamma, -1), be_const_closure(class_Leds_segment_to_gamma_closure) },
        { be_const_key(get_animate, 12), be_const_closure(class_Leds_segment_get_animate_closure) },
        { be_const_key(show, -1), be_const_closure(class_Leds_segment_show_closure) },
        { be_const_key(set_gamma, -1), be_const_closure(class_Leds_segment_set_gamma_closure) },
        { be_const_key(offset, 6), be_const_var(1) },
        { be_const_key(pixel_offset, 14), be_const_closure(class_Leds_segment_pixel_offset_closure) },
        { be_const_key(pixel_count, -1), be_const_closure(class_Leds_segment_pixel_count_closure) },
        { be_const_key(get_gamma, -1), be_const_closure(class_Leds_segment_get_gamma_closure) },
        { be_const_key(pixels_buffer, -1), be_const_closure(class_Leds_segment_pixels_buffer_closure) },
        { be_const_key(dirty, 23), be_const_closure(class_Leds_segment_dirty_closure) },
        { be_const_key(begin, -1), be_const_closure(class_Leds_segment_begin_closure) },
        { be_const_key(pixel_size, 22), be_const_closure(class_Leds_segment_pixel_size_closure) },
        { be_const_key(is_dirty, 16), be_const_closure(class_Leds_segment_is_dirty_closure) },
        { be_const_key(init, -1), be_const_closure(class_Leds_segment_init_closure) },
        { be_const_key(clear_to, -1), be_const_closure(class_Leds_segment_clear_to_closure) },
        { be_const_key(bri, 24), be_const_var(3) },
        { be_const_key(strip, -1), be_const_var(0) },
        { be_const_key(can_show, 25), be_const_closure(class_Leds_segment_can_show_closure) },
        { be_const_key(animate, -1), be_const_var(5) },
        { be_const_key(get_bri, -1), be_const_closure(class_Leds_segment_get_bri_closure) },
        { be_const_key(clear, -1), be_const_closure(class_Leds_segment_clear_closure) },
        { be_const_key(set_bri, 5), be_const_closure(class_Leds_segment_set_bri_closure) },
    })),
    (bstring*) &be_const_str_Leds_segment
);
// compact class 'Leds' ktab size: 38, total: 69 (saved 248 bytes)
static const bvalue be_ktab_class_Leds[38] = {
  /* K0   */  be_nested_str(call_native),
  /* K1   */  be_nested_str(pixel_size),
  /* K2   */  be_nested_str(pixel_count),
  /* K3   */  be_nested_str(_change_buffer),
  /* K4   */  be_nested_str(gamma),
  /* K5   */  be_const_int(1),
  /* K6   */  be_const_int(2),
  /* K7   */  be_nested_str(animate),
  /* K8   */  be_nested_str(gpio),
  /* K9   */  be_nested_str(pin),
  /* K10  */  be_nested_str(WS2812),
  /* K11  */  be_const_int(0),
  /* K12  */  be_nested_str(ctor),
  /* K13  */  be_nested_str(leds),
  /* K14  */  be_nested_str(light),
  /* K15  */  be_nested_str(bri),
  /* K16  */  be_nested_str(get),
  /* K17  */  be_nested_str(global),
  /* K18  */  be_nested_str(contains),
  /* K19  */  be_nested_str(_lhw),
  /* K20  */  be_nested_str(find),
  /* K21  */  be_nested_str(number_X20of_X20leds_X20do_X20not_X20match_X20with_X20previous_X20instanciation_X20_X25s_X20vs_X20_X25s),
  /* K22  */  be_nested_str(value_error),
  /* K23  */  be_nested_str(_p),
  /* K24  */  be_nested_str(begin),
  /* K25  */  be_nested_str(internal_error),
  /* K26  */  be_nested_str(couldn_X27t_X20not_X20initialize_X20noepixelbus),
  /* K27  */  be_nested_str(to_gamma),
  /* K28  */  be_nested_str(clear_to),
  /* K29  */  be_nested_str(show),
  /* K30  */  be_nested_str(apply_bri_gamma),
  /* K31  */  be_nested_str(can_show),
  /* K32  */  be_nested_str(tasmota),
  /* K33  */  be_nested_str(yield),
  /* K34  */  be_const_int(3),
  /* K35  */  be_nested_str(out_X20of_X20range),
  /* K36  */  be_const_class(be_class_Leds_segment),
  /* K37  */  be_nested_str(WS2812_GRB),
};


extern const bclass be_class_Leds;

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
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x54120005,  //  0001  LDINT	R4	6
      0x7C080400,  //  0002  CALL	R2	2
      0x8C0C0101,  //  0003  GETMET	R3	R0	K1
      0x7C0C0200,  //  0004  CALL	R3	1
      0x8C100102,  //  0005  GETMET	R4	R0	K2
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
      0x8C100303,  //  0016  GETMET	R4	R1	K3
      0x5C180400,  //  0017  MOVE	R6	R2
      0x7C100400,  //  0018  CALL	R4	2
      0x80040200,  //  0019  RET	1	R1
      0x80000000,  //  001A  RET	0
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
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: length
********************************************************************/
be_local_closure(class_Leds_length,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_length,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040102,  //  0000  GETMET	R1	R0	K2
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
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
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0005,  //  0001  LDCONST	R3	K5
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
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
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0006,  //  0001  LDCONST	R3	K6
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
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
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x80040200,  //  0001  RET	1	R1
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
      0xA4161000,  //  0000  IMPORT	R5	K8
      0x50180200,  //  0001  LDBOOL	R6	1	0
      0x90020806,  //  0002  SETMBR	R0	K4	R6
      0x4C180000,  //  0003  LDNIL	R6
      0x1C180206,  //  0004  EQ	R6	R1	R6
      0x741A0008,  //  0005  JMPT	R6	#000F
      0x4C180000,  //  0006  LDNIL	R6
      0x1C180406,  //  0007  EQ	R6	R2	R6
      0x741A0005,  //  0008  JMPT	R6	#000F
      0x8C180B09,  //  0009  GETMET	R6	R5	K9
      0x88200B0A,  //  000A  GETMBR	R8	R5	K10
      0x5824000B,  //  000B  LDCONST	R9	K11
      0x7C180600,  //  000C  CALL	R6	3
      0x1C180406,  //  000D  EQ	R6	R2	R6
      0x781A000A,  //  000E  JMPF	R6	#001A
      0x8C18010C,  //  000F  GETMET	R6	R0	K12
      0x7C180200,  //  0010  CALL	R6	1
      0x8C180102,  //  0011  GETMET	R6	R0	K2
      0x7C180200,  //  0012  CALL	R6	1
      0x90021A06,  //  0013  SETMBR	R0	K13	R6
      0xA41A1C00,  //  0014  IMPORT	R6	K14
      0x8C1C0D10,  //  0015  GETMET	R7	R6	K16
      0x7C1C0200,  //  0016  CALL	R7	1
      0x941C0F0F,  //  0017  GETIDX	R7	R7	K15
      0x90021E07,  //  0018  SETMBR	R0	K15	R7
      0x70020039,  //  0019  JMP		#0054
      0x60180009,  //  001A  GETGBL	R6	G9
      0x5C1C0200,  //  001B  MOVE	R7	R1
      0x7C180200,  //  001C  CALL	R6	1
      0x5C040C00,  //  001D  MOVE	R1	R6
      0x90021A01,  //  001E  SETMBR	R0	K13	R1
      0x541A007E,  //  001F  LDINT	R6	127
      0x90021E06,  //  0020  SETMBR	R0	K15	R6
      0xB81A2200,  //  0021  GETNGBL	R6	K17
      0x8C180D12,  //  0022  GETMET	R6	R6	K18
      0x58200013,  //  0023  LDCONST	R8	K19
      0x7C180400,  //  0024  CALL	R6	2
      0x741A0003,  //  0025  JMPT	R6	#002A
      0xB81A2200,  //  0026  GETNGBL	R6	K17
      0x601C0013,  //  0027  GETGBL	R7	G19
      0x7C1C0000,  //  0028  CALL	R7	0
      0x901A2607,  //  0029  SETMBR	R6	K19	R7
      0xB81A2200,  //  002A  GETNGBL	R6	K17
      0x88180D13,  //  002B  GETMBR	R6	R6	K19
      0x8C180D14,  //  002C  GETMET	R6	R6	K20
      0x5C200200,  //  002D  MOVE	R8	R1
      0x7C180400,  //  002E  CALL	R6	2
      0x4C1C0000,  //  002F  LDNIL	R7
      0x20180C07,  //  0030  NE	R6	R6	R7
      0x781A0016,  //  0031  JMPF	R6	#0049
      0xB81A2200,  //  0032  GETNGBL	R6	K17
      0x88180D13,  //  0033  GETMBR	R6	R6	K19
      0x8C180D14,  //  0034  GETMET	R6	R6	K20
      0x5C200200,  //  0035  MOVE	R8	R1
      0x7C180400,  //  0036  CALL	R6	2
      0x881C010D,  //  0037  GETMBR	R7	R0	K13
      0x88200D0D,  //  0038  GETMBR	R8	R6	K13
      0x201C0E08,  //  0039  NE	R7	R7	R8
      0x781E0005,  //  003A  JMPF	R7	#0041
      0x601C0018,  //  003B  GETGBL	R7	G24
      0x58200015,  //  003C  LDCONST	R8	K21
      0x8824010D,  //  003D  GETMBR	R9	R0	K13
      0x88280D0D,  //  003E  GETMBR	R10	R6	K13
      0x7C1C0600,  //  003F  CALL	R7	3
      0xB0062C07,  //  0040  RAISE	1	K22	R7
      0x881C0D17,  //  0041  GETMBR	R7	R6	K23
      0x90022E07,  //  0042  SETMBR	R0	K23	R7
      0x881C0D07,  //  0043  GETMBR	R7	R6	K7
      0x90020E07,  //  0044  SETMBR	R0	K7	R7
      0xB81E2200,  //  0045  GETNGBL	R7	K17
      0x881C0F13,  //  0046  GETMBR	R7	R7	K19
      0x981C0200,  //  0047  SETIDX	R7	R1	R0
      0x7002000A,  //  0048  JMP		#0054
      0x8C18010C,  //  0049  GETMET	R6	R0	K12
      0x5C200200,  //  004A  MOVE	R8	R1
      0x5C240400,  //  004B  MOVE	R9	R2
      0x5C280600,  //  004C  MOVE	R10	R3
      0x5C2C0800,  //  004D  MOVE	R11	R4
      0x7C180A00,  //  004E  CALL	R6	5
      0xB81A2200,  //  004F  GETNGBL	R6	K17
      0x88180D13,  //  0050  GETMBR	R6	R6	K19
      0x98180200,  //  0051  SETIDX	R6	R1	R0
      0x8C180118,  //  0052  GETMET	R6	R0	K24
      0x7C180200,  //  0053  CALL	R6	1
      0x88180117,  //  0054  GETMBR	R6	R0	K23
      0x4C1C0000,  //  0055  LDNIL	R7
      0x1C180C07,  //  0056  EQ	R6	R6	R7
      0x781A0000,  //  0057  JMPF	R6	#0059
      0xB006331A,  //  0058  RAISE	1	K25	K26
      0x80000000,  //  0059  RET	0
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
      0x8C100100,  //  0004  GETMET	R4	R0	K0
      0x541A0009,  //  0005  LDINT	R6	10
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x8C20011B,  //  0007  GETMET	R8	R0	K27
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
      0x8C040100,  //  0000  GETMET	R1	R0	K0
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
      0x8C04011C,  //  0000  GETMET	R1	R0	K28
      0x580C000B,  //  0001  LDCONST	R3	K11
      0x7C040400,  //  0002  CALL	R1	2
      0x8C04011D,  //  0003  GETMET	R1	R0	K29
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
      0x8C040100,  //  0000  GETMET	R1	R0	K0
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
      0x8C040100,  //  0000  GETMET	R1	R0	K0
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
      0x8C0C011E,  //  0004  GETMET	R3	R0	K30
      0x5C140200,  //  0005  MOVE	R5	R1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x881C0104,  //  0007  GETMBR	R7	R0	K4
      0x7C0C0800,  //  0008  CALL	R3	4
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_show_wait
********************************************************************/
be_local_closure(class_Leds_can_show_wait,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Leds,     /* shared constants */
    &be_const_str_can_show_wait,
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C04011F,  //  0000  GETMET	R1	R0	K31
      0x7C040200,  //  0001  CALL	R1	1
      0x74060003,  //  0002  JMPT	R1	#0007
      0xB8064000,  //  0003  GETNGBL	R1	K32
      0x8C040321,  //  0004  GETMET	R1	R1	K33
      0x7C040200,  //  0005  CALL	R1	1
      0x7001FFF8,  //  0006  JMP		#0000
      0x80000000,  //  0007  RET	0
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
      0x8C040100,  //  0000  GETMET	R1	R0	K0
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
      0x1408030B,  //  0000  LT	R2	R1	K11
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x5804000B,  //  0002  LDCONST	R1	K11
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
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0022,  //  0001  LDCONST	R3	K34
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
      0x90020E01,  //  0000  SETMBR	R0	K7	R1
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
      0x8C140100,  //  000A  GETMET	R5	R0	K0
      0x541E0008,  //  000B  LDINT	R7	9
      0x8C20011B,  //  000C  GETMET	R8	R0	K27
      0x5C280200,  //  000D  MOVE	R10	R1
      0x5C2C0400,  //  000E  MOVE	R11	R2
      0x7C200600,  //  000F  CALL	R8	3
      0x5C240600,  //  0010  MOVE	R9	R3
      0x5C280800,  //  0011  MOVE	R10	R4
      0x7C140A00,  //  0012  CALL	R5	5
      0x70020006,  //  0013  JMP		#001B
      0x8C140100,  //  0014  GETMET	R5	R0	K0
      0x541E0008,  //  0015  LDINT	R7	9
      0x8C20011B,  //  0016  GETMET	R8	R0	K27
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
      0x80061600,  //  0000  RET	1	K11
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
      0x8810010D,  //  0007  GETMBR	R4	R0	K13
      0x240C0604,  //  0008  GT	R3	R3	R4
      0x740E0003,  //  0009  JMPT	R3	#000E
      0x140C030B,  //  000A  LT	R3	R1	K11
      0x740E0001,  //  000B  JMPT	R3	#000E
      0x140C050B,  //  000C  LT	R3	R2	K11
      0x780E0000,  //  000D  JMPF	R3	#000F
      0xB0062D23,  //  000E  RAISE	1	K22	K35
      0x580C0024,  //  000F  LDCONST	R3	K36
      0xB4000024,  //  0010  CLASS	K36
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
      0x8C140100,  //  0003  GETMET	R5	R0	K0
      0x581C000B,  //  0004  LDCONST	R7	K11
      0x7C140400,  //  0005  CALL	R5	2
      0x7002000A,  //  0006  JMP		#0012
      0x4C140000,  //  0007  LDNIL	R5
      0x1C140605,  //  0008  EQ	R5	R3	R5
      0x78160000,  //  0009  JMPF	R5	#000B
      0x880C0125,  //  000A  GETMBR	R3	R0	K37
      0x8C140100,  //  000B  GETMET	R5	R0	K0
      0x581C000B,  //  000C  LDCONST	R7	K11
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
      0x90020802,  //  0003  SETMBR	R0	K4	R2
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
        { be_const_key(begin, -1), be_const_closure(class_Leds_begin_closure) },
        { be_const_key(get_gamma, 26), be_const_closure(class_Leds_get_gamma_closure) },
        { be_const_key(length, -1), be_const_closure(class_Leds_length_closure) },
        { be_const_key(pixels_buffer, 0), be_const_closure(class_Leds_pixels_buffer_closure) },
        { be_const_key(clear, -1), be_const_closure(class_Leds_clear_closure) },
        { be_const_key(get_animate, -1), be_const_closure(class_Leds_get_animate_closure) },
        { be_const_key(create_segment, 14), be_const_closure(class_Leds_create_segment_closure) },
        { be_const_key(set_pixel_color, -1), be_const_closure(class_Leds_set_pixel_color_closure) },
        { be_const_key(animate, -1), be_const_var(3) },
        { be_const_key(is_dirty, -1), be_const_closure(class_Leds_is_dirty_closure) },
        { be_const_key(init, 6), be_const_closure(class_Leds_init_closure) },
        { be_const_key(get_pixel_color, 13), be_const_closure(class_Leds_get_pixel_color_closure) },
        { be_const_key(pixel_offset, -1), be_const_closure(class_Leds_pixel_offset_closure) },
        { be_const_key(clear_to, -1), be_const_closure(class_Leds_clear_to_closure) },
        { be_const_key(can_show_wait, 23), be_const_closure(class_Leds_can_show_wait_closure) },
        { be_const_key(set_animate, -1), be_const_closure(class_Leds_set_animate_closure) },
        { be_const_key(can_show, 24), be_const_closure(class_Leds_can_show_closure) },
        { be_const_key(dirty, -1), be_const_closure(class_Leds_dirty_closure) },
        { be_const_key(set_bri, -1), be_const_closure(class_Leds_set_bri_closure) },
        { be_const_key(leds, 16), be_const_var(1) },
        { be_const_key(gamma, -1), be_const_var(0) },
        { be_const_key(bri, 15), be_const_var(2) },
        { be_const_key(get_bri, 12), be_const_closure(class_Leds_get_bri_closure) },
        { be_const_key(to_gamma, 4), be_const_closure(class_Leds_to_gamma_closure) },
        { be_const_key(pixel_count, -1), be_const_closure(class_Leds_pixel_count_closure) },
        { be_const_key(show, 11), be_const_closure(class_Leds_show_closure) },
        { be_const_key(pixel_size, -1), be_const_closure(class_Leds_pixel_size_closure) },
        { be_const_key(ctor, -1), be_const_closure(class_Leds_ctor_closure) },
        { be_const_key(set_gamma, -1), be_const_closure(class_Leds_set_gamma_closure) },
    })),
    (bstring*) &be_const_str_Leds
);
/********************************************************************/
/* End of solidification */
