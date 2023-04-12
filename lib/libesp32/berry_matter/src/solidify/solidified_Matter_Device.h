/* Solidification of Matter_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Device;

/********************************************************************
** Solidified function: start_operational_discovery
********************************************************************/
be_local_closure(Matter_Device_start_operational_discovery,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(mdns),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(stop_basic_commissioning),
    /* K4   */  be_nested_str_weak(root_w0),
    /* K5   */  be_nested_str_weak(root_L),
    /* K6   */  be_nested_str_weak(set_expire_in_seconds),
    /* K7   */  be_nested_str_weak(mdns_announce_op_discovery),
    /* K8   */  be_nested_str_weak(get_fabric),
    }),
    be_str_weak(start_operational_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0x8C140103,  //  0003  GETMET	R5	R0	K3
      0x7C140200,  //  0004  CALL	R5	1
      0x4C140000,  //  0005  LDNIL	R5
      0x90020805,  //  0006  SETMBR	R0	K4	R5
      0x4C140000,  //  0007  LDNIL	R5
      0x90020A05,  //  0008  SETMBR	R0	K5	R5
      0x8C140306,  //  0009  GETMET	R5	R1	K6
      0x541E003B,  //  000A  LDINT	R7	60
      0x7C140400,  //  000B  CALL	R5	2
      0x8C140107,  //  000C  GETMET	R5	R0	K7
      0x8C1C0308,  //  000D  GETMET	R7	R1	K8
      0x7C1C0200,  //  000E  CALL	R7	1
      0x7C140400,  //  000F  CALL	R5	2
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _init_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device__init_basic_commissioning,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(count_active_fabrics),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(start_root_basic_commissioning),
    }),
    be_str_weak(_init_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040302,  //  0003  EQ	R1	R1	K2
      0x78060001,  //  0004  JMPF	R1	#0007
      0x8C040103,  //  0005  GETMET	R1	R0	K3
      0x7C040200,  //  0006  CALL	R1	1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_manual_pairing_code
********************************************************************/
be_local_closure(Matter_Device_compute_manual_pairing_code,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(root_discriminator),
    /* K2   */  be_nested_str_weak(root_passcode),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X251i_X2505i_X2504i),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(Verhoeff),
    /* K7   */  be_nested_str_weak(checksum),
    }),
    be_str_weak(compute_manual_pairing_code),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x540E0FFE,  //  0002  LDINT	R3	4095
      0x2C080403,  //  0003  AND	R2	R2	R3
      0x540E0009,  //  0004  LDINT	R3	10
      0x3C080403,  //  0005  SHR	R2	R2	R3
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x541202FF,  //  0007  LDINT	R4	768
      0x2C0C0604,  //  0008  AND	R3	R3	R4
      0x54120005,  //  0009  LDINT	R4	6
      0x380C0604,  //  000A  SHL	R3	R3	R4
      0x88100102,  //  000B  GETMBR	R4	R0	K2
      0x54163FFE,  //  000C  LDINT	R5	16383
      0x2C100805,  //  000D  AND	R4	R4	R5
      0x300C0604,  //  000E  OR	R3	R3	R4
      0x88100102,  //  000F  GETMBR	R4	R0	K2
      0x5416000D,  //  0010  LDINT	R5	14
      0x3C100805,  //  0011  SHR	R4	R4	R5
      0x8C140303,  //  0012  GETMET	R5	R1	K3
      0x581C0004,  //  0013  LDCONST	R7	K4
      0x5C200400,  //  0014  MOVE	R8	R2
      0x5C240600,  //  0015  MOVE	R9	R3
      0x5C280800,  //  0016  MOVE	R10	R4
      0x7C140A00,  //  0017  CALL	R5	5
      0xB81A0A00,  //  0018  GETNGBL	R6	K5
      0x88180D06,  //  0019  GETMBR	R6	R6	K6
      0x8C180D07,  //  001A  GETMET	R6	R6	K7
      0x5C200A00,  //  001B  MOVE	R8	R5
      0x7C180400,  //  001C  CALL	R6	2
      0x00140A06,  //  001D  ADD	R5	R5	R6
      0x80040A00,  //  001E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _trigger_read_sensors
********************************************************************/
be_local_closure(Matter_Device__trigger_read_sensors,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(read_sensors),
    /* K3   */  be_nested_str_weak(load),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(plugins),
    /* K6   */  be_nested_str_weak(parse_sensors),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20unable_X20to_X20parse_X20read_sensors_X3A_X20),
    /* K10  */  be_const_int(3),
    }),
    be_str_weak(_trigger_read_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C0C0403,  //  0005  EQ	R3	R2	R3
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x80000600,  //  0007  RET	0
      0x8C0C0303,  //  0008  GETMET	R3	R1	K3
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x4C100000,  //  000B  LDNIL	R4
      0x20100604,  //  000C  NE	R4	R3	R4
      0x7812000D,  //  000D  JMPF	R4	#001C
      0x58100004,  //  000E  LDCONST	R4	K4
      0x6014000C,  //  000F  GETGBL	R5	G12
      0x88180105,  //  0010  GETMBR	R6	R0	K5
      0x7C140200,  //  0011  CALL	R5	1
      0x14140805,  //  0012  LT	R5	R4	R5
      0x78160006,  //  0013  JMPF	R5	#001B
      0x88140105,  //  0014  GETMBR	R5	R0	K5
      0x94140A04,  //  0015  GETIDX	R5	R5	R4
      0x8C140B06,  //  0016  GETMET	R5	R5	K6
      0x5C1C0600,  //  0017  MOVE	R7	R3
      0x7C140400,  //  0018  CALL	R5	2
      0x00100907,  //  0019  ADD	R4	R4	K7
      0x7001FFF3,  //  001A  JMP		#000F
      0x70020007,  //  001B  JMP		#0024
      0xB8120200,  //  001C  GETNGBL	R4	K1
      0x8C100908,  //  001D  GETMET	R4	R4	K8
      0x60180008,  //  001E  GETGBL	R6	G8
      0x5C1C0400,  //  001F  MOVE	R7	R2
      0x7C180200,  //  0020  CALL	R6	1
      0x001A1206,  //  0021  ADD	R6	K9	R6
      0x581C000A,  //  0022  LDCONST	R7	K10
      0x7C100600,  //  0023  CALL	R4	3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(Matter_Device_start,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
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
        /* K0   */  be_nested_str_weak(_trigger_read_sensors),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80000000,  //  0003  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(started),
    /* K1   */  be_nested_str_weak(plugins),
    /* K2   */  be_nested_str_weak(push),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Plugin_Root),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(autoconf_device),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(add_cron),
    /* K9   */  be_nested_str_weak(_X2A_X2F5_X20_X2A_X20_X2A_X20_X2A_X20_X2A_X20_X2A),
    /* K10  */  be_nested_str_weak(matter_sensors_5s),
    /* K11  */  be_nested_str_weak(_start_udp),
    /* K12  */  be_nested_str_weak(UDP_PORT),
    /* K13  */  be_nested_str_weak(start_mdns_announce_hostnames),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060000,  //  0001  JMPF	R1	#0003
      0x80000200,  //  0002  RET	0
      0x88040101,  //  0003  GETMBR	R1	R0	K1
      0x8C040302,  //  0004  GETMET	R1	R1	K2
      0xB80E0600,  //  0005  GETNGBL	R3	K3
      0x8C0C0704,  //  0006  GETMET	R3	R3	K4
      0x5C140000,  //  0007  MOVE	R5	R0
      0x58180005,  //  0008  LDCONST	R6	K5
      0x7C0C0600,  //  0009  CALL	R3	3
      0x7C040400,  //  000A  CALL	R1	2
      0x8C040106,  //  000B  GETMET	R1	R0	K6
      0x7C040200,  //  000C  CALL	R1	1
      0xB8060E00,  //  000D  GETNGBL	R1	K7
      0x8C040308,  //  000E  GETMET	R1	R1	K8
      0x580C0009,  //  000F  LDCONST	R3	K9
      0x84100000,  //  0010  CLOSURE	R4	P0
      0x5814000A,  //  0011  LDCONST	R5	K10
      0x7C040800,  //  0012  CALL	R1	4
      0x8C04010B,  //  0013  GETMET	R1	R0	K11
      0x880C010C,  //  0014  GETMBR	R3	R0	K12
      0x7C040400,  //  0015  CALL	R1	2
      0x8C04010D,  //  0016  GETMET	R1	R0	K13
      0x7C040200,  //  0017  CALL	R1	1
      0x50040200,  //  0018  LDBOOL	R1	1	0
      0x90020001,  //  0019  SETMBR	R0	K0	R1
      0xA0000000,  //  001A  CLOSE	R0
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_stop_basic_commissioning,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(commissioning_open),
    /* K1   */  be_nested_str_weak(mdns_remove_PASE),
    /* K2   */  be_nested_str_weak(commissioning_iterations),
    /* K3   */  be_nested_str_weak(commissioning_discriminator),
    /* K4   */  be_nested_str_weak(commissioning_salt),
    /* K5   */  be_nested_str_weak(commissioning_w0),
    /* K6   */  be_nested_str_weak(commissioning_L),
    /* K7   */  be_nested_str_weak(commissioning_admin_fabric),
    }),
    be_str_weak(stop_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x4C040000,  //  0004  LDNIL	R1
      0x90020401,  //  0005  SETMBR	R0	K2	R1
      0x4C040000,  //  0006  LDNIL	R1
      0x90020601,  //  0007  SETMBR	R0	K3	R1
      0x4C040000,  //  0008  LDNIL	R1
      0x90020801,  //  0009  SETMBR	R0	K4	R1
      0x4C040000,  //  000A  LDNIL	R1
      0x90020A01,  //  000B  SETMBR	R0	K5	R1
      0x4C040000,  //  000C  LDNIL	R1
      0x90020C01,  //  000D  SETMBR	R0	K6	R1
      0x4C040000,  //  000E  LDNIL	R1
      0x90020E01,  //  000F  SETMBR	R0	K7	R1
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_discovery_deferred
********************************************************************/
be_local_closure(Matter_Device_start_operational_discovery_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_operational_discovery),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_timer),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(start_operational_discovery_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fabric
********************************************************************/
be_local_closure(Matter_Device_remove_fabric,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(find_children_fabrics),
    /* K2   */  be_nested_str_weak(get_fabric_index),
    /* K3   */  be_nested_str_weak(find_fabric_by_index),
    /* K4   */  be_nested_str_weak(message_handler),
    /* K5   */  be_nested_str_weak(im),
    /* K6   */  be_nested_str_weak(subs_shop),
    /* K7   */  be_nested_str_weak(remove_by_fabric),
    /* K8   */  be_nested_str_weak(mdns_remove_op_discovery),
    /* K9   */  be_nested_str_weak(remove_fabric),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(save_fabrics),
    }),
    be_str_weak(remove_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100302,  //  0002  GETMET	R4	R1	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x7C080400,  //  0004  CALL	R2	2
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C0C0403,  //  0006  EQ	R3	R2	R3
      0x780E0000,  //  0007  JMPF	R3	#0009
      0x80000600,  //  0008  RET	0
      0x600C0010,  //  0009  GETGBL	R3	G16
      0x5C100400,  //  000A  MOVE	R4	R2
      0x7C0C0200,  //  000B  CALL	R3	1
      0xA8020016,  //  000C  EXBLK	0	#0024
      0x5C100600,  //  000D  MOVE	R4	R3
      0x7C100000,  //  000E  CALL	R4	0
      0x88140100,  //  000F  GETMBR	R5	R0	K0
      0x8C140B03,  //  0010  GETMET	R5	R5	K3
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x7C140400,  //  0012  CALL	R5	2
      0x4C180000,  //  0013  LDNIL	R6
      0x20180A06,  //  0014  NE	R6	R5	R6
      0x781A000C,  //  0015  JMPF	R6	#0023
      0x88180104,  //  0016  GETMBR	R6	R0	K4
      0x88180D05,  //  0017  GETMBR	R6	R6	K5
      0x88180D06,  //  0018  GETMBR	R6	R6	K6
      0x8C180D07,  //  0019  GETMET	R6	R6	K7
      0x5C200A00,  //  001A  MOVE	R8	R5
      0x7C180400,  //  001B  CALL	R6	2
      0x8C180108,  //  001C  GETMET	R6	R0	K8
      0x5C200A00,  //  001D  MOVE	R8	R5
      0x7C180400,  //  001E  CALL	R6	2
      0x88180100,  //  001F  GETMBR	R6	R0	K0
      0x8C180D09,  //  0020  GETMET	R6	R6	K9
      0x5C200A00,  //  0021  MOVE	R8	R5
      0x7C180400,  //  0022  CALL	R6	2
      0x7001FFE8,  //  0023  JMP		#000D
      0x580C000A,  //  0024  LDCONST	R3	K10
      0xAC0C0200,  //  0025  CATCH	R3	1	0
      0xB0080000,  //  0026  RAISE	2	R0	R0
      0x880C0100,  //  0027  GETMBR	R3	R0	K0
      0x8C0C070B,  //  0028  GETMET	R3	R3	K11
      0x7C0C0200,  //  0029  CALL	R3	1
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: received_ack
********************************************************************/
be_local_closure(Matter_Device_received_ack,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(received_ack),
    }),
    be_str_weak(received_ack),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_root_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_start_root_basic_commissioning,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(PASE_TIMEOUT),
    /* K2   */  be_nested_str_weak(compute_manual_pairing_code),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20Manual_X20pairing_X20code_X3A_X20_X25s_X2D_X25s_X2D_X25s),
    /* K7   */  be_const_int(0),
    /* K8   */  be_const_int(3),
    /* K9   */  be_const_int(2147483647),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(_compute_pbkdf),
    /* K12  */  be_nested_str_weak(root_passcode),
    /* K13  */  be_nested_str_weak(root_iterations),
    /* K14  */  be_nested_str_weak(root_salt),
    /* K15  */  be_nested_str_weak(start_basic_commissioning),
    /* K16  */  be_nested_str_weak(root_discriminator),
    /* K17  */  be_nested_str_weak(root_w0),
    /* K18  */  be_nested_str_weak(root_L),
    }),
    be_str_weak(start_root_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C0C0102,  //  0005  GETMET	R3	R0	K2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x8C100904,  //  0008  GETMET	R4	R4	K4
      0x8C180505,  //  0009  GETMET	R6	R2	K5
      0x58200006,  //  000A  LDCONST	R8	K6
      0x40260F08,  //  000B  CONNECT	R9	K7	K8
      0x94240609,  //  000C  GETIDX	R9	R3	R9
      0x542A0003,  //  000D  LDINT	R10	4
      0x542E0005,  //  000E  LDINT	R11	6
      0x4028140B,  //  000F  CONNECT	R10	R10	R11
      0x9428060A,  //  0010  GETIDX	R10	R3	R10
      0x542E0006,  //  0011  LDINT	R11	7
      0x402C1709,  //  0012  CONNECT	R11	R11	K9
      0x942C060B,  //  0013  GETIDX	R11	R3	R11
      0x7C180A00,  //  0014  CALL	R6	5
      0x581C000A,  //  0015  LDCONST	R7	K10
      0x7C100600,  //  0016  CALL	R4	3
      0x8C10010B,  //  0017  GETMET	R4	R0	K11
      0x8818010C,  //  0018  GETMBR	R6	R0	K12
      0x881C010D,  //  0019  GETMBR	R7	R0	K13
      0x8820010E,  //  001A  GETMBR	R8	R0	K14
      0x7C100800,  //  001B  CALL	R4	4
      0x8C10010F,  //  001C  GETMET	R4	R0	K15
      0x5C180200,  //  001D  MOVE	R6	R1
      0x881C010D,  //  001E  GETMBR	R7	R0	K13
      0x88200110,  //  001F  GETMBR	R8	R0	K16
      0x8824010E,  //  0020  GETMBR	R9	R0	K14
      0x88280111,  //  0021  GETMBR	R10	R0	K17
      0x882C0112,  //  0022  GETMBR	R11	R0	K18
      0x4C300000,  //  0023  LDNIL	R12
      0x7C101000,  //  0024  CALL	R4	8
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_before_restart
********************************************************************/
be_local_closure(Matter_Device_save_before_restart,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(stop_basic_commissioning),
    /* K1   */  be_nested_str_weak(mdns_remove_op_discovery_all_fabrics),
    }),
    be_str_weak(save_before_restart),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_qrcode_content
********************************************************************/
be_local_closure(Matter_Device_compute_qrcode_content,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(resize),
    /* K1   */  be_nested_str_weak(setbits),
    /* K2   */  be_const_int(3),
    /* K3   */  be_nested_str_weak(vendorid),
    /* K4   */  be_nested_str_weak(productid),
    /* K5   */  be_nested_str_weak(root_discriminator),
    /* K6   */  be_nested_str_weak(root_passcode),
    /* K7   */  be_const_int(134217727),
    /* K8   */  be_nested_str_weak(MT_X3A),
    /* K9   */  be_nested_str_weak(matter),
    /* K10  */  be_nested_str_weak(Base38),
    /* K11  */  be_nested_str_weak(encode),
    }),
    be_str_weak(compute_qrcode_content),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x7C040000,  //  0001  CALL	R1	0
      0x8C040300,  //  0002  GETMET	R1	R1	K0
      0x540E000A,  //  0003  LDINT	R3	11
      0x7C040400,  //  0004  CALL	R1	2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x5416000F,  //  0007  LDINT	R5	16
      0x88180103,  //  0008  GETMBR	R6	R0	K3
      0x7C080800,  //  0009  CALL	R2	4
      0x8C080301,  //  000A  GETMET	R2	R1	K1
      0x54120012,  //  000B  LDINT	R4	19
      0x5416000F,  //  000C  LDINT	R5	16
      0x88180104,  //  000D  GETMBR	R6	R0	K4
      0x7C080800,  //  000E  CALL	R2	4
      0x8C080301,  //  000F  GETMET	R2	R1	K1
      0x54120024,  //  0010  LDINT	R4	37
      0x54160007,  //  0011  LDINT	R5	8
      0x541A0003,  //  0012  LDINT	R6	4
      0x7C080800,  //  0013  CALL	R2	4
      0x8C080301,  //  0014  GETMET	R2	R1	K1
      0x5412002C,  //  0015  LDINT	R4	45
      0x5416000B,  //  0016  LDINT	R5	12
      0x88180105,  //  0017  GETMBR	R6	R0	K5
      0x541E0FFE,  //  0018  LDINT	R7	4095
      0x2C180C07,  //  0019  AND	R6	R6	R7
      0x7C080800,  //  001A  CALL	R2	4
      0x8C080301,  //  001B  GETMET	R2	R1	K1
      0x54120038,  //  001C  LDINT	R4	57
      0x5416001A,  //  001D  LDINT	R5	27
      0x88180106,  //  001E  GETMBR	R6	R0	K6
      0x2C180D07,  //  001F  AND	R6	R6	K7
      0x7C080800,  //  0020  CALL	R2	4
      0xB80A1200,  //  0021  GETNGBL	R2	K9
      0x8808050A,  //  0022  GETMBR	R2	R2	K10
      0x8C08050B,  //  0023  GETMET	R2	R2	K11
      0x5C100200,  //  0024  MOVE	R4	R1
      0x7C080400,  //  0025  CALL	R2	2
      0x000A1002,  //  0026  ADD	R2	K8	R2
      0x80040400,  //  0027  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _start_udp
********************************************************************/
be_local_closure(Matter_Device__start_udp,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(msg_received),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20starting_X20UDP_X20server_X20on_X20port_X3A_X20),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(UDPServer),
    /* K7   */  be_nested_str_weak(),
    /* K8   */  be_nested_str_weak(start),
    }),
    be_str_weak(_start_udp),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540615A3,  //  0006  LDINT	R1	5540
      0xB80A0200,  //  0007  GETNGBL	R2	K1
      0x8C080502,  //  0008  GETMET	R2	R2	K2
      0x60100008,  //  0009  GETGBL	R4	G8
      0x5C140200,  //  000A  MOVE	R5	R1
      0x7C100200,  //  000B  CALL	R4	1
      0x00120604,  //  000C  ADD	R4	K3	R4
      0x58140004,  //  000D  LDCONST	R5	K4
      0x7C080600,  //  000E  CALL	R2	3
      0xB80A0A00,  //  000F  GETNGBL	R2	K5
      0x8C080506,  //  0010  GETMET	R2	R2	K6
      0x58100007,  //  0011  LDCONST	R4	K7
      0x5C140200,  //  0012  MOVE	R5	R1
      0x7C080600,  //  0013  CALL	R2	3
      0x90020002,  //  0014  SETMBR	R0	K0	R2
      0x88080100,  //  0015  GETMBR	R2	R0	K0
      0x8C080508,  //  0016  GETMET	R2	R2	K8
      0x84100000,  //  0017  CLOSURE	R4	P0
      0x7C080400,  //  0018  CALL	R2	2
      0xA0000000,  //  0019  CLOSE	R0
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(Matter_Device_attribute_updated,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Path),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(message_handler),
    /* K6   */  be_nested_str_weak(im),
    /* K7   */  be_nested_str_weak(subs_shop),
    /* K8   */  be_nested_str_weak(attribute_updated_ctx),
    }),
    be_str_weak(attribute_updated),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x50100000,  //  0003  LDBOOL	R4	0	0
      0xB8160000,  //  0004  GETNGBL	R5	K0
      0x8C140B01,  //  0005  GETMET	R5	R5	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x90160401,  //  0007  SETMBR	R5	K2	R1
      0x90160602,  //  0008  SETMBR	R5	K3	R2
      0x90160803,  //  0009  SETMBR	R5	K4	R3
      0x88180105,  //  000A  GETMBR	R6	R0	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x88180D07,  //  000C  GETMBR	R6	R6	K7
      0x8C180D08,  //  000D  GETMET	R6	R6	K8
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x5C240800,  //  000F  MOVE	R9	R4
      0x7C180600,  //  0010  CALL	R6	3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(Matter_Device_every_250ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(message_handler),
    /* K1   */  be_nested_str_weak(every_250ms),
    }),
    be_str_weak(every_250ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_attribute_expansion
********************************************************************/
be_local_closure(Matter_Device_process_attribute_expansion,   /* name */
  be_nested_proto(
    30,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        7,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(keys),
        /* K1   */  be_nested_str_weak(push),
        /* K2   */  be_nested_str_weak(stop_iteration),
        /* K3   */  be_const_int(1),
        /* K4   */  be_const_int(0),
        }),
        be_str_weak(keys_sorted),
        &be_const_str_solidified,
        ( &(const binstruction[45]) {  /* code */
          0x60040012,  //  0000  GETGBL	R1	G18
          0x7C040000,  //  0001  CALL	R1	0
          0x60080010,  //  0002  GETGBL	R2	G16
          0x8C0C0100,  //  0003  GETMET	R3	R0	K0
          0x7C0C0200,  //  0004  CALL	R3	1
          0x7C080200,  //  0005  CALL	R2	1
          0xA8020005,  //  0006  EXBLK	0	#000D
          0x5C0C0400,  //  0007  MOVE	R3	R2
          0x7C0C0000,  //  0008  CALL	R3	0
          0x8C100301,  //  0009  GETMET	R4	R1	K1
          0x5C180600,  //  000A  MOVE	R6	R3
          0x7C100400,  //  000B  CALL	R4	2
          0x7001FFF9,  //  000C  JMP		#0007
          0x58080002,  //  000D  LDCONST	R2	K2
          0xAC080200,  //  000E  CATCH	R2	1	0
          0xB0080000,  //  000F  RAISE	2	R0	R0
          0x60080010,  //  0010  GETGBL	R2	G16
          0x600C000C,  //  0011  GETGBL	R3	G12
          0x5C100200,  //  0012  MOVE	R4	R1
          0x7C0C0200,  //  0013  CALL	R3	1
          0x040C0703,  //  0014  SUB	R3	R3	K3
          0x400E0603,  //  0015  CONNECT	R3	K3	R3
          0x7C080200,  //  0016  CALL	R2	1
          0xA8020010,  //  0017  EXBLK	0	#0029
          0x5C0C0400,  //  0018  MOVE	R3	R2
          0x7C0C0000,  //  0019  CALL	R3	0
          0x94100203,  //  001A  GETIDX	R4	R1	R3
          0x5C140600,  //  001B  MOVE	R5	R3
          0x24180B04,  //  001C  GT	R6	R5	K4
          0x781A0008,  //  001D  JMPF	R6	#0027
          0x04180B03,  //  001E  SUB	R6	R5	K3
          0x94180206,  //  001F  GETIDX	R6	R1	R6
          0x24180C04,  //  0020  GT	R6	R6	R4
          0x781A0004,  //  0021  JMPF	R6	#0027
          0x04180B03,  //  0022  SUB	R6	R5	K3
          0x94180206,  //  0023  GETIDX	R6	R1	R6
          0x98040A06,  //  0024  SETIDX	R1	R5	R6
          0x04140B03,  //  0025  SUB	R5	R5	K3
          0x7001FFF4,  //  0026  JMP		#001C
          0x98040A04,  //  0027  SETIDX	R1	R5	R4
          0x7001FFEE,  //  0028  JMP		#0018
          0x58080002,  //  0029  LDCONST	R2	K2
          0xAC080200,  //  002A  CATCH	R2	1	0
          0xB0080000,  //  002B  RAISE	2	R0	R0
          0x80040200,  //  002C  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(format),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20process_attribute_expansion_X20_X25s),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20endpoint_X3D_X25s_X20cluster_X3D_X25s_X20attribute_X3D_X25s),
    /* K9   */  be_nested_str_weak(plugins),
    /* K10  */  be_nested_str_weak(get_endpoint),
    /* K11  */  be_nested_str_weak(contains),
    /* K12  */  be_nested_str_weak(get_cluster_list),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20pi_X3D_X25s_X20ep_X3D_X25s_X20cl_list_X3D_X25s),
    /* K14  */  be_nested_str_weak(get_attribute_list),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20pi_X3D_X25s_X20ep_X3D_X25s_X20cl_X3D_X25s_X20at_list_X3D_X25s),
    /* K16  */  be_nested_str_weak(push),
    /* K17  */  be_nested_str_weak(stop_iteration),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20expansion_X20_X5B_X2502X_X5D_X2504X_X2F_X2504X),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(status),
    /* K21  */  be_nested_str_weak(matter),
    /* K22  */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    /* K23  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    /* K24  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K25  */  be_nested_str_weak(UNREPORTABLE_ATTRIBUTE),
    }),
    be_str_weak(process_attribute_expansion),
    &be_const_str_solidified,
    ( &(const binstruction[271]) {  /* code */
      0x840C0000,  //  0000  CLOSURE	R3	P0
      0xA4120000,  //  0001  IMPORT	R4	K0
      0x88140301,  //  0002  GETMBR	R5	R1	K1
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x881C0302,  //  0004  GETMBR	R7	R1	K2
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x88240303,  //  0006  GETMBR	R9	R1	K3
      0x50280000,  //  0007  LDBOOL	R10	0	0
      0x882C0301,  //  0008  GETMBR	R11	R1	K1
      0x4C300000,  //  0009  LDNIL	R12
      0x202C160C,  //  000A  NE	R11	R11	R12
      0x782E0007,  //  000B  JMPF	R11	#0014
      0x882C0302,  //  000C  GETMBR	R11	R1	K2
      0x4C300000,  //  000D  LDNIL	R12
      0x202C160C,  //  000E  NE	R11	R11	R12
      0x782E0003,  //  000F  JMPF	R11	#0014
      0x882C0303,  //  0010  GETMBR	R11	R1	K3
      0x4C300000,  //  0011  LDNIL	R12
      0x202C160C,  //  0012  NE	R11	R11	R12
      0x742E0000,  //  0013  JMPT	R11	#0015
      0x502C0001,  //  0014  LDBOOL	R11	0	1
      0x502C0200,  //  0015  LDBOOL	R11	1	0
      0xB8320800,  //  0016  GETNGBL	R12	K4
      0x8C301905,  //  0017  GETMET	R12	R12	K5
      0x8C380906,  //  0018  GETMET	R14	R4	K6
      0x58400007,  //  0019  LDCONST	R16	K7
      0x60440008,  //  001A  GETGBL	R17	G8
      0x5C480200,  //  001B  MOVE	R18	R1
      0x7C440200,  //  001C  CALL	R17	1
      0x7C380600,  //  001D  CALL	R14	3
      0x543E0003,  //  001E  LDINT	R15	4
      0x7C300600,  //  001F  CALL	R12	3
      0x60300013,  //  0020  GETGBL	R12	G19
      0x7C300000,  //  0021  CALL	R12	0
      0xB8360800,  //  0022  GETNGBL	R13	K4
      0x8C341B05,  //  0023  GETMET	R13	R13	K5
      0x8C3C0906,  //  0024  GETMET	R15	R4	K6
      0x58440008,  //  0025  LDCONST	R17	K8
      0x5C480A00,  //  0026  MOVE	R18	R5
      0x5C4C0E00,  //  0027  MOVE	R19	R7
      0x5C501200,  //  0028  MOVE	R20	R9
      0x7C3C0A00,  //  0029  CALL	R15	5
      0x54420003,  //  002A  LDINT	R16	4
      0x7C340600,  //  002B  CALL	R13	3
      0x60340010,  //  002C  GETGBL	R13	G16
      0x88380109,  //  002D  GETMBR	R14	R0	K9
      0x7C340200,  //  002E  CALL	R13	1
      0xA8020078,  //  002F  EXBLK	0	#00A9
      0x5C381A00,  //  0030  MOVE	R14	R13
      0x7C380000,  //  0031  CALL	R14	0
      0x8C3C1D0A,  //  0032  GETMET	R15	R14	K10
      0x7C3C0200,  //  0033  CALL	R15	1
      0x4C400000,  //  0034  LDNIL	R16
      0x20400A10,  //  0035  NE	R16	R5	R16
      0x78420002,  //  0036  JMPF	R16	#003A
      0x20401E05,  //  0037  NE	R16	R15	R5
      0x78420000,  //  0038  JMPF	R16	#003A
      0x7001FFF5,  //  0039  JMP		#0030
      0x8C40190B,  //  003A  GETMET	R16	R12	K11
      0x5C481E00,  //  003B  MOVE	R18	R15
      0x7C400400,  //  003C  CALL	R16	2
      0x74420002,  //  003D  JMPT	R16	#0041
      0x60400013,  //  003E  GETGBL	R16	G19
      0x7C400000,  //  003F  CALL	R16	0
      0x98301E10,  //  0040  SETIDX	R12	R15	R16
      0x50180200,  //  0041  LDBOOL	R6	1	0
      0x8C401D0C,  //  0042  GETMET	R16	R14	K12
      0x5C481E00,  //  0043  MOVE	R18	R15
      0x7C400400,  //  0044  CALL	R16	2
      0xB8460800,  //  0045  GETNGBL	R17	K4
      0x8C442305,  //  0046  GETMET	R17	R17	K5
      0x8C4C0906,  //  0047  GETMET	R19	R4	K6
      0x5854000D,  //  0048  LDCONST	R21	K13
      0x60580008,  //  0049  GETGBL	R22	G8
      0x5C5C1C00,  //  004A  MOVE	R23	R14
      0x7C580200,  //  004B  CALL	R22	1
      0x605C0008,  //  004C  GETGBL	R23	G8
      0x5C601E00,  //  004D  MOVE	R24	R15
      0x7C5C0200,  //  004E  CALL	R23	1
      0x60600008,  //  004F  GETGBL	R24	G8
      0x5C642000,  //  0050  MOVE	R25	R16
      0x7C600200,  //  0051  CALL	R24	1
      0x7C4C0A00,  //  0052  CALL	R19	5
      0x54520003,  //  0053  LDINT	R20	4
      0x7C440600,  //  0054  CALL	R17	3
      0x60440010,  //  0055  GETGBL	R17	G16
      0x5C482000,  //  0056  MOVE	R18	R16
      0x7C440200,  //  0057  CALL	R17	1
      0xA802004B,  //  0058  EXBLK	0	#00A5
      0x5C482200,  //  0059  MOVE	R18	R17
      0x7C480000,  //  005A  CALL	R18	0
      0x4C4C0000,  //  005B  LDNIL	R19
      0x204C0E13,  //  005C  NE	R19	R7	R19
      0x784E0002,  //  005D  JMPF	R19	#0061
      0x204C2407,  //  005E  NE	R19	R18	R7
      0x784E0000,  //  005F  JMPF	R19	#0061
      0x7001FFF7,  //  0060  JMP		#0059
      0x944C180F,  //  0061  GETIDX	R19	R12	R15
      0x8C4C270B,  //  0062  GETMET	R19	R19	K11
      0x5C542400,  //  0063  MOVE	R21	R18
      0x7C4C0400,  //  0064  CALL	R19	2
      0x744E0003,  //  0065  JMPT	R19	#006A
      0x944C180F,  //  0066  GETIDX	R19	R12	R15
      0x60500013,  //  0067  GETGBL	R20	G19
      0x7C500000,  //  0068  CALL	R20	0
      0x984C2414,  //  0069  SETIDX	R19	R18	R20
      0x50200200,  //  006A  LDBOOL	R8	1	0
      0x8C4C1D0E,  //  006B  GETMET	R19	R14	K14
      0x5C541E00,  //  006C  MOVE	R21	R15
      0x5C582400,  //  006D  MOVE	R22	R18
      0x7C4C0600,  //  006E  CALL	R19	3
      0xB8520800,  //  006F  GETNGBL	R20	K4
      0x8C502905,  //  0070  GETMET	R20	R20	K5
      0x8C580906,  //  0071  GETMET	R22	R4	K6
      0x5860000F,  //  0072  LDCONST	R24	K15
      0x60640008,  //  0073  GETGBL	R25	G8
      0x5C681C00,  //  0074  MOVE	R26	R14
      0x7C640200,  //  0075  CALL	R25	1
      0x60680008,  //  0076  GETGBL	R26	G8
      0x5C6C1E00,  //  0077  MOVE	R27	R15
      0x7C680200,  //  0078  CALL	R26	1
      0x606C0008,  //  0079  GETGBL	R27	G8
      0x5C702400,  //  007A  MOVE	R28	R18
      0x7C6C0200,  //  007B  CALL	R27	1
      0x60700008,  //  007C  GETGBL	R28	G8
      0x5C742600,  //  007D  MOVE	R29	R19
      0x7C700200,  //  007E  CALL	R28	1
      0x7C580C00,  //  007F  CALL	R22	6
      0x545E0003,  //  0080  LDINT	R23	4
      0x7C500600,  //  0081  CALL	R20	3
      0x60500010,  //  0082  GETGBL	R20	G16
      0x5C542600,  //  0083  MOVE	R21	R19
      0x7C500200,  //  0084  CALL	R20	1
      0xA802001A,  //  0085  EXBLK	0	#00A1
      0x5C542800,  //  0086  MOVE	R21	R20
      0x7C540000,  //  0087  CALL	R21	0
      0x4C580000,  //  0088  LDNIL	R22
      0x20581216,  //  0089  NE	R22	R9	R22
      0x785A0002,  //  008A  JMPF	R22	#008E
      0x20582A09,  //  008B  NE	R22	R21	R9
      0x785A0000,  //  008C  JMPF	R22	#008E
      0x7001FFF7,  //  008D  JMP		#0086
      0x9458180F,  //  008E  GETIDX	R22	R12	R15
      0x94582C12,  //  008F  GETIDX	R22	R22	R18
      0x8C582D0B,  //  0090  GETMET	R22	R22	K11
      0x5C602A00,  //  0091  MOVE	R24	R21
      0x7C580400,  //  0092  CALL	R22	2
      0x745A0004,  //  0093  JMPT	R22	#0099
      0x9458180F,  //  0094  GETIDX	R22	R12	R15
      0x94582C12,  //  0095  GETIDX	R22	R22	R18
      0x605C0012,  //  0096  GETGBL	R23	G18
      0x7C5C0000,  //  0097  CALL	R23	0
      0x98582A17,  //  0098  SETIDX	R22	R21	R23
      0x50280200,  //  0099  LDBOOL	R10	1	0
      0x9458180F,  //  009A  GETIDX	R22	R12	R15
      0x94582C12,  //  009B  GETIDX	R22	R22	R18
      0x94582C15,  //  009C  GETIDX	R22	R22	R21
      0x8C582D10,  //  009D  GETMET	R22	R22	K16
      0x5C601C00,  //  009E  MOVE	R24	R14
      0x7C580400,  //  009F  CALL	R22	2
      0x7001FFE4,  //  00A0  JMP		#0086
      0x58500011,  //  00A1  LDCONST	R20	K17
      0xAC500200,  //  00A2  CATCH	R20	1	0
      0xB0080000,  //  00A3  RAISE	2	R0	R0
      0x7001FFB3,  //  00A4  JMP		#0059
      0x58440011,  //  00A5  LDCONST	R17	K17
      0xAC440200,  //  00A6  CATCH	R17	1	0
      0xB0080000,  //  00A7  RAISE	2	R0	R0
      0x7001FF86,  //  00A8  JMP		#0030
      0x58340011,  //  00A9  LDCONST	R13	K17
      0xAC340200,  //  00AA  CATCH	R13	1	0
      0xB0080000,  //  00AB  RAISE	2	R0	R0
      0x60340010,  //  00AC  GETGBL	R13	G16
      0x5C380600,  //  00AD  MOVE	R14	R3
      0x5C3C1800,  //  00AE  MOVE	R15	R12
      0x7C380200,  //  00AF  CALL	R14	1
      0x7C340200,  //  00B0  CALL	R13	1
      0xA802003D,  //  00B1  EXBLK	0	#00F0
      0x5C381A00,  //  00B2  MOVE	R14	R13
      0x7C380000,  //  00B3  CALL	R14	0
      0x603C0010,  //  00B4  GETGBL	R15	G16
      0x5C400600,  //  00B5  MOVE	R16	R3
      0x9444180E,  //  00B6  GETIDX	R17	R12	R14
      0x7C400200,  //  00B7  CALL	R16	1
      0x7C3C0200,  //  00B8  CALL	R15	1
      0xA8020031,  //  00B9  EXBLK	0	#00EC
      0x5C401E00,  //  00BA  MOVE	R16	R15
      0x7C400000,  //  00BB  CALL	R16	0
      0x60440010,  //  00BC  GETGBL	R17	G16
      0x5C480600,  //  00BD  MOVE	R18	R3
      0x944C180E,  //  00BE  GETIDX	R19	R12	R14
      0x944C2610,  //  00BF  GETIDX	R19	R19	R16
      0x7C480200,  //  00C0  CALL	R18	1
      0x7C440200,  //  00C1  CALL	R17	1
      0xA8020024,  //  00C2  EXBLK	0	#00E8
      0x5C482200,  //  00C3  MOVE	R18	R17
      0x7C480000,  //  00C4  CALL	R18	0
      0x604C0010,  //  00C5  GETGBL	R19	G16
      0x9450180E,  //  00C6  GETIDX	R20	R12	R14
      0x94502810,  //  00C7  GETIDX	R20	R20	R16
      0x94502812,  //  00C8  GETIDX	R20	R20	R18
      0x7C4C0200,  //  00C9  CALL	R19	1
      0xA8020018,  //  00CA  EXBLK	0	#00E4
      0x5C502600,  //  00CB  MOVE	R20	R19
      0x7C500000,  //  00CC  CALL	R20	0
      0xB8560800,  //  00CD  GETNGBL	R21	K4
      0x8C542B05,  //  00CE  GETMET	R21	R21	K5
      0x8C5C0906,  //  00CF  GETMET	R23	R4	K6
      0x58640012,  //  00D0  LDCONST	R25	K18
      0x5C681C00,  //  00D1  MOVE	R26	R14
      0x5C6C2000,  //  00D2  MOVE	R27	R16
      0x5C702400,  //  00D3  MOVE	R28	R18
      0x7C5C0A00,  //  00D4  CALL	R23	5
      0x58600013,  //  00D5  LDCONST	R24	K19
      0x7C540600,  //  00D6  CALL	R21	3
      0x9006020E,  //  00D7  SETMBR	R1	K1	R14
      0x90060410,  //  00D8  SETMBR	R1	K2	R16
      0x90060612,  //  00D9  SETMBR	R1	K3	R18
      0x5C540400,  //  00DA  MOVE	R21	R2
      0x5C582800,  //  00DB  MOVE	R22	R20
      0x5C5C0200,  //  00DC  MOVE	R23	R1
      0x5C601600,  //  00DD  MOVE	R24	R11
      0x7C540600,  //  00DE  CALL	R21	3
      0x782E0002,  //  00DF  JMPF	R11	#00E3
      0x78560001,  //  00E0  JMPF	R21	#00E3
      0xA8040004,  //  00E1  EXBLK	1	4
      0x80002C00,  //  00E2  RET	0
      0x7001FFE6,  //  00E3  JMP		#00CB
      0x584C0011,  //  00E4  LDCONST	R19	K17
      0xAC4C0200,  //  00E5  CATCH	R19	1	0
      0xB0080000,  //  00E6  RAISE	2	R0	R0
      0x7001FFDA,  //  00E7  JMP		#00C3
      0x58440011,  //  00E8  LDCONST	R17	K17
      0xAC440200,  //  00E9  CATCH	R17	1	0
      0xB0080000,  //  00EA  RAISE	2	R0	R0
      0x7001FFCD,  //  00EB  JMP		#00BA
      0x583C0011,  //  00EC  LDCONST	R15	K17
      0xAC3C0200,  //  00ED  CATCH	R15	1	0
      0xB0080000,  //  00EE  RAISE	2	R0	R0
      0x7001FFC1,  //  00EF  JMP		#00B2
      0x58340011,  //  00F0  LDCONST	R13	K17
      0xAC340200,  //  00F1  CATCH	R13	1	0
      0xB0080000,  //  00F2  RAISE	2	R0	R0
      0x782E0019,  //  00F3  JMPF	R11	#010E
      0x5C340C00,  //  00F4  MOVE	R13	R6
      0x74360003,  //  00F5  JMPT	R13	#00FA
      0xB8362A00,  //  00F6  GETNGBL	R13	K21
      0x88341B16,  //  00F7  GETMBR	R13	R13	K22
      0x9006280D,  //  00F8  SETMBR	R1	K20	R13
      0x7002000E,  //  00F9  JMP		#0109
      0x5C341000,  //  00FA  MOVE	R13	R8
      0x74360003,  //  00FB  JMPT	R13	#0100
      0xB8362A00,  //  00FC  GETNGBL	R13	K21
      0x88341B17,  //  00FD  GETMBR	R13	R13	K23
      0x9006280D,  //  00FE  SETMBR	R1	K20	R13
      0x70020008,  //  00FF  JMP		#0109
      0x5C341400,  //  0100  MOVE	R13	R10
      0x74360003,  //  0101  JMPT	R13	#0106
      0xB8362A00,  //  0102  GETNGBL	R13	K21
      0x88341B18,  //  0103  GETMBR	R13	R13	K24
      0x9006280D,  //  0104  SETMBR	R1	K20	R13
      0x70020002,  //  0105  JMP		#0109
      0xB8362A00,  //  0106  GETNGBL	R13	K21
      0x88341B19,  //  0107  GETMBR	R13	R13	K25
      0x9006280D,  //  0108  SETMBR	R1	K20	R13
      0x5C340400,  //  0109  MOVE	R13	R2
      0x4C380000,  //  010A  LDNIL	R14
      0x5C3C0200,  //  010B  MOVE	R15	R1
      0x50400200,  //  010C  LDBOOL	R16	1	0
      0x7C340600,  //  010D  CALL	R13	3
      0x80000000,  //  010E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_param
********************************************************************/
be_local_closure(Matter_Device_save_param,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(dump),
    /* K2   */  be_nested_str_weak(distinguish),
    /* K3   */  be_nested_str_weak(root_discriminator),
    /* K4   */  be_nested_str_weak(passcode),
    /* K5   */  be_nested_str_weak(root_passcode),
    /* K6   */  be_nested_str_weak(ipv4only),
    /* K7   */  be_nested_str_weak(string),
    /* K8   */  be_nested_str_weak(FILENAME),
    /* K9   */  be_nested_str_weak(w),
    /* K10  */  be_nested_str_weak(write),
    /* K11  */  be_nested_str_weak(close),
    /* K12  */  be_nested_str_weak(tasmota),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K15  */  be_nested_str_weak(_X7C),
    /* K16  */  be_const_int(2),
    }),
    be_str_weak(save_param),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C100000,  //  0003  CALL	R4	0
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x98120405,  //  0005  SETIDX	R4	K2	R5
      0x88140105,  //  0006  GETMBR	R5	R0	K5
      0x98120805,  //  0007  SETIDX	R4	K4	R5
      0x88140106,  //  0008  GETMBR	R5	R0	K6
      0x98120C05,  //  0009  SETIDX	R4	K6	R5
      0x7C080400,  //  000A  CALL	R2	2
      0xA802000D,  //  000B  EXBLK	0	#001A
      0xA40E0E00,  //  000C  IMPORT	R3	K7
      0x60100011,  //  000D  GETGBL	R4	G17
      0x88140108,  //  000E  GETMBR	R5	R0	K8
      0x58180009,  //  000F  LDCONST	R6	K9
      0x7C100400,  //  0010  CALL	R4	2
      0x8C14090A,  //  0011  GETMET	R5	R4	K10
      0x5C1C0400,  //  0012  MOVE	R7	R2
      0x7C140400,  //  0013  CALL	R5	2
      0x8C14090B,  //  0014  GETMET	R5	R4	K11
      0x7C140200,  //  0015  CALL	R5	1
      0xA8040001,  //  0016  EXBLK	1	1
      0x80040400,  //  0017  RET	1	R2
      0xA8040001,  //  0018  EXBLK	1	1
      0x70020011,  //  0019  JMP		#002C
      0xAC0C0002,  //  001A  CATCH	R3	0	2
      0x7002000E,  //  001B  JMP		#002B
      0xB8161800,  //  001C  GETNGBL	R5	K12
      0x8C140B0D,  //  001D  GETMET	R5	R5	K13
      0x601C0008,  //  001E  GETGBL	R7	G8
      0x5C200600,  //  001F  MOVE	R8	R3
      0x7C1C0200,  //  0020  CALL	R7	1
      0x001E1C07,  //  0021  ADD	R7	K14	R7
      0x001C0F0F,  //  0022  ADD	R7	R7	K15
      0x60200008,  //  0023  GETGBL	R8	G8
      0x5C240800,  //  0024  MOVE	R9	R4
      0x7C200200,  //  0025  CALL	R8	1
      0x001C0E08,  //  0026  ADD	R7	R7	R8
      0x58200010,  //  0027  LDCONST	R8	K16
      0x7C140600,  //  0028  CALL	R5	3
      0x80040400,  //  0029  RET	1	R2
      0x70020000,  //  002A  JMP		#002C
      0xB0080000,  //  002B  RAISE	2	R0	R0
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_PASE
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_PASE,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[44]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(crypto),
    /* K3   */  be_nested_str_weak(VP),
    /* K4   */  be_nested_str_weak(vendorid),
    /* K5   */  be_nested_str_weak(_X2B),
    /* K6   */  be_nested_str_weak(productid),
    /* K7   */  be_nested_str_weak(D),
    /* K8   */  be_nested_str_weak(commissioning_discriminator),
    /* K9   */  be_nested_str_weak(CM),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(T),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str_weak(SII),
    /* K14  */  be_nested_str_weak(SAI),
    /* K15  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K16  */  be_nested_str_weak(random),
    /* K17  */  be_nested_str_weak(tohex),
    /* K18  */  be_nested_str_weak(commissioning_instance_eth),
    /* K19  */  be_nested_str_weak(hostname_eth),
    /* K20  */  be_nested_str_weak(tasmota),
    /* K21  */  be_nested_str_weak(log),
    /* K22  */  be_nested_str_weak(format),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eadd_service_X28_X25s_X2C_X20_X25s_X2C_X20_X25i_X2C_X20_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
    /* K24  */  be_nested_str_weak(_matterc),
    /* K25  */  be_nested_str_weak(_udp),
    /* K26  */  be_const_int(3),
    /* K27  */  be_nested_str_weak(add_service),
    /* K28  */  be_nested_str_weak(mdns_pase_eth),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20announce_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K30  */  be_nested_str_weak(eth),
    /* K31  */  be_const_int(2),
    /* K32  */  be_nested_str_weak(_L),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K34  */  be_nested_str_weak(add_subtype),
    /* K35  */  be_nested_str_weak(_S),
    /* K36  */  be_nested_str_weak(_V),
    /* K37  */  be_nested_str_weak(_CM1),
    /* K38  */  be_nested_str_weak(hostname_wifi),
    /* K39  */  be_nested_str_weak(mdns_pase_wifi),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20starting_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K41  */  be_nested_str_weak(wifi),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K43  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_announce_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[267]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x60100013,  //  0003  GETGBL	R4	G19
      0x7C100000,  //  0004  CALL	R4	0
      0x60140008,  //  0005  GETGBL	R5	G8
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x7C140200,  //  0007  CALL	R5	1
      0x00140B05,  //  0008  ADD	R5	R5	K5
      0x60180008,  //  0009  GETGBL	R6	G8
      0x881C0106,  //  000A  GETMBR	R7	R0	K6
      0x7C180200,  //  000B  CALL	R6	1
      0x00140A06,  //  000C  ADD	R5	R5	R6
      0x98120605,  //  000D  SETIDX	R4	K3	R5
      0x88140108,  //  000E  GETMBR	R5	R0	K8
      0x98120E05,  //  000F  SETIDX	R4	K7	R5
      0x9812130A,  //  0010  SETIDX	R4	K9	K10
      0x9812170C,  //  0011  SETIDX	R4	K11	K12
      0x54161387,  //  0012  LDINT	R5	5000
      0x98121A05,  //  0013  SETIDX	R4	K13	R5
      0x5416012B,  //  0014  LDINT	R5	300
      0x98121C05,  //  0015  SETIDX	R4	K14	R5
      0x8C140710,  //  0016  GETMET	R5	R3	K16
      0x541E0007,  //  0017  LDINT	R7	8
      0x7C140400,  //  0018  CALL	R5	2
      0x8C140B11,  //  0019  GETMET	R5	R5	K17
      0x7C140200,  //  001A  CALL	R5	1
      0x90021E05,  //  001B  SETMBR	R0	K15	R5
      0x8C140710,  //  001C  GETMET	R5	R3	K16
      0x541E0007,  //  001D  LDINT	R7	8
      0x7C140400,  //  001E  CALL	R5	2
      0x8C140B11,  //  001F  GETMET	R5	R5	K17
      0x7C140200,  //  0020  CALL	R5	1
      0x90022405,  //  0021  SETMBR	R0	K18	R5
      0xA80200D5,  //  0022  EXBLK	0	#00F9
      0x88140113,  //  0023  GETMBR	R5	R0	K19
      0x78160067,  //  0024  JMPF	R5	#008D
      0xB8162800,  //  0025  GETNGBL	R5	K20
      0x8C140B15,  //  0026  GETMET	R5	R5	K21
      0x8C1C0516,  //  0027  GETMET	R7	R2	K22
      0x58240017,  //  0028  LDCONST	R9	K23
      0x58280018,  //  0029  LDCONST	R10	K24
      0x582C0019,  //  002A  LDCONST	R11	K25
      0x543215A3,  //  002B  LDINT	R12	5540
      0x60340008,  //  002C  GETGBL	R13	G8
      0x5C380800,  //  002D  MOVE	R14	R4
      0x7C340200,  //  002E  CALL	R13	1
      0x88380112,  //  002F  GETMBR	R14	R0	K18
      0x883C0113,  //  0030  GETMBR	R15	R0	K19
      0x7C1C1000,  //  0031  CALL	R7	8
      0x5820001A,  //  0032  LDCONST	R8	K26
      0x7C140600,  //  0033  CALL	R5	3
      0x8C14031B,  //  0034  GETMET	R5	R1	K27
      0x581C0018,  //  0035  LDCONST	R7	K24
      0x58200019,  //  0036  LDCONST	R8	K25
      0x542615A3,  //  0037  LDINT	R9	5540
      0x5C280800,  //  0038  MOVE	R10	R4
      0x882C0112,  //  0039  GETMBR	R11	R0	K18
      0x88300113,  //  003A  GETMBR	R12	R0	K19
      0x7C140E00,  //  003B  CALL	R5	7
      0x50140200,  //  003C  LDBOOL	R5	1	0
      0x90023805,  //  003D  SETMBR	R0	K28	R5
      0xB8162800,  //  003E  GETNGBL	R5	K20
      0x8C140B15,  //  003F  GETMET	R5	R5	K21
      0x8C1C0516,  //  0040  GETMET	R7	R2	K22
      0x5824001D,  //  0041  LDCONST	R9	K29
      0x5828001E,  //  0042  LDCONST	R10	K30
      0x882C0112,  //  0043  GETMBR	R11	R0	K18
      0x88300113,  //  0044  GETMBR	R12	R0	K19
      0x7C1C0A00,  //  0045  CALL	R7	5
      0x5820001F,  //  0046  LDCONST	R8	K31
      0x7C140600,  //  0047  CALL	R5	3
      0x60140008,  //  0048  GETGBL	R5	G8
      0x88180108,  //  0049  GETMBR	R6	R0	K8
      0x541E0FFE,  //  004A  LDINT	R7	4095
      0x2C180C07,  //  004B  AND	R6	R6	R7
      0x7C140200,  //  004C  CALL	R5	1
      0x00164005,  //  004D  ADD	R5	K32	R5
      0xB81A2800,  //  004E  GETNGBL	R6	K20
      0x8C180D15,  //  004F  GETMET	R6	R6	K21
      0x00224205,  //  0050  ADD	R8	K33	R5
      0x5824001F,  //  0051  LDCONST	R9	K31
      0x7C180600,  //  0052  CALL	R6	3
      0x8C180322,  //  0053  GETMET	R6	R1	K34
      0x58200018,  //  0054  LDCONST	R8	K24
      0x58240019,  //  0055  LDCONST	R9	K25
      0x88280112,  //  0056  GETMBR	R10	R0	K18
      0x882C0113,  //  0057  GETMBR	R11	R0	K19
      0x5C300A00,  //  0058  MOVE	R12	R5
      0x7C180C00,  //  0059  CALL	R6	6
      0x60180008,  //  005A  GETGBL	R6	G8
      0x881C0108,  //  005B  GETMBR	R7	R0	K8
      0x54220EFF,  //  005C  LDINT	R8	3840
      0x2C1C0E08,  //  005D  AND	R7	R7	R8
      0x54220007,  //  005E  LDINT	R8	8
      0x3C1C0E08,  //  005F  SHR	R7	R7	R8
      0x7C180200,  //  0060  CALL	R6	1
      0x001A4606,  //  0061  ADD	R6	K35	R6
      0x5C140C00,  //  0062  MOVE	R5	R6
      0xB81A2800,  //  0063  GETNGBL	R6	K20
      0x8C180D15,  //  0064  GETMET	R6	R6	K21
      0x00224205,  //  0065  ADD	R8	K33	R5
      0x5824001F,  //  0066  LDCONST	R9	K31
      0x7C180600,  //  0067  CALL	R6	3
      0x8C180322,  //  0068  GETMET	R6	R1	K34
      0x58200018,  //  0069  LDCONST	R8	K24
      0x58240019,  //  006A  LDCONST	R9	K25
      0x88280112,  //  006B  GETMBR	R10	R0	K18
      0x882C0113,  //  006C  GETMBR	R11	R0	K19
      0x5C300A00,  //  006D  MOVE	R12	R5
      0x7C180C00,  //  006E  CALL	R6	6
      0x60180008,  //  006F  GETGBL	R6	G8
      0x881C0104,  //  0070  GETMBR	R7	R0	K4
      0x7C180200,  //  0071  CALL	R6	1
      0x001A4806,  //  0072  ADD	R6	K36	R6
      0x5C140C00,  //  0073  MOVE	R5	R6
      0xB81A2800,  //  0074  GETNGBL	R6	K20
      0x8C180D15,  //  0075  GETMET	R6	R6	K21
      0x00224205,  //  0076  ADD	R8	K33	R5
      0x5824001F,  //  0077  LDCONST	R9	K31
      0x7C180600,  //  0078  CALL	R6	3
      0x8C180322,  //  0079  GETMET	R6	R1	K34
      0x58200018,  //  007A  LDCONST	R8	K24
      0x58240019,  //  007B  LDCONST	R9	K25
      0x88280112,  //  007C  GETMBR	R10	R0	K18
      0x882C0113,  //  007D  GETMBR	R11	R0	K19
      0x5C300A00,  //  007E  MOVE	R12	R5
      0x7C180C00,  //  007F  CALL	R6	6
      0x58140025,  //  0080  LDCONST	R5	K37
      0xB81A2800,  //  0081  GETNGBL	R6	K20
      0x8C180D15,  //  0082  GETMET	R6	R6	K21
      0x00224205,  //  0083  ADD	R8	K33	R5
      0x5824001F,  //  0084  LDCONST	R9	K31
      0x7C180600,  //  0085  CALL	R6	3
      0x8C180322,  //  0086  GETMET	R6	R1	K34
      0x58200018,  //  0087  LDCONST	R8	K24
      0x58240019,  //  0088  LDCONST	R9	K25
      0x88280112,  //  0089  GETMBR	R10	R0	K18
      0x882C0113,  //  008A  GETMBR	R11	R0	K19
      0x5C300A00,  //  008B  MOVE	R12	R5
      0x7C180C00,  //  008C  CALL	R6	6
      0x88140126,  //  008D  GETMBR	R5	R0	K38
      0x78160067,  //  008E  JMPF	R5	#00F7
      0xB8162800,  //  008F  GETNGBL	R5	K20
      0x8C140B15,  //  0090  GETMET	R5	R5	K21
      0x8C1C0516,  //  0091  GETMET	R7	R2	K22
      0x58240017,  //  0092  LDCONST	R9	K23
      0x58280018,  //  0093  LDCONST	R10	K24
      0x582C0019,  //  0094  LDCONST	R11	K25
      0x543215A3,  //  0095  LDINT	R12	5540
      0x60340008,  //  0096  GETGBL	R13	G8
      0x5C380800,  //  0097  MOVE	R14	R4
      0x7C340200,  //  0098  CALL	R13	1
      0x8838010F,  //  0099  GETMBR	R14	R0	K15
      0x883C0126,  //  009A  GETMBR	R15	R0	K38
      0x7C1C1000,  //  009B  CALL	R7	8
      0x5820001A,  //  009C  LDCONST	R8	K26
      0x7C140600,  //  009D  CALL	R5	3
      0x8C14031B,  //  009E  GETMET	R5	R1	K27
      0x581C0018,  //  009F  LDCONST	R7	K24
      0x58200019,  //  00A0  LDCONST	R8	K25
      0x542615A3,  //  00A1  LDINT	R9	5540
      0x5C280800,  //  00A2  MOVE	R10	R4
      0x882C010F,  //  00A3  GETMBR	R11	R0	K15
      0x88300126,  //  00A4  GETMBR	R12	R0	K38
      0x7C140E00,  //  00A5  CALL	R5	7
      0x50140200,  //  00A6  LDBOOL	R5	1	0
      0x90024E05,  //  00A7  SETMBR	R0	K39	R5
      0xB8162800,  //  00A8  GETNGBL	R5	K20
      0x8C140B15,  //  00A9  GETMET	R5	R5	K21
      0x8C1C0516,  //  00AA  GETMET	R7	R2	K22
      0x58240028,  //  00AB  LDCONST	R9	K40
      0x58280029,  //  00AC  LDCONST	R10	K41
      0x882C010F,  //  00AD  GETMBR	R11	R0	K15
      0x88300126,  //  00AE  GETMBR	R12	R0	K38
      0x7C1C0A00,  //  00AF  CALL	R7	5
      0x5820001F,  //  00B0  LDCONST	R8	K31
      0x7C140600,  //  00B1  CALL	R5	3
      0x60140008,  //  00B2  GETGBL	R5	G8
      0x88180108,  //  00B3  GETMBR	R6	R0	K8
      0x541E0FFE,  //  00B4  LDINT	R7	4095
      0x2C180C07,  //  00B5  AND	R6	R6	R7
      0x7C140200,  //  00B6  CALL	R5	1
      0x00164005,  //  00B7  ADD	R5	K32	R5
      0xB81A2800,  //  00B8  GETNGBL	R6	K20
      0x8C180D15,  //  00B9  GETMET	R6	R6	K21
      0x00224205,  //  00BA  ADD	R8	K33	R5
      0x5824001F,  //  00BB  LDCONST	R9	K31
      0x7C180600,  //  00BC  CALL	R6	3
      0x8C180322,  //  00BD  GETMET	R6	R1	K34
      0x58200018,  //  00BE  LDCONST	R8	K24
      0x58240019,  //  00BF  LDCONST	R9	K25
      0x8828010F,  //  00C0  GETMBR	R10	R0	K15
      0x882C0126,  //  00C1  GETMBR	R11	R0	K38
      0x5C300A00,  //  00C2  MOVE	R12	R5
      0x7C180C00,  //  00C3  CALL	R6	6
      0x60180008,  //  00C4  GETGBL	R6	G8
      0x881C0108,  //  00C5  GETMBR	R7	R0	K8
      0x54220EFF,  //  00C6  LDINT	R8	3840
      0x2C1C0E08,  //  00C7  AND	R7	R7	R8
      0x54220007,  //  00C8  LDINT	R8	8
      0x3C1C0E08,  //  00C9  SHR	R7	R7	R8
      0x7C180200,  //  00CA  CALL	R6	1
      0x001A4606,  //  00CB  ADD	R6	K35	R6
      0x5C140C00,  //  00CC  MOVE	R5	R6
      0xB81A2800,  //  00CD  GETNGBL	R6	K20
      0x8C180D15,  //  00CE  GETMET	R6	R6	K21
      0x00224205,  //  00CF  ADD	R8	K33	R5
      0x5824001F,  //  00D0  LDCONST	R9	K31
      0x7C180600,  //  00D1  CALL	R6	3
      0x8C180322,  //  00D2  GETMET	R6	R1	K34
      0x58200018,  //  00D3  LDCONST	R8	K24
      0x58240019,  //  00D4  LDCONST	R9	K25
      0x8828010F,  //  00D5  GETMBR	R10	R0	K15
      0x882C0126,  //  00D6  GETMBR	R11	R0	K38
      0x5C300A00,  //  00D7  MOVE	R12	R5
      0x7C180C00,  //  00D8  CALL	R6	6
      0x60180008,  //  00D9  GETGBL	R6	G8
      0x881C0104,  //  00DA  GETMBR	R7	R0	K4
      0x7C180200,  //  00DB  CALL	R6	1
      0x001A4806,  //  00DC  ADD	R6	K36	R6
      0x5C140C00,  //  00DD  MOVE	R5	R6
      0xB81A2800,  //  00DE  GETNGBL	R6	K20
      0x8C180D15,  //  00DF  GETMET	R6	R6	K21
      0x00224205,  //  00E0  ADD	R8	K33	R5
      0x5824001F,  //  00E1  LDCONST	R9	K31
      0x7C180600,  //  00E2  CALL	R6	3
      0x8C180322,  //  00E3  GETMET	R6	R1	K34
      0x58200018,  //  00E4  LDCONST	R8	K24
      0x58240019,  //  00E5  LDCONST	R9	K25
      0x8828010F,  //  00E6  GETMBR	R10	R0	K15
      0x882C0126,  //  00E7  GETMBR	R11	R0	K38
      0x5C300A00,  //  00E8  MOVE	R12	R5
      0x7C180C00,  //  00E9  CALL	R6	6
      0x58140025,  //  00EA  LDCONST	R5	K37
      0xB81A2800,  //  00EB  GETNGBL	R6	K20
      0x8C180D15,  //  00EC  GETMET	R6	R6	K21
      0x00224205,  //  00ED  ADD	R8	K33	R5
      0x5824001F,  //  00EE  LDCONST	R9	K31
      0x7C180600,  //  00EF  CALL	R6	3
      0x8C180322,  //  00F0  GETMET	R6	R1	K34
      0x58200018,  //  00F1  LDCONST	R8	K24
      0x58240019,  //  00F2  LDCONST	R9	K25
      0x8828010F,  //  00F3  GETMBR	R10	R0	K15
      0x882C0126,  //  00F4  GETMBR	R11	R0	K38
      0x5C300A00,  //  00F5  MOVE	R12	R5
      0x7C180C00,  //  00F6  CALL	R6	6
      0xA8040001,  //  00F7  EXBLK	1	1
      0x70020010,  //  00F8  JMP		#010A
      0xAC140002,  //  00F9  CATCH	R5	0	2
      0x7002000D,  //  00FA  JMP		#0109
      0xB81E2800,  //  00FB  GETNGBL	R7	K20
      0x8C1C0F15,  //  00FC  GETMET	R7	R7	K21
      0x60240008,  //  00FD  GETGBL	R9	G8
      0x5C280A00,  //  00FE  MOVE	R10	R5
      0x7C240200,  //  00FF  CALL	R9	1
      0x00265409,  //  0100  ADD	R9	K42	R9
      0x0024132B,  //  0101  ADD	R9	R9	K43
      0x60280008,  //  0102  GETGBL	R10	G8
      0x5C2C0C00,  //  0103  MOVE	R11	R6
      0x7C280200,  //  0104  CALL	R10	1
      0x0024120A,  //  0105  ADD	R9	R9	R10
      0x5828001F,  //  0106  LDCONST	R10	K31
      0x7C1C0600,  //  0107  CALL	R7	3
      0x70020000,  //  0108  JMP		#010A
      0xB0080000,  //  0109  RAISE	2	R0	R0
      0x80000000,  //  010A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_start_basic_commissioning,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    8,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str_weak(mdns_announce_PASE),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0000,  //  0006  LDCONST	R3	K0
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str_weak(mdns_announce_PASE),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0000,  //  0006  LDCONST	R3	K0
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(commissioning_open),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(commissioning_iterations),
    /* K4   */  be_nested_str_weak(commissioning_discriminator),
    /* K5   */  be_nested_str_weak(commissioning_salt),
    /* K6   */  be_nested_str_weak(commissioning_w0),
    /* K7   */  be_nested_str_weak(commissioning_L),
    /* K8   */  be_nested_str_weak(commissioning_admin_fabric),
    /* K9   */  be_nested_str_weak(wifi),
    /* K10  */  be_nested_str_weak(up),
    /* K11  */  be_nested_str_weak(eth),
    /* K12  */  be_nested_str_weak(mdns_announce_PASE),
    /* K13  */  be_nested_str_weak(add_rule),
    /* K14  */  be_nested_str_weak(Wifi_X23Connected),
    /* K15  */  be_nested_str_weak(Eth_X23Connected),
    }),
    be_str_weak(start_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0xB8220200,  //  0000  GETNGBL	R8	K1
      0x8C201102,  //  0001  GETMET	R8	R8	K2
      0x7C200200,  //  0002  CALL	R8	1
      0x542603E7,  //  0003  LDINT	R9	1000
      0x08240209,  //  0004  MUL	R9	R1	R9
      0x00201009,  //  0005  ADD	R8	R8	R9
      0x90020008,  //  0006  SETMBR	R0	K0	R8
      0x90020602,  //  0007  SETMBR	R0	K3	R2
      0x90020803,  //  0008  SETMBR	R0	K4	R3
      0x90020A04,  //  0009  SETMBR	R0	K5	R4
      0x90020C05,  //  000A  SETMBR	R0	K6	R5
      0x90020E06,  //  000B  SETMBR	R0	K7	R6
      0x90021007,  //  000C  SETMBR	R0	K8	R7
      0xB8220200,  //  000D  GETNGBL	R8	K1
      0x8C201109,  //  000E  GETMET	R8	R8	K9
      0x7C200200,  //  000F  CALL	R8	1
      0x9420110A,  //  0010  GETIDX	R8	R8	K10
      0x74220004,  //  0011  JMPT	R8	#0017
      0xB8220200,  //  0012  GETNGBL	R8	K1
      0x8C20110B,  //  0013  GETMET	R8	R8	K11
      0x7C200200,  //  0014  CALL	R8	1
      0x9420110A,  //  0015  GETIDX	R8	R8	K10
      0x78220002,  //  0016  JMPF	R8	#001A
      0x8C20010C,  //  0017  GETMET	R8	R0	K12
      0x7C200200,  //  0018  CALL	R8	1
      0x7002000B,  //  0019  JMP		#0026
      0xB8220200,  //  001A  GETNGBL	R8	K1
      0x8C20110D,  //  001B  GETMET	R8	R8	K13
      0x5828000E,  //  001C  LDCONST	R10	K14
      0x842C0000,  //  001D  CLOSURE	R11	P0
      0x5830000C,  //  001E  LDCONST	R12	K12
      0x7C200800,  //  001F  CALL	R8	4
      0xB8220200,  //  0020  GETNGBL	R8	K1
      0x8C20110D,  //  0021  GETMET	R8	R8	K13
      0x5828000F,  //  0022  LDCONST	R10	K15
      0x842C0001,  //  0023  CLOSURE	R11	P1
      0x5830000C,  //  0024  LDCONST	R12	K12
      0x7C200800,  //  0025  CALL	R8	4
      0xA0000000,  //  0026  CLOSE	R0
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Device_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(start),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        /* K4   */  be_nested_str_weak(matter_start),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0004,  //  0006  LDCONST	R3	K4
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(start),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        /* K4   */  be_nested_str_weak(matter_start),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0004,  //  0006  LDCONST	R3	K4
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[35]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(get_option),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(MATTER_OPTION),
    /* K6   */  be_nested_str_weak(UI),
    /* K7   */  be_nested_str_weak(started),
    /* K8   */  be_nested_str_weak(plugins),
    /* K9   */  be_nested_str_weak(vendorid),
    /* K10  */  be_nested_str_weak(VENDOR_ID),
    /* K11  */  be_nested_str_weak(productid),
    /* K12  */  be_nested_str_weak(PRODUCT_ID),
    /* K13  */  be_nested_str_weak(root_iterations),
    /* K14  */  be_nested_str_weak(PBKDF_ITERATIONS),
    /* K15  */  be_nested_str_weak(root_salt),
    /* K16  */  be_nested_str_weak(random),
    /* K17  */  be_nested_str_weak(ipv4only),
    /* K18  */  be_nested_str_weak(load_param),
    /* K19  */  be_nested_str_weak(sessions),
    /* K20  */  be_nested_str_weak(Session_Store),
    /* K21  */  be_nested_str_weak(load_fabrics),
    /* K22  */  be_nested_str_weak(message_handler),
    /* K23  */  be_nested_str_weak(MessageHandler),
    /* K24  */  be_nested_str_weak(ui),
    /* K25  */  be_nested_str_weak(wifi),
    /* K26  */  be_nested_str_weak(up),
    /* K27  */  be_nested_str_weak(eth),
    /* K28  */  be_nested_str_weak(start),
    /* K29  */  be_nested_str_weak(add_rule),
    /* K30  */  be_nested_str_weak(Wifi_X23Connected),
    /* K31  */  be_nested_str_weak(matter_start),
    /* K32  */  be_nested_str_weak(Eth_X23Connected),
    /* K33  */  be_nested_str_weak(_init_basic_commissioning),
    /* K34  */  be_nested_str_weak(add_driver),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0xB8160800,  //  0004  GETNGBL	R5	K4
      0x88140B05,  //  0005  GETMBR	R5	R5	K5
      0x7C0C0400,  //  0006  CALL	R3	2
      0x740E0004,  //  0007  JMPT	R3	#000D
      0xB80E0800,  //  0008  GETNGBL	R3	K4
      0x8C0C0706,  //  0009  GETMET	R3	R3	K6
      0x5C140000,  //  000A  MOVE	R5	R0
      0x7C0C0400,  //  000B  CALL	R3	2
      0x80000600,  //  000C  RET	0
      0x500C0000,  //  000D  LDBOOL	R3	0	0
      0x90020E03,  //  000E  SETMBR	R0	K7	R3
      0x600C0012,  //  000F  GETGBL	R3	G18
      0x7C0C0000,  //  0010  CALL	R3	0
      0x90021003,  //  0011  SETMBR	R0	K8	R3
      0x880C010A,  //  0012  GETMBR	R3	R0	K10
      0x90021203,  //  0013  SETMBR	R0	K9	R3
      0x880C010C,  //  0014  GETMBR	R3	R0	K12
      0x90021603,  //  0015  SETMBR	R0	K11	R3
      0x880C010E,  //  0016  GETMBR	R3	R0	K14
      0x90021A03,  //  0017  SETMBR	R0	K13	R3
      0x8C0C0310,  //  0018  GETMET	R3	R1	K16
      0x5416000F,  //  0019  LDINT	R5	16
      0x7C0C0400,  //  001A  CALL	R3	2
      0x90021E03,  //  001B  SETMBR	R0	K15	R3
      0x500C0000,  //  001C  LDBOOL	R3	0	0
      0x90022203,  //  001D  SETMBR	R0	K17	R3
      0x8C0C0112,  //  001E  GETMET	R3	R0	K18
      0x7C0C0200,  //  001F  CALL	R3	1
      0xB80E0800,  //  0020  GETNGBL	R3	K4
      0x8C0C0714,  //  0021  GETMET	R3	R3	K20
      0x7C0C0200,  //  0022  CALL	R3	1
      0x90022603,  //  0023  SETMBR	R0	K19	R3
      0x880C0113,  //  0024  GETMBR	R3	R0	K19
      0x8C0C0715,  //  0025  GETMET	R3	R3	K21
      0x7C0C0200,  //  0026  CALL	R3	1
      0xB80E0800,  //  0027  GETNGBL	R3	K4
      0x8C0C0717,  //  0028  GETMET	R3	R3	K23
      0x5C140000,  //  0029  MOVE	R5	R0
      0x7C0C0400,  //  002A  CALL	R3	2
      0x90022C03,  //  002B  SETMBR	R0	K22	R3
      0xB80E0800,  //  002C  GETNGBL	R3	K4
      0x8C0C0706,  //  002D  GETMET	R3	R3	K6
      0x5C140000,  //  002E  MOVE	R5	R0
      0x7C0C0400,  //  002F  CALL	R3	2
      0x90023003,  //  0030  SETMBR	R0	K24	R3
      0xB80E0400,  //  0031  GETNGBL	R3	K2
      0x8C0C0719,  //  0032  GETMET	R3	R3	K25
      0x7C0C0200,  //  0033  CALL	R3	1
      0x940C071A,  //  0034  GETIDX	R3	R3	K26
      0x740E0004,  //  0035  JMPT	R3	#003B
      0xB80E0400,  //  0036  GETNGBL	R3	K2
      0x8C0C071B,  //  0037  GETMET	R3	R3	K27
      0x7C0C0200,  //  0038  CALL	R3	1
      0x940C071A,  //  0039  GETIDX	R3	R3	K26
      0x780E0001,  //  003A  JMPF	R3	#003D
      0x8C0C011C,  //  003B  GETMET	R3	R0	K28
      0x7C0C0200,  //  003C  CALL	R3	1
      0xB80E0400,  //  003D  GETNGBL	R3	K2
      0x8C0C0719,  //  003E  GETMET	R3	R3	K25
      0x7C0C0200,  //  003F  CALL	R3	1
      0x940C071A,  //  0040  GETIDX	R3	R3	K26
      0x740E0005,  //  0041  JMPT	R3	#0048
      0xB80E0400,  //  0042  GETNGBL	R3	K2
      0x8C0C071D,  //  0043  GETMET	R3	R3	K29
      0x5814001E,  //  0044  LDCONST	R5	K30
      0x84180000,  //  0045  CLOSURE	R6	P0
      0x581C001F,  //  0046  LDCONST	R7	K31
      0x7C0C0800,  //  0047  CALL	R3	4
      0xB80E0400,  //  0048  GETNGBL	R3	K2
      0x8C0C071B,  //  0049  GETMET	R3	R3	K27
      0x7C0C0200,  //  004A  CALL	R3	1
      0x940C071A,  //  004B  GETIDX	R3	R3	K26
      0x740E0005,  //  004C  JMPT	R3	#0053
      0xB80E0400,  //  004D  GETNGBL	R3	K2
      0x8C0C071D,  //  004E  GETMET	R3	R3	K29
      0x58140020,  //  004F  LDCONST	R5	K32
      0x84180001,  //  0050  CLOSURE	R6	P1
      0x581C001F,  //  0051  LDCONST	R7	K31
      0x7C0C0800,  //  0052  CALL	R3	4
      0x8C0C0121,  //  0053  GETMET	R3	R0	K33
      0x7C0C0200,  //  0054  CALL	R3	1
      0xB80E0400,  //  0055  GETNGBL	R3	K2
      0x8C0C0722,  //  0056  GETMET	R3	R3	K34
      0x5C140000,  //  0057  MOVE	R5	R0
      0x7C0C0400,  //  0058  CALL	R3	2
      0xA0000000,  //  0059  CLOSE	R0
      0x80000000,  //  005A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X20Commissioning_X20complete_X20_X2A_X2A_X2A),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_weak(stop_basic_commissioning),
    }),
    be_str_weak(start_commissioning_complete),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C080600,  //  0004  CALL	R2	3
      0x8C080104,  //  0005  GETMET	R2	R0	K4
      0x7C080200,  //  0006  CALL	R2	1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_root_commissioning_open
********************************************************************/
be_local_closure(Matter_Device_is_root_commissioning_open,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(commissioning_open),
    /* K1   */  be_nested_str_weak(commissioning_admin_fabric),
    }),
    be_str_weak(is_root_commissioning_open),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_commissioning_open
********************************************************************/
be_local_closure(Matter_Device_is_commissioning_open,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(commissioning_open),
    }),
    be_str_weak(is_commissioning_open),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Matter_Device_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(remove_driver),
    /* K2   */  be_nested_str_weak(udp_server),
    /* K3   */  be_nested_str_weak(stop),
    }),
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x88040102,  //  0004  GETMBR	R1	R0	K2
      0x78060002,  //  0005  JMPF	R1	#0009
      0x88040102,  //  0006  GETMBR	R1	R0	K2
      0x8C040303,  //  0007  GETMET	R1	R1	K3
      0x7C040200,  //  0008  CALL	R1	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_op_discovery
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_op_discovery,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(reverse),
    /* K5   */  be_nested_str_weak(get_fabric_compressed),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_nested_str_weak(_X2D),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(eth),
    /* K10  */  be_nested_str_weak(find),
    /* K11  */  be_nested_str_weak(up),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(format),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20remove_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(remove_service),
    /* K17  */  be_nested_str_weak(_matter),
    /* K18  */  be_nested_str_weak(_tcp),
    /* K19  */  be_nested_str_weak(hostname_eth),
    /* K20  */  be_nested_str_weak(wifi),
    /* K21  */  be_nested_str_weak(hostname_wifi),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K23  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_remove_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[81]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA802003B,  //  0002  EXBLK	0	#003F
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140305,  //  0009  GETMET	R5	R1	K5
      0x7C140200,  //  000A  CALL	R5	1
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x7C180200,  //  000C  CALL	R6	1
      0x00180D07,  //  000D  ADD	R6	R6	K7
      0x8C1C0906,  //  000E  GETMET	R7	R4	K6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x00180C07,  //  0010  ADD	R6	R6	R7
      0xB81E1000,  //  0011  GETNGBL	R7	K8
      0x8C1C0F09,  //  0012  GETMET	R7	R7	K9
      0x7C1C0200,  //  0013  CALL	R7	1
      0x8C1C0F0A,  //  0014  GETMET	R7	R7	K10
      0x5824000B,  //  0015  LDCONST	R9	K11
      0x7C1C0400,  //  0016  CALL	R7	2
      0x781E000E,  //  0017  JMPF	R7	#0027
      0xB81E1000,  //  0018  GETNGBL	R7	K8
      0x8C1C0F0C,  //  0019  GETMET	R7	R7	K12
      0x8C24070D,  //  001A  GETMET	R9	R3	K13
      0x582C000E,  //  001B  LDCONST	R11	K14
      0x58300009,  //  001C  LDCONST	R12	K9
      0x5C340C00,  //  001D  MOVE	R13	R6
      0x7C240800,  //  001E  CALL	R9	4
      0x5828000F,  //  001F  LDCONST	R10	K15
      0x7C1C0600,  //  0020  CALL	R7	3
      0x8C1C0510,  //  0021  GETMET	R7	R2	K16
      0x58240011,  //  0022  LDCONST	R9	K17
      0x58280012,  //  0023  LDCONST	R10	K18
      0x5C2C0C00,  //  0024  MOVE	R11	R6
      0x88300113,  //  0025  GETMBR	R12	R0	K19
      0x7C1C0A00,  //  0026  CALL	R7	5
      0xB81E1000,  //  0027  GETNGBL	R7	K8
      0x8C1C0F14,  //  0028  GETMET	R7	R7	K20
      0x7C1C0200,  //  0029  CALL	R7	1
      0x8C1C0F0A,  //  002A  GETMET	R7	R7	K10
      0x5824000B,  //  002B  LDCONST	R9	K11
      0x7C1C0400,  //  002C  CALL	R7	2
      0x781E000E,  //  002D  JMPF	R7	#003D
      0xB81E1000,  //  002E  GETNGBL	R7	K8
      0x8C1C0F0C,  //  002F  GETMET	R7	R7	K12
      0x8C24070D,  //  0030  GETMET	R9	R3	K13
      0x582C000E,  //  0031  LDCONST	R11	K14
      0x58300014,  //  0032  LDCONST	R12	K20
      0x5C340C00,  //  0033  MOVE	R13	R6
      0x7C240800,  //  0034  CALL	R9	4
      0x5828000F,  //  0035  LDCONST	R10	K15
      0x7C1C0600,  //  0036  CALL	R7	3
      0x8C1C0510,  //  0037  GETMET	R7	R2	K16
      0x58240011,  //  0038  LDCONST	R9	K17
      0x58280012,  //  0039  LDCONST	R10	K18
      0x5C2C0C00,  //  003A  MOVE	R11	R6
      0x88300115,  //  003B  GETMBR	R12	R0	K21
      0x7C1C0A00,  //  003C  CALL	R7	5
      0xA8040001,  //  003D  EXBLK	1	1
      0x70020010,  //  003E  JMP		#0050
      0xAC100002,  //  003F  CATCH	R4	0	2
      0x7002000D,  //  0040  JMP		#004F
      0xB81A1000,  //  0041  GETNGBL	R6	K8
      0x8C180D0C,  //  0042  GETMET	R6	R6	K12
      0x60200008,  //  0043  GETGBL	R8	G8
      0x5C240800,  //  0044  MOVE	R9	R4
      0x7C200200,  //  0045  CALL	R8	1
      0x00222C08,  //  0046  ADD	R8	K22	R8
      0x00201117,  //  0047  ADD	R8	R8	K23
      0x60240008,  //  0048  GETGBL	R9	G8
      0x5C280A00,  //  0049  MOVE	R10	R5
      0x7C240200,  //  004A  CALL	R9	1
      0x00201009,  //  004B  ADD	R8	R8	R9
      0x5824000F,  //  004C  LDCONST	R9	K15
      0x7C180600,  //  004D  CALL	R6	3
      0x70020000,  //  004E  JMP		#0050
      0xB0080000,  //  004F  RAISE	2	R0	R0
      0x80000000,  //  0050  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(Matter_Device_msg_received,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(message_handler),
    /* K1   */  be_nested_str_weak(msg_received),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0x5C180200,  //  0002  MOVE	R6	R1
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x5C200600,  //  0004  MOVE	R8	R3
      0x7C100800,  //  0005  CALL	R4	4
      0x80040800,  //  0006  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort_distinct
********************************************************************/
be_local_closure(Matter_Device_sort_distinct,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Device),
    /* K1   */  be_const_int(1),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(remove),
    }),
    be_str_weak(sort_distinct),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x040C0701,  //  0005  SUB	R3	R3	K1
      0x400E0203,  //  0006  CONNECT	R3	K1	R3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020010,  //  0008  EXBLK	0	#001A
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x94100003,  //  000B  GETIDX	R4	R0	R3
      0x5C140600,  //  000C  MOVE	R5	R3
      0x24180B02,  //  000D  GT	R6	R5	K2
      0x781A0008,  //  000E  JMPF	R6	#0018
      0x04180B01,  //  000F  SUB	R6	R5	K1
      0x94180006,  //  0010  GETIDX	R6	R0	R6
      0x24180C04,  //  0011  GT	R6	R6	R4
      0x781A0004,  //  0012  JMPF	R6	#0018
      0x04180B01,  //  0013  SUB	R6	R5	K1
      0x94180006,  //  0014  GETIDX	R6	R0	R6
      0x98000A06,  //  0015  SETIDX	R0	R5	R6
      0x04140B01,  //  0016  SUB	R5	R5	K1
      0x7001FFF4,  //  0017  JMP		#000D
      0x98000A04,  //  0018  SETIDX	R0	R5	R4
      0x7001FFEE,  //  0019  JMP		#0009
      0x58080003,  //  001A  LDCONST	R2	K3
      0xAC080200,  //  001B  CATCH	R2	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x58080001,  //  001D  LDCONST	R2	K1
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x5C100000,  //  001F  MOVE	R4	R0
      0x7C0C0200,  //  0020  CALL	R3	1
      0x180C0701,  //  0021  LE	R3	R3	K1
      0x780E0000,  //  0022  JMPF	R3	#0024
      0x80040000,  //  0023  RET	1	R0
      0x940C0102,  //  0024  GETIDX	R3	R0	K2
      0x6010000C,  //  0025  GETGBL	R4	G12
      0x5C140000,  //  0026  MOVE	R5	R0
      0x7C100200,  //  0027  CALL	R4	1
      0x14100404,  //  0028  LT	R4	R2	R4
      0x78120009,  //  0029  JMPF	R4	#0034
      0x94100002,  //  002A  GETIDX	R4	R0	R2
      0x1C100803,  //  002B  EQ	R4	R4	R3
      0x78120003,  //  002C  JMPF	R4	#0031
      0x8C100104,  //  002D  GETMET	R4	R0	K4
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x70020001,  //  0030  JMP		#0033
      0x940C0002,  //  0031  GETIDX	R3	R0	R2
      0x00080501,  //  0032  ADD	R2	R2	K1
      0x7001FFF0,  //  0033  JMP		#0025
      0x80040000,  //  0034  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_mdns_announce_hostnames
********************************************************************/
be_local_closure(Matter_Device_start_mdns_announce_hostnames,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(_mdns_announce_hostname),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        /* K4   */  be_nested_str_weak(matter_mdns_host),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x50080000,  //  0002  LDBOOL	R2	0	0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020200,  //  0004  GETNGBL	R0	K1
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x58080003,  //  0006  LDCONST	R2	K3
          0x580C0004,  //  0007  LDCONST	R3	K4
          0x7C000600,  //  0008  CALL	R0	3
          0x80000000,  //  0009  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(_mdns_announce_hostname),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        /* K4   */  be_nested_str_weak(matter_mdns_host),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x50080200,  //  0002  LDBOOL	R2	1	0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020200,  //  0004  GETNGBL	R0	K1
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x58080003,  //  0006  LDCONST	R2	K3
          0x580C0004,  //  0007  LDCONST	R3	K4
          0x7C000600,  //  0008  CALL	R0	3
          0x80000000,  //  0009  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(wifi),
    /* K2   */  be_nested_str_weak(up),
    /* K3   */  be_nested_str_weak(_mdns_announce_hostname),
    /* K4   */  be_nested_str_weak(add_rule),
    /* K5   */  be_nested_str_weak(Wifi_X23Connected),
    /* K6   */  be_nested_str_weak(matter_mdns_host),
    /* K7   */  be_nested_str_weak(eth),
    /* K8   */  be_nested_str_weak(Eth_X23Connected),
    }),
    be_str_weak(start_mdns_announce_hostnames),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x94040302,  //  0003  GETIDX	R1	R1	K2
      0x78060003,  //  0004  JMPF	R1	#0009
      0x8C040103,  //  0005  GETMET	R1	R0	K3
      0x500C0000,  //  0006  LDBOOL	R3	0	0
      0x7C040400,  //  0007  CALL	R1	2
      0x70020005,  //  0008  JMP		#000F
      0xB8060000,  //  0009  GETNGBL	R1	K0
      0x8C040304,  //  000A  GETMET	R1	R1	K4
      0x580C0005,  //  000B  LDCONST	R3	K5
      0x84100000,  //  000C  CLOSURE	R4	P0
      0x58140006,  //  000D  LDCONST	R5	K6
      0x7C040800,  //  000E  CALL	R1	4
      0xB8060000,  //  000F  GETNGBL	R1	K0
      0x8C040307,  //  0010  GETMET	R1	R1	K7
      0x7C040200,  //  0011  CALL	R1	1
      0x94040302,  //  0012  GETIDX	R1	R1	K2
      0x78060003,  //  0013  JMPF	R1	#0018
      0x8C040103,  //  0014  GETMET	R1	R0	K3
      0x500C0200,  //  0015  LDBOOL	R3	1	0
      0x7C040400,  //  0016  CALL	R1	2
      0x70020005,  //  0017  JMP		#001E
      0xB8060000,  //  0018  GETNGBL	R1	K0
      0x8C040304,  //  0019  GETMET	R1	R1	K4
      0x580C0008,  //  001A  LDCONST	R3	K8
      0x84100001,  //  001B  CLOSURE	R4	P1
      0x58140006,  //  001C  LDCONST	R5	K6
      0x7C040800,  //  001D  CALL	R1	4
      0xA0000000,  //  001E  CLOSE	R0
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _mdns_announce_hostname
********************************************************************/
be_local_closure(Matter_Device__mdns_announce_hostname,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(start),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(eth),
    /* K5   */  be_nested_str_weak(hostname_eth),
    /* K6   */  be_nested_str_weak(replace),
    /* K7   */  be_nested_str_weak(find),
    /* K8   */  be_nested_str_weak(mac),
    /* K9   */  be_nested_str_weak(_X3A),
    /* K10  */  be_nested_str_weak(),
    /* K11  */  be_nested_str_weak(ipv4only),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(format),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eadd_hostname_X28_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
    /* K15  */  be_nested_str_weak(ip6local),
    /* K16  */  be_nested_str_weak(ip),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(add_hostname),
    /* K19  */  be_nested_str_weak(ip6),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eadd_hostname_X28_X25s_X2C_X20_X25s_X29),
    /* K21  */  be_nested_str_weak(wifi),
    /* K22  */  be_nested_str_weak(hostname_wifi),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20start_X20mDNS_X20on_X20_X25s_X20host_X20_X27_X25s_X2Elocal_X27),
    /* K24  */  be_const_int(2),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K26  */  be_nested_str_weak(_X7C),
    /* K27  */  be_nested_str_weak(mdns_announce_op_discovery_all_fabrics),
    }),
    be_str_weak(_mdns_announce_hostname),
    &be_const_str_solidified,
    ( &(const binstruction[172]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100502,  //  0002  GETMET	R4	R2	K2
      0x7C100200,  //  0003  CALL	R4	1
      0xA8020092,  //  0004  EXBLK	0	#0098
      0x78060040,  //  0005  JMPF	R1	#0047
      0xB8120600,  //  0006  GETNGBL	R4	K3
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140706,  //  0009  GETMET	R5	R3	K6
      0x8C1C0907,  //  000A  GETMET	R7	R4	K7
      0x58240008,  //  000B  LDCONST	R9	K8
      0x7C1C0400,  //  000C  CALL	R7	2
      0x58200009,  //  000D  LDCONST	R8	K9
      0x5824000A,  //  000E  LDCONST	R9	K10
      0x7C140800,  //  000F  CALL	R5	4
      0x90020A05,  //  0010  SETMBR	R0	K5	R5
      0x8814010B,  //  0011  GETMBR	R5	R0	K11
      0x7416001F,  //  0012  JMPT	R5	#0033
      0xB8160600,  //  0013  GETNGBL	R5	K3
      0x8C140B0C,  //  0014  GETMET	R5	R5	K12
      0x8C1C070D,  //  0015  GETMET	R7	R3	K13
      0x5824000E,  //  0016  LDCONST	R9	K14
      0x88280105,  //  0017  GETMBR	R10	R0	K5
      0x8C2C0907,  //  0018  GETMET	R11	R4	K7
      0x5834000F,  //  0019  LDCONST	R13	K15
      0x5838000A,  //  001A  LDCONST	R14	K10
      0x7C2C0600,  //  001B  CALL	R11	3
      0x8C300907,  //  001C  GETMET	R12	R4	K7
      0x58380010,  //  001D  LDCONST	R14	K16
      0x583C000A,  //  001E  LDCONST	R15	K10
      0x7C300600,  //  001F  CALL	R12	3
      0x7C1C0A00,  //  0020  CALL	R7	5
      0x58200011,  //  0021  LDCONST	R8	K17
      0x7C140600,  //  0022  CALL	R5	3
      0x8C140512,  //  0023  GETMET	R5	R2	K18
      0x881C0105,  //  0024  GETMBR	R7	R0	K5
      0x8C200907,  //  0025  GETMET	R8	R4	K7
      0x5828000F,  //  0026  LDCONST	R10	K15
      0x582C000A,  //  0027  LDCONST	R11	K10
      0x7C200600,  //  0028  CALL	R8	3
      0x8C240907,  //  0029  GETMET	R9	R4	K7
      0x582C0010,  //  002A  LDCONST	R11	K16
      0x5830000A,  //  002B  LDCONST	R12	K10
      0x7C240600,  //  002C  CALL	R9	3
      0x8C280907,  //  002D  GETMET	R10	R4	K7
      0x58300013,  //  002E  LDCONST	R12	K19
      0x5834000A,  //  002F  LDCONST	R13	K10
      0x7C280600,  //  0030  CALL	R10	3
      0x7C140A00,  //  0031  CALL	R5	5
      0x70020012,  //  0032  JMP		#0046
      0xB8160600,  //  0033  GETNGBL	R5	K3
      0x8C140B0C,  //  0034  GETMET	R5	R5	K12
      0x8C1C070D,  //  0035  GETMET	R7	R3	K13
      0x58240014,  //  0036  LDCONST	R9	K20
      0x88280105,  //  0037  GETMBR	R10	R0	K5
      0x8C2C0907,  //  0038  GETMET	R11	R4	K7
      0x58340010,  //  0039  LDCONST	R13	K16
      0x5838000A,  //  003A  LDCONST	R14	K10
      0x7C2C0600,  //  003B  CALL	R11	3
      0x7C1C0800,  //  003C  CALL	R7	4
      0x58200011,  //  003D  LDCONST	R8	K17
      0x7C140600,  //  003E  CALL	R5	3
      0x8C140512,  //  003F  GETMET	R5	R2	K18
      0x881C0105,  //  0040  GETMBR	R7	R0	K5
      0x8C200907,  //  0041  GETMET	R8	R4	K7
      0x58280010,  //  0042  LDCONST	R10	K16
      0x582C000A,  //  0043  LDCONST	R11	K10
      0x7C200600,  //  0044  CALL	R8	3
      0x7C140600,  //  0045  CALL	R5	3
      0x7002003F,  //  0046  JMP		#0087
      0xB8120600,  //  0047  GETNGBL	R4	K3
      0x8C100915,  //  0048  GETMET	R4	R4	K21
      0x7C100200,  //  0049  CALL	R4	1
      0x8C140706,  //  004A  GETMET	R5	R3	K6
      0x8C1C0907,  //  004B  GETMET	R7	R4	K7
      0x58240008,  //  004C  LDCONST	R9	K8
      0x7C1C0400,  //  004D  CALL	R7	2
      0x58200009,  //  004E  LDCONST	R8	K9
      0x5824000A,  //  004F  LDCONST	R9	K10
      0x7C140800,  //  0050  CALL	R5	4
      0x90022C05,  //  0051  SETMBR	R0	K22	R5
      0x8814010B,  //  0052  GETMBR	R5	R0	K11
      0x7416001F,  //  0053  JMPT	R5	#0074
      0xB8160600,  //  0054  GETNGBL	R5	K3
      0x8C140B0C,  //  0055  GETMET	R5	R5	K12
      0x8C1C070D,  //  0056  GETMET	R7	R3	K13
      0x5824000E,  //  0057  LDCONST	R9	K14
      0x88280116,  //  0058  GETMBR	R10	R0	K22
      0x8C2C0907,  //  0059  GETMET	R11	R4	K7
      0x5834000F,  //  005A  LDCONST	R13	K15
      0x5838000A,  //  005B  LDCONST	R14	K10
      0x7C2C0600,  //  005C  CALL	R11	3
      0x8C300907,  //  005D  GETMET	R12	R4	K7
      0x58380010,  //  005E  LDCONST	R14	K16
      0x583C000A,  //  005F  LDCONST	R15	K10
      0x7C300600,  //  0060  CALL	R12	3
      0x7C1C0A00,  //  0061  CALL	R7	5
      0x58200011,  //  0062  LDCONST	R8	K17
      0x7C140600,  //  0063  CALL	R5	3
      0x8C140512,  //  0064  GETMET	R5	R2	K18
      0x881C0116,  //  0065  GETMBR	R7	R0	K22
      0x8C200907,  //  0066  GETMET	R8	R4	K7
      0x5828000F,  //  0067  LDCONST	R10	K15
      0x582C000A,  //  0068  LDCONST	R11	K10
      0x7C200600,  //  0069  CALL	R8	3
      0x8C240907,  //  006A  GETMET	R9	R4	K7
      0x582C0010,  //  006B  LDCONST	R11	K16
      0x5830000A,  //  006C  LDCONST	R12	K10
      0x7C240600,  //  006D  CALL	R9	3
      0x8C280907,  //  006E  GETMET	R10	R4	K7
      0x58300013,  //  006F  LDCONST	R12	K19
      0x5834000A,  //  0070  LDCONST	R13	K10
      0x7C280600,  //  0071  CALL	R10	3
      0x7C140A00,  //  0072  CALL	R5	5
      0x70020012,  //  0073  JMP		#0087
      0xB8160600,  //  0074  GETNGBL	R5	K3
      0x8C140B0C,  //  0075  GETMET	R5	R5	K12
      0x8C1C070D,  //  0076  GETMET	R7	R3	K13
      0x58240014,  //  0077  LDCONST	R9	K20
      0x88280105,  //  0078  GETMBR	R10	R0	K5
      0x8C2C0907,  //  0079  GETMET	R11	R4	K7
      0x58340010,  //  007A  LDCONST	R13	K16
      0x5838000A,  //  007B  LDCONST	R14	K10
      0x7C2C0600,  //  007C  CALL	R11	3
      0x7C1C0800,  //  007D  CALL	R7	4
      0x58200011,  //  007E  LDCONST	R8	K17
      0x7C140600,  //  007F  CALL	R5	3
      0x8C140512,  //  0080  GETMET	R5	R2	K18
      0x881C0116,  //  0081  GETMBR	R7	R0	K22
      0x8C200907,  //  0082  GETMET	R8	R4	K7
      0x58280010,  //  0083  LDCONST	R10	K16
      0x582C000A,  //  0084  LDCONST	R11	K10
      0x7C200600,  //  0085  CALL	R8	3
      0x7C140600,  //  0086  CALL	R5	3
      0xB8120600,  //  0087  GETNGBL	R4	K3
      0x8C10090C,  //  0088  GETMET	R4	R4	K12
      0x8C18070D,  //  0089  GETMET	R6	R3	K13
      0x58200017,  //  008A  LDCONST	R8	K23
      0x78060001,  //  008B  JMPF	R1	#008E
      0x58240004,  //  008C  LDCONST	R9	K4
      0x70020000,  //  008D  JMP		#008F
      0x58240015,  //  008E  LDCONST	R9	K21
      0x78060001,  //  008F  JMPF	R1	#0092
      0x88280105,  //  0090  GETMBR	R10	R0	K5
      0x70020000,  //  0091  JMP		#0093
      0x88280116,  //  0092  GETMBR	R10	R0	K22
      0x7C180800,  //  0093  CALL	R6	4
      0x581C0018,  //  0094  LDCONST	R7	K24
      0x7C100600,  //  0095  CALL	R4	3
      0xA8040001,  //  0096  EXBLK	1	1
      0x70020010,  //  0097  JMP		#00A9
      0xAC100002,  //  0098  CATCH	R4	0	2
      0x7002000D,  //  0099  JMP		#00A8
      0xB81A0600,  //  009A  GETNGBL	R6	K3
      0x8C180D0C,  //  009B  GETMET	R6	R6	K12
      0x60200008,  //  009C  GETGBL	R8	G8
      0x5C240800,  //  009D  MOVE	R9	R4
      0x7C200200,  //  009E  CALL	R8	1
      0x00223208,  //  009F  ADD	R8	K25	R8
      0x0020111A,  //  00A0  ADD	R8	R8	K26
      0x60240008,  //  00A1  GETGBL	R9	G8
      0x5C280A00,  //  00A2  MOVE	R10	R5
      0x7C240200,  //  00A3  CALL	R9	1
      0x00201009,  //  00A4  ADD	R8	R8	R9
      0x58240018,  //  00A5  LDCONST	R9	K24
      0x7C180600,  //  00A6  CALL	R6	3
      0x70020000,  //  00A7  JMP		#00A9
      0xB0080000,  //  00A8  RAISE	2	R0	R0
      0x8C10011B,  //  00A9  GETMET	R4	R0	K27
      0x7C100200,  //  00AA  CALL	R4	1
      0x80000000,  //  00AB  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_param
********************************************************************/
be_local_closure(Matter_Device_load_param,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(FILENAME),
    /* K3   */  be_nested_str_weak(read),
    /* K4   */  be_nested_str_weak(close),
    /* K5   */  be_nested_str_weak(json),
    /* K6   */  be_nested_str_weak(load),
    /* K7   */  be_nested_str_weak(root_discriminator),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_nested_str_weak(distinguish),
    /* K10  */  be_nested_str_weak(root_passcode),
    /* K11  */  be_nested_str_weak(passcode),
    /* K12  */  be_nested_str_weak(ipv4only),
    /* K13  */  be_nested_str_weak(io_error),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
    /* K17  */  be_nested_str_weak(_X7C),
    /* K18  */  be_const_int(2),
    /* K19  */  be_nested_str_weak(random),
    /* K20  */  be_nested_str_weak(get),
    /* K21  */  be_const_int(0),
    /* K22  */  be_nested_str_weak(PASSCODE_DEFAULT),
    /* K23  */  be_nested_str_weak(save_param),
    }),
    be_str_weak(load_param),
    &be_const_str_solidified,
    ( &(const binstruction[79]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA802001D,  //  0002  EXBLK	0	#0021
      0x600C0011,  //  0003  GETGBL	R3	G17
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C100703,  //  0006  GETMET	R4	R3	K3
      0x7C100200,  //  0007  CALL	R4	1
      0x8C140704,  //  0008  GETMET	R5	R3	K4
      0x7C140200,  //  0009  CALL	R5	1
      0xA4160A00,  //  000A  IMPORT	R5	K5
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x5C200800,  //  000C  MOVE	R8	R4
      0x7C180400,  //  000D  CALL	R6	2
      0x8C1C0D08,  //  000E  GETMET	R7	R6	K8
      0x58240009,  //  000F  LDCONST	R9	K9
      0x88280107,  //  0010  GETMBR	R10	R0	K7
      0x7C1C0600,  //  0011  CALL	R7	3
      0x90020E07,  //  0012  SETMBR	R0	K7	R7
      0x8C1C0D08,  //  0013  GETMET	R7	R6	K8
      0x5824000B,  //  0014  LDCONST	R9	K11
      0x8828010A,  //  0015  GETMBR	R10	R0	K10
      0x7C1C0600,  //  0016  CALL	R7	3
      0x90021407,  //  0017  SETMBR	R0	K10	R7
      0x601C0017,  //  0018  GETGBL	R7	G23
      0x8C200D08,  //  0019  GETMET	R8	R6	K8
      0x5828000C,  //  001A  LDCONST	R10	K12
      0x502C0000,  //  001B  LDBOOL	R11	0	0
      0x7C200600,  //  001C  CALL	R8	3
      0x7C1C0200,  //  001D  CALL	R7	1
      0x90021807,  //  001E  SETMBR	R0	K12	R7
      0xA8040001,  //  001F  EXBLK	1	1
      0x70020012,  //  0020  JMP		#0034
      0xAC0C0002,  //  0021  CATCH	R3	0	2
      0x7002000F,  //  0022  JMP		#0033
      0x2014070D,  //  0023  NE	R5	R3	K13
      0x7816000C,  //  0024  JMPF	R5	#0032
      0xB8161C00,  //  0025  GETNGBL	R5	K14
      0x8C140B0F,  //  0026  GETMET	R5	R5	K15
      0x601C0008,  //  0027  GETGBL	R7	G8
      0x5C200600,  //  0028  MOVE	R8	R3
      0x7C1C0200,  //  0029  CALL	R7	1
      0x001E2007,  //  002A  ADD	R7	K16	R7
      0x001C0F11,  //  002B  ADD	R7	R7	K17
      0x60200008,  //  002C  GETGBL	R8	G8
      0x5C240800,  //  002D  MOVE	R9	R4
      0x7C200200,  //  002E  CALL	R8	1
      0x001C0E08,  //  002F  ADD	R7	R7	R8
      0x58200012,  //  0030  LDCONST	R8	K18
      0x7C140600,  //  0031  CALL	R5	3
      0x70020000,  //  0032  JMP		#0034
      0xB0080000,  //  0033  RAISE	2	R0	R0
      0x500C0000,  //  0034  LDBOOL	R3	0	0
      0x88100107,  //  0035  GETMBR	R4	R0	K7
      0x4C140000,  //  0036  LDNIL	R5
      0x1C100805,  //  0037  EQ	R4	R4	R5
      0x7812000A,  //  0038  JMPF	R4	#0044
      0x8C100513,  //  0039  GETMET	R4	R2	K19
      0x58180012,  //  003A  LDCONST	R6	K18
      0x7C100400,  //  003B  CALL	R4	2
      0x8C100914,  //  003C  GETMET	R4	R4	K20
      0x58180015,  //  003D  LDCONST	R6	K21
      0x581C0012,  //  003E  LDCONST	R7	K18
      0x7C100600,  //  003F  CALL	R4	3
      0x54160FFE,  //  0040  LDINT	R5	4095
      0x2C100805,  //  0041  AND	R4	R4	R5
      0x90020E04,  //  0042  SETMBR	R0	K7	R4
      0x500C0200,  //  0043  LDBOOL	R3	1	0
      0x8810010A,  //  0044  GETMBR	R4	R0	K10
      0x4C140000,  //  0045  LDNIL	R5
      0x1C100805,  //  0046  EQ	R4	R4	R5
      0x78120002,  //  0047  JMPF	R4	#004B
      0x88100116,  //  0048  GETMBR	R4	R0	K22
      0x90021404,  //  0049  SETMBR	R0	K10	R4
      0x500C0200,  //  004A  LDBOOL	R3	1	0
      0x780E0001,  //  004B  JMPF	R3	#004E
      0x8C100117,  //  004C  GETMET	R4	R0	K23
      0x7C100200,  //  004D  CALL	R4	1
      0x80000000,  //  004E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _compute_pbkdf
********************************************************************/
be_local_closure(Matter_Device__compute_pbkdf,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(add),
    /* K3   */  be_nested_str_weak(PBKDF2_HMAC_SHA256),
    /* K4   */  be_nested_str_weak(derive),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(root_w0),
    /* K7   */  be_nested_str_weak(EC_P256),
    /* K8   */  be_nested_str_weak(mod),
    /* K9   */  be_nested_str_weak(root_L),
    /* K10  */  be_nested_str_weak(public_key),
    }),
    be_str_weak(_compute_pbkdf),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xA4160200,  //  0001  IMPORT	R5	K1
      0x60180015,  //  0002  GETGBL	R6	G21
      0x7C180000,  //  0003  CALL	R6	0
      0x8C180D02,  //  0004  GETMET	R6	R6	K2
      0x5C200200,  //  0005  MOVE	R8	R1
      0x54260003,  //  0006  LDINT	R9	4
      0x7C180600,  //  0007  CALL	R6	3
      0x8C1C0903,  //  0008  GETMET	R7	R4	K3
      0x7C1C0200,  //  0009  CALL	R7	1
      0x8C1C0F04,  //  000A  GETMET	R7	R7	K4
      0x5C240C00,  //  000B  MOVE	R9	R6
      0x5C280600,  //  000C  MOVE	R10	R3
      0x5C2C0400,  //  000D  MOVE	R11	R2
      0x5432004F,  //  000E  LDINT	R12	80
      0x7C1C0A00,  //  000F  CALL	R7	5
      0x54220026,  //  0010  LDINT	R8	39
      0x40220A08,  //  0011  CONNECT	R8	K5	R8
      0x94200E08,  //  0012  GETIDX	R8	R7	R8
      0x54260027,  //  0013  LDINT	R9	40
      0x542A004E,  //  0014  LDINT	R10	79
      0x4024120A,  //  0015  CONNECT	R9	R9	R10
      0x94240E09,  //  0016  GETIDX	R9	R7	R9
      0x8C280907,  //  0017  GETMET	R10	R4	K7
      0x7C280200,  //  0018  CALL	R10	1
      0x8C281508,  //  0019  GETMET	R10	R10	K8
      0x5C301000,  //  001A  MOVE	R12	R8
      0x7C280400,  //  001B  CALL	R10	2
      0x90020C0A,  //  001C  SETMBR	R0	K6	R10
      0x8C280907,  //  001D  GETMET	R10	R4	K7
      0x7C280200,  //  001E  CALL	R10	1
      0x8C281508,  //  001F  GETMET	R10	R10	K8
      0x5C301200,  //  0020  MOVE	R12	R9
      0x7C280400,  //  0021  CALL	R10	2
      0x8C2C0907,  //  0022  GETMET	R11	R4	K7
      0x7C2C0200,  //  0023  CALL	R11	1
      0x8C2C170A,  //  0024  GETMET	R11	R11	K10
      0x5C341400,  //  0025  MOVE	R13	R10
      0x7C2C0400,  //  0026  CALL	R11	2
      0x9002120B,  //  0027  SETMBR	R0	K9	R11
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Device_invoke_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(plugins),
    /* K3   */  be_nested_str_weak(invoke_request),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(status),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58100000,  //  0000  LDCONST	R4	K0
      0x88140701,  //  0001  GETMBR	R5	R3	K1
      0x6018000C,  //  0002  GETGBL	R6	G12
      0x881C0102,  //  0003  GETMBR	R7	R0	K2
      0x7C180200,  //  0004  CALL	R6	1
      0x14180806,  //  0005  LT	R6	R4	R6
      0x781A000C,  //  0006  JMPF	R6	#0014
      0x88180102,  //  0007  GETMBR	R6	R0	K2
      0x94180C04,  //  0008  GETIDX	R6	R6	R4
      0x881C0D01,  //  0009  GETMBR	R7	R6	K1
      0x1C1C0E05,  //  000A  EQ	R7	R7	R5
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0D03,  //  000C  GETMET	R7	R6	K3
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280400,  //  000E  MOVE	R10	R2
      0x5C2C0600,  //  000F  MOVE	R11	R3
      0x7C1C0800,  //  0010  CALL	R7	4
      0x80040E00,  //  0011  RET	1	R7
      0x00100904,  //  0012  ADD	R4	R4	K4
      0x7001FFED,  //  0013  JMP		#0002
      0xB81A0C00,  //  0014  GETNGBL	R6	K6
      0x88180D07,  //  0015  GETMBR	R6	R6	K7
      0x900E0A06,  //  0016  SETMBR	R3	K5	R6
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_PASE
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_PASE,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(mdns_pase_eth),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eremove_service_X28_X25s_X2C_X20_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
    /* K7   */  be_nested_str_weak(_matterc),
    /* K8   */  be_nested_str_weak(_udp),
    /* K9   */  be_nested_str_weak(commissioning_instance_eth),
    /* K10  */  be_nested_str_weak(hostname_eth),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20remove_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27),
    /* K13  */  be_nested_str_weak(eth),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(remove_service),
    /* K16  */  be_nested_str_weak(mdns_pase_wifi),
    /* K17  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K18  */  be_nested_str_weak(hostname_wifi),
    /* K19  */  be_nested_str_weak(wifi),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K21  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_remove_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA802003D,  //  0002  EXBLK	0	#0041
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x780E001B,  //  0004  JMPF	R3	#0021
      0xB80E0600,  //  0005  GETNGBL	R3	K3
      0x8C0C0704,  //  0006  GETMET	R3	R3	K4
      0x8C140505,  //  0007  GETMET	R5	R2	K5
      0x581C0006,  //  0008  LDCONST	R7	K6
      0x58200007,  //  0009  LDCONST	R8	K7
      0x58240008,  //  000A  LDCONST	R9	K8
      0x88280109,  //  000B  GETMBR	R10	R0	K9
      0x882C010A,  //  000C  GETMBR	R11	R0	K10
      0x7C140C00,  //  000D  CALL	R5	6
      0x5818000B,  //  000E  LDCONST	R6	K11
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB80E0600,  //  0010  GETNGBL	R3	K3
      0x8C0C0704,  //  0011  GETMET	R3	R3	K4
      0x8C140505,  //  0012  GETMET	R5	R2	K5
      0x581C000C,  //  0013  LDCONST	R7	K12
      0x5820000D,  //  0014  LDCONST	R8	K13
      0x88240109,  //  0015  GETMBR	R9	R0	K9
      0x7C140800,  //  0016  CALL	R5	4
      0x5818000E,  //  0017  LDCONST	R6	K14
      0x7C0C0600,  //  0018  CALL	R3	3
      0x500C0000,  //  0019  LDBOOL	R3	0	0
      0x90020403,  //  001A  SETMBR	R0	K2	R3
      0x8C0C030F,  //  001B  GETMET	R3	R1	K15
      0x58140007,  //  001C  LDCONST	R5	K7
      0x58180008,  //  001D  LDCONST	R6	K8
      0x881C0109,  //  001E  GETMBR	R7	R0	K9
      0x8820010A,  //  001F  GETMBR	R8	R0	K10
      0x7C0C0A00,  //  0020  CALL	R3	5
      0x880C0110,  //  0021  GETMBR	R3	R0	K16
      0x780E001B,  //  0022  JMPF	R3	#003F
      0xB80E0600,  //  0023  GETNGBL	R3	K3
      0x8C0C0704,  //  0024  GETMET	R3	R3	K4
      0x8C140505,  //  0025  GETMET	R5	R2	K5
      0x581C0006,  //  0026  LDCONST	R7	K6
      0x58200007,  //  0027  LDCONST	R8	K7
      0x58240008,  //  0028  LDCONST	R9	K8
      0x88280111,  //  0029  GETMBR	R10	R0	K17
      0x882C0112,  //  002A  GETMBR	R11	R0	K18
      0x7C140C00,  //  002B  CALL	R5	6
      0x5818000B,  //  002C  LDCONST	R6	K11
      0x7C0C0600,  //  002D  CALL	R3	3
      0xB80E0600,  //  002E  GETNGBL	R3	K3
      0x8C0C0704,  //  002F  GETMET	R3	R3	K4
      0x8C140505,  //  0030  GETMET	R5	R2	K5
      0x581C000C,  //  0031  LDCONST	R7	K12
      0x58200013,  //  0032  LDCONST	R8	K19
      0x88240111,  //  0033  GETMBR	R9	R0	K17
      0x7C140800,  //  0034  CALL	R5	4
      0x5818000E,  //  0035  LDCONST	R6	K14
      0x7C0C0600,  //  0036  CALL	R3	3
      0x500C0000,  //  0037  LDBOOL	R3	0	0
      0x90022003,  //  0038  SETMBR	R0	K16	R3
      0x8C0C030F,  //  0039  GETMET	R3	R1	K15
      0x58140007,  //  003A  LDCONST	R5	K7
      0x58180008,  //  003B  LDCONST	R6	K8
      0x881C0111,  //  003C  GETMBR	R7	R0	K17
      0x88200112,  //  003D  GETMBR	R8	R0	K18
      0x7C0C0A00,  //  003E  CALL	R3	5
      0xA8040001,  //  003F  EXBLK	1	1
      0x70020010,  //  0040  JMP		#0052
      0xAC0C0002,  //  0041  CATCH	R3	0	2
      0x7002000D,  //  0042  JMP		#0051
      0xB8160600,  //  0043  GETNGBL	R5	K3
      0x8C140B04,  //  0044  GETMET	R5	R5	K4
      0x601C0008,  //  0045  GETGBL	R7	G8
      0x5C200600,  //  0046  MOVE	R8	R3
      0x7C1C0200,  //  0047  CALL	R7	1
      0x001E2807,  //  0048  ADD	R7	K20	R7
      0x001C0F15,  //  0049  ADD	R7	R7	K21
      0x60200008,  //  004A  GETGBL	R8	G8
      0x5C240800,  //  004B  MOVE	R9	R4
      0x7C200200,  //  004C  CALL	R8	1
      0x001C0E08,  //  004D  ADD	R7	R7	R8
      0x5820000E,  //  004E  LDCONST	R8	K14
      0x7C140600,  //  004F  CALL	R5	3
      0x70020000,  //  0050  JMP		#0052
      0xB0080000,  //  0051  RAISE	2	R0	R0
      0x80000000,  //  0052  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery_all_fabrics
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery_all_fabrics,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(active_fabrics),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(get_fabric_id),
    /* K4   */  be_nested_str_weak(mdns_announce_op_discovery),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(mdns_announce_op_discovery_all_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000B,  //  0005  EXBLK	0	#0012
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x780E0005,  //  000A  JMPF	R3	#0011
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x8C0C0104,  //  000E  GETMET	R3	R0	K4
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x7001FFF3,  //  0011  JMP		#0006
      0x58040005,  //  0012  LDCONST	R1	K5
      0xAC040200,  //  0013  CATCH	R1	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active_endpoints
********************************************************************/
be_local_closure(Matter_Device_get_active_endpoints,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins),
    /* K1   */  be_nested_str_weak(get_endpoint),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_active_endpoints),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA8020011,  //  0005  EXBLK	0	#0018
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140901,  //  0008  GETMET	R5	R4	K1
      0x7C140200,  //  0009  CALL	R5	1
      0x78060002,  //  000A  JMPF	R1	#000E
      0x1C180B02,  //  000B  EQ	R6	R5	K2
      0x781A0000,  //  000C  JMPF	R6	#000E
      0x7001FFF7,  //  000D  JMP		#0006
      0x8C180503,  //  000E  GETMET	R6	R2	K3
      0x5C200A00,  //  000F  MOVE	R8	R5
      0x7C180400,  //  0010  CALL	R6	2
      0x4C1C0000,  //  0011  LDNIL	R7
      0x1C180C07,  //  0012  EQ	R6	R6	R7
      0x781A0002,  //  0013  JMPF	R6	#0017
      0x8C180504,  //  0014  GETMET	R6	R2	K4
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x7C180400,  //  0016  CALL	R6	2
      0x7001FFED,  //  0017  JMP		#0006
      0x580C0005,  //  0018  LDCONST	R3	K5
      0xAC0C0200,  //  0019  CATCH	R3	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x80040400,  //  001B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device
********************************************************************/
be_local_closure(Matter_Device_autoconf_device,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(light),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(channels),
    /* K6   */  be_nested_str_weak(),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(plugins),
    /* K9   */  be_nested_str_weak(push),
    /* K10  */  be_nested_str_weak(matter),
    /* K11  */  be_nested_str_weak(Plugin_Light1),
    /* K12  */  be_nested_str_weak(tasmota),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(format),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20Endpoint_X3A_X25i_X20Light_Dimmer),
    /* K16  */  be_const_int(2),
    /* K17  */  be_nested_str_weak(Plugin_Light2),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20Endpoint_X3A_X25i_X20Light_CT),
    /* K19  */  be_nested_str_weak(Plugin_Light3),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20Endpoint_X3A_X25i_X20Light_RGB),
    /* K21  */  be_nested_str_weak(get_power),
    /* K22  */  be_nested_str_weak(Plugin_OnOff),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20Endpoint_X3A_X25i_X20Relay__X25i),
    }),
    be_str_weak(autoconf_device),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x58080001,  //  0001  LDCONST	R2	K1
      0x500C0000,  //  0002  LDBOOL	R3	0	0
      0xA4120400,  //  0003  IMPORT	R4	K2
      0x8C140903,  //  0004  GETMET	R5	R4	K3
      0x7C140200,  //  0005  CALL	R5	1
      0x4C180000,  //  0006  LDNIL	R6
      0x20180A06,  //  0007  NE	R6	R5	R6
      0x781A003F,  //  0008  JMPF	R6	#0049
      0x6018000C,  //  0009  GETGBL	R6	G12
      0x8C1C0B04,  //  000A  GETMET	R7	R5	K4
      0x58240005,  //  000B  LDCONST	R9	K5
      0x58280006,  //  000C  LDCONST	R10	K6
      0x7C1C0600,  //  000D  CALL	R7	3
      0x7C180200,  //  000E  CALL	R6	1
      0x241C0D07,  //  000F  GT	R7	R6	K7
      0x781E0037,  //  0010  JMPF	R7	#0049
      0x1C1C0D01,  //  0011  EQ	R7	R6	K1
      0x781E0010,  //  0012  JMPF	R7	#0024
      0x881C0108,  //  0013  GETMBR	R7	R0	K8
      0x8C1C0F09,  //  0014  GETMET	R7	R7	K9
      0xB8261400,  //  0015  GETNGBL	R9	K10
      0x8C24130B,  //  0016  GETMET	R9	R9	K11
      0x5C2C0000,  //  0017  MOVE	R11	R0
      0x5C300400,  //  0018  MOVE	R12	R2
      0x7C240600,  //  0019  CALL	R9	3
      0x7C1C0400,  //  001A  CALL	R7	2
      0xB81E1800,  //  001B  GETNGBL	R7	K12
      0x8C1C0F0D,  //  001C  GETMET	R7	R7	K13
      0x8C24030E,  //  001D  GETMET	R9	R1	K14
      0x582C000F,  //  001E  LDCONST	R11	K15
      0x5C300400,  //  001F  MOVE	R12	R2
      0x7C240600,  //  0020  CALL	R9	3
      0x58280010,  //  0021  LDCONST	R10	K16
      0x7C1C0600,  //  0022  CALL	R7	3
      0x70020022,  //  0023  JMP		#0047
      0x1C1C0D10,  //  0024  EQ	R7	R6	K16
      0x781E0010,  //  0025  JMPF	R7	#0037
      0x881C0108,  //  0026  GETMBR	R7	R0	K8
      0x8C1C0F09,  //  0027  GETMET	R7	R7	K9
      0xB8261400,  //  0028  GETNGBL	R9	K10
      0x8C241311,  //  0029  GETMET	R9	R9	K17
      0x5C2C0000,  //  002A  MOVE	R11	R0
      0x5C300400,  //  002B  MOVE	R12	R2
      0x7C240600,  //  002C  CALL	R9	3
      0x7C1C0400,  //  002D  CALL	R7	2
      0xB81E1800,  //  002E  GETNGBL	R7	K12
      0x8C1C0F0D,  //  002F  GETMET	R7	R7	K13
      0x8C24030E,  //  0030  GETMET	R9	R1	K14
      0x582C0012,  //  0031  LDCONST	R11	K18
      0x5C300400,  //  0032  MOVE	R12	R2
      0x7C240600,  //  0033  CALL	R9	3
      0x58280010,  //  0034  LDCONST	R10	K16
      0x7C1C0600,  //  0035  CALL	R7	3
      0x7002000F,  //  0036  JMP		#0047
      0x881C0108,  //  0037  GETMBR	R7	R0	K8
      0x8C1C0F09,  //  0038  GETMET	R7	R7	K9
      0xB8261400,  //  0039  GETNGBL	R9	K10
      0x8C241313,  //  003A  GETMET	R9	R9	K19
      0x5C2C0000,  //  003B  MOVE	R11	R0
      0x5C300400,  //  003C  MOVE	R12	R2
      0x7C240600,  //  003D  CALL	R9	3
      0x7C1C0400,  //  003E  CALL	R7	2
      0xB81E1800,  //  003F  GETNGBL	R7	K12
      0x8C1C0F0D,  //  0040  GETMET	R7	R7	K13
      0x8C24030E,  //  0041  GETMET	R9	R1	K14
      0x582C0014,  //  0042  LDCONST	R11	K20
      0x5C300400,  //  0043  MOVE	R12	R2
      0x7C240600,  //  0044  CALL	R9	3
      0x58280010,  //  0045  LDCONST	R10	K16
      0x7C1C0600,  //  0046  CALL	R7	3
      0x500C0200,  //  0047  LDBOOL	R3	1	0
      0x00080501,  //  0048  ADD	R2	R2	K1
      0x6018000C,  //  0049  GETGBL	R6	G12
      0xB81E1800,  //  004A  GETNGBL	R7	K12
      0x8C1C0F15,  //  004B  GETMET	R7	R7	K21
      0x7C1C0200,  //  004C  CALL	R7	1
      0x7C180200,  //  004D  CALL	R6	1
      0x581C0007,  //  004E  LDCONST	R7	K7
      0x780E0000,  //  004F  JMPF	R3	#0051
      0x04180D01,  //  0050  SUB	R6	R6	K1
      0x14200E06,  //  0051  LT	R8	R7	R6
      0x78220014,  //  0052  JMPF	R8	#0068
      0x88200108,  //  0053  GETMBR	R8	R0	K8
      0x8C201109,  //  0054  GETMET	R8	R8	K9
      0xB82A1400,  //  0055  GETNGBL	R10	K10
      0x8C281516,  //  0056  GETMET	R10	R10	K22
      0x5C300000,  //  0057  MOVE	R12	R0
      0x5C340400,  //  0058  MOVE	R13	R2
      0x5C380E00,  //  0059  MOVE	R14	R7
      0x7C280800,  //  005A  CALL	R10	4
      0x7C200400,  //  005B  CALL	R8	2
      0xB8221800,  //  005C  GETNGBL	R8	K12
      0x8C20110D,  //  005D  GETMET	R8	R8	K13
      0x8C28030E,  //  005E  GETMET	R10	R1	K14
      0x58300017,  //  005F  LDCONST	R12	K23
      0x5C340400,  //  0060  MOVE	R13	R2
      0x00380F01,  //  0061  ADD	R14	R7	K1
      0x7C280800,  //  0062  CALL	R10	4
      0x582C0010,  //  0063  LDCONST	R11	K16
      0x7C200600,  //  0064  CALL	R8	3
      0x001C0F01,  //  0065  ADD	R7	R7	K1
      0x00080501,  //  0066  ADD	R2	R2	K1
      0x7001FFE8,  //  0067  JMP		#0051
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_send
********************************************************************/
be_local_closure(Matter_Device_msg_send,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(send_UDP),
    }),
    be_str_weak(msg_send),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Device_every_second,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(every_second),
    /* K2   */  be_nested_str_weak(message_handler),
    /* K3   */  be_nested_str_weak(commissioning_open),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(time_reached),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(plugins),
    /* K8   */  be_const_int(1),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x8C040301,  //  0004  GETMET	R1	R1	K1
      0x7C040200,  //  0005  CALL	R1	1
      0x88040103,  //  0006  GETMBR	R1	R0	K3
      0x4C080000,  //  0007  LDNIL	R2
      0x20040202,  //  0008  NE	R1	R1	R2
      0x78060006,  //  0009  JMPF	R1	#0011
      0xB8060800,  //  000A  GETNGBL	R1	K4
      0x8C040305,  //  000B  GETMET	R1	R1	K5
      0x880C0103,  //  000C  GETMBR	R3	R0	K3
      0x7C040400,  //  000D  CALL	R1	2
      0x78060001,  //  000E  JMPF	R1	#0011
      0x4C040000,  //  000F  LDNIL	R1
      0x90020601,  //  0010  SETMBR	R0	K3	R1
      0x58040006,  //  0011  LDCONST	R1	K6
      0x6008000C,  //  0012  GETGBL	R2	G12
      0x880C0107,  //  0013  GETMBR	R3	R0	K7
      0x7C080200,  //  0014  CALL	R2	1
      0x14080202,  //  0015  LT	R2	R1	R2
      0x780A0005,  //  0016  JMPF	R2	#001D
      0x88080107,  //  0017  GETMBR	R2	R0	K7
      0x94080401,  //  0018  GETIDX	R2	R2	R1
      0x8C080501,  //  0019  GETMET	R2	R2	K1
      0x7C080200,  //  001A  CALL	R2	1
      0x00040308,  //  001B  ADD	R1	R1	K8
      0x7001FFF4,  //  001C  JMP		#0012
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_op_discovery_all_fabrics
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_op_discovery_all_fabrics,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(active_fabrics),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(get_fabric_id),
    /* K4   */  be_nested_str_weak(mdns_remove_op_discovery),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(mdns_remove_op_discovery_all_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000B,  //  0005  EXBLK	0	#0012
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x780E0005,  //  000A  JMPF	R3	#0011
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x8C0C0104,  //  000E  GETMET	R3	R0	K4
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x7001FFF3,  //  0011  JMP		#0006
      0x58040005,  //  0012  LDCONST	R1	K5
      0xAC040200,  //  0013  CATCH	R1	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(reverse),
    /* K5   */  be_nested_str_weak(get_fabric_compressed),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_nested_str_weak(_X2D),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20Operational_X20Discovery_X20node_X20_X3D_X20),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(eth),
    /* K13  */  be_nested_str_weak(find),
    /* K14  */  be_nested_str_weak(up),
    /* K15  */  be_nested_str_weak(format),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20adding_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K17  */  be_nested_str_weak(hostname_eth),
    /* K18  */  be_const_int(3),
    /* K19  */  be_nested_str_weak(add_service),
    /* K20  */  be_nested_str_weak(_matter),
    /* K21  */  be_nested_str_weak(_tcp),
    /* K22  */  be_nested_str_weak(_I),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K24  */  be_nested_str_weak(add_subtype),
    /* K25  */  be_nested_str_weak(wifi),
    /* K26  */  be_nested_str_weak(hostname_wifi),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K28  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_announce_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[122]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA8020064,  //  0002  EXBLK	0	#0068
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140305,  //  0009  GETMET	R5	R1	K5
      0x7C140200,  //  000A  CALL	R5	1
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x7C180200,  //  000C  CALL	R6	1
      0x00180D07,  //  000D  ADD	R6	R6	K7
      0x8C1C0906,  //  000E  GETMET	R7	R4	K6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x00180C07,  //  0010  ADD	R6	R6	R7
      0xB81E1000,  //  0011  GETNGBL	R7	K8
      0x8C1C0F09,  //  0012  GETMET	R7	R7	K9
      0x00261406,  //  0013  ADD	R9	K10	R6
      0x5828000B,  //  0014  LDCONST	R10	K11
      0x7C1C0600,  //  0015  CALL	R7	3
      0xB81E1000,  //  0016  GETNGBL	R7	K8
      0x8C1C0F0C,  //  0017  GETMET	R7	R7	K12
      0x7C1C0200,  //  0018  CALL	R7	1
      0x8C1C0F0D,  //  0019  GETMET	R7	R7	K13
      0x5824000E,  //  001A  LDCONST	R9	K14
      0x7C1C0400,  //  001B  CALL	R7	2
      0x781E0020,  //  001C  JMPF	R7	#003E
      0xB81E1000,  //  001D  GETNGBL	R7	K8
      0x8C1C0F09,  //  001E  GETMET	R7	R7	K9
      0x8C24070F,  //  001F  GETMET	R9	R3	K15
      0x582C0010,  //  0020  LDCONST	R11	K16
      0x5830000C,  //  0021  LDCONST	R12	K12
      0x5C340C00,  //  0022  MOVE	R13	R6
      0x88380111,  //  0023  GETMBR	R14	R0	K17
      0x7C240A00,  //  0024  CALL	R9	5
      0x58280012,  //  0025  LDCONST	R10	K18
      0x7C1C0600,  //  0026  CALL	R7	3
      0x8C1C0513,  //  0027  GETMET	R7	R2	K19
      0x58240014,  //  0028  LDCONST	R9	K20
      0x58280015,  //  0029  LDCONST	R10	K21
      0x542E15A3,  //  002A  LDINT	R11	5540
      0x4C300000,  //  002B  LDNIL	R12
      0x5C340C00,  //  002C  MOVE	R13	R6
      0x88380111,  //  002D  GETMBR	R14	R0	K17
      0x7C1C0E00,  //  002E  CALL	R7	7
      0x8C1C0B06,  //  002F  GETMET	R7	R5	K6
      0x7C1C0200,  //  0030  CALL	R7	1
      0x001E2C07,  //  0031  ADD	R7	K22	R7
      0xB8221000,  //  0032  GETNGBL	R8	K8
      0x8C201109,  //  0033  GETMET	R8	R8	K9
      0x002A2E07,  //  0034  ADD	R10	K23	R7
      0x582C0012,  //  0035  LDCONST	R11	K18
      0x7C200600,  //  0036  CALL	R8	3
      0x8C200518,  //  0037  GETMET	R8	R2	K24
      0x58280014,  //  0038  LDCONST	R10	K20
      0x582C0015,  //  0039  LDCONST	R11	K21
      0x5C300C00,  //  003A  MOVE	R12	R6
      0x88340111,  //  003B  GETMBR	R13	R0	K17
      0x5C380E00,  //  003C  MOVE	R14	R7
      0x7C200C00,  //  003D  CALL	R8	6
      0xB81E1000,  //  003E  GETNGBL	R7	K8
      0x8C1C0F19,  //  003F  GETMET	R7	R7	K25
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C1C0F0D,  //  0041  GETMET	R7	R7	K13
      0x5824000E,  //  0042  LDCONST	R9	K14
      0x7C1C0400,  //  0043  CALL	R7	2
      0x781E0020,  //  0044  JMPF	R7	#0066
      0xB81E1000,  //  0045  GETNGBL	R7	K8
      0x8C1C0F09,  //  0046  GETMET	R7	R7	K9
      0x8C24070F,  //  0047  GETMET	R9	R3	K15
      0x582C0010,  //  0048  LDCONST	R11	K16
      0x58300019,  //  0049  LDCONST	R12	K25
      0x5C340C00,  //  004A  MOVE	R13	R6
      0x8838011A,  //  004B  GETMBR	R14	R0	K26
      0x7C240A00,  //  004C  CALL	R9	5
      0x58280012,  //  004D  LDCONST	R10	K18
      0x7C1C0600,  //  004E  CALL	R7	3
      0x8C1C0513,  //  004F  GETMET	R7	R2	K19
      0x58240014,  //  0050  LDCONST	R9	K20
      0x58280015,  //  0051  LDCONST	R10	K21
      0x542E15A3,  //  0052  LDINT	R11	5540
      0x4C300000,  //  0053  LDNIL	R12
      0x5C340C00,  //  0054  MOVE	R13	R6
      0x8838011A,  //  0055  GETMBR	R14	R0	K26
      0x7C1C0E00,  //  0056  CALL	R7	7
      0x8C1C0B06,  //  0057  GETMET	R7	R5	K6
      0x7C1C0200,  //  0058  CALL	R7	1
      0x001E2C07,  //  0059  ADD	R7	K22	R7
      0xB8221000,  //  005A  GETNGBL	R8	K8
      0x8C201109,  //  005B  GETMET	R8	R8	K9
      0x002A2E07,  //  005C  ADD	R10	K23	R7
      0x582C0012,  //  005D  LDCONST	R11	K18
      0x7C200600,  //  005E  CALL	R8	3
      0x8C200518,  //  005F  GETMET	R8	R2	K24
      0x58280014,  //  0060  LDCONST	R10	K20
      0x582C0015,  //  0061  LDCONST	R11	K21
      0x5C300C00,  //  0062  MOVE	R12	R6
      0x8834011A,  //  0063  GETMBR	R13	R0	K26
      0x5C380E00,  //  0064  MOVE	R14	R7
      0x7C200C00,  //  0065  CALL	R8	6
      0xA8040001,  //  0066  EXBLK	1	1
      0x70020010,  //  0067  JMP		#0079
      0xAC100002,  //  0068  CATCH	R4	0	2
      0x7002000D,  //  0069  JMP		#0078
      0xB81A1000,  //  006A  GETNGBL	R6	K8
      0x8C180D09,  //  006B  GETMET	R6	R6	K9
      0x60200008,  //  006C  GETGBL	R8	G8
      0x5C240800,  //  006D  MOVE	R9	R4
      0x7C200200,  //  006E  CALL	R8	1
      0x00223608,  //  006F  ADD	R8	K27	R8
      0x0020111C,  //  0070  ADD	R8	R8	K28
      0x60240008,  //  0071  GETGBL	R9	G8
      0x5C280A00,  //  0072  MOVE	R10	R5
      0x7C240200,  //  0073  CALL	R9	1
      0x00201009,  //  0074  ADD	R8	R8	R9
      0x5824000B,  //  0075  LDCONST	R9	K11
      0x7C180600,  //  0076  CALL	R6	3
      0x70020000,  //  0077  JMP		#0079
      0xB0080000,  //  0078  RAISE	2	R0	R0
      0x80000000,  //  0079  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete_deferred
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_commissioning_complete),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_timer),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(start_commissioning_complete_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Device
********************************************************************/
be_local_class(Matter_Device,
    28,
    NULL,
    be_nested_map(76,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(PRODUCT_ID, -1), be_const_int(32768) },
        { be_const_key_weak(start_operational_discovery, 57), be_const_closure(Matter_Device_start_operational_discovery_closure) },
        { be_const_key_weak(root_L, 14), be_const_var(27) },
        { be_const_key_weak(commissioning_discriminator, 10), be_const_var(8) },
        { be_const_key_weak(FILENAME, 41), be_nested_str_weak(_matter_device_X2Ejson) },
        { be_const_key_weak(_init_basic_commissioning, -1), be_const_closure(Matter_Device__init_basic_commissioning_closure) },
        { be_const_key_weak(compute_manual_pairing_code, -1), be_const_closure(Matter_Device_compute_manual_pairing_code_closure) },
        { be_const_key_weak(_trigger_read_sensors, -1), be_const_closure(Matter_Device__trigger_read_sensors_closure) },
        { be_const_key_weak(hostname_eth, -1), be_const_var(16) },
        { be_const_key_weak(productid, 44), be_const_var(18) },
        { be_const_key_weak(start, -1), be_const_closure(Matter_Device_start_closure) },
        { be_const_key_weak(root_salt, -1), be_const_var(25) },
        { be_const_key_weak(mdns_announce_op_discovery, 65), be_const_closure(Matter_Device_mdns_announce_op_discovery_closure) },
        { be_const_key_weak(start_operational_discovery_deferred, -1), be_const_closure(Matter_Device_start_operational_discovery_deferred_closure) },
        { be_const_key_weak(mdns_remove_op_discovery_all_fabrics, -1), be_const_closure(Matter_Device_mdns_remove_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(UDP_PORT, -1), be_const_int(5540) },
        { be_const_key_weak(compute_qrcode_content, 71), be_const_closure(Matter_Device_compute_qrcode_content_closure) },
        { be_const_key_weak(msg_send, 42), be_const_closure(Matter_Device_msg_send_closure) },
        { be_const_key_weak(commissioning_instance_eth, -1), be_const_var(14) },
        { be_const_key_weak(PBKDF_ITERATIONS, 16), be_const_int(1000) },
        { be_const_key_weak(received_ack, -1), be_const_closure(Matter_Device_received_ack_closure) },
        { be_const_key_weak(start_root_basic_commissioning, -1), be_const_closure(Matter_Device_start_root_basic_commissioning_closure) },
        { be_const_key_weak(save_before_restart, -1), be_const_closure(Matter_Device_save_before_restart_closure) },
        { be_const_key_weak(plugins, 40), be_const_var(1) },
        { be_const_key_weak(commissioning_admin_fabric, -1), be_const_var(12) },
        { be_const_key_weak(commissioning_iterations, 17), be_const_var(7) },
        { be_const_key_weak(root_passcode, 63), be_const_var(22) },
        { be_const_key_weak(mdns_pase_eth, 36), be_const_var(19) },
        { be_const_key_weak(autoconf_device, 33), be_const_closure(Matter_Device_autoconf_device_closure) },
        { be_const_key_weak(mdns_announce_PASE, -1), be_const_closure(Matter_Device_mdns_announce_PASE_closure) },
        { be_const_key_weak(get_active_endpoints, 29), be_const_closure(Matter_Device_get_active_endpoints_closure) },
        { be_const_key_weak(attribute_updated, -1), be_const_closure(Matter_Device_attribute_updated_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_Device_every_250ms_closure) },
        { be_const_key_weak(mdns_announce_op_discovery_all_fabrics, -1), be_const_closure(Matter_Device_mdns_announce_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(save_param, 12), be_const_closure(Matter_Device_save_param_closure) },
        { be_const_key_weak(process_attribute_expansion, 30), be_const_closure(Matter_Device_process_attribute_expansion_closure) },
        { be_const_key_weak(commissioning_L, -1), be_const_var(11) },
        { be_const_key_weak(root_w0, -1), be_const_var(26) },
        { be_const_key_weak(started, 64), be_const_var(0) },
        { be_const_key_weak(message_handler, -1), be_const_var(3) },
        { be_const_key_weak(PASSCODE_DEFAULT, -1), be_const_int(20202021) },
        { be_const_key_weak(stop_basic_commissioning, -1), be_const_closure(Matter_Device_stop_basic_commissioning_closure) },
        { be_const_key_weak(_compute_pbkdf, -1), be_const_closure(Matter_Device__compute_pbkdf_closure) },
        { be_const_key_weak(is_root_commissioning_open, -1), be_const_closure(Matter_Device_is_root_commissioning_open_closure) },
        { be_const_key_weak(VENDOR_ID, -1), be_const_int(65521) },
        { be_const_key_weak(commissioning_open, -1), be_const_var(6) },
        { be_const_key_weak(commissioning_instance_wifi, 60), be_const_var(13) },
        { be_const_key_weak(mdns_remove_op_discovery, 62), be_const_closure(Matter_Device_mdns_remove_op_discovery_closure) },
        { be_const_key_weak(sessions, -1), be_const_var(4) },
        { be_const_key_weak(stop, -1), be_const_closure(Matter_Device_stop_closure) },
        { be_const_key_weak(root_discriminator, -1), be_const_var(21) },
        { be_const_key_weak(init, 55), be_const_closure(Matter_Device_init_closure) },
        { be_const_key_weak(msg_received, -1), be_const_closure(Matter_Device_msg_received_closure) },
        { be_const_key_weak(commissioning_salt, -1), be_const_var(9) },
        { be_const_key_weak(sort_distinct, -1), be_const_static_closure(Matter_Device_sort_distinct_closure) },
        { be_const_key_weak(ui, 47), be_const_var(5) },
        { be_const_key_weak(hostname_wifi, -1), be_const_var(15) },
        { be_const_key_weak(start_mdns_announce_hostnames, 13), be_const_closure(Matter_Device_start_mdns_announce_hostnames_closure) },
        { be_const_key_weak(_mdns_announce_hostname, -1), be_const_closure(Matter_Device__mdns_announce_hostname_closure) },
        { be_const_key_weak(load_param, -1), be_const_closure(Matter_Device_load_param_closure) },
        { be_const_key_weak(vendorid, 73), be_const_var(17) },
        { be_const_key_weak(mdns_pase_wifi, -1), be_const_var(20) },
        { be_const_key_weak(start_commissioning_complete, -1), be_const_closure(Matter_Device_start_commissioning_complete_closure) },
        { be_const_key_weak(commissioning_w0, 68), be_const_var(10) },
        { be_const_key_weak(start_basic_commissioning, -1), be_const_closure(Matter_Device_start_basic_commissioning_closure) },
        { be_const_key_weak(PASE_TIMEOUT, -1), be_const_int(600) },
        { be_const_key_weak(udp_server, -1), be_const_var(2) },
        { be_const_key_weak(invoke_request, 28), be_const_closure(Matter_Device_invoke_request_closure) },
        { be_const_key_weak(_start_udp, -1), be_const_closure(Matter_Device__start_udp_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Device_every_second_closure) },
        { be_const_key_weak(mdns_remove_PASE, 15), be_const_closure(Matter_Device_mdns_remove_PASE_closure) },
        { be_const_key_weak(remove_fabric, -1), be_const_closure(Matter_Device_remove_fabric_closure) },
        { be_const_key_weak(ipv4only, -1), be_const_var(23) },
        { be_const_key_weak(root_iterations, -1), be_const_var(24) },
        { be_const_key_weak(is_commissioning_open, 11), be_const_closure(Matter_Device_is_commissioning_open_closure) },
        { be_const_key_weak(start_commissioning_complete_deferred, -1), be_const_closure(Matter_Device_start_commissioning_complete_deferred_closure) },
    })),
    be_str_weak(Matter_Device)
);
/*******************************************************************/

void be_load_Matter_Device_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Device);
    be_setglobal(vm, "Matter_Device");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
