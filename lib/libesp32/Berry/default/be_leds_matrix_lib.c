/********************************************************************
 * Berry class `Leds_matrix`
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WS2812

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
    /* K0   */  be_nested_str_literal("alternate"),
    }),
    (be_nested_const_str("get_alternate", 1450148894, 13)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
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
    /* K0   */  be_nested_str_literal("alternate"),
    }),
    (be_nested_const_str("set_alternate", 1709680562, 13)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_literal("alternate"),
    /* K1   */  be_const_int(2),
    /* K2   */  be_nested_str_literal("set_pixel_color"),
    /* K3   */  be_nested_str_literal("w"),
    /* K4   */  be_nested_str_literal("h"),
    /* K5   */  be_const_int(1),
    }),
    (be_nested_const_str("set_matrix_pixel_color", 1197149462, 22)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[23]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x7816000C,  //  0001  JMPF	R5	#000F
      0x10140301,  //  0002  MOD	R5	R1	K1
      0x7816000A,  //  0003  JMPF	R5	#000F
      0x8C140102,  //  0004  GETMET	R5	R0	K2
      0x881C0103,  //  0005  GETMBR	R7	R0	K3
      0x081C0207,  //  0006  MUL	R7	R1	R7
      0x88200104,  //  0007  GETMBR	R8	R0	K4
      0x001C0E08,  //  0008  ADD	R7	R7	R8
      0x041C0E02,  //  0009  SUB	R7	R7	R2
      0x041C0F05,  //  000A  SUB	R7	R7	K5
      0x5C200600,  //  000B  MOVE	R8	R3
      0x5C240800,  //  000C  MOVE	R9	R4
      0x7C140800,  //  000D  CALL	R5	4
      0x70020006,  //  000E  JMP		#0016
      0x8C140102,  //  000F  GETMET	R5	R0	K2
      0x881C0103,  //  0010  GETMBR	R7	R0	K3
      0x081C0207,  //  0011  MUL	R7	R1	R7
      0x001C0E02,  //  0012  ADD	R7	R7	R2
      0x5C200600,  //  0013  MOVE	R8	R3
      0x5C240800,  //  0014  MOVE	R9	R4
      0x7C140800,  //  0015  CALL	R5	4
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Leds_matrix_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_literal("w"),
    /* K1   */  be_nested_str_literal("h"),
    /* K2   */  be_nested_str_literal("alternate"),
    /* K3   */  be_nested_str_literal("init"),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[13]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x50140000,  //  0002  LDBOOL	R5	0	0
      0x90020405,  //  0003  SETMBR	R0	K2	R5
      0x60140003,  //  0004  GETGBL	R5	G3
      0x5C180000,  //  0005  MOVE	R6	R0
      0x7C140200,  //  0006  CALL	R5	1
      0x8C140B03,  //  0007  GETMET	R5	R5	K3
      0x081C0202,  //  0008  MUL	R7	R1	R2
      0x5C200600,  //  0009  MOVE	R8	R3
      0x5C240800,  //  000A  MOVE	R9	R4
      0x7C140800,  //  000B  CALL	R5	4
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds_matrix
********************************************************************/
extern const bclass be_class_Leds;
be_local_class(Leds_matrix,
    3,
    &be_class_Leds,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, 3), be_const_closure(Leds_matrix_init_closure) },
        { be_nested_key("h", -317966505, 1, -1), be_const_var(0) },
        { be_nested_key("set_alternate", 1709680562, 13, 6), be_const_closure(Leds_matrix_set_alternate_closure) },
        { be_nested_key("set_matrix_pixel_color", 1197149462, 22, 4), be_const_closure(Leds_matrix_set_matrix_pixel_color_closure) },
        { be_nested_key("w", -234078410, 1, -1), be_const_var(1) },
        { be_nested_key("get_alternate", 1450148894, 13, 0), be_const_closure(Leds_matrix_get_alternate_closure) },
        { be_nested_key("alternate", 1140253277, 9, -1), be_const_var(2) },
    })),
    be_str_literal("Leds_matrix")
);
/*******************************************************************/

void be_load_Leds_matrix_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds_matrix);
    be_setglobal(vm, "Leds_matrix");
    be_pop(vm, 1);
}


#endif // USE_WS2812
