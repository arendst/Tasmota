/* Solidification of Matter_Plugin_4_Bridge_Sensor_Illuminance.h */
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
** Solidified function: value_changed
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Illuminance_value_changed,   /* name */
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
      0x540E03FF,  //  0001  LDINT	R3	1024
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
be_local_closure(Matter_Plugin_Bridge_Sensor_Illuminance_read_attribute,   /* name */
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
    ( &(const binstruction[71]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E03FF,  //  0004  LDINT	R7	1024
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
      0x58280009,  //  001F  LDCONST	R10	K9
      0x7C1C0600,  //  0020  CALL	R7	3
      0x80040E00,  //  0021  RET	1	R7
      0x70020018,  //  0022  JMP		#003C
      0x1C1C0D0A,  //  0023  EQ	R7	R6	K10
      0x781E0005,  //  0024  JMPF	R7	#002B
      0x8C1C0706,  //  0025  GETMET	R7	R3	K6
      0x88240907,  //  0026  GETMBR	R9	R4	K7
      0x542AFFFD,  //  0027  LDINT	R10	65534
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
        { be_const_key_weak(TYPES, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(262, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(read_attribute, 7), be_const_closure(Matter_Plugin_Bridge_Sensor_Illuminance_read_attribute_closure) },
        { be_const_key_weak(value_changed, 6), be_const_closure(Matter_Plugin_Bridge_Sensor_Illuminance_value_changed_closure) },
        { be_const_key_weak(DISPLAY_NAME, 4), be_nested_str_weak(Illuminance) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_illuminance) },
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
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
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
        { be_const_key_int(1024, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(5,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
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
