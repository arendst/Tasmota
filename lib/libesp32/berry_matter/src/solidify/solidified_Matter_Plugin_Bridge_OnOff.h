/* Solidification of Matter_Plugin_Bridge_OnOff.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_OnOff;

/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_OnOff_web_values,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X7C_X20Relay_X20_X25i_X20_X25s),
    /* K5   */  be_nested_str_weak(tasmota_relay_index),
    /* K6   */  be_nested_str_weak(web_value_onoff),
    /* K7   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x8C240106,  //  0006  GETMET	R9	R0	K6
      0x882C0107,  //  0007  GETMBR	R11	R0	K7
      0x7C240400,  //  0008  CALL	R9	2
      0x7C140800,  //  0009  CALL	R5	4
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_OnOff
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Light0;
be_local_class(Matter_Plugin_Bridge_OnOff,
    0,
    &be_class_Matter_Plugin_Bridge_Light0,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(266, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Relay) },
        { be_const_key_weak(web_values, -1), be_const_closure(Matter_Plugin_Bridge_OnOff_web_values_closure) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Enter_X20Relay_X3Cx_X3E_X20number) },
        { be_const_key_weak(TYPE, 1), be_nested_str_weak(http_relay) },
    })),
    be_str_weak(Matter_Plugin_Bridge_OnOff)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_OnOff_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_OnOff);
    be_setglobal(vm, "Matter_Plugin_Bridge_OnOff");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
