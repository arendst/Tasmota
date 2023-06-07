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
    /* K8   */  be_nested_str_weak(web_value_RGB),
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
** Solidified function: web_value_RGB
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_web_value_RGB,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(shadow_hue),
    /* K2   */  be_nested_str_weak(shadow_sat),
    /* K3   */  be_nested_str_weak(light_state),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(set_bri),
    /* K6   */  be_nested_str_weak(set_huesat),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(scale_uint),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(format),
    /* K11  */  be_nested_str_weak(_X23_X2502X_X2502X_X2502X),
    /* K12  */  be_nested_str_weak(r),
    /* K13  */  be_nested_str_weak(g),
    /* K14  */  be_nested_str_weak(b),
    /* K15  */  be_nested_str_weak(_X3Ci_X20class_X3D_X22bxm_X22_X20style_X3D_X22_X2D_X2Dcl_X3A_X25s_X22_X3E_X3C_X2Fi_X3E_X25s),
    /* K16  */  be_nested_str_weak(),
    }),
    be_str_weak(web_value_RGB),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A0027,  //  0004  JMPF	R2	#002D
      0x88080102,  //  0005  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0006  LDNIL	R3
      0x20080403,  //  0007  NE	R2	R2	R3
      0x780A0023,  //  0008  JMPF	R2	#002D
      0xB80A0600,  //  0009  GETNGBL	R2	K3
      0x580C0004,  //  000A  LDCONST	R3	K4
      0x7C080200,  //  000B  CALL	R2	1
      0x8C0C0505,  //  000C  GETMET	R3	R2	K5
      0x541600FE,  //  000D  LDINT	R5	255
      0x7C0C0400,  //  000E  CALL	R3	2
      0x8C0C0506,  //  000F  GETMET	R3	R2	K6
      0xB8160E00,  //  0010  GETNGBL	R5	K7
      0x8C140B08,  //  0011  GETMET	R5	R5	K8
      0x881C0101,  //  0012  GETMBR	R7	R0	K1
      0x58200009,  //  0013  LDCONST	R8	K9
      0x542600FD,  //  0014  LDINT	R9	254
      0x58280009,  //  0015  LDCONST	R10	K9
      0x542E0167,  //  0016  LDINT	R11	360
      0x7C140C00,  //  0017  CALL	R5	6
      0xB81A0E00,  //  0018  GETNGBL	R6	K7
      0x8C180D08,  //  0019  GETMET	R6	R6	K8
      0x88200102,  //  001A  GETMBR	R8	R0	K2
      0x58240009,  //  001B  LDCONST	R9	K9
      0x542A00FD,  //  001C  LDINT	R10	254
      0x582C0009,  //  001D  LDCONST	R11	K9
      0x543200FE,  //  001E  LDINT	R12	255
      0x7C180C00,  //  001F  CALL	R6	6
      0x7C0C0600,  //  0020  CALL	R3	3
      0x8C0C030A,  //  0021  GETMET	R3	R1	K10
      0x5814000B,  //  0022  LDCONST	R5	K11
      0x8818050C,  //  0023  GETMBR	R6	R2	K12
      0x881C050D,  //  0024  GETMBR	R7	R2	K13
      0x8820050E,  //  0025  GETMBR	R8	R2	K14
      0x7C0C0A00,  //  0026  CALL	R3	5
      0x8C10030A,  //  0027  GETMET	R4	R1	K10
      0x5818000F,  //  0028  LDCONST	R6	K15
      0x5C1C0600,  //  0029  MOVE	R7	R3
      0x5C200600,  //  002A  MOVE	R8	R3
      0x7C100800,  //  002B  CALL	R4	4
      0x80040800,  //  002C  RET	1	R4
      0x80062000,  //  002D  RET	1	K16
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
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(shadow_hue),
    /* K8   */  be_nested_str_weak(create_TLV),
    /* K9   */  be_nested_str_weak(U1),
    /* K10  */  be_nested_str_weak(NULL),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(shadow_sat),
    /* K13  */  be_nested_str_weak(U4),
    /* K14  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[127]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E02FF,  //  0005  LDINT	R7	768
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E006D,  //  0007  JMPF	R7	#0076
      0x8C1C0105,  //  0008  GETMET	R7	R0	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x1C1C0D06,  //  000A  EQ	R7	R6	K6
      0x781E000F,  //  000B  JMPF	R7	#001C
      0x881C0107,  //  000C  GETMBR	R7	R0	K7
      0x4C200000,  //  000D  LDNIL	R8
      0x201C0E08,  //  000E  NE	R7	R7	R8
      0x781E0005,  //  000F  JMPF	R7	#0016
      0x8C1C0908,  //  0010  GETMET	R7	R4	K8
      0x88240909,  //  0011  GETMBR	R9	R4	K9
      0x88280107,  //  0012  GETMBR	R10	R0	K7
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x70020004,  //  0015  JMP		#001B
      0x8C1C0908,  //  0016  GETMET	R7	R4	K8
      0x8824090A,  //  0017  GETMBR	R9	R4	K10
      0x4C280000,  //  0018  LDNIL	R10
      0x7C1C0600,  //  0019  CALL	R7	3
      0x80040E00,  //  001A  RET	1	R7
      0x70020058,  //  001B  JMP		#0075
      0x1C1C0D0B,  //  001C  EQ	R7	R6	K11
      0x781E000F,  //  001D  JMPF	R7	#002E
      0x881C010C,  //  001E  GETMBR	R7	R0	K12
      0x4C200000,  //  001F  LDNIL	R8
      0x201C0E08,  //  0020  NE	R7	R7	R8
      0x781E0005,  //  0021  JMPF	R7	#0028
      0x8C1C0908,  //  0022  GETMET	R7	R4	K8
      0x88240909,  //  0023  GETMBR	R9	R4	K9
      0x8828010C,  //  0024  GETMBR	R10	R0	K12
      0x7C1C0600,  //  0025  CALL	R7	3
      0x80040E00,  //  0026  RET	1	R7
      0x70020004,  //  0027  JMP		#002D
      0x8C1C0908,  //  0028  GETMET	R7	R4	K8
      0x8824090A,  //  0029  GETMBR	R9	R4	K10
      0x4C280000,  //  002A  LDNIL	R10
      0x7C1C0600,  //  002B  CALL	R7	3
      0x80040E00,  //  002C  RET	1	R7
      0x70020046,  //  002D  JMP		#0075
      0x541E0006,  //  002E  LDINT	R7	7
      0x1C1C0C07,  //  002F  EQ	R7	R6	R7
      0x781E0005,  //  0030  JMPF	R7	#0037
      0x8C1C0908,  //  0031  GETMET	R7	R4	K8
      0x88240909,  //  0032  GETMBR	R9	R4	K9
      0x58280006,  //  0033  LDCONST	R10	K6
      0x7C1C0600,  //  0034  CALL	R7	3
      0x80040E00,  //  0035  RET	1	R7
      0x7002003D,  //  0036  JMP		#0075
      0x541E0007,  //  0037  LDINT	R7	8
      0x1C1C0C07,  //  0038  EQ	R7	R6	R7
      0x781E0005,  //  0039  JMPF	R7	#0040
      0x8C1C0908,  //  003A  GETMET	R7	R4	K8
      0x88240909,  //  003B  GETMBR	R9	R4	K9
      0x58280006,  //  003C  LDCONST	R10	K6
      0x7C1C0600,  //  003D  CALL	R7	3
      0x80040E00,  //  003E  RET	1	R7
      0x70020034,  //  003F  JMP		#0075
      0x541E000E,  //  0040  LDINT	R7	15
      0x1C1C0C07,  //  0041  EQ	R7	R6	R7
      0x781E0005,  //  0042  JMPF	R7	#0049
      0x8C1C0908,  //  0043  GETMET	R7	R4	K8
      0x88240909,  //  0044  GETMBR	R9	R4	K9
      0x58280006,  //  0045  LDCONST	R10	K6
      0x7C1C0600,  //  0046  CALL	R7	3
      0x80040E00,  //  0047  RET	1	R7
      0x7002002B,  //  0048  JMP		#0075
      0x541E4000,  //  0049  LDINT	R7	16385
      0x1C1C0C07,  //  004A  EQ	R7	R6	R7
      0x781E0005,  //  004B  JMPF	R7	#0052
      0x8C1C0908,  //  004C  GETMET	R7	R4	K8
      0x88240909,  //  004D  GETMBR	R9	R4	K9
      0x58280006,  //  004E  LDCONST	R10	K6
      0x7C1C0600,  //  004F  CALL	R7	3
      0x80040E00,  //  0050  RET	1	R7
      0x70020022,  //  0051  JMP		#0075
      0x541E4009,  //  0052  LDINT	R7	16394
      0x1C1C0C07,  //  0053  EQ	R7	R6	R7
      0x781E0005,  //  0054  JMPF	R7	#005B
      0x8C1C0908,  //  0055  GETMET	R7	R4	K8
      0x8824090D,  //  0056  GETMBR	R9	R4	K13
      0x5828000B,  //  0057  LDCONST	R10	K11
      0x7C1C0600,  //  0058  CALL	R7	3
      0x80040E00,  //  0059  RET	1	R7
      0x70020019,  //  005A  JMP		#0075
      0x541E000F,  //  005B  LDINT	R7	16
      0x1C1C0C07,  //  005C  EQ	R7	R6	R7
      0x781E0005,  //  005D  JMPF	R7	#0064
      0x8C1C0908,  //  005E  GETMET	R7	R4	K8
      0x88240909,  //  005F  GETMBR	R9	R4	K9
      0x58280006,  //  0060  LDCONST	R10	K6
      0x7C1C0600,  //  0061  CALL	R7	3
      0x80040E00,  //  0062  RET	1	R7
      0x70020010,  //  0063  JMP		#0075
      0x541EFFFB,  //  0064  LDINT	R7	65532
      0x1C1C0C07,  //  0065  EQ	R7	R6	R7
      0x781E0005,  //  0066  JMPF	R7	#006D
      0x8C1C0908,  //  0067  GETMET	R7	R4	K8
      0x8824090D,  //  0068  GETMBR	R9	R4	K13
      0x5828000B,  //  0069  LDCONST	R10	K11
      0x7C1C0600,  //  006A  CALL	R7	3
      0x80040E00,  //  006B  RET	1	R7
      0x70020007,  //  006C  JMP		#0075
      0x541EFFFC,  //  006D  LDINT	R7	65533
      0x1C1C0C07,  //  006E  EQ	R7	R6	R7
      0x781E0004,  //  006F  JMPF	R7	#0075
      0x8C1C0908,  //  0070  GETMET	R7	R4	K8
      0x8824090D,  //  0071  GETMBR	R9	R4	K13
      0x542A0004,  //  0072  LDINT	R10	5
      0x7C1C0600,  //  0073  CALL	R7	3
      0x80040E00,  //  0074  RET	1	R7
      0x70020007,  //  0075  JMP		#007E
      0x601C0003,  //  0076  GETGBL	R7	G3
      0x5C200000,  //  0077  MOVE	R8	R0
      0x7C1C0200,  //  0078  CALL	R7	1
      0x8C1C0F0E,  //  0079  GETMET	R7	R7	K14
      0x5C240200,  //  007A  MOVE	R9	R1
      0x5C280400,  //  007B  MOVE	R10	R2
      0x7C1C0600,  //  007C  CALL	R7	3
      0x80040E00,  //  007D  RET	1	R7
      0x80000000,  //  007E  RET	0
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
        { be_const_key_weak(NAME, -1), be_nested_str_weak(_X26_X23x1F517_X3B_X20Light_X203_X20RGB) },
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
