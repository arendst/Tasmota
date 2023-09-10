/* Solidification of Matter_Plugin_9_Virt_OnOff.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Virt_OnOff;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Virt_OnOff_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(virtual),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x50100200,  //  0008  LDBOOL	R4	1	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Virt_OnOff
********************************************************************/
extern const bclass be_class_Matter_Plugin_OnOff;
be_local_class(Matter_Plugin_Virt_OnOff,
    0,
    &be_class_Matter_Plugin_OnOff,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG_HINT, 3), be_nested_str_weak(_Not_X20used_) },
        { be_const_key_weak(NAME, 2), be_nested_str_weak(_X28v_X29_X20Relay) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(v_relay) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Virt_OnOff_init_closure) },
        { be_const_key_weak(ARG, 1), be_nested_str_weak() },
    })),
    be_str_weak(Matter_Plugin_Virt_OnOff)
);
/*******************************************************************/

void be_load_Matter_Plugin_Virt_OnOff_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Virt_OnOff);
    be_setglobal(vm, "Matter_Plugin_Virt_OnOff");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
