/* Solidification of Matter_Plugin_3_Sensor_OnOff.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_OnOff' ktab size: 12, total: 14 (saved 16 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_OnOff[12] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(cluster),
  /* K3   */  be_nested_str_weak(attribute),
  /* K4   */  be_nested_str_weak(update_shadow_lazy),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(set),
  /* K7   */  be_nested_str_weak(BOOL),
  /* K8   */  be_nested_str_weak(shadow_bool_value),
  /* K9   */  be_nested_str_weak(read_attribute),
  /* K10  */  be_nested_str_weak(_X2C_X22OnOff_X22_X3A_X25s),
  /* K11  */  be_nested_str_weak(attribute_updated),
};


extern const bclass be_class_Matter_Plugin_Sensor_OnOff;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_OnOff_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
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
    ( &(const binstruction[25]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0008,  //  0006  JMPF	R7	#0010
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0004,  //  000A  JMPF	R7	#0010
      0x8C1C0706,  //  000B  GETMET	R7	R3	K6
      0x88240907,  //  000C  GETMBR	R9	R4	K7
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x601C0003,  //  0010  GETGBL	R7	G3
      0x5C200000,  //  0011  MOVE	R8	R0
      0x7C1C0200,  //  0012  CALL	R7	1
      0x8C1C0F09,  //  0013  GETMET	R7	R7	K9
      0x5C240200,  //  0014  MOVE	R9	R1
      0x5C280400,  //  0015  MOVE	R10	R2
      0x5C2C0600,  //  0016  MOVE	R11	R3
      0x7C1C0800,  //  0017  CALL	R7	4
      0x80040E00,  //  0018  RET	1	R7
    })
  )
);
/*******************************************************************/


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
      0x5808000A,  //  0001  LDCONST	R2	K10
      0x600C0009,  //  0002  GETGBL	R3	G9
      0x88100108,  //  0003  GETMBR	R4	R0	K8
      0x7C0C0200,  //  0004  CALL	R3	1
      0x7C040400,  //  0005  CALL	R1	2
      0x80040200,  //  0006  RET	1	R1
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
      0x8C04010B,  //  0000  GETMET	R1	R0	K11
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
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, 1), be_const_closure(class_Matter_Plugin_Sensor_OnOff_read_attribute_closure) },
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
        { be_const_key_weak(append_state_json, -1), be_const_closure(class_Matter_Plugin_Sensor_OnOff_append_state_json_closure) },
        { be_const_key_weak(TYPES, 6), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(2128, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(value_updated, -1), be_const_closure(class_Matter_Plugin_Sensor_OnOff_value_updated_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(OnOff_X20Sensor) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(onoff) },
    })),
    be_str_weak(Matter_Plugin_Sensor_OnOff)
);
/********************************************************************/
/* End of solidification */
