/* Solidification of Matter_Plugin_3_Sensor_Occupancy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_Occupancy' ktab size: 22, total: 27 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_Occupancy[22] = {
  /* K0   */  be_nested_str_weak(cluster),
  /* K1   */  be_nested_str_weak(attribute),
  /* K2   */  be_const_int(0),
  /* K3   */  be_nested_str_weak(set_or_nil),
  /* K4   */  be_nested_str_weak(shadow_bool_value),
  /* K5   */  be_const_int(1),
  /* K6   */  be_nested_str_weak(set),
  /* K7   */  be_const_int(3),
  /* K8   */  be_const_int(2),
  /* K9   */  be_nested_str_weak(read_attribute),
  /* K10  */  be_nested_str_weak(webserver),
  /* K11  */  be_nested_str_weak(web_values_prefix),
  /* K12  */  be_nested_str_weak(content_send),
  /* K13  */  be_nested_str_weak(Occupancy_X25i_X20_X25s),
  /* K14  */  be_nested_str_weak(web_value_onoff),
  /* K15  */  be_nested_str_weak(shadow_occupancy),
  /* K16  */  be_nested_str_weak(get_name),
  /* K17  */  be_nested_str_weak(Switch),
  /* K18  */  be_nested_str_weak(PREFIX),
  /* K19  */  be_nested_str_weak(html_escape),
  /* K20  */  be_nested_str_weak(),
  /* K21  */  be_nested_str_weak(attribute_updated),
};


extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Occupancy,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x88100500,  //  0000  GETMBR	R4	R2	K0
      0x88140501,  //  0001  GETMBR	R5	R2	K1
      0x541A0405,  //  0002  LDINT	R6	1030
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A0016,  //  0004  JMPF	R6	#001C
      0x1C180B02,  //  0005  EQ	R6	R5	K2
      0x781A0005,  //  0006  JMPF	R6	#000D
      0x8C180703,  //  0007  GETMET	R6	R3	K3
      0x54220003,  //  0008  LDINT	R8	4
      0x88240104,  //  0009  GETMBR	R9	R0	K4
      0x7C180600,  //  000A  CALL	R6	3
      0x80040C00,  //  000B  RET	1	R6
      0x7002000E,  //  000C  JMP		#001C
      0x1C180B05,  //  000D  EQ	R6	R5	K5
      0x781A0005,  //  000E  JMPF	R6	#0015
      0x8C180706,  //  000F  GETMET	R6	R3	K6
      0x54220003,  //  0010  LDINT	R8	4
      0x58240007,  //  0011  LDCONST	R9	K7
      0x7C180600,  //  0012  CALL	R6	3
      0x80040C00,  //  0013  RET	1	R6
      0x70020006,  //  0014  JMP		#001C
      0x1C180B08,  //  0015  EQ	R6	R5	K8
      0x781A0004,  //  0016  JMPF	R6	#001C
      0x8C180706,  //  0017  GETMET	R6	R3	K6
      0x54220003,  //  0018  LDINT	R8	4
      0x58240002,  //  0019  LDCONST	R9	K2
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
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_web_values,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Occupancy,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA4061400,  //  0000  IMPORT	R1	K10
      0x8C08010B,  //  0001  GETMET	R2	R0	K11
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08030C,  //  0003  GETMET	R2	R1	K12
      0x60100018,  //  0004  GETGBL	R4	G24
      0x5814000D,  //  0005  LDCONST	R5	K13
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x8C1C010E,  //  0007  GETMET	R7	R0	K14
      0x8824010F,  //  0008  GETMBR	R9	R0	K15
      0x7C1C0400,  //  0009  CALL	R7	2
      0x7C100600,  //  000A  CALL	R4	3
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values_prefix
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Occupancy,     /* shared constants */
    be_str_weak(web_values_prefix),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4061400,  //  0000  IMPORT	R1	K10
      0x8C080110,  //  0001  GETMET	R2	R0	K16
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x88100104,  //  0006  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E2203,  //  0008  ADD	R3	K17	R3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C030C,  //  000A  GETMET	R3	R1	K12
      0x60140018,  //  000B  GETGBL	R5	G24
      0x88180112,  //  000C  GETMBR	R6	R0	K18
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C1C0313,  //  000E  GETMET	R7	R1	K19
      0x5C240400,  //  000F  MOVE	R9	R2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020000,  //  0011  JMP		#0013
      0x581C0014,  //  0012  LDCONST	R7	K20
      0x7C140400,  //  0013  CALL	R5	2
      0x7C0C0400,  //  0014  CALL	R3	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: value_updated
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_value_updated,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Occupancy,     /* shared constants */
    be_str_weak(value_updated),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040115,  //  0000  GETMET	R1	R0	K21
      0x540E0405,  //  0001  LDINT	R3	1030
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C040600,  //  0003  CALL	R1	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_Occupancy
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Boolean;
be_local_class(Matter_Plugin_Sensor_Occupancy,
    0,
    &be_class_Matter_Plugin_Sensor_Boolean,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(263, -1), be_const_int(4) },
    }))    ) } )) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Occupancy) },
        { be_const_key_weak(read_attribute, 6), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_read_attribute_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Occupancy),
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, 2), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(1030, 1), be_const_bytes_instance(000000010002FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 0), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak(Occupancy) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_web_values_closure) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_web_values_prefix_closure) },
        { be_const_key_weak(value_updated, -1), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_value_updated_closure) },
        { be_const_key_weak(TYPE, 3), be_nested_str_weak(occupancy) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Occupancy)
);
/********************************************************************/
/* End of solidification */
