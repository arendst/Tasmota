/* Solidification of Matter_Plugin_2_Sensor_Occupancy.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Occupancy, 
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(shadow_occupancy),
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(U1),
    /* K8   */  be_nested_str_weak(NULL),
    /* K9   */  be_const_int(1),
    /* K10  */  be_const_int(3),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0405,  //  0004  LDINT	R7	1030
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0020,  //  0006  JMPF	R7	#0028
      0x1C1C0D04,  //  0007  EQ	R7	R6	K4
      0x781E000F,  //  0008  JMPF	R7	#0019
      0x881C0105,  //  0009  GETMBR	R7	R0	K5
      0x4C200000,  //  000A  LDNIL	R8
      0x201C0E08,  //  000B  NE	R7	R7	R8
      0x781E0005,  //  000C  JMPF	R7	#0013
      0x8C1C0706,  //  000D  GETMET	R7	R3	K6
      0x88240907,  //  000E  GETMBR	R9	R4	K7
      0x88280105,  //  000F  GETMBR	R10	R0	K5
      0x7C1C0600,  //  0010  CALL	R7	3
      0x80040E00,  //  0011  RET	1	R7
      0x70020004,  //  0012  JMP		#0018
      0x8C1C0706,  //  0013  GETMET	R7	R3	K6
      0x88240908,  //  0014  GETMBR	R9	R4	K8
      0x4C280000,  //  0015  LDNIL	R10
      0x7C1C0600,  //  0016  CALL	R7	3
      0x80040E00,  //  0017  RET	1	R7
      0x7002000E,  //  0018  JMP		#0028
      0x1C1C0D09,  //  0019  EQ	R7	R6	K9
      0x781E0005,  //  001A  JMPF	R7	#0021
      0x8C1C0706,  //  001B  GETMET	R7	R3	K6
      0x88240907,  //  001C  GETMBR	R9	R4	K7
      0x5828000A,  //  001D  LDCONST	R10	K10
      0x7C1C0600,  //  001E  CALL	R7	3
      0x80040E00,  //  001F  RET	1	R7
      0x70020006,  //  0020  JMP		#0028
      0x1C1C0D0B,  //  0021  EQ	R7	R6	K11
      0x781E0004,  //  0022  JMPF	R7	#0028
      0x8C1C0706,  //  0023  GETMET	R7	R3	K6
      0x88240907,  //  0024  GETMBR	R9	R4	K7
      0x58280004,  //  0025  LDCONST	R10	K4
      0x7C1C0600,  //  0026  CALL	R7	3
      0x80040E00,  //  0027  RET	1	R7
      0x601C0003,  //  0028  GETGBL	R7	G3
      0x5C200000,  //  0029  MOVE	R8	R0
      0x7C1C0200,  //  002A  CALL	R7	1
      0x8C1C0F0C,  //  002B  GETMET	R7	R7	K12
      0x5C240200,  //  002C  MOVE	R9	R1
      0x5C280400,  //  002D  MOVE	R10	R2
      0x5C2C0600,  //  002E  MOVE	R11	R3
      0x7C1C0800,  //  002F  CALL	R7	4
      0x80040E00,  //  0030  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Occupancy, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_occupancy),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x50100000,  //  0008  LDBOOL	R4	0	0
      0x90020204,  //  0009  SETMBR	R0	K1	R4
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values_prefix
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Occupancy, 
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(get_name),
    /* K2   */  be_nested_str_weak(Switch),
    /* K3   */  be_nested_str_weak(tasmota_switch_index),
    /* K4   */  be_nested_str_weak(content_send),
    /* K5   */  be_nested_str_weak(PREFIX),
    /* K6   */  be_nested_str_weak(html_escape),
    /* K7   */  be_nested_str_weak(),
    }),
    be_str_weak(web_values_prefix),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E0403,  //  0008  ADD	R3	K2	R3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C0304,  //  000A  GETMET	R3	R1	K4
      0x60140018,  //  000B  GETGBL	R5	G24
      0x88180105,  //  000C  GETMBR	R6	R0	K5
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C1C0306,  //  000E  GETMET	R7	R1	K6
      0x5C240400,  //  000F  MOVE	R9	R2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020000,  //  0011  JMP		#0013
      0x581C0007,  //  0012  LDCONST	R7	K7
      0x7C140400,  //  0013  CALL	R5	2
      0x7C0C0400,  //  0014  CALL	R3	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_update_shadow,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Occupancy, 
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(update_shadow),
    /* K1   */  be_nested_str_weak(VIRTUAL),
    /* K2   */  be_nested_str_weak(Switch),
    /* K3   */  be_nested_str_weak(tasmota_switch_index),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(cmd),
    /* K6   */  be_nested_str_weak(Status_X2010),
    /* K7   */  be_nested_str_weak(find),
    /* K8   */  be_nested_str_weak(StatusSNS),
    /* K9   */  be_nested_str_weak(contains),
    /* K10  */  be_nested_str_weak(ON),
    /* K11  */  be_nested_str_weak(shadow_occupancy),
    /* K12  */  be_nested_str_weak(attribute_updated),
    /* K13  */  be_const_int(0),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x88040101,  //  0005  GETMBR	R1	R0	K1
      0x74060022,  //  0006  JMPT	R1	#002A
      0x60040008,  //  0007  GETGBL	R1	G8
      0x88080103,  //  0008  GETMBR	R2	R0	K3
      0x7C040200,  //  0009  CALL	R1	1
      0x00060401,  //  000A  ADD	R1	K2	R1
      0xB80A0800,  //  000B  GETNGBL	R2	K4
      0x8C080505,  //  000C  GETMET	R2	R2	K5
      0x58100006,  //  000D  LDCONST	R4	K6
      0x50140200,  //  000E  LDBOOL	R5	1	0
      0x7C080600,  //  000F  CALL	R2	3
      0x4C0C0000,  //  0010  LDNIL	R3
      0x200C0403,  //  0011  NE	R3	R2	R3
      0x780E0003,  //  0012  JMPF	R3	#0017
      0x8C0C0507,  //  0013  GETMET	R3	R2	K7
      0x58140008,  //  0014  LDCONST	R5	K8
      0x7C0C0400,  //  0015  CALL	R3	2
      0x5C080600,  //  0016  MOVE	R2	R3
      0x4C0C0000,  //  0017  LDNIL	R3
      0x200C0403,  //  0018  NE	R3	R2	R3
      0x780E000F,  //  0019  JMPF	R3	#002A
      0x8C0C0509,  //  001A  GETMET	R3	R2	K9
      0x5C140200,  //  001B  MOVE	R5	R1
      0x7C0C0400,  //  001C  CALL	R3	2
      0x780E000B,  //  001D  JMPF	R3	#002A
      0x8C0C0507,  //  001E  GETMET	R3	R2	K7
      0x5C140200,  //  001F  MOVE	R5	R1
      0x7C0C0400,  //  0020  CALL	R3	2
      0x1C0C070A,  //  0021  EQ	R3	R3	K10
      0x8810010B,  //  0022  GETMBR	R4	R0	K11
      0x20100803,  //  0023  NE	R4	R4	R3
      0x78120003,  //  0024  JMPF	R4	#0029
      0x8C10010C,  //  0025  GETMET	R4	R0	K12
      0x541A0405,  //  0026  LDINT	R6	1030
      0x581C000D,  //  0027  LDCONST	R7	K13
      0x7C100600,  //  0028  CALL	R4	3
      0x90021603,  //  0029  SETMBR	R0	K11	R3
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_update_virtual,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Occupancy, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(val_onoff),
    /* K1   */  be_nested_str_weak(_parse_update_virtual),
    /* K2   */  be_nested_str_weak(Occupancy),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(update_virtual),
    }),
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x8C080101,  //  0000  GETMET	R2	R0	K1
      0x5C100200,  //  0001  MOVE	R4	R1
      0x58140002,  //  0002  LDCONST	R5	K2
      0x88180100,  //  0003  GETMBR	R6	R0	K0
      0x601C0017,  //  0004  GETGBL	R7	G23
      0x54220405,  //  0005  LDINT	R8	1030
      0x58240003,  //  0006  LDCONST	R9	K3
      0x7C080E00,  //  0007  CALL	R2	7
      0x90020002,  //  0008  SETMBR	R0	K0	R2
      0x60080003,  //  0009  GETGBL	R2	G3
      0x5C0C0000,  //  000A  MOVE	R3	R0
      0x7C080200,  //  000B  CALL	R2	1
      0x8C080504,  //  000C  GETMET	R2	R2	K4
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_parse_status,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Occupancy, 
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
    be_str_weak(parse_status),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x540E0009,  //  0000  LDINT	R3	10
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
** Solidified function: parse_configuration
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_parse_configuration,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Occupancy, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota_switch_index),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    /* K3   */  be_const_int(1),
    /* K4   */  be_const_int(0),
    }),
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x8C0C0301,  //  0001  GETMET	R3	R1	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x58180003,  //  0003  LDCONST	R6	K3
      0x7C0C0600,  //  0004  CALL	R3	3
      0x7C080200,  //  0005  CALL	R2	1
      0x90020002,  //  0006  SETMBR	R0	K0	R2
      0x88080100,  //  0007  GETMBR	R2	R0	K0
      0x18080504,  //  0008  LE	R2	R2	K4
      0x780A0000,  //  0009  JMPF	R2	#000B
      0x90020103,  //  000A  SETMBR	R0	K0	K3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Occupancy;
be_local_closure(class_Matter_Plugin_Sensor_Occupancy_web_values,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Occupancy, 
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(web_values_prefix),
    /* K2   */  be_nested_str_weak(content_send),
    /* K3   */  be_nested_str_weak(Occupancy_X25i_X20_X25s),
    /* K4   */  be_nested_str_weak(tasmota_switch_index),
    /* K5   */  be_nested_str_weak(web_value_onoff),
    /* K6   */  be_nested_str_weak(shadow_occupancy),
    }),
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080302,  //  0003  GETMET	R2	R1	K2
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140003,  //  0005  LDCONST	R5	K3
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x8C1C0105,  //  0007  GETMET	R7	R0	K5
      0x88240106,  //  0008  GETMBR	R9	R0	K6
      0x7C1C0400,  //  0009  CALL	R7	2
      0x7C100600,  //  000A  CALL	R4	3
      0x7C080400,  //  000B  CALL	R2	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Occupancy__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL, 
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
** Solidified class: Matter_Plugin_Sensor_Occupancy
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor_Occupancy,
    2,
    &be_class_Matter_Plugin_Device,
    be_nested_map(19,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_read_attribute_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(263, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(shadow_occupancy, -1), be_const_var(1) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Occupancy),
    }))    ) } )) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(750) },
        { be_const_key_weak(DISPLAY_NAME, 6), be_nested_str_weak(Occupancy) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_web_values_closure) },
        { be_const_key_weak(parse_configuration, 4), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_parse_configuration_closure) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_web_values_prefix_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_update_shadow_closure) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_update_virtual_closure) },
        { be_const_key_weak(parse_status, 16), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_parse_status_closure) },
        { be_const_key_weak(ARG_HINT, 18), be_nested_str_weak(Switch_X3Cx_X3E_X20number) },
        { be_const_key_weak(ARG, 12), be_nested_str_weak(switch) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(1030, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(9,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(29, 0), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(tasmota_switch_index, 7), be_const_var(0) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(occupancy) },
        { be_const_key_weak(ARG_TYPE, 2), be_const_static_closure(class_Matter_Plugin_Sensor_Occupancy__X3Clambda_X3E_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Sensor_Occupancy_init_closure) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Occupancy)
);
/********************************************************************/
/* End of solidification */
