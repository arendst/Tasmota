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
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(create_TLV),
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
    ( &(const binstruction[80]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E02FF,  //  0005  LDINT	R7	768
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E003E,  //  0007  JMPF	R7	#0047
      0x541E0006,  //  0008  LDINT	R7	7
      0x1C1C0C07,  //  0009  EQ	R7	R6	R7
      0x781E0005,  //  000A  JMPF	R7	#0011
      0x8C1C0905,  //  000B  GETMET	R7	R4	K5
      0x88240906,  //  000C  GETMBR	R9	R4	K6
      0x88280107,  //  000D  GETMBR	R10	R0	K7
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x70020034,  //  0010  JMP		#0046
      0x541E0007,  //  0011  LDINT	R7	8
      0x1C1C0C07,  //  0012  EQ	R7	R6	R7
      0x781E0005,  //  0013  JMPF	R7	#001A
      0x8C1C0905,  //  0014  GETMET	R7	R4	K5
      0x88240906,  //  0015  GETMBR	R9	R4	K6
      0x58280008,  //  0016  LDCONST	R10	K8
      0x7C1C0600,  //  0017  CALL	R7	3
      0x80040E00,  //  0018  RET	1	R7
      0x7002002B,  //  0019  JMP		#0046
      0x541E000E,  //  001A  LDINT	R7	15
      0x1C1C0C07,  //  001B  EQ	R7	R6	R7
      0x781E0005,  //  001C  JMPF	R7	#0023
      0x8C1C0905,  //  001D  GETMET	R7	R4	K5
      0x88240906,  //  001E  GETMBR	R9	R4	K6
      0x58280009,  //  001F  LDCONST	R10	K9
      0x7C1C0600,  //  0020  CALL	R7	3
      0x80040E00,  //  0021  RET	1	R7
      0x70020022,  //  0022  JMP		#0046
      0x541E400A,  //  0023  LDINT	R7	16395
      0x1C1C0C07,  //  0024  EQ	R7	R6	R7
      0x781E0005,  //  0025  JMPF	R7	#002C
      0x8C1C0905,  //  0026  GETMET	R7	R4	K5
      0x88240906,  //  0027  GETMBR	R9	R4	K6
      0x8828010A,  //  0028  GETMBR	R10	R0	K10
      0x7C1C0600,  //  0029  CALL	R7	3
      0x80040E00,  //  002A  RET	1	R7
      0x70020019,  //  002B  JMP		#0046
      0x541E400B,  //  002C  LDINT	R7	16396
      0x1C1C0C07,  //  002D  EQ	R7	R6	R7
      0x781E0005,  //  002E  JMPF	R7	#0035
      0x8C1C0905,  //  002F  GETMET	R7	R4	K5
      0x88240906,  //  0030  GETMBR	R9	R4	K6
      0x8828010B,  //  0031  GETMBR	R10	R0	K11
      0x7C1C0600,  //  0032  CALL	R7	3
      0x80040E00,  //  0033  RET	1	R7
      0x70020010,  //  0034  JMP		#0046
      0x541EFFFB,  //  0035  LDINT	R7	65532
      0x1C1C0C07,  //  0036  EQ	R7	R6	R7
      0x781E0005,  //  0037  JMPF	R7	#003E
      0x8C1C0905,  //  0038  GETMET	R7	R4	K5
      0x8824090C,  //  0039  GETMBR	R9	R4	K12
      0x542A000F,  //  003A  LDINT	R10	16
      0x7C1C0600,  //  003B  CALL	R7	3
      0x80040E00,  //  003C  RET	1	R7
      0x70020007,  //  003D  JMP		#0046
      0x541EFFFC,  //  003E  LDINT	R7	65533
      0x1C1C0C07,  //  003F  EQ	R7	R6	R7
      0x781E0004,  //  0040  JMPF	R7	#0046
      0x8C1C0905,  //  0041  GETMET	R7	R4	K5
      0x8824090C,  //  0042  GETMBR	R9	R4	K12
      0x542A0004,  //  0043  LDINT	R10	5
      0x7C1C0600,  //  0044  CALL	R7	3
      0x80040E00,  //  0045  RET	1	R7
      0x70020007,  //  0046  JMP		#004F
      0x601C0003,  //  0047  GETGBL	R7	G3
      0x5C200000,  //  0048  MOVE	R8	R0
      0x7C1C0200,  //  0049  CALL	R7	1
      0x8C1C0F0D,  //  004A  GETMET	R7	R7	K13
      0x5C240200,  //  004B  MOVE	R9	R1
      0x5C280400,  //  004C  MOVE	R10	R2
      0x7C1C0600,  //  004D  CALL	R7	3
      0x80040E00,  //  004E  RET	1	R7
      0x80000000,  //  004F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light2_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
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
    ( &(const binstruction[12]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x540E0144,  //  0007  LDINT	R3	325
      0x90020203,  //  0008  SETMBR	R0	K1	R3
      0x8C0C0102,  //  0009  GETMET	R3	R0	K2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x80000000,  //  000B  RET	0
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(findsubval),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(ct_min),
    /* K8   */  be_nested_str_weak(ct_max),
    /* K9   */  be_nested_str_weak(set),
    /* K10  */  be_nested_str_weak(ct),
    /* K11  */  be_nested_str_weak(update_shadow),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(ct_X3A),
    /* K14  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x542202FF,  //  0005  LDINT	R8	768
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x7822002E,  //  0007  JMPF	R8	#0037
      0x54220009,  //  0008  LDINT	R8	10
      0x1C200E08,  //  0009  EQ	R8	R7	R8
      0x78220019,  //  000A  JMPF	R8	#0025
      0x8C200505,  //  000B  GETMET	R8	R2	K5
      0x58280006,  //  000C  LDCONST	R10	K6
      0x7C200400,  //  000D  CALL	R8	2
      0x88240107,  //  000E  GETMBR	R9	R0	K7
      0x14241009,  //  000F  LT	R9	R8	R9
      0x78260000,  //  0010  JMPF	R9	#0012
      0x88200107,  //  0011  GETMBR	R8	R0	K7
      0x88240108,  //  0012  GETMBR	R9	R0	K8
      0x24241009,  //  0013  GT	R9	R8	R9
      0x78260000,  //  0014  JMPF	R9	#0016
      0x88200108,  //  0015  GETMBR	R8	R0	K8
      0x8C240909,  //  0016  GETMET	R9	R4	K9
      0x602C0013,  //  0017  GETGBL	R11	G19
      0x7C2C0000,  //  0018  CALL	R11	0
      0x982E1408,  //  0019  SETIDX	R11	K10	R8
      0x7C240400,  //  001A  CALL	R9	2
      0x8C24010B,  //  001B  GETMET	R9	R0	K11
      0x7C240200,  //  001C  CALL	R9	1
      0x60240008,  //  001D  GETGBL	R9	G8
      0x5C281000,  //  001E  MOVE	R10	R8
      0x7C240200,  //  001F  CALL	R9	1
      0x00261A09,  //  0020  ADD	R9	K13	R9
      0x900E1809,  //  0021  SETMBR	R3	K12	R9
      0x50240200,  //  0022  LDBOOL	R9	1	0
      0x80041200,  //  0023  RET	1	R9
      0x70020010,  //  0024  JMP		#0036
      0x54220046,  //  0025  LDINT	R8	71
      0x1C200E08,  //  0026  EQ	R8	R7	R8
      0x78220002,  //  0027  JMPF	R8	#002B
      0x50200200,  //  0028  LDBOOL	R8	1	0
      0x80041000,  //  0029  RET	1	R8
      0x7002000A,  //  002A  JMP		#0036
      0x5422004A,  //  002B  LDINT	R8	75
      0x1C200E08,  //  002C  EQ	R8	R7	R8
      0x78220002,  //  002D  JMPF	R8	#0031
      0x50200200,  //  002E  LDBOOL	R8	1	0
      0x80041000,  //  002F  RET	1	R8
      0x70020004,  //  0030  JMP		#0036
      0x5422004B,  //  0031  LDINT	R8	76
      0x1C200E08,  //  0032  EQ	R8	R7	R8
      0x78220001,  //  0033  JMPF	R8	#0036
      0x50200200,  //  0034  LDBOOL	R8	1	0
      0x80041000,  //  0035  RET	1	R8
      0x70020008,  //  0036  JMP		#0040
      0x60200003,  //  0037  GETGBL	R8	G3
      0x5C240000,  //  0038  MOVE	R9	R0
      0x7C200200,  //  0039  CALL	R8	1
      0x8C20110E,  //  003A  GETMET	R8	R8	K14
      0x5C280200,  //  003B  MOVE	R10	R1
      0x5C2C0400,  //  003C  MOVE	R11	R2
      0x5C300600,  //  003D  MOVE	R12	R3
      0x7C200800,  //  003E  CALL	R8	4
      0x80041000,  //  003F  RET	1	R8
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light2_update_shadow,   /* name */
  be_nested_proto(
    9,                          /* nstack */
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
    ( &(const binstruction[28]) {  /* code */
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
      0x8C0C0504,  //  000A  GETMET	R3	R2	K4
      0x58140005,  //  000B  LDCONST	R5	K5
      0x4C180000,  //  000C  LDNIL	R6
      0x7C0C0600,  //  000D  CALL	R3	3
      0x4C100000,  //  000E  LDNIL	R4
      0x1C100604,  //  000F  EQ	R4	R3	R4
      0x78120000,  //  0010  JMPF	R4	#0012
      0x880C0106,  //  0011  GETMBR	R3	R0	K6
      0x88100106,  //  0012  GETMBR	R4	R0	K6
      0x20100604,  //  0013  NE	R4	R3	R4
      0x78120005,  //  0014  JMPF	R4	#001B
      0x8C100107,  //  0015  GETMET	R4	R0	K7
      0x4C180000,  //  0016  LDNIL	R6
      0x541E02FF,  //  0017  LDINT	R7	768
      0x54220006,  //  0018  LDINT	R8	7
      0x7C100800,  //  0019  CALL	R4	4
      0x90020C03,  //  001A  SETMBR	R0	K6	R3
      0x80000000,  //  001B  RET	0
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
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(268, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(shadow_ct, -1), be_const_var(0) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light2_read_attribute_closure) },
        { be_const_key_weak(ct_min, 8), be_const_var(1) },
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
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light2_init_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light2_invoke_request_closure) },
        { be_const_key_weak(update_ct_minmax, 6), be_const_closure(Matter_Plugin_Light2_update_ct_minmax_closure) },
        { be_const_key_weak(ct_max, -1), be_const_var(2) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Light2_update_shadow_closure) },
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
