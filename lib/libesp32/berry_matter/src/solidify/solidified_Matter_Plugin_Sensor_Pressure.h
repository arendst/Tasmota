/* Solidification of Matter_Plugin_Sensor_Pressure.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Pressure;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Pressure_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
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
    /* K6   */  be_nested_str_weak(shadow_value),
    /* K7   */  be_nested_str_weak(create_TLV),
    /* K8   */  be_nested_str_weak(I2),
    /* K9   */  be_nested_str_weak(NULL),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(U4),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0402,  //  0005  LDINT	R7	1027
      0x1C1C0A07,  //  0006  EQ	R7	R5	R7
      0x781E0035,  //  0007  JMPF	R7	#003E
      0x1C1C0D05,  //  0008  EQ	R7	R6	K5
      0x781E0011,  //  0009  JMPF	R7	#001C
      0x881C0106,  //  000A  GETMBR	R7	R0	K6
      0x4C200000,  //  000B  LDNIL	R8
      0x201C0E08,  //  000C  NE	R7	R7	R8
      0x781E0007,  //  000D  JMPF	R7	#0016
      0x8C1C0907,  //  000E  GETMET	R7	R4	K7
      0x88240908,  //  000F  GETMBR	R9	R4	K8
      0x60280009,  //  0010  GETGBL	R10	G9
      0x882C0106,  //  0011  GETMBR	R11	R0	K6
      0x7C280200,  //  0012  CALL	R10	1
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x70020004,  //  0015  JMP		#001B
      0x8C1C0907,  //  0016  GETMET	R7	R4	K7
      0x88240909,  //  0017  GETMBR	R9	R4	K9
      0x4C280000,  //  0018  LDNIL	R10
      0x7C1C0600,  //  0019  CALL	R7	3
      0x80040E00,  //  001A  RET	1	R7
      0x70020020,  //  001B  JMP		#003D
      0x1C1C0D0A,  //  001C  EQ	R7	R6	K10
      0x781E0005,  //  001D  JMPF	R7	#0024
      0x8C1C0907,  //  001E  GETMET	R7	R4	K7
      0x88240908,  //  001F  GETMBR	R9	R4	K8
      0x542A01F3,  //  0020  LDINT	R10	500
      0x7C1C0600,  //  0021  CALL	R7	3
      0x80040E00,  //  0022  RET	1	R7
      0x70020018,  //  0023  JMP		#003D
      0x1C1C0D0B,  //  0024  EQ	R7	R6	K11
      0x781E0005,  //  0025  JMPF	R7	#002C
      0x8C1C0907,  //  0026  GETMET	R7	R4	K7
      0x88240908,  //  0027  GETMBR	R9	R4	K8
      0x542A05DB,  //  0028  LDINT	R10	1500
      0x7C1C0600,  //  0029  CALL	R7	3
      0x80040E00,  //  002A  RET	1	R7
      0x70020010,  //  002B  JMP		#003D
      0x541EFFFB,  //  002C  LDINT	R7	65532
      0x1C1C0C07,  //  002D  EQ	R7	R6	R7
      0x781E0005,  //  002E  JMPF	R7	#0035
      0x8C1C0907,  //  002F  GETMET	R7	R4	K7
      0x8824090C,  //  0030  GETMBR	R9	R4	K12
      0x58280005,  //  0031  LDCONST	R10	K5
      0x7C1C0600,  //  0032  CALL	R7	3
      0x80040E00,  //  0033  RET	1	R7
      0x70020007,  //  0034  JMP		#003D
      0x541EFFFC,  //  0035  LDINT	R7	65533
      0x1C1C0C07,  //  0036  EQ	R7	R6	R7
      0x781E0004,  //  0037  JMPF	R7	#003D
      0x8C1C0907,  //  0038  GETMET	R7	R4	K7
      0x8824090C,  //  0039  GETMBR	R9	R4	K12
      0x5828000D,  //  003A  LDCONST	R10	K13
      0x7C1C0600,  //  003B  CALL	R7	3
      0x80040E00,  //  003C  RET	1	R7
      0x70020007,  //  003D  JMP		#0046
      0x601C0003,  //  003E  GETGBL	R7	G3
      0x5C200000,  //  003F  MOVE	R8	R0
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C1C0F0E,  //  0041  GETMET	R7	R7	K14
      0x5C240200,  //  0042  MOVE	R9	R1
      0x5C280400,  //  0043  MOVE	R10	R2
      0x7C1C0600,  //  0044  CALL	R7	3
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
** Solidified function: value_changed
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Pressure_value_changed,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
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
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x54120402,  //  0001  LDINT	R4	1027
      0x58140001,  //  0002  LDCONST	R5	K1
      0x7C080600,  //  0003  CALL	R2	3
      0x80000000,  //  0004  RET	0
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
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, 2), be_const_closure(Matter_Plugin_Sensor_Pressure_read_attribute_closure) },
        { be_const_key_weak(pre_value, 4), be_const_closure(Matter_Plugin_Sensor_Pressure_pre_value_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(1027, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(TYPE, 6), be_nested_str_weak(pressure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Pressure) },
        { be_const_key_weak(value_changed, -1), be_const_closure(Matter_Plugin_Sensor_Pressure_value_changed_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(773, -1), be_const_int(2) },
    }))    ) } )) },
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
