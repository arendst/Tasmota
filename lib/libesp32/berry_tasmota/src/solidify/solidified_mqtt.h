/* Solidification of mqtt.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_MQTT;

/********************************************************************
** Solidified function: mqtt_data
********************************************************************/
be_local_closure(MQTT_mqtt_data,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str(topics),
    /* K1   */  be_nested_str(closure),
    /* K2   */  be_nested_str(mqtt_data),
    /* K3   */  be_nested_str(stop_iteration),
    }),
    &be_const_str_mqtt_data,
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x4C180000,  //  0001  LDNIL	R6
      0x1C140A06,  //  0002  EQ	R5	R5	R6
      0x78160000,  //  0003  JMPF	R5	#0005
      0x80000A00,  //  0004  RET	0
      0x50140000,  //  0005  LDBOOL	R5	0	0
      0x60180010,  //  0006  GETGBL	R6	G16
      0x881C0100,  //  0007  GETMBR	R7	R0	K0
      0x7C180200,  //  0008  CALL	R6	1
      0xA8020010,  //  0009  EXBLK	0	#001B
      0x5C1C0C00,  //  000A  MOVE	R7	R6
      0x7C1C0000,  //  000B  CALL	R7	0
      0x88200F01,  //  000C  GETMBR	R8	R7	K1
      0x4C240000,  //  000D  LDNIL	R9
      0x20201009,  //  000E  NE	R8	R8	R9
      0x78220009,  //  000F  JMPF	R8	#001A
      0x8C200F02,  //  0010  GETMET	R8	R7	K2
      0x5C280200,  //  0011  MOVE	R10	R1
      0x5C2C0400,  //  0012  MOVE	R11	R2
      0x5C300600,  //  0013  MOVE	R12	R3
      0x5C340800,  //  0014  MOVE	R13	R4
      0x7C200A00,  //  0015  CALL	R8	5
      0x74160001,  //  0016  JMPT	R5	#0019
      0x74220000,  //  0017  JMPT	R8	#0019
      0x50140001,  //  0018  LDBOOL	R5	0	1
      0x50140200,  //  0019  LDBOOL	R5	1	0
      0x7001FFEE,  //  001A  JMP		#000A
      0x58180003,  //  001B  LDCONST	R6	K3
      0xAC180200,  //  001C  CATCH	R6	1	0
      0xB0080000,  //  001D  RAISE	2	R0	R0
      0x80040A00,  //  001E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: lazy_init
********************************************************************/
be_local_closure(MQTT_lazy_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
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
        /* K0   */  be_nested_str(mqtt_connect),
        }),
        &be_const_str__X3Clambda_X3E,
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str(topics),
    /* K1   */  be_nested_str(tasmota),
    /* K2   */  be_nested_str(add_driver),
    /* K3   */  be_nested_str(add_rule),
    /* K4   */  be_nested_str(Mqtt_X23Connected),
    }),
    &be_const_str_lazy_init,
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806000B,  //  0003  JMPF	R1	#0010
      0x60040012,  //  0004  GETGBL	R1	G18
      0x7C040000,  //  0005  CALL	R1	0
      0x90020001,  //  0006  SETMBR	R0	K0	R1
      0xB8060200,  //  0007  GETNGBL	R1	K1
      0x8C040302,  //  0008  GETMET	R1	R1	K2
      0x5C0C0000,  //  0009  MOVE	R3	R0
      0x7C040400,  //  000A  CALL	R1	2
      0xB8060200,  //  000B  GETNGBL	R1	K1
      0x8C040303,  //  000C  GETMET	R1	R1	K3
      0x580C0004,  //  000D  LDCONST	R3	K4
      0x84100000,  //  000E  CLOSURE	R4	P0
      0x7C040600,  //  000F  CALL	R1	3
      0xA0000000,  //  0010  CLOSE	R0
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: unsubscribe
********************************************************************/
be_local_closure(MQTT_unsubscribe,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(topics),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str(fulltopic),
    /* K3   */  be_nested_str(_unsubscribe),
    /* K4   */  be_nested_str(remove),
    /* K5   */  be_const_int(1),
    }),
    &be_const_str_unsubscribe,
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x780A0000,  //  0003  JMPF	R2	#0005
      0x80000400,  //  0004  RET	0
      0x58080001,  //  0005  LDCONST	R2	K1
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x88100100,  //  0007  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0008  CALL	R3	1
      0x140C0403,  //  0009  LT	R3	R2	R3
      0x780E0016,  //  000A  JMPF	R3	#0022
      0x4C0C0000,  //  000B  LDNIL	R3
      0x1C0C0203,  //  000C  EQ	R3	R1	R3
      0x740E0004,  //  000D  JMPT	R3	#0013
      0x880C0100,  //  000E  GETMBR	R3	R0	K0
      0x940C0602,  //  000F  GETIDX	R3	R3	R2
      0x880C0702,  //  0010  GETMBR	R3	R3	K2
      0x1C0C0601,  //  0011  EQ	R3	R3	R1
      0x780E000C,  //  0012  JMPF	R3	#0020
      0x4C0C0000,  //  0013  LDNIL	R3
      0x1C0C0203,  //  0014  EQ	R3	R1	R3
      0x780E0004,  //  0015  JMPF	R3	#001B
      0x8C0C0103,  //  0016  GETMET	R3	R0	K3
      0x88140100,  //  0017  GETMBR	R5	R0	K0
      0x94140A02,  //  0018  GETIDX	R5	R5	R2
      0x88140B02,  //  0019  GETMBR	R5	R5	K2
      0x7C0C0400,  //  001A  CALL	R3	2
      0x880C0100,  //  001B  GETMBR	R3	R0	K0
      0x8C0C0704,  //  001C  GETMET	R3	R3	K4
      0x5C140400,  //  001D  MOVE	R5	R2
      0x7C0C0400,  //  001E  CALL	R3	2
      0x70020000,  //  001F  JMP		#0021
      0x00080505,  //  0020  ADD	R2	R2	K5
      0x7001FFE3,  //  0021  JMP		#0006
      0x4C0C0000,  //  0022  LDNIL	R3
      0x200C0203,  //  0023  NE	R3	R1	R3
      0x780E0002,  //  0024  JMPF	R3	#0028
      0x8C0C0103,  //  0025  GETMET	R3	R0	K3
      0x5C140200,  //  0026  MOVE	R5	R1
      0x7C0C0400,  //  0027  CALL	R3	2
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mqtt_connect
********************************************************************/
be_local_closure(MQTT_mqtt_connect,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str(tasmota),
    /* K1   */  be_nested_str(log),
    /* K2   */  be_nested_str(BRY_X3A_X20mqtt_X20subscribe_X20all_X20registered_X20topics),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str(topics),
    /* K5   */  be_nested_str(fulltopic),
    /* K6   */  be_nested_str(_subscribe),
    /* K7   */  be_nested_str(stop_iteration),
    }),
    &be_const_str_mqtt_connect,
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x58100003,  //  0003  LDCONST	R4	K3
      0x7C040600,  //  0004  CALL	R1	3
      0x60040010,  //  0005  GETGBL	R1	G16
      0x88080104,  //  0006  GETMBR	R2	R0	K4
      0x7C040200,  //  0007  CALL	R1	1
      0xA8020006,  //  0008  EXBLK	0	#0010
      0x5C080200,  //  0009  MOVE	R2	R1
      0x7C080000,  //  000A  CALL	R2	0
      0x880C0505,  //  000B  GETMBR	R3	R2	K5
      0x8C100106,  //  000C  GETMET	R4	R0	K6
      0x5C180600,  //  000D  MOVE	R6	R3
      0x7C100400,  //  000E  CALL	R4	2
      0x7001FFF8,  //  000F  JMP		#0009
      0x58040007,  //  0010  LDCONST	R1	K7
      0xAC040200,  //  0011  CATCH	R1	1	0
      0xB0080000,  //  0012  RAISE	2	R0	R0
      0x50040000,  //  0013  LDBOOL	R1	0	0
      0x80040200,  //  0014  RET	1	R1
    })
  )
);
/*******************************************************************/


extern const bclass be_class_mqtt_listener;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(mqtt_listener_tostring,   /* name */
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
    /* K0   */  be_nested_str(_X3Cinstance_X3A_X20_X25s_X28_X27_X25s_X27_X29_X3E),
    /* K1   */  be_nested_str(fulltopic),
    }),
    &be_const_str_tostring,
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60040018,  //  0000  GETGBL	R1	G24
      0x58080000,  //  0001  LDCONST	R2	K0
      0x600C0005,  //  0002  GETGBL	R3	G5
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x88100101,  //  0005  GETMBR	R4	R0	K1
      0x7C040600,  //  0006  CALL	R1	3
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(mqtt_listener_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(fulltopic),
    /* K2   */  be_nested_str(topic),
    /* K3   */  be_nested_str(split),
    /* K4   */  be_nested_str(_X2F),
    /* K5   */  be_nested_str(closure),
    }),
    &be_const_str_init,
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x8C100703,  //  0002  GETMET	R4	R3	K3
      0x5C180200,  //  0003  MOVE	R6	R1
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x7C100600,  //  0005  CALL	R4	3
      0x90020404,  //  0006  SETMBR	R0	K2	R4
      0x90020A02,  //  0007  SETMBR	R0	K5	R2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mqtt_data
********************************************************************/
be_local_closure(mqtt_listener_mqtt_data,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str(string),
    /* K1   */  be_nested_str(split),
    /* K2   */  be_nested_str(_X2F),
    /* K3   */  be_nested_str(topic),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str(_X23),
    /* K6   */  be_nested_str(_X2B),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str(closure),
    }),
    &be_const_str_mqtt_data,
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0x8C180B01,  //  0001  GETMET	R6	R5	K1
      0x5C200200,  //  0002  MOVE	R8	R1
      0x58240002,  //  0003  LDCONST	R9	K2
      0x7C180600,  //  0004  CALL	R6	3
      0x601C000C,  //  0005  GETGBL	R7	G12
      0x5C200C00,  //  0006  MOVE	R8	R6
      0x7C1C0200,  //  0007  CALL	R7	1
      0x88200103,  //  0008  GETMBR	R8	R0	K3
      0x6024000C,  //  0009  GETGBL	R9	G12
      0x5C281000,  //  000A  MOVE	R10	R8
      0x7C240200,  //  000B  CALL	R9	1
      0x58280004,  //  000C  LDCONST	R10	K4
      0x142C1409,  //  000D  LT	R11	R10	R9
      0x782E0013,  //  000E  JMPF	R11	#0023
      0x942C100A,  //  000F  GETIDX	R11	R8	R10
      0x1C301705,  //  0010  EQ	R12	R11	K5
      0x78320001,  //  0011  JMPF	R12	#0014
      0x7002000F,  //  0012  JMP		#0023
      0x7002000C,  //  0013  JMP		#0021
      0x28301407,  //  0014  GE	R12	R10	R7
      0x78320002,  //  0015  JMPF	R12	#0019
      0x50300000,  //  0016  LDBOOL	R12	0	0
      0x80041800,  //  0017  RET	1	R12
      0x70020007,  //  0018  JMP		#0021
      0x1C301706,  //  0019  EQ	R12	R11	K6
      0x78320000,  //  001A  JMPF	R12	#001C
      0x70020004,  //  001B  JMP		#0021
      0x94300C0A,  //  001C  GETIDX	R12	R6	R10
      0x2030160C,  //  001D  NE	R12	R11	R12
      0x78320001,  //  001E  JMPF	R12	#0021
      0x50300000,  //  001F  LDBOOL	R12	0	0
      0x80041800,  //  0020  RET	1	R12
      0x00281507,  //  0021  ADD	R10	R10	K7
      0x7001FFE9,  //  0022  JMP		#000D
      0x282C1409,  //  0023  GE	R11	R10	R9
      0x782E0003,  //  0024  JMPF	R11	#0029
      0x202C1207,  //  0025  NE	R11	R9	R7
      0x782E0001,  //  0026  JMPF	R11	#0029
      0x502C0000,  //  0027  LDBOOL	R11	0	0
      0x80041600,  //  0028  RET	1	R11
      0x882C0108,  //  0029  GETMBR	R11	R0	K8
      0x5C301600,  //  002A  MOVE	R12	R11
      0x5C340200,  //  002B  MOVE	R13	R1
      0x5C380400,  //  002C  MOVE	R14	R2
      0x5C3C0600,  //  002D  MOVE	R15	R3
      0x5C400800,  //  002E  MOVE	R16	R4
      0x7C300800,  //  002F  CALL	R12	4
      0x4C340000,  //  0030  LDNIL	R13
      0x1C34180D,  //  0031  EQ	R13	R12	R13
      0x78360000,  //  0032  JMPF	R13	#0034
      0x50300200,  //  0033  LDBOOL	R12	1	0
      0x80041800,  //  0034  RET	1	R12
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: mqtt_listener
********************************************************************/
be_local_class(mqtt_listener,
    3,
    NULL,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(mqtt_data, -1), be_const_closure(mqtt_listener_mqtt_data_closure) },
        { be_const_key(tostring, -1), be_const_closure(mqtt_listener_tostring_closure) },
        { be_const_key(topic, -1), be_const_var(0) },
        { be_const_key(init, -1), be_const_closure(mqtt_listener_init_closure) },
        { be_const_key(closure, 0), be_const_var(2) },
        { be_const_key(fulltopic, -1), be_const_var(1) },
    })),
    (bstring*) &be_const_str_mqtt_listener
);

/********************************************************************
** Solidified function: mqtt_listener_class
********************************************************************/
be_local_closure(MQTT_mqtt_listener_class,   /* name */
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
    /* K0   */  be_const_class(be_class_mqtt_listener),
    }),
    &be_const_str_mqtt_listener_class,
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xB4000000,  //  0001  CLASS	K0
      0x5C080200,  //  0002  MOVE	R2	R1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe
********************************************************************/
be_local_closure(MQTT_subscribe,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str(lazy_init),
    /* K1   */  be_nested_str(topics),
    /* K2   */  be_nested_str(fulltopic),
    /* K3   */  be_nested_str(closure),
    /* K4   */  be_nested_str(stop_iteration),
    /* K5   */  be_nested_str(mqtt_listener_class),
    /* K6   */  be_nested_str(function),
    /* K7   */  be_nested_str(tasmota),
    /* K8   */  be_nested_str(check_not_method),
    /* K9   */  be_nested_str(push),
    /* K10  */  be_nested_str(_subscribe),
    }),
    &be_const_str_subscribe,
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x8C0C0100,  //  0000  GETMET	R3	R0	K0
      0x7C0C0200,  //  0001  CALL	R3	1
      0x500C0000,  //  0002  LDBOOL	R3	0	0
      0x60100010,  //  0003  GETGBL	R4	G16
      0x88140101,  //  0004  GETMBR	R5	R0	K1
      0x7C100200,  //  0005  CALL	R4	1
      0xA802000A,  //  0006  EXBLK	0	#0012
      0x5C140800,  //  0007  MOVE	R5	R4
      0x7C140000,  //  0008  CALL	R5	0
      0x88180B02,  //  0009  GETMBR	R6	R5	K2
      0x1C180C01,  //  000A  EQ	R6	R6	R1
      0x781A0004,  //  000B  JMPF	R6	#0011
      0x88180B03,  //  000C  GETMBR	R6	R5	K3
      0x1C180C02,  //  000D  EQ	R6	R6	R2
      0x781A0001,  //  000E  JMPF	R6	#0011
      0xA8040001,  //  000F  EXBLK	1	1
      0x80000C00,  //  0010  RET	0
      0x7001FFF4,  //  0011  JMP		#0007
      0x58100004,  //  0012  LDCONST	R4	K4
      0xAC100200,  //  0013  CATCH	R4	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x8C100105,  //  0015  GETMET	R4	R0	K5
      0x7C100200,  //  0016  CALL	R4	1
      0x60140004,  //  0017  GETGBL	R5	G4
      0x5C180400,  //  0018  MOVE	R6	R2
      0x7C140200,  //  0019  CALL	R5	1
      0x1C140B06,  //  001A  EQ	R5	R5	K6
      0x7816000B,  //  001B  JMPF	R5	#0028
      0xB8160E00,  //  001C  GETNGBL	R5	K7
      0x8C140B08,  //  001D  GETMET	R5	R5	K8
      0x5C1C0400,  //  001E  MOVE	R7	R2
      0x7C140400,  //  001F  CALL	R5	2
      0x88140101,  //  0020  GETMBR	R5	R0	K1
      0x8C140B09,  //  0021  GETMET	R5	R5	K9
      0x5C1C0800,  //  0022  MOVE	R7	R4
      0x5C200200,  //  0023  MOVE	R8	R1
      0x5C240400,  //  0024  MOVE	R9	R2
      0x7C1C0400,  //  0025  CALL	R7	2
      0x7C140400,  //  0026  CALL	R5	2
      0x70020005,  //  0027  JMP		#002E
      0x88140101,  //  0028  GETMBR	R5	R0	K1
      0x8C140B09,  //  0029  GETMET	R5	R5	K9
      0x5C1C0800,  //  002A  MOVE	R7	R4
      0x5C200200,  //  002B  MOVE	R8	R1
      0x7C1C0200,  //  002C  CALL	R7	1
      0x7C140400,  //  002D  CALL	R5	2
      0x8C14010A,  //  002E  GETMET	R5	R0	K10
      0x5C1C0200,  //  002F  MOVE	R7	R1
      0x7C140400,  //  0030  CALL	R5	2
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: MQTT
********************************************************************/
extern const bclass be_class_MQTT_ntv;
be_local_class(MQTT,
    1,
    &be_class_MQTT_ntv,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key(mqtt_connect, -1), be_const_closure(MQTT_mqtt_connect_closure) },
        { be_const_key(mqtt_data, -1), be_const_closure(MQTT_mqtt_data_closure) },
        { be_const_key(lazy_init, -1), be_const_closure(MQTT_lazy_init_closure) },
        { be_const_key(unsubscribe, -1), be_const_closure(MQTT_unsubscribe_closure) },
        { be_const_key(topics, 0), be_const_var(0) },
        { be_const_key(mqtt_listener_class, -1), be_const_closure(MQTT_mqtt_listener_class_closure) },
        { be_const_key(subscribe, -1), be_const_closure(MQTT_subscribe_closure) },
    })),
    (bstring*) &be_const_str_MQTT
);
/*******************************************************************/

void be_load_MQTT_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_MQTT);
    be_setglobal(vm, "MQTT");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
