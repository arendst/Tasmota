/* Solidification of Matter_Plugin_Sensor_Occupancy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Occupancy_init,   /* name */
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
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Occupancy__X3Clambda_X3E,   /* name */
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
be_local_closure(Matter_Plugin_Sensor_Occupancy_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TLV),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(shadow_occupancy),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(U1),
    /* K9   */  be_nested_str_weak(NULL),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(3),
    /* K12  */  be_const_int(2),
    /* K13  */  be_nested_str_weak(U4),
    /* K14  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0405,  //  0005  LDINT	R7	1030
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0033,  //  0007  JMPF	R7	#003C
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
      0x70020020,  //  0019  JMP		#003B
      0x1C1C0D0A,  //  001A  EQ	R7	R6	K10
      0x781E0005,  //  001B  JMPF	R7	#0022
      0x8C1C0907,  //  001C  GETMET	R7	R4	K7
      0x88240908,  //  001D  GETMBR	R9	R4	K8
      0x5828000B,  //  001E  LDCONST	R10	K11
      0x7C1C0600,  //  001F  CALL	R7	3
      0x80040E00,  //  0020  RET	1	R7
      0x70020018,  //  0021  JMP		#003B
      0x1C1C0D0C,  //  0022  EQ	R7	R6	K12
      0x781E0005,  //  0023  JMPF	R7	#002A
      0x8C1C0907,  //  0024  GETMET	R7	R4	K7
      0x88240908,  //  0025  GETMBR	R9	R4	K8
      0x58280005,  //  0026  LDCONST	R10	K5
      0x7C1C0600,  //  0027  CALL	R7	3
      0x80040E00,  //  0028  RET	1	R7
      0x70020010,  //  0029  JMP		#003B
      0x541EFFFB,  //  002A  LDINT	R7	65532
      0x1C1C0C07,  //  002B  EQ	R7	R6	R7
      0x781E0005,  //  002C  JMPF	R7	#0033
      0x8C1C0907,  //  002D  GETMET	R7	R4	K7
      0x8824090D,  //  002E  GETMBR	R9	R4	K13
      0x58280005,  //  002F  LDCONST	R10	K5
      0x7C1C0600,  //  0030  CALL	R7	3
      0x80040E00,  //  0031  RET	1	R7
      0x70020007,  //  0032  JMP		#003B
      0x541EFFFC,  //  0033  LDINT	R7	65533
      0x1C1C0C07,  //  0034  EQ	R7	R6	R7
      0x781E0004,  //  0035  JMPF	R7	#003B
      0x8C1C0907,  //  0036  GETMET	R7	R4	K7
      0x8824090D,  //  0037  GETMBR	R9	R4	K13
      0x5828000B,  //  0038  LDCONST	R10	K11
      0x7C1C0600,  //  0039  CALL	R7	3
      0x80040E00,  //  003A  RET	1	R7
      0x70020007,  //  003B  JMP		#0044
      0x601C0003,  //  003C  GETGBL	R7	G3
      0x5C200000,  //  003D  MOVE	R8	R0
      0x7C1C0200,  //  003E  CALL	R7	1
      0x8C1C0F0E,  //  003F  GETMET	R7	R7	K14
      0x5C240200,  //  0040  MOVE	R9	R1
      0x5C280400,  //  0041  MOVE	R10	R2
      0x7C1C0600,  //  0042  CALL	R7	3
      0x80040E00,  //  0043  RET	1	R7
      0x80000000,  //  0044  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Occupancy_update_shadow,   /* name */
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
    /* K10  */  be_nested_str_weak(shadow_occupancy),
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
** Solidified class: Matter_Plugin_Sensor_Occupancy
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor_Occupancy,
    2,
    &be_class_Matter_Plugin_Device,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(TYPE, 9), be_nested_str_weak(occupancy) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(263, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(init, 11), be_const_closure(Matter_Plugin_Sensor_Occupancy_init_closure) },
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(Matter_Plugin_Sensor_Occupancy__X3Clambda_X3E_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Occupancy) },
        { be_const_key_weak(read_attribute, 5), be_const_closure(Matter_Plugin_Sensor_Occupancy_read_attribute_closure) },
        { be_const_key_weak(shadow_occupancy, -1), be_const_var(1) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(1030, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(Matter_Plugin_Sensor_Occupancy_update_shadow_closure) },
        { be_const_key_weak(tasmota_switch_index, 8), be_const_var(0) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(switch) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Occupancy)
);
/*******************************************************************/

void be_load_Matter_Plugin_Sensor_Occupancy_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Sensor_Occupancy);
    be_setglobal(vm, "Matter_Plugin_Sensor_Occupancy");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
