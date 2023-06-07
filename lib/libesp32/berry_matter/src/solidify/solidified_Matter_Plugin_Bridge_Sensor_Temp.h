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
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
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
    /* K13  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[69]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x88100902,  //  0002  GETMBR	R4	R4	K2
      0x88140503,  //  0003  GETMBR	R5	R2	K3
      0x88180504,  //  0004  GETMBR	R6	R2	K4
      0x541E0401,  //  0005  LDINT	R7	1026
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
      0x5429EC77,  //  001E  LDINT	R10	-5000
      0x7C1C0600,  //  001F  CALL	R7	3
      0x80040E00,  //  0020  RET	1	R7
      0x70020018,  //  0021  JMP		#003B
      0x1C1C0D0B,  //  0022  EQ	R7	R6	K11
      0x781E0005,  //  0023  JMPF	R7	#002A
      0x8C1C0907,  //  0024  GETMET	R7	R4	K7
      0x88240908,  //  0025  GETMBR	R9	R4	K8
      0x542A3A97,  //  0026  LDINT	R10	15000
      0x7C1C0600,  //  0027  CALL	R7	3
      0x80040E00,  //  0028  RET	1	R7
      0x70020010,  //  0029  JMP		#003B
      0x541EFFFB,  //  002A  LDINT	R7	65532
      0x1C1C0C07,  //  002B  EQ	R7	R6	R7
      0x781E0005,  //  002C  JMPF	R7	#0033
      0x8C1C0907,  //  002D  GETMET	R7	R4	K7
      0x8824090C,  //  002E  GETMBR	R9	R4	K12
      0x58280005,  //  002F  LDCONST	R10	K5
      0x7C1C0600,  //  0030  CALL	R7	3
      0x80040E00,  //  0031  RET	1	R7
      0x70020007,  //  0032  JMP		#003B
      0x541EFFFC,  //  0033  LDINT	R7	65533
      0x1C1C0C07,  //  0034  EQ	R7	R6	R7
      0x781E0004,  //  0035  JMPF	R7	#003B
      0x8C1C0907,  //  0036  GETMET	R7	R4	K7
      0x8824090C,  //  0037  GETMBR	R9	R4	K12
      0x542A0003,  //  0038  LDINT	R10	4
      0x7C1C0600,  //  0039  CALL	R7	3
      0x80040E00,  //  003A  RET	1	R7
      0x70020007,  //  003B  JMP		#0044
      0x601C0003,  //  003C  GETGBL	R7	G3
      0x5C200000,  //  003D  MOVE	R8	R0
      0x7C1C0200,  //  003E  CALL	R7	1
      0x8C1C0F0D,  //  003F  GETMET	R7	R7	K13
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
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X7C_X20_X25s_X20_X26_X23x2600_X3B_X26_X23xFE0F_X3B_X20_X25_X2E1f_X20_XC2_XB0C),
    /* K5   */  be_nested_str_weak(filter_name_html),
    /* K6   */  be_nested_str_weak(shadow_value),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x8C200105,  //  0005  GETMET	R8	R0	K5
      0x7C200200,  //  0006  CALL	R8	1
      0x88240106,  //  0007  GETMBR	R9	R0	K6
      0x4C280000,  //  0008  LDNIL	R10
      0x2024120A,  //  0009  NE	R9	R9	R10
      0x78260005,  //  000A  JMPF	R9	#0011
      0x6024000A,  //  000B  GETGBL	R9	G10
      0x88280106,  //  000C  GETMBR	R10	R0	K6
      0x7C240200,  //  000D  CALL	R9	1
      0x542A0063,  //  000E  LDINT	R10	100
      0x0C24120A,  //  000F  DIV	R9	R9	R10
      0x70020000,  //  0010  JMP		#0012
      0x4C240000,  //  0011  LDNIL	R9
      0x7C140800,  //  0012  CALL	R5	4
      0x7C0C0400,  //  0013  CALL	R3	2
      0x80000000,  //  0014  RET	0
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
        { be_const_key_weak(NAME, -1), be_nested_str_weak(_X26_X23x1F517_X3B_X20Temperature) },
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
