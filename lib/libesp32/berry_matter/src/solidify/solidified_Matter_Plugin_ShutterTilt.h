/* Solidification of Matter_Plugin_ShutterTilt.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_ShutterTilt;

/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_ShutterTilt_invoke_request,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(tilt_X25_X3A),
    /* K10  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220101,  //  0005  LDINT	R8	258
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220013,  //  0007  JMPF	R8	#001C
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x54220007,  //  000A  LDINT	R8	8
      0x1C200E08,  //  000B  EQ	R8	R7	R8
      0x7822000E,  //  000C  JMPF	R8	#001C
      0x8C200506,  //  000D  GETMET	R8	R2	K6
      0x58280007,  //  000E  LDCONST	R10	K7
      0x7C200400,  //  000F  CALL	R8	2
      0x4C240000,  //  0010  LDNIL	R9
      0x20241009,  //  0011  NE	R9	R8	R9
      0x78260006,  //  0012  JMPF	R9	#001A
      0x54260009,  //  0013  LDINT	R9	10
      0x0C201009,  //  0014  DIV	R8	R8	R9
      0x60240008,  //  0015  GETGBL	R9	G8
      0x5C281000,  //  0016  MOVE	R10	R8
      0x7C240200,  //  0017  CALL	R9	1
      0x00261209,  //  0018  ADD	R9	K9	R9
      0x900E1009,  //  0019  SETMBR	R3	K8	R9
      0x50240200,  //  001A  LDBOOL	R9	1	0
      0x80041200,  //  001B  RET	1	R9
      0x60200003,  //  001C  GETGBL	R8	G3
      0x5C240000,  //  001D  MOVE	R9	R0
      0x7C200200,  //  001E  CALL	R8	1
      0x8C20110A,  //  001F  GETMET	R8	R8	K10
      0x5C280200,  //  0020  MOVE	R10	R1
      0x5C2C0400,  //  0021  MOVE	R11	R2
      0x5C300600,  //  0022  MOVE	R12	R3
      0x7C200800,  //  0023  CALL	R8	4
      0x80041000,  //  0024  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_ShutterTilt_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(U2),
    /* K10  */  be_nested_str_weak(shadow_shutter_tilt),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(U4),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0101,  //  0005  LDINT	R7	258
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E002E,  //  0007  JMPF	R7	#0037
      0x8C1C0105,  //  0008  GETMET	R7	R0	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x541E0006,  //  000A  LDINT	R7	7
      0x1C1C0C07,  //  000B  EQ	R7	R6	R7
      0x781E0008,  //  000C  JMPF	R7	#0016
      0x8C1C0906,  //  000D  GETMET	R7	R4	K6
      0x88240907,  //  000E  GETMBR	R9	R4	K7
      0x542A0007,  //  000F  LDINT	R10	8
      0x002A100A,  //  0010  ADD	R10	K8	R10
      0x542E000F,  //  0011  LDINT	R11	16
      0x0028140B,  //  0012  ADD	R10	R10	R11
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x70020020,  //  0015  JMP		#0037
      0x541E000E,  //  0016  LDINT	R7	15
      0x1C1C0C07,  //  0017  EQ	R7	R6	R7
      0x781E0009,  //  0018  JMPF	R7	#0023
      0x8C1C0906,  //  0019  GETMET	R7	R4	K6
      0x88240909,  //  001A  GETMBR	R9	R4	K9
      0x542A0063,  //  001B  LDINT	R10	100
      0x882C010A,  //  001C  GETMBR	R11	R0	K10
      0x0428140B,  //  001D  SUB	R10	R10	R11
      0x542E0063,  //  001E  LDINT	R11	100
      0x0828140B,  //  001F  MUL	R10	R10	R11
      0x7C1C0600,  //  0020  CALL	R7	3
      0x80040E00,  //  0021  RET	1	R7
      0x70020013,  //  0022  JMP		#0037
      0x541E000B,  //  0023  LDINT	R7	12
      0x1C1C0C07,  //  0024  EQ	R7	R6	R7
      0x781E0005,  //  0025  JMPF	R7	#002C
      0x8C1C0906,  //  0026  GETMET	R7	R4	K6
      0x88240907,  //  0027  GETMBR	R9	R4	K7
      0x5828000B,  //  0028  LDCONST	R10	K11
      0x7C1C0600,  //  0029  CALL	R7	3
      0x80040E00,  //  002A  RET	1	R7
      0x7002000A,  //  002B  JMP		#0037
      0x541EFFFB,  //  002C  LDINT	R7	65532
      0x1C1C0C07,  //  002D  EQ	R7	R6	R7
      0x781E0007,  //  002E  JMPF	R7	#0037
      0x8C1C0906,  //  002F  GETMET	R7	R4	K6
      0x8824090C,  //  0030  GETMBR	R9	R4	K12
      0x542A0003,  //  0031  LDINT	R10	4
      0x002A1A0A,  //  0032  ADD	R10	K13	R10
      0x542E000F,  //  0033  LDINT	R11	16
      0x0028140B,  //  0034  ADD	R10	R10	R11
      0x7C1C0600,  //  0035  CALL	R7	3
      0x80040E00,  //  0036  RET	1	R7
      0x601C0003,  //  0037  GETGBL	R7	G3
      0x5C200000,  //  0038  MOVE	R8	R0
      0x7C1C0200,  //  0039  CALL	R7	1
      0x8C1C0F0E,  //  003A  GETMET	R7	R7	K14
      0x5C240200,  //  003B  MOVE	R9	R1
      0x5C280400,  //  003C  MOVE	R10	R2
      0x7C1C0600,  //  003D  CALL	R7	3
      0x80040E00,  //  003E  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_ShutterTilt
********************************************************************/
extern const bclass be_class_Matter_Plugin_Shutter;
be_local_class(Matter_Plugin_ShutterTilt,
    0,
    &be_class_Matter_Plugin_Shutter,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPE, 1), be_nested_str_weak(shutter_X2Btilt) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Shutter_X20_X2B_X20Tilt) },
        { be_const_key_weak(invoke_request, 3), be_const_closure(Matter_Plugin_ShutterTilt_invoke_request_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_ShutterTilt_read_attribute_closure) },
        { be_const_key_weak(CLUSTERS, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(258, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(7),
        be_const_int(12),
        be_const_int(15),
        be_const_int(65532),
    }))    ) } )) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_ShutterTilt)
);
/*******************************************************************/

void be_load_Matter_Plugin_ShutterTilt_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_ShutterTilt);
    be_setglobal(vm, "Matter_Plugin_ShutterTilt");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
