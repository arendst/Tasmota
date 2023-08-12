/* Solidification of Matter_Plugin_Light2.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light2;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light2_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_nested_str_weak(set),
    /* K6   */  be_nested_str_weak(U1),
    /* K7   */  be_nested_str_weak(shadow_ct),
    /* K8   */  be_const_int(2),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(ct_min),
    /* K11  */  be_nested_str_weak(ct_max),
    /* K12  */  be_nested_str_weak(U4),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[82]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0040,  //  0006  JMPF	R7	#0048
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x541E0006,  //  0009  LDINT	R7	7
      0x1C1C0C07,  //  000A  EQ	R7	R6	R7
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0705,  //  000C  GETMET	R7	R3	K5
      0x88240906,  //  000D  GETMBR	R9	R4	K6
      0x88280107,  //  000E  GETMBR	R10	R0	K7
      0x7C1C0600,  //  000F  CALL	R7	3
      0x80040E00,  //  0010  RET	1	R7
      0x70020034,  //  0011  JMP		#0047
      0x541E0007,  //  0012  LDINT	R7	8
      0x1C1C0C07,  //  0013  EQ	R7	R6	R7
      0x781E0005,  //  0014  JMPF	R7	#001B
      0x8C1C0705,  //  0015  GETMET	R7	R3	K5
      0x88240906,  //  0016  GETMBR	R9	R4	K6
      0x58280008,  //  0017  LDCONST	R10	K8
      0x7C1C0600,  //  0018  CALL	R7	3
      0x80040E00,  //  0019  RET	1	R7
      0x7002002B,  //  001A  JMP		#0047
      0x541E000E,  //  001B  LDINT	R7	15
      0x1C1C0C07,  //  001C  EQ	R7	R6	R7
      0x781E0005,  //  001D  JMPF	R7	#0024
      0x8C1C0705,  //  001E  GETMET	R7	R3	K5
      0x88240906,  //  001F  GETMBR	R9	R4	K6
      0x58280009,  //  0020  LDCONST	R10	K9
      0x7C1C0600,  //  0021  CALL	R7	3
      0x80040E00,  //  0022  RET	1	R7
      0x70020022,  //  0023  JMP		#0047
      0x541E400A,  //  0024  LDINT	R7	16395
      0x1C1C0C07,  //  0025  EQ	R7	R6	R7
      0x781E0005,  //  0026  JMPF	R7	#002D
      0x8C1C0705,  //  0027  GETMET	R7	R3	K5
      0x88240906,  //  0028  GETMBR	R9	R4	K6
      0x8828010A,  //  0029  GETMBR	R10	R0	K10
      0x7C1C0600,  //  002A  CALL	R7	3
      0x80040E00,  //  002B  RET	1	R7
      0x70020019,  //  002C  JMP		#0047
      0x541E400B,  //  002D  LDINT	R7	16396
      0x1C1C0C07,  //  002E  EQ	R7	R6	R7
      0x781E0005,  //  002F  JMPF	R7	#0036
      0x8C1C0705,  //  0030  GETMET	R7	R3	K5
      0x88240906,  //  0031  GETMBR	R9	R4	K6
      0x8828010B,  //  0032  GETMBR	R10	R0	K11
      0x7C1C0600,  //  0033  CALL	R7	3
      0x80040E00,  //  0034  RET	1	R7
      0x70020010,  //  0035  JMP		#0047
      0x541EFFFB,  //  0036  LDINT	R7	65532
      0x1C1C0C07,  //  0037  EQ	R7	R6	R7
      0x781E0005,  //  0038  JMPF	R7	#003F
      0x8C1C0705,  //  0039  GETMET	R7	R3	K5
      0x8824090C,  //  003A  GETMBR	R9	R4	K12
      0x542A000F,  //  003B  LDINT	R10	16
      0x7C1C0600,  //  003C  CALL	R7	3
      0x80040E00,  //  003D  RET	1	R7
      0x70020007,  //  003E  JMP		#0047
      0x541EFFFC,  //  003F  LDINT	R7	65533
      0x1C1C0C07,  //  0040  EQ	R7	R6	R7
      0x781E0004,  //  0041  JMPF	R7	#0047
      0x8C1C0705,  //  0042  GETMET	R7	R3	K5
      0x8824090C,  //  0043  GETMBR	R9	R4	K12
      0x542A0004,  //  0044  LDINT	R10	5
      0x7C1C0600,  //  0045  CALL	R7	3
      0x80040E00,  //  0046  RET	1	R7
      0x70020008,  //  0047  JMP		#0051
      0x601C0003,  //  0048  GETGBL	R7	G3
      0x5C200000,  //  0049  MOVE	R8	R0
      0x7C1C0200,  //  004A  CALL	R7	1
      0x8C1C0F0D,  //  004B  GETMET	R7	R7	K13
      0x5C240200,  //  004C  MOVE	R9	R1
      0x5C280400,  //  004D  MOVE	R10	R2
      0x5C2C0600,  //  004E  MOVE	R11	R3
      0x7C1C0800,  //  004F  CALL	R7	4
      0x80040E00,  //  0050  RET	1	R7
      0x80000000,  //  0051  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light2_update_shadow,   /* name */
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
    /* K1   */  be_nested_str_weak(update_ct_minmax),
    /* K2   */  be_nested_str_weak(update_shadow),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(ct),
    /* K6   */  be_nested_str_weak(shadow_ct),
    /* K7   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x60080003,  //  0003  GETGBL	R2	G3
      0x5C0C0000,  //  0004  MOVE	R3	R0
      0x7C080200,  //  0005  CALL	R2	1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080303,  //  0008  GETMET	R2	R1	K3
      0x7C080200,  //  0009  CALL	R2	1
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E000F,  //  000C  JMPF	R3	#001D
      0x8C0C0504,  //  000D  GETMET	R3	R2	K4
      0x58140005,  //  000E  LDCONST	R5	K5
      0x4C180000,  //  000F  LDNIL	R6
      0x7C0C0600,  //  0010  CALL	R3	3
      0x4C100000,  //  0011  LDNIL	R4
      0x1C100604,  //  0012  EQ	R4	R3	R4
      0x78120000,  //  0013  JMPF	R4	#0015
      0x880C0106,  //  0014  GETMBR	R3	R0	K6
      0x88100106,  //  0015  GETMBR	R4	R0	K6
      0x20100604,  //  0016  NE	R4	R3	R4
      0x78120004,  //  0017  JMPF	R4	#001D
      0x8C100107,  //  0018  GETMET	R4	R0	K7
      0x541A02FF,  //  0019  LDINT	R6	768
      0x541E0006,  //  001A  LDINT	R7	7
      0x7C100600,  //  001B  CALL	R4	3
      0x90020C03,  //  001C  SETMBR	R0	K6	R3
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light2_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_ct),
    /* K2   */  be_nested_str_weak(update_ct_minmax),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x54120144,  //  0008  LDINT	R4	325
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x8C100102,  //  000A  GETMET	R4	R0	K2
      0x7C100200,  //  000B  CALL	R4	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_ct_minmax
********************************************************************/
be_local_closure(Matter_Plugin_Light2_update_ct_minmax,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(get_option),
    /* K2   */  be_nested_str_weak(ct_min),
    /* K3   */  be_nested_str_weak(ct_max),
    }),
    be_str_weak(update_ct_minmax),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x540E0051,  //  0002  LDINT	R3	82
      0x7C040400,  //  0003  CALL	R1	2
      0x78060001,  //  0004  JMPF	R1	#0007
      0x540A00C7,  //  0005  LDINT	R2	200
      0x70020000,  //  0006  JMP		#0008
      0x540A0098,  //  0007  LDINT	R2	153
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x540A017B,  //  000A  LDINT	R2	380
      0x70020000,  //  000B  JMP		#000D
      0x540A01F3,  //  000C  LDINT	R2	500
      0x90020602,  //  000D  SETMBR	R0	K3	R2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light2_invoke_request,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(ct_min),
    /* K9   */  be_nested_str_weak(ct_max),
    /* K10  */  be_nested_str_weak(set),
    /* K11  */  be_nested_str_weak(ct),
    /* K12  */  be_nested_str_weak(update_shadow),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(ct_X3A),
    /* K15  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[67]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x542202FF,  //  0005  LDINT	R8	768
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220030,  //  0007  JMPF	R8	#0039
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x54220009,  //  000A  LDINT	R8	10
      0x1C200E08,  //  000B  EQ	R8	R7	R8
      0x78220019,  //  000C  JMPF	R8	#0027
      0x8C200506,  //  000D  GETMET	R8	R2	K6
      0x58280007,  //  000E  LDCONST	R10	K7
      0x7C200400,  //  000F  CALL	R8	2
      0x88240108,  //  0010  GETMBR	R9	R0	K8
      0x14241009,  //  0011  LT	R9	R8	R9
      0x78260000,  //  0012  JMPF	R9	#0014
      0x88200108,  //  0013  GETMBR	R8	R0	K8
      0x88240109,  //  0014  GETMBR	R9	R0	K9
      0x24241009,  //  0015  GT	R9	R8	R9
      0x78260000,  //  0016  JMPF	R9	#0018
      0x88200109,  //  0017  GETMBR	R8	R0	K9
      0x8C24090A,  //  0018  GETMET	R9	R4	K10
      0x602C0013,  //  0019  GETGBL	R11	G19
      0x7C2C0000,  //  001A  CALL	R11	0
      0x982E1608,  //  001B  SETIDX	R11	K11	R8
      0x7C240400,  //  001C  CALL	R9	2
      0x8C24010C,  //  001D  GETMET	R9	R0	K12
      0x7C240200,  //  001E  CALL	R9	1
      0x60240008,  //  001F  GETGBL	R9	G8
      0x5C281000,  //  0020  MOVE	R10	R8
      0x7C240200,  //  0021  CALL	R9	1
      0x00261C09,  //  0022  ADD	R9	K14	R9
      0x900E1A09,  //  0023  SETMBR	R3	K13	R9
      0x50240200,  //  0024  LDBOOL	R9	1	0
      0x80041200,  //  0025  RET	1	R9
      0x70020010,  //  0026  JMP		#0038
      0x54220046,  //  0027  LDINT	R8	71
      0x1C200E08,  //  0028  EQ	R8	R7	R8
      0x78220002,  //  0029  JMPF	R8	#002D
      0x50200200,  //  002A  LDBOOL	R8	1	0
      0x80041000,  //  002B  RET	1	R8
      0x7002000A,  //  002C  JMP		#0038
      0x5422004A,  //  002D  LDINT	R8	75
      0x1C200E08,  //  002E  EQ	R8	R7	R8
      0x78220002,  //  002F  JMPF	R8	#0033
      0x50200200,  //  0030  LDBOOL	R8	1	0
      0x80041000,  //  0031  RET	1	R8
      0x70020004,  //  0032  JMP		#0038
      0x5422004B,  //  0033  LDINT	R8	76
      0x1C200E08,  //  0034  EQ	R8	R7	R8
      0x78220001,  //  0035  JMPF	R8	#0038
      0x50200200,  //  0036  LDBOOL	R8	1	0
      0x80041000,  //  0037  RET	1	R8
      0x70020008,  //  0038  JMP		#0042
      0x60200003,  //  0039  GETGBL	R8	G3
      0x5C240000,  //  003A  MOVE	R9	R0
      0x7C200200,  //  003B  CALL	R8	1
      0x8C20110F,  //  003C  GETMET	R8	R8	K15
      0x5C280200,  //  003D  MOVE	R10	R1
      0x5C2C0400,  //  003E  MOVE	R11	R2
      0x5C300600,  //  003F  MOVE	R12	R3
      0x7C200800,  //  0040  CALL	R8	4
      0x80041000,  //  0041  RET	1	R8
      0x80000000,  //  0042  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light2
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_class(Matter_Plugin_Light2,
    3,
    &be_class_Matter_Plugin_Light1,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ct_min, -1), be_const_var(1) },
        { be_const_key_weak(shadow_ct, 8), be_const_var(0) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(268, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(ct_max, 7), be_const_var(2) },
        { be_const_key_weak(TYPE, 0), be_nested_str_weak(light2) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light2_read_attribute_closure) },
        { be_const_key_weak(NAME, 5), be_nested_str_weak(Light_X202_X20CT) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light2_init_closure) },
        { be_const_key_weak(update_shadow, 4), be_const_closure(Matter_Plugin_Light2_update_shadow_closure) },
        { be_const_key_weak(update_ct_minmax, -1), be_const_closure(Matter_Plugin_Light2_update_ct_minmax_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(768, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(7),
        be_const_int(8),
        be_const_int(15),
        be_const_int(16395),
        be_const_int(16396),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light2_invoke_request_closure) },
    })),
    be_str_weak(Matter_Plugin_Light2)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light2_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light2);
    be_setglobal(vm, "Matter_Plugin_Light2");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
