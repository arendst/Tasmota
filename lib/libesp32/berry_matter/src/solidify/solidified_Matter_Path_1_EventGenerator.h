/* Solidification of Matter_Path_1_EventGenerator.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_EventGenerator;

/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_EventGenerator;
be_local_closure(class_Matter_EventGenerator_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventGenerator, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_pi
********************************************************************/
extern const bclass be_class_Matter_EventGenerator;
be_local_closure(class_Matter_EventGenerator_get_pi,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventGenerator, 
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(get_pi),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
extern const bclass be_class_Matter_EventGenerator;
be_local_closure(class_Matter_EventGenerator_start,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventGenerator, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(reset),
    /* K1   */  be_nested_str_weak(path_in_endpoint),
    /* K2   */  be_nested_str_weak(path_in_cluster),
    /* K3   */  be_nested_str_weak(path_in_event),
    /* K4   */  be_nested_str_weak(path_in_event_min),
    /* K5   */  be_nested_str_weak(event_no),
    /* K6   */  be_nested_str_weak(finished),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8C140100,  //  0000  GETMET	R5	R0	K0
      0x7C140200,  //  0001  CALL	R5	1
      0x90020201,  //  0002  SETMBR	R0	K1	R1
      0x90020402,  //  0003  SETMBR	R0	K2	R2
      0x90020603,  //  0004  SETMBR	R0	K3	R3
      0x90020804,  //  0005  SETMBR	R0	K4	R4
      0x90020A04,  //  0006  SETMBR	R0	K5	R4
      0x50140000,  //  0007  LDBOOL	R5	0	0
      0x90020C05,  //  0008  SETMBR	R0	K6	R5
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset
********************************************************************/
extern const bclass be_class_Matter_EventGenerator;
be_local_closure(class_Matter_EventGenerator_reset,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventGenerator, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(event_no),
    /* K1   */  be_nested_str_weak(finished),
    }),
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x50080200,  //  0002  LDBOOL	R2	1	0
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_finished
********************************************************************/
extern const bclass be_class_Matter_EventGenerator;
be_local_closure(class_Matter_EventGenerator_is_finished,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventGenerator, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(finished),
    }),
    be_str_weak(is_finished),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_is_match
********************************************************************/
extern const bclass be_class_Matter_EventGenerator;
be_local_closure(class_Matter_EventGenerator_event_is_match,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventGenerator, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(path_in_endpoint),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(path_in_cluster),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(path_in_event),
    /* K5   */  be_nested_str_weak(event_id),
    }),
    be_str_weak(event_is_match),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x50080200,  //  0000  LDBOOL	R2	1	0
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x4C100000,  //  0002  LDNIL	R4
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x780E0007,  //  0004  JMPF	R3	#000D
      0x780A0003,  //  0005  JMPF	R2	#000A
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x88100301,  //  0007  GETMBR	R4	R1	K1
      0x1C0C0604,  //  0008  EQ	R3	R3	R4
      0x740E0000,  //  0009  JMPT	R3	#000B
      0x500C0001,  //  000A  LDBOOL	R3	0	1
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0x5C080600,  //  000C  MOVE	R2	R3
      0x880C0102,  //  000D  GETMBR	R3	R0	K2
      0x4C100000,  //  000E  LDNIL	R4
      0x200C0604,  //  000F  NE	R3	R3	R4
      0x780E0007,  //  0010  JMPF	R3	#0019
      0x780A0003,  //  0011  JMPF	R2	#0016
      0x880C0102,  //  0012  GETMBR	R3	R0	K2
      0x88100303,  //  0013  GETMBR	R4	R1	K3
      0x1C0C0604,  //  0014  EQ	R3	R3	R4
      0x740E0000,  //  0015  JMPT	R3	#0017
      0x500C0001,  //  0016  LDBOOL	R3	0	1
      0x500C0200,  //  0017  LDBOOL	R3	1	0
      0x5C080600,  //  0018  MOVE	R2	R3
      0x880C0104,  //  0019  GETMBR	R3	R0	K4
      0x4C100000,  //  001A  LDNIL	R4
      0x200C0604,  //  001B  NE	R3	R3	R4
      0x780E0007,  //  001C  JMPF	R3	#0025
      0x780A0003,  //  001D  JMPF	R2	#0022
      0x880C0104,  //  001E  GETMBR	R3	R0	K4
      0x88100305,  //  001F  GETMBR	R4	R1	K5
      0x1C0C0604,  //  0020  EQ	R3	R3	R4
      0x740E0000,  //  0021  JMPT	R3	#0023
      0x500C0001,  //  0022  LDBOOL	R3	0	1
      0x500C0200,  //  0023  LDBOOL	R3	1	0
      0x5C080600,  //  0024  MOVE	R2	R3
      0x80040400,  //  0025  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next_event
********************************************************************/
extern const bclass be_class_Matter_EventGenerator;
be_local_closure(class_Matter_EventGenerator_next_event,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventGenerator, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(finished),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(events),
    /* K3   */  be_nested_str_weak(find_min_no),
    /* K4   */  be_nested_str_weak(event_no),
    /* K5   */  be_nested_str_weak(reset),
    /* K6   */  be_nested_str_weak(event_is_match),
    }),
    be_str_weak(next_event),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060001,  //  0001  JMPF	R1	#0004
      0x4C040000,  //  0002  LDNIL	R1
      0x80040200,  //  0003  RET	1	R1
      0x50040200,  //  0004  LDBOOL	R1	1	0
      0x78060013,  //  0005  JMPF	R1	#001A
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x88040302,  //  0007  GETMBR	R1	R1	K2
      0x8C040303,  //  0008  GETMET	R1	R1	K3
      0x880C0104,  //  0009  GETMBR	R3	R0	K4
      0x7C040400,  //  000A  CALL	R1	2
      0x4C080000,  //  000B  LDNIL	R2
      0x1C080202,  //  000C  EQ	R2	R1	R2
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C080105,  //  000E  GETMET	R2	R0	K5
      0x7C080200,  //  000F  CALL	R2	1
      0x4C080000,  //  0010  LDNIL	R2
      0x80040400,  //  0011  RET	1	R2
      0x88080304,  //  0012  GETMBR	R2	R1	K4
      0x90020802,  //  0013  SETMBR	R0	K4	R2
      0x8C080106,  //  0014  GETMET	R2	R0	K6
      0x5C100200,  //  0015  MOVE	R4	R1
      0x7C080400,  //  0016  CALL	R2	2
      0x780A0000,  //  0017  JMPF	R2	#0019
      0x80040200,  //  0018  RET	1	R1
      0x7001FFE9,  //  0019  JMP		#0004
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: restart_from
********************************************************************/
extern const bclass be_class_Matter_EventGenerator;
be_local_closure(class_Matter_EventGenerator_restart_from,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventGenerator, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(finished),
    /* K1   */  be_nested_str_weak(event_no),
    }),
    be_str_weak(restart_from),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x50080000,  //  0000  LDBOOL	R2	0	0
      0x90020002,  //  0001  SETMBR	R0	K0	R2
      0x90020201,  //  0002  SETMBR	R0	K1	R1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_EventGenerator
********************************************************************/
be_local_class(Matter_EventGenerator,
    7,
    NULL,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_EventGenerator_init_closure) },
        { be_const_key_weak(path_in_endpoint, -1), be_const_var(1) },
        { be_const_key_weak(get_pi, -1), be_const_closure(class_Matter_EventGenerator_get_pi_closure) },
        { be_const_key_weak(path_in_event, -1), be_const_var(3) },
        { be_const_key_weak(finished, -1), be_const_var(6) },
        { be_const_key_weak(path_in_event_min, -1), be_const_var(4) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(event_no, 9), be_const_var(5) },
        { be_const_key_weak(is_finished, -1), be_const_closure(class_Matter_EventGenerator_is_finished_closure) },
        { be_const_key_weak(restart_from, 5), be_const_closure(class_Matter_EventGenerator_restart_from_closure) },
        { be_const_key_weak(event_is_match, -1), be_const_closure(class_Matter_EventGenerator_event_is_match_closure) },
        { be_const_key_weak(start, 13), be_const_closure(class_Matter_EventGenerator_start_closure) },
        { be_const_key_weak(reset, 7), be_const_closure(class_Matter_EventGenerator_reset_closure) },
        { be_const_key_weak(next_event, -1), be_const_closure(class_Matter_EventGenerator_next_event_closure) },
        { be_const_key_weak(path_in_cluster, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_EventGenerator)
);
/********************************************************************/
/* End of solidification */
