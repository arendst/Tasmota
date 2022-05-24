/********************************************************************
 * Zigbee
 * 
 *******************************************************************/
#ifdef USE_ZIGBEE

#include "be_constobj.h"

extern const bclass be_class_zcl_frame_ntv;


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(zcl_frame_member,   /* name */
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
    /* K0   */  be_nested_str_literal("payload"),
    /* K1   */  be_nested_str_literal("_get_bytes"),
    /* K2   */  be_nested_str_literal("member"),
    }),
    be_str_literal("member"),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x1C080300,  //  0000  EQ	R2	R1	K0
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x80040400,  //  0004  RET	1	R2
      0x70020006,  //  0005  JMP		#000D
      0x60080003,  //  0006  GETGBL	R2	G3
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x7C080200,  //  0008  CALL	R2	1
      0x8C080502,  //  0009  GETMET	R2	R2	K2
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x80040400,  //  000C  RET	1	R2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tomap
********************************************************************/
be_local_closure(zcl_frame_tomap,   /* name */
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
    /* K0   */  be_nested_str_literal("string"),
    /* K1   */  be_nested_str_literal("tomap"),
    /* K2   */  be_nested_str_literal("shortaddr_hex"),
    /* K3   */  be_nested_str_literal("format"),
    /* K4   */  be_nested_str_literal("0x%04X"),
    /* K5   */  be_nested_str_literal("shortaddr"),
    /* K6   */  be_nested_str_literal("payload"),
    }),
    be_str_literal("tomap"),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080003,  //  0001  GETGBL	R2	G3
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x8C0C0303,  //  0006  GETMET	R3	R1	K3
      0x58140004,  //  0007  LDCONST	R5	K4
      0x88180105,  //  0008  GETMBR	R6	R0	K5
      0x7C0C0600,  //  0009  CALL	R3	3
      0x980A0403,  //  000A  SETIDX	R2	K2	R3
      0x880C0106,  //  000B  GETMBR	R3	R0	K6
      0x980A0C03,  //  000C  SETIDX	R2	K6	R3
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(zcl_frame_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_literal("init"),
    /* K1   */  be_nested_str_literal("no_bytes"),
    }),
    be_str_literal("init"),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080015,  //  0006  GETGBL	R2	G21
      0x7C080000,  //  0007  CALL	R2	0
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _get_bytes
********************************************************************/
be_local_closure(zcl_frame__get_bytes,   /* name */
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
    /* K0   */  be_nested_str_literal("introspect"),
    /* K1   */  be_nested_str_literal("toptr"),
    /* K2   */  be_nested_str_literal("payload_ptr"),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_literal("get"),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_literal("no_bytes"),
    }),
    be_str_literal("_get_bytes"),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x600C0009,  //  0004  GETGBL	R3	G9
      0x5C100400,  //  0005  MOVE	R4	R2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x200C0703,  //  0007  NE	R3	R3	K3
      0x780E0013,  //  0008  JMPF	R3	#001D
      0x600C0015,  //  0009  GETGBL	R3	G21
      0x5C100400,  //  000A  MOVE	R4	R2
      0x5415FFFB,  //  000B  LDINT	R5	-4
      0x7C0C0400,  //  000C  CALL	R3	2
      0x8C100704,  //  000D  GETMET	R4	R3	K4
      0x58180005,  //  000E  LDCONST	R6	K5
      0x581C0005,  //  000F  LDCONST	R7	K5
      0x7C100600,  //  0010  CALL	R4	3
      0x60140015,  //  0011  GETGBL	R5	G21
      0x8C180301,  //  0012  GETMET	R6	R1	K1
      0x60200009,  //  0013  GETGBL	R8	G9
      0x5C240400,  //  0014  MOVE	R9	R2
      0x7C200200,  //  0015  CALL	R8	1
      0x54260003,  //  0016  LDINT	R9	4
      0x00201009,  //  0017  ADD	R8	R8	R9
      0x7C180400,  //  0018  CALL	R6	2
      0x5C1C0800,  //  0019  MOVE	R7	R4
      0x7C140400,  //  001A  CALL	R5	2
      0x80040A00,  //  001B  RET	1	R5
      0x70020001,  //  001C  JMP		#001F
      0x880C0106,  //  001D  GETMBR	R3	R0	K6
      0x80040600,  //  001E  RET	1	R3
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(zcl_frame_tostring,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_literal("tomap"),
    }),
    be_str_literal("tostring"),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x60080008,  //  0002  GETGBL	R2	G8
      0x5C0C0200,  //  0003  MOVE	R3	R1
      0x7C080200,  //  0004  CALL	R2	1
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: zcl_frame
********************************************************************/
extern const bclass be_class_zcl_frame_ntv;
be_local_class(zcl_frame,
    1,
    &be_class_zcl_frame_ntv,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_literal("no_bytes", -1), be_const_var(0) },
        { be_const_key_literal("member", 2), be_const_closure(zcl_frame_member_closure) },
        { be_const_key_literal("tostring", -1), be_const_closure(zcl_frame_tostring_closure) },
        { be_const_key_literal("init", -1), be_const_closure(zcl_frame_init_closure) },
        { be_const_key_literal("tomap", 5), be_const_closure(zcl_frame_tomap_closure) },
        { be_const_key_literal("_get_bytes", -1), be_const_closure(zcl_frame__get_bytes_closure) },
    })),
    be_str_literal("zcl_frame")
);
/*******************************************************************/

void be_load_zcl_frame_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_zcl_frame);
    be_setglobal(vm, "zcl_frame");
    be_pop(vm, 1);
}


#endif // USE_LIGHT
