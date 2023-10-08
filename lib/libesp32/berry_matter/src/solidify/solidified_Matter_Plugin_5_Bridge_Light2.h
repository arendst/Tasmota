/* Solidification of Matter_Plugin_5_Bridge_Light2.h */
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
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X25s_X20_X25s_X20_X25s),
    /* K4   */  be_nested_str_weak(web_value_onoff),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    /* K6   */  be_nested_str_weak(web_value_dimmer),
    /* K7   */  be_nested_str_weak(web_value_ct),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140003,  //  0005  LDCONST	R5	K3
      0x8C180104,  //  0006  GETMET	R6	R0	K4
      0x88200105,  //  0007  GETMBR	R8	R0	K5
      0x7C180400,  //  0008  CALL	R6	2
      0x8C1C0106,  //  0009  GETMET	R7	R0	K6
      0x7C1C0200,  //  000A  CALL	R7	1
      0x8C200107,  //  000B  GETMET	R8	R0	K7
      0x7C200200,  //  000C  CALL	R8	1
      0x7C100800,  //  000D  CALL	R4	4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_value_ct
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_web_value_ct,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(shadow_ct),
    /* K2   */  be_const_int(1000000),
    /* K3   */  be_nested_str_weak(_X25iK),
    /* K4   */  be_nested_str_weak(_X26_X239898_X3B_X20),
    }),
    be_str_weak(web_value_ct),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A000C,  //  0004  JMPF	R2	#0012
      0x88080101,  //  0005  GETMBR	R2	R0	K1
      0x0C0A0402,  //  0006  DIV	R2	K2	R2
      0x540E0018,  //  0007  LDINT	R3	25
      0x00080403,  //  0008  ADD	R2	R2	R3
      0x540E0031,  //  0009  LDINT	R3	50
      0x0C080403,  //  000A  DIV	R2	R2	R3
      0x540E0031,  //  000B  LDINT	R3	50
      0x08080403,  //  000C  MUL	R2	R2	R3
      0x600C0018,  //  000D  GETGBL	R3	G24
      0x58100003,  //  000E  LDCONST	R4	K3
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x5C040600,  //  0011  MOVE	R1	R3
      0x000A0801,  //  0012  ADD	R2	K4	R1
      0x80040400,  //  0013  RET	1	R2
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_nested_str_weak(findsubval),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(ct_min),
    /* K7   */  be_nested_str_weak(ct_max),
    /* K8   */  be_nested_str_weak(set_ct),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(ct_X3A),
    /* K11  */  be_nested_str_weak(publish_command),
    /* K12  */  be_nested_str_weak(CT),
    /* K13  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E002E,  //  0006  JMPF	R7	#0036
      0x541E0009,  //  0007  LDINT	R7	10
      0x1C1C0C07,  //  0008  EQ	R7	R6	R7
      0x781E0019,  //  0009  JMPF	R7	#0024
      0x8C1C0504,  //  000A  GETMET	R7	R2	K4
      0x58240005,  //  000B  LDCONST	R9	K5
      0x7C1C0400,  //  000C  CALL	R7	2
      0x88200106,  //  000D  GETMBR	R8	R0	K6
      0x14200E08,  //  000E  LT	R8	R7	R8
      0x78220000,  //  000F  JMPF	R8	#0011
      0x881C0106,  //  0010  GETMBR	R7	R0	K6
      0x88200107,  //  0011  GETMBR	R8	R0	K7
      0x24200E08,  //  0012  GT	R8	R7	R8
      0x78220000,  //  0013  JMPF	R8	#0015
      0x881C0107,  //  0014  GETMBR	R7	R0	K7
      0x8C200108,  //  0015  GETMET	R8	R0	K8
      0x5C280E00,  //  0016  MOVE	R10	R7
      0x7C200400,  //  0017  CALL	R8	2
      0x60200008,  //  0018  GETGBL	R8	G8
      0x5C240E00,  //  0019  MOVE	R9	R7
      0x7C200200,  //  001A  CALL	R8	1
      0x00221408,  //  001B  ADD	R8	K10	R8
      0x900E1208,  //  001C  SETMBR	R3	K9	R8
      0x8C20010B,  //  001D  GETMET	R8	R0	K11
      0x5828000C,  //  001E  LDCONST	R10	K12
      0x5C2C0E00,  //  001F  MOVE	R11	R7
      0x7C200600,  //  0020  CALL	R8	3
      0x50200200,  //  0021  LDBOOL	R8	1	0
      0x80041000,  //  0022  RET	1	R8
      0x70020010,  //  0023  JMP		#0035
      0x541E0046,  //  0024  LDINT	R7	71
      0x1C1C0C07,  //  0025  EQ	R7	R6	R7
      0x781E0002,  //  0026  JMPF	R7	#002A
      0x501C0200,  //  0027  LDBOOL	R7	1	0
      0x80040E00,  //  0028  RET	1	R7
      0x7002000A,  //  0029  JMP		#0035
      0x541E004A,  //  002A  LDINT	R7	75
      0x1C1C0C07,  //  002B  EQ	R7	R6	R7
      0x781E0002,  //  002C  JMPF	R7	#0030
      0x501C0200,  //  002D  LDBOOL	R7	1	0
      0x80040E00,  //  002E  RET	1	R7
      0x70020004,  //  002F  JMP		#0035
      0x541E004B,  //  0030  LDINT	R7	76
      0x1C1C0C07,  //  0031  EQ	R7	R6	R7
      0x781E0001,  //  0032  JMPF	R7	#0035
      0x501C0200,  //  0033  LDBOOL	R7	1	0
      0x80040E00,  //  0034  RET	1	R7
      0x70020008,  //  0035  JMP		#003F
      0x601C0003,  //  0036  GETGBL	R7	G3
      0x5C200000,  //  0037  MOVE	R8	R0
      0x7C1C0200,  //  0038  CALL	R7	1
      0x8C1C0F0D,  //  0039  GETMET	R7	R7	K13
      0x5C240200,  //  003A  MOVE	R9	R1
      0x5C280400,  //  003B  MOVE	R10	R2
      0x5C2C0600,  //  003C  MOVE	R11	R3
      0x7C1C0800,  //  003D  CALL	R7	4
      0x80040E00,  //  003E  RET	1	R7
      0x80000000,  //  003F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light2_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_nested_str_weak(shadow_ct),
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(NULL),
    /* K9   */  be_const_int(2),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(ct_min),
    /* K12  */  be_nested_str_weak(ct_max),
    /* K13  */  be_nested_str_weak(U4),
    /* K14  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[101]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0053,  //  0006  JMPF	R7	#005B
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x541E0006,  //  0009  LDINT	R7	7
      0x1C1C0C07,  //  000A  EQ	R7	R6	R7
      0x781E000F,  //  000B  JMPF	R7	#001C
      0x881C0105,  //  000C  GETMBR	R7	R0	K5
      0x4C200000,  //  000D  LDNIL	R8
      0x201C0E08,  //  000E  NE	R7	R7	R8
      0x781E0005,  //  000F  JMPF	R7	#0016
      0x8C1C0706,  //  0010  GETMET	R7	R3	K6
      0x88240907,  //  0011  GETMBR	R9	R4	K7
      0x88280105,  //  0012  GETMBR	R10	R0	K5
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x70020004,  //  0015  JMP		#001B
      0x8C1C0706,  //  0016  GETMET	R7	R3	K6
      0x88240908,  //  0017  GETMBR	R9	R4	K8
      0x4C280000,  //  0018  LDNIL	R10
      0x7C1C0600,  //  0019  CALL	R7	3
      0x80040E00,  //  001A  RET	1	R7
      0x7002003D,  //  001B  JMP		#005A
      0x541E0007,  //  001C  LDINT	R7	8
      0x1C1C0C07,  //  001D  EQ	R7	R6	R7
      0x781E0005,  //  001E  JMPF	R7	#0025
      0x8C1C0706,  //  001F  GETMET	R7	R3	K6
      0x88240907,  //  0020  GETMBR	R9	R4	K7
      0x58280009,  //  0021  LDCONST	R10	K9
      0x7C1C0600,  //  0022  CALL	R7	3
      0x80040E00,  //  0023  RET	1	R7
      0x70020034,  //  0024  JMP		#005A
      0x541E000E,  //  0025  LDINT	R7	15
      0x1C1C0C07,  //  0026  EQ	R7	R6	R7
      0x781E0005,  //  0027  JMPF	R7	#002E
      0x8C1C0706,  //  0028  GETMET	R7	R3	K6
      0x88240907,  //  0029  GETMBR	R9	R4	K7
      0x5828000A,  //  002A  LDCONST	R10	K10
      0x7C1C0600,  //  002B  CALL	R7	3
      0x80040E00,  //  002C  RET	1	R7
      0x7002002B,  //  002D  JMP		#005A
      0x541E400A,  //  002E  LDINT	R7	16395
      0x1C1C0C07,  //  002F  EQ	R7	R6	R7
      0x781E0005,  //  0030  JMPF	R7	#0037
      0x8C1C0706,  //  0031  GETMET	R7	R3	K6
      0x88240907,  //  0032  GETMBR	R9	R4	K7
      0x8828010B,  //  0033  GETMBR	R10	R0	K11
      0x7C1C0600,  //  0034  CALL	R7	3
      0x80040E00,  //  0035  RET	1	R7
      0x70020022,  //  0036  JMP		#005A
      0x541E400B,  //  0037  LDINT	R7	16396
      0x1C1C0C07,  //  0038  EQ	R7	R6	R7
      0x781E0005,  //  0039  JMPF	R7	#0040
      0x8C1C0706,  //  003A  GETMET	R7	R3	K6
      0x88240907,  //  003B  GETMBR	R9	R4	K7
      0x8828010C,  //  003C  GETMBR	R10	R0	K12
      0x7C1C0600,  //  003D  CALL	R7	3
      0x80040E00,  //  003E  RET	1	R7
      0x70020019,  //  003F  JMP		#005A
      0x541E4009,  //  0040  LDINT	R7	16394
      0x1C1C0C07,  //  0041  EQ	R7	R6	R7
      0x781E0005,  //  0042  JMPF	R7	#0049
      0x8C1C0706,  //  0043  GETMET	R7	R3	K6
      0x8824090D,  //  0044  GETMBR	R9	R4	K13
      0x542A000F,  //  0045  LDINT	R10	16
      0x7C1C0600,  //  0046  CALL	R7	3
      0x80040E00,  //  0047  RET	1	R7
      0x70020010,  //  0048  JMP		#005A
      0x541EFFFB,  //  0049  LDINT	R7	65532
      0x1C1C0C07,  //  004A  EQ	R7	R6	R7
      0x781E0005,  //  004B  JMPF	R7	#0052
      0x8C1C0706,  //  004C  GETMET	R7	R3	K6
      0x8824090D,  //  004D  GETMBR	R9	R4	K13
      0x542A000F,  //  004E  LDINT	R10	16
      0x7C1C0600,  //  004F  CALL	R7	3
      0x80040E00,  //  0050  RET	1	R7
      0x70020007,  //  0051  JMP		#005A
      0x541EFFFC,  //  0052  LDINT	R7	65533
      0x1C1C0C07,  //  0053  EQ	R7	R6	R7
      0x781E0004,  //  0054  JMPF	R7	#005A
      0x8C1C0706,  //  0055  GETMET	R7	R3	K6
      0x8824090D,  //  0056  GETMBR	R9	R4	K13
      0x542A0004,  //  0057  LDINT	R10	5
      0x7C1C0600,  //  0058  CALL	R7	3
      0x80040E00,  //  0059  RET	1	R7
      0x70020008,  //  005A  JMP		#0064
      0x601C0003,  //  005B  GETGBL	R7	G3
      0x5C200000,  //  005C  MOVE	R8	R0
      0x7C1C0200,  //  005D  CALL	R7	1
      0x8C1C0F0E,  //  005E  GETMET	R7	R7	K14
      0x5C240200,  //  005F  MOVE	R9	R1
      0x5C280400,  //  0060  MOVE	R10	R2
      0x5C2C0600,  //  0061  MOVE	R11	R3
      0x7C1C0800,  //  0062  CALL	R7	4
      0x80040E00,  //  0063  RET	1	R7
      0x80000000,  //  0064  RET	0
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
** Solidified class: Matter_Plugin_Bridge_Light2
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Light1;
be_local_class(Matter_Plugin_Bridge_Light2,
    3,
    &be_class_Matter_Plugin_Bridge_Light1,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Bridge_Light2_init_closure) },
        { be_const_key_weak(parse_update, 9), be_const_closure(Matter_Plugin_Bridge_Light2_parse_update_closure) },
        { be_const_key_weak(web_values, 11), be_const_closure(Matter_Plugin_Bridge_Light2_web_values_closure) },
        { be_const_key_weak(ct_max, -1), be_const_var(2) },
        { be_const_key_weak(CLUSTERS, 14), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(8, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(2),
        be_const_int(3),
        be_const_int(15),
        be_const_int(17),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(17),
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
    }))    ) } )) },
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(5, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
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
        { be_const_key_weak(web_value_ct, 8), be_const_closure(Matter_Plugin_Bridge_Light2_web_value_ct_closure) },
        { be_const_key_weak(set_ct, -1), be_const_closure(Matter_Plugin_Bridge_Light2_set_ct_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X202_X20CT) },
        { be_const_key_weak(shadow_ct, -1), be_const_var(0) },
        { be_const_key_weak(update_ct_minmax, -1), be_const_closure(Matter_Plugin_Bridge_Light2_update_ct_minmax_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Bridge_Light2_invoke_request_closure) },
        { be_const_key_weak(read_attribute, 10), be_const_closure(Matter_Plugin_Bridge_Light2_read_attribute_closure) },
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
