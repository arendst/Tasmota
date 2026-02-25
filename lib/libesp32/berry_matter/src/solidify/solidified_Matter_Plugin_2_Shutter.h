/* Solidification of Matter_Plugin_2_Shutter.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Shutter' ktab size: 47, total: 76 (saved 232 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Shutter[47] = {
  /* K0   */  be_nested_str_weak(cluster),
  /* K1   */  be_nested_str_weak(attribute),
  /* K2   */  be_nested_str_weak(update_shadow_lazy),
  /* K3   */  be_nested_str_weak(update_inverted),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(set),
  /* K6   */  be_const_int(1),
  /* K7   */  be_nested_str_weak(shadow_shutter_pos),
  /* K8   */  be_nested_str_weak(shadow_shutter_inverted),
  /* K9   */  be_nested_str_weak(set_or_nil),
  /* K10  */  be_nested_str_weak(shadow_shutter_direction),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str_weak(shadow_shutter_target),
  /* K13  */  be_nested_str_weak(read_attribute),
  /* K14  */  be_nested_str_weak(tasmota),
  /* K15  */  be_nested_str_weak(cmd),
  /* K16  */  be_nested_str_weak(Status_X2013),
  /* K17  */  be_nested_str_weak(contains),
  /* K18  */  be_nested_str_weak(StatusSHT),
  /* K19  */  be_nested_str_weak(find),
  /* K20  */  be_nested_str_weak(SHT),
  /* K21  */  be_nested_str_weak(tasmota_shutter_index),
  /* K22  */  be_nested_str_weak(Opt),
  /* K23  */  be_nested_str_weak(parse_configuration),
  /* K24  */  be_nested_str_weak(shutter),
  /* K25  */  be_nested_str_weak(light),
  /* K26  */  be_nested_str_weak(matter),
  /* K27  */  be_nested_str_weak(TLV),
  /* K28  */  be_nested_str_weak(command),
  /* K29  */  be_nested_str_weak(ShutterStopOpen),
  /* K30  */  be_nested_str_weak(update_shadow),
  /* K31  */  be_nested_str_weak(ShutterStopClose),
  /* K32  */  be_nested_str_weak(ShutterStop),
  /* K33  */  be_nested_str_weak(log),
  /* K34  */  be_nested_str_weak(MTR_X3A_X20Tilt_X20_X3D_X20),
  /* K35  */  be_nested_str_weak(findsubval),
  /* K36  */  be_nested_str_weak(ShutterPosition),
  /* K37  */  be_nested_str_weak(_X20),
  /* K38  */  be_nested_str_weak(pos_X25_X3A),
  /* K39  */  be_nested_str_weak(invoke_request),
  /* K40  */  be_nested_str_weak(Shutter),
  /* K41  */  be_nested_str_weak(Position),
  /* K42  */  be_nested_str_weak(attribute_updated),
  /* K43  */  be_nested_str_weak(Direction),
  /* K44  */  be_nested_str_weak(Target),
  /* K45  */  be_nested_str_weak(VIRTUAL),
  /* K46  */  be_nested_str_weak(parse_sensors),
};


extern const bclass be_class_Matter_Plugin_Shutter;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Shutter_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Shutter,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[143]) {  /* code */
      0x88100500,  //  0000  GETMBR	R4	R2	K0
      0x88140501,  //  0001  GETMBR	R5	R2	K1
      0x4C180000,  //  0002  LDNIL	R6
      0x541E0101,  //  0003  LDINT	R7	258
      0x1C1C0807,  //  0004  EQ	R7	R4	R7
      0x781E007F,  //  0005  JMPF	R7	#0086
      0x8C1C0102,  //  0006  GETMET	R7	R0	K2
      0x7C1C0200,  //  0007  CALL	R7	1
      0x8C1C0103,  //  0008  GETMET	R7	R0	K3
      0x7C1C0200,  //  0009  CALL	R7	1
      0x1C1C0B04,  //  000A  EQ	R7	R5	K4
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0705,  //  000C  GETMET	R7	R3	K5
      0x54260003,  //  000D  LDINT	R9	4
      0x542A00FE,  //  000E  LDINT	R10	255
      0x7C1C0600,  //  000F  CALL	R7	3
      0x80040E00,  //  0010  RET	1	R7
      0x70020073,  //  0011  JMP		#0086
      0x541E0004,  //  0012  LDINT	R7	5
      0x1C1C0A07,  //  0013  EQ	R7	R5	R7
      0x781E0005,  //  0014  JMPF	R7	#001B
      0x8C1C0705,  //  0015  GETMET	R7	R3	K5
      0x54260004,  //  0016  LDINT	R9	5
      0x58280004,  //  0017  LDCONST	R10	K4
      0x7C1C0600,  //  0018  CALL	R7	3
      0x80040E00,  //  0019  RET	1	R7
      0x7002006A,  //  001A  JMP		#0086
      0x541E0006,  //  001B  LDINT	R7	7
      0x1C1C0A07,  //  001C  EQ	R7	R5	R7
      0x781E0006,  //  001D  JMPF	R7	#0025
      0x8C1C0705,  //  001E  GETMET	R7	R3	K5
      0x54260003,  //  001F  LDINT	R9	4
      0x542A0007,  //  0020  LDINT	R10	8
      0x002A0C0A,  //  0021  ADD	R10	K6	R10
      0x7C1C0600,  //  0022  CALL	R7	3
      0x80040E00,  //  0023  RET	1	R7
      0x70020060,  //  0024  JMP		#0086
      0x541E000C,  //  0025  LDINT	R7	13
      0x1C1C0A07,  //  0026  EQ	R7	R5	R7
      0x781E0005,  //  0027  JMPF	R7	#002E
      0x8C1C0705,  //  0028  GETMET	R7	R3	K5
      0x54260003,  //  0029  LDINT	R9	4
      0x542A00FE,  //  002A  LDINT	R10	255
      0x7C1C0600,  //  002B  CALL	R7	3
      0x80040E00,  //  002C  RET	1	R7
      0x70020057,  //  002D  JMP		#0086
      0x541E000D,  //  002E  LDINT	R7	14
      0x1C1C0A07,  //  002F  EQ	R7	R5	R7
      0x781E0017,  //  0030  JMPF	R7	#0049
      0x881C0107,  //  0031  GETMBR	R7	R0	K7
      0x4C200000,  //  0032  LDNIL	R8
      0x201C0E08,  //  0033  NE	R7	R7	R8
      0x781E000D,  //  0034  JMPF	R7	#0043
      0x881C0108,  //  0035  GETMBR	R7	R0	K8
      0x1C1C0F04,  //  0036  EQ	R7	R7	K4
      0x781E0006,  //  0037  JMPF	R7	#003F
      0x541E0063,  //  0038  LDINT	R7	100
      0x88200107,  //  0039  GETMBR	R8	R0	K7
      0x041C0E08,  //  003A  SUB	R7	R7	R8
      0x54220063,  //  003B  LDINT	R8	100
      0x081C0E08,  //  003C  MUL	R7	R7	R8
      0x5C180E00,  //  003D  MOVE	R6	R7
      0x70020003,  //  003E  JMP		#0043
      0x881C0107,  //  003F  GETMBR	R7	R0	K7
      0x54220063,  //  0040  LDINT	R8	100
      0x081C0E08,  //  0041  MUL	R7	R7	R8
      0x5C180E00,  //  0042  MOVE	R6	R7
      0x8C1C0709,  //  0043  GETMET	R7	R3	K9
      0x54260004,  //  0044  LDINT	R9	5
      0x5C280C00,  //  0045  MOVE	R10	R6
      0x7C1C0600,  //  0046  CALL	R7	3
      0x80040E00,  //  0047  RET	1	R7
      0x7002003C,  //  0048  JMP		#0086
      0x541E0009,  //  0049  LDINT	R7	10
      0x1C1C0A07,  //  004A  EQ	R7	R5	R7
      0x781E0016,  //  004B  JMPF	R7	#0063
      0x4C1C0000,  //  004C  LDNIL	R7
      0x8820010A,  //  004D  GETMBR	R8	R0	K10
      0x4C240000,  //  004E  LDNIL	R9
      0x20201009,  //  004F  NE	R8	R8	R9
      0x7822000B,  //  0050  JMPF	R8	#005D
      0x8820010A,  //  0051  GETMBR	R8	R0	K10
      0x1C201104,  //  0052  EQ	R8	R8	K4
      0x78220001,  //  0053  JMPF	R8	#0056
      0x58200004,  //  0054  LDCONST	R8	K4
      0x70020005,  //  0055  JMP		#005C
      0x8820010A,  //  0056  GETMBR	R8	R0	K10
      0x24201104,  //  0057  GT	R8	R8	K4
      0x78220001,  //  0058  JMPF	R8	#005B
      0x58200006,  //  0059  LDCONST	R8	K6
      0x70020000,  //  005A  JMP		#005C
      0x5820000B,  //  005B  LDCONST	R8	K11
      0x5C1C1000,  //  005C  MOVE	R7	R8
      0x8C200709,  //  005D  GETMET	R8	R3	K9
      0x542A0003,  //  005E  LDINT	R10	4
      0x5C2C0E00,  //  005F  MOVE	R11	R7
      0x7C200600,  //  0060  CALL	R8	3
      0x80041000,  //  0061  RET	1	R8
      0x70020022,  //  0062  JMP		#0086
      0x541E000A,  //  0063  LDINT	R7	11
      0x1C1C0A07,  //  0064  EQ	R7	R5	R7
      0x781E0017,  //  0065  JMPF	R7	#007E
      0x881C010C,  //  0066  GETMBR	R7	R0	K12
      0x4C200000,  //  0067  LDNIL	R8
      0x201C0E08,  //  0068  NE	R7	R7	R8
      0x781E000D,  //  0069  JMPF	R7	#0078
      0x881C0108,  //  006A  GETMBR	R7	R0	K8
      0x1C1C0F04,  //  006B  EQ	R7	R7	K4
      0x781E0006,  //  006C  JMPF	R7	#0074
      0x541E0063,  //  006D  LDINT	R7	100
      0x8820010C,  //  006E  GETMBR	R8	R0	K12
      0x041C0E08,  //  006F  SUB	R7	R7	R8
      0x54220063,  //  0070  LDINT	R8	100
      0x081C0E08,  //  0071  MUL	R7	R7	R8
      0x5C180E00,  //  0072  MOVE	R6	R7
      0x70020003,  //  0073  JMP		#0078
      0x881C010C,  //  0074  GETMBR	R7	R0	K12
      0x54220063,  //  0075  LDINT	R8	100
      0x081C0E08,  //  0076  MUL	R7	R7	R8
      0x5C180E00,  //  0077  MOVE	R6	R7
      0x8C1C0709,  //  0078  GETMET	R7	R3	K9
      0x54260004,  //  0079  LDINT	R9	5
      0x5C280C00,  //  007A  MOVE	R10	R6
      0x7C1C0600,  //  007B  CALL	R7	3
      0x80040E00,  //  007C  RET	1	R7
      0x70020007,  //  007D  JMP		#0086
      0x541E0016,  //  007E  LDINT	R7	23
      0x1C1C0A07,  //  007F  EQ	R7	R5	R7
      0x781E0004,  //  0080  JMPF	R7	#0086
      0x8C1C0705,  //  0081  GETMET	R7	R3	K5
      0x54260003,  //  0082  LDINT	R9	4
      0x58280004,  //  0083  LDCONST	R10	K4
      0x7C1C0600,  //  0084  CALL	R7	3
      0x80040E00,  //  0085  RET	1	R7
      0x601C0003,  //  0086  GETGBL	R7	G3
      0x5C200000,  //  0087  MOVE	R8	R0
      0x7C1C0200,  //  0088  CALL	R7	1
      0x8C1C0F0D,  //  0089  GETMET	R7	R7	K13
      0x5C240200,  //  008A  MOVE	R9	R1
      0x5C280400,  //  008B  MOVE	R10	R2
      0x5C2C0600,  //  008C  MOVE	R11	R3
      0x7C1C0800,  //  008D  CALL	R7	4
      0x80040E00,  //  008E  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_inverted
********************************************************************/
be_local_closure(class_Matter_Plugin_Shutter_update_inverted,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Shutter,     /* shared constants */
    be_str_weak(update_inverted),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x5409FFFE,  //  0001  LDINT	R2	-1
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806001F,  //  0003  JMPF	R1	#0024
      0xB8061C00,  //  0004  GETNGBL	R1	K14
      0x8C04030F,  //  0005  GETMET	R1	R1	K15
      0x580C0010,  //  0006  LDCONST	R3	K16
      0x50100200,  //  0007  LDBOOL	R4	1	0
      0x7C040600,  //  0008  CALL	R1	3
      0x8C080311,  //  0009  GETMET	R2	R1	K17
      0x58100012,  //  000A  LDCONST	R4	K18
      0x7C080400,  //  000B  CALL	R2	2
      0x780A0016,  //  000C  JMPF	R2	#0024
      0x94040312,  //  000D  GETIDX	R1	R1	K18
      0x8C080313,  //  000E  GETMET	R2	R1	K19
      0x60100008,  //  000F  GETGBL	R4	G8
      0x88140115,  //  0010  GETMBR	R5	R0	K21
      0x7C100200,  //  0011  CALL	R4	1
      0x00122804,  //  0012  ADD	R4	K20	R4
      0x60140013,  //  0013  GETGBL	R5	G19
      0x7C140000,  //  0014  CALL	R5	0
      0x7C080600,  //  0015  CALL	R2	3
      0x8C080513,  //  0016  GETMET	R2	R2	K19
      0x58100016,  //  0017  LDCONST	R4	K22
      0x7C080400,  //  0018  CALL	R2	2
      0x4C0C0000,  //  0019  LDNIL	R3
      0x200C0403,  //  001A  NE	R3	R2	R3
      0x780E0007,  //  001B  JMPF	R3	#0024
      0x600C0009,  //  001C  GETGBL	R3	G9
      0x6010000C,  //  001D  GETGBL	R4	G12
      0x5C140400,  //  001E  MOVE	R5	R2
      0x7C100200,  //  001F  CALL	R4	1
      0x04100906,  //  0020  SUB	R4	R4	K6
      0x94100404,  //  0021  GETIDX	R4	R2	R4
      0x7C0C0200,  //  0022  CALL	R3	1
      0x90021003,  //  0023  SETMBR	R0	K8	R3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(class_Matter_Plugin_Shutter_parse_configuration,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Shutter,     /* shared constants */
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080517,  //  0003  GETMET	R2	R2	K23
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080313,  //  0006  GETMET	R2	R1	K19
      0x58100018,  //  0007  LDCONST	R4	K24
      0x7C080400,  //  0008  CALL	R2	2
      0x90022A02,  //  0009  SETMBR	R0	K21	R2
      0x88080115,  //  000A  GETMBR	R2	R0	K21
      0x4C0C0000,  //  000B  LDNIL	R3
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0000,  //  000D  JMPF	R2	#000F
      0x90022B04,  //  000E  SETMBR	R0	K21	K4
      0x5409FFFE,  //  000F  LDINT	R2	-1
      0x90021002,  //  0010  SETMBR	R0	K8	R2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(class_Matter_Plugin_Shutter_invoke_request,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Shutter,     /* shared constants */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
      0xA4123200,  //  0000  IMPORT	R4	K25
      0xB8163400,  //  0001  GETNGBL	R5	K26
      0x88140B1B,  //  0002  GETMBR	R5	R5	K27
      0x88180700,  //  0003  GETMBR	R6	R3	K0
      0x881C071C,  //  0004  GETMBR	R7	R3	K28
      0x54220101,  //  0005  LDINT	R8	258
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220060,  //  0007  JMPF	R8	#0069
      0x8C200102,  //  0008  GETMET	R8	R0	K2
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F04,  //  000A  EQ	R8	R7	K4
      0x7822000D,  //  000B  JMPF	R8	#001A
      0xB8221C00,  //  000C  GETNGBL	R8	K14
      0x8C20110F,  //  000D  GETMET	R8	R8	K15
      0x60280008,  //  000E  GETGBL	R10	G8
      0x882C0115,  //  000F  GETMBR	R11	R0	K21
      0x002C1706,  //  0010  ADD	R11	R11	K6
      0x7C280200,  //  0011  CALL	R10	1
      0x002A3A0A,  //  0012  ADD	R10	K29	R10
      0x502C0200,  //  0013  LDBOOL	R11	1	0
      0x7C200600,  //  0014  CALL	R8	3
      0x8C20011E,  //  0015  GETMET	R8	R0	K30
      0x7C200200,  //  0016  CALL	R8	1
      0x50200200,  //  0017  LDBOOL	R8	1	0
      0x80041000,  //  0018  RET	1	R8
      0x7002004D,  //  0019  JMP		#0068
      0x1C200F06,  //  001A  EQ	R8	R7	K6
      0x7822000D,  //  001B  JMPF	R8	#002A
      0xB8221C00,  //  001C  GETNGBL	R8	K14
      0x8C20110F,  //  001D  GETMET	R8	R8	K15
      0x60280008,  //  001E  GETGBL	R10	G8
      0x882C0115,  //  001F  GETMBR	R11	R0	K21
      0x002C1706,  //  0020  ADD	R11	R11	K6
      0x7C280200,  //  0021  CALL	R10	1
      0x002A3E0A,  //  0022  ADD	R10	K31	R10
      0x502C0200,  //  0023  LDBOOL	R11	1	0
      0x7C200600,  //  0024  CALL	R8	3
      0x8C20011E,  //  0025  GETMET	R8	R0	K30
      0x7C200200,  //  0026  CALL	R8	1
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x7002003D,  //  0029  JMP		#0068
      0x1C200F0B,  //  002A  EQ	R8	R7	K11
      0x7822000D,  //  002B  JMPF	R8	#003A
      0xB8221C00,  //  002C  GETNGBL	R8	K14
      0x8C20110F,  //  002D  GETMET	R8	R8	K15
      0x60280008,  //  002E  GETGBL	R10	G8
      0x882C0115,  //  002F  GETMBR	R11	R0	K21
      0x002C1706,  //  0030  ADD	R11	R11	K6
      0x7C280200,  //  0031  CALL	R10	1
      0x002A400A,  //  0032  ADD	R10	K32	R10
      0x502C0200,  //  0033  LDBOOL	R11	1	0
      0x7C200600,  //  0034  CALL	R8	3
      0x8C20011E,  //  0035  GETMET	R8	R0	K30
      0x7C200200,  //  0036  CALL	R8	1
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x80041000,  //  0038  RET	1	R8
      0x7002002D,  //  0039  JMP		#0068
      0x54220004,  //  003A  LDINT	R8	5
      0x1C200E08,  //  003B  EQ	R8	R7	R8
      0x7822002A,  //  003C  JMPF	R8	#0068
      0xB8224200,  //  003D  GETNGBL	R8	K33
      0x60240008,  //  003E  GETGBL	R9	G8
      0x5C280400,  //  003F  MOVE	R10	R2
      0x7C240200,  //  0040  CALL	R9	1
      0x00264409,  //  0041  ADD	R9	K34	R9
      0x5828000B,  //  0042  LDCONST	R10	K11
      0x7C200400,  //  0043  CALL	R8	2
      0x8C200523,  //  0044  GETMET	R8	R2	K35
      0x58280004,  //  0045  LDCONST	R10	K4
      0x7C200400,  //  0046  CALL	R8	2
      0x4C240000,  //  0047  LDNIL	R9
      0x20241009,  //  0048  NE	R9	R8	R9
      0x7826001B,  //  0049  JMPF	R9	#0066
      0x54260063,  //  004A  LDINT	R9	100
      0x0C201009,  //  004B  DIV	R8	R8	R9
      0x88240108,  //  004C  GETMBR	R9	R0	K8
      0x1C241304,  //  004D  EQ	R9	R9	K4
      0x78260001,  //  004E  JMPF	R9	#0051
      0x54260063,  //  004F  LDINT	R9	100
      0x04201208,  //  0050  SUB	R8	R9	R8
      0xB8261C00,  //  0051  GETNGBL	R9	K14
      0x8C24130F,  //  0052  GETMET	R9	R9	K15
      0x602C0008,  //  0053  GETGBL	R11	G8
      0x88300115,  //  0054  GETMBR	R12	R0	K21
      0x00301906,  //  0055  ADD	R12	R12	K6
      0x7C2C0200,  //  0056  CALL	R11	1
      0x002E480B,  //  0057  ADD	R11	K36	R11
      0x002C1725,  //  0058  ADD	R11	R11	K37
      0x60300008,  //  0059  GETGBL	R12	G8
      0x5C341000,  //  005A  MOVE	R13	R8
      0x7C300200,  //  005B  CALL	R12	1
      0x002C160C,  //  005C  ADD	R11	R11	R12
      0x50300200,  //  005D  LDBOOL	R12	1	0
      0x7C240600,  //  005E  CALL	R9	3
      0x60240008,  //  005F  GETGBL	R9	G8
      0x5C281000,  //  0060  MOVE	R10	R8
      0x7C240200,  //  0061  CALL	R9	1
      0x00264C09,  //  0062  ADD	R9	K38	R9
      0x900E4209,  //  0063  SETMBR	R3	K33	R9
      0x8C24011E,  //  0064  GETMET	R9	R0	K30
      0x7C240200,  //  0065  CALL	R9	1
      0x50240200,  //  0066  LDBOOL	R9	1	0
      0x80041200,  //  0067  RET	1	R9
      0x70020008,  //  0068  JMP		#0072
      0x60200003,  //  0069  GETGBL	R8	G3
      0x5C240000,  //  006A  MOVE	R9	R0
      0x7C200200,  //  006B  CALL	R8	1
      0x8C201127,  //  006C  GETMET	R8	R8	K39
      0x5C280200,  //  006D  MOVE	R10	R1
      0x5C2C0400,  //  006E  MOVE	R11	R2
      0x5C300600,  //  006F  MOVE	R12	R3
      0x7C200800,  //  0070  CALL	R8	4
      0x80041000,  //  0071  RET	1	R8
      0x80000000,  //  0072  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(class_Matter_Plugin_Shutter_parse_sensors,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Shutter,     /* shared constants */
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x880C0115,  //  0001  GETMBR	R3	R0	K21
      0x000C0706,  //  0002  ADD	R3	R3	K6
      0x7C080200,  //  0003  CALL	R2	1
      0x000A5002,  //  0004  ADD	R2	K40	R2
      0x8C0C0311,  //  0005  GETMET	R3	R1	K17
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E002A,  //  0008  JMPF	R3	#0034
      0x940C0202,  //  0009  GETIDX	R3	R1	R2
      0x8C100713,  //  000A  GETMET	R4	R3	K19
      0x58180029,  //  000B  LDCONST	R6	K41
      0x7C100400,  //  000C  CALL	R4	2
      0x4C140000,  //  000D  LDNIL	R5
      0x20140805,  //  000E  NE	R5	R4	R5
      0x78160007,  //  000F  JMPF	R5	#0018
      0x88140107,  //  0010  GETMBR	R5	R0	K7
      0x20140805,  //  0011  NE	R5	R4	R5
      0x78160003,  //  0012  JMPF	R5	#0017
      0x8C14012A,  //  0013  GETMET	R5	R0	K42
      0x541E0101,  //  0014  LDINT	R7	258
      0x5422000D,  //  0015  LDINT	R8	14
      0x7C140600,  //  0016  CALL	R5	3
      0x90020E04,  //  0017  SETMBR	R0	K7	R4
      0x8C140713,  //  0018  GETMET	R5	R3	K19
      0x581C002B,  //  0019  LDCONST	R7	K43
      0x7C140400,  //  001A  CALL	R5	2
      0x4C180000,  //  001B  LDNIL	R6
      0x20180A06,  //  001C  NE	R6	R5	R6
      0x781A0007,  //  001D  JMPF	R6	#0026
      0x8818010A,  //  001E  GETMBR	R6	R0	K10
      0x20180A06,  //  001F  NE	R6	R5	R6
      0x781A0003,  //  0020  JMPF	R6	#0025
      0x8C18012A,  //  0021  GETMET	R6	R0	K42
      0x54220101,  //  0022  LDINT	R8	258
      0x54260009,  //  0023  LDINT	R9	10
      0x7C180600,  //  0024  CALL	R6	3
      0x90021405,  //  0025  SETMBR	R0	K10	R5
      0x8C180713,  //  0026  GETMET	R6	R3	K19
      0x5820002C,  //  0027  LDCONST	R8	K44
      0x7C180400,  //  0028  CALL	R6	2
      0x4C1C0000,  //  0029  LDNIL	R7
      0x201C0C07,  //  002A  NE	R7	R6	R7
      0x781E0007,  //  002B  JMPF	R7	#0034
      0x881C010C,  //  002C  GETMBR	R7	R0	K12
      0x201C0C07,  //  002D  NE	R7	R6	R7
      0x781E0003,  //  002E  JMPF	R7	#0033
      0x8C1C012A,  //  002F  GETMET	R7	R0	K42
      0x54260101,  //  0030  LDINT	R9	258
      0x542A000A,  //  0031  LDINT	R10	11
      0x7C1C0600,  //  0032  CALL	R7	3
      0x90021806,  //  0033  SETMBR	R0	K12	R6
      0x80000000,  //  0034  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(class_Matter_Plugin_Shutter_update_shadow,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Shutter,     /* shared constants */
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8804012D,  //  0000  GETMBR	R1	R0	K45
      0x7406000E,  //  0001  JMPT	R1	#0011
      0x8C040103,  //  0002  GETMET	R1	R0	K3
      0x7C040200,  //  0003  CALL	R1	1
      0xB8061C00,  //  0004  GETNGBL	R1	K14
      0x8C04030F,  //  0005  GETMET	R1	R1	K15
      0x600C0008,  //  0006  GETGBL	R3	G8
      0x88100115,  //  0007  GETMBR	R4	R0	K21
      0x00100906,  //  0008  ADD	R4	R4	K6
      0x7C0C0200,  //  0009  CALL	R3	1
      0x000E4803,  //  000A  ADD	R3	K36	R3
      0x50100200,  //  000B  LDBOOL	R4	1	0
      0x7C040600,  //  000C  CALL	R1	3
      0x78060002,  //  000D  JMPF	R1	#0011
      0x8C08012E,  //  000E  GETMET	R2	R0	K46
      0x5C100200,  //  000F  MOVE	R4	R1
      0x7C080400,  //  0010  CALL	R2	2
      0x60040003,  //  0011  GETGBL	R1	G3
      0x5C080000,  //  0012  MOVE	R2	R0
      0x7C040200,  //  0013  CALL	R1	1
      0x8C04031E,  //  0014  GETMET	R1	R1	K30
      0x7C040200,  //  0015  CALL	R1	1
      0x80000000,  //  0016  RET	0
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
        { be_const_key_weak(update_shadow, 14), be_const_closure(class_Matter_Plugin_Shutter_update_shadow_closure) },
        { be_const_key_weak(shadow_shutter_inverted, -1), be_const_var(4) },
        { be_const_key_weak(SCHEMA, -1), be_nested_str_weak(shutter_X7Cl_X3AShutter_X7Ct_X3Ai_X7Ch_X3ARelay_X3Cx_X3E_X20number) },
        { be_const_key_weak(shadow_shutter_direction, 15), be_const_var(3) },
        { be_const_key_weak(update_inverted, -1), be_const_closure(class_Matter_Plugin_Shutter_update_inverted_closure) },
        { be_const_key_weak(shadow_shutter_target, 0), be_const_var(2) },
        { be_const_key_weak(shadow_shutter_pos, 2), be_const_var(1) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(258, -1), be_const_bytes_instance(000000050007000A000B000D000E0017FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, 2), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 1), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(TYPES, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(514, -1), be_const_int(3) },
    }))    ) } )) },
        { be_const_key_weak(tasmota_shutter_index, -1), be_const_var(0) },
        { be_const_key_weak(read_attribute, 8), be_const_closure(class_Matter_Plugin_Shutter_read_attribute_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(class_Matter_Plugin_Shutter_invoke_request_closure) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(class_Matter_Plugin_Shutter_parse_sensors_closure) },
        { be_const_key_weak(DISPLAY_NAME, 3), be_nested_str_weak(Shutter) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(class_Matter_Plugin_Shutter_parse_configuration_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(shutter) },
    })),
    be_str_weak(Matter_Plugin_Shutter)
);
/********************************************************************/
/* End of solidification */
