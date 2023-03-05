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
** Solidified function: init
********************************************************************/
be_local_closure(Rule_Matcher_Operator_init,   /* name */
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
    /* K0   */  be_nested_str(op_func),
    /* K1   */  be_nested_str(op_value),
    /* K2   */  be_nested_str(op_str),
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(_X3CMatcher_X20op_X20_X27),
    /* K1   */  be_nested_str(op_str),
    /* K2   */  be_nested_str(_X27_X20val_X3D_X27),
    /* K3   */  be_nested_str(op_value),
    /* K4   */  be_nested_str(_X27_X3E),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x00060001,  //  0001  ADD	R1	K0	R1
      0x00040302,  //  0002  ADD	R1	R1	K2
      0x60080008,  //  0003  GETGBL	R2	G8
      0x880C0103,  //  0004  GETMBR	R3	R0	K3
      0x7C080200,  //  0005  CALL	R2	1
      0x00040202,  //  0006  ADD	R1	R1	R2
      0x00040304,  //  0007  ADD	R1	R1	K4
      0x80040200,  //  0008  RET	1	R1
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
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(op_value, -1), be_const_var(2) },
        { be_const_key(op_func, 2), be_const_var(0) },
        { be_const_key(tostring, -1), be_const_closure(Rule_Matcher_Operator_tostring_closure) },
        { be_const_key(init, -1), be_const_closure(Rule_Matcher_Operator_init_closure) },
        { be_const_key(match, 5), be_const_closure(Rule_Matcher_Operator_match_closure) },
        { be_const_key(op_str, -1), be_const_var(1) },
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
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(index),
    }),
    &be_const_str_match,
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
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
      0x600C000C,  //  000C  GETGBL	R3	G12
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C0C0200,  //  000E  CALL	R3	1
      0x28080403,  //  000F  GE	R2	R2	R3
      0x780A0001,  //  0010  JMPF	R2	#0013
      0x4C080000,  //  0011  LDNIL	R2
      0x80040400,  //  0012  RET	1	R2
      0x88080100,  //  0013  GETMBR	R2	R0	K0
      0x94080202,  //  0014  GETIDX	R2	R1	R2
      0x80040400,  //  0015  RET	1	R2
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
** Solidified function: op_parse
********************************************************************/
be_local_closure(Rule_Matcher_op_parse,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 8]) {
      be_nested_proto(
        5,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        &be_const_str_op_eq_str,
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x60080008,  //  0000  GETGBL	R2	G8
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x600C0008,  //  0003  GETGBL	R3	G8
          0x5C100200,  //  0004  MOVE	R4	R1
          0x7C0C0200,  //  0005  CALL	R3	1
          0x1C080403,  //  0006  EQ	R2	R2	R3
          0x80040400,  //  0007  RET	1	R2
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        &be_const_str_op_neq_str,
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x60080008,  //  0000  GETGBL	R2	G8
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x600C0008,  //  0003  GETGBL	R3	G8
          0x5C100200,  //  0004  MOVE	R4	R1
          0x7C0C0200,  //  0005  CALL	R3	1
          0x20080403,  //  0006  NE	R2	R2	R3
          0x80040400,  //  0007  RET	1	R2
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
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
        ( &(const binstruction[ 8]) {  /* code */
          0x6008000A,  //  0000  GETGBL	R2	G10
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x600C000A,  //  0003  GETGBL	R3	G10
          0x5C100200,  //  0004  MOVE	R4	R1
          0x7C0C0200,  //  0005  CALL	R3	1
          0x1C080403,  //  0006  EQ	R2	R2	R3
          0x80040400,  //  0007  RET	1	R2
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
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
        ( &(const binstruction[ 8]) {  /* code */
          0x6008000A,  //  0000  GETGBL	R2	G10
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x600C000A,  //  0003  GETGBL	R3	G10
          0x5C100200,  //  0004  MOVE	R4	R1
          0x7C0C0200,  //  0005  CALL	R3	1
          0x20080403,  //  0006  NE	R2	R2	R3
          0x80040400,  //  0007  RET	1	R2
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
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
        ( &(const binstruction[ 8]) {  /* code */
          0x6008000A,  //  0000  GETGBL	R2	G10
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x600C000A,  //  0003  GETGBL	R3	G10
          0x5C100200,  //  0004  MOVE	R4	R1
          0x7C0C0200,  //  0005  CALL	R3	1
          0x24080403,  //  0006  GT	R2	R2	R3
          0x80040400,  //  0007  RET	1	R2
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
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
        ( &(const binstruction[ 8]) {  /* code */
          0x6008000A,  //  0000  GETGBL	R2	G10
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x600C000A,  //  0003  GETGBL	R3	G10
          0x5C100200,  //  0004  MOVE	R4	R1
          0x7C0C0200,  //  0005  CALL	R3	1
          0x28080403,  //  0006  GE	R2	R2	R3
          0x80040400,  //  0007  RET	1	R2
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
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
        ( &(const binstruction[ 8]) {  /* code */
          0x6008000A,  //  0000  GETGBL	R2	G10
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x600C000A,  //  0003  GETGBL	R3	G10
          0x5C100200,  //  0004  MOVE	R4	R1
          0x7C0C0200,  //  0005  CALL	R3	1
          0x14080403,  //  0006  LT	R2	R2	R3
          0x80040400,  //  0007  RET	1	R2
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
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
        ( &(const binstruction[ 8]) {  /* code */
          0x6008000A,  //  0000  GETGBL	R2	G10
          0x5C0C0000,  //  0001  MOVE	R3	R0
          0x7C080200,  //  0002  CALL	R2	1
          0x600C000A,  //  0003  GETGBL	R3	G10
          0x5C100200,  //  0004  MOVE	R4	R1
          0x7C0C0200,  //  0005  CALL	R3	1
          0x18080403,  //  0006  LE	R2	R2	R3
          0x80040400,  //  0007  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_class(be_class_Rule_Matcher),
    /* K1   */  be_nested_str(_X3D_X3D),
    /* K2   */  be_nested_str(_X21_X3D_X3D),
    /* K3   */  be_nested_str(_X3D),
    /* K4   */  be_nested_str(_X21_X3D),
    /* K5   */  be_nested_str(_X3E),
    /* K6   */  be_nested_str(_X3E_X3D),
    /* K7   */  be_nested_str(_X3C),
    /* K8   */  be_nested_str(_X3C_X3D),
    }),
    &be_const_str_op_parse,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x84080000,  //  0001  CLOSURE	R2	P0
      0x840C0001,  //  0002  CLOSURE	R3	P1
      0x84100002,  //  0003  CLOSURE	R4	P2
      0x84140003,  //  0004  CLOSURE	R5	P3
      0x84180004,  //  0005  CLOSURE	R6	P4
      0x841C0005,  //  0006  CLOSURE	R7	P5
      0x84200006,  //  0007  CLOSURE	R8	P6
      0x84240007,  //  0008  CLOSURE	R9	P7
      0x1C280101,  //  0009  EQ	R10	R0	K1
      0x782A0001,  //  000A  JMPF	R10	#000D
      0x80040400,  //  000B  RET	1	R2
      0x7002001A,  //  000C  JMP		#0028
      0x1C280102,  //  000D  EQ	R10	R0	K2
      0x782A0001,  //  000E  JMPF	R10	#0011
      0x80040600,  //  000F  RET	1	R3
      0x70020016,  //  0010  JMP		#0028
      0x1C280103,  //  0011  EQ	R10	R0	K3
      0x782A0001,  //  0012  JMPF	R10	#0015
      0x80040800,  //  0013  RET	1	R4
      0x70020012,  //  0014  JMP		#0028
      0x1C280104,  //  0015  EQ	R10	R0	K4
      0x782A0001,  //  0016  JMPF	R10	#0019
      0x80040A00,  //  0017  RET	1	R5
      0x7002000E,  //  0018  JMP		#0028
      0x1C280105,  //  0019  EQ	R10	R0	K5
      0x782A0001,  //  001A  JMPF	R10	#001D
      0x80040C00,  //  001B  RET	1	R6
      0x7002000A,  //  001C  JMP		#0028
      0x1C280106,  //  001D  EQ	R10	R0	K6
      0x782A0001,  //  001E  JMPF	R10	#0021
      0x80040E00,  //  001F  RET	1	R7
      0x70020006,  //  0020  JMP		#0028
      0x1C280107,  //  0021  EQ	R10	R0	K7
      0x782A0001,  //  0022  JMPF	R10	#0025
      0x80041000,  //  0023  RET	1	R8
      0x70020002,  //  0024  JMP		#0028
      0x1C280108,  //  0025  EQ	R10	R0	K8
      0x782A0000,  //  0026  JMPF	R10	#0028
      0x80041200,  //  0027  RET	1	R9
      0x80000000,  //  0028  RET	0
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
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_const_class(be_class_Rule_Matcher),
    /* K1   */  be_nested_str(string),
    /* K2   */  be_nested_str(tasmota),
    /* K3   */  be_nested_str(find_op),
    /* K4   */  be_const_int(0),
    /* K5   */  be_const_int(1),
    /* K6   */  be_const_int(2),
    /* K7   */  be_nested_str(find),
    /* K8   */  be_nested_str(_X23),
    /* K9   */  be_nested_str(pattern_error),
    /* K10  */  be_nested_str(empty_X20pattern_X20not_X20allowed),
    /* K11  */  be_const_int(2147483647),
    /* K12  */  be_nested_str(_X5B),
    /* K13  */  be_nested_str(_X5D),
    /* K14  */  be_nested_str(value_error),
    /* K15  */  be_nested_str(missing_X20_X27_X5D_X27_X20in_X20rule_X20pattern),
    /* K16  */  be_nested_str(_X3F),
    /* K17  */  be_nested_str(push),
    /* K18  */  be_nested_str(Rule_Matcher_Wildcard),
    /* K19  */  be_nested_str(Rule_Matcher_Key),
    /* K20  */  be_nested_str(Rule_Matcher_Array),
    /* K21  */  be_nested_str(op_parse),
    /* K22  */  be_nested_str(Rule_Matcher_Operator),
    }),
    &be_const_str_parse,
    &be_const_str_solidified,
    ( &(const binstruction[113]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C0C0003,  //  0003  EQ	R3	R0	R3
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x4C0C0000,  //  0005  LDNIL	R3
      0x80040600,  //  0006  RET	1	R3
      0x600C0012,  //  0007  GETGBL	R3	G18
      0x7C0C0000,  //  0008  CALL	R3	0
      0xB8120400,  //  0009  GETNGBL	R4	K2
      0x8C100903,  //  000A  GETMET	R4	R4	K3
      0x5C180000,  //  000B  MOVE	R6	R0
      0x7C100400,  //  000C  CALL	R4	2
      0x94140904,  //  000D  GETIDX	R5	R4	K4
      0x94180905,  //  000E  GETIDX	R6	R4	K5
      0x941C0906,  //  000F  GETIDX	R7	R4	K6
      0x6020000C,  //  0010  GETGBL	R8	G12
      0x5C240A00,  //  0011  MOVE	R9	R5
      0x7C200200,  //  0012  CALL	R8	1
      0x58240004,  //  0013  LDCONST	R9	K4
      0x5429FFFE,  //  0014  LDINT	R10	-1
      0x142C1208,  //  0015  LT	R11	R9	R8
      0x782E0042,  //  0016  JMPF	R11	#005A
      0x8C2C0507,  //  0017  GETMET	R11	R2	K7
      0x5C340A00,  //  0018  MOVE	R13	R5
      0x58380008,  //  0019  LDCONST	R14	K8
      0x5C3C1200,  //  001A  MOVE	R15	R9
      0x7C2C0800,  //  001B  CALL	R11	4
      0x4C300000,  //  001C  LDNIL	R12
      0x28341704,  //  001D  GE	R13	R11	K4
      0x78360008,  //  001E  JMPF	R13	#0028
      0x1C341609,  //  001F  EQ	R13	R11	R9
      0x78360000,  //  0020  JMPF	R13	#0022
      0xB006130A,  //  0021  RAISE	1	K9	K10
      0x04341705,  //  0022  SUB	R13	R11	K5
      0x4034120D,  //  0023  CONNECT	R13	R9	R13
      0x94300A0D,  //  0024  GETIDX	R12	R5	R13
      0x00381705,  //  0025  ADD	R14	R11	K5
      0x5C241C00,  //  0026  MOVE	R9	R14
      0x70020002,  //  0027  JMP		#002B
      0x4034130B,  //  0028  CONNECT	R13	R9	K11
      0x94300A0D,  //  0029  GETIDX	R12	R5	R13
      0x5C241000,  //  002A  MOVE	R9	R8
      0x8C340507,  //  002B  GETMET	R13	R2	K7
      0x5C3C1800,  //  002C  MOVE	R15	R12
      0x5840000C,  //  002D  LDCONST	R16	K12
      0x7C340600,  //  002E  CALL	R13	3
      0x4C380000,  //  002F  LDNIL	R14
      0x283C1B04,  //  0030  GE	R15	R13	K4
      0x783E0012,  //  0031  JMPF	R15	#0045
      0x8C3C0507,  //  0032  GETMET	R15	R2	K7
      0x5C441800,  //  0033  MOVE	R17	R12
      0x5848000D,  //  0034  LDCONST	R18	K13
      0x5C4C1A00,  //  0035  MOVE	R19	R13
      0x7C3C0800,  //  0036  CALL	R15	4
      0x14401F04,  //  0037  LT	R16	R15	K4
      0x78420000,  //  0038  JMPF	R16	#003A
      0xB0061D0F,  //  0039  RAISE	1	K14	K15
      0x00401B05,  //  003A  ADD	R16	R13	K5
      0x04441F05,  //  003B  SUB	R17	R15	K5
      0x40402011,  //  003C  CONNECT	R16	R16	R17
      0x94401810,  //  003D  GETIDX	R16	R12	R16
      0x04441B05,  //  003E  SUB	R17	R13	K5
      0x40460811,  //  003F  CONNECT	R17	K4	R17
      0x94301811,  //  0040  GETIDX	R12	R12	R17
      0x60480009,  //  0041  GETGBL	R18	G9
      0x5C4C2000,  //  0042  MOVE	R19	R16
      0x7C480200,  //  0043  CALL	R18	1
      0x5C382400,  //  0044  MOVE	R14	R18
      0x1C3C1910,  //  0045  EQ	R15	R12	K16
      0x783E0004,  //  0046  JMPF	R15	#004C
      0x8C3C0711,  //  0047  GETMET	R15	R3	K17
      0x8C440312,  //  0048  GETMET	R17	R1	K18
      0x7C440200,  //  0049  CALL	R17	1
      0x7C3C0400,  //  004A  CALL	R15	2
      0x70020004,  //  004B  JMP		#0051
      0x8C3C0711,  //  004C  GETMET	R15	R3	K17
      0x8C440313,  //  004D  GETMET	R17	R1	K19
      0x5C4C1800,  //  004E  MOVE	R19	R12
      0x7C440400,  //  004F  CALL	R17	2
      0x7C3C0400,  //  0050  CALL	R15	2
      0x4C3C0000,  //  0051  LDNIL	R15
      0x203C1C0F,  //  0052  NE	R15	R14	R15
      0x783E0004,  //  0053  JMPF	R15	#0059
      0x8C3C0711,  //  0054  GETMET	R15	R3	K17
      0x8C440314,  //  0055  GETMET	R17	R1	K20
      0x5C4C1C00,  //  0056  MOVE	R19	R14
      0x7C440400,  //  0057  CALL	R17	2
      0x7C3C0400,  //  0058  CALL	R15	2
      0x7001FFBA,  //  0059  JMP		#0015
      0x4C2C0000,  //  005A  LDNIL	R11
      0x202C0C0B,  //  005B  NE	R11	R6	R11
      0x782E000D,  //  005C  JMPF	R11	#006B
      0x4C2C0000,  //  005D  LDNIL	R11
      0x202C0E0B,  //  005E  NE	R11	R7	R11
      0x782E000A,  //  005F  JMPF	R11	#006B
      0x8C2C0315,  //  0060  GETMET	R11	R1	K21
      0x5C340C00,  //  0061  MOVE	R13	R6
      0x7C2C0400,  //  0062  CALL	R11	2
      0x782E0006,  //  0063  JMPF	R11	#006B
      0x8C300711,  //  0064  GETMET	R12	R3	K17
      0x8C380316,  //  0065  GETMET	R14	R1	K22
      0x5C401600,  //  0066  MOVE	R16	R11
      0x5C440E00,  //  0067  MOVE	R17	R7
      0x5C480C00,  //  0068  MOVE	R18	R6
      0x7C380800,  //  0069  CALL	R14	4
      0x7C300400,  //  006A  CALL	R12	2
      0x5C2C0200,  //  006B  MOVE	R11	R1
      0x5C300000,  //  006C  MOVE	R12	R0
      0x5C340A00,  //  006D  MOVE	R13	R5
      0x5C380600,  //  006E  MOVE	R14	R3
      0x7C2C0600,  //  006F  CALL	R11	3
      0x80041600,  //  0070  RET	1	R11
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
        { be_const_key(rule, -1), be_const_var(0) },
        { be_const_key(tostring, -1), be_const_closure(Rule_Matcher_tostring_closure) },
        { be_const_key(op_parse, -1), be_const_static_closure(Rule_Matcher_op_parse_closure) },
        { be_const_key(init, 0), be_const_closure(Rule_Matcher_init_closure) },
        { be_const_key(Rule_Matcher_Array, 11), be_const_class(be_class_Rule_Matcher_Array) },
        { be_const_key(Rule_Matcher_Key, 6), be_const_class(be_class_Rule_Matcher_Key) },
        { be_const_key(match, -1), be_const_closure(Rule_Matcher_match_closure) },
        { be_const_key(Rule_Matcher_Operator, 8), be_const_class(be_class_Rule_Matcher_Operator) },
        { be_const_key(trigger, -1), be_const_var(1) },
        { be_const_key(Rule_Matcher_Wildcard, -1), be_const_class(be_class_Rule_Matcher_Wildcard) },
        { be_const_key(matchers, 5), be_const_var(2) },
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
