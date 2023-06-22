/* Solidification of Matter_Plugin_Bridge_Sensor_Temp.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Temp;

/********************************************************************
** Solidified function: pre_value
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Temp_pre_value,   /* name */
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
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Temp_read_attribute,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(shadow_value),
    /* K6   */  be_nested_str_weak(create_TLV),
    /* K7   */  be_nested_str_weak(I2),
    /* K8   */  be_nested_str_weak(NULL),
    /* K9   */  be_const_int(1),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(U4),
    /* K12  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x88100502,  //  0002  GETMBR	R4	R2	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x541A0401,  //  0004  LDINT	R6	1026
      0x1C180806,  //  0005  EQ	R6	R4	R6
      0x781A0033,  //  0006  JMPF	R6	#003B
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
      0x70020020,  //  0018  JMP		#003A
      0x1C180B09,  //  0019  EQ	R6	R5	K9
      0x781A0005,  //  001A  JMPF	R6	#0021
      0x8C180706,  //  001B  GETMET	R6	R3	K6
      0x88200707,  //  001C  GETMBR	R8	R3	K7
      0x5425EC77,  //  001D  LDINT	R9	-5000
      0x7C180600,  //  001E  CALL	R6	3
      0x80040C00,  //  001F  RET	1	R6
      0x70020018,  //  0020  JMP		#003A
      0x1C180B0A,  //  0021  EQ	R6	R5	K10
      0x781A0005,  //  0022  JMPF	R6	#0029
      0x8C180706,  //  0023  GETMET	R6	R3	K6
      0x88200707,  //  0024  GETMBR	R8	R3	K7
      0x54263A97,  //  0025  LDINT	R9	15000
      0x7C180600,  //  0026  CALL	R6	3
      0x80040C00,  //  0027  RET	1	R6
      0x70020010,  //  0028  JMP		#003A
      0x541AFFFB,  //  0029  LDINT	R6	65532
      0x1C180A06,  //  002A  EQ	R6	R5	R6
      0x781A0005,  //  002B  JMPF	R6	#0032
      0x8C180706,  //  002C  GETMET	R6	R3	K6
      0x8820070B,  //  002D  GETMBR	R8	R3	K11
      0x58240004,  //  002E  LDCONST	R9	K4
      0x7C180600,  //  002F  CALL	R6	3
      0x80040C00,  //  0030  RET	1	R6
      0x70020007,  //  0031  JMP		#003A
      0x541AFFFC,  //  0032  LDINT	R6	65533
      0x1C180A06,  //  0033  EQ	R6	R5	R6
      0x781A0004,  //  0034  JMPF	R6	#003A
      0x8C180706,  //  0035  GETMET	R6	R3	K6
      0x8820070B,  //  0036  GETMBR	R8	R3	K11
      0x54260003,  //  0037  LDINT	R9	4
      0x7C180600,  //  0038  CALL	R6	3
      0x80040C00,  //  0039  RET	1	R6
      0x70020007,  //  003A  JMP		#0043
      0x60180003,  //  003B  GETGBL	R6	G3
      0x5C1C0000,  //  003C  MOVE	R7	R0
      0x7C180200,  //  003D  CALL	R6	1
      0x8C180D0C,  //  003E  GETMET	R6	R6	K12
      0x5C200200,  //  003F  MOVE	R8	R1
      0x5C240400,  //  0040  MOVE	R9	R2
      0x7C180600,  //  0041  CALL	R6	3
      0x80040C00,  //  0042  RET	1	R6
      0x80000000,  //  0043  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: value_changed
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Temp_value_changed,   /* name */
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
      0x54120401,  //  0001  LDINT	R4	1026
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
be_local_closure(Matter_Plugin_Bridge_Sensor_Temp_web_values,   /* name */
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
    /* K3   */  be_nested_str_weak(_X26_X23x2600_X3B_X26_X23xFE0F_X3B_X20_X25_X2E1f_X20_XC2_XB0C),
    /* K4   */  be_nested_str_weak(shadow_value),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140003,  //  0005  LDCONST	R5	K3
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x4C1C0000,  //  0007  LDNIL	R7
      0x20180C07,  //  0008  NE	R6	R6	R7
      0x781A0005,  //  0009  JMPF	R6	#0010
      0x6018000A,  //  000A  GETGBL	R6	G10
      0x881C0104,  //  000B  GETMBR	R7	R0	K4
      0x7C180200,  //  000C  CALL	R6	1
      0x541E0063,  //  000D  LDINT	R7	100
      0x0C180C07,  //  000E  DIV	R6	R6	R7
      0x70020000,  //  000F  JMP		#0011
      0x4C180000,  //  0010  LDNIL	R6
      0x7C100400,  //  0011  CALL	R4	2
      0x7C080400,  //  0012  CALL	R2	2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Bridge_Sensor_Temp
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_Sensor;
be_local_class(Matter_Plugin_Bridge_Sensor_Temp,
    0,
    &be_class_Matter_Plugin_Bridge_Sensor,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pre_value, 1), be_const_closure(Matter_Plugin_Bridge_Sensor_Temp_pre_value_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Temp_web_values_closure) },
        { be_const_key_weak(CLUSTERS, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(1026, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, 7), be_const_closure(Matter_Plugin_Bridge_Sensor_Temp_read_attribute_closure) },
        { be_const_key_weak(value_changed, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Temp_value_changed_closure) },
        { be_const_key_weak(TYPE, 4), be_nested_str_weak(http_temperature) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Temperature) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(770, -1), be_const_int(2) },
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor_Temp)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_Sensor_Temp_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_Sensor_Temp);
    be_setglobal(vm, "Matter_Plugin_Bridge_Sensor_Temp");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
