/* Solidification of Matter_Plugin_Light3.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light3;

/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light3_update_shadow,   /* name */
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
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(update_shadow),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(hue),
    /* K5   */  be_nested_str_weak(sat),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(scale_uint),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(shadow_hue),
    /* K10  */  be_nested_str_weak(shadow_sat),
    /* K11  */  be_nested_str_weak(attribute_updated),
    /* K12  */  be_const_int(1),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[64]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080003,  //  0001  GETGBL	R2	G3
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x8C080302,  //  0006  GETMET	R2	R1	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x4C0C0000,  //  0008  LDNIL	R3
      0x200C0403,  //  0009  NE	R3	R2	R3
      0x780E0033,  //  000A  JMPF	R3	#003F
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x58140004,  //  000C  LDCONST	R5	K4
      0x4C180000,  //  000D  LDNIL	R6
      0x7C0C0600,  //  000E  CALL	R3	3
      0x8C100503,  //  000F  GETMET	R4	R2	K3
      0x58180005,  //  0010  LDCONST	R6	K5
      0x4C1C0000,  //  0011  LDNIL	R7
      0x7C100600,  //  0012  CALL	R4	3
      0x4C140000,  //  0013  LDNIL	R5
      0x20140605,  //  0014  NE	R5	R3	R5
      0x78160009,  //  0015  JMPF	R5	#0020
      0xB8160C00,  //  0016  GETNGBL	R5	K6
      0x8C140B07,  //  0017  GETMET	R5	R5	K7
      0x5C1C0600,  //  0018  MOVE	R7	R3
      0x58200008,  //  0019  LDCONST	R8	K8
      0x54260167,  //  001A  LDINT	R9	360
      0x58280008,  //  001B  LDCONST	R10	K8
      0x542E00FD,  //  001C  LDINT	R11	254
      0x7C140C00,  //  001D  CALL	R5	6
      0x5C0C0A00,  //  001E  MOVE	R3	R5
      0x70020000,  //  001F  JMP		#0021
      0x880C0109,  //  0020  GETMBR	R3	R0	K9
      0x4C140000,  //  0021  LDNIL	R5
      0x20140805,  //  0022  NE	R5	R4	R5
      0x78160009,  //  0023  JMPF	R5	#002E
      0xB8160C00,  //  0024  GETNGBL	R5	K6
      0x8C140B07,  //  0025  GETMET	R5	R5	K7
      0x5C1C0800,  //  0026  MOVE	R7	R4
      0x58200008,  //  0027  LDCONST	R8	K8
      0x542600FE,  //  0028  LDINT	R9	255
      0x58280008,  //  0029  LDCONST	R10	K8
      0x542E00FD,  //  002A  LDINT	R11	254
      0x7C140C00,  //  002B  CALL	R5	6
      0x5C100A00,  //  002C  MOVE	R4	R5
      0x70020000,  //  002D  JMP		#002F
      0x8810010A,  //  002E  GETMBR	R4	R0	K10
      0x88140109,  //  002F  GETMBR	R5	R0	K9
      0x20140605,  //  0030  NE	R5	R3	R5
      0x78160004,  //  0031  JMPF	R5	#0037
      0x8C14010B,  //  0032  GETMET	R5	R0	K11
      0x541E02FF,  //  0033  LDINT	R7	768
      0x58200008,  //  0034  LDCONST	R8	K8
      0x7C140600,  //  0035  CALL	R5	3
      0x90021203,  //  0036  SETMBR	R0	K9	R3
      0x8814010A,  //  0037  GETMBR	R5	R0	K10
      0x20140805,  //  0038  NE	R5	R4	R5
      0x78160004,  //  0039  JMPF	R5	#003F
      0x8C14010B,  //  003A  GETMET	R5	R0	K11
      0x541E02FF,  //  003B  LDINT	R7	768
      0x5820000C,  //  003C  LDCONST	R8	K12
      0x7C140600,  //  003D  CALL	R5	3
      0x90021404,  //  003E  SETMBR	R0	K10	R4
      0x80000000,  //  003F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light3_read_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(shadow_hue),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(shadow_sat),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[106]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A02FF,  //  0004  LDINT	R6	768
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A0059,  //  0006  JMPF	R6	#0061
      0x8C180104,  //  0007  GETMET	R6	R0	K4
      0x7C180200,  //  0008  CALL	R6	1
      0x1C180B05,  //  0009  EQ	R6	R5	K5
      0x781A0005,  //  000A  JMPF	R6	#0011
      0x8C180706,  //  000B  GETMET	R6	R3	K6
      0x88200707,  //  000C  GETMBR	R8	R3	K7
      0x88240108,  //  000D  GETMBR	R9	R0	K8
      0x7C180600,  //  000E  CALL	R6	3
      0x80040C00,  //  000F  RET	1	R6
      0x7002004E,  //  0010  JMP		#0060
      0x1C180B09,  //  0011  EQ	R6	R5	K9
      0x781A0005,  //  0012  JMPF	R6	#0019
      0x8C180706,  //  0013  GETMET	R6	R3	K6
      0x88200707,  //  0014  GETMBR	R8	R3	K7
      0x8824010A,  //  0015  GETMBR	R9	R0	K10
      0x7C180600,  //  0016  CALL	R6	3
      0x80040C00,  //  0017  RET	1	R6
      0x70020046,  //  0018  JMP		#0060
      0x541A0006,  //  0019  LDINT	R6	7
      0x1C180A06,  //  001A  EQ	R6	R5	R6
      0x781A0005,  //  001B  JMPF	R6	#0022
      0x8C180706,  //  001C  GETMET	R6	R3	K6
      0x88200707,  //  001D  GETMBR	R8	R3	K7
      0x58240005,  //  001E  LDCONST	R9	K5
      0x7C180600,  //  001F  CALL	R6	3
      0x80040C00,  //  0020  RET	1	R6
      0x7002003D,  //  0021  JMP		#0060
      0x541A0007,  //  0022  LDINT	R6	8
      0x1C180A06,  //  0023  EQ	R6	R5	R6
      0x781A0005,  //  0024  JMPF	R6	#002B
      0x8C180706,  //  0025  GETMET	R6	R3	K6
      0x88200707,  //  0026  GETMBR	R8	R3	K7
      0x58240005,  //  0027  LDCONST	R9	K5
      0x7C180600,  //  0028  CALL	R6	3
      0x80040C00,  //  0029  RET	1	R6
      0x70020034,  //  002A  JMP		#0060
      0x541A000E,  //  002B  LDINT	R6	15
      0x1C180A06,  //  002C  EQ	R6	R5	R6
      0x781A0005,  //  002D  JMPF	R6	#0034
      0x8C180706,  //  002E  GETMET	R6	R3	K6
      0x88200707,  //  002F  GETMBR	R8	R3	K7
      0x58240005,  //  0030  LDCONST	R9	K5
      0x7C180600,  //  0031  CALL	R6	3
      0x80040C00,  //  0032  RET	1	R6
      0x7002002B,  //  0033  JMP		#0060
      0x541A4000,  //  0034  LDINT	R6	16385
      0x1C180A06,  //  0035  EQ	R6	R5	R6
      0x781A0005,  //  0036  JMPF	R6	#003D
      0x8C180706,  //  0037  GETMET	R6	R3	K6
      0x88200707,  //  0038  GETMBR	R8	R3	K7
      0x58240005,  //  0039  LDCONST	R9	K5
      0x7C180600,  //  003A  CALL	R6	3
      0x80040C00,  //  003B  RET	1	R6
      0x70020022,  //  003C  JMP		#0060
      0x541A4009,  //  003D  LDINT	R6	16394
      0x1C180A06,  //  003E  EQ	R6	R5	R6
      0x781A0005,  //  003F  JMPF	R6	#0046
      0x8C180706,  //  0040  GETMET	R6	R3	K6
      0x88200707,  //  0041  GETMBR	R8	R3	K7
      0x58240009,  //  0042  LDCONST	R9	K9
      0x7C180600,  //  0043  CALL	R6	3
      0x80040C00,  //  0044  RET	1	R6
      0x70020019,  //  0045  JMP		#0060
      0x541A000F,  //  0046  LDINT	R6	16
      0x1C180A06,  //  0047  EQ	R6	R5	R6
      0x781A0005,  //  0048  JMPF	R6	#004F
      0x8C180706,  //  0049  GETMET	R6	R3	K6
      0x88200707,  //  004A  GETMBR	R8	R3	K7
      0x58240005,  //  004B  LDCONST	R9	K5
      0x7C180600,  //  004C  CALL	R6	3
      0x80040C00,  //  004D  RET	1	R6
      0x70020010,  //  004E  JMP		#0060
      0x541AFFFB,  //  004F  LDINT	R6	65532
      0x1C180A06,  //  0050  EQ	R6	R5	R6
      0x781A0005,  //  0051  JMPF	R6	#0058
      0x8C180706,  //  0052  GETMET	R6	R3	K6
      0x8820070B,  //  0053  GETMBR	R8	R3	K11
      0x58240009,  //  0054  LDCONST	R9	K9
      0x7C180600,  //  0055  CALL	R6	3
      0x80040C00,  //  0056  RET	1	R6
      0x70020007,  //  0057  JMP		#0060
      0x541AFFFC,  //  0058  LDINT	R6	65533
      0x1C180A06,  //  0059  EQ	R6	R5	R6
      0x781A0004,  //  005A  JMPF	R6	#0060
      0x8C180706,  //  005B  GETMET	R6	R3	K6
      0x8820070B,  //  005C  GETMBR	R8	R3	K11
      0x54260004,  //  005D  LDINT	R9	5
      0x7C180600,  //  005E  CALL	R6	3
      0x80040C00,  //  005F  RET	1	R6
      0x70020007,  //  0060  JMP		#0069
      0x60180003,  //  0061  GETGBL	R6	G3
      0x5C1C0000,  //  0062  MOVE	R7	R0
      0x7C180200,  //  0063  CALL	R6	1
      0x8C180D0C,  //  0064  GETMET	R6	R6	K12
      0x5C200200,  //  0065  MOVE	R8	R1
      0x5C240400,  //  0066  MOVE	R9	R2
      0x7C180600,  //  0067  CALL	R6	3
      0x80040C00,  //  0068  RET	1	R6
      0x80000000,  //  0069  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light3_init,   /* name */
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
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light3_invoke_request,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(findsubval),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(scale_uint),
    /* K10  */  be_nested_str_weak(set),
    /* K11  */  be_nested_str_weak(hue),
    /* K12  */  be_nested_str_weak(update_shadow),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(hue_X3A),
    /* K15  */  be_const_int(1),
    /* K16  */  be_const_int(2),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(sat),
    /* K19  */  be_nested_str_weak(sat_X3A),
    /* K20  */  be_nested_str_weak(_X20sat_X3A),
    /* K21  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[150]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x542202FF,  //  0005  LDINT	R8	768
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220083,  //  0007  JMPF	R8	#008C
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x78220019,  //  000B  JMPF	R8	#0026
      0x8C200507,  //  000C  GETMET	R8	R2	K7
      0x58280006,  //  000D  LDCONST	R10	K6
      0x7C200400,  //  000E  CALL	R8	2
      0xB8261000,  //  000F  GETNGBL	R9	K8
      0x8C241309,  //  0010  GETMET	R9	R9	K9
      0x5C2C1000,  //  0011  MOVE	R11	R8
      0x58300006,  //  0012  LDCONST	R12	K6
      0x543600FD,  //  0013  LDINT	R13	254
      0x58380006,  //  0014  LDCONST	R14	K6
      0x543E0167,  //  0015  LDINT	R15	360
      0x7C240C00,  //  0016  CALL	R9	6
      0x8C28090A,  //  0017  GETMET	R10	R4	K10
      0x60300013,  //  0018  GETGBL	R12	G19
      0x7C300000,  //  0019  CALL	R12	0
      0x98321609,  //  001A  SETIDX	R12	K11	R9
      0x7C280400,  //  001B  CALL	R10	2
      0x8C28010C,  //  001C  GETMET	R10	R0	K12
      0x7C280200,  //  001D  CALL	R10	1
      0x60280008,  //  001E  GETGBL	R10	G8
      0x5C2C1000,  //  001F  MOVE	R11	R8
      0x7C280200,  //  0020  CALL	R10	1
      0x002A1C0A,  //  0021  ADD	R10	K14	R10
      0x900E1A0A,  //  0022  SETMBR	R3	K13	R10
      0x50280200,  //  0023  LDBOOL	R10	1	0
      0x80041400,  //  0024  RET	1	R10
      0x70020064,  //  0025  JMP		#008B
      0x1C200F0F,  //  0026  EQ	R8	R7	K15
      0x78220002,  //  0027  JMPF	R8	#002B
      0x50200200,  //  0028  LDBOOL	R8	1	0
      0x80041000,  //  0029  RET	1	R8
      0x7002005F,  //  002A  JMP		#008B
      0x1C200F10,  //  002B  EQ	R8	R7	K16
      0x78220002,  //  002C  JMPF	R8	#0030
      0x50200200,  //  002D  LDBOOL	R8	1	0
      0x80041000,  //  002E  RET	1	R8
      0x7002005A,  //  002F  JMP		#008B
      0x1C200F11,  //  0030  EQ	R8	R7	K17
      0x78220019,  //  0031  JMPF	R8	#004C
      0x8C200507,  //  0032  GETMET	R8	R2	K7
      0x58280006,  //  0033  LDCONST	R10	K6
      0x7C200400,  //  0034  CALL	R8	2
      0xB8261000,  //  0035  GETNGBL	R9	K8
      0x8C241309,  //  0036  GETMET	R9	R9	K9
      0x5C2C1000,  //  0037  MOVE	R11	R8
      0x58300006,  //  0038  LDCONST	R12	K6
      0x543600FD,  //  0039  LDINT	R13	254
      0x58380006,  //  003A  LDCONST	R14	K6
      0x543E00FE,  //  003B  LDINT	R15	255
      0x7C240C00,  //  003C  CALL	R9	6
      0x8C28090A,  //  003D  GETMET	R10	R4	K10
      0x60300013,  //  003E  GETGBL	R12	G19
      0x7C300000,  //  003F  CALL	R12	0
      0x98322409,  //  0040  SETIDX	R12	K18	R9
      0x7C280400,  //  0041  CALL	R10	2
      0x8C28010C,  //  0042  GETMET	R10	R0	K12
      0x7C280200,  //  0043  CALL	R10	1
      0x60280008,  //  0044  GETGBL	R10	G8
      0x5C2C1000,  //  0045  MOVE	R11	R8
      0x7C280200,  //  0046  CALL	R10	1
      0x002A260A,  //  0047  ADD	R10	K19	R10
      0x900E1A0A,  //  0048  SETMBR	R3	K13	R10
      0x50280200,  //  0049  LDBOOL	R10	1	0
      0x80041400,  //  004A  RET	1	R10
      0x7002003E,  //  004B  JMP		#008B
      0x54220003,  //  004C  LDINT	R8	4
      0x1C200E08,  //  004D  EQ	R8	R7	R8
      0x78220002,  //  004E  JMPF	R8	#0052
      0x50200200,  //  004F  LDBOOL	R8	1	0
      0x80041000,  //  0050  RET	1	R8
      0x70020038,  //  0051  JMP		#008B
      0x54220004,  //  0052  LDINT	R8	5
      0x1C200E08,  //  0053  EQ	R8	R7	R8
      0x78220002,  //  0054  JMPF	R8	#0058
      0x50200200,  //  0055  LDBOOL	R8	1	0
      0x80041000,  //  0056  RET	1	R8
      0x70020032,  //  0057  JMP		#008B
      0x54220005,  //  0058  LDINT	R8	6
      0x1C200E08,  //  0059  EQ	R8	R7	R8
      0x7822002A,  //  005A  JMPF	R8	#0086
      0x8C200507,  //  005B  GETMET	R8	R2	K7
      0x58280006,  //  005C  LDCONST	R10	K6
      0x7C200400,  //  005D  CALL	R8	2
      0xB8261000,  //  005E  GETNGBL	R9	K8
      0x8C241309,  //  005F  GETMET	R9	R9	K9
      0x5C2C1000,  //  0060  MOVE	R11	R8
      0x58300006,  //  0061  LDCONST	R12	K6
      0x543600FD,  //  0062  LDINT	R13	254
      0x58380006,  //  0063  LDCONST	R14	K6
      0x543E0167,  //  0064  LDINT	R15	360
      0x7C240C00,  //  0065  CALL	R9	6
      0x8C280507,  //  0066  GETMET	R10	R2	K7
      0x5830000F,  //  0067  LDCONST	R12	K15
      0x7C280400,  //  0068  CALL	R10	2
      0xB82E1000,  //  0069  GETNGBL	R11	K8
      0x8C2C1709,  //  006A  GETMET	R11	R11	K9
      0x5C341400,  //  006B  MOVE	R13	R10
      0x58380006,  //  006C  LDCONST	R14	K6
      0x543E00FD,  //  006D  LDINT	R15	254
      0x58400006,  //  006E  LDCONST	R16	K6
      0x544600FE,  //  006F  LDINT	R17	255
      0x7C2C0C00,  //  0070  CALL	R11	6
      0x8C30090A,  //  0071  GETMET	R12	R4	K10
      0x60380013,  //  0072  GETGBL	R14	G19
      0x7C380000,  //  0073  CALL	R14	0
      0x983A1609,  //  0074  SETIDX	R14	K11	R9
      0x983A240B,  //  0075  SETIDX	R14	K18	R11
      0x7C300400,  //  0076  CALL	R12	2
      0x8C30010C,  //  0077  GETMET	R12	R0	K12
      0x7C300200,  //  0078  CALL	R12	1
      0x60300008,  //  0079  GETGBL	R12	G8
      0x5C341000,  //  007A  MOVE	R13	R8
      0x7C300200,  //  007B  CALL	R12	1
      0x00321C0C,  //  007C  ADD	R12	K14	R12
      0x00301914,  //  007D  ADD	R12	R12	K20
      0x60340008,  //  007E  GETGBL	R13	G8
      0x5C381400,  //  007F  MOVE	R14	R10
      0x7C340200,  //  0080  CALL	R13	1
      0x0030180D,  //  0081  ADD	R12	R12	R13
      0x900E1A0C,  //  0082  SETMBR	R3	K13	R12
      0x50300200,  //  0083  LDBOOL	R12	1	0
      0x80041800,  //  0084  RET	1	R12
      0x70020004,  //  0085  JMP		#008B
      0x54220046,  //  0086  LDINT	R8	71
      0x1C200E08,  //  0087  EQ	R8	R7	R8
      0x78220001,  //  0088  JMPF	R8	#008B
      0x50200200,  //  0089  LDBOOL	R8	1	0
      0x80041000,  //  008A  RET	1	R8
      0x70020008,  //  008B  JMP		#0095
      0x60200003,  //  008C  GETGBL	R8	G3
      0x5C240000,  //  008D  MOVE	R9	R0
      0x7C200200,  //  008E  CALL	R8	1
      0x8C201115,  //  008F  GETMET	R8	R8	K21
      0x5C280200,  //  0090  MOVE	R10	R1
      0x5C2C0400,  //  0091  MOVE	R11	R2
      0x5C300600,  //  0092  MOVE	R12	R3
      0x7C200800,  //  0093  CALL	R8	4
      0x80041000,  //  0094  RET	1	R8
      0x80000000,  //  0095  RET	0
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
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(269, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light3) },
        { be_const_key_weak(shadow_hue, 8), be_const_var(0) },
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
        { be_const_key_weak(NAME, 3), be_nested_str_weak(Light_X203_X20RGB) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light3_init_closure) },
        { be_const_key_weak(shadow_sat, -1), be_const_var(1) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light3_invoke_request_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light3_read_attribute_closure) },
        { be_const_key_weak(update_shadow, 1), be_const_closure(Matter_Plugin_Light3_update_shadow_closure) },
    })),
    be_str_weak(Matter_Plugin_Light3)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light3_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light3);
    be_setglobal(vm, "Matter_Plugin_Light3");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
