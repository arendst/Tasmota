/* Solidification of Matter_Plugin_Bridge_Light3.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Light3;

/********************************************************************
** Solidified function: set_sat
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_set_sat,   /* name */
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
    /* K4   */  be_nested_str_weak(HSBColor2),
    /* K5   */  be_nested_str_weak(parse_update),
    }),
    be_str_weak(set_sat),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
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
      0x5C180400,  //  000A  MOVE	R6	R2
      0x7C0C0600,  //  000B  CALL	R3	3
      0x4C100000,  //  000C  LDNIL	R4
      0x20100604,  //  000D  NE	R4	R3	R4
      0x78120003,  //  000E  JMPF	R4	#0013
      0x8C100105,  //  000F  GETMET	R4	R0	K5
      0x5C180600,  //  0010  MOVE	R6	R3
      0x541E000A,  //  0011  LDINT	R7	11
      0x7C100600,  //  0012  CALL	R4	3
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_update
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_parse_update,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(parse_update),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(HSBColor),
    /* K3   */  be_nested_str_weak(string),
    /* K4   */  be_nested_str_weak(split),
    /* K5   */  be_nested_str_weak(_X2C),
    /* K6   */  be_const_int(0),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(scale_uint),
    /* K10  */  be_nested_str_weak(shadow_hue),
    /* K11  */  be_nested_str_weak(shadow_sat),
    /* K12  */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(parse_update),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x540E000A,  //  0007  LDINT	R3	11
      0x1C0C0403,  //  0008  EQ	R3	R2	R3
      0x780E003A,  //  0009  JMPF	R3	#0045
      0x8C0C0301,  //  000A  GETMET	R3	R1	K1
      0x58140002,  //  000B  LDCONST	R5	K2
      0x7C0C0400,  //  000C  CALL	R3	2
      0x780E0036,  //  000D  JMPF	R3	#0045
      0xA4120600,  //  000E  IMPORT	R4	K3
      0x8C140904,  //  000F  GETMET	R5	R4	K4
      0x5C1C0600,  //  0010  MOVE	R7	R3
      0x58200005,  //  0011  LDCONST	R8	K5
      0x7C140600,  //  0012  CALL	R5	3
      0x60180009,  //  0013  GETGBL	R6	G9
      0x941C0B06,  //  0014  GETIDX	R7	R5	K6
      0x7C180200,  //  0015  CALL	R6	1
      0x601C0009,  //  0016  GETGBL	R7	G9
      0x94200B07,  //  0017  GETIDX	R8	R5	K7
      0x7C1C0200,  //  0018  CALL	R7	1
      0x4C200000,  //  0019  LDNIL	R8
      0x20200C08,  //  001A  NE	R8	R6	R8
      0x78220009,  //  001B  JMPF	R8	#0026
      0xB8221000,  //  001C  GETNGBL	R8	K8
      0x8C201109,  //  001D  GETMET	R8	R8	K9
      0x5C280C00,  //  001E  MOVE	R10	R6
      0x582C0006,  //  001F  LDCONST	R11	K6
      0x54320167,  //  0020  LDINT	R12	360
      0x58340006,  //  0021  LDCONST	R13	K6
      0x543A00FD,  //  0022  LDINT	R14	254
      0x7C200C00,  //  0023  CALL	R8	6
      0x5C181000,  //  0024  MOVE	R6	R8
      0x70020000,  //  0025  JMP		#0027
      0x8818010A,  //  0026  GETMBR	R6	R0	K10
      0x4C200000,  //  0027  LDNIL	R8
      0x20200E08,  //  0028  NE	R8	R7	R8
      0x78220009,  //  0029  JMPF	R8	#0034
      0xB8221000,  //  002A  GETNGBL	R8	K8
      0x8C201109,  //  002B  GETMET	R8	R8	K9
      0x5C280E00,  //  002C  MOVE	R10	R7
      0x582C0006,  //  002D  LDCONST	R11	K6
      0x54320063,  //  002E  LDINT	R12	100
      0x58340006,  //  002F  LDCONST	R13	K6
      0x543A00FD,  //  0030  LDINT	R14	254
      0x7C200C00,  //  0031  CALL	R8	6
      0x5C1C1000,  //  0032  MOVE	R7	R8
      0x70020000,  //  0033  JMP		#0035
      0x881C010B,  //  0034  GETMBR	R7	R0	K11
      0x8820010A,  //  0035  GETMBR	R8	R0	K10
      0x20200C08,  //  0036  NE	R8	R6	R8
      0x78220004,  //  0037  JMPF	R8	#003D
      0x8C20010C,  //  0038  GETMET	R8	R0	K12
      0x542A02FF,  //  0039  LDINT	R10	768
      0x582C0006,  //  003A  LDCONST	R11	K6
      0x7C200600,  //  003B  CALL	R8	3
      0x90021406,  //  003C  SETMBR	R0	K10	R6
      0x8820010B,  //  003D  GETMBR	R8	R0	K11
      0x20200E08,  //  003E  NE	R8	R7	R8
      0x78220004,  //  003F  JMPF	R8	#0045
      0x8C20010C,  //  0040  GETMET	R8	R0	K12
      0x542A02FF,  //  0041  LDINT	R10	768
      0x582C0007,  //  0042  LDCONST	R11	K7
      0x7C200600,  //  0043  CALL	R8	3
      0x90021607,  //  0044  SETMBR	R0	K11	R7
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_web_values,   /* name */
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
    /* K7   */  be_nested_str_weak(web_value_RGB),
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
** Solidified function: web_value_RGB
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_web_value_RGB,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(shadow_hue),
    /* K1   */  be_nested_str_weak(shadow_sat),
    /* K2   */  be_nested_str_weak(light_state),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str_weak(set_bri),
    /* K5   */  be_nested_str_weak(set_huesat),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(scale_uint),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(_X23_X2502X_X2502X_X2502X),
    /* K10  */  be_nested_str_weak(r),
    /* K11  */  be_nested_str_weak(g),
    /* K12  */  be_nested_str_weak(b),
    /* K13  */  be_nested_str_weak(_X3Ci_X20class_X3D_X22bxm_X22_X20style_X3D_X22_X2D_X2Dcl_X3A_X25s_X22_X3E_X3C_X2Fi_X3E_X25s),
    /* K14  */  be_nested_str_weak(),
    }),
    be_str_weak(web_value_RGB),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060027,  //  0003  JMPF	R1	#002C
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060023,  //  0007  JMPF	R1	#002C
      0xB8060400,  //  0008  GETNGBL	R1	K2
      0x58080003,  //  0009  LDCONST	R2	K3
      0x7C040200,  //  000A  CALL	R1	1
      0x8C080304,  //  000B  GETMET	R2	R1	K4
      0x541200FE,  //  000C  LDINT	R4	255
      0x7C080400,  //  000D  CALL	R2	2
      0x8C080305,  //  000E  GETMET	R2	R1	K5
      0xB8120C00,  //  000F  GETNGBL	R4	K6
      0x8C100907,  //  0010  GETMET	R4	R4	K7
      0x88180100,  //  0011  GETMBR	R6	R0	K0
      0x581C0008,  //  0012  LDCONST	R7	K8
      0x542200FD,  //  0013  LDINT	R8	254
      0x58240008,  //  0014  LDCONST	R9	K8
      0x542A0167,  //  0015  LDINT	R10	360
      0x7C100C00,  //  0016  CALL	R4	6
      0xB8160C00,  //  0017  GETNGBL	R5	K6
      0x8C140B07,  //  0018  GETMET	R5	R5	K7
      0x881C0101,  //  0019  GETMBR	R7	R0	K1
      0x58200008,  //  001A  LDCONST	R8	K8
      0x542600FD,  //  001B  LDINT	R9	254
      0x58280008,  //  001C  LDCONST	R10	K8
      0x542E00FE,  //  001D  LDINT	R11	255
      0x7C140C00,  //  001E  CALL	R5	6
      0x7C080600,  //  001F  CALL	R2	3
      0x60080018,  //  0020  GETGBL	R2	G24
      0x580C0009,  //  0021  LDCONST	R3	K9
      0x8810030A,  //  0022  GETMBR	R4	R1	K10
      0x8814030B,  //  0023  GETMBR	R5	R1	K11
      0x8818030C,  //  0024  GETMBR	R6	R1	K12
      0x7C080800,  //  0025  CALL	R2	4
      0x600C0018,  //  0026  GETGBL	R3	G24
      0x5810000D,  //  0027  LDCONST	R4	K13
      0x5C140400,  //  0028  MOVE	R5	R2
      0x5C180400,  //  0029  MOVE	R6	R2
      0x7C0C0600,  //  002A  CALL	R3	3
      0x80040600,  //  002B  RET	1	R3
      0x80061C00,  //  002C  RET	1	K14
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_invoke_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_nested_str_weak(set_hue),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(hue_X3A),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(set_sat),
    /* K14  */  be_nested_str_weak(sat_X3A),
    /* K15  */  be_nested_str_weak(_X20sat_X3A),
    /* K16  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[107]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0059,  //  0006  JMPF	R7	#0061
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
      0x70020046,  //  0018  JMP		#0060
      0x1C1C0D0A,  //  0019  EQ	R7	R6	K10
      0x781E0002,  //  001A  JMPF	R7	#001E
      0x501C0200,  //  001B  LDBOOL	R7	1	0
      0x80040E00,  //  001C  RET	1	R7
      0x70020041,  //  001D  JMP		#0060
      0x1C1C0D0B,  //  001E  EQ	R7	R6	K11
      0x781E0002,  //  001F  JMPF	R7	#0023
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x80040E00,  //  0021  RET	1	R7
      0x7002003C,  //  0022  JMP		#0060
      0x1C1C0D0C,  //  0023  EQ	R7	R6	K12
      0x781E000D,  //  0024  JMPF	R7	#0033
      0x8C1C0506,  //  0025  GETMET	R7	R2	K6
      0x58240005,  //  0026  LDCONST	R9	K5
      0x7C1C0400,  //  0027  CALL	R7	2
      0x8C20010D,  //  0028  GETMET	R8	R0	K13
      0x5C280E00,  //  0029  MOVE	R10	R7
      0x7C200400,  //  002A  CALL	R8	2
      0x60200008,  //  002B  GETGBL	R8	G8
      0x5C240E00,  //  002C  MOVE	R9	R7
      0x7C200200,  //  002D  CALL	R8	1
      0x00221C08,  //  002E  ADD	R8	K14	R8
      0x900E1008,  //  002F  SETMBR	R3	K8	R8
      0x50200200,  //  0030  LDBOOL	R8	1	0
      0x80041000,  //  0031  RET	1	R8
      0x7002002C,  //  0032  JMP		#0060
      0x541E0003,  //  0033  LDINT	R7	4
      0x1C1C0C07,  //  0034  EQ	R7	R6	R7
      0x781E0002,  //  0035  JMPF	R7	#0039
      0x501C0200,  //  0036  LDBOOL	R7	1	0
      0x80040E00,  //  0037  RET	1	R7
      0x70020026,  //  0038  JMP		#0060
      0x541E0004,  //  0039  LDINT	R7	5
      0x1C1C0C07,  //  003A  EQ	R7	R6	R7
      0x781E0002,  //  003B  JMPF	R7	#003F
      0x501C0200,  //  003C  LDBOOL	R7	1	0
      0x80040E00,  //  003D  RET	1	R7
      0x70020020,  //  003E  JMP		#0060
      0x541E0005,  //  003F  LDINT	R7	6
      0x1C1C0C07,  //  0040  EQ	R7	R6	R7
      0x781E0018,  //  0041  JMPF	R7	#005B
      0x8C1C0506,  //  0042  GETMET	R7	R2	K6
      0x58240005,  //  0043  LDCONST	R9	K5
      0x7C1C0400,  //  0044  CALL	R7	2
      0x8C200506,  //  0045  GETMET	R8	R2	K6
      0x5828000A,  //  0046  LDCONST	R10	K10
      0x7C200400,  //  0047  CALL	R8	2
      0x8C240107,  //  0048  GETMET	R9	R0	K7
      0x5C2C0E00,  //  0049  MOVE	R11	R7
      0x7C240400,  //  004A  CALL	R9	2
      0x8C24010D,  //  004B  GETMET	R9	R0	K13
      0x5C2C1000,  //  004C  MOVE	R11	R8
      0x7C240400,  //  004D  CALL	R9	2
      0x60240008,  //  004E  GETGBL	R9	G8
      0x5C280E00,  //  004F  MOVE	R10	R7
      0x7C240200,  //  0050  CALL	R9	1
      0x00261209,  //  0051  ADD	R9	K9	R9
      0x0024130F,  //  0052  ADD	R9	R9	K15
      0x60280008,  //  0053  GETGBL	R10	G8
      0x5C2C1000,  //  0054  MOVE	R11	R8
      0x7C280200,  //  0055  CALL	R10	1
      0x0024120A,  //  0056  ADD	R9	R9	R10
      0x900E1009,  //  0057  SETMBR	R3	K8	R9
      0x50240200,  //  0058  LDBOOL	R9	1	0
      0x80041200,  //  0059  RET	1	R9
      0x70020004,  //  005A  JMP		#0060
      0x541E0046,  //  005B  LDINT	R7	71
      0x1C1C0C07,  //  005C  EQ	R7	R6	R7
      0x781E0001,  //  005D  JMPF	R7	#0060
      0x501C0200,  //  005E  LDBOOL	R7	1	0
      0x80040E00,  //  005F  RET	1	R7
      0x70020008,  //  0060  JMP		#006A
      0x601C0003,  //  0061  GETGBL	R7	G3
      0x5C200000,  //  0062  MOVE	R8	R0
      0x7C1C0200,  //  0063  CALL	R7	1
      0x8C1C0F10,  //  0064  GETMET	R7	R7	K16
      0x5C240200,  //  0065  MOVE	R9	R1
      0x5C280400,  //  0066  MOVE	R10	R2
      0x5C2C0600,  //  0067  MOVE	R11	R3
      0x7C1C0800,  //  0068  CALL	R7	4
      0x80040E00,  //  0069  RET	1	R7
      0x80000000,  //  006A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_read_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
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
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_hue),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(U1),
    /* K9   */  be_nested_str_weak(NULL),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(shadow_sat),
    /* K12  */  be_nested_str_weak(U4),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[126]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A02FF,  //  0004  LDINT	R6	768
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A006D,  //  0006  JMPF	R6	#0075
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
      0x70020058,  //  001A  JMP		#0074
      0x1C180B0A,  //  001B  EQ	R6	R5	K10
      0x781A000F,  //  001C  JMPF	R6	#002D
      0x8818010B,  //  001D  GETMBR	R6	R0	K11
      0x4C1C0000,  //  001E  LDNIL	R7
      0x20180C07,  //  001F  NE	R6	R6	R7
      0x781A0005,  //  0020  JMPF	R6	#0027
      0x8C180707,  //  0021  GETMET	R6	R3	K7
      0x88200708,  //  0022  GETMBR	R8	R3	K8
      0x8824010B,  //  0023  GETMBR	R9	R0	K11
      0x7C180600,  //  0024  CALL	R6	3
      0x80040C00,  //  0025  RET	1	R6
      0x70020004,  //  0026  JMP		#002C
      0x8C180707,  //  0027  GETMET	R6	R3	K7
      0x88200709,  //  0028  GETMBR	R8	R3	K9
      0x4C240000,  //  0029  LDNIL	R9
      0x7C180600,  //  002A  CALL	R6	3
      0x80040C00,  //  002B  RET	1	R6
      0x70020046,  //  002C  JMP		#0074
      0x541A0006,  //  002D  LDINT	R6	7
      0x1C180A06,  //  002E  EQ	R6	R5	R6
      0x781A0005,  //  002F  JMPF	R6	#0036
      0x8C180707,  //  0030  GETMET	R6	R3	K7
      0x88200708,  //  0031  GETMBR	R8	R3	K8
      0x58240005,  //  0032  LDCONST	R9	K5
      0x7C180600,  //  0033  CALL	R6	3
      0x80040C00,  //  0034  RET	1	R6
      0x7002003D,  //  0035  JMP		#0074
      0x541A0007,  //  0036  LDINT	R6	8
      0x1C180A06,  //  0037  EQ	R6	R5	R6
      0x781A0005,  //  0038  JMPF	R6	#003F
      0x8C180707,  //  0039  GETMET	R6	R3	K7
      0x88200708,  //  003A  GETMBR	R8	R3	K8
      0x58240005,  //  003B  LDCONST	R9	K5
      0x7C180600,  //  003C  CALL	R6	3
      0x80040C00,  //  003D  RET	1	R6
      0x70020034,  //  003E  JMP		#0074
      0x541A000E,  //  003F  LDINT	R6	15
      0x1C180A06,  //  0040  EQ	R6	R5	R6
      0x781A0005,  //  0041  JMPF	R6	#0048
      0x8C180707,  //  0042  GETMET	R6	R3	K7
      0x88200708,  //  0043  GETMBR	R8	R3	K8
      0x58240005,  //  0044  LDCONST	R9	K5
      0x7C180600,  //  0045  CALL	R6	3
      0x80040C00,  //  0046  RET	1	R6
      0x7002002B,  //  0047  JMP		#0074
      0x541A4000,  //  0048  LDINT	R6	16385
      0x1C180A06,  //  0049  EQ	R6	R5	R6
      0x781A0005,  //  004A  JMPF	R6	#0051
      0x8C180707,  //  004B  GETMET	R6	R3	K7
      0x88200708,  //  004C  GETMBR	R8	R3	K8
      0x58240005,  //  004D  LDCONST	R9	K5
      0x7C180600,  //  004E  CALL	R6	3
      0x80040C00,  //  004F  RET	1	R6
      0x70020022,  //  0050  JMP		#0074
      0x541A4009,  //  0051  LDINT	R6	16394
      0x1C180A06,  //  0052  EQ	R6	R5	R6
      0x781A0005,  //  0053  JMPF	R6	#005A
      0x8C180707,  //  0054  GETMET	R6	R3	K7
      0x8820070C,  //  0055  GETMBR	R8	R3	K12
      0x5824000A,  //  0056  LDCONST	R9	K10
      0x7C180600,  //  0057  CALL	R6	3
      0x80040C00,  //  0058  RET	1	R6
      0x70020019,  //  0059  JMP		#0074
      0x541A000F,  //  005A  LDINT	R6	16
      0x1C180A06,  //  005B  EQ	R6	R5	R6
      0x781A0005,  //  005C  JMPF	R6	#0063
      0x8C180707,  //  005D  GETMET	R6	R3	K7
      0x88200708,  //  005E  GETMBR	R8	R3	K8
      0x58240005,  //  005F  LDCONST	R9	K5
      0x7C180600,  //  0060  CALL	R6	3
      0x80040C00,  //  0061  RET	1	R6
      0x70020010,  //  0062  JMP		#0074
      0x541AFFFB,  //  0063  LDINT	R6	65532
      0x1C180A06,  //  0064  EQ	R6	R5	R6
      0x781A0005,  //  0065  JMPF	R6	#006C
      0x8C180707,  //  0066  GETMET	R6	R3	K7
      0x8820070C,  //  0067  GETMBR	R8	R3	K12
      0x5824000A,  //  0068  LDCONST	R9	K10
      0x7C180600,  //  0069  CALL	R6	3
      0x80040C00,  //  006A  RET	1	R6
      0x70020007,  //  006B  JMP		#0074
      0x541AFFFC,  //  006C  LDINT	R6	65533
      0x1C180A06,  //  006D  EQ	R6	R5	R6
      0x781A0004,  //  006E  JMPF	R6	#0074
      0x8C180707,  //  006F  GETMET	R6	R3	K7
      0x8820070C,  //  0070  GETMBR	R8	R3	K12
      0x54260004,  //  0071  LDINT	R9	5
      0x7C180600,  //  0072  CALL	R6	3
      0x80040C00,  //  0073  RET	1	R6
      0x70020007,  //  0074  JMP		#007D
      0x60180003,  //  0075  GETGBL	R6	G3
      0x5C1C0000,  //  0076  MOVE	R7	R0
      0x7C180200,  //  0077  CALL	R6	1
      0x8C180D0D,  //  0078  GETMET	R6	R6	K13
      0x5C200200,  //  0079  MOVE	R8	R1
      0x5C240400,  //  007A  MOVE	R9	R2
      0x7C180600,  //  007B  CALL	R6	3
      0x80040C00,  //  007C  RET	1	R6
      0x80000000,  //  007D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_hue
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_set_hue,   /* name */
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
    /* K4   */  be_nested_str_weak(HSBColor1),
    /* K5   */  be_nested_str_weak(parse_update),
    }),
    be_str_weak(set_hue),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x58140002,  //  0003  LDCONST	R5	K2
      0x541A00FD,  //  0004  LDINT	R6	254
      0x581C0002,  //  0005  LDCONST	R7	K2
      0x54220167,  //  0006  LDINT	R8	360
      0x7C080C00,  //  0007  CALL	R2	6
      0x8C0C0103,  //  0008  GETMET	R3	R0	K3
      0x58140004,  //  0009  LDCONST	R5	K4
      0x5C180400,  //  000A  MOVE	R6	R2
      0x7C0C0600,  //  000B  CALL	R3	3
      0x4C100000,  //  000C  LDNIL	R4
      0x20100604,  //  000D  NE	R4	R3	R4
      0x78120003,  //  000E  JMPF	R4	#0013
      0x8C100105,  //  000F  GETMET	R4	R0	K5
      0x5C180600,  //  0010  MOVE	R6	R3
      0x541E000A,  //  0011  LDINT	R7	11
      0x7C100600,  //  0012  CALL	R4	3
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Light3
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Light1;
be_local_class(Matter_Plugin_Bridge_Light3,
    2,
    &be_class_Matter_Plugin_Bridge_Light1,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_sat, 12), be_const_closure(Matter_Plugin_Bridge_Light3_set_sat_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Bridge_Light3_init_closure) },
        { be_const_key_weak(parse_update, -1), be_const_closure(Matter_Plugin_Bridge_Light3_parse_update_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_light3) },
        { be_const_key_weak(web_values, -1), be_const_closure(Matter_Plugin_Bridge_Light3_web_values_closure) },
        { be_const_key_weak(web_value_RGB, 1), be_const_closure(Matter_Plugin_Bridge_Light3_web_value_RGB_closure) },
        { be_const_key_weak(shadow_sat, 9), be_const_var(1) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Bridge_Light3_invoke_request_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Light_X203_X20RGB) },
        { be_const_key_weak(TYPES, 11), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(269, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(shadow_hue, 6), be_const_var(0) },
        { be_const_key_weak(set_hue, -1), be_const_closure(Matter_Plugin_Bridge_Light3_set_hue_closure) },
        { be_const_key_weak(read_attribute, 13), be_const_closure(Matter_Plugin_Bridge_Light3_read_attribute_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(768, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(7),
        be_const_int(8),
        be_const_int(15),
        be_const_int(16385),
        be_const_int(16394),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Light3)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_Light3_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_Light3);
    be_setglobal(vm, "Matter_Plugin_Bridge_Light3");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
