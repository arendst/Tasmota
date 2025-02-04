/* Solidification of Matter_Plugin_8_Bridge_Light3.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Light3;

/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(class_Matter_Plugin_Bridge_Light3__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(_X3Clambda_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040009,  //  0000  GETGBL	R1	G9
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Light3
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_class(Matter_Plugin_Bridge_Light3,
    0,
    &be_class_Matter_Plugin_Light3,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(3000) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_light3) },
        { be_const_key_weak(BRIDGE, -1), be_const_bool(1) },
        { be_const_key_weak(ARG_HINT, 5), be_nested_str_weak(Relay_X3Cx_X3E_X20number) },
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(class_Matter_Plugin_Bridge_Light3__X3Clambda_X3E_closure) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(relay) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Light3)
);
/********************************************************************/
/* End of solidification */
