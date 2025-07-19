/* Solidification of i2c_driver.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'I2C_Driver' ktab size: 20, total: 71 (saved 408 bytes)
static const bvalue be_ktab_class_I2C_Driver[20] = {
  /* K0   */  be_nested_str(wire),
  /* K1   */  be_nested_str(read_bytes),
  /* K2   */  be_nested_str(addr),
  /* K3   */  be_const_int(0),
  /* K4   */  be_const_int(1),
  /* K5   */  be_const_int(2),
  /* K6   */  be_const_int(3),
  /* K7   */  be_nested_str(tasmota),
  /* K8   */  be_nested_str(i2c_enabled),
  /* K9   */  be_nested_str(wire_scan),
  /* K10  */  be_nested_str(function),
  /* K11  */  be_nested_str(name),
  /* K12  */  be_nested_str(I2C_X3A),
  /* K13  */  be_nested_str(detected_X20on_X20bus),
  /* K14  */  be_nested_str(bus),
  /* K15  */  be_nested_str(write8),
  /* K16  */  be_nested_str(read8),
  /* K17  */  be_nested_str(write16),
  /* K18  */  be_nested_str(read),
  /* K19  */  be_nested_str(write),
};


extern const bclass be_class_I2C_Driver;

/********************************************************************
** Solidified function: read32
********************************************************************/
be_local_closure(class_I2C_Driver_read32,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_read32,
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x541A0003,  //  0004  LDINT	R6	4
      0x7C080800,  //  0005  CALL	R2	4
      0x940C0503,  //  0006  GETIDX	R3	R2	K3
      0x54120017,  //  0007  LDINT	R4	24
      0x380C0604,  //  0008  SHL	R3	R3	R4
      0x94100504,  //  0009  GETIDX	R4	R2	K4
      0x5416000F,  //  000A  LDINT	R5	16
      0x38100805,  //  000B  SHL	R4	R4	R5
      0x000C0604,  //  000C  ADD	R3	R3	R4
      0x94100505,  //  000D  GETIDX	R4	R2	K5
      0x54160007,  //  000E  LDINT	R5	8
      0x38100805,  //  000F  SHL	R4	R4	R5
      0x000C0604,  //  0010  ADD	R3	R3	R4
      0x94100506,  //  0011  GETIDX	R4	R2	K6
      0x000C0604,  //  0012  ADD	R3	R3	R4
      0x80040600,  //  0013  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_I2C_Driver_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x20100604,  //  0001  NE	R4	R3	R4
      0x78120005,  //  0002  JMPF	R4	#0009
      0xB8120E00,  //  0003  GETNGBL	R4	K7
      0x8C100908,  //  0004  GETMET	R4	R4	K8
      0x5C180600,  //  0005  MOVE	R6	R3
      0x7C100400,  //  0006  CALL	R4	2
      0x74120000,  //  0007  JMPT	R4	#0009
      0x80000800,  //  0008  RET	0
      0x90020402,  //  0009  SETMBR	R0	K2	R2
      0xB8120E00,  //  000A  GETNGBL	R4	K7
      0x8C100909,  //  000B  GETMET	R4	R4	K9
      0x88180102,  //  000C  GETMBR	R6	R0	K2
      0x7C100400,  //  000D  CALL	R4	2
      0x90020004,  //  000E  SETMBR	R0	K0	R4
      0x88100100,  //  000F  GETMBR	R4	R0	K0
      0x78120019,  //  0010  JMPF	R4	#002B
      0x60100004,  //  0011  GETGBL	R4	G4
      0x5C140200,  //  0012  MOVE	R5	R1
      0x7C100200,  //  0013  CALL	R4	1
      0x1C10090A,  //  0014  EQ	R4	R4	K10
      0x78120004,  //  0015  JMPF	R4	#001B
      0x5C100200,  //  0016  MOVE	R4	R1
      0x5C140000,  //  0017  MOVE	R5	R0
      0x7C100200,  //  0018  CALL	R4	1
      0x90021604,  //  0019  SETMBR	R0	K11	R4
      0x70020000,  //  001A  JMP		#001C
      0x90021601,  //  001B  SETMBR	R0	K11	R1
      0x8810010B,  //  001C  GETMBR	R4	R0	K11
      0x4C140000,  //  001D  LDNIL	R5
      0x1C100805,  //  001E  EQ	R4	R4	R5
      0x78120001,  //  001F  JMPF	R4	#0022
      0x4C100000,  //  0020  LDNIL	R4
      0x90020004,  //  0021  SETMBR	R0	K0	R4
      0x88100100,  //  0022  GETMBR	R4	R0	K0
      0x78120006,  //  0023  JMPF	R4	#002B
      0x60100001,  //  0024  GETGBL	R4	G1
      0x5814000C,  //  0025  LDCONST	R5	K12
      0x8818010B,  //  0026  GETMBR	R6	R0	K11
      0x581C000D,  //  0027  LDCONST	R7	K13
      0x88200100,  //  0028  GETMBR	R8	R0	K0
      0x8820110E,  //  0029  GETMBR	R8	R8	K14
      0x7C100800,  //  002A  CALL	R4	4
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read24
********************************************************************/
be_local_closure(class_I2C_Driver_read24,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_read24,
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x58180006,  //  0004  LDCONST	R6	K6
      0x7C080800,  //  0005  CALL	R2	4
      0x940C0503,  //  0006  GETIDX	R3	R2	K3
      0x5412000F,  //  0007  LDINT	R4	16
      0x380C0604,  //  0008  SHL	R3	R3	R4
      0x94100504,  //  0009  GETIDX	R4	R2	K4
      0x54160007,  //  000A  LDINT	R5	8
      0x38100805,  //  000B  SHL	R4	R4	R5
      0x000C0604,  //  000C  ADD	R3	R3	R4
      0x94100505,  //  000D  GETIDX	R4	R2	K5
      0x000C0604,  //  000E  ADD	R3	R3	R4
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write_bit
********************************************************************/
be_local_closure(class_I2C_Driver_write_bit,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_write_bit,
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x14100503,  //  0000  LT	R4	R2	K3
      0x74120002,  //  0001  JMPT	R4	#0005
      0x54120006,  //  0002  LDINT	R4	7
      0x24100404,  //  0003  GT	R4	R2	R4
      0x78120000,  //  0004  JMPF	R4	#0006
      0x80000800,  //  0005  RET	0
      0x38120802,  //  0006  SHL	R4	K4	R2
      0x780E0007,  //  0007  JMPF	R3	#0010
      0x8C14010F,  //  0008  GETMET	R5	R0	K15
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x8C200110,  //  000A  GETMET	R8	R0	K16
      0x5C280200,  //  000B  MOVE	R10	R1
      0x7C200400,  //  000C  CALL	R8	2
      0x30201004,  //  000D  OR	R8	R8	R4
      0x7C140600,  //  000E  CALL	R5	3
      0x70020008,  //  000F  JMP		#0019
      0x8C14010F,  //  0010  GETMET	R5	R0	K15
      0x5C1C0200,  //  0011  MOVE	R7	R1
      0x8C200110,  //  0012  GETMET	R8	R0	K16
      0x5C280200,  //  0013  MOVE	R10	R1
      0x7C200400,  //  0014  CALL	R8	2
      0x542600FE,  //  0015  LDINT	R9	255
      0x04241204,  //  0016  SUB	R9	R9	R4
      0x2C201009,  //  0017  AND	R8	R8	R9
      0x7C140600,  //  0018  CALL	R5	3
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write16LE
********************************************************************/
be_local_closure(class_I2C_Driver_write16LE,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_write16LE,
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x540E00FE,  //  0000  LDINT	R3	255
      0x2C0C0403,  //  0001  AND	R3	R2	R3
      0x54120007,  //  0002  LDINT	R4	8
      0x380C0604,  //  0003  SHL	R3	R3	R4
      0x5412FEFF,  //  0004  LDINT	R4	65280
      0x2C100404,  //  0005  AND	R4	R2	R4
      0x54160007,  //  0006  LDINT	R5	8
      0x3C100805,  //  0007  SHR	R4	R4	R5
      0x300C0604,  //  0008  OR	R3	R3	R4
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C0111,  //  000A  GETMET	R3	R0	K17
      0x5C140200,  //  000B  MOVE	R5	R1
      0x5C180400,  //  000C  MOVE	R6	R2
      0x7C0C0600,  //  000D  CALL	R3	3
      0x80040600,  //  000E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read8
********************************************************************/
be_local_closure(class_I2C_Driver_read8,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_read8,
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080512,  //  0001  GETMET	R2	R2	K18
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x58180004,  //  0004  LDCONST	R6	K4
      0x7C080800,  //  0005  CALL	R2	4
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read14
********************************************************************/
be_local_closure(class_I2C_Driver_read14,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_read14,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x58180005,  //  0004  LDCONST	R6	K5
      0x7C080800,  //  0005  CALL	R2	4
      0x940C0503,  //  0006  GETIDX	R3	R2	K3
      0x54120005,  //  0007  LDINT	R4	6
      0x380C0604,  //  0008  SHL	R3	R3	R4
      0x94100504,  //  0009  GETIDX	R4	R2	K4
      0x000C0604,  //  000A  ADD	R3	R3	R4
      0x80040600,  //  000B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read16LE
********************************************************************/
be_local_closure(class_I2C_Driver_read16LE,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_read16LE,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x58180005,  //  0004  LDCONST	R6	K5
      0x7C080800,  //  0005  CALL	R2	4
      0x940C0504,  //  0006  GETIDX	R3	R2	K4
      0x54120007,  //  0007  LDINT	R4	8
      0x380C0604,  //  0008  SHL	R3	R3	R4
      0x94100503,  //  0009  GETIDX	R4	R2	K3
      0x000C0604,  //  000A  ADD	R3	R3	R4
      0x80040600,  //  000B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read12
********************************************************************/
be_local_closure(class_I2C_Driver_read12,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_read12,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x58180005,  //  0004  LDCONST	R6	K5
      0x7C080800,  //  0005  CALL	R2	4
      0x940C0503,  //  0006  GETIDX	R3	R2	K3
      0x54120003,  //  0007  LDINT	R4	4
      0x380C0604,  //  0008  SHL	R3	R3	R4
      0x94100504,  //  0009  GETIDX	R4	R2	K4
      0x000C0604,  //  000A  ADD	R3	R3	R4
      0x80040600,  //  000B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write8
********************************************************************/
be_local_closure(class_I2C_Driver_write8,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_write8,
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0713,  //  0001  GETMET	R3	R3	K19
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x58200004,  //  0005  LDCONST	R8	K4
      0x7C0C0A00,  //  0006  CALL	R3	5
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read13
********************************************************************/
be_local_closure(class_I2C_Driver_read13,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_read13,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x58180005,  //  0004  LDCONST	R6	K5
      0x7C080800,  //  0005  CALL	R2	4
      0x940C0503,  //  0006  GETIDX	R3	R2	K3
      0x54120004,  //  0007  LDINT	R4	5
      0x380C0604,  //  0008  SHL	R3	R3	R4
      0x94100504,  //  0009  GETIDX	R4	R2	K4
      0x000C0604,  //  000A  ADD	R3	R3	R4
      0x80040600,  //  000B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read16
********************************************************************/
be_local_closure(class_I2C_Driver_read16,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_read16,
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x58180005,  //  0004  LDCONST	R6	K5
      0x7C080800,  //  0005  CALL	R2	4
      0x940C0503,  //  0006  GETIDX	R3	R2	K3
      0x54120007,  //  0007  LDINT	R4	8
      0x380C0604,  //  0008  SHL	R3	R3	R4
      0x94100504,  //  0009  GETIDX	R4	R2	K4
      0x000C0604,  //  000A  ADD	R3	R3	R4
      0x80040600,  //  000B  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: write16
********************************************************************/
be_local_closure(class_I2C_Driver_write16,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_I2C_Driver,     /* shared constants */
    &be_const_str_write16,
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0713,  //  0001  GETMET	R3	R3	K19
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x5C1C0400,  //  0004  MOVE	R7	R2
      0x58200005,  //  0005  LDCONST	R8	K5
      0x7C0C0A00,  //  0006  CALL	R3	5
      0x80040600,  //  0007  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: I2C_Driver
********************************************************************/
be_local_class(I2C_Driver,
    3,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(read32, -1), be_const_closure(class_I2C_Driver_read32_closure) },
        { be_const_key(write16, 15), be_const_closure(class_I2C_Driver_write16_closure) },
        { be_const_key(addr, -1), be_const_var(1) },
        { be_const_key(init, 1), be_const_closure(class_I2C_Driver_init_closure) },
        { be_const_key(write_bit, -1), be_const_closure(class_I2C_Driver_write_bit_closure) },
        { be_const_key(read14, -1), be_const_closure(class_I2C_Driver_read14_closure) },
        { be_const_key(name, -1), be_const_var(2) },
        { be_const_key(read8, -1), be_const_closure(class_I2C_Driver_read8_closure) },
        { be_const_key(write16LE, 11), be_const_closure(class_I2C_Driver_write16LE_closure) },
        { be_const_key(read16LE, -1), be_const_closure(class_I2C_Driver_read16LE_closure) },
        { be_const_key(read12, -1), be_const_closure(class_I2C_Driver_read12_closure) },
        { be_const_key(wire, 5), be_const_var(0) },
        { be_const_key(write8, -1), be_const_closure(class_I2C_Driver_write8_closure) },
        { be_const_key(read13, -1), be_const_closure(class_I2C_Driver_read13_closure) },
        { be_const_key(read16, -1), be_const_closure(class_I2C_Driver_read16_closure) },
        { be_const_key(read24, -1), be_const_closure(class_I2C_Driver_read24_closure) },
    })),
    (bstring*) &be_const_str_I2C_Driver
);
/********************************************************************/
/* End of solidification */
