/* Solidification of Matter_Plugin_3_Bridge_Sensor_Air_Quality.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Air_Quality;
// Borrowed method 'read_attribute' from class 'class_Matter_Plugin_Sensor_Air_Quality'
extern bclosure *class_Matter_Plugin_Sensor_Air_Quality_read_attribute;

/********************************************************************
** Solidified function: web_values
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Air_Quality;
be_local_closure(class_Matter_Plugin_Bridge_Sensor_Air_Quality_web_values,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        9,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL, 
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_nested_str_weak(webserver),
        /* K1   */  be_nested_str_weak(content_send),
        /* K2   */  be_nested_str_weak(_X25s_X20_X25i_X20),
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
    &be_class_Matter_Plugin_Bridge_Sensor_Air_Quality, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
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
    }),
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
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Air_Quality;
be_local_closure(class_Matter_Plugin_Bridge_Sensor_Air_Quality_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Bridge_Sensor_Air_Quality, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(prefix),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(ARG),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x60100008,  //  0008  GETGBL	R4	G8
      0x8C140702,  //  0009  GETMET	R5	R3	K2
      0x881C0103,  //  000A  GETMBR	R7	R0	K3
      0x7C140400,  //  000B  CALL	R5	2
      0x7C100200,  //  000C  CALL	R4	1
      0x90020204,  //  000D  SETMBR	R0	K1	R4
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_update
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Air_Quality;
be_local_closure(class_Matter_Plugin_Bridge_Sensor_Air_Quality_parse_update,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Bridge_Sensor_Air_Quality, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(prefix),
    }),
    be_str_weak(parse_update),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x540E0007,  //  0000  LDINT	R3	8
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0002,  //  0002  JMPF	R3	#0006
      0x8C0C0300,  //  0003  GETMET	R3	R1	K0
      0x88140101,  //  0004  GETMBR	R5	R0	K1
      0x7C0C0400,  //  0005  CALL	R3	2
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Sensor_Air_Quality
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_HTTP;
be_local_class(Matter_Plugin_Bridge_Sensor_Air_Quality,
    8,
    &be_class_Matter_Plugin_Bridge_HTTP,
    be_nested_map(20,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(UPDATE_CMD, -1), be_nested_str_weak(Status_X208) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Air_X20Quality) },
        { be_const_key_weak(shadow_tvoc, 8), be_const_var(7) },
        { be_const_key_weak(shadow_pm10, 7), be_const_var(4) },
        { be_const_key_weak(shadow_pm2_5, -1), be_const_var(3) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Sensor_X20Model) },
        { be_const_key_weak(shadow_co2, -1), be_const_var(5) },
        { be_const_key_weak(parse_update, 18), be_const_closure(class_Matter_Plugin_Bridge_Sensor_Air_Quality_parse_update_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Sensor_Air_Quality_read_attribute_closure) },
        { be_const_key_weak(prefix, 13), be_const_var(0) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(44, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Bridge_Sensor_Air_Quality_web_values_closure) },
        { be_const_key_weak(shadow_pm1, 11), be_const_var(2) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_airquality) },
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
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Bridge_Sensor_Air_Quality_init_closure) },
        { be_const_key_weak(UPDATE_TIME, 3), be_const_int(5000) },
        { be_const_key_weak(shadow_no2, 4), be_const_var(6) },
        { be_const_key_weak(shadow_air_quality, -1), be_const_var(1) },
        { be_const_key_weak(ARG, 0), be_nested_str_weak(airquality) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor_Air_Quality)
);
/********************************************************************/
/* End of solidification */
