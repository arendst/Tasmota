/* Solidification of Matter_Plugin_Bridge_Sensor_Occupancy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Bridge_Sensor_Occupancy;

/********************************************************************
** Solidified function: parse_update
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Occupancy_parse_update,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(Switch),
    /* K2   */  be_nested_str_weak(tasmota_switch_index),
    /* K3   */  be_nested_str_weak(ON),
    /* K4   */  be_nested_str_weak(shadow_occupancy),
    /* K5   */  be_nested_str_weak(attribute_updated),
    /* K6   */  be_const_int(0),
    }),
    be_str_weak(parse_update),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x540E0007,  //  0000  LDINT	R3	8
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0017,  //  0002  JMPF	R3	#001B
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x8C100300,  //  0004  GETMET	R4	R1	K0
      0x60180008,  //  0005  GETGBL	R6	G8
      0x881C0102,  //  0006  GETMBR	R7	R0	K2
      0x7C180200,  //  0007  CALL	R6	1
      0x001A0206,  //  0008  ADD	R6	K1	R6
      0x7C100400,  //  0009  CALL	R4	2
      0x1C100903,  //  000A  EQ	R4	R4	K3
      0x5C0C0800,  //  000B  MOVE	R3	R4
      0x88100104,  //  000C  GETMBR	R4	R0	K4
      0x4C140000,  //  000D  LDNIL	R5
      0x20100805,  //  000E  NE	R4	R4	R5
      0x78120009,  //  000F  JMPF	R4	#001A
      0x88100104,  //  0010  GETMBR	R4	R0	K4
      0x60140017,  //  0011  GETGBL	R5	G23
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C140200,  //  0013  CALL	R5	1
      0x20100805,  //  0014  NE	R4	R4	R5
      0x78120003,  //  0015  JMPF	R4	#001A
      0x8C100105,  //  0016  GETMET	R4	R0	K5
      0x541A0405,  //  0017  LDINT	R6	1030
      0x581C0006,  //  0018  LDCONST	R7	K6
      0x7C100600,  //  0019  CALL	R4	3
      0x90020803,  //  001A  SETMBR	R0	K4	R3
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Occupancy_web_values,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X7C_X20Occupancy_X25i_X20_X25s),
    /* K5   */  be_nested_str_weak(tasmota_switch_index),
    /* K6   */  be_nested_str_weak(web_value_onoff),
    /* K7   */  be_nested_str_weak(shadow_occupancy),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x8C240106,  //  0006  GETMET	R9	R0	K6
      0x882C0107,  //  0007  GETMBR	R11	R0	K7
      0x7C240400,  //  0008  CALL	R9	2
      0x7C140800,  //  0009  CALL	R5	4
      0x7C0C0400,  //  000A  CALL	R3	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Occupancy__X3Clambda_X3E,   /* name */
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
be_local_closure(Matter_Plugin_Bridge_Sensor_Occupancy_read_attribute,   /* name */
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
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Plugin_Bridge_Sensor_Occupancy_init,   /* name */
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
** Solidified class: Matter_Plugin_Bridge_Sensor_Occupancy
********************************************************************/
extern const bclass be_class_Matter_Plugin_Bridge_HTTP;
be_local_class(Matter_Plugin_Bridge_Sensor_Occupancy,
    2,
    &be_class_Matter_Plugin_Bridge_HTTP,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG_HINT, 3), be_nested_str_weak(Enter_X20Switch_X3Cx_X3E_X20number) },
        { be_const_key_weak(parse_update, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Occupancy_parse_update_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Occupancy_web_values_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Occupancy_init_closure) },
        { be_const_key_weak(ARG_TYPE, 11), be_const_static_closure(Matter_Plugin_Bridge_Sensor_Occupancy__X3Clambda_X3E_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(263, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(tasmota_switch_index, -1), be_const_var(0) },
        { be_const_key_weak(ARG, 13), be_nested_str_weak(switch) },
        { be_const_key_weak(UPDATE_CMD, 8), be_nested_str_weak(Status_X208) },
        { be_const_key_weak(shadow_occupancy, -1), be_const_var(1) },
        { be_const_key_weak(CLUSTERS, 14), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
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
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Bridge_Sensor_Occupancy_read_attribute_closure) },
        { be_const_key_weak(NAME, -1), be_nested_str_weak(Occupancy) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(http_occupancy) },
    })),
    be_str_weak(Matter_Plugin_Bridge_Sensor_Occupancy)
);
/*******************************************************************/

void be_load_Matter_Plugin_Bridge_Sensor_Occupancy_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Bridge_Sensor_Occupancy);
    be_setglobal(vm, "Matter_Plugin_Bridge_Sensor_Occupancy");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
