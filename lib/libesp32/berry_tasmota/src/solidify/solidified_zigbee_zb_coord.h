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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(instance),
    /* K1   */  be_nested_str_weak(value_error),
    /* K2   */  be_nested_str_weak(instance_X20required),
    /* K3   */  be_nested_str_weak(_handlers),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(push),
    }),
    be_str_weak(add_handler),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x20080500,  //  0003  NE	R2	R2	K0
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB0060302,  //  0005  RAISE	1	K1	K2
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x780A000B,  //  0007  JMPF	R2	#0014
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x8C080504,  //  0009  GETMET	R2	R2	K4
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x4C0C0000,  //  000C  LDNIL	R3
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A0003,  //  000E  JMPF	R2	#0013
      0x88080103,  //  000F  GETMBR	R2	R0	K3
      0x8C080505,  //  0010  GETMET	R2	R2	K5
      0x5C100200,  //  0011  MOVE	R4	R1
      0x7C080400,  //  0012  CALL	R2	2
      0x70020003,  //  0013  JMP		#0018
      0x60080012,  //  0014  GETGBL	R2	G18
      0x7C080000,  //  0015  CALL	R2	0
      0x400C0401,  //  0016  CONNECT	R3	R2	R1
      0x90020602,  //  0017  SETMBR	R0	K3	R2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dispatch
********************************************************************/
be_local_closure(zb_coord_dispatch,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(_handlers),
    /* K1   */  be_nested_str_weak(introspect),
    /* K2   */  be_nested_str_weak(toptr),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(zcl_frame),
    /* K5   */  be_nested_str_weak(zcl_attribute_list),
    /* K6   */  be_nested_str_weak(get),
    /* K7   */  be_nested_str_weak(function),
    /* K8   */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(_debug_present),
    /* K11  */  be_nested_str_weak(debug),
    /* K12  */  be_nested_str_weak(traceback),
    /* K13  */  be_const_int(1),
    }),
    be_str_weak(dispatch),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x4C180000,  //  0001  LDNIL	R6
      0x1C140A06,  //  0002  EQ	R5	R5	R6
      0x78160000,  //  0003  JMPF	R5	#0005
      0x80000A00,  //  0004  RET	0
      0xA4160200,  //  0005  IMPORT	R5	K1
      0x4C180000,  //  0006  LDNIL	R6
      0x4C1C0000,  //  0007  LDNIL	R7
      0x8C200B02,  //  0008  GETMET	R8	R5	K2
      0x58280003,  //  0009  LDCONST	R10	K3
      0x7C200400,  //  000A  CALL	R8	2
      0x20240408,  //  000B  NE	R9	R2	R8
      0x78260003,  //  000C  JMPF	R9	#0011
      0x8C240104,  //  000D  GETMET	R9	R0	K4
      0x5C2C0400,  //  000E  MOVE	R11	R2
      0x7C240400,  //  000F  CALL	R9	2
      0x5C181200,  //  0010  MOVE	R6	R9
      0x20240608,  //  0011  NE	R9	R3	R8
      0x78260003,  //  0012  JMPF	R9	#0017
      0x8C240105,  //  0013  GETMET	R9	R0	K5
      0x5C2C0600,  //  0014  MOVE	R11	R3
      0x7C240400,  //  0015  CALL	R9	2
      0x5C1C1200,  //  0016  MOVE	R7	R9
      0x58240003,  //  0017  LDCONST	R9	K3
      0x6028000C,  //  0018  GETGBL	R10	G12
      0x882C0100,  //  0019  GETMBR	R11	R0	K0
      0x7C280200,  //  001A  CALL	R10	1
      0x1428120A,  //  001B  LT	R10	R9	R10
      0x782A0027,  //  001C  JMPF	R10	#0045
      0x88280100,  //  001D  GETMBR	R10	R0	K0
      0x94281409,  //  001E  GETIDX	R10	R10	R9
      0x8C2C0B06,  //  001F  GETMET	R11	R5	K6
      0x5C341400,  //  0020  MOVE	R13	R10
      0x5C380200,  //  0021  MOVE	R14	R1
      0x7C2C0600,  //  0022  CALL	R11	3
      0x60300004,  //  0023  GETGBL	R12	G4
      0x5C341600,  //  0024  MOVE	R13	R11
      0x7C300200,  //  0025  CALL	R12	1
      0x1C301907,  //  0026  EQ	R12	R12	K7
      0x7832001A,  //  0027  JMPF	R12	#0043
      0xA8020008,  //  0028  EXBLK	0	#0032
      0x5C301600,  //  0029  MOVE	R12	R11
      0x5C341400,  //  002A  MOVE	R13	R10
      0x5C380200,  //  002B  MOVE	R14	R1
      0x5C3C0C00,  //  002C  MOVE	R15	R6
      0x5C400E00,  //  002D  MOVE	R16	R7
      0x5C440800,  //  002E  MOVE	R17	R4
      0x7C300A00,  //  002F  CALL	R12	5
      0xA8040001,  //  0030  EXBLK	1	1
      0x70020010,  //  0031  JMP		#0043
      0xAC300002,  //  0032  CATCH	R12	0	2
      0x7002000D,  //  0033  JMP		#0042
      0x60380001,  //  0034  GETGBL	R14	G1
      0x603C0018,  //  0035  GETGBL	R15	G24
      0x58400008,  //  0036  LDCONST	R16	K8
      0x5C441800,  //  0037  MOVE	R17	R12
      0x5C481A00,  //  0038  MOVE	R18	R13
      0x7C3C0600,  //  0039  CALL	R15	3
      0x7C380200,  //  003A  CALL	R14	1
      0xB83A1200,  //  003B  GETNGBL	R14	K9
      0x88381D0A,  //  003C  GETMBR	R14	R14	K10
      0x783A0002,  //  003D  JMPF	R14	#0041
      0xA43A1600,  //  003E  IMPORT	R14	K11
      0x8C3C1D0C,  //  003F  GETMET	R15	R14	K12
      0x7C3C0200,  //  0040  CALL	R15	1
      0x70020000,  //  0041  JMP		#0043
      0xB0080000,  //  0042  RAISE	2	R0	R0
      0x0024130D,  //  0043  ADD	R9	R9	K13
      0x7001FFD2,  //  0044  JMP		#0018
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_handler
********************************************************************/
be_local_closure(zb_coord_remove_handler,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_handlers),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(remove),
    }),
    be_str_weak(remove_handler),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A000A,  //  0001  JMPF	R2	#000D
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x8C080501,  //  0003  GETMET	R2	R2	K1
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x200C0403,  //  0007  NE	R3	R2	R3
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C0100,  //  0009  GETMBR	R3	R0	K0
      0x8C0C0702,  //  000A  GETMET	R3	R3	K2
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80000000,  //  000D  RET	0
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
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(zb_coord_init_closure) },
        { be_const_key_weak(remove_handler, 2), be_const_closure(zb_coord_remove_handler_closure) },
        { be_const_key_weak(add_handler, -1), be_const_closure(zb_coord_add_handler_closure) },
        { be_const_key_weak(dispatch, -1), be_const_closure(zb_coord_dispatch_closure) },
        { be_const_key_weak(_handlers, 1), be_const_var(0) },
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
