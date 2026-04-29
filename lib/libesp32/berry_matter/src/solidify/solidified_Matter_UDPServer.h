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
// compact class 'Matter_UDPServer' ktab size: 59, total: 107 (saved 384 bytes)
static const bvalue be_ktab_class_Matter_UDPServer[59] = {
  /* K0   */  be_nested_str_weak(listening),
  /* K1   */  be_nested_str_weak(udp_socket),
  /* K2   */  be_nested_str_weak(close),
  /* K3   */  be_nested_str_weak(packets_sent),
  /* K4   */  be_nested_str_weak(matter),
  /* K5   */  be_nested_str_weak(profiler),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(read),
  /* K8   */  be_nested_str_weak(packet),
  /* K9   */  be_nested_str_weak(start),
  /* K10  */  be_const_int(1),
  /* K11  */  be_nested_str_weak(remote_ip),
  /* K12  */  be_nested_str_weak(remote_port),
  /* K13  */  be_nested_str_weak(dispatch_cb),
  /* K14  */  be_nested_str_weak(dump),
  /* K15  */  be_const_int(2),
  /* K16  */  be_nested_str_weak(MAX_PACKETS_READ),
  /* K17  */  be_nested_str_weak(_resend_packets),
  /* K18  */  be_const_class(be_class_Matter_UDPServer),
  /* K19  */  be_nested_str_weak(math),
  /* K20  */  be_nested_str_weak(rand),
  /* K21  */  be_const_real_hex(0x3FCCCCCD),
  /* K22  */  be_const_real_hex(0x3F800000),
  /* K23  */  be_const_real_hex(0x3E800000),
  /* K24  */  be_nested_str_weak(tasmota),
  /* K25  */  be_nested_str_weak(time_reached),
  /* K26  */  be_nested_str_weak(next_try),
  /* K27  */  be_nested_str_weak(retries),
  /* K28  */  be_nested_str_weak(RETRIES),
  /* K29  */  be_nested_str_weak(log),
  /* K30  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Resending_X20packet_X20id_X3D_X25s_X20packet_X2Eretries_X3D_X25s),
  /* K31  */  be_nested_str_weak(msg_id),
  /* K32  */  be_const_int(3),
  /* K33  */  be_nested_str_weak(send),
  /* K34  */  be_nested_str_weak(millis),
  /* K35  */  be_nested_str_weak(_backoff_time),
  /* K36  */  be_nested_str_weak(remove),
  /* K37  */  be_nested_str_weak(MTR_X3A_X20_X2E_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20Unacked_X20packet_X20_X27_X5B_X25s_X5D_X3A_X25i_X27_X20msg_id_X3D_X25i),
  /* K38  */  be_nested_str_weak(session_id),
  /* K39  */  be_nested_str_weak(addr),
  /* K40  */  be_nested_str_weak(port),
  /* K41  */  be_nested_str_weak(loop),
  /* K42  */  be_nested_str_weak(device),
  /* K43  */  be_nested_str_weak(),
  /* K44  */  be_nested_str_weak(loop_cb),
  /* K45  */  be_nested_str_weak(raw),
  /* K46  */  be_nested_str_weak(loglevel),
  /* K47  */  be_nested_str_weak(MTR_X3A_X20error_X20sending_X20packet_X20to_X20_X27_X5B_X25s_X5D_X3A_X25i_X27),
  /* K48  */  be_nested_str_weak(ack_message_counter),
  /* K49  */  be_nested_str_weak(exchange_id),
  /* K50  */  be_nested_str_weak(UDPPacket_sent),
  /* K51  */  be_nested_str_weak(push),
  /* K52  */  be_nested_str_weak(udp),
  /* K53  */  be_nested_str_weak(begin),
  /* K54  */  be_nested_str_weak(MTR_X3A_X20error_X20reopening_X20UDP_X20server),
  /* K55  */  be_nested_str_weak(remove_fast_loop),
  /* K56  */  be_nested_str_weak(network_error),
  /* K57  */  be_nested_str_weak(could_X20not_X20open_X20UDP_X20server),
  /* K58  */  be_nested_str_weak(add_fast_loop),
};


extern const bclass be_class_Matter_UDPServer;

/********************************************************************
** Solidified function: flush_socket
********************************************************************/
be_local_closure(class_Matter_UDPServer_flush_socket,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(flush_socket),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x7806000B,  //  0001  JMPF	R1	#000E
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x4C080000,  //  0003  LDNIL	R2
      0x20040202,  //  0004  NE	R1	R1	R2
      0x78060007,  //  0005  JMPF	R1	#000E
      0x88040101,  //  0006  GETMBR	R1	R0	K1
      0x8C040302,  //  0007  GETMET	R1	R1	K2
      0x7C040200,  //  0008  CALL	R1	1
      0x4C040000,  //  0009  LDNIL	R1
      0x90020201,  //  000A  SETMBR	R0	K1	R1
      0x60040012,  //  000B  GETGBL	R1	G18
      0x7C040000,  //  000C  CALL	R1	0
      0x90020601,  //  000D  SETMBR	R0	K3	R1
      0x80000000,  //  000E  RET	0
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
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(loop),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0xB8060800,  //  0000  GETNGBL	R1	K4
      0x88040305,  //  0001  GETMBR	R1	R1	K5
      0x58080006,  //  0002  LDCONST	R2	K6
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x4C100000,  //  0004  LDNIL	R4
      0x1C0C0604,  //  0005  EQ	R3	R3	R4
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x80000600,  //  0007  RET	0
      0x880C0101,  //  0008  GETMBR	R3	R0	K1
      0x8C0C0707,  //  0009  GETMET	R3	R3	K7
      0x88140108,  //  000A  GETMBR	R5	R0	K8
      0x7C0C0400,  //  000B  CALL	R3	2
      0x4C100000,  //  000C  LDNIL	R4
      0x20100604,  //  000D  NE	R4	R3	R4
      0x7812001B,  //  000E  JMPF	R4	#002B
      0x8C100309,  //  000F  GETMET	R4	R1	K9
      0x7C100200,  //  0010  CALL	R4	1
      0x90021003,  //  0011  SETMBR	R0	K8	R3
      0x0008050A,  //  0012  ADD	R2	R2	K10
      0x88100101,  //  0013  GETMBR	R4	R0	K1
      0x8810090B,  //  0014  GETMBR	R4	R4	K11
      0x88140101,  //  0015  GETMBR	R5	R0	K1
      0x88140B0C,  //  0016  GETMBR	R5	R5	K12
      0x8818010D,  //  0017  GETMBR	R6	R0	K13
      0x781A0004,  //  0018  JMPF	R6	#001E
      0x8C18010D,  //  0019  GETMET	R6	R0	K13
      0x5C200600,  //  001A  MOVE	R8	R3
      0x5C240800,  //  001B  MOVE	R9	R4
      0x5C280A00,  //  001C  MOVE	R10	R5
      0x7C180800,  //  001D  CALL	R6	4
      0x8C18030E,  //  001E  GETMET	R6	R1	K14
      0x5820000F,  //  001F  LDCONST	R8	K15
      0x7C180400,  //  0020  CALL	R6	2
      0x88180110,  //  0021  GETMBR	R6	R0	K16
      0x14180406,  //  0022  LT	R6	R2	R6
      0x781A0004,  //  0023  JMPF	R6	#0029
      0x88180101,  //  0024  GETMBR	R6	R0	K1
      0x8C180D07,  //  0025  GETMET	R6	R6	K7
      0x7C180200,  //  0026  CALL	R6	1
      0x5C0C0C00,  //  0027  MOVE	R3	R6
      0x70020000,  //  0028  JMP		#002A
      0x4C0C0000,  //  0029  LDNIL	R3
      0x7001FFE0,  //  002A  JMP		#000C
      0x8C100111,  //  002B  GETMET	R4	R0	K17
      0x7C100200,  //  002C  CALL	R4	1
      0x80000000,  //  002D  RET	0
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
    12,                          /* varg */
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
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(_backoff_time),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x58040012,  //  0000  LDCONST	R1	K18
      0x84080000,  //  0001  CLOSURE	R2	P0
      0xA40E2600,  //  0002  IMPORT	R3	K19
      0x5412012B,  //  0003  LDINT	R4	300
      0x6014000A,  //  0004  GETGBL	R5	G10
      0x8C180714,  //  0005  GETMET	R6	R3	K20
      0x7C180200,  //  0006  CALL	R6	1
      0x541E00FE,  //  0007  LDINT	R7	255
      0x2C180C07,  //  0008  AND	R6	R6	R7
      0x7C140200,  //  0009  CALL	R5	1
      0x541A00FE,  //  000A  LDINT	R6	255
      0x0C140A06,  //  000B  DIV	R5	R5	R6
      0x24180106,  //  000C  GT	R6	R0	K6
      0x781A0001,  //  000D  JMPF	R6	#0010
      0x0418010A,  //  000E  SUB	R6	R0	K10
      0x70020000,  //  000F  JMP		#0011
      0x58180006,  //  0010  LDCONST	R6	K6
      0x5C1C0400,  //  0011  MOVE	R7	R2
      0x58200015,  //  0012  LDCONST	R8	K21
      0x5C240C00,  //  0013  MOVE	R9	R6
      0x7C1C0400,  //  0014  CALL	R7	2
      0x081C0807,  //  0015  MUL	R7	R4	R7
      0x08200B17,  //  0016  MUL	R8	R5	K23
      0x00222C08,  //  0017  ADD	R8	K22	R8
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
** Solidified function: _resend_packets
********************************************************************/
be_local_closure(class_Matter_UDPServer__resend_packets,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(_resend_packets),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0x58040006,  //  0000  LDCONST	R1	K6
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A0036,  //  0005  JMPF	R2	#003D
      0x540A0003,  //  0006  LDINT	R2	4
      0x14080202,  //  0007  LT	R2	R1	R2
      0x780A0033,  //  0008  JMPF	R2	#003D
      0x88080103,  //  0009  GETMBR	R2	R0	K3
      0x94080401,  //  000A  GETIDX	R2	R2	R1
      0xB80E3000,  //  000B  GETNGBL	R3	K24
      0x8C0C0719,  //  000C  GETMET	R3	R3	K25
      0x8814051A,  //  000D  GETMBR	R5	R2	K26
      0x7C0C0400,  //  000E  CALL	R3	2
      0x780E002A,  //  000F  JMPF	R3	#003B
      0x880C051B,  //  0010  GETMBR	R3	R2	K27
      0x8810011C,  //  0011  GETMBR	R4	R0	K28
      0x180C0604,  //  0012  LE	R3	R3	R4
      0x780E0017,  //  0013  JMPF	R3	#002C
      0xB80E3A00,  //  0014  GETNGBL	R3	K29
      0x60100018,  //  0015  GETGBL	R4	G24
      0x5814001E,  //  0016  LDCONST	R5	K30
      0x8818051F,  //  0017  GETMBR	R6	R2	K31
      0x881C051B,  //  0018  GETMBR	R7	R2	K27
      0x7C100600,  //  0019  CALL	R4	3
      0x58140020,  //  001A  LDCONST	R5	K32
      0x7C0C0400,  //  001B  CALL	R3	2
      0x8C0C0121,  //  001C  GETMET	R3	R0	K33
      0x5C140400,  //  001D  MOVE	R5	R2
      0x7C0C0400,  //  001E  CALL	R3	2
      0xB80E3000,  //  001F  GETNGBL	R3	K24
      0x8C0C0722,  //  0020  GETMET	R3	R3	K34
      0x7C0C0200,  //  0021  CALL	R3	1
      0x8C100123,  //  0022  GETMET	R4	R0	K35
      0x8818051B,  //  0023  GETMBR	R6	R2	K27
      0x7C100400,  //  0024  CALL	R4	2
      0x000C0604,  //  0025  ADD	R3	R3	R4
      0x900A3403,  //  0026  SETMBR	R2	K26	R3
      0x880C051B,  //  0027  GETMBR	R3	R2	K27
      0x000C070A,  //  0028  ADD	R3	R3	K10
      0x900A3603,  //  0029  SETMBR	R2	K27	R3
      0x0004030A,  //  002A  ADD	R1	R1	K10
      0x7002000D,  //  002B  JMP		#003A
      0x880C0103,  //  002C  GETMBR	R3	R0	K3
      0x8C0C0724,  //  002D  GETMET	R3	R3	K36
      0x5C140200,  //  002E  MOVE	R5	R1
      0x7C0C0400,  //  002F  CALL	R3	2
      0xB80E3A00,  //  0030  GETNGBL	R3	K29
      0x60100018,  //  0031  GETGBL	R4	G24
      0x58140025,  //  0032  LDCONST	R5	K37
      0x88180526,  //  0033  GETMBR	R6	R2	K38
      0x881C0527,  //  0034  GETMBR	R7	R2	K39
      0x88200528,  //  0035  GETMBR	R8	R2	K40
      0x8824051F,  //  0036  GETMBR	R9	R2	K31
      0x7C100A00,  //  0037  CALL	R4	5
      0x58140020,  //  0038  LDCONST	R5	K32
      0x7C0C0400,  //  0039  CALL	R3	2
      0x70020000,  //  003A  JMP		#003C
      0x0004030A,  //  003B  ADD	R1	R1	K10
      0x7001FFC3,  //  003C  JMP		#0001
      0x80000000,  //  003D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(class_Matter_UDPServer_every_50ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040129,  //  0000  GETMET	R1	R0	K41
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
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
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
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
    10,                          /* varg */
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
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x90025401,  //  0000  SETMBR	R0	K42	R1
      0x780A0001,  //  0001  JMPF	R2	#0004
      0x5C100400,  //  0002  MOVE	R4	R2
      0x70020000,  //  0003  JMP		#0005
      0x5810002B,  //  0004  LDCONST	R4	K43
      0x90024E04,  //  0005  SETMBR	R0	K39	R4
      0x780E0001,  //  0006  JMPF	R3	#0009
      0x5C100600,  //  0007  MOVE	R4	R3
      0x70020000,  //  0008  JMP		#000A
      0x541215A3,  //  0009  LDINT	R4	5540
      0x90025004,  //  000A  SETMBR	R0	K40	R4
      0x50100000,  //  000B  LDBOOL	R4	0	0
      0x90020004,  //  000C  SETMBR	R0	K0	R4
      0x60100012,  //  000D  GETGBL	R4	G18
      0x7C100000,  //  000E  CALL	R4	0
      0x90020604,  //  000F  SETMBR	R0	K3	R4
      0x84100000,  //  0010  CLOSURE	R4	P0
      0x90025804,  //  0011  SETMBR	R0	K44	R4
      0xA0000000,  //  0012  CLOSE	R0
      0x80000000,  //  0013  RET	0
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
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(send),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0001,  //  0003  JMPF	R2	#0006
      0x50080000,  //  0004  LDBOOL	R2	0	0
      0x80040400,  //  0005  RET	1	R2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x8C080521,  //  0007  GETMET	R2	R2	K33
      0x88100327,  //  0008  GETMBR	R4	R1	K39
      0x78120001,  //  0009  JMPF	R4	#000C
      0x88100327,  //  000A  GETMBR	R4	R1	K39
      0x70020001,  //  000B  JMP		#000E
      0x88100101,  //  000C  GETMBR	R4	R0	K1
      0x8810090B,  //  000D  GETMBR	R4	R4	K11
      0x88140328,  //  000E  GETMBR	R5	R1	K40
      0x78160001,  //  000F  JMPF	R5	#0012
      0x88140328,  //  0010  GETMBR	R5	R1	K40
      0x70020001,  //  0011  JMP		#0014
      0x88140101,  //  0012  GETMBR	R5	R0	K1
      0x88140B0C,  //  0013  GETMBR	R5	R5	K12
      0x8818032D,  //  0014  GETMBR	R6	R1	K45
      0x7C080800,  //  0015  CALL	R2	4
      0x780A0000,  //  0016  JMPF	R2	#0018
      0x7002000C,  //  0017  JMP		#0025
      0xB80E3000,  //  0018  GETNGBL	R3	K24
      0x8C0C072E,  //  0019  GETMET	R3	R3	K46
      0x58140020,  //  001A  LDCONST	R5	K32
      0x7C0C0400,  //  001B  CALL	R3	2
      0x780E0007,  //  001C  JMPF	R3	#0025
      0xB80E3A00,  //  001D  GETNGBL	R3	K29
      0x60100018,  //  001E  GETGBL	R4	G24
      0x5814002F,  //  001F  LDCONST	R5	K47
      0x88180327,  //  0020  GETMBR	R6	R1	K39
      0x881C0328,  //  0021  GETMBR	R7	R1	K40
      0x7C100600,  //  0022  CALL	R4	3
      0x58140020,  //  0023  LDCONST	R5	K32
      0x7C0C0400,  //  0024  CALL	R3	2
      0x80040400,  //  0025  RET	1	R2
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
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(received_ack),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x88080330,  //  0000  GETMBR	R2	R1	K48
      0x880C0331,  //  0001  GETMBR	R3	R1	K49
      0x4C100000,  //  0002  LDNIL	R4
      0x1C100404,  //  0003  EQ	R4	R2	R4
      0x78120000,  //  0004  JMPF	R4	#0006
      0x80000800,  //  0005  RET	0
      0x58100006,  //  0006  LDCONST	R4	K6
      0x6014000C,  //  0007  GETGBL	R5	G12
      0x88180103,  //  0008  GETMBR	R6	R0	K3
      0x7C140200,  //  0009  CALL	R5	1
      0x14140805,  //  000A  LT	R5	R4	R5
      0x7816000E,  //  000B  JMPF	R5	#001B
      0x88140103,  //  000C  GETMBR	R5	R0	K3
      0x94140A04,  //  000D  GETIDX	R5	R5	R4
      0x88180B1F,  //  000E  GETMBR	R6	R5	K31
      0x1C180C02,  //  000F  EQ	R6	R6	R2
      0x781A0007,  //  0010  JMPF	R6	#0019
      0x88180B31,  //  0011  GETMBR	R6	R5	K49
      0x1C180C03,  //  0012  EQ	R6	R6	R3
      0x781A0004,  //  0013  JMPF	R6	#0019
      0x88180103,  //  0014  GETMBR	R6	R0	K3
      0x8C180D24,  //  0015  GETMET	R6	R6	K36
      0x5C200800,  //  0016  MOVE	R8	R4
      0x7C180400,  //  0017  CALL	R6	2
      0x70020000,  //  0018  JMP		#001A
      0x0010090A,  //  0019  ADD	R4	R4	K10
      0x7001FFEB,  //  001A  JMP		#0007
      0x80000000,  //  001B  RET	0
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
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(send_UDP),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xB80A0800,  //  0000  GETNGBL	R2	K4
      0x8C080532,  //  0001  GETMET	R2	R2	K50
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x8C0C0121,  //  0004  GETMET	R3	R0	K33
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C0C0400,  //  0006  CALL	R3	2
      0x880C051F,  //  0007  GETMBR	R3	R2	K31
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C0103,  //  0009  GETMBR	R3	R0	K3
      0x8C0C0733,  //  000A  GETMET	R3	R3	K51
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reopen_socket
********************************************************************/
be_local_closure(class_Matter_UDPServer_reopen_socket,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(reopen_socket),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060011,  //  0001  JMPF	R1	#0014
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x4C080000,  //  0003  LDNIL	R2
      0x1C040202,  //  0004  EQ	R1	R1	R2
      0x7806000D,  //  0005  JMPF	R1	#0014
      0xB8066800,  //  0006  GETNGBL	R1	K52
      0x7C040000,  //  0007  CALL	R1	0
      0x90020201,  //  0008  SETMBR	R0	K1	R1
      0x88040101,  //  0009  GETMBR	R1	R0	K1
      0x8C040335,  //  000A  GETMET	R1	R1	K53
      0x880C0127,  //  000B  GETMBR	R3	R0	K39
      0x88100128,  //  000C  GETMBR	R4	R0	K40
      0x7C040600,  //  000D  CALL	R1	3
      0x5C080200,  //  000E  MOVE	R2	R1
      0x740A0003,  //  000F  JMPT	R2	#0014
      0xB80A3A00,  //  0010  GETNGBL	R2	K29
      0x580C0036,  //  0011  LDCONST	R3	K54
      0x5810000F,  //  0012  LDCONST	R4	K15
      0x7C080400,  //  0013  CALL	R2	2
      0x80000000,  //  0014  RET	0
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
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
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
      0xB8063000,  //  0007  GETNGBL	R1	K24
      0x8C040337,  //  0008  GETMET	R1	R1	K55
      0x880C012C,  //  0009  GETMBR	R3	R0	K44
      0x7C040400,  //  000A  CALL	R1	2
      0x80000000,  //  000B  RET	0
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
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_UDPServer,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0011,  //  0001  JMPT	R2	#0014
      0xB80A6800,  //  0002  GETNGBL	R2	K52
      0x7C080000,  //  0003  CALL	R2	0
      0x90020202,  //  0004  SETMBR	R0	K1	R2
      0x88080101,  //  0005  GETMBR	R2	R0	K1
      0x8C080535,  //  0006  GETMET	R2	R2	K53
      0x88100127,  //  0007  GETMBR	R4	R0	K39
      0x88140128,  //  0008  GETMBR	R5	R0	K40
      0x7C080600,  //  0009  CALL	R2	3
      0x5C0C0400,  //  000A  MOVE	R3	R2
      0x740E0000,  //  000B  JMPT	R3	#000D
      0xB0067139,  //  000C  RAISE	1	K56	K57
      0x500C0200,  //  000D  LDBOOL	R3	1	0
      0x90020003,  //  000E  SETMBR	R0	K0	R3
      0x90021A01,  //  000F  SETMBR	R0	K13	R1
      0xB80E3000,  //  0010  GETNGBL	R3	K24
      0x8C0C073A,  //  0011  GETMET	R3	R3	K58
      0x8814012C,  //  0012  GETMBR	R5	R0	K44
      0x7C0C0400,  //  0013  CALL	R3	2
      0x80000000,  //  0014  RET	0
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
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(udp_socket, 16), be_const_var(4) },
        { be_const_key_weak(start, -1), be_const_closure(class_Matter_UDPServer_start_closure) },
        { be_const_key_weak(addr, -1), be_const_var(0) },
        { be_const_key_weak(device, 14), be_const_var(2) },
        { be_const_key_weak(loop, -1), be_const_closure(class_Matter_UDPServer_loop_closure) },
        { be_const_key_weak(dispatch_cb, 10), be_const_var(5) },
        { be_const_key_weak(_backoff_time, -1), be_const_static_closure(class_Matter_UDPServer__backoff_time_closure) },
        { be_const_key_weak(_resend_packets, 15), be_const_closure(class_Matter_UDPServer__resend_packets_closure) },
        { be_const_key_weak(every_50ms, 21), be_const_closure(class_Matter_UDPServer_every_50ms_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_UDPServer_every_second_closure) },
        { be_const_key_weak(stop, 11), be_const_closure(class_Matter_UDPServer_stop_closure) },
        { be_const_key_weak(packets_sent, -1), be_const_var(6) },
        { be_const_key_weak(loop_cb, -1), be_const_var(7) },
        { be_const_key_weak(RETRIES, 12), be_const_int(5) },
        { be_const_key_weak(init, 4), be_const_closure(class_Matter_UDPServer_init_closure) },
        { be_const_key_weak(send, -1), be_const_closure(class_Matter_UDPServer_send_closure) },
        { be_const_key_weak(received_ack, -1), be_const_closure(class_Matter_UDPServer_received_ack_closure) },
        { be_const_key_weak(packet, -1), be_const_var(8) },
        { be_const_key_weak(listening, -1), be_const_var(3) },
        { be_const_key_weak(send_UDP, -1), be_const_closure(class_Matter_UDPServer_send_UDP_closure) },
        { be_const_key_weak(reopen_socket, -1), be_const_closure(class_Matter_UDPServer_reopen_socket_closure) },
        { be_const_key_weak(MAX_PACKETS_READ, -1), be_const_int(4) },
        { be_const_key_weak(port, -1), be_const_var(1) },
        { be_const_key_weak(flush_socket, 1), be_const_closure(class_Matter_UDPServer_flush_socket_closure) },
    })),
    be_str_weak(Matter_UDPServer)
);
/********************************************************************/
/* End of solidification */
