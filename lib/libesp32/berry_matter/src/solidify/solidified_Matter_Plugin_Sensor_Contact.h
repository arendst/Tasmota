/* Solidification of Matter_Plugin_Sensor_Contact.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Contact;

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
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Contact_read_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
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
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(BOOL),
    /* K8   */  be_nested_str_weak(NULL),
    /* K9   */  be_nested_str_weak(U4),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A0044,  //  0004  LDINT	R6	69
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A0023,  //  0006  JMPF	R6	#002B
      0x1C180B04,  //  0007  EQ	R6	R5	K4
      0x781A000F,  //  0008  JMPF	R6	#0019
      0x88180105,  //  0009  GETMBR	R6	R0	K5
      0x4C1C0000,  //  000A  LDNIL	R7
      0x20180C07,  //  000B  NE	R6	R6	R7
      0x781A0005,  //  000C  JMPF	R6	#0013
      0x8C180706,  //  000D  GETMET	R6	R3	K6
      0x88200707,  //  000E  GETMBR	R8	R3	K7
      0x88240105,  //  000F  GETMBR	R9	R0	K5
      0x7C180600,  //  0010  CALL	R6	3
      0x80040C00,  //  0011  RET	1	R6
      0x70020004,  //  0012  JMP		#0018
      0x8C180706,  //  0013  GETMET	R6	R3	K6
      0x88200708,  //  0014  GETMBR	R8	R3	K8
      0x4C240000,  //  0015  LDNIL	R9
      0x7C180600,  //  0016  CALL	R6	3
      0x80040C00,  //  0017  RET	1	R6
      0x70020010,  //  0018  JMP		#002A
      0x541AFFFB,  //  0019  LDINT	R6	65532
      0x1C180A06,  //  001A  EQ	R6	R5	R6
      0x781A0005,  //  001B  JMPF	R6	#0022
      0x8C180706,  //  001C  GETMET	R6	R3	K6
      0x88200709,  //  001D  GETMBR	R8	R3	K9
      0x58240004,  //  001E  LDCONST	R9	K4
      0x7C180600,  //  001F  CALL	R6	3
      0x80040C00,  //  0020  RET	1	R6
      0x70020007,  //  0021  JMP		#002A
      0x541AFFFC,  //  0022  LDINT	R6	65533
      0x1C180A06,  //  0023  EQ	R6	R5	R6
      0x781A0004,  //  0024  JMPF	R6	#002A
      0x8C180706,  //  0025  GETMET	R6	R3	K6
      0x88200709,  //  0026  GETMBR	R8	R3	K9
      0x5824000A,  //  0027  LDCONST	R9	K10
      0x7C180600,  //  0028  CALL	R6	3
      0x80040C00,  //  0029  RET	1	R6
      0x70020007,  //  002A  JMP		#0033
      0x60180003,  //  002B  GETGBL	R6	G3
      0x5C1C0000,  //  002C  MOVE	R7	R0
      0x7C180200,  //  002D  CALL	R6	1
      0x8C180D0B,  //  002E  GETMET	R6	R6	K11
      0x5C200200,  //  002F  MOVE	R8	R1
      0x5C240400,  //  0030  MOVE	R9	R2
      0x7C180600,  //  0031  CALL	R6	3
      0x80040C00,  //  0032  RET	1	R6
      0x80000000,  //  0033  RET	0
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
    ( &(const binstruction[45]) {  /* code */
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
      0x780E001D,  //  000D  JMPF	R3	#002C
      0x8C0C0305,  //  000E  GETMET	R3	R1	K5
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x4C100000,  //  0011  LDNIL	R4
      0x20100604,  //  0012  NE	R4	R3	R4
      0x78120017,  //  0013  JMPF	R4	#002C
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
      0x4C180000,  //  001E  LDNIL	R6
      0x20140A06,  //  001F  NE	R5	R5	R6
      0x78160009,  //  0020  JMPF	R5	#002B
      0x8814010A,  //  0021  GETMBR	R5	R0	K10
      0x60180017,  //  0022  GETGBL	R6	G23
      0x5C1C0800,  //  0023  MOVE	R7	R4
      0x7C180200,  //  0024  CALL	R6	1
      0x20140A06,  //  0025  NE	R5	R5	R6
      0x78160003,  //  0026  JMPF	R5	#002B
      0x8C14010B,  //  0027  GETMET	R5	R0	K11
      0x541E0044,  //  0028  LDINT	R7	69
      0x5820000C,  //  0029  LDCONST	R8	K12
      0x7C140600,  //  002A  CALL	R5	3
      0x90021404,  //  002B  SETMBR	R0	K10	R4
      0x80000000,  //  002C  RET	0
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
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG, -1), be_nested_str_weak(switch) },
        { be_const_key_weak(shadow_contact, -1), be_const_var(1) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(contact) },
        { be_const_key_weak(ARG_TYPE, 5), be_const_static_closure(Matter_Plugin_Sensor_Contact__X3Clambda_X3E_closure) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(ARG_HINT, 1), be_nested_str_weak(Switch_X3Cx_X3E_X20number) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Contact) },
        { be_const_key_weak(parse_configuration, 8), be_const_closure(Matter_Plugin_Sensor_Contact_parse_configuration_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Sensor_Contact_update_shadow_closure) },
        { be_const_key_weak(tasmota_switch_index, 6), be_const_var(0) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(69, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, 4), be_const_closure(Matter_Plugin_Sensor_Contact_read_attribute_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(21, -1), be_const_int(1) },
    }))    ) } )) },
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
