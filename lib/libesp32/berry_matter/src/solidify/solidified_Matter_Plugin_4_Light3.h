/* Solidification of Matter_Plugin_4_Light3.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light3;

/********************************************************************
** Solidified function: invoke_request
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_closure(class_Matter_Plugin_Light3_invoke_request,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light3, 
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_nested_str_weak(set_hue_sat),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(hue_X3A),
    /* K10  */  be_nested_str_weak(publish_command),
    /* K11  */  be_nested_str_weak(Hue),
    /* K12  */  be_const_int(1),
    /* K13  */  be_const_int(2),
    /* K14  */  be_const_int(3),
    /* K15  */  be_nested_str_weak(sat_X3A),
    /* K16  */  be_nested_str_weak(Sat),
    /* K17  */  be_nested_str_weak(_X20sat_X3A),
    /* K18  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[121]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0067,  //  0006  JMPF	R7	#006F
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0012,  //  000A  JMPF	R7	#001E
      0x8C1C0506,  //  000B  GETMET	R7	R2	K6
      0x58240005,  //  000C  LDCONST	R9	K5
      0x7C1C0400,  //  000D  CALL	R7	2
      0x8C200107,  //  000E  GETMET	R8	R0	K7
      0x5C280E00,  //  000F  MOVE	R10	R7
      0x4C2C0000,  //  0010  LDNIL	R11
      0x7C200600,  //  0011  CALL	R8	3
      0x60200008,  //  0012  GETGBL	R8	G8
      0x5C240E00,  //  0013  MOVE	R9	R7
      0x7C200200,  //  0014  CALL	R8	1
      0x00221208,  //  0015  ADD	R8	K9	R8
      0x900E1008,  //  0016  SETMBR	R3	K8	R8
      0x8C20010A,  //  0017  GETMET	R8	R0	K10
      0x5828000B,  //  0018  LDCONST	R10	K11
      0x5C2C0E00,  //  0019  MOVE	R11	R7
      0x7C200600,  //  001A  CALL	R8	3
      0x50200200,  //  001B  LDBOOL	R8	1	0
      0x80041000,  //  001C  RET	1	R8
      0x7002004F,  //  001D  JMP		#006E
      0x1C1C0D0C,  //  001E  EQ	R7	R6	K12
      0x781E0002,  //  001F  JMPF	R7	#0023
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x80040E00,  //  0021  RET	1	R7
      0x7002004A,  //  0022  JMP		#006E
      0x1C1C0D0D,  //  0023  EQ	R7	R6	K13
      0x781E0002,  //  0024  JMPF	R7	#0028
      0x501C0200,  //  0025  LDBOOL	R7	1	0
      0x80040E00,  //  0026  RET	1	R7
      0x70020045,  //  0027  JMP		#006E
      0x1C1C0D0E,  //  0028  EQ	R7	R6	K14
      0x781E0012,  //  0029  JMPF	R7	#003D
      0x8C1C0506,  //  002A  GETMET	R7	R2	K6
      0x58240005,  //  002B  LDCONST	R9	K5
      0x7C1C0400,  //  002C  CALL	R7	2
      0x8C200107,  //  002D  GETMET	R8	R0	K7
      0x4C280000,  //  002E  LDNIL	R10
      0x5C2C0E00,  //  002F  MOVE	R11	R7
      0x7C200600,  //  0030  CALL	R8	3
      0x60200008,  //  0031  GETGBL	R8	G8
      0x5C240E00,  //  0032  MOVE	R9	R7
      0x7C200200,  //  0033  CALL	R8	1
      0x00221E08,  //  0034  ADD	R8	K15	R8
      0x900E1008,  //  0035  SETMBR	R3	K8	R8
      0x8C20010A,  //  0036  GETMET	R8	R0	K10
      0x58280010,  //  0037  LDCONST	R10	K16
      0x5C2C0E00,  //  0038  MOVE	R11	R7
      0x7C200600,  //  0039  CALL	R8	3
      0x50200200,  //  003A  LDBOOL	R8	1	0
      0x80041000,  //  003B  RET	1	R8
      0x70020030,  //  003C  JMP		#006E
      0x541E0003,  //  003D  LDINT	R7	4
      0x1C1C0C07,  //  003E  EQ	R7	R6	R7
      0x781E0002,  //  003F  JMPF	R7	#0043
      0x501C0200,  //  0040  LDBOOL	R7	1	0
      0x80040E00,  //  0041  RET	1	R7
      0x7002002A,  //  0042  JMP		#006E
      0x541E0004,  //  0043  LDINT	R7	5
      0x1C1C0C07,  //  0044  EQ	R7	R6	R7
      0x781E0002,  //  0045  JMPF	R7	#0049
      0x501C0200,  //  0046  LDBOOL	R7	1	0
      0x80040E00,  //  0047  RET	1	R7
      0x70020024,  //  0048  JMP		#006E
      0x541E0005,  //  0049  LDINT	R7	6
      0x1C1C0C07,  //  004A  EQ	R7	R6	R7
      0x781E001C,  //  004B  JMPF	R7	#0069
      0x8C1C0506,  //  004C  GETMET	R7	R2	K6
      0x58240005,  //  004D  LDCONST	R9	K5
      0x7C1C0400,  //  004E  CALL	R7	2
      0x8C200506,  //  004F  GETMET	R8	R2	K6
      0x5828000C,  //  0050  LDCONST	R10	K12
      0x7C200400,  //  0051  CALL	R8	2
      0x8C240107,  //  0052  GETMET	R9	R0	K7
      0x5C2C0E00,  //  0053  MOVE	R11	R7
      0x5C301000,  //  0054  MOVE	R12	R8
      0x7C240600,  //  0055  CALL	R9	3
      0x60240008,  //  0056  GETGBL	R9	G8
      0x5C280E00,  //  0057  MOVE	R10	R7
      0x7C240200,  //  0058  CALL	R9	1
      0x00261209,  //  0059  ADD	R9	K9	R9
      0x00241311,  //  005A  ADD	R9	R9	K17
      0x60280008,  //  005B  GETGBL	R10	G8
      0x5C2C1000,  //  005C  MOVE	R11	R8
      0x7C280200,  //  005D  CALL	R10	1
      0x0024120A,  //  005E  ADD	R9	R9	R10
      0x900E1009,  //  005F  SETMBR	R3	K8	R9
      0x8C24010A,  //  0060  GETMET	R9	R0	K10
      0x582C000B,  //  0061  LDCONST	R11	K11
      0x5C300E00,  //  0062  MOVE	R12	R7
      0x58340010,  //  0063  LDCONST	R13	K16
      0x5C381000,  //  0064  MOVE	R14	R8
      0x7C240A00,  //  0065  CALL	R9	5
      0x50240200,  //  0066  LDBOOL	R9	1	0
      0x80041200,  //  0067  RET	1	R9
      0x70020004,  //  0068  JMP		#006E
      0x541E0046,  //  0069  LDINT	R7	71
      0x1C1C0C07,  //  006A  EQ	R7	R6	R7
      0x781E0001,  //  006B  JMPF	R7	#006E
      0x501C0200,  //  006C  LDBOOL	R7	1	0
      0x80040E00,  //  006D  RET	1	R7
      0x70020008,  //  006E  JMP		#0078
      0x601C0003,  //  006F  GETGBL	R7	G3
      0x5C200000,  //  0070  MOVE	R8	R0
      0x7C1C0200,  //  0071  CALL	R7	1
      0x8C1C0F12,  //  0072  GETMET	R7	R7	K18
      0x5C240200,  //  0073  MOVE	R9	R1
      0x5C280400,  //  0074  MOVE	R10	R2
      0x5C2C0600,  //  0075  MOVE	R11	R3
      0x7C1C0800,  //  0076  CALL	R7	4
      0x80040E00,  //  0077  RET	1	R7
      0x80000000,  //  0078  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_closure(class_Matter_Plugin_Light3_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light3, 
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(set_or_nil),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(shadow_hue),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(shadow_sat),
    /* K11  */  be_nested_str_weak(set),
    /* K12  */  be_nested_str_weak(U4),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[96]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E004F,  //  0006  JMPF	R7	#0057
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0005,  //  000A  JMPF	R7	#0011
      0x8C1C0706,  //  000B  GETMET	R7	R3	K6
      0x88240907,  //  000C  GETMBR	R9	R4	K7
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x70020045,  //  0010  JMP		#0057
      0x1C1C0D09,  //  0011  EQ	R7	R6	K9
      0x781E0005,  //  0012  JMPF	R7	#0019
      0x8C1C0706,  //  0013  GETMET	R7	R3	K6
      0x88240907,  //  0014  GETMBR	R9	R4	K7
      0x8828010A,  //  0015  GETMBR	R10	R0	K10
      0x7C1C0600,  //  0016  CALL	R7	3
      0x80040E00,  //  0017  RET	1	R7
      0x7002003D,  //  0018  JMP		#0057
      0x541E0006,  //  0019  LDINT	R7	7
      0x1C1C0C07,  //  001A  EQ	R7	R6	R7
      0x781E0005,  //  001B  JMPF	R7	#0022
      0x8C1C070B,  //  001C  GETMET	R7	R3	K11
      0x88240907,  //  001D  GETMBR	R9	R4	K7
      0x58280005,  //  001E  LDCONST	R10	K5
      0x7C1C0600,  //  001F  CALL	R7	3
      0x80040E00,  //  0020  RET	1	R7
      0x70020034,  //  0021  JMP		#0057
      0x541E0007,  //  0022  LDINT	R7	8
      0x1C1C0C07,  //  0023  EQ	R7	R6	R7
      0x781E0005,  //  0024  JMPF	R7	#002B
      0x8C1C070B,  //  0025  GETMET	R7	R3	K11
      0x88240907,  //  0026  GETMBR	R9	R4	K7
      0x58280005,  //  0027  LDCONST	R10	K5
      0x7C1C0600,  //  0028  CALL	R7	3
      0x80040E00,  //  0029  RET	1	R7
      0x7002002B,  //  002A  JMP		#0057
      0x541E000E,  //  002B  LDINT	R7	15
      0x1C1C0C07,  //  002C  EQ	R7	R6	R7
      0x781E0005,  //  002D  JMPF	R7	#0034
      0x8C1C070B,  //  002E  GETMET	R7	R3	K11
      0x88240907,  //  002F  GETMBR	R9	R4	K7
      0x58280005,  //  0030  LDCONST	R10	K5
      0x7C1C0600,  //  0031  CALL	R7	3
      0x80040E00,  //  0032  RET	1	R7
      0x70020022,  //  0033  JMP		#0057
      0x541E4000,  //  0034  LDINT	R7	16385
      0x1C1C0C07,  //  0035  EQ	R7	R6	R7
      0x781E0005,  //  0036  JMPF	R7	#003D
      0x8C1C070B,  //  0037  GETMET	R7	R3	K11
      0x88240907,  //  0038  GETMBR	R9	R4	K7
      0x58280005,  //  0039  LDCONST	R10	K5
      0x7C1C0600,  //  003A  CALL	R7	3
      0x80040E00,  //  003B  RET	1	R7
      0x70020019,  //  003C  JMP		#0057
      0x541E4009,  //  003D  LDINT	R7	16394
      0x1C1C0C07,  //  003E  EQ	R7	R6	R7
      0x781E0005,  //  003F  JMPF	R7	#0046
      0x8C1C070B,  //  0040  GETMET	R7	R3	K11
      0x88240907,  //  0041  GETMBR	R9	R4	K7
      0x58280009,  //  0042  LDCONST	R10	K9
      0x7C1C0600,  //  0043  CALL	R7	3
      0x80040E00,  //  0044  RET	1	R7
      0x70020010,  //  0045  JMP		#0057
      0x541E000F,  //  0046  LDINT	R7	16
      0x1C1C0C07,  //  0047  EQ	R7	R6	R7
      0x781E0005,  //  0048  JMPF	R7	#004F
      0x8C1C070B,  //  0049  GETMET	R7	R3	K11
      0x88240907,  //  004A  GETMBR	R9	R4	K7
      0x58280005,  //  004B  LDCONST	R10	K5
      0x7C1C0600,  //  004C  CALL	R7	3
      0x80040E00,  //  004D  RET	1	R7
      0x70020007,  //  004E  JMP		#0057
      0x541EFFFB,  //  004F  LDINT	R7	65532
      0x1C1C0C07,  //  0050  EQ	R7	R6	R7
      0x781E0004,  //  0051  JMPF	R7	#0057
      0x8C1C070B,  //  0052  GETMET	R7	R3	K11
      0x8824090C,  //  0053  GETMBR	R9	R4	K12
      0x58280009,  //  0054  LDCONST	R10	K9
      0x7C1C0600,  //  0055  CALL	R7	3
      0x80040E00,  //  0056  RET	1	R7
      0x601C0003,  //  0057  GETGBL	R7	G3
      0x5C200000,  //  0058  MOVE	R8	R0
      0x7C1C0200,  //  0059  CALL	R7	1
      0x8C1C0F0D,  //  005A  GETMET	R7	R7	K13
      0x5C240200,  //  005B  MOVE	R9	R1
      0x5C280400,  //  005C  MOVE	R10	R2
      0x5C2C0600,  //  005D  MOVE	R11	R3
      0x7C1C0800,  //  005E  CALL	R7	4
      0x80040E00,  //  005F  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_closure(class_Matter_Plugin_Light3_update_shadow,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light3, 
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(VIRTUAL),
    /* K1   */  be_nested_str_weak(BRIDGE),
    /* K2   */  be_nested_str_weak(light),
    /* K3   */  be_nested_str_weak(update_shadow),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_nested_str_weak(light_index),
    /* K6   */  be_nested_str_weak(find),
    /* K7   */  be_nested_str_weak(hue),
    /* K8   */  be_nested_str_weak(sat),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(scale_uint),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(shadow_hue),
    /* K13  */  be_nested_str_weak(shadow_sat),
    /* K14  */  be_nested_str_weak(attribute_updated),
    /* K15  */  be_const_int(1),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060042,  //  0001  JMPT	R1	#0045
      0x88040101,  //  0002  GETMBR	R1	R0	K1
      0x74060040,  //  0003  JMPT	R1	#0045
      0xA4060400,  //  0004  IMPORT	R1	K2
      0x60080003,  //  0005  GETGBL	R2	G3
      0x5C0C0000,  //  0006  MOVE	R3	R0
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x7C080200,  //  0009  CALL	R2	1
      0x8C080304,  //  000A  GETMET	R2	R1	K4
      0x88100105,  //  000B  GETMBR	R4	R0	K5
      0x7C080400,  //  000C  CALL	R2	2
      0x4C0C0000,  //  000D  LDNIL	R3
      0x200C0403,  //  000E  NE	R3	R2	R3
      0x780E0033,  //  000F  JMPF	R3	#0044
      0x8C0C0506,  //  0010  GETMET	R3	R2	K6
      0x58140007,  //  0011  LDCONST	R5	K7
      0x4C180000,  //  0012  LDNIL	R6
      0x7C0C0600,  //  0013  CALL	R3	3
      0x8C100506,  //  0014  GETMET	R4	R2	K6
      0x58180008,  //  0015  LDCONST	R6	K8
      0x4C1C0000,  //  0016  LDNIL	R7
      0x7C100600,  //  0017  CALL	R4	3
      0x4C140000,  //  0018  LDNIL	R5
      0x20140605,  //  0019  NE	R5	R3	R5
      0x78160009,  //  001A  JMPF	R5	#0025
      0xB8161200,  //  001B  GETNGBL	R5	K9
      0x8C140B0A,  //  001C  GETMET	R5	R5	K10
      0x5C1C0600,  //  001D  MOVE	R7	R3
      0x5820000B,  //  001E  LDCONST	R8	K11
      0x54260167,  //  001F  LDINT	R9	360
      0x5828000B,  //  0020  LDCONST	R10	K11
      0x542E00FD,  //  0021  LDINT	R11	254
      0x7C140C00,  //  0022  CALL	R5	6
      0x5C0C0A00,  //  0023  MOVE	R3	R5
      0x70020000,  //  0024  JMP		#0026
      0x880C010C,  //  0025  GETMBR	R3	R0	K12
      0x4C140000,  //  0026  LDNIL	R5
      0x20140805,  //  0027  NE	R5	R4	R5
      0x78160009,  //  0028  JMPF	R5	#0033
      0xB8161200,  //  0029  GETNGBL	R5	K9
      0x8C140B0A,  //  002A  GETMET	R5	R5	K10
      0x5C1C0800,  //  002B  MOVE	R7	R4
      0x5820000B,  //  002C  LDCONST	R8	K11
      0x542600FE,  //  002D  LDINT	R9	255
      0x5828000B,  //  002E  LDCONST	R10	K11
      0x542E00FD,  //  002F  LDINT	R11	254
      0x7C140C00,  //  0030  CALL	R5	6
      0x5C100A00,  //  0031  MOVE	R4	R5
      0x70020000,  //  0032  JMP		#0034
      0x8810010D,  //  0033  GETMBR	R4	R0	K13
      0x8814010C,  //  0034  GETMBR	R5	R0	K12
      0x20140605,  //  0035  NE	R5	R3	R5
      0x78160004,  //  0036  JMPF	R5	#003C
      0x8C14010E,  //  0037  GETMET	R5	R0	K14
      0x541E02FF,  //  0038  LDINT	R7	768
      0x5820000B,  //  0039  LDCONST	R8	K11
      0x7C140600,  //  003A  CALL	R5	3
      0x90021803,  //  003B  SETMBR	R0	K12	R3
      0x8814010D,  //  003C  GETMBR	R5	R0	K13
      0x20140805,  //  003D  NE	R5	R4	R5
      0x78160004,  //  003E  JMPF	R5	#0044
      0x8C14010E,  //  003F  GETMET	R5	R0	K14
      0x541E02FF,  //  0040  LDINT	R7	768
      0x5820000F,  //  0041  LDCONST	R8	K15
      0x7C140600,  //  0042  CALL	R5	3
      0x90021A04,  //  0043  SETMBR	R0	K13	R4
      0x70020004,  //  0044  JMP		#004A
      0x60040003,  //  0045  GETGBL	R1	G3
      0x5C080000,  //  0046  MOVE	R2	R0
      0x7C040200,  //  0047  CALL	R1	1
      0x8C040303,  //  0048  GETMET	R1	R1	K3
      0x7C040200,  //  0049  CALL	R1	1
      0x80000000,  //  004A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_closure(class_Matter_Plugin_Light3_update_virtual,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light3, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(Hue),
    /* K2   */  be_nested_str_weak(Sat),
    /* K3   */  be_nested_str_weak(set_hue_sat),
    /* K4   */  be_nested_str_weak(update_virtual),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x8C0C0300,  //  0001  GETMET	R3	R1	K0
      0x58140001,  //  0002  LDCONST	R5	K1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x7C080200,  //  0004  CALL	R2	1
      0x600C0009,  //  0005  GETGBL	R3	G9
      0x8C100300,  //  0006  GETMET	R4	R1	K0
      0x58180002,  //  0007  LDCONST	R6	K2
      0x7C100400,  //  0008  CALL	R4	2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x4C100000,  //  000A  LDNIL	R4
      0x20100404,  //  000B  NE	R4	R2	R4
      0x74120002,  //  000C  JMPT	R4	#0010
      0x4C100000,  //  000D  LDNIL	R4
      0x20100604,  //  000E  NE	R4	R3	R4
      0x78120003,  //  000F  JMPF	R4	#0014
      0x8C100103,  //  0010  GETMET	R4	R0	K3
      0x5C180400,  //  0011  MOVE	R6	R2
      0x5C1C0600,  //  0012  MOVE	R7	R3
      0x7C100600,  //  0013  CALL	R4	3
      0x60100003,  //  0014  GETGBL	R4	G3
      0x5C140000,  //  0015  MOVE	R5	R0
      0x7C100200,  //  0016  CALL	R4	1
      0x8C100904,  //  0017  GETMET	R4	R4	K4
      0x5C180200,  //  0018  MOVE	R6	R1
      0x7C100400,  //  0019  CALL	R4	2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_hue_sat
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_closure(class_Matter_Plugin_Light3_set_hue_sat,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light3, 
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(BRIDGE),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(scale_uint),
    /* K4   */  be_nested_str_weak(call_remote_sync),
    /* K5   */  be_nested_str_weak(HSBColor1),
    /* K6   */  be_nested_str_weak(parse_status),
    /* K7   */  be_nested_str_weak(HSBColor2),
    /* K8   */  be_nested_str_weak(VIRTUAL),
    /* K9   */  be_nested_str_weak(shadow_hue),
    /* K10  */  be_nested_str_weak(attribute_updated),
    /* K11  */  be_nested_str_weak(shadow_sat),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(light),
    /* K14  */  be_nested_str_weak(set),
    /* K15  */  be_nested_str_weak(hue),
    /* K16  */  be_nested_str_weak(sat),
    /* K17  */  be_nested_str_weak(light_index),
    /* K18  */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(set_hue_sat),
    &be_const_str_solidified,
    ( &(const binstruction[154]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0203,  //  0001  NE	R3	R1	R3
      0x780E0006,  //  0002  JMPF	R3	#000A
      0x140C0300,  //  0003  LT	R3	R1	K0
      0x780E0000,  //  0004  JMPF	R3	#0006
      0x58040000,  //  0005  LDCONST	R1	K0
      0x540E00FD,  //  0006  LDINT	R3	254
      0x240C0203,  //  0007  GT	R3	R1	R3
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x540600FD,  //  0009  LDINT	R1	254
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x140C0500,  //  000D  LT	R3	R2	K0
      0x780E0000,  //  000E  JMPF	R3	#0010
      0x58080000,  //  000F  LDCONST	R2	K0
      0x540E00FD,  //  0010  LDINT	R3	254
      0x240C0403,  //  0011  GT	R3	R2	R3
      0x780E0000,  //  0012  JMPF	R3	#0014
      0x540A00FD,  //  0013  LDINT	R2	254
      0x880C0101,  //  0014  GETMBR	R3	R0	K1
      0x780E002C,  //  0015  JMPF	R3	#0043
      0x4C0C0000,  //  0016  LDNIL	R3
      0x200C0203,  //  0017  NE	R3	R1	R3
      0x780E0012,  //  0018  JMPF	R3	#002C
      0xB80E0400,  //  0019  GETNGBL	R3	K2
      0x8C0C0703,  //  001A  GETMET	R3	R3	K3
      0x5C140200,  //  001B  MOVE	R5	R1
      0x58180000,  //  001C  LDCONST	R6	K0
      0x541E00FD,  //  001D  LDINT	R7	254
      0x58200000,  //  001E  LDCONST	R8	K0
      0x54260167,  //  001F  LDINT	R9	360
      0x7C0C0C00,  //  0020  CALL	R3	6
      0x8C100104,  //  0021  GETMET	R4	R0	K4
      0x58180005,  //  0022  LDCONST	R6	K5
      0x5C1C0600,  //  0023  MOVE	R7	R3
      0x7C100600,  //  0024  CALL	R4	3
      0x4C140000,  //  0025  LDNIL	R5
      0x20140805,  //  0026  NE	R5	R4	R5
      0x78160003,  //  0027  JMPF	R5	#002C
      0x8C140106,  //  0028  GETMET	R5	R0	K6
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x5422000A,  //  002A  LDINT	R8	11
      0x7C140600,  //  002B  CALL	R5	3
      0x4C0C0000,  //  002C  LDNIL	R3
      0x200C0403,  //  002D  NE	R3	R2	R3
      0x780E0012,  //  002E  JMPF	R3	#0042
      0xB80E0400,  //  002F  GETNGBL	R3	K2
      0x8C0C0703,  //  0030  GETMET	R3	R3	K3
      0x5C140400,  //  0031  MOVE	R5	R2
      0x58180000,  //  0032  LDCONST	R6	K0
      0x541E00FD,  //  0033  LDINT	R7	254
      0x58200000,  //  0034  LDCONST	R8	K0
      0x54260063,  //  0035  LDINT	R9	100
      0x7C0C0C00,  //  0036  CALL	R3	6
      0x8C100104,  //  0037  GETMET	R4	R0	K4
      0x58180007,  //  0038  LDCONST	R6	K7
      0x5C1C0600,  //  0039  MOVE	R7	R3
      0x7C100600,  //  003A  CALL	R4	3
      0x4C140000,  //  003B  LDNIL	R5
      0x20140805,  //  003C  NE	R5	R4	R5
      0x78160003,  //  003D  JMPF	R5	#0042
      0x8C140106,  //  003E  GETMET	R5	R0	K6
      0x5C1C0800,  //  003F  MOVE	R7	R4
      0x5422000A,  //  0040  LDINT	R8	11
      0x7C140600,  //  0041  CALL	R5	3
      0x70020055,  //  0042  JMP		#0099
      0x880C0108,  //  0043  GETMBR	R3	R0	K8
      0x780E0016,  //  0044  JMPF	R3	#005C
      0x4C0C0000,  //  0045  LDNIL	R3
      0x200C0203,  //  0046  NE	R3	R1	R3
      0x780E0007,  //  0047  JMPF	R3	#0050
      0x880C0109,  //  0048  GETMBR	R3	R0	K9
      0x200C0203,  //  0049  NE	R3	R1	R3
      0x780E0004,  //  004A  JMPF	R3	#0050
      0x8C0C010A,  //  004B  GETMET	R3	R0	K10
      0x541602FF,  //  004C  LDINT	R5	768
      0x58180000,  //  004D  LDCONST	R6	K0
      0x7C0C0600,  //  004E  CALL	R3	3
      0x90021201,  //  004F  SETMBR	R0	K9	R1
      0x4C0C0000,  //  0050  LDNIL	R3
      0x200C0403,  //  0051  NE	R3	R2	R3
      0x780E0007,  //  0052  JMPF	R3	#005B
      0x880C010B,  //  0053  GETMBR	R3	R0	K11
      0x200C0403,  //  0054  NE	R3	R2	R3
      0x780E0004,  //  0055  JMPF	R3	#005B
      0x8C0C010A,  //  0056  GETMET	R3	R0	K10
      0x541602FF,  //  0057  LDINT	R5	768
      0x5818000C,  //  0058  LDCONST	R6	K12
      0x7C0C0600,  //  0059  CALL	R3	3
      0x90021602,  //  005A  SETMBR	R0	K11	R2
      0x7002003C,  //  005B  JMP		#0099
      0x4C0C0000,  //  005C  LDNIL	R3
      0x200C0203,  //  005D  NE	R3	R1	R3
      0x780E0008,  //  005E  JMPF	R3	#0068
      0xB80E0400,  //  005F  GETNGBL	R3	K2
      0x8C0C0703,  //  0060  GETMET	R3	R3	K3
      0x5C140200,  //  0061  MOVE	R5	R1
      0x58180000,  //  0062  LDCONST	R6	K0
      0x541E00FD,  //  0063  LDINT	R7	254
      0x58200000,  //  0064  LDCONST	R8	K0
      0x54260167,  //  0065  LDINT	R9	360
      0x7C0C0C00,  //  0066  CALL	R3	6
      0x70020000,  //  0067  JMP		#0069
      0x4C0C0000,  //  0068  LDNIL	R3
      0x4C100000,  //  0069  LDNIL	R4
      0x20100404,  //  006A  NE	R4	R2	R4
      0x78120008,  //  006B  JMPF	R4	#0075
      0xB8120400,  //  006C  GETNGBL	R4	K2
      0x8C100903,  //  006D  GETMET	R4	R4	K3
      0x5C180400,  //  006E  MOVE	R6	R2
      0x581C0000,  //  006F  LDCONST	R7	K0
      0x542200FD,  //  0070  LDINT	R8	254
      0x58240000,  //  0071  LDCONST	R9	K0
      0x542A00FE,  //  0072  LDINT	R10	255
      0x7C100C00,  //  0073  CALL	R4	6
      0x70020000,  //  0074  JMP		#0076
      0x4C100000,  //  0075  LDNIL	R4
      0x4C140000,  //  0076  LDNIL	R5
      0x20140605,  //  0077  NE	R5	R3	R5
      0x7816000B,  //  0078  JMPF	R5	#0085
      0x4C140000,  //  0079  LDNIL	R5
      0x20140805,  //  007A  NE	R5	R4	R5
      0x78160008,  //  007B  JMPF	R5	#0085
      0xB8161A00,  //  007C  GETNGBL	R5	K13
      0x8C140B0E,  //  007D  GETMET	R5	R5	K14
      0x601C0013,  //  007E  GETGBL	R7	G19
      0x7C1C0000,  //  007F  CALL	R7	0
      0x981E1E03,  //  0080  SETIDX	R7	K15	R3
      0x981E2004,  //  0081  SETIDX	R7	K16	R4
      0x88200111,  //  0082  GETMBR	R8	R0	K17
      0x7C140600,  //  0083  CALL	R5	3
      0x70020011,  //  0084  JMP		#0097
      0x4C140000,  //  0085  LDNIL	R5
      0x20140605,  //  0086  NE	R5	R3	R5
      0x78160007,  //  0087  JMPF	R5	#0090
      0xB8161A00,  //  0088  GETNGBL	R5	K13
      0x8C140B0E,  //  0089  GETMET	R5	R5	K14
      0x601C0013,  //  008A  GETGBL	R7	G19
      0x7C1C0000,  //  008B  CALL	R7	0
      0x981E1E03,  //  008C  SETIDX	R7	K15	R3
      0x88200111,  //  008D  GETMBR	R8	R0	K17
      0x7C140600,  //  008E  CALL	R5	3
      0x70020006,  //  008F  JMP		#0097
      0xB8161A00,  //  0090  GETNGBL	R5	K13
      0x8C140B0E,  //  0091  GETMET	R5	R5	K14
      0x601C0013,  //  0092  GETGBL	R7	G19
      0x7C1C0000,  //  0093  CALL	R7	0
      0x981E2004,  //  0094  SETIDX	R7	K16	R4
      0x88200111,  //  0095  GETMBR	R8	R0	K17
      0x7C140600,  //  0096  CALL	R5	3
      0x8C140112,  //  0097  GETMET	R5	R0	K18
      0x7C140200,  //  0098  CALL	R5	1
      0x80000000,  //  0099  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_closure(class_Matter_Plugin_Light3_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light3, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_hue),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(shadow_sat),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x90020302,  //  0008  SETMBR	R0	K1	K2
      0x90020702,  //  0009  SETMBR	R0	K3	K2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_closure(class_Matter_Plugin_Light3_web_values,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light3, 
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
** Solidified function: parse_status
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_closure(class_Matter_Plugin_Light3_parse_status,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light3, 
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(parse_status),
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
    be_str_weak(parse_status),
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
** Solidified function: web_value_RGB
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light3;
be_local_closure(class_Matter_Plugin_Light3_web_value_RGB,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Light3, 
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
** Solidified class: Matter_Plugin_Light3
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_class(Matter_Plugin_Light3,
    2,
    &be_class_Matter_Plugin_Light1,
    be_nested_map(18,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG_HINT, 17), be_nested_str_weak(_Not_X20used_) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light3) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(269, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
        be_nested_str_weak(Bri),
        be_nested_str_weak(Hue),
        be_nested_str_weak(Sat),
    }))    ) } )) },
        { be_const_key_weak(shadow_hue, 13), be_const_var(0) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(class_Matter_Plugin_Light3_invoke_request_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Light3_read_attribute_closure) },
        { be_const_key_weak(update_shadow, 11), be_const_closure(class_Matter_Plugin_Light3_update_shadow_closure) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Light3_update_virtual_closure) },
        { be_const_key_weak(ARG, 0), be_nested_str_weak() },
        { be_const_key_weak(shadow_sat, -1), be_const_var(1) },
        { be_const_key_weak(web_value_RGB, -1), be_const_closure(class_Matter_Plugin_Light3_web_value_RGB_closure) },
        { be_const_key_weak(parse_status, 16), be_const_closure(class_Matter_Plugin_Light3_parse_status_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(8, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(11,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(2),
        be_const_int(3),
        be_const_int(15),
        be_const_int(17),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(5, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(768, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(13,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(7),
        be_const_int(8),
        be_const_int(15),
        be_const_int(16385),
        be_const_int(16394),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Light3_web_values_closure) },
        { be_const_key_weak(set_hue_sat, 12), be_const_closure(class_Matter_Plugin_Light3_set_hue_sat_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Light3_init_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X203_X20RGB) },
    })),
    be_str_weak(Matter_Plugin_Light3)
);
/********************************************************************/
/* End of solidification */
