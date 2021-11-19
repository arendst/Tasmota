/********************************************************************
 * Berry module `animate`
 * 
 * To use: `import animate`
 * 
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_rotate_init,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_string("init", 380752755, 4),
    /* K1   */  be_nested_string("closure", 1548407746, 7),
    /* K2   */  be_nested_string("code", -114201356, 4),
    /* K3   */  be_nested_string("push", -2022703139, 4),
    /* K4   */  be_nested_string("animate", -409180496, 7),
    /* K5   */  be_nested_string("ins_ramp", 1068049360, 8),
    /* K6   */  be_nested_string("ins_goto", 1342843963, 8),
    /* K7   */  be_const_int(0),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[28]) {  /* code */
      0x60140003,  //  0000  GETGBL	R5	G3
      0x5C180000,  //  0001  MOVE	R6	R0
      0x7C140200,  //  0002  CALL	R5	1
      0x8C140B00,  //  0003  GETMET	R5	R5	K0
      0x7C140200,  //  0004  CALL	R5	1
      0x4C140000,  //  0005  LDNIL	R5
      0x20140205,  //  0006  NE	R5	R1	R5
      0x78160000,  //  0007  JMPF	R5	#0009
      0x90020201,  //  0008  SETMBR	R0	K1	R1
      0x88140102,  //  0009  GETMBR	R5	R0	K2
      0x8C140B03,  //  000A  GETMET	R5	R5	K3
      0xB81E0800,  //  000B  GETNGBL	R7	K4
      0x8C1C0F05,  //  000C  GETMET	R7	R7	K5
      0x5C240400,  //  000D  MOVE	R9	R2
      0x5C280600,  //  000E  MOVE	R10	R3
      0x5C2C0800,  //  000F  MOVE	R11	R4
      0x7C1C0800,  //  0010  CALL	R7	4
      0x7C140400,  //  0011  CALL	R5	2
      0x88140102,  //  0012  GETMBR	R5	R0	K2
      0x8C140B03,  //  0013  GETMET	R5	R5	K3
      0xB81E0800,  //  0014  GETNGBL	R7	K4
      0x8C1C0F06,  //  0015  GETMET	R7	R7	K6
      0x58240007,  //  0016  LDCONST	R9	K7
      0x58280007,  //  0017  LDCONST	R10	K7
      0x582C0007,  //  0018  LDCONST	R11	K7
      0x7C1C0800,  //  0019  CALL	R7	4
      0x7C140400,  //  001A  CALL	R5	2
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_rotate
********************************************************************/
extern const bclass be_class_Animate_engine;
be_local_class(Animate_rotate,
    0,
    &be_class_Animate_engine,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(Animate_rotate_init_closure) },
    })),
    (be_nested_const_str("Animate_rotate", -787188142, 14))
);

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_from_to_init,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_string("init", 380752755, 4),
    /* K1   */  be_nested_string("closure", 1548407746, 7),
    /* K2   */  be_nested_string("code", -114201356, 4),
    /* K3   */  be_nested_string("push", -2022703139, 4),
    /* K4   */  be_nested_string("animate", -409180496, 7),
    /* K5   */  be_nested_string("ins_ramp", 1068049360, 8),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[19]) {  /* code */
      0x60140003,  //  0000  GETGBL	R5	G3
      0x5C180000,  //  0001  MOVE	R6	R0
      0x7C140200,  //  0002  CALL	R5	1
      0x8C140B00,  //  0003  GETMET	R5	R5	K0
      0x7C140200,  //  0004  CALL	R5	1
      0x4C140000,  //  0005  LDNIL	R5
      0x20140205,  //  0006  NE	R5	R1	R5
      0x78160000,  //  0007  JMPF	R5	#0009
      0x90020201,  //  0008  SETMBR	R0	K1	R1
      0x88140102,  //  0009  GETMBR	R5	R0	K2
      0x8C140B03,  //  000A  GETMET	R5	R5	K3
      0xB81E0800,  //  000B  GETNGBL	R7	K4
      0x8C1C0F05,  //  000C  GETMET	R7	R7	K5
      0x5C240400,  //  000D  MOVE	R9	R2
      0x5C280600,  //  000E  MOVE	R10	R3
      0x5C2C0800,  //  000F  MOVE	R11	R4
      0x7C1C0800,  //  0010  CALL	R7	4
      0x7C140400,  //  0011  CALL	R5	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_from_to
********************************************************************/
extern const bclass be_class_Animate_engine;
be_local_class(Animate_from_to,
    0,
    &be_class_Animate_engine,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(Animate_from_to_init_closure) },
    })),
    (be_nested_const_str("Animate_from_to", 1699049867, 15))
);

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_back_forth_init,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_string("init", 380752755, 4),
    /* K1   */  be_nested_string("closure", 1548407746, 7),
    /* K2   */  be_nested_string("code", -114201356, 4),
    /* K3   */  be_nested_string("push", -2022703139, 4),
    /* K4   */  be_nested_string("animate", -409180496, 7),
    /* K5   */  be_nested_string("ins_ramp", 1068049360, 8),
    /* K6   */  be_const_int(2),
    /* K7   */  be_nested_string("ins_goto", 1342843963, 8),
    /* K8   */  be_const_int(0),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[37]) {  /* code */
      0x60140003,  //  0000  GETGBL	R5	G3
      0x5C180000,  //  0001  MOVE	R6	R0
      0x7C140200,  //  0002  CALL	R5	1
      0x8C140B00,  //  0003  GETMET	R5	R5	K0
      0x7C140200,  //  0004  CALL	R5	1
      0x4C140000,  //  0005  LDNIL	R5
      0x20140205,  //  0006  NE	R5	R1	R5
      0x78160000,  //  0007  JMPF	R5	#0009
      0x90020201,  //  0008  SETMBR	R0	K1	R1
      0x88140102,  //  0009  GETMBR	R5	R0	K2
      0x8C140B03,  //  000A  GETMET	R5	R5	K3
      0xB81E0800,  //  000B  GETNGBL	R7	K4
      0x8C1C0F05,  //  000C  GETMET	R7	R7	K5
      0x5C240400,  //  000D  MOVE	R9	R2
      0x5C280600,  //  000E  MOVE	R10	R3
      0x0C2C0906,  //  000F  DIV	R11	R4	K6
      0x7C1C0800,  //  0010  CALL	R7	4
      0x7C140400,  //  0011  CALL	R5	2
      0x88140102,  //  0012  GETMBR	R5	R0	K2
      0x8C140B03,  //  0013  GETMET	R5	R5	K3
      0xB81E0800,  //  0014  GETNGBL	R7	K4
      0x8C1C0F05,  //  0015  GETMET	R7	R7	K5
      0x5C240600,  //  0016  MOVE	R9	R3
      0x5C280400,  //  0017  MOVE	R10	R2
      0x0C2C0906,  //  0018  DIV	R11	R4	K6
      0x7C1C0800,  //  0019  CALL	R7	4
      0x7C140400,  //  001A  CALL	R5	2
      0x88140102,  //  001B  GETMBR	R5	R0	K2
      0x8C140B03,  //  001C  GETMET	R5	R5	K3
      0xB81E0800,  //  001D  GETNGBL	R7	K4
      0x8C1C0F07,  //  001E  GETMET	R7	R7	K7
      0x58240008,  //  001F  LDCONST	R9	K8
      0x58280008,  //  0020  LDCONST	R10	K8
      0x582C0008,  //  0021  LDCONST	R11	K8
      0x7C1C0800,  //  0022  CALL	R7	4
      0x7C140400,  //  0023  CALL	R5	2
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_back_forth
********************************************************************/
extern const bclass be_class_Animate_engine;
be_local_class(Animate_back_forth,
    0,
    &be_class_Animate_engine,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(Animate_back_forth_init_closure) },
    })),
    (be_nested_const_str("Animate_back_forth", 5319526, 18))
);

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_ins_goto_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("pc_rel", 991921176, 6),
    /* K1   */  be_nested_string("pc_abs", 920256495, 6),
    /* K2   */  be_nested_string("duration", 799079693, 8),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
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
** Solidified class: Animate_ins_goto
********************************************************************/
be_local_class(Animate_ins_goto,
    3,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("pc_rel", 991921176, 6, -1), be_const_var(0) },
        { be_nested_key("duration", 799079693, 8, -1), be_const_var(2) },
        { be_nested_key("pc_abs", 920256495, 6, -1), be_const_var(1) },
        { be_nested_key("init", 380752755, 4, 2), be_const_closure(Animate_ins_goto_init_closure) },
    })),
    (be_nested_const_str("Animate_ins_goto", 1667367043, 16))
);

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_ins_ramp_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    4,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("a", -468965076, 1),
    /* K1   */  be_nested_string("b", -418632219, 1),
    /* K2   */  be_nested_string("duration", 799079693, 8),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
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
** Solidified class: Animate_ins_ramp
********************************************************************/
be_local_class(Animate_ins_ramp,
    3,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("a", -468965076, 1, -1), be_const_var(0) },
        { be_nested_key("b", -418632219, 1, 2), be_const_var(1) },
        { be_nested_key("duration", 799079693, 8, -1), be_const_var(2) },
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(Animate_ins_ramp_init_closure) },
    })),
    (be_nested_const_str("Animate_ins_ramp", 785058280, 16))
);

/********************************************************************
** Solidified function: is_running
********************************************************************/
be_local_closure(Animate_engine_is_running,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("running", 343848780, 7),
    }),
    (be_nested_const_str("is_running", -2068120035, 10)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run
********************************************************************/
be_local_closure(Animate_engine_run,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_string("tasmota", 424643812, 7),
    /* K1   */  be_nested_string("millis", 1214679063, 6),
    /* K2   */  be_nested_string("ins_time", -1314721743, 8),
    /* K3   */  be_nested_string("running", 343848780, 7),
    /* K4   */  be_nested_string("add_driver", 1654458371, 10),
    }),
    (be_nested_const_str("run", 718098122, 3)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[15]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0003,  //  0002  JMPF	R2	#0007
      0xB80A0000,  //  0003  GETNGBL	R2	K0
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x5C040400,  //  0006  MOVE	R1	R2
      0x90020401,  //  0007  SETMBR	R0	K2	R1
      0x50080200,  //  0008  LDBOOL	R2	1	0
      0x90020602,  //  0009  SETMBR	R0	K3	R2
      0xB80A0000,  //  000A  GETNGBL	R2	K0
      0x8C080504,  //  000B  GETMET	R2	R2	K4
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C080400,  //  000D  CALL	R2	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Animate_engine_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        4,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_string("Animate next value:", 443143038, 19),
        }),
        (be_nested_const_str("_anonymous_", 1957281476, 11)),
        ((bstring*) &be_const_str_input),
        ( &(const binstruction[ 5]) {  /* code */
          0x60040001,  //  0000  GETGBL	R1	G1
          0x58080000,  //  0001  LDCONST	R2	K0
          0x5C0C0000,  //  0002  MOVE	R3	R0
          0x7C040400,  //  0003  CALL	R1	2
          0x80000000,  //  0004  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_string("code", -114201356, 4),
    /* K1   */  be_nested_string("closure", 1548407746, 7),
    /* K2   */  be_nested_string("pc", 1313756516, 2),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_string("ins_time", -1314721743, 8),
    /* K5   */  be_nested_string("running", 343848780, 7),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[10]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x84040000,  //  0003  CLOSURE	R1	P0
      0x90020201,  //  0004  SETMBR	R0	K1	R1
      0x90020503,  //  0005  SETMBR	R0	K2	K3
      0x90020903,  //  0006  SETMBR	R0	K4	K3
      0x50040000,  //  0007  LDBOOL	R1	0	0
      0x90020A01,  //  0008  SETMBR	R0	K5	R1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(Animate_engine_every_50ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_string("animate", -409180496, 7),
    }),
    (be_nested_const_str("every_50ms", -1911083288, 10)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Animate_engine_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("running", 343848780, 7),
    /* K1   */  be_nested_string("tasmota", 424643812, 7),
    /* K2   */  be_nested_string("remove_driver", 1030243768, 13),
    }),
    (be_nested_const_str("stop", -883741979, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 7]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0xB8060200,  //  0002  GETNGBL	R1	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x5C0C0000,  //  0004  MOVE	R3	R0
      0x7C040400,  //  0005  CALL	R1	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: animate
********************************************************************/
be_local_closure(Animate_engine_animate,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_string("running", 343848780, 7),
    /* K1   */  be_nested_string("tasmota", 424643812, 7),
    /* K2   */  be_nested_string("millis", 1214679063, 6),
    /* K3   */  be_nested_string("ins_time", -1314721743, 8),
    /* K4   */  be_nested_string("pc", 1313756516, 2),
    /* K5   */  be_nested_string("code", -114201356, 4),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_string("internal_error", -1775809127, 14),
    /* K8   */  be_nested_string("Animate pc is out of range", 1854929421, 26),
    /* K9   */  be_nested_string("animate", -409180496, 7),
    /* K10  */  be_nested_string("ins_ramp", 1068049360, 8),
    /* K11  */  be_nested_string("closure", 1548407746, 7),
    /* K12  */  be_nested_string("duration", 799079693, 8),
    /* K13  */  be_nested_string("scale_uint", -1204156202, 10),
    /* K14  */  be_nested_string("a", -468965076, 1),
    /* K15  */  be_nested_string("b", -418632219, 1),
    /* K16  */  be_const_int(1),
    /* K17  */  be_nested_string("ins_goto", 1342843963, 8),
    /* K18  */  be_nested_string("pc_rel", 991921176, 6),
    /* K19  */  be_nested_string("pc_abs", 920256495, 6),
    /* K20  */  be_nested_string("unknown instruction", 1093911841, 19),
    }),
    (be_nested_const_str("animate", -409180496, 7)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[93]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0000,  //  0001  JMPT	R2	#0003
      0x80000400,  //  0002  RET	0
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0003,  //  0005  JMPF	R2	#000A
      0xB80A0200,  //  0006  GETNGBL	R2	K1
      0x8C080502,  //  0007  GETMET	R2	R2	K2
      0x7C080200,  //  0008  CALL	R2	1
      0x5C040400,  //  0009  MOVE	R1	R2
      0x50080200,  //  000A  LDBOOL	R2	1	0
      0x780A004F,  //  000B  JMPF	R2	#005C
      0x88080103,  //  000C  GETMBR	R2	R0	K3
      0x04080202,  //  000D  SUB	R2	R1	R2
      0x880C0104,  //  000E  GETMBR	R3	R0	K4
      0x6010000C,  //  000F  GETGBL	R4	G12
      0x88140105,  //  0010  GETMBR	R5	R0	K5
      0x7C100200,  //  0011  CALL	R4	1
      0x280C0604,  //  0012  GE	R3	R3	R4
      0x780E0002,  //  0013  JMPF	R3	#0017
      0x500C0000,  //  0014  LDBOOL	R3	0	0
      0x90020003,  //  0015  SETMBR	R0	K0	R3
      0x70020044,  //  0016  JMP		#005C
      0x880C0104,  //  0017  GETMBR	R3	R0	K4
      0x140C0706,  //  0018  LT	R3	R3	K6
      0x780E0000,  //  0019  JMPF	R3	#001B
      0xB0060F08,  //  001A  RAISE	1	K7	K8
      0x880C0104,  //  001B  GETMBR	R3	R0	K4
      0x88100105,  //  001C  GETMBR	R4	R0	K5
      0x940C0803,  //  001D  GETIDX	R3	R4	R3
      0x6014000F,  //  001E  GETGBL	R5	G15
      0x5C180600,  //  001F  MOVE	R6	R3
      0xB81E1200,  //  0020  GETNGBL	R7	K9
      0x881C0F0A,  //  0021  GETMBR	R7	R7	K10
      0x7C140400,  //  0022  CALL	R5	2
      0x7816001B,  //  0023  JMPF	R5	#0040
      0x8810010B,  //  0024  GETMBR	R4	R0	K11
      0x8814070C,  //  0025  GETMBR	R5	R3	K12
      0x14140405,  //  0026  LT	R5	R2	R5
      0x7816000C,  //  0027  JMPF	R5	#0035
      0xB8160200,  //  0028  GETNGBL	R5	K1
      0x8C140B0D,  //  0029  GETMET	R5	R5	K13
      0x5C1C0400,  //  002A  MOVE	R7	R2
      0x58200006,  //  002B  LDCONST	R8	K6
      0x8824070C,  //  002C  GETMBR	R9	R3	K12
      0x8828070E,  //  002D  GETMBR	R10	R3	K14
      0x882C070F,  //  002E  GETMBR	R11	R3	K15
      0x7C140C00,  //  002F  CALL	R5	6
      0x5C180800,  //  0030  MOVE	R6	R4
      0x5C1C0A00,  //  0031  MOVE	R7	R5
      0x7C180200,  //  0032  CALL	R6	1
      0x70020027,  //  0033  JMP		#005C
      0x70020009,  //  0034  JMP		#003F
      0x5C140800,  //  0035  MOVE	R5	R4
      0x8818070F,  //  0036  GETMBR	R6	R3	K15
      0x7C140200,  //  0037  CALL	R5	1
      0x88140104,  //  0038  GETMBR	R5	R0	K4
      0x00140B10,  //  0039  ADD	R5	R5	K16
      0x90020805,  //  003A  SETMBR	R0	K4	R5
      0x8814070C,  //  003B  GETMBR	R5	R3	K12
      0x04140405,  //  003C  SUB	R5	R2	R5
      0x04140205,  //  003D  SUB	R5	R1	R5
      0x90020605,  //  003E  SETMBR	R0	K3	R5
      0x7002001A,  //  003F  JMP		#005B
      0x6010000F,  //  0040  GETGBL	R4	G15
      0x5C140600,  //  0041  MOVE	R5	R3
      0xB81A1200,  //  0042  GETNGBL	R6	K9
      0x88180D11,  //  0043  GETMBR	R6	R6	K17
      0x7C100400,  //  0044  CALL	R4	2
      0x78120013,  //  0045  JMPF	R4	#005A
      0x8810070C,  //  0046  GETMBR	R4	R3	K12
      0x14100404,  //  0047  LT	R4	R2	R4
      0x78120001,  //  0048  JMPF	R4	#004B
      0x70020011,  //  0049  JMP		#005C
      0x7002000D,  //  004A  JMP		#0059
      0x88100712,  //  004B  GETMBR	R4	R3	K18
      0x20100906,  //  004C  NE	R4	R4	K6
      0x78120004,  //  004D  JMPF	R4	#0053
      0x88100104,  //  004E  GETMBR	R4	R0	K4
      0x88140712,  //  004F  GETMBR	R5	R3	K18
      0x00100805,  //  0050  ADD	R4	R4	R5
      0x90020804,  //  0051  SETMBR	R0	K4	R4
      0x70020001,  //  0052  JMP		#0055
      0x88100713,  //  0053  GETMBR	R4	R3	K19
      0x90020804,  //  0054  SETMBR	R0	K4	R4
      0x8810070C,  //  0055  GETMBR	R4	R3	K12
      0x04100404,  //  0056  SUB	R4	R2	R4
      0x04100204,  //  0057  SUB	R4	R1	R4
      0x90020604,  //  0058  SETMBR	R0	K3	R4
      0x70020000,  //  0059  JMP		#005B
      0xB0060F14,  //  005A  RAISE	1	K7	K20
      0x7001FFAD,  //  005B  JMP		#000A
      0x80000000,  //  005C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autorun
********************************************************************/
be_local_closure(Animate_engine_autorun,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_string("run", 718098122, 3),
    /* K1   */  be_nested_string("tasmota", 424643812, 7),
    /* K2   */  be_nested_string("add_driver", 1654458371, 10),
    }),
    (be_nested_const_str("autorun", 1447527407, 7)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 8]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0xB80A0200,  //  0003  GETNGBL	R2	K1
      0x8C080502,  //  0004  GETMET	R2	R2	K2
      0x5C100000,  //  0005  MOVE	R4	R0
      0x7C080400,  //  0006  CALL	R2	2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Animate_engine
********************************************************************/
be_local_class(Animate_engine,
    5,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("running", 343848780, 7, -1), be_const_var(4) },
        { be_nested_key("is_running", -2068120035, 10, 7), be_const_closure(Animate_engine_is_running_closure) },
        { be_nested_key("run", 718098122, 3, -1), be_const_closure(Animate_engine_run_closure) },
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(Animate_engine_init_closure) },
        { be_nested_key("every_50ms", -1911083288, 10, -1), be_const_closure(Animate_engine_every_50ms_closure) },
        { be_nested_key("stop", -883741979, 4, -1), be_const_closure(Animate_engine_stop_closure) },
        { be_nested_key("pc", 1313756516, 2, 4), be_const_var(2) },
        { be_nested_key("ins_time", -1314721743, 8, -1), be_const_var(3) },
        { be_nested_key("animate", -409180496, 7, 9), be_const_closure(Animate_engine_animate_closure) },
        { be_nested_key("code", -114201356, 4, 6), be_const_var(0) },
        { be_nested_key("closure", 1548407746, 7, -1), be_const_var(1) },
        { be_nested_key("autorun", 1447527407, 7, -1), be_const_closure(Animate_engine_autorun_closure) },
    })),
    (be_nested_const_str("Animate_engine", 1498417667, 14))
);

/********************************************************************
** Solidified module: animate
********************************************************************/
be_local_module(animate,
    "animate",
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("rotate", -1510671094, 6, 2), be_const_class(be_class_Animate_rotate) },
        { be_nested_key("from_to", 21625507, 7, 3), be_const_class(be_class_Animate_from_to) },
        { be_nested_key("back_forth", -1629925234, 10, -1), be_const_class(be_class_Animate_back_forth) },
        { be_nested_key("ins_goto", 1342843963, 8, -1), be_const_class(be_class_Animate_ins_goto) },
        { be_nested_key("ins_ramp", 1068049360, 8, -1), be_const_class(be_class_Animate_ins_ramp) },
        { be_nested_key("engine", -301606853, 6, -1), be_const_class(be_class_Animate_engine) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(animate);
/********************************************************************/
