/* Solidification of Matter_Plugin_Bridge_Light2.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Light2;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(update_ct_minmax),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x8C100101,  //  0008  GETMET	R4	R0	K1
      0x7C100200,  //  0009  CALL	R4	1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_update
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_parse_update,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(parse_update),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(CT),
    /* K3   */  be_nested_str_weak(shadow_ct),
    /* K4   */  be_nested_str_weak(ct_min),
    /* K5   */  be_nested_str_weak(ct_max),
    /* K6   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(parse_update),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x540E000A,  //  0007  LDINT	R3	11
      0x1C0C0403,  //  0008  EQ	R3	R2	R3
      0x780E0017,  //  0009  JMPF	R3	#0022
      0x600C0009,  //  000A  GETGBL	R3	G9
      0x8C100301,  //  000B  GETMET	R4	R1	K1
      0x58180002,  //  000C  LDCONST	R6	K2
      0x7C100400,  //  000D  CALL	R4	2
      0x7C0C0200,  //  000E  CALL	R3	1
      0x4C100000,  //  000F  LDNIL	R4
      0x20100604,  //  0010  NE	R4	R3	R4
      0x7812000F,  //  0011  JMPF	R4	#0022
      0x88100103,  //  0012  GETMBR	R4	R0	K3
      0x20100604,  //  0013  NE	R4	R3	R4
      0x7812000C,  //  0014  JMPF	R4	#0022
      0x88100104,  //  0015  GETMBR	R4	R0	K4
      0x14100604,  //  0016  LT	R4	R3	R4
      0x78120000,  //  0017  JMPF	R4	#0019
      0x880C0104,  //  0018  GETMBR	R3	R0	K4
      0x88100105,  //  0019  GETMBR	R4	R0	K5
      0x24100604,  //  001A  GT	R4	R3	R4
      0x78120000,  //  001B  JMPF	R4	#001D
      0x880C0105,  //  001C  GETMBR	R3	R0	K5
      0x8C100106,  //  001D  GETMET	R4	R0	K6
      0x541A02FF,  //  001E  LDINT	R6	768
      0x541E0006,  //  001F  LDINT	R7	7
      0x7C100600,  //  0020  CALL	R4	3
      0x90020603,  //  0021  SETMBR	R0	K3	R3
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_web_values,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X7C_X20Light_X20_X25s_X20_X25s_X20_X25s),
    /* K5   */  be_nested_str_weak(web_value_onoff),
    /* K6   */  be_nested_str_weak(shadow_onoff),
    /* K7   */  be_nested_str_weak(web_value_dimmer),
    /* K8   */  be_nested_str_weak(web_value_ct),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x8C200105,  //  0005  GETMET	R8	R0	K5
      0x88280106,  //  0006  GETMBR	R10	R0	K6
      0x7C200400,  //  0007  CALL	R8	2
      0x8C240107,  //  0008  GETMET	R9	R0	K7
      0x7C240200,  //  0009  CALL	R9	1
      0x8C280108,  //  000A  GETMET	R10	R0	K8
      0x7C280200,  //  000B  CALL	R10	1
      0x7C140A00,  //  000C  CALL	R5	5
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_value_ct
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_web_value_ct,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(shadow_ct),
    /* K3   */  be_const_int(1000000),
    /* K4   */  be_nested_str_weak(format),
    /* K5   */  be_nested_str_weak(_X25iK),
    /* K6   */  be_nested_str_weak(_X26_X239898_X3B_X20),
    }),
    be_str_weak(web_value_ct),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x58080001,  //  0001  LDCONST	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x4C100000,  //  0003  LDNIL	R4
      0x200C0604,  //  0004  NE	R3	R3	R4
      0x780E000C,  //  0005  JMPF	R3	#0013
      0x880C0102,  //  0006  GETMBR	R3	R0	K2
      0x0C0E0603,  //  0007  DIV	R3	K3	R3
      0x54120018,  //  0008  LDINT	R4	25
      0x000C0604,  //  0009  ADD	R3	R3	R4
      0x54120031,  //  000A  LDINT	R4	50
      0x0C0C0604,  //  000B  DIV	R3	R3	R4
      0x54120031,  //  000C  LDINT	R4	50
      0x080C0604,  //  000D  MUL	R3	R3	R4
      0x8C100304,  //  000E  GETMET	R4	R1	K4
      0x58180005,  //  000F  LDCONST	R6	K5
      0x5C1C0600,  //  0010  MOVE	R7	R3
      0x7C100600,  //  0011  CALL	R4	3
      0x5C080800,  //  0012  MOVE	R2	R4
      0x000E0C02,  //  0013  ADD	R3	K6	R2
      0x80040600,  //  0014  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ct
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_set_ct,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(call_remote_sync),
    /* K1   */  be_nested_str_weak(CT),
    /* K2   */  be_nested_str_weak(parse_update),
    }),
    be_str_weak(set_ct),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x60140008,  //  0002  GETGBL	R5	G8
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C140200,  //  0004  CALL	R5	1
      0x7C080600,  //  0005  CALL	R2	3
      0x4C0C0000,  //  0006  LDNIL	R3
      0x200C0403,  //  0007  NE	R3	R2	R3
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x8C0C0102,  //  0009  GETMET	R3	R0	K2
      0x5C140400,  //  000A  MOVE	R5	R2
      0x541A000A,  //  000B  LDINT	R6	11
      0x7C0C0600,  //  000C  CALL	R3	3
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_invoke_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_nested_str_weak(findsubval),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(ct_min),
    /* K8   */  be_nested_str_weak(ct_max),
    /* K9   */  be_nested_str_weak(set_ct),
    /* K10  */  be_nested_str_weak(log),
    /* K11  */  be_nested_str_weak(ct_X3A),
    /* K12  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E002C,  //  0006  JMPF	R7	#0034
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x541E0009,  //  0009  LDINT	R7	10
      0x1C1C0C07,  //  000A  EQ	R7	R6	R7
      0x781E0015,  //  000B  JMPF	R7	#0022
      0x8C1C0505,  //  000C  GETMET	R7	R2	K5
      0x58240006,  //  000D  LDCONST	R9	K6
      0x7C1C0400,  //  000E  CALL	R7	2
      0x88200107,  //  000F  GETMBR	R8	R0	K7
      0x14200E08,  //  0010  LT	R8	R7	R8
      0x78220000,  //  0011  JMPF	R8	#0013
      0x881C0107,  //  0012  GETMBR	R7	R0	K7
      0x88200108,  //  0013  GETMBR	R8	R0	K8
      0x24200E08,  //  0014  GT	R8	R7	R8
      0x78220000,  //  0015  JMPF	R8	#0017
      0x881C0108,  //  0016  GETMBR	R7	R0	K8
      0x8C200109,  //  0017  GETMET	R8	R0	K9
      0x5C280E00,  //  0018  MOVE	R10	R7
      0x7C200400,  //  0019  CALL	R8	2
      0x60200008,  //  001A  GETGBL	R8	G8
      0x5C240E00,  //  001B  MOVE	R9	R7
      0x7C200200,  //  001C  CALL	R8	1
      0x00221608,  //  001D  ADD	R8	K11	R8
      0x900E1408,  //  001E  SETMBR	R3	K10	R8
      0x50200200,  //  001F  LDBOOL	R8	1	0
      0x80041000,  //  0020  RET	1	R8
      0x70020010,  //  0021  JMP		#0033
      0x541E0046,  //  0022  LDINT	R7	71
      0x1C1C0C07,  //  0023  EQ	R7	R6	R7
      0x781E0002,  //  0024  JMPF	R7	#0028
      0x501C0200,  //  0025  LDBOOL	R7	1	0
      0x80040E00,  //  0026  RET	1	R7
      0x7002000A,  //  0027  JMP		#0033
      0x541E004A,  //  0028  LDINT	R7	75
      0x1C1C0C07,  //  0029  EQ	R7	R6	R7
      0x781E0002,  //  002A  JMPF	R7	#002E
      0x501C0200,  //  002B  LDBOOL	R7	1	0
      0x80040E00,  //  002C  RET	1	R7
      0x70020004,  //  002D  JMP		#0033
      0x541E004B,  //  002E  LDINT	R7	76
      0x1C1C0C07,  //  002F  EQ	R7	R6	R7
      0x781E0001,  //  0030  JMPF	R7	#0033
      0x501C0200,  //  0031  LDBOOL	R7	1	0
      0x80040E00,  //  0032  RET	1	R7
      0x70020008,  //  0033  JMP		#003D
      0x601C0003,  //  0034  GETGBL	R7	G3
      0x5C200000,  //  0035  MOVE	R8	R0
      0x7C1C0200,  //  0036  CALL	R7	1
      0x8C1C0F0C,  //  0037  GETMET	R7	R7	K12
      0x5C240200,  //  0038  MOVE	R9	R1
      0x5C280400,  //  0039  MOVE	R10	R2
      0x5C2C0600,  //  003A  MOVE	R11	R3
      0x7C1C0800,  //  003B  CALL	R7	4
      0x80040E00,  //  003C  RET	1	R7
      0x80000000,  //  003D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_ct_minmax
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_update_ct_minmax,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(get_option),
    /* K2   */  be_nested_str_weak(ct_min),
    /* K3   */  be_nested_str_weak(ct_max),
    }),
    be_str_weak(update_ct_minmax),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x540E0051,  //  0002  LDINT	R3	82
      0x7C040400,  //  0003  CALL	R1	2
      0x78060001,  //  0004  JMPF	R1	#0007
      0x540A00C7,  //  0005  LDINT	R2	200
      0x70020000,  //  0006  JMP		#0008
      0x540A0098,  //  0007  LDINT	R2	153
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x540A017B,  //  000A  LDINT	R2	380
      0x70020000,  //  000B  JMP		#000D
      0x540A01F3,  //  000C  LDINT	R2	500
      0x90020602,  //  000D  SETMBR	R0	K3	R2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_nested_str_weak(shadow_ct),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(U1),
    /* K9   */  be_nested_str_weak(NULL),
    /* K10  */  be_const_int(2),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(ct_min),
    /* K13  */  be_nested_str_weak(ct_max),
    /* K14  */  be_nested_str_weak(U4),
    /* K15  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[101]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E02FF,  //  0005  LDINT	R7	768
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0053,  //  0007  JMPF	R7	#005C
      0x8C1C0105,  //  0008  GETMET	R7	R0	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x541E0006,  //  000A  LDINT	R7	7
      0x1C1C0C07,  //  000B  EQ	R7	R6	R7
      0x781E000F,  //  000C  JMPF	R7	#001D
      0x881C0106,  //  000D  GETMBR	R7	R0	K6
      0x4C200000,  //  000E  LDNIL	R8
      0x201C0E08,  //  000F  NE	R7	R7	R8
      0x781E0005,  //  0010  JMPF	R7	#0017
      0x8C1C0907,  //  0011  GETMET	R7	R4	K7
      0x88240908,  //  0012  GETMBR	R9	R4	K8
      0x88280106,  //  0013  GETMBR	R10	R0	K6
      0x7C1C0600,  //  0014  CALL	R7	3
      0x80040E00,  //  0015  RET	1	R7
      0x70020004,  //  0016  JMP		#001C
      0x8C1C0907,  //  0017  GETMET	R7	R4	K7
      0x88240909,  //  0018  GETMBR	R9	R4	K9
      0x4C280000,  //  0019  LDNIL	R10
      0x7C1C0600,  //  001A  CALL	R7	3
      0x80040E00,  //  001B  RET	1	R7
      0x7002003D,  //  001C  JMP		#005B
      0x541E0007,  //  001D  LDINT	R7	8
      0x1C1C0C07,  //  001E  EQ	R7	R6	R7
      0x781E0005,  //  001F  JMPF	R7	#0026
      0x8C1C0907,  //  0020  GETMET	R7	R4	K7
      0x88240908,  //  0021  GETMBR	R9	R4	K8
      0x5828000A,  //  0022  LDCONST	R10	K10
      0x7C1C0600,  //  0023  CALL	R7	3
      0x80040E00,  //  0024  RET	1	R7
      0x70020034,  //  0025  JMP		#005B
      0x541E000E,  //  0026  LDINT	R7	15
      0x1C1C0C07,  //  0027  EQ	R7	R6	R7
      0x781E0005,  //  0028  JMPF	R7	#002F
      0x8C1C0907,  //  0029  GETMET	R7	R4	K7
      0x88240908,  //  002A  GETMBR	R9	R4	K8
      0x5828000B,  //  002B  LDCONST	R10	K11
      0x7C1C0600,  //  002C  CALL	R7	3
      0x80040E00,  //  002D  RET	1	R7
      0x7002002B,  //  002E  JMP		#005B
      0x541E400A,  //  002F  LDINT	R7	16395
      0x1C1C0C07,  //  0030  EQ	R7	R6	R7
      0x781E0005,  //  0031  JMPF	R7	#0038
      0x8C1C0907,  //  0032  GETMET	R7	R4	K7
      0x88240908,  //  0033  GETMBR	R9	R4	K8
      0x8828010C,  //  0034  GETMBR	R10	R0	K12
      0x7C1C0600,  //  0035  CALL	R7	3
      0x80040E00,  //  0036  RET	1	R7
      0x70020022,  //  0037  JMP		#005B
      0x541E400B,  //  0038  LDINT	R7	16396
      0x1C1C0C07,  //  0039  EQ	R7	R6	R7
      0x781E0005,  //  003A  JMPF	R7	#0041
      0x8C1C0907,  //  003B  GETMET	R7	R4	K7
      0x88240908,  //  003C  GETMBR	R9	R4	K8
      0x8828010D,  //  003D  GETMBR	R10	R0	K13
      0x7C1C0600,  //  003E  CALL	R7	3
      0x80040E00,  //  003F  RET	1	R7
      0x70020019,  //  0040  JMP		#005B
      0x541E4009,  //  0041  LDINT	R7	16394
      0x1C1C0C07,  //  0042  EQ	R7	R6	R7
      0x781E0005,  //  0043  JMPF	R7	#004A
      0x8C1C0907,  //  0044  GETMET	R7	R4	K7
      0x8824090E,  //  0045  GETMBR	R9	R4	K14
      0x542A000F,  //  0046  LDINT	R10	16
      0x7C1C0600,  //  0047  CALL	R7	3
      0x80040E00,  //  0048  RET	1	R7
      0x70020010,  //  0049  JMP		#005B
      0x541EFFFB,  //  004A  LDINT	R7	65532
      0x1C1C0C07,  //  004B  EQ	R7	R6	R7
      0x781E0005,  //  004C  JMPF	R7	#0053
      0x8C1C0907,  //  004D  GETMET	R7	R4	K7
      0x8824090E,  //  004E  GETMBR	R9	R4	K14
      0x542A000F,  //  004F  LDINT	R10	16
      0x7C1C0600,  //  0050  CALL	R7	3
      0x80040E00,  //  0051  RET	1	R7
      0x70020007,  //  0052  JMP		#005B
      0x541EFFFC,  //  0053  LDINT	R7	65533
      0x1C1C0C07,  //  0054  EQ	R7	R6	R7
      0x781E0004,  //  0055  JMPF	R7	#005B
      0x8C1C0907,  //  0056  GETMET	R7	R4	K7
      0x8824090E,  //  0057  GETMBR	R9	R4	K14
      0x542A0004,  //  0058  LDINT	R10	5
      0x7C1C0600,  //  0059  CALL	R7	3
      0x80040E00,  //  005A  RET	1	R7
      0x70020007,  //  005B  JMP		#0064
      0x601C0003,  //  005C  GETGBL	R7	G3
      0x5C200000,  //  005D  MOVE	R8	R0
      0x7C1C0200,  //  005E  CALL	R7	1
      0x8C1C0F0F,  //  005F  GETMET	R7	R7	K15
      0x5C240200,  //  0060  MOVE	R9	R1
      0x5C280400,  //  0061  MOVE	R10	R2
      0x7C1C0600,  //  0062  CALL	R7	3
      0x80040E00,  //  0063  RET	1	R7
      0x80000000,  //  0064  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Light2
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Light1;
be_local_class(Matter_Plugin_Bridge_Light2,
    3,
    &be_class_Matter_Plugin_Bridge_Light1,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Bridge_Light2_init_closure) },
        { be_const_key_weak(parse_update, 10), be_const_closure(Matter_Plugin_Bridge_Light2_parse_update_closure) },
        { be_const_key_weak(web_values, 11), be_const_closure(Matter_Plugin_Bridge_Light2_web_values_closure) },
        { be_const_key_weak(ct_max, -1), be_const_var(2) },
        { be_const_key_weak(CLUSTERS, 14), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(768, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(7),
        be_const_int(8),
        be_const_int(15),
        be_const_int(16394),
        be_const_int(16395),
        be_const_int(16396),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(268, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(web_value_ct, -1), be_const_closure(Matter_Plugin_Bridge_Light2_web_value_ct_closure) },
        { be_const_key_weak(set_ct, -1), be_const_closure(Matter_Plugin_Bridge_Light2_set_ct_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Bridge_Light2_read_attribute_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Light_X202_X20CT) },
        { be_const_key_weak(shadow_ct, -1), be_const_var(0) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Bridge_Light2_invoke_request_closure) },
        { be_const_key_weak(update_ct_minmax, 8), be_const_closure(Matter_Plugin_Bridge_Light2_update_ct_minmax_closure) },
        { be_const_key_weak(ct_min, -1), be_const_var(1) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_light2) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Light2)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_Light2_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_Light2);
    be_setglobal(vm, "Matter_Plugin_Bridge_Light2");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
