/* Solidification of Matter_Plugin_4_Bridge_Sensor_Humidity.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Humidity;

/********************************************************************
** Solidified function: pre_value
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Humidity;
be_local_closure(class_Matter_Plugin_Bridge_Sensor_Humidity_pre_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Bridge_Sensor_Humidity, 
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(pre_value),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0004,  //  0002  JMPF	R2	#0008
      0x60080009,  //  0003  GETGBL	R2	G9
      0x540E0063,  //  0004  LDINT	R3	100
      0x080C0203,  //  0005  MUL	R3	R1	R3
      0x7C080200,  //  0006  CALL	R2	1
      0x70020000,  //  0007  JMP		#0009
      0x4C080000,  //  0008  LDNIL	R2
      0x80040400,  //  0009  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: value_changed
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Humidity;
be_local_closure(class_Matter_Plugin_Bridge_Sensor_Humidity_value_changed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Bridge_Sensor_Humidity, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(attribute_updated),
    /* K1   */  be_const_int(0),
    }),
    be_str_weak(value_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0404,  //  0001  LDINT	R3	1029
      0x58100001,  //  0002  LDCONST	R4	K1
      0x7C040600,  //  0003  CALL	R1	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/

// Borrowed method 'read_attribute' from class 'class_Matter_Plugin_Sensor_Humidity'
extern bclosure *class_Matter_Plugin_Sensor_Humidity_read_attribute;

/********************************************************************
** Solidified function: web_values
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Humidity;
be_local_closure(class_Matter_Plugin_Bridge_Sensor_Humidity_web_values,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Bridge_Sensor_Humidity, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X26_X23x1F4A7_X3B_X20_X252_X2E0f_X25_X25),
    /* K4   */  be_nested_str_weak(shadow_value),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140003,  //  0005  LDCONST	R5	K3
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x4C1C0000,  //  0007  LDNIL	R7
      0x20180C07,  //  0008  NE	R6	R6	R7
      0x781A0005,  //  0009  JMPF	R6	#0010
      0x6018000A,  //  000A  GETGBL	R6	G10
      0x881C0104,  //  000B  GETMBR	R7	R0	K4
      0x7C180200,  //  000C  CALL	R6	1
      0x541E0063,  //  000D  LDINT	R7	100
      0x0C180C07,  //  000E  DIV	R6	R6	R7
      0x70020000,  //  000F  JMP		#0011
      0x4C180000,  //  0010  LDNIL	R6
      0x7C100400,  //  0011  CALL	R4	2
      0x7C080400,  //  0012  CALL	R2	2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Sensor_Humidity
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Sensor;
be_local_class(Matter_Plugin_Bridge_Sensor_Humidity,
    0,
    &be_class_Matter_Plugin_Bridge_Sensor,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pre_value, 1), be_const_closure(class_Matter_Plugin_Bridge_Sensor_Humidity_pre_value_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Bridge_Sensor_Humidity_web_values_closure) },
        { be_const_key_weak(TYPES, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(775, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, 7), be_const_closure(class_Matter_Plugin_Sensor_Humidity_read_attribute_closure) },
        { be_const_key_weak(value_changed, 6), be_const_closure(class_Matter_Plugin_Bridge_Sensor_Humidity_value_changed_closure) },
        { be_const_key_weak(DISPLAY_NAME, 4), be_nested_str_weak(Humidity) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_humidity) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(57, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(1029, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
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
        { be_const_key_int(29, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor_Humidity)
);
/********************************************************************/
/* End of solidification */
