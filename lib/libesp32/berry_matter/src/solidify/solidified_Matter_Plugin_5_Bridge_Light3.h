/* Solidification of Matter_Plugin_5_Bridge_Light3.h */
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
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_read_attribute,   /* name */
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
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_hue),
    /* K7   */  be_nested_str_weak(set),
    /* K8   */  be_nested_str_weak(U1),
    /* K9   */  be_nested_str_weak(NULL),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(shadow_sat),
    /* K12  */  be_nested_str_weak(U4),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[127]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E006D,  //  0006  JMPF	R7	#0075
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E000F,  //  000A  JMPF	R7	#001B
      0x881C0106,  //  000B  GETMBR	R7	R0	K6
      0x4C200000,  //  000C  LDNIL	R8
      0x201C0E08,  //  000D  NE	R7	R7	R8
      0x781E0005,  //  000E  JMPF	R7	#0015
      0x8C1C0707,  //  000F  GETMET	R7	R3	K7
      0x88240908,  //  0010  GETMBR	R9	R4	K8
      0x88280106,  //  0011  GETMBR	R10	R0	K6
      0x7C1C0600,  //  0012  CALL	R7	3
      0x80040E00,  //  0013  RET	1	R7
      0x70020004,  //  0014  JMP		#001A
      0x8C1C0707,  //  0015  GETMET	R7	R3	K7
      0x88240909,  //  0016  GETMBR	R9	R4	K9
      0x4C280000,  //  0017  LDNIL	R10
      0x7C1C0600,  //  0018  CALL	R7	3
      0x80040E00,  //  0019  RET	1	R7
      0x70020058,  //  001A  JMP		#0074
      0x1C1C0D0A,  //  001B  EQ	R7	R6	K10
      0x781E000F,  //  001C  JMPF	R7	#002D
      0x881C010B,  //  001D  GETMBR	R7	R0	K11
      0x4C200000,  //  001E  LDNIL	R8
      0x201C0E08,  //  001F  NE	R7	R7	R8
      0x781E0005,  //  0020  JMPF	R7	#0027
      0x8C1C0707,  //  0021  GETMET	R7	R3	K7
      0x88240908,  //  0022  GETMBR	R9	R4	K8
      0x8828010B,  //  0023  GETMBR	R10	R0	K11
      0x7C1C0600,  //  0024  CALL	R7	3
      0x80040E00,  //  0025  RET	1	R7
      0x70020004,  //  0026  JMP		#002C
      0x8C1C0707,  //  0027  GETMET	R7	R3	K7
      0x88240909,  //  0028  GETMBR	R9	R4	K9
      0x4C280000,  //  0029  LDNIL	R10
      0x7C1C0600,  //  002A  CALL	R7	3
      0x80040E00,  //  002B  RET	1	R7
      0x70020046,  //  002C  JMP		#0074
      0x541E0006,  //  002D  LDINT	R7	7
      0x1C1C0C07,  //  002E  EQ	R7	R6	R7
      0x781E0005,  //  002F  JMPF	R7	#0036
      0x8C1C0707,  //  0030  GETMET	R7	R3	K7
      0x88240908,  //  0031  GETMBR	R9	R4	K8
      0x58280005,  //  0032  LDCONST	R10	K5
      0x7C1C0600,  //  0033  CALL	R7	3
      0x80040E00,  //  0034  RET	1	R7
      0x7002003D,  //  0035  JMP		#0074
      0x541E0007,  //  0036  LDINT	R7	8
      0x1C1C0C07,  //  0037  EQ	R7	R6	R7
      0x781E0005,  //  0038  JMPF	R7	#003F
      0x8C1C0707,  //  0039  GETMET	R7	R3	K7
      0x88240908,  //  003A  GETMBR	R9	R4	K8
      0x58280005,  //  003B  LDCONST	R10	K5
      0x7C1C0600,  //  003C  CALL	R7	3
      0x80040E00,  //  003D  RET	1	R7
      0x70020034,  //  003E  JMP		#0074
      0x541E000E,  //  003F  LDINT	R7	15
      0x1C1C0C07,  //  0040  EQ	R7	R6	R7
      0x781E0005,  //  0041  JMPF	R7	#0048
      0x8C1C0707,  //  0042  GETMET	R7	R3	K7
      0x88240908,  //  0043  GETMBR	R9	R4	K8
      0x58280005,  //  0044  LDCONST	R10	K5
      0x7C1C0600,  //  0045  CALL	R7	3
      0x80040E00,  //  0046  RET	1	R7
      0x7002002B,  //  0047  JMP		#0074
      0x541E4000,  //  0048  LDINT	R7	16385
      0x1C1C0C07,  //  0049  EQ	R7	R6	R7
      0x781E0005,  //  004A  JMPF	R7	#0051
      0x8C1C0707,  //  004B  GETMET	R7	R3	K7
      0x88240908,  //  004C  GETMBR	R9	R4	K8
      0x58280005,  //  004D  LDCONST	R10	K5
      0x7C1C0600,  //  004E  CALL	R7	3
      0x80040E00,  //  004F  RET	1	R7
      0x70020022,  //  0050  JMP		#0074
      0x541E4009,  //  0051  LDINT	R7	16394
      0x1C1C0C07,  //  0052  EQ	R7	R6	R7
      0x781E0005,  //  0053  JMPF	R7	#005A
      0x8C1C0707,  //  0054  GETMET	R7	R3	K7
      0x8824090C,  //  0055  GETMBR	R9	R4	K12
      0x5828000A,  //  0056  LDCONST	R10	K10
      0x7C1C0600,  //  0057  CALL	R7	3
      0x80040E00,  //  0058  RET	1	R7
      0x70020019,  //  0059  JMP		#0074
      0x541E000F,  //  005A  LDINT	R7	16
      0x1C1C0C07,  //  005B  EQ	R7	R6	R7
      0x781E0005,  //  005C  JMPF	R7	#0063
      0x8C1C0707,  //  005D  GETMET	R7	R3	K7
      0x88240908,  //  005E  GETMBR	R9	R4	K8
      0x58280005,  //  005F  LDCONST	R10	K5
      0x7C1C0600,  //  0060  CALL	R7	3
      0x80040E00,  //  0061  RET	1	R7
      0x70020010,  //  0062  JMP		#0074
      0x541EFFFB,  //  0063  LDINT	R7	65532
      0x1C1C0C07,  //  0064  EQ	R7	R6	R7
      0x781E0005,  //  0065  JMPF	R7	#006C
      0x8C1C0707,  //  0066  GETMET	R7	R3	K7
      0x8824090C,  //  0067  GETMBR	R9	R4	K12
      0x5828000A,  //  0068  LDCONST	R10	K10
      0x7C1C0600,  //  0069  CALL	R7	3
      0x80040E00,  //  006A  RET	1	R7
      0x70020007,  //  006B  JMP		#0074
      0x541EFFFC,  //  006C  LDINT	R7	65533
      0x1C1C0C07,  //  006D  EQ	R7	R6	R7
      0x781E0004,  //  006E  JMPF	R7	#0074
      0x8C1C0707,  //  006F  GETMET	R7	R3	K7
      0x8824090C,  //  0070  GETMBR	R9	R4	K12
      0x542A0004,  //  0071  LDINT	R10	5
      0x7C1C0600,  //  0072  CALL	R7	3
      0x80040E00,  //  0073  RET	1	R7
      0x70020008,  //  0074  JMP		#007E
      0x601C0003,  //  0075  GETGBL	R7	G3
      0x5C200000,  //  0076  MOVE	R8	R0
      0x7C1C0200,  //  0077  CALL	R7	1
      0x8C1C0F0D,  //  0078  GETMET	R7	R7	K13
      0x5C240200,  //  0079  MOVE	R9	R1
      0x5C280400,  //  007A  MOVE	R10	R2
      0x5C2C0600,  //  007B  MOVE	R11	R3
      0x7C1C0800,  //  007C  CALL	R7	4
      0x80040E00,  //  007D  RET	1	R7
      0x80000000,  //  007E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light3_invoke_request,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(findsubval),
    /* K6   */  be_nested_str_weak(set_hue),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(hue_X3A),
    /* K9   */  be_nested_str_weak(publish_command),
    /* K10  */  be_nested_str_weak(Hue),
    /* K11  */  be_const_int(1),
    /* K12  */  be_const_int(2),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(set_sat),
    /* K15  */  be_nested_str_weak(sat_X3A),
    /* K16  */  be_nested_str_weak(Sat),
    /* K17  */  be_nested_str_weak(_X20sat_X3A),
    /* K18  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[119]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0065,  //  0006  JMPF	R7	#006D
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E0011,  //  0008  JMPF	R7	#001B
      0x8C1C0505,  //  0009  GETMET	R7	R2	K5
      0x58240004,  //  000A  LDCONST	R9	K4
      0x7C1C0400,  //  000B  CALL	R7	2
      0x8C200106,  //  000C  GETMET	R8	R0	K6
      0x5C280E00,  //  000D  MOVE	R10	R7
      0x7C200400,  //  000E  CALL	R8	2
      0x60200008,  //  000F  GETGBL	R8	G8
      0x5C240E00,  //  0010  MOVE	R9	R7
      0x7C200200,  //  0011  CALL	R8	1
      0x00221008,  //  0012  ADD	R8	K8	R8
      0x900E0E08,  //  0013  SETMBR	R3	K7	R8
      0x8C200109,  //  0014  GETMET	R8	R0	K9
      0x5828000A,  //  0015  LDCONST	R10	K10
      0x5C2C0E00,  //  0016  MOVE	R11	R7
      0x7C200600,  //  0017  CALL	R8	3
      0x50200200,  //  0018  LDBOOL	R8	1	0
      0x80041000,  //  0019  RET	1	R8
      0x70020050,  //  001A  JMP		#006C
      0x1C1C0D0B,  //  001B  EQ	R7	R6	K11
      0x781E0002,  //  001C  JMPF	R7	#0020
      0x501C0200,  //  001D  LDBOOL	R7	1	0
      0x80040E00,  //  001E  RET	1	R7
      0x7002004B,  //  001F  JMP		#006C
      0x1C1C0D0C,  //  0020  EQ	R7	R6	K12
      0x781E0002,  //  0021  JMPF	R7	#0025
      0x501C0200,  //  0022  LDBOOL	R7	1	0
      0x80040E00,  //  0023  RET	1	R7
      0x70020046,  //  0024  JMP		#006C
      0x1C1C0D0D,  //  0025  EQ	R7	R6	K13
      0x781E0011,  //  0026  JMPF	R7	#0039
      0x8C1C0505,  //  0027  GETMET	R7	R2	K5
      0x58240004,  //  0028  LDCONST	R9	K4
      0x7C1C0400,  //  0029  CALL	R7	2
      0x8C20010E,  //  002A  GETMET	R8	R0	K14
      0x5C280E00,  //  002B  MOVE	R10	R7
      0x7C200400,  //  002C  CALL	R8	2
      0x60200008,  //  002D  GETGBL	R8	G8
      0x5C240E00,  //  002E  MOVE	R9	R7
      0x7C200200,  //  002F  CALL	R8	1
      0x00221E08,  //  0030  ADD	R8	K15	R8
      0x900E0E08,  //  0031  SETMBR	R3	K7	R8
      0x8C200109,  //  0032  GETMET	R8	R0	K9
      0x58280010,  //  0033  LDCONST	R10	K16
      0x5C2C0E00,  //  0034  MOVE	R11	R7
      0x7C200600,  //  0035  CALL	R8	3
      0x50200200,  //  0036  LDBOOL	R8	1	0
      0x80041000,  //  0037  RET	1	R8
      0x70020032,  //  0038  JMP		#006C
      0x541E0003,  //  0039  LDINT	R7	4
      0x1C1C0C07,  //  003A  EQ	R7	R6	R7
      0x781E0002,  //  003B  JMPF	R7	#003F
      0x501C0200,  //  003C  LDBOOL	R7	1	0
      0x80040E00,  //  003D  RET	1	R7
      0x7002002C,  //  003E  JMP		#006C
      0x541E0004,  //  003F  LDINT	R7	5
      0x1C1C0C07,  //  0040  EQ	R7	R6	R7
      0x781E0002,  //  0041  JMPF	R7	#0045
      0x501C0200,  //  0042  LDBOOL	R7	1	0
      0x80040E00,  //  0043  RET	1	R7
      0x70020026,  //  0044  JMP		#006C
      0x541E0005,  //  0045  LDINT	R7	6
      0x1C1C0C07,  //  0046  EQ	R7	R6	R7
      0x781E001E,  //  0047  JMPF	R7	#0067
      0x8C1C0505,  //  0048  GETMET	R7	R2	K5
      0x58240004,  //  0049  LDCONST	R9	K4
      0x7C1C0400,  //  004A  CALL	R7	2
      0x8C200505,  //  004B  GETMET	R8	R2	K5
      0x5828000B,  //  004C  LDCONST	R10	K11
      0x7C200400,  //  004D  CALL	R8	2
      0x8C240106,  //  004E  GETMET	R9	R0	K6
      0x5C2C0E00,  //  004F  MOVE	R11	R7
      0x7C240400,  //  0050  CALL	R9	2
      0x8C24010E,  //  0051  GETMET	R9	R0	K14
      0x5C2C1000,  //  0052  MOVE	R11	R8
      0x7C240400,  //  0053  CALL	R9	2
      0x60240008,  //  0054  GETGBL	R9	G8
      0x5C280E00,  //  0055  MOVE	R10	R7
      0x7C240200,  //  0056  CALL	R9	1
      0x00261009,  //  0057  ADD	R9	K8	R9
      0x00241311,  //  0058  ADD	R9	R9	K17
      0x60280008,  //  0059  GETGBL	R10	G8
      0x5C2C1000,  //  005A  MOVE	R11	R8
      0x7C280200,  //  005B  CALL	R10	1
      0x0024120A,  //  005C  ADD	R9	R9	R10
      0x900E0E09,  //  005D  SETMBR	R3	K7	R9
      0x8C240109,  //  005E  GETMET	R9	R0	K9
      0x582C000A,  //  005F  LDCONST	R11	K10
      0x5C300E00,  //  0060  MOVE	R12	R7
      0x58340010,  //  0061  LDCONST	R13	K16
      0x5C381000,  //  0062  MOVE	R14	R8
      0x7C240A00,  //  0063  CALL	R9	5
      0x50240200,  //  0064  LDBOOL	R9	1	0
      0x80041200,  //  0065  RET	1	R9
      0x70020004,  //  0066  JMP		#006C
      0x541E0046,  //  0067  LDINT	R7	71
      0x1C1C0C07,  //  0068  EQ	R7	R6	R7
      0x781E0001,  //  0069  JMPF	R7	#006C
      0x501C0200,  //  006A  LDBOOL	R7	1	0
      0x80040E00,  //  006B  RET	1	R7
      0x70020008,  //  006C  JMP		#0076
      0x601C0003,  //  006D  GETGBL	R7	G3
      0x5C200000,  //  006E  MOVE	R8	R0
      0x7C1C0200,  //  006F  CALL	R7	1
      0x8C1C0F12,  //  0070  GETMET	R7	R7	K18
      0x5C240200,  //  0071  MOVE	R9	R1
      0x5C280400,  //  0072  MOVE	R10	R2
      0x5C2C0600,  //  0073  MOVE	R11	R3
      0x7C1C0800,  //  0074  CALL	R7	4
      0x80040E00,  //  0075  RET	1	R7
      0x80000000,  //  0076  RET	0
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
        { be_const_key_weak(set_sat, 6), be_const_closure(Matter_Plugin_Bridge_Light3_set_sat_closure) },
        { be_const_key_weak(init, 8), be_const_closure(Matter_Plugin_Bridge_Light3_init_closure) },
        { be_const_key_weak(parse_update, -1), be_const_closure(Matter_Plugin_Bridge_Light3_parse_update_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_light3) },
        { be_const_key_weak(web_values, -1), be_const_closure(Matter_Plugin_Bridge_Light3_web_values_closure) },
        { be_const_key_weak(web_value_RGB, 1), be_const_closure(Matter_Plugin_Bridge_Light3_web_value_RGB_closure) },
        { be_const_key_weak(CLUSTERS, 13), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Bridge_Light3_invoke_request_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X203_X20RGB) },
        { be_const_key_weak(set_hue, 11), be_const_closure(Matter_Plugin_Bridge_Light3_set_hue_closure) },
        { be_const_key_weak(shadow_hue, 9), be_const_var(0) },
        { be_const_key_weak(TYPES, 12), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(269, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(shadow_sat, -1), be_const_var(1) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Bridge_Light3_read_attribute_closure) },
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
