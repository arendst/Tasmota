/* Solidification of Matter_Plugin_3_Sensor_Pressure.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Pressure;

/********************************************************************
** Solidified function: value_changed
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Pressure_value_changed,   /* name */
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
    /* K0   */  be_nested_str_weak(attribute_updated),
    /* K1   */  be_const_int(0),
    }),
    be_str_weak(value_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x540E0402,  //  0001  LDINT	R3	1027
      0x58100001,  //  0002  LDCONST	R4	K1
      0x7C040600,  //  0003  CALL	R1	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Pressure_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(shadow_value),
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(I2),
    /* K8   */  be_nested_str_weak(NULL),
    /* K9   */  be_const_int(1),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0402,  //  0004  LDINT	R7	1027
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0035,  //  0006  JMPF	R7	#003D
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E0011,  //  0008  JMPF	R7	#001B
      0x881C0105,  //  0009  GETMBR	R7	R0	K5
      0x4C200000,  //  000A  LDNIL	R8
      0x201C0E08,  //  000B  NE	R7	R7	R8
      0x781E0007,  //  000C  JMPF	R7	#0015
      0x8C1C0706,  //  000D  GETMET	R7	R3	K6
      0x88240907,  //  000E  GETMBR	R9	R4	K7
      0x60280009,  //  000F  GETGBL	R10	G9
      0x882C0105,  //  0010  GETMBR	R11	R0	K5
      0x7C280200,  //  0011  CALL	R10	1
      0x7C1C0600,  //  0012  CALL	R7	3
      0x80040E00,  //  0013  RET	1	R7
      0x70020004,  //  0014  JMP		#001A
      0x8C1C0706,  //  0015  GETMET	R7	R3	K6
      0x88240908,  //  0016  GETMBR	R9	R4	K8
      0x4C280000,  //  0017  LDNIL	R10
      0x7C1C0600,  //  0018  CALL	R7	3
      0x80040E00,  //  0019  RET	1	R7
      0x70020020,  //  001A  JMP		#003C
      0x1C1C0D09,  //  001B  EQ	R7	R6	K9
      0x781E0005,  //  001C  JMPF	R7	#0023
      0x8C1C0706,  //  001D  GETMET	R7	R3	K6
      0x88240907,  //  001E  GETMBR	R9	R4	K7
      0x542A01F3,  //  001F  LDINT	R10	500
      0x7C1C0600,  //  0020  CALL	R7	3
      0x80040E00,  //  0021  RET	1	R7
      0x70020018,  //  0022  JMP		#003C
      0x1C1C0D0A,  //  0023  EQ	R7	R6	K10
      0x781E0005,  //  0024  JMPF	R7	#002B
      0x8C1C0706,  //  0025  GETMET	R7	R3	K6
      0x88240907,  //  0026  GETMBR	R9	R4	K7
      0x542A05DB,  //  0027  LDINT	R10	1500
      0x7C1C0600,  //  0028  CALL	R7	3
      0x80040E00,  //  0029  RET	1	R7
      0x70020010,  //  002A  JMP		#003C
      0x541EFFFB,  //  002B  LDINT	R7	65532
      0x1C1C0C07,  //  002C  EQ	R7	R6	R7
      0x781E0005,  //  002D  JMPF	R7	#0034
      0x8C1C0706,  //  002E  GETMET	R7	R3	K6
      0x8824090B,  //  002F  GETMBR	R9	R4	K11
      0x58280004,  //  0030  LDCONST	R10	K4
      0x7C1C0600,  //  0031  CALL	R7	3
      0x80040E00,  //  0032  RET	1	R7
      0x70020007,  //  0033  JMP		#003C
      0x541EFFFC,  //  0034  LDINT	R7	65533
      0x1C1C0C07,  //  0035  EQ	R7	R6	R7
      0x781E0004,  //  0036  JMPF	R7	#003C
      0x8C1C0706,  //  0037  GETMET	R7	R3	K6
      0x8824090B,  //  0038  GETMBR	R9	R4	K11
      0x5828000C,  //  0039  LDCONST	R10	K12
      0x7C1C0600,  //  003A  CALL	R7	3
      0x80040E00,  //  003B  RET	1	R7
      0x70020008,  //  003C  JMP		#0046
      0x601C0003,  //  003D  GETGBL	R7	G3
      0x5C200000,  //  003E  MOVE	R8	R0
      0x7C1C0200,  //  003F  CALL	R7	1
      0x8C1C0F0D,  //  0040  GETMET	R7	R7	K13
      0x5C240200,  //  0041  MOVE	R9	R1
      0x5C280400,  //  0042  MOVE	R10	R2
      0x5C2C0600,  //  0043  MOVE	R11	R3
      0x7C1C0800,  //  0044  CALL	R7	4
      0x80040E00,  //  0045  RET	1	R7
      0x80000000,  //  0046  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Pressure_pre_value,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(pre_value),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0003,  //  0002  JMPF	R2	#0007
      0x60080009,  //  0003  GETGBL	R2	G9
      0x5C0C0200,  //  0004  MOVE	R3	R1
      0x7C080200,  //  0005  CALL	R2	1
      0x70020000,  //  0006  JMP		#0008
      0x4C080000,  //  0007  LDNIL	R2
      0x80040400,  //  0008  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_Pressure
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_class(Matter_Plugin_Sensor_Pressure,
    0,
    &be_class_Matter_Plugin_Sensor,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Pressure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(pressure) },
        { be_const_key_weak(TYPES, 8), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(773, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Pressure),
    }))    ) } )) },
        { be_const_key_weak(value_changed, 7), be_const_closure(Matter_Plugin_Sensor_Pressure_value_changed_closure) },
        { be_const_key_weak(JSON_NAME, -1), be_nested_str_weak(Pressure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Sensor_Pressure_read_attribute_closure) },
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
        { be_const_key_int(1027, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_int(3, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
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
        { be_const_key_weak(pre_value, -1), be_const_closure(Matter_Plugin_Sensor_Pressure_pre_value_closure) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Pressure)
);
/*******************************************************************/

void be_load_Matter_Plugin_Sensor_Pressure_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Sensor_Pressure);
    be_setglobal(vm, "Matter_Plugin_Sensor_Pressure");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
