/* Solidification of Matter_Plugin_Sensor_OnOff.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_OnOff;

/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_OnOff__X3Clambda_X3E,   /* name */
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
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_OnOff_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(update_shadow_lazy),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(BOOL),
    /* K9   */  be_nested_str_weak(shadow_onoff),
    /* K10  */  be_nested_str_weak(U4),
    /* K11  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0005,  //  0005  LDINT	R7	6
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E001B,  //  0007  JMPF	R7	#0024
      0x8C1C0105,  //  0008  GETMET	R7	R0	K5
      0x7C1C0200,  //  0009  CALL	R7	1
      0x1C1C0D06,  //  000A  EQ	R7	R6	K6
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0907,  //  000C  GETMET	R7	R4	K7
      0x88240908,  //  000D  GETMBR	R9	R4	K8
      0x88280109,  //  000E  GETMBR	R10	R0	K9
      0x7C1C0600,  //  000F  CALL	R7	3
      0x80040E00,  //  0010  RET	1	R7
      0x70020010,  //  0011  JMP		#0023
      0x541EFFFB,  //  0012  LDINT	R7	65532
      0x1C1C0C07,  //  0013  EQ	R7	R6	R7
      0x781E0005,  //  0014  JMPF	R7	#001B
      0x8C1C0907,  //  0015  GETMET	R7	R4	K7
      0x8824090A,  //  0016  GETMBR	R9	R4	K10
      0x58280006,  //  0017  LDCONST	R10	K6
      0x7C1C0600,  //  0018  CALL	R7	3
      0x80040E00,  //  0019  RET	1	R7
      0x70020007,  //  001A  JMP		#0023
      0x541EFFFC,  //  001B  LDINT	R7	65533
      0x1C1C0C07,  //  001C  EQ	R7	R6	R7
      0x781E0004,  //  001D  JMPF	R7	#0023
      0x8C1C0907,  //  001E  GETMET	R7	R4	K7
      0x8824090A,  //  001F  GETMBR	R9	R4	K10
      0x542A0003,  //  0020  LDINT	R10	4
      0x7C1C0600,  //  0021  CALL	R7	3
      0x80040E00,  //  0022  RET	1	R7
      0x70020007,  //  0023  JMP		#002C
      0x601C0003,  //  0024  GETGBL	R7	G3
      0x5C200000,  //  0025  MOVE	R8	R0
      0x7C1C0200,  //  0026  CALL	R7	1
      0x8C1C0F0B,  //  0027  GETMET	R7	R7	K11
      0x5C240200,  //  0028  MOVE	R9	R1
      0x5C280400,  //  0029  MOVE	R10	R2
      0x7C1C0600,  //  002A  CALL	R7	3
      0x80040E00,  //  002B  RET	1	R7
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_OnOff_update_shadow,   /* name */
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
    /* K10  */  be_nested_str_weak(shadow_onoff),
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
      0x541E0405,  //  0028  LDINT	R7	1030
      0x5820000C,  //  0029  LDCONST	R8	K12
      0x7C140600,  //  002A  CALL	R5	3
      0x90021404,  //  002B  SETMBR	R0	K10	R4
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_OnOff_init,   /* name */
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
** Solidified class: Matter_Plugin_Sensor_OnOff
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor_OnOff,
    2,
    &be_class_Matter_Plugin_Device,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(TYPE, 11), be_nested_str_weak(onoff) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(2128, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(ARG, 9), be_nested_str_weak(switch) },
        { be_const_key_weak(ARG_TYPE, 8), be_const_static_closure(Matter_Plugin_Sensor_OnOff__X3Clambda_X3E_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(OnOff) },
        { be_const_key_weak(read_attribute, 5), be_const_closure(Matter_Plugin_Sensor_OnOff_read_attribute_closure) },
        { be_const_key_weak(tasmota_switch_index, -1), be_const_var(0) },
        { be_const_key_weak(shadow_onoff, -1), be_const_var(1) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Sensor_OnOff_init_closure) },
        { be_const_key_weak(CLUSTERS, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(6, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Sensor_OnOff_update_shadow_closure) },
    })),
    be_str_weak(Matter_Plugin_Sensor_OnOff)
);
/*******************************************************************/

void be_load_Matter_Plugin_Sensor_OnOff_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Sensor_OnOff);
    be_setglobal(vm, "Matter_Plugin_Sensor_OnOff");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
