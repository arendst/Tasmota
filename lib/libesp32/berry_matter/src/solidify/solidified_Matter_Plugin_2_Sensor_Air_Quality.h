/* Solidification of Matter_Plugin_2_Sensor_Air_Quality.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_Air_Quality' ktab size: 64, total: 134 (saved 560 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_Air_Quality[64] = {
  /* K0   */  be_nested_str_weak(prefix_co2),
  /* K1   */  be_nested_str_weak(shadow_co2),
  /* K2   */  be_nested_str_weak(_parse_sensor_filter),
  /* K3   */  be_nested_str_weak(CarbonDioxide),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(prefix_no2),
  /* K6   */  be_nested_str_weak(shadow_no2),
  /* K7   */  be_nested_str_weak(NO2),
  /* K8   */  be_nested_str_weak(prefix_pm1),
  /* K9   */  be_nested_str_weak(shadow_pm1),
  /* K10  */  be_nested_str_weak(PM1),
  /* K11  */  be_nested_str_weak(prefix_pm2_5),
  /* K12  */  be_nested_str_weak(shadow_pm2_5),
  /* K13  */  be_nested_str_weak(PM2_X2E5),
  /* K14  */  be_nested_str_weak(prefix_pm10),
  /* K15  */  be_nested_str_weak(shadow_pm10),
  /* K16  */  be_nested_str_weak(PM10),
  /* K17  */  be_nested_str_weak(prefix_tvoc),
  /* K18  */  be_nested_str_weak(shadow_tvoc),
  /* K19  */  be_nested_str_weak(TVOC),
  /* K20  */  be_nested_str_weak(prefix_airquality),
  /* K21  */  be_nested_str_weak(find),
  /* K22  */  be_nested_str_weak(contains),
  /* K23  */  be_nested_str_weak(AirQuality),
  /* K24  */  be_nested_str_weak(shadow_air_quality),
  /* K25  */  be_nested_str_weak(_parse_sensor_entry),
  /* K26  */  be_nested_str_weak(compute_air_quality),
  /* K27  */  be_nested_str_weak(parse_sensors),
  /* K28  */  be_nested_str_weak(clusters_derived),
  /* K29  */  be_nested_str_weak(CLUSTERS),
  /* K30  */  be_const_int(1),
  /* K31  */  be_const_int(2),
  /* K32  */  be_const_int(3),
  /* K33  */  be_nested_str_weak(attribute_updated),
  /* K34  */  be_nested_str_weak(parse_configuration),
  /* K35  */  be_nested_str_weak(airquality),
  /* K36  */  be_nested_str_weak(co2),
  /* K37  */  be_nested_str_weak(no2),
  /* K38  */  be_nested_str_weak(pm1),
  /* K39  */  be_nested_str_weak(pm2_5),
  /* K40  */  be_nested_str_weak(pm10),
  /* K41  */  be_nested_str_weak(tvoc),
  /* K42  */  be_nested_str_weak(_build_clusters),
  /* K43  */  be_nested_str_weak(keys),
  /* K44  */  be_nested_str_weak(stop_iteration),
  /* K45  */  be_nested_str_weak(remove),
  /* K46  */  be_nested_str_weak(init),
  /* K47  */  be_nested_str_weak(add_read_sensors_schedule),
  /* K48  */  be_nested_str_weak(UPDATE_TIME),
  /* K49  */  be_nested_str_weak(_parse_update_virtual),
  /* K50  */  be_nested_str_weak(CO2),
  /* K51  */  be_nested_str_weak(update_virtual),
  /* K52  */  be_nested_str_weak(matter),
  /* K53  */  be_nested_str_weak(TLV),
  /* K54  */  be_nested_str_weak(cluster),
  /* K55  */  be_nested_str_weak(attribute),
  /* K56  */  be_nested_str_weak(set_or_nil),
  /* K57  */  be_nested_str_weak(U1),
  /* K58  */  be_nested_str_weak(read_attribute),
  /* K59  */  be_nested_str_weak(string),
  /* K60  */  be_nested_str_weak(_X23),
  /* K61  */  be_const_int(2147483647),
  /* K62  */  be_nested_str_weak(web_values_prefix),
  /* K63  */  be_nested_str_weak(Air),
};


extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;

/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_sensors,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[106]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0009,  //  0001  JMPF	R2	#000C
      0x8C080102,  //  0002  GETMET	R2	R0	K2
      0x5C100200,  //  0003  MOVE	R4	R1
      0x88140100,  //  0004  GETMBR	R5	R0	K0
      0x58180003,  //  0005  LDCONST	R6	K3
      0x881C0101,  //  0006  GETMBR	R7	R0	K1
      0x60200007,  //  0007  GETGBL	R8	G7
      0x5426040C,  //  0008  LDINT	R9	1037
      0x58280004,  //  0009  LDCONST	R10	K4
      0x7C081000,  //  000A  CALL	R2	8
      0x90020202,  //  000B  SETMBR	R0	K1	R2
      0x88080105,  //  000C  GETMBR	R2	R0	K5
      0x780A0009,  //  000D  JMPF	R2	#0018
      0x8C080102,  //  000E  GETMET	R2	R0	K2
      0x5C100200,  //  000F  MOVE	R4	R1
      0x88140105,  //  0010  GETMBR	R5	R0	K5
      0x58180007,  //  0011  LDCONST	R6	K7
      0x881C0106,  //  0012  GETMBR	R7	R0	K6
      0x60200007,  //  0013  GETGBL	R8	G7
      0x54260412,  //  0014  LDINT	R9	1043
      0x58280004,  //  0015  LDCONST	R10	K4
      0x7C081000,  //  0016  CALL	R2	8
      0x90020C02,  //  0017  SETMBR	R0	K6	R2
      0x88080108,  //  0018  GETMBR	R2	R0	K8
      0x780A0009,  //  0019  JMPF	R2	#0024
      0x8C080102,  //  001A  GETMET	R2	R0	K2
      0x5C100200,  //  001B  MOVE	R4	R1
      0x88140108,  //  001C  GETMBR	R5	R0	K8
      0x5818000A,  //  001D  LDCONST	R6	K10
      0x881C0109,  //  001E  GETMBR	R7	R0	K9
      0x60200007,  //  001F  GETGBL	R8	G7
      0x5426042B,  //  0020  LDINT	R9	1068
      0x58280004,  //  0021  LDCONST	R10	K4
      0x7C081000,  //  0022  CALL	R2	8
      0x90021202,  //  0023  SETMBR	R0	K9	R2
      0x8808010B,  //  0024  GETMBR	R2	R0	K11
      0x780A0009,  //  0025  JMPF	R2	#0030
      0x8C080102,  //  0026  GETMET	R2	R0	K2
      0x5C100200,  //  0027  MOVE	R4	R1
      0x8814010B,  //  0028  GETMBR	R5	R0	K11
      0x5818000D,  //  0029  LDCONST	R6	K13
      0x881C010C,  //  002A  GETMBR	R7	R0	K12
      0x60200007,  //  002B  GETGBL	R8	G7
      0x54260429,  //  002C  LDINT	R9	1066
      0x58280004,  //  002D  LDCONST	R10	K4
      0x7C081000,  //  002E  CALL	R2	8
      0x90021802,  //  002F  SETMBR	R0	K12	R2
      0x8808010E,  //  0030  GETMBR	R2	R0	K14
      0x780A0009,  //  0031  JMPF	R2	#003C
      0x8C080102,  //  0032  GETMET	R2	R0	K2
      0x5C100200,  //  0033  MOVE	R4	R1
      0x8814010E,  //  0034  GETMBR	R5	R0	K14
      0x58180010,  //  0035  LDCONST	R6	K16
      0x881C010F,  //  0036  GETMBR	R7	R0	K15
      0x60200007,  //  0037  GETGBL	R8	G7
      0x5426042C,  //  0038  LDINT	R9	1069
      0x58280004,  //  0039  LDCONST	R10	K4
      0x7C081000,  //  003A  CALL	R2	8
      0x90021E02,  //  003B  SETMBR	R0	K15	R2
      0x88080111,  //  003C  GETMBR	R2	R0	K17
      0x780A0009,  //  003D  JMPF	R2	#0048
      0x8C080102,  //  003E  GETMET	R2	R0	K2
      0x5C100200,  //  003F  MOVE	R4	R1
      0x88140111,  //  0040  GETMBR	R5	R0	K17
      0x58180013,  //  0041  LDCONST	R6	K19
      0x881C0112,  //  0042  GETMBR	R7	R0	K18
      0x60200007,  //  0043  GETGBL	R8	G7
      0x5426042D,  //  0044  LDINT	R9	1070
      0x58280004,  //  0045  LDCONST	R10	K4
      0x7C081000,  //  0046  CALL	R2	8
      0x90022402,  //  0047  SETMBR	R0	K18	R2
      0x88080114,  //  0048  GETMBR	R2	R0	K20
      0x780A0016,  //  0049  JMPF	R2	#0061
      0x8C080315,  //  004A  GETMET	R2	R1	K21
      0x88100114,  //  004B  GETMBR	R4	R0	K20
      0x7C080400,  //  004C  CALL	R2	2
      0x4C0C0000,  //  004D  LDNIL	R3
      0x200C0403,  //  004E  NE	R3	R2	R3
      0x780E000D,  //  004F  JMPF	R3	#005E
      0x8C0C0516,  //  0050  GETMET	R3	R2	K22
      0x58140017,  //  0051  LDCONST	R5	K23
      0x7C0C0400,  //  0052  CALL	R3	2
      0x780E0009,  //  0053  JMPF	R3	#005E
      0x8C0C0119,  //  0054  GETMET	R3	R0	K25
      0x5C140400,  //  0055  MOVE	R5	R2
      0x58180017,  //  0056  LDCONST	R6	K23
      0x881C0118,  //  0057  GETMBR	R7	R0	K24
      0x60200007,  //  0058  GETGBL	R8	G7
      0x5426005A,  //  0059  LDINT	R9	91
      0x58280004,  //  005A  LDCONST	R10	K4
      0x7C0C0E00,  //  005B  CALL	R3	7
      0x90023003,  //  005C  SETMBR	R0	K24	R3
      0x70020001,  //  005D  JMP		#0060
      0x8C0C011A,  //  005E  GETMET	R3	R0	K26
      0x7C0C0200,  //  005F  CALL	R3	1
      0x70020001,  //  0060  JMP		#0063
      0x8C08011A,  //  0061  GETMET	R2	R0	K26
      0x7C080200,  //  0062  CALL	R2	1
      0x60080003,  //  0063  GETGBL	R2	G3
      0x5C0C0000,  //  0064  MOVE	R3	R0
      0x7C080200,  //  0065  CALL	R2	1
      0x8C08051B,  //  0066  GETMET	R2	R2	K27
      0x5C100200,  //  0067  MOVE	R4	R1
      0x7C080400,  //  0068  CALL	R2	2
      0x80000000,  //  0069  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_clusters
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_get_clusters,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(get_clusters),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x8804011C,  //  0000  GETMBR	R1	R0	K28
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060001,  //  0003  JMPF	R1	#0006
      0x8804011C,  //  0004  GETMBR	R1	R0	K28
      0x80040200,  //  0005  RET	1	R1
      0x8804011D,  //  0006  GETMBR	R1	R0	K29
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_status,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(parse_status),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x540E0009,  //  0000  LDINT	R3	10
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0002,  //  0002  JMPF	R3	#0006
      0x8C0C0315,  //  0003  GETMET	R3	R1	K21
      0x88140114,  //  0004  GETMBR	R5	R0	K20
      0x7C0C0400,  //  0005  CALL	R3	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_air_quality
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_compute_air_quality,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(compute_air_quality),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A001A,  //  0004  JMPF	R2	#0020
      0x88080101,  //  0005  GETMBR	R2	R0	K1
      0x540E02ED,  //  0006  LDINT	R3	750
      0x180C0403,  //  0007  LE	R3	R2	R3
      0x780E0001,  //  0008  JMPF	R3	#000B
      0x5804001E,  //  0009  LDCONST	R1	K30
      0x70020014,  //  000A  JMP		#0020
      0x540E03E7,  //  000B  LDINT	R3	1000
      0x180C0403,  //  000C  LE	R3	R2	R3
      0x780E0001,  //  000D  JMPF	R3	#0010
      0x5804001F,  //  000E  LDCONST	R1	K31
      0x7002000F,  //  000F  JMP		#0020
      0x540E04E1,  //  0010  LDINT	R3	1250
      0x180C0403,  //  0011  LE	R3	R2	R3
      0x780E0001,  //  0012  JMPF	R3	#0015
      0x58040020,  //  0013  LDCONST	R1	K32
      0x7002000A,  //  0014  JMP		#0020
      0x540E05DB,  //  0015  LDINT	R3	1500
      0x180C0403,  //  0016  LE	R3	R2	R3
      0x780E0001,  //  0017  JMPF	R3	#001A
      0x54060003,  //  0018  LDINT	R1	4
      0x70020005,  //  0019  JMP		#0020
      0x540E06D5,  //  001A  LDINT	R3	1750
      0x180C0403,  //  001B  LE	R3	R2	R3
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x54060004,  //  001D  LDINT	R1	5
      0x70020000,  //  001E  JMP		#0020
      0x54060005,  //  001F  LDINT	R1	6
      0x4C080000,  //  0020  LDNIL	R2
      0x20080202,  //  0021  NE	R2	R1	R2
      0x780A0007,  //  0022  JMPF	R2	#002B
      0x88080118,  //  0023  GETMBR	R2	R0	K24
      0x20080202,  //  0024  NE	R2	R1	R2
      0x780A0004,  //  0025  JMPF	R2	#002B
      0x90023001,  //  0026  SETMBR	R0	K24	R1
      0x8C080121,  //  0027  GETMET	R2	R0	K33
      0x5412005A,  //  0028  LDINT	R4	91
      0x58140004,  //  0029  LDCONST	R5	K4
      0x7C080600,  //  002A  CALL	R2	3
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_configuration,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080522,  //  0003  GETMET	R2	R2	K34
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8C080315,  //  0006  GETMET	R2	R1	K21
      0x58100023,  //  0007  LDCONST	R4	K35
      0x7C080400,  //  0008  CALL	R2	2
      0x90022802,  //  0009  SETMBR	R0	K20	R2
      0x8C080315,  //  000A  GETMET	R2	R1	K21
      0x58100024,  //  000B  LDCONST	R4	K36
      0x7C080400,  //  000C  CALL	R2	2
      0x90020002,  //  000D  SETMBR	R0	K0	R2
      0x8C080315,  //  000E  GETMET	R2	R1	K21
      0x58100025,  //  000F  LDCONST	R4	K37
      0x7C080400,  //  0010  CALL	R2	2
      0x90020A02,  //  0011  SETMBR	R0	K5	R2
      0x8C080315,  //  0012  GETMET	R2	R1	K21
      0x58100026,  //  0013  LDCONST	R4	K38
      0x7C080400,  //  0014  CALL	R2	2
      0x90021002,  //  0015  SETMBR	R0	K8	R2
      0x8C080315,  //  0016  GETMET	R2	R1	K21
      0x58100027,  //  0017  LDCONST	R4	K39
      0x7C080400,  //  0018  CALL	R2	2
      0x90021602,  //  0019  SETMBR	R0	K11	R2
      0x8C080315,  //  001A  GETMET	R2	R1	K21
      0x58100028,  //  001B  LDCONST	R4	K40
      0x7C080400,  //  001C  CALL	R2	2
      0x90021C02,  //  001D  SETMBR	R0	K14	R2
      0x8C080315,  //  001E  GETMET	R2	R1	K21
      0x58100029,  //  001F  LDCONST	R4	K41
      0x7C080400,  //  0020  CALL	R2	2
      0x90022202,  //  0021  SETMBR	R0	K17	R2
      0x8C08012A,  //  0022  GETMET	R2	R0	K42
      0x7C080200,  //  0023  CALL	R2	1
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _build_clusters
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality__build_clusters,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(_build_clusters),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x8808011D,  //  0002  GETMBR	R2	R0	K29
      0x600C0010,  //  0003  GETGBL	R3	G16
      0x8C10052B,  //  0004  GETMET	R4	R2	K43
      0x7C100200,  //  0005  CALL	R4	1
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA8020004,  //  0007  EXBLK	0	#000D
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x94140404,  //  000A  GETIDX	R5	R2	R4
      0x98040805,  //  000B  SETIDX	R1	R4	R5
      0x7001FFFA,  //  000C  JMP		#0008
      0x580C002C,  //  000D  LDCONST	R3	K44
      0xAC0C0200,  //  000E  CATCH	R3	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x880C0100,  //  0010  GETMBR	R3	R0	K0
      0x740E0002,  //  0011  JMPT	R3	#0015
      0x8C0C032D,  //  0012  GETMET	R3	R1	K45
      0x5416040C,  //  0013  LDINT	R5	1037
      0x7C0C0400,  //  0014  CALL	R3	2
      0x880C0108,  //  0015  GETMBR	R3	R0	K8
      0x740E0002,  //  0016  JMPT	R3	#001A
      0x8C0C032D,  //  0017  GETMET	R3	R1	K45
      0x5416042B,  //  0018  LDINT	R5	1068
      0x7C0C0400,  //  0019  CALL	R3	2
      0x880C010B,  //  001A  GETMBR	R3	R0	K11
      0x740E0002,  //  001B  JMPT	R3	#001F
      0x8C0C032D,  //  001C  GETMET	R3	R1	K45
      0x54160429,  //  001D  LDINT	R5	1066
      0x7C0C0400,  //  001E  CALL	R3	2
      0x880C010E,  //  001F  GETMBR	R3	R0	K14
      0x740E0002,  //  0020  JMPT	R3	#0024
      0x8C0C032D,  //  0021  GETMET	R3	R1	K45
      0x5416042C,  //  0022  LDINT	R5	1069
      0x7C0C0400,  //  0023  CALL	R3	2
      0x880C0111,  //  0024  GETMBR	R3	R0	K17
      0x740E0002,  //  0025  JMPT	R3	#0029
      0x8C0C032D,  //  0026  GETMET	R3	R1	K45
      0x5416042D,  //  0027  LDINT	R5	1070
      0x7C0C0400,  //  0028  CALL	R3	2
      0x880C0105,  //  0029  GETMBR	R3	R0	K5
      0x740E0002,  //  002A  JMPT	R3	#002E
      0x8C0C032D,  //  002B  GETMET	R3	R1	K45
      0x54160412,  //  002C  LDINT	R5	1043
      0x7C0C0400,  //  002D  CALL	R3	2
      0x90023801,  //  002E  SETMBR	R0	K28	R1
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _parse_sensor_entry
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality__parse_sensor_entry,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    7,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(_parse_sensor_entry),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x8C1C0315,  //  0000  GETMET	R7	R1	K21
      0x5C240400,  //  0001  MOVE	R9	R2
      0x7C1C0400,  //  0002  CALL	R7	2
      0x4C200000,  //  0003  LDNIL	R8
      0x20200E08,  //  0004  NE	R8	R7	R8
      0x7822000D,  //  0005  JMPF	R8	#0014
      0x5C200800,  //  0006  MOVE	R8	R4
      0x5C240E00,  //  0007  MOVE	R9	R7
      0x7C200200,  //  0008  CALL	R8	1
      0x5C1C1000,  //  0009  MOVE	R7	R8
      0x4C200000,  //  000A  LDNIL	R8
      0x20200E08,  //  000B  NE	R8	R7	R8
      0x78220005,  //  000C  JMPF	R8	#0013
      0x20200E03,  //  000D  NE	R8	R7	R3
      0x78220003,  //  000E  JMPF	R8	#0013
      0x8C200121,  //  000F  GETMET	R8	R0	K33
      0x5C280A00,  //  0010  MOVE	R10	R5
      0x5C2C0C00,  //  0011  MOVE	R11	R6
      0x7C200600,  //  0012  CALL	R8	3
      0x80040E00,  //  0013  RET	1	R7
      0x80040600,  //  0014  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C10092E,  //  0003  GETMET	R4	R4	K46
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x90023104,  //  0008  SETMBR	R0	K24	K4
      0x8C10032F,  //  0009  GETMET	R4	R1	K47
      0x88180130,  //  000A  GETMBR	R6	R0	K48
      0x7C100400,  //  000B  CALL	R4	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_update_virtual,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x8C080131,  //  0000  GETMET	R2	R0	K49
      0x5C100200,  //  0001  MOVE	R4	R1
      0x58140032,  //  0002  LDCONST	R5	K50
      0x88180101,  //  0003  GETMBR	R6	R0	K1
      0x601C0007,  //  0004  GETGBL	R7	G7
      0x5422040C,  //  0005  LDINT	R8	1037
      0x58240004,  //  0006  LDCONST	R9	K4
      0x7C080E00,  //  0007  CALL	R2	7
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x8C080131,  //  0009  GETMET	R2	R0	K49
      0x5C100200,  //  000A  MOVE	R4	R1
      0x5814000A,  //  000B  LDCONST	R5	K10
      0x88180109,  //  000C  GETMBR	R6	R0	K9
      0x601C0007,  //  000D  GETGBL	R7	G7
      0x5422042B,  //  000E  LDINT	R8	1068
      0x58240004,  //  000F  LDCONST	R9	K4
      0x7C080E00,  //  0010  CALL	R2	7
      0x90021202,  //  0011  SETMBR	R0	K9	R2
      0x8C080131,  //  0012  GETMET	R2	R0	K49
      0x5C100200,  //  0013  MOVE	R4	R1
      0x5814000D,  //  0014  LDCONST	R5	K13
      0x8818010C,  //  0015  GETMBR	R6	R0	K12
      0x601C0007,  //  0016  GETGBL	R7	G7
      0x54220429,  //  0017  LDINT	R8	1066
      0x58240004,  //  0018  LDCONST	R9	K4
      0x7C080E00,  //  0019  CALL	R2	7
      0x90021802,  //  001A  SETMBR	R0	K12	R2
      0x8C080131,  //  001B  GETMET	R2	R0	K49
      0x5C100200,  //  001C  MOVE	R4	R1
      0x58140010,  //  001D  LDCONST	R5	K16
      0x8818010F,  //  001E  GETMBR	R6	R0	K15
      0x601C0007,  //  001F  GETGBL	R7	G7
      0x5422042C,  //  0020  LDINT	R8	1069
      0x58240004,  //  0021  LDCONST	R9	K4
      0x7C080E00,  //  0022  CALL	R2	7
      0x90021E02,  //  0023  SETMBR	R0	K15	R2
      0x8C080131,  //  0024  GETMET	R2	R0	K49
      0x5C100200,  //  0025  MOVE	R4	R1
      0x58140013,  //  0026  LDCONST	R5	K19
      0x88180112,  //  0027  GETMBR	R6	R0	K18
      0x601C0007,  //  0028  GETGBL	R7	G7
      0x5422042D,  //  0029  LDINT	R8	1070
      0x58240004,  //  002A  LDCONST	R9	K4
      0x7C080E00,  //  002B  CALL	R2	7
      0x90022402,  //  002C  SETMBR	R0	K18	R2
      0x8C080316,  //  002D  GETMET	R2	R1	K22
      0x58100017,  //  002E  LDCONST	R4	K23
      0x7C080400,  //  002F  CALL	R2	2
      0x780A0009,  //  0030  JMPF	R2	#003B
      0x8C080131,  //  0031  GETMET	R2	R0	K49
      0x5C100200,  //  0032  MOVE	R4	R1
      0x58140017,  //  0033  LDCONST	R5	K23
      0x60180007,  //  0034  GETGBL	R6	G7
      0x881C0118,  //  0035  GETMBR	R7	R0	K24
      0x5422005A,  //  0036  LDINT	R8	91
      0x58240004,  //  0037  LDCONST	R9	K4
      0x7C080E00,  //  0038  CALL	R2	7
      0x90023002,  //  0039  SETMBR	R0	K24	R2
      0x70020001,  //  003A  JMP		#003D
      0x8C08011A,  //  003B  GETMET	R2	R0	K26
      0x7C080200,  //  003C  CALL	R2	1
      0x60080003,  //  003D  GETGBL	R2	G3
      0x5C0C0000,  //  003E  MOVE	R3	R0
      0x7C080200,  //  003F  CALL	R2	1
      0x8C080533,  //  0040  GETMET	R2	R2	K51
      0x5C100200,  //  0041  MOVE	R4	R1
      0x7C080400,  //  0042  CALL	R2	2
      0x80000000,  //  0043  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_read_attribute,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        6,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 6),
          be_local_const_upval(1, 3),
          be_local_const_upval(1, 4),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 9]) {     /* constants */
        /* K0   */  be_const_int(0),
        /* K1   */  be_nested_str_weak(set_or_nil),
        /* K2   */  be_nested_str_weak(FLOAT),
        /* K3   */  be_const_int(1),
        /* K4   */  be_nested_str_weak(set),
        /* K5   */  be_nested_str_weak(NULL),
        /* K6   */  be_const_int(2),
        /* K7   */  be_nested_str_weak(U1),
        /* K8   */  be_nested_str_weak(U4),
        }),
        be_str_weak(handle_value),
        &be_const_str_solidified,
        ( &(const binstruction[74]) {  /* code */
          0x4C080000,  //  0000  LDNIL	R2
          0x1C080202,  //  0001  EQ	R2	R1	R2
          0x780A0000,  //  0002  JMPF	R2	#0004
          0x58040000,  //  0003  LDCONST	R1	K0
          0x68080000,  //  0004  GETUPV	R2	U0
          0x1C080500,  //  0005  EQ	R2	R2	K0
          0x780A0007,  //  0006  JMPF	R2	#000F
          0x68080001,  //  0007  GETUPV	R2	U1
          0x8C080501,  //  0008  GETMET	R2	R2	K1
          0x68100002,  //  0009  GETUPV	R4	U2
          0x88100902,  //  000A  GETMBR	R4	R4	K2
          0x5C140000,  //  000B  MOVE	R5	R0
          0x7C080600,  //  000C  CALL	R2	3
          0x80040400,  //  000D  RET	1	R2
          0x70020038,  //  000E  JMP		#0048
          0x68080000,  //  000F  GETUPV	R2	U0
          0x1C080503,  //  0010  EQ	R2	R2	K3
          0x780A0007,  //  0011  JMPF	R2	#001A
          0x68080001,  //  0012  GETUPV	R2	U1
          0x8C080504,  //  0013  GETMET	R2	R2	K4
          0x68100002,  //  0014  GETUPV	R4	U2
          0x88100905,  //  0015  GETMBR	R4	R4	K5
          0x4C140000,  //  0016  LDNIL	R5
          0x7C080600,  //  0017  CALL	R2	3
          0x80040400,  //  0018  RET	1	R2
          0x7002002D,  //  0019  JMP		#0048
          0x68080000,  //  001A  GETUPV	R2	U0
          0x1C080506,  //  001B  EQ	R2	R2	K6
          0x780A0007,  //  001C  JMPF	R2	#0025
          0x68080001,  //  001D  GETUPV	R2	U1
          0x8C080504,  //  001E  GETMET	R2	R2	K4
          0x68100002,  //  001F  GETUPV	R4	U2
          0x88100905,  //  0020  GETMBR	R4	R4	K5
          0x4C140000,  //  0021  LDNIL	R5
          0x7C080600,  //  0022  CALL	R2	3
          0x80040400,  //  0023  RET	1	R2
          0x70020022,  //  0024  JMP		#0048
          0x68080000,  //  0025  GETUPV	R2	U0
          0x540E0007,  //  0026  LDINT	R3	8
          0x1C080403,  //  0027  EQ	R2	R2	R3
          0x780A0007,  //  0028  JMPF	R2	#0031
          0x68080001,  //  0029  GETUPV	R2	U1
          0x8C080504,  //  002A  GETMET	R2	R2	K4
          0x68100002,  //  002B  GETUPV	R4	U2
          0x88100907,  //  002C  GETMBR	R4	R4	K7
          0x5C140200,  //  002D  MOVE	R5	R1
          0x7C080600,  //  002E  CALL	R2	3
          0x80040400,  //  002F  RET	1	R2
          0x70020016,  //  0030  JMP		#0048
          0x68080000,  //  0031  GETUPV	R2	U0
          0x540E0008,  //  0032  LDINT	R3	9
          0x1C080403,  //  0033  EQ	R2	R2	R3
          0x780A0007,  //  0034  JMPF	R2	#003D
          0x68080001,  //  0035  GETUPV	R2	U1
          0x8C080504,  //  0036  GETMET	R2	R2	K4
          0x68100002,  //  0037  GETUPV	R4	U2
          0x88100907,  //  0038  GETMBR	R4	R4	K7
          0x58140000,  //  0039  LDCONST	R5	K0
          0x7C080600,  //  003A  CALL	R2	3
          0x80040400,  //  003B  RET	1	R2
          0x7002000A,  //  003C  JMP		#0048
          0x68080000,  //  003D  GETUPV	R2	U0
          0x540EFFFB,  //  003E  LDINT	R3	65532
          0x1C080403,  //  003F  EQ	R2	R2	R3
          0x780A0006,  //  0040  JMPF	R2	#0048
          0x68080001,  //  0041  GETUPV	R2	U1
          0x8C080504,  //  0042  GETMET	R2	R2	K4
          0x68100002,  //  0043  GETUPV	R4	U2
          0x88100908,  //  0044  GETMBR	R4	R4	K8
          0x58140003,  //  0045  LDCONST	R5	K3
          0x7C080600,  //  0046  CALL	R2	3
          0x80040400,  //  0047  RET	1	R2
          0x4C080000,  //  0048  LDNIL	R2
          0x80040400,  //  0049  RET	1	R2
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
      0xB8126800,  //  0000  GETNGBL	R4	K52
      0x88100935,  //  0001  GETMBR	R4	R4	K53
      0x88140536,  //  0002  GETMBR	R5	R2	K54
      0x88180537,  //  0003  GETMBR	R6	R2	K55
      0x4C1C0000,  //  0004  LDNIL	R7
      0x84200000,  //  0005  CLOSURE	R8	P0
      0x5426005A,  //  0006  LDINT	R9	91
      0x1C240A09,  //  0007  EQ	R9	R5	R9
      0x78260008,  //  0008  JMPF	R9	#0012
      0x1C240D04,  //  0009  EQ	R9	R6	K4
      0x78260005,  //  000A  JMPF	R9	#0011
      0x8C240738,  //  000B  GETMET	R9	R3	K56
      0x882C0939,  //  000C  GETMBR	R11	R4	K57
      0x88300118,  //  000D  GETMBR	R12	R0	K24
      0x7C240600,  //  000E  CALL	R9	3
      0xA0000000,  //  000F  CLOSE	R0
      0x80041200,  //  0010  RET	1	R9
      0x70020040,  //  0011  JMP		#0053
      0x5426040C,  //  0012  LDINT	R9	1037
      0x1C240A09,  //  0013  EQ	R9	R5	R9
      0x78260007,  //  0014  JMPF	R9	#001D
      0x5C241000,  //  0015  MOVE	R9	R8
      0x88280101,  //  0016  GETMBR	R10	R0	K1
      0x7C240200,  //  0017  CALL	R9	1
      0x5C1C1200,  //  0018  MOVE	R7	R9
      0x78260001,  //  0019  JMPF	R9	#001C
      0xA0000000,  //  001A  CLOSE	R0
      0x80040E00,  //  001B  RET	1	R7
      0x70020035,  //  001C  JMP		#0053
      0x5426042B,  //  001D  LDINT	R9	1068
      0x1C240A09,  //  001E  EQ	R9	R5	R9
      0x78260007,  //  001F  JMPF	R9	#0028
      0x5C241000,  //  0020  MOVE	R9	R8
      0x88280109,  //  0021  GETMBR	R10	R0	K9
      0x7C240200,  //  0022  CALL	R9	1
      0x5C1C1200,  //  0023  MOVE	R7	R9
      0x78260001,  //  0024  JMPF	R9	#0027
      0xA0000000,  //  0025  CLOSE	R0
      0x80040E00,  //  0026  RET	1	R7
      0x7002002A,  //  0027  JMP		#0053
      0x54260429,  //  0028  LDINT	R9	1066
      0x1C240A09,  //  0029  EQ	R9	R5	R9
      0x78260007,  //  002A  JMPF	R9	#0033
      0x5C241000,  //  002B  MOVE	R9	R8
      0x8828010C,  //  002C  GETMBR	R10	R0	K12
      0x7C240200,  //  002D  CALL	R9	1
      0x5C1C1200,  //  002E  MOVE	R7	R9
      0x78260001,  //  002F  JMPF	R9	#0032
      0xA0000000,  //  0030  CLOSE	R0
      0x80040E00,  //  0031  RET	1	R7
      0x7002001F,  //  0032  JMP		#0053
      0x5426042C,  //  0033  LDINT	R9	1069
      0x1C240A09,  //  0034  EQ	R9	R5	R9
      0x78260007,  //  0035  JMPF	R9	#003E
      0x5C241000,  //  0036  MOVE	R9	R8
      0x8828010F,  //  0037  GETMBR	R10	R0	K15
      0x7C240200,  //  0038  CALL	R9	1
      0x5C1C1200,  //  0039  MOVE	R7	R9
      0x78260001,  //  003A  JMPF	R9	#003D
      0xA0000000,  //  003B  CLOSE	R0
      0x80040E00,  //  003C  RET	1	R7
      0x70020014,  //  003D  JMP		#0053
      0x5426042D,  //  003E  LDINT	R9	1070
      0x1C240A09,  //  003F  EQ	R9	R5	R9
      0x78260007,  //  0040  JMPF	R9	#0049
      0x5C241000,  //  0041  MOVE	R9	R8
      0x88280112,  //  0042  GETMBR	R10	R0	K18
      0x7C240200,  //  0043  CALL	R9	1
      0x5C1C1200,  //  0044  MOVE	R7	R9
      0x78260001,  //  0045  JMPF	R9	#0048
      0xA0000000,  //  0046  CLOSE	R0
      0x80040E00,  //  0047  RET	1	R7
      0x70020009,  //  0048  JMP		#0053
      0x54260412,  //  0049  LDINT	R9	1043
      0x1C240A09,  //  004A  EQ	R9	R5	R9
      0x78260006,  //  004B  JMPF	R9	#0053
      0x5C241000,  //  004C  MOVE	R9	R8
      0x88280106,  //  004D  GETMBR	R10	R0	K6
      0x7C240200,  //  004E  CALL	R9	1
      0x5C1C1200,  //  004F  MOVE	R7	R9
      0x78260001,  //  0050  JMPF	R9	#0053
      0xA0000000,  //  0051  CLOSE	R0
      0x80040E00,  //  0052  RET	1	R7
      0x60240003,  //  0053  GETGBL	R9	G3
      0x5C280000,  //  0054  MOVE	R10	R0
      0x7C240200,  //  0055  CALL	R9	1
      0x8C24133A,  //  0056  GETMET	R9	R9	K58
      0x5C2C0200,  //  0057  MOVE	R11	R1
      0x5C300400,  //  0058  MOVE	R12	R2
      0x5C340600,  //  0059  MOVE	R13	R3
      0x7C240800,  //  005A  CALL	R9	4
      0xA0000000,  //  005B  CLOSE	R0
      0x80041200,  //  005C  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _parse_sensor_filter
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality__parse_sensor_filter,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    8,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(_parse_sensor_filter),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0xA4227600,  //  0000  IMPORT	R8	K59
      0x5C240400,  //  0001  MOVE	R9	R2
      0x5C280600,  //  0002  MOVE	R10	R3
      0x8C2C1115,  //  0003  GETMET	R11	R8	K21
      0x5C340400,  //  0004  MOVE	R13	R2
      0x5838003C,  //  0005  LDCONST	R14	K60
      0x7C2C0600,  //  0006  CALL	R11	3
      0x28301704,  //  0007  GE	R12	R11	K4
      0x78320005,  //  0008  JMPF	R12	#000F
      0x0430171E,  //  0009  SUB	R12	R11	K30
      0x4032080C,  //  000A  CONNECT	R12	K4	R12
      0x9424040C,  //  000B  GETIDX	R9	R2	R12
      0x0030171E,  //  000C  ADD	R12	R11	K30
      0x4030193D,  //  000D  CONNECT	R12	R12	K61
      0x9428040C,  //  000E  GETIDX	R10	R2	R12
      0x8C300315,  //  000F  GETMET	R12	R1	K21
      0x5C381200,  //  0010  MOVE	R14	R9
      0x7C300400,  //  0011  CALL	R12	2
      0x4C340000,  //  0012  LDNIL	R13
      0x2034180D,  //  0013  NE	R13	R12	R13
      0x78360008,  //  0014  JMPF	R13	#001E
      0x8C340119,  //  0015  GETMET	R13	R0	K25
      0x5C3C1800,  //  0016  MOVE	R15	R12
      0x5C401400,  //  0017  MOVE	R16	R10
      0x5C440800,  //  0018  MOVE	R17	R4
      0x5C480A00,  //  0019  MOVE	R18	R5
      0x5C4C0C00,  //  001A  MOVE	R19	R6
      0x5C500E00,  //  001B  MOVE	R20	R7
      0x7C340E00,  //  001C  CALL	R13	7
      0x80041A00,  //  001D  RET	1	R13
      0x80040800,  //  001E  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_web_values,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        9,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_nested_str_weak(webserver),
        /* K1   */  be_nested_str_weak(content_send),
        /* K2   */  be_nested_str_weak(_X25s_X3A_X25i_X20),
        }),
        be_str_weak(web_values_single),
        &be_const_str_solidified,
        ( &(const binstruction[12]) {  /* code */
          0x4C080000,  //  0000  LDNIL	R2
          0x20080202,  //  0001  NE	R2	R1	R2
          0x780A0007,  //  0002  JMPF	R2	#000B
          0xA40A0000,  //  0003  IMPORT	R2	K0
          0x8C0C0501,  //  0004  GETMET	R3	R2	K1
          0x60140018,  //  0005  GETGBL	R5	G24
          0x58180002,  //  0006  LDCONST	R6	K2
          0x5C1C0000,  //  0007  MOVE	R7	R0
          0x5C200200,  //  0008  MOVE	R8	R1
          0x7C140600,  //  0009  CALL	R5	3
          0x7C0C0400,  //  000A  CALL	R3	2
          0x80000000,  //  000B  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Air_Quality,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x84040000,  //  0000  CLOSURE	R1	P0
      0x8C08013E,  //  0001  GETMET	R2	R0	K62
      0x7C080200,  //  0002  CALL	R2	1
      0x5C080200,  //  0003  MOVE	R2	R1
      0x580C003F,  //  0004  LDCONST	R3	K63
      0x88100118,  //  0005  GETMBR	R4	R0	K24
      0x7C080400,  //  0006  CALL	R2	2
      0x88080108,  //  0007  GETMBR	R2	R0	K8
      0x780A0003,  //  0008  JMPF	R2	#000D
      0x5C080200,  //  0009  MOVE	R2	R1
      0x580C000A,  //  000A  LDCONST	R3	K10
      0x88100109,  //  000B  GETMBR	R4	R0	K9
      0x7C080400,  //  000C  CALL	R2	2
      0x8808010B,  //  000D  GETMBR	R2	R0	K11
      0x780A0003,  //  000E  JMPF	R2	#0013
      0x5C080200,  //  000F  MOVE	R2	R1
      0x580C000D,  //  0010  LDCONST	R3	K13
      0x8810010C,  //  0011  GETMBR	R4	R0	K12
      0x7C080400,  //  0012  CALL	R2	2
      0x8808010E,  //  0013  GETMBR	R2	R0	K14
      0x780A0003,  //  0014  JMPF	R2	#0019
      0x5C080200,  //  0015  MOVE	R2	R1
      0x580C0010,  //  0016  LDCONST	R3	K16
      0x8810010F,  //  0017  GETMBR	R4	R0	K15
      0x7C080400,  //  0018  CALL	R2	2
      0x88080100,  //  0019  GETMBR	R2	R0	K0
      0x780A0003,  //  001A  JMPF	R2	#001F
      0x5C080200,  //  001B  MOVE	R2	R1
      0x580C0032,  //  001C  LDCONST	R3	K50
      0x88100101,  //  001D  GETMBR	R4	R0	K1
      0x7C080400,  //  001E  CALL	R2	2
      0x88080105,  //  001F  GETMBR	R2	R0	K5
      0x780A0003,  //  0020  JMPF	R2	#0025
      0x5C080200,  //  0021  MOVE	R2	R1
      0x580C0007,  //  0022  LDCONST	R3	K7
      0x88100106,  //  0023  GETMBR	R4	R0	K6
      0x7C080400,  //  0024  CALL	R2	2
      0x88080111,  //  0025  GETMBR	R2	R0	K17
      0x780A0003,  //  0026  JMPF	R2	#002B
      0x5C080200,  //  0027  MOVE	R2	R1
      0x580C0013,  //  0028  LDCONST	R3	K19
      0x88100112,  //  0029  GETMBR	R4	R0	K18
      0x7C080400,  //  002A  CALL	R2	2
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_Air_Quality
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor_Air_Quality,
    15,
    &be_class_Matter_Plugin_Device,
    be_nested_map(41,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(parse_sensors, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_sensors_closure) },
        { be_const_key_weak(SCHEMA, 20), be_nested_str_weak(airquality_X7Cl_X3AAir_X20Quality_X7Ch_X3ADevice_X20key_X20_X28ex_X3A_X20SCD40_X29) },
        { be_const_key_weak(parse_status, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_status_closure) },
        { be_const_key_weak(SCHEMA6, -1), be_nested_str_weak(pm10_X7Cl_X3APM10_X7Ch_X3APM10_X20filter_X20_X28ex_X3A_X20SEN55_X23PM10_X29) },
        { be_const_key_weak(prefix_pm1, 18), be_const_var(2) },
        { be_const_key_weak(get_clusters, 33), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_get_clusters_closure) },
        { be_const_key_weak(TYPE, 32), be_nested_str_weak(airquality) },
        { be_const_key_weak(SCHEMA3, -1), be_nested_str_weak(no2_X7Cl_X3ANO2_X7Ch_X3ANO2_X20filter_X20_X28ex_X3A_X20SEN55_X23NO2_X29) },
        { be_const_key_weak(UPDATE_COMMANDS, 35), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(AirQuality),
        be_nested_str_weak(CO2),
        be_nested_str_weak(PM1),
        be_nested_str_weak(PM2_X2E5),
        be_nested_str_weak(PM10),
        be_nested_str_weak(TVOC),
        be_nested_str_weak(NO2),
    }))    ) } )) },
        { be_const_key_weak(shadow_air_quality, 6), be_const_var(8) },
        { be_const_key_weak(compute_air_quality, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_compute_air_quality_closure) },
        { be_const_key_weak(shadow_tvoc, -1), be_const_var(14) },
        { be_const_key_weak(SCHEMA7, -1), be_nested_str_weak(tvoc_X7Cl_X3ATVOC_X7Ch_X3ATVOC_X20filter_X20_X28ex_X3A_X20SEN55_X23TVOC_X29) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_configuration_closure) },
        { be_const_key_weak(shadow_pm1, 11), be_const_var(9) },
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak(AirQuality) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(44, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(shadow_pm2_5, -1), be_const_var(10) },
        { be_const_key_weak(prefix_airquality, -1), be_const_var(6) },
        { be_const_key_weak(_parse_sensor_entry, 26), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality__parse_sensor_entry_closure) },
        { be_const_key_weak(clusters_derived, 36), be_const_var(7) },
        { be_const_key_weak(SCHEMA4, -1), be_nested_str_weak(pm1_X7Cl_X3APM1_X7Ch_X3APM1_X20filter_X20_X28ex_X3A_X20SEN55_X23PM1_X29) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_web_values_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_init_closure) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_update_virtual_closure) },
        { be_const_key_weak(prefix_pm2_5, -1), be_const_var(3) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(10000) },
        { be_const_key_weak(shadow_pm10, -1), be_const_var(11) },
        { be_const_key_weak(prefix_tvoc, 22), be_const_var(5) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_read_attribute_closure) },
        { be_const_key_weak(SCHEMA5, -1), be_nested_str_weak(pm2_5_X7Cl_X3APM2_X2E5_X7Ch_X3APM2_X2E5_X20filter_X20_X28ex_X3A_X20SEN55_X23PM2_X2E5_X29) },
        { be_const_key_weak(shadow_co2, -1), be_const_var(12) },
        { be_const_key_weak(_parse_sensor_filter, 34), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality__parse_sensor_filter_closure) },
        { be_const_key_weak(prefix_co2, 38), be_const_var(0) },
        { be_const_key_weak(prefix_pm10, -1), be_const_var(4) },
        { be_const_key_weak(prefix_no2, -1), be_const_var(1) },
        { be_const_key_weak(_build_clusters, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality__build_clusters_closure) },
        { be_const_key_weak(shadow_no2, -1), be_const_var(13) },
        { be_const_key_weak(DISPLAY_NAME, 2), be_nested_str_weak(Air_X20Quality) },
        { be_const_key_weak(SCHEMA2, 4), be_nested_str_weak(co2_X7Cl_X3ACO2_X7Ch_X3ACO2_X20filter_X20_X28ex_X3A_X20SCD40_X23CarbonDioxide_X29) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(1068, -1), be_const_bytes_instance(00000001000200080009FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(1069, -1), be_const_bytes_instance(00000001000200080009FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(1070, -1), be_const_bytes_instance(00000001000200080009FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, -1), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(5, 8), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(1037, -1), be_const_bytes_instance(00000001000200080009FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(91, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 6), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(1066, -1), be_const_bytes_instance(00000001000200080009FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(1043, -1), be_const_bytes_instance(00000001000200080009FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Air_Quality)
);
/********************************************************************/
/* End of solidification */
