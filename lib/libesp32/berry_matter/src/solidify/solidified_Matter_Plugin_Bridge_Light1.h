/* Solidification of Matter_Plugin_Bridge_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Light1;

/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light1_invoke_request,   /* name */
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
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_nested_str_weak(set_bri),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(bri_X3A),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(set_onoff),
    /* K14  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[89]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x541E0007,  //  0004  LDINT	R7	8
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0047,  //  0006  JMPF	R7	#004F
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E000D,  //  000A  JMPF	R7	#0019
      0x8C1C0506,  //  000B  GETMET	R7	R2	K6
      0x58240005,  //  000C  LDCONST	R9	K5
      0x7C1C0400,  //  000D  CALL	R7	2
      0x8C200107,  //  000E  GETMET	R8	R0	K7
      0x5C280E00,  //  000F  MOVE	R10	R7
      0x7C200400,  //  0010  CALL	R8	2
      0x60200008,  //  0011  GETGBL	R8	G8
      0x5C240E00,  //  0012  MOVE	R9	R7
      0x7C200200,  //  0013  CALL	R8	1
      0x00221208,  //  0014  ADD	R8	K9	R8
      0x900E1008,  //  0015  SETMBR	R3	K8	R8
      0x50200200,  //  0016  LDBOOL	R8	1	0
      0x80041000,  //  0017  RET	1	R8
      0x70020034,  //  0018  JMP		#004E
      0x1C1C0D0A,  //  0019  EQ	R7	R6	K10
      0x781E0002,  //  001A  JMPF	R7	#001E
      0x501C0200,  //  001B  LDBOOL	R7	1	0
      0x80040E00,  //  001C  RET	1	R7
      0x7002002F,  //  001D  JMP		#004E
      0x1C1C0D0B,  //  001E  EQ	R7	R6	K11
      0x781E0002,  //  001F  JMPF	R7	#0023
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x80040E00,  //  0021  RET	1	R7
      0x7002002A,  //  0022  JMP		#004E
      0x1C1C0D0C,  //  0023  EQ	R7	R6	K12
      0x781E0002,  //  0024  JMPF	R7	#0028
      0x501C0200,  //  0025  LDBOOL	R7	1	0
      0x80040E00,  //  0026  RET	1	R7
      0x70020025,  //  0027  JMP		#004E
      0x541E0003,  //  0028  LDINT	R7	4
      0x1C1C0C07,  //  0029  EQ	R7	R6	R7
      0x781E0011,  //  002A  JMPF	R7	#003D
      0x8C1C0506,  //  002B  GETMET	R7	R2	K6
      0x58240005,  //  002C  LDCONST	R9	K5
      0x7C1C0400,  //  002D  CALL	R7	2
      0x8C200107,  //  002E  GETMET	R8	R0	K7
      0x5C280E00,  //  002F  MOVE	R10	R7
      0x7C200400,  //  0030  CALL	R8	2
      0x24200F05,  //  0031  GT	R8	R7	K5
      0x8C24010D,  //  0032  GETMET	R9	R0	K13
      0x5C2C1000,  //  0033  MOVE	R11	R8
      0x7C240400,  //  0034  CALL	R9	2
      0x60240008,  //  0035  GETGBL	R9	G8
      0x5C280E00,  //  0036  MOVE	R10	R7
      0x7C240200,  //  0037  CALL	R9	1
      0x00261209,  //  0038  ADD	R9	K9	R9
      0x900E1009,  //  0039  SETMBR	R3	K8	R9
      0x50240200,  //  003A  LDBOOL	R9	1	0
      0x80041200,  //  003B  RET	1	R9
      0x70020010,  //  003C  JMP		#004E
      0x541E0004,  //  003D  LDINT	R7	5
      0x1C1C0C07,  //  003E  EQ	R7	R6	R7
      0x781E0002,  //  003F  JMPF	R7	#0043
      0x501C0200,  //  0040  LDBOOL	R7	1	0
      0x80040E00,  //  0041  RET	1	R7
      0x7002000A,  //  0042  JMP		#004E
      0x541E0005,  //  0043  LDINT	R7	6
      0x1C1C0C07,  //  0044  EQ	R7	R6	R7
      0x781E0002,  //  0045  JMPF	R7	#0049
      0x501C0200,  //  0046  LDBOOL	R7	1	0
      0x80040E00,  //  0047  RET	1	R7
      0x70020004,  //  0048  JMP		#004E
      0x541E0006,  //  0049  LDINT	R7	7
      0x1C1C0C07,  //  004A  EQ	R7	R6	R7
      0x781E0001,  //  004B  JMPF	R7	#004E
      0x501C0200,  //  004C  LDBOOL	R7	1	0
      0x80040E00,  //  004D  RET	1	R7
      0x70020008,  //  004E  JMP		#0058
      0x601C0003,  //  004F  GETGBL	R7	G3
      0x5C200000,  //  0050  MOVE	R8	R0
      0x7C1C0200,  //  0051  CALL	R7	1
      0x8C1C0F0E,  //  0052  GETMET	R7	R7	K14
      0x5C240200,  //  0053  MOVE	R9	R1
      0x5C280400,  //  0054  MOVE	R10	R2
      0x5C2C0600,  //  0055  MOVE	R11	R3
      0x7C1C0800,  //  0056  CALL	R7	4
      0x80040E00,  //  0057  RET	1	R7
      0x80000000,  //  0058  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light1_read_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
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
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_bri),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(U1),
    /* K9   */  be_nested_str_weak(NULL),
    /* K10  */  be_const_int(2),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(U4),
    /* K13  */  be_const_int(1),
    /* K14  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[98]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A0007,  //  0004  LDINT	R6	8
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A0051,  //  0006  JMPF	R6	#0059
      0x8C180104,  //  0007  GETMET	R6	R0	K4
      0x7C180200,  //  0008  CALL	R6	1
      0x1C180B05,  //  0009  EQ	R6	R5	K5
      0x781A000F,  //  000A  JMPF	R6	#001B
      0x88180106,  //  000B  GETMBR	R6	R0	K6
      0x4C1C0000,  //  000C  LDNIL	R7
      0x20180C07,  //  000D  NE	R6	R6	R7
      0x781A0005,  //  000E  JMPF	R6	#0015
      0x8C180707,  //  000F  GETMET	R6	R3	K7
      0x88200708,  //  0010  GETMBR	R8	R3	K8
      0x88240106,  //  0011  GETMBR	R9	R0	K6
      0x7C180600,  //  0012  CALL	R6	3
      0x80040C00,  //  0013  RET	1	R6
      0x70020004,  //  0014  JMP		#001A
      0x8C180707,  //  0015  GETMET	R6	R3	K7
      0x88200709,  //  0016  GETMBR	R8	R3	K9
      0x4C240000,  //  0017  LDNIL	R9
      0x7C180600,  //  0018  CALL	R6	3
      0x80040C00,  //  0019  RET	1	R6
      0x7002003C,  //  001A  JMP		#0058
      0x1C180B0A,  //  001B  EQ	R6	R5	K10
      0x781A0005,  //  001C  JMPF	R6	#0023
      0x8C180707,  //  001D  GETMET	R6	R3	K7
      0x88200708,  //  001E  GETMBR	R8	R3	K8
      0x58240005,  //  001F  LDCONST	R9	K5
      0x7C180600,  //  0020  CALL	R6	3
      0x80040C00,  //  0021  RET	1	R6
      0x70020034,  //  0022  JMP		#0058
      0x1C180B0B,  //  0023  EQ	R6	R5	K11
      0x781A0005,  //  0024  JMPF	R6	#002B
      0x8C180707,  //  0025  GETMET	R6	R3	K7
      0x88200708,  //  0026  GETMBR	R8	R3	K8
      0x542600FD,  //  0027  LDINT	R9	254
      0x7C180600,  //  0028  CALL	R6	3
      0x80040C00,  //  0029  RET	1	R6
      0x7002002C,  //  002A  JMP		#0058
      0x541A000E,  //  002B  LDINT	R6	15
      0x1C180A06,  //  002C  EQ	R6	R5	R6
      0x781A0005,  //  002D  JMPF	R6	#0034
      0x8C180707,  //  002E  GETMET	R6	R3	K7
      0x88200708,  //  002F  GETMBR	R8	R3	K8
      0x58240005,  //  0030  LDCONST	R9	K5
      0x7C180600,  //  0031  CALL	R6	3
      0x80040C00,  //  0032  RET	1	R6
      0x70020023,  //  0033  JMP		#0058
      0x541A0010,  //  0034  LDINT	R6	17
      0x1C180A06,  //  0035  EQ	R6	R5	R6
      0x781A000F,  //  0036  JMPF	R6	#0047
      0x88180106,  //  0037  GETMBR	R6	R0	K6
      0x4C1C0000,  //  0038  LDNIL	R7
      0x20180C07,  //  0039  NE	R6	R6	R7
      0x781A0005,  //  003A  JMPF	R6	#0041
      0x8C180707,  //  003B  GETMET	R6	R3	K7
      0x88200708,  //  003C  GETMBR	R8	R3	K8
      0x88240106,  //  003D  GETMBR	R9	R0	K6
      0x7C180600,  //  003E  CALL	R6	3
      0x80040C00,  //  003F  RET	1	R6
      0x70020004,  //  0040  JMP		#0046
      0x8C180707,  //  0041  GETMET	R6	R3	K7
      0x88200709,  //  0042  GETMBR	R8	R3	K9
      0x4C240000,  //  0043  LDNIL	R9
      0x7C180600,  //  0044  CALL	R6	3
      0x80040C00,  //  0045  RET	1	R6
      0x70020010,  //  0046  JMP		#0058
      0x541AFFFB,  //  0047  LDINT	R6	65532
      0x1C180A06,  //  0048  EQ	R6	R5	R6
      0x781A0005,  //  0049  JMPF	R6	#0050
      0x8C180707,  //  004A  GETMET	R6	R3	K7
      0x8820070C,  //  004B  GETMBR	R8	R3	K12
      0x5824000D,  //  004C  LDCONST	R9	K13
      0x7C180600,  //  004D  CALL	R6	3
      0x80040C00,  //  004E  RET	1	R6
      0x70020007,  //  004F  JMP		#0058
      0x541AFFFC,  //  0050  LDINT	R6	65533
      0x1C180A06,  //  0051  EQ	R6	R5	R6
      0x781A0004,  //  0052  JMPF	R6	#0058
      0x8C180707,  //  0053  GETMET	R6	R3	K7
      0x8820070C,  //  0054  GETMBR	R8	R3	K12
      0x54260004,  //  0055  LDINT	R9	5
      0x7C180600,  //  0056  CALL	R6	3
      0x80040C00,  //  0057  RET	1	R6
      0x70020007,  //  0058  JMP		#0061
      0x60180003,  //  0059  GETGBL	R6	G3
      0x5C1C0000,  //  005A  MOVE	R7	R0
      0x7C180200,  //  005B  CALL	R6	1
      0x8C180D0E,  //  005C  GETMET	R6	R6	K14
      0x5C200200,  //  005D  MOVE	R8	R1
      0x5C240400,  //  005E  MOVE	R9	R2
      0x7C180600,  //  005F  CALL	R6	3
      0x80040C00,  //  0060  RET	1	R6
      0x80000000,  //  0061  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light1_web_values,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X25s_X20_X25s),
    /* K4   */  be_nested_str_weak(web_value_onoff),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    /* K6   */  be_nested_str_weak(web_value_dimmer),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
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
      0x7C100600,  //  000B  CALL	R4	3
      0x7C080400,  //  000C  CALL	R2	2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bri
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light1_set_bri,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(scale_uint),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(call_remote_sync),
    /* K4   */  be_nested_str_weak(Dimmer),
    /* K5   */  be_nested_str_weak(parse_update),
    }),
    be_str_weak(set_bri),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x58140002,  //  0003  LDCONST	R5	K2
      0x541A00FD,  //  0004  LDINT	R6	254
      0x581C0002,  //  0005  LDCONST	R7	K2
      0x54220063,  //  0006  LDINT	R8	100
      0x7C080C00,  //  0007  CALL	R2	6
      0x8C0C0103,  //  0008  GETMET	R3	R0	K3
      0x58140004,  //  0009  LDCONST	R5	K4
      0x60180008,  //  000A  GETGBL	R6	G8
      0x5C1C0400,  //  000B  MOVE	R7	R2
      0x7C180200,  //  000C  CALL	R6	1
      0x7C0C0600,  //  000D  CALL	R3	3
      0x4C100000,  //  000E  LDNIL	R4
      0x20100604,  //  000F  NE	R4	R3	R4
      0x78120003,  //  0010  JMPF	R4	#0015
      0x8C100105,  //  0011  GETMET	R4	R0	K5
      0x5C180600,  //  0012  MOVE	R6	R3
      0x541E000A,  //  0013  LDINT	R7	11
      0x7C100600,  //  0014  CALL	R4	3
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_update
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light1_parse_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(parse_update),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(Dimmer),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(scale_uint),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_bri),
    /* K7   */  be_nested_str_weak(attribute_updated),
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
      0xB8120600,  //  0012  GETNGBL	R4	K3
      0x8C100904,  //  0013  GETMET	R4	R4	K4
      0x5C180600,  //  0014  MOVE	R6	R3
      0x581C0005,  //  0015  LDCONST	R7	K5
      0x54220063,  //  0016  LDINT	R8	100
      0x58240005,  //  0017  LDCONST	R9	K5
      0x542A00FD,  //  0018  LDINT	R10	254
      0x7C100C00,  //  0019  CALL	R4	6
      0x88140106,  //  001A  GETMBR	R5	R0	K6
      0x20140805,  //  001B  NE	R5	R4	R5
      0x78160004,  //  001C  JMPF	R5	#0022
      0x8C140107,  //  001D  GETMET	R5	R0	K7
      0x541E0007,  //  001E  LDINT	R7	8
      0x58200005,  //  001F  LDCONST	R8	K5
      0x7C140600,  //  0020  CALL	R5	3
      0x90020C04,  //  0021  SETMBR	R0	K6	R4
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_value_dimmer
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light1_web_value_dimmer,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(shadow_bri),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(scale_uint),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(_X25i_X25_X25),
    /* K6   */  be_nested_str_weak(_X26_X23128261_X3B_X20),
    }),
    be_str_weak(web_value_dimmer),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A000C,  //  0004  JMPF	R2	#0012
      0xB80A0400,  //  0005  GETNGBL	R2	K2
      0x8C080503,  //  0006  GETMET	R2	R2	K3
      0x88100101,  //  0007  GETMBR	R4	R0	K1
      0x58140004,  //  0008  LDCONST	R5	K4
      0x541A00FD,  //  0009  LDINT	R6	254
      0x581C0004,  //  000A  LDCONST	R7	K4
      0x54220063,  //  000B  LDINT	R8	100
      0x7C080C00,  //  000C  CALL	R2	6
      0x600C0018,  //  000D  GETGBL	R3	G24
      0x58100005,  //  000E  LDCONST	R4	K5
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x5C040600,  //  0011  MOVE	R1	R3
      0x000A0C01,  //  0012  ADD	R2	K6	R1
      0x80040400,  //  0013  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Light1
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Light0;
be_local_class(Matter_Plugin_Bridge_Light1,
    1,
    &be_class_Matter_Plugin_Bridge_Light0,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(web_value_dimmer, -1), be_const_closure(Matter_Plugin_Bridge_Light1_web_value_dimmer_closure) },
        { be_const_key_weak(parse_update, -1), be_const_closure(Matter_Plugin_Bridge_Light1_parse_update_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(257, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Bridge_Light1_read_attribute_closure) },
        { be_const_key_weak(TYPE, 2), be_nested_str_weak(http_light1) },
        { be_const_key_weak(shadow_bri, 8), be_const_var(0) },
        { be_const_key_weak(CLUSTERS, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(8, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
    }))    ) } )) },
        { be_const_key_weak(set_bri, -1), be_const_closure(Matter_Plugin_Bridge_Light1_set_bri_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Light_X201_X20Dimmer) },
        { be_const_key_weak(web_values, 1), be_const_closure(Matter_Plugin_Bridge_Light1_web_values_closure) },
        { be_const_key_weak(invoke_request, 0), be_const_closure(Matter_Plugin_Bridge_Light1_invoke_request_closure) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Light1)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_Light1_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_Light1);
    be_setglobal(vm, "Matter_Plugin_Bridge_Light1");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
