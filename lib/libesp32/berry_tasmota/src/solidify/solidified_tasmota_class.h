/* Solidification of tasmota_class.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Tasmota;

/********************************************************************
** Solidified function: try_rule
********************************************************************/
be_local_closure(Tasmota_try_rule,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(match),
    /* K1   */  be_nested_str(trigger),
    }),
    &be_const_str_try_rule,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C100500,  //  0000  GETMET	R4	R2	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x7C100400,  //  0002  CALL	R4	2
      0x4C140000,  //  0003  LDNIL	R5
      0x20140805,  //  0004  NE	R5	R4	R5
      0x78160009,  //  0005  JMPF	R5	#0010
      0x4C140000,  //  0006  LDNIL	R5
      0x20140605,  //  0007  NE	R5	R3	R5
      0x78160004,  //  0008  JMPF	R5	#000E
      0x5C140600,  //  0009  MOVE	R5	R3
      0x5C180800,  //  000A  MOVE	R6	R4
      0x881C0501,  //  000B  GETMBR	R7	R2	K1
      0x5C200200,  //  000C  MOVE	R8	R1
      0x7C140600,  //  000D  CALL	R5	3
      0x50140200,  //  000E  LDBOOL	R5	1	0
      0x80040A00,  //  000F  RET	1	R5
      0x50140000,  //  0010  LDBOOL	R5	0	0
      0x80040A00,  //  0011  RET	1	R5
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
      0x14100403,  //  0008  LT	R4	R2	R3
      0x78120003,  //  0009  JMPF	R4	#000E
      0x94100202,  //  000A  GETIDX	R4	R1	R2
      0x7C100000,  //  000B  CALL	R4	0
      0x00080502,  //  000C  ADD	R2	R2	K2
      0x7001FFF9,  //  000D  JMP		#0008
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
** Solidified function: event
********************************************************************/
be_local_closure(Tasmota_event,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str(introspect),
    /* K1   */  be_nested_str(every_50ms),
    /* K2   */  be_nested_str(run_deferred),
    /* K3   */  be_nested_str(every_250ms),
    /* K4   */  be_nested_str(run_cron),
    /* K5   */  be_nested_str(mqtt_data),
    /* K6   */  be_nested_str(cmd),
    /* K7   */  be_nested_str(exec_cmd),
    /* K8   */  be_nested_str(tele),
    /* K9   */  be_nested_str(exec_tele),
    /* K10  */  be_nested_str(rule),
    /* K11  */  be_nested_str(exec_rules),
    /* K12  */  be_nested_str(gc),
    /* K13  */  be_nested_str(_drivers),
    /* K14  */  be_const_int(0),
    /* K15  */  be_nested_str(get),
    /* K16  */  be_nested_str(function),
    /* K17  */  be_nested_str(BRY_X3A_X20Exception_X3E_X20_X27_X25s_X27_X20_X2D_X20_X25s),
    /* K18  */  be_nested_str(_debug_present),
    /* K19  */  be_nested_str(debug),
    /* K20  */  be_nested_str(traceback),
    /* K21  */  be_const_int(1),
    /* K22  */  be_nested_str(save_before_restart),
    /* K23  */  be_nested_str(persist),
    /* K24  */  be_nested_str(save),
    }),
    &be_const_str_event,
    &be_const_str_solidified,
    ( &(const binstruction[108]) {  /* code */
      0xA41A0000,  //  0000  IMPORT	R6	K0
      0x1C1C0301,  //  0001  EQ	R7	R1	K1
      0x781E0001,  //  0002  JMPF	R7	#0005
      0x8C1C0102,  //  0003  GETMET	R7	R0	K2
      0x7C1C0200,  //  0004  CALL	R7	1
      0x1C1C0303,  //  0005  EQ	R7	R1	K3
      0x781E0001,  //  0006  JMPF	R7	#0009
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x501C0000,  //  0009  LDBOOL	R7	0	0
      0x50200000,  //  000A  LDBOOL	R8	0	0
      0x1C240305,  //  000B  EQ	R9	R1	K5
      0x78260000,  //  000C  JMPF	R9	#000E
      0x50200200,  //  000D  LDBOOL	R8	1	0
      0x1C240306,  //  000E  EQ	R9	R1	K6
      0x78260006,  //  000F  JMPF	R9	#0017
      0x8C240107,  //  0010  GETMET	R9	R0	K7
      0x5C2C0400,  //  0011  MOVE	R11	R2
      0x5C300600,  //  0012  MOVE	R12	R3
      0x5C340800,  //  0013  MOVE	R13	R4
      0x7C240800,  //  0014  CALL	R9	4
      0x80041200,  //  0015  RET	1	R9
      0x7002004E,  //  0016  JMP		#0066
      0x1C240308,  //  0017  EQ	R9	R1	K8
      0x78260004,  //  0018  JMPF	R9	#001E
      0x8C240109,  //  0019  GETMET	R9	R0	K9
      0x5C2C0800,  //  001A  MOVE	R11	R4
      0x7C240400,  //  001B  CALL	R9	2
      0x80041200,  //  001C  RET	1	R9
      0x70020047,  //  001D  JMP		#0066
      0x1C24030A,  //  001E  EQ	R9	R1	K10
      0x78260007,  //  001F  JMPF	R9	#0028
      0x8C24010B,  //  0020  GETMET	R9	R0	K11
      0x5C2C0800,  //  0021  MOVE	R11	R4
      0x60300017,  //  0022  GETGBL	R12	G23
      0x5C340600,  //  0023  MOVE	R13	R3
      0x7C300200,  //  0024  CALL	R12	1
      0x7C240600,  //  0025  CALL	R9	3
      0x80041200,  //  0026  RET	1	R9
      0x7002003D,  //  0027  JMP		#0066
      0x1C24030C,  //  0028  EQ	R9	R1	K12
      0x78260003,  //  0029  JMPF	R9	#002E
      0x8C24010C,  //  002A  GETMET	R9	R0	K12
      0x7C240200,  //  002B  CALL	R9	1
      0x80041200,  //  002C  RET	1	R9
      0x70020037,  //  002D  JMP		#0066
      0x8824010D,  //  002E  GETMBR	R9	R0	K13
      0x78260035,  //  002F  JMPF	R9	#0066
      0x5824000E,  //  0030  LDCONST	R9	K14
      0x6028000C,  //  0031  GETGBL	R10	G12
      0x882C010D,  //  0032  GETMBR	R11	R0	K13
      0x7C280200,  //  0033  CALL	R10	1
      0x1428120A,  //  0034  LT	R10	R9	R10
      0x782A002F,  //  0035  JMPF	R10	#0066
      0x8828010D,  //  0036  GETMBR	R10	R0	K13
      0x94281409,  //  0037  GETIDX	R10	R10	R9
      0x8C2C0D0F,  //  0038  GETMET	R11	R6	K15
      0x5C341400,  //  0039  MOVE	R13	R10
      0x5C380200,  //  003A  MOVE	R14	R1
      0x7C2C0600,  //  003B  CALL	R11	3
      0x60300004,  //  003C  GETGBL	R12	G4
      0x5C341600,  //  003D  MOVE	R13	R11
      0x7C300200,  //  003E  CALL	R12	1
      0x1C301910,  //  003F  EQ	R12	R12	K16
      0x78320022,  //  0040  JMPF	R12	#0064
      0xA8020011,  //  0041  EXBLK	0	#0054
      0x5C301600,  //  0042  MOVE	R12	R11
      0x5C341400,  //  0043  MOVE	R13	R10
      0x5C380400,  //  0044  MOVE	R14	R2
      0x5C3C0600,  //  0045  MOVE	R15	R3
      0x5C400800,  //  0046  MOVE	R16	R4
      0x5C440A00,  //  0047  MOVE	R17	R5
      0x7C300A00,  //  0048  CALL	R12	5
      0x74320001,  //  0049  JMPT	R12	#004C
      0x741E0000,  //  004A  JMPT	R7	#004C
      0x501C0001,  //  004B  LDBOOL	R7	0	1
      0x501C0200,  //  004C  LDBOOL	R7	1	0
      0x781E0003,  //  004D  JMPF	R7	#0052
      0x5C301000,  //  004E  MOVE	R12	R8
      0x74320001,  //  004F  JMPT	R12	#0052
      0xA8040001,  //  0050  EXBLK	1	1
      0x70020013,  //  0051  JMP		#0066
      0xA8040001,  //  0052  EXBLK	1	1
      0x7002000F,  //  0053  JMP		#0064
      0xAC300002,  //  0054  CATCH	R12	0	2
      0x7002000C,  //  0055  JMP		#0063
      0x60380001,  //  0056  GETGBL	R14	G1
      0x603C0018,  //  0057  GETGBL	R15	G24
      0x58400011,  //  0058  LDCONST	R16	K17
      0x5C441800,  //  0059  MOVE	R17	R12
      0x5C481A00,  //  005A  MOVE	R18	R13
      0x7C3C0600,  //  005B  CALL	R15	3
      0x7C380200,  //  005C  CALL	R14	1
      0x88380112,  //  005D  GETMBR	R14	R0	K18
      0x783A0002,  //  005E  JMPF	R14	#0062
      0xA43A2600,  //  005F  IMPORT	R14	K19
      0x8C3C1D14,  //  0060  GETMET	R15	R14	K20
      0x7C3C0200,  //  0061  CALL	R15	1
      0x70020000,  //  0062  JMP		#0064
      0xB0080000,  //  0063  RAISE	2	R0	R0
      0x00241315,  //  0064  ADD	R9	R9	K21
      0x7001FFCA,  //  0065  JMP		#0031
      0x1C240316,  //  0066  EQ	R9	R1	K22
      0x78260002,  //  0067  JMPF	R9	#006B
      0xA4262E00,  //  0068  IMPORT	R9	K23
      0x8C281318,  //  0069  GETMET	R10	R9	K24
      0x7C280200,  //  006A  CALL	R10	1
      0x80040E00,  //  006B  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: urlfetch
********************************************************************/
be_local_closure(Tasmota_urlfetch,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(split),
    /* K2   */  be_nested_str(_X2F),
    /* K3   */  be_nested_str(pop),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str(index_X2Ehtml),
    /* K6   */  be_nested_str(webclient),
    /* K7   */  be_nested_str(set_follow_redirects),
    /* K8   */  be_nested_str(begin),
    /* K9   */  be_nested_str(GET),
    /* K10  */  be_nested_str(status_X3A_X20),
    /* K11  */  be_nested_str(connection_error),
    /* K12  */  be_nested_str(write_file),
    /* K13  */  be_nested_str(close),
    /* K14  */  be_nested_str(log),
    /* K15  */  be_nested_str(BRY_X3A_X20Fetched_X20),
    /* K16  */  be_const_int(3),
    }),
    &be_const_str_urlfetch,
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E000D,  //  0002  JMPF	R3	#0011
      0xA40E0000,  //  0003  IMPORT	R3	K0
      0x8C100701,  //  0004  GETMET	R4	R3	K1
      0x5C180200,  //  0005  MOVE	R6	R1
      0x581C0002,  //  0006  LDCONST	R7	K2
      0x7C100600,  //  0007  CALL	R4	3
      0x8C100903,  //  0008  GETMET	R4	R4	K3
      0x7C100200,  //  0009  CALL	R4	1
      0x5C080800,  //  000A  MOVE	R2	R4
      0x6010000C,  //  000B  GETGBL	R4	G12
      0x5C140400,  //  000C  MOVE	R5	R2
      0x7C100200,  //  000D  CALL	R4	1
      0x1C100904,  //  000E  EQ	R4	R4	K4
      0x78120000,  //  000F  JMPF	R4	#0011
      0x58080005,  //  0010  LDCONST	R2	K5
      0xB80E0C00,  //  0011  GETNGBL	R3	K6
      0x7C0C0000,  //  0012  CALL	R3	0
      0x8C100707,  //  0013  GETMET	R4	R3	K7
      0x50180200,  //  0014  LDBOOL	R6	1	0
      0x7C100400,  //  0015  CALL	R4	2
      0x8C100708,  //  0016  GETMET	R4	R3	K8
      0x5C180200,  //  0017  MOVE	R6	R1
      0x7C100400,  //  0018  CALL	R4	2
      0x8C100709,  //  0019  GETMET	R4	R3	K9
      0x7C100200,  //  001A  CALL	R4	1
      0x541600C7,  //  001B  LDINT	R5	200
      0x20140805,  //  001C  NE	R5	R4	R5
      0x78160004,  //  001D  JMPF	R5	#0023
      0x60140008,  //  001E  GETGBL	R5	G8
      0x5C180800,  //  001F  MOVE	R6	R4
      0x7C140200,  //  0020  CALL	R5	1
      0x00161405,  //  0021  ADD	R5	K10	R5
      0xB0061605,  //  0022  RAISE	1	K11	R5
      0x8C14070C,  //  0023  GETMET	R5	R3	K12
      0x5C1C0400,  //  0024  MOVE	R7	R2
      0x7C140400,  //  0025  CALL	R5	2
      0x8C18070D,  //  0026  GETMET	R6	R3	K13
      0x7C180200,  //  0027  CALL	R6	1
      0x8C18010E,  //  0028  GETMET	R6	R0	K14
      0x60200008,  //  0029  GETGBL	R8	G8
      0x5C240A00,  //  002A  MOVE	R9	R5
      0x7C200200,  //  002B  CALL	R8	1
      0x00221E08,  //  002C  ADD	R8	K15	R8
      0x58240010,  //  002D  LDCONST	R9	K16
      0x7C180600,  //  002E  CALL	R6	3
      0x80040800,  //  002F  RET	1	R4
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
** Solidified function: find_list_i
********************************************************************/
be_local_closure(Tasmota_find_list_i,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(toupper),
    /* K3   */  be_const_int(1),
    }),
    &be_const_str_find_list_i,
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x8C140702,  //  0002  GETMET	R5	R3	K2
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x7C140400,  //  0004  CALL	R5	2
      0x6018000C,  //  0005  GETGBL	R6	G12
      0x5C1C0200,  //  0006  MOVE	R7	R1
      0x7C180200,  //  0007  CALL	R6	1
      0x14180806,  //  0008  LT	R6	R4	R6
      0x781A0007,  //  0009  JMPF	R6	#0012
      0x8C180702,  //  000A  GETMET	R6	R3	K2
      0x94200204,  //  000B  GETIDX	R8	R1	R4
      0x7C180400,  //  000C  CALL	R6	2
      0x1C180C05,  //  000D  EQ	R6	R6	R5
      0x781A0000,  //  000E  JMPF	R6	#0010
      0x80040800,  //  000F  RET	1	R4
      0x00100903,  //  0010  ADD	R4	R4	K3
      0x7001FFF2,  //  0011  JMP		#0005
      0x4C180000,  //  0012  LDNIL	R6
      0x80040C00,  //  0013  RET	1	R6
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
** Solidified function: init
********************************************************************/
be_local_closure(Tasmota_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        10,                          /* nstack */
        4,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str(urlfetch_cmd),
        }),
        &be_const_str__anonymous_,
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x68100000,  //  0000  GETUPV	R4	U0
          0x8C100900,  //  0001  GETMET	R4	R4	K0
          0x5C180000,  //  0002  MOVE	R6	R0
          0x5C1C0200,  //  0003  MOVE	R7	R1
          0x5C200400,  //  0004  MOVE	R8	R2
          0x5C240600,  //  0005  MOVE	R9	R3
          0x7C100A00,  //  0006  CALL	R4	5
          0x80000000,  //  0007  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
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
    /* K15  */  be_nested_str(add_cmd),
    /* K16  */  be_nested_str(UrlFetch),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
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
      0x8C0C010F,  //  0023  GETMET	R3	R0	K15
      0x58140010,  //  0024  LDCONST	R5	K16
      0x84180000,  //  0025  CLOSURE	R6	P0
      0x7C0C0600,  //  0026  CALL	R3	3
      0xA0000000,  //  0027  CLOSE	R0
      0x80000000,  //  0028  RET	0
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
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(instance),
    /* K1   */  be_nested_str(value_error),
    /* K2   */  be_nested_str(instance_X20required),
    /* K3   */  be_nested_str(_drivers),
    /* K4   */  be_nested_str(find),
    /* K5   */  be_nested_str(push),
    }),
    &be_const_str_add_driver,
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x20080500,  //  0003  NE	R2	R2	K0
      0x780A0000,  //  0004  JMPF	R2	#0006
      0xB0060302,  //  0005  RAISE	1	K1	K2
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x780A000B,  //  0007  JMPF	R2	#0014
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x8C080504,  //  0009  GETMET	R2	R2	K4
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x4C0C0000,  //  000C  LDNIL	R3
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A0003,  //  000E  JMPF	R2	#0013
      0x88080103,  //  000F  GETMBR	R2	R0	K3
      0x8C080505,  //  0010  GETMET	R2	R2	K5
      0x5C100200,  //  0011  MOVE	R4	R1
      0x7C080400,  //  0012  CALL	R2	2
      0x70020003,  //  0013  JMP		#0018
      0x60080012,  //  0014  GETGBL	R2	G18
      0x7C080000,  //  0015  CALL	R2	0
      0x400C0401,  //  0016  CONNECT	R3	R2	R1
      0x90020602,  //  0017  SETMBR	R0	K3	R2
      0x80000000,  //  0018  RET	0
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
    ( &(const binstruction[20]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140400,  //  0001  MOVE	R5	R2
      0x7C0C0400,  //  0002  CALL	R3	2
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x4C100000,  //  0004  LDNIL	R4
      0x1C0C0604,  //  0005  EQ	R3	R3	R4
      0x780E0002,  //  0006  JMPF	R3	#000A
      0x600C0013,  //  0007  GETGBL	R3	G19
      0x7C0C0000,  //  0008  CALL	R3	0
      0x90020203,  //  0009  SETMBR	R0	K1	R3
      0x600C0004,  //  000A  GETGBL	R3	G4
      0x5C100400,  //  000B  MOVE	R4	R2
      0x7C0C0200,  //  000C  CALL	R3	1
      0x1C0C0702,  //  000D  EQ	R3	R3	K2
      0x780E0002,  //  000E  JMPF	R3	#0012
      0x880C0101,  //  000F  GETMBR	R3	R0	K1
      0x980C0202,  //  0010  SETIDX	R3	R1	R2
      0x70020000,  //  0011  JMP		#0013
      0xB0060704,  //  0012  RAISE	1	K3	K4
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_op
********************************************************************/
be_local_closure(Tasmota_find_op,   /* name */
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
    /* K0   */  be_nested_str(_find_op),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    /* K3   */  be_const_int(2147483647),
    }),
    &be_const_str_find_op,
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x280C0501,  //  0003  GE	R3	R2	K1
      0x780E0011,  //  0004  JMPF	R3	#0017
      0x540E7FFE,  //  0005  LDINT	R3	32767
      0x2C0C0403,  //  0006  AND	R3	R2	R3
      0x5412000F,  //  0007  LDINT	R4	16
      0x3C100404,  //  0008  SHR	R4	R2	R4
      0x60140012,  //  0009  GETGBL	R5	G18
      0x7C140000,  //  000A  CALL	R5	0
      0x04180702,  //  000B  SUB	R6	R3	K2
      0x401A0206,  //  000C  CONNECT	R6	K1	R6
      0x94180206,  //  000D  GETIDX	R6	R1	R6
      0x40180A06,  //  000E  CONNECT	R6	R5	R6
      0x04180902,  //  000F  SUB	R6	R4	K2
      0x40180606,  //  0010  CONNECT	R6	R3	R6
      0x94180206,  //  0011  GETIDX	R6	R1	R6
      0x40180A06,  //  0012  CONNECT	R6	R5	R6
      0x40180903,  //  0013  CONNECT	R6	R4	K3
      0x94180206,  //  0014  GETIDX	R6	R1	R6
      0x40180A06,  //  0015  CONNECT	R6	R5	R6
      0x80040A00,  //  0016  RET	1	R5
      0x600C0012,  //  0017  GETGBL	R3	G18
      0x7C0C0000,  //  0018  CALL	R3	0
      0x40100601,  //  0019  CONNECT	R4	R3	R1
      0x4C100000,  //  001A  LDNIL	R4
      0x40100604,  //  001B  CONNECT	R4	R3	R4
      0x4C100000,  //  001C  LDNIL	R4
      0x40100604,  //  001D  CONNECT	R4	R3	R4
      0x80040600,  //  001E  RET	1	R3
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
    ( &(const binstruction[27]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x4C140000,  //  0004  LDNIL	R5
      0x1C100805,  //  0005  EQ	R4	R4	R5
      0x78120002,  //  0006  JMPF	R4	#000A
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C100000,  //  0008  CALL	R4	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0xB8120400,  //  000A  GETNGBL	R4	K2
      0x60140008,  //  000B  GETGBL	R5	G8
      0x5C180200,  //  000C  MOVE	R6	R1
      0x7C140200,  //  000D  CALL	R5	1
      0x7C100200,  //  000E  CALL	R4	1
      0x8C140903,  //  000F  GETMET	R5	R4	K3
      0x7C140200,  //  0010  CALL	R5	1
      0x88180101,  //  0011  GETMBR	R6	R0	K1
      0x8C180D04,  //  0012  GETMET	R6	R6	K4
      0xB8220A00,  //  0013  GETNGBL	R8	K5
      0x5C240A00,  //  0014  MOVE	R9	R5
      0x5C280400,  //  0015  MOVE	R10	R2
      0x5C2C0600,  //  0016  MOVE	R11	R3
      0x5C300800,  //  0017  MOVE	R12	R4
      0x7C200800,  //  0018  CALL	R8	4
      0x7C180400,  //  0019  CALL	R6	2
      0x80000000,  //  001A  RET	0
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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(_rules),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(trig),
    /* K3   */  be_nested_str(rule),
    /* K4   */  be_nested_str(id),
    /* K5   */  be_nested_str(remove),
    /* K6   */  be_const_int(1),
    }),
    &be_const_str_remove_rule,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x780E0017,  //  0001  JMPF	R3	#001A
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x6010000C,  //  0003  GETGBL	R4	G12
      0x88140100,  //  0004  GETMBR	R5	R0	K0
      0x7C100200,  //  0005  CALL	R4	1
      0x14100604,  //  0006  LT	R4	R3	R4
      0x78120011,  //  0007  JMPF	R4	#001A
      0x88100100,  //  0008  GETMBR	R4	R0	K0
      0x94100803,  //  0009  GETIDX	R4	R4	R3
      0x88100902,  //  000A  GETMBR	R4	R4	K2
      0x88100903,  //  000B  GETMBR	R4	R4	K3
      0x1C100801,  //  000C  EQ	R4	R4	R1
      0x78120009,  //  000D  JMPF	R4	#0018
      0x88100100,  //  000E  GETMBR	R4	R0	K0
      0x94100803,  //  000F  GETIDX	R4	R4	R3
      0x88100904,  //  0010  GETMBR	R4	R4	K4
      0x1C100802,  //  0011  EQ	R4	R4	R2
      0x78120004,  //  0012  JMPF	R4	#0018
      0x88100100,  //  0013  GETMBR	R4	R0	K0
      0x8C100905,  //  0014  GETMET	R4	R4	K5
      0x5C180600,  //  0015  MOVE	R6	R3
      0x7C100400,  //  0016  CALL	R4	2
      0x70020000,  //  0017  JMP		#0019
      0x000C0706,  //  0018  ADD	R3	R3	K6
      0x7001FFE8,  //  0019  JMP		#0003
      0x80000000,  //  001A  RET	0
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
** Solidified function: cmd
********************************************************************/
be_local_closure(Tasmota_cmd,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(cmd_res),
    /* K1   */  be_nested_str(tasmota),
    /* K2   */  be_nested_str(global),
    /* K3   */  be_nested_str(maxlog_level),
    /* K4   */  be_const_int(2),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str(_cmd),
    }),
    &be_const_str_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x50100200,  //  0001  LDBOOL	R4	1	0
      0x90020004,  //  0002  SETMBR	R0	K0	R4
      0xB8120200,  //  0003  GETNGBL	R4	K1
      0x88100902,  //  0004  GETMBR	R4	R4	K2
      0x88100903,  //  0005  GETMBR	R4	R4	K3
      0x780A0004,  //  0006  JMPF	R2	#000C
      0x28140904,  //  0007  GE	R5	R4	K4
      0x78160002,  //  0008  JMPF	R5	#000C
      0xB8160200,  //  0009  GETNGBL	R5	K1
      0x88140B02,  //  000A  GETMBR	R5	R5	K2
      0x90160705,  //  000B  SETMBR	R5	K3	K5
      0x8C140106,  //  000C  GETMET	R5	R0	K6
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x7C140400,  //  000E  CALL	R5	2
      0x4C140000,  //  000F  LDNIL	R5
      0x88180100,  //  0010  GETMBR	R6	R0	K0
      0x501C0200,  //  0011  LDBOOL	R7	1	0
      0x20180C07,  //  0012  NE	R6	R6	R7
      0x781A0000,  //  0013  JMPF	R6	#0015
      0x88140100,  //  0014  GETMBR	R5	R0	K0
      0x90020003,  //  0015  SETMBR	R0	K0	R3
      0x780A0002,  //  0016  JMPF	R2	#001A
      0xB81A0200,  //  0017  GETNGBL	R6	K1
      0x88180D02,  //  0018  GETMBR	R6	R6	K2
      0x901A0604,  //  0019  SETMBR	R6	K3	R4
      0x80040A00,  //  001A  RET	1	R5
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
    ( &(const binstruction[21]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x4C140000,  //  0004  LDNIL	R5
      0x1C100805,  //  0005  EQ	R4	R4	R5
      0x78120002,  //  0006  JMPF	R4	#000A
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C100000,  //  0008  CALL	R4	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x88100101,  //  000A  GETMBR	R4	R0	K1
      0x8C100902,  //  000B  GETMET	R4	R4	K2
      0xB81A0600,  //  000C  GETNGBL	R6	K3
      0x8C1C0104,  //  000D  GETMET	R7	R0	K4
      0x5C240200,  //  000E  MOVE	R9	R1
      0x7C1C0400,  //  000F  CALL	R7	2
      0x5C200400,  //  0010  MOVE	R8	R2
      0x5C240600,  //  0011  MOVE	R9	R3
      0x7C180600,  //  0012  CALL	R6	3
      0x7C100400,  //  0013  CALL	R4	2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load
********************************************************************/
be_local_closure(Tasmota_load,   /* name */
  be_nested_proto(
    26,                          /* nstack */
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
        8,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 8]) {     /* constants */
        /* K0   */  be_nested_str(r),
        /* K1   */  be_nested_str(readbytes),
        /* K2   */  be_const_int(3),
        /* K3   */  be_const_int(1),
        /* K4   */  be_nested_str(BECDFE),
        /* K5   */  be_const_int(0),
        /* K6   */  be_nested_str(close),
        /* K7   */  be_nested_str(BRY_X3A_X20failed_X20to_X20load_X20compiled_X20_X27_X25s_X27_X20_X28_X25s_X29),
        }),
        &be_const_str_try_get_bec_version,
        &be_const_str_solidified,
        ( &(const binstruction[43]) {  /* code */
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
          0x7002000F,  //  0018  JMP		#0029
          0xAC080001,  //  0019  CATCH	R2	0	1
          0x7002000C,  //  001A  JMP		#0028
          0x4C0C0000,  //  001B  LDNIL	R3
          0x200C0203,  //  001C  NE	R3	R1	R3
          0x780E0001,  //  001D  JMPF	R3	#0020
          0x8C0C0306,  //  001E  GETMET	R3	R1	K6
          0x7C0C0200,  //  001F  CALL	R3	1
          0x600C0001,  //  0020  GETGBL	R3	G1
          0x60100018,  //  0021  GETGBL	R4	G24
          0x58140007,  //  0022  LDCONST	R5	K7
          0x5C180000,  //  0023  MOVE	R6	R0
          0x5C1C0400,  //  0024  MOVE	R7	R2
          0x7C100600,  //  0025  CALL	R4	3
          0x7C0C0200,  //  0026  CALL	R3	1
          0x70020000,  //  0027  JMP		#0029
          0xB0080000,  //  0028  RAISE	2	R0	R0
          0x4C080000,  //  0029  LDNIL	R2
          0x80040400,  //  002A  RET	1	R2
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
        9,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str(file),
        /* K1   */  be_nested_str(BRY_X3A_X20failed_X20to_X20load_X20_X27_X25s_X27_X20_X28_X25s_X20_X2D_X20_X25s_X29),
        }),
        &be_const_str_try_compile,
        &be_const_str_solidified,
        ( &(const binstruction[23]) {  /* code */
          0xA8020007,  //  0000  EXBLK	0	#0009
          0x6004000D,  //  0001  GETGBL	R1	G13
          0x5C080000,  //  0002  MOVE	R2	R0
          0x580C0000,  //  0003  LDCONST	R3	K0
          0x7C040400,  //  0004  CALL	R1	2
          0xA8040001,  //  0005  EXBLK	1	1
          0x80040200,  //  0006  RET	1	R1
          0xA8040001,  //  0007  EXBLK	1	1
          0x7002000B,  //  0008  JMP		#0015
          0xAC040002,  //  0009  CATCH	R1	0	2
          0x70020008,  //  000A  JMP		#0014
          0x600C0001,  //  000B  GETGBL	R3	G1
          0x60100018,  //  000C  GETGBL	R4	G24
          0x58140001,  //  000D  LDCONST	R5	K1
          0x5C180000,  //  000E  MOVE	R6	R0
          0x5C1C0200,  //  000F  MOVE	R7	R1
          0x5C200400,  //  0010  MOVE	R8	R2
          0x7C100800,  //  0011  CALL	R4	4
          0x7C0C0200,  //  0012  CALL	R3	1
          0x70020000,  //  0013  JMP		#0015
          0xB0080000,  //  0014  RAISE	2	R0	R0
          0x4C040000,  //  0015  LDNIL	R1
          0x80040200,  //  0016  RET	1	R1
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str(BRY_X3A_X20failed_X20to_X20run_X20compiled_X20code_X20_X27_X25s_X27_X20_X2D_X20_X25s),
        }),
        &be_const_str_try_run_compiled,
        &be_const_str_solidified,
        ( &(const binstruction[24]) {  /* code */
          0x4C040000,  //  0000  LDNIL	R1
          0x20040001,  //  0001  NE	R1	R0	R1
          0x78060012,  //  0002  JMPF	R1	#0016
          0xA8020006,  //  0003  EXBLK	0	#000B
          0x5C040000,  //  0004  MOVE	R1	R0
          0x7C040000,  //  0005  CALL	R1	0
          0x50040200,  //  0006  LDBOOL	R1	1	0
          0xA8040001,  //  0007  EXBLK	1	1
          0x80040200,  //  0008  RET	1	R1
          0xA8040001,  //  0009  EXBLK	1	1
          0x7002000A,  //  000A  JMP		#0016
          0xAC040002,  //  000B  CATCH	R1	0	2
          0x70020007,  //  000C  JMP		#0015
          0x600C0001,  //  000D  GETGBL	R3	G1
          0x60100018,  //  000E  GETGBL	R4	G24
          0x58140000,  //  000F  LDCONST	R5	K0
          0x5C180200,  //  0010  MOVE	R6	R1
          0x5C1C0400,  //  0011  MOVE	R7	R2
          0x7C100600,  //  0012  CALL	R4	3
          0x7C0C0200,  //  0013  CALL	R3	1
          0x70020000,  //  0014  JMP		#0016
          0xB0080000,  //  0015  RAISE	2	R0	R0
          0x50040000,  //  0016  LDBOOL	R1	0	0
          0x80040200,  //  0017  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
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
    /* K17  */  be_nested_str(BRY_X3A_X20corrupt_X20bytecode_X20_X27_X25s_X27),
    /* K18  */  be_nested_str(BRY_X3A_X20bytecode_X20has_X20wrong_X20version_X20_X27_X25s_X27_X20_X28_X25i_X29),
    /* K19  */  be_nested_str(save),
    /* K20  */  be_nested_str(BRY_X3A_X20could_X20not_X20save_X20compiled_X20file_X20_X25s_X20_X28_X25s_X29),
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
      0x60580018,  //  0069  GETGBL	R22	G24
      0x585C0011,  //  006A  LDCONST	R23	K17
      0x5C602200,  //  006B  MOVE	R24	R17
      0x7C580400,  //  006C  CALL	R22	2
      0x7C540200,  //  006D  CALL	R21	1
      0x50500000,  //  006E  LDBOOL	R20	0	0
      0x7002000A,  //  006F  JMP		#007B
      0x54560003,  //  0070  LDINT	R21	4
      0x20542615,  //  0071  NE	R21	R19	R21
      0x78560007,  //  0072  JMPF	R21	#007B
      0x60540001,  //  0073  GETGBL	R21	G1
      0x60580018,  //  0074  GETGBL	R22	G24
      0x585C0012,  //  0075  LDCONST	R23	K18
      0x5C602200,  //  0076  MOVE	R24	R17
      0x5C642600,  //  0077  MOVE	R25	R19
      0x7C580600,  //  0078  CALL	R22	3
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
      0x8C4C0113,  //  0097  GETMET	R19	R0	K19
      0x5C542200,  //  0098  MOVE	R21	R17
      0x5C582400,  //  0099  MOVE	R22	R18
      0x7C4C0600,  //  009A  CALL	R19	3
      0xA8040001,  //  009B  EXBLK	1	1
      0x7002000A,  //  009C  JMP		#00A8
      0xAC4C0001,  //  009D  CATCH	R19	0	1
      0x70020007,  //  009E  JMP		#00A7
      0x60500001,  //  009F  GETGBL	R20	G1
      0x60540018,  //  00A0  GETGBL	R21	G24
      0x58580014,  //  00A1  LDCONST	R22	K20
      0x5C5C2200,  //  00A2  MOVE	R23	R17
      0x5C602600,  //  00A3  MOVE	R24	R19
      0x7C540600,  //  00A4  CALL	R21	3
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
** Solidified function: time_str
********************************************************************/
be_local_closure(Tasmota_time_str,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(time_dump),
    /* K1   */  be_nested_str(_X2504d_X2D_X2502d_X2D_X2502dT_X2502d_X3A_X2502d_X3A_X2502d),
    /* K2   */  be_nested_str(year),
    /* K3   */  be_nested_str(month),
    /* K4   */  be_nested_str(day),
    /* K5   */  be_nested_str(hour),
    /* K6   */  be_nested_str(min),
    /* K7   */  be_nested_str(sec),
    }),
    &be_const_str_time_str,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x600C0018,  //  0003  GETGBL	R3	G24
      0x58100001,  //  0004  LDCONST	R4	K1
      0x94140502,  //  0005  GETIDX	R5	R2	K2
      0x94180503,  //  0006  GETIDX	R6	R2	K3
      0x941C0504,  //  0007  GETIDX	R7	R2	K4
      0x94200505,  //  0008  GETIDX	R8	R2	K5
      0x94240506,  //  0009  GETIDX	R9	R2	K6
      0x94280507,  //  000A  GETIDX	R10	R2	K7
      0x7C0C0E00,  //  000B  CALL	R3	7
      0x80040600,  //  000C  RET	1	R3
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(check_not_method),
    /* K1   */  be_nested_str(_rules),
    /* K2   */  be_nested_str(function),
    /* K3   */  be_nested_str(push),
    /* K4   */  be_nested_str(Trigger),
    /* K5   */  be_nested_str(Rule_Matcher),
    /* K6   */  be_nested_str(parse),
    /* K7   */  be_nested_str(value_error),
    /* K8   */  be_nested_str(the_X20second_X20argument_X20is_X20not_X20a_X20function),
    }),
    &be_const_str_add_rule,
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180400,  //  0001  MOVE	R6	R2
      0x7C100400,  //  0002  CALL	R4	2
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x4C140000,  //  0004  LDNIL	R5
      0x1C100805,  //  0005  EQ	R4	R4	R5
      0x78120002,  //  0006  JMPF	R4	#000A
      0x60100012,  //  0007  GETGBL	R4	G18
      0x7C100000,  //  0008  CALL	R4	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x60100004,  //  000A  GETGBL	R4	G4
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C100200,  //  000C  CALL	R4	1
      0x1C100902,  //  000D  EQ	R4	R4	K2
      0x7812000B,  //  000E  JMPF	R4	#001B
      0x88100101,  //  000F  GETMBR	R4	R0	K1
      0x8C100903,  //  0010  GETMET	R4	R4	K3
      0xB81A0800,  //  0011  GETNGBL	R6	K4
      0x881C0105,  //  0012  GETMBR	R7	R0	K5
      0x8C1C0F06,  //  0013  GETMET	R7	R7	K6
      0x5C240200,  //  0014  MOVE	R9	R1
      0x7C1C0400,  //  0015  CALL	R7	2
      0x5C200400,  //  0016  MOVE	R8	R2
      0x5C240600,  //  0017  MOVE	R9	R3
      0x7C180600,  //  0018  CALL	R6	3
      0x7C100400,  //  0019  CALL	R4	2
      0x70020000,  //  001A  JMP		#001C
      0xB0060F08,  //  001B  RAISE	1	K7	K8
      0x80000000,  //  001C  RET	0
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
** Solidified function: urlfetch_cmd
********************************************************************/
be_local_closure(Tasmota_urlfetch_cmd,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(find),
    /* K2   */  be_nested_str(http),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str(resp_cmnd_str),
    /* K5   */  be_nested_str(URL_X20must_X20start_X20with_X20_X27http_X28s_X29_X27),
    /* K6   */  be_nested_str(urlfetch),
    /* K7   */  be_nested_str(resp_cmnd_failed),
    /* K8   */  be_nested_str(tasmota),
    /* K9   */  be_nested_str(resp_cmnd_done),
    }),
    &be_const_str_urlfetch_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0x8C180B01,  //  0001  GETMET	R6	R5	K1
      0x5C200600,  //  0002  MOVE	R8	R3
      0x58240002,  //  0003  LDCONST	R9	K2
      0x7C180600,  //  0004  CALL	R6	3
      0x20180D03,  //  0005  NE	R6	R6	K3
      0x781A0003,  //  0006  JMPF	R6	#000B
      0x8C180104,  //  0007  GETMET	R6	R0	K4
      0x58200005,  //  0008  LDCONST	R8	K5
      0x7C180400,  //  0009  CALL	R6	2
      0x80000C00,  //  000A  RET	0
      0xA802000A,  //  000B  EXBLK	0	#0017
      0x8C180106,  //  000C  GETMET	R6	R0	K6
      0x5C200600,  //  000D  MOVE	R8	R3
      0x7C180400,  //  000E  CALL	R6	2
      0x141C0D03,  //  000F  LT	R7	R6	K3
      0x781E0003,  //  0010  JMPF	R7	#0015
      0x8C1C0107,  //  0011  GETMET	R7	R0	K7
      0x7C1C0200,  //  0012  CALL	R7	1
      0xA8040001,  //  0013  EXBLK	1	1
      0x80000E00,  //  0014  RET	0
      0xA8040001,  //  0015  EXBLK	1	1
      0x70020006,  //  0016  JMP		#001E
      0xAC180002,  //  0017  CATCH	R6	0	2
      0x70020003,  //  0018  JMP		#001D
      0x8C200107,  //  0019  GETMET	R8	R0	K7
      0x7C200200,  //  001A  CALL	R8	1
      0x80001000,  //  001B  RET	0
      0x70020000,  //  001C  JMP		#001E
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0xB81A1000,  //  001E  GETNGBL	R6	K8
      0x8C180D09,  //  001F  GETMET	R6	R6	K9
      0x7C180200,  //  0020  CALL	R6	1
      0x80000000,  //  0021  RET	0
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
    ( &(const binstruction[23]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0002,  //  0006  JMPF	R2	#000A
      0x60080012,  //  0007  GETGBL	R2	G18
      0x7C080000,  //  0008  CALL	R2	0
      0x90020202,  //  0009  SETMBR	R0	K1	R2
      0x60080004,  //  000A  GETGBL	R2	G4
      0x5C0C0200,  //  000B  MOVE	R3	R1
      0x7C080200,  //  000C  CALL	R2	1
      0x20080502,  //  000D  NE	R2	R2	K2
      0x780A0000,  //  000E  JMPF	R2	#0010
      0xB0060704,  //  000F  RAISE	1	K3	K4
      0x88080105,  //  0010  GETMBR	R2	R0	K5
      0x900A0D07,  //  0011  SETMBR	R2	K6	K7
      0x88080101,  //  0012  GETMBR	R2	R0	K1
      0x8C080508,  //  0013  GETMET	R2	R2	K8
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x80000000,  //  0016  RET	0
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
** Solidified class: Tasmota
********************************************************************/
be_local_class(Tasmota,
    13,
    NULL,
    be_nested_map(50,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(hs2rgb, 34), be_const_closure(Tasmota_hs2rgb_closure) },
        { be_const_key(gen_cb, 22), be_const_closure(Tasmota_gen_cb_closure) },
        { be_const_key(fast_loop, 44), be_const_closure(Tasmota_fast_loop_closure) },
        { be_const_key(remove_fast_loop, -1), be_const_closure(Tasmota_remove_fast_loop_closure) },
        { be_const_key(global, -1), be_const_var(9) },
        { be_const_key(try_rule, 17), be_const_closure(Tasmota_try_rule_closure) },
        { be_const_key(wd, -1), be_const_var(11) },
        { be_const_key(check_not_method, -1), be_const_closure(Tasmota_check_not_method_closure) },
        { be_const_key(run_deferred, -1), be_const_closure(Tasmota_run_deferred_closure) },
        { be_const_key(exec_cmd, -1), be_const_closure(Tasmota_exec_cmd_closure) },
        { be_const_key(urlfetch, -1), be_const_closure(Tasmota_urlfetch_closure) },
        { be_const_key(urlfetch_cmd, -1), be_const_closure(Tasmota_urlfetch_cmd_closure) },
        { be_const_key(remove_cron, 24), be_const_closure(Tasmota_remove_cron_closure) },
        { be_const_key(find_list_i, 23), be_const_closure(Tasmota_find_list_i_closure) },
        { be_const_key(gc, -1), be_const_closure(Tasmota_gc_closure) },
        { be_const_key(remove_timer, -1), be_const_closure(Tasmota_remove_timer_closure) },
        { be_const_key(_timers, -1), be_const_var(2) },
        { be_const_key(init, 48), be_const_closure(Tasmota_init_closure) },
        { be_const_key(remove_driver, -1), be_const_closure(Tasmota_remove_driver_closure) },
        { be_const_key(wire1, -1), be_const_var(6) },
        { be_const_key(_fl, -1), be_const_var(0) },
        { be_const_key(add_driver, 14), be_const_closure(Tasmota_add_driver_closure) },
        { be_const_key(exec_tele, -1), be_const_closure(Tasmota_exec_tele_closure) },
        { be_const_key(_ccmd, -1), be_const_var(4) },
        { be_const_key(time_str, 36), be_const_closure(Tasmota_time_str_closure) },
        { be_const_key(_debug_present, -1), be_const_var(12) },
        { be_const_key(find_key_i, 31), be_const_closure(Tasmota_find_key_i_closure) },
        { be_const_key(add_cron, -1), be_const_closure(Tasmota_add_cron_closure) },
        { be_const_key(remove_rule, 6), be_const_closure(Tasmota_remove_rule_closure) },
        { be_const_key(add_cmd, 39), be_const_closure(Tasmota_add_cmd_closure) },
        { be_const_key(wire_scan, -1), be_const_closure(Tasmota_wire_scan_closure) },
        { be_const_key(get_light, 40), be_const_closure(Tasmota_get_light_closure) },
        { be_const_key(cmd, 9), be_const_closure(Tasmota_cmd_closure) },
        { be_const_key(set_timer, 11), be_const_closure(Tasmota_set_timer_closure) },
        { be_const_key(load, 32), be_const_closure(Tasmota_load_closure) },
        { be_const_key(_drivers, -1), be_const_var(5) },
        { be_const_key(cmd_res, -1), be_const_var(8) },
        { be_const_key(next_cron, -1), be_const_closure(Tasmota_next_cron_closure) },
        { be_const_key(wire2, -1), be_const_var(7) },
        { be_const_key(_crons, -1), be_const_var(3) },
        { be_const_key(find_op, 41), be_const_closure(Tasmota_find_op_closure) },
        { be_const_key(settings, -1), be_const_var(10) },
        { be_const_key(exec_rules, 20), be_const_closure(Tasmota_exec_rules_closure) },
        { be_const_key(add_rule, -1), be_const_closure(Tasmota_add_rule_closure) },
        { be_const_key(remove_cmd, 45), be_const_closure(Tasmota_remove_cmd_closure) },
        { be_const_key(set_light, -1), be_const_closure(Tasmota_set_light_closure) },
        { be_const_key(add_fast_loop, 8), be_const_closure(Tasmota_add_fast_loop_closure) },
        { be_const_key(_rules, -1), be_const_var(1) },
        { be_const_key(run_cron, 47), be_const_closure(Tasmota_run_cron_closure) },
        { be_const_key(event, 0), be_const_closure(Tasmota_event_closure) },
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
