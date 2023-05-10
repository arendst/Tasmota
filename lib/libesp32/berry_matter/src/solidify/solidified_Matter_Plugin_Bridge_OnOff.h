/* Solidification of Matter_Plugin_Bridge_OnOff.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_OnOff;

/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_OnOff__X3Clambda_X3E,   /* name */
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
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_OnOff_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(init),
    /* K2   */  be_nested_str_weak(shadow_onoff),
    /* K3   */  be_nested_str_weak(tasmota_relay_index),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(ARG),
    /* K6   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x60140003,  //  0001  GETGBL	R5	G3
      0x5C180000,  //  0002  MOVE	R6	R0
      0x7C140200,  //  0003  CALL	R5	1
      0x8C140B01,  //  0004  GETMET	R5	R5	K1
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x5C200400,  //  0006  MOVE	R8	R2
      0x5C240600,  //  0007  MOVE	R9	R3
      0x7C140800,  //  0008  CALL	R5	4
      0x50140000,  //  0009  LDBOOL	R5	0	0
      0x90020405,  //  000A  SETMBR	R0	K2	R5
      0x8C140704,  //  000B  GETMET	R5	R3	K4
      0x881C0105,  //  000C  GETMBR	R7	R0	K5
      0x7C140400,  //  000D  CALL	R5	2
      0x90020605,  //  000E  SETMBR	R0	K3	R5
      0x88140103,  //  000F  GETMBR	R5	R0	K3
      0x4C180000,  //  0010  LDNIL	R6
      0x1C140A06,  //  0011  EQ	R5	R5	R6
      0x78160000,  //  0012  JMPF	R5	#0014
      0x90020706,  //  0013  SETMBR	R0	K3	K6
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_onoff
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_OnOff_set_onoff,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(call_remote),
    /* K1   */  be_nested_str_weak(Power),
    /* K2   */  be_nested_str_weak(1),
    /* K3   */  be_nested_str_weak(0),
    /* K4   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(set_onoff),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x78060001,  //  0002  JMPF	R1	#0005
      0x58140002,  //  0003  LDCONST	R5	K2
      0x70020000,  //  0004  JMP		#0006
      0x58140003,  //  0005  LDCONST	R5	K3
      0x7C080600,  //  0006  CALL	R2	3
      0x8C080104,  //  0007  GETMET	R2	R0	K4
      0x7C080200,  //  0008  CALL	R2	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_OnOff_invoke_request,   /* name */
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
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_OnOff_read_attribute,   /* name */
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
be_local_closure(Matter_Plugin_Bridge_OnOff_update_shadow,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_status_11),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(StatusSTS),
    /* K3   */  be_nested_str_weak(POWER),
    /* K4   */  be_nested_str_weak(ON),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    /* K6   */  be_nested_str_weak(attribute_updated),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x78060018,  //  0002  JMPF	R1	#001C
      0x8C080301,  //  0003  GETMET	R2	R1	K1
      0x58100002,  //  0004  LDCONST	R4	K2
      0x60140013,  //  0005  GETGBL	R5	G19
      0x7C140000,  //  0006  CALL	R5	0
      0x7C080600,  //  0007  CALL	R2	3
      0x5C040400,  //  0008  MOVE	R1	R2
      0x8C080301,  //  0009  GETMET	R2	R1	K1
      0x58100003,  //  000A  LDCONST	R4	K3
      0x7C080400,  //  000B  CALL	R2	2
      0x1C080504,  //  000C  EQ	R2	R2	K4
      0x880C0105,  //  000D  GETMBR	R3	R0	K5
      0x4C100000,  //  000E  LDNIL	R4
      0x200C0604,  //  000F  NE	R3	R3	R4
      0x780E0009,  //  0010  JMPF	R3	#001B
      0x880C0105,  //  0011  GETMBR	R3	R0	K5
      0x60100017,  //  0012  GETGBL	R4	G23
      0x5C140400,  //  0013  MOVE	R5	R2
      0x7C100200,  //  0014  CALL	R4	1
      0x200C0604,  //  0015  NE	R3	R3	R4
      0x780E0003,  //  0016  JMPF	R3	#001B
      0x8C0C0106,  //  0017  GETMET	R3	R0	K6
      0x54160005,  //  0018  LDINT	R5	6
      0x58180007,  //  0019  LDCONST	R6	K7
      0x7C0C0600,  //  001A  CALL	R3	3
      0x90020A02,  //  001B  SETMBR	R0	K5	R2
      0x60080003,  //  001C  GETGBL	R2	G3
      0x5C0C0000,  //  001D  MOVE	R3	R0
      0x7C080200,  //  001E  CALL	R2	1
      0x8C080508,  //  001F  GETMET	R2	R2	K8
      0x7C080200,  //  0020  CALL	R2	1
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_OnOff
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_HTTP;
be_local_class(Matter_Plugin_Bridge_OnOff,
    3,
    &be_class_Matter_Plugin_Bridge_HTTP,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(CLUSTERS, 11), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        { be_const_key_weak(ARG, -1), be_nested_str_weak(relay) },
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(Matter_Plugin_Bridge_OnOff__X3Clambda_X3E_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_relay) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Bridge_OnOff_update_shadow_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Bridge_OnOff_init_closure) },
        { be_const_key_weak(shadow_onoff, 13), be_const_var(2) },
        { be_const_key_weak(invoke_request, 4), be_const_closure(Matter_Plugin_Bridge_OnOff_invoke_request_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(_X26_X23x1F517_X3B_X20Relay) },
        { be_const_key_weak(tasmota_http, 12), be_const_var(0) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(266, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Bridge_OnOff_read_attribute_closure) },
        { be_const_key_weak(tasmota_relay_index, -1), be_const_var(1) },
        { be_const_key_weak(set_onoff, -1), be_const_closure(Matter_Plugin_Bridge_OnOff_set_onoff_closure) },
    })),
    be_str_weak(Matter_Plugin_Bridge_OnOff)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_OnOff_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_OnOff);
    be_setglobal(vm, "Matter_Plugin_Bridge_OnOff");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
