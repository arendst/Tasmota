/********************************************************************
 * Tasmota lib
 * 
 * To use: `import power`
 * 
 * read power values
 *******************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: json_fdump_map
********************************************************************/
be_local_closure(Persist_json_fdump_map,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_literal("json"),
    /* K1   */  be_nested_str_literal("write"),
    /* K2   */  be_nested_str_literal("{"),
    /* K3   */  be_nested_str_literal("keys"),
    /* K4   */  be_nested_str_literal("dump"),
    /* K5   */  be_nested_str_literal(":"),
    /* K6   */  be_nested_str_literal("json_fdump_any"),
    /* K7   */  be_nested_str_literal(","),
    /* K8   */  be_nested_str_literal("stop_iteration"),
    /* K9   */  be_nested_str_literal("}"),
    }),
    (be_nested_const_str("json_fdump_map", -203012643, 14)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[41]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100301,  //  0001  GETMET	R4	R1	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x7C100400,  //  0003  CALL	R4	2
      0x4C100000,  //  0004  LDNIL	R4
      0x60140010,  //  0005  GETGBL	R5	G16
      0x8C180503,  //  0006  GETMET	R6	R2	K3
      0x7C180200,  //  0007  CALL	R6	1
      0x7C140200,  //  0008  CALL	R5	1
      0xA8020017,  //  0009  EXBLK	0	#0022
      0x5C180A00,  //  000A  MOVE	R6	R5
      0x7C180000,  //  000B  CALL	R6	0
      0x4C1C0000,  //  000C  LDNIL	R7
      0x201C0807,  //  000D  NE	R7	R4	R7
      0x781E0002,  //  000E  JMPF	R7	#0012
      0x8C1C0301,  //  000F  GETMET	R7	R1	K1
      0x5C240800,  //  0010  MOVE	R9	R4
      0x7C1C0400,  //  0011  CALL	R7	2
      0x8C1C0301,  //  0012  GETMET	R7	R1	K1
      0x8C240704,  //  0013  GETMET	R9	R3	K4
      0x602C0008,  //  0014  GETGBL	R11	G8
      0x5C300C00,  //  0015  MOVE	R12	R6
      0x7C2C0200,  //  0016  CALL	R11	1
      0x7C240400,  //  0017  CALL	R9	2
      0x7C1C0400,  //  0018  CALL	R7	2
      0x8C1C0301,  //  0019  GETMET	R7	R1	K1
      0x58240005,  //  001A  LDCONST	R9	K5
      0x7C1C0400,  //  001B  CALL	R7	2
      0x8C1C0106,  //  001C  GETMET	R7	R0	K6
      0x5C240200,  //  001D  MOVE	R9	R1
      0x94280406,  //  001E  GETIDX	R10	R2	R6
      0x7C1C0600,  //  001F  CALL	R7	3
      0x58100007,  //  0020  LDCONST	R4	K7
      0x7001FFE7,  //  0021  JMP		#000A
      0x58140008,  //  0022  LDCONST	R5	K8
      0xAC140200,  //  0023  CATCH	R5	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x8C140301,  //  0025  GETMET	R5	R1	K1
      0x581C0009,  //  0026  LDCONST	R7	K9
      0x7C140400,  //  0027  CALL	R5	2
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: setmember
********************************************************************/
be_local_closure(Persist_setmember,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_literal("_p"),
    /* K1   */  be_nested_str_literal("_dirty"),
    }),
    (be_nested_const_str("setmember", 1432909441, 9)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x90020203,  //  0003  SETMBR	R0	K1	R3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: zero
********************************************************************/
be_local_closure(Persist_zero,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_literal("_p"),
    /* K1   */  be_nested_str_literal("_dirty"),
    }),
    (be_nested_const_str("zero", -1955600541, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 6]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x50040200,  //  0003  LDBOOL	R1	1	0
      0x90020201,  //  0004  SETMBR	R0	K1	R1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: member
********************************************************************/
be_local_closure(Persist_member,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_literal("_p"),
    /* K1   */  be_nested_str_literal("find"),
    }),
    (be_nested_const_str("member", 719708611, 6)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: json_fdump
********************************************************************/
be_local_closure(Persist_json_fdump,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_literal("json"),
    /* K1   */  be_nested_str_literal("_p"),
    /* K2   */  be_nested_str_literal("json_fdump_map"),
    /* K3   */  be_nested_str_literal("internal_error"),
    /* K4   */  be_nested_str_literal("persist._p is not a map"),
    }),
    (be_nested_const_str("json_fdump", 1694216580, 10)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[13]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C000F,  //  0001  GETGBL	R3	G15
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x60140013,  //  0003  GETGBL	R5	G19
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0004,  //  0005  JMPF	R3	#000B
      0x8C0C0102,  //  0006  GETMET	R3	R0	K2
      0x5C140200,  //  0007  MOVE	R5	R1
      0x88180101,  //  0008  GETMBR	R6	R0	K1
      0x7C0C0600,  //  0009  CALL	R3	3
      0x70020000,  //  000A  JMP		#000C
      0xB0060704,  //  000B  RAISE	1	K3	K4
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove
********************************************************************/
be_local_closure(Persist_remove,   /* name */
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
    /* K0   */  be_nested_str_literal("_p"),
    /* K1   */  be_nested_str_literal("remove"),
    /* K2   */  be_nested_str_literal("_dirty"),
    }),
    (be_nested_const_str("remove", -611183107, 6)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x50080200,  //  0004  LDBOOL	R2	1	0
      0x90020402,  //  0005  SETMBR	R0	K2	R2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: json_fdump_any
********************************************************************/
be_local_closure(Persist_json_fdump_any,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_literal("json"),
    /* K1   */  be_nested_str_literal("json_fdump_map"),
    /* K2   */  be_nested_str_literal("json_fdump_list"),
    /* K3   */  be_nested_str_literal("write"),
    /* K4   */  be_nested_str_literal("dump"),
    }),
    (be_nested_const_str("json_fdump_any", -946337911, 14)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[27]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x6010000F,  //  0001  GETGBL	R4	G15
      0x5C140400,  //  0002  MOVE	R5	R2
      0x60180013,  //  0003  GETGBL	R6	G19
      0x7C100400,  //  0004  CALL	R4	2
      0x78120004,  //  0005  JMPF	R4	#000B
      0x8C100101,  //  0006  GETMET	R4	R0	K1
      0x5C180200,  //  0007  MOVE	R6	R1
      0x5C1C0400,  //  0008  MOVE	R7	R2
      0x7C100600,  //  0009  CALL	R4	3
      0x7002000E,  //  000A  JMP		#001A
      0x6010000F,  //  000B  GETGBL	R4	G15
      0x5C140400,  //  000C  MOVE	R5	R2
      0x60180012,  //  000D  GETGBL	R6	G18
      0x7C100400,  //  000E  CALL	R4	2
      0x78120004,  //  000F  JMPF	R4	#0015
      0x8C100102,  //  0010  GETMET	R4	R0	K2
      0x5C180200,  //  0011  MOVE	R6	R1
      0x5C1C0400,  //  0012  MOVE	R7	R2
      0x7C100600,  //  0013  CALL	R4	3
      0x70020004,  //  0014  JMP		#001A
      0x8C100303,  //  0015  GETMET	R4	R1	K3
      0x8C180704,  //  0016  GETMET	R6	R3	K4
      0x5C200400,  //  0017  MOVE	R8	R2
      0x7C180400,  //  0018  CALL	R6	2
      0x7C100400,  //  0019  CALL	R4	2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save
********************************************************************/
be_local_closure(Persist_save,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_literal("_filename"),
    /* K1   */  be_nested_str_literal("w"),
    /* K2   */  be_nested_str_literal("json_fdump"),
    /* K3   */  be_nested_str_literal("close"),
    /* K4   */  be_nested_str_literal("write"),
    /* K5   */  be_nested_str_literal("{}"),
    /* K6   */  be_nested_str_literal("_dirty"),
    }),
    (be_nested_const_str("save", -855671224, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[37]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0xA802000B,  //  0001  EXBLK	0	#000E
      0x60080011,  //  0002  GETGBL	R2	G17
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x58100001,  //  0004  LDCONST	R4	K1
      0x7C080400,  //  0005  CALL	R2	2
      0x5C040400,  //  0006  MOVE	R1	R2
      0x8C080102,  //  0007  GETMET	R2	R0	K2
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080303,  //  000A  GETMET	R2	R1	K3
      0x7C080200,  //  000B  CALL	R2	1
      0xA8040001,  //  000C  EXBLK	1	1
      0x70020013,  //  000D  JMP		#0022
      0xAC080002,  //  000E  CATCH	R2	0	2
      0x70020010,  //  000F  JMP		#0021
      0x4C100000,  //  0010  LDNIL	R4
      0x20100204,  //  0011  NE	R4	R1	R4
      0x78120001,  //  0012  JMPF	R4	#0015
      0x8C100303,  //  0013  GETMET	R4	R1	K3
      0x7C100200,  //  0014  CALL	R4	1
      0x60100011,  //  0015  GETGBL	R4	G17
      0x88140100,  //  0016  GETMBR	R5	R0	K0
      0x58180001,  //  0017  LDCONST	R6	K1
      0x7C100400,  //  0018  CALL	R4	2
      0x5C040800,  //  0019  MOVE	R1	R4
      0x8C100304,  //  001A  GETMET	R4	R1	K4
      0x58180005,  //  001B  LDCONST	R6	K5
      0x7C100400,  //  001C  CALL	R4	2
      0x8C100303,  //  001D  GETMET	R4	R1	K3
      0x7C100200,  //  001E  CALL	R4	1
      0xB0040403,  //  001F  RAISE	1	R2	R3
      0x70020000,  //  0020  JMP		#0022
      0xB0080000,  //  0021  RAISE	2	R0	R0
      0x50080000,  //  0022  LDBOOL	R2	0	0
      0x90020C02,  //  0023  SETMBR	R0	K6	R2
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(Persist_load,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_literal("json"),
    /* K1   */  be_nested_str_literal("path"),
    /* K2   */  be_nested_str_literal("exists"),
    /* K3   */  be_nested_str_literal("_filename"),
    /* K4   */  be_nested_str_literal("r"),
    /* K5   */  be_nested_str_literal("load"),
    /* K6   */  be_nested_str_literal("read"),
    /* K7   */  be_nested_str_literal("close"),
    /* K8   */  be_nested_str_literal("_p"),
    /* K9   */  be_nested_str_literal("BRY: failed to load _persist.json"),
    /* K10  */  be_nested_str_literal("_dirty"),
    /* K11  */  be_nested_str_literal("save"),
    }),
    (be_nested_const_str("load", -435725847, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[49]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x4C100000,  //  0003  LDNIL	R4
      0x8C140502,  //  0004  GETMET	R5	R2	K2
      0x881C0103,  //  0005  GETMBR	R7	R0	K3
      0x7C140400,  //  0006  CALL	R5	2
      0x78160025,  //  0007  JMPF	R5	#002E
      0xA802000D,  //  0008  EXBLK	0	#0017
      0x60140011,  //  0009  GETGBL	R5	G17
      0x88180103,  //  000A  GETMBR	R6	R0	K3
      0x581C0004,  //  000B  LDCONST	R7	K4
      0x7C140400,  //  000C  CALL	R5	2
      0x5C0C0A00,  //  000D  MOVE	R3	R5
      0x8C140305,  //  000E  GETMET	R5	R1	K5
      0x8C1C0706,  //  000F  GETMET	R7	R3	K6
      0x7C1C0200,  //  0010  CALL	R7	1
      0x7C140400,  //  0011  CALL	R5	2
      0x5C100A00,  //  0012  MOVE	R4	R5
      0x8C140707,  //  0013  GETMET	R5	R3	K7
      0x7C140200,  //  0014  CALL	R5	1
      0xA8040001,  //  0015  EXBLK	1	1
      0x70020009,  //  0016  JMP		#0021
      0xAC140002,  //  0017  CATCH	R5	0	2
      0x70020006,  //  0018  JMP		#0020
      0x4C1C0000,  //  0019  LDNIL	R7
      0x201C0607,  //  001A  NE	R7	R3	R7
      0x781E0001,  //  001B  JMPF	R7	#001E
      0x8C1C0707,  //  001C  GETMET	R7	R3	K7
      0x7C1C0200,  //  001D  CALL	R7	1
      0xB0040A06,  //  001E  RAISE	1	R5	R6
      0x70020000,  //  001F  JMP		#0021
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0x6014000F,  //  0021  GETGBL	R5	G15
      0x5C180800,  //  0022  MOVE	R6	R4
      0x601C0013,  //  0023  GETGBL	R7	G19
      0x7C140400,  //  0024  CALL	R5	2
      0x78160001,  //  0025  JMPF	R5	#0028
      0x90021004,  //  0026  SETMBR	R0	K8	R4
      0x70020002,  //  0027  JMP		#002B
      0x60140001,  //  0028  GETGBL	R5	G1
      0x58180009,  //  0029  LDCONST	R6	K9
      0x7C140200,  //  002A  CALL	R5	1
      0x50140000,  //  002B  LDBOOL	R5	0	0
      0x90021405,  //  002C  SETMBR	R0	K10	R5
      0x70020001,  //  002D  JMP		#0030
      0x8C14010B,  //  002E  GETMET	R5	R0	K11
      0x7C140200,  //  002F  CALL	R5	1
      0x80000000,  //  0030  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find
********************************************************************/
be_local_closure(Persist_find,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_literal("_p"),
    /* K1   */  be_nested_str_literal("find"),
    }),
    (be_nested_const_str("find", -1108310694, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 6]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x80040600,  //  0005  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Persist_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_literal("_filename"),
    /* K1   */  be_nested_str_literal("_persist.json"),
    /* K2   */  be_nested_str_literal("_p"),
    /* K3   */  be_nested_str_literal("copy"),
    /* K4   */  be_nested_str_literal("load"),
    /* K5   */  be_nested_str_literal("_dirty"),
    }),
    (be_nested_const_str("init", 380752755, 4)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[20]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x6008000F,  //  0001  GETGBL	R2	G15
      0x5C0C0200,  //  0002  MOVE	R3	R1
      0x60100013,  //  0003  GETGBL	R4	G19
      0x7C080400,  //  0004  CALL	R2	2
      0x780A0003,  //  0005  JMPF	R2	#000A
      0x8C080303,  //  0006  GETMET	R2	R1	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x70020002,  //  0009  JMP		#000D
      0x60080013,  //  000A  GETGBL	R2	G19
      0x7C080000,  //  000B  CALL	R2	0
      0x90020402,  //  000C  SETMBR	R0	K2	R2
      0x8C080104,  //  000D  GETMET	R2	R0	K4
      0x88100102,  //  000E  GETMBR	R4	R0	K2
      0x88140100,  //  000F  GETMBR	R5	R0	K0
      0x7C080600,  //  0010  CALL	R2	3
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x90020A02,  //  0012  SETMBR	R0	K5	R2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: json_fdump_list
********************************************************************/
be_local_closure(Persist_json_fdump_list,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_literal("json"),
    /* K1   */  be_nested_str_literal("write"),
    /* K2   */  be_nested_str_literal("["),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_literal(","),
    /* K5   */  be_nested_str_literal("json_fdump_any"),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_literal("]"),
    }),
    (be_nested_const_str("json_fdump_list", -391087443, 15)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[25]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100301,  //  0001  GETMET	R4	R1	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x7C100400,  //  0003  CALL	R4	2
      0x58100003,  //  0004  LDCONST	R4	K3
      0x6014000C,  //  0005  GETGBL	R5	G12
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C140200,  //  0007  CALL	R5	1
      0x14140805,  //  0008  LT	R5	R4	R5
      0x7816000A,  //  0009  JMPF	R5	#0015
      0x24140903,  //  000A  GT	R5	R4	K3
      0x78160002,  //  000B  JMPF	R5	#000F
      0x8C140301,  //  000C  GETMET	R5	R1	K1
      0x581C0004,  //  000D  LDCONST	R7	K4
      0x7C140400,  //  000E  CALL	R5	2
      0x8C140105,  //  000F  GETMET	R5	R0	K5
      0x5C1C0200,  //  0010  MOVE	R7	R1
      0x94200404,  //  0011  GETIDX	R8	R2	R4
      0x7C140600,  //  0012  CALL	R5	3
      0x00100906,  //  0013  ADD	R4	R4	K6
      0x7001FFEF,  //  0014  JMP		#0005
      0x8C140301,  //  0015  GETMET	R5	R1	K1
      0x581C0007,  //  0016  LDCONST	R7	K7
      0x7C140400,  //  0017  CALL	R5	2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has
********************************************************************/
be_local_closure(Persist_has,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_literal("_p"),
    /* K1   */  be_nested_str_literal("has"),
    }),
    (be_nested_const_str("has", -306245661, 3)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Persist
********************************************************************/
be_local_class(Persist,
    3,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("has", -306245661, 3, 6), be_const_closure(Persist_has_closure) },
        { be_nested_key("setmember", 1432909441, 9, -1), be_const_closure(Persist_setmember_closure) },
        { be_nested_key("remove", -611183107, 6, -1), be_const_closure(Persist_remove_closure) },
        { be_nested_key("zero", -1955600541, 4, 0), be_const_closure(Persist_zero_closure) },
        { be_nested_key("json_fdump", 1694216580, 10, -1), be_const_closure(Persist_json_fdump_closure) },
        { be_nested_key("json_fdump_list", -391087443, 15, 2), be_const_closure(Persist_json_fdump_list_closure) },
        { be_nested_key("init", 380752755, 4, 15), be_const_closure(Persist_init_closure) },
        { be_nested_key("find", -1108310694, 4, -1), be_const_closure(Persist_find_closure) },
        { be_nested_key("save", -855671224, 4, -1), be_const_closure(Persist_save_closure) },
        { be_nested_key("json_fdump_any", -946337911, 14, 12), be_const_closure(Persist_json_fdump_any_closure) },
        { be_nested_key("_p", 1594591802, 2, 7), be_const_var(1) },
        { be_nested_key("_filename", 1430813195, 9, -1), be_const_var(0) },
        { be_nested_key("load", -435725847, 4, -1), be_const_closure(Persist_load_closure) },
        { be_nested_key("json_fdump_map", -203012643, 14, 5), be_const_closure(Persist_json_fdump_map_closure) },
        { be_nested_key("_dirty", 283846766, 6, -1), be_const_var(2) },
        { be_nested_key("member", 719708611, 6, -1), be_const_closure(Persist_member_closure) },
    })),
    be_str_literal("Persist")
);

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(persist__anonymous_,   /* name */
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
    /* K0   */  be_const_class(be_class_Persist),
    }),
    (be_nested_const_str("_anonymous_", 1957281476, 11)),
    ((bstring*) &be_const_str_input),
    ( &(const binstruction[ 5]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xB4000000,  //  0001  CLASS	K0
      0x5C080200,  //  0002  MOVE	R2	R1
      0x7C080000,  //  0003  CALL	R2	0
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: persist
********************************************************************/
be_local_module(persist,
    "persist",
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("init", 380752755, 4, -1), be_const_closure(persist__anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(persist);
/********************************************************************/
