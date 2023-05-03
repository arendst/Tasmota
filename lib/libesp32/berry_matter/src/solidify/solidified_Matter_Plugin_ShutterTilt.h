/* Solidification of Matter_Plugin_ShutterTilt.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_ShutterTilt;

/********************************************************************
** Solidified function: update_tilt_min_max
********************************************************************/
be_local_closure(Matter_Plugin_ShutterTilt_update_tilt_min_max,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(cmd),
    /* K2   */  be_nested_str_weak(Status_X2013),
    /* K3   */  be_nested_str_weak(contains),
    /* K4   */  be_nested_str_weak(StatusSHT),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_nested_str_weak(SHT),
    /* K7   */  be_nested_str_weak(tasmota_shutter_index),
    /* K8   */  be_nested_str_weak(TiltConfig),
    /* K9   */  be_nested_str_weak(tilt_min),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(tilt_max),
    /* K12  */  be_const_int(1),
    }),
    be_str_weak(update_tilt_min_max),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x50100200,  //  0003  LDBOOL	R4	1	0
      0x7C040600,  //  0004  CALL	R1	3
      0x8C080303,  //  0005  GETMET	R2	R1	K3
      0x58100004,  //  0006  LDCONST	R4	K4
      0x7C080400,  //  0007  CALL	R2	2
      0x780A0016,  //  0008  JMPF	R2	#0020
      0x94040304,  //  0009  GETIDX	R1	R1	K4
      0x8C080305,  //  000A  GETMET	R2	R1	K5
      0x60100008,  //  000B  GETGBL	R4	G8
      0x88140107,  //  000C  GETMBR	R5	R0	K7
      0x7C100200,  //  000D  CALL	R4	1
      0x00120C04,  //  000E  ADD	R4	K6	R4
      0x60140013,  //  000F  GETGBL	R5	G19
      0x7C140000,  //  0010  CALL	R5	0
      0x7C080600,  //  0011  CALL	R2	3
      0x8C080505,  //  0012  GETMET	R2	R2	K5
      0x58100008,  //  0013  LDCONST	R4	K8
      0x7C080400,  //  0014  CALL	R2	2
      0x4C0C0000,  //  0015  LDNIL	R3
      0x200C0403,  //  0016  NE	R3	R2	R3
      0x780E0007,  //  0017  JMPF	R3	#0020
      0x600C0009,  //  0018  GETGBL	R3	G9
      0x9410050A,  //  0019  GETIDX	R4	R2	K10
      0x7C0C0200,  //  001A  CALL	R3	1
      0x90021203,  //  001B  SETMBR	R0	K9	R3
      0x600C0009,  //  001C  GETGBL	R3	G9
      0x9410050C,  //  001D  GETIDX	R4	R2	K12
      0x7C0C0200,  //  001E  CALL	R3	1
      0x90021603,  //  001F  SETMBR	R0	K11	R3
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_ShutterTilt_invoke_request,   /* name */
  be_nested_proto(
    16,                          /* nstack */
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
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(update_tilt_min_max),
    /* K9   */  be_nested_str_weak(tilt_min),
    /* K10  */  be_nested_str_weak(tilt_max),
    /* K11  */  be_nested_str_weak(tasmota),
    /* K12  */  be_nested_str_weak(scale_uint),
    /* K13  */  be_nested_str_weak(cmd),
    /* K14  */  be_nested_str_weak(ShutterTilt),
    /* K15  */  be_nested_str_weak(tasmota_shutter_index),
    /* K16  */  be_const_int(1),
    /* K17  */  be_nested_str_weak(_X20),
    /* K18  */  be_nested_str_weak(update_shadow),
    /* K19  */  be_nested_str_weak(log),
    /* K20  */  be_nested_str_weak(tilt_X25_X3A),
    /* K21  */  be_nested_str_weak(tilt_X25_X28no_tilt_support_X29_X3A),
    /* K22  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220101,  //  0005  LDINT	R8	258
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220039,  //  0007  JMPF	R8	#0042
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x54220007,  //  000A  LDINT	R8	8
      0x1C200E08,  //  000B  EQ	R8	R7	R8
      0x78220034,  //  000C  JMPF	R8	#0042
      0x8C200506,  //  000D  GETMET	R8	R2	K6
      0x58280007,  //  000E  LDCONST	R10	K7
      0x7C200400,  //  000F  CALL	R8	2
      0x4C240000,  //  0010  LDNIL	R9
      0x20241009,  //  0011  NE	R9	R8	R9
      0x7826002C,  //  0012  JMPF	R9	#0040
      0x8C240108,  //  0013  GETMET	R9	R0	K8
      0x7C240200,  //  0014  CALL	R9	1
      0x88240109,  //  0015  GETMBR	R9	R0	K9
      0x4C280000,  //  0016  LDNIL	R10
      0x2024120A,  //  0017  NE	R9	R9	R10
      0x78260021,  //  0018  JMPF	R9	#003B
      0x8824010A,  //  0019  GETMBR	R9	R0	K10
      0x4C280000,  //  001A  LDNIL	R10
      0x2024120A,  //  001B  NE	R9	R9	R10
      0x7826001D,  //  001C  JMPF	R9	#003B
      0xB8261600,  //  001D  GETNGBL	R9	K11
      0x8C24130C,  //  001E  GETMET	R9	R9	K12
      0x5C2C1000,  //  001F  MOVE	R11	R8
      0x58300007,  //  0020  LDCONST	R12	K7
      0x543603E7,  //  0021  LDINT	R13	1000
      0x88380109,  //  0022  GETMBR	R14	R0	K9
      0x883C010A,  //  0023  GETMBR	R15	R0	K10
      0x7C240C00,  //  0024  CALL	R9	6
      0xB82A1600,  //  0025  GETNGBL	R10	K11
      0x8C28150D,  //  0026  GETMET	R10	R10	K13
      0x60300008,  //  0027  GETGBL	R12	G8
      0x8834010F,  //  0028  GETMBR	R13	R0	K15
      0x00341B10,  //  0029  ADD	R13	R13	K16
      0x7C300200,  //  002A  CALL	R12	1
      0x00321C0C,  //  002B  ADD	R12	K14	R12
      0x00301911,  //  002C  ADD	R12	R12	K17
      0x60340008,  //  002D  GETGBL	R13	G8
      0x5C381200,  //  002E  MOVE	R14	R9
      0x7C340200,  //  002F  CALL	R13	1
      0x0030180D,  //  0030  ADD	R12	R12	R13
      0x50340000,  //  0031  LDBOOL	R13	0	0
      0x7C280600,  //  0032  CALL	R10	3
      0x8C280112,  //  0033  GETMET	R10	R0	K18
      0x7C280200,  //  0034  CALL	R10	1
      0x60280008,  //  0035  GETGBL	R10	G8
      0x5C2C1000,  //  0036  MOVE	R11	R8
      0x7C280200,  //  0037  CALL	R10	1
      0x002A280A,  //  0038  ADD	R10	K20	R10
      0x900E260A,  //  0039  SETMBR	R3	K19	R10
      0x70020004,  //  003A  JMP		#0040
      0x60240008,  //  003B  GETGBL	R9	G8
      0x5C281000,  //  003C  MOVE	R10	R8
      0x7C240200,  //  003D  CALL	R9	1
      0x00262A09,  //  003E  ADD	R9	K21	R9
      0x900E2609,  //  003F  SETMBR	R3	K19	R9
      0x50240200,  //  0040  LDBOOL	R9	1	0
      0x80041200,  //  0041  RET	1	R9
      0x60200003,  //  0042  GETGBL	R8	G3
      0x5C240000,  //  0043  MOVE	R9	R0
      0x7C200200,  //  0044  CALL	R8	1
      0x8C201116,  //  0045  GETMET	R8	R8	K22
      0x5C280200,  //  0046  MOVE	R10	R1
      0x5C2C0400,  //  0047  MOVE	R11	R2
      0x5C300600,  //  0048  MOVE	R12	R3
      0x7C200800,  //  0049  CALL	R8	4
      0x80041000,  //  004A  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_ShutterTilt_read_attribute,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(update_tilt_min_max),
    /* K10  */  be_nested_str_weak(tilt_min),
    /* K11  */  be_nested_str_weak(tilt_max),
    /* K12  */  be_nested_str_weak(tasmota),
    /* K13  */  be_nested_str_weak(scale_uint),
    /* K14  */  be_nested_str_weak(shadow_shutter_tilt),
    /* K15  */  be_const_int(0),
    /* K16  */  be_nested_str_weak(U2),
    /* K17  */  be_nested_str_weak(NULL),
    /* K18  */  be_nested_str_weak(U4),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0101,  //  0005  LDINT	R7	258
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0058,  //  0007  JMPF	R7	#0061
      0x8C1C0105,  //  0008  GETMET	R7	R0	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x541E0006,  //  000A  LDINT	R7	7
      0x1C1C0C07,  //  000B  EQ	R7	R6	R7
      0x781E0008,  //  000C  JMPF	R7	#0016
      0x8C1C0906,  //  000D  GETMET	R7	R4	K6
      0x88240907,  //  000E  GETMBR	R9	R4	K7
      0x542A0007,  //  000F  LDINT	R10	8
      0x002A100A,  //  0010  ADD	R10	K8	R10
      0x542E000F,  //  0011  LDINT	R11	16
      0x0028140B,  //  0012  ADD	R10	R10	R11
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x7002004A,  //  0015  JMP		#0061
      0x541E000E,  //  0016  LDINT	R7	15
      0x1C1C0C07,  //  0017  EQ	R7	R6	R7
      0x781E001D,  //  0018  JMPF	R7	#0037
      0x8C1C0109,  //  0019  GETMET	R7	R0	K9
      0x7C1C0200,  //  001A  CALL	R7	1
      0x881C010A,  //  001B  GETMBR	R7	R0	K10
      0x4C200000,  //  001C  LDNIL	R8
      0x201C0E08,  //  001D  NE	R7	R7	R8
      0x781E0011,  //  001E  JMPF	R7	#0031
      0x881C010B,  //  001F  GETMBR	R7	R0	K11
      0x4C200000,  //  0020  LDNIL	R8
      0x201C0E08,  //  0021  NE	R7	R7	R8
      0x781E000D,  //  0022  JMPF	R7	#0031
      0xB81E1800,  //  0023  GETNGBL	R7	K12
      0x8C1C0F0D,  //  0024  GETMET	R7	R7	K13
      0x8824010E,  //  0025  GETMBR	R9	R0	K14
      0x8828010A,  //  0026  GETMBR	R10	R0	K10
      0x882C010B,  //  0027  GETMBR	R11	R0	K11
      0x5830000F,  //  0028  LDCONST	R12	K15
      0x543603E7,  //  0029  LDINT	R13	1000
      0x7C1C0C00,  //  002A  CALL	R7	6
      0x8C200906,  //  002B  GETMET	R8	R4	K6
      0x88280910,  //  002C  GETMBR	R10	R4	K16
      0x5C2C0E00,  //  002D  MOVE	R11	R7
      0x7C200600,  //  002E  CALL	R8	3
      0x80041000,  //  002F  RET	1	R8
      0x70020004,  //  0030  JMP		#0036
      0x8C1C0906,  //  0031  GETMET	R7	R4	K6
      0x88240911,  //  0032  GETMBR	R9	R4	K17
      0x4C280000,  //  0033  LDNIL	R10
      0x7C1C0600,  //  0034  CALL	R7	3
      0x80040E00,  //  0035  RET	1	R7
      0x70020029,  //  0036  JMP		#0061
      0x541E000B,  //  0037  LDINT	R7	12
      0x1C1C0C07,  //  0038  EQ	R7	R6	R7
      0x781E001B,  //  0039  JMPF	R7	#0056
      0x881C010A,  //  003A  GETMBR	R7	R0	K10
      0x4C200000,  //  003B  LDNIL	R8
      0x201C0E08,  //  003C  NE	R7	R7	R8
      0x781E0011,  //  003D  JMPF	R7	#0050
      0x881C010B,  //  003E  GETMBR	R7	R0	K11
      0x4C200000,  //  003F  LDNIL	R8
      0x201C0E08,  //  0040  NE	R7	R7	R8
      0x781E000D,  //  0041  JMPF	R7	#0050
      0xB81E1800,  //  0042  GETNGBL	R7	K12
      0x8C1C0F0D,  //  0043  GETMET	R7	R7	K13
      0x8824010E,  //  0044  GETMBR	R9	R0	K14
      0x8828010A,  //  0045  GETMBR	R10	R0	K10
      0x882C010B,  //  0046  GETMBR	R11	R0	K11
      0x5830000F,  //  0047  LDCONST	R12	K15
      0x543603E7,  //  0048  LDINT	R13	1000
      0x7C1C0C00,  //  0049  CALL	R7	6
      0x8C200906,  //  004A  GETMET	R8	R4	K6
      0x88280910,  //  004B  GETMBR	R10	R4	K16
      0x5C2C0E00,  //  004C  MOVE	R11	R7
      0x7C200600,  //  004D  CALL	R8	3
      0x80041000,  //  004E  RET	1	R8
      0x70020004,  //  004F  JMP		#0055
      0x8C1C0906,  //  0050  GETMET	R7	R4	K6
      0x88240911,  //  0051  GETMBR	R9	R4	K17
      0x4C280000,  //  0052  LDNIL	R10
      0x7C1C0600,  //  0053  CALL	R7	3
      0x80040E00,  //  0054  RET	1	R7
      0x7002000A,  //  0055  JMP		#0061
      0x541EFFFB,  //  0056  LDINT	R7	65532
      0x1C1C0C07,  //  0057  EQ	R7	R6	R7
      0x781E0007,  //  0058  JMPF	R7	#0061
      0x8C1C0906,  //  0059  GETMET	R7	R4	K6
      0x88240912,  //  005A  GETMBR	R9	R4	K18
      0x542A0003,  //  005B  LDINT	R10	4
      0x002A260A,  //  005C  ADD	R10	K19	R10
      0x542E000F,  //  005D  LDINT	R11	16
      0x0028140B,  //  005E  ADD	R10	R10	R11
      0x7C1C0600,  //  005F  CALL	R7	3
      0x80040E00,  //  0060  RET	1	R7
      0x601C0003,  //  0061  GETGBL	R7	G3
      0x5C200000,  //  0062  MOVE	R8	R0
      0x7C1C0200,  //  0063  CALL	R7	1
      0x8C1C0F14,  //  0064  GETMET	R7	R7	K20
      0x5C240200,  //  0065  MOVE	R9	R1
      0x5C280400,  //  0066  MOVE	R10	R2
      0x7C1C0600,  //  0067  CALL	R7	3
      0x80040E00,  //  0068  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(Matter_Plugin_ShutterTilt_parse_sensors,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(Shutter),
    /* K2   */  be_nested_str_weak(tasmota_shutter_index),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(contains),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_nested_str_weak(Tilt),
    /* K7   */  be_nested_str_weak(shadow_shutter_tilt),
    /* K8   */  be_nested_str_weak(attribute_updated),
    /* K9   */  be_nested_str_weak(parse_sensors),
    }),
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0008,  //  0001  GETGBL	R3	G8
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x00100903,  //  0003  ADD	R4	R4	K3
      0x7C0C0200,  //  0004  CALL	R3	1
      0x000E0203,  //  0005  ADD	R3	K1	R3
      0x8C100304,  //  0006  GETMET	R4	R1	K4
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0x7812000E,  //  0009  JMPF	R4	#0019
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
      0x5426000E,  //  0016  LDINT	R9	15
      0x7C180600,  //  0017  CALL	R6	3
      0x90020E05,  //  0018  SETMBR	R0	K7	R5
      0x60100003,  //  0019  GETGBL	R4	G3
      0x5C140000,  //  001A  MOVE	R5	R0
      0x7C100200,  //  001B  CALL	R4	1
      0x8C100909,  //  001C  GETMET	R4	R4	K9
      0x5C180200,  //  001D  MOVE	R6	R1
      0x7C100400,  //  001E  CALL	R4	2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_ShutterTilt
********************************************************************/
extern const bclass be_class_Matter_Plugin_Shutter;
be_local_class(Matter_Plugin_ShutterTilt,
    2,
    &be_class_Matter_Plugin_Shutter,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(shutter_X2Btilt) },
        { be_const_key_weak(update_tilt_min_max, 0), be_const_closure(Matter_Plugin_ShutterTilt_update_tilt_min_max_closure) },
        { be_const_key_weak(parse_sensors, 7), be_const_closure(Matter_Plugin_ShutterTilt_parse_sensors_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Shutter_X20_X2B_X20Tilt) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(258, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(7),
        be_const_int(12),
        be_const_int(15),
        be_const_int(65532),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_ShutterTilt_invoke_request_closure) },
        { be_const_key_weak(tilt_min, 2), be_const_var(0) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_ShutterTilt_read_attribute_closure) },
        { be_const_key_weak(tilt_max, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_Plugin_ShutterTilt)
);
/*******************************************************************/

void be_load_Matter_Plugin_ShutterTilt_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_ShutterTilt);
    be_setglobal(vm, "Matter_Plugin_ShutterTilt");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
