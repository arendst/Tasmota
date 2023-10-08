/* Solidification of Matter_Plugin_3_Sensor_Contact.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Contact;

/********************************************************************
** Solidified function: append_state_json
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Contact_append_state_json,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(_X2C_X22Contact_X22_X3A_X25s),
    /* K1   */  be_nested_str_weak(shadow_contact),
    }),
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
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Contact_parse_configuration,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_switch_index),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x8C0C0301,  //  0001  GETMET	R3	R1	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x58180003,  //  0003  LDCONST	R6	K3
      0x7C0C0600,  //  0004  CALL	R3	3
      0x7C080200,  //  0005  CALL	R2	1
      0x90020002,  //  0006  SETMBR	R0	K0	R2
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x18080504,  //  0008  LE	R2	R2	K4
      0x780A0000,  //  0009  JMPF	R2	#000B
      0x90020103,  //  000A  SETMBR	R0	K0	K3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Contact_update_shadow,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(update_shadow),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(cmd),
    /* K4   */  be_nested_str_weak(Status_X208),
    /* K5   */  be_nested_str_weak(load),
    /* K6   */  be_nested_str_weak(find),
    /* K7   */  be_nested_str_weak(Switch),
    /* K8   */  be_nested_str_weak(tasmota_switch_index),
    /* K9   */  be_nested_str_weak(ON),
    /* K10  */  be_nested_str_weak(shadow_contact),
    /* K11  */  be_nested_str_weak(attribute_updated),
    /* K12  */  be_const_int(0),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0xA4060200,  //  0005  IMPORT	R1	K1
      0xB80A0400,  //  0006  GETNGBL	R2	K2
      0x8C080503,  //  0007  GETMET	R2	R2	K3
      0x58100004,  //  0008  LDCONST	R4	K4
      0x50140200,  //  0009  LDBOOL	R5	1	0
      0x7C080600,  //  000A  CALL	R2	3
      0x4C0C0000,  //  000B  LDNIL	R3
      0x200C0403,  //  000C  NE	R3	R2	R3
      0x780E0016,  //  000D  JMPF	R3	#0025
      0x8C0C0305,  //  000E  GETMET	R3	R1	K5
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x4C100000,  //  0011  LDNIL	R4
      0x20100604,  //  0012  NE	R4	R3	R4
      0x78120010,  //  0013  JMPF	R4	#0025
      0x50100000,  //  0014  LDBOOL	R4	0	0
      0x8C140706,  //  0015  GETMET	R5	R3	K6
      0x601C0008,  //  0016  GETGBL	R7	G8
      0x88200108,  //  0017  GETMBR	R8	R0	K8
      0x7C1C0200,  //  0018  CALL	R7	1
      0x001E0E07,  //  0019  ADD	R7	K7	R7
      0x7C140400,  //  001A  CALL	R5	2
      0x1C140B09,  //  001B  EQ	R5	R5	K9
      0x5C100A00,  //  001C  MOVE	R4	R5
      0x8814010A,  //  001D  GETMBR	R5	R0	K10
      0x20140A04,  //  001E  NE	R5	R5	R4
      0x78160004,  //  001F  JMPF	R5	#0025
      0x8C14010B,  //  0020  GETMET	R5	R0	K11
      0x541E0044,  //  0021  LDINT	R7	69
      0x5820000C,  //  0022  LDCONST	R8	K12
      0x7C140600,  //  0023  CALL	R5	3
      0x90021404,  //  0024  SETMBR	R0	K10	R4
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Contact_update_virtual,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(Contact),
    /* K2   */  be_nested_str_weak(shadow_contact),
    /* K3   */  be_nested_str_weak(attribute_updated),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(update_virtual),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x58100001,  //  0001  LDCONST	R4	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E000B,  //  0005  JMPF	R3	#0012
      0x600C0017,  //  0006  GETGBL	R3	G23
      0x5C100400,  //  0007  MOVE	R4	R2
      0x7C0C0200,  //  0008  CALL	R3	1
      0x5C080600,  //  0009  MOVE	R2	R3
      0x880C0102,  //  000A  GETMBR	R3	R0	K2
      0x200C0602,  //  000B  NE	R3	R3	R2
      0x780E0004,  //  000C  JMPF	R3	#0012
      0x8C0C0103,  //  000D  GETMET	R3	R0	K3
      0x54160044,  //  000E  LDINT	R5	69
      0x58180004,  //  000F  LDCONST	R6	K4
      0x7C0C0600,  //  0010  CALL	R3	3
      0x90020402,  //  0011  SETMBR	R0	K2	R2
      0x600C0003,  //  0012  GETGBL	R3	G3
      0x5C100000,  //  0013  MOVE	R4	R0
      0x7C0C0200,  //  0014  CALL	R3	1
      0x8C0C0705,  //  0015  GETMET	R3	R3	K5
      0x5C140200,  //  0016  MOVE	R5	R1
      0x7C0C0400,  //  0017  CALL	R3	2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Contact__X3Clambda_X3E,   /* name */
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
be_local_closure(Matter_Plugin_Sensor_Contact_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_contact),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x50100000,  //  0008  LDBOOL	R4	0	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Contact_read_attribute,   /* name */
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
** Solidified class: Matter_Plugin_Sensor_Contact
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor_Contact,
    2,
    &be_class_Matter_Plugin_Device,
    be_nested_map(17,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, 12), be_const_closure(Matter_Plugin_Sensor_Contact_read_attribute_closure) },
        { be_const_key_weak(append_state_json, -1), be_const_closure(Matter_Plugin_Sensor_Contact_append_state_json_closure) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Switch_X3Cx_X3E_X20number) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Sensor_Contact_init_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(21, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(Matter_Plugin_Sensor_Contact_parse_configuration_closure) },
        { be_const_key_weak(shadow_contact, 16), be_const_var(1) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(Matter_Plugin_Sensor_Contact_update_virtual_closure) },
        { be_const_key_weak(ARG, 7), be_nested_str_weak(switch) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        { be_const_key_weak(tasmota_switch_index, -1), be_const_var(0) },
        { be_const_key_weak(DISPLAY_NAME, 0), be_nested_str_weak(Contact) },
        { be_const_key_weak(ARG_TYPE, 13), be_const_static_closure(Matter_Plugin_Sensor_Contact__X3Clambda_X3E_closure) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(750) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(contact) },
        { be_const_key_weak(UPDATE_COMMANDS, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Contact),
    }))    ) } )) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Sensor_Contact_update_shadow_closure) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Contact)
);
/*******************************************************************/

void be_load_Matter_Plugin_Sensor_Contact_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Sensor_Contact);
    be_setglobal(vm, "Matter_Plugin_Sensor_Contact");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
