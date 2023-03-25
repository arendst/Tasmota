/* Solidification of Matter_Plugin.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_read_attribute,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x80040600,  //  0001  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_attribute
********************************************************************/
be_local_closure(Matter_Plugin_write_attribute,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(write_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x80040800,  //  0001  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_invoke_request,   /* name */
  be_nested_proto(
    5,                          /* nstack */
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
    ( &(const binstruction[ 2]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x80040800,  //  0001  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Plugin_every_second,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_attribute
********************************************************************/
be_local_closure(Matter_Plugin_subscribe_attribute,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(subscribe_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x80040A00,  //  0001  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_event
********************************************************************/
be_local_closure(Matter_Plugin_subscribe_event,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(subscribe_event),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x80040A00,  //  0001  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_event
********************************************************************/
be_local_closure(Matter_Plugin_read_event,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(read_event),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x80040A00,  //  0001  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_endpoint
********************************************************************/
be_local_closure(Matter_Plugin_get_endpoint,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(endpoint),
    }),
    be_str_weak(get_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(clusters),
    /* K3   */  be_nested_str_weak(EMPTY_LIST),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x90020403,  //  0003  SETMBR	R0	K2	R3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has
********************************************************************/
be_local_closure(Matter_Plugin_has,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(clusters),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(endpoints),
    /* K3   */  be_nested_str_weak(find),
    }),
    be_str_weak(has),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0006,  //  0004  JMPF	R3	#000C
      0x880C0102,  //  0005  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x5C140400,  //  0007  MOVE	R5	R2
      0x7C0C0400,  //  0008  CALL	R3	2
      0x4C100000,  //  0009  LDNIL	R4
      0x200C0604,  //  000A  NE	R3	R3	R4
      0x740E0000,  //  000B  JMPT	R3	#000D
      0x500C0001,  //  000C  LDBOOL	R3	0	1
      0x500C0200,  //  000D  LDBOOL	R3	1	0
      0x80040600,  //  000E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: timed_request
********************************************************************/
be_local_closure(Matter_Plugin_timed_request,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x80040800,  //  0001  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_attribute_list
********************************************************************/
be_local_closure(Matter_Plugin_get_attribute_list,   /* name */
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
    /* K0   */  be_nested_str_weak(clusters),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(EMPTY_LIST),
    }),
    be_str_weak(get_attribute_list),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140400,  //  0002  MOVE	R5	R2
      0x88180102,  //  0003  GETMBR	R6	R0	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x80040600,  //  0005  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_cluster_list
********************************************************************/
be_local_closure(Matter_Plugin_get_cluster_list,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(clusters),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(push),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_cluster_list),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x8C100901,  //  0004  GETMET	R4	R4	K1
      0x7C100200,  //  0005  CALL	R4	1
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x8C140502,  //  000A  GETMET	R5	R2	K2
      0x5C1C0800,  //  000B  MOVE	R7	R4
      0x7C140400,  //  000C  CALL	R5	2
      0x7001FFF9,  //  000D  JMP		#0008
      0x580C0003,  //  000E  LDCONST	R3	K3
      0xAC0C0200,  //  000F  CATCH	R3	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x80040400,  //  0011  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_cluster_map
********************************************************************/
be_local_closure(Matter_Plugin_get_cluster_map,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(clusters),
    }),
    be_str_weak(get_cluster_map),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(Matter_Plugin_attribute_updated,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(endpoint),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(attribute_updated),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140205,  //  0001  EQ	R5	R1	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x88140101,  //  0004  GETMBR	R5	R0	K1
      0x8C140B02,  //  0005  GETMET	R5	R5	K2
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x5C200400,  //  0007  MOVE	R8	R2
      0x5C240600,  //  0008  MOVE	R9	R3
      0x5C280800,  //  0009  MOVE	R10	R4
      0x7C140A00,  //  000A  CALL	R5	5
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(Matter_Plugin_parse_sensors,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin
********************************************************************/
be_local_class(Matter_Plugin,
    3,
    NULL,
    be_nested_map(21,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_read_attribute_closure) },
        { be_const_key_weak(parse_sensors, 13), be_const_closure(Matter_Plugin_parse_sensors_closure) },
        { be_const_key_weak(has, -1), be_const_closure(Matter_Plugin_has_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Plugin_every_second_closure) },
        { be_const_key_weak(write_attribute, 9), be_const_closure(Matter_Plugin_write_attribute_closure) },
        { be_const_key_weak(subscribe_event, -1), be_const_closure(Matter_Plugin_subscribe_event_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(endpoint, -1), be_const_var(1) },
        { be_const_key_weak(EMPTY_LIST, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(0,
    ( (struct bvalue*) &(const bvalue[]) {
    }))    ) } )) },
        { be_const_key_weak(attribute_updated, 20), be_const_closure(Matter_Plugin_attribute_updated_closure) },
        { be_const_key_weak(get_endpoint, 2), be_const_closure(Matter_Plugin_get_endpoint_closure) },
        { be_const_key_weak(EMPTY_MAP, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(0,
    ( (struct bmapnode*) &(const bmapnode[]) {
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_init_closure) },
        { be_const_key_weak(get_cluster_map, 17), be_const_closure(Matter_Plugin_get_cluster_map_closure) },
        { be_const_key_weak(invoke_request, 18), be_const_closure(Matter_Plugin_invoke_request_closure) },
        { be_const_key_weak(read_event, 1), be_const_closure(Matter_Plugin_read_event_closure) },
        { be_const_key_weak(get_cluster_list, -1), be_const_closure(Matter_Plugin_get_cluster_list_closure) },
        { be_const_key_weak(get_attribute_list, -1), be_const_closure(Matter_Plugin_get_attribute_list_closure) },
        { be_const_key_weak(timed_request, -1), be_const_closure(Matter_Plugin_timed_request_closure) },
        { be_const_key_weak(clusters, -1), be_const_var(2) },
        { be_const_key_weak(subscribe_attribute, -1), be_const_closure(Matter_Plugin_subscribe_attribute_closure) },
    })),
    be_str_weak(Matter_Plugin)
);
/*******************************************************************/

void be_load_Matter_Plugin_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin);
    be_setglobal(vm, "Matter_Plugin");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
