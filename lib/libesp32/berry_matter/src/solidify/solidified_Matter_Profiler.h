/* Solidification of Matter_Profiler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Profiler;

/********************************************************************
** Solidified function: set_active
********************************************************************/
be_local_closure(Matter_Profiler_set_active,   /* name */
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
    /* K0   */  be_nested_str_weak(active),
    }),
    be_str_weak(set_active),
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
** Solidified function: start
********************************************************************/
be_local_closure(Matter_Profiler_start,   /* name */
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
    /* K0   */  be_nested_str_weak(active),
    /* K1   */  be_nested_str_weak(millis),
    /* K2   */  be_nested_str_weak(resize),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(names),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(start),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060000,  //  0001  JMPT	R1	#0003
      0x80000200,  //  0002  RET	0
      0x88040101,  //  0003  GETMBR	R1	R0	K1
      0x8C040302,  //  0004  GETMET	R1	R1	K2
      0x580C0003,  //  0005  LDCONST	R3	K3
      0x7C040400,  //  0006  CALL	R1	2
      0x88040104,  //  0007  GETMBR	R1	R0	K4
      0x8C040302,  //  0008  GETMET	R1	R1	K2
      0x580C0003,  //  0009  LDCONST	R3	K3
      0x7C040400,  //  000A  CALL	R1	2
      0x8C040105,  //  000B  GETMET	R1	R0	K5
      0x580C0006,  //  000C  LDCONST	R3	K6
      0x7C040400,  //  000D  CALL	R1	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: log
********************************************************************/
be_local_closure(Matter_Profiler_log,   /* name */
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
    /* K0   */  be_nested_str_weak(active),
    /* K1   */  be_nested_str_weak(millis),
    /* K2   */  be_nested_str_weak(push),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(names),
    }),
    be_str_weak(log),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0000,  //  0001  JMPT	R2	#0003
      0x80000400,  //  0002  RET	0
      0x88080101,  //  0003  GETMBR	R2	R0	K1
      0x8C080502,  //  0004  GETMET	R2	R2	K2
      0xB8120600,  //  0005  GETNGBL	R4	K3
      0x8C100901,  //  0006  GETMET	R4	R4	K1
      0x7C100200,  //  0007  CALL	R4	1
      0x7C080400,  //  0008  CALL	R2	2
      0x88080104,  //  0009  GETMBR	R2	R0	K4
      0x8C080502,  //  000A  GETMET	R2	R2	K2
      0x5C100200,  //  000B  MOVE	R4	R1
      0x7C080400,  //  000C  CALL	R2	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Profiler_init,   /* name */
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
    /* K0   */  be_nested_str_weak(active),
    /* K1   */  be_nested_str_weak(millis),
    /* K2   */  be_nested_str_weak(resize),
    /* K3   */  be_nested_str_weak(PREALLOCATED),
    /* K4   */  be_nested_str_weak(names),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x60040012,  //  0002  GETGBL	R1	G18
      0x7C040000,  //  0003  CALL	R1	0
      0x90020201,  //  0004  SETMBR	R0	K1	R1
      0x88040101,  //  0005  GETMBR	R1	R0	K1
      0x8C040302,  //  0006  GETMET	R1	R1	K2
      0x880C0103,  //  0007  GETMBR	R3	R0	K3
      0x7C040400,  //  0008  CALL	R1	2
      0x60040012,  //  0009  GETGBL	R1	G18
      0x7C040000,  //  000A  CALL	R1	0
      0x90020801,  //  000B  SETMBR	R0	K4	R1
      0x88040104,  //  000C  GETMBR	R1	R0	K4
      0x8C040302,  //  000D  GETMET	R1	R1	K2
      0x880C0103,  //  000E  GETMBR	R3	R0	K3
      0x7C040400,  //  000F  CALL	R1	2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dump
********************************************************************/
be_local_closure(Matter_Profiler_dump,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(active),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(_X3C_X2D_X2Dend_X2D_X2D_X3E),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20Profiler_X20dump_X3A),
    /* K5   */  be_nested_str_weak(millis),
    /* K6   */  be_const_int(0),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X254i_X20_X27_X25s_X27),
    /* K9   */  be_nested_str_weak(names),
    }),
    be_str_weak(dump),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0000,  //  0001  JMPT	R2	#0003
      0x80000400,  //  0002  RET	0
      0x8C080101,  //  0003  GETMET	R2	R0	K1
      0x58100002,  //  0004  LDCONST	R4	K2
      0x7C080400,  //  0005  CALL	R2	2
      0xB80A0600,  //  0006  GETNGBL	R2	K3
      0x8C080501,  //  0007  GETMET	R2	R2	K1
      0x58100004,  //  0008  LDCONST	R4	K4
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C080600,  //  000A  CALL	R2	3
      0x88080105,  //  000B  GETMBR	R2	R0	K5
      0x94080506,  //  000C  GETIDX	R2	R2	K6
      0x580C0007,  //  000D  LDCONST	R3	K7
      0x6010000C,  //  000E  GETGBL	R4	G12
      0x88140105,  //  000F  GETMBR	R5	R0	K5
      0x7C100200,  //  0010  CALL	R4	1
      0x14100604,  //  0011  LT	R4	R3	R4
      0x7812000D,  //  0012  JMPF	R4	#0021
      0xB8120600,  //  0013  GETNGBL	R4	K3
      0x8C100901,  //  0014  GETMET	R4	R4	K1
      0x60180018,  //  0015  GETGBL	R6	G24
      0x581C0008,  //  0016  LDCONST	R7	K8
      0x88200105,  //  0017  GETMBR	R8	R0	K5
      0x94201003,  //  0018  GETIDX	R8	R8	R3
      0x04201002,  //  0019  SUB	R8	R8	R2
      0x88240109,  //  001A  GETMBR	R9	R0	K9
      0x94241203,  //  001B  GETIDX	R9	R9	R3
      0x7C180600,  //  001C  CALL	R6	3
      0x5C1C0200,  //  001D  MOVE	R7	R1
      0x7C100600,  //  001E  CALL	R4	3
      0x000C0707,  //  001F  ADD	R3	R3	K7
      0x7001FFEC,  //  0020  JMP		#000E
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Profiler
********************************************************************/
be_local_class(Matter_Profiler,
    3,
    NULL,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_active, 5), be_const_closure(Matter_Profiler_set_active_closure) },
        { be_const_key_weak(millis, -1), be_const_var(0) },
        { be_const_key_weak(start, 4), be_const_closure(Matter_Profiler_start_closure) },
        { be_const_key_weak(active, 1), be_const_var(2) },
        { be_const_key_weak(PREALLOCATED, -1), be_const_int(50) },
        { be_const_key_weak(log, -1), be_const_closure(Matter_Profiler_log_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Profiler_init_closure) },
        { be_const_key_weak(names, -1), be_const_var(1) },
        { be_const_key_weak(dump, -1), be_const_closure(Matter_Profiler_dump_closure) },
    })),
    be_str_weak(Matter_Profiler)
);
/*******************************************************************/

void be_load_Matter_Profiler_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Profiler);
    be_setglobal(vm, "Matter_Profiler");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
