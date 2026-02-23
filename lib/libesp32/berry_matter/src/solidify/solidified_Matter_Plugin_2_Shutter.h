/* Solidification of Matter_Plugin_2_Shutter.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Shutter' ktab size: 49, total: 80 (saved 248 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Shutter[49] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(cluster),
  /* K3   */  be_nested_str_weak(attribute),
  /* K4   */  be_nested_str_weak(update_shadow_lazy),
  /* K5   */  be_nested_str_weak(update_inverted),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(set),
  /* K8   */  be_nested_str_weak(U1),
  /* K9   */  be_nested_str_weak(U2),
  /* K10  */  be_const_int(1),
  /* K11  */  be_nested_str_weak(shadow_shutter_pos),
  /* K12  */  be_nested_str_weak(shadow_shutter_inverted),
  /* K13  */  be_nested_str_weak(set_or_nil),
  /* K14  */  be_nested_str_weak(shadow_shutter_direction),
  /* K15  */  be_const_int(2),
  /* K16  */  be_nested_str_weak(shadow_shutter_target),
  /* K17  */  be_nested_str_weak(read_attribute),
  /* K18  */  be_nested_str_weak(tasmota),
  /* K19  */  be_nested_str_weak(cmd),
  /* K20  */  be_nested_str_weak(Status_X2013),
  /* K21  */  be_nested_str_weak(contains),
  /* K22  */  be_nested_str_weak(StatusSHT),
  /* K23  */  be_nested_str_weak(find),
  /* K24  */  be_nested_str_weak(SHT),
  /* K25  */  be_nested_str_weak(tasmota_shutter_index),
  /* K26  */  be_nested_str_weak(Opt),
  /* K27  */  be_nested_str_weak(parse_configuration),
  /* K28  */  be_nested_str_weak(shutter),
  /* K29  */  be_nested_str_weak(light),
  /* K30  */  be_nested_str_weak(command),
  /* K31  */  be_nested_str_weak(ShutterStopOpen),
  /* K32  */  be_nested_str_weak(update_shadow),
  /* K33  */  be_nested_str_weak(ShutterStopClose),
  /* K34  */  be_nested_str_weak(ShutterStop),
  /* K35  */  be_nested_str_weak(log),
  /* K36  */  be_nested_str_weak(MTR_X3A_X20Tilt_X20_X3D_X20),
  /* K37  */  be_nested_str_weak(findsubval),
  /* K38  */  be_nested_str_weak(ShutterPosition),
  /* K39  */  be_nested_str_weak(_X20),
  /* K40  */  be_nested_str_weak(pos_X25_X3A),
  /* K41  */  be_nested_str_weak(invoke_request),
  /* K42  */  be_nested_str_weak(Shutter),
  /* K43  */  be_nested_str_weak(Position),
  /* K44  */  be_nested_str_weak(attribute_updated),
  /* K45  */  be_nested_str_weak(Direction),
  /* K46  */  be_nested_str_weak(Target),
  /* K47  */  be_nested_str_weak(VIRTUAL),
  /* K48  */  be_nested_str_weak(parse_sensors),
};


extern const bclass be_class_Matter_Plugin_Shutter;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Shutter_read_attribute,   /* name */
  be_nested_proto(
    13,                          /* nstack */
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
    ( &(const binstruction[145]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x4C1C0000,  //  0004  LDNIL	R7
      0x54220101,  //  0005  LDINT	R8	258
      0x1C200A08,  //  0006  EQ	R8	R5	R8
      0x7822007F,  //  0007  JMPF	R8	#0088
      0x8C200104,  //  0008  GETMET	R8	R0	K4
      0x7C200200,  //  0009  CALL	R8	1
      0x8C200105,  //  000A  GETMET	R8	R0	K5
      0x7C200200,  //  000B  CALL	R8	1
      0x1C200D06,  //  000C  EQ	R8	R6	K6
      0x78220005,  //  000D  JMPF	R8	#0014
      0x8C200707,  //  000E  GETMET	R8	R3	K7
      0x88280908,  //  000F  GETMBR	R10	R4	K8
      0x542E00FE,  //  0010  LDINT	R11	255
      0x7C200600,  //  0011  CALL	R8	3
      0x80041000,  //  0012  RET	1	R8
      0x70020073,  //  0013  JMP		#0088
      0x54220004,  //  0014  LDINT	R8	5
      0x1C200C08,  //  0015  EQ	R8	R6	R8
      0x78220005,  //  0016  JMPF	R8	#001D
      0x8C200707,  //  0017  GETMET	R8	R3	K7
      0x88280909,  //  0018  GETMBR	R10	R4	K9
      0x582C0006,  //  0019  LDCONST	R11	K6
      0x7C200600,  //  001A  CALL	R8	3
      0x80041000,  //  001B  RET	1	R8
      0x7002006A,  //  001C  JMP		#0088
      0x54220006,  //  001D  LDINT	R8	7
      0x1C200C08,  //  001E  EQ	R8	R6	R8
      0x78220006,  //  001F  JMPF	R8	#0027
      0x8C200707,  //  0020  GETMET	R8	R3	K7
      0x88280908,  //  0021  GETMBR	R10	R4	K8
      0x542E0007,  //  0022  LDINT	R11	8
      0x002E140B,  //  0023  ADD	R11	K10	R11
      0x7C200600,  //  0024  CALL	R8	3
      0x80041000,  //  0025  RET	1	R8
      0x70020060,  //  0026  JMP		#0088
      0x5422000C,  //  0027  LDINT	R8	13
      0x1C200C08,  //  0028  EQ	R8	R6	R8
      0x78220005,  //  0029  JMPF	R8	#0030
      0x8C200707,  //  002A  GETMET	R8	R3	K7
      0x88280908,  //  002B  GETMBR	R10	R4	K8
      0x542E00FE,  //  002C  LDINT	R11	255
      0x7C200600,  //  002D  CALL	R8	3
      0x80041000,  //  002E  RET	1	R8
      0x70020057,  //  002F  JMP		#0088
      0x5422000D,  //  0030  LDINT	R8	14
      0x1C200C08,  //  0031  EQ	R8	R6	R8
      0x78220017,  //  0032  JMPF	R8	#004B
      0x8820010B,  //  0033  GETMBR	R8	R0	K11
      0x4C240000,  //  0034  LDNIL	R9
      0x20201009,  //  0035  NE	R8	R8	R9
      0x7822000D,  //  0036  JMPF	R8	#0045
      0x8820010C,  //  0037  GETMBR	R8	R0	K12
      0x1C201106,  //  0038  EQ	R8	R8	K6
      0x78220006,  //  0039  JMPF	R8	#0041
      0x54220063,  //  003A  LDINT	R8	100
      0x8824010B,  //  003B  GETMBR	R9	R0	K11
      0x04201009,  //  003C  SUB	R8	R8	R9
      0x54260063,  //  003D  LDINT	R9	100
      0x08201009,  //  003E  MUL	R8	R8	R9
      0x5C1C1000,  //  003F  MOVE	R7	R8
      0x70020003,  //  0040  JMP		#0045
      0x8820010B,  //  0041  GETMBR	R8	R0	K11
      0x54260063,  //  0042  LDINT	R9	100
      0x08201009,  //  0043  MUL	R8	R8	R9
      0x5C1C1000,  //  0044  MOVE	R7	R8
      0x8C20070D,  //  0045  GETMET	R8	R3	K13
      0x88280909,  //  0046  GETMBR	R10	R4	K9
      0x5C2C0E00,  //  0047  MOVE	R11	R7
      0x7C200600,  //  0048  CALL	R8	3
      0x80041000,  //  0049  RET	1	R8
      0x7002003C,  //  004A  JMP		#0088
      0x54220009,  //  004B  LDINT	R8	10
      0x1C200C08,  //  004C  EQ	R8	R6	R8
      0x78220016,  //  004D  JMPF	R8	#0065
      0x4C200000,  //  004E  LDNIL	R8
      0x8824010E,  //  004F  GETMBR	R9	R0	K14
      0x4C280000,  //  0050  LDNIL	R10
      0x2024120A,  //  0051  NE	R9	R9	R10
      0x7826000B,  //  0052  JMPF	R9	#005F
      0x8824010E,  //  0053  GETMBR	R9	R0	K14
      0x1C241306,  //  0054  EQ	R9	R9	K6
      0x78260001,  //  0055  JMPF	R9	#0058
      0x58240006,  //  0056  LDCONST	R9	K6
      0x70020005,  //  0057  JMP		#005E
      0x8824010E,  //  0058  GETMBR	R9	R0	K14
      0x24241306,  //  0059  GT	R9	R9	K6
      0x78260001,  //  005A  JMPF	R9	#005D
      0x5824000A,  //  005B  LDCONST	R9	K10
      0x70020000,  //  005C  JMP		#005E
      0x5824000F,  //  005D  LDCONST	R9	K15
      0x5C201200,  //  005E  MOVE	R8	R9
      0x8C24070D,  //  005F  GETMET	R9	R3	K13
      0x882C0908,  //  0060  GETMBR	R11	R4	K8
      0x5C301000,  //  0061  MOVE	R12	R8
      0x7C240600,  //  0062  CALL	R9	3
      0x80041200,  //  0063  RET	1	R9
      0x70020022,  //  0064  JMP		#0088
      0x5422000A,  //  0065  LDINT	R8	11
      0x1C200C08,  //  0066  EQ	R8	R6	R8
      0x78220017,  //  0067  JMPF	R8	#0080
      0x88200110,  //  0068  GETMBR	R8	R0	K16
      0x4C240000,  //  0069  LDNIL	R9
      0x20201009,  //  006A  NE	R8	R8	R9
      0x7822000D,  //  006B  JMPF	R8	#007A
      0x8820010C,  //  006C  GETMBR	R8	R0	K12
      0x1C201106,  //  006D  EQ	R8	R8	K6
      0x78220006,  //  006E  JMPF	R8	#0076
      0x54220063,  //  006F  LDINT	R8	100
      0x88240110,  //  0070  GETMBR	R9	R0	K16
      0x04201009,  //  0071  SUB	R8	R8	R9
      0x54260063,  //  0072  LDINT	R9	100
      0x08201009,  //  0073  MUL	R8	R8	R9
      0x5C1C1000,  //  0074  MOVE	R7	R8
      0x70020003,  //  0075  JMP		#007A
      0x88200110,  //  0076  GETMBR	R8	R0	K16
      0x54260063,  //  0077  LDINT	R9	100
      0x08201009,  //  0078  MUL	R8	R8	R9
      0x5C1C1000,  //  0079  MOVE	R7	R8
      0x8C20070D,  //  007A  GETMET	R8	R3	K13
      0x88280909,  //  007B  GETMBR	R10	R4	K9
      0x5C2C0E00,  //  007C  MOVE	R11	R7
      0x7C200600,  //  007D  CALL	R8	3
      0x80041000,  //  007E  RET	1	R8
      0x70020007,  //  007F  JMP		#0088
      0x54220016,  //  0080  LDINT	R8	23
      0x1C200C08,  //  0081  EQ	R8	R6	R8
      0x78220004,  //  0082  JMPF	R8	#0088
      0x8C200707,  //  0083  GETMET	R8	R3	K7
      0x88280908,  //  0084  GETMBR	R10	R4	K8
      0x582C0006,  //  0085  LDCONST	R11	K6
      0x7C200600,  //  0086  CALL	R8	3
      0x80041000,  //  0087  RET	1	R8
      0x60200003,  //  0088  GETGBL	R8	G3
      0x5C240000,  //  0089  MOVE	R9	R0
      0x7C200200,  //  008A  CALL	R8	1
      0x8C201111,  //  008B  GETMET	R8	R8	K17
      0x5C280200,  //  008C  MOVE	R10	R1
      0x5C2C0400,  //  008D  MOVE	R11	R2
      0x5C300600,  //  008E  MOVE	R12	R3
      0x7C200800,  //  008F  CALL	R8	4
      0x80041000,  //  0090  RET	1	R8
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
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x5409FFFE,  //  0001  LDINT	R2	-1
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806001F,  //  0003  JMPF	R1	#0024
      0xB8062400,  //  0004  GETNGBL	R1	K18
      0x8C040313,  //  0005  GETMET	R1	R1	K19
      0x580C0014,  //  0006  LDCONST	R3	K20
      0x50100200,  //  0007  LDBOOL	R4	1	0
      0x7C040600,  //  0008  CALL	R1	3
      0x8C080315,  //  0009  GETMET	R2	R1	K21
      0x58100016,  //  000A  LDCONST	R4	K22
      0x7C080400,  //  000B  CALL	R2	2
      0x780A0016,  //  000C  JMPF	R2	#0024
      0x94040316,  //  000D  GETIDX	R1	R1	K22
      0x8C080317,  //  000E  GETMET	R2	R1	K23
      0x60100008,  //  000F  GETGBL	R4	G8
      0x88140119,  //  0010  GETMBR	R5	R0	K25
      0x7C100200,  //  0011  CALL	R4	1
      0x00123004,  //  0012  ADD	R4	K24	R4
      0x60140013,  //  0013  GETGBL	R5	G19
      0x7C140000,  //  0014  CALL	R5	0
      0x7C080600,  //  0015  CALL	R2	3
      0x8C080517,  //  0016  GETMET	R2	R2	K23
      0x5810001A,  //  0017  LDCONST	R4	K26
      0x7C080400,  //  0018  CALL	R2	2
      0x4C0C0000,  //  0019  LDNIL	R3
      0x200C0403,  //  001A  NE	R3	R2	R3
      0x780E0007,  //  001B  JMPF	R3	#0024
      0x600C0009,  //  001C  GETGBL	R3	G9
      0x6010000C,  //  001D  GETGBL	R4	G12
      0x5C140400,  //  001E  MOVE	R5	R2
      0x7C100200,  //  001F  CALL	R4	1
      0x0410090A,  //  0020  SUB	R4	R4	K10
      0x94100404,  //  0021  GETIDX	R4	R2	R4
      0x7C0C0200,  //  0022  CALL	R3	1
      0x90021803,  //  0023  SETMBR	R0	K12	R3
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
      0x8C08051B,  //  0003  GETMET	R2	R2	K27
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080317,  //  0006  GETMET	R2	R1	K23
      0x5810001C,  //  0007  LDCONST	R4	K28
      0x7C080400,  //  0008  CALL	R2	2
      0x90023202,  //  0009  SETMBR	R0	K25	R2
      0x88080119,  //  000A  GETMBR	R2	R0	K25
      0x4C0C0000,  //  000B  LDNIL	R3
      0x1C080403,  //  000C  EQ	R2	R2	R3
      0x780A0000,  //  000D  JMPF	R2	#000F
      0x90023306,  //  000E  SETMBR	R0	K25	K6
      0x5409FFFE,  //  000F  LDINT	R2	-1
      0x90021802,  //  0010  SETMBR	R0	K12	R2
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
      0xA4123A00,  //  0000  IMPORT	R4	K29
      0xB8160000,  //  0001  GETNGBL	R5	K0
      0x88140B01,  //  0002  GETMBR	R5	R5	K1
      0x88180702,  //  0003  GETMBR	R6	R3	K2
      0x881C071E,  //  0004  GETMBR	R7	R3	K30
      0x54220101,  //  0005  LDINT	R8	258
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220060,  //  0007  JMPF	R8	#0069
      0x8C200104,  //  0008  GETMET	R8	R0	K4
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x7822000D,  //  000B  JMPF	R8	#001A
      0xB8222400,  //  000C  GETNGBL	R8	K18
      0x8C201113,  //  000D  GETMET	R8	R8	K19
      0x60280008,  //  000E  GETGBL	R10	G8
      0x882C0119,  //  000F  GETMBR	R11	R0	K25
      0x002C170A,  //  0010  ADD	R11	R11	K10
      0x7C280200,  //  0011  CALL	R10	1
      0x002A3E0A,  //  0012  ADD	R10	K31	R10
      0x502C0200,  //  0013  LDBOOL	R11	1	0
      0x7C200600,  //  0014  CALL	R8	3
      0x8C200120,  //  0015  GETMET	R8	R0	K32
      0x7C200200,  //  0016  CALL	R8	1
      0x50200200,  //  0017  LDBOOL	R8	1	0
      0x80041000,  //  0018  RET	1	R8
      0x7002004D,  //  0019  JMP		#0068
      0x1C200F0A,  //  001A  EQ	R8	R7	K10
      0x7822000D,  //  001B  JMPF	R8	#002A
      0xB8222400,  //  001C  GETNGBL	R8	K18
      0x8C201113,  //  001D  GETMET	R8	R8	K19
      0x60280008,  //  001E  GETGBL	R10	G8
      0x882C0119,  //  001F  GETMBR	R11	R0	K25
      0x002C170A,  //  0020  ADD	R11	R11	K10
      0x7C280200,  //  0021  CALL	R10	1
      0x002A420A,  //  0022  ADD	R10	K33	R10
      0x502C0200,  //  0023  LDBOOL	R11	1	0
      0x7C200600,  //  0024  CALL	R8	3
      0x8C200120,  //  0025  GETMET	R8	R0	K32
      0x7C200200,  //  0026  CALL	R8	1
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x7002003D,  //  0029  JMP		#0068
      0x1C200F0F,  //  002A  EQ	R8	R7	K15
      0x7822000D,  //  002B  JMPF	R8	#003A
      0xB8222400,  //  002C  GETNGBL	R8	K18
      0x8C201113,  //  002D  GETMET	R8	R8	K19
      0x60280008,  //  002E  GETGBL	R10	G8
      0x882C0119,  //  002F  GETMBR	R11	R0	K25
      0x002C170A,  //  0030  ADD	R11	R11	K10
      0x7C280200,  //  0031  CALL	R10	1
      0x002A440A,  //  0032  ADD	R10	K34	R10
      0x502C0200,  //  0033  LDBOOL	R11	1	0
      0x7C200600,  //  0034  CALL	R8	3
      0x8C200120,  //  0035  GETMET	R8	R0	K32
      0x7C200200,  //  0036  CALL	R8	1
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x80041000,  //  0038  RET	1	R8
      0x7002002D,  //  0039  JMP		#0068
      0x54220004,  //  003A  LDINT	R8	5
      0x1C200E08,  //  003B  EQ	R8	R7	R8
      0x7822002A,  //  003C  JMPF	R8	#0068
      0xB8224600,  //  003D  GETNGBL	R8	K35
      0x60240008,  //  003E  GETGBL	R9	G8
      0x5C280400,  //  003F  MOVE	R10	R2
      0x7C240200,  //  0040  CALL	R9	1
      0x00264809,  //  0041  ADD	R9	K36	R9
      0x5828000F,  //  0042  LDCONST	R10	K15
      0x7C200400,  //  0043  CALL	R8	2
      0x8C200525,  //  0044  GETMET	R8	R2	K37
      0x58280006,  //  0045  LDCONST	R10	K6
      0x7C200400,  //  0046  CALL	R8	2
      0x4C240000,  //  0047  LDNIL	R9
      0x20241009,  //  0048  NE	R9	R8	R9
      0x7826001B,  //  0049  JMPF	R9	#0066
      0x54260063,  //  004A  LDINT	R9	100
      0x0C201009,  //  004B  DIV	R8	R8	R9
      0x8824010C,  //  004C  GETMBR	R9	R0	K12
      0x1C241306,  //  004D  EQ	R9	R9	K6
      0x78260001,  //  004E  JMPF	R9	#0051
      0x54260063,  //  004F  LDINT	R9	100
      0x04201208,  //  0050  SUB	R8	R9	R8
      0xB8262400,  //  0051  GETNGBL	R9	K18
      0x8C241313,  //  0052  GETMET	R9	R9	K19
      0x602C0008,  //  0053  GETGBL	R11	G8
      0x88300119,  //  0054  GETMBR	R12	R0	K25
      0x0030190A,  //  0055  ADD	R12	R12	K10
      0x7C2C0200,  //  0056  CALL	R11	1
      0x002E4C0B,  //  0057  ADD	R11	K38	R11
      0x002C1727,  //  0058  ADD	R11	R11	K39
      0x60300008,  //  0059  GETGBL	R12	G8
      0x5C341000,  //  005A  MOVE	R13	R8
      0x7C300200,  //  005B  CALL	R12	1
      0x002C160C,  //  005C  ADD	R11	R11	R12
      0x50300200,  //  005D  LDBOOL	R12	1	0
      0x7C240600,  //  005E  CALL	R9	3
      0x60240008,  //  005F  GETGBL	R9	G8
      0x5C281000,  //  0060  MOVE	R10	R8
      0x7C240200,  //  0061  CALL	R9	1
      0x00265009,  //  0062  ADD	R9	K40	R9
      0x900E4609,  //  0063  SETMBR	R3	K35	R9
      0x8C240120,  //  0064  GETMET	R9	R0	K32
      0x7C240200,  //  0065  CALL	R9	1
      0x50240200,  //  0066  LDBOOL	R9	1	0
      0x80041200,  //  0067  RET	1	R9
      0x70020008,  //  0068  JMP		#0072
      0x60200003,  //  0069  GETGBL	R8	G3
      0x5C240000,  //  006A  MOVE	R9	R0
      0x7C200200,  //  006B  CALL	R8	1
      0x8C201129,  //  006C  GETMET	R8	R8	K41
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
      0x880C0119,  //  0001  GETMBR	R3	R0	K25
      0x000C070A,  //  0002  ADD	R3	R3	K10
      0x7C080200,  //  0003  CALL	R2	1
      0x000A5402,  //  0004  ADD	R2	K42	R2
      0x8C0C0315,  //  0005  GETMET	R3	R1	K21
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E002A,  //  0008  JMPF	R3	#0034
      0x940C0202,  //  0009  GETIDX	R3	R1	R2
      0x8C100717,  //  000A  GETMET	R4	R3	K23
      0x5818002B,  //  000B  LDCONST	R6	K43
      0x7C100400,  //  000C  CALL	R4	2
      0x4C140000,  //  000D  LDNIL	R5
      0x20140805,  //  000E  NE	R5	R4	R5
      0x78160007,  //  000F  JMPF	R5	#0018
      0x8814010B,  //  0010  GETMBR	R5	R0	K11
      0x20140805,  //  0011  NE	R5	R4	R5
      0x78160003,  //  0012  JMPF	R5	#0017
      0x8C14012C,  //  0013  GETMET	R5	R0	K44
      0x541E0101,  //  0014  LDINT	R7	258
      0x5422000D,  //  0015  LDINT	R8	14
      0x7C140600,  //  0016  CALL	R5	3
      0x90021604,  //  0017  SETMBR	R0	K11	R4
      0x8C140717,  //  0018  GETMET	R5	R3	K23
      0x581C002D,  //  0019  LDCONST	R7	K45
      0x7C140400,  //  001A  CALL	R5	2
      0x4C180000,  //  001B  LDNIL	R6
      0x20180A06,  //  001C  NE	R6	R5	R6
      0x781A0007,  //  001D  JMPF	R6	#0026
      0x8818010E,  //  001E  GETMBR	R6	R0	K14
      0x20180A06,  //  001F  NE	R6	R5	R6
      0x781A0003,  //  0020  JMPF	R6	#0025
      0x8C18012C,  //  0021  GETMET	R6	R0	K44
      0x54220101,  //  0022  LDINT	R8	258
      0x54260009,  //  0023  LDINT	R9	10
      0x7C180600,  //  0024  CALL	R6	3
      0x90021C05,  //  0025  SETMBR	R0	K14	R5
      0x8C180717,  //  0026  GETMET	R6	R3	K23
      0x5820002E,  //  0027  LDCONST	R8	K46
      0x7C180400,  //  0028  CALL	R6	2
      0x4C1C0000,  //  0029  LDNIL	R7
      0x201C0C07,  //  002A  NE	R7	R6	R7
      0x781E0007,  //  002B  JMPF	R7	#0034
      0x881C0110,  //  002C  GETMBR	R7	R0	K16
      0x201C0C07,  //  002D  NE	R7	R6	R7
      0x781E0003,  //  002E  JMPF	R7	#0033
      0x8C1C012C,  //  002F  GETMET	R7	R0	K44
      0x54260101,  //  0030  LDINT	R9	258
      0x542A000A,  //  0031  LDINT	R10	11
      0x7C1C0600,  //  0032  CALL	R7	3
      0x90022006,  //  0033  SETMBR	R0	K16	R6
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
      0x8804012F,  //  0000  GETMBR	R1	R0	K47
      0x7406000E,  //  0001  JMPT	R1	#0011
      0x8C040105,  //  0002  GETMET	R1	R0	K5
      0x7C040200,  //  0003  CALL	R1	1
      0xB8062400,  //  0004  GETNGBL	R1	K18
      0x8C040313,  //  0005  GETMET	R1	R1	K19
      0x600C0008,  //  0006  GETGBL	R3	G8
      0x88100119,  //  0007  GETMBR	R4	R0	K25
      0x0010090A,  //  0008  ADD	R4	R4	K10
      0x7C0C0200,  //  0009  CALL	R3	1
      0x000E4C03,  //  000A  ADD	R3	K38	R3
      0x50100200,  //  000B  LDBOOL	R4	1	0
      0x7C040600,  //  000C  CALL	R1	3
      0x78060002,  //  000D  JMPF	R1	#0011
      0x8C080130,  //  000E  GETMET	R2	R0	K48
      0x5C100200,  //  000F  MOVE	R4	R1
      0x7C080400,  //  0010  CALL	R2	2
      0x60040003,  //  0011  GETGBL	R1	G3
      0x5C080000,  //  0012  MOVE	R2	R0
      0x7C040200,  //  0013  CALL	R1	1
      0x8C040320,  //  0014  GETMET	R1	R1	K32
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
