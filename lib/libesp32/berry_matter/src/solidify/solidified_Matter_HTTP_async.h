/* Solidification of Matter_HTTP_async.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_HTTP_async;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_HTTP_async_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(compile_re),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60140003,  //  0000  GETGBL	R5	G3
      0x5C180000,  //  0001  MOVE	R6	R0
      0x7C140200,  //  0002  CALL	R5	1
      0x8C140B00,  //  0003  GETMET	R5	R5	K0
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x5C240600,  //  0006  MOVE	R9	R3
      0x5C280800,  //  0007  MOVE	R10	R4
      0x7C140A00,  //  0008  CALL	R5	5
      0x8C140101,  //  0009  GETMET	R5	R0	K1
      0x7C140200,  //  000A  CALL	R5	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_failed
********************************************************************/
be_local_closure(Matter_HTTP_async_event_http_failed,   /* name */
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
    be_str_weak(event_http_failed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compile_re
********************************************************************/
be_local_closure(Matter_HTTP_async_compile_re,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(re),
    /* K1   */  be_nested_str_weak(global),
    /* K2   */  be_nested_str_weak(contains),
    /* K3   */  be_nested_str_weak(_re_http_status),
    /* K4   */  be_nested_str_weak(compile),
    /* K5   */  be_nested_str_weak(HTTP_STATUS_REGEX),
    /* K6   */  be_nested_str_weak(_re_http_header),
    /* K7   */  be_nested_str_weak(HTTP_HEADER_REGEX),
    /* K8   */  be_nested_str_weak(_re_http_body),
    /* K9   */  be_nested_str_weak(HTTP_BODY_REGEX),
    /* K10  */  be_nested_str_weak(_re_http_chunk),
    /* K11  */  be_nested_str_weak(HTTP_CHUNK_REGEX),
    }),
    be_str_weak(compile_re),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x7C080400,  //  0004  CALL	R2	2
      0x740A0013,  //  0005  JMPT	R2	#001A
      0xB80A0200,  //  0006  GETNGBL	R2	K1
      0x8C0C0304,  //  0007  GETMET	R3	R1	K4
      0x88140105,  //  0008  GETMBR	R5	R0	K5
      0x7C0C0400,  //  0009  CALL	R3	2
      0x900A0603,  //  000A  SETMBR	R2	K3	R3
      0xB80A0200,  //  000B  GETNGBL	R2	K1
      0x8C0C0304,  //  000C  GETMET	R3	R1	K4
      0x88140107,  //  000D  GETMBR	R5	R0	K7
      0x7C0C0400,  //  000E  CALL	R3	2
      0x900A0C03,  //  000F  SETMBR	R2	K6	R3
      0xB80A0200,  //  0010  GETNGBL	R2	K1
      0x8C0C0304,  //  0011  GETMET	R3	R1	K4
      0x88140109,  //  0012  GETMBR	R5	R0	K9
      0x7C0C0400,  //  0013  CALL	R3	2
      0x900A1003,  //  0014  SETMBR	R2	K8	R3
      0xB80A0200,  //  0015  GETNGBL	R2	K1
      0x8C0C0304,  //  0016  GETMET	R3	R1	K4
      0x8814010B,  //  0017  GETMBR	R5	R0	K11
      0x7C0C0400,  //  0018  CALL	R3	2
      0x900A1403,  //  0019  SETMBR	R2	K10	R3
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_response
********************************************************************/
be_local_closure(Matter_HTTP_async_parse_http_response,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(phase),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(parse_http_status_line),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(parse_http_headers),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(parse_http_payload),
    }),
    be_str_weak(parse_http_response),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C040301,  //  0001  EQ	R1	R1	K1
      0x78060002,  //  0002  JMPF	R1	#0006
      0x8C040102,  //  0003  GETMET	R1	R0	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x7002000A,  //  0005  JMP		#0011
      0x88040100,  //  0006  GETMBR	R1	R0	K0
      0x1C040303,  //  0007  EQ	R1	R1	K3
      0x78060002,  //  0008  JMPF	R1	#000C
      0x8C040104,  //  0009  GETMET	R1	R0	K4
      0x7C040200,  //  000A  CALL	R1	1
      0x70020004,  //  000B  JMP		#0011
      0x88040100,  //  000C  GETMBR	R1	R0	K0
      0x1C040305,  //  000D  EQ	R1	R1	K5
      0x78060001,  //  000E  JMPF	R1	#0011
      0x8C040106,  //  000F  GETMET	R1	R0	K6
      0x7C040200,  //  0010  CALL	R1	1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_available
********************************************************************/
be_local_closure(Matter_HTTP_async_event_available,   /* name */
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
    /* K0   */  be_nested_str_weak(receive),
    }),
    be_str_weak(event_available),
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
** Solidified function: receive
********************************************************************/
be_local_closure(Matter_HTTP_async_receive,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(tcp_connected),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(time_start),
    /* K4   */  be_nested_str_weak(timeout),
    /* K5   */  be_nested_str_weak(status),
    /* K6   */  be_nested_str_weak(close),
    /* K7   */  be_nested_str_weak(http_status),
    /* K8   */  be_nested_str_weak(event_http_timeout),
    /* K9   */  be_nested_str_weak(tcp),
    /* K10  */  be_nested_str_weak(available),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(read),
    /* K13  */  be_nested_str_weak(response),
    /* K14  */  be_nested_str_weak(parse_http_response),
    }),
    be_str_weak(receive),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x50080200,  //  0001  LDBOOL	R2	1	0
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0xB8060200,  //  0005  GETNGBL	R1	K1
      0x8C040302,  //  0006  GETMET	R1	R1	K2
      0x7C040200,  //  0007  CALL	R1	1
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x04040202,  //  0009  SUB	R1	R1	R2
      0x88080104,  //  000A  GETMBR	R2	R0	K4
      0x24040202,  //  000B  GT	R1	R1	R2
      0x78060008,  //  000C  JMPF	R1	#0016
      0x5405FFFC,  //  000D  LDINT	R1	-3
      0x90020A01,  //  000E  SETMBR	R0	K5	R1
      0x8C040106,  //  000F  GETMET	R1	R0	K6
      0x7C040200,  //  0010  CALL	R1	1
      0x5405FFFD,  //  0011  LDINT	R1	-2
      0x90020E01,  //  0012  SETMBR	R0	K7	R1
      0x8C040108,  //  0013  GETMET	R1	R0	K8
      0x7C040200,  //  0014  CALL	R1	1
      0x70020018,  //  0015  JMP		#002F
      0x88040109,  //  0016  GETMBR	R1	R0	K9
      0x8C04030A,  //  0017  GETMET	R1	R1	K10
      0x7C040200,  //  0018  CALL	R1	1
      0x5808000B,  //  0019  LDCONST	R2	K11
      0x240C030B,  //  001A  GT	R3	R1	K11
      0x780E000E,  //  001B  JMPF	R3	#002B
      0x880C0109,  //  001C  GETMBR	R3	R0	K9
      0x8C0C070C,  //  001D  GETMET	R3	R3	K12
      0x7C0C0200,  //  001E  CALL	R3	1
      0x8810010D,  //  001F  GETMBR	R4	R0	K13
      0x00100803,  //  0020  ADD	R4	R4	R3
      0x90021A04,  //  0021  SETMBR	R0	K13	R4
      0x6010000C,  //  0022  GETGBL	R4	G12
      0x5C140600,  //  0023  MOVE	R5	R3
      0x7C100200,  //  0024  CALL	R4	1
      0x00080404,  //  0025  ADD	R2	R2	R4
      0x88100109,  //  0026  GETMBR	R4	R0	K9
      0x8C10090A,  //  0027  GETMET	R4	R4	K10
      0x7C100200,  //  0028  CALL	R4	1
      0x5C040800,  //  0029  MOVE	R1	R4
      0x7001FFEE,  //  002A  JMP		#001A
      0x240C050B,  //  002B  GT	R3	R2	K11
      0x780E0001,  //  002C  JMPF	R3	#002F
      0x8C0C010E,  //  002D  GETMET	R3	R0	K14
      0x7C0C0200,  //  002E  CALL	R3	1
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(Matter_HTTP_async_reset,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(reset),
    /* K1   */  be_nested_str_weak(cmd),
    /* K2   */  be_nested_str_weak(response),
    /* K3   */  be_nested_str_weak(response_offset),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(payload),
    /* K6   */  be_nested_str_weak(),
    /* K7   */  be_nested_str_weak(phase),
    /* K8   */  be_nested_str_weak(http_status),
    /* K9   */  be_nested_str_weak(is_chunked),
    /* K10  */  be_nested_str_weak(chunk_size),
    }),
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x4C040000,  //  0005  LDNIL	R1
      0x90020201,  //  0006  SETMBR	R0	K1	R1
      0x4C040000,  //  0007  LDNIL	R1
      0x90020401,  //  0008  SETMBR	R0	K2	R1
      0x90020704,  //  0009  SETMBR	R0	K3	K4
      0x90020B06,  //  000A  SETMBR	R0	K5	K6
      0x90020F04,  //  000B  SETMBR	R0	K7	K4
      0x90021104,  //  000C  SETMBR	R0	K8	K4
      0x50040000,  //  000D  LDBOOL	R1	0	0
      0x90021201,  //  000E  SETMBR	R0	K9	R1
      0x4C040000,  //  000F  LDNIL	R1
      0x90021401,  //  0010  SETMBR	R0	K10	R1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_status_code
********************************************************************/
be_local_closure(Matter_HTTP_async_event_http_status_code,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(status_code),
    }),
    be_str_weak(event_http_status_code),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_payload
********************************************************************/
be_local_closure(Matter_HTTP_async_parse_http_payload,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(is_chunked),
    /* K1   */  be_nested_str_weak(chunk_size),
    /* K2   */  be_nested_str_weak(global),
    /* K3   */  be_nested_str_weak(_re_http_chunk),
    /* K4   */  be_nested_str_weak(match2),
    /* K5   */  be_nested_str_weak(response),
    /* K6   */  be_nested_str_weak(response_offset),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(0x),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(status),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(),
    /* K13  */  be_nested_str_weak(close),
    /* K14  */  be_nested_str_weak(payload),
    /* K15  */  be_const_int(2147483647),
    }),
    be_str_weak(parse_http_payload),
    &be_const_str_solidified,
    ( &(const binstruction[82]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060045,  //  0001  JMPF	R1	#0048
      0x50040200,  //  0002  LDBOOL	R1	1	0
      0x78060042,  //  0003  JMPF	R1	#0047
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x78060018,  //  0007  JMPF	R1	#0021
      0xB8060400,  //  0008  GETNGBL	R1	K2
      0x88040303,  //  0009  GETMBR	R1	R1	K3
      0x8C040304,  //  000A  GETMET	R1	R1	K4
      0x880C0105,  //  000B  GETMBR	R3	R0	K5
      0x88100106,  //  000C  GETMBR	R4	R0	K6
      0x7C040600,  //  000D  CALL	R1	3
      0x78060011,  //  000E  JMPF	R1	#0021
      0x88080106,  //  000F  GETMBR	R2	R0	K6
      0x940C0307,  //  0010  GETIDX	R3	R1	K7
      0x00080403,  //  0011  ADD	R2	R2	R3
      0x90020C02,  //  0012  SETMBR	R0	K6	R2
      0x60080009,  //  0013  GETGBL	R2	G9
      0x940C0309,  //  0014  GETIDX	R3	R1	K9
      0x000E1003,  //  0015  ADD	R3	K8	R3
      0x7C080200,  //  0016  CALL	R2	1
      0x90020202,  //  0017  SETMBR	R0	K1	R2
      0x88080101,  //  0018  GETMBR	R2	R0	K1
      0x1C080507,  //  0019  EQ	R2	R2	K7
      0x780A0005,  //  001A  JMPF	R2	#0021
      0x9002150B,  //  001B  SETMBR	R0	K10	K11
      0x90020B0C,  //  001C  SETMBR	R0	K5	K12
      0x90020D07,  //  001D  SETMBR	R0	K6	K7
      0x8C08010D,  //  001E  GETMET	R2	R0	K13
      0x7C080200,  //  001F  CALL	R2	1
      0x80000400,  //  0020  RET	0
      0x88040101,  //  0021  GETMBR	R1	R0	K1
      0x4C080000,  //  0022  LDNIL	R2
      0x20040202,  //  0023  NE	R1	R1	R2
      0x7806001F,  //  0024  JMPF	R1	#0045
      0x88040101,  //  0025  GETMBR	R1	R0	K1
      0x6008000C,  //  0026  GETGBL	R2	G12
      0x880C0105,  //  0027  GETMBR	R3	R0	K5
      0x7C080200,  //  0028  CALL	R2	1
      0x880C0106,  //  0029  GETMBR	R3	R0	K6
      0x04080403,  //  002A  SUB	R2	R2	R3
      0x18040202,  //  002B  LE	R1	R1	R2
      0x78060015,  //  002C  JMPF	R1	#0043
      0x88080106,  //  002D  GETMBR	R2	R0	K6
      0x880C0106,  //  002E  GETMBR	R3	R0	K6
      0x88100101,  //  002F  GETMBR	R4	R0	K1
      0x000C0604,  //  0030  ADD	R3	R3	R4
      0x040C0709,  //  0031  SUB	R3	R3	K9
      0x40080403,  //  0032  CONNECT	R2	R2	R3
      0x880C0105,  //  0033  GETMBR	R3	R0	K5
      0x8804010E,  //  0034  GETMBR	R1	R0	K14
      0x94080602,  //  0035  GETIDX	R2	R3	R2
      0x00040202,  //  0036  ADD	R1	R1	R2
      0x90021C01,  //  0037  SETMBR	R0	K14	R1
      0x88040106,  //  0038  GETMBR	R1	R0	K6
      0x88080101,  //  0039  GETMBR	R2	R0	K1
      0x00040202,  //  003A  ADD	R1	R1	R2
      0x4004030F,  //  003B  CONNECT	R1	R1	K15
      0x88080105,  //  003C  GETMBR	R2	R0	K5
      0x94040401,  //  003D  GETIDX	R1	R2	R1
      0x90020A01,  //  003E  SETMBR	R0	K5	R1
      0x90020D07,  //  003F  SETMBR	R0	K6	K7
      0x4C040000,  //  0040  LDNIL	R1
      0x90020201,  //  0041  SETMBR	R0	K1	R1
      0x70020000,  //  0042  JMP		#0044
      0x80000200,  //  0043  RET	0
      0x70020000,  //  0044  JMP		#0046
      0x80000200,  //  0045  RET	0
      0x7001FFBA,  //  0046  JMP		#0002
      0x70020008,  //  0047  JMP		#0051
      0x88080106,  //  0048  GETMBR	R2	R0	K6
      0x4008050F,  //  0049  CONNECT	R2	R2	K15
      0x880C0105,  //  004A  GETMBR	R3	R0	K5
      0x8804010E,  //  004B  GETMBR	R1	R0	K14
      0x94080602,  //  004C  GETIDX	R2	R3	R2
      0x00040202,  //  004D  ADD	R1	R1	R2
      0x90021C01,  //  004E  SETMBR	R0	K14	R1
      0x90020B0C,  //  004F  SETMBR	R0	K5	K12
      0x90020D07,  //  0050  SETMBR	R0	K6	K7
      0x80000000,  //  0051  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_http
********************************************************************/
be_local_closure(Matter_HTTP_async_send_http,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(response),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(addr),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(_X3A),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(_X5B),
    /* K8   */  be_nested_str_weak(_X5D),
    /* K9   */  be_nested_str_weak(HTTP_GET),
    /* K10  */  be_nested_str_weak(cmd),
    /* K11  */  be_nested_str_weak(port),
    /* K12  */  be_nested_str_weak(write),
    /* K13  */  be_nested_str_weak(close),
    /* K14  */  be_nested_str_weak(status),
    /* K15  */  be_nested_str_weak(http_status),
    /* K16  */  be_nested_str_weak(event_http_failed),
    }),
    be_str_weak(send_http),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x90020302,  //  0001  SETMBR	R0	K1	K2
      0x88080103,  //  0002  GETMBR	R2	R0	K3
      0x8C0C0304,  //  0003  GETMET	R3	R1	K4
      0x5C140400,  //  0004  MOVE	R5	R2
      0x58180005,  //  0005  LDCONST	R6	K5
      0x7C0C0600,  //  0006  CALL	R3	3
      0x280C0706,  //  0007  GE	R3	R3	K6
      0x780E0002,  //  0008  JMPF	R3	#000C
      0x000E0E02,  //  0009  ADD	R3	K7	R2
      0x000C0708,  //  000A  ADD	R3	R3	K8
      0x5C080600,  //  000B  MOVE	R2	R3
      0x600C0018,  //  000C  GETGBL	R3	G24
      0x88100109,  //  000D  GETMBR	R4	R0	K9
      0x8814010A,  //  000E  GETMBR	R5	R0	K10
      0x5C180400,  //  000F  MOVE	R6	R2
      0x881C010B,  //  0010  GETMBR	R7	R0	K11
      0x7C0C0800,  //  0011  CALL	R3	4
      0x8C10010C,  //  0012  GETMET	R4	R0	K12
      0x5C180600,  //  0013  MOVE	R6	R3
      0x7C100400,  //  0014  CALL	R4	2
      0x6014000C,  //  0015  GETGBL	R5	G12
      0x5C180600,  //  0016  MOVE	R6	R3
      0x7C140200,  //  0017  CALL	R5	1
      0x20140805,  //  0018  NE	R5	R4	R5
      0x78160007,  //  0019  JMPF	R5	#0022
      0x8C14010D,  //  001A  GETMET	R5	R0	K13
      0x7C140200,  //  001B  CALL	R5	1
      0x5415FFFB,  //  001C  LDINT	R5	-4
      0x90021C05,  //  001D  SETMBR	R0	K14	R5
      0x5415FFFE,  //  001E  LDINT	R5	-1
      0x90021E05,  //  001F  SETMBR	R0	K15	R5
      0x8C140110,  //  0020  GETMET	R5	R0	K16
      0x7C140200,  //  0021  CALL	R5	1
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin_sync
********************************************************************/
be_local_closure(Matter_HTTP_async_begin_sync,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(timeout),
    /* K1   */  be_nested_str_weak(set_timeout),
    /* K2   */  be_nested_str_weak(SPINLOCK),
    /* K3   */  be_nested_str_weak(begin),
    /* K4   */  be_nested_str_weak(http_status),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(loop),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(delay),
    /* K9   */  be_nested_str_weak(payload),
    }),
    be_str_weak(begin_sync),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x4C100000,  //  0001  LDNIL	R4
      0x20100404,  //  0002  NE	R4	R2	R4
      0x78120002,  //  0003  JMPF	R4	#0007
      0x8C100101,  //  0004  GETMET	R4	R0	K1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x4C100000,  //  0007  LDNIL	R4
      0x88140102,  //  0008  GETMBR	R5	R0	K2
      0x8C180103,  //  0009  GETMET	R6	R0	K3
      0x5C200200,  //  000A  MOVE	R8	R1
      0x7C180400,  //  000B  CALL	R6	2
      0x781A0009,  //  000C  JMPF	R6	#0017
      0x881C0104,  //  000D  GETMBR	R7	R0	K4
      0x1C1C0F05,  //  000E  EQ	R7	R7	K5
      0x781E0006,  //  000F  JMPF	R7	#0017
      0x8C1C0106,  //  0010  GETMET	R7	R0	K6
      0x7C1C0200,  //  0011  CALL	R7	1
      0xB81E0E00,  //  0012  GETNGBL	R7	K7
      0x8C1C0F08,  //  0013  GETMET	R7	R7	K8
      0x5C240A00,  //  0014  MOVE	R9	R5
      0x7C1C0400,  //  0015  CALL	R7	2
      0x7001FFF5,  //  0016  JMP		#000D
      0x8C1C0101,  //  0017  GETMET	R7	R0	K1
      0x5C240600,  //  0018  MOVE	R9	R3
      0x7C1C0400,  //  0019  CALL	R7	2
      0x881C0104,  //  001A  GETMBR	R7	R0	K4
      0x241C0F05,  //  001B  GT	R7	R7	K5
      0x781E0001,  //  001C  JMPF	R7	#001F
      0x881C0109,  //  001D  GETMBR	R7	R0	K9
      0x70020000,  //  001E  JMP		#0020
      0x4C1C0000,  //  001F  LDNIL	R7
      0x80040E00,  //  0020  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_finished
********************************************************************/
be_local_closure(Matter_HTTP_async_event_http_finished,   /* name */
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
    be_str_weak(event_http_finished),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_timeout
********************************************************************/
be_local_closure(Matter_HTTP_async_event_timeout,   /* name */
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
    /* K0   */  be_nested_str_weak(http_status),
    /* K1   */  be_nested_str_weak(event_http_timeout),
    }),
    be_str_weak(event_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x5405FFFD,  //  0000  LDINT	R1	-2
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_established
********************************************************************/
be_local_closure(Matter_HTTP_async_event_established,   /* name */
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
    /* K0   */  be_nested_str_weak(send_http),
    }),
    be_str_weak(event_established),
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
** Solidified function: event_refused
********************************************************************/
be_local_closure(Matter_HTTP_async_event_refused,   /* name */
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
    /* K0   */  be_nested_str_weak(http_status),
    /* K1   */  be_nested_str_weak(event_http_failed),
    }),
    be_str_weak(event_refused),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x5405FFFE,  //  0000  LDINT	R1	-1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_closed
********************************************************************/
be_local_closure(Matter_HTTP_async_event_closed,   /* name */
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
    /* K0   */  be_nested_str_weak(http_status),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(event_http_finished),
    }),
    be_str_weak(event_closed),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x1C040301,  //  0001  EQ	R1	R1	K1
      0x78060002,  //  0002  JMPF	R1	#0006
      0x90020102,  //  0003  SETMBR	R0	K0	K2
      0x8C040103,  //  0004  GETMET	R1	R0	K3
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_status_line
********************************************************************/
be_local_closure(Matter_HTTP_async_parse_http_status_line,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(_re_http_status),
    /* K2   */  be_nested_str_weak(match2),
    /* K3   */  be_nested_str_weak(response),
    /* K4   */  be_nested_str_weak(response_offset),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(status_code),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(phase),
    /* K9   */  be_nested_str_weak(parse_http_headers),
    /* K10  */  be_nested_str_weak(close),
    /* K11  */  be_nested_str_weak(status),
    }),
    be_str_weak(parse_http_status_line),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C040302,  //  0002  GETMET	R1	R1	K2
      0x880C0103,  //  0003  GETMBR	R3	R0	K3
      0x88100104,  //  0004  GETMBR	R4	R0	K4
      0x7C040600,  //  0005  CALL	R1	3
      0x78060009,  //  0006  JMPF	R1	#0011
      0x94080305,  //  0007  GETIDX	R2	R1	K5
      0x90020802,  //  0008  SETMBR	R0	K4	R2
      0x60080009,  //  0009  GETGBL	R2	G9
      0x940C0307,  //  000A  GETIDX	R3	R1	K7
      0x7C080200,  //  000B  CALL	R2	1
      0x90020C02,  //  000C  SETMBR	R0	K6	R2
      0x90021107,  //  000D  SETMBR	R0	K8	K7
      0x8C080109,  //  000E  GETMET	R2	R0	K9
      0x7C080200,  //  000F  CALL	R2	1
      0x70020009,  //  0010  JMP		#001B
      0x6008000C,  //  0011  GETGBL	R2	G12
      0x880C0103,  //  0012  GETMBR	R3	R0	K3
      0x7C080200,  //  0013  CALL	R2	1
      0x540E0063,  //  0014  LDINT	R3	100
      0x24080403,  //  0015  GT	R2	R2	R3
      0x780A0003,  //  0016  JMPF	R2	#001B
      0x8C08010A,  //  0017  GETMET	R2	R0	K10
      0x7C080200,  //  0018  CALL	R2	1
      0x5409FFFB,  //  0019  LDINT	R2	-4
      0x90021602,  //  001A  SETMBR	R0	K11	R2
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_headers
********************************************************************/
be_local_closure(Matter_HTTP_async_parse_http_headers,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(global),
    /* K1   */  be_nested_str_weak(_re_http_header),
    /* K2   */  be_nested_str_weak(match2),
    /* K3   */  be_nested_str_weak(response),
    /* K4   */  be_nested_str_weak(response_offset),
    /* K5   */  be_nested_str_weak(event_http_header),
    /* K6   */  be_const_int(1),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(_re_http_body),
    /* K10  */  be_nested_str_weak(event_http_headers_end),
    /* K11  */  be_nested_str_weak(phase),
    /* K12  */  be_nested_str_weak(parse_http_payload),
    /* K13  */  be_nested_str_weak(close),
    /* K14  */  be_nested_str_weak(status),
    /* K15  */  be_nested_str_weak(http_status),
    /* K16  */  be_nested_str_weak(event_http_failed),
    }),
    be_str_weak(parse_http_headers),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x50040200,  //  0000  LDBOOL	R1	1	0
      0x7806002B,  //  0001  JMPF	R1	#002E
      0xB8060000,  //  0002  GETNGBL	R1	K0
      0x88040301,  //  0003  GETMBR	R1	R1	K1
      0x8C040302,  //  0004  GETMET	R1	R1	K2
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x88100104,  //  0006  GETMBR	R4	R0	K4
      0x7C040600,  //  0007  CALL	R1	3
      0x78060008,  //  0008  JMPF	R1	#0012
      0x8C080105,  //  0009  GETMET	R2	R0	K5
      0x94100306,  //  000A  GETIDX	R4	R1	K6
      0x94140307,  //  000B  GETIDX	R5	R1	K7
      0x7C080600,  //  000C  CALL	R2	3
      0x88080104,  //  000D  GETMBR	R2	R0	K4
      0x940C0308,  //  000E  GETIDX	R3	R1	K8
      0x00080403,  //  000F  ADD	R2	R2	R3
      0x90020802,  //  0010  SETMBR	R0	K4	R2
      0x7002001A,  //  0011  JMP		#002D
      0xB80A0000,  //  0012  GETNGBL	R2	K0
      0x88080509,  //  0013  GETMBR	R2	R2	K9
      0x8C080502,  //  0014  GETMET	R2	R2	K2
      0x88100103,  //  0015  GETMBR	R4	R0	K3
      0x88140104,  //  0016  GETMBR	R5	R0	K4
      0x7C080600,  //  0017  CALL	R2	3
      0x780A0004,  //  0018  JMPF	R2	#001E
      0x8C0C010A,  //  0019  GETMET	R3	R0	K10
      0x7C0C0200,  //  001A  CALL	R3	1
      0x90021707,  //  001B  SETMBR	R0	K11	K7
      0x8C0C010C,  //  001C  GETMET	R3	R0	K12
      0x7C0C0200,  //  001D  CALL	R3	1
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x88100103,  //  001F  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0020  CALL	R3	1
      0x541203FF,  //  0021  LDINT	R4	1024
      0x240C0604,  //  0022  GT	R3	R3	R4
      0x780E0007,  //  0023  JMPF	R3	#002C
      0x8C0C010D,  //  0024  GETMET	R3	R0	K13
      0x7C0C0200,  //  0025  CALL	R3	1
      0x540DFFFB,  //  0026  LDINT	R3	-4
      0x90021C03,  //  0027  SETMBR	R0	K14	R3
      0x540DFFFE,  //  0028  LDINT	R3	-1
      0x90021E03,  //  0029  SETMBR	R0	K15	R3
      0x8C0C0110,  //  002A  GETMET	R3	R0	K16
      0x7C0C0200,  //  002B  CALL	R3	1
      0x80000600,  //  002C  RET	0
      0x7001FFD1,  //  002D  JMP		#0000
      0x80000000,  //  002E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_timeout
********************************************************************/
be_local_closure(Matter_HTTP_async_event_http_timeout,   /* name */
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
    be_str_weak(event_http_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_headers_end
********************************************************************/
be_local_closure(Matter_HTTP_async_event_http_headers_end,   /* name */
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
    /* K0   */  be_nested_str_weak(response_offset),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(response),
    /* K3   */  be_const_int(2147483647),
    }),
    be_str_weak(event_http_headers_end),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x24040301,  //  0001  GT	R1	R1	K1
      0x78060005,  //  0002  JMPF	R1	#0009
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x40040303,  //  0004  CONNECT	R1	R1	K3
      0x88080102,  //  0005  GETMBR	R2	R0	K2
      0x94040401,  //  0006  GETIDX	R1	R2	R1
      0x90020401,  //  0007  SETMBR	R0	K2	R1
      0x90020101,  //  0008  SETMBR	R0	K0	K1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_header
********************************************************************/
be_local_closure(Matter_HTTP_async_event_http_header,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tolower),
    /* K2   */  be_nested_str_weak(transfer_X2Dencoding),
    /* K3   */  be_nested_str_weak(chunked),
    /* K4   */  be_nested_str_weak(is_chunked),
    }),
    be_str_weak(event_http_header),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100701,  //  0001  GETMET	R4	R3	K1
      0x5C180200,  //  0002  MOVE	R6	R1
      0x7C100400,  //  0003  CALL	R4	2
      0x5C040800,  //  0004  MOVE	R1	R4
      0x8C100701,  //  0005  GETMET	R4	R3	K1
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C100400,  //  0007  CALL	R4	2
      0x5C080800,  //  0008  MOVE	R2	R4
      0x1C100302,  //  0009  EQ	R4	R1	K2
      0x78120006,  //  000A  JMPF	R4	#0012
      0x8C100701,  //  000B  GETMET	R4	R3	K1
      0x5C180400,  //  000C  MOVE	R6	R2
      0x7C100400,  //  000D  CALL	R4	2
      0x1C100903,  //  000E  EQ	R4	R4	K3
      0x78120001,  //  000F  JMPF	R4	#0012
      0x50100200,  //  0010  LDBOOL	R4	1	0
      0x90020804,  //  0011  SETMBR	R0	K4	R4
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(Matter_HTTP_async_begin,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(begin),
    /* K1   */  be_nested_str_weak(cmd),
    }),
    be_str_weak(begin),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x7C080200,  //  0004  CALL	R2	1
      0x90020201,  //  0005  SETMBR	R0	K1	R1
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_HTTP_async
********************************************************************/
extern const bclass be_class_Matter_TCP_async;
be_local_class(Matter_HTTP_async,
    9,
    &be_class_Matter_TCP_async,
    be_nested_map(37,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, 2), be_const_closure(Matter_HTTP_async_init_closure) },
        { be_const_key_weak(response, 28), be_const_var(1) },
        { be_const_key_weak(http_status, -1), be_const_var(5) },
        { be_const_key_weak(HTTP_STATUS_REGEX, 14), be_nested_str_weak(HTTP_X2F1_X5C_X2E_X5B0_X2D1_X5D_X20_X28_X5Cd_X2B_X29_X20_X2E_X2A_X3F_X0D_X0A) },
        { be_const_key_weak(HTTP_GET, 8), be_nested_str_weak(GET_X20_X25s_X20HTTP_X2F1_X2E1_X0D_X0AHost_X20_X25s_X3A_X25s_X0D_X0AConnection_X3A_X20close_X0D_X0A_X0D_X0A) },
        { be_const_key_weak(response_offset, -1), be_const_var(2) },
        { be_const_key_weak(compile_re, -1), be_const_closure(Matter_HTTP_async_compile_re_closure) },
        { be_const_key_weak(event_http_header, -1), be_const_closure(Matter_HTTP_async_event_http_header_closure) },
        { be_const_key_weak(event_http_status_code, -1), be_const_closure(Matter_HTTP_async_event_http_status_code_closure) },
        { be_const_key_weak(payload, -1), be_const_var(4) },
        { be_const_key_weak(event_available, -1), be_const_closure(Matter_HTTP_async_event_available_closure) },
        { be_const_key_weak(chunk_size, 16), be_const_var(8) },
        { be_const_key_weak(event_http_timeout, -1), be_const_closure(Matter_HTTP_async_event_http_timeout_closure) },
        { be_const_key_weak(parse_http_payload, -1), be_const_closure(Matter_HTTP_async_parse_http_payload_closure) },
        { be_const_key_weak(parse_http_status_line, 29), be_const_closure(Matter_HTTP_async_parse_http_status_line_closure) },
        { be_const_key_weak(begin_sync, -1), be_const_closure(Matter_HTTP_async_begin_sync_closure) },
        { be_const_key_weak(cmd, -1), be_const_var(0) },
        { be_const_key_weak(event_http_failed, 13), be_const_closure(Matter_HTTP_async_event_http_failed_closure) },
        { be_const_key_weak(SPINLOCK, -1), be_const_int(5) },
        { be_const_key_weak(reset, 31), be_const_closure(Matter_HTTP_async_reset_closure) },
        { be_const_key_weak(send_http, -1), be_const_closure(Matter_HTTP_async_send_http_closure) },
        { be_const_key_weak(parse_http_response, 15), be_const_closure(Matter_HTTP_async_parse_http_response_closure) },
        { be_const_key_weak(event_http_finished, -1), be_const_closure(Matter_HTTP_async_event_http_finished_closure) },
        { be_const_key_weak(HTTP_BODY_REGEX, -1), be_nested_str_weak(_X0D_X0A) },
        { be_const_key_weak(event_refused, -1), be_const_closure(Matter_HTTP_async_event_refused_closure) },
        { be_const_key_weak(HTTP_HEADER_REGEX, -1), be_nested_str_weak(_X28_X5BA_X2DZa_X2Dz0_X2D9_X2D_X5D_X2B_X29_X3A_X20_X28_X2E_X2A_X3F_X29_X0D_X0A) },
        { be_const_key_weak(is_chunked, 25), be_const_var(7) },
        { be_const_key_weak(event_established, 24), be_const_closure(Matter_HTTP_async_event_established_closure) },
        { be_const_key_weak(HTTP_CHUNK_REGEX, -1), be_nested_str_weak(_X0D_X0A_X28_X5BA_X2DFa_X2Df0_X2D9_X5D_X2B_X29_X5B_X20_X09_X5D_X2A_X2E_X2A_X3F_X0D_X0A) },
        { be_const_key_weak(receive, -1), be_const_closure(Matter_HTTP_async_receive_closure) },
        { be_const_key_weak(event_closed, -1), be_const_closure(Matter_HTTP_async_event_closed_closure) },
        { be_const_key_weak(phase, -1), be_const_var(6) },
        { be_const_key_weak(parse_http_headers, -1), be_const_closure(Matter_HTTP_async_parse_http_headers_closure) },
        { be_const_key_weak(event_timeout, 12), be_const_closure(Matter_HTTP_async_event_timeout_closure) },
        { be_const_key_weak(event_http_headers_end, -1), be_const_closure(Matter_HTTP_async_event_http_headers_end_closure) },
        { be_const_key_weak(status_code, 7), be_const_var(3) },
        { be_const_key_weak(begin, -1), be_const_closure(Matter_HTTP_async_begin_closure) },
    })),
    be_str_weak(Matter_HTTP_async)
);
/*******************************************************************/

void be_load_Matter_HTTP_async_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_HTTP_async);
    be_setglobal(vm, "Matter_HTTP_async");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
