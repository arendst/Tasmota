/* Solidification of AXP202.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'AXP202' ktab size: 24, total: 64 (saved 320 bytes)
static const bvalue be_ktab_class_AXP202[24] = {
  /* K0   */  be_const_int(0),
  /* K1   */  be_const_int(1),
  /* K2   */  be_const_int(2),
  /* K3   */  be_const_int(3),
  /* K4   */  be_nested_str(write8),
  /* K5   */  be_nested_str(read8),
  /* K6   */  be_nested_str(read12),
  /* K7   */  be_const_real_hex(0x3EC00000),
  /* K8   */  be_nested_str(read13),
  /* K9   */  be_const_real_hex(0x3F000000),
  /* K10  */  be_nested_str(read24),
  /* K11  */  be_const_real_hex(0x3A102DE0),
  /* K12  */  be_nested_str(wire),
  /* K13  */  be_nested_str(read),
  /* K14  */  be_nested_str(addr),
  /* K15  */  be_const_real_hex(0x3ADED28A),
  /* K16  */  be_const_real_hex(0x3AB78035),
  /* K17  */  be_nested_str(write_bit),
  /* K18  */  be_nested_str(I2C_Driver),
  /* K19  */  be_nested_str(init),
  /* K20  */  be_nested_str(AXP202),
  /* K21  */  be_const_real_hex(0x3A902DE0),
  /* K22  */  be_const_real_hex(0x3DCCCCCD),
  /* K23  */  be_const_real_hex(0x4310B333),
};


extern const bclass be_class_AXP202;

/********************************************************************
** Solidified function: set_shutdown_time
********************************************************************/
be_local_closure(class_AXP202_set_shutdown_time,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_set_shutdown_time,
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x540E0005,  //  0001  LDINT	R3	6
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x58080001,  //  0004  LDCONST	R2	K1
      0x540E0007,  //  0005  LDINT	R3	8
      0x1C0C0203,  //  0006  EQ	R3	R1	R3
      0x780E0000,  //  0007  JMPF	R3	#0009
      0x58080002,  //  0008  LDCONST	R2	K2
      0x540E0009,  //  0009  LDINT	R3	10
      0x1C0C0203,  //  000A  EQ	R3	R1	R3
      0x780E0000,  //  000B  JMPF	R3	#000D
      0x58080003,  //  000C  LDCONST	R2	K3
      0x8C0C0104,  //  000D  GETMET	R3	R0	K4
      0x54160035,  //  000E  LDINT	R5	54
      0x8C180105,  //  000F  GETMET	R6	R0	K5
      0x54220035,  //  0010  LDINT	R8	54
      0x7C180400,  //  0011  CALL	R6	2
      0x541E00FB,  //  0012  LDINT	R7	252
      0x2C180C07,  //  0013  AND	R6	R6	R7
      0x2C1C0503,  //  0014  AND	R7	R2	K3
      0x30180C07,  //  0015  OR	R6	R6	R7
      0x7C0C0600,  //  0016  CALL	R3	3
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_vbus_current
********************************************************************/
be_local_closure(class_AXP202_get_vbus_current,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_vbus_current,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x540E005B,  //  0001  LDINT	R3	92
      0x7C040400,  //  0002  CALL	R1	2
      0x08040307,  //  0003  MUL	R1	R1	K7
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_charge_current
********************************************************************/
be_local_closure(class_AXP202_get_bat_charge_current,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_bat_charge_current,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040108,  //  0000  GETMET	R1	R0	K8
      0x540E0079,  //  0001  LDINT	R3	122
      0x7C040400,  //  0002  CALL	R1	2
      0x08040309,  //  0003  MUL	R1	R1	K9
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_current
********************************************************************/
be_local_closure(class_AXP202_get_bat_current,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_bat_current,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C040108,  //  0000  GETMET	R1	R0	K8
      0x540E0079,  //  0001  LDINT	R3	122
      0x7C040400,  //  0002  CALL	R1	2
      0x8C080108,  //  0003  GETMET	R2	R0	K8
      0x5412007B,  //  0004  LDINT	R4	124
      0x7C080400,  //  0005  CALL	R2	2
      0x04040202,  //  0006  SUB	R1	R1	R2
      0x08040309,  //  0007  MUL	R1	R1	K9
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_power
********************************************************************/
be_local_closure(class_AXP202_get_bat_power,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_bat_power,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C04010A,  //  0000  GETMET	R1	R0	K10
      0x540E006F,  //  0001  LDINT	R3	112
      0x7C040400,  //  0002  CALL	R1	2
      0x0804030B,  //  0003  MUL	R1	R1	K11
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_battery_charging_status
********************************************************************/
be_local_closure(class_AXP202_get_battery_charging_status,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_battery_charging_status,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x8C04030D,  //  0001  GETMET	R1	R1	K13
      0x880C010E,  //  0002  GETMBR	R3	R0	K14
      0x58100001,  //  0003  LDCONST	R4	K1
      0x58140001,  //  0004  LDCONST	R5	K1
      0x7C040800,  //  0005  CALL	R1	4
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_vbus_voltage
********************************************************************/
be_local_closure(class_AXP202_get_vbus_voltage,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_vbus_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x540E0059,  //  0001  LDINT	R3	90
      0x7C040400,  //  0002  CALL	R1	2
      0x0804030F,  //  0003  MUL	R1	R1	K15
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_aps_voltage
********************************************************************/
be_local_closure(class_AXP202_get_aps_voltage,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_aps_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x540E007D,  //  0001  LDINT	R3	126
      0x7C040400,  //  0002  CALL	R1	2
      0x08040310,  //  0003  MUL	R1	R1	K16
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: battery_present
********************************************************************/
be_local_closure(class_AXP202_battery_present,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_battery_present,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x8C04030D,  //  0001  GETMET	R1	R1	K13
      0x880C010E,  //  0002  GETMBR	R3	R0	K14
      0x58100001,  //  0003  LDCONST	R4	K1
      0x58140001,  //  0004  LDCONST	R5	K1
      0x7C040800,  //  0005  CALL	R1	4
      0x540A001F,  //  0006  LDINT	R2	32
      0x2C040202,  //  0007  AND	R1	R1	R2
      0x78060002,  //  0008  JMPF	R1	#000C
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
      0x70020001,  //  000B  JMP		#000E
      0x50040000,  //  000C  LDBOOL	R1	0	0
      0x80040200,  //  000D  RET	1	R1
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_chg_current_ma
********************************************************************/
be_local_closure(class_AXP202_set_chg_current_ma,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_set_chg_current_ma,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x540A012B,  //  0000  LDINT	R2	300
      0x04080202,  //  0001  SUB	R2	R1	R2
      0x540E0063,  //  0002  LDINT	R3	100
      0x0C080403,  //  0003  DIV	R2	R2	R3
      0x8C0C0104,  //  0004  GETMET	R3	R0	K4
      0x54160032,  //  0005  LDINT	R5	51
      0x8C180105,  //  0006  GETMET	R6	R0	K5
      0x54220032,  //  0007  LDINT	R8	51
      0x7C180400,  //  0008  CALL	R6	2
      0x541E00EF,  //  0009  LDINT	R7	240
      0x2C180C07,  //  000A  AND	R6	R6	R7
      0x541E000E,  //  000B  LDINT	R7	15
      0x2C1C0407,  //  000C  AND	R7	R2	R7
      0x30180C07,  //  000D  OR	R6	R6	R7
      0x7C0C0600,  //  000E  CALL	R3	3
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_dcdc_enable
********************************************************************/
be_local_closure(class_AXP202_set_dcdc_enable,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_set_dcdc_enable,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x1C0C0302,  //  0000  EQ	R3	R1	K2
      0x780E0004,  //  0001  JMPF	R3	#0007
      0x8C0C0111,  //  0002  GETMET	R3	R0	K17
      0x54160011,  //  0003  LDINT	R5	18
      0x541A0003,  //  0004  LDINT	R6	4
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x7C0C0800,  //  0006  CALL	R3	4
      0x1C0C0303,  //  0007  EQ	R3	R1	K3
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x8C0C0111,  //  0009  GETMET	R3	R0	K17
      0x54160011,  //  000A  LDINT	R5	18
      0x58180001,  //  000B  LDCONST	R6	K1
      0x5C1C0400,  //  000C  MOVE	R7	R2
      0x7C0C0800,  //  000D  CALL	R3	4
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_chg_led_mode
********************************************************************/
be_local_closure(class_AXP202_set_chg_led_mode,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_set_chg_led_mode,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x8C080105,  //  0000  GETMET	R2	R0	K5
      0x54120031,  //  0001  LDINT	R4	50
      0x7C080400,  //  0002  CALL	R2	2
      0x540E00CE,  //  0003  LDINT	R3	207
      0x2C0C0403,  //  0004  AND	R3	R2	R3
      0x54120007,  //  0005  LDINT	R4	8
      0x300C0604,  //  0006  OR	R3	R3	R4
      0x5C080600,  //  0007  MOVE	R2	R3
      0x2C0C0303,  //  0008  AND	R3	R1	K3
      0x54120003,  //  0009  LDINT	R4	4
      0x380C0604,  //  000A  SHL	R3	R3	R4
      0x30080403,  //  000B  OR	R2	R2	R3
      0x8C0C0104,  //  000C  GETMET	R3	R0	K4
      0x54160031,  //  000D  LDINT	R5	50
      0x5C180400,  //  000E  MOVE	R6	R2
      0x7C0C0600,  //  000F  CALL	R3	3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_AXP202_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x54060034,  //  0003  LDINT	R1	53
      0x60080003,  //  0004  GETGBL	R2	G3
      0x5C0C0000,  //  0005  MOVE	R3	R0
      0xB8122400,  //  0006  GETNGBL	R4	K18
      0x7C080400,  //  0007  CALL	R2	2
      0x8C080513,  //  0008  GETMET	R2	R2	K19
      0x58100014,  //  0009  LDCONST	R4	K20
      0x54160034,  //  000A  LDINT	R5	53
      0x7C080600,  //  000B  CALL	R2	3
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_exten
********************************************************************/
be_local_closure(class_AXP202_set_exten,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_set_exten,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080111,  //  0000  GETMET	R2	R0	K17
      0x54120011,  //  0001  LDINT	R4	18
      0x58140000,  //  0002  LDCONST	R5	K0
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C080800,  //  0004  CALL	R2	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ldo_voltage
********************************************************************/
be_local_closure(class_AXP202_set_ldo_voltage,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_set_ldo_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0x540E0CE3,  //  0000  LDINT	R3	3300
      0x240C0403,  //  0001  GT	R3	R2	R3
      0x780E0000,  //  0002  JMPF	R3	#0004
      0x540A000E,  //  0003  LDINT	R2	15
      0x1C0C0302,  //  0004  EQ	R3	R1	K2
      0x740E0002,  //  0005  JMPT	R3	#0009
      0x540E0003,  //  0006  LDINT	R3	4
      0x1C0C0203,  //  0007  EQ	R3	R1	R3
      0x780E0020,  //  0008  JMPF	R3	#002A
      0x540E0063,  //  0009  LDINT	R3	100
      0x0C0C0403,  //  000A  DIV	R3	R2	R3
      0x54120011,  //  000B  LDINT	R4	18
      0x040C0604,  //  000C  SUB	R3	R3	R4
      0x5C080600,  //  000D  MOVE	R2	R3
      0x1C0C0302,  //  000E  EQ	R3	R1	K2
      0x780E000D,  //  000F  JMPF	R3	#001E
      0x8C0C0104,  //  0010  GETMET	R3	R0	K4
      0x54160027,  //  0011  LDINT	R5	40
      0x8C180105,  //  0012  GETMET	R6	R0	K5
      0x54220027,  //  0013  LDINT	R8	40
      0x7C180400,  //  0014  CALL	R6	2
      0x541E000E,  //  0015  LDINT	R7	15
      0x2C180C07,  //  0016  AND	R6	R6	R7
      0x541E000E,  //  0017  LDINT	R7	15
      0x2C1C0407,  //  0018  AND	R7	R2	R7
      0x54220003,  //  0019  LDINT	R8	4
      0x381C0E08,  //  001A  SHL	R7	R7	R8
      0x30180C07,  //  001B  OR	R6	R6	R7
      0x7C0C0600,  //  001C  CALL	R3	3
      0x7002000A,  //  001D  JMP		#0029
      0x8C0C0104,  //  001E  GETMET	R3	R0	K4
      0x54160027,  //  001F  LDINT	R5	40
      0x8C180105,  //  0020  GETMET	R6	R0	K5
      0x54220027,  //  0021  LDINT	R8	40
      0x7C180400,  //  0022  CALL	R6	2
      0x541E00EF,  //  0023  LDINT	R7	240
      0x2C180C07,  //  0024  AND	R6	R6	R7
      0x541E000E,  //  0025  LDINT	R7	15
      0x2C1C0407,  //  0026  AND	R7	R2	R7
      0x30180C07,  //  0027  OR	R6	R6	R7
      0x7C0C0600,  //  0028  CALL	R3	3
      0x7002000F,  //  0029  JMP		#003A
      0x540E0018,  //  002A  LDINT	R3	25
      0x0C0C0403,  //  002B  DIV	R3	R2	R3
      0x5412001B,  //  002C  LDINT	R4	28
      0x040C0604,  //  002D  SUB	R3	R3	R4
      0x5C080600,  //  002E  MOVE	R2	R3
      0x8C0C0104,  //  002F  GETMET	R3	R0	K4
      0x54160027,  //  0030  LDINT	R5	40
      0x8C180105,  //  0031  GETMET	R6	R0	K5
      0x54220028,  //  0032  LDINT	R8	41
      0x7C180400,  //  0033  CALL	R6	2
      0x541E007F,  //  0034  LDINT	R7	128
      0x2C180C07,  //  0035  AND	R6	R6	R7
      0x541E007E,  //  0036  LDINT	R7	127
      0x2C1C0407,  //  0037  AND	R7	R2	R7
      0x30180C07,  //  0038  OR	R6	R6	R7
      0x7C0C0600,  //  0039  CALL	R3	3
      0x80000000,  //  003A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_dc_voltage
********************************************************************/
be_local_closure(class_AXP202_set_dc_voltage,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_set_dc_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x140C0302,  //  0000  LT	R3	R1	K2
      0x740E0001,  //  0001  JMPT	R3	#0004
      0x240C0303,  //  0002  GT	R3	R1	K3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x4C0C0000,  //  0005  LDNIL	R3
      0x541202BB,  //  0006  LDINT	R4	700
      0x14100404,  //  0007  LT	R4	R2	R4
      0x78120001,  //  0008  JMPF	R4	#000B
      0x580C0000,  //  0009  LDCONST	R3	K0
      0x70020010,  //  000A  JMP		#001C
      0x54120DAB,  //  000B  LDINT	R4	3500
      0x24100404,  //  000C  GT	R4	R2	R4
      0x78120001,  //  000D  JMPF	R4	#0010
      0x540E006F,  //  000E  LDINT	R3	112
      0x7002000B,  //  000F  JMP		#001C
      0x1C100302,  //  0010  EQ	R4	R1	K2
      0x78120004,  //  0011  JMPF	R4	#0017
      0x541208E2,  //  0012  LDINT	R4	2275
      0x24100404,  //  0013  GT	R4	R2	R4
      0x78120001,  //  0014  JMPF	R4	#0017
      0x540E003E,  //  0015  LDINT	R3	63
      0x70020004,  //  0016  JMP		#001C
      0x541202BB,  //  0017  LDINT	R4	700
      0x04100404,  //  0018  SUB	R4	R2	R4
      0x54160018,  //  0019  LDINT	R5	25
      0x0C100805,  //  001A  DIV	R4	R4	R5
      0x5C0C0800,  //  001B  MOVE	R3	R4
      0x54120022,  //  001C  LDINT	R4	35
      0x1C140303,  //  001D  EQ	R5	R1	K3
      0x78160000,  //  001E  JMPF	R5	#0020
      0x54120026,  //  001F  LDINT	R4	39
      0x8C140104,  //  0020  GETMET	R5	R0	K4
      0x5C1C0800,  //  0021  MOVE	R7	R4
      0x8C200105,  //  0022  GETMET	R8	R0	K5
      0x5C280800,  //  0023  MOVE	R10	R4
      0x7C200400,  //  0024  CALL	R8	2
      0x5426007F,  //  0025  LDINT	R9	128
      0x2C201009,  //  0026  AND	R8	R8	R9
      0x5426007E,  //  0027  LDINT	R9	127
      0x2C240609,  //  0028  AND	R9	R3	R9
      0x30201009,  //  0029  OR	R8	R8	R9
      0x7C140600,  //  002A  CALL	R5	3
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_voltage
********************************************************************/
be_local_closure(class_AXP202_get_bat_voltage,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_bat_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x540E0077,  //  0001  LDINT	R3	120
      0x7C040400,  //  0002  CALL	R1	2
      0x08040315,  //  0003  MUL	R1	R1	K21
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_temp
********************************************************************/
be_local_closure(class_AXP202_get_temp,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_temp,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040106,  //  0000  GETMET	R1	R0	K6
      0x540E005D,  //  0001  LDINT	R3	94
      0x7C040400,  //  0002  CALL	R1	2
      0x08040316,  //  0003  MUL	R1	R1	K22
      0x04040317,  //  0004  SUB	R1	R1	K23
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ldo_enable
********************************************************************/
be_local_closure(class_AXP202_set_ldo_enable,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_set_ldo_enable,
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x1C0C0302,  //  0000  EQ	R3	R1	K2
      0x780E0004,  //  0001  JMPF	R3	#0007
      0x8C0C0111,  //  0002  GETMET	R3	R0	K17
      0x54160011,  //  0003  LDINT	R5	18
      0x58180002,  //  0004  LDCONST	R6	K2
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x7C0C0800,  //  0006  CALL	R3	4
      0x1C0C0303,  //  0007  EQ	R3	R1	K3
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x8C0C0111,  //  0009  GETMET	R3	R0	K17
      0x54160011,  //  000A  LDINT	R5	18
      0x541A0005,  //  000B  LDINT	R6	6
      0x5C1C0400,  //  000C  MOVE	R7	R2
      0x7C0C0800,  //  000D  CALL	R3	4
      0x540E0003,  //  000E  LDINT	R3	4
      0x1C0C0203,  //  000F  EQ	R3	R1	R3
      0x780E0004,  //  0010  JMPF	R3	#0016
      0x8C0C0111,  //  0011  GETMET	R3	R0	K17
      0x54160011,  //  0012  LDINT	R5	18
      0x58180003,  //  0013  LDCONST	R6	K3
      0x5C1C0400,  //  0014  MOVE	R7	R2
      0x7C0C0800,  //  0015  CALL	R3	4
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_limiting_off
********************************************************************/
be_local_closure(class_AXP202_set_limiting_off,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_set_limiting_off,
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8C040104,  //  0000  GETMET	R1	R0	K4
      0x540E002F,  //  0001  LDINT	R3	48
      0x8C100105,  //  0002  GETMET	R4	R0	K5
      0x541A002F,  //  0003  LDINT	R6	48
      0x7C100400,  //  0004  CALL	R4	2
      0x30100903,  //  0005  OR	R4	R4	K3
      0x7C040600,  //  0006  CALL	R1	3
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_input_power_status
********************************************************************/
be_local_closure(class_AXP202_get_input_power_status,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_AXP202,     /* shared constants */
    &be_const_str_get_input_power_status,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x8C04030D,  //  0001  GETMET	R1	R1	K13
      0x880C010E,  //  0002  GETMBR	R3	R0	K14
      0x58100000,  //  0003  LDCONST	R4	K0
      0x58140001,  //  0004  LDCONST	R5	K1
      0x7C040800,  //  0005  CALL	R1	4
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: AXP202
********************************************************************/
extern const bclass be_class_I2C_Driver;
be_local_class(AXP202,
    0,
    &be_class_I2C_Driver,
    be_nested_map(21,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(set_shutdown_time, -1), be_const_closure(class_AXP202_set_shutdown_time_closure) },
        { be_const_key(get_vbus_current, -1), be_const_closure(class_AXP202_get_vbus_current_closure) },
        { be_const_key(get_aps_voltage, -1), be_const_closure(class_AXP202_get_aps_voltage_closure) },
        { be_const_key(get_bat_current, -1), be_const_closure(class_AXP202_get_bat_current_closure) },
        { be_const_key(get_bat_power, 2), be_const_closure(class_AXP202_get_bat_power_closure) },
        { be_const_key(get_battery_charging_status, -1), be_const_closure(class_AXP202_get_battery_charging_status_closure) },
        { be_const_key(get_vbus_voltage, -1), be_const_closure(class_AXP202_get_vbus_voltage_closure) },
        { be_const_key(set_ldo_voltage, -1), be_const_closure(class_AXP202_set_ldo_voltage_closure) },
        { be_const_key(battery_present, -1), be_const_closure(class_AXP202_battery_present_closure) },
        { be_const_key(set_chg_current_ma, -1), be_const_closure(class_AXP202_set_chg_current_ma_closure) },
        { be_const_key(set_dcdc_enable, -1), be_const_closure(class_AXP202_set_dcdc_enable_closure) },
        { be_const_key(set_chg_led_mode, 16), be_const_closure(class_AXP202_set_chg_led_mode_closure) },
        { be_const_key(init, 14), be_const_closure(class_AXP202_init_closure) },
        { be_const_key(set_exten, -1), be_const_closure(class_AXP202_set_exten_closure) },
        { be_const_key(set_ldo_enable, -1), be_const_closure(class_AXP202_set_ldo_enable_closure) },
        { be_const_key(get_bat_charge_current, 17), be_const_closure(class_AXP202_get_bat_charge_current_closure) },
        { be_const_key(get_temp, 7), be_const_closure(class_AXP202_get_temp_closure) },
        { be_const_key(get_bat_voltage, 18), be_const_closure(class_AXP202_get_bat_voltage_closure) },
        { be_const_key(set_dc_voltage, -1), be_const_closure(class_AXP202_set_dc_voltage_closure) },
        { be_const_key(set_limiting_off, -1), be_const_closure(class_AXP202_set_limiting_off_closure) },
        { be_const_key(get_input_power_status, -1), be_const_closure(class_AXP202_get_input_power_status_closure) },
    })),
    (bstring*) &be_const_str_AXP202
);
/********************************************************************/
/* End of solidification */
