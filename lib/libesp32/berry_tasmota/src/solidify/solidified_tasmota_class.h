/* Solidification of tasmota_class.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

/********************************************************************
** Solidified function: time_str
********************************************************************/
be_local_closure(Tasmota_time_str,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(time_dump),
    /* K2   */  be_nested_str(format),
    /* K3   */  be_nested_str(_X2504d_X2D_X2502d_X2D_X2502dT_X2502d_X3A_X2502d_X3A_X2502d),
    /* K4   */  be_nested_str(year),
    /* K5   */  be_nested_str(month),
    /* K6   */  be_nested_str(day),
    /* K7   */  be_nested_str(hour),
    /* K8   */  be_nested_str(min),
    /* K9   */  be_nested_str(sec),
    }),
    &be_const_str_time_str,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0101,  //  0001  GETMET	R3	R0	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C100502,  //  0004  GETMET	R4	R2	K2
      0x58180003,  //  0005  LDCONST	R6	K3
      0x941C0704,  //  0006  GETIDX	R7	R3	K4
      0x94200705,  //  0007  GETIDX	R8	R3	K5
      0x94240706,  //  0008  GETIDX	R9	R3	K6
      0x94280707,  //  0009  GETIDX	R10	R3	K7
      0x942C0708,  //  000A  GETIDX	R11	R3	K8
      0x94300709,  //  000B  GETIDX	R12	R3	K9
      0x7C101000,  //  000C  CALL	R4	8
      0x80040800,  //  000D  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_cmd
********************************************************************/
be_local_closure(Tasmota_exec_cmd,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(_ccmd),
    /* K1   */  be_nested_str(json),
    /* K2   */  be_nested_str(load),
    /* K3   */  be_nested_str(find_key_i),
    /* K4   */  be_nested_str(resolvecmnd),
    }),
    &be_const_str_exec_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x78120016,  //  0001  JMPF	R4	#0019
      0xA4120200,  //  0002  IMPORT	R4	K1
      0x8C140902,  //  0003  GETMET	R5	R4	K2
      0x5C1C0600,  //  0004  MOVE	R7	R3
      0x7C140400,  //  0005  CALL	R5	2
      0x8C180103,  //  0006  GETMET	R6	R0	K3
      0x88200100,  //  0007  GETMBR	R8	R0	K0
      0x5C240200,  //  0008  MOVE	R9	R1
      0x7C180600,  //  0009  CALL	R6	3
      0x4C1C0000,  //  000A  LDNIL	R7
      0x201C0C07,  //  000B  NE	R7	R6	R7
      0x781E000B,  //  000C  JMPF	R7	#0019
      0x8C1C0104,  //  000D  GETMET	R7	R0	K4
      0x5C240C00,  //  000E  MOVE	R9	R6
      0x7C1C0400,  //  000F  CALL	R7	2
      0x881C0100,  //  0010  GETMBR	R7	R0	K0
      0x941C0E06,  //  0011  GETIDX	R7	R7	R6
      0x5C200C00,  //  0012  MOVE	R8	R6
      0x5C240400,  //  0013  MOVE	R9	R2
      0x5C280600,  //  0014  MOVE	R10	R3
      0x5C2C0A00,  //  0015  MOVE	R11	R5
      0x7C1C0800,  //  0016  CALL	R7	4
      0x501C0200,  //  0017  LDBOOL	R7	1	0
      0x80040E00,  //  0018  RET	1	R7
      0x50100000,  //  0019  LDBOOL	R4	0	0
      0x80040800,  //  001A  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_key_i
********************************************************************/
be_local_closure(Tasmota_find_key_i,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(toupper),
    /* K2   */  be_nested_str(keys),
    /* K3   */  be_nested_str(_X3F),
    /* K4   */  be_nested_str(stop_iteration),
    }),
    &be_const_str_find_key_i,
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100701,  //  0001  GETMET	R4	R3	K1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x7C100400,  //  0003  CALL	R4	2
      0x6014000F,  //  0004  GETGBL	R5	G15
      0x5C180200,  //  0005  MOVE	R6	R1
      0x601C0013,  //  0006  GETGBL	R7	G19
      0x7C140400,  //  0007  CALL	R5	2
      0x78160013,  //  0008  JMPF	R5	#001D
      0x60140010,  //  0009  GETGBL	R5	G16
      0x8C180302,  //  000A  GETMET	R6	R1	K2
      0x7C180200,  //  000B  CALL	R6	1
      0x7C140200,  //  000C  CALL	R5	1
      0xA802000B,  //  000D  EXBLK	0	#001A
      0x5C180A00,  //  000E  MOVE	R6	R5
      0x7C180000,  //  000F  CALL	R6	0
      0x8C1C0701,  //  0010  GETMET	R7	R3	K1
      0x5C240C00,  //  0011  MOVE	R9	R6
      0x7C1C0400,  //  0012  CALL	R7	2
      0x1C1C0E04,  //  0013  EQ	R7	R7	R4
      0x741E0001,  //  0014  JMPT	R7	#0017
      0x1C1C0503,  //  0015  EQ	R7	R2	K3
      0x781E0001,  //  0016  JMPF	R7	#0019
      0xA8040001,  //  0017  EXBLK	1	1
      0x80040C00,  //  0018  RET	1	R6
      0x7001FFF3,  //  0019  JMP		#000E
      0x58140004,  //  001A  LDCONST	R5	K4
      0xAC140200,  //  001B  CATCH	R5	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_cron
********************************************************************/
be_local_closure(Tasmota_add_cron,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_crons),
    /* K2   */  be_nested_str(ccronexpr),
    /* K3   */  be_nested_str(next),
    /* K4   */  be_nested_str(push),
    /* K5   */  be_nested_str(Trigger),
    }),
    &be_const_str_add_cron,
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x74120002,  //  0004  JMPT	R4	#0008
      0x60100012,  //  0005  GETGBL	R4	G18
      0x7C100000,  //  0006  CALL	R4	0
      0x90020204,  //  0007  SETMBR	R0	K1	R4
      0xB8120400,  //  0008  GETNGBL	R4	K2
      0x60140008,  //  0009  GETGBL	R5	G8
      0x5C180200,  //  000A  MOVE	R6	R1
      0x7C140200,  //  000B  CALL	R5	1
      0x7C100200,  //  000C  CALL	R4	1
      0x8C140903,  //  000D  GETMET	R5	R4	K3
      0x7C140200,  //  000E  CALL	R5	1
      0x88180101,  //  000F  GETMBR	R6	R0	K1
      0x8C180D04,  //  0010  GETMET	R6	R6	K4
      0xB8220A00,  //  0011  GETNGBL	R8	K5
      0x5C240A00,  //  0012  MOVE	R9	R5
      0x5C280400,  //  0013  MOVE	R10	R2
      0x5C2C0600,  //  0014  MOVE	R11	R3
      0x5C300800,  //  0015  MOVE	R12	R4
      0x7C200800,  //  0016  CALL	R8	4
      0x7C180400,  //  0017  CALL	R6	2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_timer
********************************************************************/
be_local_closure(Tasmota_remove_timer,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(_timers),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(size),
    /* K3   */  be_nested_str(id),
    /* K4   */  be_nested_str(remove),
    /* K5   */  be_const_int(1),
    }),
    &be_const_str_remove_timer,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A000E,  //  0001  JMPF	R2	#0011
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x8C100502,  //  0003  GETMET	R4	R2	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120009,  //  0006  JMPF	R4	#0011
      0x94100403,  //  0007  GETIDX	R4	R2	R3
      0x88100903,  //  0008  GETMBR	R4	R4	K3
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120003,  //  000A  JMPF	R4	#000F
      0x8C100504,  //  000B  GETMET	R4	R2	K4
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0x70020000,  //  000E  JMP		#0010
      0x000C0705,  //  000F  ADD	R3	R3	K5
      0x7001FFF1,  //  0010  JMP		#0003
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_deferred
********************************************************************/
be_local_closure(Tasmota_run_deferred,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(_timers),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(size),
    /* K3   */  be_nested_str(time_reached),
    /* K4   */  be_nested_str(trig),
    /* K5   */  be_nested_str(f),
    /* K6   */  be_nested_str(remove),
    /* K7   */  be_const_int(1),
    }),
    &be_const_str_run_deferred,
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060015,  //  0001  JMPF	R1	#0018
      0x58040001,  //  0002  LDCONST	R1	K1
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x8C080502,  //  0004  GETMET	R2	R2	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x14080202,  //  0006  LT	R2	R1	R2
      0x780A000F,  //  0007  JMPF	R2	#0018
      0x88080100,  //  0008  GETMBR	R2	R0	K0
      0x94080401,  //  0009  GETIDX	R2	R2	R1
      0x8C0C0103,  //  000A  GETMET	R3	R0	K3
      0x88140504,  //  000B  GETMBR	R5	R2	K4
      0x7C0C0400,  //  000C  CALL	R3	2
      0x780E0007,  //  000D  JMPF	R3	#0016
      0x880C0505,  //  000E  GETMBR	R3	R2	K5
      0x88100100,  //  000F  GETMBR	R4	R0	K0
      0x8C100906,  //  0010  GETMET	R4	R4	K6
      0x5C180200,  //  0011  MOVE	R6	R1
      0x7C100400,  //  0012  CALL	R4	2
      0x5C100600,  //  0013  MOVE	R4	R3
      0x7C100000,  //  0014  CALL	R4	0
      0x70020000,  //  0015  JMP		#0017
      0x00040307,  //  0016  ADD	R1	R1	K7
      0x7001FFEA,  //  0017  JMP		#0003
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_fast_loop
********************************************************************/
be_local_closure(Tasmota_add_fast_loop,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_fl),
    /* K2   */  be_nested_str(function),
    /* K3   */  be_nested_str(value_error),
    /* K4   */  be_nested_str(argument_X20must_X20be_X20a_X20function),
    /* K5   */  be_nested_str(global),
    /* K6   */  be_nested_str(fast_loop_enabled),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str(push),
    }),
    &be_const_str_add_fast_loop,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x740A0002,  //  0004  JMPT	R2	#0008
      0x60080012,  //  0005  GETGBL	R2	G18
      0x7C080000,  //  0006  CALL	R2	0
      0x90020202,  //  0007  SETMBR	R0	K1	R2
      0x60080004,  //  0008  GETGBL	R2	G4
      0x5C0C0200,  //  0009  MOVE	R3	R1
      0x7C080200,  //  000A  CALL	R2	1
      0x20080502,  //  000B  NE	R2	R2	K2
      0x780A0000,  //  000C  JMPF	R2	#000E
      0xB0060704,  //  000D  RAISE	1	K3	K4
      0x88080105,  //  000E  GETMBR	R2	R0	K5
      0x900A0D07,  //  000F  SETMBR	R2	K6	K7
      0x88080101,  //  0010  GETMBR	R2	R0	K1
      0x8C080508,  //  0011  GETMET	R2	R2	K8
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C080400,  //  0013  CALL	R2	2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_tele
********************************************************************/
be_local_closure(Tasmota_exec_tele,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(_rules),
    /* K1   */  be_nested_str(json),
    /* K2   */  be_nested_str(load),
    /* K3   */  be_nested_str(log),
    /* K4   */  be_nested_str(BRY_X3A_X20ERROR_X2C_X20bad_X20json_X3A_X20),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str(Tele),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str(try_rule),
    /* K9   */  be_nested_str(trig),
    /* K10  */  be_nested_str(f),
    /* K11  */  be_const_int(1),
    }),
    &be_const_str_exec_tele,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0024,  //  0001  JMPF	R2	#0027
      0xA40A0200,  //  0002  IMPORT	R2	K1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x4C140000,  //  0007  LDNIL	R5
      0x1C140605,  //  0008  EQ	R5	R3	R5
      0x78160004,  //  0009  JMPF	R5	#000F
      0x8C140103,  //  000A  GETMET	R5	R0	K3
      0x001E0801,  //  000B  ADD	R7	K4	R1
      0x58200005,  //  000C  LDCONST	R8	K5
      0x7C140600,  //  000D  CALL	R5	3
      0x5C0C0200,  //  000E  MOVE	R3	R1
      0x60140013,  //  000F  GETGBL	R5	G19
      0x7C140000,  //  0010  CALL	R5	0
      0x98160C03,  //  0011  SETIDX	R5	K6	R3
      0x5C0C0A00,  //  0012  MOVE	R3	R5
      0x58140007,  //  0013  LDCONST	R5	K7
      0x6018000C,  //  0014  GETGBL	R6	G12
      0x881C0100,  //  0015  GETMBR	R7	R0	K0
      0x7C180200,  //  0016  CALL	R6	1
      0x14180A06,  //  0017  LT	R6	R5	R6
      0x781A000C,  //  0018  JMPF	R6	#0026
      0x88180100,  //  0019  GETMBR	R6	R0	K0
      0x94180C05,  //  001A  GETIDX	R6	R6	R5
      0x8C1C0108,  //  001B  GETMET	R7	R0	K8
      0x5C240600,  //  001C  MOVE	R9	R3
      0x88280D09,  //  001D  GETMBR	R10	R6	K9
      0x882C0D0A,  //  001E  GETMBR	R11	R6	K10
      0x7C1C0800,  //  001F  CALL	R7	4
      0x741E0001,  //  0020  JMPT	R7	#0023
      0x74120000,  //  0021  JMPT	R4	#0023
      0x50100001,  //  0022  LDBOOL	R4	0	1
      0x50100200,  //  0023  LDBOOL	R4	1	0
      0x00140B0B,  //  0024  ADD	R5	R5	K11
      0x7001FFED,  //  0025  JMP		#0014
      0x80040800,  //  0026  RET	1	R4
      0x50080000,  //  0027  LDBOOL	R2	0	0
      0x80040400,  //  0028  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(Tasmota_load,   /* name */
  be_nested_proto(
    27,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 6]) {
      be_nested_proto(
        6,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str(sys),
        /* K1   */  be_nested_str(path),
        /* K2   */  be_nested_str(find),
        /* K3   */  be_nested_str(push),
        }),
        &be_const_str_push_path,
        &be_const_str_solidified,
        ( &(const binstruction[13]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0x8C080301,  //  0001  GETMET	R2	R1	K1
          0x7C080200,  //  0002  CALL	R2	1
          0x8C0C0502,  //  0003  GETMET	R3	R2	K2
          0x5C140000,  //  0004  MOVE	R5	R0
          0x7C0C0400,  //  0005  CALL	R3	2
          0x4C100000,  //  0006  LDNIL	R4
          0x1C0C0604,  //  0007  EQ	R3	R3	R4
          0x780E0002,  //  0008  JMPF	R3	#000C
          0x8C0C0503,  //  0009  GETMET	R3	R2	K3
          0x5C140000,  //  000A  MOVE	R5	R0
          0x7C0C0400,  //  000B  CALL	R3	2
          0x80000000,  //  000C  RET	0
        })
      ),
      be_nested_proto(
        7,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str(sys),
        /* K1   */  be_nested_str(path),
        /* K2   */  be_nested_str(find),
        /* K3   */  be_nested_str(remove),
        }),
        &be_const_str_pop_path,
        &be_const_str_solidified,
        ( &(const binstruction[13]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0x8C080301,  //  0001  GETMET	R2	R1	K1
          0x7C080200,  //  0002  CALL	R2	1
          0x8C0C0502,  //  0003  GETMET	R3	R2	K2
          0x5C140000,  //  0004  MOVE	R5	R0
          0x7C0C0400,  //  0005  CALL	R3	2
          0x4C100000,  //  0006  LDNIL	R4
          0x20100604,  //  0007  NE	R4	R3	R4
          0x78120002,  //  0008  JMPF	R4	#000C
          0x8C100503,  //  0009  GETMET	R4	R2	K3
          0x5C180600,  //  000A  MOVE	R6	R3
          0x7C100400,  //  000B  CALL	R4	2
          0x80000000,  //  000C  RET	0
        })
      ),
      be_nested_proto(
        10,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[10]) {     /* constants */
        /* K0   */  be_nested_str(r),
        /* K1   */  be_nested_str(readbytes),
        /* K2   */  be_const_int(3),
        /* K3   */  be_const_int(1),
        /* K4   */  be_nested_str(BECDFE),
        /* K5   */  be_const_int(0),
        /* K6   */  be_nested_str(close),
        /* K7   */  be_nested_str(string),
        /* K8   */  be_nested_str(format),
        /* K9   */  be_nested_str(BRY_X3A_X20failed_X20to_X20load_X20compiled_X20_X27_X25s_X27_X20_X28_X25s_X29),
        }),
        &be_const_str_try_get_bec_version,
        &be_const_str_solidified,
        ( &(const binstruction[44]) {  /* code */
          0x4C040000,  //  0000  LDNIL	R1
          0xA8020016,  //  0001  EXBLK	0	#0019
          0x60080011,  //  0002  GETGBL	R2	G17
          0x5C0C0000,  //  0003  MOVE	R3	R0
          0x58100000,  //  0004  LDCONST	R4	K0
          0x7C080400,  //  0005  CALL	R2	2
          0x5C040400,  //  0006  MOVE	R1	R2
          0x8C080301,  //  0007  GETMET	R2	R1	K1
          0x58100002,  //  0008  LDCONST	R4	K2
          0x7C080400,  //  0009  CALL	R2	2
          0x8C0C0301,  //  000A  GETMET	R3	R1	K1
          0x58140003,  //  000B  LDCONST	R5	K3
          0x7C0C0400,  //  000C  CALL	R3	2
          0x60100015,  //  000D  GETGBL	R4	G21
          0x58140004,  //  000E  LDCONST	R5	K4
          0x7C100200,  //  000F  CALL	R4	1
          0x1C100404,  //  0010  EQ	R4	R2	R4
          0x78120002,  //  0011  JMPF	R4	#0015
          0x94100705,  //  0012  GETIDX	R4	R3	K5
          0xA8040001,  //  0013  EXBLK	1	1
          0x80040800,  //  0014  RET	1	R4
          0x8C100306,  //  0015  GETMET	R4	R1	K6
          0x7C100200,  //  0016  CALL	R4	1
          0xA8040001,  //  0017  EXBLK	1	1
          0x70020010,  //  0018  JMP		#002A
          0xAC080001,  //  0019  CATCH	R2	0	1
          0x7002000D,  //  001A  JMP		#0029
          0x4C0C0000,  //  001B  LDNIL	R3
          0x200C0203,  //  001C  NE	R3	R1	R3
          0x780E0001,  //  001D  JMPF	R3	#0020
          0x8C0C0306,  //  001E  GETMET	R3	R1	K6
          0x7C0C0200,  //  001F  CALL	R3	1
          0xA40E0E00,  //  0020  IMPORT	R3	K7
          0x60100001,  //  0021  GETGBL	R4	G1
          0x8C140708,  //  0022  GETMET	R5	R3	K8
          0x581C0009,  //  0023  LDCONST	R7	K9
          0x5C200000,  //  0024  MOVE	R8	R0
          0x5C240400,  //  0025  MOVE	R9	R2
          0x7C140800,  //  0026  CALL	R5	4
          0x7C100200,  //  0027  CALL	R4	1
          0x70020000,  //  0028  JMP		#002A
          0xB0080000,  //  0029  RAISE	2	R0	R0
          0x4C080000,  //  002A  LDNIL	R2
          0x80040400,  //  002B  RET	1	R2
        })
      ),
      be_nested_proto(
        5,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str(path),
        /* K1   */  be_nested_str(remove),
        }),
        &be_const_str_try_remove_file,
        &be_const_str_solidified,
        ( &(const binstruction[15]) {  /* code */
          0xA4060000,  //  0000  IMPORT	R1	K0
          0xA8020006,  //  0001  EXBLK	0	#0009
          0x8C080301,  //  0002  GETMET	R2	R1	K1
          0x5C100000,  //  0003  MOVE	R4	R0
          0x7C080400,  //  0004  CALL	R2	2
          0xA8040001,  //  0005  EXBLK	1	1
          0x80040400,  //  0006  RET	1	R2
          0xA8040001,  //  0007  EXBLK	1	1
          0x70020003,  //  0008  JMP		#000D
          0xAC080000,  //  0009  CATCH	R2	0	0
          0x70020000,  //  000A  JMP		#000C
          0x70020000,  //  000B  JMP		#000D
          0xB0080000,  //  000C  RAISE	2	R0	R0
          0x50080000,  //  000D  LDBOOL	R2	0	0
          0x80040400,  //  000E  RET	1	R2
        })
      ),
      be_nested_proto(
        11,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str(file),
        /* K1   */  be_nested_str(string),
        /* K2   */  be_nested_str(format),
        /* K3   */  be_nested_str(BRY_X3A_X20failed_X20to_X20load_X20_X27_X25s_X27_X20_X28_X25s_X20_X2D_X20_X25s_X29),
        }),
        &be_const_str_try_compile,
        &be_const_str_solidified,
        ( &(const binstruction[24]) {  /* code */
          0xA8020007,  //  0000  EXBLK	0	#0009
          0x6004000D,  //  0001  GETGBL	R1	G13
          0x5C080000,  //  0002  MOVE	R2	R0
          0x580C0000,  //  0003  LDCONST	R3	K0
          0x7C040400,  //  0004  CALL	R1	2
          0xA8040001,  //  0005  EXBLK	1	1
          0x80040200,  //  0006  RET	1	R1
          0xA8040001,  //  0007  EXBLK	1	1
          0x7002000C,  //  0008  JMP		#0016
          0xAC040002,  //  0009  CATCH	R1	0	2
          0x70020009,  //  000A  JMP		#0015
          0xA40E0200,  //  000B  IMPORT	R3	K1
          0x60100001,  //  000C  GETGBL	R4	G1
          0x8C140702,  //  000D  GETMET	R5	R3	K2
          0x581C0003,  //  000E  LDCONST	R7	K3
          0x5C200000,  //  000F  MOVE	R8	R0
          0x5C240200,  //  0010  MOVE	R9	R1
          0x5C280400,  //  0011  MOVE	R10	R2
          0x7C140A00,  //  0012  CALL	R5	5
          0x7C100200,  //  0013  CALL	R4	1
          0x70020000,  //  0014  JMP		#0016
          0xB0080000,  //  0015  RAISE	2	R0	R0
          0x4C040000,  //  0016  LDNIL	R1
          0x80040200,  //  0017  RET	1	R1
        })
      ),
      be_nested_proto(
        10,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_nested_str(string),
        /* K1   */  be_nested_str(format),
        /* K2   */  be_nested_str(BRY_X3A_X20failed_X20to_X20run_X20compiled_X20code_X20_X27_X25s_X27_X20_X2D_X20_X25s),
        }),
        &be_const_str_try_run_compiled,
        &be_const_str_solidified,
        ( &(const binstruction[25]) {  /* code */
          0x4C040000,  //  0000  LDNIL	R1
          0x20040001,  //  0001  NE	R1	R0	R1
          0x78060013,  //  0002  JMPF	R1	#0017
          0xA8020006,  //  0003  EXBLK	0	#000B
          0x5C040000,  //  0004  MOVE	R1	R0
          0x7C040000,  //  0005  CALL	R1	0
          0x50040200,  //  0006  LDBOOL	R1	1	0
          0xA8040001,  //  0007  EXBLK	1	1
          0x80040200,  //  0008  RET	1	R1
          0xA8040001,  //  0009  EXBLK	1	1
          0x7002000B,  //  000A  JMP		#0017
          0xAC040002,  //  000B  CATCH	R1	0	2
          0x70020008,  //  000C  JMP		#0016
          0xA40E0000,  //  000D  IMPORT	R3	K0
          0x60100001,  //  000E  GETGBL	R4	G1
          0x8C140701,  //  000F  GETMET	R5	R3	K1
          0x581C0002,  //  0010  LDCONST	R7	K2
          0x5C200200,  //  0011  MOVE	R8	R1
          0x5C240400,  //  0012  MOVE	R9	R2
          0x7C140800,  //  0013  CALL	R5	4
          0x7C100200,  //  0014  CALL	R4	1
          0x70020000,  //  0015  JMP		#0017
          0xB0080000,  //  0016  RAISE	2	R0	R0
          0x50040000,  //  0017  LDBOOL	R1	0	0
          0x80040200,  //  0018  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(path),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str(_X2F),
    /* K4   */  be_nested_str(split),
    /* K5   */  be_nested_str(_X23),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str(find),
    /* K8   */  be_nested_str(_X2E),
    /* K9   */  be_nested_str(_X2Ebe),
    /* K10  */  be_nested_str(_X2Ebec),
    /* K11  */  be_nested_str(io_error),
    /* K12  */  be_nested_str(file_X20extension_X20is_X20not_X20_X27_X2Ebe_X27_X20or_X20_X27_X2Ebec_X27),
    /* K13  */  be_nested_str(last_modified),
    /* K14  */  be_nested_str(c),
    /* K15  */  be_nested_str(wd),
    /* K16  */  be_nested_str(),
    /* K17  */  be_nested_str(format),
    /* K18  */  be_nested_str(BRY_X3A_X20corrupt_X20bytecode_X20_X27_X25s_X27),
    /* K19  */  be_nested_str(BRY_X3A_X20bytecode_X20has_X20wrong_X20version_X20_X27_X25s_X27_X20_X28_X25i_X29),
    /* K20  */  be_nested_str(save),
    /* K21  */  be_nested_str(BRY_X3A_X20could_X20not_X20save_X20compiled_X20file_X20_X25s_X20_X28_X25s_X29),
    }),
    &be_const_str_load,
    &be_const_str_solidified,
    ( &(const binstruction[176]) {  /* code */
      0x84080000,  //  0000  CLOSURE	R2	P0
      0x840C0001,  //  0001  CLOSURE	R3	P1
      0x84100002,  //  0002  CLOSURE	R4	P2
      0x84140003,  //  0003  CLOSURE	R5	P3
      0x84180004,  //  0004  CLOSURE	R6	P4
      0x841C0005,  //  0005  CLOSURE	R7	P5
      0xA4220000,  //  0006  IMPORT	R8	K0
      0xA4260200,  //  0007  IMPORT	R9	K1
      0x6028000C,  //  0008  GETGBL	R10	G12
      0x5C2C0200,  //  0009  MOVE	R11	R1
      0x7C280200,  //  000A  CALL	R10	1
      0x1C281502,  //  000B  EQ	R10	R10	K2
      0x782A0001,  //  000C  JMPF	R10	#000F
      0x50280000,  //  000D  LDBOOL	R10	0	0
      0x80041400,  //  000E  RET	1	R10
      0x94280302,  //  000F  GETIDX	R10	R1	K2
      0x20281503,  //  0010  NE	R10	R10	K3
      0x782A0000,  //  0011  JMPF	R10	#0013
      0x00060601,  //  0012  ADD	R1	K3	R1
      0x8C281104,  //  0013  GETMET	R10	R8	K4
      0x5C300200,  //  0014  MOVE	R12	R1
      0x58340005,  //  0015  LDCONST	R13	K5
      0x7C280600,  //  0016  CALL	R10	3
      0x942C1502,  //  0017  GETIDX	R11	R10	K2
      0x5431FFFE,  //  0018  LDINT	R12	-1
      0x9430140C,  //  0019  GETIDX	R12	R10	R12
      0x6034000C,  //  001A  GETGBL	R13	G12
      0x5C381400,  //  001B  MOVE	R14	R10
      0x7C340200,  //  001C  CALL	R13	1
      0x24341B06,  //  001D  GT	R13	R13	K6
      0x8C381107,  //  001E  GETMET	R14	R8	K7
      0x5C401800,  //  001F  MOVE	R16	R12
      0x58440008,  //  0020  LDCONST	R17	K8
      0x7C380600,  //  0021  CALL	R14	3
      0x14381D02,  //  0022  LT	R14	R14	K2
      0x783A0001,  //  0023  JMPF	R14	#0026
      0x00040309,  //  0024  ADD	R1	R1	K9
      0x00301909,  //  0025  ADD	R12	R12	K9
      0x5439FFFC,  //  0026  LDINT	R14	-3
      0x543DFFFE,  //  0027  LDINT	R15	-1
      0x40381C0F,  //  0028  CONNECT	R14	R14	R15
      0x9438180E,  //  0029  GETIDX	R14	R12	R14
      0x1C381D09,  //  002A  EQ	R14	R14	K9
      0x543DFFFB,  //  002B  LDINT	R15	-4
      0x5441FFFE,  //  002C  LDINT	R16	-1
      0x403C1E10,  //  002D  CONNECT	R15	R15	R16
      0x943C180F,  //  002E  GETIDX	R15	R12	R15
      0x1C3C1F0A,  //  002F  EQ	R15	R15	K10
      0x5C401C00,  //  0030  MOVE	R16	R14
      0x74420002,  //  0031  JMPT	R16	#0035
      0x5C401E00,  //  0032  MOVE	R16	R15
      0x74420000,  //  0033  JMPT	R16	#0035
      0xB006170C,  //  0034  RAISE	1	K11	K12
      0x8C40130D,  //  0035  GETMET	R16	R9	K13
      0x5C480200,  //  0036  MOVE	R18	R1
      0x7C400400,  //  0037  CALL	R16	2
      0x783E0001,  //  0038  JMPF	R15	#003B
      0x5C440200,  //  0039  MOVE	R17	R1
      0x70020000,  //  003A  JMP		#003C
      0x0044030E,  //  003B  ADD	R17	R1	K14
      0x783E0005,  //  003C  JMPF	R15	#0043
      0x4C480000,  //  003D  LDNIL	R18
      0x1C482012,  //  003E  EQ	R18	R16	R18
      0x784A0001,  //  003F  JMPF	R18	#0042
      0x50480000,  //  0040  LDBOOL	R18	0	0
      0x80042400,  //  0041  RET	1	R18
      0x70020013,  //  0042  JMP		#0057
      0x8C48130D,  //  0043  GETMET	R18	R9	K13
      0x5C502200,  //  0044  MOVE	R20	R17
      0x7C480400,  //  0045  CALL	R18	2
      0x4C4C0000,  //  0046  LDNIL	R19
      0x1C4C2013,  //  0047  EQ	R19	R16	R19
      0x784E0004,  //  0048  JMPF	R19	#004E
      0x4C4C0000,  //  0049  LDNIL	R19
      0x1C4C2413,  //  004A  EQ	R19	R18	R19
      0x784E0001,  //  004B  JMPF	R19	#004E
      0x504C0000,  //  004C  LDBOOL	R19	0	0
      0x80042600,  //  004D  RET	1	R19
      0x4C4C0000,  //  004E  LDNIL	R19
      0x204C2413,  //  004F  NE	R19	R18	R19
      0x784E0005,  //  0050  JMPF	R19	#0057
      0x4C4C0000,  //  0051  LDNIL	R19
      0x1C4C2013,  //  0052  EQ	R19	R16	R19
      0x744E0001,  //  0053  JMPT	R19	#0056
      0x284C2410,  //  0054  GE	R19	R18	R16
      0x784E0000,  //  0055  JMPF	R19	#0057
      0x503C0200,  //  0056  LDBOOL	R15	1	0
      0x78360005,  //  0057  JMPF	R13	#005E
      0x00481705,  //  0058  ADD	R18	R11	K5
      0x90021E12,  //  0059  SETMBR	R0	K15	R18
      0x5C480400,  //  005A  MOVE	R18	R2
      0x884C010F,  //  005B  GETMBR	R19	R0	K15
      0x7C480200,  //  005C  CALL	R18	1
      0x70020000,  //  005D  JMP		#005F
      0x90021F10,  //  005E  SETMBR	R0	K15	K16
      0x4C480000,  //  005F  LDNIL	R18
      0x783E0025,  //  0060  JMPF	R15	#0087
      0x5C4C0800,  //  0061  MOVE	R19	R4
      0x5C502200,  //  0062  MOVE	R20	R17
      0x7C4C0200,  //  0063  CALL	R19	1
      0x50500200,  //  0064  LDBOOL	R20	1	0
      0x4C540000,  //  0065  LDNIL	R21
      0x1C542615,  //  0066  EQ	R21	R19	R21
      0x78560007,  //  0067  JMPF	R21	#0070
      0x60540001,  //  0068  GETGBL	R21	G1
      0x8C581111,  //  0069  GETMET	R22	R8	K17
      0x58600012,  //  006A  LDCONST	R24	K18
      0x5C642200,  //  006B  MOVE	R25	R17
      0x7C580600,  //  006C  CALL	R22	3
      0x7C540200,  //  006D  CALL	R21	1
      0x50500000,  //  006E  LDBOOL	R20	0	0
      0x7002000A,  //  006F  JMP		#007B
      0x54560003,  //  0070  LDINT	R21	4
      0x20542615,  //  0071  NE	R21	R19	R21
      0x78560007,  //  0072  JMPF	R21	#007B
      0x60540001,  //  0073  GETGBL	R21	G1
      0x8C581111,  //  0074  GETMET	R22	R8	K17
      0x58600013,  //  0075  LDCONST	R24	K19
      0x5C642200,  //  0076  MOVE	R25	R17
      0x5C682600,  //  0077  MOVE	R26	R19
      0x7C580800,  //  0078  CALL	R22	4
      0x7C540200,  //  0079  CALL	R21	1
      0x50500000,  //  007A  LDBOOL	R20	0	0
      0x78520003,  //  007B  JMPF	R20	#0080
      0x5C540C00,  //  007C  MOVE	R21	R6
      0x5C582200,  //  007D  MOVE	R22	R17
      0x7C540200,  //  007E  CALL	R21	1
      0x5C482A00,  //  007F  MOVE	R18	R21
      0x4C540000,  //  0080  LDNIL	R21
      0x1C542415,  //  0081  EQ	R21	R18	R21
      0x78560003,  //  0082  JMPF	R21	#0087
      0x5C540A00,  //  0083  MOVE	R21	R5
      0x5C582200,  //  0084  MOVE	R22	R17
      0x7C540200,  //  0085  CALL	R21	1
      0x503C0000,  //  0086  LDBOOL	R15	0	0
      0x783A0006,  //  0087  JMPF	R14	#008F
      0x4C4C0000,  //  0088  LDNIL	R19
      0x1C4C2413,  //  0089  EQ	R19	R18	R19
      0x784E0003,  //  008A  JMPF	R19	#008F
      0x5C4C0C00,  //  008B  MOVE	R19	R6
      0x5C500200,  //  008C  MOVE	R20	R1
      0x7C4C0200,  //  008D  CALL	R19	1
      0x5C482600,  //  008E  MOVE	R18	R19
      0x4C4C0000,  //  008F  LDNIL	R19
      0x204C2413,  //  0090  NE	R19	R18	R19
      0x784E0015,  //  0091  JMPF	R19	#00A8
      0x5C4C1E00,  //  0092  MOVE	R19	R15
      0x744E0013,  //  0093  JMPT	R19	#00A8
      0x5C4C1A00,  //  0094  MOVE	R19	R13
      0x744E0011,  //  0095  JMPT	R19	#00A8
      0xA8020005,  //  0096  EXBLK	0	#009D
      0x8C4C0114,  //  0097  GETMET	R19	R0	K20
      0x5C542200,  //  0098  MOVE	R21	R17
      0x5C582400,  //  0099  MOVE	R22	R18
      0x7C4C0600,  //  009A  CALL	R19	3
      0xA8040001,  //  009B  EXBLK	1	1
      0x7002000A,  //  009C  JMP		#00A8
      0xAC4C0001,  //  009D  CATCH	R19	0	1
      0x70020007,  //  009E  JMP		#00A7
      0x60500001,  //  009F  GETGBL	R20	G1
      0x8C541111,  //  00A0  GETMET	R21	R8	K17
      0x585C0015,  //  00A1  LDCONST	R23	K21
      0x5C602200,  //  00A2  MOVE	R24	R17
      0x5C642600,  //  00A3  MOVE	R25	R19
      0x7C540800,  //  00A4  CALL	R21	4
      0x7C500200,  //  00A5  CALL	R20	1
      0x70020000,  //  00A6  JMP		#00A8
      0xB0080000,  //  00A7  RAISE	2	R0	R0
      0x5C4C0E00,  //  00A8  MOVE	R19	R7
      0x5C502400,  //  00A9  MOVE	R20	R18
      0x7C4C0200,  //  00AA  CALL	R19	1
      0x78360002,  //  00AB  JMPF	R13	#00AF
      0x5C500600,  //  00AC  MOVE	R20	R3
      0x00541705,  //  00AD  ADD	R21	R11	K5
      0x7C500200,  //  00AE  CALL	R20	1
      0x80042600,  //  00AF  RET	1	R19
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: try_rule
********************************************************************/
be_local_closure(Tasmota_try_rule,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(find_op),
    /* K2   */  be_nested_str(split),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str(_X23),
    /* K5   */  be_nested_str(find_key_i),
    /* K6   */  be_const_int(1),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str(_X3D_X3D),
    /* K9   */  be_nested_str(_X21_X3D_X3D),
    /* K10  */  be_nested_str(_X3D),
    /* K11  */  be_nested_str(_X21_X3D),
    /* K12  */  be_nested_str(_X3E),
    /* K13  */  be_nested_str(_X3E_X3D),
    /* K14  */  be_nested_str(_X3C),
    /* K15  */  be_nested_str(_X3C_X3D),
    }),
    &be_const_str_try_rule,
    &be_const_str_solidified,
    ( &(const binstruction[141]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x8C140101,  //  0001  GETMET	R5	R0	K1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x7C140400,  //  0003  CALL	R5	2
      0x5C180200,  //  0004  MOVE	R6	R1
      0x8C1C0902,  //  0005  GETMET	R7	R4	K2
      0x94240B03,  //  0006  GETIDX	R9	R5	K3
      0x58280004,  //  0007  LDCONST	R10	K4
      0x7C1C0600,  //  0008  CALL	R7	3
      0x58200003,  //  0009  LDCONST	R8	K3
      0x6024000C,  //  000A  GETGBL	R9	G12
      0x5C280E00,  //  000B  MOVE	R10	R7
      0x7C240200,  //  000C  CALL	R9	1
      0x14241009,  //  000D  LT	R9	R8	R9
      0x7826000C,  //  000E  JMPF	R9	#001C
      0x94240E08,  //  000F  GETIDX	R9	R7	R8
      0x8C280105,  //  0010  GETMET	R10	R0	K5
      0x5C300C00,  //  0011  MOVE	R12	R6
      0x5C341200,  //  0012  MOVE	R13	R9
      0x7C280600,  //  0013  CALL	R10	3
      0x4C2C0000,  //  0014  LDNIL	R11
      0x1C2C140B,  //  0015  EQ	R11	R10	R11
      0x782E0001,  //  0016  JMPF	R11	#0019
      0x502C0000,  //  0017  LDBOOL	R11	0	0
      0x80041600,  //  0018  RET	1	R11
      0x94180C0A,  //  0019  GETIDX	R6	R6	R10
      0x00201106,  //  001A  ADD	R8	R8	K6
      0x7001FFED,  //  001B  JMP		#000A
      0x94240B06,  //  001C  GETIDX	R9	R5	K6
      0x94280B07,  //  001D  GETIDX	R10	R5	K7
      0x78260066,  //  001E  JMPF	R9	#0086
      0x1C2C1308,  //  001F  EQ	R11	R9	K8
      0x782E000A,  //  0020  JMPF	R11	#002C
      0x602C0008,  //  0021  GETGBL	R11	G8
      0x5C300C00,  //  0022  MOVE	R12	R6
      0x7C2C0200,  //  0023  CALL	R11	1
      0x60300008,  //  0024  GETGBL	R12	G8
      0x5C341400,  //  0025  MOVE	R13	R10
      0x7C300200,  //  0026  CALL	R12	1
      0x202C160C,  //  0027  NE	R11	R11	R12
      0x782E0001,  //  0028  JMPF	R11	#002B
      0x502C0000,  //  0029  LDBOOL	R11	0	0
      0x80041600,  //  002A  RET	1	R11
      0x70020059,  //  002B  JMP		#0086
      0x1C2C1309,  //  002C  EQ	R11	R9	K9
      0x782E000A,  //  002D  JMPF	R11	#0039
      0x602C0008,  //  002E  GETGBL	R11	G8
      0x5C300C00,  //  002F  MOVE	R12	R6
      0x7C2C0200,  //  0030  CALL	R11	1
      0x60300008,  //  0031  GETGBL	R12	G8
      0x5C341400,  //  0032  MOVE	R13	R10
      0x7C300200,  //  0033  CALL	R12	1
      0x1C2C160C,  //  0034  EQ	R11	R11	R12
      0x782E0001,  //  0035  JMPF	R11	#0038
      0x502C0000,  //  0036  LDBOOL	R11	0	0
      0x80041600,  //  0037  RET	1	R11
      0x7002004C,  //  0038  JMP		#0086
      0x1C2C130A,  //  0039  EQ	R11	R9	K10
      0x782E000A,  //  003A  JMPF	R11	#0046
      0x602C000A,  //  003B  GETGBL	R11	G10
      0x5C300C00,  //  003C  MOVE	R12	R6
      0x7C2C0200,  //  003D  CALL	R11	1
      0x6030000A,  //  003E  GETGBL	R12	G10
      0x5C341400,  //  003F  MOVE	R13	R10
      0x7C300200,  //  0040  CALL	R12	1
      0x202C160C,  //  0041  NE	R11	R11	R12
      0x782E0001,  //  0042  JMPF	R11	#0045
      0x502C0000,  //  0043  LDBOOL	R11	0	0
      0x80041600,  //  0044  RET	1	R11
      0x7002003F,  //  0045  JMP		#0086
      0x1C2C130B,  //  0046  EQ	R11	R9	K11
      0x782E000A,  //  0047  JMPF	R11	#0053
      0x602C000A,  //  0048  GETGBL	R11	G10
      0x5C300C00,  //  0049  MOVE	R12	R6
      0x7C2C0200,  //  004A  CALL	R11	1
      0x6030000A,  //  004B  GETGBL	R12	G10
      0x5C341400,  //  004C  MOVE	R13	R10
      0x7C300200,  //  004D  CALL	R12	1
      0x1C2C160C,  //  004E  EQ	R11	R11	R12
      0x782E0001,  //  004F  JMPF	R11	#0052
      0x502C0000,  //  0050  LDBOOL	R11	0	0
      0x80041600,  //  0051  RET	1	R11
      0x70020032,  //  0052  JMP		#0086
      0x1C2C130C,  //  0053  EQ	R11	R9	K12
      0x782E000A,  //  0054  JMPF	R11	#0060
      0x602C000A,  //  0055  GETGBL	R11	G10
      0x5C300C00,  //  0056  MOVE	R12	R6
      0x7C2C0200,  //  0057  CALL	R11	1
      0x6030000A,  //  0058  GETGBL	R12	G10
      0x5C341400,  //  0059  MOVE	R13	R10
      0x7C300200,  //  005A  CALL	R12	1
      0x182C160C,  //  005B  LE	R11	R11	R12
      0x782E0001,  //  005C  JMPF	R11	#005F
      0x502C0000,  //  005D  LDBOOL	R11	0	0
      0x80041600,  //  005E  RET	1	R11
      0x70020025,  //  005F  JMP		#0086
      0x1C2C130D,  //  0060  EQ	R11	R9	K13
      0x782E000A,  //  0061  JMPF	R11	#006D
      0x602C000A,  //  0062  GETGBL	R11	G10
      0x5C300C00,  //  0063  MOVE	R12	R6
      0x7C2C0200,  //  0064  CALL	R11	1
      0x6030000A,  //  0065  GETGBL	R12	G10
      0x5C341400,  //  0066  MOVE	R13	R10
      0x7C300200,  //  0067  CALL	R12	1
      0x142C160C,  //  0068  LT	R11	R11	R12
      0x782E0001,  //  0069  JMPF	R11	#006C
      0x502C0000,  //  006A  LDBOOL	R11	0	0
      0x80041600,  //  006B  RET	1	R11
      0x70020018,  //  006C  JMP		#0086
      0x1C2C130E,  //  006D  EQ	R11	R9	K14
      0x782E000A,  //  006E  JMPF	R11	#007A
      0x602C000A,  //  006F  GETGBL	R11	G10
      0x5C300C00,  //  0070  MOVE	R12	R6
      0x7C2C0200,  //  0071  CALL	R11	1
      0x6030000A,  //  0072  GETGBL	R12	G10
      0x5C341400,  //  0073  MOVE	R13	R10
      0x7C300200,  //  0074  CALL	R12	1
      0x282C160C,  //  0075  GE	R11	R11	R12
      0x782E0001,  //  0076  JMPF	R11	#0079
      0x502C0000,  //  0077  LDBOOL	R11	0	0
      0x80041600,  //  0078  RET	1	R11
      0x7002000B,  //  0079  JMP		#0086
      0x1C2C130F,  //  007A  EQ	R11	R9	K15
      0x782E0009,  //  007B  JMPF	R11	#0086
      0x602C000A,  //  007C  GETGBL	R11	G10
      0x5C300C00,  //  007D  MOVE	R12	R6
      0x7C2C0200,  //  007E  CALL	R11	1
      0x6030000A,  //  007F  GETGBL	R12	G10
      0x5C341400,  //  0080  MOVE	R13	R10
      0x7C300200,  //  0081  CALL	R12	1
      0x242C160C,  //  0082  GT	R11	R11	R12
      0x782E0001,  //  0083  JMPF	R11	#0086
      0x502C0000,  //  0084  LDBOOL	R11	0	0
      0x80041600,  //  0085  RET	1	R11
      0x5C2C0600,  //  0086  MOVE	R11	R3
      0x5C300C00,  //  0087  MOVE	R12	R6
      0x94340B03,  //  0088  GETIDX	R13	R5	K3
      0x5C380200,  //  0089  MOVE	R14	R1
      0x7C2C0600,  //  008A  CALL	R11	3
      0x502C0200,  //  008B  LDBOOL	R11	1	0
      0x80041600,  //  008C  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_rule
********************************************************************/
be_local_closure(Tasmota_remove_rule,   /* name */
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
    /* K0   */  be_nested_str(_rules),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(trig),
    /* K3   */  be_nested_str(id),
    /* K4   */  be_nested_str(remove),
    /* K5   */  be_const_int(1),
    }),
    &be_const_str_remove_rule,
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x780E0016,  //  0001  JMPF	R3	#0019
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140100,  //  0004  GETMBR	R5	R0	K0
      0x7C100200,  //  0005  CALL	R4	1
      0x14100604,  //  0006  LT	R4	R3	R4
      0x78120010,  //  0007  JMPF	R4	#0019
      0x88100100,  //  0008  GETMBR	R4	R0	K0
      0x94100803,  //  0009  GETIDX	R4	R4	R3
      0x88100902,  //  000A  GETMBR	R4	R4	K2
      0x1C100801,  //  000B  EQ	R4	R4	R1
      0x78120009,  //  000C  JMPF	R4	#0017
      0x88100100,  //  000D  GETMBR	R4	R0	K0
      0x94100803,  //  000E  GETIDX	R4	R4	R3
      0x88100903,  //  000F  GETMBR	R4	R4	K3
      0x1C100802,  //  0010  EQ	R4	R4	R2
      0x78120004,  //  0011  JMPF	R4	#0017
      0x88100100,  //  0012  GETMBR	R4	R0	K0
      0x8C100904,  //  0013  GETMET	R4	R4	K4
      0x5C180600,  //  0014  MOVE	R6	R3
      0x7C100400,  //  0015  CALL	R4	2
      0x70020000,  //  0016  JMP		#0018
      0x000C0705,  //  0017  ADD	R3	R3	K5
      0x7001FFE9,  //  0018  JMP		#0003
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: cmd
********************************************************************/
be_local_closure(Tasmota_cmd,   /* name */
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
    /* K0   */  be_nested_str(cmd_res),
    /* K1   */  be_nested_str(_cmd),
    }),
    &be_const_str_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x500C0200,  //  0001  LDBOOL	R3	1	0
      0x90020003,  //  0002  SETMBR	R0	K0	R3
      0x8C0C0101,  //  0003  GETMET	R3	R0	K1
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x88100100,  //  0007  GETMBR	R4	R0	K0
      0x50140200,  //  0008  LDBOOL	R5	1	0
      0x20100805,  //  0009  NE	R4	R4	R5
      0x78120000,  //  000A  JMPF	R4	#000C
      0x880C0100,  //  000B  GETMBR	R3	R0	K0
      0x90020002,  //  000C  SETMBR	R0	K0	R2
      0x80040600,  //  000D  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_cron
********************************************************************/
be_local_closure(Tasmota_remove_cron,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(_crons),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(size),
    /* K3   */  be_nested_str(id),
    /* K4   */  be_nested_str(remove),
    /* K5   */  be_const_int(1),
    }),
    &be_const_str_remove_cron,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A000E,  //  0001  JMPF	R2	#0011
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x8C100502,  //  0003  GETMET	R4	R2	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120009,  //  0006  JMPF	R4	#0011
      0x94100403,  //  0007  GETIDX	R4	R2	R3
      0x88100903,  //  0008  GETMBR	R4	R4	K3
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120003,  //  000A  JMPF	R4	#000F
      0x8C100504,  //  000B  GETMET	R4	R2	K4
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0x70020000,  //  000E  JMP		#0010
      0x000C0705,  //  000F  ADD	R3	R3	K5
      0x7001FFF1,  //  0010  JMP		#0003
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next_cron
********************************************************************/
be_local_closure(Tasmota_next_cron,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(_crons),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(size),
    /* K3   */  be_nested_str(id),
    /* K4   */  be_nested_str(trig),
    /* K5   */  be_const_int(1),
    }),
    &be_const_str_next_cron,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A000D,  //  0001  JMPF	R2	#0010
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x8C100502,  //  0003  GETMET	R4	R2	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x14100604,  //  0005  LT	R4	R3	R4
      0x78120008,  //  0006  JMPF	R4	#0010
      0x94100403,  //  0007  GETIDX	R4	R2	R3
      0x88100903,  //  0008  GETMBR	R4	R4	K3
      0x1C100801,  //  0009  EQ	R4	R4	R1
      0x78120002,  //  000A  JMPF	R4	#000E
      0x94100403,  //  000B  GETIDX	R4	R2	R3
      0x88100904,  //  000C  GETMBR	R4	R4	K4
      0x80040800,  //  000D  RET	1	R4
      0x000C0705,  //  000E  ADD	R3	R3	K5
      0x7001FFF2,  //  000F  JMP		#0003
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: hs2rgb
********************************************************************/
be_local_closure(Tasmota_hs2rgb,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(tasmota),
    /* K2   */  be_nested_str(scale_uint),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_int(2),
    /* K5   */  be_const_int(3),
    }),
    &be_const_str_hs2rgb,
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x540A00FE,  //  0003  LDINT	R2	255
      0x540E00FE,  //  0004  LDINT	R3	255
      0x541200FE,  //  0005  LDINT	R4	255
      0x541600FE,  //  0006  LDINT	R5	255
      0x541A0167,  //  0007  LDINT	R6	360
      0x10040206,  //  0008  MOD	R1	R1	R6
      0x24180500,  //  0009  GT	R6	R2	K0
      0x781A0031,  //  000A  JMPF	R6	#003D
      0x541A003B,  //  000B  LDINT	R6	60
      0x0C180206,  //  000C  DIV	R6	R1	R6
      0x541E003B,  //  000D  LDINT	R7	60
      0x101C0207,  //  000E  MOD	R7	R1	R7
      0x542200FE,  //  000F  LDINT	R8	255
      0x04201002,  //  0010  SUB	R8	R8	R2
      0xB8260200,  //  0011  GETNGBL	R9	K1
      0x8C241302,  //  0012  GETMET	R9	R9	K2
      0x5C2C0E00,  //  0013  MOVE	R11	R7
      0x58300000,  //  0014  LDCONST	R12	K0
      0x5436003B,  //  0015  LDINT	R13	60
      0x543A00FE,  //  0016  LDINT	R14	255
      0x5C3C1000,  //  0017  MOVE	R15	R8
      0x7C240C00,  //  0018  CALL	R9	6
      0xB82A0200,  //  0019  GETNGBL	R10	K1
      0x8C281502,  //  001A  GETMET	R10	R10	K2
      0x5C300E00,  //  001B  MOVE	R12	R7
      0x58340000,  //  001C  LDCONST	R13	K0
      0x543A003B,  //  001D  LDINT	R14	60
      0x5C3C1000,  //  001E  MOVE	R15	R8
      0x544200FE,  //  001F  LDINT	R16	255
      0x7C280C00,  //  0020  CALL	R10	6
      0x1C2C0D00,  //  0021  EQ	R11	R6	K0
      0x782E0002,  //  0022  JMPF	R11	#0026
      0x5C141400,  //  0023  MOVE	R5	R10
      0x5C101000,  //  0024  MOVE	R4	R8
      0x70020016,  //  0025  JMP		#003D
      0x1C2C0D03,  //  0026  EQ	R11	R6	K3
      0x782E0002,  //  0027  JMPF	R11	#002B
      0x5C0C1200,  //  0028  MOVE	R3	R9
      0x5C101000,  //  0029  MOVE	R4	R8
      0x70020011,  //  002A  JMP		#003D
      0x1C2C0D04,  //  002B  EQ	R11	R6	K4
      0x782E0002,  //  002C  JMPF	R11	#0030
      0x5C0C1000,  //  002D  MOVE	R3	R8
      0x5C101400,  //  002E  MOVE	R4	R10
      0x7002000C,  //  002F  JMP		#003D
      0x1C2C0D05,  //  0030  EQ	R11	R6	K5
      0x782E0002,  //  0031  JMPF	R11	#0035
      0x5C0C1000,  //  0032  MOVE	R3	R8
      0x5C141200,  //  0033  MOVE	R5	R9
      0x70020007,  //  0034  JMP		#003D
      0x542E0003,  //  0035  LDINT	R11	4
      0x1C2C0C0B,  //  0036  EQ	R11	R6	R11
      0x782E0002,  //  0037  JMPF	R11	#003B
      0x5C0C1400,  //  0038  MOVE	R3	R10
      0x5C141000,  //  0039  MOVE	R5	R8
      0x70020001,  //  003A  JMP		#003D
      0x5C141000,  //  003B  MOVE	R5	R8
      0x5C101200,  //  003C  MOVE	R4	R9
      0x541A000F,  //  003D  LDINT	R6	16
      0x38180606,  //  003E  SHL	R6	R3	R6
      0x541E0007,  //  003F  LDINT	R7	8
      0x381C0A07,  //  0040  SHL	R7	R5	R7
      0x30180C07,  //  0041  OR	R6	R6	R7
      0x30180C04,  //  0042  OR	R6	R6	R4
      0x80040C00,  //  0043  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_light
********************************************************************/
be_local_closure(Tasmota_set_light,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(tasmota_X2Eset_light_X28_X29_X20is_X20deprecated_X2C_X20use_X20light_X2Eset_X28_X29),
    /* K1   */  be_nested_str(light),
    /* K2   */  be_nested_str(set),
    }),
    &be_const_str_set_light,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x600C0001,  //  0000  GETGBL	R3	G1
      0x58100000,  //  0001  LDCONST	R4	K0
      0x7C0C0200,  //  0002  CALL	R3	1
      0xA40E0200,  //  0003  IMPORT	R3	K1
      0x4C100000,  //  0004  LDNIL	R4
      0x20100404,  //  0005  NE	R4	R2	R4
      0x78120005,  //  0006  JMPF	R4	#000D
      0x8C100702,  //  0007  GETMET	R4	R3	K2
      0x5C180200,  //  0008  MOVE	R6	R1
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C100600,  //  000A  CALL	R4	3
      0x80040800,  //  000B  RET	1	R4
      0x70020003,  //  000C  JMP		#0011
      0x8C100702,  //  000D  GETMET	R4	R3	K2
      0x5C180200,  //  000E  MOVE	R6	R1
      0x7C100400,  //  000F  CALL	R4	2
      0x80040800,  //  0010  RET	1	R4
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gc
********************************************************************/
be_local_closure(Tasmota_gc,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(gc),
    /* K1   */  be_nested_str(collect),
    /* K2   */  be_nested_str(allocated),
    }),
    &be_const_str_gc,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_rule
********************************************************************/
be_local_closure(Tasmota_add_rule,   /* name */
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
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_rules),
    /* K2   */  be_nested_str(function),
    /* K3   */  be_nested_str(push),
    /* K4   */  be_nested_str(Trigger),
    /* K5   */  be_nested_str(value_error),
    /* K6   */  be_nested_str(the_X20second_X20argument_X20is_X20not_X20a_X20function),
    }),
    &be_const_str_add_rule,
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x74120002,  //  0004  JMPT	R4	#0008
      0x60100012,  //  0005  GETGBL	R4	G18
      0x7C100000,  //  0006  CALL	R4	0
      0x90020204,  //  0007  SETMBR	R0	K1	R4
      0x60100004,  //  0008  GETGBL	R4	G4
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C100200,  //  000A  CALL	R4	1
      0x1C100902,  //  000B  EQ	R4	R4	K2
      0x78120008,  //  000C  JMPF	R4	#0016
      0x88100101,  //  000D  GETMBR	R4	R0	K1
      0x8C100903,  //  000E  GETMET	R4	R4	K3
      0xB81A0800,  //  000F  GETNGBL	R6	K4
      0x5C1C0200,  //  0010  MOVE	R7	R1
      0x5C200400,  //  0011  MOVE	R8	R2
      0x5C240600,  //  0012  MOVE	R9	R3
      0x7C180600,  //  0013  CALL	R6	3
      0x7C100400,  //  0014  CALL	R4	2
      0x70020000,  //  0015  JMP		#0017
      0xB0060B06,  //  0016  RAISE	1	K5	K6
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: wire_scan
********************************************************************/
be_local_closure(Tasmota_wire_scan,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(i2c_enabled),
    /* K1   */  be_nested_str(wire1),
    /* K2   */  be_nested_str(enabled),
    /* K3   */  be_nested_str(detect),
    /* K4   */  be_nested_str(wire2),
    }),
    &be_const_str_wire_scan,
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0403,  //  0001  NE	R3	R2	R3
      0x780E0005,  //  0002  JMPF	R3	#0009
      0x8C0C0100,  //  0003  GETMET	R3	R0	K0
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x740E0001,  //  0006  JMPT	R3	#0009
      0x4C0C0000,  //  0007  LDNIL	R3
      0x80040600,  //  0008  RET	1	R3
      0x880C0101,  //  0009  GETMBR	R3	R0	K1
      0x8C0C0702,  //  000A  GETMET	R3	R3	K2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x880C0101,  //  000D  GETMBR	R3	R0	K1
      0x8C0C0703,  //  000E  GETMET	R3	R3	K3
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x780E0001,  //  0011  JMPF	R3	#0014
      0x880C0101,  //  0012  GETMBR	R3	R0	K1
      0x80040600,  //  0013  RET	1	R3
      0x880C0104,  //  0014  GETMBR	R3	R0	K4
      0x8C0C0702,  //  0015  GETMET	R3	R3	K2
      0x7C0C0200,  //  0016  CALL	R3	1
      0x780E0006,  //  0017  JMPF	R3	#001F
      0x880C0104,  //  0018  GETMBR	R3	R0	K4
      0x8C0C0703,  //  0019  GETMET	R3	R3	K3
      0x5C140200,  //  001A  MOVE	R5	R1
      0x7C0C0400,  //  001B  CALL	R3	2
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x880C0104,  //  001D  GETMBR	R3	R0	K4
      0x80040600,  //  001E  RET	1	R3
      0x4C0C0000,  //  001F  LDNIL	R3
      0x80040600,  //  0020  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Tasmota_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(ctypes_bytes_dyn),
    /* K2   */  be_nested_str(_global_addr),
    /* K3   */  be_nested_str(_global_def),
    /* K4   */  be_nested_str(introspect),
    /* K5   */  be_nested_str(_settings_ptr),
    /* K6   */  be_nested_str(get),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str(settings),
    /* K9   */  be_nested_str(toptr),
    /* K10  */  be_nested_str(_settings_def),
    /* K11  */  be_nested_str(wd),
    /* K12  */  be_nested_str(),
    /* K13  */  be_nested_str(_debug_present),
    /* K14  */  be_nested_str(debug),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x88080102,  //  0001  GETMBR	R2	R0	K2
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x7C040400,  //  0003  CALL	R1	2
      0x90020001,  //  0004  SETMBR	R0	K0	R1
      0xA4060800,  //  0005  IMPORT	R1	K4
      0x60080015,  //  0006  GETGBL	R2	G21
      0x880C0105,  //  0007  GETMBR	R3	R0	K5
      0x54120003,  //  0008  LDINT	R4	4
      0x7C080400,  //  0009  CALL	R2	2
      0x8C080506,  //  000A  GETMET	R2	R2	K6
      0x58100007,  //  000B  LDCONST	R4	K7
      0x54160003,  //  000C  LDINT	R5	4
      0x7C080600,  //  000D  CALL	R2	3
      0x780A0006,  //  000E  JMPF	R2	#0016
      0xB80E0200,  //  000F  GETNGBL	R3	K1
      0x8C100309,  //  0010  GETMET	R4	R1	K9
      0x5C180400,  //  0011  MOVE	R6	R2
      0x7C100400,  //  0012  CALL	R4	2
      0x8814010A,  //  0013  GETMBR	R5	R0	K10
      0x7C0C0400,  //  0014  CALL	R3	2
      0x90021003,  //  0015  SETMBR	R0	K8	R3
      0x9002170C,  //  0016  SETMBR	R0	K11	K12
      0x500C0000,  //  0017  LDBOOL	R3	0	0
      0x90021A03,  //  0018  SETMBR	R0	K13	R3
      0xA8020004,  //  0019  EXBLK	0	#001F
      0xA40E1C00,  //  001A  IMPORT	R3	K14
      0x50100200,  //  001B  LDBOOL	R4	1	0
      0x90021A04,  //  001C  SETMBR	R0	K13	R4
      0xA8040001,  //  001D  EXBLK	1	1
      0x70020003,  //  001E  JMP		#0023
      0xAC0C0000,  //  001F  CATCH	R3	0	0
      0x70020000,  //  0020  JMP		#0022
      0x70020000,  //  0021  JMP		#0023
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event
********************************************************************/
be_local_closure(Tasmota_event,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str(introspect),
    /* K1   */  be_nested_str(string),
    /* K2   */  be_nested_str(every_50ms),
    /* K3   */  be_nested_str(run_deferred),
    /* K4   */  be_nested_str(every_250ms),
    /* K5   */  be_nested_str(run_cron),
    /* K6   */  be_nested_str(mqtt_data),
    /* K7   */  be_nested_str(cmd),
    /* K8   */  be_nested_str(exec_cmd),
    /* K9   */  be_nested_str(tele),
    /* K10  */  be_nested_str(exec_tele),
    /* K11  */  be_nested_str(rule),
    /* K12  */  be_nested_str(exec_rules),
    /* K13  */  be_nested_str(gc),
    /* K14  */  be_nested_str(_drivers),
    /* K15  */  be_const_int(0),
    /* K16  */  be_nested_str(get),
    /* K17  */  be_nested_str(function),
    /* K18  */  be_nested_str(format),
    /* K19  */  be_nested_str(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K20  */  be_nested_str(_debug_present),
    /* K21  */  be_nested_str(debug),
    /* K22  */  be_nested_str(traceback),
    /* K23  */  be_const_int(1),
    /* K24  */  be_nested_str(save_before_restart),
    /* K25  */  be_nested_str(persist),
    /* K26  */  be_nested_str(save),
    }),
    &be_const_str_event,
    &be_const_str_solidified,
    ( &(const binstruction[109]) {  /* code */
      0xA41A0000,  //  0000  IMPORT	R6	K0
      0xA41E0200,  //  0001  IMPORT	R7	K1
      0x1C200302,  //  0002  EQ	R8	R1	K2
      0x78220001,  //  0003  JMPF	R8	#0006
      0x8C200103,  //  0004  GETMET	R8	R0	K3
      0x7C200200,  //  0005  CALL	R8	1
      0x1C200304,  //  0006  EQ	R8	R1	K4
      0x78220001,  //  0007  JMPF	R8	#000A
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x50200000,  //  000A  LDBOOL	R8	0	0
      0x50240000,  //  000B  LDBOOL	R9	0	0
      0x1C280306,  //  000C  EQ	R10	R1	K6
      0x782A0000,  //  000D  JMPF	R10	#000F
      0x50240200,  //  000E  LDBOOL	R9	1	0
      0x1C280307,  //  000F  EQ	R10	R1	K7
      0x782A0006,  //  0010  JMPF	R10	#0018
      0x8C280108,  //  0011  GETMET	R10	R0	K8
      0x5C300400,  //  0012  MOVE	R12	R2
      0x5C340600,  //  0013  MOVE	R13	R3
      0x5C380800,  //  0014  MOVE	R14	R4
      0x7C280800,  //  0015  CALL	R10	4
      0x80041400,  //  0016  RET	1	R10
      0x7002004E,  //  0017  JMP		#0067
      0x1C280309,  //  0018  EQ	R10	R1	K9
      0x782A0004,  //  0019  JMPF	R10	#001F
      0x8C28010A,  //  001A  GETMET	R10	R0	K10
      0x5C300800,  //  001B  MOVE	R12	R4
      0x7C280400,  //  001C  CALL	R10	2
      0x80041400,  //  001D  RET	1	R10
      0x70020047,  //  001E  JMP		#0067
      0x1C28030B,  //  001F  EQ	R10	R1	K11
      0x782A0007,  //  0020  JMPF	R10	#0029
      0x8C28010C,  //  0021  GETMET	R10	R0	K12
      0x5C300800,  //  0022  MOVE	R12	R4
      0x60340017,  //  0023  GETGBL	R13	G23
      0x5C380600,  //  0024  MOVE	R14	R3
      0x7C340200,  //  0025  CALL	R13	1
      0x7C280600,  //  0026  CALL	R10	3
      0x80041400,  //  0027  RET	1	R10
      0x7002003D,  //  0028  JMP		#0067
      0x1C28030D,  //  0029  EQ	R10	R1	K13
      0x782A0003,  //  002A  JMPF	R10	#002F
      0x8C28010D,  //  002B  GETMET	R10	R0	K13
      0x7C280200,  //  002C  CALL	R10	1
      0x80041400,  //  002D  RET	1	R10
      0x70020037,  //  002E  JMP		#0067
      0x8828010E,  //  002F  GETMBR	R10	R0	K14
      0x782A0035,  //  0030  JMPF	R10	#0067
      0x5828000F,  //  0031  LDCONST	R10	K15
      0x602C000C,  //  0032  GETGBL	R11	G12
      0x8830010E,  //  0033  GETMBR	R12	R0	K14
      0x7C2C0200,  //  0034  CALL	R11	1
      0x142C140B,  //  0035  LT	R11	R10	R11
      0x782E002F,  //  0036  JMPF	R11	#0067
      0x882C010E,  //  0037  GETMBR	R11	R0	K14
      0x942C160A,  //  0038  GETIDX	R11	R11	R10
      0x8C300D10,  //  0039  GETMET	R12	R6	K16
      0x5C381600,  //  003A  MOVE	R14	R11
      0x5C3C0200,  //  003B  MOVE	R15	R1
      0x7C300600,  //  003C  CALL	R12	3
      0x60340004,  //  003D  GETGBL	R13	G4
      0x5C381800,  //  003E  MOVE	R14	R12
      0x7C340200,  //  003F  CALL	R13	1
      0x1C341B11,  //  0040  EQ	R13	R13	K17
      0x78360022,  //  0041  JMPF	R13	#0065
      0xA8020011,  //  0042  EXBLK	0	#0055
      0x5C341800,  //  0043  MOVE	R13	R12
      0x5C381600,  //  0044  MOVE	R14	R11
      0x5C3C0400,  //  0045  MOVE	R15	R2
      0x5C400600,  //  0046  MOVE	R16	R3
      0x5C440800,  //  0047  MOVE	R17	R4
      0x5C480A00,  //  0048  MOVE	R18	R5
      0x7C340A00,  //  0049  CALL	R13	5
      0x74360001,  //  004A  JMPT	R13	#004D
      0x74220000,  //  004B  JMPT	R8	#004D
      0x50200001,  //  004C  LDBOOL	R8	0	1
      0x50200200,  //  004D  LDBOOL	R8	1	0
      0x78220003,  //  004E  JMPF	R8	#0053
      0x5C341200,  //  004F  MOVE	R13	R9
      0x74360001,  //  0050  JMPT	R13	#0053
      0xA8040001,  //  0051  EXBLK	1	1
      0x70020013,  //  0052  JMP		#0067
      0xA8040001,  //  0053  EXBLK	1	1
      0x7002000F,  //  0054  JMP		#0065
      0xAC340002,  //  0055  CATCH	R13	0	2
      0x7002000C,  //  0056  JMP		#0064
      0x603C0001,  //  0057  GETGBL	R15	G1
      0x8C400F12,  //  0058  GETMET	R16	R7	K18
      0x58480013,  //  0059  LDCONST	R18	K19
      0x5C4C1A00,  //  005A  MOVE	R19	R13
      0x5C501C00,  //  005B  MOVE	R20	R14
      0x7C400800,  //  005C  CALL	R16	4
      0x7C3C0200,  //  005D  CALL	R15	1
      0x883C0114,  //  005E  GETMBR	R15	R0	K20
      0x783E0002,  //  005F  JMPF	R15	#0063
      0xA43E2A00,  //  0060  IMPORT	R15	K21
      0x8C401F16,  //  0061  GETMET	R16	R15	K22
      0x7C400200,  //  0062  CALL	R16	1
      0x70020000,  //  0063  JMP		#0065
      0xB0080000,  //  0064  RAISE	2	R0	R0
      0x00281517,  //  0065  ADD	R10	R10	K23
      0x7001FFCA,  //  0066  JMP		#0032
      0x1C280318,  //  0067  EQ	R10	R1	K24
      0x782A0002,  //  0068  JMPF	R10	#006C
      0xA42A3200,  //  0069  IMPORT	R10	K25
      0x8C2C151A,  //  006A  GETMET	R11	R10	K26
      0x7C2C0200,  //  006B  CALL	R11	1
      0x80041000,  //  006C  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: fast_loop
********************************************************************/
be_local_closure(Tasmota_fast_loop,   /* name */
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
    /* K0   */  be_nested_str(_fl),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    }),
    &be_const_str_fast_loop,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x5C080200,  //  0001  MOVE	R2	R1
      0x740A0000,  //  0002  JMPT	R2	#0004
      0x80000400,  //  0003  RET	0
      0x58080001,  //  0004  LDCONST	R2	K1
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x940C0202,  //  000A  GETIDX	R3	R1	R2
      0x7C0C0000,  //  000B  CALL	R3	0
      0x00080502,  //  000C  ADD	R2	R2	K2
      0x7001FFF6,  //  000D  JMP		#0005
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fast_loop
********************************************************************/
be_local_closure(Tasmota_remove_fast_loop,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_fl),
    /* K1   */  be_nested_str(find),
    /* K2   */  be_nested_str(remove),
    }),
    &be_const_str_remove_fast_loop,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0000,  //  0001  JMPT	R2	#0003
      0x80000400,  //  0002  RET	0
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x880C0100,  //  000A  GETMBR	R3	R0	K0
      0x8C0C0702,  //  000B  GETMET	R3	R3	K2
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: exec_rules
********************************************************************/
be_local_closure(Tasmota_exec_rules,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(cmd_res),
    /* K1   */  be_nested_str(_rules),
    /* K2   */  be_nested_str(json),
    /* K3   */  be_nested_str(load),
    /* K4   */  be_nested_str(log),
    /* K5   */  be_nested_str(BRY_X3A_X20ERROR_X2C_X20bad_X20json_X3A_X20),
    /* K6   */  be_const_int(3),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str(try_rule),
    /* K9   */  be_nested_str(trig),
    /* K10  */  be_nested_str(f),
    /* K11  */  be_const_int(1),
    }),
    &be_const_str_exec_rules,
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x74120002,  //  0002  JMPT	R4	#0006
      0x4C100000,  //  0003  LDNIL	R4
      0x20100604,  //  0004  NE	R4	R3	R4
      0x78120029,  //  0005  JMPF	R4	#0030
      0xA4120400,  //  0006  IMPORT	R4	K2
      0x4C140000,  //  0007  LDNIL	R5
      0x90020005,  //  0008  SETMBR	R0	K0	R5
      0x50140000,  //  0009  LDBOOL	R5	0	0
      0x8C180903,  //  000A  GETMET	R6	R4	K3
      0x5C200200,  //  000B  MOVE	R8	R1
      0x7C180400,  //  000C  CALL	R6	2
      0x4C1C0000,  //  000D  LDNIL	R7
      0x1C1C0C07,  //  000E  EQ	R7	R6	R7
      0x781E0004,  //  000F  JMPF	R7	#0015
      0x8C1C0104,  //  0010  GETMET	R7	R0	K4
      0x00260A01,  //  0011  ADD	R9	K5	R1
      0x58280006,  //  0012  LDCONST	R10	K6
      0x7C1C0600,  //  0013  CALL	R7	3
      0x5C180200,  //  0014  MOVE	R6	R1
      0x780A0014,  //  0015  JMPF	R2	#002B
      0x881C0101,  //  0016  GETMBR	R7	R0	K1
      0x781E0012,  //  0017  JMPF	R7	#002B
      0x581C0007,  //  0018  LDCONST	R7	K7
      0x6020000C,  //  0019  GETGBL	R8	G12
      0x88240101,  //  001A  GETMBR	R9	R0	K1
      0x7C200200,  //  001B  CALL	R8	1
      0x14200E08,  //  001C  LT	R8	R7	R8
      0x7822000C,  //  001D  JMPF	R8	#002B
      0x88200101,  //  001E  GETMBR	R8	R0	K1
      0x94201007,  //  001F  GETIDX	R8	R8	R7
      0x8C240108,  //  0020  GETMET	R9	R0	K8
      0x5C2C0C00,  //  0021  MOVE	R11	R6
      0x88301109,  //  0022  GETMBR	R12	R8	K9
      0x8834110A,  //  0023  GETMBR	R13	R8	K10
      0x7C240800,  //  0024  CALL	R9	4
      0x74260001,  //  0025  JMPT	R9	#0028
      0x74160000,  //  0026  JMPT	R5	#0028
      0x50140001,  //  0027  LDBOOL	R5	0	1
      0x50140200,  //  0028  LDBOOL	R5	1	0
      0x001C0F0B,  //  0029  ADD	R7	R7	K11
      0x7001FFED,  //  002A  JMP		#0019
      0x4C1C0000,  //  002B  LDNIL	R7
      0x201C0607,  //  002C  NE	R7	R3	R7
      0x781E0000,  //  002D  JMPF	R7	#002F
      0x90020006,  //  002E  SETMBR	R0	K0	R6
      0x80040A00,  //  002F  RET	1	R5
      0x50100000,  //  0030  LDBOOL	R4	0	0
      0x80040800,  //  0031  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_cmd
********************************************************************/
be_local_closure(Tasmota_add_cmd,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_ccmd),
    /* K2   */  be_nested_str(function),
    /* K3   */  be_nested_str(value_error),
    /* K4   */  be_nested_str(the_X20second_X20argument_X20is_X20not_X20a_X20function),
    }),
    &be_const_str_add_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140400,  //  0001  MOVE	R5	R2
      0x7C0C0400,  //  0002  CALL	R3	2
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x740E0002,  //  0004  JMPT	R3	#0008
      0x600C0013,  //  0005  GETGBL	R3	G19
      0x7C0C0000,  //  0006  CALL	R3	0
      0x90020203,  //  0007  SETMBR	R0	K1	R3
      0x600C0004,  //  0008  GETGBL	R3	G4
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x1C0C0702,  //  000B  EQ	R3	R3	K2
      0x780E0002,  //  000C  JMPF	R3	#0010
      0x880C0101,  //  000D  GETMBR	R3	R0	K1
      0x980C0202,  //  000E  SETIDX	R3	R1	R2
      0x70020000,  //  000F  JMP		#0011
      0xB0060704,  //  0010  RAISE	1	K3	K4
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: run_cron
********************************************************************/
be_local_closure(Tasmota_run_cron,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(_crons),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(ccronexpr),
    /* K3   */  be_nested_str(now),
    /* K4   */  be_nested_str(size),
    /* K5   */  be_nested_str(trig),
    /* K6   */  be_nested_str(next),
    /* K7   */  be_nested_str(time_reached),
    /* K8   */  be_nested_str(f),
    /* K9   */  be_const_int(1),
    }),
    &be_const_str_run_cron,
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x7806001E,  //  0001  JMPF	R1	#0021
      0x58040001,  //  0002  LDCONST	R1	K1
      0xB80A0400,  //  0003  GETNGBL	R2	K2
      0x8C080503,  //  0004  GETMET	R2	R2	K3
      0x7C080200,  //  0005  CALL	R2	1
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x8C0C0704,  //  0007  GETMET	R3	R3	K4
      0x7C0C0200,  //  0008  CALL	R3	1
      0x140C0203,  //  0009  LT	R3	R1	R3
      0x780E0015,  //  000A  JMPF	R3	#0021
      0x880C0100,  //  000B  GETMBR	R3	R0	K0
      0x940C0601,  //  000C  GETIDX	R3	R3	R1
      0x88100705,  //  000D  GETMBR	R4	R3	K5
      0x1C100901,  //  000E  EQ	R4	R4	K1
      0x78120003,  //  000F  JMPF	R4	#0014
      0x8C100706,  //  0010  GETMET	R4	R3	K6
      0x7C100200,  //  0011  CALL	R4	1
      0x900E0A04,  //  0012  SETMBR	R3	K5	R4
      0x7002000A,  //  0013  JMP		#001F
      0x8C100707,  //  0014  GETMET	R4	R3	K7
      0x7C100200,  //  0015  CALL	R4	1
      0x78120007,  //  0016  JMPF	R4	#001F
      0x88100708,  //  0017  GETMBR	R4	R3	K8
      0x8C140706,  //  0018  GETMET	R5	R3	K6
      0x7C140200,  //  0019  CALL	R5	1
      0x900E0A05,  //  001A  SETMBR	R3	K5	R5
      0x5C180800,  //  001B  MOVE	R6	R4
      0x5C1C0400,  //  001C  MOVE	R7	R2
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x00040309,  //  001F  ADD	R1	R1	K9
      0x7001FFE4,  //  0020  JMP		#0006
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_not_method
********************************************************************/
be_local_closure(Tasmota_check_not_method,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(introspect),
    /* K1   */  be_nested_str(function),
    /* K2   */  be_nested_str(type_error),
    /* K3   */  be_nested_str(BRY_X3A_X20argument_X20must_X20be_X20a_X20function),
    /* K4   */  be_nested_str(ismethod),
    /* K5   */  be_nested_str(BRY_X3A_X20method_X20not_X20allowed_X2C_X20use_X20a_X20closure_X20like_X20_X27_X2F_X20args_X20_X2D_X3E_X20obj_X2Efunc_X28args_X29_X27),
    }),
    &be_const_str_check_not_method,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0004,  //  0001  GETGBL	R3	G4
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x200C0701,  //  0004  NE	R3	R3	K1
      0x780E0000,  //  0005  JMPF	R3	#0007
      0xB0060503,  //  0006  RAISE	1	K2	K3
      0x8C0C0504,  //  0007  GETMET	R3	R2	K4
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x50100200,  //  000A  LDBOOL	R4	1	0
      0x1C0C0604,  //  000B  EQ	R3	R3	R4
      0x780E0000,  //  000C  JMPF	R3	#000E
      0xB0060505,  //  000D  RAISE	1	K2	K5
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: gen_cb
********************************************************************/
be_local_closure(Tasmota_gen_cb,   /* name */
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
    /* K0   */  be_nested_str(cb),
    /* K1   */  be_nested_str(gen_cb),
    }),
    &be_const_str_gen_cb,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x80040600,  //  0004  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_timer
********************************************************************/
be_local_closure(Tasmota_set_timer,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_timers),
    /* K2   */  be_nested_str(push),
    /* K3   */  be_nested_str(Trigger),
    /* K4   */  be_nested_str(millis),
    }),
    &be_const_str_set_timer,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x74120002,  //  0004  JMPT	R4	#0008
      0x60100012,  //  0005  GETGBL	R4	G18
      0x7C100000,  //  0006  CALL	R4	0
      0x90020204,  //  0007  SETMBR	R0	K1	R4
      0x88100101,  //  0008  GETMBR	R4	R0	K1
      0x8C100902,  //  0009  GETMET	R4	R4	K2
      0xB81A0600,  //  000A  GETNGBL	R6	K3
      0x8C1C0104,  //  000B  GETMET	R7	R0	K4
      0x5C240200,  //  000C  MOVE	R9	R1
      0x7C1C0400,  //  000D  CALL	R7	2
      0x5C200400,  //  000E  MOVE	R8	R2
      0x5C240600,  //  000F  MOVE	R9	R3
      0x7C180600,  //  0010  CALL	R6	3
      0x7C100400,  //  0011  CALL	R4	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_op
********************************************************************/
be_local_closure(Tasmota_find_op,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(_X3D_X3C_X3E_X21),
    /* K2   */  be_nested_str(_find_op),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str(split),
    /* K5   */  be_const_int(1),
    }),
    &be_const_str_find_op,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x7C100600,  //  0005  CALL	R4	3
      0x28140903,  //  0006  GE	R5	R4	K3
      0x78160018,  //  0007  JMPF	R5	#0021
      0x8C140504,  //  0008  GETMET	R5	R2	K4
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C140600,  //  000B  CALL	R5	3
      0x94180B03,  //  000C  GETIDX	R6	R5	K3
      0x941C0B05,  //  000D  GETIDX	R7	R5	K5
      0x8C200102,  //  000E  GETMET	R8	R0	K2
      0x5C280E00,  //  000F  MOVE	R10	R7
      0x502C0200,  //  0010  LDBOOL	R11	1	0
      0x7C200600,  //  0011  CALL	R8	3
      0x5C101000,  //  0012  MOVE	R4	R8
      0x28200903,  //  0013  GE	R8	R4	K3
      0x7822000B,  //  0014  JMPF	R8	#0021
      0x8C200504,  //  0015  GETMET	R8	R2	K4
      0x5C280E00,  //  0016  MOVE	R10	R7
      0x5C2C0800,  //  0017  MOVE	R11	R4
      0x7C200600,  //  0018  CALL	R8	3
      0x94241103,  //  0019  GETIDX	R9	R8	K3
      0x94281105,  //  001A  GETIDX	R10	R8	K5
      0x602C0012,  //  001B  GETGBL	R11	G18
      0x7C2C0000,  //  001C  CALL	R11	0
      0x40301606,  //  001D  CONNECT	R12	R11	R6
      0x40301609,  //  001E  CONNECT	R12	R11	R9
      0x4030160A,  //  001F  CONNECT	R12	R11	R10
      0x80041600,  //  0020  RET	1	R11
      0x60140012,  //  0021  GETGBL	R5	G18
      0x7C140000,  //  0022  CALL	R5	0
      0x40180A01,  //  0023  CONNECT	R6	R5	R1
      0x4C180000,  //  0024  LDNIL	R6
      0x40180A06,  //  0025  CONNECT	R6	R5	R6
      0x4C180000,  //  0026  LDNIL	R6
      0x40180A06,  //  0027  CONNECT	R6	R5	R6
      0x80040A00,  //  0028  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_driver
********************************************************************/
be_local_closure(Tasmota_remove_driver,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(_drivers),
    /* K1   */  be_nested_str(find),
    /* K2   */  be_nested_str(pop),
    }),
    &be_const_str_remove_driver,
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A000A,  //  0001  JMPF	R2	#000D
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x8C080501,  //  0003  GETMET	R2	R2	K1
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x200C0403,  //  0007  NE	R3	R2	R3
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C0100,  //  0009  GETMBR	R3	R0	K0
      0x8C0C0702,  //  000A  GETMET	R3	R3	K2
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_light
********************************************************************/
be_local_closure(Tasmota_get_light,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(tasmota_X2Eget_light_X28_X29_X20is_X20deprecated_X2C_X20use_X20light_X2Eget_X28_X29),
    /* K1   */  be_nested_str(light),
    /* K2   */  be_nested_str(get),
    }),
    &be_const_str_get_light,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080001,  //  0000  GETGBL	R2	G1
      0x580C0000,  //  0001  LDCONST	R3	K0
      0x7C080200,  //  0002  CALL	R2	1
      0xA40A0200,  //  0003  IMPORT	R2	K1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x200C0203,  //  0005  NE	R3	R1	R3
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x8C0C0502,  //  0007  GETMET	R3	R2	K2
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80040600,  //  000A  RET	1	R3
      0x70020002,  //  000B  JMP		#000F
      0x8C0C0502,  //  000C  GETMET	R3	R2	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80040600,  //  000E  RET	1	R3
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_driver
********************************************************************/
be_local_closure(Tasmota_add_driver,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(instance),
    /* K1   */  be_nested_str(value_error),
    /* K2   */  be_nested_str(instance_X20required),
    /* K3   */  be_nested_str(_drivers),
    /* K4   */  be_nested_str(push),
    }),
    &be_const_str_add_driver,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x20080500,  //  0003  NE	R2	R2	K0
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB0060302,  //  0005  RAISE	1	K1	K2
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x780A0004,  //  0007  JMPF	R2	#000D
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x8C080504,  //  0009  GETMET	R2	R2	K4
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x70020003,  //  000C  JMP		#0011
      0x60080012,  //  000D  GETGBL	R2	G18
      0x7C080000,  //  000E  CALL	R2	0
      0x400C0401,  //  000F  CONNECT	R3	R2	R1
      0x90020602,  //  0010  SETMBR	R0	K3	R2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_cmd
********************************************************************/
be_local_closure(Tasmota_remove_cmd,   /* name */
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
    /* K0   */  be_nested_str(_ccmd),
    /* K1   */  be_nested_str(remove),
    }),
    &be_const_str_remove_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0003,  //  0001  JMPF	R2	#0006
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x8C080501,  //  0003  GETMET	R2	R2	K1
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Tasmota
********************************************************************/
be_local_class(Tasmota,
    13,
    NULL,
    be_nested_map(47,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(find_key_i, -1), be_const_closure(Tasmota_find_key_i_closure) },
        { be_const_key(exec_cmd, 42), be_const_closure(Tasmota_exec_cmd_closure) },
        { be_const_key(remove_cmd, -1), be_const_closure(Tasmota_remove_cmd_closure) },
        { be_const_key(add_cron, -1), be_const_closure(Tasmota_add_cron_closure) },
        { be_const_key(_timers, -1), be_const_var(2) },
        { be_const_key(remove_timer, 35), be_const_closure(Tasmota_remove_timer_closure) },
        { be_const_key(run_deferred, 20), be_const_closure(Tasmota_run_deferred_closure) },
        { be_const_key(add_fast_loop, -1), be_const_closure(Tasmota_add_fast_loop_closure) },
        { be_const_key(exec_tele, 18), be_const_closure(Tasmota_exec_tele_closure) },
        { be_const_key(load, -1), be_const_closure(Tasmota_load_closure) },
        { be_const_key(add_driver, -1), be_const_closure(Tasmota_add_driver_closure) },
        { be_const_key(remove_rule, 43), be_const_closure(Tasmota_remove_rule_closure) },
        { be_const_key(cmd, 28), be_const_closure(Tasmota_cmd_closure) },
        { be_const_key(remove_cron, 30), be_const_closure(Tasmota_remove_cron_closure) },
        { be_const_key(next_cron, -1), be_const_closure(Tasmota_next_cron_closure) },
        { be_const_key(get_light, -1), be_const_closure(Tasmota_get_light_closure) },
        { be_const_key(set_light, 15), be_const_closure(Tasmota_set_light_closure) },
        { be_const_key(gc, -1), be_const_closure(Tasmota_gc_closure) },
        { be_const_key(remove_driver, -1), be_const_closure(Tasmota_remove_driver_closure) },
        { be_const_key(init, 0), be_const_closure(Tasmota_init_closure) },
        { be_const_key(remove_fast_loop, -1), be_const_closure(Tasmota_remove_fast_loop_closure) },
        { be_const_key(time_str, 19), be_const_closure(Tasmota_time_str_closure) },
        { be_const_key(global, -1), be_const_var(9) },
        { be_const_key(wire2, -1), be_const_var(7) },
        { be_const_key(event, -1), be_const_closure(Tasmota_event_closure) },
        { be_const_key(fast_loop, -1), be_const_closure(Tasmota_fast_loop_closure) },
        { be_const_key(cmd_res, 27), be_const_var(8) },
        { be_const_key(_crons, -1), be_const_var(3) },
        { be_const_key(add_cmd, -1), be_const_closure(Tasmota_add_cmd_closure) },
        { be_const_key(_debug_present, -1), be_const_var(12) },
        { be_const_key(wd, 22), be_const_var(11) },
        { be_const_key(gen_cb, 34), be_const_closure(Tasmota_gen_cb_closure) },
        { be_const_key(check_not_method, -1), be_const_closure(Tasmota_check_not_method_closure) },
        { be_const_key(hs2rgb, 31), be_const_closure(Tasmota_hs2rgb_closure) },
        { be_const_key(_rules, -1), be_const_var(1) },
        { be_const_key(run_cron, -1), be_const_closure(Tasmota_run_cron_closure) },
        { be_const_key(set_timer, -1), be_const_closure(Tasmota_set_timer_closure) },
        { be_const_key(find_op, -1), be_const_closure(Tasmota_find_op_closure) },
        { be_const_key(_drivers, 26), be_const_var(5) },
        { be_const_key(_ccmd, -1), be_const_var(4) },
        { be_const_key(settings, -1), be_const_var(10) },
        { be_const_key(wire1, -1), be_const_var(6) },
        { be_const_key(wire_scan, -1), be_const_closure(Tasmota_wire_scan_closure) },
        { be_const_key(add_rule, -1), be_const_closure(Tasmota_add_rule_closure) },
        { be_const_key(exec_rules, 10), be_const_closure(Tasmota_exec_rules_closure) },
        { be_const_key(try_rule, 2), be_const_closure(Tasmota_try_rule_closure) },
        { be_const_key(_fl, -1), be_const_var(0) },
    })),
    (bstring*) &be_const_str_Tasmota
);
/*******************************************************************/

void be_load_Tasmota_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Tasmota);
    be_setglobal(vm, "Tasmota");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
