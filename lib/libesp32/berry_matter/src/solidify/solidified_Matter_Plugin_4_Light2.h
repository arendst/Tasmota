/* Solidification of Matter_Plugin_4_Light2.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Light2;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Light2_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_ct),
    /* K2   */  be_nested_str_weak(update_ct_minmax),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x54120144,  //  0008  LDINT	R4	325
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x8C100102,  //  000A  GETMET	R4	R0	K2
      0x7C100200,  //  000B  CALL	R4	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Light2_invoke_request,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(command),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_nested_str_weak(findsubval),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(set_ct),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(ct_X3A),
    /* K11  */  be_nested_str_weak(publish_command),
    /* K12  */  be_nested_str_weak(CT),
    /* K13  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xB8160200,  //  0001  GETNGBL	R5	K1
      0x88140B02,  //  0002  GETMBR	R5	R5	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x881C0704,  //  0004  GETMBR	R7	R3	K4
      0x542202FF,  //  0005  LDINT	R8	768
      0x1C200C08,  //  0006  EQ	R8	R6	R8
      0x78220028,  //  0007  JMPF	R8	#0031
      0x8C200105,  //  0008  GETMET	R8	R0	K5
      0x7C200200,  //  0009  CALL	R8	1
      0x54220009,  //  000A  LDINT	R8	10
      0x1C200E08,  //  000B  EQ	R8	R7	R8
      0x78220011,  //  000C  JMPF	R8	#001F
      0x8C200506,  //  000D  GETMET	R8	R2	K6
      0x58280007,  //  000E  LDCONST	R10	K7
      0x7C200400,  //  000F  CALL	R8	2
      0x8C240108,  //  0010  GETMET	R9	R0	K8
      0x5C2C1000,  //  0011  MOVE	R11	R8
      0x7C240400,  //  0012  CALL	R9	2
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
      0x70020010,  //  001E  JMP		#0030
      0x54220046,  //  001F  LDINT	R8	71
      0x1C200E08,  //  0020  EQ	R8	R7	R8
      0x78220002,  //  0021  JMPF	R8	#0025
      0x50200200,  //  0022  LDBOOL	R8	1	0
      0x80041000,  //  0023  RET	1	R8
      0x7002000A,  //  0024  JMP		#0030
      0x5422004A,  //  0025  LDINT	R8	75
      0x1C200E08,  //  0026  EQ	R8	R7	R8
      0x78220002,  //  0027  JMPF	R8	#002B
      0x50200200,  //  0028  LDBOOL	R8	1	0
      0x80041000,  //  0029  RET	1	R8
      0x70020004,  //  002A  JMP		#0030
      0x5422004B,  //  002B  LDINT	R8	76
      0x1C200E08,  //  002C  EQ	R8	R7	R8
      0x78220001,  //  002D  JMPF	R8	#0030
      0x50200200,  //  002E  LDBOOL	R8	1	0
      0x80041000,  //  002F  RET	1	R8
      0x70020008,  //  0030  JMP		#003A
      0x60200003,  //  0031  GETGBL	R8	G3
      0x5C240000,  //  0032  MOVE	R9	R0
      0x7C200200,  //  0033  CALL	R8	1
      0x8C20110D,  //  0034  GETMET	R8	R8	K13
      0x5C280200,  //  0035  MOVE	R10	R1
      0x5C2C0400,  //  0036  MOVE	R11	R2
      0x5C300600,  //  0037  MOVE	R12	R3
      0x7C200800,  //  0038  CALL	R8	4
      0x80041000,  //  0039  RET	1	R8
      0x80000000,  //  003A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Light2_update_shadow,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(light),
    /* K1   */  be_nested_str_weak(update_ct_minmax),
    /* K2   */  be_nested_str_weak(update_shadow),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(ct),
    /* K6   */  be_nested_str_weak(shadow_ct),
    /* K7   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x60080003,  //  0003  GETGBL	R2	G3
      0x5C0C0000,  //  0004  MOVE	R3	R0
      0x7C080200,  //  0005  CALL	R2	1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080303,  //  0008  GETMET	R2	R1	K3
      0x7C080200,  //  0009  CALL	R2	1
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E000F,  //  000C  JMPF	R3	#001D
      0x8C0C0504,  //  000D  GETMET	R3	R2	K4
      0x58140005,  //  000E  LDCONST	R5	K5
      0x4C180000,  //  000F  LDNIL	R6
      0x7C0C0600,  //  0010  CALL	R3	3
      0x4C100000,  //  0011  LDNIL	R4
      0x1C100604,  //  0012  EQ	R4	R3	R4
      0x78120000,  //  0013  JMPF	R4	#0015
      0x880C0106,  //  0014  GETMBR	R3	R0	K6
      0x88100106,  //  0015  GETMBR	R4	R0	K6
      0x20100604,  //  0016  NE	R4	R3	R4
      0x78120004,  //  0017  JMPF	R4	#001D
      0x8C100107,  //  0018  GETMET	R4	R0	K7
      0x541A02FF,  //  0019  LDINT	R6	768
      0x541E0006,  //  001A  LDINT	R7	7
      0x7C100600,  //  001B  CALL	R4	3
      0x90020C03,  //  001C  SETMBR	R0	K6	R3
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(Matter_Plugin_Light2_update_virtual,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(CT),
    /* K2   */  be_nested_str_weak(set_ct),
    /* K3   */  be_nested_str_weak(update_virtual),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x8C0C0300,  //  0001  GETMET	R3	R1	K0
      0x58140001,  //  0002  LDCONST	R5	K1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x7C080200,  //  0004  CALL	R2	1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x200C0403,  //  0006  NE	R3	R2	R3
      0x780E0002,  //  0007  JMPF	R3	#000B
      0x8C0C0102,  //  0008  GETMET	R3	R0	K2
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x600C0003,  //  000B  GETGBL	R3	G3
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C0703,  //  000E  GETMET	R3	R3	K3
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ct
********************************************************************/
be_local_closure(Matter_Plugin_Light2_set_ct,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(ct_min),
    /* K1   */  be_nested_str_weak(ct_max),
    /* K2   */  be_nested_str_weak(VIRTUAL),
    /* K3   */  be_nested_str_weak(light),
    /* K4   */  be_nested_str_weak(set),
    /* K5   */  be_nested_str_weak(ct),
    /* K6   */  be_nested_str_weak(update_shadow),
    /* K7   */  be_nested_str_weak(shadow_ct),
    /* K8   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(set_ct),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x14080202,  //  0001  LT	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x24080202,  //  0005  GT	R2	R1	R2
      0x780A0000,  //  0006  JMPF	R2	#0008
      0x88040101,  //  0007  GETMBR	R1	R0	K1
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x740A0008,  //  0009  JMPT	R2	#0013
      0xA40A0600,  //  000A  IMPORT	R2	K3
      0x8C0C0504,  //  000B  GETMET	R3	R2	K4
      0x60140013,  //  000C  GETGBL	R5	G19
      0x7C140000,  //  000D  CALL	R5	0
      0x98160A01,  //  000E  SETIDX	R5	K5	R1
      0x7C0C0400,  //  000F  CALL	R3	2
      0x8C0C0106,  //  0010  GETMET	R3	R0	K6
      0x7C0C0200,  //  0011  CALL	R3	1
      0x70020007,  //  0012  JMP		#001B
      0x88080107,  //  0013  GETMBR	R2	R0	K7
      0x20080202,  //  0014  NE	R2	R1	R2
      0x780A0004,  //  0015  JMPF	R2	#001B
      0x8C080108,  //  0016  GETMET	R2	R0	K8
      0x541202FF,  //  0017  LDINT	R4	768
      0x54160006,  //  0018  LDINT	R5	7
      0x7C080600,  //  0019  CALL	R2	3
      0x90020E01,  //  001A  SETMBR	R0	K7	R1
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Light2_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_nested_str_weak(set),
    /* K6   */  be_nested_str_weak(U1),
    /* K7   */  be_nested_str_weak(shadow_ct),
    /* K8   */  be_const_int(2),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(ct_min),
    /* K11  */  be_nested_str_weak(ct_max),
    /* K12  */  be_nested_str_weak(U4),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[82]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0040,  //  0006  JMPF	R7	#0048
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x541E0006,  //  0009  LDINT	R7	7
      0x1C1C0C07,  //  000A  EQ	R7	R6	R7
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0705,  //  000C  GETMET	R7	R3	K5
      0x88240906,  //  000D  GETMBR	R9	R4	K6
      0x88280107,  //  000E  GETMBR	R10	R0	K7
      0x7C1C0600,  //  000F  CALL	R7	3
      0x80040E00,  //  0010  RET	1	R7
      0x70020034,  //  0011  JMP		#0047
      0x541E0007,  //  0012  LDINT	R7	8
      0x1C1C0C07,  //  0013  EQ	R7	R6	R7
      0x781E0005,  //  0014  JMPF	R7	#001B
      0x8C1C0705,  //  0015  GETMET	R7	R3	K5
      0x88240906,  //  0016  GETMBR	R9	R4	K6
      0x58280008,  //  0017  LDCONST	R10	K8
      0x7C1C0600,  //  0018  CALL	R7	3
      0x80040E00,  //  0019  RET	1	R7
      0x7002002B,  //  001A  JMP		#0047
      0x541E000E,  //  001B  LDINT	R7	15
      0x1C1C0C07,  //  001C  EQ	R7	R6	R7
      0x781E0005,  //  001D  JMPF	R7	#0024
      0x8C1C0705,  //  001E  GETMET	R7	R3	K5
      0x88240906,  //  001F  GETMBR	R9	R4	K6
      0x58280009,  //  0020  LDCONST	R10	K9
      0x7C1C0600,  //  0021  CALL	R7	3
      0x80040E00,  //  0022  RET	1	R7
      0x70020022,  //  0023  JMP		#0047
      0x541E400A,  //  0024  LDINT	R7	16395
      0x1C1C0C07,  //  0025  EQ	R7	R6	R7
      0x781E0005,  //  0026  JMPF	R7	#002D
      0x8C1C0705,  //  0027  GETMET	R7	R3	K5
      0x88240906,  //  0028  GETMBR	R9	R4	K6
      0x8828010A,  //  0029  GETMBR	R10	R0	K10
      0x7C1C0600,  //  002A  CALL	R7	3
      0x80040E00,  //  002B  RET	1	R7
      0x70020019,  //  002C  JMP		#0047
      0x541E400B,  //  002D  LDINT	R7	16396
      0x1C1C0C07,  //  002E  EQ	R7	R6	R7
      0x781E0005,  //  002F  JMPF	R7	#0036
      0x8C1C0705,  //  0030  GETMET	R7	R3	K5
      0x88240906,  //  0031  GETMBR	R9	R4	K6
      0x8828010B,  //  0032  GETMBR	R10	R0	K11
      0x7C1C0600,  //  0033  CALL	R7	3
      0x80040E00,  //  0034  RET	1	R7
      0x70020010,  //  0035  JMP		#0047
      0x541EFFFB,  //  0036  LDINT	R7	65532
      0x1C1C0C07,  //  0037  EQ	R7	R6	R7
      0x781E0005,  //  0038  JMPF	R7	#003F
      0x8C1C0705,  //  0039  GETMET	R7	R3	K5
      0x8824090C,  //  003A  GETMBR	R9	R4	K12
      0x542A000F,  //  003B  LDINT	R10	16
      0x7C1C0600,  //  003C  CALL	R7	3
      0x80040E00,  //  003D  RET	1	R7
      0x70020007,  //  003E  JMP		#0047
      0x541EFFFC,  //  003F  LDINT	R7	65533
      0x1C1C0C07,  //  0040  EQ	R7	R6	R7
      0x781E0004,  //  0041  JMPF	R7	#0047
      0x8C1C0705,  //  0042  GETMET	R7	R3	K5
      0x8824090C,  //  0043  GETMBR	R9	R4	K12
      0x542A0004,  //  0044  LDINT	R10	5
      0x7C1C0600,  //  0045  CALL	R7	3
      0x80040E00,  //  0046  RET	1	R7
      0x70020008,  //  0047  JMP		#0051
      0x601C0003,  //  0048  GETGBL	R7	G3
      0x5C200000,  //  0049  MOVE	R8	R0
      0x7C1C0200,  //  004A  CALL	R7	1
      0x8C1C0F0D,  //  004B  GETMET	R7	R7	K13
      0x5C240200,  //  004C  MOVE	R9	R1
      0x5C280400,  //  004D  MOVE	R10	R2
      0x5C2C0600,  //  004E  MOVE	R11	R3
      0x7C1C0800,  //  004F  CALL	R7	4
      0x80040E00,  //  0050  RET	1	R7
      0x80000000,  //  0051  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_ct_minmax
********************************************************************/
be_local_closure(Matter_Plugin_Light2_update_ct_minmax,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(get_option),
    /* K2   */  be_nested_str_weak(ct_min),
    /* K3   */  be_nested_str_weak(ct_max),
    }),
    be_str_weak(update_ct_minmax),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x540E0051,  //  0002  LDINT	R3	82
      0x7C040400,  //  0003  CALL	R1	2
      0x78060001,  //  0004  JMPF	R1	#0007
      0x540A00C7,  //  0005  LDINT	R2	200
      0x70020000,  //  0006  JMP		#0008
      0x540A0098,  //  0007  LDINT	R2	153
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x540A017B,  //  000A  LDINT	R2	380
      0x70020000,  //  000B  JMP		#000D
      0x540A01F3,  //  000C  LDINT	R2	500
      0x90020602,  //  000D  SETMBR	R0	K3	R2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light2
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_class(Matter_Plugin_Light2,
    3,
    &be_class_Matter_Plugin_Light1,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Light2_init_closure) },
        { be_const_key_weak(shadow_ct, -1), be_const_var(0) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Light2_invoke_request_closure) },
        { be_const_key_weak(ct_max, -1), be_const_var(2) },
        { be_const_key_weak(update_shadow, 10), be_const_closure(Matter_Plugin_Light2_update_shadow_closure) },
        { be_const_key_weak(update_virtual, 8), be_const_closure(Matter_Plugin_Light2_update_virtual_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X202_X20CT) },
        { be_const_key_weak(set_ct, -1), be_const_closure(Matter_Plugin_Light2_set_ct_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(268, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
        be_nested_str_weak(Bri),
        be_nested_str_weak(CT),
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, 14), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(7),
        be_const_int(8),
        be_const_int(15),
        be_const_int(16395),
        be_const_int(16396),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(update_ct_minmax, -1), be_const_closure(Matter_Plugin_Light2_update_ct_minmax_closure) },
        { be_const_key_weak(read_attribute, 11), be_const_closure(Matter_Plugin_Light2_read_attribute_closure) },
        { be_const_key_weak(ct_min, -1), be_const_var(1) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light2) },
    })),
    be_str_weak(Matter_Plugin_Light2)
);
/*******************************************************************/

void be_load_Matter_Plugin_Light2_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Light2);
    be_setglobal(vm, "Matter_Plugin_Light2");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
