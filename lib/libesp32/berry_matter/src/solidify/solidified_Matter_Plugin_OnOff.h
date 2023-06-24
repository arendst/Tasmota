/* Solidification of Matter_Plugin_OnOff.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_OnOff;

/********************************************************************
** Solidified function: set_onoff
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_set_onoff,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_power),
    /* K2   */  be_nested_str_weak(tasmota_relay_index),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(set_onoff),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x04100903,  //  0003  SUB	R4	R4	K3
      0x60140017,  //  0004  GETGBL	R5	G23
      0x5C180200,  //  0005  MOVE	R6	R1
      0x7C140200,  //  0006  CALL	R5	1
      0x7C080600,  //  0007  CALL	R2	3
      0x8C080104,  //  0008  GETMET	R2	R0	K4
      0x7C080200,  //  0009  CALL	R2	1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_init,   /* name */
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
be_local_closure(Matter_Plugin_OnOff_update_shadow,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(get_power),
    /* K2   */  be_nested_str_weak(tasmota_relay_index),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(shadow_onoff),
    /* K5   */  be_nested_str_weak(attribute_updated),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x040C0703,  //  0003  SUB	R3	R3	K3
      0x7C040400,  //  0004  CALL	R1	2
      0x4C080000,  //  0005  LDNIL	R2
      0x20080202,  //  0006  NE	R2	R1	R2
      0x780A000E,  //  0007  JMPF	R2	#0017
      0x88080104,  //  0008  GETMBR	R2	R0	K4
      0x4C0C0000,  //  0009  LDNIL	R3
      0x20080403,  //  000A  NE	R2	R2	R3
      0x780A0009,  //  000B  JMPF	R2	#0016
      0x88080104,  //  000C  GETMBR	R2	R0	K4
      0x600C0017,  //  000D  GETGBL	R3	G23
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C0C0200,  //  000F  CALL	R3	1
      0x20080403,  //  0010  NE	R2	R2	R3
      0x780A0003,  //  0011  JMPF	R2	#0016
      0x8C080105,  //  0012  GETMET	R2	R0	K5
      0x54120005,  //  0013  LDINT	R4	6
      0x58140006,  //  0014  LDCONST	R5	K6
      0x7C080600,  //  0015  CALL	R2	3
      0x90020801,  //  0016  SETMBR	R0	K4	R1
      0x60080003,  //  0017  GETGBL	R2	G3
      0x5C0C0000,  //  0018  MOVE	R3	R0
      0x7C080200,  //  0019  CALL	R2	1
      0x8C080507,  //  001A  GETMET	R2	R2	K7
      0x7C080200,  //  001B  CALL	R2	1
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_parse_configuration,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_relay_index),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x8C0C0301,  //  0001  GETMET	R3	R1	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x58180003,  //  0003  LDCONST	R6	K3
      0x7C0C0600,  //  0004  CALL	R3	3
      0x7C080200,  //  0005  CALL	R2	1
      0x90020002,  //  0006  SETMBR	R0	K0	R2
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x18080504,  //  0008  LE	R2	R2	K4
      0x780A0000,  //  0009  JMPF	R2	#000B
      0x90020103,  //  000A  SETMBR	R0	K0	K3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_read_attribute,   /* name */
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
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_OnOff_invoke_request,   /* name */
  be_nested_proto(
    10,                          /* nstack */
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
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(set_onoff),
    /* K7   */  be_nested_str_weak(update_shadow),
    /* K8   */  be_const_int(1),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0021,  //  0006  JMPF	R7	#0029
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0007,  //  000A  JMPF	R7	#0013
      0x8C1C0106,  //  000B  GETMET	R7	R0	K6
      0x50240000,  //  000C  LDBOOL	R9	0	0
      0x7C1C0400,  //  000D  CALL	R7	2
      0x8C1C0107,  //  000E  GETMET	R7	R0	K7
      0x7C1C0200,  //  000F  CALL	R7	1
      0x501C0200,  //  0010  LDBOOL	R7	1	0
      0x80040E00,  //  0011  RET	1	R7
      0x70020015,  //  0012  JMP		#0029
      0x1C1C0D08,  //  0013  EQ	R7	R6	K8
      0x781E0007,  //  0014  JMPF	R7	#001D
      0x8C1C0106,  //  0015  GETMET	R7	R0	K6
      0x50240200,  //  0016  LDBOOL	R9	1	0
      0x7C1C0400,  //  0017  CALL	R7	2
      0x8C1C0107,  //  0018  GETMET	R7	R0	K7
      0x7C1C0200,  //  0019  CALL	R7	1
      0x501C0200,  //  001A  LDBOOL	R7	1	0
      0x80040E00,  //  001B  RET	1	R7
      0x7002000B,  //  001C  JMP		#0029
      0x1C1C0D09,  //  001D  EQ	R7	R6	K9
      0x781E0009,  //  001E  JMPF	R7	#0029
      0x8C1C0106,  //  001F  GETMET	R7	R0	K6
      0x8824010A,  //  0020  GETMBR	R9	R0	K10
      0x78260000,  //  0021  JMPF	R9	#0023
      0x50240001,  //  0022  LDBOOL	R9	0	1
      0x50240200,  //  0023  LDBOOL	R9	1	0
      0x7C1C0400,  //  0024  CALL	R7	2
      0x8C1C0107,  //  0025  GETMET	R7	R0	K7
      0x7C1C0200,  //  0026  CALL	R7	1
      0x501C0200,  //  0027  LDBOOL	R7	1	0
      0x80040E00,  //  0028  RET	1	R7
      0x80000000,  //  0029  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_OnOff__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(_X3Clambda_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040009,  //  0000  GETGBL	R1	G9
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_OnOff
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_OnOff,
    2,
    &be_class_Matter_Plugin_Device,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(shadow_onoff, 4), be_const_var(1) },
        { be_const_key_weak(tasmota_relay_index, 12), be_const_var(0) },
        { be_const_key_weak(set_onoff, -1), be_const_closure(Matter_Plugin_OnOff_set_onoff_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_OnOff_init_closure) },
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(Matter_Plugin_OnOff__X3Clambda_X3E_closure) },
        { be_const_key_weak(update_shadow, 15), be_const_closure(Matter_Plugin_OnOff_update_shadow_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Relay) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(relay) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(250) },
        { be_const_key_weak(CLUSTERS, 14), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        { be_const_key_weak(TYPES, 9), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(266, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_OnOff_invoke_request_closure) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Enter_X20Relay_X3Cx_X3E_X20number) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(relay) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_OnOff_read_attribute_closure) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(Matter_Plugin_OnOff_parse_configuration_closure) },
    })),
    be_str_weak(Matter_Plugin_OnOff)
);
/*******************************************************************/

void be_load_Matter_Plugin_OnOff_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_OnOff);
    be_setglobal(vm, "Matter_Plugin_OnOff");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
