/* Solidification of Matter_Plugin_2_Sensor_Air_Quality.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_Air_Quality' ktab size: 37, total: 74 (saved 296 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_Air_Quality[37] = {
  /* K0   */  be_nested_str_weak(web_values_prefix),
  /* K1   */  be_nested_str_weak(Air),
  /* K2   */  be_nested_str_weak(shadow_air_quality),
  /* K3   */  be_nested_str_weak(PM1),
  /* K4   */  be_nested_str_weak(shadow_pm1),
  /* K5   */  be_nested_str_weak(PM2_X2E5),
  /* K6   */  be_nested_str_weak(shadow_pm2_5),
  /* K7   */  be_nested_str_weak(PM10),
  /* K8   */  be_nested_str_weak(shadow_pm10),
  /* K9   */  be_nested_str_weak(CO2),
  /* K10  */  be_nested_str_weak(shadow_co2),
  /* K11  */  be_nested_str_weak(NO2),
  /* K12  */  be_nested_str_weak(shadow_no2),
  /* K13  */  be_nested_str_weak(TVOC),
  /* K14  */  be_nested_str_weak(shadow_tvoc),
  /* K15  */  be_nested_str_weak(find),
  /* K16  */  be_nested_str_weak(prefix),
  /* K17  */  be_nested_str_weak(_parse_update_virtual),
  /* K18  */  be_nested_str_weak(AirQuality),
  /* K19  */  be_const_int(0),
  /* K20  */  be_nested_str_weak(update_virtual),
  /* K21  */  be_nested_str_weak(init),
  /* K22  */  be_nested_str_weak(add_read_sensors_schedule),
  /* K23  */  be_nested_str_weak(UPDATE_TIME),
  /* K24  */  be_nested_str_weak(matter),
  /* K25  */  be_nested_str_weak(TLV),
  /* K26  */  be_nested_str_weak(cluster),
  /* K27  */  be_nested_str_weak(attribute),
  /* K28  */  be_nested_str_weak(set_or_nil),
  /* K29  */  be_nested_str_weak(U1),
  /* K30  */  be_nested_str_weak(read_attribute),
  /* K31  */  be_nested_str_weak(parse_configuration),
  /* K32  */  be_nested_str_weak(ARG),
  /* K33  */  be_nested_str_weak(_parse_sensor_entry),
  /* K34  */  be_nested_str_weak(CarbonDioxide),
  /* K35  */  be_nested_str_weak(parse_sensors),
  /* K36  */  be_nested_str_weak(attribute_updated),
};


extern const bclass be_class_Matter_Plugin_Sensor_Air_Quality;

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
    ( &(const binstruction[32]) {  /* code */
      0x84040000,  //  0000  CLOSURE	R1	P0
      0x8C080100,  //  0001  GETMET	R2	R0	K0
      0x7C080200,  //  0002  CALL	R2	1
      0x5C080200,  //  0003  MOVE	R2	R1
      0x580C0001,  //  0004  LDCONST	R3	K1
      0x88100102,  //  0005  GETMBR	R4	R0	K2
      0x7C080400,  //  0006  CALL	R2	2
      0x5C080200,  //  0007  MOVE	R2	R1
      0x580C0003,  //  0008  LDCONST	R3	K3
      0x88100104,  //  0009  GETMBR	R4	R0	K4
      0x7C080400,  //  000A  CALL	R2	2
      0x5C080200,  //  000B  MOVE	R2	R1
      0x580C0005,  //  000C  LDCONST	R3	K5
      0x88100106,  //  000D  GETMBR	R4	R0	K6
      0x7C080400,  //  000E  CALL	R2	2
      0x5C080200,  //  000F  MOVE	R2	R1
      0x580C0007,  //  0010  LDCONST	R3	K7
      0x88100108,  //  0011  GETMBR	R4	R0	K8
      0x7C080400,  //  0012  CALL	R2	2
      0x5C080200,  //  0013  MOVE	R2	R1
      0x580C0009,  //  0014  LDCONST	R3	K9
      0x8810010A,  //  0015  GETMBR	R4	R0	K10
      0x7C080400,  //  0016  CALL	R2	2
      0x5C080200,  //  0017  MOVE	R2	R1
      0x580C000B,  //  0018  LDCONST	R3	K11
      0x8810010C,  //  0019  GETMBR	R4	R0	K12
      0x7C080400,  //  001A  CALL	R2	2
      0x5C080200,  //  001B  MOVE	R2	R1
      0x580C000D,  //  001C  LDCONST	R3	K13
      0x8810010E,  //  001D  GETMBR	R4	R0	K14
      0x7C080400,  //  001E  CALL	R2	2
      0x80000000,  //  001F  RET	0
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
      0x8C0C030F,  //  0003  GETMET	R3	R1	K15
      0x88140110,  //  0004  GETMBR	R5	R0	K16
      0x7C0C0400,  //  0005  CALL	R3	2
      0x80000000,  //  0006  RET	0
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
    ( &(const binstruction[61]) {  /* code */
      0x8C080111,  //  0000  GETMET	R2	R0	K17
      0x5C100200,  //  0001  MOVE	R4	R1
      0x58140012,  //  0002  LDCONST	R5	K18
      0x60180007,  //  0003  GETGBL	R6	G7
      0x881C0102,  //  0004  GETMBR	R7	R0	K2
      0x5422005A,  //  0005  LDINT	R8	91
      0x58240013,  //  0006  LDCONST	R9	K19
      0x7C080E00,  //  0007  CALL	R2	7
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x8C080111,  //  0009  GETMET	R2	R0	K17
      0x5C100200,  //  000A  MOVE	R4	R1
      0x58140009,  //  000B  LDCONST	R5	K9
      0x8818010A,  //  000C  GETMBR	R6	R0	K10
      0x601C0007,  //  000D  GETGBL	R7	G7
      0x5422040C,  //  000E  LDINT	R8	1037
      0x58240013,  //  000F  LDCONST	R9	K19
      0x7C080E00,  //  0010  CALL	R2	7
      0x90021402,  //  0011  SETMBR	R0	K10	R2
      0x8C080111,  //  0012  GETMET	R2	R0	K17
      0x5C100200,  //  0013  MOVE	R4	R1
      0x58140003,  //  0014  LDCONST	R5	K3
      0x88180104,  //  0015  GETMBR	R6	R0	K4
      0x601C0007,  //  0016  GETGBL	R7	G7
      0x5422042B,  //  0017  LDINT	R8	1068
      0x58240013,  //  0018  LDCONST	R9	K19
      0x7C080E00,  //  0019  CALL	R2	7
      0x90020802,  //  001A  SETMBR	R0	K4	R2
      0x8C080111,  //  001B  GETMET	R2	R0	K17
      0x5C100200,  //  001C  MOVE	R4	R1
      0x58140005,  //  001D  LDCONST	R5	K5
      0x88180106,  //  001E  GETMBR	R6	R0	K6
      0x601C0007,  //  001F  GETGBL	R7	G7
      0x54220429,  //  0020  LDINT	R8	1066
      0x58240013,  //  0021  LDCONST	R9	K19
      0x7C080E00,  //  0022  CALL	R2	7
      0x90020C02,  //  0023  SETMBR	R0	K6	R2
      0x8C080111,  //  0024  GETMET	R2	R0	K17
      0x5C100200,  //  0025  MOVE	R4	R1
      0x58140007,  //  0026  LDCONST	R5	K7
      0x88180108,  //  0027  GETMBR	R6	R0	K8
      0x601C0007,  //  0028  GETGBL	R7	G7
      0x5422042C,  //  0029  LDINT	R8	1069
      0x58240013,  //  002A  LDCONST	R9	K19
      0x7C080E00,  //  002B  CALL	R2	7
      0x90021002,  //  002C  SETMBR	R0	K8	R2
      0x8C080111,  //  002D  GETMET	R2	R0	K17
      0x5C100200,  //  002E  MOVE	R4	R1
      0x5814000D,  //  002F  LDCONST	R5	K13
      0x8818010E,  //  0030  GETMBR	R6	R0	K14
      0x601C0007,  //  0031  GETGBL	R7	G7
      0x5422042D,  //  0032  LDINT	R8	1070
      0x58240013,  //  0033  LDCONST	R9	K19
      0x7C080E00,  //  0034  CALL	R2	7
      0x90021C02,  //  0035  SETMBR	R0	K14	R2
      0x60080003,  //  0036  GETGBL	R2	G3
      0x5C0C0000,  //  0037  MOVE	R3	R0
      0x7C080200,  //  0038  CALL	R2	1
      0x8C080514,  //  0039  GETMET	R2	R2	K20
      0x5C100200,  //  003A  MOVE	R4	R1
      0x7C080400,  //  003B  CALL	R2	2
      0x80000000,  //  003C  RET	0
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
    ( &(const binstruction[14]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100915,  //  0003  GETMET	R4	R4	K21
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x50100000,  //  0008  LDBOOL	R4	0	0
      0x90020404,  //  0009  SETMBR	R0	K2	R4
      0x8C100316,  //  000A  GETMET	R4	R1	K22
      0x88180117,  //  000B  GETMBR	R6	R0	K23
      0x7C100400,  //  000C  CALL	R4	2
      0x80000000,  //  000D  RET	0
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
      0xB8123000,  //  0000  GETNGBL	R4	K24
      0x88100919,  //  0001  GETMBR	R4	R4	K25
      0x8814051A,  //  0002  GETMBR	R5	R2	K26
      0x8818051B,  //  0003  GETMBR	R6	R2	K27
      0x4C1C0000,  //  0004  LDNIL	R7
      0x84200000,  //  0005  CLOSURE	R8	P0
      0x5426005A,  //  0006  LDINT	R9	91
      0x1C240A09,  //  0007  EQ	R9	R5	R9
      0x78260008,  //  0008  JMPF	R9	#0012
      0x1C240D13,  //  0009  EQ	R9	R6	K19
      0x78260005,  //  000A  JMPF	R9	#0011
      0x8C24071C,  //  000B  GETMET	R9	R3	K28
      0x882C091D,  //  000C  GETMBR	R11	R4	K29
      0x88300102,  //  000D  GETMBR	R12	R0	K2
      0x7C240600,  //  000E  CALL	R9	3
      0xA0000000,  //  000F  CLOSE	R0
      0x80041200,  //  0010  RET	1	R9
      0x70020040,  //  0011  JMP		#0053
      0x5426040C,  //  0012  LDINT	R9	1037
      0x1C240A09,  //  0013  EQ	R9	R5	R9
      0x78260007,  //  0014  JMPF	R9	#001D
      0x5C241000,  //  0015  MOVE	R9	R8
      0x8828010A,  //  0016  GETMBR	R10	R0	K10
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
      0x88280104,  //  0021  GETMBR	R10	R0	K4
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
      0x88280106,  //  002C  GETMBR	R10	R0	K6
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
      0x88280108,  //  0037  GETMBR	R10	R0	K8
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
      0x8828010E,  //  0042  GETMBR	R10	R0	K14
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
      0x8828010C,  //  004D  GETMBR	R10	R0	K12
      0x7C240200,  //  004E  CALL	R9	1
      0x5C1C1200,  //  004F  MOVE	R7	R9
      0x78260001,  //  0050  JMPF	R9	#0053
      0xA0000000,  //  0051  CLOSE	R0
      0x80040E00,  //  0052  RET	1	R7
      0x60240003,  //  0053  GETGBL	R9	G3
      0x5C280000,  //  0054  MOVE	R10	R0
      0x7C240200,  //  0055  CALL	R9	1
      0x8C24131E,  //  0056  GETMET	R9	R9	K30
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
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_configuration,   /* name */
  be_nested_proto(
    6,                          /* nstack */
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
    ( &(const binstruction[13]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08051F,  //  0003  GETMET	R2	R2	K31
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080008,  //  0006  GETGBL	R2	G8
      0x8C0C030F,  //  0007  GETMET	R3	R1	K15
      0x88140120,  //  0008  GETMBR	R5	R0	K32
      0x7C0C0400,  //  0009  CALL	R3	2
      0x7C080200,  //  000A  CALL	R2	1
      0x90022002,  //  000B  SETMBR	R0	K16	R2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


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
    ( &(const binstruction[67]) {  /* code */
      0x8C08030F,  //  0000  GETMET	R2	R1	K15
      0x88100110,  //  0001  GETMBR	R4	R0	K16
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0035,  //  0005  JMPF	R3	#003C
      0x8C0C0121,  //  0006  GETMET	R3	R0	K33
      0x5C140400,  //  0007  MOVE	R5	R2
      0x58180022,  //  0008  LDCONST	R6	K34
      0x881C010A,  //  0009  GETMBR	R7	R0	K10
      0x60200007,  //  000A  GETGBL	R8	G7
      0x5426040C,  //  000B  LDINT	R9	1037
      0x58280013,  //  000C  LDCONST	R10	K19
      0x7C0C0E00,  //  000D  CALL	R3	7
      0x90021403,  //  000E  SETMBR	R0	K10	R3
      0x8C0C0121,  //  000F  GETMET	R3	R0	K33
      0x5C140400,  //  0010  MOVE	R5	R2
      0x58180003,  //  0011  LDCONST	R6	K3
      0x881C0104,  //  0012  GETMBR	R7	R0	K4
      0x60200007,  //  0013  GETGBL	R8	G7
      0x5426042B,  //  0014  LDINT	R9	1068
      0x58280013,  //  0015  LDCONST	R10	K19
      0x7C0C0E00,  //  0016  CALL	R3	7
      0x90020803,  //  0017  SETMBR	R0	K4	R3
      0x8C0C0121,  //  0018  GETMET	R3	R0	K33
      0x5C140400,  //  0019  MOVE	R5	R2
      0x58180005,  //  001A  LDCONST	R6	K5
      0x881C0106,  //  001B  GETMBR	R7	R0	K6
      0x60200007,  //  001C  GETGBL	R8	G7
      0x54260429,  //  001D  LDINT	R9	1066
      0x58280013,  //  001E  LDCONST	R10	K19
      0x7C0C0E00,  //  001F  CALL	R3	7
      0x90020C03,  //  0020  SETMBR	R0	K6	R3
      0x8C0C0121,  //  0021  GETMET	R3	R0	K33
      0x5C140400,  //  0022  MOVE	R5	R2
      0x58180007,  //  0023  LDCONST	R6	K7
      0x881C0108,  //  0024  GETMBR	R7	R0	K8
      0x60200007,  //  0025  GETGBL	R8	G7
      0x5426042C,  //  0026  LDINT	R9	1069
      0x58280013,  //  0027  LDCONST	R10	K19
      0x7C0C0E00,  //  0028  CALL	R3	7
      0x90021003,  //  0029  SETMBR	R0	K8	R3
      0x8C0C0121,  //  002A  GETMET	R3	R0	K33
      0x5C140400,  //  002B  MOVE	R5	R2
      0x5818000D,  //  002C  LDCONST	R6	K13
      0x881C010E,  //  002D  GETMBR	R7	R0	K14
      0x60200007,  //  002E  GETGBL	R8	G7
      0x5426042D,  //  002F  LDINT	R9	1070
      0x58280013,  //  0030  LDCONST	R10	K19
      0x7C0C0E00,  //  0031  CALL	R3	7
      0x90021C03,  //  0032  SETMBR	R0	K14	R3
      0x8C0C0121,  //  0033  GETMET	R3	R0	K33
      0x5C140400,  //  0034  MOVE	R5	R2
      0x5818000B,  //  0035  LDCONST	R6	K11
      0x881C010C,  //  0036  GETMBR	R7	R0	K12
      0x60200007,  //  0037  GETGBL	R8	G7
      0x54260412,  //  0038  LDINT	R9	1043
      0x58280013,  //  0039  LDCONST	R10	K19
      0x7C0C0E00,  //  003A  CALL	R3	7
      0x90021803,  //  003B  SETMBR	R0	K12	R3
      0x600C0003,  //  003C  GETGBL	R3	G3
      0x5C100000,  //  003D  MOVE	R4	R0
      0x7C0C0200,  //  003E  CALL	R3	1
      0x8C0C0723,  //  003F  GETMET	R3	R3	K35
      0x5C140200,  //  0040  MOVE	R5	R1
      0x7C0C0400,  //  0041  CALL	R3	2
      0x80000000,  //  0042  RET	0
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
      0x8C1C030F,  //  0000  GETMET	R7	R1	K15
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
      0x8C200124,  //  000F  GETMET	R8	R0	K36
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
** Solidified class: Matter_Plugin_Sensor_Air_Quality
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor_Air_Quality,
    8,
    &be_class_Matter_Plugin_Device,
    be_nested_map(25,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak(AirQuality) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Air_X20Quality) },
        { be_const_key_weak(web_values, 16), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_web_values_closure) },
        { be_const_key_weak(parse_status, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_status_closure) },
        { be_const_key_weak(_parse_sensor_entry, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality__parse_sensor_entry_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_init_closure) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(10000) },
        { be_const_key_weak(shadow_no2, -1), be_const_var(6) },
        { be_const_key_weak(shadow_air_quality, 13), be_const_var(1) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_weak(prefix, -1), be_const_var(0) },
        { be_const_key_weak(shadow_pm10, -1), be_const_var(4) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_read_attribute_closure) },
        { be_const_key_weak(parse_sensors, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_sensors_closure) },
        { be_const_key_weak(TYPE, 10), be_nested_str_weak(airquality) },
        { be_const_key_weak(update_virtual, 23), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_update_virtual_closure) },
        { be_const_key_weak(shadow_tvoc, -1), be_const_var(7) },
        { be_const_key_weak(shadow_pm1, -1), be_const_var(2) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(airquality) },
        { be_const_key_weak(parse_configuration, 18), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_parse_configuration_closure) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Device_X20key_X20_X28ex_X3A_X20SCD40_X29) },
        { be_const_key_weak(shadow_co2, 8), be_const_var(5) },
        { be_const_key_weak(shadow_pm2_5, -1), be_const_var(3) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(44, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, 4), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
