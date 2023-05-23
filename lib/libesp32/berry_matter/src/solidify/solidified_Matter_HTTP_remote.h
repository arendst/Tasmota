/* Solidification of Matter_HTTP_remote.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_HTTP_remote;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_HTTP_remote_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60140003,  //  0000  GETGBL	R5	G3
      0x5C180000,  //  0001  MOVE	R6	R0
      0x7C140200,  //  0002  CALL	R5	1
      0x8C140B00,  //  0003  GETMET	R5	R5	K0
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x5C240600,  //  0006  MOVE	R9	R3
      0x5C280800,  //  0007  MOVE	R10	R4
      0x7C140A00,  //  0008  CALL	R5	5
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin_sync
********************************************************************/
be_local_closure(Matter_HTTP_remote_begin_sync,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20HTTP_X20sync_X20request_X20_X27http_X3A_X2F_X2F_X25s_X3A_X25i_X25s_X27),
    /* K5   */  be_nested_str_weak(addr),
    /* K6   */  be_nested_str_weak(port),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(begin_sync),
    }),
    be_str_weak(begin_sync),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x8C180703,  //  0003  GETMET	R6	R3	K3
      0x58200004,  //  0004  LDCONST	R8	K4
      0x88240105,  //  0005  GETMBR	R9	R0	K5
      0x88280106,  //  0006  GETMBR	R10	R0	K6
      0x5C2C0200,  //  0007  MOVE	R11	R1
      0x7C180A00,  //  0008  CALL	R6	5
      0x581C0007,  //  0009  LDCONST	R7	K7
      0x7C100600,  //  000A  CALL	R4	3
      0x60100003,  //  000B  GETGBL	R4	G3
      0x5C140000,  //  000C  MOVE	R5	R0
      0x7C100200,  //  000D  CALL	R4	1
      0x8C100908,  //  000E  GETMET	R4	R4	K8
      0x5C180200,  //  000F  MOVE	R6	R1
      0x5C1C0400,  //  0010  MOVE	R7	R2
      0x7C100600,  //  0011  CALL	R4	3
      0x80040800,  //  0012  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_finished
********************************************************************/
be_local_closure(Matter_HTTP_remote_event_http_finished,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(payload),
    /* K2   */  be_nested_str_weak(nil),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(_X2E_X2E_X2E),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(format),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20HTTP_X20response_X20in_X20_X25i_X20ms_X3A_X20_X27_X25s_X27),
    /* K9   */  be_nested_str_weak(millis),
    /* K10  */  be_nested_str_weak(time_start),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(cb),
    /* K13  */  be_nested_str_weak(http_status),
    }),
    be_str_weak(event_http_finished),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x88080101,  //  0005  GETMBR	R2	R0	K1
      0x70020000,  //  0006  JMP		#0008
      0x58080002,  //  0007  LDCONST	R2	K2
      0x600C000C,  //  0008  GETGBL	R3	G12
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x5412001D,  //  000B  LDINT	R4	30
      0x240C0604,  //  000C  GT	R3	R3	R4
      0x780E0004,  //  000D  JMPF	R3	#0013
      0x540E001C,  //  000E  LDINT	R3	29
      0x400E0603,  //  000F  CONNECT	R3	K3	R3
      0x940C0403,  //  0010  GETIDX	R3	R2	R3
      0x000C0704,  //  0011  ADD	R3	R3	K4
      0x5C080600,  //  0012  MOVE	R2	R3
      0xB80E0A00,  //  0013  GETNGBL	R3	K5
      0x8C0C0706,  //  0014  GETMET	R3	R3	K6
      0x8C140307,  //  0015  GETMET	R5	R1	K7
      0x581C0008,  //  0016  LDCONST	R7	K8
      0xB8220A00,  //  0017  GETNGBL	R8	K5
      0x8C201109,  //  0018  GETMET	R8	R8	K9
      0x7C200200,  //  0019  CALL	R8	1
      0x8824010A,  //  001A  GETMBR	R9	R0	K10
      0x04201009,  //  001B  SUB	R8	R8	R9
      0x5C240400,  //  001C  MOVE	R9	R2
      0x7C140800,  //  001D  CALL	R5	4
      0x5818000B,  //  001E  LDCONST	R6	K11
      0x7C0C0600,  //  001F  CALL	R3	3
      0x880C010C,  //  0020  GETMBR	R3	R0	K12
      0x780E0003,  //  0021  JMPF	R3	#0026
      0x8C0C010C,  //  0022  GETMET	R3	R0	K12
      0x8814010D,  //  0023  GETMBR	R5	R0	K13
      0x88180101,  //  0024  GETMBR	R6	R0	K1
      0x7C0C0600,  //  0025  CALL	R3	3
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_failed
********************************************************************/
be_local_closure(Matter_HTTP_remote_event_http_failed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20HTTP_X20failed),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str_weak(cb),
    /* K5   */  be_nested_str_weak(http_status),
    }),
    be_str_weak(event_http_failed),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x7C040600,  //  0004  CALL	R1	3
      0x88040104,  //  0005  GETMBR	R1	R0	K4
      0x78060003,  //  0006  JMPF	R1	#000B
      0x8C040104,  //  0007  GETMET	R1	R0	K4
      0x880C0105,  //  0008  GETMBR	R3	R0	K5
      0x4C100000,  //  0009  LDNIL	R4
      0x7C040600,  //  000A  CALL	R1	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_cb
********************************************************************/
be_local_closure(Matter_HTTP_remote_set_cb,   /* name */
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
    /* K0   */  be_nested_str_weak(cb),
    }),
    be_str_weak(set_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(Matter_HTTP_remote_begin,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20HTTP_X20async_X20request_X20_X27http_X3A_X2F_X2F_X25s_X3A_X25i_X25s_X27),
    /* K5   */  be_nested_str_weak(addr),
    /* K6   */  be_nested_str_weak(port),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(begin),
    }),
    be_str_weak(begin),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88240106,  //  0006  GETMBR	R9	R0	K6
      0x5C280200,  //  0007  MOVE	R10	R1
      0x7C140A00,  //  0008  CALL	R5	5
      0x58180007,  //  0009  LDCONST	R6	K7
      0x7C0C0600,  //  000A  CALL	R3	3
      0x600C0003,  //  000B  GETGBL	R3	G3
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C0708,  //  000E  GETMET	R3	R3	K8
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80040600,  //  0011  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_timeout
********************************************************************/
be_local_closure(Matter_HTTP_remote_event_http_timeout,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20HTTP_X20timeout),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str_weak(cb),
    /* K5   */  be_nested_str_weak(http_status),
    }),
    be_str_weak(event_http_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x7C040600,  //  0004  CALL	R1	3
      0x88040104,  //  0005  GETMBR	R1	R0	K4
      0x78060003,  //  0006  JMPF	R1	#000B
      0x8C040104,  //  0007  GETMET	R1	R0	K4
      0x880C0105,  //  0008  GETMBR	R3	R0	K5
      0x4C100000,  //  0009  LDNIL	R4
      0x7C040600,  //  000A  CALL	R1	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_HTTP_remote
********************************************************************/
extern const bclass be_class_Matter_HTTP_async;
be_local_class(Matter_HTTP_remote,
    1,
    &be_class_Matter_HTTP_async,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(cb, 1), be_const_var(0) },
        { be_const_key_weak(event_http_timeout, -1), be_const_closure(Matter_HTTP_remote_event_http_timeout_closure) },
        { be_const_key_weak(event_http_failed, -1), be_const_closure(Matter_HTTP_remote_event_http_failed_closure) },
        { be_const_key_weak(init, 7), be_const_closure(Matter_HTTP_remote_init_closure) },
        { be_const_key_weak(begin_sync, 2), be_const_closure(Matter_HTTP_remote_begin_sync_closure) },
        { be_const_key_weak(set_cb, -1), be_const_closure(Matter_HTTP_remote_set_cb_closure) },
        { be_const_key_weak(begin, -1), be_const_closure(Matter_HTTP_remote_begin_closure) },
        { be_const_key_weak(event_http_finished, -1), be_const_closure(Matter_HTTP_remote_event_http_finished_closure) },
    })),
    be_str_weak(Matter_HTTP_remote)
);
/*******************************************************************/

void be_load_Matter_HTTP_remote_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_HTTP_remote);
    be_setglobal(vm, "Matter_HTTP_remote");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
