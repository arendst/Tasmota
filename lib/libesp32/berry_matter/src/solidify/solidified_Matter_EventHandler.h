/* Solidification of Matter_EventHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_EventHandler;

/********************************************************************
** Solidified function: dump
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_dump,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventHandler, 
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20Events_X20queues_X20sizes_X3A_X20critical_X20_X25s_X2C_X20info_X20_X25s_X2C_X20debug_X20_X25s),
    /* K3   */  be_nested_str_weak(queue_critical),
    /* K4   */  be_nested_str_weak(queue_info),
    /* K5   */  be_nested_str_weak(queue_debug),
    /* K6   */  be_const_int(2),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(priority),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20Events_X20by_X20types_X3A_X20critical_X20_X25s_X2C_X20info_X20_X25s_X2C_X20debug_X20_X25s),
    }),
    be_str_weak(dump),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x600C0018,  //  0002  GETGBL	R3	G24
      0x58100002,  //  0003  LDCONST	R4	K2
      0x6014000C,  //  0004  GETGBL	R5	G12
      0x88180103,  //  0005  GETMBR	R6	R0	K3
      0x7C140200,  //  0006  CALL	R5	1
      0x6018000C,  //  0007  GETGBL	R6	G12
      0x881C0104,  //  0008  GETMBR	R7	R0	K4
      0x7C180200,  //  0009  CALL	R6	1
      0x601C000C,  //  000A  GETGBL	R7	G12
      0x88200105,  //  000B  GETMBR	R8	R0	K5
      0x7C1C0200,  //  000C  CALL	R7	1
      0x7C0C0800,  //  000D  CALL	R3	4
      0x58100006,  //  000E  LDCONST	R4	K6
      0x7C040600,  //  000F  CALL	R1	3
      0x60040012,  //  0010  GETGBL	R1	G18
      0x7C040000,  //  0011  CALL	R1	0
      0x40080307,  //  0012  CONNECT	R2	R1	K7
      0x40080307,  //  0013  CONNECT	R2	R1	K7
      0x40080307,  //  0014  CONNECT	R2	R1	K7
      0x60080010,  //  0015  GETGBL	R2	G16
      0x880C0105,  //  0016  GETMBR	R3	R0	K5
      0x7C080200,  //  0017  CALL	R2	1
      0xA8020006,  //  0018  EXBLK	0	#0020
      0x5C0C0400,  //  0019  MOVE	R3	R2
      0x7C0C0000,  //  001A  CALL	R3	0
      0x88100708,  //  001B  GETMBR	R4	R3	K8
      0x94140204,  //  001C  GETIDX	R5	R1	R4
      0x00140B09,  //  001D  ADD	R5	R5	K9
      0x98040805,  //  001E  SETIDX	R1	R4	R5
      0x7001FFF8,  //  001F  JMP		#0019
      0x5808000A,  //  0020  LDCONST	R2	K10
      0xAC080200,  //  0021  CATCH	R2	1	0
      0xB0080000,  //  0022  RAISE	2	R0	R0
      0x60080010,  //  0023  GETGBL	R2	G16
      0x880C0104,  //  0024  GETMBR	R3	R0	K4
      0x7C080200,  //  0025  CALL	R2	1
      0xA8020006,  //  0026  EXBLK	0	#002E
      0x5C0C0400,  //  0027  MOVE	R3	R2
      0x7C0C0000,  //  0028  CALL	R3	0
      0x88100708,  //  0029  GETMBR	R4	R3	K8
      0x94140204,  //  002A  GETIDX	R5	R1	R4
      0x00140B09,  //  002B  ADD	R5	R5	K9
      0x98040805,  //  002C  SETIDX	R1	R4	R5
      0x7001FFF8,  //  002D  JMP		#0027
      0x5808000A,  //  002E  LDCONST	R2	K10
      0xAC080200,  //  002F  CATCH	R2	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0x60080010,  //  0031  GETGBL	R2	G16
      0x880C0103,  //  0032  GETMBR	R3	R0	K3
      0x7C080200,  //  0033  CALL	R2	1
      0xA8020006,  //  0034  EXBLK	0	#003C
      0x5C0C0400,  //  0035  MOVE	R3	R2
      0x7C0C0000,  //  0036  CALL	R3	0
      0x88100708,  //  0037  GETMBR	R4	R3	K8
      0x94140204,  //  0038  GETIDX	R5	R1	R4
      0x00140B09,  //  0039  ADD	R5	R5	K9
      0x98040805,  //  003A  SETIDX	R1	R4	R5
      0x7001FFF8,  //  003B  JMP		#0035
      0x5808000A,  //  003C  LDCONST	R2	K10
      0xAC080200,  //  003D  CATCH	R2	1	0
      0xB0080000,  //  003E  RAISE	2	R0	R0
      0xB80A0000,  //  003F  GETNGBL	R2	K0
      0x8C080501,  //  0040  GETMET	R2	R2	K1
      0x60100018,  //  0041  GETGBL	R4	G24
      0x5814000B,  //  0042  LDCONST	R5	K11
      0x94180306,  //  0043  GETIDX	R6	R1	K6
      0x941C0309,  //  0044  GETIDX	R7	R1	K9
      0x94200307,  //  0045  GETIDX	R8	R1	K7
      0x7C100800,  //  0046  CALL	R4	4
      0x58140006,  //  0047  LDCONST	R5	K6
      0x7C080600,  //  0048  CALL	R2	3
      0x80000000,  //  0049  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_last_event_no
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_get_last_event_no,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventHandler, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(counter_event_no),
    }),
    be_str_weak(get_last_event_no),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_next_event_no
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_get_next_event_no,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventHandler, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(counter_event_no),
    /* K1   */  be_nested_str_weak(add),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(counter_event_no_persisted),
    /* K4   */  be_nested_str_weak(load_event_no_persisted),
    }),
    be_str_weak(get_next_event_no),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x7C040400,  //  0003  CALL	R1	2
      0x90020001,  //  0004  SETMBR	R0	K0	R1
      0x88040100,  //  0005  GETMBR	R1	R0	K0
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x28040202,  //  0007  GE	R1	R1	R2
      0x78060001,  //  0008  JMPF	R1	#000B
      0x8C040104,  //  0009  GETMET	R1	R0	K4
      0x7C040200,  //  000A  CALL	R1	1
      0x88040100,  //  000B  GETMBR	R1	R0	K0
      0x80040200,  //  000C  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_event_no_persisted
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_load_event_no_persisted,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventHandler, 
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(persist),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(EVENT_NO_KEY),
    /* K3   */  be_nested_str_weak(0),
    /* K4   */  be_nested_str_weak(counter_event_no),
    /* K5   */  be_nested_str_weak(int64),
    /* K6   */  be_nested_str_weak(fromstring),
    /* K7   */  be_nested_str_weak(counter_event_no_persisted),
    /* K8   */  be_nested_str_weak(add),
    /* K9   */  be_nested_str_weak(EVENT_NO_INCR),
    /* K10  */  be_nested_str_weak(setmember),
    /* K11  */  be_nested_str_weak(tostring),
    /* K12  */  be_nested_str_weak(save),
    }),
    be_str_weak(load_event_no_persisted),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080008,  //  0001  GETGBL	R2	G8
      0x8C0C0301,  //  0002  GETMET	R3	R1	K1
      0x88140102,  //  0003  GETMBR	R5	R0	K2
      0x58180003,  //  0004  LDCONST	R6	K3
      0x7C0C0600,  //  0005  CALL	R3	3
      0x7C080200,  //  0006  CALL	R2	1
      0xB80E0A00,  //  0007  GETNGBL	R3	K5
      0x8C0C0706,  //  0008  GETMET	R3	R3	K6
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x90020803,  //  000B  SETMBR	R0	K4	R3
      0x880C0104,  //  000C  GETMBR	R3	R0	K4
      0x8C0C0708,  //  000D  GETMET	R3	R3	K8
      0x88140109,  //  000E  GETMBR	R5	R0	K9
      0x7C0C0400,  //  000F  CALL	R3	2
      0x90020E03,  //  0010  SETMBR	R0	K7	R3
      0x8C0C030A,  //  0011  GETMET	R3	R1	K10
      0x88140102,  //  0012  GETMBR	R5	R0	K2
      0x88180107,  //  0013  GETMBR	R6	R0	K7
      0x8C180D0B,  //  0014  GETMET	R6	R6	K11
      0x7C180200,  //  0015  CALL	R6	1
      0x7C0C0600,  //  0016  CALL	R3	3
      0x8C0C030C,  //  0017  GETMET	R3	R1	K12
      0x7C0C0200,  //  0018  CALL	R3	1
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: queue_event
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_queue_event,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventHandler, 
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(priority),
    /* K1   */  be_nested_str_weak(queue_debug),
    /* K2   */  be_nested_str_weak(push),
    /* K3   */  be_nested_str_weak(EVENT_QUEUE_SIZE_MAX),
    /* K4   */  be_nested_str_weak(pop),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(EVENT_DEBUG),
    /* K8   */  be_nested_str_weak(queue_info),
    /* K9   */  be_nested_str_weak(EVENT_INFO),
    /* K10  */  be_nested_str_weak(queue_critical),
    }),
    be_str_weak(queue_event),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x88100103,  //  0008  GETMBR	R4	R0	K3
      0x240C0604,  //  0009  GT	R3	R3	R4
      0x780E0029,  //  000A  JMPF	R3	#0035
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x8C0C0704,  //  000C  GETMET	R3	R3	K4
      0x58140005,  //  000D  LDCONST	R5	K5
      0x7C0C0400,  //  000E  CALL	R3	2
      0x88100700,  //  000F  GETMBR	R4	R3	K0
      0xB8160C00,  //  0010  GETNGBL	R5	K6
      0x88140B07,  //  0011  GETMBR	R5	R5	K7
      0x24100805,  //  0012  GT	R4	R4	R5
      0x78120020,  //  0013  JMPF	R4	#0035
      0x88100108,  //  0014  GETMBR	R4	R0	K8
      0x8C100902,  //  0015  GETMET	R4	R4	K2
      0x5C180600,  //  0016  MOVE	R6	R3
      0x7C100400,  //  0017  CALL	R4	2
      0x6010000C,  //  0018  GETGBL	R4	G12
      0x88140108,  //  0019  GETMBR	R5	R0	K8
      0x7C100200,  //  001A  CALL	R4	1
      0x88140103,  //  001B  GETMBR	R5	R0	K3
      0x24100805,  //  001C  GT	R4	R4	R5
      0x78120016,  //  001D  JMPF	R4	#0035
      0x88100108,  //  001E  GETMBR	R4	R0	K8
      0x8C100904,  //  001F  GETMET	R4	R4	K4
      0x58180005,  //  0020  LDCONST	R6	K5
      0x7C100400,  //  0021  CALL	R4	2
      0x88140900,  //  0022  GETMBR	R5	R4	K0
      0xB81A0C00,  //  0023  GETNGBL	R6	K6
      0x88180D09,  //  0024  GETMBR	R6	R6	K9
      0x24140A06,  //  0025  GT	R5	R5	R6
      0x7816000D,  //  0026  JMPF	R5	#0035
      0x8814010A,  //  0027  GETMBR	R5	R0	K10
      0x8C140B02,  //  0028  GETMET	R5	R5	K2
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x7C140400,  //  002A  CALL	R5	2
      0x6014000C,  //  002B  GETGBL	R5	G12
      0x8818010A,  //  002C  GETMBR	R6	R0	K10
      0x7C140200,  //  002D  CALL	R5	1
      0x88180103,  //  002E  GETMBR	R6	R0	K3
      0x24140A06,  //  002F  GT	R5	R5	R6
      0x78160003,  //  0030  JMPF	R5	#0035
      0x8814010A,  //  0031  GETMBR	R5	R0	K10
      0x8C140B04,  //  0032  GETMET	R5	R5	K4
      0x581C0005,  //  0033  LDCONST	R7	K5
      0x7C140400,  //  0034  CALL	R5	2
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventHandler, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(compact),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventHandler, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(queue_debug),
    /* K2   */  be_nested_str_weak(queue_info),
    /* K3   */  be_nested_str_weak(queue_critical),
    /* K4   */  be_nested_str_weak(load_event_no_persisted),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0x60080012,  //  0004  GETGBL	R2	G18
      0x7C080000,  //  0005  CALL	R2	0
      0x90020402,  //  0006  SETMBR	R0	K2	R2
      0x60080012,  //  0007  GETGBL	R2	G18
      0x7C080000,  //  0008  CALL	R2	0
      0x90020602,  //  0009  SETMBR	R0	K3	R2
      0x8C080104,  //  000A  GETMET	R2	R0	K4
      0x7C080200,  //  000B  CALL	R2	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_min_no
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_find_min_no,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        7,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL, 
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_const_int(1),
        /* K1   */  be_const_int(0),
        /* K2   */  be_nested_str_weak(event_no),
        }),
        be_str_weak(find_in_queue),
        &be_const_str_solidified,
        ( &(const binstruction[25]) {  /* code */
          0x600C000C,  //  0000  GETGBL	R3	G12
          0x5C100200,  //  0001  MOVE	R4	R1
          0x7C0C0200,  //  0002  CALL	R3	1
          0x040C0700,  //  0003  SUB	R3	R3	K0
          0x28100701,  //  0004  GE	R4	R3	K1
          0x78120011,  //  0005  JMPF	R4	#0018
          0x94100203,  //  0006  GETIDX	R4	R1	R3
          0x4C140000,  //  0007  LDNIL	R5
          0x20140405,  //  0008  NE	R5	R2	R5
          0x78160003,  //  0009  JMPF	R5	#000E
          0x88140902,  //  000A  GETMBR	R5	R4	K2
          0x18140A02,  //  000B  LE	R5	R5	R2
          0x78160000,  //  000C  JMPF	R5	#000E
          0x80040000,  //  000D  RET	1	R0
          0x4C140000,  //  000E  LDNIL	R5
          0x1C140005,  //  000F  EQ	R5	R0	R5
          0x74160003,  //  0010  JMPT	R5	#0015
          0x88140902,  //  0011  GETMBR	R5	R4	K2
          0x88180102,  //  0012  GETMBR	R6	R0	K2
          0x14140A06,  //  0013  LT	R5	R5	R6
          0x78160000,  //  0014  JMPF	R5	#0016
          0x5C000800,  //  0015  MOVE	R0	R4
          0x040C0700,  //  0016  SUB	R3	R3	K0
          0x7001FFEB,  //  0017  JMP		#0004
          0x80040000,  //  0018  RET	1	R0
        })
      ),
    &be_class_Matter_EventHandler, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(counter_event_no),
    /* K1   */  be_nested_str_weak(queue_debug),
    /* K2   */  be_nested_str_weak(queue_info),
    /* K3   */  be_nested_str_weak(queue_critical),
    }),
    be_str_weak(find_min_no),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0004,  //  0002  JMPF	R2	#0008
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x28080202,  //  0004  GE	R2	R1	R2
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x4C080000,  //  0006  LDNIL	R2
      0x80040400,  //  0007  RET	1	R2
      0x84080000,  //  0008  CLOSURE	R2	P0
      0x4C0C0000,  //  0009  LDNIL	R3
      0x5C100400,  //  000A  MOVE	R4	R2
      0x5C140600,  //  000B  MOVE	R5	R3
      0x88180101,  //  000C  GETMBR	R6	R0	K1
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x7C100600,  //  000E  CALL	R4	3
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x5C100400,  //  0010  MOVE	R4	R2
      0x5C140600,  //  0011  MOVE	R5	R3
      0x88180102,  //  0012  GETMBR	R6	R0	K2
      0x5C1C0200,  //  0013  MOVE	R7	R1
      0x7C100600,  //  0014  CALL	R4	3
      0x5C0C0800,  //  0015  MOVE	R3	R4
      0x5C100400,  //  0016  MOVE	R4	R2
      0x5C140600,  //  0017  MOVE	R5	R3
      0x88180103,  //  0018  GETMBR	R6	R0	K3
      0x5C1C0200,  //  0019  MOVE	R7	R1
      0x7C100600,  //  001A  CALL	R4	3
      0x5C0C0800,  //  001B  MOVE	R3	R4
      0x80040600,  //  001C  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: publish_event
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_publish_event,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    9,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventHandler, 
    1,                          /* has constants */
    ( &(const bvalue[31]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(EventQueued),
    /* K2   */  be_nested_str_weak(get_next_event_no),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(loglevel),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(),
    /* K7   */  be_nested_str_weak(_X2C_X20),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(val),
    /* K10  */  be_nested_str_weak(_X25i_X2E_X25i_X2E_X25i_X2E_X25i),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(CRIT_X20_X20),
    /* K13  */  be_const_int(1),
    /* K14  */  be_nested_str_weak(INFO_X20_X20),
    /* K15  */  be_nested_str_weak(DEBUG_X20),
    /* K16  */  be_nested_str_weak(get_event_name),
    /* K17  */  be_nested_str_weak(_X28),
    /* K18  */  be_nested_str_weak(_X29_X20),
    /* K19  */  be_nested_str_weak(log),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20_X2BAdd_Event_X20_X28_X25s_X258s_X29_X20_X5B_X2502X_X5D_X2504X_X2F_X2502X_X20_X25s_X2D_X20_X25s),
    /* K21  */  be_nested_str_weak(event_no),
    /* K22  */  be_nested_str_weak(endpoint),
    /* K23  */  be_nested_str_weak(cluster),
    /* K24  */  be_nested_str_weak(event_id),
    /* K25  */  be_nested_str_weak(queue_event),
    /* K26  */  be_nested_str_weak(device),
    /* K27  */  be_nested_str_weak(message_handler),
    /* K28  */  be_nested_str_weak(im),
    /* K29  */  be_nested_str_weak(subs_shop),
    /* K30  */  be_nested_str_weak(event_published),
    }),
    be_str_weak(publish_event),
    &be_const_str_solidified,
    ( &(const binstruction[112]) {  /* code */
      0xB8260000,  //  0000  GETNGBL	R9	K0
      0x8C241301,  //  0001  GETMET	R9	R9	K1
      0x8C2C0102,  //  0002  GETMET	R11	R0	K2
      0x7C2C0200,  //  0003  CALL	R11	1
      0x5C300200,  //  0004  MOVE	R12	R1
      0x5C340400,  //  0005  MOVE	R13	R2
      0x5C380600,  //  0006  MOVE	R14	R3
      0x5C3C0800,  //  0007  MOVE	R15	R4
      0x5C400A00,  //  0008  MOVE	R16	R5
      0x5C440C00,  //  0009  MOVE	R17	R6
      0x5C480E00,  //  000A  MOVE	R18	R7
      0x5C4C1000,  //  000B  MOVE	R19	R8
      0x7C241400,  //  000C  CALL	R9	10
      0xB82A0600,  //  000D  GETNGBL	R10	K3
      0x8C281504,  //  000E  GETMET	R10	R10	K4
      0x58300005,  //  000F  LDCONST	R12	K5
      0x7C280400,  //  0010  CALL	R10	2
      0x782A0052,  //  0011  JMPF	R10	#0065
      0x58280006,  //  0012  LDCONST	R10	K6
      0x4C2C0000,  //  0013  LDNIL	R11
      0x202C0C0B,  //  0014  NE	R11	R6	R11
      0x782E0003,  //  0015  JMPF	R11	#001A
      0x602C0008,  //  0016  GETGBL	R11	G8
      0x5C300C00,  //  0017  MOVE	R12	R6
      0x7C2C0200,  //  0018  CALL	R11	1
      0x5C281600,  //  0019  MOVE	R10	R11
      0x4C2C0000,  //  001A  LDNIL	R11
      0x202C0E0B,  //  001B  NE	R11	R7	R11
      0x782E0004,  //  001C  JMPF	R11	#0022
      0x602C0008,  //  001D  GETGBL	R11	G8
      0x5C300E00,  //  001E  MOVE	R12	R7
      0x7C2C0200,  //  001F  CALL	R11	1
      0x002E0E0B,  //  0020  ADD	R11	K7	R11
      0x0028140B,  //  0021  ADD	R10	R10	R11
      0x4C2C0000,  //  0022  LDNIL	R11
      0x202C100B,  //  0023  NE	R11	R8	R11
      0x782E0004,  //  0024  JMPF	R11	#002A
      0x602C0008,  //  0025  GETGBL	R11	G8
      0x5C301000,  //  0026  MOVE	R12	R8
      0x7C2C0200,  //  0027  CALL	R11	1
      0x002E0E0B,  //  0028  ADD	R11	K7	R11
      0x0028140B,  //  0029  ADD	R10	R10	R11
      0x542E0027,  //  002A  LDINT	R11	40
      0x1C2C040B,  //  002B  EQ	R11	R2	R11
      0x782E0014,  //  002C  JMPF	R11	#0042
      0x1C2C0708,  //  002D  EQ	R11	R3	K8
      0x782E0012,  //  002E  JMPF	R11	#0042
      0x882C0D09,  //  002F  GETMBR	R11	R6	K9
      0x60300018,  //  0030  GETGBL	R12	G24
      0x5834000A,  //  0031  LDCONST	R13	K10
      0x543A0017,  //  0032  LDINT	R14	24
      0x3C38160E,  //  0033  SHR	R14	R11	R14
      0x543E00FE,  //  0034  LDINT	R15	255
      0x2C381C0F,  //  0035  AND	R14	R14	R15
      0x543E000F,  //  0036  LDINT	R15	16
      0x3C3C160F,  //  0037  SHR	R15	R11	R15
      0x544200FE,  //  0038  LDINT	R16	255
      0x2C3C1E10,  //  0039  AND	R15	R15	R16
      0x54420007,  //  003A  LDINT	R16	8
      0x3C401610,  //  003B  SHR	R16	R11	R16
      0x544600FE,  //  003C  LDINT	R17	255
      0x2C402011,  //  003D  AND	R16	R16	R17
      0x544600FE,  //  003E  LDINT	R17	255
      0x2C441611,  //  003F  AND	R17	R11	R17
      0x7C300A00,  //  0040  CALL	R12	5
      0x5C281800,  //  0041  MOVE	R10	R12
      0x1C2C0B0B,  //  0042  EQ	R11	R5	K11
      0x782E0001,  //  0043  JMPF	R11	#0046
      0x582C000C,  //  0044  LDCONST	R11	K12
      0x70020004,  //  0045  JMP		#004B
      0x1C2C0B0D,  //  0046  EQ	R11	R5	K13
      0x782E0001,  //  0047  JMPF	R11	#004A
      0x582C000E,  //  0048  LDCONST	R11	K14
      0x70020000,  //  0049  JMP		#004B
      0x582C000F,  //  004A  LDCONST	R11	K15
      0xB8320000,  //  004B  GETNGBL	R12	K0
      0x8C301910,  //  004C  GETMET	R12	R12	K16
      0x5C380400,  //  004D  MOVE	R14	R2
      0x5C3C0600,  //  004E  MOVE	R15	R3
      0x7C300600,  //  004F  CALL	R12	3
      0x4C340000,  //  0050  LDNIL	R13
      0x2034180D,  //  0051  NE	R13	R12	R13
      0x78360002,  //  0052  JMPF	R13	#0056
      0x0036220C,  //  0053  ADD	R13	K17	R12
      0x00341B12,  //  0054  ADD	R13	R13	K18
      0x70020000,  //  0055  JMP		#0057
      0x58340006,  //  0056  LDCONST	R13	K6
      0x5C301A00,  //  0057  MOVE	R12	R13
      0xB8362600,  //  0058  GETNGBL	R13	K19
      0x60380018,  //  0059  GETGBL	R14	G24
      0x583C0014,  //  005A  LDCONST	R15	K20
      0x5C401600,  //  005B  MOVE	R16	R11
      0x88441315,  //  005C  GETMBR	R17	R9	K21
      0x88481316,  //  005D  GETMBR	R18	R9	K22
      0x884C1317,  //  005E  GETMBR	R19	R9	K23
      0x88501318,  //  005F  GETMBR	R20	R9	K24
      0x5C541800,  //  0060  MOVE	R21	R12
      0x5C581400,  //  0061  MOVE	R22	R10
      0x7C381000,  //  0062  CALL	R14	8
      0x583C000B,  //  0063  LDCONST	R15	K11
      0x7C340400,  //  0064  CALL	R13	2
      0x8C280119,  //  0065  GETMET	R10	R0	K25
      0x5C301200,  //  0066  MOVE	R12	R9
      0x7C280400,  //  0067  CALL	R10	2
      0x8828011A,  //  0068  GETMBR	R10	R0	K26
      0x8828151B,  //  0069  GETMBR	R10	R10	K27
      0x8828151C,  //  006A  GETMBR	R10	R10	K28
      0x8828151D,  //  006B  GETMBR	R10	R10	K29
      0x8C28151E,  //  006C  GETMET	R10	R10	K30
      0x5C301200,  //  006D  MOVE	R12	R9
      0x7C280400,  //  006E  CALL	R10	2
      0x80000000,  //  006F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compact
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_compact,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL, 
        1,                          /* has constants */
        ( &(const bvalue[ 3]) {     /* constants */
        /* K0   */  be_const_int(0),
        /* K1   */  be_nested_str_weak(compact),
        /* K2   */  be_const_int(1),
        }),
        be_str_weak(compact_queue),
        &be_const_str_solidified,
        ( &(const binstruction[12]) {  /* code */
          0x58040000,  //  0000  LDCONST	R1	K0
          0x6008000C,  //  0001  GETGBL	R2	G12
          0x5C0C0000,  //  0002  MOVE	R3	R0
          0x7C080200,  //  0003  CALL	R2	1
          0x14080202,  //  0004  LT	R2	R1	R2
          0x780A0004,  //  0005  JMPF	R2	#000B
          0x94080001,  //  0006  GETIDX	R2	R0	R1
          0x8C080501,  //  0007  GETMET	R2	R2	K1
          0x7C080200,  //  0008  CALL	R2	1
          0x00040302,  //  0009  ADD	R1	R1	K2
          0x7001FFF5,  //  000A  JMP		#0001
          0x80000000,  //  000B  RET	0
        })
      ),
    &be_class_Matter_EventHandler, 
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(queue_debug),
    /* K1   */  be_nested_str_weak(queue_info),
    /* K2   */  be_nested_str_weak(queue_critical),
    }),
    be_str_weak(compact),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x84040000,  //  0000  CLOSURE	R1	P0
      0x5C080200,  //  0001  MOVE	R2	R1
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x7C080200,  //  0003  CALL	R2	1
      0x5C080200,  //  0004  MOVE	R2	R1
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x7C080200,  //  0006  CALL	R2	1
      0x5C080200,  //  0007  MOVE	R2	R1
      0x880C0102,  //  0008  GETMBR	R3	R0	K2
      0x7C080200,  //  0009  CALL	R2	1
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_EventHandler
********************************************************************/
be_local_class(Matter_EventHandler,
    6,
    NULL,
    be_nested_map(19,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(queue_info, -1), be_const_var(1) },
        { be_const_key_weak(dump, 4), be_const_closure(class_Matter_EventHandler_dump_closure) },
        { be_const_key_weak(compact, -1), be_const_closure(class_Matter_EventHandler_compact_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_EventHandler_init_closure) },
        { be_const_key_weak(EVENT_NO_INCR, -1), be_const_int(1000) },
        { be_const_key_weak(load_event_no_persisted, 2), be_const_closure(class_Matter_EventHandler_load_event_no_persisted_closure) },
        { be_const_key_weak(EVENT_NO_KEY, -1), be_nested_str_weak(_matter_event_no) },
        { be_const_key_weak(EVENT_QUEUE_SIZE_MAX, -1), be_const_int(10) },
        { be_const_key_weak(counter_event_no, -1), be_const_var(4) },
        { be_const_key_weak(queue_debug, -1), be_const_var(0) },
        { be_const_key_weak(publish_event, -1), be_const_closure(class_Matter_EventHandler_publish_event_closure) },
        { be_const_key_weak(queue_event, -1), be_const_closure(class_Matter_EventHandler_queue_event_closure) },
        { be_const_key_weak(get_last_event_no, 18), be_const_closure(class_Matter_EventHandler_get_last_event_no_closure) },
        { be_const_key_weak(get_next_event_no, 3), be_const_closure(class_Matter_EventHandler_get_next_event_no_closure) },
        { be_const_key_weak(counter_event_no_persisted, 16), be_const_var(5) },
        { be_const_key_weak(device, 10), be_const_var(3) },
        { be_const_key_weak(find_min_no, -1), be_const_closure(class_Matter_EventHandler_find_min_no_closure) },
        { be_const_key_weak(queue_critical, -1), be_const_var(2) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_EventHandler_every_second_closure) },
    })),
    be_str_weak(Matter_EventHandler)
);

extern const bclass be_class_Matter_EventQueued;

/********************************************************************
** Solidified function: compact
********************************************************************/
extern const bclass be_class_Matter_EventQueued;
be_local_closure(class_Matter_EventQueued_compact,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventQueued, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(raw_tlv),
    }),
    be_str_weak(compact),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x90020001,  //  0001  SETMBR	R0	K0	R1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: to_raw_bytes
********************************************************************/
extern const bclass be_class_Matter_EventQueued;
be_local_closure(class_Matter_EventQueued_to_raw_bytes,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventQueued, 
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(raw_tlv),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(EventReportIB),
    /* K3   */  be_nested_str_weak(EventDataIB),
    /* K4   */  be_nested_str_weak(event_data),
    /* K5   */  be_nested_str_weak(EventPathIB),
    /* K6   */  be_nested_str_weak(endpoint),
    /* K7   */  be_nested_str_weak(cluster),
    /* K8   */  be_nested_str_weak(event),
    /* K9   */  be_nested_str_weak(event_id),
    /* K10  */  be_nested_str_weak(is_urgent),
    /* K11  */  be_nested_str_weak(path),
    /* K12  */  be_nested_str_weak(priority),
    /* K13  */  be_nested_str_weak(event_number),
    /* K14  */  be_nested_str_weak(event_no),
    /* K15  */  be_nested_str_weak(epoch_timestamp),
    /* K16  */  be_nested_str_weak(data),
    /* K17  */  be_nested_str_weak(TLV),
    /* K18  */  be_nested_str_weak(Matter_TLV_struct),
    /* K19  */  be_nested_str_weak(data0),
    /* K20  */  be_nested_str_weak(add_obj),
    /* K21  */  be_const_int(0),
    /* K22  */  be_nested_str_weak(data1),
    /* K23  */  be_const_int(1),
    /* K24  */  be_nested_str_weak(data2),
    /* K25  */  be_const_int(2),
    /* K26  */  be_nested_str_weak(to_TLV),
    /* K27  */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(to_raw_bytes),
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x7806003D,  //  0003  JMPF	R1	#0042
      0xB8060200,  //  0004  GETNGBL	R1	K1
      0x8C040302,  //  0005  GETMET	R1	R1	K2
      0x7C040200,  //  0006  CALL	R1	1
      0xB80A0200,  //  0007  GETNGBL	R2	K1
      0x8C080503,  //  0008  GETMET	R2	R2	K3
      0x7C080200,  //  0009  CALL	R2	1
      0x90060802,  //  000A  SETMBR	R1	K4	R2
      0xB80E0200,  //  000B  GETNGBL	R3	K1
      0x8C0C0705,  //  000C  GETMET	R3	R3	K5
      0x7C0C0200,  //  000D  CALL	R3	1
      0x88100106,  //  000E  GETMBR	R4	R0	K6
      0x900E0C04,  //  000F  SETMBR	R3	K6	R4
      0x88100107,  //  0010  GETMBR	R4	R0	K7
      0x900E0E04,  //  0011  SETMBR	R3	K7	R4
      0x88100109,  //  0012  GETMBR	R4	R0	K9
      0x900E1004,  //  0013  SETMBR	R3	K8	R4
      0x8810010A,  //  0014  GETMBR	R4	R0	K10
      0x900E1404,  //  0015  SETMBR	R3	K10	R4
      0x900A1603,  //  0016  SETMBR	R2	K11	R3
      0x8810010C,  //  0017  GETMBR	R4	R0	K12
      0x900A1804,  //  0018  SETMBR	R2	K12	R4
      0x8810010E,  //  0019  GETMBR	R4	R0	K14
      0x900A1A04,  //  001A  SETMBR	R2	K13	R4
      0x8810010F,  //  001B  GETMBR	R4	R0	K15
      0x900A1E04,  //  001C  SETMBR	R2	K15	R4
      0xB8120200,  //  001D  GETNGBL	R4	K1
      0x88100911,  //  001E  GETMBR	R4	R4	K17
      0x8C100912,  //  001F  GETMET	R4	R4	K18
      0x7C100200,  //  0020  CALL	R4	1
      0x900A2004,  //  0021  SETMBR	R2	K16	R4
      0x88100113,  //  0022  GETMBR	R4	R0	K19
      0x4C140000,  //  0023  LDNIL	R5
      0x20100805,  //  0024  NE	R4	R4	R5
      0x78120004,  //  0025  JMPF	R4	#002B
      0x88100510,  //  0026  GETMBR	R4	R2	K16
      0x8C100914,  //  0027  GETMET	R4	R4	K20
      0x58180015,  //  0028  LDCONST	R6	K21
      0x881C0113,  //  0029  GETMBR	R7	R0	K19
      0x7C100600,  //  002A  CALL	R4	3
      0x88100116,  //  002B  GETMBR	R4	R0	K22
      0x4C140000,  //  002C  LDNIL	R5
      0x20100805,  //  002D  NE	R4	R4	R5
      0x78120004,  //  002E  JMPF	R4	#0034
      0x88100510,  //  002F  GETMBR	R4	R2	K16
      0x8C100914,  //  0030  GETMET	R4	R4	K20
      0x58180017,  //  0031  LDCONST	R6	K23
      0x881C0116,  //  0032  GETMBR	R7	R0	K22
      0x7C100600,  //  0033  CALL	R4	3
      0x88100118,  //  0034  GETMBR	R4	R0	K24
      0x4C140000,  //  0035  LDNIL	R5
      0x20100805,  //  0036  NE	R4	R4	R5
      0x78120004,  //  0037  JMPF	R4	#003D
      0x88100510,  //  0038  GETMBR	R4	R2	K16
      0x8C100914,  //  0039  GETMET	R4	R4	K20
      0x58180019,  //  003A  LDCONST	R6	K25
      0x881C0118,  //  003B  GETMBR	R7	R0	K24
      0x7C100600,  //  003C  CALL	R4	3
      0x8C10031A,  //  003D  GETMET	R4	R1	K26
      0x7C100200,  //  003E  CALL	R4	1
      0x8C10091B,  //  003F  GETMET	R4	R4	K27
      0x7C100200,  //  0040  CALL	R4	1
      0x90020004,  //  0041  SETMBR	R0	K0	R4
      0x88040100,  //  0042  GETMBR	R1	R0	K0
      0x80040200,  //  0043  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_EventQueued;
be_local_closure(class_Matter_EventQueued_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    10,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventQueued, 
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(event_no),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(event_id),
    /* K4   */  be_nested_str_weak(is_urgent),
    /* K5   */  be_nested_str_weak(priority),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(EVENT_CRITICAL),
    /* K9   */  be_nested_str_weak(epoch_timestamp),
    /* K10  */  be_nested_str_weak(tasmota),
    /* K11  */  be_nested_str_weak(rtc),
    /* K12  */  be_nested_str_weak(utc),
    /* K13  */  be_const_int(1700000000),
    /* K14  */  be_nested_str_weak(config_time),
    /* K15  */  be_nested_str_weak(data0),
    /* K16  */  be_nested_str_weak(data1),
    /* K17  */  be_nested_str_weak(data2),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x90020604,  //  0003  SETMBR	R0	K3	R4
      0x90020805,  //  0004  SETMBR	R0	K4	R5
      0x90020A06,  //  0005  SETMBR	R0	K5	R6
      0x88280105,  //  0006  GETMBR	R10	R0	K5
      0x14281506,  //  0007  LT	R10	R10	K6
      0x782A0000,  //  0008  JMPF	R10	#000A
      0x90020B06,  //  0009  SETMBR	R0	K5	K6
      0x88280105,  //  000A  GETMBR	R10	R0	K5
      0xB82E0E00,  //  000B  GETNGBL	R11	K7
      0x882C1708,  //  000C  GETMBR	R11	R11	K8
      0x2428140B,  //  000D  GT	R10	R10	R11
      0x782A0002,  //  000E  JMPF	R10	#0012
      0xB82A0E00,  //  000F  GETNGBL	R10	K7
      0x88281508,  //  0010  GETMBR	R10	R10	K8
      0x90020A0A,  //  0011  SETMBR	R0	K5	R10
      0xB82A1400,  //  0012  GETNGBL	R10	K10
      0x8C28150B,  //  0013  GETMET	R10	R10	K11
      0x5830000C,  //  0014  LDCONST	R12	K12
      0x7C280400,  //  0015  CALL	R10	2
      0x9002120A,  //  0016  SETMBR	R0	K9	R10
      0x88280109,  //  0017  GETMBR	R10	R0	K9
      0x1428150D,  //  0018  LT	R10	R10	K13
      0x782A0004,  //  0019  JMPF	R10	#001F
      0xB82A1400,  //  001A  GETNGBL	R10	K10
      0x8C28150B,  //  001B  GETMET	R10	R10	K11
      0x5830000E,  //  001C  LDCONST	R12	K14
      0x7C280400,  //  001D  CALL	R10	2
      0x9002120A,  //  001E  SETMBR	R0	K9	R10
      0x90021E07,  //  001F  SETMBR	R0	K15	R7
      0x90022008,  //  0020  SETMBR	R0	K16	R8
      0x90022209,  //  0021  SETMBR	R0	K17	R9
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_EventQueued
********************************************************************/
be_local_class(Matter_EventQueued,
    11,
    NULL,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(endpoint, -1), be_const_var(0) },
        { be_const_key_weak(epoch_timestamp, -1), be_const_var(9) },
        { be_const_key_weak(compact, -1), be_const_closure(class_Matter_EventQueued_compact_closure) },
        { be_const_key_weak(init, 10), be_const_closure(class_Matter_EventQueued_init_closure) },
        { be_const_key_weak(data2, 11), be_const_var(7) },
        { be_const_key_weak(event_no, 3), be_const_var(8) },
        { be_const_key_weak(raw_tlv, -1), be_const_var(10) },
        { be_const_key_weak(is_urgent, 12), be_const_var(3) },
        { be_const_key_weak(data1, -1), be_const_var(6) },
        { be_const_key_weak(priority, 4), be_const_var(4) },
        { be_const_key_weak(event_id, -1), be_const_var(2) },
        { be_const_key_weak(to_raw_bytes, 1), be_const_closure(class_Matter_EventQueued_to_raw_bytes_closure) },
        { be_const_key_weak(cluster, -1), be_const_var(1) },
        { be_const_key_weak(data0, -1), be_const_var(5) },
    })),
    be_str_weak(Matter_EventQueued)
);
/********************************************************************/
/* End of solidification */
