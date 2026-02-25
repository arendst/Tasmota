/* Solidification of Matter_Plugin_3_Sensor_OnOff.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_OnOff' ktab size: 9, total: 11 (saved 16 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_OnOff[9] = {
  /* K0   */  be_nested_str_weak(_X2C_X22OnOff_X22_X3A_X25s),
  /* K1   */  be_nested_str_weak(shadow_bool_value),
  /* K2   */  be_nested_str_weak(cluster),
  /* K3   */  be_nested_str_weak(attribute),
  /* K4   */  be_nested_str_weak(update_shadow_lazy),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(set),
  /* K7   */  be_nested_str_weak(read_attribute),
  /* K8   */  be_nested_str_weak(attribute_updated),
};


extern const bclass be_class_Matter_Plugin_Sensor_OnOff;

/********************************************************************
** Solidified function: append_state_json
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_OnOff_append_state_json,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_OnOff,     /* shared constants */
    be_str_weak(append_state_json),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080000,  //  0001  LDCONST	R2	K0
      0x600C0009,  //  0002  GETGBL	R3	G9
      0x88100101,  //  0003  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0004  CALL	R3	1
      0x7C040400,  //  0005  CALL	R1	2
      0x80040200,  //  0006  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_OnOff_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_OnOff,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88100502,  //  0000  GETMBR	R4	R2	K2
      0x88140503,  //  0001  GETMBR	R5	R2	K3
      0x541A0005,  //  0002  LDINT	R6	6
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A0008,  //  0004  JMPF	R6	#000E
      0x8C180104,  //  0005  GETMET	R6	R0	K4
      0x7C180200,  //  0006  CALL	R6	1
      0x1C180B05,  //  0007  EQ	R6	R5	K5
      0x781A0004,  //  0008  JMPF	R6	#000E
      0x8C180706,  //  0009  GETMET	R6	R3	K6
      0x54220007,  //  000A  LDINT	R8	8
      0x88240101,  //  000B  GETMBR	R9	R0	K1
      0x7C180600,  //  000C  CALL	R6	3
      0x80040C00,  //  000D  RET	1	R6
      0x60180003,  //  000E  GETGBL	R6	G3
      0x5C1C0000,  //  000F  MOVE	R7	R0
      0x7C180200,  //  0010  CALL	R6	1
      0x8C180D07,  //  0011  GETMET	R6	R6	K7
      0x5C200200,  //  0012  MOVE	R8	R1
      0x5C240400,  //  0013  MOVE	R9	R2
      0x5C280600,  //  0014  MOVE	R10	R3
      0x7C180800,  //  0015  CALL	R6	4
      0x80040C00,  //  0016  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: value_updated
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_OnOff_value_updated,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_OnOff,     /* shared constants */
    be_str_weak(value_updated),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040108,  //  0000  GETMET	R1	R0	K8
      0x540E0005,  //  0001  LDINT	R3	6
      0x58100005,  //  0002  LDCONST	R4	K5
      0x7C040600,  //  0003  CALL	R1	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_OnOff
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Boolean;
be_local_class(Matter_Plugin_Sensor_OnOff,
    0,
    &be_class_Matter_Plugin_Sensor_Boolean,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(OnOff_X20Sensor) },
        { be_const_key_weak(append_state_json, 7), be_const_closure(class_Matter_Plugin_Sensor_OnOff_append_state_json_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(2128, -1), be_const_int(3) },
    }))    ) } )) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(OnOff),
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(6, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, 2), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 1), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak(OnOff) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Sensor_OnOff_read_attribute_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(onoff) },
        { be_const_key_weak(value_updated, -1), be_const_closure(class_Matter_Plugin_Sensor_OnOff_value_updated_closure) },
    })),
    be_str_weak(Matter_Plugin_Sensor_OnOff)
);
/********************************************************************/
/* End of solidification */
