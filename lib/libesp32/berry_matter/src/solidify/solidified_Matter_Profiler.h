/* Solidification of Matter_Profiler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Profiler;

/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(Matter_Profiler_start,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(active),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(PREALLOCATED),
    /* K3   */  be_nested_str_weak(millis),
    /* K4   */  be_nested_str_weak(names),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(len),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(gc),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(start),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060000,  //  0001  JMPT	R1	#0003
      0x80000200,  //  0002  RET	0
      0x58040001,  //  0003  LDCONST	R1	K1
      0x88080102,  //  0004  GETMBR	R2	R0	K2
      0x14080202,  //  0005  LT	R2	R1	R2
      0x780A0007,  //  0006  JMPF	R2	#000F
      0x88080103,  //  0007  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0008  LDNIL	R3
      0x98080203,  //  0009  SETIDX	R2	R1	R3
      0x88080104,  //  000A  GETMBR	R2	R0	K4
      0x4C0C0000,  //  000B  LDNIL	R3
      0x98080203,  //  000C  SETIDX	R2	R1	R3
      0x00040305,  //  000D  ADD	R1	R1	K5
      0x7001FFF4,  //  000E  JMP		#0004
      0x90020D01,  //  000F  SETMBR	R0	K6	K1
      0xB80A0E00,  //  0010  GETNGBL	R2	K7
      0x8C080508,  //  0011  GETMET	R2	R2	K8
      0x7C080200,  //  0012  CALL	R2	1
      0x8C080109,  //  0013  GETMET	R2	R0	K9
      0x5810000A,  //  0014  LDCONST	R4	K10
      0x7C080400,  //  0015  CALL	R2	2
      0x80000000,  //  0016  RET	0
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(active),
    /* K1   */  be_nested_str_weak(millis),
    /* K2   */  be_nested_str_weak(resize),
    /* K3   */  be_nested_str_weak(PREALLOCATED),
    /* K4   */  be_nested_str_weak(names),
    /* K5   */  be_nested_str_weak(allocs),
    /* K6   */  be_nested_str_weak(reallocs),
    /* K7   */  be_nested_str_weak(len),
    /* K8   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
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
      0x60040012,  //  0010  GETGBL	R1	G18
      0x7C040000,  //  0011  CALL	R1	0
      0x90020A01,  //  0012  SETMBR	R0	K5	R1
      0x88040105,  //  0013  GETMBR	R1	R0	K5
      0x8C040302,  //  0014  GETMET	R1	R1	K2
      0x880C0103,  //  0015  GETMBR	R3	R0	K3
      0x7C040400,  //  0016  CALL	R1	2
      0x60040012,  //  0017  GETGBL	R1	G18
      0x7C040000,  //  0018  CALL	R1	0
      0x90020C01,  //  0019  SETMBR	R0	K6	R1
      0x88040106,  //  001A  GETMBR	R1	R0	K6
      0x8C040302,  //  001B  GETMET	R1	R1	K2
      0x880C0103,  //  001C  GETMBR	R3	R0	K3
      0x7C040400,  //  001D  CALL	R1	2
      0x90020F08,  //  001E  SETMBR	R0	K7	K8
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


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
** Solidified function: log
********************************************************************/
be_local_closure(Matter_Profiler_log,   /* name */
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
    /* K0   */  be_nested_str_weak(active),
    /* K1   */  be_nested_str_weak(debug),
    /* K2   */  be_nested_str_weak(len),
    /* K3   */  be_nested_str_weak(PREALLOCATED),
    /* K4   */  be_nested_str_weak(millis),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(names),
    /* K7   */  be_nested_str_weak(allocs),
    /* K8   */  be_nested_str_weak(reallocs),
    /* K9   */  be_const_int(1),
    }),
    be_str_weak(log),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0000,  //  0001  JMPT	R2	#0003
      0x80000400,  //  0002  RET	0
      0xA40A0200,  //  0003  IMPORT	R2	K1
      0x880C0102,  //  0004  GETMBR	R3	R0	K2
      0x88100103,  //  0005  GETMBR	R4	R0	K3
      0x28100604,  //  0006  GE	R4	R3	R4
      0x78120000,  //  0007  JMPF	R4	#0009
      0x80000800,  //  0008  RET	0
      0x88100104,  //  0009  GETMBR	R4	R0	K4
      0xB8160A00,  //  000A  GETNGBL	R5	K5
      0x8C140B04,  //  000B  GETMET	R5	R5	K4
      0x7C140200,  //  000C  CALL	R5	1
      0x98100605,  //  000D  SETIDX	R4	R3	R5
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x98100601,  //  000F  SETIDX	R4	R3	R1
      0x88100107,  //  0010  GETMBR	R4	R0	K7
      0x8C140507,  //  0011  GETMET	R5	R2	K7
      0x7C140200,  //  0012  CALL	R5	1
      0x98100605,  //  0013  SETIDX	R4	R3	R5
      0x88100108,  //  0014  GETMBR	R4	R0	K8
      0x8C140508,  //  0015  GETMET	R5	R2	K8
      0x7C140200,  //  0016  CALL	R5	1
      0x98100605,  //  0017  SETIDX	R4	R3	R5
      0x88100102,  //  0018  GETMBR	R4	R0	K2
      0x00100909,  //  0019  ADD	R4	R4	K9
      0x90020404,  //  001A  SETMBR	R0	K2	R4
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dump
********************************************************************/
be_local_closure(Matter_Profiler_dump,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(active),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(_X3C_X2D_X2Dend_X2D_X2D_X3E),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20Profiler_X20dump_X3A),
    /* K5   */  be_nested_str_weak(millis),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(allocs),
    /* K8   */  be_nested_str_weak(reallocs),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(len),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X254i_X20_X5B_X254i_X5D_X27_X25s_X27),
    /* K12  */  be_nested_str_weak(names),
    }),
    be_str_weak(dump),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
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
      0x880C0107,  //  000D  GETMBR	R3	R0	K7
      0x940C0706,  //  000E  GETIDX	R3	R3	K6
      0x88100108,  //  000F  GETMBR	R4	R0	K8
      0x94100906,  //  0010  GETIDX	R4	R4	K6
      0x58140009,  //  0011  LDCONST	R5	K9
      0x8818010A,  //  0012  GETMBR	R6	R0	K10
      0x14180A06,  //  0013  LT	R6	R5	R6
      0x781A0010,  //  0014  JMPF	R6	#0026
      0xB81A0600,  //  0015  GETNGBL	R6	K3
      0x8C180D01,  //  0016  GETMET	R6	R6	K1
      0x60200018,  //  0017  GETGBL	R8	G24
      0x5824000B,  //  0018  LDCONST	R9	K11
      0x88280105,  //  0019  GETMBR	R10	R0	K5
      0x94281405,  //  001A  GETIDX	R10	R10	R5
      0x04281402,  //  001B  SUB	R10	R10	R2
      0x882C0107,  //  001C  GETMBR	R11	R0	K7
      0x942C1605,  //  001D  GETIDX	R11	R11	R5
      0x042C1603,  //  001E  SUB	R11	R11	R3
      0x8830010C,  //  001F  GETMBR	R12	R0	K12
      0x94301805,  //  0020  GETIDX	R12	R12	R5
      0x7C200800,  //  0021  CALL	R8	4
      0x5C240200,  //  0022  MOVE	R9	R1
      0x7C180600,  //  0023  CALL	R6	3
      0x00140B09,  //  0024  ADD	R5	R5	K9
      0x7001FFEB,  //  0025  JMP		#0012
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Profiler
********************************************************************/
be_local_class(Matter_Profiler,
    6,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(dump, 8), be_const_closure(Matter_Profiler_dump_closure) },
        { be_const_key_weak(millis, 5), be_const_var(0) },
        { be_const_key_weak(reallocs, -1), be_const_var(4) },
        { be_const_key_weak(init, 1), be_const_closure(Matter_Profiler_init_closure) },
        { be_const_key_weak(len, -1), be_const_var(5) },
        { be_const_key_weak(active, -1), be_const_var(2) },
        { be_const_key_weak(set_active, -1), be_const_closure(Matter_Profiler_set_active_closure) },
        { be_const_key_weak(names, -1), be_const_var(1) },
        { be_const_key_weak(allocs, 10), be_const_var(3) },
        { be_const_key_weak(log, -1), be_const_closure(Matter_Profiler_log_closure) },
        { be_const_key_weak(PREALLOCATED, -1), be_const_int(50) },
        { be_const_key_weak(start, 0), be_const_closure(Matter_Profiler_start_closure) },
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
