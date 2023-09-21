/* Solidification of Matter_Plugin_2_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light0;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light0_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
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
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(BOOL),
    /* K8   */  be_nested_str_weak(shadow_onoff),
    /* K9   */  be_nested_str_weak(U4),
    /* K10  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E001B,  //  0006  JMPF	R7	#0023
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0005,  //  000A  JMPF	R7	#0011
      0x8C1C0706,  //  000B  GETMET	R7	R3	K6
      0x88240907,  //  000C  GETMBR	R9	R4	K7
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x70020010,  //  0010  JMP		#0022
      0x541EFFFB,  //  0011  LDINT	R7	65532
      0x1C1C0C07,  //  0012  EQ	R7	R6	R7
      0x781E0005,  //  0013  JMPF	R7	#001A
      0x8C1C0706,  //  0014  GETMET	R7	R3	K6
      0x88240909,  //  0015  GETMBR	R9	R4	K9
      0x58280005,  //  0016  LDCONST	R10	K5
      0x7C1C0600,  //  0017  CALL	R7	3
      0x80040E00,  //  0018  RET	1	R7
      0x70020007,  //  0019  JMP		#0022
      0x541EFFFC,  //  001A  LDINT	R7	65533
      0x1C1C0C07,  //  001B  EQ	R7	R6	R7
      0x781E0004,  //  001C  JMPF	R7	#0022
      0x8C1C0706,  //  001D  GETMET	R7	R3	K6
      0x88240909,  //  001E  GETMBR	R9	R4	K9
      0x542A0003,  //  001F  LDINT	R10	4
      0x7C1C0600,  //  0020  CALL	R7	3
      0x80040E00,  //  0021  RET	1	R7
      0x70020008,  //  0022  JMP		#002C
      0x601C0003,  //  0023  GETGBL	R7	G3
      0x5C200000,  //  0024  MOVE	R8	R0
      0x7C1C0200,  //  0025  CALL	R7	1
      0x8C1C0F0A,  //  0026  GETMET	R7	R7	K10
      0x5C240200,  //  0027  MOVE	R9	R1
      0x5C280400,  //  0028  MOVE	R10	R2
      0x5C2C0600,  //  0029  MOVE	R11	R3
      0x7C1C0800,  //  002A  CALL	R7	4
      0x80040E00,  //  002B  RET	1	R7
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_onoff
********************************************************************/
be_local_closure(Matter_Plugin_Light0_set_onoff,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(VIRTUAL),
    /* K1   */  be_nested_str_weak(light),
    /* K2   */  be_nested_str_weak(set),
    /* K3   */  be_nested_str_weak(power),
    /* K4   */  be_nested_str_weak(update_shadow),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    /* K6   */  be_nested_str_weak(attribute_updated),
    /* K7   */  be_const_int(0),
    }),
    be_str_weak(set_onoff),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0008,  //  0001  JMPT	R2	#000B
      0xA40A0200,  //  0002  IMPORT	R2	K1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x60140013,  //  0004  GETGBL	R5	G19
      0x7C140000,  //  0005  CALL	R5	0
      0x98160601,  //  0006  SETIDX	R5	K3	R1
      0x7C0C0400,  //  0007  CALL	R3	2
      0x8C0C0104,  //  0008  GETMET	R3	R0	K4
      0x7C0C0200,  //  0009  CALL	R3	1
      0x70020007,  //  000A  JMP		#0013
      0x88080105,  //  000B  GETMBR	R2	R0	K5
      0x20080202,  //  000C  NE	R2	R1	R2
      0x780A0004,  //  000D  JMPF	R2	#0013
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x54120005,  //  000F  LDINT	R4	6
      0x58140007,  //  0010  LDCONST	R5	K7
      0x7C080600,  //  0011  CALL	R2	3
      0x90020A01,  //  0012  SETMBR	R0	K5	R1
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(Matter_Plugin_Light0_update_virtual,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(Power),
    /* K2   */  be_nested_str_weak(set_onoff),
    /* K3   */  be_nested_str_weak(update_virtual),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0004,  //  0005  JMPF	R3	#000B
      0x8C0C0102,  //  0006  GETMET	R3	R0	K2
      0x60140017,  //  0007  GETGBL	R5	G23
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x600C0003,  //  000B  GETGBL	R3	G3
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C0703,  //  000E  GETMET	R3	R3	K3
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80000000,  //  0011  RET	0
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(VIRTUAL),
    /* K1   */  be_nested_str_weak(light),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(power),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    /* K6   */  be_nested_str_weak(attribute_updated),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060011,  //  0001  JMPT	R1	#0014
      0xA4060200,  //  0002  IMPORT	R1	K1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x200C0403,  //  0006  NE	R3	R2	R3
      0x780E000B,  //  0007  JMPF	R3	#0014
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x58140004,  //  0009  LDCONST	R5	K4
      0x4C180000,  //  000A  LDNIL	R6
      0x7C0C0600,  //  000B  CALL	R3	3
      0x88100105,  //  000C  GETMBR	R4	R0	K5
      0x20100604,  //  000D  NE	R4	R3	R4
      0x78120004,  //  000E  JMPF	R4	#0014
      0x8C100106,  //  000F  GETMET	R4	R0	K6
      0x541A0005,  //  0010  LDINT	R6	6
      0x581C0007,  //  0011  LDCONST	R7	K7
      0x7C100600,  //  0012  CALL	R4	3
      0x90020A03,  //  0013  SETMBR	R0	K5	R3
      0x60040003,  //  0014  GETGBL	R1	G3
      0x5C080000,  //  0015  MOVE	R2	R0
      0x7C040200,  //  0016  CALL	R1	1
      0x8C040308,  //  0017  GETMET	R1	R1	K8
      0x7C040200,  //  0018  CALL	R1	1
      0x80000000,  //  0019  RET	0
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
    /* K7   */  be_nested_str_weak(set_onoff),
    /* K8   */  be_nested_str_weak(publish_command),
    /* K9   */  be_nested_str_weak(Power),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220005,  //  0005  LDINT	R8	6
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x7822002B,  //  0007  JMPF	R8	#0034
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x78220009,  //  000B  JMPF	R8	#0016
      0x8C200107,  //  000C  GETMET	R8	R0	K7
      0x50280000,  //  000D  LDBOOL	R10	0	0
      0x7C200400,  //  000E  CALL	R8	2
      0x8C200108,  //  000F  GETMET	R8	R0	K8
      0x58280009,  //  0010  LDCONST	R10	K9
      0x582C0006,  //  0011  LDCONST	R11	K6
      0x7C200600,  //  0012  CALL	R8	3
      0x50200200,  //  0013  LDBOOL	R8	1	0
      0x80041000,  //  0014  RET	1	R8
      0x7002001D,  //  0015  JMP		#0034
      0x1C200F0A,  //  0016  EQ	R8	R7	K10
      0x78220009,  //  0017  JMPF	R8	#0022
      0x8C200107,  //  0018  GETMET	R8	R0	K7
      0x50280200,  //  0019  LDBOOL	R10	1	0
      0x7C200400,  //  001A  CALL	R8	2
      0x8C200108,  //  001B  GETMET	R8	R0	K8
      0x58280009,  //  001C  LDCONST	R10	K9
      0x582C000A,  //  001D  LDCONST	R11	K10
      0x7C200600,  //  001E  CALL	R8	3
      0x50200200,  //  001F  LDBOOL	R8	1	0
      0x80041000,  //  0020  RET	1	R8
      0x70020011,  //  0021  JMP		#0034
      0x1C200F0B,  //  0022  EQ	R8	R7	K11
      0x7822000F,  //  0023  JMPF	R8	#0034
      0x8C200107,  //  0024  GETMET	R8	R0	K7
      0x8828010C,  //  0025  GETMBR	R10	R0	K12
      0x782A0000,  //  0026  JMPF	R10	#0028
      0x50280001,  //  0027  LDBOOL	R10	0	1
      0x50280200,  //  0028  LDBOOL	R10	1	0
      0x7C200400,  //  0029  CALL	R8	2
      0x8C200108,  //  002A  GETMET	R8	R0	K8
      0x58280009,  //  002B  LDCONST	R10	K9
      0x882C010C,  //  002C  GETMBR	R11	R0	K12
      0x782E0001,  //  002D  JMPF	R11	#0030
      0x582C000A,  //  002E  LDCONST	R11	K10
      0x70020000,  //  002F  JMP		#0031
      0x582C0006,  //  0030  LDCONST	R11	K6
      0x7C200600,  //  0031  CALL	R8	3
      0x50200200,  //  0032  LDBOOL	R8	1	0
      0x80041000,  //  0033  RET	1	R8
      0x80000000,  //  0034  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_val_i
********************************************************************/
be_local_closure(Matter_Plugin_Light0_find_val_i,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Plugin_Light0),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(find_key_i),
    /* K3   */  be_nested_str_weak(find),
    }),
    be_str_weak(find_val_i),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x5C140000,  //  0003  MOVE	R5	R0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C0C0600,  //  0005  CALL	R3	3
      0x8C100103,  //  0006  GETMET	R4	R0	K3
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0x80040800,  //  0009  RET	1	R4
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
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, 9), be_const_closure(Matter_Plugin_Light0_read_attribute_closure) },
        { be_const_key_weak(UPDATE_TIME, 4), be_const_int(250) },
        { be_const_key_weak(update_virtual, 1), be_const_closure(Matter_Plugin_Light0_update_virtual_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, 13), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
    }))    ) } )) },
        { be_const_key_weak(find_val_i, -1), be_const_static_closure(Matter_Plugin_Light0_find_val_i_closure) },
        { be_const_key_weak(init, 11), be_const_closure(Matter_Plugin_Light0_init_closure) },
        { be_const_key_weak(set_onoff, 12), be_const_closure(Matter_Plugin_Light0_set_onoff_closure) },
        { be_const_key_weak(update_shadow, 8), be_const_closure(Matter_Plugin_Light0_update_shadow_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light0_invoke_request_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(17),
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
    }))    ) } )) },
        { be_const_key_int(3, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(29, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(256, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X200_X20On) },
        { be_const_key_weak(shadow_onoff, -1), be_const_var(0) },
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
