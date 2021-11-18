/********************************************************************
 * Berry class `Leds_matrix`
 * 
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_WS2812

/********************************************************************
** Solidified function: set_matrix_pixel_color
********************************************************************/
be_local_closure(set_matrix_pixel_color,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_string("set_pixel_color", 1275248356, 15),
    /* K1   */  be_nested_string("w", -234078410, 1),
    }),
    (be_nested_const_str("set_matrix_pixel_color", 1197149462, 22)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 8]) {  /* code */
      0x8C140100,  //  0000  GETMET	R5	R0	K0
      0x881C0101,  //  0001  GETMBR	R7	R0	K1
      0x081C0207,  //  0002  MUL	R7	R1	R7
      0x001C0E02,  //  0003  ADD	R7	R7	R2
      0x5C200600,  //  0004  MOVE	R8	R3
      0x5C240800,  //  0005  MOVE	R9	R4
      0x7C140800,  //  0006  CALL	R5	4
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("w", -234078410, 1),
    /* K1   */  be_nested_string("h", -317966505, 1),
    /* K2   */  be_nested_string("init", 380752755, 4),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[11]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x60140003,  //  0002  GETGBL	R5	G3
      0x5C180000,  //  0003  MOVE	R6	R0
      0x7C140200,  //  0004  CALL	R5	1
      0x8C140B02,  //  0005  GETMET	R5	R5	K2
      0x081C0202,  //  0006  MUL	R7	R1	R2
      0x5C200600,  //  0007  MOVE	R8	R3
      0x5C240800,  //  0008  MOVE	R9	R4
      0x7C140800,  //  0009  CALL	R5	4
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Leds_matrix
********************************************************************/
extern const bclass be_class_Leds;
be_local_class(Leds_matrix,
    2,
    &be_class_Leds,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("h", -317966505, 1, -1), be_const_var(0) },
        { be_nested_key("w", -234078410, 1, -1), be_const_var(1) },
        { be_nested_key("set_matrix_pixel_color", 1197149462, 22, 1), be_const_closure(set_matrix_pixel_color_closure) },
        { be_nested_key("init", 380752755, 4, 0), be_const_closure(init_closure) },
    })),
    (be_nested_const_str("Leds_matrix", -1450073227, 11))
);
/*******************************************************************/

void be_load_Leds_matrix_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Leds_matrix);
    be_setglobal(vm, "Leds_matrix");
    be_pop(vm, 1);
}


#endif // USE_WS2812
