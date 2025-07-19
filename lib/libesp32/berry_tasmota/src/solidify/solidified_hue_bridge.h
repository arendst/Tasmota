/* Solidification of hue_bridge.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'hue_bridge_monad' ktab size: 50, total: 85 (saved 280 bytes)
static const bvalue be_ktab_class_hue_bridge_monad[50] = {
  /* K0   */  be_nested_str(hue_ntv),
  /* K1   */  be_nested_str(lights),
  /* K2   */  be_nested_str(contains),
  /* K3   */  be_nested_str(full_state),
  /* K4   */  be_nested_str(light),
  /* K5   */  be_nested_str(name),
  /* K6   */  be_nested_str(model),
  /* K7   */  be_nested_str(manuf),
  /* K8   */  be_nested_str(light_state),
  /* K9   */  be_nested_str(int),
  /* K10  */  be_nested_str(value_error),
  /* K11  */  be_nested_str(id_X20must_X20be_X20of_X20type_X20_X27int_X27),
  /* K12  */  be_nested_str(light_X20must_X20be_X20of_X20class_X20_X27light_state_X27),
  /* K13  */  be_const_int(0),
  /* K14  */  be_nested_str(missing_X20name),
  /* K15  */  be_nested_str(Unknown),
  /* K16  */  be_nested_str(Tasmota),
  /* K17  */  be_nested_str(remove),
  /* K18  */  be_nested_str(json),
  /* K19  */  be_nested_str(keys),
  /* K20  */  be_nested_str(full_status),
  /* K21  */  be_nested_str(_X22),
  /* K22  */  be_nested_str(_X22_X3A),
  /* K23  */  be_nested_str(push),
  /* K24  */  be_nested_str(stop_iteration),
  /* K25  */  be_nested_str(concat),
  /* K26  */  be_nested_str(_X2C),
  /* K27  */  be_nested_str(load),
  /* K28  */  be_nested_str(tasmota),
  /* K29  */  be_nested_str(log),
  /* K30  */  be_nested_str(BRY_X3A_X20invalid_X20hue_X20payload_X3A_X20),
  /* K31  */  be_const_int(3),
  /* K32  */  be_nested_str(on),
  /* K33  */  be_nested_str(set_power),
  /* K34  */  be_nested_str(xy),
  /* K35  */  be_const_int(1),
  /* K36  */  be_nested_str(set_xy),
  /* K37  */  be_nested_str(hue),
  /* K38  */  be_nested_str(sat),
  /* K39  */  be_nested_str(type),
  /* K40  */  be_nested_str(set_hue16sat),
  /* K41  */  be_nested_str(ct),
  /* K42  */  be_nested_str(set_ct),
  /* K43  */  be_nested_str(bri),
  /* K44  */  be_nested_str(set_bri),
  /* K45  */  be_nested_str(_X2Flights_X2F),
  /* K46  */  be_nested_str(_X2Fstate_X2F),
  /* K47  */  be_nested_str(success),
  /* K48  */  be_nested_str(signal_change),
  /* K49  */  be_nested_str(dump),
};


extern const bclass be_class_hue_bridge_monad;

/********************************************************************
** Solidified function: full_status
********************************************************************/
be_local_closure(class_hue_bridge_monad_full_status,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_hue_bridge_monad,     /* shared constants */
    &be_const_str_full_status,
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0009,  //  0005  JMPF	R3	#0010
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0601,  //  0007  GETIDX	R3	R3	R1
      0x8C100503,  //  0008  GETMET	R4	R2	K3
      0x5C180200,  //  0009  MOVE	R6	R1
      0x941C0704,  //  000A  GETIDX	R7	R3	K4
      0x94200705,  //  000B  GETIDX	R8	R3	K5
      0x94240706,  //  000C  GETIDX	R9	R3	K6
      0x94280707,  //  000D  GETIDX	R10	R3	K7
      0x7C100C00,  //  000E  CALL	R4	6
      0x80040800,  //  000F  RET	1	R4
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: hue_status
********************************************************************/
be_local_closure(class_hue_bridge_monad_hue_status,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_hue_bridge_monad,     /* shared constants */
    &be_const_str_hue_status,
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x780E0005,  //  0005  JMPF	R3	#000C
      0x8C0C0508,  //  0006  GETMET	R3	R2	K8
      0x88140101,  //  0007  GETMBR	R5	R0	K1
      0x94140A01,  //  0008  GETIDX	R5	R5	R1
      0x94140B04,  //  0009  GETIDX	R5	R5	K4
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80040600,  //  000B  RET	1	R3
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_light
********************************************************************/
be_local_closure(class_hue_bridge_monad_add_light,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_hue_bridge_monad,     /* shared constants */
    &be_const_str_add_light,
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x60180004,  //  0000  GETGBL	R6	G4
      0x5C1C0200,  //  0001  MOVE	R7	R1
      0x7C180200,  //  0002  CALL	R6	1
      0x20180D09,  //  0003  NE	R6	R6	K9
      0x781A0000,  //  0004  JMPF	R6	#0006
      0xB006150B,  //  0005  RAISE	1	K10	K11
      0x6018000F,  //  0006  GETGBL	R6	G15
      0x5C1C0400,  //  0007  MOVE	R7	R2
      0xB8221000,  //  0008  GETNGBL	R8	K8
      0x7C180400,  //  0009  CALL	R6	2
      0x741A0000,  //  000A  JMPT	R6	#000C
      0xB006150C,  //  000B  RAISE	1	K10	K12
      0x60180008,  //  000C  GETGBL	R6	G8
      0x5C1C0600,  //  000D  MOVE	R7	R3
      0x7C180200,  //  000E  CALL	R6	1
      0x5C0C0C00,  //  000F  MOVE	R3	R6
      0x6018000C,  //  0010  GETGBL	R6	G12
      0x5C1C0600,  //  0011  MOVE	R7	R3
      0x7C180200,  //  0012  CALL	R6	1
      0x1C180D0D,  //  0013  EQ	R6	R6	K13
      0x781A0000,  //  0014  JMPF	R6	#0016
      0xB006150E,  //  0015  RAISE	1	K10	K14
      0x5C180800,  //  0016  MOVE	R6	R4
      0x741A0000,  //  0017  JMPT	R6	#0019
      0x5810000F,  //  0018  LDCONST	R4	K15
      0x5C180A00,  //  0019  MOVE	R6	R5
      0x741A0000,  //  001A  JMPT	R6	#001C
      0x58140010,  //  001B  LDCONST	R5	K16
      0x88180101,  //  001C  GETMBR	R6	R0	K1
      0x601C0013,  //  001D  GETGBL	R7	G19
      0x7C1C0000,  //  001E  CALL	R7	0
      0x981E0802,  //  001F  SETIDX	R7	K4	R2
      0x981E0A03,  //  0020  SETIDX	R7	K5	R3
      0x60200008,  //  0021  GETGBL	R8	G8
      0x5C240800,  //  0022  MOVE	R9	R4
      0x7C200200,  //  0023  CALL	R8	1
      0x981E0C08,  //  0024  SETIDX	R7	K6	R8
      0x60200008,  //  0025  GETGBL	R8	G8
      0x5C240A00,  //  0026  MOVE	R9	R5
      0x7C200200,  //  0027  CALL	R8	1
      0x981E0E08,  //  0028  SETIDX	R7	K7	R8
      0x98180207,  //  0029  SETIDX	R6	R1	R7
      0x80040400,  //  002A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_light
********************************************************************/
be_local_closure(class_hue_bridge_monad_remove_light,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_hue_bridge_monad,     /* shared constants */
    &be_const_str_remove_light,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x8C080511,  //  0001  GETMET	R2	R2	K17
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_hue_bridge_monad_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_hue_bridge_monad,     /* shared constants */
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x90020201,  //  0002  SETMBR	R0	K1	R1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: discover
********************************************************************/
be_local_closure(class_hue_bridge_monad_discover,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_hue_bridge_monad,     /* shared constants */
    &be_const_str_discover,
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A2400,  //  0001  IMPORT	R2	K18
      0x600C0012,  //  0002  GETGBL	R3	G18
      0x7C0C0000,  //  0003  CALL	R3	0
      0x60100010,  //  0004  GETGBL	R4	G16
      0x88140101,  //  0005  GETMBR	R5	R0	K1
      0x8C140B13,  //  0006  GETMET	R5	R5	K19
      0x7C140200,  //  0007  CALL	R5	1
      0x7C100200,  //  0008  CALL	R4	1
      0xA802000F,  //  0009  EXBLK	0	#001A
      0x5C140800,  //  000A  MOVE	R5	R4
      0x7C140000,  //  000B  CALL	R5	0
      0x8C180114,  //  000C  GETMET	R6	R0	K20
      0x5C200A00,  //  000D  MOVE	R8	R5
      0x7C180400,  //  000E  CALL	R6	2
      0x781A0008,  //  000F  JMPF	R6	#0019
      0x601C0008,  //  0010  GETGBL	R7	G8
      0x5C200A00,  //  0011  MOVE	R8	R5
      0x7C1C0200,  //  0012  CALL	R7	1
      0x001E2A07,  //  0013  ADD	R7	K21	R7
      0x001C0F16,  //  0014  ADD	R7	R7	K22
      0x00180E06,  //  0015  ADD	R6	R7	R6
      0x8C1C0717,  //  0016  GETMET	R7	R3	K23
      0x5C240C00,  //  0017  MOVE	R9	R6
      0x7C1C0400,  //  0018  CALL	R7	2
      0x7001FFEF,  //  0019  JMP		#000A
      0x58100018,  //  001A  LDCONST	R4	K24
      0xAC100200,  //  001B  CATCH	R4	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x6010000C,  //  001D  GETGBL	R4	G12
      0x5C140600,  //  001E  MOVE	R5	R3
      0x7C100200,  //  001F  CALL	R4	1
      0x2410090D,  //  0020  GT	R4	R4	K13
      0x78120003,  //  0021  JMPF	R4	#0026
      0x8C100719,  //  0022  GETMET	R4	R3	K25
      0x5818001A,  //  0023  LDCONST	R6	K26
      0x7C100400,  //  0024  CALL	R4	2
      0x80040800,  //  0025  RET	1	R4
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: light_to_id
********************************************************************/
be_local_closure(class_hue_bridge_monad_light_to_id,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_hue_bridge_monad,     /* shared constants */
    &be_const_str_light_to_id,
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x60080010,  //  0000  GETGBL	R2	G16
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x8C0C0713,  //  0002  GETMET	R3	R3	K19
      0x7C0C0200,  //  0003  CALL	R3	1
      0x7C080200,  //  0004  CALL	R2	1
      0xA8020009,  //  0005  EXBLK	0	#0010
      0x5C0C0400,  //  0006  MOVE	R3	R2
      0x7C0C0000,  //  0007  CALL	R3	0
      0x88100101,  //  0008  GETMBR	R4	R0	K1
      0x94100803,  //  0009  GETIDX	R4	R4	R3
      0x94100904,  //  000A  GETIDX	R4	R4	K4
      0x1C100204,  //  000B  EQ	R4	R1	R4
      0x78120001,  //  000C  JMPF	R4	#000F
      0xA8040001,  //  000D  EXBLK	1	1
      0x80040600,  //  000E  RET	1	R3
      0x7001FFF5,  //  000F  JMP		#0006
      0x58080018,  //  0010  LDCONST	R2	K24
      0xAC080200,  //  0011  CATCH	R2	1	0
      0xB0080000,  //  0012  RAISE	2	R0	R0
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: cmd
********************************************************************/
be_local_closure(class_hue_bridge_monad_cmd,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_hue_bridge_monad,     /* shared constants */
    &be_const_str_cmd,
    &be_const_str_solidified,
    ( &(const binstruction[167]) {  /* code */
      0xA40E2400,  //  0000  IMPORT	R3	K18
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x74120000,  //  0005  JMPT	R4	#0007
      0x80000800,  //  0006  RET	0
      0x88100101,  //  0007  GETMBR	R4	R0	K1
      0x94100801,  //  0008  GETIDX	R4	R4	R1
      0x94100904,  //  0009  GETIDX	R4	R4	K4
      0x60140013,  //  000A  GETGBL	R5	G19
      0x7C140000,  //  000B  CALL	R5	0
      0x8C18071B,  //  000C  GETMET	R6	R3	K27
      0x5C200400,  //  000D  MOVE	R8	R2
      0x7C180400,  //  000E  CALL	R6	2
      0x4C1C0000,  //  000F  LDNIL	R7
      0x1C1C0407,  //  0010  EQ	R7	R2	R7
      0x781E0008,  //  0011  JMPF	R7	#001B
      0xB81E3800,  //  0012  GETNGBL	R7	K28
      0x8C1C0F1D,  //  0013  GETMET	R7	R7	K29
      0x60240008,  //  0014  GETGBL	R9	G8
      0x5C280400,  //  0015  MOVE	R10	R2
      0x7C240200,  //  0016  CALL	R9	1
      0x00263C09,  //  0017  ADD	R9	K30	R9
      0x5828001F,  //  0018  LDCONST	R10	K31
      0x7C1C0600,  //  0019  CALL	R7	3
      0x80000E00,  //  001A  RET	0
      0x8C1C0D02,  //  001B  GETMET	R7	R6	K2
      0x58240020,  //  001C  LDCONST	R9	K32
      0x7C1C0400,  //  001D  CALL	R7	2
      0x781E0006,  //  001E  JMPF	R7	#0026
      0x601C0017,  //  001F  GETGBL	R7	G23
      0x94200D20,  //  0020  GETIDX	R8	R6	K32
      0x7C1C0200,  //  0021  CALL	R7	1
      0x98164007,  //  0022  SETIDX	R5	K32	R7
      0x8C200921,  //  0023  GETMET	R8	R4	K33
      0x5C280E00,  //  0024  MOVE	R10	R7
      0x7C200400,  //  0025  CALL	R8	2
      0x8C1C0D02,  //  0026  GETMET	R7	R6	K2
      0x58240022,  //  0027  LDCONST	R9	K34
      0x7C1C0400,  //  0028  CALL	R7	2
      0x781E0013,  //  0029  JMPF	R7	#003E
      0x941C0D22,  //  002A  GETIDX	R7	R6	K34
      0x6020000A,  //  002B  GETGBL	R8	G10
      0x94240F0D,  //  002C  GETIDX	R9	R7	K13
      0x7C200200,  //  002D  CALL	R8	1
      0x6024000A,  //  002E  GETGBL	R9	G10
      0x94280F23,  //  002F  GETIDX	R10	R7	K35
      0x7C240200,  //  0030  CALL	R9	1
      0x60280012,  //  0031  GETGBL	R10	G18
      0x7C280000,  //  0032  CALL	R10	0
      0x402C1408,  //  0033  CONNECT	R11	R10	R8
      0x402C1409,  //  0034  CONNECT	R11	R10	R9
      0x9816440A,  //  0035  SETIDX	R5	K34	R10
      0x542AFFFF,  //  0036  LDINT	R10	65536
      0x0820100A,  //  0037  MUL	R8	R8	R10
      0x542AFFFF,  //  0038  LDINT	R10	65536
      0x0824120A,  //  0039  MUL	R9	R9	R10
      0x8C280924,  //  003A  GETMET	R10	R4	K36
      0x5C301000,  //  003B  MOVE	R12	R8
      0x5C341200,  //  003C  MOVE	R13	R9
      0x7C280600,  //  003D  CALL	R10	3
      0x501C0000,  //  003E  LDBOOL	R7	0	0
      0x88200925,  //  003F  GETMBR	R8	R4	K37
      0x88240926,  //  0040  GETMBR	R9	R4	K38
      0x8C280D02,  //  0041  GETMET	R10	R6	K2
      0x58300025,  //  0042  LDCONST	R12	K37
      0x7C280400,  //  0043  CALL	R10	2
      0x782A0009,  //  0044  JMPF	R10	#004F
      0x60280009,  //  0045  GETGBL	R10	G9
      0x942C0D25,  //  0046  GETIDX	R11	R6	K37
      0x7C280200,  //  0047  CALL	R10	1
      0x5C201400,  //  0048  MOVE	R8	R10
      0x98164A08,  //  0049  SETIDX	R5	K37	R8
      0x542AFFFE,  //  004A  LDINT	R10	65535
      0x2428100A,  //  004B  GT	R10	R8	R10
      0x782A0000,  //  004C  JMPF	R10	#004E
      0x5422FFFE,  //  004D  LDINT	R8	65535
      0x501C0200,  //  004E  LDBOOL	R7	1	0
      0x8C280D02,  //  004F  GETMET	R10	R6	K2
      0x58300026,  //  0050  LDCONST	R12	K38
      0x7C280400,  //  0051  CALL	R10	2
      0x782A000C,  //  0052  JMPF	R10	#0060
      0x60280009,  //  0053  GETGBL	R10	G9
      0x942C0D26,  //  0054  GETIDX	R11	R6	K38
      0x7C280200,  //  0055  CALL	R10	1
      0x5C241400,  //  0056  MOVE	R9	R10
      0x98164C09,  //  0057  SETIDX	R5	K38	R9
      0x542A00FD,  //  0058  LDINT	R10	254
      0x2828120A,  //  0059  GE	R10	R9	R10
      0x782A0000,  //  005A  JMPF	R10	#005C
      0x542600FE,  //  005B  LDINT	R9	255
      0x88280927,  //  005C  GETMBR	R10	R4	K39
      0x2828151F,  //  005D  GE	R10	R10	K31
      0x782A0000,  //  005E  JMPF	R10	#0060
      0x501C0200,  //  005F  LDBOOL	R7	1	0
      0x781E0003,  //  0060  JMPF	R7	#0065
      0x8C280928,  //  0061  GETMET	R10	R4	K40
      0x5C301000,  //  0062  MOVE	R12	R8
      0x5C341200,  //  0063  MOVE	R13	R9
      0x7C280600,  //  0064  CALL	R10	3
      0x8C280D02,  //  0065  GETMET	R10	R6	K2
      0x58300029,  //  0066  LDCONST	R12	K41
      0x7C280400,  //  0067  CALL	R10	2
      0x782A0006,  //  0068  JMPF	R10	#0070
      0x60280009,  //  0069  GETGBL	R10	G9
      0x942C0D29,  //  006A  GETIDX	R11	R6	K41
      0x7C280200,  //  006B  CALL	R10	1
      0x9816520A,  //  006C  SETIDX	R5	K41	R10
      0x8C2C092A,  //  006D  GETMET	R11	R4	K42
      0x5C341400,  //  006E  MOVE	R13	R10
      0x7C2C0400,  //  006F  CALL	R11	2
      0x8C280D02,  //  0070  GETMET	R10	R6	K2
      0x5830002B,  //  0071  LDCONST	R12	K43
      0x7C280400,  //  0072  CALL	R10	2
      0x782A000A,  //  0073  JMPF	R10	#007F
      0x60280009,  //  0074  GETGBL	R10	G9
      0x942C0D2B,  //  0075  GETIDX	R11	R6	K43
      0x7C280200,  //  0076  CALL	R10	1
      0x9816560A,  //  0077  SETIDX	R5	K43	R10
      0x542E00FD,  //  0078  LDINT	R11	254
      0x282C140B,  //  0079  GE	R11	R10	R11
      0x782E0000,  //  007A  JMPF	R11	#007C
      0x542A00FE,  //  007B  LDINT	R10	255
      0x8C2C092C,  //  007C  GETMET	R11	R4	K44
      0x5C341400,  //  007D  MOVE	R13	R10
      0x7C2C0400,  //  007E  CALL	R11	2
      0x60280012,  //  007F  GETGBL	R10	G18
      0x7C280000,  //  0080  CALL	R10	0
      0x602C0010,  //  0081  GETGBL	R11	G16
      0x8C300B13,  //  0082  GETMET	R12	R5	K19
      0x7C300200,  //  0083  CALL	R12	1
      0x7C2C0200,  //  0084  CALL	R11	1
      0xA8020012,  //  0085  EXBLK	0	#0099
      0x5C301600,  //  0086  MOVE	R12	R11
      0x7C300000,  //  0087  CALL	R12	0
      0x60340008,  //  0088  GETGBL	R13	G8
      0x5C380200,  //  0089  MOVE	R14	R1
      0x7C340200,  //  008A  CALL	R13	1
      0x00365A0D,  //  008B  ADD	R13	K45	R13
      0x00341B2E,  //  008C  ADD	R13	R13	K46
      0x00341A0C,  //  008D  ADD	R13	R13	R12
      0x60380013,  //  008E  GETGBL	R14	G19
      0x7C380000,  //  008F  CALL	R14	0
      0x603C0013,  //  0090  GETGBL	R15	G19
      0x7C3C0000,  //  0091  CALL	R15	0
      0x94400A0C,  //  0092  GETIDX	R16	R5	R12
      0x983C1A10,  //  0093  SETIDX	R15	R13	R16
      0x983A5E0F,  //  0094  SETIDX	R14	K47	R15
      0x8C3C1517,  //  0095  GETMET	R15	R10	K23
      0x5C441C00,  //  0096  MOVE	R17	R14
      0x7C3C0400,  //  0097  CALL	R15	2
      0x7001FFEC,  //  0098  JMP		#0086
      0x582C0018,  //  0099  LDCONST	R11	K24
      0xAC2C0200,  //  009A  CATCH	R11	1	0
      0xB0080000,  //  009B  RAISE	2	R0	R0
      0x602C000C,  //  009C  GETGBL	R11	G12
      0x5C301400,  //  009D  MOVE	R12	R10
      0x7C2C0200,  //  009E  CALL	R11	1
      0x242C170D,  //  009F  GT	R11	R11	K13
      0x782E0001,  //  00A0  JMPF	R11	#00A3
      0x8C2C0930,  //  00A1  GETMET	R11	R4	K48
      0x7C2C0200,  //  00A2  CALL	R11	1
      0x8C2C0731,  //  00A3  GETMET	R11	R3	K49
      0x5C341400,  //  00A4  MOVE	R13	R10
      0x7C2C0400,  //  00A5  CALL	R11	2
      0x80041600,  //  00A6  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: groups
********************************************************************/
be_local_closure(class_hue_bridge_monad_groups,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_hue_bridge_monad,     /* shared constants */
    &be_const_str_groups,
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x60040012,  //  0000  GETGBL	R1	G18
      0x7C040000,  //  0001  CALL	R1	0
      0x60080010,  //  0002  GETGBL	R2	G16
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x8C0C0713,  //  0004  GETMET	R3	R3	K19
      0x7C0C0200,  //  0005  CALL	R3	1
      0x7C080200,  //  0006  CALL	R2	1
      0xA8020009,  //  0007  EXBLK	0	#0012
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x7C0C0000,  //  0009  CALL	R3	0
      0x8C100317,  //  000A  GETMET	R4	R1	K23
      0x60180008,  //  000B  GETGBL	R6	G8
      0x5C1C0600,  //  000C  MOVE	R7	R3
      0x7C180200,  //  000D  CALL	R6	1
      0x001A2A06,  //  000E  ADD	R6	K21	R6
      0x00180D15,  //  000F  ADD	R6	R6	K21
      0x7C100400,  //  0010  CALL	R4	2
      0x7001FFF5,  //  0011  JMP		#0008
      0x58080018,  //  0012  LDCONST	R2	K24
      0xAC080200,  //  0013  CATCH	R2	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x6008000C,  //  0015  GETGBL	R2	G12
      0x5C0C0200,  //  0016  MOVE	R3	R1
      0x7C080200,  //  0017  CALL	R2	1
      0x2408050D,  //  0018  GT	R2	R2	K13
      0x780A0003,  //  0019  JMPF	R2	#001E
      0x8C080319,  //  001A  GETMET	R2	R1	K25
      0x5810001A,  //  001B  LDCONST	R4	K26
      0x7C080400,  //  001C  CALL	R2	2
      0x80040400,  //  001D  RET	1	R2
      0x80000000,  //  001E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: hue_bridge_monad
********************************************************************/
be_local_class(hue_bridge_monad,
    1,
    NULL,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(lights, -1), be_const_var(0) },
        { be_const_key(groups, 6), be_const_closure(class_hue_bridge_monad_groups_closure) },
        { be_const_key(hue_status, -1), be_const_closure(class_hue_bridge_monad_hue_status_closure) },
        { be_const_key(add_light, -1), be_const_closure(class_hue_bridge_monad_add_light_closure) },
        { be_const_key(remove_light, 8), be_const_closure(class_hue_bridge_monad_remove_light_closure) },
        { be_const_key(init, -1), be_const_closure(class_hue_bridge_monad_init_closure) },
        { be_const_key(cmd, -1), be_const_closure(class_hue_bridge_monad_cmd_closure) },
        { be_const_key(light_to_id, -1), be_const_closure(class_hue_bridge_monad_light_to_id_closure) },
        { be_const_key(discover, -1), be_const_closure(class_hue_bridge_monad_discover_closure) },
        { be_const_key(full_status, 1), be_const_closure(class_hue_bridge_monad_full_status_closure) },
    })),
    (bstring*) &be_const_str_hue_bridge_monad
);

/********************************************************************
** Solidified function: _anonymous_
********************************************************************/
be_local_closure(_anonymous_,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_const_class(be_class_hue_bridge_monad),
    }),
    &be_const_str__anonymous_,
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xB4000000,  //  0001  CLASS	K0
      0x5C080200,  //  0002  MOVE	R2	R1
      0x7C080000,  //  0003  CALL	R2	0
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified module: hue_bridge
********************************************************************/
be_local_module(hue_bridge,
    "hue_bridge",
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(init, -1), be_const_closure(_anonymous__closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(hue_bridge);
/********************************************************************/
/********************************************************************/
/* End of solidification */
