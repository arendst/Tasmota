/* Solidification of Matter_Plugin_4_Bridge_Light1.h */
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
    21,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(findsubval),
    /* K6   */  be_nested_str_weak(set_bri),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(bri_X3A),
    /* K9   */  be_nested_str_weak(publish_command),
    /* K10  */  be_nested_str_weak(Bri),
    /* K11  */  be_nested_str_weak(tasmota),
    /* K12  */  be_nested_str_weak(scale_uint),
    /* K13  */  be_nested_str_weak(Dimmer),
    /* K14  */  be_const_int(1),
    /* K15  */  be_const_int(2),
    /* K16  */  be_const_int(3),
    /* K17  */  be_nested_str_weak(set_onoff),
    /* K18  */  be_nested_str_weak(Power),
    /* K19  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[132]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x541E0007,  //  0004  LDINT	R7	8
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0072,  //  0006  JMPF	R7	#007A
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E0021,  //  0008  JMPF	R7	#002B
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
      0xB82E1600,  //  0016  GETNGBL	R11	K11
      0x8C2C170C,  //  0017  GETMET	R11	R11	K12
      0x5C340E00,  //  0018  MOVE	R13	R7
      0x58380004,  //  0019  LDCONST	R14	K4
      0x543E00FD,  //  001A  LDINT	R15	254
      0x58400004,  //  001B  LDCONST	R16	K4
      0x544600FE,  //  001C  LDINT	R17	255
      0x7C2C0C00,  //  001D  CALL	R11	6
      0x5830000D,  //  001E  LDCONST	R12	K13
      0xB8361600,  //  001F  GETNGBL	R13	K11
      0x8C341B0C,  //  0020  GETMET	R13	R13	K12
      0x5C3C0E00,  //  0021  MOVE	R15	R7
      0x58400004,  //  0022  LDCONST	R16	K4
      0x544600FD,  //  0023  LDINT	R17	254
      0x58480004,  //  0024  LDCONST	R18	K4
      0x544E0063,  //  0025  LDINT	R19	100
      0x7C340C00,  //  0026  CALL	R13	6
      0x7C200A00,  //  0027  CALL	R8	5
      0x50200200,  //  0028  LDBOOL	R8	1	0
      0x80041000,  //  0029  RET	1	R8
      0x7002004D,  //  002A  JMP		#0079
      0x1C1C0D0E,  //  002B  EQ	R7	R6	K14
      0x781E0002,  //  002C  JMPF	R7	#0030
      0x501C0200,  //  002D  LDBOOL	R7	1	0
      0x80040E00,  //  002E  RET	1	R7
      0x70020048,  //  002F  JMP		#0079
      0x1C1C0D0F,  //  0030  EQ	R7	R6	K15
      0x781E0002,  //  0031  JMPF	R7	#0035
      0x501C0200,  //  0032  LDBOOL	R7	1	0
      0x80040E00,  //  0033  RET	1	R7
      0x70020043,  //  0034  JMP		#0079
      0x1C1C0D10,  //  0035  EQ	R7	R6	K16
      0x781E0002,  //  0036  JMPF	R7	#003A
      0x501C0200,  //  0037  LDBOOL	R7	1	0
      0x80040E00,  //  0038  RET	1	R7
      0x7002003E,  //  0039  JMP		#0079
      0x541E0003,  //  003A  LDINT	R7	4
      0x1C1C0C07,  //  003B  EQ	R7	R6	R7
      0x781E002A,  //  003C  JMPF	R7	#0068
      0x8C1C0505,  //  003D  GETMET	R7	R2	K5
      0x58240004,  //  003E  LDCONST	R9	K4
      0x7C1C0400,  //  003F  CALL	R7	2
      0x8C200106,  //  0040  GETMET	R8	R0	K6
      0x5C280E00,  //  0041  MOVE	R10	R7
      0x7C200400,  //  0042  CALL	R8	2
      0x24200F04,  //  0043  GT	R8	R7	K4
      0x8C240111,  //  0044  GETMET	R9	R0	K17
      0x5C2C1000,  //  0045  MOVE	R11	R8
      0x7C240400,  //  0046  CALL	R9	2
      0x60240008,  //  0047  GETGBL	R9	G8
      0x5C280E00,  //  0048  MOVE	R10	R7
      0x7C240200,  //  0049  CALL	R9	1
      0x00261009,  //  004A  ADD	R9	K8	R9
      0x900E0E09,  //  004B  SETMBR	R3	K7	R9
      0x8C240109,  //  004C  GETMET	R9	R0	K9
      0x582C000A,  //  004D  LDCONST	R11	K10
      0xB8321600,  //  004E  GETNGBL	R12	K11
      0x8C30190C,  //  004F  GETMET	R12	R12	K12
      0x5C380E00,  //  0050  MOVE	R14	R7
      0x583C0004,  //  0051  LDCONST	R15	K4
      0x544200FD,  //  0052  LDINT	R16	254
      0x58440004,  //  0053  LDCONST	R17	K4
      0x544A00FE,  //  0054  LDINT	R18	255
      0x7C300C00,  //  0055  CALL	R12	6
      0x5834000D,  //  0056  LDCONST	R13	K13
      0xB83A1600,  //  0057  GETNGBL	R14	K11
      0x8C381D0C,  //  0058  GETMET	R14	R14	K12
      0x5C400E00,  //  0059  MOVE	R16	R7
      0x58440004,  //  005A  LDCONST	R17	K4
      0x544A00FD,  //  005B  LDINT	R18	254
      0x584C0004,  //  005C  LDCONST	R19	K4
      0x54520063,  //  005D  LDINT	R20	100
      0x7C380C00,  //  005E  CALL	R14	6
      0x583C0012,  //  005F  LDCONST	R15	K18
      0x78220001,  //  0060  JMPF	R8	#0063
      0x5840000E,  //  0061  LDCONST	R16	K14
      0x70020000,  //  0062  JMP		#0064
      0x58400004,  //  0063  LDCONST	R16	K4
      0x7C240E00,  //  0064  CALL	R9	7
      0x50240200,  //  0065  LDBOOL	R9	1	0
      0x80041200,  //  0066  RET	1	R9
      0x70020010,  //  0067  JMP		#0079
      0x541E0004,  //  0068  LDINT	R7	5
      0x1C1C0C07,  //  0069  EQ	R7	R6	R7
      0x781E0002,  //  006A  JMPF	R7	#006E
      0x501C0200,  //  006B  LDBOOL	R7	1	0
      0x80040E00,  //  006C  RET	1	R7
      0x7002000A,  //  006D  JMP		#0079
      0x541E0005,  //  006E  LDINT	R7	6
      0x1C1C0C07,  //  006F  EQ	R7	R6	R7
      0x781E0002,  //  0070  JMPF	R7	#0074
      0x501C0200,  //  0071  LDBOOL	R7	1	0
      0x80040E00,  //  0072  RET	1	R7
      0x70020004,  //  0073  JMP		#0079
      0x541E0006,  //  0074  LDINT	R7	7
      0x1C1C0C07,  //  0075  EQ	R7	R6	R7
      0x781E0001,  //  0076  JMPF	R7	#0079
      0x501C0200,  //  0077  LDBOOL	R7	1	0
      0x80040E00,  //  0078  RET	1	R7
      0x70020008,  //  0079  JMP		#0083
      0x601C0003,  //  007A  GETGBL	R7	G3
      0x5C200000,  //  007B  MOVE	R8	R0
      0x7C1C0200,  //  007C  CALL	R7	1
      0x8C1C0F13,  //  007D  GETMET	R7	R7	K19
      0x5C240200,  //  007E  MOVE	R9	R1
      0x5C280400,  //  007F  MOVE	R10	R2
      0x5C2C0600,  //  0080  MOVE	R11	R3
      0x7C1C0800,  //  0081  CALL	R7	4
      0x80040E00,  //  0082  RET	1	R7
      0x80000000,  //  0083  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Light1_read_attribute,   /* name */
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
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_bri),
    /* K7   */  be_nested_str_weak(set),
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
    ( &(const binstruction[99]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0007,  //  0004  LDINT	R7	8
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0051,  //  0006  JMPF	R7	#0059
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
      0x7002003C,  //  001A  JMP		#0058
      0x1C1C0D0A,  //  001B  EQ	R7	R6	K10
      0x781E0005,  //  001C  JMPF	R7	#0023
      0x8C1C0707,  //  001D  GETMET	R7	R3	K7
      0x88240908,  //  001E  GETMBR	R9	R4	K8
      0x58280005,  //  001F  LDCONST	R10	K5
      0x7C1C0600,  //  0020  CALL	R7	3
      0x80040E00,  //  0021  RET	1	R7
      0x70020034,  //  0022  JMP		#0058
      0x1C1C0D0B,  //  0023  EQ	R7	R6	K11
      0x781E0005,  //  0024  JMPF	R7	#002B
      0x8C1C0707,  //  0025  GETMET	R7	R3	K7
      0x88240908,  //  0026  GETMBR	R9	R4	K8
      0x542A00FD,  //  0027  LDINT	R10	254
      0x7C1C0600,  //  0028  CALL	R7	3
      0x80040E00,  //  0029  RET	1	R7
      0x7002002C,  //  002A  JMP		#0058
      0x541E000E,  //  002B  LDINT	R7	15
      0x1C1C0C07,  //  002C  EQ	R7	R6	R7
      0x781E0005,  //  002D  JMPF	R7	#0034
      0x8C1C0707,  //  002E  GETMET	R7	R3	K7
      0x88240908,  //  002F  GETMBR	R9	R4	K8
      0x58280005,  //  0030  LDCONST	R10	K5
      0x7C1C0600,  //  0031  CALL	R7	3
      0x80040E00,  //  0032  RET	1	R7
      0x70020023,  //  0033  JMP		#0058
      0x541E0010,  //  0034  LDINT	R7	17
      0x1C1C0C07,  //  0035  EQ	R7	R6	R7
      0x781E000F,  //  0036  JMPF	R7	#0047
      0x881C0106,  //  0037  GETMBR	R7	R0	K6
      0x4C200000,  //  0038  LDNIL	R8
      0x201C0E08,  //  0039  NE	R7	R7	R8
      0x781E0005,  //  003A  JMPF	R7	#0041
      0x8C1C0707,  //  003B  GETMET	R7	R3	K7
      0x88240908,  //  003C  GETMBR	R9	R4	K8
      0x88280106,  //  003D  GETMBR	R10	R0	K6
      0x7C1C0600,  //  003E  CALL	R7	3
      0x80040E00,  //  003F  RET	1	R7
      0x70020004,  //  0040  JMP		#0046
      0x8C1C0707,  //  0041  GETMET	R7	R3	K7
      0x88240909,  //  0042  GETMBR	R9	R4	K9
      0x4C280000,  //  0043  LDNIL	R10
      0x7C1C0600,  //  0044  CALL	R7	3
      0x80040E00,  //  0045  RET	1	R7
      0x70020010,  //  0046  JMP		#0058
      0x541EFFFB,  //  0047  LDINT	R7	65532
      0x1C1C0C07,  //  0048  EQ	R7	R6	R7
      0x781E0005,  //  0049  JMPF	R7	#0050
      0x8C1C0707,  //  004A  GETMET	R7	R3	K7
      0x8824090C,  //  004B  GETMBR	R9	R4	K12
      0x5828000D,  //  004C  LDCONST	R10	K13
      0x7C1C0600,  //  004D  CALL	R7	3
      0x80040E00,  //  004E  RET	1	R7
      0x70020007,  //  004F  JMP		#0058
      0x541EFFFC,  //  0050  LDINT	R7	65533
      0x1C1C0C07,  //  0051  EQ	R7	R6	R7
      0x781E0004,  //  0052  JMPF	R7	#0058
      0x8C1C0707,  //  0053  GETMET	R7	R3	K7
      0x8824090C,  //  0054  GETMBR	R9	R4	K12
      0x542A0004,  //  0055  LDINT	R10	5
      0x7C1C0600,  //  0056  CALL	R7	3
      0x80040E00,  //  0057  RET	1	R7
      0x70020008,  //  0058  JMP		#0062
      0x601C0003,  //  0059  GETGBL	R7	G3
      0x5C200000,  //  005A  MOVE	R8	R0
      0x7C1C0200,  //  005B  CALL	R7	1
      0x8C1C0F0E,  //  005C  GETMET	R7	R7	K14
      0x5C240200,  //  005D  MOVE	R9	R1
      0x5C280400,  //  005E  MOVE	R10	R2
      0x5C2C0600,  //  005F  MOVE	R11	R3
      0x7C1C0800,  //  0060  CALL	R7	4
      0x80040E00,  //  0061  RET	1	R7
      0x80000000,  //  0062  RET	0
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
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_light1) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Bridge_Light1_read_attribute_closure) },
        { be_const_key_weak(TYPES, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(257, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(shadow_bri, -1), be_const_var(0) },
        { be_const_key_weak(CLUSTERS, 5), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(29, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(8, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
    }))    ) } )) },
        { be_const_key_weak(set_bri, -1), be_const_closure(Matter_Plugin_Bridge_Light1_set_bri_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X201_X20Dimmer) },
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
