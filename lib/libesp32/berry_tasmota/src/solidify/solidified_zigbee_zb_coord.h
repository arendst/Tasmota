/* Solidification of zigbee_zb_coord.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'zb_coord' ktab size: 21, total: 24 (saved 24 bytes)
static const bvalue be_ktab_class_zb_coord[21] = {
  /* K0   */  be_nested_str_weak(init),
  /* K1   */  be_nested_str_weak(instance),
  /* K2   */  be_nested_str_weak(value_error),
  /* K3   */  be_nested_str_weak(instance_X20required),
  /* K4   */  be_nested_str_weak(_handlers),
  /* K5   */  be_nested_str_weak(find),
  /* K6   */  be_nested_str_weak(push),
  /* K7   */  be_nested_str_weak(introspect),
  /* K8   */  be_nested_str_weak(toptr),
  /* K9   */  be_const_int(0),
  /* K10  */  be_nested_str_weak(zcl_frame),
  /* K11  */  be_nested_str_weak(zcl_attribute_list),
  /* K12  */  be_nested_str_weak(get),
  /* K13  */  be_nested_str_weak(function),
  /* K14  */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
  /* K15  */  be_nested_str_weak(tasmota),
  /* K16  */  be_nested_str_weak(_debug_present),
  /* K17  */  be_nested_str_weak(debug),
  /* K18  */  be_nested_str_weak(traceback),
  /* K19  */  be_const_int(1),
  /* K20  */  be_nested_str_weak(remove),
};


extern const bclass be_class_zb_coord;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_zb_coord_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zb_coord,     /* shared constants */
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
be_local_closure(class_zb_coord_add_handler,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zb_coord,     /* shared constants */
    be_str_weak(add_handler),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x20080501,  //  0003  NE	R2	R2	K1
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB0060503,  //  0005  RAISE	1	K2	K3
      0x88080104,  //  0006  GETMBR	R2	R0	K4
      0x780A000B,  //  0007  JMPF	R2	#0014
      0x88080104,  //  0008  GETMBR	R2	R0	K4
      0x8C080505,  //  0009  GETMET	R2	R2	K5
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x4C0C0000,  //  000C  LDNIL	R3
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A0003,  //  000E  JMPF	R2	#0013
      0x88080104,  //  000F  GETMBR	R2	R0	K4
      0x8C080506,  //  0010  GETMET	R2	R2	K6
      0x5C100200,  //  0011  MOVE	R4	R1
      0x7C080400,  //  0012  CALL	R2	2
      0x70020003,  //  0013  JMP		#0018
      0x60080012,  //  0014  GETGBL	R2	G18
      0x7C080000,  //  0015  CALL	R2	0
      0x400C0401,  //  0016  CONNECT	R3	R2	R1
      0x90020802,  //  0017  SETMBR	R0	K4	R2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dispatch
********************************************************************/
be_local_closure(class_zb_coord_dispatch,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zb_coord,     /* shared constants */
    be_str_weak(dispatch),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0x88140104,  //  0000  GETMBR	R5	R0	K4
      0x4C180000,  //  0001  LDNIL	R6
      0x1C140A06,  //  0002  EQ	R5	R5	R6
      0x78160000,  //  0003  JMPF	R5	#0005
      0x80000A00,  //  0004  RET	0
      0xA4160E00,  //  0005  IMPORT	R5	K7
      0x4C180000,  //  0006  LDNIL	R6
      0x4C1C0000,  //  0007  LDNIL	R7
      0x8C200B08,  //  0008  GETMET	R8	R5	K8
      0x58280009,  //  0009  LDCONST	R10	K9
      0x7C200400,  //  000A  CALL	R8	2
      0x20240408,  //  000B  NE	R9	R2	R8
      0x78260003,  //  000C  JMPF	R9	#0011
      0x8C24010A,  //  000D  GETMET	R9	R0	K10
      0x5C2C0400,  //  000E  MOVE	R11	R2
      0x7C240400,  //  000F  CALL	R9	2
      0x5C181200,  //  0010  MOVE	R6	R9
      0x20240608,  //  0011  NE	R9	R3	R8
      0x78260003,  //  0012  JMPF	R9	#0017
      0x8C24010B,  //  0013  GETMET	R9	R0	K11
      0x5C2C0600,  //  0014  MOVE	R11	R3
      0x7C240400,  //  0015  CALL	R9	2
      0x5C1C1200,  //  0016  MOVE	R7	R9
      0x58240009,  //  0017  LDCONST	R9	K9
      0x6028000C,  //  0018  GETGBL	R10	G12
      0x882C0104,  //  0019  GETMBR	R11	R0	K4
      0x7C280200,  //  001A  CALL	R10	1
      0x1428120A,  //  001B  LT	R10	R9	R10
      0x782A0027,  //  001C  JMPF	R10	#0045
      0x88280104,  //  001D  GETMBR	R10	R0	K4
      0x94281409,  //  001E  GETIDX	R10	R10	R9
      0x8C2C0B0C,  //  001F  GETMET	R11	R5	K12
      0x5C341400,  //  0020  MOVE	R13	R10
      0x5C380200,  //  0021  MOVE	R14	R1
      0x7C2C0600,  //  0022  CALL	R11	3
      0x60300004,  //  0023  GETGBL	R12	G4
      0x5C341600,  //  0024  MOVE	R13	R11
      0x7C300200,  //  0025  CALL	R12	1
      0x1C30190D,  //  0026  EQ	R12	R12	K13
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
      0x5840000E,  //  0036  LDCONST	R16	K14
      0x5C441800,  //  0037  MOVE	R17	R12
      0x5C481A00,  //  0038  MOVE	R18	R13
      0x7C3C0600,  //  0039  CALL	R15	3
      0x7C380200,  //  003A  CALL	R14	1
      0xB83A1E00,  //  003B  GETNGBL	R14	K15
      0x88381D10,  //  003C  GETMBR	R14	R14	K16
      0x783A0002,  //  003D  JMPF	R14	#0041
      0xA43A2200,  //  003E  IMPORT	R14	K17
      0x8C3C1D12,  //  003F  GETMET	R15	R14	K18
      0x7C3C0200,  //  0040  CALL	R15	1
      0x70020000,  //  0041  JMP		#0043
      0xB0080000,  //  0042  RAISE	2	R0	R0
      0x00241313,  //  0043  ADD	R9	R9	K19
      0x7001FFD2,  //  0044  JMP		#0018
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_handler
********************************************************************/
be_local_closure(class_zb_coord_remove_handler,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_zb_coord,     /* shared constants */
    be_str_weak(remove_handler),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080104,  //  0000  GETMBR	R2	R0	K4
      0x780A000A,  //  0001  JMPF	R2	#000D
      0x88080104,  //  0002  GETMBR	R2	R0	K4
      0x8C080505,  //  0003  GETMET	R2	R2	K5
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x200C0403,  //  0007  NE	R3	R2	R3
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C0104,  //  0009  GETMBR	R3	R0	K4
      0x8C0C0714,  //  000A  GETMET	R3	R3	K20
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
        { be_const_key_weak(init, -1), be_const_closure(class_zb_coord_init_closure) },
        { be_const_key_weak(remove_handler, 2), be_const_closure(class_zb_coord_remove_handler_closure) },
        { be_const_key_weak(add_handler, -1), be_const_closure(class_zb_coord_add_handler_closure) },
        { be_const_key_weak(dispatch, -1), be_const_closure(class_zb_coord_dispatch_closure) },
        { be_const_key_weak(_handlers, 1), be_const_var(0) },
    })),
    be_str_weak(zb_coord)
);
/********************************************************************/
/* End of solidification */
