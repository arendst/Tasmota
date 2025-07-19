/* Solidification of Matter_Plugin_3_Sensor_Occupancy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_Occupancy' ktab size: 25, total: 30 (saved 40 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_Occupancy[25] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(cluster),
  /* K3   */  be_nested_str_weak(attribute),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(set_or_nil),
  /* K6   */  be_nested_str_weak(U1),
  /* K7   */  be_nested_str_weak(shadow_bool_value),
  /* K8   */  be_const_int(1),
  /* K9   */  be_nested_str_weak(set),
  /* K10  */  be_const_int(3),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str_weak(read_attribute),
  /* K13  */  be_nested_str_weak(webserver),
  /* K14  */  be_nested_str_weak(web_values_prefix),
  /* K15  */  be_nested_str_weak(content_send),
  /* K16  */  be_nested_str_weak(Occupancy_X25i_X20_X25s),
  /* K17  */  be_nested_str_weak(web_value_onoff),
  /* K18  */  be_nested_str_weak(shadow_occupancy),
  /* K19  */  be_nested_str_weak(get_name),
  /* K20  */  be_nested_str_weak(Switch),
  /* K21  */  be_nested_str_weak(PREFIX),
  /* K22  */  be_nested_str_weak(html_escape),
  /* K23  */  be_nested_str_weak(),
  /* K24  */  be_nested_str_weak(attribute_updated),
};


extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
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
    ( &(const binstruction[39]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0405,  //  0004  LDINT	R7	1030
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0016,  //  0006  JMPF	R7	#001E
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E0005,  //  0008  JMPF	R7	#000F
      0x8C1C0705,  //  0009  GETMET	R7	R3	K5
      0x88240906,  //  000A  GETMBR	R9	R4	K6
      0x88280107,  //  000B  GETMBR	R10	R0	K7
      0x7C1C0600,  //  000C  CALL	R7	3
      0x80040E00,  //  000D  RET	1	R7
      0x7002000E,  //  000E  JMP		#001E
      0x1C1C0D08,  //  000F  EQ	R7	R6	K8
      0x781E0005,  //  0010  JMPF	R7	#0017
      0x8C1C0709,  //  0011  GETMET	R7	R3	K9
      0x88240906,  //  0012  GETMBR	R9	R4	K6
      0x5828000A,  //  0013  LDCONST	R10	K10
      0x7C1C0600,  //  0014  CALL	R7	3
      0x80040E00,  //  0015  RET	1	R7
      0x70020006,  //  0016  JMP		#001E
      0x1C1C0D0B,  //  0017  EQ	R7	R6	K11
      0x781E0004,  //  0018  JMPF	R7	#001E
      0x8C1C0709,  //  0019  GETMET	R7	R3	K9
      0x88240906,  //  001A  GETMBR	R9	R4	K6
      0x58280004,  //  001B  LDCONST	R10	K4
      0x7C1C0600,  //  001C  CALL	R7	3
      0x80040E00,  //  001D  RET	1	R7
      0x601C0003,  //  001E  GETGBL	R7	G3
      0x5C200000,  //  001F  MOVE	R8	R0
      0x7C1C0200,  //  0020  CALL	R7	1
      0x8C1C0F0C,  //  0021  GETMET	R7	R7	K12
      0x5C240200,  //  0022  MOVE	R9	R1
      0x5C280400,  //  0023  MOVE	R10	R2
      0x5C2C0600,  //  0024  MOVE	R11	R3
      0x7C1C0800,  //  0025  CALL	R7	4
      0x80040E00,  //  0026  RET	1	R7
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
      0xA4061A00,  //  0000  IMPORT	R1	K13
      0x8C08010E,  //  0001  GETMET	R2	R0	K14
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08030F,  //  0003  GETMET	R2	R1	K15
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140010,  //  0005  LDCONST	R5	K16
      0x88180107,  //  0006  GETMBR	R6	R0	K7
      0x8C1C0111,  //  0007  GETMET	R7	R0	K17
      0x88240112,  //  0008  GETMBR	R9	R0	K18
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
      0xA4061A00,  //  0000  IMPORT	R1	K13
      0x8C080113,  //  0001  GETMET	R2	R0	K19
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x88100107,  //  0006  GETMBR	R4	R0	K7
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E2803,  //  0008  ADD	R3	K20	R3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C030F,  //  000A  GETMET	R3	R1	K15
      0x60140018,  //  000B  GETGBL	R5	G24
      0x88180115,  //  000C  GETMBR	R6	R0	K21
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C1C0316,  //  000E  GETMET	R7	R1	K22
      0x5C240400,  //  000F  MOVE	R9	R2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020000,  //  0011  JMP		#0013
      0x581C0017,  //  0012  LDCONST	R7	K23
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
      0x8C040118,  //  0000  GETMET	R1	R0	K24
      0x540E0405,  //  0001  LDINT	R3	1030
      0x58100004,  //  0002  LDCONST	R4	K4
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
        { be_const_key_int(263, -1), be_const_int(2) },
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
