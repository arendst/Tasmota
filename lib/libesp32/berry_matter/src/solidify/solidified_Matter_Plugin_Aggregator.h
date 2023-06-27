/* Solidification of Matter_Plugin_Aggregator.h */
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
    15,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(Matter_TLV_array),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(get_active_endpoints),
    /* K8   */  be_nested_str_weak(add_TLV),
    /* K9   */  be_nested_str_weak(U2),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A001C,  //  0004  LDINT	R6	29
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A0024,  //  0006  JMPF	R6	#002C
      0x1C180B04,  //  0007  EQ	R6	R5	K4
      0x781A0019,  //  0008  JMPF	R6	#0023
      0x8C180705,  //  0009  GETMET	R6	R3	K5
      0x7C180200,  //  000A  CALL	R6	1
      0x881C0106,  //  000B  GETMBR	R7	R0	K6
      0x8C1C0F07,  //  000C  GETMET	R7	R7	K7
      0x50240200,  //  000D  LDBOOL	R9	1	0
      0x7C1C0400,  //  000E  CALL	R7	2
      0x60200010,  //  000F  GETGBL	R8	G16
      0x5C240E00,  //  0010  MOVE	R9	R7
      0x7C200200,  //  0011  CALL	R8	1
      0xA802000A,  //  0012  EXBLK	0	#001E
      0x5C241000,  //  0013  MOVE	R9	R8
      0x7C240000,  //  0014  CALL	R9	0
      0x542AFEFF,  //  0015  LDINT	R10	65280
      0x1428120A,  //  0016  LT	R10	R9	R10
      0x782A0004,  //  0017  JMPF	R10	#001D
      0x8C280D08,  //  0018  GETMET	R10	R6	K8
      0x4C300000,  //  0019  LDNIL	R12
      0x88340709,  //  001A  GETMBR	R13	R3	K9
      0x5C381200,  //  001B  MOVE	R14	R9
      0x7C280800,  //  001C  CALL	R10	4
      0x7001FFF4,  //  001D  JMP		#0013
      0x5820000A,  //  001E  LDCONST	R8	K10
      0xAC200200,  //  001F  CATCH	R8	1	0
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0x80040C00,  //  0021  RET	1	R6
      0x70020007,  //  0022  JMP		#002B
      0x60180003,  //  0023  GETGBL	R6	G3
      0x5C1C0000,  //  0024  MOVE	R7	R0
      0x7C180200,  //  0025  CALL	R6	1
      0x8C180D0B,  //  0026  GETMET	R6	R6	K11
      0x5C200200,  //  0027  MOVE	R8	R1
      0x5C240400,  //  0028  MOVE	R9	R2
      0x7C180600,  //  0029  CALL	R6	3
      0x80040C00,  //  002A  RET	1	R6
      0x70020007,  //  002B  JMP		#0034
      0x60180003,  //  002C  GETGBL	R6	G3
      0x5C1C0000,  //  002D  MOVE	R7	R0
      0x7C180200,  //  002E  CALL	R6	1
      0x8C180D0B,  //  002F  GETMET	R6	R6	K11
      0x5C200200,  //  0030  MOVE	R8	R1
      0x5C240400,  //  0031  MOVE	R9	R2
      0x7C180600,  //  0032  CALL	R6	3
      0x80040C00,  //  0033  RET	1	R6
      0x80000000,  //  0034  RET	0
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
        { be_const_key_weak(NAME, 3), be_nested_str_weak(Aggregator) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(aggregator) },
        { be_const_key_weak(TYPES, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(14, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Aggregator_read_attribute_closure) },
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
