/* Solidification of animation_dsl.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'DSLRuntime' ktab size: 26, total: 45 (saved 152 bytes)
static const bvalue be_ktab_class_DSLRuntime[26] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_nested_str_weak(debug_mode),
  /* K2   */  be_nested_str_weak(DSL_X3A_X20Empty_X20source_X20code),
  /* K3   */  be_nested_str_weak(DSL_X3A_X20Compiling_X20source_X2E_X2E_X2E),
  /* K4   */  be_nested_str_weak(animation_dsl),
  /* K5   */  be_nested_str_weak(compile),
  /* K6   */  be_nested_str_weak(execute_berry_code),
  /* K7   */  be_nested_str_weak(dsl_compilation_error),
  /* K8   */  be_nested_str_weak(DSL_X3A_X20Compilation_X20failed_X20_X2D_X20),
  /* K9   */  be_nested_str_weak(active_source),
  /* K10  */  be_nested_str_weak(engine),
  /* K11  */  be_nested_str_weak(r),
  /* K12  */  be_nested_str_weak(DSL_X3A_X20Cannot_X20open_X20file_X20_X25s),
  /* K13  */  be_nested_str_weak(read),
  /* K14  */  be_nested_str_weak(close),
  /* K15  */  be_nested_str_weak(DSL_X3A_X20Loaded_X20_X25s_X20characters_X20from_X20_X25s),
  /* K16  */  be_nested_str_weak(load_dsl),
  /* K17  */  be_nested_str_weak(DSL_X3A_X20File_X20loading_X20error_X3A_X20_X25s),
  /* K18  */  be_nested_str_weak(DSL_X3A_X20Code_X20generation_X20failed_X20_X2D_X20),
  /* K19  */  be_nested_str_weak(DSL_X3A_X20No_X20active_X20DSL_X20to_X20reload),
  /* K20  */  be_nested_str_weak(DSL_X3A_X20Reloading_X20current_X20DSL_X2E_X2E_X2E),
  /* K21  */  be_nested_str_weak(stop),
  /* K22  */  be_nested_str_weak(clear),
  /* K23  */  be_nested_str_weak(DSL_X3A_X20Berry_X20compilation_X20failed),
  /* K24  */  be_nested_str_weak(DSL_X3A_X20Execution_X20successful),
  /* K25  */  be_nested_str_weak(DSL_X3A_X20Execution_X20error_X3A_X20_X25s),
};


extern const bclass be_class_DSLRuntime;

/********************************************************************
** Solidified function: load_dsl
********************************************************************/
be_local_closure(class_DSLRuntime_load_dsl,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(load_dsl),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x5C0C0200,  //  0004  MOVE	R3	R1
      0x7C080200,  //  0005  CALL	R2	1
      0x1C080500,  //  0006  EQ	R2	R2	K0
      0x780A0006,  //  0007  JMPF	R2	#000F
      0x88080101,  //  0008  GETMBR	R2	R0	K1
      0x780A0002,  //  0009  JMPF	R2	#000D
      0x60080001,  //  000A  GETGBL	R2	G1
      0x580C0002,  //  000B  LDCONST	R3	K2
      0x7C080200,  //  000C  CALL	R2	1
      0x50080000,  //  000D  LDBOOL	R2	0	0
      0x80040400,  //  000E  RET	1	R2
      0x88080101,  //  000F  GETMBR	R2	R0	K1
      0x780A0002,  //  0010  JMPF	R2	#0014
      0x60080001,  //  0011  GETGBL	R2	G1
      0x580C0003,  //  0012  LDCONST	R3	K3
      0x7C080200,  //  0013  CALL	R2	1
      0xA802000B,  //  0014  EXBLK	0	#0021
      0xB80A0800,  //  0015  GETNGBL	R2	K4
      0x8C080505,  //  0016  GETMET	R2	R2	K5
      0x5C100200,  //  0017  MOVE	R4	R1
      0x7C080400,  //  0018  CALL	R2	2
      0x8C0C0106,  //  0019  GETMET	R3	R0	K6
      0x5C140400,  //  001A  MOVE	R5	R2
      0x5C180200,  //  001B  MOVE	R6	R1
      0x7C0C0600,  //  001C  CALL	R3	3
      0xA8040001,  //  001D  EXBLK	1	1
      0x80040600,  //  001E  RET	1	R3
      0xA8040001,  //  001F  EXBLK	1	1
      0x7002000B,  //  0020  JMP		#002D
      0x58080007,  //  0021  LDCONST	R2	K7
      0xAC080202,  //  0022  CATCH	R2	1	2
      0x70020007,  //  0023  JMP		#002C
      0x88100101,  //  0024  GETMBR	R4	R0	K1
      0x78120002,  //  0025  JMPF	R4	#0029
      0x60100001,  //  0026  GETGBL	R4	G1
      0x00161003,  //  0027  ADD	R5	K8	R3
      0x7C100200,  //  0028  CALL	R4	1
      0x50100000,  //  0029  LDBOOL	R4	0	0
      0x80040800,  //  002A  RET	1	R4
      0x70020000,  //  002B  JMP		#002D
      0xB0080000,  //  002C  RAISE	2	R0	R0
      0x80000000,  //  002D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active_source
********************************************************************/
be_local_closure(class_DSLRuntime_get_active_source,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(get_active_source),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_DSLRuntime_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x90021401,  //  0000  SETMBR	R0	K10	R1
      0x4C0C0000,  //  0001  LDNIL	R3
      0x90021203,  //  0002  SETMBR	R0	K9	R3
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x70020000,  //  0007  JMP		#0009
      0x500C0000,  //  0008  LDBOOL	R3	0	0
      0x90020203,  //  0009  SETMBR	R0	K1	R3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_dsl_file
********************************************************************/
be_local_closure(class_DSLRuntime_load_dsl_file,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(load_dsl_file),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0xA8020027,  //  0000  EXBLK	0	#0029
      0x60080011,  //  0001  GETGBL	R2	G17
      0x5C0C0200,  //  0002  MOVE	R3	R1
      0x5810000B,  //  0003  LDCONST	R4	K11
      0x7C080400,  //  0004  CALL	R2	2
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C0C0403,  //  0006  EQ	R3	R2	R3
      0x780E000A,  //  0007  JMPF	R3	#0013
      0x880C0101,  //  0008  GETMBR	R3	R0	K1
      0x780E0005,  //  0009  JMPF	R3	#0010
      0x600C0001,  //  000A  GETGBL	R3	G1
      0x60100018,  //  000B  GETGBL	R4	G24
      0x5814000C,  //  000C  LDCONST	R5	K12
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0x7C0C0200,  //  000F  CALL	R3	1
      0x500C0000,  //  0010  LDBOOL	R3	0	0
      0xA8040001,  //  0011  EXBLK	1	1
      0x80040600,  //  0012  RET	1	R3
      0x8C0C050D,  //  0013  GETMET	R3	R2	K13
      0x7C0C0200,  //  0014  CALL	R3	1
      0x8C10050E,  //  0015  GETMET	R4	R2	K14
      0x7C100200,  //  0016  CALL	R4	1
      0x88100101,  //  0017  GETMBR	R4	R0	K1
      0x78120008,  //  0018  JMPF	R4	#0022
      0x60100001,  //  0019  GETGBL	R4	G1
      0x60140018,  //  001A  GETGBL	R5	G24
      0x5818000F,  //  001B  LDCONST	R6	K15
      0x601C000C,  //  001C  GETGBL	R7	G12
      0x5C200600,  //  001D  MOVE	R8	R3
      0x7C1C0200,  //  001E  CALL	R7	1
      0x5C200200,  //  001F  MOVE	R8	R1
      0x7C140600,  //  0020  CALL	R5	3
      0x7C100200,  //  0021  CALL	R4	1
      0x8C100110,  //  0022  GETMET	R4	R0	K16
      0x5C180600,  //  0023  MOVE	R6	R3
      0x7C100400,  //  0024  CALL	R4	2
      0xA8040001,  //  0025  EXBLK	1	1
      0x80040800,  //  0026  RET	1	R4
      0xA8040001,  //  0027  EXBLK	1	1
      0x7002000D,  //  0028  JMP		#0037
      0xAC080002,  //  0029  CATCH	R2	0	2
      0x7002000A,  //  002A  JMP		#0036
      0x88100101,  //  002B  GETMBR	R4	R0	K1
      0x78120005,  //  002C  JMPF	R4	#0033
      0x60100001,  //  002D  GETGBL	R4	G1
      0x60140018,  //  002E  GETGBL	R5	G24
      0x58180011,  //  002F  LDCONST	R6	K17
      0x5C1C0600,  //  0030  MOVE	R7	R3
      0x7C140400,  //  0031  CALL	R5	2
      0x7C100200,  //  0032  CALL	R4	1
      0x50100000,  //  0033  LDBOOL	R4	0	0
      0x80040800,  //  0034  RET	1	R4
      0x70020000,  //  0035  JMP		#0037
      0xB0080000,  //  0036  RAISE	2	R0	R0
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_engine
********************************************************************/
be_local_closure(class_DSLRuntime_get_engine,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(get_engine),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010A,  //  0000  GETMBR	R1	R0	K10
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_generated_code
********************************************************************/
be_local_closure(class_DSLRuntime_get_generated_code,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(get_generated_code),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x88040109,  //  0003  GETMBR	R1	R0	K9
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x780A0001,  //  0006  JMPF	R2	#0009
      0x4C080000,  //  0007  LDNIL	R2
      0x80040400,  //  0008  RET	1	R2
      0xA8020007,  //  0009  EXBLK	0	#0012
      0xB80A0800,  //  000A  GETNGBL	R2	K4
      0x8C080505,  //  000B  GETMET	R2	R2	K5
      0x5C100200,  //  000C  MOVE	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0xA8040001,  //  000E  EXBLK	1	1
      0x80040400,  //  000F  RET	1	R2
      0xA8040001,  //  0010  EXBLK	1	1
      0x7002000B,  //  0011  JMP		#001E
      0x58080007,  //  0012  LDCONST	R2	K7
      0xAC080202,  //  0013  CATCH	R2	1	2
      0x70020007,  //  0014  JMP		#001D
      0x88100101,  //  0015  GETMBR	R4	R0	K1
      0x78120002,  //  0016  JMPF	R4	#001A
      0x60100001,  //  0017  GETGBL	R4	G1
      0x00162403,  //  0018  ADD	R5	K18	R3
      0x7C100200,  //  0019  CALL	R4	1
      0x4C100000,  //  001A  LDNIL	R4
      0x80040800,  //  001B  RET	1	R4
      0x70020000,  //  001C  JMP		#001E
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reload_dsl
********************************************************************/
be_local_closure(class_DSLRuntime_reload_dsl,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(reload_dsl),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060006,  //  0003  JMPF	R1	#000B
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x78060002,  //  0005  JMPF	R1	#0009
      0x60040001,  //  0006  GETGBL	R1	G1
      0x58080013,  //  0007  LDCONST	R2	K19
      0x7C040200,  //  0008  CALL	R1	1
      0x50040000,  //  0009  LDBOOL	R1	0	0
      0x80040200,  //  000A  RET	1	R1
      0x88040101,  //  000B  GETMBR	R1	R0	K1
      0x78060002,  //  000C  JMPF	R1	#0010
      0x60040001,  //  000D  GETGBL	R1	G1
      0x58080014,  //  000E  LDCONST	R2	K20
      0x7C040200,  //  000F  CALL	R1	1
      0x8804010A,  //  0010  GETMBR	R1	R0	K10
      0x8C040315,  //  0011  GETMET	R1	R1	K21
      0x7C040200,  //  0012  CALL	R1	1
      0x8804010A,  //  0013  GETMBR	R1	R0	K10
      0x8C040316,  //  0014  GETMET	R1	R1	K22
      0x7C040200,  //  0015  CALL	R1	1
      0x8C040110,  //  0016  GETMET	R1	R0	K16
      0x880C0109,  //  0017  GETMBR	R3	R0	K9
      0x7C040400,  //  0018  CALL	R1	2
      0x80040200,  //  0019  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: execute_berry_code
********************************************************************/
be_local_closure(class_DSLRuntime_execute_berry_code,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(execute_berry_code),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0xA8020020,  //  0000  EXBLK	0	#0022
      0x880C010A,  //  0001  GETMBR	R3	R0	K10
      0x8C0C0715,  //  0002  GETMET	R3	R3	K21
      0x7C0C0200,  //  0003  CALL	R3	1
      0x880C010A,  //  0004  GETMBR	R3	R0	K10
      0x8C0C0716,  //  0005  GETMET	R3	R3	K22
      0x7C0C0200,  //  0006  CALL	R3	1
      0x600C000D,  //  0007  GETGBL	R3	G13
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x4C100000,  //  000A  LDNIL	R4
      0x1C100604,  //  000B  EQ	R4	R3	R4
      0x78120007,  //  000C  JMPF	R4	#0015
      0x88100101,  //  000D  GETMBR	R4	R0	K1
      0x78120002,  //  000E  JMPF	R4	#0012
      0x60100001,  //  000F  GETGBL	R4	G1
      0x58140017,  //  0010  LDCONST	R5	K23
      0x7C100200,  //  0011  CALL	R4	1
      0x50100000,  //  0012  LDBOOL	R4	0	0
      0xA8040001,  //  0013  EXBLK	1	1
      0x80040800,  //  0014  RET	1	R4
      0x5C100600,  //  0015  MOVE	R4	R3
      0x7C100000,  //  0016  CALL	R4	0
      0x90021202,  //  0017  SETMBR	R0	K9	R2
      0x88100101,  //  0018  GETMBR	R4	R0	K1
      0x78120002,  //  0019  JMPF	R4	#001D
      0x60100001,  //  001A  GETGBL	R4	G1
      0x58140018,  //  001B  LDCONST	R5	K24
      0x7C100200,  //  001C  CALL	R4	1
      0x50100200,  //  001D  LDBOOL	R4	1	0
      0xA8040001,  //  001E  EXBLK	1	1
      0x80040800,  //  001F  RET	1	R4
      0xA8040001,  //  0020  EXBLK	1	1
      0x7002000D,  //  0021  JMP		#0030
      0xAC0C0002,  //  0022  CATCH	R3	0	2
      0x7002000A,  //  0023  JMP		#002F
      0x88140101,  //  0024  GETMBR	R5	R0	K1
      0x78160005,  //  0025  JMPF	R5	#002C
      0x60140001,  //  0026  GETGBL	R5	G1
      0x60180018,  //  0027  GETGBL	R6	G24
      0x581C0019,  //  0028  LDCONST	R7	K25
      0x5C200800,  //  0029  MOVE	R8	R4
      0x7C180400,  //  002A  CALL	R6	2
      0x7C140200,  //  002B  CALL	R5	1
      0x50140000,  //  002C  LDBOOL	R5	0	0
      0x80040A00,  //  002D  RET	1	R5
      0x70020000,  //  002E  JMP		#0030
      0xB0080000,  //  002F  RAISE	2	R0	R0
      0x80000000,  //  0030  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_loaded
********************************************************************/
be_local_closure(class_DSLRuntime_is_loaded,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLRuntime,     /* shared constants */
    be_str_weak(is_loaded),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: DSLRuntime
********************************************************************/
be_local_class(DSLRuntime,
    3,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(is_loaded, -1), be_const_closure(class_DSLRuntime_is_loaded_closure) },
        { be_const_key_weak(load_dsl, -1), be_const_closure(class_DSLRuntime_load_dsl_closure) },
        { be_const_key_weak(get_active_source, -1), be_const_closure(class_DSLRuntime_get_active_source_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_DSLRuntime_init_closure) },
        { be_const_key_weak(load_dsl_file, -1), be_const_closure(class_DSLRuntime_load_dsl_file_closure) },
        { be_const_key_weak(active_source, -1), be_const_var(1) },
        { be_const_key_weak(get_engine, -1), be_const_closure(class_DSLRuntime_get_engine_closure) },
        { be_const_key_weak(get_generated_code, -1), be_const_closure(class_DSLRuntime_get_generated_code_closure) },
        { be_const_key_weak(reload_dsl, -1), be_const_closure(class_DSLRuntime_reload_dsl_closure) },
        { be_const_key_weak(execute_berry_code, -1), be_const_closure(class_DSLRuntime_execute_berry_code_closure) },
        { be_const_key_weak(debug_mode, -1), be_const_var(2) },
        { be_const_key_weak(engine, 0), be_const_var(0) },
    })),
    be_str_weak(DSLRuntime)
);

/********************************************************************
** Solidified function: create_runtime
********************************************************************/
be_local_closure(create_runtime,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(create_engine),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(DSLRuntime),
    }),
    be_str_weak(create_runtime),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0xB80E0400,  //  0004  GETNGBL	R3	K2
      0x8C0C0703,  //  0005  GETMET	R3	R3	K3
      0x5C140400,  //  0006  MOVE	R5	R2
      0x5C180200,  //  0007  MOVE	R6	R1
      0x7C0C0600,  //  0008  CALL	R3	3
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_keyword
********************************************************************/
be_local_closure(is_keyword,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    /* K2   */  be_nested_str_weak(keywords),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(is_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0xB80A0000,  //  0001  GETNGBL	R2	K0
      0x88080501,  //  0002  GETMBR	R2	R2	K1
      0x88080502,  //  0003  GETMBR	R2	R2	K2
      0x7C040200,  //  0004  CALL	R1	1
      0xA8020007,  //  0005  EXBLK	0	#000E
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x1C0C0002,  //  0008  EQ	R3	R0	R2
      0x780E0002,  //  0009  JMPF	R3	#000D
      0x500C0200,  //  000A  LDBOOL	R3	1	0
      0xA8040001,  //  000B  EXBLK	1	1
      0x80040600,  //  000C  RET	1	R3
      0x7001FFF7,  //  000D  JMP		#0006
      0x58040003,  //  000E  LDCONST	R1	K3
      0xAC040200,  //  000F  CATCH	R1	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x50040000,  //  0011  LDBOOL	R1	0	0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compile_dsl_source
********************************************************************/
be_local_closure(compile_dsl_source,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(compile_dsl),
    }),
    be_str_weak(compile_dsl_source),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_newline_token
********************************************************************/
be_local_closure(create_newline_token,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    /* K2   */  be_nested_str_weak(_X0A),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(create_newline_token),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x54120022,  //  0002  LDINT	R4	35
      0x58140002,  //  0003  LDCONST	R5	K2
      0x5C180000,  //  0004  MOVE	R6	R0
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x58200003,  //  0006  LDCONST	R8	K3
      0x7C080C00,  //  0007  CALL	R2	6
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compile_dsl
********************************************************************/
be_local_closure(compile_dsl,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(DSLLexer),
    /* K2   */  be_nested_str_weak(tokenize),
    /* K3   */  be_nested_str_weak(has_errors),
    /* K4   */  be_nested_str_weak(DSL_X20Lexer_X20errors_X3A_X0A),
    /* K5   */  be_nested_str_weak(get_errors),
    /* K6   */  be_nested_str_weak(_X20_X20),
    /* K7   */  be_nested_str_weak(_X0A),
    /* K8   */  be_nested_str_weak(stop_iteration),
    /* K9   */  be_nested_str_weak(dsl_compilation_error),
    /* K10  */  be_nested_str_weak(SimpleDSLTranspiler),
    /* K11  */  be_nested_str_weak(transpile),
    /* K12  */  be_nested_str_weak(DSL_X20Transpiler_X20errors_X3A_X0A),
    }),
    be_str_weak(compile_dsl),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x8C080302,  //  0004  GETMET	R2	R1	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x8C0C0303,  //  0006  GETMET	R3	R1	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x780E000F,  //  0008  JMPF	R3	#0019
      0x580C0004,  //  0009  LDCONST	R3	K4
      0x60100010,  //  000A  GETGBL	R4	G16
      0x8C140305,  //  000B  GETMET	R5	R1	K5
      0x7C140200,  //  000C  CALL	R5	1
      0x7C100200,  //  000D  CALL	R4	1
      0xA8020005,  //  000E  EXBLK	0	#0015
      0x5C140800,  //  000F  MOVE	R5	R4
      0x7C140000,  //  0010  CALL	R5	0
      0x001A0C05,  //  0011  ADD	R6	K6	R5
      0x00180D07,  //  0012  ADD	R6	R6	K7
      0x000C0606,  //  0013  ADD	R3	R3	R6
      0x7001FFF9,  //  0014  JMP		#000F
      0x58100008,  //  0015  LDCONST	R4	K8
      0xAC100200,  //  0016  CATCH	R4	1	0
      0xB0080000,  //  0017  RAISE	2	R0	R0
      0xB0061203,  //  0018  RAISE	1	K9	R3
      0xB80E0000,  //  0019  GETNGBL	R3	K0
      0x8C0C070A,  //  001A  GETMET	R3	R3	K10
      0x5C140400,  //  001B  MOVE	R5	R2
      0x7C0C0400,  //  001C  CALL	R3	2
      0x8C10070B,  //  001D  GETMET	R4	R3	K11
      0x7C100200,  //  001E  CALL	R4	1
      0x8C140703,  //  001F  GETMET	R5	R3	K3
      0x7C140200,  //  0020  CALL	R5	1
      0x7816000F,  //  0021  JMPF	R5	#0032
      0x5814000C,  //  0022  LDCONST	R5	K12
      0x60180010,  //  0023  GETGBL	R6	G16
      0x8C1C0705,  //  0024  GETMET	R7	R3	K5
      0x7C1C0200,  //  0025  CALL	R7	1
      0x7C180200,  //  0026  CALL	R6	1
      0xA8020005,  //  0027  EXBLK	0	#002E
      0x5C1C0C00,  //  0028  MOVE	R7	R6
      0x7C1C0000,  //  0029  CALL	R7	0
      0x00220C07,  //  002A  ADD	R8	K6	R7
      0x00201107,  //  002B  ADD	R8	R8	K7
      0x00140A08,  //  002C  ADD	R5	R5	R8
      0x7001FFF9,  //  002D  JMP		#0028
      0x58180008,  //  002E  LDCONST	R6	K8
      0xAC180200,  //  002F  CATCH	R6	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0xB0061205,  //  0031  RAISE	1	K9	R5
      0x80040800,  //  0032  RET	1	R4
    })
  )
);
/*******************************************************************/

extern const bclass be_class_Token;
// compact class 'Token' ktab size: 52, total: 108 (saved 448 bytes)
static const bvalue be_ktab_class_Token[52] = {
  /* K0   */  be_nested_str_weak(type),
  /* K1   */  be_nested_str_weak(animation_dsl),
  /* K2   */  be_nested_str_weak(Token),
  /* K3   */  be_nested_str_weak(value),
  /* K4   */  be_nested_str_weak(line),
  /* K5   */  be_nested_str_weak(column),
  /* K6   */  be_nested_str_weak(length),
  /* K7   */  be_const_int(1),
  /* K8   */  be_nested_str_weak(),
  /* K9   */  be_nested_str_weak(string),
  /* K10  */  be_nested_str_weak(math),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str_weak(round),
  /* K13  */  be_nested_str_weak(endswith),
  /* K14  */  be_nested_str_weak(ms),
  /* K15  */  be_const_int(0),
  /* K16  */  be_nested_str_weak(s),
  /* K17  */  be_nested_str_weak(m),
  /* K18  */  be_nested_str_weak(h),
  /* K19  */  be_const_int(3600000),
  /* K20  */  be_nested_str_weak(tasmota),
  /* K21  */  be_nested_str_weak(scale_uint),
  /* K22  */  be_nested_str_weak(is_literal),
  /* K23  */  be_const_class(be_class_Token),
  /* K24  */  be_nested_str_weak(names),
  /* K25  */  be_nested_str_weak(UNKNOWN),
  /* K26  */  be_nested_str_weak(end_X20of_X20file),
  /* K27  */  be_nested_str_weak(newline),
  /* K28  */  be_nested_str_weak(keyword_X20_X27_X25s_X27),
  /* K29  */  be_nested_str_weak(identifier_X20_X27_X25s_X27),
  /* K30  */  be_const_int(3),
  /* K31  */  be_nested_str_weak(string_X20_X27_X25s_X27),
  /* K32  */  be_nested_str_weak(number_X20_X27_X25s_X27),
  /* K33  */  be_nested_str_weak(color_X20_X27_X25s_X27),
  /* K34  */  be_nested_str_weak(time_X20_X27_X25s_X27),
  /* K35  */  be_nested_str_weak(percentage_X20_X27_X25s_X27),
  /* K36  */  be_nested_str_weak(invalid_X20token_X20_X27_X25s_X27),
  /* K37  */  be_nested_str_weak(_X27_X25s_X27),
  /* K38  */  be_nested_str_weak(is_boolean),
  /* K39  */  be_nested_str_weak(true),
  /* K40  */  be_nested_str_weak(statement_keywords),
  /* K41  */  be_nested_str_weak(stop_iteration),
  /* K42  */  be_nested_str_weak(introspect),
  /* K43  */  be_nested_str_weak(global),
  /* K44  */  be_nested_str_weak(animation),
  /* K45  */  be_nested_str_weak(members),
  /* K46  */  be_nested_str_weak(find),
  /* K47  */  be_nested_str_weak(false),
  /* K48  */  be_nested_str_weak(to_string),
  /* K49  */  be_nested_str_weak(Token_X28_X25s_X20at_X20_X25s_X3A_X25s_X29),
  /* K50  */  be_nested_str_weak(_X2E_X2E_X2E),
  /* K51  */  be_nested_str_weak(Token_X28_X25s_X2C_X20_X27_X25s_X27_X20at_X20_X25s_X3A_X25s_X29),
};


extern const bclass be_class_Token;

/********************************************************************
** Solidified function: is_type
********************************************************************/
be_local_closure(class_Token_is_type,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_type),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x1C080401,  //  0001  EQ	R2	R2	R1
      0x80040400,  //  0002  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: with_type
********************************************************************/
be_local_closure(class_Token_with_type,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(with_type),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB80A0200,  //  0000  GETNGBL	R2	K1
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x5C100200,  //  0002  MOVE	R4	R1
      0x88140103,  //  0003  GETMBR	R5	R0	K3
      0x88180104,  //  0004  GETMBR	R6	R0	K4
      0x881C0105,  //  0005  GETMBR	R7	R0	K5
      0x88200106,  //  0006  GETMBR	R8	R0	K6
      0x7C080C00,  //  0007  CALL	R2	6
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: end_column
********************************************************************/
be_local_closure(class_Token_end_column,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(end_column),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x88080106,  //  0001  GETMBR	R2	R0	K6
      0x00040202,  //  0002  ADD	R1	R1	R2
      0x04040307,  //  0003  SUB	R1	R1	K7
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_operator
********************************************************************/
be_local_closure(class_Token_is_operator,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_operator),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A0007,  //  0001  LDINT	R2	8
      0x28040202,  //  0002  GE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x540A0016,  //  0005  LDINT	R2	23
      0x18040202,  //  0006  LE	R1	R1	R2
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Token_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x4C180000,  //  0001  LDNIL	R6
      0x20180406,  //  0002  NE	R6	R2	R6
      0x781A0001,  //  0003  JMPF	R6	#0006
      0x5C180400,  //  0004  MOVE	R6	R2
      0x70020000,  //  0005  JMP		#0007
      0x58180008,  //  0006  LDCONST	R6	K8
      0x90020606,  //  0007  SETMBR	R0	K3	R6
      0x4C180000,  //  0008  LDNIL	R6
      0x20180606,  //  0009  NE	R6	R3	R6
      0x781A0001,  //  000A  JMPF	R6	#000D
      0x5C180600,  //  000B  MOVE	R6	R3
      0x70020000,  //  000C  JMP		#000E
      0x58180007,  //  000D  LDCONST	R6	K7
      0x90020806,  //  000E  SETMBR	R0	K4	R6
      0x4C180000,  //  000F  LDNIL	R6
      0x20180806,  //  0010  NE	R6	R4	R6
      0x781A0001,  //  0011  JMPF	R6	#0014
      0x5C180800,  //  0012  MOVE	R6	R4
      0x70020000,  //  0013  JMP		#0015
      0x58180007,  //  0014  LDCONST	R6	K7
      0x90020A06,  //  0015  SETMBR	R0	K5	R6
      0x4C180000,  //  0016  LDNIL	R6
      0x20180A06,  //  0017  NE	R6	R5	R6
      0x781A0001,  //  0018  JMPF	R6	#001B
      0x5C180A00,  //  0019  MOVE	R6	R5
      0x70020002,  //  001A  JMP		#001E
      0x6018000C,  //  001B  GETGBL	R6	G12
      0x881C0103,  //  001C  GETMBR	R7	R0	K3
      0x7C180200,  //  001D  CALL	R6	1
      0x90020C06,  //  001E  SETMBR	R0	K6	R6
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_delimiter
********************************************************************/
be_local_closure(class_Token_is_delimiter,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_delimiter),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A0017,  //  0001  LDINT	R2	24
      0x28040202,  //  0002  GE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x540A001C,  //  0005  LDINT	R2	29
      0x18040202,  //  0006  LE	R1	R1	R2
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_numeric_value
********************************************************************/
be_local_closure(class_Token_get_numeric_value,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(get_numeric_value),
    &be_const_str_solidified,
    ( &(const binstruction[117]) {  /* code */
      0xA4061200,  //  0000  IMPORT	R1	K9
      0xA40A1400,  //  0001  IMPORT	R2	K10
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x1C0C070B,  //  0003  EQ	R3	R3	K11
      0x780E0006,  //  0004  JMPF	R3	#000C
      0x8C0C050C,  //  0005  GETMET	R3	R2	K12
      0x6014000A,  //  0006  GETGBL	R5	G10
      0x88180103,  //  0007  GETMBR	R6	R0	K3
      0x7C140200,  //  0008  CALL	R5	1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x80040600,  //  000A  RET	1	R3
      0x70020066,  //  000B  JMP		#0073
      0x880C0100,  //  000C  GETMBR	R3	R0	K0
      0x54120004,  //  000D  LDINT	R4	5
      0x1C0C0604,  //  000E  EQ	R3	R3	R4
      0x780E003D,  //  000F  JMPF	R3	#004E
      0x880C0103,  //  0010  GETMBR	R3	R0	K3
      0x8C10030D,  //  0011  GETMET	R4	R1	K13
      0x5C180600,  //  0012  MOVE	R6	R3
      0x581C000E,  //  0013  LDCONST	R7	K14
      0x7C100600,  //  0014  CALL	R4	3
      0x78120008,  //  0015  JMPF	R4	#001F
      0x8C10050C,  //  0016  GETMET	R4	R2	K12
      0x6018000A,  //  0017  GETGBL	R6	G10
      0x541DFFFC,  //  0018  LDINT	R7	-3
      0x401E1E07,  //  0019  CONNECT	R7	K15	R7
      0x941C0607,  //  001A  GETIDX	R7	R3	R7
      0x7C180200,  //  001B  CALL	R6	1
      0x7C100400,  //  001C  CALL	R4	2
      0x80040800,  //  001D  RET	1	R4
      0x7002002D,  //  001E  JMP		#004D
      0x8C10030D,  //  001F  GETMET	R4	R1	K13
      0x5C180600,  //  0020  MOVE	R6	R3
      0x581C0010,  //  0021  LDCONST	R7	K16
      0x7C100600,  //  0022  CALL	R4	3
      0x7812000A,  //  0023  JMPF	R4	#002F
      0x8C10050C,  //  0024  GETMET	R4	R2	K12
      0x6018000A,  //  0025  GETGBL	R6	G10
      0x541DFFFD,  //  0026  LDINT	R7	-2
      0x401E1E07,  //  0027  CONNECT	R7	K15	R7
      0x941C0607,  //  0028  GETIDX	R7	R3	R7
      0x7C180200,  //  0029  CALL	R6	1
      0x541E03E7,  //  002A  LDINT	R7	1000
      0x08180C07,  //  002B  MUL	R6	R6	R7
      0x7C100400,  //  002C  CALL	R4	2
      0x80040800,  //  002D  RET	1	R4
      0x7002001D,  //  002E  JMP		#004D
      0x8C10030D,  //  002F  GETMET	R4	R1	K13
      0x5C180600,  //  0030  MOVE	R6	R3
      0x581C0011,  //  0031  LDCONST	R7	K17
      0x7C100600,  //  0032  CALL	R4	3
      0x7812000A,  //  0033  JMPF	R4	#003F
      0x8C10050C,  //  0034  GETMET	R4	R2	K12
      0x6018000A,  //  0035  GETGBL	R6	G10
      0x541DFFFD,  //  0036  LDINT	R7	-2
      0x401E1E07,  //  0037  CONNECT	R7	K15	R7
      0x941C0607,  //  0038  GETIDX	R7	R3	R7
      0x7C180200,  //  0039  CALL	R6	1
      0x541EEA5F,  //  003A  LDINT	R7	60000
      0x08180C07,  //  003B  MUL	R6	R6	R7
      0x7C100400,  //  003C  CALL	R4	2
      0x80040800,  //  003D  RET	1	R4
      0x7002000D,  //  003E  JMP		#004D
      0x8C10030D,  //  003F  GETMET	R4	R1	K13
      0x5C180600,  //  0040  MOVE	R6	R3
      0x581C0012,  //  0041  LDCONST	R7	K18
      0x7C100600,  //  0042  CALL	R4	3
      0x78120008,  //  0043  JMPF	R4	#004D
      0x8C10050C,  //  0044  GETMET	R4	R2	K12
      0x6018000A,  //  0045  GETGBL	R6	G10
      0x541DFFFD,  //  0046  LDINT	R7	-2
      0x401E1E07,  //  0047  CONNECT	R7	K15	R7
      0x941C0607,  //  0048  GETIDX	R7	R3	R7
      0x7C180200,  //  0049  CALL	R6	1
      0x08180D13,  //  004A  MUL	R6	R6	K19
      0x7C100400,  //  004B  CALL	R4	2
      0x80040800,  //  004C  RET	1	R4
      0x70020024,  //  004D  JMP		#0073
      0x880C0100,  //  004E  GETMBR	R3	R0	K0
      0x54120005,  //  004F  LDINT	R4	6
      0x1C0C0604,  //  0050  EQ	R3	R3	R4
      0x780E0011,  //  0051  JMPF	R3	#0064
      0x8C0C050C,  //  0052  GETMET	R3	R2	K12
      0x6014000A,  //  0053  GETGBL	R5	G10
      0x5419FFFD,  //  0054  LDINT	R6	-2
      0x401A1E06,  //  0055  CONNECT	R6	K15	R6
      0x881C0103,  //  0056  GETMBR	R7	R0	K3
      0x94180E06,  //  0057  GETIDX	R6	R7	R6
      0x7C140200,  //  0058  CALL	R5	1
      0x7C0C0400,  //  0059  CALL	R3	2
      0xB8122800,  //  005A  GETNGBL	R4	K20
      0x8C100915,  //  005B  GETMET	R4	R4	K21
      0x5C180600,  //  005C  MOVE	R6	R3
      0x581C000F,  //  005D  LDCONST	R7	K15
      0x54220063,  //  005E  LDINT	R8	100
      0x5824000F,  //  005F  LDCONST	R9	K15
      0x542A00FE,  //  0060  LDINT	R10	255
      0x7C100C00,  //  0061  CALL	R4	6
      0x80040800,  //  0062  RET	1	R4
      0x7002000E,  //  0063  JMP		#0073
      0x880C0100,  //  0064  GETMBR	R3	R0	K0
      0x54120006,  //  0065  LDINT	R4	7
      0x1C0C0604,  //  0066  EQ	R3	R3	R4
      0x780E000A,  //  0067  JMPF	R3	#0073
      0x600C000A,  //  0068  GETGBL	R3	G10
      0x5411FFFD,  //  0069  LDINT	R4	-2
      0x40121E04,  //  006A  CONNECT	R4	K15	R4
      0x88140103,  //  006B  GETMBR	R5	R0	K3
      0x94100A04,  //  006C  GETIDX	R4	R5	R4
      0x7C0C0200,  //  006D  CALL	R3	1
      0x8C10050C,  //  006E  GETMET	R4	R2	K12
      0x541A00FF,  //  006F  LDINT	R6	256
      0x08180606,  //  0070  MUL	R6	R3	R6
      0x7C100400,  //  0071  CALL	R4	2
      0x80040800,  //  0072  RET	1	R4
      0x4C0C0000,  //  0073  LDNIL	R3
      0x80040600,  //  0074  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_end_expression
********************************************************************/
be_local_closure(class_Token_can_end_expression,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(can_end_expression),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C040116,  //  0000  GETMET	R1	R0	K22
      0x7C040200,  //  0001  CALL	R1	1
      0x7406000B,  //  0002  JMPT	R1	#000F
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x1C040307,  //  0004  EQ	R1	R1	K7
      0x74060008,  //  0005  JMPT	R1	#000F
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0023,  //  0007  LDINT	R2	36
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x74060004,  //  0009  JMPT	R1	#000F
      0x88040100,  //  000A  GETMBR	R1	R0	K0
      0x540A0018,  //  000B  LDINT	R2	25
      0x1C040202,  //  000C  EQ	R1	R1	R2
      0x74060000,  //  000D  JMPT	R1	#000F
      0x50040001,  //  000E  LDBOOL	R1	0	1
      0x50040200,  //  000F  LDBOOL	R1	1	0
      0x80040200,  //  0010  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_string
********************************************************************/
be_local_closure(class_Token_to_string,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(to_string),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x58040017,  //  0000  LDCONST	R1	K23
      0x2808010F,  //  0001  GE	R2	R0	K15
      0x780A0007,  //  0002  JMPF	R2	#000B
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x880C0318,  //  0004  GETMBR	R3	R1	K24
      0x7C080200,  //  0005  CALL	R2	1
      0x14080002,  //  0006  LT	R2	R0	R2
      0x780A0002,  //  0007  JMPF	R2	#000B
      0x88080318,  //  0008  GETMBR	R2	R1	K24
      0x94080400,  //  0009  GETIDX	R2	R2	R0
      0x80040400,  //  000A  RET	1	R2
      0x80063200,  //  000B  RET	1	K25
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_error_string
********************************************************************/
be_local_closure(class_Token_to_error_string,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(to_error_string),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A0025,  //  0001  LDINT	R2	38
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x80063400,  //  0004  RET	1	K26
      0x70020056,  //  0005  JMP		#005D
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0022,  //  0007  LDINT	R2	35
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x80063600,  //  000A  RET	1	K27
      0x70020050,  //  000B  JMP		#005D
      0x88040100,  //  000C  GETMBR	R1	R0	K0
      0x1C04030F,  //  000D  EQ	R1	R1	K15
      0x78060005,  //  000E  JMPF	R1	#0015
      0x60040018,  //  000F  GETGBL	R1	G24
      0x5808001C,  //  0010  LDCONST	R2	K28
      0x880C0103,  //  0011  GETMBR	R3	R0	K3
      0x7C040400,  //  0012  CALL	R1	2
      0x80040200,  //  0013  RET	1	R1
      0x70020047,  //  0014  JMP		#005D
      0x88040100,  //  0015  GETMBR	R1	R0	K0
      0x1C040307,  //  0016  EQ	R1	R1	K7
      0x78060005,  //  0017  JMPF	R1	#001E
      0x60040018,  //  0018  GETGBL	R1	G24
      0x5808001D,  //  0019  LDCONST	R2	K29
      0x880C0103,  //  001A  GETMBR	R3	R0	K3
      0x7C040400,  //  001B  CALL	R1	2
      0x80040200,  //  001C  RET	1	R1
      0x7002003E,  //  001D  JMP		#005D
      0x88040100,  //  001E  GETMBR	R1	R0	K0
      0x1C04031E,  //  001F  EQ	R1	R1	K30
      0x78060005,  //  0020  JMPF	R1	#0027
      0x60040018,  //  0021  GETGBL	R1	G24
      0x5808001F,  //  0022  LDCONST	R2	K31
      0x880C0103,  //  0023  GETMBR	R3	R0	K3
      0x7C040400,  //  0024  CALL	R1	2
      0x80040200,  //  0025  RET	1	R1
      0x70020035,  //  0026  JMP		#005D
      0x88040100,  //  0027  GETMBR	R1	R0	K0
      0x1C04030B,  //  0028  EQ	R1	R1	K11
      0x78060005,  //  0029  JMPF	R1	#0030
      0x60040018,  //  002A  GETGBL	R1	G24
      0x58080020,  //  002B  LDCONST	R2	K32
      0x880C0103,  //  002C  GETMBR	R3	R0	K3
      0x7C040400,  //  002D  CALL	R1	2
      0x80040200,  //  002E  RET	1	R1
      0x7002002C,  //  002F  JMP		#005D
      0x88040100,  //  0030  GETMBR	R1	R0	K0
      0x540A0003,  //  0031  LDINT	R2	4
      0x1C040202,  //  0032  EQ	R1	R1	R2
      0x78060005,  //  0033  JMPF	R1	#003A
      0x60040018,  //  0034  GETGBL	R1	G24
      0x58080021,  //  0035  LDCONST	R2	K33
      0x880C0103,  //  0036  GETMBR	R3	R0	K3
      0x7C040400,  //  0037  CALL	R1	2
      0x80040200,  //  0038  RET	1	R1
      0x70020022,  //  0039  JMP		#005D
      0x88040100,  //  003A  GETMBR	R1	R0	K0
      0x540A0004,  //  003B  LDINT	R2	5
      0x1C040202,  //  003C  EQ	R1	R1	R2
      0x78060005,  //  003D  JMPF	R1	#0044
      0x60040018,  //  003E  GETGBL	R1	G24
      0x58080022,  //  003F  LDCONST	R2	K34
      0x880C0103,  //  0040  GETMBR	R3	R0	K3
      0x7C040400,  //  0041  CALL	R1	2
      0x80040200,  //  0042  RET	1	R1
      0x70020018,  //  0043  JMP		#005D
      0x88040100,  //  0044  GETMBR	R1	R0	K0
      0x540A0005,  //  0045  LDINT	R2	6
      0x1C040202,  //  0046  EQ	R1	R1	R2
      0x78060005,  //  0047  JMPF	R1	#004E
      0x60040018,  //  0048  GETGBL	R1	G24
      0x58080023,  //  0049  LDCONST	R2	K35
      0x880C0103,  //  004A  GETMBR	R3	R0	K3
      0x7C040400,  //  004B  CALL	R1	2
      0x80040200,  //  004C  RET	1	R1
      0x7002000E,  //  004D  JMP		#005D
      0x88040100,  //  004E  GETMBR	R1	R0	K0
      0x540A0026,  //  004F  LDINT	R2	39
      0x1C040202,  //  0050  EQ	R1	R1	R2
      0x78060005,  //  0051  JMPF	R1	#0058
      0x60040018,  //  0052  GETGBL	R1	G24
      0x58080024,  //  0053  LDCONST	R2	K36
      0x880C0103,  //  0054  GETMBR	R3	R0	K3
      0x7C040400,  //  0055  CALL	R1	2
      0x80040200,  //  0056  RET	1	R1
      0x70020004,  //  0057  JMP		#005D
      0x60040018,  //  0058  GETGBL	R1	G24
      0x58080025,  //  0059  LDCONST	R2	K37
      0x880C0103,  //  005A  GETMBR	R3	R0	K3
      0x7C040400,  //  005B  CALL	R1	2
      0x80040200,  //  005C  RET	1	R1
      0x80000000,  //  005D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_separator
********************************************************************/
be_local_closure(class_Token_is_separator,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_separator),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A001D,  //  0001  LDINT	R2	30
      0x28040202,  //  0002  GE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x540A0021,  //  0005  LDINT	R2	34
      0x18040202,  //  0006  LE	R1	R1	R2
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_keyword
********************************************************************/
be_local_closure(class_Token_is_keyword,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x1C08050F,  //  0001  EQ	R2	R2	K15
      0x780A0002,  //  0002  JMPF	R2	#0006
      0x88080103,  //  0003  GETMBR	R2	R0	K3
      0x1C080401,  //  0004  EQ	R2	R2	R1
      0x740A0000,  //  0005  JMPT	R2	#0007
      0x50080001,  //  0006  LDBOOL	R2	0	1
      0x50080200,  //  0007  LDBOOL	R2	1	0
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_literal
********************************************************************/
be_local_closure(class_Token_is_literal,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_literal),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C04030B,  //  0001  EQ	R1	R1	K11
      0x74060013,  //  0002  JMPT	R1	#0017
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x1C04031E,  //  0004  EQ	R1	R1	K30
      0x74060010,  //  0005  JMPT	R1	#0017
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0003,  //  0007  LDINT	R2	4
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x7406000C,  //  0009  JMPT	R1	#0017
      0x88040100,  //  000A  GETMBR	R1	R0	K0
      0x540A0004,  //  000B  LDINT	R2	5
      0x1C040202,  //  000C  EQ	R1	R1	R2
      0x74060008,  //  000D  JMPT	R1	#0017
      0x88040100,  //  000E  GETMBR	R1	R0	K0
      0x540A0005,  //  000F  LDINT	R2	6
      0x1C040202,  //  0010  EQ	R1	R1	R2
      0x74060004,  //  0011  JMPT	R1	#0017
      0x88040100,  //  0012  GETMBR	R1	R0	K0
      0x540A0006,  //  0013  LDINT	R2	7
      0x1C040202,  //  0014  EQ	R1	R1	R2
      0x74060000,  //  0015  JMPT	R1	#0017
      0x50040001,  //  0016  LDBOOL	R1	0	1
      0x50040200,  //  0017  LDBOOL	R1	1	0
      0x80040200,  //  0018  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_start_expression
********************************************************************/
be_local_closure(class_Token_can_start_expression,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(can_start_expression),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x8C040116,  //  0000  GETMET	R1	R0	K22
      0x7C040200,  //  0001  CALL	R1	1
      0x74060017,  //  0002  JMPT	R1	#001B
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x1C040307,  //  0004  EQ	R1	R1	K7
      0x74060014,  //  0005  JMPT	R1	#001B
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0023,  //  0007  LDINT	R2	36
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x74060010,  //  0009  JMPT	R1	#001B
      0x88040100,  //  000A  GETMBR	R1	R0	K0
      0x540A0017,  //  000B  LDINT	R2	24
      0x1C040202,  //  000C  EQ	R1	R1	R2
      0x7406000C,  //  000D  JMPT	R1	#001B
      0x88040100,  //  000E  GETMBR	R1	R0	K0
      0x540A0016,  //  000F  LDINT	R2	23
      0x1C040202,  //  0010  EQ	R1	R1	R2
      0x74060008,  //  0011  JMPT	R1	#001B
      0x88040100,  //  0012  GETMBR	R1	R0	K0
      0x540A0009,  //  0013  LDINT	R2	10
      0x1C040202,  //  0014  EQ	R1	R1	R2
      0x74060004,  //  0015  JMPT	R1	#001B
      0x88040100,  //  0016  GETMBR	R1	R0	K0
      0x540A0008,  //  0017  LDINT	R2	9
      0x1C040202,  //  0018  EQ	R1	R1	R2
      0x74060000,  //  0019  JMPT	R1	#001B
      0x50040001,  //  001A  LDBOOL	R1	0	1
      0x50040200,  //  001B  LDBOOL	R1	1	0
      0x80040200,  //  001C  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_boolean_value
********************************************************************/
be_local_closure(class_Token_get_boolean_value,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(get_boolean_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C040126,  //  0000  GETMET	R1	R0	K38
      0x7C040200,  //  0001  CALL	R1	1
      0x78060002,  //  0002  JMPF	R1	#0006
      0x88040103,  //  0003  GETMBR	R1	R0	K3
      0x1C040327,  //  0004  EQ	R1	R1	K39
      0x80040200,  //  0005  RET	1	R1
      0x4C040000,  //  0006  LDNIL	R1
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_numeric
********************************************************************/
be_local_closure(class_Token_is_numeric,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_numeric),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C04030B,  //  0001  EQ	R1	R1	K11
      0x7406000C,  //  0002  JMPT	R1	#0010
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x540A0004,  //  0004  LDINT	R2	5
      0x1C040202,  //  0005  EQ	R1	R1	R2
      0x74060008,  //  0006  JMPT	R1	#0010
      0x88040100,  //  0007  GETMBR	R1	R0	K0
      0x540A0005,  //  0008  LDINT	R2	6
      0x1C040202,  //  0009  EQ	R1	R1	R2
      0x74060004,  //  000A  JMPT	R1	#0010
      0x88040100,  //  000B  GETMBR	R1	R0	K0
      0x540A0006,  //  000C  LDINT	R2	7
      0x1C040202,  //  000D  EQ	R1	R1	R2
      0x74060000,  //  000E  JMPT	R1	#0010
      0x50040001,  //  000F  LDBOOL	R1	0	1
      0x50040200,  //  0010  LDBOOL	R1	1	0
      0x80040200,  //  0011  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_statement_start
********************************************************************/
be_local_closure(class_Token_is_statement_start,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_statement_start),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x2004030F,  //  0001  NE	R1	R1	K15
      0x78060001,  //  0002  JMPF	R1	#0005
      0x50040000,  //  0003  LDBOOL	R1	0	0
      0x80040200,  //  0004  RET	1	R1
      0x60040010,  //  0005  GETGBL	R1	G16
      0x88080128,  //  0006  GETMBR	R2	R0	K40
      0x7C040200,  //  0007  CALL	R1	1
      0xA8020008,  //  0008  EXBLK	0	#0012
      0x5C080200,  //  0009  MOVE	R2	R1
      0x7C080000,  //  000A  CALL	R2	0
      0x880C0103,  //  000B  GETMBR	R3	R0	K3
      0x1C0C0602,  //  000C  EQ	R3	R3	R2
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0xA8040001,  //  000F  EXBLK	1	1
      0x80040600,  //  0010  RET	1	R3
      0x7001FFF6,  //  0011  JMP		#0009
      0x58040029,  //  0012  LDCONST	R1	K41
      0xAC040200,  //  0013  CATCH	R1	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x50040000,  //  0015  LDBOOL	R1	0	0
      0x80040200,  //  0016  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_dsl_function
********************************************************************/
be_local_closure(class_Token_is_dsl_function,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_dsl_function),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x2004030F,  //  0001  NE	R1	R1	K15
      0x78060001,  //  0002  JMPF	R1	#0005
      0x50040000,  //  0003  LDBOOL	R1	0	0
      0x80040200,  //  0004  RET	1	R1
      0xA8020011,  //  0005  EXBLK	0	#0018
      0xA4065400,  //  0006  IMPORT	R1	K42
      0xB80A5600,  //  0007  GETNGBL	R2	K43
      0x8808052C,  //  0008  GETMBR	R2	R2	K44
      0x4C0C0000,  //  0009  LDNIL	R3
      0x200C0403,  //  000A  NE	R3	R2	R3
      0x780E0009,  //  000B  JMPF	R3	#0016
      0x8C0C032D,  //  000C  GETMET	R3	R1	K45
      0x5C140400,  //  000D  MOVE	R5	R2
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C10072E,  //  000F  GETMET	R4	R3	K46
      0x88180103,  //  0010  GETMBR	R6	R0	K3
      0x7C100400,  //  0011  CALL	R4	2
      0x4C140000,  //  0012  LDNIL	R5
      0x20100805,  //  0013  NE	R4	R4	R5
      0xA8040001,  //  0014  EXBLK	1	1
      0x80040800,  //  0015  RET	1	R4
      0xA8040001,  //  0016  EXBLK	1	1
      0x70020005,  //  0017  JMP		#001E
      0xAC040002,  //  0018  CATCH	R1	0	2
      0x70020002,  //  0019  JMP		#001D
      0x500C0000,  //  001A  LDBOOL	R3	0	0
      0x80040600,  //  001B  RET	1	R3
      0x70020000,  //  001C  JMP		#001E
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x50040000,  //  001E  LDBOOL	R1	0	0
      0x80040200,  //  001F  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_identifier
********************************************************************/
be_local_closure(class_Token_is_identifier,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_identifier),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x1C080507,  //  0001  EQ	R2	R2	K7
      0x780A0002,  //  0002  JMPF	R2	#0006
      0x88080103,  //  0003  GETMBR	R2	R0	K3
      0x1C080401,  //  0004  EQ	R2	R2	R1
      0x740A0000,  //  0005  JMPT	R2	#0007
      0x50080001,  //  0006  LDBOOL	R2	0	1
      0x50080200,  //  0007  LDBOOL	R2	1	0
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_boolean
********************************************************************/
be_local_closure(class_Token_is_boolean,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(is_boolean),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C04030F,  //  0001  EQ	R1	R1	K15
      0x78060005,  //  0002  JMPF	R1	#0009
      0x88040103,  //  0003  GETMBR	R1	R0	K3
      0x1C040327,  //  0004  EQ	R1	R1	K39
      0x74060003,  //  0005  JMPT	R1	#000A
      0x88040103,  //  0006  GETMBR	R1	R0	K3
      0x1C04032F,  //  0007  EQ	R1	R1	K47
      0x74060000,  //  0008  JMPT	R1	#000A
      0x50040001,  //  0009  LDBOOL	R1	0	1
      0x50040200,  //  000A  LDBOOL	R1	1	0
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(class_Token_tostring,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x8C040130,  //  0000  GETMET	R1	R0	K48
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x7C040400,  //  0002  CALL	R1	2
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x540E0025,  //  0004  LDINT	R3	38
      0x1C080403,  //  0005  EQ	R2	R2	R3
      0x780A0007,  //  0006  JMPF	R2	#000F
      0x60080018,  //  0007  GETGBL	R2	G24
      0x580C0031,  //  0008  LDCONST	R3	K49
      0x5C100200,  //  0009  MOVE	R4	R1
      0x88140104,  //  000A  GETMBR	R5	R0	K4
      0x88180105,  //  000B  GETMBR	R6	R0	K5
      0x7C080800,  //  000C  CALL	R2	4
      0x80040400,  //  000D  RET	1	R2
      0x70020027,  //  000E  JMP		#0037
      0x88080100,  //  000F  GETMBR	R2	R0	K0
      0x540E0022,  //  0010  LDINT	R3	35
      0x1C080403,  //  0011  EQ	R2	R2	R3
      0x780A0007,  //  0012  JMPF	R2	#001B
      0x60080018,  //  0013  GETGBL	R2	G24
      0x580C0031,  //  0014  LDCONST	R3	K49
      0x5C100200,  //  0015  MOVE	R4	R1
      0x88140104,  //  0016  GETMBR	R5	R0	K4
      0x88180105,  //  0017  GETMBR	R6	R0	K5
      0x7C080800,  //  0018  CALL	R2	4
      0x80040400,  //  0019  RET	1	R2
      0x7002001B,  //  001A  JMP		#0037
      0x6008000C,  //  001B  GETGBL	R2	G12
      0x880C0103,  //  001C  GETMBR	R3	R0	K3
      0x7C080200,  //  001D  CALL	R2	1
      0x540E0013,  //  001E  LDINT	R3	20
      0x24080403,  //  001F  GT	R2	R2	R3
      0x780A000D,  //  0020  JMPF	R2	#002F
      0x540A0010,  //  0021  LDINT	R2	17
      0x400A1E02,  //  0022  CONNECT	R2	K15	R2
      0x880C0103,  //  0023  GETMBR	R3	R0	K3
      0x94080602,  //  0024  GETIDX	R2	R3	R2
      0x00080532,  //  0025  ADD	R2	R2	K50
      0x600C0018,  //  0026  GETGBL	R3	G24
      0x58100033,  //  0027  LDCONST	R4	K51
      0x5C140200,  //  0028  MOVE	R5	R1
      0x5C180400,  //  0029  MOVE	R6	R2
      0x881C0104,  //  002A  GETMBR	R7	R0	K4
      0x88200105,  //  002B  GETMBR	R8	R0	K5
      0x7C0C0A00,  //  002C  CALL	R3	5
      0x80040600,  //  002D  RET	1	R3
      0x70020007,  //  002E  JMP		#0037
      0x60080018,  //  002F  GETGBL	R2	G24
      0x580C0033,  //  0030  LDCONST	R3	K51
      0x5C100200,  //  0031  MOVE	R4	R1
      0x88140103,  //  0032  GETMBR	R5	R0	K3
      0x88180104,  //  0033  GETMBR	R6	R0	K4
      0x881C0105,  //  0034  GETMBR	R7	R0	K5
      0x7C080A00,  //  0035  CALL	R2	5
      0x80040400,  //  0036  RET	1	R2
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: with_value
********************************************************************/
be_local_closure(class_Token_with_value,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Token,     /* shared constants */
    be_str_weak(with_value),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB80A0200,  //  0000  GETNGBL	R2	K1
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x88100100,  //  0002  GETMBR	R4	R0	K0
      0x5C140200,  //  0003  MOVE	R5	R1
      0x88180104,  //  0004  GETMBR	R6	R0	K4
      0x881C0105,  //  0005  GETMBR	R7	R0	K5
      0x6020000C,  //  0006  GETGBL	R8	G12
      0x5C240200,  //  0007  MOVE	R9	R1
      0x7C200200,  //  0008  CALL	R8	1
      0x7C080C00,  //  0009  CALL	R2	6
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Token
********************************************************************/
be_local_class(Token,
    5,
    NULL,
    be_nested_map(75,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(with_value, 17), be_const_closure(class_Token_with_value_closure) },
        { be_const_key_weak(TIME, -1), be_const_int(5) },
        { be_const_key_weak(ASSIGN, 25), be_const_int(8) },
        { be_const_key_weak(POWER, -1), be_const_int(14) },
        { be_const_key_weak(MULTIPLIER, -1), be_const_int(7) },
        { be_const_key_weak(MULTIPLY, 0), be_const_int(11) },
        { be_const_key_weak(with_type, 74), be_const_closure(class_Token_with_type_closure) },
        { be_const_key_weak(end_column, -1), be_const_closure(class_Token_end_column_closure) },
        { be_const_key_weak(value, 29), be_const_var(1) },
        { be_const_key_weak(LEFT_PAREN, -1), be_const_int(24) },
        { be_const_key_weak(is_operator, -1), be_const_closure(class_Token_is_operator_closure) },
        { be_const_key_weak(LOGICAL_NOT, -1), be_const_int(23) },
        { be_const_key_weak(NEWLINE, -1), be_const_int(35) },
        { be_const_key_weak(LEFT_BRACKET, -1), be_const_int(28) },
        { be_const_key_weak(tostring, 28), be_const_closure(class_Token_tostring_closure) },
        { be_const_key_weak(GREATER_EQUAL, -1), be_const_int(20) },
        { be_const_key_weak(is_delimiter, -1), be_const_closure(class_Token_is_delimiter_closure) },
        { be_const_key_weak(COMMENT, -1), be_const_int(37) },
        { be_const_key_weak(LOGICAL_OR, -1), be_const_int(22) },
        { be_const_key_weak(is_type, 73), be_const_closure(class_Token_is_type_closure) },
        { be_const_key_weak(PERCENTAGE, -1), be_const_int(6) },
        { be_const_key_weak(SEMICOLON, -1), be_const_int(31) },
        { be_const_key_weak(column, 72), be_const_var(3) },
        { be_const_key_weak(color_names, 11), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(37,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(red),
        be_nested_str_weak(green),
        be_nested_str_weak(blue),
        be_nested_str_weak(white),
        be_nested_str_weak(black),
        be_nested_str_weak(yellow),
        be_nested_str_weak(orange),
        be_nested_str_weak(purple),
        be_nested_str_weak(pink),
        be_nested_str_weak(cyan),
        be_nested_str_weak(magenta),
        be_nested_str_weak(gray),
        be_nested_str_weak(grey),
        be_nested_str_weak(silver),
        be_nested_str_weak(gold),
        be_nested_str_weak(brown),
        be_nested_str_weak(lime),
        be_nested_str_weak(navy),
        be_nested_str_weak(olive),
        be_nested_str_weak(maroon),
        be_nested_str_weak(teal),
        be_nested_str_weak(aqua),
        be_nested_str_weak(fuchsia),
        be_nested_str_weak(indigo),
        be_nested_str_weak(violet),
        be_nested_str_weak(crimson),
        be_nested_str_weak(coral),
        be_nested_str_weak(salmon),
        be_nested_str_weak(khaki),
        be_nested_str_weak(plum),
        be_nested_str_weak(orchid),
        be_nested_str_weak(turquoise),
        be_nested_str_weak(tan),
        be_nested_str_weak(beige),
        be_nested_str_weak(ivory),
        be_nested_str_weak(snow),
        be_nested_str_weak(transparent),
    }))    ) } )) },
        { be_const_key_weak(is_identifier, 44), be_const_closure(class_Token_is_identifier_closure) },
        { be_const_key_weak(is_dsl_function, -1), be_const_closure(class_Token_is_dsl_function_closure) },
        { be_const_key_weak(EOF, 61), be_const_int(38) },
        { be_const_key_weak(GREATER_THAN, -1), be_const_int(19) },
        { be_const_key_weak(is_numeric, 66), be_const_closure(class_Token_is_numeric_closure) },
        { be_const_key_weak(MODULO, -1), be_const_int(13) },
        { be_const_key_weak(init, 67), be_const_closure(class_Token_init_closure) },
        { be_const_key_weak(get_numeric_value, -1), be_const_closure(class_Token_get_numeric_value_closure) },
        { be_const_key_weak(IDENTIFIER, -1), be_const_int(1) },
        { be_const_key_weak(KEYWORD, 8), be_const_int(0) },
        { be_const_key_weak(RIGHT_PAREN, 68), be_const_int(25) },
        { be_const_key_weak(can_end_expression, 27), be_const_closure(class_Token_can_end_expression_closure) },
        { be_const_key_weak(MINUS, 56), be_const_int(10) },
        { be_const_key_weak(COLOR, -1), be_const_int(4) },
        { be_const_key_weak(get_boolean_value, 46), be_const_closure(class_Token_get_boolean_value_closure) },
        { be_const_key_weak(to_string, -1), be_const_static_closure(class_Token_to_string_closure) },
        { be_const_key_weak(COLON, -1), be_const_int(32) },
        { be_const_key_weak(EVENT_INTERRUPT, -1), be_const_int(41) },
        { be_const_key_weak(LESS_THAN, 64), be_const_int(17) },
        { be_const_key_weak(VARIABLE_REF, -1), be_const_int(36) },
        { be_const_key_weak(type, -1), be_const_var(0) },
        { be_const_key_weak(NOT_EQUAL, 34), be_const_int(16) },
        { be_const_key_weak(length, 63), be_const_var(4) },
        { be_const_key_weak(to_error_string, -1), be_const_closure(class_Token_to_error_string_closure) },
        { be_const_key_weak(line, 24), be_const_var(2) },
        { be_const_key_weak(is_separator, -1), be_const_closure(class_Token_is_separator_closure) },
        { be_const_key_weak(NUMBER, -1), be_const_int(2) },
        { be_const_key_weak(keywords, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(72,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(strip),
        be_nested_str_weak(set),
        be_nested_str_weak(import),
        be_nested_str_weak(color),
        be_nested_str_weak(palette),
        be_nested_str_weak(animation),
        be_nested_str_weak(sequence),
        be_nested_str_weak(function),
        be_nested_str_weak(zone),
        be_nested_str_weak(template),
        be_nested_str_weak(param),
        be_nested_str_weak(type),
        be_nested_str_weak(play),
        be_nested_str_weak(for),
        be_nested_str_weak(with),
        be_nested_str_weak(repeat),
        be_nested_str_weak(times),
        be_nested_str_weak(forever),
        be_nested_str_weak(if),
        be_nested_str_weak(else),
        be_nested_str_weak(elif),
        be_nested_str_weak(choose),
        be_nested_str_weak(random),
        be_nested_str_weak(on),
        be_nested_str_weak(run),
        be_nested_str_weak(wait),
        be_nested_str_weak(goto),
        be_nested_str_weak(interrupt),
        be_nested_str_weak(resume),
        be_nested_str_weak(while),
        be_nested_str_weak(from),
        be_nested_str_weak(to),
        be_nested_str_weak(return),
        be_nested_str_weak(reset),
        be_nested_str_weak(restart),
        be_nested_str_weak(at),
        be_nested_str_weak(ease),
        be_nested_str_weak(sync),
        be_nested_str_weak(every),
        be_nested_str_weak(stagger),
        be_nested_str_weak(across),
        be_nested_str_weak(pixels),
        be_nested_str_weak(rgb),
        be_nested_str_weak(hsv),
        be_nested_str_weak(all),
        be_nested_str_weak(even),
        be_nested_str_weak(odd),
        be_nested_str_weak(center),
        be_nested_str_weak(edges),
        be_nested_str_weak(left),
        be_nested_str_weak(right),
        be_nested_str_weak(top),
        be_nested_str_weak(bottom),
        be_nested_str_weak(true),
        be_nested_str_weak(false),
        be_nested_str_weak(nil),
        be_nested_str_weak(transparent),
        be_nested_str_weak(startup),
        be_nested_str_weak(shutdown),
        be_nested_str_weak(button_press),
        be_nested_str_weak(button_hold),
        be_nested_str_weak(motion_detected),
        be_nested_str_weak(brightness_change),
        be_nested_str_weak(timer),
        be_nested_str_weak(time),
        be_nested_str_weak(sound_peak),
        be_nested_str_weak(network_message),
        be_nested_str_weak(ms),
        be_nested_str_weak(s),
        be_nested_str_weak(m),
        be_nested_str_weak(h),
        be_nested_str_weak(bpm),
    }))    ) } )) },
        { be_const_key_weak(ERROR, -1), be_const_int(39) },
        { be_const_key_weak(DOT, 60), be_const_int(33) },
        { be_const_key_weak(EQUAL, 38), be_const_int(15) },
        { be_const_key_weak(is_literal, -1), be_const_closure(class_Token_is_literal_closure) },
        { be_const_key_weak(LOGICAL_AND, -1), be_const_int(21) },
        { be_const_key_weak(can_start_expression, -1), be_const_closure(class_Token_can_start_expression_closure) },
        { be_const_key_weak(DIVIDE, -1), be_const_int(12) },
        { be_const_key_weak(LEFT_BRACE, 3), be_const_int(26) },
        { be_const_key_weak(is_keyword, -1), be_const_closure(class_Token_is_keyword_closure) },
        { be_const_key_weak(LESS_EQUAL, -1), be_const_int(18) },
        { be_const_key_weak(RIGHT_BRACE, 20), be_const_int(27) },
        { be_const_key_weak(RIGHT_BRACKET, -1), be_const_int(29) },
        { be_const_key_weak(COMMA, -1), be_const_int(30) },
        { be_const_key_weak(is_statement_start, -1), be_const_closure(class_Token_is_statement_start_closure) },
        { be_const_key_weak(names, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(44,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(KEYWORD),
        be_nested_str_weak(IDENTIFIER),
        be_nested_str_weak(NUMBER),
        be_nested_str_weak(STRING),
        be_nested_str_weak(COLOR),
        be_nested_str_weak(TIME),
        be_nested_str_weak(PERCENTAGE),
        be_nested_str_weak(MULTIPLIER),
        be_nested_str_weak(ASSIGN),
        be_nested_str_weak(PLUS),
        be_nested_str_weak(MINUS),
        be_nested_str_weak(MULTIPLY),
        be_nested_str_weak(DIVIDE),
        be_nested_str_weak(MODULO),
        be_nested_str_weak(POWER),
        be_nested_str_weak(EQUAL),
        be_nested_str_weak(NOT_EQUAL),
        be_nested_str_weak(LESS_THAN),
        be_nested_str_weak(LESS_EQUAL),
        be_nested_str_weak(GREATER_THAN),
        be_nested_str_weak(GREATER_EQUAL),
        be_nested_str_weak(LOGICAL_AND),
        be_nested_str_weak(LOGICAL_OR),
        be_nested_str_weak(LOGICAL_NOT),
        be_nested_str_weak(LEFT_PAREN),
        be_nested_str_weak(RIGHT_PAREN),
        be_nested_str_weak(LEFT_BRACE),
        be_nested_str_weak(RIGHT_BRACE),
        be_nested_str_weak(LEFT_BRACKET),
        be_nested_str_weak(RIGHT_BRACKET),
        be_nested_str_weak(COMMA),
        be_nested_str_weak(SEMICOLON),
        be_nested_str_weak(COLON),
        be_nested_str_weak(DOT),
        be_nested_str_weak(ARROW),
        be_nested_str_weak(NEWLINE),
        be_nested_str_weak(VARIABLE_REF),
        be_nested_str_weak(COMMENT),
        be_nested_str_weak(EOF),
        be_nested_str_weak(ERROR),
        be_nested_str_weak(EVENT_ON),
        be_nested_str_weak(EVENT_INTERRUPT),
        be_nested_str_weak(EVENT_RESUME),
        be_nested_str_weak(EVENT_AFTER),
    }))    ) } )) },
        { be_const_key_weak(ARROW, -1), be_const_int(34) },
        { be_const_key_weak(PLUS, -1), be_const_int(9) },
        { be_const_key_weak(is_boolean, -1), be_const_closure(class_Token_is_boolean_closure) },
        { be_const_key_weak(STRING, 14), be_const_int(3) },
        { be_const_key_weak(EVENT_ON, -1), be_const_int(40) },
        { be_const_key_weak(EVENT_AFTER, -1), be_const_int(43) },
        { be_const_key_weak(statement_keywords, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(13,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(strip),
        be_nested_str_weak(set),
        be_nested_str_weak(color),
        be_nested_str_weak(palette),
        be_nested_str_weak(animation),
        be_nested_str_weak(sequence),
        be_nested_str_weak(function),
        be_nested_str_weak(zone),
        be_nested_str_weak(on),
        be_nested_str_weak(run),
        be_nested_str_weak(template),
        be_nested_str_weak(param),
        be_nested_str_weak(import),
    }))    ) } )) },
        { be_const_key_weak(EVENT_RESUME, -1), be_const_int(42) },
    })),
    be_str_weak(Token)
);

extern const bclass be_class_MockEngine;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_MockEngine_init,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(time_ms),
    /* K1   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020101,  //  0000  SETMBR	R0	K0	K1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_strip_length
********************************************************************/
be_local_closure(class_MockEngine_get_strip_length,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(get_strip_length),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x5406001D,  //  0000  LDINT	R1	30
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: MockEngine
********************************************************************/
be_local_class(MockEngine,
    1,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(time_ms, 1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(class_MockEngine_init_closure) },
        { be_const_key_weak(get_strip_length, -1), be_const_closure(class_MockEngine_get_strip_length_closure) },
    })),
    be_str_weak(MockEngine)
);

/********************************************************************
** Solidified function: is_right_associative
********************************************************************/
be_local_closure(is_right_associative,   /* name */
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
    /* K0   */  be_nested_str_weak(type),
    }),
    be_str_weak(is_right_associative),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A000D,  //  0001  LDINT	R2	14
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_operator_precedence
********************************************************************/
be_local_closure(get_operator_precedence,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(type),
    /* K1   */  be_const_int(1),
    /* K2   */  be_const_int(2),
    /* K3   */  be_const_int(3),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(get_operator_precedence),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A0015,  //  0001  LDINT	R2	22
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x80060200,  //  0004  RET	1	K1
      0x70020042,  //  0005  JMP		#0049
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x540A0014,  //  0007  LDINT	R2	21
      0x1C040202,  //  0008  EQ	R1	R1	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x80060400,  //  000A  RET	1	K2
      0x7002003C,  //  000B  JMP		#0049
      0x88040100,  //  000C  GETMBR	R1	R0	K0
      0x540A000E,  //  000D  LDINT	R2	15
      0x1C040202,  //  000E  EQ	R1	R1	R2
      0x74060003,  //  000F  JMPT	R1	#0014
      0x88040100,  //  0010  GETMBR	R1	R0	K0
      0x540A000F,  //  0011  LDINT	R2	16
      0x1C040202,  //  0012  EQ	R1	R1	R2
      0x78060001,  //  0013  JMPF	R1	#0016
      0x80060600,  //  0014  RET	1	K3
      0x70020032,  //  0015  JMP		#0049
      0x88040100,  //  0016  GETMBR	R1	R0	K0
      0x540A0010,  //  0017  LDINT	R2	17
      0x1C040202,  //  0018  EQ	R1	R1	R2
      0x7406000B,  //  0019  JMPT	R1	#0026
      0x88040100,  //  001A  GETMBR	R1	R0	K0
      0x540A0011,  //  001B  LDINT	R2	18
      0x1C040202,  //  001C  EQ	R1	R1	R2
      0x74060007,  //  001D  JMPT	R1	#0026
      0x88040100,  //  001E  GETMBR	R1	R0	K0
      0x540A0012,  //  001F  LDINT	R2	19
      0x1C040202,  //  0020  EQ	R1	R1	R2
      0x74060003,  //  0021  JMPT	R1	#0026
      0x88040100,  //  0022  GETMBR	R1	R0	K0
      0x540A0013,  //  0023  LDINT	R2	20
      0x1C040202,  //  0024  EQ	R1	R1	R2
      0x78060002,  //  0025  JMPF	R1	#0029
      0x54060003,  //  0026  LDINT	R1	4
      0x80040200,  //  0027  RET	1	R1
      0x7002001F,  //  0028  JMP		#0049
      0x88040100,  //  0029  GETMBR	R1	R0	K0
      0x540A0008,  //  002A  LDINT	R2	9
      0x1C040202,  //  002B  EQ	R1	R1	R2
      0x74060003,  //  002C  JMPT	R1	#0031
      0x88040100,  //  002D  GETMBR	R1	R0	K0
      0x540A0009,  //  002E  LDINT	R2	10
      0x1C040202,  //  002F  EQ	R1	R1	R2
      0x78060002,  //  0030  JMPF	R1	#0034
      0x54060004,  //  0031  LDINT	R1	5
      0x80040200,  //  0032  RET	1	R1
      0x70020014,  //  0033  JMP		#0049
      0x88040100,  //  0034  GETMBR	R1	R0	K0
      0x540A000A,  //  0035  LDINT	R2	11
      0x1C040202,  //  0036  EQ	R1	R1	R2
      0x74060007,  //  0037  JMPT	R1	#0040
      0x88040100,  //  0038  GETMBR	R1	R0	K0
      0x540A000B,  //  0039  LDINT	R2	12
      0x1C040202,  //  003A  EQ	R1	R1	R2
      0x74060003,  //  003B  JMPT	R1	#0040
      0x88040100,  //  003C  GETMBR	R1	R0	K0
      0x540A000C,  //  003D  LDINT	R2	13
      0x1C040202,  //  003E  EQ	R1	R1	R2
      0x78060002,  //  003F  JMPF	R1	#0043
      0x54060005,  //  0040  LDINT	R1	6
      0x80040200,  //  0041  RET	1	R1
      0x70020005,  //  0042  JMP		#0049
      0x88040100,  //  0043  GETMBR	R1	R0	K0
      0x540A000D,  //  0044  LDINT	R2	14
      0x1C040202,  //  0045  EQ	R1	R1	R2
      0x78060001,  //  0046  JMPF	R1	#0049
      0x54060006,  //  0047  LDINT	R1	7
      0x80040200,  //  0048  RET	1	R1
      0x80060800,  //  0049  RET	1	K4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_eof_token
********************************************************************/
be_local_closure(create_eof_token,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_const_int(0),
    }),
    be_str_weak(create_eof_token),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x54120025,  //  0002  LDINT	R4	38
      0x58140002,  //  0003  LDCONST	R5	K2
      0x5C180000,  //  0004  MOVE	R6	R0
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x58200003,  //  0006  LDCONST	R8	K3
      0x7C080C00,  //  0007  CALL	R2	6
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/

// ktab too big for class 'SimpleDSLTranspiler' - skipping

extern const bclass be_class_SimpleDSLTranspiler;

/********************************************************************
** Solidified function: expect_keyword
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_keyword,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(KEYWORD),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(Expected_X20_X27_X25s_X27),
    }),
    be_str_weak(expect_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x200C0403,  //  0003  NE	R3	R2	R3
      0x780E000B,  //  0004  JMPF	R3	#0011
      0x880C0501,  //  0005  GETMBR	R3	R2	K1
      0xB8120400,  //  0006  GETNGBL	R4	K2
      0x88100903,  //  0007  GETMBR	R4	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
      0x1C0C0604,  //  0009  EQ	R3	R3	R4
      0x780E0005,  //  000A  JMPF	R3	#0011
      0x880C0505,  //  000B  GETMBR	R3	R2	K5
      0x1C0C0601,  //  000C  EQ	R3	R3	R1
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x8C0C0106,  //  000E  GETMET	R3	R0	K6
      0x7C0C0200,  //  000F  CALL	R3	1
      0x70020005,  //  0010  JMP		#0017
      0x8C0C0107,  //  0011  GETMET	R3	R0	K7
      0x60140018,  //  0012  GETGBL	R5	G24
      0x58180008,  //  0013  LDCONST	R6	K8
      0x5C1C0200,  //  0014  MOVE	R7	R1
      0x7C140400,  //  0015  CALL	R5	2
      0x7C0C0400,  //  0016  CALL	R3	2
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_identifier
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_identifier,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(IDENTIFIER),
    /* K5   */  be_nested_str_weak(COLOR),
    /* K6   */  be_nested_str_weak(KEYWORD),
    /* K7   */  be_nested_str_weak(can_use_as_identifier),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(next),
    /* K10  */  be_nested_str_weak(error),
    /* K11  */  be_nested_str_weak(Expected_X20identifier),
    /* K12  */  be_nested_str_weak(unknown),
    }),
    be_str_weak(expect_identifier),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A001A,  //  0004  JMPF	R2	#0020
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A000F,  //  000A  JMPT	R2	#001B
      0x88080301,  //  000B  GETMBR	R2	R1	K1
      0xB80E0400,  //  000C  GETNGBL	R3	K2
      0x880C0703,  //  000D  GETMBR	R3	R3	K3
      0x880C0705,  //  000E  GETMBR	R3	R3	K5
      0x1C080403,  //  000F  EQ	R2	R2	R3
      0x740A0009,  //  0010  JMPT	R2	#001B
      0x88080301,  //  0011  GETMBR	R2	R1	K1
      0xB80E0400,  //  0012  GETNGBL	R3	K2
      0x880C0703,  //  0013  GETMBR	R3	R3	K3
      0x880C0706,  //  0014  GETMBR	R3	R3	K6
      0x1C080403,  //  0015  EQ	R2	R2	R3
      0x780A0008,  //  0016  JMPF	R2	#0020
      0x8C080107,  //  0017  GETMET	R2	R0	K7
      0x88100308,  //  0018  GETMBR	R4	R1	K8
      0x7C080400,  //  0019  CALL	R2	2
      0x780A0004,  //  001A  JMPF	R2	#0020
      0x88080308,  //  001B  GETMBR	R2	R1	K8
      0x8C0C0109,  //  001C  GETMET	R3	R0	K9
      0x7C0C0200,  //  001D  CALL	R3	1
      0x80040400,  //  001E  RET	1	R2
      0x70020003,  //  001F  JMP		#0024
      0x8C08010A,  //  0020  GETMET	R2	R0	K10
      0x5810000B,  //  0021  LDCONST	R4	K11
      0x7C080400,  //  0022  CALL	R2	2
      0x80061800,  //  0023  RET	1	K12
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: symbol_exists
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_symbol_exists,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(animation_dsl),
    /* K2   */  be_nested_str_weak(is_color_name),
    /* K3   */  be_nested_str_weak(symbol_table),
    /* K4   */  be_nested_str_weak(contains),
    /* K5   */  be_nested_str_weak(sequence_names),
    /* K6   */  be_nested_str_weak(animation),
    }),
    be_str_weak(symbol_exists),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x740E000F,  //  0005  JMPT	R3	#0016
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0x8C0C0704,  //  0007  GETMET	R3	R3	K4
      0x5C140200,  //  0008  MOVE	R5	R1
      0x7C0C0400,  //  0009  CALL	R3	2
      0x740E000A,  //  000A  JMPT	R3	#0016
      0x880C0105,  //  000B  GETMBR	R3	R0	K5
      0x8C0C0704,  //  000C  GETMET	R3	R3	K4
      0x5C140200,  //  000D  MOVE	R5	R1
      0x7C0C0400,  //  000E  CALL	R3	2
      0x740E0005,  //  000F  JMPT	R3	#0016
      0x8C0C0504,  //  0010  GETMET	R3	R2	K4
      0xB8160C00,  //  0011  GETNGBL	R5	K6
      0x5C180200,  //  0012  MOVE	R6	R1
      0x7C0C0600,  //  0013  CALL	R3	3
      0x740E0000,  //  0014  JMPT	R3	#0016
      0x500C0001,  //  0015  LDBOOL	R3	0	1
      0x500C0200,  //  0016  LDBOOL	R3	1	0
      0x80040600,  //  0017  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_additive_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_additive_expression,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(process_multiplicative_expression),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(PLUS),
    /* K7   */  be_nested_str_weak(MINUS),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(next),
    /* K10  */  be_nested_str_weak(_X25s_X20_X25s_X20_X25s),
    /* K11  */  be_nested_str_weak(is_anonymous_function),
    /* K12  */  be_nested_str_weak(repeat_count),
    /* K13  */  be_nested_str_weak(string),
    /* K14  */  be_nested_str_weak(is_computed_expression_string),
    /* K15  */  be_nested_str_weak(find),
    /* K16  */  be_nested_str_weak(_X2E),
    /* K17  */  be_const_int(0),
    /* K18  */  be_nested_str_weak(create_simple_function_from_string),
    /* K19  */  be_nested_str_weak(create_computation_closure_from_string),
    }),
    be_str_weak(process_additive_expression),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x5C200600,  //  0003  MOVE	R8	R3
      0x7C100800,  //  0004  CALL	R4	4
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x74160022,  //  0007  JMPT	R5	#002B
      0x8C140102,  //  0008  GETMET	R5	R0	K2
      0x7C140200,  //  0009  CALL	R5	1
      0x4C180000,  //  000A  LDNIL	R6
      0x20180A06,  //  000B  NE	R6	R5	R6
      0x781A001B,  //  000C  JMPF	R6	#0029
      0x88180B03,  //  000D  GETMBR	R6	R5	K3
      0xB81E0800,  //  000E  GETNGBL	R7	K4
      0x881C0F05,  //  000F  GETMBR	R7	R7	K5
      0x881C0F06,  //  0010  GETMBR	R7	R7	K6
      0x1C180C07,  //  0011  EQ	R6	R6	R7
      0x741A0005,  //  0012  JMPT	R6	#0019
      0x88180B03,  //  0013  GETMBR	R6	R5	K3
      0xB81E0800,  //  0014  GETNGBL	R7	K4
      0x881C0F05,  //  0015  GETMBR	R7	R7	K5
      0x881C0F07,  //  0016  GETMBR	R7	R7	K7
      0x1C180C07,  //  0017  EQ	R6	R6	R7
      0x781A000F,  //  0018  JMPF	R6	#0029
      0x88180B08,  //  0019  GETMBR	R6	R5	K8
      0x8C1C0109,  //  001A  GETMET	R7	R0	K9
      0x7C1C0200,  //  001B  CALL	R7	1
      0x8C1C0100,  //  001C  GETMET	R7	R0	K0
      0x5C240200,  //  001D  MOVE	R9	R1
      0x50280000,  //  001E  LDBOOL	R10	0	0
      0x5C2C0600,  //  001F  MOVE	R11	R3
      0x7C1C0800,  //  0020  CALL	R7	4
      0x60200018,  //  0021  GETGBL	R8	G24
      0x5824000A,  //  0022  LDCONST	R9	K10
      0x5C280800,  //  0023  MOVE	R10	R4
      0x5C2C0C00,  //  0024  MOVE	R11	R6
      0x5C300E00,  //  0025  MOVE	R12	R7
      0x7C200800,  //  0026  CALL	R8	4
      0x5C101000,  //  0027  MOVE	R4	R8
      0x70020000,  //  0028  JMP		#002A
      0x70020000,  //  0029  JMP		#002B
      0x7001FFD9,  //  002A  JMP		#0005
      0x780E0000,  //  002B  JMPF	R3	#002D
      0x80040800,  //  002C  RET	1	R4
      0x780A0022,  //  002D  JMPF	R2	#0051
      0x8C14010B,  //  002E  GETMET	R5	R0	K11
      0x5C1C0800,  //  002F  MOVE	R7	R4
      0x7C140400,  //  0030  CALL	R5	2
      0x7416001E,  //  0031  JMPT	R5	#0051
      0x1C14030C,  //  0032  EQ	R5	R1	K12
      0x78160011,  //  0033  JMPF	R5	#0046
      0xA4161A00,  //  0034  IMPORT	R5	K13
      0x8C18010E,  //  0035  GETMET	R6	R0	K14
      0x5C200800,  //  0036  MOVE	R8	R4
      0x7C180400,  //  0037  CALL	R6	2
      0x741A0005,  //  0038  JMPT	R6	#003F
      0x8C180B0F,  //  0039  GETMET	R6	R5	K15
      0x5C200800,  //  003A  MOVE	R8	R4
      0x58240010,  //  003B  LDCONST	R9	K16
      0x7C180600,  //  003C  CALL	R6	3
      0x28180D11,  //  003D  GE	R6	R6	K17
      0x781A0004,  //  003E  JMPF	R6	#0044
      0x8C180112,  //  003F  GETMET	R6	R0	K18
      0x5C200800,  //  0040  MOVE	R8	R4
      0x7C180400,  //  0041  CALL	R6	2
      0x80040C00,  //  0042  RET	1	R6
      0x70020000,  //  0043  JMP		#0045
      0x80040800,  //  0044  RET	1	R4
      0x70020009,  //  0045  JMP		#0050
      0x8C14010E,  //  0046  GETMET	R5	R0	K14
      0x5C1C0800,  //  0047  MOVE	R7	R4
      0x7C140400,  //  0048  CALL	R5	2
      0x78160004,  //  0049  JMPF	R5	#004F
      0x8C140113,  //  004A  GETMET	R5	R0	K19
      0x5C1C0800,  //  004B  MOVE	R7	R4
      0x7C140400,  //  004C  CALL	R5	2
      0x80040A00,  //  004D  RET	1	R5
      0x70020000,  //  004E  JMP		#0050
      0x80040800,  //  004F  RET	1	R4
      0x70020000,  //  0050  JMP		#0052
      0x80040800,  //  0051  RET	1	R4
      0x80000000,  //  0052  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: at_end
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_at_end,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(pos),
    /* K1   */  be_nested_str_weak(tokens),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(EOF),
    }),
    be_str_weak(at_end),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x28040202,  //  0004  GE	R1	R1	R2
      0x7406000D,  //  0005  JMPT	R1	#0014
      0x8C040102,  //  0006  GETMET	R1	R0	K2
      0x7C040200,  //  0007  CALL	R1	1
      0x4C080000,  //  0008  LDNIL	R2
      0x20040202,  //  0009  NE	R1	R1	R2
      0x78060007,  //  000A  JMPF	R1	#0013
      0x8C040102,  //  000B  GETMET	R1	R0	K2
      0x7C040200,  //  000C  CALL	R1	1
      0x88040303,  //  000D  GETMBR	R1	R1	K3
      0xB80A0800,  //  000E  GETNGBL	R2	K4
      0x88080505,  //  000F  GETMBR	R2	R2	K5
      0x88080506,  //  0010  GETMBR	R2	R2	K6
      0x1C040202,  //  0011  EQ	R1	R1	R2
      0x74060000,  //  0012  JMPT	R1	#0014
      0x50040001,  //  0013  LDBOOL	R1	0	1
      0x50040200,  //  0014  LDBOOL	R1	1	0
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_wait_statement_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_wait_statement_fluent,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(process_time_value),
    /* K2   */  be_nested_str_weak(collect_inline_comment),
    /* K3   */  be_nested_str_weak(add),
    /* K4   */  be_nested_str_weak(_X25s_X2Epush_wait_step_X28_X25s_X29_X25s),
    /* K5   */  be_nested_str_weak(get_indent),
    }),
    be_str_weak(process_wait_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x8C0C0103,  //  0006  GETMET	R3	R0	K3
      0x60140018,  //  0007  GETGBL	R5	G24
      0x58180004,  //  0008  LDCONST	R6	K4
      0x8C1C0105,  //  0009  GETMET	R7	R0	K5
      0x7C1C0200,  //  000A  CALL	R7	1
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240400,  //  000C  MOVE	R9	R2
      0x7C140800,  //  000D  CALL	R5	4
      0x7C0C0400,  //  000E  CALL	R3	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_computed_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_is_computed_expression,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(_X28),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(animation_X2E),
    /* K5   */  be_nested_str_weak(_),
    }),
    be_str_weak(is_computed_expression),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x8C140901,  //  0001  GETMET	R5	R4	K1
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x58200002,  //  0003  LDCONST	R8	K2
      0x7C140600,  //  0004  CALL	R5	3
      0x28140B03,  //  0005  GE	R5	R5	K3
      0x7416001E,  //  0006  JMPT	R5	#0026
      0x8C140901,  //  0007  GETMET	R5	R4	K1
      0x5C1C0600,  //  0008  MOVE	R7	R3
      0x58200002,  //  0009  LDCONST	R8	K2
      0x7C140600,  //  000A  CALL	R5	3
      0x28140B03,  //  000B  GE	R5	R5	K3
      0x74160018,  //  000C  JMPT	R5	#0026
      0x8C140901,  //  000D  GETMET	R5	R4	K1
      0x5C1C0200,  //  000E  MOVE	R7	R1
      0x58200004,  //  000F  LDCONST	R8	K4
      0x7C140600,  //  0010  CALL	R5	3
      0x28140B03,  //  0011  GE	R5	R5	K3
      0x74160012,  //  0012  JMPT	R5	#0026
      0x8C140901,  //  0013  GETMET	R5	R4	K1
      0x5C1C0600,  //  0014  MOVE	R7	R3
      0x58200004,  //  0015  LDCONST	R8	K4
      0x7C140600,  //  0016  CALL	R5	3
      0x28140B03,  //  0017  GE	R5	R5	K3
      0x7416000C,  //  0018  JMPT	R5	#0026
      0x8C140901,  //  0019  GETMET	R5	R4	K1
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0x58200005,  //  001B  LDCONST	R8	K5
      0x7C140600,  //  001C  CALL	R5	3
      0x28140B03,  //  001D  GE	R5	R5	K3
      0x74160006,  //  001E  JMPT	R5	#0026
      0x8C140901,  //  001F  GETMET	R5	R4	K1
      0x5C1C0600,  //  0020  MOVE	R7	R3
      0x58200005,  //  0021  LDCONST	R8	K5
      0x7C140600,  //  0022  CALL	R5	3
      0x28140B03,  //  0023  GE	R5	R5	K3
      0x74160000,  //  0024  JMPT	R5	#0026
      0x50140001,  //  0025  LDBOOL	R5	0	1
      0x50140200,  //  0026  LDBOOL	R5	1	0
      0x80040A00,  //  0027  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_colon
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_colon,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(COLON),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X3A_X27),
    }),
    be_str_weak(expect_colon),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_multiplicative_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_multiplicative_expression,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(process_unary_expression),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(MULTIPLY),
    /* K7   */  be_nested_str_weak(DIVIDE),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(next),
    /* K10  */  be_nested_str_weak(_X25s_X20_X25s_X20_X25s),
    }),
    be_str_weak(process_multiplicative_expression),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x5C200600,  //  0003  MOVE	R8	R3
      0x7C100800,  //  0004  CALL	R4	4
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x74160022,  //  0007  JMPT	R5	#002B
      0x8C140102,  //  0008  GETMET	R5	R0	K2
      0x7C140200,  //  0009  CALL	R5	1
      0x4C180000,  //  000A  LDNIL	R6
      0x20180A06,  //  000B  NE	R6	R5	R6
      0x781A001B,  //  000C  JMPF	R6	#0029
      0x88180B03,  //  000D  GETMBR	R6	R5	K3
      0xB81E0800,  //  000E  GETNGBL	R7	K4
      0x881C0F05,  //  000F  GETMBR	R7	R7	K5
      0x881C0F06,  //  0010  GETMBR	R7	R7	K6
      0x1C180C07,  //  0011  EQ	R6	R6	R7
      0x741A0005,  //  0012  JMPT	R6	#0019
      0x88180B03,  //  0013  GETMBR	R6	R5	K3
      0xB81E0800,  //  0014  GETNGBL	R7	K4
      0x881C0F05,  //  0015  GETMBR	R7	R7	K5
      0x881C0F07,  //  0016  GETMBR	R7	R7	K7
      0x1C180C07,  //  0017  EQ	R6	R6	R7
      0x781A000F,  //  0018  JMPF	R6	#0029
      0x88180B08,  //  0019  GETMBR	R6	R5	K8
      0x8C1C0109,  //  001A  GETMET	R7	R0	K9
      0x7C1C0200,  //  001B  CALL	R7	1
      0x8C1C0100,  //  001C  GETMET	R7	R0	K0
      0x5C240200,  //  001D  MOVE	R9	R1
      0x50280000,  //  001E  LDBOOL	R10	0	0
      0x5C2C0600,  //  001F  MOVE	R11	R3
      0x7C1C0800,  //  0020  CALL	R7	4
      0x60200018,  //  0021  GETGBL	R8	G24
      0x5824000A,  //  0022  LDCONST	R9	K10
      0x5C280800,  //  0023  MOVE	R10	R4
      0x5C2C0C00,  //  0024  MOVE	R11	R6
      0x5C300E00,  //  0025  MOVE	R12	R7
      0x7C200800,  //  0026  CALL	R8	4
      0x5C101000,  //  0027  MOVE	R4	R8
      0x70020000,  //  0028  JMP		#002A
      0x70020000,  //  0029  JMP		#002B
      0x7001FFD9,  //  002A  JMP		#0005
      0x80040800,  //  002B  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_named_color_value
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_named_color_value,   /* name */
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
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(SimpleDSLTranspiler),
    /* K2   */  be_nested_str_weak(named_colors),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(0xFFFFFFFF),
    }),
    be_str_weak(get_named_color_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x88080502,  //  0002  GETMBR	R2	R2	K2
      0x8C080503,  //  0003  GETMET	R2	R2	K3
      0x5C100200,  //  0004  MOVE	R4	R1
      0x58140004,  //  0005  LDCONST	R5	K4
      0x7C080600,  //  0006  CALL	R2	3
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_color
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_color,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[37]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(color),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_assign),
    /* K6   */  be_nested_str_weak(current),
    /* K7   */  be_nested_str_weak(type),
    /* K8   */  be_nested_str_weak(animation_dsl),
    /* K9   */  be_nested_str_weak(Token),
    /* K10  */  be_nested_str_weak(KEYWORD),
    /* K11  */  be_nested_str_weak(IDENTIFIER),
    /* K12  */  be_nested_str_weak(peek),
    /* K13  */  be_nested_str_weak(LEFT_PAREN),
    /* K14  */  be_nested_str_weak(value),
    /* K15  */  be_nested_str_weak(),
    /* K16  */  be_nested_str_weak(COMMENT),
    /* K17  */  be_nested_str_weak(_X20_X20),
    /* K18  */  be_nested_str_weak(template_definitions),
    /* K19  */  be_nested_str_weak(contains),
    /* K20  */  be_nested_str_weak(process_function_arguments),
    /* K21  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K22  */  be_nested_str_weak(engine),
    /* K23  */  be_nested_str_weak(add),
    /* K24  */  be_nested_str_weak(_X25s_template_X28_X25s_X29_X25s),
    /* K25  */  be_nested_str_weak(_validate_color_provider_factory_exists),
    /* K26  */  be_nested_str_weak(error),
    /* K27  */  be_nested_str_weak(Color_X20provider_X20factory_X20function_X20_X27_X25s_X27_X20does_X20not_X20exist_X2E_X20Check_X20the_X20function_X20name_X20and_X20ensure_X20it_X27s_X20available_X20in_X20the_X20animation_X20module_X2E),
    /* K28  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2E_X25s_X28engine_X29_X25s),
    /* K29  */  be_nested_str_weak(_create_instance_for_validation),
    /* K30  */  be_nested_str_weak(symbol_table),
    /* K31  */  be_nested_str_weak(_process_named_arguments_for_color_provider),
    /* K32  */  be_nested_str_weak(_X25s_),
    /* K33  */  be_nested_str_weak(process_value),
    /* K34  */  be_nested_str_weak(collect_inline_comment),
    /* K35  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20_X25s_X25s),
    /* K36  */  be_nested_str_weak(string),
    }),
    be_str_weak(process_color),
    &be_const_str_solidified,
    ( &(const binstruction[192]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x880C0507,  //  0010  GETMBR	R3	R2	K7
      0xB8121000,  //  0011  GETNGBL	R4	K8
      0x88100909,  //  0012  GETMBR	R4	R4	K9
      0x8810090A,  //  0013  GETMBR	R4	R4	K10
      0x1C0C0604,  //  0014  EQ	R3	R3	R4
      0x740E0005,  //  0015  JMPT	R3	#001C
      0x880C0507,  //  0016  GETMBR	R3	R2	K7
      0xB8121000,  //  0017  GETNGBL	R4	K8
      0x88100909,  //  0018  GETMBR	R4	R4	K9
      0x8810090B,  //  0019  GETMBR	R4	R4	K11
      0x1C0C0604,  //  001A  EQ	R3	R3	R4
      0x780E0062,  //  001B  JMPF	R3	#007F
      0x8C0C010C,  //  001C  GETMET	R3	R0	K12
      0x7C0C0200,  //  001D  CALL	R3	1
      0x4C100000,  //  001E  LDNIL	R4
      0x200C0604,  //  001F  NE	R3	R3	R4
      0x780E005D,  //  0020  JMPF	R3	#007F
      0x8C0C010C,  //  0021  GETMET	R3	R0	K12
      0x7C0C0200,  //  0022  CALL	R3	1
      0x880C0707,  //  0023  GETMBR	R3	R3	K7
      0xB8121000,  //  0024  GETNGBL	R4	K8
      0x88100909,  //  0025  GETMBR	R4	R4	K9
      0x8810090D,  //  0026  GETMBR	R4	R4	K13
      0x1C0C0604,  //  0027  EQ	R3	R3	R4
      0x780E0055,  //  0028  JMPF	R3	#007F
      0x880C050E,  //  0029  GETMBR	R3	R2	K14
      0x8C100100,  //  002A  GETMET	R4	R0	K0
      0x7C100200,  //  002B  CALL	R4	1
      0x5810000F,  //  002C  LDCONST	R4	K15
      0x8C140106,  //  002D  GETMET	R5	R0	K6
      0x7C140200,  //  002E  CALL	R5	1
      0x4C180000,  //  002F  LDNIL	R6
      0x20140A06,  //  0030  NE	R5	R5	R6
      0x7816000E,  //  0031  JMPF	R5	#0041
      0x8C140106,  //  0032  GETMET	R5	R0	K6
      0x7C140200,  //  0033  CALL	R5	1
      0x88140B07,  //  0034  GETMBR	R5	R5	K7
      0xB81A1000,  //  0035  GETNGBL	R6	K8
      0x88180D09,  //  0036  GETMBR	R6	R6	K9
      0x88180D10,  //  0037  GETMBR	R6	R6	K16
      0x1C140A06,  //  0038  EQ	R5	R5	R6
      0x78160006,  //  0039  JMPF	R5	#0041
      0x8C140106,  //  003A  GETMET	R5	R0	K6
      0x7C140200,  //  003B  CALL	R5	1
      0x88140B0E,  //  003C  GETMBR	R5	R5	K14
      0x00162205,  //  003D  ADD	R5	K17	R5
      0x5C100A00,  //  003E  MOVE	R4	R5
      0x8C140100,  //  003F  GETMET	R5	R0	K0
      0x7C140200,  //  0040  CALL	R5	1
      0x88140112,  //  0041  GETMBR	R5	R0	K18
      0x8C140B13,  //  0042  GETMET	R5	R5	K19
      0x5C1C0600,  //  0043  MOVE	R7	R3
      0x7C140400,  //  0044  CALL	R5	2
      0x78160013,  //  0045  JMPF	R5	#005A
      0x8C140114,  //  0046  GETMET	R5	R0	K20
      0x501C0000,  //  0047  LDBOOL	R7	0	0
      0x7C140400,  //  0048  CALL	R5	2
      0x20180B0F,  //  0049  NE	R6	R5	K15
      0x781A0004,  //  004A  JMPF	R6	#0050
      0x60180018,  //  004B  GETGBL	R6	G24
      0x581C0015,  //  004C  LDCONST	R7	K21
      0x5C200A00,  //  004D  MOVE	R8	R5
      0x7C180400,  //  004E  CALL	R6	2
      0x70020000,  //  004F  JMP		#0051
      0x58180016,  //  0050  LDCONST	R6	K22
      0x8C1C0117,  //  0051  GETMET	R7	R0	K23
      0x60240018,  //  0052  GETGBL	R9	G24
      0x58280018,  //  0053  LDCONST	R10	K24
      0x5C2C0600,  //  0054  MOVE	R11	R3
      0x5C300C00,  //  0055  MOVE	R12	R6
      0x5C340800,  //  0056  MOVE	R13	R4
      0x7C240800,  //  0057  CALL	R9	4
      0x7C1C0400,  //  0058  CALL	R7	2
      0x70020023,  //  0059  JMP		#007E
      0x8C140119,  //  005A  GETMET	R5	R0	K25
      0x5C1C0600,  //  005B  MOVE	R7	R3
      0x7C140400,  //  005C  CALL	R5	2
      0x74160008,  //  005D  JMPT	R5	#0067
      0x8C14011A,  //  005E  GETMET	R5	R0	K26
      0x601C0018,  //  005F  GETGBL	R7	G24
      0x5820001B,  //  0060  LDCONST	R8	K27
      0x5C240600,  //  0061  MOVE	R9	R3
      0x7C1C0400,  //  0062  CALL	R7	2
      0x7C140400,  //  0063  CALL	R5	2
      0x8C140104,  //  0064  GETMET	R5	R0	K4
      0x7C140200,  //  0065  CALL	R5	1
      0x80000A00,  //  0066  RET	0
      0x8C140117,  //  0067  GETMET	R5	R0	K23
      0x601C0018,  //  0068  GETGBL	R7	G24
      0x5820001C,  //  0069  LDCONST	R8	K28
      0x5C240200,  //  006A  MOVE	R9	R1
      0x5C280600,  //  006B  MOVE	R10	R3
      0x5C2C0800,  //  006C  MOVE	R11	R4
      0x7C1C0800,  //  006D  CALL	R7	4
      0x7C140400,  //  006E  CALL	R5	2
      0x8C14011D,  //  006F  GETMET	R5	R0	K29
      0x5C1C0600,  //  0070  MOVE	R7	R3
      0x7C140400,  //  0071  CALL	R5	2
      0x4C180000,  //  0072  LDNIL	R6
      0x20180A06,  //  0073  NE	R6	R5	R6
      0x781A0001,  //  0074  JMPF	R6	#0077
      0x8818011E,  //  0075  GETMBR	R6	R0	K30
      0x98180205,  //  0076  SETIDX	R6	R1	R5
      0x8C18011F,  //  0077  GETMET	R6	R0	K31
      0x60200018,  //  0078  GETGBL	R8	G24
      0x58240020,  //  0079  LDCONST	R9	K32
      0x5C280200,  //  007A  MOVE	R10	R1
      0x7C200400,  //  007B  CALL	R8	2
      0x5C240600,  //  007C  MOVE	R9	R3
      0x7C180600,  //  007D  CALL	R6	3
      0x7002003F,  //  007E  JMP		#00BF
      0x8C0C0106,  //  007F  GETMET	R3	R0	K6
      0x7C0C0200,  //  0080  CALL	R3	1
      0x4C100000,  //  0081  LDNIL	R4
      0x20100604,  //  0082  NE	R4	R3	R4
      0x78120012,  //  0083  JMPF	R4	#0097
      0x88100707,  //  0084  GETMBR	R4	R3	K7
      0xB8161000,  //  0085  GETNGBL	R5	K8
      0x88140B09,  //  0086  GETMBR	R5	R5	K9
      0x88140B0B,  //  0087  GETMBR	R5	R5	K11
      0x1C100805,  //  0088  EQ	R4	R4	R5
      0x7812000C,  //  0089  JMPF	R4	#0097
      0x8C10010C,  //  008A  GETMET	R4	R0	K12
      0x7C100200,  //  008B  CALL	R4	1
      0x4C140000,  //  008C  LDNIL	R5
      0x1C100805,  //  008D  EQ	R4	R4	R5
      0x74120008,  //  008E  JMPT	R4	#0098
      0x8C10010C,  //  008F  GETMET	R4	R0	K12
      0x7C100200,  //  0090  CALL	R4	1
      0x88100907,  //  0091  GETMBR	R4	R4	K7
      0xB8161000,  //  0092  GETNGBL	R5	K8
      0x88140B09,  //  0093  GETMBR	R5	R5	K9
      0x88140B0D,  //  0094  GETMBR	R5	R5	K13
      0x20100805,  //  0095  NE	R4	R4	R5
      0x74120000,  //  0096  JMPT	R4	#0098
      0x50100001,  //  0097  LDBOOL	R4	0	1
      0x50100200,  //  0098  LDBOOL	R4	1	0
      0x78120001,  //  0099  JMPF	R4	#009C
      0x8814070E,  //  009A  GETMBR	R5	R3	K14
      0x70020000,  //  009B  JMP		#009D
      0x4C140000,  //  009C  LDNIL	R5
      0x8C180121,  //  009D  GETMET	R6	R0	K33
      0x58200003,  //  009E  LDCONST	R8	K3
      0x7C180400,  //  009F  CALL	R6	2
      0x8C1C0122,  //  00A0  GETMET	R7	R0	K34
      0x7C1C0200,  //  00A1  CALL	R7	1
      0x8C200117,  //  00A2  GETMET	R8	R0	K23
      0x60280018,  //  00A3  GETGBL	R10	G24
      0x582C0023,  //  00A4  LDCONST	R11	K35
      0x5C300200,  //  00A5  MOVE	R12	R1
      0x5C340C00,  //  00A6  MOVE	R13	R6
      0x5C380E00,  //  00A7  MOVE	R14	R7
      0x7C280800,  //  00A8  CALL	R10	4
      0x7C200400,  //  00A9  CALL	R8	2
      0x78120011,  //  00AA  JMPF	R4	#00BD
      0x4C200000,  //  00AB  LDNIL	R8
      0x20200A08,  //  00AC  NE	R8	R5	R8
      0x7822000E,  //  00AD  JMPF	R8	#00BD
      0x8820011E,  //  00AE  GETMBR	R8	R0	K30
      0x8C201113,  //  00AF  GETMET	R8	R8	K19
      0x5C280A00,  //  00B0  MOVE	R10	R5
      0x7C200400,  //  00B1  CALL	R8	2
      0x78220009,  //  00B2  JMPF	R8	#00BD
      0x8820011E,  //  00B3  GETMBR	R8	R0	K30
      0x94201005,  //  00B4  GETIDX	R8	R8	R5
      0x60240004,  //  00B5  GETGBL	R9	G4
      0x5C281000,  //  00B6  MOVE	R10	R8
      0x7C240200,  //  00B7  CALL	R9	1
      0x20241324,  //  00B8  NE	R9	R9	K36
      0x78260001,  //  00B9  JMPF	R9	#00BC
      0x8824011E,  //  00BA  GETMBR	R9	R0	K30
      0x98240208,  //  00BB  SETIDX	R9	R1	R8
      0x70020001,  //  00BC  JMP		#00BF
      0x8820011E,  //  00BD  GETMBR	R8	R0	K30
      0x98200303,  //  00BE  SETIDX	R8	R1	K3
      0x80000000,  //  00BF  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_standalone_log
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_standalone_log,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_left_paren),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(STRING),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(log_X28_X29_X20function_X20requires_X20a_X20string_X20message),
    /* K9   */  be_nested_str_weak(skip_statement),
    /* K10  */  be_nested_str_weak(value),
    /* K11  */  be_nested_str_weak(expect_right_paren),
    /* K12  */  be_nested_str_weak(collect_inline_comment),
    /* K13  */  be_nested_str_weak(process_log_call),
    /* K14  */  be_nested_str_weak(standalone),
    /* K15  */  be_nested_str_weak(add),
    }),
    be_str_weak(process_standalone_log),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C040102,  //  0004  GETMET	R1	R0	K2
      0x7C040200,  //  0005  CALL	R1	1
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x740A0005,  //  0008  JMPT	R2	#000F
      0x88080303,  //  0009  GETMBR	R2	R1	K3
      0xB80E0800,  //  000A  GETNGBL	R3	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x880C0706,  //  000C  GETMBR	R3	R3	K6
      0x20080403,  //  000D  NE	R2	R2	R3
      0x780A0005,  //  000E  JMPF	R2	#0015
      0x8C080107,  //  000F  GETMET	R2	R0	K7
      0x58100008,  //  0010  LDCONST	R4	K8
      0x7C080400,  //  0011  CALL	R2	2
      0x8C080109,  //  0012  GETMET	R2	R0	K9
      0x7C080200,  //  0013  CALL	R2	1
      0x80000400,  //  0014  RET	0
      0x8808030A,  //  0015  GETMBR	R2	R1	K10
      0x8C0C0100,  //  0016  GETMET	R3	R0	K0
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C010B,  //  0018  GETMET	R3	R0	K11
      0x7C0C0200,  //  0019  CALL	R3	1
      0x8C0C010C,  //  001A  GETMET	R3	R0	K12
      0x7C0C0200,  //  001B  CALL	R3	1
      0x8C10010D,  //  001C  GETMET	R4	R0	K13
      0x5C180400,  //  001D  MOVE	R6	R2
      0x581C000E,  //  001E  LDCONST	R7	K14
      0x5C200600,  //  001F  MOVE	R8	R3
      0x7C100800,  //  0020  CALL	R4	4
      0x8C14010F,  //  0021  GETMET	R5	R0	K15
      0x5C1C0800,  //  0022  MOVE	R7	R4
      0x7C140400,  //  0023  CALL	R5	2
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resolve_symbol_reference
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_resolve_symbol_reference,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(animation_dsl),
    /* K2   */  be_nested_str_weak(is_color_name),
    /* K3   */  be_nested_str_weak(get_named_color_value),
    /* K4   */  be_nested_str_weak(symbol_table),
    /* K5   */  be_nested_str_weak(contains),
    /* K6   */  be_nested_str_weak(sequence_names),
    /* K7   */  be_nested_str_weak(_X25s_),
    /* K8   */  be_nested_str_weak(animation),
    /* K9   */  be_nested_str_weak(animation_X2E_X25s),
    }),
    be_str_weak(resolve_symbol_reference),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0003,  //  0005  JMPF	R3	#000A
      0x8C0C0103,  //  0006  GETMET	R3	R0	K3
      0x5C140200,  //  0007  MOVE	R5	R1
      0x7C0C0400,  //  0008  CALL	R3	2
      0x80040600,  //  0009  RET	1	R3
      0x880C0104,  //  000A  GETMBR	R3	R0	K4
      0x8C0C0705,  //  000B  GETMET	R3	R3	K5
      0x5C140200,  //  000C  MOVE	R5	R1
      0x7C0C0400,  //  000D  CALL	R3	2
      0x740E0004,  //  000E  JMPT	R3	#0014
      0x880C0106,  //  000F  GETMBR	R3	R0	K6
      0x8C0C0705,  //  0010  GETMET	R3	R3	K5
      0x5C140200,  //  0011  MOVE	R5	R1
      0x7C0C0400,  //  0012  CALL	R3	2
      0x780E0005,  //  0013  JMPF	R3	#001A
      0x600C0018,  //  0014  GETGBL	R3	G24
      0x58100007,  //  0015  LDCONST	R4	K7
      0x5C140200,  //  0016  MOVE	R5	R1
      0x7C0C0400,  //  0017  CALL	R3	2
      0x80040600,  //  0018  RET	1	R3
      0x7002000F,  //  0019  JMP		#002A
      0x8C0C0505,  //  001A  GETMET	R3	R2	K5
      0xB8161000,  //  001B  GETNGBL	R5	K8
      0x5C180200,  //  001C  MOVE	R6	R1
      0x7C0C0600,  //  001D  CALL	R3	3
      0x780E0005,  //  001E  JMPF	R3	#0025
      0x600C0018,  //  001F  GETGBL	R3	G24
      0x58100009,  //  0020  LDCONST	R4	K9
      0x5C140200,  //  0021  MOVE	R5	R1
      0x7C0C0400,  //  0022  CALL	R3	2
      0x80040600,  //  0023  RET	1	R3
      0x70020004,  //  0024  JMP		#002A
      0x600C0018,  //  0025  GETGBL	R3	G24
      0x58100007,  //  0026  LDCONST	R4	K7
      0x5C140200,  //  0027  MOVE	R5	R1
      0x7C0C0400,  //  0028  CALL	R3	2
      0x80040600,  //  0029  RET	1	R3
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: skip_whitespace_including_newlines
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_skip_whitespace_including_newlines,   /* name */
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
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(COMMENT),
    /* K6   */  be_nested_str_weak(NEWLINE),
    /* K7   */  be_nested_str_weak(next),
    }),
    be_str_weak(skip_whitespace_including_newlines),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060015,  //  0002  JMPT	R1	#0019
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x4C080000,  //  0005  LDNIL	R2
      0x20080202,  //  0006  NE	R2	R1	R2
      0x780A000E,  //  0007  JMPF	R2	#0017
      0x88080302,  //  0008  GETMBR	R2	R1	K2
      0xB80E0600,  //  0009  GETNGBL	R3	K3
      0x880C0704,  //  000A  GETMBR	R3	R3	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x740A0005,  //  000D  JMPT	R2	#0014
      0x88080302,  //  000E  GETMBR	R2	R1	K2
      0xB80E0600,  //  000F  GETNGBL	R3	K3
      0x880C0704,  //  0010  GETMBR	R3	R3	K4
      0x880C0706,  //  0011  GETMBR	R3	R3	K6
      0x1C080403,  //  0012  EQ	R2	R2	R3
      0x780A0002,  //  0013  JMPF	R2	#0017
      0x8C080107,  //  0014  GETMET	R2	R0	K7
      0x7C080200,  //  0015  CALL	R2	1
      0x70020000,  //  0016  JMP		#0018
      0x70020000,  //  0017  JMP		#0019
      0x7001FFE6,  //  0018  JMP		#0000
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_import
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_import,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(collect_inline_comment),
    /* K3   */  be_nested_str_weak(add),
    /* K4   */  be_nested_str_weak(import_X20_X25s_X20_X25s),
    }),
    be_str_weak(process_import),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x8C0C0103,  //  0006  GETMET	R3	R0	K3
      0x60140018,  //  0007  GETGBL	R5	G24
      0x58180004,  //  0008  LDCONST	R6	K4
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x5C200400,  //  000A  MOVE	R8	R2
      0x7C140600,  //  000B  CALL	R5	3
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_function_arguments
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_function_arguments,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_paren),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(check_right_paren),
    /* K3   */  be_nested_str_weak(skip_whitespace),
    /* K4   */  be_nested_str_weak(process_additive_expression),
    /* K5   */  be_nested_str_weak(argument),
    /* K6   */  be_nested_str_weak(process_value),
    /* K7   */  be_nested_str_weak(push),
    /* K8   */  be_nested_str_weak(current),
    /* K9   */  be_nested_str_weak(type),
    /* K10  */  be_nested_str_weak(animation_dsl),
    /* K11  */  be_nested_str_weak(Token),
    /* K12  */  be_nested_str_weak(COMMA),
    /* K13  */  be_nested_str_weak(next),
    /* K14  */  be_nested_str_weak(error),
    /* K15  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X29_X27_X20in_X20function_X20arguments),
    /* K16  */  be_nested_str_weak(expect_right_paren),
    /* K17  */  be_nested_str_weak(),
    /* K18  */  be_const_int(0),
    /* K19  */  be_const_int(1),
    /* K20  */  be_nested_str_weak(_X2C_X20),
    /* K21  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(process_function_arguments),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x60080012,  //  0002  GETGBL	R2	G18
      0x7C080000,  //  0003  CALL	R2	0
      0x8C0C0101,  //  0004  GETMET	R3	R0	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E0034,  //  0006  JMPT	R3	#003C
      0x8C0C0102,  //  0007  GETMET	R3	R0	K2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x740E0031,  //  0009  JMPT	R3	#003C
      0x8C0C0103,  //  000A  GETMET	R3	R0	K3
      0x7C0C0200,  //  000B  CALL	R3	1
      0x8C0C0102,  //  000C  GETMET	R3	R0	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x780E0000,  //  000E  JMPF	R3	#0010
      0x7002002B,  //  000F  JMP		#003C
      0x4C0C0000,  //  0010  LDNIL	R3
      0x78060006,  //  0011  JMPF	R1	#0019
      0x8C100104,  //  0012  GETMET	R4	R0	K4
      0x58180005,  //  0013  LDCONST	R6	K5
      0x501C0200,  //  0014  LDBOOL	R7	1	0
      0x50200200,  //  0015  LDBOOL	R8	1	0
      0x7C100800,  //  0016  CALL	R4	4
      0x5C0C0800,  //  0017  MOVE	R3	R4
      0x70020003,  //  0018  JMP		#001D
      0x8C100106,  //  0019  GETMET	R4	R0	K6
      0x58180005,  //  001A  LDCONST	R6	K5
      0x7C100400,  //  001B  CALL	R4	2
      0x5C0C0800,  //  001C  MOVE	R3	R4
      0x8C100507,  //  001D  GETMET	R4	R2	K7
      0x5C180600,  //  001E  MOVE	R6	R3
      0x7C100400,  //  001F  CALL	R4	2
      0x8C100103,  //  0020  GETMET	R4	R0	K3
      0x7C100200,  //  0021  CALL	R4	1
      0x8C100108,  //  0022  GETMET	R4	R0	K8
      0x7C100200,  //  0023  CALL	R4	1
      0x4C140000,  //  0024  LDNIL	R5
      0x20100805,  //  0025  NE	R4	R4	R5
      0x7812000C,  //  0026  JMPF	R4	#0034
      0x8C100108,  //  0027  GETMET	R4	R0	K8
      0x7C100200,  //  0028  CALL	R4	1
      0x88100909,  //  0029  GETMBR	R4	R4	K9
      0xB8161400,  //  002A  GETNGBL	R5	K10
      0x88140B0B,  //  002B  GETMBR	R5	R5	K11
      0x88140B0C,  //  002C  GETMBR	R5	R5	K12
      0x1C100805,  //  002D  EQ	R4	R4	R5
      0x78120004,  //  002E  JMPF	R4	#0034
      0x8C10010D,  //  002F  GETMET	R4	R0	K13
      0x7C100200,  //  0030  CALL	R4	1
      0x8C100103,  //  0031  GETMET	R4	R0	K3
      0x7C100200,  //  0032  CALL	R4	1
      0x70020006,  //  0033  JMP		#003B
      0x8C100102,  //  0034  GETMET	R4	R0	K2
      0x7C100200,  //  0035  CALL	R4	1
      0x74120003,  //  0036  JMPT	R4	#003B
      0x8C10010E,  //  0037  GETMET	R4	R0	K14
      0x5818000F,  //  0038  LDCONST	R6	K15
      0x7C100400,  //  0039  CALL	R4	2
      0x70020000,  //  003A  JMP		#003C
      0x7001FFC7,  //  003B  JMP		#0004
      0x8C0C0110,  //  003C  GETMET	R3	R0	K16
      0x7C0C0200,  //  003D  CALL	R3	1
      0x580C0011,  //  003E  LDCONST	R3	K17
      0x60100010,  //  003F  GETGBL	R4	G16
      0x6014000C,  //  0040  GETGBL	R5	G12
      0x5C180400,  //  0041  MOVE	R6	R2
      0x7C140200,  //  0042  CALL	R5	1
      0x04140B13,  //  0043  SUB	R5	R5	K19
      0x40162405,  //  0044  CONNECT	R5	K18	R5
      0x7C100200,  //  0045  CALL	R4	1
      0xA8020007,  //  0046  EXBLK	0	#004F
      0x5C140800,  //  0047  MOVE	R5	R4
      0x7C140000,  //  0048  CALL	R5	0
      0x24180B12,  //  0049  GT	R6	R5	K18
      0x781A0000,  //  004A  JMPF	R6	#004C
      0x000C0714,  //  004B  ADD	R3	R3	K20
      0x94180405,  //  004C  GETIDX	R6	R2	R5
      0x000C0606,  //  004D  ADD	R3	R3	R6
      0x7001FFF7,  //  004E  JMP		#0047
      0x58100015,  //  004F  LDCONST	R4	K21
      0xAC100200,  //  0050  CATCH	R4	1	0
      0xB0080000,  //  0051  RAISE	2	R0	R0
      0x80040600,  //  0052  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_percentage_value
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_percentage_value,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(PERCENTAGE),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(NUMBER),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Expected_X20percentage_X20value),
    }),
    be_str_weak(process_percentage_value),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0015,  //  0004  JMPF	R2	#001B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A000F,  //  000A  JMPF	R2	#001B
      0x88080305,  //  000B  GETMBR	R2	R1	K5
      0x8C0C0106,  //  000C  GETMET	R3	R0	K6
      0x7C0C0200,  //  000D  CALL	R3	1
      0x600C000A,  //  000E  GETGBL	R3	G10
      0x5411FFFD,  //  000F  LDINT	R4	-2
      0x40120E04,  //  0010  CONNECT	R4	K7	R4
      0x94100404,  //  0011  GETIDX	R4	R2	R4
      0x7C0C0200,  //  0012  CALL	R3	1
      0x60100009,  //  0013  GETGBL	R4	G9
      0x541600FE,  //  0014  LDINT	R5	255
      0x08140605,  //  0015  MUL	R5	R3	R5
      0x541A0063,  //  0016  LDINT	R6	100
      0x0C140A06,  //  0017  DIV	R5	R5	R6
      0x7C100200,  //  0018  CALL	R4	1
      0x80040800,  //  0019  RET	1	R4
      0x70020017,  //  001A  JMP		#0033
      0x4C080000,  //  001B  LDNIL	R2
      0x20080202,  //  001C  NE	R2	R1	R2
      0x780A000F,  //  001D  JMPF	R2	#002E
      0x88080301,  //  001E  GETMBR	R2	R1	K1
      0xB80E0400,  //  001F  GETNGBL	R3	K2
      0x880C0703,  //  0020  GETMBR	R3	R3	K3
      0x880C0708,  //  0021  GETMBR	R3	R3	K8
      0x1C080403,  //  0022  EQ	R2	R2	R3
      0x780A0009,  //  0023  JMPF	R2	#002E
      0x88080305,  //  0024  GETMBR	R2	R1	K5
      0x8C0C0106,  //  0025  GETMET	R3	R0	K6
      0x7C0C0200,  //  0026  CALL	R3	1
      0x600C0009,  //  0027  GETGBL	R3	G9
      0x6010000A,  //  0028  GETGBL	R4	G10
      0x5C140400,  //  0029  MOVE	R5	R2
      0x7C100200,  //  002A  CALL	R4	1
      0x7C0C0200,  //  002B  CALL	R3	1
      0x80040600,  //  002C  RET	1	R3
      0x70020004,  //  002D  JMP		#0033
      0x8C080109,  //  002E  GETMET	R2	R0	K9
      0x5810000A,  //  002F  LDCONST	R4	K10
      0x7C080400,  //  0030  CALL	R2	2
      0x540A00FE,  //  0031  LDINT	R2	255
      0x80040400,  //  0032  RET	1	R2
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_left_brace
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_left_brace,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_BRACE),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X7B_X27),
    }),
    be_str_weak(expect_left_brace),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_right_brace
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_check_right_brace,   /* name */
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
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_BRACE),
    }),
    be_str_weak(check_right_brace),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0005,  //  0004  JMPF	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A0000,  //  000A  JMPT	R2	#000C
      0x50080001,  //  000B  LDBOOL	R2	0	1
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence_statement
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence_statement,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[47]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(EOF),
    /* K5   */  be_nested_str_weak(COMMENT),
    /* K6   */  be_nested_str_weak(add),
    /* K7   */  be_nested_str_weak(get_indent),
    /* K8   */  be_nested_str_weak(value),
    /* K9   */  be_nested_str_weak(next),
    /* K10  */  be_nested_str_weak(NEWLINE),
    /* K11  */  be_nested_str_weak(KEYWORD),
    /* K12  */  be_nested_str_weak(play),
    /* K13  */  be_nested_str_weak(process_play_statement_fluent),
    /* K14  */  be_nested_str_weak(wait),
    /* K15  */  be_nested_str_weak(process_wait_statement_fluent),
    /* K16  */  be_nested_str_weak(IDENTIFIER),
    /* K17  */  be_nested_str_weak(log),
    /* K18  */  be_nested_str_weak(process_log_statement_fluent),
    /* K19  */  be_nested_str_weak(restart),
    /* K20  */  be_nested_str_weak(process_restart_statement_fluent),
    /* K21  */  be_nested_str_weak(reset),
    /* K22  */  be_nested_str_weak(error),
    /* K23  */  be_nested_str_weak(_X27reset_X27_X20command_X20is_X20no_X20longer_X20supported_X2E_X20Use_X20_X27restart_X27_X20instead_X2E),
    /* K24  */  be_nested_str_weak(skip_statement),
    /* K25  */  be_nested_str_weak(repeat),
    /* K26  */  be_nested_str_weak(1),
    /* K27  */  be_nested_str_weak(forever),
    /* K28  */  be_nested_str_weak(_X2D1),
    /* K29  */  be_nested_str_weak(process_value),
    /* K30  */  be_nested_str_weak(repeat_count),
    /* K31  */  be_nested_str_weak(expect_keyword),
    /* K32  */  be_nested_str_weak(times),
    /* K33  */  be_nested_str_weak(expect_left_brace),
    /* K34  */  be_nested_str_weak(_X25s_X2Epush_repeat_subsequence_X28animation_X2ESequenceManager_X28engine_X2C_X20_X25s_X29),
    /* K35  */  be_nested_str_weak(indent_level),
    /* K36  */  be_const_int(1),
    /* K37  */  be_nested_str_weak(at_end),
    /* K38  */  be_nested_str_weak(check_right_brace),
    /* K39  */  be_nested_str_weak(process_sequence_statement),
    /* K40  */  be_nested_str_weak(expect_right_brace),
    /* K41  */  be_nested_str_weak(_X25s_X29),
    /* K42  */  be_nested_str_weak(peek),
    /* K43  */  be_nested_str_weak(DOT),
    /* K44  */  be_nested_str_weak(process_sequence_assignment_fluent),
    /* K45  */  be_nested_str_weak(Unknown_X20command_X20_X27_X25s_X27_X20in_X20sequence_X2E_X20Valid_X20sequence_X20commands_X20are_X3A_X20play_X2C_X20wait_X2C_X20repeat_X2C_X20restart_X2C_X20log_X2C_X20or_X20property_X20assignments_X20_X28object_X2Eproperty_X20_X3D_X20value_X29),
    /* K46  */  be_nested_str_weak(Invalid_X20statement_X20in_X20sequence_X2E_X20Expected_X3A_X20play_X2C_X20wait_X2C_X20repeat_X2C_X20restart_X2C_X20log_X2C_X20or_X20property_X20assignments),
    }),
    be_str_weak(process_sequence_statement),
    &be_const_str_solidified,
    ( &(const binstruction[210]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x740A0005,  //  0004  JMPT	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0000,  //  000A  JMPF	R2	#000C
      0x80000400,  //  000B  RET	0
      0x88080301,  //  000C  GETMBR	R2	R1	K1
      0xB80E0400,  //  000D  GETNGBL	R3	K2
      0x880C0703,  //  000E  GETMBR	R3	R3	K3
      0x880C0705,  //  000F  GETMBR	R3	R3	K5
      0x1C080403,  //  0010  EQ	R2	R2	R3
      0x780A0008,  //  0011  JMPF	R2	#001B
      0x8C080106,  //  0012  GETMET	R2	R0	K6
      0x8C100107,  //  0013  GETMET	R4	R0	K7
      0x7C100200,  //  0014  CALL	R4	1
      0x88140308,  //  0015  GETMBR	R5	R1	K8
      0x00100805,  //  0016  ADD	R4	R4	R5
      0x7C080400,  //  0017  CALL	R2	2
      0x8C080109,  //  0018  GETMET	R2	R0	K9
      0x7C080200,  //  0019  CALL	R2	1
      0x80000400,  //  001A  RET	0
      0x88080301,  //  001B  GETMBR	R2	R1	K1
      0xB80E0400,  //  001C  GETNGBL	R3	K2
      0x880C0703,  //  001D  GETMBR	R3	R3	K3
      0x880C070A,  //  001E  GETMBR	R3	R3	K10
      0x1C080403,  //  001F  EQ	R2	R2	R3
      0x780A0002,  //  0020  JMPF	R2	#0024
      0x8C080109,  //  0021  GETMET	R2	R0	K9
      0x7C080200,  //  0022  CALL	R2	1
      0x80000400,  //  0023  RET	0
      0x88080301,  //  0024  GETMBR	R2	R1	K1
      0xB80E0400,  //  0025  GETNGBL	R3	K2
      0x880C0703,  //  0026  GETMBR	R3	R3	K3
      0x880C070B,  //  0027  GETMBR	R3	R3	K11
      0x1C080403,  //  0028  EQ	R2	R2	R3
      0x780A0005,  //  0029  JMPF	R2	#0030
      0x88080308,  //  002A  GETMBR	R2	R1	K8
      0x1C08050C,  //  002B  EQ	R2	R2	K12
      0x780A0002,  //  002C  JMPF	R2	#0030
      0x8C08010D,  //  002D  GETMET	R2	R0	K13
      0x7C080200,  //  002E  CALL	R2	1
      0x700200A0,  //  002F  JMP		#00D1
      0x88080301,  //  0030  GETMBR	R2	R1	K1
      0xB80E0400,  //  0031  GETNGBL	R3	K2
      0x880C0703,  //  0032  GETMBR	R3	R3	K3
      0x880C070B,  //  0033  GETMBR	R3	R3	K11
      0x1C080403,  //  0034  EQ	R2	R2	R3
      0x780A0005,  //  0035  JMPF	R2	#003C
      0x88080308,  //  0036  GETMBR	R2	R1	K8
      0x1C08050E,  //  0037  EQ	R2	R2	K14
      0x780A0002,  //  0038  JMPF	R2	#003C
      0x8C08010F,  //  0039  GETMET	R2	R0	K15
      0x7C080200,  //  003A  CALL	R2	1
      0x70020094,  //  003B  JMP		#00D1
      0x88080301,  //  003C  GETMBR	R2	R1	K1
      0xB80E0400,  //  003D  GETNGBL	R3	K2
      0x880C0703,  //  003E  GETMBR	R3	R3	K3
      0x880C0710,  //  003F  GETMBR	R3	R3	K16
      0x1C080403,  //  0040  EQ	R2	R2	R3
      0x780A0005,  //  0041  JMPF	R2	#0048
      0x88080308,  //  0042  GETMBR	R2	R1	K8
      0x1C080511,  //  0043  EQ	R2	R2	K17
      0x780A0002,  //  0044  JMPF	R2	#0048
      0x8C080112,  //  0045  GETMET	R2	R0	K18
      0x7C080200,  //  0046  CALL	R2	1
      0x70020088,  //  0047  JMP		#00D1
      0x88080301,  //  0048  GETMBR	R2	R1	K1
      0xB80E0400,  //  0049  GETNGBL	R3	K2
      0x880C0703,  //  004A  GETMBR	R3	R3	K3
      0x880C070B,  //  004B  GETMBR	R3	R3	K11
      0x1C080403,  //  004C  EQ	R2	R2	R3
      0x780A0005,  //  004D  JMPF	R2	#0054
      0x88080308,  //  004E  GETMBR	R2	R1	K8
      0x1C080513,  //  004F  EQ	R2	R2	K19
      0x780A0002,  //  0050  JMPF	R2	#0054
      0x8C080114,  //  0051  GETMET	R2	R0	K20
      0x7C080200,  //  0052  CALL	R2	1
      0x7002007C,  //  0053  JMP		#00D1
      0x88080301,  //  0054  GETMBR	R2	R1	K1
      0xB80E0400,  //  0055  GETNGBL	R3	K2
      0x880C0703,  //  0056  GETMBR	R3	R3	K3
      0x880C070B,  //  0057  GETMBR	R3	R3	K11
      0x1C080403,  //  0058  EQ	R2	R2	R3
      0x780A0008,  //  0059  JMPF	R2	#0063
      0x88080308,  //  005A  GETMBR	R2	R1	K8
      0x1C080515,  //  005B  EQ	R2	R2	K21
      0x780A0005,  //  005C  JMPF	R2	#0063
      0x8C080116,  //  005D  GETMET	R2	R0	K22
      0x58100017,  //  005E  LDCONST	R4	K23
      0x7C080400,  //  005F  CALL	R2	2
      0x8C080118,  //  0060  GETMET	R2	R0	K24
      0x7C080200,  //  0061  CALL	R2	1
      0x7002006D,  //  0062  JMP		#00D1
      0x88080301,  //  0063  GETMBR	R2	R1	K1
      0xB80E0400,  //  0064  GETNGBL	R3	K2
      0x880C0703,  //  0065  GETMBR	R3	R3	K3
      0x880C070B,  //  0066  GETMBR	R3	R3	K11
      0x1C080403,  //  0067  EQ	R2	R2	R3
      0x780A0041,  //  0068  JMPF	R2	#00AB
      0x88080308,  //  0069  GETMBR	R2	R1	K8
      0x1C080519,  //  006A  EQ	R2	R2	K25
      0x780A003E,  //  006B  JMPF	R2	#00AB
      0x8C080109,  //  006C  GETMET	R2	R0	K9
      0x7C080200,  //  006D  CALL	R2	1
      0x5808001A,  //  006E  LDCONST	R2	K26
      0x8C0C0100,  //  006F  GETMET	R3	R0	K0
      0x7C0C0200,  //  0070  CALL	R3	1
      0x4C100000,  //  0071  LDNIL	R4
      0x20100604,  //  0072  NE	R4	R3	R4
      0x7812000C,  //  0073  JMPF	R4	#0081
      0x88100701,  //  0074  GETMBR	R4	R3	K1
      0xB8160400,  //  0075  GETNGBL	R5	K2
      0x88140B03,  //  0076  GETMBR	R5	R5	K3
      0x88140B0B,  //  0077  GETMBR	R5	R5	K11
      0x1C100805,  //  0078  EQ	R4	R4	R5
      0x78120006,  //  0079  JMPF	R4	#0081
      0x88100708,  //  007A  GETMBR	R4	R3	K8
      0x1C10091B,  //  007B  EQ	R4	R4	K27
      0x78120003,  //  007C  JMPF	R4	#0081
      0x8C100109,  //  007D  GETMET	R4	R0	K9
      0x7C100200,  //  007E  CALL	R4	1
      0x5808001C,  //  007F  LDCONST	R2	K28
      0x70020006,  //  0080  JMP		#0088
      0x8C10011D,  //  0081  GETMET	R4	R0	K29
      0x5818001E,  //  0082  LDCONST	R6	K30
      0x7C100400,  //  0083  CALL	R4	2
      0x8C14011F,  //  0084  GETMET	R5	R0	K31
      0x581C0020,  //  0085  LDCONST	R7	K32
      0x7C140400,  //  0086  CALL	R5	2
      0x5C080800,  //  0087  MOVE	R2	R4
      0x8C100121,  //  0088  GETMET	R4	R0	K33
      0x7C100200,  //  0089  CALL	R4	1
      0x8C100106,  //  008A  GETMET	R4	R0	K6
      0x60180018,  //  008B  GETGBL	R6	G24
      0x581C0022,  //  008C  LDCONST	R7	K34
      0x8C200107,  //  008D  GETMET	R8	R0	K7
      0x7C200200,  //  008E  CALL	R8	1
      0x5C240400,  //  008F  MOVE	R9	R2
      0x7C180600,  //  0090  CALL	R6	3
      0x7C100400,  //  0091  CALL	R4	2
      0x88100123,  //  0092  GETMBR	R4	R0	K35
      0x00100924,  //  0093  ADD	R4	R4	K36
      0x90024604,  //  0094  SETMBR	R0	K35	R4
      0x8C100125,  //  0095  GETMET	R4	R0	K37
      0x7C100200,  //  0096  CALL	R4	1
      0x74120005,  //  0097  JMPT	R4	#009E
      0x8C100126,  //  0098  GETMET	R4	R0	K38
      0x7C100200,  //  0099  CALL	R4	1
      0x74120002,  //  009A  JMPT	R4	#009E
      0x8C100127,  //  009B  GETMET	R4	R0	K39
      0x7C100200,  //  009C  CALL	R4	1
      0x7001FFF6,  //  009D  JMP		#0095
      0x8C100128,  //  009E  GETMET	R4	R0	K40
      0x7C100200,  //  009F  CALL	R4	1
      0x8C100106,  //  00A0  GETMET	R4	R0	K6
      0x60180018,  //  00A1  GETGBL	R6	G24
      0x581C0029,  //  00A2  LDCONST	R7	K41
      0x8C200107,  //  00A3  GETMET	R8	R0	K7
      0x7C200200,  //  00A4  CALL	R8	1
      0x7C180400,  //  00A5  CALL	R6	2
      0x7C100400,  //  00A6  CALL	R4	2
      0x88100123,  //  00A7  GETMBR	R4	R0	K35
      0x04100924,  //  00A8  SUB	R4	R4	K36
      0x90024604,  //  00A9  SETMBR	R0	K35	R4
      0x70020025,  //  00AA  JMP		#00D1
      0x88080301,  //  00AB  GETMBR	R2	R1	K1
      0xB80E0400,  //  00AC  GETNGBL	R3	K2
      0x880C0703,  //  00AD  GETMBR	R3	R3	K3
      0x880C0710,  //  00AE  GETMBR	R3	R3	K16
      0x1C080403,  //  00AF  EQ	R2	R2	R3
      0x780A0018,  //  00B0  JMPF	R2	#00CA
      0x8C08012A,  //  00B1  GETMET	R2	R0	K42
      0x7C080200,  //  00B2  CALL	R2	1
      0x4C0C0000,  //  00B3  LDNIL	R3
      0x20080403,  //  00B4  NE	R2	R2	R3
      0x780A000A,  //  00B5  JMPF	R2	#00C1
      0x8C08012A,  //  00B6  GETMET	R2	R0	K42
      0x7C080200,  //  00B7  CALL	R2	1
      0x88080501,  //  00B8  GETMBR	R2	R2	K1
      0xB80E0400,  //  00B9  GETNGBL	R3	K2
      0x880C0703,  //  00BA  GETMBR	R3	R3	K3
      0x880C072B,  //  00BB  GETMBR	R3	R3	K43
      0x1C080403,  //  00BC  EQ	R2	R2	R3
      0x780A0002,  //  00BD  JMPF	R2	#00C1
      0x8C08012C,  //  00BE  GETMET	R2	R0	K44
      0x7C080200,  //  00BF  CALL	R2	1
      0x70020007,  //  00C0  JMP		#00C9
      0x8C080116,  //  00C1  GETMET	R2	R0	K22
      0x60100018,  //  00C2  GETGBL	R4	G24
      0x5814002D,  //  00C3  LDCONST	R5	K45
      0x88180308,  //  00C4  GETMBR	R6	R1	K8
      0x7C100400,  //  00C5  CALL	R4	2
      0x7C080400,  //  00C6  CALL	R2	2
      0x8C080118,  //  00C7  GETMET	R2	R0	K24
      0x7C080200,  //  00C8  CALL	R2	1
      0x70020006,  //  00C9  JMP		#00D1
      0x8C080116,  //  00CA  GETMET	R2	R0	K22
      0x60100018,  //  00CB  GETGBL	R4	G24
      0x5814002E,  //  00CC  LDCONST	R5	K46
      0x7C100200,  //  00CD  CALL	R4	1
      0x7C080400,  //  00CE  CALL	R2	2
      0x8C080118,  //  00CF  GETMET	R2	R0	K24
      0x7C080200,  //  00D0  CALL	R2	1
      0x80000000,  //  00D1  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_named_arguments_for_variable
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_named_arguments_for_variable,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_process_named_arguments_unified),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(variable),
    }),
    be_str_weak(process_named_arguments_for_variable),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x58140001,  //  0002  LDCONST	R5	K1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C080800,  //  0004  CALL	R2	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_add,   /* name */
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
    /* K0   */  be_nested_str_weak(output),
    /* K1   */  be_nested_str_weak(push),
    }),
    be_str_weak(add),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_errors
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_errors,   /* name */
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
    /* K0   */  be_nested_str_weak(errors),
    }),
    be_str_weak(get_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_nested_function_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_nested_function_call,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(IDENTIFIER),
    /* K6   */  be_nested_str_weak(KEYWORD),
    /* K7   */  be_nested_str_weak(value),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Expected_X20function_X20name),
    /* K11  */  be_nested_str_weak(nil),
    /* K12  */  be_nested_str_weak(is_math_method),
    /* K13  */  be_nested_str_weak(process_function_arguments),
    /* K14  */  be_nested_str_weak(animation_X2E_math_X2E_X25s_X28_X25s_X29),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(process_log_call),
    /* K17  */  be_nested_str_weak(expression),
    /* K18  */  be_nested_str_weak(template_definitions),
    /* K19  */  be_nested_str_weak(contains),
    /* K20  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K21  */  be_nested_str_weak(engine),
    /* K22  */  be_nested_str_weak(_X25s_template_X28_X25s_X29),
    /* K23  */  be_nested_str_weak(_is_simple_function_call),
    /* K24  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X2C_X20_X25s_X29),
    /* K25  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X29),
    /* K26  */  be_nested_str_weak(_validate_animation_factory_exists),
    /* K27  */  be_nested_str_weak(Animation_X20factory_X20function_X20_X27_X25s_X27_X20does_X20not_X20exist_X2E_X20Check_X20the_X20function_X20name_X20and_X20ensure_X20it_X27s_X20available_X20in_X20the_X20animation_X20module_X2E),
    /* K28  */  be_nested_str_weak(skip_function_arguments),
    /* K29  */  be_nested_str_weak(_generate_anonymous_function_call),
    }),
    be_str_weak(process_nested_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[113]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58080001,  //  0002  LDCONST	R2	K1
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0203,  //  0004  NE	R3	R1	R3
      0x780E000F,  //  0005  JMPF	R3	#0016
      0x880C0302,  //  0006  GETMBR	R3	R1	K2
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x88100904,  //  0008  GETMBR	R4	R4	K4
      0x88100905,  //  0009  GETMBR	R4	R4	K5
      0x1C0C0604,  //  000A  EQ	R3	R3	R4
      0x740E0005,  //  000B  JMPT	R3	#0012
      0x880C0302,  //  000C  GETMBR	R3	R1	K2
      0xB8120600,  //  000D  GETNGBL	R4	K3
      0x88100904,  //  000E  GETMBR	R4	R4	K4
      0x88100906,  //  000F  GETMBR	R4	R4	K6
      0x1C0C0604,  //  0010  EQ	R3	R3	R4
      0x780E0003,  //  0011  JMPF	R3	#0016
      0x88080307,  //  0012  GETMBR	R2	R1	K7
      0x8C0C0108,  //  0013  GETMET	R3	R0	K8
      0x7C0C0200,  //  0014  CALL	R3	1
      0x70020003,  //  0015  JMP		#001A
      0x8C0C0109,  //  0016  GETMET	R3	R0	K9
      0x5814000A,  //  0017  LDCONST	R5	K10
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80061600,  //  0019  RET	1	K11
      0x8C0C010C,  //  001A  GETMET	R3	R0	K12
      0x5C140400,  //  001B  MOVE	R5	R2
      0x7C0C0400,  //  001C  CALL	R3	2
      0x780E0008,  //  001D  JMPF	R3	#0027
      0x8C0C010D,  //  001E  GETMET	R3	R0	K13
      0x50140200,  //  001F  LDBOOL	R5	1	0
      0x7C0C0400,  //  0020  CALL	R3	2
      0x60100018,  //  0021  GETGBL	R4	G24
      0x5814000E,  //  0022  LDCONST	R5	K14
      0x5C180400,  //  0023  MOVE	R6	R2
      0x5C1C0600,  //  0024  MOVE	R7	R3
      0x7C100600,  //  0025  CALL	R4	3
      0x80040800,  //  0026  RET	1	R4
      0x1C0C050F,  //  0027  EQ	R3	R2	K15
      0x780E0008,  //  0028  JMPF	R3	#0032
      0x8C0C010D,  //  0029  GETMET	R3	R0	K13
      0x50140200,  //  002A  LDBOOL	R5	1	0
      0x7C0C0400,  //  002B  CALL	R3	2
      0x8C100110,  //  002C  GETMET	R4	R0	K16
      0x5C180600,  //  002D  MOVE	R6	R3
      0x581C0011,  //  002E  LDCONST	R7	K17
      0x58200001,  //  002F  LDCONST	R8	K1
      0x7C100800,  //  0030  CALL	R4	4
      0x80040800,  //  0031  RET	1	R4
      0x880C0112,  //  0032  GETMBR	R3	R0	K18
      0x8C0C0713,  //  0033  GETMET	R3	R3	K19
      0x5C140400,  //  0034  MOVE	R5	R2
      0x7C0C0400,  //  0035  CALL	R3	2
      0x780E0011,  //  0036  JMPF	R3	#0049
      0x8C0C010D,  //  0037  GETMET	R3	R0	K13
      0x50140200,  //  0038  LDBOOL	R5	1	0
      0x7C0C0400,  //  0039  CALL	R3	2
      0x20100701,  //  003A  NE	R4	R3	K1
      0x78120004,  //  003B  JMPF	R4	#0041
      0x60100018,  //  003C  GETGBL	R4	G24
      0x58140014,  //  003D  LDCONST	R5	K20
      0x5C180600,  //  003E  MOVE	R6	R3
      0x7C100400,  //  003F  CALL	R4	2
      0x70020000,  //  0040  JMP		#0042
      0x58100015,  //  0041  LDCONST	R4	K21
      0x60140018,  //  0042  GETGBL	R5	G24
      0x58180016,  //  0043  LDCONST	R6	K22
      0x5C1C0400,  //  0044  MOVE	R7	R2
      0x5C200800,  //  0045  MOVE	R8	R4
      0x7C140600,  //  0046  CALL	R5	3
      0x80040A00,  //  0047  RET	1	R5
      0x70020026,  //  0048  JMP		#0070
      0x8C0C0117,  //  0049  GETMET	R3	R0	K23
      0x5C140400,  //  004A  MOVE	R5	R2
      0x7C0C0400,  //  004B  CALL	R3	2
      0x780E0011,  //  004C  JMPF	R3	#005F
      0x8C0C010D,  //  004D  GETMET	R3	R0	K13
      0x50140000,  //  004E  LDBOOL	R5	0	0
      0x7C0C0400,  //  004F  CALL	R3	2
      0x20100701,  //  0050  NE	R4	R3	K1
      0x78120006,  //  0051  JMPF	R4	#0059
      0x60100018,  //  0052  GETGBL	R4	G24
      0x58140018,  //  0053  LDCONST	R5	K24
      0x5C180400,  //  0054  MOVE	R6	R2
      0x5C1C0600,  //  0055  MOVE	R7	R3
      0x7C100600,  //  0056  CALL	R4	3
      0x80040800,  //  0057  RET	1	R4
      0x70020004,  //  0058  JMP		#005E
      0x60100018,  //  0059  GETGBL	R4	G24
      0x58140019,  //  005A  LDCONST	R5	K25
      0x5C180400,  //  005B  MOVE	R6	R2
      0x7C100400,  //  005C  CALL	R4	2
      0x80040800,  //  005D  RET	1	R4
      0x70020010,  //  005E  JMP		#0070
      0x8C0C011A,  //  005F  GETMET	R3	R0	K26
      0x5C140400,  //  0060  MOVE	R5	R2
      0x7C0C0400,  //  0061  CALL	R3	2
      0x740E0008,  //  0062  JMPT	R3	#006C
      0x8C0C0109,  //  0063  GETMET	R3	R0	K9
      0x60140018,  //  0064  GETGBL	R5	G24
      0x5818001B,  //  0065  LDCONST	R6	K27
      0x5C1C0400,  //  0066  MOVE	R7	R2
      0x7C140400,  //  0067  CALL	R5	2
      0x7C0C0400,  //  0068  CALL	R3	2
      0x8C0C011C,  //  0069  GETMET	R3	R0	K28
      0x7C0C0200,  //  006A  CALL	R3	1
      0x80061600,  //  006B  RET	1	K11
      0x8C0C011D,  //  006C  GETMET	R3	R0	K29
      0x5C140400,  //  006D  MOVE	R5	R2
      0x7C0C0400,  //  006E  CALL	R3	2
      0x80040600,  //  006F  RET	1	R3
      0x80000000,  //  0070  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(tokens),
    /* K1   */  be_nested_str_weak(pos),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(output),
    /* K4   */  be_nested_str_weak(errors),
    /* K5   */  be_nested_str_weak(run_statements),
    /* K6   */  be_nested_str_weak(first_statement),
    /* K7   */  be_nested_str_weak(strip_initialized),
    /* K8   */  be_nested_str_weak(sequence_names),
    /* K9   */  be_nested_str_weak(symbol_table),
    /* K10  */  be_nested_str_weak(indent_level),
    /* K11  */  be_nested_str_weak(template_definitions),
    /* K12  */  be_nested_str_weak(has_template_calls),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x5C080200,  //  0003  MOVE	R2	R1
      0x70020001,  //  0004  JMP		#0007
      0x60080012,  //  0005  GETGBL	R2	G18
      0x7C080000,  //  0006  CALL	R2	0
      0x90020002,  //  0007  SETMBR	R0	K0	R2
      0x90020302,  //  0008  SETMBR	R0	K1	K2
      0x60080012,  //  0009  GETGBL	R2	G18
      0x7C080000,  //  000A  CALL	R2	0
      0x90020602,  //  000B  SETMBR	R0	K3	R2
      0x60080012,  //  000C  GETGBL	R2	G18
      0x7C080000,  //  000D  CALL	R2	0
      0x90020802,  //  000E  SETMBR	R0	K4	R2
      0x60080012,  //  000F  GETGBL	R2	G18
      0x7C080000,  //  0010  CALL	R2	0
      0x90020A02,  //  0011  SETMBR	R0	K5	R2
      0x50080200,  //  0012  LDBOOL	R2	1	0
      0x90020C02,  //  0013  SETMBR	R0	K6	R2
      0x50080000,  //  0014  LDBOOL	R2	0	0
      0x90020E02,  //  0015  SETMBR	R0	K7	R2
      0x60080013,  //  0016  GETGBL	R2	G19
      0x7C080000,  //  0017  CALL	R2	0
      0x90021002,  //  0018  SETMBR	R0	K8	R2
      0x60080013,  //  0019  GETGBL	R2	G19
      0x7C080000,  //  001A  CALL	R2	0
      0x90021202,  //  001B  SETMBR	R0	K9	R2
      0x90021502,  //  001C  SETMBR	R0	K10	K2
      0x60080013,  //  001D  GETGBL	R2	G19
      0x7C080000,  //  001E  CALL	R2	0
      0x90021602,  //  001F  SETMBR	R0	K11	R2
      0x50080000,  //  0020  LDBOOL	R2	0	0
      0x90021802,  //  0021  SETMBR	R0	K12	R2
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_value_provider_reference
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_value_provider_reference,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_exists),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Undefined_X20reference_X20_X27_X25s_X27_X20in_X20_X25s_X20statement_X2E_X20Make_X20sure_X20the_X20value_X20provider_X20or_X20animation_X20is_X20defined_X20before_X20use_X2E),
    /* K3   */  be_nested_str_weak(symbol_table),
    /* K4   */  be_nested_str_weak(contains),
    /* K5   */  be_nested_str_weak(string),
    /* K6   */  be_nested_str_weak(value_provider),
    /* K7   */  be_nested_str_weak(animation),
    /* K8   */  be_nested_str_weak(_X27_X25s_X27_X20in_X20_X25s_X20statement_X20is_X20not_X20a_X20value_X20provider_X20or_X20animation_X2E_X20Only_X20value_X20providers_X20_X28like_X20oscillators_X29_X20and_X20animations_X20can_X20be_X20restarted_X2E),
    /* K9   */  be_nested_str_weak(Could_X20not_X20validate_X20_X27_X25s_X27_X20in_X20_X25s_X20statement_X3A_X20_X25s),
    }),
    be_str_weak(_validate_value_provider_reference),
    &be_const_str_solidified,
    ( &(const binstruction[97]) {  /* code */
      0xA8020050,  //  0000  EXBLK	0	#0052
      0x8C0C0100,  //  0001  GETMET	R3	R0	K0
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0009,  //  0004  JMPT	R3	#000F
      0x8C0C0101,  //  0005  GETMET	R3	R0	K1
      0x60140018,  //  0006  GETGBL	R5	G24
      0x58180002,  //  0007  LDCONST	R6	K2
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x5C200400,  //  0009  MOVE	R8	R2
      0x7C140600,  //  000A  CALL	R5	3
      0x7C0C0400,  //  000B  CALL	R3	2
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0xA8040001,  //  000D  EXBLK	1	1
      0x80040600,  //  000E  RET	1	R3
      0x880C0103,  //  000F  GETMBR	R3	R0	K3
      0x8C0C0704,  //  0010  GETMET	R3	R3	K4
      0x5C140200,  //  0011  MOVE	R5	R1
      0x7C0C0400,  //  0012  CALL	R3	2
      0x780E0038,  //  0013  JMPF	R3	#004D
      0x880C0103,  //  0014  GETMBR	R3	R0	K3
      0x940C0601,  //  0015  GETIDX	R3	R3	R1
      0x60100004,  //  0016  GETGBL	R4	G4
      0x5C140600,  //  0017  MOVE	R5	R3
      0x7C100200,  //  0018  CALL	R4	1
      0x1C100905,  //  0019  EQ	R4	R4	K5
      0x78120007,  //  001A  JMPF	R4	#0023
      0x1C100706,  //  001B  EQ	R4	R3	K6
      0x74120001,  //  001C  JMPT	R4	#001F
      0x1C100707,  //  001D  EQ	R4	R3	K7
      0x78120003,  //  001E  JMPF	R4	#0023
      0x50100200,  //  001F  LDBOOL	R4	1	0
      0xA8040001,  //  0020  EXBLK	1	1
      0x80040800,  //  0021  RET	1	R4
      0x70020029,  //  0022  JMP		#004D
      0x60100004,  //  0023  GETGBL	R4	G4
      0x5C140600,  //  0024  MOVE	R5	R3
      0x7C100200,  //  0025  CALL	R4	1
      0x1C100905,  //  0026  EQ	R4	R4	K5
      0x7812000A,  //  0027  JMPF	R4	#0033
      0x8C100101,  //  0028  GETMET	R4	R0	K1
      0x60180018,  //  0029  GETGBL	R6	G24
      0x581C0008,  //  002A  LDCONST	R7	K8
      0x5C200200,  //  002B  MOVE	R8	R1
      0x5C240400,  //  002C  MOVE	R9	R2
      0x7C180600,  //  002D  CALL	R6	3
      0x7C100400,  //  002E  CALL	R4	2
      0x50100000,  //  002F  LDBOOL	R4	0	0
      0xA8040001,  //  0030  EXBLK	1	1
      0x80040800,  //  0031  RET	1	R4
      0x70020019,  //  0032  JMP		#004D
      0x6010000F,  //  0033  GETGBL	R4	G15
      0x5C140600,  //  0034  MOVE	R5	R3
      0xB81A0E00,  //  0035  GETNGBL	R6	K7
      0x88180D06,  //  0036  GETMBR	R6	R6	K6
      0x7C100400,  //  0037  CALL	R4	2
      0x74120005,  //  0038  JMPT	R4	#003F
      0x6010000F,  //  0039  GETGBL	R4	G15
      0x5C140600,  //  003A  MOVE	R5	R3
      0xB81A0E00,  //  003B  GETNGBL	R6	K7
      0x88180D07,  //  003C  GETMBR	R6	R6	K7
      0x7C100400,  //  003D  CALL	R4	2
      0x78120003,  //  003E  JMPF	R4	#0043
      0x50100200,  //  003F  LDBOOL	R4	1	0
      0xA8040001,  //  0040  EXBLK	1	1
      0x80040800,  //  0041  RET	1	R4
      0x70020009,  //  0042  JMP		#004D
      0x8C100101,  //  0043  GETMET	R4	R0	K1
      0x60180018,  //  0044  GETGBL	R6	G24
      0x581C0008,  //  0045  LDCONST	R7	K8
      0x5C200200,  //  0046  MOVE	R8	R1
      0x5C240400,  //  0047  MOVE	R9	R2
      0x7C180600,  //  0048  CALL	R6	3
      0x7C100400,  //  0049  CALL	R4	2
      0x50100000,  //  004A  LDBOOL	R4	0	0
      0xA8040001,  //  004B  EXBLK	1	1
      0x80040800,  //  004C  RET	1	R4
      0x500C0200,  //  004D  LDBOOL	R3	1	0
      0xA8040001,  //  004E  EXBLK	1	1
      0x80040600,  //  004F  RET	1	R3
      0xA8040001,  //  0050  EXBLK	1	1
      0x7002000D,  //  0051  JMP		#0060
      0xAC0C0002,  //  0052  CATCH	R3	0	2
      0x7002000A,  //  0053  JMP		#005F
      0x8C140101,  //  0054  GETMET	R5	R0	K1
      0x601C0018,  //  0055  GETGBL	R7	G24
      0x58200009,  //  0056  LDCONST	R8	K9
      0x5C240200,  //  0057  MOVE	R9	R1
      0x5C280400,  //  0058  MOVE	R10	R2
      0x5C2C0800,  //  0059  MOVE	R11	R4
      0x7C1C0800,  //  005A  CALL	R7	4
      0x7C140400,  //  005B  CALL	R5	2
      0x50140000,  //  005C  LDBOOL	R5	0	0
      0x80040A00,  //  005D  RET	1	R5
      0x70020000,  //  005E  JMP		#0060
      0xB0080000,  //  005F  RAISE	2	R0	R0
      0x80000000,  //  0060  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_left_paren
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_left_paren,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_PAREN),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X28_X27),
    }),
    be_str_weak(expect_left_paren),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_computed_expression_string
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_is_computed_expression_string,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(_X20_X2B_X20),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(_X20_X2D_X20),
    /* K5   */  be_nested_str_weak(_X20_X2A_X20),
    /* K6   */  be_nested_str_weak(_X20_X2F_X20),
    /* K7   */  be_nested_str_weak(_X28),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(is_identifier_char),
    /* K10  */  be_nested_str_weak(_is_simple_function_call),
    }),
    be_str_weak(is_computed_expression_string),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x280C0703,  //  0005  GE	R3	R3	K3
      0x740E0012,  //  0006  JMPT	R3	#001A
      0x8C0C0501,  //  0007  GETMET	R3	R2	K1
      0x5C140200,  //  0008  MOVE	R5	R1
      0x58180004,  //  0009  LDCONST	R6	K4
      0x7C0C0600,  //  000A  CALL	R3	3
      0x280C0703,  //  000B  GE	R3	R3	K3
      0x740E000C,  //  000C  JMPT	R3	#001A
      0x8C0C0501,  //  000D  GETMET	R3	R2	K1
      0x5C140200,  //  000E  MOVE	R5	R1
      0x58180005,  //  000F  LDCONST	R6	K5
      0x7C0C0600,  //  0010  CALL	R3	3
      0x280C0703,  //  0011  GE	R3	R3	K3
      0x740E0006,  //  0012  JMPT	R3	#001A
      0x8C0C0501,  //  0013  GETMET	R3	R2	K1
      0x5C140200,  //  0014  MOVE	R5	R1
      0x58180006,  //  0015  LDCONST	R6	K6
      0x7C0C0600,  //  0016  CALL	R3	3
      0x280C0703,  //  0017  GE	R3	R3	K3
      0x740E0000,  //  0018  JMPT	R3	#001A
      0x500C0001,  //  0019  LDBOOL	R3	0	1
      0x500C0200,  //  001A  LDBOOL	R3	1	0
      0x50100000,  //  001B  LDBOOL	R4	0	0
      0x8C140501,  //  001C  GETMET	R5	R2	K1
      0x5C1C0200,  //  001D  MOVE	R7	R1
      0x58200007,  //  001E  LDCONST	R8	K7
      0x7C140600,  //  001F  CALL	R5	3
      0x24180B03,  //  0020  GT	R6	R5	K3
      0x781A0017,  //  0021  JMPF	R6	#003A
      0x04180B08,  //  0022  SUB	R6	R5	K8
      0x94180206,  //  0023  GETIDX	R6	R1	R6
      0x8C1C0109,  //  0024  GETMET	R7	R0	K9
      0x5C240C00,  //  0025  MOVE	R9	R6
      0x7C1C0400,  //  0026  CALL	R7	2
      0x781E0011,  //  0027  JMPF	R7	#003A
      0x041C0B08,  //  0028  SUB	R7	R5	K8
      0x28200F03,  //  0029  GE	R8	R7	K3
      0x78220005,  //  002A  JMPF	R8	#0031
      0x8C200109,  //  002B  GETMET	R8	R0	K9
      0x94280207,  //  002C  GETIDX	R10	R1	R7
      0x7C200400,  //  002D  CALL	R8	2
      0x78220001,  //  002E  JMPF	R8	#0031
      0x041C0F08,  //  002F  SUB	R7	R7	K8
      0x7001FFF7,  //  0030  JMP		#0029
      0x001C0F08,  //  0031  ADD	R7	R7	K8
      0x04200B08,  //  0032  SUB	R8	R5	K8
      0x40200E08,  //  0033  CONNECT	R8	R7	R8
      0x94200208,  //  0034  GETIDX	R8	R1	R8
      0x8C24010A,  //  0035  GETMET	R9	R0	K10
      0x5C2C1000,  //  0036  MOVE	R11	R8
      0x7C240400,  //  0037  CALL	R9	2
      0x74260000,  //  0038  JMPT	R9	#003A
      0x50100200,  //  0039  LDBOOL	R4	1	0
      0x740E0001,  //  003A  JMPT	R3	#003D
      0x74120000,  //  003B  JMPT	R4	#003D
      0x50180001,  //  003C  LDBOOL	R6	0	1
      0x50180200,  //  003D  LDBOOL	R6	1	0
      0x80040C00,  //  003E  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: collect_inline_comment
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_collect_inline_comment,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(COMMENT),
    /* K5   */  be_nested_str_weak(_X20_X20),
    /* K6   */  be_nested_str_weak(value),
    /* K7   */  be_nested_str_weak(next),
    /* K8   */  be_nested_str_weak(),
    }),
    be_str_weak(collect_inline_comment),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A000A,  //  0004  JMPF	R2	#0010
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0004,  //  000A  JMPF	R2	#0010
      0x88080306,  //  000B  GETMBR	R2	R1	K6
      0x000A0A02,  //  000C  ADD	R2	K5	R2
      0x8C0C0107,  //  000D  GETMET	R3	R0	K7
      0x7C0C0200,  //  000E  CALL	R3	1
      0x80040400,  //  000F  RET	1	R2
      0x80061000,  //  0010  RET	1	K8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_right_brace
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_right_brace,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_BRACE),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X7D_X27),
    }),
    be_str_weak(expect_right_brace),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(sequence),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(sequence_names),
    /* K6   */  be_nested_str_weak(symbol_table),
    /* K7   */  be_nested_str_weak(1),
    /* K8   */  be_nested_str_weak(current),
    /* K9   */  be_nested_str_weak(type),
    /* K10  */  be_nested_str_weak(animation_dsl),
    /* K11  */  be_nested_str_weak(Token),
    /* K12  */  be_nested_str_weak(KEYWORD),
    /* K13  */  be_nested_str_weak(value),
    /* K14  */  be_nested_str_weak(repeat),
    /* K15  */  be_nested_str_weak(forever),
    /* K16  */  be_nested_str_weak(_X2D1),
    /* K17  */  be_nested_str_weak(process_value),
    /* K18  */  be_nested_str_weak(repeat_count),
    /* K19  */  be_nested_str_weak(expect_keyword),
    /* K20  */  be_nested_str_weak(times),
    /* K21  */  be_nested_str_weak(NUMBER),
    /* K22  */  be_nested_str_weak(expect_left_brace),
    /* K23  */  be_nested_str_weak(add),
    /* K24  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2ESequenceManager_X28engine_X2C_X20_X25s_X29),
    /* K25  */  be_nested_str_weak(at_end),
    /* K26  */  be_nested_str_weak(check_right_brace),
    /* K27  */  be_nested_str_weak(process_sequence_statement),
    /* K28  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2ESequenceManager_X28engine_X29),
    /* K29  */  be_nested_str_weak(expect_right_brace),
    }),
    be_str_weak(process_sequence),
    &be_const_str_solidified,
    ( &(const binstruction[125]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x88080105,  //  000C  GETMBR	R2	R0	K5
      0x500C0200,  //  000D  LDBOOL	R3	1	0
      0x98080203,  //  000E  SETIDX	R2	R1	R3
      0x88080106,  //  000F  GETMBR	R2	R0	K6
      0x98080303,  //  0010  SETIDX	R2	R1	K3
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x580C0007,  //  0012  LDCONST	R3	K7
      0x8C100108,  //  0013  GETMET	R4	R0	K8
      0x7C100200,  //  0014  CALL	R4	1
      0x4C140000,  //  0015  LDNIL	R5
      0x20140805,  //  0016  NE	R5	R4	R5
      0x7816002D,  //  0017  JMPF	R5	#0046
      0x88140909,  //  0018  GETMBR	R5	R4	K9
      0xB81A1400,  //  0019  GETNGBL	R6	K10
      0x88180D0B,  //  001A  GETMBR	R6	R6	K11
      0x88180D0C,  //  001B  GETMBR	R6	R6	K12
      0x1C140A06,  //  001C  EQ	R5	R5	R6
      0x78160027,  //  001D  JMPF	R5	#0046
      0x8814090D,  //  001E  GETMBR	R5	R4	K13
      0x1C140B0E,  //  001F  EQ	R5	R5	K14
      0x7816001C,  //  0020  JMPF	R5	#003E
      0x50080200,  //  0021  LDBOOL	R2	1	0
      0x8C140100,  //  0022  GETMET	R5	R0	K0
      0x7C140200,  //  0023  CALL	R5	1
      0x8C140108,  //  0024  GETMET	R5	R0	K8
      0x7C140200,  //  0025  CALL	R5	1
      0x4C180000,  //  0026  LDNIL	R6
      0x20180A06,  //  0027  NE	R6	R5	R6
      0x781A000C,  //  0028  JMPF	R6	#0036
      0x88180B09,  //  0029  GETMBR	R6	R5	K9
      0xB81E1400,  //  002A  GETNGBL	R7	K10
      0x881C0F0B,  //  002B  GETMBR	R7	R7	K11
      0x881C0F0C,  //  002C  GETMBR	R7	R7	K12
      0x1C180C07,  //  002D  EQ	R6	R6	R7
      0x781A0006,  //  002E  JMPF	R6	#0036
      0x88180B0D,  //  002F  GETMBR	R6	R5	K13
      0x1C180D0F,  //  0030  EQ	R6	R6	K15
      0x781A0003,  //  0031  JMPF	R6	#0036
      0x8C180100,  //  0032  GETMET	R6	R0	K0
      0x7C180200,  //  0033  CALL	R6	1
      0x580C0010,  //  0034  LDCONST	R3	K16
      0x70020006,  //  0035  JMP		#003D
      0x8C180111,  //  0036  GETMET	R6	R0	K17
      0x58200012,  //  0037  LDCONST	R8	K18
      0x7C180400,  //  0038  CALL	R6	2
      0x8C1C0113,  //  0039  GETMET	R7	R0	K19
      0x58240014,  //  003A  LDCONST	R9	K20
      0x7C1C0400,  //  003B  CALL	R7	2
      0x5C0C0C00,  //  003C  MOVE	R3	R6
      0x70020006,  //  003D  JMP		#0045
      0x8814090D,  //  003E  GETMBR	R5	R4	K13
      0x1C140B0F,  //  003F  EQ	R5	R5	K15
      0x78160003,  //  0040  JMPF	R5	#0045
      0x50080200,  //  0041  LDBOOL	R2	1	0
      0x8C140100,  //  0042  GETMET	R5	R0	K0
      0x7C140200,  //  0043  CALL	R5	1
      0x580C0010,  //  0044  LDCONST	R3	K16
      0x70020010,  //  0045  JMP		#0057
      0x4C140000,  //  0046  LDNIL	R5
      0x20140805,  //  0047  NE	R5	R4	R5
      0x7816000D,  //  0048  JMPF	R5	#0057
      0x88140909,  //  0049  GETMBR	R5	R4	K9
      0xB81A1400,  //  004A  GETNGBL	R6	K10
      0x88180D0B,  //  004B  GETMBR	R6	R6	K11
      0x88180D15,  //  004C  GETMBR	R6	R6	K21
      0x1C140A06,  //  004D  EQ	R5	R5	R6
      0x78160007,  //  004E  JMPF	R5	#0057
      0x50080200,  //  004F  LDBOOL	R2	1	0
      0x8C140111,  //  0050  GETMET	R5	R0	K17
      0x581C0012,  //  0051  LDCONST	R7	K18
      0x7C140400,  //  0052  CALL	R5	2
      0x8C180113,  //  0053  GETMET	R6	R0	K19
      0x58200014,  //  0054  LDCONST	R8	K20
      0x7C180400,  //  0055  CALL	R6	2
      0x5C0C0A00,  //  0056  MOVE	R3	R5
      0x8C140116,  //  0057  GETMET	R5	R0	K22
      0x7C140200,  //  0058  CALL	R5	1
      0x780A0010,  //  0059  JMPF	R2	#006B
      0x8C140117,  //  005A  GETMET	R5	R0	K23
      0x601C0018,  //  005B  GETGBL	R7	G24
      0x58200018,  //  005C  LDCONST	R8	K24
      0x5C240200,  //  005D  MOVE	R9	R1
      0x5C280600,  //  005E  MOVE	R10	R3
      0x7C1C0600,  //  005F  CALL	R7	3
      0x7C140400,  //  0060  CALL	R5	2
      0x8C140119,  //  0061  GETMET	R5	R0	K25
      0x7C140200,  //  0062  CALL	R5	1
      0x74160005,  //  0063  JMPT	R5	#006A
      0x8C14011A,  //  0064  GETMET	R5	R0	K26
      0x7C140200,  //  0065  CALL	R5	1
      0x74160002,  //  0066  JMPT	R5	#006A
      0x8C14011B,  //  0067  GETMET	R5	R0	K27
      0x7C140200,  //  0068  CALL	R5	1
      0x7001FFF6,  //  0069  JMP		#0061
      0x7002000E,  //  006A  JMP		#007A
      0x8C140117,  //  006B  GETMET	R5	R0	K23
      0x601C0018,  //  006C  GETGBL	R7	G24
      0x5820001C,  //  006D  LDCONST	R8	K28
      0x5C240200,  //  006E  MOVE	R9	R1
      0x7C1C0400,  //  006F  CALL	R7	2
      0x7C140400,  //  0070  CALL	R5	2
      0x8C140119,  //  0071  GETMET	R5	R0	K25
      0x7C140200,  //  0072  CALL	R5	1
      0x74160005,  //  0073  JMPT	R5	#007A
      0x8C14011A,  //  0074  GETMET	R5	R0	K26
      0x7C140200,  //  0075  CALL	R5	1
      0x74160002,  //  0076  JMPT	R5	#007A
      0x8C14011B,  //  0077  GETMET	R5	R0	K27
      0x7C140200,  //  0078  CALL	R5	1
      0x7001FFF6,  //  0079  JMP		#0071
      0x8C14011D,  //  007A  GETMET	R5	R0	K29
      0x7C140200,  //  007B  CALL	R5	1
      0x80000000,  //  007C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: transpile_template_body
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_transpile_template_body,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(process_statement),
    /* K2   */  be_nested_str_weak(run_statements),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(name),
    /* K5   */  be_nested_str_weak(comment),
    /* K6   */  be_nested_str_weak(add),
    /* K7   */  be_nested_str_weak(engine_X2Eadd_X28_X25s__X29_X25s),
    /* K8   */  be_nested_str_weak(stop_iteration),
    /* K9   */  be_nested_str_weak(errors),
    /* K10  */  be_nested_str_weak(join_output),
    /* K11  */  be_nested_str_weak(error),
    /* K12  */  be_nested_str_weak(Template_X20body_X20transpilation_X20failed_X3A_X20_X25s),
    }),
    be_str_weak(transpile_template_body),
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0xA802002A,  //  0000  EXBLK	0	#002C
      0x8C040100,  //  0001  GETMET	R1	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x74060002,  //  0003  JMPT	R1	#0007
      0x8C040101,  //  0004  GETMET	R1	R0	K1
      0x7C040200,  //  0005  CALL	R1	1
      0x7001FFF9,  //  0006  JMP		#0001
      0x6004000C,  //  0007  GETGBL	R1	G12
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x7C040200,  //  0009  CALL	R1	1
      0x24040303,  //  000A  GT	R1	R1	K3
      0x78060012,  //  000B  JMPF	R1	#001F
      0x60040010,  //  000C  GETGBL	R1	G16
      0x88080102,  //  000D  GETMBR	R2	R0	K2
      0x7C040200,  //  000E  CALL	R1	1
      0xA802000B,  //  000F  EXBLK	0	#001C
      0x5C080200,  //  0010  MOVE	R2	R1
      0x7C080000,  //  0011  CALL	R2	0
      0x940C0504,  //  0012  GETIDX	R3	R2	K4
      0x94100505,  //  0013  GETIDX	R4	R2	K5
      0x8C140106,  //  0014  GETMET	R5	R0	K6
      0x601C0018,  //  0015  GETGBL	R7	G24
      0x58200007,  //  0016  LDCONST	R8	K7
      0x5C240600,  //  0017  MOVE	R9	R3
      0x5C280800,  //  0018  MOVE	R10	R4
      0x7C1C0600,  //  0019  CALL	R7	3
      0x7C140400,  //  001A  CALL	R5	2
      0x7001FFF3,  //  001B  JMP		#0010
      0x58040008,  //  001C  LDCONST	R1	K8
      0xAC040200,  //  001D  CATCH	R1	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x6004000C,  //  001F  GETGBL	R1	G12
      0x88080109,  //  0020  GETMBR	R2	R0	K9
      0x7C040200,  //  0021  CALL	R1	1
      0x1C040303,  //  0022  EQ	R1	R1	K3
      0x78060002,  //  0023  JMPF	R1	#0027
      0x8C04010A,  //  0024  GETMET	R1	R0	K10
      0x7C040200,  //  0025  CALL	R1	1
      0x70020000,  //  0026  JMP		#0028
      0x4C040000,  //  0027  LDNIL	R1
      0xA8040001,  //  0028  EXBLK	1	1
      0x80040200,  //  0029  RET	1	R1
      0xA8040001,  //  002A  EXBLK	1	1
      0x7002000B,  //  002B  JMP		#0038
      0xAC040002,  //  002C  CATCH	R1	0	2
      0x70020008,  //  002D  JMP		#0037
      0x8C0C010B,  //  002E  GETMET	R3	R0	K11
      0x60140018,  //  002F  GETGBL	R5	G24
      0x5818000C,  //  0030  LDCONST	R6	K12
      0x5C1C0400,  //  0031  MOVE	R7	R2
      0x7C140400,  //  0032  CALL	R5	2
      0x7C0C0400,  //  0033  CALL	R3	2
      0x4C0C0000,  //  0034  LDNIL	R3
      0x80040600,  //  0035  RET	1	R3
      0x70020000,  //  0036  JMP		#0038
      0xB0080000,  //  0037  RAISE	2	R0	R0
      0x80000000,  //  0038  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_animation_factory_creates_animation
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_animation_factory_creates_animation,   /* name */
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
    /* K0   */  be_nested_str_weak(_validate_factory_function),
    /* K1   */  be_nested_str_weak(animation),
    }),
    be_str_weak(_validate_animation_factory_creates_animation),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0xB8160200,  //  0002  GETNGBL	R5	K1
      0x88140B01,  //  0003  GETMBR	R5	R5	K1
      0x7C080600,  //  0004  CALL	R2	3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_user_function_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_user_function_call,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_PAREN),
    /* K5   */  be_nested_str_weak(process_function_arguments),
    /* K6   */  be_nested_str_weak(),
    /* K7   */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K8   */  be_nested_str_weak(engine),
    /* K9   */  be_nested_str_weak(animation_X2Eget_user_function_X28_X27_X25s_X27_X29_X28_X25s_X29),
    /* K10  */  be_nested_str_weak(error),
    /* K11  */  be_nested_str_weak(User_X20functions_X20must_X20be_X20called_X20with_X20parentheses_X3A_X20user_X2Efunction_name_X28_X29),
    /* K12  */  be_nested_str_weak(nil),
    }),
    be_str_weak(_process_user_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0019,  //  0004  JMPF	R2	#001F
      0x8C080100,  //  0005  GETMET	R2	R0	K0
      0x7C080200,  //  0006  CALL	R2	1
      0x88080501,  //  0007  GETMBR	R2	R2	K1
      0xB80E0400,  //  0008  GETNGBL	R3	K2
      0x880C0703,  //  0009  GETMBR	R3	R3	K3
      0x880C0704,  //  000A  GETMBR	R3	R3	K4
      0x1C080403,  //  000B  EQ	R2	R2	R3
      0x780A0011,  //  000C  JMPF	R2	#001F
      0x8C080105,  //  000D  GETMET	R2	R0	K5
      0x50100200,  //  000E  LDBOOL	R4	1	0
      0x7C080400,  //  000F  CALL	R2	2
      0x200C0506,  //  0010  NE	R3	R2	K6
      0x780E0004,  //  0011  JMPF	R3	#0017
      0x600C0018,  //  0012  GETGBL	R3	G24
      0x58100007,  //  0013  LDCONST	R4	K7
      0x5C140400,  //  0014  MOVE	R5	R2
      0x7C0C0400,  //  0015  CALL	R3	2
      0x70020000,  //  0016  JMP		#0018
      0x580C0008,  //  0017  LDCONST	R3	K8
      0x60100018,  //  0018  GETGBL	R4	G24
      0x58140009,  //  0019  LDCONST	R5	K9
      0x5C180200,  //  001A  MOVE	R6	R1
      0x5C1C0600,  //  001B  MOVE	R7	R3
      0x7C100600,  //  001C  CALL	R4	3
      0x80040800,  //  001D  RET	1	R4
      0x70020003,  //  001E  JMP		#0023
      0x8C08010A,  //  001F  GETMET	R2	R0	K10
      0x5810000B,  //  0020  LDCONST	R4	K11
      0x7C080400,  //  0021  CALL	R2	2
      0x80061800,  //  0022  RET	1	K12
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_right_bracket
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_right_bracket,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_BRACKET),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X5D_X27),
    }),
    be_str_weak(expect_right_bracket),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_animation_factory_exists
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_animation_factory_exists,   /* name */
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
    /* K0   */  be_nested_str_weak(is_math_method),
    /* K1   */  be_nested_str_weak(_validate_factory_function),
    }),
    be_str_weak(_validate_animation_factory_exists),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x50080200,  //  0004  LDBOOL	R2	1	0
      0x80040400,  //  0005  RET	1	R2
      0x8C080101,  //  0006  GETMET	R2	R0	K1
      0x5C100200,  //  0007  MOVE	R4	R1
      0x4C140000,  //  0008  LDNIL	R5
      0x7C080600,  //  0009  CALL	R2	3
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_event_handler
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_event_handler,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(line),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(_X7B_X7D),
    /* K6   */  be_nested_str_weak(type),
    /* K7   */  be_nested_str_weak(animation_dsl),
    /* K8   */  be_nested_str_weak(Token),
    /* K9   */  be_nested_str_weak(LEFT_PAREN),
    /* K10  */  be_nested_str_weak(process_event_parameters),
    /* K11  */  be_nested_str_weak(expect_colon),
    /* K12  */  be_nested_str_weak(event_handler__X25s__X25s),
    /* K13  */  be_nested_str_weak(add),
    /* K14  */  be_nested_str_weak(def_X20_X25s_X28event_data_X29),
    /* K15  */  be_nested_str_weak(KEYWORD),
    /* K16  */  be_nested_str_weak(value),
    /* K17  */  be_nested_str_weak(interrupt),
    /* K18  */  be_nested_str_weak(_X20_X20engine_X2Einterrupt_current_X28_X29),
    /* K19  */  be_nested_str_weak(_X20_X20engine_X2Einterrupt_animation_X28_X22_X25s_X22_X29),
    /* K20  */  be_nested_str_weak(process_value),
    /* K21  */  be_nested_str_weak(animation),
    /* K22  */  be_nested_str_weak(_X20_X20var_X20temp_anim_X20_X3D_X20_X25s),
    /* K23  */  be_nested_str_weak(_X20_X20engine_X2Eadd_X28temp_anim_X29),
    /* K24  */  be_nested_str_weak(end),
    /* K25  */  be_nested_str_weak(animation_X2Eregister_event_handler_X28_X22_X25s_X22_X2C_X20_X25s_X2C_X200_X2C_X20nil_X2C_X20_X25s_X29),
    }),
    be_str_weak(process_event_handler),
    &be_const_str_solidified,
    ( &(const binstruction[101]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x4C0C0000,  //  0006  LDNIL	R3
      0x20080403,  //  0007  NE	R2	R2	R3
      0x780A0003,  //  0008  JMPF	R2	#000D
      0x8C080102,  //  0009  GETMET	R2	R0	K2
      0x7C080200,  //  000A  CALL	R2	1
      0x88080503,  //  000B  GETMBR	R2	R2	K3
      0x70020000,  //  000C  JMP		#000E
      0x58080004,  //  000D  LDCONST	R2	K4
      0x580C0005,  //  000E  LDCONST	R3	K5
      0x8C100102,  //  000F  GETMET	R4	R0	K2
      0x7C100200,  //  0010  CALL	R4	1
      0x4C140000,  //  0011  LDNIL	R5
      0x20100805,  //  0012  NE	R4	R4	R5
      0x7812000A,  //  0013  JMPF	R4	#001F
      0x8C100102,  //  0014  GETMET	R4	R0	K2
      0x7C100200,  //  0015  CALL	R4	1
      0x88100906,  //  0016  GETMBR	R4	R4	K6
      0xB8160E00,  //  0017  GETNGBL	R5	K7
      0x88140B08,  //  0018  GETMBR	R5	R5	K8
      0x88140B09,  //  0019  GETMBR	R5	R5	K9
      0x1C100805,  //  001A  EQ	R4	R4	R5
      0x78120002,  //  001B  JMPF	R4	#001F
      0x8C10010A,  //  001C  GETMET	R4	R0	K10
      0x7C100200,  //  001D  CALL	R4	1
      0x5C0C0800,  //  001E  MOVE	R3	R4
      0x8C10010B,  //  001F  GETMET	R4	R0	K11
      0x7C100200,  //  0020  CALL	R4	1
      0x60100018,  //  0021  GETGBL	R4	G24
      0x5814000C,  //  0022  LDCONST	R5	K12
      0x5C180200,  //  0023  MOVE	R6	R1
      0x5C1C0400,  //  0024  MOVE	R7	R2
      0x7C100600,  //  0025  CALL	R4	3
      0x8C14010D,  //  0026  GETMET	R5	R0	K13
      0x601C0018,  //  0027  GETGBL	R7	G24
      0x5820000E,  //  0028  LDCONST	R8	K14
      0x5C240800,  //  0029  MOVE	R9	R4
      0x7C1C0400,  //  002A  CALL	R7	2
      0x7C140400,  //  002B  CALL	R5	2
      0x8C140102,  //  002C  GETMET	R5	R0	K2
      0x7C140200,  //  002D  CALL	R5	1
      0x4C180000,  //  002E  LDNIL	R6
      0x20180A06,  //  002F  NE	R6	R5	R6
      0x781A0027,  //  0030  JMPF	R6	#0059
      0x88180B06,  //  0031  GETMBR	R6	R5	K6
      0xB81E0E00,  //  0032  GETNGBL	R7	K7
      0x881C0F08,  //  0033  GETMBR	R7	R7	K8
      0x881C0F0F,  //  0034  GETMBR	R7	R7	K15
      0x1C180C07,  //  0035  EQ	R6	R6	R7
      0x781A0013,  //  0036  JMPF	R6	#004B
      0x88180B10,  //  0037  GETMBR	R6	R5	K16
      0x1C180D11,  //  0038  EQ	R6	R6	K17
      0x781A0010,  //  0039  JMPF	R6	#004B
      0x8C180100,  //  003A  GETMET	R6	R0	K0
      0x7C180200,  //  003B  CALL	R6	1
      0x8C180101,  //  003C  GETMET	R6	R0	K1
      0x7C180200,  //  003D  CALL	R6	1
      0x1C1C0D02,  //  003E  EQ	R7	R6	K2
      0x781E0003,  //  003F  JMPF	R7	#0044
      0x8C1C010D,  //  0040  GETMET	R7	R0	K13
      0x58240012,  //  0041  LDCONST	R9	K18
      0x7C1C0400,  //  0042  CALL	R7	2
      0x70020005,  //  0043  JMP		#004A
      0x8C1C010D,  //  0044  GETMET	R7	R0	K13
      0x60240018,  //  0045  GETGBL	R9	G24
      0x58280013,  //  0046  LDCONST	R10	K19
      0x5C2C0C00,  //  0047  MOVE	R11	R6
      0x7C240400,  //  0048  CALL	R9	2
      0x7C1C0400,  //  0049  CALL	R7	2
      0x7002000D,  //  004A  JMP		#0059
      0x8C180114,  //  004B  GETMET	R6	R0	K20
      0x58200015,  //  004C  LDCONST	R8	K21
      0x7C180400,  //  004D  CALL	R6	2
      0x8C1C010D,  //  004E  GETMET	R7	R0	K13
      0x60240018,  //  004F  GETGBL	R9	G24
      0x58280016,  //  0050  LDCONST	R10	K22
      0x5C2C0C00,  //  0051  MOVE	R11	R6
      0x7C240400,  //  0052  CALL	R9	2
      0x7C1C0400,  //  0053  CALL	R7	2
      0x8C1C010D,  //  0054  GETMET	R7	R0	K13
      0x60240018,  //  0055  GETGBL	R9	G24
      0x58280017,  //  0056  LDCONST	R10	K23
      0x7C240200,  //  0057  CALL	R9	1
      0x7C1C0400,  //  0058  CALL	R7	2
      0x8C18010D,  //  0059  GETMET	R6	R0	K13
      0x58200018,  //  005A  LDCONST	R8	K24
      0x7C180400,  //  005B  CALL	R6	2
      0x8C18010D,  //  005C  GETMET	R6	R0	K13
      0x60200018,  //  005D  GETGBL	R8	G24
      0x58240019,  //  005E  LDCONST	R9	K25
      0x5C280200,  //  005F  MOVE	R10	R1
      0x5C2C0800,  //  0060  MOVE	R11	R4
      0x5C300600,  //  0061  MOVE	R12	R3
      0x7C200800,  //  0062  CALL	R8	4
      0x7C180400,  //  0063  CALL	R6	2
      0x80000000,  //  0064  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_number
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_number,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(NUMBER),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(Expected_X20number),
    /* K9   */  be_nested_str_weak(0),
    }),
    be_str_weak(expect_number),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A000A,  //  0004  JMPF	R2	#0010
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0004,  //  000A  JMPF	R2	#0010
      0x88080305,  //  000B  GETMBR	R2	R1	K5
      0x8C0C0106,  //  000C  GETMET	R3	R0	K6
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80040400,  //  000E  RET	1	R2
      0x70020003,  //  000F  JMP		#0014
      0x8C080107,  //  0010  GETMET	R2	R0	K7
      0x58100008,  //  0011  LDCONST	R4	K8
      0x7C080400,  //  0012  CALL	R2	2
      0x80061200,  //  0013  RET	1	K9
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: validate_symbol_reference
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_validate_symbol_reference,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(symbol_exists),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Undefined_X20reference_X20_X27_X25s_X27_X20in_X20_X25s_X2E_X20Make_X20sure_X20the_X20object_X20is_X20defined_X20before_X20use_X2E),
    }),
    be_str_weak(validate_symbol_reference),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x7C0C0400,  //  0002  CALL	R3	2
      0x740E0008,  //  0003  JMPT	R3	#000D
      0x8C0C0101,  //  0004  GETMET	R3	R0	K1
      0x60140018,  //  0005  GETGBL	R5	G24
      0x58180002,  //  0006  LDCONST	R6	K2
      0x5C1C0200,  //  0007  MOVE	R7	R1
      0x5C200400,  //  0008  MOVE	R8	R2
      0x7C140600,  //  0009  CALL	R5	3
      0x7C0C0400,  //  000A  CALL	R3	2
      0x500C0000,  //  000B  LDBOOL	R3	0	0
      0x80040600,  //  000C  RET	1	R3
      0x500C0200,  //  000D  LDBOOL	R3	1	0
      0x80040600,  //  000E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_engine_run
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_generate_engine_run,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(run_statements),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(has_template_calls),
    /* K3   */  be_nested_str_weak(name),
    /* K4   */  be_nested_str_weak(comment),
    /* K5   */  be_nested_str_weak(add),
    /* K6   */  be_nested_str_weak(engine_X2Eadd_X28_X25s__X29_X25s),
    /* K7   */  be_nested_str_weak(stop_iteration),
    /* K8   */  be_nested_str_weak(engine_X2Erun_X28_X29),
    }),
    be_str_weak(generate_engine_run),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040301,  //  0003  EQ	R1	R1	K1
      0x78060002,  //  0004  JMPF	R1	#0008
      0x88040102,  //  0005  GETMBR	R1	R0	K2
      0x74060000,  //  0006  JMPT	R1	#0008
      0x80000200,  //  0007  RET	0
      0x60040010,  //  0008  GETGBL	R1	G16
      0x88080100,  //  0009  GETMBR	R2	R0	K0
      0x7C040200,  //  000A  CALL	R1	1
      0xA802000B,  //  000B  EXBLK	0	#0018
      0x5C080200,  //  000C  MOVE	R2	R1
      0x7C080000,  //  000D  CALL	R2	0
      0x940C0503,  //  000E  GETIDX	R3	R2	K3
      0x94100504,  //  000F  GETIDX	R4	R2	K4
      0x8C140105,  //  0010  GETMET	R5	R0	K5
      0x601C0018,  //  0011  GETGBL	R7	G24
      0x58200006,  //  0012  LDCONST	R8	K6
      0x5C240600,  //  0013  MOVE	R9	R3
      0x5C280800,  //  0014  MOVE	R10	R4
      0x7C1C0600,  //  0015  CALL	R7	3
      0x7C140400,  //  0016  CALL	R5	2
      0x7001FFF3,  //  0017  JMP		#000C
      0x58040007,  //  0018  LDCONST	R1	K7
      0xAC040200,  //  0019  CATCH	R1	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x8C040105,  //  001B  GETMET	R1	R0	K5
      0x580C0008,  //  001C  LDCONST	R3	K8
      0x7C040400,  //  001D  CALL	R1	2
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: skip_whitespace
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_skip_whitespace,   /* name */
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
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(NEWLINE),
    /* K6   */  be_nested_str_weak(COMMENT),
    /* K7   */  be_nested_str_weak(next),
    }),
    be_str_weak(skip_whitespace),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060015,  //  0002  JMPT	R1	#0019
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x4C080000,  //  0005  LDNIL	R2
      0x20080202,  //  0006  NE	R2	R1	R2
      0x780A000E,  //  0007  JMPF	R2	#0017
      0x88080302,  //  0008  GETMBR	R2	R1	K2
      0xB80E0600,  //  0009  GETNGBL	R3	K3
      0x880C0704,  //  000A  GETMBR	R3	R3	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x740A0005,  //  000D  JMPT	R2	#0014
      0x88080302,  //  000E  GETMBR	R2	R1	K2
      0xB80E0600,  //  000F  GETNGBL	R3	K3
      0x880C0704,  //  0010  GETMBR	R3	R3	K4
      0x880C0706,  //  0011  GETMBR	R3	R3	K6
      0x1C080403,  //  0012  EQ	R2	R2	R3
      0x780A0002,  //  0013  JMPF	R2	#0017
      0x8C080107,  //  0014  GETMET	R2	R0	K7
      0x7C080200,  //  0015  CALL	R2	1
      0x70020000,  //  0016  JMP		#0018
      0x70020000,  //  0017  JMP		#0019
      0x7001FFE6,  //  0018  JMP		#0000
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_right_bracket
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_check_right_bracket,   /* name */
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
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_BRACKET),
    }),
    be_str_weak(check_right_bracket),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0005,  //  0004  JMPF	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A0000,  //  000A  JMPT	R2	#000C
      0x50080001,  //  000B  LDBOOL	R2	0	1
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_template
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_template,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(template),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_left_brace),
    /* K6   */  be_nested_str_weak(at_end),
    /* K7   */  be_nested_str_weak(check_right_brace),
    /* K8   */  be_nested_str_weak(skip_whitespace_including_newlines),
    /* K9   */  be_nested_str_weak(current),
    /* K10  */  be_nested_str_weak(type),
    /* K11  */  be_nested_str_weak(animation_dsl),
    /* K12  */  be_nested_str_weak(Token),
    /* K13  */  be_nested_str_weak(KEYWORD),
    /* K14  */  be_nested_str_weak(value),
    /* K15  */  be_nested_str_weak(param),
    /* K16  */  be_nested_str_weak(push),
    /* K17  */  be_nested_str_weak(NEWLINE),
    /* K18  */  be_const_int(0),
    /* K19  */  be_nested_str_weak(EOF),
    /* K20  */  be_nested_str_weak(LEFT_BRACE),
    /* K21  */  be_const_int(1),
    /* K22  */  be_nested_str_weak(RIGHT_BRACE),
    /* K23  */  be_nested_str_weak(expect_right_brace),
    /* K24  */  be_nested_str_weak(template_definitions),
    /* K25  */  be_nested_str_weak(params),
    /* K26  */  be_nested_str_weak(param_types),
    /* K27  */  be_nested_str_weak(body_tokens),
    /* K28  */  be_nested_str_weak(generate_template_function),
    /* K29  */  be_nested_str_weak(symbol_table),
    }),
    be_str_weak(process_template),
    &be_const_str_solidified,
    ( &(const binstruction[165]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x60080012,  //  000E  GETGBL	R2	G18
      0x7C080000,  //  000F  CALL	R2	0
      0x600C0013,  //  0010  GETGBL	R3	G19
      0x7C0C0000,  //  0011  CALL	R3	0
      0x8C100106,  //  0012  GETMET	R4	R0	K6
      0x7C100200,  //  0013  CALL	R4	1
      0x7412004B,  //  0014  JMPT	R4	#0061
      0x8C100107,  //  0015  GETMET	R4	R0	K7
      0x7C100200,  //  0016  CALL	R4	1
      0x74120048,  //  0017  JMPT	R4	#0061
      0x8C100108,  //  0018  GETMET	R4	R0	K8
      0x7C100200,  //  0019  CALL	R4	1
      0x8C100107,  //  001A  GETMET	R4	R0	K7
      0x7C100200,  //  001B  CALL	R4	1
      0x78120000,  //  001C  JMPF	R4	#001E
      0x70020042,  //  001D  JMP		#0061
      0x8C100109,  //  001E  GETMET	R4	R0	K9
      0x7C100200,  //  001F  CALL	R4	1
      0x4C140000,  //  0020  LDNIL	R5
      0x20140805,  //  0021  NE	R5	R4	R5
      0x7816003B,  //  0022  JMPF	R5	#005F
      0x8814090A,  //  0023  GETMBR	R5	R4	K10
      0xB81A1600,  //  0024  GETNGBL	R6	K11
      0x88180D0C,  //  0025  GETMBR	R6	R6	K12
      0x88180D0D,  //  0026  GETMBR	R6	R6	K13
      0x1C140A06,  //  0027  EQ	R5	R5	R6
      0x78160035,  //  0028  JMPF	R5	#005F
      0x8814090E,  //  0029  GETMBR	R5	R4	K14
      0x1C140B0F,  //  002A  EQ	R5	R5	K15
      0x78160032,  //  002B  JMPF	R5	#005F
      0x8C140100,  //  002C  GETMET	R5	R0	K0
      0x7C140200,  //  002D  CALL	R5	1
      0x8C140101,  //  002E  GETMET	R5	R0	K1
      0x7C140200,  //  002F  CALL	R5	1
      0x4C180000,  //  0030  LDNIL	R6
      0x8C1C0109,  //  0031  GETMET	R7	R0	K9
      0x7C1C0200,  //  0032  CALL	R7	1
      0x4C200000,  //  0033  LDNIL	R8
      0x201C0E08,  //  0034  NE	R7	R7	R8
      0x781E0011,  //  0035  JMPF	R7	#0048
      0x8C1C0109,  //  0036  GETMET	R7	R0	K9
      0x7C1C0200,  //  0037  CALL	R7	1
      0x881C0F0A,  //  0038  GETMBR	R7	R7	K10
      0xB8221600,  //  0039  GETNGBL	R8	K11
      0x8820110C,  //  003A  GETMBR	R8	R8	K12
      0x8820110D,  //  003B  GETMBR	R8	R8	K13
      0x1C1C0E08,  //  003C  EQ	R7	R7	R8
      0x781E0009,  //  003D  JMPF	R7	#0048
      0x8C1C0109,  //  003E  GETMET	R7	R0	K9
      0x7C1C0200,  //  003F  CALL	R7	1
      0x881C0F0E,  //  0040  GETMBR	R7	R7	K14
      0x1C1C0F0A,  //  0041  EQ	R7	R7	K10
      0x781E0004,  //  0042  JMPF	R7	#0048
      0x8C1C0100,  //  0043  GETMET	R7	R0	K0
      0x7C1C0200,  //  0044  CALL	R7	1
      0x8C1C0101,  //  0045  GETMET	R7	R0	K1
      0x7C1C0200,  //  0046  CALL	R7	1
      0x5C180E00,  //  0047  MOVE	R6	R7
      0x8C1C0510,  //  0048  GETMET	R7	R2	K16
      0x5C240A00,  //  0049  MOVE	R9	R5
      0x7C1C0400,  //  004A  CALL	R7	2
      0x4C1C0000,  //  004B  LDNIL	R7
      0x201C0C07,  //  004C  NE	R7	R6	R7
      0x781E0000,  //  004D  JMPF	R7	#004F
      0x980C0A06,  //  004E  SETIDX	R3	R5	R6
      0x8C1C0109,  //  004F  GETMET	R7	R0	K9
      0x7C1C0200,  //  0050  CALL	R7	1
      0x4C200000,  //  0051  LDNIL	R8
      0x201C0E08,  //  0052  NE	R7	R7	R8
      0x781E0009,  //  0053  JMPF	R7	#005E
      0x8C1C0109,  //  0054  GETMET	R7	R0	K9
      0x7C1C0200,  //  0055  CALL	R7	1
      0x881C0F0A,  //  0056  GETMBR	R7	R7	K10
      0xB8221600,  //  0057  GETNGBL	R8	K11
      0x8820110C,  //  0058  GETMBR	R8	R8	K12
      0x88201111,  //  0059  GETMBR	R8	R8	K17
      0x1C1C0E08,  //  005A  EQ	R7	R7	R8
      0x781E0001,  //  005B  JMPF	R7	#005E
      0x8C1C0100,  //  005C  GETMET	R7	R0	K0
      0x7C1C0200,  //  005D  CALL	R7	1
      0x70020000,  //  005E  JMP		#0060
      0x70020000,  //  005F  JMP		#0061
      0x7001FFB0,  //  0060  JMP		#0012
      0x60100012,  //  0061  GETGBL	R4	G18
      0x7C100000,  //  0062  CALL	R4	0
      0x58140012,  //  0063  LDCONST	R5	K18
      0x8C180106,  //  0064  GETMET	R6	R0	K6
      0x7C180200,  //  0065  CALL	R6	1
      0x741A002B,  //  0066  JMPT	R6	#0093
      0x8C180109,  //  0067  GETMET	R6	R0	K9
      0x7C180200,  //  0068  CALL	R6	1
      0x4C1C0000,  //  0069  LDNIL	R7
      0x1C1C0C07,  //  006A  EQ	R7	R6	R7
      0x741E0005,  //  006B  JMPT	R7	#0072
      0x881C0D0A,  //  006C  GETMBR	R7	R6	K10
      0xB8221600,  //  006D  GETNGBL	R8	K11
      0x8820110C,  //  006E  GETMBR	R8	R8	K12
      0x88201113,  //  006F  GETMBR	R8	R8	K19
      0x1C1C0E08,  //  0070  EQ	R7	R7	R8
      0x781E0000,  //  0071  JMPF	R7	#0073
      0x7002001F,  //  0072  JMP		#0093
      0x881C0D0A,  //  0073  GETMBR	R7	R6	K10
      0xB8221600,  //  0074  GETNGBL	R8	K11
      0x8820110C,  //  0075  GETMBR	R8	R8	K12
      0x88201114,  //  0076  GETMBR	R8	R8	K20
      0x1C1C0E08,  //  0077  EQ	R7	R7	R8
      0x781E0004,  //  0078  JMPF	R7	#007E
      0x00140B15,  //  0079  ADD	R5	R5	K21
      0x8C1C0910,  //  007A  GETMET	R7	R4	K16
      0x5C240C00,  //  007B  MOVE	R9	R6
      0x7C1C0400,  //  007C  CALL	R7	2
      0x70020011,  //  007D  JMP		#0090
      0x881C0D0A,  //  007E  GETMBR	R7	R6	K10
      0xB8221600,  //  007F  GETNGBL	R8	K11
      0x8820110C,  //  0080  GETMBR	R8	R8	K12
      0x88201116,  //  0081  GETMBR	R8	R8	K22
      0x1C1C0E08,  //  0082  EQ	R7	R7	R8
      0x781E0008,  //  0083  JMPF	R7	#008D
      0x1C1C0B12,  //  0084  EQ	R7	R5	K18
      0x781E0001,  //  0085  JMPF	R7	#0088
      0x7002000B,  //  0086  JMP		#0093
      0x70020003,  //  0087  JMP		#008C
      0x04140B15,  //  0088  SUB	R5	R5	K21
      0x8C1C0910,  //  0089  GETMET	R7	R4	K16
      0x5C240C00,  //  008A  MOVE	R9	R6
      0x7C1C0400,  //  008B  CALL	R7	2
      0x70020002,  //  008C  JMP		#0090
      0x8C1C0910,  //  008D  GETMET	R7	R4	K16
      0x5C240C00,  //  008E  MOVE	R9	R6
      0x7C1C0400,  //  008F  CALL	R7	2
      0x8C1C0100,  //  0090  GETMET	R7	R0	K0
      0x7C1C0200,  //  0091  CALL	R7	1
      0x7001FFD0,  //  0092  JMP		#0064
      0x8C180117,  //  0093  GETMET	R6	R0	K23
      0x7C180200,  //  0094  CALL	R6	1
      0x88180118,  //  0095  GETMBR	R6	R0	K24
      0x601C0013,  //  0096  GETGBL	R7	G19
      0x7C1C0000,  //  0097  CALL	R7	0
      0x981E3202,  //  0098  SETIDX	R7	K25	R2
      0x981E3403,  //  0099  SETIDX	R7	K26	R3
      0x981E3604,  //  009A  SETIDX	R7	K27	R4
      0x98180207,  //  009B  SETIDX	R6	R1	R7
      0x8C18011C,  //  009C  GETMET	R6	R0	K28
      0x5C200200,  //  009D  MOVE	R8	R1
      0x5C240400,  //  009E  MOVE	R9	R2
      0x5C280600,  //  009F  MOVE	R10	R3
      0x5C2C0800,  //  00A0  MOVE	R11	R4
      0x7C180A00,  //  00A1  CALL	R6	5
      0x8818011D,  //  00A2  GETMBR	R6	R0	K29
      0x98180303,  //  00A3  SETIDX	R6	R1	K3
      0x80000000,  //  00A4  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: skip_statement
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_skip_statement,   /* name */
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
    /* K0   */  be_nested_str_weak(at_end),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(NEWLINE),
    /* K6   */  be_nested_str_weak(EOF),
    /* K7   */  be_nested_str_weak(next),
    }),
    be_str_weak(skip_statement),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060011,  //  0002  JMPT	R1	#0015
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x88080302,  //  0005  GETMBR	R2	R1	K2
      0xB80E0600,  //  0006  GETNGBL	R3	K3
      0x880C0704,  //  0007  GETMBR	R3	R3	K4
      0x880C0705,  //  0008  GETMBR	R3	R3	K5
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A0005,  //  000A  JMPT	R2	#0011
      0x88080302,  //  000B  GETMBR	R2	R1	K2
      0xB80E0600,  //  000C  GETNGBL	R3	K3
      0x880C0704,  //  000D  GETMBR	R3	R3	K4
      0x880C0706,  //  000E  GETMBR	R3	R3	K6
      0x1C080403,  //  000F  EQ	R2	R2	R3
      0x780A0000,  //  0010  JMPF	R2	#0012
      0x70020002,  //  0011  JMP		#0015
      0x8C080107,  //  0012  GETMET	R2	R0	K7
      0x7C080200,  //  0013  CALL	R2	1
      0x7001FFEA,  //  0014  JMP		#0000
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_anonymous_function
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_is_anonymous_function,   /* name */
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
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(_X28def_X20),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(_X29_X28engine_X29),
    }),
    be_str_weak(is_anonymous_function),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x1C0C0703,  //  0005  EQ	R3	R3	K3
      0x780E0005,  //  0006  JMPF	R3	#000D
      0x8C0C0501,  //  0007  GETMET	R3	R2	K1
      0x5C140200,  //  0008  MOVE	R5	R1
      0x58180004,  //  0009  LDCONST	R6	K4
      0x7C0C0600,  //  000A  CALL	R3	3
      0x280C0703,  //  000B  GE	R3	R3	K3
      0x740E0000,  //  000C  JMPT	R3	#000E
      0x500C0001,  //  000D  LDBOOL	R3	0	1
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_factory_function
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_factory_function,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(animation),
    /* K3   */  be_nested_str_weak(function),
    /* K4   */  be_nested_str_weak(class),
    /* K5   */  be_nested_str_weak(animation_dsl),
    /* K6   */  be_nested_str_weak(MockEngine),
    }),
    be_str_weak(_validate_factory_function),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0xA8020032,  //  0000  EXBLK	0	#0034
      0xA40E0000,  //  0001  IMPORT	R3	K0
      0x8C100701,  //  0002  GETMET	R4	R3	K1
      0xB81A0400,  //  0003  GETNGBL	R6	K2
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x7C100600,  //  0005  CALL	R4	3
      0x74120002,  //  0006  JMPT	R4	#000A
      0x50100000,  //  0007  LDBOOL	R4	0	0
      0xA8040001,  //  0008  EXBLK	1	1
      0x80040800,  //  0009  RET	1	R4
      0xB8120400,  //  000A  GETNGBL	R4	K2
      0x88100801,  //  000B  GETMBR	R4	R4	R1
      0x60140004,  //  000C  GETGBL	R5	G4
      0x5C180800,  //  000D  MOVE	R6	R4
      0x7C140200,  //  000E  CALL	R5	1
      0x20180B03,  //  000F  NE	R6	R5	K3
      0x781A0004,  //  0010  JMPF	R6	#0016
      0x20180B04,  //  0011  NE	R6	R5	K4
      0x781A0002,  //  0012  JMPF	R6	#0016
      0x50180000,  //  0013  LDBOOL	R6	0	0
      0xA8040001,  //  0014  EXBLK	1	1
      0x80040C00,  //  0015  RET	1	R6
      0x4C180000,  //  0016  LDNIL	R6
      0x1C180406,  //  0017  EQ	R6	R2	R6
      0x781A0002,  //  0018  JMPF	R6	#001C
      0x50180200,  //  0019  LDBOOL	R6	1	0
      0xA8040001,  //  001A  EXBLK	1	1
      0x80040C00,  //  001B  RET	1	R6
      0xB81A0A00,  //  001C  GETNGBL	R6	K5
      0x8C180D06,  //  001D  GETMET	R6	R6	K6
      0x7C180200,  //  001E  CALL	R6	1
      0xA802000A,  //  001F  EXBLK	0	#002B
      0x5C1C0800,  //  0020  MOVE	R7	R4
      0x5C200C00,  //  0021  MOVE	R8	R6
      0x7C1C0200,  //  0022  CALL	R7	1
      0x6020000F,  //  0023  GETGBL	R8	G15
      0x5C240E00,  //  0024  MOVE	R9	R7
      0x5C280400,  //  0025  MOVE	R10	R2
      0x7C200400,  //  0026  CALL	R8	2
      0xA8040002,  //  0027  EXBLK	1	2
      0x80041000,  //  0028  RET	1	R8
      0xA8040001,  //  0029  EXBLK	1	1
      0x70020006,  //  002A  JMP		#0032
      0xAC1C0002,  //  002B  CATCH	R7	0	2
      0x70020003,  //  002C  JMP		#0031
      0x50240000,  //  002D  LDBOOL	R9	0	0
      0xA8040001,  //  002E  EXBLK	1	1
      0x80041200,  //  002F  RET	1	R9
      0x70020000,  //  0030  JMP		#0032
      0xB0080000,  //  0031  RAISE	2	R0	R0
      0xA8040001,  //  0032  EXBLK	1	1
      0x70020005,  //  0033  JMP		#003A
      0xAC0C0002,  //  0034  CATCH	R3	0	2
      0x70020002,  //  0035  JMP		#0039
      0x50140000,  //  0036  LDBOOL	R5	0	0
      0x80040A00,  //  0037  RET	1	R5
      0x70020000,  //  0038  JMP		#003A
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x80000000,  //  003A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_restart_statement_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_restart_statement_fluent,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(value),
    /* K2   */  be_nested_str_weak(next),
    /* K3   */  be_nested_str_weak(expect_identifier),
    /* K4   */  be_nested_str_weak(_validate_value_provider_reference),
    /* K5   */  be_nested_str_weak(skip_statement),
    /* K6   */  be_nested_str_weak(collect_inline_comment),
    /* K7   */  be_nested_str_weak(def_X20_X28engine_X29_X20_X25s__X2Estart_X28engine_X2Etime_ms_X29_X20end),
    /* K8   */  be_nested_str_weak(add),
    /* K9   */  be_nested_str_weak(_X25s_X2Epush_closure_step_X28_X25s_X29_X25s),
    /* K10  */  be_nested_str_weak(get_indent),
    }),
    be_str_weak(process_restart_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x88040301,  //  0002  GETMBR	R1	R1	K1
      0x8C080102,  //  0003  GETMET	R2	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x8C080103,  //  0005  GETMET	R2	R0	K3
      0x7C080200,  //  0006  CALL	R2	1
      0x8C0C0104,  //  0007  GETMET	R3	R0	K4
      0x5C140400,  //  0008  MOVE	R5	R2
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C0C0600,  //  000A  CALL	R3	3
      0x740E0002,  //  000B  JMPT	R3	#000F
      0x8C0C0105,  //  000C  GETMET	R3	R0	K5
      0x7C0C0200,  //  000D  CALL	R3	1
      0x80000600,  //  000E  RET	0
      0x8C0C0106,  //  000F  GETMET	R3	R0	K6
      0x7C0C0200,  //  0010  CALL	R3	1
      0x60100018,  //  0011  GETGBL	R4	G24
      0x58140007,  //  0012  LDCONST	R5	K7
      0x5C180400,  //  0013  MOVE	R6	R2
      0x7C100400,  //  0014  CALL	R4	2
      0x8C140108,  //  0015  GETMET	R5	R0	K8
      0x601C0018,  //  0016  GETGBL	R7	G24
      0x58200009,  //  0017  LDCONST	R8	K9
      0x8C24010A,  //  0018  GETMET	R9	R0	K10
      0x7C240200,  //  0019  CALL	R9	1
      0x5C280800,  //  001A  MOVE	R10	R4
      0x5C2C0600,  //  001B  MOVE	R11	R3
      0x7C1C0800,  //  001C  CALL	R7	4
      0x7C140400,  //  001D  CALL	R5	2
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: next
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_next,   /* name */
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
    /* K0   */  be_nested_str_weak(pos),
    /* K1   */  be_nested_str_weak(tokens),
    /* K2   */  be_const_int(1),
    }),
    be_str_weak(next),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14040202,  //  0004  LT	R1	R1	R2
      0x78060002,  //  0005  JMPF	R1	#0009
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x00040302,  //  0007  ADD	R1	R1	K2
      0x90020001,  //  0008  SETMBR	R0	K0	R1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_animation
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_animation,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[37]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(animation),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_assign),
    /* K6   */  be_nested_str_weak(current),
    /* K7   */  be_nested_str_weak(type),
    /* K8   */  be_nested_str_weak(animation_dsl),
    /* K9   */  be_nested_str_weak(Token),
    /* K10  */  be_nested_str_weak(KEYWORD),
    /* K11  */  be_nested_str_weak(IDENTIFIER),
    /* K12  */  be_nested_str_weak(peek),
    /* K13  */  be_nested_str_weak(LEFT_PAREN),
    /* K14  */  be_nested_str_weak(value),
    /* K15  */  be_nested_str_weak(),
    /* K16  */  be_nested_str_weak(COMMENT),
    /* K17  */  be_nested_str_weak(_X20_X20),
    /* K18  */  be_nested_str_weak(template_definitions),
    /* K19  */  be_nested_str_weak(contains),
    /* K20  */  be_nested_str_weak(process_function_arguments),
    /* K21  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K22  */  be_nested_str_weak(engine),
    /* K23  */  be_nested_str_weak(add),
    /* K24  */  be_nested_str_weak(_X25s_template_X28_X25s_X29_X25s),
    /* K25  */  be_nested_str_weak(_validate_animation_factory_creates_animation),
    /* K26  */  be_nested_str_weak(error),
    /* K27  */  be_nested_str_weak(Animation_X20factory_X20function_X20_X27_X25s_X27_X20does_X20not_X20exist_X20or_X20does_X20not_X20create_X20an_X20instance_X20of_X20animation_X2Eanimation_X20class_X2E_X20Check_X20the_X20function_X20name_X20and_X20ensure_X20it_X20returns_X20an_X20animation_X20object_X2E),
    /* K28  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20animation_X2E_X25s_X28engine_X29_X25s),
    /* K29  */  be_nested_str_weak(_create_instance_for_validation),
    /* K30  */  be_nested_str_weak(symbol_table),
    /* K31  */  be_nested_str_weak(_process_named_arguments_for_animation),
    /* K32  */  be_nested_str_weak(_X25s_),
    /* K33  */  be_nested_str_weak(process_value),
    /* K34  */  be_nested_str_weak(collect_inline_comment),
    /* K35  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20_X25s_X25s),
    /* K36  */  be_nested_str_weak(string),
    }),
    be_str_weak(process_animation),
    &be_const_str_solidified,
    ( &(const binstruction[192]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x880C0507,  //  0010  GETMBR	R3	R2	K7
      0xB8121000,  //  0011  GETNGBL	R4	K8
      0x88100909,  //  0012  GETMBR	R4	R4	K9
      0x8810090A,  //  0013  GETMBR	R4	R4	K10
      0x1C0C0604,  //  0014  EQ	R3	R3	R4
      0x740E0005,  //  0015  JMPT	R3	#001C
      0x880C0507,  //  0016  GETMBR	R3	R2	K7
      0xB8121000,  //  0017  GETNGBL	R4	K8
      0x88100909,  //  0018  GETMBR	R4	R4	K9
      0x8810090B,  //  0019  GETMBR	R4	R4	K11
      0x1C0C0604,  //  001A  EQ	R3	R3	R4
      0x780E0062,  //  001B  JMPF	R3	#007F
      0x8C0C010C,  //  001C  GETMET	R3	R0	K12
      0x7C0C0200,  //  001D  CALL	R3	1
      0x4C100000,  //  001E  LDNIL	R4
      0x200C0604,  //  001F  NE	R3	R3	R4
      0x780E005D,  //  0020  JMPF	R3	#007F
      0x8C0C010C,  //  0021  GETMET	R3	R0	K12
      0x7C0C0200,  //  0022  CALL	R3	1
      0x880C0707,  //  0023  GETMBR	R3	R3	K7
      0xB8121000,  //  0024  GETNGBL	R4	K8
      0x88100909,  //  0025  GETMBR	R4	R4	K9
      0x8810090D,  //  0026  GETMBR	R4	R4	K13
      0x1C0C0604,  //  0027  EQ	R3	R3	R4
      0x780E0055,  //  0028  JMPF	R3	#007F
      0x880C050E,  //  0029  GETMBR	R3	R2	K14
      0x8C100100,  //  002A  GETMET	R4	R0	K0
      0x7C100200,  //  002B  CALL	R4	1
      0x5810000F,  //  002C  LDCONST	R4	K15
      0x8C140106,  //  002D  GETMET	R5	R0	K6
      0x7C140200,  //  002E  CALL	R5	1
      0x4C180000,  //  002F  LDNIL	R6
      0x20140A06,  //  0030  NE	R5	R5	R6
      0x7816000E,  //  0031  JMPF	R5	#0041
      0x8C140106,  //  0032  GETMET	R5	R0	K6
      0x7C140200,  //  0033  CALL	R5	1
      0x88140B07,  //  0034  GETMBR	R5	R5	K7
      0xB81A1000,  //  0035  GETNGBL	R6	K8
      0x88180D09,  //  0036  GETMBR	R6	R6	K9
      0x88180D10,  //  0037  GETMBR	R6	R6	K16
      0x1C140A06,  //  0038  EQ	R5	R5	R6
      0x78160006,  //  0039  JMPF	R5	#0041
      0x8C140106,  //  003A  GETMET	R5	R0	K6
      0x7C140200,  //  003B  CALL	R5	1
      0x88140B0E,  //  003C  GETMBR	R5	R5	K14
      0x00162205,  //  003D  ADD	R5	K17	R5
      0x5C100A00,  //  003E  MOVE	R4	R5
      0x8C140100,  //  003F  GETMET	R5	R0	K0
      0x7C140200,  //  0040  CALL	R5	1
      0x88140112,  //  0041  GETMBR	R5	R0	K18
      0x8C140B13,  //  0042  GETMET	R5	R5	K19
      0x5C1C0600,  //  0043  MOVE	R7	R3
      0x7C140400,  //  0044  CALL	R5	2
      0x78160013,  //  0045  JMPF	R5	#005A
      0x8C140114,  //  0046  GETMET	R5	R0	K20
      0x501C0000,  //  0047  LDBOOL	R7	0	0
      0x7C140400,  //  0048  CALL	R5	2
      0x20180B0F,  //  0049  NE	R6	R5	K15
      0x781A0004,  //  004A  JMPF	R6	#0050
      0x60180018,  //  004B  GETGBL	R6	G24
      0x581C0015,  //  004C  LDCONST	R7	K21
      0x5C200A00,  //  004D  MOVE	R8	R5
      0x7C180400,  //  004E  CALL	R6	2
      0x70020000,  //  004F  JMP		#0051
      0x58180016,  //  0050  LDCONST	R6	K22
      0x8C1C0117,  //  0051  GETMET	R7	R0	K23
      0x60240018,  //  0052  GETGBL	R9	G24
      0x58280018,  //  0053  LDCONST	R10	K24
      0x5C2C0600,  //  0054  MOVE	R11	R3
      0x5C300C00,  //  0055  MOVE	R12	R6
      0x5C340800,  //  0056  MOVE	R13	R4
      0x7C240800,  //  0057  CALL	R9	4
      0x7C1C0400,  //  0058  CALL	R7	2
      0x70020023,  //  0059  JMP		#007E
      0x8C140119,  //  005A  GETMET	R5	R0	K25
      0x5C1C0600,  //  005B  MOVE	R7	R3
      0x7C140400,  //  005C  CALL	R5	2
      0x74160008,  //  005D  JMPT	R5	#0067
      0x8C14011A,  //  005E  GETMET	R5	R0	K26
      0x601C0018,  //  005F  GETGBL	R7	G24
      0x5820001B,  //  0060  LDCONST	R8	K27
      0x5C240600,  //  0061  MOVE	R9	R3
      0x7C1C0400,  //  0062  CALL	R7	2
      0x7C140400,  //  0063  CALL	R5	2
      0x8C140104,  //  0064  GETMET	R5	R0	K4
      0x7C140200,  //  0065  CALL	R5	1
      0x80000A00,  //  0066  RET	0
      0x8C140117,  //  0067  GETMET	R5	R0	K23
      0x601C0018,  //  0068  GETGBL	R7	G24
      0x5820001C,  //  0069  LDCONST	R8	K28
      0x5C240200,  //  006A  MOVE	R9	R1
      0x5C280600,  //  006B  MOVE	R10	R3
      0x5C2C0800,  //  006C  MOVE	R11	R4
      0x7C1C0800,  //  006D  CALL	R7	4
      0x7C140400,  //  006E  CALL	R5	2
      0x8C14011D,  //  006F  GETMET	R5	R0	K29
      0x5C1C0600,  //  0070  MOVE	R7	R3
      0x7C140400,  //  0071  CALL	R5	2
      0x4C180000,  //  0072  LDNIL	R6
      0x20180A06,  //  0073  NE	R6	R5	R6
      0x781A0001,  //  0074  JMPF	R6	#0077
      0x8818011E,  //  0075  GETMBR	R6	R0	K30
      0x98180205,  //  0076  SETIDX	R6	R1	R5
      0x8C18011F,  //  0077  GETMET	R6	R0	K31
      0x60200018,  //  0078  GETGBL	R8	G24
      0x58240020,  //  0079  LDCONST	R9	K32
      0x5C280200,  //  007A  MOVE	R10	R1
      0x7C200400,  //  007B  CALL	R8	2
      0x5C240600,  //  007C  MOVE	R9	R3
      0x7C180600,  //  007D  CALL	R6	3
      0x7002003F,  //  007E  JMP		#00BF
      0x8C0C0106,  //  007F  GETMET	R3	R0	K6
      0x7C0C0200,  //  0080  CALL	R3	1
      0x4C100000,  //  0081  LDNIL	R4
      0x20100604,  //  0082  NE	R4	R3	R4
      0x78120012,  //  0083  JMPF	R4	#0097
      0x88100707,  //  0084  GETMBR	R4	R3	K7
      0xB8161000,  //  0085  GETNGBL	R5	K8
      0x88140B09,  //  0086  GETMBR	R5	R5	K9
      0x88140B0B,  //  0087  GETMBR	R5	R5	K11
      0x1C100805,  //  0088  EQ	R4	R4	R5
      0x7812000C,  //  0089  JMPF	R4	#0097
      0x8C10010C,  //  008A  GETMET	R4	R0	K12
      0x7C100200,  //  008B  CALL	R4	1
      0x4C140000,  //  008C  LDNIL	R5
      0x1C100805,  //  008D  EQ	R4	R4	R5
      0x74120008,  //  008E  JMPT	R4	#0098
      0x8C10010C,  //  008F  GETMET	R4	R0	K12
      0x7C100200,  //  0090  CALL	R4	1
      0x88100907,  //  0091  GETMBR	R4	R4	K7
      0xB8161000,  //  0092  GETNGBL	R5	K8
      0x88140B09,  //  0093  GETMBR	R5	R5	K9
      0x88140B0D,  //  0094  GETMBR	R5	R5	K13
      0x20100805,  //  0095  NE	R4	R4	R5
      0x74120000,  //  0096  JMPT	R4	#0098
      0x50100001,  //  0097  LDBOOL	R4	0	1
      0x50100200,  //  0098  LDBOOL	R4	1	0
      0x78120001,  //  0099  JMPF	R4	#009C
      0x8814070E,  //  009A  GETMBR	R5	R3	K14
      0x70020000,  //  009B  JMP		#009D
      0x4C140000,  //  009C  LDNIL	R5
      0x8C180121,  //  009D  GETMET	R6	R0	K33
      0x58200003,  //  009E  LDCONST	R8	K3
      0x7C180400,  //  009F  CALL	R6	2
      0x8C1C0122,  //  00A0  GETMET	R7	R0	K34
      0x7C1C0200,  //  00A1  CALL	R7	1
      0x8C200117,  //  00A2  GETMET	R8	R0	K23
      0x60280018,  //  00A3  GETGBL	R10	G24
      0x582C0023,  //  00A4  LDCONST	R11	K35
      0x5C300200,  //  00A5  MOVE	R12	R1
      0x5C340C00,  //  00A6  MOVE	R13	R6
      0x5C380E00,  //  00A7  MOVE	R14	R7
      0x7C280800,  //  00A8  CALL	R10	4
      0x7C200400,  //  00A9  CALL	R8	2
      0x78120011,  //  00AA  JMPF	R4	#00BD
      0x4C200000,  //  00AB  LDNIL	R8
      0x20200A08,  //  00AC  NE	R8	R5	R8
      0x7822000E,  //  00AD  JMPF	R8	#00BD
      0x8820011E,  //  00AE  GETMBR	R8	R0	K30
      0x8C201113,  //  00AF  GETMET	R8	R8	K19
      0x5C280A00,  //  00B0  MOVE	R10	R5
      0x7C200400,  //  00B1  CALL	R8	2
      0x78220009,  //  00B2  JMPF	R8	#00BD
      0x8820011E,  //  00B3  GETMBR	R8	R0	K30
      0x94201005,  //  00B4  GETIDX	R8	R8	R5
      0x60240004,  //  00B5  GETGBL	R9	G4
      0x5C281000,  //  00B6  MOVE	R10	R8
      0x7C240200,  //  00B7  CALL	R9	1
      0x20241324,  //  00B8  NE	R9	R9	K36
      0x78260001,  //  00B9  JMPF	R9	#00BC
      0x8824011E,  //  00BA  GETMBR	R9	R0	K30
      0x98240208,  //  00BB  SETIDX	R9	R1	R8
      0x70020001,  //  00BC  JMP		#00BF
      0x8820011E,  //  00BD  GETMBR	R8	R0	K30
      0x98200303,  //  00BE  SETIDX	R8	R1	K3
      0x80000000,  //  00BF  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_property_assignment
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_property_assignment,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[34]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_identifier),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(LEFT_PAREN),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(process_function_arguments),
    /* K8   */  be_nested_str_weak(collect_inline_comment),
    /* K9   */  be_nested_str_weak(process_log_call),
    /* K10  */  be_nested_str_weak(standalone),
    /* K11  */  be_nested_str_weak(add),
    /* K12  */  be_nested_str_weak(template_definitions),
    /* K13  */  be_nested_str_weak(contains),
    /* K14  */  be_nested_str_weak(),
    /* K15  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K16  */  be_nested_str_weak(engine),
    /* K17  */  be_nested_str_weak(_X25s_template_X28_X25s_X29_X25s),
    /* K18  */  be_nested_str_weak(has_template_calls),
    /* K19  */  be_nested_str_weak(error),
    /* K20  */  be_nested_str_weak(Standalone_X20function_X20calls_X20are_X20only_X20supported_X20for_X20templates_X2E_X20_X27_X25s_X27_X20is_X20not_X20a_X20template_X2E),
    /* K21  */  be_nested_str_weak(skip_statement),
    /* K22  */  be_nested_str_weak(DOT),
    /* K23  */  be_nested_str_weak(next),
    /* K24  */  be_nested_str_weak(symbol_table),
    /* K25  */  be_nested_str_weak(string),
    /* K26  */  be_nested_str_weak(_validate_single_parameter),
    /* K27  */  be_nested_str_weak(Sequences_X20like_X20_X27_X25s_X27_X20do_X20not_X20have_X20properties_X2E_X20Property_X20assignments_X20are_X20only_X20valid_X20for_X20animations_X20and_X20color_X20providers_X2E),
    /* K28  */  be_nested_str_weak(expect_assign),
    /* K29  */  be_nested_str_weak(process_value),
    /* K30  */  be_nested_str_weak(property),
    /* K31  */  be_nested_str_weak(resolve_symbol_reference),
    /* K32  */  be_nested_str_weak(_X25s_X2E_X25s_X20_X3D_X20_X25s_X25s),
    /* K33  */  be_nested_str_weak(Expected_X20property_X20assignment_X20for_X20_X27_X25s_X27_X20but_X20found_X20no_X20dot),
    }),
    be_str_weak(process_property_assignment),
    &be_const_str_solidified,
    ( &(const binstruction[142]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x20080403,  //  0005  NE	R2	R2	R3
      0x780A003D,  //  0006  JMPF	R2	#0045
      0x8C080101,  //  0007  GETMET	R2	R0	K1
      0x7C080200,  //  0008  CALL	R2	1
      0x88080502,  //  0009  GETMBR	R2	R2	K2
      0xB80E0600,  //  000A  GETNGBL	R3	K3
      0x880C0704,  //  000B  GETMBR	R3	R3	K4
      0x880C0705,  //  000C  GETMBR	R3	R3	K5
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A0035,  //  000E  JMPF	R2	#0045
      0x1C080306,  //  000F  EQ	R2	R1	K6
      0x780A000D,  //  0010  JMPF	R2	#001F
      0x8C080107,  //  0011  GETMET	R2	R0	K7
      0x50100000,  //  0012  LDBOOL	R4	0	0
      0x7C080400,  //  0013  CALL	R2	2
      0x8C0C0108,  //  0014  GETMET	R3	R0	K8
      0x7C0C0200,  //  0015  CALL	R3	1
      0x8C100109,  //  0016  GETMET	R4	R0	K9
      0x5C180400,  //  0017  MOVE	R6	R2
      0x581C000A,  //  0018  LDCONST	R7	K10
      0x5C200600,  //  0019  MOVE	R8	R3
      0x7C100800,  //  001A  CALL	R4	4
      0x8C14010B,  //  001B  GETMET	R5	R0	K11
      0x5C1C0800,  //  001C  MOVE	R7	R4
      0x7C140400,  //  001D  CALL	R5	2
      0x80000A00,  //  001E  RET	0
      0x8808010C,  //  001F  GETMBR	R2	R0	K12
      0x8C08050D,  //  0020  GETMET	R2	R2	K13
      0x5C100200,  //  0021  MOVE	R4	R1
      0x7C080400,  //  0022  CALL	R2	2
      0x780A0017,  //  0023  JMPF	R2	#003C
      0x8C080107,  //  0024  GETMET	R2	R0	K7
      0x50100000,  //  0025  LDBOOL	R4	0	0
      0x7C080400,  //  0026  CALL	R2	2
      0x200C050E,  //  0027  NE	R3	R2	K14
      0x780E0004,  //  0028  JMPF	R3	#002E
      0x600C0018,  //  0029  GETGBL	R3	G24
      0x5810000F,  //  002A  LDCONST	R4	K15
      0x5C140400,  //  002B  MOVE	R5	R2
      0x7C0C0400,  //  002C  CALL	R3	2
      0x70020000,  //  002D  JMP		#002F
      0x580C0010,  //  002E  LDCONST	R3	K16
      0x8C100108,  //  002F  GETMET	R4	R0	K8
      0x7C100200,  //  0030  CALL	R4	1
      0x8C14010B,  //  0031  GETMET	R5	R0	K11
      0x601C0018,  //  0032  GETGBL	R7	G24
      0x58200011,  //  0033  LDCONST	R8	K17
      0x5C240200,  //  0034  MOVE	R9	R1
      0x5C280600,  //  0035  MOVE	R10	R3
      0x5C2C0800,  //  0036  MOVE	R11	R4
      0x7C1C0800,  //  0037  CALL	R7	4
      0x7C140400,  //  0038  CALL	R5	2
      0x50140200,  //  0039  LDBOOL	R5	1	0
      0x90022405,  //  003A  SETMBR	R0	K18	R5
      0x70020007,  //  003B  JMP		#0044
      0x8C080113,  //  003C  GETMET	R2	R0	K19
      0x60100018,  //  003D  GETGBL	R4	G24
      0x58140014,  //  003E  LDCONST	R5	K20
      0x5C180200,  //  003F  MOVE	R6	R1
      0x7C100400,  //  0040  CALL	R4	2
      0x7C080400,  //  0041  CALL	R2	2
      0x8C080115,  //  0042  GETMET	R2	R0	K21
      0x7C080200,  //  0043  CALL	R2	1
      0x80000400,  //  0044  RET	0
      0x8C080101,  //  0045  GETMET	R2	R0	K1
      0x7C080200,  //  0046  CALL	R2	1
      0x4C0C0000,  //  0047  LDNIL	R3
      0x20080403,  //  0048  NE	R2	R2	R3
      0x780A003A,  //  0049  JMPF	R2	#0085
      0x8C080101,  //  004A  GETMET	R2	R0	K1
      0x7C080200,  //  004B  CALL	R2	1
      0x88080502,  //  004C  GETMBR	R2	R2	K2
      0xB80E0600,  //  004D  GETNGBL	R3	K3
      0x880C0704,  //  004E  GETMBR	R3	R3	K4
      0x880C0716,  //  004F  GETMBR	R3	R3	K22
      0x1C080403,  //  0050  EQ	R2	R2	R3
      0x780A0032,  //  0051  JMPF	R2	#0085
      0x8C080117,  //  0052  GETMET	R2	R0	K23
      0x7C080200,  //  0053  CALL	R2	1
      0x8C080100,  //  0054  GETMET	R2	R0	K0
      0x7C080200,  //  0055  CALL	R2	1
      0x880C0118,  //  0056  GETMBR	R3	R0	K24
      0x8C0C070D,  //  0057  GETMET	R3	R3	K13
      0x5C140200,  //  0058  MOVE	R5	R1
      0x7C0C0400,  //  0059  CALL	R3	2
      0x780E0015,  //  005A  JMPF	R3	#0071
      0x880C0118,  //  005B  GETMBR	R3	R0	K24
      0x940C0601,  //  005C  GETIDX	R3	R3	R1
      0x60100004,  //  005D  GETGBL	R4	G4
      0x5C140600,  //  005E  MOVE	R5	R3
      0x7C100200,  //  005F  CALL	R4	1
      0x20100919,  //  0060  NE	R4	R4	K25
      0x78120008,  //  0061  JMPF	R4	#006B
      0x60100005,  //  0062  GETGBL	R4	G5
      0x5C140600,  //  0063  MOVE	R5	R3
      0x7C100200,  //  0064  CALL	R4	1
      0x8C14011A,  //  0065  GETMET	R5	R0	K26
      0x5C1C0800,  //  0066  MOVE	R7	R4
      0x5C200400,  //  0067  MOVE	R8	R2
      0x5C240600,  //  0068  MOVE	R9	R3
      0x7C140800,  //  0069  CALL	R5	4
      0x70020005,  //  006A  JMP		#0071
      0x8C100113,  //  006B  GETMET	R4	R0	K19
      0x60180018,  //  006C  GETGBL	R6	G24
      0x581C001B,  //  006D  LDCONST	R7	K27
      0x5C200200,  //  006E  MOVE	R8	R1
      0x7C180400,  //  006F  CALL	R6	2
      0x7C100400,  //  0070  CALL	R4	2
      0x8C0C011C,  //  0071  GETMET	R3	R0	K28
      0x7C0C0200,  //  0072  CALL	R3	1
      0x8C0C011D,  //  0073  GETMET	R3	R0	K29
      0x5814001E,  //  0074  LDCONST	R5	K30
      0x7C0C0400,  //  0075  CALL	R3	2
      0x8C100108,  //  0076  GETMET	R4	R0	K8
      0x7C100200,  //  0077  CALL	R4	1
      0x8C14011F,  //  0078  GETMET	R5	R0	K31
      0x5C1C0200,  //  0079  MOVE	R7	R1
      0x7C140400,  //  007A  CALL	R5	2
      0x8C18010B,  //  007B  GETMET	R6	R0	K11
      0x60200018,  //  007C  GETGBL	R8	G24
      0x58240020,  //  007D  LDCONST	R9	K32
      0x5C280A00,  //  007E  MOVE	R10	R5
      0x5C2C0400,  //  007F  MOVE	R11	R2
      0x5C300600,  //  0080  MOVE	R12	R3
      0x5C340800,  //  0081  MOVE	R13	R4
      0x7C200A00,  //  0082  CALL	R8	5
      0x7C180400,  //  0083  CALL	R6	2
      0x70020007,  //  0084  JMP		#008D
      0x8C080113,  //  0085  GETMET	R2	R0	K19
      0x60100018,  //  0086  GETGBL	R4	G24
      0x58140021,  //  0087  LDCONST	R5	K33
      0x5C180200,  //  0088  MOVE	R6	R1
      0x7C100400,  //  0089  CALL	R4	2
      0x7C080400,  //  008A  CALL	R2	2
      0x8C080115,  //  008B  GETMET	R2	R0	K21
      0x7C080200,  //  008C  CALL	R2	1
      0x80000000,  //  008D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: validate_user_name
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_validate_user_name,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    /* K2   */  be_nested_str_weak(color_names),
    /* K3   */  be_nested_str_weak(error),
    /* K4   */  be_nested_str_weak(Cannot_X20redefine_X20predefined_X20color_X20_X27_X25s_X27_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_custom_X27_X20or_X20_X27my__X25s_X27),
    /* K5   */  be_nested_str_weak(stop_iteration),
    /* K6   */  be_nested_str_weak(statement_keywords),
    /* K7   */  be_nested_str_weak(Cannot_X20use_X20DSL_X20keyword_X20_X27_X25s_X27_X20as_X20_X25s_X20name_X2E_X20Use_X20a_X20different_X20name_X20like_X20_X27_X25s_custom_X27_X20or_X20_X27my__X25s_X27),
    }),
    be_str_weak(validate_user_name),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x600C0010,  //  0000  GETGBL	R3	G16
      0xB8120000,  //  0001  GETNGBL	R4	K0
      0x88100901,  //  0002  GETMBR	R4	R4	K1
      0x88100902,  //  0003  GETMBR	R4	R4	K2
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA802000F,  //  0005  EXBLK	0	#0016
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x1C140204,  //  0008  EQ	R5	R1	R4
      0x7816000A,  //  0009  JMPF	R5	#0015
      0x8C140103,  //  000A  GETMET	R5	R0	K3
      0x601C0018,  //  000B  GETGBL	R7	G24
      0x58200004,  //  000C  LDCONST	R8	K4
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280200,  //  000E  MOVE	R10	R1
      0x5C2C0200,  //  000F  MOVE	R11	R1
      0x7C1C0800,  //  0010  CALL	R7	4
      0x7C140400,  //  0011  CALL	R5	2
      0x50140000,  //  0012  LDBOOL	R5	0	0
      0xA8040001,  //  0013  EXBLK	1	1
      0x80040A00,  //  0014  RET	1	R5
      0x7001FFEF,  //  0015  JMP		#0006
      0x580C0005,  //  0016  LDCONST	R3	K5
      0xAC0C0200,  //  0017  CATCH	R3	1	0
      0xB0080000,  //  0018  RAISE	2	R0	R0
      0x600C0010,  //  0019  GETGBL	R3	G16
      0xB8120000,  //  001A  GETNGBL	R4	K0
      0x88100901,  //  001B  GETMBR	R4	R4	K1
      0x88100906,  //  001C  GETMBR	R4	R4	K6
      0x7C0C0200,  //  001D  CALL	R3	1
      0xA8020010,  //  001E  EXBLK	0	#0030
      0x5C100600,  //  001F  MOVE	R4	R3
      0x7C100000,  //  0020  CALL	R4	0
      0x1C140204,  //  0021  EQ	R5	R1	R4
      0x7816000B,  //  0022  JMPF	R5	#002F
      0x8C140103,  //  0023  GETMET	R5	R0	K3
      0x601C0018,  //  0024  GETGBL	R7	G24
      0x58200007,  //  0025  LDCONST	R8	K7
      0x5C240200,  //  0026  MOVE	R9	R1
      0x5C280400,  //  0027  MOVE	R10	R2
      0x5C2C0200,  //  0028  MOVE	R11	R1
      0x5C300200,  //  0029  MOVE	R12	R1
      0x7C1C0A00,  //  002A  CALL	R7	5
      0x7C140400,  //  002B  CALL	R5	2
      0x50140000,  //  002C  LDBOOL	R5	0	0
      0xA8040001,  //  002D  EXBLK	1	1
      0x80040A00,  //  002E  RET	1	R5
      0x7001FFEE,  //  002F  JMP		#001F
      0x580C0005,  //  0030  LDCONST	R3	K5
      0xAC0C0200,  //  0031  CATCH	R3	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x500C0200,  //  0033  LDBOOL	R3	1	0
      0x80040600,  //  0034  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_right_paren
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_right_paren,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_PAREN),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X29_X27),
    }),
    be_str_weak(expect_right_paren),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_set
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_set,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(variable),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_assign),
    /* K6   */  be_nested_str_weak(current),
    /* K7   */  be_nested_str_weak(type),
    /* K8   */  be_nested_str_weak(animation_dsl),
    /* K9   */  be_nested_str_weak(Token),
    /* K10  */  be_nested_str_weak(KEYWORD),
    /* K11  */  be_nested_str_weak(IDENTIFIER),
    /* K12  */  be_nested_str_weak(peek),
    /* K13  */  be_nested_str_weak(LEFT_PAREN),
    /* K14  */  be_nested_str_weak(value),
    /* K15  */  be_nested_str_weak(_validate_value_provider_factory_exists),
    /* K16  */  be_nested_str_weak(process_value),
    /* K17  */  be_nested_str_weak(collect_inline_comment),
    /* K18  */  be_nested_str_weak(add),
    /* K19  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20_X25s_X25s),
    /* K20  */  be_nested_str_weak(symbol_table),
    /* K21  */  be_nested_str_weak(value_provider),
    }),
    be_str_weak(process_set),
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x50080000,  //  000E  LDBOOL	R2	0	0
      0x8C0C0106,  //  000F  GETMET	R3	R0	K6
      0x7C0C0200,  //  0010  CALL	R3	1
      0x88100707,  //  0011  GETMBR	R4	R3	K7
      0xB8161000,  //  0012  GETNGBL	R5	K8
      0x88140B09,  //  0013  GETMBR	R5	R5	K9
      0x88140B0A,  //  0014  GETMBR	R5	R5	K10
      0x1C100805,  //  0015  EQ	R4	R4	R5
      0x74120005,  //  0016  JMPT	R4	#001D
      0x88100707,  //  0017  GETMBR	R4	R3	K7
      0xB8161000,  //  0018  GETNGBL	R5	K8
      0x88140B09,  //  0019  GETMBR	R5	R5	K9
      0x88140B0B,  //  001A  GETMBR	R5	R5	K11
      0x1C100805,  //  001B  EQ	R4	R4	R5
      0x78120012,  //  001C  JMPF	R4	#0030
      0x8C10010C,  //  001D  GETMET	R4	R0	K12
      0x7C100200,  //  001E  CALL	R4	1
      0x4C140000,  //  001F  LDNIL	R5
      0x20100805,  //  0020  NE	R4	R4	R5
      0x7812000D,  //  0021  JMPF	R4	#0030
      0x8C10010C,  //  0022  GETMET	R4	R0	K12
      0x7C100200,  //  0023  CALL	R4	1
      0x88100907,  //  0024  GETMBR	R4	R4	K7
      0xB8161000,  //  0025  GETNGBL	R5	K8
      0x88140B09,  //  0026  GETMBR	R5	R5	K9
      0x88140B0D,  //  0027  GETMBR	R5	R5	K13
      0x1C100805,  //  0028  EQ	R4	R4	R5
      0x78120005,  //  0029  JMPF	R4	#0030
      0x8810070E,  //  002A  GETMBR	R4	R3	K14
      0x8C14010F,  //  002B  GETMET	R5	R0	K15
      0x5C1C0800,  //  002C  MOVE	R7	R4
      0x7C140400,  //  002D  CALL	R5	2
      0x78160000,  //  002E  JMPF	R5	#0030
      0x50080200,  //  002F  LDBOOL	R2	1	0
      0x8C100110,  //  0030  GETMET	R4	R0	K16
      0x58180003,  //  0031  LDCONST	R6	K3
      0x7C100400,  //  0032  CALL	R4	2
      0x8C140111,  //  0033  GETMET	R5	R0	K17
      0x7C140200,  //  0034  CALL	R5	1
      0x8C180112,  //  0035  GETMET	R6	R0	K18
      0x60200018,  //  0036  GETGBL	R8	G24
      0x58240013,  //  0037  LDCONST	R9	K19
      0x5C280200,  //  0038  MOVE	R10	R1
      0x5C2C0800,  //  0039  MOVE	R11	R4
      0x5C300A00,  //  003A  MOVE	R12	R5
      0x7C200800,  //  003B  CALL	R8	4
      0x7C180400,  //  003C  CALL	R6	2
      0x780A0002,  //  003D  JMPF	R2	#0041
      0x88180114,  //  003E  GETMBR	R6	R0	K20
      0x98180315,  //  003F  SETIDX	R6	R1	K21
      0x70020001,  //  0040  JMP		#0043
      0x88180114,  //  0041  GETMBR	R6	R0	K20
      0x98180303,  //  0042  SETIDX	R6	R1	K3
      0x80000000,  //  0043  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence_assignment
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence_assignment,   /* name */
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
    /* K0   */  be_nested_str_weak(process_sequence_assignment_generic),
    /* K1   */  be_nested_str_weak(steps),
    }),
    be_str_weak(process_sequence_assignment),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x58140001,  //  0002  LDCONST	R5	K1
      0x7C080600,  //  0003  CALL	R2	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _is_simple_function_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__is_simple_function_call,   /* name */
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
    /* K0   */  be_nested_str_weak(strip_length),
    /* K1   */  be_nested_str_weak(static_value),
    /* K2   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_is_simple_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x400C0500,  //  0002  CONNECT	R3	R2	K0
      0x400C0501,  //  0003  CONNECT	R3	R2	K1
      0x600C0010,  //  0004  GETGBL	R3	G16
      0x5C100400,  //  0005  MOVE	R4	R2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA8020007,  //  0007  EXBLK	0	#0010
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x1C140204,  //  000A  EQ	R5	R1	R4
      0x78160002,  //  000B  JMPF	R5	#000F
      0x50140200,  //  000C  LDBOOL	R5	1	0
      0xA8040001,  //  000D  EXBLK	1	1
      0x80040A00,  //  000E  RET	1	R5
      0x7001FFF7,  //  000F  JMP		#0008
      0x580C0002,  //  0010  LDCONST	R3	K2
      0xAC0C0200,  //  0011  CATCH	R3	1	0
      0xB0080000,  //  0012  RAISE	2	R0	R0
      0x500C0000,  //  0013  LDBOOL	R3	0	0
      0x80040600,  //  0014  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_statement
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_statement,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[42]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(EOF),
    /* K5   */  be_nested_str_weak(COMMENT),
    /* K6   */  be_nested_str_weak(add),
    /* K7   */  be_nested_str_weak(value),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(NEWLINE),
    /* K10  */  be_nested_str_weak(first_statement),
    /* K11  */  be_nested_str_weak(KEYWORD),
    /* K12  */  be_nested_str_weak(IDENTIFIER),
    /* K13  */  be_nested_str_weak(strip),
    /* K14  */  be_nested_str_weak(error),
    /* K15  */  be_nested_str_weak(_X27strip_X27_X20directive_X20is_X20temporarily_X20disabled_X2E_X20Strip_X20configuration_X20is_X20handled_X20automatically_X2E),
    /* K16  */  be_nested_str_weak(skip_statement),
    /* K17  */  be_nested_str_weak(strip_initialized),
    /* K18  */  be_nested_str_weak(generate_default_strip_initialization),
    /* K19  */  be_nested_str_weak(color),
    /* K20  */  be_nested_str_weak(process_color),
    /* K21  */  be_nested_str_weak(palette),
    /* K22  */  be_nested_str_weak(process_palette),
    /* K23  */  be_nested_str_weak(animation),
    /* K24  */  be_nested_str_weak(process_animation),
    /* K25  */  be_nested_str_weak(set),
    /* K26  */  be_nested_str_weak(process_set),
    /* K27  */  be_nested_str_weak(sequence),
    /* K28  */  be_nested_str_weak(process_sequence),
    /* K29  */  be_nested_str_weak(template),
    /* K30  */  be_nested_str_weak(process_template),
    /* K31  */  be_nested_str_weak(run),
    /* K32  */  be_nested_str_weak(process_run),
    /* K33  */  be_nested_str_weak(import),
    /* K34  */  be_nested_str_weak(process_import),
    /* K35  */  be_nested_str_weak(on),
    /* K36  */  be_nested_str_weak(process_event_handler),
    /* K37  */  be_nested_str_weak(log),
    /* K38  */  be_nested_str_weak(peek),
    /* K39  */  be_nested_str_weak(LEFT_PAREN),
    /* K40  */  be_nested_str_weak(process_standalone_log),
    /* K41  */  be_nested_str_weak(process_property_assignment),
    }),
    be_str_weak(process_statement),
    &be_const_str_solidified,
    ( &(const binstruction[160]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x740A0005,  //  0004  JMPT	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0000,  //  000A  JMPF	R2	#000C
      0x80000400,  //  000B  RET	0
      0x88080301,  //  000C  GETMBR	R2	R1	K1
      0xB80E0400,  //  000D  GETNGBL	R3	K2
      0x880C0703,  //  000E  GETMBR	R3	R3	K3
      0x880C0705,  //  000F  GETMBR	R3	R3	K5
      0x1C080403,  //  0010  EQ	R2	R2	R3
      0x780A0005,  //  0011  JMPF	R2	#0018
      0x8C080106,  //  0012  GETMET	R2	R0	K6
      0x88100307,  //  0013  GETMBR	R4	R1	K7
      0x7C080400,  //  0014  CALL	R2	2
      0x8C080108,  //  0015  GETMET	R2	R0	K8
      0x7C080200,  //  0016  CALL	R2	1
      0x80000400,  //  0017  RET	0
      0x88080301,  //  0018  GETMBR	R2	R1	K1
      0xB80E0400,  //  0019  GETNGBL	R3	K2
      0x880C0703,  //  001A  GETMBR	R3	R3	K3
      0x880C0709,  //  001B  GETMBR	R3	R3	K9
      0x1C080403,  //  001C  EQ	R2	R2	R3
      0x780A0002,  //  001D  JMPF	R2	#0021
      0x8C080108,  //  001E  GETMET	R2	R0	K8
      0x7C080200,  //  001F  CALL	R2	1
      0x80000400,  //  0020  RET	0
      0x8808010A,  //  0021  GETMBR	R2	R0	K10
      0x880C0301,  //  0022  GETMBR	R3	R1	K1
      0xB8120400,  //  0023  GETNGBL	R4	K2
      0x88100903,  //  0024  GETMBR	R4	R4	K3
      0x8810090B,  //  0025  GETMBR	R4	R4	K11
      0x1C0C0604,  //  0026  EQ	R3	R3	R4
      0x740E0005,  //  0027  JMPT	R3	#002E
      0x880C0301,  //  0028  GETMBR	R3	R1	K1
      0xB8120400,  //  0029  GETNGBL	R4	K2
      0x88100903,  //  002A  GETMBR	R4	R4	K3
      0x8810090C,  //  002B  GETMBR	R4	R4	K12
      0x1C0C0604,  //  002C  EQ	R3	R3	R4
      0x780E0001,  //  002D  JMPF	R3	#0030
      0x500C0000,  //  002E  LDBOOL	R3	0	0
      0x90021403,  //  002F  SETMBR	R0	K10	R3
      0x880C0301,  //  0030  GETMBR	R3	R1	K1
      0xB8120400,  //  0031  GETNGBL	R4	K2
      0x88100903,  //  0032  GETMBR	R4	R4	K3
      0x8810090B,  //  0033  GETMBR	R4	R4	K11
      0x1C0C0604,  //  0034  EQ	R3	R3	R4
      0x780E0046,  //  0035  JMPF	R3	#007D
      0x880C0307,  //  0036  GETMBR	R3	R1	K7
      0x1C0C070D,  //  0037  EQ	R3	R3	K13
      0x780E0006,  //  0038  JMPF	R3	#0040
      0x8C0C010E,  //  0039  GETMET	R3	R0	K14
      0x5814000F,  //  003A  LDCONST	R5	K15
      0x7C0C0400,  //  003B  CALL	R3	2
      0x8C0C0110,  //  003C  GETMET	R3	R0	K16
      0x7C0C0200,  //  003D  CALL	R3	1
      0x80000600,  //  003E  RET	0
      0x7002003B,  //  003F  JMP		#007C
      0x880C0111,  //  0040  GETMBR	R3	R0	K17
      0x740E0001,  //  0041  JMPT	R3	#0044
      0x8C0C0112,  //  0042  GETMET	R3	R0	K18
      0x7C0C0200,  //  0043  CALL	R3	1
      0x880C0307,  //  0044  GETMBR	R3	R1	K7
      0x1C0C0713,  //  0045  EQ	R3	R3	K19
      0x780E0002,  //  0046  JMPF	R3	#004A
      0x8C0C0114,  //  0047  GETMET	R3	R0	K20
      0x7C0C0200,  //  0048  CALL	R3	1
      0x70020031,  //  0049  JMP		#007C
      0x880C0307,  //  004A  GETMBR	R3	R1	K7
      0x1C0C0715,  //  004B  EQ	R3	R3	K21
      0x780E0002,  //  004C  JMPF	R3	#0050
      0x8C0C0116,  //  004D  GETMET	R3	R0	K22
      0x7C0C0200,  //  004E  CALL	R3	1
      0x7002002B,  //  004F  JMP		#007C
      0x880C0307,  //  0050  GETMBR	R3	R1	K7
      0x1C0C0717,  //  0051  EQ	R3	R3	K23
      0x780E0002,  //  0052  JMPF	R3	#0056
      0x8C0C0118,  //  0053  GETMET	R3	R0	K24
      0x7C0C0200,  //  0054  CALL	R3	1
      0x70020025,  //  0055  JMP		#007C
      0x880C0307,  //  0056  GETMBR	R3	R1	K7
      0x1C0C0719,  //  0057  EQ	R3	R3	K25
      0x780E0002,  //  0058  JMPF	R3	#005C
      0x8C0C011A,  //  0059  GETMET	R3	R0	K26
      0x7C0C0200,  //  005A  CALL	R3	1
      0x7002001F,  //  005B  JMP		#007C
      0x880C0307,  //  005C  GETMBR	R3	R1	K7
      0x1C0C071B,  //  005D  EQ	R3	R3	K27
      0x780E0002,  //  005E  JMPF	R3	#0062
      0x8C0C011C,  //  005F  GETMET	R3	R0	K28
      0x7C0C0200,  //  0060  CALL	R3	1
      0x70020019,  //  0061  JMP		#007C
      0x880C0307,  //  0062  GETMBR	R3	R1	K7
      0x1C0C071D,  //  0063  EQ	R3	R3	K29
      0x780E0002,  //  0064  JMPF	R3	#0068
      0x8C0C011E,  //  0065  GETMET	R3	R0	K30
      0x7C0C0200,  //  0066  CALL	R3	1
      0x70020013,  //  0067  JMP		#007C
      0x880C0307,  //  0068  GETMBR	R3	R1	K7
      0x1C0C071F,  //  0069  EQ	R3	R3	K31
      0x780E0002,  //  006A  JMPF	R3	#006E
      0x8C0C0120,  //  006B  GETMET	R3	R0	K32
      0x7C0C0200,  //  006C  CALL	R3	1
      0x7002000D,  //  006D  JMP		#007C
      0x880C0307,  //  006E  GETMBR	R3	R1	K7
      0x1C0C0721,  //  006F  EQ	R3	R3	K33
      0x780E0002,  //  0070  JMPF	R3	#0074
      0x8C0C0122,  //  0071  GETMET	R3	R0	K34
      0x7C0C0200,  //  0072  CALL	R3	1
      0x70020007,  //  0073  JMP		#007C
      0x880C0307,  //  0074  GETMBR	R3	R1	K7
      0x1C0C0723,  //  0075  EQ	R3	R3	K35
      0x780E0002,  //  0076  JMPF	R3	#007A
      0x8C0C0124,  //  0077  GETMET	R3	R0	K36
      0x7C0C0200,  //  0078  CALL	R3	1
      0x70020001,  //  0079  JMP		#007C
      0x8C0C0110,  //  007A  GETMET	R3	R0	K16
      0x7C0C0200,  //  007B  CALL	R3	1
      0x70020021,  //  007C  JMP		#009F
      0x880C0301,  //  007D  GETMBR	R3	R1	K1
      0xB8120400,  //  007E  GETNGBL	R4	K2
      0x88100903,  //  007F  GETMBR	R4	R4	K3
      0x8810090C,  //  0080  GETMBR	R4	R4	K12
      0x1C0C0604,  //  0081  EQ	R3	R3	R4
      0x780E0019,  //  0082  JMPF	R3	#009D
      0x880C0111,  //  0083  GETMBR	R3	R0	K17
      0x740E0001,  //  0084  JMPT	R3	#0087
      0x8C0C0112,  //  0085  GETMET	R3	R0	K18
      0x7C0C0200,  //  0086  CALL	R3	1
      0x880C0307,  //  0087  GETMBR	R3	R1	K7
      0x1C0C0725,  //  0088  EQ	R3	R3	K37
      0x780E000F,  //  0089  JMPF	R3	#009A
      0x8C0C0126,  //  008A  GETMET	R3	R0	K38
      0x7C0C0200,  //  008B  CALL	R3	1
      0x4C100000,  //  008C  LDNIL	R4
      0x200C0604,  //  008D  NE	R3	R3	R4
      0x780E000A,  //  008E  JMPF	R3	#009A
      0x8C0C0126,  //  008F  GETMET	R3	R0	K38
      0x7C0C0200,  //  0090  CALL	R3	1
      0x880C0701,  //  0091  GETMBR	R3	R3	K1
      0xB8120400,  //  0092  GETNGBL	R4	K2
      0x88100903,  //  0093  GETMBR	R4	R4	K3
      0x88100927,  //  0094  GETMBR	R4	R4	K39
      0x1C0C0604,  //  0095  EQ	R3	R3	R4
      0x780E0002,  //  0096  JMPF	R3	#009A
      0x8C0C0128,  //  0097  GETMET	R3	R0	K40
      0x7C0C0200,  //  0098  CALL	R3	1
      0x70020001,  //  0099  JMP		#009C
      0x8C0C0129,  //  009A  GETMET	R3	R0	K41
      0x7C0C0200,  //  009B  CALL	R3	1
      0x70020001,  //  009C  JMP		#009F
      0x8C0C0110,  //  009D  GETMET	R3	R0	K16
      0x7C0C0200,  //  009E  CALL	R3	1
      0x80000000,  //  009F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_error_report
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_error_report,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(has_errors),
    /* K1   */  be_nested_str_weak(No_X20compilation_X20errors),
    /* K2   */  be_nested_str_weak(Compilation_X20errors_X3A_X0A),
    /* K3   */  be_nested_str_weak(errors),
    /* K4   */  be_nested_str_weak(_X20_X20),
    /* K5   */  be_nested_str_weak(_X0A),
    /* K6   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_error_report),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060000,  //  0002  JMPT	R1	#0004
      0x80060200,  //  0003  RET	1	K1
      0x58040002,  //  0004  LDCONST	R1	K2
      0x60080010,  //  0005  GETGBL	R2	G16
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020005,  //  0008  EXBLK	0	#000F
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x00120803,  //  000B  ADD	R4	K4	R3
      0x00100905,  //  000C  ADD	R4	R4	K5
      0x00040204,  //  000D  ADD	R1	R1	R4
      0x7001FFF9,  //  000E  JMP		#0009
      0x58080006,  //  000F  LDCONST	R2	K6
      0xAC080200,  //  0010  CATCH	R2	1	0
      0xB0080000,  //  0011  RAISE	2	R0	R0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: join_output
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_join_output,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(output),
    /* K2   */  be_nested_str_weak(_X0A),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(join_output),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0xA8020004,  //  0004  EXBLK	0	#000A
      0x5C0C0400,  //  0005  MOVE	R3	R2
      0x7C0C0000,  //  0006  CALL	R3	0
      0x00100702,  //  0007  ADD	R4	R3	K2
      0x00040204,  //  0008  ADD	R1	R1	R4
      0x7001FFFA,  //  0009  JMP		#0005
      0x58080003,  //  000A  LDCONST	R2	K3
      0xAC080200,  //  000B  CATCH	R2	1	0
      0xB0080000,  //  000C  RAISE	2	R0	R0
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: error
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_error,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(line),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(errors),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(Line_X20_X25s_X3A_X20_X25s),
    }),
    be_str_weak(error),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x8C080100,  //  0005  GETMET	R2	R0	K0
      0x7C080200,  //  0006  CALL	R2	1
      0x88080501,  //  0007  GETMBR	R2	R2	K1
      0x70020000,  //  0008  JMP		#000A
      0x58080002,  //  0009  LDCONST	R2	K2
      0x880C0103,  //  000A  GETMBR	R3	R0	K3
      0x8C0C0704,  //  000B  GETMET	R3	R3	K4
      0x60140018,  //  000C  GETGBL	R5	G24
      0x58180005,  //  000D  LDCONST	R6	K5
      0x5C1C0400,  //  000E  MOVE	R7	R2
      0x5C200200,  //  000F  MOVE	R8	R1
      0x7C140600,  //  0010  CALL	R5	3
      0x7C0C0400,  //  0011  CALL	R3	2
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence_assignment_generic
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence_assignment_generic,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_identifier),
    /* K1   */  be_nested_str_weak(current),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(DOT),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(symbol_table),
    /* K8   */  be_nested_str_weak(contains),
    /* K9   */  be_nested_str_weak(string),
    /* K10  */  be_nested_str_weak(_validate_single_parameter),
    /* K11  */  be_nested_str_weak(error),
    /* K12  */  be_nested_str_weak(Sequences_X20like_X20_X27_X25s_X27_X20do_X20not_X20have_X20properties_X2E_X20Property_X20assignments_X20are_X20only_X20valid_X20for_X20animations_X20and_X20color_X20providers_X2E),
    /* K13  */  be_nested_str_weak(expect_assign),
    /* K14  */  be_nested_str_weak(process_value),
    /* K15  */  be_nested_str_weak(property),
    /* K16  */  be_nested_str_weak(collect_inline_comment),
    /* K17  */  be_nested_str_weak(resolve_symbol_reference),
    /* K18  */  be_nested_str_weak(def_X20_X28engine_X29_X20_X25s_X2E_X25s_X20_X3D_X20_X25s_X20end),
    /* K19  */  be_nested_str_weak(add),
    /* K20  */  be_nested_str_weak(_X25s_X25s_X2Epush_X28animation_X2Ecreate_assign_step_X28_X25s_X29_X29_X25s),
    /* K21  */  be_nested_str_weak(Expected_X20property_X20assignment_X20for_X20_X27_X25s_X27_X20but_X20found_X20no_X20dot),
    /* K22  */  be_nested_str_weak(skip_statement),
    }),
    be_str_weak(process_sequence_assignment_generic),
    &be_const_str_solidified,
    ( &(const binstruction[82]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x7C0C0200,  //  0001  CALL	R3	1
      0x8C100101,  //  0002  GETMET	R4	R0	K1
      0x7C100200,  //  0003  CALL	R4	1
      0x4C140000,  //  0004  LDNIL	R5
      0x20100805,  //  0005  NE	R4	R4	R5
      0x78120041,  //  0006  JMPF	R4	#0049
      0x8C100101,  //  0007  GETMET	R4	R0	K1
      0x7C100200,  //  0008  CALL	R4	1
      0x88100902,  //  0009  GETMBR	R4	R4	K2
      0xB8160600,  //  000A  GETNGBL	R5	K3
      0x88140B04,  //  000B  GETMBR	R5	R5	K4
      0x88140B05,  //  000C  GETMBR	R5	R5	K5
      0x1C100805,  //  000D  EQ	R4	R4	R5
      0x78120039,  //  000E  JMPF	R4	#0049
      0x8C100106,  //  000F  GETMET	R4	R0	K6
      0x7C100200,  //  0010  CALL	R4	1
      0x8C100100,  //  0011  GETMET	R4	R0	K0
      0x7C100200,  //  0012  CALL	R4	1
      0x88140107,  //  0013  GETMBR	R5	R0	K7
      0x8C140B08,  //  0014  GETMET	R5	R5	K8
      0x5C1C0600,  //  0015  MOVE	R7	R3
      0x7C140400,  //  0016  CALL	R5	2
      0x78160016,  //  0017  JMPF	R5	#002F
      0x88140107,  //  0018  GETMBR	R5	R0	K7
      0x94140A03,  //  0019  GETIDX	R5	R5	R3
      0x60180004,  //  001A  GETGBL	R6	G4
      0x5C1C0A00,  //  001B  MOVE	R7	R5
      0x7C180200,  //  001C  CALL	R6	1
      0x20180D09,  //  001D  NE	R6	R6	K9
      0x781A0008,  //  001E  JMPF	R6	#0028
      0x60180005,  //  001F  GETGBL	R6	G5
      0x5C1C0A00,  //  0020  MOVE	R7	R5
      0x7C180200,  //  0021  CALL	R6	1
      0x8C1C010A,  //  0022  GETMET	R7	R0	K10
      0x5C240C00,  //  0023  MOVE	R9	R6
      0x5C280800,  //  0024  MOVE	R10	R4
      0x5C2C0A00,  //  0025  MOVE	R11	R5
      0x7C1C0800,  //  0026  CALL	R7	4
      0x70020006,  //  0027  JMP		#002F
      0x8C18010B,  //  0028  GETMET	R6	R0	K11
      0x60200018,  //  0029  GETGBL	R8	G24
      0x5824000C,  //  002A  LDCONST	R9	K12
      0x5C280600,  //  002B  MOVE	R10	R3
      0x7C200400,  //  002C  CALL	R8	2
      0x7C180400,  //  002D  CALL	R6	2
      0x80000C00,  //  002E  RET	0
      0x8C14010D,  //  002F  GETMET	R5	R0	K13
      0x7C140200,  //  0030  CALL	R5	1
      0x8C14010E,  //  0031  GETMET	R5	R0	K14
      0x581C000F,  //  0032  LDCONST	R7	K15
      0x7C140400,  //  0033  CALL	R5	2
      0x8C180110,  //  0034  GETMET	R6	R0	K16
      0x7C180200,  //  0035  CALL	R6	1
      0x8C1C0111,  //  0036  GETMET	R7	R0	K17
      0x5C240600,  //  0037  MOVE	R9	R3
      0x7C1C0400,  //  0038  CALL	R7	2
      0x60200018,  //  0039  GETGBL	R8	G24
      0x58240012,  //  003A  LDCONST	R9	K18
      0x5C280E00,  //  003B  MOVE	R10	R7
      0x5C2C0800,  //  003C  MOVE	R11	R4
      0x5C300A00,  //  003D  MOVE	R12	R5
      0x7C200800,  //  003E  CALL	R8	4
      0x8C240113,  //  003F  GETMET	R9	R0	K19
      0x602C0018,  //  0040  GETGBL	R11	G24
      0x58300014,  //  0041  LDCONST	R12	K20
      0x5C340200,  //  0042  MOVE	R13	R1
      0x5C380400,  //  0043  MOVE	R14	R2
      0x5C3C1000,  //  0044  MOVE	R15	R8
      0x5C400C00,  //  0045  MOVE	R16	R6
      0x7C2C0A00,  //  0046  CALL	R11	5
      0x7C240400,  //  0047  CALL	R9	2
      0x70020007,  //  0048  JMP		#0051
      0x8C10010B,  //  0049  GETMET	R4	R0	K11
      0x60180018,  //  004A  GETGBL	R6	G24
      0x581C0015,  //  004B  LDCONST	R7	K21
      0x5C200600,  //  004C  MOVE	R8	R3
      0x7C180400,  //  004D  CALL	R6	2
      0x7C100400,  //  004E  CALL	R4	2
      0x8C100116,  //  004F  GETMET	R4	R0	K22
      0x7C100200,  //  0050  CALL	R4	1
      0x80000000,  //  0051  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _generate_anonymous_function_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__generate_anonymous_function_call,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        10,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 2),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(push),
        /* K1   */  be_nested_str_weak(_X20_X20provider_X2E_X25s_X20_X3D_X20_X25s_X25s),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x60140018,  //  0002  GETGBL	R5	G24
          0x58180001,  //  0003  LDCONST	R6	K1
          0x5C1C0000,  //  0004  MOVE	R7	R0
          0x5C200200,  //  0005  MOVE	R8	R1
          0x5C240400,  //  0006  MOVE	R9	R2
          0x7C140800,  //  0007  CALL	R5	4
          0x7C0C0400,  //  0008  CALL	R3	2
          0x80000000,  //  0009  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(push),
    /* K1   */  be_nested_str_weak(_X28def_X20_X28engine_X29),
    /* K2   */  be_nested_str_weak(_X20_X20var_X20provider_X20_X3D_X20animation_X2E_X25s_X28engine_X29),
    /* K3   */  be_nested_str_weak(expect_left_paren),
    /* K4   */  be_nested_str_weak(_process_parameters_core),
    /* K5   */  be_nested_str_weak(generic),
    /* K6   */  be_nested_str_weak(expect_right_paren),
    /* K7   */  be_nested_str_weak(_X20_X20return_X20provider),
    /* K8   */  be_nested_str_weak(end_X29_X28engine_X29),
    /* K9   */  be_nested_str_weak(),
    /* K10  */  be_const_int(0),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(_X0A),
    /* K13  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_generate_anonymous_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x8C0C0500,  //  0002  GETMET	R3	R2	K0
      0x58140001,  //  0003  LDCONST	R5	K1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x8C0C0500,  //  0005  GETMET	R3	R2	K0
      0x60140018,  //  0006  GETGBL	R5	G24
      0x58180002,  //  0007  LDCONST	R6	K2
      0x5C1C0200,  //  0008  MOVE	R7	R1
      0x7C140400,  //  0009  CALL	R5	2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x8C0C0103,  //  000B  GETMET	R3	R0	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x840C0000,  //  000D  CLOSURE	R3	P0
      0x8C100104,  //  000E  GETMET	R4	R0	K4
      0x5C180200,  //  000F  MOVE	R6	R1
      0x581C0005,  //  0010  LDCONST	R7	K5
      0x5C200600,  //  0011  MOVE	R8	R3
      0x7C100800,  //  0012  CALL	R4	4
      0x8C100106,  //  0013  GETMET	R4	R0	K6
      0x7C100200,  //  0014  CALL	R4	1
      0x8C100500,  //  0015  GETMET	R4	R2	K0
      0x58180007,  //  0016  LDCONST	R6	K7
      0x7C100400,  //  0017  CALL	R4	2
      0x8C100500,  //  0018  GETMET	R4	R2	K0
      0x58180008,  //  0019  LDCONST	R6	K8
      0x7C100400,  //  001A  CALL	R4	2
      0x58100009,  //  001B  LDCONST	R4	K9
      0x60140010,  //  001C  GETGBL	R5	G16
      0x6018000C,  //  001D  GETGBL	R6	G12
      0x5C1C0400,  //  001E  MOVE	R7	R2
      0x7C180200,  //  001F  CALL	R6	1
      0x04180D0B,  //  0020  SUB	R6	R6	K11
      0x401A1406,  //  0021  CONNECT	R6	K10	R6
      0x7C140200,  //  0022  CALL	R5	1
      0xA8020007,  //  0023  EXBLK	0	#002C
      0x5C180A00,  //  0024  MOVE	R6	R5
      0x7C180000,  //  0025  CALL	R6	0
      0x241C0D0A,  //  0026  GT	R7	R6	K10
      0x781E0000,  //  0027  JMPF	R7	#0029
      0x0010090C,  //  0028  ADD	R4	R4	K12
      0x941C0406,  //  0029  GETIDX	R7	R2	R6
      0x00100807,  //  002A  ADD	R4	R4	R7
      0x7001FFF7,  //  002B  JMP		#0024
      0x5814000D,  //  002C  LDCONST	R5	K13
      0xAC140200,  //  002D  CATCH	R5	1	0
      0xB0080000,  //  002E  RAISE	2	R0	R0
      0xA0000000,  //  002F  CLOSE	R0
      0x80040800,  //  0030  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_computation_closure_from_string
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_create_computation_closure_from_string,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(transform_expression_for_closure),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(_X20_X20),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(replace),
    /* K6   */  be_nested_str_weak(_X20),
    /* K7   */  be_nested_str_weak(def_X20_X28engine_X29_X20return_X20_X25s_X20end),
    /* K8   */  be_nested_str_weak(animation_X2Ecreate_closure_value_X28engine_X2C_X20_X25s_X29),
    }),
    be_str_weak(create_computation_closure_from_string),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0101,  //  0001  GETMET	R3	R0	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x8C100502,  //  0004  GETMET	R4	R2	K2
      0x5C180600,  //  0005  MOVE	R6	R3
      0x581C0003,  //  0006  LDCONST	R7	K3
      0x7C100600,  //  0007  CALL	R4	3
      0x28100904,  //  0008  GE	R4	R4	K4
      0x78120006,  //  0009  JMPF	R4	#0011
      0x8C100505,  //  000A  GETMET	R4	R2	K5
      0x5C180600,  //  000B  MOVE	R6	R3
      0x581C0003,  //  000C  LDCONST	R7	K3
      0x58200006,  //  000D  LDCONST	R8	K6
      0x7C100800,  //  000E  CALL	R4	4
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x7001FFF2,  //  0010  JMP		#0004
      0x60100018,  //  0011  GETGBL	R4	G24
      0x58140007,  //  0012  LDCONST	R5	K7
      0x5C180600,  //  0013  MOVE	R6	R3
      0x7C100400,  //  0014  CALL	R4	2
      0x60140018,  //  0015  GETGBL	R5	G24
      0x58180008,  //  0016  LDCONST	R6	K8
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x7C140400,  //  0018  CALL	R5	2
      0x80040A00,  //  0019  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_unary_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_unary_expression,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Expected_X20value),
    /* K3   */  be_nested_str_weak(nil),
    /* K4   */  be_nested_str_weak(type),
    /* K5   */  be_nested_str_weak(animation_dsl),
    /* K6   */  be_nested_str_weak(Token),
    /* K7   */  be_nested_str_weak(MINUS),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(process_unary_expression),
    /* K10  */  be_nested_str_weak(_X28_X2D_X25s_X29),
    /* K11  */  be_nested_str_weak(PLUS),
    /* K12  */  be_nested_str_weak(process_primary_expression),
    }),
    be_str_weak(process_unary_expression),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x7C100200,  //  0001  CALL	R4	1
      0x4C140000,  //  0002  LDNIL	R5
      0x1C140805,  //  0003  EQ	R5	R4	R5
      0x78160003,  //  0004  JMPF	R5	#0009
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x581C0002,  //  0006  LDCONST	R7	K2
      0x7C140400,  //  0007  CALL	R5	2
      0x80060600,  //  0008  RET	1	K3
      0x88140904,  //  0009  GETMBR	R5	R4	K4
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x88180D07,  //  000C  GETMBR	R6	R6	K7
      0x1C140A06,  //  000D  EQ	R5	R5	R6
      0x7816000B,  //  000E  JMPF	R5	#001B
      0x8C140108,  //  000F  GETMET	R5	R0	K8
      0x7C140200,  //  0010  CALL	R5	1
      0x8C140109,  //  0011  GETMET	R5	R0	K9
      0x5C1C0200,  //  0012  MOVE	R7	R1
      0x50200000,  //  0013  LDBOOL	R8	0	0
      0x5C240600,  //  0014  MOVE	R9	R3
      0x7C140800,  //  0015  CALL	R5	4
      0x60180018,  //  0016  GETGBL	R6	G24
      0x581C000A,  //  0017  LDCONST	R7	K10
      0x5C200A00,  //  0018  MOVE	R8	R5
      0x7C180400,  //  0019  CALL	R6	2
      0x80040C00,  //  001A  RET	1	R6
      0x88140904,  //  001B  GETMBR	R5	R4	K4
      0xB81A0A00,  //  001C  GETNGBL	R6	K5
      0x88180D06,  //  001D  GETMBR	R6	R6	K6
      0x88180D0B,  //  001E  GETMBR	R6	R6	K11
      0x1C140A06,  //  001F  EQ	R5	R5	R6
      0x78160007,  //  0020  JMPF	R5	#0029
      0x8C140108,  //  0021  GETMET	R5	R0	K8
      0x7C140200,  //  0022  CALL	R5	1
      0x8C140109,  //  0023  GETMET	R5	R0	K9
      0x5C1C0200,  //  0024  MOVE	R7	R1
      0x50200000,  //  0025  LDBOOL	R8	0	0
      0x5C240600,  //  0026  MOVE	R9	R3
      0x7C140800,  //  0027  CALL	R5	4
      0x80040A00,  //  0028  RET	1	R5
      0x8C14010C,  //  0029  GETMET	R5	R0	K12
      0x5C1C0200,  //  002A  MOVE	R7	R1
      0x5C200400,  //  002B  MOVE	R8	R2
      0x5C240600,  //  002C  MOVE	R9	R3
      0x7C140800,  //  002D  CALL	R5	4
      0x80040A00,  //  002E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_primary_expression
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_primary_expression,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[62]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Expected_X20value),
    /* K3   */  be_nested_str_weak(nil),
    /* K4   */  be_nested_str_weak(type),
    /* K5   */  be_nested_str_weak(animation_dsl),
    /* K6   */  be_nested_str_weak(Token),
    /* K7   */  be_nested_str_weak(LEFT_PAREN),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(process_additive_expression),
    /* K10  */  be_nested_str_weak(expect_right_paren),
    /* K11  */  be_nested_str_weak(_X28_X25s_X29),
    /* K12  */  be_nested_str_weak(KEYWORD),
    /* K13  */  be_nested_str_weak(IDENTIFIER),
    /* K14  */  be_nested_str_weak(peek),
    /* K15  */  be_nested_str_weak(value),
    /* K16  */  be_nested_str_weak(is_math_method),
    /* K17  */  be_nested_str_weak(process_function_arguments),
    /* K18  */  be_nested_str_weak(animation_X2E_math_X2E_X25s_X28_X25s_X29),
    /* K19  */  be_nested_str_weak(log),
    /* K20  */  be_nested_str_weak(process_log_call),
    /* K21  */  be_nested_str_weak(expression),
    /* K22  */  be_nested_str_weak(),
    /* K23  */  be_nested_str_weak(template_definitions),
    /* K24  */  be_nested_str_weak(contains),
    /* K25  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K26  */  be_nested_str_weak(engine),
    /* K27  */  be_nested_str_weak(_X25s_template_X28_X25s_X29),
    /* K28  */  be_nested_str_weak(Function_X20_X27_X25s_X27_X20not_X20supported_X20in_X20expression_X20context),
    /* K29  */  be_nested_str_weak(_is_simple_function_call),
    /* K30  */  be_nested_str_weak(process_function_call),
    /* K31  */  be_nested_str_weak(argument),
    /* K32  */  be_nested_str_weak(property),
    /* K33  */  be_nested_str_weak(variable),
    /* K34  */  be_nested_str_weak(process_nested_function_call),
    /* K35  */  be_nested_str_weak(COLOR),
    /* K36  */  be_nested_str_weak(convert_color),
    /* K37  */  be_nested_str_weak(TIME),
    /* K38  */  be_nested_str_weak(process_time_value),
    /* K39  */  be_nested_str_weak(PERCENTAGE),
    /* K40  */  be_nested_str_weak(process_percentage_value),
    /* K41  */  be_nested_str_weak(NUMBER),
    /* K42  */  be_nested_str_weak(STRING),
    /* K43  */  be_nested_str_weak(_X22_X25s_X22),
    /* K44  */  be_nested_str_weak(LEFT_BRACKET),
    /* K45  */  be_nested_str_weak(process_array_literal),
    /* K46  */  be_nested_str_weak(DOT),
    /* K47  */  be_nested_str_weak(expect_identifier),
    /* K48  */  be_nested_str_weak(user),
    /* K49  */  be_nested_str_weak(_process_user_function_call),
    /* K50  */  be_nested_str_weak(symbol_table),
    /* K51  */  be_nested_str_weak(string),
    /* K52  */  be_nested_str_weak(_validate_single_parameter),
    /* K53  */  be_nested_str_weak(Sequences_X20like_X20_X27_X25s_X27_X20do_X20not_X20have_X20properties_X2E_X20Property_X20references_X20are_X20only_X20valid_X20for_X20animations_X20and_X20color_X20providers_X2E),
    /* K54  */  be_nested_str_weak(resolve_symbol_reference),
    /* K55  */  be_nested_str_weak(repeat_count),
    /* K56  */  be_nested_str_weak(_X25s_X2E_X25s),
    /* K57  */  be_nested_str_weak(animation_X2Eresolve_X28_X25s_X2C_X20_X27_X25s_X27_X29),
    /* K58  */  be_nested_str_weak(true),
    /* K59  */  be_nested_str_weak(false),
    /* K60  */  be_nested_str_weak(animation_X2E_X25s),
    /* K61  */  be_nested_str_weak(Unexpected_X20value_X3A_X20_X25s),
    }),
    be_str_weak(process_primary_expression),
    &be_const_str_solidified,
    ( &(const binstruction[328]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x7C100200,  //  0001  CALL	R4	1
      0x4C140000,  //  0002  LDNIL	R5
      0x1C140805,  //  0003  EQ	R5	R4	R5
      0x78160003,  //  0004  JMPF	R5	#0009
      0x8C140101,  //  0005  GETMET	R5	R0	K1
      0x581C0002,  //  0006  LDCONST	R7	K2
      0x7C140400,  //  0007  CALL	R5	2
      0x80060600,  //  0008  RET	1	K3
      0x88140904,  //  0009  GETMBR	R5	R4	K4
      0xB81A0A00,  //  000A  GETNGBL	R6	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x88180D07,  //  000C  GETMBR	R6	R6	K7
      0x1C140A06,  //  000D  EQ	R5	R5	R6
      0x7816000D,  //  000E  JMPF	R5	#001D
      0x8C140108,  //  000F  GETMET	R5	R0	K8
      0x7C140200,  //  0010  CALL	R5	1
      0x8C140109,  //  0011  GETMET	R5	R0	K9
      0x5C1C0200,  //  0012  MOVE	R7	R1
      0x50200000,  //  0013  LDBOOL	R8	0	0
      0x5C240600,  //  0014  MOVE	R9	R3
      0x7C140800,  //  0015  CALL	R5	4
      0x8C18010A,  //  0016  GETMET	R6	R0	K10
      0x7C180200,  //  0017  CALL	R6	1
      0x60180018,  //  0018  GETGBL	R6	G24
      0x581C000B,  //  0019  LDCONST	R7	K11
      0x5C200A00,  //  001A  MOVE	R8	R5
      0x7C180400,  //  001B  CALL	R6	2
      0x80040C00,  //  001C  RET	1	R6
      0x88140904,  //  001D  GETMBR	R5	R4	K4
      0xB81A0A00,  //  001E  GETNGBL	R6	K5
      0x88180D06,  //  001F  GETMBR	R6	R6	K6
      0x88180D0C,  //  0020  GETMBR	R6	R6	K12
      0x1C140A06,  //  0021  EQ	R5	R5	R6
      0x74160005,  //  0022  JMPT	R5	#0029
      0x88140904,  //  0023  GETMBR	R5	R4	K4
      0xB81A0A00,  //  0024  GETNGBL	R6	K5
      0x88180D06,  //  0025  GETMBR	R6	R6	K6
      0x88180D0D,  //  0026  GETMBR	R6	R6	K13
      0x1C140A06,  //  0027  EQ	R5	R5	R6
      0x7816005D,  //  0028  JMPF	R5	#0087
      0x8C14010E,  //  0029  GETMET	R5	R0	K14
      0x7C140200,  //  002A  CALL	R5	1
      0x4C180000,  //  002B  LDNIL	R6
      0x20140A06,  //  002C  NE	R5	R5	R6
      0x78160058,  //  002D  JMPF	R5	#0087
      0x8C14010E,  //  002E  GETMET	R5	R0	K14
      0x7C140200,  //  002F  CALL	R5	1
      0x88140B04,  //  0030  GETMBR	R5	R5	K4
      0xB81A0A00,  //  0031  GETNGBL	R6	K5
      0x88180D06,  //  0032  GETMBR	R6	R6	K6
      0x88180D07,  //  0033  GETMBR	R6	R6	K7
      0x1C140A06,  //  0034  EQ	R5	R5	R6
      0x78160050,  //  0035  JMPF	R5	#0087
      0x8814090F,  //  0036  GETMBR	R5	R4	K15
      0x780E0037,  //  0037  JMPF	R3	#0070
      0x8C180108,  //  0038  GETMET	R6	R0	K8
      0x7C180200,  //  0039  CALL	R6	1
      0x8C180110,  //  003A  GETMET	R6	R0	K16
      0x5C200A00,  //  003B  MOVE	R8	R5
      0x7C180400,  //  003C  CALL	R6	2
      0x781A0008,  //  003D  JMPF	R6	#0047
      0x8C180111,  //  003E  GETMET	R6	R0	K17
      0x50200200,  //  003F  LDBOOL	R8	1	0
      0x7C180400,  //  0040  CALL	R6	2
      0x601C0018,  //  0041  GETGBL	R7	G24
      0x58200012,  //  0042  LDCONST	R8	K18
      0x5C240A00,  //  0043  MOVE	R9	R5
      0x5C280C00,  //  0044  MOVE	R10	R6
      0x7C1C0600,  //  0045  CALL	R7	3
      0x80040E00,  //  0046  RET	1	R7
      0x1C180B13,  //  0047  EQ	R6	R5	K19
      0x781A0008,  //  0048  JMPF	R6	#0052
      0x8C180111,  //  0049  GETMET	R6	R0	K17
      0x50200200,  //  004A  LDBOOL	R8	1	0
      0x7C180400,  //  004B  CALL	R6	2
      0x8C1C0114,  //  004C  GETMET	R7	R0	K20
      0x5C240C00,  //  004D  MOVE	R9	R6
      0x58280015,  //  004E  LDCONST	R10	K21
      0x582C0016,  //  004F  LDCONST	R11	K22
      0x7C1C0800,  //  0050  CALL	R7	4
      0x80040E00,  //  0051  RET	1	R7
      0x88180117,  //  0052  GETMBR	R6	R0	K23
      0x8C180D18,  //  0053  GETMET	R6	R6	K24
      0x5C200A00,  //  0054  MOVE	R8	R5
      0x7C180400,  //  0055  CALL	R6	2
      0x781A0010,  //  0056  JMPF	R6	#0068
      0x8C180111,  //  0057  GETMET	R6	R0	K17
      0x50200200,  //  0058  LDBOOL	R8	1	0
      0x7C180400,  //  0059  CALL	R6	2
      0x201C0D16,  //  005A  NE	R7	R6	K22
      0x781E0004,  //  005B  JMPF	R7	#0061
      0x601C0018,  //  005C  GETGBL	R7	G24
      0x58200019,  //  005D  LDCONST	R8	K25
      0x5C240C00,  //  005E  MOVE	R9	R6
      0x7C1C0400,  //  005F  CALL	R7	2
      0x70020000,  //  0060  JMP		#0062
      0x581C001A,  //  0061  LDCONST	R7	K26
      0x60200018,  //  0062  GETGBL	R8	G24
      0x5824001B,  //  0063  LDCONST	R9	K27
      0x5C280A00,  //  0064  MOVE	R10	R5
      0x5C2C0E00,  //  0065  MOVE	R11	R7
      0x7C200600,  //  0066  CALL	R8	3
      0x80041000,  //  0067  RET	1	R8
      0x8C180101,  //  0068  GETMET	R6	R0	K1
      0x60200018,  //  0069  GETGBL	R8	G24
      0x5824001C,  //  006A  LDCONST	R9	K28
      0x5C280A00,  //  006B  MOVE	R10	R5
      0x7C200400,  //  006C  CALL	R8	2
      0x7C180400,  //  006D  CALL	R6	2
      0x80060600,  //  006E  RET	1	K3
      0x70020016,  //  006F  JMP		#0087
      0x8C18011D,  //  0070  GETMET	R6	R0	K29
      0x5C200A00,  //  0071  MOVE	R8	R5
      0x7C180400,  //  0072  CALL	R6	2
      0x781A0004,  //  0073  JMPF	R6	#0079
      0x8C18011E,  //  0074  GETMET	R6	R0	K30
      0x5C200200,  //  0075  MOVE	R8	R1
      0x7C180400,  //  0076  CALL	R6	2
      0x80040C00,  //  0077  RET	1	R6
      0x7002000D,  //  0078  JMP		#0087
      0x1C18031F,  //  0079  EQ	R6	R1	K31
      0x741A0003,  //  007A  JMPT	R6	#007F
      0x1C180320,  //  007B  EQ	R6	R1	K32
      0x741A0001,  //  007C  JMPT	R6	#007F
      0x1C180321,  //  007D  EQ	R6	R1	K33
      0x781A0003,  //  007E  JMPF	R6	#0083
      0x8C180122,  //  007F  GETMET	R6	R0	K34
      0x7C180200,  //  0080  CALL	R6	1
      0x80040C00,  //  0081  RET	1	R6
      0x70020003,  //  0082  JMP		#0087
      0x8C18011E,  //  0083  GETMET	R6	R0	K30
      0x5C200200,  //  0084  MOVE	R8	R1
      0x7C180400,  //  0085  CALL	R6	2
      0x80040C00,  //  0086  RET	1	R6
      0x88140904,  //  0087  GETMBR	R5	R4	K4
      0xB81A0A00,  //  0088  GETNGBL	R6	K5
      0x88180D06,  //  0089  GETMBR	R6	R6	K6
      0x88180D23,  //  008A  GETMBR	R6	R6	K35
      0x1C140A06,  //  008B  EQ	R5	R5	R6
      0x78160005,  //  008C  JMPF	R5	#0093
      0x8C140108,  //  008D  GETMET	R5	R0	K8
      0x7C140200,  //  008E  CALL	R5	1
      0x8C140124,  //  008F  GETMET	R5	R0	K36
      0x881C090F,  //  0090  GETMBR	R7	R4	K15
      0x7C140400,  //  0091  CALL	R5	2
      0x80040A00,  //  0092  RET	1	R5
      0x88140904,  //  0093  GETMBR	R5	R4	K4
      0xB81A0A00,  //  0094  GETNGBL	R6	K5
      0x88180D06,  //  0095  GETMBR	R6	R6	K6
      0x88180D25,  //  0096  GETMBR	R6	R6	K37
      0x1C140A06,  //  0097  EQ	R5	R5	R6
      0x78160004,  //  0098  JMPF	R5	#009E
      0x60140008,  //  0099  GETGBL	R5	G8
      0x8C180126,  //  009A  GETMET	R6	R0	K38
      0x7C180200,  //  009B  CALL	R6	1
      0x7C140200,  //  009C  CALL	R5	1
      0x80040A00,  //  009D  RET	1	R5
      0x88140904,  //  009E  GETMBR	R5	R4	K4
      0xB81A0A00,  //  009F  GETNGBL	R6	K5
      0x88180D06,  //  00A0  GETMBR	R6	R6	K6
      0x88180D27,  //  00A1  GETMBR	R6	R6	K39
      0x1C140A06,  //  00A2  EQ	R5	R5	R6
      0x78160004,  //  00A3  JMPF	R5	#00A9
      0x60140008,  //  00A4  GETGBL	R5	G8
      0x8C180128,  //  00A5  GETMET	R6	R0	K40
      0x7C180200,  //  00A6  CALL	R6	1
      0x7C140200,  //  00A7  CALL	R5	1
      0x80040A00,  //  00A8  RET	1	R5
      0x88140904,  //  00A9  GETMBR	R5	R4	K4
      0xB81A0A00,  //  00AA  GETNGBL	R6	K5
      0x88180D06,  //  00AB  GETMBR	R6	R6	K6
      0x88180D29,  //  00AC  GETMBR	R6	R6	K41
      0x1C140A06,  //  00AD  EQ	R5	R5	R6
      0x78160003,  //  00AE  JMPF	R5	#00B3
      0x8814090F,  //  00AF  GETMBR	R5	R4	K15
      0x8C180108,  //  00B0  GETMET	R6	R0	K8
      0x7C180200,  //  00B1  CALL	R6	1
      0x80040A00,  //  00B2  RET	1	R5
      0x88140904,  //  00B3  GETMBR	R5	R4	K4
      0xB81A0A00,  //  00B4  GETNGBL	R6	K5
      0x88180D06,  //  00B5  GETMBR	R6	R6	K6
      0x88180D2A,  //  00B6  GETMBR	R6	R6	K42
      0x1C140A06,  //  00B7  EQ	R5	R5	R6
      0x78160007,  //  00B8  JMPF	R5	#00C1
      0x8814090F,  //  00B9  GETMBR	R5	R4	K15
      0x8C180108,  //  00BA  GETMET	R6	R0	K8
      0x7C180200,  //  00BB  CALL	R6	1
      0x60180018,  //  00BC  GETGBL	R6	G24
      0x581C002B,  //  00BD  LDCONST	R7	K43
      0x5C200A00,  //  00BE  MOVE	R8	R5
      0x7C180400,  //  00BF  CALL	R6	2
      0x80040C00,  //  00C0  RET	1	R6
      0x88140904,  //  00C1  GETMBR	R5	R4	K4
      0xB81A0A00,  //  00C2  GETNGBL	R6	K5
      0x88180D06,  //  00C3  GETMBR	R6	R6	K6
      0x88180D2C,  //  00C4  GETMBR	R6	R6	K44
      0x1C140A06,  //  00C5  EQ	R5	R5	R6
      0x78160004,  //  00C6  JMPF	R5	#00CC
      0x5C140600,  //  00C7  MOVE	R5	R3
      0x74160002,  //  00C8  JMPT	R5	#00CC
      0x8C14012D,  //  00C9  GETMET	R5	R0	K45
      0x7C140200,  //  00CA  CALL	R5	1
      0x80040A00,  //  00CB  RET	1	R5
      0x88140904,  //  00CC  GETMBR	R5	R4	K4
      0xB81A0A00,  //  00CD  GETNGBL	R6	K5
      0x88180D06,  //  00CE  GETMBR	R6	R6	K6
      0x88180D0D,  //  00CF  GETMBR	R6	R6	K13
      0x1C140A06,  //  00D0  EQ	R5	R5	R6
      0x7816004E,  //  00D1  JMPF	R5	#0121
      0x8814090F,  //  00D2  GETMBR	R5	R4	K15
      0x8C180108,  //  00D3  GETMET	R6	R0	K8
      0x7C180200,  //  00D4  CALL	R6	1
      0x8C180100,  //  00D5  GETMET	R6	R0	K0
      0x7C180200,  //  00D6  CALL	R6	1
      0x4C1C0000,  //  00D7  LDNIL	R7
      0x20180C07,  //  00D8  NE	R6	R6	R7
      0x781A0042,  //  00D9  JMPF	R6	#011D
      0x8C180100,  //  00DA  GETMET	R6	R0	K0
      0x7C180200,  //  00DB  CALL	R6	1
      0x88180D04,  //  00DC  GETMBR	R6	R6	K4
      0xB81E0A00,  //  00DD  GETNGBL	R7	K5
      0x881C0F06,  //  00DE  GETMBR	R7	R7	K6
      0x881C0F2E,  //  00DF  GETMBR	R7	R7	K46
      0x1C180C07,  //  00E0  EQ	R6	R6	R7
      0x781A003A,  //  00E1  JMPF	R6	#011D
      0x8C180108,  //  00E2  GETMET	R6	R0	K8
      0x7C180200,  //  00E3  CALL	R6	1
      0x8C18012F,  //  00E4  GETMET	R6	R0	K47
      0x7C180200,  //  00E5  CALL	R6	1
      0x1C1C0B30,  //  00E6  EQ	R7	R5	K48
      0x781E0003,  //  00E7  JMPF	R7	#00EC
      0x8C1C0131,  //  00E8  GETMET	R7	R0	K49
      0x5C240C00,  //  00E9  MOVE	R9	R6
      0x7C1C0400,  //  00EA  CALL	R7	2
      0x80040E00,  //  00EB  RET	1	R7
      0x5C1C0600,  //  00EC  MOVE	R7	R3
      0x741E001B,  //  00ED  JMPT	R7	#010A
      0x881C0132,  //  00EE  GETMBR	R7	R0	K50
      0x8C1C0F18,  //  00EF  GETMET	R7	R7	K24
      0x5C240A00,  //  00F0  MOVE	R9	R5
      0x7C1C0400,  //  00F1  CALL	R7	2
      0x781E0016,  //  00F2  JMPF	R7	#010A
      0x881C0132,  //  00F3  GETMBR	R7	R0	K50
      0x941C0E05,  //  00F4  GETIDX	R7	R7	R5
      0x60200004,  //  00F5  GETGBL	R8	G4
      0x5C240E00,  //  00F6  MOVE	R9	R7
      0x7C200200,  //  00F7  CALL	R8	1
      0x20201133,  //  00F8  NE	R8	R8	K51
      0x78220008,  //  00F9  JMPF	R8	#0103
      0x60200005,  //  00FA  GETGBL	R8	G5
      0x5C240E00,  //  00FB  MOVE	R9	R7
      0x7C200200,  //  00FC  CALL	R8	1
      0x8C240134,  //  00FD  GETMET	R9	R0	K52
      0x5C2C1000,  //  00FE  MOVE	R11	R8
      0x5C300C00,  //  00FF  MOVE	R12	R6
      0x5C340E00,  //  0100  MOVE	R13	R7
      0x7C240800,  //  0101  CALL	R9	4
      0x70020006,  //  0102  JMP		#010A
      0x8C200101,  //  0103  GETMET	R8	R0	K1
      0x60280018,  //  0104  GETGBL	R10	G24
      0x582C0035,  //  0105  LDCONST	R11	K53
      0x5C300A00,  //  0106  MOVE	R12	R5
      0x7C280400,  //  0107  CALL	R10	2
      0x7C200400,  //  0108  CALL	R8	2
      0x80060600,  //  0109  RET	1	K3
      0x8C1C0136,  //  010A  GETMET	R7	R0	K54
      0x5C240A00,  //  010B  MOVE	R9	R5
      0x7C1C0400,  //  010C  CALL	R7	2
      0x740E0001,  //  010D  JMPT	R3	#0110
      0x1C200337,  //  010E  EQ	R8	R1	K55
      0x78220006,  //  010F  JMPF	R8	#0117
      0x60200018,  //  0110  GETGBL	R8	G24
      0x58240038,  //  0111  LDCONST	R9	K56
      0x5C280E00,  //  0112  MOVE	R10	R7
      0x5C2C0C00,  //  0113  MOVE	R11	R6
      0x7C200600,  //  0114  CALL	R8	3
      0x80041000,  //  0115  RET	1	R8
      0x70020005,  //  0116  JMP		#011D
      0x60200018,  //  0117  GETGBL	R8	G24
      0x58240039,  //  0118  LDCONST	R9	K57
      0x5C280E00,  //  0119  MOVE	R10	R7
      0x5C2C0C00,  //  011A  MOVE	R11	R6
      0x7C200600,  //  011B  CALL	R8	3
      0x80041000,  //  011C  RET	1	R8
      0x8C180136,  //  011D  GETMET	R6	R0	K54
      0x5C200A00,  //  011E  MOVE	R8	R5
      0x7C180400,  //  011F  CALL	R6	2
      0x80040C00,  //  0120  RET	1	R6
      0x88140904,  //  0121  GETMBR	R5	R4	K4
      0xB81A0A00,  //  0122  GETNGBL	R6	K5
      0x88180D06,  //  0123  GETMBR	R6	R6	K6
      0x88180D0C,  //  0124  GETMBR	R6	R6	K12
      0x1C140A06,  //  0125  EQ	R5	R5	R6
      0x78160009,  //  0126  JMPF	R5	#0131
      0x8814090F,  //  0127  GETMBR	R5	R4	K15
      0x1C140B3A,  //  0128  EQ	R5	R5	K58
      0x74160002,  //  0129  JMPT	R5	#012D
      0x8814090F,  //  012A  GETMBR	R5	R4	K15
      0x1C140B3B,  //  012B  EQ	R5	R5	K59
      0x78160003,  //  012C  JMPF	R5	#0131
      0x8814090F,  //  012D  GETMBR	R5	R4	K15
      0x8C180108,  //  012E  GETMET	R6	R0	K8
      0x7C180200,  //  012F  CALL	R6	1
      0x80040A00,  //  0130  RET	1	R5
      0x88140904,  //  0131  GETMBR	R5	R4	K4
      0xB81A0A00,  //  0132  GETNGBL	R6	K5
      0x88180D06,  //  0133  GETMBR	R6	R6	K6
      0x88180D0C,  //  0134  GETMBR	R6	R6	K12
      0x1C140A06,  //  0135  EQ	R5	R5	R6
      0x78160007,  //  0136  JMPF	R5	#013F
      0x8814090F,  //  0137  GETMBR	R5	R4	K15
      0x8C180108,  //  0138  GETMET	R6	R0	K8
      0x7C180200,  //  0139  CALL	R6	1
      0x60180018,  //  013A  GETGBL	R6	G24
      0x581C003C,  //  013B  LDCONST	R7	K60
      0x5C200A00,  //  013C  MOVE	R8	R5
      0x7C180400,  //  013D  CALL	R6	2
      0x80040C00,  //  013E  RET	1	R6
      0x8C140101,  //  013F  GETMET	R5	R0	K1
      0x601C0018,  //  0140  GETGBL	R7	G24
      0x5820003D,  //  0141  LDCONST	R8	K61
      0x8824090F,  //  0142  GETMBR	R9	R4	K15
      0x7C1C0400,  //  0143  CALL	R7	2
      0x7C140400,  //  0144  CALL	R5	2
      0x8C140108,  //  0145  GETMET	R5	R0	K8
      0x7C140200,  //  0146  CALL	R5	1
      0x80060600,  //  0147  RET	1	K3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_dot
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_dot,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(DOT),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X2E_X27),
    }),
    be_str_weak(expect_dot),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_identifier_char
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_is_identifier_char,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(a),
    /* K1   */  be_nested_str_weak(z),
    /* K2   */  be_nested_str_weak(A),
    /* K3   */  be_nested_str_weak(Z),
    /* K4   */  be_nested_str_weak(0),
    /* K5   */  be_nested_str_weak(9),
    /* K6   */  be_nested_str_weak(_),
    }),
    be_str_weak(is_identifier_char),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x28080300,  //  0000  GE	R2	R1	K0
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x18080301,  //  0002  LE	R2	R1	K1
      0x740A000A,  //  0003  JMPT	R2	#000F
      0x28080302,  //  0004  GE	R2	R1	K2
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x18080303,  //  0006  LE	R2	R1	K3
      0x740A0006,  //  0007  JMPT	R2	#000F
      0x28080304,  //  0008  GE	R2	R1	K4
      0x780A0001,  //  0009  JMPF	R2	#000C
      0x18080305,  //  000A  LE	R2	R1	K5
      0x740A0002,  //  000B  JMPT	R2	#000F
      0x1C080306,  //  000C  EQ	R2	R1	K6
      0x740A0000,  //  000D  JMPT	R2	#000F
      0x50080001,  //  000E  LDBOOL	R2	0	1
      0x50080200,  //  000F  LDBOOL	R2	1	0
      0x80040400,  //  0010  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_template_function
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_generate_template_function,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(engine),
    /* K2   */  be_nested_str_weak(_X2C_X20_X25s_),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(add),
    /* K5   */  be_nested_str_weak(_X23_X20Template_X20function_X3A_X20_X25s),
    /* K6   */  be_nested_str_weak(def_X20_X25s_template_X28_X25s_X29),
    /* K7   */  be_nested_str_weak(animation_dsl),
    /* K8   */  be_nested_str_weak(SimpleDSLTranspiler),
    /* K9   */  be_nested_str_weak(symbol_table),
    /* K10  */  be_nested_str_weak(strip_initialized),
    /* K11  */  be_nested_str_weak(parameter),
    /* K12  */  be_nested_str_weak(transpile_template_body),
    /* K13  */  be_nested_str_weak(split),
    /* K14  */  be_nested_str_weak(_X0A),
    /* K15  */  be_const_int(0),
    /* K16  */  be_nested_str_weak(_X20_X20_X25s),
    /* K17  */  be_nested_str_weak(errors),
    /* K18  */  be_nested_str_weak(error),
    /* K19  */  be_nested_str_weak(Template_X20_X27_X25s_X27_X20body_X20error_X3A_X20_X25s),
    /* K20  */  be_nested_str_weak(end),
    /* K21  */  be_nested_str_weak(),
    /* K22  */  be_nested_str_weak(animation_X2Eregister_user_function_X28_X27_X25s_X27_X2C_X20_X25s_template_X29),
    }),
    be_str_weak(generate_template_function),
    &be_const_str_solidified,
    ( &(const binstruction[116]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0x58180001,  //  0001  LDCONST	R6	K1
      0x601C0010,  //  0002  GETGBL	R7	G16
      0x5C200400,  //  0003  MOVE	R8	R2
      0x7C1C0200,  //  0004  CALL	R7	1
      0xA8020007,  //  0005  EXBLK	0	#000E
      0x5C200E00,  //  0006  MOVE	R8	R7
      0x7C200000,  //  0007  CALL	R8	0
      0x60240018,  //  0008  GETGBL	R9	G24
      0x58280002,  //  0009  LDCONST	R10	K2
      0x5C2C1000,  //  000A  MOVE	R11	R8
      0x7C240400,  //  000B  CALL	R9	2
      0x00180C09,  //  000C  ADD	R6	R6	R9
      0x7001FFF7,  //  000D  JMP		#0006
      0x581C0003,  //  000E  LDCONST	R7	K3
      0xAC1C0200,  //  000F  CATCH	R7	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x8C1C0104,  //  0011  GETMET	R7	R0	K4
      0x60240018,  //  0012  GETGBL	R9	G24
      0x58280005,  //  0013  LDCONST	R10	K5
      0x5C2C0200,  //  0014  MOVE	R11	R1
      0x7C240400,  //  0015  CALL	R9	2
      0x7C1C0400,  //  0016  CALL	R7	2
      0x8C1C0104,  //  0017  GETMET	R7	R0	K4
      0x60240018,  //  0018  GETGBL	R9	G24
      0x58280006,  //  0019  LDCONST	R10	K6
      0x5C2C0200,  //  001A  MOVE	R11	R1
      0x5C300C00,  //  001B  MOVE	R12	R6
      0x7C240600,  //  001C  CALL	R9	3
      0x7C1C0400,  //  001D  CALL	R7	2
      0xB81E0E00,  //  001E  GETNGBL	R7	K7
      0x8C1C0F08,  //  001F  GETMET	R7	R7	K8
      0x5C240800,  //  0020  MOVE	R9	R4
      0x7C1C0400,  //  0021  CALL	R7	2
      0x60200013,  //  0022  GETGBL	R8	G19
      0x7C200000,  //  0023  CALL	R8	0
      0x901E1208,  //  0024  SETMBR	R7	K9	R8
      0x50200200,  //  0025  LDBOOL	R8	1	0
      0x901E1408,  //  0026  SETMBR	R7	K10	R8
      0x60200010,  //  0027  GETGBL	R8	G16
      0x5C240400,  //  0028  MOVE	R9	R2
      0x7C200200,  //  0029  CALL	R8	1
      0xA8020004,  //  002A  EXBLK	0	#0030
      0x5C241000,  //  002B  MOVE	R9	R8
      0x7C240000,  //  002C  CALL	R9	0
      0x88280F09,  //  002D  GETMBR	R10	R7	K9
      0x9828130B,  //  002E  SETIDX	R10	R9	K11
      0x7001FFFA,  //  002F  JMP		#002B
      0x58200003,  //  0030  LDCONST	R8	K3
      0xAC200200,  //  0031  CATCH	R8	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x8C200F0C,  //  0033  GETMET	R8	R7	K12
      0x7C200200,  //  0034  CALL	R8	1
      0x4C240000,  //  0035  LDNIL	R9
      0x20241009,  //  0036  NE	R9	R8	R9
      0x78260019,  //  0037  JMPF	R9	#0052
      0x8C240B0D,  //  0038  GETMET	R9	R5	K13
      0x5C2C1000,  //  0039  MOVE	R11	R8
      0x5830000E,  //  003A  LDCONST	R12	K14
      0x7C240600,  //  003B  CALL	R9	3
      0x60280010,  //  003C  GETGBL	R10	G16
      0x5C2C1200,  //  003D  MOVE	R11	R9
      0x7C280200,  //  003E  CALL	R10	1
      0xA802000D,  //  003F  EXBLK	0	#004E
      0x5C2C1400,  //  0040  MOVE	R11	R10
      0x7C2C0000,  //  0041  CALL	R11	0
      0x6030000C,  //  0042  GETGBL	R12	G12
      0x5C341600,  //  0043  MOVE	R13	R11
      0x7C300200,  //  0044  CALL	R12	1
      0x2430190F,  //  0045  GT	R12	R12	K15
      0x78320005,  //  0046  JMPF	R12	#004D
      0x8C300104,  //  0047  GETMET	R12	R0	K4
      0x60380018,  //  0048  GETGBL	R14	G24
      0x583C0010,  //  0049  LDCONST	R15	K16
      0x5C401600,  //  004A  MOVE	R16	R11
      0x7C380400,  //  004B  CALL	R14	2
      0x7C300400,  //  004C  CALL	R12	2
      0x7001FFF1,  //  004D  JMP		#0040
      0x58280003,  //  004E  LDCONST	R10	K3
      0xAC280200,  //  004F  CATCH	R10	1	0
      0xB0080000,  //  0050  RAISE	2	R0	R0
      0x70020010,  //  0051  JMP		#0063
      0x60240010,  //  0052  GETGBL	R9	G16
      0x88280F11,  //  0053  GETMBR	R10	R7	K17
      0x7C240200,  //  0054  CALL	R9	1
      0xA8020009,  //  0055  EXBLK	0	#0060
      0x5C281200,  //  0056  MOVE	R10	R9
      0x7C280000,  //  0057  CALL	R10	0
      0x8C2C0112,  //  0058  GETMET	R11	R0	K18
      0x60340018,  //  0059  GETGBL	R13	G24
      0x58380013,  //  005A  LDCONST	R14	K19
      0x5C3C0200,  //  005B  MOVE	R15	R1
      0x5C401400,  //  005C  MOVE	R16	R10
      0x7C340600,  //  005D  CALL	R13	3
      0x7C2C0400,  //  005E  CALL	R11	2
      0x7001FFF5,  //  005F  JMP		#0056
      0x58240003,  //  0060  LDCONST	R9	K3
      0xAC240200,  //  0061  CATCH	R9	1	0
      0xB0080000,  //  0062  RAISE	2	R0	R0
      0x8C240104,  //  0063  GETMET	R9	R0	K4
      0x582C0014,  //  0064  LDCONST	R11	K20
      0x7C240400,  //  0065  CALL	R9	2
      0x8C240104,  //  0066  GETMET	R9	R0	K4
      0x582C0015,  //  0067  LDCONST	R11	K21
      0x7C240400,  //  0068  CALL	R9	2
      0x8C240104,  //  0069  GETMET	R9	R0	K4
      0x602C0018,  //  006A  GETGBL	R11	G24
      0x58300016,  //  006B  LDCONST	R12	K22
      0x5C340200,  //  006C  MOVE	R13	R1
      0x5C380200,  //  006D  MOVE	R14	R1
      0x7C2C0600,  //  006E  CALL	R11	3
      0x7C240400,  //  006F  CALL	R9	2
      0x8C240104,  //  0070  GETMET	R9	R0	K4
      0x582C0015,  //  0071  LDCONST	R11	K21
      0x7C240400,  //  0072  CALL	R9	2
      0x80000000,  //  0073  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: convert_to_vrgb
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_convert_to_vrgb,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(format),
    /* K3   */  be_nested_str_weak(_X2502X),
    /* K4   */  be_nested_str_weak(FFFFFF),
    /* K5   */  be_nested_str_weak(startswith),
    /* K6   */  be_nested_str_weak(0x),
    /* K7   */  be_const_int(2),
    }),
    be_str_weak(convert_to_vrgb),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x60100009,  //  0001  GETGBL	R4	G9
      0x6014000A,  //  0002  GETGBL	R5	G10
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x7C100200,  //  0005  CALL	R4	1
      0x14140901,  //  0006  LT	R5	R4	K1
      0x78160001,  //  0007  JMPF	R5	#000A
      0x58100001,  //  0008  LDCONST	R4	K1
      0x70020003,  //  0009  JMP		#000E
      0x541600FE,  //  000A  LDINT	R5	255
      0x24140805,  //  000B  GT	R5	R4	R5
      0x78160000,  //  000C  JMPF	R5	#000E
      0x541200FE,  //  000D  LDINT	R4	255
      0x8C140702,  //  000E  GETMET	R5	R3	K2
      0x581C0003,  //  000F  LDCONST	R7	K3
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x60180008,  //  0012  GETGBL	R6	G8
      0x5C1C0400,  //  0013  MOVE	R7	R2
      0x7C180200,  //  0014  CALL	R6	1
      0x581C0004,  //  0015  LDCONST	R7	K4
      0x8C200705,  //  0016  GETMET	R8	R3	K5
      0x5C280C00,  //  0017  MOVE	R10	R6
      0x582C0006,  //  0018  LDCONST	R11	K6
      0x7C200600,  //  0019  CALL	R8	3
      0x7822000A,  //  001A  JMPF	R8	#0026
      0x6020000C,  //  001B  GETGBL	R8	G12
      0x5C240C00,  //  001C  MOVE	R9	R6
      0x7C200200,  //  001D  CALL	R8	1
      0x54260009,  //  001E  LDINT	R9	10
      0x28201009,  //  001F  GE	R8	R8	R9
      0x78220004,  //  0020  JMPF	R8	#0026
      0x54220003,  //  0021  LDINT	R8	4
      0x54260008,  //  0022  LDINT	R9	9
      0x40201009,  //  0023  CONNECT	R8	R8	R9
      0x941C0C08,  //  0024  GETIDX	R7	R6	R8
      0x7002000D,  //  0025  JMP		#0034
      0x8C200705,  //  0026  GETMET	R8	R3	K5
      0x5C280C00,  //  0027  MOVE	R10	R6
      0x582C0006,  //  0028  LDCONST	R11	K6
      0x7C200600,  //  0029  CALL	R8	3
      0x78220008,  //  002A  JMPF	R8	#0034
      0x6020000C,  //  002B  GETGBL	R8	G12
      0x5C240C00,  //  002C  MOVE	R9	R6
      0x7C200200,  //  002D  CALL	R8	1
      0x54260007,  //  002E  LDINT	R9	8
      0x1C201009,  //  002F  EQ	R8	R8	R9
      0x78220002,  //  0030  JMPF	R8	#0034
      0x54220006,  //  0031  LDINT	R8	7
      0x40220E08,  //  0032  CONNECT	R8	K7	R8
      0x941C0C08,  //  0033  GETIDX	R7	R6	R8
      0x00200A07,  //  0034  ADD	R8	R5	R7
      0x80041000,  //  0035  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: transpile
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_transpile,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(add),
    /* K1   */  be_nested_str_weak(import_X20animation),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(at_end),
    /* K4   */  be_nested_str_weak(process_statement),
    /* K5   */  be_nested_str_weak(generate_engine_run),
    /* K6   */  be_nested_str_weak(errors),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(join_output),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Transpilation_X20failed_X3A_X20_X25s),
    }),
    be_str_weak(transpile),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xA802001A,  //  0000  EXBLK	0	#001C
      0x8C040100,  //  0001  GETMET	R1	R0	K0
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x7C040400,  //  0003  CALL	R1	2
      0x8C040100,  //  0004  GETMET	R1	R0	K0
      0x580C0002,  //  0005  LDCONST	R3	K2
      0x7C040400,  //  0006  CALL	R1	2
      0x8C040103,  //  0007  GETMET	R1	R0	K3
      0x7C040200,  //  0008  CALL	R1	1
      0x74060002,  //  0009  JMPT	R1	#000D
      0x8C040104,  //  000A  GETMET	R1	R0	K4
      0x7C040200,  //  000B  CALL	R1	1
      0x7001FFF9,  //  000C  JMP		#0007
      0x8C040105,  //  000D  GETMET	R1	R0	K5
      0x7C040200,  //  000E  CALL	R1	1
      0x6004000C,  //  000F  GETGBL	R1	G12
      0x88080106,  //  0010  GETMBR	R2	R0	K6
      0x7C040200,  //  0011  CALL	R1	1
      0x1C040307,  //  0012  EQ	R1	R1	K7
      0x78060002,  //  0013  JMPF	R1	#0017
      0x8C040108,  //  0014  GETMET	R1	R0	K8
      0x7C040200,  //  0015  CALL	R1	1
      0x70020000,  //  0016  JMP		#0018
      0x4C040000,  //  0017  LDNIL	R1
      0xA8040001,  //  0018  EXBLK	1	1
      0x80040200,  //  0019  RET	1	R1
      0xA8040001,  //  001A  EXBLK	1	1
      0x7002000B,  //  001B  JMP		#0028
      0xAC040002,  //  001C  CATCH	R1	0	2
      0x70020008,  //  001D  JMP		#0027
      0x8C0C0109,  //  001E  GETMET	R3	R0	K9
      0x60140018,  //  001F  GETGBL	R5	G24
      0x5818000A,  //  0020  LDCONST	R6	K10
      0x5C1C0400,  //  0021  MOVE	R7	R2
      0x7C140400,  //  0022  CALL	R5	2
      0x7C0C0400,  //  0023  CALL	R3	2
      0x4C0C0000,  //  0024  LDNIL	R3
      0x80040600,  //  0025  RET	1	R3
      0x70020000,  //  0026  JMP		#0028
      0xB0080000,  //  0027  RAISE	2	R0	R0
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_color_provider_factory_exists
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_color_provider_factory_exists,   /* name */
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
    /* K0   */  be_nested_str_weak(_validate_factory_function),
    /* K1   */  be_nested_str_weak(animation),
    /* K2   */  be_nested_str_weak(color_provider),
    }),
    be_str_weak(_validate_color_provider_factory_exists),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0xB8160200,  //  0002  GETNGBL	R5	K1
      0x88140B02,  //  0003  GETMBR	R5	R5	K2
      0x7C080600,  //  0004  CALL	R2	3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_right_paren
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_check_right_paren,   /* name */
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
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(RIGHT_PAREN),
    }),
    be_str_weak(check_right_paren),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0005,  //  0004  JMPF	R2	#000B
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x740A0000,  //  000A  JMPT	R2	#000C
      0x50080001,  //  000B  LDBOOL	R2	0	1
      0x50080200,  //  000C  LDBOOL	R2	1	0
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: skip_function_arguments
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_skip_function_arguments,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_PAREN),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(at_end),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(RIGHT_PAREN),
    }),
    be_str_weak(skip_function_arguments),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20040202,  //  0003  NE	R1	R1	R2
      0x78060023,  //  0004  JMPF	R1	#0029
      0x8C040100,  //  0005  GETMET	R1	R0	K0
      0x7C040200,  //  0006  CALL	R1	1
      0x88040301,  //  0007  GETMBR	R1	R1	K1
      0xB80A0400,  //  0008  GETNGBL	R2	K2
      0x88080503,  //  0009  GETMBR	R2	R2	K3
      0x88080504,  //  000A  GETMBR	R2	R2	K4
      0x1C040202,  //  000B  EQ	R1	R1	R2
      0x7806001B,  //  000C  JMPF	R1	#0029
      0x8C040105,  //  000D  GETMET	R1	R0	K5
      0x7C040200,  //  000E  CALL	R1	1
      0x58040006,  //  000F  LDCONST	R1	K6
      0x8C080107,  //  0010  GETMET	R2	R0	K7
      0x7C080200,  //  0011  CALL	R2	1
      0x740A0015,  //  0012  JMPT	R2	#0029
      0x24080308,  //  0013  GT	R2	R1	K8
      0x780A0013,  //  0014  JMPF	R2	#0029
      0x8C080100,  //  0015  GETMET	R2	R0	K0
      0x7C080200,  //  0016  CALL	R2	1
      0x880C0501,  //  0017  GETMBR	R3	R2	K1
      0xB8120400,  //  0018  GETNGBL	R4	K2
      0x88100903,  //  0019  GETMBR	R4	R4	K3
      0x88100904,  //  001A  GETMBR	R4	R4	K4
      0x1C0C0604,  //  001B  EQ	R3	R3	R4
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x00040306,  //  001D  ADD	R1	R1	K6
      0x70020006,  //  001E  JMP		#0026
      0x880C0501,  //  001F  GETMBR	R3	R2	K1
      0xB8120400,  //  0020  GETNGBL	R4	K2
      0x88100903,  //  0021  GETMBR	R4	R4	K3
      0x88100909,  //  0022  GETMBR	R4	R4	K9
      0x1C0C0604,  //  0023  EQ	R3	R3	R4
      0x780E0000,  //  0024  JMPF	R3	#0026
      0x04040306,  //  0025  SUB	R1	R1	K6
      0x8C0C0105,  //  0026  GETMET	R3	R0	K5
      0x7C0C0200,  //  0027  CALL	R3	1
      0x7001FFE6,  //  0028  JMP		#0010
      0x80000000,  //  0029  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_named_arguments_generic
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_named_arguments_generic,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_process_named_arguments_unified),
    /* K1   */  be_nested_str_weak(generic),
    }),
    be_str_weak(_process_named_arguments_generic),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x581C0001,  //  0003  LDCONST	R7	K1
      0x7C0C0800,  //  0004  CALL	R3	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_parameters_core
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_parameters_core,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(variable),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(temp_),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(split),
    /* K6   */  be_nested_str_weak(_),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(),
    /* K10  */  be_nested_str_weak(_create_instance_for_validation),
    /* K11  */  be_nested_str_weak(at_end),
    /* K12  */  be_nested_str_weak(check_right_paren),
    /* K13  */  be_nested_str_weak(skip_whitespace_including_newlines),
    /* K14  */  be_nested_str_weak(expect_identifier),
    /* K15  */  be_nested_str_weak(_validate_single_parameter),
    /* K16  */  be_nested_str_weak(expect_assign),
    /* K17  */  be_nested_str_weak(process_value),
    /* K18  */  be_nested_str_weak(argument),
    /* K19  */  be_nested_str_weak(collect_inline_comment),
    /* K20  */  be_nested_str_weak(current),
    /* K21  */  be_nested_str_weak(type),
    /* K22  */  be_nested_str_weak(animation_dsl),
    /* K23  */  be_nested_str_weak(Token),
    /* K24  */  be_nested_str_weak(COMMENT),
    /* K25  */  be_nested_str_weak(next),
    /* K26  */  be_nested_str_weak(COMMA),
    /* K27  */  be_nested_str_weak(NEWLINE),
    /* K28  */  be_nested_str_weak(error),
    /* K29  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X29_X27_X20in_X20function_X20arguments),
    }),
    be_str_weak(_process_parameters_core),
    &be_const_str_solidified,
    ( &(const binstruction[127]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x5C140200,  //  0001  MOVE	R5	R1
      0x1C180500,  //  0002  EQ	R6	R2	K0
      0x781A0010,  //  0003  JMPF	R6	#0015
      0xA41A0200,  //  0004  IMPORT	R6	K1
      0x8C1C0D02,  //  0005  GETMET	R7	R6	K2
      0x5C240200,  //  0006  MOVE	R9	R1
      0x58280003,  //  0007  LDCONST	R10	K3
      0x7C1C0600,  //  0008  CALL	R7	3
      0x1C1C0F04,  //  0009  EQ	R7	R7	K4
      0x781E0009,  //  000A  JMPF	R7	#0015
      0x8C1C0D05,  //  000B  GETMET	R7	R6	K5
      0x5C240200,  //  000C  MOVE	R9	R1
      0x58280006,  //  000D  LDCONST	R10	K6
      0x7C1C0600,  //  000E  CALL	R7	3
      0x6020000C,  //  000F  GETGBL	R8	G12
      0x5C240E00,  //  0010  MOVE	R9	R7
      0x7C200200,  //  0011  CALL	R8	1
      0x28201107,  //  0012  GE	R8	R8	K7
      0x78220000,  //  0013  JMPF	R8	#0015
      0x94140F08,  //  0014  GETIDX	R5	R7	K8
      0x20180B09,  //  0015  NE	R6	R5	K9
      0x781A0003,  //  0016  JMPF	R6	#001B
      0x8C18010A,  //  0017  GETMET	R6	R0	K10
      0x5C200A00,  //  0018  MOVE	R8	R5
      0x7C180400,  //  0019  CALL	R6	2
      0x5C100C00,  //  001A  MOVE	R4	R6
      0x8C18010B,  //  001B  GETMET	R6	R0	K11
      0x7C180200,  //  001C  CALL	R6	1
      0x741A005F,  //  001D  JMPT	R6	#007E
      0x8C18010C,  //  001E  GETMET	R6	R0	K12
      0x7C180200,  //  001F  CALL	R6	1
      0x741A005C,  //  0020  JMPT	R6	#007E
      0x8C18010D,  //  0021  GETMET	R6	R0	K13
      0x7C180200,  //  0022  CALL	R6	1
      0x8C18010C,  //  0023  GETMET	R6	R0	K12
      0x7C180200,  //  0024  CALL	R6	1
      0x781A0000,  //  0025  JMPF	R6	#0027
      0x70020056,  //  0026  JMP		#007E
      0x8C18010E,  //  0027  GETMET	R6	R0	K14
      0x7C180200,  //  0028  CALL	R6	1
      0x4C1C0000,  //  0029  LDNIL	R7
      0x201C0807,  //  002A  NE	R7	R4	R7
      0x781E0006,  //  002B  JMPF	R7	#0033
      0x201C0B09,  //  002C  NE	R7	R5	K9
      0x781E0004,  //  002D  JMPF	R7	#0033
      0x8C1C010F,  //  002E  GETMET	R7	R0	K15
      0x5C240A00,  //  002F  MOVE	R9	R5
      0x5C280C00,  //  0030  MOVE	R10	R6
      0x5C2C0800,  //  0031  MOVE	R11	R4
      0x7C1C0800,  //  0032  CALL	R7	4
      0x8C1C0110,  //  0033  GETMET	R7	R0	K16
      0x7C1C0200,  //  0034  CALL	R7	1
      0x8C1C0111,  //  0035  GETMET	R7	R0	K17
      0x58240012,  //  0036  LDCONST	R9	K18
      0x7C1C0400,  //  0037  CALL	R7	2
      0x8C200113,  //  0038  GETMET	R8	R0	K19
      0x7C200200,  //  0039  CALL	R8	1
      0x5C240600,  //  003A  MOVE	R9	R3
      0x5C280C00,  //  003B  MOVE	R10	R6
      0x5C2C0E00,  //  003C  MOVE	R11	R7
      0x5C301000,  //  003D  MOVE	R12	R8
      0x7C240600,  //  003E  CALL	R9	3
      0x8C24010B,  //  003F  GETMET	R9	R0	K11
      0x7C240200,  //  0040  CALL	R9	1
      0x7426000F,  //  0041  JMPT	R9	#0052
      0x8C240114,  //  0042  GETMET	R9	R0	K20
      0x7C240200,  //  0043  CALL	R9	1
      0x4C280000,  //  0044  LDNIL	R10
      0x2028120A,  //  0045  NE	R10	R9	R10
      0x782A0008,  //  0046  JMPF	R10	#0050
      0x88281315,  //  0047  GETMBR	R10	R9	K21
      0xB82E2C00,  //  0048  GETNGBL	R11	K22
      0x882C1717,  //  0049  GETMBR	R11	R11	K23
      0x882C1718,  //  004A  GETMBR	R11	R11	K24
      0x1C28140B,  //  004B  EQ	R10	R10	R11
      0x782A0002,  //  004C  JMPF	R10	#0050
      0x8C280119,  //  004D  GETMET	R10	R0	K25
      0x7C280200,  //  004E  CALL	R10	1
      0x70020000,  //  004F  JMP		#0051
      0x70020000,  //  0050  JMP		#0052
      0x7001FFEC,  //  0051  JMP		#003F
      0x8C240114,  //  0052  GETMET	R9	R0	K20
      0x7C240200,  //  0053  CALL	R9	1
      0x4C280000,  //  0054  LDNIL	R10
      0x2024120A,  //  0055  NE	R9	R9	R10
      0x7826000C,  //  0056  JMPF	R9	#0064
      0x8C240114,  //  0057  GETMET	R9	R0	K20
      0x7C240200,  //  0058  CALL	R9	1
      0x88241315,  //  0059  GETMBR	R9	R9	K21
      0xB82A2C00,  //  005A  GETNGBL	R10	K22
      0x88281517,  //  005B  GETMBR	R10	R10	K23
      0x8828151A,  //  005C  GETMBR	R10	R10	K26
      0x1C24120A,  //  005D  EQ	R9	R9	R10
      0x78260004,  //  005E  JMPF	R9	#0064
      0x8C240119,  //  005F  GETMET	R9	R0	K25
      0x7C240200,  //  0060  CALL	R9	1
      0x8C24010D,  //  0061  GETMET	R9	R0	K13
      0x7C240200,  //  0062  CALL	R9	1
      0x70020018,  //  0063  JMP		#007D
      0x8C240114,  //  0064  GETMET	R9	R0	K20
      0x7C240200,  //  0065  CALL	R9	1
      0x4C280000,  //  0066  LDNIL	R10
      0x2024120A,  //  0067  NE	R9	R9	R10
      0x7826000C,  //  0068  JMPF	R9	#0076
      0x8C240114,  //  0069  GETMET	R9	R0	K20
      0x7C240200,  //  006A  CALL	R9	1
      0x88241315,  //  006B  GETMBR	R9	R9	K21
      0xB82A2C00,  //  006C  GETNGBL	R10	K22
      0x88281517,  //  006D  GETMBR	R10	R10	K23
      0x8828151B,  //  006E  GETMBR	R10	R10	K27
      0x1C24120A,  //  006F  EQ	R9	R9	R10
      0x78260004,  //  0070  JMPF	R9	#0076
      0x8C240119,  //  0071  GETMET	R9	R0	K25
      0x7C240200,  //  0072  CALL	R9	1
      0x8C24010D,  //  0073  GETMET	R9	R0	K13
      0x7C240200,  //  0074  CALL	R9	1
      0x70020006,  //  0075  JMP		#007D
      0x8C24010C,  //  0076  GETMET	R9	R0	K12
      0x7C240200,  //  0077  CALL	R9	1
      0x74260003,  //  0078  JMPT	R9	#007D
      0x8C24011C,  //  0079  GETMET	R9	R0	K28
      0x582C001D,  //  007A  LDCONST	R11	K29
      0x7C240400,  //  007B  CALL	R9	2
      0x70020000,  //  007C  JMP		#007E
      0x7001FF9C,  //  007D  JMP		#001B
      0x80000000,  //  007E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_named_arguments_for_animation
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_named_arguments_for_animation,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_process_named_arguments_unified),
    /* K1   */  be_nested_str_weak(animation),
    }),
    be_str_weak(_process_named_arguments_for_animation),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x581C0001,  //  0003  LDCONST	R7	K1
      0x7C0C0800,  //  0004  CALL	R3	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_indent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_get_indent,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(_X20_X20),
    /* K1   */  be_nested_str_weak(indent_level),
    /* K2   */  be_const_int(1),
    }),
    be_str_weak(get_indent),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040101,  //  0000  GETMBR	R1	R0	K1
      0x00040302,  //  0001  ADD	R1	R1	K2
      0x08060001,  //  0002  MUL	R1	K0	R1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_object_reference
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_object_reference,   /* name */
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
    /* K0   */  be_nested_str_weak(validate_symbol_reference),
    }),
    be_str_weak(_validate_object_reference),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA8020005,  //  0000  EXBLK	0	#0007
      0x8C0C0100,  //  0001  GETMET	R3	R0	K0
      0x5C140200,  //  0002  MOVE	R5	R1
      0x5C180400,  //  0003  MOVE	R6	R2
      0x7C0C0600,  //  0004  CALL	R3	3
      0xA8040001,  //  0005  EXBLK	1	1
      0x70020003,  //  0006  JMP		#000B
      0xAC0C0002,  //  0007  CATCH	R3	0	2
      0x70020000,  //  0008  JMP		#000A
      0x70020000,  //  0009  JMP		#000B
      0xB0080000,  //  000A  RAISE	2	R0	R0
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: can_use_as_identifier
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_can_use_as_identifier,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(color),
    /* K1   */  be_nested_str_weak(animation),
    /* K2   */  be_nested_str_weak(palette),
    /* K3   */  be_nested_str_weak(startup),
    /* K4   */  be_nested_str_weak(shutdown),
    /* K5   */  be_nested_str_weak(button_press),
    /* K6   */  be_nested_str_weak(button_hold),
    /* K7   */  be_nested_str_weak(motion_detected),
    /* K8   */  be_nested_str_weak(brightness_change),
    /* K9   */  be_nested_str_weak(timer),
    /* K10  */  be_nested_str_weak(time),
    /* K11  */  be_nested_str_weak(sound_peak),
    /* K12  */  be_nested_str_weak(network_message),
    /* K13  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(can_use_as_identifier),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x400C0500,  //  0002  CONNECT	R3	R2	K0
      0x400C0501,  //  0003  CONNECT	R3	R2	K1
      0x400C0502,  //  0004  CONNECT	R3	R2	K2
      0x400C0503,  //  0005  CONNECT	R3	R2	K3
      0x400C0504,  //  0006  CONNECT	R3	R2	K4
      0x400C0505,  //  0007  CONNECT	R3	R2	K5
      0x400C0506,  //  0008  CONNECT	R3	R2	K6
      0x400C0507,  //  0009  CONNECT	R3	R2	K7
      0x400C0508,  //  000A  CONNECT	R3	R2	K8
      0x400C0509,  //  000B  CONNECT	R3	R2	K9
      0x400C050A,  //  000C  CONNECT	R3	R2	K10
      0x400C050B,  //  000D  CONNECT	R3	R2	K11
      0x400C050C,  //  000E  CONNECT	R3	R2	K12
      0x600C0010,  //  000F  GETGBL	R3	G16
      0x5C100400,  //  0010  MOVE	R4	R2
      0x7C0C0200,  //  0011  CALL	R3	1
      0xA8020007,  //  0012  EXBLK	0	#001B
      0x5C100600,  //  0013  MOVE	R4	R3
      0x7C100000,  //  0014  CALL	R4	0
      0x1C140204,  //  0015  EQ	R5	R1	R4
      0x78160002,  //  0016  JMPF	R5	#001A
      0x50140200,  //  0017  LDBOOL	R5	1	0
      0xA8040001,  //  0018  EXBLK	1	1
      0x80040A00,  //  0019  RET	1	R5
      0x7001FFF7,  //  001A  JMP		#0013
      0x580C000D,  //  001B  LDCONST	R3	K13
      0xAC0C0200,  //  001C  CATCH	R3	1	0
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x500C0000,  //  001E  LDBOOL	R3	0	0
      0x80040600,  //  001F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_named_arguments_unified
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_named_arguments_unified,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        11,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(add),
        /* K1   */  be_nested_str_weak(_X25s_X2E_X25s_X20_X3D_X20_X25s_X25s),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[11]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x60140018,  //  0002  GETGBL	R5	G24
          0x58180001,  //  0003  LDCONST	R6	K1
          0x681C0001,  //  0004  GETUPV	R7	U1
          0x5C200000,  //  0005  MOVE	R8	R0
          0x5C240200,  //  0006  MOVE	R9	R1
          0x5C280400,  //  0007  MOVE	R10	R2
          0x7C140A00,  //  0008  CALL	R5	5
          0x7C0C0400,  //  0009  CALL	R3	2
          0x80000000,  //  000A  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_paren),
    /* K1   */  be_nested_str_weak(_process_parameters_core),
    /* K2   */  be_nested_str_weak(expect_right_paren),
    }),
    be_str_weak(_process_named_arguments_unified),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x7C100200,  //  0001  CALL	R4	1
      0x84100000,  //  0002  CLOSURE	R4	P0
      0x8C140101,  //  0003  GETMET	R5	R0	K1
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x5C200600,  //  0005  MOVE	R8	R3
      0x5C240800,  //  0006  MOVE	R9	R4
      0x7C140800,  //  0007  CALL	R5	4
      0x8C140102,  //  0008  GETMET	R5	R0	K2
      0x7C140200,  //  0009  CALL	R5	1
      0xA0000000,  //  000A  CLOSE	R0
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_play_statement_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_play_statement_fluent,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(IDENTIFIER),
    /* K7   */  be_nested_str_weak(KEYWORD),
    /* K8   */  be_nested_str_weak(peek),
    /* K9   */  be_nested_str_weak(LEFT_PAREN),
    /* K10  */  be_nested_str_weak(process_nested_function_call),
    /* K11  */  be_nested_str_weak(expect_identifier),
    /* K12  */  be_nested_str_weak(_validate_object_reference),
    /* K13  */  be_nested_str_weak(sequence_X20play),
    /* K14  */  be_nested_str_weak(_X25s_),
    /* K15  */  be_nested_str_weak(nil),
    /* K16  */  be_nested_str_weak(value),
    /* K17  */  be_nested_str_weak(for),
    /* K18  */  be_nested_str_weak(process_time_value),
    /* K19  */  be_nested_str_weak(collect_inline_comment),
    /* K20  */  be_nested_str_weak(add),
    /* K21  */  be_nested_str_weak(_X25s_X2Epush_play_step_X28_X25s_X2C_X20_X25s_X29_X25s),
    /* K22  */  be_nested_str_weak(get_indent),
    }),
    be_str_weak(process_play_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[87]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58040001,  //  0002  LDCONST	R1	K1
      0x8C080102,  //  0003  GETMET	R2	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x200C0403,  //  0006  NE	R3	R2	R3
      0x780E001C,  //  0007  JMPF	R3	#0025
      0x880C0503,  //  0008  GETMBR	R3	R2	K3
      0xB8120800,  //  0009  GETNGBL	R4	K4
      0x88100905,  //  000A  GETMBR	R4	R4	K5
      0x88100906,  //  000B  GETMBR	R4	R4	K6
      0x1C0C0604,  //  000C  EQ	R3	R3	R4
      0x740E0005,  //  000D  JMPT	R3	#0014
      0x880C0503,  //  000E  GETMBR	R3	R2	K3
      0xB8120800,  //  000F  GETNGBL	R4	K4
      0x88100905,  //  0010  GETMBR	R4	R4	K5
      0x88100907,  //  0011  GETMBR	R4	R4	K7
      0x1C0C0604,  //  0012  EQ	R3	R3	R4
      0x780E0010,  //  0013  JMPF	R3	#0025
      0x8C0C0108,  //  0014  GETMET	R3	R0	K8
      0x7C0C0200,  //  0015  CALL	R3	1
      0x4C100000,  //  0016  LDNIL	R4
      0x200C0604,  //  0017  NE	R3	R3	R4
      0x780E000B,  //  0018  JMPF	R3	#0025
      0x8C0C0108,  //  0019  GETMET	R3	R0	K8
      0x7C0C0200,  //  001A  CALL	R3	1
      0x880C0703,  //  001B  GETMBR	R3	R3	K3
      0xB8120800,  //  001C  GETNGBL	R4	K4
      0x88100905,  //  001D  GETMBR	R4	R4	K5
      0x88100909,  //  001E  GETMBR	R4	R4	K9
      0x1C0C0604,  //  001F  EQ	R3	R3	R4
      0x780E0003,  //  0020  JMPF	R3	#0025
      0x8C0C010A,  //  0021  GETMET	R3	R0	K10
      0x7C0C0200,  //  0022  CALL	R3	1
      0x5C040600,  //  0023  MOVE	R1	R3
      0x7002000A,  //  0024  JMP		#0030
      0x8C0C010B,  //  0025  GETMET	R3	R0	K11
      0x7C0C0200,  //  0026  CALL	R3	1
      0x8C10010C,  //  0027  GETMET	R4	R0	K12
      0x5C180600,  //  0028  MOVE	R6	R3
      0x581C000D,  //  0029  LDCONST	R7	K13
      0x7C100600,  //  002A  CALL	R4	3
      0x60100018,  //  002B  GETGBL	R4	G24
      0x5814000E,  //  002C  LDCONST	R5	K14
      0x5C180600,  //  002D  MOVE	R6	R3
      0x7C100400,  //  002E  CALL	R4	2
      0x5C040800,  //  002F  MOVE	R1	R4
      0x580C000F,  //  0030  LDCONST	R3	K15
      0x8C100102,  //  0031  GETMET	R4	R0	K2
      0x7C100200,  //  0032  CALL	R4	1
      0x4C140000,  //  0033  LDNIL	R5
      0x20100805,  //  0034  NE	R4	R4	R5
      0x78120013,  //  0035  JMPF	R4	#004A
      0x8C100102,  //  0036  GETMET	R4	R0	K2
      0x7C100200,  //  0037  CALL	R4	1
      0x88100903,  //  0038  GETMBR	R4	R4	K3
      0xB8160800,  //  0039  GETNGBL	R5	K4
      0x88140B05,  //  003A  GETMBR	R5	R5	K5
      0x88140B07,  //  003B  GETMBR	R5	R5	K7
      0x1C100805,  //  003C  EQ	R4	R4	R5
      0x7812000B,  //  003D  JMPF	R4	#004A
      0x8C100102,  //  003E  GETMET	R4	R0	K2
      0x7C100200,  //  003F  CALL	R4	1
      0x88100910,  //  0040  GETMBR	R4	R4	K16
      0x1C100911,  //  0041  EQ	R4	R4	K17
      0x78120006,  //  0042  JMPF	R4	#004A
      0x8C100100,  //  0043  GETMET	R4	R0	K0
      0x7C100200,  //  0044  CALL	R4	1
      0x60100008,  //  0045  GETGBL	R4	G8
      0x8C140112,  //  0046  GETMET	R5	R0	K18
      0x7C140200,  //  0047  CALL	R5	1
      0x7C100200,  //  0048  CALL	R4	1
      0x5C0C0800,  //  0049  MOVE	R3	R4
      0x8C100113,  //  004A  GETMET	R4	R0	K19
      0x7C100200,  //  004B  CALL	R4	1
      0x8C140114,  //  004C  GETMET	R5	R0	K20
      0x601C0018,  //  004D  GETGBL	R7	G24
      0x58200015,  //  004E  LDCONST	R8	K21
      0x8C240116,  //  004F  GETMET	R9	R0	K22
      0x7C240200,  //  0050  CALL	R9	1
      0x5C280200,  //  0051  MOVE	R10	R1
      0x5C2C0600,  //  0052  MOVE	R11	R3
      0x5C300800,  //  0053  MOVE	R12	R4
      0x7C1C0A00,  //  0054  CALL	R7	5
      0x7C140400,  //  0055  CALL	R5	2
      0x80000000,  //  0056  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_palette
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_palette,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[47]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(validate_user_name),
    /* K3   */  be_nested_str_weak(palette),
    /* K4   */  be_nested_str_weak(skip_statement),
    /* K5   */  be_nested_str_weak(expect_assign),
    /* K6   */  be_nested_str_weak(expect_left_bracket),
    /* K7   */  be_nested_str_weak(skip_whitespace_including_newlines),
    /* K8   */  be_nested_str_weak(check_right_bracket),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Empty_X20palettes_X20are_X20not_X20allowed_X2E_X20A_X20palette_X20must_X20contain_X20at_X20least_X20one_X20color_X20entry_X2E),
    /* K11  */  be_nested_str_weak(current),
    /* K12  */  be_nested_str_weak(type),
    /* K13  */  be_nested_str_weak(animation_dsl),
    /* K14  */  be_nested_str_weak(Token),
    /* K15  */  be_nested_str_weak(LEFT_PAREN),
    /* K16  */  be_nested_str_weak(at_end),
    /* K17  */  be_nested_str_weak(Cannot_X20mix_X20alternative_X20syntax_X20_X5Bcolor1_X2C_X20color2_X2C_X20_X2E_X2E_X2E_X5D_X20with_X20tuple_X20syntax_X20_X28value_X2C_X20color_X29_X2E_X20Use_X20only_X20one_X20syntax_X20per_X20palette_X2E),
    /* K18  */  be_nested_str_weak(expect_left_paren),
    /* K19  */  be_nested_str_weak(expect_number),
    /* K20  */  be_nested_str_weak(expect_comma),
    /* K21  */  be_nested_str_weak(process_palette_color),
    /* K22  */  be_nested_str_weak(expect_right_paren),
    /* K23  */  be_nested_str_weak(convert_to_vrgb),
    /* K24  */  be_nested_str_weak(0x_X25s),
    /* K25  */  be_nested_str_weak(push),
    /* K26  */  be_nested_str_weak(Cannot_X20mix_X20tuple_X20syntax_X20_X28value_X2C_X20color_X29_X20with_X20alternative_X20syntax_X20_X5Bcolor1_X2C_X20color2_X2C_X20_X2E_X2E_X2E_X5D_X2E_X20Use_X20only_X20one_X20syntax_X20per_X20palette_X2E),
    /* K27  */  be_nested_str_weak(),
    /* K28  */  be_nested_str_weak(COMMA),
    /* K29  */  be_nested_str_weak(COMMENT),
    /* K30  */  be_nested_str_weak(value),
    /* K31  */  be_nested_str_weak(NEWLINE),
    /* K32  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X5D_X27_X20in_X20palette_X20definition),
    /* K33  */  be_nested_str_weak(expect_right_bracket),
    /* K34  */  be_nested_str_weak(collect_inline_comment),
    /* K35  */  be_nested_str_weak(stop_iteration),
    /* K36  */  be_nested_str_weak(add),
    /* K37  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20bytes_X28_X25s),
    /* K38  */  be_const_int(0),
    /* K39  */  be_const_int(1),
    /* K40  */  be_nested_str_weak(_X2508X),
    /* K41  */  be_nested_str_weak(_X20_X20_X25s),
    /* K42  */  be_nested_str_weak(_X20_X20_X22_X25s_X22_X25s),
    /* K43  */  be_nested_str_weak(_X29),
    /* K44  */  be_nested_str_weak(_X20),
    /* K45  */  be_nested_str_weak(_X22_X25s_X22),
    /* K46  */  be_nested_str_weak(var_X20_X25s__X20_X3D_X20bytes_X28_X25s_X29_X25s),
    }),
    be_str_weak(process_palette),
    &be_const_str_solidified,
    ( &(const binstruction[344]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x740A0002,  //  0008  JMPT	R2	#000C
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x80000400,  //  000B  RET	0
      0x8C080105,  //  000C  GETMET	R2	R0	K5
      0x7C080200,  //  000D  CALL	R2	1
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x7C080200,  //  000F  CALL	R2	1
      0x60080012,  //  0010  GETGBL	R2	G18
      0x7C080000,  //  0011  CALL	R2	0
      0x600C0012,  //  0012  GETGBL	R3	G18
      0x7C0C0000,  //  0013  CALL	R3	0
      0x8C100107,  //  0014  GETMET	R4	R0	K7
      0x7C100200,  //  0015  CALL	R4	1
      0x8C100108,  //  0016  GETMET	R4	R0	K8
      0x7C100200,  //  0017  CALL	R4	1
      0x78120005,  //  0018  JMPF	R4	#001F
      0x8C100109,  //  0019  GETMET	R4	R0	K9
      0x5818000A,  //  001A  LDCONST	R6	K10
      0x7C100400,  //  001B  CALL	R4	2
      0x8C100104,  //  001C  GETMET	R4	R0	K4
      0x7C100200,  //  001D  CALL	R4	1
      0x80000800,  //  001E  RET	0
      0x8C10010B,  //  001F  GETMET	R4	R0	K11
      0x7C100200,  //  0020  CALL	R4	1
      0x4C140000,  //  0021  LDNIL	R5
      0x20100805,  //  0022  NE	R4	R4	R5
      0x78120007,  //  0023  JMPF	R4	#002C
      0x8C10010B,  //  0024  GETMET	R4	R0	K11
      0x7C100200,  //  0025  CALL	R4	1
      0x8810090C,  //  0026  GETMBR	R4	R4	K12
      0xB8161A00,  //  0027  GETNGBL	R5	K13
      0x88140B0E,  //  0028  GETMBR	R5	R5	K14
      0x88140B0F,  //  0029  GETMBR	R5	R5	K15
      0x1C100805,  //  002A  EQ	R4	R4	R5
      0x74120000,  //  002B  JMPT	R4	#002D
      0x50100001,  //  002C  LDBOOL	R4	0	1
      0x50100200,  //  002D  LDBOOL	R4	1	0
      0x8C140110,  //  002E  GETMET	R5	R0	K16
      0x7C140200,  //  002F  CALL	R5	1
      0x741600BF,  //  0030  JMPT	R5	#00F1
      0x8C140108,  //  0031  GETMET	R5	R0	K8
      0x7C140200,  //  0032  CALL	R5	1
      0x741600BC,  //  0033  JMPT	R5	#00F1
      0x8C140107,  //  0034  GETMET	R5	R0	K7
      0x7C140200,  //  0035  CALL	R5	1
      0x8C140108,  //  0036  GETMET	R5	R0	K8
      0x7C140200,  //  0037  CALL	R5	1
      0x78160000,  //  0038  JMPF	R5	#003A
      0x700200B6,  //  0039  JMP		#00F1
      0x7812002A,  //  003A  JMPF	R4	#0066
      0x8C14010B,  //  003B  GETMET	R5	R0	K11
      0x7C140200,  //  003C  CALL	R5	1
      0x4C180000,  //  003D  LDNIL	R6
      0x20140A06,  //  003E  NE	R5	R5	R6
      0x7816000D,  //  003F  JMPF	R5	#004E
      0x8C14010B,  //  0040  GETMET	R5	R0	K11
      0x7C140200,  //  0041  CALL	R5	1
      0x88140B0C,  //  0042  GETMBR	R5	R5	K12
      0xB81A1A00,  //  0043  GETNGBL	R6	K13
      0x88180D0E,  //  0044  GETMBR	R6	R6	K14
      0x88180D0F,  //  0045  GETMBR	R6	R6	K15
      0x20140A06,  //  0046  NE	R5	R5	R6
      0x78160005,  //  0047  JMPF	R5	#004E
      0x8C140109,  //  0048  GETMET	R5	R0	K9
      0x581C0011,  //  0049  LDCONST	R7	K17
      0x7C140400,  //  004A  CALL	R5	2
      0x8C140104,  //  004B  GETMET	R5	R0	K4
      0x7C140200,  //  004C  CALL	R5	1
      0x80000A00,  //  004D  RET	0
      0x8C140112,  //  004E  GETMET	R5	R0	K18
      0x7C140200,  //  004F  CALL	R5	1
      0x8C140113,  //  0050  GETMET	R5	R0	K19
      0x7C140200,  //  0051  CALL	R5	1
      0x8C180114,  //  0052  GETMET	R6	R0	K20
      0x7C180200,  //  0053  CALL	R6	1
      0x8C180115,  //  0054  GETMET	R6	R0	K21
      0x7C180200,  //  0055  CALL	R6	1
      0x8C1C0116,  //  0056  GETMET	R7	R0	K22
      0x7C1C0200,  //  0057  CALL	R7	1
      0x8C1C0117,  //  0058  GETMET	R7	R0	K23
      0x5C240A00,  //  0059  MOVE	R9	R5
      0x5C280C00,  //  005A  MOVE	R10	R6
      0x7C1C0600,  //  005B  CALL	R7	3
      0x60200009,  //  005C  GETGBL	R8	G9
      0x60240018,  //  005D  GETGBL	R9	G24
      0x58280018,  //  005E  LDCONST	R10	K24
      0x5C2C0E00,  //  005F  MOVE	R11	R7
      0x7C240400,  //  0060  CALL	R9	2
      0x7C200200,  //  0061  CALL	R8	1
      0x8C240519,  //  0062  GETMET	R9	R2	K25
      0x5C2C1000,  //  0063  MOVE	R11	R8
      0x7C240400,  //  0064  CALL	R9	2
      0x70020021,  //  0065  JMP		#0088
      0x8C14010B,  //  0066  GETMET	R5	R0	K11
      0x7C140200,  //  0067  CALL	R5	1
      0x4C180000,  //  0068  LDNIL	R6
      0x20140A06,  //  0069  NE	R5	R5	R6
      0x7816000D,  //  006A  JMPF	R5	#0079
      0x8C14010B,  //  006B  GETMET	R5	R0	K11
      0x7C140200,  //  006C  CALL	R5	1
      0x88140B0C,  //  006D  GETMBR	R5	R5	K12
      0xB81A1A00,  //  006E  GETNGBL	R6	K13
      0x88180D0E,  //  006F  GETMBR	R6	R6	K14
      0x88180D0F,  //  0070  GETMBR	R6	R6	K15
      0x1C140A06,  //  0071  EQ	R5	R5	R6
      0x78160005,  //  0072  JMPF	R5	#0079
      0x8C140109,  //  0073  GETMET	R5	R0	K9
      0x581C001A,  //  0074  LDCONST	R7	K26
      0x7C140400,  //  0075  CALL	R5	2
      0x8C140104,  //  0076  GETMET	R5	R0	K4
      0x7C140200,  //  0077  CALL	R5	1
      0x80000A00,  //  0078  RET	0
      0x8C140115,  //  0079  GETMET	R5	R0	K21
      0x7C140200,  //  007A  CALL	R5	1
      0x8C180117,  //  007B  GETMET	R6	R0	K23
      0x542200FE,  //  007C  LDINT	R8	255
      0x5C240A00,  //  007D  MOVE	R9	R5
      0x7C180600,  //  007E  CALL	R6	3
      0x601C0009,  //  007F  GETGBL	R7	G9
      0x60200018,  //  0080  GETGBL	R8	G24
      0x58240018,  //  0081  LDCONST	R9	K24
      0x5C280C00,  //  0082  MOVE	R10	R6
      0x7C200400,  //  0083  CALL	R8	2
      0x7C1C0200,  //  0084  CALL	R7	1
      0x8C200519,  //  0085  GETMET	R8	R2	K25
      0x5C280E00,  //  0086  MOVE	R10	R7
      0x7C200400,  //  0087  CALL	R8	2
      0x5814001B,  //  0088  LDCONST	R5	K27
      0x8C18010B,  //  0089  GETMET	R6	R0	K11
      0x7C180200,  //  008A  CALL	R6	1
      0x4C1C0000,  //  008B  LDNIL	R7
      0x20180C07,  //  008C  NE	R6	R6	R7
      0x781A002F,  //  008D  JMPF	R6	#00BE
      0x8C18010B,  //  008E  GETMET	R6	R0	K11
      0x7C180200,  //  008F  CALL	R6	1
      0x88180D0C,  //  0090  GETMBR	R6	R6	K12
      0xB81E1A00,  //  0091  GETNGBL	R7	K13
      0x881C0F0E,  //  0092  GETMBR	R7	R7	K14
      0x881C0F1C,  //  0093  GETMBR	R7	R7	K28
      0x1C180C07,  //  0094  EQ	R6	R6	R7
      0x781A0027,  //  0095  JMPF	R6	#00BE
      0x8C180100,  //  0096  GETMET	R6	R0	K0
      0x7C180200,  //  0097  CALL	R6	1
      0x8C18010B,  //  0098  GETMET	R6	R0	K11
      0x7C180200,  //  0099  CALL	R6	1
      0x4C1C0000,  //  009A  LDNIL	R7
      0x20180C07,  //  009B  NE	R6	R6	R7
      0x781A000C,  //  009C  JMPF	R6	#00AA
      0x8C18010B,  //  009D  GETMET	R6	R0	K11
      0x7C180200,  //  009E  CALL	R6	1
      0x88180D0C,  //  009F  GETMBR	R6	R6	K12
      0xB81E1A00,  //  00A0  GETNGBL	R7	K13
      0x881C0F0E,  //  00A1  GETMBR	R7	R7	K14
      0x881C0F1D,  //  00A2  GETMBR	R7	R7	K29
      0x1C180C07,  //  00A3  EQ	R6	R6	R7
      0x781A0004,  //  00A4  JMPF	R6	#00AA
      0x8C18010B,  //  00A5  GETMET	R6	R0	K11
      0x7C180200,  //  00A6  CALL	R6	1
      0x88140D1E,  //  00A7  GETMBR	R5	R6	K30
      0x8C180100,  //  00A8  GETMET	R6	R0	K0
      0x7C180200,  //  00A9  CALL	R6	1
      0x8C180110,  //  00AA  GETMET	R6	R0	K16
      0x7C180200,  //  00AB  CALL	R6	1
      0x741A000F,  //  00AC  JMPT	R6	#00BD
      0x8C18010B,  //  00AD  GETMET	R6	R0	K11
      0x7C180200,  //  00AE  CALL	R6	1
      0x4C1C0000,  //  00AF  LDNIL	R7
      0x201C0C07,  //  00B0  NE	R7	R6	R7
      0x781E0008,  //  00B1  JMPF	R7	#00BB
      0x881C0D0C,  //  00B2  GETMBR	R7	R6	K12
      0xB8221A00,  //  00B3  GETNGBL	R8	K13
      0x8820110E,  //  00B4  GETMBR	R8	R8	K14
      0x8820111F,  //  00B5  GETMBR	R8	R8	K31
      0x1C1C0E08,  //  00B6  EQ	R7	R7	R8
      0x781E0002,  //  00B7  JMPF	R7	#00BB
      0x8C1C0100,  //  00B8  GETMET	R7	R0	K0
      0x7C1C0200,  //  00B9  CALL	R7	1
      0x70020000,  //  00BA  JMP		#00BC
      0x70020000,  //  00BB  JMP		#00BD
      0x7001FFEC,  //  00BC  JMP		#00AA
      0x7002002E,  //  00BD  JMP		#00ED
      0x8C18010B,  //  00BE  GETMET	R6	R0	K11
      0x7C180200,  //  00BF  CALL	R6	1
      0x4C1C0000,  //  00C0  LDNIL	R7
      0x20180C07,  //  00C1  NE	R6	R6	R7
      0x781A000C,  //  00C2  JMPF	R6	#00D0
      0x8C18010B,  //  00C3  GETMET	R6	R0	K11
      0x7C180200,  //  00C4  CALL	R6	1
      0x88180D0C,  //  00C5  GETMBR	R6	R6	K12
      0xB81E1A00,  //  00C6  GETNGBL	R7	K13
      0x881C0F0E,  //  00C7  GETMBR	R7	R7	K14
      0x881C0F1F,  //  00C8  GETMBR	R7	R7	K31
      0x1C180C07,  //  00C9  EQ	R6	R6	R7
      0x781A0004,  //  00CA  JMPF	R6	#00D0
      0x8C180100,  //  00CB  GETMET	R6	R0	K0
      0x7C180200,  //  00CC  CALL	R6	1
      0x8C180107,  //  00CD  GETMET	R6	R0	K7
      0x7C180200,  //  00CE  CALL	R6	1
      0x7002001C,  //  00CF  JMP		#00ED
      0x8C180108,  //  00D0  GETMET	R6	R0	K8
      0x7C180200,  //  00D1  CALL	R6	1
      0x741A0019,  //  00D2  JMPT	R6	#00ED
      0x8C18010B,  //  00D3  GETMET	R6	R0	K11
      0x7C180200,  //  00D4  CALL	R6	1
      0x4C1C0000,  //  00D5  LDNIL	R7
      0x20180C07,  //  00D6  NE	R6	R6	R7
      0x781A000D,  //  00D7  JMPF	R6	#00E6
      0x8C18010B,  //  00D8  GETMET	R6	R0	K11
      0x7C180200,  //  00D9  CALL	R6	1
      0x88180D0C,  //  00DA  GETMBR	R6	R6	K12
      0xB81E1A00,  //  00DB  GETNGBL	R7	K13
      0x881C0F0E,  //  00DC  GETMBR	R7	R7	K14
      0x881C0F1D,  //  00DD  GETMBR	R7	R7	K29
      0x1C180C07,  //  00DE  EQ	R6	R6	R7
      0x781A0005,  //  00DF  JMPF	R6	#00E6
      0x8C18010B,  //  00E0  GETMET	R6	R0	K11
      0x7C180200,  //  00E1  CALL	R6	1
      0x88140D1E,  //  00E2  GETMBR	R5	R6	K30
      0x8C180100,  //  00E3  GETMET	R6	R0	K0
      0x7C180200,  //  00E4  CALL	R6	1
      0x70020006,  //  00E5  JMP		#00ED
      0x8C180108,  //  00E6  GETMET	R6	R0	K8
      0x7C180200,  //  00E7  CALL	R6	1
      0x741A0003,  //  00E8  JMPT	R6	#00ED
      0x8C180109,  //  00E9  GETMET	R6	R0	K9
      0x58200020,  //  00EA  LDCONST	R8	K32
      0x7C180400,  //  00EB  CALL	R6	2
      0x70020003,  //  00EC  JMP		#00F1
      0x8C180719,  //  00ED  GETMET	R6	R3	K25
      0x5C200A00,  //  00EE  MOVE	R8	R5
      0x7C180400,  //  00EF  CALL	R6	2
      0x7001FF3C,  //  00F0  JMP		#002E
      0x8C140121,  //  00F1  GETMET	R5	R0	K33
      0x7C140200,  //  00F2  CALL	R5	1
      0x8C140122,  //  00F3  GETMET	R5	R0	K34
      0x7C140200,  //  00F4  CALL	R5	1
      0x50180000,  //  00F5  LDBOOL	R6	0	0
      0x601C0010,  //  00F6  GETGBL	R7	G16
      0x5C200600,  //  00F7  MOVE	R8	R3
      0x7C1C0200,  //  00F8  CALL	R7	1
      0xA8020008,  //  00F9  EXBLK	0	#0103
      0x5C200E00,  //  00FA  MOVE	R8	R7
      0x7C200000,  //  00FB  CALL	R8	0
      0x2024111B,  //  00FC  NE	R9	R8	K27
      0x78260001,  //  00FD  JMPF	R9	#0100
      0x50180200,  //  00FE  LDBOOL	R6	1	0
      0x70020000,  //  00FF  JMP		#0101
      0x7001FFF8,  //  0100  JMP		#00FA
      0xA8040001,  //  0101  EXBLK	1	1
      0x70020002,  //  0102  JMP		#0106
      0x581C0023,  //  0103  LDCONST	R7	K35
      0xAC1C0200,  //  0104  CATCH	R7	1	0
      0xB0080000,  //  0105  RAISE	2	R0	R0
      0x781A002C,  //  0106  JMPF	R6	#0134
      0x8C1C0124,  //  0107  GETMET	R7	R0	K36
      0x60240018,  //  0108  GETGBL	R9	G24
      0x58280025,  //  0109  LDCONST	R10	K37
      0x5C2C0200,  //  010A  MOVE	R11	R1
      0x5C300A00,  //  010B  MOVE	R12	R5
      0x7C240600,  //  010C  CALL	R9	3
      0x7C1C0400,  //  010D  CALL	R7	2
      0x601C0010,  //  010E  GETGBL	R7	G16
      0x6020000C,  //  010F  GETGBL	R8	G12
      0x5C240400,  //  0110  MOVE	R9	R2
      0x7C200200,  //  0111  CALL	R8	1
      0x04201127,  //  0112  SUB	R8	R8	K39
      0x40224C08,  //  0113  CONNECT	R8	K38	R8
      0x7C1C0200,  //  0114  CALL	R7	1
      0xA8020016,  //  0115  EXBLK	0	#012D
      0x5C200E00,  //  0116  MOVE	R8	R7
      0x7C200000,  //  0117  CALL	R8	0
      0x60240018,  //  0118  GETGBL	R9	G24
      0x58280028,  //  0119  LDCONST	R10	K40
      0x942C0408,  //  011A  GETIDX	R11	R2	R8
      0x7C240400,  //  011B  CALL	R9	2
      0x94280608,  //  011C  GETIDX	R10	R3	R8
      0x202C151B,  //  011D  NE	R11	R10	K27
      0x782E0004,  //  011E  JMPF	R11	#0124
      0x602C0018,  //  011F  GETGBL	R11	G24
      0x58300029,  //  0120  LDCONST	R12	K41
      0x5C341400,  //  0121  MOVE	R13	R10
      0x7C2C0400,  //  0122  CALL	R11	2
      0x70020000,  //  0123  JMP		#0125
      0x582C001B,  //  0124  LDCONST	R11	K27
      0x8C300124,  //  0125  GETMET	R12	R0	K36
      0x60380018,  //  0126  GETGBL	R14	G24
      0x583C002A,  //  0127  LDCONST	R15	K42
      0x5C401200,  //  0128  MOVE	R16	R9
      0x5C441600,  //  0129  MOVE	R17	R11
      0x7C380600,  //  012A  CALL	R14	3
      0x7C300400,  //  012B  CALL	R12	2
      0x7001FFE8,  //  012C  JMP		#0116
      0x581C0023,  //  012D  LDCONST	R7	K35
      0xAC1C0200,  //  012E  CATCH	R7	1	0
      0xB0080000,  //  012F  RAISE	2	R0	R0
      0x8C1C0124,  //  0130  GETMET	R7	R0	K36
      0x5824002B,  //  0131  LDCONST	R9	K43
      0x7C1C0400,  //  0132  CALL	R7	2
      0x70020022,  //  0133  JMP		#0157
      0x581C001B,  //  0134  LDCONST	R7	K27
      0x60200010,  //  0135  GETGBL	R8	G16
      0x6024000C,  //  0136  GETGBL	R9	G12
      0x5C280400,  //  0137  MOVE	R10	R2
      0x7C240200,  //  0138  CALL	R9	1
      0x04241327,  //  0139  SUB	R9	R9	K39
      0x40264C09,  //  013A  CONNECT	R9	K38	R9
      0x7C200200,  //  013B  CALL	R8	1
      0xA802000E,  //  013C  EXBLK	0	#014C
      0x5C241000,  //  013D  MOVE	R9	R8
      0x7C240000,  //  013E  CALL	R9	0
      0x24281326,  //  013F  GT	R10	R9	K38
      0x782A0000,  //  0140  JMPF	R10	#0142
      0x001C0F2C,  //  0141  ADD	R7	R7	K44
      0x60280018,  //  0142  GETGBL	R10	G24
      0x582C0028,  //  0143  LDCONST	R11	K40
      0x94300409,  //  0144  GETIDX	R12	R2	R9
      0x7C280400,  //  0145  CALL	R10	2
      0x602C0018,  //  0146  GETGBL	R11	G24
      0x5830002D,  //  0147  LDCONST	R12	K45
      0x5C341400,  //  0148  MOVE	R13	R10
      0x7C2C0400,  //  0149  CALL	R11	2
      0x001C0E0B,  //  014A  ADD	R7	R7	R11
      0x7001FFF0,  //  014B  JMP		#013D
      0x58200023,  //  014C  LDCONST	R8	K35
      0xAC200200,  //  014D  CATCH	R8	1	0
      0xB0080000,  //  014E  RAISE	2	R0	R0
      0x8C200124,  //  014F  GETMET	R8	R0	K36
      0x60280018,  //  0150  GETGBL	R10	G24
      0x582C002E,  //  0151  LDCONST	R11	K46
      0x5C300200,  //  0152  MOVE	R12	R1
      0x5C340E00,  //  0153  MOVE	R13	R7
      0x5C380A00,  //  0154  MOVE	R14	R5
      0x7C280800,  //  0155  CALL	R10	4
      0x7C200400,  //  0156  CALL	R8	2
      0x80000000,  //  0157  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_value_provider_factory_exists
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_value_provider_factory_exists,   /* name */
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
    /* K0   */  be_nested_str_weak(_validate_factory_function),
    /* K1   */  be_nested_str_weak(animation),
    /* K2   */  be_nested_str_weak(value_provider),
    }),
    be_str_weak(_validate_value_provider_factory_exists),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0xB8160200,  //  0002  GETNGBL	R5	K1
      0x88140B02,  //  0003  GETMBR	R5	R5	K2
      0x7C080600,  //  0004  CALL	R2	3
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_log_statement_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_log_statement_fluent,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_left_paren),
    /* K2   */  be_nested_str_weak(current),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(animation_dsl),
    /* K5   */  be_nested_str_weak(Token),
    /* K6   */  be_nested_str_weak(STRING),
    /* K7   */  be_nested_str_weak(error),
    /* K8   */  be_nested_str_weak(log_X28_X29_X20function_X20requires_X20a_X20string_X20message),
    /* K9   */  be_nested_str_weak(skip_statement),
    /* K10  */  be_nested_str_weak(value),
    /* K11  */  be_nested_str_weak(expect_right_paren),
    /* K12  */  be_nested_str_weak(collect_inline_comment),
    /* K13  */  be_nested_str_weak(process_log_call),
    /* K14  */  be_nested_str_weak(fluent),
    /* K15  */  be_nested_str_weak(add),
    }),
    be_str_weak(process_log_statement_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C040102,  //  0004  GETMET	R1	R0	K2
      0x7C040200,  //  0005  CALL	R1	1
      0x4C080000,  //  0006  LDNIL	R2
      0x1C080202,  //  0007  EQ	R2	R1	R2
      0x740A0005,  //  0008  JMPT	R2	#000F
      0x88080303,  //  0009  GETMBR	R2	R1	K3
      0xB80E0800,  //  000A  GETNGBL	R3	K4
      0x880C0705,  //  000B  GETMBR	R3	R3	K5
      0x880C0706,  //  000C  GETMBR	R3	R3	K6
      0x20080403,  //  000D  NE	R2	R2	R3
      0x780A0005,  //  000E  JMPF	R2	#0015
      0x8C080107,  //  000F  GETMET	R2	R0	K7
      0x58100008,  //  0010  LDCONST	R4	K8
      0x7C080400,  //  0011  CALL	R2	2
      0x8C080109,  //  0012  GETMET	R2	R0	K9
      0x7C080200,  //  0013  CALL	R2	1
      0x80000400,  //  0014  RET	0
      0x8808030A,  //  0015  GETMBR	R2	R1	K10
      0x8C0C0100,  //  0016  GETMET	R3	R0	K0
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C010B,  //  0018  GETMET	R3	R0	K11
      0x7C0C0200,  //  0019  CALL	R3	1
      0x8C0C010C,  //  001A  GETMET	R3	R0	K12
      0x7C0C0200,  //  001B  CALL	R3	1
      0x8C10010D,  //  001C  GETMET	R4	R0	K13
      0x5C180400,  //  001D  MOVE	R6	R2
      0x581C000E,  //  001E  LDCONST	R7	K14
      0x5C200600,  //  001F  MOVE	R8	R3
      0x7C100800,  //  0020  CALL	R4	4
      0x8C14010F,  //  0021  GETMET	R5	R0	K15
      0x5C1C0800,  //  0022  MOVE	R7	R4
      0x7C140400,  //  0023  CALL	R5	2
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_value
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_value,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(process_additive_expression),
    }),
    be_str_weak(process_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x50140200,  //  0002  LDBOOL	R5	1	0
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x7C080800,  //  0004  CALL	R2	4
      0x80040400,  //  0005  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_assign
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_assign,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(ASSIGN),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X3D_X27),
    }),
    be_str_weak(expect_assign),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: convert_color
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_convert_color,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(startswith),
    /* K2   */  be_nested_str_weak(0x),
    /* K3   */  be_nested_str_weak(0xFF_X25s),
    /* K4   */  be_const_int(2),
    /* K5   */  be_const_int(2147483647),
    /* K6   */  be_nested_str_weak(_X23),
    /* K7   */  be_nested_str_weak(0x_X25s),
    /* K8   */  be_const_int(1),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(0x_X25s_X25s_X25s_X25s_X25s_X25s_X25s_X25s),
    /* K11  */  be_nested_str_weak(0xFF_X25s_X25s_X25s_X25s_X25s_X25s),
    /* K12  */  be_nested_str_weak(animation_dsl),
    /* K13  */  be_nested_str_weak(is_color_name),
    /* K14  */  be_nested_str_weak(get_named_color_value),
    /* K15  */  be_nested_str_weak(0xFFFFFFFF),
    }),
    be_str_weak(convert_color),
    &be_const_str_solidified,
    ( &(const binstruction[110]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0013,  //  0005  JMPF	R3	#001A
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x5C100200,  //  0007  MOVE	R4	R1
      0x7C0C0200,  //  0008  CALL	R3	1
      0x54120009,  //  0009  LDINT	R4	10
      0x1C0C0604,  //  000A  EQ	R3	R3	R4
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x80040200,  //  000C  RET	1	R1
      0x7002000B,  //  000D  JMP		#001A
      0x600C000C,  //  000E  GETGBL	R3	G12
      0x5C100200,  //  000F  MOVE	R4	R1
      0x7C0C0200,  //  0010  CALL	R3	1
      0x54120007,  //  0011  LDINT	R4	8
      0x1C0C0604,  //  0012  EQ	R3	R3	R4
      0x780E0005,  //  0013  JMPF	R3	#001A
      0x600C0018,  //  0014  GETGBL	R3	G24
      0x58100003,  //  0015  LDCONST	R4	K3
      0x40160905,  //  0016  CONNECT	R5	K4	K5
      0x94140205,  //  0017  GETIDX	R5	R1	R5
      0x7C0C0400,  //  0018  CALL	R3	2
      0x80040600,  //  0019  RET	1	R3
      0x8C0C0501,  //  001A  GETMET	R3	R2	K1
      0x5C140200,  //  001B  MOVE	R5	R1
      0x58180006,  //  001C  LDCONST	R6	K6
      0x7C0C0600,  //  001D  CALL	R3	3
      0x780E0044,  //  001E  JMPF	R3	#0064
      0x600C000C,  //  001F  GETGBL	R3	G12
      0x5C100200,  //  0020  MOVE	R4	R1
      0x7C0C0200,  //  0021  CALL	R3	1
      0x54120008,  //  0022  LDINT	R4	9
      0x1C0C0604,  //  0023  EQ	R3	R3	R4
      0x780E0006,  //  0024  JMPF	R3	#002C
      0x600C0018,  //  0025  GETGBL	R3	G24
      0x58100007,  //  0026  LDCONST	R4	K7
      0x40161105,  //  0027  CONNECT	R5	K8	K5
      0x94140205,  //  0028  GETIDX	R5	R1	R5
      0x7C0C0400,  //  0029  CALL	R3	2
      0x80040600,  //  002A  RET	1	R3
      0x70020037,  //  002B  JMP		#0064
      0x600C000C,  //  002C  GETGBL	R3	G12
      0x5C100200,  //  002D  MOVE	R4	R1
      0x7C0C0200,  //  002E  CALL	R3	1
      0x54120006,  //  002F  LDINT	R4	7
      0x1C0C0604,  //  0030  EQ	R3	R3	R4
      0x780E0006,  //  0031  JMPF	R3	#0039
      0x600C0018,  //  0032  GETGBL	R3	G24
      0x58100003,  //  0033  LDCONST	R4	K3
      0x40161105,  //  0034  CONNECT	R5	K8	K5
      0x94140205,  //  0035  GETIDX	R5	R1	R5
      0x7C0C0400,  //  0036  CALL	R3	2
      0x80040600,  //  0037  RET	1	R3
      0x7002002A,  //  0038  JMP		#0064
      0x600C000C,  //  0039  GETGBL	R3	G12
      0x5C100200,  //  003A  MOVE	R4	R1
      0x7C0C0200,  //  003B  CALL	R3	1
      0x54120004,  //  003C  LDINT	R4	5
      0x1C0C0604,  //  003D  EQ	R3	R3	R4
      0x780E0011,  //  003E  JMPF	R3	#0051
      0x940C0308,  //  003F  GETIDX	R3	R1	K8
      0x94100304,  //  0040  GETIDX	R4	R1	K4
      0x94140309,  //  0041  GETIDX	R5	R1	K9
      0x541A0003,  //  0042  LDINT	R6	4
      0x94180206,  //  0043  GETIDX	R6	R1	R6
      0x601C0018,  //  0044  GETGBL	R7	G24
      0x5820000A,  //  0045  LDCONST	R8	K10
      0x5C240600,  //  0046  MOVE	R9	R3
      0x5C280600,  //  0047  MOVE	R10	R3
      0x5C2C0800,  //  0048  MOVE	R11	R4
      0x5C300800,  //  0049  MOVE	R12	R4
      0x5C340A00,  //  004A  MOVE	R13	R5
      0x5C380A00,  //  004B  MOVE	R14	R5
      0x5C3C0C00,  //  004C  MOVE	R15	R6
      0x5C400C00,  //  004D  MOVE	R16	R6
      0x7C1C1200,  //  004E  CALL	R7	9
      0x80040E00,  //  004F  RET	1	R7
      0x70020012,  //  0050  JMP		#0064
      0x600C000C,  //  0051  GETGBL	R3	G12
      0x5C100200,  //  0052  MOVE	R4	R1
      0x7C0C0200,  //  0053  CALL	R3	1
      0x54120003,  //  0054  LDINT	R4	4
      0x1C0C0604,  //  0055  EQ	R3	R3	R4
      0x780E000C,  //  0056  JMPF	R3	#0064
      0x940C0308,  //  0057  GETIDX	R3	R1	K8
      0x94100304,  //  0058  GETIDX	R4	R1	K4
      0x94140309,  //  0059  GETIDX	R5	R1	K9
      0x60180018,  //  005A  GETGBL	R6	G24
      0x581C000B,  //  005B  LDCONST	R7	K11
      0x5C200600,  //  005C  MOVE	R8	R3
      0x5C240600,  //  005D  MOVE	R9	R3
      0x5C280800,  //  005E  MOVE	R10	R4
      0x5C2C0800,  //  005F  MOVE	R11	R4
      0x5C300A00,  //  0060  MOVE	R12	R5
      0x5C340A00,  //  0061  MOVE	R13	R5
      0x7C180E00,  //  0062  CALL	R6	7
      0x80040C00,  //  0063  RET	1	R6
      0xB80E1800,  //  0064  GETNGBL	R3	K12
      0x8C0C070D,  //  0065  GETMET	R3	R3	K13
      0x5C140200,  //  0066  MOVE	R5	R1
      0x7C0C0400,  //  0067  CALL	R3	2
      0x780E0003,  //  0068  JMPF	R3	#006D
      0x8C0C010E,  //  0069  GETMET	R3	R0	K14
      0x5C140200,  //  006A  MOVE	R5	R1
      0x7C0C0400,  //  006B  CALL	R3	2
      0x80040600,  //  006C  RET	1	R3
      0x80061E00,  //  006D  RET	1	K15
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: transform_expression_for_closure
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_transform_expression_for_closure,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(strip_length),
    /* K3   */  be_nested_str_weak(animation_X2E_X25s_X28engine_X29),
    /* K4   */  be_nested_str_weak(animation_X2Eresolve_X28animation_X2E_X25s_X28engine_X29_X29),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(animation_X2Eresolve_X28),
    /* K8   */  be_nested_str_weak(),
    /* K9   */  be_const_int(2147483647),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(_),
    /* K12  */  be_nested_str_weak(is_identifier_char),
    /* K13  */  be_nested_str_weak(animation_X2E),
    /* K14  */  be_nested_str_weak(animation_X2Eresolve_X28_X25s_X29),
    }),
    be_str_weak(transform_expression_for_closure),
    &be_const_str_solidified,
    ( &(const binstruction[210]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x58100001,  //  0002  LDCONST	R4	K1
      0x60140012,  //  0003  GETGBL	R5	G18
      0x7C140000,  //  0004  CALL	R5	0
      0x40180B02,  //  0005  CONNECT	R6	R5	K2
      0x60180010,  //  0006  GETGBL	R6	G16
      0x5C1C0A00,  //  0007  MOVE	R7	R5
      0x7C180200,  //  0008  CALL	R6	1
      0xA802004F,  //  0009  EXBLK	0	#005A
      0x5C1C0C00,  //  000A  MOVE	R7	R6
      0x7C1C0000,  //  000B  CALL	R7	0
      0x60200018,  //  000C  GETGBL	R8	G24
      0x58240003,  //  000D  LDCONST	R9	K3
      0x5C280E00,  //  000E  MOVE	R10	R7
      0x7C200400,  //  000F  CALL	R8	2
      0x60240018,  //  0010  GETGBL	R9	G24
      0x58280004,  //  0011  LDCONST	R10	K4
      0x5C2C0E00,  //  0012  MOVE	R11	R7
      0x7C240400,  //  0013  CALL	R9	2
      0x58100001,  //  0014  LDCONST	R4	K1
      0x6028000C,  //  0015  GETGBL	R10	G12
      0x5C2C0600,  //  0016  MOVE	R11	R3
      0x7C280200,  //  0017  CALL	R10	1
      0x1428080A,  //  0018  LT	R10	R4	R10
      0x782A003E,  //  0019  JMPF	R10	#0059
      0x8C280505,  //  001A  GETMET	R10	R2	K5
      0x5C300600,  //  001B  MOVE	R12	R3
      0x5C341000,  //  001C  MOVE	R13	R8
      0x5C380800,  //  001D  MOVE	R14	R4
      0x7C280800,  //  001E  CALL	R10	4
      0x142C1501,  //  001F  LT	R11	R10	K1
      0x782E0000,  //  0020  JMPF	R11	#0022
      0x70020036,  //  0021  JMP		#0059
      0x542E0011,  //  0022  LDINT	R11	18
      0x282C140B,  //  0023  GE	R11	R10	R11
      0x782E0002,  //  0024  JMPF	R11	#0028
      0x542E0011,  //  0025  LDINT	R11	18
      0x042C140B,  //  0026  SUB	R11	R10	R11
      0x70020000,  //  0027  JMP		#0029
      0x582C0001,  //  0028  LDCONST	R11	K1
      0x04301506,  //  0029  SUB	R12	R10	K6
      0x4030160C,  //  002A  CONNECT	R12	R11	R12
      0x9430060C,  //  002B  GETIDX	R12	R3	R12
      0x8C340505,  //  002C  GETMET	R13	R2	K5
      0x5C3C1800,  //  002D  MOVE	R15	R12
      0x58400007,  //  002E  LDCONST	R16	K7
      0x7C340600,  //  002F  CALL	R13	3
      0x28341B01,  //  0030  GE	R13	R13	K1
      0x78360005,  //  0031  JMPF	R13	#0038
      0x6034000C,  //  0032  GETGBL	R13	G12
      0x5C381000,  //  0033  MOVE	R14	R8
      0x7C340200,  //  0034  CALL	R13	1
      0x0034140D,  //  0035  ADD	R13	R10	R13
      0x5C101A00,  //  0036  MOVE	R4	R13
      0x7001FFDC,  //  0037  JMP		#0015
      0x24341501,  //  0038  GT	R13	R10	K1
      0x78360003,  //  0039  JMPF	R13	#003E
      0x04341506,  //  003A  SUB	R13	R10	K6
      0x4036020D,  //  003B  CONNECT	R13	K1	R13
      0x9434060D,  //  003C  GETIDX	R13	R3	R13
      0x70020000,  //  003D  JMP		#003F
      0x58340008,  //  003E  LDCONST	R13	K8
      0x6038000C,  //  003F  GETGBL	R14	G12
      0x5C3C1000,  //  0040  MOVE	R15	R8
      0x7C380200,  //  0041  CALL	R14	1
      0x0038140E,  //  0042  ADD	R14	R10	R14
      0x603C000C,  //  0043  GETGBL	R15	G12
      0x5C400600,  //  0044  MOVE	R16	R3
      0x7C3C0200,  //  0045  CALL	R15	1
      0x14381C0F,  //  0046  LT	R14	R14	R15
      0x783A0006,  //  0047  JMPF	R14	#004F
      0x6038000C,  //  0048  GETGBL	R14	G12
      0x5C3C1000,  //  0049  MOVE	R15	R8
      0x7C380200,  //  004A  CALL	R14	1
      0x0038140E,  //  004B  ADD	R14	R10	R14
      0x40381D09,  //  004C  CONNECT	R14	R14	K9
      0x9438060E,  //  004D  GETIDX	R14	R3	R14
      0x70020000,  //  004E  JMP		#0050
      0x58380008,  //  004F  LDCONST	R14	K8
      0x003C1A09,  //  0050  ADD	R15	R13	R9
      0x003C1E0E,  //  0051  ADD	R15	R15	R14
      0x5C0C1E00,  //  0052  MOVE	R3	R15
      0x603C000C,  //  0053  GETGBL	R15	G12
      0x5C401200,  //  0054  MOVE	R16	R9
      0x7C3C0200,  //  0055  CALL	R15	1
      0x003C140F,  //  0056  ADD	R15	R10	R15
      0x5C101E00,  //  0057  MOVE	R4	R15
      0x7001FFBB,  //  0058  JMP		#0015
      0x7001FFAF,  //  0059  JMP		#000A
      0x5818000A,  //  005A  LDCONST	R6	K10
      0xAC180200,  //  005B  CATCH	R6	1	0
      0xB0080000,  //  005C  RAISE	2	R0	R0
      0x58100001,  //  005D  LDCONST	R4	K1
      0x6018000C,  //  005E  GETGBL	R6	G12
      0x5C1C0600,  //  005F  MOVE	R7	R3
      0x7C180200,  //  0060  CALL	R6	1
      0x14180806,  //  0061  LT	R6	R4	R6
      0x781A006D,  //  0062  JMPF	R6	#00D1
      0x8C180505,  //  0063  GETMET	R6	R2	K5
      0x5C200600,  //  0064  MOVE	R8	R3
      0x5824000B,  //  0065  LDCONST	R9	K11
      0x5C280800,  //  0066  MOVE	R10	R4
      0x7C180800,  //  0067  CALL	R6	4
      0x141C0D01,  //  0068  LT	R7	R6	K1
      0x781E0000,  //  0069  JMPF	R7	#006B
      0x70020065,  //  006A  JMP		#00D1
      0x5C1C0C00,  //  006B  MOVE	R7	R6
      0x24200F01,  //  006C  GT	R8	R7	K1
      0x78220006,  //  006D  JMPF	R8	#0075
      0x8C20010C,  //  006E  GETMET	R8	R0	K12
      0x04280F06,  //  006F  SUB	R10	R7	K6
      0x9428060A,  //  0070  GETIDX	R10	R3	R10
      0x7C200400,  //  0071  CALL	R8	2
      0x78220001,  //  0072  JMPF	R8	#0075
      0x041C0F06,  //  0073  SUB	R7	R7	K6
      0x7001FFF6,  //  0074  JMP		#006C
      0x50200200,  //  0075  LDBOOL	R8	1	0
      0x54260011,  //  0076  LDINT	R9	18
      0x28240E09,  //  0077  GE	R9	R7	R9
      0x78260010,  //  0078  JMPF	R9	#008A
      0x54260011,  //  0079  LDINT	R9	18
      0x28240E09,  //  007A  GE	R9	R7	R9
      0x78260002,  //  007B  JMPF	R9	#007F
      0x54260011,  //  007C  LDINT	R9	18
      0x04240E09,  //  007D  SUB	R9	R7	R9
      0x70020000,  //  007E  JMP		#0080
      0x58240001,  //  007F  LDCONST	R9	K1
      0x04280F06,  //  0080  SUB	R10	R7	K6
      0x4028120A,  //  0081  CONNECT	R10	R9	R10
      0x9428060A,  //  0082  GETIDX	R10	R3	R10
      0x8C2C0505,  //  0083  GETMET	R11	R2	K5
      0x5C341400,  //  0084  MOVE	R13	R10
      0x58380007,  //  0085  LDCONST	R14	K7
      0x7C2C0600,  //  0086  CALL	R11	3
      0x282C1701,  //  0087  GE	R11	R11	K1
      0x782E0000,  //  0088  JMPF	R11	#008A
      0x50200000,  //  0089  LDBOOL	R8	0	0
      0x78220013,  //  008A  JMPF	R8	#009F
      0x54260009,  //  008B  LDINT	R9	10
      0x28240E09,  //  008C  GE	R9	R7	R9
      0x78260010,  //  008D  JMPF	R9	#009F
      0x54260009,  //  008E  LDINT	R9	10
      0x28240E09,  //  008F  GE	R9	R7	R9
      0x78260002,  //  0090  JMPF	R9	#0094
      0x54260009,  //  0091  LDINT	R9	10
      0x04240E09,  //  0092  SUB	R9	R7	R9
      0x70020000,  //  0093  JMP		#0095
      0x58240001,  //  0094  LDCONST	R9	K1
      0x04280F06,  //  0095  SUB	R10	R7	K6
      0x4028120A,  //  0096  CONNECT	R10	R9	R10
      0x9428060A,  //  0097  GETIDX	R10	R3	R10
      0x8C2C0505,  //  0098  GETMET	R11	R2	K5
      0x5C341400,  //  0099  MOVE	R13	R10
      0x5838000D,  //  009A  LDCONST	R14	K13
      0x7C2C0600,  //  009B  CALL	R11	3
      0x282C1701,  //  009C  GE	R11	R11	K1
      0x782E0000,  //  009D  JMPF	R11	#009F
      0x50200000,  //  009E  LDBOOL	R8	0	0
      0x7822002D,  //  009F  JMPF	R8	#00CE
      0x14240E06,  //  00A0  LT	R9	R7	R6
      0x7826002B,  //  00A1  JMPF	R9	#00CE
      0x40240E06,  //  00A2  CONNECT	R9	R7	R6
      0x94240609,  //  00A3  GETIDX	R9	R3	R9
      0x00280D06,  //  00A4  ADD	R10	R6	K6
      0x602C000C,  //  00A5  GETGBL	R11	G12
      0x5C300600,  //  00A6  MOVE	R12	R3
      0x7C2C0200,  //  00A7  CALL	R11	1
      0x282C140B,  //  00A8  GE	R11	R10	R11
      0x742E0003,  //  00A9  JMPT	R11	#00AE
      0x8C2C010C,  //  00AA  GETMET	R11	R0	K12
      0x9434060A,  //  00AB  GETIDX	R13	R3	R10
      0x7C2C0400,  //  00AC  CALL	R11	2
      0x742E001C,  //  00AD  JMPT	R11	#00CB
      0x602C0018,  //  00AE  GETGBL	R11	G24
      0x5830000E,  //  00AF  LDCONST	R12	K14
      0x5C341200,  //  00B0  MOVE	R13	R9
      0x7C2C0400,  //  00B1  CALL	R11	2
      0x24300F01,  //  00B2  GT	R12	R7	K1
      0x78320003,  //  00B3  JMPF	R12	#00B8
      0x04300F06,  //  00B4  SUB	R12	R7	K6
      0x4032020C,  //  00B5  CONNECT	R12	K1	R12
      0x9430060C,  //  00B6  GETIDX	R12	R3	R12
      0x70020000,  //  00B7  JMP		#00B9
      0x58300008,  //  00B8  LDCONST	R12	K8
      0x6034000C,  //  00B9  GETGBL	R13	G12
      0x5C380600,  //  00BA  MOVE	R14	R3
      0x7C340200,  //  00BB  CALL	R13	1
      0x1434140D,  //  00BC  LT	R13	R10	R13
      0x78360002,  //  00BD  JMPF	R13	#00C1
      0x40341509,  //  00BE  CONNECT	R13	R10	K9
      0x9434060D,  //  00BF  GETIDX	R13	R3	R13
      0x70020000,  //  00C0  JMP		#00C2
      0x58340008,  //  00C1  LDCONST	R13	K8
      0x0038180B,  //  00C2  ADD	R14	R12	R11
      0x00381C0D,  //  00C3  ADD	R14	R14	R13
      0x5C0C1C00,  //  00C4  MOVE	R3	R14
      0x6038000C,  //  00C5  GETGBL	R14	G12
      0x5C3C1600,  //  00C6  MOVE	R15	R11
      0x7C380200,  //  00C7  CALL	R14	1
      0x00380E0E,  //  00C8  ADD	R14	R7	R14
      0x5C101C00,  //  00C9  MOVE	R4	R14
      0x70020001,  //  00CA  JMP		#00CD
      0x002C0D06,  //  00CB  ADD	R11	R6	K6
      0x5C101600,  //  00CC  MOVE	R4	R11
      0x70020001,  //  00CD  JMP		#00D0
      0x00240D06,  //  00CE  ADD	R9	R6	K6
      0x5C101200,  //  00CF  MOVE	R4	R9
      0x7001FF8C,  //  00D0  JMP		#005E
      0x80040600,  //  00D1  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_palette_color
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_palette_color,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(error),
    /* K2   */  be_nested_str_weak(Expected_X20color_X20value_X20in_X20palette),
    /* K3   */  be_nested_str_weak(0xFFFFFFFF),
    /* K4   */  be_nested_str_weak(type),
    /* K5   */  be_nested_str_weak(animation_dsl),
    /* K6   */  be_nested_str_weak(Token),
    /* K7   */  be_nested_str_weak(COLOR),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(convert_color),
    /* K10  */  be_nested_str_weak(value),
    /* K11  */  be_nested_str_weak(IDENTIFIER),
    /* K12  */  be_nested_str_weak(is_color_name),
    /* K13  */  be_nested_str_weak(get_named_color_value),
    /* K14  */  be_nested_str_weak(Unknown_X20color_X20_X27_X25s_X27_X2E_X20Palettes_X20only_X20accept_X20hex_X20colors_X20_X280xRRGGBB_X29_X20or_X20predefined_X20color_X20names_X20_X28like_X20_X27red_X27_X2C_X20_X27blue_X27_X2C_X20_X27green_X27_X29_X2C_X20but_X20not_X20custom_X20colors_X20defined_X20previously_X2E_X20For_X20dynamic_X20palettes_X20with_X20custom_X20colors_X2C_X20use_X20user_X20functions_X20instead_X2E),
    /* K15  */  be_nested_str_weak(Expected_X20color_X20value_X20in_X20palette_X2E_X20Use_X20hex_X20colors_X20_X280xRRGGBB_X29_X20or_X20predefined_X20color_X20names_X20_X28like_X20_X27red_X27_X2C_X20_X27blue_X27_X2C_X20_X27green_X27_X29_X2E),
    }),
    be_str_weak(process_palette_color),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x1C080202,  //  0003  EQ	R2	R1	R2
      0x780A0003,  //  0004  JMPF	R2	#0009
      0x8C080101,  //  0005  GETMET	R2	R0	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x7C080400,  //  0007  CALL	R2	2
      0x80060600,  //  0008  RET	1	K3
      0x88080304,  //  0009  GETMBR	R2	R1	K4
      0xB80E0A00,  //  000A  GETNGBL	R3	K5
      0x880C0706,  //  000B  GETMBR	R3	R3	K6
      0x880C0707,  //  000C  GETMBR	R3	R3	K7
      0x1C080403,  //  000D  EQ	R2	R2	R3
      0x780A0005,  //  000E  JMPF	R2	#0015
      0x8C080108,  //  000F  GETMET	R2	R0	K8
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080109,  //  0011  GETMET	R2	R0	K9
      0x8810030A,  //  0012  GETMBR	R4	R1	K10
      0x7C080400,  //  0013  CALL	R2	2
      0x80040400,  //  0014  RET	1	R2
      0x88080304,  //  0015  GETMBR	R2	R1	K4
      0xB80E0A00,  //  0016  GETNGBL	R3	K5
      0x880C0706,  //  0017  GETMBR	R3	R3	K6
      0x880C070B,  //  0018  GETMBR	R3	R3	K11
      0x1C080403,  //  0019  EQ	R2	R2	R3
      0x780A0012,  //  001A  JMPF	R2	#002E
      0x8808030A,  //  001B  GETMBR	R2	R1	K10
      0x8C0C0108,  //  001C  GETMET	R3	R0	K8
      0x7C0C0200,  //  001D  CALL	R3	1
      0xB80E0A00,  //  001E  GETNGBL	R3	K5
      0x8C0C070C,  //  001F  GETMET	R3	R3	K12
      0x5C140400,  //  0020  MOVE	R5	R2
      0x7C0C0400,  //  0021  CALL	R3	2
      0x780E0003,  //  0022  JMPF	R3	#0027
      0x8C0C010D,  //  0023  GETMET	R3	R0	K13
      0x5C140400,  //  0024  MOVE	R5	R2
      0x7C0C0400,  //  0025  CALL	R3	2
      0x80040600,  //  0026  RET	1	R3
      0x8C0C0101,  //  0027  GETMET	R3	R0	K1
      0x60140018,  //  0028  GETGBL	R5	G24
      0x5818000E,  //  0029  LDCONST	R6	K14
      0x5C1C0400,  //  002A  MOVE	R7	R2
      0x7C140400,  //  002B  CALL	R5	2
      0x7C0C0400,  //  002C  CALL	R3	2
      0x80060600,  //  002D  RET	1	K3
      0x8C080101,  //  002E  GETMET	R2	R0	K1
      0x5810000F,  //  002F  LDCONST	R4	K15
      0x7C080400,  //  0030  CALL	R2	2
      0x80060600,  //  0031  RET	1	K3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_time_value
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_time_value,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(TIME),
    /* K5   */  be_nested_str_weak(value),
    /* K6   */  be_nested_str_weak(next),
    /* K7   */  be_nested_str_weak(convert_time_to_ms),
    /* K8   */  be_nested_str_weak(NUMBER),
    /* K9   */  be_nested_str_weak(IDENTIFIER),
    /* K10  */  be_nested_str_weak(_validate_object_reference),
    /* K11  */  be_nested_str_weak(duration),
    /* K12  */  be_nested_str_weak(process_primary_expression),
    /* K13  */  be_nested_str_weak(time),
    /* K14  */  be_nested_str_weak(error),
    /* K15  */  be_nested_str_weak(Expected_X20time_X20value),
    }),
    be_str_weak(process_time_value),
    &be_const_str_solidified,
    ( &(const binstruction[67]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A000D,  //  0004  JMPF	R2	#0013
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0007,  //  000A  JMPF	R2	#0013
      0x88080305,  //  000B  GETMBR	R2	R1	K5
      0x8C0C0106,  //  000C  GETMET	R3	R0	K6
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C0107,  //  000E  GETMET	R3	R0	K7
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80040600,  //  0011  RET	1	R3
      0x7002002E,  //  0012  JMP		#0042
      0x4C080000,  //  0013  LDNIL	R2
      0x20080202,  //  0014  NE	R2	R1	R2
      0x780A0011,  //  0015  JMPF	R2	#0028
      0x88080301,  //  0016  GETMBR	R2	R1	K1
      0xB80E0400,  //  0017  GETNGBL	R3	K2
      0x880C0703,  //  0018  GETMBR	R3	R3	K3
      0x880C0708,  //  0019  GETMBR	R3	R3	K8
      0x1C080403,  //  001A  EQ	R2	R2	R3
      0x780A000B,  //  001B  JMPF	R2	#0028
      0x88080305,  //  001C  GETMBR	R2	R1	K5
      0x8C0C0106,  //  001D  GETMET	R3	R0	K6
      0x7C0C0200,  //  001E  CALL	R3	1
      0x600C0009,  //  001F  GETGBL	R3	G9
      0x6010000A,  //  0020  GETGBL	R4	G10
      0x5C140400,  //  0021  MOVE	R5	R2
      0x7C100200,  //  0022  CALL	R4	1
      0x7C0C0200,  //  0023  CALL	R3	1
      0x541203E7,  //  0024  LDINT	R4	1000
      0x080C0604,  //  0025  MUL	R3	R3	R4
      0x80040600,  //  0026  RET	1	R3
      0x70020019,  //  0027  JMP		#0042
      0x4C080000,  //  0028  LDNIL	R2
      0x20080202,  //  0029  NE	R2	R1	R2
      0x780A0011,  //  002A  JMPF	R2	#003D
      0x88080301,  //  002B  GETMBR	R2	R1	K1
      0xB80E0400,  //  002C  GETNGBL	R3	K2
      0x880C0703,  //  002D  GETMBR	R3	R3	K3
      0x880C0709,  //  002E  GETMBR	R3	R3	K9
      0x1C080403,  //  002F  EQ	R2	R2	R3
      0x780A000B,  //  0030  JMPF	R2	#003D
      0x88080305,  //  0031  GETMBR	R2	R1	K5
      0x8C0C010A,  //  0032  GETMET	R3	R0	K10
      0x5C140400,  //  0033  MOVE	R5	R2
      0x5818000B,  //  0034  LDCONST	R6	K11
      0x7C0C0600,  //  0035  CALL	R3	3
      0x8C0C010C,  //  0036  GETMET	R3	R0	K12
      0x5814000D,  //  0037  LDCONST	R5	K13
      0x50180200,  //  0038  LDBOOL	R6	1	0
      0x501C0000,  //  0039  LDBOOL	R7	0	0
      0x7C0C0800,  //  003A  CALL	R3	4
      0x80040600,  //  003B  RET	1	R3
      0x70020004,  //  003C  JMP		#0042
      0x8C08010E,  //  003D  GETMET	R2	R0	K14
      0x5810000F,  //  003E  LDCONST	R4	K15
      0x7C080400,  //  003F  CALL	R2	2
      0x540A03E7,  //  0040  LDINT	R2	1000
      0x80040400,  //  0041  RET	1	R2
      0x80000000,  //  0042  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_simple_function_from_string
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_create_simple_function_from_string,   /* name */
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
    /* K0   */  be_nested_str_weak(def_X20_X28engine_X29_X20return_X20_X25s_X20end),
    }),
    be_str_weak(create_simple_function_from_string),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x60080018,  //  0000  GETGBL	R2	G24
      0x580C0000,  //  0001  LDCONST	R3	K0
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_sequence_assignment_fluent
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_sequence_assignment_fluent,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_identifier),
    /* K1   */  be_nested_str_weak(expect_dot),
    /* K2   */  be_nested_str_weak(expect_assign),
    /* K3   */  be_nested_str_weak(process_value),
    /* K4   */  be_nested_str_weak(property),
    /* K5   */  be_nested_str_weak(collect_inline_comment),
    /* K6   */  be_nested_str_weak(def_X20_X28engine_X29_X20_X25s__X2E_X25s_X20_X3D_X20_X25s_X20end),
    /* K7   */  be_nested_str_weak(add),
    /* K8   */  be_nested_str_weak(_X25s_X2Epush_closure_step_X28_X25s_X29_X25s),
    /* K9   */  be_nested_str_weak(get_indent),
    }),
    be_str_weak(process_sequence_assignment_fluent),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x8C080100,  //  0004  GETMET	R2	R0	K0
      0x7C080200,  //  0005  CALL	R2	1
      0x8C0C0102,  //  0006  GETMET	R3	R0	K2
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0103,  //  0008  GETMET	R3	R0	K3
      0x58140004,  //  0009  LDCONST	R5	K4
      0x7C0C0400,  //  000A  CALL	R3	2
      0x8C100105,  //  000B  GETMET	R4	R0	K5
      0x7C100200,  //  000C  CALL	R4	1
      0x60140018,  //  000D  GETGBL	R5	G24
      0x58180006,  //  000E  LDCONST	R6	K6
      0x5C1C0200,  //  000F  MOVE	R7	R1
      0x5C200400,  //  0010  MOVE	R8	R2
      0x5C240600,  //  0011  MOVE	R9	R3
      0x7C140800,  //  0012  CALL	R5	4
      0x8C180107,  //  0013  GETMET	R6	R0	K7
      0x60200018,  //  0014  GETGBL	R8	G24
      0x58240008,  //  0015  LDCONST	R9	K8
      0x8C280109,  //  0016  GETMET	R10	R0	K9
      0x7C280200,  //  0017  CALL	R10	1
      0x5C2C0A00,  //  0018  MOVE	R11	R5
      0x5C300800,  //  0019  MOVE	R12	R4
      0x7C200800,  //  001A  CALL	R8	4
      0x7C180400,  //  001B  CALL	R6	2
      0x80000000,  //  001C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_peek,   /* name */
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
    /* K0   */  be_nested_str_weak(pos),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(tokens),
    }),
    be_str_weak(peek),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x00040301,  //  0001  ADD	R1	R1	K1
      0x6008000C,  //  0002  GETGBL	R2	G12
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x14040202,  //  0005  LT	R1	R1	R2
      0x78060004,  //  0006  JMPF	R1	#000C
      0x88040100,  //  0007  GETMBR	R1	R0	K0
      0x00040301,  //  0008  ADD	R1	R1	K1
      0x88080102,  //  0009  GETMBR	R2	R0	K2
      0x94040401,  //  000A  GETIDX	R1	R2	R1
      0x70020000,  //  000B  JMP		#000D
      0x4C040000,  //  000C  LDNIL	R1
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_computation_closure
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_create_computation_closure,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(transform_operand_for_closure),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(_X20_X20),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(replace),
    /* K6   */  be_nested_str_weak(_X20),
    /* K7   */  be_nested_str_weak(def_X20_X28engine_X29_X20return_X20_X25s_X20_X25s_X20_X25s_X20end),
    /* K8   */  be_nested_str_weak(animation_X2Ecreate_closure_value_X28engine_X2C_X20_X25s_X29),
    }),
    be_str_weak(create_computation_closure),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x8C140101,  //  0001  GETMET	R5	R0	K1
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x7C140400,  //  0003  CALL	R5	2
      0x8C180101,  //  0004  GETMET	R6	R0	K1
      0x5C200600,  //  0005  MOVE	R8	R3
      0x7C180400,  //  0006  CALL	R6	2
      0x8C1C0902,  //  0007  GETMET	R7	R4	K2
      0x5C240A00,  //  0008  MOVE	R9	R5
      0x58280003,  //  0009  LDCONST	R10	K3
      0x7C1C0600,  //  000A  CALL	R7	3
      0x281C0F04,  //  000B  GE	R7	R7	K4
      0x781E0006,  //  000C  JMPF	R7	#0014
      0x8C1C0905,  //  000D  GETMET	R7	R4	K5
      0x5C240A00,  //  000E  MOVE	R9	R5
      0x58280003,  //  000F  LDCONST	R10	K3
      0x582C0006,  //  0010  LDCONST	R11	K6
      0x7C1C0800,  //  0011  CALL	R7	4
      0x5C140E00,  //  0012  MOVE	R5	R7
      0x7001FFF2,  //  0013  JMP		#0007
      0x8C1C0902,  //  0014  GETMET	R7	R4	K2
      0x5C240C00,  //  0015  MOVE	R9	R6
      0x58280003,  //  0016  LDCONST	R10	K3
      0x7C1C0600,  //  0017  CALL	R7	3
      0x281C0F04,  //  0018  GE	R7	R7	K4
      0x781E0006,  //  0019  JMPF	R7	#0021
      0x8C1C0905,  //  001A  GETMET	R7	R4	K5
      0x5C240C00,  //  001B  MOVE	R9	R6
      0x58280003,  //  001C  LDCONST	R10	K3
      0x582C0006,  //  001D  LDCONST	R11	K6
      0x7C1C0800,  //  001E  CALL	R7	4
      0x5C180E00,  //  001F  MOVE	R6	R7
      0x7001FFF2,  //  0020  JMP		#0014
      0x601C0018,  //  0021  GETGBL	R7	G24
      0x58200007,  //  0022  LDCONST	R8	K7
      0x5C240A00,  //  0023  MOVE	R9	R5
      0x5C280400,  //  0024  MOVE	R10	R2
      0x5C2C0C00,  //  0025  MOVE	R11	R6
      0x7C1C0800,  //  0026  CALL	R7	4
      0x60200018,  //  0027  GETGBL	R8	G24
      0x58240008,  //  0028  LDCONST	R9	K8
      0x5C280E00,  //  0029  MOVE	R10	R7
      0x7C200400,  //  002A  CALL	R8	2
      0x80041000,  //  002B  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_function_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_function_call,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(type),
    /* K3   */  be_nested_str_weak(animation_dsl),
    /* K4   */  be_nested_str_weak(Token),
    /* K5   */  be_nested_str_weak(IDENTIFIER),
    /* K6   */  be_nested_str_weak(KEYWORD),
    /* K7   */  be_nested_str_weak(value),
    /* K8   */  be_nested_str_weak(next),
    /* K9   */  be_nested_str_weak(error),
    /* K10  */  be_nested_str_weak(Expected_X20function_X20name),
    /* K11  */  be_nested_str_weak(nil),
    /* K12  */  be_nested_str_weak(is_math_method),
    /* K13  */  be_nested_str_weak(process_function_arguments),
    /* K14  */  be_nested_str_weak(_X25s_X28_X25s_X29),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(process_log_call),
    /* K17  */  be_nested_str_weak(expression),
    /* K18  */  be_nested_str_weak(template_definitions),
    /* K19  */  be_nested_str_weak(contains),
    /* K20  */  be_nested_str_weak(engine_X2C_X20_X25s),
    /* K21  */  be_nested_str_weak(engine),
    /* K22  */  be_nested_str_weak(_X25s_template_X28_X25s_X29),
    /* K23  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X2C_X20_X25s_X29),
    /* K24  */  be_nested_str_weak(animation_X2E_X25s_X28engine_X29),
    }),
    be_str_weak(process_function_call),
    &be_const_str_solidified,
    ( &(const binstruction[88]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x580C0001,  //  0002  LDCONST	R3	K1
      0x4C100000,  //  0003  LDNIL	R4
      0x20100404,  //  0004  NE	R4	R2	R4
      0x7812000F,  //  0005  JMPF	R4	#0016
      0x88100502,  //  0006  GETMBR	R4	R2	K2
      0xB8160600,  //  0007  GETNGBL	R5	K3
      0x88140B04,  //  0008  GETMBR	R5	R5	K4
      0x88140B05,  //  0009  GETMBR	R5	R5	K5
      0x1C100805,  //  000A  EQ	R4	R4	R5
      0x74120005,  //  000B  JMPT	R4	#0012
      0x88100502,  //  000C  GETMBR	R4	R2	K2
      0xB8160600,  //  000D  GETNGBL	R5	K3
      0x88140B04,  //  000E  GETMBR	R5	R5	K4
      0x88140B06,  //  000F  GETMBR	R5	R5	K6
      0x1C100805,  //  0010  EQ	R4	R4	R5
      0x78120003,  //  0011  JMPF	R4	#0016
      0x880C0507,  //  0012  GETMBR	R3	R2	K7
      0x8C100108,  //  0013  GETMET	R4	R0	K8
      0x7C100200,  //  0014  CALL	R4	1
      0x70020003,  //  0015  JMP		#001A
      0x8C100109,  //  0016  GETMET	R4	R0	K9
      0x5818000A,  //  0017  LDCONST	R6	K10
      0x7C100400,  //  0018  CALL	R4	2
      0x80061600,  //  0019  RET	1	K11
      0x8C10010C,  //  001A  GETMET	R4	R0	K12
      0x5C180600,  //  001B  MOVE	R6	R3
      0x7C100400,  //  001C  CALL	R4	2
      0x78120008,  //  001D  JMPF	R4	#0027
      0x8C10010D,  //  001E  GETMET	R4	R0	K13
      0x50180000,  //  001F  LDBOOL	R6	0	0
      0x7C100400,  //  0020  CALL	R4	2
      0x60140018,  //  0021  GETGBL	R5	G24
      0x5818000E,  //  0022  LDCONST	R6	K14
      0x5C1C0600,  //  0023  MOVE	R7	R3
      0x5C200800,  //  0024  MOVE	R8	R4
      0x7C140600,  //  0025  CALL	R5	3
      0x80040A00,  //  0026  RET	1	R5
      0x1C10070F,  //  0027  EQ	R4	R3	K15
      0x78120008,  //  0028  JMPF	R4	#0032
      0x8C10010D,  //  0029  GETMET	R4	R0	K13
      0x50180000,  //  002A  LDBOOL	R6	0	0
      0x7C100400,  //  002B  CALL	R4	2
      0x8C140110,  //  002C  GETMET	R5	R0	K16
      0x5C1C0800,  //  002D  MOVE	R7	R4
      0x58200011,  //  002E  LDCONST	R8	K17
      0x58240001,  //  002F  LDCONST	R9	K1
      0x7C140800,  //  0030  CALL	R5	4
      0x80040A00,  //  0031  RET	1	R5
      0x8C10010D,  //  0032  GETMET	R4	R0	K13
      0x50180000,  //  0033  LDBOOL	R6	0	0
      0x7C100400,  //  0034  CALL	R4	2
      0x88140112,  //  0035  GETMBR	R5	R0	K18
      0x8C140B13,  //  0036  GETMET	R5	R5	K19
      0x5C1C0600,  //  0037  MOVE	R7	R3
      0x7C140400,  //  0038  CALL	R5	2
      0x7816000E,  //  0039  JMPF	R5	#0049
      0x20140901,  //  003A  NE	R5	R4	K1
      0x78160004,  //  003B  JMPF	R5	#0041
      0x60140018,  //  003C  GETGBL	R5	G24
      0x58180014,  //  003D  LDCONST	R6	K20
      0x5C1C0800,  //  003E  MOVE	R7	R4
      0x7C140400,  //  003F  CALL	R5	2
      0x70020000,  //  0040  JMP		#0042
      0x58140015,  //  0041  LDCONST	R5	K21
      0x60180018,  //  0042  GETGBL	R6	G24
      0x581C0016,  //  0043  LDCONST	R7	K22
      0x5C200600,  //  0044  MOVE	R8	R3
      0x5C240A00,  //  0045  MOVE	R9	R5
      0x7C180600,  //  0046  CALL	R6	3
      0x80040C00,  //  0047  RET	1	R6
      0x7002000D,  //  0048  JMP		#0057
      0x20140901,  //  0049  NE	R5	R4	K1
      0x78160006,  //  004A  JMPF	R5	#0052
      0x60140018,  //  004B  GETGBL	R5	G24
      0x58180017,  //  004C  LDCONST	R6	K23
      0x5C1C0600,  //  004D  MOVE	R7	R3
      0x5C200800,  //  004E  MOVE	R8	R4
      0x7C140600,  //  004F  CALL	R5	3
      0x80040A00,  //  0050  RET	1	R5
      0x70020004,  //  0051  JMP		#0057
      0x60140018,  //  0052  GETGBL	R5	G24
      0x58180018,  //  0053  LDCONST	R6	K24
      0x5C1C0600,  //  0054  MOVE	R7	R3
      0x7C140400,  //  0055  CALL	R5	2
      0x80040A00,  //  0056  RET	1	R5
      0x80000000,  //  0057  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_event_parameters
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_event_parameters,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_paren),
    /* K1   */  be_nested_str_weak(_X7B),
    /* K2   */  be_nested_str_weak(at_end),
    /* K3   */  be_nested_str_weak(check_right_paren),
    /* K4   */  be_nested_str_weak(current),
    /* K5   */  be_nested_str_weak(type),
    /* K6   */  be_nested_str_weak(animation_dsl),
    /* K7   */  be_nested_str_weak(Token),
    /* K8   */  be_nested_str_weak(TIME),
    /* K9   */  be_nested_str_weak(process_time_value),
    /* K10  */  be_nested_str_weak(_X22interval_X22_X3A_X20_X25s),
    /* K11  */  be_nested_str_weak(process_value),
    /* K12  */  be_nested_str_weak(event_param),
    /* K13  */  be_nested_str_weak(_X22value_X22_X3A_X20_X25s),
    /* K14  */  be_nested_str_weak(expect_right_paren),
    /* K15  */  be_nested_str_weak(_X7D),
    }),
    be_str_weak(process_event_parameters),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x58040001,  //  0002  LDCONST	R1	K1
      0x8C080102,  //  0003  GETMET	R2	R0	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x740A001D,  //  0005  JMPT	R2	#0024
      0x8C080103,  //  0006  GETMET	R2	R0	K3
      0x7C080200,  //  0007  CALL	R2	1
      0x740A001A,  //  0008  JMPT	R2	#0024
      0x8C080104,  //  0009  GETMET	R2	R0	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x4C0C0000,  //  000B  LDNIL	R3
      0x200C0403,  //  000C  NE	R3	R2	R3
      0x780E000D,  //  000D  JMPF	R3	#001C
      0x880C0505,  //  000E  GETMBR	R3	R2	K5
      0xB8120C00,  //  000F  GETNGBL	R4	K6
      0x88100907,  //  0010  GETMBR	R4	R4	K7
      0x88100908,  //  0011  GETMBR	R4	R4	K8
      0x1C0C0604,  //  0012  EQ	R3	R3	R4
      0x780E0007,  //  0013  JMPF	R3	#001C
      0x8C0C0109,  //  0014  GETMET	R3	R0	K9
      0x7C0C0200,  //  0015  CALL	R3	1
      0x60100018,  //  0016  GETGBL	R4	G24
      0x5814000A,  //  0017  LDCONST	R5	K10
      0x5C180600,  //  0018  MOVE	R6	R3
      0x7C100400,  //  0019  CALL	R4	2
      0x00040204,  //  001A  ADD	R1	R1	R4
      0x70020007,  //  001B  JMP		#0024
      0x8C0C010B,  //  001C  GETMET	R3	R0	K11
      0x5814000C,  //  001D  LDCONST	R5	K12
      0x7C0C0400,  //  001E  CALL	R3	2
      0x60100018,  //  001F  GETGBL	R4	G24
      0x5814000D,  //  0020  LDCONST	R5	K13
      0x5C180600,  //  0021  MOVE	R6	R3
      0x7C100400,  //  0022  CALL	R4	2
      0x00040204,  //  0023  ADD	R1	R1	R4
      0x8C08010E,  //  0024  GETMET	R2	R0	K14
      0x7C080200,  //  0025  CALL	R2	1
      0x0004030F,  //  0026  ADD	R1	R1	K15
      0x80040200,  //  0027  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_log_call
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_log_call,   /* name */
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
    /* K0   */  be_nested_str_weak(fluent),
    /* K1   */  be_nested_str_weak(def_X20_X28engine_X29_X20log_X28f_X22_X25s_X22_X2C_X203_X29_X20end),
    /* K2   */  be_nested_str_weak(_X25s_X2Epush_closure_step_X28_X25s_X29_X25s),
    /* K3   */  be_nested_str_weak(get_indent),
    /* K4   */  be_nested_str_weak(expression),
    /* K5   */  be_nested_str_weak(log_X28f_X22_X25s_X22_X2C_X203_X29),
    /* K6   */  be_nested_str_weak(log_X28f_X22_X25s_X22_X2C_X203_X29_X25s),
    }),
    be_str_weak(process_log_call),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x1C100500,  //  0000  EQ	R4	R2	K0
      0x7812000C,  //  0001  JMPF	R4	#000F
      0x60100018,  //  0002  GETGBL	R4	G24
      0x58140001,  //  0003  LDCONST	R5	K1
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x60140018,  //  0006  GETGBL	R5	G24
      0x58180002,  //  0007  LDCONST	R6	K2
      0x8C1C0103,  //  0008  GETMET	R7	R0	K3
      0x7C1C0200,  //  0009  CALL	R7	1
      0x5C200800,  //  000A  MOVE	R8	R4
      0x5C240600,  //  000B  MOVE	R9	R3
      0x7C140800,  //  000C  CALL	R5	4
      0x80040A00,  //  000D  RET	1	R5
      0x7002000D,  //  000E  JMP		#001D
      0x1C100504,  //  000F  EQ	R4	R2	K4
      0x78120005,  //  0010  JMPF	R4	#0017
      0x60100018,  //  0011  GETGBL	R4	G24
      0x58140005,  //  0012  LDCONST	R5	K5
      0x5C180200,  //  0013  MOVE	R6	R1
      0x7C100400,  //  0014  CALL	R4	2
      0x80040800,  //  0015  RET	1	R4
      0x70020005,  //  0016  JMP		#001D
      0x60100018,  //  0017  GETGBL	R4	G24
      0x58140006,  //  0018  LDCONST	R5	K6
      0x5C180200,  //  0019  MOVE	R6	R1
      0x5C1C0600,  //  001A  MOVE	R7	R3
      0x7C100600,  //  001B  CALL	R4	3
      0x80040800,  //  001C  RET	1	R4
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _validate_single_parameter
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__validate_single_parameter,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(_has_param),
    /* K3   */  be_nested_str_weak(current),
    /* K4   */  be_nested_str_weak(line),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Animation_X20_X27_X25s_X27_X20does_X20not_X20have_X20parameter_X20_X27_X25s_X27_X2E_X20Check_X20the_X20animation_X20documentation_X20for_X20valid_X20parameters_X2E),
    }),
    be_str_weak(_validate_single_parameter),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0xA802001F,  //  0000  EXBLK	0	#0021
      0xA4120000,  //  0001  IMPORT	R4	K0
      0x4C140000,  //  0002  LDNIL	R5
      0x20140605,  //  0003  NE	R5	R3	R5
      0x78160019,  //  0004  JMPF	R5	#001F
      0x8C140901,  //  0005  GETMET	R5	R4	K1
      0x5C1C0600,  //  0006  MOVE	R7	R3
      0x58200002,  //  0007  LDCONST	R8	K2
      0x7C140600,  //  0008  CALL	R5	3
      0x78160014,  //  0009  JMPF	R5	#001F
      0x8C140702,  //  000A  GETMET	R5	R3	K2
      0x5C1C0400,  //  000B  MOVE	R7	R2
      0x7C140400,  //  000C  CALL	R5	2
      0x74160010,  //  000D  JMPT	R5	#001F
      0x8C140103,  //  000E  GETMET	R5	R0	K3
      0x7C140200,  //  000F  CALL	R5	1
      0x4C180000,  //  0010  LDNIL	R6
      0x20140A06,  //  0011  NE	R5	R5	R6
      0x78160003,  //  0012  JMPF	R5	#0017
      0x8C140103,  //  0013  GETMET	R5	R0	K3
      0x7C140200,  //  0014  CALL	R5	1
      0x88140B04,  //  0015  GETMBR	R5	R5	K4
      0x70020000,  //  0016  JMP		#0018
      0x58140005,  //  0017  LDCONST	R5	K5
      0x8C180106,  //  0018  GETMET	R6	R0	K6
      0x60200018,  //  0019  GETGBL	R8	G24
      0x58240007,  //  001A  LDCONST	R9	K7
      0x5C280200,  //  001B  MOVE	R10	R1
      0x5C2C0400,  //  001C  MOVE	R11	R2
      0x7C200600,  //  001D  CALL	R8	3
      0x7C180400,  //  001E  CALL	R6	2
      0xA8040001,  //  001F  EXBLK	1	1
      0x70020003,  //  0020  JMP		#0025
      0xAC100002,  //  0021  CATCH	R4	0	2
      0x70020000,  //  0022  JMP		#0024
      0x70020000,  //  0023  JMP		#0025
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: transform_operand_for_closure
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_transform_operand_for_closure,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(animation_X2Ecreate_closure_value),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(return_X20_X28),
    /* K5   */  be_nested_str_weak(_X20_X20),
    /* K6   */  be_nested_str_weak(replace),
    /* K7   */  be_nested_str_weak(_X20),
    /* K8   */  be_nested_str_weak(_),
    /* K9   */  be_nested_str_weak(_X28),
    /* K10  */  be_nested_str_weak(animation_X2E),
    /* K11  */  be_nested_str_weak(animation_X2Eresolve_X28_X25s_X29),
    }),
    be_str_weak(transform_operand_for_closure),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x280C0703,  //  0005  GE	R3	R3	K3
      0x780E001A,  //  0006  JMPF	R3	#0022
      0x8C0C0501,  //  0007  GETMET	R3	R2	K1
      0x5C140200,  //  0008  MOVE	R5	R1
      0x58180004,  //  0009  LDCONST	R6	K4
      0x7C0C0600,  //  000A  CALL	R3	3
      0x54120007,  //  000B  LDINT	R4	8
      0x000C0604,  //  000C  ADD	R3	R3	R4
      0x6010000C,  //  000D  GETGBL	R4	G12
      0x5C140200,  //  000E  MOVE	R5	R1
      0x7C100200,  //  000F  CALL	R4	1
      0x54160004,  //  0010  LDINT	R5	5
      0x04100805,  //  0011  SUB	R4	R4	R5
      0x40140604,  //  0012  CONNECT	R5	R3	R4
      0x94140205,  //  0013  GETIDX	R5	R1	R5
      0x8C180501,  //  0014  GETMET	R6	R2	K1
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x58240005,  //  0016  LDCONST	R9	K5
      0x7C180600,  //  0017  CALL	R6	3
      0x28180D03,  //  0018  GE	R6	R6	K3
      0x781A0006,  //  0019  JMPF	R6	#0021
      0x8C180506,  //  001A  GETMET	R6	R2	K6
      0x5C200A00,  //  001B  MOVE	R8	R5
      0x58240005,  //  001C  LDCONST	R9	K5
      0x58280007,  //  001D  LDCONST	R10	K7
      0x7C180800,  //  001E  CALL	R6	4
      0x5C140C00,  //  001F  MOVE	R5	R6
      0x7001FFF2,  //  0020  JMP		#0014
      0x80040A00,  //  0021  RET	1	R5
      0x8C0C0501,  //  0022  GETMET	R3	R2	K1
      0x5C140200,  //  0023  MOVE	R5	R1
      0x58180008,  //  0024  LDCONST	R6	K8
      0x7C0C0600,  //  0025  CALL	R3	3
      0x280C0703,  //  0026  GE	R3	R3	K3
      0x8C100501,  //  0027  GETMET	R4	R2	K1
      0x5C180200,  //  0028  MOVE	R6	R1
      0x581C0007,  //  0029  LDCONST	R7	K7
      0x7C100600,  //  002A  CALL	R4	3
      0x28100903,  //  002B  GE	R4	R4	K3
      0x8C140501,  //  002C  GETMET	R5	R2	K1
      0x5C1C0200,  //  002D  MOVE	R7	R1
      0x58200009,  //  002E  LDCONST	R8	K9
      0x7C140600,  //  002F  CALL	R5	3
      0x28140B03,  //  0030  GE	R5	R5	K3
      0x8C180501,  //  0031  GETMET	R6	R2	K1
      0x5C200200,  //  0032  MOVE	R8	R1
      0x5824000A,  //  0033  LDCONST	R9	K10
      0x7C180600,  //  0034  CALL	R6	3
      0x28180D03,  //  0035  GE	R6	R6	K3
      0x780E000B,  //  0036  JMPF	R3	#0043
      0x5C1C0800,  //  0037  MOVE	R7	R4
      0x741E0009,  //  0038  JMPT	R7	#0043
      0x5C1C0A00,  //  0039  MOVE	R7	R5
      0x741E0007,  //  003A  JMPT	R7	#0043
      0x5C1C0C00,  //  003B  MOVE	R7	R6
      0x741E0005,  //  003C  JMPT	R7	#0043
      0x601C0018,  //  003D  GETGBL	R7	G24
      0x5820000B,  //  003E  LDCONST	R8	K11
      0x5C240200,  //  003F  MOVE	R9	R1
      0x7C1C0400,  //  0040  CALL	R7	2
      0x80040E00,  //  0041  RET	1	R7
      0x70020000,  //  0042  JMP		#0044
      0x80040200,  //  0043  RET	1	R1
      0x80000000,  //  0044  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_run
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_run,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(next),
    /* K1   */  be_nested_str_weak(expect_identifier),
    /* K2   */  be_nested_str_weak(_validate_object_reference),
    /* K3   */  be_nested_str_weak(run),
    /* K4   */  be_nested_str_weak(collect_inline_comment),
    /* K5   */  be_nested_str_weak(run_statements),
    /* K6   */  be_nested_str_weak(push),
    /* K7   */  be_nested_str_weak(name),
    /* K8   */  be_nested_str_weak(comment),
    }),
    be_str_weak(process_run),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x8C080102,  //  0004  GETMET	R2	R0	K2
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140003,  //  0006  LDCONST	R5	K3
      0x7C080600,  //  0007  CALL	R2	3
      0x8C080104,  //  0008  GETMET	R2	R0	K4
      0x7C080200,  //  0009  CALL	R2	1
      0x880C0105,  //  000A  GETMBR	R3	R0	K5
      0x8C0C0706,  //  000B  GETMET	R3	R3	K6
      0x60140013,  //  000C  GETGBL	R5	G19
      0x7C140000,  //  000D  CALL	R5	0
      0x98160E01,  //  000E  SETIDX	R5	K7	R1
      0x98161002,  //  000F  SETIDX	R5	K8	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has_errors
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_has_errors,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(errors),
    /* K1   */  be_const_int(0),
    }),
    be_str_weak(has_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x24040301,  //  0003  GT	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _create_instance_for_validation
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__create_instance_for_validation,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(MockEngine),
    /* K2   */  be_nested_str_weak(introspect),
    /* K3   */  be_nested_str_weak(contains),
    /* K4   */  be_nested_str_weak(animation),
    /* K5   */  be_nested_str_weak(class),
    /* K6   */  be_nested_str_weak(function),
    }),
    be_str_weak(_create_instance_for_validation),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0xA802001E,  //  0000  EXBLK	0	#0020
      0xB80A0000,  //  0001  GETNGBL	R2	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x7C080200,  //  0003  CALL	R2	1
      0xA40E0400,  //  0004  IMPORT	R3	K2
      0x8C100703,  //  0005  GETMET	R4	R3	K3
      0xB81A0800,  //  0006  GETNGBL	R6	K4
      0x5C1C0200,  //  0007  MOVE	R7	R1
      0x7C100600,  //  0008  CALL	R4	3
      0x78120010,  //  0009  JMPF	R4	#001B
      0xB8120800,  //  000A  GETNGBL	R4	K4
      0x88100801,  //  000B  GETMBR	R4	R4	R1
      0x60140004,  //  000C  GETGBL	R5	G4
      0x5C180800,  //  000D  MOVE	R6	R4
      0x7C140200,  //  000E  CALL	R5	1
      0x1C140B05,  //  000F  EQ	R5	R5	K5
      0x74160004,  //  0010  JMPT	R5	#0016
      0x60140004,  //  0011  GETGBL	R5	G4
      0x5C180800,  //  0012  MOVE	R6	R4
      0x7C140200,  //  0013  CALL	R5	1
      0x1C140B06,  //  0014  EQ	R5	R5	K6
      0x78160004,  //  0015  JMPF	R5	#001B
      0x5C140800,  //  0016  MOVE	R5	R4
      0x5C180400,  //  0017  MOVE	R6	R2
      0x7C140200,  //  0018  CALL	R5	1
      0xA8040001,  //  0019  EXBLK	1	1
      0x80040A00,  //  001A  RET	1	R5
      0x4C100000,  //  001B  LDNIL	R4
      0xA8040001,  //  001C  EXBLK	1	1
      0x80040800,  //  001D  RET	1	R4
      0xA8040001,  //  001E  EXBLK	1	1
      0x70020005,  //  001F  JMP		#0026
      0xAC080002,  //  0020  CATCH	R2	0	2
      0x70020002,  //  0021  JMP		#0025
      0x4C100000,  //  0022  LDNIL	R4
      0x80040800,  //  0023  RET	1	R4
      0x70020000,  //  0024  JMP		#0026
      0xB0080000,  //  0025  RAISE	2	R0	R0
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_default_strip_initialization
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_generate_default_strip_initialization,   /* name */
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
    /* K0   */  be_nested_str_weak(strip_initialized),
    /* K1   */  be_nested_str_weak(add),
    /* K2   */  be_nested_str_weak(_X23_X20Auto_X2Dgenerated_X20strip_X20initialization_X20_X28using_X20Tasmota_X20configuration_X29),
    /* K3   */  be_nested_str_weak(var_X20engine_X20_X3D_X20animation_X2Einit_strip_X28_X29),
    /* K4   */  be_nested_str_weak(),
    }),
    be_str_weak(generate_default_strip_initialization),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060000,  //  0001  JMPF	R1	#0003
      0x80000200,  //  0002  RET	0
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x580C0002,  //  0004  LDCONST	R3	K2
      0x7C040400,  //  0005  CALL	R1	2
      0x8C040101,  //  0006  GETMET	R1	R0	K1
      0x580C0003,  //  0007  LDCONST	R3	K3
      0x7C040400,  //  0008  CALL	R1	2
      0x8C040101,  //  0009  GETMET	R1	R0	K1
      0x580C0004,  //  000A  LDCONST	R3	K4
      0x7C040400,  //  000B  CALL	R1	2
      0x50040200,  //  000C  LDBOOL	R1	1	0
      0x90020001,  //  000D  SETMBR	R0	K0	R1
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_array_literal
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_process_array_literal,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(expect_left_bracket),
    /* K1   */  be_nested_str_weak(at_end),
    /* K2   */  be_nested_str_weak(check_right_bracket),
    /* K3   */  be_nested_str_weak(process_value),
    /* K4   */  be_nested_str_weak(array_element),
    /* K5   */  be_nested_str_weak(push),
    /* K6   */  be_nested_str_weak(current),
    /* K7   */  be_nested_str_weak(type),
    /* K8   */  be_nested_str_weak(animation_dsl),
    /* K9   */  be_nested_str_weak(Token),
    /* K10  */  be_nested_str_weak(COMMA),
    /* K11  */  be_nested_str_weak(next),
    /* K12  */  be_nested_str_weak(error),
    /* K13  */  be_nested_str_weak(Expected_X20_X27_X2C_X27_X20or_X20_X27_X5D_X27_X20in_X20array_X20literal),
    /* K14  */  be_nested_str_weak(expect_right_bracket),
    /* K15  */  be_nested_str_weak(_X5B),
    /* K16  */  be_const_int(0),
    /* K17  */  be_const_int(1),
    /* K18  */  be_nested_str_weak(_X2C_X20),
    /* K19  */  be_nested_str_weak(stop_iteration),
    /* K20  */  be_nested_str_weak(_X5D),
    }),
    be_str_weak(process_array_literal),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x60040012,  //  0002  GETGBL	R1	G18
      0x7C040000,  //  0003  CALL	R1	0
      0x8C080101,  //  0004  GETMET	R2	R0	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x740A0020,  //  0006  JMPT	R2	#0028
      0x8C080102,  //  0007  GETMET	R2	R0	K2
      0x7C080200,  //  0008  CALL	R2	1
      0x740A001D,  //  0009  JMPT	R2	#0028
      0x8C080103,  //  000A  GETMET	R2	R0	K3
      0x58100004,  //  000B  LDCONST	R4	K4
      0x7C080400,  //  000C  CALL	R2	2
      0x8C0C0305,  //  000D  GETMET	R3	R1	K5
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x8C0C0106,  //  0010  GETMET	R3	R0	K6
      0x7C0C0200,  //  0011  CALL	R3	1
      0x4C100000,  //  0012  LDNIL	R4
      0x200C0604,  //  0013  NE	R3	R3	R4
      0x780E000A,  //  0014  JMPF	R3	#0020
      0x8C0C0106,  //  0015  GETMET	R3	R0	K6
      0x7C0C0200,  //  0016  CALL	R3	1
      0x880C0707,  //  0017  GETMBR	R3	R3	K7
      0xB8121000,  //  0018  GETNGBL	R4	K8
      0x88100909,  //  0019  GETMBR	R4	R4	K9
      0x8810090A,  //  001A  GETMBR	R4	R4	K10
      0x1C0C0604,  //  001B  EQ	R3	R3	R4
      0x780E0002,  //  001C  JMPF	R3	#0020
      0x8C0C010B,  //  001D  GETMET	R3	R0	K11
      0x7C0C0200,  //  001E  CALL	R3	1
      0x70020006,  //  001F  JMP		#0027
      0x8C0C0102,  //  0020  GETMET	R3	R0	K2
      0x7C0C0200,  //  0021  CALL	R3	1
      0x740E0003,  //  0022  JMPT	R3	#0027
      0x8C0C010C,  //  0023  GETMET	R3	R0	K12
      0x5814000D,  //  0024  LDCONST	R5	K13
      0x7C0C0400,  //  0025  CALL	R3	2
      0x70020000,  //  0026  JMP		#0028
      0x7001FFDB,  //  0027  JMP		#0004
      0x8C08010E,  //  0028  GETMET	R2	R0	K14
      0x7C080200,  //  0029  CALL	R2	1
      0x5808000F,  //  002A  LDCONST	R2	K15
      0x600C0010,  //  002B  GETGBL	R3	G16
      0x6010000C,  //  002C  GETGBL	R4	G12
      0x5C140200,  //  002D  MOVE	R5	R1
      0x7C100200,  //  002E  CALL	R4	1
      0x04100911,  //  002F  SUB	R4	R4	K17
      0x40122004,  //  0030  CONNECT	R4	K16	R4
      0x7C0C0200,  //  0031  CALL	R3	1
      0xA8020007,  //  0032  EXBLK	0	#003B
      0x5C100600,  //  0033  MOVE	R4	R3
      0x7C100000,  //  0034  CALL	R4	0
      0x24140910,  //  0035  GT	R5	R4	K16
      0x78160000,  //  0036  JMPF	R5	#0038
      0x00080512,  //  0037  ADD	R2	R2	K18
      0x94140204,  //  0038  GETIDX	R5	R1	R4
      0x00080405,  //  0039  ADD	R2	R2	R5
      0x7001FFF7,  //  003A  JMP		#0033
      0x580C0013,  //  003B  LDCONST	R3	K19
      0xAC0C0200,  //  003C  CATCH	R3	1	0
      0xB0080000,  //  003D  RAISE	2	R0	R0
      0x00080514,  //  003E  ADD	R2	R2	K20
      0x80040400,  //  003F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: convert_time_to_ms
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_convert_time_to_ms,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(endswith),
    /* K2   */  be_nested_str_weak(ms),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(s),
    /* K5   */  be_nested_str_weak(m),
    /* K6   */  be_nested_str_weak(h),
    /* K7   */  be_const_int(3600000),
    }),
    be_str_weak(convert_time_to_ms),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x600C0009,  //  0006  GETGBL	R3	G9
      0x6010000A,  //  0007  GETGBL	R4	G10
      0x5415FFFC,  //  0008  LDINT	R5	-3
      0x40160605,  //  0009  CONNECT	R5	K3	R5
      0x94140205,  //  000A  GETIDX	R5	R1	R5
      0x7C100200,  //  000B  CALL	R4	1
      0x7C0C0200,  //  000C  CALL	R3	1
      0x80040600,  //  000D  RET	1	R3
      0x7002002D,  //  000E  JMP		#003D
      0x8C0C0501,  //  000F  GETMET	R3	R2	K1
      0x5C140200,  //  0010  MOVE	R5	R1
      0x58180004,  //  0011  LDCONST	R6	K4
      0x7C0C0600,  //  0012  CALL	R3	3
      0x780E000A,  //  0013  JMPF	R3	#001F
      0x600C0009,  //  0014  GETGBL	R3	G9
      0x6010000A,  //  0015  GETGBL	R4	G10
      0x5415FFFD,  //  0016  LDINT	R5	-2
      0x40160605,  //  0017  CONNECT	R5	K3	R5
      0x94140205,  //  0018  GETIDX	R5	R1	R5
      0x7C100200,  //  0019  CALL	R4	1
      0x541603E7,  //  001A  LDINT	R5	1000
      0x08100805,  //  001B  MUL	R4	R4	R5
      0x7C0C0200,  //  001C  CALL	R3	1
      0x80040600,  //  001D  RET	1	R3
      0x7002001D,  //  001E  JMP		#003D
      0x8C0C0501,  //  001F  GETMET	R3	R2	K1
      0x5C140200,  //  0020  MOVE	R5	R1
      0x58180005,  //  0021  LDCONST	R6	K5
      0x7C0C0600,  //  0022  CALL	R3	3
      0x780E000A,  //  0023  JMPF	R3	#002F
      0x600C0009,  //  0024  GETGBL	R3	G9
      0x6010000A,  //  0025  GETGBL	R4	G10
      0x5415FFFD,  //  0026  LDINT	R5	-2
      0x40160605,  //  0027  CONNECT	R5	K3	R5
      0x94140205,  //  0028  GETIDX	R5	R1	R5
      0x7C100200,  //  0029  CALL	R4	1
      0x5416EA5F,  //  002A  LDINT	R5	60000
      0x08100805,  //  002B  MUL	R4	R4	R5
      0x7C0C0200,  //  002C  CALL	R3	1
      0x80040600,  //  002D  RET	1	R3
      0x7002000D,  //  002E  JMP		#003D
      0x8C0C0501,  //  002F  GETMET	R3	R2	K1
      0x5C140200,  //  0030  MOVE	R5	R1
      0x58180006,  //  0031  LDCONST	R6	K6
      0x7C0C0600,  //  0032  CALL	R3	3
      0x780E0008,  //  0033  JMPF	R3	#003D
      0x600C0009,  //  0034  GETGBL	R3	G9
      0x6010000A,  //  0035  GETGBL	R4	G10
      0x5415FFFD,  //  0036  LDINT	R5	-2
      0x40160605,  //  0037  CONNECT	R5	K3	R5
      0x94140205,  //  0038  GETIDX	R5	R1	R5
      0x7C100200,  //  0039  CALL	R4	1
      0x08100907,  //  003A  MUL	R4	R4	K7
      0x7C0C0200,  //  003B  CALL	R3	1
      0x80040600,  //  003C  RET	1	R3
      0x540E03E7,  //  003D  LDINT	R3	1000
      0x80040600,  //  003E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _process_named_arguments_for_color_provider
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler__process_named_arguments_for_color_provider,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_process_named_arguments_unified),
    /* K1   */  be_nested_str_weak(color_provider),
    }),
    be_str_weak(_process_named_arguments_for_color_provider),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x5C140200,  //  0001  MOVE	R5	R1
      0x5C180400,  //  0002  MOVE	R6	R2
      0x581C0001,  //  0003  LDCONST	R7	K1
      0x7C0C0800,  //  0004  CALL	R3	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: current
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_current,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(pos),
    /* K1   */  be_nested_str_weak(tokens),
    }),
    be_str_weak(current),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14040202,  //  0004  LT	R1	R1	R2
      0x78060003,  //  0005  JMPF	R1	#000A
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x94040202,  //  0008  GETIDX	R1	R1	R2
      0x70020000,  //  0009  JMP		#000B
      0x4C040000,  //  000A  LDNIL	R1
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_math_method
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_is_math_method,   /* name */
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
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(animation),
    /* K3   */  be_nested_str_weak(_math),
    }),
    be_str_weak(is_math_method),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0xA8020009,  //  0000  EXBLK	0	#000B
      0xA40A0000,  //  0001  IMPORT	R2	K0
      0x8C0C0501,  //  0002  GETMET	R3	R2	K1
      0xB8160400,  //  0003  GETNGBL	R5	K2
      0x88140B03,  //  0004  GETMBR	R5	R5	K3
      0x5C180200,  //  0005  MOVE	R6	R1
      0x7C0C0600,  //  0006  CALL	R3	3
      0xA8040001,  //  0007  EXBLK	1	1
      0x80040600,  //  0008  RET	1	R3
      0xA8040001,  //  0009  EXBLK	1	1
      0x70020005,  //  000A  JMP		#0011
      0xAC080002,  //  000B  CATCH	R2	0	2
      0x70020002,  //  000C  JMP		#0010
      0x50100000,  //  000D  LDBOOL	R4	0	0
      0x80040800,  //  000E  RET	1	R4
      0x70020000,  //  000F  JMP		#0011
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_comma
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_comma,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(COMMA),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X2C_X27),
    }),
    be_str_weak(expect_comma),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expect_left_bracket
********************************************************************/
be_local_closure(class_SimpleDSLTranspiler_expect_left_bracket,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(current),
    /* K1   */  be_nested_str_weak(type),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(Token),
    /* K4   */  be_nested_str_weak(LEFT_BRACKET),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(error),
    /* K7   */  be_nested_str_weak(Expected_X20_X27_X5B_X27),
    }),
    be_str_weak(expect_left_bracket),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A0008,  //  0004  JMPF	R2	#000E
      0x88080301,  //  0005  GETMBR	R2	R1	K1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x880C0703,  //  0007  GETMBR	R3	R3	K3
      0x880C0704,  //  0008  GETMBR	R3	R3	K4
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x8C080105,  //  000B  GETMET	R2	R0	K5
      0x7C080200,  //  000C  CALL	R2	1
      0x70020002,  //  000D  JMP		#0011
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x58100007,  //  000F  LDCONST	R4	K7
      0x7C080400,  //  0010  CALL	R2	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: SimpleDSLTranspiler
********************************************************************/
be_local_class(SimpleDSLTranspiler,
    12,
    NULL,
    be_nested_map(122,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(expect_left_bracket, -1), be_const_closure(class_SimpleDSLTranspiler_expect_left_bracket_closure) },
        { be_const_key_weak(expect_left_brace, -1), be_const_closure(class_SimpleDSLTranspiler_expect_left_brace_closure) },
        { be_const_key_weak(symbol_exists, -1), be_const_closure(class_SimpleDSLTranspiler_symbol_exists_closure) },
        { be_const_key_weak(is_math_method, 81), be_const_closure(class_SimpleDSLTranspiler_is_math_method_closure) },
        { be_const_key_weak(at_end, 85), be_const_closure(class_SimpleDSLTranspiler_at_end_closure) },
        { be_const_key_weak(process_wait_statement_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_wait_statement_fluent_closure) },
        { be_const_key_weak(is_computed_expression, -1), be_const_closure(class_SimpleDSLTranspiler_is_computed_expression_closure) },
        { be_const_key_weak(_process_user_function_call, -1), be_const_closure(class_SimpleDSLTranspiler__process_user_function_call_closure) },
        { be_const_key_weak(expect_identifier, 121), be_const_closure(class_SimpleDSLTranspiler_expect_identifier_closure) },
        { be_const_key_weak(_process_named_arguments_for_color_provider, -1), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_for_color_provider_closure) },
        { be_const_key_weak(process_color, -1), be_const_closure(class_SimpleDSLTranspiler_process_color_closure) },
        { be_const_key_weak(process_standalone_log, -1), be_const_closure(class_SimpleDSLTranspiler_process_standalone_log_closure) },
        { be_const_key_weak(resolve_symbol_reference, 57), be_const_closure(class_SimpleDSLTranspiler_resolve_symbol_reference_closure) },
        { be_const_key_weak(named_colors, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(37,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(brown, -1), be_nested_str_weak(0xFFA52A2A) },
        { be_const_key_weak(silver, -1), be_nested_str_weak(0xFFC0C0C0) },
        { be_const_key_weak(salmon, -1), be_nested_str_weak(0xFFFA8072) },
        { be_const_key_weak(transparent, -1), be_nested_str_weak(0x00000000) },
        { be_const_key_weak(lime, 30), be_nested_str_weak(0xFF00FF00) },
        { be_const_key_weak(coral, 18), be_nested_str_weak(0xFFFF7F50) },
        { be_const_key_weak(cyan, -1), be_nested_str_weak(0xFF00FFFF) },
        { be_const_key_weak(olive, -1), be_nested_str_weak(0xFF808000) },
        { be_const_key_weak(snow, 16), be_nested_str_weak(0xFFFFFAFA) },
        { be_const_key_weak(violet, -1), be_nested_str_weak(0xFFEE82EE) },
        { be_const_key_weak(green, 19), be_nested_str_weak(0xFF008000) },
        { be_const_key_weak(turquoise, -1), be_nested_str_weak(0xFF40E0D0) },
        { be_const_key_weak(grey, -1), be_nested_str_weak(0xFF808080) },
        { be_const_key_weak(indigo, -1), be_nested_str_weak(0xFF4B0082) },
        { be_const_key_weak(gray, 20), be_nested_str_weak(0xFF808080) },
        { be_const_key_weak(white, 21), be_nested_str_weak(0xFFFFFFFF) },
        { be_const_key_weak(red, -1), be_nested_str_weak(0xFFFF0000) },
        { be_const_key_weak(orange, -1), be_nested_str_weak(0xFFFFA500) },
        { be_const_key_weak(gold, 35), be_nested_str_weak(0xFFFFD700) },
        { be_const_key_weak(beige, -1), be_nested_str_weak(0xFFF5F5DC) },
        { be_const_key_weak(black, 26), be_nested_str_weak(0xFF000000) },
        { be_const_key_weak(teal, 27), be_nested_str_weak(0xFF008080) },
        { be_const_key_weak(crimson, 12), be_nested_str_weak(0xFFDC143C) },
        { be_const_key_weak(fuchsia, -1), be_nested_str_weak(0xFFFF00FF) },
        { be_const_key_weak(magenta, 2), be_nested_str_weak(0xFFFF00FF) },
        { be_const_key_weak(yellow, 15), be_nested_str_weak(0xFFFFFF00) },
        { be_const_key_weak(navy, -1), be_nested_str_weak(0xFF000080) },
        { be_const_key_weak(khaki, -1), be_nested_str_weak(0xFFF0E68C) },
        { be_const_key_weak(pink, -1), be_nested_str_weak(0xFFFFC0CB) },
        { be_const_key_weak(ivory, -1), be_nested_str_weak(0xFFFFFFF0) },
        { be_const_key_weak(purple, -1), be_nested_str_weak(0xFF800080) },
        { be_const_key_weak(aqua, -1), be_nested_str_weak(0xFF00FFFF) },
        { be_const_key_weak(blue, -1), be_nested_str_weak(0xFF0000FF) },
        { be_const_key_weak(plum, 23), be_nested_str_weak(0xFFDDA0DD) },
        { be_const_key_weak(orchid, -1), be_nested_str_weak(0xFFDA70D6) },
        { be_const_key_weak(tan, -1), be_nested_str_weak(0xFFD2B48C) },
        { be_const_key_weak(maroon, -1), be_nested_str_weak(0xFF800000) },
    }))    ) } )) },
        { be_const_key_weak(output, -1), be_const_var(2) },
        { be_const_key_weak(convert_time_to_ms, -1), be_const_closure(class_SimpleDSLTranspiler_convert_time_to_ms_closure) },
        { be_const_key_weak(process_function_arguments, -1), be_const_closure(class_SimpleDSLTranspiler_process_function_arguments_closure) },
        { be_const_key_weak(process_percentage_value, -1), be_const_closure(class_SimpleDSLTranspiler_process_percentage_value_closure) },
        { be_const_key_weak(next, -1), be_const_closure(class_SimpleDSLTranspiler_next_closure) },
        { be_const_key_weak(pos, -1), be_const_var(1) },
        { be_const_key_weak(generate_default_strip_initialization, 62), be_const_closure(class_SimpleDSLTranspiler_generate_default_strip_initialization_closure) },
        { be_const_key_weak(sequence_names, 13), be_const_var(7) },
        { be_const_key_weak(process_named_arguments_for_variable, 60), be_const_closure(class_SimpleDSLTranspiler_process_named_arguments_for_variable_closure) },
        { be_const_key_weak(first_statement, 7), be_const_var(5) },
        { be_const_key_weak(indent_level, -1), be_const_var(9) },
        { be_const_key_weak(get_errors, 73), be_const_closure(class_SimpleDSLTranspiler_get_errors_closure) },
        { be_const_key_weak(process_nested_function_call, -1), be_const_closure(class_SimpleDSLTranspiler_process_nested_function_call_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_SimpleDSLTranspiler_init_closure) },
        { be_const_key_weak(process_import, 119), be_const_closure(class_SimpleDSLTranspiler_process_import_closure) },
        { be_const_key_weak(expect_left_paren, -1), be_const_closure(class_SimpleDSLTranspiler_expect_left_paren_closure) },
        { be_const_key_weak(is_computed_expression_string, -1), be_const_closure(class_SimpleDSLTranspiler_is_computed_expression_string_closure) },
        { be_const_key_weak(collect_inline_comment, -1), be_const_closure(class_SimpleDSLTranspiler_collect_inline_comment_closure) },
        { be_const_key_weak(has_template_calls, 20), be_const_var(11) },
        { be_const_key_weak(_create_instance_for_validation, 89), be_const_closure(class_SimpleDSLTranspiler__create_instance_for_validation_closure) },
        { be_const_key_weak(process_sequence, 95), be_const_closure(class_SimpleDSLTranspiler_process_sequence_closure) },
        { be_const_key_weak(errors, -1), be_const_var(3) },
        { be_const_key_weak(error, -1), be_const_closure(class_SimpleDSLTranspiler_error_closure) },
        { be_const_key_weak(transpile_template_body, 23), be_const_closure(class_SimpleDSLTranspiler_transpile_template_body_closure) },
        { be_const_key_weak(process_sequence_assignment_generic, -1), be_const_closure(class_SimpleDSLTranspiler_process_sequence_assignment_generic_closure) },
        { be_const_key_weak(_validate_animation_factory_exists, 0), be_const_closure(class_SimpleDSLTranspiler__validate_animation_factory_exists_closure) },
        { be_const_key_weak(process_additive_expression, 93), be_const_closure(class_SimpleDSLTranspiler_process_additive_expression_closure) },
        { be_const_key_weak(strip_initialized, 80), be_const_var(6) },
        { be_const_key_weak(validate_symbol_reference, -1), be_const_closure(class_SimpleDSLTranspiler_validate_symbol_reference_closure) },
        { be_const_key_weak(generate_engine_run, -1), be_const_closure(class_SimpleDSLTranspiler_generate_engine_run_closure) },
        { be_const_key_weak(skip_whitespace, -1), be_const_closure(class_SimpleDSLTranspiler_skip_whitespace_closure) },
        { be_const_key_weak(check_right_bracket, -1), be_const_closure(class_SimpleDSLTranspiler_check_right_bracket_closure) },
        { be_const_key_weak(template_definitions, -1), be_const_var(10) },
        { be_const_key_weak(process_template, -1), be_const_closure(class_SimpleDSLTranspiler_process_template_closure) },
        { be_const_key_weak(skip_statement, -1), be_const_closure(class_SimpleDSLTranspiler_skip_statement_closure) },
        { be_const_key_weak(_validate_single_parameter, -1), be_const_closure(class_SimpleDSLTranspiler__validate_single_parameter_closure) },
        { be_const_key_weak(check_right_brace, 3), be_const_closure(class_SimpleDSLTranspiler_check_right_brace_closure) },
        { be_const_key_weak(process_log_call, 91), be_const_closure(class_SimpleDSLTranspiler_process_log_call_closure) },
        { be_const_key_weak(process_event_parameters, 35), be_const_closure(class_SimpleDSLTranspiler_process_event_parameters_closure) },
        { be_const_key_weak(process_animation, -1), be_const_closure(class_SimpleDSLTranspiler_process_animation_closure) },
        { be_const_key_weak(process_property_assignment, -1), be_const_closure(class_SimpleDSLTranspiler_process_property_assignment_closure) },
        { be_const_key_weak(validate_user_name, -1), be_const_closure(class_SimpleDSLTranspiler_validate_user_name_closure) },
        { be_const_key_weak(expect_right_paren, -1), be_const_closure(class_SimpleDSLTranspiler_expect_right_paren_closure) },
        { be_const_key_weak(is_identifier_char, 1), be_const_closure(class_SimpleDSLTranspiler_is_identifier_char_closure) },
        { be_const_key_weak(process_sequence_assignment, 111), be_const_closure(class_SimpleDSLTranspiler_process_sequence_assignment_closure) },
        { be_const_key_weak(expect_right_brace, 112), be_const_closure(class_SimpleDSLTranspiler_expect_right_brace_closure) },
        { be_const_key_weak(generate_template_function, -1), be_const_closure(class_SimpleDSLTranspiler_generate_template_function_closure) },
        { be_const_key_weak(get_error_report, 36), be_const_closure(class_SimpleDSLTranspiler_get_error_report_closure) },
        { be_const_key_weak(convert_to_vrgb, -1), be_const_closure(class_SimpleDSLTranspiler_convert_to_vrgb_closure) },
        { be_const_key_weak(join_output, -1), be_const_closure(class_SimpleDSLTranspiler_join_output_closure) },
        { be_const_key_weak(symbol_table, 52), be_const_var(8) },
        { be_const_key_weak(process_set, -1), be_const_closure(class_SimpleDSLTranspiler_process_set_closure) },
        { be_const_key_weak(expect_colon, 38), be_const_closure(class_SimpleDSLTranspiler_expect_colon_closure) },
        { be_const_key_weak(_generate_anonymous_function_call, -1), be_const_closure(class_SimpleDSLTranspiler__generate_anonymous_function_call_closure) },
        { be_const_key_weak(create_computation_closure_from_string, 9), be_const_closure(class_SimpleDSLTranspiler_create_computation_closure_from_string_closure) },
        { be_const_key_weak(_validate_animation_factory_creates_animation, 103), be_const_closure(class_SimpleDSLTranspiler__validate_animation_factory_creates_animation_closure) },
        { be_const_key_weak(process_primary_expression, 74), be_const_closure(class_SimpleDSLTranspiler_process_primary_expression_closure) },
        { be_const_key_weak(create_simple_function_from_string, -1), be_const_closure(class_SimpleDSLTranspiler_create_simple_function_from_string_closure) },
        { be_const_key_weak(process_time_value, -1), be_const_closure(class_SimpleDSLTranspiler_process_time_value_closure) },
        { be_const_key_weak(check_right_paren, -1), be_const_closure(class_SimpleDSLTranspiler_check_right_paren_closure) },
        { be_const_key_weak(transform_expression_for_closure, -1), be_const_closure(class_SimpleDSLTranspiler_transform_expression_for_closure_closure) },
        { be_const_key_weak(convert_color, -1), be_const_closure(class_SimpleDSLTranspiler_convert_color_closure) },
        { be_const_key_weak(expect_number, 97), be_const_closure(class_SimpleDSLTranspiler_expect_number_closure) },
        { be_const_key_weak(run_statements, -1), be_const_var(4) },
        { be_const_key_weak(expect_assign, 92), be_const_closure(class_SimpleDSLTranspiler_expect_assign_closure) },
        { be_const_key_weak(tokens, -1), be_const_var(0) },
        { be_const_key_weak(process_value, -1), be_const_closure(class_SimpleDSLTranspiler_process_value_closure) },
        { be_const_key_weak(process_log_statement_fluent, 116), be_const_closure(class_SimpleDSLTranspiler_process_log_statement_fluent_closure) },
        { be_const_key_weak(add, 78), be_const_closure(class_SimpleDSLTranspiler_add_closure) },
        { be_const_key_weak(_process_named_arguments_for_animation, -1), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_for_animation_closure) },
        { be_const_key_weak(_validate_value_provider_factory_exists, -1), be_const_closure(class_SimpleDSLTranspiler__validate_value_provider_factory_exists_closure) },
        { be_const_key_weak(process_palette, -1), be_const_closure(class_SimpleDSLTranspiler_process_palette_closure) },
        { be_const_key_weak(can_use_as_identifier, 72), be_const_closure(class_SimpleDSLTranspiler_can_use_as_identifier_closure) },
        { be_const_key_weak(_process_named_arguments_unified, -1), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_unified_closure) },
        { be_const_key_weak(process_play_statement_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_play_statement_fluent_closure) },
        { be_const_key_weak(_validate_object_reference, -1), be_const_closure(class_SimpleDSLTranspiler__validate_object_reference_closure) },
        { be_const_key_weak(is_anonymous_function, 84), be_const_closure(class_SimpleDSLTranspiler_is_anonymous_function_closure) },
        { be_const_key_weak(get_indent, -1), be_const_closure(class_SimpleDSLTranspiler_get_indent_closure) },
        { be_const_key_weak(_process_parameters_core, 18), be_const_closure(class_SimpleDSLTranspiler__process_parameters_core_closure) },
        { be_const_key_weak(_process_named_arguments_generic, 117), be_const_closure(class_SimpleDSLTranspiler__process_named_arguments_generic_closure) },
        { be_const_key_weak(skip_whitespace_including_newlines, 75), be_const_closure(class_SimpleDSLTranspiler_skip_whitespace_including_newlines_closure) },
        { be_const_key_weak(skip_function_arguments, -1), be_const_closure(class_SimpleDSLTranspiler_skip_function_arguments_closure) },
        { be_const_key_weak(process_palette_color, -1), be_const_closure(class_SimpleDSLTranspiler_process_palette_color_closure) },
        { be_const_key_weak(_validate_color_provider_factory_exists, -1), be_const_closure(class_SimpleDSLTranspiler__validate_color_provider_factory_exists_closure) },
        { be_const_key_weak(expect_keyword, 71), be_const_closure(class_SimpleDSLTranspiler_expect_keyword_closure) },
        { be_const_key_weak(process_sequence_assignment_fluent, -1), be_const_closure(class_SimpleDSLTranspiler_process_sequence_assignment_fluent_closure) },
        { be_const_key_weak(peek, -1), be_const_closure(class_SimpleDSLTranspiler_peek_closure) },
        { be_const_key_weak(create_computation_closure, -1), be_const_closure(class_SimpleDSLTranspiler_create_computation_closure_closure) },
        { be_const_key_weak(process_function_call, -1), be_const_closure(class_SimpleDSLTranspiler_process_function_call_closure) },
        { be_const_key_weak(process_unary_expression, 65), be_const_closure(class_SimpleDSLTranspiler_process_unary_expression_closure) },
        { be_const_key_weak(expect_dot, 51), be_const_closure(class_SimpleDSLTranspiler_expect_dot_closure) },
        { be_const_key_weak(get_named_color_value, 49), be_const_closure(class_SimpleDSLTranspiler_get_named_color_value_closure) },
        { be_const_key_weak(expect_right_bracket, 41), be_const_closure(class_SimpleDSLTranspiler_expect_right_bracket_closure) },
        { be_const_key_weak(transform_operand_for_closure, -1), be_const_closure(class_SimpleDSLTranspiler_transform_operand_for_closure_closure) },
        { be_const_key_weak(process_run, -1), be_const_closure(class_SimpleDSLTranspiler_process_run_closure) },
        { be_const_key_weak(has_errors, -1), be_const_closure(class_SimpleDSLTranspiler_has_errors_closure) },
        { be_const_key_weak(process_sequence_statement, 33), be_const_closure(class_SimpleDSLTranspiler_process_sequence_statement_closure) },
        { be_const_key_weak(process_statement, -1), be_const_closure(class_SimpleDSLTranspiler_process_statement_closure) },
        { be_const_key_weak(_is_simple_function_call, -1), be_const_closure(class_SimpleDSLTranspiler__is_simple_function_call_closure) },
        { be_const_key_weak(transpile, 19), be_const_closure(class_SimpleDSLTranspiler_transpile_closure) },
        { be_const_key_weak(process_array_literal, -1), be_const_closure(class_SimpleDSLTranspiler_process_array_literal_closure) },
        { be_const_key_weak(process_restart_statement_fluent, 15), be_const_closure(class_SimpleDSLTranspiler_process_restart_statement_fluent_closure) },
        { be_const_key_weak(_validate_factory_function, -1), be_const_closure(class_SimpleDSLTranspiler__validate_factory_function_closure) },
        { be_const_key_weak(process_event_handler, -1), be_const_closure(class_SimpleDSLTranspiler_process_event_handler_closure) },
        { be_const_key_weak(current, -1), be_const_closure(class_SimpleDSLTranspiler_current_closure) },
        { be_const_key_weak(_validate_value_provider_reference, -1), be_const_closure(class_SimpleDSLTranspiler__validate_value_provider_reference_closure) },
        { be_const_key_weak(expect_comma, -1), be_const_closure(class_SimpleDSLTranspiler_expect_comma_closure) },
        { be_const_key_weak(process_multiplicative_expression, -1), be_const_closure(class_SimpleDSLTranspiler_process_multiplicative_expression_closure) },
    })),
    be_str_weak(SimpleDSLTranspiler)
);

/********************************************************************
** Solidified function: tokenize_dsl
********************************************************************/
be_local_closure(tokenize_dsl,   /* name */
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
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(DSLLexer),
    /* K2   */  be_nested_str_weak(tokenize),
    }),
    be_str_weak(tokenize_dsl),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x8C080302,  //  0004  GETMET	R2	R1	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_color_name
********************************************************************/
be_local_closure(is_color_name,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    /* K2   */  be_nested_str_weak(color_names),
    /* K3   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(is_color_name),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0xB80A0000,  //  0001  GETNGBL	R2	K0
      0x88080501,  //  0002  GETMBR	R2	R2	K1
      0x88080502,  //  0003  GETMBR	R2	R2	K2
      0x7C040200,  //  0004  CALL	R1	1
      0xA8020007,  //  0005  EXBLK	0	#000E
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x1C0C0002,  //  0008  EQ	R3	R0	R2
      0x780E0002,  //  0009  JMPF	R3	#000D
      0x500C0200,  //  000A  LDBOOL	R3	1	0
      0xA8040001,  //  000B  EXBLK	1	1
      0x80040600,  //  000C  RET	1	R3
      0x7001FFF7,  //  000D  JMP		#0006
      0x58040003,  //  000E  LDCONST	R1	K3
      0xAC040200,  //  000F  CATCH	R1	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x50040000,  //  0011  LDBOOL	R1	0	0
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_error_token
********************************************************************/
be_local_closure(create_error_token,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(Token),
    }),
    be_str_weak(create_error_token),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x54160026,  //  0002  LDINT	R5	39
      0x5C180000,  //  0003  MOVE	R6	R0
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x6024000C,  //  0006  GETGBL	R9	G12
      0x5C280000,  //  0007  MOVE	R10	R0
      0x7C240200,  //  0008  CALL	R9	1
      0x7C0C0C00,  //  0009  CALL	R3	6
      0x80040600,  //  000A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: create_dsl_runtime
********************************************************************/
be_local_closure(create_dsl_runtime,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation),
    /* K1   */  be_nested_str_weak(create_engine),
    /* K2   */  be_nested_str_weak(animation_dsl),
    /* K3   */  be_nested_str_weak(DSLRuntime),
    }),
    be_str_weak(create_dsl_runtime),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100000,  //  0002  MOVE	R4	R0
      0x7C080400,  //  0003  CALL	R2	2
      0xB80E0400,  //  0004  GETNGBL	R3	K2
      0x8C0C0703,  //  0005  GETMET	R3	R3	K3
      0x5C140400,  //  0006  MOVE	R5	R2
      0x5C180200,  //  0007  MOVE	R6	R1
      0x7C0C0600,  //  0008  CALL	R3	3
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_file
********************************************************************/
be_local_closure(load_file,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(r),
    /* K1   */  be_nested_str_weak(Cannot_X20open_X20DSL_X20file_X3A_X20_X25s),
    /* K2   */  be_nested_str_weak(io_error),
    /* K3   */  be_nested_str_weak(read),
    /* K4   */  be_nested_str_weak(close),
    /* K5   */  be_nested_str_weak(animation_dsl),
    /* K6   */  be_nested_str_weak(execute),
    }),
    be_str_weak(load_file),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60040011,  //  0000  GETGBL	R1	G17
      0x5C080000,  //  0001  MOVE	R2	R0
      0x580C0000,  //  0002  LDCONST	R3	K0
      0x7C040400,  //  0003  CALL	R1	2
      0x4C080000,  //  0004  LDNIL	R2
      0x1C080202,  //  0005  EQ	R2	R1	R2
      0x780A0004,  //  0006  JMPF	R2	#000C
      0x60080018,  //  0007  GETGBL	R2	G24
      0x580C0001,  //  0008  LDCONST	R3	K1
      0x5C100000,  //  0009  MOVE	R4	R0
      0x7C080400,  //  000A  CALL	R2	2
      0xB0060402,  //  000B  RAISE	1	K2	R2
      0x8C080303,  //  000C  GETMET	R2	R1	K3
      0x7C080200,  //  000D  CALL	R2	1
      0x8C0C0304,  //  000E  GETMET	R3	R1	K4
      0x7C0C0200,  //  000F  CALL	R3	1
      0xB80E0A00,  //  0010  GETNGBL	R3	K5
      0x8C0C0706,  //  0011  GETMET	R3	R3	K6
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x80040600,  //  0014  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tokenize_dsl_with_errors
********************************************************************/
be_local_closure(tokenize_dsl_with_errors,   /* name */
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
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(DSLLexer),
    /* K2   */  be_nested_str_weak(tokenize_with_errors),
    }),
    be_str_weak(tokenize_dsl_with_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x8C080302,  //  0004  GETMET	R2	R1	K2
      0x7C080200,  //  0005  CALL	R2	1
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/

// compact class 'DSLLexer' ktab size: 113, total: 273 (saved 1280 bytes)
static const bvalue be_ktab_class_DSLLexer[113] = {
  /* K0   */  be_nested_str_weak(errors),
  /* K1   */  be_nested_str_weak(has_errors),
  /* K2   */  be_nested_str_weak(No_X20lexical_X20errors),
  /* K3   */  be_nested_str_weak(Lexical_X20errors_X20_X28),
  /* K4   */  be_nested_str_weak(_X29_X3A_X0A),
  /* K5   */  be_nested_str_weak(_X20_X20Line_X20),
  /* K6   */  be_nested_str_weak(line),
  /* K7   */  be_nested_str_weak(_X3A),
  /* K8   */  be_nested_str_weak(column),
  /* K9   */  be_nested_str_weak(_X3A_X20),
  /* K10  */  be_nested_str_weak(message),
  /* K11  */  be_nested_str_weak(_X0A),
  /* K12  */  be_nested_str_weak(stop_iteration),
  /* K13  */  be_nested_str_weak(position),
  /* K14  */  be_const_int(1),
  /* K15  */  be_nested_str_weak(source),
  /* K16  */  be_nested_str_weak(),
  /* K17  */  be_nested_str_weak(at_end),
  /* K18  */  be_nested_str_weak(is_digit),
  /* K19  */  be_nested_str_weak(peek),
  /* K20  */  be_nested_str_weak(advance),
  /* K21  */  be_nested_str_weak(_X2E),
  /* K22  */  be_nested_str_weak(check_time_suffix),
  /* K23  */  be_nested_str_weak(scan_time_suffix),
  /* K24  */  be_nested_str_weak(add_token),
  /* K25  */  be_nested_str_weak(_X25),
  /* K26  */  be_nested_str_weak(x),
  /* K27  */  be_const_int(2),
  /* K28  */  be_nested_str_weak(_X5C),
  /* K29  */  be_nested_str_weak(n),
  /* K30  */  be_nested_str_weak(t),
  /* K31  */  be_nested_str_weak(_X09),
  /* K32  */  be_nested_str_weak(r),
  /* K33  */  be_nested_str_weak(_X0D),
  /* K34  */  be_nested_str_weak(add_error),
  /* K35  */  be_nested_str_weak(Unterminated_X20string_X20literal),
  /* K36  */  be_const_int(3),
  /* K37  */  be_const_int(0),
  /* K38  */  be_nested_str_weak(is_hex_digit),
  /* K39  */  be_nested_str_weak(Invalid_X20hex_X20color_X20format_X3A_X20),
  /* K40  */  be_nested_str_weak(_X20_X28expected_X200xRRGGBB_X20or_X200xAARRGGBB_X29),
  /* K41  */  be_nested_str_weak(animation_dsl),
  /* K42  */  be_nested_str_weak(Token),
  /* K43  */  be_nested_str_weak(tokens),
  /* K44  */  be_nested_str_weak(push),
  /* K45  */  be_nested_str_weak(string),
  /* K46  */  be_nested_str_weak(startswith),
  /* K47  */  be_const_int(2147483647),
  /* K48  */  be_nested_str_weak(ms),
  /* K49  */  be_nested_str_weak(s),
  /* K50  */  be_nested_str_weak(m),
  /* K51  */  be_nested_str_weak(h),
  /* K52  */  be_nested_str_weak(is_alpha),
  /* K53  */  be_nested_str_weak(_),
  /* K54  */  be_nested_str_weak(Invalid_X20variable_X20reference_X3A_X20_X24_X20must_X20be_X20followed_X20by_X20identifier),
  /* K55  */  be_nested_str_weak(_X24),
  /* K56  */  be_nested_str_weak(is_alnum),
  /* K57  */  be_nested_str_weak(0),
  /* K58  */  be_nested_str_weak(9),
  /* K59  */  be_nested_str_weak(a),
  /* K60  */  be_nested_str_weak(z),
  /* K61  */  be_nested_str_weak(A),
  /* K62  */  be_nested_str_weak(Z),
  /* K63  */  be_nested_str_weak(is_color_name),
  /* K64  */  be_nested_str_weak(is_keyword),
  /* K65  */  be_nested_str_weak(f),
  /* K66  */  be_nested_str_weak(F),
  /* K67  */  be_nested_str_weak(_X3D),
  /* K68  */  be_nested_str_weak(match),
  /* K69  */  be_nested_str_weak(_X3D_X3D),
  /* K70  */  be_nested_str_weak(_X21),
  /* K71  */  be_nested_str_weak(_X21_X3D),
  /* K72  */  be_nested_str_weak(_X3C),
  /* K73  */  be_nested_str_weak(_X3C_X3D),
  /* K74  */  be_nested_str_weak(_X3C_X3C),
  /* K75  */  be_nested_str_weak(_X3E),
  /* K76  */  be_nested_str_weak(_X3E_X3D),
  /* K77  */  be_nested_str_weak(_X3E_X3E),
  /* K78  */  be_nested_str_weak(_X26),
  /* K79  */  be_nested_str_weak(_X26_X26),
  /* K80  */  be_nested_str_weak(Single_X20_X27_X26_X27_X20not_X20supported_X20in_X20DSL),
  /* K81  */  be_nested_str_weak(_X7C),
  /* K82  */  be_nested_str_weak(_X7C_X7C),
  /* K83  */  be_nested_str_weak(Single_X20_X27_X7C_X27_X20not_X20supported_X20in_X20DSL),
  /* K84  */  be_nested_str_weak(_X2D),
  /* K85  */  be_nested_str_weak(_X2D_X3E),
  /* K86  */  be_nested_str_weak(_X2B),
  /* K87  */  be_nested_str_weak(_X2A),
  /* K88  */  be_nested_str_weak(_X2F),
  /* K89  */  be_nested_str_weak(_X5E),
  /* K90  */  be_nested_str_weak(_X28),
  /* K91  */  be_nested_str_weak(_X29),
  /* K92  */  be_nested_str_weak(_X7B),
  /* K93  */  be_nested_str_weak(_X7D),
  /* K94  */  be_nested_str_weak(_X5B),
  /* K95  */  be_nested_str_weak(_X5D),
  /* K96  */  be_nested_str_weak(_X2C),
  /* K97  */  be_nested_str_weak(_X3B),
  /* K98  */  be_nested_str_weak(Unexpected_X20character_X3A_X20_X27),
  /* K99  */  be_nested_str_weak(_X27),
  /* K100 */  be_nested_str_weak(scan_token),
  /* K101 */  be_nested_str_weak(_X20),
  /* K102 */  be_nested_str_weak(_X23),
  /* K103 */  be_nested_str_weak(scan_comment),
  /* K104 */  be_nested_str_weak(scan_hex_color_0x),
  /* K105 */  be_nested_str_weak(scan_identifier_or_keyword),
  /* K106 */  be_nested_str_weak(scan_number),
  /* K107 */  be_nested_str_weak(_X22),
  /* K108 */  be_nested_str_weak(scan_string),
  /* K109 */  be_nested_str_weak(scan_variable_reference),
  /* K110 */  be_nested_str_weak(scan_operator_or_delimiter),
  /* K111 */  be_nested_str_weak(tokenize),
  /* K112 */  be_nested_str_weak(success),
};


extern const bclass be_class_DSLLexer;

/********************************************************************
** Solidified function: get_errors
********************************************************************/
be_local_closure(class_DSLLexer_get_errors,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(get_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_error_report
********************************************************************/
be_local_closure(class_DSLLexer_get_error_report,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(get_error_report),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x8C040101,  //  0000  GETMET	R1	R0	K1
      0x7C040200,  //  0001  CALL	R1	1
      0x74060000,  //  0002  JMPT	R1	#0004
      0x80060400,  //  0003  RET	1	K2
      0x60040008,  //  0004  GETGBL	R1	G8
      0x6008000C,  //  0005  GETGBL	R2	G12
      0x880C0100,  //  0006  GETMBR	R3	R0	K0
      0x7C080200,  //  0007  CALL	R2	1
      0x7C040200,  //  0008  CALL	R1	1
      0x00060601,  //  0009  ADD	R1	K3	R1
      0x00040304,  //  000A  ADD	R1	R1	K4
      0x60080010,  //  000B  GETGBL	R2	G16
      0x880C0100,  //  000C  GETMBR	R3	R0	K0
      0x7C080200,  //  000D  CALL	R2	1
      0xA8020010,  //  000E  EXBLK	0	#0020
      0x5C0C0400,  //  000F  MOVE	R3	R2
      0x7C0C0000,  //  0010  CALL	R3	0
      0x60100008,  //  0011  GETGBL	R4	G8
      0x94140706,  //  0012  GETIDX	R5	R3	K6
      0x7C100200,  //  0013  CALL	R4	1
      0x00120A04,  //  0014  ADD	R4	K5	R4
      0x00100907,  //  0015  ADD	R4	R4	K7
      0x60140008,  //  0016  GETGBL	R5	G8
      0x94180708,  //  0017  GETIDX	R6	R3	K8
      0x7C140200,  //  0018  CALL	R5	1
      0x00100805,  //  0019  ADD	R4	R4	R5
      0x00100909,  //  001A  ADD	R4	R4	K9
      0x9414070A,  //  001B  GETIDX	R5	R3	K10
      0x00100805,  //  001C  ADD	R4	R4	R5
      0x0010090B,  //  001D  ADD	R4	R4	K11
      0x00040204,  //  001E  ADD	R1	R1	R4
      0x7001FFEE,  //  001F  JMP		#000F
      0x5808000C,  //  0020  LDCONST	R2	K12
      0xAC080200,  //  0021  CATCH	R2	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x80040200,  //  0023  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek_next
********************************************************************/
be_local_closure(class_DSLLexer_peek_next,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(peek_next),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x0004030E,  //  0001  ADD	R1	R1	K14
      0x6008000C,  //  0002  GETGBL	R2	G12
      0x880C010F,  //  0003  GETMBR	R3	R0	K15
      0x7C080200,  //  0004  CALL	R2	1
      0x28040202,  //  0005  GE	R1	R1	R2
      0x78060000,  //  0006  JMPF	R1	#0008
      0x80062000,  //  0007  RET	1	K16
      0x8804010D,  //  0008  GETMBR	R1	R0	K13
      0x0004030E,  //  0009  ADD	R1	R1	K14
      0x8808010F,  //  000A  GETMBR	R2	R0	K15
      0x94040401,  //  000B  GETIDX	R1	R2	R1
      0x80040200,  //  000C  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: advance
********************************************************************/
be_local_closure(class_DSLLexer_advance,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(advance),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040111,  //  0000  GETMET	R1	R0	K17
      0x7C040200,  //  0001  CALL	R1	1
      0x78060000,  //  0002  JMPF	R1	#0004
      0x80062000,  //  0003  RET	1	K16
      0x8804010F,  //  0004  GETMBR	R1	R0	K15
      0x8808010D,  //  0005  GETMBR	R2	R0	K13
      0x94040202,  //  0006  GETIDX	R1	R1	R2
      0x8808010D,  //  0007  GETMBR	R2	R0	K13
      0x0008050E,  //  0008  ADD	R2	R2	K14
      0x90021A02,  //  0009  SETMBR	R0	K13	R2
      0x88080108,  //  000A  GETMBR	R2	R0	K8
      0x0008050E,  //  000B  ADD	R2	R2	K14
      0x90021002,  //  000C  SETMBR	R0	K8	R2
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_number
********************************************************************/
be_local_closure(class_DSLLexer_scan_number,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_number),
    &be_const_str_solidified,
    ( &(const binstruction[113]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x0404030E,  //  0001  SUB	R1	R1	K14
      0x88080108,  //  0002  GETMBR	R2	R0	K8
      0x0408050E,  //  0003  SUB	R2	R2	K14
      0x500C0000,  //  0004  LDBOOL	R3	0	0
      0x8C100111,  //  0005  GETMET	R4	R0	K17
      0x7C100200,  //  0006  CALL	R4	1
      0x74120007,  //  0007  JMPT	R4	#0010
      0x8C100112,  //  0008  GETMET	R4	R0	K18
      0x8C180113,  //  0009  GETMET	R6	R0	K19
      0x7C180200,  //  000A  CALL	R6	1
      0x7C100400,  //  000B  CALL	R4	2
      0x78120002,  //  000C  JMPF	R4	#0010
      0x8C100114,  //  000D  GETMET	R4	R0	K20
      0x7C100200,  //  000E  CALL	R4	1
      0x7001FFF4,  //  000F  JMP		#0005
      0x8C100111,  //  0010  GETMET	R4	R0	K17
      0x7C100200,  //  0011  CALL	R4	1
      0x7412001F,  //  0012  JMPT	R4	#0033
      0x8C100113,  //  0013  GETMET	R4	R0	K19
      0x7C100200,  //  0014  CALL	R4	1
      0x1C100915,  //  0015  EQ	R4	R4	K21
      0x7812001B,  //  0016  JMPF	R4	#0033
      0x8810010D,  //  0017  GETMBR	R4	R0	K13
      0x0010090E,  //  0018  ADD	R4	R4	K14
      0x6014000C,  //  0019  GETGBL	R5	G12
      0x8818010F,  //  001A  GETMBR	R6	R0	K15
      0x7C140200,  //  001B  CALL	R5	1
      0x14100805,  //  001C  LT	R4	R4	R5
      0x78120014,  //  001D  JMPF	R4	#0033
      0x8C100112,  //  001E  GETMET	R4	R0	K18
      0x8818010D,  //  001F  GETMBR	R6	R0	K13
      0x00180D0E,  //  0020  ADD	R6	R6	K14
      0x881C010F,  //  0021  GETMBR	R7	R0	K15
      0x94180E06,  //  0022  GETIDX	R6	R7	R6
      0x7C100400,  //  0023  CALL	R4	2
      0x7812000D,  //  0024  JMPF	R4	#0033
      0x500C0200,  //  0025  LDBOOL	R3	1	0
      0x8C100114,  //  0026  GETMET	R4	R0	K20
      0x7C100200,  //  0027  CALL	R4	1
      0x8C100111,  //  0028  GETMET	R4	R0	K17
      0x7C100200,  //  0029  CALL	R4	1
      0x74120007,  //  002A  JMPT	R4	#0033
      0x8C100112,  //  002B  GETMET	R4	R0	K18
      0x8C180113,  //  002C  GETMET	R6	R0	K19
      0x7C180200,  //  002D  CALL	R6	1
      0x7C100400,  //  002E  CALL	R4	2
      0x78120002,  //  002F  JMPF	R4	#0033
      0x8C100114,  //  0030  GETMET	R4	R0	K20
      0x7C100200,  //  0031  CALL	R4	1
      0x7001FFF4,  //  0032  JMP		#0028
      0x8810010D,  //  0033  GETMBR	R4	R0	K13
      0x0410090E,  //  0034  SUB	R4	R4	K14
      0x40100204,  //  0035  CONNECT	R4	R1	R4
      0x8814010F,  //  0036  GETMBR	R5	R0	K15
      0x94100A04,  //  0037  GETIDX	R4	R5	R4
      0x8C140116,  //  0038  GETMET	R5	R0	K22
      0x7C140200,  //  0039  CALL	R5	1
      0x78160009,  //  003A  JMPF	R5	#0045
      0x8C140117,  //  003B  GETMET	R5	R0	K23
      0x7C140200,  //  003C  CALL	R5	1
      0x8C180118,  //  003D  GETMET	R6	R0	K24
      0x54220004,  //  003E  LDINT	R8	5
      0x00240805,  //  003F  ADD	R9	R4	R5
      0x6028000C,  //  0040  GETGBL	R10	G12
      0x002C0805,  //  0041  ADD	R11	R4	R5
      0x7C280200,  //  0042  CALL	R10	1
      0x7C180800,  //  0043  CALL	R6	4
      0x7002002A,  //  0044  JMP		#0070
      0x8C140111,  //  0045  GETMET	R5	R0	K17
      0x7C140200,  //  0046  CALL	R5	1
      0x7416000E,  //  0047  JMPT	R5	#0057
      0x8C140113,  //  0048  GETMET	R5	R0	K19
      0x7C140200,  //  0049  CALL	R5	1
      0x1C140B19,  //  004A  EQ	R5	R5	K25
      0x7816000A,  //  004B  JMPF	R5	#0057
      0x8C140114,  //  004C  GETMET	R5	R0	K20
      0x7C140200,  //  004D  CALL	R5	1
      0x8C140118,  //  004E  GETMET	R5	R0	K24
      0x541E0005,  //  004F  LDINT	R7	6
      0x00200919,  //  0050  ADD	R8	R4	K25
      0x6024000C,  //  0051  GETGBL	R9	G12
      0x5C280800,  //  0052  MOVE	R10	R4
      0x7C240200,  //  0053  CALL	R9	1
      0x0024130E,  //  0054  ADD	R9	R9	K14
      0x7C140800,  //  0055  CALL	R5	4
      0x70020018,  //  0056  JMP		#0070
      0x8C140111,  //  0057  GETMET	R5	R0	K17
      0x7C140200,  //  0058  CALL	R5	1
      0x7416000E,  //  0059  JMPT	R5	#0069
      0x8C140113,  //  005A  GETMET	R5	R0	K19
      0x7C140200,  //  005B  CALL	R5	1
      0x1C140B1A,  //  005C  EQ	R5	R5	K26
      0x7816000A,  //  005D  JMPF	R5	#0069
      0x8C140114,  //  005E  GETMET	R5	R0	K20
      0x7C140200,  //  005F  CALL	R5	1
      0x8C140118,  //  0060  GETMET	R5	R0	K24
      0x541E0006,  //  0061  LDINT	R7	7
      0x0020091A,  //  0062  ADD	R8	R4	K26
      0x6024000C,  //  0063  GETGBL	R9	G12
      0x5C280800,  //  0064  MOVE	R10	R4
      0x7C240200,  //  0065  CALL	R9	1
      0x0024130E,  //  0066  ADD	R9	R9	K14
      0x7C140800,  //  0067  CALL	R5	4
      0x70020006,  //  0068  JMP		#0070
      0x8C140118,  //  0069  GETMET	R5	R0	K24
      0x581C001B,  //  006A  LDCONST	R7	K27
      0x5C200800,  //  006B  MOVE	R8	R4
      0x6024000C,  //  006C  GETGBL	R9	G12
      0x5C280800,  //  006D  MOVE	R10	R4
      0x7C240200,  //  006E  CALL	R9	1
      0x7C140800,  //  006F  CALL	R5	4
      0x80000000,  //  0070  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_string
********************************************************************/
be_local_closure(class_DSLLexer_scan_string,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_string),
    &be_const_str_solidified,
    ( &(const binstruction[78]) {  /* code */
      0x8808010D,  //  0000  GETMBR	R2	R0	K13
      0x0408050E,  //  0001  SUB	R2	R2	K14
      0x880C0108,  //  0002  GETMBR	R3	R0	K8
      0x040C070E,  //  0003  SUB	R3	R3	K14
      0x58100010,  //  0004  LDCONST	R4	K16
      0x8C140111,  //  0005  GETMET	R5	R0	K17
      0x7C140200,  //  0006  CALL	R5	1
      0x7416002F,  //  0007  JMPT	R5	#0038
      0x8C140113,  //  0008  GETMET	R5	R0	K19
      0x7C140200,  //  0009  CALL	R5	1
      0x20140A01,  //  000A  NE	R5	R5	R1
      0x7816002B,  //  000B  JMPF	R5	#0038
      0x8C140114,  //  000C  GETMET	R5	R0	K20
      0x7C140200,  //  000D  CALL	R5	1
      0x1C180B1C,  //  000E  EQ	R6	R5	K28
      0x781A001D,  //  000F  JMPF	R6	#002E
      0x8C180111,  //  0010  GETMET	R6	R0	K17
      0x7C180200,  //  0011  CALL	R6	1
      0x741A0018,  //  0012  JMPT	R6	#002C
      0x8C180114,  //  0013  GETMET	R6	R0	K20
      0x7C180200,  //  0014  CALL	R6	1
      0x1C1C0D1D,  //  0015  EQ	R7	R6	K29
      0x781E0001,  //  0016  JMPF	R7	#0019
      0x0010090B,  //  0017  ADD	R4	R4	K11
      0x70020011,  //  0018  JMP		#002B
      0x1C1C0D1E,  //  0019  EQ	R7	R6	K30
      0x781E0001,  //  001A  JMPF	R7	#001D
      0x0010091F,  //  001B  ADD	R4	R4	K31
      0x7002000D,  //  001C  JMP		#002B
      0x1C1C0D20,  //  001D  EQ	R7	R6	K32
      0x781E0001,  //  001E  JMPF	R7	#0021
      0x00100921,  //  001F  ADD	R4	R4	K33
      0x70020009,  //  0020  JMP		#002B
      0x1C1C0D1C,  //  0021  EQ	R7	R6	K28
      0x781E0001,  //  0022  JMPF	R7	#0025
      0x0010091C,  //  0023  ADD	R4	R4	K28
      0x70020005,  //  0024  JMP		#002B
      0x1C1C0C01,  //  0025  EQ	R7	R6	R1
      0x781E0001,  //  0026  JMPF	R7	#0029
      0x00100801,  //  0027  ADD	R4	R4	R1
      0x70020001,  //  0028  JMP		#002B
      0x0010091C,  //  0029  ADD	R4	R4	K28
      0x00100806,  //  002A  ADD	R4	R4	R6
      0x70020000,  //  002B  JMP		#002D
      0x0010091C,  //  002C  ADD	R4	R4	K28
      0x70020008,  //  002D  JMP		#0037
      0x1C180B0B,  //  002E  EQ	R6	R5	K11
      0x781A0005,  //  002F  JMPF	R6	#0036
      0x88180106,  //  0030  GETMBR	R6	R0	K6
      0x00180D0E,  //  0031  ADD	R6	R6	K14
      0x90020C06,  //  0032  SETMBR	R0	K6	R6
      0x9002110E,  //  0033  SETMBR	R0	K8	K14
      0x00100805,  //  0034  ADD	R4	R4	R5
      0x70020000,  //  0035  JMP		#0037
      0x00100805,  //  0036  ADD	R4	R4	R5
      0x7001FFCC,  //  0037  JMP		#0005
      0x8C140111,  //  0038  GETMET	R5	R0	K17
      0x7C140200,  //  0039  CALL	R5	1
      0x78160009,  //  003A  JMPF	R5	#0045
      0x8C140122,  //  003B  GETMET	R5	R0	K34
      0x581C0023,  //  003C  LDCONST	R7	K35
      0x7C140400,  //  003D  CALL	R5	2
      0x8C140118,  //  003E  GETMET	R5	R0	K24
      0x541E0026,  //  003F  LDINT	R7	39
      0x5C200800,  //  0040  MOVE	R8	R4
      0x8824010D,  //  0041  GETMBR	R9	R0	K13
      0x04241202,  //  0042  SUB	R9	R9	R2
      0x7C140800,  //  0043  CALL	R5	4
      0x70020007,  //  0044  JMP		#004D
      0x8C140114,  //  0045  GETMET	R5	R0	K20
      0x7C140200,  //  0046  CALL	R5	1
      0x8C140118,  //  0047  GETMET	R5	R0	K24
      0x581C0024,  //  0048  LDCONST	R7	K36
      0x5C200800,  //  0049  MOVE	R8	R4
      0x8824010D,  //  004A  GETMBR	R9	R0	K13
      0x04241202,  //  004B  SUB	R9	R9	R2
      0x7C140800,  //  004C  CALL	R5	4
      0x80000000,  //  004D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_hex_color_0x
********************************************************************/
be_local_closure(class_DSLLexer_scan_hex_color_0x,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_hex_color_0x),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x0404030E,  //  0001  SUB	R1	R1	K14
      0x88080108,  //  0002  GETMBR	R2	R0	K8
      0x0408050E,  //  0003  SUB	R2	R2	K14
      0x8C0C0114,  //  0004  GETMET	R3	R0	K20
      0x7C0C0200,  //  0005  CALL	R3	1
      0x580C0025,  //  0006  LDCONST	R3	K37
      0x8C100111,  //  0007  GETMET	R4	R0	K17
      0x7C100200,  //  0008  CALL	R4	1
      0x74120008,  //  0009  JMPT	R4	#0013
      0x8C100126,  //  000A  GETMET	R4	R0	K38
      0x8C180113,  //  000B  GETMET	R6	R0	K19
      0x7C180200,  //  000C  CALL	R6	1
      0x7C100400,  //  000D  CALL	R4	2
      0x78120003,  //  000E  JMPF	R4	#0013
      0x8C100114,  //  000F  GETMET	R4	R0	K20
      0x7C100200,  //  0010  CALL	R4	1
      0x000C070E,  //  0011  ADD	R3	R3	K14
      0x7001FFF3,  //  0012  JMP		#0007
      0x8810010D,  //  0013  GETMBR	R4	R0	K13
      0x0410090E,  //  0014  SUB	R4	R4	K14
      0x40100204,  //  0015  CONNECT	R4	R1	R4
      0x8814010F,  //  0016  GETMBR	R5	R0	K15
      0x94100A04,  //  0017  GETIDX	R4	R5	R4
      0x54160005,  //  0018  LDINT	R5	6
      0x1C140605,  //  0019  EQ	R5	R3	R5
      0x74160002,  //  001A  JMPT	R5	#001E
      0x54160007,  //  001B  LDINT	R5	8
      0x1C140605,  //  001C  EQ	R5	R3	R5
      0x78160007,  //  001D  JMPF	R5	#0026
      0x8C140118,  //  001E  GETMET	R5	R0	K24
      0x541E0003,  //  001F  LDINT	R7	4
      0x5C200800,  //  0020  MOVE	R8	R4
      0x6024000C,  //  0021  GETGBL	R9	G12
      0x5C280800,  //  0022  MOVE	R10	R4
      0x7C240200,  //  0023  CALL	R9	1
      0x7C140800,  //  0024  CALL	R5	4
      0x7002000A,  //  0025  JMP		#0031
      0x8C140122,  //  0026  GETMET	R5	R0	K34
      0x001E4E04,  //  0027  ADD	R7	K39	R4
      0x001C0F28,  //  0028  ADD	R7	R7	K40
      0x7C140400,  //  0029  CALL	R5	2
      0x8C140118,  //  002A  GETMET	R5	R0	K24
      0x541E0026,  //  002B  LDINT	R7	39
      0x5C200800,  //  002C  MOVE	R8	R4
      0x6024000C,  //  002D  GETGBL	R9	G12
      0x5C280800,  //  002E  MOVE	R10	R4
      0x7C240200,  //  002F  CALL	R9	1
      0x7C140800,  //  0030  CALL	R5	4
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_token
********************************************************************/
be_local_closure(class_DSLLexer_add_token,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(add_token),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xB8125200,  //  0000  GETNGBL	R4	K41
      0x8C10092A,  //  0001  GETMET	R4	R4	K42
      0x5C180200,  //  0002  MOVE	R6	R1
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x88200106,  //  0004  GETMBR	R8	R0	K6
      0x88240108,  //  0005  GETMBR	R9	R0	K8
      0x04241203,  //  0006  SUB	R9	R9	R3
      0x5C280600,  //  0007  MOVE	R10	R3
      0x7C100C00,  //  0008  CALL	R4	6
      0x8814012B,  //  0009  GETMBR	R5	R0	K43
      0x8C140B2C,  //  000A  GETMET	R5	R5	K44
      0x5C1C0800,  //  000B  MOVE	R7	R4
      0x7C140400,  //  000C  CALL	R5	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_time_suffix
********************************************************************/
be_local_closure(class_DSLLexer_scan_time_suffix,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_time_suffix),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0xA4065A00,  //  0000  IMPORT	R1	K45
      0x8C08032E,  //  0001  GETMET	R2	R1	K46
      0x8810010D,  //  0002  GETMBR	R4	R0	K13
      0x4010092F,  //  0003  CONNECT	R4	R4	K47
      0x8814010F,  //  0004  GETMBR	R5	R0	K15
      0x94100A04,  //  0005  GETIDX	R4	R5	R4
      0x58140030,  //  0006  LDCONST	R5	K48
      0x7C080600,  //  0007  CALL	R2	3
      0x780A0005,  //  0008  JMPF	R2	#000F
      0x8C080114,  //  0009  GETMET	R2	R0	K20
      0x7C080200,  //  000A  CALL	R2	1
      0x8C080114,  //  000B  GETMET	R2	R0	K20
      0x7C080200,  //  000C  CALL	R2	1
      0x80066000,  //  000D  RET	1	K48
      0x70020016,  //  000E  JMP		#0026
      0x8C080113,  //  000F  GETMET	R2	R0	K19
      0x7C080200,  //  0010  CALL	R2	1
      0x1C080531,  //  0011  EQ	R2	R2	K49
      0x780A0003,  //  0012  JMPF	R2	#0017
      0x8C080114,  //  0013  GETMET	R2	R0	K20
      0x7C080200,  //  0014  CALL	R2	1
      0x80066200,  //  0015  RET	1	K49
      0x7002000E,  //  0016  JMP		#0026
      0x8C080113,  //  0017  GETMET	R2	R0	K19
      0x7C080200,  //  0018  CALL	R2	1
      0x1C080532,  //  0019  EQ	R2	R2	K50
      0x780A0003,  //  001A  JMPF	R2	#001F
      0x8C080114,  //  001B  GETMET	R2	R0	K20
      0x7C080200,  //  001C  CALL	R2	1
      0x80066400,  //  001D  RET	1	K50
      0x70020006,  //  001E  JMP		#0026
      0x8C080113,  //  001F  GETMET	R2	R0	K19
      0x7C080200,  //  0020  CALL	R2	1
      0x1C080533,  //  0021  EQ	R2	R2	K51
      0x780A0002,  //  0022  JMPF	R2	#0026
      0x8C080114,  //  0023  GETMET	R2	R0	K20
      0x7C080200,  //  0024  CALL	R2	1
      0x80066600,  //  0025  RET	1	K51
      0x80062000,  //  0026  RET	1	K16
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: at_end
********************************************************************/
be_local_closure(class_DSLLexer_at_end,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(at_end),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C010F,  //  0002  GETMBR	R3	R0	K15
      0x7C080200,  //  0003  CALL	R2	1
      0x28040202,  //  0004  GE	R1	R1	R2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: match
********************************************************************/
be_local_closure(class_DSLLexer_match,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(match),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C080111,  //  0000  GETMET	R2	R0	K17
      0x7C080200,  //  0001  CALL	R2	1
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x8808010F,  //  0003  GETMBR	R2	R0	K15
      0x880C010D,  //  0004  GETMBR	R3	R0	K13
      0x94080403,  //  0005  GETIDX	R2	R2	R3
      0x20080401,  //  0006  NE	R2	R2	R1
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x50080000,  //  0008  LDBOOL	R2	0	0
      0x80040400,  //  0009  RET	1	R2
      0x8808010D,  //  000A  GETMBR	R2	R0	K13
      0x0008050E,  //  000B  ADD	R2	R2	K14
      0x90021A02,  //  000C  SETMBR	R0	K13	R2
      0x88080108,  //  000D  GETMBR	R2	R0	K8
      0x0008050E,  //  000E  ADD	R2	R2	K14
      0x90021002,  //  000F  SETMBR	R0	K8	R2
      0x50080200,  //  0010  LDBOOL	R2	1	0
      0x80040400,  //  0011  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_time_suffix
********************************************************************/
be_local_closure(class_DSLLexer_check_time_suffix,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(check_time_suffix),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0xA4065A00,  //  0000  IMPORT	R1	K45
      0x8C080111,  //  0001  GETMET	R2	R0	K17
      0x7C080200,  //  0002  CALL	R2	1
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x50080000,  //  0004  LDBOOL	R2	0	0
      0x80040400,  //  0005  RET	1	R2
      0x8808010D,  //  0006  GETMBR	R2	R0	K13
      0x4008052F,  //  0007  CONNECT	R2	R2	K47
      0x880C010F,  //  0008  GETMBR	R3	R0	K15
      0x94080602,  //  0009  GETIDX	R2	R3	R2
      0x8C0C032E,  //  000A  GETMET	R3	R1	K46
      0x5C140400,  //  000B  MOVE	R5	R2
      0x58180030,  //  000C  LDCONST	R6	K48
      0x7C0C0600,  //  000D  CALL	R3	3
      0x740E000F,  //  000E  JMPT	R3	#001F
      0x8C0C032E,  //  000F  GETMET	R3	R1	K46
      0x5C140400,  //  0010  MOVE	R5	R2
      0x58180031,  //  0011  LDCONST	R6	K49
      0x7C0C0600,  //  0012  CALL	R3	3
      0x740E000A,  //  0013  JMPT	R3	#001F
      0x8C0C032E,  //  0014  GETMET	R3	R1	K46
      0x5C140400,  //  0015  MOVE	R5	R2
      0x58180032,  //  0016  LDCONST	R6	K50
      0x7C0C0600,  //  0017  CALL	R3	3
      0x740E0005,  //  0018  JMPT	R3	#001F
      0x8C0C032E,  //  0019  GETMET	R3	R1	K46
      0x5C140400,  //  001A  MOVE	R5	R2
      0x58180033,  //  001B  LDCONST	R6	K51
      0x7C0C0600,  //  001C  CALL	R3	3
      0x740E0000,  //  001D  JMPT	R3	#001F
      0x500C0001,  //  001E  LDBOOL	R3	0	1
      0x500C0200,  //  001F  LDBOOL	R3	1	0
      0x80040600,  //  0020  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(class_DSLLexer_reset,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x5C080200,  //  0003  MOVE	R2	R1
      0x70020000,  //  0004  JMP		#0006
      0x58080010,  //  0005  LDCONST	R2	K16
      0x90021E02,  //  0006  SETMBR	R0	K15	R2
      0x90021B25,  //  0007  SETMBR	R0	K13	K37
      0x90020D0E,  //  0008  SETMBR	R0	K6	K14
      0x9002110E,  //  0009  SETMBR	R0	K8	K14
      0x60080012,  //  000A  GETGBL	R2	G18
      0x7C080000,  //  000B  CALL	R2	0
      0x90025602,  //  000C  SETMBR	R0	K43	R2
      0x60080012,  //  000D  GETGBL	R2	G18
      0x7C080000,  //  000E  CALL	R2	0
      0x90020002,  //  000F  SETMBR	R0	K0	R2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_variable_reference
********************************************************************/
be_local_closure(class_DSLLexer_scan_variable_reference,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_variable_reference),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x0404030E,  //  0001  SUB	R1	R1	K14
      0x88080108,  //  0002  GETMBR	R2	R0	K8
      0x0408050E,  //  0003  SUB	R2	R2	K14
      0x8C0C0111,  //  0004  GETMET	R3	R0	K17
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E000B,  //  0006  JMPT	R3	#0013
      0x8C0C0134,  //  0007  GETMET	R3	R0	K52
      0x8C140113,  //  0008  GETMET	R5	R0	K19
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x740E0004,  //  000B  JMPT	R3	#0011
      0x8C0C0113,  //  000C  GETMET	R3	R0	K19
      0x7C0C0200,  //  000D  CALL	R3	1
      0x1C0C0735,  //  000E  EQ	R3	R3	K53
      0x740E0000,  //  000F  JMPT	R3	#0011
      0x500C0001,  //  0010  LDBOOL	R3	0	1
      0x500C0200,  //  0011  LDBOOL	R3	1	0
      0x740E0008,  //  0012  JMPT	R3	#001C
      0x8C0C0122,  //  0013  GETMET	R3	R0	K34
      0x58140036,  //  0014  LDCONST	R5	K54
      0x7C0C0400,  //  0015  CALL	R3	2
      0x8C0C0118,  //  0016  GETMET	R3	R0	K24
      0x54160026,  //  0017  LDINT	R5	39
      0x58180037,  //  0018  LDCONST	R6	K55
      0x581C000E,  //  0019  LDCONST	R7	K14
      0x7C0C0800,  //  001A  CALL	R3	4
      0x80000600,  //  001B  RET	0
      0x8C0C0111,  //  001C  GETMET	R3	R0	K17
      0x7C0C0200,  //  001D  CALL	R3	1
      0x740E000B,  //  001E  JMPT	R3	#002B
      0x8C0C0138,  //  001F  GETMET	R3	R0	K56
      0x8C140113,  //  0020  GETMET	R5	R0	K19
      0x7C140200,  //  0021  CALL	R5	1
      0x7C0C0400,  //  0022  CALL	R3	2
      0x740E0003,  //  0023  JMPT	R3	#0028
      0x8C0C0113,  //  0024  GETMET	R3	R0	K19
      0x7C0C0200,  //  0025  CALL	R3	1
      0x1C0C0735,  //  0026  EQ	R3	R3	K53
      0x780E0002,  //  0027  JMPF	R3	#002B
      0x8C0C0114,  //  0028  GETMET	R3	R0	K20
      0x7C0C0200,  //  0029  CALL	R3	1
      0x7001FFF0,  //  002A  JMP		#001C
      0x880C010D,  //  002B  GETMBR	R3	R0	K13
      0x040C070E,  //  002C  SUB	R3	R3	K14
      0x400C0203,  //  002D  CONNECT	R3	R1	R3
      0x8810010F,  //  002E  GETMBR	R4	R0	K15
      0x940C0803,  //  002F  GETIDX	R3	R4	R3
      0x8C100118,  //  0030  GETMET	R4	R0	K24
      0x541A0023,  //  0031  LDINT	R6	36
      0x5C1C0600,  //  0032  MOVE	R7	R3
      0x6020000C,  //  0033  GETGBL	R8	G12
      0x5C240600,  //  0034  MOVE	R9	R3
      0x7C200200,  //  0035  CALL	R8	1
      0x7C100800,  //  0036  CALL	R4	4
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_digit
********************************************************************/
be_local_closure(class_DSLLexer_is_digit,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(is_digit),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x28080339,  //  0000  GE	R2	R1	K57
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x1808033A,  //  0002  LE	R2	R1	K58
      0x740A0000,  //  0003  JMPT	R2	#0005
      0x50080001,  //  0004  LDBOOL	R2	0	1
      0x50080200,  //  0005  LDBOOL	R2	1	0
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_DSLLexer_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x5C080200,  //  0003  MOVE	R2	R1
      0x70020000,  //  0004  JMP		#0006
      0x58080010,  //  0005  LDCONST	R2	K16
      0x90021E02,  //  0006  SETMBR	R0	K15	R2
      0x90021B25,  //  0007  SETMBR	R0	K13	K37
      0x90020D0E,  //  0008  SETMBR	R0	K6	K14
      0x9002110E,  //  0009  SETMBR	R0	K8	K14
      0x60080012,  //  000A  GETGBL	R2	G18
      0x7C080000,  //  000B  CALL	R2	0
      0x90025602,  //  000C  SETMBR	R0	K43	R2
      0x60080012,  //  000D  GETGBL	R2	G18
      0x7C080000,  //  000E  CALL	R2	0
      0x90020002,  //  000F  SETMBR	R0	K0	R2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_alpha
********************************************************************/
be_local_closure(class_DSLLexer_is_alpha,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(is_alpha),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x2808033B,  //  0000  GE	R2	R1	K59
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x1808033C,  //  0002  LE	R2	R1	K60
      0x740A0004,  //  0003  JMPT	R2	#0009
      0x2808033D,  //  0004  GE	R2	R1	K61
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x1808033E,  //  0006  LE	R2	R1	K62
      0x740A0000,  //  0007  JMPT	R2	#0009
      0x50080001,  //  0008  LDBOOL	R2	0	1
      0x50080200,  //  0009  LDBOOL	R2	1	0
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_comment
********************************************************************/
be_local_closure(class_DSLLexer_scan_comment,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_comment),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x0404030E,  //  0001  SUB	R1	R1	K14
      0x88080108,  //  0002  GETMBR	R2	R0	K8
      0x0408050E,  //  0003  SUB	R2	R2	K14
      0x8C0C0111,  //  0004  GETMET	R3	R0	K17
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E0006,  //  0006  JMPT	R3	#000E
      0x8C0C0113,  //  0007  GETMET	R3	R0	K19
      0x7C0C0200,  //  0008  CALL	R3	1
      0x200C070B,  //  0009  NE	R3	R3	K11
      0x780E0002,  //  000A  JMPF	R3	#000E
      0x8C0C0114,  //  000B  GETMET	R3	R0	K20
      0x7C0C0200,  //  000C  CALL	R3	1
      0x7001FFF5,  //  000D  JMP		#0004
      0x880C010D,  //  000E  GETMBR	R3	R0	K13
      0x040C070E,  //  000F  SUB	R3	R3	K14
      0x400C0203,  //  0010  CONNECT	R3	R1	R3
      0x8810010F,  //  0011  GETMBR	R4	R0	K15
      0x940C0803,  //  0012  GETIDX	R3	R4	R3
      0x8C100118,  //  0013  GETMET	R4	R0	K24
      0x541A0024,  //  0014  LDINT	R6	37
      0x5C1C0600,  //  0015  MOVE	R7	R3
      0x8820010D,  //  0016  GETMBR	R8	R0	K13
      0x04201001,  //  0017  SUB	R8	R8	R1
      0x7C100800,  //  0018  CALL	R4	4
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_error
********************************************************************/
be_local_closure(class_DSLLexer_add_error,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(add_error),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C08052C,  //  0001  GETMET	R2	R2	K44
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C100000,  //  0003  CALL	R4	0
      0x98121401,  //  0004  SETIDX	R4	K10	R1
      0x88140106,  //  0005  GETMBR	R5	R0	K6
      0x98120C05,  //  0006  SETIDX	R4	K6	R5
      0x88140108,  //  0007  GETMBR	R5	R0	K8
      0x98121005,  //  0008  SETIDX	R4	K8	R5
      0x8814010D,  //  0009  GETMBR	R5	R0	K13
      0x98121A05,  //  000A  SETIDX	R4	K13	R5
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_alnum
********************************************************************/
be_local_closure(class_DSLLexer_is_alnum,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(is_alnum),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C080134,  //  0000  GETMET	R2	R0	K52
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x740A0004,  //  0003  JMPT	R2	#0009
      0x8C080112,  //  0004  GETMET	R2	R0	K18
      0x5C100200,  //  0005  MOVE	R4	R1
      0x7C080400,  //  0006  CALL	R2	2
      0x740A0000,  //  0007  JMPT	R2	#0009
      0x50080001,  //  0008  LDBOOL	R2	0	1
      0x50080200,  //  0009  LDBOOL	R2	1	0
      0x80040400,  //  000A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_identifier_or_keyword
********************************************************************/
be_local_closure(class_DSLLexer_scan_identifier_or_keyword,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_identifier_or_keyword),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x8804010D,  //  0000  GETMBR	R1	R0	K13
      0x0404030E,  //  0001  SUB	R1	R1	K14
      0x88080108,  //  0002  GETMBR	R2	R0	K8
      0x0408050E,  //  0003  SUB	R2	R2	K14
      0x8C0C0111,  //  0004  GETMET	R3	R0	K17
      0x7C0C0200,  //  0005  CALL	R3	1
      0x740E000B,  //  0006  JMPT	R3	#0013
      0x8C0C0138,  //  0007  GETMET	R3	R0	K56
      0x8C140113,  //  0008  GETMET	R5	R0	K19
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x740E0003,  //  000B  JMPT	R3	#0010
      0x8C0C0113,  //  000C  GETMET	R3	R0	K19
      0x7C0C0200,  //  000D  CALL	R3	1
      0x1C0C0735,  //  000E  EQ	R3	R3	K53
      0x780E0002,  //  000F  JMPF	R3	#0013
      0x8C0C0114,  //  0010  GETMET	R3	R0	K20
      0x7C0C0200,  //  0011  CALL	R3	1
      0x7001FFF0,  //  0012  JMP		#0004
      0x880C010D,  //  0013  GETMBR	R3	R0	K13
      0x040C070E,  //  0014  SUB	R3	R3	K14
      0x400C0203,  //  0015  CONNECT	R3	R1	R3
      0x8810010F,  //  0016  GETMBR	R4	R0	K15
      0x940C0803,  //  0017  GETIDX	R3	R4	R3
      0x4C100000,  //  0018  LDNIL	R4
      0xB8165200,  //  0019  GETNGBL	R5	K41
      0x8C140B3F,  //  001A  GETMET	R5	R5	K63
      0x5C1C0600,  //  001B  MOVE	R7	R3
      0x7C140400,  //  001C  CALL	R5	2
      0x78160001,  //  001D  JMPF	R5	#0020
      0x54120003,  //  001E  LDINT	R4	4
      0x70020007,  //  001F  JMP		#0028
      0xB8165200,  //  0020  GETNGBL	R5	K41
      0x8C140B40,  //  0021  GETMET	R5	R5	K64
      0x5C1C0600,  //  0022  MOVE	R7	R3
      0x7C140400,  //  0023  CALL	R5	2
      0x78160001,  //  0024  JMPF	R5	#0027
      0x58100025,  //  0025  LDCONST	R4	K37
      0x70020000,  //  0026  JMP		#0028
      0x5810000E,  //  0027  LDCONST	R4	K14
      0x8C140118,  //  0028  GETMET	R5	R0	K24
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x5C200600,  //  002A  MOVE	R8	R3
      0x6024000C,  //  002B  GETGBL	R9	G12
      0x5C280600,  //  002C  MOVE	R10	R3
      0x7C240200,  //  002D  CALL	R9	1
      0x7C140800,  //  002E  CALL	R5	4
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_hex_digit
********************************************************************/
be_local_closure(class_DSLLexer_is_hex_digit,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(is_hex_digit),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C080112,  //  0000  GETMET	R2	R0	K18
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x740A0008,  //  0003  JMPT	R2	#000D
      0x2808033B,  //  0004  GE	R2	R1	K59
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x18080341,  //  0006  LE	R2	R1	K65
      0x740A0004,  //  0007  JMPT	R2	#000D
      0x2808033D,  //  0008  GE	R2	R1	K61
      0x780A0001,  //  0009  JMPF	R2	#000C
      0x18080342,  //  000A  LE	R2	R1	K66
      0x740A0000,  //  000B  JMPT	R2	#000D
      0x50080001,  //  000C  LDBOOL	R2	0	1
      0x50080200,  //  000D  LDBOOL	R2	1	0
      0x80040400,  //  000E  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: peek
********************************************************************/
be_local_closure(class_DSLLexer_peek,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(peek),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C040111,  //  0000  GETMET	R1	R0	K17
      0x7C040200,  //  0001  CALL	R1	1
      0x78060000,  //  0002  JMPF	R1	#0004
      0x80062000,  //  0003  RET	1	K16
      0x8804010F,  //  0004  GETMBR	R1	R0	K15
      0x8808010D,  //  0005  GETMBR	R2	R0	K13
      0x94040202,  //  0006  GETIDX	R1	R1	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_operator_or_delimiter
********************************************************************/
be_local_closure(class_DSLLexer_scan_operator_or_delimiter,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_operator_or_delimiter),
    &be_const_str_solidified,
    ( &(const binstruction[299]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x0408050E,  //  0001  SUB	R2	R2	K14
      0x1C0C0343,  //  0002  EQ	R3	R1	K67
      0x780E000F,  //  0003  JMPF	R3	#0014
      0x8C0C0144,  //  0004  GETMET	R3	R0	K68
      0x58140043,  //  0005  LDCONST	R5	K67
      0x7C0C0400,  //  0006  CALL	R3	2
      0x780E0005,  //  0007  JMPF	R3	#000E
      0x8C0C0118,  //  0008  GETMET	R3	R0	K24
      0x5416000E,  //  0009  LDINT	R5	15
      0x58180045,  //  000A  LDCONST	R6	K69
      0x581C001B,  //  000B  LDCONST	R7	K27
      0x7C0C0800,  //  000C  CALL	R3	4
      0x70020004,  //  000D  JMP		#0013
      0x8C0C0118,  //  000E  GETMET	R3	R0	K24
      0x54160007,  //  000F  LDINT	R5	8
      0x58180043,  //  0010  LDCONST	R6	K67
      0x581C000E,  //  0011  LDCONST	R7	K14
      0x7C0C0800,  //  0012  CALL	R3	4
      0x70020115,  //  0013  JMP		#012A
      0x1C0C0346,  //  0014  EQ	R3	R1	K70
      0x780E000F,  //  0015  JMPF	R3	#0026
      0x8C0C0144,  //  0016  GETMET	R3	R0	K68
      0x58140043,  //  0017  LDCONST	R5	K67
      0x7C0C0400,  //  0018  CALL	R3	2
      0x780E0005,  //  0019  JMPF	R3	#0020
      0x8C0C0118,  //  001A  GETMET	R3	R0	K24
      0x5416000F,  //  001B  LDINT	R5	16
      0x58180047,  //  001C  LDCONST	R6	K71
      0x581C001B,  //  001D  LDCONST	R7	K27
      0x7C0C0800,  //  001E  CALL	R3	4
      0x70020004,  //  001F  JMP		#0025
      0x8C0C0118,  //  0020  GETMET	R3	R0	K24
      0x54160016,  //  0021  LDINT	R5	23
      0x58180046,  //  0022  LDCONST	R6	K70
      0x581C000E,  //  0023  LDCONST	R7	K14
      0x7C0C0800,  //  0024  CALL	R3	4
      0x70020103,  //  0025  JMP		#012A
      0x1C0C0348,  //  0026  EQ	R3	R1	K72
      0x780E0019,  //  0027  JMPF	R3	#0042
      0x8C0C0144,  //  0028  GETMET	R3	R0	K68
      0x58140043,  //  0029  LDCONST	R5	K67
      0x7C0C0400,  //  002A  CALL	R3	2
      0x780E0005,  //  002B  JMPF	R3	#0032
      0x8C0C0118,  //  002C  GETMET	R3	R0	K24
      0x54160011,  //  002D  LDINT	R5	18
      0x58180049,  //  002E  LDCONST	R6	K73
      0x581C001B,  //  002F  LDCONST	R7	K27
      0x7C0C0800,  //  0030  CALL	R3	4
      0x7002000E,  //  0031  JMP		#0041
      0x8C0C0144,  //  0032  GETMET	R3	R0	K68
      0x58140048,  //  0033  LDCONST	R5	K72
      0x7C0C0400,  //  0034  CALL	R3	2
      0x780E0005,  //  0035  JMPF	R3	#003C
      0x8C0C0118,  //  0036  GETMET	R3	R0	K24
      0x54160026,  //  0037  LDINT	R5	39
      0x5818004A,  //  0038  LDCONST	R6	K74
      0x581C001B,  //  0039  LDCONST	R7	K27
      0x7C0C0800,  //  003A  CALL	R3	4
      0x70020004,  //  003B  JMP		#0041
      0x8C0C0118,  //  003C  GETMET	R3	R0	K24
      0x54160010,  //  003D  LDINT	R5	17
      0x58180048,  //  003E  LDCONST	R6	K72
      0x581C000E,  //  003F  LDCONST	R7	K14
      0x7C0C0800,  //  0040  CALL	R3	4
      0x700200E7,  //  0041  JMP		#012A
      0x1C0C034B,  //  0042  EQ	R3	R1	K75
      0x780E0019,  //  0043  JMPF	R3	#005E
      0x8C0C0144,  //  0044  GETMET	R3	R0	K68
      0x58140043,  //  0045  LDCONST	R5	K67
      0x7C0C0400,  //  0046  CALL	R3	2
      0x780E0005,  //  0047  JMPF	R3	#004E
      0x8C0C0118,  //  0048  GETMET	R3	R0	K24
      0x54160013,  //  0049  LDINT	R5	20
      0x5818004C,  //  004A  LDCONST	R6	K76
      0x581C001B,  //  004B  LDCONST	R7	K27
      0x7C0C0800,  //  004C  CALL	R3	4
      0x7002000E,  //  004D  JMP		#005D
      0x8C0C0144,  //  004E  GETMET	R3	R0	K68
      0x5814004B,  //  004F  LDCONST	R5	K75
      0x7C0C0400,  //  0050  CALL	R3	2
      0x780E0005,  //  0051  JMPF	R3	#0058
      0x8C0C0118,  //  0052  GETMET	R3	R0	K24
      0x54160026,  //  0053  LDINT	R5	39
      0x5818004D,  //  0054  LDCONST	R6	K77
      0x581C001B,  //  0055  LDCONST	R7	K27
      0x7C0C0800,  //  0056  CALL	R3	4
      0x70020004,  //  0057  JMP		#005D
      0x8C0C0118,  //  0058  GETMET	R3	R0	K24
      0x54160012,  //  0059  LDINT	R5	19
      0x5818004B,  //  005A  LDCONST	R6	K75
      0x581C000E,  //  005B  LDCONST	R7	K14
      0x7C0C0800,  //  005C  CALL	R3	4
      0x700200CB,  //  005D  JMP		#012A
      0x1C0C034E,  //  005E  EQ	R3	R1	K78
      0x780E0012,  //  005F  JMPF	R3	#0073
      0x8C0C0144,  //  0060  GETMET	R3	R0	K68
      0x5814004E,  //  0061  LDCONST	R5	K78
      0x7C0C0400,  //  0062  CALL	R3	2
      0x780E0005,  //  0063  JMPF	R3	#006A
      0x8C0C0118,  //  0064  GETMET	R3	R0	K24
      0x54160014,  //  0065  LDINT	R5	21
      0x5818004F,  //  0066  LDCONST	R6	K79
      0x581C001B,  //  0067  LDCONST	R7	K27
      0x7C0C0800,  //  0068  CALL	R3	4
      0x70020007,  //  0069  JMP		#0072
      0x8C0C0122,  //  006A  GETMET	R3	R0	K34
      0x58140050,  //  006B  LDCONST	R5	K80
      0x7C0C0400,  //  006C  CALL	R3	2
      0x8C0C0118,  //  006D  GETMET	R3	R0	K24
      0x54160026,  //  006E  LDINT	R5	39
      0x5818004E,  //  006F  LDCONST	R6	K78
      0x581C000E,  //  0070  LDCONST	R7	K14
      0x7C0C0800,  //  0071  CALL	R3	4
      0x700200B6,  //  0072  JMP		#012A
      0x1C0C0351,  //  0073  EQ	R3	R1	K81
      0x780E0012,  //  0074  JMPF	R3	#0088
      0x8C0C0144,  //  0075  GETMET	R3	R0	K68
      0x58140051,  //  0076  LDCONST	R5	K81
      0x7C0C0400,  //  0077  CALL	R3	2
      0x780E0005,  //  0078  JMPF	R3	#007F
      0x8C0C0118,  //  0079  GETMET	R3	R0	K24
      0x54160015,  //  007A  LDINT	R5	22
      0x58180052,  //  007B  LDCONST	R6	K82
      0x581C001B,  //  007C  LDCONST	R7	K27
      0x7C0C0800,  //  007D  CALL	R3	4
      0x70020007,  //  007E  JMP		#0087
      0x8C0C0122,  //  007F  GETMET	R3	R0	K34
      0x58140053,  //  0080  LDCONST	R5	K83
      0x7C0C0400,  //  0081  CALL	R3	2
      0x8C0C0118,  //  0082  GETMET	R3	R0	K24
      0x54160026,  //  0083  LDINT	R5	39
      0x58180051,  //  0084  LDCONST	R6	K81
      0x581C000E,  //  0085  LDCONST	R7	K14
      0x7C0C0800,  //  0086  CALL	R3	4
      0x700200A1,  //  0087  JMP		#012A
      0x1C0C0354,  //  0088  EQ	R3	R1	K84
      0x780E000F,  //  0089  JMPF	R3	#009A
      0x8C0C0144,  //  008A  GETMET	R3	R0	K68
      0x5814004B,  //  008B  LDCONST	R5	K75
      0x7C0C0400,  //  008C  CALL	R3	2
      0x780E0005,  //  008D  JMPF	R3	#0094
      0x8C0C0118,  //  008E  GETMET	R3	R0	K24
      0x54160021,  //  008F  LDINT	R5	34
      0x58180055,  //  0090  LDCONST	R6	K85
      0x581C001B,  //  0091  LDCONST	R7	K27
      0x7C0C0800,  //  0092  CALL	R3	4
      0x70020004,  //  0093  JMP		#0099
      0x8C0C0118,  //  0094  GETMET	R3	R0	K24
      0x54160009,  //  0095  LDINT	R5	10
      0x58180054,  //  0096  LDCONST	R6	K84
      0x581C000E,  //  0097  LDCONST	R7	K14
      0x7C0C0800,  //  0098  CALL	R3	4
      0x7002008F,  //  0099  JMP		#012A
      0x1C0C0356,  //  009A  EQ	R3	R1	K86
      0x780E0005,  //  009B  JMPF	R3	#00A2
      0x8C0C0118,  //  009C  GETMET	R3	R0	K24
      0x54160008,  //  009D  LDINT	R5	9
      0x58180056,  //  009E  LDCONST	R6	K86
      0x581C000E,  //  009F  LDCONST	R7	K14
      0x7C0C0800,  //  00A0  CALL	R3	4
      0x70020087,  //  00A1  JMP		#012A
      0x1C0C0357,  //  00A2  EQ	R3	R1	K87
      0x780E0005,  //  00A3  JMPF	R3	#00AA
      0x8C0C0118,  //  00A4  GETMET	R3	R0	K24
      0x5416000A,  //  00A5  LDINT	R5	11
      0x58180057,  //  00A6  LDCONST	R6	K87
      0x581C000E,  //  00A7  LDCONST	R7	K14
      0x7C0C0800,  //  00A8  CALL	R3	4
      0x7002007F,  //  00A9  JMP		#012A
      0x1C0C0358,  //  00AA  EQ	R3	R1	K88
      0x780E0005,  //  00AB  JMPF	R3	#00B2
      0x8C0C0118,  //  00AC  GETMET	R3	R0	K24
      0x5416000B,  //  00AD  LDINT	R5	12
      0x58180058,  //  00AE  LDCONST	R6	K88
      0x581C000E,  //  00AF  LDCONST	R7	K14
      0x7C0C0800,  //  00B0  CALL	R3	4
      0x70020077,  //  00B1  JMP		#012A
      0x1C0C0319,  //  00B2  EQ	R3	R1	K25
      0x780E0005,  //  00B3  JMPF	R3	#00BA
      0x8C0C0118,  //  00B4  GETMET	R3	R0	K24
      0x5416000C,  //  00B5  LDINT	R5	13
      0x58180019,  //  00B6  LDCONST	R6	K25
      0x581C000E,  //  00B7  LDCONST	R7	K14
      0x7C0C0800,  //  00B8  CALL	R3	4
      0x7002006F,  //  00B9  JMP		#012A
      0x1C0C0359,  //  00BA  EQ	R3	R1	K89
      0x780E0005,  //  00BB  JMPF	R3	#00C2
      0x8C0C0118,  //  00BC  GETMET	R3	R0	K24
      0x5416000D,  //  00BD  LDINT	R5	14
      0x58180059,  //  00BE  LDCONST	R6	K89
      0x581C000E,  //  00BF  LDCONST	R7	K14
      0x7C0C0800,  //  00C0  CALL	R3	4
      0x70020067,  //  00C1  JMP		#012A
      0x1C0C035A,  //  00C2  EQ	R3	R1	K90
      0x780E0005,  //  00C3  JMPF	R3	#00CA
      0x8C0C0118,  //  00C4  GETMET	R3	R0	K24
      0x54160017,  //  00C5  LDINT	R5	24
      0x5818005A,  //  00C6  LDCONST	R6	K90
      0x581C000E,  //  00C7  LDCONST	R7	K14
      0x7C0C0800,  //  00C8  CALL	R3	4
      0x7002005F,  //  00C9  JMP		#012A
      0x1C0C035B,  //  00CA  EQ	R3	R1	K91
      0x780E0005,  //  00CB  JMPF	R3	#00D2
      0x8C0C0118,  //  00CC  GETMET	R3	R0	K24
      0x54160018,  //  00CD  LDINT	R5	25
      0x5818005B,  //  00CE  LDCONST	R6	K91
      0x581C000E,  //  00CF  LDCONST	R7	K14
      0x7C0C0800,  //  00D0  CALL	R3	4
      0x70020057,  //  00D1  JMP		#012A
      0x1C0C035C,  //  00D2  EQ	R3	R1	K92
      0x780E0005,  //  00D3  JMPF	R3	#00DA
      0x8C0C0118,  //  00D4  GETMET	R3	R0	K24
      0x54160019,  //  00D5  LDINT	R5	26
      0x5818005C,  //  00D6  LDCONST	R6	K92
      0x581C000E,  //  00D7  LDCONST	R7	K14
      0x7C0C0800,  //  00D8  CALL	R3	4
      0x7002004F,  //  00D9  JMP		#012A
      0x1C0C035D,  //  00DA  EQ	R3	R1	K93
      0x780E0005,  //  00DB  JMPF	R3	#00E2
      0x8C0C0118,  //  00DC  GETMET	R3	R0	K24
      0x5416001A,  //  00DD  LDINT	R5	27
      0x5818005D,  //  00DE  LDCONST	R6	K93
      0x581C000E,  //  00DF  LDCONST	R7	K14
      0x7C0C0800,  //  00E0  CALL	R3	4
      0x70020047,  //  00E1  JMP		#012A
      0x1C0C035E,  //  00E2  EQ	R3	R1	K94
      0x780E0005,  //  00E3  JMPF	R3	#00EA
      0x8C0C0118,  //  00E4  GETMET	R3	R0	K24
      0x5416001B,  //  00E5  LDINT	R5	28
      0x5818005E,  //  00E6  LDCONST	R6	K94
      0x581C000E,  //  00E7  LDCONST	R7	K14
      0x7C0C0800,  //  00E8  CALL	R3	4
      0x7002003F,  //  00E9  JMP		#012A
      0x1C0C035F,  //  00EA  EQ	R3	R1	K95
      0x780E0005,  //  00EB  JMPF	R3	#00F2
      0x8C0C0118,  //  00EC  GETMET	R3	R0	K24
      0x5416001C,  //  00ED  LDINT	R5	29
      0x5818005F,  //  00EE  LDCONST	R6	K95
      0x581C000E,  //  00EF  LDCONST	R7	K14
      0x7C0C0800,  //  00F0  CALL	R3	4
      0x70020037,  //  00F1  JMP		#012A
      0x1C0C0360,  //  00F2  EQ	R3	R1	K96
      0x780E0005,  //  00F3  JMPF	R3	#00FA
      0x8C0C0118,  //  00F4  GETMET	R3	R0	K24
      0x5416001D,  //  00F5  LDINT	R5	30
      0x58180060,  //  00F6  LDCONST	R6	K96
      0x581C000E,  //  00F7  LDCONST	R7	K14
      0x7C0C0800,  //  00F8  CALL	R3	4
      0x7002002F,  //  00F9  JMP		#012A
      0x1C0C0361,  //  00FA  EQ	R3	R1	K97
      0x780E0005,  //  00FB  JMPF	R3	#0102
      0x8C0C0118,  //  00FC  GETMET	R3	R0	K24
      0x5416001E,  //  00FD  LDINT	R5	31
      0x58180061,  //  00FE  LDCONST	R6	K97
      0x581C000E,  //  00FF  LDCONST	R7	K14
      0x7C0C0800,  //  0100  CALL	R3	4
      0x70020027,  //  0101  JMP		#012A
      0x1C0C0307,  //  0102  EQ	R3	R1	K7
      0x780E0005,  //  0103  JMPF	R3	#010A
      0x8C0C0118,  //  0104  GETMET	R3	R0	K24
      0x5416001F,  //  0105  LDINT	R5	32
      0x58180007,  //  0106  LDCONST	R6	K7
      0x581C000E,  //  0107  LDCONST	R7	K14
      0x7C0C0800,  //  0108  CALL	R3	4
      0x7002001F,  //  0109  JMP		#012A
      0x1C0C0315,  //  010A  EQ	R3	R1	K21
      0x780E0014,  //  010B  JMPF	R3	#0121
      0x8C0C0144,  //  010C  GETMET	R3	R0	K68
      0x58140015,  //  010D  LDCONST	R5	K21
      0x7C0C0400,  //  010E  CALL	R3	2
      0x780E000A,  //  010F  JMPF	R3	#011B
      0x8C0C0118,  //  0110  GETMET	R3	R0	K24
      0x54160020,  //  0111  LDINT	R5	33
      0x58180015,  //  0112  LDCONST	R6	K21
      0x581C000E,  //  0113  LDCONST	R7	K14
      0x7C0C0800,  //  0114  CALL	R3	4
      0x8C0C0118,  //  0115  GETMET	R3	R0	K24
      0x54160020,  //  0116  LDINT	R5	33
      0x58180015,  //  0117  LDCONST	R6	K21
      0x581C000E,  //  0118  LDCONST	R7	K14
      0x7C0C0800,  //  0119  CALL	R3	4
      0x70020004,  //  011A  JMP		#0120
      0x8C0C0118,  //  011B  GETMET	R3	R0	K24
      0x54160020,  //  011C  LDINT	R5	33
      0x58180015,  //  011D  LDCONST	R6	K21
      0x581C000E,  //  011E  LDCONST	R7	K14
      0x7C0C0800,  //  011F  CALL	R3	4
      0x70020008,  //  0120  JMP		#012A
      0x8C0C0122,  //  0121  GETMET	R3	R0	K34
      0x0016C401,  //  0122  ADD	R5	K98	R1
      0x00140B63,  //  0123  ADD	R5	R5	K99
      0x7C0C0400,  //  0124  CALL	R3	2
      0x8C0C0118,  //  0125  GETMET	R3	R0	K24
      0x54160026,  //  0126  LDINT	R5	39
      0x5C180200,  //  0127  MOVE	R6	R1
      0x581C000E,  //  0128  LDCONST	R7	K14
      0x7C0C0800,  //  0129  CALL	R3	4
      0x80000000,  //  012A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tokenize
********************************************************************/
be_local_closure(class_DSLLexer_tokenize,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(tokenize),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x90025601,  //  0002  SETMBR	R0	K43	R1
      0x60040012,  //  0003  GETGBL	R1	G18
      0x7C040000,  //  0004  CALL	R1	0
      0x90020001,  //  0005  SETMBR	R0	K0	R1
      0x90021B25,  //  0006  SETMBR	R0	K13	K37
      0x90020D0E,  //  0007  SETMBR	R0	K6	K14
      0x9002110E,  //  0008  SETMBR	R0	K8	K14
      0x8C040111,  //  0009  GETMET	R1	R0	K17
      0x7C040200,  //  000A  CALL	R1	1
      0x74060002,  //  000B  JMPT	R1	#000F
      0x8C040164,  //  000C  GETMET	R1	R0	K100
      0x7C040200,  //  000D  CALL	R1	1
      0x7001FFF9,  //  000E  JMP		#0009
      0x8C040118,  //  000F  GETMET	R1	R0	K24
      0x540E0025,  //  0010  LDINT	R3	38
      0x58100010,  //  0011  LDCONST	R4	K16
      0x58140025,  //  0012  LDCONST	R5	K37
      0x7C040800,  //  0013  CALL	R1	4
      0x8804012B,  //  0014  GETMBR	R1	R0	K43
      0x80040200,  //  0015  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: has_errors
********************************************************************/
be_local_closure(class_DSLLexer_has_errors,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(has_errors),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x24040325,  //  0003  GT	R1	R1	K37
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scan_token
********************************************************************/
be_local_closure(class_DSLLexer_scan_token,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(scan_token),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x8C080114,  //  0001  GETMET	R2	R0	K20
      0x7C080200,  //  0002  CALL	R2	1
      0x1C0C0565,  //  0003  EQ	R3	R2	K101
      0x740E0003,  //  0004  JMPT	R3	#0009
      0x1C0C051F,  //  0005  EQ	R3	R2	K31
      0x740E0001,  //  0006  JMPT	R3	#0009
      0x1C0C0521,  //  0007  EQ	R3	R2	K33
      0x780E0001,  //  0008  JMPF	R3	#000B
      0x80000600,  //  0009  RET	0
      0x7002003A,  //  000A  JMP		#0046
      0x1C0C050B,  //  000B  EQ	R3	R2	K11
      0x780E000A,  //  000C  JMPF	R3	#0018
      0x8C0C0118,  //  000D  GETMET	R3	R0	K24
      0x54160022,  //  000E  LDINT	R5	35
      0x5818000B,  //  000F  LDCONST	R6	K11
      0x581C000E,  //  0010  LDCONST	R7	K14
      0x7C0C0800,  //  0011  CALL	R3	4
      0x880C0106,  //  0012  GETMBR	R3	R0	K6
      0x000C070E,  //  0013  ADD	R3	R3	K14
      0x90020C03,  //  0014  SETMBR	R0	K6	R3
      0x9002110E,  //  0015  SETMBR	R0	K8	K14
      0x80000600,  //  0016  RET	0
      0x7002002D,  //  0017  JMP		#0046
      0x1C0C0566,  //  0018  EQ	R3	R2	K102
      0x780E0002,  //  0019  JMPF	R3	#001D
      0x8C0C0167,  //  001A  GETMET	R3	R0	K103
      0x7C0C0200,  //  001B  CALL	R3	1
      0x70020028,  //  001C  JMP		#0046
      0x1C0C0539,  //  001D  EQ	R3	R2	K57
      0x780E0006,  //  001E  JMPF	R3	#0026
      0x8C0C0113,  //  001F  GETMET	R3	R0	K19
      0x7C0C0200,  //  0020  CALL	R3	1
      0x1C0C071A,  //  0021  EQ	R3	R3	K26
      0x780E0002,  //  0022  JMPF	R3	#0026
      0x8C0C0168,  //  0023  GETMET	R3	R0	K104
      0x7C0C0200,  //  0024  CALL	R3	1
      0x7002001F,  //  0025  JMP		#0046
      0x8C0C0134,  //  0026  GETMET	R3	R0	K52
      0x5C140400,  //  0027  MOVE	R5	R2
      0x7C0C0400,  //  0028  CALL	R3	2
      0x740E0001,  //  0029  JMPT	R3	#002C
      0x1C0C0535,  //  002A  EQ	R3	R2	K53
      0x780E0002,  //  002B  JMPF	R3	#002F
      0x8C0C0169,  //  002C  GETMET	R3	R0	K105
      0x7C0C0200,  //  002D  CALL	R3	1
      0x70020016,  //  002E  JMP		#0046
      0x8C0C0112,  //  002F  GETMET	R3	R0	K18
      0x5C140400,  //  0030  MOVE	R5	R2
      0x7C0C0400,  //  0031  CALL	R3	2
      0x780E0002,  //  0032  JMPF	R3	#0036
      0x8C0C016A,  //  0033  GETMET	R3	R0	K106
      0x7C0C0200,  //  0034  CALL	R3	1
      0x7002000F,  //  0035  JMP		#0046
      0x1C0C056B,  //  0036  EQ	R3	R2	K107
      0x740E0001,  //  0037  JMPT	R3	#003A
      0x1C0C0563,  //  0038  EQ	R3	R2	K99
      0x780E0003,  //  0039  JMPF	R3	#003E
      0x8C0C016C,  //  003A  GETMET	R3	R0	K108
      0x5C140400,  //  003B  MOVE	R5	R2
      0x7C0C0400,  //  003C  CALL	R3	2
      0x70020007,  //  003D  JMP		#0046
      0x1C0C0537,  //  003E  EQ	R3	R2	K55
      0x780E0002,  //  003F  JMPF	R3	#0043
      0x8C0C016D,  //  0040  GETMET	R3	R0	K109
      0x7C0C0200,  //  0041  CALL	R3	1
      0x70020002,  //  0042  JMP		#0046
      0x8C0C016E,  //  0043  GETMET	R3	R0	K110
      0x5C140400,  //  0044  MOVE	R5	R2
      0x7C0C0400,  //  0045  CALL	R3	2
      0x80000000,  //  0046  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: tokenize_with_errors
********************************************************************/
be_local_closure(class_DSLLexer_tokenize_with_errors,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(tokenize_with_errors),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C04016F,  //  0000  GETMET	R1	R0	K111
      0x7C040200,  //  0001  CALL	R1	1
      0x60080013,  //  0002  GETGBL	R2	G19
      0x7C080000,  //  0003  CALL	R2	0
      0x980A5601,  //  0004  SETIDX	R2	K43	R1
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x980A0003,  //  0006  SETIDX	R2	K0	R3
      0x8C0C0101,  //  0007  GETMET	R3	R0	K1
      0x7C0C0200,  //  0008  CALL	R3	1
      0x780E0000,  //  0009  JMPF	R3	#000B
      0x500C0001,  //  000A  LDBOOL	R3	0	1
      0x500C0200,  //  000B  LDBOOL	R3	1	0
      0x980AE003,  //  000C  SETIDX	R2	K112	R3
      0x80040400,  //  000D  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_position_info
********************************************************************/
be_local_closure(class_DSLLexer_get_position_info,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_DSLLexer,     /* shared constants */
    be_str_weak(get_position_info),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x8808010D,  //  0002  GETMBR	R2	R0	K13
      0x98061A02,  //  0003  SETIDX	R1	K13	R2
      0x88080106,  //  0004  GETMBR	R2	R0	K6
      0x98060C02,  //  0005  SETIDX	R1	K6	R2
      0x88080108,  //  0006  GETMBR	R2	R0	K8
      0x98061002,  //  0007  SETIDX	R1	K8	R2
      0x8C080111,  //  0008  GETMET	R2	R0	K17
      0x7C080200,  //  0009  CALL	R2	1
      0x98062202,  //  000A  SETIDX	R1	K17	R2
      0x80040200,  //  000B  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: DSLLexer
********************************************************************/
be_local_class(DSLLexer,
    6,
    NULL,
    be_nested_map(35,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(get_errors, -1), be_const_closure(class_DSLLexer_get_errors_closure) },
        { be_const_key_weak(get_error_report, -1), be_const_closure(class_DSLLexer_get_error_report_closure) },
        { be_const_key_weak(source, 31), be_const_var(0) },
        { be_const_key_weak(advance, -1), be_const_closure(class_DSLLexer_advance_closure) },
        { be_const_key_weak(scan_number, 26), be_const_closure(class_DSLLexer_scan_number_closure) },
        { be_const_key_weak(scan_string, 22), be_const_closure(class_DSLLexer_scan_string_closure) },
        { be_const_key_weak(scan_hex_color_0x, 25), be_const_closure(class_DSLLexer_scan_hex_color_0x_closure) },
        { be_const_key_weak(add_token, -1), be_const_closure(class_DSLLexer_add_token_closure) },
        { be_const_key_weak(is_digit, -1), be_const_closure(class_DSLLexer_is_digit_closure) },
        { be_const_key_weak(at_end, -1), be_const_closure(class_DSLLexer_at_end_closure) },
        { be_const_key_weak(peek_next, 34), be_const_closure(class_DSLLexer_peek_next_closure) },
        { be_const_key_weak(check_time_suffix, -1), be_const_closure(class_DSLLexer_check_time_suffix_closure) },
        { be_const_key_weak(reset, -1), be_const_closure(class_DSLLexer_reset_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_DSLLexer_init_closure) },
        { be_const_key_weak(scan_variable_reference, 8), be_const_closure(class_DSLLexer_scan_variable_reference_closure) },
        { be_const_key_weak(tokenize_with_errors, 23), be_const_closure(class_DSLLexer_tokenize_with_errors_closure) },
        { be_const_key_weak(scan_time_suffix, 2), be_const_closure(class_DSLLexer_scan_time_suffix_closure) },
        { be_const_key_weak(column, -1), be_const_var(3) },
        { be_const_key_weak(is_alnum, -1), be_const_closure(class_DSLLexer_is_alnum_closure) },
        { be_const_key_weak(has_errors, -1), be_const_closure(class_DSLLexer_has_errors_closure) },
        { be_const_key_weak(scan_comment, 18), be_const_closure(class_DSLLexer_scan_comment_closure) },
        { be_const_key_weak(tokens, -1), be_const_var(4) },
        { be_const_key_weak(scan_identifier_or_keyword, 13), be_const_closure(class_DSLLexer_scan_identifier_or_keyword_closure) },
        { be_const_key_weak(tokenize, -1), be_const_closure(class_DSLLexer_tokenize_closure) },
        { be_const_key_weak(is_hex_digit, -1), be_const_closure(class_DSLLexer_is_hex_digit_closure) },
        { be_const_key_weak(scan_operator_or_delimiter, -1), be_const_closure(class_DSLLexer_scan_operator_or_delimiter_closure) },
        { be_const_key_weak(position, -1), be_const_var(1) },
        { be_const_key_weak(peek, 15), be_const_closure(class_DSLLexer_peek_closure) },
        { be_const_key_weak(line, -1), be_const_var(2) },
        { be_const_key_weak(add_error, 19), be_const_closure(class_DSLLexer_add_error_closure) },
        { be_const_key_weak(scan_token, -1), be_const_closure(class_DSLLexer_scan_token_closure) },
        { be_const_key_weak(is_alpha, -1), be_const_closure(class_DSLLexer_is_alpha_closure) },
        { be_const_key_weak(get_position_info, -1), be_const_closure(class_DSLLexer_get_position_info_closure) },
        { be_const_key_weak(errors, -1), be_const_var(5) },
        { be_const_key_weak(match, -1), be_const_closure(class_DSLLexer_match_closure) },
    })),
    be_str_weak(DSLLexer)
);

/********************************************************************
** Solidified function: execute
********************************************************************/
be_local_closure(execute,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(animation_dsl),
    /* K1   */  be_nested_str_weak(compile),
    }),
    be_str_weak(execute),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x6008000D,  //  0004  GETGBL	R2	G13
      0x5C0C0200,  //  0005  MOVE	R3	R1
      0x7C080200,  //  0006  CALL	R2	1
      0x5C0C0400,  //  0007  MOVE	R3	R2
      0x7C0C0000,  //  0008  CALL	R3	0
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: animation_dsl
********************************************************************/
be_local_module(animation_dsl,
    "animation_dsl",
    be_nested_map(21,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(create_dsl_runtime, 20), be_const_closure(create_dsl_runtime_closure) },
        { be_const_key_weak(DSLLexer, -1), be_const_class(be_class_DSLLexer) },
        { be_const_key_weak(create_eof_token, -1), be_const_closure(create_eof_token_closure) },
        { be_const_key_weak(tokenize_dsl_with_errors, -1), be_const_closure(tokenize_dsl_with_errors_closure) },
        { be_const_key_weak(compile_dsl, -1), be_const_closure(compile_dsl_closure) },
        { be_const_key_weak(load_file, -1), be_const_closure(load_file_closure) },
        { be_const_key_weak(compile, -1), be_const_closure(compile_dsl_source_closure) },
        { be_const_key_weak(execute, -1), be_const_closure(execute_closure) },
        { be_const_key_weak(SimpleDSLTranspiler, -1), be_const_class(be_class_SimpleDSLTranspiler) },
        { be_const_key_weak(DSLRuntime, -1), be_const_class(be_class_DSLRuntime) },
        { be_const_key_weak(VERSION, 0), be_const_int(65536) },
        { be_const_key_weak(is_keyword, 8), be_const_closure(is_keyword_closure) },
        { be_const_key_weak(is_right_associative, -1), be_const_closure(is_right_associative_closure) },
        { be_const_key_weak(tokenize_dsl, -1), be_const_closure(tokenize_dsl_closure) },
        { be_const_key_weak(is_color_name, -1), be_const_closure(is_color_name_closure) },
        { be_const_key_weak(create_error_token, 3), be_const_closure(create_error_token_closure) },
        { be_const_key_weak(get_operator_precedence, 13), be_const_closure(get_operator_precedence_closure) },
        { be_const_key_weak(create_runtime, -1), be_const_closure(create_runtime_closure) },
        { be_const_key_weak(create_newline_token, 19), be_const_closure(create_newline_token_closure) },
        { be_const_key_weak(MockEngine, -1), be_const_class(be_class_MockEngine) },
        { be_const_key_weak(Token, -1), be_const_class(be_class_Token) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(animation_dsl);
/********************************************************************/
/********************************************************************/
/* End of solidification */
