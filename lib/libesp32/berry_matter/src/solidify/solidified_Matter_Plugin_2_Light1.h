/* Solidification of Matter_Plugin_2_Light1.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light1;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light1_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_onoff),
    /* K2   */  be_nested_str_weak(shadow_bri),
    /* K3   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x50100000,  //  0008  LDBOOL	R4	0	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x90020503,  //  000A  SETMBR	R0	K2	K3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light1_invoke_request,   /* name */
  be_nested_proto(
    22,                          /* nstack */
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
    /* K7   */  be_nested_str_weak(set_onoff),
    /* K8   */  be_nested_str_weak(publish_command),
    /* K9   */  be_nested_str_weak(Power),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(shadow_onoff),
    /* K13  */  be_nested_str_weak(findsubval),
    /* K14  */  be_nested_str_weak(set_bri),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(bri_X3A),
    /* K17  */  be_nested_str_weak(Bri),
    /* K18  */  be_nested_str_weak(Dimmer),
    /* K19  */  be_nested_str_weak(tasmota),
    /* K20  */  be_nested_str_weak(scale_uint),
    /* K21  */  be_const_int(3),
    /* K22  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[167]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x54220005,  //  0005  LDINT	R8	6
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x7822002C,  //  0007  JMPF	R8	#0035
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x78220009,  //  000B  JMPF	R8	#0016
      0x8C200107,  //  000C  GETMET	R8	R0	K7
      0x50280000,  //  000D  LDBOOL	R10	0	0
      0x7C200400,  //  000E  CALL	R8	2
      0x8C200108,  //  000F  GETMET	R8	R0	K8
      0x58280009,  //  0010  LDCONST	R10	K9
      0x582C0006,  //  0011  LDCONST	R11	K6
      0x7C200600,  //  0012  CALL	R8	3
      0x50200200,  //  0013  LDBOOL	R8	1	0
      0x80041000,  //  0014  RET	1	R8
      0x7002001D,  //  0015  JMP		#0034
      0x1C200F0A,  //  0016  EQ	R8	R7	K10
      0x78220009,  //  0017  JMPF	R8	#0022
      0x8C200107,  //  0018  GETMET	R8	R0	K7
      0x50280200,  //  0019  LDBOOL	R10	1	0
      0x7C200400,  //  001A  CALL	R8	2
      0x8C200108,  //  001B  GETMET	R8	R0	K8
      0x58280009,  //  001C  LDCONST	R10	K9
      0x582C000A,  //  001D  LDCONST	R11	K10
      0x7C200600,  //  001E  CALL	R8	3
      0x50200200,  //  001F  LDBOOL	R8	1	0
      0x80041000,  //  0020  RET	1	R8
      0x70020011,  //  0021  JMP		#0034
      0x1C200F0B,  //  0022  EQ	R8	R7	K11
      0x7822000F,  //  0023  JMPF	R8	#0034
      0x8C200107,  //  0024  GETMET	R8	R0	K7
      0x8828010C,  //  0025  GETMBR	R10	R0	K12
      0x782A0000,  //  0026  JMPF	R10	#0028
      0x50280001,  //  0027  LDBOOL	R10	0	1
      0x50280200,  //  0028  LDBOOL	R10	1	0
      0x7C200400,  //  0029  CALL	R8	2
      0x8C200108,  //  002A  GETMET	R8	R0	K8
      0x58280009,  //  002B  LDCONST	R10	K9
      0x882C010C,  //  002C  GETMBR	R11	R0	K12
      0x782E0001,  //  002D  JMPF	R11	#0030
      0x582C000A,  //  002E  LDCONST	R11	K10
      0x70020000,  //  002F  JMP		#0031
      0x582C0006,  //  0030  LDCONST	R11	K6
      0x7C200600,  //  0031  CALL	R8	3
      0x50200200,  //  0032  LDBOOL	R8	1	0
      0x80041000,  //  0033  RET	1	R8
      0x70020070,  //  0034  JMP		#00A6
      0x54220007,  //  0035  LDINT	R8	8
      0x1C200C08,  //  0036  EQ	R8	R6	R8
      0x78220064,  //  0037  JMPF	R8	#009D
      0x8C200105,  //  0038  GETMET	R8	R0	K5
      0x7C200200,  //  0039  CALL	R8	1
      0x1C200F06,  //  003A  EQ	R8	R7	K6
      0x7822001A,  //  003B  JMPF	R8	#0057
      0x8C20050D,  //  003C  GETMET	R8	R2	K13
      0x58280006,  //  003D  LDCONST	R10	K6
      0x7C200400,  //  003E  CALL	R8	2
      0x8C24010E,  //  003F  GETMET	R9	R0	K14
      0x5C2C1000,  //  0040  MOVE	R11	R8
      0x7C240400,  //  0041  CALL	R9	2
      0x60240008,  //  0042  GETGBL	R9	G8
      0x5C281000,  //  0043  MOVE	R10	R8
      0x7C240200,  //  0044  CALL	R9	1
      0x00262009,  //  0045  ADD	R9	K16	R9
      0x900E1E09,  //  0046  SETMBR	R3	K15	R9
      0x8C240108,  //  0047  GETMET	R9	R0	K8
      0x582C0011,  //  0048  LDCONST	R11	K17
      0x5C301000,  //  0049  MOVE	R12	R8
      0x58340012,  //  004A  LDCONST	R13	K18
      0xB83A2600,  //  004B  GETNGBL	R14	K19
      0x8C381D14,  //  004C  GETMET	R14	R14	K20
      0x5C401000,  //  004D  MOVE	R16	R8
      0x58440006,  //  004E  LDCONST	R17	K6
      0x544A00FD,  //  004F  LDINT	R18	254
      0x584C0006,  //  0050  LDCONST	R19	K6
      0x54520063,  //  0051  LDINT	R20	100
      0x7C380C00,  //  0052  CALL	R14	6
      0x7C240A00,  //  0053  CALL	R9	5
      0x50240200,  //  0054  LDBOOL	R9	1	0
      0x80041200,  //  0055  RET	1	R9
      0x70020044,  //  0056  JMP		#009C
      0x1C200F0A,  //  0057  EQ	R8	R7	K10
      0x78220002,  //  0058  JMPF	R8	#005C
      0x50200200,  //  0059  LDBOOL	R8	1	0
      0x80041000,  //  005A  RET	1	R8
      0x7002003F,  //  005B  JMP		#009C
      0x1C200F0B,  //  005C  EQ	R8	R7	K11
      0x78220002,  //  005D  JMPF	R8	#0061
      0x50200200,  //  005E  LDBOOL	R8	1	0
      0x80041000,  //  005F  RET	1	R8
      0x7002003A,  //  0060  JMP		#009C
      0x1C200F15,  //  0061  EQ	R8	R7	K21
      0x78220002,  //  0062  JMPF	R8	#0066
      0x50200200,  //  0063  LDBOOL	R8	1	0
      0x80041000,  //  0064  RET	1	R8
      0x70020035,  //  0065  JMP		#009C
      0x54220003,  //  0066  LDINT	R8	4
      0x1C200E08,  //  0067  EQ	R8	R7	R8
      0x78220021,  //  0068  JMPF	R8	#008B
      0x8C20050D,  //  0069  GETMET	R8	R2	K13
      0x58280006,  //  006A  LDCONST	R10	K6
      0x7C200400,  //  006B  CALL	R8	2
      0x24241106,  //  006C  GT	R9	R8	K6
      0x8C28010E,  //  006D  GETMET	R10	R0	K14
      0x5C301000,  //  006E  MOVE	R12	R8
      0x5C341200,  //  006F  MOVE	R13	R9
      0x7C280600,  //  0070  CALL	R10	3
      0x60280008,  //  0071  GETGBL	R10	G8
      0x5C2C1000,  //  0072  MOVE	R11	R8
      0x7C280200,  //  0073  CALL	R10	1
      0x002A200A,  //  0074  ADD	R10	K16	R10
      0x900E1E0A,  //  0075  SETMBR	R3	K15	R10
      0x8C280108,  //  0076  GETMET	R10	R0	K8
      0x58300011,  //  0077  LDCONST	R12	K17
      0x5C341000,  //  0078  MOVE	R13	R8
      0x58380012,  //  0079  LDCONST	R14	K18
      0xB83E2600,  //  007A  GETNGBL	R15	K19
      0x8C3C1F14,  //  007B  GETMET	R15	R15	K20
      0x5C441000,  //  007C  MOVE	R17	R8
      0x58480006,  //  007D  LDCONST	R18	K6
      0x544E00FD,  //  007E  LDINT	R19	254
      0x58500006,  //  007F  LDCONST	R20	K6
      0x54560063,  //  0080  LDINT	R21	100
      0x7C3C0C00,  //  0081  CALL	R15	6
      0x58400009,  //  0082  LDCONST	R16	K9
      0x78260001,  //  0083  JMPF	R9	#0086
      0x5844000A,  //  0084  LDCONST	R17	K10
      0x70020000,  //  0085  JMP		#0087
      0x58440006,  //  0086  LDCONST	R17	K6
      0x7C280E00,  //  0087  CALL	R10	7
      0x50280200,  //  0088  LDBOOL	R10	1	0
      0x80041400,  //  0089  RET	1	R10
      0x70020010,  //  008A  JMP		#009C
      0x54220004,  //  008B  LDINT	R8	5
      0x1C200E08,  //  008C  EQ	R8	R7	R8
      0x78220002,  //  008D  JMPF	R8	#0091
      0x50200200,  //  008E  LDBOOL	R8	1	0
      0x80041000,  //  008F  RET	1	R8
      0x7002000A,  //  0090  JMP		#009C
      0x54220005,  //  0091  LDINT	R8	6
      0x1C200E08,  //  0092  EQ	R8	R7	R8
      0x78220002,  //  0093  JMPF	R8	#0097
      0x50200200,  //  0094  LDBOOL	R8	1	0
      0x80041000,  //  0095  RET	1	R8
      0x70020004,  //  0096  JMP		#009C
      0x54220006,  //  0097  LDINT	R8	7
      0x1C200E08,  //  0098  EQ	R8	R7	R8
      0x78220001,  //  0099  JMPF	R8	#009C
      0x50200200,  //  009A  LDBOOL	R8	1	0
      0x80041000,  //  009B  RET	1	R8
      0x70020008,  //  009C  JMP		#00A6
      0x60200003,  //  009D  GETGBL	R8	G3
      0x5C240000,  //  009E  MOVE	R9	R0
      0x7C200200,  //  009F  CALL	R8	1
      0x8C201116,  //  00A0  GETMET	R8	R8	K22
      0x5C280200,  //  00A1  MOVE	R10	R1
      0x5C2C0400,  //  00A2  MOVE	R11	R2
      0x5C300600,  //  00A3  MOVE	R12	R3
      0x7C200800,  //  00A4  CALL	R8	4
      0x80041000,  //  00A5  RET	1	R8
      0x80000000,  //  00A6  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_onoff
********************************************************************/
be_local_closure(Matter_Plugin_Light1_set_onoff,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(VIRTUAL),
    /* K1   */  be_nested_str_weak(light),
    /* K2   */  be_nested_str_weak(set),
    /* K3   */  be_nested_str_weak(power),
    /* K4   */  be_nested_str_weak(update_shadow),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    /* K6   */  be_nested_str_weak(attribute_updated),
    /* K7   */  be_const_int(0),
    }),
    be_str_weak(set_onoff),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0008,  //  0001  JMPT	R2	#000B
      0xA40A0200,  //  0002  IMPORT	R2	K1
      0x8C0C0502,  //  0003  GETMET	R3	R2	K2
      0x60140013,  //  0004  GETGBL	R5	G19
      0x7C140000,  //  0005  CALL	R5	0
      0x98160601,  //  0006  SETIDX	R5	K3	R1
      0x7C0C0400,  //  0007  CALL	R3	2
      0x8C0C0104,  //  0008  GETMET	R3	R0	K4
      0x7C0C0200,  //  0009  CALL	R3	1
      0x70020007,  //  000A  JMP		#0013
      0x88080105,  //  000B  GETMBR	R2	R0	K5
      0x20080202,  //  000C  NE	R2	R1	R2
      0x780A0004,  //  000D  JMPF	R2	#0013
      0x8C080106,  //  000E  GETMET	R2	R0	K6
      0x54120005,  //  000F  LDINT	R4	6
      0x58140007,  //  0010  LDCONST	R5	K7
      0x7C080600,  //  0011  CALL	R2	3
      0x90020A01,  //  0012  SETMBR	R0	K5	R1
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light1_update_shadow,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(VIRTUAL),
    /* K1   */  be_nested_str_weak(light),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(power),
    /* K5   */  be_nested_str_weak(shadow_onoff),
    /* K6   */  be_nested_str_weak(attribute_updated),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(bri),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(scale_uint),
    /* K11  */  be_nested_str_weak(shadow_bri),
    /* K12  */  be_nested_str_weak(update_shadow),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060029,  //  0001  JMPT	R1	#002C
      0xA4060200,  //  0002  IMPORT	R1	K1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x200C0403,  //  0006  NE	R3	R2	R3
      0x780E0023,  //  0007  JMPF	R3	#002C
      0x8C0C0503,  //  0008  GETMET	R3	R2	K3
      0x58140004,  //  0009  LDCONST	R5	K4
      0x4C180000,  //  000A  LDNIL	R6
      0x7C0C0600,  //  000B  CALL	R3	3
      0x88100105,  //  000C  GETMBR	R4	R0	K5
      0x20100604,  //  000D  NE	R4	R3	R4
      0x78120004,  //  000E  JMPF	R4	#0014
      0x8C100106,  //  000F  GETMET	R4	R0	K6
      0x541A0005,  //  0010  LDINT	R6	6
      0x581C0007,  //  0011  LDCONST	R7	K7
      0x7C100600,  //  0012  CALL	R4	3
      0x90020A03,  //  0013  SETMBR	R0	K5	R3
      0x8C100503,  //  0014  GETMET	R4	R2	K3
      0x58180008,  //  0015  LDCONST	R6	K8
      0x4C1C0000,  //  0016  LDNIL	R7
      0x7C100600,  //  0017  CALL	R4	3
      0x4C140000,  //  0018  LDNIL	R5
      0x20140805,  //  0019  NE	R5	R4	R5
      0x78160010,  //  001A  JMPF	R5	#002C
      0xB8161200,  //  001B  GETNGBL	R5	K9
      0x8C140B0A,  //  001C  GETMET	R5	R5	K10
      0x5C1C0800,  //  001D  MOVE	R7	R4
      0x58200007,  //  001E  LDCONST	R8	K7
      0x542600FE,  //  001F  LDINT	R9	255
      0x58280007,  //  0020  LDCONST	R10	K7
      0x542E00FD,  //  0021  LDINT	R11	254
      0x7C140C00,  //  0022  CALL	R5	6
      0x5C100A00,  //  0023  MOVE	R4	R5
      0x8814010B,  //  0024  GETMBR	R5	R0	K11
      0x20140805,  //  0025  NE	R5	R4	R5
      0x78160004,  //  0026  JMPF	R5	#002C
      0x8C140106,  //  0027  GETMET	R5	R0	K6
      0x541E0007,  //  0028  LDINT	R7	8
      0x58200007,  //  0029  LDCONST	R8	K7
      0x7C140600,  //  002A  CALL	R5	3
      0x90021604,  //  002B  SETMBR	R0	K11	R4
      0x60040003,  //  002C  GETGBL	R1	G3
      0x5C080000,  //  002D  MOVE	R2	R0
      0x7C040200,  //  002E  CALL	R1	1
      0x8C04030C,  //  002F  GETMET	R1	R1	K12
      0x7C040200,  //  0030  CALL	R1	1
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(Matter_Plugin_Light1_update_virtual,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(Power),
    /* K2   */  be_nested_str_weak(Bri),
    /* K3   */  be_nested_str_weak(set_bri),
    /* K4   */  be_nested_str_weak(set_onoff),
    /* K5   */  be_nested_str_weak(update_virtual),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x8C0C0300,  //  0003  GETMET	R3	R1	K0
      0x58140002,  //  0004  LDCONST	R5	K2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x4C100000,  //  0006  LDNIL	R4
      0x20100604,  //  0007  NE	R4	R3	R4
      0x78120007,  //  0008  JMPF	R4	#0011
      0x8C100103,  //  0009  GETMET	R4	R0	K3
      0x60180009,  //  000A  GETGBL	R6	G9
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C180200,  //  000C  CALL	R6	1
      0x5C1C0400,  //  000D  MOVE	R7	R2
      0x7C100600,  //  000E  CALL	R4	3
      0x80000800,  //  000F  RET	0
      0x70020007,  //  0010  JMP		#0019
      0x4C100000,  //  0011  LDNIL	R4
      0x20100404,  //  0012  NE	R4	R2	R4
      0x78120004,  //  0013  JMPF	R4	#0019
      0x8C100104,  //  0014  GETMET	R4	R0	K4
      0x60180017,  //  0015  GETGBL	R6	G23
      0x5C1C0400,  //  0016  MOVE	R7	R2
      0x7C180200,  //  0017  CALL	R6	1
      0x7C100400,  //  0018  CALL	R4	2
      0x60100003,  //  0019  GETGBL	R4	G3
      0x5C140000,  //  001A  MOVE	R5	R0
      0x7C100200,  //  001B  CALL	R4	1
      0x8C100905,  //  001C  GETMET	R4	R4	K5
      0x5C180200,  //  001D  MOVE	R6	R1
      0x7C100400,  //  001E  CALL	R4	2
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light1_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(BOOL),
    /* K8   */  be_nested_str_weak(shadow_onoff),
    /* K9   */  be_nested_str_weak(U4),
    /* K10  */  be_nested_str_weak(U1),
    /* K11  */  be_nested_str_weak(shadow_bri),
    /* K12  */  be_const_int(2),
    /* K13  */  be_const_int(3),
    /* K14  */  be_const_int(1),
    /* K15  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[110]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E001B,  //  0006  JMPF	R7	#0023
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0005,  //  000A  JMPF	R7	#0011
      0x8C1C0706,  //  000B  GETMET	R7	R3	K6
      0x88240907,  //  000C  GETMBR	R9	R4	K7
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x70020010,  //  0010  JMP		#0022
      0x541EFFFB,  //  0011  LDINT	R7	65532
      0x1C1C0C07,  //  0012  EQ	R7	R6	R7
      0x781E0005,  //  0013  JMPF	R7	#001A
      0x8C1C0706,  //  0014  GETMET	R7	R3	K6
      0x88240909,  //  0015  GETMBR	R9	R4	K9
      0x58280005,  //  0016  LDCONST	R10	K5
      0x7C1C0600,  //  0017  CALL	R7	3
      0x80040E00,  //  0018  RET	1	R7
      0x70020007,  //  0019  JMP		#0022
      0x541EFFFC,  //  001A  LDINT	R7	65533
      0x1C1C0C07,  //  001B  EQ	R7	R6	R7
      0x781E0004,  //  001C  JMPF	R7	#0022
      0x8C1C0706,  //  001D  GETMET	R7	R3	K6
      0x88240909,  //  001E  GETMBR	R9	R4	K9
      0x542A0003,  //  001F  LDINT	R10	4
      0x7C1C0600,  //  0020  CALL	R7	3
      0x80040E00,  //  0021  RET	1	R7
      0x70020049,  //  0022  JMP		#006D
      0x541E0007,  //  0023  LDINT	R7	8
      0x1C1C0A07,  //  0024  EQ	R7	R5	R7
      0x781E003D,  //  0025  JMPF	R7	#0064
      0x8C1C0104,  //  0026  GETMET	R7	R0	K4
      0x7C1C0200,  //  0027  CALL	R7	1
      0x1C1C0D05,  //  0028  EQ	R7	R6	K5
      0x781E0005,  //  0029  JMPF	R7	#0030
      0x8C1C0706,  //  002A  GETMET	R7	R3	K6
      0x8824090A,  //  002B  GETMBR	R9	R4	K10
      0x8828010B,  //  002C  GETMBR	R10	R0	K11
      0x7C1C0600,  //  002D  CALL	R7	3
      0x80040E00,  //  002E  RET	1	R7
      0x70020032,  //  002F  JMP		#0063
      0x1C1C0D0C,  //  0030  EQ	R7	R6	K12
      0x781E0005,  //  0031  JMPF	R7	#0038
      0x8C1C0706,  //  0032  GETMET	R7	R3	K6
      0x8824090A,  //  0033  GETMBR	R9	R4	K10
      0x58280005,  //  0034  LDCONST	R10	K5
      0x7C1C0600,  //  0035  CALL	R7	3
      0x80040E00,  //  0036  RET	1	R7
      0x7002002A,  //  0037  JMP		#0063
      0x1C1C0D0D,  //  0038  EQ	R7	R6	K13
      0x781E0005,  //  0039  JMPF	R7	#0040
      0x8C1C0706,  //  003A  GETMET	R7	R3	K6
      0x8824090A,  //  003B  GETMBR	R9	R4	K10
      0x542A00FD,  //  003C  LDINT	R10	254
      0x7C1C0600,  //  003D  CALL	R7	3
      0x80040E00,  //  003E  RET	1	R7
      0x70020022,  //  003F  JMP		#0063
      0x541E000E,  //  0040  LDINT	R7	15
      0x1C1C0C07,  //  0041  EQ	R7	R6	R7
      0x781E0005,  //  0042  JMPF	R7	#0049
      0x8C1C0706,  //  0043  GETMET	R7	R3	K6
      0x8824090A,  //  0044  GETMBR	R9	R4	K10
      0x58280005,  //  0045  LDCONST	R10	K5
      0x7C1C0600,  //  0046  CALL	R7	3
      0x80040E00,  //  0047  RET	1	R7
      0x70020019,  //  0048  JMP		#0063
      0x541E0010,  //  0049  LDINT	R7	17
      0x1C1C0C07,  //  004A  EQ	R7	R6	R7
      0x781E0005,  //  004B  JMPF	R7	#0052
      0x8C1C0706,  //  004C  GETMET	R7	R3	K6
      0x8824090A,  //  004D  GETMBR	R9	R4	K10
      0x8828010B,  //  004E  GETMBR	R10	R0	K11
      0x7C1C0600,  //  004F  CALL	R7	3
      0x80040E00,  //  0050  RET	1	R7
      0x70020010,  //  0051  JMP		#0063
      0x541EFFFB,  //  0052  LDINT	R7	65532
      0x1C1C0C07,  //  0053  EQ	R7	R6	R7
      0x781E0005,  //  0054  JMPF	R7	#005B
      0x8C1C0706,  //  0055  GETMET	R7	R3	K6
      0x88240909,  //  0056  GETMBR	R9	R4	K9
      0x5828000E,  //  0057  LDCONST	R10	K14
      0x7C1C0600,  //  0058  CALL	R7	3
      0x80040E00,  //  0059  RET	1	R7
      0x70020007,  //  005A  JMP		#0063
      0x541EFFFC,  //  005B  LDINT	R7	65533
      0x1C1C0C07,  //  005C  EQ	R7	R6	R7
      0x781E0004,  //  005D  JMPF	R7	#0063
      0x8C1C0706,  //  005E  GETMET	R7	R3	K6
      0x88240909,  //  005F  GETMBR	R9	R4	K9
      0x542A0004,  //  0060  LDINT	R10	5
      0x7C1C0600,  //  0061  CALL	R7	3
      0x80040E00,  //  0062  RET	1	R7
      0x70020008,  //  0063  JMP		#006D
      0x601C0003,  //  0064  GETGBL	R7	G3
      0x5C200000,  //  0065  MOVE	R8	R0
      0x7C1C0200,  //  0066  CALL	R7	1
      0x8C1C0F0F,  //  0067  GETMET	R7	R7	K15
      0x5C240200,  //  0068  MOVE	R9	R1
      0x5C280400,  //  0069  MOVE	R10	R2
      0x5C2C0600,  //  006A  MOVE	R11	R3
      0x7C1C0800,  //  006B  CALL	R7	4
      0x80040E00,  //  006C  RET	1	R7
      0x80000000,  //  006D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_bri
********************************************************************/
be_local_closure(Matter_Plugin_Light1_set_bri,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(VIRTUAL),
    /* K2   */  be_nested_str_weak(light),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(scale_uint),
    /* K5   */  be_nested_str_weak(set),
    /* K6   */  be_nested_str_weak(bri),
    /* K7   */  be_nested_str_weak(power),
    /* K8   */  be_nested_str_weak(update_shadow),
    /* K9   */  be_nested_str_weak(shadow_onoff),
    /* K10  */  be_nested_str_weak(attribute_updated),
    /* K11  */  be_nested_str_weak(shadow_bri),
    }),
    be_str_weak(set_bri),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0x140C0300,  //  0000  LT	R3	R1	K0
      0x780E0000,  //  0001  JMPF	R3	#0003
      0x58040000,  //  0002  LDCONST	R1	K0
      0x540E00FD,  //  0003  LDINT	R3	254
      0x240C0203,  //  0004  GT	R3	R1	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x540600FD,  //  0006  LDINT	R1	254
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0003,  //  0009  JMPF	R3	#000E
      0x600C0017,  //  000A  GETGBL	R3	G23
      0x5C100400,  //  000B  MOVE	R4	R2
      0x7C0C0200,  //  000C  CALL	R3	1
      0x70020000,  //  000D  JMP		#000F
      0x4C0C0000,  //  000E  LDNIL	R3
      0x5C080600,  //  000F  MOVE	R2	R3
      0x880C0101,  //  0010  GETMBR	R3	R0	K1
      0x740E001A,  //  0011  JMPT	R3	#002D
      0xA40E0400,  //  0012  IMPORT	R3	K2
      0xB8120600,  //  0013  GETNGBL	R4	K3
      0x8C100904,  //  0014  GETMET	R4	R4	K4
      0x5C180200,  //  0015  MOVE	R6	R1
      0x581C0000,  //  0016  LDCONST	R7	K0
      0x542200FD,  //  0017  LDINT	R8	254
      0x58240000,  //  0018  LDCONST	R9	K0
      0x542A00FE,  //  0019  LDINT	R10	255
      0x7C100C00,  //  001A  CALL	R4	6
      0x4C140000,  //  001B  LDNIL	R5
      0x1C140405,  //  001C  EQ	R5	R2	R5
      0x78160005,  //  001D  JMPF	R5	#0024
      0x8C140705,  //  001E  GETMET	R5	R3	K5
      0x601C0013,  //  001F  GETGBL	R7	G19
      0x7C1C0000,  //  0020  CALL	R7	0
      0x981E0C04,  //  0021  SETIDX	R7	K6	R4
      0x7C140400,  //  0022  CALL	R5	2
      0x70020005,  //  0023  JMP		#002A
      0x8C140705,  //  0024  GETMET	R5	R3	K5
      0x601C0013,  //  0025  GETGBL	R7	G19
      0x7C1C0000,  //  0026  CALL	R7	0
      0x981E0C04,  //  0027  SETIDX	R7	K6	R4
      0x981E0E02,  //  0028  SETIDX	R7	K7	R2
      0x7C140400,  //  0029  CALL	R5	2
      0x8C140108,  //  002A  GETMET	R5	R0	K8
      0x7C140200,  //  002B  CALL	R5	1
      0x70020012,  //  002C  JMP		#0040
      0x4C0C0000,  //  002D  LDNIL	R3
      0x200C0403,  //  002E  NE	R3	R2	R3
      0x780E0007,  //  002F  JMPF	R3	#0038
      0x880C0109,  //  0030  GETMBR	R3	R0	K9
      0x200C0403,  //  0031  NE	R3	R2	R3
      0x780E0004,  //  0032  JMPF	R3	#0038
      0x8C0C010A,  //  0033  GETMET	R3	R0	K10
      0x54160005,  //  0034  LDINT	R5	6
      0x58180000,  //  0035  LDCONST	R6	K0
      0x7C0C0600,  //  0036  CALL	R3	3
      0x90021202,  //  0037  SETMBR	R0	K9	R2
      0x880C010B,  //  0038  GETMBR	R3	R0	K11
      0x200C0203,  //  0039  NE	R3	R1	R3
      0x780E0004,  //  003A  JMPF	R3	#0040
      0x8C0C010A,  //  003B  GETMET	R3	R0	K10
      0x54160007,  //  003C  LDINT	R5	8
      0x58180000,  //  003D  LDCONST	R6	K0
      0x7C0C0600,  //  003E  CALL	R3	3
      0x90021601,  //  003F  SETMBR	R0	K11	R1
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light1
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Light1,
    2,
    &be_class_Matter_Plugin_Device,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light1_init_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Light1_update_shadow_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light1_invoke_request_closure) },
        { be_const_key_weak(set_onoff, -1), be_const_closure(Matter_Plugin_Light1_set_onoff_closure) },
        { be_const_key_weak(TYPE, 10), be_nested_str_weak(light1) },
        { be_const_key_weak(update_virtual, 11), be_const_closure(Matter_Plugin_Light1_update_virtual_closure) },
        { be_const_key_weak(UPDATE_TIME, 8), be_const_int(250) },
        { be_const_key_weak(shadow_bri, 1), be_const_var(1) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X201_X20Dimmer) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
        be_nested_str_weak(Bri),
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(257, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light1_read_attribute_closure) },
        { be_const_key_weak(shadow_onoff, -1), be_const_var(0) },
        { be_const_key_weak(set_bri, -1), be_const_closure(Matter_Plugin_Light1_set_bri_closure) },
    })),
    be_str_weak(Matter_Plugin_Light1)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light1_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light1);
    be_setglobal(vm, "Matter_Plugin_Light1");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
