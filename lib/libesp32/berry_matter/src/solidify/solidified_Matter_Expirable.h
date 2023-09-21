/* Solidification of Matter_Expirable.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Expirable;

/********************************************************************
** Solidified function: before_remove
********************************************************************/
be_local_closure(Matter_Expirable_before_remove,   /* name */
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
    be_str_weak(before_remove),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_no_expiration
********************************************************************/
be_local_closure(Matter_Expirable_set_no_expiration,   /* name */
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
    /* K0   */  be_nested_str_weak(_expiration),
    }),
    be_str_weak(set_no_expiration),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Expirable_init,   /* name */
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
    /* K0   */  be_nested_str_weak(_persist),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_expire_time
********************************************************************/
be_local_closure(Matter_Expirable_set_expire_time,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_expiration),
    }),
    be_str_weak(set_expire_time),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has_expired
********************************************************************/
be_local_closure(Matter_Expirable_has_expired,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(rtc_utc),
    /* K2   */  be_nested_str_weak(_expiration),
    }),
    be_str_weak(has_expired),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0003,  //  0002  JMPF	R2	#0007
      0xB80A0000,  //  0003  GETNGBL	R2	K0
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x5C040400,  //  0006  MOVE	R1	R2
      0x88080102,  //  0007  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0008  LDNIL	R3
      0x20080403,  //  0009  NE	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x88080102,  //  000B  GETMBR	R2	R0	K2
      0x28080202,  //  000C  GE	R2	R1	R2
      0x80040400,  //  000D  RET	1	R2
      0x50080000,  //  000E  LDBOOL	R2	0	0
      0x80040400,  //  000F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_parent_list
********************************************************************/
be_local_closure(Matter_Expirable_set_parent_list,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_list),
    }),
    be_str_weak(set_parent_list),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: hydrate_post
********************************************************************/
be_local_closure(Matter_Expirable_hydrate_post,   /* name */
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
    be_str_weak(hydrate_post),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_expire_in_seconds
********************************************************************/
be_local_closure(Matter_Expirable_set_expire_in_seconds,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(rtc_utc),
    /* K2   */  be_nested_str_weak(set_expire_time),
    }),
    be_str_weak(set_expire_in_seconds),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0203,  //  0001  EQ	R3	R1	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x80000600,  //  0003  RET	0
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C0C0403,  //  0005  EQ	R3	R2	R3
      0x780E0003,  //  0006  JMPF	R3	#000B
      0xB80E0000,  //  0007  GETNGBL	R3	K0
      0x8C0C0701,  //  0008  GETMET	R3	R3	K1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x5C080600,  //  000A  MOVE	R2	R3
      0x8C0C0102,  //  000B  GETMET	R3	R0	K2
      0x00140401,  //  000C  ADD	R5	R2	R1
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_parent_list
********************************************************************/
be_local_closure(Matter_Expirable_get_parent_list,   /* name */
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
    /* K0   */  be_nested_str_weak(_list),
    }),
    be_str_weak(get_parent_list),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: does_persist
********************************************************************/
be_local_closure(Matter_Expirable_does_persist,   /* name */
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
    /* K0   */  be_nested_str_weak(_persist),
    }),
    be_str_weak(does_persist),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_persist
********************************************************************/
be_local_closure(Matter_Expirable_set_persist,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(_persist),
    }),
    be_str_weak(set_persist),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080017,  //  0000  GETGBL	R2	G23
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: persist_pre
********************************************************************/
be_local_closure(Matter_Expirable_persist_pre,   /* name */
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
    be_str_weak(persist_pre),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: persist_post
********************************************************************/
be_local_closure(Matter_Expirable_persist_post,   /* name */
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
    be_str_weak(persist_post),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Expirable
********************************************************************/
be_local_class(Matter_Expirable,
    3,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(_expiration, -1), be_const_var(2) },
        { be_const_key_weak(set_no_expiration, 9), be_const_closure(Matter_Expirable_set_no_expiration_closure) },
        { be_const_key_weak(persist_post, -1), be_const_closure(Matter_Expirable_persist_post_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Expirable_init_closure) },
        { be_const_key_weak(has_expired, -1), be_const_closure(Matter_Expirable_has_expired_closure) },
        { be_const_key_weak(set_expire_time, 6), be_const_closure(Matter_Expirable_set_expire_time_closure) },
        { be_const_key_weak(set_parent_list, 4), be_const_closure(Matter_Expirable_set_parent_list_closure) },
        { be_const_key_weak(hydrate_post, -1), be_const_closure(Matter_Expirable_hydrate_post_closure) },
        { be_const_key_weak(set_expire_in_seconds, -1), be_const_closure(Matter_Expirable_set_expire_in_seconds_closure) },
        { be_const_key_weak(get_parent_list, 8), be_const_closure(Matter_Expirable_get_parent_list_closure) },
        { be_const_key_weak(_list, -1), be_const_var(0) },
        { be_const_key_weak(does_persist, -1), be_const_closure(Matter_Expirable_does_persist_closure) },
        { be_const_key_weak(set_persist, -1), be_const_closure(Matter_Expirable_set_persist_closure) },
        { be_const_key_weak(persist_pre, -1), be_const_closure(Matter_Expirable_persist_pre_closure) },
        { be_const_key_weak(_persist, 2), be_const_var(1) },
        { be_const_key_weak(before_remove, 0), be_const_closure(Matter_Expirable_before_remove_closure) },
    })),
    be_str_weak(Matter_Expirable)
);
/*******************************************************************/

void be_load_Matter_Expirable_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Expirable);
    be_setglobal(vm, "Matter_Expirable");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_Expirable_list;

/********************************************************************
** Solidified function: count_persistables
********************************************************************/
be_local_closure(Matter_Expirable_list_count_persistables,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(_persist),
    /* K2   */  be_const_int(1),
    }),
    be_str_weak(count_persistables),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x58080000,  //  0001  LDCONST	R2	K0
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x140C0403,  //  0005  LT	R3	R2	R3
      0x780E0005,  //  0006  JMPF	R3	#000D
      0x940C0002,  //  0007  GETIDX	R3	R0	R2
      0x880C0701,  //  0008  GETMBR	R3	R3	K1
      0x780E0000,  //  0009  JMPF	R3	#000B
      0x00040302,  //  000A  ADD	R1	R1	K2
      0x00080502,  //  000B  ADD	R2	R2	K2
      0x7001FFF4,  //  000C  JMP		#0002
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove
********************************************************************/
be_local_closure(Matter_Expirable_list_remove,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(before_remove),
    /* K2   */  be_nested_str_weak(remove),
    }),
    be_str_weak(remove),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x28080300,  //  0000  GE	R2	R1	K0
      0x780A0007,  //  0001  JMPF	R2	#000A
      0x6008000C,  //  0002  GETGBL	R2	G12
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C080200,  //  0004  CALL	R2	1
      0x14080202,  //  0005  LT	R2	R1	R2
      0x780A0002,  //  0006  JMPF	R2	#000A
      0x94080001,  //  0007  GETIDX	R2	R0	R1
      0x8C080501,  //  0008  GETMET	R2	R2	K1
      0x7C080200,  //  0009  CALL	R2	1
      0x60080003,  //  000A  GETGBL	R2	G3
      0x5C0C0000,  //  000B  MOVE	R3	R0
      0x7C080200,  //  000C  CALL	R2	1
      0x8C080502,  //  000D  GETMET	R2	R2	K2
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x80040400,  //  0010  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: push
********************************************************************/
be_local_closure(Matter_Expirable_list_push,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Expirable),
    /* K2   */  be_nested_str_weak(type_error),
    /* K3   */  be_nested_str_weak(argument_X20must_X20be_X20of_X20class_X20_X27Expirable_X27),
    /* K4   */  be_nested_str_weak(set_parent_list),
    /* K5   */  be_nested_str_weak(push),
    }),
    be_str_weak(push),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0xB8120000,  //  0002  GETNGBL	R4	K0
      0x88100901,  //  0003  GETMBR	R4	R4	K1
      0x7C080400,  //  0004  CALL	R2	2
      0x740A0000,  //  0005  JMPT	R2	#0007
      0xB0060503,  //  0006  RAISE	1	K2	K3
      0x8C080304,  //  0007  GETMET	R2	R1	K4
      0x5C100000,  //  0008  MOVE	R4	R0
      0x7C080400,  //  0009  CALL	R2	2
      0x60080003,  //  000A  GETGBL	R2	G3
      0x5C0C0000,  //  000B  MOVE	R3	R0
      0x7C080200,  //  000C  CALL	R2	1
      0x8C080505,  //  000D  GETMET	R2	R2	K5
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x80040400,  //  0010  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Expirable_list_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(remove_expired),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_expired
********************************************************************/
be_local_closure(Matter_Expirable_list_remove_expired,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(has_expired),
    /* K2   */  be_nested_str_weak(_persist),
    /* K3   */  be_nested_str_weak(remove),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(remove_expired),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x58080000,  //  0001  LDCONST	R2	K0
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x140C0403,  //  0005  LT	R3	R2	R3
      0x780E000D,  //  0006  JMPF	R3	#0015
      0x940C0002,  //  0007  GETIDX	R3	R0	R2
      0x8C0C0701,  //  0008  GETMET	R3	R3	K1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x780E0007,  //  000A  JMPF	R3	#0013
      0x940C0002,  //  000B  GETIDX	R3	R0	R2
      0x880C0702,  //  000C  GETMBR	R3	R3	K2
      0x780E0000,  //  000D  JMPF	R3	#000F
      0x50040200,  //  000E  LDBOOL	R1	1	0
      0x8C0C0103,  //  000F  GETMET	R3	R0	K3
      0x5C140400,  //  0010  MOVE	R5	R2
      0x7C0C0400,  //  0011  CALL	R3	2
      0x70020000,  //  0012  JMP		#0014
      0x00080504,  //  0013  ADD	R2	R2	K4
      0x7001FFEC,  //  0014  JMP		#0002
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: persistables
********************************************************************/
be_local_closure(Matter_Expirable_list_persistables,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(_persist),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x50000200,  //  0000  LDBOOL	R0	1	0
          0x78020005,  //  0001  JMPF	R0	#0008
          0x68000000,  //  0002  GETUPV	R0	U0
          0x7C000000,  //  0003  CALL	R0	0
          0x88040100,  //  0004  GETMBR	R1	R0	K0
          0x78060000,  //  0005  JMPF	R1	#0007
          0x80040000,  //  0006  RET	1	R0
          0x7001FFF7,  //  0007  JMP		#0000
          0x80000000,  //  0008  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(iter),
    }),
    be_str_weak(persistables),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x84080000,  //  0002  CLOSURE	R2	P0
      0xA0000000,  //  0003  CLOSE	R0
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setitem
********************************************************************/
be_local_closure(Matter_Expirable_list_setitem,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Expirable),
    /* K2   */  be_nested_str_weak(type_error),
    /* K3   */  be_nested_str_weak(argument_X20must_X20be_X20of_X20class_X20_X27Expirable_X27),
    /* K4   */  be_nested_str_weak(set_parent_list),
    /* K5   */  be_nested_str_weak(setitem),
    }),
    be_str_weak(setitem),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x600C000F,  //  0000  GETGBL	R3	G15
      0x5C100400,  //  0001  MOVE	R4	R2
      0xB8160000,  //  0002  GETNGBL	R5	K0
      0x88140B01,  //  0003  GETMBR	R5	R5	K1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x740E0000,  //  0005  JMPT	R3	#0007
      0xB0060503,  //  0006  RAISE	1	K2	K3
      0x8C0C0504,  //  0007  GETMET	R3	R2	K4
      0x5C140000,  //  0008  MOVE	R5	R0
      0x7C0C0400,  //  0009  CALL	R3	2
      0x600C0003,  //  000A  GETGBL	R3	G3
      0x5C100000,  //  000B  MOVE	R4	R0
      0x7C0C0200,  //  000C  CALL	R3	1
      0x8C0C0705,  //  000D  GETMET	R3	R3	K5
      0x5C140200,  //  000E  MOVE	R5	R1
      0x5C180400,  //  000F  MOVE	R6	R2
      0x7C0C0600,  //  0010  CALL	R3	3
      0x80040600,  //  0011  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Expirable_list
********************************************************************/
extern const bclass be_class_list;
be_local_class(Matter_Expirable_list,
    0,
    &be_class_list,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(count_persistables, 4), be_const_closure(Matter_Expirable_list_count_persistables_closure) },
        { be_const_key_weak(remove, -1), be_const_closure(Matter_Expirable_list_remove_closure) },
        { be_const_key_weak(push, 5), be_const_closure(Matter_Expirable_list_push_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Expirable_list_every_second_closure) },
        { be_const_key_weak(setitem, 6), be_const_closure(Matter_Expirable_list_setitem_closure) },
        { be_const_key_weak(persistables, -1), be_const_closure(Matter_Expirable_list_persistables_closure) },
        { be_const_key_weak(remove_expired, -1), be_const_closure(Matter_Expirable_list_remove_expired_closure) },
    })),
    be_str_weak(Matter_Expirable_list)
);
/*******************************************************************/

void be_load_Matter_Expirable_list_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Expirable_list);
    be_setglobal(vm, "Matter_Expirable_list");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
