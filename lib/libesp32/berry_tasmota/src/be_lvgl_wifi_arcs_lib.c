/********************************************************************
 * Tasmota LVGL lv_signal_bars widget
 *******************************************************************/
#include "be_constobj.h"

#ifdef USE_LVGL

#include "lvgl.h"

/********************************************************************
** Solidified function: before_del
********************************************************************/
be_local_closure(lv_wifi_arcs_before_del,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(remove_driver),
    }),
    &be_const_str_before_del,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(lv_wifi_arcs_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(init),
    /* K1   */  be_nested_str(tasmota),
    /* K2   */  be_nested_str(add_driver),
    /* K3   */  be_nested_str(set_percentage),
    /* K4   */  be_const_int(0),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0200,  //  0006  GETNGBL	R2	K1
      0x8C080502,  //  0007  GETMET	R2	R2	K2
      0x5C100000,  //  0008  MOVE	R4	R0
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080103,  //  000A  GETMET	R2	R0	K3
      0x58100004,  //  000B  LDCONST	R4	K4
      0x7C080400,  //  000C  CALL	R2	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(lv_wifi_arcs_every_second,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(wifi),
    /* K2   */  be_nested_str(find),
    /* K3   */  be_nested_str(quality),
    /* K4   */  be_nested_str(ip),
    /* K5   */  be_nested_str(set_percentage),
    /* K6   */  be_const_int(0),
    }),
    &be_const_str_every_second,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x58100003,  //  0004  LDCONST	R4	K3
      0x7C080400,  //  0005  CALL	R2	2
      0x8C0C0302,  //  0006  GETMET	R3	R1	K2
      0x58140004,  //  0007  LDCONST	R5	K4
      0x7C0C0400,  //  0008  CALL	R3	2
      0x4C100000,  //  0009  LDNIL	R4
      0x1C100604,  //  000A  EQ	R4	R3	R4
      0x78120003,  //  000B  JMPF	R4	#0010
      0x8C100105,  //  000C  GETMET	R4	R0	K5
      0x58180006,  //  000D  LDCONST	R6	K6
      0x7C100400,  //  000E  CALL	R4	2
      0x70020005,  //  000F  JMP		#0016
      0x4C100000,  //  0010  LDNIL	R4
      0x20100404,  //  0011  NE	R4	R2	R4
      0x78120002,  //  0012  JMPF	R4	#0016
      0x8C100105,  //  0013  GETMET	R4	R0	K5
      0x5C180400,  //  0014  MOVE	R6	R2
      0x7C100400,  //  0015  CALL	R4	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: lv_wifi_arcs
********************************************************************/
extern const bclass be_class_lv_signal_arcs;
be_local_class(lv_wifi_arcs,
    0,
    &be_class_lv_signal_arcs,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(before_del, 1), be_const_closure(lv_wifi_arcs_before_del_closure) },
        { be_const_key(every_second, 2), be_const_closure(lv_wifi_arcs_every_second_closure) },
        { be_const_key(init, -1), be_const_closure(lv_wifi_arcs_init_closure) },
    })),
    be_str_literal("lv_wifi_arcs")
);
/*******************************************************************/

void be_load_lv_wifi_arcs_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_lv_wifi_arcs);
    be_setglobal(vm, "lv_wifi_arcs");
    be_pop(vm, 1);
}

#endif // USE_LVGL