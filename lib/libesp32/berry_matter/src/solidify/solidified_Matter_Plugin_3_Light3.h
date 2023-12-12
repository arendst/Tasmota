/* Solidification of Matter_Plugin_3_Light3.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light3;

/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(Matter_Plugin_Light3_update_virtual,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
be_local_closure(Matter_Plugin_Light3_set_hue_sat,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(VIRTUAL),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(scale_uint),
    /* K4   */  be_nested_str_weak(light),
    /* K5   */  be_nested_str_weak(set),
    /* K6   */  be_nested_str_weak(hue),
    /* K7   */  be_nested_str_weak(sat),
    /* K8   */  be_nested_str_weak(update_shadow),
    /* K9   */  be_nested_str_weak(shadow_hue),
    /* K10  */  be_nested_str_weak(attribute_updated),
    /* K11  */  be_nested_str_weak(shadow_sat),
    /* K12  */  be_const_int(1),
    }),
    be_str_weak(set_hue_sat),
    &be_const_str_solidified,
    ( &(const binstruction[104]) {  /* code */
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
      0x740E003A,  //  0015  JMPT	R3	#0051
      0x4C0C0000,  //  0016  LDNIL	R3
      0x200C0203,  //  0017  NE	R3	R1	R3
      0x780E0008,  //  0018  JMPF	R3	#0022
      0xB80E0400,  //  0019  GETNGBL	R3	K2
      0x8C0C0703,  //  001A  GETMET	R3	R3	K3
      0x5C140200,  //  001B  MOVE	R5	R1
      0x58180000,  //  001C  LDCONST	R6	K0
      0x541E00FD,  //  001D  LDINT	R7	254
      0x58200000,  //  001E  LDCONST	R8	K0
      0x54260167,  //  001F  LDINT	R9	360
      0x7C0C0C00,  //  0020  CALL	R3	6
      0x70020000,  //  0021  JMP		#0023
      0x4C0C0000,  //  0022  LDNIL	R3
      0x4C100000,  //  0023  LDNIL	R4
      0x20100404,  //  0024  NE	R4	R2	R4
      0x78120008,  //  0025  JMPF	R4	#002F
      0xB8120400,  //  0026  GETNGBL	R4	K2
      0x8C100903,  //  0027  GETMET	R4	R4	K3
      0x5C180400,  //  0028  MOVE	R6	R2
      0x581C0000,  //  0029  LDCONST	R7	K0
      0x542200FD,  //  002A  LDINT	R8	254
      0x58240000,  //  002B  LDCONST	R9	K0
      0x542A00FE,  //  002C  LDINT	R10	255
      0x7C100C00,  //  002D  CALL	R4	6
      0x70020000,  //  002E  JMP		#0030
      0x4C100000,  //  002F  LDNIL	R4
      0x4C140000,  //  0030  LDNIL	R5
      0x20140605,  //  0031  NE	R5	R3	R5
      0x7816000A,  //  0032  JMPF	R5	#003E
      0x4C140000,  //  0033  LDNIL	R5
      0x20140805,  //  0034  NE	R5	R4	R5
      0x78160007,  //  0035  JMPF	R5	#003E
      0xB8160800,  //  0036  GETNGBL	R5	K4
      0x8C140B05,  //  0037  GETMET	R5	R5	K5
      0x601C0013,  //  0038  GETGBL	R7	G19
      0x7C1C0000,  //  0039  CALL	R7	0
      0x981E0C03,  //  003A  SETIDX	R7	K6	R3
      0x981E0E04,  //  003B  SETIDX	R7	K7	R4
      0x7C140400,  //  003C  CALL	R5	2
      0x7002000F,  //  003D  JMP		#004E
      0x4C140000,  //  003E  LDNIL	R5
      0x20140605,  //  003F  NE	R5	R3	R5
      0x78160006,  //  0040  JMPF	R5	#0048
      0xB8160800,  //  0041  GETNGBL	R5	K4
      0x8C140B05,  //  0042  GETMET	R5	R5	K5
      0x601C0013,  //  0043  GETGBL	R7	G19
      0x7C1C0000,  //  0044  CALL	R7	0
      0x981E0C03,  //  0045  SETIDX	R7	K6	R3
      0x7C140400,  //  0046  CALL	R5	2
      0x70020005,  //  0047  JMP		#004E
      0xB8160800,  //  0048  GETNGBL	R5	K4
      0x8C140B05,  //  0049  GETMET	R5	R5	K5
      0x601C0013,  //  004A  GETGBL	R7	G19
      0x7C1C0000,  //  004B  CALL	R7	0
      0x981E0E04,  //  004C  SETIDX	R7	K7	R4
      0x7C140400,  //  004D  CALL	R5	2
      0x8C140108,  //  004E  GETMET	R5	R0	K8
      0x7C140200,  //  004F  CALL	R5	1
      0x70020015,  //  0050  JMP		#0067
      0x4C0C0000,  //  0051  LDNIL	R3
      0x200C0203,  //  0052  NE	R3	R1	R3
      0x780E0007,  //  0053  JMPF	R3	#005C
      0x880C0109,  //  0054  GETMBR	R3	R0	K9
      0x200C0203,  //  0055  NE	R3	R1	R3
      0x780E0004,  //  0056  JMPF	R3	#005C
      0x8C0C010A,  //  0057  GETMET	R3	R0	K10
      0x541602FF,  //  0058  LDINT	R5	768
      0x58180000,  //  0059  LDCONST	R6	K0
      0x7C0C0600,  //  005A  CALL	R3	3
      0x90021201,  //  005B  SETMBR	R0	K9	R1
      0x4C0C0000,  //  005C  LDNIL	R3
      0x200C0403,  //  005D  NE	R3	R2	R3
      0x780E0007,  //  005E  JMPF	R3	#0067
      0x880C010B,  //  005F  GETMBR	R3	R0	K11
      0x200C0403,  //  0060  NE	R3	R2	R3
      0x780E0004,  //  0061  JMPF	R3	#0067
      0x8C0C010A,  //  0062  GETMET	R3	R0	K10
      0x541602FF,  //  0063  LDINT	R5	768
      0x5818000C,  //  0064  LDCONST	R6	K12
      0x7C0C0600,  //  0065  CALL	R3	3
      0x90021602,  //  0066  SETMBR	R0	K11	R2
      0x80000000,  //  0067  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light3_invoke_request,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(findsubval),
    /* K8   */  be_nested_str_weak(set_hue_sat),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(hue_X3A),
    /* K11  */  be_nested_str_weak(publish_command),
    /* K12  */  be_nested_str_weak(Hue),
    /* K13  */  be_const_int(1),
    /* K14  */  be_const_int(2),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(sat_X3A),
    /* K17  */  be_nested_str_weak(Sat),
    /* K18  */  be_nested_str_weak(_X20sat_X3A),
    /* K19  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[122]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x542202FF,  //  0005  LDINT	R8	768
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220067,  //  0007  JMPF	R8	#0070
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x1C200F06,  //  000A  EQ	R8	R7	K6
      0x78220012,  //  000B  JMPF	R8	#001F
      0x8C200507,  //  000C  GETMET	R8	R2	K7
      0x58280006,  //  000D  LDCONST	R10	K6
      0x7C200400,  //  000E  CALL	R8	2
      0x8C240108,  //  000F  GETMET	R9	R0	K8
      0x5C2C1000,  //  0010  MOVE	R11	R8
      0x4C300000,  //  0011  LDNIL	R12
      0x7C240600,  //  0012  CALL	R9	3
      0x60240008,  //  0013  GETGBL	R9	G8
      0x5C281000,  //  0014  MOVE	R10	R8
      0x7C240200,  //  0015  CALL	R9	1
      0x00261409,  //  0016  ADD	R9	K10	R9
      0x900E1209,  //  0017  SETMBR	R3	K9	R9
      0x8C24010B,  //  0018  GETMET	R9	R0	K11
      0x582C000C,  //  0019  LDCONST	R11	K12
      0x5C301000,  //  001A  MOVE	R12	R8
      0x7C240600,  //  001B  CALL	R9	3
      0x50240200,  //  001C  LDBOOL	R9	1	0
      0x80041200,  //  001D  RET	1	R9
      0x7002004F,  //  001E  JMP		#006F
      0x1C200F0D,  //  001F  EQ	R8	R7	K13
      0x78220002,  //  0020  JMPF	R8	#0024
      0x50200200,  //  0021  LDBOOL	R8	1	0
      0x80041000,  //  0022  RET	1	R8
      0x7002004A,  //  0023  JMP		#006F
      0x1C200F0E,  //  0024  EQ	R8	R7	K14
      0x78220002,  //  0025  JMPF	R8	#0029
      0x50200200,  //  0026  LDBOOL	R8	1	0
      0x80041000,  //  0027  RET	1	R8
      0x70020045,  //  0028  JMP		#006F
      0x1C200F0F,  //  0029  EQ	R8	R7	K15
      0x78220012,  //  002A  JMPF	R8	#003E
      0x8C200507,  //  002B  GETMET	R8	R2	K7
      0x58280006,  //  002C  LDCONST	R10	K6
      0x7C200400,  //  002D  CALL	R8	2
      0x8C240108,  //  002E  GETMET	R9	R0	K8
      0x4C2C0000,  //  002F  LDNIL	R11
      0x5C301000,  //  0030  MOVE	R12	R8
      0x7C240600,  //  0031  CALL	R9	3
      0x60240008,  //  0032  GETGBL	R9	G8
      0x5C281000,  //  0033  MOVE	R10	R8
      0x7C240200,  //  0034  CALL	R9	1
      0x00262009,  //  0035  ADD	R9	K16	R9
      0x900E1209,  //  0036  SETMBR	R3	K9	R9
      0x8C24010B,  //  0037  GETMET	R9	R0	K11
      0x582C0011,  //  0038  LDCONST	R11	K17
      0x5C301000,  //  0039  MOVE	R12	R8
      0x7C240600,  //  003A  CALL	R9	3
      0x50240200,  //  003B  LDBOOL	R9	1	0
      0x80041200,  //  003C  RET	1	R9
      0x70020030,  //  003D  JMP		#006F
      0x54220003,  //  003E  LDINT	R8	4
      0x1C200E08,  //  003F  EQ	R8	R7	R8
      0x78220002,  //  0040  JMPF	R8	#0044
      0x50200200,  //  0041  LDBOOL	R8	1	0
      0x80041000,  //  0042  RET	1	R8
      0x7002002A,  //  0043  JMP		#006F
      0x54220004,  //  0044  LDINT	R8	5
      0x1C200E08,  //  0045  EQ	R8	R7	R8
      0x78220002,  //  0046  JMPF	R8	#004A
      0x50200200,  //  0047  LDBOOL	R8	1	0
      0x80041000,  //  0048  RET	1	R8
      0x70020024,  //  0049  JMP		#006F
      0x54220005,  //  004A  LDINT	R8	6
      0x1C200E08,  //  004B  EQ	R8	R7	R8
      0x7822001C,  //  004C  JMPF	R8	#006A
      0x8C200507,  //  004D  GETMET	R8	R2	K7
      0x58280006,  //  004E  LDCONST	R10	K6
      0x7C200400,  //  004F  CALL	R8	2
      0x8C240507,  //  0050  GETMET	R9	R2	K7
      0x582C000D,  //  0051  LDCONST	R11	K13
      0x7C240400,  //  0052  CALL	R9	2
      0x8C280108,  //  0053  GETMET	R10	R0	K8
      0x5C301000,  //  0054  MOVE	R12	R8
      0x5C341200,  //  0055  MOVE	R13	R9
      0x7C280600,  //  0056  CALL	R10	3
      0x60280008,  //  0057  GETGBL	R10	G8
      0x5C2C1000,  //  0058  MOVE	R11	R8
      0x7C280200,  //  0059  CALL	R10	1
      0x002A140A,  //  005A  ADD	R10	K10	R10
      0x00281512,  //  005B  ADD	R10	R10	K18
      0x602C0008,  //  005C  GETGBL	R11	G8
      0x5C301200,  //  005D  MOVE	R12	R9
      0x7C2C0200,  //  005E  CALL	R11	1
      0x0028140B,  //  005F  ADD	R10	R10	R11
      0x900E120A,  //  0060  SETMBR	R3	K9	R10
      0x8C28010B,  //  0061  GETMET	R10	R0	K11
      0x5830000C,  //  0062  LDCONST	R12	K12
      0x5C341000,  //  0063  MOVE	R13	R8
      0x58380011,  //  0064  LDCONST	R14	K17
      0x5C3C1200,  //  0065  MOVE	R15	R9
      0x7C280A00,  //  0066  CALL	R10	5
      0x50280200,  //  0067  LDBOOL	R10	1	0
      0x80041400,  //  0068  RET	1	R10
      0x70020004,  //  0069  JMP		#006F
      0x54220046,  //  006A  LDINT	R8	71
      0x1C200E08,  //  006B  EQ	R8	R7	R8
      0x78220001,  //  006C  JMPF	R8	#006F
      0x50200200,  //  006D  LDBOOL	R8	1	0
      0x80041000,  //  006E  RET	1	R8
      0x70020008,  //  006F  JMP		#0079
      0x60200003,  //  0070  GETGBL	R8	G3
      0x5C240000,  //  0071  MOVE	R9	R0
      0x7C200200,  //  0072  CALL	R8	1
      0x8C201113,  //  0073  GETMET	R8	R8	K19
      0x5C280200,  //  0074  MOVE	R10	R1
      0x5C2C0400,  //  0075  MOVE	R11	R2
      0x5C300600,  //  0076  MOVE	R12	R3
      0x7C200800,  //  0077  CALL	R8	4
      0x80041000,  //  0078  RET	1	R8
      0x80000000,  //  0079  RET	0
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(update_shadow),
    /* K1   */  be_nested_str_weak(VIRTUAL),
    /* K2   */  be_nested_str_weak(light),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(hue),
    /* K6   */  be_nested_str_weak(sat),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(scale_uint),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(shadow_hue),
    /* K11  */  be_nested_str_weak(shadow_sat),
    /* K12  */  be_nested_str_weak(attribute_updated),
    /* K13  */  be_const_int(1),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x88040101,  //  0005  GETMBR	R1	R0	K1
      0x74060039,  //  0006  JMPT	R1	#0041
      0xA4060400,  //  0007  IMPORT	R1	K2
      0x8C080303,  //  0008  GETMET	R2	R1	K3
      0x7C080200,  //  0009  CALL	R2	1
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0033,  //  000C  JMPF	R3	#0041
      0x8C0C0504,  //  000D  GETMET	R3	R2	K4
      0x58140005,  //  000E  LDCONST	R5	K5
      0x4C180000,  //  000F  LDNIL	R6
      0x7C0C0600,  //  0010  CALL	R3	3
      0x8C100504,  //  0011  GETMET	R4	R2	K4
      0x58180006,  //  0012  LDCONST	R6	K6
      0x4C1C0000,  //  0013  LDNIL	R7
      0x7C100600,  //  0014  CALL	R4	3
      0x4C140000,  //  0015  LDNIL	R5
      0x20140605,  //  0016  NE	R5	R3	R5
      0x78160009,  //  0017  JMPF	R5	#0022
      0xB8160E00,  //  0018  GETNGBL	R5	K7
      0x8C140B08,  //  0019  GETMET	R5	R5	K8
      0x5C1C0600,  //  001A  MOVE	R7	R3
      0x58200009,  //  001B  LDCONST	R8	K9
      0x54260167,  //  001C  LDINT	R9	360
      0x58280009,  //  001D  LDCONST	R10	K9
      0x542E00FD,  //  001E  LDINT	R11	254
      0x7C140C00,  //  001F  CALL	R5	6
      0x5C0C0A00,  //  0020  MOVE	R3	R5
      0x70020000,  //  0021  JMP		#0023
      0x880C010A,  //  0022  GETMBR	R3	R0	K10
      0x4C140000,  //  0023  LDNIL	R5
      0x20140805,  //  0024  NE	R5	R4	R5
      0x78160009,  //  0025  JMPF	R5	#0030
      0xB8160E00,  //  0026  GETNGBL	R5	K7
      0x8C140B08,  //  0027  GETMET	R5	R5	K8
      0x5C1C0800,  //  0028  MOVE	R7	R4
      0x58200009,  //  0029  LDCONST	R8	K9
      0x542600FE,  //  002A  LDINT	R9	255
      0x58280009,  //  002B  LDCONST	R10	K9
      0x542E00FD,  //  002C  LDINT	R11	254
      0x7C140C00,  //  002D  CALL	R5	6
      0x5C100A00,  //  002E  MOVE	R4	R5
      0x70020000,  //  002F  JMP		#0031
      0x8810010B,  //  0030  GETMBR	R4	R0	K11
      0x8814010A,  //  0031  GETMBR	R5	R0	K10
      0x20140605,  //  0032  NE	R5	R3	R5
      0x78160004,  //  0033  JMPF	R5	#0039
      0x8C14010C,  //  0034  GETMET	R5	R0	K12
      0x541E02FF,  //  0035  LDINT	R7	768
      0x58200009,  //  0036  LDCONST	R8	K9
      0x7C140600,  //  0037  CALL	R5	3
      0x90021403,  //  0038  SETMBR	R0	K10	R3
      0x8814010B,  //  0039  GETMBR	R5	R0	K11
      0x20140805,  //  003A  NE	R5	R4	R5
      0x78160004,  //  003B  JMPF	R5	#0041
      0x8C14010C,  //  003C  GETMET	R5	R0	K12
      0x541E02FF,  //  003D  LDINT	R7	768
      0x5820000D,  //  003E  LDCONST	R8	K13
      0x7C140600,  //  003F  CALL	R5	3
      0x90021604,  //  0040  SETMBR	R0	K11	R4
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light3_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
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
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(shadow_hue),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(shadow_sat),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[107]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0059,  //  0006  JMPF	R7	#0061
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0005,  //  000A  JMPF	R7	#0011
      0x8C1C0706,  //  000B  GETMET	R7	R3	K6
      0x88240907,  //  000C  GETMBR	R9	R4	K7
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x7002004E,  //  0010  JMP		#0060
      0x1C1C0D09,  //  0011  EQ	R7	R6	K9
      0x781E0005,  //  0012  JMPF	R7	#0019
      0x8C1C0706,  //  0013  GETMET	R7	R3	K6
      0x88240907,  //  0014  GETMBR	R9	R4	K7
      0x8828010A,  //  0015  GETMBR	R10	R0	K10
      0x7C1C0600,  //  0016  CALL	R7	3
      0x80040E00,  //  0017  RET	1	R7
      0x70020046,  //  0018  JMP		#0060
      0x541E0006,  //  0019  LDINT	R7	7
      0x1C1C0C07,  //  001A  EQ	R7	R6	R7
      0x781E0005,  //  001B  JMPF	R7	#0022
      0x8C1C0706,  //  001C  GETMET	R7	R3	K6
      0x88240907,  //  001D  GETMBR	R9	R4	K7
      0x58280005,  //  001E  LDCONST	R10	K5
      0x7C1C0600,  //  001F  CALL	R7	3
      0x80040E00,  //  0020  RET	1	R7
      0x7002003D,  //  0021  JMP		#0060
      0x541E0007,  //  0022  LDINT	R7	8
      0x1C1C0C07,  //  0023  EQ	R7	R6	R7
      0x781E0005,  //  0024  JMPF	R7	#002B
      0x8C1C0706,  //  0025  GETMET	R7	R3	K6
      0x88240907,  //  0026  GETMBR	R9	R4	K7
      0x58280005,  //  0027  LDCONST	R10	K5
      0x7C1C0600,  //  0028  CALL	R7	3
      0x80040E00,  //  0029  RET	1	R7
      0x70020034,  //  002A  JMP		#0060
      0x541E000E,  //  002B  LDINT	R7	15
      0x1C1C0C07,  //  002C  EQ	R7	R6	R7
      0x781E0005,  //  002D  JMPF	R7	#0034
      0x8C1C0706,  //  002E  GETMET	R7	R3	K6
      0x88240907,  //  002F  GETMBR	R9	R4	K7
      0x58280005,  //  0030  LDCONST	R10	K5
      0x7C1C0600,  //  0031  CALL	R7	3
      0x80040E00,  //  0032  RET	1	R7
      0x7002002B,  //  0033  JMP		#0060
      0x541E4000,  //  0034  LDINT	R7	16385
      0x1C1C0C07,  //  0035  EQ	R7	R6	R7
      0x781E0005,  //  0036  JMPF	R7	#003D
      0x8C1C0706,  //  0037  GETMET	R7	R3	K6
      0x88240907,  //  0038  GETMBR	R9	R4	K7
      0x58280005,  //  0039  LDCONST	R10	K5
      0x7C1C0600,  //  003A  CALL	R7	3
      0x80040E00,  //  003B  RET	1	R7
      0x70020022,  //  003C  JMP		#0060
      0x541E4009,  //  003D  LDINT	R7	16394
      0x1C1C0C07,  //  003E  EQ	R7	R6	R7
      0x781E0005,  //  003F  JMPF	R7	#0046
      0x8C1C0706,  //  0040  GETMET	R7	R3	K6
      0x88240907,  //  0041  GETMBR	R9	R4	K7
      0x58280009,  //  0042  LDCONST	R10	K9
      0x7C1C0600,  //  0043  CALL	R7	3
      0x80040E00,  //  0044  RET	1	R7
      0x70020019,  //  0045  JMP		#0060
      0x541E000F,  //  0046  LDINT	R7	16
      0x1C1C0C07,  //  0047  EQ	R7	R6	R7
      0x781E0005,  //  0048  JMPF	R7	#004F
      0x8C1C0706,  //  0049  GETMET	R7	R3	K6
      0x88240907,  //  004A  GETMBR	R9	R4	K7
      0x58280005,  //  004B  LDCONST	R10	K5
      0x7C1C0600,  //  004C  CALL	R7	3
      0x80040E00,  //  004D  RET	1	R7
      0x70020010,  //  004E  JMP		#0060
      0x541EFFFB,  //  004F  LDINT	R7	65532
      0x1C1C0C07,  //  0050  EQ	R7	R6	R7
      0x781E0005,  //  0051  JMPF	R7	#0058
      0x8C1C0706,  //  0052  GETMET	R7	R3	K6
      0x8824090B,  //  0053  GETMBR	R9	R4	K11
      0x58280009,  //  0054  LDCONST	R10	K9
      0x7C1C0600,  //  0055  CALL	R7	3
      0x80040E00,  //  0056  RET	1	R7
      0x70020007,  //  0057  JMP		#0060
      0x541EFFFC,  //  0058  LDINT	R7	65533
      0x1C1C0C07,  //  0059  EQ	R7	R6	R7
      0x781E0004,  //  005A  JMPF	R7	#0060
      0x8C1C0706,  //  005B  GETMET	R7	R3	K6
      0x8824090B,  //  005C  GETMBR	R9	R4	K11
      0x542A0004,  //  005D  LDINT	R10	5
      0x7C1C0600,  //  005E  CALL	R7	3
      0x80040E00,  //  005F  RET	1	R7
      0x70020008,  //  0060  JMP		#006A
      0x601C0003,  //  0061  GETGBL	R7	G3
      0x5C200000,  //  0062  MOVE	R8	R0
      0x7C1C0200,  //  0063  CALL	R7	1
      0x8C1C0F0C,  //  0064  GETMET	R7	R7	K12
      0x5C240200,  //  0065  MOVE	R9	R1
      0x5C280400,  //  0066  MOVE	R10	R2
      0x5C2C0600,  //  0067  MOVE	R11	R3
      0x7C1C0800,  //  0068  CALL	R7	4
      0x80040E00,  //  0069  RET	1	R7
      0x80000000,  //  006A  RET	0
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
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(shadow_hue, -1), be_const_var(0) },
        { be_const_key_weak(shadow_sat, -1), be_const_var(1) },
        { be_const_key_weak(update_virtual, 8), be_const_closure(Matter_Plugin_Light3_update_virtual_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Light3_update_shadow_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Light3_read_attribute_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light3_invoke_request_closure) },
        { be_const_key_weak(init, 9), be_const_closure(Matter_Plugin_Light3_init_closure) },
        { be_const_key_weak(set_hue_sat, 3), be_const_closure(Matter_Plugin_Light3_set_hue_sat_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light3) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X203_X20RGB) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        { be_const_key_weak(UPDATE_COMMANDS, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
        be_nested_str_weak(Bri),
        be_nested_str_weak(Hue),
        be_nested_str_weak(Sat),
    }))    ) } )) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(269, -1), be_const_int(2) },
    }))    ) } )) },
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
