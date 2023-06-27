/* Solidification of Matter_Plugin_Bridge_Sensor_Illuminance.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Illuminance;

/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Illuminance_pre_value,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(math),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(log10),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(pre_value),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0xA40A0000,  //  0005  IMPORT	R2	K0
      0x140C0301,  //  0006  LT	R3	R1	K1
      0x780E0001,  //  0007  JMPF	R3	#000A
      0x80060200,  //  0008  RET	1	K1
      0x70020005,  //  0009  JMP		#0010
      0x8C0C0502,  //  000A  GETMET	R3	R2	K2
      0x00140303,  //  000B  ADD	R5	R1	K3
      0x7C0C0400,  //  000C  CALL	R3	2
      0x5412270F,  //  000D  LDINT	R4	10000
      0x080C0604,  //  000E  MUL	R3	R3	R4
      0x80040600,  //  000F  RET	1	R3
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Illuminance_read_attribute,   /* name */
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
      0x541A03FF,  //  0004  LDINT	R6	1024
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
      0x58240009,  //  001F  LDCONST	R9	K9
      0x7C180600,  //  0020  CALL	R6	3
      0x80040C00,  //  0021  RET	1	R6
      0x70020018,  //  0022  JMP		#003C
      0x1C180B0A,  //  0023  EQ	R6	R5	K10
      0x781A0005,  //  0024  JMPF	R6	#002B
      0x8C180706,  //  0025  GETMET	R6	R3	K6
      0x88200707,  //  0026  GETMBR	R8	R3	K7
      0x5426FFFD,  //  0027  LDINT	R9	65534
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
** Solidified function: value_changed
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Illuminance_value_changed,   /* name */
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
      0x541203FF,  //  0001  LDINT	R4	1024
      0x58140001,  //  0002  LDCONST	R5	K1
      0x7C080600,  //  0003  CALL	R2	3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Illuminance_web_values,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(_X26_X23128261_X3B_X20_X25ilux),
    /* K4   */  be_nested_str_weak(shadow_value),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140003,  //  0005  LDCONST	R5	K3
      0x60180009,  //  0006  GETGBL	R6	G9
      0x881C0104,  //  0007  GETMBR	R7	R0	K4
      0x7C180200,  //  0008  CALL	R6	1
      0x7C100400,  //  0009  CALL	R4	2
      0x7C080400,  //  000A  CALL	R2	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Sensor_Illuminance
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Sensor;
be_local_class(Matter_Plugin_Bridge_Sensor_Illuminance,
    0,
    &be_class_Matter_Plugin_Bridge_Sensor,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pre_value, 1), be_const_closure(Matter_Plugin_Bridge_Sensor_Illuminance_pre_value_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Illuminance_web_values_closure) },
        { be_const_key_weak(CLUSTERS, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(1024, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, 7), be_const_closure(Matter_Plugin_Bridge_Sensor_Illuminance_read_attribute_closure) },
        { be_const_key_weak(value_changed, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Illuminance_value_changed_closure) },
        { be_const_key_weak(TYPE, 4), be_nested_str_weak(http_illuminance) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Illuminance) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(262, -1), be_const_int(2) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor_Illuminance)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_Sensor_Illuminance_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_Sensor_Illuminance);
    be_setglobal(vm, "Matter_Plugin_Bridge_Sensor_Illuminance");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
