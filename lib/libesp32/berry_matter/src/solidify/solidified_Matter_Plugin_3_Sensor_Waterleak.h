/* Solidification of Matter_Plugin_3_Sensor_Waterleak.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Waterleak;

/********************************************************************
** Solidified function: update_virtual
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Waterleak;
be_local_closure(class_Matter_Plugin_Sensor_Waterleak_update_virtual,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Waterleak, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(shadow_bool_value),
    /* K1   */  be_nested_str_weak(_parse_update_virtual),
    /* K2   */  be_nested_str_weak(Waterleak),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(update_virtual),
    }),
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

// Borrowed method 'read_attribute' from class 'class_Matter_Plugin_Sensor_Rain'
extern bclosure *class_Matter_Plugin_Sensor_Rain_read_attribute;

/********************************************************************
** Solidified function: web_values
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Waterleak;
be_local_closure(class_Matter_Plugin_Sensor_Waterleak_web_values,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Waterleak, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(Waterleak_X25i_X20_X25s),
    /* K4   */  be_nested_str_weak(tasmota_switch_index),
    /* K5   */  be_nested_str_weak(web_value_onoff),
    /* K6   */  be_nested_str_weak(shadow_bool_value),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140003,  //  0005  LDCONST	R5	K3
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x8C1C0105,  //  0007  GETMET	R7	R0	K5
      0x88240106,  //  0008  GETMBR	R9	R0	K6
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
extern const bclass be_class_Matter_Plugin_Sensor_Waterleak;
be_local_closure(class_Matter_Plugin_Sensor_Waterleak_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Waterleak, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(get_name),
    /* K2   */  be_nested_str_weak(Switch),
    /* K3   */  be_nested_str_weak(tasmota_switch_index),
    /* K4   */  be_nested_str_weak(content_send),
    /* K5   */  be_nested_str_weak(PREFIX),
    /* K6   */  be_nested_str_weak(html_escape),
    /* K7   */  be_nested_str_weak(),
    }),
    be_str_weak(web_values_prefix),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E0403,  //  0008  ADD	R3	K2	R3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C0304,  //  000A  GETMET	R3	R1	K4
      0x60140018,  //  000B  GETGBL	R5	G24
      0x88180105,  //  000C  GETMBR	R6	R0	K5
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C1C0306,  //  000E  GETMET	R7	R1	K6
      0x5C240400,  //  000F  MOVE	R9	R2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020000,  //  0011  JMP		#0013
      0x581C0007,  //  0012  LDCONST	R7	K7
      0x7C140400,  //  0013  CALL	R5	2
      0x7C0C0400,  //  0014  CALL	R3	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/

// Borrowed method 'value_updated' from class 'class_Matter_Plugin_Sensor_Rain'
extern bclosure *class_Matter_Plugin_Sensor_Rain_value_updated;

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
        { be_const_key_weak(read_attribute, 5), be_const_closure(class_Matter_Plugin_Sensor_Rain_read_attribute_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Waterleak),
    }))    ) } )) },
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
        { be_const_key_int(69, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Sensor_Waterleak_web_values_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(67, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(class_Matter_Plugin_Sensor_Waterleak_web_values_prefix_closure) },
        { be_const_key_weak(value_updated, -1), be_const_closure(class_Matter_Plugin_Sensor_Rain_value_updated_closure) },
        { be_const_key_weak(TYPE, 3), be_nested_str_weak(waterleak) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Waterleak)
);
/********************************************************************/
/* End of solidification */
