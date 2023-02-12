/* Solidification of Matter_Plugin_Relay.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Relay;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Relay_read_attribute,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(Matter_TLV_array),
    /* K7   */  be_nested_str_weak(add_struct),
    /* K8   */  be_nested_str_weak(add_TLV),
    /* K9   */  be_nested_str_weak(U2),
    /* K10  */  be_nested_str_weak(TYPES),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(get_cluster_list),
    /* K13  */  be_nested_str_weak(U4),
    /* K14  */  be_nested_str_weak(stop_iteration),
    /* K15  */  be_const_int(2),
    /* K16  */  be_const_int(3),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E001C,  //  0005  LDINT	R7	29
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0038,  //  0007  JMPF	R7	#0041
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E0010,  //  0009  JMPF	R7	#001B
      0x8C1C0906,  //  000A  GETMET	R7	R4	K6
      0x7C1C0200,  //  000B  CALL	R7	1
      0x8C200F07,  //  000C  GETMET	R8	R7	K7
      0x7C200200,  //  000D  CALL	R8	1
      0x8C241108,  //  000E  GETMET	R9	R8	K8
      0x582C0005,  //  000F  LDCONST	R11	K5
      0x88300909,  //  0010  GETMBR	R12	R4	K9
      0x8834010A,  //  0011  GETMBR	R13	R0	K10
      0x94341B05,  //  0012  GETIDX	R13	R13	K5
      0x7C240800,  //  0013  CALL	R9	4
      0x8C241108,  //  0014  GETMET	R9	R8	K8
      0x582C000B,  //  0015  LDCONST	R11	K11
      0x88300909,  //  0016  GETMBR	R12	R4	K9
      0x5834000B,  //  0017  LDCONST	R13	K11
      0x7C240800,  //  0018  CALL	R9	4
      0x80040E00,  //  0019  RET	1	R7
      0x70020025,  //  001A  JMP		#0041
      0x1C1C0D0B,  //  001B  EQ	R7	R6	K11
      0x781E0013,  //  001C  JMPF	R7	#0031
      0x8C1C0906,  //  001D  GETMET	R7	R4	K6
      0x7C1C0200,  //  001E  CALL	R7	1
      0x60200010,  //  001F  GETGBL	R8	G16
      0x8C24010C,  //  0020  GETMET	R9	R0	K12
      0x7C240200,  //  0021  CALL	R9	1
      0x7C200200,  //  0022  CALL	R8	1
      0xA8020007,  //  0023  EXBLK	0	#002C
      0x5C241000,  //  0024  MOVE	R9	R8
      0x7C240000,  //  0025  CALL	R9	0
      0x8C280F08,  //  0026  GETMET	R10	R7	K8
      0x4C300000,  //  0027  LDNIL	R12
      0x8834090D,  //  0028  GETMBR	R13	R4	K13
      0x5C381200,  //  0029  MOVE	R14	R9
      0x7C280800,  //  002A  CALL	R10	4
      0x7001FFF7,  //  002B  JMP		#0024
      0x5820000E,  //  002C  LDCONST	R8	K14
      0xAC200200,  //  002D  CATCH	R8	1	0
      0xB0080000,  //  002E  RAISE	2	R0	R0
      0x80040E00,  //  002F  RET	1	R7
      0x7002000F,  //  0030  JMP		#0041
      0x1C1C0D0F,  //  0031  EQ	R7	R6	K15
      0x781E0008,  //  0032  JMPF	R7	#003C
      0x8C1C0906,  //  0033  GETMET	R7	R4	K6
      0x7C1C0200,  //  0034  CALL	R7	1
      0x8C200F08,  //  0035  GETMET	R8	R7	K8
      0x4C280000,  //  0036  LDNIL	R10
      0x882C0909,  //  0037  GETMBR	R11	R4	K9
      0x54320005,  //  0038  LDINT	R12	6
      0x7C200800,  //  0039  CALL	R8	4
      0x80040E00,  //  003A  RET	1	R7
      0x70020004,  //  003B  JMP		#0041
      0x1C1C0D10,  //  003C  EQ	R7	R6	K16
      0x781E0002,  //  003D  JMPF	R7	#0041
      0x8C1C0906,  //  003E  GETMET	R7	R4	K6
      0x7C1C0200,  //  003F  CALL	R7	1
      0x80040E00,  //  0040  RET	1	R7
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Relay_init,   /* name */
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
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(endpoints),
    /* K2   */  be_nested_str_weak(ENDPOINTS),
    /* K3   */  be_nested_str_weak(clusters),
    /* K4   */  be_nested_str_weak(CLUSTERS),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080102,  //  0006  GETMBR	R2	R0	K2
      0x90020202,  //  0007  SETMBR	R0	K1	R2
      0x88080104,  //  0008  GETMBR	R2	R0	K4
      0x90020602,  //  0009  SETMBR	R0	K3	R2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Relay_invoke_request,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Relay
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Relay,
    0,
    &be_class_Matter_Plugin,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Relay_read_attribute_closure) },
        { be_const_key_weak(ENDPOINTS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(1),
    }))    ) } )) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(256),
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Relay_init_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
    }))    ) } )) },
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
    }))    ) } )) },
        { be_const_key_int(8, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_int(5, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Relay_invoke_request_closure) },
    })),
    be_str_weak(Matter_Plugin_Relay)
);
/*******************************************************************/

void be_load_Matter_Plugin_Relay_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Relay);
    be_setglobal(vm, "Matter_Plugin_Relay");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
