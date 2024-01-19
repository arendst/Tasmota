/* Solidification of Matter_Plugin_1_Aggregator.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Aggregator;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Aggregator_read_attribute,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(Matter_TLV_array),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(get_active_endpoints),
    /* K8   */  be_nested_str_weak(AGGREGATOR_ENDPOINT),
    /* K9   */  be_nested_str_weak(add_TLV),
    /* K10  */  be_nested_str_weak(U2),
    /* K11  */  be_nested_str_weak(stop_iteration),
    /* K12  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E001C,  //  0004  LDINT	R7	29
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0026,  //  0006  JMPF	R7	#002E
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E001A,  //  0008  JMPF	R7	#0024
      0x8C1C0905,  //  0009  GETMET	R7	R4	K5
      0x7C1C0200,  //  000A  CALL	R7	1
      0x88200106,  //  000B  GETMBR	R8	R0	K6
      0x8C201107,  //  000C  GETMET	R8	R8	K7
      0x50280200,  //  000D  LDBOOL	R10	1	0
      0x7C200400,  //  000E  CALL	R8	2
      0x60240010,  //  000F  GETGBL	R9	G16
      0x5C281000,  //  0010  MOVE	R10	R8
      0x7C240200,  //  0011  CALL	R9	1
      0xA802000B,  //  0012  EXBLK	0	#001F
      0x5C281200,  //  0013  MOVE	R10	R9
      0x7C280000,  //  0014  CALL	R10	0
      0xB82E0000,  //  0015  GETNGBL	R11	K0
      0x882C1708,  //  0016  GETMBR	R11	R11	K8
      0x142C140B,  //  0017  LT	R11	R10	R11
      0x782E0004,  //  0018  JMPF	R11	#001E
      0x8C2C0F09,  //  0019  GETMET	R11	R7	K9
      0x4C340000,  //  001A  LDNIL	R13
      0x8838090A,  //  001B  GETMBR	R14	R4	K10
      0x5C3C1400,  //  001C  MOVE	R15	R10
      0x7C2C0800,  //  001D  CALL	R11	4
      0x7001FFF3,  //  001E  JMP		#0013
      0x5824000B,  //  001F  LDCONST	R9	K11
      0xAC240200,  //  0020  CATCH	R9	1	0
      0xB0080000,  //  0021  RAISE	2	R0	R0
      0x80040E00,  //  0022  RET	1	R7
      0x70020008,  //  0023  JMP		#002D
      0x601C0003,  //  0024  GETGBL	R7	G3
      0x5C200000,  //  0025  MOVE	R8	R0
      0x7C1C0200,  //  0026  CALL	R7	1
      0x8C1C0F0C,  //  0027  GETMET	R7	R7	K12
      0x5C240200,  //  0028  MOVE	R9	R1
      0x5C280400,  //  0029  MOVE	R10	R2
      0x5C2C0600,  //  002A  MOVE	R11	R3
      0x7C1C0800,  //  002B  CALL	R7	4
      0x80040E00,  //  002C  RET	1	R7
      0x70020008,  //  002D  JMP		#0037
      0x601C0003,  //  002E  GETGBL	R7	G3
      0x5C200000,  //  002F  MOVE	R8	R0
      0x7C1C0200,  //  0030  CALL	R7	1
      0x8C1C0F0C,  //  0031  GETMET	R7	R7	K12
      0x5C240200,  //  0032  MOVE	R9	R1
      0x5C280400,  //  0033  MOVE	R10	R2
      0x5C2C0600,  //  0034  MOVE	R11	R3
      0x7C1C0800,  //  0035  CALL	R7	4
      0x80040E00,  //  0036  RET	1	R7
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Aggregator
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Aggregator,
    0,
    &be_class_Matter_Plugin,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(14, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(TYPE, 3), be_nested_str_weak(aggregator) },
        { be_const_key_weak(read_attribute, 0), be_const_closure(Matter_Plugin_Aggregator_read_attribute_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Aggregator) },
    })),
    be_str_weak(Matter_Plugin_Aggregator)
);
/*******************************************************************/

void be_load_Matter_Plugin_Aggregator_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Aggregator);
    be_setglobal(vm, "Matter_Plugin_Aggregator");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
