/* Solidification of leds_frame_be_methods.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Leds_frame_be;

/********************************************************************
** Solidified function: setitem
********************************************************************/
be_local_closure(Leds_frame_be_setitem,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(set),
    }),
    &be_const_str_setitem,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x54160003,  //  0001  LDINT	R5	4
      0x08140205,  //  0002  MUL	R5	R1	R5
      0x5C180400,  //  0003  MOVE	R6	R2
      0x541E0003,  //  0004  LDINT	R7	4
      0x7C0C0800,  //  0005  CALL	R3	4
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pixel
********************************************************************/
be_local_closure(Leds_frame_be_set_pixel,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(setitem),
    }),
    &be_const_str_set_pixel,
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x4C180000,  //  0000  LDNIL	R6
      0x1C180A06,  //  0001  EQ	R6	R5	R6
      0x781A0000,  //  0002  JMPF	R6	#0004
      0x58140000,  //  0003  LDCONST	R5	K0
      0x541A00FE,  //  0004  LDINT	R6	255
      0x2C180A06,  //  0005  AND	R6	R5	R6
      0x541E0017,  //  0006  LDINT	R7	24
      0x38180C07,  //  0007  SHL	R6	R6	R7
      0x541E00FE,  //  0008  LDINT	R7	255
      0x2C1C0407,  //  0009  AND	R7	R2	R7
      0x5422000F,  //  000A  LDINT	R8	16
      0x381C0E08,  //  000B  SHL	R7	R7	R8
      0x30180C07,  //  000C  OR	R6	R6	R7
      0x541E00FE,  //  000D  LDINT	R7	255
      0x2C1C0607,  //  000E  AND	R7	R3	R7
      0x54220007,  //  000F  LDINT	R8	8
      0x381C0E08,  //  0010  SHL	R7	R7	R8
      0x30180C07,  //  0011  OR	R6	R6	R7
      0x541E00FE,  //  0012  LDINT	R7	255
      0x2C1C0807,  //  0013  AND	R7	R4	R7
      0x30180C07,  //  0014  OR	R6	R6	R7
      0x8C1C0101,  //  0015  GETMET	R7	R0	K1
      0x5C240200,  //  0016  MOVE	R9	R1
      0x5C280C00,  //  0017  MOVE	R10	R6
      0x7C1C0600,  //  0018  CALL	R7	3
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: item
********************************************************************/
be_local_closure(Leds_frame_be_item,   /* name */
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
    /* K0   */  be_nested_str(get),
    }),
    &be_const_str_item,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x54120003,  //  0001  LDINT	R4	4
      0x08100204,  //  0002  MUL	R4	R1	R4
      0x54160003,  //  0003  LDINT	R5	4
      0x7C080600,  //  0004  CALL	R2	3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Leds_frame_be_init,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(pixel_size),
    /* K2   */  be_nested_str(init),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x14080300,  //  0000  LT	R2	R1	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x44040200,  //  0002  NEG	R1	R1
      0x90020201,  //  0003  SETMBR	R0	K1	R1
      0x60080003,  //  0004  GETGBL	R2	G3
      0x5C0C0000,  //  0005  MOVE	R3	R0
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080502,  //  0007  GETMET	R2	R2	K2
      0x5411FFFB,  //  0008  LDINT	R4	-4
      0x08100204,  //  0009  MUL	R4	R1	R4
      0x7C080400,  //  000A  CALL	R2	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds_frame_be
********************************************************************/
be_local_class(Leds_frame_be,
    0,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(setitem, 1), be_const_closure(Leds_frame_be_setitem_closure) },
        { be_const_key(set_pixel, -1), be_const_closure(Leds_frame_be_set_pixel_closure) },
        { be_const_key(item, -1), be_const_closure(Leds_frame_be_item_closure) },
        { be_const_key(init, -1), be_const_closure(Leds_frame_be_init_closure) },
    })),
    (bstring*) &be_const_str_Leds_frame_be
);
/*******************************************************************/

void be_load_Leds_frame_be_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds_frame_be);
    be_setglobal(vm, "Leds_frame_be");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
