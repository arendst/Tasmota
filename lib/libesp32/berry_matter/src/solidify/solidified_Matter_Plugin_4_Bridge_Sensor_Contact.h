/* Solidification of Matter_Plugin_4_Bridge_Sensor_Contact.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Contact;

/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Contact__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(_X3Clambda_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040009,  //  0000  GETGBL	R1	G9
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Contact_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(tasmota_switch_index),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(ARG),
    /* K4   */  be_const_int(1),
    /* K5   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x60100009,  //  0008  GETGBL	R4	G9
      0x8C140702,  //  0009  GETMET	R5	R3	K2
      0x881C0103,  //  000A  GETMBR	R7	R0	K3
      0x58200004,  //  000B  LDCONST	R8	K4
      0x7C140600,  //  000C  CALL	R5	3
      0x7C100200,  //  000D  CALL	R4	1
      0x90020204,  //  000E  SETMBR	R0	K1	R4
      0x88100101,  //  000F  GETMBR	R4	R0	K1
      0x18100905,  //  0010  LE	R4	R4	K5
      0x78120000,  //  0011  JMPF	R4	#0013
      0x90020304,  //  0012  SETMBR	R0	K1	K4
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values_prefix
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Contact_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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


/********************************************************************
** Solidified function: parse_update
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Contact_parse_update,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(Switch),
    /* K2   */  be_nested_str_weak(tasmota_switch_index),
    /* K3   */  be_nested_str_weak(ON),
    /* K4   */  be_nested_str_weak(shadow_contact),
    /* K5   */  be_nested_str_weak(attribute_updated),
    /* K6   */  be_const_int(0),
    }),
    be_str_weak(parse_update),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x540E0007,  //  0000  LDINT	R3	8
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0017,  //  0002  JMPF	R3	#001B
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x8C100300,  //  0004  GETMET	R4	R1	K0
      0x60180008,  //  0005  GETGBL	R6	G8
      0x881C0102,  //  0006  GETMBR	R7	R0	K2
      0x7C180200,  //  0007  CALL	R6	1
      0x001A0206,  //  0008  ADD	R6	K1	R6
      0x7C100400,  //  0009  CALL	R4	2
      0x1C100903,  //  000A  EQ	R4	R4	K3
      0x5C0C0800,  //  000B  MOVE	R3	R4
      0x88100104,  //  000C  GETMBR	R4	R0	K4
      0x4C140000,  //  000D  LDNIL	R5
      0x20100805,  //  000E  NE	R4	R4	R5
      0x78120009,  //  000F  JMPF	R4	#001A
      0x88100104,  //  0010  GETMBR	R4	R0	K4
      0x60140017,  //  0011  GETGBL	R5	G23
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C140200,  //  0013  CALL	R5	1
      0x20100805,  //  0014  NE	R4	R4	R5
      0x78120003,  //  0015  JMPF	R4	#001A
      0x8C100105,  //  0016  GETMET	R4	R0	K5
      0x541A0044,  //  0017  LDINT	R6	69
      0x581C0006,  //  0018  LDCONST	R7	K6
      0x7C100600,  //  0019  CALL	R4	3
      0x90020803,  //  001A  SETMBR	R0	K4	R3
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Contact_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(shadow_contact),
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(BOOL),
    /* K8   */  be_nested_str_weak(NULL),
    /* K9   */  be_nested_str_weak(U4),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0044,  //  0004  LDINT	R7	69
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0023,  //  0006  JMPF	R7	#002B
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E000F,  //  0008  JMPF	R7	#0019
      0x881C0105,  //  0009  GETMBR	R7	R0	K5
      0x4C200000,  //  000A  LDNIL	R8
      0x201C0E08,  //  000B  NE	R7	R7	R8
      0x781E0005,  //  000C  JMPF	R7	#0013
      0x8C1C0706,  //  000D  GETMET	R7	R3	K6
      0x88240907,  //  000E  GETMBR	R9	R4	K7
      0x88280105,  //  000F  GETMBR	R10	R0	K5
      0x7C1C0600,  //  0010  CALL	R7	3
      0x80040E00,  //  0011  RET	1	R7
      0x70020004,  //  0012  JMP		#0018
      0x8C1C0706,  //  0013  GETMET	R7	R3	K6
      0x88240908,  //  0014  GETMBR	R9	R4	K8
      0x4C280000,  //  0015  LDNIL	R10
      0x7C1C0600,  //  0016  CALL	R7	3
      0x80040E00,  //  0017  RET	1	R7
      0x70020010,  //  0018  JMP		#002A
      0x541EFFFB,  //  0019  LDINT	R7	65532
      0x1C1C0C07,  //  001A  EQ	R7	R6	R7
      0x781E0005,  //  001B  JMPF	R7	#0022
      0x8C1C0706,  //  001C  GETMET	R7	R3	K6
      0x88240909,  //  001D  GETMBR	R9	R4	K9
      0x58280004,  //  001E  LDCONST	R10	K4
      0x7C1C0600,  //  001F  CALL	R7	3
      0x80040E00,  //  0020  RET	1	R7
      0x70020007,  //  0021  JMP		#002A
      0x541EFFFC,  //  0022  LDINT	R7	65533
      0x1C1C0C07,  //  0023  EQ	R7	R6	R7
      0x781E0004,  //  0024  JMPF	R7	#002A
      0x8C1C0706,  //  0025  GETMET	R7	R3	K6
      0x88240909,  //  0026  GETMBR	R9	R4	K9
      0x5828000A,  //  0027  LDCONST	R10	K10
      0x7C1C0600,  //  0028  CALL	R7	3
      0x80040E00,  //  0029  RET	1	R7
      0x70020008,  //  002A  JMP		#0034
      0x601C0003,  //  002B  GETGBL	R7	G3
      0x5C200000,  //  002C  MOVE	R8	R0
      0x7C1C0200,  //  002D  CALL	R7	1
      0x8C1C0F0B,  //  002E  GETMET	R7	R7	K11
      0x5C240200,  //  002F  MOVE	R9	R1
      0x5C280400,  //  0030  MOVE	R10	R2
      0x5C2C0600,  //  0031  MOVE	R11	R3
      0x7C1C0800,  //  0032  CALL	R7	4
      0x80040E00,  //  0033  RET	1	R7
      0x80000000,  //  0034  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Contact_web_values,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(Contact_X25i_X20_X25s),
    /* K4   */  be_nested_str_weak(tasmota_switch_index),
    /* K5   */  be_nested_str_weak(web_value_onoff),
    /* K6   */  be_nested_str_weak(shadow_contact),
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
** Solidified class: Matter_Plugin_Bridge_Sensor_Contact
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_HTTP;
be_local_class(Matter_Plugin_Bridge_Sensor_Contact,
    2,
    &be_class_Matter_Plugin_Bridge_HTTP,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG_TYPE, 5), be_const_static_closure(Matter_Plugin_Bridge_Sensor_Contact__X3Clambda_X3E_closure) },
        { be_const_key_weak(ARG_HINT, 4), be_nested_str_weak(Switch_X3Cx_X3E_X20number) },
        { be_const_key_weak(shadow_contact, -1), be_const_var(1) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Contact_init_closure) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Contact_web_values_prefix_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Contact_web_values_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Contact) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(switch) },
        { be_const_key_weak(parse_update, 11), be_const_closure(Matter_Plugin_Bridge_Sensor_Contact_parse_update_closure) },
        { be_const_key_weak(read_attribute, 12), be_const_closure(Matter_Plugin_Bridge_Sensor_Contact_read_attribute_closure) },
        { be_const_key_weak(CLUSTERS, 9), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(17),
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
    }))    ) } )) },
        { be_const_key_int(69, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(29, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(6,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(TYPES, 14), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(21, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(TYPE, 6), be_nested_str_weak(http_contact) },
        { be_const_key_weak(tasmota_switch_index, -1), be_const_var(0) },
        { be_const_key_weak(UPDATE_CMD, -1), be_nested_str_weak(Status_X208) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor_Contact)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_Sensor_Contact_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_Sensor_Contact);
    be_setglobal(vm, "Matter_Plugin_Bridge_Sensor_Contact");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
