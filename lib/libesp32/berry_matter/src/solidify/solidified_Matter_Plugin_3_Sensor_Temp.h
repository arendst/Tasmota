/* Solidification of Matter_Plugin_3_Sensor_Temp.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_Temp' ktab size: 20, total: 23 (saved 24 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_Temp[20] = {
  /* K0   */  be_nested_str_weak(attribute_updated),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str_weak(cluster),
  /* K3   */  be_nested_str_weak(attribute),
  /* K4   */  be_nested_str_weak(set_or_nil),
  /* K5   */  be_const_int(1),
  /* K6   */  be_nested_str_weak(shadow_value),
  /* K7   */  be_nested_str_weak(set),
  /* K8   */  be_const_int(2),
  /* K9   */  be_nested_str_weak(read_attribute),
  /* K10  */  be_nested_str_weak(BRIDGE),
  /* K11  */  be_nested_str_weak(temp_unit),
  /* K12  */  be_nested_str_weak(TEMP_F),
  /* K13  */  be_const_real_hex(0x3FE66666),
  /* K14  */  be_nested_str_weak(tasmota),
  /* K15  */  be_nested_str_weak(get_option),
  /* K16  */  be_nested_str_weak(webserver),
  /* K17  */  be_nested_str_weak(web_values_prefix),
  /* K18  */  be_nested_str_weak(content_send),
  /* K19  */  be_nested_str_weak(_X26_X23x2600_X3B_X26_X23xFE0F_X3B_X20_X25_X2E1f_X20_XC2_XB0C),
};


extern const bclass be_class_Matter_Plugin_Sensor_Temp;

/********************************************************************
** Solidified function: value_changed
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Temp_value_changed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Temp,     /* shared constants */
    be_str_weak(value_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0401,  //  0001  LDINT	R3	1026
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
be_local_closure(class_Matter_Plugin_Sensor_Temp_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Temp,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x88100502,  //  0000  GETMBR	R4	R2	K2
      0x88140503,  //  0001  GETMBR	R5	R2	K3
      0x541A0401,  //  0002  LDINT	R6	1026
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A0016,  //  0004  JMPF	R6	#001C
      0x1C180B01,  //  0005  EQ	R6	R5	K1
      0x781A0005,  //  0006  JMPF	R6	#000D
      0x8C180704,  //  0007  GETMET	R6	R3	K4
      0x58200005,  //  0008  LDCONST	R8	K5
      0x88240106,  //  0009  GETMBR	R9	R0	K6
      0x7C180600,  //  000A  CALL	R6	3
      0x80040C00,  //  000B  RET	1	R6
      0x7002000E,  //  000C  JMP		#001C
      0x1C180B05,  //  000D  EQ	R6	R5	K5
      0x781A0005,  //  000E  JMPF	R6	#0015
      0x8C180707,  //  000F  GETMET	R6	R3	K7
      0x58200005,  //  0010  LDCONST	R8	K5
      0x5425EC77,  //  0011  LDINT	R9	-5000
      0x7C180600,  //  0012  CALL	R6	3
      0x80040C00,  //  0013  RET	1	R6
      0x70020006,  //  0014  JMP		#001C
      0x1C180B08,  //  0015  EQ	R6	R5	K8
      0x781A0004,  //  0016  JMPF	R6	#001C
      0x8C180707,  //  0017  GETMET	R6	R3	K7
      0x58200005,  //  0018  LDCONST	R8	K5
      0x54263A97,  //  0019  LDINT	R9	15000
      0x7C180600,  //  001A  CALL	R6	3
      0x80040C00,  //  001B  RET	1	R6
      0x60180003,  //  001C  GETGBL	R6	G3
      0x5C1C0000,  //  001D  MOVE	R7	R0
      0x7C180200,  //  001E  CALL	R6	1
      0x8C180D09,  //  001F  GETMET	R6	R6	K9
      0x5C200200,  //  0020  MOVE	R8	R1
      0x5C240400,  //  0021  MOVE	R9	R2
      0x5C280600,  //  0022  MOVE	R10	R3
      0x7C180800,  //  0023  CALL	R6	4
      0x80040C00,  //  0024  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Temp_pre_value,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Temp,     /* shared constants */
    be_str_weak(pre_value),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x8808010A,  //  0000  GETMBR	R2	R0	K10
      0x780A0012,  //  0001  JMPF	R2	#0015
      0x8808010B,  //  0002  GETMBR	R2	R0	K11
      0x880C010C,  //  0003  GETMBR	R3	R0	K12
      0x1C080403,  //  0004  EQ	R2	R2	R3
      0x780A0003,  //  0005  JMPF	R2	#000A
      0x540A001F,  //  0006  LDINT	R2	32
      0x04080202,  //  0007  SUB	R2	R1	R2
      0x0C08050D,  //  0008  DIV	R2	R2	K13
      0x5C040400,  //  0009  MOVE	R1	R2
      0x4C080000,  //  000A  LDNIL	R2
      0x20080202,  //  000B  NE	R2	R1	R2
      0x780A0004,  //  000C  JMPF	R2	#0012
      0x60080009,  //  000D  GETGBL	R2	G9
      0x540E0063,  //  000E  LDINT	R3	100
      0x080C0203,  //  000F  MUL	R3	R1	R3
      0x7C080200,  //  0010  CALL	R2	1
      0x70020000,  //  0011  JMP		#0013
      0x4C080000,  //  0012  LDNIL	R2
      0x80040400,  //  0013  RET	1	R2
      0x70020013,  //  0014  JMP		#0029
      0xB80A1C00,  //  0015  GETNGBL	R2	K14
      0x8C08050F,  //  0016  GETMET	R2	R2	K15
      0x54120007,  //  0017  LDINT	R4	8
      0x7C080400,  //  0018  CALL	R2	2
      0x1C080505,  //  0019  EQ	R2	R2	K5
      0x780A0003,  //  001A  JMPF	R2	#001F
      0x540A001F,  //  001B  LDINT	R2	32
      0x04080202,  //  001C  SUB	R2	R1	R2
      0x0C08050D,  //  001D  DIV	R2	R2	K13
      0x5C040400,  //  001E  MOVE	R1	R2
      0x4C080000,  //  001F  LDNIL	R2
      0x20080202,  //  0020  NE	R2	R1	R2
      0x780A0004,  //  0021  JMPF	R2	#0027
      0x60080009,  //  0022  GETGBL	R2	G9
      0x540E0063,  //  0023  LDINT	R3	100
      0x080C0203,  //  0024  MUL	R3	R1	R3
      0x7C080200,  //  0025  CALL	R2	1
      0x70020000,  //  0026  JMP		#0028
      0x4C080000,  //  0027  LDNIL	R2
      0x80040400,  //  0028  RET	1	R2
      0x80000000,  //  0029  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Temp_web_values,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Temp,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA4062000,  //  0000  IMPORT	R1	K16
      0x8C080111,  //  0001  GETMET	R2	R0	K17
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080312,  //  0003  GETMET	R2	R1	K18
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140013,  //  0005  LDCONST	R5	K19
      0x88180106,  //  0006  GETMBR	R6	R0	K6
      0x4C1C0000,  //  0007  LDNIL	R7
      0x20180C07,  //  0008  NE	R6	R6	R7
      0x781A0005,  //  0009  JMPF	R6	#0010
      0x6018000A,  //  000A  GETGBL	R6	G10
      0x881C0106,  //  000B  GETMBR	R7	R0	K6
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
** Solidified class: Matter_Plugin_Sensor_Temp
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_class(Matter_Plugin_Sensor_Temp,
    0,
    &be_class_Matter_Plugin_Sensor,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(770, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(value_changed, 8), be_const_closure(class_Matter_Plugin_Sensor_Temp_value_changed_closure) },
        { be_const_key_weak(read_attribute, 7), be_const_closure(class_Matter_Plugin_Sensor_Temp_read_attribute_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Temperature),
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(1026, -1), be_const_bytes_instance(000000010002FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, 2), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 1), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak(Temperature) },
        { be_const_key_weak(pre_value, -1), be_const_closure(class_Matter_Plugin_Sensor_Temp_pre_value_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Sensor_Temp_web_values_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Temperature) },
        { be_const_key_weak(TYPE, 3), be_nested_str_weak(temperature) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Temp)
);
/********************************************************************/
/* End of solidification */
