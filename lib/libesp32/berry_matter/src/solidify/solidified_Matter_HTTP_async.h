/* Solidification of Matter_HTTP_async.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_HTTP_async' ktab size: 74, total: 152 (saved 624 bytes)
static const bvalue be_ktab_class_Matter_HTTP_async[74] = {
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
  /* K16  */  be_nested_str_weak(string),
  /* K17  */  be_nested_str_weak(tolower),
  /* K18  */  be_nested_str_weak(transfer_X2Dencoding),
  /* K19  */  be_nested_str_weak(chunked),
  /* K20  */  be_nested_str_weak(find),
  /* K21  */  be_nested_str_weak(_X40),
  /* K22  */  be_nested_str_weak(auth),
  /* K23  */  be_nested_str_weak(fromstring),
  /* K24  */  be_nested_str_weak(tob64),
  /* K25  */  be_nested_str_weak(init),
  /* K26  */  be_nested_str_weak(compile_re),
  /* K27  */  be_nested_str_weak(phase),
  /* K28  */  be_nested_str_weak(parse_http_status_line),
  /* K29  */  be_nested_str_weak(parse_http_headers),
  /* K30  */  be_nested_str_weak(parse_http_payload),
  /* K31  */  be_nested_str_weak(http_status),
  /* K32  */  be_nested_str_weak(event_http_failed),
  /* K33  */  be_nested_str_weak(reset),
  /* K34  */  be_nested_str_weak(cmd),
  /* K35  */  be_nested_str_weak(begin),
  /* K36  */  be_nested_str_weak(send_http),
  /* K37  */  be_nested_str_weak(_re_http_header),
  /* K38  */  be_nested_str_weak(event_http_header),
  /* K39  */  be_nested_str_weak(_re_http_body),
  /* K40  */  be_nested_str_weak(event_http_headers_end),
  /* K41  */  be_nested_str_weak(receive),
  /* K42  */  be_nested_str_weak(event_http_timeout),
  /* K43  */  be_nested_str_weak(re),
  /* K44  */  be_nested_str_weak(contains),
  /* K45  */  be_nested_str_weak(_re_http_status),
  /* K46  */  be_nested_str_weak(compile),
  /* K47  */  be_nested_str_weak(HTTP_STATUS_REGEX),
  /* K48  */  be_nested_str_weak(HTTP_HEADER_REGEX),
  /* K49  */  be_nested_str_weak(HTTP_BODY_REGEX),
  /* K50  */  be_nested_str_weak(HTTP_CHUNK_REGEX),
  /* K51  */  be_nested_str_weak(event_http_finished),
  /* K52  */  be_nested_str_weak(timeout),
  /* K53  */  be_nested_str_weak(set_timeout),
  /* K54  */  be_nested_str_weak(SPINLOCK),
  /* K55  */  be_nested_str_weak(loop),
  /* K56  */  be_nested_str_weak(tasmota),
  /* K57  */  be_nested_str_weak(delay),
  /* K58  */  be_nested_str_weak(addr),
  /* K59  */  be_nested_str_weak(_X3A),
  /* K60  */  be_nested_str_weak(_X5B),
  /* K61  */  be_nested_str_weak(_X5D),
  /* K62  */  be_nested_str_weak(HTTP_GET),
  /* K63  */  be_nested_str_weak(port),
  /* K64  */  be_nested_str_weak(HTTP_GET_AUTH),
  /* K65  */  be_nested_str_weak(write),
  /* K66  */  be_nested_str_weak(status_code),
  /* K67  */  be_nested_str_weak(tcp_connected),
  /* K68  */  be_nested_str_weak(millis),
  /* K69  */  be_nested_str_weak(time_start),
  /* K70  */  be_nested_str_weak(tcp),
  /* K71  */  be_nested_str_weak(available),
  /* K72  */  be_nested_str_weak(read),
  /* K73  */  be_nested_str_weak(parse_http_response),
};


extern const bclass be_class_Matter_HTTP_async;

/********************************************************************
** Solidified function: parse_http_payload
********************************************************************/
be_local_closure(class_Matter_HTTP_async_parse_http_payload,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
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
** Solidified function: event_http_finished
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_http_finished,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_http_finished),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_header
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_http_header,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_http_header),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA40E2000,  //  0000  IMPORT	R3	K16
      0x8C100711,  //  0001  GETMET	R4	R3	K17
      0x5C180200,  //  0002  MOVE	R6	R1
      0x7C100400,  //  0003  CALL	R4	2
      0x5C040800,  //  0004  MOVE	R1	R4
      0x8C100711,  //  0005  GETMET	R4	R3	K17
      0x5C180400,  //  0006  MOVE	R6	R2
      0x7C100400,  //  0007  CALL	R4	2
      0x5C080800,  //  0008  MOVE	R2	R4
      0x1C100312,  //  0009  EQ	R4	R1	K18
      0x78120006,  //  000A  JMPF	R4	#0012
      0x8C100711,  //  000B  GETMET	R4	R3	K17
      0x5C180400,  //  000C  MOVE	R6	R2
      0x7C100400,  //  000D  CALL	R4	2
      0x1C100913,  //  000E  EQ	R4	R4	K19
      0x78120001,  //  000F  JMPF	R4	#0012
      0x50100200,  //  0010  LDBOOL	R4	1	0
      0x90020004,  //  0011  SETMBR	R0	K0	R4
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_HTTP_async_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0xA4162000,  //  0000  IMPORT	R5	K16
      0x60180008,  //  0001  GETGBL	R6	G8
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x7C180200,  //  0003  CALL	R6	1
      0x5C040C00,  //  0004  MOVE	R1	R6
      0x8C180B14,  //  0005  GETMET	R6	R5	K20
      0x5C200200,  //  0006  MOVE	R8	R1
      0x58240015,  //  0007  LDCONST	R9	K21
      0x7C180600,  //  0008  CALL	R6	3
      0x281C0D07,  //  0009  GE	R7	R6	K7
      0x781E000C,  //  000A  JMPF	R7	#0018
      0x601C0015,  //  000B  GETGBL	R7	G21
      0x7C1C0000,  //  000C  CALL	R7	0
      0x8C1C0F17,  //  000D  GETMET	R7	R7	K23
      0x04240D09,  //  000E  SUB	R9	R6	K9
      0x40260E09,  //  000F  CONNECT	R9	K7	R9
      0x94240209,  //  0010  GETIDX	R9	R1	R9
      0x7C1C0400,  //  0011  CALL	R7	2
      0x8C1C0F18,  //  0012  GETMET	R7	R7	K24
      0x7C1C0200,  //  0013  CALL	R7	1
      0x90022C07,  //  0014  SETMBR	R0	K22	R7
      0x001C0D09,  //  0015  ADD	R7	R6	K9
      0x401C0F0F,  //  0016  CONNECT	R7	R7	K15
      0x94040207,  //  0017  GETIDX	R1	R1	R7
      0x601C0003,  //  0018  GETGBL	R7	G3
      0x5C200000,  //  0019  MOVE	R8	R0
      0x7C1C0200,  //  001A  CALL	R7	1
      0x8C1C0F19,  //  001B  GETMET	R7	R7	K25
      0x5C240200,  //  001C  MOVE	R9	R1
      0x5C280400,  //  001D  MOVE	R10	R2
      0x5C2C0600,  //  001E  MOVE	R11	R3
      0x5C300800,  //  001F  MOVE	R12	R4
      0x7C1C0A00,  //  0020  CALL	R7	5
      0x8C1C011A,  //  0021  GETMET	R7	R0	K26
      0x7C1C0200,  //  0022  CALL	R7	1
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_failed
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_http_failed,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_http_failed),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_response
********************************************************************/
be_local_closure(class_Matter_HTTP_async_parse_http_response,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(parse_http_response),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8804011B,  //  0000  GETMBR	R1	R0	K27
      0x1C040307,  //  0001  EQ	R1	R1	K7
      0x78060002,  //  0002  JMPF	R1	#0006
      0x8C04011C,  //  0003  GETMET	R1	R0	K28
      0x7C040200,  //  0004  CALL	R1	1
      0x7002000A,  //  0005  JMP		#0011
      0x8804011B,  //  0006  GETMBR	R1	R0	K27
      0x1C040309,  //  0007  EQ	R1	R1	K9
      0x78060002,  //  0008  JMPF	R1	#000C
      0x8C04011D,  //  0009  GETMET	R1	R0	K29
      0x7C040200,  //  000A  CALL	R1	1
      0x70020004,  //  000B  JMP		#0011
      0x8804011B,  //  000C  GETMBR	R1	R0	K27
      0x1C04030B,  //  000D  EQ	R1	R1	K11
      0x78060001,  //  000E  JMPF	R1	#0011
      0x8C04011E,  //  000F  GETMET	R1	R0	K30
      0x7C040200,  //  0010  CALL	R1	1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_refused
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_refused,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_refused),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x5405FFFE,  //  0000  LDINT	R1	-1
      0x90023E01,  //  0001  SETMBR	R0	K31	R1
      0x8C040120,  //  0002  GETMET	R1	R0	K32
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(class_Matter_HTTP_async_reset,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040321,  //  0003  GETMET	R1	R1	K33
      0x7C040200,  //  0004  CALL	R1	1
      0x4C040000,  //  0005  LDNIL	R1
      0x90024401,  //  0006  SETMBR	R0	K34	R1
      0x4C040000,  //  0007  LDNIL	R1
      0x90020A01,  //  0008  SETMBR	R0	K5	R1
      0x90020D07,  //  0009  SETMBR	R0	K6	K7
      0x90021D0C,  //  000A  SETMBR	R0	K14	K12
      0x90023707,  //  000B  SETMBR	R0	K27	K7
      0x90023F07,  //  000C  SETMBR	R0	K31	K7
      0x50040000,  //  000D  LDBOOL	R1	0	0
      0x90020001,  //  000E  SETMBR	R0	K0	R1
      0x4C040000,  //  000F  LDNIL	R1
      0x90020201,  //  0010  SETMBR	R0	K1	R1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin
********************************************************************/
be_local_closure(class_Matter_HTTP_async_begin,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(begin),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080523,  //  0003  GETMET	R2	R2	K35
      0x7C080200,  //  0004  CALL	R2	1
      0x90024401,  //  0005  SETMBR	R0	K34	R1
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_established
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_established,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_established),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040124,  //  0000  GETMET	R1	R0	K36
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_headers
********************************************************************/
be_local_closure(class_Matter_HTTP_async_parse_http_headers,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(parse_http_headers),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x50040200,  //  0000  LDBOOL	R1	1	0
      0x7806002B,  //  0001  JMPF	R1	#002E
      0xB8060400,  //  0002  GETNGBL	R1	K2
      0x88040325,  //  0003  GETMBR	R1	R1	K37
      0x8C040304,  //  0004  GETMET	R1	R1	K4
      0x880C0105,  //  0005  GETMBR	R3	R0	K5
      0x88100106,  //  0006  GETMBR	R4	R0	K6
      0x7C040600,  //  0007  CALL	R1	3
      0x78060008,  //  0008  JMPF	R1	#0012
      0x8C080126,  //  0009  GETMET	R2	R0	K38
      0x94100309,  //  000A  GETIDX	R4	R1	K9
      0x9414030B,  //  000B  GETIDX	R5	R1	K11
      0x7C080600,  //  000C  CALL	R2	3
      0x88080106,  //  000D  GETMBR	R2	R0	K6
      0x940C0307,  //  000E  GETIDX	R3	R1	K7
      0x00080403,  //  000F  ADD	R2	R2	R3
      0x90020C02,  //  0010  SETMBR	R0	K6	R2
      0x7002001A,  //  0011  JMP		#002D
      0xB80A0400,  //  0012  GETNGBL	R2	K2
      0x88080527,  //  0013  GETMBR	R2	R2	K39
      0x8C080504,  //  0014  GETMET	R2	R2	K4
      0x88100105,  //  0015  GETMBR	R4	R0	K5
      0x88140106,  //  0016  GETMBR	R5	R0	K6
      0x7C080600,  //  0017  CALL	R2	3
      0x780A0004,  //  0018  JMPF	R2	#001E
      0x8C0C0128,  //  0019  GETMET	R3	R0	K40
      0x7C0C0200,  //  001A  CALL	R3	1
      0x9002370B,  //  001B  SETMBR	R0	K27	K11
      0x8C0C011E,  //  001C  GETMET	R3	R0	K30
      0x7C0C0200,  //  001D  CALL	R3	1
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x88100105,  //  001F  GETMBR	R4	R0	K5
      0x7C0C0200,  //  0020  CALL	R3	1
      0x541203FF,  //  0021  LDINT	R4	1024
      0x240C0604,  //  0022  GT	R3	R3	R4
      0x780E0007,  //  0023  JMPF	R3	#002C
      0x8C0C010D,  //  0024  GETMET	R3	R0	K13
      0x7C0C0200,  //  0025  CALL	R3	1
      0x540DFFFB,  //  0026  LDINT	R3	-4
      0x90021403,  //  0027  SETMBR	R0	K10	R3
      0x540DFFFE,  //  0028  LDINT	R3	-1
      0x90023E03,  //  0029  SETMBR	R0	K31	R3
      0x8C0C0120,  //  002A  GETMET	R3	R0	K32
      0x7C0C0200,  //  002B  CALL	R3	1
      0x80000600,  //  002C  RET	0
      0x7001FFD1,  //  002D  JMP		#0000
      0x80000000,  //  002E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_available
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_available,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_available),
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
** Solidified function: event_timeout
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_timeout,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x5405FFFD,  //  0000  LDINT	R1	-2
      0x90023E01,  //  0001  SETMBR	R0	K31	R1
      0x8C04012A,  //  0002  GETMET	R1	R0	K42
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compile_re
********************************************************************/
be_local_closure(class_Matter_HTTP_async_compile_re,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(compile_re),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0xA4065600,  //  0000  IMPORT	R1	K43
      0xB80A0400,  //  0001  GETNGBL	R2	K2
      0x8C08052C,  //  0002  GETMET	R2	R2	K44
      0x5810002D,  //  0003  LDCONST	R4	K45
      0x7C080400,  //  0004  CALL	R2	2
      0x740A0013,  //  0005  JMPT	R2	#001A
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C0C032E,  //  0007  GETMET	R3	R1	K46
      0x8814012F,  //  0008  GETMBR	R5	R0	K47
      0x7C0C0400,  //  0009  CALL	R3	2
      0x900A5A03,  //  000A  SETMBR	R2	K45	R3
      0xB80A0400,  //  000B  GETNGBL	R2	K2
      0x8C0C032E,  //  000C  GETMET	R3	R1	K46
      0x88140130,  //  000D  GETMBR	R5	R0	K48
      0x7C0C0400,  //  000E  CALL	R3	2
      0x900A4A03,  //  000F  SETMBR	R2	K37	R3
      0xB80A0400,  //  0010  GETNGBL	R2	K2
      0x8C0C032E,  //  0011  GETMET	R3	R1	K46
      0x88140131,  //  0012  GETMBR	R5	R0	K49
      0x7C0C0400,  //  0013  CALL	R3	2
      0x900A4E03,  //  0014  SETMBR	R2	K39	R3
      0xB80A0400,  //  0015  GETNGBL	R2	K2
      0x8C0C032E,  //  0016  GETMET	R3	R1	K46
      0x88140132,  //  0017  GETMBR	R5	R0	K50
      0x7C0C0400,  //  0018  CALL	R3	2
      0x900A0603,  //  0019  SETMBR	R2	K3	R3
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_closed
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_closed,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_closed),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8804011F,  //  0000  GETMBR	R1	R0	K31
      0x1C040307,  //  0001  EQ	R1	R1	K7
      0x78060002,  //  0002  JMPF	R1	#0006
      0x90023F09,  //  0003  SETMBR	R0	K31	K9
      0x8C040133,  //  0004  GETMET	R1	R0	K51
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: begin_sync
********************************************************************/
be_local_closure(class_Matter_HTTP_async_begin_sync,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(begin_sync),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x880C0134,  //  0000  GETMBR	R3	R0	K52
      0x4C100000,  //  0001  LDNIL	R4
      0x20100404,  //  0002  NE	R4	R2	R4
      0x78120002,  //  0003  JMPF	R4	#0007
      0x8C100135,  //  0004  GETMET	R4	R0	K53
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x4C100000,  //  0007  LDNIL	R4
      0x88140136,  //  0008  GETMBR	R5	R0	K54
      0x8C180123,  //  0009  GETMET	R6	R0	K35
      0x5C200200,  //  000A  MOVE	R8	R1
      0x7C180400,  //  000B  CALL	R6	2
      0x781A0009,  //  000C  JMPF	R6	#0017
      0x881C011F,  //  000D  GETMBR	R7	R0	K31
      0x1C1C0F07,  //  000E  EQ	R7	R7	K7
      0x781E0006,  //  000F  JMPF	R7	#0017
      0x8C1C0137,  //  0010  GETMET	R7	R0	K55
      0x7C1C0200,  //  0011  CALL	R7	1
      0xB81E7000,  //  0012  GETNGBL	R7	K56
      0x8C1C0F39,  //  0013  GETMET	R7	R7	K57
      0x5C240A00,  //  0014  MOVE	R9	R5
      0x7C1C0400,  //  0015  CALL	R7	2
      0x7001FFF5,  //  0016  JMP		#000D
      0x8C1C0135,  //  0017  GETMET	R7	R0	K53
      0x5C240600,  //  0018  MOVE	R9	R3
      0x7C1C0400,  //  0019  CALL	R7	2
      0x881C011F,  //  001A  GETMBR	R7	R0	K31
      0x241C0F07,  //  001B  GT	R7	R7	K7
      0x781E0001,  //  001C  JMPF	R7	#001F
      0x881C010E,  //  001D  GETMBR	R7	R0	K14
      0x70020000,  //  001E  JMP		#0020
      0x4C1C0000,  //  001F  LDNIL	R7
      0x80040E00,  //  0020  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_http
********************************************************************/
be_local_closure(class_Matter_HTTP_async_send_http,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(send_http),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0xA4062000,  //  0000  IMPORT	R1	K16
      0x90020B0C,  //  0001  SETMBR	R0	K5	K12
      0x8808013A,  //  0002  GETMBR	R2	R0	K58
      0x8C0C0314,  //  0003  GETMET	R3	R1	K20
      0x5C140400,  //  0004  MOVE	R5	R2
      0x5818003B,  //  0005  LDCONST	R6	K59
      0x7C0C0600,  //  0006  CALL	R3	3
      0x280C0707,  //  0007  GE	R3	R3	K7
      0x780E0002,  //  0008  JMPF	R3	#000C
      0x000E7802,  //  0009  ADD	R3	K60	R2
      0x000C073D,  //  000A  ADD	R3	R3	K61
      0x5C080600,  //  000B  MOVE	R2	R3
      0x4C0C0000,  //  000C  LDNIL	R3
      0x88100116,  //  000D  GETMBR	R4	R0	K22
      0x4C140000,  //  000E  LDNIL	R5
      0x1C100805,  //  000F  EQ	R4	R4	R5
      0x78120007,  //  0010  JMPF	R4	#0019
      0x60100018,  //  0011  GETGBL	R4	G24
      0x8814013E,  //  0012  GETMBR	R5	R0	K62
      0x88180122,  //  0013  GETMBR	R6	R0	K34
      0x5C1C0400,  //  0014  MOVE	R7	R2
      0x8820013F,  //  0015  GETMBR	R8	R0	K63
      0x7C100800,  //  0016  CALL	R4	4
      0x5C0C0800,  //  0017  MOVE	R3	R4
      0x70020007,  //  0018  JMP		#0021
      0x60100018,  //  0019  GETGBL	R4	G24
      0x88140140,  //  001A  GETMBR	R5	R0	K64
      0x88180122,  //  001B  GETMBR	R6	R0	K34
      0x5C1C0400,  //  001C  MOVE	R7	R2
      0x8820013F,  //  001D  GETMBR	R8	R0	K63
      0x88240116,  //  001E  GETMBR	R9	R0	K22
      0x7C100A00,  //  001F  CALL	R4	5
      0x5C0C0800,  //  0020  MOVE	R3	R4
      0x8C100141,  //  0021  GETMET	R4	R0	K65
      0x5C180600,  //  0022  MOVE	R6	R3
      0x7C100400,  //  0023  CALL	R4	2
      0x6014000C,  //  0024  GETGBL	R5	G12
      0x5C180600,  //  0025  MOVE	R6	R3
      0x7C140200,  //  0026  CALL	R5	1
      0x20140805,  //  0027  NE	R5	R4	R5
      0x78160007,  //  0028  JMPF	R5	#0031
      0x8C14010D,  //  0029  GETMET	R5	R0	K13
      0x7C140200,  //  002A  CALL	R5	1
      0x5415FFFB,  //  002B  LDINT	R5	-4
      0x90021405,  //  002C  SETMBR	R0	K10	R5
      0x5415FFFE,  //  002D  LDINT	R5	-1
      0x90023E05,  //  002E  SETMBR	R0	K31	R5
      0x8C140120,  //  002F  GETMET	R5	R0	K32
      0x7C140200,  //  0030  CALL	R5	1
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_headers_end
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_http_headers_end,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_http_headers_end),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040106,  //  0000  GETMBR	R1	R0	K6
      0x24040307,  //  0001  GT	R1	R1	K7
      0x78060005,  //  0002  JMPF	R1	#0009
      0x88040106,  //  0003  GETMBR	R1	R0	K6
      0x4004030F,  //  0004  CONNECT	R1	R1	K15
      0x88080105,  //  0005  GETMBR	R2	R0	K5
      0x94040401,  //  0006  GETIDX	R1	R2	R1
      0x90020A01,  //  0007  SETMBR	R0	K5	R1
      0x90020D07,  //  0008  SETMBR	R0	K6	K7
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_status_code
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_http_status_code,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_http_status_code),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90028401,  //  0000  SETMBR	R0	K66	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_status_line
********************************************************************/
be_local_closure(class_Matter_HTTP_async_parse_http_status_line,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(parse_http_status_line),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0xB8060400,  //  0000  GETNGBL	R1	K2
      0x8804032D,  //  0001  GETMBR	R1	R1	K45
      0x8C040304,  //  0002  GETMET	R1	R1	K4
      0x880C0105,  //  0003  GETMBR	R3	R0	K5
      0x88100106,  //  0004  GETMBR	R4	R0	K6
      0x7C040600,  //  0005  CALL	R1	3
      0x78060009,  //  0006  JMPF	R1	#0011
      0x94080307,  //  0007  GETIDX	R2	R1	K7
      0x90020C02,  //  0008  SETMBR	R0	K6	R2
      0x60080009,  //  0009  GETGBL	R2	G9
      0x940C0309,  //  000A  GETIDX	R3	R1	K9
      0x7C080200,  //  000B  CALL	R2	1
      0x90028402,  //  000C  SETMBR	R0	K66	R2
      0x90023709,  //  000D  SETMBR	R0	K27	K9
      0x8C08011D,  //  000E  GETMET	R2	R0	K29
      0x7C080200,  //  000F  CALL	R2	1
      0x70020009,  //  0010  JMP		#001B
      0x6008000C,  //  0011  GETGBL	R2	G12
      0x880C0105,  //  0012  GETMBR	R3	R0	K5
      0x7C080200,  //  0013  CALL	R2	1
      0x540E0063,  //  0014  LDINT	R3	100
      0x24080403,  //  0015  GT	R2	R2	R3
      0x780A0003,  //  0016  JMPF	R2	#001B
      0x8C08010D,  //  0017  GETMET	R2	R0	K13
      0x7C080200,  //  0018  CALL	R2	1
      0x5409FFFB,  //  0019  LDINT	R2	-4
      0x90021402,  //  001A  SETMBR	R0	K10	R2
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_timeout
********************************************************************/
be_local_closure(class_Matter_HTTP_async_event_http_timeout,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(event_http_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: receive
********************************************************************/
be_local_closure(class_Matter_HTTP_async_receive,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_async,     /* shared constants */
    be_str_weak(receive),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x88040143,  //  0000  GETMBR	R1	R0	K67
      0x50080200,  //  0001  LDBOOL	R2	1	0
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0xB8067000,  //  0005  GETNGBL	R1	K56
      0x8C040344,  //  0006  GETMET	R1	R1	K68
      0x7C040200,  //  0007  CALL	R1	1
      0x88080145,  //  0008  GETMBR	R2	R0	K69
      0x04040202,  //  0009  SUB	R1	R1	R2
      0x88080134,  //  000A  GETMBR	R2	R0	K52
      0x24040202,  //  000B  GT	R1	R1	R2
      0x78060008,  //  000C  JMPF	R1	#0016
      0x5405FFFC,  //  000D  LDINT	R1	-3
      0x90021401,  //  000E  SETMBR	R0	K10	R1
      0x8C04010D,  //  000F  GETMET	R1	R0	K13
      0x7C040200,  //  0010  CALL	R1	1
      0x5405FFFD,  //  0011  LDINT	R1	-2
      0x90023E01,  //  0012  SETMBR	R0	K31	R1
      0x8C04012A,  //  0013  GETMET	R1	R0	K42
      0x7C040200,  //  0014  CALL	R1	1
      0x70020018,  //  0015  JMP		#002F
      0x88040146,  //  0016  GETMBR	R1	R0	K70
      0x8C040347,  //  0017  GETMET	R1	R1	K71
      0x7C040200,  //  0018  CALL	R1	1
      0x58080007,  //  0019  LDCONST	R2	K7
      0x240C0307,  //  001A  GT	R3	R1	K7
      0x780E000E,  //  001B  JMPF	R3	#002B
      0x880C0146,  //  001C  GETMBR	R3	R0	K70
      0x8C0C0748,  //  001D  GETMET	R3	R3	K72
      0x7C0C0200,  //  001E  CALL	R3	1
      0x88100105,  //  001F  GETMBR	R4	R0	K5
      0x00100803,  //  0020  ADD	R4	R4	R3
      0x90020A04,  //  0021  SETMBR	R0	K5	R4
      0x6010000C,  //  0022  GETGBL	R4	G12
      0x5C140600,  //  0023  MOVE	R5	R3
      0x7C100200,  //  0024  CALL	R4	1
      0x00080404,  //  0025  ADD	R2	R2	R4
      0x88100146,  //  0026  GETMBR	R4	R0	K70
      0x8C100947,  //  0027  GETMET	R4	R4	K71
      0x7C100200,  //  0028  CALL	R4	1
      0x5C040800,  //  0029  MOVE	R1	R4
      0x7001FFEE,  //  002A  JMP		#001A
      0x240C0507,  //  002B  GT	R3	R2	K7
      0x780E0001,  //  002C  JMPF	R3	#002F
      0x8C0C0149,  //  002D  GETMET	R3	R0	K73
      0x7C0C0200,  //  002E  CALL	R3	1
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_HTTP_async
********************************************************************/
extern const bclass be_class_Matter_TCP_async;
be_local_class(Matter_HTTP_async,
    10,
    &be_class_Matter_TCP_async,
    be_nested_map(39,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(response, -1), be_const_var(2) },
        { be_const_key_weak(parse_http_payload, -1), be_const_closure(class_Matter_HTTP_async_parse_http_payload_closure) },
        { be_const_key_weak(receive, 10), be_const_closure(class_Matter_HTTP_async_receive_closure) },
        { be_const_key_weak(event_http_finished, 22), be_const_closure(class_Matter_HTTP_async_event_http_finished_closure) },
        { be_const_key_weak(HTTP_BODY_REGEX, -1), be_nested_str_weak(_X0D_X0A) },
        { be_const_key_weak(HTTP_GET_AUTH, -1), be_nested_str_weak(GET_X20_X25s_X20HTTP_X2F1_X2E1_X0D_X0AHost_X20_X25s_X3A_X25s_X0D_X0AAuthorization_X3A_X20Basic_X20_X25s_X0D_X0AConnection_X3A_X20close_X0D_X0A_X0D_X0A) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_HTTP_async_init_closure) },
        { be_const_key_weak(response_offset, 23), be_const_var(3) },
        { be_const_key_weak(parse_http_response, -1), be_const_closure(class_Matter_HTTP_async_parse_http_response_closure) },
        { be_const_key_weak(event_refused, -1), be_const_closure(class_Matter_HTTP_async_event_refused_closure) },
        { be_const_key_weak(event_http_timeout, -1), be_const_closure(class_Matter_HTTP_async_event_http_timeout_closure) },
        { be_const_key_weak(HTTP_STATUS_REGEX, -1), be_nested_str_weak(HTTP_X2F1_X5C_X2E_X5B0_X2D1_X5D_X20_X28_X5Cd_X2B_X29_X20_X2E_X2A_X3F_X0D_X0A) },
        { be_const_key_weak(cmd, -1), be_const_var(1) },
        { be_const_key_weak(event_http_status_code, -1), be_const_closure(class_Matter_HTTP_async_event_http_status_code_closure) },
        { be_const_key_weak(payload, -1), be_const_var(5) },
        { be_const_key_weak(is_chunked, -1), be_const_var(8) },
        { be_const_key_weak(HTTP_GET, -1), be_nested_str_weak(GET_X20_X25s_X20HTTP_X2F1_X2E1_X0D_X0AHost_X20_X25s_X3A_X25s_X0D_X0AConnection_X3A_X20close_X0D_X0A_X0D_X0A) },
        { be_const_key_weak(chunk_size, 16), be_const_var(9) },
        { be_const_key_weak(auth, 35), be_const_var(0) },
        { be_const_key_weak(parse_http_headers, 26), be_const_closure(class_Matter_HTTP_async_parse_http_headers_closure) },
        { be_const_key_weak(event_available, -1), be_const_closure(class_Matter_HTTP_async_event_available_closure) },
        { be_const_key_weak(status_code, 11), be_const_var(4) },
        { be_const_key_weak(phase, 36), be_const_var(7) },
        { be_const_key_weak(event_http_headers_end, -1), be_const_closure(class_Matter_HTTP_async_event_http_headers_end_closure) },
        { be_const_key_weak(reset, 31), be_const_closure(class_Matter_HTTP_async_reset_closure) },
        { be_const_key_weak(begin_sync, -1), be_const_closure(class_Matter_HTTP_async_begin_sync_closure) },
        { be_const_key_weak(send_http, -1), be_const_closure(class_Matter_HTTP_async_send_http_closure) },
        { be_const_key_weak(event_timeout, 28), be_const_closure(class_Matter_HTTP_async_event_timeout_closure) },
        { be_const_key_weak(HTTP_CHUNK_REGEX, -1), be_nested_str_weak(_X0D_X0A_X28_X5BA_X2DFa_X2Df0_X2D9_X5D_X2B_X29_X5B_X20_X09_X5D_X2A_X2E_X2A_X3F_X0D_X0A) },
        { be_const_key_weak(compile_re, 7), be_const_closure(class_Matter_HTTP_async_compile_re_closure) },
        { be_const_key_weak(HTTP_HEADER_REGEX, -1), be_nested_str_weak(_X28_X5BA_X2DZa_X2Dz0_X2D9_X2D_X5D_X2B_X29_X3A_X20_X28_X2E_X2A_X3F_X29_X0D_X0A) },
        { be_const_key_weak(event_closed, 30), be_const_closure(class_Matter_HTTP_async_event_closed_closure) },
        { be_const_key_weak(event_http_header, 14), be_const_closure(class_Matter_HTTP_async_event_http_header_closure) },
        { be_const_key_weak(event_http_failed, 13), be_const_closure(class_Matter_HTTP_async_event_http_failed_closure) },
        { be_const_key_weak(parse_http_status_line, -1), be_const_closure(class_Matter_HTTP_async_parse_http_status_line_closure) },
        { be_const_key_weak(event_established, -1), be_const_closure(class_Matter_HTTP_async_event_established_closure) },
        { be_const_key_weak(begin, 4), be_const_closure(class_Matter_HTTP_async_begin_closure) },
        { be_const_key_weak(http_status, -1), be_const_var(6) },
        { be_const_key_weak(SPINLOCK, 2), be_const_int(5) },
    })),
    be_str_weak(Matter_HTTP_async)
);
/********************************************************************/
/* End of solidification */
