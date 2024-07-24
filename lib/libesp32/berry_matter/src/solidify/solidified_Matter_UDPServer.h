/* Solidification of Matter_UDPServer.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_UDPPacket_sent;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_UDPPacket_sent_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(raw),
    /* K1   */  be_nested_str_weak(addr),
    /* K2   */  be_nested_str_weak(remote_ip),
    /* K3   */  be_nested_str_weak(port),
    /* K4   */  be_nested_str_weak(remote_port),
    /* K5   */  be_nested_str_weak(msg_id),
    /* K6   */  be_nested_str_weak(x_flag_r),
    /* K7   */  be_nested_str_weak(message_counter),
    /* K8   */  be_nested_str_weak(exchange_id),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(session_id),
    /* K11  */  be_nested_str_weak(local_session_id),
    /* K12  */  be_nested_str_weak(retries),
    /* K13  */  be_nested_str_weak(next_try),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(millis),
    /* K16  */  be_nested_str_weak(matter),
    /* K17  */  be_nested_str_weak(UDPServer),
    /* K18  */  be_nested_str_weak(_backoff_time),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x90020002,  //  0001  SETMBR	R0	K0	R2
      0x88080302,  //  0002  GETMBR	R2	R1	K2
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0x88080304,  //  0004  GETMBR	R2	R1	K4
      0x90020602,  //  0005  SETMBR	R0	K3	R2
      0x88080306,  //  0006  GETMBR	R2	R1	K6
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x88080307,  //  0008  GETMBR	R2	R1	K7
      0x70020000,  //  0009  JMP		#000B
      0x4C080000,  //  000A  LDNIL	R2
      0x90020A02,  //  000B  SETMBR	R0	K5	R2
      0x88080308,  //  000C  GETMBR	R2	R1	K8
      0x4C0C0000,  //  000D  LDNIL	R3
      0x20080403,  //  000E  NE	R2	R2	R3
      0x780A0001,  //  000F  JMPF	R2	#0012
      0x88080308,  //  0010  GETMBR	R2	R1	K8
      0x70020000,  //  0011  JMP		#0013
      0x58080009,  //  0012  LDCONST	R2	K9
      0x90021002,  //  0013  SETMBR	R0	K8	R2
      0x8808030B,  //  0014  GETMBR	R2	R1	K11
      0x4C0C0000,  //  0015  LDNIL	R3
      0x20080403,  //  0016  NE	R2	R2	R3
      0x780A0001,  //  0017  JMPF	R2	#001A
      0x8808030B,  //  0018  GETMBR	R2	R1	K11
      0x70020000,  //  0019  JMP		#001B
      0x58080009,  //  001A  LDCONST	R2	K9
      0x90021402,  //  001B  SETMBR	R0	K10	R2
      0x90021909,  //  001C  SETMBR	R0	K12	K9
      0xB80A1C00,  //  001D  GETNGBL	R2	K14
      0x8C08050F,  //  001E  GETMET	R2	R2	K15
      0x7C080200,  //  001F  CALL	R2	1
      0xB80E2000,  //  0020  GETNGBL	R3	K16
      0x880C0711,  //  0021  GETMBR	R3	R3	K17
      0x8C0C0712,  //  0022  GETMET	R3	R3	K18
      0x8814010C,  //  0023  GETMBR	R5	R0	K12
      0x7C0C0400,  //  0024  CALL	R3	2
      0x00080403,  //  0025  ADD	R2	R2	R3
      0x90021A02,  //  0026  SETMBR	R0	K13	R2
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_UDPPacket_sent
********************************************************************/
be_local_class(Matter_UDPPacket_sent,
    8,
    NULL,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(next_try, -1), be_const_var(7) },
        { be_const_key_weak(session_id, 8), be_const_var(5) },
        { be_const_key_weak(retries, 3), be_const_var(6) },
        { be_const_key_weak(addr, -1), be_const_var(1) },
        { be_const_key_weak(port, 0), be_const_var(2) },
        { be_const_key_weak(raw, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_UDPPacket_sent_init_closure) },
        { be_const_key_weak(exchange_id, -1), be_const_var(4) },
        { be_const_key_weak(msg_id, -1), be_const_var(3) },
    })),
    be_str_weak(Matter_UDPPacket_sent)
);

extern const bclass be_class_Matter_UDPServer;

/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(class_Matter_UDPServer_every_50ms,   /* name */
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
** Solidified function: send_UDP
********************************************************************/
be_local_closure(class_Matter_UDPServer_send_UDP,   /* name */
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
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(UDPPacket_sent),
    /* K2   */  be_nested_str_weak(send),
    /* K3   */  be_nested_str_weak(msg_id),
    /* K4   */  be_nested_str_weak(packets_sent),
    /* K5   */  be_nested_str_weak(push),
    }),
    be_str_weak(send_UDP),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x8C0C0102,  //  0004  GETMET	R3	R0	K2
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C0C0400,  //  0006  CALL	R3	2
      0x880C0503,  //  0007  GETMBR	R3	R2	K3
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C0104,  //  0009  GETMBR	R3	R0	K4
      0x8C0C0705,  //  000A  GETMET	R3	R3	K5
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: received_ack
********************************************************************/
be_local_closure(class_Matter_UDPServer_received_ack,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(ack_message_counter),
    /* K1   */  be_nested_str_weak(exchange_id),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(packets_sent),
    /* K4   */  be_nested_str_weak(msg_id),
    /* K5   */  be_nested_str_weak(remove),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(loglevel),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Removed_X20packet_X20from_X20sending_X20list_X20id_X3D),
    /* K10  */  be_const_int(1),
    }),
    be_str_weak(received_ack),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x4C100000,  //  0002  LDNIL	R4
      0x1C100404,  //  0003  EQ	R4	R2	R4
      0x78120000,  //  0004  JMPF	R4	#0006
      0x80000800,  //  0005  RET	0
      0x58100002,  //  0006  LDCONST	R4	K2
      0x6014000C,  //  0007  GETGBL	R5	G12
      0x88180103,  //  0008  GETMBR	R6	R0	K3
      0x7C140200,  //  0009  CALL	R5	1
      0x14140805,  //  000A  LT	R5	R4	R5
      0x7816001A,  //  000B  JMPF	R5	#0027
      0x88140103,  //  000C  GETMBR	R5	R0	K3
      0x94140A04,  //  000D  GETIDX	R5	R5	R4
      0x88180B04,  //  000E  GETMBR	R6	R5	K4
      0x1C180C02,  //  000F  EQ	R6	R6	R2
      0x781A0013,  //  0010  JMPF	R6	#0025
      0x88180B01,  //  0011  GETMBR	R6	R5	K1
      0x1C180C03,  //  0012  EQ	R6	R6	R3
      0x781A0010,  //  0013  JMPF	R6	#0025
      0x88180103,  //  0014  GETMBR	R6	R0	K3
      0x8C180D05,  //  0015  GETMET	R6	R6	K5
      0x5C200800,  //  0016  MOVE	R8	R4
      0x7C180400,  //  0017  CALL	R6	2
      0xB81A0C00,  //  0018  GETNGBL	R6	K6
      0x8C180D07,  //  0019  GETMET	R6	R6	K7
      0x54220003,  //  001A  LDINT	R8	4
      0x7C180400,  //  001B  CALL	R6	2
      0x781A0006,  //  001C  JMPF	R6	#0024
      0xB81A1000,  //  001D  GETNGBL	R6	K8
      0x601C0008,  //  001E  GETGBL	R7	G8
      0x5C200400,  //  001F  MOVE	R8	R2
      0x7C1C0200,  //  0020  CALL	R7	1
      0x001E1207,  //  0021  ADD	R7	K9	R7
      0x54220003,  //  0022  LDINT	R8	4
      0x7C180400,  //  0023  CALL	R6	2
      0x70020000,  //  0024  JMP		#0026
      0x0010090A,  //  0025  ADD	R4	R4	K10
      0x7001FFDF,  //  0026  JMP		#0007
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_Matter_UDPServer_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(listening),
    /* K1   */  be_nested_str_weak(udp_socket),
    /* K2   */  be_nested_str_weak(udp),
    /* K3   */  be_nested_str_weak(begin),
    /* K4   */  be_nested_str_weak(addr),
    /* K5   */  be_nested_str_weak(port),
    /* K6   */  be_nested_str_weak(network_error),
    /* K7   */  be_nested_str_weak(could_X20not_X20open_X20UDP_X20server),
    /* K8   */  be_nested_str_weak(dispatch_cb),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(add_fast_loop),
    /* K11  */  be_nested_str_weak(loop_cb),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0011,  //  0001  JMPT	R2	#0014
      0xB80A0400,  //  0002  GETNGBL	R2	K2
      0x7C080000,  //  0003  CALL	R2	0
      0x90020202,  //  0004  SETMBR	R0	K1	R2
      0x88080101,  //  0005  GETMBR	R2	R0	K1
      0x8C080503,  //  0006  GETMET	R2	R2	K3
      0x88100104,  //  0007  GETMBR	R4	R0	K4
      0x88140105,  //  0008  GETMBR	R5	R0	K5
      0x7C080600,  //  0009  CALL	R2	3
      0x5C0C0400,  //  000A  MOVE	R3	R2
      0x740E0000,  //  000B  JMPT	R3	#000D
      0xB0060D07,  //  000C  RAISE	1	K6	K7
      0x500C0200,  //  000D  LDBOOL	R3	1	0
      0x90020003,  //  000E  SETMBR	R0	K0	R3
      0x90021001,  //  000F  SETMBR	R0	K8	R1
      0xB80E1200,  //  0010  GETNGBL	R3	K9
      0x8C0C070A,  //  0011  GETMET	R3	R3	K10
      0x8814010B,  //  0012  GETMBR	R5	R0	K11
      0x7C0C0400,  //  0013  CALL	R3	2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send
********************************************************************/
be_local_closure(class_Matter_UDPServer_send,   /* name */
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
    /* K0   */  be_nested_str_weak(udp_socket),
    /* K1   */  be_nested_str_weak(send),
    /* K2   */  be_nested_str_weak(addr),
    /* K3   */  be_nested_str_weak(remote_ip),
    /* K4   */  be_nested_str_weak(port),
    /* K5   */  be_nested_str_weak(remote_port),
    /* K6   */  be_nested_str_weak(raw),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(loglevel),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20sending_X20packet_X20to_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20error_X20sending_X20packet_X20to_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    }),
    be_str_weak(send),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100302,  //  0002  GETMBR	R4	R1	K2
      0x78120001,  //  0003  JMPF	R4	#0006
      0x88100302,  //  0004  GETMBR	R4	R1	K2
      0x70020001,  //  0005  JMP		#0008
      0x88100100,  //  0006  GETMBR	R4	R0	K0
      0x88100903,  //  0007  GETMBR	R4	R4	K3
      0x88140304,  //  0008  GETMBR	R5	R1	K4
      0x78160001,  //  0009  JMPF	R5	#000C
      0x88140304,  //  000A  GETMBR	R5	R1	K4
      0x70020001,  //  000B  JMP		#000E
      0x88140100,  //  000C  GETMBR	R5	R0	K0
      0x88140B05,  //  000D  GETMBR	R5	R5	K5
      0x88180306,  //  000E  GETMBR	R6	R1	K6
      0x7C080800,  //  000F  CALL	R2	4
      0x780A000D,  //  0010  JMPF	R2	#001F
      0xB80E0E00,  //  0011  GETNGBL	R3	K7
      0x8C0C0708,  //  0012  GETMET	R3	R3	K8
      0x54160003,  //  0013  LDINT	R5	4
      0x7C0C0400,  //  0014  CALL	R3	2
      0x780E0007,  //  0015  JMPF	R3	#001E
      0xB80E1200,  //  0016  GETNGBL	R3	K9
      0x60100018,  //  0017  GETGBL	R4	G24
      0x5814000A,  //  0018  LDCONST	R5	K10
      0x88180302,  //  0019  GETMBR	R6	R1	K2
      0x881C0304,  //  001A  GETMBR	R7	R1	K4
      0x7C100600,  //  001B  CALL	R4	3
      0x54160003,  //  001C  LDINT	R5	4
      0x7C0C0400,  //  001D  CALL	R3	2
      0x7002000C,  //  001E  JMP		#002C
      0xB80E0E00,  //  001F  GETNGBL	R3	K7
      0x8C0C0708,  //  0020  GETMET	R3	R3	K8
      0x5814000B,  //  0021  LDCONST	R5	K11
      0x7C0C0400,  //  0022  CALL	R3	2
      0x780E0007,  //  0023  JMPF	R3	#002C
      0xB80E1200,  //  0024  GETNGBL	R3	K9
      0x60100018,  //  0025  GETGBL	R4	G24
      0x5814000C,  //  0026  LDCONST	R5	K12
      0x88180302,  //  0027  GETMBR	R6	R1	K2
      0x881C0304,  //  0028  GETMBR	R7	R1	K4
      0x7C100600,  //  0029  CALL	R4	3
      0x5814000B,  //  002A  LDCONST	R5	K11
      0x7C0C0400,  //  002B  CALL	R3	2
      0x80040400,  //  002C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_Matter_UDPServer_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(listening),
    /* K1   */  be_nested_str_weak(udp_socket),
    /* K2   */  be_nested_str_weak(stop),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(remove_fast_loop),
    /* K5   */  be_nested_str_weak(loop_cb),
    }),
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060008,  //  0001  JMPF	R1	#000B
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x50040000,  //  0005  LDBOOL	R1	0	0
      0x90020001,  //  0006  SETMBR	R0	K0	R1
      0xB8060600,  //  0007  GETNGBL	R1	K3
      0x8C040304,  //  0008  GETMET	R1	R1	K4
      0x880C0105,  //  0009  GETMBR	R3	R0	K5
      0x7C040400,  //  000A  CALL	R1	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_UDPServer_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    4,                          /* argc */
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
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80000000,  //  0003  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(addr),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(port),
    /* K4   */  be_nested_str_weak(listening),
    /* K5   */  be_nested_str_weak(packets_sent),
    /* K6   */  be_nested_str_weak(loop_cb),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x5C100400,  //  0002  MOVE	R4	R2
      0x70020000,  //  0003  JMP		#0005
      0x58100002,  //  0004  LDCONST	R4	K2
      0x90020204,  //  0005  SETMBR	R0	K1	R4
      0x780E0001,  //  0006  JMPF	R3	#0009
      0x5C100600,  //  0007  MOVE	R4	R3
      0x70020000,  //  0008  JMP		#000A
      0x541215A3,  //  0009  LDINT	R4	5540
      0x90020604,  //  000A  SETMBR	R0	K3	R4
      0x50100000,  //  000B  LDBOOL	R4	0	0
      0x90020804,  //  000C  SETMBR	R0	K4	R4
      0x60100012,  //  000D  GETGBL	R4	G18
      0x7C100000,  //  000E  CALL	R4	0
      0x90020A04,  //  000F  SETMBR	R0	K5	R4
      0x84100000,  //  0010  CLOSURE	R4	P0
      0x90020C04,  //  0011  SETMBR	R0	K6	R4
      0xA0000000,  //  0012  CLOSE	R0
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _resend_packets
********************************************************************/
be_local_closure(class_Matter_UDPServer__resend_packets,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(packets_sent),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(time_reached),
    /* K4   */  be_nested_str_weak(next_try),
    /* K5   */  be_nested_str_weak(retries),
    /* K6   */  be_nested_str_weak(RETRIES),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Resending_X20packet_X20id_X3D),
    /* K9   */  be_nested_str_weak(msg_id),
    /* K10  */  be_nested_str_weak(send),
    /* K11  */  be_nested_str_weak(millis),
    /* K12  */  be_nested_str_weak(_backoff_time),
    /* K13  */  be_const_int(1),
    /* K14  */  be_nested_str_weak(remove),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20Unacked_X20packet_X20_X27_X5B_X25s_X5D_X3A_X25i_X27_X20msg_id_X3D_X25i),
    /* K16  */  be_nested_str_weak(session_id),
    /* K17  */  be_nested_str_weak(addr),
    /* K18  */  be_nested_str_weak(port),
    /* K19  */  be_const_int(3),
    }),
    be_str_weak(_resend_packets),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A0032,  //  0005  JMPF	R2	#0039
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0xB80E0400,  //  0008  GETNGBL	R3	K2
      0x8C0C0703,  //  0009  GETMET	R3	R3	K3
      0x88140504,  //  000A  GETMBR	R5	R2	K4
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0029,  //  000C  JMPF	R3	#0037
      0x880C0505,  //  000D  GETMBR	R3	R2	K5
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x180C0604,  //  000F  LE	R3	R3	R4
      0x780E0016,  //  0010  JMPF	R3	#0028
      0xB80E0E00,  //  0011  GETNGBL	R3	K7
      0x60100008,  //  0012  GETGBL	R4	G8
      0x88140509,  //  0013  GETMBR	R5	R2	K9
      0x7C100200,  //  0014  CALL	R4	1
      0x00121004,  //  0015  ADD	R4	K8	R4
      0x54160003,  //  0016  LDINT	R5	4
      0x7C0C0400,  //  0017  CALL	R3	2
      0x8C0C010A,  //  0018  GETMET	R3	R0	K10
      0x5C140400,  //  0019  MOVE	R5	R2
      0x7C0C0400,  //  001A  CALL	R3	2
      0xB80E0400,  //  001B  GETNGBL	R3	K2
      0x8C0C070B,  //  001C  GETMET	R3	R3	K11
      0x7C0C0200,  //  001D  CALL	R3	1
      0x8C10010C,  //  001E  GETMET	R4	R0	K12
      0x88180505,  //  001F  GETMBR	R6	R2	K5
      0x7C100400,  //  0020  CALL	R4	2
      0x000C0604,  //  0021  ADD	R3	R3	R4
      0x900A0803,  //  0022  SETMBR	R2	K4	R3
      0x880C0505,  //  0023  GETMBR	R3	R2	K5
      0x000C070D,  //  0024  ADD	R3	R3	K13
      0x900A0A03,  //  0025  SETMBR	R2	K5	R3
      0x0004030D,  //  0026  ADD	R1	R1	K13
      0x7002000D,  //  0027  JMP		#0036
      0x880C0101,  //  0028  GETMBR	R3	R0	K1
      0x8C0C070E,  //  0029  GETMET	R3	R3	K14
      0x5C140200,  //  002A  MOVE	R5	R1
      0x7C0C0400,  //  002B  CALL	R3	2
      0xB80E0E00,  //  002C  GETNGBL	R3	K7
      0x60100018,  //  002D  GETGBL	R4	G24
      0x5814000F,  //  002E  LDCONST	R5	K15
      0x88180510,  //  002F  GETMBR	R6	R2	K16
      0x881C0511,  //  0030  GETMBR	R7	R2	K17
      0x88200512,  //  0031  GETMBR	R8	R2	K18
      0x88240509,  //  0032  GETMBR	R9	R2	K9
      0x7C100A00,  //  0033  CALL	R4	5
      0x58140013,  //  0034  LDCONST	R5	K19
      0x7C0C0400,  //  0035  CALL	R3	2
      0x70020000,  //  0036  JMP		#0038
      0x0004030D,  //  0037  ADD	R1	R1	K13
      0x7001FFC7,  //  0038  JMP		#0001
      0x80000000,  //  0039  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _backoff_time
********************************************************************/
be_local_closure(class_Matter_UDPServer__backoff_time,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        4,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_const_int(1),
        /* K1   */  be_const_int(0),
        }),
        be_str_weak(power_int),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x58080000,  //  0000  LDCONST	R2	K0
          0x240C0301,  //  0001  GT	R3	R1	K1
          0x780E0002,  //  0002  JMPF	R3	#0006
          0x08080400,  //  0003  MUL	R2	R2	R0
          0x04040300,  //  0004  SUB	R1	R1	K0
          0x7001FFFA,  //  0005  JMP		#0001
          0x80040400,  //  0006  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_UDPServer),
    /* K1   */  be_nested_str_weak(math),
    /* K2   */  be_nested_str_weak(rand),
    /* K3   */  be_const_int(0),
    /* K4   */  be_const_int(1),
    /* K5   */  be_const_real_hex(0x3FCCCCCD),
    /* K6   */  be_const_real_hex(0x3F800000),
    /* K7   */  be_const_real_hex(0x3E800000),
    }),
    be_str_weak(_backoff_time),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x84080000,  //  0001  CLOSURE	R2	P0
      0xA40E0200,  //  0002  IMPORT	R3	K1
      0x5412012B,  //  0003  LDINT	R4	300
      0x6014000A,  //  0004  GETGBL	R5	G10
      0x8C180702,  //  0005  GETMET	R6	R3	K2
      0x7C180200,  //  0006  CALL	R6	1
      0x541E00FE,  //  0007  LDINT	R7	255
      0x2C180C07,  //  0008  AND	R6	R6	R7
      0x7C140200,  //  0009  CALL	R5	1
      0x541A00FE,  //  000A  LDINT	R6	255
      0x0C140A06,  //  000B  DIV	R5	R5	R6
      0x24180103,  //  000C  GT	R6	R0	K3
      0x781A0001,  //  000D  JMPF	R6	#0010
      0x04180104,  //  000E  SUB	R6	R0	K4
      0x70020000,  //  000F  JMP		#0011
      0x58180003,  //  0010  LDCONST	R6	K3
      0x5C1C0400,  //  0011  MOVE	R7	R2
      0x58200005,  //  0012  LDCONST	R8	K5
      0x5C240C00,  //  0013  MOVE	R9	R6
      0x7C1C0400,  //  0014  CALL	R7	2
      0x081C0807,  //  0015  MUL	R7	R4	R7
      0x08200B07,  //  0016  MUL	R8	R5	K7
      0x00220C08,  //  0017  ADD	R8	K6	R8
      0x081C0E08,  //  0018  MUL	R7	R7	R8
      0x60200009,  //  0019  GETGBL	R8	G9
      0x5C240E00,  //  001A  MOVE	R9	R7
      0x7C200200,  //  001B  CALL	R8	1
      0x80041000,  //  001C  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: loop
********************************************************************/
be_local_closure(class_Matter_UDPServer_loop,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(udp_socket),
    /* K4   */  be_nested_str_weak(read),
    /* K5   */  be_nested_str_weak(packet),
    /* K6   */  be_nested_str_weak(start),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(remote_ip),
    /* K9   */  be_nested_str_weak(remote_port),
    /* K10  */  be_nested_str_weak(tasmota),
    /* K11  */  be_nested_str_weak(loglevel),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20UDP_X20received_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K14  */  be_nested_str_weak(dispatch_cb),
    /* K15  */  be_nested_str_weak(dump),
    /* K16  */  be_const_int(2),
    /* K17  */  be_nested_str_weak(MAX_PACKETS_READ),
    /* K18  */  be_nested_str_weak(_resend_packets),
    }),
    be_str_weak(loop),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x58080002,  //  0002  LDCONST	R2	K2
      0x880C0103,  //  0003  GETMBR	R3	R0	K3
      0x4C100000,  //  0004  LDNIL	R4
      0x1C0C0604,  //  0005  EQ	R3	R3	R4
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x80000600,  //  0007  RET	0
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x8C0C0704,  //  0009  GETMET	R3	R3	K4
      0x88140105,  //  000A  GETMBR	R5	R0	K5
      0x7C0C0400,  //  000B  CALL	R3	2
      0x4C100000,  //  000C  LDNIL	R4
      0x20100604,  //  000D  NE	R4	R3	R4
      0x78120028,  //  000E  JMPF	R4	#0038
      0x8C100306,  //  000F  GETMET	R4	R1	K6
      0x7C100200,  //  0010  CALL	R4	1
      0x90020A03,  //  0011  SETMBR	R0	K5	R3
      0x00080507,  //  0012  ADD	R2	R2	K7
      0x88100103,  //  0013  GETMBR	R4	R0	K3
      0x88100908,  //  0014  GETMBR	R4	R4	K8
      0x88140103,  //  0015  GETMBR	R5	R0	K3
      0x88140B09,  //  0016  GETMBR	R5	R5	K9
      0xB81A1400,  //  0017  GETNGBL	R6	K10
      0x8C180D0B,  //  0018  GETMET	R6	R6	K11
      0x54220003,  //  0019  LDINT	R8	4
      0x7C180400,  //  001A  CALL	R6	2
      0x781A0007,  //  001B  JMPF	R6	#0024
      0xB81A1800,  //  001C  GETNGBL	R6	K12
      0x601C0018,  //  001D  GETGBL	R7	G24
      0x5820000D,  //  001E  LDCONST	R8	K13
      0x5C240800,  //  001F  MOVE	R9	R4
      0x5C280A00,  //  0020  MOVE	R10	R5
      0x7C1C0600,  //  0021  CALL	R7	3
      0x54220003,  //  0022  LDINT	R8	4
      0x7C180400,  //  0023  CALL	R6	2
      0x8818010E,  //  0024  GETMBR	R6	R0	K14
      0x781A0004,  //  0025  JMPF	R6	#002B
      0x8C18010E,  //  0026  GETMET	R6	R0	K14
      0x5C200600,  //  0027  MOVE	R8	R3
      0x5C240800,  //  0028  MOVE	R9	R4
      0x5C280A00,  //  0029  MOVE	R10	R5
      0x7C180800,  //  002A  CALL	R6	4
      0x8C18030F,  //  002B  GETMET	R6	R1	K15
      0x58200010,  //  002C  LDCONST	R8	K16
      0x7C180400,  //  002D  CALL	R6	2
      0x88180111,  //  002E  GETMBR	R6	R0	K17
      0x14180406,  //  002F  LT	R6	R2	R6
      0x781A0004,  //  0030  JMPF	R6	#0036
      0x88180103,  //  0031  GETMBR	R6	R0	K3
      0x8C180D04,  //  0032  GETMET	R6	R6	K4
      0x7C180200,  //  0033  CALL	R6	1
      0x5C0C0C00,  //  0034  MOVE	R3	R6
      0x70020000,  //  0035  JMP		#0037
      0x4C0C0000,  //  0036  LDNIL	R3
      0x7001FFD3,  //  0037  JMP		#000C
      0x8C100112,  //  0038  GETMET	R4	R0	K18
      0x7C100200,  //  0039  CALL	R4	1
      0x80000000,  //  003A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_Matter_UDPServer_every_second,   /* name */
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
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_UDPServer
********************************************************************/
be_local_class(Matter_UDPServer,
    9,
    NULL,
    be_nested_map(22,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(every_50ms, -1), be_const_closure(class_Matter_UDPServer_every_50ms_closure) },
        { be_const_key_weak(send_UDP, -1), be_const_closure(class_Matter_UDPServer_send_UDP_closure) },
        { be_const_key_weak(received_ack, -1), be_const_closure(class_Matter_UDPServer_received_ack_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_UDPServer_every_second_closure) },
        { be_const_key_weak(stop, 18), be_const_closure(class_Matter_UDPServer_stop_closure) },
        { be_const_key_weak(start, 4), be_const_closure(class_Matter_UDPServer_start_closure) },
        { be_const_key_weak(listening, -1), be_const_var(3) },
        { be_const_key_weak(send, -1), be_const_closure(class_Matter_UDPServer_send_closure) },
        { be_const_key_weak(loop, -1), be_const_closure(class_Matter_UDPServer_loop_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_UDPServer_init_closure) },
        { be_const_key_weak(udp_socket, 6), be_const_var(4) },
        { be_const_key_weak(packet, -1), be_const_var(8) },
        { be_const_key_weak(_backoff_time, -1), be_const_static_closure(class_Matter_UDPServer__backoff_time_closure) },
        { be_const_key_weak(_resend_packets, -1), be_const_closure(class_Matter_UDPServer__resend_packets_closure) },
        { be_const_key_weak(addr, -1), be_const_var(0) },
        { be_const_key_weak(dispatch_cb, 8), be_const_var(5) },
        { be_const_key_weak(port, 12), be_const_var(1) },
        { be_const_key_weak(packets_sent, -1), be_const_var(6) },
        { be_const_key_weak(device, -1), be_const_var(2) },
        { be_const_key_weak(loop_cb, -1), be_const_var(7) },
        { be_const_key_weak(MAX_PACKETS_READ, -1), be_const_int(4) },
        { be_const_key_weak(RETRIES, 3), be_const_int(5) },
    })),
    be_str_weak(Matter_UDPServer)
);
/********************************************************************/
/* End of solidification */
