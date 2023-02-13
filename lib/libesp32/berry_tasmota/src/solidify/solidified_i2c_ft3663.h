/* Solidification of i2c_ft3663.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_FT3663;

/********************************************************************
** Solidified function: every_100ms
********************************************************************/
be_local_closure(FT3663_every_100ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str(wire),
    /* K1   */  be_nested_str(ts_loop),
    }),
    &be_const_str_every_100ms,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060001,  //  0001  JMPF	R1	#0004
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(FT3663_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str(init),
    /* K1   */  be_nested_str(FT3663),
    /* K2   */  be_nested_str(wire),
    /* K3   */  be_nested_str(read8),
    /* K4   */  be_nested_str(tasmota),
    /* K5   */  be_nested_str(log),
    /* K6   */  be_nested_str(I2C_X3A_X20ignoring_X20address_X200x38_X2C_X20not_X20FT3663),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str(TS_X20_X3A_X20FT3663_X20Touch_X20Screen_X20detected),
    /* K9   */  be_nested_str(write8),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str(add_driver),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x580C0001,  //  0004  LDCONST	R3	K1
      0x54120037,  //  0005  LDINT	R4	56
      0x7C040600,  //  0006  CALL	R1	3
      0x88040102,  //  0007  GETMBR	R1	R0	K2
      0x78060027,  //  0008  JMPF	R1	#0031
      0x8C040103,  //  0009  GETMET	R1	R0	K3
      0x540E00A7,  //  000A  LDINT	R3	168
      0x7C040400,  //  000B  CALL	R1	2
      0x8C080103,  //  000C  GETMET	R2	R0	K3
      0x541200A2,  //  000D  LDINT	R4	163
      0x7C080400,  //  000E  CALL	R2	2
      0x540E0010,  //  000F  LDINT	R3	17
      0x200C0203,  //  0010  NE	R3	R1	R3
      0x740E0002,  //  0011  JMPT	R3	#0015
      0x540E0063,  //  0012  LDINT	R3	100
      0x200C0403,  //  0013  NE	R3	R2	R3
      0x780E0007,  //  0014  JMPF	R3	#001D
      0xB80E0800,  //  0015  GETNGBL	R3	K4
      0x8C0C0705,  //  0016  GETMET	R3	R3	K5
      0x58140006,  //  0017  LDCONST	R5	K6
      0x58180007,  //  0018  LDCONST	R6	K7
      0x7C0C0600,  //  0019  CALL	R3	3
      0x4C0C0000,  //  001A  LDNIL	R3
      0x90020403,  //  001B  SETMBR	R0	K2	R3
      0x80000600,  //  001C  RET	0
      0xB80E0800,  //  001D  GETNGBL	R3	K4
      0x8C0C0705,  //  001E  GETMET	R3	R3	K5
      0x58140008,  //  001F  LDCONST	R5	K8
      0x7C0C0400,  //  0020  CALL	R3	2
      0x8C0C0109,  //  0021  GETMET	R3	R0	K9
      0x5814000A,  //  0022  LDCONST	R5	K10
      0x5818000A,  //  0023  LDCONST	R6	K10
      0x7C0C0600,  //  0024  CALL	R3	3
      0x8C0C0109,  //  0025  GETMET	R3	R0	K9
      0x5416007F,  //  0026  LDINT	R5	128
      0x541A0015,  //  0027  LDINT	R6	22
      0x7C0C0600,  //  0028  CALL	R3	3
      0x8C0C0109,  //  0029  GETMET	R3	R0	K9
      0x54160087,  //  002A  LDINT	R5	136
      0x541A000D,  //  002B  LDINT	R6	14
      0x7C0C0600,  //  002C  CALL	R3	3
      0xB80E0800,  //  002D  GETNGBL	R3	K4
      0x8C0C070B,  //  002E  GETMET	R3	R3	K11
      0x5C140000,  //  002F  MOVE	R5	R0
      0x7C0C0400,  //  0030  CALL	R3	2
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ts_loop
********************************************************************/
be_local_closure(FT3663_ts_loop,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str(wire),
    /* K1   */  be_nested_str(display),
    /* K2   */  be_nested_str(read_bytes),
    /* K3   */  be_nested_str(addr),
    /* K4   */  be_const_int(0),
    /* K5   */  be_const_int(2),
    /* K6   */  be_const_int(3),
    /* K7   */  be_nested_str(gest_id_codes),
    /* K8   */  be_nested_str(find),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str(touch_update),
    }),
    &be_const_str_ts_loop,
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x74060000,  //  0001  JMPT	R1	#0003
      0x80000200,  //  0002  RET	0
      0xA4060200,  //  0003  IMPORT	R1	K1
      0x88080100,  //  0004  GETMBR	R2	R0	K0
      0x8C080502,  //  0005  GETMET	R2	R2	K2
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x58140004,  //  0007  LDCONST	R5	K4
      0x541A000E,  //  0008  LDINT	R6	15
      0x7C080800,  //  0009  CALL	R2	4
      0x940C0505,  //  000A  GETIDX	R3	R2	K5
      0x5412000E,  //  000B  LDINT	R4	15
      0x2C0C0604,  //  000C  AND	R3	R3	R4
      0x94100506,  //  000D  GETIDX	R4	R2	K6
      0x5416000E,  //  000E  LDINT	R5	15
      0x2C100805,  //  000F  AND	R4	R4	R5
      0x54160007,  //  0010  LDINT	R5	8
      0x38100805,  //  0011  SHL	R4	R4	R5
      0x54160003,  //  0012  LDINT	R5	4
      0x94140405,  //  0013  GETIDX	R5	R2	R5
      0x30100805,  //  0014  OR	R4	R4	R5
      0x54160004,  //  0015  LDINT	R5	5
      0x94140405,  //  0016  GETIDX	R5	R2	R5
      0x541A000E,  //  0017  LDINT	R6	15
      0x2C140A06,  //  0018  AND	R5	R5	R6
      0x541A0007,  //  0019  LDINT	R6	8
      0x38140A06,  //  001A  SHL	R5	R5	R6
      0x541A0005,  //  001B  LDINT	R6	6
      0x94180406,  //  001C  GETIDX	R6	R2	R6
      0x30140A06,  //  001D  OR	R5	R5	R6
      0x541A0006,  //  001E  LDINT	R6	7
      0x94180406,  //  001F  GETIDX	R6	R2	R6
      0x941C0506,  //  0020  GETIDX	R7	R2	K6
      0x54220008,  //  0021  LDINT	R8	9
      0x2C1C0E08,  //  0022  AND	R7	R7	R8
      0x54220007,  //  0023  LDINT	R8	8
      0x381C0E08,  //  0024  SHL	R7	R7	R8
      0x54220009,  //  0025  LDINT	R8	10
      0x94200408,  //  0026  GETIDX	R8	R2	R8
      0x301C0E08,  //  0027  OR	R7	R7	R8
      0x54220004,  //  0028  LDINT	R8	5
      0x94200408,  //  0029  GETIDX	R8	R2	R8
      0x5426000A,  //  002A  LDINT	R9	11
      0x2C201009,  //  002B  AND	R8	R8	R9
      0x54260007,  //  002C  LDINT	R9	8
      0x38201009,  //  002D  SHL	R8	R8	R9
      0x5426000B,  //  002E  LDINT	R9	12
      0x94240409,  //  002F  GETIDX	R9	R2	R9
      0x30201009,  //  0030  OR	R8	R8	R9
      0x5426000C,  //  0031  LDINT	R9	13
      0x94240409,  //  0032  GETIDX	R9	R2	R9
      0x88280107,  //  0033  GETMBR	R10	R0	K7
      0x8C281508,  //  0034  GETMET	R10	R10	K8
      0x94300509,  //  0035  GETIDX	R12	R2	K9
      0x58340004,  //  0036  LDCONST	R13	K4
      0x7C280600,  //  0037  CALL	R10	3
      0x8C2C030A,  //  0038  GETMET	R11	R1	K10
      0x5C340600,  //  0039  MOVE	R13	R3
      0x5C380800,  //  003A  MOVE	R14	R4
      0x5C3C0A00,  //  003B  MOVE	R15	R5
      0x5C401400,  //  003C  MOVE	R16	R10
      0x7C2C0A00,  //  003D  CALL	R11	5
      0x80000000,  //  003E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: FT3663
********************************************************************/
extern const bclass be_class_I2C_Driver;
be_local_class(FT3663,
    0,
    &be_class_I2C_Driver,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(every_100ms, -1), be_const_closure(FT3663_every_100ms_closure) },
        { be_const_key(ts_loop, 2), be_const_closure(FT3663_ts_loop_closure) },
        { be_const_key(gest_id_codes, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(0, 4), be_const_int(0) },
        { be_const_key_int(72, -1), be_const_int(32) },
        { be_const_key_int(16, 1), be_const_int(16) },
        { be_const_key_int(73, 5), be_const_int(33) },
        { be_const_key_int(28, -1), be_const_int(18) },
        { be_const_key_int(24, -1), be_const_int(17) },
        { be_const_key_int(20, -1), be_const_int(19) },
    }))    ) } )) },
        { be_const_key(init, 1), be_const_closure(FT3663_init_closure) },
    })),
    (bstring*) &be_const_str_FT3663
);
/*******************************************************************/

void be_load_FT3663_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_FT3663);
    be_setglobal(vm, "FT3663");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
