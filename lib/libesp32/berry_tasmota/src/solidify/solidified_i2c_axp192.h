/* Solidification of i2c_axp192.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_AXP192;

/********************************************************************
** Solidified function: set_dcdc_enable
********************************************************************/
be_local_closure(AXP192_set_dcdc_enable,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(1),
    /* K1   */  be_nested_str(write_bit),
    /* K2   */  be_const_int(0),
    /* K3   */  be_const_int(2),
    /* K4   */  be_const_int(3),
    }),
    &be_const_str_set_dcdc_enable,
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0004,  //  0001  JMPF	R3	#0007
      0x8C0C0101,  //  0002  GETMET	R3	R0	K1
      0x54160011,  //  0003  LDINT	R5	18
      0x58180002,  //  0004  LDCONST	R6	K2
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x7C0C0800,  //  0006  CALL	R3	4
      0x1C0C0303,  //  0007  EQ	R3	R1	K3
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x8C0C0101,  //  0009  GETMET	R3	R0	K1
      0x54160011,  //  000A  LDINT	R5	18
      0x541A0003,  //  000B  LDINT	R6	4
      0x5C1C0400,  //  000C  MOVE	R7	R2
      0x7C0C0800,  //  000D  CALL	R3	4
      0x1C0C0304,  //  000E  EQ	R3	R1	K4
      0x780E0004,  //  000F  JMPF	R3	#0015
      0x8C0C0101,  //  0010  GETMET	R3	R0	K1
      0x54160011,  //  0011  LDINT	R5	18
      0x58180000,  //  0012  LDCONST	R6	K0
      0x5C1C0400,  //  0013  MOVE	R7	R2
      0x7C0C0800,  //  0014  CALL	R3	4
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_power
********************************************************************/
be_local_closure(AXP192_get_bat_power,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(read24),
    /* K1   */  be_const_real_hex(0x3A102DE0),
    }),
    &be_const_str_get_bat_power,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E006F,  //  0001  LDINT	R3	112
      0x7C040400,  //  0002  CALL	R1	2
      0x08040301,  //  0003  MUL	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_voltage
********************************************************************/
be_local_closure(AXP192_get_bat_voltage,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(read12),
    /* K1   */  be_const_real_hex(0x3A902DE0),
    }),
    &be_const_str_get_bat_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0077,  //  0001  LDINT	R3	120
      0x7C040400,  //  0002  CALL	R1	2
      0x08040301,  //  0003  MUL	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_vbus_current
********************************************************************/
be_local_closure(AXP192_get_vbus_current,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(read12),
    /* K1   */  be_const_real_hex(0x3EC00000),
    }),
    &be_const_str_get_vbus_current,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E005B,  //  0001  LDINT	R3	92
      0x7C040400,  //  0002  CALL	R1	2
      0x08040301,  //  0003  MUL	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_warning_level
********************************************************************/
be_local_closure(AXP192_get_warning_level,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(read12),
    /* K1   */  be_const_int(1),
    }),
    &be_const_str_get_warning_level,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0046,  //  0001  LDINT	R3	71
      0x7C040400,  //  0002  CALL	R1	2
      0x2C040301,  //  0003  AND	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ldo_enable
********************************************************************/
be_local_closure(AXP192_set_ldo_enable,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_const_int(2),
    /* K1   */  be_nested_str(write_bit),
    /* K2   */  be_const_int(3),
    }),
    &be_const_str_set_ldo_enable,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x1C0C0300,  //  0000  EQ	R3	R1	K0
      0x780E0004,  //  0001  JMPF	R3	#0007
      0x8C0C0101,  //  0002  GETMET	R3	R0	K1
      0x54160011,  //  0003  LDINT	R5	18
      0x58180000,  //  0004  LDCONST	R6	K0
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x7C0C0800,  //  0006  CALL	R3	4
      0x1C0C0302,  //  0007  EQ	R3	R1	K2
      0x780E0004,  //  0008  JMPF	R3	#000E
      0x8C0C0101,  //  0009  GETMET	R3	R0	K1
      0x54160011,  //  000A  LDINT	R5	18
      0x58180002,  //  000B  LDCONST	R6	K2
      0x5C1C0400,  //  000C  MOVE	R7	R2
      0x7C0C0800,  //  000D  CALL	R3	4
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_input_power_status
********************************************************************/
be_local_closure(AXP192_get_input_power_status,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(wire),
    /* K1   */  be_nested_str(read),
    /* K2   */  be_nested_str(addr),
    /* K3   */  be_const_int(0),
    /* K4   */  be_const_int(1),
    }),
    &be_const_str_get_input_power_status,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x58140004,  //  0004  LDCONST	R5	K4
      0x7C040800,  //  0005  CALL	R1	4
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_aps_voltage
********************************************************************/
be_local_closure(AXP192_get_aps_voltage,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(read12),
    /* K1   */  be_const_real_hex(0x3AB78035),
    }),
    &be_const_str_get_aps_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E007D,  //  0001  LDINT	R3	126
      0x7C040400,  //  0002  CALL	R1	2
      0x08040301,  //  0003  MUL	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_exten
********************************************************************/
be_local_closure(AXP192_set_exten,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str(write_bit),
    }),
    &be_const_str_set_exten,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x54120011,  //  0001  LDINT	R4	18
      0x54160005,  //  0002  LDINT	R5	6
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C080800,  //  0004  CALL	R2	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: battery_present
********************************************************************/
be_local_closure(AXP192_battery_present,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(wire),
    /* K1   */  be_nested_str(read),
    /* K2   */  be_nested_str(addr),
    /* K3   */  be_const_int(1),
    }),
    &be_const_str_battery_present,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x58140003,  //  0004  LDCONST	R5	K3
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
** Solidified function: get_vbus_voltage
********************************************************************/
be_local_closure(AXP192_get_vbus_voltage,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(read12),
    /* K1   */  be_const_real_hex(0x3ADED28A),
    }),
    &be_const_str_get_vbus_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0059,  //  0001  LDINT	R3	90
      0x7C040400,  //  0002  CALL	R1	2
      0x08040301,  //  0003  MUL	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_gpio
********************************************************************/
be_local_closure(AXP192_write_gpio,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_const_int(2),
    /* K2   */  be_nested_str(write_bit),
    /* K3   */  be_const_int(3),
    }),
    &be_const_str_write_gpio,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x280C0300,  //  0000  GE	R3	R1	K0
      0x780E0007,  //  0001  JMPF	R3	#000A
      0x180C0301,  //  0002  LE	R3	R1	K1
      0x780E0005,  //  0003  JMPF	R3	#000A
      0x8C0C0102,  //  0004  GETMET	R3	R0	K2
      0x54160093,  //  0005  LDINT	R5	148
      0x5C180200,  //  0006  MOVE	R6	R1
      0x5C1C0400,  //  0007  MOVE	R7	R2
      0x7C0C0800,  //  0008  CALL	R3	4
      0x70020009,  //  0009  JMP		#0014
      0x280C0303,  //  000A  GE	R3	R1	K3
      0x780E0007,  //  000B  JMPF	R3	#0014
      0x540E0003,  //  000C  LDINT	R3	4
      0x180C0203,  //  000D  LE	R3	R1	R3
      0x780E0004,  //  000E  JMPF	R3	#0014
      0x8C0C0102,  //  000F  GETMET	R3	R0	K2
      0x54160095,  //  0010  LDINT	R5	150
      0x04180303,  //  0011  SUB	R6	R1	K3
      0x5C1C0400,  //  0012  MOVE	R7	R2
      0x7C0C0800,  //  0013  CALL	R3	4
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ldo_voltage
********************************************************************/
be_local_closure(AXP192_set_ldo_voltage,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(2),
    /* K1   */  be_nested_str(write8),
    /* K2   */  be_nested_str(read8),
    /* K3   */  be_const_int(3),
    }),
    &be_const_str_set_ldo_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x540E0CE3,  //  0000  LDINT	R3	3300
      0x240C0403,  //  0001  GT	R3	R2	R3
      0x780E0001,  //  0002  JMPF	R3	#0005
      0x540A000E,  //  0003  LDINT	R2	15
      0x70020004,  //  0004  JMP		#000A
      0x540E0063,  //  0005  LDINT	R3	100
      0x0C0C0403,  //  0006  DIV	R3	R2	R3
      0x54120011,  //  0007  LDINT	R4	18
      0x040C0604,  //  0008  SUB	R3	R3	R4
      0x5C080600,  //  0009  MOVE	R2	R3
      0x1C0C0300,  //  000A  EQ	R3	R1	K0
      0x780E000C,  //  000B  JMPF	R3	#0019
      0x8C0C0101,  //  000C  GETMET	R3	R0	K1
      0x54160027,  //  000D  LDINT	R5	40
      0x8C180102,  //  000E  GETMET	R6	R0	K2
      0x54220027,  //  000F  LDINT	R8	40
      0x7C180400,  //  0010  CALL	R6	2
      0x541E000E,  //  0011  LDINT	R7	15
      0x2C180C07,  //  0012  AND	R6	R6	R7
      0x541E000E,  //  0013  LDINT	R7	15
      0x2C1C0407,  //  0014  AND	R7	R2	R7
      0x54220003,  //  0015  LDINT	R8	4
      0x381C0E08,  //  0016  SHL	R7	R7	R8
      0x30180C07,  //  0017  OR	R6	R6	R7
      0x7C0C0600,  //  0018  CALL	R3	3
      0x1C0C0303,  //  0019  EQ	R3	R1	K3
      0x780E000A,  //  001A  JMPF	R3	#0026
      0x8C0C0101,  //  001B  GETMET	R3	R0	K1
      0x54160027,  //  001C  LDINT	R5	40
      0x8C180102,  //  001D  GETMET	R6	R0	K2
      0x54220027,  //  001E  LDINT	R8	40
      0x7C180400,  //  001F  CALL	R6	2
      0x541E00EF,  //  0020  LDINT	R7	240
      0x2C180C07,  //  0021  AND	R6	R6	R7
      0x541E000E,  //  0022  LDINT	R7	15
      0x2C1C0407,  //  0023  AND	R7	R2	R7
      0x30180C07,  //  0024  OR	R6	R6	R7
      0x7C0C0600,  //  0025  CALL	R3	3
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: json_append
********************************************************************/
be_local_closure(AXP192_json_append,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(wire),
    /* K1   */  be_nested_str(_X2C_X22AXP192_X22_X3A_X7B_X22VBusVoltage_X22_X3A_X25_X2E3f_X2C_X22VBusCurrent_X22_X3A_X25_X2E1f_X2C_X22BattVoltage_X22_X3A_X25_X2E3f_X2C_X22BattCurrent_X22_X3A_X25_X2E1f_X2C_X22Temperature_X22_X3A_X25_X2E1f_X7D),
    /* K2   */  be_nested_str(get_vbus_voltage),
    /* K3   */  be_nested_str(get_bat_voltage),
    /* K4   */  be_nested_str(get_bat_current),
    /* K5   */  be_nested_str(get_temp),
    /* K6   */  be_nested_str(tasmota),
    /* K7   */  be_nested_str(response_append),
    }),
    &be_const_str_json_append,
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060001,  //  0001  JMPT	R1	#0004
      0x4C040000,  //  0002  LDNIL	R1
      0x80040200,  //  0003  RET	1	R1
      0x60040018,  //  0004  GETGBL	R1	G24
      0x58080001,  //  0005  LDCONST	R2	K1
      0x8C0C0102,  //  0006  GETMET	R3	R0	K2
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C100102,  //  0008  GETMET	R4	R0	K2
      0x7C100200,  //  0009  CALL	R4	1
      0x8C140103,  //  000A  GETMET	R5	R0	K3
      0x7C140200,  //  000B  CALL	R5	1
      0x8C180104,  //  000C  GETMET	R6	R0	K4
      0x7C180200,  //  000D  CALL	R6	1
      0x8C1C0105,  //  000E  GETMET	R7	R0	K5
      0x7C1C0200,  //  000F  CALL	R7	1
      0x7C040C00,  //  0010  CALL	R1	6
      0xB80A0C00,  //  0011  GETNGBL	R2	K6
      0x8C080507,  //  0012  GETMET	R2	R2	K7
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C080400,  //  0014  CALL	R2	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(AXP192_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(I2C_Driver),
    /* K1   */  be_nested_str(init),
    /* K2   */  be_nested_str(AXP192),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0xB80E0000,  //  0002  GETNGBL	R3	K0
      0x7C040400,  //  0003  CALL	R1	2
      0x8C040301,  //  0004  GETMET	R1	R1	K1
      0x580C0002,  //  0005  LDCONST	R3	K2
      0x54120033,  //  0006  LDINT	R4	52
      0x7C040600,  //  0007  CALL	R1	3
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_current
********************************************************************/
be_local_closure(AXP192_get_bat_current,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(read13),
    /* K1   */  be_const_real_hex(0x3F000000),
    }),
    &be_const_str_get_bat_current,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0079,  //  0001  LDINT	R3	122
      0x7C040400,  //  0002  CALL	R1	2
      0x8C080100,  //  0003  GETMET	R2	R0	K0
      0x5412007B,  //  0004  LDINT	R4	124
      0x7C080400,  //  0005  CALL	R2	2
      0x04040202,  //  0006  SUB	R1	R1	R2
      0x08040301,  //  0007  MUL	R1	R1	K1
      0x80040200,  //  0008  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_sensor
********************************************************************/
be_local_closure(AXP192_web_sensor,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(wire),
    /* K1   */  be_nested_str(_X7Bs_X7DVBus_X20Voltage_X7Bm_X7D_X25_X2E3f_X20V_X7Be_X7D),
    /* K2   */  be_nested_str(_X7Bs_X7DVBus_X20Current_X7Bm_X7D_X25_X2E1f_X20mA_X7Be_X7D),
    /* K3   */  be_nested_str(_X7Bs_X7DBatt_X20Voltage_X7Bm_X7D_X25_X2E3f_X20V_X7Be_X7D),
    /* K4   */  be_nested_str(_X7Bs_X7DBatt_X20Current_X7Bm_X7D_X25_X2E1f_X20mA_X7Be_X7D),
    /* K5   */  be_nested_str(_X7Bs_X7DTemp_X20AXP_X7Bm_X7D_X25_X2E1f_X20_X26deg_X3BC_X7Be_X7D),
    /* K6   */  be_nested_str(get_vbus_voltage),
    /* K7   */  be_nested_str(get_bat_voltage),
    /* K8   */  be_nested_str(get_bat_current),
    /* K9   */  be_nested_str(get_temp),
    /* K10  */  be_nested_str(tasmota),
    /* K11  */  be_nested_str(web_send_decimal),
    }),
    &be_const_str_web_sensor,
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060001,  //  0001  JMPT	R1	#0004
      0x4C040000,  //  0002  LDNIL	R1
      0x80040200,  //  0003  RET	1	R1
      0x60040018,  //  0004  GETGBL	R1	G24
      0x400A0302,  //  0005  CONNECT	R2	K1	K2
      0x40080503,  //  0006  CONNECT	R2	R2	K3
      0x40080504,  //  0007  CONNECT	R2	R2	K4
      0x40080505,  //  0008  CONNECT	R2	R2	K5
      0x8C0C0106,  //  0009  GETMET	R3	R0	K6
      0x7C0C0200,  //  000A  CALL	R3	1
      0x8C100106,  //  000B  GETMET	R4	R0	K6
      0x7C100200,  //  000C  CALL	R4	1
      0x8C140107,  //  000D  GETMET	R5	R0	K7
      0x7C140200,  //  000E  CALL	R5	1
      0x8C180108,  //  000F  GETMET	R6	R0	K8
      0x7C180200,  //  0010  CALL	R6	1
      0x8C1C0109,  //  0011  GETMET	R7	R0	K9
      0x7C1C0200,  //  0012  CALL	R7	1
      0x7C040C00,  //  0013  CALL	R1	6
      0xB80A1400,  //  0014  GETNGBL	R2	K10
      0x8C08050B,  //  0015  GETMET	R2	R2	K11
      0x5C100200,  //  0016  MOVE	R4	R1
      0x7C080400,  //  0017  CALL	R2	2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_chg_current
********************************************************************/
be_local_closure(AXP192_set_chg_current,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(write8),
    /* K1   */  be_nested_str(read8),
    }),
    &be_const_str_set_chg_current,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x54120032,  //  0001  LDINT	R4	51
      0x8C140101,  //  0002  GETMET	R5	R0	K1
      0x541E0032,  //  0003  LDINT	R7	51
      0x7C140400,  //  0004  CALL	R5	2
      0x541A00EF,  //  0005  LDINT	R6	240
      0x2C140A06,  //  0006  AND	R5	R5	R6
      0x541A000E,  //  0007  LDINT	R6	15
      0x2C180206,  //  0008  AND	R6	R1	R6
      0x30140A06,  //  0009  OR	R5	R5	R6
      0x7C080600,  //  000A  CALL	R2	3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_temp
********************************************************************/
be_local_closure(AXP192_get_temp,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str(read12),
    /* K1   */  be_const_real_hex(0x3DCCCCCD),
    /* K2   */  be_const_real_hex(0x4310B333),
    }),
    &be_const_str_get_temp,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E005D,  //  0001  LDINT	R3	94
      0x7C040400,  //  0002  CALL	R1	2
      0x08040301,  //  0003  MUL	R1	R1	K1
      0x04040302,  //  0004  SUB	R1	R1	K2
      0x80040200,  //  0005  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_dc_voltage
********************************************************************/
be_local_closure(AXP192_set_dc_voltage,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(1),
    /* K1   */  be_const_int(3),
    /* K2   */  be_const_int(0),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str(write8),
    /* K5   */  be_nested_str(read8),
    }),
    &be_const_str_set_dc_voltage,
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x140C0300,  //  0000  LT	R3	R1	K0
      0x740E0001,  //  0001  JMPT	R3	#0004
      0x240C0301,  //  0002  GT	R3	R1	K1
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x80000600,  //  0004  RET	0
      0x4C0C0000,  //  0005  LDNIL	R3
      0x541202BB,  //  0006  LDINT	R4	700
      0x14100404,  //  0007  LT	R4	R2	R4
      0x78120001,  //  0008  JMPF	R4	#000B
      0x580C0002,  //  0009  LDCONST	R3	K2
      0x70020010,  //  000A  JMP		#001C
      0x54120DAB,  //  000B  LDINT	R4	3500
      0x24100404,  //  000C  GT	R4	R2	R4
      0x78120001,  //  000D  JMPF	R4	#0010
      0x540E006F,  //  000E  LDINT	R3	112
      0x7002000B,  //  000F  JMP		#001C
      0x1C100303,  //  0010  EQ	R4	R1	K3
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
      0x54120025,  //  001C  LDINT	R4	38
      0x1C140301,  //  001D  EQ	R5	R1	K1
      0x78160001,  //  001E  JMPF	R5	#0021
      0x54120026,  //  001F  LDINT	R4	39
      0x70020002,  //  0020  JMP		#0024
      0x1C140303,  //  0021  EQ	R5	R1	K3
      0x78160000,  //  0022  JMPF	R5	#0024
      0x54120022,  //  0023  LDINT	R4	35
      0x8C140104,  //  0024  GETMET	R5	R0	K4
      0x5C1C0800,  //  0025  MOVE	R7	R4
      0x8C200105,  //  0026  GETMET	R8	R0	K5
      0x5C280800,  //  0027  MOVE	R10	R4
      0x7C200400,  //  0028  CALL	R8	2
      0x5426007F,  //  0029  LDINT	R9	128
      0x2C201009,  //  002A  AND	R8	R8	R9
      0x5426007E,  //  002B  LDINT	R9	127
      0x2C240609,  //  002C  AND	R9	R3	R9
      0x30201009,  //  002D  OR	R8	R8	R9
      0x7C140600,  //  002E  CALL	R5	3
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: power_off
********************************************************************/
be_local_closure(AXP192_power_off,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(write_bit),
    /* K1   */  be_const_int(1),
    }),
    &be_const_str_power_off,
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0031,  //  0001  LDINT	R3	50
      0x54120006,  //  0002  LDINT	R4	7
      0x58140001,  //  0003  LDCONST	R5	K1
      0x7C040800,  //  0004  CALL	R1	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_battery_chargin_status
********************************************************************/
be_local_closure(AXP192_get_battery_chargin_status,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(wire),
    /* K1   */  be_nested_str(read),
    /* K2   */  be_nested_str(addr),
    /* K3   */  be_const_int(1),
    }),
    &be_const_str_get_battery_chargin_status,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x58140003,  //  0004  LDCONST	R5	K3
      0x7C040800,  //  0005  CALL	R1	4
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_bat_charge_current
********************************************************************/
be_local_closure(AXP192_get_bat_charge_current,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(read13),
    /* K1   */  be_const_real_hex(0x3F000000),
    }),
    &be_const_str_get_bat_charge_current,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0079,  //  0001  LDINT	R3	122
      0x7C040400,  //  0002  CALL	R1	2
      0x08040301,  //  0003  MUL	R1	R1	K1
      0x80040200,  //  0004  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: AXP192
********************************************************************/
extern const bclass be_class_I2C_Driver;
be_local_class(AXP192,
    0,
    &be_class_I2C_Driver,
    be_nested_map(23,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(set_dcdc_enable, -1), be_const_closure(AXP192_set_dcdc_enable_closure) },
        { be_const_key(get_bat_power, -1), be_const_closure(AXP192_get_bat_power_closure) },
        { be_const_key(get_bat_charge_current, 3), be_const_closure(AXP192_get_bat_charge_current_closure) },
        { be_const_key(get_battery_chargin_status, -1), be_const_closure(AXP192_get_battery_chargin_status_closure) },
        { be_const_key(get_warning_level, -1), be_const_closure(AXP192_get_warning_level_closure) },
        { be_const_key(power_off, -1), be_const_closure(AXP192_power_off_closure) },
        { be_const_key(get_vbus_current, 22), be_const_closure(AXP192_get_vbus_current_closure) },
        { be_const_key(get_aps_voltage, -1), be_const_closure(AXP192_get_aps_voltage_closure) },
        { be_const_key(set_exten, -1), be_const_closure(AXP192_set_exten_closure) },
        { be_const_key(battery_present, -1), be_const_closure(AXP192_battery_present_closure) },
        { be_const_key(get_vbus_voltage, -1), be_const_closure(AXP192_get_vbus_voltage_closure) },
        { be_const_key(write_gpio, -1), be_const_closure(AXP192_write_gpio_closure) },
        { be_const_key(set_ldo_voltage, -1), be_const_closure(AXP192_set_ldo_voltage_closure) },
        { be_const_key(set_ldo_enable, 21), be_const_closure(AXP192_set_ldo_enable_closure) },
        { be_const_key(init, 2), be_const_closure(AXP192_init_closure) },
        { be_const_key(get_bat_current, -1), be_const_closure(AXP192_get_bat_current_closure) },
        { be_const_key(web_sensor, -1), be_const_closure(AXP192_web_sensor_closure) },
        { be_const_key(set_dc_voltage, -1), be_const_closure(AXP192_set_dc_voltage_closure) },
        { be_const_key(get_temp, -1), be_const_closure(AXP192_get_temp_closure) },
        { be_const_key(set_chg_current, 17), be_const_closure(AXP192_set_chg_current_closure) },
        { be_const_key(get_bat_voltage, 5), be_const_closure(AXP192_get_bat_voltage_closure) },
        { be_const_key(json_append, -1), be_const_closure(AXP192_json_append_closure) },
        { be_const_key(get_input_power_status, -1), be_const_closure(AXP192_get_input_power_status_closure) },
    })),
    (bstring*) &be_const_str_AXP192
);
/*******************************************************************/

void be_load_AXP192_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_AXP192);
    be_setglobal(vm, "AXP192");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
