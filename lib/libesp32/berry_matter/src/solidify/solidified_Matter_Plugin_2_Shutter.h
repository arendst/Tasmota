/* Solidification of Matter_Plugin_2_Shutter.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Shutter' ktab size: 47, total: 78 (saved 248 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Shutter[47] = {
  /* K0   */  be_nested_str_weak(tasmota_shutter_index),
  /* K1   */  be_nested_str_weak(find),
  /* K2   */  be_nested_str_weak(ARG),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(shadow_shutter_inverted),
  /* K5   */  be_nested_str_weak(light),
  /* K6   */  be_nested_str_weak(matter),
  /* K7   */  be_nested_str_weak(TLV),
  /* K8   */  be_nested_str_weak(cluster),
  /* K9   */  be_nested_str_weak(command),
  /* K10  */  be_nested_str_weak(update_shadow_lazy),
  /* K11  */  be_nested_str_weak(tasmota),
  /* K12  */  be_nested_str_weak(cmd),
  /* K13  */  be_nested_str_weak(ShutterStopOpen),
  /* K14  */  be_const_int(1),
  /* K15  */  be_nested_str_weak(update_shadow),
  /* K16  */  be_nested_str_weak(ShutterStopClose),
  /* K17  */  be_const_int(2),
  /* K18  */  be_nested_str_weak(ShutterStop),
  /* K19  */  be_nested_str_weak(log),
  /* K20  */  be_nested_str_weak(MTR_X3A_X20Tilt_X20_X3D_X20),
  /* K21  */  be_nested_str_weak(findsubval),
  /* K22  */  be_nested_str_weak(ShutterPosition),
  /* K23  */  be_nested_str_weak(_X20),
  /* K24  */  be_nested_str_weak(pos_X25_X3A),
  /* K25  */  be_nested_str_weak(invoke_request),
  /* K26  */  be_nested_str_weak(attribute),
  /* K27  */  be_nested_str_weak(update_inverted),
  /* K28  */  be_nested_str_weak(set),
  /* K29  */  be_nested_str_weak(U1),
  /* K30  */  be_nested_str_weak(U2),
  /* K31  */  be_nested_str_weak(shadow_shutter_pos),
  /* K32  */  be_nested_str_weak(shadow_shutter_direction),
  /* K33  */  be_nested_str_weak(shadow_shutter_target),
  /* K34  */  be_nested_str_weak(read_attribute),
  /* K35  */  be_nested_str_weak(Shutter),
  /* K36  */  be_nested_str_weak(contains),
  /* K37  */  be_nested_str_weak(Position),
  /* K38  */  be_nested_str_weak(attribute_updated),
  /* K39  */  be_nested_str_weak(Direction),
  /* K40  */  be_nested_str_weak(Target),
  /* K41  */  be_nested_str_weak(Status_X2013),
  /* K42  */  be_nested_str_weak(StatusSHT),
  /* K43  */  be_nested_str_weak(SHT),
  /* K44  */  be_nested_str_weak(Opt),
  /* K45  */  be_nested_str_weak(VIRTUAL),
  /* K46  */  be_nested_str_weak(parse_sensors),
};


extern const bclass be_class_Matter_Plugin_Shutter;

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
    ( &(const binstruction[12]) {  /* code */
      0x8C080301,  //  0000  GETMET	R2	R1	K1
      0x88100102,  //  0001  GETMBR	R4	R0	K2
      0x7C080400,  //  0002  CALL	R2	2
      0x90020002,  //  0003  SETMBR	R0	K0	R2
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C080403,  //  0006  EQ	R2	R2	R3
      0x780A0000,  //  0007  JMPF	R2	#0009
      0x90020103,  //  0008  SETMBR	R0	K0	K3
      0x5409FFFE,  //  0009  LDINT	R2	-1
      0x90020802,  //  000A  SETMBR	R0	K4	R2
      0x80000000,  //  000B  RET	0
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
      0xA4120A00,  //  0000  IMPORT	R4	K5
      0xB8160C00,  //  0001  GETNGBL	R5	K6
      0x88140B07,  //  0002  GETMBR	R5	R5	K7
      0x88180708,  //  0003  GETMBR	R6	R3	K8
      0x881C0709,  //  0004  GETMBR	R7	R3	K9
      0x54220101,  //  0005  LDINT	R8	258
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220060,  //  0007  JMPF	R8	#0069
      0x8C20010A,  //  0008  GETMET	R8	R0	K10
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F03,  //  000A  EQ	R8	R7	K3
      0x7822000D,  //  000B  JMPF	R8	#001A
      0xB8221600,  //  000C  GETNGBL	R8	K11
      0x8C20110C,  //  000D  GETMET	R8	R8	K12
      0x60280008,  //  000E  GETGBL	R10	G8
      0x882C0100,  //  000F  GETMBR	R11	R0	K0
      0x002C170E,  //  0010  ADD	R11	R11	K14
      0x7C280200,  //  0011  CALL	R10	1
      0x002A1A0A,  //  0012  ADD	R10	K13	R10
      0x502C0200,  //  0013  LDBOOL	R11	1	0
      0x7C200600,  //  0014  CALL	R8	3
      0x8C20010F,  //  0015  GETMET	R8	R0	K15
      0x7C200200,  //  0016  CALL	R8	1
      0x50200200,  //  0017  LDBOOL	R8	1	0
      0x80041000,  //  0018  RET	1	R8
      0x7002004D,  //  0019  JMP		#0068
      0x1C200F0E,  //  001A  EQ	R8	R7	K14
      0x7822000D,  //  001B  JMPF	R8	#002A
      0xB8221600,  //  001C  GETNGBL	R8	K11
      0x8C20110C,  //  001D  GETMET	R8	R8	K12
      0x60280008,  //  001E  GETGBL	R10	G8
      0x882C0100,  //  001F  GETMBR	R11	R0	K0
      0x002C170E,  //  0020  ADD	R11	R11	K14
      0x7C280200,  //  0021  CALL	R10	1
      0x002A200A,  //  0022  ADD	R10	K16	R10
      0x502C0200,  //  0023  LDBOOL	R11	1	0
      0x7C200600,  //  0024  CALL	R8	3
      0x8C20010F,  //  0025  GETMET	R8	R0	K15
      0x7C200200,  //  0026  CALL	R8	1
      0x50200200,  //  0027  LDBOOL	R8	1	0
      0x80041000,  //  0028  RET	1	R8
      0x7002003D,  //  0029  JMP		#0068
      0x1C200F11,  //  002A  EQ	R8	R7	K17
      0x7822000D,  //  002B  JMPF	R8	#003A
      0xB8221600,  //  002C  GETNGBL	R8	K11
      0x8C20110C,  //  002D  GETMET	R8	R8	K12
      0x60280008,  //  002E  GETGBL	R10	G8
      0x882C0100,  //  002F  GETMBR	R11	R0	K0
      0x002C170E,  //  0030  ADD	R11	R11	K14
      0x7C280200,  //  0031  CALL	R10	1
      0x002A240A,  //  0032  ADD	R10	K18	R10
      0x502C0200,  //  0033  LDBOOL	R11	1	0
      0x7C200600,  //  0034  CALL	R8	3
      0x8C20010F,  //  0035  GETMET	R8	R0	K15
      0x7C200200,  //  0036  CALL	R8	1
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x80041000,  //  0038  RET	1	R8
      0x7002002D,  //  0039  JMP		#0068
      0x54220004,  //  003A  LDINT	R8	5
      0x1C200E08,  //  003B  EQ	R8	R7	R8
      0x7822002A,  //  003C  JMPF	R8	#0068
      0xB8222600,  //  003D  GETNGBL	R8	K19
      0x60240008,  //  003E  GETGBL	R9	G8
      0x5C280400,  //  003F  MOVE	R10	R2
      0x7C240200,  //  0040  CALL	R9	1
      0x00262809,  //  0041  ADD	R9	K20	R9
      0x58280011,  //  0042  LDCONST	R10	K17
      0x7C200400,  //  0043  CALL	R8	2
      0x8C200515,  //  0044  GETMET	R8	R2	K21
      0x58280003,  //  0045  LDCONST	R10	K3
      0x7C200400,  //  0046  CALL	R8	2
      0x4C240000,  //  0047  LDNIL	R9
      0x20241009,  //  0048  NE	R9	R8	R9
      0x7826001B,  //  0049  JMPF	R9	#0066
      0x54260063,  //  004A  LDINT	R9	100
      0x0C201009,  //  004B  DIV	R8	R8	R9
      0x88240104,  //  004C  GETMBR	R9	R0	K4
      0x1C241303,  //  004D  EQ	R9	R9	K3
      0x78260001,  //  004E  JMPF	R9	#0051
      0x54260063,  //  004F  LDINT	R9	100
      0x04201208,  //  0050  SUB	R8	R9	R8
      0xB8261600,  //  0051  GETNGBL	R9	K11
      0x8C24130C,  //  0052  GETMET	R9	R9	K12
      0x602C0008,  //  0053  GETGBL	R11	G8
      0x88300100,  //  0054  GETMBR	R12	R0	K0
      0x0030190E,  //  0055  ADD	R12	R12	K14
      0x7C2C0200,  //  0056  CALL	R11	1
      0x002E2C0B,  //  0057  ADD	R11	K22	R11
      0x002C1717,  //  0058  ADD	R11	R11	K23
      0x60300008,  //  0059  GETGBL	R12	G8
      0x5C341000,  //  005A  MOVE	R13	R8
      0x7C300200,  //  005B  CALL	R12	1
      0x002C160C,  //  005C  ADD	R11	R11	R12
      0x50300200,  //  005D  LDBOOL	R12	1	0
      0x7C240600,  //  005E  CALL	R9	3
      0x60240008,  //  005F  GETGBL	R9	G8
      0x5C281000,  //  0060  MOVE	R10	R8
      0x7C240200,  //  0061  CALL	R9	1
      0x00263009,  //  0062  ADD	R9	K24	R9
      0x900E2609,  //  0063  SETMBR	R3	K19	R9
      0x8C24010F,  //  0064  GETMET	R9	R0	K15
      0x7C240200,  //  0065  CALL	R9	1
      0x50240200,  //  0066  LDBOOL	R9	1	0
      0x80041200,  //  0067  RET	1	R9
      0x70020008,  //  0068  JMP		#0072
      0x60200003,  //  0069  GETGBL	R8	G3
      0x5C240000,  //  006A  MOVE	R9	R0
      0x7C200200,  //  006B  CALL	R8	1
      0x8C201119,  //  006C  GETMET	R8	R8	K25
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
    ( &(const binstruction[131]) {  /* code */
      0xB8120C00,  //  0000  GETNGBL	R4	K6
      0x88100907,  //  0001  GETMBR	R4	R4	K7
      0x88140508,  //  0002  GETMBR	R5	R2	K8
      0x8818051A,  //  0003  GETMBR	R6	R2	K26
      0x4C1C0000,  //  0004  LDNIL	R7
      0x54220101,  //  0005  LDINT	R8	258
      0x1C200A08,  //  0006  EQ	R8	R5	R8
      0x78220071,  //  0007  JMPF	R8	#007A
      0x8C20010A,  //  0008  GETMET	R8	R0	K10
      0x7C200200,  //  0009  CALL	R8	1
      0x8C20011B,  //  000A  GETMET	R8	R0	K27
      0x7C200200,  //  000B  CALL	R8	1
      0x1C200D03,  //  000C  EQ	R8	R6	K3
      0x78220005,  //  000D  JMPF	R8	#0014
      0x8C20071C,  //  000E  GETMET	R8	R3	K28
      0x8828091D,  //  000F  GETMBR	R10	R4	K29
      0x542E00FE,  //  0010  LDINT	R11	255
      0x7C200600,  //  0011  CALL	R8	3
      0x80041000,  //  0012  RET	1	R8
      0x70020065,  //  0013  JMP		#007A
      0x54220004,  //  0014  LDINT	R8	5
      0x1C200C08,  //  0015  EQ	R8	R6	R8
      0x78220005,  //  0016  JMPF	R8	#001D
      0x8C20071C,  //  0017  GETMET	R8	R3	K28
      0x8828091E,  //  0018  GETMBR	R10	R4	K30
      0x582C0003,  //  0019  LDCONST	R11	K3
      0x7C200600,  //  001A  CALL	R8	3
      0x80041000,  //  001B  RET	1	R8
      0x7002005C,  //  001C  JMP		#007A
      0x54220006,  //  001D  LDINT	R8	7
      0x1C200C08,  //  001E  EQ	R8	R6	R8
      0x78220006,  //  001F  JMPF	R8	#0027
      0x8C20071C,  //  0020  GETMET	R8	R3	K28
      0x8828091D,  //  0021  GETMBR	R10	R4	K29
      0x542E0007,  //  0022  LDINT	R11	8
      0x002E1C0B,  //  0023  ADD	R11	K14	R11
      0x7C200600,  //  0024  CALL	R8	3
      0x80041000,  //  0025  RET	1	R8
      0x70020052,  //  0026  JMP		#007A
      0x5422000C,  //  0027  LDINT	R8	13
      0x1C200C08,  //  0028  EQ	R8	R6	R8
      0x78220005,  //  0029  JMPF	R8	#0030
      0x8C20071C,  //  002A  GETMET	R8	R3	K28
      0x8828091D,  //  002B  GETMBR	R10	R4	K29
      0x542E00FE,  //  002C  LDINT	R11	255
      0x7C200600,  //  002D  CALL	R8	3
      0x80041000,  //  002E  RET	1	R8
      0x70020049,  //  002F  JMP		#007A
      0x5422000D,  //  0030  LDINT	R8	14
      0x1C200C08,  //  0031  EQ	R8	R6	R8
      0x78220013,  //  0032  JMPF	R8	#0047
      0x88200104,  //  0033  GETMBR	R8	R0	K4
      0x1C201103,  //  0034  EQ	R8	R8	K3
      0x78220006,  //  0035  JMPF	R8	#003D
      0x54220063,  //  0036  LDINT	R8	100
      0x8824011F,  //  0037  GETMBR	R9	R0	K31
      0x04201009,  //  0038  SUB	R8	R8	R9
      0x54260063,  //  0039  LDINT	R9	100
      0x08201009,  //  003A  MUL	R8	R8	R9
      0x5C1C1000,  //  003B  MOVE	R7	R8
      0x70020003,  //  003C  JMP		#0041
      0x8820011F,  //  003D  GETMBR	R8	R0	K31
      0x54260063,  //  003E  LDINT	R9	100
      0x08201009,  //  003F  MUL	R8	R8	R9
      0x5C1C1000,  //  0040  MOVE	R7	R8
      0x8C20071C,  //  0041  GETMET	R8	R3	K28
      0x8828091E,  //  0042  GETMBR	R10	R4	K30
      0x5C2C0E00,  //  0043  MOVE	R11	R7
      0x7C200600,  //  0044  CALL	R8	3
      0x80041000,  //  0045  RET	1	R8
      0x70020032,  //  0046  JMP		#007A
      0x54220009,  //  0047  LDINT	R8	10
      0x1C200C08,  //  0048  EQ	R8	R6	R8
      0x78220010,  //  0049  JMPF	R8	#005B
      0x88200120,  //  004A  GETMBR	R8	R0	K32
      0x1C201103,  //  004B  EQ	R8	R8	K3
      0x78220001,  //  004C  JMPF	R8	#004F
      0x58200003,  //  004D  LDCONST	R8	K3
      0x70020005,  //  004E  JMP		#0055
      0x88200120,  //  004F  GETMBR	R8	R0	K32
      0x24201103,  //  0050  GT	R8	R8	K3
      0x78220001,  //  0051  JMPF	R8	#0054
      0x5820000E,  //  0052  LDCONST	R8	K14
      0x70020000,  //  0053  JMP		#0055
      0x58200011,  //  0054  LDCONST	R8	K17
      0x8C24071C,  //  0055  GETMET	R9	R3	K28
      0x882C091D,  //  0056  GETMBR	R11	R4	K29
      0x5C301000,  //  0057  MOVE	R12	R8
      0x7C240600,  //  0058  CALL	R9	3
      0x80041200,  //  0059  RET	1	R9
      0x7002001E,  //  005A  JMP		#007A
      0x5422000A,  //  005B  LDINT	R8	11
      0x1C200C08,  //  005C  EQ	R8	R6	R8
      0x78220013,  //  005D  JMPF	R8	#0072
      0x88200104,  //  005E  GETMBR	R8	R0	K4
      0x1C201103,  //  005F  EQ	R8	R8	K3
      0x78220006,  //  0060  JMPF	R8	#0068
      0x54220063,  //  0061  LDINT	R8	100
      0x88240121,  //  0062  GETMBR	R9	R0	K33
      0x04201009,  //  0063  SUB	R8	R8	R9
      0x54260063,  //  0064  LDINT	R9	100
      0x08201009,  //  0065  MUL	R8	R8	R9
      0x5C1C1000,  //  0066  MOVE	R7	R8
      0x70020003,  //  0067  JMP		#006C
      0x88200121,  //  0068  GETMBR	R8	R0	K33
      0x54260063,  //  0069  LDINT	R9	100
      0x08201009,  //  006A  MUL	R8	R8	R9
      0x5C1C1000,  //  006B  MOVE	R7	R8
      0x8C20071C,  //  006C  GETMET	R8	R3	K28
      0x8828091E,  //  006D  GETMBR	R10	R4	K30
      0x5C2C0E00,  //  006E  MOVE	R11	R7
      0x7C200600,  //  006F  CALL	R8	3
      0x80041000,  //  0070  RET	1	R8
      0x70020007,  //  0071  JMP		#007A
      0x54220016,  //  0072  LDINT	R8	23
      0x1C200C08,  //  0073  EQ	R8	R6	R8
      0x78220004,  //  0074  JMPF	R8	#007A
      0x8C20071C,  //  0075  GETMET	R8	R3	K28
      0x8828091D,  //  0076  GETMBR	R10	R4	K29
      0x582C0003,  //  0077  LDCONST	R11	K3
      0x7C200600,  //  0078  CALL	R8	3
      0x80041000,  //  0079  RET	1	R8
      0x60200003,  //  007A  GETGBL	R8	G3
      0x5C240000,  //  007B  MOVE	R9	R0
      0x7C200200,  //  007C  CALL	R8	1
      0x8C201122,  //  007D  GETMET	R8	R8	K34
      0x5C280200,  //  007E  MOVE	R10	R1
      0x5C2C0400,  //  007F  MOVE	R11	R2
      0x5C300600,  //  0080  MOVE	R12	R3
      0x7C200800,  //  0081  CALL	R8	4
      0x80041000,  //  0082  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(class_Matter_Plugin_Shutter__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    8,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Shutter,     /* shared constants */
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
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x000C070E,  //  0002  ADD	R3	R3	K14
      0x7C080200,  //  0003  CALL	R2	1
      0x000A4602,  //  0004  ADD	R2	K35	R2
      0x8C0C0324,  //  0005  GETMET	R3	R1	K36
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E002A,  //  0008  JMPF	R3	#0034
      0x940C0202,  //  0009  GETIDX	R3	R1	R2
      0x8C100701,  //  000A  GETMET	R4	R3	K1
      0x58180025,  //  000B  LDCONST	R6	K37
      0x7C100400,  //  000C  CALL	R4	2
      0x4C140000,  //  000D  LDNIL	R5
      0x20140805,  //  000E  NE	R5	R4	R5
      0x78160007,  //  000F  JMPF	R5	#0018
      0x8814011F,  //  0010  GETMBR	R5	R0	K31
      0x20140805,  //  0011  NE	R5	R4	R5
      0x78160003,  //  0012  JMPF	R5	#0017
      0x8C140126,  //  0013  GETMET	R5	R0	K38
      0x541E0101,  //  0014  LDINT	R7	258
      0x5422000D,  //  0015  LDINT	R8	14
      0x7C140600,  //  0016  CALL	R5	3
      0x90023E04,  //  0017  SETMBR	R0	K31	R4
      0x8C140701,  //  0018  GETMET	R5	R3	K1
      0x581C0027,  //  0019  LDCONST	R7	K39
      0x7C140400,  //  001A  CALL	R5	2
      0x4C180000,  //  001B  LDNIL	R6
      0x20180A06,  //  001C  NE	R6	R5	R6
      0x781A0007,  //  001D  JMPF	R6	#0026
      0x88180120,  //  001E  GETMBR	R6	R0	K32
      0x20180A06,  //  001F  NE	R6	R5	R6
      0x781A0003,  //  0020  JMPF	R6	#0025
      0x8C180126,  //  0021  GETMET	R6	R0	K38
      0x54220101,  //  0022  LDINT	R8	258
      0x54260009,  //  0023  LDINT	R9	10
      0x7C180600,  //  0024  CALL	R6	3
      0x90024005,  //  0025  SETMBR	R0	K32	R5
      0x8C180701,  //  0026  GETMET	R6	R3	K1
      0x58200028,  //  0027  LDCONST	R8	K40
      0x7C180400,  //  0028  CALL	R6	2
      0x4C1C0000,  //  0029  LDNIL	R7
      0x201C0C07,  //  002A  NE	R7	R6	R7
      0x781E0007,  //  002B  JMPF	R7	#0034
      0x881C0121,  //  002C  GETMBR	R7	R0	K33
      0x201C0C07,  //  002D  NE	R7	R6	R7
      0x781E0003,  //  002E  JMPF	R7	#0033
      0x8C1C0126,  //  002F  GETMET	R7	R0	K38
      0x54260101,  //  0030  LDINT	R9	258
      0x542A000A,  //  0031  LDINT	R10	11
      0x7C1C0600,  //  0032  CALL	R7	3
      0x90024206,  //  0033  SETMBR	R0	K33	R6
      0x80000000,  //  0034  RET	0
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
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x5409FFFE,  //  0001  LDINT	R2	-1
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806001F,  //  0003  JMPF	R1	#0024
      0xB8061600,  //  0004  GETNGBL	R1	K11
      0x8C04030C,  //  0005  GETMET	R1	R1	K12
      0x580C0029,  //  0006  LDCONST	R3	K41
      0x50100200,  //  0007  LDBOOL	R4	1	0
      0x7C040600,  //  0008  CALL	R1	3
      0x8C080324,  //  0009  GETMET	R2	R1	K36
      0x5810002A,  //  000A  LDCONST	R4	K42
      0x7C080400,  //  000B  CALL	R2	2
      0x780A0016,  //  000C  JMPF	R2	#0024
      0x9404032A,  //  000D  GETIDX	R1	R1	K42
      0x8C080301,  //  000E  GETMET	R2	R1	K1
      0x60100008,  //  000F  GETGBL	R4	G8
      0x88140100,  //  0010  GETMBR	R5	R0	K0
      0x7C100200,  //  0011  CALL	R4	1
      0x00125604,  //  0012  ADD	R4	K43	R4
      0x60140013,  //  0013  GETGBL	R5	G19
      0x7C140000,  //  0014  CALL	R5	0
      0x7C080600,  //  0015  CALL	R2	3
      0x8C080501,  //  0016  GETMET	R2	R2	K1
      0x5810002C,  //  0017  LDCONST	R4	K44
      0x7C080400,  //  0018  CALL	R2	2
      0x4C0C0000,  //  0019  LDNIL	R3
      0x200C0403,  //  001A  NE	R3	R2	R3
      0x780E0007,  //  001B  JMPF	R3	#0024
      0x600C0009,  //  001C  GETGBL	R3	G9
      0x6010000C,  //  001D  GETGBL	R4	G12
      0x5C140400,  //  001E  MOVE	R5	R2
      0x7C100200,  //  001F  CALL	R4	1
      0x0410090E,  //  0020  SUB	R4	R4	K14
      0x94100404,  //  0021  GETIDX	R4	R2	R4
      0x7C0C0200,  //  0022  CALL	R3	1
      0x90020803,  //  0023  SETMBR	R0	K4	R3
      0x80000000,  //  0024  RET	0
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
      0x8C04011B,  //  0002  GETMET	R1	R0	K27
      0x7C040200,  //  0003  CALL	R1	1
      0xB8061600,  //  0004  GETNGBL	R1	K11
      0x8C04030C,  //  0005  GETMET	R1	R1	K12
      0x600C0008,  //  0006  GETGBL	R3	G8
      0x88100100,  //  0007  GETMBR	R4	R0	K0
      0x0010090E,  //  0008  ADD	R4	R4	K14
      0x7C0C0200,  //  0009  CALL	R3	1
      0x000E2C03,  //  000A  ADD	R3	K22	R3
      0x50100200,  //  000B  LDBOOL	R4	1	0
      0x7C040600,  //  000C  CALL	R1	3
      0x78060002,  //  000D  JMPF	R1	#0011
      0x8C08012E,  //  000E  GETMET	R2	R0	K46
      0x5C100200,  //  000F  MOVE	R4	R1
      0x7C080400,  //  0010  CALL	R2	2
      0x60040003,  //  0011  GETGBL	R1	G3
      0x5C080000,  //  0012  MOVE	R2	R0
      0x7C040200,  //  0013  CALL	R1	1
      0x8C04030F,  //  0014  GETMET	R1	R1	K15
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
    be_nested_map(18,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(shadow_shutter_inverted, -1), be_const_var(4) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(shutter) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(514, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(shadow_shutter_target, -1), be_const_var(2) },
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
        { be_const_key_weak(invoke_request, 3), be_const_closure(class_Matter_Plugin_Shutter_invoke_request_closure) },
        { be_const_key_weak(read_attribute, 11), be_const_closure(class_Matter_Plugin_Shutter_read_attribute_closure) },
        { be_const_key_weak(parse_configuration, 14), be_const_closure(class_Matter_Plugin_Shutter_parse_configuration_closure) },
        { be_const_key_weak(shadow_shutter_pos, -1), be_const_var(1) },
        { be_const_key_weak(ARG, 13), be_nested_str_weak(shutter) },
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(class_Matter_Plugin_Shutter__X3Clambda_X3E_closure) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(class_Matter_Plugin_Shutter_parse_sensors_closure) },
        { be_const_key_weak(update_inverted, -1), be_const_closure(class_Matter_Plugin_Shutter_update_inverted_closure) },
        { be_const_key_weak(ARG_HINT, 16), be_nested_str_weak(Relay_X3Cx_X3E_X20number) },
        { be_const_key_weak(update_shadow, 0), be_const_closure(class_Matter_Plugin_Shutter_update_shadow_closure) },
        { be_const_key_weak(tasmota_shutter_index, -1), be_const_var(0) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Shutter) },
        { be_const_key_weak(shadow_shutter_direction, -1), be_const_var(3) },
    })),
    be_str_weak(Matter_Plugin_Shutter)
);
/********************************************************************/
/* End of solidification */
