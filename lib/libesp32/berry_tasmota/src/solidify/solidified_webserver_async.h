/* Solidification of webserver_async.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_webserver_async_cnx;

/********************************************************************
** Solidified function: send
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_send,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str(HTTP_X2F1_X2E_X25s_X20_X25s_X20_X25s_X0D_X0A),
    /* K1   */  be_nested_str(resp_version),
    /* K2   */  be_nested_str(code_to_string),
    /* K3   */  be_nested_str(send_header),
    /* K4   */  be_nested_str(Content_X2DType),
    /* K5   */  be_nested_str(text_X2Fhtml),
    /* K6   */  be_nested_str(Accept_X2DRanges),
    /* K7   */  be_nested_str(none),
    /* K8   */  be_nested_str(chunked),
    /* K9   */  be_nested_str(Transfer_X2DEncoding),
    /* K10  */  be_nested_str(cors),
    /* K11  */  be_nested_str(Access_X2DControl_X2DAllow_X2DOrigin),
    /* K12  */  be_nested_str(_X2A),
    /* K13  */  be_nested_str(Access_X2DControl_X2DAllow_X2DMethods),
    /* K14  */  be_nested_str(Access_X2DControl_X2DAllow_X2DHeaders),
    /* K15  */  be_nested_str(Connection),
    /* K16  */  be_nested_str(close),
    /* K17  */  be_nested_str(resp_headers),
    /* K18  */  be_nested_str(_X0D_X0A),
    /* K19  */  be_nested_str(write_raw),
    /* K20  */  be_nested_str(write),
    }),
    &be_const_str_send,
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0x60100018,  //  0000  GETGBL	R4	G24
      0x58140000,  //  0001  LDCONST	R5	K0
      0x88180101,  //  0002  GETMBR	R6	R0	K1
      0x5C1C0200,  //  0003  MOVE	R7	R1
      0x8C200102,  //  0004  GETMET	R8	R0	K2
      0x5C280200,  //  0005  MOVE	R10	R1
      0x7C200400,  //  0006  CALL	R8	2
      0x7C100800,  //  0007  CALL	R4	4
      0x8C140103,  //  0008  GETMET	R5	R0	K3
      0x581C0004,  //  0009  LDCONST	R7	K4
      0x780A0001,  //  000A  JMPF	R2	#000D
      0x5C200400,  //  000B  MOVE	R8	R2
      0x70020000,  //  000C  JMP		#000E
      0x58200005,  //  000D  LDCONST	R8	K5
      0x50240200,  //  000E  LDBOOL	R9	1	0
      0x7C140800,  //  000F  CALL	R5	4
      0x8C140103,  //  0010  GETMET	R5	R0	K3
      0x581C0006,  //  0011  LDCONST	R7	K6
      0x58200007,  //  0012  LDCONST	R8	K7
      0x7C140600,  //  0013  CALL	R5	3
      0x88140108,  //  0014  GETMBR	R5	R0	K8
      0x78160003,  //  0015  JMPF	R5	#001A
      0x8C140103,  //  0016  GETMET	R5	R0	K3
      0x581C0009,  //  0017  LDCONST	R7	K9
      0x58200008,  //  0018  LDCONST	R8	K8
      0x7C140600,  //  0019  CALL	R5	3
      0x8814010A,  //  001A  GETMBR	R5	R0	K10
      0x7816000B,  //  001B  JMPF	R5	#0028
      0x8C140103,  //  001C  GETMET	R5	R0	K3
      0x581C000B,  //  001D  LDCONST	R7	K11
      0x5820000C,  //  001E  LDCONST	R8	K12
      0x7C140600,  //  001F  CALL	R5	3
      0x8C140103,  //  0020  GETMET	R5	R0	K3
      0x581C000D,  //  0021  LDCONST	R7	K13
      0x5820000C,  //  0022  LDCONST	R8	K12
      0x7C140600,  //  0023  CALL	R5	3
      0x8C140103,  //  0024  GETMET	R5	R0	K3
      0x581C000E,  //  0025  LDCONST	R7	K14
      0x5820000C,  //  0026  LDCONST	R8	K12
      0x7C140600,  //  0027  CALL	R5	3
      0x8C140103,  //  0028  GETMET	R5	R0	K3
      0x581C000F,  //  0029  LDCONST	R7	K15
      0x58200010,  //  002A  LDCONST	R8	K16
      0x7C140600,  //  002B  CALL	R5	3
      0x88140111,  //  002C  GETMBR	R5	R0	K17
      0x00100805,  //  002D  ADD	R4	R4	R5
      0x00100912,  //  002E  ADD	R4	R4	K18
      0x4C140000,  //  002F  LDNIL	R5
      0x90022205,  //  0030  SETMBR	R0	K17	R5
      0x8C140113,  //  0031  GETMET	R5	R0	K19
      0x5C1C0800,  //  0032  MOVE	R7	R4
      0x7C140400,  //  0033  CALL	R5	2
      0x780E0002,  //  0034  JMPF	R3	#0038
      0x8C140114,  //  0035  GETMET	R5	R0	K20
      0x5C1C0600,  //  0036  MOVE	R7	R3
      0x7C140400,  //  0037  CALL	R5	2
      0x80000000,  //  0038  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _write
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx__write,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(buf_out),
    /* K2   */  be_nested_str(resize),
    /* K3   */  be_nested_str(setbytes),
    /* K4   */  be_nested_str(_send),
    }),
    &be_const_str__write,
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x6008000C,  //  0000  GETGBL	R2	G12
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C0C0500,  //  0003  EQ	R3	R2	K0
      0x780E0000,  //  0004  JMPF	R3	#0006
      0x80000600,  //  0005  RET	0
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x6010000C,  //  0007  GETGBL	R4	G12
      0x5C140600,  //  0008  MOVE	R5	R3
      0x7C100200,  //  0009  CALL	R4	1
      0x8C140702,  //  000A  GETMET	R5	R3	K2
      0x001C0802,  //  000B  ADD	R7	R4	R2
      0x7C140400,  //  000C  CALL	R5	2
      0x8C140703,  //  000D  GETMET	R5	R3	K3
      0x5C1C0800,  //  000E  MOVE	R7	R4
      0x5C200200,  //  000F  MOVE	R8	R1
      0x7C140600,  //  0010  CALL	R5	3
      0x8C140104,  //  0011  GETMET	R5	R0	K4
      0x7C140200,  //  0012  CALL	R5	1
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: buf_out_empty
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_buf_out_empty,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(buf_out),
    /* K1   */  be_const_int(0),
    }),
    &be_const_str_buf_out_empty,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x6004000C,  //  0000  GETGBL	R1	G12
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040301,  //  0003  EQ	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL, 
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str(loop),
        }),
        &be_const_str__anonymous_,
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80000000,  //  0003  RET	0
        })
      ),
    &be_class_webserver_async_cnx, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str(server),
    /* K1   */  be_nested_str(cnx),
    /* K2   */  be_nested_str(buf_in),
    /* K3   */  be_nested_str(),
    /* K4   */  be_nested_str(buf_in_offset),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str(buf_out),
    /* K7   */  be_nested_str(phase),
    /* K8   */  be_nested_str(payload1),
    /* K9   */  be_nested_str(close_after_send),
    /* K10  */  be_nested_str(resp_headers),
    /* K11  */  be_nested_str(resp_version),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str(chunked),
    /* K14  */  be_nested_str(cors),
    /* K15  */  be_nested_str(fastloop_cb),
    /* K16  */  be_nested_str(tasmota),
    /* K17  */  be_nested_str(add_fast_loop),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020503,  //  0002  SETMBR	R0	K2	K3
      0x90020905,  //  0003  SETMBR	R0	K4	K5
      0x600C0015,  //  0004  GETGBL	R3	G21
      0x7C0C0000,  //  0005  CALL	R3	0
      0x90020C03,  //  0006  SETMBR	R0	K6	R3
      0x90020F05,  //  0007  SETMBR	R0	K7	K5
      0x600C0015,  //  0008  GETGBL	R3	G21
      0x7C0C0000,  //  0009  CALL	R3	0
      0x90021003,  //  000A  SETMBR	R0	K8	R3
      0x500C0000,  //  000B  LDBOOL	R3	0	0
      0x90021203,  //  000C  SETMBR	R0	K9	R3
      0x90021503,  //  000D  SETMBR	R0	K10	K3
      0x9002170C,  //  000E  SETMBR	R0	K11	K12
      0x500C0200,  //  000F  LDBOOL	R3	1	0
      0x90021A03,  //  0010  SETMBR	R0	K13	R3
      0x500C0000,  //  0011  LDBOOL	R3	0	0
      0x90021C03,  //  0012  SETMBR	R0	K14	R3
      0x840C0000,  //  0013  CLOSURE	R3	P0
      0x90021E03,  //  0014  SETMBR	R0	K15	R3
      0xB80E2000,  //  0015  GETNGBL	R3	K16
      0x8C0C0711,  //  0016  GETMET	R3	R3	K17
      0x8814010F,  //  0017  GETMBR	R5	R0	K15
      0x7C0C0400,  //  0018  CALL	R3	2
      0xA0000000,  //  0019  CLOSE	R0
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_parse,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(phase),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(parse_http_req_line),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str(parse_http_headers),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str(parse_http_payload),
    }),
    &be_const_str_parse,
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
** Solidified function: content_stop
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_content_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(write),
    /* K1   */  be_nested_str(),
    /* K2   */  be_nested_str(close_after_send),
    }),
    &be_const_str_content_stop,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x50040200,  //  0003  LDBOOL	R1	1	0
      0x90020401,  //  0004  SETMBR	R0	K2	R1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _send
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx__send,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(cnx),
    /* K1   */  be_nested_str(buf_out),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str(listening),
    /* K4   */  be_nested_str(write),
    /* K5   */  be_nested_str(clear),
    /* K6   */  be_nested_str(setbytes),
    /* K7   */  be_nested_str(resize),
    /* K8   */  be_nested_str(close_after_send),
    /* K9   */  be_nested_str(close),
    }),
    &be_const_str__send,
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C080202,  //  0002  EQ	R2	R1	R2
      0x780A0000,  //  0003  JMPF	R2	#0005
      0x80000400,  //  0004  RET	0
      0x88080101,  //  0005  GETMBR	R2	R0	K1
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x5C100400,  //  0007  MOVE	R4	R2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x240C0702,  //  0009  GT	R3	R3	K2
      0x780E001E,  //  000A  JMPF	R3	#002A
      0x8C0C0303,  //  000B  GETMET	R3	R1	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x780E001A,  //  000D  JMPF	R3	#0029
      0x8C0C0304,  //  000E  GETMET	R3	R1	K4
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x24100702,  //  0011  GT	R4	R3	K2
      0x78120015,  //  0012  JMPF	R4	#0029
      0x6010000C,  //  0013  GETGBL	R4	G12
      0x5C140400,  //  0014  MOVE	R5	R2
      0x7C100200,  //  0015  CALL	R4	1
      0x28100604,  //  0016  GE	R4	R3	R4
      0x78120003,  //  0017  JMPF	R4	#001C
      0x88100101,  //  0018  GETMBR	R4	R0	K1
      0x8C100905,  //  0019  GETMET	R4	R4	K5
      0x7C100200,  //  001A  CALL	R4	1
      0x7002000C,  //  001B  JMP		#0029
      0x88100101,  //  001C  GETMBR	R4	R0	K1
      0x8C100906,  //  001D  GETMET	R4	R4	K6
      0x58180002,  //  001E  LDCONST	R6	K2
      0x5C1C0400,  //  001F  MOVE	R7	R2
      0x5C200600,  //  0020  MOVE	R8	R3
      0x7C100800,  //  0021  CALL	R4	4
      0x88100101,  //  0022  GETMBR	R4	R0	K1
      0x8C100907,  //  0023  GETMET	R4	R4	K7
      0x6018000C,  //  0024  GETGBL	R6	G12
      0x5C1C0400,  //  0025  MOVE	R7	R2
      0x7C180200,  //  0026  CALL	R6	1
      0x04180C03,  //  0027  SUB	R6	R6	R3
      0x7C100400,  //  0028  CALL	R4	2
      0x70020003,  //  0029  JMP		#002E
      0x880C0108,  //  002A  GETMBR	R3	R0	K8
      0x780E0001,  //  002B  JMPF	R3	#002E
      0x8C0C0109,  //  002C  GETMET	R3	R0	K9
      0x7C0C0200,  //  002D  CALL	R3	1
      0x80000000,  //  002E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_payload
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_parse_http_payload,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(server),
    /* K1   */  be_nested_str(dispatch),
    /* K2   */  be_nested_str(req_uri),
    /* K3   */  be_nested_str(req_verb),
    }),
    &be_const_str_parse_http_payload,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x7C040800,  //  0005  CALL	R1	4
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: connected
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_connected,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(cnx),
    /* K1   */  be_nested_str(connected),
    }),
    &be_const_str_connected,
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060003,  //  0001  JMPF	R1	#0006
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x8C040301,  //  0003  GETMET	R1	R1	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x70020000,  //  0005  JMP		#0007
      0x50040000,  //  0006  LDBOOL	R1	0	0
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: close
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_close,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(cnx),
    /* K1   */  be_nested_str(close),
    }),
    &be_const_str_close,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060002,  //  0003  JMPF	R1	#0007
      0x88040100,  //  0004  GETMBR	R1	R0	K0
      0x8C040301,  //  0005  GETMET	R1	R1	K1
      0x7C040200,  //  0006  CALL	R1	1
      0x4C040000,  //  0007  LDNIL	R1
      0x90020001,  //  0008  SETMBR	R0	K0	R1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_headers
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_parse_http_headers,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(_re_http_srv_header),
    /* K2   */  be_nested_str(match2),
    /* K3   */  be_nested_str(buf_in),
    /* K4   */  be_nested_str(buf_in_offset),
    /* K5   */  be_nested_str(event_http_header),
    /* K6   */  be_const_int(1),
    /* K7   */  be_const_int(2),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str(_re_http_srv_body),
    /* K10  */  be_const_int(2147483647),
    /* K11  */  be_nested_str(event_http_headers_end),
    /* K12  */  be_nested_str(phase),
    /* K13  */  be_nested_str(parse_http_payload),
    /* K14  */  be_nested_str(log),
    /* K15  */  be_nested_str(WEB_X3A_X20error_X20header_X20is_X20bigger_X20than_X201KB),
    /* K16  */  be_nested_str(close),
    /* K17  */  be_nested_str(),
    }),
    &be_const_str_parse_http_headers,
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x50040200,  //  0000  LDBOOL	R1	1	0
      0x78060032,  //  0001  JMPF	R1	#0035
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
      0x70020021,  //  0011  JMP		#0034
      0xB80A0000,  //  0012  GETNGBL	R2	K0
      0x88080509,  //  0013  GETMBR	R2	R2	K9
      0x8C080502,  //  0014  GETMET	R2	R2	K2
      0x88100103,  //  0015  GETMBR	R4	R0	K3
      0x88140104,  //  0016  GETMBR	R5	R0	K4
      0x7C080600,  //  0017  CALL	R2	3
      0x780A000C,  //  0018  JMPF	R2	#0026
      0x880C0104,  //  0019  GETMBR	R3	R0	K4
      0x94100508,  //  001A  GETIDX	R4	R2	K8
      0x000C0604,  //  001B  ADD	R3	R3	R4
      0x400C070A,  //  001C  CONNECT	R3	R3	K10
      0x88100103,  //  001D  GETMBR	R4	R0	K3
      0x940C0803,  //  001E  GETIDX	R3	R4	R3
      0x90020603,  //  001F  SETMBR	R0	K3	R3
      0x90020908,  //  0020  SETMBR	R0	K4	K8
      0x8C0C010B,  //  0021  GETMET	R3	R0	K11
      0x7C0C0200,  //  0022  CALL	R3	1
      0x90021907,  //  0023  SETMBR	R0	K12	K7
      0x8C0C010D,  //  0024  GETMET	R3	R0	K13
      0x7C0C0200,  //  0025  CALL	R3	1
      0x600C000C,  //  0026  GETGBL	R3	G12
      0x88100103,  //  0027  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0028  CALL	R3	1
      0x541203FF,  //  0029  LDINT	R4	1024
      0x240C0604,  //  002A  GT	R3	R3	R4
      0x780E0006,  //  002B  JMPF	R3	#0033
      0xB80E1C00,  //  002C  GETNGBL	R3	K14
      0x5810000F,  //  002D  LDCONST	R4	K15
      0x54160003,  //  002E  LDINT	R5	4
      0x7C0C0400,  //  002F  CALL	R3	2
      0x8C0C0110,  //  0030  GETMET	R3	R0	K16
      0x7C0C0200,  //  0031  CALL	R3	1
      0x90020711,  //  0032  SETMBR	R0	K3	K17
      0x80000600,  //  0033  RET	0
      0x7001FFCA,  //  0034  JMP		#0000
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_write,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(fromstring),
    /* K2   */  be_nested_str(chunked),
    /* K3   */  be_nested_str(payload1),
    /* K4   */  be_nested_str(clear),
    /* K5   */  be_nested_str(append),
    /* K6   */  be_nested_str(_X25X_X0D_X0A),
    /* K7   */  be_nested_str(_X0D_X0A),
    /* K8   */  be_nested_str(_write),
    }),
    &be_const_str_write,
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x60080004,  //  0000  GETGBL	R2	G4
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080500,  //  0003  EQ	R2	R2	K0
      0x780A0005,  //  0004  JMPF	R2	#000B
      0x60080015,  //  0005  GETGBL	R2	G21
      0x7C080000,  //  0006  CALL	R2	0
      0x8C080501,  //  0007  GETMET	R2	R2	K1
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x5C040400,  //  000A  MOVE	R1	R2
      0x88080102,  //  000B  GETMBR	R2	R0	K2
      0x780A0014,  //  000C  JMPF	R2	#0022
      0x88080103,  //  000D  GETMBR	R2	R0	K3
      0x8C0C0504,  //  000E  GETMET	R3	R2	K4
      0x7C0C0200,  //  000F  CALL	R3	1
      0x8C0C0505,  //  0010  GETMET	R3	R2	K5
      0x60140018,  //  0011  GETGBL	R5	G24
      0x58180006,  //  0012  LDCONST	R6	K6
      0x601C000C,  //  0013  GETGBL	R7	G12
      0x5C200200,  //  0014  MOVE	R8	R1
      0x7C1C0200,  //  0015  CALL	R7	1
      0x7C140400,  //  0016  CALL	R5	2
      0x7C0C0400,  //  0017  CALL	R3	2
      0x8C0C0505,  //  0018  GETMET	R3	R2	K5
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x8C0C0505,  //  001B  GETMET	R3	R2	K5
      0x58140007,  //  001C  LDCONST	R5	K7
      0x7C0C0400,  //  001D  CALL	R3	2
      0x8C0C0108,  //  001E  GETMET	R3	R0	K8
      0x5C140400,  //  001F  MOVE	R5	R2
      0x7C0C0400,  //  0020  CALL	R3	2
      0x70020002,  //  0021  JMP		#0025
      0x8C080108,  //  0022  GETMET	R2	R0	K8
      0x5C100200,  //  0023  MOVE	R4	R1
      0x7C080400,  //  0024  CALL	R2	2
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_raw
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_write_raw,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(string),
    /* K2   */  be_nested_str(fromstring),
    /* K3   */  be_nested_str(_write),
    }),
    &be_const_str_write_raw,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x6008000C,  //  0000  GETGBL	R2	G12
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x1C080500,  //  0003  EQ	R2	R2	K0
      0x780A0000,  //  0004  JMPF	R2	#0006
      0x80000400,  //  0005  RET	0
      0x60080004,  //  0006  GETGBL	R2	G4
      0x5C0C0200,  //  0007  MOVE	R3	R1
      0x7C080200,  //  0008  CALL	R2	1
      0x1C080501,  //  0009  EQ	R2	R2	K1
      0x780A0005,  //  000A  JMPF	R2	#0011
      0x60080015,  //  000B  GETGBL	R2	G21
      0x7C080000,  //  000C  CALL	R2	0
      0x8C080502,  //  000D  GETMET	R2	R2	K2
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x5C040400,  //  0010  MOVE	R1	R2
      0x8C080103,  //  0011  GETMET	R2	R0	K3
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C080400,  //  0013  CALL	R2	2
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_req_line
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_parse_http_req_line,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str(global),
    /* K1   */  be_nested_str(_re_http_srv),
    /* K2   */  be_nested_str(match2),
    /* K3   */  be_nested_str(buf_in),
    /* K4   */  be_nested_str(buf_in_offset),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str(req_verb),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str(req_uri),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str(req_version),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str(phase),
    /* K13  */  be_const_int(2147483647),
    /* K14  */  be_nested_str(tasmota),
    /* K15  */  be_nested_str(loglevel),
    /* K16  */  be_nested_str(log),
    /* K17  */  be_nested_str(WEB_X3A_X20HTTP_X20verb_X3A_X20_X25s_X20URI_X3A_X20_X27_X25s_X27_X20Version_X3A_X25s),
    /* K18  */  be_nested_str(parse_http_headers),
    /* K19  */  be_nested_str(WEB_X3A_X20error_X20invalid_X20request),
    /* K20  */  be_nested_str(close),
    /* K21  */  be_nested_str(),
    }),
    &be_const_str_parse_http_req_line,
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C040302,  //  0002  GETMET	R1	R1	K2
      0x880C0103,  //  0003  GETMBR	R3	R0	K3
      0x88100104,  //  0004  GETMBR	R4	R0	K4
      0x7C040600,  //  0005  CALL	R1	3
      0x7806001C,  //  0006  JMPF	R1	#0024
      0x94080305,  //  0007  GETIDX	R2	R1	K5
      0x940C0307,  //  0008  GETIDX	R3	R1	K7
      0x90020C03,  //  0009  SETMBR	R0	K6	R3
      0x940C0309,  //  000A  GETIDX	R3	R1	K9
      0x90021003,  //  000B  SETMBR	R0	K8	R3
      0x940C030B,  //  000C  GETIDX	R3	R1	K11
      0x90021403,  //  000D  SETMBR	R0	K10	R3
      0x90021907,  //  000E  SETMBR	R0	K12	K7
      0x400C050D,  //  000F  CONNECT	R3	R2	K13
      0x88100103,  //  0010  GETMBR	R4	R0	K3
      0x940C0803,  //  0011  GETIDX	R3	R4	R3
      0x90020603,  //  0012  SETMBR	R0	K3	R3
      0xB80E1C00,  //  0013  GETNGBL	R3	K14
      0x8C0C070F,  //  0014  GETMET	R3	R3	K15
      0x54160003,  //  0015  LDINT	R5	4
      0x7C0C0400,  //  0016  CALL	R3	2
      0x780E0008,  //  0017  JMPF	R3	#0021
      0xB80E2000,  //  0018  GETNGBL	R3	K16
      0x60100018,  //  0019  GETGBL	R4	G24
      0x58140011,  //  001A  LDCONST	R5	K17
      0x88180106,  //  001B  GETMBR	R6	R0	K6
      0x881C0108,  //  001C  GETMBR	R7	R0	K8
      0x8820010A,  //  001D  GETMBR	R8	R0	K10
      0x7C100800,  //  001E  CALL	R4	4
      0x54160003,  //  001F  LDINT	R5	4
      0x7C0C0400,  //  0020  CALL	R3	2
      0x8C0C0112,  //  0021  GETMET	R3	R0	K18
      0x7C0C0200,  //  0022  CALL	R3	1
      0x7002000C,  //  0023  JMP		#0031
      0x6008000C,  //  0024  GETGBL	R2	G12
      0x880C0103,  //  0025  GETMBR	R3	R0	K3
      0x7C080200,  //  0026  CALL	R2	1
      0x540E0063,  //  0027  LDINT	R3	100
      0x24080403,  //  0028  GT	R2	R2	R3
      0x780A0006,  //  0029  JMPF	R2	#0031
      0xB80A2000,  //  002A  GETNGBL	R2	K16
      0x580C0013,  //  002B  LDCONST	R3	K19
      0x54120003,  //  002C  LDINT	R4	4
      0x7C080400,  //  002D  CALL	R2	2
      0x8C080114,  //  002E  GETMET	R2	R0	K20
      0x7C080200,  //  002F  CALL	R2	1
      0x90020715,  //  0030  SETMBR	R0	K3	K21
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: code_to_string
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_code_to_string,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_class(be_class_webserver_async_cnx),
    /* K1   */  be_nested_str(CODE_TO_STRING),
    /* K2   */  be_nested_str(find),
    /* K3   */  be_nested_str(UNKNOWN),
    }),
    &be_const_str_code_to_string,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x88080301,  //  0001  GETMBR	R2	R1	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100000,  //  0003  MOVE	R4	R0
      0x58140003,  //  0004  LDCONST	R5	K3
      0x7C080600,  //  0005  CALL	R2	3
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_header
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_send_header,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(resp_headers),
    /* K1   */  be_nested_str(_X25s_X3A_X20_X25s_X0D_X0A_X25s),
    /* K2   */  be_nested_str(_X25s_X25s_X3A_X20_X25s_X0D_X0A),
    }),
    &be_const_str_send_header,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x780E0007,  //  0000  JMPF	R3	#0009
      0x60100018,  //  0001  GETGBL	R4	G24
      0x58140001,  //  0002  LDCONST	R5	K1
      0x5C180200,  //  0003  MOVE	R6	R1
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x88200100,  //  0005  GETMBR	R8	R0	K0
      0x7C100800,  //  0006  CALL	R4	4
      0x90020004,  //  0007  SETMBR	R0	K0	R4
      0x70020006,  //  0008  JMP		#0010
      0x60100018,  //  0009  GETGBL	R4	G24
      0x58140002,  //  000A  LDCONST	R5	K2
      0x88180100,  //  000B  GETMBR	R6	R0	K0
      0x5C1C0200,  //  000C  MOVE	R7	R1
      0x5C200400,  //  000D  MOVE	R8	R2
      0x7C100800,  //  000E  CALL	R4	4
      0x90020004,  //  000F  SETMBR	R0	K0	R4
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_chunked
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_set_chunked,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(chunked),
    }),
    &be_const_str_set_chunked,
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
** Solidified function: event_http_headers_end
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_event_http_headers_end,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    0,                          /* has constants */
    NULL,                       /* no const */
    &be_const_str_event_http_headers_end,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_cors
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_set_cors,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(cors),
    }),
    &be_const_str_set_cors,
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
** Solidified function: event_http_header
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_event_http_header,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(Host),
    /* K1   */  be_nested_str(header_host),
    }),
    &be_const_str_event_http_header,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0000,  //  0001  JMPF	R3	#0003
      0x90020202,  //  0002  SETMBR	R0	K1	R2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: loop
********************************************************************/
extern const bclass be_class_webserver_async_cnx;
be_local_closure(class_webserver_async_cnx_loop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_cnx, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(cnx),
    /* K1   */  be_nested_str(tasmota),
    /* K2   */  be_nested_str(remove_fast_loop),
    /* K3   */  be_nested_str(fastloop_cb),
    /* K4   */  be_nested_str(_send),
    /* K5   */  be_nested_str(available),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str(read),
    /* K8   */  be_nested_str(buf_in),
    /* K9   */  be_nested_str(parse),
    }),
    &be_const_str_loop,
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060006,  //  0003  JMPF	R1	#000B
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0x7C040400,  //  0007  CALL	R1	2
      0x4C040000,  //  0008  LDNIL	R1
      0x90020601,  //  0009  SETMBR	R0	K3	R1
      0x80000200,  //  000A  RET	0
      0x8C040104,  //  000B  GETMET	R1	R0	K4
      0x7C040200,  //  000C  CALL	R1	1
      0x88040100,  //  000D  GETMBR	R1	R0	K0
      0x4C080000,  //  000E  LDNIL	R2
      0x1C080202,  //  000F  EQ	R2	R1	R2
      0x780A0000,  //  0010  JMPF	R2	#0012
      0x80000400,  //  0011  RET	0
      0x8C080305,  //  0012  GETMET	R2	R1	K5
      0x7C080200,  //  0013  CALL	R2	1
      0x24080506,  //  0014  GT	R2	R2	K6
      0x780A0008,  //  0015  JMPF	R2	#001F
      0x8C080307,  //  0016  GETMET	R2	R1	K7
      0x7C080200,  //  0017  CALL	R2	1
      0x880C0108,  //  0018  GETMBR	R3	R0	K8
      0x740E0001,  //  0019  JMPT	R3	#001C
      0x90021002,  //  001A  SETMBR	R0	K8	R2
      0x70020002,  //  001B  JMP		#001F
      0x880C0108,  //  001C  GETMBR	R3	R0	K8
      0x000C0602,  //  001D  ADD	R3	R3	R2
      0x90021003,  //  001E  SETMBR	R0	K8	R3
      0x88080108,  //  001F  GETMBR	R2	R0	K8
      0x780A0001,  //  0020  JMPF	R2	#0023
      0x8C080109,  //  0021  GETMET	R2	R0	K9
      0x7C080200,  //  0022  CALL	R2	1
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: webserver_async_cnx
********************************************************************/
be_local_class(webserver_async_cnx,
    17,
    NULL,
    be_nested_map(39,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(server, 9), be_const_var(0) },
        { be_const_key(send, 37), be_const_closure(class_webserver_async_cnx_send_closure) },
        { be_const_key(fastloop_cb, -1), be_const_var(3) },
        { be_const_key(cors, 5), be_const_var(15) },
        { be_const_key(_write, -1), be_const_closure(class_webserver_async_cnx__write_closure) },
        { be_const_key(loop, 36), be_const_closure(class_webserver_async_cnx_loop_closure) },
        { be_const_key(init, 38), be_const_closure(class_webserver_async_cnx_init_closure) },
        { be_const_key(parse, -1), be_const_closure(class_webserver_async_cnx_parse_closure) },
        { be_const_key(buf_out, -1), be_const_var(6) },
        { be_const_key(cnx, 31), be_const_var(1) },
        { be_const_key(content_stop, -1), be_const_closure(class_webserver_async_cnx_content_stop_closure) },
        { be_const_key(_send, -1), be_const_closure(class_webserver_async_cnx__send_closure) },
        { be_const_key(payload1, 16), be_const_var(16) },
        { be_const_key(header_host, -1), be_const_var(11) },
        { be_const_key(req_uri, -1), be_const_var(9) },
        { be_const_key(buf_in, -1), be_const_var(4) },
        { be_const_key(set_chunked, -1), be_const_closure(class_webserver_async_cnx_set_chunked_closure) },
        { be_const_key(resp_version, -1), be_const_var(13) },
        { be_const_key(close, -1), be_const_closure(class_webserver_async_cnx_close_closure) },
        { be_const_key(parse_http_headers, -1), be_const_closure(class_webserver_async_cnx_parse_http_headers_closure) },
        { be_const_key(buf_out_empty, 34), be_const_closure(class_webserver_async_cnx_buf_out_empty_closure) },
        { be_const_key(write_raw, -1), be_const_closure(class_webserver_async_cnx_write_raw_closure) },
        { be_const_key(resp_headers, -1), be_const_var(12) },
        { be_const_key(parse_http_req_line, 15), be_const_closure(class_webserver_async_cnx_parse_http_req_line_closure) },
        { be_const_key(code_to_string, -1), be_const_static_closure(class_webserver_async_cnx_code_to_string_closure) },
        { be_const_key(send_header, -1), be_const_closure(class_webserver_async_cnx_send_header_closure) },
        { be_const_key(buf_in_offset, 12), be_const_var(5) },
        { be_const_key(req_verb, -1), be_const_var(8) },
        { be_const_key(connected, 17), be_const_closure(class_webserver_async_cnx_connected_closure) },
        { be_const_key(event_http_headers_end, -1), be_const_closure(class_webserver_async_cnx_event_http_headers_end_closure) },
        { be_const_key(CODE_TO_STRING, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(200, 3), be_nested_str(OK) },
        { be_const_key_int(301, 2), be_nested_str(Moved_X20Permanently) },
        { be_const_key_int(401, -1), be_nested_str(Unauthorized) },
        { be_const_key_int(500, -1), be_nested_str(Internal_X20Server_X20Error) },
        { be_const_key_int(404, -1), be_nested_str(Not_X20Found) },
    }))    ) } )) },
        { be_const_key(set_cors, 13), be_const_closure(class_webserver_async_cnx_set_cors_closure) },
        { be_const_key(event_http_header, -1), be_const_closure(class_webserver_async_cnx_event_http_header_closure) },
        { be_const_key(chunked, -1), be_const_var(14) },
        { be_const_key(write, -1), be_const_closure(class_webserver_async_cnx_write_closure) },
        { be_const_key(close_after_send, -1), be_const_var(2) },
        { be_const_key(phase, -1), be_const_var(7) },
        { be_const_key(parse_http_payload, -1), be_const_closure(class_webserver_async_cnx_parse_http_payload_closure) },
        { be_const_key(req_version, -1), be_const_var(10) },
    })),
    (bstring*) &be_const_str_webserver_async_cnx
);

extern const bclass be_class_webserver_async_dispatcher;

/********************************************************************
** Solidified function: dispatch
********************************************************************/
extern const bclass be_class_webserver_async_dispatcher;
be_local_closure(class_webserver_async_dispatcher_dispatch,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_dispatcher, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(find),
    /* K2   */  be_nested_str(uri_prefix),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str(verb),
    /* K5   */  be_nested_str(cb_obj),
    /* K6   */  be_nested_str(cb_mth),
    }),
    &be_const_str_dispatch,
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x8C140901,  //  0001  GETMET	R5	R4	K1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x88200102,  //  0003  GETMBR	R8	R0	K2
      0x7C140600,  //  0004  CALL	R5	3
      0x1C140B03,  //  0005  EQ	R5	R5	K3
      0x78160019,  //  0006  JMPF	R5	#0021
      0x50140000,  //  0007  LDBOOL	R5	0	0
      0x88180104,  //  0008  GETMBR	R6	R0	K4
      0x4C1C0000,  //  0009  LDNIL	R7
      0x1C180C07,  //  000A  EQ	R6	R6	R7
      0x741A0002,  //  000B  JMPT	R6	#000F
      0x88180104,  //  000C  GETMBR	R6	R0	K4
      0x1C180C03,  //  000D  EQ	R6	R6	R3
      0x781A0011,  //  000E  JMPF	R6	#0021
      0x88180105,  //  000F  GETMBR	R6	R0	K5
      0x4C1C0000,  //  0010  LDNIL	R7
      0x201C0C07,  //  0011  NE	R7	R6	R7
      0x781E0006,  //  0012  JMPF	R7	#001A
      0x8C1C0106,  //  0013  GETMET	R7	R0	K6
      0x88240105,  //  0014  GETMBR	R9	R0	K5
      0x5C280200,  //  0015  MOVE	R10	R1
      0x5C2C0400,  //  0016  MOVE	R11	R2
      0x5C300600,  //  0017  MOVE	R12	R3
      0x7C1C0A00,  //  0018  CALL	R7	5
      0x70020004,  //  0019  JMP		#001F
      0x8C1C0106,  //  001A  GETMET	R7	R0	K6
      0x5C240200,  //  001B  MOVE	R9	R1
      0x5C280400,  //  001C  MOVE	R10	R2
      0x5C2C0600,  //  001D  MOVE	R11	R3
      0x7C1C0800,  //  001E  CALL	R7	4
      0x501C0200,  //  001F  LDBOOL	R7	1	0
      0x80040E00,  //  0020  RET	1	R7
      0x50140000,  //  0021  LDBOOL	R5	0	0
      0x80040A00,  //  0022  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_webserver_async_dispatcher;
be_local_closure(class_webserver_async_dispatcher_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async_dispatcher, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(uri_prefix),
    /* K1   */  be_nested_str(cb_obj),
    /* K2   */  be_nested_str(cb_mth),
    /* K3   */  be_nested_str(verb),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x90020604,  //  0003  SETMBR	R0	K3	R4
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: webserver_async_dispatcher
********************************************************************/
be_local_class(webserver_async_dispatcher,
    4,
    NULL,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(verb, 4), be_const_var(1) },
        { be_const_key(init, -1), be_const_closure(class_webserver_async_dispatcher_init_closure) },
        { be_const_key(uri_prefix, -1), be_const_var(0) },
        { be_const_key(dispatch, 1), be_const_closure(class_webserver_async_dispatcher_dispatch_closure) },
        { be_const_key(cb_obj, 5), be_const_var(2) },
        { be_const_key(cb_mth, -1), be_const_var(3) },
    })),
    (bstring*) &be_const_str_webserver_async_dispatcher
);

extern const bclass be_class_webserver_async;

/********************************************************************
** Solidified function: bytes_format_hex
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_bytes_format_hex,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_webserver_async),
    /* K1   */  be_nested_str(clear),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str(resize),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str(reverse),
    }),
    &be_const_str_bytes_format_hex,
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x8C100101,  //  0001  GETMET	R4	R0	K1
      0x7C100200,  //  0002  CALL	R4	1
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100204,  //  0004  EQ	R4	R1	R4
      0x78120001,  //  0005  JMPF	R4	#0008
      0x40100002,  //  0006  CONNECT	R4	R0	R2
      0x80000800,  //  0007  RET	0
      0x14100302,  //  0008  LT	R4	R1	K2
      0x78120000,  //  0009  JMPF	R4	#000B
      0x44040200,  //  000A  NEG	R1	R1
      0x14100302,  //  000B  LT	R4	R1	K2
      0x78120000,  //  000C  JMPF	R4	#000E
      0x80000800,  //  000D  RET	0
      0x1C100302,  //  000E  EQ	R4	R1	K2
      0x78120005,  //  000F  JMPF	R4	#0016
      0x8C100103,  //  0010  GETMET	R4	R0	K3
      0x58180004,  //  0011  LDCONST	R6	K4
      0x7C100400,  //  0012  CALL	R4	2
      0x5412002F,  //  0013  LDINT	R4	48
      0x98020404,  //  0014  SETIDX	R0	K2	R4
      0x80000800,  //  0015  RET	0
      0x8C100103,  //  0016  GETMET	R4	R0	K3
      0x541A0007,  //  0017  LDINT	R6	8
      0x7C100400,  //  0018  CALL	R4	2
      0x58100002,  //  0019  LDCONST	R4	K2
      0x24140302,  //  001A  GT	R5	R1	K2
      0x7816000F,  //  001B  JMPF	R5	#002C
      0x5416000E,  //  001C  LDINT	R5	15
      0x2C140205,  //  001D  AND	R5	R1	R5
      0x541A0009,  //  001E  LDINT	R6	10
      0x14180A06,  //  001F  LT	R6	R5	R6
      0x781A0003,  //  0020  JMPF	R6	#0025
      0x541A002F,  //  0021  LDINT	R6	48
      0x00180C05,  //  0022  ADD	R6	R6	R5
      0x98000806,  //  0023  SETIDX	R0	R4	R6
      0x70020002,  //  0024  JMP		#0028
      0x541A0036,  //  0025  LDINT	R6	55
      0x00180C05,  //  0026  ADD	R6	R6	R5
      0x98000806,  //  0027  SETIDX	R0	R4	R6
      0x00100904,  //  0028  ADD	R4	R4	K4
      0x541A0003,  //  0029  LDINT	R6	4
      0x3C040206,  //  002A  SHR	R1	R1	R6
      0x7001FFED,  //  002B  JMP		#001A
      0x8C140103,  //  002C  GETMET	R5	R0	K3
      0x5C1C0800,  //  002D  MOVE	R7	R4
      0x7C140400,  //  002E  CALL	R5	2
      0x8C140105,  //  002F  GETMET	R5	R0	K5
      0x7C140200,  //  0030  CALL	R5	1
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bytes_format_int
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_bytes_format_int,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_webserver_async),
    /* K1   */  be_nested_str(clear),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str(resize),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str(reverse),
    }),
    &be_const_str_bytes_format_int,
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x8C100101,  //  0001  GETMET	R4	R0	K1
      0x7C100200,  //  0002  CALL	R4	1
      0x4C100000,  //  0003  LDNIL	R4
      0x1C100204,  //  0004  EQ	R4	R1	R4
      0x78120001,  //  0005  JMPF	R4	#0008
      0x40100002,  //  0006  CONNECT	R4	R0	R2
      0x80000800,  //  0007  RET	0
      0x50100000,  //  0008  LDBOOL	R4	0	0
      0x14140302,  //  0009  LT	R5	R1	K2
      0x78160001,  //  000A  JMPF	R5	#000D
      0x44040200,  //  000B  NEG	R1	R1
      0x50100200,  //  000C  LDBOOL	R4	1	0
      0x14140302,  //  000D  LT	R5	R1	K2
      0x78160000,  //  000E  JMPF	R5	#0010
      0x80000A00,  //  000F  RET	0
      0x1C140302,  //  0010  EQ	R5	R1	K2
      0x78160005,  //  0011  JMPF	R5	#0018
      0x8C140103,  //  0012  GETMET	R5	R0	K3
      0x581C0004,  //  0013  LDCONST	R7	K4
      0x7C140400,  //  0014  CALL	R5	2
      0x5416002F,  //  0015  LDINT	R5	48
      0x98020405,  //  0016  SETIDX	R0	K2	R5
      0x80000A00,  //  0017  RET	0
      0x8C140103,  //  0018  GETMET	R5	R0	K3
      0x541E000A,  //  0019  LDINT	R7	11
      0x7C140400,  //  001A  CALL	R5	2
      0x58140002,  //  001B  LDCONST	R5	K2
      0x24180302,  //  001C  GT	R6	R1	K2
      0x781A0008,  //  001D  JMPF	R6	#0027
      0x541A0009,  //  001E  LDINT	R6	10
      0x10180206,  //  001F  MOD	R6	R1	R6
      0x541E002F,  //  0020  LDINT	R7	48
      0x001C0E06,  //  0021  ADD	R7	R7	R6
      0x98000A07,  //  0022  SETIDX	R0	R5	R7
      0x00140B04,  //  0023  ADD	R5	R5	K4
      0x541E0009,  //  0024  LDINT	R7	10
      0x0C040207,  //  0025  DIV	R1	R1	R7
      0x7001FFF4,  //  0026  JMP		#001C
      0x78120002,  //  0027  JMPF	R4	#002B
      0x541A002C,  //  0028  LDINT	R6	45
      0x98000A06,  //  0029  SETIDX	R0	R5	R6
      0x00140B04,  //  002A  ADD	R5	R5	K4
      0x8C180103,  //  002B  GETMET	R6	R0	K3
      0x5C200A00,  //  002C  MOVE	R8	R5
      0x7C180400,  //  002D  CALL	R6	2
      0x8C180105,  //  002E  GETMET	R6	R0	K5
      0x7C180200,  //  002F  CALL	R6	1
      0x80000000,  //  0030  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dispatch
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_dispatch,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(dispatchers),
    /* K2   */  be_nested_str(dispatch),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str(send),
    /* K5   */  be_nested_str(text_X2Fplain),
    /* K6   */  be_nested_str(write),
    /* K7   */  be_nested_str(Unsupported),
    /* K8   */  be_nested_str(content_stop),
    }),
    &be_const_str_dispatch,
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x58100000,  //  0000  LDCONST	R4	K0
      0x6014000C,  //  0001  GETGBL	R5	G12
      0x88180101,  //  0002  GETMBR	R6	R0	K1
      0x7C140200,  //  0003  CALL	R5	1
      0x14140805,  //  0004  LT	R5	R4	R5
      0x7816000A,  //  0005  JMPF	R5	#0011
      0x88140101,  //  0006  GETMBR	R5	R0	K1
      0x94140A04,  //  0007  GETIDX	R5	R5	R4
      0x8C140B02,  //  0008  GETMET	R5	R5	K2
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x5C200400,  //  000A  MOVE	R8	R2
      0x5C240600,  //  000B  MOVE	R9	R3
      0x7C140800,  //  000C  CALL	R5	4
      0x78160000,  //  000D  JMPF	R5	#000F
      0x80000A00,  //  000E  RET	0
      0x00100903,  //  000F  ADD	R4	R4	K3
      0x7001FFEF,  //  0010  JMP		#0001
      0x8C140304,  //  0011  GETMET	R5	R1	K4
      0x541E01F3,  //  0012  LDINT	R7	500
      0x58200005,  //  0013  LDCONST	R8	K5
      0x7C140600,  //  0014  CALL	R5	3
      0x8C140306,  //  0015  GETMET	R5	R1	K6
      0x581C0007,  //  0016  LDCONST	R7	K7
      0x7C140400,  //  0017  CALL	R5	2
      0x8C140308,  //  0018  GETMET	R5	R1	K8
      0x7C140200,  //  0019  CALL	R5	1
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clean_connections
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_clean_connections,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str(connections),
    /* K2   */  be_nested_str(connected),
    /* K3   */  be_nested_str(close),
    /* K4   */  be_nested_str(remove),
    /* K5   */  be_const_int(1),
    }),
    &be_const_str_clean_connections,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A000D,  //  0005  JMPF	R2	#0014
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x740E0006,  //  000A  JMPT	R3	#0012
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x880C0101,  //  000D  GETMBR	R3	R0	K1
      0x8C0C0704,  //  000E  GETMET	R3	R3	K4
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x70020000,  //  0011  JMP		#0013
      0x00040305,  //  0012  ADD	R1	R1	K5
      0x7001FFEC,  //  0013  JMP		#0001
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL, 
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str(loop),
        }),
        &be_const_str__anonymous_,
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80000000,  //  0003  RET	0
        })
      ),
    &be_class_webserver_async, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str(TIMEOUT),
    /* K1   */  be_nested_str(connections),
    /* K2   */  be_nested_str(dispatchers),
    /* K3   */  be_nested_str(server),
    /* K4   */  be_nested_str(tcpserver),
    /* K5   */  be_nested_str(chunked),
    /* K6   */  be_nested_str(cors),
    /* K7   */  be_nested_str(payload1),
    /* K8   */  be_nested_str(payload2),
    /* K9   */  be_nested_str(compile_re),
    /* K10  */  be_nested_str(tasmota),
    /* K11  */  be_nested_str(add_driver),
    /* K12  */  be_nested_str(fastloop_cb),
    /* K13  */  be_nested_str(add_fast_loop),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x600C0012,  //  0004  GETGBL	R3	G18
      0x7C0C0000,  //  0005  CALL	R3	0
      0x90020203,  //  0006  SETMBR	R0	K1	R3
      0x600C0012,  //  0007  GETGBL	R3	G18
      0x7C0C0000,  //  0008  CALL	R3	0
      0x90020403,  //  0009  SETMBR	R0	K2	R3
      0xB80E0800,  //  000A  GETNGBL	R3	K4
      0x5C100200,  //  000B  MOVE	R4	R1
      0x7C0C0200,  //  000C  CALL	R3	1
      0x90020603,  //  000D  SETMBR	R0	K3	R3
      0x500C0200,  //  000E  LDBOOL	R3	1	0
      0x90020A03,  //  000F  SETMBR	R0	K5	R3
      0x500C0000,  //  0010  LDBOOL	R3	0	0
      0x90020C03,  //  0011  SETMBR	R0	K6	R3
      0x600C0015,  //  0012  GETGBL	R3	G21
      0x54120063,  //  0013  LDINT	R4	100
      0x7C0C0200,  //  0014  CALL	R3	1
      0x90020E03,  //  0015  SETMBR	R0	K7	R3
      0x600C0015,  //  0016  GETGBL	R3	G21
      0x54120063,  //  0017  LDINT	R4	100
      0x7C0C0200,  //  0018  CALL	R3	1
      0x90021003,  //  0019  SETMBR	R0	K8	R3
      0x8C0C0109,  //  001A  GETMET	R3	R0	K9
      0x7C0C0200,  //  001B  CALL	R3	1
      0xB80E1400,  //  001C  GETNGBL	R3	K10
      0x8C0C070B,  //  001D  GETMET	R3	R3	K11
      0x5C140000,  //  001E  MOVE	R5	R0
      0x7C0C0400,  //  001F  CALL	R3	2
      0x840C0000,  //  0020  CLOSURE	R3	P0
      0x90021803,  //  0021  SETMBR	R0	K12	R3
      0xB80E1400,  //  0022  GETNGBL	R3	K10
      0x8C0C070D,  //  0023  GETMET	R3	R3	K13
      0x8814010C,  //  0024  GETMBR	R5	R0	K12
      0x7C0C0400,  //  0025  CALL	R3	2
      0xA0000000,  //  0026  CLOSE	R0
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_cors
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_set_cors,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(cors),
    }),
    &be_const_str_set_cors,
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
** Solidified function: set_chunked
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_set_chunked,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(chunked),
    }),
    &be_const_str_set_chunked,
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
** Solidified function: loop
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_loop,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str(clean_connections),
    /* K1   */  be_nested_str(server),
    /* K2   */  be_nested_str(hasclient),
    /* K3   */  be_nested_str(webserver_async_cnx),
    /* K4   */  be_nested_str(acceptasync),
    /* K5   */  be_nested_str(set_chunked),
    /* K6   */  be_nested_str(chunked),
    /* K7   */  be_nested_str(set_cors),
    /* K8   */  be_nested_str(cors),
    /* K9   */  be_nested_str(connections),
    /* K10  */  be_nested_str(push),
    }),
    &be_const_str_loop,
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x8C040302,  //  0003  GETMET	R1	R1	K2
      0x7C040200,  //  0004  CALL	R1	1
      0x78060010,  //  0005  JMPF	R1	#0017
      0x8C040103,  //  0006  GETMET	R1	R0	K3
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x88100101,  //  0008  GETMBR	R4	R0	K1
      0x8C100904,  //  0009  GETMET	R4	R4	K4
      0x7C100200,  //  000A  CALL	R4	1
      0x7C040600,  //  000B  CALL	R1	3
      0x8C080305,  //  000C  GETMET	R2	R1	K5
      0x88100106,  //  000D  GETMBR	R4	R0	K6
      0x7C080400,  //  000E  CALL	R2	2
      0x8C080307,  //  000F  GETMET	R2	R1	K7
      0x88100108,  //  0010  GETMBR	R4	R0	K8
      0x7C080400,  //  0011  CALL	R2	2
      0x88080109,  //  0012  GETMBR	R2	R0	K9
      0x8C08050A,  //  0013  GETMET	R2	R2	K10
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x7001FFEA,  //  0016  JMP		#0002
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: on
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_on,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(webserver_async_dispatcher),
    /* K1   */  be_nested_str(dispatchers),
    /* K2   */  be_nested_str(push),
    }),
    &be_const_str_on,
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8C140100,  //  0000  GETMET	R5	R0	K0
      0x5C1C0200,  //  0001  MOVE	R7	R1
      0x5C200400,  //  0002  MOVE	R8	R2
      0x5C240600,  //  0003  MOVE	R9	R3
      0x5C280800,  //  0004  MOVE	R10	R4
      0x7C140A00,  //  0005  CALL	R5	5
      0x88180101,  //  0006  GETMBR	R6	R0	K1
      0x8C180D02,  //  0007  GETMET	R6	R6	K2
      0x5C200A00,  //  0008  MOVE	R8	R5
      0x7C180400,  //  0009  CALL	R6	2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: close
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_close,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(remove_driver),
    /* K2   */  be_nested_str(remove_fast_loop),
    /* K3   */  be_nested_str(fastloop_cb),
    /* K4   */  be_nested_str(server),
    /* K5   */  be_nested_str(close),
    /* K6   */  be_nested_str(connections),
    /* K7   */  be_nested_str(stop_iteration),
    }),
    &be_const_str_close,
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0xB8060000,  //  0004  GETNGBL	R1	K0
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x880C0103,  //  0006  GETMBR	R3	R0	K3
      0x7C040400,  //  0007  CALL	R1	2
      0x4C040000,  //  0008  LDNIL	R1
      0x90020601,  //  0009  SETMBR	R0	K3	R1
      0x88040104,  //  000A  GETMBR	R1	R0	K4
      0x8C040305,  //  000B  GETMET	R1	R1	K5
      0x7C040200,  //  000C  CALL	R1	1
      0x60040010,  //  000D  GETGBL	R1	G16
      0x88080106,  //  000E  GETMBR	R2	R0	K6
      0x7C040200,  //  000F  CALL	R1	1
      0xA8020004,  //  0010  EXBLK	0	#0016
      0x5C080200,  //  0011  MOVE	R2	R1
      0x7C080000,  //  0012  CALL	R2	0
      0x8C0C0505,  //  0013  GETMET	R3	R2	K5
      0x7C0C0200,  //  0014  CALL	R3	1
      0x7001FFFA,  //  0015  JMP		#0011
      0x58040007,  //  0016  LDCONST	R1	K7
      0xAC040200,  //  0017  CATCH	R1	1	0
      0xB0080000,  //  0018  RAISE	2	R0	R0
      0x4C040000,  //  0019  LDNIL	R1
      0x90020C01,  //  001A  SETMBR	R0	K6	R1
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compile_re
********************************************************************/
extern const bclass be_class_webserver_async;
be_local_closure(class_webserver_async_compile_re,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_webserver_async, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str(re),
    /* K1   */  be_nested_str(global),
    /* K2   */  be_nested_str(contains),
    /* K3   */  be_nested_str(_re_http_srv),
    /* K4   */  be_nested_str(compile),
    /* K5   */  be_nested_str(HTTP_REQ),
    /* K6   */  be_nested_str(_re_http_srv_header),
    /* K7   */  be_nested_str(HTTP_HEADER_REGEX),
    /* K8   */  be_nested_str(_re_http_srv_body),
    /* K9   */  be_nested_str(HTTP_BODY_REGEX),
    }),
    &be_const_str_compile_re,
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x7C080400,  //  0004  CALL	R2	2
      0x740A000E,  //  0005  JMPT	R2	#0015
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
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: webserver_async
********************************************************************/
be_local_class(webserver_async,
    10,
    NULL,
    be_nested_map(27,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(bytes_format_hex, -1), be_const_static_closure(class_webserver_async_bytes_format_hex_closure) },
        { be_const_key(clean_connections, -1), be_const_closure(class_webserver_async_clean_connections_closure) },
        { be_const_key(cors, 1), be_const_var(7) },
        { be_const_key(dispatch, 6), be_const_closure(class_webserver_async_dispatch_closure) },
        { be_const_key(HTTP_REQ, 16), be_nested_str(_X5E_X28_X5Cw_X2B_X29_X20_X28_X5CS_X2B_X29_X20HTTP_X5C_X2F_X28_X5Cd_X5C_X2E_X5Cd_X29_X0D_X0A) },
        { be_const_key(init, -1), be_const_closure(class_webserver_async_init_closure) },
        { be_const_key(webserver_async_cnx, -1), be_const_class(be_class_webserver_async_cnx) },
        { be_const_key(close, -1), be_const_closure(class_webserver_async_close_closure) },
        { be_const_key(connections, -1), be_const_var(4) },
        { be_const_key(server, 2), be_const_var(1) },
        { be_const_key(loop, 9), be_const_closure(class_webserver_async_loop_closure) },
        { be_const_key(payload2, -1), be_const_var(9) },
        { be_const_key(dispatchers, 10), be_const_var(5) },
        { be_const_key(chunked, 19), be_const_var(6) },
        { be_const_key(payload1, -1), be_const_var(8) },
        { be_const_key(HTTP_BODY_REGEX, -1), be_nested_str(_X0D_X0A) },
        { be_const_key(local_port, 5), be_const_var(0) },
        { be_const_key(set_chunked, -1), be_const_closure(class_webserver_async_set_chunked_closure) },
        { be_const_key(HTTP_HEADER_REGEX, 13), be_nested_str(_X28_X5BA_X2DZa_X2Dz0_X2D9_X2D_X5D_X2B_X29_X3A_X20_X28_X2E_X2A_X3F_X29_X0D_X0A) },
        { be_const_key(set_cors, -1), be_const_closure(class_webserver_async_set_cors_closure) },
        { be_const_key(on, -1), be_const_closure(class_webserver_async_on_closure) },
        { be_const_key(TIMEOUT, 7), be_const_int(1000) },
        { be_const_key(timeout, -1), be_const_var(3) },
        { be_const_key(compile_re, -1), be_const_closure(class_webserver_async_compile_re_closure) },
        { be_const_key(bytes_format_int, 4), be_const_static_closure(class_webserver_async_bytes_format_int_closure) },
        { be_const_key(webserver_async_dispatcher, -1), be_const_class(be_class_webserver_async_dispatcher) },
        { be_const_key(fastloop_cb, -1), be_const_var(2) },
    })),
    (bstring*) &be_const_str_webserver_async
);
/********************************************************************/
/* End of solidification */
