/* Solidification of Matter_Plugin_2_Sensor_Air_Quality.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;

/********************************************************************
** Solidified function: _parse_sensor_entry
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality__parse_sensor_entry,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Air_Quality, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(attribute_updated),
    }),
    be_str_weak(_parse_sensor_entry),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C180300,  //  0000  GETMET	R6	R1	K0
      0x5C200400,  //  0001  MOVE	R8	R2
      0x7C180400,  //  0002  CALL	R6	2
      0x4C1C0000,  //  0003  LDNIL	R7
      0x201C0C07,  //  0004  NE	R7	R6	R7
      0x781E0006,  //  0005  JMPF	R7	#000D
      0x201C0C03,  //  0006  NE	R7	R6	R3
      0x781E0003,  //  0007  JMPF	R7	#000C
      0x8C1C0101,  //  0008  GETMET	R7	R0	K1
      0x5C240800,  //  0009  MOVE	R9	R4
      0x5C280A00,  //  000A  MOVE	R10	R5
      0x7C1C0600,  //  000B  CALL	R7	3
      0x80040C00,  //  000C  RET	1	R6
      0x80040600,  //  000D  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_update_virtual,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Air_Quality, 
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(shadow_air_quality),
    /* K1   */  be_nested_str_weak(_parse_update_virtual),
    /* K2   */  be_nested_str_weak(AirQuality),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(shadow_co2),
    /* K5   */  be_nested_str_weak(CO2),
    /* K6   */  be_nested_str_weak(shadow_pm1),
    /* K7   */  be_nested_str_weak(PM1),
    /* K8   */  be_nested_str_weak(shadow_pm2_5),
    /* K9   */  be_nested_str_weak(PM2_X2E5),
    /* K10  */  be_nested_str_weak(shadow_pm10),
    /* K11  */  be_nested_str_weak(PM10),
    /* K12  */  be_nested_str_weak(shadow_tvoc),
    /* K13  */  be_nested_str_weak(TVOC),
    /* K14  */  be_nested_str_weak(update_virtual),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0x8C080101,  //  0000  GETMET	R2	R0	K1
      0x5C100200,  //  0001  MOVE	R4	R1
      0x58140002,  //  0002  LDCONST	R5	K2
      0x60180007,  //  0003  GETGBL	R6	G7
      0x881C0100,  //  0004  GETMBR	R7	R0	K0
      0x5422005A,  //  0005  LDINT	R8	91
      0x58240003,  //  0006  LDCONST	R9	K3
      0x7C080E00,  //  0007  CALL	R2	7
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x8C080101,  //  0009  GETMET	R2	R0	K1
      0x5C100200,  //  000A  MOVE	R4	R1
      0x58140005,  //  000B  LDCONST	R5	K5
      0x88180104,  //  000C  GETMBR	R6	R0	K4
      0x601C0007,  //  000D  GETGBL	R7	G7
      0x5422040C,  //  000E  LDINT	R8	1037
      0x58240003,  //  000F  LDCONST	R9	K3
      0x7C080E00,  //  0010  CALL	R2	7
      0x90020802,  //  0011  SETMBR	R0	K4	R2
      0x8C080101,  //  0012  GETMET	R2	R0	K1
      0x5C100200,  //  0013  MOVE	R4	R1
      0x58140007,  //  0014  LDCONST	R5	K7
      0x88180106,  //  0015  GETMBR	R6	R0	K6
      0x601C0007,  //  0016  GETGBL	R7	G7
      0x5422042B,  //  0017  LDINT	R8	1068
      0x58240003,  //  0018  LDCONST	R9	K3
      0x7C080E00,  //  0019  CALL	R2	7
      0x90020C02,  //  001A  SETMBR	R0	K6	R2
      0x8C080101,  //  001B  GETMET	R2	R0	K1
      0x5C100200,  //  001C  MOVE	R4	R1
      0x58140009,  //  001D  LDCONST	R5	K9
      0x88180108,  //  001E  GETMBR	R6	R0	K8
      0x601C0007,  //  001F  GETGBL	R7	G7
      0x54220429,  //  0020  LDINT	R8	1066
      0x58240003,  //  0021  LDCONST	R9	K3
      0x7C080E00,  //  0022  CALL	R2	7
      0x90021002,  //  0023  SETMBR	R0	K8	R2
      0x8C080101,  //  0024  GETMET	R2	R0	K1
      0x5C100200,  //  0025  MOVE	R4	R1
      0x5814000B,  //  0026  LDCONST	R5	K11
      0x8818010A,  //  0027  GETMBR	R6	R0	K10
      0x601C0007,  //  0028  GETGBL	R7	G7
      0x5422042C,  //  0029  LDINT	R8	1069
      0x58240003,  //  002A  LDCONST	R9	K3
      0x7C080E00,  //  002B  CALL	R2	7
      0x90021402,  //  002C  SETMBR	R0	K10	R2
      0x8C080101,  //  002D  GETMET	R2	R0	K1
      0x5C100200,  //  002E  MOVE	R4	R1
      0x5814000D,  //  002F  LDCONST	R5	K13
      0x8818010C,  //  0030  GETMBR	R6	R0	K12
      0x601C0007,  //  0031  GETGBL	R7	G7
      0x5422042D,  //  0032  LDINT	R8	1070
      0x58240003,  //  0033  LDCONST	R9	K3
      0x7C080E00,  //  0034  CALL	R2	7
      0x90021802,  //  0035  SETMBR	R0	K12	R2
      0x60080003,  //  0036  GETGBL	R2	G3
      0x5C0C0000,  //  0037  MOVE	R3	R0
      0x7C080200,  //  0038  CALL	R2	1
      0x8C08050E,  //  0039  GETMET	R2	R2	K14
      0x5C100200,  //  003A  MOVE	R4	R1
      0x7C080400,  //  003B  CALL	R2	2
      0x80000000,  //  003C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_sensors
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_sensors,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Air_Quality, 
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(prefix),
    /* K2   */  be_nested_str_weak(shadow_co2),
    /* K3   */  be_nested_str_weak(_parse_sensor_entry),
    /* K4   */  be_nested_str_weak(CarbonDioxide),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_pm1),
    /* K7   */  be_nested_str_weak(PM1),
    /* K8   */  be_nested_str_weak(shadow_pm2_5),
    /* K9   */  be_nested_str_weak(PM2_X2E5),
    /* K10  */  be_nested_str_weak(shadow_pm10),
    /* K11  */  be_nested_str_weak(PM10),
    /* K12  */  be_nested_str_weak(shadow_tvoc),
    /* K13  */  be_nested_str_weak(TVOC),
    /* K14  */  be_nested_str_weak(shadow_no2),
    /* K15  */  be_nested_str_weak(NO2),
    /* K16  */  be_nested_str_weak(parse_sensors),
    }),
    be_str_weak(parse_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[67]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0035,  //  0005  JMPF	R3	#003C
      0x8C0C0103,  //  0006  GETMET	R3	R0	K3
      0x5C140400,  //  0007  MOVE	R5	R2
      0x58180004,  //  0008  LDCONST	R6	K4
      0x881C0102,  //  0009  GETMBR	R7	R0	K2
      0x60200007,  //  000A  GETGBL	R8	G7
      0x5426040C,  //  000B  LDINT	R9	1037
      0x58280005,  //  000C  LDCONST	R10	K5
      0x7C0C0E00,  //  000D  CALL	R3	7
      0x90020403,  //  000E  SETMBR	R0	K2	R3
      0x8C0C0103,  //  000F  GETMET	R3	R0	K3
      0x5C140400,  //  0010  MOVE	R5	R2
      0x58180007,  //  0011  LDCONST	R6	K7
      0x881C0106,  //  0012  GETMBR	R7	R0	K6
      0x60200007,  //  0013  GETGBL	R8	G7
      0x5426042B,  //  0014  LDINT	R9	1068
      0x58280005,  //  0015  LDCONST	R10	K5
      0x7C0C0E00,  //  0016  CALL	R3	7
      0x90020C03,  //  0017  SETMBR	R0	K6	R3
      0x8C0C0103,  //  0018  GETMET	R3	R0	K3
      0x5C140400,  //  0019  MOVE	R5	R2
      0x58180009,  //  001A  LDCONST	R6	K9
      0x881C0108,  //  001B  GETMBR	R7	R0	K8
      0x60200007,  //  001C  GETGBL	R8	G7
      0x54260429,  //  001D  LDINT	R9	1066
      0x58280005,  //  001E  LDCONST	R10	K5
      0x7C0C0E00,  //  001F  CALL	R3	7
      0x90021003,  //  0020  SETMBR	R0	K8	R3
      0x8C0C0103,  //  0021  GETMET	R3	R0	K3
      0x5C140400,  //  0022  MOVE	R5	R2
      0x5818000B,  //  0023  LDCONST	R6	K11
      0x881C010A,  //  0024  GETMBR	R7	R0	K10
      0x60200007,  //  0025  GETGBL	R8	G7
      0x5426042C,  //  0026  LDINT	R9	1069
      0x58280005,  //  0027  LDCONST	R10	K5
      0x7C0C0E00,  //  0028  CALL	R3	7
      0x90021403,  //  0029  SETMBR	R0	K10	R3
      0x8C0C0103,  //  002A  GETMET	R3	R0	K3
      0x5C140400,  //  002B  MOVE	R5	R2
      0x5818000D,  //  002C  LDCONST	R6	K13
      0x881C010C,  //  002D  GETMBR	R7	R0	K12
      0x60200007,  //  002E  GETGBL	R8	G7
      0x5426042D,  //  002F  LDINT	R9	1070
      0x58280005,  //  0030  LDCONST	R10	K5
      0x7C0C0E00,  //  0031  CALL	R3	7
      0x90021803,  //  0032  SETMBR	R0	K12	R3
      0x8C0C0103,  //  0033  GETMET	R3	R0	K3
      0x5C140400,  //  0034  MOVE	R5	R2
      0x5818000F,  //  0035  LDCONST	R6	K15
      0x881C010E,  //  0036  GETMBR	R7	R0	K14
      0x60200007,  //  0037  GETGBL	R8	G7
      0x54260412,  //  0038  LDINT	R9	1043
      0x58280005,  //  0039  LDCONST	R10	K5
      0x7C0C0E00,  //  003A  CALL	R3	7
      0x90021C03,  //  003B  SETMBR	R0	K14	R3
      0x600C0003,  //  003C  GETGBL	R3	G3
      0x5C100000,  //  003D  MOVE	R4	R0
      0x7C0C0200,  //  003E  CALL	R3	1
      0x8C0C0710,  //  003F  GETMET	R3	R3	K16
      0x5C140200,  //  0040  MOVE	R5	R1
      0x7C0C0400,  //  0041  CALL	R3	2
      0x80000000,  //  0042  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_read_attribute,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
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
        NULL, 
        1,                          /* has constants */
        ( &(const bvalue[ 8]) {     /* constants */
        /* K0   */  be_const_int(0),
        /* K1   */  be_nested_str_weak(set),
        /* K2   */  be_nested_str_weak(FLOAT),
        /* K3   */  be_nested_str_weak(NULL),
        /* K4   */  be_const_int(1),
        /* K5   */  be_const_int(2),
        /* K6   */  be_nested_str_weak(U1),
        /* K7   */  be_nested_str_weak(U4),
        }),
        be_str_weak(handle_value),
        &be_const_str_solidified,
        ( &(const binstruction[85]) {  /* code */
          0x4C080000,  //  0000  LDNIL	R2
          0x1C080202,  //  0001  EQ	R2	R1	R2
          0x780A0000,  //  0002  JMPF	R2	#0004
          0x58040000,  //  0003  LDCONST	R1	K0
          0x68080000,  //  0004  GETUPV	R2	U0
          0x1C080500,  //  0005  EQ	R2	R2	K0
          0x780A0012,  //  0006  JMPF	R2	#001A
          0x4C080000,  //  0007  LDNIL	R2
          0x20080002,  //  0008  NE	R2	R0	R2
          0x780A0007,  //  0009  JMPF	R2	#0012
          0x68080001,  //  000A  GETUPV	R2	U1
          0x8C080501,  //  000B  GETMET	R2	R2	K1
          0x68100002,  //  000C  GETUPV	R4	U2
          0x88100902,  //  000D  GETMBR	R4	R4	K2
          0x5C140000,  //  000E  MOVE	R5	R0
          0x7C080600,  //  000F  CALL	R2	3
          0x80040400,  //  0010  RET	1	R2
          0x70020006,  //  0011  JMP		#0019
          0x68080001,  //  0012  GETUPV	R2	U1
          0x8C080501,  //  0013  GETMET	R2	R2	K1
          0x68100002,  //  0014  GETUPV	R4	U2
          0x88100903,  //  0015  GETMBR	R4	R4	K3
          0x4C140000,  //  0016  LDNIL	R5
          0x7C080600,  //  0017  CALL	R2	3
          0x80040400,  //  0018  RET	1	R2
          0x70020038,  //  0019  JMP		#0053
          0x68080000,  //  001A  GETUPV	R2	U0
          0x1C080504,  //  001B  EQ	R2	R2	K4
          0x780A0007,  //  001C  JMPF	R2	#0025
          0x68080001,  //  001D  GETUPV	R2	U1
          0x8C080501,  //  001E  GETMET	R2	R2	K1
          0x68100002,  //  001F  GETUPV	R4	U2
          0x88100903,  //  0020  GETMBR	R4	R4	K3
          0x4C140000,  //  0021  LDNIL	R5
          0x7C080600,  //  0022  CALL	R2	3
          0x80040400,  //  0023  RET	1	R2
          0x7002002D,  //  0024  JMP		#0053
          0x68080000,  //  0025  GETUPV	R2	U0
          0x1C080505,  //  0026  EQ	R2	R2	K5
          0x780A0007,  //  0027  JMPF	R2	#0030
          0x68080001,  //  0028  GETUPV	R2	U1
          0x8C080501,  //  0029  GETMET	R2	R2	K1
          0x68100002,  //  002A  GETUPV	R4	U2
          0x88100903,  //  002B  GETMBR	R4	R4	K3
          0x4C140000,  //  002C  LDNIL	R5
          0x7C080600,  //  002D  CALL	R2	3
          0x80040400,  //  002E  RET	1	R2
          0x70020022,  //  002F  JMP		#0053
          0x68080000,  //  0030  GETUPV	R2	U0
          0x540E0007,  //  0031  LDINT	R3	8
          0x1C080403,  //  0032  EQ	R2	R2	R3
          0x780A0007,  //  0033  JMPF	R2	#003C
          0x68080001,  //  0034  GETUPV	R2	U1
          0x8C080501,  //  0035  GETMET	R2	R2	K1
          0x68100002,  //  0036  GETUPV	R4	U2
          0x88100906,  //  0037  GETMBR	R4	R4	K6
          0x5C140200,  //  0038  MOVE	R5	R1
          0x7C080600,  //  0039  CALL	R2	3
          0x80040400,  //  003A  RET	1	R2
          0x70020016,  //  003B  JMP		#0053
          0x68080000,  //  003C  GETUPV	R2	U0
          0x540E0008,  //  003D  LDINT	R3	9
          0x1C080403,  //  003E  EQ	R2	R2	R3
          0x780A0007,  //  003F  JMPF	R2	#0048
          0x68080001,  //  0040  GETUPV	R2	U1
          0x8C080501,  //  0041  GETMET	R2	R2	K1
          0x68100002,  //  0042  GETUPV	R4	U2
          0x88100906,  //  0043  GETMBR	R4	R4	K6
          0x58140000,  //  0044  LDCONST	R5	K0
          0x7C080600,  //  0045  CALL	R2	3
          0x80040400,  //  0046  RET	1	R2
          0x7002000A,  //  0047  JMP		#0053
          0x68080000,  //  0048  GETUPV	R2	U0
          0x540EFFFB,  //  0049  LDINT	R3	65532
          0x1C080403,  //  004A  EQ	R2	R2	R3
          0x780A0006,  //  004B  JMPF	R2	#0053
          0x68080001,  //  004C  GETUPV	R2	U1
          0x8C080501,  //  004D  GETMET	R2	R2	K1
          0x68100002,  //  004E  GETUPV	R4	U2
          0x88100907,  //  004F  GETMBR	R4	R4	K7
          0x58140004,  //  0050  LDCONST	R5	K4
          0x7C080600,  //  0051  CALL	R2	3
          0x80040400,  //  0052  RET	1	R2
          0x4C080000,  //  0053  LDNIL	R2
          0x80040400,  //  0054  RET	1	R2
        })
      ),
    &be_class_Matter_Plugin_Sensor_Air_Quality, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(shadow_air_quality),
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(NULL),
    /* K9   */  be_nested_str_weak(shadow_co2),
    /* K10  */  be_nested_str_weak(shadow_pm1),
    /* K11  */  be_nested_str_weak(shadow_pm2_5),
    /* K12  */  be_nested_str_weak(shadow_pm10),
    /* K13  */  be_nested_str_weak(shadow_tvoc),
    /* K14  */  be_nested_str_weak(shadow_no2),
    /* K15  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[104]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x4C1C0000,  //  0004  LDNIL	R7
      0x84200000,  //  0005  CLOSURE	R8	P0
      0x5426005A,  //  0006  LDINT	R9	91
      0x1C240A09,  //  0007  EQ	R9	R5	R9
      0x78260013,  //  0008  JMPF	R9	#001D
      0x1C240D04,  //  0009  EQ	R9	R6	K4
      0x78260010,  //  000A  JMPF	R9	#001C
      0x88240105,  //  000B  GETMBR	R9	R0	K5
      0x4C280000,  //  000C  LDNIL	R10
      0x2024120A,  //  000D  NE	R9	R9	R10
      0x78260006,  //  000E  JMPF	R9	#0016
      0x8C240706,  //  000F  GETMET	R9	R3	K6
      0x882C0907,  //  0010  GETMBR	R11	R4	K7
      0x88300105,  //  0011  GETMBR	R12	R0	K5
      0x7C240600,  //  0012  CALL	R9	3
      0xA0000000,  //  0013  CLOSE	R0
      0x80041200,  //  0014  RET	1	R9
      0x70020005,  //  0015  JMP		#001C
      0x8C240706,  //  0016  GETMET	R9	R3	K6
      0x882C0908,  //  0017  GETMBR	R11	R4	K8
      0x4C300000,  //  0018  LDNIL	R12
      0x7C240600,  //  0019  CALL	R9	3
      0xA0000000,  //  001A  CLOSE	R0
      0x80041200,  //  001B  RET	1	R9
      0x70020040,  //  001C  JMP		#005E
      0x5426040C,  //  001D  LDINT	R9	1037
      0x1C240A09,  //  001E  EQ	R9	R5	R9
      0x78260007,  //  001F  JMPF	R9	#0028
      0x5C241000,  //  0020  MOVE	R9	R8
      0x88280109,  //  0021  GETMBR	R10	R0	K9
      0x7C240200,  //  0022  CALL	R9	1
      0x5C1C1200,  //  0023  MOVE	R7	R9
      0x78260001,  //  0024  JMPF	R9	#0027
      0xA0000000,  //  0025  CLOSE	R0
      0x80040E00,  //  0026  RET	1	R7
      0x70020035,  //  0027  JMP		#005E
      0x5426042B,  //  0028  LDINT	R9	1068
      0x1C240A09,  //  0029  EQ	R9	R5	R9
      0x78260007,  //  002A  JMPF	R9	#0033
      0x5C241000,  //  002B  MOVE	R9	R8
      0x8828010A,  //  002C  GETMBR	R10	R0	K10
      0x7C240200,  //  002D  CALL	R9	1
      0x5C1C1200,  //  002E  MOVE	R7	R9
      0x78260001,  //  002F  JMPF	R9	#0032
      0xA0000000,  //  0030  CLOSE	R0
      0x80040E00,  //  0031  RET	1	R7
      0x7002002A,  //  0032  JMP		#005E
      0x54260429,  //  0033  LDINT	R9	1066
      0x1C240A09,  //  0034  EQ	R9	R5	R9
      0x78260007,  //  0035  JMPF	R9	#003E
      0x5C241000,  //  0036  MOVE	R9	R8
      0x8828010B,  //  0037  GETMBR	R10	R0	K11
      0x7C240200,  //  0038  CALL	R9	1
      0x5C1C1200,  //  0039  MOVE	R7	R9
      0x78260001,  //  003A  JMPF	R9	#003D
      0xA0000000,  //  003B  CLOSE	R0
      0x80040E00,  //  003C  RET	1	R7
      0x7002001F,  //  003D  JMP		#005E
      0x5426042C,  //  003E  LDINT	R9	1069
      0x1C240A09,  //  003F  EQ	R9	R5	R9
      0x78260007,  //  0040  JMPF	R9	#0049
      0x5C241000,  //  0041  MOVE	R9	R8
      0x8828010C,  //  0042  GETMBR	R10	R0	K12
      0x7C240200,  //  0043  CALL	R9	1
      0x5C1C1200,  //  0044  MOVE	R7	R9
      0x78260001,  //  0045  JMPF	R9	#0048
      0xA0000000,  //  0046  CLOSE	R0
      0x80040E00,  //  0047  RET	1	R7
      0x70020014,  //  0048  JMP		#005E
      0x5426042D,  //  0049  LDINT	R9	1070
      0x1C240A09,  //  004A  EQ	R9	R5	R9
      0x78260007,  //  004B  JMPF	R9	#0054
      0x5C241000,  //  004C  MOVE	R9	R8
      0x8828010D,  //  004D  GETMBR	R10	R0	K13
      0x7C240200,  //  004E  CALL	R9	1
      0x5C1C1200,  //  004F  MOVE	R7	R9
      0x78260001,  //  0050  JMPF	R9	#0053
      0xA0000000,  //  0051  CLOSE	R0
      0x80040E00,  //  0052  RET	1	R7
      0x70020009,  //  0053  JMP		#005E
      0x54260412,  //  0054  LDINT	R9	1043
      0x1C240A09,  //  0055  EQ	R9	R5	R9
      0x78260006,  //  0056  JMPF	R9	#005E
      0x5C241000,  //  0057  MOVE	R9	R8
      0x8828010E,  //  0058  GETMBR	R10	R0	K14
      0x7C240200,  //  0059  CALL	R9	1
      0x5C1C1200,  //  005A  MOVE	R7	R9
      0x78260001,  //  005B  JMPF	R9	#005E
      0xA0000000,  //  005C  CLOSE	R0
      0x80040E00,  //  005D  RET	1	R7
      0x60240003,  //  005E  GETGBL	R9	G3
      0x5C280000,  //  005F  MOVE	R10	R0
      0x7C240200,  //  0060  CALL	R9	1
      0x8C24130F,  //  0061  GETMET	R9	R9	K15
      0x5C2C0200,  //  0062  MOVE	R11	R1
      0x5C300400,  //  0063  MOVE	R12	R2
      0x5C340600,  //  0064  MOVE	R13	R3
      0x7C240800,  //  0065  CALL	R9	4
      0xA0000000,  //  0066  CLOSE	R0
      0x80041200,  //  0067  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_configuration,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Air_Quality, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(prefix),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    }),
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080008,  //  0000  GETGBL	R2	G8
      0x8C0C0301,  //  0001  GETMET	R3	R1	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x7C0C0400,  //  0003  CALL	R3	2
      0x7C080200,  //  0004  CALL	R2	1
      0x90020002,  //  0005  SETMBR	R0	K0	R2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Air_Quality, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_air_quality),
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
      0x50100000,  //  0008  LDBOOL	R4	0	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_Air_Quality
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor_Air_Quality,
    8,
    &be_class_Matter_Plugin_Device,
    be_nested_map(23,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_update_virtual_closure) },
        { be_const_key_weak(shadow_pm10, 0), be_const_var(4) },
        { be_const_key_weak(ARG, 1), be_nested_str_weak(airquality) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(airquality) },
        { be_const_key_weak(parse_configuration, 21), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_configuration_closure) },
        { be_const_key_weak(shadow_no2, -1), be_const_var(6) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(30000) },
        { be_const_key_weak(UPDATE_COMMANDS, 13), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_weak(shadow_pm2_5, -1), be_const_var(3) },
        { be_const_key_weak(shadow_air_quality, -1), be_const_var(1) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_read_attribute_closure) },
        { be_const_key_weak(shadow_tvoc, -1), be_const_var(7) },
        { be_const_key_weak(prefix, -1), be_const_var(0) },
        { be_const_key_weak(TYPES, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(44, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(_parse_sensor_entry, 19), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality__parse_sensor_entry_closure) },
        { be_const_key_weak(shadow_pm1, 9), be_const_var(2) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Air_X20Quality) },
        { be_const_key_weak(parse_sensors, 7), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_sensors_closure) },
        { be_const_key_weak(shadow_co2, -1), be_const_var(5) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_init_closure) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Device_X20key_X20_X28ex_X3A_X20SCD40_X29) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(1068, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(11,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(8),
        be_const_int(9),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(1069, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(11,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(8),
        be_const_int(9),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(1070, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(11,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(8),
        be_const_int(9),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(5, 8), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(1037, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(11,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(8),
        be_const_int(9),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(91, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(29, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(1066, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(11,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(8),
        be_const_int(9),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(1043, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(11,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(8),
        be_const_int(9),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak(AirQuality) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Air_Quality)
);
/********************************************************************/
/* End of solidification */
