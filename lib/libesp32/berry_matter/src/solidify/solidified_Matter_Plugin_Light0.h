/* Solidification of Matter_Plugin_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light0;

/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light0_invoke_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(set),
    /* K8   */  be_nested_str_weak(power),
    /* K9   */  be_nested_str_weak(update_shadow),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220005,  //  0005  LDINT	R8	6
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x7822002A,  //  0007  JMPF	R8	#0033
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x7822000A,  //  000B  JMPF	R8	#0017
      0x8C200907,  //  000C  GETMET	R8	R4	K7
      0x60280013,  //  000D  GETGBL	R10	G19
      0x7C280000,  //  000E  CALL	R10	0
      0x502C0000,  //  000F  LDBOOL	R11	0	0
      0x982A100B,  //  0010  SETIDX	R10	K8	R11
      0x7C200400,  //  0011  CALL	R8	2
      0x8C200109,  //  0012  GETMET	R8	R0	K9
      0x7C200200,  //  0013  CALL	R8	1
      0x50200200,  //  0014  LDBOOL	R8	1	0
      0x80041000,  //  0015  RET	1	R8
      0x7002001B,  //  0016  JMP		#0033
      0x1C200F0A,  //  0017  EQ	R8	R7	K10
      0x7822000A,  //  0018  JMPF	R8	#0024
      0x8C200907,  //  0019  GETMET	R8	R4	K7
      0x60280013,  //  001A  GETGBL	R10	G19
      0x7C280000,  //  001B  CALL	R10	0
      0x502C0200,  //  001C  LDBOOL	R11	1	0
      0x982A100B,  //  001D  SETIDX	R10	K8	R11
      0x7C200400,  //  001E  CALL	R8	2
      0x8C200109,  //  001F  GETMET	R8	R0	K9
      0x7C200200,  //  0020  CALL	R8	1
      0x50200200,  //  0021  LDBOOL	R8	1	0
      0x80041000,  //  0022  RET	1	R8
      0x7002000E,  //  0023  JMP		#0033
      0x1C200F0B,  //  0024  EQ	R8	R7	K11
      0x7822000C,  //  0025  JMPF	R8	#0033
      0x8C200907,  //  0026  GETMET	R8	R4	K7
      0x60280013,  //  0027  GETGBL	R10	G19
      0x7C280000,  //  0028  CALL	R10	0
      0x882C010C,  //  0029  GETMBR	R11	R0	K12
      0x782E0000,  //  002A  JMPF	R11	#002C
      0x502C0001,  //  002B  LDBOOL	R11	0	1
      0x502C0200,  //  002C  LDBOOL	R11	1	0
      0x982A100B,  //  002D  SETIDX	R10	K8	R11
      0x7C200400,  //  002E  CALL	R8	2
      0x8C200109,  //  002F  GETMET	R8	R0	K9
      0x7C200200,  //  0030  CALL	R8	1
      0x50200200,  //  0031  LDBOOL	R8	1	0
      0x80041000,  //  0032  RET	1	R8
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light0_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(BOOL),
    /* K9   */  be_nested_str_weak(shadow_onoff),
    /* K10  */  be_nested_str_weak(U4),
    /* K11  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0005,  //  0005  LDINT	R7	6
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E001B,  //  0007  JMPF	R7	#0024
      0x8C1C0105,  //  0008  GETMET	R7	R0	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x1C1C0D06,  //  000A  EQ	R7	R6	K6
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0907,  //  000C  GETMET	R7	R4	K7
      0x88240908,  //  000D  GETMBR	R9	R4	K8
      0x88280109,  //  000E  GETMBR	R10	R0	K9
      0x7C1C0600,  //  000F  CALL	R7	3
      0x80040E00,  //  0010  RET	1	R7
      0x70020010,  //  0011  JMP		#0023
      0x541EFFFB,  //  0012  LDINT	R7	65532
      0x1C1C0C07,  //  0013  EQ	R7	R6	R7
      0x781E0005,  //  0014  JMPF	R7	#001B
      0x8C1C0907,  //  0015  GETMET	R7	R4	K7
      0x8824090A,  //  0016  GETMBR	R9	R4	K10
      0x58280006,  //  0017  LDCONST	R10	K6
      0x7C1C0600,  //  0018  CALL	R7	3
      0x80040E00,  //  0019  RET	1	R7
      0x70020007,  //  001A  JMP		#0023
      0x541EFFFC,  //  001B  LDINT	R7	65533
      0x1C1C0C07,  //  001C  EQ	R7	R6	R7
      0x781E0004,  //  001D  JMPF	R7	#0023
      0x8C1C0907,  //  001E  GETMET	R7	R4	K7
      0x8824090A,  //  001F  GETMBR	R9	R4	K10
      0x542A0003,  //  0020  LDINT	R10	4
      0x7C1C0600,  //  0021  CALL	R7	3
      0x80040E00,  //  0022  RET	1	R7
      0x70020007,  //  0023  JMP		#002C
      0x601C0003,  //  0024  GETGBL	R7	G3
      0x5C200000,  //  0025  MOVE	R8	R0
      0x7C1C0200,  //  0026  CALL	R7	1
      0x8C1C0F0B,  //  0027  GETMET	R7	R7	K11
      0x5C240200,  //  0028  MOVE	R9	R1
      0x5C280400,  //  0029  MOVE	R10	R2
      0x7C1C0600,  //  002A  CALL	R7	3
      0x80040E00,  //  002B  RET	1	R7
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light0_update_shadow,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(power),
    /* K4   */  be_nested_str_weak(shadow_onoff),
    /* K5   */  be_nested_str_weak(attribute_updated),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x58140003,  //  0004  LDCONST	R5	K3
      0x4C180000,  //  0005  LDNIL	R6
      0x7C0C0600,  //  0006  CALL	R3	3
      0x88100104,  //  0007  GETMBR	R4	R0	K4
      0x20100604,  //  0008  NE	R4	R3	R4
      0x78120004,  //  0009  JMPF	R4	#000F
      0x8C100105,  //  000A  GETMET	R4	R0	K5
      0x541A0005,  //  000B  LDINT	R6	6
      0x581C0006,  //  000C  LDCONST	R7	K6
      0x7C100600,  //  000D  CALL	R4	3
      0x90020803,  //  000E  SETMBR	R0	K4	R3
      0x60100003,  //  000F  GETGBL	R4	G3
      0x5C140000,  //  0010  MOVE	R5	R0
      0x7C100200,  //  0011  CALL	R4	1
      0x8C100907,  //  0012  GETMET	R4	R4	K7
      0x7C100200,  //  0013  CALL	R4	1
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light0_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x50100000,  //  0008  LDBOOL	R4	0	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light0
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Light0,
    1,
    &be_class_Matter_Plugin_Device,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light0_init_closure) },
        { be_const_key_weak(shadow_onoff, 8), be_const_var(0) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(256, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Light_X200_X20On) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light0_invoke_request_closure) },
        { be_const_key_weak(read_attribute, 0), be_const_closure(Matter_Plugin_Light0_read_attribute_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Light0_update_shadow_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light0) },
    })),
    be_str_weak(Matter_Plugin_Light0)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light0_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light0);
    be_setglobal(vm, "Matter_Plugin_Light0");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
