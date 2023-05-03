/* Solidification of Matter_Plugin_Shutter.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Shutter;

/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_Shutter__X3Clambda_X3E,   /* name */
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
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(U1),
    /* K9   */  be_nested_str_weak(U2),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(shadow_shutter_pos),
    /* K12  */  be_nested_str_weak(shadow_shutter_direction),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(shadow_shutter_target),
    /* K15  */  be_nested_str_weak(U4),
    /* K16  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[129]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0101,  //  0005  LDINT	R7	258
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E006F,  //  0007  JMPF	R7	#0078
      0x8C1C0105,  //  0008  GETMET	R7	R0	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x1C1C0D06,  //  000A  EQ	R7	R6	K6
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0907,  //  000C  GETMET	R7	R4	K7
      0x88240908,  //  000D  GETMBR	R9	R4	K8
      0x542A00FE,  //  000E  LDINT	R10	255
      0x7C1C0600,  //  000F  CALL	R7	3
      0x80040E00,  //  0010  RET	1	R7
      0x70020064,  //  0011  JMP		#0077
      0x541E0004,  //  0012  LDINT	R7	5
      0x1C1C0C07,  //  0013  EQ	R7	R6	R7
      0x781E0005,  //  0014  JMPF	R7	#001B
      0x8C1C0907,  //  0015  GETMET	R7	R4	K7
      0x88240909,  //  0016  GETMBR	R9	R4	K9
      0x58280006,  //  0017  LDCONST	R10	K6
      0x7C1C0600,  //  0018  CALL	R7	3
      0x80040E00,  //  0019  RET	1	R7
      0x7002005B,  //  001A  JMP		#0077
      0x541E0006,  //  001B  LDINT	R7	7
      0x1C1C0C07,  //  001C  EQ	R7	R6	R7
      0x781E0006,  //  001D  JMPF	R7	#0025
      0x8C1C0907,  //  001E  GETMET	R7	R4	K7
      0x88240908,  //  001F  GETMBR	R9	R4	K8
      0x542A0007,  //  0020  LDINT	R10	8
      0x002A140A,  //  0021  ADD	R10	K10	R10
      0x7C1C0600,  //  0022  CALL	R7	3
      0x80040E00,  //  0023  RET	1	R7
      0x70020051,  //  0024  JMP		#0077
      0x541E000C,  //  0025  LDINT	R7	13
      0x1C1C0C07,  //  0026  EQ	R7	R6	R7
      0x781E0005,  //  0027  JMPF	R7	#002E
      0x8C1C0907,  //  0028  GETMET	R7	R4	K7
      0x88240908,  //  0029  GETMBR	R9	R4	K8
      0x542A00FE,  //  002A  LDINT	R10	255
      0x7C1C0600,  //  002B  CALL	R7	3
      0x80040E00,  //  002C  RET	1	R7
      0x70020048,  //  002D  JMP		#0077
      0x541E000D,  //  002E  LDINT	R7	14
      0x1C1C0C07,  //  002F  EQ	R7	R6	R7
      0x781E0009,  //  0030  JMPF	R7	#003B
      0x8C1C0907,  //  0031  GETMET	R7	R4	K7
      0x88240909,  //  0032  GETMBR	R9	R4	K9
      0x542A0063,  //  0033  LDINT	R10	100
      0x882C010B,  //  0034  GETMBR	R11	R0	K11
      0x0428140B,  //  0035  SUB	R10	R10	R11
      0x542E0063,  //  0036  LDINT	R11	100
      0x0828140B,  //  0037  MUL	R10	R10	R11
      0x7C1C0600,  //  0038  CALL	R7	3
      0x80040E00,  //  0039  RET	1	R7
      0x7002003B,  //  003A  JMP		#0077
      0x541E0009,  //  003B  LDINT	R7	10
      0x1C1C0C07,  //  003C  EQ	R7	R6	R7
      0x781E0010,  //  003D  JMPF	R7	#004F
      0x881C010C,  //  003E  GETMBR	R7	R0	K12
      0x1C1C0F06,  //  003F  EQ	R7	R7	K6
      0x781E0001,  //  0040  JMPF	R7	#0043
      0x581C0006,  //  0041  LDCONST	R7	K6
      0x70020005,  //  0042  JMP		#0049
      0x881C010C,  //  0043  GETMBR	R7	R0	K12
      0x241C0F06,  //  0044  GT	R7	R7	K6
      0x781E0001,  //  0045  JMPF	R7	#0048
      0x581C000A,  //  0046  LDCONST	R7	K10
      0x70020000,  //  0047  JMP		#0049
      0x581C000D,  //  0048  LDCONST	R7	K13
      0x8C200907,  //  0049  GETMET	R8	R4	K7
      0x88280908,  //  004A  GETMBR	R10	R4	K8
      0x5C2C0E00,  //  004B  MOVE	R11	R7
      0x7C200600,  //  004C  CALL	R8	3
      0x80041000,  //  004D  RET	1	R8
      0x70020027,  //  004E  JMP		#0077
      0x541E000A,  //  004F  LDINT	R7	11
      0x1C1C0C07,  //  0050  EQ	R7	R6	R7
      0x781E0009,  //  0051  JMPF	R7	#005C
      0x8C1C0907,  //  0052  GETMET	R7	R4	K7
      0x88240909,  //  0053  GETMBR	R9	R4	K9
      0x542A0063,  //  0054  LDINT	R10	100
      0x882C010E,  //  0055  GETMBR	R11	R0	K14
      0x0428140B,  //  0056  SUB	R10	R10	R11
      0x542E0063,  //  0057  LDINT	R11	100
      0x0828140B,  //  0058  MUL	R10	R10	R11
      0x7C1C0600,  //  0059  CALL	R7	3
      0x80040E00,  //  005A  RET	1	R7
      0x7002001A,  //  005B  JMP		#0077
      0x541E0016,  //  005C  LDINT	R7	23
      0x1C1C0C07,  //  005D  EQ	R7	R6	R7
      0x781E0005,  //  005E  JMPF	R7	#0065
      0x8C1C0907,  //  005F  GETMET	R7	R4	K7
      0x88240908,  //  0060  GETMBR	R9	R4	K8
      0x58280006,  //  0061  LDCONST	R10	K6
      0x7C1C0600,  //  0062  CALL	R7	3
      0x80040E00,  //  0063  RET	1	R7
      0x70020011,  //  0064  JMP		#0077
      0x541EFFFB,  //  0065  LDINT	R7	65532
      0x1C1C0C07,  //  0066  EQ	R7	R6	R7
      0x781E0006,  //  0067  JMPF	R7	#006F
      0x8C1C0907,  //  0068  GETMET	R7	R4	K7
      0x8824090F,  //  0069  GETMBR	R9	R4	K15
      0x542A0003,  //  006A  LDINT	R10	4
      0x002A140A,  //  006B  ADD	R10	K10	R10
      0x7C1C0600,  //  006C  CALL	R7	3
      0x80040E00,  //  006D  RET	1	R7
      0x70020007,  //  006E  JMP		#0077
      0x541EFFFC,  //  006F  LDINT	R7	65533
      0x1C1C0C07,  //  0070  EQ	R7	R6	R7
      0x781E0004,  //  0071  JMPF	R7	#0077
      0x8C1C0907,  //  0072  GETMET	R7	R4	K7
      0x8824090F,  //  0073  GETMBR	R9	R4	K15
      0x542A0004,  //  0074  LDINT	R10	5
      0x7C1C0600,  //  0075  CALL	R7	3
      0x80040E00,  //  0076  RET	1	R7
      0x70020007,  //  0077  JMP		#0080
      0x601C0003,  //  0078  GETGBL	R7	G3
      0x5C200000,  //  0079  MOVE	R8	R0
      0x7C1C0200,  //  007A  CALL	R7	1
      0x8C1C0F10,  //  007B  GETMET	R7	R7	K16
      0x5C240200,  //  007C  MOVE	R9	R1
      0x5C280400,  //  007D  MOVE	R10	R2
      0x7C1C0600,  //  007E  CALL	R7	3
      0x80040E00,  //  007F  RET	1	R7
      0x80000000,  //  0080  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(tasmota_shutter_index),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(ARG),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x8C100702,  //  0008  GETMET	R4	R3	K2
      0x88180103,  //  0009  GETMBR	R6	R0	K3
      0x7C100400,  //  000A  CALL	R4	2
      0x90020204,  //  000B  SETMBR	R0	K1	R4
      0x88100101,  //  000C  GETMBR	R4	R0	K1
      0x4C140000,  //  000D  LDNIL	R5
      0x1C100805,  //  000E  EQ	R4	R4	R5
      0x78120000,  //  000F  JMPF	R4	#0011
      0x90020304,  //  0010  SETMBR	R0	K1	K4
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_update_shadow,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(cmd),
    /* K2   */  be_nested_str_weak(ShutterPosition),
    /* K3   */  be_nested_str_weak(tasmota_shutter_index),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(parse_sensors),
    /* K6   */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x600C0008,  //  0002  GETGBL	R3	G8
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x00100904,  //  0004  ADD	R4	R4	K4
      0x7C0C0200,  //  0005  CALL	R3	1
      0x000E0403,  //  0006  ADD	R3	K2	R3
      0x50100200,  //  0007  LDBOOL	R4	1	0
      0x7C040600,  //  0008  CALL	R1	3
      0x78060002,  //  0009  JMPF	R1	#000D
      0x8C080105,  //  000A  GETMET	R2	R0	K5
      0x5C100200,  //  000B  MOVE	R4	R1
      0x7C080400,  //  000C  CALL	R2	2
      0x60080003,  //  000D  GETGBL	R2	G3
      0x5C0C0000,  //  000E  MOVE	R3	R0
      0x7C080200,  //  000F  CALL	R2	1
      0x8C080506,  //  0010  GETMET	R2	R2	K6
      0x7C080200,  //  0011  CALL	R2	1
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_invoke_request,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(cmd),
    /* K9   */  be_nested_str_weak(ShutterStopOpen),
    /* K10  */  be_nested_str_weak(tasmota_shutter_index),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(update_shadow),
    /* K13  */  be_nested_str_weak(ShutterStopClose),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(ShutterStop),
    /* K16  */  be_nested_str_weak(log),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20Tilt_X20_X3D_X20),
    /* K18  */  be_nested_str_weak(findsubval),
    /* K19  */  be_nested_str_weak(ShutterStopPosition),
    /* K20  */  be_nested_str_weak(_X20),
    /* K21  */  be_nested_str_weak(pos_X25_X3A),
    /* K22  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[112]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220101,  //  0005  LDINT	R8	258
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x7822005D,  //  0007  JMPF	R8	#0066
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x7822000D,  //  000B  JMPF	R8	#001A
      0xB8220E00,  //  000C  GETNGBL	R8	K7
      0x8C201108,  //  000D  GETMET	R8	R8	K8
      0x60280008,  //  000E  GETGBL	R10	G8
      0x882C010A,  //  000F  GETMBR	R11	R0	K10
      0x002C170B,  //  0010  ADD	R11	R11	K11
      0x7C280200,  //  0011  CALL	R10	1
      0x002A120A,  //  0012  ADD	R10	K9	R10
      0x502C0200,  //  0013  LDBOOL	R11	1	0
      0x7C200600,  //  0014  CALL	R8	3
      0x8C20010C,  //  0015  GETMET	R8	R0	K12
      0x7C200200,  //  0016  CALL	R8	1
      0x50200200,  //  0017  LDBOOL	R8	1	0
      0x80041000,  //  0018  RET	1	R8
      0x7002004A,  //  0019  JMP		#0065
      0x1C200F0B,  //  001A  EQ	R8	R7	K11
      0x7822000D,  //  001B  JMPF	R8	#002A
      0xB8220E00,  //  001C  GETNGBL	R8	K7
      0x8C201108,  //  001D  GETMET	R8	R8	K8
      0x60280008,  //  001E  GETGBL	R10	G8
      0x882C010A,  //  001F  GETMBR	R11	R0	K10
      0x002C170B,  //  0020  ADD	R11	R11	K11
      0x7C280200,  //  0021  CALL	R10	1
      0x002A1A0A,  //  0022  ADD	R10	K13	R10
      0x502C0200,  //  0023  LDBOOL	R11	1	0
      0x7C200600,  //  0024  CALL	R8	3
      0x8C20010C,  //  0025  GETMET	R8	R0	K12
      0x7C200200,  //  0026  CALL	R8	1
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x7002003A,  //  0029  JMP		#0065
      0x1C200F0E,  //  002A  EQ	R8	R7	K14
      0x7822000D,  //  002B  JMPF	R8	#003A
      0xB8220E00,  //  002C  GETNGBL	R8	K7
      0x8C201108,  //  002D  GETMET	R8	R8	K8
      0x60280008,  //  002E  GETGBL	R10	G8
      0x882C010A,  //  002F  GETMBR	R11	R0	K10
      0x002C170B,  //  0030  ADD	R11	R11	K11
      0x7C280200,  //  0031  CALL	R10	1
      0x002A1E0A,  //  0032  ADD	R10	K15	R10
      0x502C0200,  //  0033  LDBOOL	R11	1	0
      0x7C200600,  //  0034  CALL	R8	3
      0x8C20010C,  //  0035  GETMET	R8	R0	K12
      0x7C200200,  //  0036  CALL	R8	1
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x80041000,  //  0038  RET	1	R8
      0x7002002A,  //  0039  JMP		#0065
      0x54220004,  //  003A  LDINT	R8	5
      0x1C200E08,  //  003B  EQ	R8	R7	R8
      0x78220027,  //  003C  JMPF	R8	#0065
      0xB8220E00,  //  003D  GETNGBL	R8	K7
      0x8C201110,  //  003E  GETMET	R8	R8	K16
      0x60280008,  //  003F  GETGBL	R10	G8
      0x5C2C0400,  //  0040  MOVE	R11	R2
      0x7C280200,  //  0041  CALL	R10	1
      0x002A220A,  //  0042  ADD	R10	K17	R10
      0x582C000E,  //  0043  LDCONST	R11	K14
      0x7C200600,  //  0044  CALL	R8	3
      0x8C200512,  //  0045  GETMET	R8	R2	K18
      0x58280006,  //  0046  LDCONST	R10	K6
      0x7C200400,  //  0047  CALL	R8	2
      0x4C240000,  //  0048  LDNIL	R9
      0x20241009,  //  0049  NE	R9	R8	R9
      0x78260017,  //  004A  JMPF	R9	#0063
      0x54260063,  //  004B  LDINT	R9	100
      0x0C201009,  //  004C  DIV	R8	R8	R9
      0xB8260E00,  //  004D  GETNGBL	R9	K7
      0x8C241308,  //  004E  GETMET	R9	R9	K8
      0x602C0008,  //  004F  GETGBL	R11	G8
      0x8830010A,  //  0050  GETMBR	R12	R0	K10
      0x0030190B,  //  0051  ADD	R12	R12	K11
      0x7C2C0200,  //  0052  CALL	R11	1
      0x002E260B,  //  0053  ADD	R11	K19	R11
      0x002C1714,  //  0054  ADD	R11	R11	K20
      0x60300008,  //  0055  GETGBL	R12	G8
      0x54360063,  //  0056  LDINT	R13	100
      0x04341A08,  //  0057  SUB	R13	R13	R8
      0x7C300200,  //  0058  CALL	R12	1
      0x002C160C,  //  0059  ADD	R11	R11	R12
      0x50300200,  //  005A  LDBOOL	R12	1	0
      0x7C240600,  //  005B  CALL	R9	3
      0x60240008,  //  005C  GETGBL	R9	G8
      0x5C281000,  //  005D  MOVE	R10	R8
      0x7C240200,  //  005E  CALL	R9	1
      0x00262A09,  //  005F  ADD	R9	K21	R9
      0x900E2009,  //  0060  SETMBR	R3	K16	R9
      0x8C24010C,  //  0061  GETMET	R9	R0	K12
      0x7C240200,  //  0062  CALL	R9	1
      0x50240200,  //  0063  LDBOOL	R9	1	0
      0x80041200,  //  0064  RET	1	R9
      0x70020008,  //  0065  JMP		#006F
      0x60200003,  //  0066  GETGBL	R8	G3
      0x5C240000,  //  0067  MOVE	R9	R0
      0x7C200200,  //  0068  CALL	R8	1
      0x8C201116,  //  0069  GETMET	R8	R8	K22
      0x5C280200,  //  006A  MOVE	R10	R1
      0x5C2C0400,  //  006B  MOVE	R11	R2
      0x5C300600,  //  006C  MOVE	R12	R3
      0x7C200800,  //  006D  CALL	R8	4
      0x80041000,  //  006E  RET	1	R8
      0x80000000,  //  006F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(Matter_Plugin_Shutter_parse_sensors,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(Shutter),
    /* K2   */  be_nested_str_weak(tasmota_shutter_index),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(contains),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_nested_str_weak(Position),
    /* K7   */  be_nested_str_weak(shadow_shutter_pos),
    /* K8   */  be_nested_str_weak(attribute_updated),
    /* K9   */  be_nested_str_weak(Direction),
    /* K10  */  be_nested_str_weak(shadow_shutter_direction),
    /* K11  */  be_nested_str_weak(Target),
    /* K12  */  be_nested_str_weak(shadow_shutter_target),
    }),
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0008,  //  0001  GETGBL	R3	G8
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x00100903,  //  0003  ADD	R4	R4	K3
      0x7C0C0200,  //  0004  CALL	R3	1
      0x000E0203,  //  0005  ADD	R3	K1	R3
      0x8C100304,  //  0006  GETMET	R4	R1	K4
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0x7812002A,  //  0009  JMPF	R4	#0035
      0x94100203,  //  000A  GETIDX	R4	R1	R3
      0x8C140905,  //  000B  GETMET	R5	R4	K5
      0x581C0006,  //  000C  LDCONST	R7	K6
      0x7C140400,  //  000D  CALL	R5	2
      0x4C180000,  //  000E  LDNIL	R6
      0x20180A06,  //  000F  NE	R6	R5	R6
      0x781A0007,  //  0010  JMPF	R6	#0019
      0x88180107,  //  0011  GETMBR	R6	R0	K7
      0x20180A06,  //  0012  NE	R6	R5	R6
      0x781A0003,  //  0013  JMPF	R6	#0018
      0x8C180108,  //  0014  GETMET	R6	R0	K8
      0x54220101,  //  0015  LDINT	R8	258
      0x5426000D,  //  0016  LDINT	R9	14
      0x7C180600,  //  0017  CALL	R6	3
      0x90020E05,  //  0018  SETMBR	R0	K7	R5
      0x8C180905,  //  0019  GETMET	R6	R4	K5
      0x58200009,  //  001A  LDCONST	R8	K9
      0x7C180400,  //  001B  CALL	R6	2
      0x4C1C0000,  //  001C  LDNIL	R7
      0x201C0C07,  //  001D  NE	R7	R6	R7
      0x781E0007,  //  001E  JMPF	R7	#0027
      0x881C010A,  //  001F  GETMBR	R7	R0	K10
      0x201C0C07,  //  0020  NE	R7	R6	R7
      0x781E0003,  //  0021  JMPF	R7	#0026
      0x8C1C0108,  //  0022  GETMET	R7	R0	K8
      0x54260101,  //  0023  LDINT	R9	258
      0x542A0009,  //  0024  LDINT	R10	10
      0x7C1C0600,  //  0025  CALL	R7	3
      0x90021406,  //  0026  SETMBR	R0	K10	R6
      0x8C1C0905,  //  0027  GETMET	R7	R4	K5
      0x5824000B,  //  0028  LDCONST	R9	K11
      0x7C1C0400,  //  0029  CALL	R7	2
      0x4C200000,  //  002A  LDNIL	R8
      0x20200E08,  //  002B  NE	R8	R7	R8
      0x78220007,  //  002C  JMPF	R8	#0035
      0x8820010C,  //  002D  GETMBR	R8	R0	K12
      0x20200E08,  //  002E  NE	R8	R7	R8
      0x78220003,  //  002F  JMPF	R8	#0034
      0x8C200108,  //  0030  GETMET	R8	R0	K8
      0x542A0101,  //  0031  LDINT	R10	258
      0x542E000A,  //  0032  LDINT	R11	11
      0x7C200600,  //  0033  CALL	R8	3
      0x90021807,  //  0034  SETMBR	R0	K12	R7
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Shutter
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Shutter,
    5,
    &be_class_Matter_Plugin_Device,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(Matter_Plugin_Shutter__X3Clambda_X3E_closure) },
        { be_const_key_weak(shadow_shutter_pos, -1), be_const_var(1) },
        { be_const_key_weak(CLUSTERS, 14), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(258, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(5),
        be_const_int(7),
        be_const_int(10),
        be_const_int(11),
        be_const_int(13),
        be_const_int(14),
        be_const_int(23),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Shutter_init_closure) },
        { be_const_key_weak(shadow_shutter_direction, -1), be_const_var(4) },
        { be_const_key_weak(shadow_shutter_target, 15), be_const_var(2) },
        { be_const_key_weak(NAME, 1), be_nested_str_weak(Shutter) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(shutter) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Shutter_invoke_request_closure) },
        { be_const_key_weak(tasmota_shutter_index, -1), be_const_var(0) },
        { be_const_key_weak(read_attribute, 2), be_const_closure(Matter_Plugin_Shutter_read_attribute_closure) },
        { be_const_key_weak(shadow_shutter_tilt, 8), be_const_var(3) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(Matter_Plugin_Shutter_parse_sensors_closure) },
        { be_const_key_weak(TYPE, 4), be_nested_str_weak(shutter) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(514, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Shutter_update_shadow_closure) },
    })),
    be_str_weak(Matter_Plugin_Shutter)
);
/*******************************************************************/

void be_load_Matter_Plugin_Shutter_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Shutter);
    be_setglobal(vm, "Matter_Plugin_Shutter");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
