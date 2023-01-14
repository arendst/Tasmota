/* Solidification of zigbee_zb_coord.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_zb_coord;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(zb_coord_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_handler
********************************************************************/
be_local_closure(zb_coord_add_handler,   /* name */
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
    /* K0   */  be_nested_str_weak(instance),
    /* K1   */  be_nested_str_weak(value_error),
    /* K2   */  be_nested_str_weak(instance_X20required),
    /* K3   */  be_nested_str_weak(_handlers),
    /* K4   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_handler),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x20080500,  //  0003  NE	R2	R2	K0
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB0060302,  //  0005  RAISE	1	K1	K2
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x780A0004,  //  0007  JMPF	R2	#000D
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x8C080504,  //  0009  GETMET	R2	R2	K4
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x70020003,  //  000C  JMP		#0011
      0x60080012,  //  000D  GETGBL	R2	G18
      0x7C080000,  //  000E  CALL	R2	0
      0x400C0401,  //  000F  CONNECT	R3	R2	R1
      0x90020602,  //  0010  SETMBR	R0	K3	R2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dispatch
********************************************************************/
be_local_closure(zb_coord_dispatch,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(_handlers),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(toptr),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(zcl_frame),
    /* K6   */  be_nested_str_weak(zcl_attribute_list),
    /* K7   */  be_nested_str_weak(get),
    /* K8   */  be_nested_str_weak(function),
    /* K9   */  be_nested_str_weak(format),
    /* K10  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K11  */  be_nested_str_weak(tasmota),
    /* K12  */  be_nested_str_weak(_debug_present),
    /* K13  */  be_nested_str_weak(debug),
    /* K14  */  be_nested_str_weak(traceback),
    /* K15  */  be_const_int(1),
    }),
    be_str_weak(dispatch),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x4C180000,  //  0001  LDNIL	R6
      0x1C140A06,  //  0002  EQ	R5	R5	R6
      0x78160000,  //  0003  JMPF	R5	#0005
      0x80000A00,  //  0004  RET	0
      0xA4160200,  //  0005  IMPORT	R5	K1
      0xA41A0400,  //  0006  IMPORT	R6	K2
      0x4C1C0000,  //  0007  LDNIL	R7
      0x4C200000,  //  0008  LDNIL	R8
      0x8C240B03,  //  0009  GETMET	R9	R5	K3
      0x582C0004,  //  000A  LDCONST	R11	K4
      0x7C240400,  //  000B  CALL	R9	2
      0x20280409,  //  000C  NE	R10	R2	R9
      0x782A0003,  //  000D  JMPF	R10	#0012
      0x8C280105,  //  000E  GETMET	R10	R0	K5
      0x5C300400,  //  000F  MOVE	R12	R2
      0x7C280400,  //  0010  CALL	R10	2
      0x5C1C1400,  //  0011  MOVE	R7	R10
      0x20280609,  //  0012  NE	R10	R3	R9
      0x782A0003,  //  0013  JMPF	R10	#0018
      0x8C280106,  //  0014  GETMET	R10	R0	K6
      0x5C300600,  //  0015  MOVE	R12	R3
      0x7C280400,  //  0016  CALL	R10	2
      0x5C201400,  //  0017  MOVE	R8	R10
      0x58280004,  //  0018  LDCONST	R10	K4
      0x602C000C,  //  0019  GETGBL	R11	G12
      0x88300100,  //  001A  GETMBR	R12	R0	K0
      0x7C2C0200,  //  001B  CALL	R11	1
      0x142C140B,  //  001C  LT	R11	R10	R11
      0x782E0027,  //  001D  JMPF	R11	#0046
      0x882C0100,  //  001E  GETMBR	R11	R0	K0
      0x942C160A,  //  001F  GETIDX	R11	R11	R10
      0x8C300B07,  //  0020  GETMET	R12	R5	K7
      0x5C381600,  //  0021  MOVE	R14	R11
      0x5C3C0200,  //  0022  MOVE	R15	R1
      0x7C300600,  //  0023  CALL	R12	3
      0x60340004,  //  0024  GETGBL	R13	G4
      0x5C381800,  //  0025  MOVE	R14	R12
      0x7C340200,  //  0026  CALL	R13	1
      0x1C341B08,  //  0027  EQ	R13	R13	K8
      0x7836001A,  //  0028  JMPF	R13	#0044
      0xA8020008,  //  0029  EXBLK	0	#0033
      0x5C341800,  //  002A  MOVE	R13	R12
      0x5C381600,  //  002B  MOVE	R14	R11
      0x5C3C0200,  //  002C  MOVE	R15	R1
      0x5C400E00,  //  002D  MOVE	R16	R7
      0x5C441000,  //  002E  MOVE	R17	R8
      0x5C480800,  //  002F  MOVE	R18	R4
      0x7C340A00,  //  0030  CALL	R13	5
      0xA8040001,  //  0031  EXBLK	1	1
      0x70020010,  //  0032  JMP		#0044
      0xAC340002,  //  0033  CATCH	R13	0	2
      0x7002000D,  //  0034  JMP		#0043
      0x603C0001,  //  0035  GETGBL	R15	G1
      0x8C400D09,  //  0036  GETMET	R16	R6	K9
      0x5848000A,  //  0037  LDCONST	R18	K10
      0x5C4C1A00,  //  0038  MOVE	R19	R13
      0x5C501C00,  //  0039  MOVE	R20	R14
      0x7C400800,  //  003A  CALL	R16	4
      0x7C3C0200,  //  003B  CALL	R15	1
      0xB83E1600,  //  003C  GETNGBL	R15	K11
      0x883C1F0C,  //  003D  GETMBR	R15	R15	K12
      0x783E0002,  //  003E  JMPF	R15	#0042
      0xA43E1A00,  //  003F  IMPORT	R15	K13
      0x8C401F0E,  //  0040  GETMET	R16	R15	K14
      0x7C400200,  //  0041  CALL	R16	1
      0x70020000,  //  0042  JMP		#0044
      0xB0080000,  //  0043  RAISE	2	R0	R0
      0x0028150F,  //  0044  ADD	R10	R10	K15
      0x7001FFD2,  //  0045  JMP		#0019
      0x80000000,  //  0046  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: zb_coord
********************************************************************/
extern const bclass be_class_zb_coord_ntv;
be_local_class(zb_coord,
    1,
    &be_class_zb_coord_ntv,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(dispatch, 1), be_const_closure(zb_coord_dispatch_closure) },
        { be_const_key_weak(add_handler, 2), be_const_closure(zb_coord_add_handler_closure) },
        { be_const_key_weak(_handlers, -1), be_const_var(0) },
        { be_const_key_weak(init, 0), be_const_closure(zb_coord_init_closure) },
    })),
    be_str_weak(zb_coord)
);
/*******************************************************************/

void be_load_zb_coord_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_zb_coord);
    be_setglobal(vm, "zb_coord");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
