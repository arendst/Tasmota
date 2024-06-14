/* Solidification of Matter_Plugin_2_Sensor_Boolean.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Sensor_Boolean;

/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Boolean;
be_local_closure(class_Matter_Plugin_Sensor_Boolean_parse_configuration,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Boolean, 
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
** Solidified function: value_updated
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Boolean;
be_local_closure(class_Matter_Plugin_Sensor_Boolean_value_updated,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Boolean, 
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(value_updated),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(class_Matter_Plugin_Sensor_Boolean__X3Clambda_X3E,   /* name */
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
** Solidified function: update_shadow
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Boolean;
be_local_closure(class_Matter_Plugin_Sensor_Boolean_update_shadow,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Boolean, 
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
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
    /* K11  */  be_nested_str_weak(shadow_bool_value),
    /* K12  */  be_nested_str_weak(value_updated),
    }),
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x60040003,  //  0000  GETGBL	R1	G3
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x8C040300,  //  0003  GETMET	R1	R1	K0
      0x7C040200,  //  0004  CALL	R1	1
      0x88040101,  //  0005  GETMBR	R1	R0	K1
      0x74060020,  //  0006  JMPT	R1	#0028
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
      0x780E000D,  //  0019  JMPF	R3	#0028
      0x8C0C0509,  //  001A  GETMET	R3	R2	K9
      0x5C140200,  //  001B  MOVE	R5	R1
      0x7C0C0400,  //  001C  CALL	R3	2
      0x780E0009,  //  001D  JMPF	R3	#0028
      0x8C0C0507,  //  001E  GETMET	R3	R2	K7
      0x5C140200,  //  001F  MOVE	R5	R1
      0x7C0C0400,  //  0020  CALL	R3	2
      0x1C0C070A,  //  0021  EQ	R3	R3	K10
      0x8810010B,  //  0022  GETMBR	R4	R0	K11
      0x20100803,  //  0023  NE	R4	R4	R3
      0x78120001,  //  0024  JMPF	R4	#0027
      0x8C10010C,  //  0025  GETMET	R4	R0	K12
      0x7C100200,  //  0026  CALL	R4	1
      0x90021603,  //  0027  SETMBR	R0	K11	R3
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Boolean;
be_local_closure(class_Matter_Plugin_Sensor_Boolean_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Boolean, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(shadow_bool_value),
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
** Solidified function: parse_status
********************************************************************/
extern const bclass be_class_Matter_Plugin_Sensor_Boolean;
be_local_closure(class_Matter_Plugin_Sensor_Boolean_parse_status,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_Plugin_Sensor_Boolean, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(find),
    /* K1   */  be_nested_str_weak(Switch),
    /* K2   */  be_nested_str_weak(tasmota_switch_index),
    /* K3   */  be_nested_str_weak(ON),
    /* K4   */  be_nested_str_weak(shadow_bool_value),
    /* K5   */  be_nested_str_weak(value_updated),
    }),
    be_str_weak(parse_status),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0x540E0009,  //  0000  LDINT	R3	10
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0015,  //  0002  JMPF	R3	#0019
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
      0x78120007,  //  000F  JMPF	R4	#0018
      0x88100104,  //  0010  GETMBR	R4	R0	K4
      0x60140017,  //  0011  GETGBL	R5	G23
      0x5C180600,  //  0012  MOVE	R6	R3
      0x7C140200,  //  0013  CALL	R5	1
      0x20100805,  //  0014  NE	R4	R4	R5
      0x78120001,  //  0015  JMPF	R4	#0018
      0x8C100105,  //  0016  GETMET	R4	R0	K5
      0x7C100200,  //  0017  CALL	R4	1
      0x90020803,  //  0018  SETMBR	R0	K4	R3
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Sensor_Boolean
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Sensor_Boolean,
    2,
    &be_class_Matter_Plugin_Device,
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(shadow_bool_value, -1), be_const_var(1) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Switch_X3Cx_X3E_X20number) },
        { be_const_key_weak(parse_status, -1), be_const_closure(class_Matter_Plugin_Sensor_Boolean_parse_status_closure) },
        { be_const_key_weak(value_updated, -1), be_const_closure(class_Matter_Plugin_Sensor_Boolean_value_updated_closure) },
        { be_const_key_weak(tasmota_switch_index, -1), be_const_var(0) },
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(class_Matter_Plugin_Sensor_Boolean__X3Clambda_X3E_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(class_Matter_Plugin_Sensor_Boolean_update_shadow_closure) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(switch) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(750) },
        { be_const_key_weak(init, 8), be_const_closure(class_Matter_Plugin_Sensor_Boolean_init_closure) },
        { be_const_key_weak(parse_configuration, 2), be_const_closure(class_Matter_Plugin_Sensor_Boolean_parse_configuration_closure) },
    })),
    be_str_weak(Matter_Plugin_Sensor_Boolean)
);
/********************************************************************/
/* End of solidification */
