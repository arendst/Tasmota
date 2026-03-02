/* Solidification of Matter_Plugin_3_Sensor_Illuminance.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_Illuminance' ktab size: 16, total: 20 (saved 32 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_Illuminance[16] = {
  /* K0   */  be_nested_str_weak(attribute_updated),
  /* K1   */  be_const_int(0),
  /* K2   */  be_nested_str_weak(cluster),
  /* K3   */  be_nested_str_weak(attribute),
  /* K4   */  be_nested_str_weak(set_or_nil),
  /* K5   */  be_nested_str_weak(shadow_value),
  /* K6   */  be_const_int(1),
  /* K7   */  be_nested_str_weak(set),
  /* K8   */  be_const_int(2),
  /* K9   */  be_nested_str_weak(read_attribute),
  /* K10  */  be_nested_str_weak(math),
  /* K11  */  be_nested_str_weak(log10),
  /* K12  */  be_nested_str_weak(webserver),
  /* K13  */  be_nested_str_weak(web_values_prefix),
  /* K14  */  be_nested_str_weak(content_send),
  /* K15  */  be_nested_str_weak(_X26_X23128261_X3B_X20_X25i_X20lux),
};


extern const bclass be_class_Matter_Plugin_Sensor_Illuminance;

/********************************************************************
** Solidified function: value_changed
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Illuminance_value_changed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Illuminance,     /* shared constants */
    be_str_weak(value_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E03FF,  //  0001  LDINT	R3	1024
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
be_local_closure(class_Matter_Plugin_Sensor_Illuminance_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Illuminance,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x88100502,  //  0000  GETMBR	R4	R2	K2
      0x88140503,  //  0001  GETMBR	R5	R2	K3
      0x541A03FF,  //  0002  LDINT	R6	1024
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A0018,  //  0004  JMPF	R6	#001E
      0x1C180B01,  //  0005  EQ	R6	R5	K1
      0x781A0007,  //  0006  JMPF	R6	#000F
      0x8C180704,  //  0007  GETMET	R6	R3	K4
      0x54220004,  //  0008  LDINT	R8	5
      0x60240009,  //  0009  GETGBL	R9	G9
      0x88280105,  //  000A  GETMBR	R10	R0	K5
      0x7C240200,  //  000B  CALL	R9	1
      0x7C180600,  //  000C  CALL	R6	3
      0x80040C00,  //  000D  RET	1	R6
      0x7002000E,  //  000E  JMP		#001E
      0x1C180B06,  //  000F  EQ	R6	R5	K6
      0x781A0005,  //  0010  JMPF	R6	#0017
      0x8C180707,  //  0011  GETMET	R6	R3	K7
      0x54220004,  //  0012  LDINT	R8	5
      0x58240006,  //  0013  LDCONST	R9	K6
      0x7C180600,  //  0014  CALL	R6	3
      0x80040C00,  //  0015  RET	1	R6
      0x70020006,  //  0016  JMP		#001E
      0x1C180B08,  //  0017  EQ	R6	R5	K8
      0x781A0004,  //  0018  JMPF	R6	#001E
      0x8C180707,  //  0019  GETMET	R6	R3	K7
      0x54220004,  //  001A  LDINT	R8	5
      0x5426FFFD,  //  001B  LDINT	R9	65534
      0x7C180600,  //  001C  CALL	R6	3
      0x80040C00,  //  001D  RET	1	R6
      0x60180003,  //  001E  GETGBL	R6	G3
      0x5C1C0000,  //  001F  MOVE	R7	R0
      0x7C180200,  //  0020  CALL	R6	1
      0x8C180D09,  //  0021  GETMET	R6	R6	K9
      0x5C200200,  //  0022  MOVE	R8	R1
      0x5C240400,  //  0023  MOVE	R9	R2
      0x5C280600,  //  0024  MOVE	R10	R3
      0x7C180800,  //  0025  CALL	R6	4
      0x80040C00,  //  0026  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Illuminance_pre_value,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Illuminance,     /* shared constants */
    be_str_weak(pre_value),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xA40A1400,  //  0005  IMPORT	R2	K10
      0x140C0301,  //  0006  LT	R3	R1	K1
      0x780E0001,  //  0007  JMPF	R3	#000A
      0x80060200,  //  0008  RET	1	K1
      0x70020005,  //  0009  JMP		#0010
      0x8C0C050B,  //  000A  GETMET	R3	R2	K11
      0x00140306,  //  000B  ADD	R5	R1	K6
      0x7C0C0400,  //  000C  CALL	R3	2
      0x5412270F,  //  000D  LDINT	R4	10000
      0x080C0604,  //  000E  MUL	R3	R3	R4
      0x80040600,  //  000F  RET	1	R3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Illuminance_web_values,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Illuminance,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA4061800,  //  0000  IMPORT	R1	K12
      0x8C08010D,  //  0001  GETMET	R2	R0	K13
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08030E,  //  0003  GETMET	R2	R1	K14
      0x60100018,  //  0004  GETGBL	R4	G24
      0x5814000F,  //  0005  LDCONST	R5	K15
      0x60180009,  //  0006  GETGBL	R6	G9
      0x881C0105,  //  0007  GETMBR	R7	R0	K5
      0x7C180200,  //  0008  CALL	R6	1
      0x7C100400,  //  0009  CALL	R4	2
      0x7C080400,  //  000A  CALL	R2	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_Illuminance
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_class(Matter_Plugin_Sensor_Illuminance,
    0,
    &be_class_Matter_Plugin_Sensor,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(262, -1), be_const_int(3) },
    }))    ) } )) },
        { be_const_key_weak(value_changed, 8), be_const_closure(class_Matter_Plugin_Sensor_Illuminance_value_changed_closure) },
        { be_const_key_weak(read_attribute, 7), be_const_closure(class_Matter_Plugin_Sensor_Illuminance_read_attribute_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Illuminance),
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, 2), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(1024, 1), be_const_bytes_instance(000000010002FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 0), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak(Illuminance) },
        { be_const_key_weak(pre_value, -1), be_const_closure(class_Matter_Plugin_Sensor_Illuminance_pre_value_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Sensor_Illuminance_web_values_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Illuminance) },
        { be_const_key_weak(TYPE, 3), be_nested_str_weak(illuminance) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Illuminance)
);
/********************************************************************/
/* End of solidification */
