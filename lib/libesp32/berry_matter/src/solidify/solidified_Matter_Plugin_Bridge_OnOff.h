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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(web_values_prefix),
    /* K3   */  be_nested_str_weak(content_send),
    /* K4   */  be_nested_str_weak(format),
    /* K5   */  be_nested_str_weak(Relay_X20_X25i_X20_X25s),
    /* K6   */  be_nested_str_weak(tasmota_relay_index),
    /* K7   */  be_nested_str_weak(web_value_onoff),
    /* K8   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0102,  //  0002  GETMET	R3	R0	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0303,  //  0004  GETMET	R3	R1	K3
      0x8C140504,  //  0005  GETMET	R5	R2	K4
      0x581C0005,  //  0006  LDCONST	R7	K5
      0x88200106,  //  0007  GETMBR	R8	R0	K6
      0x8C240107,  //  0008  GETMET	R9	R0	K7
      0x882C0108,  //  0009  GETMBR	R11	R0	K8
      0x7C240400,  //  000A  CALL	R9	2
      0x7C140800,  //  000B  CALL	R5	4
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80000000,  //  000D  RET	0
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
