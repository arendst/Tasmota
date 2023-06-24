/* Solidification of rule_matcher.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Rule_Matcher_Key;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Rule_Matcher_Key_tostring,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_X3CMatcher_X20key_X3D_X27),
    /* K1   */  be_nested_str(name),
    /* K2   */  be_nested_str(_X27_X3E),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x60040008,  //  0000  GETGBL	R1	G8
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x00060001,  //  0003  ADD	R1	K0	R1
      0x00040302,  //  0004  ADD	R1	R1	K2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_key_i
********************************************************************/
be_local_closure(Rule_Matcher_Key_find_key_i,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_class(be_class_Rule_Matcher_Key),
    /* K1   */  be_nested_str(string),
    /* K2   */  be_nested_str(toupper),
    /* K3   */  be_nested_str(keys),
    /* K4   */  be_nested_str(stop_iteration),
    }),
    &be_const_str_find_key_i,
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100702,  //  0002  GETMET	R4	R3	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x6014000F,  //  0005  GETGBL	R5	G15
      0x5C180000,  //  0006  MOVE	R6	R0
      0x601C0013,  //  0007  GETGBL	R7	G19
      0x7C140400,  //  0008  CALL	R5	2
      0x78160011,  //  0009  JMPF	R5	#001C
      0x60140010,  //  000A  GETGBL	R5	G16
      0x8C180103,  //  000B  GETMET	R6	R0	K3
      0x7C180200,  //  000C  CALL	R6	1
      0x7C140200,  //  000D  CALL	R5	1
      0xA8020009,  //  000E  EXBLK	0	#0019
      0x5C180A00,  //  000F  MOVE	R6	R5
      0x7C180000,  //  0010  CALL	R6	0
      0x8C1C0702,  //  0011  GETMET	R7	R3	K2
      0x5C240C00,  //  0012  MOVE	R9	R6
      0x7C1C0400,  //  0013  CALL	R7	2
      0x1C1C0E04,  //  0014  EQ	R7	R7	R4
      0x781E0001,  //  0015  JMPF	R7	#0018
      0xA8040001,  //  0016  EXBLK	1	1
      0x80040C00,  //  0017  RET	1	R6
      0x7001FFF5,  //  0018  JMP		#000F
      0x58140004,  //  0019  LDCONST	R5	K4
      0xAC140200,  //  001A  CATCH	R5	1	0
      0xB0080000,  //  001B  RAISE	2	R0	R0
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: match
********************************************************************/
be_local_closure(Rule_Matcher_Key_match,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(find_key_i),
    /* K1   */  be_nested_str(name),
    }),
    &be_const_str_match,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x6008000F,  //  0005  GETGBL	R2	G15
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x60100013,  //  0007  GETGBL	R4	G19
      0x7C080400,  //  0008  CALL	R2	2
      0x740A0001,  //  0009  JMPT	R2	#000C
      0x4C080000,  //  000A  LDNIL	R2
      0x80040400,  //  000B  RET	1	R2
      0x8C080100,  //  000C  GETMET	R2	R0	K0
      0x5C100200,  //  000D  MOVE	R4	R1
      0x88140101,  //  000E  GETMBR	R5	R0	K1
      0x7C080600,  //  000F  CALL	R2	3
      0x4C0C0000,  //  0010  LDNIL	R3
      0x1C0C0403,  //  0011  EQ	R3	R2	R3
      0x780E0001,  //  0012  JMPF	R3	#0015
      0x4C0C0000,  //  0013  LDNIL	R3
      0x80040600,  //  0014  RET	1	R3
      0x940C0202,  //  0015  GETIDX	R3	R1	R2
      0x80040600,  //  0016  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Rule_Matcher_Key_init,   /* name */
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
    /* K0   */  be_nested_str(name),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Rule_Matcher_Key
********************************************************************/
be_local_class(Rule_Matcher_Key,
    1,
    NULL,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(tostring, 3), be_const_closure(Rule_Matcher_Key_tostring_closure) },
        { be_const_key(find_key_i, -1), be_const_static_closure(Rule_Matcher_Key_find_key_i_closure) },
        { be_const_key(name, -1), be_const_var(0) },
        { be_const_key(init, 4), be_const_closure(Rule_Matcher_Key_init_closure) },
        { be_const_key(match, -1), be_const_closure(Rule_Matcher_Key_match_closure) },
    })),
    (bstring*) &be_const_str_Rule_Matcher_Key
);
/*******************************************************************/

void be_load_Rule_Matcher_Key_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Rule_Matcher_Key);
    be_setglobal(vm, "Rule_Matcher_Key");
    be_pop(vm, 1);
}

extern const bclass be_class_Rule_Matcher_Wildcard;

/********************************************************************
** Solidified function: match
********************************************************************/
be_local_closure(Rule_Matcher_Wildcard_match,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(iter),
    }),
    &be_const_str_match,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x6008000F,  //  0005  GETGBL	R2	G15
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x60100013,  //  0007  GETGBL	R4	G19
      0x7C080400,  //  0008  CALL	R2	2
      0x740A0001,  //  0009  JMPT	R2	#000C
      0x4C080000,  //  000A  LDNIL	R2
      0x80040400,  //  000B  RET	1	R2
      0x6008000C,  //  000C  GETGBL	R2	G12
      0x5C0C0200,  //  000D  MOVE	R3	R1
      0x7C080200,  //  000E  CALL	R2	1
      0x1C080500,  //  000F  EQ	R2	R2	K0
      0x780A0001,  //  0010  JMPF	R2	#0013
      0x4C080000,  //  0011  LDNIL	R2
      0x80040400,  //  0012  RET	1	R2
      0x8C080301,  //  0013  GETMET	R2	R1	K1
      0x7C080200,  //  0014  CALL	R2	1
      0x7C080000,  //  0015  CALL	R2	0
      0x80040400,  //  0016  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Rule_Matcher_Wildcard_tostring,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(_X3CMatcher_X20any_X3E),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80060000,  //  0000  RET	1	K0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Rule_Matcher_Wildcard
********************************************************************/
be_local_class(Rule_Matcher_Wildcard,
    0,
    NULL,
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(match, -1), be_const_closure(Rule_Matcher_Wildcard_match_closure) },
        { be_const_key(tostring, -1), be_const_closure(Rule_Matcher_Wildcard_tostring_closure) },
    })),
    (bstring*) &be_const_str_Rule_Matcher_Wildcard
);
/*******************************************************************/

void be_load_Rule_Matcher_Wildcard_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Rule_Matcher_Wildcard);
    be_setglobal(vm, "Rule_Matcher_Wildcard");
    be_pop(vm, 1);
}

extern const bclass be_class_Rule_Matcher_Operator;

/********************************************************************
** Solidified function: match
********************************************************************/
be_local_closure(Rule_Matcher_Operator_match,   /* name */
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
    /* K0   */  be_nested_str(int),
    /* K1   */  be_nested_str(real),
    /* K2   */  be_nested_str(string),
    /* K3   */  be_nested_str(op_func),
    /* K4   */  be_nested_str(op_value),
    }),
    &be_const_str_match,
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x200C0500,  //  0003  NE	R3	R2	K0
      0x780E0005,  //  0004  JMPF	R3	#000B
      0x200C0501,  //  0005  NE	R3	R2	K1
      0x780E0003,  //  0006  JMPF	R3	#000B
      0x200C0502,  //  0007  NE	R3	R2	K2
      0x780E0001,  //  0008  JMPF	R3	#000B
      0x4C0C0000,  //  0009  LDNIL	R3
      0x80040600,  //  000A  RET	1	R3
      0x8C0C0103,  //  000B  GETMET	R3	R0	K3
      0x5C140200,  //  000C  MOVE	R5	R1
      0x88180104,  //  000D  GETMBR	R6	R0	K4
      0x7C0C0600,  //  000E  CALL	R3	3
      0x780E0001,  //  000F  JMPF	R3	#0012
      0x5C0C0200,  //  0010  MOVE	R3	R1
      0x70020000,  //  0011  JMP		#0013
      0x4C0C0000,  //  0012  LDNIL	R3
      0x80040600,  //  0013  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: op_parse
********************************************************************/
be_local_closure(Rule_Matcher_Operator_op_parse,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[13]) {
      be_nested_proto(
        7,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_nested_str(tasmota),
        /* K1   */  be_nested_str(_apply_str_op),
        /* K2   */  be_const_int(1),
        }),
        &be_const_str_op_eq_str,
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0xB80A0000,  //  0000  GETNGBL	R2	K0
          0x8C080501,  //  0001  GETMET	R2	R2	K1
          0x58100002,  //  0002  LDCONST	R4	K2
          0x60140008,  //  0003  GETGBL	R5	G8
          0x5C180000,  //  0004  MOVE	R6	R0
          0x7C140200,  //  0005  CALL	R5	1
          0x5C180200,  //  0006  MOVE	R6	R1
          0x7C080800,  //  0007  CALL	R2	4
          0x80040400,  //  0008  RET	1	R2
        })
      ),
      be_nested_proto(
        7,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_nested_str(tasmota),
        /* K1   */  be_nested_str(_apply_str_op),
        /* K2   */  be_const_int(2),
        }),
        &be_const_str_op_neq_str,
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0xB80A0000,  //  0000  GETNGBL	R2	K0
          0x8C080501,  //  0001  GETMET	R2	R2	K1
          0x58100002,  //  0002  LDCONST	R4	K2
          0x60140008,  //  0003  GETGBL	R5	G8
          0x5C180000,  //  0004  MOVE	R6	R0
          0x7C140200,  //  0005  CALL	R5	1
          0x5C180200,  //  0006  MOVE	R6	R1
          0x7C080800,  //  0007  CALL	R2	4
          0x80040400,  //  0008  RET	1	R2
        })
      ),
      be_nested_proto(
        7,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_nested_str(tasmota),
        /* K1   */  be_nested_str(_apply_str_op),
        /* K2   */  be_const_int(3),
        }),
        &be_const_str_op_start_str,
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0xB80A0000,  //  0000  GETNGBL	R2	K0
          0x8C080501,  //  0001  GETMET	R2	R2	K1
          0x58100002,  //  0002  LDCONST	R4	K2
          0x60140008,  //  0003  GETGBL	R5	G8
          0x5C180000,  //  0004  MOVE	R6	R0
          0x7C140200,  //  0005  CALL	R5	1
          0x5C180200,  //  0006  MOVE	R6	R1
          0x7C080800,  //  0007  CALL	R2	4
          0x80040400,  //  0008  RET	1	R2
        })
      ),
      be_nested_proto(
        7,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str(tasmota),
        /* K1   */  be_nested_str(_apply_str_op),
        }),
        &be_const_str_op_end_str,
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0xB80A0000,  //  0000  GETNGBL	R2	K0
          0x8C080501,  //  0001  GETMET	R2	R2	K1
          0x54120003,  //  0002  LDINT	R4	4
          0x60140008,  //  0003  GETGBL	R5	G8
          0x5C180000,  //  0004  MOVE	R6	R0
          0x7C140200,  //  0005  CALL	R5	1
          0x5C180200,  //  0006  MOVE	R6	R1
          0x7C080800,  //  0007  CALL	R2	4
          0x80040400,  //  0008  RET	1	R2
        })
      ),
      be_nested_proto(
        7,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str(tasmota),
        /* K1   */  be_nested_str(_apply_str_op),
        }),
        &be_const_str_op_sub_str,
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0xB80A0000,  //  0000  GETNGBL	R2	K0
          0x8C080501,  //  0001  GETMET	R2	R2	K1
          0x54120004,  //  0002  LDINT	R4	5
          0x60140008,  //  0003  GETGBL	R5	G8
          0x5C180000,  //  0004  MOVE	R6	R0
          0x7C140200,  //  0005  CALL	R5	1
          0x5C180200,  //  0006  MOVE	R6	R1
          0x7C080800,  //  0007  CALL	R2	4
          0x80040400,  //  0008  RET	1	R2
        })
      ),
      be_nested_proto(
        7,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str(tasmota),
        /* K1   */  be_nested_str(_apply_str_op),
        }),
        &be_const_str_op_notsub_str,
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0xB80A0000,  //  0000  GETNGBL	R2	K0
          0x8C080501,  //  0001  GETMET	R2	R2	K1
          0x54120005,  //  0002  LDINT	R4	6
          0x60140008,  //  0003  GETGBL	R5	G8
          0x5C180000,  //  0004  MOVE	R6	R0
          0x7C140200,  //  0005  CALL	R5	1
          0x5C180200,  //  0006  MOVE	R6	R1
          0x7C080800,  //  0007  CALL	R2	4
          0x80040400,  //  0008  RET	1	R2
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        &be_const_str_op_eq,
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x60080007,  //  0000  GETGBL	R2	G7
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x1C080401,  //  0003  EQ	R2	R2	R1
          0x80040400,  //  0004  RET	1	R2
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        &be_const_str_op_neq,
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x60080007,  //  0000  GETGBL	R2	G7
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x20080401,  //  0003  NE	R2	R2	R1
          0x80040400,  //  0004  RET	1	R2
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        &be_const_str_op_gt,
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x60080007,  //  0000  GETGBL	R2	G7
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x24080401,  //  0003  GT	R2	R2	R1
          0x80040400,  //  0004  RET	1	R2
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        &be_const_str_op_gte,
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x60080007,  //  0000  GETGBL	R2	G7
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x28080401,  //  0003  GE	R2	R2	R1
          0x80040400,  //  0004  RET	1	R2
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        &be_const_str_op_lt,
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x60080007,  //  0000  GETGBL	R2	G7
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x14080401,  //  0003  LT	R2	R2	R1
          0x80040400,  //  0004  RET	1	R2
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        &be_const_str_op_lte,
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x60080007,  //  0000  GETGBL	R2	G7
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x18080401,  //  0003  LE	R2	R2	R1
          0x80040400,  //  0004  RET	1	R2
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_const_int(0),
        }),
        &be_const_str_op_mod,
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x60080009,  //  0000  GETGBL	R2	G9
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x10080401,  //  0003  MOD	R2	R2	R1
          0x1C080500,  //  0004  EQ	R2	R2	K0
          0x80040400,  //  0005  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str(op_str),
    /* K1   */  be_nested_str(_X3D),
    /* K2   */  be_nested_str(_X21_X3D_X3D),
    /* K3   */  be_nested_str(_X24_X21),
    /* K4   */  be_nested_str(_X24_X3C),
    /* K5   */  be_nested_str(_X24_X3E),
    /* K6   */  be_nested_str(_X24_X7C),
    /* K7   */  be_nested_str(_X24_X5E),
    /* K8   */  be_nested_str(_X3D_X3D),
    /* K9   */  be_nested_str(_X21_X3D),
    /* K10  */  be_nested_str(_X3E),
    /* K11  */  be_nested_str(_X3E_X3D),
    /* K12  */  be_nested_str(_X3C),
    /* K13  */  be_nested_str(_X3C_X3D),
    /* K14  */  be_nested_str(_X7C),
    /* K15  */  be_nested_str(op_func),
    /* K16  */  be_nested_str(json),
    /* K17  */  be_nested_str(load),
    /* K18  */  be_nested_str(int),
    /* K19  */  be_nested_str(real),
    /* K20  */  be_nested_str(value_error),
    /* K21  */  be_nested_str(value_X20needs_X20to_X20be_X20a_X20number),
    /* K22  */  be_nested_str(op_value),
    }),
    &be_const_str_op_parse,
    &be_const_str_solidified,
    ( &(const binstruction[97]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x840C0000,  //  0001  CLOSURE	R3	P0
      0x84100001,  //  0002  CLOSURE	R4	P1
      0x84140002,  //  0003  CLOSURE	R5	P2
      0x84180003,  //  0004  CLOSURE	R6	P3
      0x841C0004,  //  0005  CLOSURE	R7	P4
      0x84200005,  //  0006  CLOSURE	R8	P5
      0x84240006,  //  0007  CLOSURE	R9	P6
      0x84280007,  //  0008  CLOSURE	R10	P7
      0x842C0008,  //  0009  CLOSURE	R11	P8
      0x84300009,  //  000A  CLOSURE	R12	P9
      0x8434000A,  //  000B  CLOSURE	R13	P10
      0x8438000B,  //  000C  CLOSURE	R14	P11
      0x843C000C,  //  000D  CLOSURE	R15	P12
      0x50400000,  //  000E  LDBOOL	R16	0	0
      0x4C440000,  //  000F  LDNIL	R17
      0x1C480301,  //  0010  EQ	R18	R1	K1
      0x784A0001,  //  0011  JMPF	R18	#0014
      0x5C440600,  //  0012  MOVE	R17	R3
      0x70020033,  //  0013  JMP		#0048
      0x1C480302,  //  0014  EQ	R18	R1	K2
      0x784A0001,  //  0015  JMPF	R18	#0018
      0x5C440800,  //  0016  MOVE	R17	R4
      0x7002002F,  //  0017  JMP		#0048
      0x1C480303,  //  0018  EQ	R18	R1	K3
      0x784A0001,  //  0019  JMPF	R18	#001C
      0x5C440800,  //  001A  MOVE	R17	R4
      0x7002002B,  //  001B  JMP		#0048
      0x1C480304,  //  001C  EQ	R18	R1	K4
      0x784A0001,  //  001D  JMPF	R18	#0020
      0x5C440A00,  //  001E  MOVE	R17	R5
      0x70020027,  //  001F  JMP		#0048
      0x1C480305,  //  0020  EQ	R18	R1	K5
      0x784A0001,  //  0021  JMPF	R18	#0024
      0x5C440C00,  //  0022  MOVE	R17	R6
      0x70020023,  //  0023  JMP		#0048
      0x1C480306,  //  0024  EQ	R18	R1	K6
      0x784A0001,  //  0025  JMPF	R18	#0028
      0x5C440E00,  //  0026  MOVE	R17	R7
      0x7002001F,  //  0027  JMP		#0048
      0x1C480307,  //  0028  EQ	R18	R1	K7
      0x784A0001,  //  0029  JMPF	R18	#002C
      0x5C441000,  //  002A  MOVE	R17	R8
      0x7002001B,  //  002B  JMP		#0048
      0x50400200,  //  002C  LDBOOL	R16	1	0
      0x1C480308,  //  002D  EQ	R18	R1	K8
      0x784A0001,  //  002E  JMPF	R18	#0031
      0x5C441200,  //  002F  MOVE	R17	R9
      0x70020016,  //  0030  JMP		#0048
      0x1C480309,  //  0031  EQ	R18	R1	K9
      0x784A0001,  //  0032  JMPF	R18	#0035
      0x5C441400,  //  0033  MOVE	R17	R10
      0x70020012,  //  0034  JMP		#0048
      0x1C48030A,  //  0035  EQ	R18	R1	K10
      0x784A0001,  //  0036  JMPF	R18	#0039
      0x5C441600,  //  0037  MOVE	R17	R11
      0x7002000E,  //  0038  JMP		#0048
      0x1C48030B,  //  0039  EQ	R18	R1	K11
      0x784A0001,  //  003A  JMPF	R18	#003D
      0x5C441800,  //  003B  MOVE	R17	R12
      0x7002000A,  //  003C  JMP		#0048
      0x1C48030C,  //  003D  EQ	R18	R1	K12
      0x784A0001,  //  003E  JMPF	R18	#0041
      0x5C441A00,  //  003F  MOVE	R17	R13
      0x70020006,  //  0040  JMP		#0048
      0x1C48030D,  //  0041  EQ	R18	R1	K13
      0x784A0001,  //  0042  JMPF	R18	#0045
      0x5C441C00,  //  0043  MOVE	R17	R14
      0x70020002,  //  0044  JMP		#0048
      0x1C48030E,  //  0045  EQ	R18	R1	K14
      0x784A0000,  //  0046  JMPF	R18	#0048
      0x5C441E00,  //  0047  MOVE	R17	R15
      0x90021E11,  //  0048  SETMBR	R0	K15	R17
      0x78420011,  //  0049  JMPF	R16	#005C
      0xA44A2000,  //  004A  IMPORT	R18	K16
      0x8C4C2511,  //  004B  GETMET	R19	R18	K17
      0x5C540400,  //  004C  MOVE	R21	R2
      0x7C4C0400,  //  004D  CALL	R19	2
      0x60500004,  //  004E  GETGBL	R20	G4
      0x5C542600,  //  004F  MOVE	R21	R19
      0x7C500200,  //  0050  CALL	R20	1
      0x20502912,  //  0051  NE	R20	R20	K18
      0x78520006,  //  0052  JMPF	R20	#005A
      0x60500004,  //  0053  GETGBL	R20	G4
      0x5C542600,  //  0054  MOVE	R21	R19
      0x7C500200,  //  0055  CALL	R20	1
      0x20502913,  //  0056  NE	R20	R20	K19
      0x78520001,  //  0057  JMPF	R20	#005A
      0xB0062915,  //  0058  RAISE	1	K20	K21
      0x70020000,  //  0059  JMP		#005B
      0x90022C13,  //  005A  SETMBR	R0	K22	R19
      0x70020003,  //  005B  JMP		#0060
      0x60480008,  //  005C  GETGBL	R18	G8
      0x5C4C0400,  //  005D  MOVE	R19	R2
      0x7C480200,  //  005E  CALL	R18	1
      0x90022C12,  //  005F  SETMBR	R0	K22	R18
      0x80000000,  //  0060  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Rule_Matcher_Operator_tostring,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(op_value),
    /* K1   */  be_nested_str(string),
    /* K2   */  be_nested_str(_X3CMatcher_X20op_X20_X27),
    /* K3   */  be_nested_str(op_str),
    /* K4   */  be_nested_str(_X27_X20val_X3D_X27),
    /* K5   */  be_nested_str(_X27_X3E),
    /* K6   */  be_nested_str(_X27_X20val_X3D),
    /* K7   */  be_nested_str(_X3E),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60040004,  //  0000  GETGBL	R1	G4
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040301,  //  0003  EQ	R1	R1	K1
      0x78060009,  //  0004  JMPF	R1	#000F
      0x88040103,  //  0005  GETMBR	R1	R0	K3
      0x00060401,  //  0006  ADD	R1	K2	R1
      0x00040304,  //  0007  ADD	R1	R1	K4
      0x60080008,  //  0008  GETGBL	R2	G8
      0x880C0100,  //  0009  GETMBR	R3	R0	K0
      0x7C080200,  //  000A  CALL	R2	1
      0x00040202,  //  000B  ADD	R1	R1	R2
      0x00040305,  //  000C  ADD	R1	R1	K5
      0x80040200,  //  000D  RET	1	R1
      0x70020008,  //  000E  JMP		#0018
      0x88040103,  //  000F  GETMBR	R1	R0	K3
      0x00060401,  //  0010  ADD	R1	K2	R1
      0x00040306,  //  0011  ADD	R1	R1	K6
      0x60080008,  //  0012  GETGBL	R2	G8
      0x880C0100,  //  0013  GETMBR	R3	R0	K0
      0x7C080200,  //  0014  CALL	R2	1
      0x00040202,  //  0015  ADD	R1	R1	R2
      0x00040307,  //  0016  ADD	R1	R1	K7
      0x80040200,  //  0017  RET	1	R1
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Rule_Matcher_Operator_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(op_parse),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C0C0600,  //  0003  CALL	R3	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Rule_Matcher_Operator
********************************************************************/
be_local_class(Rule_Matcher_Operator,
    3,
    NULL,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(op_value, 6), be_const_var(2) },
        { be_const_key(tostring, -1), be_const_closure(Rule_Matcher_Operator_tostring_closure) },
        { be_const_key(op_func, 0), be_const_var(0) },
        { be_const_key(match, 1), be_const_closure(Rule_Matcher_Operator_match_closure) },
        { be_const_key(op_str, -1), be_const_var(1) },
        { be_const_key(init, -1), be_const_closure(Rule_Matcher_Operator_init_closure) },
        { be_const_key(op_parse, -1), be_const_closure(Rule_Matcher_Operator_op_parse_closure) },
    })),
    (bstring*) &be_const_str_Rule_Matcher_Operator
);
/*******************************************************************/

void be_load_Rule_Matcher_Operator_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Rule_Matcher_Operator);
    be_setglobal(vm, "Rule_Matcher_Operator");
    be_pop(vm, 1);
}

extern const bclass be_class_Rule_Matcher_Array;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Rule_Matcher_Array_init,   /* name */
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
    /* K0   */  be_nested_str(index),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Rule_Matcher_Array_tostring,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_X3CMatcher_X20_X5B),
    /* K1   */  be_nested_str(index),
    /* K2   */  be_nested_str(_X5D_X3E),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x60040008,  //  0000  GETGBL	R1	G8
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x00060001,  //  0003  ADD	R1	K0	R1
      0x00040302,  //  0004  ADD	R1	R1	K2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: match
********************************************************************/
be_local_closure(Rule_Matcher_Array_match,   /* name */
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
    /* K0   */  be_nested_str(index),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    }),
    &be_const_str_match,
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x6008000F,  //  0005  GETGBL	R2	G15
      0x5C0C0200,  //  0006  MOVE	R3	R1
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C080400,  //  0008  CALL	R2	2
      0x740A0000,  //  0009  JMPT	R2	#000B
      0x80040200,  //  000A  RET	1	R1
      0x88080100,  //  000B  GETMBR	R2	R0	K0
      0x18080501,  //  000C  LE	R2	R2	K1
      0x780A0001,  //  000D  JMPF	R2	#0010
      0x4C080000,  //  000E  LDNIL	R2
      0x80040400,  //  000F  RET	1	R2
      0x88080100,  //  0010  GETMBR	R2	R0	K0
      0x600C000C,  //  0011  GETGBL	R3	G12
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C0C0200,  //  0013  CALL	R3	1
      0x24080403,  //  0014  GT	R2	R2	R3
      0x780A0001,  //  0015  JMPF	R2	#0018
      0x4C080000,  //  0016  LDNIL	R2
      0x80040400,  //  0017  RET	1	R2
      0x88080100,  //  0018  GETMBR	R2	R0	K0
      0x04080502,  //  0019  SUB	R2	R2	K2
      0x94080202,  //  001A  GETIDX	R2	R1	R2
      0x80040400,  //  001B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Rule_Matcher_Array
********************************************************************/
be_local_class(Rule_Matcher_Array,
    1,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(index, -1), be_const_var(0) },
        { be_const_key(tostring, -1), be_const_closure(Rule_Matcher_Array_tostring_closure) },
        { be_const_key(match, -1), be_const_closure(Rule_Matcher_Array_match_closure) },
        { be_const_key(init, 0), be_const_closure(Rule_Matcher_Array_init_closure) },
    })),
    (bstring*) &be_const_str_Rule_Matcher_Array
);
/*******************************************************************/

void be_load_Rule_Matcher_Array_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Rule_Matcher_Array);
    be_setglobal(vm, "Rule_Matcher_Array");
    be_pop(vm, 1);
}

extern const bclass be_class_Rule_Matcher_AND_List;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Rule_Matcher_AND_List_init,   /* name */
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
    /* K0   */  be_nested_str(and_list),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Rule_Matcher_AND_List_tostring,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_X3CMatcher_AND_List_X20),
    /* K1   */  be_nested_str(and_list),
    /* K2   */  be_nested_str(_X3E),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x60040008,  //  0000  GETGBL	R1	G8
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x00060001,  //  0003  ADD	R1	K0	R1
      0x00040302,  //  0004  ADD	R1	R1	K2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: match
********************************************************************/
be_local_closure(Rule_Matcher_AND_List_match,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(and_list),
    /* K2   */  be_nested_str(match),
    /* K3   */  be_nested_str(push),
    /* K4   */  be_const_int(1),
    }),
    &be_const_str_match,
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C0012,  //  0001  GETGBL	R3	G18
      0x7C0C0000,  //  0002  CALL	R3	0
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140101,  //  0004  GETMBR	R5	R0	K1
      0x7C100200,  //  0005  CALL	R4	1
      0x14100404,  //  0006  LT	R4	R2	R4
      0x7812000E,  //  0007  JMPF	R4	#0017
      0x88100101,  //  0008  GETMBR	R4	R0	K1
      0x94100802,  //  0009  GETIDX	R4	R4	R2
      0x8C140902,  //  000A  GETMET	R5	R4	K2
      0x5C1C0200,  //  000B  MOVE	R7	R1
      0x7C140400,  //  000C  CALL	R5	2
      0x4C180000,  //  000D  LDNIL	R6
      0x1C180A06,  //  000E  EQ	R6	R5	R6
      0x781A0001,  //  000F  JMPF	R6	#0012
      0x4C180000,  //  0010  LDNIL	R6
      0x80040C00,  //  0011  RET	1	R6
      0x8C180703,  //  0012  GETMET	R6	R3	K3
      0x5C200A00,  //  0013  MOVE	R8	R5
      0x7C180400,  //  0014  CALL	R6	2
      0x00080504,  //  0015  ADD	R2	R2	K4
      0x7001FFEB,  //  0016  JMP		#0003
      0x80040600,  //  0017  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Rule_Matcher_AND_List
********************************************************************/
be_local_class(Rule_Matcher_AND_List,
    1,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(and_list, -1), be_const_var(0) },
        { be_const_key(tostring, -1), be_const_closure(Rule_Matcher_AND_List_tostring_closure) },
        { be_const_key(match, -1), be_const_closure(Rule_Matcher_AND_List_match_closure) },
        { be_const_key(init, 0), be_const_closure(Rule_Matcher_AND_List_init_closure) },
    })),
    (bstring*) &be_const_str_Rule_Matcher_AND_List
);
/*******************************************************************/

void be_load_Rule_Matcher_AND_List_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Rule_Matcher_AND_List);
    be_setglobal(vm, "Rule_Matcher_AND_List");
    be_pop(vm, 1);
}

extern const bclass be_class_Rule_Matcher;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Rule_Matcher_tostring,   /* name */
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
    /* K0   */  be_nested_str(matchers),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040008,  //  0000  GETGBL	R1	G8
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Rule_Matcher_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(rule),
    /* K1   */  be_nested_str(trigger),
    /* K2   */  be_nested_str(matchers),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
be_local_closure(Rule_Matcher_parse,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_const_class(be_class_Rule_Matcher),
    /* K1   */  be_nested_str(string),
    /* K2   */  be_nested_str(parse),
    /* K3   */  be_nested_str(push),
    /* K4   */  be_nested_str(trigger),
    /* K5   */  be_nested_str(stop_iteration),
    /* K6   */  be_nested_str(Rule_Matcher_AND_List),
    /* K7   */  be_nested_str(tasmota),
    /* K8   */  be_nested_str(find_op),
    /* K9   */  be_const_int(0),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str(find),
    /* K13  */  be_nested_str(_X23),
    /* K14  */  be_nested_str(pattern_error),
    /* K15  */  be_nested_str(empty_X20pattern_X20not_X20allowed),
    /* K16  */  be_const_int(2147483647),
    /* K17  */  be_nested_str(_X5B),
    /* K18  */  be_nested_str(_X5D),
    /* K19  */  be_nested_str(value_error),
    /* K20  */  be_nested_str(missing_X20_X27_X5D_X27_X20in_X20rule_X20pattern),
    /* K21  */  be_nested_str(_X3F),
    /* K22  */  be_nested_str(Rule_Matcher_Wildcard),
    /* K23  */  be_nested_str(Rule_Matcher_Key),
    /* K24  */  be_nested_str(Rule_Matcher_Array),
    /* K25  */  be_nested_str(Rule_Matcher_Operator),
    }),
    &be_const_str_parse,
    &be_const_str_solidified,
    ( &(const binstruction[147]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0003,  //  0003  EQ	R3	R0	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x4C0C0000,  //  0005  LDNIL	R3
      0x80040600,  //  0006  RET	1	R3
      0x600C000F,  //  0007  GETGBL	R3	G15
      0x5C100000,  //  0008  MOVE	R4	R0
      0x60140012,  //  0009  GETGBL	R5	G18
      0x7C0C0400,  //  000A  CALL	R3	2
      0x780E0021,  //  000B  JMPF	R3	#002E
      0x600C0012,  //  000C  GETGBL	R3	G18
      0x7C0C0000,  //  000D  CALL	R3	0
      0x60100012,  //  000E  GETGBL	R4	G18
      0x7C100000,  //  000F  CALL	R4	0
      0x60140010,  //  0010  GETGBL	R5	G16
      0x5C180000,  //  0011  MOVE	R6	R0
      0x7C140200,  //  0012  CALL	R5	1
      0xA802000B,  //  0013  EXBLK	0	#0020
      0x5C180A00,  //  0014  MOVE	R6	R5
      0x7C180000,  //  0015  CALL	R6	0
      0x8C1C0302,  //  0016  GETMET	R7	R1	K2
      0x5C240C00,  //  0017  MOVE	R9	R6
      0x7C1C0400,  //  0018  CALL	R7	2
      0x8C200703,  //  0019  GETMET	R8	R3	K3
      0x5C280E00,  //  001A  MOVE	R10	R7
      0x7C200400,  //  001B  CALL	R8	2
      0x8C200903,  //  001C  GETMET	R8	R4	K3
      0x88280F04,  //  001D  GETMBR	R10	R7	K4
      0x7C200400,  //  001E  CALL	R8	2
      0x7001FFF3,  //  001F  JMP		#0014
      0x58140005,  //  0020  LDCONST	R5	K5
      0xAC140200,  //  0021  CATCH	R5	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x5C140200,  //  0023  MOVE	R5	R1
      0x5C180000,  //  0024  MOVE	R6	R0
      0x5C1C0800,  //  0025  MOVE	R7	R4
      0x60200012,  //  0026  GETGBL	R8	G18
      0x7C200000,  //  0027  CALL	R8	0
      0x8C240306,  //  0028  GETMET	R9	R1	K6
      0x5C2C0600,  //  0029  MOVE	R11	R3
      0x7C240400,  //  002A  CALL	R9	2
      0x40241009,  //  002B  CONNECT	R9	R8	R9
      0x7C140600,  //  002C  CALL	R5	3
      0x80040A00,  //  002D  RET	1	R5
      0x600C0012,  //  002E  GETGBL	R3	G18
      0x7C0C0000,  //  002F  CALL	R3	0
      0xB8120E00,  //  0030  GETNGBL	R4	K7
      0x8C100908,  //  0031  GETMET	R4	R4	K8
      0x5C180000,  //  0032  MOVE	R6	R0
      0x7C100400,  //  0033  CALL	R4	2
      0x94140909,  //  0034  GETIDX	R5	R4	K9
      0x9418090A,  //  0035  GETIDX	R6	R4	K10
      0x941C090B,  //  0036  GETIDX	R7	R4	K11
      0x6020000C,  //  0037  GETGBL	R8	G12
      0x5C240A00,  //  0038  MOVE	R9	R5
      0x7C200200,  //  0039  CALL	R8	1
      0x58240009,  //  003A  LDCONST	R9	K9
      0x5429FFFE,  //  003B  LDINT	R10	-1
      0x142C1208,  //  003C  LT	R11	R9	R8
      0x782E0042,  //  003D  JMPF	R11	#0081
      0x8C2C050C,  //  003E  GETMET	R11	R2	K12
      0x5C340A00,  //  003F  MOVE	R13	R5
      0x5838000D,  //  0040  LDCONST	R14	K13
      0x5C3C1200,  //  0041  MOVE	R15	R9
      0x7C2C0800,  //  0042  CALL	R11	4
      0x4C300000,  //  0043  LDNIL	R12
      0x28341709,  //  0044  GE	R13	R11	K9
      0x78360008,  //  0045  JMPF	R13	#004F
      0x1C341609,  //  0046  EQ	R13	R11	R9
      0x78360000,  //  0047  JMPF	R13	#0049
      0xB0061D0F,  //  0048  RAISE	1	K14	K15
      0x0434170A,  //  0049  SUB	R13	R11	K10
      0x4034120D,  //  004A  CONNECT	R13	R9	R13
      0x94300A0D,  //  004B  GETIDX	R12	R5	R13
      0x0038170A,  //  004C  ADD	R14	R11	K10
      0x5C241C00,  //  004D  MOVE	R9	R14
      0x70020002,  //  004E  JMP		#0052
      0x40341310,  //  004F  CONNECT	R13	R9	K16
      0x94300A0D,  //  0050  GETIDX	R12	R5	R13
      0x5C241000,  //  0051  MOVE	R9	R8
      0x8C34050C,  //  0052  GETMET	R13	R2	K12
      0x5C3C1800,  //  0053  MOVE	R15	R12
      0x58400011,  //  0054  LDCONST	R16	K17
      0x7C340600,  //  0055  CALL	R13	3
      0x4C380000,  //  0056  LDNIL	R14
      0x283C1B09,  //  0057  GE	R15	R13	K9
      0x783E0012,  //  0058  JMPF	R15	#006C
      0x8C3C050C,  //  0059  GETMET	R15	R2	K12
      0x5C441800,  //  005A  MOVE	R17	R12
      0x58480012,  //  005B  LDCONST	R18	K18
      0x5C4C1A00,  //  005C  MOVE	R19	R13
      0x7C3C0800,  //  005D  CALL	R15	4
      0x14401F09,  //  005E  LT	R16	R15	K9
      0x78420000,  //  005F  JMPF	R16	#0061
      0xB0062714,  //  0060  RAISE	1	K19	K20
      0x00401B0A,  //  0061  ADD	R16	R13	K10
      0x04441F0A,  //  0062  SUB	R17	R15	K10
      0x40402011,  //  0063  CONNECT	R16	R16	R17
      0x94401810,  //  0064  GETIDX	R16	R12	R16
      0x04441B0A,  //  0065  SUB	R17	R13	K10
      0x40461211,  //  0066  CONNECT	R17	K9	R17
      0x94301811,  //  0067  GETIDX	R12	R12	R17
      0x60480009,  //  0068  GETGBL	R18	G9
      0x5C4C2000,  //  0069  MOVE	R19	R16
      0x7C480200,  //  006A  CALL	R18	1
      0x5C382400,  //  006B  MOVE	R14	R18
      0x1C3C1915,  //  006C  EQ	R15	R12	K21
      0x783E0004,  //  006D  JMPF	R15	#0073
      0x8C3C0703,  //  006E  GETMET	R15	R3	K3
      0x8C440316,  //  006F  GETMET	R17	R1	K22
      0x7C440200,  //  0070  CALL	R17	1
      0x7C3C0400,  //  0071  CALL	R15	2
      0x70020004,  //  0072  JMP		#0078
      0x8C3C0703,  //  0073  GETMET	R15	R3	K3
      0x8C440317,  //  0074  GETMET	R17	R1	K23
      0x5C4C1800,  //  0075  MOVE	R19	R12
      0x7C440400,  //  0076  CALL	R17	2
      0x7C3C0400,  //  0077  CALL	R15	2
      0x4C3C0000,  //  0078  LDNIL	R15
      0x203C1C0F,  //  0079  NE	R15	R14	R15
      0x783E0004,  //  007A  JMPF	R15	#0080
      0x8C3C0703,  //  007B  GETMET	R15	R3	K3
      0x8C440318,  //  007C  GETMET	R17	R1	K24
      0x5C4C1C00,  //  007D  MOVE	R19	R14
      0x7C440400,  //  007E  CALL	R17	2
      0x7C3C0400,  //  007F  CALL	R15	2
      0x7001FFBA,  //  0080  JMP		#003C
      0x4C2C0000,  //  0081  LDNIL	R11
      0x202C0C0B,  //  0082  NE	R11	R6	R11
      0x782E0008,  //  0083  JMPF	R11	#008D
      0x4C2C0000,  //  0084  LDNIL	R11
      0x202C0E0B,  //  0085  NE	R11	R7	R11
      0x782E0005,  //  0086  JMPF	R11	#008D
      0x8C2C0703,  //  0087  GETMET	R11	R3	K3
      0x8C340319,  //  0088  GETMET	R13	R1	K25
      0x5C3C0C00,  //  0089  MOVE	R15	R6
      0x5C400E00,  //  008A  MOVE	R16	R7
      0x7C340600,  //  008B  CALL	R13	3
      0x7C2C0400,  //  008C  CALL	R11	2
      0x5C2C0200,  //  008D  MOVE	R11	R1
      0x5C300000,  //  008E  MOVE	R12	R0
      0x5C340A00,  //  008F  MOVE	R13	R5
      0x5C380600,  //  0090  MOVE	R14	R3
      0x7C2C0600,  //  0091  CALL	R11	3
      0x80041600,  //  0092  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: match
********************************************************************/
be_local_closure(Rule_Matcher_match,   /* name */
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
    /* K0   */  be_nested_str(matchers),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(match),
    /* K3   */  be_const_int(1),
    }),
    &be_const_str_match,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x4C080000,  //  0004  LDNIL	R2
      0x80040400,  //  0005  RET	1	R2
      0x5C080200,  //  0006  MOVE	R2	R1
      0x580C0001,  //  0007  LDCONST	R3	K1
      0x6010000C,  //  0008  GETGBL	R4	G12
      0x88140100,  //  0009  GETMBR	R5	R0	K0
      0x7C100200,  //  000A  CALL	R4	1
      0x14100604,  //  000B  LT	R4	R3	R4
      0x7812000C,  //  000C  JMPF	R4	#001A
      0x88100100,  //  000D  GETMBR	R4	R0	K0
      0x94100803,  //  000E  GETIDX	R4	R4	R3
      0x8C100902,  //  000F  GETMET	R4	R4	K2
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C100400,  //  0011  CALL	R4	2
      0x5C080800,  //  0012  MOVE	R2	R4
      0x4C100000,  //  0013  LDNIL	R4
      0x1C100404,  //  0014  EQ	R4	R2	R4
      0x78120001,  //  0015  JMPF	R4	#0018
      0x4C100000,  //  0016  LDNIL	R4
      0x80040800,  //  0017  RET	1	R4
      0x000C0703,  //  0018  ADD	R3	R3	K3
      0x7001FFED,  //  0019  JMP		#0008
      0x80040400,  //  001A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Rule_Matcher
********************************************************************/
be_local_class(Rule_Matcher,
    3,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(Rule_Matcher_AND_List, -1), be_const_class(be_class_Rule_Matcher_AND_List) },
        { be_const_key(tostring, 0), be_const_closure(Rule_Matcher_tostring_closure) },
        { be_const_key(match, 6), be_const_closure(Rule_Matcher_match_closure) },
        { be_const_key(init, 5), be_const_closure(Rule_Matcher_init_closure) },
        { be_const_key(Rule_Matcher_Array, 11), be_const_class(be_class_Rule_Matcher_Array) },
        { be_const_key(rule, -1), be_const_var(0) },
        { be_const_key(Rule_Matcher_Key, -1), be_const_class(be_class_Rule_Matcher_Key) },
        { be_const_key(Rule_Matcher_Operator, 8), be_const_class(be_class_Rule_Matcher_Operator) },
        { be_const_key(trigger, -1), be_const_var(1) },
        { be_const_key(Rule_Matcher_Wildcard, -1), be_const_class(be_class_Rule_Matcher_Wildcard) },
        { be_const_key(matchers, 2), be_const_var(2) },
        { be_const_key(parse, -1), be_const_static_closure(Rule_Matcher_parse_closure) },
    })),
    (bstring*) &be_const_str_Rule_Matcher
);
/*******************************************************************/

void be_load_Rule_Matcher_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Rule_Matcher);
    be_setglobal(vm, "Rule_Matcher");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
