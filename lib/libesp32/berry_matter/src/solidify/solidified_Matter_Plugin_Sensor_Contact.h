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
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_contact),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(BOOL),
    /* K9   */  be_nested_str_weak(NULL),
    /* K10  */  be_nested_str_weak(U4),
    /* K11  */  be_const_int(1),
    /* K12  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0044,  //  0005  LDINT	R7	69
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0023,  //  0007  JMPF	R7	#002C
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E000F,  //  0009  JMPF	R7	#001A
      0x881C0106,  //  000A  GETMBR	R7	R0	K6
      0x4C200000,  //  000B  LDNIL	R8
      0x201C0E08,  //  000C  NE	R7	R7	R8
      0x781E0005,  //  000D  JMPF	R7	#0014
      0x8C1C0907,  //  000E  GETMET	R7	R4	K7
      0x88240908,  //  000F  GETMBR	R9	R4	K8
      0x88280106,  //  0010  GETMBR	R10	R0	K6
      0x7C1C0600,  //  0011  CALL	R7	3
      0x80040E00,  //  0012  RET	1	R7
      0x70020004,  //  0013  JMP		#0019
      0x8C1C0907,  //  0014  GETMET	R7	R4	K7
      0x88240909,  //  0015  GETMBR	R9	R4	K9
      0x4C280000,  //  0016  LDNIL	R10
      0x7C1C0600,  //  0017  CALL	R7	3
      0x80040E00,  //  0018  RET	1	R7
      0x70020010,  //  0019  JMP		#002B
      0x541EFFFB,  //  001A  LDINT	R7	65532
      0x1C1C0C07,  //  001B  EQ	R7	R6	R7
      0x781E0005,  //  001C  JMPF	R7	#0023
      0x8C1C0907,  //  001D  GETMET	R7	R4	K7
      0x8824090A,  //  001E  GETMBR	R9	R4	K10
      0x58280005,  //  001F  LDCONST	R10	K5
      0x7C1C0600,  //  0020  CALL	R7	3
      0x80040E00,  //  0021  RET	1	R7
      0x70020007,  //  0022  JMP		#002B
      0x541EFFFC,  //  0023  LDINT	R7	65533
      0x1C1C0C07,  //  0024  EQ	R7	R6	R7
      0x781E0004,  //  0025  JMPF	R7	#002B
      0x8C1C0907,  //  0026  GETMET	R7	R4	K7
      0x8824090A,  //  0027  GETMBR	R9	R4	K10
      0x5828000B,  //  0028  LDCONST	R10	K11
      0x7C1C0600,  //  0029  CALL	R7	3
      0x80040E00,  //  002A  RET	1	R7
      0x70020007,  //  002B  JMP		#0034
      0x601C0003,  //  002C  GETGBL	R7	G3
      0x5C200000,  //  002D  MOVE	R8	R0
      0x7C1C0200,  //  002E  CALL	R7	1
      0x8C1C0F0C,  //  002F  GETMET	R7	R7	K12
      0x5C240200,  //  0030  MOVE	R9	R1
      0x5C280400,  //  0031  MOVE	R10	R2
      0x7C1C0600,  //  0032  CALL	R7	3
      0x80040E00,  //  0033  RET	1	R7
      0x80000000,  //  0034  RET	0
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
        { be_const_key_weak(ARG_HINT, 1), be_nested_str_weak(Enter_X20Switch_X3Cx_X3E_X20number) },
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
