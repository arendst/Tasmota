/* Solidification of AXP2102.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'AXP2102' ktab size: 18, total: 58 (saved 320 bytes)
static const bvalue be_ktab_class_AXP2102[18] = {
  /* K0   */  be_nested_str(read8),
  /* K1   */  be_const_int(1),
  /* K2   */  be_nested_str(wire),
  /* K3   */  be_nested_str(read),
  /* K4   */  be_nested_str(addr),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str(init),
  /* K7   */  be_nested_str(AXP2102),
  /* K8   */  be_nested_str(write8),
  /* K9   */  be_nested_str(read16),
  /* K10  */  be_const_int(3),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str(_charge),
  /* K13  */  be_nested_str(_pre_charge),
  /* K14  */  be_nested_str(read14),
  /* K15  */  be_const_real_hex(0x447A0000),
  /* K16  */  be_nested_str(_charge_voltage),
  /* K17  */  be_nested_str(get_vbus_good),
};


extern const bclass be_class_AXP2102;

/********************************************************************
** Solidified function: is_charging
********************************************************************/
be_local_closure(class_AXP2102_is_charging,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_is_charging,
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x540A005F,  //  0003  LDINT	R2	96
      0x2C040202,  //  0004  AND	R1	R1	R2
      0x540A001F,  //  0005  LDINT	R2	32
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: battery_present
********************************************************************/
be_local_closure(class_AXP2102_battery_present,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_battery_present,
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60040017,  //  0000  GETGBL	R1	G23
      0x88080102,  //  0001  GETMBR	R2	R0	K2
      0x8C080503,  //  0002  GETMET	R2	R2	K3
      0x88100104,  //  0003  GETMBR	R4	R0	K4
      0x58140005,  //  0004  LDCONST	R5	K5
      0x58180001,  //  0005  LDCONST	R6	K1
      0x7C080800,  //  0006  CALL	R2	4
      0x540E0007,  //  0007  LDINT	R3	8
      0x2C080403,  //  0008  AND	R2	R2	R3
      0x7C040200,  //  0009  CALL	R1	1
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_AXP2102_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x54060033,  //  0003  LDINT	R1	52
      0x60080003,  //  0004  GETGBL	R2	G3
      0x5C0C0000,  //  0005  MOVE	R3	R0
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080506,  //  0007  GETMET	R2	R2	K6
      0x58100007,  //  0008  LDCONST	R4	K7
      0x5C140200,  //  0009  MOVE	R5	R1
      0x7C080600,  //  000A  CALL	R2	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_dldo_voltage
********************************************************************/
be_local_closure(class_AXP2102_set_dldo_voltage,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_set_dldo_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0x140C0305,  //  0000  LT	R3	R1	K5
      0x740E0001,  //  0001  JMPT	R3	#0004
      0x240C0301,  //  0002  GT	R3	R1	K1
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x540E0098,  //  0005  LDINT	R3	153
      0x000C0203,  //  0006  ADD	R3	R1	R3
      0x541201F3,  //  0007  LDINT	R4	500
      0x04080404,  //  0008  SUB	R2	R2	R4
      0x14100505,  //  0009  LT	R4	R2	K5
      0x78120001,  //  000A  JMPF	R4	#000D
      0x58100005,  //  000B  LDCONST	R4	K5
      0x70020004,  //  000C  JMP		#0012
      0x78060001,  //  000D  JMPF	R1	#0010
      0x54120031,  //  000E  LDINT	R4	50
      0x70020000,  //  000F  JMP		#0011
      0x54120063,  //  0010  LDINT	R4	100
      0x0C100404,  //  0011  DIV	R4	R2	R4
      0x78060001,  //  0012  JMPF	R1	#0015
      0x54160012,  //  0013  LDINT	R5	19
      0x70020000,  //  0014  JMP		#0016
      0x5416001B,  //  0015  LDINT	R5	28
      0x24140805,  //  0016  GT	R5	R4	R5
      0x78160004,  //  0017  JMPF	R5	#001D
      0x78060001,  //  0018  JMPF	R1	#001B
      0x54160012,  //  0019  LDINT	R5	19
      0x70020000,  //  001A  JMP		#001C
      0x5416001B,  //  001B  LDINT	R5	28
      0x5C100A00,  //  001C  MOVE	R4	R5
      0x8C140108,  //  001D  GETMET	R5	R0	K8
      0x5C1C0600,  //  001E  MOVE	R7	R3
      0x5C200800,  //  001F  MOVE	R8	R4
      0x7C140600,  //  0020  CALL	R5	3
      0x5416008F,  //  0021  LDINT	R5	144
      0x00140A01,  //  0022  ADD	R5	R5	R1
      0x78060001,  //  0023  JMPF	R1	#0026
      0x58180001,  //  0024  LDCONST	R6	K1
      0x70020000,  //  0025  JMP		#0027
      0x541A007F,  //  0026  LDINT	R6	128
      0x141C0505,  //  0027  LT	R7	R2	K5
      0x781E0008,  //  0028  JMPF	R7	#0032
      0x8C1C0108,  //  0029  GETMET	R7	R0	K8
      0x5426008F,  //  002A  LDINT	R9	144
      0x8C280100,  //  002B  GETMET	R10	R0	K0
      0x5C300A00,  //  002C  MOVE	R12	R5
      0x7C280400,  //  002D  CALL	R10	2
      0x482C0C00,  //  002E  FLIP	R11	R6
      0x2C28140B,  //  002F  AND	R10	R10	R11
      0x7C1C0600,  //  0030  CALL	R7	3
      0x70020006,  //  0031  JMP		#0039
      0x8C1C0108,  //  0032  GETMET	R7	R0	K8
      0x5426008F,  //  0033  LDINT	R9	144
      0x8C280100,  //  0034  GETMET	R10	R0	K0
      0x5C300A00,  //  0035  MOVE	R12	R5
      0x7C280400,  //  0036  CALL	R10	2
      0x30281406,  //  0037  OR	R10	R10	R6
      0x7C1C0600,  //  0038  CALL	R7	3
      0x80000000,  //  0039  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_temp
********************************************************************/
be_local_closure(class_AXP2102_get_temp,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_temp,
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x54060015,  //  0000  LDINT	R1	22
      0x540A1C69,  //  0001  LDINT	R2	7274
      0x8C0C0109,  //  0002  GETMET	R3	R0	K9
      0x5416003B,  //  0003  LDINT	R5	60
      0x7C0C0400,  //  0004  CALL	R3	2
      0x04080403,  //  0005  SUB	R2	R2	R3
      0x540E0013,  //  0006  LDINT	R3	20
      0x0C080403,  //  0007  DIV	R2	R2	R3
      0x00040202,  //  0008  ADD	R1	R1	R2
      0x80040200,  //  0009  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_ldo_enable
********************************************************************/
be_local_closure(class_AXP2102_get_ldo_enable,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_ldo_enable,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x50080000,  //  0000  LDBOOL	R2	0	0
      0x540E0004,  //  0001  LDINT	R3	5
      0x180C0203,  //  0002  LE	R3	R1	R3
      0x780E0007,  //  0003  JMPF	R3	#000C
      0x380E0201,  //  0004  SHL	R3	K1	R1
      0x60100017,  //  0005  GETGBL	R4	G23
      0x8C140100,  //  0006  GETMET	R5	R0	K0
      0x541E008F,  //  0007  LDINT	R7	144
      0x7C140400,  //  0008  CALL	R5	2
      0x2C140A03,  //  0009  AND	R5	R5	R3
      0x7C100200,  //  000A  CALL	R4	1
      0x5C080800,  //  000B  MOVE	R2	R4
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_battery_charging_status
********************************************************************/
be_local_closure(class_AXP2102_get_battery_charging_status,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_battery_charging_status,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x540A0004,  //  0003  LDINT	R2	5
      0x3C040202,  //  0004  SHR	R1	R1	R2
      0x2C04030A,  //  0005  AND	R1	R1	K10
      0x1C080301,  //  0006  EQ	R2	R1	K1
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x58080001,  //  0008  LDCONST	R2	K1
      0x70020004,  //  0009  JMP		#000F
      0x1C08030B,  //  000A  EQ	R2	R1	K11
      0x780A0001,  //  000B  JMPF	R2	#000E
      0x5409FFFE,  //  000C  LDINT	R2	-1
      0x70020000,  //  000D  JMP		#000F
      0x58080005,  //  000E  LDCONST	R2	K5
      0x80040400,  //  000F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ldo_voltage
********************************************************************/
be_local_closure(class_AXP2102_set_ldo_voltage,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_set_ldo_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x140C0305,  //  0000  LT	R3	R1	K5
      0x740E0002,  //  0001  JMPT	R3	#0005
      0x540E0004,  //  0002  LDINT	R3	5
      0x240C0203,  //  0003  GT	R3	R1	R3
      0x780E0000,  //  0004  JMPF	R3	#0006
      0x80000600,  //  0005  RET	0
      0x540E0091,  //  0006  LDINT	R3	146
      0x000C0203,  //  0007  ADD	R3	R1	R3
      0x541201F3,  //  0008  LDINT	R4	500
      0x04080404,  //  0009  SUB	R2	R2	R4
      0x14100505,  //  000A  LT	R4	R2	K5
      0x78120001,  //  000B  JMPF	R4	#000E
      0x58100005,  //  000C  LDCONST	R4	K5
      0x70020001,  //  000D  JMP		#0010
      0x54120063,  //  000E  LDINT	R4	100
      0x0C100404,  //  000F  DIV	R4	R2	R4
      0x5416001D,  //  0010  LDINT	R5	30
      0x24140805,  //  0011  GT	R5	R4	R5
      0x78160000,  //  0012  JMPF	R5	#0014
      0x5412001D,  //  0013  LDINT	R4	30
      0x8C140108,  //  0014  GETMET	R5	R0	K8
      0x5C1C0600,  //  0015  MOVE	R7	R3
      0x5C200800,  //  0016  MOVE	R8	R4
      0x7C140600,  //  0017  CALL	R5	3
      0x38160201,  //  0018  SHL	R5	K1	R1
      0x14180505,  //  0019  LT	R6	R2	K5
      0x781A0008,  //  001A  JMPF	R6	#0024
      0x8C180108,  //  001B  GETMET	R6	R0	K8
      0x5422008F,  //  001C  LDINT	R8	144
      0x8C240100,  //  001D  GETMET	R9	R0	K0
      0x542E008F,  //  001E  LDINT	R11	144
      0x7C240400,  //  001F  CALL	R9	2
      0x48280A00,  //  0020  FLIP	R10	R5
      0x2C24120A,  //  0021  AND	R9	R9	R10
      0x7C180600,  //  0022  CALL	R6	3
      0x70020006,  //  0023  JMP		#002B
      0x8C180108,  //  0024  GETMET	R6	R0	K8
      0x5422008F,  //  0025  LDINT	R8	144
      0x8C240100,  //  0026  GETMET	R9	R0	K0
      0x542E008F,  //  0027  LDINT	R11	144
      0x7C240400,  //  0028  CALL	R9	2
      0x30241205,  //  0029  OR	R9	R9	R5
      0x7C180600,  //  002A  CALL	R6	3
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_current
********************************************************************/
be_local_closure(class_AXP2102_get_bat_current,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_bat_current,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80060A00,  //  0000  RET	1	K5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_charge_current
********************************************************************/
be_local_closure(class_AXP2102_set_charge_current,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_set_charge_current,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x14080305,  //  0000  LT	R2	R1	K5
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040005,  //  0002  LDCONST	R1	K5
      0x540A00C7,  //  0003  LDINT	R2	200
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540600C7,  //  0006  LDINT	R1	200
      0x58080005,  //  0007  LDCONST	R2	K5
      0x880C010C,  //  0008  GETMBR	R3	R0	K12
      0x940C0602,  //  0009  GETIDX	R3	R3	R2
      0x180C0601,  //  000A  LE	R3	R3	R1
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x00080501,  //  000C  ADD	R2	R2	K1
      0x7001FFF9,  //  000D  JMP		#0008
      0x540E0003,  //  000E  LDINT	R3	4
      0x00080403,  //  000F  ADD	R2	R2	R3
      0x8C0C0108,  //  0010  GETMET	R3	R0	K8
      0x54160061,  //  0011  LDINT	R5	98
      0x5C180400,  //  0012  MOVE	R6	R2
      0x7C0C0600,  //  0013  CALL	R3	3
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_aps_voltage
********************************************************************/
be_local_closure(class_AXP2102_get_aps_voltage,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_aps_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80060A00,  //  0000  RET	1	K5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_pre_charge_current
********************************************************************/
be_local_closure(class_AXP2102_set_pre_charge_current,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_set_pre_charge_current,
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x14080305,  //  0000  LT	R2	R1	K5
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040005,  //  0002  LDCONST	R1	K5
      0x540A00C7,  //  0003  LDINT	R2	200
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540600C7,  //  0006  LDINT	R1	200
      0x58080005,  //  0007  LDCONST	R2	K5
      0x880C010D,  //  0008  GETMBR	R3	R0	K13
      0x940C0602,  //  0009  GETIDX	R3	R3	R2
      0x180C0601,  //  000A  LE	R3	R3	R1
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x00080501,  //  000C  ADD	R2	R2	K1
      0x7001FFF9,  //  000D  JMP		#0008
      0x8C0C0108,  //  000E  GETMET	R3	R0	K8
      0x54160060,  //  000F  LDINT	R5	97
      0x5C180400,  //  0010  MOVE	R6	R2
      0x7C0C0600,  //  0011  CALL	R3	3
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_power
********************************************************************/
be_local_closure(class_AXP2102_get_bat_power,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_bat_power,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80060A00,  //  0000  RET	1	K5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_battery_charge
********************************************************************/
be_local_closure(class_AXP2102_set_battery_charge,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_set_battery_charge,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C080108,  //  0000  GETMET	R2	R0	K8
      0x54120017,  //  0001  LDINT	R4	24
      0x8C140100,  //  0002  GETMET	R5	R0	K0
      0x541E0017,  //  0003  LDINT	R7	24
      0x7C140400,  //  0004  CALL	R5	2
      0x541A00FC,  //  0005  LDINT	R6	253
      0x2C140A06,  //  0006  AND	R5	R5	R6
      0x78060001,  //  0007  JMPF	R1	#000A
      0x58180001,  //  0008  LDCONST	R6	K1
      0x70020000,  //  0009  JMP		#000B
      0x58180005,  //  000A  LDCONST	R6	K5
      0x38180D01,  //  000B  SHL	R6	R6	K1
      0x30140A06,  //  000C  OR	R5	R5	R6
      0x7C080600,  //  000D  CALL	R2	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_charge_current
********************************************************************/
be_local_closure(class_AXP2102_get_bat_charge_current,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_bat_charge_current,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80060A00,  //  0000  RET	1	K5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_vbus_good
********************************************************************/
be_local_closure(class_AXP2102_get_vbus_good,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_vbus_good,
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60040017,  //  0000  GETGBL	R1	G23
      0x88080102,  //  0001  GETMBR	R2	R0	K2
      0x8C080503,  //  0002  GETMET	R2	R2	K3
      0x88100104,  //  0003  GETMBR	R4	R0	K4
      0x58140005,  //  0004  LDCONST	R5	K5
      0x58180001,  //  0005  LDCONST	R6	K1
      0x7C080800,  //  0006  CALL	R2	4
      0x540E001F,  //  0007  LDINT	R3	32
      0x2C080403,  //  0008  AND	R2	R2	R3
      0x7C040200,  //  0009  CALL	R1	1
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_voltage
********************************************************************/
be_local_closure(class_AXP2102_get_bat_voltage,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_bat_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C04010E,  //  0000  GETMET	R1	R0	K14
      0x540E0033,  //  0001  LDINT	R3	52
      0x7C040400,  //  0002  CALL	R1	2
      0x0C04030F,  //  0003  DIV	R1	R1	K15
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_battery_level
********************************************************************/
be_local_closure(class_AXP2102_get_battery_level,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_battery_level,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E00A3,  //  0001  LDINT	R3	164
      0x7C040400,  //  0002  CALL	R1	2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_charge_voltage
********************************************************************/
be_local_closure(class_AXP2102_set_charge_voltage,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_set_charge_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x14080305,  //  0000  LT	R2	R1	K5
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x58040005,  //  0002  LDCONST	R1	K5
      0x540A01CB,  //  0003  LDINT	R2	460
      0x24080202,  //  0004  GT	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540601CB,  //  0006  LDINT	R1	460
      0x58080005,  //  0007  LDCONST	R2	K5
      0x880C0110,  //  0008  GETMBR	R3	R0	K16
      0x940C0602,  //  0009  GETIDX	R3	R3	R2
      0x180C0601,  //  000A  LE	R3	R3	R1
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x00080501,  //  000C  ADD	R2	R2	K1
      0x7001FFF9,  //  000D  JMP		#0008
      0x00080501,  //  000E  ADD	R2	R2	K1
      0x540E0005,  //  000F  LDINT	R3	6
      0x280C0403,  //  0010  GE	R3	R2	R3
      0x780E0000,  //  0011  JMPF	R3	#0013
      0x58080005,  //  0012  LDCONST	R2	K5
      0x8C0C0108,  //  0013  GETMET	R3	R0	K8
      0x54160063,  //  0014  LDINT	R5	100
      0x5C180400,  //  0015  MOVE	R6	R2
      0x7C0C0600,  //  0016  CALL	R3	3
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_vbus_current
********************************************************************/
be_local_closure(class_AXP2102_get_vbus_current,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_vbus_current,
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80060A00,  //  0000  RET	1	K5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_vbus_voltage
********************************************************************/
be_local_closure(class_AXP2102_get_vbus_voltage,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP2102,     /* shared constants */
    &be_const_str_get_vbus_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8C040111,  //  0000  GETMET	R1	R0	K17
      0x7C040200,  //  0001  CALL	R1	1
      0x74060000,  //  0002  JMPT	R1	#0004
      0x80060A00,  //  0003  RET	1	K5
      0x8C04010E,  //  0004  GETMET	R1	R0	K14
      0x540E0037,  //  0005  LDINT	R3	56
      0x7C040400,  //  0006  CALL	R1	2
      0x540A3FF6,  //  0007  LDINT	R2	16375
      0x24080202,  //  0008  GT	R2	R1	R2
      0x780A0000,  //  0009  JMPF	R2	#000B
      0x80060A00,  //  000A  RET	1	K5
      0x0C08030F,  //  000B  DIV	R2	R1	K15
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: AXP2102
********************************************************************/
extern const bclass be_class_I2C_Driver;
be_local_class(AXP2102,
    0,
    &be_class_I2C_Driver,
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(get_vbus_voltage, -1), be_const_closure(class_AXP2102_get_vbus_voltage_closure) },
        { be_const_key(is_charging, -1), be_const_closure(class_AXP2102_is_charging_closure) },
        { be_const_key(battery_present, -1), be_const_closure(class_AXP2102_battery_present_closure) },
        { be_const_key(init, -1), be_const_closure(class_AXP2102_init_closure) },
        { be_const_key(set_dldo_voltage, -1), be_const_closure(class_AXP2102_set_dldo_voltage_closure) },
        { be_const_key(_charge, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(13,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(25),
        be_const_int(30),
        be_const_int(35),
        be_const_int(40),
        be_const_int(60),
        be_const_int(80),
        be_const_int(100),
        be_const_int(120),
        be_const_int(140),
        be_const_int(160),
        be_const_int(180),
        be_const_int(200),
        be_const_int(255),
    }))    ) } )) },
        { be_const_key(set_charge_voltage, -1), be_const_closure(class_AXP2102_set_charge_voltage_closure) },
        { be_const_key(get_battery_level, 23), be_const_closure(class_AXP2102_get_battery_level_closure) },
        { be_const_key(set_ldo_voltage, -1), be_const_closure(class_AXP2102_set_ldo_voltage_closure) },
        { be_const_key(get_bat_current, 0), be_const_closure(class_AXP2102_get_bat_current_closure) },
        { be_const_key(set_charge_current, -1), be_const_closure(class_AXP2102_set_charge_current_closure) },
        { be_const_key(_charge_voltage, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(410),
        be_const_int(420),
        be_const_int(435),
        be_const_int(440),
        be_const_int(460),
        be_const_int(655),
    }))    ) } )) },
        { be_const_key(set_pre_charge_current, 16), be_const_closure(class_AXP2102_set_pre_charge_current_closure) },
        { be_const_key(get_bat_power, -1), be_const_closure(class_AXP2102_get_bat_power_closure) },
        { be_const_key(get_temp, 5), be_const_closure(class_AXP2102_get_temp_closure) },
        { be_const_key(get_bat_voltage, 17), be_const_closure(class_AXP2102_get_bat_voltage_closure) },
        { be_const_key(get_vbus_good, -1), be_const_closure(class_AXP2102_get_vbus_good_closure) },
        { be_const_key(get_bat_charge_current, -1), be_const_closure(class_AXP2102_get_bat_charge_current_closure) },
        { be_const_key(set_battery_charge, 15), be_const_closure(class_AXP2102_set_battery_charge_closure) },
        { be_const_key(get_aps_voltage, 11), be_const_closure(class_AXP2102_get_aps_voltage_closure) },
        { be_const_key(_pre_charge, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(25),
        be_const_int(50),
        be_const_int(75),
        be_const_int(100),
        be_const_int(125),
        be_const_int(150),
        be_const_int(175),
        be_const_int(200),
        be_const_int(255),
    }))    ) } )) },
        { be_const_key(get_ldo_enable, 6), be_const_closure(class_AXP2102_get_ldo_enable_closure) },
        { be_const_key(get_vbus_current, -1), be_const_closure(class_AXP2102_get_vbus_current_closure) },
        { be_const_key(get_battery_charging_status, -1), be_const_closure(class_AXP2102_get_battery_charging_status_closure) },
    })),
    (bstring*) &be_const_str_AXP2102
);
/********************************************************************/
/* End of solidification */
