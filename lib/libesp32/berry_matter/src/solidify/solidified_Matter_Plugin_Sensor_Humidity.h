/* Solidification of Matter_Plugin_Sensor_Humidity.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Humidity;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Humidity_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
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
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(U2),
    /* K8   */  be_nested_str_weak(NULL),
    /* K9   */  be_const_int(1),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A0404,  //  0004  LDINT	R6	1029
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A0035,  //  0006  JMPF	R6	#003D
      0x1C180B04,  //  0007  EQ	R6	R5	K4
      0x781A0011,  //  0008  JMPF	R6	#001B
      0x88180105,  //  0009  GETMBR	R6	R0	K5
      0x4C1C0000,  //  000A  LDNIL	R7
      0x20180C07,  //  000B  NE	R6	R6	R7
      0x781A0007,  //  000C  JMPF	R6	#0015
      0x8C180706,  //  000D  GETMET	R6	R3	K6
      0x88200707,  //  000E  GETMBR	R8	R3	K7
      0x60240009,  //  000F  GETGBL	R9	G9
      0x88280105,  //  0010  GETMBR	R10	R0	K5
      0x7C240200,  //  0011  CALL	R9	1
      0x7C180600,  //  0012  CALL	R6	3
      0x80040C00,  //  0013  RET	1	R6
      0x70020004,  //  0014  JMP		#001A
      0x8C180706,  //  0015  GETMET	R6	R3	K6
      0x88200708,  //  0016  GETMBR	R8	R3	K8
      0x4C240000,  //  0017  LDNIL	R9
      0x7C180600,  //  0018  CALL	R6	3
      0x80040C00,  //  0019  RET	1	R6
      0x70020020,  //  001A  JMP		#003C
      0x1C180B09,  //  001B  EQ	R6	R5	K9
      0x781A0005,  //  001C  JMPF	R6	#0023
      0x8C180706,  //  001D  GETMET	R6	R3	K6
      0x88200707,  //  001E  GETMBR	R8	R3	K7
      0x542601F3,  //  001F  LDINT	R9	500
      0x7C180600,  //  0020  CALL	R6	3
      0x80040C00,  //  0021  RET	1	R6
      0x70020018,  //  0022  JMP		#003C
      0x1C180B0A,  //  0023  EQ	R6	R5	K10
      0x781A0005,  //  0024  JMPF	R6	#002B
      0x8C180706,  //  0025  GETMET	R6	R3	K6
      0x88200707,  //  0026  GETMBR	R8	R3	K7
      0x5426270F,  //  0027  LDINT	R9	10000
      0x7C180600,  //  0028  CALL	R6	3
      0x80040C00,  //  0029  RET	1	R6
      0x70020010,  //  002A  JMP		#003C
      0x541AFFFB,  //  002B  LDINT	R6	65532
      0x1C180A06,  //  002C  EQ	R6	R5	R6
      0x781A0005,  //  002D  JMPF	R6	#0034
      0x8C180706,  //  002E  GETMET	R6	R3	K6
      0x8820070B,  //  002F  GETMBR	R8	R3	K11
      0x58240004,  //  0030  LDCONST	R9	K4
      0x7C180600,  //  0031  CALL	R6	3
      0x80040C00,  //  0032  RET	1	R6
      0x70020007,  //  0033  JMP		#003C
      0x541AFFFC,  //  0034  LDINT	R6	65533
      0x1C180A06,  //  0035  EQ	R6	R5	R6
      0x781A0004,  //  0036  JMPF	R6	#003C
      0x8C180706,  //  0037  GETMET	R6	R3	K6
      0x8820070B,  //  0038  GETMBR	R8	R3	K11
      0x5824000C,  //  0039  LDCONST	R9	K12
      0x7C180600,  //  003A  CALL	R6	3
      0x80040C00,  //  003B  RET	1	R6
      0x70020007,  //  003C  JMP		#0045
      0x60180003,  //  003D  GETGBL	R6	G3
      0x5C1C0000,  //  003E  MOVE	R7	R0
      0x7C180200,  //  003F  CALL	R6	1
      0x8C180D0D,  //  0040  GETMET	R6	R6	K13
      0x5C200200,  //  0041  MOVE	R8	R1
      0x5C240400,  //  0042  MOVE	R9	R2
      0x7C180600,  //  0043  CALL	R6	3
      0x80040C00,  //  0044  RET	1	R6
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Humidity_pre_value,   /* name */
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
    ( &(const binstruction[10]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0004,  //  0002  JMPF	R2	#0008
      0x60080009,  //  0003  GETGBL	R2	G9
      0x540E0063,  //  0004  LDINT	R3	100
      0x080C0203,  //  0005  MUL	R3	R1	R3
      0x7C080200,  //  0006  CALL	R2	1
      0x70020000,  //  0007  JMP		#0009
      0x4C080000,  //  0008  LDNIL	R2
      0x80040400,  //  0009  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: value_changed
********************************************************************/
be_local_closure(Matter_Plugin_Sensor_Humidity_value_changed,   /* name */
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
      0x54120404,  //  0001  LDINT	R4	1029
      0x58140001,  //  0002  LDCONST	R5	K1
      0x7C080600,  //  0003  CALL	R2	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_Humidity
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor;
be_local_class(Matter_Plugin_Sensor_Humidity,
    0,
    &be_class_Matter_Plugin_Sensor,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, 2), be_const_closure(Matter_Plugin_Sensor_Humidity_read_attribute_closure) },
        { be_const_key_weak(pre_value, 4), be_const_closure(Matter_Plugin_Sensor_Humidity_pre_value_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(1029, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(TYPE, 6), be_nested_str_weak(humidity) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Humidity) },
        { be_const_key_weak(value_changed, -1), be_const_closure(Matter_Plugin_Sensor_Humidity_value_changed_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(775, -1), be_const_int(2) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Humidity)
);
/*******************************************************************/

void be_load_Matter_Plugin_Sensor_Humidity_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Sensor_Humidity);
    be_setglobal(vm, "Matter_Plugin_Sensor_Humidity");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
