/* Solidification of persist.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

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
    /* K0   */  be_nested_str(Persist),
    }),
    &be_const_str__anonymous_,
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


extern const bclass be_class_Persist;

/********************************************************************
** Solidified function: json_fdump_map
********************************************************************/
be_local_closure(Persist_json_fdump_map,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(json),
    /* K1   */  be_nested_str(write),
    /* K2   */  be_nested_str(_X7B),
    /* K3   */  be_nested_str(keys),
    /* K4   */  be_nested_str(dump),
    /* K5   */  be_nested_str(_X3A),
    /* K6   */  be_nested_str(json_fdump_any),
    /* K7   */  be_nested_str(_X2C),
    /* K8   */  be_nested_str(stop_iteration),
    /* K9   */  be_nested_str(_X7D),
    }),
    &be_const_str_json_fdump_map,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(_p),
    /* K1   */  be_nested_str(_dirty),
    }),
    &be_const_str_setmember,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(_p),
    /* K1   */  be_nested_str(_dirty),
    }),
    &be_const_str_zero,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(_p),
    /* K1   */  be_nested_str(find),
    }),
    &be_const_str_member,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(json),
    /* K1   */  be_nested_str(_p),
    /* K2   */  be_nested_str(json_fdump_map),
    /* K3   */  be_nested_str(internal_error),
    /* K4   */  be_nested_str(persist_X2E_p_X20is_X20not_X20a_X20map),
    }),
    &be_const_str_json_fdump,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_p),
    /* K1   */  be_nested_str(remove),
    /* K2   */  be_nested_str(_dirty),
    }),
    &be_const_str_remove,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(json),
    /* K1   */  be_nested_str(json_fdump_map),
    /* K2   */  be_nested_str(json_fdump_list),
    /* K3   */  be_nested_str(write),
    /* K4   */  be_nested_str(dump),
    }),
    &be_const_str_json_fdump_any,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(_filename),
    /* K1   */  be_nested_str(w),
    /* K2   */  be_nested_str(json_fdump),
    /* K3   */  be_nested_str(close),
    /* K4   */  be_nested_str(write),
    /* K5   */  be_nested_str(_X7B_X7D),
    /* K6   */  be_nested_str(_dirty),
    }),
    &be_const_str_save,
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
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
      0x7002001E,  //  000D  JMP		#002D
      0xAC080002,  //  000E  CATCH	R2	0	2
      0x7002001B,  //  000F  JMP		#002C
      0x4C100000,  //  0010  LDNIL	R4
      0x20100204,  //  0011  NE	R4	R1	R4
      0x78120001,  //  0012  JMPF	R4	#0015
      0x8C100303,  //  0013  GETMET	R4	R1	K3
      0x7C100200,  //  0014  CALL	R4	1
      0x4C040000,  //  0015  LDNIL	R1
      0xA8020009,  //  0016  EXBLK	0	#0021
      0x60100011,  //  0017  GETGBL	R4	G17
      0x88140100,  //  0018  GETMBR	R5	R0	K0
      0x58180001,  //  0019  LDCONST	R6	K1
      0x7C100400,  //  001A  CALL	R4	2
      0x5C040800,  //  001B  MOVE	R1	R4
      0x8C100304,  //  001C  GETMET	R4	R1	K4
      0x58180005,  //  001D  LDCONST	R6	K5
      0x7C100400,  //  001E  CALL	R4	2
      0xA8040001,  //  001F  EXBLK	1	1
      0x70020003,  //  0020  JMP		#0025
      0xAC100000,  //  0021  CATCH	R4	0	0
      0x70020000,  //  0022  JMP		#0024
      0x70020000,  //  0023  JMP		#0025
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x4C100000,  //  0025  LDNIL	R4
      0x20100204,  //  0026  NE	R4	R1	R4
      0x78120001,  //  0027  JMPF	R4	#002A
      0x8C100303,  //  0028  GETMET	R4	R1	K3
      0x7C100200,  //  0029  CALL	R4	1
      0xB0040403,  //  002A  RAISE	1	R2	R3
      0x70020000,  //  002B  JMP		#002D
      0xB0080000,  //  002C  RAISE	2	R0	R0
      0x50080000,  //  002D  LDBOOL	R2	0	0
      0x90020C02,  //  002E  SETMBR	R0	K6	R2
      0x80000000,  //  002F  RET	0
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(json),
    /* K1   */  be_nested_str(path),
    /* K2   */  be_nested_str(exists),
    /* K3   */  be_nested_str(_filename),
    /* K4   */  be_nested_str(r),
    /* K5   */  be_nested_str(load),
    /* K6   */  be_nested_str(read),
    /* K7   */  be_nested_str(close),
    /* K8   */  be_nested_str(_p),
    /* K9   */  be_nested_str(BRY_X3A_X20failed_X20to_X20load_X20_persist_X2Ejson),
    /* K10  */  be_nested_str(_dirty),
    /* K11  */  be_nested_str(save),
    }),
    &be_const_str_load,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(_p),
    /* K1   */  be_nested_str(find),
    }),
    &be_const_str_find,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(_p),
    /* K1   */  be_nested_str(copy),
    /* K2   */  be_nested_str(load),
    /* K3   */  be_nested_str(_filename),
    /* K4   */  be_nested_str(_dirty),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x6008000F,  //  0000  GETGBL	R2	G15
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x7C080200,  //  0006  CALL	R2	1
      0x90020002,  //  0007  SETMBR	R0	K0	R2
      0x70020002,  //  0008  JMP		#000C
      0x60080013,  //  0009  GETGBL	R2	G19
      0x7C080000,  //  000A  CALL	R2	0
      0x90020002,  //  000B  SETMBR	R0	K0	R2
      0x8C080102,  //  000C  GETMET	R2	R0	K2
      0x88100100,  //  000D  GETMBR	R4	R0	K0
      0x88140103,  //  000E  GETMBR	R5	R0	K3
      0x7C080600,  //  000F  CALL	R2	3
      0x50080000,  //  0010  LDBOOL	R2	0	0
      0x90020802,  //  0011  SETMBR	R0	K4	R2
      0x80000000,  //  0012  RET	0
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(json),
    /* K1   */  be_nested_str(write),
    /* K2   */  be_nested_str(_X5B),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str(_X2C),
    /* K5   */  be_nested_str(json_fdump_any),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str(_X5D),
    }),
    &be_const_str_json_fdump_list,
    &be_const_str_solidified,
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
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(_p),
    /* K1   */  be_nested_str(has),
    }),
    &be_const_str_has,
    &be_const_str_solidified,
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
    2,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(has, 6), be_const_closure(Persist_has_closure) },
        { be_const_key(setmember, -1), be_const_closure(Persist_setmember_closure) },
        { be_const_key(remove, -1), be_const_closure(Persist_remove_closure) },
        { be_const_key(zero, 0), be_const_closure(Persist_zero_closure) },
        { be_const_key(json_fdump, -1), be_const_closure(Persist_json_fdump_closure) },
        { be_const_key(json_fdump_list, 2), be_const_closure(Persist_json_fdump_list_closure) },
        { be_const_key(init, 15), be_const_closure(Persist_init_closure) },
        { be_const_key(find, -1), be_const_closure(Persist_find_closure) },
        { be_const_key(save, -1), be_const_closure(Persist_save_closure) },
        { be_const_key(json_fdump_any, 12), be_const_closure(Persist_json_fdump_any_closure) },
        { be_const_key(_p, 7), be_const_var(0) },
        { be_const_key(_filename, -1), be_nested_str(_persist_X2Ejson) },
        { be_const_key(load, -1), be_const_closure(Persist_load_closure) },
        { be_const_key(json_fdump_map, 5), be_const_closure(Persist_json_fdump_map_closure) },
        { be_const_key(_dirty, -1), be_const_var(1) },
        { be_const_key(member, -1), be_const_closure(Persist_member_closure) },
    })),
    (bstring*) &be_const_str_Persist
);

/********************************************************************
** Solidified module: persist
********************************************************************/
be_local_module(persist,
    "persist",
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(Persist, -1), be_const_class(be_class_Persist) },
        { be_const_key(init, 0), be_const_closure(persist__anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(persist);
/********************************************************************/
/********************************************************************/
/* End of solidification */
