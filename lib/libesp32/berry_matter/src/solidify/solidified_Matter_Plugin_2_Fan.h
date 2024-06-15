/* Solidification of Matter_Plugin_2_Fan.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Fan;

/********************************************************************
** Solidified function: set_fan_mode
********************************************************************/
extern const bclass be_class_Matter_Plugin_Fan;
be_local_closure(class_Matter_Plugin_Fan_set_fan_mode,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Fan, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(shadow_fan_mode),
    /* K2   */  be_nested_str_weak(attribute_updated),
    /* K3   */  be_nested_str_weak(shadow_fan_speed_pct),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(scale_uint),
    /* K7   */  be_const_int(2),
    }),
    be_str_weak(set_fan_mode),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x14080300,  //  0004  LT	R2	R1	K0
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x58040000,  //  0006  LDCONST	R1	K0
      0x88080101,  //  0007  GETMBR	R2	R0	K1
      0x20080202,  //  0008  NE	R2	R1	R2
      0x780A0024,  //  0009  JMPF	R2	#002F
      0x8C080102,  //  000A  GETMET	R2	R0	K2
      0x54120201,  //  000B  LDINT	R4	514
      0x58140000,  //  000C  LDCONST	R5	K0
      0x7C080600,  //  000D  CALL	R2	3
      0x60080009,  //  000E  GETGBL	R2	G9
      0x5C0C0200,  //  000F  MOVE	R3	R1
      0x7C080200,  //  0010  CALL	R2	1
      0x90020202,  //  0011  SETMBR	R0	K1	R2
      0x88080103,  //  0012  GETMBR	R2	R0	K3
      0x880C0101,  //  0013  GETMBR	R3	R0	K1
      0x1C0C0700,  //  0014  EQ	R3	R3	K0
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x58080000,  //  0016  LDCONST	R2	K0
      0x7002000E,  //  0017  JMP		#0027
      0x880C0101,  //  0018  GETMBR	R3	R0	K1
      0x240C0704,  //  0019  GT	R3	R3	K4
      0x780E0002,  //  001A  JMPF	R3	#001E
      0x90020304,  //  001B  SETMBR	R0	K1	K4
      0x540A0063,  //  001C  LDINT	R2	100
      0x70020008,  //  001D  JMP		#0027
      0xB80E0A00,  //  001E  GETNGBL	R3	K5
      0x8C0C0706,  //  001F  GETMET	R3	R3	K6
      0x5C140200,  //  0020  MOVE	R5	R1
      0x58180000,  //  0021  LDCONST	R6	K0
      0x581C0004,  //  0022  LDCONST	R7	K4
      0x58200000,  //  0023  LDCONST	R8	K0
      0x54260063,  //  0024  LDINT	R9	100
      0x7C0C0C00,  //  0025  CALL	R3	6
      0x5C080600,  //  0026  MOVE	R2	R3
      0x880C0103,  //  0027  GETMBR	R3	R0	K3
      0x200C0602,  //  0028  NE	R3	R3	R2
      0x780E0004,  //  0029  JMPF	R3	#002F
      0x90020602,  //  002A  SETMBR	R0	K3	R2
      0x8C0C0102,  //  002B  GETMET	R3	R0	K2
      0x54160201,  //  002C  LDINT	R5	514
      0x58180007,  //  002D  LDCONST	R6	K7
      0x7C0C0600,  //  002E  CALL	R3	3
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
extern const bclass be_class_Matter_Plugin_Fan;
be_local_closure(class_Matter_Plugin_Fan_update_virtual,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Fan, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(FanMode),
    /* K2   */  be_nested_str_weak(set_fan_mode),
    /* K3   */  be_nested_str_weak(FanSpeed),
    /* K4   */  be_nested_str_weak(set_fan_speed_pct),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0004,  //  0005  JMPF	R3	#000B
      0x8C0C0102,  //  0006  GETMET	R3	R0	K2
      0x60140009,  //  0007  GETGBL	R5	G9
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x8C0C0300,  //  000B  GETMET	R3	R1	K0
      0x58140003,  //  000C  LDCONST	R5	K3
      0x7C0C0400,  //  000D  CALL	R3	2
      0x4C100000,  //  000E  LDNIL	R4
      0x20100604,  //  000F  NE	R4	R3	R4
      0x78120004,  //  0010  JMPF	R4	#0016
      0x8C100104,  //  0011  GETMET	R4	R0	K4
      0x60180009,  //  0012  GETGBL	R6	G9
      0x5C1C0600,  //  0013  MOVE	R7	R3
      0x7C180200,  //  0014  CALL	R6	1
      0x7C100400,  //  0015  CALL	R4	2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_fan_speed_pct
********************************************************************/
extern const bclass be_class_Matter_Plugin_Fan;
be_local_closure(class_Matter_Plugin_Fan_set_fan_speed_pct,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Fan, 
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(shadow_fan_speed_pct),
    /* K2   */  be_nested_str_weak(attribute_updated),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_weak(shadow_fan_mode),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(scale_uint),
    /* K7   */  be_const_int(1),
    /* K8   */  be_const_int(3),
    }),
    be_str_weak(set_fan_speed_pct),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x5C0C0200,  //  0001  MOVE	R3	R1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C040400,  //  0003  MOVE	R1	R2
      0x14080300,  //  0004  LT	R2	R1	K0
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x58040000,  //  0006  LDCONST	R1	K0
      0x540A0063,  //  0007  LDINT	R2	100
      0x24080202,  //  0008  GT	R2	R1	R2
      0x780A0000,  //  0009  JMPF	R2	#000B
      0x54060063,  //  000A  LDINT	R1	100
      0x88080101,  //  000B  GETMBR	R2	R0	K1
      0x20080202,  //  000C  NE	R2	R1	R2
      0x780A001A,  //  000D  JMPF	R2	#0029
      0x8C080102,  //  000E  GETMET	R2	R0	K2
      0x54120201,  //  000F  LDINT	R4	514
      0x58140003,  //  0010  LDCONST	R5	K3
      0x7C080600,  //  0011  CALL	R2	3
      0x90020201,  //  0012  SETMBR	R0	K1	R1
      0x88080104,  //  0013  GETMBR	R2	R0	K4
      0x1C0C0300,  //  0014  EQ	R3	R1	K0
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x58080000,  //  0016  LDCONST	R2	K0
      0x70020008,  //  0017  JMP		#0021
      0xB80E0A00,  //  0018  GETNGBL	R3	K5
      0x8C0C0706,  //  0019  GETMET	R3	R3	K6
      0x5C140200,  //  001A  MOVE	R5	R1
      0x58180007,  //  001B  LDCONST	R6	K7
      0x541E0063,  //  001C  LDINT	R7	100
      0x58200007,  //  001D  LDCONST	R8	K7
      0x58240008,  //  001E  LDCONST	R9	K8
      0x7C0C0C00,  //  001F  CALL	R3	6
      0x5C080600,  //  0020  MOVE	R2	R3
      0x880C0104,  //  0021  GETMBR	R3	R0	K4
      0x200C0403,  //  0022  NE	R3	R2	R3
      0x780E0004,  //  0023  JMPF	R3	#0029
      0x90020802,  //  0024  SETMBR	R0	K4	R2
      0x8C0C0102,  //  0025  GETMET	R3	R0	K2
      0x54160201,  //  0026  LDINT	R5	514
      0x58180000,  //  0027  LDCONST	R6	K0
      0x7C0C0600,  //  0028  CALL	R3	3
      0x80000000,  //  0029  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_attribute
********************************************************************/
extern const bclass be_class_Matter_Plugin_Fan;
be_local_closure(class_Matter_Plugin_Fan_write_attribute,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Fan, 
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(int),
    /* K7   */  be_nested_str_weak(set_fan_mode),
    /* K8   */  be_nested_str_weak(publish_command),
    /* K9   */  be_nested_str_weak(FanMode),
    /* K10  */  be_nested_str_weak(shadow_fan_mode),
    /* K11  */  be_nested_str_weak(FanSpeed),
    /* K12  */  be_nested_str_weak(shadow_fan_speed_pct),
    /* K13  */  be_nested_str_weak(status),
    /* K14  */  be_nested_str_weak(CONSTRAINT_ERROR),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(set_fan_speed_pct),
    }),
    be_str_weak(write_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[60]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0201,  //  0004  LDINT	R7	514
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0032,  //  0006  JMPF	R7	#003A
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0016,  //  000A  JMPF	R7	#0022
      0x601C0004,  //  000B  GETGBL	R7	G4
      0x5C200600,  //  000C  MOVE	R8	R3
      0x7C1C0200,  //  000D  CALL	R7	1
      0x1C1C0F06,  //  000E  EQ	R7	R7	K6
      0x781E000B,  //  000F  JMPF	R7	#001C
      0x8C1C0107,  //  0010  GETMET	R7	R0	K7
      0x5C240600,  //  0011  MOVE	R9	R3
      0x7C1C0400,  //  0012  CALL	R7	2
      0x8C1C0108,  //  0013  GETMET	R7	R0	K8
      0x58240009,  //  0014  LDCONST	R9	K9
      0x8828010A,  //  0015  GETMBR	R10	R0	K10
      0x582C000B,  //  0016  LDCONST	R11	K11
      0x8830010C,  //  0017  GETMBR	R12	R0	K12
      0x7C1C0A00,  //  0018  CALL	R7	5
      0x501C0200,  //  0019  LDBOOL	R7	1	0
      0x80040E00,  //  001A  RET	1	R7
      0x70020004,  //  001B  JMP		#0021
      0xB81E0000,  //  001C  GETNGBL	R7	K0
      0x881C0F0E,  //  001D  GETMBR	R7	R7	K14
      0x900A1A07,  //  001E  SETMBR	R2	K13	R7
      0x501C0000,  //  001F  LDBOOL	R7	0	0
      0x80040E00,  //  0020  RET	1	R7
      0x70020017,  //  0021  JMP		#003A
      0x1C1C0D0F,  //  0022  EQ	R7	R6	K15
      0x781E0015,  //  0023  JMPF	R7	#003A
      0x601C0004,  //  0024  GETGBL	R7	G4
      0x5C200600,  //  0025  MOVE	R8	R3
      0x7C1C0200,  //  0026  CALL	R7	1
      0x1C1C0F06,  //  0027  EQ	R7	R7	K6
      0x781E000B,  //  0028  JMPF	R7	#0035
      0x8C1C0110,  //  0029  GETMET	R7	R0	K16
      0x5C240600,  //  002A  MOVE	R9	R3
      0x7C1C0400,  //  002B  CALL	R7	2
      0x8C1C0108,  //  002C  GETMET	R7	R0	K8
      0x58240009,  //  002D  LDCONST	R9	K9
      0x8828010A,  //  002E  GETMBR	R10	R0	K10
      0x582C000B,  //  002F  LDCONST	R11	K11
      0x8830010C,  //  0030  GETMBR	R12	R0	K12
      0x7C1C0A00,  //  0031  CALL	R7	5
      0x501C0200,  //  0032  LDBOOL	R7	1	0
      0x80040E00,  //  0033  RET	1	R7
      0x70020004,  //  0034  JMP		#003A
      0xB81E0000,  //  0035  GETNGBL	R7	K0
      0x881C0F0E,  //  0036  GETMBR	R7	R7	K14
      0x900A1A07,  //  0037  SETMBR	R2	K13	R7
      0x501C0000,  //  0038  LDBOOL	R7	0	0
      0x80040E00,  //  0039  RET	1	R7
      0x4C1C0000,  //  003A  LDNIL	R7
      0x80040E00,  //  003B  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_Plugin_Fan;
be_local_closure(class_Matter_Plugin_Fan_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Fan, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_fan_mode),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(shadow_fan_speed_pct),
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
** Solidified function: read_attribute
********************************************************************/
extern const bclass be_class_Matter_Plugin_Fan;
be_local_closure(class_Matter_Plugin_Fan_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Fan, 
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(update_shadow_lazy),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(shadow_fan_mode),
    /* K9   */  be_const_int(1),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(shadow_fan_speed_pct),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0201,  //  0004  LDINT	R7	514
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0020,  //  0006  JMPF	R7	#0028
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0005,  //  000A  JMPF	R7	#0011
      0x8C1C0706,  //  000B  GETMET	R7	R3	K6
      0x88240907,  //  000C  GETMBR	R9	R4	K7
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x70020016,  //  0010  JMP		#0028
      0x1C1C0D09,  //  0011  EQ	R7	R6	K9
      0x781E0005,  //  0012  JMPF	R7	#0019
      0x8C1C0706,  //  0013  GETMET	R7	R3	K6
      0x88240907,  //  0014  GETMBR	R9	R4	K7
      0x5828000A,  //  0015  LDCONST	R10	K10
      0x7C1C0600,  //  0016  CALL	R7	3
      0x80040E00,  //  0017  RET	1	R7
      0x7002000E,  //  0018  JMP		#0028
      0x1C1C0D0A,  //  0019  EQ	R7	R6	K10
      0x781E0005,  //  001A  JMPF	R7	#0021
      0x8C1C0706,  //  001B  GETMET	R7	R3	K6
      0x88240907,  //  001C  GETMBR	R9	R4	K7
      0x8828010B,  //  001D  GETMBR	R10	R0	K11
      0x7C1C0600,  //  001E  CALL	R7	3
      0x80040E00,  //  001F  RET	1	R7
      0x70020006,  //  0020  JMP		#0028
      0x1C1C0D0C,  //  0021  EQ	R7	R6	K12
      0x781E0004,  //  0022  JMPF	R7	#0028
      0x8C1C0706,  //  0023  GETMET	R7	R3	K6
      0x88240907,  //  0024  GETMBR	R9	R4	K7
      0x8828010B,  //  0025  GETMBR	R10	R0	K11
      0x7C1C0600,  //  0026  CALL	R7	3
      0x80040E00,  //  0027  RET	1	R7
      0x601C0003,  //  0028  GETGBL	R7	G3
      0x5C200000,  //  0029  MOVE	R8	R0
      0x7C1C0200,  //  002A  CALL	R7	1
      0x8C1C0F0D,  //  002B  GETMET	R7	R7	K13
      0x5C240200,  //  002C  MOVE	R9	R1
      0x5C280400,  //  002D  MOVE	R10	R2
      0x5C2C0600,  //  002E  MOVE	R11	R3
      0x7C1C0800,  //  002F  CALL	R7	4
      0x80040E00,  //  0030  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Fan
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Fan,
    2,
    &be_class_Matter_Plugin_Device,
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Fan_update_virtual_closure) },
        { be_const_key_weak(read_attribute, 3), be_const_closure(class_Matter_Plugin_Fan_read_attribute_closure) },
        { be_const_key_weak(TYPE, 0), be_nested_str_weak(fan) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(FanMode),
        be_nested_str_weak(FanSpeed),
    }))    ) } )) },
        { be_const_key_weak(shadow_fan_speed_pct, -1), be_const_var(1) },
        { be_const_key_weak(set_fan_speed_pct, -1), be_const_closure(class_Matter_Plugin_Fan_set_fan_speed_pct_closure) },
        { be_const_key_weak(shadow_fan_mode, 8), be_const_var(0) },
        { be_const_key_weak(write_attribute, -1), be_const_closure(class_Matter_Plugin_Fan_write_attribute_closure) },
        { be_const_key_weak(init, 9), be_const_closure(class_Matter_Plugin_Fan_init_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Fan) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(3, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(514, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(29, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
    }))    ) } )) },
        { be_const_key_weak(set_fan_mode, 1), be_const_closure(class_Matter_Plugin_Fan_set_fan_mode_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(43, -1), be_const_int(2) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Fan)
);
/********************************************************************/
/* End of solidification */
