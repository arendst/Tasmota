/* Solidification of leds.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Leds_segment;
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
// compact class 'Leds' ktab size: 35, total: 65 (saved 240 bytes)
static const bvalue be_ktab_class_Leds[35] = {
  /* K0   */  be_nested_str(leds),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str(value_error),
  /* K3   */  be_nested_str(out_X20of_X20range),
  /* K4   */  be_const_class(be_class_Leds_segment),
  /* K5   */  be_nested_str(bri),
  /* K6   */  be_nested_str(call_native),
  /* K7   */  be_const_int(1),
  /* K8   */  be_nested_str(clear_to),
  /* K9   */  be_nested_str(show),
  /* K10  */  be_const_int(2),
  /* K11  */  be_nested_str(apply_bri_gamma),
  /* K12  */  be_nested_str(gamma),
  /* K13  */  be_nested_str(gpio),
  /* K14  */  be_nested_str(pin),
  /* K15  */  be_nested_str(WS2812),
  /* K16  */  be_nested_str(ctor),
  /* K17  */  be_nested_str(pixel_count),
  /* K18  */  be_nested_str(light),
  /* K19  */  be_nested_str(get),
  /* K20  */  be_nested_str(global),
  /* K21  */  be_nested_str(contains),
  /* K22  */  be_nested_str(_lhw),
  /* K23  */  be_nested_str(find),
  /* K24  */  be_nested_str(number_X20of_X20leds_X20do_X20not_X20match_X20with_X20previous_X20instanciation_X20_X25s_X20vs_X20_X25s),
  /* K25  */  be_nested_str(_p),
  /* K26  */  be_nested_str(animate),
  /* K27  */  be_nested_str(begin),
  /* K28  */  be_nested_str(internal_error),
  /* K29  */  be_nested_str(couldn_X27t_X20not_X20initialize_X20noepixelbus),
  /* K30  */  be_nested_str(to_gamma),
  /* K31  */  be_const_int(3),
  /* K32  */  be_nested_str(WS2812_GRB),
  /* K33  */  be_nested_str(pixel_size),
  /* K34  */  be_nested_str(_change_buffer),
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
      0x90020A01,  //  0007  SETMBR	R0	K5	R1
      0x80000000,  //  0008  RET	0
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
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x580C0007,  //  0001  LDCONST	R3	K7
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
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
      0x8C040108,  //  0000  GETMET	R1	R0	K8
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x8C040109,  //  0003  GETMET	R1	R0	K9
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
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
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x580C000A,  //  0001  LDCONST	R3	K10
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
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
      0x88080105,  //  0003  GETMBR	R2	R0	K5
      0x8C0C010B,  //  0004  GETMET	R3	R0	K11
      0x5C140200,  //  0005  MOVE	R5	R1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x881C010C,  //  0007  GETMBR	R7	R0	K12
      0x7C0C0800,  //  0008  CALL	R3	4
      0x80040600,  //  0009  RET	1	R3
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
      0xA4161A00,  //  0000  IMPORT	R5	K13
      0x50180200,  //  0001  LDBOOL	R6	1	0
      0x90021806,  //  0002  SETMBR	R0	K12	R6
      0x4C180000,  //  0003  LDNIL	R6
      0x1C180206,  //  0004  EQ	R6	R1	R6
      0x741A0008,  //  0005  JMPT	R6	#000F
      0x4C180000,  //  0006  LDNIL	R6
      0x1C180406,  //  0007  EQ	R6	R2	R6
      0x741A0005,  //  0008  JMPT	R6	#000F
      0x8C180B0E,  //  0009  GETMET	R6	R5	K14
      0x88200B0F,  //  000A  GETMBR	R8	R5	K15
      0x58240001,  //  000B  LDCONST	R9	K1
      0x7C180600,  //  000C  CALL	R6	3
      0x1C180406,  //  000D  EQ	R6	R2	R6
      0x781A000A,  //  000E  JMPF	R6	#001A
      0x8C180110,  //  000F  GETMET	R6	R0	K16
      0x7C180200,  //  0010  CALL	R6	1
      0x8C180111,  //  0011  GETMET	R6	R0	K17
      0x7C180200,  //  0012  CALL	R6	1
      0x90020006,  //  0013  SETMBR	R0	K0	R6
      0xA41A2400,  //  0014  IMPORT	R6	K18
      0x8C1C0D13,  //  0015  GETMET	R7	R6	K19
      0x7C1C0200,  //  0016  CALL	R7	1
      0x941C0F05,  //  0017  GETIDX	R7	R7	K5
      0x90020A07,  //  0018  SETMBR	R0	K5	R7
      0x70020039,  //  0019  JMP		#0054
      0x60180009,  //  001A  GETGBL	R6	G9
      0x5C1C0200,  //  001B  MOVE	R7	R1
      0x7C180200,  //  001C  CALL	R6	1
      0x5C040C00,  //  001D  MOVE	R1	R6
      0x90020001,  //  001E  SETMBR	R0	K0	R1
      0x541A007E,  //  001F  LDINT	R6	127
      0x90020A06,  //  0020  SETMBR	R0	K5	R6
      0xB81A2800,  //  0021  GETNGBL	R6	K20
      0x8C180D15,  //  0022  GETMET	R6	R6	K21
      0x58200016,  //  0023  LDCONST	R8	K22
      0x7C180400,  //  0024  CALL	R6	2
      0x741A0003,  //  0025  JMPT	R6	#002A
      0xB81A2800,  //  0026  GETNGBL	R6	K20
      0x601C0013,  //  0027  GETGBL	R7	G19
      0x7C1C0000,  //  0028  CALL	R7	0
      0x901A2C07,  //  0029  SETMBR	R6	K22	R7
      0xB81A2800,  //  002A  GETNGBL	R6	K20
      0x88180D16,  //  002B  GETMBR	R6	R6	K22
      0x8C180D17,  //  002C  GETMET	R6	R6	K23
      0x5C200200,  //  002D  MOVE	R8	R1
      0x7C180400,  //  002E  CALL	R6	2
      0x4C1C0000,  //  002F  LDNIL	R7
      0x20180C07,  //  0030  NE	R6	R6	R7
      0x781A0016,  //  0031  JMPF	R6	#0049
      0xB81A2800,  //  0032  GETNGBL	R6	K20
      0x88180D16,  //  0033  GETMBR	R6	R6	K22
      0x8C180D17,  //  0034  GETMET	R6	R6	K23
      0x5C200200,  //  0035  MOVE	R8	R1
      0x7C180400,  //  0036  CALL	R6	2
      0x881C0100,  //  0037  GETMBR	R7	R0	K0
      0x88200D00,  //  0038  GETMBR	R8	R6	K0
      0x201C0E08,  //  0039  NE	R7	R7	R8
      0x781E0005,  //  003A  JMPF	R7	#0041
      0x601C0018,  //  003B  GETGBL	R7	G24
      0x58200018,  //  003C  LDCONST	R8	K24
      0x88240100,  //  003D  GETMBR	R9	R0	K0
      0x88280D00,  //  003E  GETMBR	R10	R6	K0
      0x7C1C0600,  //  003F  CALL	R7	3
      0xB0060407,  //  0040  RAISE	1	K2	R7
      0x881C0D19,  //  0041  GETMBR	R7	R6	K25
      0x90023207,  //  0042  SETMBR	R0	K25	R7
      0x881C0D1A,  //  0043  GETMBR	R7	R6	K26
      0x90023407,  //  0044  SETMBR	R0	K26	R7
      0xB81E2800,  //  0045  GETNGBL	R7	K20
      0x881C0F16,  //  0046  GETMBR	R7	R7	K22
      0x981C0200,  //  0047  SETIDX	R7	R1	R0
      0x7002000A,  //  0048  JMP		#0054
      0x8C180110,  //  0049  GETMET	R6	R0	K16
      0x5C200200,  //  004A  MOVE	R8	R1
      0x5C240400,  //  004B  MOVE	R9	R2
      0x5C280600,  //  004C  MOVE	R10	R3
      0x5C2C0800,  //  004D  MOVE	R11	R4
      0x7C180A00,  //  004E  CALL	R6	5
      0xB81A2800,  //  004F  GETNGBL	R6	K20
      0x88180D16,  //  0050  GETMBR	R6	R6	K22
      0x98180200,  //  0051  SETIDX	R6	R1	R0
      0x8C18011B,  //  0052  GETMET	R6	R0	K27
      0x7C180200,  //  0053  CALL	R6	1
      0x88180119,  //  0054  GETMBR	R6	R0	K25
      0x4C1C0000,  //  0055  LDNIL	R7
      0x1C180C07,  //  0056  EQ	R6	R6	R7
      0x781A0000,  //  0057  JMPF	R6	#0059
      0xB006391D,  //  0058  RAISE	1	K28	K29
      0x80000000,  //  0059  RET	0
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
      0x8804011A,  //  0000  GETMBR	R1	R0	K26
      0x80040200,  //  0001  RET	1	R1
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
      0x90023401,  //  0000  SETMBR	R0	K26	R1
      0x80000000,  //  0001  RET	0
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
      0x90021802,  //  0003  SETMBR	R0	K12	R2
      0x80000000,  //  0004  RET	0
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
      0x8C080106,  //  0000  GETMET	R2	R0	K6
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
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x540E0004,  //  0001  LDINT	R3	5
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
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
      0x88080105,  //  0003  GETMBR	R2	R0	K5
      0x4C140000,  //  0004  LDNIL	R5
      0x20140605,  //  0005  NE	R5	R3	R5
      0x7816000C,  //  0006  JMPF	R5	#0014
      0x4C140000,  //  0007  LDNIL	R5
      0x20140805,  //  0008  NE	R5	R4	R5
      0x78160009,  //  0009  JMPF	R5	#0014
      0x8C140106,  //  000A  GETMET	R5	R0	K6
      0x541E0008,  //  000B  LDINT	R7	9
      0x8C20011E,  //  000C  GETMET	R8	R0	K30
      0x5C280200,  //  000D  MOVE	R10	R1
      0x5C2C0400,  //  000E  MOVE	R11	R2
      0x7C200600,  //  000F  CALL	R8	3
      0x5C240600,  //  0010  MOVE	R9	R3
      0x5C280800,  //  0011  MOVE	R10	R4
      0x7C140A00,  //  0012  CALL	R5	5
      0x70020006,  //  0013  JMP		#001B
      0x8C140106,  //  0014  GETMET	R5	R0	K6
      0x541E0008,  //  0015  LDINT	R7	9
      0x8C20011E,  //  0016  GETMET	R8	R0	K30
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
      0x880C0105,  //  0003  GETMBR	R3	R0	K5
      0x8C100106,  //  0004  GETMET	R4	R0	K6
      0x541A0009,  //  0005  LDINT	R6	10
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x8C20011E,  //  0007  GETMET	R8	R0	K30
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
      0x8C040106,  //  0000  GETMET	R1	R0	K6
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
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x80040200,  //  0001  RET	1	R1
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
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x80040200,  //  0001  RET	1	R1
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
      0x8C040106,  //  0000  GETMET	R1	R0	K6
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
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x580C001F,  //  0001  LDCONST	R3	K31
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
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
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x540E0006,  //  0001  LDINT	R3	7
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
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
      0x8C140106,  //  0003  GETMET	R5	R0	K6
      0x581C0001,  //  0004  LDCONST	R7	K1
      0x7C140400,  //  0005  CALL	R5	2
      0x7002000A,  //  0006  JMP		#0012
      0x4C140000,  //  0007  LDNIL	R5
      0x1C140605,  //  0008  EQ	R5	R3	R5
      0x78160000,  //  0009  JMPF	R5	#000B
      0x880C0120,  //  000A  GETMBR	R3	R0	K32
      0x8C140106,  //  000B  GETMET	R5	R0	K6
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
      0x8C080106,  //  0000  GETMET	R2	R0	K6
      0x54120005,  //  0001  LDINT	R4	6
      0x7C080400,  //  0002  CALL	R2	2
      0x8C0C0121,  //  0003  GETMET	R3	R0	K33
      0x7C0C0200,  //  0004  CALL	R3	1
      0x8C100111,  //  0005  GETMET	R4	R0	K17
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
      0x8C100322,  //  0016  GETMET	R4	R1	K34
      0x5C180400,  //  0017  MOVE	R6	R2
      0x7C100400,  //  0018  CALL	R4	2
      0x80040200,  //  0019  RET	1	R1
      0x80000000,  //  001A  RET	0
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
    be_nested_map(27,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(leds, 8), be_const_var(1) },
        { be_const_key(create_segment, 25), be_const_closure(class_Leds_create_segment_closure) },
        { be_const_key(clear, -1), be_const_closure(class_Leds_clear_closure) },
        { be_const_key(begin, -1), be_const_closure(class_Leds_begin_closure) },
        { be_const_key(ctor, 9), be_const_closure(class_Leds_ctor_closure) },
        { be_const_key(pixel_size, -1), be_const_closure(class_Leds_pixel_size_closure) },
        { be_const_key(to_gamma, -1), be_const_closure(class_Leds_to_gamma_closure) },
        { be_const_key(get_animate, -1), be_const_closure(class_Leds_get_animate_closure) },
        { be_const_key(set_animate, 7), be_const_closure(class_Leds_set_animate_closure) },
        { be_const_key(dirty, -1), be_const_closure(class_Leds_dirty_closure) },
        { be_const_key(set_gamma, 4), be_const_closure(class_Leds_set_gamma_closure) },
        { be_const_key(get_pixel_color, -1), be_const_closure(class_Leds_get_pixel_color_closure) },
        { be_const_key(pixel_offset, 2), be_const_closure(class_Leds_pixel_offset_closure) },
        { be_const_key(pixel_count, -1), be_const_closure(class_Leds_pixel_count_closure) },
        { be_const_key(set_bri, 12), be_const_closure(class_Leds_set_bri_closure) },
        { be_const_key(clear_to, -1), be_const_closure(class_Leds_clear_to_closure) },
        { be_const_key(set_pixel_color, -1), be_const_closure(class_Leds_set_pixel_color_closure) },
        { be_const_key(animate, -1), be_const_var(3) },
        { be_const_key(gamma, 13), be_const_var(0) },
        { be_const_key(get_bri, -1), be_const_closure(class_Leds_get_bri_closure) },
        { be_const_key(get_gamma, -1), be_const_closure(class_Leds_get_gamma_closure) },
        { be_const_key(bri, -1), be_const_var(2) },
        { be_const_key(is_dirty, -1), be_const_closure(class_Leds_is_dirty_closure) },
        { be_const_key(can_show, -1), be_const_closure(class_Leds_can_show_closure) },
        { be_const_key(init, 5), be_const_closure(class_Leds_init_closure) },
        { be_const_key(show, -1), be_const_closure(class_Leds_show_closure) },
        { be_const_key(pixels_buffer, -1), be_const_closure(class_Leds_pixels_buffer_closure) },
    })),
    (bstring*) &be_const_str_Leds
);
/********************************************************************/
/* End of solidification */
