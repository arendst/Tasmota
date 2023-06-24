/* Solidification of Matter_Plugin_Bridge_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Light0;

/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light0__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(_X3Clambda_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040009,  //  0000  GETGBL	R1	G9
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_onoff
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light0_set_onoff,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(call_remote_sync),
    /* K1   */  be_nested_str_weak(Power),
    /* K2   */  be_nested_str_weak(tasmota_relay_index),
    /* K3   */  be_nested_str_weak(1),
    /* K4   */  be_nested_str_weak(0),
    /* K5   */  be_nested_str_weak(parse_update),
    }),
    be_str_weak(set_onoff),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x60100008,  //  0001  GETGBL	R4	G8
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x00120204,  //  0004  ADD	R4	K1	R4
      0x78060001,  //  0005  JMPF	R1	#0008
      0x58140003,  //  0006  LDCONST	R5	K3
      0x70020000,  //  0007  JMP		#0009
      0x58140004,  //  0008  LDCONST	R5	K4
      0x7C080600,  //  0009  CALL	R2	3
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0003,  //  000C  JMPF	R3	#0011
      0x8C0C0105,  //  000D  GETMET	R3	R0	K5
      0x5C140400,  //  000E  MOVE	R5	R2
      0x541A000A,  //  000F  LDINT	R6	11
      0x7C0C0600,  //  0010  CALL	R3	3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light0_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_onoff),
    /* K2   */  be_nested_str_weak(tasmota_relay_index),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(ARG),
    /* K5   */  be_const_int(1),
    /* K6   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x50100000,  //  0008  LDBOOL	R4	0	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x60100009,  //  000A  GETGBL	R4	G9
      0x8C140703,  //  000B  GETMET	R5	R3	K3
      0x881C0104,  //  000C  GETMBR	R7	R0	K4
      0x58200005,  //  000D  LDCONST	R8	K5
      0x7C140600,  //  000E  CALL	R5	3
      0x7C100200,  //  000F  CALL	R4	1
      0x90020404,  //  0010  SETMBR	R0	K2	R4
      0x88100102,  //  0011  GETMBR	R4	R0	K2
      0x18100906,  //  0012  LE	R4	R4	K6
      0x78120000,  //  0013  JMPF	R4	#0015
      0x90020505,  //  0014  SETMBR	R0	K2	K5
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light0_read_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(BOOL),
    /* K8   */  be_nested_str_weak(shadow_onoff),
    /* K9   */  be_nested_str_weak(U4),
    /* K10  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A0005,  //  0004  LDINT	R6	6
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A001B,  //  0006  JMPF	R6	#0023
      0x8C180104,  //  0007  GETMET	R6	R0	K4
      0x7C180200,  //  0008  CALL	R6	1
      0x1C180B05,  //  0009  EQ	R6	R5	K5
      0x781A0005,  //  000A  JMPF	R6	#0011
      0x8C180706,  //  000B  GETMET	R6	R3	K6
      0x88200707,  //  000C  GETMBR	R8	R3	K7
      0x88240108,  //  000D  GETMBR	R9	R0	K8
      0x7C180600,  //  000E  CALL	R6	3
      0x80040C00,  //  000F  RET	1	R6
      0x70020010,  //  0010  JMP		#0022
      0x541AFFFB,  //  0011  LDINT	R6	65532
      0x1C180A06,  //  0012  EQ	R6	R5	R6
      0x781A0005,  //  0013  JMPF	R6	#001A
      0x8C180706,  //  0014  GETMET	R6	R3	K6
      0x88200709,  //  0015  GETMBR	R8	R3	K9
      0x58240005,  //  0016  LDCONST	R9	K5
      0x7C180600,  //  0017  CALL	R6	3
      0x80040C00,  //  0018  RET	1	R6
      0x70020007,  //  0019  JMP		#0022
      0x541AFFFC,  //  001A  LDINT	R6	65533
      0x1C180A06,  //  001B  EQ	R6	R5	R6
      0x781A0004,  //  001C  JMPF	R6	#0022
      0x8C180706,  //  001D  GETMET	R6	R3	K6
      0x88200709,  //  001E  GETMBR	R8	R3	K9
      0x54260003,  //  001F  LDINT	R9	4
      0x7C180600,  //  0020  CALL	R6	3
      0x80040C00,  //  0021  RET	1	R6
      0x70020007,  //  0022  JMP		#002B
      0x60180003,  //  0023  GETGBL	R6	G3
      0x5C1C0000,  //  0024  MOVE	R7	R0
      0x7C180200,  //  0025  CALL	R6	1
      0x8C180D0A,  //  0026  GETMET	R6	R6	K10
      0x5C200200,  //  0027  MOVE	R8	R1
      0x5C240400,  //  0028  MOVE	R9	R2
      0x7C180600,  //  0029  CALL	R6	3
      0x80040C00,  //  002A  RET	1	R6
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_update
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light0_parse_update,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_relay_index),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(contains),
    /* K3   */  be_nested_str_weak(POWER),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(ON),
    /* K6   */  be_nested_str_weak(shadow_onoff),
    /* K7   */  be_nested_str_weak(attribute_updated),
    /* K8   */  be_const_int(0),
    }),
    be_str_weak(parse_update),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x540E000A,  //  0000  LDINT	R3	11
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0024,  //  0002  JMPF	R3	#0028
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x88100100,  //  0004  GETMBR	R4	R0	K0
      0x1C100901,  //  0005  EQ	R4	R4	K1
      0x78120009,  //  0006  JMPF	R4	#0011
      0x8C100302,  //  0007  GETMET	R4	R1	K2
      0x58180003,  //  0008  LDCONST	R6	K3
      0x7C100400,  //  0009  CALL	R4	2
      0x78120005,  //  000A  JMPF	R4	#0011
      0x8C100304,  //  000B  GETMET	R4	R1	K4
      0x58180003,  //  000C  LDCONST	R6	K3
      0x7C100400,  //  000D  CALL	R4	2
      0x1C100905,  //  000E  EQ	R4	R4	K5
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x70020007,  //  0010  JMP		#0019
      0x8C100304,  //  0011  GETMET	R4	R1	K4
      0x60180008,  //  0012  GETGBL	R6	G8
      0x881C0100,  //  0013  GETMBR	R7	R0	K0
      0x7C180200,  //  0014  CALL	R6	1
      0x001A0606,  //  0015  ADD	R6	K3	R6
      0x7C100400,  //  0016  CALL	R4	2
      0x1C100905,  //  0017  EQ	R4	R4	K5
      0x5C0C0800,  //  0018  MOVE	R3	R4
      0x88100106,  //  0019  GETMBR	R4	R0	K6
      0x4C140000,  //  001A  LDNIL	R5
      0x20100805,  //  001B  NE	R4	R4	R5
      0x78120009,  //  001C  JMPF	R4	#0027
      0x88100106,  //  001D  GETMBR	R4	R0	K6
      0x60140017,  //  001E  GETGBL	R5	G23
      0x5C180600,  //  001F  MOVE	R6	R3
      0x7C140200,  //  0020  CALL	R5	1
      0x20100805,  //  0021  NE	R4	R4	R5
      0x78120003,  //  0022  JMPF	R4	#0027
      0x8C100107,  //  0023  GETMET	R4	R0	K7
      0x541A0005,  //  0024  LDINT	R6	6
      0x581C0008,  //  0025  LDCONST	R7	K8
      0x7C100600,  //  0026  CALL	R4	3
      0x90020C03,  //  0027  SETMBR	R0	K6	R3
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light0_invoke_request,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(set_onoff),
    /* K7   */  be_const_int(1),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E001B,  //  0006  JMPF	R7	#0023
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0005,  //  000A  JMPF	R7	#0011
      0x8C1C0106,  //  000B  GETMET	R7	R0	K6
      0x50240000,  //  000C  LDBOOL	R9	0	0
      0x7C1C0400,  //  000D  CALL	R7	2
      0x501C0200,  //  000E  LDBOOL	R7	1	0
      0x80040E00,  //  000F  RET	1	R7
      0x70020011,  //  0010  JMP		#0023
      0x1C1C0D07,  //  0011  EQ	R7	R6	K7
      0x781E0005,  //  0012  JMPF	R7	#0019
      0x8C1C0106,  //  0013  GETMET	R7	R0	K6
      0x50240200,  //  0014  LDBOOL	R9	1	0
      0x7C1C0400,  //  0015  CALL	R7	2
      0x501C0200,  //  0016  LDBOOL	R7	1	0
      0x80040E00,  //  0017  RET	1	R7
      0x70020009,  //  0018  JMP		#0023
      0x1C1C0D08,  //  0019  EQ	R7	R6	K8
      0x781E0007,  //  001A  JMPF	R7	#0023
      0x8C1C0106,  //  001B  GETMET	R7	R0	K6
      0x88240109,  //  001C  GETMBR	R9	R0	K9
      0x78260000,  //  001D  JMPF	R9	#001F
      0x50240001,  //  001E  LDBOOL	R9	0	1
      0x50240200,  //  001F  LDBOOL	R9	1	0
      0x7C1C0400,  //  0020  CALL	R7	2
      0x501C0200,  //  0021  LDBOOL	R7	1	0
      0x80040E00,  //  0022  RET	1	R7
      0x80000000,  //  0023  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light0_web_values,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X25s),
    /* K4   */  be_nested_str_weak(web_value_onoff),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140003,  //  0005  LDCONST	R5	K3
      0x8C180104,  //  0006  GETMET	R6	R0	K4
      0x88200105,  //  0007  GETMBR	R8	R0	K5
      0x7C180400,  //  0008  CALL	R6	2
      0x7C100400,  //  0009  CALL	R4	2
      0x7C080400,  //  000A  CALL	R2	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values_prefix
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light0_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(get_name),
    /* K2   */  be_nested_str_weak(Power),
    /* K3   */  be_nested_str_weak(tasmota_relay_index),
    /* K4   */  be_nested_str_weak(content_send),
    /* K5   */  be_nested_str_weak(PREFIX),
    /* K6   */  be_nested_str_weak(html_escape),
    /* K7   */  be_nested_str_weak(),
    }),
    be_str_weak(web_values_prefix),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E0403,  //  0008  ADD	R3	K2	R3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C0304,  //  000A  GETMET	R3	R1	K4
      0x60140018,  //  000B  GETGBL	R5	G24
      0x88180105,  //  000C  GETMBR	R6	R0	K5
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C1C0306,  //  000E  GETMET	R7	R1	K6
      0x5C240400,  //  000F  MOVE	R9	R2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020000,  //  0011  JMP		#0013
      0x581C0007,  //  0012  LDCONST	R7	K7
      0x7C140400,  //  0013  CALL	R5	2
      0x7C0C0400,  //  0014  CALL	R3	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Light0
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_HTTP;
be_local_class(Matter_Plugin_Bridge_Light0,
    2,
    &be_class_Matter_Plugin_Bridge_HTTP,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG_TYPE, 5), be_const_static_closure(Matter_Plugin_Bridge_Light0__X3Clambda_X3E_closure) },
        { be_const_key_weak(tasmota_relay_index, 4), be_const_var(0) },
        { be_const_key_weak(set_onoff, -1), be_const_closure(Matter_Plugin_Bridge_Light0_set_onoff_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Bridge_Light0_init_closure) },
        { be_const_key_weak(web_values_prefix, 12), be_const_closure(Matter_Plugin_Bridge_Light0_web_values_prefix_closure) },
        { be_const_key_weak(web_values, 14), be_const_closure(Matter_Plugin_Bridge_Light0_web_values_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Light_X200_X20On) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(relay) },
        { be_const_key_weak(parse_update, -1), be_const_closure(Matter_Plugin_Bridge_Light0_parse_update_closure) },
        { be_const_key_weak(CLUSTERS, 15), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, 9), be_const_closure(Matter_Plugin_Bridge_Light0_read_attribute_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Bridge_Light0_invoke_request_closure) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Enter_X20Power_X3Cx_X3E_X20number) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_light0) },
        { be_const_key_weak(shadow_onoff, -1), be_const_var(1) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(256, -1), be_const_int(2) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Light0)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_Light0_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_Light0);
    be_setglobal(vm, "Matter_Plugin_Bridge_Light0");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
