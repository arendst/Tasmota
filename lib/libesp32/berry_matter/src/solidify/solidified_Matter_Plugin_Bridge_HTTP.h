/* Solidification of Matter_Plugin_Bridge_HTTP.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_HTTP;

/********************************************************************
** Solidified function: ui_conf_to_string
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_ui_conf_to_string,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Plugin_Bridge_HTTP),
    /* K1   */  be_nested_str_weak(ui_conf_to_string),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(ARG_HTTP),
    /* K4   */  be_nested_str_weak(),
    /* K5   */  be_nested_str_weak(_X2C),
    }),
    be_str_weak(ui_conf_to_string),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C0003,  //  0001  GETGBL	R3	G3
      0x5C100400,  //  0002  MOVE	R4	R2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x5C140000,  //  0005  MOVE	R5	R0
      0x5C180200,  //  0006  MOVE	R6	R1
      0x7C0C0600,  //  0007  CALL	R3	3
      0x60100008,  //  0008  GETGBL	R4	G8
      0x8C140302,  //  0009  GETMET	R5	R1	K2
      0x881C0503,  //  000A  GETMBR	R7	R2	K3
      0x58200004,  //  000B  LDCONST	R8	K4
      0x7C140600,  //  000C  CALL	R5	3
      0x7C100200,  //  000D  CALL	R4	1
      0x00140705,  //  000E  ADD	R5	R3	K5
      0x00140A04,  //  000F  ADD	R5	R5	R4
      0x80040A00,  //  0010  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(parse_http_response),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 6]) {  /* code */
          0x68080000,  //  0000  GETUPV	R2	U0
          0x8C080500,  //  0001  GETMET	R2	R2	K0
          0x5C100000,  //  0002  MOVE	R4	R0
          0x5C140200,  //  0003  MOVE	R5	R1
          0x7C080600,  //  0004  CALL	R2	3
          0x80040400,  //  0005  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(init),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(ARG_HTTP),
    /* K4   */  be_nested_str_weak(http_shadow_map),
    /* K5   */  be_nested_str_weak(reachable),
    /* K6   */  be_nested_str_weak(next_probe_timestamp),
    /* K7   */  be_nested_str_weak(http_remote),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(HTTP_remote),
    /* K10  */  be_nested_str_weak(PROBE_TIMEOUT),
    /* K11  */  be_nested_str_weak(set_cb),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x60140003,  //  0001  GETGBL	R5	G3
      0x5C180000,  //  0002  MOVE	R6	R0
      0x7C140200,  //  0003  CALL	R5	1
      0x8C140B01,  //  0004  GETMET	R5	R5	K1
      0x5C1C0200,  //  0005  MOVE	R7	R1
      0x5C200400,  //  0006  MOVE	R8	R2
      0x5C240600,  //  0007  MOVE	R9	R3
      0x7C140800,  //  0008  CALL	R5	4
      0x8C140702,  //  0009  GETMET	R5	R3	K2
      0x881C0103,  //  000A  GETMBR	R7	R0	K3
      0x7C140400,  //  000B  CALL	R5	2
      0x60180013,  //  000C  GETGBL	R6	G19
      0x7C180000,  //  000D  CALL	R6	0
      0x90020806,  //  000E  SETMBR	R0	K4	R6
      0x50180000,  //  000F  LDBOOL	R6	0	0
      0x90020A06,  //  0010  SETMBR	R0	K5	R6
      0x4C180000,  //  0011  LDNIL	R6
      0x90020C06,  //  0012  SETMBR	R0	K6	R6
      0xB81A1000,  //  0013  GETNGBL	R6	K8
      0x8C180D09,  //  0014  GETMET	R6	R6	K9
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x5426004F,  //  0016  LDINT	R9	80
      0x8828010A,  //  0017  GETMBR	R10	R0	K10
      0x7C180800,  //  0018  CALL	R6	4
      0x90020E06,  //  0019  SETMBR	R0	K7	R6
      0x88180107,  //  001A  GETMBR	R6	R0	K7
      0x8C180D0B,  //  001B  GETMET	R6	R6	K11
      0x84200000,  //  001C  CLOSURE	R8	P0
      0x7C180400,  //  001D  CALL	R6	2
      0xA0000000,  //  001E  CLOSE	R0
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_remote_status_lazy
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_get_remote_status_lazy,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(tick),
    /* K2   */  be_nested_str_weak(http_shadow_map),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(_tick),
    /* K5   */  be_nested_str_weak(call_remote_sync),
    /* K6   */  be_nested_str_weak(Status),
    }),
    be_str_weak(get_remote_status_lazy),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x78120005,  //  0006  JMPF	R4	#000D
      0x8C140903,  //  0007  GETMET	R5	R4	K3
      0x581C0004,  //  0008  LDCONST	R7	K4
      0x7C140400,  //  0009  CALL	R5	2
      0x1C140A03,  //  000A  EQ	R5	R5	R3
      0x78160000,  //  000B  JMPF	R5	#000D
      0x80040800,  //  000C  RET	1	R4
      0x8C140105,  //  000D  GETMET	R5	R0	K5
      0x581C0006,  //  000E  LDCONST	R7	K6
      0x60200008,  //  000F  GETGBL	R8	G8
      0x5C240200,  //  0010  MOVE	R9	R1
      0x7C200200,  //  0011  CALL	R8	1
      0x7C140600,  //  0012  CALL	R5	3
      0x80040A00,  //  0013  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_response
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_parse_http_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(reachable),
    /* K2   */  be_nested_str_weak(device),
    /* K3   */  be_nested_str_weak(tick),
    /* K4   */  be_nested_str_weak(reachable_tick),
    /* K5   */  be_nested_str_weak(json),
    /* K6   */  be_nested_str_weak(load),
    /* K7   */  be_nested_str_weak(contains),
    /* K8   */  be_nested_str_weak(StatusSNS),
    /* K9   */  be_nested_str_weak(StatusSTS),
    /* K10  */  be_nested_str_weak(StatusSHT),
    /* K11  */  be_nested_str_weak(_tick),
    /* K12  */  be_nested_str_weak(http_shadow_map),
    /* K13  */  be_nested_str_weak(parse_update),
    }),
    be_str_weak(parse_http_response),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x240C0300,  //  0000  GT	R3	R1	K0
      0x780E0028,  //  0001  JMPF	R3	#002B
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x90020203,  //  0003  SETMBR	R0	K1	R3
      0x880C0102,  //  0004  GETMBR	R3	R0	K2
      0x880C0703,  //  0005  GETMBR	R3	R3	K3
      0x90020803,  //  0006  SETMBR	R0	K4	R3
      0xA4120A00,  //  0007  IMPORT	R4	K5
      0x8C140906,  //  0008  GETMET	R5	R4	K6
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C140400,  //  000A  CALL	R5	2
      0x4C180000,  //  000B  LDNIL	R6
      0x7816001D,  //  000C  JMPF	R5	#002B
      0x8C1C0B07,  //  000D  GETMET	R7	R5	K7
      0x58240008,  //  000E  LDCONST	R9	K8
      0x7C1C0400,  //  000F  CALL	R7	2
      0x781E0002,  //  0010  JMPF	R7	#0014
      0x94140B08,  //  0011  GETIDX	R5	R5	K8
      0x541A0007,  //  0012  LDINT	R6	8
      0x7002000C,  //  0013  JMP		#0021
      0x8C1C0B07,  //  0014  GETMET	R7	R5	K7
      0x58240009,  //  0015  LDCONST	R9	K9
      0x7C1C0400,  //  0016  CALL	R7	2
      0x781E0002,  //  0017  JMPF	R7	#001B
      0x94140B09,  //  0018  GETIDX	R5	R5	K9
      0x541A000A,  //  0019  LDINT	R6	11
      0x70020005,  //  001A  JMP		#0021
      0x8C1C0B07,  //  001B  GETMET	R7	R5	K7
      0x5824000A,  //  001C  LDCONST	R9	K10
      0x7C1C0400,  //  001D  CALL	R7	2
      0x781E0001,  //  001E  JMPF	R7	#0021
      0x94140B09,  //  001F  GETIDX	R5	R5	K9
      0x541A000C,  //  0020  LDINT	R6	13
      0x4C1C0000,  //  0021  LDNIL	R7
      0x201C0C07,  //  0022  NE	R7	R6	R7
      0x781E0002,  //  0023  JMPF	R7	#0027
      0x98161603,  //  0024  SETIDX	R5	K11	R3
      0x881C010C,  //  0025  GETMBR	R7	R0	K12
      0x981C0C05,  //  0026  SETIDX	R7	R6	R5
      0x8C1C010D,  //  0027  GETMET	R7	R0	K13
      0x5C240A00,  //  0028  MOVE	R9	R5
      0x5C280C00,  //  0029  MOVE	R10	R6
      0x7C1C0600,  //  002A  CALL	R7	3
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_types
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_get_types,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(TYPES),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(stop_iteration),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(get_types),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x60080010,  //  0002  GETGBL	R2	G16
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0004  GETMET	R3	R3	K1
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x88100100,  //  000A  GETMBR	R4	R0	K0
      0x94100803,  //  000B  GETIDX	R4	R4	R3
      0x98040604,  //  000C  SETIDX	R1	R3	R4
      0x7001FFF9,  //  000D  JMP		#0008
      0x58080002,  //  000E  LDCONST	R2	K2
      0xAC080200,  //  000F  CATCH	R2	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x540A0012,  //  0011  LDINT	R2	19
      0x98040503,  //  0012  SETIDX	R1	R2	K3
      0x80040200,  //  0013  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ui_string_to_conf
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_ui_string_to_conf,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Plugin_Bridge_HTTP),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(split),
    /* K3   */  be_nested_str_weak(_X2C),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(ARG_HTTP),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(ui_string_to_conf),
    /* K8   */  be_const_int(0),
    }),
    be_str_weak(ui_string_to_conf),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0x8C140902,  //  0002  GETMET	R5	R4	K2
      0x001C0503,  //  0003  ADD	R7	R2	K3
      0x58200003,  //  0004  LDCONST	R8	K3
      0x58240004,  //  0005  LDCONST	R9	K4
      0x7C140800,  //  0006  CALL	R5	4
      0x88180705,  //  0007  GETMBR	R6	R3	K5
      0x941C0B06,  //  0008  GETIDX	R7	R5	K6
      0x98040C07,  //  0009  SETIDX	R1	R6	R7
      0x60180003,  //  000A  GETGBL	R6	G3
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C180200,  //  000C  CALL	R6	1
      0x8C180D07,  //  000D  GETMET	R6	R6	K7
      0x5C200000,  //  000E  MOVE	R8	R0
      0x5C240200,  //  000F  MOVE	R9	R1
      0x94280B08,  //  0010  GETIDX	R10	R5	K8
      0x7C180800,  //  0011  CALL	R6	4
      0x80040200,  //  0012  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: probe_shadow_async
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_probe_shadow_async,   /* name */
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
    be_str_weak(probe_shadow_async),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_read_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(create_TLV),
    /* K6   */  be_nested_str_weak(BOOL),
    /* K7   */  be_nested_str_weak(reachable),
    /* K8   */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A0038,  //  0004  LDINT	R6	57
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A000B,  //  0006  JMPF	R6	#0013
      0x1C180B04,  //  0007  EQ	R6	R5	K4
      0x781A0000,  //  0008  JMPF	R6	#000A
      0x70020007,  //  0009  JMP		#0012
      0x541A0010,  //  000A  LDINT	R6	17
      0x1C180A06,  //  000B  EQ	R6	R5	R6
      0x781A0004,  //  000C  JMPF	R6	#0012
      0x8C180705,  //  000D  GETMET	R6	R3	K5
      0x88200706,  //  000E  GETMBR	R8	R3	K6
      0x88240107,  //  000F  GETMBR	R9	R0	K7
      0x7C180600,  //  0010  CALL	R6	3
      0x80040C00,  //  0011  RET	1	R6
      0x70020007,  //  0012  JMP		#001B
      0x60180003,  //  0013  GETGBL	R6	G3
      0x5C1C0000,  //  0014  MOVE	R7	R0
      0x7C180200,  //  0015  CALL	R6	1
      0x8C180D08,  //  0016  GETMET	R6	R6	K8
      0x5C200200,  //  0017  MOVE	R8	R1
      0x5C240400,  //  0018  MOVE	R9	R2
      0x7C180600,  //  0019  CALL	R6	3
      0x80040C00,  //  001A  RET	1	R6
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_reachable_sync
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_is_reachable_sync,   /* name */
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
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(tick),
    /* K2   */  be_nested_str_weak(reachable_tick),
    /* K3   */  be_nested_str_weak(call_remote_sync),
    /* K4   */  be_nested_str_weak(),
    /* K5   */  be_nested_str_weak(reachable),
    }),
    be_str_weak(is_reachable_sync),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x88080102,  //  0002  GETMBR	R2	R0	K2
      0x20040202,  //  0003  NE	R1	R1	R2
      0x78060006,  //  0004  JMPF	R1	#000C
      0x8C040103,  //  0005  GETMET	R1	R0	K3
      0x580C0004,  //  0006  LDCONST	R3	K4
      0x58100004,  //  0007  LDCONST	R4	K4
      0x7C040600,  //  0008  CALL	R1	3
      0x4C080000,  //  0009  LDNIL	R2
      0x20080202,  //  000A  NE	R2	R1	R2
      0x90020A02,  //  000B  SETMBR	R0	K5	R2
      0x88040105,  //  000C  GETMBR	R1	R0	K5
      0x80040200,  //  000D  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: call_remote_sync
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_call_remote_sync,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(wifi),
    /* K3   */  be_nested_str_weak(up),
    /* K4   */  be_nested_str_weak(eth),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(format),
    /* K7   */  be_nested_str_weak(_X2Fcm_X3Fcmnd_X3D_X25s_X25_X2520_X25s),
    /* K8   */  be_nested_str_weak(),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(http_remote),
    /* K11  */  be_nested_str_weak(begin_sync),
    /* K12  */  be_nested_str_weak(SYNC_TIMEOUT),
    /* K13  */  be_nested_str_weak(reachable),
    /* K14  */  be_nested_str_weak(reachable_tick),
    /* K15  */  be_nested_str_weak(device),
    /* K16  */  be_nested_str_weak(tick),
    /* K17  */  be_const_int(1),
    /* K18  */  be_nested_str_weak(log),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20GET_X20retrying),
    /* K20  */  be_const_int(3),
    }),
    be_str_weak(call_remote_sync),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x94100903,  //  0004  GETIDX	R4	R4	K3
      0x74120006,  //  0005  JMPT	R4	#000D
      0xB8120200,  //  0006  GETNGBL	R4	K1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x94100903,  //  0009  GETIDX	R4	R4	K3
      0x74120001,  //  000A  JMPT	R4	#000D
      0x4C100000,  //  000B  LDNIL	R4
      0x80040800,  //  000C  RET	1	R4
      0x58100005,  //  000D  LDCONST	R4	K5
      0x8C140706,  //  000E  GETMET	R5	R3	K6
      0x581C0007,  //  000F  LDCONST	R7	K7
      0x5C200200,  //  0010  MOVE	R8	R1
      0x780A0001,  //  0011  JMPF	R2	#0014
      0x5C240400,  //  0012  MOVE	R9	R2
      0x70020000,  //  0013  JMP		#0015
      0x58240008,  //  0014  LDCONST	R9	K8
      0x7C140800,  //  0015  CALL	R5	4
      0x24180909,  //  0016  GT	R6	R4	K9
      0x781A0014,  //  0017  JMPF	R6	#002D
      0x8818010A,  //  0018  GETMBR	R6	R0	K10
      0x8C180D0B,  //  0019  GETMET	R6	R6	K11
      0x5C200A00,  //  001A  MOVE	R8	R5
      0x8824010C,  //  001B  GETMBR	R9	R0	K12
      0x7C180600,  //  001C  CALL	R6	3
      0x4C1C0000,  //  001D  LDNIL	R7
      0x201C0C07,  //  001E  NE	R7	R6	R7
      0x781E0005,  //  001F  JMPF	R7	#0026
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x90021A07,  //  0021  SETMBR	R0	K13	R7
      0x881C010F,  //  0022  GETMBR	R7	R0	K15
      0x881C0F10,  //  0023  GETMBR	R7	R7	K16
      0x90021C07,  //  0024  SETMBR	R0	K14	R7
      0x80040C00,  //  0025  RET	1	R6
      0x04100911,  //  0026  SUB	R4	R4	K17
      0xB81E0200,  //  0027  GETNGBL	R7	K1
      0x8C1C0F12,  //  0028  GETMET	R7	R7	K18
      0x58240013,  //  0029  LDCONST	R9	K19
      0x58280014,  //  002A  LDCONST	R10	K20
      0x7C1C0600,  //  002B  CALL	R7	3
      0x7001FFE8,  //  002C  JMP		#0016
      0x50180000,  //  002D  LDBOOL	R6	0	0
      0x90021A06,  //  002E  SETMBR	R0	K13	R6
      0x4C180000,  //  002F  LDNIL	R6
      0x80040C00,  //  0030  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: call_remote_async
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_HTTP_call_remote_async,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(wifi),
    /* K3   */  be_nested_str_weak(up),
    /* K4   */  be_nested_str_weak(eth),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(_X2Fcm_X3Fcmnd_X3D_X25s_X25_X2520_X25s),
    /* K7   */  be_nested_str_weak(),
    /* K8   */  be_nested_str_weak(http_remote),
    /* K9   */  be_nested_str_weak(begin),
    /* K10  */  be_nested_str_weak(PROBE_TIMEOUT),
    }),
    be_str_weak(call_remote_async),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x94100903,  //  0004  GETIDX	R4	R4	K3
      0x74120006,  //  0005  JMPT	R4	#000D
      0xB8120200,  //  0006  GETNGBL	R4	K1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x94100903,  //  0009  GETIDX	R4	R4	K3
      0x74120001,  //  000A  JMPT	R4	#000D
      0x4C100000,  //  000B  LDNIL	R4
      0x80040800,  //  000C  RET	1	R4
      0x8C100705,  //  000D  GETMET	R4	R3	K5
      0x58180006,  //  000E  LDCONST	R6	K6
      0x5C1C0200,  //  000F  MOVE	R7	R1
      0x780A0001,  //  0010  JMPF	R2	#0013
      0x5C200400,  //  0011  MOVE	R8	R2
      0x70020000,  //  0012  JMP		#0014
      0x58200007,  //  0013  LDCONST	R8	K7
      0x7C100800,  //  0014  CALL	R4	4
      0x88140108,  //  0015  GETMBR	R5	R0	K8
      0x8C140B09,  //  0016  GETMET	R5	R5	K9
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x8820010A,  //  0018  GETMBR	R8	R0	K10
      0x7C140600,  //  0019  CALL	R5	3
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_HTTP
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Bridge_HTTP,
    5,
    &be_class_Matter_Plugin_Device,
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG_HTTP, 1), be_nested_str_weak(url) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(3000) },
        { be_const_key_weak(SYNC_TIMEOUT, -1), be_const_int(500) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_init_closure) },
        { be_const_key_weak(get_remote_status_lazy, 19), be_const_closure(Matter_Plugin_Bridge_HTTP_get_remote_status_lazy_closure) },
        { be_const_key_weak(parse_http_response, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_parse_http_response_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak() },
        { be_const_key_weak(call_remote_sync, 16), be_const_closure(Matter_Plugin_Bridge_HTTP_call_remote_sync_closure) },
        { be_const_key_weak(reachable, -1), be_const_var(0) },
        { be_const_key_weak(get_types, 12), be_const_closure(Matter_Plugin_Bridge_HTTP_get_types_closure) },
        { be_const_key_weak(ui_conf_to_string, 7), be_const_static_closure(Matter_Plugin_Bridge_HTTP_ui_conf_to_string_closure) },
        { be_const_key_weak(http_shadow_map, -1), be_const_var(2) },
        { be_const_key_weak(probe_shadow_async, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_probe_shadow_async_closure) },
        { be_const_key_weak(TYPE, 14), be_nested_str_weak() },
        { be_const_key_weak(PROBE_TIMEOUT, -1), be_const_int(700) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak() },
        { be_const_key_weak(CLUSTERS, 22), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(17),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_read_attribute_closure) },
        { be_const_key_weak(next_probe_timestamp, 17), be_const_var(4) },
        { be_const_key_weak(http_remote, -1), be_const_var(3) },
        { be_const_key_weak(reachable_tick, -1), be_const_var(1) },
        { be_const_key_weak(is_reachable_sync, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_is_reachable_sync_closure) },
        { be_const_key_weak(ui_string_to_conf, -1), be_const_static_closure(Matter_Plugin_Bridge_HTTP_ui_string_to_conf_closure) },
        { be_const_key_weak(call_remote_async, -1), be_const_closure(Matter_Plugin_Bridge_HTTP_call_remote_async_closure) },
    })),
    be_str_weak(Matter_Plugin_Bridge_HTTP)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_HTTP_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_HTTP);
    be_setglobal(vm, "Matter_Plugin_Bridge_HTTP");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
