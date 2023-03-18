/* Solidification of Matter_UDPServer.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_UDPPacket_sent;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_UDPPacket_sent_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(raw),
    /* K1   */  be_nested_str_weak(addr),
    /* K2   */  be_nested_str_weak(port),
    /* K3   */  be_nested_str_weak(msg_id),
    /* K4   */  be_nested_str_weak(retries),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(next_try),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(millis),
    /* K9   */  be_nested_str_weak(backoff_time),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x90020604,  //  0003  SETMBR	R0	K3	R4
      0x90020905,  //  0004  SETMBR	R0	K4	K5
      0xB8160E00,  //  0005  GETNGBL	R5	K7
      0x8C140B08,  //  0006  GETMET	R5	R5	K8
      0x7C140200,  //  0007  CALL	R5	1
      0x8C180109,  //  0008  GETMET	R6	R0	K9
      0x88200104,  //  0009  GETMBR	R8	R0	K4
      0x7C180400,  //  000A  CALL	R6	2
      0x00140A06,  //  000B  ADD	R5	R5	R6
      0x90020C05,  //  000C  SETMBR	R0	K6	R5
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: backoff_time
********************************************************************/
be_local_closure(Matter_UDPPacket_sent_backoff_time,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(math),
    /* K1   */  be_nested_str_weak(rand),
    /* K2   */  be_const_int(0),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_real_hex(0x3FCCCCCD),
    /* K5   */  be_const_real_hex(0x3F800000),
    /* K6   */  be_const_real_hex(0x3E800000),
    }),
    be_str_weak(backoff_time),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x84080000,  //  0000  CLOSURE	R2	P0
      0xA40E0000,  //  0001  IMPORT	R3	K0
      0x5412012B,  //  0002  LDINT	R4	300
      0x6014000A,  //  0003  GETGBL	R5	G10
      0x8C180701,  //  0004  GETMET	R6	R3	K1
      0x7C180200,  //  0005  CALL	R6	1
      0x541E00FE,  //  0006  LDINT	R7	255
      0x2C180C07,  //  0007  AND	R6	R6	R7
      0x7C140200,  //  0008  CALL	R5	1
      0x541A00FE,  //  0009  LDINT	R6	255
      0x0C140A06,  //  000A  DIV	R5	R5	R6
      0x24180302,  //  000B  GT	R6	R1	K2
      0x781A0001,  //  000C  JMPF	R6	#000F
      0x04180303,  //  000D  SUB	R6	R1	K3
      0x70020000,  //  000E  JMP		#0010
      0x58180002,  //  000F  LDCONST	R6	K2
      0x5C1C0400,  //  0010  MOVE	R7	R2
      0x58200004,  //  0011  LDCONST	R8	K4
      0x5C240C00,  //  0012  MOVE	R9	R6
      0x7C1C0400,  //  0013  CALL	R7	2
      0x081C0807,  //  0014  MUL	R7	R4	R7
      0x08200B06,  //  0015  MUL	R8	R5	K6
      0x00220A08,  //  0016  ADD	R8	K5	R8
      0x081C0E08,  //  0017  MUL	R7	R7	R8
      0x60200009,  //  0018  GETGBL	R8	G9
      0x5C240E00,  //  0019  MOVE	R9	R7
      0x7C200200,  //  001A  CALL	R8	1
      0x80041000,  //  001B  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send
********************************************************************/
be_local_closure(Matter_UDPPacket_sent_send,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(send),
    /* K2   */  be_nested_str_weak(addr),
    /* K3   */  be_nested_str_weak(remote_ip),
    /* K4   */  be_nested_str_weak(port),
    /* K5   */  be_nested_str_weak(remote_port),
    /* K6   */  be_nested_str_weak(raw),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(format),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20sending_X20packet_X20to_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20error_X20sending_X20packet_X20to_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
    /* K12  */  be_const_int(3),
    }),
    be_str_weak(send),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0301,  //  0001  GETMET	R3	R1	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x78160001,  //  0003  JMPF	R5	#0006
      0x88140102,  //  0004  GETMBR	R5	R0	K2
      0x70020000,  //  0005  JMP		#0007
      0x88140303,  //  0006  GETMBR	R5	R1	K3
      0x88180104,  //  0007  GETMBR	R6	R0	K4
      0x781A0001,  //  0008  JMPF	R6	#000B
      0x88180104,  //  0009  GETMBR	R6	R0	K4
      0x70020000,  //  000A  JMP		#000C
      0x88180305,  //  000B  GETMBR	R6	R1	K5
      0x881C0106,  //  000C  GETMBR	R7	R0	K6
      0x7C0C0800,  //  000D  CALL	R3	4
      0x780E0009,  //  000E  JMPF	R3	#0019
      0xB8120E00,  //  000F  GETNGBL	R4	K7
      0x8C100908,  //  0010  GETMET	R4	R4	K8
      0x8C180509,  //  0011  GETMET	R6	R2	K9
      0x5820000A,  //  0012  LDCONST	R8	K10
      0x88240102,  //  0013  GETMBR	R9	R0	K2
      0x88280104,  //  0014  GETMBR	R10	R0	K4
      0x7C180800,  //  0015  CALL	R6	4
      0x541E0003,  //  0016  LDINT	R7	4
      0x7C100600,  //  0017  CALL	R4	3
      0x70020008,  //  0018  JMP		#0022
      0xB8120E00,  //  0019  GETNGBL	R4	K7
      0x8C100908,  //  001A  GETMET	R4	R4	K8
      0x8C180509,  //  001B  GETMET	R6	R2	K9
      0x5820000B,  //  001C  LDCONST	R8	K11
      0x88240102,  //  001D  GETMBR	R9	R0	K2
      0x88280104,  //  001E  GETMBR	R10	R0	K4
      0x7C180800,  //  001F  CALL	R6	4
      0x581C000C,  //  0020  LDCONST	R7	K12
      0x7C100600,  //  0021  CALL	R4	3
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_UDPPacket_sent
********************************************************************/
be_local_class(Matter_UDPPacket_sent,
    6,
    NULL,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(send, 7), be_const_closure(Matter_UDPPacket_sent_send_closure) },
        { be_const_key_weak(msg_id, -1), be_const_var(3) },
        { be_const_key_weak(retries, 8), be_const_var(4) },
        { be_const_key_weak(backoff_time, -1), be_const_closure(Matter_UDPPacket_sent_backoff_time_closure) },
        { be_const_key_weak(next_try, 0), be_const_var(5) },
        { be_const_key_weak(raw, 3), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_UDPPacket_sent_init_closure) },
        { be_const_key_weak(port, -1), be_const_var(2) },
        { be_const_key_weak(addr, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_UDPPacket_sent)
);
/*******************************************************************/

void be_load_Matter_UDPPacket_sent_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_UDPPacket_sent);
    be_setglobal(vm, "Matter_UDPPacket_sent");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_UDPServer;

/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(Matter_UDPServer_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(listening),
    /* K1   */  be_nested_str_weak(udp_socket),
    /* K2   */  be_nested_str_weak(udp),
    /* K3   */  be_nested_str_weak(begin),
    /* K4   */  be_nested_str_weak(address),
    /* K5   */  be_nested_str_weak(port),
    /* K6   */  be_nested_str_weak(network_error),
    /* K7   */  be_nested_str_weak(could_X20not_X20open_X20UDP_X20server),
    /* K8   */  be_nested_str_weak(dispatch_cb),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(add_driver),
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
      0x5C140000,  //  0012  MOVE	R5	R0
      0x7C0C0400,  //  0013  CALL	R3	2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_UDPServer_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(address),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(port),
    /* K3   */  be_nested_str_weak(listening),
    /* K4   */  be_nested_str_weak(packets_sent),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x78060001,  //  0000  JMPF	R1	#0003
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x70020000,  //  0002  JMP		#0004
      0x580C0001,  //  0003  LDCONST	R3	K1
      0x90020003,  //  0004  SETMBR	R0	K0	R3
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x70020000,  //  0007  JMP		#0009
      0x540E15A3,  //  0008  LDINT	R3	5540
      0x90020403,  //  0009  SETMBR	R0	K2	R3
      0x500C0000,  //  000A  LDBOOL	R3	0	0
      0x90020603,  //  000B  SETMBR	R0	K3	R3
      0x600C0012,  //  000C  GETGBL	R3	G18
      0x7C0C0000,  //  000D  CALL	R3	0
      0x90020803,  //  000E  SETMBR	R0	K4	R3
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resend_packets
********************************************************************/
be_local_closure(Matter_UDPServer_resend_packets,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(packets_sent),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(time_reached),
    /* K4   */  be_nested_str_weak(next_try),
    /* K5   */  be_nested_str_weak(retries),
    /* K6   */  be_nested_str_weak(RETRIES),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20resending_X20packet_X20id_X3D),
    /* K9   */  be_nested_str_weak(msg_id),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(send),
    /* K12  */  be_nested_str_weak(udp_socket),
    /* K13  */  be_nested_str_weak(millis),
    /* K14  */  be_nested_str_weak(backoff_time),
    /* K15  */  be_const_int(1),
    /* K16  */  be_nested_str_weak(string),
    /* K17  */  be_nested_str_weak(remove),
    /* K18  */  be_nested_str_weak(format),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20target_X20unreachable_X20_X27_X5B_X25s_X5D_X3A_X25i_X27_X20msg_id_X3D_X25i),
    /* K20  */  be_nested_str_weak(addr),
    /* K21  */  be_nested_str_weak(port),
    /* K22  */  be_const_int(2),
    }),
    be_str_weak(resend_packets),
    &be_const_str_solidified,
    ( &(const binstruction[60]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A0034,  //  0005  JMPF	R2	#003B
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0xB80E0400,  //  0008  GETNGBL	R3	K2
      0x8C0C0703,  //  0009  GETMET	R3	R3	K3
      0x88140504,  //  000A  GETMBR	R5	R2	K4
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E002B,  //  000C  JMPF	R3	#0039
      0x880C0505,  //  000D  GETMBR	R3	R2	K5
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x180C0604,  //  000F  LE	R3	R3	R4
      0x780E0017,  //  0010  JMPF	R3	#0029
      0xB80E0400,  //  0011  GETNGBL	R3	K2
      0x8C0C0707,  //  0012  GETMET	R3	R3	K7
      0x60140008,  //  0013  GETGBL	R5	G8
      0x88180509,  //  0014  GETMBR	R6	R2	K9
      0x7C140200,  //  0015  CALL	R5	1
      0x00161005,  //  0016  ADD	R5	K8	R5
      0x5818000A,  //  0017  LDCONST	R6	K10
      0x7C0C0600,  //  0018  CALL	R3	3
      0x8C0C050B,  //  0019  GETMET	R3	R2	K11
      0x8814010C,  //  001A  GETMBR	R5	R0	K12
      0x7C0C0400,  //  001B  CALL	R3	2
      0xB80E0400,  //  001C  GETNGBL	R3	K2
      0x8C0C070D,  //  001D  GETMET	R3	R3	K13
      0x7C0C0200,  //  001E  CALL	R3	1
      0x8C10050E,  //  001F  GETMET	R4	R2	K14
      0x88180505,  //  0020  GETMBR	R6	R2	K5
      0x7C100400,  //  0021  CALL	R4	2
      0x000C0604,  //  0022  ADD	R3	R3	R4
      0x900A0803,  //  0023  SETMBR	R2	K4	R3
      0x880C0505,  //  0024  GETMBR	R3	R2	K5
      0x000C070F,  //  0025  ADD	R3	R3	K15
      0x900A0A03,  //  0026  SETMBR	R2	K5	R3
      0x0004030F,  //  0027  ADD	R1	R1	K15
      0x7002000E,  //  0028  JMP		#0038
      0xA40E2000,  //  0029  IMPORT	R3	K16
      0x88100101,  //  002A  GETMBR	R4	R0	K1
      0x8C100911,  //  002B  GETMET	R4	R4	K17
      0x5C180200,  //  002C  MOVE	R6	R1
      0x7C100400,  //  002D  CALL	R4	2
      0xB8120400,  //  002E  GETNGBL	R4	K2
      0x8C100907,  //  002F  GETMET	R4	R4	K7
      0x8C180712,  //  0030  GETMET	R6	R3	K18
      0x58200013,  //  0031  LDCONST	R8	K19
      0x88240514,  //  0032  GETMBR	R9	R2	K20
      0x88280515,  //  0033  GETMBR	R10	R2	K21
      0x882C0509,  //  0034  GETMBR	R11	R2	K9
      0x7C180A00,  //  0035  CALL	R6	5
      0x581C0016,  //  0036  LDCONST	R7	K22
      0x7C100600,  //  0037  CALL	R4	3
      0x70020000,  //  0038  JMP		#003A
      0x0004030F,  //  0039  ADD	R1	R1	K15
      0x7001FFC5,  //  003A  JMP		#0001
      0x80000000,  //  003B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: packet_ack
********************************************************************/
be_local_closure(Matter_UDPServer_packet_ack,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(packets_sent),
    /* K2   */  be_nested_str_weak(msg_id),
    /* K3   */  be_nested_str_weak(remove),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20removed_X20packet_X20from_X20sending_X20list_X20id_X3D),
    /* K7   */  be_const_int(1),
    }),
    be_str_weak(packet_ack),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x58080000,  //  0004  LDCONST	R2	K0
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E0013,  //  0009  JMPF	R3	#001E
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x940C0602,  //  000B  GETIDX	R3	R3	R2
      0x880C0702,  //  000C  GETMBR	R3	R3	K2
      0x1C0C0601,  //  000D  EQ	R3	R3	R1
      0x780E000C,  //  000E  JMPF	R3	#001C
      0x880C0101,  //  000F  GETMBR	R3	R0	K1
      0x8C0C0703,  //  0010  GETMET	R3	R3	K3
      0x5C140400,  //  0011  MOVE	R5	R2
      0x7C0C0400,  //  0012  CALL	R3	2
      0xB80E0800,  //  0013  GETNGBL	R3	K4
      0x8C0C0705,  //  0014  GETMET	R3	R3	K5
      0x60140008,  //  0015  GETGBL	R5	G8
      0x5C180200,  //  0016  MOVE	R6	R1
      0x7C140200,  //  0017  CALL	R5	1
      0x00160C05,  //  0018  ADD	R5	K6	R5
      0x541A0003,  //  0019  LDINT	R6	4
      0x7C0C0600,  //  001A  CALL	R3	3
      0x70020000,  //  001B  JMP		#001D
      0x00080507,  //  001C  ADD	R2	R2	K7
      0x7001FFE6,  //  001D  JMP		#0005
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(Matter_UDPServer_every_50ms,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(udp_socket),
    /* K3   */  be_nested_str_weak(read),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(remote_ip),
    /* K6   */  be_nested_str_weak(remote_port),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(format),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20UDP_X20received_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(dispatch_cb),
    /* K13  */  be_nested_str_weak(MAX_PACKETS_READ),
    /* K14  */  be_nested_str_weak(resend_packets),
    }),
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x58080001,  //  0001  LDCONST	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x4C100000,  //  0003  LDNIL	R4
      0x1C0C0604,  //  0004  EQ	R3	R3	R4
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80000600,  //  0006  RET	0
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0008  GETMET	R3	R3	K3
      0x7C0C0200,  //  0009  CALL	R3	1
      0x4C100000,  //  000A  LDNIL	R4
      0x20100604,  //  000B  NE	R4	R3	R4
      0x7812001E,  //  000C  JMPF	R4	#002C
      0x00080504,  //  000D  ADD	R2	R2	K4
      0x88100102,  //  000E  GETMBR	R4	R0	K2
      0x88100905,  //  000F  GETMBR	R4	R4	K5
      0x88140102,  //  0010  GETMBR	R5	R0	K2
      0x88140B06,  //  0011  GETMBR	R5	R5	K6
      0xB81A0E00,  //  0012  GETNGBL	R6	K7
      0x8C180D08,  //  0013  GETMET	R6	R6	K8
      0x8C200309,  //  0014  GETMET	R8	R1	K9
      0x5828000A,  //  0015  LDCONST	R10	K10
      0x5C2C0800,  //  0016  MOVE	R11	R4
      0x5C300A00,  //  0017  MOVE	R12	R5
      0x7C200800,  //  0018  CALL	R8	4
      0x5824000B,  //  0019  LDCONST	R9	K11
      0x7C180600,  //  001A  CALL	R6	3
      0x8818010C,  //  001B  GETMBR	R6	R0	K12
      0x781A0004,  //  001C  JMPF	R6	#0022
      0x8C18010C,  //  001D  GETMET	R6	R0	K12
      0x5C200600,  //  001E  MOVE	R8	R3
      0x5C240800,  //  001F  MOVE	R9	R4
      0x5C280A00,  //  0020  MOVE	R10	R5
      0x7C180800,  //  0021  CALL	R6	4
      0x8818010D,  //  0022  GETMBR	R6	R0	K13
      0x14180406,  //  0023  LT	R6	R2	R6
      0x781A0004,  //  0024  JMPF	R6	#002A
      0x88180102,  //  0025  GETMBR	R6	R0	K2
      0x8C180D03,  //  0026  GETMET	R6	R6	K3
      0x7C180200,  //  0027  CALL	R6	1
      0x5C0C0C00,  //  0028  MOVE	R3	R6
      0x70020000,  //  0029  JMP		#002B
      0x4C0C0000,  //  002A  LDNIL	R3
      0x7001FFDD,  //  002B  JMP		#000A
      0x8C10010E,  //  002C  GETMET	R4	R0	K14
      0x7C100200,  //  002D  CALL	R4	1
      0x80000000,  //  002E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_UDPServer_every_second,   /* name */
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
** Solidified function: stop
********************************************************************/
be_local_closure(Matter_UDPServer_stop,   /* name */
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
    /* K0   */  be_nested_str_weak(listening),
    /* K1   */  be_nested_str_weak(udp_socket),
    /* K2   */  be_nested_str_weak(stop),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(remove_driver),
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
      0x5C0C0000,  //  0009  MOVE	R3	R0
      0x7C040400,  //  000A  CALL	R1	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_response
********************************************************************/
be_local_closure(Matter_UDPServer_send_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
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
    /* K3   */  be_nested_str_weak(udp_socket),
    /* K4   */  be_nested_str_weak(packets_sent),
    /* K5   */  be_nested_str_weak(push),
    }),
    be_str_weak(send_response),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xB8160000,  //  0000  GETNGBL	R5	K0
      0x8C140B01,  //  0001  GETMET	R5	R5	K1
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x5C200400,  //  0003  MOVE	R8	R2
      0x5C240600,  //  0004  MOVE	R9	R3
      0x5C280800,  //  0005  MOVE	R10	R4
      0x7C140A00,  //  0006  CALL	R5	5
      0x8C180B02,  //  0007  GETMET	R6	R5	K2
      0x88200103,  //  0008  GETMBR	R8	R0	K3
      0x7C180400,  //  0009  CALL	R6	2
      0x78120003,  //  000A  JMPF	R4	#000F
      0x88180104,  //  000B  GETMBR	R6	R0	K4
      0x8C180D05,  //  000C  GETMET	R6	R6	K5
      0x5C200A00,  //  000D  MOVE	R8	R5
      0x7C180400,  //  000E  CALL	R6	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_UDPServer
********************************************************************/
be_local_class(Matter_UDPServer,
    6,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(MAX_PACKETS_READ, 14), be_const_int(4) },
        { be_const_key_weak(send_response, -1), be_const_closure(Matter_UDPServer_send_response_closure) },
        { be_const_key_weak(stop, -1), be_const_closure(Matter_UDPServer_stop_closure) },
        { be_const_key_weak(init, 12), be_const_closure(Matter_UDPServer_init_closure) },
        { be_const_key_weak(resend_packets, -1), be_const_closure(Matter_UDPServer_resend_packets_closure) },
        { be_const_key_weak(RETRIES, 2), be_const_int(4) },
        { be_const_key_weak(port, -1), be_const_var(1) },
        { be_const_key_weak(packet_ack, -1), be_const_closure(Matter_UDPServer_packet_ack_closure) },
        { be_const_key_weak(every_50ms, -1), be_const_closure(Matter_UDPServer_every_50ms_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_UDPServer_every_second_closure) },
        { be_const_key_weak(listening, -1), be_const_var(2) },
        { be_const_key_weak(dispatch_cb, -1), be_const_var(4) },
        { be_const_key_weak(address, -1), be_const_var(0) },
        { be_const_key_weak(packets_sent, 11), be_const_var(5) },
        { be_const_key_weak(udp_socket, -1), be_const_var(3) },
        { be_const_key_weak(start, 1), be_const_closure(Matter_UDPServer_start_closure) },
    })),
    be_str_weak(Matter_UDPServer)
);
/*******************************************************************/

void be_load_Matter_UDPServer_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_UDPServer);
    be_setglobal(vm, "Matter_UDPServer");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
