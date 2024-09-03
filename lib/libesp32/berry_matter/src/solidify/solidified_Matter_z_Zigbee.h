/* Solidification of Matter_z_Zigbee.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Zigbee_Mapper' ktab size: 21, total: 32 (saved 88 bytes)
static const bvalue be_ktab_class_Matter_Zigbee_Mapper[21] = {
  /* K0   */  be_nested_str_weak(zigbee),
  /* K1   */  be_nested_str_weak(device_arg),
  /* K2   */  be_nested_str_weak(shortaddr),
  /* K3   */  be_nested_str_weak(zigbee_device),
  /* K4   */  be_nested_str_weak(find),
  /* K5   */  be_nested_str_weak(log),
  /* K6   */  be_nested_str_weak(MTR_X3A_X20cannot_X20find_X20zigbee_X20device_X20_X27_X25s_X27),
  /* K7   */  be_const_int(3),
  /* K8   */  be_nested_str_weak(string),
  /* K9   */  be_nested_str_weak(pi),
  /* K10  */  be_nested_str_weak(ARG),
  /* K11  */  be_nested_str_weak(startswith),
  /* K12  */  be_nested_str_weak(0x),
  /* K13  */  be_nested_str_weak(0X),
  /* K14  */  be_nested_str_weak(tasmota),
  /* K15  */  be_nested_str_weak(set_timer),
  /* K16  */  be_nested_str_weak(resolve_zb_device),
  /* K17  */  be_nested_str_weak(info),
  /* K18  */  be_nested_str_weak(read_zb_info),
  /* K19  */  be_nested_str_weak(MTR_X3A_X20Read_X20information_X20for_X20zigbee_X20device_X200x_X2504X),
  /* K20  */  be_nested_str_weak(zigbee_received),
};


extern const bclass be_class_Matter_Zigbee_Mapper;

/********************************************************************
** Solidified function: resolve_zb_device
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_resolve_zb_device,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(resolve_zb_device),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0002  LDNIL	R3
      0x1C080403,  //  0003  EQ	R2	R2	R3
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x50080000,  //  0005  LDBOOL	R2	0	0
      0x80040400,  //  0006  RET	1	R2
      0x88080102,  //  0007  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0008  LDNIL	R3
      0x20080403,  //  0009  NE	R2	R2	R3
      0x780A0001,  //  000A  JMPF	R2	#000D
      0x50080200,  //  000B  LDBOOL	R2	1	0
      0x80040400,  //  000C  RET	1	R2
      0x8C080304,  //  000D  GETMET	R2	R1	K4
      0x88100101,  //  000E  GETMBR	R4	R0	K1
      0x7C080400,  //  000F  CALL	R2	2
      0x90020602,  //  0010  SETMBR	R0	K3	R2
      0x88080103,  //  0011  GETMBR	R2	R0	K3
      0x780A0005,  //  0012  JMPF	R2	#0019
      0x88080103,  //  0013  GETMBR	R2	R0	K3
      0x88080502,  //  0014  GETMBR	R2	R2	K2
      0x90020402,  //  0015  SETMBR	R0	K2	R2
      0x50080200,  //  0016  LDBOOL	R2	1	0
      0x80040400,  //  0017  RET	1	R2
      0x70020008,  //  0018  JMP		#0022
      0xB80A0A00,  //  0019  GETNGBL	R2	K5
      0x600C0018,  //  001A  GETGBL	R3	G24
      0x58100006,  //  001B  LDCONST	R4	K6
      0x88140101,  //  001C  GETMBR	R5	R0	K1
      0x7C0C0400,  //  001D  CALL	R3	2
      0x58100007,  //  001E  LDCONST	R4	K7
      0x7C080400,  //  001F  CALL	R2	2
      0x50080000,  //  0020  LDBOOL	R2	0	0
      0x80040400,  //  0021  RET	1	R2
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_parse_configuration,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(probe_zb_values),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E1000,  //  0001  IMPORT	R3	K8
      0x8C100304,  //  0002  GETMET	R4	R1	K4
      0x88180109,  //  0003  GETMBR	R6	R0	K9
      0x88180D0A,  //  0004  GETMBR	R6	R6	K10
      0x4C1C0000,  //  0005  LDNIL	R7
      0x7C100600,  //  0006  CALL	R4	3
      0x90020204,  //  0007  SETMBR	R0	K1	R4
      0x60100004,  //  0008  GETGBL	R4	G4
      0x88140101,  //  0009  GETMBR	R5	R0	K1
      0x7C100200,  //  000A  CALL	R4	1
      0x1C100908,  //  000B  EQ	R4	R4	K8
      0x7812000D,  //  000C  JMPF	R4	#001B
      0x8C10070B,  //  000D  GETMET	R4	R3	K11
      0x88180101,  //  000E  GETMBR	R6	R0	K1
      0x581C000C,  //  000F  LDCONST	R7	K12
      0x7C100600,  //  0010  CALL	R4	3
      0x74120004,  //  0011  JMPT	R4	#0017
      0x8C10070B,  //  0012  GETMET	R4	R3	K11
      0x88180101,  //  0013  GETMBR	R6	R0	K1
      0x581C000D,  //  0014  LDCONST	R7	K13
      0x7C100600,  //  0015  CALL	R4	3
      0x78120003,  //  0016  JMPF	R4	#001B
      0x60100009,  //  0017  GETGBL	R4	G9
      0x88140101,  //  0018  GETMBR	R5	R0	K1
      0x7C100200,  //  0019  CALL	R4	1
      0x90020204,  //  001A  SETMBR	R0	K1	R4
      0x88100101,  //  001B  GETMBR	R4	R0	K1
      0x4C140000,  //  001C  LDNIL	R5
      0x20100805,  //  001D  NE	R4	R4	R5
      0x78120004,  //  001E  JMPF	R4	#0024
      0xB8121C00,  //  001F  GETNGBL	R4	K14
      0x8C10090F,  //  0020  GETMET	R4	R4	K15
      0x541A0063,  //  0021  LDINT	R6	100
      0x841C0000,  //  0022  CLOSURE	R7	P0
      0x7C100600,  //  0023  CALL	R4	3
      0xA0000000,  //  0024  CLOSE	R0
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_zb_info
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_read_zb_info,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(read_zb_info),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040110,  //  0000  GETMET	R1	R0	K16
      0x7C040200,  //  0001  CALL	R1	1
      0x78060009,  //  0002  JMPF	R1	#000D
      0xA4060000,  //  0003  IMPORT	R1	K0
      0x8C080304,  //  0004  GETMET	R2	R1	K4
      0x88100102,  //  0005  GETMBR	R4	R0	K2
      0x7C080400,  //  0006  CALL	R2	2
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0002,  //  0009  JMPF	R3	#000D
      0x8C0C0511,  //  000A  GETMET	R3	R2	K17
      0x7C0C0200,  //  000B  CALL	R3	1
      0x80040600,  //  000C  RET	1	R3
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90021201,  //  0000  SETMBR	R0	K9	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: probe_zb_values
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_probe_zb_values,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(probe_zb_values),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C040112,  //  0000  GETMET	R1	R0	K18
      0x7C040200,  //  0001  CALL	R1	1
      0x4C080000,  //  0002  LDNIL	R2
      0x20080202,  //  0003  NE	R2	R1	R2
      0x780A000B,  //  0004  JMPF	R2	#0011
      0xB80A0A00,  //  0005  GETNGBL	R2	K5
      0x600C0018,  //  0006  GETGBL	R3	G24
      0x58100013,  //  0007  LDCONST	R4	K19
      0x88140102,  //  0008  GETMBR	R5	R0	K2
      0x7C0C0400,  //  0009  CALL	R3	2
      0x58100007,  //  000A  LDCONST	R4	K7
      0x7C080400,  //  000B  CALL	R2	2
      0x88080109,  //  000C  GETMBR	R2	R0	K9
      0x8C080514,  //  000D  GETMET	R2	R2	K20
      0x4C100000,  //  000E  LDNIL	R4
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C080600,  //  0010  CALL	R2	3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Zigbee_Mapper
********************************************************************/
be_local_class(Matter_Zigbee_Mapper,
    4,
    NULL,
    be_nested_map(9,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(shortaddr, -1), be_const_var(3) },
        { be_const_key_weak(resolve_zb_device, -1), be_const_closure(class_Matter_Zigbee_Mapper_resolve_zb_device_closure) },
        { be_const_key_weak(device_arg, -1), be_const_var(1) },
        { be_const_key_weak(read_zb_info, -1), be_const_closure(class_Matter_Zigbee_Mapper_read_zb_info_closure) },
        { be_const_key_weak(zigbee_device, -1), be_const_var(2) },
        { be_const_key_weak(probe_zb_values, 3), be_const_closure(class_Matter_Zigbee_Mapper_probe_zb_values_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Zigbee_Mapper_init_closure) },
        { be_const_key_weak(parse_configuration, 5), be_const_closure(class_Matter_Zigbee_Mapper_parse_configuration_closure) },
        { be_const_key_weak(pi, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Zigbee_Mapper)
);

/********************************************************************
** Solidified function: matter_zigbee_init
********************************************************************/
be_local_closure(matter_zigbee_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(Matter_Zigbee),
    }),
    be_str_weak(matter_zigbee_init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'Matter_Zigbee' ktab size: 11, total: 12 (saved 8 bytes)
static const bvalue be_ktab_class_Matter_Zigbee[11] = {
  /* K0   */  be_nested_str_weak(zigbee),
  /* K1   */  be_nested_str_weak(device),
  /* K2   */  be_nested_str_weak(add_handler),
  /* K3   */  be_nested_str_weak(plugins),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(ZIGBEE),
  /* K6   */  be_nested_str_weak(zigbee_mapper),
  /* K7   */  be_nested_str_weak(resolve_zb_device),
  /* K8   */  be_nested_str_weak(shortaddr),
  /* K9   */  be_nested_str_weak(zigbee_received),
  /* K10  */  be_const_int(1),
};


extern const bclass be_class_Matter_Zigbee;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Zigbee_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x8C0C0502,  //  0002  GETMET	R3	R2	K2
      0x5C140000,  //  0003  MOVE	R5	R0
      0x7C0C0400,  //  0004  CALL	R3	2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attributes_final
********************************************************************/
be_local_closure(class_Matter_Zigbee_attributes_final,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee,     /* shared constants */
    be_str_weak(attributes_final),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x88140101,  //  0000  GETMBR	R5	R0	K1
      0x88140B03,  //  0001  GETMBR	R5	R5	K3
      0x58180004,  //  0002  LDCONST	R6	K4
      0x601C000C,  //  0003  GETGBL	R7	G12
      0x5C200A00,  //  0004  MOVE	R8	R5
      0x7C1C0200,  //  0005  CALL	R7	1
      0x141C0C07,  //  0006  LT	R7	R6	R7
      0x781E0012,  //  0007  JMPF	R7	#001B
      0x941C0A06,  //  0008  GETIDX	R7	R5	R6
      0x88200F05,  //  0009  GETMBR	R8	R7	K5
      0x7822000D,  //  000A  JMPF	R8	#0019
      0x88200F06,  //  000B  GETMBR	R8	R7	K6
      0x7822000B,  //  000C  JMPF	R8	#0019
      0x88200F06,  //  000D  GETMBR	R8	R7	K6
      0x8C201107,  //  000E  GETMET	R8	R8	K7
      0x7C200200,  //  000F  CALL	R8	1
      0x78220007,  //  0010  JMPF	R8	#0019
      0x88200F06,  //  0011  GETMBR	R8	R7	K6
      0x88201108,  //  0012  GETMBR	R8	R8	K8
      0x1C201004,  //  0013  EQ	R8	R8	R4
      0x78220003,  //  0014  JMPF	R8	#0019
      0x8C200F09,  //  0015  GETMET	R8	R7	K9
      0x5C280400,  //  0016  MOVE	R10	R2
      0x5C2C0600,  //  0017  MOVE	R11	R3
      0x7C200600,  //  0018  CALL	R8	3
      0x00180D0A,  //  0019  ADD	R6	R6	K10
      0x7001FFE7,  //  001A  JMP		#0003
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Zigbee
********************************************************************/
be_local_class(Matter_Zigbee,
    1,
    NULL,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, 3), be_const_closure(class_Matter_Zigbee_init_closure) },
        { be_const_key_weak(Matter_Zigbee_Mapper, 4), be_const_class(be_class_Matter_Zigbee_Mapper) },
        { be_const_key_weak(attributes_final, -1), be_const_closure(class_Matter_Zigbee_attributes_final_closure) },
        { be_const_key_weak(_CLASSES_TYPES, -1), be_nested_str_weak(_X2Dzigbee_X7Cz_temp_X7Cz_pressure_X7Cz_humidity) },
        { be_const_key_weak(device, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Zigbee)
);

/********************************************************************
** Solidified module: matter_zigbee
********************************************************************/
be_local_module(matter_zigbee,
    "matter_zigbee",
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(Matter_Zigbee, -1), be_const_class(be_class_Matter_Zigbee) },
        { be_const_key_weak(init, 0), be_const_closure(matter_zigbee_init_closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(matter_zigbee);
/********************************************************************/
/********************************************************************/
/* End of solidification */
