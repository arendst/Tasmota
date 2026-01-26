/* Solidification of Matter_Plugin_3_Sensor_Humidity.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_Humidity' ktab size: 17, total: 19 (saved 16 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_Humidity[17] = {
  /* K0   */  be_nested_str_weak(attribute_updated),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str_weak(matter),
  /* K3   */  be_nested_str_weak(TLV),
  /* K4   */  be_nested_str_weak(cluster),
  /* K5   */  be_nested_str_weak(attribute),
  /* K6   */  be_nested_str_weak(set_or_nil),
  /* K7   */  be_nested_str_weak(U2),
  /* K8   */  be_nested_str_weak(shadow_value),
  /* K9   */  be_const_int(1),
  /* K10  */  be_nested_str_weak(set),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str_weak(read_attribute),
  /* K13  */  be_nested_str_weak(webserver),
  /* K14  */  be_nested_str_weak(web_values_prefix),
  /* K15  */  be_nested_str_weak(content_send),
  /* K16  */  be_nested_str_weak(_X26_X23x1F4A7_X3B_X20_X252_X2E0f_X25_X25),
};


extern const bclass be_class_Matter_Plugin_Sensor_Humidity;

/********************************************************************
** Solidified function: value_changed
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Humidity_value_changed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Humidity,     /* shared constants */
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


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Humidity_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Humidity,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xB8120400,  //  0000  GETNGBL	R4	K2
      0x88100903,  //  0001  GETMBR	R4	R4	K3
      0x88140504,  //  0002  GETMBR	R5	R2	K4
      0x88180505,  //  0003  GETMBR	R6	R2	K5
      0x541E0404,  //  0004  LDINT	R7	1029
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0018,  //  0006  JMPF	R7	#0020
      0x1C1C0D01,  //  0007  EQ	R7	R6	K1
      0x781E0007,  //  0008  JMPF	R7	#0011
      0x8C1C0706,  //  0009  GETMET	R7	R3	K6
      0x88240907,  //  000A  GETMBR	R9	R4	K7
      0x60280009,  //  000B  GETGBL	R10	G9
      0x882C0108,  //  000C  GETMBR	R11	R0	K8
      0x7C280200,  //  000D  CALL	R10	1
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x7002000E,  //  0010  JMP		#0020
      0x1C1C0D09,  //  0011  EQ	R7	R6	K9
      0x781E0005,  //  0012  JMPF	R7	#0019
      0x8C1C070A,  //  0013  GETMET	R7	R3	K10
      0x88240907,  //  0014  GETMBR	R9	R4	K7
      0x542A01F3,  //  0015  LDINT	R10	500
      0x7C1C0600,  //  0016  CALL	R7	3
      0x80040E00,  //  0017  RET	1	R7
      0x70020006,  //  0018  JMP		#0020
      0x1C1C0D0B,  //  0019  EQ	R7	R6	K11
      0x781E0004,  //  001A  JMPF	R7	#0020
      0x8C1C070A,  //  001B  GETMET	R7	R3	K10
      0x88240907,  //  001C  GETMBR	R9	R4	K7
      0x542A270F,  //  001D  LDINT	R10	10000
      0x7C1C0600,  //  001E  CALL	R7	3
      0x80040E00,  //  001F  RET	1	R7
      0x601C0003,  //  0020  GETGBL	R7	G3
      0x5C200000,  //  0021  MOVE	R8	R0
      0x7C1C0200,  //  0022  CALL	R7	1
      0x8C1C0F0C,  //  0023  GETMET	R7	R7	K12
      0x5C240200,  //  0024  MOVE	R9	R1
      0x5C280400,  //  0025  MOVE	R10	R2
      0x5C2C0600,  //  0026  MOVE	R11	R3
      0x7C1C0800,  //  0027  CALL	R7	4
      0x80040E00,  //  0028  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Humidity_pre_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Humidity,     /* shared constants */
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
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Humidity_web_values,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Humidity,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA4061A00,  //  0000  IMPORT	R1	K13
      0x8C08010E,  //  0001  GETMET	R2	R0	K14
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08030F,  //  0003  GETMET	R2	R1	K15
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140010,  //  0005  LDCONST	R5	K16
      0x88180108,  //  0006  GETMBR	R6	R0	K8
      0x4C1C0000,  //  0007  LDNIL	R7
      0x20180C07,  //  0008  NE	R6	R6	R7
      0x781A0005,  //  0009  JMPF	R6	#0010
      0x6018000A,  //  000A  GETGBL	R6	G10
      0x881C0108,  //  000B  GETMBR	R7	R0	K8
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
** Solidified class: Matter_Plugin_Sensor_Humidity
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_class(Matter_Plugin_Sensor_Humidity,
    0,
    &be_class_Matter_Plugin_Sensor,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(775, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(value_changed, 8), be_const_closure(class_Matter_Plugin_Sensor_Humidity_value_changed_closure) },
        { be_const_key_weak(read_attribute, 7), be_const_closure(class_Matter_Plugin_Sensor_Humidity_read_attribute_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Humidity),
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, -1), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, 1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(1029, 2), be_const_bytes_instance(000000010002FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 0), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak(Humidity) },
        { be_const_key_weak(pre_value, -1), be_const_closure(class_Matter_Plugin_Sensor_Humidity_pre_value_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Sensor_Humidity_web_values_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Humidity) },
        { be_const_key_weak(TYPE, 3), be_nested_str_weak(humidity) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Humidity)
);
/********************************************************************/
/* End of solidification */
