/********************************************************************
 * Zigbee
 * 
 *******************************************************************/
#ifdef USE_ZIGBEE

#include "be_constobj.h"

extern const bclass be_class_zb_coord_ntv;


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
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(zcl_frame),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(get),
    /* K6   */  be_nested_str_weak(function),
    /* K7   */  be_nested_str_weak(format),
    /* K8   */  be_nested_str_weak(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(_debug_present),
    /* K11  */  be_nested_str_weak(debug),
    /* K12  */  be_nested_str_weak(traceback),
    /* K13  */  be_const_int(1),
    }),
    be_str_weak(dispatch),
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x4C180000,  //  0001  LDNIL	R6
      0x1C140A06,  //  0002  EQ	R5	R5	R6
      0x78160000,  //  0003  JMPF	R5	#0005
      0x80000A00,  //  0004  RET	0
      0xA4160200,  //  0005  IMPORT	R5	K1
      0xA41A0400,  //  0006  IMPORT	R6	K2
      0x5C1C0600,  //  0007  MOVE	R7	R3
      0x1C200503,  //  0008  EQ	R8	R2	K3
      0x78220003,  //  0009  JMPF	R8	#000E
      0x8C200103,  //  000A  GETMET	R8	R0	K3
      0x5C280600,  //  000B  MOVE	R10	R3
      0x7C200400,  //  000C  CALL	R8	2
      0x5C1C1000,  //  000D  MOVE	R7	R8
      0x58200004,  //  000E  LDCONST	R8	K4
      0x6024000C,  //  000F  GETGBL	R9	G12
      0x88280100,  //  0010  GETMBR	R10	R0	K0
      0x7C240200,  //  0011  CALL	R9	1
      0x14241009,  //  0012  LT	R9	R8	R9
      0x78260027,  //  0013  JMPF	R9	#003C
      0x88240100,  //  0014  GETMBR	R9	R0	K0
      0x94241208,  //  0015  GETIDX	R9	R9	R8
      0x8C280B05,  //  0016  GETMET	R10	R5	K5
      0x5C301200,  //  0017  MOVE	R12	R9
      0x5C340200,  //  0018  MOVE	R13	R1
      0x7C280600,  //  0019  CALL	R10	3
      0x602C0004,  //  001A  GETGBL	R11	G4
      0x5C301400,  //  001B  MOVE	R12	R10
      0x7C2C0200,  //  001C  CALL	R11	1
      0x1C2C1706,  //  001D  EQ	R11	R11	K6
      0x782E001A,  //  001E  JMPF	R11	#003A
      0xA8020008,  //  001F  EXBLK	0	#0029
      0x5C2C1400,  //  0020  MOVE	R11	R10
      0x5C301200,  //  0021  MOVE	R12	R9
      0x5C340200,  //  0022  MOVE	R13	R1
      0x5C380400,  //  0023  MOVE	R14	R2
      0x5C3C0E00,  //  0024  MOVE	R15	R7
      0x5C400800,  //  0025  MOVE	R16	R4
      0x7C2C0A00,  //  0026  CALL	R11	5
      0xA8040001,  //  0027  EXBLK	1	1
      0x70020010,  //  0028  JMP		#003A
      0xAC2C0002,  //  0029  CATCH	R11	0	2
      0x7002000D,  //  002A  JMP		#0039
      0x60340001,  //  002B  GETGBL	R13	G1
      0x8C380D07,  //  002C  GETMET	R14	R6	K7
      0x58400008,  //  002D  LDCONST	R16	K8
      0x5C441600,  //  002E  MOVE	R17	R11
      0x5C481800,  //  002F  MOVE	R18	R12
      0x7C380800,  //  0030  CALL	R14	4
      0x7C340200,  //  0031  CALL	R13	1
      0xB8361200,  //  0032  GETNGBL	R13	K9
      0x88341B0A,  //  0033  GETMBR	R13	R13	K10
      0x78360002,  //  0034  JMPF	R13	#0038
      0xA4361600,  //  0035  IMPORT	R13	K11
      0x8C381B0C,  //  0036  GETMET	R14	R13	K12
      0x7C380200,  //  0037  CALL	R14	1
      0x70020000,  //  0038  JMP		#003A
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x0020110D,  //  003A  ADD	R8	R8	K13
      0x7001FFD2,  //  003B  JMP		#000F
      0x80000000,  //  003C  RET	0
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


#endif // USE_LIGHT
