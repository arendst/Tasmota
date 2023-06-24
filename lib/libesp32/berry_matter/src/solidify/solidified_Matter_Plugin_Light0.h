/* Solidification of Matter_Plugin_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light0;

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
    ( &(const binstruction[24]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E000B,  //  0005  JMPF	R3	#0012
      0x8C0C0502,  //  0006  GETMET	R3	R2	K2
      0x58140003,  //  0007  LDCONST	R5	K3
      0x4C180000,  //  0008  LDNIL	R6
      0x7C0C0600,  //  0009  CALL	R3	3
      0x88100104,  //  000A  GETMBR	R4	R0	K4
      0x20100604,  //  000B  NE	R4	R3	R4
      0x78120004,  //  000C  JMPF	R4	#0012
      0x8C100105,  //  000D  GETMET	R4	R0	K5
      0x541A0005,  //  000E  LDINT	R6	6
      0x581C0006,  //  000F  LDCONST	R7	K6
      0x7C100600,  //  0010  CALL	R4	3
      0x90020803,  //  0011  SETMBR	R0	K4	R3
      0x600C0003,  //  0012  GETGBL	R3	G3
      0x5C100000,  //  0013  MOVE	R4	R0
      0x7C0C0200,  //  0014  CALL	R3	1
      0x8C0C0707,  //  0015  GETMET	R3	R3	K7
      0x7C0C0200,  //  0016  CALL	R3	1
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light0_read_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(BOOL),
    /* K8   */  be_nested_str_weak(shadow_onoff),
    /* K9   */  be_nested_str_weak(U4),
    /* K10  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A0005,  //  0004  LDINT	R6	6
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A001B,  //  0006  JMPF	R6	#0023
      0x8C180104,  //  0007  GETMET	R6	R0	K4
      0x7C180200,  //  0008  CALL	R6	1
      0x1C180B05,  //  0009  EQ	R6	R5	K5
      0x781A0005,  //  000A  JMPF	R6	#0011
      0x8C180706,  //  000B  GETMET	R6	R3	K6
      0x88200707,  //  000C  GETMBR	R8	R3	K7
      0x88240108,  //  000D  GETMBR	R9	R0	K8
      0x7C180600,  //  000E  CALL	R6	3
      0x80040C00,  //  000F  RET	1	R6
      0x70020010,  //  0010  JMP		#0022
      0x541AFFFB,  //  0011  LDINT	R6	65532
      0x1C180A06,  //  0012  EQ	R6	R5	R6
      0x781A0005,  //  0013  JMPF	R6	#001A
      0x8C180706,  //  0014  GETMET	R6	R3	K6
      0x88200709,  //  0015  GETMBR	R8	R3	K9
      0x58240005,  //  0016  LDCONST	R9	K5
      0x7C180600,  //  0017  CALL	R6	3
      0x80040C00,  //  0018  RET	1	R6
      0x70020007,  //  0019  JMP		#0022
      0x541AFFFC,  //  001A  LDINT	R6	65533
      0x1C180A06,  //  001B  EQ	R6	R5	R6
      0x781A0004,  //  001C  JMPF	R6	#0022
      0x8C180706,  //  001D  GETMET	R6	R3	K6
      0x88200709,  //  001E  GETMBR	R8	R3	K9
      0x54260003,  //  001F  LDINT	R9	4
      0x7C180600,  //  0020  CALL	R6	3
      0x80040C00,  //  0021  RET	1	R6
      0x70020007,  //  0022  JMP		#002B
      0x60180003,  //  0023  GETGBL	R6	G3
      0x5C1C0000,  //  0024  MOVE	R7	R0
      0x7C180200,  //  0025  CALL	R6	1
      0x8C180D0A,  //  0026  GETMET	R6	R6	K10
      0x5C200200,  //  0027  MOVE	R8	R1
      0x5C240400,  //  0028  MOVE	R9	R2
      0x7C180600,  //  0029  CALL	R6	3
      0x80040C00,  //  002A  RET	1	R6
      0x80000000,  //  002B  RET	0
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
** Solidified class: Matter_Plugin_Light0
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Light0,
    1,
    &be_class_Matter_Plugin_Device,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(256, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light0) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light0_read_attribute_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Light_X200_X20On) },
        { be_const_key_weak(CLUSTERS, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light0_init_closure) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(250) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light0_invoke_request_closure) },
        { be_const_key_weak(shadow_onoff, -1), be_const_var(0) },
        { be_const_key_weak(update_shadow, 1), be_const_closure(Matter_Plugin_Light0_update_shadow_closure) },
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
