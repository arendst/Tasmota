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
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(payload),
    /* K1   */  be_nested_str_weak(_get_bytes),
    /* K2   */  be_nested_str_weak(payload_ptr),
    /* K3   */  be_nested_str_weak(member),
    }),
    be_str_weak(member),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x1C080300,  //  0000  EQ	R2	R1	K0
      0x780A0004,  //  0001  JMPF	R2	#0007
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x7C080400,  //  0004  CALL	R2	2
      0x80040400,  //  0005  RET	1	R2
      0x70020006,  //  0006  JMP		#000E
      0x60080003,  //  0007  GETGBL	R2	G3
      0x5C0C0000,  //  0008  MOVE	R3	R0
      0x7C080200,  //  0009  CALL	R2	1
      0x8C080503,  //  000A  GETMET	R2	R2	K3
      0x5C100200,  //  000B  MOVE	R4	R1
      0x7C080400,  //  000C  CALL	R2	2
      0x80040400,  //  000D  RET	1	R2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(zcl_frame_setmember,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(payload),
    /* K1   */  be_nested_str_weak(_set_bytes),
    /* K2   */  be_nested_str_weak(payload_ptr),
    /* K3   */  be_nested_str_weak(setmember),
    }),
    be_str_weak(setmember),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0005,  //  0001  JMPF	R3	#0008
      0x8C0C0101,  //  0002  GETMET	R3	R0	K1
      0x88140102,  //  0003  GETMBR	R5	R0	K2
      0x5C180400,  //  0004  MOVE	R6	R2
      0x7C0C0600,  //  0005  CALL	R3	3
      0x80040600,  //  0006  RET	1	R3
      0x70020007,  //  0007  JMP		#0010
      0x600C0003,  //  0008  GETGBL	R3	G3
      0x5C100000,  //  0009  MOVE	R4	R0
      0x7C0C0200,  //  000A  CALL	R3	1
      0x8C0C0703,  //  000B  GETMET	R3	R3	K3
      0x5C140200,  //  000C  MOVE	R5	R1
      0x5C180400,  //  000D  MOVE	R6	R2
      0x7C0C0600,  //  000E  CALL	R3	3
      0x80040600,  //  000F  RET	1	R3
      0x80000000,  //  0010  RET	0
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
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(no_bytes),
    }),
    be_str_weak(init),
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
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tomap),
    /* K2   */  be_nested_str_weak(shortaddr_hex),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(0x_X2504X),
    /* K5   */  be_nested_str_weak(shortaddr),
    /* K6   */  be_nested_str_weak(payload),
    }),
    be_str_weak(tomap),
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
    /* K0   */  be_nested_str_weak(tomap),
    }),
    be_str_weak(tostring),
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
        { be_const_key_weak(no_bytes, -1), be_const_var(0) },
        { be_const_key_weak(member, 2), be_const_closure(zcl_frame_member_closure) },
        { be_const_key_weak(tostring, 5), be_const_closure(zcl_frame_tostring_closure) },
        { be_const_key_weak(init, -1), be_const_closure(zcl_frame_init_closure) },
        { be_const_key_weak(tomap, -1), be_const_closure(zcl_frame_tomap_closure) },
        { be_const_key_weak(setmember, -1), be_const_closure(zcl_frame_setmember_closure) },
    })),
    be_str_weak(zcl_frame)
);
/*******************************************************************/

void be_load_zcl_frame_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_zcl_frame);
    be_setglobal(vm, "zcl_frame");
    be_pop(vm, 1);
}


#endif // USE_LIGHT
