/* Solidification of Matter_TCP_async.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_TCP_async;

/********************************************************************
** Solidified function: read
********************************************************************/
be_local_closure(Matter_TCP_async_read,   /* name */
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
    /* K0   */  be_nested_str_weak(tcp_connected),
    /* K1   */  be_nested_str_weak(tcp),
    /* K2   */  be_nested_str_weak(read),
    }),
    be_str_weak(read),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060003,  //  0001  JMPF	R1	#0006
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x80040200,  //  0005  RET	1	R1
      0x4C040000,  //  0006  LDNIL	R1
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(Matter_TCP_async_begin,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(reset),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(wifi),
    /* K3   */  be_nested_str_weak(up),
    /* K4   */  be_nested_str_weak(eth),
    /* K5   */  be_nested_str_weak(time_start),
    /* K6   */  be_nested_str_weak(millis),
    /* K7   */  be_nested_str_weak(status),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(tcp),
    /* K10  */  be_nested_str_weak(connect),
    /* K11  */  be_nested_str_weak(addr),
    /* K12  */  be_nested_str_weak(port),
    /* K13  */  be_nested_str_weak(fast_loop),
    /* K14  */  be_nested_str_weak(remove_fast_loop),
    /* K15  */  be_nested_str_weak(add_fast_loop),
    /* K16  */  be_nested_str_weak(add_driver),
    /* K17  */  be_nested_str_weak(log),
    /* K18  */  be_nested_str_weak(BRY_X3A_X20failed_X20to_X20resolve_X20_X5B_X25s_X5D_X3A_X25i),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(close),
    /* K21  */  be_nested_str_weak(tcp_connected),
    /* K22  */  be_nested_str_weak(event_dnsfailed),
    }),
    be_str_weak(begin),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0xB8060200,  //  0002  GETNGBL	R1	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x94040303,  //  0005  GETIDX	R1	R1	K3
      0x74060006,  //  0006  JMPT	R1	#000E
      0xB8060200,  //  0007  GETNGBL	R1	K1
      0x8C040304,  //  0008  GETMET	R1	R1	K4
      0x7C040200,  //  0009  CALL	R1	1
      0x94040303,  //  000A  GETIDX	R1	R1	K3
      0x74060001,  //  000B  JMPT	R1	#000E
      0x4C040000,  //  000C  LDNIL	R1
      0x80040200,  //  000D  RET	1	R1
      0xB8060200,  //  000E  GETNGBL	R1	K1
      0x8C040306,  //  000F  GETMET	R1	R1	K6
      0x7C040200,  //  0010  CALL	R1	1
      0x90020A01,  //  0011  SETMBR	R0	K5	R1
      0x90020F08,  //  0012  SETMBR	R0	K7	K8
      0x88040109,  //  0013  GETMBR	R1	R0	K9
      0x8C04030A,  //  0014  GETMET	R1	R1	K10
      0x880C010B,  //  0015  GETMBR	R3	R0	K11
      0x8810010C,  //  0016  GETMBR	R4	R0	K12
      0x7C040600,  //  0017  CALL	R1	3
      0x78060011,  //  0018  JMPF	R1	#002B
      0x8804010D,  //  0019  GETMBR	R1	R0	K13
      0x78060008,  //  001A  JMPF	R1	#0024
      0xB8060200,  //  001B  GETNGBL	R1	K1
      0x8C04030E,  //  001C  GETMET	R1	R1	K14
      0x880C010D,  //  001D  GETMBR	R3	R0	K13
      0x7C040400,  //  001E  CALL	R1	2
      0xB8060200,  //  001F  GETNGBL	R1	K1
      0x8C04030F,  //  0020  GETMET	R1	R1	K15
      0x880C010D,  //  0021  GETMBR	R3	R0	K13
      0x7C040400,  //  0022  CALL	R1	2
      0x70020003,  //  0023  JMP		#0028
      0xB8060200,  //  0024  GETNGBL	R1	K1
      0x8C040310,  //  0025  GETMET	R1	R1	K16
      0x5C0C0000,  //  0026  MOVE	R3	R0
      0x7C040400,  //  0027  CALL	R1	2
      0x50040200,  //  0028  LDBOOL	R1	1	0
      0x80040200,  //  0029  RET	1	R1
      0x70020012,  //  002A  JMP		#003E
      0xB8060200,  //  002B  GETNGBL	R1	K1
      0x8C040311,  //  002C  GETMET	R1	R1	K17
      0x600C0018,  //  002D  GETGBL	R3	G24
      0x58100012,  //  002E  LDCONST	R4	K18
      0x8814010B,  //  002F  GETMBR	R5	R0	K11
      0x8818010C,  //  0030  GETMBR	R6	R0	K12
      0x7C0C0600,  //  0031  CALL	R3	3
      0x58100013,  //  0032  LDCONST	R4	K19
      0x7C040600,  //  0033  CALL	R1	3
      0x8C040114,  //  0034  GETMET	R1	R0	K20
      0x7C040200,  //  0035  CALL	R1	1
      0x5405FFFE,  //  0036  LDINT	R1	-1
      0x90020E01,  //  0037  SETMBR	R0	K7	R1
      0x50040000,  //  0038  LDBOOL	R1	0	0
      0x90022A01,  //  0039  SETMBR	R0	K21	R1
      0x8C040116,  //  003A  GETMET	R1	R0	K22
      0x7C040200,  //  003B  CALL	R1	1
      0x50040000,  //  003C  LDBOOL	R1	0	0
      0x80040200,  //  003D  RET	1	R1
      0x80000000,  //  003E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: readbytes
********************************************************************/
be_local_closure(Matter_TCP_async_readbytes,   /* name */
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
    /* K0   */  be_nested_str_weak(tcp_connected),
    /* K1   */  be_nested_str_weak(tcp),
    /* K2   */  be_nested_str_weak(readbytes),
    }),
    be_str_weak(readbytes),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060003,  //  0001  JMPF	R1	#0006
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x80040200,  //  0005  RET	1	R1
      0x4C040000,  //  0006  LDNIL	R1
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_closed
********************************************************************/
be_local_closure(Matter_TCP_async_event_closed,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(event_closed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: available
********************************************************************/
be_local_closure(Matter_TCP_async_available,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tcp_connected),
    /* K1   */  be_nested_str_weak(tcp),
    /* K2   */  be_nested_str_weak(available),
    /* K3   */  be_const_int(0),
    }),
    be_str_weak(available),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060003,  //  0001  JMPF	R1	#0006
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x80040200,  //  0005  RET	1	R1
      0x80060600,  //  0006  RET	1	K3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_listening
********************************************************************/
be_local_closure(Matter_TCP_async_event_listening,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(event_listening),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_timeout
********************************************************************/
be_local_closure(Matter_TCP_async_get_timeout,   /* name */
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
    /* K0   */  be_nested_str_weak(timeout),
    }),
    be_str_weak(get_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_TCP_async_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(loop),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(TIMEOUT),
    /* K1   */  be_nested_str_weak(addr),
    /* K2   */  be_nested_str_weak(port),
    /* K3   */  be_nested_str_weak(timeout),
    /* K4   */  be_nested_str_weak(tcp),
    /* K5   */  be_nested_str_weak(tcpclientasync),
    /* K6   */  be_nested_str_weak(fast_loop),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140605,  //  0001  EQ	R5	R3	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x4C140000,  //  0004  LDNIL	R5
      0x1C140405,  //  0005  EQ	R5	R2	R5
      0x78160000,  //  0006  JMPF	R5	#0008
      0x540A004F,  //  0007  LDINT	R2	80
      0x60140008,  //  0008  GETGBL	R5	G8
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C140200,  //  000A  CALL	R5	1
      0x90020205,  //  000B  SETMBR	R0	K1	R5
      0x60140009,  //  000C  GETGBL	R5	G9
      0x5C180400,  //  000D  MOVE	R6	R2
      0x7C140200,  //  000E  CALL	R5	1
      0x90020405,  //  000F  SETMBR	R0	K2	R5
      0x90020603,  //  0010  SETMBR	R0	K3	R3
      0xB8160A00,  //  0011  GETNGBL	R5	K5
      0x7C140000,  //  0012  CALL	R5	0
      0x90020805,  //  0013  SETMBR	R0	K4	R5
      0x78120001,  //  0014  JMPF	R4	#0017
      0x84140000,  //  0015  CLOSURE	R5	P0
      0x90020C05,  //  0016  SETMBR	R0	K6	R5
      0xA0000000,  //  0017  CLOSE	R0
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(Matter_TCP_async_every_50ms,   /* name */
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
    /* K0   */  be_nested_str_weak(loop),
    }),
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_timeout
********************************************************************/
be_local_closure(Matter_TCP_async_event_timeout,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(event_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_available
********************************************************************/
be_local_closure(Matter_TCP_async_event_available,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(event_available),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write
********************************************************************/
be_local_closure(Matter_TCP_async_write,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tcp_connected),
    /* K1   */  be_nested_str_weak(tcp),
    /* K2   */  be_nested_str_weak(write),
    /* K3   */  be_const_int(0),
    }),
    be_str_weak(write),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0004,  //  0001  JMPF	R2	#0007
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80040400,  //  0006  RET	1	R2
      0x80060600,  //  0007  RET	1	K3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_established
********************************************************************/
be_local_closure(Matter_TCP_async_event_established,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(event_established),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(Matter_TCP_async_reset,   /* name */
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
    /* K0   */  be_nested_str_weak(tcp),
    /* K1   */  be_nested_str_weak(close),
    /* K2   */  be_nested_str_weak(tcp_connected),
    }),
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x4C040000,  //  0003  LDNIL	R1
      0x90020401,  //  0004  SETMBR	R0	K2	R1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: loop
********************************************************************/
be_local_closure(Matter_TCP_async_loop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(tcp_connected),
    /* K1   */  be_nested_str_weak(status),
    /* K2   */  be_nested_str_weak(close),
    /* K3   */  be_nested_str_weak(tcp),
    /* K4   */  be_nested_str_weak(connected),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(event_established),
    /* K7   */  be_nested_str_weak(event_refused),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(millis),
    /* K10  */  be_nested_str_weak(time_start),
    /* K11  */  be_nested_str_weak(timeout),
    /* K12  */  be_nested_str_weak(event_timeout),
    /* K13  */  be_nested_str_weak(event_closed),
    /* K14  */  be_nested_str_weak(available),
    /* K15  */  be_nested_str_weak(event_available),
    /* K16  */  be_nested_str_weak(listening),
    /* K17  */  be_nested_str_weak(event_listening),
    }),
    be_str_weak(loop),
    &be_const_str_solidified,
    ( &(const binstruction[89]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C080403,  //  0003  EQ	R2	R2	R3
      0x780A0002,  //  0004  JMPF	R2	#0008
      0x8C080102,  //  0005  GETMET	R2	R0	K2
      0x7C080200,  //  0006  CALL	R2	1
      0x80000400,  //  0007  RET	0
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x8C080504,  //  0009  GETMET	R2	R2	K4
      0x7C080200,  //  000A  CALL	R2	1
      0x90020002,  //  000B  SETMBR	R0	K0	R2
      0x4C080000,  //  000C  LDNIL	R2
      0x1C080202,  //  000D  EQ	R2	R1	R2
      0x780A0021,  //  000E  JMPF	R2	#0031
      0x88080100,  //  000F  GETMBR	R2	R0	K0
      0x500C0200,  //  0010  LDBOOL	R3	1	0
      0x1C080403,  //  0011  EQ	R2	R2	R3
      0x780A0003,  //  0012  JMPF	R2	#0017
      0x90020305,  //  0013  SETMBR	R0	K1	K5
      0x8C080106,  //  0014  GETMET	R2	R0	K6
      0x7C080200,  //  0015  CALL	R2	1
      0x70020019,  //  0016  JMP		#0031
      0x88080100,  //  0017  GETMBR	R2	R0	K0
      0x500C0000,  //  0018  LDBOOL	R3	0	0
      0x1C080403,  //  0019  EQ	R2	R2	R3
      0x780A0007,  //  001A  JMPF	R2	#0023
      0x5409FFFD,  //  001B  LDINT	R2	-2
      0x90020202,  //  001C  SETMBR	R0	K1	R2
      0x8C080107,  //  001D  GETMET	R2	R0	K7
      0x7C080200,  //  001E  CALL	R2	1
      0x8C080102,  //  001F  GETMET	R2	R0	K2
      0x7C080200,  //  0020  CALL	R2	1
      0x80000400,  //  0021  RET	0
      0x7002000D,  //  0022  JMP		#0031
      0xB80A1000,  //  0023  GETNGBL	R2	K8
      0x8C080509,  //  0024  GETMET	R2	R2	K9
      0x7C080200,  //  0025  CALL	R2	1
      0x880C010A,  //  0026  GETMBR	R3	R0	K10
      0x04080403,  //  0027  SUB	R2	R2	R3
      0x880C010B,  //  0028  GETMBR	R3	R0	K11
      0x24080403,  //  0029  GT	R2	R2	R3
      0x780A0005,  //  002A  JMPF	R2	#0031
      0x5409FFFC,  //  002B  LDINT	R2	-3
      0x90020202,  //  002C  SETMBR	R0	K1	R2
      0x50080000,  //  002D  LDBOOL	R2	0	0
      0x90020002,  //  002E  SETMBR	R0	K0	R2
      0x8C08010C,  //  002F  GETMET	R2	R0	K12
      0x7C080200,  //  0030  CALL	R2	1
      0xB80A1000,  //  0031  GETNGBL	R2	K8
      0x8C080509,  //  0032  GETMET	R2	R2	K9
      0x7C080200,  //  0033  CALL	R2	1
      0x880C010A,  //  0034  GETMBR	R3	R0	K10
      0x04080403,  //  0035  SUB	R2	R2	R3
      0x880C010B,  //  0036  GETMBR	R3	R0	K11
      0x24080403,  //  0037  GT	R2	R2	R3
      0x780A0006,  //  0038  JMPF	R2	#0040
      0x8C080102,  //  0039  GETMET	R2	R0	K2
      0x7C080200,  //  003A  CALL	R2	1
      0x5409FFFC,  //  003B  LDINT	R2	-3
      0x90020202,  //  003C  SETMBR	R0	K1	R2
      0x8C08010C,  //  003D  GETMET	R2	R0	K12
      0x7C080200,  //  003E  CALL	R2	1
      0x80000400,  //  003F  RET	0
      0x88080100,  //  0040  GETMBR	R2	R0	K0
      0x500C0000,  //  0041  LDBOOL	R3	0	0
      0x1C080403,  //  0042  EQ	R2	R2	R3
      0x780A0007,  //  0043  JMPF	R2	#004C
      0x50080200,  //  0044  LDBOOL	R2	1	0
      0x1C080202,  //  0045  EQ	R2	R1	R2
      0x780A0001,  //  0046  JMPF	R2	#0049
      0x8C08010D,  //  0047  GETMET	R2	R0	K13
      0x7C080200,  //  0048  CALL	R2	1
      0x8C080102,  //  0049  GETMET	R2	R0	K2
      0x7C080200,  //  004A  CALL	R2	1
      0x80000400,  //  004B  RET	0
      0x88080103,  //  004C  GETMBR	R2	R0	K3
      0x8C08050E,  //  004D  GETMET	R2	R2	K14
      0x7C080200,  //  004E  CALL	R2	1
      0x780A0001,  //  004F  JMPF	R2	#0052
      0x8C08010F,  //  0050  GETMET	R2	R0	K15
      0x7C080200,  //  0051  CALL	R2	1
      0x88080103,  //  0052  GETMBR	R2	R0	K3
      0x8C080510,  //  0053  GETMET	R2	R2	K16
      0x7C080200,  //  0054  CALL	R2	1
      0x780A0001,  //  0055  JMPF	R2	#0058
      0x8C080111,  //  0056  GETMET	R2	R0	K17
      0x7C080200,  //  0057  CALL	R2	1
      0x80000000,  //  0058  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: listening
********************************************************************/
be_local_closure(Matter_TCP_async_listening,   /* name */
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
    /* K0   */  be_nested_str_weak(tcp_connected),
    /* K1   */  be_nested_str_weak(tcp),
    /* K2   */  be_nested_str_weak(listening),
    }),
    be_str_weak(listening),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060003,  //  0001  JMPF	R1	#0006
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x80040200,  //  0005  RET	1	R1
      0x50040000,  //  0006  LDBOOL	R1	0	0
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_timeout
********************************************************************/
be_local_closure(Matter_TCP_async_set_timeout,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(TIMEOUT),
    /* K1   */  be_nested_str_weak(timeout),
    }),
    be_str_weak(set_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x90020201,  //  0004  SETMBR	R0	K1	R1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_dnsfailed
********************************************************************/
be_local_closure(Matter_TCP_async_event_dnsfailed,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(event_dnsfailed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: close
********************************************************************/
be_local_closure(Matter_TCP_async_close,   /* name */
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
    /* K0   */  be_nested_str_weak(tcp),
    /* K1   */  be_nested_str_weak(close),
    /* K2   */  be_nested_str_weak(fast_loop),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(remove_fast_loop),
    /* K5   */  be_nested_str_weak(remove_driver),
    /* K6   */  be_nested_str_weak(tcp_connected),
    /* K7   */  be_nested_str_weak(event_closed),
    }),
    be_str_weak(close),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x78060004,  //  0004  JMPF	R1	#000A
      0xB8060600,  //  0005  GETNGBL	R1	K3
      0x8C040304,  //  0006  GETMET	R1	R1	K4
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0x7C040400,  //  0008  CALL	R1	2
      0x70020003,  //  0009  JMP		#000E
      0xB8060600,  //  000A  GETNGBL	R1	K3
      0x8C040305,  //  000B  GETMET	R1	R1	K5
      0x5C0C0000,  //  000C  MOVE	R3	R0
      0x7C040400,  //  000D  CALL	R1	2
      0x88040106,  //  000E  GETMBR	R1	R0	K6
      0x50080200,  //  000F  LDBOOL	R2	1	0
      0x1C040202,  //  0010  EQ	R1	R1	R2
      0x78060001,  //  0011  JMPF	R1	#0014
      0x8C040107,  //  0012  GETMET	R1	R0	K7
      0x7C040200,  //  0013  CALL	R1	1
      0x50040000,  //  0014  LDBOOL	R1	0	0
      0x90020C01,  //  0015  SETMBR	R0	K6	R1
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_refused
********************************************************************/
be_local_closure(Matter_TCP_async_event_refused,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(event_refused),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_TCP_async
********************************************************************/
be_local_class(Matter_TCP_async,
    8,
    NULL,
    be_nested_map(29,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read, 13), be_const_closure(Matter_TCP_async_read_closure) },
        { be_const_key_weak(addr, -1), be_const_var(0) },
        { be_const_key_weak(event_refused, -1), be_const_closure(Matter_TCP_async_event_refused_closure) },
        { be_const_key_weak(begin, -1), be_const_closure(Matter_TCP_async_begin_closure) },
        { be_const_key_weak(readbytes, -1), be_const_closure(Matter_TCP_async_readbytes_closure) },
        { be_const_key_weak(event_closed, 11), be_const_closure(Matter_TCP_async_event_closed_closure) },
        { be_const_key_weak(event_listening, -1), be_const_closure(Matter_TCP_async_event_listening_closure) },
        { be_const_key_weak(tcp_connected, -1), be_const_var(5) },
        { be_const_key_weak(get_timeout, -1), be_const_closure(Matter_TCP_async_get_timeout_closure) },
        { be_const_key_weak(timeout, 18), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_TCP_async_init_closure) },
        { be_const_key_weak(event_timeout, 6), be_const_closure(Matter_TCP_async_event_timeout_closure) },
        { be_const_key_weak(set_timeout, -1), be_const_closure(Matter_TCP_async_set_timeout_closure) },
        { be_const_key_weak(listening, 23), be_const_closure(Matter_TCP_async_listening_closure) },
        { be_const_key_weak(tcp, -1), be_const_var(3) },
        { be_const_key_weak(event_available, -1), be_const_closure(Matter_TCP_async_event_available_closure) },
        { be_const_key_weak(write, -1), be_const_closure(Matter_TCP_async_write_closure) },
        { be_const_key_weak(event_established, -1), be_const_closure(Matter_TCP_async_event_established_closure) },
        { be_const_key_weak(TIMEOUT, 7), be_const_int(1000) },
        { be_const_key_weak(time_start, 22), be_const_var(4) },
        { be_const_key_weak(reset, -1), be_const_closure(Matter_TCP_async_reset_closure) },
        { be_const_key_weak(loop, -1), be_const_closure(Matter_TCP_async_loop_closure) },
        { be_const_key_weak(status, -1), be_const_var(6) },
        { be_const_key_weak(port, -1), be_const_var(1) },
        { be_const_key_weak(every_50ms, 12), be_const_closure(Matter_TCP_async_every_50ms_closure) },
        { be_const_key_weak(fast_loop, -1), be_const_var(7) },
        { be_const_key_weak(event_dnsfailed, -1), be_const_closure(Matter_TCP_async_event_dnsfailed_closure) },
        { be_const_key_weak(close, -1), be_const_closure(Matter_TCP_async_close_closure) },
        { be_const_key_weak(available, 2), be_const_closure(Matter_TCP_async_available_closure) },
    })),
    be_str_weak(Matter_TCP_async)
);
/*******************************************************************/

void be_load_Matter_TCP_async_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_TCP_async);
    be_setglobal(vm, "Matter_TCP_async");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
