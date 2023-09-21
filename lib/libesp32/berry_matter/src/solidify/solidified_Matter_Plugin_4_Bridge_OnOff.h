/* Solidification of Matter_Plugin_4_Bridge_OnOff.h */
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
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(Relay_X20_X25i_X20_X25s),
    /* K4   */  be_nested_str_weak(tasmota_relay_index),
    /* K5   */  be_nested_str_weak(web_value_onoff),
    /* K6   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140003,  //  0005  LDCONST	R5	K3
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x8C1C0105,  //  0007  GETMET	R7	R0	K5
      0x88240106,  //  0008  GETMBR	R9	R0	K6
      0x7C1C0400,  //  0009  CALL	R7	2
      0x7C100600,  //  000A  CALL	R4	3
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
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
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Relay) },
        { be_const_key_weak(web_values, -1), be_const_closure(Matter_Plugin_Bridge_OnOff_web_values_closure) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Relay_X3Cx_X3E_X20number) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_relay) },
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
