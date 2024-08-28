/* Solidification of Matter_Plugin_3_Sensor_Waterleak.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Sensor_Waterleak' ktab size: 16, total: 20 (saved 32 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Sensor_Waterleak[16] = {
  /* K0   */  be_nested_str_weak(shadow_bool_value),
  /* K1   */  be_nested_str_weak(_parse_update_virtual),
  /* K2   */  be_nested_str_weak(Waterleak),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(update_virtual),
  /* K5   */  be_nested_str_weak(webserver),
  /* K6   */  be_nested_str_weak(web_values_prefix),
  /* K7   */  be_nested_str_weak(content_send),
  /* K8   */  be_nested_str_weak(Waterleak_X25i_X20_X25s),
  /* K9   */  be_nested_str_weak(tasmota_switch_index),
  /* K10  */  be_nested_str_weak(web_value_onoff),
  /* K11  */  be_nested_str_weak(get_name),
  /* K12  */  be_nested_str_weak(Switch),
  /* K13  */  be_nested_str_weak(PREFIX),
  /* K14  */  be_nested_str_weak(html_escape),
  /* K15  */  be_nested_str_weak(),
};


extern const bclass be_class_Matter_Plugin_Sensor_Waterleak;

/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Waterleak_update_virtual,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Waterleak,     /* shared constants */
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C080101,  //  0000  GETMET	R2	R0	K1
      0x5C100200,  //  0001  MOVE	R4	R1
      0x58140002,  //  0002  LDCONST	R5	K2
      0x88180100,  //  0003  GETMBR	R6	R0	K0
      0x601C0017,  //  0004  GETGBL	R7	G23
      0x54220044,  //  0005  LDINT	R8	69
      0x58240003,  //  0006  LDCONST	R9	K3
      0x7C080E00,  //  0007  CALL	R2	7
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x60080003,  //  0009  GETGBL	R2	G3
      0x5C0C0000,  //  000A  MOVE	R3	R0
      0x7C080200,  //  000B  CALL	R2	1
      0x8C080504,  //  000C  GETMET	R2	R2	K4
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Waterleak_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Waterleak,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xB8120A00,  //  0000  GETNGBL	R4	K5
      0x88100906,  //  0001  GETMBR	R4	R4	K6
      0x88140507,  //  0002  GETMBR	R5	R2	K7
      0x88180508,  //  0003  GETMBR	R6	R2	K8
      0x541E0044,  //  0004  LDINT	R7	69
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0006,  //  0006  JMPF	R7	#000E
      0x1C1C0D03,  //  0007  EQ	R7	R6	K3
      0x781E0004,  //  0008  JMPF	R7	#000E
      0x8C1C0709,  //  0009  GETMET	R7	R3	K9
      0x8824090A,  //  000A  GETMBR	R9	R4	K10
      0x88280100,  //  000B  GETMBR	R10	R0	K0
      0x7C1C0600,  //  000C  CALL	R7	3
      0x80040E00,  //  000D  RET	1	R7
      0x601C0003,  //  000E  GETGBL	R7	G3
      0x5C200000,  //  000F  MOVE	R8	R0
      0x7C1C0200,  //  0010  CALL	R7	1
      0x8C1C0F0B,  //  0011  GETMET	R7	R7	K11
      0x5C240200,  //  0012  MOVE	R9	R1
      0x5C280400,  //  0013  MOVE	R10	R2
      0x5C2C0600,  //  0014  MOVE	R11	R3
      0x7C1C0800,  //  0015  CALL	R7	4
      0x80040E00,  //  0016  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Waterleak_web_values,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Waterleak,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA4060A00,  //  0000  IMPORT	R1	K5
      0x8C080106,  //  0001  GETMET	R2	R0	K6
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080307,  //  0003  GETMET	R2	R1	K7
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140008,  //  0005  LDCONST	R5	K8
      0x88180109,  //  0006  GETMBR	R6	R0	K9
      0x8C1C010A,  //  0007  GETMET	R7	R0	K10
      0x88240100,  //  0008  GETMBR	R9	R0	K0
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
be_local_closure(class_Matter_Plugin_Sensor_Waterleak_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Waterleak,     /* shared constants */
    be_str_weak(web_values_prefix),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060A00,  //  0000  IMPORT	R1	K5
      0x8C08010B,  //  0001  GETMET	R2	R0	K11
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x88100109,  //  0006  GETMBR	R4	R0	K9
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E1803,  //  0008  ADD	R3	K12	R3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C0307,  //  000A  GETMET	R3	R1	K7
      0x60140018,  //  000B  GETGBL	R5	G24
      0x8818010D,  //  000C  GETMBR	R6	R0	K13
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C1C030E,  //  000E  GETMET	R7	R1	K14
      0x5C240400,  //  000F  MOVE	R9	R2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020000,  //  0011  JMP		#0013
      0x581C000F,  //  0012  LDCONST	R7	K15
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
be_local_closure(class_Matter_Plugin_Sensor_Waterleak_value_updated,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Sensor_Waterleak,     /* shared constants */
    be_str_weak(value_updated),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040117,  //  0000  GETMET	R1	R0	K23
      0x540E0044,  //  0001  LDINT	R3	69
      0x58100003,  //  0002  LDCONST	R4	K3
      0x7C040600,  //  0003  CALL	R1	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_Waterleak
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Boolean;
be_local_class(Matter_Plugin_Sensor_Waterleak,
    0,
    &be_class_Matter_Plugin_Sensor_Boolean,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(update_virtual, 6), be_const_closure(class_Matter_Plugin_Sensor_Waterleak_update_virtual_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Waterleak) },
        { be_const_key_weak(read_attribute, 5), be_const_closure(class_Matter_Plugin_Sensor_Waterleak_read_attribute_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Waterleak),
    }))    ) } )) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, -1), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, 1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(69, 2), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 0), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Sensor_Waterleak_web_values_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(67, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(class_Matter_Plugin_Sensor_Waterleak_web_values_prefix_closure) },
        { be_const_key_weak(value_updated, -1), be_const_closure(class_Matter_Plugin_Sensor_Waterleak_value_updated_closure) },
        { be_const_key_weak(TYPE, 3), be_nested_str_weak(waterleak) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Waterleak)
);
/********************************************************************/
/* End of solidification */
